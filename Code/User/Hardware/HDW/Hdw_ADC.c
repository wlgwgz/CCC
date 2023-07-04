/******************************************************************************

          版权所有 (C), 2001-2016, 广州市邦普电脑技术开发有限公司

 ******************************************************************************
  文 件 名   : Mid_ADC.c
  版 本 号   : 初稿
  作    者   : zzp
  生成日期   : 2021年4月28日
  最近修改   :
  功能描述   : ADC采样模块
  函数列表   :
              HDW_Adcinit
              HDW_AdcSample
              HDW_AdcSampleAppend
  修改历史   :
  1.日    期   : 2021年4月28日
    作    者   : zzp
    修改内容   : 创建文件

******************************************************************************/
#define _MID_ADC_GBL_
/*-----------------包含头文件 -----------------------------*/
#include "includes.h"


/*-----------------外部变量说明----------------------------*/



/*-----------------外部函数原型说明------------------------*/



/*-----------------内部函数原型说明------------------------*/



/*-----------------全局变量--------------------------------*/



/*-----------------模块级变量------------------------------*/



/*-----------------常量定义--------------------------------*/



/*-----------------宏定义 ---------------------------------*/









enum
{
	ADC_CONVER_STEP_INIT = 0,   // 初始化
	ADC_CONVER_STEP_START,      // 启动转换
	ADC_CONVER_STEP_SAMPLE,     // 采样    
	ADC_CONVER_STEP_SEL_CH      // 选择通道
};

typedef struct tagHDW_AD_CH{
    U08 ad_idx;     // AD编号索引
    U08 ntc_pg;     // NTC放大4051地址
    U08 ntc_addr;   // NTC选通4051地址
    U08 cur_addr;   // 电流选通4051地址    
    U32 adc_ch;     // 芯片ADC通道
}HDW_AD_CH;


const HDW_AD_CH HdwAdTbl[AD_IDX_MAX] = {
    /* ad_idx           ntc_pg,  ntc_addr, cur_addr, adc_ch   */
    {AD_IDX_NTC_1 ,     0,       0,        0,        ADC_CH_NTC_1},     //  0 
    {AD_IDX_NTC_2 ,     0,       0,        0,        ADC_CH_NTC_2},     //  1 
    {AD_IDX_NTC_3 ,     0,       0,        0,        ADC_CH_NTC_3},     //  2 
    {AD_IDX_NTC_4 ,     0,       0,        0,        ADC_CH_NTC_4},     //  3 
    {AD_IDX_NTC_5 ,     0,       0,        0,        ADC_CH_NTC},       //  4 
    {AD_IDX_NTC_6 ,     0,       1,        0,        ADC_CH_NTC},       //  5 
    {AD_IDX_NTC_7 ,     0,       2,        0,        ADC_CH_NTC},       //  6 
    {AD_IDX_NTC_8 ,     0,       3,        0,        ADC_CH_NTC},       //  7 
    {AD_IDX_NTC_9 ,     0,       4,        0,        ADC_CH_NTC},       //  8 
    {AD_IDX_NTC_10,     0,       5,        0,        ADC_CH_NTC},       //  9 
    {AD_IDX_NTC_11 ,    0,       6,        0,        ADC_CH_NTC},       //  10
    {AD_IDX_NTC_12,     0,       7,        0,        ADC_CH_NTC},       //  11 
};


void HDW_Adcinit(void)
{
    ARR2SET(AdCell.adBuf, 0xFFFF);
    if (!eprom_rd_near_crc(&AdCalibrate, EEPROMADDR_ADC_CALIBRATE, sizeof(AdCalibrate)))
    {
        AdCalibrate.sta = ADCAL_STA_NULL;
        AdCalibrate.val = 0;
    }

    if (AdCalibrate.sta != ADCAL_STA_SUCC
        || AdCalibrate.val > AD_CAL_15
        || AdCalibrate.val < -AD_CAL_15)
    {
        AdCalibrate.sta = ADCAL_STA_NULL;
        AdCalibrate.val = 0;
    }
}



/*******************************************************************************
 函 数: HDW_AdcFilter()
 功 能: AD滤波处理函数
 参 数: adBuf--数据缓存首地址; 
        useIntCtrl--使用中断控制否(关、开中断)，
            为解决中断与非中断的共享变量adBuf冲突时，useIntCtrl为TRUE 
            在中断中调用该函数时，useIntCtrl为FALSE; 
 返 回: 滤波值
 其 它: 去掉最小值、最大值，然后求平均值；数据滤波个数固定为MAX_AD_FILTER_NUM
*******************************************************************************/
U16 HDW_AdcFilter(U16 adBuf[], BOOL useIntCtrl)
{
    U08 i;
    U16 adMin, adMax;
#ifdef USE_AD_FILTER2
	U16 adMin2, adMax2;
	U08 posMin=0, posMax=0;
#endif /* USE_AD_FILTER2 */
    U32 adSum;
    U16 adTmp[MAX_AD_FILTER_NUM];
    U16 *pAd;

    if (useIntCtrl)
    {
        ENTER_CRITICAL();
            for (i=0; i<MAX_AD_FILTER_NUM; i++)
            {
                adTmp[i] = adBuf[i];
            }
        EXIT_CRITICAL();
        pAd = adTmp;
    }
    else
    {
        pAd = adBuf;
    }

	/* 查找最小值、最大值，和累加 */
    adMin = pAd[0];
    adMax = pAd[0]; 
	adSum = pAd[0];
    for (i=1; i<MAX_AD_FILTER_NUM; i++)
    {
        adSum += pAd[i];
        if (adMin > pAd[i]) 
        {
            adMin = pAd[i];
#ifdef USE_AD_FILTER2
			posMin = i;
#endif /* USE_AD_FILTER2 */
        } 
		
        if (adMax < pAd[i]) 
        {
            adMax = pAd[i];
#ifdef USE_AD_FILTER2
			posMax = i;
#endif /* USE_AD_FILTER2 */
        }   
    }
	
#ifdef USE_AD_FILTER2
	/* 查找第二小值、第二大值 */
    adMin2 = 0xffff;
    adMax2 = 0; 
    for (i=0; i<MAX_AD_FILTER_NUM; i++)     
    {
        //adSum += pAd[i];
        if ((adMin2 > pAd[i]) && (i != posMin)) 
        {
            adMin2 = pAd[i];
        } 
		
        if ((adMax2 < pAd[i]) && (i != posMax)) 
        {
            adMax2 = pAd[i];
        }   
    }
	
	/* 去掉2个小值、2个大值，求平均值 */
    adSum -= adMin; 
	adSum -= adMin2;
    adSum -= adMax;
	adSum -= adMax2;
    adSum /= (MAX_AD_FILTER_NUM - 4);
#else
	/* 去掉最小值、最大值，求平均值 */
    adSum -= adMin; 
    adSum -= adMax;
    adSum /= (MAX_AD_FILTER_NUM - 2);	
#endif /* USE_AD_FILTER2 */

    return (U16)adSum;
} 


/*****************************************************************************
函  数 : HDW_AdcSampleAppend()
功  能 : 新增一个采样点
参  数 : U16* adBuf : 
         U16 ad     : 
         U08 num    :
返  回 : 无
其  他 : 无
*****************************************************************************/
void HDW_AdcSampleAppend(U16* adBuf, U16 ad, U08 num)
{
    U08 i=0;

    // 第一次值
    if (adBuf[0] == 0xFFFF)
    {
        for (i=0; i<num; i++)
        {
            adBuf[i] = ad;
        }
    }
    else
    {
        for (i=0; i<num-1; i++)
        {
            adBuf[i] = adBuf[i+1];
        }

        adBuf[i] = ad;
    }
}


/*****************************************************************************
函  数 : HDW_AdcSample()
功  能 : AD采样,所有AD都在一个函数中处理,减少各个模块的耦合性
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
void HDW_AdcSample(void)
{
	
    static U08 adc_conver_step=ADC_CONVER_STEP_INIT;
    static U08 smp_cnt;
    static U16 adc_over_smp;    
    static U08 ad_tbl_idx;
    static U08 timeout_cnt;
    static U08 timeout_dly;
    BOOL is_next_idx = FALSE;
    U16 ad;

    switch (adc_conver_step)
    {
        //3 初始化,使能AD等
        default:
        case ADC_CONVER_STEP_INIT:
            MCU_ADC_Enable();
            ad_tbl_idx = 0;
            
            adc_conver_step = ADC_CONVER_STEP_SEL_CH;
            break;

        //3 启动ADC转换
        case ADC_CONVER_STEP_START:
            if (timeout_dly != 0)   {timeout_dly--;  break;}
            timeout_cnt = 0;
            MCU_ADC_Start();
            adc_conver_step = ADC_CONVER_STEP_SAMPLE;
            break;
			
        //3 转换,包括丢弃和过采样
        case ADC_CONVER_STEP_SAMPLE:
            if (MCU_ADC_IsCplt())
            {
                ad = MCU_ADC_ReadAd();  
                MCU_ADC_ClrCplt();
                if (smp_cnt >= AD_UNUSE_SAMPLE_NUM) // 丢弃前几次,保留后面的
                {
                    adc_over_smp += ad;
                }
                if (++smp_cnt >= MAX_AD_CONVER_NUM) // 过采样完,保留AD值
                {
                    U08 ad_idx = HdwAdTbl[ad_tbl_idx].ad_idx;
                    HDW_AdcSampleAppend(AdCell.adBuf[ad_idx], adc_over_smp, MAX_AD_FILTER_NUM);
                    SETBIT_N(AdCell.adUpdated, ad_idx);
                    _CNT(AdCell.cnt[ad_idx]);
                    adc_conver_step = ADC_CONVER_STEP_SEL_CH;
                    is_next_idx = TRUE;
                    if (++ad_tbl_idx >= AD_IDX_MAX)    ad_tbl_idx = 0;     
                }
                else
                {
                    adc_conver_step = ADC_CONVER_STEP_START;
                }
            }
            else
            {
                if (++timeout_cnt >= 50)    // 超时,重启
                {
                    adc_conver_step = ADC_CONVER_STEP_INIT;
                }
            }

            // 立即准备下一通道,则没有break;
            if (!is_next_idx)
            {
                break;
            }
            // break;
                   
        //3 准备下一个AD相关通道            
        case ADC_CONVER_STEP_SEL_CH:
            smp_cnt = 0;
            adc_over_smp = 0;

            //4 准备NTC 4051通道
            MCU_SetNtcChannel(HdwAdTbl[ad_tbl_idx].ntc_addr);
//          //4 准备NTC 放大通道
//          MCU_SetNtcPga(HdwAdTbl[ad_tbl_idx].ntc_pg);
//          //4 准备CUR 4051通道
//          MCU_SetCurChannel(HdwAdTbl[ad_tbl_idx].cur_addr);
            //4 准备芯片ADC通道
            MCU_ADC_SelectCh(HdwAdTbl[ad_tbl_idx].adc_ch);
            
            #if AD_UNUSE_SAMPLE_NUM > 0
            timeout_dly = 0;    // 如果丢弃则不延时
            #else
            timeout_dly = 1;
            #endif
            
            adc_conver_step = ADC_CONVER_STEP_START;
            break;
            

    }

}

/*****************************************************************************
函  数 : HDW_AdcUpdateAdVal()
功  能 : 更新ADC采样值
参  数 : U16* ad    : 滑窗均值
         U08 ad_idx : ad路
         U08 by     : 获取方式
返  回 : TRUE/FALSE  有更新/无更新
其  他 : 无
*****************************************************************************/
BOOL HDW_AdcUpdateAdVal(U16* ad, U08 ad_idx, U08 by)
{

    if (by == HDW_UPDATE_VOL_BY_ONE_INTV)
    {
        /* 一个滤波周期更新一次 */
        if (AdCell.cnt[ad_idx] < MAX_AD_FILTER_NUM)
        {
            return FALSE;
        }
    	
        AdCell.cnt[ad_idx] = 0; 
    }
    else
    {
        /* 每次采样更新一次 */
        if (!GETBIT_N(AdCell.adUpdated, ad_idx))
        {
            return FALSE;
        }
    	
        ATOMIC(CLRBIT_N(AdCell.adUpdated, ad_idx));
    }

    *ad = (I16)HDW_AdcFilter(AdCell.adBuf[ad_idx], TRUE);

    return TRUE;
}


/*****************************************************************************
函  数 : HDW_AdcCompensate()
功  能 : AD补偿(按过采样值)
参  数 : U16 ad : 原AD值
返  回 : 补偿后AD值
其  他 : 无
*****************************************************************************/
U16 HDW_AdcCompensate(U16 ad)
{
    I32 ad_cmps = ad;

    ad_cmps += AdCalibrate.val;

    if (ad_cmps < 0)
    {
        ad_cmps = 0;
    }
    else if (ad_cmps > AD_MAX)
    {
        ad_cmps = AD_MAX;
    }

    return (U16)ad_cmps;
}



