/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: Tuya_B.h
 �ļ�����: Ϳѻ���߹���(��岿��)
 ������  : �ֺƱ�
 ��������: 2020.12.13
*******************************************************************************/

#ifndef __TUYA_B_H__
#define __TUYA_B_H__

#ifdef TUYA_B_GLOBAL
    #define TUYA_B_EXT
    #define TUYA_B_SLP_EXT
#else
    #define TUYA_B_EXT extern
    #define TUYA_B_SLP_EXT extern
#endif

/* �����Դ��� */
#ifndef far
    #define far
#endif

/* ��ƷID���� */
#define TUYA_PID_LEN                    (16)   

#ifndef TUYA_F_H                        /* �˲���Ĭ��ǰ���Ѷ��� */
/* ��Ʒ��Ϣ���� */
typedef struct tag_TUYA_INFO
{
    U08 dpid_num;                       /* ��Ҫ�ϱ��Ĺ��ܵ���� */
    U08 product_id[TUYA_PID_LEN];       /* ��ƷID */
    U08 bak;                            /* ���ã�ʹ�ṹ���ֽ���Ϊż�� */
}TUYA_INFO;
#define TUYA_INFO_SIZE                  (sizeof(TUYA_INFO)+1) / sizeof(U16)

/* ���ܵ����� */
typedef struct tag_TUYA_DATA
{
    U08 dpid;                           /* DPID */
    U08 dp_type;                        /* ���ܵ����� */
    U08 data_buf[244];                  /* �������� */
}TUYA_DATA;
#define TUYA_DATA_SIZE                  (sizeof(TUYA_DATA)+1) / sizeof(U16)

/* "����ָ��"���ֳ���(�ֽ���) */
#define RAW_DATA_BYTES                  (244)  /* ͸���� */
#define BOOL_DATA_BYTES                 (1)    /* ������ */
#define NUMBER_DATA_BYTES               (4)    /* ��ֵ�� */
#define STRING_DATA_BYTES               (244)  /* �ַ��� */
#define ENUM_DATA_BYTES                 (1)    /* ö���� */
#define FAULT_DATA_BYTES                (1)    /* ������ */
#endif

/* ���ܵ����� */
typedef struct tag_TUYA_DP_PROPS
{
    U08 dpid;                           /* DPID */
    U08 dp_type;                        /* ���ܵ����� */
    U08 data_type;                      /* ���ݴ������� */
    U08 func_type;                      /* �������� */
    U08 freq;                           /* �ϱ�Ƶ�� */
} TUYA_DP_PROPS;

/* ���ܵ����� */
#define DP_RAW                          (0)    /* ͸���� */
#define DP_BOOL                         (1)    /* ������ */
#define DP_NUMBER                       (2)    /* ��ֵ�� */
#define DP_STRING                       (3)    /* �ַ��� */
#define DP_ENUM                         (4)    /* ö���� */
#define DP_FAULT                        (5)    /* ������ */

/* ���ݴ������� */
#define DATA_RO                         (0)    /* ֻ�ϱ� */
#define DATA_WO                         (1)    /* ֻ�·� */
#define DATA_RW                         (2)    /* ���ϱ����·� */

/* �������� */
#define FUNC_CUSTOM                     (0)    /* �Զ��幦�� */
#define FUNC_STANDARD                   (1)    /* ��׼���� */
#define FUNC_CHANNEL                    (2)    /* ͨ�����书�� */

/* �ϱ�Ƶ������(Խ��Խ��) */
#define FREQ_VERY_LOW                   (0)    /* ���� */
#define FREQ_LOW                        (1)    /* �� */
#define FREQ_NORMAL                     (2)    /* ���� */
#define FREQ_FAST                       (3)    /* �� */
#define FREQ_VERY_FAST                  (4)    /* �ܿ� */
#define FREQ_INFINITE                   (5)    /* ���� */
#define FREQ_TYPE_MAX                   (6)    /* ����ϱ�Ƶ�������� */

/* ���ܵ��ϱ����� */
#define DP_INTV_1_HOUR                  (3600) /* 1h */
#define DP_INTV_30_MIN                  (1800) /* 30min */
#define DP_INTV_1_MIN                   (60)   /* 1min */
#define DP_INTV_10_SEC                  (10)   /* 10s */
#define DP_INTV_3_SEC                   (3)    /* 3s */

/* ��Ч���ܵ����� */
#define DPID_IDX_NULL                   (0xFF)                              

/* �ַ��������� */
#define P_CODE                          (0)    /* ���� */
#define GBK_CODE                        (1)    /* GBK2312�� */
#define UNI_CODE                        (2)    /* UNICODE�� */

/* ģ������ */
#define MODULE_WIFI                     (0)    /* WIFIģ�� */
#define MODULE_4G                       (1)    /* 4Gģ�� */

/* ҵ���߼����� */
#define TUYA_B_SLP_NT_HLXXX             (0)    /* ůͨHLϵ�г���ҵ�� */
#define TUYA_B_SLP_NT_HLXXX_MULTI       (1)    /* ůͨHLϵ�ж�����ҵ�� */

/* ================================================================================ */
/* �� �� �� �� Э �� �� �� �� ��                                                 */
/* ================================================================================ */
/* �ֶ����� */
#define FIELD_NULL                      (0x00) /* ��Ч */
#define FIELD_BOOL                      (0x01) /* ������ */
#define FIELD_NUMBER                    (0x02) /* ��ֵ�� */
#define FIELD_ENUM                      (0x03) /* ö���� */
#define FIELD_RO_NUM                    (0x04) /* ֻ����ֵ�� */
#define FIELD_RO_MSG                    (0x05) /* ֻ���ַ����� */
#define FIELD_ONE_MSG                   (0x06) /* ���ַ����� */
#define FIELD_ONE_NUM                   (0x07) /* ����ֵ�� */

/* �ֶ�ʹ�� */
#define FIELD_DISABLED                  (0x00) /* ����(�ֶ��ϴ�����Ч) */
#define FIELD_ENABLE                    (0x01) /* ʹ�� */

/* �ֶ����� */
#define FIELD_READONLY                  (0)    /* ֻ��(ֻ�ϱ�) */
#define FIELD_WRITABLE                  (1)    /* ��д(���ϱ����·�) */

/* ================================================================================ */
/* �� �� �� �� �� �� ҵ �� �� �� �� �� ʹ �� (Tuya_B_SLP_XXXXX.c)                   */
/* ================================================================================ */
TUYA_B_EXT BOOL fg_dpid_upload_init;        /* �ƶ˴洢��Э��汾��ձ�־ */
TUYA_B_EXT BOOL fg_dpid_upload_all;         /* ȫ���ϱ���־ */
TUYA_B_EXT void TYB_create_bool_field(U08 *buf, U08 *acc_len, U08 func_attr, U08 func_id, U08 field_type, U16 title, BOOL value);
TUYA_B_EXT void TYB_create_number_field(U08 *buf, U08 *acc_len, U08 func_attr, U08 func_id, U08 field_type, U16 title, U16 attr, I16 value, I16 max, I16 min);
TUYA_B_EXT void TYB_create_enum_field(U08 *buf, U08 *acc_len, U08 func_attr, U08 func_id, U08 field_type, U16 title, U16 *range, U08 range_len, I16 value);
TUYA_B_EXT void TYB_create_ro_num_field(U08 *buf, U08 *acc_len, U08 func_attr, U08 func_id, U08 field_type, U16 title, U16 attr, I16 value);
TUYA_B_EXT void TYB_create_ro_msg_field(U08 *buf, U08 *acc_len, U08 func_attr, U08 func_id, U08 field_type, U16 title, U16 msg);
TUYA_B_EXT void TYB_create_one_msg_field(far U08 *buf, far U08 *acc_len, far U08* content, U08 content_size);

/* ================================================================================ */
/* �� �� �� ��                                                                      */
/* ================================================================================ */
TUYA_B_EXT void TYB_init(void);                                     /* Ϳѻ���ܳ�ʼ�� */
TUYA_B_EXT void TYB_timer(void);                                    /* Ϳѻ���ܼ�ʱ */
TUYA_B_EXT BOOL TYB_upload_product_info(U08 *dest_buf);             /* ��Ʒ��Ϣ�ϱ� */
TUYA_B_EXT BOOL TYB_fill_dp(TUYA_DATA* tuya_data, U08 dpid_idx);    /* ������书�ܵ� */
TUYA_B_EXT BOOL TYB_parse_tuya_data(U08 *src_buf);                  /* ���ܵ���� */
TUYA_B_EXT U08 TYB_get_dpid_idx(U08 dpid);                          /* ��ȡ���ܵ��ڱ��е����� */
TUYA_B_EXT BOOL TYB_is_dpid_idx_valid(U08 idx);                     /* ��Ч���ܵ��ж� */

#define TYB_SET_FG_DPID_UPDATE_ALL    (fg_dpid_upload_all = TRUE)   /* ���ù��ܵ�ȫ���ϴ� */

/* ����ͷ�ļ� */
#include "Tuya_B_Config.h"

/* ҵ��ͷ�ļ� */
#if (TUYA_B_SLP_TYPE == TUYA_B_SLP_NT_HLXXX)                /* ůͨHLϵ�г���SLP */
    #include "SLP_NT/Tuya_B_SLP_HLxxx.h"
#elif (TUYA_B_SLP_TYPE == TUYA_B_SLP_NT_HLXXX_MULTI)		/* ůͨHLϵ�ж�����SLP */
    #include "SLP_NT/Tuya_B_SLP_HLxxx_MULTI.h"
#endif

#endif

