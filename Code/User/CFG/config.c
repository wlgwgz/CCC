/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: config.c
 �ļ�����: 
 ������  : 
 ��������: 
*******************************************************************************/
#define CFG_GLOBAL
#include "includes.h"

#define UPDATA_BY_NONE          0
#define UPDATA_BY_RS485         1
#define UPDATA_BY_SPIFLASH      2

/*******************************************************************************
 �� ��: SYS_JumpToApp()  
 �� ��: ��ת��boot
 �� ��: ��
 �� ��: 
 �� ��: ��
*******************************************************************************/
void SYS_JumpToBoot(void)
{
	#define BOOT_VECT_TAB	FALSH_ADDR_BASE				/* BOOT�����ж��������ַ */
	PVOIDFUNC ptrJump;		/* ��תָ�� */
	U32 spInit;				/* Stack Pointer ��ʼֵ */

    DISABLE_IRQ();
    PunpCloseAllDo();
	MCU_DeInit();
    MCU_SetBootUpdateBy(UPDATA_BY_RS485);
	
	spInit = (*(U32*)BOOT_VECT_TAB);
	ptrJump = (PVOIDFUNC)(*(U32*)(BOOT_VECT_TAB + 4));		/* Reset_Handler */
	__set_MSP(spInit);	/* ��ʼ���û� Stack Pointer */
	ptrJump();			/* ��ת��BOOT */
}


