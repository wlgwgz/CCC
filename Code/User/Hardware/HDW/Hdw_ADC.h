/******************************************************************************

          ��Ȩ���� (C), 2001-2016, �����а��յ��Լ����������޹�˾

 ******************************************************************************
  �� �� ��   : Mid_ADC.h
  �� �� ��   : ����
  ��    ��   : zzp
  ��������   : 2021��4��28��
  ����޸�   :
  ��������   : Mid_ADC.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2021��4��28��
    ��    ��   : zzp
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef __MID_ADC_H__
#define __MID_ADC_H__

#ifdef  _MID_ADC_GBL_
    #define MID_ADC_EXT
#else
    #define MID_ADC_EXT extern
#endif

/*-----------------����ͷ�ļ� -----------------------------*/



/*-----------------�ⲿ����˵��----------------------------*/



/*-----------------�ⲿ����ԭ��˵��------------------------*/



/*-----------------�ڲ�����ԭ��˵��------------------------*/



/*-----------------ȫ�ֱ���--------------------------------*/



/*-----------------ģ�鼶����------------------------------*/



/*-----------------��������--------------------------------*/

#define HDW_UPDATE_VOL_BY_ONE_INTV      0   // һ�����ڸ���һ��
#define HDW_UPDATE_VOL_BY_ONE_TIMES     1   // һ����������һ��

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


/*-----------------�궨�� ---------------------------------*/
typedef struct tagAD_DATA
{
    U08 cnt[AD_IDX_MAX];               /* AD���������� */
    U16 adUpdated[_DIVUCARRY(AD_IDX_MAX, 16)];    /* AD�������±�־ */
	U16 adBuf[AD_IDX_MAX][MAX_AD_FILTER_NUM];          /* AD���������� */
    I16 adTestDiff;                /* �˷�������Adֵ����Ϊ���� */
} AD_DATA;                                 
MID_ADC_EXT AD_DATA AdCell;


#define AD_R10000       (10000UL*AD_MAX/(10000+10000))
#define AD_CAL_50       (50*AD_OVER_SAMPLE_NUM)
#define AD_CAL_18       (18*AD_OVER_SAMPLE_NUM)
#define AD_CAL_15       (15*AD_OVER_SAMPLE_NUM)
#define AD_CAL_3        (3*AD_OVER_SAMPLE_NUM)

#define ADCAL_STA_NULL  0   // δУ׼
#define ADCAL_STA_SUCC  1   // У׼�ɹ�
#define ADCAL_STA_ERROR 2   // У׼ʧ��
typedef struct {
    U08 sta;
    I16 val;
} ADCAL;
MID_ADC_EXT ADCAL AdCalibrate;  // ADУ׼ֵ,10K����AD֮��


MID_ADC_EXT void HDW_Adcinit(void);
MID_ADC_EXT void HDW_AdcSample(void);
MID_ADC_EXT U16 HDW_AD_Filter(U16 adBuf[], BOOL useIntCtrl);
MID_ADC_EXT void HDW_AdcGetVrefVol(void);
MID_ADC_EXT BOOL HDW_AdcUpdateAdVol(U16* vol, U08 ad_idx, U08 by);
MID_ADC_EXT BOOL HDW_AdcUpdateAdVal(U16* ad, U08 ad_idx, U08 by);
MID_ADC_EXT U16 HDW_AdcCalVrefVol(U16 ad_vol);
MID_ADC_EXT U16 HDW_AdcCompensate(U16 ad);

#endif /* __MID_ADC_H__ */
