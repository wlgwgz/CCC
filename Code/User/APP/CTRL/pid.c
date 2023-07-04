/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: pid.c
 文件描述: 简化版PID算法库
           为应用层提供PID计算接口，封装"PID算法"。
 创建人  : hzy
 创建日期: 2017.02.08
*******************************************************************************/

#define PIDT_GLOBALS
#include "includes.h"


#define AVG_E1_MAX  60   //记录e1偏差数组
#define AVG_E1_TIME 20   //记录e1偏差数组计算有效值此定义一定要小于AVG_E1_MAX
#define AVG_E1_NUM  5   //记录e1偏差数组临近数据的平均

//PID信息结构体
typedef struct
{
    //只写项
    PID_CTRL_ENUM control; //控制算法
    I16* pv;    //控制点
    I16* sv;    //目标值   
    I16 Ts;     //采样周期(100ms)
    I16 Kp;     //比例系数(0.1)
    I16 Ti;     //积分时间(100ms)
    I16 Td;     //微分时间(100ms)   
    I16 Kc;     //反作用比例带
    I16 Ai;     //变速积分常数(单位0.1)(范围10~100)
//    I16 oldset;    //用作设定温度变更的设定温度记录
    //只读项
    I16 Uk;     //PID作用值  
    //不可读写
    I16 e1;     //k时刻的偏差
    I16 e2;     //k-1时刻的偏差
    I16 e3;     //k-2时刻的偏差
    I16 e4;     //k-3时刻的偏差
    I16 e5;     //k-4时刻的偏差
    I16 ec1;    //k时刻的偏差变化率
    I16 ec2;    //k-1时刻的偏差变化率  
    I16 ec3;    //k-2时刻的偏差变化率  
    I16 ec4;    //k-3时刻的偏差变化率  
    I16 ec5;    //k-4时刻的偏差变化率  

    I32 avebuf[AVG_E1_MAX];     //偏差数组0:最近一次偏差
    I32 avemin;     //偏差数组内5个数据的最小平均值
    I32 avemax;     //偏差数组内5个数据的最小平均值
    I16 avemod;     //温度稳定不动变速计数值
    U16 ave_across_cnt; //穿过目标值计数
    I16 Ui1;     //温度稳定不动变速计数值
    I16 Ui2;     //温度稳定不动变速计数值

    I16 Up;     //比例项
    I16 Ui;     //积分项
    I16 Ud;     //微分项
    I16 Mp;     //比例项的余数
    I16 Mi;     //积分项的余数
    I16 Md;     //微分项的余数
    U08 cnt_pos;//正偏差记数
    U08 cnt_neg;//负偏差记数

    I16 FgMaxUi;     //变速积分项到达最大值标志

    U08 cnt_highold;//正偏差稳定时间记数
    U08 cnt_lowhold;//负偏差稳定时间记数
    
}PID_STRUCT;

static PID_STRUCT pid[PID_ID_MAX];
static U16 timer_sampling_100ms[PID_ID_MAX];

//PID信息结构体
typedef enum
{
    PID_OFF, //PID关闭
    PID_ON   //PID开启
}PID_ONOFF_ENUM;
static PID_ONOFF_ENUM pid_onoff[PID_ID_MAX];

//模块计时
//100ms计时
enum
{
    PID_TIMER_100MS_BAK,   //备用
    PID_TIMER_100MS_MAX
};
static U16 pid_timer_100ms[PID_TIMER_100MS_MAX];
//1s计时
enum
{
    PID_TIMER_1S_BAK,   //备用
    PID_TIMER_1S_MAX
};
static U16 pid_timer_1s[PID_TIMER_1S_MAX];





/*******************************************************************************
 函 数: pid_math_bound()
 功 能: 限定值的范围
 参 数: v--值; 
        lower--下边界; 
        upper--上边界; 
 返 回: 是否超界
 其 它: 空
*******************************************************************************/
static BOOL pid_math_bound(I16 *v, I16 lower, I16 upper)
{
    BOOL fg_over_bounds = TRUE; //超界标志
    
    if (*v > upper)         *v = upper;
    else if (*v < lower)    *v = lower;
    else                    fg_over_bounds = FALSE; //未超界
    
    return  fg_over_bounds;
}

/*******************************************************************************
 函 数: pid_math_divide()
 功 能: 利用上次余数进行除法运算
 参 数: a1--上次的余数(指针); 
        a2--被除数; 
        b--除数; 
 返 回: 商
 其 它: 空
*******************************************************************************/
static I16 pid_math_divide(I16 *a1, I32 a2, I32 b)
{
    I32 a;
    
    //除数不能为0
    if (b == 0) b = 1;

    a = (*a1) + a2;
    *a1 = (I16)a % b;   //余数
    return (a / b);     //商数
}
/*******************************************************************************
 函 数: pid_ui_aveflexible()
 功 能: 变速积分判定变速积分在偏差2℃以外才启用
 参 数: id--PID序号;updatafg 0:初始化 1:正常更新
 返 回: 空
 其 它: 实际温度信号有毛刺因此连续判断才合理
*******************************************************************************/
void pid_ui_aveflexible(PID_ID_ENUM id,U08 updatafg)
{
    I16 temp[AVG_E1_MAX];
//    static I16 getin_cnt[5]={0,0,0,0,0};  //进入计数次数
    U08 i=0,j=0;

    //更新偏差缓存
    for((i=AVG_E1_MAX-1);i>0;i--)
    {
        pid[id].avebuf[i] = pid[id].avebuf[i-1];
    }
    pid[id].avebuf[0]= pid[id].e1;
    //更新平均值
    for(i=0;i<AVG_E1_MAX-AVG_E1_NUM;i++)
    {
        temp[i] = 0;
        for(j=0;j<AVG_E1_NUM;j++)
        {
           temp[i] += pid[id].avebuf[i+j];
        }
    }
    //if(++getin_cnt[id]>=AVG_E1_TIME) //每TS计算一次
    {
        pid[id].avemin = temp[0];
        pid[id].avemax = temp[0];
        //求最小最大平均值
        for(i=0;i<AVG_E1_TIME;i++)
        {
            if(pid[id].avemin>temp[i]) pid[id].avemin = temp[i];
            if(pid[id].avemax<temp[i]) pid[id].avemax = temp[i];
        }
        pid[id].avemin = pid[id].avemin/AVG_E1_NUM;
        pid[id].avemax = pid[id].avemax/AVG_E1_NUM;
        if(((pid[id].e1>0)&&(pid[id].e2<=0))||((pid[id].e1<0)&&(pid[id].e2>=0))) //穿过设定目标值
        {
            pid[id].avemod = 0;   //加速清零
            if(pid[id].ave_across_cnt!=0x7fff) pid[id].ave_across_cnt++;   //穿过次数清零
        }
        else if((pid[id].avemax-pid[id].avemin)<val_ave_stand)
        {
            if(pid[id].avemod!=0x7ff) pid[id].avemod = pid[id].avemod + 1;
        }
        else
        {
            pid[id].avemod = 0;
        }
    }

    // 2018.08.13 zzl变速积分的启用不受偏差值的限定
	if(updatafg==0)
	{
        for(i=0;i<AVG_E1_MAX;i++)
        {
            pid[id].avebuf[i] = pid[id].e1;
        }
        pid[id].avemod = 0;
	}
	test_buf[0]=pid[PID_ID_EEV_1].ave_across_cnt;
	test_buf[1]=pid[PID_ID_EEV_2].ave_across_cnt;
	test_buf[2]=pid[PID_ID_EEV_1].avemod;
	test_buf[3]=pid[PID_ID_EEV_2].avemod;
	
}

/*******************************************************************************
 函 数: pid_ui_flexible()
 功 能: 变速积分判定变速积分在偏差2℃以外才启用
 参 数: id--PID序号;
 返 回: 空
 其 它: 实际温度信号有毛刺因此连续判断才合理
*******************************************************************************/
void pid_ui_flexible(PID_ID_ENUM id)
{
	if (pid[id].e1 > 20)		
	{
		if (pid[id].ec1 > 0)			/* 偏差为正且处于上升区间 */
		{
			if (pid[id].cnt_pos != 0xff)	pid[id].cnt_pos++;
		}
		else if ((pid[id].ec1 <= 0)&&(pid[id].ec2 <= 0)&&(pid[id].ec3 <= 0))		/* 下降，正偏差计数清零 */
		{
			pid[id].cnt_pos = 0;
		}
		pid[id].cnt_neg = 0;
	}
	else if (pid[id].e1 <-20)
	{
		if (pid[id].ec1 < 0)			/* 偏差为负且处于下降区间 */
		{
			if (pid[id].cnt_neg != 0xff)	pid[id].cnt_neg++;
		}
		else if ((pid[id].ec1 >= 0)&&(pid[id].ec2 >= 0)&&(pid[id].ec3 >= 0))		/* 上升，负偏差计数清零 */
		{
			pid[id].cnt_neg = 0;
		}
		pid[id].cnt_pos = 0;
	}
	else 
	{
		pid[id].cnt_neg = 0;
		pid[id].cnt_pos = 0;
	}
}

/*******************************************************************************
 函 数: pid_firstsampling()
 功 能: PID更新采样值首次进入
 参 数: id--PID序号; 
 返 回: 空
 其 它: 空
*******************************************************************************/
#define AVEGE  5 
void pid_firstsampling(PID_ID_ENUM id)
{
    pid[id].e1  = *pid[id].pv - *pid[id].sv;    //偏差
   // pid[id].ec1 = pid[id].e1 - pid[id].e2;      //偏差变化量
    pid[id].ec1 = 0;      //偏差变化量
    pid[id].ec2 = pid[id].ec1;
    pid[id].ec3 = pid[id].ec2;
    pid[id].ec4 = pid[id].ec3;
    pid[id].ec5 = pid[id].ec4;

    pid[id].e2  = pid[id].e1;  //
    pid[id].e3  = pid[id].e2;  //
    pid[id].e4  = pid[id].e3;  //
    pid[id].e5  = pid[id].e4;  //
}

/*******************************************************************************
 函 数: pid_sampling()
 功 能: PID更新采样值
 参 数: id--PID序号; 
 返 回: 空
 其 它: 空
*******************************************************************************/
#define AVEGE  5 
void pid_sampling(PID_ID_ENUM id)
{
    pid[id].ec5 = pid[id].ec4;
    pid[id].ec4 = pid[id].ec3;
    pid[id].ec3 = pid[id].ec2;
    pid[id].ec2 = pid[id].ec1;

    pid[id].e5  = pid[id].e4;  //
    pid[id].e4  = pid[id].e3;  //
    pid[id].e3  = pid[id].e2;  //
    pid[id].e2  = pid[id].e1;  //


    pid[id].e1  = *pid[id].pv - *pid[id].sv;    //偏差
    pid[id].ec1 = pid[id].e1 - pid[id].e2;      //偏差变化量

  //  pid[id].e5  = pid[id].e4;  //
  //  pid[id].e4  = pid[id].e3;  //
  //  pid[id].e3  = pid[id].e2;  //
  //  pid[id].e2  = pid[id].e1;  //

    
    pid_ui_aveflexible(id,1);
    pid_ui_flexible(id);
}

/*******************************************************************************
 函 数: pid_control_fuzzy()
 功 能: 模糊式算法
 参 数: id--PID序号; 
 返 回: 空
 其 它: 原始列表中每1度的偏差和偏差变化率都对应1个单位的变化;
*******************************************************************************/
void pid_control_fuzzy(PID_ID_ENUM id)
{
    #define PID_FUZZY_RANGE     21  //模糊可调范围
    //若要使用模糊式，以下3个宏定义最好改为参数，加入到PID_STRUCT中
    #define FUZZY_A			10		//E 的比例因子 
    #define FUZZY_B			10		//EC的比例因子 
    #define FUZZY_U			10		//Uk的比例因子
    
    const I08 UkMap[PID_FUZZY_RANGE][PID_FUZZY_RANGE] = 
    {
        {   -20, -19, -18, -17, -16, -15, -14, -13, -12, -11, -10,  -9,  -8,  -7,  -6,  -5,  -4,  -3,  -2,  -1,   0   },
        {   -19, -18, -17, -16, -15, -14, -13, -12, -11, -10,  -9,  -8,  -7,  -6,  -5,  -4,  -3,  -2,  -1,   0,   1   },
        {   -18, -17, -16, -15, -14, -13, -12, -11, -10,  -9,  -8,  -7,  -6,  -5,  -4,  -3,  -2,  -1,   0,   1,   2   },
        {   -17, -16, -15, -14, -13, -12, -11, -10,  -9,  -8,  -7,  -6,  -5,  -4,  -3,  -2,  -1,   0,   1,   2,   3   },
        {   -16, -15, -14, -13, -12, -11, -10,  -9,  -8,  -7,  -6,  -5,  -4,  -3,  -2,  -1,   0,   1,   2,   3,   4   },
        {   -15, -14, -13, -12, -11, -10,  -9,  -8,  -7,  -6,  -5,  -4,  -3,  -2,  -1,   0,   1,   2,   3,   4,   5   },
        {   -14, -13, -12, -11, -10,  -9,  -8,  -7,  -6,  -5,  -4,  -3,  -2,  -1,   0,   1,   2,   3,   4,   5,   6   },
        {   -13, -12, -11, -10,  -9,  -8,  -7,  -6,  -5,  -4,  -3,  -2,  -1,   0,   1,   2,   3,   4,   5,   6,   7   },
        {   -12, -11, -10,  -9,  -8,  -7,  -6,  -5,  -4,  -3,  -2,  -1,   0,   1,   2,   3,   4,   5,   6,   7,   8   },
        {   -11, -10,  -9,  -8,  -7,  -6,  -5,  -4,  -3,  -2,  -1,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9   },
        {   -10,  -9,  -8,  -7,  -6,  -5,  -4,  -3,  -2,  -1,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10   },
        {    -9,  -8,  -7,  -6,  -5,  -4,  -3,  -2,  -1,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11   },
        {    -8,  -7,  -6,  -5,  -4,  -3,  -2,  -1,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12   },
        {    -7,  -6,  -5,  -4,  -3,  -2,  -1,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13   },
        {    -6,  -5,  -4,  -3,  -2,  -1,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14   },
        {    -5,  -4,  -3,  -2,  -1,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15   },
        {    -4,  -3,  -2,  -1,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16   },
        {    -3,  -2,  -1,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17   },
        {    -2,  -1,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18   },
        {    -1,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19   },
        {     0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20   },
    };
    
    I16 x, y;
    I16 tmp;
    
    tmp = pid[id].e1 * FUZZY_A / 10;
    x = (tmp + 105) / 10;
    if (x < 0)
    {
        x = 0;
    }
    if (x >= PID_FUZZY_RANGE)
    {
        x = PID_FUZZY_RANGE - 1;
    }
    
    tmp = pid[id].ec1 * FUZZY_B / 10;
    y = (tmp + 105) / 10;
    if (y < 0)
    {
        y = 0;
    }
    if (y >= PID_FUZZY_RANGE)
    {
        y = PID_FUZZY_RANGE - 1;
    }
    
    pid[id].Uk += (I16)UkMap[x][y] * FUZZY_U / 10;
    pid_math_bound(&pid[id].Uk, -pid[id].Kc, pid[id].Kc);
}

/*******************************************************************************
 函 数: pid_control_position()
 功 能: 位置式算法
 参 数: pid--PID序号; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void pid_control_position(PID_ID_ENUM id)
{
    I32 tmp;
    I16 max, min;
    
    max = pid[id].Kc;   
    min = -pid[id].Kc;
    
    
    if (pid_math_bound(&pid[id].e1, min, max))
    {
        pid[id].Up = pid[id].e1;
        pid[id].Ui = 0;
        pid[id].Mi = 0;
        pid[id].Ud = 0;
        pid[id].Md = 0;   
    }
    else 
    {
        /* 比例项 */
        pid[id].Up = pid_math_divide(&pid[id].Mp, (I32)pid[id].Kp * pid[id].e1, 10);
        
        /* 积分项 */
        if (pid[id].Ti == 0)
        {
            pid[id].Ui = 0;
            pid[id].Mi = 0;
        }
        else 
        {
            tmp = (I32)pid[id].e1 * pid[id].Ts;
		    if (pid->cnt_pos > 10 || pid->cnt_neg > 10)	tmp = tmp * pid->Ai / 10;
            pid[id].Ui += pid_math_divide(&pid[id].Mi, tmp, 10*pid[id].Ti);
            max -= pid[id].e1;
            min -= pid[id].e1;
            if (pid[id].Ui > max) pid[id].Ui = max;
            if (pid[id].Ui < min) pid[id].Ui = min;
        }
        
        /* 微分项 */
        if (pid[id].Td == 0)
        {
            pid[id].Ud = 0;
            pid[id].Md = 0;
        }
        else
        {
            tmp = pid[id].ec1 * pid[id].Td;
            pid[id].Ud = pid_math_divide(&pid[id].Md, tmp, pid[id].Ts);
            max -= pid[id].Ui;
            min -= pid[id].Ui;
            if (pid[id].Ud > max) pid[id].Ud = max;
            if (pid[id].Ud < min) pid[id].Ud = min;
        }
    }
    
    pid[id].Uk = pid[id].Up + pid[id].Ui + pid[id].Ud;
    pid_math_bound(&pid[id].Uk, 0, pid[id].Kc);
}

/*******************************************************************************
 函 数: pid_control_increment()
 功 能: 增量式算法
 参 数: pid--PID序号; 
 返 回: 空
 其 它: 空
*******************************************************************************/
#define DISABLE_D_V  5

void pid_control_increment(PID_ID_ENUM id)
{
    I32 tmp,ec1,ec2,tmp2,tmp3;
//    static U16 temp_getin_cnt[5]={0,0,0,0,0};

    tmp2 = 0;
    tmp3 = 0;
    /* 比例项 */
     pid[id].Up = pid_math_divide(&pid[id].Mp, (I32)pid[id].Kp * pid[id].ec1, 10);
     pid[id].FgMaxUi = 0;
    /* 积分项 */
    if (pid[id].Ti == 0)
    {
        pid[id].Ui = 0;
        pid[id].Mi = 0;
    }
    else 
    {
        tmp = (I32)pid[id].e1 * pid[id].Ts;
		if (pid[id].cnt_pos > 10 || pid[id].cnt_neg > 10)	tmp2 = (I32)pid[id].Ai*tmp / 10; //方向变速
		if (pid[id].avemod >50 )     //50倍的TS
		{
            
		    pid[id].avemod = MIN(pid[id].avemod, 450);
            tmp3  = (I32)pid[id].avemod*tmp / 30;    // 变速

    		tmp3  = (I32)pid[id].Ai*tmp3 / 10;    // 稳定性变速
		}

        pid[id].Ui1 = tmp2/100; //减小100倍方便监控20180813
        pid[id].Ui2 = tmp3/100; //减小100倍方便监控20180813
        tmp = tmp+tmp2+tmp3;
        pid[id].Ui = pid_math_divide(&(pid[id].Mi), tmp, 10*pid[id].Ti);
    }
    
    /* 微分项 */
    if (pid[id].Td == 0)
    {
        pid[id].Ud = 0;
        pid[id].Md = 0;
    }
    else
    {
        if((pid[id].e1>(-DISABLE_D_V)) && (pid[id].e1<DISABLE_D_V))//WLG V100A01 偏差在0.5内不微分
        {
            ec1 = 0;
            ec2 = 0;
        }
        else
        {
            ec1 = pid[id].ec1;
            ec2 = pid[id].ec2;
        }
        if(val_pid_d_type==0) //无低通滤波
        {
            tmp = (I32)pid[id].Td * (ec1-ec2);
            pid[id].Ud = pid_math_divide(&pid[id].Md, tmp, pid[id].Ts); 
    	}
        else
        {
            tmp = (I32)(4*(ec1-ec2) + pid[id].Ud) * pid[id].Td * 2;
    		pid[id].Ud = pid_math_divide(&pid[id].Md, tmp, (4*pid[id].Ts + 10*pid[id].Td)); //此处还需要推敲zzl 2018.04.10
        }
    }
    
    pid[id].Uk += pid[id].Up + pid[id].Ui + pid[id].Ud;
    pid_math_bound(&pid[id].Uk, -pid[id].Kc, pid[id].Kc);

    //NOTE-CXW PID调试
    DebugPID[DEBUG_UK]=pid[0].Uk;
    DebugPID[DEBUG_UP]=pid[0].Up;
    DebugPID[DEBUG_UI]=pid[0].Ui;
    DebugPID[DEBUG_UD]=pid[0].Ud;
    DebugPID[DEBUG_PID_E1]=pid[0].e1;
    DebugPID[DEBUG_PID_EC1]=pid[0].ec1;
    DebugPID[DEBUG_PID_EC2]=pid[0].ec2;
    DebugPID[DEBUG_PID_1UI]=pid[0].Ui1;
    DebugPID[DEBUG_PID_2UI]=pid[0].Ui2;


    DebugEEV_ID_2PID[DEBUG_UK]=pid[1].Uk;
    DebugEEV_ID_2PID[DEBUG_UP]=pid[1].Up;
    DebugEEV_ID_2PID[DEBUG_UI]=pid[1].Ui;
    DebugEEV_ID_2PID[DEBUG_UD]=pid[1].Ud;
    DebugEEV_ID_2PID[DEBUG_PID_E1]=pid[1].e1;
    DebugEEV_ID_2PID[DEBUG_PID_EC1]=pid[1].ec1;
    DebugEEV_ID_2PID[DEBUG_PID_EC2]=pid[1].ec2;
    DebugEEV_ID_2PID[DEBUG_PID_1UI]=pid[1].Ui1;
    DebugEEV_ID_2PID[DEBUG_PID_2UI]=pid[1].Ui2;



    DebugINVT_ID_1PID[DEBUG_UK]=pid[PID_ID_INVT_1].Uk;
    DebugINVT_ID_1PID[DEBUG_UP]=pid[PID_ID_INVT_1].Up;
    DebugINVT_ID_1PID[DEBUG_UI]=pid[PID_ID_INVT_1].Ui;
    DebugINVT_ID_1PID[DEBUG_UD]=pid[PID_ID_INVT_1].Ud;
    DebugINVT_ID_1PID[DEBUG_PID_E1]=pid[PID_ID_INVT_1].e1;
    DebugINVT_ID_1PID[DEBUG_PID_EC1]=pid[PID_ID_INVT_1].ec1;
    DebugINVT_ID_1PID[DEBUG_PID_EC2]=pid[PID_ID_INVT_1].ec2;
    DebugINVT_ID_1PID[DEBUG_PID_1UI]=pid[PID_ID_INVT_1].Ui1;
    DebugINVT_ID_1PID[DEBUG_PID_2UI]=pid[PID_ID_INVT_1].Ui2;



    
}

/*******************************************************************************
 函 数: pid_timer()
 功 能: 模块计时函数
 参 数: BOOL TimeFlag 时间标志（用参数传递，以免移植时100ms标志名称不同）
 返 回: 空
 其 它: 在100ms计时处理处调用
*******************************************************************************/
void pid_timer(BOOL TimeFlag)
{
    U08 i;
    static U08 cnt_100ms_to_1s = 0; //100毫秒换算成1秒

    if(TimeFlag)
    {
        for (i=0; i<PID_TIMER_100MS_MAX; i++)
        {
            if (pid_timer_100ms[i] != 0xffff) pid_timer_100ms[i]++;
        }
        
        for (i=0; i<PID_ID_MAX; i++)
        {
            if (timer_sampling_100ms[i] != 0xffff) timer_sampling_100ms[i]++;
        }
        
        if (++cnt_100ms_to_1s >= 10)
        {
            cnt_100ms_to_1s = 0;
            for (i=0; i<PID_TIMER_1S_MAX; i++)
            {
                if (pid_timer_1s[i] != 0xffff) pid_timer_1s[i]++;
            }
        }
    }
}

/*******************************************************************************
 函 数: pid_init()
 功 能: pid参数初始化
 参 数: 空
 返 回: 空
 其 它: 应用程序需要在此初始化之后才能设置pid参数，否则会被初始化覆盖
*******************************************************************************/
void pid_init(void)
{
    U08 i;
    static I16 value = 0;
    
    for (i=0; i<PID_ID_MAX; i++)
    {
        pid_onoff[i] = PID_OFF;//关闭
        pid[i].control = PID_CTRL_MAX;//无效算法
        pid[i].pv = &value;//控制值
        pid[i].sv = &value;//目标值
        pid[i].Kc = 1000;//反作用比例带
        pid[i].Ts = 10;//采样周期(100ms)
        pid[i].Kp = 1;//比例系数
        pid[i].Ti = 100;//积分时间
        pid[i].Td = 1;//微分时间
        pid[i].e1 = 0;//k时刻的偏差
        pid[i].e2 = 0;//k-1时刻的偏差
        pid[i].ec1 = 0;//k时刻的偏差变化率
        pid[i].ec2 = 0;//k-1时刻的偏差变化率
        pid[i].Up = 0;//比例项
        pid[i].Ui = 0;//积分项
        pid[i].Ud = 0;//微分项
        pid[i].Mp = 0;//比例项的余数
        pid[i].Mi = 0;//积分项的余数
        pid[i].Md = 0;//微分项的余数
        pid[i].Uk = 0;//PID作用值
        pid[i].Ai = 10;//变速积分常数
        pid[i].cnt_pos = 0;//正偏差记数
        pid[i].cnt_neg = 0;//负偏差记数
        pid_ui_aveflexible((PID_ID_ENUM)i,0);
        pid_set_AveAcrossCnt((PID_ID_ENUM)i,0);
    }
}

/*******************************************************************************
 函 数: pid_prog()
 功 能: pid主调度程序
 参 数: 空
 返 回: 空
 其 它: 在主循环中调用
*******************************************************************************/
void pid_prog(void)
{
    U08 i;
    U08 tmp=0;
    PID_ID_ENUM p_id;
    for (i=0; i<PID_ID_MAX; i++)
    {
        p_id = (PID_ID_ENUM)i;
        
        if (pid_onoff[i] == PID_ON)
        {
            tmp++;
        }

        /* 只有主阀且关阀需求才使用加速逻辑 */
        if (i != PID_ID_EEV_1
        ||  pid[i].ave_across_cnt>AVG_ACROSS_NUM
        ||  pid[i].e1>0
        ||  CM_IS_UP(INVT_ID_1)				/* 要确保每个循环都有执行 */
            )
        {
            pid[i].avemod = 0;
        }

        if (timer_sampling_100ms[i] >= pid[i].Ts)
        {
            timer_sampling_100ms[i] = 0;

            if (pid_onoff[i] == PID_ON)
            {
                pid_sampling(p_id);
                
                switch (pid[i].control)
                {
                    case PID_CTRL_POSITION:
                        pid_control_position(p_id);
                        break;
                        
                    case PID_CTRL_INCREMENT:
                        pid_control_increment(p_id);
                        break;
                        
                    case PID_CTRL_FUZZY:
                        pid_control_fuzzy(p_id);
                        break;
                        
                    default:
                        break;
                }   
            }
            DebugPID[DEBUG_PID_USE] = pid_onoff[0];
            DebugEEV_ID_2PID[DEBUG_PID_USE] = pid_onoff[1];
            DebugINVT_ID_1PID[DEBUG_PID_USE] = pid_onoff[PID_ID_INVT_1];
        }
    }
    if(tmp != 0)
    {
        pid_environ_aveflexible(pv_sys_TA,1);
    }
    else
    {
        pid_environ_aveflexible(pv_sys_TA,0);
    }

}

/*******************************************************************************
 函 数: pid_read_Uk()
 功 能: 读取PID作用值
 参 数: id--PID序号; 
 返 回: PID作用值
 其 它: 增量式或模糊式的作用值在每次读取后会清零
*******************************************************************************/
I16 pid_read_Uk(PID_ID_ENUM id)
{
    I16 Uk = 0;

    if (id < PID_ID_MAX)
    {
        Uk = pid[id].Uk;
        
        if (pid[id].control == PID_CTRL_INCREMENT
         || pid[id].control == PID_CTRL_FUZZY)
        {
            pid[id].Uk = 0;
        }
    }
    return Uk;
}
/*******************************************************************************
 函 数: pid_read_FgMaxUi()
 功 能: 读取PID作用值变速积分积累到最大值标志
 参 数: id--PID序号; 
 返 回: 1:变速积分到达最大值  0:变速积分没有到达最大值
 其 它: 增量式或模糊式的作用值在每次读取后会清零
*******************************************************************************/
I16 pid_read_FgMaxUi(PID_ID_ENUM id)
{
    I16 tmp = 0;

    if (id < PID_ID_MAX)
    {
       tmp=pid[id].FgMaxUi;        
    }
    return tmp;
}


/*******************************************************************************
 函 数: pid_turn_on()
 功 能: 开启PID
 参 数: id--PID序号;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL pid_turn_on(PID_ID_ENUM id)
{
    BOOL result = TRUE;

    if (id >= PID_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        if(pid_onoff[id] == PID_OFF)//从关到开预采一次样，避免偏差变化率虚高
        {
            pid_firstsampling(id);
            pid[id].Uk = 0;
            pid_ui_aveflexible(id,0);
            pid_set_AveAcrossCnt(id,0);
        }
        pid_onoff[id] = PID_ON;
    }

    return result;
}

/*******************************************************************************
 函 数: pid_turn_off()
 功 能: 关闭PID
 参 数: id--PID序号;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL pid_turn_off(PID_ID_ENUM id)
{
    BOOL result = TRUE;

    if (id >= PID_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        pid_onoff[id] = PID_OFF;
        pid_ui_aveflexible(id,0);
        pid_set_AveAcrossCnt(id,0);
    }

    return result;
}

/*******************************************************************************
 函 数: pid_set_ctrl()
 功 能: 设置控制算法
 参 数: id--PID序号; 
        value--算法; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL pid_set_ctrl(PID_ID_ENUM id, PID_CTRL_ENUM value)
{
    BOOL result = TRUE;

    if (id >= PID_ID_MAX || value >= PID_CTRL_MAX)
    {
        result = FALSE;
    }
    else
    {
        pid[id].control = value;//控制算法
    }

    return result;
}

/*******************************************************************************
 函 数: pid_set_pv()
 功 能: 设置控制源
 参 数: id--PID序号; 
        value--控制点指针; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL pid_set_pv(PID_ID_ENUM id, I16* p_pv)
{
    BOOL result = TRUE;
    
    if (id >= PID_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        pid[id].pv = p_pv;//控制点
    }
    return result;
}

/*******************************************************************************
 函 数: pid_set_sv()
 功 能: 设置目标值
 参 数: id--PID序号; 
        value--目标值指针; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL pid_set_sv(PID_ID_ENUM id, I16* p_sv)
{
    BOOL result = TRUE;
    
    if (id >= PID_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        pid[id].sv = p_sv;//目标值

#if 0
        if(pid[id].oldset != *p_sv)
        {
            if(id!=PID_ID_EEV_2)
            {
                pid_set_AveAcrossCnt(id,0);//没有开启时重新计数
            }
        }
        pid[id].sv = p_sv;//目标值
        pid[id].oldset = *p_sv;
#endif
        
    }
    
    return result;
} 

/*******************************************************************************
 函 数: pid_set_Ts()
 功 能: 设置采样周期
 参 数: id--PID序号; 
        value--采样周期(100ms); 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL pid_set_Ts(PID_ID_ENUM id, I16 value)
{
    BOOL result = TRUE;
    
    if (id >= PID_ID_MAX || value == 0)
    {
        result = FALSE;
    }
    else
    {
        pid[id].Ts = value;//采样周期
    }

    return result;
}

/*******************************************************************************
 函 数: pid_set_Kp()
 功 能: 设置比例系数
 参 数: id--PID序号; 
        value--比例系数; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL pid_set_Kp(PID_ID_ENUM id, I16 value)
{
    BOOL result = TRUE;
    
    if (id >= PID_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        pid[id].Kp = value;//比例系数
    }

    return result;
}

/*******************************************************************************
 函 数: pid_set_Ti()
 功 能: 设置积分时间
 参 数: id--PID序号; 
        value--积分时间(100ms); 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL pid_set_Ti(PID_ID_ENUM id, I16 value)
{
    BOOL result = TRUE;
    
    if (id >= PID_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        pid[id].Ti = value;//积分时间
    }

    return result;
}

/*******************************************************************************
 函 数: pid_set_Td设置微分时间()
 功 能: 设置微分时间
 参 数: id--PID序号; 
        value--微分时间(100ms); 
 返 回: TRUE/FALSE--成功/失败
 其 它: 微分时间设为0表示不使用微分
*******************************************************************************/
BOOL pid_set_Td(PID_ID_ENUM id, I16 value)
{
    BOOL result = TRUE;
    
    if (id >= PID_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        pid[id].Td = value;//微分时间
    }

    return result;
}

/*******************************************************************************
 函 数: pid_set_Kc()
 功 能: 设置反作用比例带
 参 数: id--PID序号; 
        value--比例带大小(单边); 
 返 回: TRUE/FALSE--成功/失败
 其 它: 此"反作用比例带"用作限定作用值Uk大小(位置式)或UK变化量(增量式模糊式)
*******************************************************************************/
BOOL pid_set_Kc(PID_ID_ENUM id, I16 value)
{
    BOOL result = TRUE;
    
    if (id >= PID_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        pid[id].Kc = value;//反作用比例带
    }

    return result;
}

/*******************************************************************************
 函 数: pid_get_Kc()
 功 能: 获取反作用比例带
 参 数: id--PID序号; 
 返 回: 比例带
 其 它: 
*******************************************************************************/
I16 pid_get_Kc(PID_ID_ENUM id)
{
    I16 result = 0;
    
    if (id >= PID_ID_MAX)
    {
        result = 0;
    }
    else
    {
        result = pid[id].Kc;//反作用比例带
    }

    return result;
}


/*******************************************************************************
 函 数: pid_set_Ai()
 功 能: 设置变速积分常数
 参 数: id--PID序号; 
        value--变速积分常数; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 单位0.1，范围10~100
*******************************************************************************/
BOOL pid_set_Ai(PID_ID_ENUM id, I16 value)
{
    #define MIN_AI  0   //Ai最小值
    #define MAX_AI  100  //Ai最大值
    
    BOOL result = TRUE;
    
    if (id >= PID_ID_MAX || value < MIN_AI || value > MAX_AI)
    {
        result = FALSE;
    }
    else
    {
        pid[id].Ai = value;//变速积分常数
    }

    return result;
}
/*******************************************************************************
 函 数: pid_set_AveAcrossCnt()
 功 能: 设置穿过设定温度次数
 参 数: id--PID序号; 
        value--变速积分常数; 
 返 回: TRUE/FALSE--成功/失败
 其 它:
*******************************************************************************/
BOOL pid_set_AveAcrossCnt(PID_ID_ENUM id, I16 value)
{
    
    BOOL result = TRUE;
    
    if (id >= PID_ID_MAX )
    {
        result = FALSE;
    }
    else
    {
        pid[id].ave_across_cnt = value;//
    }

    return result;
}

U16 pid_get_AveAcrossCnt(PID_ID_ENUM id)
{
    return pid[id].ave_across_cnt;
}

U16 pid_get_avemod(PID_ID_ENUM id)
{
    return pid[id].avemod;
}
/*******************************************************************************
 函 数: pid_environ_aveflexible()
 功 能: 根据环境温度是否剧烈波动判断是否要重新加速控制
               判断温度剧烈波动依据环境温度在10分钟内波动了5℃及以上
               主要是适应客户在实验室测试时环温测试条件的变化
 参 数: judgetemp  当前环境温度单位0.1℃; 
              updatafg:0初始化  1:正常更新
 返 回:环境温度是否满足重新加速条件
 其 它:  
*******************************************************************************/
#define AVG_ENVIRON_DIF 50     //环境温度偏差值单位0.1℃
#define AVG_ENVIRON_TIME 120   //环境温度记录时间间隔次数单位AVG_ENVIRON_SPACE
#define AVG_ENVIRON_SPACE 5    //环境温度记录时间间隔单位1秒

void pid_environ_aveflexible(I16 judgetemp,U08 updatafg)
{
//    I16 temp[AVG_E1_MAX];
//    U16 tmp_time;
//    static I16 getin_cnt[5]={0,0,0,0,0};  //进入计数次数
    static I16 environ_buf[AVG_ENVIRON_TIME];
           I16 environ_max,environ_min;

    U08 i=0;
    static U16 tmp_upfg=0;  //需更新
    static U16 tmp_upfgbak=0xff;//更新备份

    tmp_upfg = ((USER_GetTime(C_1S)%AVG_ENVIRON_SPACE)==0)?1:0;
    
    if((tmp_upfgbak!=tmp_upfg)&&(updatafg==1)) //间隔存储
    {
        tmp_upfgbak=tmp_upfg;
        if(tmp_upfg)
        {
            //更新偏差缓存
            for((i=AVG_ENVIRON_TIME-1);i>0;i--)
            {
                environ_buf[i] = environ_buf[i-1];
            }
            environ_buf[0]= judgetemp;
            environ_max = environ_buf[0];
            environ_min = environ_buf[0];
            //求最小最大值
            for(i=0;i<AVG_ENVIRON_TIME;i++)
            {
                if(environ_min>environ_buf[i]) environ_min = environ_buf[i];
                if(environ_max<environ_buf[i]) environ_max = environ_buf[i];
            }
            //判断
            if((environ_max-environ_min)>AVG_ENVIRON_DIF)
            {
                pid_set_AveAcrossCnt(PID_ID_EEV_1,0);
                pid_set_AveAcrossCnt(PID_ID_EEV_2,0);
                pid_set_AveAcrossCnt(PID_ID_INVT_1,0);
            }
        }
    }
    // 
	if(updatafg==0)
	{
	    tmp_upfg = 0;
	    tmp_upfgbak=0xff;
        for(i=0;i<AVG_ENVIRON_TIME;i++)
        {
            environ_buf[i] = judgetemp;
        }
	}
}

