/******************************************************************************

          版权所有 (C), 2001-2014, 广州市邦普电脑技术开发有限公司

 ******************************************************************************
  文 件 名   : Hdw_Ai.h
  版 本 号   : 初稿
  作    者   : // zzp v160a7
  生成日期   : 2016年11月7日星期一
  最近修改   :
  功能描述   : Hdw_Ai 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2016年11月7日星期一
    作    者   : // zzp v160a7
    修改内容   : 创建文件

******************************************************************************/
#ifndef __HDW_AI_H__
#define __HDW_AI_H__

#if MAX_AI_NUM != 0
#define MAX_AI_TYPE         3//3类，无不算在里面

#define MAX_AI_CH_NUM       4                   /* 最大的模拟量通道数*/

/* 输入模拟量数据 */
typedef struct tagAI
{
    U08 cnt;                /* AD采样计数器 */
    U16 adBuf[MAX_AI_CH_NUM][MAX_AD_FILTER_NUM];            /* AD采样缓冲区 */                            
} AI_DATA;  

//typedef struct tagLineCoe
//{
//    U16 A;//模拟量的1/斜率，3位小数一定为正数，故用无符号型，防止超界
//    I16 B;//截距，3位小数，可能为负数，故有符号数
//}LineCoe;


extern BOOL HDW_SaveAiAdj(U08 num, U08 type, U16 a, I16 b);
extern void HDW_Ai_Init(void);

extern BOL PunpGetAi(I16 AiBuf[], I16 AiType[], U16 AiNum);
#endif
#endif /* __HDW_AI_H__ */
