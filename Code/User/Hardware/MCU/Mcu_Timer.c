/******************************************************************************

          ��Ȩ���� (C), 2001-2016, �����а��յ��Լ����������޹�˾

 ******************************************************************************
  �� �� ��   : Mcu_Timer.c
  �� �� ��   : ����
  ��    ��   : ��־��
  ��������   : 2018��3��20�����ڶ�
  ����޸�   :
  ��������   : PWM��ʱ���������,ʹ��T12
  �����б�   :
              MCU_SetPwmDuty
              MCU_SetPwmPeriod
              MCU_SetPwmRun
  �޸���ʷ   :
  1.��    ��   : 2018��3��20�����ڶ�
    ��    ��   : ��־��
    �޸�����   : �����ļ�

******************************************************************************/

/*-----------------����ͷ�ļ� -----------------------------*/
#include "Mcu_def.h"
#include "Hdw_Timer.h"
/*-----------------�ⲿ����˵��----------------------------*/



/*-----------------�ⲿ����ԭ��˵��------------------------*/



/*-----------------�ڲ�����ԭ��˵��------------------------*/



/*-----------------ȫ�ֱ���--------------------------------*/



/*-----------------ģ�鼶����------------------------------*/



/*-----------------��������--------------------------------*/



/*-----------------�궨�� ---------------------------------*/
#define TIM_PWM_1             BT5
//#define TIM_PWM_2             BT7  
//#define TIM_PWM_3             BT6

#define TIM_PWC_1             BT3

#define TIM_CNT_1             BT1
#define TIM_CNT_2             BT0

#define TIM_PWM_CLK_P           (16)  // PWMʱ�ӷ�Ƶ
#define TIM_PWM_F2PCSR(f)       ((U32)SYS_CLOCK/TIM_PWM_CLK_P/(f) - 1)
#define TIM_CNT_PCSR            0xFFFFUL

/*****************************************************************************
��  �� : MCU_SetPwm()
��  �� : �趨PWMƵ��/ռ�ձ�(�ߵ�ƽ��)
��  �� : U16 Frequency : Ƶ��
         U16 Duty      : ռ�ձ�,0.1%
         U16 Channel   : ͨ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void MCU_SetPwm(U16 Frequency, U16 Duty, U16 Channel)
{
    U16 period_cnt;
    U16 duty_cnt;
    
    
    /* �޶���Χ */
    if(Frequency < 40 || Frequency > 20000)
    {
        Frequency = 500;
    }

    if(Duty > 1000) 
    {
        Duty = 1000;
    }

    period_cnt = TIM_PWM_F2PCSR(Frequency);
    duty_cnt = ((U32)Duty)*period_cnt/1000;
    
    switch (Channel)	
    {
        case PWM_OUT_1:
            Bt_Pwm_DisableCount(&TIM_PWM_1);
            Bt_Pwm_WriteCycleVal(&TIM_PWM_1, period_cnt);
            Bt_Pwm_WriteDutyVal(&TIM_PWM_1, duty_cnt);
            if (Duty > 0)
            {
                Bt_Pwm_EnableCount(&TIM_PWM_1);
                Bt_Pwm_EnableSwTrig(&TIM_PWM_1);
            }
            break;
//        case PWM_OUT_2:
//            Bt_Pwm_DisableCount(&TIM_PWM_2);
//            Bt_Pwm_WriteCycleVal(&TIM_PWM_2, period_cnt);
//            Bt_Pwm_WriteDutyVal(&TIM_PWM_2, duty_cnt);
//            if (Duty > 0)
//            {
//                Bt_Pwm_EnableCount(&TIM_PWM_2);
//                Bt_Pwm_EnableSwTrig(&TIM_PWM_2);
//            }
//            break;
//        case PWM_OUT_3:
//            Bt_Pwm_DisableCount(&TIM_PWM_3);
//            Bt_Pwm_WriteCycleVal(&TIM_PWM_3, period_cnt);
//            Bt_Pwm_WriteDutyVal(&TIM_PWM_3, duty_cnt);
//            if (Duty > 0)
//            {
//                Bt_Pwm_EnableCount(&TIM_PWM_3);
//                Bt_Pwm_EnableSwTrig(&TIM_PWM_3);
//            }
//            break;
        default:
            break;
    }
}




/*****************************************************************************
��  �� : MCU_PwmInit()
��  �� : 
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void MCU_PwmInit(void)
{
    stc_bt_pwm_config_t stcPwmConfig;


    /* Clear structures */
    PDL_ZERO_STRUCT(stcPwmConfig);

#if TIM_PWM_CLK_P != 16
    #error "�޸�enPres,���޸Ĵ�����"
#endif
    /* Initialize PWM timer */
    stcPwmConfig.enPres = PwmPres1Div16;  /* PWM clock = 2.5MHz @ PCLK = 40MHz */
    stcPwmConfig.enMode = PwmContinuous; 
    stcPwmConfig.enExtTrig = PwmExtTrigDisable;
    stcPwmConfig.enOutputMask = PwmOutputNormal;
    stcPwmConfig.enOutputPolarity = PwmPolarityLow;
    stcPwmConfig.enRestartEn = PwmRestartEnable;    
    stcPwmConfig.bTouchNvic = FALSE;

    /* Set requested I/O mode  */
    Bt_ConfigIOMode(&TIM_PWM_1, BtIoMode0);
    Bt_Pwm_Init(&TIM_PWM_1 , &stcPwmConfig);
    /* Set cycle and duty value */
    Bt_Pwm_WriteCycleVal(&TIM_PWM_1, TIM_PWM_F2PCSR(1000));     // 1000HZ
    Bt_Pwm_WriteDutyVal(&TIM_PWM_1, 0);                         // 0.0%

#if 0
    Bt_ConfigIOMode(&TIM_PWM_2, BtIoMode0);
    Bt_Pwm_Init(&TIM_PWM_2 , &stcPwmConfig);
    /* Set cycle and duty value */
    Bt_Pwm_WriteCycleVal(&TIM_PWM_2, TIM_PWM_F2PCSR(1000));     // 1000HZ
    Bt_Pwm_WriteDutyVal(&TIM_PWM_2, 0);                         // 0.0%
            

    Bt_ConfigIOMode(&TIM_PWM_3, BtIoMode0);
    Bt_Pwm_Init(&TIM_PWM_3 , &stcPwmConfig);
    /* Set cycle and duty value */
    Bt_Pwm_WriteCycleVal(&TIM_PWM_3, TIM_PWM_F2PCSR(1000));     // 1000HZ
    Bt_Pwm_WriteDutyVal(&TIM_PWM_3, 0);                         // 0.0%
#endif
}

#if 0
// ���������
void MCU_PwcMeasureCompleteCb(void)
{
    Bt_Pwc_ClrIrqFlag(&TIM_PWC_1, PwcMeasureCompleteIrq);
    HDW_PwcEdgeInt();
}


void MCU_PwcMeasureOverflowCb(void)
{
    Bt_Pwc_ClrIrqFlag(&TIM_PWC_1, PwcMeasureOverflowIrq);
    HDW_PwcTimeoutInt();
}



U16 MCU_PwcGetMeasure(void)
{
    return Bt_Pwc_Get16BitMeasureData(&TIM_PWC_1);
}
#endif

// ����ʱ��
void MCU_PwcExIntCb(void)
{
    Bt_Rt_ClrIrqFlag(&TIM_PWC_1, RtUnderflowIrq);
    HDW_PwcEdgeInt();
}

// ����ʱ��
void MCU_PwcCounterUnderflowCb(void)
{
    Bt_Rt_ClrIrqFlag(&TIM_PWC_1, RtUnderflowIrq);
    HDW_PwcTimeoutInt();
}

U16 MCU_PwcGetCounter(void)
{
    U16 counter = 0;
    
    Bt_Rt_DisableCount(&TIM_PWC_1);
    counter = TIM_CNT_PCSR - Bt_Rt_ReadCurCnt(&TIM_PWC_1);
    Bt_Rt_EnableCount(&TIM_PWC_1);
    Bt_Rt_EnableSwTrig(&TIM_PWC_1);
    
    return counter;
}


/*****************************************************************************
��  �� : MCU_PwcInit()
��  �� : PWC���������ʼ��,ʹ���ⲿIO�ж�+��ʱ��ʵ��
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void MCU_PwcInit(void)
{
    stc_bt_rt_config_t stcRtConfig;
    stc_rt_irq_en_t   stcIrqEn;
    stc_rt_irq_cb_t    stcIrqCb;
    
    // Clear structures
    PDL_ZERO_STRUCT(stcRtConfig);
    PDL_ZERO_STRUCT(stcIrqEn);
    PDL_ZERO_STRUCT(stcIrqCb);

    /* Initialize interrupt */
    stcRtConfig.pstcRtIrqEn = &stcIrqEn;
    stcRtConfig.pstcRtIrqCb = &stcIrqCb;
    /* Set BT IO mode */ 
    Bt_ConfigIOMode(&TIM_PWC_1, BtIoMode0);
    
    /* Initialize BT */
    stcRtConfig.enPres = RtPres1Div16; /* f < 0xFFFF*50Hz = 3.2M */
    stcRtConfig.enSize = RtSize16Bit;
    stcRtConfig.enMode = RtReload;
    stcRtConfig.enExtTrig = RtExtTiggerDisable;
    stcRtConfig.enOutputPolarity = RtPolarityLow;
    stcRtConfig.pstcRtIrqEn->bRtTrigIrq = FALSE;
    stcRtConfig.pstcRtIrqEn->bRtUnderflowIrq = TRUE;
    stcRtConfig.pstcRtIrqCb->pfnRtTrigIrqCb = NULL ;
    stcRtConfig.pstcRtIrqCb->pfnRtUnderflowIrqCb = MCU_PwcCounterUnderflowCb;
    stcRtConfig.bTouchNvic = TRUE;
    Bt_Rt_Init(&TIM_PWC_1, &stcRtConfig);

    Bt_Rt_WriteCycleVal(&TIM_PWC_1, TIM_CNT_PCSR);  /* Cycle = T*(m+1)=4ms @ T = 0.2us */
    
    /* Enable count operatoin */
    Bt_Rt_EnableCount(&TIM_PWC_1);

    Bt_Rt_EnableSwTrig(&TIM_PWC_1);

    // �������ű�Ե�ж�
    {
        stc_exint_config_t stcExintConfig;

        SetPinFunc_INT00_1(0u);

        /* Configure interrupt ch.6 and ch.8 */
        PDL_ZERO_STRUCT(stcExintConfig);
        
        /* Before initialize external interrupt ch.6, make sure 
        * PDL_PERIPHERAL_ENABLE_EXINT0 is set to PDL_ON in pdl_user.h
        * If external interrupt ch.6 is not used, set PDL_PERIPHERAL_ENABLE_EXINT6 
        * to PDL_OFF !! Otherwise, the external interrupt ch.6 may be mis-enabled. */
        stcExintConfig.abEnable[ExintInstanceIndexExint0] = 1u;
        stcExintConfig.aenLevel[ExintInstanceIndexExint0] = ExIntBothEdge;
        stcExintConfig.apfnExintCallback[ExintInstanceIndexExint0] = MCU_PwcExIntCb;
        stcExintConfig.bTouchNvic = TRUE;

        Exint_Init(&stcExintConfig);
    }
}

void MCU_Cnt1UnderflowCb(void)
{
    Bt_Rt_ClrIrqFlag(&TIM_CNT_1, RtUnderflowIrq);
    HDW_CounterOverInt(FREQ_IN_1);
}

void MCU_Cnt2UnderflowCb(void)
{
    Bt_Rt_ClrIrqFlag(&TIM_CNT_2, RtUnderflowIrq);
    HDW_CounterOverInt(FREQ_IN_2);
}

U16 MCU_CntGetCounter(U08 cnt_num)
{
    U16 counter = 0;
    switch (cnt_num)
    {
        case FREQ_IN_1:
            counter = TIM_CNT_PCSR - Bt_Rt_ReadCurCnt(&TIM_CNT_1);
            break;
        case FREQ_IN_2:
            counter = TIM_CNT_PCSR - Bt_Rt_ReadCurCnt(&TIM_CNT_2);
            break;
    }

    return counter;
}

void MCU_CntStop(U08 cnt_num)
{
    switch (cnt_num)
    {
        case FREQ_IN_1:
            Bt_Rt_DisableCount(&TIM_CNT_1);
            break;
        case FREQ_IN_2:
            Bt_Rt_DisableCount(&TIM_CNT_2);
            break;
    }
}

void MCU_CntStart(U08 cnt_num)
{
    switch (cnt_num)
    {
        case FREQ_IN_1:
            Bt_Rt_WriteCycleVal(&TIM_CNT_1, TIM_CNT_PCSR);
            Bt_Rt_EnableCount(&TIM_CNT_1);
            Bt_Rt_EnableSwTrig(&TIM_CNT_1);
            break;
        case FREQ_IN_2:
            Bt_Rt_WriteCycleVal(&TIM_CNT_2, TIM_CNT_PCSR);
            Bt_Rt_EnableCount(&TIM_CNT_2);
            Bt_Rt_EnableSwTrig(&TIM_CNT_2);
            break;
    }
}

/*****************************************************************************
��  �� : MCU_CntInit()
��  �� : ���������ʼ��
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void MCU_CntInit(void)
{
    stc_bt_rt_config_t stcRtConfig;
    stc_rt_irq_en_t   stcIrqEn;
    stc_rt_irq_cb_t    stcIrqCb;
   en_bt_io_mode_t mod = BtIoMode0;

    /* Clear structures */
    PDL_ZERO_STRUCT(stcRtConfig);
    PDL_ZERO_STRUCT(stcIrqEn);
    PDL_ZERO_STRUCT(stcIrqCb);
    
    /* Initialize interrupt */
    stcRtConfig.pstcRtIrqEn = &stcIrqEn;
    stcRtConfig.pstcRtIrqCb = &stcIrqCb;
    /* Set BT IO mode */ 
    Bt_ConfigIOMode(&TIM_CNT_1, BtIoMode0);
        
    /* Initialize BT */
    stcRtConfig.enPres = RtPres1ExtClkRising; /* BT_CLK_DIV_4: T = 0.2us @ PCLK = 20MHz */
    stcRtConfig.enSize = RtSize16Bit;
    stcRtConfig.enMode = RtReload;
    stcRtConfig.enExtTrig = RtExtTiggerDisable;
    stcRtConfig.enOutputPolarity = RtPolarityLow;
    stcRtConfig.pstcRtIrqEn->bRtUnderflowIrq = 1;
    stcRtConfig.pstcRtIrqCb->pfnRtUnderflowIrqCb = MCU_Cnt1UnderflowCb;
    stcRtConfig.bTouchNvic = TRUE;
    Bt_Rt_Init(&TIM_CNT_1, &stcRtConfig);
    
    /* Write cycle value */
    Bt_Rt_WriteCycleVal(&TIM_CNT_1, TIM_CNT_PCSR);  /* Cycle = T*(m+1)=4ms @ T = 0.2us */
  
    /* Start Reload timer by software trigger */
    Bt_Rt_EnableCount(&TIM_CNT_1);
    Bt_Rt_EnableSwTrig(&TIM_CNT_1);


    Bt_ConfigIOMode(&TIM_CNT_2, mod);
    stcRtConfig.pstcRtIrqCb->pfnRtUnderflowIrqCb = MCU_Cnt2UnderflowCb;
    Bt_Rt_Init(&TIM_CNT_2, &stcRtConfig);
    /* Write cycle value */
    Bt_Rt_WriteCycleVal(&TIM_CNT_2, TIM_CNT_PCSR);  /* Cycle = T*(m+1)=4ms @ T = 0.2us */
  
    /* Start Reload timer by software trigger */
    Bt_Rt_EnableCount(&TIM_CNT_2);
    Bt_Rt_EnableSwTrig(&TIM_CNT_2);

}

/*******************************************************************************
 �� ��: MCU_Tim1MsInit()
 �� ��: ��ʱ����ʼ��:���ڲ���1ms��ʱ�ж�
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void MCU_Tim1MsInit(void)
{
    SystemCoreClockUpdate();  
    SysTick_Config(SystemCoreClock / 1000);
}





/*****************************************************************************
��  �� : MCU_TIM_Init()
��  �� : 
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void MCU_TIM_Init(void)
{
    MCU_PwmInit();
    MCU_PwcInit();
//    MCU_CntInit();
    MCU_Tim1MsInit();
}

#if 0
/*****************************************************************************
��  �� : SysTick_Handler()
��  �� : 
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void SysTick_Handler(void)
{
    HDW_1ms_Deal();
	USER_1ms_Deal();
#ifdef USE_HDW_DEBUG
	DBG_1ms_Deal();
#endif
}
#endif


