/******************************************************************************

          ��Ȩ���� (C), 2001-2016, �����а��յ��Լ����������޹�˾

 ******************************************************************************
  �� �� ��   : Mcu_Timer.h
  �� �� ��   : ����
  ��    ��   : ��־��
  ��������   : 2018��3��20�����ڶ�
  ����޸�   :
  ��������   : Mcu_Timer.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��3��20�����ڶ�
    ��    ��   : ��־��
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef __MCU_TIMER_H__
#define __MCU_TIMER_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */
/*-----------------����ͷ�ļ� -----------------------------*/



/*-----------------�ⲿ����˵��----------------------------*/



/*-----------------�ⲿ����ԭ��˵��------------------------*/



/*-----------------�ڲ�����ԭ��˵��------------------------*/



/*-----------------ȫ�ֱ���--------------------------------*/



/*-----------------ģ�鼶����------------------------------*/



/*-----------------��������--------------------------------*/



/*-----------------�궨�� ---------------------------------*/




#define PWM_OUT_1        0  // PWM1���
#define PWM_OUT_2        1  // PWM2���
#define PWM_OUT_3        2  // PWM3���
#define PWM_OUT_MAX      3  // PWM�������


#define FREQ_IN_1        0  // Ƶ������1
#define FREQ_IN_2        1  // Ƶ������2
#define FREQ_IN_NUM      2  // Ƶ����������

#define PWC_IN_DUTY      0  // PWC����,��ռ�ձ�


extern void MCU_Cnt1UnderflowCb(void);
extern void MCU_Cnt2UnderflowCb(void);
extern U16 MCU_CntGetCounter(U08 cnt_num);
extern void MCU_CntStart(U08 cnt_num);
extern void MCU_CntStop(U08 cnt_num);
extern void MCU_CNT_Init(void);
extern U16 MCU_PwcGetMeasure(void);
extern U16 MCU_PwcGetCounter(void);
extern void MCU_PwcMeasureCompleteCb(void);
extern void MCU_PwcMeasureOverflowCb(void);
extern void MCU_PWC_Init(void);
extern void MCU_PWM_Init(void);
extern void MCU_SetPwm(U16 Frequency, U16 Duty, U16 Channel);
extern void MCU_TIM_Init(void);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __MCU_TIMER_H__ */
