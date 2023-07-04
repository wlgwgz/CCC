/******************************************************************************

          版权所有 (C), 2001-2016, 广州市邦普电脑技术开发有限公司

 ******************************************************************************
  文 件 名   : Signal.c
  版 本 号   : 初稿
  作    者   : 周志鹏
  生成日期   : 2019年1月10日星期四
  最近修改   :
  功能描述   : 信号量处理
  函数列表   :
              SIGNAL_Accept
              SIGNAL_AcceptEnd
              SIGNAL_Clear
              SIGNAL_Launch
              SIGNAL_LaunchEnd
  修改历史   :
  1.日    期   : 2019年1月10日星期四
    作    者   : 周志鹏
    修改内容   : 创建文件

******************************************************************************/

/*-----------------包含头文件 -----------------------------*/
#include "Includes.h"
//#include "Libs.h"


/*-----------------信号处理流程----------------------------*/
/*                                             
        信号发起方                          信号接收方
            ↓
            ↓事件发生
            ↓
     请求  ASK 
            ↓
            ---------------------------------→allow?
                                                 ↓                                       
                                                 ↓                                       
            ↓←---------------------------------↓ALLOW   允许                                   
            ↓                                   ↓     或                                 
            ↓←---------------------------------↓REFUSE  拒绝                                    
            ↓                                                                           
     结束  END                                                                             
            ↓                                                                           
            ↓---------------------------------→↓                                                                         
                                                 ↓                                      
                                                NONE                                       
                                                                                       
        SIGNAL_Launch()                     SIGNAL_Accept()
*/


/*****************************************************************************
函  数 : SIGNAL_Clear()
功  能 : 信号清零
参  数 : SIGNALSTA* p_signal : 信号量指针
返  回 : 无
其  他 : 无
*****************************************************************************/
void SIGNAL_Clear(SIGNALSTA* p_signal)
{
    *p_signal = SIGNAL_NONE;
}


/*****************************************************************************
函  数 : SIGNAL_Launch()
功  能 : 信号发起方处理
参  数 : SIGNALSTA* p_signal : 信号量指针
         BOL evt             : 发生时间
返  回 : TRUE事件有效,FALSE事件无效
其  他 : 例如:发生低压时,先申请除霜;被拒绝则事件有效(报低压),允许则不报低压
*****************************************************************************/
BOL SIGNAL_Launch(SIGNALSTA* p_signal, BOL evt)
{
    /* 默认事件无效 */
    BOL evt_eft = FALSE; 

    /* 发生事件,判断事件是否有效 */
    if (evt)
    {
        switch (*p_signal)
        {
            case SIGNAL_NONE:   /* 当前无信号,则请求 */
                *p_signal = SIGNAL_ASK;
                break;
            case SIGNAL_REFUSE: /* 被拒绝,evt事件有效,信号结束 */
                evt_eft = TRUE;
                *p_signal = SIGNAL_END;
                break;
            case SIGNAL_ALLOW:  /* 允许,evt事件无效,信号结束 */
                evt_eft = FALSE;
                *p_signal = SIGNAL_END;
                break;
            default:
                break;
        } 
    }
    else
    {
        /* 无事件,信号清零 */
        *p_signal = SIGNAL_NONE;
    }

    /* 返回判断后的结果 */
    return evt_eft;
}

/*****************************************************************************
函  数 : SIGNAL_LaunchEnd()
功  能 : 信号发起方后处理,如果信息发起函数有条件执行,则该函数必须执行
参  数 : SIGNALSTA* p_signal : 信号量指针
返  回 : 无
其  他 : 相当于最高层级调用,防止死信号
*****************************************************************************/
void SIGNAL_LaunchEnd(SIGNALSTA* p_signal)
{
    switch (*p_signal)
    {
        /* 若信号未被发起方处理,则默认处理,并结束信号 */
        case SIGNAL_REFUSE:
        case SIGNAL_ALLOW:
            *p_signal = SIGNAL_END;
            break;
        default:
            break;
    }
}

/*****************************************************************************
函  数 : SIGNAL_Accept()
功  能 : 信号接收方处理
参  数 : SIGNALSTA* p_signal : 信号量指针
         BOL allow           : 是否允许
返  回 : TRUE允许,FALSE不允许
其  他 : 例如:发生低压除霜请求时,如果达到低压除霜条件,则允许,则不允许
*****************************************************************************/
BOL SIGNAL_Accept(SIGNALSTA* p_signal, BOL allow)
{
    /* 默认不接受 */
    BOL accept = FALSE;
    
    switch (*p_signal)
    {
        case SIGNAL_NONE:   /* 接收方不处理"无信号" */
            break;
        case SIGNAL_ASK:    /* 请求:根据是否allow判断是"接收"还是"拒绝" */
            if (allow)
            {
                *p_signal = SIGNAL_ALLOW;   /* 接受 */
                accept = TRUE;
            }
            else
            {
                *p_signal = SIGNAL_REFUSE;  /* 拒绝 */
            }
            break;
        case SIGNAL_ALLOW:  /* 接收方不处理"接收" */
        case SIGNAL_REFUSE: /* 接收方不处理"拒绝" */
            break;
        case SIGNAL_END:    /* 信号结束,清信号 */
        default:
            *p_signal = SIGNAL_NONE;
            break;
    }


    /* 返回是否接受 */
    return accept;
}


/*****************************************************************************
函  数 : SIGNAL_AcceptEnd()
功  能 : 信号发起方后处理,如果信息接收函数有条件执行,则该函数必须执行
参  数 : SIGNALSTA* p_signal : 信号量指针
返  回 : 无
其  他 : 相当于最高层级调用,防止死信号
*****************************************************************************/
void SIGNAL_AcceptEnd(SIGNALSTA* p_signal)
{
    /* 若信号未被接受方处理,则默认拒绝 */
    (void)SIGNAL_Accept(p_signal, FALSE);
}


