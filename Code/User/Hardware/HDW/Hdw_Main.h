/******************************************************************************

          ��Ȩ���� (C), 2001-2016, �����а��յ��Լ����������޹�˾

 ******************************************************************************
  �� �� ��   : Hdw_Main.h
  �� �� ��   : ����
  ��    ��   : ��־��
  ��������   : 2016��11��9��������
  ����޸�   :
  ��������   : Hdw_Main.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��11��9��������
    ��    ��   : ��־��
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef __HDW_MAIN_H__
#define __HDW_MAIN_H__


/*-----------------����ͷ�ļ� -----------------------------*/
#include "Mcu_def.h"
#include "Hdw_Eeprom.h"
#include "Hdw_Timer.h"
#include "Hdw_IO.h"



#define AD_VREF          3250UL             //3.250V

/*==============================================================================
ע��:��������MAX_AD_FILTER_NUM��������AD_Filter()�и���
==============================================================================*/
 #define MAX_AD_FILTER_NUM   6           /* �˲�ʱ���������� */
#define AD_OVER_SAMPLE_NUM  16           /* �ι��������� */
#define AD_UNUSE_SAMPLE_NUM 1            /* ���õ�ת��,���ڶ���ǰ���β��ȶ���AD */
//#define AD_UNUSE_SAMPLE_NUM 6           /* ÿ����������ADC����Բ����� */
#define MAX_AD_CONVER_NUM   (AD_UNUSE_SAMPLE_NUM + AD_OVER_SAMPLE_NUM) /* �ܲ��ô��� */

#define AD_MAX  (4095UL*AD_OVER_SAMPLE_NUM)  /* AD���ֵ(4095*16��10λAD��16���ۼ�) */

#define MAX_MOTOR           MAX_MOTOR_NUM   //��󲽽������


#define TMP_PT1000          1           /* ѡ���¶�̽ͷΪPT1000*/
#define TMP_NTC             0           /* ѡ���¶�̽ͷΪNTC*/


//ģ������������
#define AI_NONE             0
#define AI_05_45V           1   //NOTE-CXW ����Ӱ�쵽8�������ԣ���ȷ��
//#define AI_4_20MA           2
//#define AI_0_5V             3
//#define AI_0_10V            4



#define IN_74HC165_BITS     (MAX_SW_BIT+MAX_SWPL_BIT+MAX_DI_NUM+1)   // 74HC165����������� 14+4+7+1 1ΪU13��Hλ
#define IN_74HC165_W        (_DIVUCARRY(IN_74HC165_BITS, 16))

#define IN_PARALLEL_BITS    (0)  // �����������
#define IN_PARALLEL_W       (_DIVUCARRY(IN_PARALLEL_BITS, 16))

// ��������������(����DI,SW,PL��)
#define INPUT_NUM           (IN_74HC165_BITS + IN_PARALLEL_BITS)
#define INPUT_W             (_DIVUCARRY(INPUT_NUM, 16))

#define DI_1                0
#if (MAX_DI_NUM != 0)
#define MAX_DI_W            ((MAX_DI_NUM-1)/16+1)
#else
#define MAX_DI_W            (1)
#endif


#define DO_1                0
#if (MAX_DO_NUM != 0)
#define MAX_DO_W            ((MAX_DO_NUM-1)/16+1)
#else
#define MAX_DO_W            (1)
#endif



#define SENSOR_MIN          -32699      /* ̽ͷ��Сֵ */
#define SENSOR_NOEXIST      -32700      /* ̽ͷ������ */
#define SENSOR_ERROR        -32701      /* ̽ͷ���� */
#define SENSOR_BREAKED      -32702      /* ̽ͷ��· */
#define SENSOR_SHORTED      -32703      /* ̽ͷ��· */

#define SensorIsError(pv)   (((I16)pv) <= SENSOR_ERROR)
#define SensorCanUse(pv)    (!(((I16)pv) <= SENSOR_NOEXIST))
#define SensorCannotUse(pv)	(((I16)pv) <= SENSOR_NOEXIST)
#define SensorNoExist(pv)   (((I16)pv) == SENSOR_NOEXIST)
#define SensorIsExist(pv)   (!SensorNoExist((I16)pv))



extern void HDW_Init(void);
extern void HDW_Main(void);
extern U32 HDW_1ms_Tick(void);
extern U16 AD_Filter(U16 adBuf[], BOOL useIntCtrl);

extern BOL PunpGetDi(U16 InputBuf[], U16 InputNum);
extern void PunpGetSwitch(U16 SwBuf[], U16 SwNum);
extern void PunpGetSwPl(U16 SwPlBuf[], U16 SwPlNum);
extern BOL PunpSetDo(U16 OutputBuf[], U16 OutputNum);
#endif /* __HDW_MAIN_H__ */
