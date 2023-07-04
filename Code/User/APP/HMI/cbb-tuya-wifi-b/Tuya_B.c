/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: Tuya_B.c
 �ļ�����: Ϳѻ���߹���(��岿��)
 ������  : �ֺƱ�
 ��������: 2020.12.13
*******************************************************************************/
#define TUYA_B_GLOBAL
#include "includes.h"

#if (TUYA_B_SLP_TYPE == TUYA_B_SLP_NT_HLXXX)                /* ůͨHLϵ��SLP */
    #include "SLP_NT/Tuya_B_SLP_HLxxx.c"
#elif (TUYA_B_SLP_TYPE == TUYA_B_SLP_NT_HLXXX_MULTI)		/* ůͨHLϵ�ж�����SLP */
    #include "SLP_NT/Tuya_B_SLP_HLxxx_MULTI.c"
#endif


/*******************************************************************************
 �� ��: TYB_init()
 �� ��: Ϳѻģ��ı�����ʼ��
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void TYB_init(void)
{
/* ��"��ʼ������" */
#ifdef TUYA_USE_CONFIG_INIT
    TYBC_config_init();
#else
    ;
#endif
    fg_dpid_upload_init = FALSE;
}


static U16 tyb_upload_timer;                                                 /* ��ʱ�ϱ���ʱ */
/*******************************************************************************
 �� ��: TYB_interval_upload_timer()
 �� ��: ���ܵ������ϱ���ʱ����
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TYB_interval_upload_timer(void)
{
    U08 dpid_idx;

    for (dpid_idx = 0; dpid_idx < DPID_USED; dpid_idx++)
    {
        if (DP[dpid_idx].data_type== DATA_WO        /* ֻд���ܵ������ϱ� */
        || DP[dpid_idx].func_type == FUNC_CHANNEL)  /* ͨ�����书�ܵ������ϱ� */
        {
            continue;
        }
        else
        {
            U16 upload_intv = 0;

#if (TYBC_MODULE_TYPE == MODULE_WIFI)
            switch (DP[dpid_idx].freq)
            {
                case FREQ_VERY_LOW:  upload_intv = DP_INTV_1_HOUR;  break;
                case FREQ_LOW:       upload_intv = DP_INTV_30_MIN;  break;
                case FREQ_NORMAL:    upload_intv = DP_INTV_1_MIN;   break;
                case FREQ_FAST:      upload_intv = DP_INTV_10_SEC;  break;
                case FREQ_VERY_FAST: upload_intv = DP_INTV_3_SEC;   break;
                case FREQ_INFINITE:  upload_intv = DP_INTV_1_MIN;   break;  /* ʵ�������ã�Ĭ��1���� */
                default:                                            break;
            }
#else
            switch (DP[dpid_idx].freq)
            {
                case FREQ_VERY_LOW:  upload_intv = DP_INTV_1_HOUR;  break;
                case FREQ_LOW:       upload_intv = DP_INTV_30_MIN;  break;
                case FREQ_NORMAL:    upload_intv = DP_INTV_1_MIN;   break;
                case FREQ_FAST:      upload_intv = DP_INTV_10_SEC;  break;
                case FREQ_VERY_FAST: upload_intv = DP_INTV_3_SEC;   break;
                case FREQ_INFINITE:  upload_intv = DP_INTV_1_MIN;   break;  /* ʵ�������ã�Ĭ��1���� */
                default:                                            break;
            }
#endif

            if (upload_intv != 0)
            {
                if (tyb_upload_timer % upload_intv == 0)
                {
                    SETBIT_N(fg_dpid_upload_pri_low, dpid_idx);
                }
            }
        }
    }
}


/*******************************************************************************
 �� ��: TYB_timer()
 �� ��: 1sʱ�䵽��������
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void TYB_timer(void)
{
    if (TYBC_SEC_FLAG)
    {

        TYB_interval_upload_timer();

        if (++tyb_upload_timer >= 36000)
        {
            tyb_upload_timer = 0;
        }
    }

/* ��"��ʱ����" */
#ifdef TUYA_USE_CONFIG_TIMER
    TYBC_config_timer();
#endif
}


/*******************************************************************************
 �� ��: TYB_upload_product_info()
 �� ��: �����ϱ����ܵ�����&PID
 �� ��: dest_buf--�ϱ�������;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL TYB_upload_product_info(U08 *dest_buf)
{
    U08 buf_len = 0;

    /* ��ֵ */
    dest_buf[buf_len++] = DPID_USED;

    MEMCPY(&dest_buf[buf_len], (far U08 *)TYBC_PID, TUYA_PID_LEN);

    return TRUE;
}

#ifdef TUYA_HAS_TRANS
/*******************************************************************************
 �� ��: TYB_get_dpid_idx()
 �� ��: ���ݹ��ܵ�ID���Ҷ�Ӧ�Ĺ��ܵ�ID����λ��
 �� ��: dpid--���ܵ�ID;
 �� ��: ���ܵ�ID����ֵ
 �� ��: ��
*******************************************************************************/
U08 TYB_get_dpid_idx(U08 dpid)
{
    U08 i;
    U08 dpid_idx = 0;
    for (i = 0; i < DPID_USED; i++)
    {
        if (dpid == DP[i].dpid)
        {
            dpid_idx = i;
            break;
        }
    }
    return dpid_idx;
}
#endif


#ifdef TUYA_USE_HMI_SLAVE
/* ֻ֧�ִӻ��� */
/*******************************************************************************
 �� ��: TYB_is_dpid_idx_valid()
 �� ��: �Ƿ�Ϊ��Ч���ܵ�
 �� ��: idx--���ܵ�ID;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL TYB_is_dpid_idx_valid(U08 idx)
{
    return ((idx) != DPID_IDX_NULL);
}
#endif

/*******************************************************************************
 �� ��: TYB_fill_dp()
 �� ��: ����ϱ�����
 �� ��: tuya_data--Ϳѻ����;
        dpid_idx--���ܵ�ID������;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL TYB_fill_dp(TUYA_DATA* tuya_data, U08 dpid_idx)
{
    /* ��� */
    PTRCLR(tuya_data);

    if (dpid_idx == DPID_IDX_NULL || dpid_idx >= DPID_USED)
    {
        tuya_data->dpid = DPID_NULL;
    }
    else
    {
        TYB_SLP_fill_upload_data(tuya_data, dpid_idx);
    }

    return TRUE;
}


/*******************************************************************************
 �� ��: TYB_parse_tuya_data()
 �� ��: ��ȡ���ջ��������������·����ܵ������Ӧ�ı������ݣ�����ǿ�����Ӧ
 �� ��: src_buf--���ջ�����(�ƶ��·�������);
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL TYB_parse_tuya_data(U08 *src_buf)
{
    U08 i;
    TUYA_DATA tuya_data;
    U08 *dest_buf = NULL;

    /* ��ȡ���ջ����� */
    dest_buf = (U08*)&tuya_data;
    for (i = 0; i < sizeof(TUYA_DATA); i++)
    {
        dest_buf[i] = src_buf[i];
    }

    /* �������ܵ㲢���������� */
    TYB_SLP_parse_dp_data(&tuya_data);

    return TRUE;
}

