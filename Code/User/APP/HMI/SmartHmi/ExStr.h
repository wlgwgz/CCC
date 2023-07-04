/******************************************************************************

          ��Ȩ���� (C), 2001-2016, �����а��յ��Լ����������޹�˾

 ******************************************************************************
  �� �� ��   : ExStr.h
  �� �� ��   : ����
  ��    ��   : ��־��
  ��������   : 2018��12��24������һ
  ����޸�   :
  ��������   : ExStr.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��12��24������һ
    ��    ��   : ��־��
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __EXSTR_H__
#define __EXSTR_H__

/*-----------------����ͷ�ļ� -----------------------------*/



/*-----------------�ⲿ����˵��----------------------------*/



/*-----------------�ⲿ����ԭ��˵��------------------------*/



/*-----------------�ڲ�����ԭ��˵��------------------------*/



/*-----------------ȫ�ֱ���--------------------------------*/



/*-----------------ģ�鼶����------------------------------*/



/*-----------------��������--------------------------------*/
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

/*-----------------�궨�� ---------------------------------*/





extern U08 EXSTR_COM_SlvReadStr(U32 file_addr, U08 block_size, U08 *TxBuf, U16 *pTxTotal);
#ifdef USE_GBK
U16 EXSTR_ChcodeStr(U08* dest, U16 dest_len, const U08* hzk_str);
#endif
#ifdef USE_UNICODE
U16 EXSTR_DecodeStr(U08* dest, U16 dest_len, const U08* hzk_str);
#endif

#endif /* __APPSTR_EX_H__ */

