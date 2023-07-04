/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: CRC16.H
 文件描述: 
 创建人  : 李靖禹
 创建日期: 2009.10.10
*******************************************************************************/
#ifndef CRC16_H
#define CRC16_H

#ifdef  CRC16_GLOBAL
	#define CRC16_EXT
#else
	#define CRC16_EXT extern
#endif

/* 兼容LY系列程序的写法 */
#define	crc_check(p, l) CRC16Ok((p), (l))

unsigned short CRC16(const unsigned char *puchMsg, unsigned short usDataLen);
BOOL CRC16Ok(unsigned char *pData, unsigned short lenIncCrc);

#endif
