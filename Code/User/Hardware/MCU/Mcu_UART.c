/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: hw_uart.c
 程序版本: V1.00
 文件描述: 串口底层驱动,为上层软件提供硬件操作接口,
 		   其中包括硬件操作,程序库使用,编译器依赖性的封装隔离
 创建人  : 杨华
 创建日期: 2015.01.06
 ================================ 修改记录 ==================================== 
 修改人  : 
 程序版本: 
 完成日期: 
 修改内容: 
 ******************************************************************************/
 
/* Includes ------------------------------------------------------------------*/
#include "includes.h"


static UART_HandleTypeDef HUart[MCU_UART_COUNT];


/*****************************************************************************
函  数 : MCU_UART_MspInit()
功  能 : 串口引脚初始化
参  数 : U08 num : 
返  回 : 无
其  他 : 无
*****************************************************************************/
void MCU_UART_MspInit(U08 num)
{
    switch (num)
    {
        case MCU_UART0_NUM:
        	SetPinFunc_SOT0_0();
        	SetPinFunc_SIN0_0();
            break;
            
        case MCU_UART1_NUM:
            SetPinFunc_SOT1_0();
        	SetPinFunc_SIN1_0();
            break;
            
        case MCU_UART4_NUM:
            SetPinFunc_SOT4_0();
        	SetPinFunc_SIN4_0();
            break;
            
        case MCU_UART5_NUM:
            SetPinFunc_SOT5_0();
        	SetPinFunc_SIN5_0();
            break;
            
        case MCU_UART6_NUM:
            SetPinFunc_SOT6_0();
        	SetPinFunc_SIN6_0();
            break;    
    }


    //UART初始化配置,默认9600 8N1
    MCU_UART_SetFormat(num, 9600, MCU_UART_8_N_1);
}




/*******************************************************************************
 函 数: MCU_UART_Init()
 功 能: 串口底层驱动初始化
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void MCU_UART_Init(void)
{
	U08 num;
	UART_HandleTypeDef *pHUart;

	for (num=0; num<MCU_UART_COUNT; num++)
	{
		pHUart = &HUart[num];
		switch (num)
		{
            default:	
            case MCU_UART0_NUM:
				pHUart->Instance = &UART0;
                break;
            case MCU_UART1_NUM:
                pHUart->Instance = &UART1;
                break;
			case MCU_UART4_NUM:
				pHUart->Instance = &UART4;
				break;	
            case MCU_UART5_NUM:
				pHUart->Instance = &UART5;
				break;
            case MCU_UART6_NUM:
				pHUart->Instance = &UART6;
				break;    
		}
        MCU_UART_MspInit(num);
	}
}

/*******************************************************************************
 函 数: MCU_UART_SetFormat()
 功 能: 设置串口通讯格式
 参 数: num--串口编号; 
		baudRate--波特率; 
		dataType--数据类型; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void MCU_UART_SetFormat(U08 num, U32 baudRate, MCU_UART_FormatTypeDef dataType)
{
    stc_mfs_uart_config_t stcUartConfig;
    stc_uart_irq_cb_t stcUartIrqCb;
    volatile stc_mfsn_uart_t* uart = HUart[num].Instance;
    PDL_ZERO_STRUCT(stcUartConfig);
    PDL_ZERO_STRUCT(stcUartIrqCb);
    
    /* Initailize UART interrupt callback functions */
    switch (num)
    {
        case MCU_UART0_NUM:
            stcUartIrqCb.pfnTxIdleCb = Uart0TxIntCallback; // 类似于STM32的TC中断
            stcUartIrqCb.pfnRxIrqCb  = Uart0RxIntCallback;
            break;
        case MCU_UART1_NUM:
            stcUartIrqCb.pfnTxIdleCb = Uart1TxIntCallback; // 类似于STM32的TC中断
            stcUartIrqCb.pfnRxIrqCb  = Uart1RxIntCallback;
            break;    
        case MCU_UART4_NUM:
            stcUartIrqCb.pfnTxIdleCb = Uart4TxIntCallback; // 类似于STM32的TC中断
            stcUartIrqCb.pfnRxIrqCb  = Uart4RxIntCallback;
            break;
        case MCU_UART5_NUM:
            stcUartIrqCb.pfnTxIdleCb = Uart5TxIntCallback; // 类似于STM32的TC中断
            stcUartIrqCb.pfnRxIrqCb  = Uart5RxIntCallback;
            break;
        case MCU_UART6_NUM:
            stcUartIrqCb.pfnTxIdleCb = Uart6TxIntCallback; // 类似于STM32的TC中断
            stcUartIrqCb.pfnRxIrqCb  = Uart6RxIntCallback;
            break;   
    }
    
    /* Initialize UART TX and RX channel  */
    stcUartConfig.enMode = UartNormal;
    stcUartConfig.enBitDirection = UartDataLsbFirst; 
    stcUartConfig.bInvertData = FALSE;
    stcUartConfig.bHwFlow = FALSE;
    stcUartConfig.pstcFifoConfig = NULL;
    stcUartConfig.bUseExtClk = FALSE;
    stcUartConfig.pstcIrqEn = NULL;
    stcUartConfig.pstcIrqCb = &stcUartIrqCb;
    stcUartConfig.bTouchNvic = TRUE;
    
	/* 波特率 */
	stcUartConfig.u32BaudRate = baudRate;
	
	/* 数据格式 */
	switch (dataType)
	{
		case MCU_UART_8_N_1:
			stcUartConfig.enDataLength = UartEightBits;
			stcUartConfig.enStopBit = UartOneStopBit;
			stcUartConfig.enParity = UartParityNone;
			break;
		case MCU_UART_8_O_1:
			stcUartConfig.enDataLength = UartEightBits;
			stcUartConfig.enStopBit = UartOneStopBit;
			stcUartConfig.enParity = UartParityOdd;
			break;
		case MCU_UART_8_E_1:
			stcUartConfig.enDataLength = UartEightBits;
			stcUartConfig.enStopBit = UartOneStopBit;
			stcUartConfig.enParity = UartParityEven;
			break;
		case MCU_UART_8_N_2:
		default:
			stcUartConfig.enDataLength = UartEightBits;
			stcUartConfig.enStopBit = UartTwoStopBits;
			stcUartConfig.enParity = UartParityNone;
			break;
	}	


    Mfs_Uart_Init(uart, &stcUartConfig);

    /* Enable RX function of UART0 */
    Mfs_Uart_EnableFunc(uart, UartRx);
    /* Enable TX function of UART0 */
    Mfs_Uart_EnableFunc(uart, UartTx);
    
    /* Configure interrupt */    
    Mfs_Uart_EnableIrq(uart, UartRxIrq);
    Mfs_Uart_EnableIrq(uart, UartTxIdleIrq);
    
#if 0   //STM32的处理
	/* 初始化串口 
	   使能串口后，会发送空闲帧，延时等待10ms(待空闲帧发送完成)，这时再清TC、打开中断；
	   否则的话，中断使能后，会产生一次不期望的发送中断 */
	__HAL_UART_DISABLE_IT(pHUart, UART_IT_TC);	
	HAL_UART_Init(pHUart);/* 其中会使能串口 */
	MCU_DelayMS(10);
	MCU_UART_CLEAR_FLAG(pHUart, UART_FLAG_TC);
	__HAL_UART_ENABLE_IT(pHUart, UART_IT_TC);
	__HAL_UART_ENABLE_IT(pHUart, UART_IT_RXNE);
#endif
}

/*******************************************************************************
 函 数: MCU_UART_Set485Rx()
 功 能: 置485收发器为接收模式
 参 数: num--串口编号; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void MCU_UART_Set485Rx(U08 num)
{
	switch (num)
	{
        case MCU_UART0_NUM:
			GPIO_ClrOutputBit(IO_UART0_TR);
			break;
        case MCU_UART1_NUM: // 4G通信,直连TTL
            break;
		case MCU_UART4_NUM:
			GPIO_ClrOutputBit(IO_UART4_TR);			
			break;		
        case MCU_UART5_NUM:
			GPIO_ClrOutputBit(IO_UART5_TR);			
			break;
        case MCU_UART6_NUM:
			GPIO_ClrOutputBit(IO_UART6_TR);			
			break;
		default:
			break;
	}
}

/*******************************************************************************
 函 数: MCU_UART_Set485Tx()
 功 能: 置485收发器为发送模式
 参 数: num--串口编号; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void MCU_UART_Set485Tx(U08 num)
{
	switch (num)
	{
        case MCU_UART0_NUM:
			GPIO_SetOutputBit(IO_UART0_TR);
			break;
        case MCU_UART1_NUM: // 4G通信,直连TTL
            break;
		case MCU_UART4_NUM:
			GPIO_SetOutputBit(IO_UART4_TR);			
			break;		
        case MCU_UART5_NUM:
			GPIO_SetOutputBit(IO_UART5_TR);			
			break;
        case MCU_UART6_NUM:
			GPIO_SetOutputBit(IO_UART6_TR);			
			break;
		default:
			break;
	}
}

/*******************************************************************************
 函 数: MCU_UART_EnableRx()
 功 能: 使能接收容许
 参 数: num--串口编号; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void MCU_UART_EnableRx(U08 num)
{
    Mfs_Uart_EnableFunc(HUart[num].Instance, UartRx);   //打开接收使能
}

/*******************************************************************************
 函 数: MCU_UART_DisableRx()
 功 能: 关接收允许
 参 数: num--串口编号; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void MCU_UART_DisableRx(U08 num)
{
	Mfs_Uart_DisableFunc(HUart[num].Instance, UartRx);  //关闭接收使能
}

/*******************************************************************************
 函 数: MCU_UART_EnableTx()
 功 能: 使能发送容许
 参 数: num--串口编号; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void MCU_UART_EnableTx(U08 num)
{
    Mfs_Uart_EnableFunc(HUart[num].Instance, UartTx);   //打开接收使能
    Mfs_Uart_EnableIrq(HUart[num].Instance, UartTxIdleIrq);
}

/*******************************************************************************
 函 数: MCU_UART_DisableRx()
 功 能: 关发送允许
 参 数: num--串口编号; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void MCU_UART_DisableTx(U08 num)
{
    Mfs_Uart_DisableIrq(HUart[num].Instance, UartTxIdleIrq);
	Mfs_Uart_DisableFunc(HUart[num].Instance, UartTx);  //关闭接收使能
}

/*******************************************************************************
 函 数: MCU_UART_ClrRxIr()
 功 能: 清接收中断标志位
 参 数: num--串口编号; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void MCU_UART_ClrRxIr(U08 num)
{
    Mfs_Uart_ClrStatus(HUart[num].Instance, UartRxFull);
}

/*******************************************************************************
 函 数: MCU_UART_ClrTxIr()
 功 能: 清发送中断标志位
 参 数: num--串口编号; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void MCU_UART_ClrTxIr(U08 num)
{
    // TXI相当于STM32的TC中断,但无法清除,会一直中断,发送完必须关闭
	Mfs_Uart_ClrStatus(HUart[num].Instance, UartTxIdle);
}

/*******************************************************************************
 函 数: MCU_UART_Tx()
 功 能: 写发送寄存器
 参 数: num--串口编号; 
		val--写数据; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void MCU_UART_Tx(U08 num, U08 val)
{
	Mfs_Uart_SendData(HUart[num].Instance, val);
}

/*******************************************************************************
 函 数: MCU_UART_Rx()
 功 能: 读接收寄存器
 参 数: num--串口编号; 
 返 回: 接收数据
 其 它: 空
*******************************************************************************/
U08 MCU_UART_Rx(U08 num)
{
	return (U08)Mfs_Uart_ReceiveData(HUart[num].Instance);
}

/*******************************************************************************
 函 数: MCU_UART_IRQHandler()
 功 能: 串口中断处理函数
 参 数: num--串口编号; 
 返 回: 空
 其 它: 
*******************************************************************************/
void MCU_UART_IRQHandlerRx(U08 num)
{
    if (MCU_UART_GetRXfg(num))
    {
        COM_UartRxInt(num);
    }

    // 过载等会触发中断,必须清除
    if (MCU_UART_GetOREfg(num)
        || MCU_UART_GetFREfg(num)
        || MCU_UART_GetPEfg(num))
    {
        MCU_UART_ClrOREfg(num);
        MCU_UART_ClrFREfg(num);
        MCU_UART_ClrPEfg(num);
    }
}

void MCU_UART_IRQHandlerTx(U08 num)
{
    if (MCU_UART_GetTCfg(num))
    {
        COM_UartTxInt(num);
    }
}

/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
