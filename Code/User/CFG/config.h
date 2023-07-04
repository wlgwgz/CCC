/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: config.h
 �ļ�����: 
 ������  : 
 ��������: 
*******************************************************************************/

#ifndef __CONFIG_H__
#define __CONFIG_H__

#ifdef  CFG_GLOBAL
    #define CFG_EXT
#else
    #define CFG_EXT extern
#endif

#define PWM_OUT_CHANNEL1        PWM_OUT_1   //PWM1���
#define PWM_OUT_CHANNEL2        PWM_OUT_2   //PWM2���
#define PWM_OUT_CHANNEL3        PWM_OUT_3   //PWM3���

#define PWM_IN_CHANNEL1        FREQ_IN_1    //PWM1����
#define PWM_IN_CHANNEL2        FREQ_IN_2    //PWM2����

#define CHANNEL_FAN1            PWM_OUT_CHANNEL2
#define CHANNEL_FAN2            PWM_OUT_CHANNEL3


CFG_EXT void SYS_JumpToBoot(void);


#endif
