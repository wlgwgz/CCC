/******************************************************************************

          版权所有 (C), 2001-2016, 广州市邦普电脑技术开发有限公司

 ******************************************************************************
  文 件 名   : Hdw_Main.h
  版 本 号   : 初稿
  作    者   : 周志鹏
  生成日期   : 2016年11月9日星期三
  最近修改   :
  功能描述   : Hdw_Main.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2016年11月9日星期三
    作    者   : 周志鹏
    修改内容   : 创建文件

******************************************************************************/

#ifndef __HDW_MAIN_H__
#define __HDW_MAIN_H__


/*-----------------包含头文件 -----------------------------*/
#include "Mcu_def.h"
#include "Hdw_Eeprom.h"
#include "Hdw_Timer.h"
#include "Hdw_IO.h"



#define AD_VREF          3250UL             //3.250V

/*==============================================================================
注意:若更改了MAX_AD_FILTER_NUM，则需在AD_Filter()中更新
==============================================================================*/
 #define MAX_AD_FILTER_NUM   6           /* 滤波时最大的样点数 */
#define AD_OVER_SAMPLE_NUM  16           /* 次过采样次数 */
#define AD_UNUSE_SAMPLE_NUM 1            /* 无用的转换,用于丢弃前几次不稳定的AD */
//#define AD_UNUSE_SAMPLE_NUM 6           /* 每次重新启动ADC则可以不丢弃 */
#define MAX_AD_CONVER_NUM   (AD_UNUSE_SAMPLE_NUM + AD_OVER_SAMPLE_NUM) /* 总采用次数 */

#define AD_MAX  (4095UL*AD_OVER_SAMPLE_NUM)  /* AD最大值(4095*16，10位AD、16次累加) */

#define MAX_MOTOR           MAX_MOTOR_NUM   //最大步进电机数


#define TMP_PT1000          1           /* 选择温度探头为PT1000*/
#define TMP_NTC             0           /* 选择温度探头为NTC*/


//模拟量输入类型
#define AI_NONE             0
#define AI_05_45V           1   //NOTE-CXW 可能影响到8寸屏测试，待确认
//#define AI_4_20MA           2
//#define AI_0_5V             3
//#define AI_0_10V            4



#define IN_74HC165_BITS     (MAX_SW_BIT+MAX_SWPL_BIT+MAX_DI_NUM+1)   // 74HC165串行输入个数 14+4+7+1 1为U13的H位
#define IN_74HC165_W        (_DIVUCARRY(IN_74HC165_BITS, 16))

#define IN_PARALLEL_BITS    (0)  // 并行输入个数
#define IN_PARALLEL_W       (_DIVUCARRY(IN_PARALLEL_BITS, 16))

// 开关量输入总数(包括DI,SW,PL等)
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



#define SENSOR_MIN          -32699      /* 探头最小值 */
#define SENSOR_NOEXIST      -32700      /* 探头不存在 */
#define SENSOR_ERROR        -32701      /* 探头故障 */
#define SENSOR_BREAKED      -32702      /* 探头断路 */
#define SENSOR_SHORTED      -32703      /* 探头短路 */

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
