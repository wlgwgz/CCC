/******************************************************************************

          ��Ȩ���� (C), 2001-2016, �����а��յ��Լ����������޹�˾

 ******************************************************************************
  �� �� ��   : Mid_ADC.c
  �� �� ��   : ����
  ��    ��   : zzp
  ��������   : 2021��4��28��
  ����޸�   :
  ��������   : ADC����ģ��
  �����б�   :
              HDW_Adcinit
              HDW_AdcSample
              HDW_AdcSampleAppend
  �޸���ʷ   :
  1.��    ��   : 2021��4��28��
    ��    ��   : zzp
    �޸�����   : �����ļ�

******************************************************************************/
#define _MID_ADC_GBL_
/*-----------------����ͷ�ļ� -----------------------------*/
#include "includes.h"


/*-----------------�ⲿ����˵��----------------------------*/



/*-----------------�ⲿ����ԭ��˵��------------------------*/



/*-----------------�ڲ�����ԭ��˵��------------------------*/



/*-----------------ȫ�ֱ���--------------------------------*/



/*-----------------ģ�鼶����------------------------------*/



/*-----------------��������--------------------------------*/



/*-----------------�궨�� ---------------------------------*/









enum
{
	ADC_CONVER_STEP_INIT = 0,   // ��ʼ��
	ADC_CONVER_STEP_START,      // ����ת��
	ADC_CONVER_STEP_SAMPLE,     // ����    
	ADC_CONVER_STEP_SEL_CH      // ѡ��ͨ��
};

typedef struct tagHDW_AD_CH{
    U08 ad_idx;     // AD�������
    U08 ntc_pg;     // NTC�Ŵ�4051��ַ
    U08 ntc_addr;   // NTCѡͨ4051��ַ
    U08 cur_addr;   // ����ѡͨ4051��ַ    
    U32 adc_ch;     // оƬADCͨ��
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
 �� ��: HDW_AdcFilter()
 �� ��: AD�˲�������
 �� ��: adBuf--���ݻ����׵�ַ; 
        useIntCtrl--ʹ���жϿ��Ʒ�(�ء����ж�)��
            Ϊ����ж�����жϵĹ������adBuf��ͻʱ��useIntCtrlΪTRUE 
            ���ж��е��øú���ʱ��useIntCtrlΪFALSE; 
 �� ��: �˲�ֵ
 �� ��: ȥ����Сֵ�����ֵ��Ȼ����ƽ��ֵ�������˲������̶�ΪMAX_AD_FILTER_NUM
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

	/* ������Сֵ�����ֵ�����ۼ� */
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
	/* ���ҵڶ�Сֵ���ڶ���ֵ */
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
	
	/* ȥ��2��Сֵ��2����ֵ����ƽ��ֵ */
    adSum -= adMin; 
	adSum -= adMin2;
    adSum -= adMax;
	adSum -= adMax2;
    adSum /= (MAX_AD_FILTER_NUM - 4);
#else
	/* ȥ����Сֵ�����ֵ����ƽ��ֵ */
    adSum -= adMin; 
    adSum -= adMax;
    adSum /= (MAX_AD_FILTER_NUM - 2);	
#endif /* USE_AD_FILTER2 */

    return (U16)adSum;
} 


/*****************************************************************************
��  �� : HDW_AdcSampleAppend()
��  �� : ����һ��������
��  �� : U16* adBuf : 
         U16 ad     : 
         U08 num    :
��  �� : ��
��  �� : ��
*****************************************************************************/
void HDW_AdcSampleAppend(U16* adBuf, U16 ad, U08 num)
{
    U08 i=0;

    // ��һ��ֵ
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
��  �� : HDW_AdcSample()
��  �� : AD����,����AD����һ�������д���,���ٸ���ģ��������
��  �� : ��
��  �� : ��
��  �� : ��
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
        //3 ��ʼ��,ʹ��AD��
        default:
        case ADC_CONVER_STEP_INIT:
            MCU_ADC_Enable();
            ad_tbl_idx = 0;
            
            adc_conver_step = ADC_CONVER_STEP_SEL_CH;
            break;

        //3 ����ADCת��
        case ADC_CONVER_STEP_START:
            if (timeout_dly != 0)   {timeout_dly--;  break;}
            timeout_cnt = 0;
            MCU_ADC_Start();
            adc_conver_step = ADC_CONVER_STEP_SAMPLE;
            break;
			
        //3 ת��,���������͹�����
        case ADC_CONVER_STEP_SAMPLE:
            if (MCU_ADC_IsCplt())
            {
                ad = MCU_ADC_ReadAd();  
                MCU_ADC_ClrCplt();
                if (smp_cnt >= AD_UNUSE_SAMPLE_NUM) // ����ǰ����,���������
                {
                    adc_over_smp += ad;
                }
                if (++smp_cnt >= MAX_AD_CONVER_NUM) // ��������,����ADֵ
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
                if (++timeout_cnt >= 50)    // ��ʱ,����
                {
                    adc_conver_step = ADC_CONVER_STEP_INIT;
                }
            }

            // ����׼����һͨ��,��û��break;
            if (!is_next_idx)
            {
                break;
            }
            // break;
                   
        //3 ׼����һ��AD���ͨ��            
        case ADC_CONVER_STEP_SEL_CH:
            smp_cnt = 0;
            adc_over_smp = 0;

            //4 ׼��NTC 4051ͨ��
            MCU_SetNtcChannel(HdwAdTbl[ad_tbl_idx].ntc_addr);
//          //4 ׼��NTC �Ŵ�ͨ��
//          MCU_SetNtcPga(HdwAdTbl[ad_tbl_idx].ntc_pg);
//          //4 ׼��CUR 4051ͨ��
//          MCU_SetCurChannel(HdwAdTbl[ad_tbl_idx].cur_addr);
            //4 ׼��оƬADCͨ��
            MCU_ADC_SelectCh(HdwAdTbl[ad_tbl_idx].adc_ch);
            
            #if AD_UNUSE_SAMPLE_NUM > 0
            timeout_dly = 0;    // �����������ʱ
            #else
            timeout_dly = 1;
            #endif
            
            adc_conver_step = ADC_CONVER_STEP_START;
            break;
            

    }

}

/*****************************************************************************
��  �� : HDW_AdcUpdateAdVal()
��  �� : ����ADC����ֵ
��  �� : U16* ad    : ������ֵ
         U08 ad_idx : ad·
         U08 by     : ��ȡ��ʽ
��  �� : TRUE/FALSE  �и���/�޸���
��  �� : ��
*****************************************************************************/
BOOL HDW_AdcUpdateAdVal(U16* ad, U08 ad_idx, U08 by)
{

    if (by == HDW_UPDATE_VOL_BY_ONE_INTV)
    {
        /* һ���˲����ڸ���һ�� */
        if (AdCell.cnt[ad_idx] < MAX_AD_FILTER_NUM)
        {
            return FALSE;
        }
    	
        AdCell.cnt[ad_idx] = 0; 
    }
    else
    {
        /* ÿ�β�������һ�� */
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
��  �� : HDW_AdcCompensate()
��  �� : AD����(��������ֵ)
��  �� : U16 ad : ԭADֵ
��  �� : ������ADֵ
��  �� : ��
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



