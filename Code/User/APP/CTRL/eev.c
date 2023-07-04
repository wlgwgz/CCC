/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: eev.c
 文件描述: 电子膨胀阀状态控制
 创建人  : hzy
 创建日期: 2017.02.13
*******************************************************************************/

#define EEV_GLOBALS
#include "includes.h"

#define EEV_NORMAL_CTRL         0       // 正常调节
#define EEV_FORCED_INCREASE     1       // 强制增大阀开度
#define EEV_FORCED_DECREASE     2       // 强制减小阀开度
#define EEV_FORCED_KEEP         3       // 保持阀开度（强制调节过程中）



#define EEV_RAPID_TIME_1    150     /* 快速调阀前延时1 */
#define EEV_RAPID_TIME_2    240     /* 快速调阀前延时2 */
#define EEV_RAPID_AREA      25      /* 快速调阀区间单位温度 */




typedef struct
{
    //非读写项
    EEV_STATE_ENUM state;   //电膨状态
    U16 timer_preopen;      //预开计时(100ms)
    U16 timer_calculate;    //预开计时(100ms)
    U16 timer_action;       //动作计时(100ms)
    U16 timer_add;       //强制开大动作计时(100ms)
    U16 timer_sub;       //强制开小动作计时(100ms)
    U16 timer_hold;       //维持当前开度动作时间计时(1秒)
    U16 timer_fastadd;       //快速强制开大动作计时(100ms)
    U16 timer_initchange;       //初始开度跳转前计时(100ms)

    U16 cnt_positive;       //正向计数
    U16 cnt_negative;       //反向计数
    U16 step_sv;            //目标步数
    U16 step_pv;            //当前步数
    I16 pid_sv;             //控制点目标值
    I16 pid_pv;             //控制点当前值引用此变量时已经的PID最小运行单位
    //只写项
    U16 steps_idle;         //待机步数
    U16 steps_preopen;      //预开步数
    U16 steps_defrost;      //除霜步数
    U16 steps_max;          //总步数
    U16 steps_run_max;      //最大运行步数
    U16 steps_run_min;      //最小步数
    U16 steps_add;          //
    U16 steps_sub;          //
    U16 period_preopen;     //预开时间(100ms)
    U16 period_calculate;   //计算周期(100ms)
    U16 period_action;      //动作周期(100ms)  
    U16 period_add;      //强制开大动作周期
    U16 period_sub;      //强制开小动作周期

    
    U16 period_hold;      //维持当前开度动作时间(单位秒)
    U16 period_cyc;      //维持当前开度动作周期(单位秒)

//    //只读项
    U16 motor_fre;          //励磁频率(序号)
    U16 motor_type;         //励磁方式
}EEV_STEPS_STRUCT;

EEV_STEPS_STRUCT eev[EEV_ID_MAX];

typedef struct
{
    U16 adj_min;        // 调节下限
    U16 adj_max;        // 调节上限
    U16 adj_step;       // 调节步长
}ADJUST_STRUCT;

//ADJUST_STRUCT eev_adj_gas[EEV_ID_MAX];  // 用于排气强制调节 


//模块计时
//100ms计时
enum
{
    EEV_TIMER_100MS_BAK,   //备用
    EEV_TIMER_100MS_MAX
};
static U16 eev_timer_100ms[EEV_TIMER_100MS_MAX];
//1s计时
enum
{
    EEV_TIMER_1S_BAK,   //备用
    EEV_TIMER_1S_MAX
};
static U16 eev_timer_1s[EEV_TIMER_1S_MAX];


/*==========================================
    功能: 余数a1 = (a1+a2) % b  
          返回商数 = (a1+a2) / b
    利用上次余数进行除法运算
/==========================================*/
I32 eev_math_divide(I32 *a1, I32 a2, I32 b)
{
    I32 a;

    if (b == 0) b = 1;
    
    a = (*a1) + a2;
    *a1 = a % b;    //余数
    return (a / b); //商数
}

///*==========================================
//  功能: 加速处理
///==========================================*/
void eev_speedup(EEV_ID_ENUM id, I16 diff)
{
    if (diff > EEV_RAPID_AREA)
    {
        eev[id].cnt_positive++;
        eev[id].cnt_negative = 0;
    }
    else if (diff < -EEV_RAPID_AREA)
    {
        eev[id].cnt_negative++;
        eev[id].cnt_positive = 0;
    }
    else 
    {
        eev[id].cnt_positive = 0;
        eev[id].cnt_negative = 0;
    }

    if(EEV_RAPID_AREA==0) //为0则清除计时
    {
        eev[id].cnt_positive = 0;
        eev[id].cnt_negative = 0;
    }
    
}

/*******************************************************************************
 函 数: eev_increment_adjust() （原名pid_valve_out）
 功 能: 变化量调整
 参 数: id--电膨序号; 
        increment--增量; 
 返 回: 调整后的增量
 其 它: 根据当前步数，调整增量值，如当前步数较小，则每次变化量适当减小，当前步
        数较大，则每次变化量适当增大。
*******************************************************************************/
I16 eev_increment_adjust(PID_ID_ENUM p_id, EEV_ID_ENUM id, I16 increment)
{
    static I32 remainder[EEV_ID_MAX] = {0,0};
    I16 dest_val = 0;
    U16 calcu_pv = 1;           //参与计算的当前值
    I32 denominator = 1;        //分母
    U16 steps_ref_min = 0;      //最小参考步数（用于防止当前开度过低时开阀缓慢） 
    I16 pid_kc;
        
    steps_ref_min = eev[id].steps_max / 4;
    if (steps_ref_min > eev[id].steps_run_max)     
    {
        steps_ref_min = eev[id].steps_run_max;
    }
        
    calcu_pv = GET_EXIO_ST_PV(0, id);
    if (calcu_pv < (eev[id].steps_max / 25))    // 什么原理zzl 2018.04.08 
    {
        calcu_pv = eev[id].steps_run_max;    
    }
    else if (calcu_pv < steps_ref_min)          // 什么原理zzl 2018.04.08
    {
        calcu_pv = steps_ref_min;
    }    
            
    pid_kc = pid_get_Kc(p_id);
    denominator = pid_kc * 2;      
    
    if (remainder[id] > denominator || remainder[id] < -denominator) //zzl 2018.04.18为什么要清掉呢?其实是容错，理论上此处不会成立
    {
        remainder[id] = 0;
    }
    dest_val = eev_math_divide(&remainder[id], (I32)increment * calcu_pv * 3, denominator);    /* 需求的步数 */

    return dest_val;
}

/*******************************************************************************
 函 数: eev_io_step_read()
 功 能: 读取实际IO步数
 参 数: no--电膨序号; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void eev_io_step_read(EEV_ID_ENUM id)
{
    if (GET_EXIO_HDW_STATUS_READY(0))
    {
        eev[id].step_sv = GET_EXIO_ST_SV(0, id);//读取目标步数
        DISABLE_IRQ();
        eev[id].step_pv = GET_EXIO_ST_PV(0, id);//读取当前步数
        ENABLE_IRQ();
    }
}

/*******************************************************************************
 函 数: eev_io_step_write_sv()
 功 能: 更新IO目标步数
 参 数: no--电膨序号; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void eev_io_step_write_sv(EEV_ID_ENUM id, EEV_CMD_ENUM cmd)
{               
	PID_ID_ENUM p_id = (PID_ID_ENUM)id;
    cmd = cmd;				/* N0001 */
    if (eev[id].state == EEV_STATE_RUNNING)
    {
        if (eev[id].step_sv < eev[id].steps_run_min) eev[id].step_sv = eev[id].steps_run_min;
        if (eev[id].step_sv > eev[id].steps_run_max) eev[id].step_sv = eev[id].steps_run_max;
    }
    else
    {
        if (eev[id].step_sv > eev[id].steps_max)    eev[id].step_sv = eev[id].steps_max;
    }

    if(id==PID_ID_EEV_1) //主路
    {
        if ((val_motor_manual_mode == USED) && (cmd>=EEV_CMD_RUN || !log_mod_cell_is_run(MODE_NULL)))
        {
            SET_EXIO_ST_SV(0, id, val_motor_manual_steps);
            pid_set_AveAcrossCnt(p_id,0);//没有开启时重新计数
        }
        else if (TF_OPEN_COMP && eev_isReady(id))
        {
            SET_EXIO_ST_SV(0, id, TF_eev_step);
            pid_set_AveAcrossCnt(p_id,0);//没有开启时重新计数
            eev[id].step_sv = TF_eev_step;
		}
#if 0
		else if (USE_ABILITY_EEV_STEP && (cmd>=EEV_CMD_RUN || !log_mod_cell_is_run(MODE_NULL)))
		{
            SET_EXIO_ST_SV(0, id, val_ability_test_motor_steps);
            pid_set_AveAcrossCnt(p_id,0);//没有开启时重新计数
            eev[id].step_sv = val_ability_test_motor_steps;
		}
#endif
        else
        {
            SET_EXIO_ST_SV(0, id, eev[id].step_sv);//更新目标步数
        }
    }
    if(id==PID_ID_EEV_2) //辅路
    {
        if((val_s_motor_manual_mode == USED) && (cmd>=EEV_CMD_RUN || !log_mod_cell_is_run(MODE_NULL)))
        {
            SET_EXIO_ST_SV(0, id, val_s_motor_manual_steps);
            pid_set_AveAcrossCnt(p_id,0);//没有开启时重新计数
        }
		else if (fg_ability_test_run && (val_ability_test_evi_steps > 0) && (cmd>=EEV_CMD_RUN || !log_mod_cell_is_run(MODE_NULL)))
		{
            SET_EXIO_ST_SV(0, id, val_ability_test_evi_steps);
            pid_set_AveAcrossCnt(p_id,0);//没有开启时重新计数
		}
        else
        {
            SET_EXIO_ST_SV(0, id, eev[id].step_sv);//更新目标步数
        }
    }
    
}

/*******************************************************************************
 函 数: eev_io_step_write_pv()
 功 能: 改写IO当前步数
 参 数: no--电膨序号; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void eev_io_step_write_pv(EEV_ID_ENUM id)
{
//   EEV_STEP_PV pIO->io_step_pv[id] = eev[id].step_pv;//更新当前步数
    /* 仅主阀可触发硬件初始化，防止辅阀误触发 */
    if ((id == EEV_ID_1) && GET_EXIO_HDW_STATUS_READY(0))
    {
        SET_EXIO_INIT_HDW(0);	/* 重新执行初始化 */
    }
}

/*******************************************************************************
 函 数: eev_calibrate()
 功 能: 归零校准
 参 数: no--电膨序号; 
 返 回: 空
 其 它: 按最大步数120%关闭
*******************************************************************************/
void eev_calibrate(EEV_ID_ENUM id)
{
    eev[id].step_sv = 0;
    /* 屏蔽此处*1.2的原因: 当前步数会被外机板真实数据改写 */
    eev[id].step_pv = eev_get_init_step_pv(id);

    eev_io_step_write_sv(id,EEV_CMD_RESET);//更新目标步数
    eev_io_step_write_pv(id);//更新当前步数
}

/*******************************************************************************
 函 数: eev_timer()
 功 能: 模块计时函数
 参 数: BOOL TimeFlag 时间标志（用参数传递，以免移植时100ms标志名称不同）
 返 回: 空
 其 它: 在100ms计时处理处调用
*******************************************************************************/
void eev_timer(BOOL TimeFlag)
{
    U08 i;
    static U08 cnt_100ms_to_1s = 0; //100毫秒换算成1秒

    if(TimeFlag)
    {
        for (i=0; i<EEV_TIMER_100MS_MAX; i++)
        {
            if (eev_timer_100ms[i] != 0xffff) eev_timer_100ms[i]++;
        }
        for (i=0; i<EEV_ID_MAX; i++)
        {
            if (eev[i].timer_preopen != 0xffff)     eev[i].timer_preopen++;//预开计时(100ms)
            if (eev[i].timer_calculate != 0xffff)   eev[i].timer_calculate++;//预开计时(100ms)
            if (eev[i].timer_action != 0xffff)      eev[i].timer_action++;//动作计时(100ms)
            if (eev[i].timer_add != 0xffff)         eev[i].timer_add++;//动作计时(100ms)
            if (eev[i].timer_fastadd != 0xffff)     eev[i].timer_fastadd++;//动作计时(100ms)
		    if (eev[i].timer_sub!= 0xffff)          eev[i].timer_sub++;//动作计时(100ms)
            if (eev[i].timer_initchange!= 0xffff)   eev[i].timer_initchange++;//动作计时(100ms)

        }
        
        if (++cnt_100ms_to_1s >= 10)
        {
            cnt_100ms_to_1s = 0;
            for (i=0; i<EEV_TIMER_1S_MAX; i++)
            {
                if (eev_timer_1s[i] != 0xffff) eev_timer_1s[i]++;
            }
            for (i=0; i<EEV_ID_MAX; i++)
            {
                if (eev[i].timer_hold != 0xffff)     eev[i].timer_hold++;//维持当前开度计时(1秒)
            }
        }
    }
}

/*******************************************************************************
 函 数: eev_init()
 功 能: 电膨状态初始化
 参 数: 空
 返 回: 空
 其 它: 在应用程序初始化中调用
*******************************************************************************/
void eev_init(void)
{
    U08 i;

    
    for (i=0; i<EEV_ID_MAX; i++)
    {
        eev[i].state = EEV_STATE_RETURN_ZERO;

        if(i==EEV_ID_1)
        {
            eev_set_motor_type(EEV_ID_1, val_motor_fre_type);
            eev_set_step_speed(EEV_ID_1, val_motor_step_fre);
        }
        else if(i==EEV_ID_2)
        {
            eev_set_motor_type(EEV_ID_2, val_s_motor_fre_type);
            eev_set_step_speed(EEV_ID_2, val_s_motor_step_fre);
        }
    }
}

/*******************************************************************************
 函 数: eev_set_pid_para()
 功 能: 设置电膨的PID参数
 参 数: PID_ID_ENUM p_id: PID序号, EEV_ID_ENUM e_id: 电膨序号
 返 回: 空
 其 它: 空
*******************************************************************************/
void eev_set_pid_para(PID_ID_ENUM p_id, EEV_ID_ENUM e_id)
{
//    pid_set_ctrl(p_id, PID_CTRL_INCREMENT);//增量式
    pid_set_sv(p_id, &eev[e_id].pid_sv);//目标值
    pid_set_pv(p_id, &eev[e_id].pid_pv);//当前值
    if(e_id==PID_ID_EEV_1)
    {
        DebugPID[DEBUG_PID_SV] = eev[e_id].pid_sv;
        DebugPID[DEBUG_PID_PV] = eev[e_id].pid_pv;
    }
    else
    {
        DebugEEV_ID_2PID[DEBUG_PID_SV] = eev[e_id].pid_sv;
        DebugEEV_ID_2PID[DEBUG_PID_PV] = eev[e_id].pid_pv;
    }

//    pid_set_Ts(p_id, val_m_pid_calculate_period);//计算周期
//    pid_set_Kp(p_id, 2);//比例系数
////    pid_set_Ti(p_id, 10*val_m_pid_i);//积分时间wlg v100a01
//    pid_set_Ti(p_id, val_m_pid_i);//积分时间
//    pid_set_Td(p_id, val_m_pid_d);//微分时间
//    pid_set_Kc(p_id, val_m_pid_p);//反作用比例带

    
//    pid_set_ctrl(PID_ID_EEV_ECNM, PID_CTRL_INCREMENT);//增量式
//    pid_set_sv(PID_ID_EEV_ECNM, &eev[EEV_ID_ECNMC].pid_sv);//目标值
//    pid_set_pv(PID_ID_EEV_ECNM, &eev[EEV_ID_ECNMC].pid_pv);//当前值
//    pid_set_Ts(PID_ID_EEV_ECNM, val_pid_calculate_period_1);//计算周期
//    pid_set_Kp(PID_ID_EEV_ECNM, 2);//比例系数
////    pid_set_Ti(PID_ID_EEV_ECNM, 10*val_pid_i_1);//积分时间
//    pid_set_Ti(PID_ID_EEV_MASTER,val_pid_i);//积分时间
//    pid_set_Td(PID_ID_EEV_ECNM, val_pid_d_1);//微分时间
//    pid_set_Kc(PID_ID_EEV_ECNM, val_pid_p_1);//反作用比例带
}

/*******************************************************************************
 函 数: eev_task_pid()
 功 能: PID控制
 参 数: p_id--PID序号; 
        id--电膨序号; 
        cmd--指令; 
        pv--当前步数; 
        sv--目标步数;
 返 回: 空
 其 它: 空
*******************************************************************************/
//#define val_control_limit			    (40)				// 快速接近温差
//#define val_limit_step_num			    (5 )				// 快速接近频率
//#define val_control_limit			    (val_pid_divisiontemp)				// 快速接近温差
//#define val_limit_step_num			    (val_pid_limitstepnum)				// 快速接近频率

void eev_task_pid(PID_ID_ENUM p_id, EEV_ID_ENUM id, EEV_CMD_ENUM cmd, I16 pv, I16 sv)
{
    I16 val = 0;
    static I16 step_quick[EEV_ID_MAX];
    static I16 step_pid[EEV_ID_MAX];
    U16 time_pos = 0;
    U16 time_neg = 0;
//    I16 val_a,val_b,val_c;

    eev[id].pid_pv = pv;
    eev[id].pid_sv = sv;
	
    //读取实际步数
    eev_io_step_read(id);
    switch (eev[id].state)
    {
        /* 阀归零操作 */
        case EEV_STATE_RETURN_ZERO: 
            eev_calibrate(id);
            eev[id].state = EEV_STATE_WAIT_ZERO;
            break;
            
        /* 等待归零操作完成 */  
        case EEV_STATE_WAIT_ZERO:  /* 等待归零操作完成 */
            if (eev[id].step_sv == eev[id].step_pv) 
            {
                eev[id].state = EEV_STATE_WAIT_CMD;
            }
            break;

        case EEV_STATE_WAIT_CMD:  /* 准备好，等待命令 */
            if (cmd == EEV_CMD_RESET)/* 归零后无需求，则转到 待机开度 等待 */
            {
                eev[id].state = EEV_STATE_IDLE;     
            }
            else if (cmd != EEV_CMD_STOP)/* 其他情况，跳转到初始开度 */
            {
                eev[id].state = EEV_STATE_PRE_OPEN;  
            }
            else if (cmd == EEV_CMD_STOP)
            {
                /* 解决待机情况下手动开启再关闭时，辅阀依然停留在手动步数的问题 */
                eev[id].step_sv = 0;
            }
            break;
                    
        case EEV_STATE_IDLE:    // 待机开度
            eev[id].step_sv = eev[id].steps_idle;
            if (eev[id].step_sv == eev[id].step_pv) /* 等待阀的动作执行 */
            {  
                if (cmd >= EEV_CMD_DEFRO)      eev[id].state = EEV_STATE_PRE_OPEN;    
                else if (cmd == EEV_CMD_STOP)  eev[id].state = EEV_STATE_RETURN_ZERO;/* 归零 */
            }
            break;
        
        case EEV_STATE_DEFRO:       // 除霜开度
        
        case EEV_STATE_PRE_OPEN:    // 初始开度（预开度）
            if (cmd == EEV_CMD_DEFRO) 
            {
                eev[id].step_sv = eev[id].steps_defrost;/* 除霜时为除霜开度 */
            }
            else
            {
                eev[id].step_sv = eev[id].steps_preopen;
            } 
            if (eev[id].step_sv == eev[id].step_pv)/* 等待阀的动作执行 */
            {
                if (cmd >= EEV_CMD_RUN) 
                {
                    eev[id].timer_preopen = 0;
                    eev[id].state = EEV_STATE_PRE_KEEP; 
                }
                else if (cmd == EEV_CMD_RESET) eev[id].state = EEV_STATE_IDLE;
                else if (cmd == EEV_CMD_STOP)  eev[id].state = EEV_STATE_RETURN_ZERO;        /* 归零 */
            }
            break;
            
        /* 预开维持 */  
        case EEV_STATE_PRE_KEEP:          
            if (eev[id].timer_preopen >= eev[id].period_preopen)
            {   
                eev[id].state = EEV_STATE_RUNNING;
                eev[id].cnt_positive = (U32)(EEV_RAPID_TIME_1 - 10) * 10 / eev[id].period_calculate;
                eev[id].cnt_negative = (U32)(EEV_RAPID_TIME_1 - 10) * 10 / eev[id].period_calculate;
                eev[id].timer_calculate = 0;
                eev[id].timer_action = 0;
            }
            else if (cmd == EEV_CMD_RESET)
            {
                eev[id].state = EEV_STATE_IDLE;
            }
            else if (cmd == EEV_CMD_STOP)
            {
                eev[id].state = EEV_STATE_RETURN_ZERO;          /* 归零 */
            }
            else
            {
                /* 继续更新初始开度 */
                eev[id].step_sv = eev[id].steps_preopen;
            }
            break;
            
        /* PID运行 */   
        case EEV_STATE_RUNNING: 
            
            if (cmd == EEV_CMD_STOP)       eev[id].state = EEV_STATE_RETURN_ZERO;          /* 归零 */
            else if (cmd == EEV_CMD_PREV)  eev[id].state = EEV_STATE_PRE_OPEN;
            else if (cmd == EEV_CMD_RESET) eev[id].state = EEV_STATE_IDLE;
            else if (cmd == EEV_CMD_DEFRO) eev[id].state = EEV_STATE_DEFRO;

            eev_set_pid_para(p_id, id);
            if(eev[id].state != EEV_STATE_RUNNING)
            {
                pid_turn_off(p_id);
                break;   
            }
            
            /* pid运算周期 */
            if (eev[id].timer_calculate >= eev[id].period_calculate)
            {
                eev[id].timer_calculate = 0;  
                pid_turn_on(p_id);
                eev_speedup(id, pv-sv);    /* 加速处理 */
            }

            /* 阀动作周期 */
            if (eev[id].timer_action >= eev[id].period_action)      
            {
                eev[id].timer_action = 0;
                /* 快速调节量 */  
                val += (I32)step_quick[id] * eev[id].period_calculate / 100; //ZZL 2018.04.10 若有快速调节则val=8或16 或24
                step_pid[id] += pid_read_Uk(p_id);//ZZL 2018.04.10 次数若有PID控制则step_pid有可能大于快速调节量

                val += step_pid[id];           

                /* 按时间快速调节处理 */
                time_pos = eev[id].cnt_positive * eev[id].period_calculate / 10;
                time_neg = eev[id].cnt_negative * eev[id].period_calculate / 10;

                if (time_pos > EEV_RAPID_TIME_2 || time_neg > EEV_RAPID_TIME_2)
                {
                    val = val * 3;
                    if (time_pos > EEV_RAPID_TIME_2 + 30)   /* 第二阶段快速30秒后，回到第一阶段调节 */
                    {
                        eev[id].cnt_positive = (U32)(EEV_RAPID_TIME_2 - 60) * 10 / eev[id].period_calculate;
                    }
                    if (time_neg > EEV_RAPID_TIME_2 + 30)
                    {
                        eev[id].cnt_negative = (U32)(EEV_RAPID_TIME_2 - 60) * 10 / eev[id].period_calculate;
                    }
                }
                else if (time_pos > EEV_RAPID_TIME_1 || time_neg > EEV_RAPID_TIME_1)
                {
                    val = val * 2;
                }
                if(p_id==PID_ID_EEV_1)    DebugPID[DEBUG_PID_MIDDL]=val;   //NOTE-CXW PID调试
                if(p_id==PID_ID_EEV_2)    DebugEEV_ID_2PID[DEBUG_PID_MIDDL]=val;   //NOTE-CXW PID调试
           
                val = eev_increment_adjust(p_id, id, val);  /* 需求的步数 */
                if (p_id == PID_ID_EEV_1)                     /* 主阀限幅 */
                {
                    I16 step_now = eev[id].step_pv;
                    I16 step_limit = MAX(1, (I32)step_now*val_m_limit_range/100);
                    val = fun_pv_limit(val, -step_limit, step_limit);	
                }

                if (id==PID_ID_EEV_1)                       // 变频调阀，不限幅
                {
                    val += log_mod_eev_freq(cmd, eev[id].step_pv);
                }
                
                if(p_id==PID_ID_EEV_1)DebugPID[DEBUG_STEP]=val;   //NOTE-CXW PID调试
                if(p_id==PID_ID_EEV_2)DebugEEV_ID_2PID[DEBUG_STEP]=val;   //NOTE-CXW PID调试

                //原代码注释是：增量值有效时才将计算值清零, 防止丢失数据
                //hzy20170220：其实这是不对的，val之所以为0，
                //是因为储存在了变化量调整的余数中，所以计算值每一次都应该清零。
                //为了和原计算结果保持一致，保留了增量有效才清零的逻辑。
               // if (val != 0)           
                {
                    step_quick[id] = 0;
                    step_pid[id] = 0;   
                }
                
                if (cmd == EEV_CMD_CONST)         /* 膨胀阀保持当前状态，不动作 */
                {
                    val = 0;
                }
                //ZZL 2018.04.10此if else 有问题，有可能导致Val出现负数导致循环输出
                //诱导机制当前步数+ val若小于0则触发
               // if(val<0 && eev[id].step_pv==0) val = 0;  
               // else                            val += eev[id].step_pv;         /* 读取膨胀阀当前步数 */
                 val += eev[id].step_pv;
                 if(val<0)  val = 0;
               
                if (cmd == EEV_CMD_LIMIT_DOWN)/* 关小限制 */ 
                {    
                    if (val < eev[id].step_sv)
                    {
                        val = eev[id].step_sv;
                    }
                } 
                if (cmd == EEV_CMD_LIMIT_UP)/* 开大限制 */ 
                {      
                    if (val > eev[id].step_sv)
                    {
                        val = eev[id].step_sv;
                    }
                }
                if (cmd == EEV_CMD_FORCE_UP     /* 强制开大 */ 
                ||  cmd == EEV_CMD_FORCE_DOWN   /* 强制关小 */ 
                ||  cmd == EEV_CMD_FORCE_FASTUP /* 快速强制开大 */ 
                    )
                {      
                    val = eev[id].step_sv;
                }
                
                eev[id].step_sv = val;
            }
		    //快速强制开大
            if (cmd != EEV_CMD_FORCE_FASTUP)
            {
                eev[id].timer_fastadd = 0;
            }
            else if ((eev[id].timer_fastadd >=(eev[id].period_add))&&(eev[id].period_add!=0))    
            {
                eev[id].timer_fastadd = 0;
                eev[id].step_sv = eev[id].step_sv + 2*eev[id].steps_add;
                pid_set_AveAcrossCnt(p_id,0);
            }
            //强制开阀动作周期
            if (cmd != EEV_CMD_FORCE_UP)
            {
                eev[id].timer_add = 0;
            }
            else if ((eev[id].timer_add >= eev[id].period_add)&&(eev[id].period_add!=0))    
            {
                eev[id].timer_add = 0;
                eev[id].step_sv = eev[id].step_sv + eev[id].steps_add;
                pid_set_AveAcrossCnt(p_id,0);
            }
            //强制关阀动作
            if(cmd != EEV_CMD_FORCE_DOWN)
            {
                eev[id].timer_sub= 0;
            }
            else if ((eev[id].timer_sub>= eev[id].period_sub)&&(eev[id].period_sub!=0))    
            {
                eev[id].timer_sub= 0;
                if(eev[id].step_sv>=eev[id].steps_sub)
                {
                    eev[id].step_sv = eev[id].step_sv - eev[id].steps_sub;
                    pid_set_AveAcrossCnt(p_id,0);
                }
            }

            if (USE_ABILITY_EEV_STEP && id==EEV_ID_1)
    		{
                eev[id].step_sv = val_ability_test_motor_steps;
    		}

#ifdef USE_AT_EEVS
            /* 辅阀能力测试步数 */
    		if (USE_ABILITY_EVI_STEP && id==EEV_ID_2)
    		{
                eev[id].step_sv = val_ability_test_s_motor_steps;
    		}
#endif

            break;
            
        default:    
            eev[id].state = EEV_STATE_RETURN_ZERO;
            break;
    }
//    debug[4]=eev[0].state;
//    debug[5]=eev[1].state;
    //更新目标步数    
    eev_io_step_write_sv(id,cmd);
}

/*******************************************************************************
 函 数: eev_task_area()
 功 能: 分段控制
 参 数: id--电膨序号; 
        cmd--指令; 
        setting_steps--指定步数; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void eev_task_area(EEV_ID_ENUM id, EEV_CMD_ENUM cmd, U16 setting_steps)
{
    //读取实际步数
    eev_io_step_read(id);
    
    switch (eev[id].state)
    {
        case EEV_STATE_RETURN_ZERO: /* 阀归零操作 */
            eev_calibrate(id);
            eev[id].state = EEV_STATE_WAIT_ZERO;
            break;
            
        case EEV_STATE_WAIT_ZERO:  /* 等待归零操作完成 */
            if (eev[id].step_sv == eev[id].step_pv) 
            {
                eev[id].state = EEV_STATE_WAIT_CMD;
            }
            break;

        case EEV_STATE_WAIT_CMD:  /* 准备好，等待命令 */
            if (cmd == EEV_CMD_RESET)/* 归零后无需求，则转到 待机开度 等待 */
            {
                eev[id].state = EEV_STATE_IDLE;     
            }
            else if (cmd != EEV_CMD_STOP)/* 其他情况，跳转到初始开度 */
            {
                eev[id].state = EEV_STATE_RUNNING;  
            }
            else if (cmd == EEV_CMD_STOP)
            {
                /* 解决待机情况下手动开启再关闭时，辅阀依然停留在手动步数的问题 */
                eev[id].step_sv = 0;
            }
            break;
            
        case EEV_STATE_IDLE:    /* 待机开度 */
            eev[id].step_sv = eev[id].steps_idle;
            if (eev[id].step_sv == eev[id].step_pv)/* 等待阀的动作执行 */
            {
                if (cmd >= EEV_CMD_PREV)   // 待机开度 要么因压机有需求转到运行开度
                {
                    eev[id].state = EEV_STATE_RUNNING;  
                }
                else if (cmd == EEV_CMD_STOP)// 要么因停机一段时间后转到归零
                {
                    eev[id].state = EEV_STATE_RETURN_ZERO;  
                }
            }
            break;
            
        case EEV_STATE_RUNNING:     // 运行开度：运行中按指定的开度执行
            
            if (eev[id].step_sv == eev[id].step_pv)// 等待阀的动作执行
            {   
                if (cmd == EEV_CMD_RESET)
                {
                    eev[id].state = EEV_STATE_IDLE;             // 待机开度
                }
                else if (cmd == EEV_CMD_STOP)
                {
                    eev[id].state = EEV_STATE_RETURN_ZERO;        // 归零
                }
            }
            if (eev[id].timer_action >= eev[id].period_action)    // 把赋值放在后面，是为了及时处理切换命令 
            {
                eev[id].timer_action = 0;
                if(cmd == EEV_CMD_CONST)    //维持
                {
                    eev[id].step_sv = eev[id].step_sv;
                }
                else
                {
                    eev[id].step_sv = setting_steps;
                }
            }
            break;
        
        default:
            eev[id].state = EEV_STATE_RETURN_ZERO;
            break;
    }
    //更新目标步数
    eev_io_step_write_sv(id,cmd);
}


/*******************************************************************************
 函 数: eev_set_steps_idle()
 功 能: 设置待机步数
 参 数: id--电膨序号; 
        value--设置值; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL eev_set_steps_idle(EEV_ID_ENUM id, U16 value)
{
    BOOL result = TRUE;
    
    if (id >= EEV_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        eev[id].steps_idle = value;//待机步数
    }

    return result;
}

/*******************************************************************************
 函 数: eev_set_steps_preopen()
 功 能: 设置预开步数
 参 数: id--电膨序号; 
        value--设置值; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL eev_set_steps_preopen(EEV_ID_ENUM id, U16 value)
{
    BOOL result = TRUE;
    
    if (id >= EEV_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        eev[id].steps_preopen = value;//预开步数
    }

    return result;
}

/*******************************************************************************
 函 数: eev_set_steps_defrost()
 功 能: 设置除霜步数
 参 数: id--电膨序号; 
        value--设置值; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL eev_set_steps_defrost(EEV_ID_ENUM id, U16 value)
{
    BOOL result = TRUE;
    
    if (id >= EEV_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        eev[id].steps_defrost = value;//除霜步数
    }

    return result;
}

/*******************************************************************************
 函 数: eev_set_steps_max()
 功 能: 设置总步数
 参 数: id--电膨序号; 
        value--设置值; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL eev_set_steps_max(EEV_ID_ENUM id, U16 value)
{
    BOOL result = TRUE;
    
    if (id >= EEV_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        eev[id].steps_max = value;//总步数
    }

    return result;
}

/*******************************************************************************
 函 数: eev_set_steps_run_max()
 功 能: 设置最大运行步数
 参 数: id--电膨序号; 
        value--设置值; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL eev_set_steps_run_max(EEV_ID_ENUM id, U16 value)
{
    BOOL result = TRUE;
    
    if (id >= EEV_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        eev[id].steps_run_max = value;//最大运行步数
    }

    return result;
}
/*******************************************************************************
 函 数: eev_set_steps_run_max()
 功 能: 设置最大运行步数
 参 数: id--电膨序号; 
        value--设置值; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
I16 eev_get_steps_run_max(EEV_ID_ENUM id)
{
    I16 result = 0;
    
    if (id >= EEV_ID_MAX)
    {
        result = 0;
    }
    else
    {
        result=eev[id].steps_run_max;//最大运行步数
    }

    return result;
}

/*******************************************************************************
 函 数: eev_set_steps_min()
 功 能: 设置最小步数
 参 数: id--电膨序号; 
        value--设置值; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL eev_set_steps_min(EEV_ID_ENUM id, U16 value)
{
    BOOL result = TRUE;
    
    if (id >= EEV_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        eev[id].steps_run_min = value;//最小步数
    }

    return result;
}
/*******************************************************************************
 函 数: eev_set_steps_run_max()
 功 能: 设置最大运行步数
 参 数: id--电膨序号; 
        value--设置值; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
I16 eev_get_steps_min(EEV_ID_ENUM id)
{
    I16	result = 0;
    
    if (id >= EEV_ID_MAX)
    {
        result = 0;
    }
    else
    {
        result=eev[id].steps_run_min;//最大运行步数
    }

    return result;
}


/*******************************************************************************
 函 数: eev_set_step_speed()
 功 能: 设置励磁频率序号
 参 数: id--电膨序号; 
        value--设置值; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL eev_set_step_speed(EEV_ID_ENUM id, U16 value)
{
    BOOL result = TRUE;
    
    if (id >= EEV_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        eev[id].motor_fre = value;  //励磁频率序号
    }

    return result;
}

/*******************************************************************************
 函 数: eev_set_motor_type()
 功 能: 设置励磁方式
 参 数: id--电膨序号; 
        value--设置值; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL eev_set_motor_type(EEV_ID_ENUM id, U16 value)
{
    BOOL result = TRUE;
    
    if (id >= EEV_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        eev[id].motor_type = value;  //励磁频率序号
    }

    return result;
}


/*******************************************************************************
 函 数: eev_degree_to_steps()
 功 能: 开度转换为步数
 参 数: id--电膨序号; 
        DegreeValue--开度值; 
 返 回: 步数
 其 它: 应在eev_set_steps_max之后使用。
*******************************************************************************/
U16 eev_degree_to_steps(EEV_ID_ENUM id, U16 DegreeValue)
{
    U16 Steps = 0;
    
    if (id >= EEV_ID_MAX )
    {
        Steps = 0;
    }
    else
    {
        Steps = (U32)DegreeValue * eev[id].steps_max / 100;     // 开度对应步数
    }

    return Steps;
}

/*******************************************************************************
 函 数: eev_get_step_speed()
 功 能: 获取电膨励磁频率序号
 参 数: id--电膨序号; 

 返 回: 励磁频率序号
 其 它: 无
*******************************************************************************/
U16 eev_get_step_speed(EEV_ID_ENUM id)
{
    U16 StepSpeed = 0;
    
    if (id >= EEV_ID_MAX )
    {
        StepSpeed = 0;
    }
    else
    {
        StepSpeed = eev[id].motor_fre;     
    }

    return StepSpeed;
}

/*******************************************************************************
 函 数: eev_get_motor_type()
 功 能: 获取电膨励磁方式
 参 数: id--电膨序号; 

 返 回: 励磁方式
 其 它: 无
*******************************************************************************/
U16 eev_get_motor_type(EEV_ID_ENUM id)
{
    U16 MotorType = 0;
    
    if (id >= EEV_ID_MAX )
    {
        MotorType = 0;
    }
    else
    {
        MotorType = eev[id].motor_type;     
    }

    return MotorType;
}
/*******************************************************************************
 函 数: eev_get_motor_type()
 功 能: 获取电膨励磁方式
 参 数: id--电膨序号; 

 返 回: 励磁方式
 其 它: 无
*******************************************************************************/
U16 eev_get_timer_initchange(EEV_ID_ENUM id)
{
    U16 MotorType = 0;
    
    if (id >= EEV_ID_MAX )
    {
        MotorType = 0;
    }
    else
    {
        MotorType = eev[id].timer_initchange;     
    }

    return MotorType;
}

/*******************************************************************************
 函 数: eev_set_period_preopen()
 功 能: 设置预开时间
 参 数: id--电膨序号; 
        value--设置值; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL eev_set_period_preopen(EEV_ID_ENUM id, U16 value)
{
    BOOL result = TRUE;
    
    if (id >= EEV_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        eev[id].period_preopen = value;//预开时间(100ms)
    }

    return result;
}

/*******************************************************************************
 函 数: eev_set_period_calculate()
 功 能: 设置计算周期
 参 数: id--电膨序号; 
        value--设置值; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL eev_set_period_calculate(EEV_ID_ENUM id, U16 value)
{
    BOOL result = TRUE;
    
    if (id >= EEV_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        eev[id].period_calculate = value;//计算周期(100ms)
    }

    return result;
}

/*******************************************************************************
 函 数: eev_get_step_fre()
 功 能: 获取励磁频率
 参 数: id--电膨序号; 
 返 回: result--励磁频率序号
 其 它: 空
*******************************************************************************/
//U16 eev_get_step_fre(EEV_ID_ENUM id)
//{
//    U16 result = 0;
//    
//    if (id >= EEV_ID_MAX)
//    {
//        result = 0;
//    }
//    else
//    {
//        result = eev[id].moter_fre;
//    }
//
//    return result;
//}
/*******************************************************************************
 函 数: eev_set_step_fre()
 功 能: 设置励磁频率
 参 数: id--电膨序号; 
        value--设置值（励磁频率序号）
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
//BOOL eev_set_step_fre(EEV_ID_ENUM id, U16 value)
//{
//    BOOL result = TRUE;
//    
//    if (id >= EEV_ID_MAX)
//    {
//        result = FALSE;
//    }
//    else
//    {
//        eev[id].moter_fre = value;
//    }
//
//    return result;
//}

/*******************************************************************************
 函 数: eev_set_period_action()
 功 能: 设置动作周期
 参 数: id--电膨序号; 
        value--设置值; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL eev_set_period_action(EEV_ID_ENUM id, U16 value)
{
    BOOL result = TRUE;
    
    if (id >= EEV_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        eev[id].period_action = value;//动作周期(100ms)
    }

    return result;
}
/*******************************************************************************
 函 数: eev_set_period_add()
 功 能: 设置强制开大动作周期
 参 数: id--电膨序号; 
        value--设置值; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL eev_set_period_add(EEV_ID_ENUM id, U16 value)
{
    BOOL result = TRUE;
    
    if (id >= EEV_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        eev[id].period_add = value;//动作周期(100ms)
    }

    return result;
}
/*******************************************************************************
 函 数: eev_set_steps_add()
 功 能: 设置强制增开阀步数
 参 数: id--电膨序号; 
        value--设置值; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL eev_set_steps_add(EEV_ID_ENUM id, U16 value)
{
    BOOL result = TRUE;
    
    if (id >= EEV_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        eev[id].steps_add = value;//动作周期(100ms)
    }

    return result;
}
/*******************************************************************************
 函 数: eev_set_period_sub()
 功 能: 设置强制开小动作周期
 参 数: id--电膨序号; 
        value--设置值; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL eev_set_period_sub(EEV_ID_ENUM id, U16 value)
{
    BOOL result = TRUE;
    
    if (id >= EEV_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        eev[id].period_sub= value;//动作周期(100ms)
    }

    return result;
}
/*******************************************************************************
 函 数: eev_set_steps_sub()
 功 能: 设置强制开小阀步数
 参 数: id--电膨序号; 
        value--设置值; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL eev_set_steps_sub(EEV_ID_ENUM id, U16 value)
{
    BOOL result = TRUE;
    
    if (id >= EEV_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        eev[id].steps_sub = value;//动作周期(100ms)
    }

    return result;
}
/*******************************************************************************
 函 数: eev_set_period_hold()
 功 能: 设置强制增开阀步数
 参 数: id--电膨序号; 
        value--设置值; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL eev_set_period_hold(EEV_ID_ENUM id, U16 value)
{
    BOOL result = TRUE;
    
    if (id >= EEV_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        eev[id].period_hold = value;//动作维持时间(秒)
    }

    return result;
}
/*******************************************************************************
 函 数: eev_set_period_cyc()
 功 能: 设置强制增开阀步数
 参 数: id--电膨序号; 
        value--设置值; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL eev_set_period_cyc(EEV_ID_ENUM id, U16 value)
{
    BOOL result = TRUE;
    
    if (id >= EEV_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        eev[id].period_cyc = value;//动作周期(秒)
    }

    return result;
}

/*******************************************************************************
 函 数: adjust_method_U16()
 功 能: 调节函数_U16类型
 参 数:   adj_info--调整要求（包括最小值、最大值、步长）; 
        original_data--原始值
        direction--调整方向; 
 返 回: adj_data--调整后的值
 其 它: 空
*******************************************************************************/
#define DIREC_ADD   0   // 增加
#define DIREC_SUB   1   // 减少
U16 adjust_method_U16(ADJUST_STRUCT adj_info, U16 original_data, U16 direction)
{
    U16 adj_data;
    adj_data = original_data;
    
    if (DIREC_ADD == direction)
    {
        if (adj_data < (adj_info.adj_max-adj_info.adj_step))
        {
            adj_data += adj_info.adj_step;
        }
        else
        {
            adj_data = adj_info.adj_max;
        }
    }
    else if (DIREC_SUB == direction)
    {
        if (adj_data > (adj_info.adj_min+adj_info.adj_step))
        {
            adj_data -= adj_info.adj_step;
        }
        else
        {
            adj_data = adj_info.adj_min;
        }
    }

    if(adj_data > adj_info.adj_max)   adj_data=adj_info.adj_max;
    if(adj_data < adj_info.adj_min)   adj_data=adj_info.adj_min;
    
    return adj_data;
}


/*******************************************************************************
 函 数: eev_isReady()
 功 能: 获取电膨是否已准备好的状态
 参 数: id--电膨序号; 

 返 回: result--电膨准备好的状态(TRUE-准备好，FALSE-未准备好)
 其 它: 归零过程为未准备好，应该不允许开压机
*******************************************************************************/
BOOL eev_isReady(EEV_ID_ENUM id)
{
    BOOL result = TRUE;
    
    if (id >= EEV_ID_MAX || eev[id].state < EEV_STATE_WAIT_CMD)
    {
        result = FALSE;
    }

    return result;
}
/*******************************************************************************
 函 数: BOOL eev_holdjudge(BOOL curt_state,EEV_ID_ENUM id)
 功 能: 是否维持当前开度判断
 参 数: curt_state----当前开关状态; 
		curt_temp--当前值; 
		borderline--边界值; 
		Diff--回差值; 
		direction--判断方向(DRCT_UNDER：小于边界值为真；DRCT_ABOVE：大于边界值为真); 
		
 返 回: TRUE/FALSE--新的开关状态为真维持/假不维持
 其 它: 
*******************************************************************************/

BOOL eev_holdjudge(BOOL curt_state,EEV_ID_ENUM id)
{  
    BOOL new_state = curt_state;    //新状态


    if(eev[id].period_hold==0)  //维持时间为0则没有维持功能
    {
        new_state = FALSE;
        eev[id].timer_hold = 0;
    }
    else
    {
        if(eev[id].timer_hold>=eev[id].period_hold+eev[id].period_cyc)
        {
            eev[id].timer_hold = 0;
            new_state = FALSE;
        }
        else if(eev[id].timer_hold>=eev[id].period_cyc)
        {
            new_state = TRUE;
        }
        else if(eev[id].timer_hold>=0)
        {
            new_state = FALSE;
        }
    }
    
    return new_state;
}

/*******************************************************************************
 函 数: eev_get_init_step_pv()
 功 能: 获取初始化时的电膨当前步数(当前步数*1.2)，用于过关
 参 数: id--; 
 返 回: 
 其 它: 空
*******************************************************************************/
U16 eev_get_init_step_pv(EEV_ID_ENUM id)
{
    return MAX((U32)eev[id].step_pv * 12 / 10, 60);			/* 保底50*1.2=60步是为了规避step_pv和step_sv相等 */
}

