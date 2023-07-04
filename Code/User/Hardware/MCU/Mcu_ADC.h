/******************************************************************************

          版权所有 (C), 2001-2014, 广州市邦普电脑技术开发有限公司

 ******************************************************************************
  文 件 名   : Mcu_ADC.h
  版 本 号   : 初稿
  作    者   : // zzp v160a7
  生成日期   : 2016年11月7日星期一
  最近修改   :
  功能描述   : Mcu_ADC.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2016年11月7日星期一
    作    者   : // zzp v160a7
    修改内容   : 创建文件

******************************************************************************/
#ifndef __MCU_ADC_H__
#define __MCU_ADC_H__


/* ADC通道定义 */ 
#define ADC_CH_NTC_1        (1UL <<  0) 
#define ADC_CH_NTC_2        (1UL <<  1) 
#define ADC_CH_NTC_3        (1UL <<  2) 
#define ADC_CH_NTC_4        (1UL <<  3) 
#define ADC_CH_NTC          (1UL <<  7)		/* 需进一步确认 */
//#define ADC_CH_NTC_5        (1UL <<  4) 
//#define ADC_CH_NTC_6        (1UL <<  5) 
//#define ADC_CH_NTC_7        (1UL <<  6) 
//#define ADC_CH_NTC_8        (1UL <<  7) 
//#define ADC_CH_NTC_9        (1UL <<  8) 
//#define ADC_CH_NTC_10       (1UL <<  9) 
//#define ADC_CH_NTC_11       (1UL <<  10) 
//#define ADC_CH_NTC_12       (1UL <<  11)






extern void MCU_ADC_ClrCplt(void);
extern void MCU_ADC_Enable(void);
extern void MCU_ADC_EXTE_Init(void);
extern void MCU_ADC_Init(void);
extern BOOL MCU_ADC_IsCplt(void);
extern U16 MCU_ADC_ReadAd(void);
extern void MCU_ADC_SelectCh(U32 adc_ch);
extern void MCU_ADC_Start(void);
extern uint32_t MCU_ADC_VoltageCalc(uint32_t ADCData);

#endif /* __MCU_ADC_H__ */

