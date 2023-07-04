/******************************************************************************

          版权所有 (C), 2001-2014, 广州市邦普电脑技术开发有限公司

 ******************************************************************************
  文 件 名   : Hdw_Timer.h
  版 本 号   : 初稿
  作    者   : // zzp v160a7
  生成日期   : 2016年11月8日星期二
  最近修改   :
  功能描述   : Hdw_Timer.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2016年11月8日星期二
    作    者   : // zzp v160a7
    修改内容   : 创建文件

******************************************************************************/

#ifndef __HDW_TIMER_H__
#define __HDW_TIMER_H__



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
    
#define MAX_PWCIN_FILTER    6      // 滤波次数
typedef struct tagPWCIN
{
    U08 edge;                      // 扫描完成bit0:低,bit1:高
    U08 level;
    BOL cal;
    BOL init;                      // 已触发
    BOL lowfull;                   // 全低电平
    BOL highfull;                  // 全高电平    
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
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
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
