/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: SmartHmi.c
 �ļ�����: ������4.3�紥������ͨ��
 ������  : lhb
 ��������: 2021.05.08
*******************************************************************************/
#define SMARTHMI_GLOBAL
#include "includes.h"
#include "SmartHmi_commu_info.h"
#include "SmartHmi_config.c"


/* �������һ�ζ�ȡ���ֽ���(����ʵ��ͨѶ�������) */
#define MAX_TRAS_BYTES      (MODBUS_BUF_SIZE-4)

/* MODBUS�쳣��(�������) */
#define SH_MB_NO_ERR                0       /* MODBUS���쳣 */
#define SH_MB_ILLEGAL_FUNCTION      1       /* �Ƿ����� */
#define SH_MB_ILLEGAL_DATA_ADDR     2       /* �Ƿ����ݵ�ַ */
#define SH_MB_ILLEGAL_DATA_VALUE    3       /* �Ƿ�����ֵ */
#define SH_MB_SLAVE_DEVICE_FALUE    4       /* �ӻ��豸���ϣ��ӻ��޷����� */

//SH_CHK_TEST test_data;

static BOOL fg_pass_big_endian = TRUE;     /* HL051A/HL055Aϵ��Ĭ��ֵΪFALSE��HL040AĬ��ֵΪTRUE */

/*****************************************************************************
��  �� : SH_sign_local2sh()
��  �� : ����sign����ת��ΪSH sign����
��  �� : U16 sign_local :
��  �� : ��
��  �� : ��
*****************************************************************************/
U16 SH_sign_local2sh(U16 sign_local)
{
    U16 sign_sh = 0;        /* ת�������� */
    U16 unit_local = 0;     /* ԭʼ��λ */
    U16 unit_sh = 0;        /* ת����λ */
    U16 decimal_local = 0;  /* ԭʼС��λ */
    U16 decimal_sh = 0;     /* ת����С��λ */
    U16 fg_is_hide = 0;     /* �������� */
    U16 fg_is_const = 0;    /* �������ɸ��� */
    U16 fg_is_bit = 0;      /* ����Ϊλ���� */
    U16 auth_local = 0;     /* ԭʼȨ�� */
    U16 auth_sh = 0;        /* ת����Ȩ�� */

    /* ��λ */
    unit_local = sign_local & UINT_MASK;
    switch (unit_local)
    {
        case UINT_0:
            unit_sh = SH_UNIT_NULL;         /* �� */
            break;
        case UINT_C:
            unit_sh = (TEMP_IS_FARA) ? (SH_UNIT_TMP_F) : (SH_UNIT_TMP_C);        /* �¶�(��) */
            break;
        case UINT_A:
            unit_sh = SH_UNIT_A;            /* ����(A) */
            break;
        case UINT_PR:
            unit_sh = SH_UNIT_PRS_B;        /* ѹ��(bar) */
            break;
        case UINT_PE:
            unit_sh = SH_UNIT_PE;           /* �ٷֱ�(%) */
            break;
        case UINT_F:
            unit_sh = SH_UNIT_HZ;           /* Ƶ��(HZ) */
            break;
        case UINT_RS:
            unit_sh = SH_UNIT_RPS;          /* ת��(rps) */
            break;
        case UINT_RM:
            unit_sh = SH_UNIT_RPM;          /* ת��(rpm) */
            break;
        case UINT_S:
            unit_sh = SH_UNIT_SECOND;       /* �� */
            break;
        case UINT_M:
            unit_sh = SH_UNIT_MINT;         /* �� */
            break;
        case UINT_H:
            unit_sh = SH_UNIT_HOUR;         /* ʱ */
            break;
        case UINT_D:
            unit_sh = SH_UNIT_DAY;          /* �� */
            break;
        case UINT_ST:
            unit_sh = SH_UNIT_ST;           /* �� */
            break;
        case UINT_V:
            unit_sh = SH_UNIT_V;            /* V */
            break;
        case UINT_LH:
            unit_sh = SH_UNIT_LH;            /* L/H */
            break;
        default:
            unit_sh = SH_UNIT_NULL;         /* �� */
            break;
    }

    /* С��λ */
    decimal_local= sign_local & D_DOTMASK;
    switch (decimal_local)
    {
        case D_DOT1:
            decimal_sh = 1;
            break;
        case D_DOT2:
            decimal_sh = 2;
            break;
        case D_DOT3:
            decimal_sh = 3;
            break;
        default:
            decimal_sh = 0;
            break;
    }

    /* Ȩ�� */
    auth_local = sign_local & D_SAFEMASK;
    switch (auth_local)
    {
        case D_PRO:
            auth_sh = SH_PARA_PRO;
            break;
        case D_SER:
            auth_sh = SH_PARA_SER;
            break;
        case D_FAC:
            auth_sh = SH_PARA_FAC;
            break;
        case D_USER:
        default:
            auth_sh = SH_PARA_USR;
            break;
    }
    /* �Ƿ����� */
    if (sign_local & (D_HIDE))
    {
        fg_is_hide = TRUE;
    }
    else
    {
        fg_is_hide = FALSE;
    }

    /* �Ƿ񲻿ɸ� */
    if (sign_local & (D_NEXCH))
    {
        fg_is_const = TRUE;
    }
    else
    {
        fg_is_const = FALSE;
    }

    /* �Ƿ�Ϊλ���� */
    if (sign_local & (D_BIT))
    {
        fg_is_bit = TRUE;
    }
    else
    {
        fg_is_bit = FALSE;
    }

    /* �ϳ��������� */
    sign_sh = (unit_sh << 0)
            | (decimal_sh << DECIMAL_BGN_BIT)
            | (fg_is_hide << HIDE_BGN_BIT)
            | (fg_is_const << CONST_BGN_BIT)
            | (auth_sh << SAFE_BGN_BIT)
            | (fg_is_bit << BIT_BGN_BIT);

    return sign_sh;
}

/*******************************************************************************
 �� ��: SH_get_disp_info()
 �� ��: ��ȡ��ʾ������Ϣ
 �� ��: dest_buf--���ݷ��ػ���;
        offset--��ʼλ��;
        num--���ݸ���(��);
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_get_disp_info(U08 *dest_buf, U16 offset, U16 num)
{
    U08 i;
    I16 disp_info[DISP_INFO_MAX];           /* ��ʾ��Ϣ */
    U08 idx = 0;

    /* �������ݼ�� */
    if (num*sizeof(U16) > MAX_TRAS_BYTES || offset+num > SH_MAX_DISP_INFO)  /* ͨѶ���泬�� */
    {
        return FALSE;
    }

    /* ��ʼ�� */
    for (i=0; i<DISP_INFO_MAX; i++)
    {
        disp_info[i] = 0;
    }

    /* *************************��������************************* */

    /* 0 ģ���� */
    disp_info[DISP_PCB_NUM] = SHC_DISP_PCB_NUM;

    /* 1 ѹ���� */
    disp_info[DISP_CMP_NUM] = SHC_DISP_COMP_NUM;

    /* 2 ����״̬ */
    disp_info[DISP_MAC_STA] = SHC_DISP_GET_MAC_STATUS;

    /* 3 ��ǰ�¶� */
    disp_info[DISP_TMP_CURT] = SHC_DISP_GET_CTRL_TMP;

    /* 4 �趨�¶� */
    disp_info[DISP_TMP_SET] = SHC_get_set_tmp_A();

    /* 5 �¶ȵ�λ */
    disp_info[DISP_SET_UNIT] = SHC_DISP_SET_UNIT;

    /* 6 ����ģʽ(λ����) */
    disp_info[DISP_MODE_VALID] = SHC_get_valid_mode();

    /* 7 �趨ģʽ */
    disp_info[DISP_MODE_SET] = SHC_get_set_mode();

    /* 8 ����ģʽ */
    disp_info[DISP_MODE_RUN] = SHC_get_run_mode();

    /* 9 ��ʱʹ�� */        /* ���ô˹��� */
    disp_info[DISP_TIMING_USE] = 0;

    /* 10 ��ʱ����(0��ʾʹ��ǰ��Ķ�ʱ�߼�) */              /* �̶�ʹ��ǰ��Ķ�ʱ */
    disp_info[DISP_TIMING_GRP] = 0;

    /* 11 ��ʼ��״̬ */
    disp_info[DISP_INIT_STATE] = SHC_get_init_sta();

    /* 12 ʹ�����޵���(TRUE:����,FALSE:δ��) */
    disp_info[DISP_LMT_ON] = SHC_DISP_GET_LMT_ON;

    /* 13 ʹ�������趨���� */
    disp_info[DISP_LMT_SET_DAY] = SHC_DISP_GET_LMT_SET_DAY;

    /* 14 ʹ������������Сʱ�� */
    disp_info[DISP_LMT_RUN_HOUR] = SHC_DISP_GET_LMT_RUN_HOUR;

    /* 15 ʹ������ʣ��Сʱ�� */
    disp_info[DISP_LMT_LEFT_HOUR] = SHC_DISP_GET_LMT_LEFT_HOUR;

    /* 16 ��ǰ������ */
    disp_info[DISP_ERR_NUM] = SHC_get_err_num();

    /* 17 ��ʷ������ */
    disp_info[DISP_HIS_NUM] = SHC_DISP_GET_HIS_NUM;

    /* 18 ��ѯ���� ģ�������� */
    disp_info[DISP_CHK_AV_NUM] = 0;         /* ���� */

    /* 19 ��ѯ���� ���������� */
    disp_info[DISP_CHK_DV_NUM] = 0;         /* ���� */

    /* 20 ��ѯ���� ���������� */
    disp_info[DISP_CHK_EX_NUM] = 0;         /* ���� */

    /* 21 ���������� */
    disp_info[DISP_PARA_GRP_NUM] = SHC_DISP_GET_PARA_GRP_NUM;

    /* 22 �������� */
    disp_info[DISP_PASS_GRP_NUM] = SHC_DISP_GET_PASS_GRP_NUM;

    /* 23 �����ֶ���˪ģ���� */             /* ���� */
    disp_info[DISP_DEFRO_USE] = 0;

    /* 24 ���߹���(TRUE:ʹ��,FALSE:����) */
    disp_info[DISP_WIFI_USE] = SHC_DISP_GET_WIFI_USE;

    /* 25 Ԥ��ʣ��Сʱ�� */
    disp_info[DISP_PRE_HEAT_TIME_MINT] = SHC_DISP_GET_PREHEAT_MINT;

    /* 26 �趨�¶���Сֵ */
    /* 27 �趨�¶����ֵ */
    disp_info[DISP_TMP_SET_MIN] = SHC_get_set_tmp_min_A();
    disp_info[DISP_TMP_SET_MAX] = SHC_get_set_tmp_max_A();

    /* 28 �Ƿ���������������(TRUE:����,FALSE:����) */     /* ���� */
    disp_info[DISP_BOOT_DOWNLOAD] = FALSE;

    /* 29 ��ǰϵͳ��ˮ�¶� */
    disp_info[DISP_TMP_OUT] = SHC_DISP_GET_TMP_OUT;

    /* 30 ��ǰϵͳ��ˮ�¶� */
    disp_info[DISP_TMP_IN] = SHC_DISP_GET_TMP_IN;

    /* 31 ���ƶ��� */
    disp_info[DISP_CTRL_OBJ] = SHC_DISP_GET_CTRL_OBJ;

    /* 32 ϵͳģ��ģ���� */
    disp_info[DISP_CHK_SYS_AV_NUM] = SHC_get_sys_av_num();

    /* 33 ѹ��ģ�������� */
    disp_info[DISP_CHK_CMP_AV_STEP] = SHC_COMP_PV_STEP;

    /* 34 �������DO������� */
    disp_info[DISP_CHK_DO_STEP] = SHC_DO_STEP;

    /* 35 �������DI���벽�� */
    disp_info[DISP_CHK_DI_STEP] = SHC_DI_STEP;

    /* 36 ϵͳ������������� */
    disp_info[DISP_CHK_SYS_DO_NUM] = SHC_get_sys_do_num();

    /* 37 ϵͳ������ѹ���� */
    disp_info[DISP_SYS_COMP_RUN_NUM] = SHC_DISP_GET_CM_RUN_NUM;

    /* 38 �趨ģʽ����(��λ��ţ���λ���) */               /* ���� */
    disp_info[DISP_MODE_SET_INDEX] = 0;

    /* 39 ����ģʽ�Ƿ�ʹ�� */
    disp_info[DISP_TEST_EN] = SHC_DISP_GET_TEST_EN;

    /* 40~41 ���г�˪��״̬ */
    disp_info[DISP_FORCE_DFRST_INFO_1] = SHC_DISP_GET_MNL_DEFRO_1;
    disp_info[DISP_FORCE_DFRST_INFO_2] = SHC_DISP_GET_MNL_DEFRO_2;

    /* 42 �趨�¶�С���� */
    disp_info[DISP_TMP_SET_DOT] = SHC_get_set_tmp_dot();

    /* 43 �û���ѯ��DO��� */
    disp_info[DISP_CHK_USER_DO] = SHC_DISP_GET_CHK_USER_DO;

    /* 44 �������С */
    disp_info[DISP_PARA_GRP_SIZE] = SHC_DISP_GET_PARA_GRP_SIZE;

    /* 45 ʹ�������������ֵ */
    disp_info[DISP_LMT_SET_MAX] = SHC_DISP_GET_LMT_SET_MAX;

    /* 46 �Ƿ����ֶ���˪ */
    disp_info[DISP_HAVE_MNL_DEFRO] = SHC_DISP_HAVE_MNL_DEFRO;

    /* 47 ���й������Ƿ������޸�ģʽ */
    disp_info[DISP_RUN_ALLOW_SET_MODE] = SHC_DISP_RUN_ALLOW_SET_MODE;

    /* 48 �Ƿ��������������� ����HL051A/HL055Aʹ�� */
    disp_info[DISP_HAVE_START_CODE] = SHC_DISP_HAVE_START_CODE_MEIYI;

    /* 49 ������������ ����HL051A/HL055Aʹ�� */
    disp_info[DISP_START_CODE] = SHC_DISP_START_CODE_MEIYI;

    /* 50 ���������־ */
    disp_info[DISP_DISP_FG] = SHC_get_disp_fg();

    /* 51 ����������ģʽ */
    disp_info[DISP_ALARM_SOUND_SET] = SHC_get_sound_set();

    /* 52 ����ģʽ */
    disp_info[DISP_MODE_TYPE] = SHC_get_mode_type();

    /* 53 ��ǰ��ˮ�¶� */
    disp_info[DISP_TMP_HOT] = SHC_DISP_GET_TMP_HOT;

    /* 54 ʪ�ȿ���ģʽ */
    disp_info[DISP_HUMI_MODE_VALID] = SHC_DISP_GET_HUMI_MODE_VALID;

    /* 55 ʪ���趨ģʽ */
    disp_info[DISP_HUMI_MODE_SET] = SHC_DISP_GET_HUMI_MODE_SET;

    /* 56 ʪ������ģʽ */
    disp_info[DISP_HUMI_MODE_RUN] = SHC_DISP_GET_HUMI_MODE_RUN;

    /* 57 ��ǰʪ�� */
    disp_info[DISP_HUMI_CURT] = SHC_DISP_GET_HUMI_CURT;

    /* 58 �趨ʪ�� */
    disp_info[DISP_HUMI_SET] = SHC_DISP_GET_HUMI_SET;

    /* 59 ʪ���趨��Сֵ */
    disp_info[DISP_HUMI_SET_MIN] = SHC_DISP_GET_HUMI_SET_MIN;

    /* 60 ʪ���趨���ֵ */
    disp_info[DISP_HUMI_SET_MAX] = SHC_DISP_GET_HUMI_SET_MAX;

    /* 61 ���������־ */
    disp_info[DISP_DISP_FG_EX] = SHC_get_disp_fg_ex();

    /* 62 ���������־���Ƿ���Զ��� */
    disp_info[DISP_CAN_DO_FG] = SHC_get_disp_can_do_fg();

    /* 63 �ڶ����趨�¶� */
    disp_info[DISP_TMP_SET_EX] = SHC_get_set_tmp_B();

    /* 64 �ڶ�����ǰ�¶� */
    disp_info[DISP_TMP_CURT_EX] = SHC_get_ctrl_tmp_B();

    /* 65 �ڶ����趨�¶���Сֵ */
    disp_info[DISP_TMP_SET_EX_MIN] = SHC_get_set_tmp_min_B();

    /* 66 �ڶ����趨�¶����ֵ */
    disp_info[DISP_TMP_SET_EX_MAX] = SHC_get_set_tmp_max_B();

    /* 67 ��ˮ�趨�¶� */
    disp_info[DISP_TMP_HOT_SET] = SHC_get_hot_tmp_set();

    /* 68 ��ˮ�趨�¶���Сֵ */
    disp_info[DISP_TMP_HOT_SET_MIN] = SHC_get_hot_tmp_set_min();

    /* 69 ��ˮ�趨�¶����ֵ */
    disp_info[DISP_TMP_HOT_SET_MAX] = SHC_get_hot_tmp_set_max();

    /* 70 ϵͳά��������Ϣ */
    disp_info[DISP_LMT_TYPE_INFO] = SHC_get_lmt_type_info();

    /* 71 ϵͳά������ */
    disp_info[DISP_LMT_TYPE_VALUE] = SHC_get_lmt_type_value();

    /* 72 ��Ƶ���ϸ��� */
    disp_info[DISP_INVT_ERR_NUM] = SHC_get_invt_err_num();

    /* 73 ����ģʽ���� */
    disp_info[DISP_SILENT_LEVEL] = SHC_get_silent_level();

    /* 74 ����ģʽ���������� */
    disp_info[DISP_SILENT_LEVEL_ATTR] = SHC_get_silent_level_attr();

    /* 75 �·������¶ȼ�� */
    disp_info[DISP_ROOM_TEMP_WR_INTVL] = SHC_get_room_temp_wr_intvl();

    /* 76 ������� */
    disp_info[DISP_BKBD_LANGUAGE] = SHC_get_bkbd_language();

    /* 77 ����������Ϣ */
    disp_info[DISP_DISP_FG_EX2] = SHC_get_disp_fg_ex2();

    /* 80 ��ʾ��������1 */
    disp_info[DISP_LANGUAGE_FG1] = SHC_get_disp_language1();

    /* 81 ��ʾ��������2 */
    disp_info[DISP_LANGUAGE_FG2] = SHC_get_disp_language2();

    /* ********************************************************** */
    /* ��ֵ */
    for (i=0; i<num; i++)
    {
        U16 temp = 0;
        if (offset+i < DISP_INFO_MAX)
        {
            temp = disp_info[offset+i];
        }
        dest_buf[idx++] = HIBYTE(temp);
        dest_buf[idx++] = LOBYTE(temp);
    }
    return TRUE;
}

/*******************************************************************************
 �� ��: SH_get_version()
 �� ��: ��ȡ�汾��
 �� ��: dest_buf--���ݷ��ػ���;
        offset--��ʼλ��;
        num--�汾����;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_get_version(U08 *dest_buf, U16 offset, U16 num)
{
    U08 i;
    U16 pcb_id = 0;         /* ��� */
    SH_CHK_VER ver;         /* ��ʾ��Ϣ */
    U08 idx = 0;            /* ���ݷ������� */
    U08 *pVerCode = NULL;   /* ָ�������ַ���Դ */
    U08 data_size = 0;      /* ���ݴ�С */
    U16 *src_buf = NULL;    /* ָ��Դ���ݵ�ָ�� */

    /* �������ݼ�� */
    if (num*sizeof(SH_CHK_VER) > MAX_TRAS_BYTES || offset+num > SH_MAX_PCB_NUM) /* ͨѶ���泬�� */
    {
        return FALSE;
    }

    for (pcb_id=offset; pcb_id<offset+num; pcb_id++)
    {
        /* *************************��������************************* */

        /* ��������š��汾��Դ���汾�ų��� */
        ver.pcb_name = SHC_VER_PCB_NAME;
        ver.pcb_id = MAKEWORD(pcb_id, NO_ID);
        pVerCode = SHC_VER_CODE(pcb_id);
        data_size = SHC_VER_DATA_SIZE;

        /* ********************************************************** */

        /* ��ȡ�汾���ַ��� */
        for (i=0; i<MAX_VER_BYTE; i++)
        {
            if (i < data_size)
            {
                ver.fun_code[i] = pVerCode[i];
            }
            else
            {
                ver.fun_code[i] = 0;
            }
        }
        /* ������ֵ */
        dest_buf[idx++] = HIBYTE(ver.pcb_name);
        dest_buf[idx++] = LOBYTE(ver.pcb_name);
        dest_buf[idx++] = HIBYTE(ver.pcb_id);
        dest_buf[idx++] = LOBYTE(ver.pcb_id);
        src_buf = (U16*)&ver.fun_code;
        data_size = MAX_VER_BYTE / 2;
        for (i=0; i<data_size; i++)
        {
            dest_buf[idx++] = HIBYTE(src_buf[i]);
            dest_buf[idx++] = LOBYTE(src_buf[i]);
        }
    }

    return TRUE;
}

/*******************************************************************************
 �� ��: SH_get_vfd_version()
 �� ��:
 �� ��: dest_buf--;
        offset--;
        num--;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
BOOL SH_get_vfd_version(U08 *dest_buf, U16 offset, U16 num)
{
    U08 i;
    U16 pcb_id = 0;         /* ��� */
    SH_CHK_VER ver;         /* ��ʾ��Ϣ */
    U08 idx = 0;            /* ���ݷ������� */
    U08 data_size = 0;      /* ���ݴ�С */
    U16 *src_buf = NULL;    /* ָ��Դ���ݵ�ָ�� */

    /* �������ݼ�� */
    if (num*sizeof(SH_CHK_VER) > MAX_TRAS_BYTES || offset+num > SH_MAX_PCB_NUM) /* ͨѶ���泬�� */
    {
        return FALSE;
    }

    for (pcb_id=offset; pcb_id<offset+num; pcb_id++)
    {
        /* *************************��������************************* */
        SHC_get_vfd_ver(&ver, pcb_id);
        /* ********************************************************** */

        /* ������ֵ */
        dest_buf[idx++] = HIBYTE(ver.pcb_name);
        dest_buf[idx++] = LOBYTE(ver.pcb_name);
        dest_buf[idx++] = HIBYTE(ver.pcb_id);
        dest_buf[idx++] = LOBYTE(ver.pcb_id);
        src_buf = (U16*)&ver.fun_code;
        data_size = MAX_VER_BYTE / 2;
        for (i=0; i<data_size; i++)
        {
            dest_buf[idx++] = HIBYTE(src_buf[i]);
            dest_buf[idx++] = LOBYTE(src_buf[i]);
        }
    }

    return TRUE;
}

/*******************************************************************************
 �� ��: SH_get_odu_version()
 �� ��: ��ȡ�����汾��
 �� ��: dest_buf--���ݷ��ػ���;
        offset--��ʼλ��;
        num--�汾����;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_get_exio_version(U08 *dest_buf, U16 offset, U16 num)
{
    U08 i;
    U16 cell_id = 0;         /* ��� */
    SH_CHK_VER ver;         /* ��ʾ��Ϣ */
    U08 idx = 0;            /* ���ݷ������� */
    U08 *pVerCode = NULL;   /* ָ�������ַ���Դ */
    U08 data_size = 0;      /* ���ݴ�С */
    U16 *src_buf = NULL;    /* ָ��Դ���ݵ�ָ�� */

    /* �������ݼ�� */
    if (num*sizeof(SH_CHK_VER) > MAX_TRAS_BYTES || offset+num > SH_MAX_PCB_NUM*SH_MAX_EXIO_NUM) /* ͨѶ���泬�� */
    {
        return FALSE;
    }

    for (cell_id=offset; cell_id<offset+num; cell_id++)
    {
        U16 pcb_id = cell_id / SH_MAX_EXIO_NUM;
        U16 exio_id = cell_id % SH_MAX_EXIO_NUM;

        /* *************************��������************************* */

        /* ��������š��汾��Դ���汾�ų��� */
        ver.pcb_name = SHC_EXIO_VER_PCB_NAME;
        ver.pcb_id = MAKEWORD(pcb_id, exio_id);
        pVerCode = SHC_EXIO_VER_CODE(pcb_id, exio_id);
        data_size = SHC_EXIO_VER_DATA_SIZE;

        /* ********************************************************** */

        /* ��ȡ�汾���ַ��� */
        for (i=0; i<MAX_VER_BYTE; i++)
        {
            if (i < data_size)
            {
                ver.fun_code[i] = pVerCode[i];
            }
            else
            {
                ver.fun_code[i] = 0;
            }
        }
        /* ������ֵ */
        dest_buf[idx++] = HIBYTE(ver.pcb_name);
        dest_buf[idx++] = LOBYTE(ver.pcb_name);
        dest_buf[idx++] = HIBYTE(ver.pcb_id);
        dest_buf[idx++] = LOBYTE(ver.pcb_id);
        src_buf = (U16*)&ver.fun_code;
        data_size = MAX_VER_BYTE / 2;
        for (i=0; i<data_size; i++)
        {
            dest_buf[idx++] = HIBYTE(src_buf[i]);
            dest_buf[idx++] = LOBYTE(src_buf[i]);
        }
    }

    return TRUE;
}

/*******************************************************************************
 �� ��: SH_get_4G_info()
 �� ��:
 �� ��: dest_buf--;
		offset--;
		num--;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_get_4G_info(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i;
    SH_4G_INFO info;
    U16 idx = 0;            /* ���ݷ������� */
    U16 *src_buf = NULL;

    /* �������ݼ�� */
    if (offset+num > SH_4G_INFO_SIZE) /* ͨѶ���泬�� */
    {
        return FALSE;
    }

    VALCLR(info);
    // ��ֵ
    SHC_get_4G_info(&info);

    /* ������ֵ */
    src_buf = (U16 *)&info + offset;
    for(i=0; i<SH_4G_INFO_SIZE; i++)
    {
        dest_buf[idx++] = HIBYTE(src_buf[i]);
        dest_buf[idx++] = LOBYTE(src_buf[i]);
    }

    return TRUE;
}

/*******************************************************************************
 �� ��: SH_get_passwrd()
 �� ��: ��ȡ����
 �� ��: dest_buf--���ݷ��ػ���;
        offset--�������;
        num--��������;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_get_passwrd(U08 *dest_buf, U16 offset, U16 num)
{
    U08 i;
    SH_PASS passwrd;        /* ���� */
    U16 pass_grp = 0;       /* ������ */
    U08 idx = 0;            /* ���ݷ������� */
    U16 *passwrd_src = PNULL;               /* ָ�������ַ���Դ */
    const U16 pass_null[MAX_PASS_BIT/2] = {0xffff, 0xffff, 0xffff, 0xffff};
    U16 pass_temp[MAX_PASS_BIT/2] = {0xffff, 0xffff, 0xffff, 0xffff};

    /* �������ݼ�� */
    if (num*sizeof(SH_PASS) > MAX_TRAS_BYTES || offset+num > SH_MAX_PASS_GRP)   /* ͨѶ���泬�� */
    {
        return FALSE;
    }

    for (pass_grp=offset; pass_grp<offset+num; pass_grp++)
    {
        /* *************************��������************************* */
        /* Ȩ�ޡ����� */
        switch (pass_grp)
        {
            case PASS_LIMIT:
                passwrd.authority = SH_PWR_SALE;
                passwrd_src = (U16*)&password[PASS_LIMIT][0];
                break;
            case PASS_USER:
                passwrd.authority = SH_PWR_USER;
                passwrd_src = (U16*)&password[PASS_USER][0];
                break;
            case PASS_PRO:
                passwrd.authority = SH_PWR_PRO;
                passwrd_src = (U16*)&password[PASS_PRO][0];
                break;
            case PASS_SER:
                passwrd.authority = SH_PWR_SER;
                passwrd_src = (U16*)&password[PASS_SER][0];
                break;
            case PASS_FAC:
                passwrd.authority = SH_PWR_FAC;
                passwrd_src = (U16*)&password[PASS_FAC][0];
                break;
            case PASS_SUPER:
                passwrd.authority = SH_PWR_SPR;
                SHC_get_pass_super(pass_temp);
                passwrd_src = (U16*)&pass_temp[0];
                break;
            case PASS_POP:
                passwrd.authority = SH_PWR_START_MAC;
                SHC_get_pass_pop(pass_temp);
                passwrd_src = (U16*)&pass_temp[0];
                break;
            default:
                passwrd.authority = SH_PWR_NONE;
                passwrd_src = (U16*)pass_null;
                break;
        }

        /* ********************************************************** */

        /* ������ֵ */
        dest_buf[idx++] = HIBYTE(passwrd.authority);
        dest_buf[idx++] = LOBYTE(passwrd.authority);
        for (i=0; i<MAX_PASS_BIT/2; i++)
        {
            if (fg_pass_big_endian == TRUE)
            {
                dest_buf[idx++] = HIBYTE(passwrd_src[i]);
                dest_buf[idx++] = LOBYTE(passwrd_src[i]);
            }
            else
            {
                dest_buf[idx++] = LOBYTE(passwrd_src[i]);
                dest_buf[idx++] = HIBYTE(passwrd_src[i]);
            }
        }
    }

    return TRUE;
}

/*******************************************************************************
 �� ��: SH_get_holiday_info()
 �� ��: ��ȡ
 �� ��: dest_buf--���ݷ��ػ���;
        offset--�������;
        num--��������;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_get_holidayinfo(U08 *dest_buf, U16 data_num)
{
    SH_HOLIDAYINFO info;
    U16 *src_buf = PNULL;
    U16 idx = 0;
    U16 n = 0;

    /* �������ݼ�� */
    if (data_num != SH_HOLIDAYINFO_SIZE)   /* ��ȡ������һ�� */
    {
        return FALSE;
    }

    VALCLR(info);

    SHC_get_holiday_info(&info);

    //������ֵ
    src_buf = (U16*)&info;
    for (idx=0,n=0; n<data_num; n++)
    {
        dest_buf[idx++] = HIBYTE(src_buf[n]);
        dest_buf[idx++] = LOBYTE(src_buf[n]);
    }

    return TRUE;
}

/*******************************************************************************
 �� ��: SH_get_curt_error()
 �� ��: ��ȡ��ǰ����
 �� ��: dest_buf--���ݷ��ػ���;
        offset--��ʼλ��;
        num--������;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_get_curt_error(U08 *dest_buf, U16 offset, U16 num)
{
    U16 j = 0;              /* ��� */
    U16 sys_err_id;
    U16 mod_id;
    U16 mod_err_id;         /* �������� */
    U08 fg_err = FALSE;     /* �ҵ����� */
    SH_ERR_CURT err;        /* ������Ϣ */
    U16 idx = 0;            /* ���ݷ������� */
    U16 *src_buf = NULL;
    U16 err_idx = 0;        /* ��ǰ�����б�Ĺ��Ϻ� ��0��ʼ */
    U16 err_cnt = 0;        /* ��ȡ�Ĺ����� */

    /* �������ݼ�� */
    if (num*sizeof(SH_ERR_CURT) > MAX_TRAS_BYTES || offset+num > SH_MAX_ERR_CURT_NUM)   /* ͨѶ���泬�� */
    {
        return FALSE;
    }

    sys_err_id = mod_id = mod_err_id = 0;
    while (TRUE)
    {
        fg_err = FALSE;

        /* �������� */
        if (err_cnt >= num)
        {
            break;
        }

        fg_err = SHC_get_curt_err(&err, &sys_err_id, &mod_id, &mod_err_id);

        if (!fg_err)
        {
            err.name = 0;
            err.id = 0;
            err.err_id = 0;
        }

        /* ********************************************************** */
        if (err_idx >= offset || !fg_err)   /* û�鵽����˵���Ѿ�û�й���,ҲҪ���ͨ�Ż��� */
        {
            err_cnt++;
            /* ������ֵ */
            src_buf = (U16*)&err;
            for (j=0; j<SH_ERR_CRT_SIZE; j++)
            {
                dest_buf[idx++] = HIBYTE(src_buf[j]);
                dest_buf[idx++] = LOBYTE(src_buf[j]);
            }
        }
        if (fg_err)
        {
            err_idx++;
        }
    }
    return TRUE;
}

/*******************************************************************************
 �� ��: SH_get_his_error()
 �� ��: ��ȡ��ʷ����
 �� ��: dest_buf--���ݷ��ػ���;
        offset--��ʼλ��;
        num--������;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_get_his_error(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i, j;
    SH_ERR_HIS err;         /* ������Ϣ */
    U16 idx = 0;            /* ���ݷ������� */
    U16 *src_buf = NULL;

    /* �������ݼ�� */
    if (num*sizeof(SH_ERR_HIS) > MAX_TRAS_BYTES || offset+num > SH_MAX_ERR_HIS_NUM) /* ͨѶ���泬�� */
    {
        return FALSE;
    }

    for(i=0; i<num; i++)
    {
        SHC_get_his_err_info(&err, offset+i);

        /* ********************************************************** */

        /* ������ֵ */
        src_buf = (U16*)&err;
        for (j=0; j<SH_ERR_HIS_SIZE; j++)
        {
            dest_buf[idx++] = HIBYTE(src_buf[j]);
            dest_buf[idx++] = LOBYTE(src_buf[j]);
        }
    }

    return TRUE;
}

/*******************************************************************************
 �� ��: SH_get_fcuinfo()
 �� ��: ��ȡ������Ϣ
 �� ��: dest_buf--���ݷ��ػ���;
        offset--��ʼλ��;
        num--��ȡ���ݸ���;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_get_fcuinfo(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i;
    SH_FCU info;         /* ������Ϣ */
    U16 idx = 0;            /* ���ݷ������� */
    U16 *src_buf = NULL;

    /* �������ݼ�� */
    if (offset+num > SH_FCUINFO_SIZE) /* ͨѶ���泬�� */
    {
        return FALSE;
    }

    VALCLR(info);
    // ��ֵ
    SHC_get_fcu_info(&info);

    /* ������ֵ */
    src_buf = (U16 *)&info + offset;
    for(i=0; i<SH_FCUINFO_SIZE; i++)
    {
        dest_buf[idx++] = HIBYTE(src_buf[i]);
        dest_buf[idx++] = LOBYTE(src_buf[i]);
    }

    return TRUE;
}

/*******************************************************************************
 �� ��: SH_get_mode_set_info()
 �� ��:
 �� ��: dest_buf--;
        offset--;
        num--;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_get_mode_set_info(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i;
    SH_MODE_SET_INFO info;
    U16 idx = 0;            /* ���ݷ������� */
    U16 *src_buf = NULL;

    /* �������ݼ�� */
    if (offset+num > SH_MODE_SET_INFO_SIZE) /* ͨѶ���泬�� */
    {
        return FALSE;
    }

    VALCLR(info);
    // ��ֵ
    SHC_get_mode_set_info(&info);

    /* ������ֵ */
    src_buf = (U16 *)&info + offset;
    for(i=0; i<SH_MODE_SET_INFO_SIZE; i++)
    {
        dest_buf[idx++] = HIBYTE(src_buf[i]);
        dest_buf[idx++] = LOBYTE(src_buf[i]);
    }

    return TRUE;
}

/*******************************************************************************
 �� ��: SH_get_main_info()
 �� ��:
 �� ��: dest_buf--;
        offset--;
        num--;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_get_main_info(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i;
    U16 idx = 0;            /* ���ݷ������� */
    U16 *src_buf = NULL;

    /* �������ݼ�� */
    if (offset+num > SH_MAIN_INFO_SIZE) /* ͨѶ���泬�� */
    {
        return FALSE;
    }

    VALCLR(sh_main_info);
    // ��ֵ
    SHC_get_main_info(&sh_main_info);

    /* ������ֵ */
    src_buf = (U16 *)&sh_main_info + offset;
    for(i=0; i<SH_MAIN_INFO_SIZE; i++)
    {
        dest_buf[idx++] = HIBYTE(src_buf[i]);
        dest_buf[idx++] = LOBYTE(src_buf[i]);
    }

    return TRUE;
}

/*******************************************************************************
 �� ��: SH_get_invt_err()
 �� ��:
 �� ��: dest_buf--;
        offset--;
        num--;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_get_invt_err(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i, j;
    SH_CHK_INFO info;       /* ģ������Ϣ */
    U16 idx = 0;            /* ���ݷ������� */
    U16 *src_buf = NULL;    /* ָ��Դ���� */

    /* �������ݼ�� */
    if (num*sizeof(SH_CHK_INFO) > MAX_TRAS_BYTES/* || offset+num > SH_MAX_CHK_INFO*/)//ͨѶ���泬��
    {
        return FALSE;
    }

    for (i=offset; i<offset+num; i++)
    {
        /* *************************��������************************* */

        if (i < SHC_get_invt_err_num())
        {
            SHC_get_invt_err_info(&info, i);
        }

        /* ********************************************************** */

        /* ������ֵ */
        src_buf = (U16*)&info;
        for (j=0; j<SH_CHK_SYS_AV_SIZE; j++)
        {
            dest_buf[idx++] = HIBYTE(src_buf[j]);
            dest_buf[idx++] = LOBYTE(src_buf[j]);
        }
    }

    return TRUE;
}

/*******************************************************************************
 �� ��: SH_cal_grp_attrib()
 �� ��: ���������Ը�ʽת��ΪSH���Ը�ʽ
 �� ��: grp--������;
 �� ��: ����������
 �� ��: ������������:Ȩ��(ʹ�õ�λ������)���Ƿ�����
*******************************************************************************/
U16 SH_cal_grp_attrib(U08 grp)
{
    U16 sign_local = 0;     /* ԭʼ���� */
    U16 sign_sh = 0;        /* ת�������� */
    U16 authority_local = 0;/* ԭʼȨ�� */
    U16 authority_sh = 0;   /* ת����Ȩ�� */
    U16 fg_is_hide = 0;     /* �������� */

    /* ��ȡ���� */
    sign_local = SHC_get_para_grp_sign(grp);

    /* Ȩ�� */
    authority_local = sign_local & D_SAFEMASK;
    switch (authority_local)
    {
        case D_USER:
            authority_sh = SH_PWR_USER;
            break;
        case D_PRO:
            authority_sh = SH_PWR_PRO;
            break;
        case D_SER:
            authority_sh = SH_PWR_SER;
            break;
        case D_FAC:
            authority_sh = SH_PWR_FAC;
            break;
        default:
            authority_sh = SH_PWR_NONE;
            break;
    }

    /* �Ƿ����� */
    if (sign_local & (D_HIDE))
    {
        fg_is_hide = TRUE;
    }
    else
    {
        fg_is_hide = FALSE;
    }

    /* �ϳ��������� */
    sign_sh = (authority_sh << 1)
            | (fg_is_hide << 12);

    return sign_sh;
}

/*******************************************************************************
 �� ��: SH_get_para_grp_info()
 �� ��: ��ȡ��������Ϣ
 �� ��: dest_buf--���ݷ��ػ���;
        offset--��ʼλ��;
        num--���ݸ���(��);
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_get_para_grp_info(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i;
    SH_PARA_GRP para_grp_name[SH_MAX_PARA_GRP_NUM];         /* ��ʾ��Ϣ */
    U16 idx = 0;            /* ���ݷ������� */
    U16 data_words = 0;     /* �����ִ�С */
    U16 offset_words = 0;   /* ��ƫ�ƻ���ַ */
    U16 *src_buf = NULL;

    /* �������ݼ�� */
    if (num*sizeof(SH_PARA_GRP) > MAX_TRAS_BYTES || offset+num > SH_MAX_PARA_GRP_NUM)/* ͨѶ���泬�� */
    {
        return FALSE;
    }

    /* *************************��������************************* */

    for (i=0; i<SHC_DISP_GET_PARA_GRP_NUM; i++)
    {
        para_grp_name[i].name = SHC_NAME_PARA_GRP(i);
        para_grp_name[i].item_num = (U16)SHC_get_para_grp_item(i);
        para_grp_name[i].attrib = SH_cal_grp_attrib((U08)i);
    }

    /* ********************************************************** */

    /* ��ֵ */
    src_buf = (U16*)&para_grp_name[0].name;
    data_words = num * SH_PARA_GRP_SIZE;
    offset_words = offset * SH_PARA_GRP_SIZE;
    for (i=0; i<data_words; i++)
    {
        if (offset_words+i < SH_MAX_PARA_GRP_NUM*SH_PARA_GRP_SIZE)
        {
            dest_buf[idx++] = HIBYTE(src_buf[offset_words+i]);
            dest_buf[idx++] = LOBYTE(src_buf[offset_words+i]);
        }
        else
        {
            dest_buf[idx++] = 0;
            dest_buf[idx++] = 0;
        }
    }
    return TRUE;
}

/*****************************************************************************
��  �� : SH_attr_local2sh()
��  �� : ��������ת��ΪSH����,�����ֵ���Ϣ
��  �� : U16 sign_local     : ��������
         const U16 *dic_str : �ֵ�
��  �� : ��
��  �� : ��
*****************************************************************************/
U16 SH_attr_local2sh(U16 sign_local, const U16 *dic_str)
{
    U16 sign_sh = 0;
    U16 dict_sh = 0;        /* ת����λ */
    U16 dict_idx = 0;       /* ����ֵ�ֵ���� */

    sign_sh |= SH_sign_local2sh(sign_local);

    if (dic_str != NULL)    /* �ַ�����:�����ֵ� */
    {
        sign_sh &= ~(SH_UNIT|SH_DECIMAL);
        dict_idx = SHC_get_dict_idx(dic_str);/* �ֵ���� */
        dict_sh = dict_idx + UNIT_OFFSET;   /* ����ƫ�� */
        sign_sh |= dict_sh;
    }

    return sign_sh;
}

/*******************************************************************************
 �� ��: SH_cal_para_attrib()
 �� ��: ���������Ը�ʽת��ΪSH���Ը�ʽ
 �� ��: grp--������;
        item--������;
 �� ��: ����������
 �� ��: ������������:�Ƿ�Ϊ�ַ����͡���λ���Ƿ����ء��Ƿ�ɸ�
*******************************************************************************/
U16 SH_cal_para_attrib(U08 grp, U08 item)
{
    U16 sign_local = 0;     /* ԭʼ���� */
    U16 sign_sh = 0;        /* ת�������� */

    /* ��ȡԭʼ���� */
    sign_local = SHC_get_para_get_sign(grp, item);

    sign_sh |= SH_attr_local2sh(sign_local, SHC_get_para_small_pmsg(grp, item));

    return sign_sh;
}

/*******************************************************************************
 �� ��: SH_get_dict_attrib()
 �� ��:
 �� ��: dest_buf--;
        offset--;
        num--;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_get_dict_attrib(U08 *dest_buf, U16 offset, U16 num)
{
    U08 i;
    U08 idx = 0;            /* ���ݷ������� */
    U08 array_num = 0;      /* �ַ����� */
    U16 data_words = 0;     /* �������� */
    U16 offset_words = 0;   /* ��ƫ�ƻ���ַ */
    U16 src_buf[SH_MAX_PARA_STRING_GRP];
    U16 dic_idx;

    /* �������ݼ�� */
    if (num > MAX_TRAS_BYTES/2 || offset+num > SH_MAX_PARA_STRING_GRP)  /* ͨѶ���泬�� */
    {
        return FALSE;
    }

    /* *************************��������************************* */
    array_num = SHC_GET_DICT_NUM;
    ARRCLR(src_buf);
    for (i=0, dic_idx=0; i<array_num; i++)
    {
        src_buf[i] = dic_idx;
        dic_idx += SHC_get_dict_items(i);
    }
    /* ********************************************************** */

    /* ��ֵ */
    data_words = num;
    offset_words = offset;
    for (i=0; i<data_words; i++)
    {
        if (offset_words+i < array_num)     /* ��Ч���ָ�ֵ */
        {
            dest_buf[idx++] = HIBYTE(src_buf[offset_words+i]);
            dest_buf[idx++] = LOBYTE(src_buf[offset_words+i]);
        }
        else                /* ��Ч���ָ�0 */
        {
            dest_buf[idx++] = 0;
            dest_buf[idx++] = 0;
        }
    }

    return TRUE;
}

/*******************************************************************************
 �� ��: SH_get_dict_string()
 �� ��: ��ȡ�����ֵ��ַ���
 �� ��: dest_buf--���ݷ��ػ���;
        offset--��ʼλ��;
        num--���ݸ���(��);
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_get_dict_string(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i;
    U16 msg = 0;            /* ��ʱ���� */
    U08 idx = 0;            /* ���ݷ������� */
    U08 array_num = 0;      /* �ַ����� */
    U16 dic_max;
    U16 dic_items[SH_MAX_PARA_STRING_GRP];

    /* �������ݼ�� */
    if (num > MAX_TRAS_BYTES/2 || offset+num > SH_MAX_DICT_STR)     /* ͨѶ���泬�� */
    {
        return FALSE;
    }

    /* *************************��������************************* */
    array_num =  SHC_GET_DICT_NUM;
    ARRCLR(dic_items);
    for (i=0, dic_max=0; i<array_num; i++)
    {
        U16 items = SHC_get_dict_items(i);
        dic_items[i] = items;
        dic_max += items;
    }
    /* ********************************************************** */

    dic_max = MIN(dic_max, SH_MAX_DICT_STR);
    for (i=offset; i<offset+num; i++)
    {
        msg = 0;
        if (i < dic_max)    /* dic_max ��Ϊ����ֵ��ַ������� */
        {
            XY xy = fun_get_relative_pos(dic_items, array_num, i);
            msg = SHC_get_dict_pmsg(xy.x, xy.y);
        }
        /* ��ֵ */
        dest_buf[idx++] = HIBYTE(msg);
        dest_buf[idx++] = LOBYTE(msg);

    }
    return TRUE;
}

/*******************************************************************************
 �� ��: SH_get_para_item_info()
 �� ��: ��ȡ��������Ϣ
 �� ��: dest_buf--���ݷ��ػ���;
        offset--��ʼλ��;
        num--���ݸ���(��);
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_get_para_item_info(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i, j;
    U08 grp_idx = 0;        /* ��� */
    U08 item_idx = 0;       /* ��� */
    SH_PARA_ATTRIB para_item;               /* ��ʾ��Ϣ */
    U08 idx = 0;            /* ���ݷ������� */
    U16 *src_buf = NULL;    /* ����ָ�� */

    /* �������ݼ�� */
    if (num*sizeof(SH_PARA_ATTRIB) > MAX_TRAS_BYTES || num+offset > SH_MAX_PARA_ITEM_NUM*SH_MAX_PARA_GRP_NUM)/* ͨѶ���泬�� */
    {
        return FALSE;
    }

    /* *************************��������************************* */

    for (i=offset; i<offset+num; i++)
    {
        /* ������ż���� */
        grp_idx = (U08)(i / SHC_DISP_GET_PARA_GRP_SIZE);
        item_idx = (U08)(i % SHC_DISP_GET_PARA_GRP_SIZE);

        /* Ĭ��Ϊ���� */
        para_item.attrib = (1 << HIDE_BGN_BIT);

        if (grp_idx < SHC_DISP_GET_PARA_GRP_NUM
            && item_idx < SHC_get_para_grp_item(grp_idx))
        {
            para_item.name = SHC_NAME_PARA(grp_idx, item_idx);
            para_item.id = 0xFFFF;
            /* ��ֵ */
            para_item.value = SHC_para_get_val(grp_idx, item_idx);
            /* ��Сֵ */
            para_item.min = SHC_para_get_min(grp_idx, item_idx);
            /* ���ֵ */
            para_item.max = SHC_para_get_max(grp_idx, item_idx);
            /* ���� */
            para_item.attrib = SH_cal_para_attrib(grp_idx, item_idx);
        }
        else
        {
            return FALSE;
        }

        /* ********************************************************** */

        /* ��ֵ */
        src_buf = (U16*)&para_item;
        for (j=0; j<SH_PARA_ATTRIB_SIZE; j++)
        {
            dest_buf[idx++] = HIBYTE(src_buf[j]);
            dest_buf[idx++] = LOBYTE(src_buf[j]);
        }
    }

    return TRUE;
}

/*******************************************************************************
 �� ��: SH_get_bms_para_info()
 �� ��: ��ȡBMSͨѶ���ò�������Ϣ
 �� ��: dest_buf--���ݷ��ػ���;
        offset--��ʼλ��;
        num--���ݸ���(��);
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_get_bms_para_info(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i, j;
    SH_PARA_ATTRIB para_item;               /* ��ʾ��Ϣ */
    U08 idx = 0;            /* ���ݷ������� */
    U16 *src_buf = NULL;    /* ����ָ�� */

    /* �������ݼ�� */
    if (num*sizeof(SH_PARA_ATTRIB) > MAX_TRAS_BYTES || num+offset > SH_MAX_BMS_PARA_NUM)    /* ͨѶ���泬�� */
    {
        return FALSE;
    }

    /* *************************��������************************* */

    for (i=offset; i<offset+num; i++)
    {
        /* Ĭ��Ϊ���� */
        para_item.attrib = (1 << HIDE_BGN_BIT);

        if (i < SHC_BMS_PARA_NUM)
        {
            para_item.name = SHC_BMS_PARA_NAME(i);
            para_item.id = 0xFFFF;
            /* ��ֵ */
            para_item.value= SHC_BMS_PARA_VAL(i);
            /* ��Сֵ */
            para_item.min = SHC_BMS_PARA_MIN(i);
            /* ���ֵ */
            para_item.max = SHC_BMS_PARA_MAX(i);
            /* ���� */
            para_item.attrib = SH_attr_local2sh(SHC_BMS_PARA_SIGN(i), SHC_BMS_PARA_STR(i));
        }
        else if (i < SH_MAX_BMS_PARA_NUM)
        {
            para_item.name = 0;
        }
        else
        {
            return FALSE;
        }

        /* ********************************************************** */

        /* ��ֵ */
        src_buf = (U16*)&para_item;
        for (j=0; j<SH_BMS_PARA_SIZE; j++)
        {
            dest_buf[idx++] = HIBYTE(src_buf[j]);
            dest_buf[idx++] = LOBYTE(src_buf[j]);
        }
    }

    return TRUE;
}

/*******************************************************************************
 �� ��: SH_get_pcb_comp_num()
 �� ��: ��ȡѹ����
 �� ��: dest_buf--���ݷ��ػ���;
        offset--��ʼλ��;
        num--���ݸ���(��);
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_get_pcb_comp_num(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i;
    U08 idx = 0;            /* ���ݷ������� */

    /* �������ݼ�� */
    if (num+offset > SH_MAX_PCB_NUM)
    {
        return FALSE;
    }

    /* *************************��������************************* */

    for (i=offset; i<offset+num; i++)
    {
        dest_buf[idx++] = HIBYTE(SHC_pcb_comp_num(i));
        dest_buf[idx++] = LOBYTE(SHC_pcb_comp_num(i));
    }

    return TRUE;
}

/*******************************************************************************
 �� ��: SH_get_test_ro()
 �� ��: ��ȡ����ģʽ���ַ����͵��򿪶ȵ���Ϣ
 �� ��: dest_buf--���ͻ���;
        offset--��ַƫ��;
        num--���ݸ���;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_get_test_ro(U08 *dest_buf, U16 offset, U16 num)
{
    U08 i;
    U08 idx = 0;

    /* �������ݼ�� */
    if (num*sizeof(U16) > MAX_TRAS_BYTES || offset+num > SH_CHK_TEST_RO_SIZE)/* ͨѶ���泬�� */
    {
        return FALSE;
    }

    /* ��������ַ�����ֵ */
    for (i=0; i<SHC_TEST_DO_NUM; i++)
    {
        test_data.ro.str_DO[i] = SHC_get_test_DO_string(i);
    }

    /* ���Ե����ַ�����ֵ */
    for (i=0; i<SHC_TEST_EEV_NUM; i++)
    {
        test_data.ro.str_EEV[i] = SHC_get_test_eev_string(i);
    }

    /* ������� */
    test_data.ro.DO_num = SHC_TEST_DO_NUM;

    /* ���ͷ����� */
    test_data.ro.EEV_num = SHC_TEST_EEV_NUM;

    /* ���Ե�����С���� */
    for (i=0; i<SHC_TEST_EEV_NUM; i++)
    {
        test_data.ro.EEV_step_min[i] = 0;
    }

    /* ���Ե�������� */
    for (i=0; i<SHC_TEST_EEV_NUM; i++)
    {
        test_data.ro.EEV_step_max[i] = SHC_get_motor_max(i);
    }

    /* ���Ե���ǰ���� */
    for (i=0; i<SHC_TEST_EEV_NUM; i++)
    {
        test_data.ro.EEV_step_pv[i] = SHC_get_motor_pv(i);
    }

    /* �Ƿ�����̵����ֶ�ģʽ */
    test_data.ro.fg_DO_can_mnl_mode = SHC_get_DO_can_mnl();

    /* �Ƿ���������ֶ�ģʽ */
    test_data.ro.fg_EEV_can_mnl_mode = TRUE;

    /* ��ֵ */
    for (i=0; i<num; i++)
    {
        U16 temp = 0;
        if (i < SH_CHK_TEST_RO_SIZE)
        {
            temp = ((U16 *)(&test_data.ro))[i];
        }
        dest_buf[idx++] = HIBYTE(temp);
        dest_buf[idx++] = LOBYTE(temp);
    }
    return TRUE;
}

/*******************************************************************************
 �� ��: SH_get_test_wr()
 �� ��: ��ȡ�������õ�DO״̬
 �� ��: dest_buf--;
        offset--;
        num--;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_get_test_wr(U08 *dest_buf, U16 offset, U16 num)
{
    U08 i;
    U08 idx = 0;
    U16 temp[_DIVUCARRY(SHC_TEST_DO_NUM, 16)];

    /* �������ݼ�� */
    if (num > MAX_TRAS_BYTES/2 || offset+num > SH_CHK_TEST_WR_SIZE)
    {
        return FALSE;
    }

    ARRCLR(temp);
    for (i=0; i<SHC_TEST_DO_NUM; i++)
    {
        if (SHC_get_test_do_sta(i))
        {
            SETBIT_N(temp, i);
        }
    }

    /* ��ֵ */
    for (i=0; i<num; i++)
    {
        dest_buf[idx++] = HIBYTE(temp[i]);
        dest_buf[idx++] = LOBYTE(temp[i]);
    }

    return TRUE;
}

/*****************************************************************************
��  �� : SH_get_chk_info_num()
��  �� : ��ѯ��Ϣ������Ϣ
��  �� : U08 *dest_buf : ���ݷ��ػ���;
         U16 offset    : ��ʼ��Ϣ��Ԫλ��
         U16 num       : ��Ϣ��Ԫ����;
��  �� : TRUE/FALSE--�ɹ�/ʧ��
��  �� : ��
*****************************************************************************/
BOOL SH_get_chk_info_num(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i, j;
    SH_CHK_INFO_NUM info_num;
    U16 idx = 0;            /* ���ݷ������� */
    U16 *src_buf = NULL;    /* ָ��Դ���� */

    /* �������ݼ�� */
    if (num*sizeof(SH_CHK_INFO_NUM) > MAX_TRAS_BYTES/* || offset+num > SH_MAX_CHK_INFO*/)//ͨѶ���泬��
    {
        return FALSE;
    }


    for (i=offset; i<offset+num; i++)
    {
        U16 pcb = i;

        if (pcb < SH_MAX_PCB_NUM)
        {
            SHC_get_info_num(&info_num, pcb);
        }

        /* ********************************************************** */

        /* ������ֵ */
        src_buf = (U16*)&info_num;
        for (j=0; j<SH_CHK_NUM_SIZE; j++)
        {
            dest_buf[idx++] = HIBYTE(src_buf[j]);
            dest_buf[idx++] = LOBYTE(src_buf[j]);
        }
    }

    return TRUE;
}

/*******************************************************************************
 �� ��: SH_get_chk_info_av_mod()
 �� ��: ��ȡ��ǰϵͳ��ģ��ģ������ʾ��Ϣ
 �� ��: dest_buf--���ݷ��ػ���;
        offset--��ʼλ��;
        num--ģ������;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_get_chk_info_av_mod(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i, j;
    SH_CHK_INFO info;       /* ģ������Ϣ */
    U16 idx = 0;            /* ���ݷ������� */
    U16 *src_buf = NULL;    /* ָ��Դ���� */
    U08 model_id = 0;       /* ģ��� */

    /* �������ݼ�� */
    if (num*sizeof(SH_CHK_INFO) > MAX_TRAS_BYTES/* || offset+num > SH_MAX_CHK_INFO*/)//ͨѶ���泬��
    {
        return FALSE;
    }

    model_id = offset;

    for (i=0; i<num; i++)
    {
        /* *************************��������************************* */
        /* ģ�� */
        SHC_get_mod_info(&info, model_id, i);

        /* ********************************************************** */

        /* ������ֵ */
        src_buf = (U16*)&info;
        for (j=0; j<SH_CHK_MOD_AV_SIZE; j++)
        {
            dest_buf[idx++] = HIBYTE(src_buf[j]);
            dest_buf[idx++] = LOBYTE(src_buf[j]);
        }
    }

    return TRUE;
}

/*******************************************************************************
 �� ��: SH_get_chk_info_av_sys()
 �� ��: ��ȡ��ǰϵͳģ������ʾ��Ϣ
 �� ��: dest_buf--���ݷ��ػ���;
        offset--��ʼλ��;
        num--ģ������;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_get_chk_info_av_sys(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i, j;
    SH_CHK_INFO info;       /* ģ������Ϣ */
    U16 idx = 0;            /* ���ݷ������� */
    U16 *src_buf = NULL;    /* ָ��Դ���� */

    /* �������ݼ�� */
    if (num*sizeof(SH_CHK_INFO) > MAX_TRAS_BYTES/* || offset+num > SH_MAX_CHK_INFO*/)//ͨѶ���泬��
    {
        return FALSE;
    }

    for (i=offset; i<offset+num; i++)
    {
        /* *************************��������************************* */

        /* ϵͳ */
        if (i < SHC_get_sys_av_num())
        {
            SHC_get_sys_info(&info, i);
        }

        /* ********************************************************** */

        /* ������ֵ */
        src_buf = (U16*)&info;
        for (j=0; j<SH_CHK_SYS_AV_SIZE; j++)
        {
            dest_buf[idx++] = HIBYTE(src_buf[j]);
            dest_buf[idx++] = LOBYTE(src_buf[j]);
        }
    }

    return TRUE;
}

/*******************************************************************************
 �� ��: SH_get_chk_info_av_comp()
 �� ��: ��ȡ��ǰѹ��ģ������ʾ��Ϣ
 �� ��: dest_buf--���ݷ��ػ���;
        offset--��ʼλ��;
        num--ģ������;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_get_chk_info_av_comp(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i, j;
    SH_CHK_INFO info;       /* ģ������Ϣ */
    U16 idx = 0;            /* ���ݷ������� */
    U16 *src_buf = NULL;    /* ָ��Դ���� */

    /* �������ݼ�� */
    if (num*sizeof(SH_CHK_INFO) > MAX_TRAS_BYTES/* || offset+num > SH_MAX_CHK_INFO*/)//ͨѶ���泬��
    {
        return FALSE;
    }

    for (i=offset; i<offset+num; i++)
    {
        U16 pcb = i / SHC_COMP_PV_STEP;
        U16 pvi = i % SHC_COMP_PV_STEP;      /* ģ���ڵĵڼ���ģ���� */
        info.value = SENSOR_NOEXIST;        /* ��ʼ��Ϊû�� */

        if (pcb < SH_MAX_PCB_NUM)
        {
            SHC_get_comp_info(&info, pcb, pvi);
        }
        /* ********************************************************** */

        /* ������ֵ */
        src_buf = (U16*)&info;
        for (j=0; j<SH_CHK_COMP_AV_SIZE; j++)
        {
            dest_buf[idx++] = HIBYTE(src_buf[j]);
            dest_buf[idx++] = LOBYTE(src_buf[j]);
        }
    }

    return TRUE;
}


/*******************************************************************************
 �� ��: SH_get_chk_info_do()
 �� ��: ��ȡ��ǰ�����������ʾ��Ϣ
 �� ��: dest_buf--���ݷ��ػ���;
        offset--��ʼλ��;
        num--��������;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_get_chk_info_do(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i, j;
    SH_CHK_INFO info;       /* ��������Ϣ */
    U16 idx = 0;            /* ���ݷ������� */
    U16 *src_buf = NULL;    /* ָ��Դ���� */

    /* �������ݼ�� */
    if (num*sizeof(SH_CHK_INFO) > MAX_TRAS_BYTES/* || offset+num > SH_MAX_CHK_INFO*/)//ͨѶ���泬��
    {
        return FALSE;
    }


    for (i=offset; i<offset+num; i++)
    {
        U16 pcb = i / SHC_DO_STEP;
        U16 doi = i % SHC_DO_STEP;

        info.value = SENSOR_NOEXIST;        /* ��ʼ��Ϊû�� */

        if (pcb < SH_MAX_PCB_NUM)
        {
            SHC_get_do_info(&info, pcb, doi);
        }
        /* ********************************************************** */

        /* ������ֵ */
        src_buf = (U16*)&info;
        for (j=0; j<SH_CHK_DO_SIZE; j++)
        {
            dest_buf[idx++] = HIBYTE(src_buf[j]);
            dest_buf[idx++] = LOBYTE(src_buf[j]);
        }
    }
    return TRUE;
}

/*******************************************************************************
 �� ��: SH_get_chk_info_di()
 �� ��: ��ȡ��ǰ�����������ʾ��Ϣ
 �� ��: dest_buf--���ݷ��ػ���;
        offset--��ʼλ��;
        num--��������;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_get_chk_info_di(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i, j;
    SH_CHK_INFO info;       /* ��������Ϣ */
    U16 idx = 0;            /* ���ݷ������� */
    U16 *src_buf = NULL;    /* ָ��Դ���� */

    /* �������ݼ�� */
    if (num*sizeof(SH_CHK_INFO) > MAX_TRAS_BYTES/* || offset+num > SH_MAX_CHK_INFO*/)//ͨѶ���泬��
    {
        return FALSE;
    }


    for (i=offset; i<offset+num; i++)
    {
        U16 pcb = i / SHC_DI_STEP;
        U16 dii = i % SHC_DI_STEP;

        info.value = SENSOR_NOEXIST;        /* ��ʼ��Ϊû�� */

        if (pcb < SH_MAX_PCB_NUM)
        {
            SHC_get_di_info(&info, pcb, dii);
        }
        /* ********************************************************** */

        /* ������ֵ */
        src_buf = (U16*)&info;
        for (j=0; j<SH_CHK_DI_SIZE; j++)
        {
            dest_buf[idx++] = HIBYTE(src_buf[j]);
            dest_buf[idx++] = LOBYTE(src_buf[j]);
        }
    }
    return TRUE;
}

/*******************************************************************************
 �� ��: SH_get_defrost_info()
 �� ��:
 �� ��: dest_buf--;
        offset--;
        num--;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_get_defrost_info(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i, j;
    SH_DEFRO_INFO info;     /* ��������Ϣ */
    U16 idx = 0;            /* ���ݷ������� */
    U16 *src_buf = NULL;    /* ָ��Դ���� */

    /* �������ݼ�� */
    if (num*sizeof(SH_DEFRO_INFO) > MAX_TRAS_BYTES/* || offset+num > SH_MAX_CHK_INFO*/)//ͨѶ���泬��
    {
        return FALSE;
    }

    for (i=offset; i<offset+num; i++)
    {
        U16 pcb = i;

        /* ��ʼ�� */
        info.cell = 0;
        info.cms = 0;

        if (pcb < MAX_PCB)
        {
            SHC_get_defro_info(&info, pcb);
        }
        /* ********************************************************** */

        /* ������ֵ */
        src_buf = (U16*)&info;
        for (j=0; j<DEFR_INFO_SIZE; j++)
        {
            dest_buf[idx++] = HIBYTE(src_buf[j]);
            dest_buf[idx++] = LOBYTE(src_buf[j]);
        }
    }

    return TRUE;
}

/*******************************************************************************
 �� ��: SH_get_start_code_tips()
 �� ��:
 �� ��: dest_buf--;
        offset--;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_get_start_code_tips(U08 *dest_buf, U16 offset)
{
    SHC_get_start_code_tips_string(dest_buf, offset);

    return TRUE;
}

/*******************************************************************************
 �� ��: SH_get_enable_info()
 �� ��: ��ȡʹ��/������Ϣ
 �� ��: dest_buf--���ݷ��ػ���;
        offset--��ʼλ��;
        num--��Ϣ�ṹ����;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
 �� ֲ:
*******************************************************************************/
BOOL SH_get_enable_info(U08 *dest_buf, U16 offset, U16 num)
{
    SH_ENABLE_INFO info;               //��������Ϣ
    U16 *src_buf = NULL;            //ָ��Դ����
    U16 idx = 0;
    U16 n = 0;

    //�������ݼ��
    if (num*sizeof(SH_ENABLE_INFO) > MAX_TRAS_BYTES/* || offset+num > SH_MAX_CHK_INFO*/)//ͨѶ���泬��
    {
        return FALSE;
    }

    VALCLR(info);

    SHC_get_enable(&info);

    //������ֵ
    src_buf = &((U16*)&info)[offset];
    for (idx=0,n=0; n<num; n++)
    {
        dest_buf[idx++] = HIBYTE(src_buf[n]);
        dest_buf[idx++] = LOBYTE(src_buf[n]);
    }


    return TRUE;
}

/*******************************************************************************
 �� ��: SH_get_chk_user_info()
 �� ��: �û���ѯ��Ϣ
 �� ��: dest_buf--;
        offset--;
        num--;
 �� ��:
 �� ��: ��
*******************************************************************************/
BOL SH_get_chk_user_info(U08 *dest_buf, U16 offset, U16 num)
{
    U08 i, j;
    U16 idx = 0;            /* ���ݷ������� */
    U16 *src_buf = NULL;    /* ָ��Դ���� */
    SH_CHK_INFO info;

    /* �������ݼ�� */
    if (num*sizeof(SH_CHK_INFO) > MAX_TRAS_BYTES)
    {
        return FALSE;
    }

    for (i=offset; i<offset+num; i++)
    {
        SHC_get_user_info(&info, i);

        /* ������ֵ */
        src_buf = (U16*)&info;
        for (j=0; j<SH_CHK_USER_SIZE; j++)
        {
            dest_buf[idx++] = HIBYTE(src_buf[j]);
            dest_buf[idx++] = LOBYTE(src_buf[j]);
        }
    }

    return TRUE;
}

/*******************************************************************************
 �� ��: SH_get_invt_para()
 �� ��:
 �� ��: dest_buf--;
        data_num--;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_get_invt_para(U08 *dest_buf, U16 data_num)
{
    SH_INVT_PARA info;
    U16 *src_buf = PNULL;
    U16 idx = 0;
    U16 n = 0;

    /* �������ݼ�� */
    if (data_num != SH_INVTPARA_SIZE)   /* ��ȡ������һ�� */
    {
        return FALSE;
    }

    VALCLR(info);

    SHC_get_invt_para(&info);

    //������ֵ
    src_buf = (U16*)&info;
    for (idx=0,n=0; n<data_num; n++)
    {
        dest_buf[idx++] = HIBYTE(src_buf[n]);
        dest_buf[idx++] = LOBYTE(src_buf[n]);
    }

    return TRUE;
}

/*******************************************************************************
 �� ��: SH_get_floor_preheat_info()
 �� ��:
 �� ��: dest_buf--;
		offset--;
		num--;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_get_floor_preheat_info(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i, j;
    SH_PARA_ATTRIB info;       /* ģ������Ϣ */
    U16 idx = 0;            /* ���ݷ������� */
    U16 *src_buf = NULL;    /* ָ��Դ���� */

    /* �������ݼ�� */
    if (num*sizeof(SH_PARA_ATTRIB) > MAX_TRAS_BYTES)//ͨѶ���泬��
    {
        return FALSE;
    }

    for (i=offset; i<offset+num; i++)
    {
        /* *************************��������************************* */

        SHC_get_floor_preheat_info(&info, i);

        /* ********************************************************** */

        /* ������ֵ */
        src_buf = (U16*)&info;
        for (j=0; j<SH_FLOOR_PREHEAT_SIZE; j++)
        {
            dest_buf[idx++] = HIBYTE(src_buf[j]);
            dest_buf[idx++] = LOBYTE(src_buf[j]);
        }
    }

    return TRUE;
}

/*******************************************************************************
 �� ��: SH_get_floor_timer_info()
 �� ��:
 �� ��: dest_buf--;
		offset--;
		num--;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_get_floor_timer_info(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i, j;
    SH_PARA_ATTRIB info;       /* ģ������Ϣ */
    U16 idx = 0;            /* ���ݷ������� */
    U16 *src_buf = NULL;    /* ָ��Դ���� */

    /* �������ݼ�� */
    if (num*sizeof(SH_PARA_ATTRIB) > MAX_TRAS_BYTES)//ͨѶ���泬��
    {
        return FALSE;
    }

    for (i=offset; i<offset+num; i++)
    {
        /* *************************��������************************* */

        SHC_get_floor_timer_info(&info, i);

        /* ********************************************************** */

        /* ������ֵ */
        src_buf = (U16*)&info;
        for (j=0; j<SH_FLOOR_PREHEAT_SIZE; j++)
        {
            dest_buf[idx++] = HIBYTE(src_buf[j]);
            dest_buf[idx++] = LOBYTE(src_buf[j]);
        }
    }

    return TRUE;
}

/*******************************************************************************
 �� ��: SH_get_test_fac_hmi_info()
 �� ��: ��ȡ��ʾ���̼���Ϣ
 �� ��: dest_buf--���ݷ��ػ���;
        offset--��ʼλ��;
        num--��ȡ���ݸ���;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_get_test_fac_hmi_info(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i;
    SH_TEST_FAC_HMI_STRU info;         /* ������Ϣ */
    U16 idx = 0;            /* ���ݷ������� */
    U16 *src_buf = NULL;

    /* �������ݼ�� */
    if (offset+num > SH_TEST_FAC_HMI_RD_SIZE) /* ͨѶ���泬�� */
    {
        return FALSE;
    }

    VALCLR(info);
    // ��ֵ
    SHC_get_test_fac_hmi_info(&info);

    /* ������ֵ */
    src_buf = (U16 *)&info + offset;
    for(i=0; i<SH_TEST_FAC_HMI_RD_SIZE; i++)
    {
        dest_buf[idx++] = HIBYTE(src_buf[i]);
        dest_buf[idx++] = LOBYTE(src_buf[i]);
    }

    return TRUE;
}

/*******************************************************************************
 �� ��: SH_set_command()
 �� ��: ��Ӧ�յ�������
 �� ��: src_buf--����Դ;
        offset--��ʼλ��;
        num--���ݸ���(��);
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_set_command(U08 *src_buf, U16 offset, U16 num)
{
    I16 command;            /* ������Ϣ */

    /* �������ݼ�� */
    if (num != 1 || offset+num > SH_MAX_CMD)
    {
        return FALSE;
    }

    /* ��ֵ */
    command = MAKEWORD(src_buf[0], src_buf[1]);

    /* *************************��������************************* */
    switch (offset)
    {
        /* 0 ���� */
        case SH_CMD_ON:
            SHC_command_ON_deal(command);
            break;

        /* 1 �ػ� */
        case SH_CMD_OFF:
            SHC_command_OFF_deal(command);
            break;

        /* 2 ���ϸ�λ */
        case SH_CMD_RST:
            SHC_command_RST_deal(command);
            break;

        /* 3 ������ϸ�λ */
        case SH_CMD_PASS_RST:
            SHC_command_PASS_RST_deal(command);
            break;

        /* 4 ȡ��Ԥ�� */
        case SH_CMD_OFF_OILHEAT:
            SHC_command_OFF_OILHEAT_deal(command);
            break;

        /* 5 ģ���ֶ���˪ */
        case SH_CMD_DEFRO:
            SHC_command_DEFRO_deal(command);
            break;

        /* 6 ������ʼ�� */
        case SH_CMD_PARA_INIT:
            SHC_command_PARA_INIT_deal(command);
            break;

        /* 7 ѹ���ۼ�����ʱ���ʼ�� */
        case SH_CMD_CM_RUNTM_INIT:
            SHC_command_CM_RUNTM_INIT_deal(command);
            break;

        /* 8 ϵͳ�ۼ�����ʱ���ʼ�� */
        case SH_CMD_SYS_RUNTM_INIT:
            SHC_command_SYS_RUNTM_INIT_deal(command);
            break;

        /* 9 �����ʷ���� */
        case SH_CMD_HIS_ERR_INIT:
            SHC_command_HIS_ERR_INIT_deal(command);
            break;

        /* 10 ��Ƶ����ʼ�� */
        case SH_CMD_INVERTER_INIT:
            SHC_command_INVERTER_INIT_deal(command);
            break;

        /* 11 �ֶ�����ˮ�� */
        case SH_CMD_PUMP:
            SHC_command_PUMP_deal(command);
            break;

        /* 12 BMS�������� */
        case SH_CMD_BMS_PARA_SET:
            SHC_command_BMS_PARA_deal(command);
            break;

        /* 13 ����Ԥ�� */
        case SH_CMD_QUICK_OILHEAT:
            SHC_command_QUICK_OILHEAT_deal(command);
            break;

        /* 14 Ϳѻ�������ӱ�־ */
        case SH_CMD_TUYA_NEW_LINK:
            SHC_command_TUYA_NEW_LINK_deal(command);
            break;

        /* 16 �����ů��ɹ��� */
        case SH_CMD_ENTER_FLOOR_DRYUP:
            SHC_command_enter_floor_dryup_deal(command);
            break;

        /* 17 ǿ���������� */
        case SH_CMD_FORCE_IBH:
            SHC_command_force_ibh_deal(command);
            break;

        /* 18 ǿ��ˮ����� */
        case SH_CMD_FORCE_TBH:
            SHC_command_force_tbh_deal(command);
            break;

        /* 19 ǿ����ˮģʽ */
        case SH_CMD_FORCE_HW_MODE:
            SHC_command_force_hw_mode_deal(command);
            break;

        /* 20 �������� */
        case SH_CMD_ABILITY_TEST:
            SHC_command_ability_test_deal(command);
            break;

        /* 21 ����һ���� */
        case SH_CMD_ON_ZONE_1:
            SHC_command_ON_zone_1_deal(command);
            break;

        /* 22 ����һ�ػ� */
        case SH_CMD_OFF_ZONE_1:
            SHC_command_OFF_zone_1_deal(command);
            break;

        /* 23 ��������� */
        case SH_CMD_ON_ZONE_2:
            SHC_command_ON_zone_2_deal(command);
            break;

        /* 24 ������ػ� */
        case SH_CMD_OFF_ZONE_2:
            SHC_command_OFF_zone_2_deal(command);
            break;

        /* 25 ��ˮ���� */
        case SH_CMD_ON_HOTW:
            SHC_command_ON_hotwater_deal(command);
            break;

        /* 26 ��ˮ�ػ� */
        case SH_CMD_OFF_HOTW:
            SHC_command_OFF_hotwater_deal(command);
            break;

        /* 27 ϵͳά����ʼ�� */
        case SH_CMD_LIMIT_INIT:
            SHC_command_limit_init_deal(command);
        	break;

        /* 28 �շ����� */
        case SH_CMD_RECYCLE_REFRI:
            SHC_command_recyle_refri_deal(command);
        	break;

        /* 29 ��ʱ����ˮ�� */
        case SH_CMD_TIMER_DHW_ON:
            SHC_command_TIMER_DHW_ON_deal(command);
        	break;

        /* 30 ǿ���ⲿ��Դ */
        case SH_CMD_FORCE_AHS:
            SHC_command_force_AHS_deal(command);
        	break;

        /* 31 ɱ�� */
        case SH_CMD_DISINFECT:
            SHC_command_disinfect_deal(command);
        	break;

        /* 31 ����ģʽ */
        case SH_CMD_SILENT:
            SHC_command_silent_deal(command);
        	break;

        /* 33 ˮ�������� */
        case SH_CMD_PUMP_TEST:
            SHC_command_pump_test_deal(command);
        	break;

        /* 34 ���������� */
        case SH_CMD_COOL_TEST:
            SHC_command_cool_test_deal(command);
        	break;

        /* 35 ���������� */
        case SH_CMD_HEAT_TEST:
            SHC_command_heat_test_deal(command);
        	break;

        /* 36 ����ˮ������ */
        case SH_CMD_HOTWATER_TEST:
            SHC_command_hotwater_test_deal(command);
        	break;

        /* 37 ϵͳ�ſյ��� */
        case SH_CMD_SYSTEM_EMPTY:
            SHC_command_system_empty_deal(command);
        	break;

        /* 38 ��ůԤ�� */
        case SH_CMD_FLOOR_PREHEAT:
            SHC_command_floor_preheat_deal(command);
        	break;

        /* 39 ��ʱ��ů */
        case SH_CMD_FLOOR_TIMER:
            SHC_command_floor_timer_deal(command);
        	break;

        /* 40 �����뿪 */
        case SH_CMD_HOLIDAY_AWAY:
            SHC_command_holiday_away_deal(command);
        	break;

        /* 41 �����ڼ� */
        case SH_CMD_HOLIDAY_HOME:
            SHC_command_holiday_home_deal(command);
        	break;

        /* 42 ����Զ���������� */
        case SH_CMD_ACCEPT_REMOTE_UPGRADE_REQ:
            SHC_command_accept_remote_upgrade_req_deal(command);
        	break;

        default:
            break;
    }
    /* ********************************************************** */

    return TRUE;
}

/*******************************************************************************
 �� ��: SH_set_setting()
 �� ��: ��Ӧ���趨
 �� ��: src_buf--����Դ;
        offset--��ʼλ��;
        num--���ݸ���(��);
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_set_setting(U08 *src_buf, U16 offset, U16 num)
{
    I16 set_data;           /* ������Ϣ */
    BOOL result = TRUE;     /* ���ý�� */

    /* �������ݼ�� */
    if (num != 1 || offset+num > SH_MAX_SET_INFO)
    {
        return FALSE;
    }

    /* ��ֵ */
    set_data = MAKEWORD(src_buf[0], src_buf[1]);

    /* *************************��������************************* */
    switch (offset)
    {
        /* 0 Ŀ���¶� */
        case SET_TMP_SET:
            result = SHC_set_tmp_deal(set_data);
            break;

        /* 1 �趨ģʽ */
        case SET_MODE_SET:
            result = SHC_set_mode_deal(set_data);
            break;

        /* 2 ʹ������ */
        case SET_LIMIT_TIME:
            result = SHC_set_limit_time_deal(set_data);
            break;

        /* 3 ��ʱʹ��(TRUE:ʹ��,FALSE:����) */
        case SET_TIMING_USE:
            result = SHC_set_timing_use_deal(set_data);
            break;

        /* 4 ����������빦�� */
        case SET_START_CODE:
            result = SHC_set_start_code_deal(set_data);
            break;

        /* 5 �趨ʪ��ģʽ */
        case SET_HUMI_MODE:
            result = SHC_set_humi_mode_deal(set_data);
            break;

        /* 6 �趨���� */
        case SET_LANGUAGE:
            result = SHC_set_language_deal(set_data);
            break;

        /* 7 ����һĿ���¶� */
        case SET_TMP_SET_ZONE_1:
            result = SHC_set_tmp_zone_1_deal(set_data);
            break;

        /* 8 �����Ŀ���¶� */
        case SET_TMP_SET_ZONE_2:
            result = SHC_set_tmp_zone_2_deal(set_data);
            break;

        /* 9 ��ˮĿ���¶� */
        case SET_TMP_SET_ZONE_HOTW:
            result = SHC_set_tmp_zone_hotw_deal(set_data);
            break;

        /* 10 ����ά������ */
        case SET_LIMIT_TYPE:
            result = SHC_set_lmt_type_deal(set_data);
            break;

        /* 11 ���þ������� */
        case SET_SILENT_LEVEL:
            result = SHC_set_silent_level_deal(set_data);
            break;

        /* 12 �����¶ȵ�λ */
        case SET_TEMP_UNIT:
            result = SHC_set_temp_unit_deal(set_data);
            break;

        default:
            result = FALSE;
            break;
    }
    /* ********************************************************** */

    return result;
}

/*******************************************************************************
 �� ��: SH_set_passwrd()
 �� ��: ��Ӧ�յ�������
 �� ��: src_buf--����Դ;
        offset--��ʼλ��;
        num--���ݸ���(��);
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_set_passwrd(U08 *src_buf, U16 offset, U16 num)
{
    U08 i;
    U08 idx = 0;            /* ���ݶ�ȡ���� */
    SH_PASS passwrd;        /* ���� */
    U16 pass_grp = 0;       /* ������ */
    U16 *passwrd_dest = NULL;               /* ָ���������� */

    /* �������ݼ�� */
    if (num*sizeof(SH_PASS) > MAX_TRAS_BYTES || offset+num > SH_MAX_PASS_GRP)   /* ͨѶ���泬�� */
    {
        return FALSE;
    }

    for (pass_grp=offset; pass_grp<offset+num; pass_grp++)
    {
        U16 data_offset = sizeof(SH_PASS) * (pass_grp - offset);    /* ����ƫ�� */
        /* ��ֵ */
        passwrd.authority = MAKEWORD(src_buf[data_offset], src_buf[data_offset+1]);
        for (i=0; i<MAX_PASS_BIT; i++)
        {
            U16 pwd_offset = data_offset+2;
            passwrd.pass_bit[i] = src_buf[pwd_offset+i];
        }

        /* *************************��������************************* */

        /* ����Ȩ���ж������� */
        switch (passwrd.authority)
        {
            case SH_PWR_SALE:
                passwrd_dest = (U16 *)&password[PASS_LIMIT][0];
                SETBIT(passwd_modify, PASS_LIMIT);
                break;
            case SH_PWR_USER:
                passwrd_dest = (U16 *)&password[PASS_USER][0];
                SETBIT(passwd_modify, PASS_USER);
                break;
            case SH_PWR_PRO:
                passwrd_dest = (U16 *)&password[PASS_PRO][0];
                SETBIT(passwd_modify, PASS_PRO);
                break;
            case SH_PWR_SER:
                passwrd_dest = (U16 *)&password[PASS_SER][0];
                SETBIT(passwd_modify, PASS_SER);
                break;
            case SH_PWR_FAC:
                passwrd_dest = (U16 *)&password[PASS_FAC][0];
                SETBIT(passwd_modify, PASS_FAC);
                break;

            default:
                passwrd_dest = (U16 *)NULL;
                return FALSE;
        }

        /* ���������� */
        if (passwrd_dest != (U16 *)NULL)
        {
            idx = 0;
            for (i=0; i<MAX_PASS_BIT/2; i++)
            {
                if (fg_pass_big_endian == TRUE)
                {
                    passwrd_dest[i] = MAKEWORD(passwrd.pass_bit[idx], passwrd.pass_bit[idx+1]);
                }
                else
                {
                    passwrd_dest[i] = MAKEWORD(passwrd.pass_bit[idx+1], passwrd.pass_bit[idx]);
                }
                idx += 2;
            }
        }
        /* ********************************************************** */

    }

    return TRUE;
}

/*******************************************************************************
 �� ��: SH_set_clock()
 �� ��: ����ʱ��
 �� ��: src_buf--����Դ;
        offset--��ʼλ��;
        num--���ݸ���(��);
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
 �� ֲ: ����Ϊǰ���·���ʱ�䡣
*******************************************************************************/
BOOL SH_set_clock(U08 *src_buf, U16 offset, U16 num)
{
    U08 i;
    SH_CLOCK time_buff;     /* ʱ�ӽ��ܻ����� */
    U08 *dest_buf = NULL;

    /* �������ݼ�� */
    if (num > MAX_TRAS_BYTES/2 || offset+num > SH_CLOCK_SIZE)
    {
        return FALSE;
    }

    /* ��ֵ */
    dest_buf = (U08*)&time_buff;
    for (i=0; i<sizeof(SH_CLOCK)/2; i++)    /* ��λ�ַ������ߵ��ֽ�˳�� */
    {
        dest_buf[2*i+0] = src_buf[2*i+1];
        dest_buf[2*i+1] = src_buf[2*i+0];
    }

    /* *************************��������************************* */
    SHC_set_clk_deal(&time_buff);

    /* ********************************************************** */

    return TRUE;
}

/*******************************************************************************
 �� ��: SH_set_holidayinfo()
 �� ��: ���ü���ģʽ����
 �� ��: src_buf--����Դ;
        data_num--���ݸ���(��);
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
 �� ֲ: ����Ϊǰ���·���ʱ�䡣
*******************************************************************************/
BOOL SH_set_holidayinfo(U08 *src_buf, U16 data_num)
{
    U08 i;
    SH_HOLIDAYINFO info_buff;     /* ���ܻ����� */
    U08 *dest_buf = NULL;

    /* �������ݼ�� */
    if (data_num != SH_HOLIDAYINFO_SIZE)
    {
        return FALSE;
    }

    /* ��ֵ */
    dest_buf = (U08*)&info_buff;
    for (i=0; i<SH_HOLIDAYINFO_SIZE; i++)    /* ��λ�ַ������ߵ��ֽ�˳�� */
    {
        dest_buf[2*i+0] = src_buf[2*i+1];
        dest_buf[2*i+1] = src_buf[2*i+0];
    }

    /* *************************��������************************* */
    SHC_set_holidayinfo_deal(info_buff);

    /* ********************************************************** */

    return TRUE;
}

/*******************************************************************************
 �� ��: SH_set_room_temp()
 �� ��: ���ü���ģʽ����
 �� ��: src_buf--����Դ;
        data_num--���ݸ���(��);
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
 �� ֲ: ����Ϊǰ���·���ʱ�䡣
*******************************************************************************/
BOOL SH_set_room_temp(U08 *src_buf, U16 data_num)
{
    U08 i;
    U08 *dest_buf = NULL;
    I16 info;

    /* �������ݼ�� */
    if (data_num != SH_ROOM_TEMP_SIZE)
    {
        return FALSE;
    }

    /* ��ֵ */

    /* *************************��������************************* */
    dest_buf = (U08*)&info;
    for (i=0; i<SH_ROOM_TEMP_SIZE; i++)    /* ��λ�ַ������ߵ��ֽ�˳�� */
    {
        dest_buf[2*i+0] = src_buf[2*i+1];
        dest_buf[2*i+1] = src_buf[2*i+0];
    }

    SHC_set_room_temp_deal(info);
    /* ********************************************************** */

    return TRUE;
}

/*******************************************************************************
 �� ��: SH_set_para()
 �� ��: �����²���
 �� ��: src_buf--����Դ;
        offset--��ʼλ��;
        num--���ݸ���(��);
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_set_para(U08 *src_buf, U16 offset, U16 num)
{
    U08 i;
    U08 grp_idx = 0;        /* ��� */
    U08 item_idx = 0;       /* ��� */
    I16 value = 0;

    /* �������ݼ�� */
    if (num > MAX_TRAS_BYTES/2 || num+offset > SH_MAX_PARA_ITEM_NUM*SH_MAX_PARA_GRP_NUM)/* ͨѶ���泬�� */
    {
        return FALSE;
    }

    for (i=0; i<num; i++)
    {
        /* ������ż���� */
        grp_idx = (U08)((offset+i) / SHC_DISP_GET_PARA_GRP_SIZE);
        item_idx = (U08)((offset+i) % SHC_DISP_GET_PARA_GRP_SIZE);
        value = MAKEWORD(src_buf[2*i], src_buf[2*i+1]);

        /* *************************��������************************* */
        SHC_set_para_deal(grp_idx, item_idx, value);
        /* ********************************************************** */
    }

    return TRUE;
}

/*******************************************************************************
 �� ��: SH_set_test_wo()
 �� ��: ���õ���ֻд����
 �� ��: src_buf--;
        offset--;
        num--;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_set_test_wo(U08 *src_buf, U16 offset, U16 num)
{
    U08 i;

    /* �������ݼ�� */
    if (num > MAX_TRAS_BYTES/2 || offset+num > SH_CHK_TEST_WO_SIZE)
    {
        return FALSE;
    }

    for (i=offset; i<offset+num; i++)
    {
        ((U16*)(&test_data.wo))[i] = MAKEWORD(src_buf[2*(i-offset)], src_buf[2*(i-offset)+1]);
    }

    return TRUE;
}

/*******************************************************************************
 �� ��: SH_set_test_wr()
 �� ��: ���õ��Զ�д����
 �� ��: src_buf--;
        offset--;
        num--;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_set_test_wr(U08 *src_buf, U16 offset, U16 num)
{
    U08 i;

    /* �������ݼ�� */
    if (num > MAX_TRAS_BYTES/2 || offset+num > SH_CHK_TEST_WR_SIZE)
    {
        return FALSE;
    }

    for (i=offset; i<offset+num; i++)
    {
        ((U16*)(&test_data.wr))[i] = MAKEWORD(src_buf[2*(i-offset)], src_buf[2*(i-offset)+1]);
    }

    return TRUE;
}

/*******************************************************************************
 �� ��: SH_set_pass_big_endian()
 �� ��:
 �� ��: ��
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_set_pass_big_endian(void)
{
    fg_pass_big_endian = TRUE;

    return TRUE;
}

/*******************************************************************************
 �� ��: SH_set_invt_para()
 �� ��:
 �� ��: src_buf--;
        data_num--;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_set_invt_para(U08 *src_buf, U16 data_num)
{
    U08 i;
    SH_INVT_PARA info_buff;     /* ���ܻ����� */
    U08 *dest_buf = NULL;

    /* �������ݼ�� */
    if (data_num != SH_INVTPARA_SIZE)
    {
        return FALSE;
    }

    /* ��ֵ */
    dest_buf = (U08*)&info_buff;
    for (i=0; i<SH_INVTPARA_SIZE; i++)    /* ��λ�ַ������ߵ��ֽ�˳�� */
    {
        dest_buf[2*i+0] = src_buf[2*i+1];
        dest_buf[2*i+1] = src_buf[2*i+0];
    }

    /* *************************��������************************* */
    SHC_set_invt_para_deal(info_buff);

    /* ********************************************************** */

    return TRUE;
}

/*******************************************************************************
 �� ��: SH_set_floor_preheat_val()
 �� ��:
 �� ��: src_buf--;
		data_num--;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_set_floor_preheat_val(U08 *src_buf, U16 offset, U16 num)
{
    U08 i;
    U08 idx = 0;        /* ��� */
    I16 value = 0;

    /* �������ݼ�� */
    if (num > MAX_TRAS_BYTES/2 || num+offset > SH_MAX_FLOOR_PREHEAT_INFO)/* ͨѶ���泬�� */
    {
        return FALSE;
    }

    for (i=0; i<num; i++)
    {
        idx = offset + i;
        value = MAKEWORD(src_buf[2*i], src_buf[2*i+1]);

        /* *************************��������************************* */
        SHC_set_floor_preheat_val_deal(idx, value);
        /* ********************************************************** */
    }

    return TRUE;
}

/*******************************************************************************
 �� ��: SH_set_floor_timer_val()
 �� ��:
 �� ��: src_buf--;
		data_num--;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_set_floor_timer_val(U08 *src_buf, U16 offset, U16 num)
{
    U08 i;
    U08 idx = 0;        /* ��� */
    I16 value = 0;

    /* �������ݼ�� */
    if (num > MAX_TRAS_BYTES/2 || num+offset > SH_MAX_FLOOR_TIMER_INFO)/* ͨѶ���泬�� */
    {
        return FALSE;
    }

    for (i=0; i<num; i++)
    {
        idx = offset + i;
        value = MAKEWORD(src_buf[2*i], src_buf[2*i+1]);

        /* *************************��������************************* */
        SHC_set_floor_timer_val_deal(idx, value);
        /* ********************************************************** */
    }

    return TRUE;
}

/*******************************************************************************
 �� ��: SH_set_test_fac_hmi_info()
 �� ��:
 �� ��: src_buf--;
        data_num--;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SH_set_test_fac_hmi_info(U08 *src_buf, U16 data_num)
{
    U08 i;
    SH_TEST_FAC_HMI_STRU info_buff;     /* ���ܻ����� */
    U08 *dest_buf = NULL;

    /* �������ݼ�� */
    if (data_num != SH_TEST_FAC_HMI_WR_SIZE)
    {
        return FALSE;
    }

    /* ��ֵ */
    dest_buf = (U08*)&info_buff.wr;
    for (i=0; i<SH_TEST_FAC_HMI_WR_SIZE; i++)    /* ��λ�ַ������ߵ��ֽ�˳�� */
    {
        dest_buf[2*i+0] = src_buf[2*i+1];
        dest_buf[2*i+1] = src_buf[2*i+0];
    }

    /* *************************��������************************* */
    SHC_set_test_fac_hmi_info_deal(info_buff);

    /* ********************************************************** */

    return TRUE;
}

/****************************************************************
������SH_MB_rd_reg
���ܣ�����HMI�������Ķ�����������
������RxBuf--���ջ�����;
      TxBuf--���ͻ�����;
      pTxTotal--�跢�͵��ֽ�����;
���أ��ɹ�/ʧ��
��������
*****************************************************************/
U08 SH_MB_rd_reg(const U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
    U16 addr_offst = 0;     /* ������ʼλ��(�����ݵ�ַ����) */
    U16 data_num = 0;
    U16 data_addr = 0;
    U08 com_err = SH_MB_NO_ERR;             /* ����״̬ */
    BOOL fg_rd_succ = FALSE;/* �����ⲿ�ӳ���д��״̬��־ */
    U16 cell_offst = 0;     /* ��Ԫ��ʼλ��(�ӵڼ����ṹ�嵥Ԫ��ʼ��) */
    U16 cell_num = 0;       /* ��Ԫ����(��ȡ�����ṹ�嵥Ԫ) */

    data_addr = MAKEWORD(RxBuf[2], RxBuf[3]);               /* ���ݵ�ַ */
    data_num = MAKEWORD(RxBuf[4], RxBuf[5]);/* ���ݸ��� */

    if (RxBuf[1] == 0x03)   /* �ɶ�д */
    {
        /* �¾�ע��ֻΪ�������޷��������ڵ���0�ı��뾯�� */
        if (/*data_addr >= ADDR_CMD_BGN && */data_addr + data_num <= ADDR_SH_CMD_END)//����
        {
            /* ��֧�ֶ����� */
        }
        else if (data_addr >= ADDR_SH_SET_BGN && data_addr + data_num <= ADDR_SH_SET_END)   /* ���� */
        {
            /* ��֧�ֶ����� */
        }
        else if (data_addr >= ADDR_SH_PASS_BGN && data_addr + data_num <= ADDR_SH_PASS_END) /* ���� */
        {
            addr_offst = data_addr - ADDR_SH_PASS_BGN;
            cell_offst = addr_offst / SH_PASS_SIZE;
            cell_num = data_num / SH_PASS_SIZE;
            fg_rd_succ = SH_get_passwrd(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_CLOCK_BGN && data_addr + data_num <= ADDR_SH_CLOCK_END)   /* ʱ�� */
        {
            /* �ݲ�֧�ֶ�ʱ�� */
        }
        else if (data_addr >= ADDR_SH_HOLIDAYINFO_BGN && data_addr + data_num <= ADDR_SH_HOLIDAYINFO_END)   /* ����ģʽ */
        {
            fg_rd_succ = SH_get_holidayinfo(&TxBuf[3], data_num);
        }
        else if (data_addr >= ADDR_SH_PARA_BGN && data_addr + data_num <= ADDR_SH_PARA_END) /* ����ֵ */
        {
            /* ����ֵ�ݴӲ��������ж� */
        }
        else if (data_addr >= ADDR_SH_CHK_TEST_WR_BGN && data_addr + data_num <= ADDR_SH_CHK_TEST_WR_END)   /* �����ַ����� */
        {
            addr_offst = data_addr - ADDR_SH_CHK_TEST_WR_BGN;
            cell_offst = addr_offst;
            cell_num = data_num;
            fg_rd_succ = SH_get_test_wr(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_TUYA_INFO_BGN && data_addr + data_num <= ADDR_SH_TUYA_INFO_END)
        {
            fg_rd_succ = SHC_get_tuya_info(&TxBuf[3]);
        }
        else if (data_addr >= ADDR_SH_TUYA_DATA_BGN && data_addr + data_num <= ADDR_SH_TUYA_DATA_END)
        {
            fg_rd_succ = SHC_get_tuya_data(&TxBuf[3], data_addr);
        }
        else if (data_addr >= ADDR_SH_INVTPARA_BGN && data_addr + data_num <= ADDR_SH_INVTPARA_END)
        {
            fg_rd_succ = SH_get_invt_para(&TxBuf[3], data_num);
        }
		else if (data_addr >= ADDR_SH_TEST_FAC_HMI_RD_BGN && data_addr + data_num <= ADDR_SH_TEST_FAC_HMI_RD_END)
        {
            addr_offst = data_addr - ADDR_SH_TEST_FAC_HMI_RD_BGN;
            fg_rd_succ = SH_get_test_fac_hmi_info(&TxBuf[3], addr_offst, data_num);
        }

    }
    else                    /* ֻ�� */
    {
        if (data_addr >= SH_ADDR_STRING_START_CODE_BGN && data_addr + data_num <= SH_ADDR_STRING_START_CODE_END)
        {
            addr_offst = data_addr - SH_ADDR_STRING_START_CODE_BGN;
            cell_offst = addr_offst / STRING_START_CODE_SIZE;
            fg_rd_succ = SH_get_start_code_tips(&TxBuf[3], cell_offst);
        }
        else if (data_addr >= SH_ADDR_ENABLE_INFO_BGN && data_addr + data_num <= SH_ADDR_ENABLE_INFO_END)
        {
            addr_offst = data_addr - SH_ADDR_ENABLE_INFO_BGN;
            fg_rd_succ = SH_get_enable_info(&TxBuf[3], addr_offst, data_num);
        }
        else if (data_addr >= SH_ADDR_DEFR_INFO_BGN && data_addr + data_num <= SH_ADDR_DEFR_INFO_END)   /* ��˪��Ϣ */
        {
            addr_offst = data_addr - SH_ADDR_DEFR_INFO_BGN;
            cell_offst = addr_offst / DEFR_INFO_SIZE;
            cell_num = data_num / DEFR_INFO_SIZE;
            fg_rd_succ = SH_get_defrost_info(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_DISP_BGN && data_addr + data_num <= ADDR_SH_DISP_END) /* ��ʾ������Ϣ */
        {
            addr_offst = data_addr - ADDR_SH_DISP_BGN;
            cell_offst = addr_offst;
            cell_num = data_num;
            fg_rd_succ = SH_get_disp_info(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_CHK_NUM_BGN && data_addr + data_num <= ADDR_SH_CHK_NUM_END)/* ��ѯ��Ϣ���� */
        {
            addr_offst = data_addr - ADDR_SH_CHK_NUM_BGN;
            cell_offst = addr_offst / SH_CHK_NUM_SIZE;
            cell_num = data_num / SH_CHK_NUM_SIZE;
            fg_rd_succ = SH_get_chk_info_num(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_CHK_TEST_RO_BGN && data_addr + data_num <= ADDR_SH_CHK_TEST_RO_END)   /* �����ַ����� */
        {
            addr_offst = data_addr - ADDR_SH_CHK_TEST_RO_BGN;
            cell_offst = addr_offst;
            cell_num = data_num;
            fg_rd_succ = SH_get_test_ro(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_VER_BGN && data_addr + data_num <= ADDR_SH_VER_END)   /* �汾�� */
        {
            addr_offst = data_addr - ADDR_SH_VER_BGN;
            cell_offst = addr_offst / SH_VER_SIZE;
            cell_num = data_num / SH_VER_SIZE;
            fg_rd_succ = SH_get_version(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_ERR_CRT_BGN && data_addr + data_num <= ADDR_SH_ERR_CRT_END)/* ���� ��ǰ */
        {
            addr_offst = data_addr - ADDR_SH_ERR_CRT_BGN;
            cell_offst = addr_offst / SH_ERR_CRT_SIZE;
            cell_num = data_num / SH_ERR_CRT_SIZE;
            fg_rd_succ = SH_get_curt_error(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_ERR_HIS_BGN && data_addr + data_num <= ADDR_SH_ERR_HIS_END)/* ���� ��ʷ */
        {
            addr_offst = data_addr - ADDR_SH_ERR_HIS_BGN;
            cell_offst = addr_offst / SH_ERR_HIS_SIZE;
            cell_num = data_num / SH_ERR_HIS_SIZE;
            fg_rd_succ = SH_get_his_error(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_FCUINFO_BGN && data_addr + data_num <= ADDR_SH_FCUINFO_END)/* ������Ϣ */
        {
            addr_offst = data_addr - ADDR_SH_FCUINFO_BGN;
            fg_rd_succ = SH_get_fcuinfo(&TxBuf[3], addr_offst, data_num);
        }
        else if (data_addr >= ADDR_SH_PARA_ATTRIB_BGN && data_addr + data_num <= ADDR_SH_PARA_ATTRIB_END)   /* ���������� */
        {
            addr_offst = data_addr - ADDR_SH_PARA_ATTRIB_BGN;
            cell_offst = addr_offst / SH_PARA_ATTRIB_SIZE;
            cell_num = data_num / SH_PARA_ATTRIB_SIZE;
            fg_rd_succ = SH_get_para_item_info(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_PCB_COMP_NUM_BGN && data_addr + data_num <= ADDR_SH_PCB_COMP_NUM_END) /* ѹ������ */
        {
            addr_offst = data_addr - ADDR_SH_PCB_COMP_NUM_BGN;
            cell_offst = addr_offst / SH_PCB_COMP_NUM_SIZE;
            cell_num = data_num / SH_PCB_COMP_NUM_SIZE;
            fg_rd_succ = SH_get_pcb_comp_num(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_BMS_PARA_BGN && data_addr + data_num <= ADDR_SH_BMS_PARA_END) /* BMSͨѶ���ò��� */
        {
            addr_offst = data_addr - ADDR_SH_BMS_PARA_BGN;
            cell_offst = addr_offst / SH_BMS_PARA_SIZE;
            cell_num = data_num / SH_BMS_PARA_SIZE;
            fg_rd_succ = SH_get_bms_para_info(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_PARA_GRP_BGN && data_addr + data_num <= ADDR_SH_PARA_GRP_END) /* ��������Ϣ */
        {
            addr_offst = data_addr - ADDR_SH_PARA_GRP_BGN;
            cell_offst = addr_offst / SH_PARA_GRP_SIZE;
            cell_num = data_num / SH_PARA_GRP_SIZE;
            fg_rd_succ = SH_get_para_grp_info(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_PARA_STRING_BGN && data_addr + data_num <= ADDR_SH_PARA_STRING_END)/* ���������ַ��� */
        {
            addr_offst = data_addr - ADDR_SH_PARA_STRING_BGN;
            cell_offst = addr_offst / SH_PARA_STRING_SIZE;
            cell_num = data_num / SH_PARA_STRING_SIZE;
            fg_rd_succ = SH_get_dict_string(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_STR_ATTR_BGN && data_addr + data_num <= ADDR_SH_STR_ATTR_END) /* �����ַ������� */
        {
            addr_offst = data_addr - ADDR_SH_STR_ATTR_BGN;
            cell_offst = addr_offst / SH_STR_ATTR_PER_SIZE;
            cell_num = data_num / SH_STR_ATTR_PER_SIZE;
            fg_rd_succ = SH_get_dict_attrib(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_CHK_SYS_AV_BGN && data_addr + data_num <= ADDR_SH_CHK_SYS_AV_END) /* ��ѯ��Ϣ ϵͳ��Ԫģ���� */
        {
            addr_offst = data_addr - ADDR_SH_CHK_SYS_AV_BGN;
            cell_offst = addr_offst / SH_CHK_SYS_AV_SIZE;
            cell_num = data_num / SH_CHK_SYS_AV_SIZE;
            fg_rd_succ = SH_get_chk_info_av_sys(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_CHK_MOD_AV_BGN && data_addr + data_num <= ADDR_SH_CHK_MOD_AV_END) /* ��ѯ��Ϣ ϵͳ��Ԫģ���� */
        {
            addr_offst = data_addr - ADDR_SH_CHK_MOD_AV_BGN;
            cell_offst = addr_offst / SH_CHK_MOD_AV_STEP;
            cell_num = data_num / SH_CHK_SYS_AV_SIZE;
            fg_rd_succ = SH_get_chk_info_av_mod(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_CHK_COMP_AV_BGN && data_addr + data_num <= ADDR_SH_CHK_COMP_AV_END)/* ��ѯ��Ϣ ѹ��ģ���� */
        {
            addr_offst = data_addr - ADDR_SH_CHK_COMP_AV_BGN;
            cell_offst = addr_offst / SH_CHK_COMP_AV_SIZE;
            cell_num = data_num / SH_CHK_COMP_AV_SIZE;
            fg_rd_succ = SH_get_chk_info_av_comp(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_CHK_DO_BGN && data_addr + data_num <= ADDR_SH_CHK_DO_END) /* ��ѯ��Ϣ ������ */
        {
            addr_offst = data_addr - ADDR_SH_CHK_DO_BGN;
            cell_offst = addr_offst / SH_CHK_DO_SIZE;
            cell_num = data_num / SH_CHK_DO_SIZE;
            fg_rd_succ = SH_get_chk_info_do(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_CHK_DI_BGN && data_addr + data_num <= ADDR_SH_CHK_DI_END) /* ��ѯ��Ϣ ���������� */
        {
            addr_offst = data_addr - ADDR_SH_CHK_DI_BGN;
            cell_offst = addr_offst / SH_CHK_DI_SIZE;
            cell_num = data_num / SH_CHK_DI_SIZE;
            fg_rd_succ = SH_get_chk_info_di(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_CHK_USER_BGN && data_addr + data_num <= ADDR_SH_CHK_USER_END)
        {
            addr_offst = data_addr - ADDR_SH_CHK_USER_BGN;
            cell_offst = addr_offst / SH_CHK_USER_SIZE;
            cell_num = data_num / SH_CHK_USER_SIZE;
            fg_rd_succ = SH_get_chk_user_info(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_VFD_VER_BGN && data_addr + data_num <= ADDR_SH_VFD_VER_END)   /* ��Ƶ�汾�� */
        {
            addr_offst = data_addr - ADDR_SH_VFD_VER_BGN;
            cell_offst = addr_offst / SH_VFD_VER_SIZE;
            cell_num = data_num / SH_VFD_VER_SIZE;
            fg_rd_succ = SH_get_vfd_version(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_EXIO_VER_BGN && data_addr + data_num <= ADDR_SH_EXIO_VER_END)   /* �����汾�� */
        {
            addr_offst = data_addr - ADDR_SH_EXIO_VER_BGN;
            cell_offst = addr_offst / SH_EXIO_VER_SIZE;
            cell_num = data_num / SH_EXIO_VER_SIZE;
            fg_rd_succ = SH_get_exio_version(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_4G_INFO_BGN && data_addr + data_num <= ADDR_SH_4G_INFO_END)
        {
            addr_offst = data_addr - ADDR_SH_4G_INFO_BGN;
            cell_offst = addr_offst / SH_4G_INFO_SIZE;
            cell_num = data_num / SH_4G_INFO_SIZE;
            fg_rd_succ = SH_get_4G_info(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_MODE_SET_INFO_BGN && data_addr + data_num <= ADDR_SH_MODE_SET_INFO_END)   /* ģʽ��Ϣ */
        {
            addr_offst = data_addr - ADDR_SH_MODE_SET_INFO_BGN;
            cell_offst = addr_offst / SH_MODE_SET_INFO_SIZE;
            cell_num = data_num / SH_MODE_SET_INFO_SIZE;
            fg_rd_succ = SH_get_mode_set_info(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_MAIN_INFO_BGN && data_addr + data_num <= ADDR_SH_MAIN_INFO_END)   /* ��������Ϣ */
        {
            addr_offst = data_addr - ADDR_SH_MAIN_INFO_BGN;
            cell_offst = addr_offst / SH_MAIN_INFO_SIZE;
            cell_num = data_num / SH_MAIN_INFO_SIZE;
            fg_rd_succ = SH_get_main_info(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_INVTERR_BGN && data_addr + data_num <= ADDR_SH_INVTERR_END) /* ��ѯ��Ϣ ϵͳ��Ԫģ���� */
        {
            addr_offst = data_addr - ADDR_SH_INVTERR_BGN;
            cell_offst = addr_offst / SH_INVTERR_SIZE;
            cell_num = data_num / SH_INVTERR_SIZE;
            fg_rd_succ = SH_get_invt_err(&TxBuf[3], cell_offst, cell_num);
        }
		else if (data_addr >= ADDR_SH_FLOOR_PREHEAT_BGN && data_addr + data_num <= ADDR_SH_FLOOR_PREHEAT_END)
        {
            addr_offst = data_addr - ADDR_SH_FLOOR_PREHEAT_BGN;
            cell_offst = addr_offst / SH_FLOOR_PREHEAT_SIZE;
            cell_num = data_num / SH_FLOOR_PREHEAT_SIZE;
            fg_rd_succ = SH_get_floor_preheat_info(&TxBuf[3], cell_offst, cell_num);
        }
		else if (data_addr >= ADDR_SH_FLOOR_TIMER_BGN && data_addr + data_num <= ADDR_SH_FLOOR_TIMER_END)
        {
            addr_offst = data_addr - ADDR_SH_FLOOR_TIMER_BGN;
            cell_offst = addr_offst / SH_FLOOR_TIMER_SIZE;
            cell_num = data_num / SH_FLOOR_TIMER_SIZE;
            fg_rd_succ = SH_get_floor_timer_info(&TxBuf[3], cell_offst, cell_num);
        }
    }
    /* ��ȡ���� */
    if (!fg_rd_succ)
    {
        com_err = SH_MB_ILLEGAL_DATA_ADDR;
    }

    TxBuf[2] = (U08)(data_num << 1);        /* ���ݸ���Ϊ�����������ֽ��������2 */
    *pTxTotal = (U08)(TxBuf[2] + 2);        /* ���Ϲ������ֽڼ������ֽ� */

    return com_err;
}


/*******************************************************************************
 �� ��: SH_MB_rd_data()
 �� ��:
 �� ��: RxBuf--;
        TxBuf--;
        pTxTotal--;
 �� ��:
 �� ��: ��
*******************************************************************************/
U08 SH_MB_rd_data(const U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
    #define MB_FILE_EXSTR       1

    U08 com_err = SH_MB_NO_ERR;             /* ����״̬ */
    U16 file_num;
    U32 addr_ofs;
    U08 read_len;

    file_num = MAKEWORD(RxBuf[2], RxBuf[3]);/* �ļ��� */
    addr_ofs = MAKEDWORD(RxBuf[4], RxBuf[5], RxBuf[6], RxBuf[7]);
    read_len = RxBuf[8];

    switch (file_num)
    {
        case MB_FILE_EXSTR:
            EXSTR_COM_SlvReadStr(addr_ofs, read_len, &TxBuf[4],pTxTotal);
            break;
        default:
            com_err = SH_MB_ILLEGAL_DATA_ADDR;
            break;
    }

    /* 4 ���ļ��� */
    TxBuf[2] = RxBuf[2];
    TxBuf[3] = RxBuf[3];
    *pTxTotal += 2;

    /* 4 ������ */
    TxBuf[1] = RxBuf[1];
    *pTxTotal += 1;

    return com_err;
}

/****************************************************************
������SH_MB_wr_reg
���ܣ���������HMI�������������֡�
������RxBuf--���ջ�����;
      TxBuf--���ͻ�����;
      pTxTotal--�跢�͵��ֽ�����;
���أ��ɹ�/ʧ��
������
*****************************************************************/
U08 SH_MB_wr_reg(const U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
    U16 addr_offst = 0;     /* ������ʼλ��(�����ݵ�ַ����) */
    U16 data_num = 0;       /* ���ݸ��� */
    U16 data_addr = 0;
    U08 *src_buf = PNULL;
    U08 com_err = SH_MB_NO_ERR;
    BOOL fg_wr_succ = FALSE;/* �����ⲿ�ӳ���д��״̬��־ */
    U16 cell_offst = 0;     /* ��Ԫ��ʼλ��(�ӵڼ����ṹ�嵥Ԫ��ʼ��) */
    U16 cell_num = 0;       /* ��Ԫ����(��ȡ�����ṹ�嵥Ԫ) */

    data_addr = MAKEWORD(RxBuf[2], RxBuf[3]);               /* ���ݵ�ַ */

    if (RxBuf[1] == 0x06)   /* ������0x06: д������ */
    {
        data_num = 1;
        src_buf = (U08*)&RxBuf[4];
    }
    else                    /* ������0x10: д����� */
    {
        data_num = MAKEWORD(RxBuf[4], RxBuf[5]);
        src_buf = (U08*)&RxBuf[7];
    }

    /* �¾�ע��ֻΪ�������޷��������ڵ���0�ı��뾯�� */
    if (/*data_addr >= ADDR_CMD_BGN && */data_addr + data_num <= ADDR_SH_CMD_END)//����
    {
        addr_offst = data_addr - ADDR_SH_CMD_BGN;
        cell_offst = addr_offst;
        cell_num = data_num;
        fg_wr_succ = SH_set_command(src_buf, cell_offst, cell_num);
    }
    else if (data_addr >= ADDR_SH_SET_BGN && data_addr + data_num <= ADDR_SH_SET_END)   /* ���� */
    {
        addr_offst = data_addr - ADDR_SH_SET_BGN;
        cell_offst = addr_offst;
        cell_num = data_num;
        fg_wr_succ = SH_set_setting(src_buf, cell_offst, cell_num);
    }
    else if (data_addr >= ADDR_SH_PASS_BGN && data_addr + data_num <= ADDR_SH_PASS_END) /* ���� */
    {
        addr_offst = data_addr - ADDR_SH_PASS_BGN;
        cell_offst = addr_offst / SH_PASS_SIZE;
        cell_num = data_num / SH_PASS_SIZE;
        fg_wr_succ = SH_set_passwrd(src_buf, cell_offst, cell_num);
    }
    else if (data_addr >= ADDR_SH_CLOCK_BGN && data_addr + data_num <= ADDR_SH_CLOCK_END)   /* ʱ�� */
    {
        addr_offst = data_addr - ADDR_SH_CLOCK_BGN;
        cell_offst = addr_offst / SH_CLOCK_SIZE;
        cell_num = data_num / SH_CLOCK_SIZE;
        fg_wr_succ = SH_set_clock(src_buf, cell_offst, cell_num);
    }
    else if (data_addr >= ADDR_SH_HOLIDAYINFO_BGN && data_addr + data_num <= ADDR_SH_HOLIDAYINFO_END)   /* ����ģʽ */
    {
        fg_wr_succ = SH_set_holidayinfo(src_buf, data_num);
    }
    else if (data_addr >= ADDR_ROOM_TEMP_BGN && data_addr + data_num <= ADDR_SH_ROOM_TEMP_END)   /* �����¶� */
    {
        fg_wr_succ = SH_set_room_temp(src_buf, data_num);
    }
    else if (data_addr >= ADDR_SH_PARA_BGN && data_addr + data_num <= ADDR_SH_PARA_END) /* ����ֵ */
    {
        addr_offst = data_addr - ADDR_SH_PARA_BGN;
        cell_offst = addr_offst;
        cell_num = data_num;
        fg_wr_succ = SH_set_para(src_buf, cell_offst, cell_num);
    }
    else if (data_addr >= ADDR_SH_CHK_TEST_WO_BGN && data_addr + data_num <= ADDR_SH_CHK_TEST_WO_END)
    {
        addr_offst = data_addr - ADDR_SH_CHK_TEST_WO_BGN;
        cell_offst = addr_offst;
        cell_num = data_num;
        fg_wr_succ = SH_set_test_wo(src_buf, cell_offst, cell_num);
    }
    else if (data_addr >= ADDR_SH_CHK_TEST_WR_BGN && data_addr + data_num <= ADDR_SH_CHK_TEST_WR_END)
    {
        addr_offst = data_addr - ADDR_SH_CHK_TEST_WR_BGN;
        cell_offst = addr_offst;
        cell_num = data_num;
        fg_wr_succ = SH_set_test_wr(src_buf, cell_offst, cell_num);
    }
    else if (data_addr >= ADDR_SH_PASS_BIG_ENDIAN_BGN && data_addr + data_num <= ADDR_SH_PASS_BIG_ENDIAN_END)
    {
        fg_wr_succ = SH_set_pass_big_endian();
    }
    else if (data_addr >= ADDR_SH_TUYA_DATA_BGN && data_addr + data_num <= ADDR_SH_TUYA_DATA_END)
    {
        addr_offst = data_addr - ADDR_SH_TUYA_DATA_BGN;
        fg_wr_succ = SHC_set_tuya_data(src_buf);
    }
    else if (data_addr >= ADDR_SH_INVTPARA_BGN && data_addr + data_num <= ADDR_SH_INVTPARA_END)
    {
        fg_wr_succ = SH_set_invt_para(src_buf, data_num);
    }
    else if (data_addr >= ADDR_SH_FLOOR_PREHEAT_VAL_BGN && data_addr + data_num <= ADDR_SH_FLOOR_PREHEAT_VAL_END)
    {
        addr_offst = data_addr - ADDR_SH_FLOOR_PREHEAT_VAL_BGN;
        cell_offst = addr_offst;
        cell_num = data_num;
        fg_wr_succ = SH_set_floor_preheat_val(src_buf, cell_offst, cell_num);
    }
    else if (data_addr >= ADDR_SH_FLOOR_TIMER_VAL_BGN && data_addr + data_num <= ADDR_SH_FLOOR_TIMER_VAL_END)
    {
        addr_offst = data_addr - ADDR_SH_FLOOR_TIMER_VAL_BGN;
        cell_offst = addr_offst;
        cell_num = data_num;
        fg_wr_succ = SH_set_floor_timer_val(src_buf, cell_offst, cell_num);
    }
    else if (data_addr >= ADDR_SH_TEST_FAC_HMI_WR_BGN && data_addr + data_num <= ADDR_SH_TEST_FAC_HMI_WR_END)
    {
        fg_wr_succ = SH_set_test_fac_hmi_info(src_buf, data_num);
    }

    /* д����� */
    if(!fg_wr_succ)
    {
        com_err = SH_MB_ILLEGAL_DATA_ADDR;
    }

    TxBuf[2] = RxBuf[2];
    TxBuf[3] = RxBuf[3];
    TxBuf[4] = RxBuf[4];
    TxBuf[5] = RxBuf[5];
    *pTxTotal = 5;

    return com_err;
}

/*******************************************************************************
 �� ��: SH_MB_diag()
 �� ��: �ӻ�����ModBus��·��ϲ�������
 �� ��: RxBuf--���ջ�����;
        TxBuf--���ͻ�����;
        pTxTotal--�跢�͵��ֽ�����;
 �� ��: �쳣��־
 �� ��:
*******************************************************************************/
static U08 SH_MB_diag(const U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
    U08 i;
    U16 subFunctionCode;    /* �ӹ����� */
    U16 data_num;           /* ���ݸ��� */
    U08 bytes;              /* �����ֽ��� */

    subFunctionCode = MAKEWORD(RxBuf[2], RxBuf[3]);

    if (subFunctionCode == 0)
    {
        /* �����ж� */
        data_num = MAKEWORD(RxBuf[4], RxBuf[5]);
        bytes = RxBuf[6];
        if ((data_num<1) || (data_num>100)
            || (bytes != 2*data_num))
        {
            return SH_MB_ILLEGAL_DATA_VALUE;
        }

        /* ������ */
        for (i=1; i<bytes+7; i++)
        {
            TxBuf[i] = RxBuf[i];
        }
        *pTxTotal = (U08)(bytes+6);

        return SH_MB_NO_ERR;
    }
    else
    {
        return SH_MB_ILLEGAL_FUNCTION;
    }
}

/*******************************************************************************
 �� ��: SH_MB_match_rx()
 �� ��: �ӻ�ƥ���������
 �� ��: pCOM--���ڽṹ��ָ��;
 �� ��: TRUE/FALSE--ƥ��ɹ�/ƥ��ʧ��
 �� ��:
*******************************************************************************/
static BOOL SH_MB_match_rx(UART *pCOM)
{
    #define LEAST_FRAME_BYTES   4           /* ���ٵ�֡�ֽ��� */
    #define SH_SLAVER_ADDR      0x01        /* �ӻ���ַ */

    U16 idxBegin;           /* ��������ƥ��Ŀ�ʼ������ */
    U16 idxEnd;             /* ��������ƥ��Ľ��������� */
    BOOL matchOk;           /* ƥ��OK��ʶ */
    U16 matchOkIdx=0;       /* ƥ��OKʱ��������(ע������ֵ0��ֹ���뾯��) */
    U16 theoreticBytes=0;   /* ���ݹ����룬��������յ��ֽ��� */

    /* ���յ��������ֽ���С�����ٵ�֡�ֽ���������ȷ���Ǹ��� */
    if (pCOM->rxCnt < LEAST_FRAME_BYTES)
    {
        /* ͨѶ��¼:֡���ݹ���++ */
        return FALSE;
    }
    /* ��ͷ���ο�ʼѰ�ҽ��������Ƿ�ƥ�䣬����Ҫ���������н������ݣ� */
    /* �� ʣ�����ݸ��� С�� ����֡�ֽ��� ʱ���Ͳ���Ҫ�����������ˡ� */
    matchOk = FALSE;
    idxEnd = pCOM->rxCnt - LEAST_FRAME_BYTES;
    for (idxBegin=0; idxBegin<=idxEnd; idxBegin++)
    {
        U08 rxAddr;         /* ���յĵ�ַ */
        U08 rxFunCode;      /* ���յĹ����� */
        BOOL funCodeMatch;  /* ������ƥ��� */

        /* ��ַ��ƥ�䣬�ͼ�����һ������ */
        rxAddr = pCOM->rxBuf[idxBegin];
        if ((rxAddr!=SH_SLAVER_ADDR) && (rxAddr!=0))
        {
            continue;
        }

        /* ���ҹ����������۽��ո��� */
        rxFunCode = pCOM->rxBuf[idxBegin+1];
        switch (rxFunCode)
        {
            case 0x01 :     /* 0x01 ���ɶ�дλ */
            case 0x02 :     /* 0x02 ��ֻ��λ */
            case 0x03 :     /* 0x03 ���ɶ�д�� */
            case 0x04 :     /* 0x04 ��ֻ���� */
            case 0x05 :     /* 0x05 д����λ */
            case 0x06 :     /* 0x06 д������ */
                funCodeMatch = TRUE;
                theoreticBytes = 8;
                break;

            case 0x08 :     /* 0x08 ��·��� */
            case 0x0f :     /* 0x0f д���λ */
            case 0x10 :     /* 0x10 д����� */
            case 0x41 :     /* 0x41 �Զ��壺�������� */
                funCodeMatch = TRUE;
                theoreticBytes = 9+pCOM->rxBuf[idxBegin+6];
                break;
            case 0x44:
                funCodeMatch = TRUE;
                theoreticBytes = 11;
                break;
            default:
                funCodeMatch = FALSE;
                break;
        }
        /* ����������ո�����ƥ�䣬�ͼ�����һ������ */
        if ((!funCodeMatch) || (pCOM->rxCnt < (idxBegin+theoreticBytes)))
        {
            continue;
        }
        /* crcУ��ͨ����ƥ��ɹ��˳����ң����������һ������ */
        if (fun_crc16_ok(&pCOM->rxBuf[idxBegin], (U08)theoreticBytes))
        {
            matchOk = TRUE;
            matchOkIdx = idxBegin;
            break;
        }
    }

    if (matchOk)
    {
        /* ƥ������ǰ������������ */
        if (matchOkIdx > 0)
        {
            U16 i;
            for (i=0; i<theoreticBytes; i++)
            {
                pCOM->rxBuf[i] = pCOM->rxBuf[matchOkIdx+i];
            }
        }
        /* ��¼��ȷ���ݴ��� */
        if (pCOM->rxCnt == theoreticBytes)
        {
            /* ͨѶ��¼:��ȫ��ȷ++ */
        }
        else
        {
            if (matchOkIdx == 0)
            {
                /* ͨѶ��¼:�����++ */
            }
            else
            {
                /* ͨѶ��¼:ǰ����++ */
            }
        }

        return TRUE;
    }
    else
    {
        /* δ�ҵ�ƥ������ */
        /* ͨѶ��¼:����У���++ */
        return FALSE;
    }
}

/*******************************************************************************
 �� ��: SH_MB_deal_rece()
 �� ��: Ӧ��HMI�Ķ�/д����
 �� ��: RxBuf--���ջ�����;
        TxBuf--���ͻ�����;
        pTxTotal--�跢�͵��ֽ�����;
 �� ��: �쳣��־
 �� ��: HD009A����ר��
 �� ֲ: ���ݵ�ǰ����ƽ̨�����޸ģ�����Ӧ��ǰ��UART���ݽṹ��ͨѶ��غ궨��ȡ�
*******************************************************************************/
U08 SH_MB_deal_rece(const U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
    U08 exceptCode = SH_MB_NO_ERR;
    W16 crc;
    U08 TxTotal = 0;

    /* ��������� */
    switch (RxBuf[1])
    {
        case MB_RD_HOLDING_REGISTERS :      /* 0x03 ���ɶ�д�� */
        case MB_RD_INPUT_REGISTERS :        /* 0x04 ��ֻ���� */
            exceptCode = SH_MB_rd_reg(RxBuf, TxBuf, pTxTotal);
            break;

        case MB_WR_SINGLE_REGISTER :        /* 0x06 д������ */
        case MB_WR_MULTIPLE_REGISTERS :     /* 0x10 д����� */
            exceptCode = SH_MB_wr_reg(RxBuf, TxBuf, pTxTotal);
            break;

        case MB_DIAGNOSTICS :               /* 0x08 ��·��� */
            exceptCode = SH_MB_diag(RxBuf, TxBuf, pTxTotal);
            break;
        case MB_USER_DEFINED_RD_BIGFILE :
            exceptCode = SH_MB_rd_data(RxBuf, TxBuf, pTxTotal);
            break;
        case MB_USER_DEFINED_DOWNLOAD :     /* 0x41 �Զ��壺�������� */
            exceptCode = PDU_SlaveDealUserDef(RxBuf, TxBuf, pTxTotal);
            break;

        /* ���²���δ�õ��Ĺ������ݶ�Ϊ�Ƿ������� */
        case MB_RD_COILS:                   /* 0x01 ���ɶ�дλ */
        case MB_RD_DISCRETE_INPUTS:         /* 0x02 ��ֻ��λ */
        case MB_WR_SINGLE_COIL:             /* 0x05 д����λ */
        case MB_WR_MULTIPLE_COILS:          /* 0x0f д���λ */
        default :                           /* ���������������ݲ�֧�֣�Ҳ��Ϊ�Ƿ������� */
            exceptCode = SH_MB_ILLEGAL_FUNCTION;
            break;
    }

    /* �����쳣��ӦPDU */
    if (exceptCode != SH_MB_NO_ERR)
    {
        TxBuf[1] = (U08)(RxBuf[1] | 0x80);
        TxBuf[2] = exceptCode;
        *pTxTotal = 2;
    }
    else
    {
        TxBuf[1] = RxBuf[1];
    }

    /* ���Ӹ�����:֡ͷ�ĵ�ַ��֡β��CRCУ�� */
    TxTotal = *pTxTotal;
    TxBuf[0] = RxBuf[0];
    TxTotal += 1;
    crc.word = fun_crc16(&TxBuf[0], TxTotal);
    TxBuf[TxTotal++] = crc.byte.low;
    TxBuf[TxTotal++] = crc.byte.high;
    *pTxTotal = TxTotal;

    return exceptCode;
}

/*******************************************************************************
 �� ��: SH_MB_DealUartSlave()
 �� ��: �ӻ�ͨѶ����
 �� ��: ���ں�
 �� ��: ��
 �� ��:
 �� ֲ: ���ݵ�ǰ����ƽ̨�����޸ģ�����Ӧ��ǰ��UART���ݽṹ��ͨѶģ�͡��궨�塢ͨѶ��ʱ�ȡ�
*******************************************************************************/
void SH_MB_DealUartSlave(U08 u)
{
    UART *pCOM;

    pCOM = &Uart[u];

    switch (pCOM->step)
    {
        case COMMU_RX :     /* �յ���ȷ֡�ҷǹ㲥ʱ��תΪ������ʱ������һֱ���� */
            /* һ֡�������:����֡ʱ����δ�յ���һ���ݻ���ջ������� */
            if ((pCOM->rxCnt>0 && pCOM->time.rxFrame==0)
                || (pCOM->rxCnt >= COMMU_RX_SIZE))
            {
                COM_DisableRece(u);         /* �������֡ʱ�رս���ʹ�� */
                if (SH_MB_match_rx(pCOM))
                {
                    SH_MB_deal_rece(&pCOM->rxBuf[0], &pCOM->txBuf[0], &pCOM->txTotal);

                    _CNT(SH_hmi_commu_succ);/* ע��:ͨѶ�ɹ�����Ҫ�ۼӳɹ���������ᱻ��Ϊ��ͨѶʧ�ܶ��л������� */

                    pCOM->time.led = COMMU_TIME_LED;
                    pCOM->time.commIntProt = COMMU_TIME_PROTECT;
                    pCOM->commuErr = COMMU_NO_ERR;          /* �Զ���λ */

                    if (pCOM->rxBuf[0] != ADDR_BROADCAST)   /* ���ǹ㲥, �Ż������� */
                    {
                        pCOM->step = COMMU_DELAY;
                        pCOM->time.txDly = COMMU_TIME_TX_DLY;
                    }
                    else    /* �㲥ʱ��׼��������һ֡ */
                    {
                        COM_ReadyRece(u);
                    }
                }
                else        /* ����֡ʱ��׼��������һ֡ */
                {
                    COM_ReadyRece(u);
                }

                pCOM->rxCnt = 0;            /* ����һ֡���ݺ�����ռ����� */
            }

            /* ͨѶ�жϱ���ʱ�䵽����δ�յ���Ч����֡����ͨѶ�жϱ������� */
            if (pCOM->time.commIntProt == 0)
            {
                pCOM->commuErr = COMMU_INT_PROT;
            }
            break;
        case COMMU_DELAY :
            COM_DisableRece(u);
            COM_Set485Tras(u);
            if (pCOM->time.txDly == 0)
            {
                pCOM->step = COMMU_START;
            }
            else
            {
                break;
            }
            break;
        case COMMU_START :
            pCOM->txCnt = 0;
            pCOM->rxCnt = 0;
            COM_WrTrasBufReg(u, pCOM->txBuf[0]);
            pCOM->step = COMMU_TX;
            pCOM->time.txDly = COMMU_TIME_TX_TIMEOUT;
            break;

        case COMMU_TX :     /* ת�������жϸ�Ϊ���� */
            if (pCOM->txCnt >= pCOM->txTotal)
            {
                pCOM->time.commIntProt = COMMU_TIME_PROTECT;/* �ӻ��� */
                pCOM->step = COMMU_RX;
                /* �ڷ����ж����Ѿ���Ϊ���գ��ڸ���ͨ��ʱ�� */
                /* ����ִ�д˴�ǰ�Ѿ����������ݣ���˴˴ο��Բ����� */
                COM_ReadyRece(u);           /* ��������ȷ��485�շ��������óɹ� */
            }
            else if (pCOM->time.txDly == 0) /* ��ʱ */
            {
                pCOM->commuErr = COMMU_OVERTIME;
                pCOM->step = COMMU_FAIL;
                COM_ReadyRece(u);
            }
            break;

        default :
            pCOM->step = COMMU_RX;
            COM_ReadyRece(u);
            pCOM->rxCnt = 0;
            break;
    }
}

/*******************************************************************************
 �� ��: SH_MB_commu_adjust()
 �� ��: ͨѶ����Ӧ:�����ı�����4.3�����
 �� ��: ��
 �� ��: ��
 �� ��: ͨѶʧ�ܳ���һ��ʱ���:ѭ���л�����ģʽ
 �� ֲ: ʹ�ñ������滻ԭ���ĵ���ͨѶ�����������ڱ������е��øú�����
        ������Ҫ�������л�ʱ�ʵ���Ӵ��ڳ�ʼ����
*******************************************************************************/
void SH_MB_commu_adjust(void)
{
    enum 
    {
        HMI_IS_M, 
        HMI_IS_S,  
#if defined(M_NIUENTAI) || defined(M_DEBUG)
        HMI_IS_NET,			/* Ŧ��̩����������BMSЭ�鿪�� */
#endif
        HMI_NUM_MAX
    };
    #define SH_TYPE_CHG_INTVL       6       /* �����л����(500ms) */

    static U08 cnt_commu_fail = 0;          /* ͨѶʧ�ܼ�ʱ */
    static U08 hmi_type = 0;                /* ǰ������ */
    static U08 flash_500_bak = 0;           /* 500ms״̬���� */

    /* ͨѶ���� */
    switch (hmi_type)
    {
        default:
        case HMI_IS_S:
            /* ��Ϊ��������(DM603)ͨ�� */
            COM_HMIMasterProg(COM_TEXT_HMI);    /* ע��:ͨѶ�ɹ�����Ҫ SH_hmi_commu_succ++; */
            break;
        case HMI_IS_M:
            /* ��Ϊ�ӻ�����ͨ�� */
            SH_MB_DealUartSlave(COM_TEXT_HMI);
            break;  
#if defined(M_NIUENTAI) || defined(M_DEBUG)
        case HMI_IS_NET:
            /* ��Ϊ�ӻ���Ŧ��̩����ͨ�� */
            COM_SlaverProg(COM_TEXT_HMI); 
            break; 
#endif             
    }

    /* ʧ�ܼ���(ÿ500msִ��һ��) */
    /* ���ڱ�������һ��ѭ�����п��ܱ����ö�Σ����Բ�����F_TIME_BASIC_500MS_BACK����ʱ */
    if (flash_500_bak != flash_500ms)
    {
        flash_500_bak = flash_500ms;

        if (SH_hmi_commu_succ == 0)
        {
            _CNT(cnt_commu_fail);
        }
        else
        {
            cnt_commu_fail = 0;
            SH_hmi_commu_succ = 0;
        }
    }
    /* ͨѶʧ�ܳ�ʱ:�л��� */
    if (cnt_commu_fail >= SH_TYPE_CHG_INTVL)
    {
        cnt_commu_fail = 0;
        if (++hmi_type >= HMI_NUM_MAX)
        {
            hmi_type = 0;
        }

        switch (hmi_type)
        {
            default:
            case HMI_IS_S:
                COM_Config(COM_TEXT_HMI, B_19200, T_8N1);  
                COM_InitMaster(COM_TEXT_HMI, BKBD_INVT_UART3);          /* ��ʾ�� */
                break;
            case HMI_IS_M:
                COM_Config(COM_TEXT_HMI, B_19200, T_8N1);  
                COM_InitSlaver(COM_TEXT_HMI, BKBD_INVT_UART3, 1);       /* 7���� */
                break;       
#if defined(M_NIUENTAI) || defined(M_DEBUG)
            case HMI_IS_NET:
                COM_Config(COM_TEXT_HMI, B_9600, T_8N1);                  
                COM_InitSlaver(COM_TEXT_HMI, BKBD_INVT_UART3, 1);	    /* AD007 */ 
                break;     
#endif                
        }
    }
}

/*****************************************************************************
��  �� : SH_timer()
��  �� : ��ʱ
��  �� : ��
��  �� : ��
��  �� : ��ŵ��ⲿִ��
*****************************************************************************/
void SH_timer(void)
{
    SHC_timer();
}

void SH_init(void)
{
    SHC_init();
}

