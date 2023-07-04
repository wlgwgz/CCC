/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: Tuya_B_SLP_HLxxx_MULTI.c
 �ļ�����: (ůͨHLϵ��)Ϳѻ(B)������ҵ���߼�Դ�ļ�
 ������  : �ֺƱ�
 ��������: 2020.12.13
*******************************************************************************/

/* ========================================================================================== */
/* Ϳ ѻ �� �� ͨ Ѷ Э �� �� �� �� �� (�����������г���һ�£����������޸�!!!!!)              */
/* ========================================================================================== */
/* Ϳѻ�߼�ҵ��汾 */
#define TYB_SLP_CORE_VER                 "1.0.0"

/* ���Э��汾 */
#define TYB_SLP_PANEL_VER                "1.0.0"
#define TYB_SLP_PROTOCOL_VER             TYB_SLP_PANEL_VER""TUYA_ARGS

/* ���ܵ�ID(DPID) (��Ч���ܵ�ID��1��ʼ��ţ����199) */
#define DPID_NULL                       (0)    /* ��Ч���ܵ�(����������HMI�󲻻��ϱ�ģ�顢�ɹ�HMI�·���ʾ���汾ʹ��) */
#define DPID_SWITCH                     (1)    /* ���� (֧���ƶ�ʱ & ��������) */
#define DPID_MODE                       (2)    /* �趨ģʽ (֧����������) */
#define DPID_TEMP_SET                   (16)   /* �趨�¶� (֧����������) */
#define DPID_TEMP_CUR                   (24)   /* ��ǰ�¶� (֧����������) */
#define DPID_VER_INFO                   (101)  /* �汾��Ϣ (��ʾ��&������) */
#define DPID_SYS_INFO                   (102)  /* ϵͳ��Ϣ */
#define DPID_PROTOCOL_VER               (103)  /* Э��汾 */
#define DPID_GENERAL_SET_INFO           (104)  /* ͨ��������Ϣ */
#define DPID_GENERAL_SET_CMD            (105)  /* ͨ���������� */
#define DPID_COMMON_FUNC_INFO           (106)  /* ���湦����Ϣ */
#define DPID_COMMON_FUNC_CMD            (107)  /* ���湦������ */
#define DPID_EXTEND_FUNC_INFO           (108)  /* ��չ������Ϣ */
#define DPID_EXTEND_FUNC_CMD            (109)  /* ��չ�������� */

#define DPID_ZONEA_FUNC_INFO            (106)  /* ����һ������Ϣ */
#define DPID_ZONEA_FUNC_CMD             (107)  /* ����һ�������� */
#define DPID_ZONEB_FUNC_INFO            (108)  /* �����������Ϣ */
#define DPID_ZONEB_FUNC_CMD             (109)  /* ������������� */
#define DPID_DHW_FUNC_INFO              (110)  /* ��ˮ������Ϣ */
#define DPID_DHW_FUNC_CMD               (111)  /* ��ˮ�������� */

#define DPID_SYS_STAT_INFO_1            (112)  /* ϵͳ״̬��Ϣ1(ԭ��ģ���"ϵͳ״̬����1") */
#define DPID_SYS_STAT_INFO_2            (113)  /* ϵͳ״̬��Ϣ2(ԭ��ģ���"ϵͳ״̬����") */
#define DPID_SYS_STAT_INFO_3            (114)  /* ϵͳ״̬��Ϣ3(ԭ��ģ���"ģ��״̬����1") */
#define DPID_SYS_STAT_INFO_4            (115)  /* ϵͳ״̬��Ϣ4(ԭ��ģ���"ģ��״̬����2") */
#define DPID_SYS_STAT_DATA              (116)  /* ϵͳ״̬����(ԭ��ģ���"00#ģ��״̬����") */
#define DPID_MOD_STAT_CMD               (117)  /* ģ��״̬�������� */
#define DPID_DATA_CHANNEL_01            (118)  /* ����ͨ��01 */
#define DPID_DATA_CHANNEL_02            (119)  /* ����ͨ��02 */
#define DPID_DATA_CHANNEL_03            (120)  /* ����ͨ��03 */
#define DPID_DATA_CHANNEL_04            (121)  /* ����ͨ��04 */
#define DPID_DATA_CHANNEL_05            (122)  /* ����ͨ��05 */
#define DPID_DATA_CHANNEL_06            (123)  /* ����ͨ��06 */
#define DPID_ERR_CUR_INFO               (133)  /* ��ǰ������Ϣ(ע: ��ǰ����ֻ�б��⣬û��ʱ��) */
#define DPID_ERR_HIS_INFO               (134)  /* ��ʷ������Ϣ */
#define DPID_ELE_STAT_CMD               (136)  /* �õ�״̬�������� */
#define DPID_SYS_STAT_CMD               (137)  /* ϵͳ״̬�������� */
#define DPID_ERR_HIS_CMD                (139)  /* ��ʷ���ϴ������� */
#define DPID_PARAM_CMD                  (140)  /* ������������ */

/* ���ܵ�λ������ */
enum DPID_IDX
{
    PROTOCOL_VER = 0,
    VER_INFO,
    SYS_INFO,
/* ��"ͨ�����ù���" */
#ifdef TUYA_HAS_GEN_SET
    GENERAL_SET_INFO,
    GENERAL_SET_CMD,
#endif

#if 0
    COMMON_FUNC_INFO,
    COMMON_FUNC_CMD,
/* ��"��չ����" */
#ifdef TUYA_HAS_EXT_FUNC
    EXTEND_FUNC_INFO,
    EXTEND_FUNC_CMD,
#endif
#endif
	ZONEA_FUNC_INFO,
	ZONEA_FUNC_CMD,
	ZONEB_FUNC_INFO,
	ZONEB_FUNC_CMD,
	DHW_FUNC_INFO,
	DHW_FUNC_CMD,

    STD_SWITCH,
    STD_MODE,
    STD_TEMP_SET,
    STD_TEMP_CUR,
/* ��"ϵͳ״̬" */
#ifdef TUYA_USE_SYS_STAT
    SYS_STAT_INFO_1,
    SYS_STAT_INFO_2,
    SYS_STAT_INFO_3,
    SYS_STAT_INFO_4,
    SYS_STAT_DATA,
#endif
/* ��"ģ��״̬" */
#ifdef TUYA_USE_MOD_STAT
    MOD_STAT_CMD,
#endif
/* ��"��ǰ����" */
#ifdef TUYA_USE_ERR_CUR
    ERR_CUR_INFO,
#endif
/* ��"��ʷ����" */
#ifdef TUYA_USE_ERR_HIS
    ERR_HIS_INFO,
#endif
/* ��ͨ������(��"��������"/"ģ��״̬") */
#ifdef TUYA_HAS_TRANS
    DATA_CHANNEL_01,
    DATA_CHANNEL_02,
    DATA_CHANNEL_03,
    DATA_CHANNEL_04,
    DATA_CHANNEL_05,
    DATA_CHANNEL_06,
#endif
/* ��"��������" */
#ifdef TUYA_USE_PARAM_SET
    PARAM_CMD,
#endif
/* ��"�õ��ѯ" */
#ifdef TUYA_USE_ELE_STAT
    ELE_STA_CMD,
#endif
    DPID_IDX_MAX,
};
/* ���ܵ��������� */
const TUYA_DP_PROPS DP[] =
{
    { DPID_PROTOCOL_VER,     DP_RAW,    DATA_RO, FUNC_CUSTOM,   FREQ_VERY_LOW,  }, /* DPID_103: Э��汾 */
    { DPID_VER_INFO,         DP_RAW,    DATA_RO, FUNC_CUSTOM,   FREQ_VERY_LOW,  }, /* DPID_101: �汾��Ϣ */
    { DPID_SYS_INFO,         DP_RAW,    DATA_RO, FUNC_CUSTOM,   FREQ_NORMAL,    }, /* DPID_102: ϵͳ��Ϣ */
/* ��"ͨ�����ù���" */
#ifdef TUYA_HAS_GEN_SET
    { DPID_GENERAL_SET_INFO, DP_RAW,    DATA_RO, FUNC_CUSTOM,   FREQ_NORMAL,    }, /* DPID_104: ͨ��������Ϣ */
    { DPID_GENERAL_SET_CMD,  DP_RAW,    DATA_WO, FUNC_CUSTOM,   FREQ_VERY_LOW,  }, /* DPID_105: ͨ���������� */
#endif
    { DPID_ZONEA_FUNC_INFO, DP_RAW,    DATA_RO, FUNC_CUSTOM,   FREQ_VERY_FAST, }, /* DPID_106: ����һ������Ϣ */
    { DPID_ZONEA_FUNC_CMD,  DP_RAW,    DATA_WO, FUNC_CUSTOM,   FREQ_VERY_LOW,  }, /* DPID_107: ����һ�������� */
    { DPID_ZONEB_FUNC_INFO, DP_RAW,    DATA_RO, FUNC_CUSTOM,   FREQ_VERY_FAST, }, /* DPID_108: �����������Ϣ */
    { DPID_ZONEB_FUNC_CMD,  DP_RAW,    DATA_WO, FUNC_CUSTOM,   FREQ_VERY_LOW,  }, /* DPID_109: ������������� */
    { DPID_DHW_FUNC_INFO,   DP_RAW,    DATA_RO, FUNC_CUSTOM,   FREQ_VERY_FAST, }, /* DPID_110: ��ˮ������Ϣ */
    { DPID_DHW_FUNC_CMD,    DP_RAW,    DATA_WO, FUNC_CUSTOM,   FREQ_VERY_LOW,  }, /* DPID_111: ��ˮ�������� */

    { DPID_SWITCH,           DP_BOOL,   DATA_RW, FUNC_STANDARD, FREQ_VERY_LOW,  }, /* DPID_1:   ���� */
    { DPID_MODE,             DP_ENUM,   DATA_RW, FUNC_STANDARD, FREQ_VERY_LOW,  }, /* DPID_2:   �趨ģʽ */
    { DPID_TEMP_SET,         DP_NUMBER, DATA_RW, FUNC_STANDARD, FREQ_VERY_LOW,  }, /* DPID_16:  �趨�¶� */
    { DPID_TEMP_CUR,         DP_NUMBER, DATA_RO, FUNC_STANDARD, FREQ_VERY_FAST, }, /* DPID_24:  ��ǰ�¶� */
/* ��"ϵͳ״̬" */
#ifdef TUYA_USE_SYS_STAT
    { DPID_SYS_STAT_INFO_1,  DP_RAW,    DATA_RO, FUNC_CUSTOM,   FREQ_LOW,       }, /* DPID_112: ϵͳ״̬��Ϣ1 */
    { DPID_SYS_STAT_INFO_2,  DP_RAW,    DATA_RO, FUNC_CUSTOM,   FREQ_LOW,       }, /* DPID_113: ϵͳ״̬��Ϣ2 */
    { DPID_SYS_STAT_INFO_3,  DP_RAW,    DATA_RO, FUNC_CUSTOM,   FREQ_LOW,       }, /* DPID_114: ϵͳ״̬��Ϣ3 */
    { DPID_SYS_STAT_INFO_4,  DP_RAW,    DATA_RO, FUNC_CUSTOM,   FREQ_LOW,       }, /* DPID_115: ϵͳ״̬��Ϣ4 */
    { DPID_SYS_STAT_DATA,    DP_RAW,    DATA_RO, FUNC_CUSTOM,   FREQ_FAST,      }, /* DPID_116: ϵͳ״̬���� */
#endif
/* ��"ģ��״̬" */
#ifdef TUYA_USE_MOD_STAT
    { DPID_MOD_STAT_CMD,     DP_RAW,    DATA_WO, FUNC_CUSTOM,   FREQ_VERY_LOW,  }, /* DPID_117: ģ��״̬�������� */
#endif
/* ��"��ǰ����" */
#ifdef TUYA_USE_ERR_CUR
    { DPID_ERR_CUR_INFO,     DP_RAW,    DATA_RO, FUNC_CUSTOM,   FREQ_LOW,       }, /* DPID_133: ��ǰ���ϱ��� */
#endif
/* ��"��ʷ����" */
#ifdef TUYA_USE_ERR_HIS
    { DPID_ERR_HIS_INFO,     DP_RAW,    DATA_RO, FUNC_CUSTOM,   FREQ_NORMAL,    }, /* DPID_134: ��ʷ���ϱ��� */
    { DPID_ERR_HIS_CMD,      DP_RAW,    DATA_RO, FUNC_CUSTOM,   FREQ_VERY_LOW,  }, /* DPID_139: ��ʷ���ϴ������� */
#endif
/* ��ͨ������(��"��������"/"ģ��״̬") */
#ifdef TUYA_HAS_TRANS
    { DPID_DATA_CHANNEL_01,  DP_RAW,    DATA_RO, FUNC_CHANNEL,  FREQ_INFINITE,  }, /* DPID_118: ����ͨ��01 */
    { DPID_DATA_CHANNEL_02,  DP_RAW,    DATA_RO, FUNC_CHANNEL,  FREQ_INFINITE,  }, /* DPID_119: ����ͨ��02 */
    { DPID_DATA_CHANNEL_03,  DP_RAW,    DATA_RO, FUNC_CHANNEL,  FREQ_INFINITE,  }, /* DPID_120: ����ͨ��03 */
    { DPID_DATA_CHANNEL_04,  DP_RAW,    DATA_RO, FUNC_CHANNEL,  FREQ_INFINITE,  }, /* DPID_121: ����ͨ��04 */
    { DPID_DATA_CHANNEL_05,  DP_RAW,    DATA_RO, FUNC_CHANNEL,  FREQ_INFINITE,  }, /* DPID_122: ����ͨ��05 */
    { DPID_DATA_CHANNEL_06,  DP_RAW,    DATA_RO, FUNC_CHANNEL,  FREQ_INFINITE,  }, /* DPID_123: ����ͨ��06 */
#endif
/* ��"��������" */
#ifdef TUYA_USE_PARAM_SET
    { DPID_PARAM_CMD,        DP_RAW,    DATA_WO, FUNC_CUSTOM,   FREQ_VERY_LOW,  }, /* DPID_140: ������������ */
#endif
/* ��"�õ��ѯ" */
#ifdef TUYA_USE_ELE_STAT
    { DPID_ELE_STAT_CMD,     DP_RAW,    DATA_WO, FUNC_CUSTOM,   FREQ_VERY_LOW,  }, /* DPID_136: �õ紫������ */
#endif
};
#define DPID_USED                       (sizeof(DP)/sizeof(TUYA_DP_PROPS))    /* ��ʹ�õĹ��ܵ���� */

/* ���ܵ��ϱ���ر�־ */
U16 fg_dpid_upload_pri_high[_DIVUCARRY(DPID_USED, 16)];       /* �����ȼ��ϱ���־ */
U16 fg_dpid_upload_pri_low[_DIVUCARRY(DPID_USED, 16)];        /* �����ȼ��ϱ���־ */

/* �ֶι��ܶ��� */
enum DP_FIELD_FUNC_ID
{
    FIELD_SWITCH        = 1,
    FIELD_TEMP_SET      = 2,
    FIELD_MODE_SET      = 3,
    FIELD_MODE_CUR      = 4,
    FIELD_TEMP_CUR      = 5,
    FIELD_RUN_STATUS    = 6,
    FIELD_LANGUAGE      = 7,
    FIELD_SPLASH        = 8,
    FIELD_CHILDLOCK     = 9,
    FIELD_WIND_SET      = 10,
    FIELD_WIND_CUR      = 11,
    FIELD_HUMI_SET      = 12,
    FIELD_HUMI_CUR      = 13,
    FIELD_HOT_WATER_SET = 14,
    FIELD_HOT_WATER_CUR = 15,
    FIELD_ZoneA_EN 		= 16,
    FIELD_ZoneB_EN 		= 17,
    FIELD_DHW_EN 		= 18,
    FIELD_TEMP_UNIT		= 19,
    FIELD_FUNC_MAX,              /* ע: ����FIELD_MAXǰ����µ��ֶι��ܶ��� */
};

/* ϵͳ��Ϣ�ֶι��ܶ��� */
enum DP_FIELD_SYS_INFO_ID
{
    FIELD_STR_CODE_TYPE =0,
    FIELD_SYS_MAINT ,
    FIELD_CNTRL_LANG ,
    FIELD_ERR_ALERT ,
    FIELD_MOD_NUM  ,
    FIELD_FAC_PWD ,
    FIELD_RUN_SW_MODE_EN ,
    FIELD_SPLASH_LOGO   ,
    FIELD_SPEECH_EN  ,
    FIELD_GRP_MAX ,
    FIELD_ITEM_MAX ,
    FIELD_FUNC_OPT  ,
    FIELD_SYS_INFO_MAX,              /* ע: ����FIELD_MAXǰ����µ��ֶι��ܶ��� */
};


/* �����ֶι��ܶ��� */
enum DP_FIELD_AREA_FUNC_ID
{
    FIELD_AREA_SWITCH   	 	= 1,
    FIELD_AREA_TEMP_SET      	= 2,
    FIELD_AREA_TEMP_CUR      	= 3,
	FIELD_AREA_MODE_SET			= 4,
	FIELD_AREA_MODE_CUR			= 5,
    FIELD_AREA_CTRL_WAY      	= 6,	//���Ʒ�ʽ
    FIELD_AREA_CTRL_obj   		= 7,	//���ƶ���

    FIELD_AREA_COMP_STA      	= 8,
    FIELD_AREA_DEFRO_STA        = 9,
    FIELD_AREA_ANFREZ_STA     	= 10,	//����״̬
    FIELD_AREA_OUTHEAT_STA      = 11,	//�ⲿ��Դ
    FIELD_AREA_STERILIZE_STA    = 12, 	//ɱ��
    FIELD_AREA_AUXHEAT_STA      = 13,	//���������
    FIELD_AREA_TANKHEAT_STA     = 14,	//ˮ������
    FIELD_AREA_SILENT_STA		= 15,	//����ģʽ
    FIELD_AREA_ENESAVING_STA 	= 16,	//����ģʽ
    FIELD_AREA_HOLIDAY_STA 	 	= 17,	//����ģʽ
    FIELD_AREA_SOLAR_STA 		= 18,	//̫����

    FIELD_AREA_MAX,              			/* ע: ����FIELD_MAXǰ����µ��ֶι��ܶ��� */
};

/* ��ֵ�洢��������(�ɸ�����Ҫ����,ע������������"��ǰ�¶�"�����Ƶ���仯������)*/
enum DP_FIELD_DATA_PREV
{
    SWITCH_IDX        = 0,  /* ���ػ� */
    TEMP_SET_IDX      = 1,  /* �趨�¶� */
    MODE_SET_IDX      = 2,  /* �趨ģʽ */
    MODE_CUR_IDX      = 3,  /* ��ǰģʽ */
    RUN_STATUS_IDX    = 4,  /* ����״̬ */
    ERR_ALERT_IDX     = 5,  /* ����澯 */
    CUR_ERR_NUM_IDX   = 6,  /* ��ǰ������ */
    SPLASH_IDX        = 7,  /* ����ҳ */
    CHILDLOCK_IDX     = 8,  /* ͯ�� */
    LANGUAGE_IDX      = 9,  /* ���� */
    WIND_SET_IDX      = 10, /* �趨���� */
    HUMI_SET_IDX      = 11, /* �趨ʪ�� */
    HOT_WATER_SET_IDX = 12, /* ��ˮ�趨�¶� */

    /* ����һ */
	ZONEA_SWITCH_IDX = 13,
	ZONEA_TEMP_SET_IDX = 14,
	ZONEA_MODE_SET_IDX = 15,
	ZONEA_MODE_CUR_IDX = 16,
	ZONEA_CTRL_END = 17,
	ZONEA_CTRL_OBJ = 18,
	ZONEA_COMP_STA = 19,
	ZONEA_DEFRO_STA = 20,
	ZONEA_ANFREZ_STA = 21,
	ZONEA_OUTHEAT_STA = 22,
//	ZONEA_STERILIZE_STA = 13,
	ZONEA_AUXHEAT_STA = 23,
//	ZONEA_TANKHEAT_STA = 13,
	ZONEA_SILENT_STA = 24,
	ZONEA_ENESAVING_STA = 25,
	ZONEA_HOLIDAY_STA = 26,
//	ZONEA_AREA_SOLAR_STA = 13,

	/* ����� */
	ZONEB_SWITCH_IDX = 27,
	ZONEB_TEMP_SET_IDX = 28,
//	ZONEB_MODE_SET_IDX = 15,
	ZONEB_CTRL_END = 29,
	ZONEB_CTRL_OBJ = 30,
	ZONEB_COMP_STA = 31,
	ZONEB_DEFRO_STA = 32,
	ZONEB_ANFREZ_STA = 33,
	ZONEB_OUTHEAT_STA = 34,
//	ZONEB_STERILIZE_STA = 13,
	ZONEB_AUXHEAT_STA = 35,
//	ZONEB_TANKHEAT_STA = 13,
	ZONEB_SILENT_STA = 36,
	ZONEB_ENESAVING_STA = 37,
	ZONEB_HOLIDAY_STA = 38,
//	ZONEB_AREA_SOLAR_STA = 13,

	/* ��ˮ */
	DHW_SWITCH_IDX = 39,
	DHW_TEMP_SET_IDX = 40,
//	DHW_MODE_SET_IDX = 15,
	DHW_CTRL_END = 41,
//	DHW_CTRL_OBJ = 29,
	DHW_COMP_STA = 42,
	DHW_DEFRO_STA = 43,
	DHW_ANFREZ_STA = 44,
	DHW_OUTHEAT_STA = 45,
	DHW_STERILIZE_STA = 46,
	DHW_AUXHEAT_STA = 47,
	DHW_TANKHEAT_STA = 48,
	DHW_SILENT_STA = 49,
	DHW_ENESAVING_STA = 50,
	DHW_HOLIDAY_STA = 51,
	DHW_AREA_SOLAR_STA = 52,

	ZONE1_EN_IDX = 53,
	ZONE2_EN_IDX = 54,
	DHW_EN_IDX = 55,

    DATA_PREV_MAX,          /* ����ֵ���� */
};
I16 tyb_slp_data_prev[DATA_PREV_MAX];                                  /* ��ֵ�洢����(��������ֵ���бȽϣ������ж��Ƿ���Ҫ��������) */

/* ========================================================================================== */
/* Ϳ ѻ �� �� �� �� �� �� �� ��              */
/* ========================================================================================== */
/* ��"���������籣��" */
#ifdef TUYA_USE_BUF_SAFETY
/*******************************************************************************
 �� ��: TYB_create_bool_field()
 �� ��: �����������ֶΣ�������Ԥ��
 �� ��: buf--�ַ���������;
        acc_len--��ǰ�ۼƳ���;
        func_attr--�ֶι�������;
        func_id--�ֶι���ID;
        field_type--�ֶ�����;
        title--�����ַ���ID;
        value--��ǰ��ֵ;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void TYB_create_bool_field(U08 *buf, U08 *acc_len, U08 func_attr, U08 func_id, U08 field_type, U16 title, BOOL value)
{
    U08 i;
    U08 buf_len = 0;
    //U08 useless_cache[30];    /* �������֧��15�������ַ���30��Ӣ���ַ� */
    U16 acc_bytes = 0;
    U16 *src_buf = NULL;
    DP_FIELD_BOOL field;

    //U08 title_msg_len = TYB_SLP_GET_CODE_STR(useless_cache, title);

    /* ����ƴ�Ӻ���ܳ��� */
    acc_bytes = *acc_len + DP_FIELD_BOOL_BYTES;

    /* �Ѵ����쳣��ʣ��ռ䲻��������ֹƴ�� */
    if (acc_bytes > RAW_DATA_BYTES)
    {
        return;
    }
    else
    {
        MEMCLR(&field, DP_FIELD_BOOL_BYTES);

        /* ���ݸ��� */
        field.type = (field_type == FIELD_BOOL) ? MAKEWORD(FIELD_BOOL, FIELD_ENABLE) : MAKEWORD(FIELD_BOOL, FIELD_DISABLED);
        field.props = MAKEWORD(func_id, func_attr);
        field.tittle_msgid = title;
        field.value = value;

        /* ��ֵ */
        src_buf = (U16*)&field;
        for (i = 0; i < DP_FIELD_BOOL_SIZE; i++)
        {
            buf[buf_len++] = HIBYTE(src_buf[i]);
            buf[buf_len++] = LOBYTE(src_buf[i]);
        }

        *acc_len += buf_len;
    }
}


/*******************************************************************************
 �� ��: TYB_create_number_field()
 �� ��: ������ֵ���ֶΣ�������Ԥ��
 �� ��: buf--�ַ���������;
        acc_len--��ǰ�ۼƳ���;
        func_attr--�ֶι�������;
        func_id--�ֶι���ID;
        field_type--�ֶ�����;
        title--�����ַ���ID;
        attr--��ǰ��ֵ����;
        value--��ǰ��ֵ;
        max--���ֵ;
        min--��Сֵ;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void TYB_create_number_field(U08 *buf, U08 *acc_len, U08 func_attr, U08 func_id, U08 field_type, U16 title, U16 attr, I16 value, I16 max, I16 min)
{
    U08 i;
    U08 buf_len = 0;
    //U08 useless_cache[30];    /* �������֧��15�������ַ���30��Ӣ���ַ� */
    U16 acc_bytes = 0;
    U16 *src_buf = NULL;
    DP_FIELD_NUMBER field;

    //U08 title_msg_len = TYB_SLP_GET_CODE_STR(useless_cache, title);

    /* ����ƴ�Ӻ���ܳ��� */
    acc_bytes = *acc_len + DP_FIELD_NUMBER_BYTES;

    /* �Ѵ����쳣��ʣ��ռ䲻��������ֹƴ�� */
    if (acc_bytes > RAW_DATA_BYTES)
    {
        return;
    }
    else
    {
        MEMCLR(&field, DP_FIELD_NUMBER_BYTES);

        /* ���ݸ��� */
        field.type = (field_type == FIELD_NUMBER) ? MAKEWORD(FIELD_NUMBER, FIELD_ENABLE) : MAKEWORD(FIELD_NUMBER, FIELD_DISABLED);
        field.props = MAKEWORD(func_id, func_attr);
        field.attr = attr;
        field.value = value;
        field.max = max;
        field.min = min;
        field.tittle_msgid = title;

        /* ��ֵ */
        src_buf = (U16*)&field;
        for (i = 0; i < DP_FIELD_NUMBER_SIZE; i++)
        {
            buf[buf_len++] = HIBYTE(src_buf[i]);
            buf[buf_len++] = LOBYTE(src_buf[i]);
        }

        *acc_len += buf_len;
    }
}


/*******************************************************************************
 �� ��: TYB_create_enum_field()
 �� ��: ����ö�����ֶΣ�������Ԥ��
 �� ��: buf--�ַ���������;
        acc_len--��ǰ�ۼƳ���;
        func_attr--�ֶι�������;
        func_id--�ֶι���ID;
        field_type--�ֶ�����;
        title--�����ַ���ID;
        range--һ��ö���ַ���;
        range_len--ö���ַ�������;
        value--��ǰ��ֵ;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void TYB_create_enum_field(U08 *buf, U08 *acc_len, U08 func_attr, U08 func_id, U08 field_type, U16 title, U16 *range, U08 range_len, I16 value)
{
    U08 i;
    U08 buf_len = 0;
    //U08 useless_cache[30];    /* �������֧��15�������ַ���30��Ӣ���ַ� */
    U16 acc_bytes = 0;
    U16 *src_buf = NULL;
    DP_FIELD_ENUM field;

    //U08 title_msg_len = TYB_SLP_GET_CODE_STR(useless_cache, title);

    /* ����ƴ�Ӻ���ܳ��� */
    acc_bytes = *acc_len + DP_FIELD_ENUM_BYTES + range_len;

    /* �Ѵ����쳣��ʣ��ռ䲻��������ֹƴ�� */
    if (acc_bytes > RAW_DATA_BYTES)
    {
        return;
    }
    else
    {
        MEMCLR(&field, DP_FIELD_ENUM_BYTES);

        /* ���ݸ��� */
        field.type = (field_type == FIELD_ENUM) ? MAKEWORD(FIELD_ENUM, FIELD_ENABLE) : MAKEWORD(FIELD_ENUM, FIELD_DISABLED);
        field.props = MAKEWORD(func_id, func_attr);
        field.value = value;
        field.tittle_msgid = title;
        field.range_len = range_len;

        /* ��ֵ */
        src_buf = (U16*)&field;
        for (i = 0; i < DP_FIELD_ENUM_SIZE; i++)
        {
            buf[buf_len++] = HIBYTE(src_buf[i]);
            buf[buf_len++] = LOBYTE(src_buf[i]);
        }

        /* ���ö��ֵ */
        for(i = 0; i < range_len; i++)
        {
            //U08 range_msg_len = TYB_SLP_GET_CODE_STR(&buf[buf_len+1], range[i]);
            buf[buf_len++] = HIBYTE(range[i]);
			buf[buf_len++] = LOBYTE(range[i]);
            //buf_len += range_msg_len;
            if (i == (range_len - 1))
            {
                break;
            }
        }

        *acc_len += buf_len;
    }
}


/*******************************************************************************
 �� ��: TYB_create_ro_num_field()
 �� ��: ����ֻ����ֵ���ֶΣ�������Ԥ��
 �� ��: buf--�ַ���������;
        acc_len--��ǰ�ۼƳ���;
        func_attr--�ֶι�������;
        func_id--�ֶι���ID;
        field_type--�ֶ�����;
        title--�����ַ���ID;
        attr--��ǰ��ֵ����;
        value--��ǰ��ֵ;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void TYB_create_ro_num_field(U08 *buf, U08 *acc_len, U08 func_attr, U08 func_id, U08 field_type, U16 title, U16 attr, I16 value)
{
    U08 i;
    U08 buf_len = 0;
    //U08 useless_cache[30];   /* �������֧��15�������ַ���30��Ӣ���ַ� */
    U16 acc_bytes = 0;
    U16 *src_buf = NULL;
    DP_FIELD_RO_NUM field;

    //U08 title_msg_len = TYB_SLP_GET_CODE_STR(useless_cache, title);

    /* ����ƴ�Ӻ���ܳ��� */
    acc_bytes = *acc_len + DP_FIELD_RO_NUM_BYTES;
    //acc_bytes += title_msg_len;

    /* �Ѵ����쳣��ʣ��ռ䲻��������ֹƴ�� */
    if (acc_bytes > RAW_DATA_BYTES)
    {
        return;
    }
    else
    {
        MEMCLR(&field, DP_FIELD_RO_NUM_BYTES);

        /* ���ݸ��� */
        field.type = (field_type == FIELD_RO_NUM) ? MAKEWORD(FIELD_RO_NUM, FIELD_ENABLE) : MAKEWORD(FIELD_RO_NUM, FIELD_DISABLED);
        field.props = MAKEWORD(func_id, func_attr);
        field.attr = attr;
        field.value = value;
        field.tittle_msgid = title;

        /* ��ֵ */
        src_buf = (U16*)&field;
        for (i = 0; i < DP_FIELD_RO_NUM_SIZE; i++)
        {
            buf[buf_len++] = HIBYTE(src_buf[i]);
            buf[buf_len++] = LOBYTE(src_buf[i]);
        }
        //buf_len += TYB_SLP_GET_CODE_STR(&buf[buf_len], title);

        *acc_len += buf_len;
    }
}


/*******************************************************************************
 �� ��: TYB_create_ro_msg_field()
 �� ��: ����ֻ���ַ������ֶΣ�������Ԥ��
 �� ��: buf--�ַ���������;
        acc_len--��ǰ�ۼƳ���;
        func_attr--�ֶι�������;
        func_id--�ֶι���ID;
        field_type--�ֶ�����;
        title--�����ַ���ID;
        msg--��ǰ�ַ���ID;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void TYB_create_ro_msg_field(U08 *buf, U08 *acc_len, U08 func_attr, U08 func_id, U08 field_type, U16 title, U16 msg)
{
    U08 i;
    U08 buf_len = 0;
    //U08 useless_cache[30];    /* �������֧��15�������ַ���30��Ӣ���ַ� */
    U16 acc_bytes = 0;
    U16 *src_buf = NULL;
    DP_FIELD_RO_MSG field;

    //U08 title_msg_len = TYB_SLP_GET_CODE_STR(useless_cache, title);
    //U08 msg_len = TYB_SLP_GET_CODE_STR(useless_cache, msg);

    /* ����ƴ�Ӻ���ܳ��� */
    acc_bytes = *acc_len + DP_FIELD_RO_MSG_BYTES;

    //acc_bytes += title_msg_len;
    //acc_bytes += msg_len;

    /* �Ѵ����쳣��ʣ��ռ䲻��������ֹƴ�� */
    if (acc_bytes > RAW_DATA_BYTES)
    {
        return;
    }
    else
    {
        MEMCLR(&field, DP_FIELD_RO_MSG_BYTES);

        /* ���ݸ��� */
        field.type = (field_type == FIELD_RO_MSG) ? MAKEWORD(FIELD_RO_MSG, FIELD_ENABLE) : MAKEWORD(FIELD_RO_MSG, FIELD_DISABLED);
        field.props = MAKEWORD(func_id, func_attr);
        field.tittle_msgid = title;
        field.content_msgid = msg;

        /* ��ֵ */
        src_buf = (U16*)&field;
        for (i = 0; i < DP_FIELD_RO_MSG_SIZE; i++)
        {
            buf[buf_len++] = HIBYTE(src_buf[i]);
            buf[buf_len++] = LOBYTE(src_buf[i]);
        }
        //buf_len += TYB_SLP_GET_CODE_STR(&buf[buf_len], title);
        //buf_len += TYB_SLP_GET_CODE_STR(&buf[buf_len], msg);

        *acc_len += buf_len;
    }
}


/*******************************************************************************
 �� ��: TYB_create_one_msg_field()
 �� ��: �������ַ����ֶΣ�������Ԥ��
 �� ��: buf--�ַ���������;
        acc_len--��ǰ�ۼƳ���;
        content--�ַ���;
        content_size--�ַ�������(�ֽ���);
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void TYB_create_one_msg_field(far U08 *buf, far U08 *acc_len, far U08 *content, U08 content_size)
{
    U08 buf_len = 0;
    U16 acc_bytes = 0;

    /* ����ƴ�Ӻ���ܳ��� */
    acc_bytes = *acc_len + DP_FIELD_ONE_MSG_HEADER_BYTES;
    acc_bytes = *acc_len + content_size;

    /* �Ѵ����쳣��ʣ��ռ䲻��������ֹƴ�� */
    if (acc_bytes > RAW_DATA_BYTES)
    {
        return;
    }
    else
    {
        acc_bytes = 0;
        if (content != PNULL)
        {
            /* ������� */
            buf[buf_len++] = FIELD_ONE_MSG;
            buf[buf_len++] = FIELD_ENABLE;
            buf[buf_len++] = content_size;
            MEMCPY(&buf[buf_len], content, content_size);
            buf_len += content_size;
        }
        else
        {
            buf[buf_len++] = FIELD_ONE_MSG;
            buf[buf_len++] = FIELD_ENABLE;
            buf[buf_len++] = 0;
        }

        *acc_len += buf_len;
    }
}


/*******************************************************************************
 �� ��: TYB_create_one_num_field()
 �� ��: ��������ֵ�ֶΣ�������Ԥ��
 �� ��: buf--�ַ���������;
        acc_len--�ۼƳ���;
        value--����;
        func_attr--�ֶι�������;
        func_id--�ֶι���ID;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void TYB_create_one_num_field(U08 *buf, U08 *acc_len, I16 value, U08 func_attr, U08 func_id )
{
    U08 buf_len = 0;
    U16 acc_bytes = 0;
	U16 props;
	props = MAKEWORD(func_id, func_attr);
    acc_bytes = *acc_len + DP_FIELD_ONE_NUM_BYTES;

    /* �Ѵ����쳣��ʣ��ռ䲻��������ֹƴ�� */
    if (acc_bytes > RAW_DATA_BYTES)
    {
        return;
    }
    else
    {
        /* ������� */
        buf[buf_len++] = FIELD_ONE_NUM;
        buf[buf_len++] = FIELD_ENABLE;

	    /* �ֶ����� */
	    buf[buf_len++] = HIBYTE(props);
	    buf[buf_len++] = LOBYTE(props);

        buf[buf_len++] = HIBYTE(value);
        buf[buf_len++] = LOBYTE(value);

        *acc_len += buf_len;
    }
}
#else
/*******************************************************************************
 �� ��: TYB_create_bool_field()
 �� ��: �����������ֶΣ�������Ԥ��
 �� ��: buf--�ַ���������;
        acc_len--��ǰ�ۼƳ���;
        func_attr--�ֶι�������;
        func_id--�ֶι���ID;
        field_type--�ֶ�����;
        title--�����ַ���ID;
        value--��ǰ��ֵ;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void TYB_create_bool_field(U08 *buf, U08 *acc_len, U08 func_attr, U08 func_id, U08 field_type, U16 title, BOOL value)
{
    U08 buf_len = 0;
    U08 title_len;
    U16 type, props;

    title_len = 0;
    type = (field_type == FIELD_BOOL) ? MAKEWORD(FIELD_BOOL, FIELD_ENABLE) : MAKEWORD(FIELD_BOOL, FIELD_DISABLED);
    props = MAKEWORD(func_id, func_attr);

    /* �ֶ����� */
    buf[buf_len++] = HIBYTE(type);
    buf[buf_len++] = LOBYTE(type);

    /* �ֶ����� */
    buf[buf_len++] = HIBYTE(props);
    buf[buf_len++] = LOBYTE(props);

    /* ��ǰֵ */
    buf[buf_len++] = 0;
    buf[buf_len++] = value;

    /* �����ַ��� */
    title_len = TYB_SLP_GET_CODE_STR(&buf[buf_len+2], title);

    /* �����ַ������� */
    buf[buf_len++] = HIBYTE(title_len);
    buf[buf_len++] = LOBYTE(title_len);
    buf_len += title_len;

    *acc_len += buf_len;
}


/*******************************************************************************
 �� ��: TYB_create_number_field()
 �� ��: ������ֵ���ֶΣ�������Ԥ��
 �� ��: buf--�ַ���������;
        acc_len--��ǰ�ۼƳ���;
        func_attr--�ֶι�������;
        func_id--�ֶι���ID;
        field_type--�ֶ�����;
        title--�����ַ���ID;
        attr--��ǰ��ֵ����;
        value--��ǰ��ֵ;
        max--���ֵ;
        min--��Сֵ;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void TYB_create_number_field(U08 *buf, U08 *acc_len, U08 func_attr, U08 func_id, U08 field_type, U16 title, U16 attr, I16 value, I16 max, I16 min)
{
    U08 buf_len = 0;
    U08 title_len;
    U16 type, props;

    title_len = 0;
    type = (field_type == FIELD_NUMBER) ? MAKEWORD(FIELD_NUMBER, FIELD_ENABLE) : MAKEWORD(FIELD_NUMBER, FIELD_DISABLED);
    props = MAKEWORD(func_id, func_attr);

    /* �ֶ����� */
    buf[buf_len++] = HIBYTE(type);
    buf[buf_len++] = LOBYTE(type);

    /* �ֶ����� */
    buf[buf_len++] = HIBYTE(props);
    buf[buf_len++] = LOBYTE(props);

    /* ���� */
    buf[buf_len++] = HIBYTE(attr);
    buf[buf_len++] = LOBYTE(attr);

    /* ��ǰֵ */
    buf[buf_len++] = HIBYTE(value);
    buf[buf_len++] = LOBYTE(value);

    /* ���ֵ */
    buf[buf_len++] = HIBYTE(max);
    buf[buf_len++] = LOBYTE(max);

    /* ��Сֵ */
    buf[buf_len++] = HIBYTE(min);
    buf[buf_len++] = LOBYTE(min);

    /* �����ַ��� */
    title_len = TYB_SLP_GET_CODE_STR(&buf[buf_len+2], title);

    /* �����ַ������� */
    buf[buf_len++] = HIBYTE(title_len);
    buf[buf_len++] = LOBYTE(title_len);
    buf_len += title_len;

    *acc_len += buf_len;
}


/*******************************************************************************
 �� ��: TYB_create_enum_field()
 �� ��: ����ö�����ֶΣ�������Ԥ��
 �� ��: buf--�ַ���������;
        acc_len--��ǰ�ۼƳ���;
        func_attr--�ֶι�������;
        func_id--�ֶι���ID;
        field_type--�ֶ�����;
        title--�����ַ���ID;
        range--һ��ö���ַ���;
        range_len--ö���ַ�������;
        value--��ǰ��ֵ;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void TYB_create_enum_field(U08 *buf, U08 *acc_len, U08 func_attr, U08 func_id, U08 field_type, U16 title, U16 *range, U08 range_len, I16 value)
{
    U08 i;
    U08 buf_len = 0;
    U08 title_len, msg_len;
    U16 type, props;

    title_len = 0;
    type = (field_type == FIELD_ENUM) ? MAKEWORD(FIELD_ENUM, FIELD_ENABLE) : MAKEWORD(FIELD_ENUM, FIELD_DISABLED);
    props = MAKEWORD(func_id, func_attr);

    /* �ֶ����� */
    buf[buf_len++] = HIBYTE(type);
    buf[buf_len++] = LOBYTE(type);

    /* �ֶ����� */
    buf[buf_len++] = HIBYTE(props);
    buf[buf_len++] = LOBYTE(props);

    /* ��ǰֵ */
    buf[buf_len++] = HIBYTE(value);
    buf[buf_len++] = LOBYTE(value);

    /* �����ַ��� */
    title_len = TYB_SLP_GET_CODE_STR(&buf[buf_len+4], title);

    /* �����ַ������� */
    buf[buf_len++] = HIBYTE(title_len);
    buf[buf_len++] = LOBYTE(title_len);

    /* ö������ */
    buf[buf_len++] = HIBYTE(range_len);
    buf[buf_len++] = LOBYTE(range_len);
    buf_len += title_len;

    /* ö���ַ��� */
    for (i = 0; i < range_len; i++)
    {
        msg_len = TYB_SLP_GET_CODE_STR(&buf[buf_len+1], range[i]);
        buf[buf_len++] = msg_len;
        buf_len += msg_len;
        if (i == (range_len - 1))
        {
            break;
        }
    }

    *acc_len += buf_len;
}


/*******************************************************************************
 �� ��: TYB_create_ro_num_field()
 �� ��: ����ֻ����ֵ���ֶΣ�������Ԥ��
 �� ��: buf--�ַ���������;
        acc_len--��ǰ�ۼƳ���;
        func_attr--�ֶι�������;
        func_id--�ֶι���ID;
        field_type--�ֶ�����;
        title--�����ַ���ID;
        attr--��ǰ��ֵ����;
        value--��ǰ��ֵ;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void TYB_create_ro_num_field(U08 *buf, U08 *acc_len, U08 func_attr, U08 func_id, U08 field_type, U16 title, U16 attr, I16 value)
{
    U08 buf_len = 0;
    U08 title_len;
    U16 type, props;

    title_len = 0;
    type = (field_type == FIELD_RO_NUM) ? MAKEWORD(FIELD_RO_NUM, FIELD_ENABLE) : MAKEWORD(FIELD_RO_NUM, FIELD_DISABLED);
    props = MAKEWORD(func_id, func_attr);

    /* �ֶ����� */
    buf[buf_len++] = HIBYTE(type);
    buf[buf_len++] = LOBYTE(type);

    /* �ֶ����� */
    buf[buf_len++] = HIBYTE(props);
    buf[buf_len++] = LOBYTE(props);

    /* ���� */
    buf[buf_len++] = HIBYTE(attr);
    buf[buf_len++] = LOBYTE(attr);

    /* ��ǰֵ */
    buf[buf_len++] = HIBYTE(value);
    buf[buf_len++] = LOBYTE(value);

    /* �����ַ��� */
    title_len = TYB_SLP_GET_CODE_STR(&buf[buf_len+2], title);

    /* �����ַ������� */
    buf[buf_len++] = HIBYTE(title_len);
    buf[buf_len++] = LOBYTE(title_len);
    buf_len += title_len;

    *acc_len += buf_len;
}


/*******************************************************************************
 �� ��: TYB_create_ro_msg_field()
 �� ��: ����ֻ���ַ������ֶΣ�������Ԥ��
 �� ��: buf--�ַ���������;
        acc_len--��ǰ�ۼƳ���;
        func_attr--�ֶι�������;
        func_id--�ֶι���ID;
        field_type--�ֶ�����;
        title--�����ַ���ID;
        msg--��ǰ�ַ���ID;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void TYB_create_ro_msg_field(U08 *buf, U08 *acc_len, U08 func_attr, U08 func_id, U08 field_type, U16 title, U16 msg)
{
    U08 buf_len = 0;
    U08 title_len, msg_len;
    U16 type, props;

    title_len = 0;
    msg_len = 0;
    type = (field_type == FIELD_RO_MSG) ? MAKEWORD(FIELD_RO_MSG, FIELD_ENABLE) : MAKEWORD(FIELD_RO_MSG, FIELD_DISABLED);
    props = MAKEWORD(func_id, func_attr);

    /* �ֶ����� */
    buf[buf_len++] = HIBYTE(type);
    buf[buf_len++] = LOBYTE(type);

    /* �ֶ����� */
    buf[buf_len++] = HIBYTE(props);
    buf[buf_len++] = LOBYTE(props);

    /* �����ַ��� */
    title_len = TYB_SLP_GET_CODE_STR(&buf[buf_len+4], title);

    /* ��Ϣ�ַ��� */
    msg_len = TYB_SLP_GET_CODE_STR(&buf[buf_len+4+title_len], msg);

    /* �����ַ������� */
    buf[buf_len++] = HIBYTE(title_len);
    buf[buf_len++] = LOBYTE(title_len);

    /* ��Ϣ�ַ������� */
    buf[buf_len++] = HIBYTE(msg_len);
    buf[buf_len++] = LOBYTE(msg_len);

    buf_len = buf_len + title_len + msg_len;

    *acc_len += buf_len;
}


/*******************************************************************************
 �� ��: TYB_create_one_msg_field()
 �� ��: �������ַ����ֶΣ�������Ԥ��
 �� ��: buf--�ַ���������;
        acc_len--��ǰ�ۼƳ���;
        content--�ַ���;
        content_size--�ַ�������(�ֽ���);
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void TYB_create_one_msg_field(far U08 *buf, far U08 *acc_len, far U08 *content, U08 content_size)
{
    U08 buf_len = 0;

    if (content != PNULL)
    {
        buf[buf_len++] = FIELD_ONE_MSG;
        buf[buf_len++] = FIELD_ENABLE;
        buf[buf_len++] = content_size;
        MEMCPY(&buf[buf_len], content, content_size);
        buf_len += content_size;
    }
    else
    {
        buf[buf_len++] = FIELD_ONE_MSG;
        buf[buf_len++] = FIELD_ENABLE;
        buf[buf_len++] = 0;
    }

    *acc_len += buf_len;
}


/*******************************************************************************
 �� ��: TYB_create_one_num_field()
 �� ��: ��������ֵ�ֶΣ�������Ԥ��
 �� ��: buf--�ַ���������;
        acc_len--�ۼƳ���;
        value--����;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void TYB_create_one_num_field(U08 *buf, U08 *acc_len, I16 value)
{
    U08 buf_len = 0;

    buf[buf_len++] = FIELD_ONE_NUM;
    buf[buf_len++] = FIELD_ENABLE;
    buf[buf_len++] = HIBYTE(value);
    buf[buf_len++] = LOBYTE(value);

    *acc_len += buf_len;
}
#endif

/* ��ʾ"��ʾ���汾��" */
#if (TYBC_DISP_HMI_VER_EN == TRUE)
#ifndef MAX_VER_BYTE
#define MAX_VER_BYTE          (32)                                /* �汾���ֽ��� */
#endif
U08 tyb_hmi_ver[MAX_VER_BYTE];                                     /* ��ʾ���汾 */
#if (TYBC_STR_CODE_TYPE == UNI_CODE)
U08 tyb_hmi_ver_unicode[MAX_VER_BYTE*2];
U08 tyb_bkbd_ver_unicode[MAX_VER_BYTE*2];
#endif
#endif

/* ��"״̬��ѯ" */
#ifdef TUYA_HAS_STAT
U16 fg_upload_pv[_DIVUCARRY(TYBC_PV_MAX, 16)];                     /* ģ�����ϱ���־ */
#endif

#ifdef TUYA_USE_FAC_PWD
#if (TYBC_STR_CODE_TYPE == UNI_CODE)
U08 pass_word_unicode[MAX_PASS_WORD*2];
#endif
#endif

/* ʹ��"����" */
#if (TYBC_STR_CODE_TYPE == P_CODE)
#define MSG_IDX_END                          (MSG_IDX_MAX)
/*******************************************************************************
 �� ��: TYB_SLP_get_PUNP_code_str()
 �� ��: �����ַ�������, ��ȡ�ַ����������飬ȥ��ĩβ�ո�󿽱����ַ�����������
 �� ��: buf--�ַ���������;
        str_idx--�ַ�������;
 �� ��: �����ֽ���(��ȥ'\0'��ĩβȫ�ո�,��ֻ��һ���ո����ʱ����)
 �� ��: ����ַ�����ֻ��һ���ո��ַ�����ո���
*******************************************************************************/
U08 TYB_SLP_get_PUNP_code_str(U08* buf, U16 str_idx)
{
    const U08* str_ptr;
    const U08* ptr_bgn;
    U08 byte;
    U08 space_cnt = 0;
    U08 len = 0;
    BOOL fg_trim_end_space;

    /* ������ */
    if (str_idx <= MSG_IDX_END)
    {
        str_ptr = (TYBC_LANGUAGE_VAL == RN_LANGUAGE_CN) ? TYBC_CODE_STR_DICT_CN(str_idx) : TYBC_CODE_STR_DICT_EN(str_idx);
    }
    else
    {
        str_ptr = (TYBC_LANGUAGE_VAL == RN_LANGUAGE_CN) ? TYBC_CODE_STR_DICT_CN(TYBC_MSG_NULL) : TYBC_CODE_STR_DICT_EN(TYBC_MSG_NULL);
    }

    while(*str_ptr != '\0')
    {
        fg_trim_end_space = TRUE;

        ptr_bgn = str_ptr;
        byte = *str_ptr++;

        if (byte >= 0x80)  /* ��˫�ֽڴ��� */
        {
            buf[len++] = byte;
            buf[len++] = *str_ptr++;
        }
        else  /* ���ֽڴ��� */
        {
            /* ��⵽�ո��ַ� */
            if (byte == ' ')
            {
                /* ������ĩβ������Ƿ�Ϊȫ�ո� */
                while (*ptr_bgn != '\0')
                {
                    /* ������ڷǿո��ַ�������Ҫ�����˿ո� */
                    if (*ptr_bgn++ != ' ')
                    {
                        fg_trim_end_space = FALSE;
                        break;
                    }
                    else
                    {
                        space_cnt++;
                    }
                }

                if (len == 0 && fg_trim_end_space)
                {
                    fg_trim_end_space = FALSE;
                }

                if (!fg_trim_end_space)
                {
                    buf[len++] = byte;
                }
            }
            else  /* �ǿո��ַ� */
            {
                buf[len++] = byte;
            }
        }
    }
    return len;
}
#define TYB_SLP_GET_CODE_STR(buf, msg_id)    (TYB_SLP_get_PUNP_code_str(buf, msg_id))
/* ʹ��"GBK��" */
#elif (TYBC_STR_CODE_TYPE == GBK_CODE)
#define MSG_IDX_BGN                          (MSG_IDX_OFFSET)
#define MSG_IDX_END                          (MSG_MAX)         /* MSG_MAX ����Ϊ xxx(�����ַ������һ��)+20000 */
/*******************************************************************************
 �� ��: TYB_SLP_get_GBK_code_str()
 �� ��: �����ַ�������, ��ȡ�ַ����������飬���������ַ���������
 �� ��: buf--�ַ���������;
        str_idx--�ַ�������;
 �� ��: �����ֽ���(��ȥ'\0')
 �� ��: ��
*******************************************************************************/
U08 TYB_SLP_get_GBK_code_str(U08* buf, U16 str_idx)
{
    const U08* str_ptr = PNULL;
    U08 byte;
    U08 len = 0;

    /* ������ */
    if (str_idx <= MSG_IDX_END)
    {
    if (str_idx >= MSG_IDX_BGN)
    {
        str_idx -= MSG_IDX_BGN;
    }
        str_ptr = (TYBC_LANGUAGE_VAL == RN_LANGUAGE_CN) ? TYBC_CODE_STR_DICT_CN(str_idx) : TYBC_CODE_STR_DICT_EN(str_idx);
    }
    else
    {
        str_ptr = (TYBC_LANGUAGE_VAL == RN_LANGUAGE_CN) ? TYBC_CODE_STR_DICT_CN(TYBC_MSG_NULL) : TYBC_CODE_STR_DICT_EN(TYBC_MSG_NULL);
    }

    while(*str_ptr != 0xFF)
    {
        byte = *str_ptr++;
        if (byte >= 0x80)
        {
            buf[len++] = byte;
            buf[len++] = *str_ptr++;
        }
        else
        {
            buf[len++] = byte;
        }
    }
    return len;
}
#define TYB_SLP_GET_CODE_STR(buf, msg_id)    (TYB_SLP_get_GBK_code_str(buf, msg_id))
#elif (TYBC_STR_CODE_TYPE == UNI_CODE)
#define MSG_IDX_END                          (MSG_IDX_MAX)
/*******************************************************************************
 �� ��: TY_get_UNI_code_str()
 �� ��: �����ַ�������, ��ȡ�ַ����������飬���������ַ���������
 �� ��: 2��
 @ buf--�ַ���������
 @ str_idx--�ַ�������
 �� ��: �����ֽ���(��ȥ'\0')
 �� ��: ��
*******************************************************************************/
U08 TY_get_UNI_code_str(U08* buf, U16 str_idx)
{
    const U08* str_ptr = PNULL;
    U08 len = 0;

	switch (TYBC_LANGUAGE_VAL)
	{
		case RN_LANGUAGE_CN:
			str_ptr = TYBC_CODE_STR_DICT_CN(str_idx);
			break;
		default:
		case RN_LANGUAGE_EN:
			str_ptr = TYBC_CODE_STR_DICT_EN(str_idx);
			break;
		case RN_LANGUAGE_SP:
			str_ptr = TYBC_CODE_STR_DICT_SP(str_idx);
			break;
		case RN_LANGUAGE_GR:
			str_ptr = TYBC_CODE_STR_DICT_GR(str_idx);
			break;
		case RN_LANGUAGE_FR:
			str_ptr = TYBC_CODE_STR_DICT_FR(str_idx);
			break;
		case RN_LANGUAGE_POL:
			str_ptr = TYBC_CODE_STR_DICT_POL(str_idx);
			break;
		case RN_LANGUAGE_HG:
			str_ptr = TYBC_CODE_STR_DICT_HG(str_idx);
			break;
		case RN_LANGUAGE_ITL:
			str_ptr = TYBC_CODE_STR_DICT_ITL(str_idx);
			break;
		case RN_LANGUAGE_GE:
			str_ptr = TYBC_CODE_STR_DICT_GE(str_idx);
			break;
	}

    while((str_ptr[len] != 0xFF) || (str_ptr[len+1] != 0xFF))
    {
    	buf[len] = str_ptr[len+1];
		buf[len+1] = str_ptr[len];
	 	len+=2;
		if (len >= 2*97)		//�Ѿ�����97���ַ����ضϺ����ַ�����...����
		{
			U08 i;
			for (i=0; i<3; i++)
			{
		    	buf[len++] = 0x00;
				buf[len++] = 0x2e;
			}
			break;
		}
    }
    return len;
}
#define TYB_SLP_GET_CODE_STR(buf, msg_id)    (TY_get_UNI_code_str(buf, msg_id))
/*******************************************************************************
 �� ��: TYB_SLP_ascii_to_unicode()
 �� ��: ת�����ֽ�ASCIIΪUnicode�����������ַ���������
 �� ��: 1��
 @ buf--�ַ���������
 �� ��: ת�����ֽ���
 �� ��: ��
*******************************************************************************/
U08 TYB_SLP_ascii_to_unicode(U08* buf_unicode, U08* buf_ascii, U08 max_length)
{
	U16 len = 0;
	U16 i=0;

	while (buf_ascii[i] != '\0' && len < max_length)		//�����п��ַ��������øú���
	{
		buf_unicode[len++] = 0x00;			//��8λΪ0x00
		buf_unicode[len++] = buf_ascii[i];	//��8λԭʼ����
		i++;
	}
	return len;
}
#endif


/* ��ͨ������(��"��������"/"ģ��״̬") */
#ifdef TUYA_HAS_TRANS
BOOL fg_has_trans_task;                     /* ͨ�������־ */
TYB_TRANS_INFO tyb_trans_info;	                /* ͨ������������Ϣ */
TYB_TRANS_CACHE tyb_trans_cache;			/* ͨ������������Ϣ�ڵ㻺����� */

/*******************************************************************************
 �� ��: TYB_SLP_cache_append()
 �� ��: �����ȼ�ͨ�������������_��β������һ���ڵ�
 �� ��: p_src--������е�Դ����ָ��;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
static BOOL TYB_SLP_cache_append(TYB_TRANS_INFO* p_src)
{
    // ����
    if (tyb_trans_cache.queue.cnt < TYB_TRANS_NODE_MAX)
    {
        tyb_trans_cache.node[tyb_trans_cache.queue.end] = *p_src;

        if (++tyb_trans_cache.queue.end >= TYB_TRANS_NODE_MAX)
        {
            tyb_trans_cache.queue.end = 0;
        }

        tyb_trans_cache.queue.cnt++;

        return TRUE;
    }

    return FALSE;
}


/*******************************************************************************
 �� ��: TYB_SLP_cache_shift()
 �� ��: �����ȼ�ͨ�������������_��ͷ���Ƴ�һ���ڵ�
 �� ��: p_dst--�Ƴ����е�Ŀ������ָ��;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
static BOOL TYB_SLP_cache_shift(TYB_TRANS_INFO* p_dst)
{
    // �ǿ�
    if (tyb_trans_cache.queue.cnt > 0)
    {
        *p_dst = tyb_trans_cache.node[tyb_trans_cache.queue.head];

        if (++tyb_trans_cache.queue.head >= TYB_TRANS_NODE_MAX)
        {
            tyb_trans_cache.queue.head = 0;
        }

        tyb_trans_cache.queue.cnt--;

        return TRUE;
    }

    return FALSE;
}


/*******************************************************************************
 �� ��: TYB_SLP_is_node_unique()
 �� ��: �ж��½ڵ��ڶ������Ƿ����
 �� ��: node--��ͨ����������ڵ�;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
static BOOL TYB_SLP_is_node_unique(TYB_TRANS_INFO* node)
{
    U08 i,idx;
    BOOL fg_node_accept = TRUE;

    for (i = 0; i < tyb_trans_cache.queue.cnt; i++)
    {
        idx = (tyb_trans_cache.queue.head + i) % TYB_TRANS_NODE_MAX;

        if (node->trigger_src == tyb_trans_cache.node[idx].trigger_src \
        && node->parse_code  == tyb_trans_cache.node[idx].parse_code \
        && node->request_code == tyb_trans_cache.node[idx].request_code \
        && node->trans_code == tyb_trans_cache.node[idx].trans_code \
        && node->pkg_id == tyb_trans_cache.node[idx].pkg_id \
        && node->addr[0] == tyb_trans_cache.node[idx].addr[0] \
        && node->addr[1] == tyb_trans_cache.node[idx].addr[1])
        {
            fg_node_accept = FALSE;
            break;
        }
    }
    return fg_node_accept;
}


/*******************************************************************************
 �� ��: TYB_SLP_check_channel_transmit()
 �� ��: ����Ƿ����ͨ������
 �� ��: channel_trans_dpid_idx--ͨ�����书�ܵ�ID������;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
static BOOL TYB_SLP_check_channel_transmit(U08 *channel_trans_dpid_idx)
{
    BOOL channel_trans_request = FALSE;
    static U08 channel_dpid = DPID_DATA_CHANNEL_01;


    /* ��ǰ��ͨ���������� */
    if (!fg_has_trans_task)
    {
       /* ��ȫ���ϱ����󣬵ȱ���ͨ��������ɺ��ٽ���ȫ���ϱ� */
       if (fg_dpid_upload_all)
       {
           return channel_trans_request;
       }

       /* ���зǿ� */
       if (TYB_SLP_cache_shift(&tyb_trans_info))
       {
           fg_has_trans_task = TRUE;
           channel_trans_request = TRUE;
       }
    }
    else
    {
        channel_trans_request = TRUE;
    }

    if (channel_trans_request)
    {
        *channel_trans_dpid_idx = TYB_get_dpid_idx(channel_dpid);
        if (++channel_dpid > DPID_DATA_CHANNEL_06)
        {
            channel_dpid = DPID_DATA_CHANNEL_01;
        }
    }

    return channel_trans_request;
}
#endif


/*******************************************************************************
 �� ��: TYB_SLP_check_data_updated()
 �� ��: ��鱾�������Ƿ������£�ͨ���Ա��¾�ֵ������ͬ������ֵ���¾�ֵ
 �� ��: cur_val--��ֵ(��ǰֵ);
        prev_val--��ֵָ��;
 �� ��: TRUE/FALSE--�����޸���/�����Ѹ���
 �� ��: ��
*******************************************************************************/
static BOOL TYB_SLP_check_data_updated(I16 cur_val, U08 idx)
{
    if (cur_val != tyb_slp_data_prev[idx])
    {
        tyb_slp_data_prev[idx] = cur_val;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/*******************************************************************************
 �� ��: TYB_SLP_check_all_upload()
 �� ��: ȫ���ϱ�һ��,���ڵ���/��������/���Ա仯/�����仯ʱ
 �� ��: all_upload_dpid_idx--��Ҫȫ����Ӧ�Ĺ��ܵ�ID����;
 �� ��: TRUE/FALSE--��Ҫȫ����Ӧ/����Ҫȫ����Ӧ
 �� ��: ��
*******************************************************************************/
static BOOL TYB_SLP_check_all_upload(U08 *all_upload_dpid_idx)
{
    BOOL all_upload_request = FALSE;
    static U08 all_dpid_idx;

    if (fg_dpid_upload_all)
    {
        /* ����ͨ������ר�ù��ܵ� */
        while (DP[all_dpid_idx].func_type == FUNC_CHANNEL)
        {
            all_dpid_idx++;
        }
        *all_upload_dpid_idx = all_dpid_idx;
        all_upload_request = TRUE;
        if (++all_dpid_idx >= DPID_USED)
        {
            all_dpid_idx = 0;
            fg_dpid_upload_all = FALSE;
        }
    }

    return all_upload_request;
}


/*******************************************************************************
 �� ��: TYB_SLP_check_local_data_update()
 �� ��: ��鱾�������Ƿ����仯��ͨ���¾�ֵ�Աȼ�飬�����ݷ����仯����Ӧ����
        ���Ǹ����ȼ�����˳���ϱ�
 �� ��: local_update_dpid_idx--��Ҫ�ϱ��Ĺ��ܵ�ID����;
 �� ��: TRUE/FALSE--��Ҫ������Ӧ/����Ҫ������Ӧ
 �� ��: ע�ⲻ���Զ�"ϵͳ/ģ��״̬"��ʹ�ô˷���������Ƶ���������ϱ�
*******************************************************************************/
static BOOL TYB_SLP_check_local_data_update(U08 *local_update_dpid_idx)
{
    U08 dpid_idx;
    BOOL update_cloud_request = FALSE;

/* ͨ��������Ϣ */
    /* ����һʹ�� */
    if (TYB_SLP_check_data_updated(TYBC_ZoneA_EN_VAL, ZONE1_EN_IDX))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEA_FUNC_INFO);
    }
	/* �����ʹ�� */
    if (TYB_SLP_check_data_updated(TYBC_ZoneB_EN_VAL, ZONE2_EN_IDX))
    {
        SETBIT_N(fg_dpid_upload_pri_high, GENERAL_SET_INFO);
    }
	/* ��ˮʹ�� */
    if (TYB_SLP_check_data_updated(TYBC_DHW_EN_VAL, DHW_EN_IDX))
    {
        SETBIT_N(fg_dpid_upload_pri_high, GENERAL_SET_INFO);
    }

/* ����һ */
    /* ���ػ� */
    if (TYB_SLP_check_data_updated(TYBC_SWITCH_VAL, ZONEA_SWITCH_IDX))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEA_FUNC_INFO);
    }
    /* �趨�¶� */
    if (TYB_SLP_check_data_updated(TYBC_TEMP_SET_VAL, ZONEA_TEMP_SET_IDX))
    {
        //SETBIT_N(fg_dpid_upload_pri_high, STD_TEMP_SET);
        SETBIT_N(fg_dpid_upload_pri_high, ZONEA_FUNC_INFO);
    }
    /* �趨ģʽ */
    if (TYB_SLP_check_data_updated(TYBC_MODE_SET_VAL, ZONEA_MODE_SET_IDX))
    {
        //SETBIT_N(fg_dpid_upload_pri_high, STD_MODE);
        SETBIT_N(fg_dpid_upload_pri_high, ZONEA_FUNC_INFO);
    }
    /* ��ǰģʽ */
    if (TYB_SLP_check_data_updated(TYBC_MODE_CUR_VAL, ZONEA_MODE_CUR_IDX))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEA_FUNC_INFO);
    }

    /* ����ĩ�� */
    if (TYB_SLP_check_data_updated(TYBC_CTRL_END_VALUE, ZONEA_CTRL_END))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEA_FUNC_INFO);
    }
	/* ���ƶ��� */
    if (TYB_SLP_check_data_updated(TYBC_CTRL_OBJ, ZONEA_CTRL_OBJ))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEA_FUNC_INFO);
    }
	/* ѹ��״̬ */
    if (TYB_SLP_check_data_updated(TYBC_COMP_STA, ZONEA_COMP_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEA_FUNC_INFO);
    }
	/* ��˪״̬ */
    if (TYB_SLP_check_data_updated(TYBC_DEFRO_STA, ZONEA_DEFRO_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEA_FUNC_INFO);
    }
	/* ����״̬ */
    if (TYB_SLP_check_data_updated(TYBC_ANFREZ_STA, ZONEA_ANFREZ_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEA_FUNC_INFO);
    }
	/* �ⲿ��Դ���״̬ */
    if (TYB_SLP_check_data_updated(TYBC_OUTHEAT_STA, ZONEA_OUTHEAT_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEA_FUNC_INFO);
    }
	/* ���������״̬ */
    if (TYB_SLP_check_data_updated(TYBC_AUXHEAT_STA, ZONEA_AUXHEAT_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEA_FUNC_INFO);
    }
	/* ����ģʽ״̬ */
    if (TYB_SLP_check_data_updated(TYBC_SILENT_STA, ZONEA_SILENT_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEA_FUNC_INFO);
    }
	/* ����ģʽ״̬ */
    if (TYB_SLP_check_data_updated(TYBC_ENESAVING_STA, ZONEA_ENESAVING_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEA_FUNC_INFO);
    }
	/* ����ģʽ״̬ */
    if (TYB_SLP_check_data_updated(TYBC_HOLIDAY_STA, ZONEA_HOLIDAY_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEA_FUNC_INFO);
    }

/* ����� */
    /* ���ػ� */
    if (TYB_SLP_check_data_updated(TYBC_ZoneB_SWITCH_VAL, ZONEB_SWITCH_IDX))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEB_FUNC_INFO);
    }
    /* �趨�¶� */
    if (TYB_SLP_check_data_updated(TYBC_ZoneB_TEMP_SET_VAL, ZONEB_TEMP_SET_IDX))
    {
        //SETBIT_N(fg_dpid_upload_pri_high, STD_TEMP_SET);
        SETBIT_N(fg_dpid_upload_pri_high, ZONEB_FUNC_INFO);
    }
    /* ����ĩ�� */
    if (TYB_SLP_check_data_updated(TYBC_ZoneB_CTRL_END_VALUE, ZONEB_CTRL_END))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEB_FUNC_INFO);
    }
	/* ���ƶ��� */
    if (TYB_SLP_check_data_updated(TYBC_ZoneB_CTRL_OBJ, ZONEB_CTRL_OBJ))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEB_FUNC_INFO);
    }
	/* ѹ��״̬ */
    if (TYB_SLP_check_data_updated(TYBC_ZoneB_COMP_STA, ZONEB_COMP_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEB_FUNC_INFO);
    }
	/* ��˪״̬ */
    if (TYB_SLP_check_data_updated(TYBC_ZoneB_DEFRO_STA, ZONEB_DEFRO_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEB_FUNC_INFO);
    }
	/* ����״̬ */
    if (TYB_SLP_check_data_updated(TYBC_ZoneB_ANFREZ_STA, ZONEB_ANFREZ_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEB_FUNC_INFO);
    }
	/* �ⲿ��Դ���״̬ */
    if (TYB_SLP_check_data_updated(TYBC_ZoneB_OUTHEAT_STA, ZONEB_OUTHEAT_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEB_FUNC_INFO);
    }
	/* ���������״̬ */
    if (TYB_SLP_check_data_updated(TYBC_ZoneB_AUXHEAT_STA, ZONEB_AUXHEAT_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEB_FUNC_INFO);
    }
	/* ����ģʽ״̬ */
    if (TYB_SLP_check_data_updated(TYBC_ZoneB_SILENT_STA, ZONEB_SILENT_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEB_FUNC_INFO);
    }
	/* ����ģʽ״̬ */
    if (TYB_SLP_check_data_updated(TYBC_ZoneB_ENESAVING_STA, ZONEB_ENESAVING_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEB_FUNC_INFO);
    }
	/* ����ģʽ״̬ */
    if (TYB_SLP_check_data_updated(TYBC_ZoneB_HOLIDAY_STA, ZONEB_HOLIDAY_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEB_FUNC_INFO);
    }

/* ������ˮ */
	/* ���ػ� */
    if (TYB_SLP_check_data_updated(TYBC_DHW_SWITCH_VAL, DHW_SWITCH_IDX))
    {
        SETBIT_N(fg_dpid_upload_pri_high, DHW_FUNC_INFO);
    }
    /* �趨�¶� */
    if (TYB_SLP_check_data_updated(TYBC_DHW_TEMP_SET_VAL, DHW_TEMP_SET_IDX))
    {
        //SETBIT_N(fg_dpid_upload_pri_high, STD_TEMP_SET);
        SETBIT_N(fg_dpid_upload_pri_high, DHW_FUNC_INFO);
    }
    /* ����ĩ�� */
    if (TYB_SLP_check_data_updated(TYBC_DHW_CTRL_END_VALUE, DHW_CTRL_END))
    {
        SETBIT_N(fg_dpid_upload_pri_high, DHW_FUNC_INFO);
    }
	/* ѹ��״̬ */
    if (TYB_SLP_check_data_updated(TYBC_DHW_COMP_STA, DHW_COMP_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, DHW_FUNC_INFO);
    }
	/* ��˪״̬ */
    if (TYB_SLP_check_data_updated(TYBC_DHW_DEFRO_STA, DHW_DEFRO_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, DHW_FUNC_INFO);
    }
	/* ����״̬ */
    if (TYB_SLP_check_data_updated(TYBC_DHW_ANFREZ_STA, DHW_ANFREZ_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, DHW_FUNC_INFO);
    }
	/* �ⲿ��Դ���״̬ */
    if (TYB_SLP_check_data_updated(TYBC_DHW_OUTHEAT_STA, DHW_OUTHEAT_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, DHW_FUNC_INFO);
    }

	/* ���������״̬ */
    if (TYB_SLP_check_data_updated(TYBC_DHW_AUXHEAT_STA, DHW_AUXHEAT_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, DHW_FUNC_INFO);
    }
	/* ����ģʽ״̬ */
    if (TYB_SLP_check_data_updated(TYBC_DHW_SILENT_STA, DHW_SILENT_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, DHW_FUNC_INFO);
    }
	/* ����ģʽ״̬ */
    if (TYB_SLP_check_data_updated(TYBC_DHW_ENESAVING_STA, DHW_ENESAVING_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, DHW_FUNC_INFO);
    }
	/* ����ģʽ״̬ */
    if (TYB_SLP_check_data_updated(TYBC_DHW_HOLIDAY_STA, DHW_HOLIDAY_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, DHW_FUNC_INFO);
    }
	/* ̫�����ź�״̬ */
    if (TYB_SLP_check_data_updated(TYBC_DHW_SOLAR_STA, DHW_AREA_SOLAR_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, DHW_FUNC_INFO);
    }
	/* ɱ��״̬ */
    if (TYB_SLP_check_data_updated(TYBC_DHW_STERILIZE_STA, DHW_STERILIZE_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, DHW_FUNC_INFO);
    }
	/* ˮ�����״̬ */
    if (TYB_SLP_check_data_updated(TYBC_DHW_TANKHEAT_STA, DHW_TANKHEAT_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, DHW_FUNC_INFO);
    }

#if 0
/* ��"��չ����" */
#ifdef TUYA_HAS_EXT_FUNC
/* ��"���ٹ���" */
#ifdef TUYA_USE_WIND
    /* �趨���� */
    if (TYB_SLP_check_data_updated(TYBC_WIND_SET_VAL, WIND_SET_IDX))
    {
        SETBIT_N(fg_dpid_upload_pri_high, EXTEND_FUNC_INFO);
    }
#endif

/* ��"ʪ�ȹ���" */
#ifdef TUYA_USE_HUMI
    /* �趨ʪ�� */
    if (TYB_SLP_check_data_updated(TYBC_HUMI_SET_VAL, HUMI_SET_IDX))
    {
        SETBIT_N(fg_dpid_upload_pri_high, EXTEND_FUNC_INFO);
    }
#endif

/* ��"��ˮ����" */
#ifdef TUYA_USE_HOT_WATER
    /* ��ˮ�趨�¶� */
    if (TYB_SLP_check_data_updated(TYBC_HOT_WATER_SET_VAL, HOT_WATER_SET_IDX))
    {
        SETBIT_N(fg_dpid_upload_pri_high, EXTEND_FUNC_INFO);
    }
#endif
#endif
#endif

/* ��"��ǰ����" */
#ifdef TUYA_USE_ERR_CUR
    /* ���ϸ澯 */
    if (TYB_SLP_check_data_updated(TYBC_ERROR_ALERT_VAL, ERR_ALERT_IDX))
    {
        SETBIT_N(fg_dpid_upload_pri_high, SYS_INFO);
        SETBIT_N(fg_dpid_upload_pri_high, ERR_CUR_INFO);
    }
    /* ��ǰ������ */
    if (TYB_SLP_check_data_updated(TYBC_CUR_ERR_NUM, CUR_ERR_NUM_IDX))
    {
        SETBIT_N(fg_dpid_upload_pri_high, SYS_INFO);
        SETBIT_N(fg_dpid_upload_pri_high, ERR_CUR_INFO);
    }
#endif

/* ��"��������" */
#ifdef TUYA_USE_FAC_PWD
    /* �������� */
    if (TYBC_check_password_modified())
    {
        SETBIT_N(fg_dpid_upload_pri_high, SYS_INFO);
    }
#endif

/* ��"����ҳ" */
#ifdef TUYA_USE_SPLASH
	/* ����ҳLOGO */
    if (TYB_SLP_check_data_updated(TYBC_SPLASH_LOGO_VAL, FIELD_SPLASH_LOGO))
    {
        SETBIT_N(fg_dpid_upload_pri_high, SYS_INFO);
    }
#endif

    for (dpid_idx = 0; dpid_idx < DPID_USED; dpid_idx++)
    {
        if (GETBIT_N(fg_dpid_upload_pri_high, dpid_idx))
        {
            CLRBIT_N(fg_dpid_upload_pri_high, dpid_idx);
            *local_update_dpid_idx = dpid_idx;
            update_cloud_request = TRUE;
            break;
        }
    }

    return update_cloud_request;
}


/*******************************************************************************
 �� ��: TYB_SLP_check_interval_upload()
 �� ��: ����Ƿ���������ϱ�
 �� ��: timer_dpid_idx--�����ϱ����ܵ�������;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
static BOOL TYB_SLP_check_interval_upload(U08 *timer_dpid_idx)
{
    BOOL update_timer_request = FALSE;
    static U08 dpid_idx;
    U16 n;

    for (n = 0; n < DPID_USED; n++)
    {
        if (GETBIT_N(fg_dpid_upload_pri_low, dpid_idx))
        {
            CLRBIT_N(fg_dpid_upload_pri_low, dpid_idx);
            *timer_dpid_idx = dpid_idx;
            update_timer_request = TRUE;
        }
        // ��һ��׼��
        if (++dpid_idx >= DPID_USED)
        {
            dpid_idx = 0;
        }

        if (update_timer_request)
        {
            break;
        }
    }

    return update_timer_request;

}


/*******************************************************************************
 �� ��: TYB_SLP_select_upload_dpid_idx()
 �� ��: ѡ����Ҫ�ϱ��Ĺ��ܵ㣬������Ӧ�ϱ����ȼ���ߡ����ݱ䶯�����ϱ���Ρ���ѯ
        �ϱ����ȼ����
 �� ��: ��
 �� ��: ��Ҫ�ϱ��Ĺ��ܵ�ID����
 �� ��: �ж���Щ��Ҫ�����ϴ�
*******************************************************************************/
U08 TYB_SLP_select_upload_dpid_idx(void)
{
#ifdef TUYA_HAS_TRANS
    U08 channel_trans_dpid_idx = DATA_CHANNEL_01;  /* ��ͨ������ʱʹ�� */
#endif
    U08 regular_upload_dpid_idx = 0;                                        /* �������ϱ�ʱʹ�� */
    U08 all_upload_dpid_idx = 0;                                            /* ����ȫ���ϱ�ʱʹ�� */

    if (!fg_dpid_upload_init)
    {
        fg_dpid_upload_init = TRUE;
        return PROTOCOL_VER;
    }

    /* �������������Ƿ����ı� */
    if (TYB_SLP_check_data_updated(TYBC_LANGUAGE_VAL, LANGUAGE_IDX))
    {
        fg_dpid_upload_all = TRUE;
    }

/* ��ͨ������(��"��������"/"ģ��״̬") */
#ifdef TUYA_HAS_TRANS
    /* ͨ������ */
    if (TYB_SLP_check_channel_transmit(&channel_trans_dpid_idx))
    {
        return channel_trans_dpid_idx;
    }
    /* �����ϱ� */
    else
    {
        /* ȫ���ϱ� */
        if (TYB_SLP_check_all_upload(&all_upload_dpid_idx))
        {
            return all_upload_dpid_idx;
        }
        /* �����ٶ��ϱ� */
        else
        {
            /* �����б仯�����ϱ� */
            if (TYB_SLP_check_local_data_update(&regular_upload_dpid_idx))
            {
                return regular_upload_dpid_idx;
            }
            /* ���ܵ���ѯ�ϱ� */
            else
            {
                if (TYB_SLP_check_interval_upload(&regular_upload_dpid_idx))
                {
                    return regular_upload_dpid_idx;
                }
            }
        }
    }
// �Ѳü��������ù���, �����Ѳü�ģ��״̬ʱ, ��ͨ������
#else
    /* ȫ���ϱ� */
    if (TYB_SLP_check_all_upload(&all_upload_dpid_idx))
    {
        return all_upload_dpid_idx;
    }
    /* �����ٶ��ϱ� */
    else
    {
        /* �����б仯�����ϱ� */
        if (TYB_SLP_check_local_data_update(&regular_upload_dpid_idx))
        {
            return regular_upload_dpid_idx;
        }
        /* ���ܵ���ѯ�ϱ� */
        else
        {
            if (TYB_SLP_check_interval_upload(&regular_upload_dpid_idx))
            {
                return regular_upload_dpid_idx;
            }
        }
    }
#endif

    return DPID_IDX_NULL;
}


/*******************************************************************************
 �� ��: TYB_SLP_concat_ver_info()
 �� ��: �汾��Ϣƴ�ӣ���ƴ�ӳ���Ԥ��
 �� ��: buf--�ַ���������;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TYB_SLP_concat_ver_info(U08 *buf)
{
    U08 buf_len = 0;

/* ��ʾ��ʾ���汾 */
#if (TYBC_DISP_HMI_VER_EN == TRUE)
#if (TYBC_STR_CODE_TYPE == UNI_CODE)
    TYB_SLP_ascii_to_unicode(&tyb_hmi_ver_unicode[0], &tyb_hmi_ver[0], MAX_VER_BYTE*2);
    /* �ֶ�0: ��ʾ���汾 */
    TYB_create_one_msg_field(&buf[buf_len], \
                             &buf_len, \
                             tyb_hmi_ver_unicode, \
                             MAX_VER_BYTE*2);
#else
    /* �ֶ�0: ��ʾ���汾 */
    TYB_create_one_msg_field(&buf[buf_len], \
                             &buf_len, \
                             tyb_hmi_ver, \
                             MAX_VER_BYTE);
#endif
/* ����ʾ��ʾ���汾 */
#else
    buf[buf_len++] = FIELD_ONE_MSG;
    buf[buf_len++] = FIELD_DISABLED;
    buf[buf_len++] = 0;
#endif

/* ��ʾ�������汾 */
#if (TYBC_DISP_BKBD_VER_EN == TRUE)
#if (TYBC_STR_CODE_TYPE == UNI_CODE)
	TYB_SLP_ascii_to_unicode(&tyb_bkbd_ver_unicode[0], (far U08 *)TYBC_BKBD_VER, MAX_VER_BYTE*2);
    /* �ֶ�0: ��ʾ���汾 */
    TYB_create_one_msg_field(&buf[buf_len], \
                             &buf_len, \
                             tyb_bkbd_ver_unicode, \
                             MAX_VER_BYTE*2);
#else
    /* �ֶ�1: �������汾 */
    TYB_create_one_msg_field(&buf[buf_len], \
                             &buf_len, \
                             (far U08 *)TYBC_BKBD_VER, \
                             (sizeof(TYBC_BKBD_VER)/sizeof(U08)));
#endif
/* ����ʾ�������汾 */
#else
    buf[buf_len++] = FIELD_ONE_MSG;
    buf[buf_len++] = FIELD_DISABLED;
    buf[buf_len++] = 0;
#endif
}


/*******************************************************************************
 �� ��: TYB_SLP_concat_sys_info()
 �� ��: ϵͳ��Ϣƴ�ӣ���ƴ�ӳ���Ԥ��
 �� ��: buf--�ַ���������;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TYB_SLP_concat_sys_info(U08 *buf)
{
    U08 buf_len = 0;
    U16 fg_func_en = 0;

    /* �ֶ�0: �ַ��������� */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_STR_CODE_TYPE, FIELD_READONLY, FIELD_STR_CODE_TYPE);

    /* �ֶ�1: ϵͳά�� */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_SYS_MAINTENANCE_VAL, FIELD_READONLY, FIELD_SYS_MAINT);

    /* �ֶ�2: ���� */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_LANGUAGE_VAL, FIELD_READONLY, FIELD_CNTRL_LANG);

    /* �ֶ�3: ���ϸ澯 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_ERROR_ALERT_VAL, FIELD_READONLY, FIELD_ERR_ALERT);

    /* �ֶ�4: ģ���� */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_MOD_NUM, FIELD_READONLY, FIELD_MOD_NUM);

    /* �ֶ�5: �������� */
    TYB_create_one_msg_field(&buf[buf_len], &buf_len, TYBC_PASSWORD_POINTER, TYBC_PWD_MAX);

    /* �ֶ�6: �����л�ģʽ���� */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_RUN_SWITCH_MODE_EN, FIELD_READONLY, FIELD_RUN_STATUS);

    /* �ֶ�7: ����ҳLOGO */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_SPLASH_LOGO_VAL, FIELD_READONLY,FIELD_SPLASH_LOGO);

    /* �ֶ�8: �������� */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_SMART_SPEAKER_EN, FIELD_READONLY, FIELD_SPEECH_EN);

/* ��"��������" */
#ifdef TUYA_USE_PARAM_SET
    /* �ֶ�9: ����������(Ԥ�������) */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_PARAM_GRP_MAX, FIELD_READONLY, FIELD_GRP_MAX);

    /* �ֶ�10: ����������(Ԥ�������) */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_PARAM_GRP_SIZE, FIELD_READONLY, FIELD_ITEM_MAX);
#else
    /* �ֶ�9: ����������(0) */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, 0, FIELD_READONLY, FIELD_GRP_MAX);

    /* �ֶ�10: ����������(0) */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, 0, FIELD_READONLY, FIELD_ITEM_MAX);
#endif

    /* �ֶ�11: ��ѡ���� */
/* ��"ͨ������" */
#ifdef TUYA_HAS_GEN_SET
    fg_func_en |= TYBC_FUNC_EN_MORE_GEN_SET << FUNC_EN_MORE_GEN_SET_BIT;
#endif
/* ��"��ʷ����" */
#ifdef TUYA_USE_ERR_HIS
    fg_func_en |= TYBC_FUNC_EN_ADVANCE_ERR_HIS << FUNC_EN_ADVANCE_ERR_HIS_BIT;
#endif
/* ��"��������" */
#ifdef TUYA_USE_PARAM_SET
    fg_func_en |= TYBC_FUNC_EN_ADVANCE_PARAM_SET << FUNC_EN_ADVANCE_PARAM_SET_BIT;
#endif
/* ��"״̬��ѯ" */
#ifdef TUYA_HAS_STAT
    fg_func_en |= TYBC_FUNC_EN_MAIN_STAT << FUNC_EN_MAIN_STAT_BIT;
    fg_func_en |= TYBC_FUNC_EN_ADVANCE_STAT << FUNC_EN_ADVANCE_STAT_BIT;
#endif
/* ��"�õ��ѯ" */
#ifdef TUYA_USE_ELE_STAT
    fg_func_en |= TYBC_FUNC_EN_MAIN_ELE_STAT << FUNC_EN_MAIN_ELE_STAT_BIT;
#endif
/* ��"�ƶ�ʱ" */
#ifdef TUYA_USE_CLO_TIME
    fg_func_en |= TYBC_FUNC_EN_MORE_CLO_TIME << FUNC_EN_MORE_CLO_TIME_BIT;
#endif
    TYB_create_one_num_field(&buf[buf_len], &buf_len, fg_func_en, FIELD_READONLY, FIELD_FUNC_OPT);
}


/*******************************************************************************
 �� ��: TYB_SLP_concat_protocol_ver()
 �� ��: Э��汾ƴ�ӣ���ƴ�ӳ���Ԥ��
 �� ��: buf--�ַ���������;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TYB_SLP_concat_protocol_ver(U08 *buf)
{
    U08 buf_len = 0;

    /* �ֶ�0: Э��汾 */
    TYB_create_one_msg_field(&buf[buf_len], \
                             &buf_len, \
                             (far U08 *)TYB_SLP_PROTOCOL_VER, \
                             (sizeof(TYB_SLP_PROTOCOL_VER)/sizeof(U08)));
}


/* ��"ͨ�����ù���" */
#ifdef TUYA_HAS_GEN_SET
/*******************************************************************************
 �� ��: TYB_SLP_concat_general_set_info()
 �� ��: ������Ϣ�ֶ�ƴ�ӣ���ƴ�ӳ���Ԥ��
 �� ��: buf--�ַ���������;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TYB_SLP_concat_general_set_info(U08 *buf)
{
    U08 buf_len = 0;

/* ��"���Թ���" */
#ifdef TUYA_USE_LANG_SET
    /* �ֶ�0: ���� */
    TYB_create_enum_field(&buf[buf_len], \
                          &buf_len, \
                          FIELD_WRITABLE, \
                          FIELD_LANGUAGE, \
                          TYBC_LANGUAGE_TYPE, \
                          TYBC_LANGUAGE_TITLE, \
                          TYBC_LANGUAGE_RANGE, \
                          TYBC_LANGUAGE_MAX_NUM, \
                          TYBC_LANGUAGE_VAL);
#endif

/* ��"����ҳ����" */
#ifdef TUYA_USE_SPLASH
    /* �ֶ�1: ����ҳ */
    TYB_create_bool_field(&buf[buf_len], \
                          &buf_len, \
                          FIELD_WRITABLE, \
                          FIELD_SPLASH, \
                          TYBC_SPLASH_TYPE, \
                          TYBC_SPLASH_TITLE, \
                          TYBC_SPLASH_VAL);
#endif

/* ��"ͯ������" */
#ifdef TUYA_USE_CHILDLOCK
    /* �ֶ�2: ͯ�� */
    TYB_create_bool_field(&buf[buf_len], \
                          &buf_len, \
                          FIELD_WRITABLE, \
                          FIELD_CHILDLOCK, \
                          TYBC_CHILDLOCK_TYPE, \
                          TYBC_CHILD_LOCK_TITLE, \
                          TYBC_CHILDLOCK_VAL);
#endif

	/* �ֶ�3: ����һʹ�� */
    TYB_create_bool_field(&buf[buf_len], \
                          &buf_len, \
                          FIELD_READONLY, \
                          FIELD_ZoneA_EN, \
                          TYBC_ZoneA_EN_TYPE, \
                          TYBC_ZoneA_EN_TITLE, \
                          TYBC_ZoneA_EN_VAL);

	/* �ֶ�4: �����ʹ�� */
    TYB_create_bool_field(&buf[buf_len], \
                          &buf_len, \
                          FIELD_READONLY, \
                          FIELD_ZoneB_EN, \
                          TYBC_ZoneB_EN_TYPE, \
                          TYBC_ZoneB_EN_TITLE, \
                          TYBC_ZoneB_EN_VAL);

	/* �ֶ�5: ��ˮʹ�� */
    TYB_create_bool_field(&buf[buf_len], \
                          &buf_len, \
                          FIELD_READONLY, \
                          FIELD_DHW_EN, \
                          TYBC_DHW_EN_TYPE, \
                          TYBC_DHW_EN_TITLE, \
                          TYBC_DHW_EN_VAL);
/* ��"���Թ���" */
#ifdef TUYA_USE_TEMP_UNIT_SET
    /* �ֶ�0: ���� */
    TYB_create_enum_field(&buf[buf_len], \
                          &buf_len, \
                          FIELD_WRITABLE, \
                          FIELD_TEMP_UNIT, \
                          TYBC_TEMP_UNIT_TYPE, \
                          TYBC_TEMP_UNIT_TITLE, \
                          TYBC_TEMP_UNIT_RANGE, \
                          TYBC_TEMP_UNIT_MAX_NUM, \
                          TYBC_TEMP_UNIT_VAL);
#endif
}
#endif


/*******************************************************************************
 �� ��: TYB_SLP_concat_zone1_func_info()
 �� ��: ����һ������Ϣ�ֶ�ƴ�ӣ���ƴ�ӳ���Ԥ��
 �� ��: buf--�ַ���������;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TYB_SLP_concat_zone1_func_info(U08 *buf)
{
    U08 buf_len = 0;

    /* �ֶ�0: ���ػ� */
    TYB_create_bool_field(&buf[buf_len], \
                          &buf_len, \
                          FIELD_WRITABLE, \
                          FIELD_AREA_SWITCH, \
                          TYBC_SWITCH_TYPE, \
                          TYBC_SWITCH_TITLE, \
                          TYBC_SWITCH_VAL);

    /* �ֶ�1: �趨�¶� */
    TYB_create_number_field(&buf[buf_len], \
                           &buf_len, \
                           FIELD_WRITABLE, \
                           FIELD_AREA_TEMP_SET, \
                           TYBC_TEMP_SET_TYPE, \
                           TYBC_TEMP_SET_TITLE, \
                           TYBC_TEMP_SET_ATTR, \
                           TYBC_TEMP_SET_VAL, \
                           TYBC_TEMP_SET_MAX, \
                           TYBC_TEMP_SET_MIN);

    /* �ֶ�2: ��ǰ�¶� */
    TYB_create_ro_num_field(&buf[buf_len], \
                            &buf_len, \
                            FIELD_READONLY, \
                            FIELD_AREA_TEMP_CUR, \
                            TYBC_TEMP_CUR_TYPE, \
                            TYBC_TEMP_CUR_TITLE, \
                            TYBC_TEMP_CUR_ATTR, \
                            TYBC_TEMP_CUR_VAL);

    /* �ֶ�3: �趨ģʽ */
    TYB_create_enum_field(&buf[buf_len], \
                          &buf_len, \
                          FIELD_WRITABLE, \
                          FIELD_AREA_MODE_SET,\
                          TYBC_MODE_SET_TYPE, \
                          TYBC_MODE_SET_TITLE, \
                          TYBC_MODE_SET_RANGE, \
                          TYBC_MODE_SET_MAX_NUM, \
                          TYBC_MODE_SET_VAL);

    /* �ֶ�4: ��ǰģʽ */
    TYB_create_enum_field(&buf[buf_len], \
                          &buf_len, \
                          FIELD_READONLY, \
                          FIELD_AREA_MODE_CUR, \
                          TYBC_MODE_CUR_TYPE, \
                          TYBC_MODE_CUR_TITLE, \
                          TYBC_MODE_CUR_RANGE, \
                          TYBC_MODE_CUR_MAX_NUM, \
                          TYBC_MODE_CUR_VAL);

    /* �ֶ�5: ����ĩ�� */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_CTRL_END_VALUE, FIELD_READONLY, FIELD_AREA_CTRL_WAY);

    /* �ֶ�6: ���ƶ��� */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_CTRL_OBJ, FIELD_READONLY, FIELD_AREA_CTRL_obj);

    /* �ֶ�7: ѹ��״̬ */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_COMP_STA, FIELD_READONLY, FIELD_AREA_COMP_STA);

    /* �ֶ�8: ��˪״̬ */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_DEFRO_STA, FIELD_READONLY, FIELD_AREA_DEFRO_STA);

    /* �ֶ�9: ����״̬ */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_ANFREZ_STA, FIELD_READONLY, FIELD_AREA_ANFREZ_STA);

    /* �ֶ�10: �ⲿ��Դ���״̬ */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_OUTHEAT_STA, FIELD_READONLY, FIELD_AREA_OUTHEAT_STA);

    /* �ֶ�11: ɱ��״̬ */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_STERILIZE_STA, FIELD_READONLY, FIELD_AREA_STERILIZE_STA);

    /* �ֶ�12: ��������״̬ */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_AUXHEAT_STA, FIELD_READONLY, FIELD_AREA_AUXHEAT_STA);

    /* �ֶ�13: ˮ�����״̬ */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_TANKHEAT_STA, FIELD_READONLY, FIELD_AREA_TANKHEAT_STA);

    /* �ֶ�14: ����ģʽ״̬ */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_SILENT_STA, FIELD_READONLY, FIELD_AREA_SILENT_STA);

    /* �ֶ�15: ����ģʽ״̬ */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_ENESAVING_STA, FIELD_READONLY, FIELD_AREA_ENESAVING_STA);

    /* �ֶ�16: ����ģʽ״̬ */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_HOLIDAY_STA, FIELD_READONLY, FIELD_AREA_HOLIDAY_STA);

    /* �ֶ�17: ̫�����ź�״̬ */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_SOLAR_STA, FIELD_READONLY, FIELD_AREA_SOLAR_STA);
}

/*******************************************************************************
 �� ��: TYB_SLP_concat_zone2_func_info()
 �� ��: �����������Ϣ�ֶ�ƴ�ӣ���ƴ�ӳ���Ԥ��
 �� ��: buf--�ַ���������;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TYB_SLP_concat_zone2_func_info(U08 *buf)
{
    U08 buf_len = 0;

    /* �ֶ�0: ���ػ� */
    TYB_create_bool_field(&buf[buf_len], \
                          &buf_len, \
                          FIELD_WRITABLE, \
                          FIELD_AREA_SWITCH, \
                          TYBC_ZoneB_SWITCH_TYPE, \
                          TYBC_ZoneB_SWITCH_TITLE, \
                          TYBC_ZoneB_SWITCH_VAL);

    /* �ֶ�1: �趨�¶� */
    TYB_create_number_field(&buf[buf_len], \
                           &buf_len, \
                           FIELD_WRITABLE, \
                           FIELD_AREA_TEMP_SET, \
                           TYBC_ZoneB_TEMP_SET_TYPE, \
                           TYBC_ZoneB_TEMP_SET_TITLE, \
                           TYBC_ZoneB_TEMP_SET_ATTR, \
                           TYBC_ZoneB_TEMP_SET_VAL, \
                           TYBC_ZoneB_TEMP_SET_MAX, \
                           TYBC_ZoneB_TEMP_SET_MIN);

    /* �ֶ�2: ��ǰ�¶� */
    TYB_create_ro_num_field(&buf[buf_len], \
                            &buf_len, \
                            FIELD_READONLY, \
                            FIELD_AREA_TEMP_CUR, \
                            TYBC_ZoneB_TEMP_CUR_TYPE, \
                            TYBC_ZoneB_TEMP_CUR_TITLE, \
                            TYBC_ZoneB_TEMP_CUR_ATTR, \
                            TYBC_ZoneB_TEMP_CUR_VAL);

    /* �ֶ�3: �趨ģʽ */
    TYB_create_enum_field(&buf[buf_len], \
                          &buf_len, \
                          FIELD_WRITABLE, \
                          FIELD_AREA_MODE_SET,\
                          TYBC_ZoneB_MODE_SET_TYPE, \
                          TYBC_ZoneB_MODE_SET_TITLE, \
                          TYBC_ZoneB_MODE_SET_RANGE, \
                          TYBC_ZoneB_MODE_SET_MAX_NUM, \
                          TYBC_ZoneB_MODE_SET_VAL);

    /* �ֶ�4: ��ǰģʽ */
    TYB_create_enum_field(&buf[buf_len], \
                          &buf_len, \
                          FIELD_READONLY, \
                          FIELD_AREA_MODE_CUR, \
                          TYBC_ZoneB_MODE_CUR_TYPE, \
                          TYBC_ZoneB_MODE_CUR_TITLE, \
                          TYBC_ZoneB_MODE_CUR_RANGE, \
                          TYBC_ZoneB_MODE_CUR_MAX_NUM, \
                          TYBC_ZoneB_MODE_CUR_VAL);

    /* �ֶ�5: ����ĩ�� */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_ZoneB_CTRL_END_VALUE, FIELD_READONLY, FIELD_AREA_CTRL_WAY);

    /* �ֶ�6: ���ƶ��� */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_ZoneB_CTRL_OBJ, FIELD_READONLY, FIELD_AREA_CTRL_obj);

    /* �ֶ�7: ѹ��״̬ */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_ZoneB_COMP_STA, FIELD_READONLY, FIELD_AREA_COMP_STA);

    /* �ֶ�8: ��˪״̬ */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_ZoneB_DEFRO_STA, FIELD_READONLY, FIELD_AREA_DEFRO_STA);

    /* �ֶ�9: ����״̬ */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_ZoneB_ANFREZ_STA, FIELD_READONLY, FIELD_AREA_ANFREZ_STA);

    /* �ֶ�10: �ⲿ��Դ���״̬ */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_ZoneB_OUTHEAT_STA, FIELD_READONLY, FIELD_AREA_OUTHEAT_STA);

    /* �ֶ�11: ɱ��״̬ */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_ZoneB_STERILIZE_STA, FIELD_READONLY, FIELD_AREA_STERILIZE_STA);

    /* �ֶ�12: ��������״̬ */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_ZoneB_AUXHEAT_STA, FIELD_READONLY, FIELD_AREA_AUXHEAT_STA);

    /* �ֶ�13: ˮ�����״̬ */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_ZoneB_TANKHEAT_STA, FIELD_READONLY, FIELD_AREA_TANKHEAT_STA);

    /* �ֶ�14: ����ģʽ״̬ */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_ZoneB_SILENT_STA, FIELD_READONLY, FIELD_AREA_SILENT_STA);

    /* �ֶ�15: ����ģʽ״̬ */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_ZoneB_ENESAVING_STA, FIELD_READONLY, FIELD_AREA_ENESAVING_STA);

    /* �ֶ�16: ����ģʽ״̬ */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_ZoneB_HOLIDAY_STA, FIELD_READONLY, FIELD_AREA_HOLIDAY_STA);

    /* �ֶ�17: ̫�����ź�״̬ */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_ZoneB_SOLAR_STA, FIELD_READONLY, FIELD_AREA_SOLAR_STA);

}

/*******************************************************************************
 �� ��: TYB_SLP_concat_DHW_func_info()
 �� ��: ��ˮ������Ϣ�ֶ�ƴ�ӣ���ƴ�ӳ���Ԥ��
 �� ��: buf--�ַ���������;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TYB_SLP_concat_DHW_func_info(U08 *buf)
{
    U08 buf_len = 0;

    /* �ֶ�0: ���ػ� */
    TYB_create_bool_field(&buf[buf_len], \
                          &buf_len, \
                          FIELD_WRITABLE, \
                          FIELD_AREA_SWITCH, \
                          TYBC_DHW_SWITCH_TYPE, \
                          TYBC_DHW_SWITCH_TITLE, \
                          TYBC_DHW_SWITCH_VAL);

    /* �ֶ�1: �趨�¶� */
    TYB_create_number_field(&buf[buf_len], \
                           &buf_len, \
                           FIELD_WRITABLE, \
                           FIELD_AREA_TEMP_SET, \
                           TYBC_DHW_TEMP_SET_TYPE, \
                           TYBC_DHW_TEMP_SET_TITLE, \
                           TYBC_DHW_TEMP_SET_ATTR, \
                           TYBC_DHW_TEMP_SET_VAL, \
                           TYBC_DHW_TEMP_SET_MAX, \
                           TYBC_DHW_TEMP_SET_MIN);

    /* �ֶ�2: ��ǰ�¶� */
    TYB_create_ro_num_field(&buf[buf_len], \
                            &buf_len, \
                            FIELD_READONLY, \
                            FIELD_AREA_TEMP_CUR, \
                            TYBC_DHW_TEMP_CUR_TYPE, \
                            TYBC_DHW_TEMP_CUR_TITLE, \
                            TYBC_DHW_TEMP_CUR_ATTR, \
                            TYBC_DHW_TEMP_CUR_VAL);

    /* �ֶ�3: �趨ģʽ */
    TYB_create_enum_field(&buf[buf_len], \
                          &buf_len, \
                          FIELD_WRITABLE, \
                          FIELD_AREA_MODE_SET,\
                          TYBC_DHW_MODE_SET_TYPE, \
                          TYBC_DHW_MODE_SET_TITLE, \
                          TYBC_DHW_MODE_SET_RANGE, \
                          TYBC_DHW_MODE_SET_MAX_NUM, \
                          TYBC_DHW_MODE_SET_VAL);

    /* �ֶ�4: ��ǰģʽ */
    TYB_create_enum_field(&buf[buf_len], \
                          &buf_len, \
                          FIELD_READONLY, \
                          FIELD_AREA_MODE_CUR, \
                          TYBC_DHW_MODE_CUR_TYPE, \
                          TYBC_DHW_MODE_CUR_TITLE, \
                          TYBC_DHW_MODE_CUR_RANGE, \
                          TYBC_DHW_MODE_CUR_MAX_NUM, \
                          TYBC_DHW_MODE_CUR_VAL);

    /* �ֶ�5: ����ĩ�� */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_DHW_CTRL_END_VALUE, FIELD_READONLY, FIELD_AREA_CTRL_WAY);

    /* �ֶ�6: ���ƶ��� */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_DHW_CTRL_OBJ, FIELD_READONLY, FIELD_AREA_CTRL_obj);

    /* �ֶ�7: ѹ��״̬ */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_DHW_COMP_STA, FIELD_READONLY, FIELD_AREA_COMP_STA);

    /* �ֶ�8: ��˪״̬ */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_DHW_DEFRO_STA, FIELD_READONLY, FIELD_AREA_DEFRO_STA);

    /* �ֶ�9: ����״̬ */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_DHW_ANFREZ_STA, FIELD_READONLY, FIELD_AREA_ANFREZ_STA);

    /* �ֶ�10: �ⲿ��Դ���״̬ */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_DHW_OUTHEAT_STA, FIELD_READONLY, FIELD_AREA_OUTHEAT_STA);

    /* �ֶ�11: ɱ��״̬ */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_DHW_STERILIZE_STA, FIELD_READONLY, FIELD_AREA_STERILIZE_STA);

    /* �ֶ�12: ��������״̬ */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_DHW_AUXHEAT_STA, FIELD_READONLY, FIELD_AREA_AUXHEAT_STA);

    /* �ֶ�13: ˮ�����״̬ */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_DHW_TANKHEAT_STA, FIELD_READONLY, FIELD_AREA_TANKHEAT_STA);

    /* �ֶ�14: ����ģʽ״̬ */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_DHW_SILENT_STA, FIELD_READONLY, FIELD_AREA_SILENT_STA);

    /* �ֶ�15: ����ģʽ״̬ */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_DHW_ENESAVING_STA, FIELD_READONLY, FIELD_AREA_ENESAVING_STA);

    /* �ֶ�16: ����ģʽ״̬ */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_DHW_HOLIDAY_STA, FIELD_READONLY, FIELD_AREA_HOLIDAY_STA);

    /* �ֶ�17: ̫�����ź�״̬ */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_DHW_SOLAR_STA, FIELD_READONLY, FIELD_AREA_SOLAR_STA);

}

/* ��"��չ����" */
#ifdef TUYA_HAS_EXT_FUNC
/*******************************************************************************
 �� ��: TYB_SLP_concat_extend_func_info()
 �� ��: ��չ������Ϣ�ֶ�ƴ�ӣ���ƴ�ӳ���Ԥ��
 �� ��: buf--�ַ���������;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TYB_SLP_concat_extend_func_info(U08 *buf)
{
    U08 buf_len = 0;

/* ��"���ٹ���" */
#ifdef TUYA_USE_WIND
    /* �ֶ�0: �趨���� */
    TYB_create_enum_field(&buf[buf_len], \
                          &buf_len, \
                          FIELD_WRITABLE, \
                          FIELD_WIND_SET, \
                          TYBC_WIND_SET_TYPE, \
                          TYBC_WIND_SET_TITLE, \
                          TYBC_WIND_SET_RANGE, \
                          TYBC_WIND_SET_MAX_NUM, \
                          TYBC_WIND_SET_VAL);

    /* �ֶ�1: ��ǰ���� */
    TYB_create_enum_field(&buf[buf_len], \
                          &buf_len, \
                          FIELD_WRITABLE, \
                          FIELD_WIND_CUR, \
                          TYBC_WIND_CUR_TYPE, \
                          TYBC_WIND_CUR_TITLE, \
                          TYBC_WIND_CUR_RANGE, \
                          TYBC_WIND_CUR_MAX_NUM, \
                          TYBC_WIND_CUR_VAL);
#endif

/* ��"ʪ�ȹ���" */
#ifdef TUYA_USE_HUMI
    /* �ֶ�2: �趨ʪ�� */
    TYB_create_number_field(&buf[buf_len], \
                            &buf_len, \
                            FIELD_WRITABLE, \
                            FIELD_HUMI_SET, \
                            TYBC_HUMI_SET_TYPE, \
                            TYBC_HUMI_SET_TITLE, \
                            TYBC_HUMI_SET_ATTR, \
                            TYBC_HUMI_SET_VAL, \
                            TYBC_HUMI_SET_MAX, \
                            TYBC_HUMI_SET_MIN);

    /* �ֶ�3: ��ǰʪ�� */
    TYB_create_ro_num_field(&buf[buf_len], \
                            &buf_len, \
                            FIELD_READONLY, \
                            FIELD_HUMI_CUR, \
                            TYBC_HUMI_CUR_TYPE, \
                            TYBC_HUMI_CUR_TITLE, \
                            TYBC_HUMI_CUR_ATTR, \
                            TYBC_HUMI_CUR_VAL);
#endif

/* ��"��ˮ����" */
#ifdef TUYA_USE_HOT_WATER
    /* �ֶ�4: ��ˮ�趨�¶� */
    TYB_create_number_field(&buf[buf_len], \
                            &buf_len, \
                            FIELD_WRITABLE, \
                            FIELD_HOT_WATER_SET, \
                            TYBC_HOT_WATER_SET_TYPE, \
                            TYBC_HOT_WATER_SET_TITLE, \
                            TYBC_HOT_WATER_SET_ATTR, \
                            TYBC_HOT_WATER_SET_VAL, \
                            TYBC_HOT_WATER_SET_MAX, \
                            TYBC_HOT_WATER_SET_MIN);

    /* �ֶ�5: ��ǰ��ˮ�¶� */
    TYB_create_ro_num_field(&buf[buf_len], \
                            &buf_len, \
                            FIELD_READONLY, \
                            FIELD_HOT_WATER_CUR, \
                            TYBC_HOT_WATER_CUR_TYPE, \
                            TYBC_HOT_WATER_CUR_TITLE, \
                            TYBC_HOT_WATER_CUR_ATTR, \
                            TYBC_HOT_WATER_CUR_VAL);
#endif
}
#endif

/* ��"��ǰ����" */
#ifdef TUYA_USE_ERR_CUR
/*******************************************************************************
 �� ��: TYB_SLP_concat_err_cur_info()
 �� ��: ��ǰ������Ϣƴ�ӣ���ƴ�ӳ���Ԥ��
 �� ��: buf--�ַ���������;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TYB_SLP_concat_err_cur_info(U08 *buf)
{
    U16 i,mod_id,sys_err_id,mod_err_id;
    TYBC_ERR_CUR err;                                 /* ������Ϣ */
    //U08 useless_cache[30];
    //U16 msg_len = 0;
    U16 acc_bytes = 0;
    U08 buf_len = 0;

    MEMCLR(&err, ((sizeof(TYBC_ERR_CUR)+1)/sizeof(U16))*2);

    sys_err_id = mod_id = mod_err_id =0;
    for (i = 0; i < TY_CUR_ERR_MAX; i++)
    {
        if (TYBC_GET_ERR_CUR(&err, &sys_err_id, &mod_id, &mod_err_id))
        {
            acc_bytes = buf_len + DP_FIELD_ERR_CUR_INFO_BYTES;

            /* �ռ䲻��������ֹ������� */
            if (acc_bytes > RAW_DATA_BYTES)
            {
                return;
            }
            else
            {
                buf[buf_len++] = HIBYTE(err.id);
                buf[buf_len++] = LOBYTE(err.id);
                buf[buf_len++] = HIBYTE(err.err_id);
                buf[buf_len++] = LOBYTE(err.err_id);
            }
        }
    }
}
#endif


/* ��"��ʷ����" */
#ifdef TUYA_USE_ERR_HIS
/*******************************************************************************
 �� ��: TYB_SLP_concat_err_his_info()
 �� ��: ��ʷ������Ϣƴ�ӣ���ƴ�ӳ���Ԥ��
 �� ��: buf--�ַ���������;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TYB_SLP_concat_err_his_info(U08 *buf)
{
    U08 i;
    TYBC_ERR_HIS err_record;                           /* ������Ϣ */
    U08 useless_cache[TYBC_STR_BYTE_MAX];
    U16 msg_len = 0;
    U16 acc_bytes = 0;
    U08 buf_len = 0;

    MEMCLR(&err_record, ((sizeof(TYBC_ERR_HIS)+1)/sizeof(U16))*2);

    for (i = 0; i < TY_HIS_ERR_MAX; i++)
    {
        /* ��ȡ��ʷ���� */
        TYBC_GET_ERR_HIS(&err_record,i);

        /* �й��� */
        if (err_record.err_id < TYBC_NO_ERR)
        {
            acc_bytes = buf_len + DP_FIELD_HIS_ERR_INFO_BYTES + TYB_SLP_GET_CODE_STR(useless_cache, err_record.name);

            /* �ռ䲻��������ֹ������� */
            if (acc_bytes > RAW_DATA_BYTES)
            {
                return;
            }
            else
            {
                msg_len = 0;
                buf[buf_len++] = HIBYTE(err_record.id);
                buf[buf_len++] = LOBYTE(err_record.id);
                buf[buf_len++] = err_record.time.month;
                buf[buf_len++] = err_record.time.day;
                buf[buf_len++] = err_record.time.hour;
                buf[buf_len++] = err_record.time.mint;
                msg_len = TYB_SLP_GET_CODE_STR(&buf[buf_len+2], err_record.name);
                buf[buf_len++] = HIBYTE(msg_len);
                buf[buf_len++] = LOBYTE(msg_len);
                buf_len += msg_len;
            }
        }
    }
}
#endif


#ifdef TUYA_HAS_STAT
/*******************************************************************************
 �� ��: TYB_SLP_mark_upload_pv()
 �� ��: �����Ҫ�ϴ���ģ����
 �� ��: mod_id--ģ���;
        upload_pv_total--��Ҫ�ϴ���ģ��������;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
static BOOL TYB_SLP_mark_upload_pv(U08 mod_id, U08 *upload_pv_total)
{
    U16 i,j,idx;
    U16 comp_pv_id = 0;
    U08 upload_cnt = 0;

    /* ģ���У�� */
    if (mod_id > TYBC_PCB_MAX)
    {
        return FALSE;
    }

    /* ��ʼ�� */
    ARRCLR(fg_upload_pv);

    /* ��¼��Ҫ�ϱ���ģ���� */
    if (mod_id == TYBC_PCB_MAX)    /* ϵͳ */
    {
        for (i = 0; i < TYBC_SYS_PV_MAX; i++)
        {
            if (TYBC_PV_VAL(mod_id, i) != SENSOR_NOEXIST)
            {
                SETBIT_N(fg_upload_pv, i);
                upload_cnt++;
            }
        }
    }
    else    /* ģ�� */
    {
        for (i = 0; i < TYBC_PV_MAX; i++)
        {
            if (TYBC_PV_VAL(mod_id, i) != SENSOR_NOEXIST)
            {
                SETBIT_N(fg_upload_pv, i);
                upload_cnt++;

                if (i < TYBC_UNIT_PV_BEGIN)    /* ѹ��ģ������ͬ����� */
                {
                    comp_pv_id = i % TYBC_COMP_PV_MAX;
                    for (j = 0; j < TYBC_COMP_MAX; j++)
                    {
                        idx = j * TYBC_COMP_PV_MAX + comp_pv_id;
                        if (!GETBIT_N(fg_upload_pv, idx))
                        {
                            SETBIT_N(fg_upload_pv, idx);
                            upload_cnt++;
                        }
                    }

                }
            }
        }
    }

    *upload_pv_total = upload_cnt;

    return TRUE;
}
#endif


/* ��"ϵͳ״̬" */
#ifdef TUYA_USE_SYS_STAT
/*******************************************************************************
 �� ��: TYB_SLP_concat_sys_stat_info()
 �� ��: ϵͳ״̬��Ϣƴ�ӣ����ֶ����������ж�
 �� ��: buf--�ַ���������;
        dpid_idx--���ܵ�ID����;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TYB_SLP_concat_sys_stat_info(U08 *buf, U08 dpid_idx)
{
    U08 i;
    static U08 idx_rec = 0;              /* ��¼��һ�����ܵ����ʼ�������� */
    static BOOL fg_all_upload_pv = FALSE;
    U08 upload_pv_num = 0;
    U08 useless_cache[20];
    U16 acc_bytes = 0;
    BOOL fg_buf_full = FALSE;
    U16 attr, msg_len;
    U08 buf_len = 0;

    /* �����Ҫ�ϴ���ģ���� */
    if (!TYB_SLP_mark_upload_pv(TYBC_PCB_MAX, &upload_pv_num))
    {
        return;
    }

    /* ��Чģ����δ�ϴ��� */
    if (!fg_all_upload_pv)
    {
        for (i = idx_rec; i < TYBC_SYS_PV_MAX; i++)
        {
            msg_len = 0;
            /* ��Чģ���� */
            if (GETBIT_N(fg_upload_pv, i))
            {
                msg_len = TYB_SLP_GET_CODE_STR(useless_cache, TYBC_SYS_PV_TITLE(i));
                acc_bytes = acc_bytes + DP_FIELD_SYS_STAT_INFO_BYTES + msg_len;

                /* д���ϱ����ܵ㻺���� */
                if (acc_bytes <= RAW_DATA_BYTES - 2)
                {
                    if (!fg_buf_full)
                    {
                        attr = TYBC_SYS_PV_ATTR(i);
                        buf[buf_len++] = HIBYTE(attr);
                        buf[buf_len++] = LOBYTE(attr);
                        buf[buf_len++] = HIBYTE(TYBC_PV_VAL(TYBC_PCB_MAX, i));
                        buf[buf_len++] = LOBYTE(TYBC_PV_VAL(TYBC_PCB_MAX, i));
                        buf[buf_len++] = HIBYTE(msg_len);
                        buf[buf_len++] = LOBYTE(msg_len);
                        buf_len += TYB_SLP_GET_CODE_STR(&buf[buf_len], TYBC_SYS_PV_TITLE(i));
                    }
                }
                else
                {
                    if (!fg_buf_full)
                    {
                        fg_buf_full = TRUE;
                        idx_rec = i;
                    }
                    acc_bytes = DP_FIELD_SYS_STAT_INFO_BYTES + msg_len;
                }
            }
        }
    }

    /* �Ѵ�����Ч���� */
    if (!fg_buf_full)
    {
        fg_all_upload_pv = TRUE;
        idx_rec = 0;
    }

    /* 4�����ܵ����һ����ѯ����¼���� */
    if (DP[dpid_idx].dpid == DPID_SYS_STAT_INFO_4)
    {
        fg_all_upload_pv = FALSE;
        idx_rec = 0;
    }

    /* ������־ */
    if (buf_len != 0 && buf_len <= RAW_DATA_BYTES - 2)
    {
        buf[buf_len++] = 0xff;
        buf[buf_len++] = 0xff;
    }
}


/*******************************************************************************
 �� ��: TYB_SLP_concat_sys_stat_data()
 �� ��: ϵͳ״̬����ƴ�ӣ����ֶ����������ж�
 �� ��: buf--�ַ���������;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TYB_SLP_concat_sys_stat_data(U08 *buf)
{
    U08 i;
    static U08 upload_pv_num = 0;
    U08 upload_pv_num_prev;
    U16 acc_bytes = 0;
    U16 attr;
    U08 buf_len = 0;

    /* ģ������������ */
    upload_pv_num_prev = upload_pv_num;

    /* �����Ҫ�ϴ���ģ���� */
    if (!TYB_SLP_mark_upload_pv(TYBC_PCB_MAX, &upload_pv_num))
    {
        return;
    }

    /* ģ���������仯��� */
    if (upload_pv_num_prev != upload_pv_num)
    {
        SETBIT_N(fg_dpid_upload_pri_high, SYS_STAT_INFO_1);
        SETBIT_N(fg_dpid_upload_pri_high, SYS_STAT_INFO_2);
        SETBIT_N(fg_dpid_upload_pri_high, SYS_STAT_INFO_3);
        SETBIT_N(fg_dpid_upload_pri_high, SYS_STAT_INFO_4);
    }

    for (i = 0; i < TYBC_SYS_PV_MAX; i++)
    {
        /* ��Чģ���� */
        if (GETBIT_N(fg_upload_pv, i))
        {
            /* �ռ����: ��������ۼ��ֽ��� */
            acc_bytes = buf_len + DP_FIELD_SYS_STAT_DATA_BYTES;

            /* д���ϱ����ܵ㻺���� */
            if (acc_bytes <= RAW_DATA_BYTES - 2)
            {
                attr = TYBC_SYS_PV_ATTR(i);
                buf[buf_len++] = HIBYTE(attr);
                buf[buf_len++] = LOBYTE(attr);
                buf[buf_len++] = HIBYTE(TYBC_PV_VAL(TYBC_PCB_MAX, i));
                buf[buf_len++] = LOBYTE(TYBC_PV_VAL(TYBC_PCB_MAX, i));
            }
            else
            {
                break;
            }
        }
    }

    /* ������־ */
    if (buf_len <= RAW_DATA_BYTES - 2)
    {
        buf[buf_len++] = 0xff;
        buf[buf_len++] = 0xff;
    }
}
#endif

/* ��"��������" */
#ifdef TUYA_USE_PARAM_SET
/*******************************************************************************
 �� ��: TYB_SLP_trans_param_grp_disp_rsrc()
 �� ��: �ְ��ϴ�����������ʾ��Դ(ͨ������Э�飬������:0x01)
 �� ��: buf--���ܵ㻺����;
        info--����������Ϣ;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TYB_SLP_trans_param_grp_disp_rsrc(U08 *buf, TYB_TRANS_INFO *info)
{
    U16 i,j;
    U08 useless_cache[TYBC_STR_BYTE_MAX];	/* ���û��� */
    DP_FIELD_DISP_RSRC disp_rsrc;  /* ����������ʾ��Դ */
    U16 upload_bytes = 0;          /* �ϴ��������ֽ��� */
    U08 package_total = 1;         /* �ܰ���(����1��) */
    BOOL fg_multi_pkg = FALSE;     /* ������������־ */
    U16 *src_buf = NULL;
    U08 buf_len = 0;

    static U16 idx_rec = 0;        /* ��¼��һ�����ݵ���ʼ�������� */


    /* ���ݰ��ط���ʼ�������� */
    if (GETBIT_N(info->request_type, RST_GRP_DISP_RSRC))
    {
        idx_rec = 0;
        CLRBIT_N(info->request_type, RST_GRP_DISP_RSRC);
    }

    /* Ѱ�ҵ�ǰ���ݰ�����ʼ��������(��0������) */
    if (GETBIT_N(info->request_type, CONT_GRP_DISP_RSRC))
    {
        if (info->pkg_id == 0)
        {
            idx_rec = 0;
        }
    }

    /* �����ܰ�����������ʼ���� */
    for (i = 0; i < TYBC_PARAM_GRP_MAX; i++)
    {
        U08 msg_len = TYB_SLP_GET_CODE_STR(useless_cache, TYBC_PARAM_GRP_TITLE(i));
        upload_bytes += DP_FIELD_DISP_RSRC_BYTES + msg_len;

        if (upload_bytes > TRANS_01_CONTENT_BYTES)
        {
            package_total++;  /* �ܰ����ۼ� */
            upload_bytes = DP_FIELD_DISP_RSRC_BYTES + msg_len;

            /* Ѱ�ҵ�ǰ���ݰ�����ʼ��������(�ǵ�0������) */
            if (GETBIT_N(info->request_type, CONT_GRP_DISP_RSRC))
            {
                /* �����ݸ������������ʱ����¼��ʱ����ʼ���� */
                if (info->pkg_id == (package_total-1))
                {
                    idx_rec = i;
                }
            }
        }
    }

    /* ���֡ͷ */
    buf[0] = info->trigger_src;          /* ����Դ */
    buf[1] = info->verif_code[0];        /* ��֤�� */
    buf[2] = info->verif_code[1];        /* ��֤�� */
    buf[3] = info->verif_code[2];        /* ��֤�� */
    buf[4] = package_total;              /* �ܰ��� */
    buf[5] = info->pkg_id;               /* ��ǰ���� */
    buf[6] = info->trans_code;           /* ������ */

    /* ����м���Ϣ: ʵ�ʲ��������� */
    buf[7] = TYBC_PARAM_GRP_NUM;

    buf_len += TRANS_01_HEADER_BYTES;

    /* �������: ����������ʾ��Դ(ȫ��) */
    for (i = idx_rec; i < TYBC_PARAM_GRP_MAX; i++)
    {
        disp_rsrc.msg_id = TYBC_PARAM_GRP_TITLE(i);
        disp_rsrc.msg_len = TYB_SLP_GET_CODE_STR(useless_cache, TYBC_PARAM_GRP_TITLE(i));

        upload_bytes = buf_len + DP_FIELD_DISP_RSRC_BYTES + disp_rsrc.msg_len;

        if (upload_bytes <= RAW_DATA_BYTES)
        {
            src_buf = (U16*)&disp_rsrc;
            for (j = 0; j < DP_FIELD_DISP_RSRC_SIZE; j++)
            {
                buf[buf_len++] = HIBYTE(src_buf[j]);
                buf[buf_len++] = LOBYTE(src_buf[j]);
            }
            buf_len += TYB_SLP_GET_CODE_STR(&buf[buf_len], disp_rsrc.msg_id);
        }
        else
        {
            fg_multi_pkg = TRUE;
            idx_rec = i;
            break;
        }
    }

    /* ������һ�� */
    info->pkg_id++;

    /* ���в���������ʾ��Դ�ϴ���� */
    if (!fg_multi_pkg)
    {
        idx_rec = 0;                                           /* ��¼��� */
        info->pkg_id = 0;                                      /* ��ǰ��ID��λ */
        if (GETBIT_N(info->request_type, RST_GRP_DISP_ALL))    /* ��������ʱ�����ŷ���һ�������� */
        {
            info->trans_code = TRANS_PARAM_GRP_DISP_CFG;       /* ׼������һ�������� */
            CLRBIT_N(info->request_type, RST_GRP_DISP_ALL);
        }
        else    /* �ط����һ�����ݻ�����������ʾ��Դ���ݣ��������� */
        {
            info->trans_code = TRANS_NULL;
            fg_has_trans_task = FALSE;
        }
    }

    /* �ش����ݰ�ʱ�������������������룬����ͨ������ */
    if (GETBIT_N(info->request_type, CONT_GRP_DISP_RSRC))
    {
        fg_has_trans_task = FALSE;
        CLRBIT_N(info->request_type, CONT_GRP_DISP_RSRC);
    }
}


/*******************************************************************************
 �� ��: TYB_SLP_trans_param_grp_disp_cfg()
 �� ��: �ְ��ϴ���������������Ϣ(ͨ������Э�飬������:0x02)
 �� ��: buf--���ܵ㻺����;
        info--����������Ϣ;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TYB_SLP_trans_param_grp_disp_cfg(U08 *buf, TYB_TRANS_INFO *info)
{
    U16 i,j;
    DP_FIELD_PARAM_GRP_DISP_CFG disp_cfg;    /* ����������ʾ��Դ */
    U16 upload_bytes = 0;                    /* �ϴ��������ֽ��� */
    U08 package_total = 1;                   /* �ܰ���(����1��) */
    BOOL fg_multi_pkg = FALSE;               /* ������������־ */
    U16 *src_buf = NULL;
    U08 buf_len = 0;

    static U16 idx_rec = 0;                  /* ��¼��һ�����ݵ���ʼ�������� */


    /* ���ݰ��ط���ʼ�������� */
    if (GETBIT_N(info->request_type, RST_GRP_DISP_CFG))
    {
        idx_rec = 0;
        CLRBIT_N(info->request_type, RST_GRP_DISP_CFG);
    }

    /* Ѱ�ҵ�ǰ���ݰ�����ʼ��������(��0������) */
    if (GETBIT_N(info->request_type, CONT_GRP_DISP_CFG))
    {
        if (info->pkg_id == 0)
        {
            idx_rec = 0;
        }
    }

    /* �����ܰ�����������ʼ���� */
    for (i = 0; i < TYBC_PARAM_GRP_NUM; i++)
    {
        upload_bytes += DP_FIELD_PARAM_GRP_DISP_CFG_BYTES;

        if (upload_bytes > TRANS_02_CONTENT_BYTES)
        {
           package_total++;    /* �ܰ����ۼ� */
           upload_bytes = DP_FIELD_PARAM_GRP_DISP_CFG_BYTES;

           /* Ѱ�ҵ�ǰ���ݰ�����ʼ��������(�ǵ�0������) */
           if (GETBIT_N(info->request_type, CONT_GRP_DISP_CFG))
           {
               if (info->pkg_id == (package_total-1))
               {
                   idx_rec = i;
               }
           }
        }
    }

    /* ����ϱ��ֽ������� */
    upload_bytes = 0;

    /* ���֡ͷ */
    buf[0] = info->trigger_src;      /* ����Դ */
    buf[1] = info->verif_code[0];    /* ��֤�� */
    buf[2] = info->verif_code[1];    /* ��֤�� */
    buf[3] = info->verif_code[2];    /* ��֤�� */
    buf[4] = package_total;          /* �ܰ��� */
    buf[5] = info->pkg_id;           /* ��ǰ���� */
    buf[6] = info->trans_code;       /* ������ */

    /* ����м���Ϣ: ʵ�ʲ��������� */
    buf[7] = TYBC_PARAM_GRP_NUM;

    buf_len += TRANS_02_HEADER_BYTES;

    /* �������: ��������������Ϣ  */
    for (i = idx_rec; i < TYBC_PARAM_GRP_NUM; i++)
    {
        disp_cfg.msg_id = TYBC_PARAM_GRP_TITLE(i);
        disp_cfg.attr = TYBC_PARAM_GRP_ATTR(i);

        upload_bytes = buf_len + DP_FIELD_PARAM_GRP_DISP_CFG_BYTES;

        if (upload_bytes <= RAW_DATA_BYTES)
        {
            src_buf = (U16*)&disp_cfg;
            for (j = 0; j < DP_FIELD_PARAM_GRP_DISP_CFG_SIZE; j++)
            {
                buf[buf_len++] = HIBYTE(src_buf[j]);
                buf[buf_len++] = LOBYTE(src_buf[j]);
            }
        }
        else
        {
            fg_multi_pkg = TRUE;
            idx_rec = i;
            break;
        }
    }

    /* ������һ�� */
    info->pkg_id++;

    /* ���в�������������Ϣ�ϴ���� */
    if (!fg_multi_pkg)
    {
        idx_rec = 0;                         /* ��¼��� */
        info->pkg_id = 0;                    /* ��ǰ��ID��λ */
        info->trans_code = TRANS_NULL;
        fg_has_trans_task = FALSE;
    }

    /* �ش����ݰ�ʱ�������������������룬����ͨ������ */
    if (GETBIT_N(info->request_type, CONT_GRP_DISP_CFG))
    {
        fg_has_trans_task = FALSE;
        CLRBIT_N(info->request_type, CONT_GRP_DISP_CFG);
    }
}


/*******************************************************************************
 �� ��: TYB_SLP_trans_param_range_disp_rsrc()
 �� ��: �ְ��ϴ�����ѡ������ʾ��Դ (ͨ������Э�飬������:0x03)
 �� ��: buf--���ܵ㻺����;
        info--����������Ϣ;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TYB_SLP_trans_param_range_disp_rsrc(U08 *buf, TYB_TRANS_INFO *info)
{
    U16 i,j;
    U08 useless_cache[TYBC_STR_BYTE_MAX];          /* ���û��� */
    U16 item_total = 0;             /* ѡ���ַ���ʵ�ʸ��� */
    U16 upload_bytes = 0;           /* �ϴ��������ֽ��� */
    U08 package_total = 1;          /* �ܰ���(����1��) */
    BOOL fg_multi_pkg = FALSE;      /* ������������־ */
    BOOL fg_enum_split = FALSE;     /* һ��ѡ���ַ����ְ������־ */
    U08 buf_len = 0;

    static U16 dict_idx_rec = 0;
    static U16 item_idx_rec = 0;    /* ��¼��һ�����ݵĲ�������ʼ���� */


    /* ���ݰ��ط���ʼ�������� */
    if (GETBIT_N(info->request_type, RST_RANGE_DISP_RSRC))
    {
        dict_idx_rec = 0;
        item_idx_rec = 0;
        CLRBIT_N(info->request_type, RST_RANGE_DISP_RSRC);
    }

    /* Ѱ�ҵ�ǰ���ݰ�����ʼ��������(��0������) */
    if (GETBIT_N(info->request_type, CONT_RANGE_DISP_RSRC))
    {
        if (info->pkg_id == 0)
        {
            dict_idx_rec = 0;
            item_idx_rec = 0;
        }
    }


    /* �����ܰ�����������ʼ���� */
    for (i = 0; i < TYBC_PARAM_DICT_NUM; i++)
    {
        for (j = 0; j < TYBC_PARAM_DICT_ITEM_NUM(i); j++)
        {
            U08 item_msg_len = TYB_SLP_GET_CODE_STR(useless_cache, TYBC_PARAM_DICT_ITEM(i, j));

            upload_bytes += DP_FIELD_DISP_RSRC_BYTES + item_msg_len;

            if (upload_bytes > TRANS_03_CONTENT_BYTES)
            {
                package_total++;    /* �ܰ����ۼ� */
                upload_bytes = DP_FIELD_DISP_RSRC_BYTES + item_msg_len;

                /* Ѱ�ҵ�ǰ���ݰ�����ʼ��������(�ǵ�0������) */
                if (GETBIT_N(info->request_type, CONT_RANGE_DISP_RSRC))
                {
                    /* �����ݸ������������ʱ����¼��ʱ����ʼ���� */
                    if (info->pkg_id == (package_total-1))
                    {
                        dict_idx_rec = i;
                        item_idx_rec = j;
                    }
                }
            }
            item_total++;
        }
    }

    /* ���֡ͷ */
    buf[0] = info->trigger_src;      /* ����Դ */
    buf[1] = info->verif_code[0];    /* ��֤�� */
    buf[2] = info->verif_code[1];    /* ��֤�� */
    buf[3] = info->verif_code[2];    /* ��֤�� */
    buf[4] = package_total;          /* �ܰ��� */
    buf[5] = info->pkg_id;           /* ��ǰ���� */
    buf[6] = info->trans_code;       /* ������ */

    /* ����м���Ϣ: ѡ���ַ������� */
    buf[7] = HIBYTE(item_total);     /* ѡ���ַ���ʵ�ʸ��� */
    buf[8] = LOBYTE(item_total);     /* ѡ���ַ���ʵ�ʸ��� */

    buf_len += TRANS_03_HEADER_BYTES;

    /* ���䵱ǰ���ö���Ͳ�����ȡֵ��Χ�ַ��� */
    for (i = dict_idx_rec; i < TYBC_PARAM_DICT_NUM; i++)
    {
        for (j = item_idx_rec; j < TYBC_PARAM_DICT_ITEM_NUM(i); j++)
        {
            U16 msg_id = TYBC_PARAM_DICT_ITEM(i, j);
            U08 msg_len = TYB_SLP_GET_CODE_STR(useless_cache, msg_id);
            U08 range_dict_id = i;

            upload_bytes = buf_len + DP_FIELD_DISP_RSRC_BYTES + msg_len;

            if (upload_bytes <= RAW_DATA_BYTES)  // ��Ϊbuf_len��������֡ͷ�ֽڣ��˴�Ҫ��Ӧ����֡ͷ�ֽ�
            {
                buf[buf_len++] = HIBYTE(msg_id);
                buf[buf_len++] = LOBYTE(msg_id);
                buf[buf_len++] = msg_len;
                buf[buf_len++] = range_dict_id;
                buf_len += TYB_SLP_GET_CODE_STR(&buf[buf_len], msg_id);
                item_idx_rec = 0;
            }
            else
            {
                fg_enum_split = TRUE;
                item_idx_rec = j;
                break;
            }
        }

        if (fg_enum_split)
        {
            fg_multi_pkg = TRUE;
            dict_idx_rec = i;
            break;
        }
    }


    /* ������һ�� */
    info->pkg_id++;


    /* �������ַ���&��ǰֵ�ַ���ȫ�������� */
    if (!fg_multi_pkg)
    {
        dict_idx_rec = 0;
        item_idx_rec = 0;                         /* ���ö����ʼ���� */
        info->pkg_id = 0;                         /* ��ǰ���Ÿ�λ */
        info->trans_code = TRANS_NULL;
        fg_has_trans_task = FALSE;
    }

    /* �ش����ݰ�ʱ�������������������룬����ͨ������ */
    if (GETBIT_N(info->request_type, CONT_RANGE_DISP_RSRC))
    {
        fg_has_trans_task = FALSE;
        CLRBIT_N(info->request_type, CONT_RANGE_DISP_RSRC);
    }
}


/*******************************************************************************
 �� ��: TYB_SLP_trans_param_item_disp_rsrc()
 �� ��: �ְ��ϴ���ǰ���������ʾ��Դ (ͨ������Э�飬������:0x04)
 �� ��: buf--���ܵ㻺����;
        info--����������Ϣ;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TYB_SLP_trans_param_item_disp_rsrc(U08 *buf, TYB_TRANS_INFO *info)
{
    U16 i,j;
    U08 useless_cache[TYBC_STR_BYTE_MAX];               /* ���û��� */
    DP_FIELD_DISP_RSRC disp_rsrc;        /* ��ǰ���������ʾ��Դ */
    U16 upload_bytes = 0;                /* �ϴ��������ֽ��� */
    U08 package_total = 1;               /* �ܰ���(����1��) */
    BOOL fg_multi_pkg = FALSE;           /* ������������־ */
    U16 *src_buf = NULL;
    U08 buf_len = 0;
    U08 param_grp_id = info->addr[0];

    static U16 idx_rec = 0;              /* ��¼��һ�����ݵĲ�������ʼ���� */


    /* ���ݰ��ط���ʼ�������� */
    if (GETBIT_N(info->request_type, RST_ITEM_DISP_RSRC))
    {
        idx_rec = 0;
        CLRBIT_N(info->request_type, RST_ITEM_DISP_RSRC);
    }

    /* Ѱ�ҵ�ǰ���ݰ�����ʼ��������(��0������) */
    if (GETBIT_N(info->request_type, CONT_ITEM_DISP_RSRC))
    {
        if (info->pkg_id == 0)
        {
            idx_rec = 0;
        }
    }

    /* �����ܰ�����������ʼ���� */
    for (i = 0; i < TYBC_PARAM_GRP_SIZE; i++)
    {
        if (i < TYBC_PARAM_ITEM_NUM(param_grp_id))
        {
            U08 msg_len = TYB_SLP_GET_CODE_STR(useless_cache, TYBC_PARAM_ITEM_TITLE(param_grp_id,i));

            /* �������ֽ��� */
            upload_bytes += DP_FIELD_DISP_RSRC_BYTES + msg_len;

            if (upload_bytes > TRANS_04_CONTENT_BYTES)
            {
                package_total++;    /* �ܰ����ۼ� */
                upload_bytes = DP_FIELD_DISP_RSRC_BYTES + msg_len;

                /* Ѱ�ҵ�ǰ���ݰ�����ʼ��������(�ǵ�0������) */
                if (GETBIT_N(info->request_type, CONT_ITEM_DISP_RSRC))
                {
                    if (info->pkg_id == (package_total-1))
                    {
                        idx_rec = i;
                    }
                }
            }
        }
    }

    /* ����ϱ��ֽ������� */
    upload_bytes = 0;

    /* ���֡ͷ */
    buf[0] = info->trigger_src;          /* ����Դ */
    buf[1] = info->verif_code[0];        /* ��֤�� */
    buf[2] = info->verif_code[1];        /* ��֤�� */
    buf[3] = info->verif_code[2];        /* ��֤�� */
    buf[4] = package_total;              /* �ܰ��� */
    buf[5] = info->pkg_id;               /* ��ǰ���� */
    buf[6] = info->trans_code;           /* ������ */

    /* ����м���Ϣ: ��ǰ������ID */
    buf[7] = param_grp_id;                        /* ��ǰ������ID */
    buf[8] = TYBC_PARAM_ITEM_NUM(param_grp_id);    /* ��ǰ�����ʵ�ʸ��� */

    buf_len += TRANS_04_HEADER_BYTES;

    /* �������: ��ǰ������� */
    for (i = idx_rec; i < TYBC_PARAM_GRP_SIZE; i++)
    {
        if (i < TYBC_PARAM_ITEM_NUM(param_grp_id))
        {
            disp_rsrc.msg_id = TYBC_PARAM_ITEM_TITLE(param_grp_id,i);
            disp_rsrc.msg_len = TYB_SLP_GET_CODE_STR(useless_cache, TYBC_PARAM_ITEM_TITLE(param_grp_id,i));

            /* �ռ����: ��������ۼ��ֽ��� */
            upload_bytes = buf_len + DP_FIELD_DISP_RSRC_BYTES + disp_rsrc.msg_len;

            if (upload_bytes <= RAW_DATA_BYTES)
            {
                /* д���ϱ����ܵ㻺���� */
                src_buf = (U16*)&disp_rsrc;
                for (j = 0; j < DP_FIELD_DISP_RSRC_SIZE; j++)
                {
                    buf[buf_len++] = HIBYTE(src_buf[j]);
                    buf[buf_len++] = LOBYTE(src_buf[j]);
                }
                buf_len += TYB_SLP_GET_CODE_STR(&buf[buf_len], disp_rsrc.msg_id);
                idx_rec = 0;
            }
            else
            {
                fg_multi_pkg = TRUE;
                idx_rec = i;
                break;
            }
        }
    }


    /* ������һ�� */
    info->pkg_id++;


    /* �������ַ���&��ǰֵ�ַ���ȫ�������� */
    if (!fg_multi_pkg)
    {
        idx_rec = 0;                         /* ���ö����ʼ���� */
        info->pkg_id = 0;                         /* ��ǰ���Ÿ�λ */
        if (GETBIT_N(info->request_type, RST_ITEM_DISP_ALL))    /* ��������ʱ�����ŷ���һ�������� */
        {
            info->trans_code = TRANS_PARAM_ITEM_DISP_CFG;             /* ׼������һ�������� */
            CLRBIT_N(info->request_type, RST_ITEM_DISP_ALL);
        }
        else    /* �ط����һ�����ݻ�����������ʾ��Դ���ݣ��������� */
        {
            info->trans_code = TRANS_NULL;
            fg_has_trans_task = FALSE;
        }
    }

    /* �ش����ݰ�ʱ�������������������룬����ͨ������ */
    if (GETBIT_N(info->request_type, CONT_ITEM_DISP_RSRC))
    {
        fg_has_trans_task = FALSE;
        CLRBIT_N(info->request_type, CONT_ITEM_DISP_RSRC);
    }
}


/*******************************************************************************
 �� ��: TYB_SLP_trans_param_item_disp_cfg()
 �� ��: �ְ��ϴ���ǰ�������(+��ǰֵ�ַ���)������Ϣ (ͨ������Э�飬������:0x05)
 �� ��: buf--���ܵ㻺����;
        info--����������Ϣ;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TYB_SLP_trans_param_item_disp_cfg(U08 *buf, TYB_TRANS_INFO *info)
{
    U16 i,j;
    DP_FIELD_PARAM_ITEM_DISP_CFG disp_cfg;    /* ��ǰ�����������Ϣ */
    U16 upload_bytes = 0;                     /* �ϴ��������ֽ��� */
    U08 package_total = 1;                    /* �ܰ���(����1��) */
    BOOL fg_multi_pkg = FALSE;                /* ������������־ */
    U16 *src_buf = NULL;
    U08 buf_len = 0;
    U08 param_grp_id = info->addr[0];

    static U16 idx_rec = 0;             /* ��¼��һ�����ݵ���ʼ�������� */


    /* ���ݰ��ط���ʼ�������� */
    if (GETBIT_N(info->request_type, RST_ITEM_DISP_CFG))
    {
        idx_rec = 0;
        CLRBIT_N(info->request_type, RST_ITEM_DISP_CFG);
    }

    /* Ѱ�ҵ�ǰ���ݰ�����ʼ��������(��0������) */
    if (GETBIT_N(info->request_type, CONT_ITEM_DISP_CFG))
    {
        if (info->pkg_id == 0)
        {
            idx_rec = 0;
        }
    }


    /* �����ܰ�����������ʼ���� */
    for (i = 0; i < TYBC_PARAM_GRP_SIZE; i++)
    {
        if (i < TYBC_PARAM_ITEM_NUM(param_grp_id))
        {
            upload_bytes += DP_FIELD_PARAM_ITEM_DISP_CFG_BYTES;

            if (upload_bytes > TRANS_05_CONTENT_BYTES)
            {
                package_total++;  // �ܰ����ۼ�
                upload_bytes = DP_FIELD_PARAM_ITEM_DISP_CFG_BYTES;

                /* Ѱ�ҵ�ǰ���ݰ�����ʼ��������(�ǵ�0������) */
                if (GETBIT_N(info->request_type,CONT_ITEM_DISP_CFG))
                {
                    if (info->pkg_id == (package_total-1))
                    {
                        idx_rec = i;
                    }
                }
            }
        }
    }

    /* ������书�ܵ� */
    // ֡ͷ��Ϣ
    buf[0] = info->trigger_src;
    buf[1] = info->verif_code[0];        /* ��֤�� */
    buf[2] = info->verif_code[1];        /* ��֤�� */
    buf[3] = info->verif_code[2];        /* ��֤�� */
    buf[4] = package_total;              /* �ܰ��� */
    buf[5] = info->pkg_id;               /* ��ǰ���� */
    buf[6] = info->trans_code;           /* ������ */

    /* ����м���Ϣ */
    buf[7] = param_grp_id;                        /* ��ǰ������� */
    buf[8] = TYBC_PARAM_ITEM_NUM(param_grp_id);    /* ��ǰ�����ʵ�ʸ��� */

    buf_len += TRANS_05_HEADER_BYTES;

    /* �������� */
    for (i = idx_rec; i < TYBC_PARAM_GRP_SIZE; i++)
    {
        if (i < TYBC_PARAM_ITEM_NUM(param_grp_id))
        {

            disp_cfg.msg_id = TYBC_PARAM_ITEM_TITLE(param_grp_id,i);    /* ������ID */
            disp_cfg.attr = TYBC_PARAM_ITEM_ATTR(param_grp_id,(U08)i);  /* �������� */
            disp_cfg.value = TYBC_PARAM_ITEM_VAL(param_grp_id,i);       /* ������ǰֵ */
            disp_cfg.max = TYBC_PARAM_MAX(param_grp_id,i);              /* �������ֵ */
            disp_cfg.min = TYBC_PARAM_MIN(param_grp_id,i);              /* ������Сֵ */

            upload_bytes = buf_len + DP_FIELD_PARAM_ITEM_DISP_CFG_BYTES;

            if (upload_bytes <= RAW_DATA_BYTES)
            {
                src_buf = (U16*)&disp_cfg;
                for (j = 0; j < DP_FIELD_PARAM_ITEM_DISP_CFG_SIZE; j++)
                {
                    buf[buf_len++] = HIBYTE(src_buf[j]);
                    buf[buf_len++] = LOBYTE(src_buf[j]);
                }
            }
            else
            {
                fg_multi_pkg = TRUE;
                idx_rec = i;
                break;
            }
        }
    }

    /* ������һ�� */
    info->pkg_id++;

    /* �������ַ���&��ǰֵ�ַ���ȫ�������� */
    if (!fg_multi_pkg)
    {
        idx_rec = 0;                          /* ��¼��� */
        info->pkg_id = 0;                     /* ��ǰ��ID��λ */
        info->trans_code = TRANS_NULL;
        fg_has_trans_task = FALSE;
    }

    /* �ش����ݰ�ʱ�������������������룬����ͨ������ */
    if (GETBIT_N(info->request_type, CONT_ITEM_DISP_CFG))
    {
        fg_has_trans_task = FALSE;
        CLRBIT_N(info->request_type, CONT_ITEM_DISP_CFG);
    }
}


/*******************************************************************************
 �� ��: TYB_SLP_parse_param_cmd()
 �� ��: ����������������
 �� ��: buf--���ջ�����(����Դ);
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TYB_SLP_parse_param_cmd(U08 *buf)
{
    TYB_TRANS_INFO info;
    U08 buf_len = 0;

    /* ��ʼ�� */
    ARRCLR(info.addr);
    ARRCLR(info.request_type);

    /* ����Դ��Ϊ"��������"���򲻼����������� */
    if (buf[buf_len] != TRANS_PARAM)
    {
        return;
    }

    /* ��1����:����Դ("״̬��ѯ"����) */
    info.trigger_src = buf[buf_len++];

    /* ��2����:��֤�� */
    MEMCPY(info.verif_code, &buf[buf_len], TRANS_VERIF_CODE_LEN);
    buf_len += TRANS_VERIF_CODE_LEN;

    /* ��3����: ������ */
    info.parse_code = buf[buf_len++];

    /* ��4����: ������ */
    info.request_code = buf[buf_len++];

    /* ��5����: ������� */
    info.pkg_id = buf[buf_len++];   // ��ָ�����ݰ���ʼ���ܰ����ڲ�����

    /* ��6����: ������� */
    info.addr[0] = buf[buf_len++];

    /* ��7����: ������� */
    info.addr[1] = buf[buf_len++];


    switch (info.parse_code)
    {
        /* ���ز��������б� */
        case TRANS_CMD_PARAM_GRP_DISP:
            switch (info.request_code)
            {
                /* ��ͷ��ʼ��ȫ������ */
                case UPLOAD_DISP_ALL:
                    info.trans_code = TRANS_PARAM_GRP_DISP_RSRC;
                    SETBIT_N(info.request_type, RST_GRP_DISP_ALL);
                    SETBIT_N(info.request_type, RST_GRP_DISP_RSRC);
                    SETBIT_N(info.request_type, RST_GRP_DISP_CFG);
                    break;

                /* ������ʾ��Դ�е�ĳ������ */
                case UPLOAD_ONE_DISP_RSRC:
                    info.trans_code = TRANS_PARAM_GRP_DISP_RSRC;
                    SETBIT_N(info.request_type, CONT_GRP_DISP_RSRC);
                    break;

                /* ����������Ϣ�е�ĳ������ */
                case UPLOAD_ONE_DISP_CFG:
                    info.trans_code = TRANS_PARAM_GRP_DISP_CFG;
                    SETBIT_N(info.request_type, CONT_GRP_DISP_CFG);
                    break;

                /* ������ʾ��Դ���������ݰ�(�������б�) */
                case UPLOAD_ALL_DISP_RSRC:
                    info.trans_code = TRANS_PARAM_GRP_DISP_RSRC;
                    SETBIT_N(info.request_type, RST_GRP_DISP_RSRC);
                    break;

                /* ����������Ϣ���������ݰ�(�������б�) */
                case UPLOAD_ALL_DISP_CFG:
                    info.trans_code = TRANS_PARAM_GRP_DISP_CFG;
                    SETBIT_N(info.request_type, RST_GRP_DISP_CFG);
                    break;
            }

            /* ������� */
            if (TYB_SLP_is_node_unique(&info))
            {
                TYB_SLP_cache_append(&info);
            }
            break;


        /* ���ز���ѡ�� */
        case TRANS_CMD_PARAM_RANGE_DISP:
            switch (info.request_code)
            {
                /* ��ͷ��ʼ��ȫ������ */
                case UPLOAD_DISP_ALL:
                    info.trans_code = TRANS_PARAM_RANGE_DISP_RSRC;
                    SETBIT_N(info.request_type, RST_RANGE_DISP_RSRC);
                    break;

                /* ������ʾ��Դ�е�ĳ������ */
                case UPLOAD_ONE_DISP_RSRC:
                    info.trans_code = TRANS_PARAM_RANGE_DISP_RSRC;
                    SETBIT_N(info.request_type, CONT_RANGE_DISP_RSRC);
                    break;
            }

            /* ������� */
            if (TYB_SLP_is_node_unique(&info))
            {
                TYB_SLP_cache_append(&info);
            }

            break;

        /* ѡ������飬���ز������б� */
        case TRANS_CMD_PARAM_ITEM_DISP:
            switch (info.request_code)
            {
                /* ��ͷ��ʼ��ȫ������ */
                case UPLOAD_DISP_ALL:
                    info.trans_code = TRANS_PARAM_ITEM_DISP_RSRC;
                    SETBIT_N(info.request_type, RST_ITEM_DISP_ALL);
                    SETBIT_N(info.request_type, RST_ITEM_DISP_RSRC);
                    SETBIT_N(info.request_type, RST_ITEM_DISP_CFG);
                    break;

                /* ������ʾ��Դ�е�ĳ������ */
                case UPLOAD_ONE_DISP_RSRC:
                    info.trans_code = TRANS_PARAM_ITEM_DISP_RSRC;
                    SETBIT_N(info.request_type, CONT_ITEM_DISP_RSRC);
                    break;

                /* ����������Ϣ�е�ĳ������ */
                case UPLOAD_ONE_DISP_CFG:
                    info.trans_code = TRANS_PARAM_ITEM_DISP_CFG;
                    SETBIT_N(info.request_type, CONT_ITEM_DISP_CFG);
                    break;

                /* ������ʾ��Դ���������ݰ�(��ǰ������б�) */
                case UPLOAD_ALL_DISP_RSRC:
                    info.trans_code = TRANS_PARAM_ITEM_DISP_RSRC;
                    SETBIT_N(info.request_type, RST_ITEM_DISP_RSRC);
                    break;

                /* ����������Ϣ���������ݰ�(��ǰ������б�) */
                case UPLOAD_ALL_DISP_CFG:
                    info.trans_code = TRANS_PARAM_ITEM_DISP_CFG;
                    SETBIT_N(info.request_type, RST_ITEM_DISP_CFG);
                    break;
            }

            /* ������� */
            if (TYB_SLP_is_node_unique(&info))
            {
                TYB_SLP_cache_append(&info);
            }

            break;

        /* �趨����(ע��: ���������) */
        case TRANS_CMD_PARAM_VAL_SET:
            TYBC_SET_PARAM(info.addr[0],info.addr[1],MAKEWORD(buf[buf_len],buf[buf_len+1]));
            break;
    }
}

#endif

#ifdef TUYA_USE_SYS_STAT

/*******************************************************************************
 �� ��: TYB_SLP_trans_sys_msg_disp_rsrc()
 �� ��: �ְ��ϴ�ϵͳ״̬�ַ�����ʾ��Դ(ͨ������Э�飬������:)
 �� ��: buf--���ܵ㻺����;
        info--����������Ϣ;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TYB_SLP_trans_sys_msg_disp_rsrc(U08 *buf, TYB_TRANS_INFO *info)
{
    U16 i, j;
    U08 useless_cache[TYBC_STR_BYTE_MAX];	/* ���û��� */
    U16 upload_bytes = 0;          /* �ϴ��������ֽ��� */
    U08 package_total = 1;         /* �ܰ���(����1��) */
    BOOL fg_multi_pkg = FALSE;     /* ������������־ */
    U08 buf_len = 0;

    static U16 idx_rec = 0;        /* ��¼��һ�����ݵ���ʼ�������� */


    /* ���ݰ��ط���ʼ�������� */
    if (GETBIT_N(info->request_type, RST_SYS_MSG_DISP_RSRC))
    {
        idx_rec = 0;
        CLRBIT_N(info->request_type, RST_SYS_MSG_DISP_RSRC);
    }

    /* Ѱ�ҵ�ǰ���ݰ�����ʼ��������(��0������) */
    if (GETBIT_N(info->request_type, CONT_SYS_MSG_DISP_RSRC))
    {
        if (info->pkg_id == 0)
        {
            idx_rec = 0;
        }
    }

    /* �����ܰ�����������ʼ���� */
    for (i = 0; i < TYBC_SYS_PV_IN_USED; i++)
    {
        U08 msg_len = 0;
		U16 str_idx;
		U16 str_idx_before;
		BOOL fg_fill_str;
        /* ֵΪ�ַ�����ģ���� */
        if (TYBC_SYS_PV_ATTR(i) == 0xFFFE)
        {
        	str_idx = (U16)(TYBC_PV_VAL(TYBC_PCB_MAX, i));
	        msg_len = TYB_SLP_GET_CODE_STR(useless_cache, str_idx);
			fg_fill_str = TRUE;
			//���ظ��ַ����������
			for (j=0; j<i; j++)
			{
				str_idx_before = (U16)(TYBC_PV_VAL(TYBC_PCB_MAX, j));
				if (str_idx == str_idx_before)
				{
					fg_fill_str = FALSE;
					break;
				}
			}
			//ʼ������һ��
			if (i == 0)
			{
				fg_fill_str= TRUE;
			}

			if (fg_fill_str)
			{
			    upload_bytes += DP_FIELD_SYS_MSG_DATA_BYTES + msg_len;

			    if (upload_bytes > TRANS_13_CONTENT_BYTES)
			    {
			        package_total++;    /* �ܰ����ۼ� */
			        upload_bytes = DP_FIELD_SYS_MSG_DATA_BYTES + msg_len;

			        /* Ѱ�ҵ�ǰ���ݰ�����ʼ��������(�ǵ�0������) */
			        if (GETBIT_N(info->request_type, CONT_SYS_MSG_DISP_RSRC))
			        {
			            /* �����ݸ������������ʱ����¼��ʱ����ʼ���� */
			            if (info->pkg_id == (package_total-1))
			            {
			                idx_rec = i;
			            }
			        }
			    }
			}


		}
    }

    /* ���֡ͷ */
    buf[0] = info->trigger_src;          /* ����Դ */
    buf[1] = info->verif_code[0];        /* ��֤�� */
    buf[2] = info->verif_code[1];        /* ��֤�� */
    buf[3] = info->verif_code[2];        /* ��֤�� */
    buf[4] = package_total;              /* �ܰ��� */
    buf[5] = info->pkg_id;               /* ��ǰ���� */
    buf[6] = info->trans_code;           /* ������ */

    /* ����м���Ϣ */
    buf[7] = 0;               /* ��ǰģ��� */
    buf[8] = 0;              /* �ϱ�״̬�� */

    buf_len += TRANS_13_HEADER_BYTES;

    /* �������: ��ǰģ��״̬����ʾ��Դ */
    for (i = idx_rec; i < TYBC_SYS_PV_IN_USED; i++)
    {
		U08 msg_len = 0;
		U16 msg_id = 0;
		U16 str_idx;
		U16 str_idx_before;
		BOOL fg_fill_str;
        /* ֵΪ�ַ�����ģ���� */
        if (TYBC_SYS_PV_ATTR(i) == 0xFFFE)
        {
        	str_idx = (U16)(TYBC_PV_VAL(TYBC_PCB_MAX, i));
			msg_len = TYB_SLP_GET_CODE_STR(useless_cache, str_idx);
			msg_id = TYBC_PV_VAL(TYBC_PCB_MAX, i);
			fg_fill_str = TRUE;
			//���ظ��ַ����������
			for (j=0; j<i ;j++)
			{
				str_idx_before = (U16)(TYBC_PV_VAL(TYBC_PCB_MAX, j));
				if (str_idx == str_idx_before)
				{
					fg_fill_str = FALSE;
					break;
				}
			}
			//ʼ������һ��
			if (i == 0)
			{
				fg_fill_str= TRUE;
			}

			if (fg_fill_str)
			{
		        /* �ռ����: ��������ۼ��ֽ��� */
		        upload_bytes = buf_len + DP_FIELD_SYS_MSG_DATA_BYTES + msg_len;
				//test_msg_len = msg_len;
		        if (upload_bytes <= RAW_DATA_BYTES - 2)
		        {
		            buf[buf_len++] = HIBYTE(msg_id);
		            buf[buf_len++] = LOBYTE(msg_id);
		            buf[buf_len++] = HIBYTE(msg_len);
		            buf[buf_len++] = LOBYTE(msg_len);
					//test_msg_len = msg_len;
		            buf_len += TYB_SLP_GET_CODE_STR(&buf[buf_len], str_idx);
		        }
		        else
		        {
		            fg_multi_pkg = TRUE;
		            idx_rec = i;
		            break;
		        }
			}
        }
    }

    /* ������һ�� */
    info->pkg_id++;

    /* ���в���������ʾ��Դ�ϴ���� */
    if (!fg_multi_pkg)
    {
        idx_rec = 0;                                           /* ��¼��� */
        info->pkg_id = 0;                                      /* ��ǰ��ID��λ */
        if (GETBIT_N(info->request_type, RST_SYS_STAT_DISP_ALL))    /* ��������ʱ�����ŷ���һ�������� */
        {
            info->trans_code = TRANS_SYS_STAT_DISP_RSRC;       /* ׼������һ�������� */
            //CLRBIT_N(info->request_type, RST_SYS_STAT_DISP_ALL);
        }
        else    /* �ط����һ�����ݻ�����������ʾ��Դ���ݣ��������� */
        {
            info->trans_code = TRANS_NULL;
            fg_has_trans_task = FALSE;
        }
    }

    /* �ش����ݰ�ʱ�������������������룬����ͨ������ */
    if (GETBIT_N(info->request_type, CONT_SYS_MSG_DISP_RSRC))
    {
        fg_has_trans_task = FALSE;
        CLRBIT_N(info->request_type, CONT_SYS_MSG_DISP_RSRC);
    }
}

/*******************************************************************************
 �� ��: TYB_SLP_trans_sys_stat_disp_rsrc()
 �� ��: �ְ��ϴ���ǰģ��״̬��ʾ��Դ(ͨ������Э�飬������:0x06)
 �� ��: buf--���ܵ㻺����;
        info--����������Ϣ;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TYB_SLP_trans_sys_stat_disp_rsrc(U08 *buf, TYB_TRANS_INFO *info)
{
    U16 i;
    U08 useless_cache[TYBC_STR_BYTE_MAX];        /* ���û��� */
    U16 upload_bytes = 0;         /* �ϴ��������ֽ��� */
    U08 package_total = 1;        /* �ܰ���(����1��) */
    U08 upload_pv_num = 0;        /* ģ�����ϱ����� */
    BOOL fg_multi_pkg = FALSE;    /* ������������־ */
    U08 buf_len = 0;
	U16 attr;

    static U16 idx_rec = 0;       /* ��¼��һ�����ݵ���ʼ�������� */


    /* ���ݰ��ط���ʼ�������� */
    if (GETBIT_N(info->request_type, RST_SYS_STAT_DISP_RSRC))
    {
        idx_rec = 0;
        CLRBIT_N(info->request_type, RST_SYS_STAT_DISP_RSRC);
    }

    /* Ѱ�ҵ�ǰ���ݰ�����ʼ��������(��0������) */
    if (GETBIT_N(info->request_type, CONT_SYS_STAT_DISP_RSRC))
    {
        if (info->pkg_id == 0)
        {
            idx_rec = 0;
        }
    }


    /* �����Ҫ�ϴ���ģ���� */
    if (!TYB_SLP_mark_upload_pv(TYBC_PCB_MAX, &upload_pv_num))
    {
        return;
    }

    /* �����ܰ�����������ʼ���� */
    for (i = 0; i < TYBC_SYS_PV_MAX; i++)
    {
        U08 msg_len = 0;
        /* ��Чģ���� */
        if (GETBIT_N(fg_upload_pv, i))
        {
	        msg_len = TYB_SLP_GET_CODE_STR(useless_cache, TYBC_SYS_PV_TITLE(i));

	        upload_bytes += DP_FIELD_SYS_STAT_INFO_BYTES+ msg_len;

	        if (upload_bytes > TRANS_06_CONTENT_BYTES)
	        {
	            package_total++;    /* �ܰ����ۼ� */
	            upload_bytes = DP_FIELD_SYS_STAT_INFO_BYTES + msg_len;

	            /* Ѱ�ҵ�ǰ���ݰ�����ʼ��������(�ǵ�0������) */
	            if (GETBIT_N(info->request_type, CONT_SYS_STAT_DISP_RSRC))
	            {
	                /* �����ݸ������������ʱ����¼��ʱ����ʼ���� */
	                if (info->pkg_id == (package_total-1))
	                {
	                    idx_rec = i;
	                }
	            }
	        }
		}
    }

    /* ����ϱ��ֽ������� */
    upload_bytes = 0;

    /* ���֡ͷ */
    buf[0] = info->trigger_src;          /* ����Դ */
    buf[1] = info->verif_code[0];        /* ��֤�� */
    buf[2] = info->verif_code[1];        /* ��֤�� */
    buf[3] = info->verif_code[2];        /* ��֤�� */
    buf[4] = package_total;              /* �ܰ��� */
    buf[5] = info->pkg_id;               /* ��ǰ���� */
    buf[6] = info->trans_code;           /* ������ */

    /* ����м���Ϣ */
    buf[7] = TYBC_PCB_MAX;               /* ��ǰģ��� */
    buf[8] = upload_pv_num;              /* �ϱ�״̬�� */

    buf_len += TRANS_06_HEADER_BYTES;

    /* �������: ��ǰģ��״̬����ʾ��Դ */
    for (i = idx_rec; i < TYBC_SYS_PV_MAX; i++)
    {
		U08 msg_len = 0;
        /* ��Чģ���� */
        if (GETBIT_N(fg_upload_pv, i))
        {
			msg_len = TYB_SLP_GET_CODE_STR(useless_cache, TYBC_SYS_PV_TITLE(i));
            /* �ռ����: ��������ۼ��ֽ��� */
            upload_bytes = buf_len + DP_FIELD_SYS_STAT_INFO_BYTES + msg_len;

            if (upload_bytes <= RAW_DATA_BYTES - 2)
            {
                attr = TYBC_SYS_PV_ATTR(i);
                buf[buf_len++] = HIBYTE(attr);
                buf[buf_len++] = LOBYTE(attr);
                buf[buf_len++] = HIBYTE(TYBC_PV_VAL(TYBC_PCB_MAX, i));
                buf[buf_len++] = LOBYTE(TYBC_PV_VAL(TYBC_PCB_MAX, i));
                buf[buf_len++] = HIBYTE(msg_len);
                buf[buf_len++] = LOBYTE(msg_len);
                buf_len += TYB_SLP_GET_CODE_STR(&buf[buf_len], TYBC_SYS_PV_TITLE(i));
            }
            else
            {
                fg_multi_pkg = TRUE;
                idx_rec = i;
                break;
            }
        }
    }

    /* ������һ�� */
    info->pkg_id++;

    /* ������־ */
    if (buf_len != 0 && buf_len <= RAW_DATA_BYTES - 2)
    {
        buf[buf_len++] = 0xff;
        buf[buf_len++] = 0xff;
    }

    /* ����ģ��״̬��ʾ��Դ�ϴ���� */
    if (!fg_multi_pkg)
    {
        idx_rec = 0;          /* ��¼��� */
        info->pkg_id = 0;     /* ��ǰ��ID��λ */

        if (GETBIT_N(info->request_type, RST_SYS_STAT_DISP_ALL))    /* ��������ʱ�����ŷ���һ�������� */
        {
            info->trans_code = TRANS_SYS_STAT_DISP_CFG;
            CLRBIT_N(info->request_type, RST_SYS_STAT_DISP_ALL);
        }
        else    /* �ط����һ�����ݻ�����������ʾ��Դ���ݣ��������� */
        {
            info->trans_code = TRANS_NULL;
            fg_has_trans_task = FALSE;
        }
    }

    /* �ش�ĳ������ʱ�������������������룬����ͨ������ */
    if (GETBIT_N(info->request_type, CONT_SYS_STAT_DISP_RSRC))
    {
        fg_has_trans_task = FALSE;
        CLRBIT_N(info->request_type, CONT_SYS_STAT_DISP_RSRC);
    }
}


/*******************************************************************************
 �� ��: TYB_SLP_trans_sys_stat_disp_cfg()
 �� ��: �ְ��ϴ���ǰģ��״̬������Ϣ (ͨ������Э�飬������:0x07)
 �� ��: buf--���ܵ㻺����;
        info--����������Ϣ;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TYB_SLP_trans_sys_stat_disp_cfg(U08 *buf, TYB_TRANS_INFO *info)
{
    U16 i;
    U16 upload_bytes = 0;                   /* �ϴ��������ֽ��� */
    U08 package_total = 1;                  /* �ܰ���(����1��) */
    U08 upload_pv_num = 0;                  /* ģ�����ϱ����� */
    BOOL fg_multi_pkg = FALSE;              /* ������������־ */
    U08 buf_len = 0;
	U16 attr;
    static U16 idx_rec = 0;                 /* ��¼��һ�����ݵ���ʼ�������� */


    /* ���ݰ��ط���ʼ�������� */
    if (GETBIT_N(info->request_type, RST_SYS_STAT_DISP_CFG))
    {
        idx_rec = 0;
        CLRBIT_N(info->request_type, RST_SYS_STAT_DISP_CFG);
    }

    /* Ѱ�ҵ�ǰ���ݰ�����ʼ��������(��0������) */
    if (GETBIT_N(info->request_type, CONT_SYS_STAT_DISP_CFG))
    {
        if (info->pkg_id == 0)
        {
            idx_rec = 0;
        }
    }

    /* �����Ҫ�ϴ���ģ���� */
    if (!TYB_SLP_mark_upload_pv(TYBC_PCB_MAX, &upload_pv_num))
    {
        return;
    }

    /* �����ܰ�����������ʼ���� */
    for (i = 0; i < TYBC_PCB_MAX; i++)
    {
        if (GETBIT_N(fg_upload_pv, i))  /* ��Чģ���� */
        {
            upload_bytes += DP_FIELD_SYS_STAT_DATA_BYTES;

            if (upload_bytes > TRANS_07_CONTENT_BYTES)
            {
                package_total++;  // �ܰ����ۼ�
                upload_bytes = DP_FIELD_SYS_STAT_DATA_BYTES;

                /* Ѱ�ҵ�ǰ���ݰ�����ʼ��������(�ǵ�0������) */
                if (GETBIT_N(info->request_type, CONT_SYS_STAT_DISP_CFG))
                {
                    if (info->pkg_id == (package_total-1))
                    {
                        idx_rec = i;
                    }
                }
            }
        }
    }

    /* ����ϱ��ֽ������� */
    upload_bytes = 0;

    /* ���֡ͷ */
    buf[0] = info->trigger_src;      /* ����Դ */
    buf[1] = info->verif_code[0];    /* ��֤�� */
    buf[2] = info->verif_code[1];    /* ��֤�� */
    buf[3] = info->verif_code[2];    /* ��֤�� */
    buf[4] = package_total;          /* �ܰ��� */
    buf[5] = info->pkg_id;           /* ��ǰ���� */
    buf[6] = info->trans_code;       /* ������ */

    /* ����м���Ϣ */
    buf[7] = TYBC_PCB_MAX;           /* ��ǰģ��� */
    buf[8] = upload_pv_num;          /* �ϱ�״̬�� */

    buf_len += TRANS_07_HEADER_BYTES;

    /* �������: ��ǰģ��״̬��������Ϣ */
    for (i = idx_rec; i < TYBC_SYS_PV_MAX; i++)
    {
        if (GETBIT_N(fg_upload_pv, i))
        {
            /* �ռ����: ��������ۼ��ֽ��� */
            upload_bytes = buf_len + DP_FIELD_SYS_STAT_DATA_BYTES;

            /* д���ϱ����ܵ㻺���� */
            if (upload_bytes <= RAW_DATA_BYTES - 2)
            {
                attr = TYBC_SYS_PV_ATTR(i);
                buf[buf_len++] = HIBYTE(attr);
                buf[buf_len++] = LOBYTE(attr);
                buf[buf_len++] = HIBYTE(TYBC_PV_VAL(TYBC_PCB_MAX, i));
                buf[buf_len++] = LOBYTE(TYBC_PV_VAL(TYBC_PCB_MAX, i));
            }
            else
            {
                fg_multi_pkg = TRUE;
                idx_rec = i;
                break;
            }
        }
        else
        {
            continue;
        }
    }

    /* ������һ�� */
    info->pkg_id++;

    /* ������־ */
    if (buf_len != 0 && buf_len <= RAW_DATA_BYTES - 2)
    {
        buf[buf_len++] = 0xff;
        buf[buf_len++] = 0xff;
    }

    /* ���в�������������Ϣ�ϴ���� */
    if (!fg_multi_pkg)
    {
        idx_rec = 0;                                // ��¼���
        info->pkg_id = 0;                           // ��ǰ��ID��λ
        info->trans_code = TRANS_NULL;
        fg_has_trans_task = FALSE;
    }

    /* �ش����ݰ�ʱ�������������������룬����ͨ������ */
    if (GETBIT_N(info->request_type, CONT_SYS_STAT_DISP_CFG))
    {
        fg_has_trans_task = FALSE;
        CLRBIT_N(info->request_type, CONT_SYS_STAT_DISP_CFG);
    }

}



/*******************************************************************************
 �� ��: TYB_SLP_parse_sys_stat_cmd()
 �� ��: ����ģ��״̬����
 �� ��: buf--���ջ�����(����Դ);
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TYB_SLP_parse_sys_stat_cmd(U08 *buf)
{
    TYB_TRANS_INFO info;
    U08 buf_len = 0;

    /* ��ʼ�� */
    ARRCLR(info.addr);
    ARRCLR(info.request_type);

    /* ����Դ��Ϊ"״̬��ѯ"���򲻼����������� */
    if (buf[buf_len] != TRANS_MOD_STAT)
    {
        return;
    }

    /* ��1����:����Դ("״̬��ѯ"����) */
    info.trigger_src = buf[buf_len++];

    /* ��2����:��֤�� */
    MEMCPY(info.verif_code, &buf[buf_len], TRANS_VERIF_CODE_LEN);
    buf_len += TRANS_VERIF_CODE_LEN;

    /* ��3����: ������ */
    info.parse_code = buf[buf_len++];

    /* ��4����: ������ */
    info.request_code = buf[buf_len++];

    /* ��5����: ������� */
    info.pkg_id = buf[buf_len++];   // ��ָ�����ݰ���ʼ���ܰ����ڲ�����

    /* ��6����: ģ��� */
    info.addr[0] = buf[buf_len++];

    switch (info.parse_code)
    {
        /* ����ģ��״̬��Ϣ */
        case TRANS_CMD_SYS_STATUS_DISP:
            switch (info.request_code)
            {
                /* ��ͷ��ʼ��ȫ������ */
                case UPLOAD_DISP_ALL:
                    info.trans_code = TRANS_SYS_MSG_DISP_RSRC;
                    SETBIT_N(info.request_type, RST_SYS_STAT_DISP_ALL);
					SETBIT_N(info.request_type, RST_SYS_MSG_DISP_RSRC);
                    SETBIT_N(info.request_type, RST_SYS_STAT_DISP_RSRC);
                    SETBIT_N(info.request_type, RST_SYS_STAT_DISP_CFG);
                    break;

                /* ����������Ϣ���������ݰ�(��ǰģ��) */
                case UPLOAD_ALL_DISP_CFG:
                    info.trans_code = TRANS_MOD_STAT_DISP_CFG;
                    SETBIT_N(info.request_type, RST_SYS_STAT_DISP_CFG);
                    break;
            }

            /* ������� */
            if (TYB_SLP_is_node_unique(&info))
            {
                TYB_SLP_cache_append(&info);
            }
            break;

        default:
            break;
    }
}

#endif

/* ��"ģ��״̬" */
#ifdef TUYA_USE_MOD_STAT

/*******************************************************************************
 �� ��: TYB_SLP_trans_mod_msg_disp_rsrc()
 �� ��: �ְ��ϴ�ģ��״̬�ַ�����ʾ��Դ(ͨ������Э�飬������:)
 �� ��: buf--���ܵ㻺����;
        info--����������Ϣ;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TYB_SLP_trans_mod_msg_disp_rsrc(U08 *buf, TYB_TRANS_INFO *info)
{
    U16 i, j, pv_id;
    U08 useless_cache[TYBC_STR_BYTE_MAX];	/* ���û��� */
    U16 upload_bytes = 0;          /* �ϴ��������ֽ��� */
    U08 package_total = 1;         /* �ܰ���(����1��) */
    U08 upload_pv_num = 0;         /* ģ�����ϱ����� */
    BOOL fg_multi_pkg = FALSE;     /* ������������־ */
    U08 buf_len = 0;

    static U16 idx_rec = 0;        /* ��¼��һ�����ݵ���ʼ�������� */
	U08 mod_id = info->addr[0];

    /* ���ݰ��ط���ʼ�������� */
    if (GETBIT_N(info->request_type, RST_MOD_MSG_DISP_RSRC))
    {
        idx_rec = 0;
        CLRBIT_N(info->request_type, RST_MOD_MSG_DISP_RSRC);
    }

    /* Ѱ�ҵ�ǰ���ݰ�����ʼ��������(��0������) */
    if (GETBIT_N(info->request_type, CONT_MOD_MSG_DISP_RSRC))
    {
        if (info->pkg_id == 0)
        {
            idx_rec = 0;
        }
    }

    /* �����Ҫ�ϴ���ģ���� */
    if (!TYB_SLP_mark_upload_pv(mod_id, &upload_pv_num))
    {
        return;
    }

    /* �����ܰ�����������ʼ���� */
    for (i = 0; i < TYBC_PV_MAX; i++)
    {
        U08 msg_len = 0;
		U16 str_idx;
		U16 str_idx_before;
		BOOL fg_fill_str;

        if (i < TYBC_UNIT_PV_BEGIN)	//ѹ��ģ����
        {
            pv_id = i % TYBC_COMP_PV_MAX;
		}
		else		//ģ��ģ����
		{
            pv_id = i - TYBC_UNIT_PV_BEGIN;
		}

        /* ֵΪ�ַ�����ģ���� */
        if ((i < TYBC_COMP_PV_IN_USED && TYBC_COMP_PV_ATTR(pv_id)==0xFFFE)
		 || (i >= TYBC_UNIT_PV_BEGIN
		 	 && i < TYBC_UNIT_PV_BEGIN+TYBC_UNIT_PV_IN_USED
		 	 && TYBC_UNIT_PV_ATTR(pv_id) == 0xFFFE
		 	)
		)
        {
        	str_idx = (U16)(TYBC_PV_VAL(mod_id, i));
	        msg_len = TYB_SLP_GET_CODE_STR(useless_cache, str_idx);
			fg_fill_str = TRUE;
			//���ظ��ַ����������
			for (j=0; j<i; j++)
			{
				str_idx_before = (U16)(TYBC_PV_VAL(mod_id, j));
				if (str_idx == str_idx_before)
				{
					fg_fill_str = FALSE;
					break;
				}
			}
			//ʼ������һ��
			if (i == 0)
			{
				fg_fill_str= TRUE;
			}

			if (fg_fill_str)
			{
			    upload_bytes += DP_FIELD_DISP_RSRC_BYTES + msg_len;

			    if (upload_bytes > TRANS_14_CONTENT_BYTES)
			    {
			        package_total++;    /* �ܰ����ۼ� */
			        upload_bytes = DP_FIELD_DISP_RSRC_BYTES + msg_len;

			        /* Ѱ�ҵ�ǰ���ݰ�����ʼ��������(�ǵ�0������) */
			        if (GETBIT_N(info->request_type, CONT_MOD_MSG_DISP_RSRC))
			        {
			            /* �����ݸ������������ʱ����¼��ʱ����ʼ���� */
			            if (info->pkg_id == (package_total-1))
			            {
			                idx_rec = i;
			            }
			        }
			    }
			}


		}
    }

    /* ���֡ͷ */
    buf[0] = info->trigger_src;          /* ����Դ */
    buf[1] = info->verif_code[0];        /* ��֤�� */
    buf[2] = info->verif_code[1];        /* ��֤�� */
    buf[3] = info->verif_code[2];        /* ��֤�� */
    buf[4] = package_total;              /* �ܰ��� */
    buf[5] = info->pkg_id;               /* ��ǰ���� */
    buf[6] = info->trans_code;           /* ������ */

    /* ����м���Ϣ */
    buf[7] = mod_id;               /* ��ǰģ��� */
    buf[8] = 0;              		/* �ϱ�״̬�� */

    buf_len += TRANS_14_HEADER_BYTES;

    /* �������: ��ǰģ��״̬����ʾ��Դ */
    for (i = idx_rec; i < TYBC_PV_MAX; i++)
    {
		U08 msg_len = 0;
		U16 msg_id = 0;
		U16 str_idx;
		U16 str_idx_before;
		BOOL fg_fill_str;
        if (i < TYBC_UNIT_PV_BEGIN)	//ѹ��ģ����
        {
            pv_id = i % TYBC_COMP_PV_MAX;
		}
		else		//ģ��ģ����
		{
            pv_id = i - TYBC_UNIT_PV_BEGIN;
		}


        /* ֵΪ�ַ�����ģ���� */
        if ((i < TYBC_COMP_PV_IN_USED && TYBC_COMP_PV_ATTR(pv_id)==0xFFFE)
		 || (i >= TYBC_UNIT_PV_BEGIN
		 	 && i < TYBC_UNIT_PV_BEGIN+TYBC_UNIT_PV_IN_USED
		 	 && TYBC_UNIT_PV_ATTR(pv_id) == 0xFFFE
		 	)
		)
        {
        	str_idx = (U16)(TYBC_PV_VAL(mod_id, i));
			msg_len = TYB_SLP_GET_CODE_STR(useless_cache, str_idx);
			msg_id = TYBC_PV_VAL(mod_id, i);
			fg_fill_str = TRUE;
			//���ظ��ַ����������
			for (j=0; j<i ;j++)
			{
				str_idx_before = (U16)(TYBC_PV_VAL(mod_id, j));
				if (str_idx == str_idx_before)
				{
					fg_fill_str = FALSE;
					break;
				}
			}
			//ʼ������һ��
			if (i == 0)
			{
				fg_fill_str= TRUE;
			}

			if (fg_fill_str)
			{
		        /* �ռ����: ��������ۼ��ֽ��� */
		        upload_bytes = buf_len + DP_FIELD_DISP_RSRC_BYTES + msg_len;
				//test_msg_len = msg_len;
		        if (upload_bytes <= RAW_DATA_BYTES - 2)
		        {
		            buf[buf_len++] = HIBYTE(msg_id);
		            buf[buf_len++] = LOBYTE(msg_id);
		            buf[buf_len++] = HIBYTE(msg_len);
		            buf[buf_len++] = LOBYTE(msg_len);
					//test_msg_len = msg_len;
		            buf_len += TYB_SLP_GET_CODE_STR(&buf[buf_len], str_idx);
		        }
		        else
		        {
		            fg_multi_pkg = TRUE;
		            idx_rec = i;
		            break;
		        }
			}
        }
    }

    /* ������һ�� */
    info->pkg_id++;

    /* ���в���������ʾ��Դ�ϴ���� */
    if (!fg_multi_pkg)
    {
        idx_rec = 0;                                           /* ��¼��� */
        info->pkg_id = 0;                                      /* ��ǰ��ID��λ */
        if (GETBIT_N(info->request_type, RST_MOD_STAT_DISP_ALL))    /* ��������ʱ�����ŷ���һ�������� */
        {
            info->trans_code = TRANS_MOD_STAT_DISP_RSRC;       /* ׼������һ�������� */
            //CLRBIT_N(info->request_type, RST_SYS_STAT_DISP_ALL);
        }
        else    /* �ط����һ�����ݻ�����������ʾ��Դ���ݣ��������� */
        {
            info->trans_code = TRANS_NULL;
            fg_has_trans_task = FALSE;
        }
    }

    /* �ش����ݰ�ʱ�������������������룬����ͨ������ */
    if (GETBIT_N(info->request_type, CONT_MOD_MSG_DISP_RSRC))
    {
        fg_has_trans_task = FALSE;
        CLRBIT_N(info->request_type, CONT_MOD_MSG_DISP_RSRC);
    }
}

/*******************************************************************************
 �� ��: TYB_SLP_trans_mod_stat_disp_rsrc()
 �� ��: �ְ��ϴ���ǰģ��״̬��ʾ��Դ(ͨ������Э�飬������:0x06)
 �� ��: buf--���ܵ㻺����;
        info--����������Ϣ;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TYB_SLP_trans_mod_stat_disp_rsrc(U08 *buf, TYB_TRANS_INFO *info)
{
    U16 i,j,pv_id;
    U08 useless_cache[TYBC_STR_BYTE_MAX];        /* ���û��� */
    DP_FIELD_DISP_RSRC disp_rsrc; /* ģ��״̬��ʾ��Դ */
    U16 upload_bytes = 0;         /* �ϴ��������ֽ��� */
    U08 package_total = 1;        /* �ܰ���(����1��) */
    U08 upload_pv_num = 0;        /* ģ�����ϱ����� */
    BOOL fg_multi_pkg = FALSE;    /* ������������־ */
    U16 *src_buf = NULL;
    U08 buf_len = 0;
    U08 mod_id = info->addr[0];

    static U16 idx_rec = 0;       /* ��¼��һ�����ݵ���ʼ�������� */


    /* ���ݰ��ط���ʼ�������� */
    if (GETBIT_N(info->request_type, RST_MOD_STAT_DISP_RSRC))
    {
        idx_rec = 0;
        CLRBIT_N(info->request_type, RST_MOD_STAT_DISP_RSRC);
    }

    /* Ѱ�ҵ�ǰ���ݰ�����ʼ��������(��0������) */
    if (GETBIT_N(info->request_type, CONT_MOD_STAT_DISP_RSRC))
    {
        if (info->pkg_id == 0)
        {
            idx_rec = 0;
        }
    }


    /* �����Ҫ�ϴ���ģ���� */
    if (!TYB_SLP_mark_upload_pv(mod_id, &upload_pv_num))
    {
        return;
    }

    /* �����ܰ�����������ʼ���� */
    for (i = 0; i < TYBC_MOD_PV_MSG_MAX; i++)
    {
        U08 msg_len = 0;

        if (i < TYBC_COMP_PV_IN_USED)    /* ѹ��ģ���� */
        {
            pv_id = i;    /* ����ѹ��ģ�����ڲ����� */
            msg_len = TYB_SLP_GET_CODE_STR(useless_cache, TYBC_COMP_PV_TITLE(pv_id));
        }
        else if (i < TYBC_COMP_PV_IN_USED + TYBC_UNIT_PV_IN_USED)    /* ��Ԫģ���� */
        {
            pv_id = i - TYBC_COMP_PV_IN_USED;    /* ���㵥Ԫģ�����ڲ����� */
            msg_len = TYB_SLP_GET_CODE_STR(useless_cache, TYBC_UNIT_PV_TITLE(pv_id));
        }
        else
        {
            break;
        }

        upload_bytes += DP_FIELD_DISP_RSRC_BYTES + msg_len;

        if (upload_bytes > TRANS_06_CONTENT_BYTES)
        {
            package_total++;    /* �ܰ����ۼ� */
            upload_bytes = DP_FIELD_DISP_RSRC_BYTES + msg_len;

            /* Ѱ�ҵ�ǰ���ݰ�����ʼ��������(�ǵ�0������) */
            if (GETBIT_N(info->request_type, CONT_MOD_STAT_DISP_RSRC))
            {
                /* �����ݸ������������ʱ����¼��ʱ����ʼ���� */
                if (info->pkg_id == (package_total-1))
                {
                    idx_rec = i;
                }
            }
        }
    }

    /* ����ϱ��ֽ������� */
    upload_bytes = 0;

    /* ���֡ͷ */
    buf[0] = info->trigger_src;          /* ����Դ */
    buf[1] = info->verif_code[0];        /* ��֤�� */
    buf[2] = info->verif_code[1];        /* ��֤�� */
    buf[3] = info->verif_code[2];        /* ��֤�� */
    buf[4] = package_total;              /* �ܰ��� */
    buf[5] = info->pkg_id;               /* ��ǰ���� */
    buf[6] = info->trans_code;           /* ������ */

    /* ����м���Ϣ */
    buf[7] = mod_id;                     /* ��ǰģ��� */
    buf[8] = upload_pv_num;              /* �ϱ�״̬�� */

    buf_len += TRANS_06_HEADER_BYTES;

    /* �������: ��ǰģ��״̬����ʾ��Դ */
    for (i = idx_rec; i < TYBC_MOD_PV_MSG_MAX; i++)
    {
        if (i < TYBC_COMP_PV_IN_USED)    /* ѹ��ģ���� */
        {
            pv_id = i;    /* ����ѹ��ģ�����ڲ����� */
            disp_rsrc.msg_id = TYBC_COMP_PV_TITLE(pv_id);
            disp_rsrc.msg_len = TYB_SLP_GET_CODE_STR(useless_cache, TYBC_COMP_PV_TITLE(pv_id));
        }
        else if (i < TYBC_COMP_PV_IN_USED + TYBC_UNIT_PV_IN_USED)    /* ��Ԫģ���� */
        {
            pv_id = i - TYBC_COMP_PV_IN_USED;    /* ���㵥Ԫģ�����ڲ����� */
            disp_rsrc.msg_id = TYBC_UNIT_PV_TITLE(pv_id);
            disp_rsrc.msg_len = TYB_SLP_GET_CODE_STR(useless_cache, TYBC_UNIT_PV_TITLE(pv_id));
        }
        else
        {
            disp_rsrc.msg_id = 0;
            disp_rsrc.msg_len = 0;
        }

        /* �ַ���ID��0 */
        if (disp_rsrc.msg_id != 0 && disp_rsrc.msg_len != 0)
        {
            /* �ռ����: ��������ۼ��ֽ��� */
            upload_bytes = buf_len + DP_FIELD_DISP_RSRC_BYTES + disp_rsrc.msg_len;

            /* д���ϱ����ܵ㻺���� */
            src_buf = (U16*)&disp_rsrc;
            if (upload_bytes <= RAW_DATA_BYTES)
            {
                for (j = 0; j < DP_FIELD_DISP_RSRC_SIZE; j++)
                {
                    buf[buf_len++] = HIBYTE(src_buf[j]);
                    buf[buf_len++] = LOBYTE(src_buf[j]);
                }

                /* д���ַ��� */
                buf_len += TYB_SLP_GET_CODE_STR(&buf[buf_len], disp_rsrc.msg_id);
            }
            else
            {
                fg_multi_pkg = TRUE;
                idx_rec = i;
                break;
            }
        }
    }

    /* ������һ�� */
    info->pkg_id++;

    /* ����ģ��״̬��ʾ��Դ�ϴ���� */
    if (!fg_multi_pkg)
    {
        idx_rec = 0;          /* ��¼��� */
        info->pkg_id = 0;     /* ��ǰ��ID��λ */

        if (GETBIT_N(info->request_type, RST_MOD_STAT_DISP_ALL))    /* ��������ʱ�����ŷ���һ�������� */
        {
            info->trans_code = TRANS_MOD_STAT_DISP_CFG;
            CLRBIT_N(info->request_type, RST_MOD_STAT_DISP_ALL);
        }
        else    /* �ط����һ�����ݻ�����������ʾ��Դ���ݣ��������� */
        {
            info->trans_code = TRANS_NULL;
            fg_has_trans_task = FALSE;
        }
    }

    /* �ش�ĳ������ʱ�������������������룬����ͨ������ */
    if (GETBIT_N(info->request_type, CONT_MOD_STAT_DISP_RSRC))
    {
        fg_has_trans_task = FALSE;
        CLRBIT_N(info->request_type, CONT_MOD_STAT_DISP_RSRC);
    }
}


/*******************************************************************************
 �� ��: TYB_SLP_trans_mod_stat_disp_cfg()
 �� ��: �ְ��ϴ���ǰģ��״̬������Ϣ (ͨ������Э�飬������:0x07)
 �� ��: buf--���ܵ㻺����;
        info--����������Ϣ;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TYB_SLP_trans_mod_stat_disp_cfg(U08 *buf, TYB_TRANS_INFO *info)
{
    U16 i,j,pv_id,comp_id;
    DP_FIELD_MOD_STAT_DISP_CFG disp_cfg;    /* ģ��״̬������Ϣ */
    U16 upload_bytes = 0;                   /* �ϴ��������ֽ��� */
    U08 package_total = 1;                  /* �ܰ���(����1��) */
    U08 upload_pv_num = 0;                  /* ģ�����ϱ����� */
    BOOL fg_multi_pkg = FALSE;              /* ������������־ */
    U16 *src_buf = NULL;
    U08 buf_len = 0;
    U08 mod_id = info->addr[0];

    static U16 idx_rec = 0;                 /* ��¼��һ�����ݵ���ʼ�������� */


    /* ���ݰ��ط���ʼ�������� */
    if (GETBIT_N(info->request_type, RST_MOD_STAT_DISP_CFG))
    {
        idx_rec = 0;
        CLRBIT_N(info->request_type, RST_MOD_STAT_DISP_CFG);
    }

    /* Ѱ�ҵ�ǰ���ݰ�����ʼ��������(��0������) */
    if (GETBIT_N(info->request_type, CONT_MOD_STAT_DISP_CFG))
    {
        if (info->pkg_id == 0)
        {
            idx_rec = 0;
        }
    }

    /* �����Ҫ�ϴ���ģ���� */
    if (!TYB_SLP_mark_upload_pv(mod_id, &upload_pv_num))
    {
        return;
    }

    /* �����ܰ�����������ʼ���� */
    for (i = 0; i < TYBC_PV_MAX; i++)
    {
        if (GETBIT_N(fg_upload_pv, i))  /* ��Чģ���� */
        {
            upload_bytes += DP_FIELD_MOD_STAT_DISP_CFG_BYTES;

            if (upload_bytes > TRANS_07_CONTENT_BYTES)
            {
                package_total++;  // �ܰ����ۼ�
                upload_bytes = DP_FIELD_MOD_STAT_DISP_CFG_BYTES;

                /* Ѱ�ҵ�ǰ���ݰ�����ʼ��������(�ǵ�0������) */
                if (GETBIT_N(info->request_type, CONT_MOD_STAT_DISP_CFG))
                {
                    if (info->pkg_id == (package_total-1))
                    {
                        idx_rec = i;
                    }
                }
            }
        }
    }

    /* ����ϱ��ֽ������� */
    upload_bytes = 0;

    /* ���֡ͷ */
    buf[0] = info->trigger_src;      /* ����Դ */
    buf[1] = info->verif_code[0];    /* ��֤�� */
    buf[2] = info->verif_code[1];    /* ��֤�� */
    buf[3] = info->verif_code[2];    /* ��֤�� */
    buf[4] = package_total;          /* �ܰ��� */
    buf[5] = info->pkg_id;           /* ��ǰ���� */
    buf[6] = info->trans_code;       /* ������ */

    /* ����м���Ϣ */
    buf[7] = mod_id;                 /* ��ǰģ��� */
    buf[8] = upload_pv_num;          /* �ϱ�״̬�� */

    buf_len += TRANS_07_HEADER_BYTES;

    /* �������: ��ǰģ��״̬��������Ϣ */
    for (i = idx_rec; i < TYBC_PV_MAX; i++)
    {
        if (GETBIT_N(fg_upload_pv, i))
        {
            /* ģ����ֵ */
            disp_cfg.value = TYBC_PV_VAL(mod_id,i);

            /* ѹ��ģ���� */
            if (i < TYBC_UNIT_PV_BEGIN)
            {
                /* ����ѹ��ģ�����е����λ��&ѹ���� */
                pv_id = i % TYBC_COMP_PV_MAX;
                comp_id = i / TYBC_COMP_PV_MAX;

                disp_cfg.msg_id = TYBC_COMP_PV_TITLE(pv_id);    /* ���� */
                disp_cfg.id = MAKEWORD(mod_id, comp_id);       /* ģ���+ѹ���� */
                disp_cfg.attr = TYBC_COMP_PV_ATTR(pv_id);       /* ���� */
            }
            else
            {
                /* ���㵥Ԫģ�����е����λ�� */
                pv_id = i - TYBC_UNIT_PV_BEGIN;

                disp_cfg.msg_id = TYBC_UNIT_PV_TITLE(pv_id);    /* ���� */
                disp_cfg.id = MAKEWORD(mod_id, 0xff);          /* ģ���+0xff(��ѹ����) */
                disp_cfg.attr = TYBC_UNIT_PV_ATTR(pv_id);       /* ���� */
            }

            /* �ռ����: ��������ۼ��ֽ��� */
            upload_bytes = buf_len + DP_FIELD_MOD_STAT_DISP_CFG_BYTES;

            /* д���ϱ����ܵ㻺���� */
            if (upload_bytes <= RAW_DATA_BYTES)
            {
                src_buf = (U16*)&disp_cfg;
                for (j = 0; j < DP_FIELD_MOD_STAT_DISP_CFG_SIZE; j++)
                {
                    buf[buf_len++] = HIBYTE(src_buf[j]);
                    buf[buf_len++] = LOBYTE(src_buf[j]);
                }
            }
            else
            {
                fg_multi_pkg = TRUE;
                idx_rec = i;
                break;
            }
        }
        else
        {
            continue;
        }
    }

    /* ������һ�� */
    info->pkg_id++;

    /* ���в�������������Ϣ�ϴ���� */
    if (!fg_multi_pkg)
    {
        idx_rec = 0;                                // ��¼���
        info->pkg_id = 0;                           // ��ǰ��ID��λ
        info->trans_code = TRANS_NULL;
        fg_has_trans_task = FALSE;
    }

    /* �ش����ݰ�ʱ�������������������룬����ͨ������ */
    if (GETBIT_N(info->request_type, CONT_MOD_STAT_DISP_CFG))
    {
        fg_has_trans_task = FALSE;
        CLRBIT_N(info->request_type, CONT_MOD_STAT_DISP_CFG);
    }
}


/*******************************************************************************
 �� ��: TYB_SLP_parse_mod_stat_cmd()
 �� ��: ����ģ��״̬����
 �� ��: buf--���ջ�����(����Դ);
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TYB_SLP_parse_mod_stat_cmd(U08 *buf)
{
    TYB_TRANS_INFO info;
    U08 buf_len = 0;

    /* ��ʼ�� */
    ARRCLR(info.addr);
    ARRCLR(info.request_type);

    /* ����Դ��Ϊ"״̬��ѯ"���򲻼����������� */
    if (buf[buf_len] != TRANS_MOD_STAT)
    {
        return;
    }

    /* ��1����:����Դ("״̬��ѯ"����) */
    info.trigger_src = buf[buf_len++];

    /* ��2����:��֤�� */
    MEMCPY(info.verif_code, &buf[buf_len], TRANS_VERIF_CODE_LEN);
    buf_len += TRANS_VERIF_CODE_LEN;

    /* ��3����: ������ */
    info.parse_code = buf[buf_len++];

    /* ��4����: ������ */
    info.request_code = buf[buf_len++];

    /* ��5����: ������� */
    info.pkg_id = buf[buf_len++];   // ��ָ�����ݰ���ʼ���ܰ����ڲ�����

    /* ��6����: ģ��� */
    info.addr[0] = buf[buf_len++];

    switch (info.parse_code)
    {
        /* ����ģ��״̬��Ϣ */
        case TRANS_CMD_MOD_STATUS_DISP:
            switch (info.request_code)
            {
                /* ��ͷ��ʼ��ȫ������ */
                case UPLOAD_DISP_ALL:
                    info.trans_code = TRANS_MOD_MSG_DISP_RSRC;
                    SETBIT_N(info.request_type, RST_MOD_STAT_DISP_ALL);
                    SETBIT_N(info.request_type, RST_MOD_STAT_DISP_RSRC);
                    SETBIT_N(info.request_type, RST_MOD_STAT_DISP_CFG);
					SETBIT_N(info.request_type, RST_MOD_MSG_DISP_RSRC);
                    break;

                /* ������ʾ��Դ�е�ĳ������ */
                case UPLOAD_ONE_DISP_RSRC:
                    info.trans_code = TRANS_MOD_STAT_DISP_RSRC;
                    SETBIT_N(info.request_type, CONT_MOD_STAT_DISP_RSRC);
                    break;

                /* ����������Ϣ�е�ĳ������ */
                case UPLOAD_ONE_DISP_CFG:
                    info.trans_code = TRANS_MOD_STAT_DISP_CFG;
                    SETBIT_N(info.request_type, CONT_MOD_STAT_DISP_CFG);
                    break;

                /* ������ʾ��Դ���������ݰ�(����ģ��״̬) */
                case UPLOAD_ALL_DISP_RSRC:
                    info.trans_code = TRANS_MOD_STAT_DISP_RSRC;
                    SETBIT_N(info.request_type, RST_MOD_STAT_DISP_RSRC);
                    break;

                /* ����������Ϣ���������ݰ�(��ǰģ��) */
                case UPLOAD_ALL_DISP_CFG:
                    info.trans_code = TRANS_MOD_STAT_DISP_CFG;
                    SETBIT_N(info.request_type, RST_MOD_STAT_DISP_CFG);
                    break;
            }

            /* ������� */
            if (TYB_SLP_is_node_unique(&info))
            {
                TYB_SLP_cache_append(&info);
            }
            break;

        default:
            break;
    }
}

#endif

/* ��"�õ�״̬" */
#ifdef TUYA_USE_ELE_STAT
/*****************************************************************************
��  �� : SH_sign_local2sh()
��  �� : �õ������Դ���
��  �� : U16 sign_local :
��  �� : ��
��  �� : ��
*****************************************************************************/

const U16 msg_title_week[] ={
    msg_tyb_title_week_sun,
	msg_tyb_title_week_mon,
	msg_tyb_title_week_tue,
	msg_tyb_title_week_wen,
	msg_tyb_title_week_thu,
	msg_tyb_title_week_fri,
	msg_tyb_title_week_sat,
};

const U16 msg_title_month[] = {
	msg_tyb_title_month_1,
	msg_tyb_title_month_2,
	msg_tyb_title_month_3,
	msg_tyb_title_month_4,
	msg_tyb_title_month_5,
	msg_tyb_title_month_6,
	msg_tyb_title_month_7,
	msg_tyb_title_month_8,
	msg_tyb_title_month_9,
	msg_tyb_title_month_10,
	msg_tyb_title_month_11,
	msg_tyb_title_month_12,
};

I16 week_cache[] =  {0 ,1 ,2 ,3 ,4 ,5 ,6};                  //ԭʼ��
I16 year_cache[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}; //ԭʼ��

I16 week_ele_val[][MAX_WEEK_VAL] = {
	{123, 234, 345 ,456 , 567, 456, 234,},/* ��ˮ */
	{234, 345, 456 ,567 , 456, 345, 567,},/* ���� */
	{345, 456, 567 ,345 , 345, 567, 572,},/* ���� */
};

I16 year_ele_val[][MAX_YEAR_VAL] = {
	{4560, 2340, 3450 ,4560, 5670, 4560, 3450, 2340, 3450 ,4560 , 5670, 4560,},	/* ��ˮ */
	{5670, 4560, 3450 ,4560 , 5670, 4560, 3450, 2340, 3450 ,4560 , 5670, 4560,},/* ���� */
	{3450, 2340, 3450 ,4560 , 5670, 4560, 3450, 2340, 3450 ,4560 , 5670, 4560,},/* ���� */
};

U16 TYB_ELE_STA_ATTR(U16 attr_chart)
{
	U16 sign_sh;
    U16 unit_sh = 0;         /* ��λ */
    U16 chart_sh = 0;        /* ����ͳ�Ʊ� */
    U16 decimal_sh = 0;      /* С��λ */

    /* ��λ */
  	unit_sh = SH_UNIT_kWH;         /* kWH */
	/* ����ͳ�Ʊ� */
	chart_sh = attr_chart;
    /* С��λ */
    decimal_sh = 0;

    /* �ϳ��������� */
    sign_sh = (unit_sh << 0)
            | (chart_sh << CHART_BGN_BIT)
            | (decimal_sh << ELE_DECIMAL_BGN_BIT);

    return sign_sh;
}

/*******************************************************************************
 �� ��: TYB_SLP_trans_ele_stat_disp_rsrc()
 �� ��: �ְ��ϴ���ǰ�õ�״̬��ʾ��Դ(ͨ������Э�飬������:0x08)
 �� ��: buf--���ܵ㻺����;
        info--����������Ϣ;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TYB_SLP_trans_ele_stat_disp_rsrc(U08 *buf, TYB_TRANS_INFO *info)
{
    U16 i,j;
    U08 useless_cache[TYBC_STR_BYTE_MAX];        /* ���û��� */
    DP_FIELD_DISP_RSRC disp_rsrc; /* ģ��״̬��ʾ��Դ */
    U16 upload_bytes = 0;         /* �ϴ��������ֽ��� */
    U08 package_total = 1;        /* �ܰ���(����1��) */
    U08 upload_pv_num = MAX_MODE_ELE*(MAX_YEAR_VAL+MAX_WEEK_VAL);        /* ģ�����ϱ����� */
    BOOL fg_multi_pkg = FALSE;    /* ������������־ */
    U16 *src_buf = NULL;
    U08 buf_len = 0;
	U16 msg_index_cache;		  /* �洢�ַ������� */

    static U16 idx_rec = 0;       /* ��¼��һ�����ݵ���ʼ�������� */

    /* ����cache */
    {
        I08 idx = 0;
        LOG_RTC time_week = fun_time_localtime(pwru_week_cache.node[pwru_week_cache.queue.end].timestamp);
        LOG_RTC time_year = fun_time_localtime(pwru_year_cache.node[pwru_year_cache.queue.end].timestamp);
        /* end����ǰ��ʼ�շ������ұ���ʾ�����ŵ�week_cache��������ұ� */
        for (idx=POWER_USAGE_WEEK_SIZE-1; idx>=0; idx--)
        {
            week_cache[idx] = (time_week.week+idx+1)%POWER_USAGE_WEEK_SIZE;
        }

        for (idx=POWER_USAGE_YEAR_SIZE-1; idx>=0; idx--)
        {
            year_cache[idx] = (time_year.month+idx)%POWER_USAGE_YEAR_SIZE;
        }
    }


    /* ���ݰ��ط���ʼ�������� */
    if (GETBIT_N(info->request_type, RST_ELE_STAT_DISP_RSRC))
    {
        idx_rec = 0;
        CLRBIT_N(info->request_type, RST_ELE_STAT_DISP_RSRC);
    }

    /* Ѱ�ҵ�ǰ���ݰ�����ʼ��������(��0������) */
    if (GETBIT_N(info->request_type, CONT_ELE_STAT_DISP_RSRC))
    {
        if (info->pkg_id == 0)
        {
            idx_rec = 0;
        }
    }

    /* �����ܰ�����������ʼ���� */
    for (i = 0; i < MAX_MODE_ELE*(MAX_YEAR_VAL+MAX_WEEK_VAL); i++)
    {
        U08 msg_len = 0;

    	if (i >= 0 && i < MAX_MODE_ELE*MAX_WEEK_VAL)			//���һ���õ�����
    	{
			msg_len = TYB_SLP_GET_CODE_STR(useless_cache, msg_title_week[week_cache[i%MAX_WEEK_VAL]]);
    	}
		else if (i < MAX_MODE_ELE*(MAX_YEAR_VAL+MAX_WEEK_VAL)) 	//���һ���õ�����
		{
			U16 offset_year = MAX_MODE_ELE*MAX_WEEK_VAL;
			msg_len = TYB_SLP_GET_CODE_STR(useless_cache, msg_title_month[year_cache[(i-offset_year)%MAX_YEAR_VAL]]);
		}

        upload_bytes += DP_FIELD_DISP_RSRC_BYTES + msg_len;

        if (upload_bytes > TRANS_08_CONTENT_BYTES)
        {
            package_total++;    /* �ܰ����ۼ� */
            upload_bytes = DP_FIELD_DISP_RSRC_BYTES + msg_len;

            /* Ѱ�ҵ�ǰ���ݰ�����ʼ��������(�ǵ�0������) */
            if (GETBIT_N(info->request_type, CONT_ELE_STAT_DISP_RSRC))
            {
                /* �����ݸ������������ʱ����¼��ʱ����ʼ���� */
                if (info->pkg_id == (package_total-1))
                {
                    idx_rec = i;
                }
            }
        }
    }

    /* ����ϱ��ֽ������� */
    upload_bytes = 0;

    /* ���֡ͷ */
    buf[0] = info->trigger_src;          /* ����Դ */
    buf[1] = info->verif_code[0];        /* ��֤�� */
    buf[2] = info->verif_code[1];        /* ��֤�� */
    buf[3] = info->verif_code[2];        /* ��֤�� */
    buf[4] = package_total;              /* �ܰ��� */
    buf[5] = info->pkg_id;               /* ��ǰ���� */
    buf[6] = info->trans_code;           /* ������ */

    /* ����м���Ϣ */
    buf[7] = upload_pv_num;              /* �ϱ�״̬�� */

    buf_len += TRANS_08_HEADER_BYTES;

    /* �������: ��ǰģ��״̬����ʾ��Դ */
    for (i = idx_rec; i < MAX_MODE_ELE*(MAX_YEAR_VAL+MAX_WEEK_VAL); i++)
    {

    	if (i >= 0 && i < MAX_MODE_ELE*MAX_WEEK_VAL)			//���һ���õ�����
    	{
    		disp_rsrc.msg_id = 1 + i % MAX_WEEK_VAL;
			msg_index_cache = msg_title_week[week_cache[i%MAX_WEEK_VAL]];
			disp_rsrc.msg_len = TYB_SLP_GET_CODE_STR(useless_cache, msg_index_cache);
    	}
		else if (i < MAX_MODE_ELE*(MAX_YEAR_VAL+MAX_WEEK_VAL))
		{
			U16 offset_year = MAX_MODE_ELE*MAX_WEEK_VAL;
			disp_rsrc.msg_id = 1 + MAX_WEEK_VAL + (i-offset_year) % MAX_YEAR_VAL;
			msg_index_cache = msg_title_month[year_cache[(i-offset_year)%MAX_YEAR_VAL]];
			disp_rsrc.msg_len = TYB_SLP_GET_CODE_STR(useless_cache, msg_index_cache);

		}

       /* �ռ����: ��������ۼ��ֽ��� */
       upload_bytes = buf_len + DP_FIELD_DISP_RSRC_BYTES + disp_rsrc.msg_len;

       /* д���ϱ����ܵ㻺���� */
       src_buf = (U16*)&disp_rsrc;
       if (upload_bytes <= RAW_DATA_BYTES)
       {
           for (j = 0; j < DP_FIELD_DISP_RSRC_SIZE; j++)
           {
               buf[buf_len++] = HIBYTE(src_buf[j]);
               buf[buf_len++] = LOBYTE(src_buf[j]);
           }

           /* д���ַ��� */
		   buf_len += TYB_SLP_GET_CODE_STR(&buf[buf_len], msg_index_cache);

       }
       else
       {
           fg_multi_pkg = TRUE;
           idx_rec = i;
           break;
       }

    }

    /* ������һ�� */
    info->pkg_id++;

    /* ����ģ��״̬��ʾ��Դ�ϴ���� */
    if (!fg_multi_pkg)
    {
        idx_rec = 0;          /* ��¼��� */
        info->pkg_id = 0;     /* ��ǰ��ID��λ */

        if (GETBIT_N(info->request_type, RST_ELE_STAT_DISP_ALL))    /* ��������ʱ�����ŷ���һ�������� */
        {
            info->trans_code = TRANS_ELE_STAT_DISP_CFG;
            CLRBIT_N(info->request_type, RST_ELE_STAT_DISP_ALL);
        }
        else    /* �ط����һ�����ݻ�����������ʾ��Դ���ݣ��������� */
        {
            info->trans_code = TRANS_NULL;
            fg_has_trans_task = FALSE;
        }
    }

    /* �ش�ĳ������ʱ�������������������룬����ͨ������ */
    if (GETBIT_N(info->request_type, CONT_ELE_STAT_DISP_RSRC))
    {
        fg_has_trans_task = FALSE;
        CLRBIT_N(info->request_type, CONT_ELE_STAT_DISP_RSRC);
    }
}


/*******************************************************************************
 �� ��: TYB_SLP_trans_ele_stat_disp_cfg()
 �� ��: �ְ��ϴ���ǰģ��״̬������Ϣ (ͨ������Э�飬������:0x09)
 �� ��: buf--���ܵ㻺����;
        info--����������Ϣ;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TYB_SLP_trans_ele_stat_disp_cfg(U08 *buf, TYB_TRANS_INFO *info)
{
    U16 i,j;
    DP_FIELD_ELE_STAT_DISP_CFG disp_cfg;    /* ģ��״̬������Ϣ */
    U16 upload_bytes = 0;                   /* �ϴ��������ֽ��� */
    U08 package_total = 1;                  /* �ܰ���(����1��) */
	/* ģ�����ϱ����� */
    U08 upload_pv_num = (MAX_WEEK_VAL+MAX_YEAR_VAL)*MAX_MODE_ELE;
    BOOL fg_multi_pkg = FALSE;              /* ������������־ */
    U16 *src_buf = NULL;
    U08 buf_len = 0;

    static U16 idx_rec = 0;                 /* ��¼��һ�����ݵ���ʼ�������� */

    /* ����ele_val */
    {
        U08 end = 0;
        I08 idx;
        /* end����ǰ����end��ǰ������nodeȡ���� */
        for (idx=POWER_USAGE_WEEK_SIZE-1; idx>=0; idx--)
        {
            end = (pwru_week_cache.queue.end+idx+1)%POWER_USAGE_WEEK_SIZE;

            week_ele_val[0][idx] = pwru_week_cache.node[end].val_dhw; /* ��ˮ */
            week_ele_val[1][idx] = pwru_week_cache.node[end].val_heat;/* ���� */
            week_ele_val[2][idx] = pwru_week_cache.node[end].val_cool;/* ���� */
        }

        for (idx=POWER_USAGE_YEAR_SIZE-1; idx>=0; idx--)
        {
            end = (pwru_year_cache.queue.end+idx+1)%POWER_USAGE_YEAR_SIZE;

            year_ele_val[0][idx] = pwru_year_cache.node[end].val_dhw; /* ��ˮ */
            year_ele_val[1][idx] = pwru_year_cache.node[end].val_heat;/* ���� */
            year_ele_val[2][idx] = pwru_year_cache.node[end].val_cool;/* ���� */
        }
    }


    /* ���ݰ��ط���ʼ�������� */
    if (GETBIT_N(info->request_type, RST_ELE_STAT_DISP_CFG))
    {
        idx_rec = 0;
        CLRBIT_N(info->request_type, RST_ELE_STAT_DISP_CFG);
    }

    /* Ѱ�ҵ�ǰ���ݰ�����ʼ��������(��0������) */
    if (GETBIT_N(info->request_type, CONT_ELE_STAT_DISP_CFG))
    {
        if (info->pkg_id == 0)
        {
            idx_rec = 0;
        }
    }

    /* �����ܰ�����������ʼ���� */
    for (i = 0; i < MAX_MODE_ELE*(MAX_YEAR_VAL+MAX_WEEK_VAL); i++)
    {
        upload_bytes += DP_FIELD_ELE_STAT_DISP_CFG_BYTES;

        if (upload_bytes > TRANS_09_CONTENT_BYTES)
        {
            package_total++;  // �ܰ����ۼ�
            upload_bytes = DP_FIELD_ELE_STAT_DISP_CFG_BYTES;

            /* Ѱ�ҵ�ǰ���ݰ�����ʼ��������(�ǵ�0������) */
            if (GETBIT_N(info->request_type, CONT_ELE_STAT_DISP_CFG))
            {
                if (info->pkg_id == (package_total-1))
                {
                    idx_rec = i;
                }
            }
        }
    }

    /* ����ϱ��ֽ������� */
    upload_bytes = 0;

    /* ���֡ͷ */
    buf[0] = info->trigger_src;      /* ����Դ */
    buf[1] = info->verif_code[0];    /* ��֤�� */
    buf[2] = info->verif_code[1];    /* ��֤�� */
    buf[3] = info->verif_code[2];    /* ��֤�� */
    buf[4] = package_total;          /* �ܰ��� */
    buf[5] = info->pkg_id;           /* ��ǰ���� */
    buf[6] = info->trans_code;       /* ������ */

    /* ����м���Ϣ */
    buf[7] = upload_pv_num;          /* �ϱ�״̬�� */

    buf_len += TRANS_09_HEADER_BYTES;

    /* �������: ��ǰģ��״̬��������Ϣ */
    for (i = idx_rec; i < (MAX_WEEK_VAL+MAX_YEAR_VAL)*MAX_MODE_ELE; i++)
    {
    	U16 attr_chart;
    	if (i >= 0 && i < MAX_MODE_ELE*MAX_WEEK_VAL)			//���һ���õ�����
    	{
			/*  �õ���ֵ & �ַ���id & ģʽid */
			if (i < MODE_HEAT_ELE*MAX_WEEK_VAL)
			{
				disp_cfg.value = week_ele_val[MODE_HOT_ELE][i];										//��ˮ���õ�
				disp_cfg.msg_id = 1 + i;
				disp_cfg.mode_id = MODE_HOT_ELE;
			}
			else if (i < MODE_COOL_ELE*MAX_WEEK_VAL)
			{
				disp_cfg.value = week_ele_val[MODE_HEAT_ELE][i - MODE_HEAT_ELE*MAX_WEEK_VAL];	 		//�������õ�
				disp_cfg.msg_id = 1 + i - MODE_HEAT_ELE*MAX_WEEK_VAL;
				disp_cfg.mode_id = MODE_HEAT_ELE;
			}
			else if (i < MAX_MODE_ELE*MAX_WEEK_VAL)
			{
				disp_cfg.value = week_ele_val[MODE_COOL_ELE][i - MODE_COOL_ELE*MAX_WEEK_VAL];		 	//�������õ�
				disp_cfg.msg_id = 1 + i - MODE_COOL_ELE*MAX_WEEK_VAL;
				disp_cfg.mode_id = MODE_COOL_ELE;
			}
			/* ���� */
			attr_chart = 0;
			disp_cfg.attr =  TYB_ELE_STA_ATTR(attr_chart);
		}
		else if (i < (MAX_WEEK_VAL+MAX_YEAR_VAL)*MAX_MODE_ELE)		//���һ���õ�����
		{
			U16 offest_year = MAX_MODE_ELE*MAX_WEEK_VAL;
			/*  �õ���ֵ & �ַ���id & ģʽid */
			if (i < offest_year + MODE_HEAT_ELE*MAX_YEAR_VAL)
			{
				disp_cfg.value = year_ele_val[MODE_HOT_ELE][i - offest_year];									//��ˮ���õ�
				disp_cfg.msg_id = 1 + MAX_WEEK_VAL + i - offest_year;
				disp_cfg.mode_id = MODE_HOT_ELE;
			}
			else if (i < offest_year + MODE_COOL_ELE*MAX_YEAR_VAL)
			{
				disp_cfg.value = year_ele_val[MODE_HEAT_ELE][i - offest_year - MODE_HEAT_ELE*MAX_YEAR_VAL];   //�������õ�
				disp_cfg.msg_id = 1 + MAX_WEEK_VAL + i - offest_year - MODE_HEAT_ELE*MAX_YEAR_VAL;
				disp_cfg.mode_id = MODE_HEAT_ELE;
			}
			else if (i < offest_year + MAX_MODE_ELE*MAX_YEAR_VAL)
			{
				disp_cfg.value = year_ele_val[MODE_COOL_ELE][i - offest_year - MODE_COOL_ELE*MAX_YEAR_VAL];	//�������õ�
				disp_cfg.msg_id = 1 + MAX_WEEK_VAL + i - offest_year - MODE_COOL_ELE*MAX_YEAR_VAL;
				disp_cfg.mode_id = MODE_COOL_ELE;
			}
			/* ���� */
			attr_chart = 1;
			disp_cfg.attr =  TYB_ELE_STA_ATTR(attr_chart);
		}



        /* �ռ����: ��������ۼ��ֽ��� */
        upload_bytes = buf_len + DP_FIELD_ELE_STAT_DISP_CFG_BYTES;

        /* д���ϱ����ܵ㻺���� */
        if (upload_bytes <= RAW_DATA_BYTES)
        {
            src_buf = (U16*)&disp_cfg;
            for (j = 0; j < DP_FIELD_MOD_STAT_DISP_CFG_SIZE; j++)
            {
                buf[buf_len++] = HIBYTE(src_buf[j]);
                buf[buf_len++] = LOBYTE(src_buf[j]);
            }
        }
        else
        {
            fg_multi_pkg = TRUE;
            idx_rec = i;
            break;
        }
    }

    /* ������һ�� */
    info->pkg_id++;

    /* ����������Ϣ�ϴ���� */
    if (!fg_multi_pkg)
    {
        idx_rec = 0;                                // ��¼���
        info->pkg_id = 0;                           // ��ǰ��ID��λ
        info->trans_code = TRANS_NULL;
        fg_has_trans_task = FALSE;
    }

    /* �ش����ݰ�ʱ�������������������룬����ͨ������ */
    if (GETBIT_N(info->request_type, CONT_ELE_STAT_DISP_CFG))
    {
        fg_has_trans_task = FALSE;
        CLRBIT_N(info->request_type, CONT_ELE_STAT_DISP_CFG);
    }
}


/*******************************************************************************
 �� ��: TYB_SLP_parse_ele_stat_cmd()
 �� ��: �����õ�״̬����
 �� ��: buf--���ջ�����(����Դ);
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TYB_SLP_parse_ele_stat_cmd(U08 *buf)
{
    TYB_TRANS_INFO info;
    U08 buf_len = 0;

    /* ��ʼ�� */
    ARRCLR(info.addr);
    ARRCLR(info.request_type);

    /* ����Դ��Ϊ"�õ��ѯ"���򲻼����������� */
    if (buf[buf_len] != TRANS_ELE_STAT)
    {
        return;
    }

    /* ��1����:����Դ("�õ��ѯ"����) */
    info.trigger_src = buf[buf_len++];

    /* ��2����:��֤�� */
    MEMCPY(info.verif_code, &buf[buf_len], TRANS_VERIF_CODE_LEN);
    buf_len += TRANS_VERIF_CODE_LEN;

    /* ��3����: ������ */
    info.parse_code = buf[buf_len++];

    /* ��4����: ������ */
    info.request_code = buf[buf_len++];

    /* ��5����: ������� */
    info.pkg_id = buf[buf_len++];   // ��ָ�����ݰ���ʼ���ܰ����ڲ�����


    switch (info.parse_code)
    {
        /* ����ģ��״̬��Ϣ */
        case TRANS_CMD_ELE_STATUS_DISP:
            switch (info.request_code)
            {
                /* ��ͷ��ʼ��ȫ������ */
                case UPLOAD_DISP_ALL:
                    info.trans_code = TRANS_ELE_STAT_DISP_RSRC;
                    SETBIT_N(info.request_type, RST_ELE_STAT_DISP_ALL);
                    SETBIT_N(info.request_type, RST_ELE_STAT_DISP_RSRC);
                    SETBIT_N(info.request_type, RST_ELE_STAT_DISP_CFG);
                    break;

                /* ������ʾ��Դ�е�ĳ������ */
                case UPLOAD_ONE_DISP_RSRC:
                    info.trans_code = TRANS_ELE_STAT_DISP_RSRC;
                    SETBIT_N(info.request_type, CONT_ELE_STAT_DISP_RSRC);
                    break;

                /* ����������Ϣ�е�ĳ������ */
                case UPLOAD_ONE_DISP_CFG:
                    info.trans_code = TRANS_ELE_STAT_DISP_CFG;
                    SETBIT_N(info.request_type, CONT_ELE_STAT_DISP_CFG);
                    break;

                /* ������ʾ��Դ���������ݰ�(�����õ�״̬) */
                case UPLOAD_ALL_DISP_RSRC:
                    info.trans_code = TRANS_ELE_STAT_DISP_RSRC;
                    SETBIT_N(info.request_type, RST_ELE_STAT_DISP_RSRC);
                    break;

                /* ����������Ϣ���������ݰ� */
                case UPLOAD_ALL_DISP_CFG:
                    info.trans_code = TRANS_ELE_STAT_DISP_CFG;
                    SETBIT_N(info.request_type, RST_ELE_STAT_DISP_CFG);
                    break;
            }

            /* ������� */
            if (TYB_SLP_is_node_unique(&info))
            {
                TYB_SLP_cache_append(&info);
            }
            break;

        default:
            break;
    }
}

#endif

#ifdef TUYA_USE_ERR_HIS
/*******************************************************************************
 �� ��: TYB_SLP_trans_err_his_disp_rsrc()
 �� ��: �ְ��ϴ���ʷ������ʾ��Դ(ͨ������Э�飬������:0x0A)
 �� ��: buf--���ܵ㻺����;
        info--����������Ϣ;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TYB_SLP_trans_err_his_disp_rsrc(U08 *buf, TYB_TRANS_INFO *info)
{
    U16 i;
    U08 useless_cache[200];	/* ���û��� */
	TYBC_ERR_HIS err_record;
    U16 upload_bytes = 0;          /* �ϴ��������ֽ��� */
    U08 package_total = 1;         /* �ܰ���(����1��) */
    BOOL fg_multi_pkg = FALSE;     /* ������������־ */
    U08 buf_len = 0;

    static U16 idx_rec = 0;        /* ��¼��һ�����ݵ���ʼ�������� */

	MEMCLR(&err_record, ((sizeof(TYBC_ERR_HIS)+1)/sizeof(U16))*2);

    /* ���ݰ��ط���ʼ�������� */
    if (GETBIT_N(info->request_type, RST_ERR_HIS_DISP_RSRC))
    {
        idx_rec = 0;
        CLRBIT_N(info->request_type, RST_ERR_HIS_DISP_RSRC);
    }

    /* Ѱ�ҵ�ǰ���ݰ�����ʼ��������(��0������) */
    if (GETBIT_N(info->request_type, CONT_ERR_HIS_DISP_RSRC))
    {
        if (info->pkg_id == 0)
        {
            idx_rec = 0;
        }
    }

    /* �����ܰ�����������ʼ���� */
    for (i = 0; i < TY_HIS_ERR_MAX; i++)
    {
   		U08 msg_len = 0;
        /* ��ȡ��ʷ���� */
        TYBC_GET_ERR_HIS(&err_record,i);
        /* �й��� */
        if (err_record.err_id < ERR_TOTAL)
        {
			msg_len = TYB_SLP_GET_CODE_STR(useless_cache, err_record.name);
	        upload_bytes += DP_FIELD_HIS_ERR_INFO_BYTES + msg_len;

	        if (upload_bytes > TRANS_10_CONTENT_BYTES)
	        {
	            package_total++;  /* �ܰ����ۼ� */
	            upload_bytes = DP_FIELD_HIS_ERR_INFO_BYTES + msg_len;

	            /* Ѱ�ҵ�ǰ���ݰ�����ʼ��������(�ǵ�0������) */
	            if (GETBIT_N(info->request_type, CONT_ERR_HIS_DISP_RSRC))
	            {
	                /* �����ݸ������������ʱ����¼��ʱ����ʼ���� */
	                if (info->pkg_id == (package_total-1))
	                {
	                    idx_rec = i;
	                }
	            }
	        }
		}
    }

    /* ����ϱ��ֽ������� */
    upload_bytes = 0;

    /* ���֡ͷ */
    buf[0] = info->trigger_src;          /* ����Դ */
    buf[1] = info->verif_code[0];        /* ��֤�� */
    buf[2] = info->verif_code[1];        /* ��֤�� */
    buf[3] = info->verif_code[2];        /* ��֤�� */
    buf[4] = package_total;              /* �ܰ��� */
    buf[5] = info->pkg_id;               /* ��ǰ���� */
    buf[6] = info->trans_code;           /* ������ */

    /* ����м���Ϣ: ʵ���ϴ���ʷ�������� */
    buf[7] = TY_HIS_ERR_MAX;

    buf_len += TRANS_10_HEADER_BYTES;

    /* �������:��ʷ������ʾ��Դ(ȫ��) */
    for (i = idx_rec; i < TY_HIS_ERR_MAX; i++)
    {
        /* ��ȡ��ʷ���� */
        TYBC_GET_ERR_HIS(&err_record,i);

        /* �й��� */
        if (err_record.err_id < ERR_TOTAL)
        {
	        upload_bytes = buf_len + DP_FIELD_HIS_ERR_INFO_BYTES + TYB_SLP_GET_CODE_STR(useless_cache, err_record.name);

	        if (upload_bytes <= RAW_DATA_BYTES)
	        {
	            U08 msg_len = 0;
                buf[buf_len++] = HIBYTE(err_record.id);
                buf[buf_len++] = LOBYTE(err_record.id);
                buf[buf_len++] = err_record.time.month;
                buf[buf_len++] = err_record.time.day;
                buf[buf_len++] = err_record.time.hour;
                buf[buf_len++] = err_record.time.mint;
                msg_len = TYB_SLP_GET_CODE_STR(&buf[buf_len+2], err_record.name);
                buf[buf_len++] = HIBYTE(msg_len);
                buf[buf_len++] = LOBYTE(msg_len);
                buf_len += msg_len;
	        }
	        else
	        {
	            fg_multi_pkg = TRUE;
	            idx_rec = i;
	            break;
	        }
		}
    }

    /* ������һ�� */
    info->pkg_id++;

    /* ���в���������ʾ��Դ�ϴ���� */
    if (!fg_multi_pkg)
    {
        idx_rec = 0;                                           /* ��¼��� */
        info->pkg_id = 0;                                      /* ��ǰ��ID��λ */
        info->trans_code = TRANS_NULL;
        fg_has_trans_task = FALSE;
    }

    /* �ش����ݰ�ʱ�������������������룬����ͨ������ */
    if (GETBIT_N(info->request_type, CONT_ERR_HIS_DISP_RSRC))
    {
        fg_has_trans_task = FALSE;
        CLRBIT_N(info->request_type, CONT_ERR_HIS_DISP_RSRC);
    }
}


/*******************************************************************************
 �� ��: TYB_SLP_parse_err_his_cmd()
 �� ��: ������ʷ��������
 �� ��: buf--���ջ�����(����Դ);
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TYB_SLP_parse_err_his_cmd(U08 *buf)
{
    TYB_TRANS_INFO info;
    U08 buf_len = 0;

    /* ��ʼ�� */
    ARRCLR(info.addr);
    ARRCLR(info.request_type);

    /* ����Դ��Ϊ"��ʷ����"���򲻼����������� */
    if (buf[buf_len] != TRANS_HIS_ERR)
    {
        return;
    }

    /* ��1����:����Դ("��ʷ����"����) */
    info.trigger_src = buf[buf_len++];

    /* ��2����:��֤�� */
    MEMCPY(info.verif_code, &buf[buf_len], TRANS_VERIF_CODE_LEN);
    buf_len += TRANS_VERIF_CODE_LEN;

    /* ��3����: ������ */
    info.parse_code = buf[buf_len++];

    /* ��4����: ������ */
    info.request_code = buf[buf_len++];

    /* ��5����: ������� */
    info.pkg_id = buf[buf_len++];   // ��ָ�����ݰ���ʼ���ܰ����ڲ�����


    switch (info.parse_code)
    {
        /* ����ģ��״̬��Ϣ */
        case TRANS_CMD_HIS_ERR_DISP:
            switch (info.request_code)
            {
                /* ������ʾ��Դ���������ݰ�(������ʷ����) */
                case UPLOAD_DISP_ALL:
                    info.trans_code = TRANS_ERR_HIS_DISP_RSRC;
                    SETBIT_N(info.request_type, RST_ERR_HIS_DISP_RSRC);
                    break;

                /* ������ʾ��Դ�е�ĳ������ */
                case UPLOAD_ONE_DISP_RSRC:
                    info.trans_code = TRANS_ERR_HIS_DISP_RSRC;
                    SETBIT_N(info.request_type, CONT_ERR_HIS_DISP_RSRC);
                    break;
            }

            /* ������� */
            if (TYB_SLP_is_node_unique(&info))
            {
                TYB_SLP_cache_append(&info);
            }
            break;

        default:
            break;
    }
}

#endif

#ifdef TUYA_HAS_TRANS
/*******************************************************************************
 �� ��: TYB_SLP_concat_trans_data()
 �� ��: ͨ����������ƴ��
 �� ��: buf--�ַ���������;
        info--����������Ϣ;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TYB_SLP_concat_trans_data(U08 *buf, TYB_TRANS_INFO* info)
{
    switch (info->trans_code)
    {
        case TRANS_NULL:
            break;

/* ��"��������" */
#ifdef TUYA_USE_PARAM_SET
        /* ���Ͳ�������ʾ��Դ */
        case TRANS_PARAM_GRP_DISP_RSRC:
            TYB_SLP_trans_param_grp_disp_rsrc(&buf[0],info);
            break;

        /* ���Ͳ�����������Ϣ */
        case TRANS_PARAM_GRP_DISP_CFG:
            TYB_SLP_trans_param_grp_disp_cfg(&buf[0],info);
            break;

        /* ���Ͳ���ѡ����ʾ��Դ */
        case TRANS_PARAM_RANGE_DISP_RSRC:
            TYB_SLP_trans_param_range_disp_rsrc(&buf[0],info);
            break;

        /* ���͵�ǰ�������ʾ��Դ */
        case TRANS_PARAM_ITEM_DISP_RSRC:
            TYB_SLP_trans_param_item_disp_rsrc(&buf[0],info);
            break;

        /* ���͵�ǰ�����������Ϣ */
        case TRANS_PARAM_ITEM_DISP_CFG:
            TYB_SLP_trans_param_item_disp_cfg(&buf[0],info);
            break;
#endif

/* ��"ϵͳ״̬" */
#ifdef TUYA_USE_SYS_STAT

        /* ����ϵͳ״̬�ַ���ʾ��Դ */
        case TRANS_SYS_MSG_DISP_RSRC:
            TYB_SLP_trans_sys_msg_disp_rsrc(&buf[0],info);
            break;

        /* ����ϵͳ״̬��ʾ��Դ */
        case TRANS_SYS_STAT_DISP_RSRC:
            TYB_SLP_trans_sys_stat_disp_rsrc(&buf[0],info);
            break;

        /* ����ϵͳ״̬������Ϣ */
        case TRANS_SYS_STAT_DISP_CFG:
            TYB_SLP_trans_sys_stat_disp_cfg(&buf[0],info);
            break;

#endif

/* ��"ģ��״̬" */
#ifdef TUYA_USE_MOD_STAT
		case TRANS_MOD_MSG_DISP_RSRC:
			TYB_SLP_trans_mod_msg_disp_rsrc(&buf[0],info);
			break;

        /* ����ģ��״̬��ʾ��Դ */
        case TRANS_MOD_STAT_DISP_RSRC:
            TYB_SLP_trans_mod_stat_disp_rsrc(&buf[0],info);
            break;

        /* ����ģ��״̬������Ϣ */
        case TRANS_MOD_STAT_DISP_CFG:
            TYB_SLP_trans_mod_stat_disp_cfg(&buf[0],info);
            break;
#endif

/* ��"�õ�״̬" */
#ifdef TUYA_USE_ELE_STAT
		case TRANS_ELE_STAT_DISP_RSRC:
			TYB_SLP_trans_ele_stat_disp_rsrc(&buf[0],info);
			break;

		case TRANS_ELE_STAT_DISP_CFG:
			TYB_SLP_trans_ele_stat_disp_cfg(&buf[0],info);
			break;
#endif

/* ��"��ʷ����" */
#ifdef TUYA_USE_ERR_HIS
		/* ������ʷ������ʾ��Դ */
		case TRANS_ERR_HIS_DISP_RSRC:
		TYB_SLP_trans_err_his_disp_rsrc(&buf[0],info);
		break;
#endif
    }
}
#endif

/*******************************************************************************
 �� ��: TYB_SLP_fill_upload_data()
 �� ��:
 �� ��: tuya_data--;
        dpid_idx--;
 �� ��:
 �� ��: ��
*******************************************************************************/
static void TYB_SLP_fill_upload_data(TUYA_DATA* tuya_data, U08 dpid_idx)
{
    U08 byte_idx = 0;

    tuya_data->dpid = DP[dpid_idx].dpid;
    tuya_data->dp_type = DP[dpid_idx].dp_type;
    switch (DP[dpid_idx].dpid)
    {
        /* DPID_1 ���� */
        case DPID_SWITCH:
            tuya_data->data_buf[byte_idx++] = TYBC_SWITCH_VAL;
            break;

        /* DPID_2 �¶����� */
        case DPID_TEMP_SET:
            tuya_data->data_buf[byte_idx++] = 0;
            tuya_data->data_buf[byte_idx++] = 0;
            tuya_data->data_buf[byte_idx++] = HIBYTE(TYBC_TEMP_SET_VAL);
            tuya_data->data_buf[byte_idx++] = LOBYTE(TYBC_TEMP_SET_VAL);
            break;

        /* DPID_3 ��ǰ�¶� */
        case DPID_TEMP_CUR:
            tuya_data->data_buf[byte_idx++] = 0;
            tuya_data->data_buf[byte_idx++] = 0;
            tuya_data->data_buf[byte_idx++] = HIBYTE(TYBC_TEMP_CUR_VAL);
            tuya_data->data_buf[byte_idx++] = LOBYTE(TYBC_TEMP_CUR_VAL);
            break;

        /* DPID_4 �趨ģʽ */
        case DPID_MODE:
            tuya_data->data_buf[byte_idx++] = LOBYTE(TYBC_MODE_SET_VAL);
            break;

        /* DPID_101 �汾��Ϣ */
        case DPID_VER_INFO:
            TYB_SLP_concat_ver_info(&tuya_data->data_buf[byte_idx]);
            break;

        /* DPID_102 ϵͳ��Ϣ */
        case DPID_SYS_INFO:
            TYB_SLP_concat_sys_info(&tuya_data->data_buf[byte_idx]);
            break;

        /* DPID_103 Э��汾 */
        case DPID_PROTOCOL_VER:
            TYB_SLP_concat_protocol_ver(&tuya_data->data_buf[byte_idx]);
            break;

/* ��"ͨ�����ù���" */
#ifdef TUYA_HAS_GEN_SET
        /* DPID_104 ͨ��������Ϣ */
        case DPID_GENERAL_SET_INFO:
            TYB_SLP_concat_general_set_info(&tuya_data->data_buf[byte_idx]);
            break;
#endif

        /* DPID_106 ����һ������Ϣ */
        case DPID_ZONEA_FUNC_INFO:
            TYB_SLP_concat_zone1_func_info(&tuya_data->data_buf[byte_idx]);
            break;
        /* DPID_108 �����������Ϣ */
        case DPID_ZONEB_FUNC_INFO:
            TYB_SLP_concat_zone2_func_info(&tuya_data->data_buf[byte_idx]);
            break;
        /* DPID_110 ��ˮ������Ϣ */
        case DPID_DHW_FUNC_INFO:
            TYB_SLP_concat_DHW_func_info(&tuya_data->data_buf[byte_idx]);
            break;
#if 0
/* ��"��չ����" */
#ifdef TUYA_HAS_EXT_FUNC
        /* DPID_108 ��չ������Ϣ */
        case DPID_EXTEND_FUNC_INFO:
            TYB_SLP_concat_extend_func_info(&tuya_data->data_buf[byte_idx]);
            break;
#endif
#endif

/* ��"ϵͳ״̬" */
#if 0
        /* DPID_112~115 ϵͳ״̬��Ϣ1~4 */
        case DPID_SYS_STAT_INFO_1:
        case DPID_SYS_STAT_INFO_2:
        case DPID_SYS_STAT_INFO_3:
        case DPID_SYS_STAT_INFO_4:
            TYB_SLP_concat_sys_stat_info(&tuya_data->data_buf[byte_idx], dpid_idx);
            break;

        /* DPID_116 ϵͳ״̬���� */
        case DPID_SYS_STAT_DATA:
            TYB_SLP_concat_sys_stat_data(&tuya_data->data_buf[byte_idx]);
            break;
#endif

/* ��"��ǰ����" */
#ifdef TUYA_USE_ERR_CUR
        /* DPID_133 ��ǰ������Ϣ */
        case DPID_ERR_CUR_INFO:
            TYB_SLP_concat_err_cur_info(&tuya_data->data_buf[byte_idx]);
            break;
#endif

#if 0
/* ��"��ʷ����" */
#ifdef TUYA_USE_ERR_HIS
        /* DPID_134 ��ʷ������Ϣ */
        case DPID_ERR_HIS_INFO:
            TYB_SLP_concat_err_his_info(&tuya_data->data_buf[byte_idx]);
            break;
#endif
#endif

/* ��ͨ������(��"��������"/"ģ��״̬") */
#ifdef TUYA_HAS_TRANS
        /* DPID_118~123 ͨ������ */
        case DPID_DATA_CHANNEL_01:
        case DPID_DATA_CHANNEL_02:
        case DPID_DATA_CHANNEL_03:
        case DPID_DATA_CHANNEL_04:
        case DPID_DATA_CHANNEL_05:
        case DPID_DATA_CHANNEL_06:
            TYB_SLP_concat_trans_data(&tuya_data->data_buf[byte_idx], &tyb_trans_info);
            break;
#endif
    }
}

#ifdef TUYA_USE_HMI_MASTER
/*******************************************************************************
 �� ��: TYB_SLP_SH_upload_tuya_data()
 �� ��: ѡ���ϱ����ܵ㣬������Ӧ���ϱ����������Ŀ�껺����
 �� ��: dest_buf--Ŀ�껺����(�ϱ��ƶ˵�����);
        data_addr--Modubs���ݵ�ַ;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL TYB_SLP_SH_upload_tuya_data(U08 *dest_buf, U16 data_addr)
{
    U08 dpid_idx = 0;
    TUYA_DATA tuya_data;
    U08 *src_buf = NULL;
    U08 idx = 0;
    U08 i;

    VALCLR(tuya_data);

    /* ������ */
    if ((data_addr - ADDR_SH_TUYA_DATA_BGN) % SH_TUYA_DATA_SIZE != 0)
    {
        return FALSE;
    }

    /* �ɺ������ϴ��ĸ����ܵ� */
    dpid_idx = TYB_SLP_select_upload_dpid_idx();

    /* ��书�ܵ����� */
    TYB_fill_dp(&tuya_data, dpid_idx);

    /* ��ֵ */
    src_buf = (U08*)&tuya_data; //ע��:��С������,ͳһ��U08���ͷ��Ϳɹ��
    for (i = 0; i < TUYA_DATA_SIZE*2; i++)
    {
        dest_buf[idx++] = src_buf[i];
    }

    return TRUE;
}
#endif

//***��ʾ��ʾ���汾��ʱ***//
#if (TYBC_DISP_HMI_VER_EN == TRUE)
/*******************************************************************************
 �� ��: TYB_SLP_store_hmi_ver()
 �� ��: �洢����������ʾ���汾
 �� ��: buf--���ջ�����(����Դ);
 �� ��: �Ѷ�ȡ�ֽ���
 �� ��: ��
*******************************************************************************/
static void TYB_SLP_store_hmi_ver(U08 *buf)
{
    U08 i;

    ARRCLR(tyb_hmi_ver);

    /* �洢��ʾ���汾 */
    for (i=0; i<MAX_VER_BYTE; i++)
    {
        tyb_hmi_ver[i] = buf[i];
    }
}
#endif


/*******************************************************************************
 �� ��: TYB_SLP_parse_switch()
 �� ��: ���ػ�����
 �� ��: buf--���ջ�����(����Դ);
        func_type--��������;
 �� ��: �Ѷ�ȡ�ֽ���
 �� ��: ��
*******************************************************************************/
static U08 TYB_SLP_parse_switch(U08 *buf, U08 func_type, U08 dpid)
{
    I16 val = 0;

    /* ���ݽ��� */
    if (func_type == FUNC_STANDARD)     /* ��׼���� */
    {
        val = buf[0];
    }
    else if (func_type == FUNC_CUSTOM)  /* �Զ��幦�� */
    {
        val = MAKEWORD(buf[0], buf[1]);
    }

    /* ����� */
    if (val == RN_SWITCH_ON)
    {
    	switch (dpid)
    	{
    		case DPID_SWITCH:
				if(!fg_zone_A_run)	TYBC_CMD_RUN; 			 /* ����һ���� */
				TYB_SLP_check_data_updated(TYBC_SWITCH_VAL, SWITCH_IDX);    /* ��ֵ���¾�ֵ */
				break;
			case DPID_ZONEA_FUNC_CMD:
				if(!fg_zone_A_run)	TYBC_CMD_RUN;    	     /* ����һ���� */
				TYB_SLP_check_data_updated(TYBC_SWITCH_VAL, ZONEA_SWITCH_IDX);    /* ��ֵ���¾�ֵ */
				break;
			case DPID_ZONEB_FUNC_CMD:
				if(!fg_zone_B_run)	TYBC_ZoneB_CMD_RUN;      /* ��������� */
				TYB_SLP_check_data_updated(TYBC_ZoneB_SWITCH_VAL, ZONEB_SWITCH_IDX);    /* ��ֵ���¾�ֵ */
				break;
			case DPID_DHW_FUNC_CMD:
				if(!fg_hotw_run)	TYBC_DHW_CMD_RUN;       /* ��ˮ���� */
				TYB_SLP_check_data_updated(TYBC_DHW_SWITCH_VAL, DHW_SWITCH_IDX);    /* ��ֵ���¾�ֵ */
				break;
		}
    }
    else
    {
        switch (dpid)
    	{
    		case DPID_SWITCH:
				if(fg_zone_A_run)	TYBC_CMD_STOP; 			 /* ����һ�ػ� */
				TYB_SLP_check_data_updated(TYBC_SWITCH_VAL, SWITCH_IDX);    /* ��ֵ���¾�ֵ */
				break;
			case DPID_ZONEA_FUNC_CMD:
				if(fg_zone_A_run)	TYBC_CMD_STOP;    	     /* ����һ�ػ� */
				TYB_SLP_check_data_updated(TYBC_SWITCH_VAL, ZONEA_SWITCH_IDX);    /* ��ֵ���¾�ֵ */
				break;
			case DPID_ZONEB_FUNC_CMD:
				if(fg_zone_B_run)	TYBC_ZoneB_CMD_STOP;     /* ������ػ� */
				TYB_SLP_check_data_updated(TYBC_ZoneB_SWITCH_VAL, ZONEB_SWITCH_IDX);    /* ��ֵ���¾�ֵ */
				break;
			case DPID_DHW_FUNC_CMD:
				if(fg_hotw_run)	TYBC_DHW_CMD_STOP;      /* ��ˮ�ػ� */
				TYB_SLP_check_data_updated(TYBC_DHW_SWITCH_VAL, DHW_SWITCH_IDX);    /* ��ֵ���¾�ֵ */
				break;
        }
    }

    return DP_CMD_FIELD_LEN;
}


/*******************************************************************************
 �� ��: TYB_SLP_parse_temp_set()
 �� ��: �趨�¶Ƚ���
 �� ��: buf--���ջ�����(����Դ);
        func_type--��������;
 �� ��: �Ѷ�ȡ�ֽ���
 �� ��: ��
*******************************************************************************/
static U08 TYB_SLP_parse_temp_set(U08 *buf, U08 func_type, U08 dpid)
{
    I32 raw_val = 0;
    I16 val = 0;

    /* ���ݽ��� */
    if (func_type == FUNC_STANDARD)     /* ��׼���� */
    {
        /* �¶��趨ֵ */
        raw_val = MAKEDWORD(buf[0], buf[1], buf[2], buf[3]);
        val = (I16)raw_val;
    }
    else if (func_type == FUNC_CUSTOM)  /* �Զ��幦�� */
    {
        /* �����¶��趨ֵ�����ַ���ת��Ϊ��ֵ */
        val = MAKEWORD(buf[0], buf[1]);
    }

    TYBC_temp_set(val, dpid);

    /* ��ֵ���¾�ֵ */
	switch (dpid)
	{
		case DPID_TEMP_SET:
   		    TYB_SLP_check_data_updated(TYBC_TEMP_SET_VAL, TEMP_SET_IDX);
			break;
		case DPID_ZONEA_FUNC_CMD:
   		    TYB_SLP_check_data_updated(TYBC_TEMP_SET_VAL, ZONEA_TEMP_SET_IDX);
			break;
		case DPID_ZONEB_FUNC_CMD:
   		    TYB_SLP_check_data_updated(TYBC_ZoneB_TEMP_SET_VAL, ZONEB_TEMP_SET_IDX);
			break;
		case DPID_DHW_FUNC_CMD:
    		TYB_SLP_check_data_updated(TYBC_DHW_TEMP_SET_VAL, DHW_TEMP_SET_IDX);
			break;
	}
    return DP_CMD_FIELD_LEN;
}


/*******************************************************************************
 �� ��: TYB_SLP_parse_mode_set()
 �� ��: �趨ģʽ����
 �� ��: buf--���ջ�����(����Դ);
        func_type--��������;
 �� ��: �Ѷ�ȡ�ֽ���
 �� ��: ��
*******************************************************************************/
static U08 TYB_SLP_parse_mode_set(U08 *buf, U08 func_type)
{
    I16 val = 0;

    /* ���ݽ��� */
    if (func_type == FUNC_STANDARD)     /* ��׼���� */
    {
        val = buf[0];
    }
    else if (func_type == FUNC_CUSTOM)  /* �Զ��幦�� */
    {
        val = MAKEWORD(buf[0], buf[1]);
    }

    TYBC_mode_set(val);

    TYB_SLP_check_data_updated(TYBC_MODE_SET_VAL, ZONEA_MODE_SET_IDX);    /* ��ֵ���¾�ֵ */

    return DP_CMD_FIELD_LEN;
}

/* ��"��չ����" */
#ifdef TUYA_HAS_EXT_FUNC
/* ��"���ٹ���" */
#ifdef TUYA_USE_WIND
/*******************************************************************************
 �� ��: TYB_SLP_parse_wind_set()
 �� ��: �趨���ٽ���
 �� ��: buf--���ջ�����(����Դ);
        func_type--��������;
 �� ��: �Ѷ�ȡ�ֽ���
 �� ��: ��
*******************************************************************************/
static U08 TYB_SLP_parse_wind_set(U08 *buf, U08 func_type)
{
    I16 val = 0;

    /* ���ݽ��� */
    if (func_type == FUNC_STANDARD)     /* ��׼���� */
    {
        val = buf[0];
    }
    else if (func_type == FUNC_CUSTOM)  /* �Զ��幦�� */
    {
        val = MAKEWORD(buf[0], buf[1]);
    }

    TYBC_wind_set(val);

    TYB_SLP_check_data_updated(TYBC_WIND_SET_VAL, WIND_SET_IDX);    /* ��ֵ���¾�ֵ */

    return DP_CMD_FIELD_LEN;
}
#endif

/* ��"ʪ�ȹ���" */
#ifdef TUYA_USE_HUMI
/*******************************************************************************
 �� ��: TYB_SLP_parse_humi_set()
 �� ��: �趨ʪ�Ƚ���
 �� ��: buf--���ջ�����(����Դ);
        func_type--��������;
 �� ��: �Ѷ�ȡ�ֽ���
 �� ��: ��
*******************************************************************************/
static U08 TYB_SLP_parse_humi_set(U08 *buf, U08 func_type)
{
    I16 val = 0;

    /* ���ݽ��� */
    if (func_type == FUNC_STANDARD)     /* ��׼���� */
    {
        val = buf[0];
    }
    else if (func_type == FUNC_CUSTOM)  /* �Զ��幦�� */
    {
        val = MAKEWORD(buf[0], buf[1]);
    }

    TYBC_humi_set(val);

    TYB_SLP_check_data_updated(TYBC_HUMI_SET_VAL, HUMI_SET_IDX);    /* ��ֵ���¾�ֵ */

    return DP_CMD_FIELD_LEN;
}
#endif

/* ��"��ˮ����" */
#ifdef TUYA_USE_HOT_WATER
/*******************************************************************************
 �� ��: TYB_SLP_parse_hot_water_set()
 �� ��: ��ˮ�趨�¶Ƚ���
 �� ��: buf--���ջ�����(����Դ);
        func_type--��������;
 �� ��: �Ѷ�ȡ�ֽ���
 �� ��: ��
*******************************************************************************/
static U08 TYB_SLP_parse_hot_water_set(U08 *buf, U08 func_type)
{
    I16 val = 0;

    /* ���ݽ��� */
    if (func_type == FUNC_STANDARD)     /* ��׼���� */
    {
        val = buf[0];
    }
    else if (func_type == FUNC_CUSTOM)  /* �Զ��幦�� */
    {
        val = MAKEWORD(buf[0], buf[1]);
    }

    TYBC_hot_water_set(val);

    TYB_SLP_check_data_updated(TYBC_HOT_WATER_SET_VAL, HOT_WATER_SET_IDX);    /* ��ֵ���¾�ֵ */

    return DP_CMD_FIELD_LEN;
}
#endif
#endif

/* ��"ͨ�����ù���" */
#ifdef TUYA_HAS_GEN_SET
/*******************************************************************************
 �� ��: TYB_SLP_parse_general_set_cmd()
 �� ��: ���� "ͨ����������" ͸���͹��ܵ�
 �� ��: buf--���ջ�����(����Դ);
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TYB_SLP_parse_general_set_cmd(U08 *buf)
{
    U08 buf_len = 0;

/* ��"���Թ���" */
#ifdef TUYA_USE_LANG_SET
    /* �ֶ�0: ���� */
	if (sh_language != MAKEWORD(buf[buf_len], buf[buf_len+1]))
	{
		fg_tuya_lang_switch = TRUE;
	    switch (MAKEWORD(buf[buf_len], buf[buf_len+1]))
	    {
	        default:
	            break;

	        case RN_LANGUAGE_CN:    /* ������ʾ */
	        case RN_LANGUAGE_EN:    /* Ӣ����ʾ */
	        case RN_LANGUAGE_GR:
	        case RN_LANGUAGE_FR:
	        case RN_LANGUAGE_ITL:
	        case RN_LANGUAGE_POL:
	        case RN_LANGUAGE_SP:
	        case RN_LANGUAGE_HG:
	        case RN_LANGUAGE_GE:
	            sh_language = TYBC_language_to_local_enum(MAKEWORD(buf[buf_len], buf[buf_len+1]));
	            break;

	    }
	    DataAccess_Language(DATA_WRITE);
	}

    buf_len += DP_CMD_FIELD_LEN;
#endif

/* ��"����ҳ����" */
#ifdef TUYA_USE_SPLASH
    /* �ֶ�1: ����ҳ */
    switch (MAKEWORD(buf[buf_len], buf[buf_len+1]))
    {
        case RN_SWITCH_ON:  /* ʹ������ҳ */
            TYBC_CMD_SPLASH_ON;
            break;

        case RN_SWITCH_OFF: /* �ر�����ҳ */
            TYBC_CMD_SPLASH_OFF;
            break;
    }
    TYB_SLP_check_data_updated(TYBC_SPLASH_VAL, SPLASH_IDX);    /* ��ֵ���¾�ֵ */
    buf_len += DP_CMD_FIELD_LEN;
#endif

/* ��"ͯ������" */
#ifdef TUYA_USE_CHILDLOCK
    /* �ֶ�2: ͯ�� */
    switch (MAKEWORD(buf[buf_len], buf[buf_len+1]))
    {
        case RN_SWITCH_ON:              /* ��ͯ�� */
            TYBC_CMD_CHILDLOCK_ON;
            break;

        case RN_SWITCH_OFF:             /* �ر�ͯ�� */
            TYBC_CMD_CHILDLOCK_OFF;
            break;
    }
    TYB_SLP_check_data_updated(TYBC_CHILDLOCK_VAL, CHILDLOCK_IDX);  /* ��ֵ���¾�ֵ */
    buf_len += DP_CMD_FIELD_LEN;
#endif

	/* ����һ�����������ˮʹ�ܲ��ù� */
    buf_len += DP_CMD_FIELD_LEN;
    buf_len += DP_CMD_FIELD_LEN;
    buf_len += DP_CMD_FIELD_LEN;

/* ��"�¶ȵ�λ���ù���" */
#ifdef TUYA_USE_TEMP_UNIT_SET
    /* �ֶ�: �¶ȵ�λ */
	if (sh_temp_unit != MAKEWORD(buf[buf_len], buf[buf_len+1]))
	{
		fg_tuya_temp_unit_switch = TRUE;
	    switch (MAKEWORD(buf[buf_len], buf[buf_len+1]))
	    {
	        default:
	            break;

	        case RN_TEMP_UNIT_C:    /* ���� */
	        case RN_TEMP_UNIT_F:    /* ���� */
	            sh_temp_unit = MAKEWORD(buf[buf_len], buf[buf_len+1]);
	            break;

	    }
	    DataAccess_Temp_unit(DATA_WRITE);
	}

    buf_len += DP_CMD_FIELD_LEN;
#endif
}
#endif


/*******************************************************************************
 �� ��: TYB_SLP_parse_zone1_func_cmd()
 �� ��: �������湦������͸���͹��ܵ�
 �� ��: buf--���ջ�����(����Դ) dpid:���ܵ�;
 �� ��: ��
 �� ��: ע��! �˴��Ƚ����趨�¶ȣ��ٽ����趨ģʽ!!!
        (Ĭ��ǰ��: APP��ʾ��ģʽ��Ϊ�������ĵ�ǰģʽ��APP�ϵ��趨�¶����趨/��ǰ
        ģʽ���Ŷ�Ӧ��ϵ��)
*******************************************************************************/
static void TYB_SLP_parse_zone_func_cmd(U08 *buf, U08 dpid)
{
    U08 buf_len = 0;

    /* �ֶ�0: �������ػ� */
    buf_len += TYB_SLP_parse_switch(&buf[buf_len], FUNC_CUSTOM, dpid);

    /* �ֶ�1: �����趨�¶� */
    buf_len += TYB_SLP_parse_temp_set(&buf[buf_len], FUNC_CUSTOM, dpid);

	/* �ֶ�2: �����趨ģʽ */
	if (dpid == DPID_ZONEA_FUNC_CMD)
    	buf_len += TYB_SLP_parse_mode_set(&buf[buf_len], FUNC_CUSTOM);
}

/* ��"��չ����" */
#ifdef TUYA_HAS_EXT_FUNC
/*******************************************************************************
 �� ��: TYB_SLP_parse_extend_func_cmd()
 �� ��: ������չ��������͸���͹��ܵ�
 �� ��: buf--���ջ�����(����Դ);
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TYB_SLP_parse_extend_func_cmd(U08 *buf)
{
    U08 buf_len = 0;

/* ��"���ٹ���" */
#ifdef TUYA_USE_WIND
    /* �ֶ�0: �����趨���� */
    buf_len += TYB_SLP_parse_wind_set(&buf[buf_len], FUNC_CUSTOM);
#endif

/* ��"ʪ�ȹ���" */
#ifdef TUYA_USE_HUMI
    /* �ֶ�1: �����趨ʪ�� */
    buf_len += TYB_SLP_parse_humi_set(&buf[buf_len], FUNC_CUSTOM);
#endif

/* ��"��ˮ����" */
#ifdef TUYA_USE_HOT_WATER
    /* �ֶ�2: ������ˮ�趨�¶� */
    buf_len += TYB_SLP_parse_hot_water_set(&buf[buf_len], FUNC_CUSTOM);
#endif
}
#endif


/*******************************************************************************
 �� ��: TYB_SLP_parse_dp_data()
 �� ��: �����·����ܵ�
 �� ��: tuya_data--Ϳѻ����;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TYB_SLP_parse_dp_data(TUYA_DATA* tuya_data)
{

    /* �����汾�Ŵ洢 */
    if (tuya_data->dpid == DPID_NULL)
    {
/* ��ʾ"��ʾ���汾��" */
#if (TYBC_DISP_HMI_VER_EN == TRUE)
        TYB_SLP_store_hmi_ver(tuya_data->data_buf);
/* ����ʾ"��ʾ���汾��" */
#else
        ;//DO NOTHING
#endif
    }
    /* �ƶ��·����ܵ���� */
    else
    {
        /* �������ܵ㲢���������� */
        switch (tuya_data->dpid)
        {
            /* DPID_1 ���� */
            case DPID_SWITCH:
                TYB_SLP_parse_switch(tuya_data->data_buf, FUNC_STANDARD, DPID_SWITCH);
                SETBIT_N(fg_dpid_upload_pri_high, STD_SWITCH);
                break;

            /* DPID_2 �¶����� */
            case DPID_TEMP_SET:
                TYB_SLP_parse_temp_set(tuya_data->data_buf, FUNC_STANDARD, DPID_TEMP_SET);
                SETBIT_N(fg_dpid_upload_pri_high, STD_TEMP_SET);
                break;

            /* DPID_4 �趨ģʽ */
            case DPID_MODE:
                TYB_SLP_parse_mode_set(tuya_data->data_buf, FUNC_STANDARD);
                SETBIT_N(fg_dpid_upload_pri_high, STD_MODE);
                break;

/* ��"ͨ�����ù���" */
#ifdef TUYA_HAS_GEN_SET
            /* DPID_105 ͨ���������� */
            case DPID_GENERAL_SET_CMD:
                TYB_SLP_parse_general_set_cmd(tuya_data->data_buf);
                SETBIT_N(fg_dpid_upload_pri_high, GENERAL_SET_INFO);
                break;
#endif

            /* DPID_107 ����һ�������� */
            case DPID_ZONEA_FUNC_CMD:
                TYB_SLP_parse_zone_func_cmd(tuya_data->data_buf, DPID_ZONEA_FUNC_CMD);
                SETBIT_N(fg_dpid_upload_pri_high, ZONEA_FUNC_INFO);
                break;
            /* DPID_109 ������������� */
            case DPID_ZONEB_FUNC_CMD:
                TYB_SLP_parse_zone_func_cmd(tuya_data->data_buf, DPID_ZONEB_FUNC_CMD);
                SETBIT_N(fg_dpid_upload_pri_high, ZONEB_FUNC_INFO);
                break;
            /* DPID_111 ��ˮ�������� */
            case DPID_DHW_FUNC_CMD:
                TYB_SLP_parse_zone_func_cmd(tuya_data->data_buf, DPID_DHW_FUNC_CMD);
                SETBIT_N(fg_dpid_upload_pri_high, DHW_FUNC_INFO);
                break;

#if 0
            /* DPID_107 ���湦������ */
            case DPID_COMMON_FUNC_CMD:
                TYB_SLP_parse_common_func_cmd(tuya_data->data_buf);
                SETBIT_N(fg_dpid_upload_pri_high, COMMON_FUNC_INFO);
                break;

/* ��"��չ����" */
#ifdef TUYA_HAS_EXT_FUNC
            /* DPID_109 ��չ�������� */
            case DPID_EXTEND_FUNC_CMD:
                TYB_SLP_parse_extend_func_cmd(tuya_data->data_buf);
                SETBIT_N(fg_dpid_upload_pri_high, EXTEND_FUNC_INFO);
                break;
#endif
#endif

/* ��"ģ��״̬" */
#ifdef TUYA_USE_MOD_STAT
            /* DPID_117 ģ��״̬�������� */
            case DPID_MOD_STAT_CMD:
                TYB_SLP_parse_mod_stat_cmd(tuya_data->data_buf);
                break;
#endif

/* ��"��������" */
#ifdef TUYA_USE_PARAM_SET
            /* DPID_140 ������������*/
            case DPID_PARAM_CMD:
                TYB_SLP_parse_param_cmd(tuya_data->data_buf);
                break;
#endif

/* ��"�õ��ѯ" */
#ifdef TUYA_USE_ELE_STAT
            /* DPID_136 �õ�״̬��������*/
            case DPID_ELE_STAT_CMD:
                TYB_SLP_parse_ele_stat_cmd(tuya_data->data_buf);
                break;
#endif

#ifdef TUYA_USE_SYS_STAT
            /* DPID_137 ϵͳ״̬��������*/
            case DPID_SYS_STAT_CMD:
                TYB_SLP_parse_sys_stat_cmd(tuya_data->data_buf);
                break;
#endif

/* ��"��ʷ����" */
#ifdef TUYA_USE_ERR_HIS
            /* DPID_139 ��ʷ���ϴ�������*/
            case DPID_ERR_HIS_CMD:
                TYB_SLP_parse_err_his_cmd(tuya_data->data_buf);
                break;
#endif
            default:
                break;
        }
            /* �����Ҫ������Ӧ�Ĺ��ܵ� */
    }
}


