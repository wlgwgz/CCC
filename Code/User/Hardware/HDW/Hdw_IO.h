/******************************************************************************

          ��Ȩ���� (C), 2001-2014, �����а��յ��Լ����������޹�˾

 ******************************************************************************
  �� �� ��   : Hdw_IO.h
  �� �� ��   : ����
  ��    ��   : 
  ��������   : 2016��11��8�����ڶ�
  ����޸�   :
  ��������   : Hdw_IO.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��11��8�����ڶ�
    ��    ��   : 
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __HDW_IO_H__
#define __HDW_IO_H__
/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
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
 * ��������                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/



#define MAX_PHASE_SHAKE       5   /* ��������һ�ζ�ȡ�Ĵ��� */


#define PHASE_TIMES         100     /*  һ�������������Ϊ100ms */
#define PHASE_OK_CNT        5       /*  ���������һ�� */
#define PHASE_ERR_CNT       -5      /*  ����Ĳ��Խ�� */
#define PHASE_LACK_CNT      2       /*  ȱ�ࡢǷѹ�Ĳ��Խ�� */

/* �¶����� */
enum 
{
    PHASE_STA_OK=0,                 /* ���� */
    PHASE_STA_ERR,                  /* ���� */
    PHASE_STA_LACK,                 /* ȱ�� */
};

//#define MAX_SW_BIT          (MAX_SW1_BIT+MAX_SW2_BIT+MAX_SWPL_BIT+MAX_SW4_BIT+MAX_SW5_BIT) // 18 SW����
//#define MAX_SWPL_BIT        4
#define MAX_SWITCH_SHAKE    5   /* ����/������һ�ζ�ȡ�Ĵ��� */

//#define MAX_COMP						2		//��ģ�����ѹ����	  //NOTE-CXW ����Ӧ�ó���������Ҫ����HDW

#define PINS_NUM_SW         MAX_SW_BIT

#define MAX_LED_NUM         2
#define LED_RED_IDX         0           /* ��Ʊ�� */
#define LED_ORG_IDX         1           /* �ȵƱ�� */

#define LED_STA_OFF         0
#define LED_STA_ON          1


extern void HDW_GetDi(void);
extern void PunpCloseAllDo(void);
extern BOL PunpSetDo(U16 OutputBuf[], U16 OutputNum);
extern BOL PunpGetDi(U16 InputBuf[], U16 InputNum);
extern void PunpGetSwitch(U16 SwBuf[], U16 SwNum);
extern void PunpGetSwPl(U16 SwPlBuf[], U16 SwPlNum);
extern U08 PunpGetPhase(void);
extern BOL PunpGet4GExist(void);
extern void HDW_GetInput(void);
extern void HDW_GetHDI(void);
extern BOOL PunpSetLed(U16 Led, U08 Sta);
extern void PunpSet4GPower(BOL onoff);
extern void PunpSet4GReset(BOL reset);

#endif /* __HDW_IO_H__ */
