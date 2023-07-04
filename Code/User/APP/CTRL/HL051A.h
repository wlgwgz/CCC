/******************************************************************************

          版权所有 (C), 2001-2016, 广州市邦普电脑技术开发有限公司

 ******************************************************************************
  文 件 名   : HL051A.h
  版 本 号   : 初稿
  作    者   : 周志鹏
  生成日期   : 2018年3月29日星期四
  最近修改   :
  功能描述   : HL051A.h 的头文件,包含硬件的资源定义
  函数列表   :
  修改历史   :
  1.日    期   : 2018年3月29日星期四
    作    者   : 周志鹏
    修改内容   : 创建文件

******************************************************************************/
#ifndef __HL051A_H__
#define __HL051A_H__


/* 开关输入量编号定义 */
#define DI_01	         0	
#define DI_02	         1	
#define DI_03	         2	
#define DI_04	         3	
#define DI_05	         4	
#define DI_06	         5	
#define DI_07	         6	
//以下是强电开关量1-5
#define DI_08	         7	
#define DI_09	         8	
#define DI_10	         9	
#define DI_11	         10	
#define DI_12	         11
#define GET_DI(n)       (gIO.input[n>>4] & (1<<(n)))


/* 开关输出量编号定义 */
#define DO_01	0	
#define DO_02	1	
#define DO_03	2	
#define DO_04	3	
#define DO_05	4	
#define DO_06	5	
#define DO_07	6	
#define DO_08	7	
#define DO_09	8	
#define DO_10	9	
#define DO_11	10	
#define DO_12	11	
#define DO_13	12	
#define DO_14	13	
#define DO_15	14	
#define DO_16	15	
#define DO_17	16	
#define DO_18	17
#define DO_MAX  18
#define SET_DO(n)       SETBIT(gIO.output[n>>4], (n&0x0f))
#define DO_BIT(n)       (1<<(n))

/* 温度输入编号定义 */
#define TEMP_01	         0
#define TEMP_02	         1
#define TEMP_03          2
#define TEMP_04          3
#define TEMP_05          4
#define TEMP_06          5
#define TEMP_07          6
#define TEMP_08          7
#define TEMP_09          8
#define TEMP_10          9
#define TEMP_11          10
#define TEMP_12          11
/* 外机商检时不检测内机模拟量 */
#define GET_TEMP(n)     (TF_IS_OUT ? SENSOR_NOEXIST : fun_pv_div_10(gIO.temp[n])) //温度0.01转为0.1

/* 模拟量输入编号定义 */
#define AI_01            0
#define AI_02            1
#define AI_03            2
#define GET_AI(n)       0//(gIO.ai[n])


/* 电流输入编号定义 */
#define CUR_01           0
#define CUR_02           1
#define GET_CUR(n)      SENSOR_NOEXIST//(gIO.cur[n])

/* 拨码编号定义 */
#define JMP_01	         0	
#define JMP_02	         1	
#define JMP_03	         2	
#define JMP_04	         3	
#define JMP_05	         4	
#define JMP_06	         5	
#define JMP_07	         6	
#define JMP_08	         7	
#define JMP_09	         8	
#define JMP_10	         9	
#define JMP_11	         10	
#define JMP_12	         11
#define JMP_13	         12	
#define JMP_14	         13
#define GET_JMP(n)       (gIO.jmp[n>>4] & (1<<(n)))

/* 使用BMS仿真硬件资源时定义 */
#ifdef USE_BMS_SOURCE
//#undef  GET_DI
//#define GET_DI(n)       GETBIT(bms_source.src[0].DI[(n)>>4], (n)&0x0f)
//#undef  GET_DI_W
//#define GET_DI_W(w)     (bms_source.src[0].DI[w])
#undef  GET_TEMP
#define GET_TEMP(n)      (bms_source.src[0].temp[n])
#undef  GET_AI
#define GET_AI(n)       (bms_source.src[0].ai[n])
#undef  GET_CUR
#define GET_CUR(n)      (bms_source.src[0].cur[n])
#undef  GET_HUMI
#define GET_HUMI(n)     (bms_source.src[0].humi[n])
#endif

#endif /* __HL051A_H__ */

