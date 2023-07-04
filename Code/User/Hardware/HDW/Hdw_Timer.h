/******************************************************************************

          ��Ȩ���� (C), 2001-2014, �����а��յ��Լ����������޹�˾

 ******************************************************************************
  �� �� ��   : Hdw_Timer.h
  �� �� ��   : ����
  ��    ��   : // zzp v160a7
  ��������   : 2016��11��8�����ڶ�
  ����޸�   :
  ��������   : Hdw_Timer.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��11��8�����ڶ�
    ��    ��   : // zzp v160a7
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef __HDW_TIMER_H__
#define __HDW_TIMER_H__



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
typedef struct tagPWMOUT
{
    U16 minduty;
    U16 maxduty;
    U16 curduty;
    U16 frequency;
}PWMOUT;

#define PWC_LEVEL_UNKNOW        0
#define PWC_LEVEL_HIGH          1
#define PWC_LEVEL_LOW           2

#define PWC_EDGE_LOW            0x01
#define PWC_EDGE_HIGH           0x02
    
#define MAX_PWCIN_FILTER    6      // �˲�����
typedef struct tagPWCIN
{
    U08 edge;                      // ɨ�����bit0:��,bit1:��
    U08 level;
    BOL cal;
    BOL init;                      // �Ѵ���
    BOL lowfull;                   // ȫ�͵�ƽ
    BOL highfull;                  // ȫ�ߵ�ƽ    
    U08 filtercnt;
    U16 dutyfilter[MAX_PWCIN_FILTER];
    U16 duty;
    U16 count_low;
    U16 count_high;
}PWCIN;

typedef struct tagFREQIN
{
    U16 freq;   // HZ
}FREQIN;
/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/


extern void HDW_DelayUs(U16 usec);
extern void HDW_DelayMs(U16 msec);
extern U16 PunpGetPwmInDuty(void);
extern void PunpSetPwmFreqDuty(U16 Freq, U16 Duty, U16 Channel);
extern U16 PunpGetPwmInput(U16 inchnel);	/* N0004 */
extern void HDW_PwmInDutyCal(void);
void HDW_PwcEdgeInt(void);
void HDW_PwcTimeoutInt(void);
void HDW_CounterOverInt(U08 cnt_num);
void HDW_1ms_Deal(void);
#endif /* __HDW_TIMER_H__ */
