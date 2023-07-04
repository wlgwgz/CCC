/******************************************************************************

          ��Ȩ���� (C), 2001-2016, �����а��յ��Լ����������޹�˾

 ******************************************************************************
  �� �� ��   : Logging.h
  �� �� ��   : ����
  ��    ��   : zzp
  ��������   : 2017��5��9�� ���ڶ�
  ����޸�   :
  ��������   : Logging.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2017��5��9�� ���ڶ�
    ��    ��   : zzp
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef __LOGGING_H__
#define __LOGGING_H__

/*-----------------����ͷ�ļ� -----------------------------*/
#include <stdarg.h>

#define LOG_UART                    COM_BMS   //Your_log_uart:ע��ѡ�LOG_TrasInt���ŵ���Ӧ�жϻص�����
//#define LOG_INVT                    BKBD_INVT_UART4
#ifndef WDG_Refresh
#define WDG_Refresh()               MCU_WDG_Refresh() 
#endif
#define UART_TrasData(u, data)          LOG_COM_WrTrasBufReg(u, data)//Your_log_TrasAPI
#define UART_Config(u, baud, dataType)  COM_Config(u, baud, dataType)//Your_log_ConfigAPI

#define MAX_LOG_BUF 1000 //ע�⣬����255ʱ���������͸�ΪU16,�������ж�����
typedef struct tagLOG   
{
    U16 head;
    U16 end;
    U16 length;
    BOL sending;
    STR buf[MAX_LOG_BUF];
    
}LOG;

/*-----------------�ⲿ����˵��----------------------------*/



/*-----------------�ⲿ����ԭ��˵��------------------------*/



/*-----------------�ڲ�����ԭ��˵��------------------------*/



/*-----------------ȫ�ֱ���--------------------------------*/



/*-----------------ģ�鼶����------------------------------*/



/*-----------------��������--------------------------------*/



/*-----------------�궨�� ---------------------------------*/

#if defined(USE_LOG_PRINT)
#define LOG_LOGGINGMSG(msg)     LOG_LoggingMsg(msg)
#define LOG_LOGGINGNUM(num)     LOG_LoggingNum(num, FDNS_DIGIT_5)
#define LOG_LOGGINGHEX(p, n, s) LOG_LoggingHex(p, n, s)
#define LOG_LOGGINGFPRINT       LOG_LoggingFPrint   /* ������־ */
//  #ifdef USE_DEBUG_LOG
//  #define LOG_LOGGINGDEBUG    LOG_LoggingFPrint   /* ������־ */
//  #else
//  #define LOG_LOGGINGDEBUG(format, ...)   
//  #endif
#else
#define LOG_LOGGINGMSG(msg)  
#define LOG_LOGGINGNUM(num)  
#define LOG_LOGGINGHEX(p, n, s) 
#define LOG_LOGGINGDEBUG(format)   ASM_NOP()
#define LOG_LOGGINGFPRINT(format)  ASM_NOP()
#endif

BOL LOG_IsEnable(void);
extern void LOG_Enable(BOL enable);
extern void LOG_LoggingMsg(STR* msg);
extern void LOG_LoggingNum(U16 num, U16 attrib);
extern void LOG_LoggingHex(U08 Buf[], U16 Length, STR Split[]);
extern void LOG_LoggingFPrint(STR* format, ...);
extern void LOG_Server(void);
extern void LOG_TrasInt(void);


#endif /* __LOGGING_H__ */
