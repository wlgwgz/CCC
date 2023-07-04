/******************************************************************************

          ��Ȩ���� (C), 2001-2016, �����а��յ��Լ����������޹�˾

 ******************************************************************************
  �� �� ��   : ExStr.c
  �� �� ��   : ����
  ��    ��   : ��־��
  ��������   : 2019��1��25��������
  ����޸�   :
  ��������   : �����ַ���Э��
  �����б�   :
              EXSTR_COM_ReadStr
              EXSTR_COM_SlvReadStr
              EXSTR_DecodeChr
              EXSTR_DecodeStr
  �޸���ʷ   :
  1.��    ��   : 2019��1��25��������
    ��    ��   : ��־��
    �޸�����   : �����ļ�

******************************************************************************/

/*-----------------����ͷ�ļ� -----------------------------*/
#include "Includes.h"


/*-----------------����ظ�Э�� -----------------------------*/
/*
01  44      [00 00] [01 02  03  04] [nn ]  [�û����ݿ�       ] [5A 8B]
|   |       |       |               |      |                   |   |
|   |       |       |               |      |                   |   CRC��
|   ������  |       |               |      |                   CRC��
�豸��ַ    �ļ���  ��ַ            |      |
                                    �ֽ���
*/


/*-----------------�ⲿ����˵��----------------------------*/



/*-----------------�ⲿ����ԭ��˵��------------------------*/



/*-----------------�ڲ�����ԭ��˵��------------------------*/



/*-----------------ȫ�ֱ���--------------------------------*/



/*-----------------ģ�鼶����------------------------------*/



/*-----------------��������--------------------------------*/



/*-----------------�궨�� ---------------------------------*/






#define EX_MAX_TX_BUF    200

/*****************************************************************************
                            �ӿں���
*****************************************************************************/



#ifdef USE_UNICODE
/*****************************************************************************
��  �� : EXSTR_DecodeChr()
��  �� : HZK�ַ��������ΪUNICODE
��  �� : U16 hzk_code : HZK����
��  �� : ��
��  �� : ��
*****************************************************************************/
U16 EXSTR_DecodeChr(U16 hzk_code)
{
    U16 unic = 0;

    unic = hzk_code;

//    if(hzk_code < 0x0080)
//    {
//        unic = hzk_code;
//    }
//    else if(hzk_code >= HZK_CH_BGN && hzk_code < HZK_CH_END)
//    {
//        unic = hzk2unicode_tab[hzk_code-HZK_CH_BGN];
//    }

    return unic;
}

/*****************************************************************************
��  �� : EXSTR_DecodeStr()
��  �� : HZK�ַ�������ΪUNICODE�ַ���
��  �� : U08* dest          : Ŀ���ַ�������
         U16 dest_len       : Ŀ���ַ�������
         const U08* hzk_str : hzk�ַ���ָ��
��  �� : �����ĳ���
��  �� : ����������
*****************************************************************************/
U16 EXSTR_DecodeStr(U08* dest, U16 dest_len, const U08* hzk_str)
{
    U32 len = 0;
    U16 char_tmp;
    U16 char_tmp2;
    U16 unic_tmp;

    while(1)
    {
        char_tmp = *hzk_str++;
        char_tmp2 = *hzk_str++;
        unic_tmp = MAKEWORD(char_tmp, char_tmp2);

        if (len+1 >= dest_len)
        {
            break;
        }

        dest[len++] = HIBYTE(unic_tmp);
        dest[len++] = LOBYTE(unic_tmp);  /* UNICODE���ߵ��ֽ���appstr.c�е�һ�� */

        if (unic_tmp == 0xffff) /* ��Ҫ������ */
        {
            break;
        }
    }

    return len;
}
#endif

#ifdef USE_GBK
/*****************************************************************************
��  �� : EXSTR_ChcodeChr()
��  �� : HZK�ַ�ת��ΪGBK
��  �� : U16 hzk_code : HZK����
��  �� : ��
��  �� : ��
*****************************************************************************/
U16 EXSTR_ChcodeChr(U16 hzk_code)
{
    U16 unic = 0;

    if(hzk_code < 0x0080)
    {
        unic = hzk_code;
    }
    else if(hzk_code >= HZK_CH_BGN && hzk_code < HZK_CH_END)
    {
        unic = hzk2gbk_tab[hzk_code-HZK_CH_BGN];
    }

    return unic;
}

/*****************************************************************************
��  �� : EXSTR_ChcodeStr()
��  �� : HZK�ַ���ת��ΪGBK�ַ���
��  �� : U08* dest          : Ŀ���ַ�������
         U16 dest_len       : Ŀ���ַ�������
         const U08* hzk_str : hzk�ַ���ָ��
��  �� : �����ĳ���
��  �� : ����������
*****************************************************************************/
U16 EXSTR_ChcodeStr(U08* dest, U16 dest_len, const U08* hzk_str)
{
    U32 len = 0;
    U16 char_tmp;
    U16 char_tmp2;
    U16 unic_tmp;
    U08 chr_len;

    while(1)
    {
        chr_len = 1;
        char_tmp = *hzk_str++;
        if(char_tmp >= 0x80)    /* ����,2�ֽ� */
        {
            char_tmp2 = *hzk_str++;
            char_tmp = MAKEWORD(char_tmp, char_tmp2);
            chr_len = 2;
        }

        unic_tmp = EXSTR_ChcodeChr(char_tmp); /* תΪUNICODE���� */

        if (len+1 >= dest_len)
        {
            break;
        }

        if (chr_len == 1)                    /* ���ֽ� */
        {
            dest[len++] = LOBYTE(unic_tmp);
        }
        else
        {
            dest[len++] = HIBYTE(unic_tmp); /* ��ǰ�ͺ� */
            dest[len++] = LOBYTE(unic_tmp);
        }

        if (unic_tmp == 0) /* ��Ҫ������ */
        {
            break;
        }
    }

    return len;
}
#endif


#define EX_SUB_CMD_INFO     1
#define EX_SUB_CMD_CN_STR   2				/* ͬ�������ַ��� */
#define EX_SUB_CMD_EN_STR   3				/* ͬ��Ӣ���ַ��� */
#define EX_SUB_CMD_ES_STR	4				/* ͬ�����������ַ��� */
#define EX_SUB_CMD_GR_STR   5				/* ͬ��ϣ�����ַ��� */
#define EX_SUB_CMD_FR_STR   6				/* ͬ�������ַ��� */
#define EX_SUB_CMD_IT_STR   7				/* ͬ����������ַ��� */
#define EX_SUB_CMD_PL_STR   8				/* ͬ���������ַ��� */
#define EX_SUB_CMD_HG_STR   9               /* ͬ�����������ַ��� */
#define EX_SUB_CMD_GE_STR   10				/* ͬ���������ַ��� */

U08 EXSTR_COM_ReadStr(U08* dest, U08 buf_size, U08 lang, U16 idx)
{
    U32 unit_len;
    U32 unic_len;
    U08 tx_idx = 0;
    const U08* str_ptr=NULL;
    U08 buf[EX_MAX_TX_BUF];
    U08 max_len;
    U16 idx_bgn;
    U16 idx_end;

    //4 ���������
    max_len = MIN(buf_size, EX_MAX_TX_BUF);

    //4 ����������Ϣ
    switch (lang)
    {
        case EX_SUB_CMD_CN_STR:
            idx_bgn = STR_EX_CH_BGN;
            idx_end = STR_EX_CH_END;
            break;
        default:
        case EX_SUB_CMD_EN_STR:
            idx_bgn = STR_EX_EN_BGN;
            idx_end = STR_EX_EN_END;
            break;
        case EX_SUB_CMD_ES_STR:
            idx_bgn = STR_EX_ES_BGN;
            idx_end = STR_EX_ES_END;
            break;
        case EX_SUB_CMD_GR_STR:
            idx_bgn = STR_EX_GR_BGN;
            idx_end = STR_EX_GR_END;
            break;
        case EX_SUB_CMD_FR_STR:
            idx_bgn = STR_EX_FR_BGN;
            idx_end = STR_EX_FR_END;
            break;
        case EX_SUB_CMD_IT_STR:
            idx_bgn = STR_EX_IT_BGN;
            idx_end = STR_EX_IT_END;
            break;
        case EX_SUB_CMD_PL_STR:
            idx_bgn = STR_EX_PL_BGN;
            idx_end = STR_EX_PL_END;
            break;
        case EX_SUB_CMD_HG_STR:
            idx_bgn = STR_EX_HG_BGN;
            idx_end = STR_EX_HG_END;
            break;
        case EX_SUB_CMD_GE_STR:
            idx_bgn = STR_EX_GE_BGN;
            idx_end = STR_EX_GE_END;
            break;
    }

    //4 ����ַ���
    while(1)
    {
        if(idx < idx_bgn || idx >= idx_end)
        {
            break;
        }

        switch (lang)
        {
            case EX_SUB_CMD_CN_STR:
#ifdef USE_GBK
                str_ptr = IDX_TO_C_PTR(idx-idx_bgn);
#elif defined (USE_UNICODE)
                str_ptr = chinese_uncompress_text(idx-idx_bgn);
#endif
                break;

            default:
            case EX_SUB_CMD_EN_STR:
#ifdef USE_GBK
                str_ptr = IDX_TO_E_PTR(idx-idx_bgn);
#elif defined (USE_UNICODE)
                str_ptr = english_uncompress_text(idx-idx_bgn);
#endif
                break;

            case EX_SUB_CMD_ES_STR:
#ifdef USE_GBK
                str_ptr = IDX_TO_C_PTR(idx-idx_bgn);
#elif defined (USE_UNICODE)
                str_ptr = spanish_uncompress_text(idx-idx_bgn);
#endif
                break;

            case EX_SUB_CMD_GR_STR:
#ifdef USE_GBK
                str_ptr = IDX_TO_C_PTR(idx-idx_bgn);
#elif defined (USE_UNICODE)
                str_ptr = greek_uncompress_text(idx-idx_bgn);
#endif
                break;

            case EX_SUB_CMD_FR_STR:
#ifdef USE_GBK
                str_ptr = IDX_TO_C_PTR(idx-idx_bgn);
#elif defined (USE_UNICODE)
                str_ptr = french_uncompress_text(idx-idx_bgn);
#endif
                break;

            case EX_SUB_CMD_IT_STR:
#ifdef USE_GBK
                str_ptr = IDX_TO_C_PTR(idx-idx_bgn);
#elif defined (USE_UNICODE)
                str_ptr = italian_uncompress_text(idx-idx_bgn);
#endif
                break;

            case EX_SUB_CMD_PL_STR:
#ifdef USE_GBK
                str_ptr = IDX_TO_C_PTR(idx-idx_bgn);
#elif defined (USE_UNICODE)
                str_ptr = polish_uncompress_text(idx-idx_bgn);
#endif
                break;

            case EX_SUB_CMD_HG_STR:
#ifdef USE_GBK
                str_ptr = IDX_TO_C_PTR(idx-idx_bgn);
#elif defined (USE_UNICODE)
                str_ptr = hungarian_uncompress_text(idx-idx_bgn);
#endif
                break;

            case EX_SUB_CMD_GE_STR:
#ifdef USE_GBK
                str_ptr = IDX_TO_C_PTR(idx-idx_bgn);
#elif defined (USE_UNICODE)
                str_ptr = german_uncompress_text(idx-idx_bgn);
#endif
                break;
        }

#ifdef USE_GBK
        unic_len = EXSTR_ChcodeStr(buf, max_len, str_ptr);
#elif defined (USE_UNICODE)
        unic_len = EXSTR_DecodeStr(buf, max_len, str_ptr);
#endif
        unit_len = 2+1+unic_len;
        if (tx_idx+unit_len >= max_len || unit_len >= 255)
        {
            break;
        }

        dest[tx_idx++] = HIBYTE(idx);
        dest[tx_idx++] = LOBYTE(idx);
        dest[tx_idx++] = LOBYTE(unic_len);
        MEMCPY(&dest[tx_idx], buf, unic_len);
        tx_idx += unic_len;

        idx++;
    }

    //4 ����ռ�õĳ���
    return tx_idx;
}


/*****************************************************************************
��  �� : EXSTR_COM_SlvReadStr()
��  �� : ��ȡ�ַ����ظ�Э��
01  44      [00 00] [01 02  03  04] [nn ]  [�û����ݿ�       ] [5A 8B]
|   |       |       |               |      |                   |   |
|   |       |       |               |      |                   |   CRC��
|   ������  |       |               |      |                   CRC��
�豸��ַ    �ļ���  ��ַ            |      |
                    ��              �ֽ���
                    TxBuf



��  �� : U32 file_addr  :
         U08 block_size :
         U08 *TxBuf     :
         U08 *pTxTotal  :
��  �� : ��
��  �� : ��
*****************************************************************************/
U08 EXSTR_COM_SlvReadStr(U32 file_addr, U08 block_size, U08 *TxBuf, U16 *pTxTotal)
{
    U08 tx_idx = 0;
    U08 sub_fun;
    U08* pTx = &TxBuf[5];

    // file_addr ��16λ:�ӹ�����,��16λ:�ַ������
    sub_fun = DWORDMH(file_addr);

    //4 ����û����ݿ�
    {
        pTx[tx_idx++] = 0;  // ���ݿ��ڲ��ֽ���
        pTx[tx_idx++] = sub_fun;  // �ӹ�����

        switch(sub_fun)
        {
            case EX_SUB_CMD_INFO:
                MEMCLR(&pTx[tx_idx], 32);
                STRCPY(&pTx[tx_idx], APPSTRLIB);
                tx_idx += 32;
                pTx[tx_idx++] = HIBYTE(STR_EX_CH_BGN);
                pTx[tx_idx++] = LOBYTE(STR_EX_CH_BGN);
                pTx[tx_idx++] = HIBYTE(STR_EX_CH_END);
                pTx[tx_idx++] = LOBYTE(STR_EX_CH_END);
                pTx[tx_idx++] = HIBYTE(STR_EX_EN_BGN);
                pTx[tx_idx++] = LOBYTE(STR_EX_EN_BGN);
                pTx[tx_idx++] = HIBYTE(STR_EX_EN_END);
                pTx[tx_idx++] = LOBYTE(STR_EX_EN_END);
                break;
            default:
            case EX_SUB_CMD_CN_STR:
            case EX_SUB_CMD_EN_STR:
            case EX_SUB_CMD_ES_STR:
            case EX_SUB_CMD_GR_STR:
            case EX_SUB_CMD_FR_STR:
            case EX_SUB_CMD_IT_STR:
            case EX_SUB_CMD_PL_STR:
            case EX_SUB_CMD_HG_STR:
            case EX_SUB_CMD_GE_STR:
                {
                    U16 idx = LOWORD(file_addr);  /* ��ʼ��ȡ��� */

                    tx_idx += EXSTR_COM_ReadStr(&pTx[tx_idx], block_size-2, sub_fun, idx);
                }
                break;
        }

        pTx[0] = tx_idx; // �û����ݿ���Ϣ�е��ֽ���
    }

    tx_idx = block_size;

    //4 addr + �ֽ���
    TxBuf[0] = DWORDHI(file_addr);
    TxBuf[1] = DWORDMH(file_addr);
    TxBuf[2] = DWORDML(file_addr);
    TxBuf[3] = DWORDLO(file_addr);
    TxBuf[4] = tx_idx;              // Э���ֽ���

    *pTxTotal = tx_idx + 5; /* ��file_addr�ͳ��� 5�ֽ� */

    return TRUE;
}
#define EX_SYNC_UNKNOW      0
#define EX_SYNC_FULL        1
#define EX_SYNC_ALLEX       2
#define EX_SYNC_CMPLT       3
#define EX_SYNC_NONE        0xFF

#define EX_PRTC_ALL_IN_BK   0
#define EX_PRTC_EX_IN_BK    1


