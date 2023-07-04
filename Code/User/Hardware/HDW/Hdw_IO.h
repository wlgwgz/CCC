/******************************************************************************

          版权所有 (C), 2001-2014, 广州市邦普电脑技术开发有限公司

 ******************************************************************************
  文 件 名   : Hdw_IO.h
  版 本 号   : 初稿
  作    者   : 
  生成日期   : 2016年11月8日星期二
  最近修改   :
  功能描述   : Hdw_IO.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2016年11月8日星期二
    作    者   : 
    修改内容   : 创建文件

******************************************************************************/
#ifndef __HDW_IO_H__
#define __HDW_IO_H__
/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/



#define MAX_PHASE_SHAKE       5   /* 相序消抖一次读取的次数 */


#define PHASE_TIMES         100     /*  一个相序测试周期为100ms */
#define PHASE_OK_CNT        5       /*  正常相序的一半 */
#define PHASE_ERR_CNT       -5      /*  逆相的测试结果 */
#define PHASE_LACK_CNT      2       /*  缺相、欠压的测试结果 */

/* 温度数据 */
enum 
{
    PHASE_STA_OK=0,                 /* 正相 */
    PHASE_STA_ERR,                  /* 错相 */
    PHASE_STA_LACK,                 /* 缺相 */
};

//#define MAX_SW_BIT          (MAX_SW1_BIT+MAX_SW2_BIT+MAX_SWPL_BIT+MAX_SW4_BIT+MAX_SW5_BIT) // 18 SW输入
//#define MAX_SWPL_BIT        4
#define MAX_SWITCH_SHAKE    5   /* 拨码/盘消抖一次读取的次数 */

//#define MAX_COMP						2		//单模块最大压机数	  //NOTE-CXW 根据应用程序变的量不要放在HDW

#define PINS_NUM_SW         MAX_SW_BIT

#define MAX_LED_NUM         2
#define LED_RED_IDX         0           /* 红灯编号 */
#define LED_ORG_IDX         1           /* 橙灯编号 */

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
