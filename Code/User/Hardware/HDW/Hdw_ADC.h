/******************************************************************************

          版权所有 (C), 2001-2016, 广州市邦普电脑技术开发有限公司

 ******************************************************************************
  文 件 名   : Mid_ADC.h
  版 本 号   : 初稿
  作    者   : zzp
  生成日期   : 2021年4月28日
  最近修改   :
  功能描述   : Mid_ADC.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2021年4月28日
    作    者   : zzp
    修改内容   : 创建文件

******************************************************************************/

#ifndef __MID_ADC_H__
#define __MID_ADC_H__

#ifdef  _MID_ADC_GBL_
    #define MID_ADC_EXT
#else
    #define MID_ADC_EXT extern
#endif

/*-----------------包含头文件 -----------------------------*/



/*-----------------外部变量说明----------------------------*/



/*-----------------外部函数原型说明------------------------*/



/*-----------------内部函数原型说明------------------------*/



/*-----------------全局变量--------------------------------*/



/*-----------------模块级变量------------------------------*/



/*-----------------常量定义--------------------------------*/

#define HDW_UPDATE_VOL_BY_ONE_INTV      0   // 一个周期更新一次
#define HDW_UPDATE_VOL_BY_ONE_TIMES     1   // 一个采样更新一次

enum {
    AD_IDX_NTC_1 = 0,      //  0 
    AD_IDX_NTC_2  ,      //  1 
    AD_IDX_NTC_3  ,      //  2 
    AD_IDX_NTC_4  ,      //  3 
    AD_IDX_NTC_5  ,      //  4 
    AD_IDX_NTC_6  ,      //  5 
    AD_IDX_NTC_7  ,      //  6 
    AD_IDX_NTC_8  ,      //  7 
    AD_IDX_NTC_9  ,      //  8 
    AD_IDX_NTC_10 ,      //  9 
    AD_IDX_NTC_11 ,      // 10 
    AD_IDX_NTC_12 ,      // 11 
    AD_IDX_MAX           // 12
};


/*-----------------宏定义 ---------------------------------*/
typedef struct tagAD_DATA
{
    U08 cnt[AD_IDX_MAX];               /* AD采样计数器 */
    U16 adUpdated[_DIVUCARRY(AD_IDX_MAX, 16)];    /* AD采样更新标志 */
	U16 adBuf[AD_IDX_MAX][MAX_AD_FILTER_NUM];          /* AD采样缓冲区 */
    I16 adTestDiff;                /* 运放零点误差Ad值，可为负数 */
} AD_DATA;                                 
MID_ADC_EXT AD_DATA AdCell;


#define AD_R10000       (10000UL*AD_MAX/(10000+10000))
#define AD_CAL_50       (50*AD_OVER_SAMPLE_NUM)
#define AD_CAL_18       (18*AD_OVER_SAMPLE_NUM)
#define AD_CAL_15       (15*AD_OVER_SAMPLE_NUM)
#define AD_CAL_3        (3*AD_OVER_SAMPLE_NUM)

#define ADCAL_STA_NULL  0   // 未校准
#define ADCAL_STA_SUCC  1   // 校准成功
#define ADCAL_STA_ERROR 2   // 校准失败
typedef struct {
    U08 sta;
    I16 val;
} ADCAL;
MID_ADC_EXT ADCAL AdCalibrate;  // AD校准值,10K理论AD之差


MID_ADC_EXT void HDW_Adcinit(void);
MID_ADC_EXT void HDW_AdcSample(void);
MID_ADC_EXT U16 HDW_AD_Filter(U16 adBuf[], BOOL useIntCtrl);
MID_ADC_EXT void HDW_AdcGetVrefVol(void);
MID_ADC_EXT BOOL HDW_AdcUpdateAdVol(U16* vol, U08 ad_idx, U08 by);
MID_ADC_EXT BOOL HDW_AdcUpdateAdVal(U16* ad, U08 ad_idx, U08 by);
MID_ADC_EXT U16 HDW_AdcCalVrefVol(U16 ad_vol);
MID_ADC_EXT U16 HDW_AdcCompensate(U16 ad);

#endif /* __MID_ADC_H__ */
