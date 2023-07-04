/******************************************************************************

          版权所有 (C), 2001-2016, 广州市邦普电脑技术开发有限公司

 ******************************************************************************
  文 件 名   : Logging.h
  版 本 号   : 初稿
  作    者   : zzp
  生成日期   : 2017年5月9日 星期二
  最近修改   :
  功能描述   : Logging.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2017年5月9日 星期二
    作    者   : zzp
    修改内容   : 创建文件

******************************************************************************/

#ifndef __LOGGING_H__
#define __LOGGING_H__

/*-----------------包含头文件 -----------------------------*/
#include <stdarg.h>

#define LOG_UART                    COM_BMS   //Your_log_uart:注意把“LOG_TrasInt”放到相应中断回调函数
//#define LOG_INVT                    BKBD_INVT_UART4
#ifndef WDG_Refresh
#define WDG_Refresh()               MCU_WDG_Refresh() 
#endif
#define UART_TrasData(u, data)          LOG_COM_WrTrasBufReg(u, data)//Your_log_TrasAPI
#define UART_Config(u, baud, dataType)  COM_Config(u, baud, dataType)//Your_log_ConfigAPI

#define MAX_LOG_BUF 1000 //注意，大于255时将下面类型改为U16,并考虑中断问题
typedef struct tagLOG   
{
    U16 head;
    U16 end;
    U16 length;
    BOL sending;
    STR buf[MAX_LOG_BUF];
    
}LOG;

/*-----------------外部变量说明----------------------------*/



/*-----------------外部函数原型说明------------------------*/



/*-----------------内部函数原型说明------------------------*/



/*-----------------全局变量--------------------------------*/



/*-----------------模块级变量------------------------------*/



/*-----------------常量定义--------------------------------*/



/*-----------------宏定义 ---------------------------------*/

#if defined(USE_LOG_PRINT)
#define LOG_LOGGINGMSG(msg)     LOG_LoggingMsg(msg)
#define LOG_LOGGINGNUM(num)     LOG_LoggingNum(num, FDNS_DIGIT_5)
#define LOG_LOGGINGHEX(p, n, s) LOG_LoggingHex(p, n, s)
#define LOG_LOGGINGFPRINT       LOG_LoggingFPrint   /* 运行日志 */
//  #ifdef USE_DEBUG_LOG
//  #define LOG_LOGGINGDEBUG    LOG_LoggingFPrint   /* 调试日志 */
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
