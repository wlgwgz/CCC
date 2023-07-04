/******************************************************************************

          版权所有 (C), 2001-2014, 广州市邦普电脑技术开发有限公司

 ******************************************************************************
  文 件 名   : Mcu_GPIO.h
  版 本 号   : 初稿
  作    者   : 
  生成日期   : 2016年11月8日星期二
  最近修改   :
  功能描述   : Mcu_GPIO.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2016年11月8日星期二
    作    者   : 
    修改内容   : 创建文件

******************************************************************************/



#ifndef __MCU_GPIO_H__
#define __MCU_GPIO_H__

#include "User_io.h"	/* N0004 */

#define GPIO_SetOutputBit(PinName) Gpio1pin_Put(PinName, 1)
#define GPIO_ClrOutputBit(PinName) Gpio1pin_Put(PinName, 0)

#define GPIO_ReadInputBit(PinName) Gpio1pin_Get(PinName)

// 使用GPIO的名称,如:
// #define OUT_PIN_00              GPIO1PIN_P3F
// WriteGpioPin(OUT_PIN_00, 1);
// WriteGpioPin(OUT_PIN_00, 0);
#define WriteGpioPin(gp, sta)   Gpio1pin_Put(gp, sta)
#define ReadGpioPin(gp)         Gpio1pin_Get(gp)




/* NTC通道选通地址IO */
#define IO_NTC_CH_A  GPIO1PIN_P14
#define IO_NTC_CH_B  GPIO1PIN_P15
#define IO_NTC_CH_C  GPIO1PIN_P16
//#define IO_NTC_CH_EN   IO_P6_2

/* NTC放大倍数地址IO */
//#define NTC_PGA_A    IO_P2_11
//#define NTC_PGA_B    IO_P2_12


/* LED输出IO */
#define LED_OUT_RED  GPIO1PIN_P04
#define LED_OUT_ORG  GPIO1PIN_P02

/* 继电器输出IO */
#define IO_OUT_00    GPIO1PIN_P31    
#define IO_OUT_01    GPIO1PIN_P4B
#define IO_OUT_02    GPIO1PIN_P49
#define IO_OUT_03    GPIO1PIN_P48
#define IO_OUT_05    GPIO1PIN_P3D
#define IO_OUT_06    GPIO1PIN_P3E
#define IO_OUT_07    GPIO1PIN_P3F
#define IO_OUT_08    GPIO1PIN_P47
#define IO_OUT_09    GPIO1PIN_P46
#define IO_OUT_10    GPIO1PIN_P45
#define IO_OUT_11    GPIO1PIN_P44
#define IO_OUT_12    GPIO1PIN_P3C
#define IO_OUT_13    GPIO1PIN_P3B
#define IO_OUT_14    GPIO1PIN_P3A
#define IO_OUT_15    GPIO1PIN_P39
#define IO_OUT_16    GPIO1PIN_P4E
#define IO_OUT_17    GPIO1PIN_P4D
#define IO_OUT_18    GPIO1PIN_P4C


/* 步进电机输出IO */
//#define IO_STEP_01    GPIO1PIN_P3E
//#define IO_STEP_02    GPIO1PIN_P3D
//#define IO_STEP_03    GPIO1PIN_P3C
//#define IO_STEP_04    GPIO1PIN_P3B
//#define IO_STEP_05    GPIO1PIN_P3A
//#define IO_STEP_06    GPIO1PIN_P39
//#define IO_STEP_07    GPIO1PIN_P52
//#define IO_STEP_08    GPIO1PIN_P51

/* 强电开关量输入IO */
#define IO_HIN_01    GPIO1PIN_P30   
#define IO_HIN_02    GPIO1PIN_P52
#define IO_HIN_03    GPIO1PIN_P51
#define IO_HIN_04    GPIO1PIN_P0C
#define IO_HIN_05    GPIO1PIN_P23


/* 开关量输入 74HC165并转串输入 */
#define IO_HC165_SH     GPIO1PIN_P1A
#define IO_HC165_CLK    GPIO1PIN_P19
#define IO_HC165_ISH    GPIO1PIN_P1B
#define IN_HC165_SH_L()     WriteGpioPin(IO_HC165_SH , 0)
#define IN_HC165_SH_H()     WriteGpioPin(IO_HC165_SH , 1)
#define IN_HC165_CLK_L()    WriteGpioPin(IO_HC165_CLK ,0)
#define IN_HC165_CLK_H()    WriteGpioPin(IO_HC165_CLK ,1)
#define IN_HC165_QH_ISH()   ReadGpioPin(IO_HC165_ISH)

/* 拨码输入IO */	/* N0004 */
//#define SW_IN_01    INIO_P5_15
//#define SW_IN_02    INIO_P5_13
//#define SW_IN_03    INIO_P5_11
//#define SW_IN_04    INIO_P5_10
//#define SW_IN_05    INIO_P5_9
//#define SW_IN_06    INIO_P5_5

/* 硬件型号IO */	/* N0004 */
//#define HW_IN_01    INIO_P15_0
//#define HW_IN_02    INIO_P7_2

/* 相序输入IO */	/* N0004 */
//#define PH_IN_01    GPIO1PIN_P4D
//#define PH_IN_02    GPIO1PIN_P4E

/* EEPROM */
#define IIC_SCL_OUT     GPIO1PIN_P33
#define IIC_SDA_OUT     GPIO1PIN_P32
#define IIC_SDA_IN      GPIO1PIN_P32
#define IIC_CFG_SDA_OUT()   Gpio1pin_InitOut(GPIO1PIN_P32, ASM_NOP())
#define IIC_CFG_SDA_IN()    Gpio1pin_InitIn(GPIO1PIN_P32, ASM_NOP())

#define IO_UART0_TR  GPIO1PIN_P50
#define IO_UART4_TR  GPIO1PIN_P80
#define IO_UART5_TR  GPIO1PIN_P60
#define IO_UART6_TR  GPIO1PIN_P81

/* PWC输入IO */
#define IO_PWC_IN    GPIO1PIN_P0B

/* 4G模块电源控制 */
#define IO_4G_POWER GPIO1PIN_P63
#define IO_4G_RESET GPIO1PIN_P00

//LED灯
#define LED_DARK     0
#define LED_LIGHT    1

#define LEDY_ON()   WriteGpioPin(LED_OUT_ORG, 0)
#define LEDY_OFF()  WriteGpioPin(LED_OUT_ORG, 1)
#define LEDR_ON()   WriteGpioPin(LED_OUT_RED, 0)
#define LEDR_OFF()  WriteGpioPin(LED_OUT_RED, 1)
#define ALLLEDON()  (LEDY_ON(),LEDR_ON())
#define ALLLEDOFF() (LEDY_OFF(),LEDR_OFF())

extern void MCU_GPIO_Init(void);
extern void MCU_CfgIICSda(U08 sta);
extern void MCU_GetHDi(U16 HDi_buf[]);
extern BOL MCU_GetIICSda(void);
extern U08 MCU_GetPhase(void);
extern U16 MCU_GetSwitch(void);
extern U08 MCU_GetHardware(void);	/* N0004 */
extern U08 MCU_GetPhase(void);		/* N0004 */
extern U16 MCU_GetSwPlate(void);
extern void MCU_ClrAllDo(void);
extern void MCU_SetDo(U16 Do_buf[]);
extern void MCU_SetEev1Step(U08 addr);
extern void MCU_SetEev2Step(U08 addr);
extern void MCU_SetIICSck(U08 sta);
extern void MCU_SetIICSda(U08 sta);
extern void MCU_SetNtcChannel(U08 addr);
extern void MCU_SetNtcPga(U08 addr);
extern BOL MCU_GetPwcIn(void);
extern void MCU_GetInput74HC165(U16 In_buf[]);
extern void MCU_SetLed(U16 Led, U08 Sta);
extern void MCU_SetEevStep(U08 num, U08 step);
extern void MCU_Set4GPower(U08 sta);
extern void MCU_Set4GReset(U08 sta);
#endif /* __MCU_GPIO_H__ */
