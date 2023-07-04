/******************************************************************************

          ��Ȩ���� (C), 2001-2014, �����а��յ��Լ����������޹�˾

 ******************************************************************************
  �� �� ��   : Debug.h
  �� �� ��   : ����
  ��    ��   : // zzp v160a7
  ��������   : 2016��11��8�����ڶ�
  ����޸�   :
  ��������   : Debug.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��11��8�����ڶ�
    ��    ��   : // zzp v160a7
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __DEBUG_H__
#define __DEBUG_H__


#ifdef  DEBUG_GLOBAL
    #define DEBUG_EXT
#else
    #define DEBUG_EXT extern
#endif
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





//#define ZZP_DEBUG   1
#ifdef ZZP_DEBUG
DEBUG_EXT I16 zzpdebug[20];
#define zzpdebug0   zzpdebug[0]
#define zzpdebug1   zzpdebug[1]
#define zzpdebug2   zzpdebug[2]
#define zzpdebug3   zzpdebug[3]
#define zzpdebug4   zzpdebug[4]
#define zzpdebug5   zzpdebug[5]
#define zzpdebug6   zzpdebug[6]
#define zzpdebug7   zzpdebug[7]
#define zzpdebug8   zzpdebug[8]
#define zzpdebug9   zzpdebug[9]
#define zzpdebug10   zzpdebug[10]
#define zzpdebug11   zzpdebug[11]
#define zzpdebug12   zzpdebug[12]
#define zzpdebug13   zzpdebug[13]
#define zzpdebug14   zzpdebug[14]
#define zzpdebug15   zzpdebug[15]
#define zzpdebug16   zzpdebug[16]
#define zzpdebug17   zzpdebug[17]
#define zzpdebug18   zzpdebug[18]
#define zzpdebug19   zzpdebug[19]
#endif

#ifdef M_DEBUG
DEBUG_EXT BOL tbhDebug[20];
DEBUG_EXT BOL ibhDebug[20];
DEBUG_EXT BOL ahsDebug[20];
DEBUG_EXT BOL s3v1Debug[20];
DEBUG_EXT BOL s3v2Debug[20];
DEBUG_EXT BOL s3v3Debug[20];
#endif
DEBUG_EXT U08 modParaRdIdDebug;
DEBUG_EXT BOL modParaRdDebug;

/* debug-kjh */
DEBUG_EXT U08 dbg_ScanPeriod_flash;
DEBUG_EXT U16 dbg_ScanPeriod_Min[10];
DEBUG_EXT U16 dbg_ScanPeriod_Cur[10];
DEBUG_EXT U16 dbg_ScanPeriod_Max[10];

#define EXEC_TIME_BGN 0
#define EXEC_TIME_END 1
DEBUG_EXT U08 dbg_ExecTime_flash;
DEBUG_EXT U16 dbg_ExecTime_init[10];
DEBUG_EXT U16 dbg_ExecTime_Tick[10];
DEBUG_EXT U16 dbg_ExecTime_Min[10];
DEBUG_EXT U16 dbg_ExecTime_Cur[10];
DEBUG_EXT U16 dbg_ExecTime_Max[10];
      
DEBUG_EXT U08 debug_kjh_flash;
DEBUG_EXT I16 debug_kjh[20];			
#define DEBUG_KJH_00 debug_kjh[ 0]
#define DEBUG_KJH_01 debug_kjh[ 1]
#define DEBUG_KJH_02 debug_kjh[ 2]
#define DEBUG_KJH_03 debug_kjh[ 3]
#define DEBUG_KJH_04 debug_kjh[ 4]
#define DEBUG_KJH_05 debug_kjh[ 5]
#define DEBUG_KJH_06 debug_kjh[ 6]
#define DEBUG_KJH_07 debug_kjh[ 7]
#define DEBUG_KJH_08 debug_kjh[ 8]
#define DEBUG_KJH_09 debug_kjh[ 9]
#define DEBUG_KJH_10 debug_kjh[10]
#define DEBUG_KJH_11 debug_kjh[11]
#define DEBUG_KJH_12 debug_kjh[12]
#define DEBUG_KJH_13 debug_kjh[13]
#define DEBUG_KJH_14 debug_kjh[14]
#define DEBUG_KJH_15 debug_kjh[15]
#define DEBUG_KJH_16 debug_kjh[16]
#define DEBUG_KJH_17 debug_kjh[17]
#define DEBUG_KJH_18 debug_kjh[18]
#define DEBUG_KJH_19 debug_kjh[19]

DEBUG_EXT BOL fg_dbg_slow;	/* 0:�� 1���� */

extern void DBG_1ms_Deal(void);
extern void DBG_Main(void);
extern void DBG_get_scan_period(U08 num);
extern void DBG_get_exec_time(U08 num, U08 en);
#endif /* __DEBUG_H__ */
