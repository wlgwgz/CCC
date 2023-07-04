/******************************************************************************

          ��Ȩ���� (C), 2001-2014, �����а��յ��Լ����������޹�˾

 ******************************************************************************
  �� �� ��   : Hdw_Ai.c
  �� �� ��   : ����
  ��    ��   : ��־��
  ��������   : 2016��11��7������һ
  ����޸�   :
  ��������   : Ӳ���ϴ���AI��ȡ
  �����б�   :
              ADC_SampleAi
  �޸���ʷ   :
  1.��    ��   : 2016��11��7������һ
    ��    ��   : ��־��
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include "Mcu_def.h"
#include "Hdw_Main.h"
#include "Hdw_Ai.h"
#include "Hdw_Eeprom.h"

#include "Includes.h"
#if MAX_AI_NUM != 0
/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

/* �ⲿ����ԭ��˵��   */
extern void MCU_GetAllAiAdCh(U16 Ai_Ad[]);

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/


GSTATIC I16 Ai_Type[MAX_AI_NUM];
GSTATIC I16 Hdw_Ai[MAX_AI_NUM];
//GSTATIC LineCoe AiCoe[MAX_AI_NUM][MAX_AI_TYPE];      
       
/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/
typedef struct tagHDW_AI_TBL {
    U08 ad_idx;         // AD�������
}HDW_AI_TBL;
const HDW_AI_TBL HdwAiAdTbl [MAX_AI_NUM] = {
    {AD_IDX_AI_1 }, 
    {AD_IDX_AI_2 }, 
};
#if MAX_AI_NUM != 2
    #error "�޸�"
#endif
/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/

void HDW_Ai_Init(void)
{
    U08 i;
//    BOOL rd_succ = FALSE;

    /* ��ȡУ׼ֵ */
//  rd_succ = eprom_rd_near_crc(&AiCoe[0][0], EEPROMADDR_ADJUST_AI,sizeof(AiCoe));

//  if(!rd_succ)
//  {
//      for (i=0; i<MAX_AI_NUM; i++)
//      {
//          AiCoe[i][0].A =  (U16)((U32)150*16*1023/AD_ADJ);//4-20
//          AiCoe[i][0].B = 0;
//          AiCoe[i][1].A =  (U16)((U32)600*16*1023/AD_ADJ);//0-5
//          AiCoe[i][1].B = 0;
//          AiCoe[i][2].A =  (U16)((U32)300*16*1023/AD_ADJ);//0-10
//          AiCoe[i][2].B = 0;
//      }            
//  }

    /* ��ʼ��AI���� */
    for (i=0; i<MAX_AI_NUM; i++)
    {
        Ai_Type[i] = AI_NONE;
    }

    /* ��ʼ��AI���� */
    for (i=0; i<MAX_AI_NUM; i++)
    {
        Hdw_Ai[i] = SENSOR_NOEXIST;
    }
}

//BOOL HDW_SaveAiAdj(U08 num, U08 type, U16 a, I16 b)
//{
//  if (num >= MAX_AI_NUM || type >= MAX_AI_TYPE)
//      return FALSE;

//  AiCoe[num][type].A = (U16)a;
//  AiCoe[num][type].B = (I16)b;
    
//  return eprom_wr_near_crc(&AiCoe[0][0], EEPROMADDR_ADJUST_AI,sizeof(AiCoe));
//}



/*****************************************************************************
��  �� : HDW_AdToAiSignal()
��  �� : ֱ�Ӷ�ȡУ׼��ģ�����źţ�3λС��
��  �� : I16 type : 
         U16 ad   : 
��  �� : ��
��  �� : ��
*****************************************************************************/
I16 HDW_AdToAiSignal(I16 type, U16 ad)
{ 
    I16 signal = ~SENSOR_NOEXIST; // ԭΪSENSOR_NOEXIST;
    U16 ad_short,ad_break;
    U32 a,b;


    switch(type)
    {
        case AI_05_45V:
            ad_short = ((U32)3200*AD_MAX)/AD_VREF;//������3.20Vʱ��Ϊ��·
            ad_break = ((U32)20*AD_MAX)/AD_VREF;//������0.02Vʱ��Ϊ��·
            break;

        case AI_NONE:
//        case AI_4_20MA:
//        case AI_0_5V:
//        case AI_0_10V:            
        default: 
            signal =  SENSOR_NOEXIST;
            break;            
    }
        
    if(signal != SENSOR_NOEXIST)
    {
        if(ad > ad_short)//��·
        {
            signal = SENSOR_SHORTED;
        }
        else if(ad < ad_break)//��·
        {
            signal = SENSOR_BREAKED;
        }
        else
        {
            switch(type)
            {
                case AI_05_45V:    
                    //      11        ad
                    // V*-------- = --------*AD_VREF
                    //    11+6.8     AD_MAX
                    a = ((U32)ad*(110+68)*(AD_VREF/10));
                    b = (110*(AD_MAX/10));
                    signal = (I16)(a/b); // ��λmV   
                    break;
                
//                case AI_4_20MA:
//                case AI_0_5V:
//                case AI_0_10V:   
                case AI_NONE:    
                default:            
                    signal = SENSOR_NOEXIST;
                    break;        
            }
        }
    }
    
    return signal;
    
}



/*****************************************************************************
��  �� : HDW_GetAllAi()
��  �� : Ӳ���ײ��ȡģ����,��Ӳ���������ȵ���
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void HDW_GetAllAi(void)
{
    U08 i;
    U16 ad;

        
    for (i=0; i<MAX_AI_NUM; i++)
    {
        if (HDW_AdcUpdateAdVal(&ad, HdwAiAdTbl[i].ad_idx, HDW_UPDATE_VOL_BY_ONE_TIMES))
        {

            TestCurCurAd(i) = ad;    // ��ʾ��ѹ

            ad = HDW_AdcCompensate(ad);

            Hdw_Ai[i] = HDW_AdToAiSignal(Ai_Type[i], ad);
        }
    }
}


/*****************************************************************************
��  �� : PunpGetAi()
��  �� : ��ȡģ�����ӿں���
��  �� : I16 AiBuf[] : ģ����������
         I16 AiType[]: ģ��������
         U16 AiNum   : ģ��������
��  �� : TRUE�ɹ�/FALSEʧ��
��  �� : ģ�������͸ı�ʱ,������������������Ч��
*****************************************************************************/
BOL PunpGetAi(I16 AiBuf[], I16 AiType[], U16 AiNum)
{
    U08 i;
    
    if (AiNum > MAX_AI_NUM)
        return FALSE;

    for (i=0; i<AiNum; i++)
    {
        Ai_Type[i] = AiType[i];
        AiBuf[i] = Hdw_Ai[i];
    }    

    return TRUE;    
}
#endif
