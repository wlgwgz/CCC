/******************************************************************************

          版权所有 (C), 2001-2014, 广州市邦普电脑技术开发有限公司

 ******************************************************************************
  文 件 名   : Hdw_Ai.c
  版 本 号   : 初稿
  作    者   : 周志鹏
  生成日期   : 2016年11月7日星期一
  最近修改   :
  功能描述   : 硬件上处理AI读取
  函数列表   :
              ADC_SampleAi
  修改历史   :
  1.日    期   : 2016年11月7日星期一
    作    者   : 周志鹏
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "Mcu_def.h"
#include "Hdw_Main.h"
#include "Hdw_Ai.h"
#include "Hdw_Eeprom.h"

#include "Includes.h"
#if MAX_AI_NUM != 0
/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/* 外部函数原型说明   */
extern void MCU_GetAllAiAdCh(U16 Ai_Ad[]);

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/


GSTATIC I16 Ai_Type[MAX_AI_NUM];
GSTATIC I16 Hdw_Ai[MAX_AI_NUM];
//GSTATIC LineCoe AiCoe[MAX_AI_NUM][MAX_AI_TYPE];      
       
/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/
typedef struct tagHDW_AI_TBL {
    U08 ad_idx;         // AD编号索引
}HDW_AI_TBL;
const HDW_AI_TBL HdwAiAdTbl [MAX_AI_NUM] = {
    {AD_IDX_AI_1 }, 
    {AD_IDX_AI_2 }, 
};
#if MAX_AI_NUM != 2
    #error "修改"
#endif
/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

void HDW_Ai_Init(void)
{
    U08 i;
//    BOOL rd_succ = FALSE;

    /* 读取校准值 */
//  rd_succ = eprom_rd_near_crc(&AiCoe[0][0], EEPROMADDR_ADJUST_AI,sizeof(AiCoe));

//  if(!rd_succ)
//  {
//      for (i=0; i<MAX_AI_NUM; i++)
//      {
//          AiCoe[i][0].A =  (U16)((U32)150*16*1023/AD_ADJ);//4-20
//          AiCoe[i][0].B = 0;
//          AiCoe[i][1].A =  (U16)((U32)600*16*1023/AD_ADJ);//0-5
//          AiCoe[i][1].B = 0;
//          AiCoe[i][2].A =  (U16)((U32)300*16*1023/AD_ADJ);//0-10
//          AiCoe[i][2].B = 0;
//      }            
//  }

    /* 初始化AI类型 */
    for (i=0; i<MAX_AI_NUM; i++)
    {
        Ai_Type[i] = AI_NONE;
    }

    /* 初始化AI变量 */
    for (i=0; i<MAX_AI_NUM; i++)
    {
        Hdw_Ai[i] = SENSOR_NOEXIST;
    }
}

//BOOL HDW_SaveAiAdj(U08 num, U08 type, U16 a, I16 b)
//{
//  if (num >= MAX_AI_NUM || type >= MAX_AI_TYPE)
//      return FALSE;

//  AiCoe[num][type].A = (U16)a;
//  AiCoe[num][type].B = (I16)b;
    
//  return eprom_wr_near_crc(&AiCoe[0][0], EEPROMADDR_ADJUST_AI,sizeof(AiCoe));
//}



/*****************************************************************************
函  数 : HDW_AdToAiSignal()
功  能 : 直接读取校准后模拟量信号，3位小数
参  数 : I16 type : 
         U16 ad   : 
返  回 : 无
其  他 : 无
*****************************************************************************/
I16 HDW_AdToAiSignal(I16 type, U16 ad)
{ 
    I16 signal = ~SENSOR_NOEXIST; // 原为SENSOR_NOEXIST;
    U16 ad_short,ad_break;
    U32 a,b;


    switch(type)
    {
        case AI_05_45V:
            ad_short = ((U32)3200*AD_MAX)/AD_VREF;//当超过3.20V时认为短路
            ad_break = ((U32)20*AD_MAX)/AD_VREF;//当低于0.02V时认为断路
            break;

        case AI_NONE:
//        case AI_4_20MA:
//        case AI_0_5V:
//        case AI_0_10V:            
        default: 
            signal =  SENSOR_NOEXIST;
            break;            
    }
        
    if(signal != SENSOR_NOEXIST)
    {
        if(ad > ad_short)//短路
        {
            signal = SENSOR_SHORTED;
        }
        else if(ad < ad_break)//断路
        {
            signal = SENSOR_BREAKED;
        }
        else
        {
            switch(type)
            {
                case AI_05_45V:    
                    //      11        ad
                    // V*-------- = --------*AD_VREF
                    //    11+6.8     AD_MAX
                    a = ((U32)ad*(110+68)*(AD_VREF/10));
                    b = (110*(AD_MAX/10));
                    signal = (I16)(a/b); // 单位mV   
                    break;
                
//                case AI_4_20MA:
//                case AI_0_5V:
//                case AI_0_10V:   
                case AI_NONE:    
                default:            
                    signal = SENSOR_NOEXIST;
                    break;        
            }
        }
    }
    
    return signal;
    
}



/*****************************************************************************
函  数 : HDW_GetAllAi()
功  能 : 硬件底层读取模拟量,在硬件层主调度调用
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
void HDW_GetAllAi(void)
{
    U08 i;
    U16 ad;

        
    for (i=0; i<MAX_AI_NUM; i++)
    {
        if (HDW_AdcUpdateAdVal(&ad, HdwAiAdTbl[i].ad_idx, HDW_UPDATE_VOL_BY_ONE_TIMES))
        {

            TestCurCurAd(i) = ad;    // 显示电压

            ad = HDW_AdcCompensate(ad);

            Hdw_Ai[i] = HDW_AdToAiSignal(Ai_Type[i], ad);
        }
    }
}


/*****************************************************************************
函  数 : PunpGetAi()
功  能 : 读取模拟量接口函数
参  数 : I16 AiBuf[] : 模拟量缓存区
         I16 AiType[]: 模拟量类型
         U16 AiNum   : 模拟量个数
返  回 : TRUE成功/FALSE失败
其  他 : 模拟量类型改变时,可能有数次数据是无效的
*****************************************************************************/
BOL PunpGetAi(I16 AiBuf[], I16 AiType[], U16 AiNum)
{
    U08 i;
    
    if (AiNum > MAX_AI_NUM)
        return FALSE;

    for (i=0; i<AiNum; i++)
    {
        Ai_Type[i] = AiType[i];
        AiBuf[i] = Hdw_Ai[i];
    }    

    return TRUE;    
}
#endif
