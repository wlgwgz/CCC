/******************************************************************************

          ��Ȩ���� (C), 2001-2014, �����а��յ��Լ����������޹�˾

 ******************************************************************************
  �� �� ��   : Mcu_def.h
  �� �� ��   : ����
  ��    ��   : // zzp v160a7
  ��������   : 2016��11��8�����ڶ�
  ����޸�   :
  ��������   : Mcu_def.h ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��11��8�����ڶ�
    ��    ��   : // zzp v160a7
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef __MCU_DEF_H__
#define __MCU_DEF_H__

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include "pdl_header.h"
#include "Types.h"
#include "Mcu_GPIO.h"
#include "Mcu_UART.h"
#include "Mcu_ADC.h"
#include "Mcu_Timer.h"
#include "Mcu_Interrupt.h"
/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/
/* Ӳ����Դ���� */
#define MAX_DI_NUM       7    /* ������������� */
#define MAX_HDI_NUM      5    /* ǿ�翪����������� */
#define MAX_HDI			 (_DIVUCARRY(MAX_HDI_NUM, 16))
#define MAX_DI_TOTAL_NUM	(MAX_HDI_NUM+MAX_DI_NUM)
#define MAX_DI			 (_DIVUCARRY(MAX_DI_TOTAL_NUM, 16))
#define MAX_DO_NUM       18    /* ������������� */ 
#define MAX_DO			 (_DIVUCARRY(MAX_DO_NUM, 16))

#define MAX_TEMP_NUM     AD_IDX_MAX   /* �����¶�ͨ���� 12 */
#define MAX_AI_NUM       0    /* ģ�������� */ 
#define MAX_CUR_NUM      0    /* �������� */ 
#define MAX_MOTOR_NUM    0    /* ������� */ 

#define MAX_SW1_BIT      2    /* 2λ����λ�� 2 */
#define MAX_SW2_BIT      4    /* 4λ����λ�� 4 */
#define MAX_SW4_BIT      4    /* 4λ����λ�� 4 */
#define MAX_SW5_BIT      4    /* 4λ����λ�� 4 */
#define MAX_SW_BIT       (MAX_SW1_BIT+MAX_SW2_BIT+MAX_SW4_BIT+MAX_SW5_BIT) // 14 ��������
#define MAX_SW			 (_DIVUCARRY(MAX_SW_BIT, 16))
#define MAX_SWPL_BIT     4    /* 4λ����λ�� 4 */
#define MAX_SWPL		 (_DIVUCARRY(MAX_SWPL_BIT, 16))
#define MAX_PWM          1    /* PWM��������� */
#define MAX_2PWM         2    /* PWM��������� */
/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/



// ϵͳʱ��Ƶ�ʶ���
#define SYSCLKCFG   (40)                    // MHz
#define SYSCLKFREQ  (SYSCLKCFG*1000000UL)   // Hz
#if __HCLK != SYSCLKFREQ
    #error "ʱ�����ô���"
#endif

#define SYS_CLOCK   SYSCLKFREQ           /* ϵͳʱ�� */


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

