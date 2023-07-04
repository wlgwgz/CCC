/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: Hw_def.h
 文件描述: CPU硬件配置
 创建人  : 王继松
 创建日期: 2016.08.22
*******************************************************************************/

#include "includes.h"
#include "iomux.h"



/*******************************************************************************
 函 数: MCU_DelayUS()
 功 能: 软件指令延时,单位微秒
 参 数: usec--微秒数; 
 返 回: 空
 其 它: 该函数延时不精确
*******************************************************************************/
void MCU_DelayUS(U16 usec)
{
    #define ASM_NOP_1() ASM_NOP()

	/* 注意:得用示波器测试确认(如测试2微秒、4微秒、2毫秒、10毫秒等值的情况)，while估算多少个nop比较合适 */
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
    #error "未测试频率,请用示波器测试延时."
#endif
	}
    

}

/*******************************************************************************
 函 数: MCU_DelayMS()
 功 能: 软件指令延时,单位毫秒
 参 数: msec--毫秒; 
 返 回: 空
 其 它: 空
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
函  数 : MCU_WDG_Init()
功  能 : 看门狗初始化
参  数 : 无
返  回 : 无
其  他 : 原STM32未启用看门狗,故使用8s超时,以降低风险.
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
 函 数: MCU_WDG_Refresh()
 功 能: 看门狗刷新
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void MCU_WDG_Refresh(void)
{
	Hwwdg_Feed(0x55u, 0xAAu);   /* Clear Irq and Reset Timer */
}


/*****************************************************************************
函  数 : MCU_LVD_Rasing()
功  能 : 上电监测电压4.2V
参  数 : 无
返  回 : 无
其  他 : 无
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
    
    // 等待电压稳定
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
        // 关闭开启一次,重新检查
        Lvd_DisableIrqDetect(0);
        MCU_DelayMS(10);
        Lvd_EnableIrqDetect(0);
        MCU_DelayMS(10);
        // 超时
        if (++cnt >= 100)
        {
            break;
        }
    }
    
    Lvd_DisableIrqDetect(0);
    
    MCU_DelayMS(100);
}

/*****************************************************************************
函  数 : MCU_LVD_Falling()
功  能 : 掉电监测,电压4.0V
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
void MCU_LVD_Falling(void)
{
    stc_lvd_config_t stcLvdConfig;
    
    /* Clear structure */
    PDL_ZERO_STRUCT(stcLvdConfig);
    
    // 开启中断
    stcLvdConfig.enLvd0IrqDetectVoltage = Lvd0IrqDetectVoltage400;
    stcLvdConfig.pfnIrqCallback[0] = LvdCallback;
    Lvd_Init(&stcLvdConfig); 
    Lvd_EnableIrqDetect(0);
}

/*****************************************************************************
函  数 : MCU_LVD_Init()
功  能 : 低电压监测初始化
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
void MCU_LVD_Init(void)
{
    MCU_LVD_Rasing();
    MCU_LVD_Falling();
}

#if 0
/*****************************************************************************
函  数 : MCU_Debug_Init()
功  能 : DEBUG时关闭IWDT WWDT保持其他定时器原来的状态 
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
void MCU_Debug_Init(void)
{
  DBG->CR =0x3; //DEBUG时关闭IWDT WWDT保持其他定时器原来的状态 
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
 函 数: MCU_Config()
 功 能: CPU硬件配置(底层驱动初始化)
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void MCU_Config(void)
{
    __disable_irq();			//关闭全局中断使能
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */

    // 设置中断向量地址
    MCU_IT_Init();
    
    Iomux_DeInit();
    Iomux_Init();

    MCU_VbatDisable();

    MCU_GPIO_Init();


    MCU_WDG_Init();
    
	/* Configure the system clock to 72 MHz */
//	MCU_ClockConfig();

    /* 芯片内部电压监测 */
    MCU_LVD_Init();
    
	/* Initialize all configured peripherals */
	MCU_UART_Init();
	MCU_ADC_Init();
	
	/* 开启定时器中断 */ 
    MCU_TIM_Init();
//	MCU_TIM_Enable(MCU_TIM_COUNT);
    __enable_irq();			//打开全局中断使能

}

/*******************************************************************************
 函 数: MCU_DeInit()
 功 能: 硬件复位(退出应用程序初始化)
 参 数: 空
 返 回: 空
 其 它: 空 wjs 看门狗也包括了吗?
*******************************************************************************/
void MCU_DeInit(void)
{
	//复位所有外设寄存器
//	HAL_DeInit();
    DISABLE_IRQ();
	//禁止所有中断 更改为32位
	ARRSET(NVIC->ICER, (U32)0xFFFFFFFF);
	//禁止SysTick中断
	SysTick->CTRL = 0;
}



/*****************************************************************************
函  数 : MCU_SetBootUpdateBy()
功  能 : 设置进入BOOT的方式,和BOOT通信的唯一方式
         复用了UART0的波特率寄存器
参  数 : U08 typ : 
返  回 : 无
其  他 : 无
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
  /* wjs 增加故障灯指示，有块试验板，外部晶振不起振 */
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









