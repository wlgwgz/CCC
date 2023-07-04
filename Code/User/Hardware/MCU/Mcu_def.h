/******************************************************************************

          版权所有 (C), 2001-2014, 广州市邦普电脑技术开发有限公司

 ******************************************************************************
  文 件 名   : Mcu_def.h
  版 本 号   : 初稿
  作    者   : // zzp v160a7
  生成日期   : 2016年11月8日星期二
  最近修改   :
  功能描述   : Mcu_def.h 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2016年11月8日星期二
    作    者   : // zzp v160a7
    修改内容   : 创建文件

******************************************************************************/

#ifndef __MCU_DEF_H__
#define __MCU_DEF_H__

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "pdl_header.h"
#include "Types.h"
#include "Mcu_GPIO.h"
#include "Mcu_UART.h"
#include "Mcu_ADC.h"
#include "Mcu_Timer.h"
#include "Mcu_Interrupt.h"
/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/
/* 硬件资源定义 */
#define MAX_DI_NUM       7    /* 开关量输入个数 */
#define MAX_HDI_NUM      5    /* 强电开关量输入个数 */
#define MAX_HDI			 (_DIVUCARRY(MAX_HDI_NUM, 16))
#define MAX_DI_TOTAL_NUM	(MAX_HDI_NUM+MAX_DI_NUM)
#define MAX_DI			 (_DIVUCARRY(MAX_DI_TOTAL_NUM, 16))
#define MAX_DO_NUM       18    /* 开关量输出个数 */ 
#define MAX_DO			 (_DIVUCARRY(MAX_DO_NUM, 16))

#define MAX_TEMP_NUM     AD_IDX_MAX   /* 最大的温度通道数 12 */
#define MAX_AI_NUM       0    /* 模拟量个数 */ 
#define MAX_CUR_NUM      0    /* 电流个数 */ 
#define MAX_MOTOR_NUM    0    /* 电机个数 */ 

#define MAX_SW1_BIT      2    /* 2位拨码位数 2 */
#define MAX_SW2_BIT      4    /* 4位拨码位数 4 */
#define MAX_SW4_BIT      4    /* 4位拨码位数 4 */
#define MAX_SW5_BIT      4    /* 4位拨码位数 4 */
#define MAX_SW_BIT       (MAX_SW1_BIT+MAX_SW2_BIT+MAX_SW4_BIT+MAX_SW5_BIT) // 14 拨码输入
#define MAX_SW			 (_DIVUCARRY(MAX_SW_BIT, 16))
#define MAX_SWPL_BIT     4    /* 4位拨盘位数 4 */
#define MAX_SWPL		 (_DIVUCARRY(MAX_SWPL_BIT, 16))
#define MAX_PWM          1    /* PWM输入输出对 */
#define MAX_2PWM         2    /* PWM输入输出对 */
/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/



// 系统时钟频率定义
#define SYSCLKCFG   (40)                    // MHz
#define SYSCLKFREQ  (SYSCLKCFG*1000000UL)   // Hz
#if __HCLK != SYSCLKFREQ
    #error "时钟配置错误"
#endif

#define SYS_CLOCK   SYSCLKFREQ           /* 系统时钟 */


#define ASM_NOP()           (__NOP())
#define ENTER_CRITICAL()    (__disable_irq())
#define EXIT_CRITICAL()     (__enable_irq())
#define DISABLE_IRQ()       (__disable_irq())
#define ENABLE_IRQ()        (__enable_irq())

#define     WDG_Refresh()   MCU_WDG_Refresh()
extern void MCU_DelayUS(U16 usec);
extern void MCU_WDG_Refresh(void);
extern void MCU_VbatDeinit(void);
extern void MCU_DeInit(void);
extern void MCU_SetBootUpdateBy(U08 typ);
#endif /* __MCU_DEF_H__ */

