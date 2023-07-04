/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: config.c
 文件描述: 
 创建人  : 
 创建日期: 
*******************************************************************************/
#define CFG_GLOBAL
#include "includes.h"

#define UPDATA_BY_NONE          0
#define UPDATA_BY_RS485         1
#define UPDATA_BY_SPIFLASH      2

/*******************************************************************************
 函 数: SYS_JumpToApp()  
 功 能: 跳转到boot
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
void SYS_JumpToBoot(void)
{
	#define BOOT_VECT_TAB	FALSH_ADDR_BASE				/* BOOT程序中断向量表地址 */
	PVOIDFUNC ptrJump;		/* 跳转指针 */
	U32 spInit;				/* Stack Pointer 初始值 */

    DISABLE_IRQ();
    PunpCloseAllDo();
	MCU_DeInit();
    MCU_SetBootUpdateBy(UPDATA_BY_RS485);
	
	spInit = (*(U32*)BOOT_VECT_TAB);
	ptrJump = (PVOIDFUNC)(*(U32*)(BOOT_VECT_TAB + 4));		/* Reset_Handler */
	__set_MSP(spInit);	/* 初始化用户 Stack Pointer */
	ptrJump();			/* 跳转到BOOT */
}


