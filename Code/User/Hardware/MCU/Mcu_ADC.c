/******************************************************************************

          ��Ȩ���� (C), 2001-2014, �����а��յ��Լ����������޹�˾

 ******************************************************************************
  �� �� ��   : Mcu_ADC.c
  �� �� ��   : ����
  ��    ��   : ��־��
  ��������   : 2016��11��7������һ
  ����޸�   :
  ��������   : AD�������
  �����б�   :
              AD_Sample
  �޸���ʷ   :
  1.��    ��   : 2016��11��7������һ
    ��    ��   : ��־��
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include "mcu_def.h"

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/



/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/

#define ADC_INSTANCE ((stc_adcn_t *)&ADC0)


/*******************************************************************************
 �� ��: MCU_ADC_Init()
 �� ��: 
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void MCU_ADC_Init(void)
{
    stc_adcn_t       *pstcAdc         = ADC_INSTANCE;
    stc_adc_config_t stcConfig;
    stc_adc_scan_t   stcScanCfg;

    /* Clear structures */
    PDL_ZERO_STRUCT(stcConfig);
    PDL_ZERO_STRUCT(stcScanCfg);

    /* Scan conversion configuration */
    stcScanCfg.u32ScanCannelSelect.u32AD_CHn = 0;
    stcScanCfg.enScanMode = ScanSingleConversion;
    stcScanCfg.enScanTimerTrigger = AdcNoTimer;
    stcScanCfg.bScanTimerStartEnable = FALSE;
    stcScanCfg.u8ScanFifoDepth = 0;     
    
/******************************************************************************
*      Common setting
*      Conversion time = Sampling time + Compare time
*      The sampling time should comply with a certain range to guarantee the 
*      accuracy (Tmin < Ts < Tmax).
*      For the value of Tmin and Tmax (e.g. 10us), see the product datasheet
*      for detail (12-bit A/D converter of Electrical Characteristics chapter)
******************************************************************************/
/******************************************************************************
*      For FM4 and FM0+, the calculation of sampling time and compare time is 
*      shown as following:
*      Sampling time = HCLK cycle * Frequency division ratio * {(ST set value +
*                                              1) * STX setting multiplier + 3}
*
*      At the following configuration:
*      Sampling time = 5ns * 5 * {(8+1)*8+3} = 1.875us (if HCLK = 200MHz)
*      Sampling time = 25ns * 5 * {(8+1)*8+3} = 9.375us (if HCLK = 40MHz)
*
*      Compare time = Compare clock cycle * 14 = Base clock (HCLK) cycle * 
+                                             Frequency division ratio * 14
*      At following configuration:
*      Compare time = 5ns * 5 * 14 = 350ns (if HCLK = 200MHz)
*      Compare time = 25ns * 5 * 14 = 1750ns (if HCLK = 40MHz)
******************************************************************************/
/******************************************************************************
*      For FM3, the calculation of sampling time and compare time is shown as 
*      following:
*      Sampling time = HCLK cycle * {(ST set value + 1) * STX setting multiplier + 3}
*      At the following configuration:
*      Sampling time = 7ns * {(8+1)*8+3} = 0.52us (if HCLK = 144MHz)
*****************************************************************************/
/****************************************************************************
*      Compare time = Compare clock cycle * 14 = Base clock (HCLK) cycle * 
*                                              Frequency division ratio * 14
*      At following configuration:
*      Compare time = 7ns * 5 * 14 = 490ns (if HCLK = 144MHz)
*****************************************************************************/     

    /* AD CLK=80M/16=4M,����ʱ��=250ns*{(8+1)*256+3}=57675ns */
    stcConfig.bLsbAlignment = TRUE;
    stcConfig.u32SamplingTimeSelect.u32AD_CHn = 0;  
    stcConfig.enSamplingTimeN0 = Value256;  /*  STX setting multiplier 0: 8 */
    stcConfig.u8SamplingTime0 = 8u;         /*  ST value 0 : 8 */
    stcConfig.enSamplingTimeN1 = Value256;  /*  STX setting multiplier 1: 8  */
    stcConfig.u8SamplingTime1 = 8u;         /*  ST value 1 : 8 */
    stcConfig.u8ComparingClockDiv = 14u;     /*  Frequency division ratio: 5, 0:Ratio 2, 1:Ratio 3, ... */
    stcConfig.pstcScanInit = &stcScanCfg;

    Adc_Init(pstcAdc, &stcConfig);
}




void MCU_ADC_SelectCh(U32 adc_ch)
{
    stc_adcn_t       *pstcAdc         = ADC_INSTANCE;
    
    // Set Sampling Time Selection Register
//  pstcAdc->ADSS01 = (uint16_t)(0x0000FFFFul & adc_ch);

//  pstcAdc->ADSS23 = (uint16_t)((0xFFFF0000ul &  adc_ch) >> 16u);

    // Set Scan Conversion Input Selection Register
    pstcAdc->SCIS01 = (uint16_t)(0x0000FFFFul & adc_ch);

    pstcAdc->SCIS23 = (uint16_t)((0xFFFF0000ul & adc_ch) >> 16u);
}

U16 MCU_ADC_ReadAd(void)
{
    uint32_t u32Data = Adc_ReadScanFifo(ADC_INSTANCE);
    
    return Adc_GetScanData(ADC_INSTANCE, u32Data);
}

BOOL MCU_ADC_IsCplt(void)
{
    return Adc_GetIrqFlag(ADC_INSTANCE, AdcScanIrq);   
}

void MCU_ADC_ClrCplt(void)
{
    Adc_ClrIrqFlag(ADC_INSTANCE, AdcScanIrq);  
}

void MCU_ADC_Start(void)
{
    Adc_SwTriggerScan(ADC_INSTANCE);
}

void MCU_ADC_Enable(void)
{
    Adc_EnableWaitReady(ADC_INSTANCE);		//ADC����
}












