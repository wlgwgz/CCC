/******************************************************************************

          版权所有 (C), 2001-2014, 广州市邦普电脑技术开发有限公司

 ******************************************************************************
  文 件 名   : Hdw_Timer.c
  版 本 号   : 初稿
  作    者   : // zzp v160a7
  生成日期   : 2016年11月8日星期二
  最近修改   :
  功能描述   : 时间/定时器/计数器相关
  函数列表   :
  修改历史   :
  1.日    期   : 2016年11月8日星期二
    作    者   : // zzp v160a7
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "includes.h"

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/
extern void HDW_GetPhase(void);
extern void HDW_Step(void);
extern void HDW_GetDi(void);
extern void HDW_GetSwitch(void);
extern void HDW_GetSwPl(void);
extern void HDW_AdcSampleTestDiff(void);
extern void HDW_AdcSampleNtc(void);
extern void HDW_AdcSampleCur(void);
extern void HDW_AdcSampleAi(void);
/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/
//static void HDW_ReadCount(void);	/* N0004 */
/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/
U32 HdwTickMs;

PWMOUT HdwPwmOut[PWM_OUT_MAX];
PWCIN HdwPwcIn;
FREQIN HdwFreq[FREQ_IN_NUM];
/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
void HDW_1ms_Deal(void)
{
    static U08 cnt10ms = 0;
    static U16 cnt1000ms = 0;

    HdwTickMs++;

    HDW_AdcSample();
	HDW_GetHDI();
        
    if (++cnt10ms >= 10) cnt10ms = 0;
    if (++cnt1000ms >= 1000)
    {
        cnt1000ms = 0;
    }
    switch (cnt10ms)
    {
        case 0  :   
            HDW_GetInput();
            break;
        case 1  : 
            break;      
        case 2  : 
            break;
        case 3  :       
            break;
        case 4  :       
            break;
        case 5  :       
            break;
        case 6  : 
            break;
        case 7  :      
            break;
        case 8  :          
            break;
        case 9  :   
           ;// HDW_ReadCount();
            break;
        default :   break;
    }
}

U32 HDW_1ms_Tick(void)
{
    U32 Val;
    
    ENTER_CRITICAL();
    Val = HdwTickMs;
    EXIT_CRITICAL();
    
    return Val; 
}

/*******************************************************************************
 函 数: HDW_DelayUs()
 功 能: 延时微秒
 参 数: usec--微秒; 
 返 回: 空
 其 它: 由于时钟高，一个nop指令属于纳秒级，故用循环实现微秒级延时，
        若更改了时钟，请同步更新、测试该函数(如用示波器测试)；
        该函数延时不精确，可用定时器来实现精确延时。
        忽略了判断条件等指令时间，会有些误差(也没关系)，但需保证延时时间是够的，
        示波器实测数据:延时1us时，实际上约1.5us，延时1ms，实际上约1.1ms
*******************************************************************************/
void HDW_DelayUs(U16 usec)
{ 
    MCU_DelayUS(usec);
}

void HDW_DelayMs(U16 msec)
{
    U16 cnt = msec;

    while (cnt-- != 0)
    {
        HDW_DelayUs(1000);
    }
}

///*****************************************************************************
//函  数 : HDW_ReadCount()
//功  能 : 读取脉冲个数
//参  数 : U16 timer : 
//返  回 : 无
//其  他 : 无
//*****************************************************************************/
//static void HDW_ReadCount(void)
//{
//    //static U08 tmp=0;
//	
//    MCU_CntStop(FREQ_IN_1);
//    MCU_CntStop(FREQ_IN_2);
//    HdwFreq[FREQ_IN_1].freq = MCU_CntGetCounter(FREQ_IN_1);
//    HdwFreq[FREQ_IN_2].freq = MCU_CntGetCounter(FREQ_IN_2);
//    MCU_CntStart(FREQ_IN_1);
//    MCU_CntStart(FREQ_IN_2);
//    pwminput_fg = TRUE;
//}

void HDW_CounterOverInt(U08 cnt_num)
{
    HdwFreq[cnt_num].freq = 0xFFFF;
    MCU_CntStart(cnt_num);
}

/*****************************************************************************
函  数 : HDW_PwcEdgeInt()
功  能 : PWC脉宽测量边沿中断
参  数 : U16 timer : 
返  回 : 无
其  他 : 无
*****************************************************************************/
void HDW_PwcEdgeInt(void)
{

    
    
    BOL edge = MCU_GetPwcIn();
    U16 pwc_count = MCU_PwcGetCounter();
    PWCIN* p = &HdwPwcIn;


    if (p->cal) return;
    
    switch (p->level)
    {
        default:
        case PWC_LEVEL_UNKNOW:
            p->count_low = 0;
            p->count_high = 0;
            p->edge = 0;    
            p->cal = FALSE;
            if (edge)
            {
                p->level = PWC_LEVEL_HIGH;
            }
            else
            {
                p->level = PWC_LEVEL_LOW;
            }
            break;
            
        case PWC_LEVEL_LOW:
            if (edge)   // 上升沿,捕获低脉宽
            {
                p->edge |= PWC_EDGE_LOW;
                p->count_low = pwc_count;
                p->level = PWC_LEVEL_HIGH;
            }
            else
            {
                p->edge = 0;
                p->lowfull = TRUE;
                p->cal = TRUE;
            }
            break;
            
        case PWC_LEVEL_HIGH:
            if (!edge)  // 下升沿,捕获高脉宽
            {
                p->edge |= PWC_EDGE_HIGH;
                p->count_high = pwc_count;
                p->level = PWC_LEVEL_LOW;
                if (p->edge & PWC_EDGE_LOW)
                {
                    p->lowfull = FALSE;
                    p->highfull = FALSE;
                    p->edge = 0;
                    p->cal = TRUE;
                }
            }
            else
            {
                p->edge = 0;
                p->highfull = TRUE;
                p->cal = TRUE;
            }
            break;
    }
}



void HDW_PwcTimeoutInt(void)
{

    PWCIN* p = &HdwPwcIn;

    if(MCU_GetPwcIn())
    {
        p->highfull = TRUE;
        p->lowfull = FALSE;
        p->edge = 0;
        p->cal = TRUE;
    }
    else
    {
        p->highfull = FALSE;
        p->lowfull = TRUE;
        p->edge = 0;
        p->cal = TRUE;
    }


}


/*****************************************************************************
函  数 : HDW_PwmInDutyCal()
功  能 : 后台计算PWM输入占空比(高电平),精度0.1%
参  数 : 无
返  回 : 占空比
其  他 : 无
*****************************************************************************/
void HDW_PwmInDutyCal(void)
{
    PWCIN* p = &HdwPwcIn;
    
    U08 i;

    U16 duty = 0;

    #if MAX_PWCIN_FILTER <= 2
    #error "MAC_PWMIN_FILTER必须大于2"
    #endif

    if (!p->init)
    {
        p->init = TRUE;
        
        if (MCU_GetPwcIn())
        {
            duty = 1000;
        }
        else
        {
            duty = 0;
        }

        ARRSET(p->dutyfilter, duty);
        p->duty = duty;
    }


    if(p->cal)
    {
        /* 计算占空比 */
        if(p->highfull) duty = 1000;
        else if(p->lowfull) duty = 0;
        else
        {
            U32 sum = p->count_high+p->count_low;
            
            if (sum > 0)
            {
                duty = _DIVUROUND(p->count_high*1000UL, sum);
            }
        }

        //      /* 滤波 */
        p->dutyfilter[p->filtercnt] = duty;
        if(++p->filtercnt >= MAX_PWCIN_FILTER)
        {
            U16 dutytmp;
            U16 min, max;
            U32 sum=0;
            
            p->filtercnt = 0;
            
            /* 统计低电平 */
            min = max = p->dutyfilter[0];
            for (i=0; i<MAX_PWCIN_FILTER; i++)
            {
                dutytmp = p->dutyfilter[i];
                sum += dutytmp;
                if(dutytmp > max) max = dutytmp;
                if(dutytmp < min) min = dutytmp;
            }
            sum -= max, sum -= min;
                
            p->duty = _DIVUROUND(sum, MAX_PWCIN_FILTER-2);
        }

        p->level = PWC_LEVEL_UNKNOW;
        p->cal = FALSE;
    }
}



/*****************************************************************************
函  数 : PunpGetPwmInDuty()
功  能 : 获取PWM输入占空比(高电平),精度0.1%
参  数 : 无
返  回 : 占空比
其  他 : 无
*****************************************************************************/
U16 PunpGetPwmInDuty(void)
{
    return HdwPwcIn.duty;
}

/* N0004 */
/*****************************************************************************
函  数 : PunpGetPwmInput()
功  能 : 获取PWM输入脉冲个数
参  数 : 无
返  回 : 占空比
其  他 : 无
*****************************************************************************/
U16 PunpGetPwmInput(U16 inchnel)
{
    U16 tmp=0;
    
    if (inchnel < FREQ_IN_NUM)
    {
        tmp = HdwFreq[inchnel].freq;
    }

    return tmp;
}



/*****************************************************************************
函  数 : PunpSetPwmFreqDuty()
功  能 : 设置PWM频率和占空比(高电平比)
参  数 : U16 Freq    : 频率
         U16 Duty    : 占空比0.1%
         U16 Channel : 通道
返  回 : 无
其  他 : 无
*****************************************************************************/
void PunpSetPwmFreqDuty(U16 Freq, U16 Duty, U16 Channel)
{
    PWMOUT* p=&HdwPwmOut[Channel];
    
    if (p->frequency != Freq
        || p->curduty != Duty)
    {
        p->frequency = Freq;
        p->curduty = Duty;

        MCU_SetPwm(Freq, Duty, Channel);
    }
    
}



