/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: uart.c
 文件描述: 
 创建人  : 
 创建日期:
*******************************************************************************/
#define UART_GLOBAL
#include "Includes.h"
U08 debug1[20];

#define SET_UART0_485_RECE()	GPIO_ClrOutputBit(UART0_485_TR)
#define SET_UART0_485_TRAS()	GPIO_SetOutputBit(UART0_485_TR)
#define SET_UART1_485_RECE()	GPIO_ClrOutputBit(UART1_485_TR)
#define SET_UART1_485_TRAS()	GPIO_SetOutputBit(UART1_485_TR)
#define SET_UART2_485_RECE()	GPIO_ClrOutputBit(UART2_485_TR)
#define SET_UART2_485_TRAS()	GPIO_SetOutputBit(UART2_485_TR)
#define SET_UART3_485_RECE()	GPIO_ClrOutputBit(UART3_485_TR)
#define SET_UART3_485_TRAS()	GPIO_SetOutputBit(UART3_485_TR)
#define SET_UART4_485_RECE()	//GPIO_ClrOutputBit(UART4_485_TR)
#define SET_UART4_485_TRAS()	//GPIO_SetOutputBit(UART4_485_TR)

#define ENABLE_UART0_RECE()		MCU_UART_EnableRx(MCU_UART0_NUM)		/* 使能接收允许 */
#define DISABLE_UART0_RECE()	MCU_UART_DisableRx(MCU_UART0_NUM)		/* 关接收允许 */
#define ENABLE_UART0_TRAS()	/* 使能发送容许，确认底层通讯配置中是否已打开 */
#define DISABLE_UART0_TRAS()/* 关发送容许 */
#define ENABLE_UART4_RECE()		MCU_UART_EnableRx(MCU_UART0_NUM)		/* 使能接收允许 */
#define DISABLE_UART4_RECE()	(U2C1_CCR &= ~0x4000)		/* 关接收允许 */
#define ENABLE_UART4_TRAS()	/* 使能发送容许 */
#define DISABLE_UART4_TRAS()/* 关发送容许 */
#define ENABLE_UART5_RECE()		MCU_UART_EnableRx(MCU_UART0_NUM)		/* 使能接收允许 */
#define DISABLE_UART5_RECE()	(U2C1_CCR &= ~0x4000)		/* 关接收允许 */
#define ENABLE_UART5_TRAS()	/* 使能发送容许 */
#define DISABLE_UART5_TRAS()/* 关发送容许 */
#define ENABLE_UART6_RECE()		MCU_UART_EnableRx(MCU_UART0_NUM)		/* 使能接收允许 */
#define DISABLE_UART6_RECE()	(U2C1_CCR &= ~0x4000)		/* 关接收允许 */
#define ENABLE_UART6_TRAS()	/* 使能发送容许 */
#define DISABLE_UART6_TRAS()/* 关发送容许 */

#define CLR_UART0_RX_IR_BIT()	(U0C0_PSCR |= 0x4000)		/* 清接收中断标志位 */
#define CLR_UART0_TX_IR_BIT()	(U0C0_PSCR |= 0x2000)		/* 清发送中断标志位 */
#define CLR_UART1_RX_IR_BIT()	(U0C1_PSCR |= 0x4000)		/* 清接收中断标志位 */
#define CLR_UART1_TX_IR_BIT()	(U0C1_PSCR |= 0x2000)		/* 清发送中断标志位 */
#define CLR_UART2_RX_IR_BIT()	(U1C0_PSCR |= 0x4000)		/* 清接收中断标志位 */
#define CLR_UART2_TX_IR_BIT()	(U1C0_PSCR |= 0x2000)		/* 清发送中断标志位 */
#define CLR_UART3_RX_IR_BIT()	(U2C0_PSCR |= 0x4000)		/* 清接收中断标志位 */
#define CLR_UART3_TX_IR_BIT()	(U2C0_PSCR |= 0x2000)		/* 清发送中断标志位 */
#define CLR_UART4_RX_IR_BIT()	(U2C1_PSCR |= 0x4000)		/* 清接收中断标志位 */
#define CLR_UART4_TX_IR_BIT()	(U2C1_PSCR |= 0x2000)		/* 清发送中断标志位 */


#define UART0_TRAS(val)			(U0C0_TBUF00 = val)			/* 写发送缓冲寄存器 */
#define UART0_RECE()			((U08)U0C0_RBUF)			/* 接收缓冲寄存器 */
#define UART1_TRAS(val)			(U0C1_TBUF00 = val)			/* 写发送缓冲寄存器 */
#define UART1_RECE()			((U08)U0C1_RBUF)			/* 接收缓冲寄存器 */
#define UART2_TRAS(val)			(U1C0_TBUF00 = val)			/* 写发送缓冲寄存器 */
#define UART2_RECE()			((U08)U1C0_RBUF)			/* 接收缓冲寄存器 */
#define UART3_TRAS(val)			(U2C0_TBUF00 = val)			/* 写发送缓冲寄存器 */
#define UART3_RECE()			((U08)U2C0_RBUF)			/* 接收缓冲寄存器 */
#define UART4_TRAS(val)			(U2C1_TBUF00 = val)			/* 写发送缓冲寄存器 */
#define UART4_RECE()			((U08)U2C1_RBUF)			/* 接收缓冲寄存器 */


static SENDQUEUE Queue[MAX_UART];	/* 通讯队列，主机使用队列发送，从机不使用 */
/*******************************************************************************
 函 数: COM_DisableRece()
 功 能: 关接收允许
 参 数: u--串口号; 
 返 回: 空
 其 它: 不确定具体串口时调用，如DealUartMaster()中调用
*******************************************************************************/
void COM_DisableRece(U08 u)
{
	MCU_UART_DisableRx(u);
}

/*******************************************************************************
 函 数: COM_ReadyRece()
 功 能: 串口接收准备
 参 数: u--串口号; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void COM_ReadyRece(U08 u)
{
	volatile U08 receData;
	
	MCU_UART_ClrRxIr(u);
	receData = MCU_UART_Rx(u);/* 虚读串口数据 */
	MCU_UART_EnableRx(u);
	MCU_UART_Set485Rx(u);
	}


/*******************************************************************************
 函 数: COM_Set485Tras()
 功 能: 设置485为发送状态
 参 数: u--串口号; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void COM_Set485Tras(U08 u)
{
	MCU_UART_Set485Tx(u);
}

/*******************************************************************************
 函 数: COM_UartRxInt()
 功 能: 串口接收中断
 参 数: u--串口号
 返 回: 空
 其 它: 空
*******************************************************************************/
void COM_UartRxInt(U08 u)
{
	UART *pCOM;
    // 需要通过读取清除,因此先读
    U08 recv_data = MCU_UART_Rx(u);
	
	MCU_UART_ClrRxIr(u);
	
	pCOM = &Uart[u];

  	if (pCOM->rxCnt < COMMU_RX_SIZE) 
	{
		pCOM->time.rxFrame = COMMU_TIME_RX_FRAME;
		pCOM->rxBuf[pCOM->rxCnt++] = recv_data;
	}

}

/*******************************************************************************
 函 数: COM_UartTxInt()
 功 能: 串口发送中断
 参 数: u--串口号
 返 回: 空
 其 它: 空
*******************************************************************************/
void COM_UartTxInt(U08 u)
{
	UART *pCOM;
	
	MCU_UART_ClrTxIr(u);
	
	pCOM = &Uart[u];

    if (pCOM->txCnt < pCOM->txTotal)
    {
    	pCOM->txCnt++;	
    }
    
	if (pCOM->txCnt < pCOM->txTotal)
	{
		MCU_UART_Tx(u, pCOM->txBuf[pCOM->txCnt]);
	}
    else 
	{
        MCU_UART_DisableTx(u);
		COM_ReadyRece(u);
        //刷新发送超时，以免低波特率下，时间不够
        pCOM->time.txDly = COMMU_TIME_TX_TIMEOUT;
		pCOM->rxCnt = 0;
	}
    
	

}
/*******************************************************************************
 函 数: COM_TrasDly()
 功 能: 串口发送延时阶段处理
 参 数: u--串口号; 
 返 回: 空
 其 它: 关接收容许、设置485为发送状态
*******************************************************************************/
void COM_TrasDly(U08 u)
{
    COM_DisableRece(u);
    COM_Set485Tras(u);
#if 0
    
	switch (u)
	{
		case COM_UART0 : 
			DISABLE_UART0_RECE();
			SET_UART0_485_TRAS();
			break;
		case COM_UART1 : 
			DISABLE_UART1_RECE();
			SET_UART1_485_TRAS();
			break;
		case COM_UART2 : 
			DISABLE_UART2_RECE();
			SET_UART2_485_TRAS(); 	
			break;
		case COM_UART3 : 
			DISABLE_UART3_RECE();
			SET_UART3_485_TRAS();
			break;
		case COM_UART4 : 
			DISABLE_UART4_RECE();
			SET_UART4_485_TRAS(); 	
			break;
		default :
			break;
	}
#endif
}

/*******************************************************************************
 函 数: COM_WrTrasBufReg()
 功 能: 写串口发送缓冲区寄存器(启动发送)
 参 数: u--串口号;
 		value--写入的值;
 返 回: 空
 其 它: 空
*******************************************************************************/
void COM_WrTrasBufReg(U08 u, U08 value)
{
	MCU_UART_Tx(u, value);
    MCU_UART_EnableTx(u);    
}

/*******************************************************************************
	以下为数据链路层中的应用接口函数处理
*******************************************************************************/

/*******************************************************************************
 函 数: COM_GetErr()
 功 能: 获取通讯故障类型
 参 数: u--串口号; 
 返 回: 通讯故障具体类型
 COMMU_NO_ERR	无通讯故障
 COMMU_OVERTIME	[主机判断]通讯超时故障:未收到数据帧
 COMMU_INTERFERE[主机判断]通讯干扰故障:数据帧长度短(干扰<4或小于理论个数)
 COMMU_DATA_ERR	[主机判断]通讯数据故障:错误数据帧，可能是地址错、数据错、校验和错误 
 COMMU_EXCEP_RSP[主机判断]从机回复异常响应
 COMMU_INT_PROT	[从机判断]通讯中断保护:超过中断保护时间仍未收到有效数据帧 
 其 它: 1、报COMMU_EXCEP_RSP时，队列会指向下个节点继续通讯
 		2、报COMMU_INT_PROT时，不影响从机的通讯状态，且当收到数据帧时会自动复位;
 		应用层可根据实际情况来处理保护措施或不处理
*******************************************************************************/
U08 COM_GetErr(U08 u)	
{	
	return Uart[u].commuErr;
}

/*******************************************************************************
 函 数: COM_GetMbExceptCode()
 功 能: 获取异常响应时的Modbus异常码
 参 数: u--串口号; 
 返 回: Modbus异常码
	 MB_NO_ERR					0		无异常
	 MB_ILLEGAL_FUNCTION       	1		非法功能 
	 MB_ILLEGAL_DATA_ADDR       2		非法数据地址 
	 MB_ILLEGAL_DATA_VALUE      3		非法数据值 
	 MB_SLAVE_DEVICE_FALUE      4		从机设备故障，从机无法处理 
 其 它: 空
*******************************************************************************/
U08 COM_GetMbExceptCode(U08 u)	
{	
	return Uart[u].exceptCode;
}

/*******************************************************************************
 函 数: COM_ResetAlarm()
 功 能: 复位通讯报警
 参 数: u--串口号; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void COM_ResetAlarm(U08 u)
{
	Uart[u].step = COMMU_IDLE;
	Uart[u].commuErr = COMMU_NO_ERR;
	Uart[u].exceptCode = MB_NO_ERR;
	COM_QInit(u);
}

/*******************************************************************************
 函 数: COM_Idle()
 功 能: 判断通讯状态是否空闲
 参 数: u--串口号; 
 返 回: TRUE/FALSE--通讯空闲/不空闲
 其 它: 空
*******************************************************************************/
BOOL COM_Idle(U08 u)
{
	return (Uart[u].step == COMMU_IDLE);
}

/*******************************************************************************
 函 数: COM_GetLedTime()
 功 能: 获取通讯灯闪烁计时
 参 数: u--串口号; 
 返 回: 通讯灯闪烁计时
 其 它: 调用时的处理，计时时间不为0时关闭指示灯，为0时打开；
 		即通讯正常时闪烁显示；失败时常亮。
*******************************************************************************/
U08 COM_GetLedTime(U08 u)
{
	return Uart[u].time.led;
}

/*******************************************************************************
 函 数: COM_2MsTimer()
 功 能: 通讯计时(2ms调用一次)
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void COM_2MsTimer(U08 u)
{
//    U08 i;
    UART *pCOM;
    
//    for (i=0; i<MAX_UART; i++)
//    {
        pCOM = &Uart[u];
        if (pCOM->time.interval != 0)       pCOM->time.interval--;
//        if (pCOM->time.rxOvertime != 0)     pCOM->time.rxOvertime--;
//        if (pCOM->time.txDly != 0)          pCOM->time.txDly--;
//        if (pCOM->time.rxFrame != 0)        pCOM->time.rxFrame--;
        if (pCOM->time.broadcastDly != 0)   pCOM->time.broadcastDly--;
        if (pCOM->time.led != 0)            pCOM->time.led--;
//    }
}

/*******************************************************************************
 函 数: COM_VariableTimer()
 功 能: 可变计时(2ms/5ms调用一次)
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void COM_VariableTimer(U08 u)
{
//    U08 i;
    UART *pCOM;
    
//    for (i=0; i<MAX_UART; i++)
//    {
        pCOM = &Uart[u];
//        if (pCOM->time.interval != 0)       pCOM->time.interval--;
        if (pCOM->time.rxOvertime != 0)     pCOM->time.rxOvertime--;
        if (pCOM->time.txDly != 0)          pCOM->time.txDly--;
        if (pCOM->time.rxFrame != 0)        pCOM->time.rxFrame--;
//        if (pCOM->time.broadcastDly != 0)   pCOM->time.broadcastDly--;
//        if (pCOM->time.led != 0)            pCOM->time.led--;
//    }
}

/*******************************************************************************
 函 数: COM_ProtectTimer()
 功 能: 从机通讯保护时间计时，秒单位
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void COM_ProtectTimer(void)
{
	U08 i;
	UART *pCOM;
	
	for (i=0; i<MAX_UART; i++)
	{
		pCOM = &Uart[i];
		if (pCOM->time.commIntProt != 0)	 
		{
			pCOM->time.commIntProt--;
		}	
	}
}

/*******************************************************************************
 函 数: COM_ClrRecord()
 功 能: 清除通讯记录信息
 参 数: 空; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void COM_ClrRecord(void)
{
	U08 i;

	for (i=0; i<MAX_UART; i++)
	{
		MEMCLR(Uart[i].pRecord, sizeof(COMRECORD));
	}
}
/*******************************************************************************
 函 数: COM_err_deal()
 功 能: 通讯故障处理
 参 数: u--串口号; 
		addr--地址; 
		op--选项：清除、记录; 
 返 回: 空
 其 它: 注意通讯号共用时的处理，避免冲突
*******************************************************************************/
BOOL COM_err_deal(U08 u, U08 addr, U08 op)
{
    BOOL fg_err = FALSE;
    
	switch (op)
	{
	    case COM_ERR_CLR:
	        switch (u)
			{
			    case COM_FCU_FBUS:
                    if (addr < MAX_PCB
                    ||  addr == ADDR_BRO_FBUS)
                    {
                        if (pcb_type == TYPE_MASTER)		CLRBIT(slave_commu_error, (addr&0x0f));
					    else	                            self_commu_err = FALSE;		
                    }
                    else if (addr>=FCU_ADDR_BASE && addr<FCU_ADDR_BASE+FCU_NUM_USE)
                    {
                        CLR_FCU_BIT(fcu_commu_err, (addr-FCU_ADDR_BASE));
                    }
                    	        
			        break;
			    case COM_INVT:
			        if (addr >= EXIO_KLV_ADDR 
                    && addr < EXIO_KLV_ADDR+EXIO_KLV_MAX)
                    {
                        U08 exio_id = addr - EXIO_KLV_ADDR;
                        CLR_EXIO_COMMU_ERR(exio_id);
//                        if (GET_INVT_CONNECT)
//                        {
//                            SET_INOUT_CONNECT;
//                        }
                    }
                    else if (USE_EX_EC_FAN 
                    &&  addr >= val_ec_fan_base_addr 
                    &&  addr < val_ec_fan_base_addr + val_freq_fan_num
                    )
                    {
                        U08 ec = addr - val_ec_fan_base_addr;
                        CLR_BIT(EcFan_commu_err, ec);
                    }
                    else
                    {
                        INVT_ID_ENUM id;

                        for (id=INVT_ID_1; id<INVT_ID_MAX; id++)
                        {
                            if (addr == invt_get_device_addr(id))   
                            {
                                CLR_BIT(fg_invt_commu_error, id);
//                                if (id == INVT_ID_1)
//                                {
//                                    fg_baudrate_confirm[u] = TRUE;
//                                    SET_INVT_CONNECT;
//                                }
                            }
                        }
                    }
			        break;
				case COM_TEXT_HMI:
					CLR_BIT(hmi_commu_error, addr);		
					break;
			}
	        break;
	    case COM_ERR_REC:
	        switch (u)
			{
			    case COM_FCU_FBUS:
                    if (addr < MAX_PCB
                    ||  addr == ADDR_BRO_FBUS)
                    {
                        if (pcb_type == TYPE_MASTER)		SET_BIT(slave_commu_error, (addr&0x0f));
					    else                                self_commu_err = TRUE;	
                    }
                    else if (addr>=FCU_ADDR_BASE && addr<FCU_ADDR_BASE+FCU_NUM_USE)
                    {
                        SET_FCU_BIT(fcu_commu_err, (addr-FCU_ADDR_BASE));
                    }
						        
			        break;
			    case COM_INVT:
			        if (addr >= EXIO_KLV_ADDR 
                    && addr < EXIO_KLV_ADDR+EXIO_KLV_MAX)
                    {
//                        if (GET_INOUT_CONNECT)
//                        {
                            U08 exio_id = addr - EXIO_KLV_ADDR;
                            SET_EXIO_COMMU_ERR(exio_id);
//                            CLR_INOUT_CONNECT;
//                        }
                    }
                    else if (USE_EX_EC_FAN 
                    &&  addr >= val_ec_fan_base_addr 
                    &&  addr < val_ec_fan_base_addr + val_freq_fan_num
                    )
                    {
                        U08 ec = addr - val_ec_fan_base_addr;
                        SET_BIT(EcFan_commu_err, ec);
                    }
                    else
                    {
                        INVT_ID_ENUM id;

                        for (id=INVT_ID_1; id<INVT_ID_MAX; id++)
                        {
                            if (addr == invt_get_device_addr(id))   
                            {
                                SET_BIT(fg_invt_commu_error, id);
//                                if (!fg_baudrate_confirm[u])  /* 波特率未确认，主机在此处切换波特率 */		
//                                {
//                                    if (id == INVT_ID_1)			/* 仅做变频压机的波特率自适应 */
//                                    {
//                                        com_baudrate[u] = (com_baudrate[u] == B_9600) ? B_2400 : B_9600;
//                                        COM_Config(u, com_baudrate[u], T_8N1);
//                                    }
//                                }
//                                else		/* 波特率已确认 */
//                                {
//                                    U08 exio_id;
//                                    for (exio_id = 0; exio_id < USE_EXIO; exio_id++)
//                                    {
//                                        if (GET_EXIO_COMMU_ERR(exio_id))
//                                        {
//                                            /* 波特率需重新确认 */
//                                            fg_baudrate_confirm[u] = FALSE;
//                                            CLR_INOUT_CONNECT;
//                                        }
//                                    }
//                                    CLR_INVT_CONNECT;
//                                }
                            }
                        }
                    }
			        break;
                    
				case COM_TEXT_HMI:
					SET_BIT(hmi_commu_error, addr);		
					break;
			}
	        break;
    case COM_ERR_GET:
	        switch (u)
			{              
			    case COM_FCU_FBUS:
                    if (addr < MAX_PCB
                    ||  addr == ADDR_BRO_FBUS)
                    {
                        if (pcb_type == TYPE_MASTER && GET_BIT(slave_commu_error, (addr&0x0f)))
                        {
                            fg_err = TRUE;
                        }
                    }
                    else if (addr>=FCU_ADDR_BASE && addr<FCU_ADDR_BASE+FCU_NUM_USE)
                    {
                        if (GET_FCU_BIT(fcu_commu_err, (addr-FCU_ADDR_BASE)))
                        {
                            fg_err = TRUE;
                        }
                    }
			        break;                 
			    case COM_INVT:
                    if (addr >= EXIO_KLV_ADDR 
                    && addr < EXIO_KLV_ADDR+EXIO_KLV_MAX)
                    {
                        U08 exio_id = addr - EXIO_KLV_ADDR;
                        if (GET_EXIO_COMMU_ERR(exio_id))
                        {
                            fg_err = TRUE;
                        }
                    }
                    else if (USE_EX_EC_FAN 
                    &&  addr >= val_ec_fan_base_addr 
                    &&  addr < val_ec_fan_base_addr + val_freq_fan_num
                    )
                    {
                        U08 ec = addr - val_ec_fan_base_addr;
                        if (GET_BIT(EcFan_commu_err, ec))
                        {
                            fg_err = TRUE;
                        }
                    }
                    else
                    {
                        INVT_ID_ENUM id;

                        for (id=INVT_ID_1; id<INVT_ID_MAX; id++)
                        {
                            if (addr == invt_get_device_addr(id))   
                            {
                                if (GET_BIT(fg_invt_commu_error, id))
                                {
                                    fg_err = TRUE;
                                }
                            }
                        }
                    }
			        break;
				case COM_TEXT_HMI:
					if (GET_BIT(hmi_commu_error, addr))
					{
					    fg_err = TRUE;
					}
					break;

                default:
                    break;
			}
	        break;
	    default:
	        break;
	}	

    return fg_err;
}

/*******************************************************************************
 函 数: COM_InitSlaver()
 功 能: 初始化指定串口号为从机
 参 数: u--串口号; 
 		pRcd--通讯信息记录结构体指针
 		addr--从机地址
 返 回: 空
 其 它: 初始化区别 
		主机: 关接收容许、485发送、COMMU_IDLE
		从机: 开接收容许、485接收、COMMU_RECE、通讯保护时间
*******************************************************************************/
void COM_InitSlaver(U08 u, COMRECORD *pRcd, U08 addr)
{
	UART *pCOM;
	
	/* 使能发送容许、开接收容许、485接收 */
	COM_ReadyRece(u);
	
	pCOM = &Uart[u];
	
	/* 先全部初始化为0, 再初始化其它特殊变量 */
	MEMCLR(pCOM, sizeof(UART));
	pCOM->ownAddr = addr;
	pCOM->step = COMMU_RX;
	pCOM->pRecord = pRcd;
	pCOM->commuErr = COMMU_NO_ERR;
	if (u == COM_4G)
	{
		pCOM->time.commIntProt = app_4g_commIntProt_time(FALSE);
	}
	else
	{
		pCOM->time.commIntProt = COMMU_TIME_PROTECT+5;			/* 第一次初始化可适当加长 */
	}
}

/*******************************************************************************
 函 数: COM_InitMaster()
 功 能: 初始化指定串口号为主机
 参 数: u--串口号; 
 		pRcd--通讯信息记录结构体指针
 返 回: 空
 其 它: 空 
*******************************************************************************/
void COM_InitMaster(U08 u, COMRECORD *pRcd)
{
	UART *pCOM;
	
	/* 使能发送容许、关接收容许、485发送 */
	COM_ReadyRece(u);
	
	pCOM = &Uart[u];
	
	/* 先全部初始化为0, 再初始化其它特殊变量 */
	MEMCLR(pCOM, sizeof(UART));
	pCOM->ownAddr = 0;
	pCOM->step = COMMU_IDLE;
	pCOM->pRecord = pRcd;
	pCOM->commuErr = COMMU_NO_ERR;
	
	/* 初始化发送队列 */
	COM_QInit(u);				
}

/*******************************************************************************
 函 数: COM_config_and_init()
 功 能: 初始化通讯
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void COM_config_and_init(U08 u)
{
    switch (u)
    {
#ifdef COM_4G
		case COM_4G:
            com_baudrate[u] = val_bms_baud_rate;
//            fg_baudrate_confirm[u] = TRUE;
            COM_Config(u, com_baudrate[u], val_bms_format);
            COM_InitSlaver(u, BKBD_INVT_UART4, val_bms_addr);	
            break;
#endif 

        case COM_BMS:/* BMS监控 */
#ifdef USE_KIT_PARA_FAC
            com_baudrate[u] = B_57600;
//            fg_baudrate_confirm[u] = TRUE;
            COM_Config(u, B_57600, T_8N2);
            COM_InitSlaver(u, BKBD_INVT_UART0, 1);
#else
            com_baudrate[u] = val_bms_baud_rate;
//            fg_baudrate_confirm[u] = TRUE;
            COM_Config(u, com_baudrate[u], val_bms_format);
            COM_InitSlaver(u, BKBD_INVT_UART0, val_bms_addr);	
#endif
            break;
            
        case COM_TEXT_HMI:/* 显示屏 */
            com_baudrate[u] = B_19200;
//            fg_baudrate_confirm[u] = TRUE;
            COM_Config(u, com_baudrate[u], T_8N1);
            COM_InitMaster(u, BKBD_INVT_UART3);	        
            break;
            
        case COM_INVT:/* 变频 */
            com_baudrate[u] = B_9600;
//            fg_baudrate_confirm[u] = FALSE;
            COM_Config(u, com_baudrate[u], T_8N1);
			COM_InitMaster(u, BKBD_INVT_UART2);
//            if (!USE_INVT)
//            {
//            	fg_baudrate_confirm[u] = TRUE;
//				SET_INVT_CONNECT;
//				SET_INOUT_CONNECT;
//            }
          
            break;
            
        case COM_FCU_FBUS:
            com_baudrate[u] = B_9600;
//            fg_baudrate_confirm[u] = TRUE;
            COM_Config(u, com_baudrate[u], T_8N1);           /* N0004 */
            if (pcb_type == TYPE_MASTER)                            
        	{
        		COM_InitMaster(u, BKBD_INVT_UART1); 
        	}
        	else
        	{
        		COM_InitSlaver(u, BKBD_INVT_UART1, FBUS_ADDR);
        	}
	           
            break;
    }
}

/*******************************************************************************
 函 数: COM_Init()
 功 能: 初始化通讯
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void COM_Init(void)
{
    COM_config_and_init(COM_BMS);
#ifdef COM_4G
    COM_config_and_init(COM_4G);
#endif 
    COM_config_and_init(COM_TEXT_HMI);
    COM_config_and_init(COM_INVT);
    COM_config_and_init(COM_FCU_FBUS);
}

/*******************************************************************************
 函 数: COM_SlaverProg()
 功 能: 串口从机处理接口
 参 数: u--串口号; 
 返 回: 空
 其 它: 注意上电后从机初始化; New485Comm
	(1) 接收：上电后，从机处于"接收"状态，打开接收使能，485芯片置为接收状态。 收
 到一帧数据后，需要对数据进行相应判断，具体与主机接收判断相同。 接收到正确数据后，
 "通讯中断保护时间"重新开始计时，若是单播命令，则要回复数据，通讯状态转为"延时",
 若是广播命令，则不能回复数据；若接收到错误帧，则需要重新接收下一帧数据。
 	通讯中断保护时间到，仍未收到有效数据帧，报通讯中断保护故障。
 	(2) 延时：发送前，需关闭接收使能，防止意外收到干扰数据。将485 芯片置为发送状
 态，消除总线上的干扰。发送延时时间到后进入"启动发送"状态。	
 	(3) 启动发送：将发送缓冲区第一个数据存入发送寄存器，启动发送。将通讯状态置为
 "发送"，在"发送"状态下等待发送完成。
 	(4) 发送：发送中断将所有数据发送完成后，将485 芯片置为接收状态，将通讯状态置
 为"接收"。
*******************************************************************************/
void COM_SlaverProg(U08 u)
{
	UART *pCOM;

	pCOM = &Uart[u];
    
	switch (pCOM->step)
	{
	  	case COMMU_RX :	/* 收到正确帧且非广播时，转为发送延时，否则一直接收 */
			/* 一帧接收完成:接收帧时间内未收到下一数据或接收缓冲区满 */
			if ((pCOM->rxCnt>0 && pCOM->time.rxFrame==0)
				|| (pCOM->rxCnt >= COMMU_RX_SIZE)) 
	  		{	 
	  			COM_DisableRece(u);			/* 处理接收帧时关闭接收使能 */
	  			if (MB_SlaverRxTx(u, pCOM)) 
	  			{
	  				pCOM->time.led = COMMU_TIME_LED;
	  				if (u == COM_4G)
					{
						pCOM->time.commIntProt = app_4g_commIntProt_time(TRUE);
					}
					else
					{
	  					pCOM->time.commIntProt = COMMU_TIME_PROTECT;
					}
	  				pCOM->commuErr = COMMU_NO_ERR;			/* 自动复位 */
	  				
	  				if (pCOM->rxBuf[0]!=ADDR_BROADCAST && pCOM->rxBuf[0]!=ADDR_BRO_FBUS)	/* 不是广播, 才回送数据 */
				    {
						pCOM->step = COMMU_DELAY;
						pCOM->time.txDly = COMMU_TIME_TX_DLY;
				    } 
				    else	/* 广播时，准备接收下一帧 */
				    {			    	
				    	COM_ReadyRece(u);
				    }			    
				}
				else		/* 错误帧时，准备接收下一帧 */
				{				
					COM_ReadyRece(u);
				}
				
				pCOM->rxCnt = 0;			/* 处理一帧数据后，清接收计数器 */
	  		}

	  		/* 通讯中断保护时间到，仍未收到有效数据帧，报通讯中断保护故障 */
	  		if (pCOM->time.commIntProt == 0)
	  		{
	  			pCOM->commuErr = COMMU_INT_PROT;
                pCOM->step = COMMU_FAIL;
                if (u == COM_4G)
				{
					app_4g_commIntProt_deal();	// 4G通信中断保护时间到，重启4G模组
					pCOM->time.commIntProt = app_4g_commIntProt_time(FALSE);				
				}
	  		}
	  		break;
	  	case COMMU_DELAY :
			COM_TrasDly(u);
            
			if (pCOM->time.txDly == 0)
			{
				pCOM->step = COMMU_START;
			}
			else
			{
				break;
			}
			break;
		case COMMU_START :	
			pCOM->txCnt = 0;
			pCOM->rxCnt = 0;
			COM_WrTrasBufReg(u, pCOM->txBuf[0]);
			pCOM->step = COMMU_TX;
			COM_err_deal(u, pCOM->txBuf[0], COM_ERR_CLR);   /* 清除通讯故障 */
            pCOM->time.txDly = COMMU_TIME_TX_TIMEOUT;
            
			break;			
	  	case COMMU_TX :	/* 转接收由中断改为这里 */
            if (pCOM->txCnt >= pCOM->txTotal)
            {   
                if (u == COM_4G)
				{
					pCOM->time.commIntProt = app_4g_commIntProt_time(TRUE);
				}
				else
				{
                	pCOM->time.commIntProt = COMMU_TIME_PROTECT;		/* 从机用 */
				}
    			pCOM->step = COMMU_RX;
                //在发送中断中已经置为接收，在高速通信时，
                //可能执行此处前已经接收了数据，因此此次可以不处理
                COM_ReadyRece(u);			/* 继续处理，确保485收发引脚设置成功 */
            }
            else if (pCOM->time.txDly == 0)/* 超时 */
            {
                pCOM->commuErr = COMMU_OVERTIME;
                pCOM->step = COMMU_FAIL;
//                COM_ReadyRece(u);			/* 统一放到COMMU_FAIL处理 */
            }
	  		break;
		default :
        case COMMU_FAIL :
            COM_err_deal(u, pCOM->txBuf[0], COM_ERR_REC);	/* 记录通讯故障 */
			pCOM->step = COMMU_RX;
			COM_ReadyRece(u);
            pCOM->time.commIntProt = COMMU_TIME_PROTECT;
            pCOM->time.txDly = COMMU_TIME_TX_DLY;
			pCOM->rxCnt = 0;
			break;
	}
}

/*******************************************************************************
 函 数: COM_MasterProg()
 功 能: 串口主机处理接口
 参 数: u--串口号; 
 返 回: 空
 其 它: 主机状态迁移说明;New485Comm
	(1) 空闲：主机上电后置为"空闲"状态，等待主程序建立发送数据启动通讯，启动通讯
 后进入"发送延时"状态。
	(2) 发送延时：发送延时时间、通讯间隔时间到后进入"启动发送"状态。在此状态下将
 485芯片置为发送状态，消除总线上的干扰。
	(4) 启动发送：可在此状态下设置通讯参数。将发送缓冲区第一个数据保存到发送寄存
 器，即启动发送。通讯状态置为"发送"，等待发送完成。发送时，需要保持接收使能关闭。
	(5) 发送：后续的发送是在发送中断中完成。发送中断中将所有数据都发送完成后，将
 485置为接收状态，将通讯状态置为"接收"。如果是广播命令，状态转为"广播结束延时"。
	(6) 接收：一帧接收完成：3.5 个字符时间未收到下一数据或接收缓冲区满。9600大概
 为5ms，4800大概为10ms，软件中可统一设置为10ms。
 	若收到正确的数据帧，则将通讯状态置为"空闲"，准备下一次通讯；若收到错误的数据
 帧，将此帧数据丢弃，则继续接收下一帧数据，直到接收到正确的数据或超时。 如果超时
 后仍未收到正确的数据，则认为通讯错误，需要进行重试，将状态转为"发送延时"， 重新
 发送刚才的数据。如果"通讯重试次数"到后仍未收到正确的数据，则认为通讯故障， 置通
 讯故障标志位，通讯状态转为"空闲"，准备下一次通讯。
	(7) 广播结束延时：广播时，发送完成后必须延时一定的时间后才能转到"空闲"状态，
 以便从机能够处理完相应的操作。
*******************************************************************************/
void COM_MasterProg(U08 u)
{
	UART *pCOM;	
	BOOL rxOk=FALSE;		/* COM-V100A1-0 */

	pCOM = &Uart[u];
	switch (pCOM->step)
	{		
		case COMMU_IDLE :	/* 发送队列有待发送节点，转发送延时 */		
			if (COM_QSendNodeData(u)) 
			{	
				pCOM->failCnt = 0;
				pCOM->step = COMMU_DELAY;
				pCOM->time.txDly = COMMU_TIME_TX_DLY;
			}
			break;

		case COMMU_DELAY :	/* 延时时间到，转启动发送 */
			COM_TrasDly(u);
			if ((pCOM->time.interval==0) && (pCOM->time.txDly==0))
			{
				pCOM->step = COMMU_START;
			}
			else
			{
				break;
			}
		
		case COMMU_START:	/* 写发送缓冲区寄存器，启动发送 */
			pCOM->txCnt = 0;
			pCOM->rxCnt = 0;
			COM_WrTrasBufReg(u, pCOM->txBuf[0]);
			pCOM->step = COMMU_TX;
			pCOM->rxFrameType = RX_FRAME_NO;
            pCOM->time.txDly = COMMU_TIME_TX_TIMEOUT;
			break;
            
		case COMMU_TX :	/* 转接收由中断改为这里 */
			if (pCOM->txCnt >= pCOM->txTotal)
            {   
            	if (pCOM->txBuf[0]!=ADDR_BROADCAST && pCOM->txBuf[0]!=ADDR_BRO_FBUS)
            	{
                    pCOM->time.rxOvertime = COMMU_TIME_RX_OVER;	/* 主机用 */
        			pCOM->step = COMMU_RX;
                    COM_ReadyRece(u);
                }
                else
                {
                    pCOM->time.broadcastDly = COMMU_TIME_BROADCAST_DLY;
			        pCOM->step = COMMU_BROADCAST;
                }

            }
            else if (pCOM->time.txDly == 0)/* 超时，重试 */
            {
				pCOM->step = COMMU_DELAY;
				pCOM->time.interval = COMMU_TIME_INTERVAL;
				pCOM->time.txDly = COMMU_TIME_TX_DLY;					
			}
			break;
			
		case COMMU_RX :	/* 根据接收帧的实际情况，分别转换到不同状态 */
			/* 一帧接收完成:接收帧时间内未收到下一数据或接收缓冲区满 */
			if (((pCOM->rxCnt > 0) && (pCOM->time.rxFrame == 0)) 
				|| (pCOM->rxCnt >= COMMU_RX_SIZE)) 
			{
				COM_DisableRece(u);			/* 处理接收帧时关闭接收使能 */
				if (COM_QAcceptNodeData(u))	/* 正确帧时，转空闲 */
				{
					rxOk = TRUE;			/* COM-V100A1-0 */
					COM_QNextNode(u);		/* 指向发送队列下一个节点 */

					pCOM->time.led = COMMU_TIME_LED;
					pCOM->step = COMMU_IDLE;
					pCOM->time.interval = COMMU_TIME_INTERVAL;			
					COM_Set485Tras(u);		/* 主机设485为发送，早点减少总线上的干扰 */
					
					/* 从机异常响应 */
					if (pCOM->rxFrameType == RX_FRAME_EXCEP_RSP)
					{
						pCOM->commuErr = COMMU_EXCEP_RSP;
					}
					else
					{
						pCOM->commuErr = COMMU_NO_ERR;
					}
                    /* 清除通讯故障 */
					COM_err_deal(u, pCOM->txBuf[0], COM_ERR_CLR);
					
				//	break;	/* COM-V100A1-0 */
				}
				else		/* 错误帧时，准备接收下一帧、清接收计数器 */
				{			
					COM_ReadyRece(u);
					pCOM->rxCnt = 0;
				}
			}

			if (rxOk)
			{
				break;
			}			

			/* 接收超时时间到，仍未收到有效数据帧，为一次通讯失败 */
			if (pCOM->time.rxOvertime == 0) 
			{	
				pCOM->pRecord->fail++;
				if (pCOM->rxFrameType == RX_FRAME_NO)
				{
					pCOM->pRecord->overtime++;
				}			

                /* 已到失败重试次数，转报警 */
				if (++pCOM->failCnt >= (u==COM_INVT ? COMMU_FAIL_NUM_INVT : COMMU_FAIL_NUM)
                || (COM_err_deal(u, pCOM->txBuf[0], COM_ERR_GET) /* 已故障的从机不重试，仅针对级联口 */
				    && u==COM_FCU_FBUS
				    )
                )       
				{			
					pCOM->pRecord->alarm++;
					pCOM->step = COMMU_FAIL;
					pCOM->time.interval = COMMU_TIME_INTERVAL;

					/* 以最后一次通讯时的数据帧类型来设置通讯故障类型 */
					switch (pCOM->rxFrameType)
					{
						case RX_FRAME_NO:
							pCOM->commuErr = COMMU_OVERTIME;
							break;
						case RX_FRAME_LEN_SHORTAGE:
							pCOM->commuErr = COMMU_INTERFERE;
							break;
						case RX_FRAME_DATA_ERR:
							pCOM->commuErr = COMMU_DATA_ERR;
							break;
						case RX_FRAME_EXCEP_RSP:	/* COM-V100A2-0 */
							pCOM->commuErr = COMMU_EXCEP_RSP;
//							COM_QNextNode(u);		/* 指向发送队列下一个节点 */
							break;
						default:
							break;
					}
				}
				else		/* 未到失败重试次数，通讯重试转延时 */
				{
					pCOM->step = COMMU_DELAY;
					pCOM->time.interval = COMMU_TIME_INTERVAL;
					pCOM->time.txDly = COMMU_TIME_TX_DLY;					
				}
			}
			break;

		case COMMU_BROADCAST :				/* 广播结束延时到后，转空闲 */
			if (pCOM->time.broadcastDly == 0)
			{
				pCOM->step = COMMU_IDLE;
				pCOM->pRecord->broadcast++;
				COM_QNextNode(u);	/* 指向发送队列下一个节点 */
			}	
			break;
			
		case COMMU_FAIL :	/* 故障自动复位，转空闲 */
		    COM_QNextNode(u);	/* 指向发送队列下一个节点 */
            COM_err_deal(u, pCOM->txBuf[0], COM_ERR_REC);	/* 记录通讯故障 */
			pCOM->step = COMMU_IDLE;
			break;
			
		default :
			pCOM->step = COMMU_IDLE;
			break;
	}
}



/**
 * 函数 : COM_HMIMasterProg()
 * 功能 : ①串口主机处理接口
 *	      ②HMI显示屏通讯程序，由于通讯数据不标准，因此函数内容特殊处理
 *
 * 参数 : @u 表示串口号; 
 * 返回 : --
 *
 * 备注 : XMacro 2017/01/16--1546
 */
void COM_HMIMasterProg(U08 u)
{
	UART *pCOM;	
	BOOL rxOk=FALSE;		/* COM-V100A1-0 */

	pCOM = &Uart[u];

	switch (pCOM->step)
	{		
		case COMMU_IDLE :	/* 发送队列有待发送节点，转发送延时 */		
			if (hmi_trans(pCOM))
			{	
				pCOM->failCnt = 0;
				pCOM->step = COMMU_DELAY;
				pCOM->time.txDly = COMMU_TIME_TX_DLY;
			}
			break;

		case COMMU_DELAY :	/* 延时时间到，转启动发送 */
			COM_TrasDly(u);
			if ((pCOM->time.interval==0) && (pCOM->time.txDly==0))
			{
				pCOM->step = COMMU_START;
			}
			else
			{
				break;
			}
		
		case COMMU_START:	/* 写发送缓冲区寄存器，启动发送 */
			pCOM->txCnt = 0;
			pCOM->rxCnt = 0;
			COM_WrTrasBufReg(u, pCOM->txBuf[0]);
			pCOM->step = COMMU_TX;
			pCOM->rxFrameType = RX_FRAME_NO;
            pCOM->time.txDly = COMMU_TIME_TX_TIMEOUT;
			break;

		case COMMU_TX :	/* 转接收由中断改为这里 */
			if (pCOM->txCnt >= pCOM->txTotal)
            {   
            	if (pCOM->txBuf[0] != ADDR_BROADCAST)
            	{
                    pCOM->time.rxOvertime = COMMU_TIME_RX_OVER;	/* 主机用 */
        			pCOM->step = COMMU_RX;
                    COM_ReadyRece(u);
                }
                else
                {
                    pCOM->time.broadcastDly = COMMU_TIME_BROADCAST_DLY;
			        pCOM->step = COMMU_BROADCAST;
                }

            }
            else if (pCOM->time.txDly == 0)/* 超时，重试 */
            {
				pCOM->step = COMMU_DELAY;
				pCOM->time.interval = COMMU_TIME_INTERVAL;
				pCOM->time.txDly = COMMU_TIME_TX_DLY;					
			}
			break;
			
		case COMMU_RX :	/* 根据接收帧的实际情况，分别转换到不同状态 */
			/* 一帧接收完成:接收帧时间内未收到下一数据或接收缓冲区满 */
			if (((pCOM->rxCnt > 0) && (pCOM->time.rxFrame == 0)) 
				|| (pCOM->rxCnt >= COMMU_RX_SIZE)) 
			{
				COM_DisableRece(u);			/* 处理接收帧时关闭接收使能 */

				if (hmi_rece(pCOM))	/* 正确帧时，转空闲 */
				{
					rxOk = TRUE;			/* COM-V100A1-0 */

					pCOM->time.led = COMMU_TIME_LED;
					pCOM->step = COMMU_IDLE;
					pCOM->time.interval = COMMU_TIME_INTERVAL;			
					COM_Set485Tras(u);		/* 主机设485为发送，早点减少总线上的干扰 */

					/* 从机异常响应 */
					if (pCOM->rxFrameType == RX_FRAME_EXCEP_RSP)
					{
						pCOM->commuErr = COMMU_EXCEP_RSP;
					}
					else
					{
						pCOM->commuErr = COMMU_NO_ERR;
					}
                    COM_err_deal(u, hmi_t_num, COM_ERR_CLR);
					_CNT(SH_hmi_commu_succ);	//注意:通讯成功后需要累加成功数，否则会被认为是通讯失败而切换成主口
				//	break;	/* COM-V100A1-0 */
				}
				else		/* 错误帧时，准备接收下一帧、清接收计数器 */
				{			
					COM_ReadyRece(u);
					pCOM->rxCnt = 0;
				}
			}
			
			/* COM-V100A1-0 */
            //errcm_alarm_fbus(u,pCOM->txBuf[0], pCOM->time.rxOvertime,rxOk);

			if (rxOk)
			{
				break;
			}			

			/* 接收超时时间到，仍未收到有效数据帧，为一次通讯失败 */
			if (pCOM->time.rxOvertime == 0) 
			{	
				pCOM->pRecord->fail++;
				if (pCOM->rxFrameType == RX_FRAME_NO)
				{
					pCOM->pRecord->overtime++;
				}			
				
				if (++pCOM->failCnt >= COMMU_FAIL_NUM)		/* 已到失败重试次数，转报警 */
				{			
					pCOM->pRecord->alarm++;
					pCOM->step = COMMU_FAIL;
					pCOM->time.interval = COMMU_TIME_INTERVAL;

					/* 以最后一次通讯时的数据帧类型来设置通讯故障类型 */
					switch (pCOM->rxFrameType)
					{
						case RX_FRAME_NO:
							pCOM->commuErr = COMMU_OVERTIME;
							break;
						case RX_FRAME_LEN_SHORTAGE:
							pCOM->commuErr = COMMU_INTERFERE;
							break;
						case RX_FRAME_DATA_ERR:
							pCOM->commuErr = COMMU_DATA_ERR;
							break;
						case RX_FRAME_EXCEP_RSP:	/* COM-V100A2-0 */
							pCOM->commuErr = COMMU_EXCEP_RSP;
							break;
						default:
							break;
					}
				}
				else		/* 未到失败重试次数，通讯重试转延时 */
				{
					pCOM->step = COMMU_DELAY;
					pCOM->time.interval = COMMU_TIME_INTERVAL;
					pCOM->time.txDly = COMMU_TIME_TX_DLY;					
				}
			}
			break;

		case COMMU_BROADCAST :				/* 广播结束延时到后，转空闲 */
			if (pCOM->time.broadcastDly == 0)
			{
				pCOM->step = COMMU_IDLE;
				pCOM->pRecord->broadcast++;
			}	
			break;
			
		case COMMU_FAIL :	/* 故障自动复位，转空闲 */
			pCOM->step = COMMU_IDLE;
			COM_err_deal(u, hmi_t_num, COM_ERR_REC);	/* 记录通讯故障 */
			break;
			
		default :
			pCOM->step = COMMU_IDLE;
			break;
	}
}

/*******************************************************************************
	以下为传输层函数，与内部标准协议相关
*******************************************************************************/

/*******************************************************************************
 函 数: COM_QEmpty()
 功 能: 判断发送队列是否为空
 参 数: u--串口号; 
 返 回: TRUE/FALSE--发送队列为空/非空
 其 它: 空
*******************************************************************************/
BOOL COM_QEmpty(U08 u)		
{	
	return  (Queue[u].cnt == 0);			
}

/*******************************************************************************
 函 数: COM_QFull()
 功 能: 判断发送队列是否为满
 参 数: u--串口号; 
 返 回: TRUE/FALSE--发送队列为满/非满
 其 它: 空
*******************************************************************************/
BOOL COM_QFull(U08 u)		
{	
//	return  (Queue[u].cnt == MAX_NODE_NUM);	
	return  (Queue[u].cnt >= MAX_NODE_NUM);	/* COM-V100A0-1 */
}

/*******************************************************************************
 函 数: COM_QGetNodeCnt()
 功 能: 获取发送队列有效个数
 参 数: u--串口号; 
 返 回: 发送队列有效个数
 其 它: 空
*******************************************************************************/
U08 COM_QGetNodeCnt(U08 u)				
{	
	return  Queue[u].cnt;					
}

/*******************************************************************************
 函 数: COM_QInit()
 功 能: 初始化发送队列
 参 数: u--串口号; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void COM_QInit(U08 u)
{
	U08 i;
	SENDQUEUE *pq;
	
	pq = &Queue[u];
	
	pq->head = 0;
	pq->end = 0;
	pq->cnt = 0;
	pq->insertPos = 0;
	for (i=0; i<MAX_NODE_NUM; i++)
	{
		pq->node[i].next = i+1;
	}
}

/*******************************************************************************
 函 数: COM_QNextNode()
 功 能: 指向发送队列下一个节点
 参 数: u--串口号; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void COM_QNextNode(U08 u)
{
	U08 newHead;
	SENDQUEUE *pq;

	pq = &Queue[u];

	pq->node[pq->head].sta = NODE_STA_FINESH; /* 当前节点发送完成 */
	
	if (pq->cnt == 0)
	{
		return;
	}
	
	if (pq->cnt > 1) 
	{
		newHead = pq->node[pq->head].next;

		pq->node[pq->head].next = pq->node[pq->end].next;
		pq->node[pq->end].next = pq->head;

		if (pq->insertPos == pq->head) 
		{
			pq->insertPos = newHead;
		}
		
		pq->head = newHead;
		pq->node[pq->head].sta = NODE_STA_SEND; /* 下一节点准备发送 */
	}
	pq->cnt--;
}

/*******************************************************************************
 函 数: COM_QInsertNode()
 功 能: 向队列中插入一个节点
 参 数: u--串口号; 
		prio--优先级 Q_PRIO_LOW/Q_PRIO_HIGH; 
		pComMessage--通讯节点指针; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL COM_QInsertNode(U08 u, U08 prio, const QNODE *pNode)
{
	U08 no;
	SENDQUEUE *pq;

	pq = &Queue[u];

	if (COM_QEmpty(u))		/* 队列空 */
	{
		no = pq->end;
	}
	else if (COM_QFull(u))	/* 队列满 */
	{
		if (prio == Q_PRIO_HIGH)			/* 高优先级 */
		{		
			if (pq->insertPos == pq->end)  
			{
				return FALSE;				/* 全部都是高优先级队列了 */
			}

			no = pq->node[pq->insertPos].next;
			pq->insertPos = no;
		}
		else 
		{					/* 低优先级 */
			return FALSE;
		}
	}
	else
	{
		if (prio == Q_PRIO_HIGH)			/* 高优先级 */
		{ 		
			if (pq->insertPos == pq->end)	/* 队列里全部都是高优先级的 */
			{	
				no = pq->node[pq->end].next;
				pq->end = no;
				pq->insertPos = no;
			}
			else 
			{
				no = pq->node[pq->end].next;
				pq->node[pq->end].next = pq->node[no].next;

				pq->node[no].next = pq->node[pq->insertPos].next;
				pq->node[pq->insertPos].next = no;
				pq->insertPos = no;
			}
		}
		else 
		{					/* 低优先级 */
			no = pq->node[pq->end].next;
			pq->end = no;
		}
	}

	pq->node[no].driverType = pNode->driverType;
	pq->node[no].data = pNode->data;
	pq->node[no].sta = NODE_STA_CREATE;
/*==============================================================================
COM-V100A0-0
1.增加条件判断，队列不满时，队列节点计数器才加1
2.以前是没有条件判断的，下述情况时会出错:
  队列满且是高优先级命令(见上面的程序分支处理)，高优先节点会覆盖低优先级节点，
  无条件判断时，队列节点计数器会加1，即已大于最大的节点数(MAX_NODE_NUM)；
  下次再插入一个低优先级命令，COM_QFull(u)会返回FALSE；
  最终会导致插入的节点数据超过定义的节点数组范围，即意外覆盖节点数组后面定义的变量。
==============================================================================*/
//	pq->cnt++;	
	if (!COM_QFull(u))
	{
		pq->cnt++;
	}
	
	return TRUE;
}

/*******************************************************************************
	以下为网络层函数(包括modbus.c中的函数)，与通信对象相关
*******************************************************************************/

/*******************************************************************************
 函 数: COM_QAcceptNodeData()
 功 能: 从接收缓冲区接收节点数据
 参 数: u--串口号; 
 返 回: TRUE/FALSE--接收数据正确/错误
 其 它: 空
*******************************************************************************/
BOOL COM_QAcceptNodeData(U08 u)
{
	QNODE *pNode;
	UART *pCOM;
	
	pNode = &Queue[u].node[Queue[u].head];
	pCOM = &Uart[u];
	
	
    switch (pNode->driverType)
    {
		case DRIVER_MB_MASTER:	
//			if (u == COM_CAN)	
//			{
//			    return MB_CAN_MasterRece(&pNode->data);
//			}
//			else
//			{
			    return MB_MasterRx(u, pCOM, &pNode->data);
//			}
			
		default:	
			return  FALSE;
    }
}

/*******************************************************************************
 函 数: COM_QSendNodeData()
 功 能: 发送节点数据至发送缓冲区
 参 数: u--串口号; 
 返 回: TRUE/FALSE--置发送缓冲区成功/失败
 其 它: 空
*******************************************************************************/
BOOL COM_QSendNodeData(U08 u)
{
	QNODE *pNode;
	UART *pCOM;
    
    if (COM_QEmpty(u)) 
    {
    	return FALSE;
    }
       
    pNode = &Queue[u].node[Queue[u].head];
    pCOM = &Uart[u];
    
    /* 由于CAN通讯时只少了设备地址和CRC，所以与标准MODBUS共用数据结构，不区分设备类型，
    	仅用串口号区分；以后有协议偏差较大时再用“设备类型”区分 */
    switch (pNode->driverType)
    {
	    case DRIVER_MB_MASTER:	
	    	if (MB_MasterTx(&pNode->data, pCOM))
	    	{
	    		return TRUE;
	    	}	    	
	    	break;
		default:	
			break;
    }

    /* 发送失败时丢弃该节点，指向下个节点 */
    COM_QNextNode(u);
    
    return FALSE;
}

/*******************************************************************************
	以下为应用层函数，提供模块接口
*******************************************************************************/
/*******************************************************************************
 函 数: COM_WriteWords()
 功 能: 向指定串口通讯队内插入命令:写多个保持寄存器(可读写字)
 参 数: u--串口号; 
        pcb--板号;
		startAddr--起始地址; 
		length--长度; 
		pData--数据指针; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 写命令用高优先级
*******************************************************************************/
BOOL COM_WriteWords(U08 u, U08 pcb, U16 startAddr, U16 length, const U16 *pData, U08 prio)
{
	QNODE node;	

	node.driverType = DRIVER_MB_MASTER;
	node.data.driverNo = pcb;
	node.data.cmd = MB_WR_MULTIPLE_REGISTERS;
	node.data.startAddr = startAddr;
	node.data.length_wr = length;
	node.data.pData_wr = pData;
	
	return COM_QInsertNode(u, prio, &node);
}

/*******************************************************************************
 函 数: COM_WriteOneWord()
 功 能: 向指定串口通讯队内插入命令:写单个保持寄存器(可读写字)
 参 数: u--串口号; 
         pcb--板号;
		startAddr--起始地址; 
		val--写值; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 写命令用高优先级
*******************************************************************************/
BOOL COM_WriteOneWord(U08 u,U08 pcb, U16 startAddr, U16 val, U08 prio)
{
	QNODE node;	

	node.driverType = DRIVER_MB_MASTER;
	node.data.driverNo = pcb;
	node.data.cmd = MB_WR_SINGLE_REGISTER;
	node.data.startAddr = startAddr;
	node.data.oneWrVal = val;
	node.data.pData_wr = NULL;
	
	return COM_QInsertNode(u, prio, &node);
}

/*******************************************************************************
 函 数: COM_WriteOneBit()
 功 能: 向指定串口通讯队内插入命令:写单个线圈(可读写位)
 参 数: u--串口号;
        pcb--板号;
		startAddr--起始地址; 
		val--写值; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 写命令用高优先级
*******************************************************************************/
BOOL COM_WriteOneBit(U08 u,U08 pcb,U16 startAddr, U16 val, U08 prio)
{
	QNODE node;	

	node.driverType = DRIVER_MB_MASTER;
	node.data.driverNo = pcb;
	node.data.cmd = MB_WR_SINGLE_COIL;
	node.data.startAddr = startAddr;
	node.data.oneWrVal = val;
	node.data.pData_wr = NULL;
	
	return COM_QInsertNode(u, prio, &node);
}

/*******************************************************************************
 函 数: COM_ReadRWwords()
 功 能: 向指定串口通讯队内插入命令:读保持寄存器(可读写字)
 参 数: u--串口号; 
         pcb--板号;
		startAddr--起始地址; 
		length--长度; 
		pData--数据指针; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 
*******************************************************************************/
BOOL COM_ReadRWwords(U08 u, U08 pcb, U16 startAddr, U16 length, U16 *pData, U08 prio)
{
	QNODE node;	

	node.driverType = DRIVER_MB_MASTER;
	node.data.driverNo = pcb;
	node.data.cmd = MB_RD_HOLDING_REGISTERS;
	node.data.startAddr = startAddr;
	node.data.length_rd = length;
	node.data.pData_rd = pData;
	
	return COM_QInsertNode(u, prio, &node);
}

/*******************************************************************************
 函 数: COM_ReadROwords()
 功 能: 向指定串口通讯队内插入命令:读输入寄存器(只读字)
 参 数: u--串口号;
        pcb--板号;
		startAddr--起始地址; 
		length--长度; 
		pData--数据指针; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 
*******************************************************************************/
BOOL COM_ReadROwords(U08 u, U08 pcb, U16 startAddr, U16 length, U16 *pData, U08 prio)
{
	QNODE node;	

	node.driverType = DRIVER_MB_MASTER;
	node.data.driverNo = pcb;
	node.data.cmd = MB_RD_INPUT_REGISTERS;
	node.data.startAddr = startAddr;
	node.data.length_rd = length;
	node.data.pData_rd = pData;
	
	return COM_QInsertNode(u, prio, &node);
}

/*******************************************************************************
 函 数: COM_Config()
 功 能: 串口设置
 参 数: u--编号; 
		baud--波特率; 
		dataType--数据格式; 
 返 回: 空
 其 它: 库接口函数
*******************************************************************************/
const U32 baudRateList[] = 
{
    2400,
    4800,
    9600,
   19200,
   38400,
   57600,
  115200,
};
void COM_Config(U08 u,U08 baud, U08 dataType)
{
	MCU_UART_SetFormat(u, baudRateList[baud], (MCU_UART_FormatTypeDef)dataType);
}

U08 COM_GetNodeSta(U08 u, U08 node_idx)
{
    U08 sta = NODE_STA_NONE;
	SENDQUEUE *pq;

	pq = &Queue[u];
    
    if(u >=MAX_UART || node_idx >= MAX_NODE_NUM)
    {
        sta = NODE_STA_NONE;
    }
    else
    {
        sta = pq->node[node_idx].sta;
    }
    
    return sta;
}

U08 COM_GetNodeIdx(U08 u, U08 prio)
{
    U08 node_idx = 0xFF;
	SENDQUEUE *pq;

	pq = &Queue[u];
    
    if(prio == Q_PRIO_HIGH)
    {
        node_idx = pq->insertPos;
    }
    else
    {
        node_idx = pq->end;
    }

    return node_idx;
}
void  LOG_COM_WrTrasBufReg(U08 u, U08 value)
{
    COM_Set485Tras(u);
    COM_WrTrasBufReg(u, value);
}

