/******************************************************************************

          版权所有 (C), 2001-2016, 广州市邦普电脑技术开发有限公司

 ******************************************************************************
  文 件 名   : Logging.c
  版 本 号   : 初稿
  作    者   : zzp
  生成日期   : 2017年5月9日 星期二
  最近修改   :
  功能描述   : 串口调试工，打印日志
  函数列表   :
  修改历史   :
  1.日    期   : 2017年5月9日 星期二
    作    者   : zzp
    修改内容   : 创建文件

******************************************************************************/

/*-----------------包含头文件 -----------------------------*/
#include "includes.h"
#include <stdarg.h>

#if defined(USE_LOG_PRINT)

/*-----------------外部变量说明----------------------------*/
BOL LogEnable;
/*GSTATIC*/ LOG Log;

/*-----------------外部函数原型说明------------------------*/



/*-----------------内部函数原型说明------------------------*/



/*-----------------全局变量--------------------------------*/



/*-----------------模块级变量------------------------------*/



/*-----------------常量定义--------------------------------*/



/*-----------------宏定义 ---------------------------------*/



BOL LOG_IsEnable(void)
{
    return (LogEnable != FALSE);
}

void LOG_Enable(BOL enable)
{
    LOG* plog = &Log;
    
    if (enable)
    {
        if (!LogEnable)
        {
            UART_Config(LOG_UART, B_57600, T_8N1);
            LogEnable = TRUE;
            LOG_LOGGINGFPRINT("s", "\r\n****串口运行日志打印功能开启****\r\n");
        }
    }
    else
    {
        while(plog->sending)
        {
            WDG_Refresh();
        }
        LogEnable = FALSE;
    }
}

void LOG_TrasInt(void)
{
    LOG* plog = &Log;
    
    WDG_Refresh();

    if(plog->length > 0)
    {
        plog->length--;
        
        if(++plog->head >= MAX_LOG_BUF)
        plog->head = 0;
    }

    if(plog->length > 0)
    {
        plog->sending = TRUE;
        UART_TrasData(LOG_UART, plog->buf[plog->head]);
    }
    else
    {
        plog->head = plog->end;/* 冗余操作 */
        plog->sending = FALSE;
    }
}
void LOG_Server(void)
{
    LOG* plog = &Log;
    //static U16 DbgLinkSel;

    /* 进入while循环才开始检测LOG */
//  if(!IntoWhile)   return;
    
//  if(!fg_test && DbgLinkSel != PunpGetSwitch())
//  {
//      DbgLinkSel = PunpGetSwitch();
//      if(DbgLinkSel&(1<<5))
//      {
//          LOG_Enable(TRUE);
//      }
//      else
//      {
//          LOG_Enable(FALSE);
//      }
//  }
    /* 强制开启LOG服务 */
    if(!LOG_IsEnable()) LOG_Enable(TRUE);

    if (!LogEnable)  return;
    
    if (plog->length > 0 && !plog->sending)
    {
        plog->sending = TRUE;
        
        UART_TrasData(LOG_UART, plog->buf[plog->head]);
    }
}

void LOG_LoggingMsg(STR* msg)
{
    LOG* plog = &Log;
    U16 msg_len = STRLEN(msg);
    U16 space_len;
    U16 space2_len;


    if (!LogEnable)  return;
    if (msg_len > MAX_LOG_BUF)  return;
    /* 等待发送 */
    while (MAX_LOG_BUF - plog->length < msg_len)
    {
        WDG_Refresh();
        LOG_Server();
    }

    
    if (plog->end >= plog->head)
    {
        space_len = MAX_LOG_BUF-plog->end;
        if (msg_len > space_len)
        {
            MEMCPY(&plog->buf[plog->end], msg, space_len);
            plog->length += space_len;
            
            space2_len = MIN(msg_len-space_len, plog->head);
            MEMCPY(&plog->buf[0], &msg[space_len], space2_len);
            plog->length += space2_len;
            plog->end = space2_len;
        }
        else
        {
            MEMCPY(&plog->buf[plog->end], msg, msg_len);
            plog->length += msg_len;
            plog->end += msg_len;
        }
    }
    else
    {
        space_len = MIN(plog->head - plog->end, msg_len);
        MEMCPY(&plog->buf[plog->end], msg, space_len);
        plog->length += space_len;
        plog->end += space_len;
    }
}



void LOG_LoggingNum(U16 num, U16 attrib)
{
    U08 buf[8];
    
    buf[Fun_NumDisp(num, buf, attrib)] = '\0';
    
    LOG_LoggingMsg(buf);
}

void LOG_LoggingHex(U08 Buf[], U16 Length, STR Split[])
{
    U16 i;
    U08 hex[8];
    
    for (i=0; i<Length; i++)
    {
        hex[Fun_NumDisp(Buf[i], hex, FDNS_HEX)]='\0';
        LOG_LOGGINGFPRINT("ss", &hex[4], Split);
    }

}

void LOG_LoggingFPrint(STR* format, ...)
{
    va_list ap;
    
    STR* str;
    U16 num;
    U16 attrib;
    
    
    va_start(ap, format);
    
    while (*format != '\0')
    {
        switch (*format)
        {
            case 's':
                str = (STR*)va_arg(ap, char*);
                LOG_LoggingMsg(str);
                break;
            case 'd':
                num = (U16)va_arg(ap, int);
                LOG_LoggingNum(num, FDNS_DIGIT_5);
                break;
            case 'i':
                num = (U16)va_arg(ap, int);
                LOG_LoggingNum(num, FDNS_DIGIT_5|FDNS_SIGNED);
                break;
            case 'x':
                num = (U16)va_arg(ap, int);
                attrib = FDNS_NULL;
                LOG_LoggingNum(num, attrib|FDNS_HEX);
                break;
        }
        format++;
    }
    va_end(ap);
}

#endif

