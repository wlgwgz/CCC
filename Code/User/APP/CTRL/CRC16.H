/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: CRC16.H
 �ļ�����: 
 ������  : ���
 ��������: 2009.10.10
*******************************************************************************/
#ifndef CRC16_H
#define CRC16_H

#ifdef  CRC16_GLOBAL
	#define CRC16_EXT
#else
	#define CRC16_EXT extern
#endif

/* ����LYϵ�г����д�� */
#define	crc_check(p, l) CRC16Ok((p), (l))

unsigned short CRC16(const unsigned char *puchMsg, unsigned short usDataLen);
BOOL CRC16Ok(unsigned char *pData, unsigned short lenIncCrc);

#endif