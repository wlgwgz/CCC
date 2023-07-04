/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: hw_uart.c
 ����汾: V1.00
 �ļ�����: ���ڵײ�����,Ϊ�ϲ�����ṩӲ�������ӿ�,
 		   ���а���Ӳ������,�����ʹ��,�����������Եķ�װ����
 ������  : �
 ��������: 2015.01.06
 ================================ �޸ļ�¼ ==================================== 
 �޸���  : 
 ����汾: 
 �������: 
 �޸�����: 
 ******************************************************************************/
 
/* Includes ------------------------------------------------------------------*/
#include "includes.h"


static UART_HandleTypeDef HUart[MCU_UART_COUNT];


/*****************************************************************************
��  �� : MCU_UART_MspInit()
��  �� : �������ų�ʼ��
��  �� : U08 num : 
��  �� : ��
��  �� : ��
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


    //UART��ʼ������,Ĭ��9600 8N1
    MCU_UART_SetFormat(num, 9600, MCU_UART_8_N_1);
}




/*******************************************************************************
 �� ��: MCU_UART_Init()
 �� ��: ���ڵײ�������ʼ��
 �� ��: ��
 �� ��: ��
 �� ��: ��
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
 �� ��: MCU_UART_SetFormat()
 �� ��: ���ô���ͨѶ��ʽ
 �� ��: num--���ڱ��; 
		baudRate--������; 
		dataType--��������; 
 �� ��: ��
 �� ��: ��
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
            stcUartIrqCb.pfnTxIdleCb = Uart0TxIntCallback; // ������STM32��TC�ж�
            stcUartIrqCb.pfnRxIrqCb  = Uart0RxIntCallback;
            break;
        case MCU_UART1_NUM:
            stcUartIrqCb.pfnTxIdleCb = Uart1TxIntCallback; // ������STM32��TC�ж�
            stcUartIrqCb.pfnRxIrqCb  = Uart1RxIntCallback;
            break;    
        case MCU_UART4_NUM:
            stcUartIrqCb.pfnTxIdleCb = Uart4TxIntCallback; // ������STM32��TC�ж�
            stcUartIrqCb.pfnRxIrqCb  = Uart4RxIntCallback;
            break;
        case MCU_UART5_NUM:
            stcUartIrqCb.pfnTxIdleCb = Uart5TxIntCallback; // ������STM32��TC�ж�
            stcUartIrqCb.pfnRxIrqCb  = Uart5RxIntCallback;
            break;
        case MCU_UART6_NUM:
            stcUartIrqCb.pfnTxIdleCb = Uart6TxIntCallback; // ������STM32��TC�ж�
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
    
	/* ������ */
	stcUartConfig.u32BaudRate = baudRate;
	
	/* ���ݸ�ʽ */
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
    
#if 0   //STM32�Ĵ���
	/* ��ʼ������ 
	   ʹ�ܴ��ں󣬻ᷢ�Ϳ���֡����ʱ�ȴ�10ms(������֡�������)����ʱ����TC�����жϣ�
	   ����Ļ����ж�ʹ�ܺ󣬻����һ�β������ķ����ж� */
	__HAL_UART_DISABLE_IT(pHUart, UART_IT_TC);	
	HAL_UART_Init(pHUart);/* ���л�ʹ�ܴ��� */
	MCU_DelayMS(10);
	MCU_UART_CLEAR_FLAG(pHUart, UART_FLAG_TC);
	__HAL_UART_ENABLE_IT(pHUart, UART_IT_TC);
	__HAL_UART_ENABLE_IT(pHUart, UART_IT_RXNE);
#endif
}

/*******************************************************************************
 �� ��: MCU_UART_Set485Rx()
 �� ��: ��485�շ���Ϊ����ģʽ
 �� ��: num--���ڱ��; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void MCU_UART_Set485Rx(U08 num)
{
	switch (num)
	{
        case MCU_UART0_NUM:
			GPIO_ClrOutputBit(IO_UART0_TR);
			break;
        case MCU_UART1_NUM: // 4Gͨ��,ֱ��TTL
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
 �� ��: MCU_UART_Set485Tx()
 �� ��: ��485�շ���Ϊ����ģʽ
 �� ��: num--���ڱ��; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void MCU_UART_Set485Tx(U08 num)
{
	switch (num)
	{
        case MCU_UART0_NUM:
			GPIO_SetOutputBit(IO_UART0_TR);
			break;
        case MCU_UART1_NUM: // 4Gͨ��,ֱ��TTL
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
 �� ��: MCU_UART_EnableRx()
 �� ��: ʹ�ܽ�������
 �� ��: num--���ڱ��; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void MCU_UART_EnableRx(U08 num)
{
    Mfs_Uart_EnableFunc(HUart[num].Instance, UartRx);   //�򿪽���ʹ��
}

/*******************************************************************************
 �� ��: MCU_UART_DisableRx()
 �� ��: �ؽ�������
 �� ��: num--���ڱ��; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void MCU_UART_DisableRx(U08 num)
{
	Mfs_Uart_DisableFunc(HUart[num].Instance, UartRx);  //�رս���ʹ��
}

/*******************************************************************************
 �� ��: MCU_UART_EnableTx()
 �� ��: ʹ�ܷ�������
 �� ��: num--���ڱ��; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void MCU_UART_EnableTx(U08 num)
{
    Mfs_Uart_EnableFunc(HUart[num].Instance, UartTx);   //�򿪽���ʹ��
    Mfs_Uart_EnableIrq(HUart[num].Instance, UartTxIdleIrq);
}

/*******************************************************************************
 �� ��: MCU_UART_DisableRx()
 �� ��: �ط�������
 �� ��: num--���ڱ��; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void MCU_UART_DisableTx(U08 num)
{
    Mfs_Uart_DisableIrq(HUart[num].Instance, UartTxIdleIrq);
	Mfs_Uart_DisableFunc(HUart[num].Instance, UartTx);  //�رս���ʹ��
}

/*******************************************************************************
 �� ��: MCU_UART_ClrRxIr()
 �� ��: ������жϱ�־λ
 �� ��: num--���ڱ��; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void MCU_UART_ClrRxIr(U08 num)
{
    Mfs_Uart_ClrStatus(HUart[num].Instance, UartRxFull);
}

/*******************************************************************************
 �� ��: MCU_UART_ClrTxIr()
 �� ��: �巢���жϱ�־λ
 �� ��: num--���ڱ��; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void MCU_UART_ClrTxIr(U08 num)
{
    // TXI�൱��STM32��TC�ж�,���޷����,��һֱ�ж�,���������ر�
	Mfs_Uart_ClrStatus(HUart[num].Instance, UartTxIdle);
}

/*******************************************************************************
 �� ��: MCU_UART_Tx()
 �� ��: д���ͼĴ���
 �� ��: num--���ڱ��; 
		val--д����; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void MCU_UART_Tx(U08 num, U08 val)
{
	Mfs_Uart_SendData(HUart[num].Instance, val);
}

/*******************************************************************************
 �� ��: MCU_UART_Rx()
 �� ��: �����ռĴ���
 �� ��: num--���ڱ��; 
 �� ��: ��������
 �� ��: ��
*******************************************************************************/
U08 MCU_UART_Rx(U08 num)
{
	return (U08)Mfs_Uart_ReceiveData(HUart[num].Instance);
}

/*******************************************************************************
 �� ��: MCU_UART_IRQHandler()
 �� ��: �����жϴ�����
 �� ��: num--���ڱ��; 
 �� ��: ��
 �� ��: 
*******************************************************************************/
void MCU_UART_IRQHandlerRx(U08 num)
{
    if (MCU_UART_GetRXfg(num))
    {
        COM_UartRxInt(num);
    }

    // ���صȻᴥ���ж�,�������
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
