/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: Tuya_B_Config.c
 �ļ�����: Ϳѻ���߹�������(��岿��)
 ������  : �ֺƱ�
 ��������: 2020.12.13
*******************************************************************************/
#define TUYA_B_CONFIG_GLOBAL
#include "includes.h"

const RN_ENUM_MAP TYBC_LANGUAGE_MAP[]=
{
    { SH_LANGUAGE_EN ,  RN_LANGUAGE_EN,  TY_MSGID_LANG_EN  },		/* Ӣ�� */
    { SH_LANGUAGE_PL ,  RN_LANGUAGE_POL, TY_MSGID_LANG_POL },		/* ������ */
    { SH_LANGUAGE_ES ,  RN_LANGUAGE_SP , TY_MSGID_LANG_ESP },		/* �������� */
    { SH_LANGUAGE_GR ,  RN_LANGUAGE_GR , TY_MSGID_LANG_GR  },		/* ϣ���� */
    { SH_LANGUAGE_FR ,  RN_LANGUAGE_FR , TY_MSGID_LANG_FR  },		/* ���� */
    { SH_LANGUAGE_IT ,  RN_LANGUAGE_ITL, TY_MSGID_LANG_ITL },		/* ������� */
    { SH_LANGUAGE_CN ,  RN_LANGUAGE_CN,  TY_MSGID_LANG_CHN },		/* ���� */
    { SH_LANGUAGE_HG,   RN_LANGUAGE_HG,  TY_MSGID_LANG_HG  },		/* �������� */
    { SH_LANGUAGE_GE ,  RN_LANGUAGE_GE,  TY_MSGID_LANG_GE  },		/* ���� */
    { SH_LANGUAGE_CZ ,  RN_LANGUAGE_CZ,  TY_MSGID_NULL 	   },		/* �ݿ� */
    { SH_LANGUAGE_PT ,  RN_LANGUAGE_PT,  TY_MSGID_NULL     },		/* ������ */
    { SH_LANGUAGE_TR ,  RN_LANGUAGE_TR,  TY_MSGID_NULL     },		/* ������ */
    { SH_LANGUAGE_NL ,  RN_LANGUAGE_NL,  TY_MSGID_NULL     },		/* ���� */
    { SH_LANGUAGE_SE ,  RN_LANGUAGE_SE,  TY_MSGID_NULL     },		/* ��� */
    { SH_LANGUAGE_DK ,  RN_LANGUAGE_DK,  TY_MSGID_NULL     },		/* ���� */
    { SH_LANGUAGE_NO ,  RN_LANGUAGE_NO,  TY_MSGID_NULL     },		/* Ų�� */
};

const RN_ENUM_MAP TYBC_MODE_SET_MAP[] =
{
    { SET_AUTO,           RN_MODE_AUTO,      TY_MSGID_MODE_AUTO,      }, /* 0: RN_MODE_AUTO �Զ� */
    { SET_COOL,           RN_MODE_COOL,      TY_MSGID_MODE_COOL,      }, /* 1: RN_MODE_COOL ���� */
    { SET_HEAT,           RN_MODE_HEAT,      TY_MSGID_MODE_HEAT,      }, /* 2: RN_MODE_HEAT ���� */
};

const RN_ENUM_MAP TYBC_RUN_STATUS_MAP[] =
{
    { _IDLE,   RN_RUN_STATUS_IDLE,    TY_MSGID_STA_READY,   }, /* 0: RN_RUN_STATUS_IDLE ׼������ */
    { _READY,  RN_RUN_STATUS_LAUNCH,  TY_MSGID_STA_ON,      }, /* 1: RN_RUN_STATUS_LAUNCH �������� */
    { _RUN,    RN_RUN_STATUS_RUN,     TY_MSGID_STA_RUNNING, }, /* 2: RN_RUN_STATUS_RUN ������ */
    { _DELAY,  RN_RUN_STATUS_STOP,    TY_MSGID_STA_OFF,     }, /* 3: RN_RUN_STATUS_STOP ����ֹͣ */
    { _ALARM,  RN_RUN_STATUS_ALERT,   TY_MSGID_STA_ALERT,   }, /* 4: RN_RUN_STATUS_ALERT ���ϸ澯 */
};

/*******************************************************************************
 �� ��: TYBC_language_to_rn_enum()
 �� ��: ����������ö��ֵ����ת��ΪRN�������ö��ֵ
 �� ��: local_language--��������ö��ֵ;
 �� ��: �������ö��ֵ
 �� ��: ��
*******************************************************************************/
U08 TYBC_language_to_rn_enum(U08 local_language)
{
    U08 i;

    for (i = 0; i < ARRELE(TYBC_LANGUAGE_MAP); i++)
    {
        if (local_language == TYBC_LANGUAGE_MAP[i].local_enum)
        {
            return TYBC_LANGUAGE_MAP[i].rn_enum;
        }
    }
    return RN_LANGUAGE_CN;
}

/*******************************************************************************
 �� ��: TYBC_language_to_local_enum()
 �� ��: ����������ö��ֵ����ת��ΪRN�������ö��ֵ
 �� ��: rn_language--��������ö��ֵ;
 �� ��: �������ö��ֵ
 �� ��: ��
*******************************************************************************/
U08 TYBC_language_to_local_enum(U08 rn_language)
{
    U08 i;

    for (i = 0; i < ARRELE(TYBC_LANGUAGE_MAP); i++)
    {
        if (rn_language == TYBC_LANGUAGE_MAP[i].rn_enum)
        {
            return TYBC_LANGUAGE_MAP[i].local_enum;
        }
    }
    return SH_LANGUAGE_CN;
}

/* ��"��������" */
#ifdef TUYA_USE_LANG_SET
/*******************************************************************************
 �� ��: TYBC_get_language_msg()
 �� ��: ͨ����������ö��ֵ��ȡ��Ӧ����ʾ�ַ���
 �� ��: local_language--;
 �� ��: ���������ʾ�ַ�����
 �� ��: ��
*******************************************************************************/
static U16 TYBC_get_language_msg(U08 local_language)
{
    U08 i;

    for (i = 0; i < ARRELE(TYBC_LANGUAGE_MAP); i++)
    {
        if (local_language == TYBC_LANGUAGE_MAP[i].local_enum)
        {
            return TYBC_LANGUAGE_MAP[i].msg_id;
        }
    }
    return TY_MSGID_NULL;
}
#endif

/* ��"��������" */
#ifdef TUYA_USE_FAC_PWD
/*******************************************************************************
 �� ��: TYBC_get_password_pointer()
 �� ��: �˴�����������ͳһת��ΪU08*, ���������С�����⡣�����ʵ������Դ˺���
        �����޸�!!!
 �� ��: ��
 �� ��: pwd--��������ָ��(U08*)
 �� ��: ��
*******************************************************************************/
U08 *TYBC_get_password_pointer(void)
{
    U08 i;
    static U08 pwd[MAX_PASS_BIT];

    ARRCLR(pwd);

    for (i = 0; i < MAX_PASS_WORD; i++)
    {
        pwd[i] = password[PASS_FAC][i];
    }

    return pwd;
}

/*******************************************************************************
 �� ��: TYBC_check_password_modified()
 �� ��: ��鳧�������Ƿ��޸ġ������ʵ������Դ˺��������޸�! ע�ⲻͬƽ̨��Ҫ��
        ��ʵ�ʵ�passwordά�Ⱥʹ�С�����ⳬ��
 �� ��: ��
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL TYBC_check_password_modified(void)
{
    U08 i;
    BOOL fg_pwd_updated = FALSE;
    static U08 pwd_prev[MAX_PASS_BIT] = {0};

    for (i = 0; i < MAX_PASS_BIT; i++)
    {
        if (pwd_prev[i] != TYBC_PASSWORD_POINTER[i])
        {
            fg_pwd_updated = TRUE;
        }
    }

    if (fg_pwd_updated)
    {
        MEMCPY(pwd_prev, TYBC_PASSWORD_POINTER, MAX_PASS_BIT);
    }

    return fg_pwd_updated;
}
#endif

/* ��"���Թ���" */
#ifdef TUYA_USE_LANG_SET
/*******************************************************************************
 �� ��: TYBC_get_language_range()
 �� ��: ��ȡ����ѡ���ַ���������ָ��
 �� ��: ��
 �� ��: �ַ���������ָ��
 �� ��: ��
*******************************************************************************/
U16 *TYBC_get_language_range(void)
{
    U08 i, rn_enum;
    U16 rn_msg;
    static U16 MSG_RN_LANGAUGE_U[RN_LANGUAGE_MAX];

    /* ��־������ģʽ��ʼ�� */
    ARRSET(MSG_RN_LANGAUGE_U, TY_MSGID_NULL);

    /* �������Կ�ѡ�� */
    for (i = 0; i < RN_LANGUAGE_MAX; i++)
    {
        rn_enum = TYBC_language_to_rn_enum(i);
        rn_msg = TYBC_get_language_msg(i);
        if (rn_msg != TY_MSGID_NULL)
        {
            MSG_RN_LANGAUGE_U[rn_enum] = rn_msg;
        }
    }

    return MSG_RN_LANGAUGE_U;
}

/*******************************************************************************
 �� ��: TYBC_language_set()
 �� ��: ���ÿ��������Բ�д��EEPROM
 �� ��: val--�����趨ֵ;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void TYBC_language_set(U08 val)
{
    if (val > 0
    && val <= RN_LANGUAGE_MAX
    )
    {
        sh_language = TYBC_language_to_local_enum(val);
        DataAccess_Language(DATA_WRITE);
    }
}
#endif

#ifdef TUYA_USE_TEMP_UNIT_SET
/*******************************************************************************
 �� ��: TYBC_get_temp_unit_range()
 �� ��:
 �� ��: ��
 �� ��:
 �� ��: ��
*******************************************************************************/
U16 *TYBC_get_temp_unit_range(void)
{
	static U16 MSG_RN_TEMP_UNIT_U[RN_TEMP_UNIT_MAX];

	MSG_RN_TEMP_UNIT_U[RN_TEMP_UNIT_C] = TY_MSGID_TEMP_UNIT_C;
	MSG_RN_TEMP_UNIT_U[RN_TEMP_UNIT_F] = TY_MSGID_TEMP_UNIT_F;

    return MSG_RN_TEMP_UNIT_U;
}
#endif

/*******************************************************************************
 �� ��: TYBC_get_switch_status()
 �� ��: ��ȡ����ʵ�ʿ���/�ر�״̬
 �� ��: ��
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL TYBC_get_switch_status(void)
{
    BOOL fg_switch_on = FALSE;

    if (status_run == _READY
        || status_run == _RUN)
    {
        fg_switch_on = TRUE;
    }

    return fg_switch_on;
}

/*******************************************************************************
 �� ��: TYBC_mode_set_to_rn_enum()
 �� ��: �������趨ģʽö��ֵ����ת��ΪRN����趨ģʽö��ֵ
 �� ��: local_mode--����ģʽö��ֵ;
 �� ��: ���ģʽö��ֵ
 �� ��: ��
*******************************************************************************/
U08 TYBC_mode_set_to_rn_enum(U08 local_mode)
{
    U08 i;

    for (i = 0; i < ARRELE(TYBC_MODE_SET_MAP); i++)
    {
        if (local_mode == TYBC_MODE_SET_MAP[i].local_enum)
        {
            return TYBC_MODE_SET_MAP[i].rn_enum;
        }
    }
    return RN_MODE_AUTO;
}

/*******************************************************************************
 �� ��: TYBC_get_mode_set_msg()
 �� ��: ͨ�������趨ģʽö��ֵ��ȡ��Ӧ����ʾ�ַ���
 �� ��: local_mode--����ģʽö��ֵ;
 �� ��: ���ģʽ��ʾ�ַ�����
 �� ��: ��
*******************************************************************************/
U16 TYBC_get_mode_set_msg(U08 local_mode)
{
    U08 i;

    for (i = 0; i < ARRELE(TYBC_MODE_SET_MAP); i++)
    {
        if (local_mode == TYBC_MODE_SET_MAP[i].local_enum)
        {
            return TYBC_MODE_SET_MAP[i].msg_id;
        }
    }
    return msg_tyb_null;
}

/*******************************************************************************
 �� ��: TYBC_get_mode_set_range()
 �� ��: ��ȡ�趨ģʽѡ���ַ���������ָ��
 �� ��: ��
 �� ��: �ַ���������ָ��
 �� ��: ��
*******************************************************************************/
U16 *TYBC_get_mode_set_range(void)
{
    U08 i, rn_enum;
    U16 rn_msg;
    I16 max, min;
    static U16 MSG_RN_MODE_SET_U[RN_MODE_MAX];

    /* ��־������ģʽ��ʼ�� */
    ARRSET(MSG_RN_MODE_SET_U, msg_tyb_null);

    /* ��ȡ���ֵ����Сֵ */
    max = para_get_max(P_GaI(sv_mode));
    min = para_get_min(P_GaI(sv_mode));

    /* ��������ģʽѡ�� */
    for (i = 0; i < SET_MODE_MAX; i++)
    {
        if (i >= min && i <= max)
        {
            rn_enum = TYBC_mode_set_to_rn_enum(i);
            rn_msg = TYBC_get_mode_set_msg(i);
            if (rn_msg != msg_tyb_null)
            {
                MSG_RN_MODE_SET_U[rn_enum] = rn_msg;
            }
        }
    }

    return MSG_RN_MODE_SET_U;
}

/*******************************************************************************
 �� ��: TYBC_run_status_to_rn_enum()
 �� ��: ����������״̬ö��ֵ����ת��ΪAPP�������״̬ö��ֵ
 �� ��: local_run_status--��������״̬ö��ֵ;
 �� ��: ��
 �� ��: ��ο�log_sys_public_info()�е�����
*******************************************************************************/
U08 TYBC_run_status_to_rn_enum(U08 local_enum)
{
    U08 i;

    /* �����˪ */
    /* ������� */
    if (fg_idle_sta_antifreeze
    || fg_run_sta_ac_antifreeze
    || fg_run_sta_hotw_antifreeze
    )
    {
        return RN_RUN_STATUS_ANTIFREEZE;
    }
    if(log_sys_defrost_comp_num(SUB_SYS_TYPE_MAX))
    {
        return RN_RUN_STATUS_DEFROST;
    }

    /* ����Ԥ��  */
    if (!fg_warm_complete)
    {
        return RN_RUN_STATUS_PREHEAT;
    }

    /* ����run_status */
    for (i = 0; i < ARRELE(TYBC_RUN_STATUS_MAP); i++)
    {
        if (local_enum == TYBC_RUN_STATUS_MAP[i].local_enum)
        {
            return TYBC_RUN_STATUS_MAP[i].rn_enum;
        }
    }
    return RN_RUN_STATUS_IDLE;
}

/*******************************************************************************
 �� ��: TYBC_temp_set()
 �� ��: �����¶�
 �� ��: cloud_val--�ƶ��·����¶��趨ֵ;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void TYBC_temp_set(I16 cloud_val, U08 dpid)
{
	switch (dpid)
	{
		case 16:  			//DPID_TEMP_SET
		case 107:			//DPID_ZONEA_FUNC_CMD
			SHC_set_tmp_zone_1_deal(cloud_val);
			break;
		case 109:			//DPID_ZONEB_FUNC_CMD
			SHC_set_tmp_zone_2_deal(cloud_val);
			break;
		case 111:			//DPID_DHW_FUNC_CMD
			SHC_set_tmp_zone_hotw_deal(cloud_val);
			break;
	}
}

/*******************************************************************************
 �� ��: TYBC_mode_set()
 �� ��: ����ģʽ
 �� ��: cloud_val--�ƶ��·���ģʽ�趨ֵ;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void TYBC_mode_set(I16 cloud_val)
{
    I16 max;
    I16 min;
    I16 val;


    /* �����л�ģʽ */
    if (SHC_ac_mode_switch_disabled())
    {
        return;
    }

    max = para_get_max(P_GaI(sv_mode));
    min = para_get_min(P_GaI(sv_mode));

    switch (cloud_val)
    {
        case RN_MODE_AUTO:
            val = SET_AUTO;
            break;
        case RN_MODE_COOL:
            val = SET_COOL;
            break;
        case RN_MODE_HEAT:
            val = SET_HEAT;
            break;
        case RN_MODE_HOTW:
//            val = SET_HOT_WATER;
//            break;
        case RN_MODE_COOL_HOTW:
//            val = SET_COOL_HOT_WATER;
//            break;
        case RN_MODE_HEAT_HOTW:
//            val = SET_HEAT_HOT_WATER;
//            break;
        case RN_MODE_AUTO_HOTW:
//            val = SET_AUTO_HOT_WATER;
//            break;
        case RN_MODE_FLOOR:
        case RN_MODE_VENT:
        default:
            return;
    }

    if (val >= min && val <= max)
    {
        para_set(P_GaI(sv_mode), val);
    }
}

/* ��"���ٹ���" */
#ifdef TUYA_USE_WIND
/*******************************************************************************
 �� ��: TYBC_get_wind_set_range()
 �� ��: ��ȡ�趨����ѡ���ַ���������ָ��
 �� ��: ��
 �� ��: �ַ���������ָ��
 �� ��: ��
*******************************************************************************/
U16 *TYBC_get_wind_set_range(void)
{
    /* ��������ΪRN_WIND_MAX��˳����RN_WIND_XXX���У���ʹ�û򲻴��ڵ�ģʽ�ÿո��ʾ */
    static U16 MSG_RN_WIND_SET[RN_WIND_MAX] =
    {
        ty_enum_wind_low,   /* 0: RN_WIND_LOW, �ͷ��� */
        ty_enum_wind_mid,   /* 1: RN_WIND_MID, �з��� */
        ty_enum_wind_high,  /* 2: RN_WIND_HIGH, �߷��� */
        ty_enum_wind_auto,  /* 3: RN_WIND_AUTO, �Զ� */
        msg_tyb_null,        /* 4: RN_WIND_SET_MAX, �����������(��) */
        msg_tyb_null,        /* 5: RN_WIND_NULL, �޷��� */
    };

    /* ���и��ݳ���ģʽ����ĳЩģʽ���ڴ˴�����Ӧ��ģʽ����(��Ϊ�ո�) */

    return MSG_RN_WIND_SET;
}

/*******************************************************************************
 �� ��: TYBC_wind_set_to_rn_enum()
 �� ��: �������趨����ö��ֵ����ת��ΪRN����趨����ö��ֵ
 �� ��: local_wind_speed--�����趨����ö��ֵ;
 �� ��: ����趨����ö��ֵ
 �� ��: ��
*******************************************************************************/
U08 TYBC_wind_set_to_rn_enum(U08 local_wind_speed)
{
    RN_ENUM_MAP local_to_rn[RN_WIND_MAX]=
    {
        { 0,      RN_WIND_LOW     },  /* 0: �ͷ��� */
        { 1,      RN_WIND_MID     },  /* 1: �з��� */
        { 2,      RN_WIND_HIGH    },  /* 2: �߷��� */
        { 3,      RN_WIND_AUTO    },  /* 3: �Զ� */
        { 4,      RN_WIND_SET_MAX },  /* 4: ����������� */
        { 5,      RN_WIND_NULL    },  /* 5: �޷��� */
    };
    U08 i;

    for (i = 0; i < RN_WIND_MAX; i++)
    {
        if (local_wind_speed == local_to_rn[i].local_enum)
        {
            return local_to_rn[i].rn_enum;
        }
    }
    return RN_WIND_NULL;
}

/*******************************************************************************
 �� ��: TYBC_get_wind_cur_range()
 �� ��: ��ȡ��ǰ����ѡ���ַ���������ָ��
 �� ��: ��
 �� ��: �ַ���������ָ��
 �� ��: ��
*******************************************************************************/
U16 *TYBC_get_wind_cur_range(void)
{
    /* ��������ΪRN_WIND_MAX��˳����TY_RN_WIND���У���ʹ�õķ����ÿո��ʾ */
    static U16 MSG_RN_WIND_CUR[RN_WIND_MAX] =
    {
        ty_enum_wind_low,   /* 0: RN_WIND_LOW, �ͷ��� */
        ty_enum_wind_mid,   /* 1: RN_WIND_MID, �з��� */
        ty_enum_wind_high,  /* 2: RN_WIND_HIGH, �߷��� */
        msg_tyb_null,        /* 3: RN_WIND_AUTO, �Զ� */
        msg_tyb_null,        /* 4: RN_WIND_SET_MAX, �����������(��) */
        ty_enum_wind_null,  /* 5: RN_WIND_NULL, �޷��� */
    };

    return MSG_RN_WIND_CUR;
}

/*******************************************************************************
 �� ��: TYBC_wind_cur_to_rn_enum()
 �� ��: �����ص�ǰ����ö��ֵ����ת��ΪRN��嵱ǰ����ö��ֵ
 �� ��: local_wind_speed--���ص�ǰ����ö��ֵ ;
 �� ��: ��嵱ǰ����ö��ֵ
 �� ��: ��
*******************************************************************************/
U08 TYBC_wind_cur_to_rn_enum(U08 local_wind_speed)
{
    RN_ENUM_MAP local_to_rn[RN_WIND_MAX]=
    {
        { 0,      RN_WIND_LOW     },  /* 0: �ͷ��� */
        { 1,      RN_WIND_MID     },  /* 1: �з��� */
        { 2,      RN_WIND_HIGH    },  /* 2: �߷��� */
        { 3,      RN_WIND_AUTO    },  /* 3: �Զ� */
        { 4,      RN_WIND_SET_MAX },  /* 4: ����������� */
        { 5,      RN_WIND_NULL    },  /* 5: �޷��� */
    };
    U08 i;

    for (i = 0; i < RN_WIND_MAX; i++)
    {
        if (local_wind_speed == local_to_rn[i].local_enum)
        {
            return local_to_rn[i].rn_enum;
        }
    }
    return RN_WIND_NULL;
}
#endif

/* ��"���ٹ���" */
#ifdef TUYA_USE_WIND
/*******************************************************************************
 �� ��: TYBC_wind_set()
 �� ��: ���÷���
 �� ��: cloud_val--�ƶ��·��ķ����趨ֵ;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void TYBC_wind_set(I16 cloud_val)
{
    ;
}
#endif

/* ��"ʪ�ȹ���" */
#ifdef TUYA_USE_HUMI
/*******************************************************************************
 �� ��: TYBC_humi_set()
 �� ��: ����ʪ��
 �� ��: cloud_val--�ƶ��·���ʪ���趨ֵ;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void TYBC_humi_set(I16 cloud_val)
{
    ;
}
#endif

/* ��"��ˮ����" */
#ifdef TUYA_USE_HOT_WATER
/*******************************************************************************
 �� ��: TYBC_hot_water_set()
 �� ��: ������ˮ�¶�
 �� ��: cloud_val--�ƶ��·�����ˮ�¶��趨ֵ;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void TYBC_hot_water_set(I16 cloud_val)
{
    U08 item_id = 0xFF;
    U08 grp_id = 0xFF;
    I16 max;
    I16 min;

    grp_id = P_grp(sv_hotw);
    item_id = P_idx(sv_hotw);

    if (item_id != 0xFF)
    {
        /* ��ȡ��Ӧ�¶ȵ����ֵ����Сֵ */
        max = para_get_max(grp_id, item_id);
        min = para_get_min(grp_id, item_id);

        /* �趨�¶� */
        if (cloud_val >= min && cloud_val <= max)
        {
            para_set(grp_id, item_id, cloud_val);
        }
    }
}
#endif

/* ��"��ʱ����" */
#ifdef TUYA_USE_CONFIG_TIMER
void TYBC_config_timer(void)
{
    ;
}
#endif

/* ��"��ʼ������" */
#ifdef TUYA_USE_CONFIG_INIT
void TYBC_config_init(void)
{
    TYBC_CMD_SPLASH_ON;
}
#endif

#ifndef TUYA_USE_HMI_SELF
/* ֻ֧�ִӻ��� */
#ifndef TUYA_USE_HMI_MASTER
/*******************************************************************************
 �� ��: TYBC_sign_local2sh()
 �� ��: ����sign����ת��ΪSH sign����(SmartHmi��ͬ�������ĸ���)
 �� ��: sign_local--��������;
 �� ��: SH����
 �� ��: ��
*******************************************************************************/
U16 TYBC_sign_local2sh(U16 sign_local)
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
            unit_sh = TYBC_UNIT_NULL;         /* �� */
            break;
        case UINT_C:
            unit_sh = TYBC_UNIT_TMP_C;        /* �¶�(��) */
            break;
        case UINT_A:
            unit_sh = TYBC_UNIT_A;            /* ����(A) */
            break;
        case UINT_PR:
            unit_sh = TYBC_UNIT_PRS_B;        /* ѹ��(bar) */
            break;
        case UINT_PE:
            unit_sh = TYBC_UNIT_PE;           /* �ٷֱ�(%) */
            break;
        case UINT_F:
            unit_sh = TYBC_UNIT_HZ;           /* Ƶ��(HZ) */
            break;
        case UINT_RS:
            unit_sh = TYBC_UNIT_RPS;          /* ת��(rps) */
            break;
        case UINT_RM:
            unit_sh = TYBC_UNIT_RPM;          /* ת��(rpm) */
            break;
        case UINT_S:
            unit_sh = TYBC_UNIT_SECOND;       /* �� */
            break;
        case UINT_M:
            unit_sh = TYBC_UNIT_MINT;         /* �� */
            break;
        case UINT_H:
            unit_sh = TYBC_UNIT_HOUR;         /* ʱ */
            break;
        case UINT_D:
            unit_sh = TYBC_UNIT_DAY;          /* �� */
            break;
        case UINT_ST:
            unit_sh = TYBC_UNIT_ST;           /* �� */
            break;
        case UINT_V:
            unit_sh = TYBC_UNIT_V;            /* V */
            break;
        default:
            unit_sh = TYBC_UNIT_NULL;         /* �� */
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
            auth_sh = TYBC_PARA_PRO;
            break;
        case D_SER:
            auth_sh = TYBC_PARA_SER;
            break;
        case D_FAC:
            auth_sh = TYBC_PARA_FAC;
            break;
        case D_USER:
        default:
            auth_sh = TYBC_PARA_USR;
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
            | (decimal_sh << TYBC_DECIMAL_BGN_BIT)
            | (fg_is_hide << TYBC_HIDE_BGN_BIT)
            | (fg_is_const << TYBC_CONST_BGN_BIT)
            | (auth_sh << TYBC_SAFE_BGN_BIT)
            | (fg_is_bit << TYBC_BIT_BGN_BIT);

    return sign_sh;
}

/* ��"��ǰ����" */
#ifdef TUYA_USE_ERR_CUR
/*******************************************************************************
 �� ��: TYBC_get_err_cur()
 �� ��: ���ϲ�ѯʹ�õ��Ĺ��������ַ���(Դ��SmartHmi_config.c)
 �� ��: err--������Ϣ;
        sys_err_id--ϵͳ���Ϻ�;
        mod_id--ģ���;
        mod_err_id--ģ����Ϻ�;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL TYBC_get_err_cur(TYBC_ERR_CUR *err, U16 *sys_err_id, U16 *mod_id, U16 *mod_err_id)
{
    BOOL fg_err = FALSE;

    /* ϵͳ���� */
    for (; *sys_err_id<TYBC_NO_ERR; (*sys_err_id)++)
    {
        if (fg_err)  break;

        if (m_get_error(MAX_PCB, *sys_err_id))
        {
            fg_err = TRUE;
            err->name = TYBC_GET_M_ERR(*sys_err_id);
            err->id = MAKEWORD(TYBC_NO_ID, TYBC_NO_ID);
            err->err_id = *sys_err_id;
        }
    }

    if (!fg_err)
    {
        /* ��Ԫ���� */
        for (; *mod_id<MAX_PCB; *mod_err_id=0,(*mod_id)++)
        {
            for (; *mod_err_id<TYBC_NO_ERR; (*mod_err_id)++)
            {
                /* ����forѭ����ͷ��ʹk�ܼ�1���´β����ظ���ȡk */
                if (fg_err)  break;

                if (m_get_error(*mod_id, *mod_err_id))
                {
                    fg_err = TRUE;

                    /* ע�⣡����������Ƶ���ϸ�Ϊϵͳ������ϣ���Ҫ���Ĵ˴� */
                    if (*mod_err_id == err_invt_id_to_err_id(INVT_ID_1, INVT_ERR_ERROR))
                    {
                        if (fun_pv_in(CELL_INVT_ERR(*mod_id, INVT_ID_1), INVT_ERR_ID_MIN, INVT_ERR_ID_MAX))
                        {
                            err->name = msg_invt_error[CELL_INVT_ERR(*mod_id, INVT_ID_1)-1];
                        }
                        else
                        {
                            err->name = msg_invt_error_unknown;
                        }
                    }
                    else if(*mod_err_id == S_ERR_PUMPF)
                    {
                        if (fun_pv_in_open(pv_cell_PUMPf_err_code(*mod_id), PUMF_NO_ERR, PUMPF_ERR_MAX))
                        {
                            err->name = msg_PUMPf_error[pv_cell_PUMPf_err_code(*mod_id)-1];
                        }
                        else
                        {
                            err->name = msg_PUMPf_error_unknown;
                        }
                    }
                    else
                    {
                        err->name = SHC_NAME_S_ERR(*mod_err_id);
                    }
                    err->id = MAKEWORD(*mod_id, TYBC_NO_ID);
                    err->err_id = *mod_err_id;
                }
            }
            /* ����forѭ���Ľ�β����ֹc��1,������©��ģ����������� */
            if (fg_err)  break;
        }
    }

    return fg_err;
}

/*******************************************************************************
 �� ��: TYBC_get_err_cur_num()
 �� ��: ��ǰ���ϸ���
 �� ��: ��
 �� ��: ��ǰ���ϸ���
 �� ��: ��
*******************************************************************************/
U16 TYBC_get_err_cur_num(void)
{
    U16 i = 0;
    U16 j = 0;
    U16 k = 0;
    U16 temp = 0;

    for (i = 0; i < val_pcb_num; i++)           /* ͳ��ģ����� */
    {
        for (j = 0; j < MAX_ERR_WORD; j++)
        {
            for (k = 0; k < 16; k++)
            {
                if (GETBIT(cell[i].cell_error[j], k))
                {
                    temp++;
                }
            }
        }
    }
    for (j = 0; j < MAX_ERR_WORD; j++)          /* ͳ��ϵͳ���� */
    {
        for (k = 0; k < 16; k++)
        {
            if (GETBIT(cell[MAX_PCB].cell_error[j], k))
            {
                temp++;
            }
        }
    }

    return temp;
}
#endif

/* ��"��ʷ����" */
#ifdef TUYA_USE_ERR_HIS
/*******************************************************************************
 �� ��: TYBC_get_err_his_info()
 �� ��: ��ʷ������Ϣ(Դ��SmartHmi_config.c)
 �� ��: err--��ʷ������Ϣ;
        idx--������;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void TYBC_get_err_his_info(TYBC_ERR_HIS *err, U08 idx)
{
    U08 i;
    U08 err_id = 0;
    U08 err_pcb = 0;
    U08 sign = 0;
    U08 his_err_one[TYBC_HIS_ERR_TBL_SIZE];

    /* ��ȡ��ʷ���� */
    for (i=0; i<TYBC_HIS_ERR_TBL_SIZE; i++)
    {
        his_err_one[i] = his_err_tbl[idx*TYBC_HIS_ERR_TBL_SIZE+i];
    }

    /* ����������š����Ϻš�����ʱ�� */
    err_id = TYBC_NO_ERR;
    if (idx < TYBC_HIS_ERR_MAX)
    {
        err_id  = his_err_one[0];
        err_pcb = his_err_one[1];
        sign    = his_err_one[2];
    }
    if (err_id < TYBC_NO_ERR)
    {
        if (err_pcb == TYBC_PCB_MAX)         /* ϵͳ���� */
        {
            err->name = TYBC_GET_M_ERR(err_id);
            err->id = MAKEWORD(TYBC_NO_ID, TYBC_NO_ID);
            err->err_id = err_id;
        }
        else            /* ��Ԫ���� */
        {
            if (err_id == err_invt_id_to_err_id(INVT_ID_1, INVT_ERR_ERROR))
            {
                if (fun_pv_in(sign, INVT_ERR_ID_MIN, INVT_ERR_ID_MAX))
                {
                    err->name = msg_invt_error[sign-1];
                }
                else if(err_id == S_ERR_PUMPF)
                {
                    if (sign > PUMF_NO_ERR && sign < PUMPF_ERR_MAX)
                    {
                        err->name = msg_PUMPf_error[sign-1];
                    }
                    else
                    {
                        err->name = msg_PUMPf_error_unknown;
                    }
                }
                else
                {
                    err->name = msg_invt_error_unknown;
                }
            }
            else
            {
                err->name = SHC_NAME_S_ERR(err_id);
            }

            err->id = MAKEWORD(err_pcb, TYBC_NO_ID);
            err->err_id = err_id;
        }
        err->time.month  = his_err_one[3];
        err->time.day    = his_err_one[4];
        err->time.hour   = his_err_one[5];
        err->time.mint   = his_err_one[6];
    }
    else
    {
        err->name = TYBC_ERR_WEI_ZHI_GZ;  /* ��Ϊ"δ֪����"����ʾ */
        err->id = 0;
        err->err_id = 0;
        err->time.month = 0;
        err->time.day = 0;
        err->time.hour = 0;
        err->time.mint= 0;
    }
}
#endif

/* ��"��������" */
#ifdef TUYA_USE_PARAM_SET
/*******************************************************************************
 �� ��: TYBC_get_para_grp_sign()
 �� ��: ��ȡ����������(Դ��SmartHmi_config.c)
 �� ��: grp--������;
 �� ��: ����������
 �� ��: ��
*******************************************************************************/
U16 TYBC_get_para_grp_sign(U08 grp)
{
    extern PARAMITEM  ParamPri[];
    U16 sign = 0;

    sign = ParamPri[grp].sign;

    return sign;
}


/*******************************************************************************
 �� ��: TYBC_cal_grp_attrib()
 �� ��: ���������Ը�ʽת��ΪSH���Ը�ʽ(SmartHmi��ͬ�������ĸ���)
 �� ��: grp--������;
 �� ��: ����������
 �� ��: ������������:Ȩ��(ʹ�õ�λ������)���Ƿ�����
*******************************************************************************/
U16 TYBC_cal_grp_attrib(U08 grp)
{
    U16 sign_local = 0;     /* ԭʼ���� */
    U16 sign_sh = 0;        /* ת�������� */
    U16 authority_local = 0;/* ԭʼȨ�� */
    U16 authority_sh = 0;   /* ת����Ȩ�� */
    U16 fg_is_hide = 0;     /* �������� */

    /* ��ȡ���� */
    sign_local = TYBC_get_para_grp_sign(grp);

    /* Ȩ�� */
    authority_local = sign_local & D_SAFEMASK;
    switch (authority_local)
    {
        case D_USER:
            authority_sh = TYBC_PWR_USER;
            break;
        case D_PRO:
            authority_sh = TYBC_PWR_PRO;
            break;
        case D_SER:
            authority_sh = TYBC_PWR_SER;
            break;
        case D_FAC:
            authority_sh = TYBC_PWR_FAC;
            break;
        default:
            authority_sh = TYBC_PWR_NONE;
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
#endif

/*******************************************************************************
 �� ��: TYBC_get_dict_idx()
 �� ��: ��ȡ�ֵ����(Դ��SmartHmi_config.c)
 �� ��: pmsg--�ֵ�ָ��;
 �� ��: �ֵ����
 �� ��: ��
*******************************************************************************/
U16 TYBC_get_dict_idx(const U16 *pmsg)
{
    U16 i;

    if (pmsg != NULL)
    {
        for (i = 0; i < TYBC_PARAM_DICT_NUM; i++)
        {
            if(para_dict[i].pmsg == pmsg)
            {
                return i;
            }
        }
    }

    return 0;
}

/*******************************************************************************
 �� ��: TYBC_attr_local2sh()
 �� ��: ��������ת��ΪSH����,�����ֵ���Ϣ(SmartHmi��ͬ�������ĸ���)
 �� ��: sign_local--��������;
        dic_str--�ֵ�;
 �� ��: SH����
 �� ��: ��
*******************************************************************************/
U16 TYBC_attr_local2sh(U16 sign_local, const U16 *dic_str)
{
    U16 sign_sh = 0;
    U16 dict_sh = 0;        /* ת����λ */
    U16 dict_idx = 0;       /* ����ֵ�ֵ���� */

    sign_sh |= TYBC_sign_local2sh(sign_local);

    if (dic_str != NULL)    /* �ַ�����:�����ֵ� */
    {
        sign_sh &= ~(TYBC_UNIT|TYBC_DECIMAL);
        dict_idx = TYBC_get_dict_idx(dic_str);/* �ֵ���� */
        dict_sh = dict_idx + TYBC_UNIT_OFFSET;   /* ����ƫ�� */
        sign_sh |= dict_sh;
    }

    return sign_sh;
}

/*******************************************************************************
 �� ��: TYBC_cal_para_attrib()
 �� ��: ���������Ը�ʽת��ΪSH���Ը�ʽ(Դ��SmartHmi.c)
 �� ��: grp--������;
        item--������;
 �� ��: ����������
 �� ��: ������������:�Ƿ�Ϊ�ַ����͡���λ���Ƿ����ء��Ƿ�ɸ�
*******************************************************************************/
U16 TYBC_cal_para_attrib(U08 grp, U08 item)
{
    U16 sign_local = 0;     /* ԭʼ���� */
    U16 sign_sh = 0;        /* ת�������� */

    /* ��ȡԭʼ���� */
    sign_local = para_get_sign(grp, item);

    sign_sh |= TYBC_attr_local2sh(sign_local, para_small[grp][item].pmsg);

    return sign_sh;
}
#endif
#endif

#ifdef TUYA_HAS_STAT
/*******************************************************************************
 �� ��: TYBC_get_pv_val()
 �� ��:
 �� ��: pcb_idx--;
		pv_idx--;
 �� ��:
 �� ��: ��
*******************************************************************************/
I16 TYBC_get_pv_val(U16 pcb_idx, U16 pv_idx)
{
	I16 value = cell[pcb_idx].cell_pv[pv_idx];
	U16 attrib = 0;

	if (TYBC_PCB_MAX == pcb_idx)
	{
		attrib = msg_sys_p[(pv_idx)].attr;
	}
	else
	{
		if (pv_idx < TYBC_UNIT_PV_BEGIN)
		{
			attrib = msg_uint_p_comp[pv_idx].attr;
		}
		else
		{
			attrib = msg_uint_p_cell[(pv_idx-TYBC_UNIT_PV_BEGIN)].attr;
		}
	}

	if (TEMP_IS_FARA && attrib != 0xFFFF)
	{
		value = C_To_F(value, attrib);
	}

	return value;
}
#endif

