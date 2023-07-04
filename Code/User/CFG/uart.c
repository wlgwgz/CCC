/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: uart.c
 �ļ�����: 
 ������  : 
 ��������:
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

#define ENABLE_UART0_RECE()		MCU_UART_EnableRx(MCU_UART0_NUM)		/* ʹ�ܽ������� */
#define DISABLE_UART0_RECE()	MCU_UART_DisableRx(MCU_UART0_NUM)		/* �ؽ������� */
#define ENABLE_UART0_TRAS()	/* ʹ�ܷ�������ȷ�ϵײ�ͨѶ�������Ƿ��Ѵ� */
#define DISABLE_UART0_TRAS()/* �ط������� */
#define ENABLE_UART4_RECE()		MCU_UART_EnableRx(MCU_UART0_NUM)		/* ʹ�ܽ������� */
#define DISABLE_UART4_RECE()	(U2C1_CCR &= ~0x4000)		/* �ؽ������� */
#define ENABLE_UART4_TRAS()	/* ʹ�ܷ������� */
#define DISABLE_UART4_TRAS()/* �ط������� */
#define ENABLE_UART5_RECE()		MCU_UART_EnableRx(MCU_UART0_NUM)		/* ʹ�ܽ������� */
#define DISABLE_UART5_RECE()	(U2C1_CCR &= ~0x4000)		/* �ؽ������� */
#define ENABLE_UART5_TRAS()	/* ʹ�ܷ������� */
#define DISABLE_UART5_TRAS()/* �ط������� */
#define ENABLE_UART6_RECE()		MCU_UART_EnableRx(MCU_UART0_NUM)		/* ʹ�ܽ������� */
#define DISABLE_UART6_RECE()	(U2C1_CCR &= ~0x4000)		/* �ؽ������� */
#define ENABLE_UART6_TRAS()	/* ʹ�ܷ������� */
#define DISABLE_UART6_TRAS()/* �ط������� */

#define CLR_UART0_RX_IR_BIT()	(U0C0_PSCR |= 0x4000)		/* ������жϱ�־λ */
#define CLR_UART0_TX_IR_BIT()	(U0C0_PSCR |= 0x2000)		/* �巢���жϱ�־λ */
#define CLR_UART1_RX_IR_BIT()	(U0C1_PSCR |= 0x4000)		/* ������жϱ�־λ */
#define CLR_UART1_TX_IR_BIT()	(U0C1_PSCR |= 0x2000)		/* �巢���жϱ�־λ */
#define CLR_UART2_RX_IR_BIT()	(U1C0_PSCR |= 0x4000)		/* ������жϱ�־λ */
#define CLR_UART2_TX_IR_BIT()	(U1C0_PSCR |= 0x2000)		/* �巢���жϱ�־λ */
#define CLR_UART3_RX_IR_BIT()	(U2C0_PSCR |= 0x4000)		/* ������жϱ�־λ */
#define CLR_UART3_TX_IR_BIT()	(U2C0_PSCR |= 0x2000)		/* �巢���жϱ�־λ */
#define CLR_UART4_RX_IR_BIT()	(U2C1_PSCR |= 0x4000)		/* ������жϱ�־λ */
#define CLR_UART4_TX_IR_BIT()	(U2C1_PSCR |= 0x2000)		/* �巢���жϱ�־λ */


#define UART0_TRAS(val)			(U0C0_TBUF00 = val)			/* д���ͻ���Ĵ��� */
#define UART0_RECE()			((U08)U0C0_RBUF)			/* ���ջ���Ĵ��� */
#define UART1_TRAS(val)			(U0C1_TBUF00 = val)			/* д���ͻ���Ĵ��� */
#define UART1_RECE()			((U08)U0C1_RBUF)			/* ���ջ���Ĵ��� */
#define UART2_TRAS(val)			(U1C0_TBUF00 = val)			/* д���ͻ���Ĵ��� */
#define UART2_RECE()			((U08)U1C0_RBUF)			/* ���ջ���Ĵ��� */
#define UART3_TRAS(val)			(U2C0_TBUF00 = val)			/* д���ͻ���Ĵ��� */
#define UART3_RECE()			((U08)U2C0_RBUF)			/* ���ջ���Ĵ��� */
#define UART4_TRAS(val)			(U2C1_TBUF00 = val)			/* д���ͻ���Ĵ��� */
#define UART4_RECE()			((U08)U2C1_RBUF)			/* ���ջ���Ĵ��� */


static SENDQUEUE Queue[MAX_UART];	/* ͨѶ���У�����ʹ�ö��з��ͣ��ӻ���ʹ�� */
/*******************************************************************************
 �� ��: COM_DisableRece()
 �� ��: �ؽ�������
 �� ��: u--���ں�; 
 �� ��: ��
 �� ��: ��ȷ�����崮��ʱ���ã���DealUartMaster()�е���
*******************************************************************************/
void COM_DisableRece(U08 u)
{
	MCU_UART_DisableRx(u);
}

/*******************************************************************************
 �� ��: COM_ReadyRece()
 �� ��: ���ڽ���׼��
 �� ��: u--���ں�; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void COM_ReadyRece(U08 u)
{
	volatile U08 receData;
	
	MCU_UART_ClrRxIr(u);
	receData = MCU_UART_Rx(u);/* ����������� */
	MCU_UART_EnableRx(u);
	MCU_UART_Set485Rx(u);
	}


/*******************************************************************************
 �� ��: COM_Set485Tras()
 �� ��: ����485Ϊ����״̬
 �� ��: u--���ں�; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void COM_Set485Tras(U08 u)
{
	MCU_UART_Set485Tx(u);
}

/*******************************************************************************
 �� ��: COM_UartRxInt()
 �� ��: ���ڽ����ж�
 �� ��: u--���ں�
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void COM_UartRxInt(U08 u)
{
	UART *pCOM;
    // ��Ҫͨ����ȡ���,����ȶ�
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
 �� ��: COM_UartTxInt()
 �� ��: ���ڷ����ж�
 �� ��: u--���ں�
 �� ��: ��
 �� ��: ��
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
        //ˢ�·��ͳ�ʱ������Ͳ������£�ʱ�䲻��
        pCOM->time.txDly = COMMU_TIME_TX_TIMEOUT;
		pCOM->rxCnt = 0;
	}
    
	

}
/*******************************************************************************
 �� ��: COM_TrasDly()
 �� ��: ���ڷ�����ʱ�׶δ���
 �� ��: u--���ں�; 
 �� ��: ��
 �� ��: �ؽ�����������485Ϊ����״̬
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
 �� ��: COM_WrTrasBufReg()
 �� ��: д���ڷ��ͻ������Ĵ���(��������)
 �� ��: u--���ں�;
 		value--д���ֵ;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void COM_WrTrasBufReg(U08 u, U08 value)
{
	MCU_UART_Tx(u, value);
    MCU_UART_EnableTx(u);    
}

/*******************************************************************************
	����Ϊ������·���е�Ӧ�ýӿں�������
*******************************************************************************/

/*******************************************************************************
 �� ��: COM_GetErr()
 �� ��: ��ȡͨѶ��������
 �� ��: u--���ں�; 
 �� ��: ͨѶ���Ͼ�������
 COMMU_NO_ERR	��ͨѶ����
 COMMU_OVERTIME	[�����ж�]ͨѶ��ʱ����:δ�յ�����֡
 COMMU_INTERFERE[�����ж�]ͨѶ���Ź���:����֡���ȶ�(����<4��С�����۸���)
 COMMU_DATA_ERR	[�����ж�]ͨѶ���ݹ���:��������֡�������ǵ�ַ�����ݴ�У��ʹ��� 
 COMMU_EXCEP_RSP[�����ж�]�ӻ��ظ��쳣��Ӧ
 COMMU_INT_PROT	[�ӻ��ж�]ͨѶ�жϱ���:�����жϱ���ʱ����δ�յ���Ч����֡ 
 �� ��: 1����COMMU_EXCEP_RSPʱ�����л�ָ���¸��ڵ����ͨѶ
 		2����COMMU_INT_PROTʱ����Ӱ��ӻ���ͨѶ״̬���ҵ��յ�����֡ʱ���Զ���λ;
 		Ӧ�ò�ɸ���ʵ���������������ʩ�򲻴���
*******************************************************************************/
U08 COM_GetErr(U08 u)	
{	
	return Uart[u].commuErr;
}

/*******************************************************************************
 �� ��: COM_GetMbExceptCode()
 �� ��: ��ȡ�쳣��Ӧʱ��Modbus�쳣��
 �� ��: u--���ں�; 
 �� ��: Modbus�쳣��
	 MB_NO_ERR					0		���쳣
	 MB_ILLEGAL_FUNCTION       	1		�Ƿ����� 
	 MB_ILLEGAL_DATA_ADDR       2		�Ƿ����ݵ�ַ 
	 MB_ILLEGAL_DATA_VALUE      3		�Ƿ�����ֵ 
	 MB_SLAVE_DEVICE_FALUE      4		�ӻ��豸���ϣ��ӻ��޷����� 
 �� ��: ��
*******************************************************************************/
U08 COM_GetMbExceptCode(U08 u)	
{	
	return Uart[u].exceptCode;
}

/*******************************************************************************
 �� ��: COM_ResetAlarm()
 �� ��: ��λͨѶ����
 �� ��: u--���ں�; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void COM_ResetAlarm(U08 u)
{
	Uart[u].step = COMMU_IDLE;
	Uart[u].commuErr = COMMU_NO_ERR;
	Uart[u].exceptCode = MB_NO_ERR;
	COM_QInit(u);
}

/*******************************************************************************
 �� ��: COM_Idle()
 �� ��: �ж�ͨѶ״̬�Ƿ����
 �� ��: u--���ں�; 
 �� ��: TRUE/FALSE--ͨѶ����/������
 �� ��: ��
*******************************************************************************/
BOOL COM_Idle(U08 u)
{
	return (Uart[u].step == COMMU_IDLE);
}

/*******************************************************************************
 �� ��: COM_GetLedTime()
 �� ��: ��ȡͨѶ����˸��ʱ
 �� ��: u--���ں�; 
 �� ��: ͨѶ����˸��ʱ
 �� ��: ����ʱ�Ĵ�����ʱʱ�䲻Ϊ0ʱ�ر�ָʾ�ƣ�Ϊ0ʱ�򿪣�
 		��ͨѶ����ʱ��˸��ʾ��ʧ��ʱ������
*******************************************************************************/
U08 COM_GetLedTime(U08 u)
{
	return Uart[u].time.led;
}

/*******************************************************************************
 �� ��: COM_2MsTimer()
 �� ��: ͨѶ��ʱ(2ms����һ��)
 �� ��: ��
 �� ��: ��
 �� ��: ��
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
 �� ��: COM_VariableTimer()
 �� ��: �ɱ��ʱ(2ms/5ms����һ��)
 �� ��: ��
 �� ��: ��
 �� ��: ��
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
 �� ��: COM_ProtectTimer()
 �� ��: �ӻ�ͨѶ����ʱ���ʱ���뵥λ
 �� ��: ��
 �� ��: ��
 �� ��: ��
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
 �� ��: COM_ClrRecord()
 �� ��: ���ͨѶ��¼��Ϣ
 �� ��: ��; 
 �� ��: ��
 �� ��: ��
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
 �� ��: COM_err_deal()
 �� ��: ͨѶ���ϴ���
 �� ��: u--���ں�; 
		addr--��ַ; 
		op--ѡ��������¼; 
 �� ��: ��
 �� ��: ע��ͨѶ�Ź���ʱ�Ĵ��������ͻ
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
//                                if (!fg_baudrate_confirm[u])  /* ������δȷ�ϣ������ڴ˴��л������� */		
//                                {
//                                    if (id == INVT_ID_1)			/* ������Ƶѹ���Ĳ���������Ӧ */
//                                    {
//                                        com_baudrate[u] = (com_baudrate[u] == B_9600) ? B_2400 : B_9600;
//                                        COM_Config(u, com_baudrate[u], T_8N1);
//                                    }
//                                }
//                                else		/* ��������ȷ�� */
//                                {
//                                    U08 exio_id;
//                                    for (exio_id = 0; exio_id < USE_EXIO; exio_id++)
//                                    {
//                                        if (GET_EXIO_COMMU_ERR(exio_id))
//                                        {
//                                            /* ������������ȷ�� */
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
 �� ��: COM_InitSlaver()
 �� ��: ��ʼ��ָ�����ں�Ϊ�ӻ�
 �� ��: u--���ں�; 
 		pRcd--ͨѶ��Ϣ��¼�ṹ��ָ��
 		addr--�ӻ���ַ
 �� ��: ��
 �� ��: ��ʼ������ 
		����: �ؽ�������485���͡�COMMU_IDLE
		�ӻ�: ����������485���ա�COMMU_RECE��ͨѶ����ʱ��
*******************************************************************************/
void COM_InitSlaver(U08 u, COMRECORD *pRcd, U08 addr)
{
	UART *pCOM;
	
	/* ʹ�ܷ�����������������485���� */
	COM_ReadyRece(u);
	
	pCOM = &Uart[u];
	
	/* ��ȫ����ʼ��Ϊ0, �ٳ�ʼ������������� */
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
		pCOM->time.commIntProt = COMMU_TIME_PROTECT+5;			/* ��һ�γ�ʼ�����ʵ��ӳ� */
	}
}

/*******************************************************************************
 �� ��: COM_InitMaster()
 �� ��: ��ʼ��ָ�����ں�Ϊ����
 �� ��: u--���ں�; 
 		pRcd--ͨѶ��Ϣ��¼�ṹ��ָ��
 �� ��: ��
 �� ��: �� 
*******************************************************************************/
void COM_InitMaster(U08 u, COMRECORD *pRcd)
{
	UART *pCOM;
	
	/* ʹ�ܷ��������ؽ�������485���� */
	COM_ReadyRece(u);
	
	pCOM = &Uart[u];
	
	/* ��ȫ����ʼ��Ϊ0, �ٳ�ʼ������������� */
	MEMCLR(pCOM, sizeof(UART));
	pCOM->ownAddr = 0;
	pCOM->step = COMMU_IDLE;
	pCOM->pRecord = pRcd;
	pCOM->commuErr = COMMU_NO_ERR;
	
	/* ��ʼ�����Ͷ��� */
	COM_QInit(u);				
}

/*******************************************************************************
 �� ��: COM_config_and_init()
 �� ��: ��ʼ��ͨѶ
 �� ��: ��
 �� ��: ��
 �� ��: ��
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

        case COM_BMS:/* BMS��� */
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
            
        case COM_TEXT_HMI:/* ��ʾ�� */
            com_baudrate[u] = B_19200;
//            fg_baudrate_confirm[u] = TRUE;
            COM_Config(u, com_baudrate[u], T_8N1);
            COM_InitMaster(u, BKBD_INVT_UART3);	        
            break;
            
        case COM_INVT:/* ��Ƶ */
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
 �� ��: COM_Init()
 �� ��: ��ʼ��ͨѶ
 �� ��: ��
 �� ��: ��
 �� ��: ��
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
 �� ��: COM_SlaverProg()
 �� ��: ���ڴӻ�����ӿ�
 �� ��: u--���ں�; 
 �� ��: ��
 �� ��: ע���ϵ��ӻ���ʼ��; New485Comm
	(1) ���գ��ϵ�󣬴ӻ�����"����"״̬���򿪽���ʹ�ܣ�485оƬ��Ϊ����״̬�� ��
 ��һ֡���ݺ���Ҫ�����ݽ�����Ӧ�жϣ����������������ж���ͬ�� ���յ���ȷ���ݺ�
 "ͨѶ�жϱ���ʱ��"���¿�ʼ��ʱ�����ǵ��������Ҫ�ظ����ݣ�ͨѶ״̬תΪ"��ʱ",
 ���ǹ㲥������ܻظ����ݣ������յ�����֡������Ҫ���½�����һ֡���ݡ�
 	ͨѶ�жϱ���ʱ�䵽����δ�յ���Ч����֡����ͨѶ�жϱ������ϡ�
 	(2) ��ʱ������ǰ����رս���ʹ�ܣ���ֹ�����յ��������ݡ���485 оƬ��Ϊ����״
 ̬�����������ϵĸ��š�������ʱʱ�䵽�����"��������"״̬��	
 	(3) �������ͣ������ͻ�������һ�����ݴ��뷢�ͼĴ������������͡���ͨѶ״̬��Ϊ
 "����"����"����"״̬�µȴ�������ɡ�
 	(4) ���ͣ������жϽ��������ݷ�����ɺ󣬽�485 оƬ��Ϊ����״̬����ͨѶ״̬��
 Ϊ"����"��
*******************************************************************************/
void COM_SlaverProg(U08 u)
{
	UART *pCOM;

	pCOM = &Uart[u];
    
	switch (pCOM->step)
	{
	  	case COMMU_RX :	/* �յ���ȷ֡�ҷǹ㲥ʱ��תΪ������ʱ������һֱ���� */
			/* һ֡�������:����֡ʱ����δ�յ���һ���ݻ���ջ������� */
			if ((pCOM->rxCnt>0 && pCOM->time.rxFrame==0)
				|| (pCOM->rxCnt >= COMMU_RX_SIZE)) 
	  		{	 
	  			COM_DisableRece(u);			/* �������֡ʱ�رս���ʹ�� */
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
	  				pCOM->commuErr = COMMU_NO_ERR;			/* �Զ���λ */
	  				
	  				if (pCOM->rxBuf[0]!=ADDR_BROADCAST && pCOM->rxBuf[0]!=ADDR_BRO_FBUS)	/* ���ǹ㲥, �Ż������� */
				    {
						pCOM->step = COMMU_DELAY;
						pCOM->time.txDly = COMMU_TIME_TX_DLY;
				    } 
				    else	/* �㲥ʱ��׼��������һ֡ */
				    {			    	
				    	COM_ReadyRece(u);
				    }			    
				}
				else		/* ����֡ʱ��׼��������һ֡ */
				{				
					COM_ReadyRece(u);
				}
				
				pCOM->rxCnt = 0;			/* ����һ֡���ݺ�����ռ����� */
	  		}

	  		/* ͨѶ�жϱ���ʱ�䵽����δ�յ���Ч����֡����ͨѶ�жϱ������� */
	  		if (pCOM->time.commIntProt == 0)
	  		{
	  			pCOM->commuErr = COMMU_INT_PROT;
                pCOM->step = COMMU_FAIL;
                if (u == COM_4G)
				{
					app_4g_commIntProt_deal();	// 4Gͨ���жϱ���ʱ�䵽������4Gģ��
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
			COM_err_deal(u, pCOM->txBuf[0], COM_ERR_CLR);   /* ���ͨѶ���� */
            pCOM->time.txDly = COMMU_TIME_TX_TIMEOUT;
            
			break;			
	  	case COMMU_TX :	/* ת�������жϸ�Ϊ���� */
            if (pCOM->txCnt >= pCOM->txTotal)
            {   
                if (u == COM_4G)
				{
					pCOM->time.commIntProt = app_4g_commIntProt_time(TRUE);
				}
				else
				{
                	pCOM->time.commIntProt = COMMU_TIME_PROTECT;		/* �ӻ��� */
				}
    			pCOM->step = COMMU_RX;
                //�ڷ����ж����Ѿ���Ϊ���գ��ڸ���ͨ��ʱ��
                //����ִ�д˴�ǰ�Ѿ����������ݣ���˴˴ο��Բ�����
                COM_ReadyRece(u);			/* ��������ȷ��485�շ��������óɹ� */
            }
            else if (pCOM->time.txDly == 0)/* ��ʱ */
            {
                pCOM->commuErr = COMMU_OVERTIME;
                pCOM->step = COMMU_FAIL;
//                COM_ReadyRece(u);			/* ͳһ�ŵ�COMMU_FAIL���� */
            }
	  		break;
		default :
        case COMMU_FAIL :
            COM_err_deal(u, pCOM->txBuf[0], COM_ERR_REC);	/* ��¼ͨѶ���� */
			pCOM->step = COMMU_RX;
			COM_ReadyRece(u);
            pCOM->time.commIntProt = COMMU_TIME_PROTECT;
            pCOM->time.txDly = COMMU_TIME_TX_DLY;
			pCOM->rxCnt = 0;
			break;
	}
}

/*******************************************************************************
 �� ��: COM_MasterProg()
 �� ��: ������������ӿ�
 �� ��: u--���ں�; 
 �� ��: ��
 �� ��: ����״̬Ǩ��˵��;New485Comm
	(1) ���У������ϵ����Ϊ"����"״̬���ȴ���������������������ͨѶ������ͨѶ
 �����"������ʱ"״̬��
	(2) ������ʱ��������ʱʱ�䡢ͨѶ���ʱ�䵽�����"��������"״̬���ڴ�״̬�½�
 485оƬ��Ϊ����״̬�����������ϵĸ��š�
	(4) �������ͣ����ڴ�״̬������ͨѶ�����������ͻ�������һ�����ݱ��浽���ͼĴ�
 �������������͡�ͨѶ״̬��Ϊ"����"���ȴ�������ɡ�����ʱ����Ҫ���ֽ���ʹ�ܹرա�
	(5) ���ͣ������ķ������ڷ����ж�����ɡ������ж��н��������ݶ�������ɺ󣬽�
 485��Ϊ����״̬����ͨѶ״̬��Ϊ"����"������ǹ㲥���״̬תΪ"�㲥������ʱ"��
	(6) ���գ�һ֡������ɣ�3.5 ���ַ�ʱ��δ�յ���һ���ݻ���ջ���������9600���
 Ϊ5ms��4800���Ϊ10ms������п�ͳһ����Ϊ10ms��
 	���յ���ȷ������֡����ͨѶ״̬��Ϊ"����"��׼����һ��ͨѶ�����յ����������
 ֡������֡���ݶ����������������һ֡���ݣ�ֱ�����յ���ȷ�����ݻ�ʱ�� �����ʱ
 ����δ�յ���ȷ�����ݣ�����ΪͨѶ������Ҫ�������ԣ���״̬תΪ"������ʱ"�� ����
 ���͸ղŵ����ݡ����"ͨѶ���Դ���"������δ�յ���ȷ�����ݣ�����ΪͨѶ���ϣ� ��ͨ
 Ѷ���ϱ�־λ��ͨѶ״̬תΪ"����"��׼����һ��ͨѶ��
	(7) �㲥������ʱ���㲥ʱ��������ɺ������ʱһ����ʱ������ת��"����"״̬��
 �Ա�ӻ��ܹ���������Ӧ�Ĳ�����
*******************************************************************************/
void COM_MasterProg(U08 u)
{
	UART *pCOM;	
	BOOL rxOk=FALSE;		/* COM-V100A1-0 */

	pCOM = &Uart[u];
	switch (pCOM->step)
	{		
		case COMMU_IDLE :	/* ���Ͷ����д����ͽڵ㣬ת������ʱ */		
			if (COM_QSendNodeData(u)) 
			{	
				pCOM->failCnt = 0;
				pCOM->step = COMMU_DELAY;
				pCOM->time.txDly = COMMU_TIME_TX_DLY;
			}
			break;

		case COMMU_DELAY :	/* ��ʱʱ�䵽��ת�������� */
			COM_TrasDly(u);
			if ((pCOM->time.interval==0) && (pCOM->time.txDly==0))
			{
				pCOM->step = COMMU_START;
			}
			else
			{
				break;
			}
		
		case COMMU_START:	/* д���ͻ������Ĵ������������� */
			pCOM->txCnt = 0;
			pCOM->rxCnt = 0;
			COM_WrTrasBufReg(u, pCOM->txBuf[0]);
			pCOM->step = COMMU_TX;
			pCOM->rxFrameType = RX_FRAME_NO;
            pCOM->time.txDly = COMMU_TIME_TX_TIMEOUT;
			break;
            
		case COMMU_TX :	/* ת�������жϸ�Ϊ���� */
			if (pCOM->txCnt >= pCOM->txTotal)
            {   
            	if (pCOM->txBuf[0]!=ADDR_BROADCAST && pCOM->txBuf[0]!=ADDR_BRO_FBUS)
            	{
                    pCOM->time.rxOvertime = COMMU_TIME_RX_OVER;	/* ������ */
        			pCOM->step = COMMU_RX;
                    COM_ReadyRece(u);
                }
                else
                {
                    pCOM->time.broadcastDly = COMMU_TIME_BROADCAST_DLY;
			        pCOM->step = COMMU_BROADCAST;
                }

            }
            else if (pCOM->time.txDly == 0)/* ��ʱ������ */
            {
				pCOM->step = COMMU_DELAY;
				pCOM->time.interval = COMMU_TIME_INTERVAL;
				pCOM->time.txDly = COMMU_TIME_TX_DLY;					
			}
			break;
			
		case COMMU_RX :	/* ���ݽ���֡��ʵ��������ֱ�ת������ͬ״̬ */
			/* һ֡�������:����֡ʱ����δ�յ���һ���ݻ���ջ������� */
			if (((pCOM->rxCnt > 0) && (pCOM->time.rxFrame == 0)) 
				|| (pCOM->rxCnt >= COMMU_RX_SIZE)) 
			{
				COM_DisableRece(u);			/* �������֡ʱ�رս���ʹ�� */
				if (COM_QAcceptNodeData(u))	/* ��ȷ֡ʱ��ת���� */
				{
					rxOk = TRUE;			/* COM-V100A1-0 */
					COM_QNextNode(u);		/* ָ���Ͷ�����һ���ڵ� */

					pCOM->time.led = COMMU_TIME_LED;
					pCOM->step = COMMU_IDLE;
					pCOM->time.interval = COMMU_TIME_INTERVAL;			
					COM_Set485Tras(u);		/* ������485Ϊ���ͣ������������ϵĸ��� */
					
					/* �ӻ��쳣��Ӧ */
					if (pCOM->rxFrameType == RX_FRAME_EXCEP_RSP)
					{
						pCOM->commuErr = COMMU_EXCEP_RSP;
					}
					else
					{
						pCOM->commuErr = COMMU_NO_ERR;
					}
                    /* ���ͨѶ���� */
					COM_err_deal(u, pCOM->txBuf[0], COM_ERR_CLR);
					
				//	break;	/* COM-V100A1-0 */
				}
				else		/* ����֡ʱ��׼��������һ֡������ռ����� */
				{			
					COM_ReadyRece(u);
					pCOM->rxCnt = 0;
				}
			}

			if (rxOk)
			{
				break;
			}			

			/* ���ճ�ʱʱ�䵽����δ�յ���Ч����֡��Ϊһ��ͨѶʧ�� */
			if (pCOM->time.rxOvertime == 0) 
			{	
				pCOM->pRecord->fail++;
				if (pCOM->rxFrameType == RX_FRAME_NO)
				{
					pCOM->pRecord->overtime++;
				}			

                /* �ѵ�ʧ�����Դ�����ת���� */
				if (++pCOM->failCnt >= (u==COM_INVT ? COMMU_FAIL_NUM_INVT : COMMU_FAIL_NUM)
                || (COM_err_deal(u, pCOM->txBuf[0], COM_ERR_GET) /* �ѹ��ϵĴӻ������ԣ�����Լ����� */
				    && u==COM_FCU_FBUS
				    )
                )       
				{			
					pCOM->pRecord->alarm++;
					pCOM->step = COMMU_FAIL;
					pCOM->time.interval = COMMU_TIME_INTERVAL;

					/* �����һ��ͨѶʱ������֡����������ͨѶ�������� */
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
//							COM_QNextNode(u);		/* ָ���Ͷ�����һ���ڵ� */
							break;
						default:
							break;
					}
				}
				else		/* δ��ʧ�����Դ�����ͨѶ����ת��ʱ */
				{
					pCOM->step = COMMU_DELAY;
					pCOM->time.interval = COMMU_TIME_INTERVAL;
					pCOM->time.txDly = COMMU_TIME_TX_DLY;					
				}
			}
			break;

		case COMMU_BROADCAST :				/* �㲥������ʱ����ת���� */
			if (pCOM->time.broadcastDly == 0)
			{
				pCOM->step = COMMU_IDLE;
				pCOM->pRecord->broadcast++;
				COM_QNextNode(u);	/* ָ���Ͷ�����һ���ڵ� */
			}	
			break;
			
		case COMMU_FAIL :	/* �����Զ���λ��ת���� */
		    COM_QNextNode(u);	/* ָ���Ͷ�����һ���ڵ� */
            COM_err_deal(u, pCOM->txBuf[0], COM_ERR_REC);	/* ��¼ͨѶ���� */
			pCOM->step = COMMU_IDLE;
			break;
			
		default :
			pCOM->step = COMMU_IDLE;
			break;
	}
}



/**
 * ���� : COM_HMIMasterProg()
 * ���� : �ٴ�����������ӿ�
 *	      ��HMI��ʾ��ͨѶ��������ͨѶ���ݲ���׼����˺����������⴦��
 *
 * ���� : @u ��ʾ���ں�; 
 * ���� : --
 *
 * ��ע : XMacro 2017/01/16--1546
 */
void COM_HMIMasterProg(U08 u)
{
	UART *pCOM;	
	BOOL rxOk=FALSE;		/* COM-V100A1-0 */

	pCOM = &Uart[u];

	switch (pCOM->step)
	{		
		case COMMU_IDLE :	/* ���Ͷ����д����ͽڵ㣬ת������ʱ */		
			if (hmi_trans(pCOM))
			{	
				pCOM->failCnt = 0;
				pCOM->step = COMMU_DELAY;
				pCOM->time.txDly = COMMU_TIME_TX_DLY;
			}
			break;

		case COMMU_DELAY :	/* ��ʱʱ�䵽��ת�������� */
			COM_TrasDly(u);
			if ((pCOM->time.interval==0) && (pCOM->time.txDly==0))
			{
				pCOM->step = COMMU_START;
			}
			else
			{
				break;
			}
		
		case COMMU_START:	/* д���ͻ������Ĵ������������� */
			pCOM->txCnt = 0;
			pCOM->rxCnt = 0;
			COM_WrTrasBufReg(u, pCOM->txBuf[0]);
			pCOM->step = COMMU_TX;
			pCOM->rxFrameType = RX_FRAME_NO;
            pCOM->time.txDly = COMMU_TIME_TX_TIMEOUT;
			break;

		case COMMU_TX :	/* ת�������жϸ�Ϊ���� */
			if (pCOM->txCnt >= pCOM->txTotal)
            {   
            	if (pCOM->txBuf[0] != ADDR_BROADCAST)
            	{
                    pCOM->time.rxOvertime = COMMU_TIME_RX_OVER;	/* ������ */
        			pCOM->step = COMMU_RX;
                    COM_ReadyRece(u);
                }
                else
                {
                    pCOM->time.broadcastDly = COMMU_TIME_BROADCAST_DLY;
			        pCOM->step = COMMU_BROADCAST;
                }

            }
            else if (pCOM->time.txDly == 0)/* ��ʱ������ */
            {
				pCOM->step = COMMU_DELAY;
				pCOM->time.interval = COMMU_TIME_INTERVAL;
				pCOM->time.txDly = COMMU_TIME_TX_DLY;					
			}
			break;
			
		case COMMU_RX :	/* ���ݽ���֡��ʵ��������ֱ�ת������ͬ״̬ */
			/* һ֡�������:����֡ʱ����δ�յ���һ���ݻ���ջ������� */
			if (((pCOM->rxCnt > 0) && (pCOM->time.rxFrame == 0)) 
				|| (pCOM->rxCnt >= COMMU_RX_SIZE)) 
			{
				COM_DisableRece(u);			/* �������֡ʱ�رս���ʹ�� */

				if (hmi_rece(pCOM))	/* ��ȷ֡ʱ��ת���� */
				{
					rxOk = TRUE;			/* COM-V100A1-0 */

					pCOM->time.led = COMMU_TIME_LED;
					pCOM->step = COMMU_IDLE;
					pCOM->time.interval = COMMU_TIME_INTERVAL;			
					COM_Set485Tras(u);		/* ������485Ϊ���ͣ������������ϵĸ��� */

					/* �ӻ��쳣��Ӧ */
					if (pCOM->rxFrameType == RX_FRAME_EXCEP_RSP)
					{
						pCOM->commuErr = COMMU_EXCEP_RSP;
					}
					else
					{
						pCOM->commuErr = COMMU_NO_ERR;
					}
                    COM_err_deal(u, hmi_t_num, COM_ERR_CLR);
					_CNT(SH_hmi_commu_succ);	//ע��:ͨѶ�ɹ�����Ҫ�ۼӳɹ���������ᱻ��Ϊ��ͨѶʧ�ܶ��л�������
				//	break;	/* COM-V100A1-0 */
				}
				else		/* ����֡ʱ��׼��������һ֡������ռ����� */
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

			/* ���ճ�ʱʱ�䵽����δ�յ���Ч����֡��Ϊһ��ͨѶʧ�� */
			if (pCOM->time.rxOvertime == 0) 
			{	
				pCOM->pRecord->fail++;
				if (pCOM->rxFrameType == RX_FRAME_NO)
				{
					pCOM->pRecord->overtime++;
				}			
				
				if (++pCOM->failCnt >= COMMU_FAIL_NUM)		/* �ѵ�ʧ�����Դ�����ת���� */
				{			
					pCOM->pRecord->alarm++;
					pCOM->step = COMMU_FAIL;
					pCOM->time.interval = COMMU_TIME_INTERVAL;

					/* �����һ��ͨѶʱ������֡����������ͨѶ�������� */
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
				else		/* δ��ʧ�����Դ�����ͨѶ����ת��ʱ */
				{
					pCOM->step = COMMU_DELAY;
					pCOM->time.interval = COMMU_TIME_INTERVAL;
					pCOM->time.txDly = COMMU_TIME_TX_DLY;					
				}
			}
			break;

		case COMMU_BROADCAST :				/* �㲥������ʱ����ת���� */
			if (pCOM->time.broadcastDly == 0)
			{
				pCOM->step = COMMU_IDLE;
				pCOM->pRecord->broadcast++;
			}	
			break;
			
		case COMMU_FAIL :	/* �����Զ���λ��ת���� */
			pCOM->step = COMMU_IDLE;
			COM_err_deal(u, hmi_t_num, COM_ERR_REC);	/* ��¼ͨѶ���� */
			break;
			
		default :
			pCOM->step = COMMU_IDLE;
			break;
	}
}

/*******************************************************************************
	����Ϊ����㺯�������ڲ���׼Э�����
*******************************************************************************/

/*******************************************************************************
 �� ��: COM_QEmpty()
 �� ��: �жϷ��Ͷ����Ƿ�Ϊ��
 �� ��: u--���ں�; 
 �� ��: TRUE/FALSE--���Ͷ���Ϊ��/�ǿ�
 �� ��: ��
*******************************************************************************/
BOOL COM_QEmpty(U08 u)		
{	
	return  (Queue[u].cnt == 0);			
}

/*******************************************************************************
 �� ��: COM_QFull()
 �� ��: �жϷ��Ͷ����Ƿ�Ϊ��
 �� ��: u--���ں�; 
 �� ��: TRUE/FALSE--���Ͷ���Ϊ��/����
 �� ��: ��
*******************************************************************************/
BOOL COM_QFull(U08 u)		
{	
//	return  (Queue[u].cnt == MAX_NODE_NUM);	
	return  (Queue[u].cnt >= MAX_NODE_NUM);	/* COM-V100A0-1 */
}

/*******************************************************************************
 �� ��: COM_QGetNodeCnt()
 �� ��: ��ȡ���Ͷ�����Ч����
 �� ��: u--���ں�; 
 �� ��: ���Ͷ�����Ч����
 �� ��: ��
*******************************************************************************/
U08 COM_QGetNodeCnt(U08 u)				
{	
	return  Queue[u].cnt;					
}

/*******************************************************************************
 �� ��: COM_QInit()
 �� ��: ��ʼ�����Ͷ���
 �� ��: u--���ں�; 
 �� ��: ��
 �� ��: ��
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
 �� ��: COM_QNextNode()
 �� ��: ָ���Ͷ�����һ���ڵ�
 �� ��: u--���ں�; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void COM_QNextNode(U08 u)
{
	U08 newHead;
	SENDQUEUE *pq;

	pq = &Queue[u];

	pq->node[pq->head].sta = NODE_STA_FINESH; /* ��ǰ�ڵ㷢����� */
	
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
		pq->node[pq->head].sta = NODE_STA_SEND; /* ��һ�ڵ�׼������ */
	}
	pq->cnt--;
}

/*******************************************************************************
 �� ��: COM_QInsertNode()
 �� ��: ������в���һ���ڵ�
 �� ��: u--���ں�; 
		prio--���ȼ� Q_PRIO_LOW/Q_PRIO_HIGH; 
		pComMessage--ͨѶ�ڵ�ָ��; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL COM_QInsertNode(U08 u, U08 prio, const QNODE *pNode)
{
	U08 no;
	SENDQUEUE *pq;

	pq = &Queue[u];

	if (COM_QEmpty(u))		/* ���п� */
	{
		no = pq->end;
	}
	else if (COM_QFull(u))	/* ������ */
	{
		if (prio == Q_PRIO_HIGH)			/* �����ȼ� */
		{		
			if (pq->insertPos == pq->end)  
			{
				return FALSE;				/* ȫ�����Ǹ����ȼ������� */
			}

			no = pq->node[pq->insertPos].next;
			pq->insertPos = no;
		}
		else 
		{					/* �����ȼ� */
			return FALSE;
		}
	}
	else
	{
		if (prio == Q_PRIO_HIGH)			/* �����ȼ� */
		{ 		
			if (pq->insertPos == pq->end)	/* ������ȫ�����Ǹ����ȼ��� */
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
		{					/* �����ȼ� */
			no = pq->node[pq->end].next;
			pq->end = no;
		}
	}

	pq->node[no].driverType = pNode->driverType;
	pq->node[no].data = pNode->data;
	pq->node[no].sta = NODE_STA_CREATE;
/*==============================================================================
COM-V100A0-0
1.���������жϣ����в���ʱ�����нڵ�������ż�1
2.��ǰ��û�������жϵģ��������ʱ�����:
  ���������Ǹ����ȼ�����(������ĳ����֧����)�������Ƚڵ�Ḳ�ǵ����ȼ��ڵ㣬
  �������ж�ʱ�����нڵ���������1�����Ѵ������Ľڵ���(MAX_NODE_NUM)��
  �´��ٲ���һ�������ȼ����COM_QFull(u)�᷵��FALSE��
  ���ջᵼ�²���Ľڵ����ݳ�������Ľڵ����鷶Χ�������⸲�ǽڵ�������涨��ı�����
==============================================================================*/
//	pq->cnt++;	
	if (!COM_QFull(u))
	{
		pq->cnt++;
	}
	
	return TRUE;
}

/*******************************************************************************
	����Ϊ����㺯��(����modbus.c�еĺ���)����ͨ�Ŷ������
*******************************************************************************/

/*******************************************************************************
 �� ��: COM_QAcceptNodeData()
 �� ��: �ӽ��ջ��������սڵ�����
 �� ��: u--���ں�; 
 �� ��: TRUE/FALSE--����������ȷ/����
 �� ��: ��
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
 �� ��: COM_QSendNodeData()
 �� ��: ���ͽڵ����������ͻ�����
 �� ��: u--���ں�; 
 �� ��: TRUE/FALSE--�÷��ͻ������ɹ�/ʧ��
 �� ��: ��
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
    
    /* ����CANͨѶʱֻ�����豸��ַ��CRC���������׼MODBUS�������ݽṹ���������豸���ͣ�
    	���ô��ں����֣��Ժ���Э��ƫ��ϴ�ʱ���á��豸���͡����� */
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

    /* ����ʧ��ʱ�����ýڵ㣬ָ���¸��ڵ� */
    COM_QNextNode(u);
    
    return FALSE;
}

/*******************************************************************************
	����ΪӦ�ò㺯�����ṩģ��ӿ�
*******************************************************************************/
/*******************************************************************************
 �� ��: COM_WriteWords()
 �� ��: ��ָ������ͨѶ���ڲ�������:д������ּĴ���(�ɶ�д��)
 �� ��: u--���ں�; 
        pcb--���;
		startAddr--��ʼ��ַ; 
		length--����; 
		pData--����ָ��; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: д�����ø����ȼ�
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
 �� ��: COM_WriteOneWord()
 �� ��: ��ָ������ͨѶ���ڲ�������:д�������ּĴ���(�ɶ�д��)
 �� ��: u--���ں�; 
         pcb--���;
		startAddr--��ʼ��ַ; 
		val--дֵ; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: д�����ø����ȼ�
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
 �� ��: COM_WriteOneBit()
 �� ��: ��ָ������ͨѶ���ڲ�������:д������Ȧ(�ɶ�дλ)
 �� ��: u--���ں�;
        pcb--���;
		startAddr--��ʼ��ַ; 
		val--дֵ; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: д�����ø����ȼ�
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
 �� ��: COM_ReadRWwords()
 �� ��: ��ָ������ͨѶ���ڲ�������:�����ּĴ���(�ɶ�д��)
 �� ��: u--���ں�; 
         pcb--���;
		startAddr--��ʼ��ַ; 
		length--����; 
		pData--����ָ��; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: 
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
 �� ��: COM_ReadROwords()
 �� ��: ��ָ������ͨѶ���ڲ�������:������Ĵ���(ֻ����)
 �� ��: u--���ں�;
        pcb--���;
		startAddr--��ʼ��ַ; 
		length--����; 
		pData--����ָ��; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: 
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
 �� ��: COM_Config()
 �� ��: ��������
 �� ��: u--���; 
		baud--������; 
		dataType--���ݸ�ʽ; 
 �� ��: ��
 �� ��: ��ӿں���
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

