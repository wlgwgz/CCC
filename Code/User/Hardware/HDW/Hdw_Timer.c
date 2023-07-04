/******************************************************************************

          ��Ȩ���� (C), 2001-2014, �����а��յ��Լ����������޹�˾

 ******************************************************************************
  �� �� ��   : Hdw_Timer.c
  �� �� ��   : ����
  ��    ��   : // zzp v160a7
  ��������   : 2016��11��8�����ڶ�
  ����޸�   :
  ��������   : ʱ��/��ʱ��/���������
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��11��8�����ڶ�
    ��    ��   : // zzp v160a7
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include "includes.h"

/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
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
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/
//static void HDW_ReadCount(void);	/* N0004 */
/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/
U32 HdwTickMs;

PWMOUT HdwPwmOut[PWM_OUT_MAX];
PWCIN HdwPwcIn;
FREQIN HdwFreq[FREQ_IN_NUM];
/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �궨��                                       *
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
 �� ��: HDW_DelayUs()
 �� ��: ��ʱ΢��
 �� ��: usec--΢��; 
 �� ��: ��
 �� ��: ����ʱ�Ӹߣ�һ��nopָ���������뼶������ѭ��ʵ��΢�뼶��ʱ��
        ��������ʱ�ӣ���ͬ�����¡����Ըú���(����ʾ��������)��
        �ú�����ʱ����ȷ�����ö�ʱ����ʵ�־�ȷ��ʱ��
        �������ж�������ָ��ʱ�䣬����Щ���(Ҳû��ϵ)�����豣֤��ʱʱ���ǹ��ģ�
        ʾ����ʵ������:��ʱ1usʱ��ʵ����Լ1.5us����ʱ1ms��ʵ����Լ1.1ms
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
//��  �� : HDW_ReadCount()
//��  �� : ��ȡ�������
//��  �� : U16 timer : 
//��  �� : ��
//��  �� : ��
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
��  �� : HDW_PwcEdgeInt()
��  �� : PWC������������ж�
��  �� : U16 timer : 
��  �� : ��
��  �� : ��
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
            if (edge)   // ������,���������
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
            if (!edge)  // ������,���������
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
��  �� : HDW_PwmInDutyCal()
��  �� : ��̨����PWM����ռ�ձ�(�ߵ�ƽ),����0.1%
��  �� : ��
��  �� : ռ�ձ�
��  �� : ��
*****************************************************************************/
void HDW_PwmInDutyCal(void)
{
    PWCIN* p = &HdwPwcIn;
    
    U08 i;

    U16 duty = 0;

    #if MAX_PWCIN_FILTER <= 2
    #error "MAC_PWMIN_FILTER�������2"
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
        /* ����ռ�ձ� */
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

        //      /* �˲� */
        p->dutyfilter[p->filtercnt] = duty;
        if(++p->filtercnt >= MAX_PWCIN_FILTER)
        {
            U16 dutytmp;
            U16 min, max;
            U32 sum=0;
            
            p->filtercnt = 0;
            
            /* ͳ�Ƶ͵�ƽ */
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
��  �� : PunpGetPwmInDuty()
��  �� : ��ȡPWM����ռ�ձ�(�ߵ�ƽ),����0.1%
��  �� : ��
��  �� : ռ�ձ�
��  �� : ��
*****************************************************************************/
U16 PunpGetPwmInDuty(void)
{
    return HdwPwcIn.duty;
}

/* N0004 */
/*****************************************************************************
��  �� : PunpGetPwmInput()
��  �� : ��ȡPWM�����������
��  �� : ��
��  �� : ռ�ձ�
��  �� : ��
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
��  �� : PunpSetPwmFreqDuty()
��  �� : ����PWMƵ�ʺ�ռ�ձ�(�ߵ�ƽ��)
��  �� : U16 Freq    : Ƶ��
         U16 Duty    : ռ�ձ�0.1%
         U16 Channel : ͨ��
��  �� : ��
��  �� : ��
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



