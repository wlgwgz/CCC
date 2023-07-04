/******************************************************************************

          版权所有 (C), 2001-2016, 广州市邦普电脑技术开发有限公司

 ******************************************************************************
  文 件 名   : ExStr.h
  版 本 号   : 初稿
  作    者   : 周志鹏
  生成日期   : 2018年12月24日星期一
  最近修改   :
  功能描述   : ExStr.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2018年12月24日星期一
    作    者   : 周志鹏
    修改内容   : 创建文件

******************************************************************************/
#ifndef __EXSTR_H__
#define __EXSTR_H__

/*-----------------包含头文件 -----------------------------*/



/*-----------------外部变量说明----------------------------*/



/*-----------------外部函数原型说明------------------------*/



/*-----------------内部函数原型说明------------------------*/



/*-----------------全局变量--------------------------------*/



/*-----------------模块级变量------------------------------*/



/*-----------------常量定义--------------------------------*/
#define STR_EX_CH_BGN   0
#define STR_EX_CH_END   MSG_IDX_MAX

#define STR_EX_EN_BGN   0
#define STR_EX_EN_END   MSG_IDX_MAX

#define STR_EX_ES_BGN   0
#define STR_EX_ES_END   MSG_IDX_MAX

#define STR_EX_GR_BGN   0
#define STR_EX_GR_END   MSG_IDX_MAX

#define STR_EX_FR_BGN   0
#define STR_EX_FR_END   MSG_IDX_MAX

#define STR_EX_IT_BGN   0
#define STR_EX_IT_END   MSG_IDX_MAX

#define STR_EX_PL_BGN   0
#define STR_EX_PL_END   MSG_IDX_MAX

#define STR_EX_HG_BGN   0
#define STR_EX_HG_END   MSG_IDX_MAX

#define STR_EX_GE_BGN   0
#define STR_EX_GE_END   MSG_IDX_MAX

/*-----------------宏定义 ---------------------------------*/





extern U08 EXSTR_COM_SlvReadStr(U32 file_addr, U08 block_size, U08 *TxBuf, U16 *pTxTotal);
#ifdef USE_GBK
U16 EXSTR_ChcodeStr(U08* dest, U16 dest_len, const U08* hzk_str);
#endif
#ifdef USE_UNICODE
U16 EXSTR_DecodeStr(U08* dest, U16 dest_len, const U08* hzk_str);
#endif

#endif /* __APPSTR_EX_H__ */

