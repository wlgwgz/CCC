/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: Tuya_B_Config.h
 �ļ�����: Ϳѻ���߹�������(��岿��)
 ������  : �ֺƱ�
 ��������: 2022.03.11
*******************************************************************************/

#ifdef TUYA_B_CONFIG_GLOBAL
    #define TUYA_B_CFG_EXT
#else
    #define TUYA_B_CFG_EXT extern
#endif

/* ========================================================================================== */
/* ע������:                                                                                  */
/* 1�����ļ����еĺ궨������ֹ���ģ�                                                          */
/* 2�����ļ�����"TUYA_USE_XXX"��ص���������(#if/#ifdef/#ifndef)��������Ķ���                */
/* ========================================================================================== */

/* �ֻ������Դ�("TUYA-DEBUG"ģʽ!����ʱһ��Ҫ����!!!) */
//#define TUYA_USE_DBG_CONSOLE

/* ʹ�ü�ʱ���� */
//#define TUYA_USE_CONFIG_TIMER
/* ʹ�ó�ʼ������ */
#define TUYA_USE_CONFIG_INIT
 /* ʹ��"���������籣��"(ע: �ǵ��򲻵���ʱ�������δ���!) */
#define TUYA_USE_BUF_SAFETY

/* ҵ���߼����� */
#define TUYA_B_SLP_TYPE                            (TUYA_B_SLP_NT_HLXXX_MULTI)

/* ����ѡ�� */
#define TUYA_USE_HMI_MASTER                        /* ֧��������(�²���Э��) */
#define TUYA_USE_HMI_SLAVE	                       /* ֧�ִӻ��� */
//#define TUYA_USE_HMI_SELF	                       /* ֧��һ��ʽ��(����) */
#define TUYA_USE_SYS_STAT                          /* ʹ��"ϵͳ״̬"���� */
#define TUYA_USE_MOD_STAT                          /* ʹ��"ģ��״̬"���� */
#define TUYA_USE_PARAM_SET                         /* ʹ��"��������"���� */
#define TUYA_USE_ERR_CUR                           /* ʹ��"��ǰ����"���� */
#define TUYA_USE_ERR_HIS                           /* ʹ��"��ʷ����"���� */
#define TUYA_USE_FAC_PWD                           /* ʹ��"��������"���� */
#define TUYA_USE_LANG_SET                          /* ʹ��"����"���� */
#define TUYA_USE_SPLASH                            /* ʹ��"����ҳ"���� */
//#define TUYA_USE_CHILDLOCK                         /* ʹ��"ͯ��"���� */
//#define TUYA_USE_WIND                              /* ʹ��"����"���� */
//#define TUYA_USE_HUMI                              /* ʹ��"ʪ��"���� */
//#define TUYA_USE_HOT_WATER                         /* ʹ��"��ˮ"���� */
#define TUYA_USE_ELE_STAT                          /* ʹ��"�õ��ѯ"���� */
//#define TUYA_USE_CLO_TIME                          /* ʹ��"�ƶ�ʱ"���� */
//#define TUYA_USE_TEMP_UNIT_SET				/* ʹ���¶ȵ�λ���� */

/* ========================================================================================== */
/* ���涨��                                                                                   */
/* ========================================================================================== */
#define TYBC_PCB_MAX                               (MAX_PCB)                   /* Ԥ�����ģ���� */
#define TYBC_COMP_MAX                              (USE_COMP)                  /* ���ѹ���� */
#define TYBC_PV_MAX                                (MAX_PV_WORD)               /* cell_pv[]���� */
#define TYBC_SYS_PV_MAX                            (SYS_PV_NUM)                /* ϵͳģ��������(ע��SYS_PV_NUM�Ժ�Ŀռ����������;) */
#define TYBC_UNIT_PV_BEGIN                         (CELL_PV_BEGIN)             /* ��Ԫģ������ʼλ�� */
#define TYBC_COMP_PV_MAX                           (COMP_PV_NUM)               /* ѹ��ģ���������� */
#define TYBC_MOD_PV_MSG_MAX                        (CELL_PV_NUM+COMP_PV_NUM)   /* ��Ԫ&ѹ��ģ������������ */
#define TYBC_PWD_MAX                               (MAX_PASS_BIT)              /* �������λ�� */
#define TYBC_NO_ERR                                (F_NO_ERR)                  /* �޹��Ϻ� */
#define TYBC_MSG_NULL                              (msg_tyb_null)               /* ���ַ��� */
#define TYBC_SEC_FLAG                              (F_TIME_BASIC_SEC_BACK)     /* 1���־ */
#define TYBC_STR_BYTE_MAX                          (120)		                   /* �����ַ�������ֽ��� */

/* ========================================================================================== */
/* ��Ʒ����                                                                                   */
/* ========================================================================================== */
/* ģ������ */
#define TYBC_MODULE_TYPE                           (MODULE_WIFI)     /* WIFIģ�� */

/* ��ƷID */
//#define TYBC_PID                                   "cawxbpbi0l9vzyzy"/* ��Ʒ����: �¿���Ʒ��_��Ƶ������ */
//#define TYBC_PID							       "6o2it5fidbsi18zc"	/* �յ�+��ˮ ͬʱ�趨 */
//#define TYBC_PID							       "p7yl8alakseirps3"	/* ��Ƶ������ */
//#define TYBC_PID                                   "9dn51spevrkyaipl"   /* ��Ƶ������debug */

/* ΢�������&�汾�� */
#if defined(M_DEBUG)
	#define TYBC_PID		"figijxoiqt2wlj8b"				/* DEBUG_ʹ��ͨ��pid */
#elif defined(M_TCL)
    #define TYBC_PID        "gvedawoegg8idjha"              /* ��ɽTCL */
#elif defined(M_CHANGHONG)
    #define TYBC_PID        "sa8ar4ibpyxm1k7u"              /* ��ɽ���� */
#elif defined(M_TONGYONG)
    #define TYBC_PID        "figijxoiqt2wlj8b"              /* ͨ�� */
#elif defined(M_PAIWO)
    #define TYBC_PID        "ey0j6ijachritgw7"              /* �㶫���� */
#elif defined(M_DISEN)
    #define TYBC_PID        "2nvnrlxwzkaodxq4"              /* ���ݵ�ɭ */
#elif defined(M_NIUENTAI)
    #define TYBC_PID        "jdvpsawdciqkkuhr"              /* �㶫Ŧ��̩ */
#elif defined(M_SHENGNENG)
    #define TYBC_PID        "1ceeidrbexlkqhcb"              /* �㽭���� */
#elif defined(M_HONGHE)
    #define TYBC_PID        "yxbw8sprrra5djvi"              /* ��ɽ��� */
#elif defined(M_WANHE)
    #define TYBC_PID        "fhsjdw1dca6vb1pl"              /* ��ͼ��� */
#elif defined(M_CHANGFAN)
    #define TYBC_PID        "kjkkmazngxxefqrd"              /* �㶫���� */
#else
    #error "δ���峧��!"
#endif


/* ========================================================================================== */
/* ��Ӣ�ַ����ֵ�����                                                                         */
/* ========================================================================================== */
#ifdef USE_GBK
#define TYBC_CODE_STR_DICT_CN(idx)                 (IDX_TO_C_PTR(idx))   /* ���������ַ��������ֵ� */
#define TYBC_CODE_STR_DICT_EN(idx)                 (IDX_TO_E_PTR(idx))   /* ����Ӣ���ַ��������ֵ� */
#elif defined (USE_UNICODE)
#define TYBC_CODE_STR_DICT_CN(idx)                 (chinese_uncompress_text(idx))   /* ���������ַ��������ֵ� */
#define TYBC_CODE_STR_DICT_EN(idx)                 (english_uncompress_text(idx))   /* ����Ӣ���ַ��������ֵ� */
#define TYBC_CODE_STR_DICT_SP(idx)                 (spanish_uncompress_text(idx))	 /* �������������ַ��������ֵ� */
#define TYBC_CODE_STR_DICT_GR(idx)				   (greek_uncompress_text(idx))	/* ����ϣ�����ַ��������ֵ� */
#define TYBC_CODE_STR_DICT_FR(idx)				   (french_uncompress_text(idx))	/* ���뷨���ַ��������ֵ� */
#define TYBC_CODE_STR_DICT_ITL(idx)				   (italian_uncompress_text(idx))	/* ������������ַ��������ֵ� */
#define TYBC_CODE_STR_DICT_POL(idx)				   (polish_uncompress_text(idx))	/* ���벨�����ַ��������ֵ� */
#define TYBC_CODE_STR_DICT_HG(idx)				   (hungarian_uncompress_text(idx))	/* �������������ַ��������ֵ� */
#define TYBC_CODE_STR_DICT_GE(idx)				   (german_uncompress_text(idx))	/* ��������ַ��������ֵ� */
#endif

/* ========================================================================================== */
/* �汾��Ϣ����                                                                               */
/* ========================================================================================== */
/* �ֶ�0: �Ƿ���ʾ��ʾ���汾�� */
#define TYBC_DISP_HMI_VER_EN                       (TRUE)

/* �ֶ�1: �Ƿ���ʾ�������汾�� */
#define TYBC_DISP_BKBD_VER_EN                      (TRUE)
/* ��ʾ�������汾 */
#if (TYBC_DISP_BKBD_VER_EN == TRUE)
#define TYBC_BKBD_VER                              (APPFUNCODEVER)
#endif

/* ========================================================================================== */
/* ϵͳ��Ϣ����                                                                               */
/* ========================================================================================== */
/* �ֶ�0: �ַ�����������Ϣ */
#ifdef USE_UNICODE
#define TYBC_STR_CODE_TYPE                         (UNI_CODE)
#else
#define TYBC_STR_CODE_TYPE                         (P_CODE)
#endif

/* �ֶ�1: ϵͳά����־��Ϣ */
#define TYBC_SYS_MAINTENANCE_VAL                   ((fg_sys_limit) ? (TRUE) : (FALSE))

/* �ֶ�2: �����ʾ������Ϣ */
#define TYBC_LANGUAGE_VAL                          (TYBC_language_to_rn_enum(sh_language))

/* �ֶ�3: ���ϸ澯��־��Ϣ */
#define TYBC_ERROR_ALERT_VAL                       ((fg_error) ? (TRUE) : (FALSE))

/* �ֶ�4: ģ������Ϣ(��������0) */
#define TYBC_MOD_NUM                               (val_pcb_num)

/* �ֶ�5: ����������Ϣ */
/* ��"��������" */
#ifdef TUYA_USE_FAC_PWD
#define TYBC_PASSWORD_POINTER                      (TYBC_get_password_pointer())  /* ��������ָ��(ָ��U08*) */
/* ��"��������" */
#else
#define TYBC_PASSWORD_POINTER                      (PNULL)
#endif

/* �ֶ�6: �����л�ģʽ������Ϣ */
#define TYBC_RUN_SWITCH_MODE_EN                    (TRUE)

/* �ֶ�7: ����ҳLOGO ID��Ϣ(������ҳ���� SPLASH_LOGO_NULL) */
#define TYBC_SPLASH_LOGO_VAL                       (val_TCL_splash_offset ? (SPLASH_LOGO_TCL+val_TCL_splash_offset-1) : 0)

/* �ֶ�8: ��������������Ϣ */
#define TYBC_SMART_SPEAKER_EN                      (FALSE)

/* �ֶ�9: ����������(Ԥ�������) */
#define TYBC_PARAM_GRP_MAX                         (GROUP_MAX)

/* �ֶ�10: ����������/ÿ����������(Ԥ�������) */
#define TYBC_PARAM_GRP_SIZE                        (GROUP_SIZE)

/* �ֶ�11: ��ѡ����(�˴��ɰ󶨲�������ֻѡ��"����"��"ʹ��") */
#define TYBC_FUNC_EN_MORE_GEN_SET                  (TRUE)	/* "����"-"ͨ������"��� */
#define TYBC_FUNC_EN_ADVANCE_ERR_HIS               (TRUE)	/* "�߼�"-"��ʷ����"��� */
#define TYBC_FUNC_EN_ADVANCE_PARAM_SET             (TRUE)	/* "�߼�"-"��������"��� */
#define TYBC_FUNC_EN_MAIN_STAT                     (FALSE)	/* "������"-"״̬��ѯ"��� */
#define TYBC_FUNC_EN_ADVANCE_STAT                  (TRUE)	/* "�߼�"-"״̬��ѯ"��� */
#define TYBC_FUNC_EN_MAIN_ELE_STAT                 (USE_ELEC_METERING)   /* "������"-"�õ��ѯ"��� */
#define TYBC_FUNC_EN_MORE_CLO_TIME                 (FALSE)   /* "����"-"�ƶ�ʱ"��� */

/* ========================================================================================== */
/* ͨ��������Ϣ����                                                                           */
/* ========================================================================================== */
/* �ֶ�0: ����������Ϣ */
/* ��"���Թ���" */
#ifdef TUYA_USE_LANG_SET
#define TYBC_LANGUAGE_TYPE                         (FIELD_ENUM)    /* ʹ�ã�FIELD_ENUM ���ã�FIELD_DISABLED */
#define TYBC_LANGUAGE_TITLE                        (TY_MSGID_TIT_LANG)
#define TYBC_LANGUAGE_RANGE                        (TYBC_get_language_range())
#define TYBC_LANGUAGE_MAX_NUM                      (RN_LANGUAGE_MAX)
#endif

/* �ֶ�1: ����ҳ������Ϣ */
/* ��"����ҳ����" */
#ifdef TUYA_USE_SPLASH
#define TYBC_SPLASH_TYPE                           (FIELD_BOOL)    /* ʹ�ã�FIELD_BOOL ���ã�FIELD_DISABLED */
#define TYBC_SPLASH_TITLE                          (TY_MSGID_TIT_SPLASH)
#define TYBC_SPLASH_VAL                            ((val_TCL_splash_offset&&para_splash) ? TRUE : FALSE)
#endif

/* �ֶ�2: ͯ��������Ϣ */
/* ��"ͯ������" */
#ifdef TUYA_USE_CHILDLOCK
#define TYBC_CHILDLOCK_TYPE                        (FIELD_BOOL)    /* ʹ�ã�FIELD_BOOL ���ã�FIELD_DISABLED */
#define TYBC_CHILD_LOCK_TITLE                      (TY_MSGID_TIT_CHILDLOCK)
#define TYBC_CHILDLOCK_VAL                         ((val_lock_screen==TRUE) ? (TRUE) : (FALSE))
#endif

/* �ֶ�3: ����һʹ�� */
/* ��"����һ" */
#define TYBC_ZoneA_EN_TYPE                        (FIELD_BOOL)    /* ʹ�ã�FIELD_BOOL ���ã�FIELD_DISABLED */
#define TYBC_ZoneA_EN_TITLE                       (TY_MSGID_TIT_ZONE1)
#define TYBC_ZoneA_EN_VAL                         (TRUE)

/* �ֶ�4: �����ʹ�� */
/* ��"�����" */
#define TYBC_ZoneB_EN_TYPE                        (FIELD_BOOL)    /* ʹ�ã�FIELD_BOOL ���ã�FIELD_DISABLED */
#define TYBC_ZoneB_EN_TITLE                       (TY_MSGID_TIT_ZONE2)
#define TYBC_ZoneB_EN_VAL                         ((USE_DUAL_ZONE_CTRL) ? (TRUE) : (FALSE))

/* �ֶ�5: ��ˮʹ�� */
/* ��"��ˮ����" */
#define TYBC_DHW_EN_TYPE                         (FIELD_BOOL)    /* ʹ�ã�FIELD_BOOL ���ã�FIELD_DISABLED */
#define TYBC_DHW_EN_TITLE                        (TY_MSGID_TIT_DHW)
#define TYBC_DHW_EN_VAL                          ((val_hotw_used==1) ? (TRUE) : (FALSE))

/* ��"�¶ȵ�λ���ù���" */
#ifdef TUYA_USE_TEMP_UNIT_SET
#define TYBC_TEMP_UNIT_TYPE                         (FIELD_ENUM)    /* ʹ�ã�FIELD_ENUM ���ã�FIELD_DISABLED */
#define TYBC_TEMP_UNIT_TITLE                        (TY_MSGID_TIT_TEMP_UNIT)
#define TYBC_TEMP_UNIT_RANGE                        (TYBC_get_temp_unit_range())
#define TYBC_TEMP_UNIT_MAX_NUM                      (RN_TEMP_UNIT_MAX)
#define TYBC_TEMP_UNIT_VAL                          (TEMP_IS_FARA)
#endif

/* ========================================================================================== */
/* ͨ�����������                                                                         */
/* ========================================================================================== */
/* �ֶ�0: ��������� */
/* ��"���Թ���" */
#ifdef TUYA_USE_LANG_SET
#define TYBC_CMD_LANGUAGE_CN                       (TYBC_language_set(LANGUE_CHINESE))    /* �������� */
#define TYBC_CMD_LANGUAGE_EN                       (TYBC_language_set(LANGUE_ENGLISH))    /* ����Ӣ�� */
#endif

/* �ֶ�1: ����ҳ����� */
/* ��"����ҳ����" */
#ifdef TUYA_USE_SPLASH
#define TYBC_CMD_SPLASH_ON                         (para_splash = 1)/* ������ҳ */
#define TYBC_CMD_SPLASH_OFF                        (para_splash = 0)/* �ر�����ҳ */
#endif

/* �ֶ�2: ͯ������� */
/* ��"ͯ������" */
#ifdef TUYA_USE_CHILDLOCK
#define TYBC_CMD_CHILDLOCK_ON                      (val_lock_screen = TRUE)  /* ��ͯ�� */
#define TYBC_CMD_CHILDLOCK_OFF                     (val_lock_screen = FALSE)  /* �ر�ͯ�� */
#endif

TUYA_B_CFG_EXT SH_MAIN_INFO sh_main_info;
/* ========================================================================================== */
/* ����һ������Ϣ����                                                                         */
/* ========================================================================================== */
/* �ֶ�0: ���ػ� */
#define TYBC_SWITCH_TYPE                           (FIELD_BOOL)      			/* ʹ�ã�FIELD_BOOL �ֶβ��ã�FIELD_DISABLED */
#define TYBC_SWITCH_TITLE                          (TY_MSGID_TIT_SWITCH)        /* �ַ���ID */
#define TYBC_SWITCH_VAL                            (sh_main_info.fg_zone_1_is_run)				/* ��ǰֵ */

/* �ֶ�1: �趨�¶� */
#define TYBC_TEMP_SET_TYPE                         (FIELD_NUMBER)    /* ʹ�ã�FIELD_NUMBER ���ã�FIELD_DISABLED */
#define TYBC_TEMP_SET_TITLE                        (TY_MSGID_TIT_SET_TEMP)	/* �ַ���ID */
#define TYBC_TEMP_SET_ADDR                         ((ac_mode==MODE_COOL)?P_idx(sv_cool):P_idx(sv_heat))	/* �������� */
#define TYBC_TEMP_SET_VAL                          (SHC_get_set_tmp_A())/* �趨ֵ */
#define TYBC_TEMP_SET_MAX                          (SHC_get_set_tmp_max_A())  /* ���� */
#define TYBC_TEMP_SET_MIN                          (SHC_get_set_tmp_min_A())  /* ���� */
#define TYBC_TEMP_SET_ATTR                         ((TYBC_cal_para_attrib(GROUP_USER,TYBC_TEMP_SET_ADDR) \
                                                   &(TYBC_UNIT|TYBC_DECIMAL))|STEP_1)  /* ����(ע�⣺�趨�¶Ȳ���ֵ�����ֵ����Сֵ��С����λ����һ��) */

/* �ֶ�2: ��ǰ�¶� */
#define TYBC_TEMP_CUR_TYPE                         (FIELD_RO_NUM)    /* ʹ�ã�FIELD_RO_NUM ���ã�FIELD_DISABLED */
#define TYBC_TEMP_CUR_TITLE                        (TY_MSGID_TIT_CUR_TEMP)                  /* �ַ���ID */
#define TYBC_TEMP_CUR_VAL                          (ctrl_temp_energy[SUB_SYS_AC_ONLY])	/* �ܳ�ˮ�¶ȵ�ǰֵ */
#define TYBC_TEMP_CUR_ATTR                         ((TYBC_sign_local2sh(UINT_C|D_DOT1) \
                                                   &(TYBC_UNIT|TYBC_DECIMAL))|STEP_1)     /* ���� */

/* �ֶ�3: �趨ģʽ */
#define TYBC_MODE_SET_TYPE                         (FIELD_ENUM)      /* ʹ�ã�FIELD_ENUM ���ã�FIELD_DISABLED */
#define TYBC_MODE_SET_TITLE                        (TY_MSGID_TIT_SET_MODE)               /* �ַ���ID */
#define TYBC_MODE_SET_VAL                          (TYBC_mode_set_to_rn_enum(sv_mode))/* ��ǰֵ */
#define TYBC_MODE_SET_RANGE                        (TYBC_get_mode_set_range())        /* ��Χ */
#define TYBC_MODE_SET_MAX_NUM                      (SET_MODE_MAX-1)                     /* ��ö���� */

/* �ֶ�4: ��ǰģʽ */
#define TYBC_MODE_CUR_TYPE                         (FIELD_ENUM)      /* ʹ�ã�FIELD_ENUM ���ã�FIELD_DISABLED */
#define TYBC_MODE_CUR_TITLE                        (TY_MSGID_NULL)                         /* �ַ���ID */
#define TYBC_MODE_CUR_VAL                          (TYBC_mode_set_to_rn_enum(ac_mode))  /* ��ǰֵ */
#define TYBC_MODE_CUR_RANGE                        (TYBC_get_mode_set_range())            /* ��Χ */
#define TYBC_MODE_CUR_MAX_NUM                      (SET_MODE_MAX-1)                         /* ��ö���� */

/* �ֶ�5: ����ĩ�� */
#define TYBC_CTRL_END_VALUE                        (ZONE_A_TERMINAL_DEVICE) //����һĩ������

/* �ֶ�6: ���ƶ��� */
#define TYBC_CTRL_OBJ                              (sh_main_info.zone_1_temp_type)

/* �ֶ�7: ѹ��״̬ */
#define TYBC_COMP_STA                              (sh_main_info.fg_zone_1_comp_on)

/* �ֶ�8: ��˪״̬ */
#define TYBC_DEFRO_STA                         	   (sh_main_info.zone_1_status == ZONE_STA_DEFRO)

/* �ֶ�9: ����״̬ */
#define TYBC_ANFREZ_STA                            (sh_main_info.zone_1_status == ZONE_STA_ANTIFREZ)

/* �ֶ�10: �ⲿ��Դ���״̬ */
#define TYBC_OUTHEAT_STA                           (sh_main_info.zone_1_addition== ZONE_ADDITION_HEAT)

/* �ֶ�11: ɱ��״̬ */
#define TYBC_STERILIZE_STA                         (0)

/* �ֶ�12: ��������״̬ */
#define TYBC_AUXHEAT_STA                           (sh_main_info.zone_1_addition == ZONE_ADDITION_ELEC)

/* �ֶ�13: ˮ�����״̬ */
#define TYBC_TANKHEAT_STA                          (0)

/* �ֶ�14: ����ģʽ״̬ */
#define TYBC_SILENT_STA                            (sh_main_info.zone_1_status == ZONE_STA_SILENT)

/* �ֶ�15: ����ģʽ״̬ */
#define TYBC_ENESAVING_STA                         (sh_main_info.zone_1_status == ZONE_STA_ECO)

/* �ֶ�16: ����״̬ */
#define TYBC_HOLIDAY_STA                           (sh_main_info.zone_1_status == ZONE_STA_HOLIDAY)

/* �ֶ�17: ̫�����ź�״̬ */
#define TYBC_SOLAR_STA                             (0)

/* ========================================================================================== */
/* ����һ���������                                                                           */
/* ========================================================================================== */
/* ���ػ����� */
#define TYBC_CMD_RUN                               (SHC_command_ON_zone_1_deal(TRUE))
#define TYBC_CMD_STOP                              (SHC_command_OFF_zone_1_deal(TRUE))

/* ========================================================================================== */
/* �����������Ϣ����                                                                         */
/* ========================================================================================== */
/* �ֶ�0: ���ػ� */
#define TYBC_ZoneB_SWITCH_TYPE                           (FIELD_BOOL)      /* ʹ�ã�FIELD_BOOL �ֶβ��ã�FIELD_DISABLED */
#define TYBC_ZoneB_SWITCH_TITLE                          (TY_MSGID_TIT_SWITCH)        /* �ַ���ID */
#define TYBC_ZoneB_SWITCH_VAL                            (sh_main_info.fg_zone_2_is_run)   /* ��ǰֵ */

/* �ֶ�1: �趨�¶� */
#define TYBC_ZoneB_TEMP_SET_TYPE                         (FIELD_NUMBER)    /* ʹ�ã�FIELD_NUMBER ���ã�FIELD_DISABLED */
#define TYBC_ZoneB_TEMP_SET_TITLE                        (TY_MSGID_TIT_SET_TEMP)	/* �ַ���ID */
#define TYBC_ZoneB_TEMP_SET_ADDR                         ((ac_mode==MODE_COOL)?P_idx(sv_cool):P_idx(sv_heat))	/* �������� */
#define TYBC_ZoneB_TEMP_SET_VAL                          (SHC_get_set_tmp_B())/* �趨ֵ */
#define TYBC_ZoneB_TEMP_SET_MAX                          (SHC_get_set_tmp_max_B())  /* ���� */
#define TYBC_ZoneB_TEMP_SET_MIN                          (SHC_get_set_tmp_min_B())  /* ���� */
#define TYBC_ZoneB_TEMP_SET_ATTR                         ((TYBC_cal_para_attrib(GROUP_USER, TYBC_ZoneB_TEMP_SET_ADDR) \
                                                   		 &(TYBC_UNIT|TYBC_DECIMAL))|STEP_1)  /* ����(ע�⣺�趨�¶Ȳ���ֵ�����ֵ����Сֵ��С����λ����һ��) */

/* �ֶ�2: ��ǰ�¶� */
#define TYBC_ZoneB_TEMP_CUR_TYPE                         (FIELD_RO_NUM)    /* ʹ�ã�FIELD_RO_NUM ���ã�FIELD_DISABLED */
#define TYBC_ZoneB_TEMP_CUR_TITLE                        (TY_MSGID_TIT_CUR_TEMP)                  /* �ַ���ID */
#define TYBC_ZoneB_TEMP_CUR_VAL                          (SHC_get_ctrl_tmp_B())	/* �ڶ����¶ȵ�ǰֵ */
#define TYBC_ZoneB_TEMP_CUR_ATTR                         ((TYBC_sign_local2sh(UINT_C|D_DOT1) \
                                                   		 &(TYBC_UNIT|TYBC_DECIMAL))|STEP_1)     /* ���� */

/* �ֶ�3: �趨ģʽ */
#define TYBC_ZoneB_MODE_SET_TYPE                         (FIELD_ENUM)      /* ʹ�ã�FIELD_ENUM ���ã�FIELD_DISABLED */
#define TYBC_ZoneB_MODE_SET_TITLE                        (TY_MSGID_TIT_SET_MODE)               /* �ַ���ID */
#define TYBC_ZoneB_MODE_SET_VAL                          (TYBC_mode_set_to_rn_enum(sv_mode))/* ��ǰֵ */
#define TYBC_ZoneB_MODE_SET_RANGE                        (TYBC_get_mode_set_range())        /* ��Χ */
#define TYBC_ZoneB_MODE_SET_MAX_NUM                      (SET_MODE_MAX-1)                     /* ��ö���� */

/* �ֶ�4: ��ǰģʽ */
#define TYBC_ZoneB_MODE_CUR_TYPE                         (FIELD_ENUM)      /* ʹ�ã�FIELD_ENUM ���ã�FIELD_DISABLED */
#define TYBC_ZoneB_MODE_CUR_TITLE                        (TY_MSGID_NULL)                         /* �ַ���ID */
#define TYBC_ZoneB_MODE_CUR_VAL                          (TYBC_mode_set_to_rn_enum(ac_mode))  /* ��ǰֵ */
#define TYBC_ZoneB_MODE_CUR_RANGE                        (TYBC_get_mode_set_range())            /* ��Χ */
#define TYBC_ZoneB_MODE_CUR_MAX_NUM                      (SET_MODE_MAX-1)                         /* ��ö���� */

/* �ֶ�5: ����ĩ�� */
#define TYBC_ZoneB_CTRL_END_VALUE                        (ZONE_B_TERMINAL_DEVICE) //�����ĩ������

/* �ֶ�6: ���ƶ��� */
#define TYBC_ZoneB_CTRL_OBJ                              (sh_main_info.zone_2_temp_type)

/* �ֶ�7: ѹ��״̬ */
#define TYBC_ZoneB_COMP_STA                              (sh_main_info.fg_zone_2_comp_on)

/* �ֶ�8: ��˪״̬ */
#define TYBC_ZoneB_DEFRO_STA                         	   (sh_main_info.zone_2_status == ZONE_STA_DEFRO)

/* �ֶ�9: ����״̬ */
#define TYBC_ZoneB_ANFREZ_STA                            (sh_main_info.zone_2_status == ZONE_STA_ANTIFREZ)

/* �ֶ�10: �ⲿ��Դ���״̬ */
#define TYBC_ZoneB_OUTHEAT_STA                           (sh_main_info.zone_2_addition== ZONE_ADDITION_HEAT)

/* �ֶ�11: ɱ��״̬ */
#define TYBC_ZoneB_STERILIZE_STA                         (0)

/* �ֶ�12: ��������״̬ */
#define TYBC_ZoneB_AUXHEAT_STA                           (sh_main_info.zone_2_addition == ZONE_ADDITION_ELEC)

/* �ֶ�13: ˮ�����״̬ */
#define TYBC_ZoneB_TANKHEAT_STA                          (0)

/* �ֶ�14: ����ģʽ״̬ */
#define TYBC_ZoneB_SILENT_STA                            (sh_main_info.zone_2_status == ZONE_STA_SILENT)

/* �ֶ�15: ����ģʽ״̬ */
#define TYBC_ZoneB_ENESAVING_STA                         (sh_main_info.zone_2_status == ZONE_STA_ECO)

/* �ֶ�16: ����״̬ */
#define TYBC_ZoneB_HOLIDAY_STA                           (sh_main_info.zone_2_status == ZONE_STA_HOLIDAY)

/* �ֶ�17: ̫�����ź�״̬ */
#define TYBC_ZoneB_SOLAR_STA                             (0)


/* ========================================================================================== */
/* ��������������                                                                           */
/* ========================================================================================== */
/* ���ػ����� */
#define TYBC_ZoneB_CMD_RUN                               (SHC_command_ON_zone_2_deal(TRUE))
#define TYBC_ZoneB_CMD_STOP                              (SHC_command_OFF_zone_2_deal(TRUE))

/* ========================================================================================== */
/* ��ˮ������Ϣ����                                                                         */
/* ========================================================================================== */
/* �ֶ�0: ���ػ� */
#define TYBC_DHW_SWITCH_TYPE                           	 (FIELD_BOOL)      /* ʹ�ã�FIELD_BOOL �ֶβ��ã�FIELD_DISABLED */
#define TYBC_DHW_SWITCH_TITLE                          	 (TY_MSGID_TIT_SWITCH)        /* �ַ���ID */
#define TYBC_DHW_SWITCH_VAL                              (sh_main_info.fg_zone_hotw_is_run)/* ��ǰֵ */

/* �ֶ�1: �趨�¶� */
#define TYBC_DHW_TEMP_SET_TYPE                           (FIELD_NUMBER)    /* ʹ�ã�FIELD_NUMBER ���ã�FIELD_DISABLED */
#define TYBC_DHW_TEMP_SET_TITLE                          (TY_MSGID_TIT_SET_DHW_TEMP)	/* �ַ���ID */
#define TYBC_DHW_TEMP_SET_ADDR                           (P_idx(sv_hotw))	/* �������� */
#define TYBC_DHW_TEMP_SET_VAL                            (sv_hotw/10)/* �趨ֵ */
#define TYBC_DHW_TEMP_SET_MAX                            (para_get_max(P_GaI(sv_hotw)))  /* ���� */
#define TYBC_DHW_TEMP_SET_MIN                            (para_get_min(P_GaI(sv_hotw)))  /* ���� */
#define TYBC_DHW_TEMP_SET_ATTR                           ((TYBC_cal_para_attrib(GROUP_USER, TYBC_DHW_TEMP_SET_ADDR) \
                                                   		 &(TYBC_UNIT|TYBC_DECIMAL))|STEP_1)  /* ����(ע�⣺�趨�¶Ȳ���ֵ�����ֵ����Сֵ��С����λ����һ��) */
/* �ֶ�2: ��ǰ�¶� */
#define TYBC_DHW_TEMP_CUR_TYPE                        	 (FIELD_RO_NUM)    /* ʹ�ã�FIELD_RO_NUM ���ã�FIELD_DISABLED */
#define TYBC_DHW_TEMP_CUR_TITLE                        	 (TY_MSGID_TIT_CUR_DHW_TEMP)                  /* �ַ���ID */
#define TYBC_DHW_TEMP_CUR_VAL                       	 (pv_sys_THWt)	/* �ܳ�ˮ�¶ȵ�ǰֵ */
#define TYBC_DHW_TEMP_CUR_ATTR                    		 ((TYBC_sign_local2sh(UINT_C|D_DOT1) \
                                                  		  &(TYBC_UNIT|TYBC_DECIMAL))|STEP_1)

/* �ֶ�3: �趨ģʽ */
#define TYBC_DHW_MODE_SET_TYPE                     		 (FIELD_ENUM)      /* ʹ�ã�FIELD_ENUM ���ã�FIELD_DISABLED */
#define TYBC_DHW_MODE_SET_TITLE                        	 (TY_MSGID_TIT_SET_MODE)               /* �ַ���ID */
#define TYBC_DHW_MODE_SET_VAL                            (TYBC_mode_set_to_rn_enum(sv_mode))/* ��ǰֵ */
#define TYBC_DHW_MODE_SET_RANGE                       	 (TYBC_get_mode_set_range())        /* ��Χ */
#define TYBC_DHW_MODE_SET_MAX_NUM                     	 (SET_MODE_MAX-1)                     /* ��ö���� */

/* �ֶ�4: ��ǰģʽ */
#define TYBC_DHW_MODE_CUR_TYPE                         	 (FIELD_ENUM)      /* ʹ�ã�FIELD_ENUM ���ã�FIELD_DISABLED */
#define TYBC_DHW_MODE_CUR_TITLE                        	 (TY_MSGID_NULL)                         /* �ַ���ID */
#define TYBC_DHW_MODE_CUR_VAL                          	 (TYBC_mode_set_to_rn_enum(ac_mode))  /* ��ǰֵ */
#define TYBC_DHW_MODE_CUR_RANGE                       	 (TYBC_get_mode_set_range())            /* ��Χ */
#define TYBC_DHW_MODE_CUR_MAX_NUM                      	 (SET_MODE_MAX-1)                         /* ��ö���� */

/* �ֶ�5: ����ĩ�� */
#define TYBC_DHW_CTRL_END_VALUE                          (ZONE_MODE_HOTW) //��ˮĩ������

/* �ֶ�6: ���ƶ��� */
#define TYBC_DHW_CTRL_OBJ                                (0)

/* �ֶ�7: ѹ��״̬ */
#define TYBC_DHW_COMP_STA                                (sh_main_info.fg_zone_hotw_comp_on)

/* �ֶ�8: ��˪״̬ */
#define TYBC_DHW_DEFRO_STA                         	     (sh_main_info.zone_hotw_status == ZONE_STA_DEFRO)

/* �ֶ�9: ����״̬ */
#define TYBC_DHW_ANFREZ_STA                           	 (sh_main_info.zone_hotw_status == ZONE_STA_ANTIFREZ)

/* �ֶ�10: �ⲿ��Դ���״̬ */
#define TYBC_DHW_OUTHEAT_STA                           	 (sh_main_info.zone_hotw_addition== ZONE_ADDITION_HEAT)

/* �ֶ�11: ɱ��״̬ */
#define TYBC_DHW_STERILIZE_STA                         	 (sh_main_info.fg_zone_hotw_disinfect)

/* �ֶ�12: ��������״̬ */
#define TYBC_DHW_AUXHEAT_STA                          	 (sh_main_info.zone_hotw_addition== ZONE_ADDITION_ELEC)

/* �ֶ�13: ˮ�����״̬ */
#define TYBC_DHW_TANKHEAT_STA                         	 (sh_main_info.zone_hotw_addition== ZONE_ADDITION_TANK_HEATER)

/* �ֶ�14: ����ģʽ״̬ */
#define TYBC_DHW_SILENT_STA                           	 (sh_main_info.zone_hotw_status == ZONE_STA_SILENT)

/* �ֶ�15: ����ģʽ״̬ */
#define TYBC_DHW_ENESAVING_STA                        	 (sh_main_info.zone_hotw_status == ZONE_STA_ECO)

/* �ֶ�16: ����״̬ */
#define TYBC_DHW_HOLIDAY_STA                          	 (sh_main_info.zone_hotw_status == ZONE_STA_HOLIDAY)

/* �ֶ�17: ̫�����ź�״̬ */
#define TYBC_DHW_SOLAR_STA                             	 (sh_main_info.zone_hotw_addition== ZONE_ADDITION_SOLAR)


/* ========================================================================================== */
/* ��ˮ���������                                                                           */
/* ========================================================================================== */
/* ���ػ����� */
#define TYBC_DHW_CMD_RUN                              		(SHC_command_ON_hotwater_deal(TRUE))
#define TYBC_DHW_CMD_STOP                              		(SHC_command_OFF_hotwater_deal(TRUE))


/* ========================================================================================== */
/* ��չ������Ϣ����                                                                           */
/* ========================================================================================== */
/* ��"���ٹ���" */
#ifdef TUYA_USE_WIND
/* �ֶ�0: �趨���� */
#define TYBC_WIND_SET_TYPE                         (FIELD_ENUM)      /* ʹ�ã�FIELD_ENUM ���ã�FIELD_DISABLED */
#define TYBC_WIND_SET_TITLE                        (msg_tyb_title_wind_set)
#define TYBC_WIND_SET_RANGE                        (TYBC_get_wind_set_range())
#define TYBC_WIND_SET_MAX_NUM                      (RN_WIND_MAX)
#define TYBC_WIND_SET_VAL                          (TYBC_wind_set_to_rn_enum(0))

/* �ֶ�1: ��ǰ���� */
#define TYBC_WIND_CUR_TYPE                         (FIELD_ENUM)      /* ʹ�ã�FIELD_ENUM ���ã�FIELD_DISABLED */
#define TYBC_WIND_CUR_TITLE                        (msg_tyb_title_wind_cur)
#define TYBC_WIND_CUR_RANGE                        (TYBC_get_wind_cur_range())
#define TYBC_WIND_CUR_MAX_NUM                      (RN_WIND_MAX)
#define TYBC_WIND_CUR_VAL                          (TYBC_wind_cur_to_rn_enum(0))
#endif

/* ��"ʪ�ȹ���" */
#ifdef TUYA_USE_HUMI
/* �ֶ�2: �趨ʪ��(Ԥ��,δ����) */
#define TYBC_HUMI_SET_TYPE                         (FIELD_NUMBER)    /* ʹ�ã�FIELD_NUMBER ���ã�FIELD_DISABLED */
#define TYBC_HUMI_SET_TITLE                        (msg_tyb_null)
#define TYBC_HUMI_SET_VAL                          (20)
#define TYBC_HUMI_SET_MAX                          (100)
#define TYBC_HUMI_SET_MIN                          (0)
#define TYBC_HUMI_SET_DECIMAL                      (0)
/* ע�⣺�趨ʪ�Ȳ���ֵ�����ֵ����Сֵ��С����λ����һ�� */
#define TYBC_HUMI_SET_ATTR                         (((6)&(TYBC_UNIT|TYBC_DECIMAL))|STEP_1)

/* �ֶ�3: ��ǰʪ��(Ԥ��,δ����) */
#define TYBC_HUMI_CUR_TYPE                         (FIELD_RO_NUM)    /* ʹ�ã�FIELD_RO_NUM ���ã�FIELD_DISABLED */
#define TYBC_HUMI_CUR_TITLE                        (msg_tyb_null)
#define TYBC_HUMI_CUR_VAL                          (10)
#define TYBC_HUMI_CUR_ATTR                         (((6)&(TYBC_UNIT|TYBC_DECIMAL))|STEP_1)
#endif

/* ��"��ˮ����" */
#ifdef TUYA_USE_HOT_WATER
/* �ֶ�4: ��ˮ�趨�¶� */
#define TYBC_HOT_WATER_SET_TYPE                    (FIELD_NUMBER)    /* ʹ�ã�FIELD_NUMBER ���ã�FIELD_DISABLED */
#define TYBC_HOT_WATER_SET_TITLE                   (msg_tyb_title_hot_water_set)
#define TYBC_HOT_WATER_SET_VAL                     (sv_hotw/10)	/* ��ˮ�趨�¶� */
#define TYBC_HOT_WATER_SET_MAX                     (HIEST_HOTW)
#define TYBC_HOT_WATER_SET_MIN                     (LOEST_HOTW)
#define TYBC_HOT_WATER_SET_ATTR                    ((TYBC_cal_para_attrib(P_grp(sv_hotw),P_idx(sv_hotw)) \
                                                   &(TYBC_UNIT|TYBC_DECIMAL))|STEP_1)

/* �ֶ�5: ��ǰ��ˮ�¶� */
#define TYBC_HOT_WATER_CUR_TYPE                    (FIELD_RO_NUM)    /* ʹ�ã�FIELD_RO_NUM ���ã�FIELD_DISABLED */
#define TYBC_HOT_WATER_CUR_TITLE                   (msg_tyb_title_hot_water_cur)
#define TYBC_HOT_WATER_CUR_VAL                     (pv_sys_THWt)	/* ��ˮˮ���¶� */
#define TYBC_HOT_WATER_CUR_ATTR                    ((TYBC_sign_local2sh(UINT_C|D_DOT1) \
                                                   &(TYBC_UNIT|TYBC_DECIMAL))|STEP_1)
#endif


/* ========================================================================================== */
/* ״̬��ѯ��������                                                                           */
/* ========================================================================================== */
/* ��"״̬��ѯ" */
#if (defined(TUYA_USE_SYS_STAT) || defined(TUYA_USE_MOD_STAT))
#define TYBC_PV_VAL(pcb_idx,pv_idx)                (TYBC_get_pv_val(pcb_idx,pv_idx))                   /* ģ������ֵ */
#endif

/* ��"ϵͳ״̬" */
#ifdef TUYA_USE_SYS_STAT
/* ϵͳģ���� */
#define TYBC_SYS_PV_TITLE(idx)                     (msg_sys_p[idx].msg)                              /* ���� */
#define TYBC_SYS_PV_ATTR(idx)                      (TYBC_sign_local2sh(msg_sys_p[(idx)].attr))        /* ���� */
#define TYBC_SYS_PV_IN_USED                        (get_sys_p_num())                                 /* ʵ��ʹ�ø��� */
#endif

/* ��"ģ��״̬" */
#ifdef TUYA_USE_MOD_STAT
/* ģ�鵥Ԫģ���� */
#define TYBC_UNIT_PV_TITLE(idx)                    (msg_uint_p_cell[idx].msg)                        /* ���� */
#define TYBC_UNIT_PV_ATTR(idx)                     (TYBC_sign_local2sh(msg_uint_p_cell[(idx)].attr))  /* ���� */
#define TYBC_UNIT_PV_IN_USED                       (get_unit_p_num())                                /* ʵ��ʹ�ø��� */
/* ģ��ѹ��ģ���� */
#define TYBC_COMP_PV_TITLE(idx)                    (msg_uint_p_comp[idx].msg)                        /* ���� */
#define TYBC_COMP_PV_ATTR(idx)                     (TYBC_sign_local2sh(msg_uint_p_comp[idx].attr))    /* ���� */
#define TYBC_COMP_PV_IN_USED                       (get_comp_p_num())                                /* ʵ��ʹ�ø��� */
#endif


/* ��"��ǰ����" */
#ifdef TUYA_USE_ERR_CUR
/* ========================================================================================== */
/* ��ѯ��ǰ���Ϲ�������                                                                       */
/* ========================================================================================== */
#define TYBC_GET_ERR_CUR(err,s_e_id,m_id,m_e_id)   (TYBC_get_err_cur(err, s_e_id, m_id, m_e_id))  /* ��ȡ��ǰ���� */
#define TYBC_CUR_ERR_NUM                           (TYBC_get_err_cur_num())                       /* ��ǰ������ */
#endif


/* ��"��ʷ����" */
#ifdef TUYA_USE_ERR_HIS
/* ========================================================================================== */
/* ��ѯ��ʷ���Ϲ�������                                                                       */
/* ========================================================================================== */
#define TYBC_GET_ERR_HIS(err,idx)                  (TYBC_get_err_his_info(err, idx))  /* ��ȡһ����ʷ������Ϣ */
#define TYBC_HIS_ERR_TBL_SIZE                      (HIS_ERR_SIZE)
#define TYBC_HIS_ERR_MAX                           (MAX_HIS_ERROR)
#endif


/* ========================================================================================== */
/* �������ù�������                                                                           */
/* ========================================================================================== */
/* ��"��������" */
#ifdef TUYA_USE_PARAM_SET
/* ������ */
#define TYBC_PARAM_GRP_TITLE(grp_id)               (para_large[grp_id])     /* ���� */
#define TYBC_PARAM_GRP_ATTR(grp_id)                (TYBC_cal_grp_attrib((U08)grp_id))       /* ���� */
#define TYBC_PARAM_GRP_NUM                         (PARA_GROUP_NUM)                         /* ʵ��ʹ�ø��� */
#define TYBC_PARAM_ITEM_NUM(grp_id)                (ParamPri[grp_id].items)                 /* �������еĲ���ʵ��ʹ�õĸ���������ParamPri[x].items */
/* ������ */
#define TYBC_PARAM_ITEM_TITLE(grp_id,item_id)      (para_small[(grp_id)][(item_id)].msg)                 /* ���� */
#define TYBC_PARAM_ITEM_VAL(grp_id,item_id)        (para_get_val(grp_id, item_id))                    /* ��ǰֵ */
#define TYBC_PARAM_MIN(grp_id,item_id)             (para_get_min(grp_id,item_id))   /* ��Сֵ */
#define TYBC_PARAM_MAX(grp_id,item_id)             (para_get_max(grp_id,item_id))   /* ���ֵ */
#define TYBC_PARAM_ITEM_ATTR(grp_id,item_id)       (TYBC_cal_para_attrib(grp_id,item_id))                /* ���� */
#define TYBC_PARAM_DICT_NUM                        (get_dict_num())                                      /* ����ѡ���ַ������� */
#define TYBC_PARAM_DICT_ITEM_NUM(dict_idx)         (para_dict[dict_idx].items)                           /* һ��������ѡ���ַ������� */
#define TYBC_PARAM_DICT_ITEM(dict_idx, idx)        (para_dict[dict_idx].pmsg[idx])                       /* ����ѡ�������ַ��� */
#define TYBC_SET_PARAM(grp_id,item_id,value)       (para_set(grp_id,item_id,value)) /* ���ò���ֵ */
#endif


/* ֻ֧�ִӻ��� */
#if defined(TUYA_USE_HMI_SLAVE) && !defined(TUYA_USE_HMI_MASTER)
/* ========================================================================================== */
/* ע:�˲������ݲο���SmartHmi���˴�������SmartHmi���°汾Ϊ׼��һ�������޸�                  */
/* ========================================================================================== */
#define TYBC_GET_M_ERR(err)                        (msg_m_error[err])/* ϵͳ�������� */
#define TYBC_GET_S_ERR(err)                        (msg_s_error[err])/* ģ��������� */

/* ����ʱ�� */
typedef struct tag_TYBC_ERR_TIME
{
    U16 month;                                     /* �� (����ͬΪ0����ʾ����) */
    U16 day;                                       /* �� */
    U16 hour;                                      /* ʱ (ʱ��ͬΪ0����ʾʱ��) */
    U16 mint;                                      /* �� */
} TYBC_ERR_TIME;

/* ��ʷ������Ϣ */
typedef struct tag_TYBC_ERR_HIS
{
    U16 name;                                      /* �����ַ����� */
    U16 id;                                        /* ���(��8λΪ��ţ���8λΪ��Ԫ�ţ�Ϊ0xffʱ����ʾ���) */
    U16 err_id;                                    /* ���Ϻ� */
    TYBC_ERR_TIME time;                            /* ����ʱ�� */
} TYBC_ERR_HIS;
#define TYBC_ERR_WEI_ZHI_GZ                        (0)

/* ��ǰ������Ϣ */
typedef struct tag_TYBC_ERR_CUR
{
    U16 name;                                     /* �����ַ����� */
    U16 id;                                       /* ���(��8λΪ��ţ���8λΪ��Ԫ�ţ�Ϊ0xffʱ����ʾ���) */
    U16 err_id;                                   /* ���Ϻ� */
} TYBC_ERR_CUR;

/* ��λ */
#define TYBC_UNIT_NULL                             (0)       /* 0 �� */
#define TYBC_UNIT_TMP_C                            (1)       /* 1 �¶�(��) */
#define TYBC_UNIT_TMP_F                            (2)       /* 2 �¶�(�H) */
#define TYBC_UNIT_A                                (3)       /* 3 ����(A) */
#define TYBC_UNIT_mA                               (4)       /* 4 ����(mA) */
#define TYBC_UNIT_PRS_B                            (5)       /* 5 ѹ��(bar) */
#define TYBC_UNIT_PE                               (6)       /* 6 �ٷֱ�(%) */
#define TYBC_UNIT_HZ                               (7)       /* 7 Ƶ��(HZ) */
#define TYBC_UNIT_RPS                              (8)       /* 8 ת��(rps) */
#define TYBC_UNIT_RPM                              (9)       /* 9 ת��(rpm) */
#define TYBC_UNIT_YEAR                             (10)      /* 10 �� */
#define TYBC_UNIT_MONTH                            (11)      /* 11 �� */
#define TYBC_UNIT_DAY                              (12)      /* 12 �� */
#define TYBC_UNIT_HOUR                             (13)      /* 13 ʱ */
#define TYBC_UNIT_MINT                             (14)      /* 14 �� */
#define TYBC_UNIT_SECOND                           (15)      /* 15 �� */
#define TYBC_UNIT_ST                               (16)      /* 16 �� */
#define TYBC_UNIT_V                                (17)      /* 17 ��ѹ(V) */
#define TYBC_UNIT_mV                               (18)      /* 18 ��ѹ(mV) */
#define TYBC_UNIT_kO                               (19)      /* 19 ����(k��) */
#define TYBC_UNIT_kW                               (20)      /* 20 ����(kW) */
#define TYBC_UNIT_kWH                              (21)      /* 20 �ܺ�(kWH) */
#define TYBC_UNIT_MAX                              (22)      /* ���λ�� */

/* ����Ȩ�� */
#define TYBC_PWR_NONE                              (0)       /* δ��¼ */
#define TYBC_PWR_SALE                              (1)       /* ������(ʹ������) */
#define TYBC_PWR_USER                              (2)       /* �û� */
#define TYBC_PWR_PRO                               (4)       /* ���̵���Ա */
#define TYBC_PWR_SER                               (6)       /* ����ά��Ա */
#define TYBC_PWR_FAC                               (9)       /* ���ҹ���Ա */
#define TYBC_PWR_SPR                               (10)      /* �������� */

/* ����Ȩ�� */
#define TYBC_PARA_USR                              (0)       /* �û� */
#define TYBC_PARA_PRO                              (1)       /* ���� */
#define TYBC_PARA_SER                              (2)       /* ά�� */
#define TYBC_PARA_FAC                              (3)       /* ���� */

#define TYBC_UNIT_OFFSET                           (32)      /* ��λƫ���� */
#define TYBC_DECIMAL_BGN_BIT                       (8)       /* С��λ�� ��ʼλ */
#define TYBC_HIDE_BGN_BIT                          (10)      /* �������� ��ʼλ */
#define TYBC_CONST_BGN_BIT                         (11)      /* �����޸� ��ʼλ */
#define TYBC_SAFE_BGN_BIT                          (12)      /* ����Ȩ�� ��ʼλ */
#define TYBC_BIT_BGN_BIT                           (15)      /* λ����   ��ʼλ */

#define TYBC_UNIT                                  (0x00ff)  /* bit0~7:   0~255����ֵС��32,��ʾΪ��ֵ�ͣ�ֵΪ��λ�ţ����֧��32����λ����ֵ����32����ʾΪ�ַ����Ͳ�����ֵ��ȥƫ��ֵ32֮������ַ���� */
#define TYBC_DECIMAL                               (0x0300)  /* bit8~9:   С��λ��(0~3) */

#define TYBC_NO_ID                                 (0xff)    /* 0xff����ʾ��� */

/* ͬʱ֧�ִӻ���+������ʱ���������ݾ���SmartHmi������ */
#elif (defined(TUYA_USE_HMI_SLAVE) && defined(TUYA_USE_HMI_MASTER))
typedef SH_ERR_HIS TYBC_ERR_HIS;
typedef SH_ERR_CURT TYBC_ERR_CUR;
#define TYBC_UNIT                                  (SH_UNIT)
#define TYBC_DECIMAL                               (SH_DECIMAL)
#define TYBC_sign_local2sh(sign_local)             ((((U16)sign_local) == 0xFFFF) ? (0xFFFE)\
                                                                           : SH_sign_local2sh((U16)sign_local))
#define TYBC_cal_grp_attrib(grp_id)                (SH_cal_grp_attrib((U08)grp_id))
#define TYBC_cal_para_attrib(grp_id,item_id)       (SH_cal_para_attrib((U08)grp_id,(U08)item_id))
#define TYBC_get_err_cur(e, s_e_id, m_id, m_e_id)  (SHC_get_curt_err((TYBC_ERR_CUR*)e,(U16*)s_e_id,(U16*)m_id,(U16*)m_e_id))
#define TYBC_get_err_cur_num()                     (SHC_get_err_num())
#define TYBC_get_err_his_info(err,idx)             (SHC_get_his_err_info((TYBC_ERR_HIS*)err,(U08)idx))
#endif


/* �������������޸� */
TUYA_B_CFG_EXT U08 TYBC_language_to_rn_enum(U08 local_language);
TUYA_B_CFG_EXT U08 TYBC_language_to_local_enum(U08 rn_language);
TUYA_B_CFG_EXT U08 *TYBC_get_password_pointer(void);
TUYA_B_CFG_EXT BOOL TYBC_check_password_modified(void);
TUYA_B_CFG_EXT U16 *TYBC_get_language_range(void);
TUYA_B_CFG_EXT void TYBC_language_set(U08 val);
TUYA_B_CFG_EXT BOOL TYBC_get_switch_status(void);
TUYA_B_CFG_EXT U08 TYBC_mode_set_to_rn_enum(U08 local_mode);
TUYA_B_CFG_EXT U16 *TYBC_get_mode_set_range(void);
TUYA_B_CFG_EXT U08 TYBC_run_status_to_rn_enum(U08 local_enum);
TUYA_B_CFG_EXT void TYBC_temp_set(I16 cloud_val, U08 dpid);
TUYA_B_CFG_EXT void TYBC_mode_set(I16 cloud_val);
TUYA_B_CFG_EXT U16 *TYBC_get_wind_set_range(void);
TUYA_B_CFG_EXT U08 TYBC_wind_set_to_rn_enum(U08 local_wind_speed);
TUYA_B_CFG_EXT U16 *TYBC_get_wind_cur_range(void);
TUYA_B_CFG_EXT U08 TYBC_wind_cur_to_rn_enum(U08 local_wind_speed);
TUYA_B_CFG_EXT void TYBC_wind_set(I16 cloud_val);
TUYA_B_CFG_EXT void TYBC_humi_set(I16 cloud_val);
TUYA_B_CFG_EXT void TYBC_hot_water_set(I16 cloud_val);
TUYA_B_CFG_EXT void TYBC_config_timer(void);
TUYA_B_CFG_EXT void TYBC_config_init(void);
#ifndef TUYA_USE_HMI_MASTER
TUYA_B_CFG_EXT U16 TYBC_sign_local2sh(U16 sign_local);
/* ��"��ǰ����" */
#ifdef TUYA_USE_ERR_CUR
TUYA_B_CFG_EXT BOOL TYBC_get_err_cur(TYBC_ERR_CUR *err, U16 *sys_err_id, U16 *mod_id, U16 *mod_err_id);
TUYA_B_CFG_EXT U16 TYBC_get_err_cur_num(void);
#endif
/* ��"��ʷ����" */
#ifdef TUYA_USE_ERR_HIS
TUYA_B_CFG_EXT void TYBC_get_err_his_info(TYBC_ERR_HIS *err, U08 idx);
#endif
/* ��"��������" */
#ifdef TUYA_USE_PARAM_SET
TUYA_B_CFG_EXT U16 TYBC_cal_grp_attrib(U08 grp);
TUYA_B_CFG_EXT U16 TYBC_cal_para_attrib(U08 grp, U08 item);
#endif
#endif

TUYA_B_CFG_EXT I16 TYBC_get_pv_val(U16 pcb_idx, U16 pv_idx);
TUYA_B_CFG_EXT U16 *TYBC_get_temp_unit_range(void);

