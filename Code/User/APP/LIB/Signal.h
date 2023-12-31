/******************************************************************************

          版权所有 (C), 2001-2016, 广州市邦普电脑技术开发有限公司

 ******************************************************************************
  文 件 名   : Signal.h
  版 本 号   : 初稿
  作    者   : 周志鹏
  生成日期   : 2019年1月10日星期四
  最近修改   :
  功能描述   : Signal.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2019年1月10日星期四
    作    者   : 周志鹏
    修改内容   : 创建文件

******************************************************************************/

#ifndef __SIGNAL_H__
#define __SIGNAL_H__



/*-----------------包含头文件 -----------------------------*/



/*-----------------外部变量说明----------------------------*/



/*-----------------外部函数原型说明------------------------*/



/*-----------------内部函数原型说明------------------------*/



/*-----------------全局变量--------------------------------*/



/*-----------------模块级变量------------------------------*/



/*-----------------常量定义--------------------------------*/
typedef enum {
    SIGNAL_NONE=0,  /* 无信号 */
    SIGNAL_ASK   ,  /* 请求:发起方写,接收方读 */
    SIGNAL_REFUSE,  /* 拒绝:接收方写,发起方读 */
    SIGNAL_ALLOW ,  /* 允许:接收方写,发起方读 */
    SIGNAL_END      /* 结束:发起方写,接收方读 */
}SIGNALSTA;



/*-----------------宏定义 ---------------------------------*/




extern BOL SIGNAL_Accept(SIGNALSTA* p_signal, BOL allow);
extern void SIGNAL_AcceptEnd(SIGNALSTA* p_signal);
extern void SIGNAL_Clear(SIGNALSTA* p_signal);
extern BOL SIGNAL_Launch(SIGNALSTA* p_signal, BOL evt);
extern void SIGNAL_LaunchEnd(SIGNALSTA* p_signal);



#endif /* __SIGNAL_H__ */

