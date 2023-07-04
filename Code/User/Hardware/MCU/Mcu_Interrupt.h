/******************************************************************************

          版权所有 (C), 2001-2016, 广州市邦普电脑技术开发有限公司

 ******************************************************************************
  文 件 名   : MCU_Interrupt.h
  版 本 号   : 初稿
  作    者   : zzp
  生成日期   : 2021年4月27日
  最近修改   :
  功能描述   : MCU_Interrupt.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2021年4月27日
    作    者   : zzp
    修改内容   : 创建文件

******************************************************************************/

/*-----------------包含头文件 -----------------------------*/



/*-----------------外部变量说明----------------------------*/



/*-----------------外部函数原型说明------------------------*/



/*-----------------内部函数原型说明------------------------*/



/*-----------------全局变量--------------------------------*/



/*-----------------模块级变量------------------------------*/



/*-----------------常量定义--------------------------------*/



/*-----------------宏定义 ---------------------------------*/



#ifndef __MCU_INTERRUPT_H__
#define __MCU_INTERRUPT_H__


/* 中断优先级，数字越低、优先级越高，该CPU支持4级优先级(0~3) */
//#define MCU_IT_PRI_SVD          0 /* FM33G0-HW-2 svd电源监控优先级最高 */
//#define MCU_IT_PRI_UART0        1
//#define MCU_IT_PRI_UART1        1
//#define MCU_IT_PRI_UART2        1
//#define MCU_IT_PRI_UART3        1
//#define MCU_IT_PRI_UART4        1
//#define MCU_IT_PRI_UART5        1
//#define MCU_IT_PRI_TIM100US     2 /* 100us(0.1ms)定时器 */
//#define MCU_IT_PRI_TIM1MS       3 /* 1ms定时器 */

void MCU_IT_Init(void);
extern void Uart0RxIntCallback(void);
extern void Uart0TxIntCallback(void);
extern void Uart1RxIntCallback(void);
extern void Uart1TxIntCallback(void);
extern void Uart4RxIntCallback(void);
extern void Uart4TxIntCallback(void);
extern void Uart5RxIntCallback(void);
extern void Uart5TxIntCallback(void);
extern void Uart6RxIntCallback(void);
extern void Uart6TxIntCallback(void);
extern void LvdCallback(void);

#endif /* __MCU_INTERRUPT_H__ */

