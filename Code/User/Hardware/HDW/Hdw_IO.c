/******************************************************************************

          版权所有 (C), 2001-2014, 广州市邦普电脑技术开发有限公司

 ******************************************************************************
  文 件 名   : Hdw_IO.c
  版 本 号   : 初稿
  作    者   : // zzp v160a7
  生成日期   : 2016年11月8日星期二
  最近修改   :
  功能描述   : IO口处理
  函数列表   :
  修改历史   :
  1.日    期   : 2016年11月8日星期二
    作    者   : // zzp v160a7
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "Includes.h"
/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
const I08 PhsTbl[8][8] =
     { /*  00   01  10   11 */
         { 0,  -1,   1,   0 },   //00
         { 1,   0,   1,  -1 },   //01
         {-1,  -1,   0,   1 },   //10
         { 0,   1,  -1,   0 },   //11
     };
/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/
U16 Hdw_Input[INPUT_W];
U16 Hdw_HDI[MAX_HDI];
U16 Hdw_DO[MAX_DO_W];
U08 Hdw_Phase;
U16 Hdw_Switch;
U16 Hdw_SwPl;
static U08 IN_time[INPUT_NUM]; /* 消抖时间 */

void HDW_GetInput(void)
{
    U16 i;
    U16 IN_tmp[INPUT_W];            /* 临时读取变量 */
    static U16 IN_bak[INPUT_W];/* 备份 */

    ARRCLR(IN_tmp);

    #if IN_74HC165_BITS > 0
    {
        U16 In_74HC165[IN_74HC165_W];
        //4 读74HC165
        MCU_GetInput74HC165(In_74HC165);
        for (i=0; i<IN_74HC165_BITS; i++)
        {
            if (GETBIT_N(In_74HC165, i))
            {
                SETBIT_N(IN_tmp, i);
            }
        }
    }
    #endif

    #if IN_PARALLEL_BITS > 0
    {
        U16 In_Parallel[IN_PARALLEL_W];
        //4 读并行Input
        MCU_GetInputParallel(In_Parallel);
        for (i=0; i<IN_PARALLEL_BITS; i++)
        {
            if (GETBIT_N(In_Parallel, i))
            {
                SETBIT_N(IN_tmp, i+IN_74HC165_BITS);
            }
        }
    }
    #endif

    //4 滤波
    for (i=0; i<INPUT_NUM; i++)
    {
        BOL cur_di = GETBIT(IN_tmp[i>>4], i&0x0f);
        BOL bak_di = GETBIT(IN_bak[i>>4], i&0x0f);
        if(cur_di != bak_di)    
        {
            if(cur_di)  SETBIT(IN_bak[i>>4], i&0x0f);
            else        CLRBIT(IN_bak[i>>4], i&0x0f);
            IN_time[i] = 0;
        }
        else
        {
            if(IN_time[i] != 0xff)  IN_time[i]++;

            if(IN_time[i] >= 10) /* 100ms消抖 */
            {
                if(cur_di)      
                    SETBIT(Hdw_Input[i>>4], i&0x0f);    
                else            
                    CLRBIT(Hdw_Input[i>>4], i&0x0f);            
            }
        }
    }
    
}

/*****************************************************************************
函  数 : HDW_GetHDI()
功  能 : 读取强电开关量输入,100ms消抖
参  数 : 无
返  回 : 无
其  他 : 防止电容失效，1ms调用一次，此处读到高电平置一，取反在PunpGetDi中处理
*****************************************************************************/
void HDW_GetHDI(void)
{
    U16 i;
    U16 HDI_tmp[MAX_HDI];       /* 临时读取变量 */
    static U16 HDI_bak[MAX_HDI];/* 备份 */
    static U08 HDI_time[MAX_HDI_NUM]; /* 消抖时间 */
	static U08 HDI_time1[MAX_HDI_NUM]; /* 电容失效消抖时间 */
//	static U08 HDI_pulse[MAX_HDI_NUM]; /* 电容失效脉冲跳变次数 */
	static U08 HDI_low[MAX_HDI_NUM]; /* 电容失效低电平采集计数 */
    
    MCU_GetHDi(HDI_tmp);
	
    for (i=0; i<MAX_HDI_NUM; i++)
    {
        BOOL cur_di = GETBIT(HDI_tmp[i>>4], i&0x0f);
        BOOL bak_di = GETBIT(HDI_bak[i>>4], i&0x0f);
        if(cur_di != bak_di)    
        {
            if(cur_di)  SETBIT(HDI_bak[i>>4], i&0x0f);
            else        CLRBIT(HDI_bak[i>>4], i&0x0f);
            HDI_time[i] = 0;
        }
        else
        {
            if(HDI_time[i] != 0xff)  HDI_time[i]++;

            if(HDI_time[i] >= 100) /* 100ms消抖 */
            {
				HDI_low[i] = 0;
				HDI_time1[i] = 0;
                if(cur_di)
                    SETBIT(Hdw_HDI[i>>4], i&0x0f);
                else
                    CLRBIT(Hdw_HDI[i>>4], i&0x0f);                
            }
        }
		
		if (!cur_di)
		{
			if(HDI_low[i] != 0xff)  HDI_low[i]++;
		}

		if(HDI_time1[i] != 0xff)  HDI_time1[i]++;
		
		if (HDI_time1[i] >= 100) /* 100ms消抖 */
		{
			HDI_time1[i] = 0;

			if (HDI_low[i] >= 35) // 在消抖时间内采集到35次以上低电平则认为闭合
			{
				CLRBIT(Hdw_HDI[i>>4], i&0x0f);
			}
			HDI_low[i] = 0;
		}
    }

#if 0
    for (i=0; i<MAX_HDI_NUM; i++)
    {
        BOOL cur_di = GETBIT(HDI_tmp[i>>4], i&0x0f);
        BOOL bak_di = GETBIT(HDI_bak[i>>4], i&0x0f);
        if(cur_di != bak_di)    
        {
        	if(HDI_pulse[i] != 0xff)  HDI_pulse[i]++;
        	 
            if(cur_di)  SETBIT(HDI_bak[i>>4], i&0x0f);
            else        CLRBIT(HDI_bak[i>>4], i&0x0f);
            HDI_time[i] = 0;
        }

		if (!cur_di)
		{
			if(HDI_low[i] != 0xff)  HDI_low[i]++;
		}

		if(HDI_time1[i] != 0xff)  HDI_time1[i]++;
		
		if (HDI_pulse[i] >= 5)
		{
			if (HDI_time1[i] >= 100) /* 100ms消抖 */
			{
				HDI_time1[i] = 0;
				if (HDI_low[i] >= 50) // 在消抖时间内采集到50次以上低电平则认为闭合
				{
					HDI_pulse[i] = 0;
					HDI_time[i] = 0;
					HDI_low[i] = 0;
					CLRBIT(Hdw_HDI[i>>4], i&0x0f);
				}
			}
		}
        else
        {
            if(HDI_time[i] != 0xff)  HDI_time[i]++;

            if(HDI_time[i] >= 100) /* 100ms消抖 */
            {
            	HDI_pulse[i] = 0;
				HDI_low[i] = 0;
				HDI_time1[i] = 0;
				
                if(cur_di)
                    SETBIT(Hdw_HDI[i>>4], i&0x0f);
                else
                    CLRBIT(Hdw_HDI[i>>4], i&0x0f);                
            }
        }
    }
#endif
    
}

/*****************************************************************************
函  数 : HDW_SetDo()
功  能 : 输出继电器
参  数 : 无
返  回 : 无
其  他 : 输出后会清除输出缓存,故每个主循环应刷新输出
*****************************************************************************/
void HDW_SetDo(void)
{
    U08 i;

    /* 输出到硬件 */
    MCU_SetDo(Hdw_DO);

    /* 输出清零 */
    for (i=0; i<MAX_DO_W; i++)
    {
        Hdw_DO[i] = 0;
    }
    
}

#if 0
/*****************************************************************************
函  数 : HDW_GetPhase()
功  能 : 检测相序
参  数 : 无
返  回 : 无
其  他 : 严格1ms中断调用
*****************************************************************************/
void HDW_GetPhase(void)
{
#define USE_PHASE_FILTER    /* 使用相序滤波 */
    U08 i;
    U08 PhaseNow = 0;
    static I08 PhaseCount;
    static U08 PhaseTimes;
    static U08 PhaseDly;
    static U08 PhaseBak;
    static U08 PhaseOld;
    static BOL fg_First = TRUE;
#ifdef USE_PHASE_FILTER    
    U08 PhaseCnt1 = 0;
    U08 PhaseCnt2 = 0;
    U08 PhaseBuf;

    /* 连续读取5次 */
    for (i=0; i<MAX_PHASE_SHAKE; i++)
    {
        PhaseBuf = MCU_GetPhase();
        if (PhaseBuf & 0x01) 
        {
            PhaseCnt1++;     
        }
        if (PhaseBuf & 0x02)
        {
            PhaseCnt2++;
        }
    }

    /* 超过一半，则置1 */
    if (PhaseCnt1 > MAX_PHASE_SHAKE/2)
        PhaseNow |= 0x01;
    if (PhaseCnt2 > MAX_PHASE_SHAKE/2)
        PhaseNow |= 0x02;
#else
    PhaseNow = MCU_GetPhase() & 0x03;
#endif

    if (PhaseBak != PhaseNow)
    {
        PhaseBak = PhaseNow;
    }
    else
    {
        if(fg_First)
        {
            fg_First = FALSE;
            PhaseOld = PhaseNow;
            PhaseTimes = 0;
            return;
        }
        else
        {
            if (PhaseOld != PhaseNow)
            {
                PhaseCount += PhsTbl[PhaseNow][PhaseOld];
                PhaseOld = PhaseNow;
            }
            
        }
    }

    /* 一个检测周期 */
    if (++PhaseTimes >= PHASE_TIMES)
    {
        PhaseTimes = 0;
        
        if (PhaseDly != 0xff)   PhaseDly++;

        if (PhaseCount >= PHASE_OK_CNT)
        {
            PhaseDly = 0;
            Hdw_Phase = PHASE_STA_OK;
        }
        else if (PhaseCount <= PHASE_ERR_CNT)
        {
            if (PhaseDly >= 10)/*  逆相、持续1秒后报警 */
            {
                Hdw_Phase = PHASE_STA_ERR;
            }
        }
        else
        {
            if (PhaseDly >= 30)/*  缺相、持续3秒钟才报警 */
            {
                Hdw_Phase = PHASE_STA_LACK;
            }
        }

        PhaseCount = 0;
    }

}

/*****************************************************************************
函  数 : PunpGetPhase()
功  能 : 三相电状态接口函数
参  数 : 无
返  回 : 三相电状态
其  他 : 无
*****************************************************************************/
U08 PunpGetPhase(void)
{
    return Hdw_Phase;
}
#endif

/*****************************************************************************
函  数 : PunpGetPhase()
功  能 : 三相电状态接口函数
参  数 : 无
返  回 : 三相电状态
其  他 : 无
*****************************************************************************/
U08 PunpGetPhase(void)
{
    return GET_EXIO_PH(0);
}

/*****************************************************************************
函  数 : PunpGetDi()
功  能 : 读取开关量接口函数
参  数 : U16 InputBuf[] : 开关量缓存 
         U16 InputNum   : 开关量个数
返  回 : TRUE成功/FALSE失败
其  他 : 无
*****************************************************************************/
BOL PunpGetDi(U16 InputBuf[], U16 InputNum)
{
    U16 i;
    U16 input_idx;
    // DI在Hdw_Input中的索引
    static const U08 di_idx[MAX_DI_NUM] = {
        7,6,5,4,3,2,1
    };

    if (InputNum > MAX_DI_TOTAL_NUM)
        return FALSE;
    
    for (i=0; i<InputNum; i++)
    {
    	if (i < MAX_DI_NUM)	// 普通开关
    	{
	        input_idx = di_idx[i];
	        // 闭合为0
	        if(GETBIT_N(Hdw_Input, input_idx))
	        {
	            CLRBIT(InputBuf[i>>4], i&0x0f);
	        }
	        else
	        {
	            SETBIT(InputBuf[i>>4], i&0x0f);
	        }
    	}
		else	// 强电开关
		{
			input_idx = i - MAX_DI_NUM;
			// 闭合为0
			if (GETBIT_N(Hdw_HDI, input_idx))
			{
				CLRBIT(InputBuf[i>>4], i&0x0f);
			}
	        else
	        {
	            SETBIT(InputBuf[i>>4], i&0x0f);
	        }
		}
    }  

    return TRUE;
}

/*****************************************************************************
函  数 : PunpGet4GExist()
功  能 : 读取是否存在4G模组
参  数 : 无
返  回 : 0为有,1为没有
其  他 : 无
*****************************************************************************/
BOL PunpGet4GExist(void)
{
    // 0为有,1为没有
    return !GETBIT_N(Hdw_Input, 0);
}

/*****************************************************************************
函  数 : PunpCloseAllDo()
功  能 : 关闭所有DO输出
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
void PunpCloseAllDo(void)
{
    ARRCLR(Hdw_DO);
    HDW_SetDo();
}

/*****************************************************************************
函  数 : PunpSetDo()
功  能 : 输出继电器接口函数
参  数 : U16 OutputBuf[] : 输出缓存 
         U16 OutputNum   : 输出个数
返  回 : TRUE成功/FALSE失败
其  他 : 每个主循环应调用
*****************************************************************************/
BOL PunpSetDo(U16 OutputBuf[], U16 OutputNum)
{
    U16 i;

    if (OutputNum > MAX_DO_NUM)
        return FALSE;
    
    for (i=0; i<OutputNum; i++)
    {
        if(GETBIT(OutputBuf[i>>4], i&0x0f))
            SETBIT(Hdw_DO[i>>4], i&0x0f);
        else
            CLRBIT(Hdw_DO[i>>4], i&0x0f);
    }    

    return TRUE;
}

/*****************************************************************************
函  数 : PunpSetLed()
功  能 : LED控制
参  数 : U16 Led : LED编号
         U08 Sta : LED状态
返  回 : 无
其  他 : 无
*****************************************************************************/
BOOL PunpSetLed(U16 Led, U08 Sta)
{
    if (Led >= MAX_LED_NUM)
        return FALSE;
    
    MCU_SetLed(Led, Sta);

    return TRUE;
}

/*****************************************************************************
函  数 : PunpSet4GPower()
功  能 : 4G模块开关机控制
参  数 : BOL onoff : 电平
返  回 : 无
其  他 : 无
*****************************************************************************/
void PunpSet4GPower(BOL onoff)
{
    MCU_Set4GPower(onoff);
}

/*****************************************************************************
函  数 : PunpSet4GReset()
功  能 : 4G模块紧急复位控制
参  数 : BOL reset : 复位电平
返  回 : 无
其  他 : 无
*****************************************************************************/
void PunpSet4GReset(BOL reset)
{
    MCU_Set4GReset(reset);
}

/*******************************************************************************
 函 数: PunpGetSwitch()
 功 能: 
 参 数: SwBuf[]--; 
		SwNum--; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void PunpGetSwitch(U16 SwBuf[], U16 SwNum)
{
    U16 switch_w[MAX_SW]={0};
    U16 i;
    U16 input_idx;
    static const U08 sw_idx[PINS_NUM_SW] = {
        24,25, // SW1
		20,21,22,23, // SW2
		12,13,14,15, // SW4
		8,9,10,11,	 // SW5
    };

	if (SwNum > PINS_NUM_SW)
	{
		return;	
	}
    for (i=0; i<SwNum; i++)
    {
        input_idx = sw_idx[i];
        // on为0
        if(GETBIT_N(Hdw_Input, input_idx))
        {
            CLRBIT(switch_w[i>>4], i&0x0f);
        }
        else
        {
            SETBIT(switch_w[i>>4], i&0x0f);
        }
    }    
	for (i=0; i<MAX_SW; i++)
	{
		SwBuf[i] = switch_w[i];
	}
}


/*******************************************************************************
 函 数: PunpGetSwPl()
 功 能: 
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
void PunpGetSwPl(U16 SwPlBuf[], U16 SwPlNum)
{
    U16 switch_w[MAX_SWPL] = {0};
    U16 i;
    U16 input_idx;
    static const U08 swpl_idx[MAX_SWPL_BIT] = {
		16,17,18,19, // SW3 拨盘
    };

    for (i=0; i<MAX_SWPL_BIT; i++)
    {
        input_idx = swpl_idx[i];
        // on为0
        if(GETBIT_N(Hdw_Input, input_idx))
        {
            CLRBIT(switch_w[i>>4], i&0x0f);
        }
        else
        {
            SETBIT(switch_w[i>>4], i&0x0f);
        }
    }    
	for (i=0; i<MAX_SWPL; i++)
	{
		SwPlBuf[i] = switch_w[i];
	}
}

#if MAX_MOTOR_NUM != 0
/*====================================================
		功能: 步进电机驱动，1ms中断中调用
				停止时，需要励磁0.5秒
				开始时，需要励磁0.5秒
				脉冲的占空比为50％
				励磁周期为32ms
				励磁速度可调，从低速到高速
机在较高速或大惯量负载时，一般不在工作速度起动，
而采用逐渐升频提速，一电机不失步，二可以减少噪音同时可以提高停止的定位精度。
步进电机最好不使用整步状态，整步状态时振动大。
		输入: 调节的步数
/====================================================*/
//	#define val_motor_fre_type	0
	#define IS_EIGHT_STEP       0 //(val_motor_fre_type == 0)		/* 四相八拍 */
	#define IS_FOUR_STEP        1 //(val_motor_fre_type == 1)		/* 四相四拍 */

	#define STEP_KEEP		600			/* 驱动时间 */
//	#define val_motor_step_fre	0
//	#define STEP_SPEED		(moter_fre[val_motor_step_fre])		/* 步进周期(ms) */

	const  U08  StepTbl[] = { 0x00, 0x09, 0x08, 0x0c, 0x04, 0x06, 0x02, 0x03, 0x01,};		/* 开阀顺序 */	
	const  U08  moter_fre[] = { 32,  16,  12,   10,   8,   6,   4};	/* 每步多少时间(ms) */
							//{ 31, 62, 83, 100, 125, 166, 250 };	/* PPS每秒多少步 */
//	static U08  step_relay = 0;
	static U08  motor_step = 0;					/* 动作步骤 */
	static I08  step_idx[MAX_MOTOR] = {0, 0};
	static U16  step_time[MAX_MOTOR] = {0, 0};

void IO_Step(void)
{
	U08  step_out = 0;
	static U08 no = 0;
	static U08 no_next = 0;
	U08 i;
    U08 step_speed_index,step_speed, motor_type;

    step_speed_index = eev_get_step_speed((EEV_ID_ENUM)no);
    step_speed = moter_fre[step_speed_index];
    motor_type   = eev_get_motor_type((EEV_ID_ENUM)no);
	
	for (i=0; i<MAX_MOTOR; i++)  
	{
		if (step_time[i] != 0xffff) step_time[i]++;
	}
	switch (motor_step)
	{
		case 0 :	/* 停止(掉电) */
			step_out = 0;	
			motor_step = 10;
			break;
			
		case 10:	/* 换下一个步进电机 */
			no = no_next;
			
			if (++no_next >= MAX_MOTOR)
			{
				no_next = 0;
			}

			/* 测试程序中步进电机特殊处理，全部为0#电机的输出 */
			//if (is_testing) no_next = 0;
			
			if (gIO.io_step_sv[no] != gIO.io_step_pv[no]) {
				motor_step = 1;		step_time[no] = 0;
			}
			break;		

		case 1 :	/* 保持过渡 */
			step_out = step_idx[no];
			if (step_time[no] >= STEP_KEEP) {
				if (gIO.io_step_sv[no] > gIO.io_step_pv[no]) 	  	motor_step = 2;	/* 需要正转 */
				else if (gIO.io_step_sv[no] < gIO.io_step_pv[no]) 	motor_step = 3;	/* 需要反转 */
				else 										  		motor_step = 0;	/* 需要停止 */
			}
			break;
		case 2 :	/* 正转 */
			if (gIO.io_step_sv[no] > gIO.io_step_pv[no]) {
				if (step_time[no] >= step_speed /*STEP_SPEED*/) 
                {
					step_time[no] = 0;
					gIO.io_step_pv[no]++;

					if (IS_FOUR_STEP == motor_type) 
                    {
						step_idx[no] += 2;
						if (step_idx[no] > 8) step_idx[no] = 2;
					}
					else 
                    {
						step_idx[no]++;
						if (step_idx[no] > 8) step_idx[no] = 1;
					}
				}
				step_out = step_idx[no];
			}
			else {
				motor_step = 1;		step_time[no] = 0;
				step_out = step_idx[no];
			}
			break;
		case 3 :	/* 反转 */
			if (gIO.io_step_sv[no] < gIO.io_step_pv[no]) {
				if (step_time[no] >= step_speed /*STEP_SPEED*/) 
                {
					step_time[no] = 0;
					gIO.io_step_pv[no]--;

					if (IS_FOUR_STEP == motor_type)
                    {
						step_idx[no] -= 2;
						if (step_idx[no] < 2) step_idx[no] = 8;
					}
					else 
                    {
						step_idx[no]--;
						if (step_idx[no] < 1) step_idx[no] = 8;
					}
				}
				step_out = step_idx[no];
			}
			else {
				motor_step = 1;		step_time[no] = 0;
				step_out = step_idx[no];
			}
			break;
		default :	
			motor_step = 0;
			break;
	}

	if (IS_FOUR_STEP == motor_type) /* 四拍,取2,4,6,8(防错用) */
    {               
		if 	(step_out & 0x01) 	step_out += 1;
	}

	step_out = StepTbl[step_out];

    MCU_SetEevStep(no, step_out);
	
	/* 测试程序中步进电机特殊处理，全部为0#电机的输出 */
#if 0
	if (is_testing) {
		if (no != 0) no = 0;
		DIN_BJ2 = (DIN_BJ1 & 0xf0) | step_out;				/* p4低4位 */
		DIN_BJ3 = (DIN_BJ4 & 0x0f) | (step_out << 4);		/* p2高4位 */
		DIN_BJ4 = (DIN_BJ3 & 0xf0) | step_out;				/* p2低4位 */
		for (i = 1; i < MAX_COMP; i++) gIO.io_step_pv[i] = gIO.io_step_pv[0];
	}
#endif
}

/*****************************************************************************
函  数 : HDW_Step()
功  能 : 步进电机
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
void HDW_Step(void)
{
	IO_Step();
}
#endif
