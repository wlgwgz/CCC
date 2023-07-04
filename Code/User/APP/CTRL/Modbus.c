/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: Modbus.c
 文件描述: 
 创建人  : 
 创建日期: 
*******************************************************************************/
#define MODBUS_GLOBAL
#include "Includes.h"
//static U16 debug_com[50];

/* 不比特访问参数区与程序区，可以比特访问其它M区 */
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
读保持、输入寄存器响应 地址(1)+功能码(1)+字节数(1)+读值(n1)+crc(2)
读线圈、离散量输入响应 地址(1)+功能码(1)+字节数(1)+读值(n2)+crc(2)
	n1=(MB_SIZE-5)/2,	n2=16*n1[保证二者的访问范围一致] 
注: n2不一定等于(MB_SIZE-5)*8 [MB_SIZE为偶数时，不成立]
例: MB缓冲区大小为256，读WORDS的最大数量为125，读BITS的最大数量为2000(不是2008)

写保持寄存器请求 地址(1)+功能码(1)+起始地址(2)+数量(2)+字节数(1)+写值(n3)+crc(2)
写线圈请求 		 地址(1)+功能码(1)+起始地址(2)+数量(2)+字节数(1)+写值(n4)+crc(2)
	n3=(MB_SIZE-9)/2,	n4=16*n3[保证二者的访问范围一致] 
注: n4不一定等于(MB_SIZE-9)*8 [MB_SIZE为偶数时，不成立]
例: MB缓冲区大小为256，写WORDS的最大数量为123，写BITS的最大数量为1968(不是1976)

邦普回路测试请求 地址(1)+功能码(1)+测试子码(2)+数量(2)+字节数(1)+写值(n3)+crc(2)
	n3=(MB_SIZE-9)/2
*******************************************************************************/


/*******************************************************************************
最少的帧字节数:地址 功能码 校验和(2个字节)
异常响应字节数:地址 功能码(>=80) 异常码 校验和(2个字节)
*******************************************************************************/
#define LEAST_FRAME_BYTES	4				/* 最少的帧字节数 */
#define EXCEP_RSP_BYTES		5				/* 异常响应字节数 */

#define MK_EXCEP_FUNCODE	0x80			/* 异常功能码屏蔽字 */

/****************************************************************
函数：com_slv_datanum_err
功能：数据个数错误。
参数：功能码。
返回：错误值(正常/数据个数错误)
其它：/。
*****************************************************************/
U08 com_slv_datanum_err(U08 *RxBuf)
{
	U08 err_result = MB_NO_ERR;
	U08 func = RxBuf[1];		/* 功能码 */
	U16 data_num = 0;
	U16 byte_count = 0;
	
	switch(func)
	{
		case MB_RD_HOLDING_REGISTERS :	/* 0x03 读可读写字 */
//	    	break;			
	    case MB_RD_INPUT_REGISTERS :	/* 0x04 读只读字 */
	        data_num = MAKEWORD(RxBuf[4], RxBuf[5]);	/* 数据个数 */
			if ((data_num < 1) || (data_num > MB_MAX_QUANTITY_RD_WORDS))	
        	{
        		err_result = MB_ILLEGAL_DATA_VALUE;
//				if (debug[0]<4)	debug[0]=4;	
        	}
			break;
			
		case MB_WR_SINGLE_COIL:			/* 0x05 写单个位 */
			data_num = MAKEWORD(RxBuf[4], RxBuf[5]);
			if (data_num!=0x0000 && data_num!=0xff00)	/* 数据值错误 */
			{
    			err_result = MB_ILLEGAL_DATA_VALUE;
    		}
			break;
			
		case MB_WR_MULTIPLE_COILS:		/* 0x0f 写多个位 */
			data_num = MAKEWORD(RxBuf[4], RxBuf[5]);	/* 数据个数 */
			byte_count = RxBuf[6];						/* 字节数 */
			
			if ((data_num<1) || (data_num>MB_MAX_QUANTITY_WR_BITS)
				|| (byte_count != (data_num%8==0 ? data_num/8 : data_num/8+1)))	/* 数据个数错误 */
        	{
        		err_result = MB_ILLEGAL_DATA_VALUE;
        	}
			break;
		
		case MB_WR_SINGLE_REGISTER:		/* 0x06 写单个字 */
			
			break;
		
		case MB_WR_MULTIPLE_REGISTERS :	/* 0x10 写多个字 */	
			data_num = MAKEWORD(RxBuf[4], RxBuf[5]);	/* 数据个数 */
			byte_count = RxBuf[6];						/* 字节数 */
			
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
 函 数: SYS_CopyMemForModbusType()
 功 能: 存储空间复制，且源数据类似于Modbus类型(高8位在前，低8位在后)
 参 数: pDst--目的数据指针; 
		pSrc--源数据指针; 
		size--字节大小(必须为偶数); 
 返 回: 空
 其 它: 空
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
 函 数: PDU_SlaveDealCoilAddr()
 功 能: 处理线圈的地址
 参 数: pBeginAddr--开始地址指针(有时需调整开始地址); 
		endAddr--结束地址; 
 返 回: 地址
 其 它: 空
*******************************************************************************/
//static U16 * PDU_SlaveDealCoilAddr(U16 *pBeginAddr,U16 endAddr)
//{
//	/* 消除pBeginAddr未被使用的编译警告信息 */
//#if (MODBUS_RW_BIT_Q_START == 0)
//	if (*pBeginAddr > 1)
//	{
//		ASM_NOP();
//	}
//#endif	
//
//#ifndef ACCESS_M_THROUGH_Q
//	/* 分开访问QM */
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
//		/* 地址超界 */
//		return NULL;
//	}	
//#else
//	/* 一起访问QM(通过Q访问M) */
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
//		/* 地址超界 */
//		return NULL;
//	}
//#endif /* ACCESS_M_THROUGH_Q */
//}

/*******************************************************************************
 函 数: PDU_SlaveDealHRegAddr()
 功 能: 处理保持寄存器的地址
 参 数: beginAddr--开始地址; 
		endAddr--结束地址; 
 返 回: 地址
 其 它: Q区与M区尽管是连续的，但M区空间已到64K(默认的地址访问不能超过64K)
*******************************************************************************/
//static U16 * PDU_SlaveDealHRegAddr(U16 beginAddr,U16 endAddr)
//{
//#ifndef ACCESS_M_THROUGH_Q
//	/* 分开访问QM */
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
//		/* 地址超界 */
//		return NULL;
//	}
//#else
//	/* 一起访问QM(通过Q访问M) */
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
//		/* 地址超界 */
//		return NULL;
//	}
//#endif /* ACCESS_M_THROUGH_Q */
//}

/*******************************************************************************
 函 数: PDU_SlaveDealRdCoils()
 功 能: 从机处理ModBus读线圈命令
 参 数: pduRxBuf--PDU接收缓冲区; 
		pduTxBuf--PDU发送缓冲区; 
		pPduTxTotal--PDU需发送的字节总数; 
 返 回: 异常标志，若检测到错误，则返回具体的异常码，否则返回无异常标志
 其 它: 判断个数、地址是否正确，若错误则返回异常码(上层再处理异常响应PDU)
 		若正确，处理命令，并置响应PDU到发送缓冲区
		具体应用协议 
		01 00 00 00 20  	从00 00 开始读取32个位 
		01 04 55 AA 22 66   从低到高，分别为55 AA 22 66 
*******************************************************************************/
//static U08 PDU_SlaveDealRdCoils(const U08 *pduRxBuf, U08 *pduTxBuf, U16 *pPduTxTotal)
//{
//	U16 quantity;			/* 数据个数 */
//	U16 beginAddr;			/* 起始地址 */
//	U16 endAddr;			/* 结束地址 */
//	U16 *pData;				/* 数据指针 */
//	U16 i;
//
//   	beginAddr = MAKEWORD(pduRxBuf[1], pduRxBuf[2]);
//   	quantity = MAKEWORD(pduRxBuf[3], pduRxBuf[4]);
//   	endAddr = beginAddr+quantity-1;
//
//	/* 个数判断 */
//	if ((quantity<1) || (quantity>MB_MAX_QUANTITY_RD_BITS))	
//	{
//		return MB_ILLEGAL_DATA_VALUE;
//	}
//
//	/* 地址判断与调整 */
//	pData = PDU_SlaveDealCoilAddr(&beginAddr, endAddr);
//	if (pData == NULL)
//	{
//		return MB_ILLEGAL_DATA_ADDR;
//	}
//
//	/* 请求处理 */	
//	pduTxBuf[0] = pduRxBuf[0];	/* 功能码 */
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
 函 数: PDU_SlaveDealRdDInputs()
 功 能: 从机处理ModBus读离散量输入命令
 参 数: pduRxBuf--PDU接收缓冲区; 
		pduTxBuf--PDU发送缓冲区; 
		pPduTxTotal--PDU需发送的字节总数; 
 返 回: 异常标志，若检测到错误，则返回具体的异常码，否则返回无异常标志
 其 它: 判断个数、地址是否正确，若错误则返回异常码(上层再处理异常响应PDU)
 		若正确，处理命令，并置响应PDU到发送缓冲区
		具体应用协议 
		参考读线圈
*******************************************************************************/
//U08 PDU_SlaveDealRdDInputs(const U08 *pduRxBuf, U08 *pduTxBuf, U16 *pPduTxTotal)
//{
//	U16 quantity;			/* 数据个数 */
//	U16 beginAddr;			/* 起始地址 */
//	U16 endAddr;			/* 结束地址 */
//	U16 i;
//
//   	beginAddr = MAKEWORD(pduRxBuf[1], pduRxBuf[2]);
//   	quantity = MAKEWORD(pduRxBuf[3], pduRxBuf[4]);
//   	endAddr = beginAddr+quantity-1;
//
//	/* 个数判断 */
//	if ((quantity<1) || (quantity>MB_MAX_QUANTITY_RD_BITS))
//	{
//		return MB_ILLEGAL_DATA_VALUE;
//	}
//	
//	/* 地址判断 */
//	if (
//#if (MODBUS_RO_BIT_I_START != 0)
//			(beginAddr < MODBUS_RO_BIT_I_START) ||
//#endif
//			(endAddr > MODBUS_RO_BIT_I_END))
//	{
//		return MB_ILLEGAL_DATA_ADDR;
//	}
//	
//	/* 请求处理 */	
//	pduTxBuf[0] = pduRxBuf[0];	/* 功能码 */
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
 函 数: PDU_SlaveDealRdHRegs()
 功 能: 从机处理ModBus读保持寄存器命令
 参 数: pduRxBuf--PDU接收缓冲区; 
		pduTxBuf--PDU发送缓冲区; 
		pPduTxTotal--PDU需发送的字节总数; 
 返 回: 异常标志，若检测到错误，则返回具体的异常码，否则返回无异常标志
 其 它: 判断个数、地址是否正确，若错误则返回异常码(上层再处理异常响应PDU)
 		若正确，处理命令，并置响应PDU到发送缓冲区
		具体应用协议 
		03 00 00 00 04  				从00开始读四个字
		03 08 00 55 11 22 33 44 00 66   8个字节，高位在前 
*******************************************************************************/
//static U08 PDU_SlaveDealRdHRegs(const U08 *pduRxBuf, U08 *pduTxBuf, U16 *pPduTxTotal)
//{
//	U16 quantity;			/* 数据个数 */
//	U16 beginAddr;			/* 起始地址 */
//	U16 endAddr;			/* 结束地址 */
//	U16 *pData;				/* 数据指针 */
//
//   	beginAddr = MAKEWORD(pduRxBuf[1], pduRxBuf[2]);
//   	quantity = MAKEWORD(pduRxBuf[3], pduRxBuf[4]);
//   	endAddr = beginAddr+quantity-1;
//
//	/* 个数判断 */
//	if ((quantity<1) || (quantity>MB_MAX_QUANTITY_RD_WORDS))	
//	{
//		return MB_ILLEGAL_DATA_VALUE;
//	}
//	
//	/* 地址判断与调整 */
//	pData = PDU_SlaveDealHRegAddr(beginAddr, endAddr);
//	if (pData == NULL)
//	{
//		return MB_ILLEGAL_DATA_ADDR;
//	}
//	
//	/* 请求处理 */	
//	pduTxBuf[0] = pduRxBuf[0];	/* 功能码 */
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
 函 数: PDU_SlaveDealRdIRegs()
 功 能: 从机处理ModBus读输入寄存器命令
 参 数: pduRxBuf--PDU接收缓冲区; 
		pduTxBuf--PDU发送缓冲区; 
		pPduTxTotal--PDU需发送的字节总数; 
 返 回: 异常标志，若检测到错误，则返回具体的异常码，否则返回无异常标志
 其 它: 判断个数、地址是否正确，若错误则返回异常码(上层再处理异常响应PDU)
 		若正确，处理命令，并置响应PDU到发送缓冲区
		具体应用协议 
		参考读保持寄存器
*******************************************************************************/
//static U08 PDU_SlaveDealRdIRegs(const U08 *pduRxBuf, U08 *pduTxBuf, U16 *pPduTxTotal)
//{
//	U16 quantity;			/* 数据个数 */
//	U16 beginAddr;			/* 起始地址 */
//	U16 endAddr;			/* 结束地址 */
//
//   	beginAddr = MAKEWORD(pduRxBuf[1], pduRxBuf[2]);
//   	quantity = MAKEWORD(pduRxBuf[3], pduRxBuf[4]);
//   	endAddr = beginAddr+quantity-1;
//
//	/* 个数判断 */
//	if ((quantity<1) || (quantity>MB_MAX_QUANTITY_RD_WORDS))
//	{
//		return MB_ILLEGAL_DATA_VALUE;
//	}
//
//	/* 地址判断 */
//	if (				
//#if (MODBUS_RO_WORD_I_START != 0)			
//			(beginAddr < MODBUS_RO_WORD_I_START) ||
//#endif
//			(endAddr > MODBUS_RO_WORD_I_END))
//	{
//		return MB_ILLEGAL_DATA_ADDR;
//	}
//
//	/* 请求处理 */	
//	pduTxBuf[0] = pduRxBuf[0];	/* 功能码 */
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
 函 数: PDU_SlaveDealWrSCoil()
 功 能: 从机处理ModBus写单个线圈命令
 参 数: pduRxBuf--PDU接收缓冲区; 
		pduTxBuf--PDU发送缓冲区; 
		pPduTxTotal--PDU需发送的字节总数; 
 返 回: 异常标志，若检测到错误，则返回具体的异常码，否则返回无异常标志
 其 它: 判断数值、地址是否正确，若错误则返回异常码(上层再处理异常响应PDU)
 		若正确，处理命令，并置响应PDU到发送缓冲区
		具体应用协议 
		05 00 01 FF 00   设置第01位为1
		05 00 01 FF 00   回复相同 
*******************************************************************************/
//static U08 PDU_SlaveDealWrSCoil(const U08 *pduRxBuf, U08 *pduTxBuf, U16 *pPduTxTotal)
//{
//	U16 wrValue;			/* 写值 */
//	U16 addr;				/* 数据地址 */
//	U16 *pData;				/* 数据指针 */
//
//   	addr = MAKEWORD(pduRxBuf[1], pduRxBuf[2]);
//   	wrValue = MAKEWORD(pduRxBuf[3], pduRxBuf[4]);
//
//	/* 数值判断(0x0000:0\0xff000:1) */
//	if (wrValue!=0x0000 && wrValue!=0xff00)
//	{
//		return MB_ILLEGAL_DATA_VALUE;
//	}
//	
//	/* 地址判断与调整 */
//	pData = PDU_SlaveDealCoilAddr(&addr, addr);
//	if (pData == NULL)
//	{
//		return MB_ILLEGAL_DATA_ADDR;
//	}
//
//	/* 请求处理 */	
//	pduTxBuf[0] = pduRxBuf[0];	/* 功能码 */
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
 函 数: PDU_SlaveDealWrSReg()
 功 能: 从机处理ModBus写单个寄存器命令
 参 数: pduRxBuf--PDU接收缓冲区; 
		pduTxBuf--PDU发送缓冲区; 
		pPduTxTotal--PDU需发送的字节总数; 
 返 回: 异常标志，若检测到错误，则返回具体的异常码，否则返回无异常标志
 其 它: 数值、地址是否正确，若错误则返回异常码(上层再处理异常响应PDU)
 		若正确，处理命令，并置响应PDU到发送缓冲区
		具体应用协议 
		06 00 01 AA 55 	写01为AA 55 
		06 00 01 AA 55 	回复相同
*******************************************************************************/
//static U08 PDU_SlaveDealWrSReg(const U08 *pduRxBuf, U08 *pduTxBuf, U16 *pPduTxTotal)
//{
//	U16 wrValue;			/* 写值 */
//	U16 addr;				/* 数据地址 */
//	U16 *pData;				/* 数据指针 */
//
//   	addr = MAKEWORD(pduRxBuf[1], pduRxBuf[2]);
//   	wrValue = MAKEWORD(pduRxBuf[3], pduRxBuf[4]);
//
//	/* 地址判断与调整 */
//	pData = PDU_SlaveDealHRegAddr(addr, addr);
//	if (pData == NULL)
//	{
//		return MB_ILLEGAL_DATA_ADDR;
//	}
//
//	/* 请求处理 */	
//	pduTxBuf[0] = pduRxBuf[0];	/* 功能码 */
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
 函 数: PDU_SlaveDealDiag()
 功 能: 从机处理ModBus回路诊断测试命令
 参 数: pduRxBuf--PDU接收缓冲区; 
		pduTxBuf--PDU发送缓冲区; 
		pPduTxTotal--PDU需发送的字节总数; 
 返 回: 异常标志，若检测到错误，则返回具体的异常码，否则返回无异常标志
 其 它: 若错误则返回异常码(上层再处理异常响应PDU)
 		若正确，处理命令，并置响应PDU到发送缓冲区
		具体应用协议(暂只支持0子功能码，原样返回，其它暂作为非法数据值) 
		功能号 测试功能(高-低) 写字数(高-低) 写字节数 测试数据(高-低)
		08		00	00			00	01			02		55	AA			
		08		00	00			00	01			02		55	AA		
 注 意:	若增加了其它子功能码处理(且与0子功能码协议不同)，则需更改SlaverMatchRx()	
*******************************************************************************/
//static U08 PDU_SlaveDealDiag(const U08 *pduRxBuf, U08 *pduTxBuf, U16 *pPduTxTotal)
//{
//	U16 subFunctionCode;	/* 子功能码 */
//	U16 quantity;			/* 数据个数 */
//	U08 bytes;				/* 数据字节数 */
//  	
//   	subFunctionCode = MAKEWORD(pduRxBuf[1], pduRxBuf[2]);
//
//	if (subFunctionCode == 0)	
//	{
//		/* 个数判断 */
//		quantity = MAKEWORD(pduRxBuf[3], pduRxBuf[4]);
//		bytes = pduRxBuf[5];
//		if ((quantity<1) || (quantity>MB_MAX_QUANTITY_DIAG)
//			|| (bytes != 2*quantity))
//		{
//			return MB_ILLEGAL_DATA_VALUE;
//		}
//
//		/* 请求处理 */	
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
 函 数: PDU_SlaveDealWrMCoils()
 功 能: 从机处理ModBus写多个线圈命令
 参 数: pduRxBuf--PDU接收缓冲区; 
		pduTxBuf--PDU发送缓冲区; 
		pPduTxTotal--PDU需发送的字节总数; 
 返 回: 异常标志，若检测到错误，则返回具体的异常码，否则返回无异常标志
 其 它: 判断个数、地址是否正确，若错误则返回异常码(上层再处理异常响应PDU)
 		若正确，处理命令，并置响应PDU到发送缓冲区
		具体应用协议 
		0F 00 00 00 06 01 6A 	从00开始发送6个位(1个字节)
		0F 00 00 00 06   
*******************************************************************************/
//static U08 PDU_SlaveDealWrMCoils(const U08 *pduRxBuf, U08 *pduTxBuf, U16 *pPduTxTotal)
//{
//	U16 quantity;			/* 数据个数 */
//	U16 beginAddr;			/* 起始地址 */
//	U16 endAddr;			/* 结束地址 */
//	U16 *pData;				/* 数据指针 */
//	U16 i;
//
//   	beginAddr = MAKEWORD(pduRxBuf[1], pduRxBuf[2]);
//   	quantity = MAKEWORD(pduRxBuf[3], pduRxBuf[4]);
//   	endAddr = beginAddr+quantity-1;
//
//	/* 个数判断 */
//	if ((quantity<1) || (quantity>MB_MAX_QUANTITY_WR_BITS)
//		|| (pduRxBuf[5] != (quantity%8==0 ? quantity/8 : quantity/8+1)))	
//	{
//		return MB_ILLEGAL_DATA_VALUE;
//	}
// 
//	/* 地址判断与调整 */
//	pData = PDU_SlaveDealCoilAddr(&beginAddr, endAddr);
//	if (pData == NULL)
//	{
//		return MB_ILLEGAL_DATA_ADDR;
//	}
//
//	/* 请求处理 */	
//	pduTxBuf[0] = pduRxBuf[0];	/* 功能码 */
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
 函 数: PDU_SlaveDealWrMRegs()
 功 能: 从机处理ModBus写多个寄存器命令
 参 数: pduRxBuf--PDU接收缓冲区; 
		pduTxBuf--PDU发送缓冲区; 
		pPduTxTotal--PDU需发送的字节总数; 
 返 回: 异常标志，若检测到错误，则返回具体的异常码，否则返回无异常标志
 其 它: 判断个数、地址是否正确，若错误则返回异常码(上层再处理异常响应PDU)
 		若正确，处理命令，并置响应PDU到发送缓冲区
		具体应用协议 
		10 00 01 00 02 04 11 22 33 44 从01写两个字，四个字节
		10 00 01 00 02 10 08  
*******************************************************************************/
//static U08 PDU_SlaveDealWrMRegs(const U08 *pduRxBuf, U08 *pduTxBuf, U16 *pPduTxTotal)
//{
//	U16 quantity;			/* 数据个数 */
//	U16 beginAddr;			/* 起始地址 */
//	U16 endAddr;			/* 结束地址 */
//	U16 *pData;				/* 数据指针 */
//
//   	beginAddr = MAKEWORD(pduRxBuf[1], pduRxBuf[2]);
//   	quantity = MAKEWORD(pduRxBuf[3], pduRxBuf[4]);
//   	endAddr = beginAddr+quantity-1;
//
//	/* 个数判断 */
//	if ((quantity<1) || (quantity>MB_MAX_QUANTITY_WR_WORDS)
//		|| (pduRxBuf[5] != 2*quantity))
//	{
//		return MB_ILLEGAL_DATA_VALUE;
//	}
//
//	/* 地址判断与调整 */
//	pData = PDU_SlaveDealHRegAddr(beginAddr, endAddr);
//	if (pData == NULL)
//	{
//		return MB_ILLEGAL_DATA_ADDR;
//	}
//	
//	/* 请求处理 */	
//	pduTxBuf[0] = pduRxBuf[0];	/* 功能码 */
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
 函 数: SlaveDealUserDefined()
 功 能: 从机处理ModBus用户自定义功能码:程序下载
 参 数: pduRxBuf--PDU接收缓冲区; 
		pduTxBuf--PDU发送缓冲区; 
		pPduTxTotal--PDU需发送的字节总数; 
 返 回: 异常标志，若检测到错误，则返回具体的异常码，否则返回无异常标志
 其 它: 判断与自定义的下载协议是否正确，若错误则返回异常码(上层再处理异常响应PDU)
 		若正确，处理命令，并置响应PDU到发送缓冲区
		具体应用协议 
1、询问应用程序是否能够进入BOOT
41 + 0000 + 00 + 10 + 20 +  "PUNPPLDOWNLOADERACROSSRS485ENTER"
能够进入BOOT:01 + 41 + 0000 + 0010 + 00 + 01 + CRC
不能进入BOOT:01 + 41 + 0000 + 0010 + 00 + 00 + CRC
2、命令后板进入BOOT
41 + 0010 + 00 + 10 + 20 +  "PUNPPLDOWNLOADERENTERBOOTFROMAPP"
如果能够进入BOOT:返回，直接进入BOOT
如果不能够进入BOOT:01 + 41 + 0010 + 0010 + 00 + 00 + CRC
*******************************************************************************/
//static U08 PDU_SlaveDealUserDef(const U08 *pduRxBuf, U08 *pduTxBuf, U16 *pPduTxTotal)
//{
//	U16 subFunctionCode;	/* 子功能码 */
//	static CONST I08 s_askMsg[] =      "PUNPPLDOWNLOADERACROSSRS485ENTER"; 
//	static CONST I08 s_downLoadMsg[] = "PUNPPLDOWNLOADERENTERBOOTFROMAPP"; 
//	//U16 *pData;				/* 数据指针 */
//
//   	subFunctionCode = MAKEWORD(pduRxBuf[1], pduRxBuf[2]);
//
//	/* 个数判断 */
//	if ((MAKEWORD(pduRxBuf[3], pduRxBuf[4]) != 16)
//		|| (pduRxBuf[5] != 32))
//	{
//		return MB_ILLEGAL_DATA_VALUE;
//	}
//
//	/* 子功能码的判断与处理 */
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
 函 数: MB_MasterMatchRx()
 功 能: 主机匹配接收数据
 参 数: pCOM--串口结构体指针; 
		pComMsg--通讯数据指针; 
 返 回: TRUE/FALSE--匹配成功(包括从机的异常响应)/匹配失败
 其 它: 从接收数据中匹配有效数据:地址匹配后，根据理论接收个数，检查帧校验；
 	 	可识别小于最少帧字节数、在正确数据前、在正确数据后的干扰；
 		且将匹配的数据前移至接收缓冲区首，以便后续数据处理。
*******************************************************************************/
static BOOL MB_MasterMatchRx(UART *pCOM, COMMSG *pComMsg)
{
	U16 idxBegin;			/* 查找数据匹配的开始索引号 */
	U16 idxEnd;				/* 查找数据匹配的结束索引号 */
	U16 leastRxBytes;		/* 至少接收字节数 */
	BOOL matchOk;			/* 匹配OK标识 */
	U16 matchOkIdx=0;		/* 匹配OK时的索引号(注，赋初值0防止编译警告) */
	U16 matchOkBytes=0;		/* 匹配OK时的字节数(注，赋初值0防止编译警告) */
	U16 true_rece_num = 0;
	
	/* 理论接收个数rxTotal在主机置发送缓冲区时已确定； 
	若收到的数据字节数小于至少接收字节数，可以确认是干扰 */
	leastRxBytes = MIN(EXCEP_RSP_BYTES, pCOM->rxTotal);
	if (pCOM->rxCnt < leastRxBytes)
	{
		pCOM->rxFrameType = RX_FRAME_LEN_SHORTAGE;
		pCOM->pRecord->fLenShortage++;
		return FALSE;
	}

	/* 从头依次开始寻找接收数据是否匹配，不需要查找完所有接收数据，
	当剩余数据个数小于至少接收字节数 时，就不需要继续往查找了。*/
	matchOk = FALSE;
	idxEnd = pCOM->rxCnt - leastRxBytes; 
	for (idxBegin=0; idxBegin<=idxEnd; idxBegin++)
	{
		U08 txFunCode;		/* 发送的功能码 */
		U08 rxFunCode;		/* 接收的功能码 */
		
		/* 地址不匹配，就继续下一个查找 */
		if (pCOM->rxBuf[idxBegin] != pComMsg->driverNo)
		{
			continue;
		}
		
		
		
		/* 功能码与crc的判断 */
		txFunCode = pCOM->txBuf[1];
		rxFunCode = pCOM->rxBuf[idxBegin+1];
		
		/* 0x17功能码特殊处理 */
		if (rxFunCode == MB_RDWR_MULTIPLE_REGISTERS)
		{
			true_rece_num = pCOM->rxBuf[idxBegin+2];	/* 字节数 */
			
			if (pCOM->rxTotal < true_rece_num + 5)		/* “5”为帧头帧尾数量 */
			{
				pComMsg->length_rd = true_rece_num / 2;
			}
			pCOM->rxTotal = true_rece_num + 5;	/* 0x17功能码，以从机送回的为准 */
		}
		
        else if (rxFunCode == MB_USER_DEFINED_EXIO_KLV)
        {
            true_rece_num = MAKEWORD(pCOM->rxBuf[idxBegin+2], pCOM->rxBuf[idxBegin+3]); /* 字节数 */
            pComMsg->length_rd = true_rece_num / 2;
            pCOM->rxTotal = true_rece_num + 6;  /* 0x48功能码，以从机送回的为准 */
        }
        
		if ((rxFunCode == txFunCode)
			&& (pCOM->rxCnt >= pCOM->rxTotal)
			&& CRC16Ok(&pCOM->rxBuf[idxBegin], pCOM->rxTotal))
		{/* 正常响应 */
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
		{/* 异常响应 */
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
		/* 匹配数据前移至缓冲区首 */
		if (matchOkIdx > 0)
		{
			U16 i;
			for (i=0; i<matchOkBytes; i++)
			{
				pCOM->rxBuf[i] = pCOM->rxBuf[matchOkIdx+i];
			}
		}

		/* 记录正确数据次数 */
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
	{/* 未找到匹配数据 */
		pCOM->rxFrameType = RX_FRAME_DATA_ERR;
		pCOM->pRecord->frameDataErr++;
		return FALSE;	
	}
}

/*******************************************************************************
 函 数: MB_SlaverMatchRx()
 功 能: 从机匹配接收数据
 参 数: pCOM--串口结构体指针; 
		ownAddr--从机自己的通讯设备地址; 
 返 回: TRUE/FALSE--匹配成功/匹配失败
 其 它: 从接收数据中匹配有效数据:找地址，根据功能码计算理论接收个数，检查帧校验；
 	 	可识别小于最少帧字节数、在正确数据前、在正确数据后的干扰；
 		且将匹配的数据前移至接收缓冲区首，以便后续数据处理。
*******************************************************************************/
static BOOL MB_SlaverMatchRx(UART *pCOM, const U08 ownAddr)
{
	U16 idxBegin;			/* 查找数据匹配的开始索引号 */
	U16 idxEnd;				/* 查找数据匹配的结束索引号 */
	BOOL matchOk;			/* 匹配OK标识 */
	U16 matchOkIdx=0;		/* 匹配OK时的索引号(注，赋初值0防止编译警告) */
	U16 theoreticBytes=0;	/* 根据功能码，理论需接收的字节数 */
	
	/* 若收到的数据字节数小于最少的帧字节数，可以确认是干扰 */
	if (pCOM->rxCnt < LEAST_FRAME_BYTES)
	{
//		if (debug[0]<3)	debug[0]=3;	
		pCOM->pRecord->fLenShortage++;
		return FALSE;
	}

	/* 从头依次开始寻找接收数据是否匹配，不需要查找完所有接收数据，
	当 剩余数据个数 小于 最少帧字节数 时，就不需要继续往查找了。*/
	matchOk = FALSE;
	idxEnd = pCOM->rxCnt - LEAST_FRAME_BYTES;
	for (idxBegin=0; idxBegin<=idxEnd; idxBegin++)
	{
		U08 rxAddr;			/* 接收的地址 */
		U08 rxFunCode;		/* 接收的功能码 */
		BOOL funCodeMatch;	/* 功能码匹配否 */

		/* 地址不匹配，就继续下一个查找 */
		rxAddr = pCOM->rxBuf[idxBegin];
		if ((rxAddr!=ownAddr) && (rxAddr!=ADDR_BROADCAST) && (rxAddr!=ADDR_BRO_FBUS))
		{
			continue;
		}

		/* 查找功能码与理论接收个数 */
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

		/* 功能码与接收个数不匹配，就继续下一个查找 */
		if ((!funCodeMatch) || (pCOM->rxCnt < (idxBegin+theoreticBytes))) 
		{
			continue;
		}

		/* crc校验通过后，匹配成功退出查找；否则继续下一个查找 */
		if (CRC16Ok(&pCOM->rxBuf[idxBegin], theoreticBytes))
		{
			matchOk = TRUE;
			matchOkIdx = idxBegin;
			break;
		}
	}

	if (matchOk)
	{
		/* 匹配数据前移至缓冲区首 */
		if (matchOkIdx > 0)
		{
			U16 i;
			for (i=0; i<theoreticBytes; i++)
			{
				pCOM->rxBuf[i] = pCOM->rxBuf[matchOkIdx+i];
			}
		}	

		/* 记录正确数据次数 */
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
	{/* 未找到匹配数据 */
//		if (debug[0]<4)	debug[0]=4;	
		pCOM->pRecord->frameDataErr++;
		return FALSE;		
	}
}

/*******************************************************************************
 函 数: MB_MasterTxReqPDU()
 功 能: 主机发送MODBUS请求PDU，不包括附加域(帧头的地址、帧尾的CRC校验)
 参 数: pComMsg--通讯数据指针; 
		pduTxBuf[]--PDU发送缓冲区; 
		pPduTxTotal--PDU需发送的字节数; 
		pPduRxTotal--PDU需接收的字节数; 
 返 回: 空
 其 它: 调用函数根据实际情况处理附加域(帧头的地址、帧尾的CRC校验)
*******************************************************************************/
BOOL MB_MasterTxReqPDU(const COMMSG *pComMsg, U08 pduTxBuf[], U16 *pPduTxTotal, U16 *pPduRxTotal)
{
	U16 i;
	U08 *p;
	
	pduTxBuf[0] = pComMsg->cmd;

	switch (pComMsg->cmd)
	{
		case MB_RD_COILS:					/* 读可读写位 */
		case MB_RD_DISCRETE_INPUTS:			/* 读只读位 */
			pduTxBuf[1] = HIBYTE(pComMsg->startAddr);
			pduTxBuf[2] = LOBYTE(pComMsg->startAddr);
			pduTxBuf[3] = HIBYTE(pComMsg->length_rd);
			pduTxBuf[4] = LOBYTE(pComMsg->length_rd);
			(*pPduTxTotal) = 5;
			(*pPduRxTotal) = (U08)(2+pComMsg->length_rd/8+(pComMsg->length_rd%8?1:0));
			break;
		case MB_RD_HOLDING_REGISTERS:		/* 读可读写字 */
		case MB_RD_INPUT_REGISTERS:			/* 读只读字 */
			pduTxBuf[1] = HIBYTE(pComMsg->startAddr);
			pduTxBuf[2] = LOBYTE(pComMsg->startAddr);
			pduTxBuf[3] = HIBYTE(pComMsg->length_rd);
			pduTxBuf[4] = LOBYTE(pComMsg->length_rd);
			(*pPduTxTotal) = 5;
			(*pPduRxTotal) = (U08)(2+pComMsg->length_rd*2);
			
			break;
		case MB_WR_SINGLE_COIL:				/* 写单个可读写位 */
			pduTxBuf[1] = HIBYTE(pComMsg->startAddr);
			pduTxBuf[2] = LOBYTE(pComMsg->startAddr);
			pduTxBuf[3] = (pComMsg->oneWrVal != 0) ? 0xff : 0;
			pduTxBuf[4] = 0;
			(*pPduTxTotal) = 5;
			(*pPduRxTotal) = 5;
			break;
		case MB_WR_SINGLE_REGISTER:			/* 写单个可读写字 */
			pduTxBuf[1] = HIBYTE(pComMsg->startAddr);
			pduTxBuf[2] = LOBYTE(pComMsg->startAddr);
			pduTxBuf[3] = HIBYTE(pComMsg->oneWrVal);
			pduTxBuf[4] = LOBYTE(pComMsg->oneWrVal);
			(*pPduTxTotal) = 5;
			(*pPduRxTotal) = 5;
			break;

		case MB_DIAGNOSTICS:				/* 邦普回路测试(从机原样返回) */
/*
	功能号 测试功能(高-低) 写字数(高-低) 写字节数 测试数据(高-低) 
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
		
		case MB_WR_MULTIPLE_COILS:			/* 写多个可读写位 */
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
				U16 inte = pComMsg->length_wr / 8;				/* 整数 */
				U16 rest = pComMsg->length_wr % 8;				/* 余数 */
				
				for (i=inte; i!=0; i--)
				{
					pduTxBuf[(*pPduTxTotal)++] = *p++;
				}
				/* 最后一个字节特殊处理 */
				if (rest != 0)
				{
					pduTxBuf[(*pPduTxTotal)++] = (*p++)&(((U16)1<<rest)-1);
				}
			}
			break;
			
		case MB_WR_MULTIPLE_REGISTERS:		/* 写多个可读写字 */
			pduTxBuf[1] = HIBYTE(pComMsg->startAddr);
	        pduTxBuf[2] = LOBYTE(pComMsg->startAddr);
			pduTxBuf[3] = HIBYTE(pComMsg->length_wr);
	        pduTxBuf[4] = LOBYTE(pComMsg->length_wr); 
			pduTxBuf[5] = LOBYTE(pComMsg->length_wr*2);
			SYS_CopyMemForModbusType(&pduTxBuf[6], (U08 *)pComMsg->pData_wr, pComMsg->length_wr*2);
			(*pPduTxTotal) = (U08)(6 + pComMsg->length_wr*2);
			(*pPduRxTotal) = 5;
			break;
		
		case MB_RDWR_MULTIPLE_REGISTERS:	/* 读写 */
			pduTxBuf[1] = LOBYTE(pComMsg->length_wr*2);
			SYS_CopyMemForModbusType(&pduTxBuf[2], (U08 *)pComMsg->pData_wr, pComMsg->length_wr*2);
			(*pPduTxTotal) = (U08)(2 + pComMsg->length_wr*2);
			(*pPduRxTotal) = (U08)(2 + pComMsg->length_rd*2);
			break;
			
        case MB_USER_DEFINED_EXIO_KLV:           /* KLV读写 */
            (*pPduTxTotal) = EK_mst_send_request(pduTxBuf, pComMsg->driverNo) + 1;
            (*pPduRxTotal) = 0; // 自协商长度,不定义
            break;
		default:
			return FALSE;
	}

	return TRUE;
}

/*******************************************************************************
 函 数: MB_MasterRxRspPDU()
 功 能: 主机接收从机的MODBUS响应PDU
 参 数: pComMsg--通讯数据指针; 
		pduRxBuf[]--PDU接收缓冲区; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void MB_MasterRxRspPDU(const COMMSG *pComMsg, const U08 pduRxBuf[])
{	
	/* 根据命令处理接收的数据 */
	switch (pComMsg->cmd) 
	{
		BOL bak_s_err_commu_expt_bit;
		case MB_RD_HOLDING_REGISTERS:
		case MB_RD_INPUT_REGISTERS:
    case MB_RDWR_MULTIPLE_REGISTERS:
			if (GET_BIT(fg_commu_err, pComMsg->driverNo))
			{
				//备份当前"通讯异常"故障的状态
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
 函 数: MB_MasterTx()
 功 能: 根据MODBUS协议，将队列节点数据转换成串口数据(如置发送缓冲区)
 参 数: pComMsg--通讯结点数据指针; 
		pCOM--串口结构体指针; 
 返 回: TRUE/FALSE--发送成功/发送失败
 其 它: 空
*******************************************************************************/
BOOL MB_MasterTx(const COMMSG *pComMsg, UART *pCOM)
{
	W16 crc;
	
	/* 发送请求PDU */
	if (!MB_MasterTxReqPDU(pComMsg, &pCOM->txBuf[1], &pCOM->txTotal, &pCOM->rxTotal))
	{
		return FALSE;
	}

	/* 发送帧头(设备地址) */
	pCOM->txBuf[0] = pComMsg->driverNo; 
	pCOM->txTotal++;
	pCOM->rxTotal++;

	/* 发送帧尾(crc) */
	crc.word = CRC16(&pCOM->txBuf[0], pCOM->txTotal);
	pCOM->txBuf[pCOM->txTotal++] = crc.byte.low;
	pCOM->txBuf[pCOM->txTotal++] = crc.byte.high;
	pCOM->rxTotal += 2;

	return  TRUE;
}

/*******************************************************************************
 函 数: MB_MasterRx()
 功 能: 主机接收协议
 参 数: pCOM--串口结构体指针; 
		pComMsg--通讯数据指针; 		
 返 回: TRUE/FALSE--接收成功/接收失败
 其 它: 空
*******************************************************************************/
BOOL MB_MasterRx(U08 u, UART *pCOM, COMMSG *pComMsg)
{
    u = u;					/* N0001 */
	/* 主机匹配接收数据失败 */
	if (!MB_MasterMatchRx(pCOM, pComMsg))
	{
		return FALSE;
	}

	/* 从机异常响应时 */
	if (pCOM->rxFrameType == RX_FRAME_EXCEP_RSP)
	{
//		return TRUE;
		return FALSE;	/* COM-V100A2-0 */
	}

//	if( u == COM_INVT               //NOTE-CXW 处理压机信息时打开 
//	    /* && fg_trans_control_cmd[pComMsg->driverNo]  */
//	    && pComMsg->cmd == MB_RD_INPUT_REGISTERS
//		/* && (pComMsg->startAddr >= ADDR_COMP_BGN 
//			 && pComMsg->startAddr < ADDR_COMP_END) */)//发送开机命令后丢弃一次压机信息的读取
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
 函 数: MB_SlaveRxReqTxRspPDU()
 功 能: 从机接受主机的请求PDU，发送响应PDU给主机
 参 数: pduRxBuf--PDU接收缓冲区; 
		pduTxBuf--PDU发送缓冲区; 
		pPduTxTotal--PDU需发送的字节总数; 
 返 回: 异常标志，若检测到错误，则返回具体的异常码，否则返回无异常标志
 其 它: 若请求PDU错误，则返回异常码;若正确，则处理请求PDU、置响应PDU到发送缓冲区
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
//	    	/* 一些正确的功能码，由于暂不支持，也作为非法功能码 */
//	    	exceptCode = MB_ILLEGAL_FUNCTION;				
//	    	break;
//	}
//
//	return exceptCode;
//}

/*******************************************************************************
 函 数: MB_SlaverRxTx()
 功 能: 从机接收发送协议
 参 数: pCOM--串口结构体指针; 
 返 回: TRUE/FALSE--接收成功/接收失败
 其 它: 广播通讯时，也返回TRUE、置发送缓冲区，在COM_SlaverProg()中再判断处理；
*******************************************************************************/
BOOL MB_SlaverRxTx(U08 u, UART *pCOM)
{
	W16 crc;

	/* 从机匹配接收数据失败 */
	if (!MB_SlaverMatchRx(pCOM, pCOM->ownAddr))
	{
		return FALSE;
	} 		

  	/* 处理请求PDU(若错误，则返回异常码;若正确，则处理命令、置响应PDU到发送缓冲区) */
	pCOM->txTotal = 0;//发送字节数先清零
	pCOM->exceptCode = com_slv_datanum_err(&pCOM->rxBuf[0]);    
	if (pCOM->exceptCode == MB_NO_ERR)
	{
        pCOM->exceptCode = MB_ILLEGAL_DATA_ADDR;//默认为非法地址
    	switch(u)
    	{     
#ifdef COM_4G
            case COM_4G:
#endif
            case COM_BMS:
    			if (TestProgStaus != TPS_NO_TEST_CMD || power_time < POWER_ON_INTO_TEST) 
    			{
    			    pCOM->exceptCode = com_slv_HMI(&pCOM->rxBuf[0], &pCOM->txBuf[0], &pCOM->txTotal);   // 测试用8寸大屏通讯
    			}
                else
                {
#ifndef USE_KIT_PARA_FAC
                    pCOM->exceptCode = com_slv_BMS(&pCOM->rxBuf[0], &pCOM->txBuf[0], &pCOM->txTotal);   // 对外的BMS通讯
#else
                    pCOM->exceptCode = com_slv_BMS_aux(&pCOM->rxBuf[0], &pCOM->txBuf[0], &pCOM->txTotal);	/* 只对参数管理助手 */
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
                    _CNT(SH_hmi_commu_succ);/* 注意:通讯成功后需要累加成功数，否则会被认为是通讯失败而切换成主口 */
                }
    			break;
#endif

    		default:
    			break;
    	}	
    }
	
	/* 处理异常响应PDU */
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

    /* 增加附加域:帧头的地址、帧尾的CRC校验 */
	pCOM->txBuf[0] = pCOM->rxBuf[0];
	crc.word = CRC16(&pCOM->txBuf[0], pCOM->txTotal);
	pCOM->txBuf[pCOM->txTotal++] = crc.byte.low;
	pCOM->txBuf[pCOM->txTotal++] = crc.byte.high;

    return  TRUE;
}
/************************************************
函数：modbus_time
功能：MODBUS相关计时
参数：
返回值：无
**************************************************/
void modbus_time(void)
{
//	U08 i;
	
	if (F_TIME_BASIC_SEC_BACK)
	{
//        #if CLOUD_USED
//		hmi_trans_type=HMI_DATA;	//每隔1s发送一次机组数据
//        #endif
	}
}

/*******************************************************************************
 函 数: USER_AllowEnterBoot()
 功 能: 是否能够进入BOOT，应用程序中的判断，待机状态下才能下载程序
 参 数: 空
 返 回: TRUE/FALSE--可以/禁止
 其 它: 空
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
 函 数: USER_EnterBoot()
 功 能: 处理进入boot
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void USER_EnterBoot(void)
{
	SYS_JumpToBoot();
}


