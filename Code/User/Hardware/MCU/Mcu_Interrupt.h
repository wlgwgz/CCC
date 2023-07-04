/******************************************************************************

          ��Ȩ���� (C), 2001-2016, �����а��յ��Լ����������޹�˾

 ******************************************************************************
  �� �� ��   : MCU_Interrupt.h
  �� �� ��   : ����
  ��    ��   : zzp
  ��������   : 2021��4��27��
  ����޸�   :
  ��������   : MCU_Interrupt.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2021��4��27��
    ��    ��   : zzp
    �޸�����   : �����ļ�

******************************************************************************/

/*-----------------����ͷ�ļ� -----------------------------*/



/*-----------------�ⲿ����˵��----------------------------*/



/*-----------------�ⲿ����ԭ��˵��------------------------*/



/*-----------------�ڲ�����ԭ��˵��------------------------*/



/*-----------------ȫ�ֱ���--------------------------------*/



/*-----------------ģ�鼶����------------------------------*/



/*-----------------��������--------------------------------*/



/*-----------------�궨�� ---------------------------------*/



#ifndef __MCU_INTERRUPT_H__
#define __MCU_INTERRUPT_H__


/* �ж����ȼ�������Խ�͡����ȼ�Խ�ߣ���CPU֧��4�����ȼ�(0~3) */
//#define MCU_IT_PRI_SVD          0 /* FM33G0-HW-2 svd��Դ������ȼ���� */
//#define MCU_IT_PRI_UART0        1
//#define MCU_IT_PRI_UART1        1
//#define MCU_IT_PRI_UART2        1
//#define MCU_IT_PRI_UART3        1
//#define MCU_IT_PRI_UART4        1
//#define MCU_IT_PRI_UART5        1
//#define MCU_IT_PRI_TIM100US     2 /* 100us(0.1ms)��ʱ�� */
//#define MCU_IT_PRI_TIM1MS       3 /* 1ms��ʱ�� */

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

