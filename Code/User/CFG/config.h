/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: config.h
 文件描述: 
 创建人  : 
 创建日期: 
*******************************************************************************/

#ifndef __CONFIG_H__
#define __CONFIG_H__

#ifdef  CFG_GLOBAL
    #define CFG_EXT
#else
    #define CFG_EXT extern
#endif

#define PWM_OUT_CHANNEL1        PWM_OUT_1   //PWM1输出
#define PWM_OUT_CHANNEL2        PWM_OUT_2   //PWM2输出
#define PWM_OUT_CHANNEL3        PWM_OUT_3   //PWM3输出

#define PWM_IN_CHANNEL1        FREQ_IN_1    //PWM1输入
#define PWM_IN_CHANNEL2        FREQ_IN_2    //PWM2输入

#define CHANNEL_FAN1            PWM_OUT_CHANNEL2
#define CHANNEL_FAN2            PWM_OUT_CHANNEL3


CFG_EXT void SYS_JumpToBoot(void);


#endif
