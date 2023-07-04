/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: Hw_def.h
 �ļ�����: CPUӲ������
 ������  : ������
 ��������: 2016.08.22
*******************************************************************************/

#include "includes.h"
#include "iomux.h"



/*******************************************************************************
 �� ��: MCU_DelayUS()
 �� ��: ���ָ����ʱ,��λ΢��
 �� ��: usec--΢����; 
 �� ��: ��
 �� ��: �ú�����ʱ����ȷ
*******************************************************************************/
void MCU_DelayUS(U16 usec)
{
    #define ASM_NOP_1() ASM_NOP()

	/* ע��:����ʾ��������ȷ��(�����2΢�롢4΢�롢2���롢10�����ֵ�����)��while������ٸ�nop�ȽϺ��� */
	while (usec--)
	{
#if SYSCLKCFG == 80
        ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();
        ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();
        ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();
        ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();
        ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();
        ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();
        ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();
        ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();
//      ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();
//      ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();
//      ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();
//      ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();
//      ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();
//      ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();
//      ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();
//      ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();
//      ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();
//      ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();
#elif SYSCLKCFG == 40
        ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();
        ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();
        ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();
        ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();
        ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();ASM_NOP_1();
#else
    #error "δ����Ƶ��,����ʾ����������ʱ."
#endif
	}
    

}

/*******************************************************************************
 �� ��: MCU_DelayMS()
 �� ��: ���ָ����ʱ,��λ����
 �� ��: msec--����; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void MCU_DelayMS(U16 msec)
{

	while (msec-- != 0)
	{
		WDG_Refresh();
		MCU_DelayUS(1000);
	}
}



/*****************************************************************************
��  �� : MCU_WDG_Init()
��  �� : ���Ź���ʼ��
��  �� : ��
��  �� : ��
��  �� : ԭSTM32δ���ÿ��Ź�,��ʹ��8s��ʱ,�Խ��ͷ���.
*****************************************************************************/
void MCU_WDG_Init(void)
{
	stc_hwwdg_config_t wdgConfig;
    
    PDL_ZERO_STRUCT(wdgConfig);

    wdgConfig.u32LoadValue = 100000u;   /* Interval: 1s (@CLKLC:100kHz) */
    wdgConfig.bResetEnable = TRUE;      /* Enables Hardware watchdog reset */
    wdgConfig.pfnHwwdgIrqCb = NULL;

	Hwwdg_Init(&wdgConfig);

	Hwwdg_Start();
}

/*******************************************************************************
 �� ��: MCU_WDG_Refresh()
 �� ��: ���Ź�ˢ��
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void MCU_WDG_Refresh(void)
{
	Hwwdg_Feed(0x55u, 0xAAu);   /* Clear Irq and Reset Timer */
}


/*****************************************************************************
��  �� : MCU_LVD_Rasing()
��  �� : �ϵ����ѹ4.2V
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void MCU_LVD_Rasing(void)
{
    stc_lvd_config_t stcLvdConfig;
    U16 cnt=0;
    
    /* Clear structure */
    PDL_ZERO_STRUCT(stcLvdConfig);
    
    /* Configure LVD ch.0 */
    stcLvdConfig.enLvd0IrqDetectVoltage = Lvd0IrqDetectVoltage420;
    Lvd_Init(&stcLvdConfig); 
    Lvd_EnableIrqDetect(0);
    MCU_DelayMS(10);
    
    // �ȴ���ѹ�ȶ�
    while (TRUE)
    {
        if(Lvd_GetIrqStatus(0u) == TRUE)
        {
            Lvd_ClrIrqStatus(0u);
        }
        else
        {
            break;
        }
        // �رտ���һ��,���¼��
        Lvd_DisableIrqDetect(0);
        MCU_DelayMS(10);
        Lvd_EnableIrqDetect(0);
        MCU_DelayMS(10);
        // ��ʱ
        if (++cnt >= 100)
        {
            break;
        }
    }
    
    Lvd_DisableIrqDetect(0);
    
    MCU_DelayMS(100);
}

/*****************************************************************************
��  �� : MCU_LVD_Falling()
��  �� : ������,��ѹ4.0V
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void MCU_LVD_Falling(void)
{
    stc_lvd_config_t stcLvdConfig;
    
    /* Clear structure */
    PDL_ZERO_STRUCT(stcLvdConfig);
    
    // �����ж�
    stcLvdConfig.enLvd0IrqDetectVoltage = Lvd0IrqDetectVoltage400;
    stcLvdConfig.pfnIrqCallback[0] = LvdCallback;
    Lvd_Init(&stcLvdConfig); 
    Lvd_EnableIrqDetect(0);
}

/*****************************************************************************
��  �� : MCU_LVD_Init()
��  �� : �͵�ѹ����ʼ��
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void MCU_LVD_Init(void)
{
    MCU_LVD_Rasing();
    MCU_LVD_Falling();
}

#if 0
/*****************************************************************************
��  �� : MCU_Debug_Init()
��  �� : DEBUGʱ�ر�IWDT WWDT����������ʱ��ԭ����״̬ 
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void MCU_Debug_Init(void)
{
  DBG->CR =0x3; //DEBUGʱ�ر�IWDT WWDT����������ʱ��ԭ����״̬ 
}
#endif




void MCU_VbatDisable(void)
{
    stc_vbat_config_t stcVbatConfig;
    
    stcVbatConfig.u8ClkDiv = 0x6E;
    stcVbatConfig.bLinkSubClk = FALSE; /* Disconnect sub clock with clock control module */
    stcVbatConfig.bVbatClkEn = FALSE;  /* Stop sub clock supply to VBAT domain */
    stcVbatConfig.enSustainCurrent = Clk445nA;
    stcVbatConfig.enBoostCurrent = Clk510nA;
    stcVbatConfig.enClkBoostTime = ClkBoost50ms;

    Vbat_Init(&stcVbatConfig);
}

void MCU_VbatDeinit(void)
{
    Vbat_InitGpioInput(VbatGpioP46, FALSE);
    Vbat_InitGpioInput(VbatGpioP47, FALSE);
    Vbat_InitGpioInput(VbatGpioP48, FALSE);
    Vbat_InitGpioInput(VbatGpioP49, FALSE);
}


/*******************************************************************************
 �� ��: MCU_Config()
 �� ��: CPUӲ������(�ײ�������ʼ��)
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void MCU_Config(void)
{
    __disable_irq();			//�ر�ȫ���ж�ʹ��
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */

    // �����ж�������ַ
    MCU_IT_Init();
    
    Iomux_DeInit();
    Iomux_Init();

    MCU_VbatDisable();

    MCU_GPIO_Init();


    MCU_WDG_Init();
    
	/* Configure the system clock to 72 MHz */
//	MCU_ClockConfig();

    /* оƬ�ڲ���ѹ��� */
    MCU_LVD_Init();
    
	/* Initialize all configured peripherals */
	MCU_UART_Init();
	MCU_ADC_Init();
	
	/* ������ʱ���ж� */ 
    MCU_TIM_Init();
//	MCU_TIM_Enable(MCU_TIM_COUNT);
    __enable_irq();			//��ȫ���ж�ʹ��

}

/*******************************************************************************
 �� ��: MCU_DeInit()
 �� ��: Ӳ����λ(�˳�Ӧ�ó����ʼ��)
 �� ��: ��
 �� ��: ��
 �� ��: �� wjs ���Ź�Ҳ��������?
*******************************************************************************/
void MCU_DeInit(void)
{
	//��λ��������Ĵ���
//	HAL_DeInit();
    DISABLE_IRQ();
	//��ֹ�����ж� ����Ϊ32λ
	ARRSET(NVIC->ICER, (U32)0xFFFFFFFF);
	//��ֹSysTick�ж�
	SysTick->CTRL = 0;
}



/*****************************************************************************
��  �� : MCU_SetBootUpdateBy()
��  �� : ���ý���BOOT�ķ�ʽ,��BOOTͨ�ŵ�Ψһ��ʽ
         ������UART0�Ĳ����ʼĴ���
��  �� : U08 typ : 
��  �� : ��
��  �� : ��
*****************************************************************************/
void MCU_SetBootUpdateBy(U08 typ)
{
    UART0.BGR = typ;
}







/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  /* wjs ���ӹ��ϵ�ָʾ���п�����壬�ⲿ�������� */
#if 0
  while(1) 
  {
  }
#endif
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif









