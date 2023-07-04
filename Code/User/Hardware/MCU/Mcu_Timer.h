/******************************************************************************

          版权所有 (C), 2001-2016, 广州市邦普电脑技术开发有限公司

 ******************************************************************************
  文 件 名   : Mcu_Timer.h
  版 本 号   : 初稿
  作    者   : 周志鹏
  生成日期   : 2018年3月20日星期二
  最近修改   :
  功能描述   : Mcu_Timer.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2018年3月20日星期二
    作    者   : 周志鹏
    修改内容   : 创建文件

******************************************************************************/

#ifndef __MCU_TIMER_H__
#define __MCU_TIMER_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */
/*-----------------包含头文件 -----------------------------*/



/*-----------------外部变量说明----------------------------*/



/*-----------------外部函数原型说明------------------------*/



/*-----------------内部函数原型说明------------------------*/



/*-----------------全局变量--------------------------------*/



/*-----------------模块级变量------------------------------*/



/*-----------------常量定义--------------------------------*/



/*-----------------宏定义 ---------------------------------*/




#define PWM_OUT_1        0  // PWM1输出
#define PWM_OUT_2        1  // PWM2输出
#define PWM_OUT_3        2  // PWM3输出
#define PWM_OUT_MAX      3  // PWM输出总数


#define FREQ_IN_1        0  // 频率输入1
#define FREQ_IN_2        1  // 频率输入2
#define FREQ_IN_NUM      2  // 频率输入总数

#define PWC_IN_DUTY      0  // PWC输入,计占空比


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
