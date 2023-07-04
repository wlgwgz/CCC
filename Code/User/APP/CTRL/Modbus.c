/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: Modbus.c
 �ļ�����: 
 ������  : 
 ��������: 
*******************************************************************************/
#define MODBUS_GLOBAL
#include "Includes.h"
//static U16 debug_com[50];

/* �����ط��ʲ�����������������Ա��ط�������M�� */
#define RW_BIT_M_SIZE			(M_CORE_SIZE)

#define MODBUS_RO_BIT_I_START	0
#define MODBUS_RO_BIT_I_END		(U16)(MODBUS_RO_BIT_I_START+I_SIZE*16-1)
#define MODBUS_RW_BIT_Q_START	0
#define MODBUS_RW_BIT_Q_END		(U16)(MODBUS_RW_BIT_Q_START+Q_SIZE*16-1)
#define MODBUS_RW_BIT_M_START	(U16)(MODBUS_RW_BIT_Q_END+1)
#define MODBUS_RW_BIT_M_END		(U16)(MODBUS_RW_BIT_M_START+RW_BIT_M_SIZE*16-1)

#define MODBUS_RO_WORD_I_START	0
#define MODBUS_RO_WORD_I_END	(U16)(MODBUS_RO_WORD_I_START+I_SIZE-1)
#define MODBUS_RW_WORD_Q_START	0
#define MODBUS_RW_WORD_Q_END	(U16)(MODBUS_RW_WORD_Q_START+Q_SIZE-1)
#define MODBUS_RW_WORD_M_START	(U16)(MODBUS_RW_WORD_Q_END+1)
#define MODBUS_RW_WORD_M_END	(U16)(MODBUS_RW_WORD_M_START+M_SIZE-1)

/*******************************************************************************
�����֡�����Ĵ�����Ӧ ��ַ(1)+������(1)+�ֽ���(1)+��ֵ(n1)+crc(2)
����Ȧ����ɢ��������Ӧ ��ַ(1)+������(1)+�ֽ���(1)+��ֵ(n2)+crc(2)
	n1=(MB_SIZE-5)/2,	n2=16*n1[��֤���ߵķ��ʷ�Χһ��] 
ע: n2��һ������(MB_SIZE-5)*8 [MB_SIZEΪż��ʱ��������]
��: MB��������СΪ256����WORDS���������Ϊ125����BITS���������Ϊ2000(����2008)

д���ּĴ������� ��ַ(1)+������(1)+��ʼ��ַ(2)+����(2)+�ֽ���(1)+дֵ(n3)+crc(2)
д��Ȧ���� 		 ��ַ(1)+������(1)+��ʼ��ַ(2)+����(2)+�ֽ���(1)+дֵ(n4)+crc(2)
	n3=(MB_SIZE-9)/2,	n4=16*n3[��֤���ߵķ��ʷ�Χһ��] 
ע: n4��һ������(MB_SIZE-9)*8 [MB_SIZEΪż��ʱ��������]
��: MB��������СΪ256��дWORDS���������Ϊ123��дBITS���������Ϊ1968(����1976)

���ջ�·�������� ��ַ(1)+������(1)+��������(2)+����(2)+�ֽ���(1)+дֵ(n3)+crc(2)
	n3=(MB_SIZE-9)/2
*******************************************************************************/


/*******************************************************************************
���ٵ�֡�ֽ���:��ַ ������ У���(2���ֽ�)
�쳣��Ӧ�ֽ���:��ַ ������(>=80) �쳣�� У���(2���ֽ�)
*******************************************************************************/
#define LEAST_FRAME_BYTES	4				/* ���ٵ�֡�ֽ��� */
#define EXCEP_RSP_BYTES		5				/* �쳣��Ӧ�ֽ��� */

#define MK_EXCEP_FUNCODE	0x80			/* �쳣������������ */

/****************************************************************
������com_slv_datanum_err
���ܣ����ݸ�������
�����������롣
���أ�����ֵ(����/���ݸ�������)
������/��
*****************************************************************/
U08 com_slv_datanum_err(U08 *RxBuf)
{
	U08 err_result = MB_NO_ERR;
	U08 func = RxBuf[1];		/* ������ */
	U16 data_num = 0;
	U16 byte_count = 0;
	
	switch(func)
	{
		case MB_RD_HOLDING_REGISTERS :	/* 0x03 ���ɶ�д�� */
//	    	break;			
	    case MB_RD_INPUT_REGISTERS :	/* 0x04 ��ֻ���� */
	        data_num = MAKEWORD(RxBuf[4], RxBuf[5]);	/* ���ݸ��� */
			if ((data_num < 1) || (data_num > MB_MAX_QUANTITY_RD_WORDS))	
        	{
        		err_result = MB_ILLEGAL_DATA_VALUE;
//				if (debug[0]<4)	debug[0]=4;	
        	}
			break;
			
		case MB_WR_SINGLE_COIL:			/* 0x05 д����λ */
			data_num = MAKEWORD(RxBuf[4], RxBuf[5]);
			if (data_num!=0x0000 && data_num!=0xff00)	/* ����ֵ���� */
			{
    			err_result = MB_ILLEGAL_DATA_VALUE;
    		}
			break;
			
		case MB_WR_MULTIPLE_COILS:		/* 0x0f д���λ */
			data_num = MAKEWORD(RxBuf[4], RxBuf[5]);	/* ���ݸ��� */
			byte_count = RxBuf[6];						/* �ֽ��� */
			
			if ((data_num<1) || (data_num>MB_MAX_QUANTITY_WR_BITS)
				|| (byte_count != (data_num%8==0 ? data_num/8 : data_num/8+1)))	/* ���ݸ������� */
        	{
        		err_result = MB_ILLEGAL_DATA_VALUE;
        	}
			break;
		
		case MB_WR_SINGLE_REGISTER:		/* 0x06 д������ */
			
			break;
		
		case MB_WR_MULTIPLE_REGISTERS :	/* 0x10 д����� */	
			data_num = MAKEWORD(RxBuf[4], RxBuf[5]);	/* ���ݸ��� */
			byte_count = RxBuf[6];						/* �ֽ��� */
			
			if ((data_num<1) || (data_num>MB_MAX_QUANTITY_WR_WORDS)
        		|| (byte_count != data_num*2))
        	{
        		err_result = MB_ILLEGAL_DATA_VALUE;
        	}
	    	break;
	    	
	    default:
	    	break;
		
	}
	
	return err_result;
}

/*******************************************************************************
 �� ��: SYS_CopyMemForModbusType()
 �� ��: �洢�ռ临�ƣ���Դ����������Modbus����(��8λ��ǰ����8λ�ں�)
 �� ��: pDst--Ŀ������ָ��; 
		pSrc--Դ����ָ��; 
		size--�ֽڴ�С(����Ϊż��); 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void SYS_CopyMemForModbusType(U08 *pDst, const U08 *pSrc, U16 size)
{
	if (size%2 == 0)
	{
		while (size != 0)
		{	
			*pDst++ = (*(W16 *)pSrc).byte.high;	
			*pDst++ = (*(W16 *)pSrc).byte.low;	
			pSrc += 2;
			size -= 2;
		}
	}
}
/*******************************************************************************
 �� ��: PDU_SlaveDealCoilAddr()
 �� ��: ������Ȧ�ĵ�ַ
 �� ��: pBeginAddr--��ʼ��ַָ��(��ʱ�������ʼ��ַ); 
		endAddr--������ַ; 
 �� ��: ��ַ
 �� ��: ��
*******************************************************************************/
//static U16 * PDU_SlaveDealCoilAddr(U16 *pBeginAddr,U16 endAddr)
//{
//	/* ����pBeginAddrδ��ʹ�õı��뾯����Ϣ */
//#if (MODBUS_RW_BIT_Q_START == 0)
//	if (*pBeginAddr > 1)
//	{
//		ASM_NOP();
//	}
//#endif	
//
//#ifndef ACCESS_M_THROUGH_Q
//	/* �ֿ�����QM */
//	if (
//#if (MODBUS_RW_BIT_Q_START != 0)			
//		(*pBeginAddr >= MODBUS_RW_BIT_Q_START) &&
//#endif
//		(endAddr <= MODBUS_RW_BIT_Q_END))
//	{
//#if (MODBUS_RW_BIT_Q_START != 0)			
//		*pBeginAddr -= MODBUS_RW_BIT_Q_START;
//#endif		
//		return (U16 *)QTbl;
//	}
//	else if (
//		(*pBeginAddr >= MODBUS_RW_BIT_M_START) &&
//		(endAddr <= MODBUS_RW_BIT_M_END))
//	{
//		*pBeginAddr -= MODBUS_RW_BIT_M_START;
//		return (U16 *)MTbl;
//	}
//	else
//	{
//		/* ��ַ���� */
//		return NULL;
//	}	
//#else
//	/* һ�����QM(ͨ��Q����M) */
//	if (
//#if (MODBUS_RW_BIT_Q_START != 0)			
//		(*pBeginAddr >= MODBUS_RW_BIT_Q_START) &&
//#endif
//		(endAddr <= MODBUS_RW_BIT_M_END))
//	{
//#if (MODBUS_RW_BIT_Q_START != 0)			
//		*pBeginAddr -= MODBUS_RW_BIT_Q_START;
//#endif		
//		return (U16 *)QTbl;
//	}
//	else
//	{
//		/* ��ַ���� */
//		return NULL;
//	}
//#endif /* ACCESS_M_THROUGH_Q */
//}

/*******************************************************************************
 �� ��: PDU_SlaveDealHRegAddr()
 �� ��: �����ּĴ����ĵ�ַ
 �� ��: beginAddr--��ʼ��ַ; 
		endAddr--������ַ; 
 �� ��: ��ַ
 �� ��: Q����M�������������ģ���M���ռ��ѵ�64K(Ĭ�ϵĵ�ַ���ʲ��ܳ���64K)
*******************************************************************************/
//static U16 * PDU_SlaveDealHRegAddr(U16 beginAddr,U16 endAddr)
//{
//#ifndef ACCESS_M_THROUGH_Q
//	/* �ֿ�����QM */
//	if (
//#if (MODBUS_RW_WORD_Q_START != 0)			
//		(beginAddr >= MODBUS_RW_WORD_Q_START) &&
//#endif
//		(endAddr <= MODBUS_RW_WORD_Q_END))
//	{	
//		return (U16 *)&QTbl[beginAddr-MODBUS_RW_WORD_Q_START].word;
//	}
//	else if (
//		(beginAddr >= MODBUS_RW_WORD_M_START) &&
//		(endAddr <= MODBUS_RW_WORD_M_END))
//	{
//		return (U16 *)&MTbl[beginAddr-MODBUS_RW_WORD_M_START].word;
//	}
//	else
//	{
//		/* ��ַ���� */
//		return NULL;
//	}
//#else
//	/* һ�����QM(ͨ��Q����M) */
//	if (
//#if (MODBUS_RW_WORD_Q_START != 0)			
//		(beginAddr >= MODBUS_RW_WORD_Q_START) &&
//#endif
//		(endAddr <= MODBUS_RW_WORD_M_END))
//	{	
//		return (U16 *)&QTbl[beginAddr-MODBUS_RW_WORD_Q_START].word;
//	}
//	else
//	{
//		/* ��ַ���� */
//		return NULL;
//	}
//#endif /* ACCESS_M_THROUGH_Q */
//}

/*******************************************************************************
 �� ��: PDU_SlaveDealRdCoils()
 �� ��: �ӻ�����ModBus����Ȧ����
 �� ��: pduRxBuf--PDU���ջ�����; 
		pduTxBuf--PDU���ͻ�����; 
		pPduTxTotal--PDU�跢�͵��ֽ�����; 
 �� ��: �쳣��־������⵽�����򷵻ؾ�����쳣�룬���򷵻����쳣��־
 �� ��: �жϸ�������ַ�Ƿ���ȷ���������򷵻��쳣��(�ϲ��ٴ����쳣��ӦPDU)
 		����ȷ���������������ӦPDU�����ͻ�����
		����Ӧ��Э�� 
		01 00 00 00 20  	��00 00 ��ʼ��ȡ32��λ 
		01 04 55 AA 22 66   �ӵ͵��ߣ��ֱ�Ϊ55 AA 22 66 
*******************************************************************************/
//static U08 PDU_SlaveDealRdCoils(const U08 *pduRxBuf, U08 *pduTxBuf, U16 *pPduTxTotal)
//{
//	U16 quantity;			/* ���ݸ��� */
//	U16 beginAddr;			/* ��ʼ��ַ */
//	U16 endAddr;			/* ������ַ */
//	U16 *pData;				/* ����ָ�� */
//	U16 i;
//
//   	beginAddr = MAKEWORD(pduRxBuf[1], pduRxBuf[2]);
//   	quantity = MAKEWORD(pduRxBuf[3], pduRxBuf[4]);
//   	endAddr = beginAddr+quantity-1;
//
//	/* �����ж� */
//	if ((quantity<1) || (quantity>MB_MAX_QUANTITY_RD_BITS))	
//	{
//		return MB_ILLEGAL_DATA_VALUE;
//	}
//
//	/* ��ַ�ж������ */
//	pData = PDU_SlaveDealCoilAddr(&beginAddr, endAddr);
//	if (pData == NULL)
//	{
//		return MB_ILLEGAL_DATA_ADDR;
//	}
//
//	/* ������ */	
//	pduTxBuf[0] = pduRxBuf[0];	/* ������ */
//	
//	pduTxBuf[1] = ((quantity%8)== 0) ? 
//						(U08)(quantity/8) : 
//						(U08)((quantity/8)+1);
//	*pPduTxTotal = 2;
//
//	for (i=0; i<pduTxBuf[1]; i++)
//	{
//		pduTxBuf[*pPduTxTotal + i] = 0;
//	}
//	
//	for (i=0; i<quantity; i++)
//	{
//		pduTxBuf[*pPduTxTotal + i/8] >>= 1;
//		
//		if (GETBIT(pData[(beginAddr+i)/16], (beginAddr+i)%16))
//		{
//			pduTxBuf[*pPduTxTotal + i/8] |= 0x80;
//		}
//	}
//
//	*pPduTxTotal += pduTxBuf[1];
//	
//	if ((quantity % 8) != 0)
//	{
//		quantity = 8- (quantity%8);
//		pduTxBuf[*pPduTxTotal-1] >>= quantity;
//	}	
//
//	return MB_NO_ERR;  	
//}

/*******************************************************************************
 �� ��: PDU_SlaveDealRdDInputs()
 �� ��: �ӻ�����ModBus����ɢ����������
 �� ��: pduRxBuf--PDU���ջ�����; 
		pduTxBuf--PDU���ͻ�����; 
		pPduTxTotal--PDU�跢�͵��ֽ�����; 
 �� ��: �쳣��־������⵽�����򷵻ؾ�����쳣�룬���򷵻����쳣��־
 �� ��: �жϸ�������ַ�Ƿ���ȷ���������򷵻��쳣��(�ϲ��ٴ����쳣��ӦPDU)
 		����ȷ���������������ӦPDU�����ͻ�����
		����Ӧ��Э�� 
		�ο�����Ȧ
*******************************************************************************/
//U08 PDU_SlaveDealRdDInputs(const U08 *pduRxBuf, U08 *pduTxBuf, U16 *pPduTxTotal)
//{
//	U16 quantity;			/* ���ݸ��� */
//	U16 beginAddr;			/* ��ʼ��ַ */
//	U16 endAddr;			/* ������ַ */
//	U16 i;
//
//   	beginAddr = MAKEWORD(pduRxBuf[1], pduRxBuf[2]);
//   	quantity = MAKEWORD(pduRxBuf[3], pduRxBuf[4]);
//   	endAddr = beginAddr+quantity-1;
//
//	/* �����ж� */
//	if ((quantity<1) || (quantity>MB_MAX_QUANTITY_RD_BITS))
//	{
//		return MB_ILLEGAL_DATA_VALUE;
//	}
//	
//	/* ��ַ�ж� */
//	if (
//#if (MODBUS_RO_BIT_I_START != 0)
//			(beginAddr < MODBUS_RO_BIT_I_START) ||
//#endif
//			(endAddr > MODBUS_RO_BIT_I_END))
//	{
//		return MB_ILLEGAL_DATA_ADDR;
//	}
//	
//	/* ������ */	
//	pduTxBuf[0] = pduRxBuf[0];	/* ������ */
//	
//	pduTxBuf[1] = ((quantity%8) == 0) ? 
//						(U08)(quantity/8) : 
//						(U08)((quantity/8)+1);
//	*pPduTxTotal = 2;
//
//	for (i=0; i<pduTxBuf[1]; i++)
//	{
//		pduTxBuf[*pPduTxTotal + i] = 0;
//	}
//	
//	for (i=0; i<quantity; i++)
//	{
//		pduTxBuf[*pPduTxTotal + i/8] >>= 1;
//		
//		if (GETBIT(ITbl[(beginAddr+i-MODBUS_RO_BIT_I_START)/16].word,
//				(beginAddr+i-MODBUS_RO_BIT_I_START)%16))
//		{
//			pduTxBuf[*pPduTxTotal + i/8] |= 0x80;
//		}
//	}
//
//	*pPduTxTotal += pduTxBuf[1];
//	
//	if ((quantity % 8) != 0)
//	{
//		quantity = 8 - (quantity%8);
//		pduTxBuf[*pPduTxTotal-1] >>= quantity;
//	}			
//
//	return MB_NO_ERR;
//}

/*******************************************************************************
 �� ��: PDU_SlaveDealRdHRegs()
 �� ��: �ӻ�����ModBus�����ּĴ�������
 �� ��: pduRxBuf--PDU���ջ�����; 
		pduTxBuf--PDU���ͻ�����; 
		pPduTxTotal--PDU�跢�͵��ֽ�����; 
 �� ��: �쳣��־������⵽�����򷵻ؾ�����쳣�룬���򷵻����쳣��־
 �� ��: �жϸ�������ַ�Ƿ���ȷ���������򷵻��쳣��(�ϲ��ٴ����쳣��ӦPDU)
 		����ȷ���������������ӦPDU�����ͻ�����
		����Ӧ��Э�� 
		03 00 00 00 04  				��00��ʼ���ĸ���
		03 08 00 55 11 22 33 44 00 66   8���ֽڣ���λ��ǰ 
*******************************************************************************/
//static U08 PDU_SlaveDealRdHRegs(const U08 *pduRxBuf, U08 *pduTxBuf, U16 *pPduTxTotal)
//{
//	U16 quantity;			/* ���ݸ��� */
//	U16 beginAddr;			/* ��ʼ��ַ */
//	U16 endAddr;			/* ������ַ */
//	U16 *pData;				/* ����ָ�� */
//
//   	beginAddr = MAKEWORD(pduRxBuf[1], pduRxBuf[2]);
//   	quantity = MAKEWORD(pduRxBuf[3], pduRxBuf[4]);
//   	endAddr = beginAddr+quantity-1;
//
//	/* �����ж� */
//	if ((quantity<1) || (quantity>MB_MAX_QUANTITY_RD_WORDS))	
//	{
//		return MB_ILLEGAL_DATA_VALUE;
//	}
//	
//	/* ��ַ�ж������ */
//	pData = PDU_SlaveDealHRegAddr(beginAddr, endAddr);
//	if (pData == NULL)
//	{
//		return MB_ILLEGAL_DATA_ADDR;
//	}
//	
//	/* ������ */	
//	pduTxBuf[0] = pduRxBuf[0];	/* ������ */
//
//	pduTxBuf[1] = (U08)(quantity << 1);	
//	*pPduTxTotal = 2;
//
//	SYS_CopyMemForModbusType(&pduTxBuf[*pPduTxTotal], (U08 *)pData, (quantity<<1));
//	*pPduTxTotal += pduTxBuf[1];
//
//	return MB_NO_ERR;  	
//}

/*******************************************************************************
 �� ��: PDU_SlaveDealRdIRegs()
 �� ��: �ӻ�����ModBus������Ĵ�������
 �� ��: pduRxBuf--PDU���ջ�����; 
		pduTxBuf--PDU���ͻ�����; 
		pPduTxTotal--PDU�跢�͵��ֽ�����; 
 �� ��: �쳣��־������⵽�����򷵻ؾ�����쳣�룬���򷵻����쳣��־
 �� ��: �жϸ�������ַ�Ƿ���ȷ���������򷵻��쳣��(�ϲ��ٴ����쳣��ӦPDU)
 		����ȷ���������������ӦPDU�����ͻ�����
		����Ӧ��Э�� 
		�ο������ּĴ���
*******************************************************************************/
//static U08 PDU_SlaveDealRdIRegs(const U08 *pduRxBuf, U08 *pduTxBuf, U16 *pPduTxTotal)
//{
//	U16 quantity;			/* ���ݸ��� */
//	U16 beginAddr;			/* ��ʼ��ַ */
//	U16 endAddr;			/* ������ַ */
//
//   	beginAddr = MAKEWORD(pduRxBuf[1], pduRxBuf[2]);
//   	quantity = MAKEWORD(pduRxBuf[3], pduRxBuf[4]);
//   	endAddr = beginAddr+quantity-1;
//
//	/* �����ж� */
//	if ((quantity<1) || (quantity>MB_MAX_QUANTITY_RD_WORDS))
//	{
//		return MB_ILLEGAL_DATA_VALUE;
//	}
//
//	/* ��ַ�ж� */
//	if (				
//#if (MODBUS_RO_WORD_I_START != 0)			
//			(beginAddr < MODBUS_RO_WORD_I_START) ||
//#endif
//			(endAddr > MODBUS_RO_WORD_I_END))
//	{
//		return MB_ILLEGAL_DATA_ADDR;
//	}
//
//	/* ������ */	
//	pduTxBuf[0] = pduRxBuf[0];	/* ������ */
//	pduTxBuf[1] = (U08)(quantity << 1);	
//	*pPduTxTotal = 2;
//
//	SYS_CopyMemForModbusType((U08 *)&pduTxBuf[*pPduTxTotal],
//		(U08 *)&ITbl[beginAddr-MODBUS_RO_WORD_I_START].word,
//		(U16)(quantity<<1));
//	
//	*pPduTxTotal += pduTxBuf[1];    	
//
//	return MB_NO_ERR;  	
//}

/*******************************************************************************
 �� ��: PDU_SlaveDealWrSCoil()
 �� ��: �ӻ�����ModBusд������Ȧ����
 �� ��: pduRxBuf--PDU���ջ�����; 
		pduTxBuf--PDU���ͻ�����; 
		pPduTxTotal--PDU�跢�͵��ֽ�����; 
 �� ��: �쳣��־������⵽�����򷵻ؾ�����쳣�룬���򷵻����쳣��־
 �� ��: �ж���ֵ����ַ�Ƿ���ȷ���������򷵻��쳣��(�ϲ��ٴ����쳣��ӦPDU)
 		����ȷ���������������ӦPDU�����ͻ�����
		����Ӧ��Э�� 
		05 00 01 FF 00   ���õ�01λΪ1
		05 00 01 FF 00   �ظ���ͬ 
*******************************************************************************/
//static U08 PDU_SlaveDealWrSCoil(const U08 *pduRxBuf, U08 *pduTxBuf, U16 *pPduTxTotal)
//{
//	U16 wrValue;			/* дֵ */
//	U16 addr;				/* ���ݵ�ַ */
//	U16 *pData;				/* ����ָ�� */
//
//   	addr = MAKEWORD(pduRxBuf[1], pduRxBuf[2]);
//   	wrValue = MAKEWORD(pduRxBuf[3], pduRxBuf[4]);
//
//	/* ��ֵ�ж�(0x0000:0\0xff000:1) */
//	if (wrValue!=0x0000 && wrValue!=0xff00)
//	{
//		return MB_ILLEGAL_DATA_VALUE;
//	}
//	
//	/* ��ַ�ж������ */
//	pData = PDU_SlaveDealCoilAddr(&addr, addr);
//	if (pData == NULL)
//	{
//		return MB_ILLEGAL_DATA_ADDR;
//	}
//
//	/* ������ */	
//	pduTxBuf[0] = pduRxBuf[0];	/* ������ */
//	/* 0x0000:0\0xff000:1 */
//	if (wrValue == 0x0000)
//	{
//		CLRBIT(pData[addr/16], addr%16);
//	}
//	else //if (wrValue == 0xff00)
//	{
//		SETBIT(pData[addr/16], addr%16);	
//	}
//
//	pduTxBuf[1] = pduRxBuf[1];
//	pduTxBuf[2] = pduRxBuf[2];
//	pduTxBuf[3] = pduRxBuf[3];
//	pduTxBuf[4] = pduRxBuf[4];
//	*pPduTxTotal = 5;			
//
//	return MB_NO_ERR;  	
//}

/*******************************************************************************
 �� ��: PDU_SlaveDealWrSReg()
 �� ��: �ӻ�����ModBusд�����Ĵ�������
 �� ��: pduRxBuf--PDU���ջ�����; 
		pduTxBuf--PDU���ͻ�����; 
		pPduTxTotal--PDU�跢�͵��ֽ�����; 
 �� ��: �쳣��־������⵽�����򷵻ؾ�����쳣�룬���򷵻����쳣��־
 �� ��: ��ֵ����ַ�Ƿ���ȷ���������򷵻��쳣��(�ϲ��ٴ����쳣��ӦPDU)
 		����ȷ���������������ӦPDU�����ͻ�����
		����Ӧ��Э�� 
		06 00 01 AA 55 	д01ΪAA 55 
		06 00 01 AA 55 	�ظ���ͬ
*******************************************************************************/
//static U08 PDU_SlaveDealWrSReg(const U08 *pduRxBuf, U08 *pduTxBuf, U16 *pPduTxTotal)
//{
//	U16 wrValue;			/* дֵ */
//	U16 addr;				/* ���ݵ�ַ */
//	U16 *pData;				/* ����ָ�� */
//
//   	addr = MAKEWORD(pduRxBuf[1], pduRxBuf[2]);
//   	wrValue = MAKEWORD(pduRxBuf[3], pduRxBuf[4]);
//
//	/* ��ַ�ж������ */
//	pData = PDU_SlaveDealHRegAddr(addr, addr);
//	if (pData == NULL)
//	{
//		return MB_ILLEGAL_DATA_ADDR;
//	}
//
//	/* ������ */	
//	pduTxBuf[0] = pduRxBuf[0];	/* ������ */
//	
//	*pData = wrValue;
//	pduTxBuf[1] = pduRxBuf[1];
//	pduTxBuf[2] = pduRxBuf[2];
//	pduTxBuf[3] = pduRxBuf[3];
//	pduTxBuf[4] = pduRxBuf[4];
//	*pPduTxTotal = 5;
//
//	return MB_NO_ERR;  	
//}

/*******************************************************************************
 �� ��: PDU_SlaveDealDiag()
 �� ��: �ӻ�����ModBus��·��ϲ�������
 �� ��: pduRxBuf--PDU���ջ�����; 
		pduTxBuf--PDU���ͻ�����; 
		pPduTxTotal--PDU�跢�͵��ֽ�����; 
 �� ��: �쳣��־������⵽�����򷵻ؾ�����쳣�룬���򷵻����쳣��־
 �� ��: �������򷵻��쳣��(�ϲ��ٴ����쳣��ӦPDU)
 		����ȷ���������������ӦPDU�����ͻ�����
		����Ӧ��Э��(��ֻ֧��0�ӹ����룬ԭ�����أ���������Ϊ�Ƿ�����ֵ) 
		���ܺ� ���Թ���(��-��) д����(��-��) д�ֽ��� ��������(��-��)
		08		00	00			00	01			02		55	AA			
		08		00	00			00	01			02		55	AA		
 ע ��:	�������������ӹ����봦��(����0�ӹ�����Э�鲻ͬ)���������SlaverMatchRx()	
*******************************************************************************/
//static U08 PDU_SlaveDealDiag(const U08 *pduRxBuf, U08 *pduTxBuf, U16 *pPduTxTotal)
//{
//	U16 subFunctionCode;	/* �ӹ����� */
//	U16 quantity;			/* ���ݸ��� */
//	U08 bytes;				/* �����ֽ��� */
//  	
//   	subFunctionCode = MAKEWORD(pduRxBuf[1], pduRxBuf[2]);
//
//	if (subFunctionCode == 0)	
//	{
//		/* �����ж� */
//		quantity = MAKEWORD(pduRxBuf[3], pduRxBuf[4]);
//		bytes = pduRxBuf[5];
//		if ((quantity<1) || (quantity>MB_MAX_QUANTITY_DIAG)
//			|| (bytes != 2*quantity))
//		{
//			return MB_ILLEGAL_DATA_VALUE;
//		}
//
//		/* ������ */	
//		MEMCPY(&pduTxBuf[0], &pduRxBuf[0], bytes+6);		
//		*pPduTxTotal = bytes+6;
//		
//		return MB_NO_ERR; 
//	}
//	else
//	{
//		return MB_ILLEGAL_DATA_VALUE;
//	} 	
//}

/*******************************************************************************
 �� ��: PDU_SlaveDealWrMCoils()
 �� ��: �ӻ�����ModBusд�����Ȧ����
 �� ��: pduRxBuf--PDU���ջ�����; 
		pduTxBuf--PDU���ͻ�����; 
		pPduTxTotal--PDU�跢�͵��ֽ�����; 
 �� ��: �쳣��־������⵽�����򷵻ؾ�����쳣�룬���򷵻����쳣��־
 �� ��: �жϸ�������ַ�Ƿ���ȷ���������򷵻��쳣��(�ϲ��ٴ����쳣��ӦPDU)
 		����ȷ���������������ӦPDU�����ͻ�����
		����Ӧ��Э�� 
		0F 00 00 00 06 01 6A 	��00��ʼ����6��λ(1���ֽ�)
		0F 00 00 00 06   
*******************************************************************************/
//static U08 PDU_SlaveDealWrMCoils(const U08 *pduRxBuf, U08 *pduTxBuf, U16 *pPduTxTotal)
//{
//	U16 quantity;			/* ���ݸ��� */
//	U16 beginAddr;			/* ��ʼ��ַ */
//	U16 endAddr;			/* ������ַ */
//	U16 *pData;				/* ����ָ�� */
//	U16 i;
//
//   	beginAddr = MAKEWORD(pduRxBuf[1], pduRxBuf[2]);
//   	quantity = MAKEWORD(pduRxBuf[3], pduRxBuf[4]);
//   	endAddr = beginAddr+quantity-1;
//
//	/* �����ж� */
//	if ((quantity<1) || (quantity>MB_MAX_QUANTITY_WR_BITS)
//		|| (pduRxBuf[5] != (quantity%8==0 ? quantity/8 : quantity/8+1)))	
//	{
//		return MB_ILLEGAL_DATA_VALUE;
//	}
// 
//	/* ��ַ�ж������ */
//	pData = PDU_SlaveDealCoilAddr(&beginAddr, endAddr);
//	if (pData == NULL)
//	{
//		return MB_ILLEGAL_DATA_ADDR;
//	}
//
//	/* ������ */	
//	pduTxBuf[0] = pduRxBuf[0];	/* ������ */
//	
//	for (i=0; i<quantity; i++)
//	{
//		if (GETBIT(pduRxBuf[6+i/8],i%8))
//		{
//			SETBIT(pData[(beginAddr+i)/16], (beginAddr+i)%16);
//		}
//		else
//		{
//			CLRBIT(pData[(beginAddr+i)/16], (beginAddr+i)%16);
//		}			
//	}
//
//	pduTxBuf[1] = pduRxBuf[1];
//	pduTxBuf[2] = pduRxBuf[2];
//	pduTxBuf[3] = pduRxBuf[3];
//	pduTxBuf[4] = pduRxBuf[4];
//	*pPduTxTotal = 5;  
//	
//	return MB_NO_ERR;  	
//}

/*******************************************************************************
 �� ��: PDU_SlaveDealWrMRegs()
 �� ��: �ӻ�����ModBusд����Ĵ�������
 �� ��: pduRxBuf--PDU���ջ�����; 
		pduTxBuf--PDU���ͻ�����; 
		pPduTxTotal--PDU�跢�͵��ֽ�����; 
 �� ��: �쳣��־������⵽�����򷵻ؾ�����쳣�룬���򷵻����쳣��־
 �� ��: �жϸ�������ַ�Ƿ���ȷ���������򷵻��쳣��(�ϲ��ٴ����쳣��ӦPDU)
 		����ȷ���������������ӦPDU�����ͻ�����
		����Ӧ��Э�� 
		10 00 01 00 02 04 11 22 33 44 ��01д�����֣��ĸ��ֽ�
		10 00 01 00 02 10 08  
*******************************************************************************/
//static U08 PDU_SlaveDealWrMRegs(const U08 *pduRxBuf, U08 *pduTxBuf, U16 *pPduTxTotal)
//{
//	U16 quantity;			/* ���ݸ��� */
//	U16 beginAddr;			/* ��ʼ��ַ */
//	U16 endAddr;			/* ������ַ */
//	U16 *pData;				/* ����ָ�� */
//
//   	beginAddr = MAKEWORD(pduRxBuf[1], pduRxBuf[2]);
//   	quantity = MAKEWORD(pduRxBuf[3], pduRxBuf[4]);
//   	endAddr = beginAddr+quantity-1;
//
//	/* �����ж� */
//	if ((quantity<1) || (quantity>MB_MAX_QUANTITY_WR_WORDS)
//		|| (pduRxBuf[5] != 2*quantity))
//	{
//		return MB_ILLEGAL_DATA_VALUE;
//	}
//
//	/* ��ַ�ж������ */
//	pData = PDU_SlaveDealHRegAddr(beginAddr, endAddr);
//	if (pData == NULL)
//	{
//		return MB_ILLEGAL_DATA_ADDR;
//	}
//	
//	/* ������ */	
//	pduTxBuf[0] = pduRxBuf[0];	/* ������ */
//
//	SYS_CopyMemForModbusType((U08 *)pData,&pduRxBuf[6], (quantity<<1));
//	pduTxBuf[1] = pduRxBuf[1];
//	pduTxBuf[2] = pduRxBuf[2];
//	pduTxBuf[3] = pduRxBuf[3];
//	pduTxBuf[4] = pduRxBuf[4];
//	*pPduTxTotal = 5;
//	
//	return MB_NO_ERR;  	
//}


/*******************************************************************************
 �� ��: SlaveDealUserDefined()
 �� ��: �ӻ�����ModBus�û��Զ��幦����:��������
 �� ��: pduRxBuf--PDU���ջ�����; 
		pduTxBuf--PDU���ͻ�����; 
		pPduTxTotal--PDU�跢�͵��ֽ�����; 
 �� ��: �쳣��־������⵽�����򷵻ؾ�����쳣�룬���򷵻����쳣��־
 �� ��: �ж����Զ��������Э���Ƿ���ȷ���������򷵻��쳣��(�ϲ��ٴ����쳣��ӦPDU)
 		����ȷ���������������ӦPDU�����ͻ�����
		����Ӧ��Э�� 
1��ѯ��Ӧ�ó����Ƿ��ܹ�����BOOT
41 + 0000 + 00 + 10 + 20 +  "PUNPPLDOWNLOADERACROSSRS485ENTER"
�ܹ�����BOOT:01 + 41 + 0000 + 0010 + 00 + 01 + CRC
���ܽ���BOOT:01 + 41 + 0000 + 0010 + 00 + 00 + CRC
2�����������BOOT
41 + 0010 + 00 + 10 + 20 +  "PUNPPLDOWNLOADERENTERBOOTFROMAPP"
����ܹ�����BOOT:���أ�ֱ�ӽ���BOOT
������ܹ�����BOOT:01 + 41 + 0010 + 0010 + 00 + 00 + CRC
*******************************************************************************/
//static U08 PDU_SlaveDealUserDef(const U08 *pduRxBuf, U08 *pduTxBuf, U16 *pPduTxTotal)
//{
//	U16 subFunctionCode;	/* �ӹ����� */
//	static CONST I08 s_askMsg[] =      "PUNPPLDOWNLOADERACROSSRS485ENTER"; 
//	static CONST I08 s_downLoadMsg[] = "PUNPPLDOWNLOADERENTERBOOTFROMAPP"; 
//	//U16 *pData;				/* ����ָ�� */
//
//   	subFunctionCode = MAKEWORD(pduRxBuf[1], pduRxBuf[2]);
//
//	/* �����ж� */
//	if ((MAKEWORD(pduRxBuf[3], pduRxBuf[4]) != 16)
//		|| (pduRxBuf[5] != 32))
//	{
//		return MB_ILLEGAL_DATA_VALUE;
//	}
//
//	/* �ӹ�������ж��봦�� */
//	switch (subFunctionCode)
//	{
//		case 0:
//			if (strncmp(s_askMsg, (I08 *)&pduRxBuf[6], 32) == 0)
//			{
//				MEMCPY(&pduTxBuf[0], &pduRxBuf[0], 5);
//				*pPduTxTotal = 7;
//					
//				if (USER_AllowEnterBoot())
//				{
//					pduTxBuf[5] = 0;
//					pduTxBuf[6] = 1;
//				//	USER_EnterBoot();
//				}
//				else
//				{
//					pduTxBuf[5] = 0;
//					pduTxBuf[6] = 0;
//				}
//				return MB_NO_ERR; 
//			}
//			break;
//		case 16:
//			if (strncmp(s_downLoadMsg, (I08 *)&pduRxBuf[6], 32) == 0)
//			{
//				MEMCPY(&pduTxBuf[0], &pduRxBuf[0], 5);
//				*pPduTxTotal = 7;
//					
//				if (USER_AllowEnterBoot())
//				{
//					pduTxBuf[5] = 0;
//					pduTxBuf[6] = 1;
//					USER_EnterBoot();				
//				}
//				else
//				{
//					pduTxBuf[5] = 0;
//					pduTxBuf[6] = 0;
//				}
//				return MB_NO_ERR; 
//			}			
//			break;
//		default:
//			break;
//	}
//
//	return MB_ILLEGAL_DATA_ADDR; 
//}

/*******************************************************************************
 �� ��: MB_MasterMatchRx()
 �� ��: ����ƥ���������
 �� ��: pCOM--���ڽṹ��ָ��; 
		pComMsg--ͨѶ����ָ��; 
 �� ��: TRUE/FALSE--ƥ��ɹ�(�����ӻ����쳣��Ӧ)/ƥ��ʧ��
 �� ��: �ӽ���������ƥ����Ч����:��ַƥ��󣬸������۽��ո��������֡У�飻
 	 	��ʶ��С������֡�ֽ���������ȷ����ǰ������ȷ���ݺ�ĸ��ţ�
 		�ҽ�ƥ�������ǰ�������ջ������ף��Ա�������ݴ���
*******************************************************************************/
static BOOL MB_MasterMatchRx(UART *pCOM, COMMSG *pComMsg)
{
	U16 idxBegin;			/* ��������ƥ��Ŀ�ʼ������ */
	U16 idxEnd;				/* ��������ƥ��Ľ��������� */
	U16 leastRxBytes;		/* ���ٽ����ֽ��� */
	BOOL matchOk;			/* ƥ��OK��ʶ */
	U16 matchOkIdx=0;		/* ƥ��OKʱ��������(ע������ֵ0��ֹ���뾯��) */
	U16 matchOkBytes=0;		/* ƥ��OKʱ���ֽ���(ע������ֵ0��ֹ���뾯��) */
	U16 true_rece_num = 0;
	
	/* ���۽��ո���rxTotal�������÷��ͻ�����ʱ��ȷ���� 
	���յ��������ֽ���С�����ٽ����ֽ���������ȷ���Ǹ��� */
	leastRxBytes = MIN(EXCEP_RSP_BYTES, pCOM->rxTotal);
	if (pCOM->rxCnt < leastRxBytes)
	{
		pCOM->rxFrameType = RX_FRAME_LEN_SHORTAGE;
		pCOM->pRecord->fLenShortage++;
		return FALSE;
	}

	/* ��ͷ���ο�ʼѰ�ҽ��������Ƿ�ƥ�䣬����Ҫ���������н������ݣ�
	��ʣ�����ݸ���С�����ٽ����ֽ��� ʱ���Ͳ���Ҫ�����������ˡ�*/
	matchOk = FALSE;
	idxEnd = pCOM->rxCnt - leastRxBytes; 
	for (idxBegin=0; idxBegin<=idxEnd; idxBegin++)
	{
		U08 txFunCode;		/* ���͵Ĺ����� */
		U08 rxFunCode;		/* ���յĹ����� */
		
		/* ��ַ��ƥ�䣬�ͼ�����һ������ */
		if (pCOM->rxBuf[idxBegin] != pComMsg->driverNo)
		{
			continue;
		}
		
		
		
		/* ��������crc���ж� */
		txFunCode = pCOM->txBuf[1];
		rxFunCode = pCOM->rxBuf[idxBegin+1];
		
		/* 0x17���������⴦�� */
		if (rxFunCode == MB_RDWR_MULTIPLE_REGISTERS)
		{
			true_rece_num = pCOM->rxBuf[idxBegin+2];	/* �ֽ��� */
			
			if (pCOM->rxTotal < true_rece_num + 5)		/* ��5��Ϊ֡ͷ֡β���� */
			{
				pComMsg->length_rd = true_rece_num / 2;
			}
			pCOM->rxTotal = true_rece_num + 5;	/* 0x17�����룬�Դӻ��ͻص�Ϊ׼ */
		}
		
        else if (rxFunCode == MB_USER_DEFINED_EXIO_KLV)
        {
            true_rece_num = MAKEWORD(pCOM->rxBuf[idxBegin+2], pCOM->rxBuf[idxBegin+3]); /* �ֽ��� */
            pComMsg->length_rd = true_rece_num / 2;
            pCOM->rxTotal = true_rece_num + 6;  /* 0x48�����룬�Դӻ��ͻص�Ϊ׼ */
        }
        
		if ((rxFunCode == txFunCode)
			&& (pCOM->rxCnt >= pCOM->rxTotal)
			&& CRC16Ok(&pCOM->rxBuf[idxBegin], pCOM->rxTotal))
		{/* ������Ӧ */
			pCOM->rxFrameType = RX_FRAME_NORMAL_RSP;
			pCOM->exceptCode = MB_NO_ERR;
			matchOk = TRUE;
			matchOkIdx = idxBegin;
			matchOkBytes = pCOM->rxTotal;
			break;
		}
		else if ((rxFunCode == (txFunCode|MK_EXCEP_FUNCODE))
				&& (pCOM->rxCnt >= EXCEP_RSP_BYTES)		
				&& (CRC16Ok(&pCOM->rxBuf[idxBegin], EXCEP_RSP_BYTES)))
		{/* �쳣��Ӧ */
			pCOM->rxFrameType = RX_FRAME_EXCEP_RSP;
			pCOM->exceptCode = pCOM->rxBuf[idxBegin+2];
			matchOk = TRUE;
			matchOkIdx = idxBegin;
			matchOkBytes = EXCEP_RSP_BYTES;
			break;
		}
	}

	if (matchOk)
	{
		/* ƥ������ǰ������������ */
		if (matchOkIdx > 0)
		{
			U16 i;
			for (i=0; i<matchOkBytes; i++)
			{
				pCOM->rxBuf[i] = pCOM->rxBuf[matchOkIdx+i];
			}
		}

		/* ��¼��ȷ���ݴ��� */
		if (pCOM->rxCnt == matchOkBytes)
		{
			pCOM->pRecord->frameOk++;
		}
		else
		{
			if (matchOkIdx == 0)
			{
				pCOM->pRecord->frameOkBack++;					
			}
			else
			{
				pCOM->pRecord->frameOkAhead++;
			}
		}
		
		return TRUE;
	}
	else
	{/* δ�ҵ�ƥ������ */
		pCOM->rxFrameType = RX_FRAME_DATA_ERR;
		pCOM->pRecord->frameDataErr++;
		return FALSE;	
	}
}

/*******************************************************************************
 �� ��: MB_SlaverMatchRx()
 �� ��: �ӻ�ƥ���������
 �� ��: pCOM--���ڽṹ��ָ��; 
		ownAddr--�ӻ��Լ���ͨѶ�豸��ַ; 
 �� ��: TRUE/FALSE--ƥ��ɹ�/ƥ��ʧ��
 �� ��: �ӽ���������ƥ����Ч����:�ҵ�ַ�����ݹ�����������۽��ո��������֡У�飻
 	 	��ʶ��С������֡�ֽ���������ȷ����ǰ������ȷ���ݺ�ĸ��ţ�
 		�ҽ�ƥ�������ǰ�������ջ������ף��Ա�������ݴ���
*******************************************************************************/
static BOOL MB_SlaverMatchRx(UART *pCOM, const U08 ownAddr)
{
	U16 idxBegin;			/* ��������ƥ��Ŀ�ʼ������ */
	U16 idxEnd;				/* ��������ƥ��Ľ��������� */
	BOOL matchOk;			/* ƥ��OK��ʶ */
	U16 matchOkIdx=0;		/* ƥ��OKʱ��������(ע������ֵ0��ֹ���뾯��) */
	U16 theoreticBytes=0;	/* ���ݹ����룬��������յ��ֽ��� */
	
	/* ���յ��������ֽ���С�����ٵ�֡�ֽ���������ȷ���Ǹ��� */
	if (pCOM->rxCnt < LEAST_FRAME_BYTES)
	{
//		if (debug[0]<3)	debug[0]=3;	
		pCOM->pRecord->fLenShortage++;
		return FALSE;
	}

	/* ��ͷ���ο�ʼѰ�ҽ��������Ƿ�ƥ�䣬����Ҫ���������н������ݣ�
	�� ʣ�����ݸ��� С�� ����֡�ֽ��� ʱ���Ͳ���Ҫ�����������ˡ�*/
	matchOk = FALSE;
	idxEnd = pCOM->rxCnt - LEAST_FRAME_BYTES;
	for (idxBegin=0; idxBegin<=idxEnd; idxBegin++)
	{
		U08 rxAddr;			/* ���յĵ�ַ */
		U08 rxFunCode;		/* ���յĹ����� */
		BOOL funCodeMatch;	/* ������ƥ��� */

		/* ��ַ��ƥ�䣬�ͼ�����һ������ */
		rxAddr = pCOM->rxBuf[idxBegin];
		if ((rxAddr!=ownAddr) && (rxAddr!=ADDR_BROADCAST) && (rxAddr!=ADDR_BRO_FBUS))
		{
			continue;
		}

		/* ���ҹ����������۽��ո��� */
		rxFunCode = pCOM->rxBuf[idxBegin+1];
		switch (rxFunCode)
		{
			case MB_RD_COILS:
			case MB_RD_DISCRETE_INPUTS:
			case MB_RD_HOLDING_REGISTERS:
			case MB_RD_INPUT_REGISTERS:
			case MB_WR_SINGLE_COIL:
			case MB_WR_SINGLE_REGISTER:
				funCodeMatch = TRUE;
				theoreticBytes = 8;
				break;
			case MB_DIAGNOSTICS:
			case MB_WR_MULTIPLE_COILS:
			case MB_WR_MULTIPLE_REGISTERS:
			case MB_USER_DEFINED_DOWNLOAD:
				funCodeMatch = TRUE;
				theoreticBytes = 9+pCOM->rxBuf[idxBegin+6];
				break;
#ifdef USE_KIT_PARA_FAC
            case MB_USER_DEFINED_RD_BIGFILE:
                funCodeMatch = TRUE;
                theoreticBytes = 11;
                break;
#endif
            case MB_RDWR_MULTIPLE_REGISTERS:
                funCodeMatch = TRUE;
                theoreticBytes = 5+pCOM->rxBuf[idxBegin+2];
                break;   
            case MB_USER_DEFINED_EXIO_KLV:
                funCodeMatch = TRUE;
                theoreticBytes = 6+MAKEWORD(pCOM->rxBuf[idxBegin+2], pCOM->rxBuf[idxBegin+3]);
                break; 
			default:
				funCodeMatch = FALSE;
				break;
		}

		/* ����������ո�����ƥ�䣬�ͼ�����һ������ */
		if ((!funCodeMatch) || (pCOM->rxCnt < (idxBegin+theoreticBytes))) 
		{
			continue;
		}

		/* crcУ��ͨ����ƥ��ɹ��˳����ң����������һ������ */
		if (CRC16Ok(&pCOM->rxBuf[idxBegin], theoreticBytes))
		{
			matchOk = TRUE;
			matchOkIdx = idxBegin;
			break;
		}
	}

	if (matchOk)
	{
		/* ƥ������ǰ������������ */
		if (matchOkIdx > 0)
		{
			U16 i;
			for (i=0; i<theoreticBytes; i++)
			{
				pCOM->rxBuf[i] = pCOM->rxBuf[matchOkIdx+i];
			}
		}	

		/* ��¼��ȷ���ݴ��� */
		if (pCOM->rxCnt == theoreticBytes)
		{
			pCOM->pRecord->frameOk++;
		}
		else
		{
			if (matchOkIdx == 0)
			{
				pCOM->pRecord->frameOkBack++;
			}
			else
			{					
				pCOM->pRecord->frameOkAhead++;
			}
		}
		
		return TRUE;
	}
	else
	{/* δ�ҵ�ƥ������ */
//		if (debug[0]<4)	debug[0]=4;	
		pCOM->pRecord->frameDataErr++;
		return FALSE;		
	}
}

/*******************************************************************************
 �� ��: MB_MasterTxReqPDU()
 �� ��: ��������MODBUS����PDU��������������(֡ͷ�ĵ�ַ��֡β��CRCУ��)
 �� ��: pComMsg--ͨѶ����ָ��; 
		pduTxBuf[]--PDU���ͻ�����; 
		pPduTxTotal--PDU�跢�͵��ֽ���; 
		pPduRxTotal--PDU����յ��ֽ���; 
 �� ��: ��
 �� ��: ���ú�������ʵ�������������(֡ͷ�ĵ�ַ��֡β��CRCУ��)
*******************************************************************************/
BOOL MB_MasterTxReqPDU(const COMMSG *pComMsg, U08 pduTxBuf[], U16 *pPduTxTotal, U16 *pPduRxTotal)
{
	U16 i;
	U08 *p;
	
	pduTxBuf[0] = pComMsg->cmd;

	switch (pComMsg->cmd)
	{
		case MB_RD_COILS:					/* ���ɶ�дλ */
		case MB_RD_DISCRETE_INPUTS:			/* ��ֻ��λ */
			pduTxBuf[1] = HIBYTE(pComMsg->startAddr);
			pduTxBuf[2] = LOBYTE(pComMsg->startAddr);
			pduTxBuf[3] = HIBYTE(pComMsg->length_rd);
			pduTxBuf[4] = LOBYTE(pComMsg->length_rd);
			(*pPduTxTotal) = 5;
			(*pPduRxTotal) = (U08)(2+pComMsg->length_rd/8+(pComMsg->length_rd%8?1:0));
			break;
		case MB_RD_HOLDING_REGISTERS:		/* ���ɶ�д�� */
		case MB_RD_INPUT_REGISTERS:			/* ��ֻ���� */
			pduTxBuf[1] = HIBYTE(pComMsg->startAddr);
			pduTxBuf[2] = LOBYTE(pComMsg->startAddr);
			pduTxBuf[3] = HIBYTE(pComMsg->length_rd);
			pduTxBuf[4] = LOBYTE(pComMsg->length_rd);
			(*pPduTxTotal) = 5;
			(*pPduRxTotal) = (U08)(2+pComMsg->length_rd*2);
			
			break;
		case MB_WR_SINGLE_COIL:				/* д�����ɶ�дλ */
			pduTxBuf[1] = HIBYTE(pComMsg->startAddr);
			pduTxBuf[2] = LOBYTE(pComMsg->startAddr);
			pduTxBuf[3] = (pComMsg->oneWrVal != 0) ? 0xff : 0;
			pduTxBuf[4] = 0;
			(*pPduTxTotal) = 5;
			(*pPduRxTotal) = 5;
			break;
		case MB_WR_SINGLE_REGISTER:			/* д�����ɶ�д�� */
			pduTxBuf[1] = HIBYTE(pComMsg->startAddr);
			pduTxBuf[2] = LOBYTE(pComMsg->startAddr);
			pduTxBuf[3] = HIBYTE(pComMsg->oneWrVal);
			pduTxBuf[4] = LOBYTE(pComMsg->oneWrVal);
			(*pPduTxTotal) = 5;
			(*pPduRxTotal) = 5;
			break;

		case MB_DIAGNOSTICS:				/* ���ջ�·����(�ӻ�ԭ������) */
/*
	���ܺ� ���Թ���(��-��) д����(��-��) д�ֽ��� ��������(��-��) 
	08		00	00			00	01			02		55	AA			
*/
			pduTxBuf[1] = 0x00;
	        pduTxBuf[2] = 0x00;
			pduTxBuf[3] = HIBYTE(pComMsg->length_wr);
	        pduTxBuf[4] = LOBYTE(pComMsg->length_wr); 
			pduTxBuf[5] = LOBYTE(pComMsg->length_wr*2);
			SYS_CopyMemForModbusType(&pduTxBuf[6],(U08 *)pComMsg->pData_rd,pComMsg->length_wr*2);
			(*pPduTxTotal) = (U08)(6 + pComMsg->length_wr*2);
	        (*pPduRxTotal) = (*pPduTxTotal);
			break;
		
		case MB_WR_MULTIPLE_COILS:			/* д����ɶ�дλ */
			pduTxBuf[1] = HIBYTE(pComMsg->startAddr);
	        pduTxBuf[2] = LOBYTE(pComMsg->startAddr);
			pduTxBuf[3] = HIBYTE(pComMsg->length_wr);
	        pduTxBuf[4] = LOBYTE(pComMsg->length_wr); 
			pduTxBuf[5] = (U08)((pComMsg->length_wr/8)+(pComMsg->length_wr%8?1:0));
			(*pPduTxTotal) = 6;
			(*pPduRxTotal) = 5;

			p = (U08 *)pComMsg->pData_wr;
//			if ((pComMsg->startAddr%8) == 0)
			{
				U16 inte = pComMsg->length_wr / 8;				/* ���� */
				U16 rest = pComMsg->length_wr % 8;				/* ���� */
				
				for (i=inte; i!=0; i--)
				{
					pduTxBuf[(*pPduTxTotal)++] = *p++;
				}
				/* ���һ���ֽ����⴦�� */
				if (rest != 0)
				{
					pduTxBuf[(*pPduTxTotal)++] = (*p++)&(((U16)1<<rest)-1);
				}
			}
			break;
			
		case MB_WR_MULTIPLE_REGISTERS:		/* д����ɶ�д�� */
			pduTxBuf[1] = HIBYTE(pComMsg->startAddr);
	        pduTxBuf[2] = LOBYTE(pComMsg->startAddr);
			pduTxBuf[3] = HIBYTE(pComMsg->length_wr);
	        pduTxBuf[4] = LOBYTE(pComMsg->length_wr); 
			pduTxBuf[5] = LOBYTE(pComMsg->length_wr*2);
			SYS_CopyMemForModbusType(&pduTxBuf[6], (U08 *)pComMsg->pData_wr, pComMsg->length_wr*2);
			(*pPduTxTotal) = (U08)(6 + pComMsg->length_wr*2);
			(*pPduRxTotal) = 5;
			break;
		
		case MB_RDWR_MULTIPLE_REGISTERS:	/* ��д */
			pduTxBuf[1] = LOBYTE(pComMsg->length_wr*2);
			SYS_CopyMemForModbusType(&pduTxBuf[2], (U08 *)pComMsg->pData_wr, pComMsg->length_wr*2);
			(*pPduTxTotal) = (U08)(2 + pComMsg->length_wr*2);
			(*pPduRxTotal) = (U08)(2 + pComMsg->length_rd*2);
			break;
			
        case MB_USER_DEFINED_EXIO_KLV:           /* KLV��д */
            (*pPduTxTotal) = EK_mst_send_request(pduTxBuf, pComMsg->driverNo) + 1;
            (*pPduRxTotal) = 0; // ��Э�̳���,������
            break;
		default:
			return FALSE;
	}

	return TRUE;
}

/*******************************************************************************
 �� ��: MB_MasterRxRspPDU()
 �� ��: �������մӻ���MODBUS��ӦPDU
 �� ��: pComMsg--ͨѶ����ָ��; 
		pduRxBuf[]--PDU���ջ�����; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void MB_MasterRxRspPDU(const COMMSG *pComMsg, const U08 pduRxBuf[])
{	
	/* �����������յ����� */
	switch (pComMsg->cmd) 
	{
		BOL bak_s_err_commu_expt_bit;
		case MB_RD_HOLDING_REGISTERS:
		case MB_RD_INPUT_REGISTERS:
    case MB_RDWR_MULTIPLE_REGISTERS:
			if (GET_BIT(fg_commu_err, pComMsg->driverNo))
			{
				//���ݵ�ǰ"ͨѶ�쳣"���ϵ�״̬
				bak_s_err_commu_expt_bit = m_get_error(pComMsg->driverNo, S_ERR_COMMU_EXPT);
			} 
		
			SYS_CopyMemForModbusType((U08 *)pComMsg->pData_rd, &pduRxBuf[2], pComMsg->length_rd*2);

			if (GET_BIT(fg_commu_err, pComMsg->driverNo))
			{
				if (bak_s_err_commu_expt_bit)	m_set_error(pComMsg->driverNo, S_ERR_COMMU_EXPT);
				else							m_clr_error(pComMsg->driverNo, S_ERR_COMMU_EXPT);
			} 	
			
            break;
			
        case MB_USER_DEFINED_EXIO_KLV:
            EK_mst_parse_respond(pduRxBuf, pComMsg->driverNo);
			break;
		case MB_DIAGNOSTICS:
		default:
			break;
	}
}

/*******************************************************************************
 �� ��: MB_MasterTx()
 �� ��: ����MODBUSЭ�飬�����нڵ�����ת���ɴ�������(���÷��ͻ�����)
 �� ��: pComMsg--ͨѶ�������ָ��; 
		pCOM--���ڽṹ��ָ��; 
 �� ��: TRUE/FALSE--���ͳɹ�/����ʧ��
 �� ��: ��
*******************************************************************************/
BOOL MB_MasterTx(const COMMSG *pComMsg, UART *pCOM)
{
	W16 crc;
	
	/* ��������PDU */
	if (!MB_MasterTxReqPDU(pComMsg, &pCOM->txBuf[1], &pCOM->txTotal, &pCOM->rxTotal))
	{
		return FALSE;
	}

	/* ����֡ͷ(�豸��ַ) */
	pCOM->txBuf[0] = pComMsg->driverNo; 
	pCOM->txTotal++;
	pCOM->rxTotal++;

	/* ����֡β(crc) */
	crc.word = CRC16(&pCOM->txBuf[0], pCOM->txTotal);
	pCOM->txBuf[pCOM->txTotal++] = crc.byte.low;
	pCOM->txBuf[pCOM->txTotal++] = crc.byte.high;
	pCOM->rxTotal += 2;

	return  TRUE;
}

/*******************************************************************************
 �� ��: MB_MasterRx()
 �� ��: ��������Э��
 �� ��: pCOM--���ڽṹ��ָ��; 
		pComMsg--ͨѶ����ָ��; 		
 �� ��: TRUE/FALSE--���ճɹ�/����ʧ��
 �� ��: ��
*******************************************************************************/
BOOL MB_MasterRx(U08 u, UART *pCOM, COMMSG *pComMsg)
{
    u = u;					/* N0001 */
	/* ����ƥ���������ʧ�� */
	if (!MB_MasterMatchRx(pCOM, pComMsg))
	{
		return FALSE;
	}

	/* �ӻ��쳣��Ӧʱ */
	if (pCOM->rxFrameType == RX_FRAME_EXCEP_RSP)
	{
//		return TRUE;
		return FALSE;	/* COM-V100A2-0 */
	}

//	if( u == COM_INVT               //NOTE-CXW ����ѹ����Ϣʱ�� 
//	    /* && fg_trans_control_cmd[pComMsg->driverNo]  */
//	    && pComMsg->cmd == MB_RD_INPUT_REGISTERS
//		/* && (pComMsg->startAddr >= ADDR_COMP_BGN 
//			 && pComMsg->startAddr < ADDR_COMP_END) */)//���Ϳ����������һ��ѹ����Ϣ�Ķ�ȡ
//	{
//        //fg_trans_control_cmd[pComMsg->driverNo] = FALSE;
//        return  TRUE;
//	}
//	else
	{
	    MB_MasterRxRspPDU(pComMsg, &pCOM->rxBuf[1]);
	}
		
	return  TRUE;
}

/*******************************************************************************
 �� ��: MB_SlaveRxReqTxRspPDU()
 �� ��: �ӻ���������������PDU��������ӦPDU������
 �� ��: pduRxBuf--PDU���ջ�����; 
		pduTxBuf--PDU���ͻ�����; 
		pPduTxTotal--PDU�跢�͵��ֽ�����; 
 �� ��: �쳣��־������⵽�����򷵻ؾ�����쳣�룬���򷵻����쳣��־
 �� ��: ������PDU�����򷵻��쳣��;����ȷ����������PDU������ӦPDU�����ͻ�����
*******************************************************************************/
//U08 MB_SlaveRxReqTxRspPDU(const U08 *pduRxBuf, U08 *pduTxBuf, U16 *pPduTxTotal)
//{
//	U08 exceptCode;
//	
//	//	if ()
//	
//    switch (pduRxBuf[0])
//	{
//	    case MB_DIAGNOSTICS :
//			exceptCode = PDU_SlaveDealDiag(pduRxBuf, pduTxBuf, pPduTxTotal);
//	    	break;
//		case MB_RD_COILS :	
//			exceptCode = PDU_SlaveDealRdCoils(pduRxBuf, pduTxBuf, pPduTxTotal);
//			break;
//		case MB_RD_DISCRETE_INPUTS :
//			exceptCode = PDU_SlaveDealRdDInputs(pduRxBuf, pduTxBuf, pPduTxTotal);
//			break;
//	    case MB_RD_HOLDING_REGISTERS :
//			exceptCode = PDU_SlaveDealRdHRegs(pduRxBuf, pduTxBuf, pPduTxTotal);
//	    	break;
//	    case MB_RD_INPUT_REGISTERS :
//			exceptCode = PDU_SlaveDealRdIRegs(pduRxBuf, pduTxBuf, pPduTxTotal);
//			break;
//	    case MB_WR_SINGLE_COIL :
//			exceptCode = PDU_SlaveDealWrSCoil(pduRxBuf, pduTxBuf, pPduTxTotal);
//	    	break;    	
//	    case MB_WR_SINGLE_REGISTER :
//			exceptCode = PDU_SlaveDealWrSReg(pduRxBuf, pduTxBuf, pPduTxTotal);
//	    	break;
//	    case MB_WR_MULTIPLE_COILS :				
//			exceptCode = PDU_SlaveDealWrMCoils(pduRxBuf, pduTxBuf, pPduTxTotal);
//	    	break;
//	    case MB_WR_MULTIPLE_REGISTERS :
//			exceptCode = PDU_SlaveDealWrMRegs(pduRxBuf, pduTxBuf, pPduTxTotal);
//	    	break;
//	    case MB_USER_DEFINED_DOWNLOAD :
//	    	exceptCode = PDU_SlaveDealUserDef(pduRxBuf, pduTxBuf, pPduTxTotal);
//	    	break;
//	    default :
//	    	/* һЩ��ȷ�Ĺ����룬�����ݲ�֧�֣�Ҳ��Ϊ�Ƿ������� */
//	    	exceptCode = MB_ILLEGAL_FUNCTION;				
//	    	break;
//	}
//
//	return exceptCode;
//}

/*******************************************************************************
 �� ��: MB_SlaverRxTx()
 �� ��: �ӻ����շ���Э��
 �� ��: pCOM--���ڽṹ��ָ��; 
 �� ��: TRUE/FALSE--���ճɹ�/����ʧ��
 �� ��: �㲥ͨѶʱ��Ҳ����TRUE���÷��ͻ���������COM_SlaverProg()�����жϴ���
*******************************************************************************/
BOOL MB_SlaverRxTx(U08 u, UART *pCOM)
{
	W16 crc;

	/* �ӻ�ƥ���������ʧ�� */
	if (!MB_SlaverMatchRx(pCOM, pCOM->ownAddr))
	{
		return FALSE;
	} 		

  	/* ��������PDU(�������򷵻��쳣��;����ȷ�������������ӦPDU�����ͻ�����) */
	pCOM->txTotal = 0;//�����ֽ���������
	pCOM->exceptCode = com_slv_datanum_err(&pCOM->rxBuf[0]);    
	if (pCOM->exceptCode == MB_NO_ERR)
	{
        pCOM->exceptCode = MB_ILLEGAL_DATA_ADDR;//Ĭ��Ϊ�Ƿ���ַ
    	switch(u)
    	{     
#ifdef COM_4G
            case COM_4G:
#endif
            case COM_BMS:
    			if (TestProgStaus != TPS_NO_TEST_CMD || power_time < POWER_ON_INTO_TEST) 
    			{
    			    pCOM->exceptCode = com_slv_HMI(&pCOM->rxBuf[0], &pCOM->txBuf[0], &pCOM->txTotal);   // ������8�����ͨѶ
    			}
                else
                {
#ifndef USE_KIT_PARA_FAC
                    pCOM->exceptCode = com_slv_BMS(&pCOM->rxBuf[0], &pCOM->txBuf[0], &pCOM->txTotal);   // �����BMSͨѶ
#else
                    pCOM->exceptCode = com_slv_BMS_aux(&pCOM->rxBuf[0], &pCOM->txBuf[0], &pCOM->txTotal);	/* ֻ�Բ����������� */
#endif              
    			}
    			break;
    			       
#ifdef COM_FCU_FBUS
            case COM_FCU_FBUS:   /* N0004 */    
    			pCOM->exceptCode = com_slv_FBUS(&pCOM->rxBuf[0], &pCOM->txBuf[0], &pCOM->txTotal);
    			break;
#endif /* COM_FCU_FBUS */        

#if defined(M_NIUENTAI) || defined(M_DEBUG)
            case COM_TEXT_HMI:  
    			pCOM->exceptCode = com_slv_BMS(&pCOM->rxBuf[0], &pCOM->txBuf[0], &pCOM->txTotal);
                if (pCOM->exceptCode == MB_NO_ERR)
                {
                    _CNT(SH_hmi_commu_succ);/* ע��:ͨѶ�ɹ�����Ҫ�ۼӳɹ���������ᱻ��Ϊ��ͨѶʧ�ܶ��л������� */
                }
    			break;
#endif

    		default:
    			break;
    	}	
    }
	
	/* �����쳣��ӦPDU */
	if (pCOM->exceptCode != MB_NO_ERR)
	{
		pCOM->txBuf[1] = (pCOM->rxBuf[1] | MK_EXCEP_FUNCODE);
		pCOM->txBuf[2] = pCOM->exceptCode;
		pCOM->txTotal = 3;
	}
	else
	{
        pCOM->txBuf[1] = pCOM->rxBuf[1];
	}

    /* ���Ӹ�����:֡ͷ�ĵ�ַ��֡β��CRCУ�� */
	pCOM->txBuf[0] = pCOM->rxBuf[0];
	crc.word = CRC16(&pCOM->txBuf[0], pCOM->txTotal);
	pCOM->txBuf[pCOM->txTotal++] = crc.byte.low;
	pCOM->txBuf[pCOM->txTotal++] = crc.byte.high;

    return  TRUE;
}
/************************************************
������modbus_time
���ܣ�MODBUS��ؼ�ʱ
������
����ֵ����
**************************************************/
void modbus_time(void)
{
//	U08 i;
	
	if (F_TIME_BASIC_SEC_BACK)
	{
//        #if CLOUD_USED
//		hmi_trans_type=HMI_DATA;	//ÿ��1s����һ�λ�������
//        #endif
	}
}

/*******************************************************************************
 �� ��: USER_AllowEnterBoot()
 �� ��: �Ƿ��ܹ�����BOOT��Ӧ�ó����е��жϣ�����״̬�²������س���
 �� ��: ��
 �� ��: TRUE/FALSE--����/��ֹ
 �� ��: ��
*******************************************************************************/
BOOL USER_AllowEnterBoot(void)
{
    if(status_run == _IDLE || status_run == _ALARM)
    {
	    return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*******************************************************************************
 �� ��: USER_EnterBoot()
 �� ��: �������boot
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void USER_EnterBoot(void)
{
	SYS_JumpToBoot();
}


