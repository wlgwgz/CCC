/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MCU_UART_H
#define __MCU_UART_H

/* Includes ------------------------------------------------------------------*/

typedef struct {
    volatile stc_mfsn_uart_t* Instance;
}UART_HandleTypeDef;


// 接收标志
#define MCU_UART_GetRXfg(num)   Mfs_Uart_GetStatus(HUart[num].Instance, UartRxFull)
// 发送完成标志
#define MCU_UART_GetTCfg(num)   Mfs_Uart_GetStatus(HUart[num].Instance, UartTxIdle)
// 过载标志
#define MCU_UART_GetOREfg(num)  Mfs_Uart_GetStatus(HUart[num].Instance, UartOverrunError)
// 帧错误标志
#define MCU_UART_GetFREfg(num)  Mfs_Uart_GetStatus(HUart[num].Instance, UartFrameError)
// 校验错标志
#define MCU_UART_GetPEfg(num)   Mfs_Uart_GetStatus(HUart[num].Instance, UartParityError)


// 过载标志
#define MCU_UART_ClrOREfg(num)  Mfs_Uart_ClrStatus(HUart[num].Instance, UartOverrunError)
// 帧错误标志
#define MCU_UART_ClrFREfg(num)  Mfs_Uart_ClrStatus(HUart[num].Instance, UartFrameError)
// 校验错标志
#define MCU_UART_ClrPEfg(num)   Mfs_Uart_ClrStatus(HUart[num].Instance, UartParityError)











/* MCU串口在具体硬件上的编号 */
/*typedef*/ enum{
    MCU_UART0_NUM  = 0,
    MCU_UART1_NUM,
	MCU_UART4_NUM,
	MCU_UART5_NUM,
	MCU_UART6_NUM,
	MCU_UART_COUNT  /* 串口个数 */
} /*MCU_UART_NumTypeDef*/;


/* 串口通讯波特率 */
#define MCU_UART_4800 4800
#define MCU_UART_9600 9600
#define MCU_UART_19200 19200
#define MCU_UART_38400 38400
#define MCU_UART_57600 57600
#define MCU_UART_115200 115200


/* 串口通讯数据格式 */
typedef enum{
	MCU_UART_8_N_1,/* 无奇偶校验、1个停止位 */
	MCU_UART_8_N_2,/* 无奇偶校验、2个停止位 */
	MCU_UART_8_O_1,/* 奇校验、1个停止位 */
	MCU_UART_8_E_1,/* 偶校验、1个停止位 */
} MCU_UART_FormatTypeDef;

void MCU_UART_Init(void);
void MCU_UART_SetFormat(U08 num, U32 baudRate, MCU_UART_FormatTypeDef dataType);
void MCU_UART_Set485Rx(U08 num);
void MCU_UART_Set485Tx(U08 num);
void MCU_UART_EnableRx(U08 num);
void MCU_UART_DisableRx(U08 num);
void MCU_UART_ClrRxIr(U08 num);
void MCU_UART_ClrTxIr(U08 num);
void MCU_UART_Tx(U08 num, U08 val);
U08  MCU_UART_Rx(U08 num);
void MCU_UART_IRQHandlerRx(U08 num);
void MCU_UART_IRQHandlerTx(U08 num);
void MCU_UART_EnableTx(U08 num);
void MCU_UART_DisableTx(U08 num);

#endif 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
