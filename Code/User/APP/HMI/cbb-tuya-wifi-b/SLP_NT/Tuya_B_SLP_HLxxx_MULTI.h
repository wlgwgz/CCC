/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: Tuya_B_SLP_HLxxx_MULTI.h
 �ļ�����: (ůͨHLϵ��)Ϳѻ(B)������ҵ���߼�ͷ�ļ�
 ������  : �ֺƱ�
 ��������: 2020.12.13
*******************************************************************************/

/* ������ʹ�� */
#ifdef TUYA_USE_DBG_CONSOLE
    #define TUYA_ARGS    "-dbg"
    #warning "TUYA-DEBUGģʽ!����ʱһ��Ҫ���� "TUYA_USE_DBG_CONSOLE" !!!"
#else
    #define TUYA_ARGS    ""
#endif

/* ================================================================================ */
/* �� �� �� ��                                                                      */
/* ================================================================================ */
/* �Ƿ����"״̬��ѯ" */
#if (defined(TUYA_USE_SYS_STAT) || defined(TUYA_USE_MOD_STAT))
    #define TUYA_HAS_STAT
#endif

/* �Ƿ����"ͨ������" */
#if (defined(TUYA_USE_MOD_STAT) || defined(TUYA_USE_PARAM_SET) || defined(TUYA_USE_ELE_STAT) || defined(TUYA_USE_ERR_HIS))
    #define TUYA_HAS_TRANS
#endif

/* �Ƿ����"ͨ������" */
//#if (defined(TUYA_USE_LANG_SET) || defined(TUYA_USE_SPLASH) || defined(TUYA_USE_CHILDLOCK))
    #define TUYA_HAS_GEN_SET
//#endif

/* �Ƿ����"��չ����" */
#if (defined(TUYA_USE_WIND) || defined(TUYA_USE_HUMI) || defined(TUYA_USE_HOT_WATER))
    #define TUYA_HAS_EXT_FUNC
#endif

/* ================================================================================ */
/* RN �� �� ö ��                                                                   */
/* ================================================================================ */
/* ����ö����ӳ���ϵ */
typedef struct tag_RN_ENUM_MAP
{
    U08 local_enum;                     /* ����ö���� */
    U08 rn_enum;                        /* RN���ö���� */
    U16 msg_id;                         /* �ַ����� */
} RN_ENUM_MAP;

/* ����״̬ */
#define RN_SWITCH_OFF                   (0)    /* �� */
#define RN_SWITCH_ON                    (1)    /* �� */

/* ����״̬ */
#define RN_RUN_STATUS_IDLE              (0)    /* ׼������ */
#define RN_RUN_STATUS_LAUNCH            (1)    /* �������� */
#define RN_RUN_STATUS_RUN               (2)    /* ������ */
#define RN_RUN_STATUS_STOP              (3)    /* ����ͣ�� */
#define RN_RUN_STATUS_ALERT             (4)    /* ���ϸ澯 */
#define RN_RUN_STATUS_MODE_SWITCHING    (5)    /* ģʽ�л��� */
#define RN_RUN_STATUS_ANTIFREEZE        (6)    /* ������ */
#define RN_RUN_STATUS_DEFROST           (7)    /* ��˪�� */
#define RN_RUN_STATUS_PREHEAT           (8)    /* Ԥ���� */
#define RN_RUN_STATUS_CLR_SNOW          (9)    /* ��ѩ�� */
#define RN_RUN_STATUS_MAX               (10)   /* �������ģʽ�� */

/* ģʽ */
#define RN_MODE_AUTO                    (0)    /* �Զ� */
#define RN_MODE_COOL                    (1)    /* ���� */
#define RN_MODE_HEAT                    (2)    /* ���� */
#define RN_MODE_FLOOR                   (3)    /* ��ů */
#define RN_MODE_VENT                    (4)    /* ͨ�� */
#define RN_MODE_HOTW                    (5)    /* ��ˮ */
#define RN_MODE_HEAT_FLOOR              (6)    /* ����+��ů */
#define RN_MODE_DEHUMI                  (7)    /* ��ʪ */
#define RN_MODE_COOL_HOTW               (8)	   /* ����+��ˮ */
#define RN_MODE_HEAT_HOTW               (9)	   /* ����+��ˮ */
#define RN_MODE_AUTO_HOTW               (10)   /* �Զ�+��ˮ */
#define RN_MODE_MAX                     (11)   /* ����趨ģʽ�� */

/* ���� */
#define RN_WIND_LOW                     (0)    /* �ͷ��� */
#define RN_WIND_MID                     (1)    /* �з��� */
#define RN_WIND_HIGH                    (2)    /* �߷��� */
#define RN_WIND_AUTO                    (3)    /* �Զ� */
#define RN_WIND_SET_MAX                 (4)    /* ����������� */
#define RN_WIND_NULL                    (5)    /* �޷��� */
#define RN_WIND_MAX                     (6)    /* �������� */

/* ���� */
#define RN_LANGUAGE_CN                  (0)	    /* ���� */
#define RN_LANGUAGE_EN                  (1)	    /* Ӣ�� */
#define RN_LANGUAGE_SP                  (2)	    /* �������� */
#define RN_LANGUAGE_GR                  (3)	    /* ϣ���� */
#define RN_LANGUAGE_FR                  (4)	    /* ���� */
#define RN_LANGUAGE_ITL                 (5)	    /* ������� */
#define RN_LANGUAGE_POL                 (6)	    /* ������ */
#define RN_LANGUAGE_HG                  (7)     /* �������� */
#define RN_LANGUAGE_CZ					(8)	    /* �ݿ� */
#define RN_LANGUAGE_PT					(9)	    /* ������ */
#define RN_LANGUAGE_TR					(10)	/* ������ */
#define RN_LANGUAGE_GE					(11)    /* ���� */
#define RN_LANGUAGE_NL					(12)    /* ���� */
#define RN_LANGUAGE_SE					(13)    /* ��� */
#define RN_LANGUAGE_DK					(14)    /* ���� */
#define RN_LANGUAGE_NO					(15)    /* Ų�� */
#define RN_LANGUAGE_MAX                 (16)    /* ��������� */

/* �¶ȵ�λ */
#define RN_TEMP_UNIT_C					(SH_SET_UNIT_C)
#define RN_TEMP_UNIT_F					(SH_SET_UNIT_F)
#define RN_TEMP_UNIT_MAX				(RN_TEMP_UNIT_F+1)

/* ��������[10]~[13] */
#define STEP_BIT                        (10)              /* ������ʼλ */
#define STEP_1                          (1<<STEP_BIT)     /* ����1 */
#define STEP_5                          (5<<STEP_BIT)     /* ����5 */
#define STEP_10                         (10<<STEP_BIT)    /* ����10 */

/* ��ѡ���� */
#define FUNC_EN_MORE_GEN_SET_BIT        (0) /* "����"-"ͨ������"����ʹ��λ */
#define FUNC_EN_ADVANCE_ERR_HIS_BIT     (1) /* "�߼�"-"��ʷ����"����ʹ��λ */
#define FUNC_EN_ADVANCE_PARAM_SET_BIT   (2) /* "�߼�"-"��������"����ʹ��λ */
#define FUNC_EN_MAIN_STAT_BIT           (3) /* "������"-"״̬��ѯ"����ʹ��λ */
#define FUNC_EN_ADVANCE_STAT_BIT        (4)	/* "�߼�"-"״̬��ѯ"����ʹ��λ */
#define FUNC_EN_MAIN_ELE_STAT_BIT       (5) /* "������"-"�õ��ѯ"����ʹ��λ */
#define FUNC_EN_MORE_CLO_TIME_BIT       (6) /* "����"-"�ƶ�ʱ"����ʹ��λ */

/* LOGO�б�(�������µ�LOGO����β�����ӣ�ע�Ᵽ��Ψһ��) */
#define SPLASH_LOGO_NULL                (0)	/* �հ�(������ҳ) */
#define SPLASH_LOGO_PUNP                (1)	/* ���ݰ��� */
#define SPLASH_LOGO_AMA                 (2)	/* �㽭�������� */
#define SPLASH_LOGO_VECK                (3)	/* ���ά�� */
#define SPLASH_LOGO_TCL                 (4)	/* ��ɽTCL */

/* �̻��ַ���ID ��APPΨһ��Ӧ */
//��ʼλ��
#define TY_MSGID_STA_BGN			    (1)
#define TY_MSGID_TITTLE_BGN			    (50)
#define TY_MSGID_MODE_BGN			    (200)
#define TY_MSGID_LANG_BGN			    (250)
#define TY_MSGID_TEMP_UNIT_BGN			(300)

#define	TY_MSGID_NULL					(0)  /* ���ַ� */
//����״̬
#define	TY_MSGID_STA_READY				(TY_MSGID_STA_BGN+0)  /* ׼������ */
#define	TY_MSGID_STA_ON					(TY_MSGID_STA_BGN+1)  /* �������� */
#define	TY_MSGID_STA_RUNNING				(TY_MSGID_STA_BGN+2)  /* ������*/
#define	TY_MSGID_STA_OFF				(TY_MSGID_STA_BGN+3)  /* ����ͣ�� */
#define	TY_MSGID_STA_ALERT				(TY_MSGID_STA_BGN+4)  /* ���ϸ澯 */
#define	TY_MSGID_STA_MODE_SW			(TY_MSGID_STA_BGN+5)  /* ģʽ�л��� */
#define	TY_MSGID_STA_ANTI_FREEZ			(TY_MSGID_STA_BGN+6)  /* ���ڷ��� */
#define	TY_MSGID_STA_DEFRO				(TY_MSGID_STA_BGN+7)  /* ���ڳ�˪ */
#define	TY_MSGID_STA_PRE_HEAT			(TY_MSGID_STA_BGN+8)  /* ����Ԥ�� */
#define	TY_MSGID_STA_SNOW_CLEAR			(TY_MSGID_STA_BGN+9)  /* ���ڳ�ѩ */

//����
#define	TY_MSGID_TIT_SWITCH					(TY_MSGID_TITTLE_BGN+0)  /* ���� */
#define	TY_MSGID_TIT_SET_TEMP				(TY_MSGID_TITTLE_BGN+1)  /* �趨�¶� */
#define	TY_MSGID_TIT_SET_MODE				(TY_MSGID_TITTLE_BGN+2)  /* �趨ģʽ */
#define	TY_MSGID_TIT_CUR_TEMP				(TY_MSGID_TITTLE_BGN+3)  /* ��ǰ�¶� */
#define	TY_MSGID_TIT_SET_DHW_TEMP			(TY_MSGID_TITTLE_BGN+4)  /* �趨��ˮ�¶� */
#define	TY_MSGID_TIT_CUR_DHW_TEMP			(TY_MSGID_TITTLE_BGN+5)  /* ��ǰ��ˮ�¶� */
#define	TY_MSGID_TIT_ZONE1					(TY_MSGID_TITTLE_BGN+6)  /* ����1 */
#define	TY_MSGID_TIT_ZONE2					(TY_MSGID_TITTLE_BGN+7)  /* ����2 */
#define	TY_MSGID_TIT_DHW					(TY_MSGID_TITTLE_BGN+8)  /* ������ˮ */
#define	TY_MSGID_TIT_LANG					(TY_MSGID_TITTLE_BGN+9)  /* ���� */
#define	TY_MSGID_TIT_SPLASH					(TY_MSGID_TITTLE_BGN+10)  /* ����ҳ */
#define	TY_MSGID_TIT_CHILDLOCK				(TY_MSGID_TITTLE_BGN+11)  /* ͯ�� */
#define	TY_MSGID_TIT_TEMP_UNIT				(TY_MSGID_TITTLE_BGN+12)  /* �¶ȵ�λ */


//ģʽ
#define	TY_MSGID_MODE_AUTO					(TY_MSGID_MODE_BGN+0)	/* �� �� */
#define	TY_MSGID_MODE_COOL					(TY_MSGID_MODE_BGN+1)	/* �� �� */
#define	TY_MSGID_MODE_HEAT					(TY_MSGID_MODE_BGN+2)	/* �� �� */

//����
#define	TY_MSGID_LANG_CHN					(TY_MSGID_LANG_BGN+0)	/* ���� */
#define	TY_MSGID_LANG_EN					(TY_MSGID_LANG_BGN+1)	/* English */
#define	TY_MSGID_LANG_GR					(TY_MSGID_LANG_BGN+2)
#define	TY_MSGID_LANG_FR					(TY_MSGID_LANG_BGN+3)
#define	TY_MSGID_LANG_ITL					(TY_MSGID_LANG_BGN+4)
#define	TY_MSGID_LANG_POL					(TY_MSGID_LANG_BGN+5)
#define	TY_MSGID_LANG_ESP					(TY_MSGID_LANG_BGN+6)
#define	TY_MSGID_LANG_HG					(TY_MSGID_LANG_BGN+7)
#define	TY_MSGID_LANG_GE					(TY_MSGID_LANG_BGN+8)

/* �¶ȵ�λ */
#define TY_MSGID_TEMP_UNIT_C				(TY_MSGID_TEMP_UNIT_BGN+0)
#define TY_MSGID_TEMP_UNIT_F				(TY_MSGID_TEMP_UNIT_BGN+1)

/* ================================================================================ */
/* I. �� �� �� �� Э �� �� �� �� ��                                                 */
/* ================================================================================ */
/* �����ϱ��Ĺ�������������� */
#define TY_CUR_ERR_MAX                  (10)   /* �����ϱ��ĵ�ǰ���������� */
#define TY_HIS_ERR_MAX                  (10)   /* �����ϱ�����ʷ���������� */

/* [0x01] �������ֶ� ���ݵ�Ԫ�ṹ */
typedef struct tag_DP_FIELD_BOOL
{
    U16 type;                           /* [0]:���ܽ���,[1]~[7]:����,[8]~[15]:�ֶ�����ֵ */
    U16 props;                          /* [0]:���·�,[1]~[7]:����,[8]~[15]:����ID */
    I16 value;                          /* ��ǰֵ */
    U16 tittle_msgid;                   /* �����ַ���ID */
} DP_FIELD_BOOL;
#define DP_FIELD_BOOL_SIZE              ((sizeof(DP_FIELD_BOOL)+1)/sizeof(U16))      /* 3���� */
#define DP_FIELD_BOOL_BYTES             (DP_FIELD_BOOL_SIZE*2)                       /* 6���ֽ� */

/* [0x02] ��ֵ���ֶ� ���ݵ�Ԫ�ṹ */
typedef struct tag_DP_FIELD_NUMBER
{
    U16 type;                           /* [0]:���ܽ���,[1]~[7]:����,[8]~[15]:�ֶ�����ֵ */
    U16 props;                          /* [0]:���·�,[1]~[7]:����,[8]~[15]:����ID */
    U16 attr;                           /* ����,[0]~[7]:��λ,[8]~[9]:С����,[10]~[13]:���� */
    I16 value;                          /* ��ǰֵ */
    I16 max;                            /* ���ֵ */
    I16 min;                            /* ��Сֵ */
    U16 tittle_msgid;                   /* �����ַ���ID */
} DP_FIELD_NUMBER;
#define DP_FIELD_NUMBER_SIZE            ((sizeof(DP_FIELD_NUMBER)+1)/sizeof(U16))    /* 6���� */
#define DP_FIELD_NUMBER_BYTES           (DP_FIELD_NUMBER_SIZE*2)                     /* 12���ֽ� */

/* [0x03] ö�����ֶ� ���ݵ�Ԫ�ṹ */
typedef struct tag_DP_FIELD_ENUM
{
    U16 type;                           /* [0]:���ܽ���,[1]~[7]:����,[8]~[15]:�ֶ�����ֵ */
    U16 props;                          /* [0]:���·�,[1]~[7]:����,[8]~[15]:����ID */
    I16 value;                          /* ��ǰֵ */
    U16 range_len;                      /* ö������(�ֵ�����Ŀ����) */
	U16 tittle_msgid;                   /* �����ַ���ID */
} DP_FIELD_ENUM;
#define DP_FIELD_ENUM_SIZE              ((sizeof(DP_FIELD_ENUM)+1)/sizeof(U16))      /* 4���� */
#define DP_FIELD_ENUM_BYTES             (DP_FIELD_ENUM_SIZE*2)                       /* 8���ֽ� */

/* [0x04] ֻ����ֵ���ֶ� ���ݵ�Ԫ�ṹ */
typedef struct tag_DP_FIELD_RO_NUM
{
    U16 type;                           /* [0]:���ܽ���,[1]~[7]:����,[8]~[15]:�ֶ�����ֵ */
    U16 props;                          /* [0]:���·�,[1]~[7]:����,[8]~[15]:����ID */
    U16 attr;                           /* ����,[0]~[7]:��λ,[8]~[9]:С����,[10]~[13]:���� */
    I16 value;                          /* ��ǰֵ */
    U16 tittle_msgid;                      	/* �����ַ���ID */
} DP_FIELD_RO_NUM;
#define DP_FIELD_RO_NUM_SIZE            ((sizeof(DP_FIELD_RO_NUM)+1)/sizeof(U16))    /* 4���� */
#define DP_FIELD_RO_NUM_BYTES           (DP_FIELD_RO_NUM_SIZE*2)                     /* 8���ֽ� */

/* [0x05] ֻ���ַ������ֶ� ���ݵ�Ԫ�ṹ */
typedef struct tag_DP_FIELD_RO_MSG
{
    U16 type;                           /* [0]:���ܽ���,[1]~[7]:����,[8]~[15]:�ֶ�����ֵ */
    U16 props;                          /* [0]:���·�,[1]~[7]:����,[8]~[15]:����ID */
    U16 tittle_msgid;                   /* �����ַ���ID */
    U16 content_msgid;                  /* �����ַ���ID */
} DP_FIELD_RO_MSG;
#define DP_FIELD_RO_MSG_SIZE            ((sizeof(DP_FIELD_RO_MSG)+1)/sizeof(U16))    /* 3���� */
#define DP_FIELD_RO_MSG_BYTES           (DP_FIELD_RO_MSG_SIZE*2)                     /* 6���ֽ� */

/* [0x06] ���ַ������ֶ�  */
typedef struct tag_DP_FIELD_ONE_MSG
{
    U08 type_H;                         /* �ֶ����͸��ֽ�: �ֶ�����ֵ */
    U08 type_L;                         /* �ֶ����͵��ֽ�: [0]:���ܽ���,[1]~[7]:���� */
    U08 msg_len;                        /* �����ַ������� */
} DP_FIELD_ONE_MSG;
#define DP_FIELD_ONE_MSG_HEADER_BYTES   (sizeof(DP_FIELD_ONE_MSG))                   /* ֡ͷ�����ֽ���: 3���ֽ� */

/* [0x07] ����ֵ���ֶ� */
typedef struct tag_DP_FIELD_ONE_NUM
{
    U08 type_H;                         /* �ֶ����͸��ֽ�: �ֶ�����ֵ */
    U08 type_L;                         /* �ֶ����͵��ֽ�: [0]:���ܽ���,[1]~[7]:���� */
	U08 props_H;
	U08 props_L;
    U08 value_H;                        /* ��ǰֵ���ֽ� */
    U08 value_L;                        /* ��ǰֵ���ֽ� */
} DP_FIELD_ONE_NUM;
#define DP_FIELD_ONE_NUM_BYTES          (sizeof(DP_FIELD_ONE_NUM))                   /* ���ֽ���: 4���ֽ� */

/* ϵͳ״̬�ַ��� */
typedef struct tag_DP_FIELD_SYS_MSG_DATA
{
    U16 msg_id;                           /* id */
    U16 msg_len;                          /* ���� */
} DP_FIELD_SYS_MSG_DATA;
#define DP_FIELD_SYS_MSG_DATA_SIZE     ((sizeof(DP_FIELD_SYS_MSG_DATA)+1)/sizeof(U16))   /* 2���� */
#define DP_FIELD_SYS_MSG_DATA_BYTES     (DP_FIELD_SYS_MSG_DATA_SIZE*2)                    /* 4���ֽ� */

/* ϵͳ״̬��Ϣ�ֶ� */
typedef struct tag_DP_FIELD_SYS_STAT_INFO
{
    U16 attr;                           /* ���� */
    I16 value;                          /* ��ֵ */
    U16 msg_len;                        /* ϵͳ״̬�ַ������� */
} DP_FIELD_SYS_STAT_INFO;
#define DP_FIELD_SYS_STAT_INFO_SIZE     ((sizeof(DP_FIELD_SYS_STAT_INFO)+1)/sizeof(U16))   /* 3���� */
#define DP_FIELD_SYS_STAT_INFO_BYTES    (DP_FIELD_SYS_STAT_INFO_SIZE*2)                    /* 6���ֽ� */

/* ϵͳ״̬�����ֶ� */
typedef struct tag_DP_FIELD_SYS_STAT_DATA
{
    U16 attr;                           /* ���� */
    I16 value;                          /* ��ֵ */
} DP_FIELD_SYS_STAT_DATA;
#define DP_FIELD_SYS_STAT_DATA_SIZE     ((sizeof(DP_FIELD_SYS_STAT_DATA)+1)/sizeof(U16))   /* 2���� */
#define DP_FIELD_SYS_STAT_DATA_BYTES    (DP_FIELD_SYS_STAT_DATA_SIZE*2)                    /* 4���ֽ� */

/* ��ǰ���������ֶ� */
typedef struct tag_DP_FIELD_ERR_CUR_INFO
{
    U16 id;                             /* ���(��8λΪ��ţ���8λΪ��Ԫ�ţ�Ϊ0xffʱ����ʾ���) */
    U16 err_id;                         /* ���Ϻ� */
} DP_FIELD_ERR_CUR_INFO;
#define DP_FIELD_ERR_CUR_INFO_SIZE      ((sizeof(DP_FIELD_ERR_CUR_INFO)+1)/sizeof(U16))    /* 2���� */
#define DP_FIELD_ERR_CUR_INFO_BYTES     (DP_FIELD_ERR_CUR_INFO_SIZE*2)                     /* 4���ֽ� */

/* ��ʷ���������ֶ� */
typedef struct tag_DP_FIELD_ERR_HIS_INFO
{
    U16 id;                             /* ���(��8λΪ��ţ���8λΪ��Ԫ�ţ�Ϊ0xffʱ����ʾ���) */
    U16 mm_dd;                          /* ��8λΪ�·ݣ���8λΪ���� */
    U16 hh_mm;                          /* ��8λΪСʱ����8λΪ���� */
    U16 msg_len;                        /* �����ַ������� */
} DP_FIELD_ERR_HIS_INFO;
#define DP_FIELD_HIS_ERR_INFO_SIZE      ((sizeof(DP_FIELD_ERR_HIS_INFO)+1)/sizeof(U16))    /* 2���� */
#define DP_FIELD_HIS_ERR_INFO_BYTES     (DP_FIELD_HIS_ERR_INFO_SIZE*2)                     /* 4���ֽ� */

/* ���ܴ��������ֶ��ֽ��� */
#define DP_CMD_FIELD_LEN                (2)

/* ================================================================================ */
/* II. ͨ �� �� �� Э �� �� �� �� ��                                                */
/* ================================================================================ */
/* ͨ������֡ͷ�ֽ��� */
#define TRANS_01_HEADER_BYTES           (8)    /* 0x01������֡ͷ�ֽ��� */
#define TRANS_02_HEADER_BYTES           (8)    /* 0x02������֡ͷ�ֽ��� */
#define TRANS_03_HEADER_BYTES           (9)    /* 0x03������֡ͷ�ֽ��� */
#define TRANS_04_HEADER_BYTES           (9)    /* 0x04������֡ͷ�ֽ��� */
#define TRANS_05_HEADER_BYTES           (9)    /* 0x05������֡ͷ�ֽ��� */
#define TRANS_06_HEADER_BYTES           (9)    /* 0x06������֡ͷ�ֽ��� */
#define TRANS_07_HEADER_BYTES           (9)    /* 0x07������֡ͷ�ֽ��� */
#define TRANS_08_HEADER_BYTES           (8)    /* 0x08������֡ͷ�ֽ��� */
#define TRANS_09_HEADER_BYTES           (8)    /* 0x09������֡ͷ�ֽ��� */
#define TRANS_10_HEADER_BYTES           (8)    /* 0x0A������֡ͷ�ֽ��� */
#define TRANS_11_HEADER_BYTES           (9)    /* 0x0B������֡ͷ�ֽ��� */
#define TRANS_12_HEADER_BYTES           (9)    /* 0x0C������֡ͷ�ֽ��� */
#define TRANS_13_HEADER_BYTES           (9)    /* 0x0D������֡ͷ�ֽ��� */
#define TRANS_14_HEADER_BYTES           (9)    /* 0x0E������֡ͷ�ֽ��� */

/* ͨ�����������ֽ��� */
#define TRANS_01_CONTENT_BYTES          (RAW_DATA_BYTES - TRANS_01_HEADER_BYTES)    /* 0x01�����������ֽ��� */
#define TRANS_02_CONTENT_BYTES          (RAW_DATA_BYTES - TRANS_02_HEADER_BYTES)    /* 0x02�����������ֽ��� */
#define TRANS_03_CONTENT_BYTES          (RAW_DATA_BYTES - TRANS_03_HEADER_BYTES)    /* 0x03�����������ֽ��� */
#define TRANS_04_CONTENT_BYTES          (RAW_DATA_BYTES - TRANS_04_HEADER_BYTES)    /* 0x04�����������ֽ��� */
#define TRANS_05_CONTENT_BYTES          (RAW_DATA_BYTES - TRANS_05_HEADER_BYTES)    /* 0x05�����������ֽ��� */
#define TRANS_06_CONTENT_BYTES          (RAW_DATA_BYTES - TRANS_06_HEADER_BYTES)    /* 0x06�����������ֽ��� */
#define TRANS_07_CONTENT_BYTES          (RAW_DATA_BYTES - TRANS_07_HEADER_BYTES)    /* 0x07�����������ֽ��� */
#define TRANS_08_CONTENT_BYTES          (RAW_DATA_BYTES - TRANS_08_HEADER_BYTES)    /* 0x08�����������ֽ��� */
#define TRANS_09_CONTENT_BYTES          (RAW_DATA_BYTES - TRANS_09_HEADER_BYTES)    /* 0x09�����������ֽ��� */
#define TRANS_10_CONTENT_BYTES          (RAW_DATA_BYTES - TRANS_10_HEADER_BYTES)    /* 0x0A�����������ֽ��� */
#define TRANS_11_CONTENT_BYTES          (RAW_DATA_BYTES - TRANS_11_HEADER_BYTES)    /* 0x0B�����������ֽ��� */
#define TRANS_12_CONTENT_BYTES          (RAW_DATA_BYTES - TRANS_12_HEADER_BYTES)    /* 0x0C�����������ֽ��� */
#define TRANS_13_CONTENT_BYTES          (RAW_DATA_BYTES - TRANS_13_HEADER_BYTES)    /* 0x0D�����������ֽ��� */
#define TRANS_14_CONTENT_BYTES          (RAW_DATA_BYTES - TRANS_14_HEADER_BYTES)    /* 0x0E�����������ֽ��� */

/* ͨ���������� */
#define TRANS_CONTENT_NULL              (0)    /* ��Ч */
#define TRANS_PARAM                     (1)    /* �������� */
#define TRANS_MOD_STAT                  (2)    /* ģ��״̬ */
#define TRANS_ELE_STAT                  (3)    /* ���״̬ */
#define TRANS_HIS_ERR                   (4)    /* ��ʷ���� */
#define TRANS_MAX                       (5)    /* ���������������� */

/* ���������� */
#define TRANS_NULL                      (0x00)    /* ��Ч������ */
#define TRANS_PARAM_GRP_DISP_RSRC       (0x01)    /* ���Ͳ�������ʾ��Դ */
#define TRANS_PARAM_GRP_DISP_CFG        (0x02)    /* ���Ͳ�����������Ϣ */
#define TRANS_PARAM_RANGE_DISP_RSRC     (0x03)    /* ���Ͳ���ѡ����ʾ��Դ */
#define TRANS_PARAM_ITEM_DISP_RSRC      (0x04)    /* ���͵�ǰ�������ʾ��Դ */
#define TRANS_PARAM_ITEM_DISP_CFG       (0x05)    /* ���͵�ǰ�����������Ϣ */
#define TRANS_MOD_STAT_DISP_RSRC        (0x06)    /* ����ģ��״̬��ʾ��Դ */
#define TRANS_MOD_STAT_DISP_CFG         (0x07)    /* ����ģ��״̬������Ϣ */
#define TRANS_ELE_STAT_DISP_RSRC        (0x08)    /* �����õ�״̬��ʾ��Դ */
#define TRANS_ELE_STAT_DISP_CFG         (0x09)    /* �����õ�״̬������Ϣ */
#define TRANS_ERR_HIS_DISP_RSRC         (0x0A)    /* ������ʷ������ʾ��Դ */
#define TRANS_SYS_STAT_DISP_RSRC        (0x0B)    /* ����ϵͳ״̬��ʾ��Դ */
#define TRANS_SYS_STAT_DISP_CFG         (0x0C)    /* ����ϵͳ״̬������Ϣ */
#define TRANS_SYS_MSG_DISP_RSRC         (0x0D)    /* ����ϵͳ״̬�ַ�����ʾ��Դ */
#define TRANS_MOD_MSG_DISP_RSRC         (0x0E)    /* ����ģ��״̬�ַ�����ʾ��Դ */

/* ������������(������) */
#define TRANS_CMD_NULL                  (0x00)    /* ��Ч�������� */
#define TRANS_CMD_PARAM_GRP_DISP        (0x01)    /* ������������� */
#define TRANS_CMD_PARAM_RANGE_DISP      (0x02)    /* �������ѡ������ */
#define TRANS_CMD_PARAM_ITEM_DISP       (0x03)    /* ����ǰ��������� */
#define TRANS_CMD_PARAM_VAL_SET         (0x04)    /* �趨�������� */
#define TRANS_CMD_MOD_STATUS_DISP       (0x05)    /* ����ģ��״̬���� */
#define TRANS_CMD_ELE_STATUS_DISP       (0x06)    /* �����õ�״̬���� */
#define TRANS_CMD_HIS_ERR_DISP          (0x07)    /* ������ʷ�������� */
#define TRANS_CMD_SYS_STATUS_DISP       (0x08)    /* ����ϵͳ״̬���� */

/* �������� */
#define UPLOAD_DISP_ALL                 (0x00)    /* ������ʾ��Դ&������Ϣ */
#define UPLOAD_ONE_DISP_RSRC            (0x01)    /* ������ʾ��Դ�е�ĳ������ */
#define UPLOAD_ONE_DISP_CFG             (0x02)    /* ����������Ϣ�е�ĳ������ */
#define UPLOAD_ALL_DISP_RSRC            (0x03)    /* ������ʾ��Դ */
#define UPLOAD_ALL_DISP_CFG             (0x04)    /* ����������Ϣ */

/* ͨ�������������� */
#define RST_GRP_DISP_ALL                (0)       /* �ϴ�����������ʾ��Դ&������Ϣ */
#define RST_GRP_DISP_RSRC               (1)       /* ֻ�ϴ�����������ʾ��Դ */
#define RST_GRP_DISP_CFG                (2)       /* ֻ�ϴ���������������Ϣ */
#define CONT_GRP_DISP_RSRC              (3)       /* ָ���ش�����������ʾ��Դ�е�ĳ������ */
#define CONT_GRP_DISP_CFG               (4)       /* ָ���ش���������������Ϣ�е�ĳ������ */
#define RST_RANGE_DISP_RSRC             (5)       /* �ϴ����в���ѡ����ʾ��Դ */
#define CONT_RANGE_DISP_RSRC            (6)       /* ָ���ش����в���ѡ����ʾ��Դ�е�ĳ������ */
#define RST_ITEM_DISP_ALL               (7)       /* �ϴ���ǰ�������ʾ��Դ&������Ϣ */
#define RST_ITEM_DISP_RSRC              (8)       /* ֻ�ϴ���ǰ���������ʾ��Դ */
#define RST_ITEM_DISP_CFG               (9)       /* ֻ�ϴ���ǰ�������������Ϣ */
#define CONT_ITEM_DISP_RSRC             (10)      /* ָ���ش���ǰ���������ʾ��Դ�е�ĳ������ */
#define CONT_ITEM_DISP_CFG              (11)      /* ָ���ش���ǰ�������������Ϣ�е�ĳ������ */
#define RST_MOD_STAT_DISP_ALL           (12)      /* �ϴ�ģ��״̬��ʾ��Դ&������Ϣ */
#define RST_MOD_STAT_DISP_RSRC          (13)      /* ֻ�ϴ�ģ��״̬��ʾ��Դ */
#define RST_MOD_STAT_DISP_CFG           (14)      /* ֻ�ϴ�ģ��״̬������Ϣ */
#define CONT_MOD_STAT_DISP_RSRC         (15)      /* ָ���ش�ģ��״̬��ʾ��Դ�е�ĳ������ */
#define CONT_MOD_STAT_DISP_CFG          (16)      /* ָ���ش�ģ��״̬������Ϣ�е�ĳ������ */
#define RST_ELE_STAT_DISP_ALL           (17)      /* �ϴ��õ�״̬��ʾ��Դ&������Ϣ */
#define RST_ELE_STAT_DISP_RSRC          (18)      /* ֻ�ϴ��õ�״̬��ʾ��Դ */
#define RST_ELE_STAT_DISP_CFG           (19)      /* ֻ�ϴ��õ�״̬������Ϣ */
#define CONT_ELE_STAT_DISP_RSRC         (20)      /* ָ���ش��õ�״̬��ʾ��Դ�е�ĳ������ */
#define CONT_ELE_STAT_DISP_CFG          (21)      /* ָ���ش��õ�״̬������Ϣ�е�ĳ������ */
#define RST_ERR_HIS_DISP_RSRC           (22)      /* ֻ�ϴ���ʷ������ʾ��Դ */
#define CONT_ERR_HIS_DISP_RSRC          (23)      /* ָ���ش���ʷ������ʾ��Դ�е�ĳ������ */
#define RST_SYS_STAT_DISP_ALL           (24)      /* �ϴ�ϵͳ״̬��ʾ��Դ&������Ϣ */
#define RST_SYS_MSG_DISP_RSRC           (25)      /* ֻ�ϴ�ϵͳ״̬�ַ�����ʾ��Դ */
#define RST_SYS_STAT_DISP_RSRC          (26)      /* ֻ�ϴ�ϵͳ״̬��ʾ��Դ */
#define RST_SYS_STAT_DISP_CFG           (27)      /* ֻ�ϴ�ϵͳ״̬������Ϣ */
#define CONT_SYS_MSG_DISP_RSRC          (28)      /* ָ���ش�ϵͳ״̬�ַ�����ʾ��Դ�е�ĳ������ */
#define CONT_SYS_STAT_DISP_RSRC         (29)      /* ָ���ش�ϵͳ״̬��ʾ��Դ�е�ĳ������ */
#define CONT_SYS_STAT_DISP_CFG          (30)      /* ָ���ش�ϵͳ״̬������Ϣ�е�ĳ������ */
#define RST_MOD_MSG_DISP_RSRC           (31)      /* ֻ�ϴ�ģ��״̬��ʾ��Դ */
#define CONT_MOD_MSG_DISP_RSRC          (32)      /* ָ���ش�ģ��״̬��ʾ��Դ�е�ĳ������ */
#define TRANS_REQUEST_MAX               (33)      /* ���ͨ���������������� */

/* �õ�״̬���� */
#define CHART_BGN_BIT                   (2)	/* ����ͳ�Ʊ�        ��ʼλ */
#define ELE_DECIMAL_BGN_BIT             (8) /* С��λ��      ��ʼλ */
#define SH_UNIT_kWH                     (0) /* 0 �õ���(kWH) */

/* ͨ������������Ϣ */
#define TRANS_VERIF_CODE_LEN            (3)       /* ��֤���ֽ��� */
#define TRANS_ADDR_INFO_MAX             (2)       /* ��ַ��Ϣ���� */
typedef struct tag_TRANS_INFO
{
    U08 trigger_src;                                       /* ����Դ */
    U08 verif_code[TRANS_VERIF_CODE_LEN];                  /* ��֤�� */
    U08 parse_code;                                        /* ������ */
    U08 request_code;                                      /* ������ */
    U08 trans_code;                                        /* ������ */
    U08 pkg_id;                                            /* ��ʼ���ݰ��� */
    U08 addr[TRANS_ADDR_INFO_MAX];                         /* ��ַ��Ϣ */
    U08 request_type[_DIVUCARRY(TRANS_REQUEST_MAX, 8)];    /* ͨ�������������� */
} TYB_TRANS_INFO;

/* ������Ϣ */
typedef struct {
    U08 head;    /* ͷ�α� */
    U08 end;     /* β�α� */
    U08 cnt;     /* �ڵ���� */
} TYB_TRANS_QUEUE;

/* ͨ������������� */
#define TYB_TRANS_NODE_MAX                    (5)  /* ���нڵ������� */
typedef struct {
    TYB_TRANS_QUEUE queue;                         /* ������Ϣ */
    TYB_TRANS_INFO node[TYB_TRANS_NODE_MAX];           /* �ڵ���Ϣ */
} TYB_TRANS_CACHE;

/* ��ʾ��Դ ���ݵ�Ԫ�ṹ */
typedef struct tag_DP_FIELD_DISP_RSRC
{
    U16 msg_id;     /* �ַ���ID */
    U16 msg_len;    /* �ַ������� */
} DP_FIELD_DISP_RSRC;
#define DP_FIELD_DISP_RSRC_SIZE               ((sizeof(DP_FIELD_DISP_RSRC)+1)/sizeof(U16))              /* 2���� */
#define DP_FIELD_DISP_RSRC_BYTES              (DP_FIELD_DISP_RSRC_SIZE*2)                               /* 4���ֽ� */

/* ������������Ϣ ���ݵ�Ԫ�ṹ */
typedef struct tag_DP_FIELD_PARAM_GRP_DISP_CFG
{
    U16 msg_id;    /* �ַ���ID */
    U16 attr;      /* ����������(��SmartHmiһ��,[1]~[4]:Ȩ��,[12]:����) */
} DP_FIELD_PARAM_GRP_DISP_CFG;
#define DP_FIELD_PARAM_GRP_DISP_CFG_SIZE      ((sizeof(DP_FIELD_PARAM_GRP_DISP_CFG)+1)/sizeof(U16))     /* 2���� */
#define DP_FIELD_PARAM_GRP_DISP_CFG_BYTES     (DP_FIELD_PARAM_GRP_DISP_CFG_SIZE*2)                      /* 4���ֽ� */

/* ��ǰ�����������Ϣ ���ݵ�Ԫ�ṹ */
typedef struct tag_DP_FIELD_PARAM_ITEM_DISP_CFG
{
    U16 msg_id;     /* �ַ���ID */
    U16 attr;       /* ��������(��SmartHmiһ��,[0]~[7]:��λ,[8]~[9]:С��λ,[10]:����,[11]:�����޸�,[12]~[14]:Ȩ��,[15]:λ����) */
    I16 value;      /* ��ǰֵ */
    I16 max;        /* ���ֵ */
    I16 min;        /* ��Сֵ */
} DP_FIELD_PARAM_ITEM_DISP_CFG;
#define DP_FIELD_PARAM_ITEM_DISP_CFG_SIZE     ((sizeof(DP_FIELD_PARAM_ITEM_DISP_CFG)+1)/sizeof(U16))    /* 5���� */
#define DP_FIELD_PARAM_ITEM_DISP_CFG_BYTES    (DP_FIELD_PARAM_ITEM_DISP_CFG_SIZE*2)                     /* 10���ֽ� */

/* ģ��״̬������Ϣ ���ݵ�Ԫ�ṹ */
typedef struct tag_TY_MOD_STAT_DISP_CFG
{
    U16 msg_id;    /* �ַ���ID */
    U16 id;        /* ���(��8λΪģ��ţ���8λΪѹ���ţ�Ϊ0xffʱ����ʾ���) */
    U16 attr;      /* ģ��������(��SmartHmiһ��,[0]~[7]:��λ,[8]~[9]:С��λ,[10]:����,[11]:�����޸�,[12]~[14]:Ȩ��,[15]:λ����) */
    U16 value;     /* ��ǰֵ */
} DP_FIELD_MOD_STAT_DISP_CFG;
#define DP_FIELD_MOD_STAT_DISP_CFG_SIZE       ((sizeof(DP_FIELD_MOD_STAT_DISP_CFG)+1)/sizeof(U16))      /* 4���� */
#define DP_FIELD_MOD_STAT_DISP_CFG_BYTES      (DP_FIELD_MOD_STAT_DISP_CFG_SIZE*2)                       /* 8���ֽ� */

/* �õ�״̬������Ϣ ���ݵ�Ԫ�ṹ */
typedef struct tag_TY_ELE_STAT_DISP_CFG
{
    U16 msg_id;    /* �ַ���ID */
    U16 mode_id;
    U16 attr;      /* �õ������� */
    U16 value;     /* ��ǰֵ */
} DP_FIELD_ELE_STAT_DISP_CFG;
#define DP_FIELD_ELE_STAT_DISP_CFG_SIZE       ((sizeof(DP_FIELD_ELE_STAT_DISP_CFG)+1)/sizeof(U16))      /* 4���� */
#define DP_FIELD_ELE_STAT_DISP_CFG_BYTES      (DP_FIELD_ELE_STAT_DISP_CFG_SIZE*2)                       /* 8���ֽ� */

#define MODE_HOT_ELE    0
#define MODE_HEAT_ELE   1
#define MODE_COOL_ELE   2
#define MAX_MODE_ELE    3

#define MAX_WEEK_VAL    7
#define MAX_YEAR_VAL    12

extern I16 week_ele_val[MAX_MODE_ELE][MAX_WEEK_VAL];
extern I16 year_ele_val[MAX_MODE_ELE][MAX_YEAR_VAL];
extern I16 week_cache[MAX_WEEK_VAL];
extern I16 year_cache[MAX_YEAR_VAL];

/* ================================================================================ */
/* �� �� �� �� ( ů ͨ H L ϵ �� ҵ �� �� �� )                                      */
/* ================================================================================ */
/* SmartHMI���� */
TUYA_B_SLP_EXT BOOL TYB_SLP_SH_upload_tuya_data(U08 *dest_buf, U16 data_addr);  /* ���ܵ��ϱ� */

/* HMI��SegHMI���� */
TUYA_B_SLP_EXT U08 TYB_SLP_select_upload_dpid_idx(void);                        /* ѡ����Ҫ�ϱ��Ĺ��ܵ� */
TUYA_B_SLP_EXT void TYB_create_one_num_field(U08 *buf, U08 *acc_len, I16 value, U08 func_attr, U08 func_id);

