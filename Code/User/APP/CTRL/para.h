#ifndef PARA_H
#define PARA_H   1

#ifdef  PARA_GLOBALS
#define PARA_EXT
#else
#define PARA_EXT extern
#endif

//---------------------------------------------------------------------------------
// #define USE_TRAS_F_TEMPERATURE       1           /* ���͵��ǻ����¶�ֵ */

typedef struct tagPARA
{
    I16  min;       /* ��Сֵ */
    I16  init;      /* ��ʼֵ */
    I16  max;       /* ���ֵ */
    U16  sign;      /* ���� */
} PARAM;

typedef struct tagPARAMITEM
{
     U08  items;        // ��������
     PARAM *array;      // �������Ե�ַ
     I16  *data_c;      // �����ڴ�����ַ(����)
     I16  *data_f;      // �����ڴ�����ַ(����)

     U08  chip;         // �ο�����chip�������

     U08  head;         // �����޸ı����ʼ��
     U08  end;          // �����޸ı����ֹ��

     U16  sign;         // �����������/
} PARAMITEM;

// chip�������
#define PARA_NEED_STORE     0x01    // (bit0)����������Ҫ����
#define PARA_STORE_PREP     0x08    // (bit3)�������黹δ��ʼ����
#define PARA_NEED_BRO       0x80    // (bit7)����������Ҫ�㲥

//---------------------------------------------------------------------------------
/* �������Ե�16λ���壺*/
/* ��0~1λ: С���� */
#define D_DOT1      0x0001  /* 1λС���� */
#define D_DOT2      0x0002  /* 2λС���� */
#define D_DOT3      0x0003  /* 3λС���� */
#define D_DOTMASK   0x0003  /* ������ */
/* ��2~3λ: */
/* ��4~7λ: ��λ */
#define UINT_0      (0<<4)      /* �� */
#define UINT_C      (1<<4)      /* �¶�() */
#define UINT_A      (2<<4)      /* ����(A) */
#define UINT_PR     (3<<4)      /* ѹ��(bar) */
#define UINT_PE     (4<<4)      /* �ٷֱ�(%) */
#define UINT_F      (5<<4)      /* Ƶ��(HZ) */
#define UINT_RS     (6<<4)      /* ת��(rps) */
#define UINT_RM     (7<<4)      /* ת��(rpm) */
#define UINT_S      (8<<4)      /* �� */
#define UINT_M      (9<<4)      /* �� */
#define UINT_H      (10<<4)     /* ʱ */
#define UINT_D      (11<<4)     /* �� */
#define UINT_ST     (12<<4)     /* �� */
#define UINT_V      (13<<4)     /* ��ѹ(V) */
#define UINT_LH     (14<<4)     /* ˮ����(L/H) */
#define UINT_MASK   (0x000f<<4) /* ������ */
/* ��8λ: */
#define D_BIT       0x0100      /* λ���� */
/* ��9λ: */
#define D_DE        0x0200      /* �²� */
/* ��10λ: */
#define D_HIDE      0x0400      /* �������� */
/* ��11λ: */
#define D_NEXCH     0x0800      /* �����޸ĸ��� */
/* ��12~14λ: Ȩ�޼���(�ɵ͵���) */
#define D_USER      (1<<12)     /* �û����� */
#define D_PRO       (2<<12)     /* �������� */
#define D_SER       (3<<12)     /* ά�������� */
#define D_FAC       (4<<12)     /* �������� */
#define D_PUNP      (5<<12)     /* �������� */
#define D_SAFEMASK  (0x0007<<12)    /* ������ */
/* ��15λ:��Ҫʱ��ɾ����λ���� */
#define D_RNEXCH     0x8000     /* �����в����޸ĸ��� */

/* ͨ������ */
#ifdef CLIENT_TY
#define TY_HIDE   (D_HIDE)
#else
#define TY_HIDE   (0)
#endif

#ifdef USE_DIS_NEXCH
    #ifdef D_NEXCH
        #undef D_NEXCH
        #define D_NEXCH 0
    #endif
#endif

//---------------------------------------------------------------------------------
typedef struct tagEEDATA
{
    U16  Addr;      // ������ʼ��ַ
    U16  DataNum;   // �����ֽڸ��������ڲ����飬����ÿ�����Ԥ���ĸ�����
} EEDATA;           // EEPROM�洢���ݵĽṹ



#define DATA_READ   0   // ��
#define DATA_WRITE  1   // д
#define DATA_INIT   2   // ��ʼ��
#define DATA_RESET  3   // ��λ�ڴ����ݣ���д�룩


// 0x0400��ǰ�����ݲ���ͨ�����ղ��Գ�ʼ��
// 0x0400~0x0FFF������ ֻ��ͨ�����ղ��Գ�ʼ���������ɳ��ҳ�ʼ��
#define MEM_PASSWORD_ADDR       0x0400      // ����洢�׵�ַ(Ԥ��128�ֽڣ��洢��ʽ����1��(8�ֽ�����+2�ֽ�CRC),��2������...�����ɴ��12��)
#define MEM_ID_CODE_ADDR        0x0480      // ��Ų�Ʒʶ����(Ԥ��16�ֽڣ��洢��ʽ����Ʒʶ����+2�ֽ�CRC)

//#define MEM_           0x0490~0x049F      // Ԥ����ų��������Ϣ(Ԥ��16�ֽڣ��洢��ʽ�����������Ϣ+2�ֽ�CRC)
                                            // ���������CDTU�ĳ���ʶ����(2�ֽ�)
//#define MEM_           0x04A0~0x069F      // Ԥ����Ű��ռ�¼��Ϣ(Ԥ��512�ֽ�)
                                            // ���������ͣ����¼�洢(48�ֽ�)

//#define MEM_           0x06A0~0x0FFF      // Ԥ����ÿ�η����¿ռ���������£�

// 0x1000~0x107F С�������ȴ��ڴ�����
#define MEM_LANGUAGE_ADDR       0x1000      // �������ô洢�׵�ַ(Ԥ��4�ֽڣ��洢��ʽ��1�ֽ���������+2�ֽ�CRC)
#define MEM_AUTO_RUN_ADDR       0x1004      // ����״̬�洢�׵�ַ(Ԥ��4�ֽڣ��洢��ʽ��1�ֽ�����״̬+2�ֽ�CRC)
//#define MEM_POWER_TIME_ADDR       0x1008      // �ϵ�ʱ��洢�׵�ַ(Ԥ��4�ֽڣ��洢��ʽ��2�ֽ��ϵ�Сʱ+2�ֽ�CRC)
#define MEM_TEMP_UNIT_ADDR		0x1008

//#define MEM_           0x100C~0x107F      // Ԥ����ÿ�η����¿ռ���������£�


#define MEM_MACHINE_CODE_ADDR   0x1080      // �������洢�׵�ַ(Ԥ��32�ֽڣ��洢��ʽ���������+2�ֽ�CRC)
#define MEM_RUNTIME_ADDR        0x10A0      // ��ģ��ѹ���ۼ�����ʱ��Ĵ洢�׵�ַ(Ԥ��32�ֽ�(Ŀǰֻ�õ���)���洢��ʽ��ѹ������ʱ������+2�ֽ�CRC�����ɷ�4ѹ��+1ˮ��+4������ۼ�ʱ������)
#define MEM_SYSTIME_ADDR        0x10C0      // ϵͳ�ۼ�����ʱ��Ĵ洢�׵�ַ(Ԥ��8�ֽ�(Ŀǰֻ�õ���)���洢��ʽ��4�ֽ�ϵͳ����ʱ������+2�ֽ�CRC)
//#define MEM_           0x10C8~0x10CF      // Ԥ����ÿ�η����¿ռ���������£�


#define MEM_DM603_TIME_ADDR     0x10D0      // ��ʱ�洢�׵�ַ(Ԥ��64�ֽڣ��洢��ʽ����ʱ����+2�ֽ�CRC)
                                            // һ�ζ�ʱ     ->  3�ֽ�(B0.b6:һ�ζ�ʱ����/ʹ��,B0.b7:ͣ��/������B1:ʱ��B2:��)
                                            //  N�鶨ʱ     ->  3�ֽ�(B0:�ܡ�B1:ʱ��B2:��)
                                            // һ��          =  3+3*6�� = 21�ֽ� + 2�ֽ�CRC

#define MEM_HIS_ERROR_ADDR      0x1110      // ��ʷ���ϴ洢�׵�ַ(Ԥ��96�ֽڣ��洢��ʽ��80�ֽڹ�������+2�ֽ�CRC)
                                            // ��ʷ����( MAX_HIS_ERROR ��)
                                            // ���Ϻ�       -> 1 �ֽ�
                                            // ģ���       -> 1 �ֽ�
                                            // ���Ӻ�      -> 1 �ֽ� ������������;������ϵͳ�����б�Ƶ���ϵĸ�������Ϻš�������0��
                                            // �� ��        -> 5 �ֽ� ������ʱ����������3�ֽڣ�
                                            // ��ʱ��������ݣ�(3+2)*MAX_HIS_ERROR �� 50 �ֽڹ�������+2�ֽ�CRC
                                            // ��ʱ��������ݣ�(3+5)*MAX_HIS_ERROR �� 80 �ֽڹ�������+2�ֽ�CRC

#define MEM_PWD_TIME_ADDR       0x1170      /* ����ǰʱ��(��ռ14�ֽ�) */
#define MEM_INVT_EXP_INFO_ADDR  0x1180      /* ��Ƶ�쳣��Ϣ:��ռ544(0x0220)�ֽ�(����CRC) */
#define MEM_SKIP_PREHEAT_ADDR   0x13A0
#define MEM_HOLIDAY_ADDR        0x13A4      // 9�ֽ��ݼ�ģʽ����+2CRC
#define MEM_POWER_USAGE_WEEK_ADDR    0x13B0     /* ���һ���õ�����Ϣ(88�ֽ�����+2�ֽ�CRC) */
#define MEM_POWER_USAGE_YEAR_ADDR    0x1410	    /* ���һ���õ�����Ϣ(148�ֽ�����+2�ֽ�CRC) */
//#define MEM_           0x14B0~0x2FFF      // Ԥ����ÿ�η����¿ռ���������£�

// 0x3000~0x3FFF���������
#define MEM_LIMIT_ADDR          0x3000      // ϵͳά���洢�׵�ַ(Ԥ��16�ֽڣ��洢��ʽ��4�ֽ�ʹ������+2�ֽ�CRC)
//#define MEM_MODBUS_ADDR           0x3010      // ModbusͨѶ�����׵�ַ(Ԥ��16�ֽڣ��洢��ʽ������+2�ֽ�CRC)
//#define MEM_           0x3020~0x30FF      // Ԥ����ÿ�η����¿ռ���������£�
#define MEM_PARA_ADDR           0x3100      // �����洢�׵�ַ(Ԥ��0x3100~0x3FFF���洢��ʽ����1��(����+2�ֽ�CRC)����2��...)


#define PARA_WR_MAX         4       /* ÿ��ֻ��д4������ */
PARA_EXT U08 PARA_GROUP_NUM;        /* �������� */

/* ���������С����  */
// ÿ�����ռ�ÿռ�(������+CRC)��   (GROUP_SIZE*2)+2    (���2�ֽڷ�CRC)
// ÿ��CRC���λ�ã�                (GROUP_SIZE*2)
#define GROUP_MAX   15
#define GROUP_SIZE  48      // ÿ��Ԥ�������������֣�
#define CRC_SIZE    1       // CRCռ1���֣�2�ֽڣ�

#define GROUP_USER                   (0)	/* �û����� */
#define GROUP_SYS                    (1)	/* ϵͳ���� */
#define GROUP_COMP                   (2)	/* ѹ������ */
#define GROUP_FAN                    (3)	/* ������� */
#define GROUP_EEV                    (4)	/* �������� */
#define GROUP_EEV_M                  (5)	/* �������� */
#define GROUP_EEV_S                  (6)	/* �������� */
#define GROUP_DEFROST                (7)	/* ��˪���� */
#define GROUP_ATFZ_FCU               (8)	/* ���������� */
#define GROUP_PROTECT                (9)	/* �������� */
#define GROUP_INPUT                  (10)                              // ����������-���������
#define GROUP_DEBUG                  (11)	/* ���Լ����� */
#define GROUP_ENERGY                 (12)
#define GROUP_PRO                    (13)
#define GROUP_WF                     (14)	/* ����Ƶ�ʱ� */

/* ��ģ��������� */
PARA_EXT I16  para_C[GROUP_MAX][GROUP_SIZE];        /* ���϶�: ���һ��ΪCRCУ��ֵ */
PARA_EXT I16  para_F[GROUP_MAX][GROUP_SIZE];        /* ���϶�: ���һ��ΪCRCУ��ֵ */

/* ��ģ��������� */
PARA_EXT I16  s_para_C[GROUP_MAX][GROUP_SIZE];

// ����������ʽ��
#define P_g_i(f, g, i)                (f ? g : i)                         // ��Ż����
#define P_grp(name)                   P_##name(TRUE)                      // ���
#define P_idx(name)                   P_##name(FALSE)                     // ���
#define P_val(name)                   (para_C[P_grp(##name)][P_idx(##name)])  // ����ֵ
#define P_ptr(name)                  (&para_C[P_grp(##name)][P_idx(##name)])  // ����ָ��
#define P_isP(g, i, name)             (((g)==(P_grp(##name))) && ((i)==(P_idx(##name))))
#define P_GaI(name)                    P_grp(##name),P_idx(##name)

// ϵͳά��------------------------------------------------------------------------------------------------------------

PARA_EXT const PARAM    para_item_90[];

#define MAX_LMT     2
PARA_EXT I16  limit_para_C[MAX_LMT];
PARA_EXT I16  limit_para_F[MAX_LMT];                    // ����(������ʽ����һ��)

// ϵͳά��ģʽ limit_type
#define STOP_UNIT       0   // ǿ�ƹػ�
#define KEEP_RUN        1   // ��������


#define limit_value             (limit_para_C[0])       // ϵͳά������
#define limit_type              (limit_para_C[1])       // ϵͳά��ģʽ��0ǿ�ƹػ���1��������

PARA_EXT U16 limit_val_bak;         /* ʹ����������ֵ���� */

//----------------------------------------------------------------------------------------------------------------------

// �������� val_air_type
#define HEAT_ONLY       0//��ů
#define REFR_HEAT       1//��ů
#define COOL_ONLY       2//����
#define MAC_TP_MAX      3
//#define HEAT_ONLY       0//��ů
//#define REFR_HEAT       1//��ů
//#define COOL_ONLY       2//����
//#define HOT_WATER_ONLY  3//��ˮ
//#define REFR_HEAT_HW    4//��ů+��ˮ
//#define MAC_TP_MAX      5


/* !!ע������λ������������ԭ�򣬷���Ӱ�첻ͬ����汾������ӻ����ж�
       1������λ�ò��ܱ䶯
       2����������ֻ���ں�����룬�������м����
       3����������ʱ��ע�����err_mod_mac_set()����ƥ�����
*/
#if (defined(M_DEBUG))
typedef enum MAC_MODEL
{
    MAC_NONE = 0,
    MAC_MULTI_MODEL_1,        /* ��ģ��ʹ�ñ�Ƶ�� */
    MAC_MULTI_MODEL_2,        /* ��ģ�鲻ʹ�ñ�Ƶ�� */
    MAC_MAX                   /* ����������MAC_MAX֮ǰ���� */
}MAC_MODEL_ENUM;
#elif (defined(M_TCL))
typedef enum MAC_MODEL
{
    MAC_NONE = 0,       // 00
    MAC_F1_4KW,         // 01
    MAC_F1_6KW,         // 02
    MAC_F1_8KW,         // 03
    MAC_F1_10KW,        // 04
    MAC_F1_12KW,        // 05
    MAC_F1_14KW,        // 06
    MAC_F1_16KW,        // 07
    MAC_F3_10KW,        // 08
    MAC_F3_12KW,        // 09
    MAC_F3_14KW,        // 10
    MAC_F3_16KW,        // 11
    MAC_M1_4KW,         // 12
    MAC_M1_6KW,         // 13
    MAC_M1_8KW,         // 14
    MAC_M1_10KW,        // 15
    MAC_M1_12KW,        // 16
    MAC_M1_14KW,        // 17
    MAC_M1_16KW,        // 18
    MAC_M3_10KW,        // 19
    MAC_M3_12KW,        // 20
    MAC_M3_14KW,        // 21
    MAC_M3_16KW,        // 22
    MAC_F3_8KW,         // 23
    MAC_BAK25,          // 24
    MAC_BAK26,          // 25
    MAC_BAK27,          // 26
    MAC_BAK28,          // 27
    MAC_BAK29,          // 28
    MAC_BAK30,          // 29
    MAC_BAK31,          // 30
    MAC_BAK32,          // 31
    MAC_MAX                     /* ����������MAC_MAX֮ǰ���� */
}MAC_MODEL_ENUM;
#elif (defined(M_DISEN))
typedef enum MAC_MODEL
{
    MAC_NONE = 0,       /* δ���� */
    MAC_DSKDLNR8BpN3HD, /* DSKDLNR8BpN3HD */
    MAC_DSKDLNR10BpN3H, /* DSKDLNR10BpN3H */
    MAC_DSKDLNR12BpN3H, /* DSKDLNR12BpN3H */
    MAC_DSKDLNR14BpN3H, /* DSKDLNR14BpN3H */
    MAC_DSKDLNR16BpN3H, /* DSKDLNR16BpN3H */
    MAC_DSKDLNR28BpN3SH,/* DSKDLNR28BpN3SH */
    MAC_DSKDLNR35BpN3SH,/* DSKDLNR35BpN3SH */
    MAC_DSKDLNR10BhN5H, /* DSKDLNR10BhN5H */
    MAC_DSKDLNR16BhN5H, /* DSKDLNR16BhN5H */
    MAC_MAX,            /* ����������MAC_MAX֮ǰ���� */
} MAC_MODEL_ENUM;
#elif (defined(M_PAIWO))
typedef enum MAC_MODEL
{
    MAC_NONE = 0,       /* δ���� */
    MAC_MODEL_1,        /* ����4P */
    MAC_MODEL_2,        /* ����4P */
    MAC_MODEL_3,        /* ����3p */
    MAC_MODEL_4,        /* ����5p */
    MAC_MODEL_5,        /* ����6P */
    MAC_MAX,            /* ����������MAC_MAX֮ǰ���� */
} MAC_MODEL_ENUM;
#elif (defined(M_NIUENTAI))
typedef enum MAC_MODEL
{
    MAC_NONE = 0,       /* δ���� */
    MAC_R32_F_4,        /* R32_F_4 */
    MAC_R32_F_6,        /* R32_F_6 */
    MAC_R32_F_9,        /* R32_F_9 */
    MAC_R32_F_13,       /* R32_F_13 */
    MAC_R32_F_16,       /* R32_F_16 */
    MAC_R32_380F_9,     /* R32_380F_9 */
    MAC_R32_380F_13,    /* R32_380F_13 */
    MAC_R32_380F_16,    /* R32_380F_16 */
    MAC_R32_380F_18,    /* R32_380F_18 */
    MAC_R32_380F_20,    /* R32_380F_20 */
    MAC_R32_380F_23,    /* R32_380F_23 */
    MAC_R32_380F_26,    /* R32_380F_26 */
    MAC_R32_380F_30_NG, /* R32_380F_30_NG */
    MAC_R32_380F_32_NG, /* R32_380F_32_NG */
    MAC_R32_Z_4,        /* R32_Z_4 */
    MAC_R32_Z_6,        /* R32_Z_6 */
    MAC_R32_Z_9,        /* R32_Z_9 */
    MAC_R32_Z_13,       /* R32_Z_13 */
    MAC_R32_Z_16,       /* R32_Z_16 */
    MAC_R32_380Z_9,     /* R32_380Z_9 */
    MAC_R32_380Z_13,    /* R32_380Z_13 */
    MAC_R32_380Z_16,    /* R32_380Z_16 */
    MAC_R32_380Z_18,    /* R32_380Z_18 */
    MAC_R32_380Z_20,    /* R32_380Z_20 */
    MAC_R32_380Z_23,    /* R32_380Z_23 */
    MAC_R32_380Z_26,    /* R32_380Z_26 */
    MAC_R32_380Z_30,    /* R32_380Z_30 */
    MAC_R32_380Z_32,    /* R32_380Z_32 */
    MAC_R290_Z_4,       /* R290_Z_4 */
    MAC_R290_Z_6,       /* R290_Z_6 */
    MAC_R290_Z_9,       /* R290_Z_9 */
    MAC_R290_Z_13,      /* R290_Z_13 */
    MAC_R290_Z_16,      /* R290_Z_16 */
    MAC_R290_380Z_9,    /* R290_380Z_9 */
    MAC_R290_380Z_13,   /* R290_380Z_13 */
    MAC_R290_380Z_16,   /* R290_380Z_16 */
    MAC_R290_380Z_18,   /* R290_380Z_18 */
    MAC_R290_380Z_20,   /* R290_380Z_20 */
    MAC_MAX,            /* ����������MAC_MAX֮ǰ���� */
} MAC_MODEL_ENUM;
#elif (defined(M_HONGHE))
typedef enum MAC_MODEL
{
    MAC_NONE = 0,       /* δ���� */
    MAC_FTD10R32,       /* FTD10R32 */
    MAC_FTD8R32,        /* FTD8R32 */
    MAC_FTD6R32,        /* FTD6R32 */
    MAC_FTS16R32,       /* FTS16R32 */
    MAC_FTS14R32,       /* FTS14R32 */
    MAC_FTS12R32,       /* FTS12R32 */
    MAC_FTD16R32,       /* FTD16R32 */
    MAC_FTD14R32,       /* FTD14R32 */
    MAC_FTD12R32,       /* FTD12R32 */
    MAC_MAX,            /* ����������MAC_MAX֮ǰ���� */
} MAC_MODEL_ENUM;
#elif (defined(M_SHENGNENG))
typedef enum MAC_MODEL
{
    MAC_NONE = 0,       /* δ���� */
    MAC_WDLRK8IBMA1V1,  /* WDLRK8IBMA1V1 */
    MAC_WDLRK10IBMA1V1, /* WDLRK10IBMA1V1 */
    MAC_MAX,            /* ����������MAC_MAX֮ǰ���� */
} MAC_MODEL_ENUM;
#elif (defined(M_CHANGHONG)||defined(M_TONGYONG)||defined(M_WANHE)||defined(M_CHANGFAN))
typedef enum MAC_MODEL
{
    MAC_NONE = 0,
    MAC_MAX                     /* ����������MAC_MAX֮ǰ���� */
}MAC_MODEL_ENUM;
#endif

typedef struct tagPARA_FAC
{
    U08 grp;
    U08 idx;
    I16 para[MAC_MAX];
}PARA_FAC;
extern const PARA_FAC para_fac[];

PARA_EXT I16 MacTypeMaster;         /* �������� */
#define MACTYPE_IS_SAME (MacTypeMaster==val_mac_model)      /* ������ӻ��ͺ���ͬ */


#define HIEST_COOL              (35)        /* �����������ֵ */
#define LOEST_COOL             (-10)        /* ����������Сֵ */
#define HIEST_HEAT              (80)        /* �����������ֵ */
#define LOEST_HEAT               (0)        /* ����������Сֵ */
#define HIEST_HOTW      (HIEST_HEAT)        /* ��ˮ�������ֵ */
#define LOEST_HOTW      (LOEST_HEAT)        /* ��ˮ������Сֵ */
//=====================================================================================
/* 01�û����� */
#define P_sv_mode(f)                                        P_g_i(f, 0, 0)	/* �趨����ģʽ */
#define P_sv_cool(f)                                        P_g_i(f, 0, 1)	/* �趨�����¶�(��) */
#define P_sv_heat(f)                                        P_g_i(f, 0, 2)	/* �趨�����¶�(��) */
#define P_sv_hotw(f)                                        P_g_i(f, 0, 3)	/* ��ˮ�趨�¶� */
#define P_sv_room_cool(f)                                   P_g_i(f, 0, 4)	/* �����趨���� */
#define P_sv_room_heat(f)                                   P_g_i(f, 0, 5)	/* �����趨���� */
#define P_sv_cool_zone_B(f)                                 P_g_i(f, 0, 6)	/* �����趨�¶�(B) */
#define P_sv_room_cool_zone_B(f)                            P_g_i(f, 0, 7)	/* �����趨����(B) */
#define P_sv_heat_zone_B(f)                                 P_g_i(f, 0, 8)	/* �����趨�¶�(B) */
#define P_sv_room_heat_zone_B(f)                            P_g_i(f, 0, 9)	/* �����趨����(B) */
#define P_val_power_mode(f)                                 P_g_i(f, 0,10)	/* ����ģʽ */
#define P_val_mode_silent_bak(f)                            P_g_i(f, 0,11)	/* ����ģʽ */
#define P_val_timed_ster_fun(f)                             P_g_i(f, 0,12)	/* ��ʱɱ������ */
#define P_val_ster_temp(f)                                  P_g_i(f, 0,13)	/* ɱ���¶� */
#define P_val_ster_max_cycle(f)                             P_g_i(f, 0,14)	/* ɱ��������� */
#define P_val_ster_high_temp_Time(f)                        P_g_i(f, 0,15)	/* ɱ������ʱ�� */
#define P_val_bak_00_11(f)                                  P_g_i(f, 0,16)	/* ���� */
#define P_val_bak_00_12(f)                                  P_g_i(f, 0,17)	/* ���� */
#define P_val_bak_00_13(f)                                  P_g_i(f, 0,18)	/* ���� */
#define P_val_bak_00_14(f)                                  P_g_i(f, 0,19)	/* ���� */
#define P_val_bak_00_15(f)                                  P_g_i(f, 0,20)	/* ���� */
#define P_val_bak_00_16(f)                                  P_g_i(f, 0,21)	/* ���� */
#define P_val_bak_00_17(f)                                  P_g_i(f, 0,22)	/* ���� */
#define P_val_lock_screen(f)                                P_g_i(f, 0,23)	/* ������������ */
#define P_val_alarm_sound_set(f)                            P_g_i(f, 0,24)	/* ���������� */
#define P_val_zone_A_cool_weather_curve(f)                  P_g_i(f, 0,25)	/* A���������� */
#define P_val_zone_A_heat_weather_curve(f)                  P_g_i(f, 0,26)	/* A���������� */
#define P_val_zone_B_cool_weather_curve(f)                  P_g_i(f, 0,27)	/* B���������� */
#define P_val_zone_B_heat_weather_curve(f)                  P_g_i(f, 0,28)	/* B���������� */
#define P_val_cool_curve_9_TA_1(f)                          P_g_i(f, 0,29)	/* ����9���价��1 */
#define P_val_cool_curve_9_TA_2(f)                          P_g_i(f, 0,30)	/* ����9���价��2 */
#define P_val_cool_curve_9_TWout2_1(f)                      P_g_i(f, 0,31)	/* ����9�������1 */
#define P_val_cool_curve_9_TWout2_2(f)                      P_g_i(f, 0,32)	/* ����9�������2 */
#define P_val_heat_curve_9_TA_1(f)                          P_g_i(f, 0,33)	/* ����9���Ȼ���1 */
#define P_val_heat_curve_9_TA_2(f)                          P_g_i(f, 0,34)	/* ����9���Ȼ���2 */
#define P_val_heat_curve_9_TWout2_1(f)                      P_g_i(f, 0,35)	/* ����9���ȳ���1 */
#define P_val_heat_curve_9_TWout2_2(f)                      P_g_i(f, 0,36)	/* ����9���ȳ���2 */
#define P_val_cell_enable_00(f)                             P_g_i(f, 0,37)  /* 00#ģ������ */	/* ʹ��ָ����� */
#define P_val_cell_enable_01(f)                             P_g_i(f, 0,38)	/* 01#ģ������ */
#define P_val_cell_enable_02(f)                             P_g_i(f, 0,39)	/* 02#ģ������ */
#define P_val_cell_enable_03(f)                             P_g_i(f, 0,40)	/* 03#ģ������ */
#define P_val_cell_enable_04(f)                             P_g_i(f, 0,41)	/* 04#ģ������ */
#define P_val_cell_enable_05(f)                             P_g_i(f, 0,42)	/* 05#ģ������ */
#define P_val_cell_enable_06(f)                             P_g_i(f, 0,43)	/* 06#ģ������ */
#define P_val_cell_enable_07(f)                             P_g_i(f, 0,44)	/* 07#ģ������ */
#define sv_mode                                             (P_val(sv_mode                            ))/* �趨����ģʽ */
#define sv_cool                                          (10*P_val(sv_cool                            ))/* �趨�����¶�(��) */
#define sv_heat                                          (10*P_val(sv_heat                            ))/* �趨�����¶�(��) */
#define sv_hotw                                          (10*P_val(sv_hotw                            ))/* ��ˮ�趨�¶� */
#define sv_room_heat                                     (10*P_val(sv_room_heat                       ))/* �����趨���� */
#define sv_room_cool                                     (10*P_val(sv_room_cool                       ))/* �����趨���� */
#define sv_cool_zone_B                                   (10*P_val(sv_cool_zone_B                     ))/* �����趨�¶�(B) */
#define sv_room_cool_zone_B                              (10*P_val(sv_room_cool_zone_B                ))/* �����趨����(B) */
#define sv_heat_zone_B                                   (10*P_val(sv_heat_zone_B                     ))/* �����趨�¶�(B) */
#define sv_room_heat_zone_B                              (10*P_val(sv_room_heat_zone_B                ))/* �����趨����(B) */
#define val_power_mode                                      (P_val(val_power_mode                     ))/* ����ģʽ */
#define val_mode_silent_bak                                 (P_val(val_mode_silent_bak                ))/* ����ģʽ */
#define val_timed_ster_fun                                  (P_val(val_timed_ster_fun                 ))/* 13 ��ʱɱ������ */
#define val_ster_temp                                    (10*P_val(val_ster_temp                      ))/* 14 ɱ���¶� */
#define val_ster_max_cycle                               (60*P_val(val_ster_max_cycle                 ))/* 15 ɱ��������� */
#define val_ster_high_temp_Time                          (60*P_val(val_ster_high_temp_Time            ))/* 16 ɱ������ʱ�� */
#define val_bak_00_11                                       (P_val(val_bak_00_11                      ))/* ���� */
#define val_bak_00_12                                       (P_val(val_bak_00_12                      ))/* ���� */
#define val_bak_00_13                                       (P_val(val_bak_00_13                      ))/* ���� */
#define val_bak_00_14                                       (P_val(val_bak_00_14                      ))/* ���� */
#define val_bak_00_15                                       (P_val(val_bak_00_15                      ))/* ���� */
#define val_bak_00_16                                       (P_val(val_bak_00_16                      ))/* ���� */
#define val_bak_00_17                                       (P_val(val_bak_00_17                      ))/* ���� */
#define val_lock_screen                                     (P_val(val_lock_screen                    ))/* ������������ */
#define val_alarm_sound_set                                 (P_val(val_alarm_sound_set                ))/* ���������� */
#define val_zone_A_cool_weather_curve                       (P_val(val_zone_A_cool_weather_curve      ))/* A���������� */
#define val_zone_A_heat_weather_curve                       (P_val(val_zone_A_heat_weather_curve      ))/* A���������� */
#define val_zone_B_cool_weather_curve                       (P_val(val_zone_B_cool_weather_curve      ))/* B���������� */
#define val_zone_B_heat_weather_curve                       (P_val(val_zone_B_heat_weather_curve      ))/* B���������� */
#define val_cool_curve_9_TA_1                            (10*P_val(val_cool_curve_9_TA_1              ))/* ����9���价��1 */
#define val_cool_curve_9_TA_2                            (10*P_val(val_cool_curve_9_TA_2              ))/* ����9���价��2 */
#define val_cool_curve_9_TWout2_1                        (10*P_val(val_cool_curve_9_TWout2_1          ))/* ����9�������1 */
#define val_cool_curve_9_TWout2_2                        (10*P_val(val_cool_curve_9_TWout2_2          ))/* ����9�������2 */
#define val_heat_curve_9_TA_1                            (10*P_val(val_heat_curve_9_TA_1              ))/* ����9���Ȼ���1 */
#define val_heat_curve_9_TA_2                            (10*P_val(val_heat_curve_9_TA_2              ))/* ����9���Ȼ���2 */
#define val_heat_curve_9_TWout2_1                        (10*P_val(val_heat_curve_9_TWout2_1          ))/* ����9���ȳ���1 */
#define val_heat_curve_9_TWout2_2                        (10*P_val(val_heat_curve_9_TWout2_2          ))/* ����9���ȳ���2 */
#define val_cell_enable_00                                  (P_val(val_cell_enable_00                 ))/* 00#ģ������ */	/* ʹ��ָ����� */
#define val_cell_enable_01                                  (P_val(val_cell_enable_01                 ))/* 01#ģ������ */
#define val_cell_enable_02                                  (P_val(val_cell_enable_02                 ))/* 02#ģ������ */
#define val_cell_enable_03                                  (P_val(val_cell_enable_03                 ))/* 03#ģ������ */
#define val_cell_enable_04                                  (P_val(val_cell_enable_04                 ))/* 04#ģ������ */
#define val_cell_enable_05                                  (P_val(val_cell_enable_05                 ))/* 05#ģ������ */
#define val_cell_enable_06                                  (P_val(val_cell_enable_06                 ))/* 06#ģ������ */
#define val_cell_enable_07                                  (P_val(val_cell_enable_07                 ))/* 07#ģ������ */

/* ��ȡA��ѡ�û���-Ŀ������ID */
#define GET_ZONE_A_WEATHER_CURVE(mode)  (((mode) == MODE_COOL) ? (val_zone_A_cool_weather_curve) \
                                                               : (val_zone_A_heat_weather_curve))

/* ��ȡB��ѡ�û���-Ŀ������ID */
#define GET_ZONE_B_WEATHER_CURVE(mode)  (((mode) == MODE_COOL) ? (val_zone_B_cool_weather_curve) \
                                                               : (val_zone_B_heat_weather_curve))

#define val_set_temp_curve              0
#define val_link_en                     0    // ���ȶ�ͨ����
#define val_duty_hot_water_set          40
#define val_duty_func                   0
#define val_duty_heat_set               30
#define val_duty_cool_set               17
#define val_duty_en_h                   20
#define val_duty_en_m                   0
#define val_duty_dis_h                  5
#define val_duty_dis_m                  0

/*02ϵͳ����*/
#define SYS_ITEM_IDX_MAC_MODEL          3
#define P_val_air_type(f)                                   P_g_i(f, 1, 0)  /* �յ����� */
#define P_val_refrigerant_type(f)                           P_g_i(f, 1, 1)	/* ��ý���� */
#define P_val_valve_type(f)                                 P_g_i(f, 1, 2)	/* ��ͨ������ */
#define P_val_mac_model(f)                                  P_g_i(f, 1, 3)	/* �����ͺ� */
#define P_val_remote_switch_type(f)                         P_g_i(f, 1, 4)  /* �߿ؿ�������,0����/1�ζ� */
#define P_val_on_off_control_type_bak(f)                    P_g_i(f, 1, 5)  /* ������ͣ���� */
#define P_val_remember_run(f)                               P_g_i(f, 1, 6)	/* ������俪�ػ� */
#define P_val_auto_run(f)                                   P_g_i(f, 1, 7)  /* ���������� */
#define P_val_pump_mode(f)                                  P_g_i(f, 1, 8)	/* ˮ������ģʽ */
#define P_val_ctrl_object(f)                                P_g_i(f, 1, 9)	/* ���ƶ��� */
#define P_val_ctrl_cpst_cool(f)                             P_g_i(f, 1,10)  /* ����ˮ�²��� */
#define P_val_ctrl_cpst_heat(f)                             P_g_i(f, 1,11)  /* ����ˮ�²��� */
#define P_val_valve_on_dly(f)                               P_g_i(f, 1,12)	/* ��ͨ������ʱ(��ֵ:��ǰ��,��ֵ:��ʱ��) */
#define P_val_valve_off_dly(f)                              P_g_i(f, 1,13)	/* ��ͨ������ʱ(��ֵ:��ǰ��,��ֵ:��ʱ��) */
#define P_val_open_v_exh(f)                                 P_g_i(f, 1,14)	/* ����Һ������ */
#define P_val_close_v_exh_diff(f)                           P_g_i(f, 1,15)	/* ����Һ�����²� */
#define P_val_hotw_used(f)                                  P_g_i(f, 1,16)	/* ��ˮ���� */
#define P_val_hotw_diff(f)                                  P_g_i(f, 1,17)  /* ��ˮ�ز� */
#define P_val_open_val_temp(f)                              P_g_i(f, 1,18)	/* �����ỷ�� */
#define P_val_close_val_temp(f)                             P_g_i(f, 1,19)	/* �����ỷ�� */
#define P_val_cool_used(f)                                  P_g_i(f, 1,20)  /* ����ģʽ */
#define P_val_heat_used(f)                                  P_g_i(f, 1,21)  /* ����ģʽ */
#define P_val_power_down_time_min(f)                        P_g_i(f, 1,22)	/* ��СԤ�ȵ��� */
#define P_val_power_down_time_max(f)                        P_g_i(f, 1,23)	/* ���Ԥ�ȵ��� */
#define P_val_warm_time_min(f)                              P_g_i(f, 1,24)	/* ��СԤ��ʱ�� */
#define P_val_warm_time_max(f)                              P_g_i(f, 1,25)	/* ���Ԥ��ʱ�� */
#define P_val_hotw_priority(f)                              P_g_i(f, 1,26)  /* ��ˮģʽ���� */
#define P_val_TBH_used(f)                                   P_g_i(f, 1,27)	/* ˮ����� */
#define P_val_use_PUMPret(f)                                P_g_i(f, 1,28)	/* �»�ˮ�� */
#define P_val_solar_used(f)                                 P_g_i(f, 1,29)	/* ̫���� */
#define P_val_remote_onoff_type(f)                          P_g_i(f, 1,30)	/* ������������ */
#define P_val_inverter_used(f)                              P_g_i(f, 1,31)	/* ��Ƶ��ʹ�ã�����ɾ����������Ӧ���룬�����Ƶѹ��ʱ���� */
#define P_val_sys_PUMPf_used(f)                             P_g_i(f, 1,32)	/* 47 ϵͳ��Ƶˮ�� */
#define P_val_RT_ctrl_type(f)                               P_g_i(f, 1,33)	/* 12 �¿������� */
#define P_val_UI35_ctrl_type(f)                             P_g_i(f, 1,34)	/* �߿������� */
#define P_val_load_revise_range(f)                          P_g_i(f, 1,35)	/* ������������ */
#define P_val_load_revise_time(f)                           P_g_i(f, 1,36)	/* ��������ʱ�� */
#define P_val_chassis_heat_mode(f)                          P_g_i(f, 1,37)	/* ���̵���ģʽ */
#define P_val_anti_ice_heat_env(f)                          P_g_i(f, 1,38)	/* ���̵��Ȼ��� */
#define P_val_TCL_splash_offset(f)                          P_g_i(f, 1,39)	/* ����ҳ���� */
#define P_val_smart_grid_cfg(f)                             P_g_i(f, 1,40)	/* ���ܵ������� */
#define P_val_smart_grid_used(f)                            P_g_i(f, 1,41)	/* ���ܵ������� */
#define P_val_cl_ht_adj_ratio(f)                            P_g_i(f, 1,42)	/* ���ȵ���ϵ�� */
#define P_val_PUMPf_interval_off_time(f)                    P_g_i(f, 1,43)	/* ˮ�ü�Ъͣʱ�� */
#define val_air_type                                        (P_val(val_air_type                       ))/* 01 �յ����� */
#define val_refrigerant_type                                (P_val(val_refrigerant_type               ))/* 02 ��ý���� */
#define val_valve_type                                      (P_val(val_valve_type                     ))/* 03 ��ͨ������ */
#define val_mac_model                                       (P_val(val_mac_model                      ))/* 04 �����ͺ� */
#define val_remote_switch_type                              (P_val(val_remote_switch_type             ))/* 05 �߿ؿ�������,0����/1�ζ� */
#define val_on_off_control_type_bak                         (P_val(val_on_off_control_type_bak        ))/* 06 ������ͣ���� */
#define val_remember_run                                    (P_val(val_remember_run                   ))/* 07 ������俪�ػ� */
#define val_auto_run                                        (P_val(val_auto_run                       ))/* 08 ���������� */
#define val_pump_mode                                       (P_val(val_pump_mode                      ))/* 09 ˮ������ģʽ */
#define val_ctrl_object                                     (P_val(val_ctrl_object                    ))/* 10 ���ƶ��� */
#define val_ctrl_cpst_cool                                  (P_val(val_ctrl_cpst_cool                 ))/* 11 ����ˮ�²��� */
#define val_ctrl_cpst_heat                                  (P_val(val_ctrl_cpst_heat                 ))/* 12 ����ˮ�²��� */
#define val_valve_on_dly                                    (P_val(val_valve_on_dly                   ))/* 13 ��ͨ������ʱ(��ֵ:��ǰ��,��ֵ:��ʱ��) */
#define val_valve_off_dly                                   (P_val(val_valve_off_dly                  ))/* 14 ��ͨ������ʱ(��ֵ:��ǰ��,��ֵ:��ʱ��) */
#define val_open_v_exh                                   (10*P_val(val_open_v_exh                     ))/* 15 ����Һ������ */
#define val_close_v_exh_diff                             (10*P_val(val_close_v_exh_diff               ))/* 16 ����Һ�����²� */
#define val_hotw_used                                       (P_val(val_hotw_used                      ))/* 17 ��ˮ���� */
#define val_hotw_diff                                       (P_val(val_hotw_diff                      ))/* 18 ��ˮ�ز� */
#define val_open_val_temp                                (10*P_val(val_open_val_temp                  ))/* 19 �����ỷ�� */
#define val_close_val_temp                               (10*P_val(val_close_val_temp                 ))/* 20 �����ỷ�� */
#define val_cool_used                                       (P_val(val_cool_used                      ))/* 21 ����ģʽ */
#define val_heat_used                                       (P_val(val_heat_used                      ))/* 22 ����ģʽ */
#define val_power_down_time_min                       (QK_60*P_val(val_power_down_time_min            ))/* 23 ��СԤ�ȵ��� */
#define val_power_down_time_max                       (QK_60*P_val(val_power_down_time_max            ))/* 24 ���Ԥ�ȵ��� */
#define val_warm_time_min                                   (P_val(val_warm_time_min                  ))/* 25 ��СԤ��ʱ�� */
#define val_warm_time_max                                   (P_val(val_warm_time_max                  ))/* 26 ���Ԥ��ʱ�� */
#define val_hotw_priority                                   (P_val(val_hotw_priority                  ))/* 27 ��ˮģʽ���� */
#define val_TBH_used                                        (P_val(val_TBH_used                       ))/* 28 ˮ����� */
#define val_use_PUMPret                                     (P_val(val_use_PUMPret                    ))/* 29 �»�ˮ�� */
#define val_solar_used                                      (P_val(val_solar_used                     ))/* 30 ̫���� */
#define val_remote_onoff_type                               (P_val(val_remote_onoff_type              ))/* 31 ������������ */
#define val_inverter_used                                   (P_val(val_inverter_used                  ))/* 32 ��Ƶ��ʹ�ã�����ɾ����������Ӧ���룬�����Ƶѹ��ʱ���� */
#define val_sys_PUMPf_used                                  (P_val(val_sys_PUMPf_used                 ))/* 33 ϵͳ��Ƶˮ�� */
#define val_RT_ctrl_type                                    (P_val(val_RT_ctrl_type                   ))/* 34 �¿������� */
#define val_UI35_ctrl_type                                  (P_val(val_UI35_ctrl_type                 ))/* 35 �߿������� */
#define val_load_revise_range                            (10*P_val(val_load_revise_range              ))/* 36 ������������ */
#define val_load_revise_time                                (P_val(val_load_revise_time               ))/* 37 ��������ʱ�� */
#define val_chassis_heat_mode                               (P_val(val_chassis_heat_mode              ))/* 38 ���̵���ģʽ */
#define val_anti_ice_heat_env                            (10*P_val(val_anti_ice_heat_env              ))/* 39 ���̵��Ȼ��� */
#define val_TCL_splash_offset                               (P_val(val_TCL_splash_offset              ))/* 40 ����ҳ���� */
#define val_smart_grid_cfg                                  (P_val(val_smart_grid_cfg                 ))/* 41 ���ܵ������� */
#define val_smart_grid_used                                 (P_val(val_smart_grid_used                ))/* 42 ���ܵ������� */
#define val_cl_ht_adj_ratio                                 (P_val(val_cl_ht_adj_ratio                ))/* 43 ���ȵ���ϵ�� */
#define val_PUMPf_interval_off_time                   (QK_60*P_val(val_PUMPf_interval_off_time        ))/* 44 ˮ�ü�Ъͣʱ�� */

/* 2023-1-9�������ȹ̶�Ϊ��һֱ���С����������Ƹù��ܺ��ٶ��嵽ʵ�ʲ��� */
// #define val_pump_mode PUMP_MODE_ON

/* �̶�Ϊ�����ϡ� */
#define val_on_off_control_type         (ONOFF_ALL)

/* ʹ�ö�Ƶѹ�� */
#define USE_FIX_COMP                    (!val_inverter_used)
/* ��ˮģʽ���� */
#define USE_HOTW_PRIORITY               (val_hotw_priority||fg_sg_hotw_priority)

/* ʹ������ģʽ */
#define USE_COOL                        ((val_air_type==COOL_ONLY)||((val_air_type==REFR_HEAT)&&(val_cool_used==USED)))
/* ʹ������ģʽ */
#define USE_HEAT                        ((val_air_type==HEAT_ONLY)||((val_air_type==REFR_HEAT)&&(val_heat_used==USED)))

/* ������ */
#define USE_COOL_ONLY                   ((val_air_type==COOL_ONLY)||((val_air_type==REFR_HEAT)&&(val_cool_used==USED)&&(val_heat_used==UNUSED)))
/* ������ */
#define USE_HEAT_ONLY                   ((val_air_type==HEAT_ONLY)||((val_air_type==REFR_HEAT)&&(val_heat_used==USED)&&(val_cool_used==UNUSED)))

#define val_machine_type                0   // �豸����
#define val_warm_env                    (val_open_val_temp)     /* Ԥ�Ȼ����¶� */
#define val_ac_heater_open (-250)

/* �߿����������¿��� */
#define USE_UI35_ONE_ZONE_ROOM_TEMP_CTRL    (!USE_UIRT_CTRL && (get_UI35_ctrl_type()==UI35_ZONE_A_TR))
/* �߿�������ˮ�¿��� */
#define USE_UI35_ONE_ZONE_WATER_TEMP_CTRL   (!USE_UIRT_CTRL && (get_UI35_ctrl_type()==UI35_ZONE_A_TWout2))
/* �߿���˫��ˮ�¿��� */
#define USE_UI35_DUAL_ZONE_WATER_TEMP_CTLR  (!USE_UIRT_CTRL && (get_UI35_ctrl_type()==UI35_DUAL_ZONE_TWout2))
/* �߿���˫����Ͽ��� */
#define USE_UI35_DUAL_ZONE_HYBRID_CTRL      (!USE_UIRT_CTRL && (get_UI35_ctrl_type()==UI35_DUAL_ZONE_TWout2_TR))
/* �߿���˫������ */
#define USE_UI35_DUAL_ZONE_CTRL             (USE_UI35_DUAL_ZONE_WATER_TEMP_CTLR || USE_UI35_DUAL_ZONE_HYBRID_CTRL)
/* �߿������¿��� */
#define USE_UI35_ROOM_TEMP_CTRL             (USE_UI35_ONE_ZONE_ROOM_TEMP_CTRL || USE_UI35_DUAL_ZONE_HYBRID_CTRL)

/* ʹ���¿������� */
#define USE_UIRT_CTRL                       (val_RT_ctrl_type != UIRT_UNUSED)
/* �¿���A��ģʽ�л� */
#define USE_UIRT_ZONE_A_MODE_SWITCH         (val_RT_ctrl_type == UIRT_ZONE_A_MODE_SWITCHING)
/* �¿���A������ */
#define USE_UIRT_ZONE_A_ON_OFF              (val_RT_ctrl_type == UIRT_ZONE_A_ON_OFF)
/* �¿���˫������ */
#define USE_UIRT_DUAL_ZONE_ON_OFF           (val_RT_ctrl_type == UIRT_DUAL_ZONE_ON_OFF)

/* ˫������ */
#define USE_DUAL_ZONE_CTRL                  (USE_UI35_DUAL_ZONE_CTRL || USE_UIRT_DUAL_ZONE_ON_OFF)

/* ���������ܵ� */
#define ONE_ZONE_ROOM_TEMP_CTRL             (USE_UI35_ONE_ZONE_ROOM_TEMP_CTRL)
/* ����ˮ���ܵ� */
#define ONE_ZONE_WATER_TEMP_CTRL            (USE_UIRT_ZONE_A_MODE_SWITCH||USE_UIRT_ZONE_A_ON_OFF||USE_UI35_ONE_ZONE_WATER_TEMP_CTRL)
/* ˫��ˮ���ܵ� */
#define DUAL_ZONE_WATER_TEMP_CTRL           (USE_UIRT_DUAL_ZONE_ON_OFF||USE_UI35_DUAL_ZONE_WATER_TEMP_CTLR)
/* ˫������ܵ� */
#define DUAL_ZONE_HYBRID_CTRL               (USE_UI35_DUAL_ZONE_HYBRID_CTRL)

/* ���ܵ������� */
enum
{
    SG_CFG_REGULAR,         /* ����: SG-Ready��׼ */
    SG_CFG_NO_STA1,         /* ����1: �������ܵ���״̬1 */
    SG_CFG_MAX,
};

/*03��Ƶѹ������*/

#define IDX_INVT_MODEL          0
#define IDX_INVT_MIN            5
#define IDX_INVT_MAX            6
#define IDX_INVT_START          18
#define IDX_INVT_CUR_LMT        34
#define IDX_INVT_LOAD_TIME      35
#define IDX_INVT_BLOCKAGE_TIME  36

#define P_val_invt_motor_type(f)                            P_g_i(f, 2, 0)	/* ��Ƶѹ������ */
#define P_val_invt_manual_mode(f)                           P_g_i(f, 2, 1)	/* �ֶ�ģʽ */
#define P_val_invt_manual_rps(f)                            P_g_i(f, 2, 2)	/* �ֶ��趨ת�� */
#define P_val_cm_off_time(f)                                P_g_i(f, 2, 3)	/* ��Ƶ������ */
#define P_val_invt_freq_pre_sub(f)                          P_g_i(f, 2, 4)	/* ����Ԥ��Ƶ */
#define P_val_invt_rps_min(f)                               P_g_i(f, 2, 5)	/* ����ת������(0.01rps) */
#define P_val_invt_rps_max(f)                               P_g_i(f, 2, 6)	/* ����ת������(0.01rps) */
#define P_val_invt_rps_cool(f)                              P_g_i(f, 2, 7)	/* ����ת��(0.01rps) */
#define P_val_invt_rps_heat(f)                              P_g_i(f, 2, 8)	/* ���ȶת��(0.01rps) */
#define P_val_freq_stably_ctrl_time(f)                      P_g_i(f, 2, 9)	/* ����ά��ʱ�䣨�룩 */
#define P_val_hp_forbid_add_prs_diff(f)                     P_g_i(f, 2,10)	/* ��ѹ�޼�ƫ�� */
#define P_val_lp_forbid_add_prs_diff(f)                     P_g_i(f, 2,11)	/* ��ѹ�޼�ƫ�� */
#define P_val_hp_force_sub_prs_diff(f)                      P_g_i(f, 2,12)	/* ��ѹǿжƫ�� */
#define P_val_lp_force_sub_prs_diff(f)                      P_g_i(f, 2,13)	/* ��ѹǿжƫ�� */
#define P_val_reoil_period(f)                               P_g_i(f, 2,14)	/* ��Ƶ�����ʱ�� */
#define P_val_reoil_time(f)                                 P_g_i(f, 2,15)	/* ǿ�ƻ���ʱ�� */
#define P_val_invt_addr(f)                                  P_g_i(f, 2,16)	/* ��Ƶ������ַ */
#define P_val_invt_rps_defrost(f)                           P_g_i(f, 2,17)	/* ѹ����˪ת�� */
#define P_val_invt_start_freq(f)                            P_g_i(f, 2,18)	/* ѹ������ת�� */
#define P_val_invt_start_speed(f)                           P_g_i(f, 2,19)	/* ������Ƶ���� */
#define P_val_invt_run_speed(f)                             P_g_i(f, 2,20)	/* ������Ƶ���� */
#define P_val_invt_defr_speed(f)                            P_g_i(f, 2,21)	/* ��˪��Ƶ���� */
#define P_val_invt_rps_adj_std(f)                           P_g_i(f, 2,22)	/* ��׼Ƶ�ʵ��� */
#define P_val_invt_start_period(f)                          P_g_i(f, 2,23)	/* ������Ƶ��� */
#define P_val_f_pid_p(f)                                    P_g_i(f, 2,24)	/* ��Ƶ������ ��λ0.1 */
#define P_val_f_pid_i(f)                                    P_g_i(f, 2,25)	/* ��Ƶ����ʱ�� */
#define P_val_f_pid_d(f)                                    P_g_i(f, 2,26)	/* ��Ƶ΢��ʱ�� */
#define P_val_f_pid_period(f)                               P_g_i(f, 2,27)	/* ��Ƶ�������� */
#define P_val_invt_start_freq2(f)                           P_g_i(f, 2,28)	/* ѹ������ά��ת�� */
#define P_val_f_pid_Kp(f)                                   P_g_i(f, 2,29)	/* ��Ƶ����ϵ�� */
#define P_val_cool_cond_temphigh(f)                         P_g_i(f, 2,30)	/* ������������ */
#define P_val_heat_cond_temphigh(f)                         P_g_i(f, 2,31)	/* ������������ */
#define P_val_invt_rps_adj_eco(f)                           P_g_i(f, 2,32)	/* ����Ƶ�ʵ��� */
#define P_val_heat_evap_templow(f)                          P_g_i(f, 2,33)	/* ������������ */
#define P_val_freq_rated_time(f)                            P_g_i(f, 2,34)	/* �ά��ʱ�� */
#define P_val_cool_reoil_env(f)                             P_g_i(f, 2,35)	/* ������ͻ��� */
#define P_val_heat_reoil_env(f)                             P_g_i(f, 2,36)	/* ���Ȼ��ͻ��� */
#define P_val_force_ctrl_gas_out(f)                         P_g_i(f, 2,37)	/* ǿ�ƽ�Ƶ���� */
#define P_val_reoil_inventrps(f)                            P_g_i(f, 2,38)	/* ��������Ƶ�� */
#define P_val_invt_rps_str_silent_max(f)                    P_g_i(f, 2,39)	/* ���������ת�� */
#define P_val_invt_rps_adj_strong(f)                        P_g_i(f, 2,40)	/* ǿ��Ƶ�ʵ��� */
#define P_val_invt_rps_silent_max(f)                        P_g_i(f, 2,41)	/* �������ת�� */
#define P_val_avoid_freq_range(f)                           P_g_i(f, 2,42)	/* �ر�Ƶ�ʷ�Χ */
#define P_val_avoid_freq1(f)                                P_g_i(f, 2,43)	/* �ر�Ƶ��1 */
#define P_val_avoid_freq2(f)                                P_g_i(f, 2,44)	/* �ر�Ƶ��2 */
#define P_val_avoid_freq3(f)                                P_g_i(f, 2,45)	/* �ر�Ƶ��3 */
#define P_val_avoid_freq4(f)                                P_g_i(f, 2,46)	/* �ر�Ƶ��4 */
#define P_val_low_env_start_freq(f)                         P_g_i(f, 2,47)	/* �ͻ�����ά��ת�� */
#define val_invt_motor_type                                 (P_val(val_invt_motor_type                ))/* 01 ��Ƶѹ������ */
#define val_invt_manual_mode                                (P_val(val_invt_manual_mode               ))/* 02 �ֶ�ģʽ */
#define val_invt_manual_rps                             (100*P_val(val_invt_manual_rps                ))/* 03 �ֶ��趨ת�� */
#define val_cm_off_time                                     (P_val(val_cm_off_time                    ))/* 04 ��Ƶ������ */
#define val_invt_freq_pre_sub                           (100*P_val(val_invt_freq_pre_sub              ))/* 05 ����Ԥ��Ƶ */
#define val_invt_rps_min                                (100*P_val(val_invt_rps_min                   ))/* 06 ����ת������(0.01rps) */
#define val_invt_rps_max                                (100*P_val(val_invt_rps_max                   ))/* 07 ����ת������(0.01rps) */
#define val_invt_rps_cool                               (100*P_val(val_invt_rps_cool                  ))/* 08 ����ת��(0.01rps) */
#define val_invt_rps_heat                               (100*P_val(val_invt_rps_heat                  ))/* 09 ���ȶת��(0.01rps) */
#define val_freq_stably_ctrl_time                           (P_val(val_freq_stably_ctrl_time          ))/* 10 ����ά��ʱ�䣨�룩 */
#define val_hp_forbid_add_prs_diff                          (P_val(val_hp_forbid_add_prs_diff         ))/* 11 ��ѹ�޼�ƫ�� */
#define val_lp_forbid_add_prs_diff                          (P_val(val_lp_forbid_add_prs_diff         ))/* 12 ��ѹ�޼�ƫ�� */
#define val_hp_force_sub_prs_diff                           (P_val(val_hp_force_sub_prs_diff          ))/* 13 ��ѹǿжƫ�� */
#define val_lp_force_sub_prs_diff                           (P_val(val_lp_force_sub_prs_diff          ))/* 14 ��ѹǿжƫ�� */
#define val_reoil_period                                 (60*P_val(val_reoil_period                   ))/* 15 ��Ƶ�����ʱ�� */
#define val_reoil_time                                      (P_val(val_reoil_time                     ))/* 16 ǿ�ƻ���ʱ�� */
#define val_invt_addr                                       (P_val(val_invt_addr                      ))/* 17 ��Ƶ������ַ */
#define val_invt_rps_defrost                            (100*P_val(val_invt_rps_defrost               ))/* 18 ѹ����˪ת�� */
#define val_invt_start_freq                             (100*P_val(val_invt_start_freq                ))/* 19 ѹ������ת�� */
#define val_invt_start_speed                            (100*P_val(val_invt_start_speed               ))/* 20 ������Ƶ���� */
#define val_invt_run_speed                              (100*P_val(val_invt_run_speed                 ))/* 21 ������Ƶ���� */
#define val_invt_defr_speed                             (100*P_val(val_invt_defr_speed                ))/* 22 ��˪��Ƶ���� */
#define val_invt_rps_adj_std                            (100*P_val(val_invt_rps_adj_std               ))/* 23 ��׼Ƶ�ʵ��� */
#define val_invt_start_period                               (P_val(val_invt_start_period              ))/* 24 ������Ƶ��� */
#define val_f_pid_p                                         (P_val(val_f_pid_p                        ))/* 25 ��Ƶ������ ��λ0.1 */
#define val_f_pid_i                                         (P_val(val_f_pid_i                        ))/* 26 ��Ƶ����ʱ�� */
#define val_f_pid_d                                         (P_val(val_f_pid_d                        ))/* 27 ��Ƶ΢��ʱ�� */
#define val_f_pid_period                                    (P_val(val_f_pid_period                   ))/* 28 ��Ƶ�������� */
#define val_invt_start_freq2                            (100*P_val(val_invt_start_freq2               ))/* 29 ѹ������ά��ת�� */
#define val_f_pid_Kp                                        (P_val(val_f_pid_Kp                       ))/* 30 ��Ƶ����ϵ�� */
#define val_cool_cond_temphigh                           (10*P_val(val_cool_cond_temphigh             ))/* 31 ������������ */
#define val_heat_cond_temphigh                           (10*P_val(val_heat_cond_temphigh             ))/* 32 ������������ */
#define val_invt_rps_adj_eco                            (100*P_val(val_invt_rps_adj_eco               ))/* 33 ����Ƶ�ʵ��� */
#define val_heat_evap_templow                            (10*P_val(val_heat_evap_templow              ))/* 34 ������������ */
#define val_freq_rated_time                                 (P_val(val_freq_rated_time                ))/* 35 �ά��ʱ�� */
#define val_cool_reoil_env                                  (P_val(val_cool_reoil_env                 ))/* 36 ������ͻ��� */
#define val_heat_reoil_env                                  (P_val(val_heat_reoil_env                 ))/* 37 ���Ȼ��ͻ��� */
#define val_force_ctrl_gas_out                           (10*P_val(val_force_ctrl_gas_out             ))/* 38 ǿ�ƽ�Ƶ���� */
#define val_reoil_inventrps                             (100*P_val(val_reoil_inventrps                ))/* 39 ��������Ƶ�� */
#define val_invt_rps_str_silent_max                     (100*P_val(val_invt_rps_str_silent_max        ))/* 40 ���������ת�� */
#define val_invt_rps_adj_strong                         (100*P_val(val_invt_rps_adj_strong            ))/* 41 ǿ��Ƶ�ʵ��� */
#define val_invt_rps_silent_max                         (100*P_val(val_invt_rps_silent_max            ))/* 42 �������ת�� */
#define val_avoid_freq_range                            (100*P_val(val_avoid_freq_range               ))/* 43 �ر�Ƶ�ʷ�Χ */
#define val_avoid_freq1                                 (100*P_val(val_avoid_freq1                    ))/* 44 �ر�Ƶ��1 */
#define val_avoid_freq2                                 (100*P_val(val_avoid_freq2                    ))/* 45 �ر�Ƶ��2 */
#define val_avoid_freq3                                 (100*P_val(val_avoid_freq3                    ))/* 46 �ر�Ƶ��3 */
#define val_avoid_freq4                                 (100*P_val(val_avoid_freq4                    ))/* 47 �ر�Ƶ��4 */
#define val_low_env_start_freq                          (100*P_val(val_low_env_start_freq             ))/* 48 �ͻ�����ά��ת�� */

#define val_avoid_freq(n)           (100*para_C[GROUP_COMP][43+n])// �ر�Ƶ��

#define val_f_pid_calculate_period      8	/* ��Ƶ�������� */
#define val_off_speed_down_time         30 // ͣ����Ƶʱ��
#define CM_off_speed_down_time  (val_inverter_used? val_off_speed_down_time: 0) // ѹ��ͣ����Ƶʱ��

#if defined(M_DISEN)
#define GAS_OUT_HIGH_DIFF1  (pv_cell_TA(pcb_addr) <= -70 ?\
                             0 : pv_cell_TA(pcb_addr) <= 70 ?\
                                 50 : 100)
#else
#define GAS_OUT_HIGH_DIFF1  50
#endif
#define GAS_OUT_HIGH_DIFF2  (GAS_OUT_HIGH_DIFF1+30)

#define val_f_pid_flexi             (10)    // ����ϵ��
#define val_f_pid_fg                (1)     // �Ƿ�ʹ��PID��Ƶ����
//#define val_energy_ctrl_time      ( 60*para_C[GROUP_COMP][8])       // 8����������ʱ���֣�

/* 04����������� */
#define FAN_FREQ_MAX 2000
#define P_val_freq_fan_set(f)                               P_g_i(f, 3, 0)	/* ��Ƶ������� */
#define P_val_invt_fan_type(f)                              P_g_i(f, 3, 1)	/* EC������� */
#define P_val_freq_fan_cool_max(f)                          P_g_i(f, 3, 2)	/* ��������� */
#define P_val_freq_fan_cool_min(f)                          P_g_i(f, 3, 3)	/* ��������� */
#define P_val_freq_fan_eco_adj(f)                           P_g_i(f, 3, 4)	/* ���ܷ��ٵ��� */
#define P_val_freq_fan_ctrl(f)                              P_g_i(f, 3, 5)	/* ��Ƶ������� */
#define P_val_freq_fan_hand_speed(f)                        P_g_i(f, 3, 6)	/* ����ֶ����� */
#define P_val_ivnt_prot_temp(f)                             P_g_i(f, 3, 7)	/* �߷�ģ���¶� */
#define P_val_hig_fan_env_cold(f)                           P_g_i(f, 3, 8)	/* ����߷绷�� */
#define P_val_low_fan_env_cold(f)                           P_g_i(f, 3, 9)	/* ����ͷ绷�� */
#define P_val_hig_fan_env_heat(f)                           P_g_i(f, 3,10)	/* ���ȸ߷绷�� */
#define P_val_low_fan_env_heat(f)                           P_g_i(f, 3,11)	/* ���ȵͷ绷�� */
#define P_val_hig_fan_wing_cold(f)                          P_g_i(f, 3,12)	/* ����߷����� */
#define P_val_low_fan_wing_cold(f)                          P_g_i(f, 3,13)	/* ����ͷ����� */
#define P_val_hig_fan_wing_heat(f)                          P_g_i(f, 3,14)	/* ���ȸ߷����� */
#define P_val_low_fan_wing_heat(f)                          P_g_i(f, 3,15)	/* ���ȵͷ����� */
#define P_val_mid_fan_temp_heat(f)                          P_g_i(f, 3,16)	/* �����з��¶� */
#define P_val_fan_speed_period(f)                           P_g_i(f, 3,17)	/* ���ٵ������� */
#define P_val_fan_speed_percent(f)                          P_g_i(f, 3,18)	/* ���ٶ������� */
#define P_val_freq_fan_heat_max(f)                          P_g_i(f, 3,19)	/* ���ȷ����� */
#define P_val_freq_fan_heat_min(f)                          P_g_i(f, 3,20)	/* ���ȷ����� */
#define P_val_fan_pwm_pulse(f)                              P_g_i(f, 3,21)	/* PWM�������  */
#define P_val_freq_fan_strong_adj(f)                        P_g_i(f, 3,22)	/* ǿ�����ٵ��� */
#define P_val_ec_fan_base_addr(f)                           P_g_i(f, 3,23)	/* EC�����ַ */
#define P_val_mid_fan_temp_cold(f)                          P_g_i(f, 3,24)	/* �����з��¶� */
#define P_val_freq_fan_str_silent_max(f)                    P_g_i(f, 3,25)	/* ������������ */
#define P_val_freq_fan_num(f)                               P_g_i(f, 3,26)	/* ��Ƶ������� */
#define P_val_freq_fan_silent_max(f)                        P_g_i(f, 3,27)	/* ���������� */
#define P_val_pwm_fan_init(f)                               P_g_i(f, 3,28)	/* pwm���ٳ�ʼֵ */
#define P_val_pwm_fan_coef(f)                               P_g_i(f, 3,29)	/* pwm����ϵ�� */
#define P_val_pwm_fan_period(f)                             P_g_i(f, 3,30)	/* pwm���ټ�� */
#define P_val_pwm_fan_area(f)                               P_g_i(f, 3,31)	/* pwm�������� */
#define P_val_freq_fan_std_adj(f)                           P_g_i(f, 3,32)	/* ��׼���ٵ��� */
#define P_val_avoid_cold(f)                                 P_g_i(f, 3,33)	/* ����繦�� */
#define P_val_send_fan_pause(f)                             P_g_i(f, 3,34)	/* �ͷ����ͣ */
#define P_val_avoid_cold_fan_time(f)                        P_g_i(f, 3,35)	/* �����ʱ�� */
#define P_val_freq_Cpst_Coef(f)                             P_g_i(f, 3,36)	/* Ƶ�ʲ���ϵ�� */
#define P_val_min_Cpst_Coef(f)                              P_g_i(f, 3,37)	/* ����ϵ������ */
#define P_val_Tp_fan_rpm_corr_1(f)                          P_g_i(f, 3,38)	/* ���·�������1 */
#define P_val_Tp_fan_rpm_corr_2(f)                          P_g_i(f, 3,39)	/* ���·�������2 */
#define P_val_Tp_fan_rpm_corr_3(f)                          P_g_i(f, 3,40)	/* ���·�������3 */
#define P_val_Tp_fan_rpm_corr_set(f)                        P_g_i(f, 3,41)	/* ������������ */
#define P_val_close_fan_set_cl(f)                           P_g_i(f, 3,42)	/* ����ͣ���²� */
#define P_val_close_fan_set_ht(f)                           P_g_i(f, 3,43)	/* ����ͣ���²� */
#define P_val_close_fan_D(f)                                P_g_i(f, 3,44)	/* ͣ��ز� */
#define P_val_freq_fan_cool_mid(f)                          P_g_i(f, 3,45)	/* ���������� */
#define P_val_freq_fan_heat_mid(f)                          P_g_i(f, 3,46)	/* ���ȷ������ */
#define val_freq_fan_set                                    (P_val(val_freq_fan_set                   ))/* ��Ƶ������� */
#define val_invt_fan_type                                   (P_val(val_invt_fan_type                  ))/* EC������� */
#define val_freq_fan_cool_max                               (P_val(val_freq_fan_cool_max              ))/* ��������� */
#define val_freq_fan_cool_min                               (P_val(val_freq_fan_cool_min              ))/* ��������� */
#define val_freq_fan_eco_adj                                (P_val(val_freq_fan_eco_adj               ))/* ���ܷ��ٵ��� */
#define val_freq_fan_ctrl                                   (P_val(val_freq_fan_ctrl                  ))/* ��Ƶ������� */
#define val_freq_fan_hand_speed                             (P_val(val_freq_fan_hand_speed            ))/* ����ֶ����� */
#define val_ivnt_prot_temp                               (10*P_val(val_ivnt_prot_temp                 ))/* �߷�ģ���¶� */
#define val_hig_fan_env_cold                             (10*P_val(val_hig_fan_env_cold               ))/* ����߷绷�� */
#define val_low_fan_env_cold                             (10*P_val(val_low_fan_env_cold               ))/* ����ͷ绷�� */
#define val_hig_fan_env_heat                             (10*P_val(val_hig_fan_env_heat               ))/* ���ȸ߷绷�� */
#define val_low_fan_env_heat                             (10*P_val(val_low_fan_env_heat               ))/* ���ȵͷ绷�� */
#define val_hig_fan_wing_cold                            (10*P_val(val_hig_fan_wing_cold              ))/* ����߷����� */
#define val_low_fan_wing_cold                            (10*P_val(val_low_fan_wing_cold              ))/* ����ͷ����� */
#define val_hig_fan_wing_heat                            (10*P_val(val_hig_fan_wing_heat              ))/* ���ȸ߷����� */
#define val_low_fan_wing_heat                            (10*P_val(val_low_fan_wing_heat              ))/* ���ȵͷ����� */
#define val_mid_fan_temp_heat                            (10*P_val(val_mid_fan_temp_heat              ))/* �����з��¶� */
#define val_fan_speed_period                                (P_val(val_fan_speed_period               ))/* ���ٵ������� */
#define val_fan_speed_percent                               (P_val(val_fan_speed_percent              ))/* ���ٶ������� */
#define val_freq_fan_heat_max                               (P_val(val_freq_fan_heat_max              ))/* ���ȷ����� */
#define val_freq_fan_heat_min                               (P_val(val_freq_fan_heat_min              ))/* ���ȷ����� */
#define val_fan_pwm_pulse                                   (P_val(val_fan_pwm_pulse                  ))/* PWM�������  */
#define val_freq_fan_strong_adj                             (P_val(val_freq_fan_strong_adj            ))/* ǿ�����ٵ��� */
#define val_ec_fan_base_addr                                (P_val(val_ec_fan_base_addr               ))/* EC�����ַ */
#define val_mid_fan_temp_cold                            (10*P_val(val_mid_fan_temp_cold              ))/* �����з��¶� */
#define val_freq_fan_str_silent_max                         (P_val(val_freq_fan_str_silent_max        ))/* ������������ */
#define val_freq_fan_num                                    (P_val(val_freq_fan_num                   ))/* ��Ƶ������� */
#define val_freq_fan_silent_max                             (P_val(val_freq_fan_silent_max            ))/* ���������� */
#define val_pwm_fan_init                                    (P_val(val_pwm_fan_init                   ))/* pwm���ٳ�ʼֵ */
#define val_pwm_fan_coef                                    (P_val(val_pwm_fan_coef                   ))/* pwm����ϵ�� */
#define val_pwm_fan_period                                  (P_val(val_pwm_fan_period                 ))/* pwm���ټ�� */
#define val_pwm_fan_area                                    (P_val(val_pwm_fan_area                   ))/* pwm�������� */
#define val_freq_fan_std_adj                                (P_val(val_freq_fan_std_adj               ))/* ��׼���ٵ��� */
#define val_avoid_cold                                      (P_val(val_avoid_cold                     ))/* ����繦�� */
#define val_send_fan_pause                                  (P_val(val_send_fan_pause                 ))/* �ͷ����ͣ */
#define val_avoid_cold_fan_time                             (P_val(val_avoid_cold_fan_time            ))/* �����ʱ�� */
#define val_freq_Cpst_Coef                                  (P_val(val_freq_Cpst_Coef                 ))/* Ƶ�ʲ���ϵ�� */
#define val_min_Cpst_Coef                                   (P_val(val_min_Cpst_Coef                  ))/* ����ϵ������ */
#define val_Tp_fan_rpm_corr_1                               (P_val(val_Tp_fan_rpm_corr_1              ))/* ���·�������1 */
#define val_Tp_fan_rpm_corr_2                               (P_val(val_Tp_fan_rpm_corr_2              ))/* ���·�������2 */
#define val_Tp_fan_rpm_corr_3                               (P_val(val_Tp_fan_rpm_corr_3              ))/* ���·�������3 */
#define val_Tp_fan_rpm_corr_set                             (P_val(val_Tp_fan_rpm_corr_set            ))/* ������������ */
#define val_close_fan_set_cl                                (P_val(val_close_fan_set_cl               ))/* ����ͣ���²� */
#define val_close_fan_set_ht                                (P_val(val_close_fan_set_ht               ))/* ����ͣ���²� */
#define val_close_fan_D                                  (10*P_val(val_close_fan_D                    ))/* ͣ��ز� */
#define val_freq_fan_cool_mid                               (P_val(val_freq_fan_cool_mid              ))/* ���������� */
#define val_freq_fan_heat_mid                               (P_val(val_freq_fan_heat_mid              ))/* ���ȷ������ */

#define val_close_fan_diff_cl                             (10*(val_close_fan_set_cl%100))/* ����ͣ���²� */
#define val_close_fan_diff_ht                             (10*(val_close_fan_set_ht%100))/* ����ͣ���²� */

#define val_close_fan_dly_cl                              (val_close_fan_set_cl/100)/* ����ͣ����ʱ */
#define val_close_fan_dly_ht                              (val_close_fan_set_ht/100)/* ����ͣ����ʱ */

#define val_Tp_fan_rpm_corr(i)                              (para_C[GROUP_FAN][38+i])

#define val_fan_speed_type                                  (FAN_SPEED_UNUSED)/* ��Ƶ������� */

/*05�������ͷ����� */
#define P_val_motor_use(f)                                  P_g_i(f, 4,0)	/* ����ʹ������ */
#define P_val_motor_fre_type(f)                             P_g_i(f, 4,1)	/* �������ŷ�ʽ */
#define P_val_motor_step_fre(f)                             P_g_i(f, 4,2)	/* ��������Ƶ�� */
#define P_val_motor_step_max(f)                             P_g_i(f, 4,3)	/* �����ܲ��� */
#define P_val_motor_force_step(f)                           P_g_i(f, 4,4)	/* ��ǿ�������� */
#define P_val_motor_max_od(f)                               P_g_i(f, 4,5)	/* ������󿪶� */
#define P_val_motor_sv_exh_cpst(f)                          P_g_i(f, 4,6)	/* �����������²� */
#define P_val_motor_manual_mode(f)                          P_g_i(f, 4,7)	/* �����ֶ�ģʽ */
#define P_val_motor_manual_steps(f)                         P_g_i(f, 4,8)	/* �����ֶ����� */
#define P_val_motor_sv_freq_cpst(f)                         P_g_i(f, 4,9)	/* ��������Ƶ�ʲ� */
#define P_val_s_motor_use(f)                                P_g_i(f, 4,10)	/* ����ʹ������ */
#define P_val_s_motor_fre_type(f)                           P_g_i(f, 4,11)	/* �������ŷ�ʽ */
#define P_val_s_motor_step_fre(f)                           P_g_i(f, 4,12)	/* ��������Ƶ�� */
#define P_val_s_motor_step_max(f)                           P_g_i(f, 4,13)	/* �����ܲ��� */
#define P_val_s_motor_force_step(f)                         P_g_i(f, 4,14)	/* ��ǿ�������� */
#define P_val_s_motor_max_od(f)                             P_g_i(f, 4,15)	/* ������󿪶� */
#define P_val_s_motor_min_od(f)                             P_g_i(f, 4,16)	/* ������С���� */
#define P_val_s_motor_manual_mode(f)                        P_g_i(f, 4,17)	/* �����ֶ�ģʽ */
#define P_val_s_motor_manual_steps(f)                       P_g_i(f, 4,18)	/* �����ֶ����� */
#define P_val_press_diff_in(f)                              P_g_i(f, 4,19)	/* �ͻ���ˮ���� */
#define P_val_press_diff_env(f)                             P_g_i(f, 4,20)	/* �ͻ���ˮ���� */
#define P_val_press_diff_eev(f)                             P_g_i(f, 4,21)	/* �ͻ���ˮ���� */
#define P_val_eev_step_refer(f)                             P_g_i(f, 4,22)	/* �����ο����� */
#define P_val_eev_refer_coef(f)                             P_g_i(f, 4,23)	/* �ο�����ϵ�� */
#define P_val_cool_sv_chg_env(f)                            P_g_i(f, 4,24)	/* �������ת���� */
#define P_val_low_env_sv_cool1(f)                           P_g_i(f, 4,25)	/* �ͻ��������1 */
#define P_val_low_env_sv_cool2(f)                           P_g_i(f, 4,26)	/* �ͻ��������2 */
#define P_val_low_env_sv_cool3(f)                           P_g_i(f, 4,27)	/* �ͻ��������3 */
#define P_val_eev_invt_rps_diff(f)                          P_g_i(f, 4,28)  /* ������Ƶ���� */
#define P_val_eev_invt_coef_up(f)                           P_g_i(f, 4,29)  /* ������Ƶϵ�� */
#define P_val_eev_invt_coef_dn(f)                           P_g_i(f, 4,30)  /* ������Ƶϵ�� */
#define P_val_init_step_chg_env_cl(f)                       P_g_i(f, 4,31)	/* ����������ƻ� */
#define P_val_motor_sv_heat10(f)                            P_g_i(f, 4,32)  /* ������������10 */
#define P_val_high_gas_eev_evi_order(f)                     P_g_i(f, 4,33)  /* �������������� */
#define val_motor_use                                       (P_val(val_motor_use                      ))/* ����ʹ������ */
#define val_motor_fre_type                                  (P_val(val_motor_fre_type                 ))/* �������ŷ�ʽ */
#define val_motor_step_fre                                  (P_val(val_motor_step_fre                 ))/* ��������Ƶ�� */
#define val_motor_step_max                                  (P_val(val_motor_step_max                 ))/* �����ܲ��� */
#define val_motor_force_step                                (P_val(val_motor_force_step               ))/* ��ǿ�������� */
#define val_motor_max_od                                    (P_val(val_motor_max_od                   ))/* ������󿪶� */
#define val_motor_sv_exh_cpst                               (P_val(val_motor_sv_exh_cpst              ))/* �����������²� */
#define val_motor_manual_mode                               (P_val(val_motor_manual_mode              ))/* �����ֶ�ģʽ */
#define val_motor_manual_steps                              (P_val(val_motor_manual_steps             ))/* �����ֶ����� */
#define val_motor_sv_freq_cpst                              (P_val(val_motor_sv_freq_cpst             ))/* ��������Ƶ�ʲ� */
#define val_s_motor_use                                     (P_val(val_s_motor_use                    ))/* ����ʹ������ */
#define val_s_motor_fre_type                                (P_val(val_s_motor_fre_type               ))/* �������ŷ�ʽ */
#define val_s_motor_step_fre                                (P_val(val_s_motor_step_fre               ))/* ��������Ƶ�� */
#define val_s_motor_step_max                                (P_val(val_s_motor_step_max               ))/* �����ܲ��� */
#define val_s_motor_force_step                              (P_val(val_s_motor_force_step             ))/* ��ǿ�������� */
#define val_s_motor_max_od                                  (P_val(val_s_motor_max_od                 ))/* ������󿪶� */
#define val_s_motor_min_od                                  (P_val(val_s_motor_min_od                 ))/* ������С���� */
#define val_s_motor_manual_mode                             (P_val(val_s_motor_manual_mode            ))/* �����ֶ�ģʽ */
#define val_s_motor_manual_steps                            (P_val(val_s_motor_manual_steps           ))/* �����ֶ����� */
#define val_press_diff_in                                (10*P_val(val_press_diff_in                  ))/* �ͻ���ˮ���� */
#define val_press_diff_env                               (10*P_val(val_press_diff_env                 ))/* �ͻ���ˮ���� */
#define val_press_diff_eev                                  (P_val(val_press_diff_eev                 ))/* �ͻ���ˮ���� */
#define val_eev_step_refer                                  (P_val(val_eev_step_refer                 ))/* �����ο����� */
#define val_eev_refer_coef                                  (P_val(val_eev_refer_coef                 ))/* �ο�����ϵ�� */
#define val_cool_sv_chg_env                              (10*P_val(val_cool_sv_chg_env                ))/* �������ת���� */
#define val_low_env_sv_cool1                                (P_val(val_low_env_sv_cool1               ))/* �ͻ��������1 */
#define val_low_env_sv_cool2                                (P_val(val_low_env_sv_cool2               ))/* �ͻ��������2 */
#define val_low_env_sv_cool3                                (P_val(val_low_env_sv_cool3               ))/* �ͻ��������3 */
#define val_eev_invt_rps_diff                               (P_val(val_eev_invt_rps_diff              ))/* ������Ƶ���� */
#define val_eev_invt_coef_up                                (P_val(val_eev_invt_coef_up               ))/* ������Ƶϵ�� */
#define val_eev_invt_coef_dn                                (P_val(val_eev_invt_coef_dn               ))/* ������Ƶϵ�� */
#define val_init_step_chg_env_cl                         (10*P_val(val_init_step_chg_env_cl           ))/* ����������ƻ� */
#define val_motor_sv_heat10                                 (P_val(val_motor_sv_heat10                ))/* ������������10 */
#define val_high_gas_eev_evi_order                          (P_val(val_high_gas_eev_evi_order         ))/* �������������� */

#define val_pid_d_type              (1)         // 1 ���Բ���6�Ƿ�ʹ��΢�����ͨ�˲�0:����ʹ�� 1:ʹ��
#define val_ave_stand               (6)         // ���Բ���7  4

#define EVI_RUN_STEP_MAX ((U32)val_s_motor_max_od*val_s_motor_step_max/100) // EVI������в���
#define EVI_RUN_STEP_MIN ((U32)val_s_motor_min_od*val_s_motor_step_max/1000)// EVI��С���в���

/* 06��·�������ͷ����� */
#define P_val_motor_idle_open(f)                            P_g_i(f, 5, 0)	/* �������� */
#define P_val_m_motor_start_time(f)                         P_g_i(f, 5, 1)	/* ����ά��ʱ�� */
#define P_val_motor_start_amplify_cool(f)                   P_g_i(f, 5, 2)	/* ��������Ŵ� */
#define P_val_motor_start_amplify_heat1(f)                  P_g_i(f, 5, 3)	/* ���ȳ����Ŵ�1 */
#define P_val_motor_start_amplify_heat2(f)                  P_g_i(f, 5, 4)	/* ���ȳ����Ŵ�2 */
#define P_val_motor_start_amplify_heat3(f)                  P_g_i(f, 5, 5)	/* ���ȳ����Ŵ�3 */
#define P_val_motor_start_change_heat1(f)                   P_g_i(f, 5, 6)	/* ���ȳ���ת��1������ */
#define P_val_motor_start_change_heat2(f)                   P_g_i(f, 5, 7)	/* ���ȳ���ת��2 */
#define P_val_cool_min_initial_od(f)                        P_g_i(f, 5, 8)	/* ����������� */
#define P_val_cool_max_initial_od(f)                        P_g_i(f, 5, 9)	/* ����������� */
#define P_val_heat_min_initial_od(f)                        P_g_i(f, 5,10)	/* ���ȳ������� */
#define P_val_heat_max_initial_od(f)                        P_g_i(f, 5,11)	/* ���ȳ������� */
#define P_val_motor_start_amp_EVI_off(f)                    P_g_i(f, 5,12)	/* �ظ������Ŵ� */
#define P_val_motor_sv_cool1(f)                             P_g_i(f, 5,13)	/* ������������1 */
#define P_val_motor_sv_cool2(f)                             P_g_i(f, 5,14)	/* ������������2 */
#define P_val_motor_sv_cool3(f)                             P_g_i(f, 5,15)	/* ������������3 */
#define P_val_cool_sv_chg_in1(f)                            P_g_i(f, 5,16)	/* �������ת��1������ */
#define P_val_cool_sv_chg_in2(f)                            P_g_i(f, 5,17)	/* �������ת��2 */
#define P_val_motor_sv_heat1(f)                             P_g_i(f, 5,18)	/* ������������1 */
#define P_val_motor_sv_heat2(f)                             P_g_i(f, 5,19)	/* ������������2 */
#define P_val_motor_sv_heat3(f)                             P_g_i(f, 5,20)	/* ������������3 */
#define P_val_motor_sv_heat4(f)                             P_g_i(f, 5,21)	/* ������������4 */
#define P_val_motor_sv_heat5(f)                             P_g_i(f, 5,22)	/* ������������5 */
#define P_val_motor_sv_heat6(f)                             P_g_i(f, 5,23)	/* ������������6 */
#define P_val_motor_sv_heat7(f)                             P_g_i(f, 5,24)	/* ������������7 */
#define P_val_motor_sv_heat8(f)                             P_g_i(f, 5,25)	/* ������������8 */
#define P_val_motor_sv_heat9(f)                             P_g_i(f, 5,26)	/* ������������9 */
#define P_val_max_evap_set(f)                               P_g_i(f, 5,27)	/* ǿ��С�������� */
#define P_val_m_defrost_od(f)                               P_g_i(f, 5,28)	/* ��˪���� */
#define P_val_m_limit_range(f)                              P_g_i(f, 5,29)	/* ���������޷� */
#define P_val_m_pid_p(f)                                    P_g_i(f, 5,30)	/* ������ */
#define P_val_m_pid_i(f)                                    P_g_i(f, 5,31)	/* ����ʱ�� */
#define P_val_m_pid_d(f)                                    P_g_i(f, 5,32)	/* ΢��ʱ�� */
#define P_val_m_pid_period(f)                               P_g_i(f, 5,33)	/* pid�������� */
#define P_val_m_pid_calculate_period(f)                     P_g_i(f, 5,34)	/* pid�������� */
#define P_val_m_pid_Kp(f)                                   P_g_i(f, 5,35)	/* ����ϵ�� */
#define P_val_motor_step_min_inht1(f)                       P_g_i(f, 5,36)	/* ������С����1 ʹ��ָ����� */
#define P_val_motor_step_min_inht2(f)                       P_g_i(f, 5,37)	/* ������С����2 */
#define P_val_motor_step_min_inht3(f)                       P_g_i(f, 5,38)	/* ������С����3 */
#define P_val_motor_step_min_inht4(f)                       P_g_i(f, 5,39)	/* ������С����4 */
#define P_val_motor_step_min_inht5(f)                       P_g_i(f, 5,40)	/* ������С����5 */
#define P_val_motor_step_min_incl1(f)                       P_g_i(f, 5,41)	/* ������С����1 ʹ��ָ����� */
#define P_val_motor_step_min_incl2(f)                       P_g_i(f, 5,42)	/* ������С����2 */
#define P_val_motor_step_min_incl3(f)                       P_g_i(f, 5,43)	/* ������С����3 */
#define P_val_motor_step_min_incl4(f)                       P_g_i(f, 5,44)	/* ������С����4 */
#define P_val_motor_step_min_incl5(f)                       P_g_i(f, 5,45)	/* ������С����5 */
#define P_val_motor_limit_down_wing(f)                      P_g_i(f, 5,46)	/* �����޹س��� */
#define P_val_motor_defr_init_cpst(f)                       P_g_i(f, 5,47)	/* ��˪�������� */
#define val_motor_idle_open                                 (P_val(val_motor_idle_open                ))/* �������� */
#define val_m_motor_start_time                              (P_val(val_m_motor_start_time             ))/* ����ά��ʱ�� */
#define val_motor_start_amplify_cool                        (P_val(val_motor_start_amplify_cool       ))/* ��������Ŵ� */
#define val_motor_start_amplify_heat1                       (P_val(val_motor_start_amplify_heat1      ))/* ���ȳ����Ŵ�1 */
#define val_motor_start_amplify_heat2                       (P_val(val_motor_start_amplify_heat2      ))/* ���ȳ����Ŵ�2 */
#define val_motor_start_amplify_heat3                       (P_val(val_motor_start_amplify_heat3      ))/* ���ȳ����Ŵ�3 */
#define val_motor_start_change_heat1                     (10*P_val(val_motor_start_change_heat1       ))/* ���ȳ���ת��1������ */
#define val_motor_start_change_heat2                     (10*P_val(val_motor_start_change_heat2       ))/* ���ȳ���ת��2 */
#define val_cool_min_initial_od                             (P_val(val_cool_min_initial_od            ))/* ����������� */
#define val_cool_max_initial_od                             (P_val(val_cool_max_initial_od            ))/* ����������� */
#define val_heat_min_initial_od                             (P_val(val_heat_min_initial_od            ))/* ���ȳ������� */
#define val_heat_max_initial_od                             (P_val(val_heat_max_initial_od            ))/* ���ȳ������� */
#define val_motor_start_amp_EVI_off                         (P_val(val_motor_start_amp_EVI_off        ))/* �ظ������Ŵ� */
#define val_motor_sv_cool1                                  (P_val(val_motor_sv_cool1                 ))/* ������������1 */
#define val_motor_sv_cool2                                  (P_val(val_motor_sv_cool2                 ))/* ������������2 */
#define val_motor_sv_cool3                                  (P_val(val_motor_sv_cool3                 ))/* ������������3 */
#define val_cool_sv_chg_in1                              (10*P_val(val_cool_sv_chg_in1                ))/* �������ת��1������ */
#define val_cool_sv_chg_in2                              (10*P_val(val_cool_sv_chg_in2                ))/* �������ת��2 */
#define val_motor_sv_heat1                                  (P_val(val_motor_sv_heat1                 ))/* ������������1 */
#define val_motor_sv_heat2                                  (P_val(val_motor_sv_heat2                 ))/* ������������2 */
#define val_motor_sv_heat3                                  (P_val(val_motor_sv_heat3                 ))/* ������������3 */
#define val_motor_sv_heat4                                  (P_val(val_motor_sv_heat4                 ))/* ������������4 */
#define val_motor_sv_heat5                                  (P_val(val_motor_sv_heat5                 ))/* ������������5 */
#define val_motor_sv_heat6                                  (P_val(val_motor_sv_heat6                 ))/* ������������6 */
#define val_motor_sv_heat7                                  (P_val(val_motor_sv_heat7                 ))/* ������������7 */
#define val_motor_sv_heat8                                  (P_val(val_motor_sv_heat8                 ))/* ������������8 */
#define val_motor_sv_heat9                                  (P_val(val_motor_sv_heat9                 ))/* ������������9 */
#define val_max_evap_set                                    (P_val(val_max_evap_set                   ))/* ǿ��С�������� */
#define val_m_defrost_od                                    (P_val(val_m_defrost_od                   ))/* ��˪���� */
#define val_m_limit_range                                   (P_val(val_m_limit_range                  ))/* ���������޷� */
#define val_m_pid_p                                         (P_val(val_m_pid_p                        ))/* ������ */
#define val_m_pid_i                                         (P_val(val_m_pid_i                        ))/* ����ʱ�� */
#define val_m_pid_d                                         (P_val(val_m_pid_d                        ))/* ΢��ʱ�� */
#define val_m_pid_period                                    (P_val(val_m_pid_period                   ))/* pid�������� */
#define val_m_pid_calculate_period                          (P_val(val_m_pid_calculate_period         ))/* pid�������� */
#define val_m_pid_Kp                                        (P_val(val_m_pid_Kp                       ))/* ����ϵ�� */
#define val_motor_step_min_inht1                            (P_val(val_motor_step_min_inht1           ))/* ������С����1 ʹ��ָ����� */
#define val_motor_step_min_inht2                            (P_val(val_motor_step_min_inht2           ))/* ������С����2 */
#define val_motor_step_min_inht3                            (P_val(val_motor_step_min_inht3           ))/* ������С����3 */
#define val_motor_step_min_inht4                            (P_val(val_motor_step_min_inht4           ))/* ������С����4 */
#define val_motor_step_min_inht5                            (P_val(val_motor_step_min_inht5           ))/* ������С����5 */
#define val_motor_step_min_incl1                            (P_val(val_motor_step_min_incl1           ))/* ������С����1 ʹ��ָ����� */
#define val_motor_step_min_incl2                            (P_val(val_motor_step_min_incl2           ))/* ������С����2 */
#define val_motor_step_min_incl3                            (P_val(val_motor_step_min_incl3           ))/* ������С����3 */
#define val_motor_step_min_incl4                            (P_val(val_motor_step_min_incl4           ))/* ������С����4 */
#define val_motor_step_min_incl5                            (P_val(val_motor_step_min_incl5           ))/* ������С����5 */
#define val_motor_limit_down_wing                        (10*P_val(val_motor_limit_down_wing          ))/* �����޹س��� */
#define val_motor_defr_init_cpst                            (P_val(val_motor_defr_init_cpst           ))/* ��˪�������� */

#define val_max_evap_temp                                (10*(val_max_evap_set%100))/* ǿ��С�������� */
#define val_evap_high_err                                (10*(val_max_evap_set/100))/* �������ޱ���ֵ */

#define val_motor_step_min_inht(x)          (*(&val_motor_step_min_inht1+(x)))
#define val_motor_step_min_incl(x)          (*(&val_motor_step_min_incl1+(x)))

#define val_m_pid_flexi                     (10)   // ����ϵ��

/* 7 ��·�������� */
#define P_val_s_motor_use_heat(f)                           P_g_i(f, 6,0)	/* ���������ȿ��� */
#define P_val_s_motor_use_cool(f)                           P_g_i(f, 6,1)	/* ���������俪�� */
#define P_val_evi_open_dly(f)                               P_g_i(f, 6,2)	/* ����������ʱ */
#define P_val_evi_open_env_heat(f)                          P_g_i(f, 6,3)	/* ���ȿ������� */
#define P_val_evi_open_exh(f)                               P_g_i(f, 6,4)	/* ������������ */
#define P_val_evi_open_env_cool(f)                          P_g_i(f, 6,5)	/* ���俪������ */
#define P_val_s_motor_sv_dif(f)                             P_g_i(f, 6,6)	/* ��·���ȶ�ƫ�� */
#define P_val_evi_cool_init_od(f)                           P_g_i(f, 6,7)	/* �����ʼ����(EVI) */
#define P_val_evi_heat_init_od1(f)                          P_g_i(f, 6,8)	/* ���ȳ�ʼ����1(EVI) */
#define P_val_evi_heat_init_od2(f)                          P_g_i(f, 6,9)	/* ���ȳ�ʼ����2 */
#define P_val_evi_heat_init_od3(f)                          P_g_i(f, 6,10)	/* ���ȳ�ʼ����3 */
#define P_val_evi_heat_init_od4(f)                          P_g_i(f, 6,11)	/* ���ȳ�ʼ����4 */
#define P_val_evi_open_exh_overheat_cool(f)                 P_g_i(f, 6,12)	/* ���俪���Ź� */
#define P_val_evi_open_exh_overheat_heat(f)                 P_g_i(f, 6,13)	/* ���ȿ����Ź� */
#define P_val_s_motion_period(f)                            P_g_i(f, 6,14)	/* ���� */
#define P_val_s_pid_p(f)                                    P_g_i(f, 6,15)	/* ������ */
#define P_val_s_pid_i(f)                                    P_g_i(f, 6,16)	/* ����ʱ�� */
#define P_val_s_pid_d(f)                                    P_g_i(f, 6,17)	/* ΢��ʱ�� */
#define P_val_s_pid_period(f)                               P_g_i(f, 6,18)	/* pid�������� */
#define P_val_s_pid_calculate_period(f)                     P_g_i(f, 6,19)	/* pid�������� */
#define P_val_s_pid_Kp(f)                                   P_g_i(f, 6,20)	/* ����ϵ�� */
#define P_val_s_pid_flexi(f)                                P_g_i(f, 6,21)	/* ����ϵ�� */
#define P_val_e_ext_1(f)                                    P_g_i(f, 6,22)	/* ���ȶ�ת��1   */
#define P_val_e_ext_2(f)                                    P_g_i(f, 6,23)	/* ���ȶ�ת��2   */
#define P_val_e_sv_1(f)                                     P_g_i(f, 6,24)	/* ���ȶ��趨1   */
#define P_val_e_sv_2(f)                                     P_g_i(f, 6,25)	/* ���ȶ��趨2   */
#define P_val_exh_sv_max(f)                                 P_g_i(f, 6,26)	/* ���Ŀ������ */
#define P_val_s_exh_sv_cold(f)                              P_g_i(f, 6,27)	/* ������������ */
#define P_val_gas_overheat_k(f)                             P_g_i(f, 6,28)	/* �Ź�ˮ��ϵ�� */
#define P_val_gas_overheat_a1(f)                            P_g_i(f, 6,29)	/* ������������1 */
#define P_val_gas_overheat_a2(f)                            P_g_i(f, 6,30)	/* ������������2 */
#define P_val_gas_overheat_a3(f)                            P_g_i(f, 6,31)	/* ������������3 */
#define P_val_gas_overheat_a4(f)                            P_g_i(f, 6,32)	/* ������������4 */
#define P_val_gas_overheat_a5(f)                            P_g_i(f, 6,33)	/* ������������5 */
#define P_val_evi_open_rps(f)                               P_g_i(f, 6,34)	/* ��������ת�� */
#define P_val_evi_close_rpg(f)                              P_g_i(f, 6,35)	/* �����ر�ת�� */
#define P_val_evi_open_exh_overheat_diff(f)                 P_g_i(f, 6,36)	/* �������Ź�ƫ�� */
#define P_val_evi_open_exh_diff(f)                          P_g_i(f, 6,37)	/* ����������ƫ�� */
#define P_val_evi_for_liquid(f)                             P_g_i(f, 6,38)	/* ��������Һ�� */
#define P_val_ability_test_s_motor_ctrl_HA(f)               P_g_i(f, 6,39)	/* ���ȸ�������A */
#define P_val_ability_test_s_motor_ctrl_HB(f)               P_g_i(f, 6,40)	/* ���ȸ�������B */
#define P_val_ability_test_s_motor_ctrl_HE(f)               P_g_i(f, 6,41)	/* ���ȸ�������E */
#define P_val_ability_test_s_motor_ctrl_HG(f)               P_g_i(f, 6,42)	/* ���ȸ�������G */
#define P_val_ability_test_s_motor_ctrl_HC(f)               P_g_i(f, 6,43)	/* ���ȸ�������C */
#define P_val_ability_test_s_motor_ctrl_HD(f)               P_g_i(f, 6,44)	/* ���ȸ�������D */
#define P_val_ability_test_s_motor_ctrl_C(f)                P_g_i(f, 6,45)	/* ���丨������  */
#define val_s_motor_use_heat                                (P_val(val_s_motor_use_heat               ))/* ���������ȿ��� */
#define val_s_motor_use_cool                                (P_val(val_s_motor_use_cool               ))/* ���������俪�� */
#define val_evi_open_dly                                    (P_val(val_evi_open_dly                   ))/* ����������ʱ */
#define val_evi_open_env_heat                            (10*P_val(val_evi_open_env_heat              ))/* ���ȿ������� */
#define val_evi_open_exh                                 (10*P_val(val_evi_open_exh                   ))/* ������������ */
#define val_evi_open_env_cool                            (10*P_val(val_evi_open_env_cool              ))/* ���俪������ */
#define val_s_motor_sv_dif                               (10*P_val(val_s_motor_sv_dif                 ))/* ��·���ȶ�ƫ��        */
#define val_evi_cool_init_od                                (P_val(val_evi_cool_init_od               ))/* �����ʼ����(EVI) */
#define val_evi_heat_init_od1                               (P_val(val_evi_heat_init_od1              ))/* ���ȳ�ʼ����1(EVI) */
#define val_evi_heat_init_od2                               (P_val(val_evi_heat_init_od2              ))/* ���ȳ�ʼ����2 */
#define val_evi_heat_init_od3                               (P_val(val_evi_heat_init_od3              ))/* ���ȳ�ʼ����3 */
#define val_evi_heat_init_od4                               (P_val(val_evi_heat_init_od4              ))/* ���ȳ�ʼ����4 */
#define val_evi_open_exh_overheat_cool                   (10*P_val(val_evi_open_exh_overheat_cool     ))/* ���俪���Ź� */
#define val_evi_open_exh_overheat_heat                   (10*P_val(val_evi_open_exh_overheat_heat     ))/* ���ȿ����Ź� */
#define val_s_motion_period                                 (P_val(val_s_motion_period                ))/* ���� */
#define val_s_pid_p                                         (P_val(val_s_pid_p                        ))/* ������ */
#define val_s_pid_i                                         (P_val(val_s_pid_i                        ))/* ����ʱ�� */
#define val_s_pid_d                                         (P_val(val_s_pid_d                        ))/* ΢��ʱ�� */
#define val_s_pid_period                                    (P_val(val_s_pid_period                   ))/* pid�������� */
#define val_s_pid_calculate_period                          (P_val(val_s_pid_calculate_period         ))/* pid�������� */
#define val_s_pid_Kp                                        (P_val(val_s_pid_Kp                       ))/* ����ϵ�� */
#define val_s_pid_flexi                                     (P_val(val_s_pid_flexi                    ))/* ����ϵ�� */
#define val_e_ext_1                                      (10*P_val(val_e_ext_1                        ))/* ���ȶ�ת��1   */
#define val_e_ext_2                                      (10*P_val(val_e_ext_2                        ))/* ���ȶ�ת��2   */
#define val_e_sv_1                                          (P_val(val_e_sv_1                         ))/* ���ȶ��趨1   */
#define val_e_sv_2                                          (P_val(val_e_sv_2                         ))/* ���ȶ��趨2   */
#define val_exh_sv_max                                   (10*P_val(val_exh_sv_max                     ))/* ���Ŀ������ */
#define val_s_exh_sv_cold                                (10*P_val(val_s_exh_sv_cold                  ))/* ������������ */
#define val_gas_overheat_k                                  (P_val(val_gas_overheat_k                 ))/* �Ź�ˮ��ϵ�� */
#define val_gas_overheat_a1                                 (P_val(val_gas_overheat_a1                ))/* ������������1 */
#define val_gas_overheat_a2                                 (P_val(val_gas_overheat_a2                ))/* ������������2 */
#define val_gas_overheat_a3                                 (P_val(val_gas_overheat_a3                ))/* ������������3 */
#define val_gas_overheat_a4                                 (P_val(val_gas_overheat_a4                ))/* ������������4 */
#define val_gas_overheat_a5                                 (P_val(val_gas_overheat_a5                ))/* ������������5 */
#define val_evi_open_rps                                (100*P_val(val_evi_open_rps                   ))/* ��������ת�� */
#define val_evi_close_rpg                               (100*P_val(val_evi_close_rpg                  ))/* �����ر�ת�� */
#define val_evi_open_exh_overheat_diff                   (10*P_val(val_evi_open_exh_overheat_diff     ))/* �������Ź�ƫ�� */
#define val_evi_open_exh_diff                            (10*P_val(val_evi_open_exh_diff              ))/* ����������ƫ�� */
#define val_evi_for_liquid                                  (P_val(val_evi_for_liquid                 ))/* ��������Һ�� */
#define val_ability_test_s_motor_ctrl_HA                    (P_val(val_ability_test_s_motor_ctrl_HA   ))/* ���ȸ�������A */
#define val_ability_test_s_motor_ctrl_HB                    (P_val(val_ability_test_s_motor_ctrl_HB   ))/* ���ȸ�������B */
#define val_ability_test_s_motor_ctrl_HE                    (P_val(val_ability_test_s_motor_ctrl_HE   ))/* ���ȸ�������E */
#define val_ability_test_s_motor_ctrl_HG                    (P_val(val_ability_test_s_motor_ctrl_HG   ))/* ���ȸ�������G */
#define val_ability_test_s_motor_ctrl_HC                    (P_val(val_ability_test_s_motor_ctrl_HC   ))/* ���ȸ�������C */
#define val_ability_test_s_motor_ctrl_HD                    (P_val(val_ability_test_s_motor_ctrl_HD   ))/* ���ȸ�������D */
#define val_ability_test_s_motor_ctrl_C                     (P_val(val_ability_test_s_motor_ctrl_C    ))/* ���丨������  */

/* 8��˪�������� */
#define P_val_defrost_first_run_period(f)                   P_g_i(f, 7, 0)	/* �״γ�˪��� */
#define P_val_defrost_time(f)                               P_g_i(f, 7, 1)	/* ��˪ʱ�� */
#define P_val_defrost_time_sel(f)                           P_g_i(f, 7, 2)	/* ��˪��ʱѡ�� */
#define P_val_defrost_allow_environ(f)                      P_g_i(f, 7, 3)	/* �����˪���� */
#define P_val_defrost_allow_wing(f)                         P_g_i(f, 7, 4)	/* �����˪���� */
#define P_val_defrost_end_wing(f)                           P_g_i(f, 7, 5)	/* �˳�˪���� */
#define P_val_defrost_end_out(f)                            P_g_i(f, 7, 6)	/* �˳�˪��ˮ�¶� */
#define P_val_defrost_env_wing_rise_en(f)                   P_g_i(f, 7, 7)	/* �������ܳ�˪ */
#define P_val_defrost_EWD_rise_diff1(f)                     P_g_i(f, 7, 8)	/* �жȽ�˪�²� */
#define P_val_defrost_EWD_rise_diff2(f)                     P_g_i(f, 7, 9)	/* �ضȽ�˪�²� */
#define P_val_defrost_temp_sure_time(f)                     P_g_i(f, 7,10)	/* �¶�ȷ��ʱ�� */
#define P_val_defrost_environ1(f)                           P_g_i(f, 7,11)	/* ��˪����1 */
#define P_val_defrost_environ2(f)                           P_g_i(f, 7,12)	/* ��˪����2 */
#define P_val_defrost_environ3(f)                           P_g_i(f, 7,13)	/* ��˪����3 */
#define P_val_defrost_environ4(f)                           P_g_i(f, 7,14)	/* ��˪����4 */
#define P_val_defrost_period1(f)                            P_g_i(f, 7,15)	/* ��˪���1 */
#define P_val_defrost_period2(f)                            P_g_i(f, 7,16)	/* ��˪���2 */
#define P_val_defrost_period3(f)                            P_g_i(f, 7,17)	/* ��˪���3 */
#define P_val_defrost_period4(f)                            P_g_i(f, 7,18)	/* ��˪���4 */
#define P_val_defrost_period5(f)                            P_g_i(f, 7,19)	/* ��˪���5 */
#define P_val_defrost_allow_diff1(f)                        P_g_i(f, 7,20)	/* ��˪�����1 */
#define P_val_defrost_allow_diff2(f)                        P_g_i(f, 7,21)	/* ��˪�����2 */
#define P_val_defrost_allow_diff3(f)                        P_g_i(f, 7,22)	/* ��˪�����3 */
#define P_val_defrost_allow_diff4(f)                        P_g_i(f, 7,23)	/* ��˪�����4 */
#define P_val_defrost_allow_diff5(f)                        P_g_i(f, 7,24)	/* ��˪�����5 */
#define P_val_defrost_LP(f)                                 P_g_i(f, 7,25)	/* ��ѹ��˪���� */
#define P_val_defrost_lp_dly(f)                             P_g_i(f, 7,26)	/* ��ѹ��˪�����ʱ */
#define P_val_defrost_lp_time(f)                            P_g_i(f, 7,27)	/* ��ѹ��˪��� */
#define P_val_defrost_lp_wing(f)                            P_g_i(f, 7,28)	/* ��ѹ��˪���� */
#define P_val_defrost_longtime(f)                           P_g_i(f, 7,29)	/* ��ʱ��˪��� */
#define P_val_defrost_emergency_diff(f)                     P_g_i(f, 7,30)	/* ������˪���� */
#define P_val_defrost_stop_cm_en(f)                         P_g_i(f, 7,31)	/* ��˪ͣ���з� */
#define P_val_defrost_stop_cm_tm_entry(f)                   P_g_i(f, 7,32)	/* ����˪ѹ����ʱ */
#define P_val_defrost_four_tm_entry(f)                      P_g_i(f, 7,33)	/* ����˪��ͨ��ʱ */
#define P_val_defrost_stop_cm_tm_exit(f)                    P_g_i(f, 7,34)	/* �˳�˪ѹ����ʱ */
#define P_val_defrost_four_tm_exit(f)                       P_g_i(f, 7,35)	/* �˳�˪��ͨ��ʱ */
#define P_val_defrost_chg_freq(f)                           P_g_i(f, 7,36)	/* ��˪�з�Ƶ�� */
#define P_val_defrost_dripping_time(f)                      P_g_i(f, 7,37)	/* ͣ���ˮʱ�� */
#define P_val_defrost_min_time(f)                           P_g_i(f, 7,38)	/* ��С��˪��� */
#define P_val_defrost_emergency_time(f)                     P_g_i(f, 7,39)	/* ������˪��� */
#define val_defrost_first_run_period                     (60*P_val(val_defrost_first_run_period       ))/* �״γ�˪��� */
#define val_defrost_time                                    (P_val(val_defrost_time                   ))/* ��˪ʱ�� */
#define val_defrost_time_sel                                (P_val(val_defrost_time_sel               ))/* ��˪��ʱѡ�� */
#define val_defrost_allow_environ                        (10*P_val(val_defrost_allow_environ          ))/* �����˪���� */
#define val_defrost_allow_wing                           (10*P_val(val_defrost_allow_wing             ))/* �����˪���� */
#define val_defrost_end_wing                             (10*P_val(val_defrost_end_wing               ))/* �˳�˪���� */
#define val_defrost_end_out                              (10*P_val(val_defrost_end_out                ))/* �˳�˪��ˮ�¶� */
#define val_defrost_env_wing_rise_en                        (P_val(val_defrost_env_wing_rise_en       ))/* �������ܳ�˪ */
#define val_defrost_EWD_rise_diff1                       (10*P_val(val_defrost_EWD_rise_diff1         ))/* �жȽ�˪�²� */
#define val_defrost_EWD_rise_diff2                       (10*P_val(val_defrost_EWD_rise_diff2         ))/* �ضȽ�˪�²� */
#define val_defrost_temp_sure_time                       (60*P_val(val_defrost_temp_sure_time         ))/* �¶�ȷ��ʱ�� */
#define val_defrost_environ1                             (10*P_val(val_defrost_environ1               ))/* ��˪����1 */
#define val_defrost_environ2                             (10*P_val(val_defrost_environ2               ))/* ��˪����2 */
#define val_defrost_environ3                             (10*P_val(val_defrost_environ3               ))/* ��˪����3 */
#define val_defrost_environ4                             (10*P_val(val_defrost_environ4               ))/* ��˪����4 */
#define val_defrost_period1                              (60*P_val(val_defrost_period1                ))/* ��˪���1 */
#define val_defrost_period2                              (60*P_val(val_defrost_period2                ))/* ��˪���2 */
#define val_defrost_period3                              (60*P_val(val_defrost_period3                ))/* ��˪���3 */
#define val_defrost_period4                              (60*P_val(val_defrost_period4                ))/* ��˪���4 */
#define val_defrost_period5                              (60*P_val(val_defrost_period5                ))/* ��˪���5 */
#define val_defrost_allow_diff1                          (10*P_val(val_defrost_allow_diff1            ))/* ��˪�����1 */
#define val_defrost_allow_diff2                          (10*P_val(val_defrost_allow_diff2            ))/* ��˪�����2 */
#define val_defrost_allow_diff3                          (10*P_val(val_defrost_allow_diff3            ))/* ��˪�����3 */
#define val_defrost_allow_diff4                          (10*P_val(val_defrost_allow_diff4            ))/* ��˪�����4 */
#define val_defrost_allow_diff5                          (10*P_val(val_defrost_allow_diff5            ))/* ��˪�����5 */
#define val_defrost_LP                                      (P_val(val_defrost_LP                     ))/* ��ѹ��˪���� */
#define val_defrost_lp_dly                               (60*P_val(val_defrost_lp_dly                 ))/* ��ѹ��˪�����ʱ */
#define val_defrost_lp_time                              (60*P_val(val_defrost_lp_time                ))/* ��ѹ��˪��� */
#define val_defrost_lp_wing                              (10*P_val(val_defrost_lp_wing                ))/* ��ѹ��˪���� */
#define val_defrost_longtime                           (3600*P_val(val_defrost_longtime               ))/* ��ʱ��˪��� */
#define val_defrost_emergency_diff                       (10*P_val(val_defrost_emergency_diff         ))/* ������˪���� */
#define val_defrost_stop_cm_en                              (P_val(val_defrost_stop_cm_en             ))/* ��˪ͣ���з� */
#define val_defrost_stop_cm_tm_entry                        (P_val(val_defrost_stop_cm_tm_entry       ))/* ����˪ѹ����ʱ */
#define val_defrost_four_tm_entry                           (P_val(val_defrost_four_tm_entry          ))/* ����˪��ͨ��ʱ */
#define val_defrost_stop_cm_tm_exit                         (P_val(val_defrost_stop_cm_tm_exit        ))/* �˳�˪ѹ����ʱ */
#define val_defrost_four_tm_exit                            (P_val(val_defrost_four_tm_exit           ))/* �˳�˪��ͨ��ʱ */
#define val_defrost_chg_freq                            (100*P_val(val_defrost_chg_freq               ))/* ��˪�з�Ƶ�� */
#define val_defrost_dripping_time                           (P_val(val_defrost_dripping_time          ))/* ͣ���ˮʱ�� */
#define val_defrost_min_time                             (60*P_val(val_defrost_min_time               ))/* ��С��˪��� */
#define val_defrost_emergency_time                       (60*P_val(val_defrost_emergency_time         ))/* ������˪��� */

#define val_err_emerg_defrost_use       (TRUE)              // ������˪Ƶ��ʹ��
#define val_defrost_env_wing_upper      (200)               /* ��������� 20.0  */
#define val_defrost_env_wing_lower      (-50)               /* ��������� -5.0  */

#define val_timingdfr_temp_in1          (250)				/* ��ʱ��˪����1 */
#define val_timingdfr_temp_in2          (400)				/* ��ʱ��˪����2 */
#define val_defrost_longtime1           (40*60)				/* ��ʱ��˪���1 */
#define val_defrost_longtime2           (60*60)				/* ��ʱ��˪���2 */
#define val_defrost_longtime3           (90*60)				/* ��ʱ��˪���3 */
#define val_exit_dfr_wingT2		        (30)                /* �˳�˪����2 */
#define val_exit_dfr_wingT2_time        (150)				/* �˳�˪����ά�� */

/* 9������������ */
#define P_val_anti_off_time1(f)                             P_g_i(f, 8,0)  /* �������1 */
#define P_val_anti_off_time2(f)                             P_g_i(f, 8,1)  /* �������2 */
#define P_val_anti_pump_on(f)                               P_g_i(f, 8,2)  /* ˮ�÷����¶� */
#define P_val_anti_heat_on(f)                               P_g_i(f, 8,3)  /* ���ȷ����¶� */
#define P_val_anti_comp_on(f)                               P_g_i(f, 8,4)  /* ѹ�������¶� */
#define P_val_anti_heat_off(f)                              P_g_i(f, 8,5)  /* �����˷����¶� */
#define P_val_anti_comp_off(f)                              P_g_i(f, 8,6)  /* ѹ���˷����¶� */
#define P_val_anti_embient(f)                               P_g_i(f, 8,7)  /* ����������� */
#define P_val_anti_env_in(f)                                P_g_i(f, 8,8)  /* ���������� */
#define P_val_anti_env_diff(f)                              P_g_i(f, 8,9)  /* �˷������²� */
#define P_val_anti_used(f)                                  P_g_i(f, 8,10)  /* ������������ */
#define P_val_PUMPsl_antifreeze(f)                          P_g_i(f, 8,11)  /* ̫����ˮ�÷��� */
#define P_val_timed_antifreeze_interval(f)                  P_g_i(f, 8,12)  /* ��ʱ������� */
#define P_val_timed_antifreeze_max(f)                       P_g_i(f, 8,13)  /* ��ʱ����ʱ�� */
#define P_val_anti_bak7(f)                                  P_g_i(f, 8,14)  /* ���� */
#define P_val_anti_bak8(f)                                  P_g_i(f, 8,15)  /* ���� */
#define P_val_anti_bak9(f)                                  P_g_i(f, 8,16)  /* ���� */
#define P_val_anti_bak10(f)                                 P_g_i(f, 8,17)  /* ���� */
#define P_val_anti_bak11(f)                                 P_g_i(f, 8,18)  /* ���� */
#define P_val_anti_bak12(f)                                 P_g_i(f, 8,19)  /* ���� */
#define P_val_anti_bak13(f)                                 P_g_i(f, 8,20)  /* ���� */
#define P_val_anti_bak14(f)                                 P_g_i(f, 8,21)  /* ���� */
#define P_val_anti_bak15(f)                                 P_g_i(f, 8,22)  /* ���� */
#define P_val_anti_bak16(f)                                 P_g_i(f, 8,23)  /* ���� */
#define P_val_anti_bak17(f)                                 P_g_i(f, 8,24)  /* ���� */
#define P_val_anti_bak18(f)                                 P_g_i(f, 8,25)  /* ���� */
#define P_val_anti_bak19(f)                                 P_g_i(f, 8,26)  /* ���� */
#define P_val_anti_bak20(f)                                 P_g_i(f, 8,27)  /* ���� */
#define P_val_anti_bak22(f)                                 P_g_i(f, 8,28)  /* ���� */
#define P_val_anti_bak23(f)                                 P_g_i(f, 8,29)  /* ���� */
#define P_val_anti_bak24(f)                                 P_g_i(f, 8,30)  /* ���� */
#define P_val_heat_chg_env1(f)                           	P_g_i(f, 8,31)  /* ����ת����1 */
#define P_val_heat_chg_env2(f)                           	P_g_i(f, 8,32)  /* ����ת����2 */
#define P_val_heat_chg_env3(f)                           	P_g_i(f, 8,33)  /* ����ת����3 */
#define P_val_heat_chg_env4(f)                           	P_g_i(f, 8,34)  /* ����ת����4 */
#define P_val_heat_chg_env5(f)                           	P_g_i(f, 8,35)  /* ����ת����5 */

#define val_anti_off_time1                               (60*P_val(val_anti_off_time1                 ))  /* �������1      */
#define val_anti_off_time2                               (60*P_val(val_anti_off_time2                 ))  /* �������2      */
#define val_anti_pump_on                                 (10*P_val(val_anti_pump_on                   ))  /* ˮ�÷����¶�   */
#define val_anti_heat_on                                 (10*P_val(val_anti_heat_on                   ))  /* ���ȷ����¶�   */
#define val_anti_comp_on                                 (10*P_val(val_anti_comp_on                   ))  /* ѹ�������¶�   */
#define val_anti_heat_off                                (10*P_val(val_anti_heat_off                  ))  /* �����˷����¶� */
#define val_anti_comp_off                                (10*P_val(val_anti_comp_off                  ))  /* ѹ���˷����¶� */
#define val_anti_embient                                 (10*P_val(val_anti_embient                   ))  /* �����������   */
#define val_anti_env_in                                  (10*P_val(val_anti_env_in                    ))  /* ����������     */
#define val_anti_env_diff                                (10*P_val(val_anti_env_diff                  ))  /* �˷������²�   */
#define val_anti_used                                       (P_val(val_anti_used                      ))  /* ������������   */
#define val_PUMPsl_antifreeze                               (P_val(val_PUMPsl_antifreeze              ))  /* ̫����ˮ�÷��� */
#define val_timed_antifreeze_interval                    (60*P_val(val_timed_antifreeze_interval      ))  /* ��ʱ������� */
#define val_timed_antifreeze_max                         (60*P_val(val_timed_antifreeze_max           ))  /* ��ʱ����ʱ�� */
#define val_anti_bak7                                       (P_val(val_anti_bak7                      ))  /* ���� */
#define val_anti_bak8                                       (P_val(val_anti_bak8                      ))  /* ���� */
#define val_anti_bak9                                       (P_val(val_anti_bak9                      ))  /* ���� */
#define val_anti_bak10                                 		(P_val(val_anti_bak10                     ))  /* ���� */
#define val_anti_bak11                                 		(P_val(val_anti_bak11                     ))  /* ���� */
#define val_anti_bak12                                 		(P_val(val_anti_bak12                     ))  /* ���� */
#define val_anti_bak13                                 		(P_val(val_anti_bak13                     ))  /* ���� */
#define val_anti_bak14                                 		(P_val(val_anti_bak14                     ))  /* ���� */
#define val_anti_bak15                                 		(P_val(val_anti_bak15                     ))  /* ���� */
#define val_anti_bak16                                 		(P_val(val_anti_bak16                     ))  /* ���� */
#define val_anti_bak17                                 		(P_val(val_anti_bak17                     ))  /* ���� */
#define val_anti_bak18                                 		(P_val(val_anti_bak18                     ))  /* ���� */
#define val_anti_bak19                                 		(P_val(val_anti_bak19                     ))  /* ���� */
#define val_anti_bak20                                 		(P_val(val_anti_bak20                     ))  /* ���� */
#define val_anti_bak22                                 		(P_val(val_anti_bak22                     ))  /* ���� */
#define val_anti_bak23                                 		(P_val(val_anti_bak23                     ))  /* ���� */
#define val_anti_bak24                                 		(P_val(val_anti_bak24                     ))  /* ���� */
#define val_heat_chg_env1                           		(10*P_val(val_heat_chg_env1            	  ))  /* ����ת����1 */
#define val_heat_chg_env2                           		(10*P_val(val_heat_chg_env2               ))  /* ����ת����2 */
#define val_heat_chg_env3                           		(10*P_val(val_heat_chg_env3               ))  /* ����ת����3 */
#define val_heat_chg_env4                           		(10*P_val(val_heat_chg_env4               ))  /* ����ת����4 */
#define val_heat_chg_env5                           		(10*P_val(val_heat_chg_env5               ))  /* ����ת����5 */

/* 10������������ */
#define P_ERR_DLY(f)                                        P_g_i(f, 9, 0)	/* һ�������ʱ ���ش˲���������������ERR_DLY����*/
#define P_val_err_ac_flux_delay(f)                          P_g_i(f, 9, 1)	/* ˮ�����������ʱ */
#define P_val_auto_reset_time(f)                            P_g_i(f, 9, 2)	/* ���ϸ�λʱ�� */
#define P_val_auto_reset_time1(f)                           P_g_i(f, 9, 3)	/* �Զ���λ����ʱ�� */
#define P_val_lp_detect_dly(f)                              P_g_i(f, 9, 4)	/* ��ѹ���ϼ����ʱ */
#define P_val_defrost_lp_limit_dly(f)                       P_g_i(f, 9, 5)	/* �˳�˪��ѹ���� */
#define P_val_cool_lp_alarm_delay(f)                        P_g_i(f, 9, 6)	/* �����ѹȷ����ʱ*/
#define P_val_heat_lp_alarm_delay(f)                        P_g_i(f, 9, 7)	/* ���ȵ�ѹȷ����ʱ */
#define P_val_err_wt_exit(f)                                P_g_i(f, 9, 8)	/* ��ˮ�±����²� */
#define P_val_err_ac_out_low(f)                             P_g_i(f, 9, 9)	/* ������¹��� */
#define P_val_err_ac_out_high(f)                            P_g_i(f, 9,10)	/* ���ȳ��¹��� */
#define P_val_forbid_on_env_cool(f)                         P_g_i(f, 9,11)	/* ����������� */
#define P_val_out_in_diff_err(f)                            P_g_i(f, 9,12)	/* �����²��쳣 */
#define P_val_gas_in_out_err_dly(f)                         P_g_i(f, 9,13)	/* �����쳣���� */
#define P_val_err_flux_open_pump(f)                         P_g_i(f, 9,14)	/* ˮ�����㿪�� */
#define P_val_invt_rps_force_sub_percent(f)                 P_g_i(f, 9,15)	/* ǿ�ƽ�Ƶ���� */
#define P_val_invt_rps_force_sub_period(f)                  P_g_i(f, 9,16)	/* ǿ�ƽ�Ƶ����  */
#define P_val_out_in_diff_high(f)                           P_g_i(f, 9,17)	/* �����²���� */
#define P_val_evap_low_cool(f)                              P_g_i(f, 9,18)	/* ������������ */
#define P_val_gas_out_high(f)                               P_g_i(f, 9,19)	/* �����¶ȹ��� */
#define P_ERR_WT_EXIT(f)                                    P_g_i(f, 9,20)	/* �������¶ȹ���(�²�) */
#define P_val_gas_in_low_cool(f)                            P_g_i(f, 9,21)	/* ������������ */
#define P_val_pwm_speed_too_low(f)                          P_g_i(f, 9,22)	/* PWM���ٹ��� */
#define P_val_pwm_speed_check_dly(f)                        P_g_i(f, 9,23)	/* PWM���ټ����ʱ */
#define P_val_check_lp_when_off(f)                          P_g_i(f, 9,24)	/* ��������ѹ */
#define P_val_TWout1_limit_1(f)                             P_g_i(f, 9,25)	/* ���ȳ�������1 */
#define P_val_TWout1_limit_2(f)                             P_g_i(f, 9,26)	/* ���ȳ�������2 */
#define P_val_TWout1_limit_3(f)                             P_g_i(f, 9,27)	/* ���ȳ�������3 */
#define P_val_TWout1_limit_4(f)                             P_g_i(f, 9,28)	/* ���ȳ�������4 */
#define P_val_TWout1_limit_5(f)                             P_g_i(f, 9,29)	/* ���ȳ�������5 */
#define P_val_TFLin_protect(f)                              P_g_i(f, 9,30)	/* ��ů���ȱ��� */
#define P_val_lpcheck_evn_temp(f)                           P_g_i(f, 9,31)	/* ��ѹ��黷��:��ѹ���ڴ��¶Ȳ�����ѹ���� */
#define P_val_lp_value_incl(f)                              P_g_i(f, 9,32)	/* �����ѹ����() */
#define P_val_lp_value_inht(f)                              P_g_i(f, 9,33)	/* ���ȵ�ѹ����() */
#define P_val_hp_value_incl(f)                              P_g_i(f, 9,34)	/* �����ѹ����() */
#define P_val_hp_value_inht(f)                              P_g_i(f, 9,35)	/* ���ȸ�ѹ����() */
#define P_val_lp_prs_exit_diff(f)                           P_g_i(f, 9,36)	/* ��ѹ������λѹ�� */
#define P_val_hp_prs_exit_diff(f)                           P_g_i(f, 9,37)	/* ��ѹ������λѹ�� */
#define P_val_low_temp_cool_env(f)                          P_g_i(f, 9,38)	/* �������价�� */
#define P_val_low_temp_cool_dly(f)                          P_g_i(f, 9,39)	/* ����������� */
#define P_val_mac_set_check(f)                              P_g_i(f, 9,40)	/* ��������У�� */
#define P_val_TWout1_limit_cool(f)                          P_g_i(f, 9,41)	/* ����������� */
#define P_val_use_UI35_commu_detect(f)                      P_g_i(f, 9,42)	/* �߿���ͨѶ��� */
#define P_val_invt_err_auto_reset(f)                        P_g_i(f, 9,43)	/* ��Ƶ�����Ը�λ */
#define P_val_hotw_max_TBH(f)                               P_g_i(f, 9,44)	/* ������ˮ���� */
#define P_val_hp_pre_adju_diff(f)                           P_g_i(f, 9,45)  /* ��ѹ�����ز� */
#define P_val_lp_pre_adju_diff(f)                           P_g_i(f, 9,46)  /* ��ѹ�����ز� */
#define ERR_DLY                                             (P_val(ERR_DLY                            ))/* 01 һ�������ʱ ���ش˲���������������ERR_DLY����*/
#define val_err_ac_flux_delay                               (P_val(val_err_ac_flux_delay              ))/* 02 ˮ�����������ʱ */
#define val_auto_reset_time                              (60*P_val(val_auto_reset_time                ))/* 03 ���ϸ�λʱ�� */
#define val_auto_reset_time1                             (60*P_val(val_auto_reset_time1               ))/* 04 �Զ���λ����ʱ�� */
#define val_lp_detect_dly                                   (P_val(val_lp_detect_dly                  ))/* 05 ��ѹ���ϼ����ʱ */
#define val_defrost_lp_limit_dly                            (P_val(val_defrost_lp_limit_dly           ))/* 06 �˳�˪��ѹ���� */
#define val_cool_lp_alarm_delay                             (P_val(val_cool_lp_alarm_delay            ))/* 07 �����ѹȷ����ʱ*/
#define val_heat_lp_alarm_delay                             (P_val(val_heat_lp_alarm_delay            ))/* 08 ���ȵ�ѹȷ����ʱ */
#define val_err_wt_exit                                  (10*P_val(val_err_wt_exit                    ))/* 09 �˳��±����²� */
#define val_err_ac_out_low                               (10*P_val(val_err_ac_out_low                 ))/* 10 ������¹��� */
#define val_err_ac_out_high                              (10*P_val(val_err_ac_out_high                ))/* 11 ���ȳ��¹��� */
#define val_forbid_on_env_cool                           (10*P_val(val_forbid_on_env_cool             ))/* 12 ����������� */
#define val_out_in_diff_err                              (10*P_val(val_out_in_diff_err                ))/* 13 �����²��쳣 */
#define val_gas_in_out_err_dly                              (P_val(val_gas_in_out_err_dly             ))/* 14 �����쳣���� */
#define val_err_flux_open_pump                              (P_val(val_err_flux_open_pump             ))/* 15 ˮ�����㿪�� */
#define val_invt_rps_force_sub_percent                      (P_val(val_invt_rps_force_sub_percent     ))/* 16 ǿ�ƽ�Ƶ���� */
#define val_invt_rps_force_sub_period                       (P_val(val_invt_rps_force_sub_period      ))/* 17 ǿ�ƽ�Ƶ����  */
#define val_out_in_diff_high                             (10*P_val(val_out_in_diff_high               ))/* 18 �����²���� */
#define val_evap_low_cool                                (10*P_val(val_evap_low_cool                  ))/* 19 ������������ */
#define val_gas_out_high                                 (10*P_val(val_gas_out_high                   ))/* 20 �����¶ȹ��� */
#define ERR_WT_EXIT                                      (10*P_val(ERR_WT_EXIT                        ))/* 21 �������¶ȹ���(�²�) */
#define val_gas_in_low_cool                              (10*P_val(val_gas_in_low_cool                ))/* 22 ������������ */
#define val_pwm_speed_too_low                               (P_val(val_pwm_speed_too_low              ))/* 23 PWM���ٹ��� */
#define val_pwm_speed_check_dly                             (P_val(val_pwm_speed_check_dly            ))/* 24 PWM���ټ����ʱ */
#define val_check_lp_when_off                               (P_val(val_check_lp_when_off              ))/* 25 ��������ѹ */
#define val_TWout1_limit_1                               (10*P_val(val_TWout1_limit_1                 ))/* 26 ���ȳ�������1 */
#define val_TWout1_limit_2                               (10*P_val(val_TWout1_limit_2                 ))/* 27 ���ȳ�������2 */
#define val_TWout1_limit_3                               (10*P_val(val_TWout1_limit_3                 ))/* 28 ���ȳ�������3 */
#define val_TWout1_limit_4                               (10*P_val(val_TWout1_limit_4                 ))/* 29 ���ȳ�������4 */
#define val_TWout1_limit_5                               (10*P_val(val_TWout1_limit_5                 ))/* 30 ���ȳ�������5 */
#define val_TFLin_protect                                (10*P_val(val_TFLin_protect                  ))/* 31 ��ů���ȱ��� */
#define val_lpcheck_evn_temp                             (10*P_val(val_lpcheck_evn_temp               ))/* 32 ��ѹ��黷��:��ѹ���ڴ��¶Ȳ�����ѹ���� */
#define val_lp_value_incl                                   (P_val(val_lp_value_incl                  ))/* 33 �����ѹ����() */
#define val_lp_value_inht                                   (P_val(val_lp_value_inht                  ))/* 34 ���ȵ�ѹ����() */
#define val_hp_value_incl                                   (P_val(val_hp_value_incl                  ))/* 35 �����ѹ����() */
#define val_hp_value_inht                                   (P_val(val_hp_value_inht                  ))/* 36 ���ȸ�ѹ����() */
#define val_lp_prs_exit_diff                                (P_val(val_lp_prs_exit_diff               ))/* 37 ��ѹ������λѹ�� */
#define val_hp_prs_exit_diff                                (P_val(val_hp_prs_exit_diff               ))/* 38 ��ѹ������λѹ�� */
#define val_low_temp_cool_env                            (10*P_val(val_low_temp_cool_env              ))/* 39 �������价�� */
#define val_low_temp_cool_dly                               (P_val(val_low_temp_cool_dly              ))/* 40 ����������� */
#define val_mac_set_check                                   (P_val(val_mac_set_check                  ))/* 41 ��������У�� */
#define val_TWout1_limit_cool                            (10*P_val(val_TWout1_limit_cool              ))/* 42 ����������� */
#define val_use_UI35_commu_detect                           (P_val(val_use_UI35_commu_detect          ))/* 43 �߿���ͨѶ��� */
#define val_invt_err_auto_reset 	                        (P_val(val_invt_err_auto_reset            ))/* 44 ��Ƶ�����Ը�λ */
#define val_hotw_max_TBH                                    (P_val(val_hotw_max_TBH                   ))/* 45 ������ˮ���� */
#define val_hp_pre_adju_diff                                (P_val(val_hp_pre_adju_diff               ))/* 46 ��ѹ�����ز� */
#define val_lp_pre_adju_diff                                (P_val(val_lp_pre_adju_diff               ))/* 47 ��ѹ�����ز� */

#define ERR_TEMP_DIFF                   (10*2)          /* һ�㱣���˳��²� */
#define TEMP_IN_DIFF                    (1*10)          /* ���±�������±����Ĳ�ֵ */

// 11��������̽ͷ
#define P_val_DI_type_lp(f)                                 P_g_i(f,10, 0)	/* ѹ����ѹ */
#define P_val_DI_type_hp(f)                                 P_g_i(f,10, 1)	/* ѹ����ѹ */
#define P_val_DI_IBH_feedback(f)                            P_g_i(f,10, 2)	/* ���õ��ȷ��� */
#define P_val_DI_type_air_flux(f)                           P_g_i(f,10, 3)	/* ˮ������ */
#define P_val_DI_TBH_feedback(f)                            P_g_i(f,10, 4)	/* ˮ����ȷ��� */
#define P_val_rfg_temp_corr_en(f)                           P_g_i(f,10, 5)	/* ��ý�¶����� */
#define P_val_rfg_vap_corr_1(f)                             P_g_i(f,10, 6)	/* ��ý��������1 */
#define P_val_rfg_vap_corr_2(f)                             P_g_i(f,10, 7)	/* ��ý��������2 */
#define P_val_rfg_vap_corr_3(f)                             P_g_i(f,10, 8)	/* ��ý��������3 */
#define P_val_rfg_vap_corr_4(f)                             P_g_i(f,10, 9)	/* ��ý��������4 */
#define P_val_rfg_vap_corr_5(f)                             P_g_i(f,10,10)	/* ��ý��������5 */
#define P_val_rfg_liq_corr_1(f)                             P_g_i(f,10,11)	/* ��ýҺ������1 */
#define P_val_rfg_liq_corr_2(f)                             P_g_i(f,10,12)	/* ��ýҺ������2 */
#define P_val_rfg_liq_corr_3(f)                             P_g_i(f,10,13)	/* ��ýҺ������3 */
#define P_val_rfg_liq_corr_4(f)                             P_g_i(f,10,14)	/* ��ýҺ������4 */
#define P_val_rfg_liq_corr_5(f)                             P_g_i(f,10,15)	/* ��ýҺ������5 */
#define P_val_rfg_liq_corr_6(f)                             P_g_i(f,10,16)	/* ��ýҺ������6 */
#define P_val_rfg_liq_corr_7(f)                             P_g_i(f,10,17)	/* ��ýҺ������7 */
#define P_val_THWt_sensor_used(f)                           P_g_i(f,10,18)	/* ��ˮˮ���¶�̽ͷ */
#define P_val_TFLin_sensor_used(f)                          P_g_i(f,10,19)	/* ��ů��ˮ�¶�̽ͷ */
#define P_val_TWout3_sensor_used(f)                         P_g_i(f,10,20)	/* ϵͳ�ܳ�ˮ�¶�̽ͷ */
#define P_val_DI_type_EVU(f)                                P_g_i(f,10,21)	/* EVU�ź� */
#define P_val_DI_type_SG(f)                                 P_g_i(f,10,22)	/* SG�ź� */
#define P_val_ntc_sensor_config(f)                          P_g_i(f,10,23)	/* �¶�̽ͷ���� */
#define P_val_unit_LDIw_used(f)                             P_g_i(f,10,24)	/* ��Ԫˮ������ */
#define P_val_TSL_sensor_used(f)                            P_g_i(f,10,25)	/* ̫�����¶�̽ͷ */
#define P_val_J5_prs_sensor_type(f)                         P_g_i(f,10,26)	/* J5ѹ������() */
#define P_val_J6_prs_sensor_type(f)                         P_g_i(f,10,27)	/* J6ѹ������() */
#define P_val_J5_scale_min(f)                               P_g_i(f,10,28)	/* J5��С����() */
#define P_val_J5_scale_max(f)                               P_g_i(f,10,29)	/* J5�������() */
#define P_val_J6_scale_min(f)                               P_g_i(f,10,30)	/* J6��С����() */
#define P_val_J6_scale_max(f)                               P_g_i(f,10,31)	/* J6�������() */
#define P_val_TACt1_sensor_used(f)                          P_g_i(f,10,32)	/* ����ˮ���ϲ��¶�̽ͷ */
#define P_val_TACt2_sensor_used(f)                          P_g_i(f,10,33)	/* ����ˮ���²��¶�̽ͷ */
#define P_val_prs_sensor_config(f)                          P_g_i(f,10,34)	/* ѹ������������ */
#define P_val_power_fan(f)                                  P_g_i(f,10,35)	/* �������  */
#define P_val_power_pump(f)                                 P_g_i(f,10,36)	/* ˮ�ù���  */
#define P_val_power_IBH1(f)                                 P_g_i(f,10,37)	/* ����1���� */
#define P_val_power_IBH2(f)                                 P_g_i(f,10,38)	/* ����2���� */
#define P_val_power_idle(f)                                 P_g_i(f,10,39)	/* �������� */
#define P_val_power_other_run(f)                            P_g_i(f,10,40)	/* �������й��� */
#define P_val_TWout2_sensor_used(f)                         P_g_i(f,10,41)	/* �ܳ�ˮ�¶�̽ͷ */
#define val_DI_type_lp                                      (P_val(val_DI_type_lp                     ))/* 01 ѹ����ѹ */
#define val_DI_type_hp                                      (P_val(val_DI_type_hp                     ))/* 02 ѹ����ѹ */
#define val_DI_IBH_feedback                                 (P_val(val_DI_IBH_feedback                ))/* 03 ���õ��ȷ��� */
#define val_DI_type_air_flux                                (P_val(val_DI_type_air_flux               ))/* 04 ˮ������ */
#define val_DI_TBH_feedback                                 (P_val(val_DI_TBH_feedback                ))/* 05 ˮ����ȷ��� */
#define val_rfg_temp_corr_en                                (P_val(val_rfg_temp_corr_en               ))/* 06 ��ý�¶����� */
#define val_rfg_vap_corr_1                              (100*P_val(val_rfg_vap_corr_1                 ))/* 07 ��ý��������1 */
#define val_rfg_vap_corr_2                              (100*P_val(val_rfg_vap_corr_2                 ))/* 08 ��ý��������2 */
#define val_rfg_vap_corr_3                              (100*P_val(val_rfg_vap_corr_3                 ))/* 09 ��ý��������3 */
#define val_rfg_vap_corr_4                              (100*P_val(val_rfg_vap_corr_4                 ))/* 10 ��ý��������4 */
#define val_rfg_vap_corr_5                              (100*P_val(val_rfg_vap_corr_5                 ))/* 11 ��ý��������5 */
#define val_rfg_liq_corr_1                              (100*P_val(val_rfg_liq_corr_1                 ))/* 12 ��ýҺ������1 */
#define val_rfg_liq_corr_2                              (100*P_val(val_rfg_liq_corr_2                 ))/* 13 ��ýҺ������2 */
#define val_rfg_liq_corr_3                              (100*P_val(val_rfg_liq_corr_3                 ))/* 14 ��ýҺ������3 */
#define val_rfg_liq_corr_4                              (100*P_val(val_rfg_liq_corr_4                 ))/* 15 ��ýҺ������4 */
#define val_rfg_liq_corr_5                              (100*P_val(val_rfg_liq_corr_5                 ))/* 16 ��ýҺ������5 */
#define val_rfg_liq_corr_6                              (100*P_val(val_rfg_liq_corr_6                 ))/* 17 ��ýҺ������6 */
#define val_rfg_liq_corr_7                              (100*P_val(val_rfg_liq_corr_7                 ))/* 18 ��ýҺ������7 */
#define val_THWt_sensor_used                                (P_val(val_THWt_sensor_used               ))/* 19 ��ˮˮ���¶�̽ͷ */
#define val_TFLin_sensor_used                               (P_val(val_TFLin_sensor_used              ))/* 20 ��ů��ˮ�¶�̽ͷ */
#define val_TWout3_sensor_used                              (P_val(val_TWout3_sensor_used             ))/* 21 ϵͳ�ܳ�ˮ�¶�̽ͷ */
#define val_DI_type_EVU                                     (P_val(val_DI_type_EVU                    ))/* 22 EVU�ź� */
#define val_DI_type_SG                                      (P_val(val_DI_type_SG                     ))/* 23 SG�ź� */
#define val_ntc_sensor_config                               (P_val(val_ntc_sensor_config              ))/* 24 �¶�̽ͷ���� */
#define val_unit_LDIw_used                                  (P_val(val_unit_LDIw_used                 ))/* 25 ��Ԫˮ������ */
#define val_TSL_sensor_used                                 (P_val(val_TSL_sensor_used                ))/* 26 ̫�����¶�̽ͷ */
#define val_J5_prs_sensor_type                              (P_val(val_J5_prs_sensor_type             ))/* 27 J5ѹ������() */
#define val_J6_prs_sensor_type                              (P_val(val_J6_prs_sensor_type             ))/* 28 J6ѹ������() */
#define val_J5_scale_min                                    (P_val(val_J5_scale_min                   ))/* 29 J5��С����() */
#define val_J5_scale_max                                    (P_val(val_J5_scale_max                   ))/* 30 J5�������() */
#define val_J6_scale_min                                    (P_val(val_J6_scale_min                   ))/* 31 J6��С����() */
#define val_J6_scale_max                                    (P_val(val_J6_scale_max                   ))/* 32 J6�������() */
#define val_TACt1_sensor_used                               (P_val(val_TACt1_sensor_used              ))/* 33 ����ˮ���ϲ��¶�̽ͷ */
#define val_TACt2_sensor_used                               (P_val(val_TACt2_sensor_used              ))/* 34 ����ˮ���²��¶�̽ͷ */
#define val_prs_sensor_config                               (P_val(val_prs_sensor_config              ))/* 35 ѹ������������ */
#define val_power_fan                                       (P_val(val_power_fan                      ))/* 36 �������  */
#define val_power_pump                                      (P_val(val_power_pump                     ))/* 37 ˮ�ù���  */
#define val_power_IBH1                                      (P_val(val_power_IBH1                     ))/* 38 ����1���� */
#define val_power_IBH2                                      (P_val(val_power_IBH2                     ))/* 39 ����2���� */
#define val_power_idle                                      (P_val(val_power_idle                     ))/* 40 �������� */
#define val_power_other_run                                 (P_val(val_power_other_run                ))/* 41 �������й��� */
#define val_TWout2_sensor_used                              (P_val(val_TWout2_sensor_used             ))/* 42 �ܳ�ˮ�¶�̽ͷ */

/* ��һ�������ʲ�Ϊ0����ʾ���� */
#define USE_ELEC_METERING (val_power_fan     \
                           || val_power_pump \
                           || val_power_IBH1 \
                           || val_power_IBH2 \
                           || val_power_idle \
                           || val_power_other_run \
                           )

#ifdef USE_ODU_HL081B		/* HL081B */
/* ��ý���� */
enum {
    RF_R22,   
    RF_R407C, 
    RF_R410A, 
    RF_R32,	
    RF_R290,
    RF_MAX
};

enum {
    NTC_CFG_PUNP,           /* ����: ����̽ͷ���� */
    NTC_CFG_TCL,            /* ����1: TCLר��̽ͷ���� */
    NTC_CFG_CHANGHONG,      /* ����2: ����ר��̽ͷ���� */
    NTC_CFG_PAIWO,			/* ����3: ����ר��̽ͷ���� */
    NTC_CFG_NIUENTAI,		/* ����4: Ŧ��̩ר��̽ͷ����(50K3950/5K3470) */
    NTC_CFG_MAX,
};

/* ʹ�ð���̽ͷ */
#define USE_PUNP_NTC                   (val_ntc_sensor_config == NTC_CFG_PUNP)
/* ʹ��TCLר��̽ͷ */
#define USE_TCL_NTC                    (val_ntc_sensor_config == NTC_CFG_TCL)
/* ʹ�ó���ר��̽ͷ */
#define USE_CHANGHONG_NTC              (val_ntc_sensor_config == NTC_CFG_CHANGHONG)
/* ʹ������ר��̽ͷ */
#define USE_PAIWO_NTC                  (val_ntc_sensor_config == NTC_CFG_PAIWO)
/* ʹ��Ŧ��̩ר��̽ͷ */
#define USE_NIUENTAI_NTC               (val_ntc_sensor_config == NTC_CFG_NIUENTAI)

#else						/* HL081A */
/* ��ý���� */
enum {
    RF_R22,   
    RF_R407C, 
    RF_R410A, 
    RF_R32,	
    RF_R290,
    RF_MAX
};

enum {
    NTC_CFG_PUNP,           /* ����: ����̽ͷ���� */
    NTC_CFG_TCL,            /* ����1: TCLר��̽ͷ���� */
    NTC_CFG_CHANGHONG,      /* ����2: ����ר��̽ͷ���� */
    NTC_CFG_MAX,
};

/* ʹ�ð���̽ͷ */
#define USE_PUNP_NTC                   (val_ntc_sensor_config == NTC_CFG_PUNP)
/* ʹ��TCLר��̽ͷ */
#define USE_TCL_NTC                    (val_ntc_sensor_config == NTC_CFG_TCL)
/* ʹ�ó���ר��̽ͷ */
#define USE_CHANGHONG_NTC              (val_ntc_sensor_config == NTC_CFG_CHANGHONG)

#endif

/* ѹ������������ */
enum
{
    PRS_CFG_REGULAR,			/* �ߵ�ѹ����: (����1��ѹ������������ͨ����廻�䣬�������ѹ�����ȼ���ѹ�����������ƺͷ������ʹ��) */
    PRS_CFG_SUCTION,			/* ����ѹ: (����1��ѹ����λ��ѹ�������ڣ�����/���Ⱦ�����ѹ��������������ʹ��) */
    PRS_CFG_SUC_EXH,			/* �ߵ�ѹ����: ��ѹ+��ѹͬʱʹ�� */
    PRS_CFG_EXHAUST,			/* ����ѹ */
    PRS_CFG_UNUSED,			    /* �ߵ�ѹ���� */
    PRS_CFG_MAX,
};

/* �ߵ�ѹ���� */
#define USE_REGULAR_PRS                (val_prs_sensor_config == PRS_CFG_REGULAR)
/* ����ѹ */
#define USE_SUCTION_PRS                (val_prs_sensor_config == PRS_CFG_SUCTION)
/* �ߵ�ѹ���� */
#define USE_SUC_EXH_PRS                (val_prs_sensor_config == PRS_CFG_SUC_EXH)
/* ����ѹ */
#define USE_EXHAUST_PRS                (val_prs_sensor_config == PRS_CFG_EXHAUST)
/* �ߵ�ѹ���� */
#define USE_NO_PRS                     (val_prs_sensor_config == PRS_CFG_UNUSED)

/* ʹ�������ѹ */
#define USE_COOL_LP (USE_REGULAR_PRS || USE_SUCTION_PRS || USE_SUC_EXH_PRS)

/* ʹ���ܳ�ˮ�¶�TWout2 */
#define USE_AC_TEMP_TWOUT2             (!val_TACt1_sensor_used && !val_TACt2_sensor_used)
/* ʹ�û���ˮ���ϲ��¶�TACt1 */
#define USE_AC_TEMP_TACT1              (val_TACt1_sensor_used && !val_TACt2_sensor_used)
/* ʹ�û���ˮ���²��¶�TACt2 */
#define USE_AC_TEMP_TACT2              (!val_TACt1_sensor_used && val_TACt2_sensor_used)
/* ͬʱʹ��TACt1��TACt2 */
#define USE_AC_TEMP_TACT1_TACT2        (val_TACt1_sensor_used && val_TACt2_sensor_used)

/* ����ģʽ */
#define USE_SP_OUT_CTRL                (fg_sv_temp_energy_overwrite_ac && AC_TWOUT2_AC_CTRL)

/* �յ�ʹ�û�ˮ���� */
#define USE_AC_TEMP_IN_CTRL            (!USE_SP_OUT_CTRL            \
                                        && (USE_AC_TEMP_TACT1       \
                                            || USE_AC_TEMP_TACT2    \
                                            || USE_AC_TEMP_TACT1_TACT2))

#define val_compensate_in              (0)
#define val_compensate_out             (0)
#define val_compensate_env             (0)
#define val_compensate_wing            (0)
#define val_compensate_gas_out         (0)
#define val_compensate_gas_in          (0)
#define val_seneor_use_anti_ice        (0)
#define val_seneor_use_comp_top        (0)
#define val_seneor_disp_evi_temp       (0)

/* 12���Լ����� */
// GB1��GB/T25127.2-2010
// GB2��GB18430.2-2016
enum {
    GB_DBG_UNUSED=0,
    GB_DBG_25127,
    GB_DBG_18430,
    GB_DBG_MAX
};

enum {
    AT_UNUSED=0,
    AT_HEAT_A,
    AT_HEAT_B,
    AT_HEAT_E,
    AT_HEAT_G,
    AT_HEAT_C,
    AT_HEAT_D,
    AT_COOL,
    AT_MAX
};
#define AT_HT_WC_NUM    6			    /* ������������ÿ�鹤������ */
#define AT_HT_PARA_NUM  42			    /* ������������ÿ��������� */
#define AT_HT_GP_ORIGIN 0xff		    /* �����������ȹ���ԭʼ��� */

enum {
    PFC_SET_OPEN=0,
    PFC_SET_CLOSE,
    PFC_SET_AUTO,
    PFC_SET_MAX
};

#define P_val_ability_test_set(f)                           P_g_i(f,11, 0)  //  ��������ѡ��
#define P_val_ability_test_invt_rps_HA(f)                   P_g_i(f,11, 1)  //  ����ѹ��ת��A
#define P_val_ability_test_fan_rpm_HA(f)                    P_g_i(f,11, 2)  //  ���ȷ��ת��A
#define P_val_ability_test_motor_ctrl_HA(f)                 P_g_i(f,11, 3)  //  ������������A
#define P_val_ability_test_invt_rps_HB(f)                   P_g_i(f,11, 4)  //  ����ѹ��ת��B
#define P_val_ability_test_fan_rpm_HB(f)                    P_g_i(f,11, 5)  //  ���ȷ��ת��B
#define P_val_ability_test_motor_ctrl_HB(f)                 P_g_i(f,11, 6)  //  ������������B
#define P_val_ability_test_invt_rps_HE(f)                   P_g_i(f,11, 7)  //  ����ѹ��ת��E
#define P_val_ability_test_fan_rpm_HE(f)                    P_g_i(f,11, 8)  //  ���ȷ��ת��E
#define P_val_ability_test_motor_ctrl_HE(f)                 P_g_i(f,11, 9)  //  ������������E
#define P_val_ability_test_invt_rps_HG(f)                   P_g_i(f,11,10)  //  ����ѹ��ת��G
#define P_val_ability_test_fan_rpm_HG(f)                    P_g_i(f,11,11)  //  ���ȷ��ת��G
#define P_val_ability_test_motor_ctrl_HG(f)                 P_g_i(f,11,12)  //  ������������G
#define P_val_ability_test_invt_rps_HC(f)                   P_g_i(f,11,13)  //  ����ѹ��ת��C
#define P_val_ability_test_fan_rpm_HC(f)                    P_g_i(f,11,14)  //  ���ȷ��ת��C
#define P_val_ability_test_motor_ctrl_HC(f)                 P_g_i(f,11,15)  //  ������������C
#define P_val_ability_test_invt_rps_HD(f)                   P_g_i(f,11,16)  //  ����ѹ��ת��D
#define P_val_ability_test_fan_rpm_HD(f)                    P_g_i(f,11,17)  //  ���ȷ��ת��D
#define P_val_ability_test_motor_ctrl_HD(f)                 P_g_i(f,11,18)  //  ������������D
#define P_val_ability_test_invt_rps_C(f)                    P_g_i(f,11,19)  //  ����ѹ��ת��
#define P_val_ability_test_fan_rpm_C(f)                     P_g_i(f,11,20)  //  ������ת��
#define P_val_ability_test_motor_ctrl_C(f)                  P_g_i(f,11,21)  //  ������������
#define P_val_ability_test_EWD_HA(f)                        P_g_i(f,11,22)  //  ���ȳ�˪����A
#define P_val_ability_test_interval_HA(f)                   P_g_i(f,11,23)  //  ���ȳ�˪���A
#define P_val_ability_test_step_HA(f)                       P_g_i(f,11,24)  //  ���ȳ�˪����A
#define P_val_ability_test_freq_HA(f)                       P_g_i(f,11,25)  //  ���ȳ�˪ת��A
#define P_val_ability_test_wing_HA(f)                       P_g_i(f,11,26)  //  �����˳�˪����A
#define P_val_ability_test_defr_time_HA(f)                  P_g_i(f,11,27)  //  ���ȳ�˪ʱ��A
#define P_val_ability_test_EWD_HB(f)                        P_g_i(f,11,28)  //  ���ȳ�˪����B
#define P_val_ability_test_interval_HB(f)                   P_g_i(f,11,29)  //  ���ȳ�˪���B
#define P_val_ability_test_step_HB(f)                       P_g_i(f,11,30)  //  ���ȳ�˪����B
#define P_val_ability_test_freq_HB(f)                       P_g_i(f,11,31)  //  ���ȳ�˪ת��B
#define P_val_ability_test_wing_HB(f)                       P_g_i(f,11,32)  //  �����˳�˪����B
#define P_val_ability_test_defr_time_HB(f)                  P_g_i(f,11,33)  //  ���ȳ�˪ʱ��B
#define P_val_ability_test_EWD_HE(f)                        P_g_i(f,11,34)  //  ���ȳ�˪����E
#define P_val_ability_test_interval_HE(f)                   P_g_i(f,11,35)  //  ���ȳ�˪���E
#define P_val_ability_test_step_HE(f)                       P_g_i(f,11,36)  //  ���ȳ�˪����E
#define P_val_ability_test_freq_HE(f)                       P_g_i(f,11,37)  //  ���ȳ�˪ת��E
#define P_val_ability_test_wing_HE(f)                       P_g_i(f,11,38)  //  �����˳�˪����E
#define P_val_ability_test_defr_time_HE(f)                  P_g_i(f,11,39)  //  ���ȳ�˪ʱ��E
#define P_val_ability_test_EWD_HG(f)                        P_g_i(f,11,40)  //  ���ȳ�˪����G
#define P_val_ability_test_interval_HG(f)                   P_g_i(f,11,41)  //  ���ȳ�˪���G
#define P_val_ability_test_step_HG(f)                       P_g_i(f,11,42)  //  ���ȳ�˪����G
#define P_val_ability_test_freq_HG(f)                       P_g_i(f,11,43)  //  ���ȳ�˪ת��G
#define P_val_ability_test_wing_HG(f)                       P_g_i(f,11,44)  //  �����˳�˪����G
#define P_val_ability_test_defr_time_HG(f)                  P_g_i(f,11,45)  //  ���ȳ�˪ʱ��G
#define P_val_ability_test_pfc_set(f)                       P_g_i(f,11,46)  //  PFC����
#define P_val_debug_test_pump(f)                            P_g_i(f,11,47)  //  �ֶ���Ƶˮ��
#define val_ability_test_set                                (P_val(val_ability_test_set               ))  //  ��������ѡ��
#define val_ability_test_invt_rps_HA                        (P_val(val_ability_test_invt_rps_HA       ))  //  ����ѹ��ת��A
#define val_ability_test_fan_rpm_HA                         (P_val(val_ability_test_fan_rpm_HA        ))  //  ���ȷ��ת��A
#define val_ability_test_motor_ctrl_HA                      (P_val(val_ability_test_motor_ctrl_HA     ))  //  ������������A
#define val_ability_test_invt_rps_HB                        (P_val(val_ability_test_invt_rps_HB       ))  //  ����ѹ��ת��B
#define val_ability_test_fan_rpm_HB                         (P_val(val_ability_test_fan_rpm_HB        ))  //  ���ȷ��ת��B
#define val_ability_test_motor_ctrl_HB                      (P_val(val_ability_test_motor_ctrl_HB     ))  //  ������������B
#define val_ability_test_invt_rps_HE                        (P_val(val_ability_test_invt_rps_HE       ))  //  ����ѹ��ת��E
#define val_ability_test_fan_rpm_HE                         (P_val(val_ability_test_fan_rpm_HE        ))  //  ���ȷ��ת��E
#define val_ability_test_motor_ctrl_HE                      (P_val(val_ability_test_motor_ctrl_HE     ))  //  ������������E
#define val_ability_test_invt_rps_HG                        (P_val(val_ability_test_invt_rps_HG       ))  //  ����ѹ��ת��G
#define val_ability_test_fan_rpm_HG                         (P_val(val_ability_test_fan_rpm_HG        ))  //  ���ȷ��ת��G
#define val_ability_test_motor_ctrl_HG                      (P_val(val_ability_test_motor_ctrl_HG     ))  //  ������������G
#define val_ability_test_invt_rps_HC                        (P_val(val_ability_test_invt_rps_HC       ))  //  ����ѹ��ת��C
#define val_ability_test_fan_rpm_HC                         (P_val(val_ability_test_fan_rpm_HC        ))  //  ���ȷ��ת��C
#define val_ability_test_motor_ctrl_HC                      (P_val(val_ability_test_motor_ctrl_HC     ))  //  ������������C
#define val_ability_test_invt_rps_HD                        (P_val(val_ability_test_invt_rps_HD       ))  //  ����ѹ��ת��D
#define val_ability_test_fan_rpm_HD                         (P_val(val_ability_test_fan_rpm_HD        ))  //  ���ȷ��ת��D
#define val_ability_test_motor_ctrl_HD                      (P_val(val_ability_test_motor_ctrl_HD     ))  //  ������������D
#define val_ability_test_invt_rps_C                         (P_val(val_ability_test_invt_rps_C        ))  //  ����ѹ��ת��
#define val_ability_test_fan_rpm_C                          (P_val(val_ability_test_fan_rpm_C         ))  //  ������ת��
#define val_ability_test_motor_ctrl_C                       (P_val(val_ability_test_motor_ctrl_C      ))  //  ������������
#define val_ability_test_EWD_HA                             (P_val(val_ability_test_EWD_HA            ))  //  ���ȳ�˪����A
#define val_ability_test_interval_HA                        (P_val(val_ability_test_interval_HA       ))  //  ���ȳ�˪���A
#define val_ability_test_step_HA                            (P_val(val_ability_test_step_HA           ))  //  ���ȳ�˪����A
#define val_ability_test_freq_HA                            (P_val(val_ability_test_freq_HA           ))  //  ���ȳ�˪ת��A
#define val_ability_test_wing_HA                            (P_val(val_ability_test_wing_HA           ))  //  �����˳�˪����A
#define val_ability_test_defr_time_HA                       (P_val(val_ability_test_defr_time_HA      ))  //  ���ȳ�˪ʱ��A
#define val_ability_test_EWD_HB                             (P_val(val_ability_test_EWD_HB            ))  //  ���ȳ�˪����B
#define val_ability_test_interval_HB                        (P_val(val_ability_test_interval_HB       ))  //  ���ȳ�˪���B
#define val_ability_test_step_HB                            (P_val(val_ability_test_step_HB           ))  //  ���ȳ�˪����B
#define val_ability_test_freq_HB                            (P_val(val_ability_test_freq_HB           ))  //  ���ȳ�˪ת��B
#define val_ability_test_wing_HB                            (P_val(val_ability_test_wing_HB           ))  //  �����˳�˪����B
#define val_ability_test_defr_time_HB                       (P_val(val_ability_test_defr_time_HB      ))  //  ���ȳ�˪ʱ��B
#define val_ability_test_EWD_HE                             (P_val(val_ability_test_EWD_HE            ))  //  ���ȳ�˪����E
#define val_ability_test_interval_HE                        (P_val(val_ability_test_interval_HE       ))  //  ���ȳ�˪���E
#define val_ability_test_step_HE                            (P_val(val_ability_test_step_HE           ))  //  ���ȳ�˪����E
#define val_ability_test_freq_HE                            (P_val(val_ability_test_freq_HE           ))  //  ���ȳ�˪ת��E
#define val_ability_test_wing_HE                            (P_val(val_ability_test_wing_HE           ))  //  �����˳�˪����E
#define val_ability_test_defr_time_HE                       (P_val(val_ability_test_defr_time_HE      ))  //  ���ȳ�˪ʱ��E
#define val_ability_test_EWD_HG                             (P_val(val_ability_test_EWD_HG            ))  //  ���ȳ�˪����G
#define val_ability_test_interval_HG                        (P_val(val_ability_test_interval_HG       ))  //  ���ȳ�˪���G
#define val_ability_test_step_HG                            (P_val(val_ability_test_step_HG           ))  //  ���ȳ�˪����G
#define val_ability_test_freq_HG                            (P_val(val_ability_test_freq_HG           ))  //  ���ȳ�˪ת��G
#define val_ability_test_wing_HG                            (P_val(val_ability_test_wing_HG           ))  //  �����˳�˪����G
#define val_ability_test_defr_time_HG                       (P_val(val_ability_test_defr_time_HG      ))  //  ���ȳ�˪ʱ��G
#define val_ability_test_pfc_set                            (P_val(val_ability_test_pfc_set           ))  //  PFC����
#define val_debug_test_pump                                 (P_val(val_debug_test_pump                ))  //  �ֶ���Ƶˮ��


#define val_ability_test_invt_rps           (para_AT_use_get()==0 ? 0 \
                                            : 100*para_C[GROUP_DEBUG][1+3*(para_AT_use_get()-1)]) // ѹ������ת��
#define val_ability_test_fan_rpm            (para_AT_use_get()==0 ? 0 \
                                            : para_C[GROUP_DEBUG][2+3*(para_AT_use_get()-1)]) // ������Է���
#define val_ability_test_motor_ctrl         (para_AT_use_get()==0 ? 0 \
                                            : para_C[GROUP_DEBUG][3+3*(para_AT_use_get()-1)]) // �������Կ���
#define val_ability_test_s_motor_ctrl       (para_AT_use_get()==0 ? 0 \
                                            : para_C[GROUP_EEV_S][39+(para_AT_use_get()-1)]) // �������Կ���

#define USE_ABILITY_EEV_SSH_SV              (fg_ability_test_run \
                                            && fun_pv_in(val_ability_test_motor_ctrl, -200, 800))
#define USE_ABILITY_EEV_STEP                (fg_ability_test_run \
                                            && val_ability_test_motor_ctrl>1000)
#define val_ability_test_eev_ssh_sv         (val_ability_test_motor_ctrl)       // �������Թ���
#define val_ability_test_motor_steps        (val_ability_test_motor_ctrl-1000)  // �������Բ���

#define USE_ABILITY_EVI_SSH_SV              (fg_ability_test_run \
                                            && fun_pv_in(val_ability_test_s_motor_ctrl, -200, 800))
#define USE_ABILITY_EVI_STEP                (fg_ability_test_run \
                                            && val_ability_test_s_motor_ctrl>1000)
#define val_ability_test_evi_ssh_sv         (val_ability_test_s_motor_ctrl)       // �������Թ���
#define val_ability_test_s_motor_steps      (val_ability_test_s_motor_ctrl-1000)  // �������Բ���


#define USE_DEBUG_PUMP                      (val_debug_test_pump>0)

#define val_ability_test_power              (0) // ���������仯�����ݽ���

#define val_ability_test_flow               (20) // ����ˮ����

#define val_ability_test_EWD                ((para_AT_use_get()==0 || para_AT_use_get()>=5) ? 0\
                                            : 10*para_C[GROUP_DEBUG][22+6*(para_AT_use_get()-1)]) // ���ȳ�˪����

#define val_ability_test_interval           ((para_AT_use_get()==0 || para_AT_use_get()>=5) ? 0\
                                            : 60*para_C[GROUP_DEBUG][23+6*(para_AT_use_get()-1)]) // ���ȳ�˪���

#define val_ability_test_step               ((para_AT_use_get()==0 || para_AT_use_get()>=5) ? 0\
                                            : para_C[GROUP_DEBUG][24+6*(para_AT_use_get()-1)]) // ���ȳ�˪����

#define val_ability_test_freq               ((para_AT_use_get()==0 || para_AT_use_get()>=5) ? val_invt_rps_defrost\
                                            : 100*para_C[GROUP_DEBUG][25+6*(para_AT_use_get()-1)]) // ���ȳ�˪ת��

#define val_ability_test_wing               ((para_AT_use_get()==0 || para_AT_use_get()>=5) ? 0\
                                            : 10*para_C[GROUP_DEBUG][26+6*(para_AT_use_get()-1)]) // �����˳�˪����

#define val_ability_test_defr_time          ((para_AT_use_get()==0 || para_AT_use_get()>=5) ? 0\
                                            : para_C[GROUP_DEBUG][27+6*(para_AT_use_get()-1)]) // ���ȳ�˪ʱ��

#define ABILITY_TEST_HAS_DEFR               (fg_ability_test_run && val_ability_test_step!=0) /* ��˪�Ĺ��� */
#define ABILITY_TEST_NO_DEFR                (fg_ability_test_run && val_ability_test_step==0) /* ����˪�Ĺ��� */


#define AT_VAL(v1, v2)                      ((fg_ability_test_run&&(v2)>0) ? (v2) : (v1))

#define val_ability_test_evi_steps          (0)


#define val_gb_dbg_use           (GB_DBG_UNUSED) //  1 �����������
#define val_drytemp_cpst                     (0) //  2 �����¶Ȳ���
#define val_invt_rps_heatrun_freq1      (100*30) //  3 ����ת��1    GB1     ����7��
#define val_invt_rps_heatrun_freq2      (100*30) //  4 ����ת��2    GB1     ����0��
#define val_invt_rps_heatrun_freq3      (100*45) //  5 ����ת��3    GB1     ����-6��
#define val_invt_rps_heatrun_freq4      (100*75) //  6 ����ת��4    GB1     ����-12��
#define val_debug_sh_sv1                    (10) //  7 ���ȹ���1    GB1     ����7��
#define val_debug_sh_sv2                    (10) //  8 ���ȹ���2    GB1     ����0��
#define val_debug_sh_sv3                    (20) //  9 ���ȹ���3    GB1     ����-6��
#define val_debug_sh_sv4                    (20) // 10 ���ȹ���4    GB1     ����-12��
#define val_invt_rps_coolrun_freq1      (100*75) // 11 ����ת��1    GB1 GB2 ����35��
#define val_invt_rps_coolrun_freq2      (100*53) // 12 ����ת��2    GB1 GB2 ����31.5��
#define val_invt_rps_coolrun_freq3      (100*35) // 13 ����ת��3    GB1 GB2 ����28��
#define val_invt_rps_coolrun_freq4      (100*30) // 14 ����ת��4    GB1 GB2 ����24.5��
#define val_debug_sc_sv1                    (20) // 15 �������1    GB1 GB2 ����35��
#define val_debug_sc_sv2                    (20) // 16 �������2    GB1 GB2 ����31.5��
#define val_debug_sc_sv3                    (20) // 17 �������3    GB1 GB2 ����28��
#define val_debug_sc_sv4                    (20) // 18 �������4    GB1 GB2 ����24.5��
#define val_invt_rps_sheatrun_freq      (100*68) // 19 ���ȱ��ת�� GB2     ����7��
#define val_debug_ssh_sv                    (10) // 20 ���ȱ������ GB2     ����7��

#define USE_GB_DBG      (val_gb_dbg_use != GB_DBG_UNUSED)
#define USE_GB_25127    (val_gb_dbg_use == GB_DBG_25127)
#define USE_GB_18430    (val_gb_dbg_use == GB_DBG_18430)

#if defined(M_TCL)||defined(M_CHANGHONG)
#define COOL_RUN_ENV_LLIMIT         (-14)   /* �������޻��� */
#define COOL_RUN_ENV_HLIMIT         (52)    /* �������޻��� */
#define HEAT_RUN_ENV_LLIMIT         (-28)   /* �������޻��� */
#define HEAT_RUN_ENV_HLIMIT         (52)    /* �������޻��� */
#define HOT_WATER_RUN_ENV_LLIMIT    (-28)   /* ��ˮ���޻��� */
#define HOT_WATER_RUN_ENV_HLIMIT    (52)    /* ��ˮ���޻��� */
#else
#define COOL_RUN_ENV_LLIMIT         (-14)   /* �������޻��� */
#define COOL_RUN_ENV_HLIMIT         (72)    /* �������޻��� */
#define HEAT_RUN_ENV_LLIMIT         (-52)   /* �������޻��� */
#define HEAT_RUN_ENV_HLIMIT         (72)    /* �������޻��� */
#define HOT_WATER_RUN_ENV_LLIMIT    (-52)   /* ��ˮ���޻��� */
#define HOT_WATER_RUN_ENV_HLIMIT    (72)    /* ��ˮ���޻��� */
#endif

/* 13�������� */
#define P_val_ctrl_period(f)                                P_g_i(f,12, 0)	/* ������������ */
#define P_val_cool_add_diff(f)                              P_g_i(f,12, 1)	/* �������ƫ��(��) */
#define P_val_cool_sub_diff(f)                              P_g_i(f,12, 2)	/* ����ж��ƫ��(��) */
#define P_val_heat_add_diff(f)                              P_g_i(f,12, 3)	/* ���ȼ���ƫ��(��) */
#define P_val_heat_sub_diff(f)                              P_g_i(f,12, 4)	/* ����ж��ƫ��(��) */
#define P_val_TR_diff(f)                                    P_g_i(f,12, 5)	/* ���»ز� */
#define P_val_cool_set_min(f)                               P_g_i(f,12, 6)	/* ����������� */
#define P_val_cool_set_max(f)                               P_g_i(f,12, 7)	/* ����������� */
#define P_val_heat_set_min(f)                               P_g_i(f,12, 8)	/* ���Ȼ������� */
#define P_val_heat_set_max(f)                               P_g_i(f,12, 9)	/* ���Ȼ������� */
#define P_val_cool_min_TA(f)                                P_g_i(f,12,10)	/* ������ͻ��� */
#define P_val_cool_max_TA(f)                                P_g_i(f,12,11)	/* ������߻��� */
#define P_val_heat_min_TA(f)                                P_g_i(f,12,12)	/* ������ͻ��� */
#define P_val_heat_max_TA(f)                                P_g_i(f,12,13)	/* ������߻��� */
#define P_val_hotw_min_TA(f)                                P_g_i(f,12,14)	/* ��ˮ��ͻ��� */
#define P_val_hotw_max_TA(f)                                P_g_i(f,12,15)	/* ��ˮ��߻��� */
#define P_val_auto_cool_min_TA(f)                           P_g_i(f,12,16)	/* �Զ�������ͻ� */
#define P_val_auto_heat_max_TA(f)                           P_g_i(f,12,17)	/* �Զ�������߻� */
#define P_val_holiday_away_sv_heat(f)                       P_g_i(f,12,18)	/* ����������� */
#define P_val_holiday_away_sv_hotw(f)                       P_g_i(f,12,19)	/* ���������ˮ */
#define P_val_eco_run_time(f)                               P_g_i(f,12,20)	/* ��������ʱ�� */
#define P_val_use_IBH(f)                                    P_g_i(f,12,21)	/* �������� */
#define P_val_use_AHS(f)                                    P_g_i(f,12,22)	/* �ⲿ��Դ */
#define P_val_IBH_high_dT(f)                                P_g_i(f,12,23)	/* ���ȿ������²� */
#define P_val_IBH_low_dT(f)                                 P_g_i(f,12,24)	/* ���ȿ������²� */
#define P_val_IBH_gear_num(f)                               P_g_i(f,12,25)	/* �������ȵ��� */
#define P_val_IBH_open_dly(f)                               P_g_i(f,12,26)	/* �������ȿ����� */
#define P_val_IBH_open_env(f)                               P_g_i(f,12,27)	/* �������ȿ����� */
#define P_val_IBH_open_diff(f)                              P_g_i(f,12,28)	/* �������ȿ��²� */
#define P_val_TBH_open_dly(f)                               P_g_i(f,12,29)	/* ˮ����ȿ����� */
#define P_val_TBH_on_TA(f)                                  P_g_i(f,12,30)	/* ˮ����ȿ����� */
#define P_val_AHS_open_dly(f)                               P_g_i(f,12,31)	/* �ⲿ��Դ������ */
#define P_val_AHS_open_diff(f)                              P_g_i(f,12,32)	/* �ⲿ��Դ���²� */
#define P_val_AHS_close_diff(f)                             P_g_i(f,12,33)	/* �ⲿ��Դ���²� */
#define P_val_AHS_open_env(f)                               P_g_i(f,12,34)	/* �ⲿ��Դ������ */
#define P_val_hotw_comp_on_max(f)                           P_g_i(f,12,35)	/* ����ˮ���ʱ�� */
#define P_val_air_comp_on_max(f)                            P_g_i(f,12,36)	/* ����ˮ�������� */
#define P_val_fcu_egy_cpst_obj(f)                           P_g_i(f,12,37)  /* �ܵ��������� */
#define P_val_cm_temp_diff(f)                               P_g_i(f,12,38)	/* ѹ��ȫ���²� */
#define P_val_TF_heat_step(f)                               P_g_i(f,12,39)	/* �̼����Ȳ��� */
#define P_val_TF_heat_freq(f)                               P_g_i(f,12,40)	/* �̼�����Ƶ�� */
#define P_val_TF_heat_time(f)                               P_g_i(f,12,41)	/* �̼�����ʱ�� */
#define P_val_TF_cool_step(f)                               P_g_i(f,12,42)	/* �̼����䲽�� */
#define P_val_TF_cool_freq(f)                               P_g_i(f,12,43)	/* �̼�����Ƶ�� */
#define P_val_TF_cool_time(f)                               P_g_i(f,12,44)	/* �̼�����ʱ�� */
#define P_val_TF_IBH_time(f)                                P_g_i(f,12,45)	/* �̼����ʱ�� */
#define P_val_TF_heat_fan(f)                                P_g_i(f,12,46)	/* �̼����ȷ��� */
#define P_val_TF_cool_fan(f)                                P_g_i(f,12,47)	/* �̼�������� */
#define val_ctrl_period                                     (P_val(val_ctrl_period                    ))/* 01 ������������ */
#define val_cool_add_diff                                   (P_val(val_cool_add_diff                  ))/* 02 �������ƫ��(��) */
#define val_cool_sub_diff                                   (P_val(val_cool_sub_diff                  ))/* 03 ����ж��ƫ��(��) */
#define val_heat_add_diff                                   (P_val(val_heat_add_diff                  ))/* 04 ���ȼ���ƫ��(��) */
#define val_heat_sub_diff                                   (P_val(val_heat_sub_diff                  ))/* 05 ����ж��ƫ��(��) */
#define val_TR_diff                                      (10*P_val(val_TR_diff                        ))/* 06 ���»ز� */
#define val_cool_set_min                                    (P_val(val_cool_set_min                   ))/* 07 ����������� */
#define val_cool_set_max                                    (P_val(val_cool_set_max                   ))/* 08 ����������� */
#define val_heat_set_min                                    (P_val(val_heat_set_min                   ))/* 09 ���Ȼ������� */
#define val_heat_set_max                                    (P_val(val_heat_set_max                   ))/* 10 ���Ȼ������� */
#define val_cool_min_TA                                  (10*P_val(val_cool_min_TA                    ))/* 11 ������ͻ��� */
#define val_cool_max_TA                                  (10*P_val(val_cool_max_TA                    ))/* 12 ������߻��� */
#define val_heat_min_TA                                  (10*P_val(val_heat_min_TA                    ))/* 13 ������ͻ��� */
#define val_heat_max_TA                                  (10*P_val(val_heat_max_TA                    ))/* 14 ������߻��� */
#define val_hotw_min_TA                                  (10*P_val(val_hotw_min_TA                    ))/* 15 ��ˮ��ͻ��� */
#define val_hotw_max_TA                                  (10*P_val(val_hotw_max_TA                    ))/* 16 ��ˮ��߻��� */
#define val_auto_cool_min_TA                             (10*P_val(val_auto_cool_min_TA               ))/* 17 �Զ�������ͻ� */
#define val_auto_heat_max_TA                             (10*P_val(val_auto_heat_max_TA               ))/* 18 �Զ�������߻� */
#define val_holiday_away_sv_heat                         (10*P_val(val_holiday_away_sv_heat           ))/* 19 ����������� */
#define val_holiday_away_sv_hotw                         (10*P_val(val_holiday_away_sv_hotw           ))/* 20 ���������ˮ */
#define val_eco_run_time                              (QK_60*P_val(val_eco_run_time                   ))/* 21 ��������ʱ�� */
#define val_use_IBH                                         (P_val(val_use_IBH                        ))/* 22 �������� */
#define val_use_AHS                                         (P_val(val_use_AHS                        ))/* 23 �ⲿ��Դ */
#define val_IBH_high_dT                                  (10*P_val(val_IBH_high_dT                    ))/* 24 ���ȿ������²� */
#define val_IBH_low_dT                                   (10*P_val(val_IBH_low_dT                     ))/* 25 ���ȿ������²� */
#define val_IBH_gear_num                                    (P_val(val_IBH_gear_num                   ))/* 26 �������ȵ��� */
#define val_IBH_open_dly                              (QK_60*P_val(val_IBH_open_dly                   ))/* 27 �������ȿ����� */
#define val_IBH_open_env                                 (10*P_val(val_IBH_open_env                   ))/* 28 �������ȿ����� */
#define val_IBH_open_diff                                (10*P_val(val_IBH_open_diff                  ))/* 29 �������ȿ��²� */
#define val_TBH_open_dly                              (QK_60*P_val(val_TBH_open_dly                   ))/* 30 ˮ����ȿ����� */
#define val_TBH_on_TA                                    (10*P_val(val_TBH_on_TA                      ))/* 31 ˮ����ȿ����� */
#define val_AHS_open_dly                              (QK_60*P_val(val_AHS_open_dly                   ))/* 32 �ⲿ��Դ������ */
#define val_AHS_open_diff                                (10*P_val(val_AHS_open_diff                  ))/* 33 �ⲿ��Դ���²� */
#define val_AHS_close_diff                               (10*P_val(val_AHS_close_diff                 ))/* 34 �ⲿ��Դ���²� */
#define val_AHS_open_env                                 (10*P_val(val_AHS_open_env                   ))/* 35 �ⲿ��Դ������ */
#define val_hotw_comp_on_max                          (QK_60*P_val(val_hotw_comp_on_max               ))/* 36 ����ˮ���ʱ�� */
#define val_air_comp_on_max                           (QK_60*P_val(val_air_comp_on_max                ))/* 37 ����ˮ�������� */
#define val_fcu_egy_cpst_obj                                (P_val(val_fcu_egy_cpst_obj               ))/* 38 �ܵ��������� */
#define val_cm_temp_diff                                 (10*P_val(val_cm_temp_diff                   ))/* 39 ѹ��ȫ���²� */
#define val_TF_heat_step                                    (P_val(val_TF_heat_step                   ))/* 40 �̼����Ȳ��� */
#define val_TF_heat_freq                                (100*P_val(val_TF_heat_freq                   ))/* 41 �̼�����Ƶ�� */
#define val_TF_heat_time                                    (P_val(val_TF_heat_time                   ))/* 42 �̼�����ʱ�� */
#define val_TF_cool_step                                    (P_val(val_TF_cool_step                   ))/* 43 �̼����䲽�� */
#define val_TF_cool_freq                                (100*P_val(val_TF_cool_freq                   ))/* 44 �̼�����Ƶ�� */
#define val_TF_cool_time                                    (P_val(val_TF_cool_time                   ))/* 45 �̼�����ʱ�� */
#define val_TF_IBH_time                                     (P_val(val_TF_IBH_time                    ))/* 46 �̼����ʱ�� */
#define val_TF_heat_fan                                     (P_val(val_TF_heat_fan                    ))/* 47 �̼����ȷ��� */
#define val_TF_cool_fan                                     (P_val(val_TF_cool_fan                    ))/* 48 �̼�������� */

/* ����ʹ�ø������� */
#define HEAT_USE_IBH                   (((val_use_IBH==IBH_HEAT_ONLY) || (val_use_IBH==IBH_HEAT_HOTW)) && (IBH_cfg_type!=IBH_NONE))
/* ����ˮʹ�ø������� */
#define HOTW_USE_IBH                   (((val_use_IBH==IBH_HOTW_ONLY) || (val_use_IBH==IBH_HEAT_HOTW)) && (IBH_cfg_type!=IBH_NONE))
/* ����ʹ���ⲿ��Դ */
#define HEAT_USE_AHS                    ((val_use_AHS==AHS_HEAT_ONLY) || (val_use_AHS==AHS_HEAT_HOTW))
/* ����ˮʹ���ⲿ��Դ */
#define HOTW_USE_AHS                    ((val_use_AHS==AHS_HOTW_ONLY) || (val_use_AHS==AHS_HEAT_HOTW))


/* 14���̲��� */
#define P_val_PUMPf_ctrl(f)                                 P_g_i(f,13, 0)	/* ��Ƶˮ�ÿ��� */
#define P_val_bak_13_02(f)                                  P_g_i(f,13, 1)	/* ���� */
#define P_val_pump_sv(f)                                    P_g_i(f,13, 2)	/* ˮ��Ŀ��ת�� */
#define P_val_PUMPf_model(f)                                P_g_i(f,13, 3)	/* ��Ƶˮ�ñ��� */
#define P_val_pump_sv_flow(f)                               P_g_i(f,13, 4)	/* ˮ��Ŀ������ */
#define P_val_PUMPret_run_time(f)                           P_g_i(f,13, 5)	/* �»�ˮ������ */
#define P_val_PUMPret_disinfect(f)                          P_g_i(f,13, 6)	/* �»�ˮ��ɱ�� */
#define P_val_PUMPret_timer(f)                              P_g_i(f,13, 7)	/* �»�ˮ�ö�ʱ */
#define P_val_floor_preheat_temp(f)                         P_g_i(f,13, 8)	/* ��ůԤ���¶� */
#define P_val_floor_preheat_interval(f)                     P_g_i(f,13, 9)	/* ��ůԤ�ȼ�� */
#define P_val_floor_preheat_max(f)                          P_g_i(f,13,10)	/* ��ůԤ��ʱ�� */
#define P_val_S3V3_on_TFLin_dT(f)                           P_g_i(f,13,11)	/* ��ůˮ�»ز� */
#define P_val_S3V3_on_TR_dT(f)                              P_g_i(f,13,12)	/* ��ů���»ز� */
#define P_val_floor_dry_head(f)                             P_g_i(f,13,13)	/* ��ů���ǰ */
#define P_val_floor_dry_mid(f)                              P_g_i(f,13,14)	/* ��ů����� */
#define P_val_floor_dry_tail(f)                             P_g_i(f,13,15)	/* ��ů��ɺ� */
#define P_val_floor_dry_temp(f)                             P_g_i(f,13,16)	/* ��ů����¶� */
#define P_val_zoneA_cl_terminal_device(f)                   P_g_i(f,13,17)	/* A������ĩ�� */
#define P_val_zoneA_ht_terminal_device(f)                   P_g_i(f,13,18)	/* A������ĩ�� */
#define P_val_zoneB_cl_terminal_device(f)                   P_g_i(f,13,19)	/* B������ĩ�� */
#define P_val_zoneB_ht_terminal_device(f)                   P_g_i(f,13,20)	/* B������ĩ�� */
#define P_val_fcu_num(f)                                    P_g_i(f,13,21)  /* ĩ�˸���    */
#define P_val_fcu_base_addr(f)                              P_g_i(f,13,22)  /* ĩ����ʼ��ַ */
#define P_val_fcu_M_start_S(f)                              P_g_i(f,13,23)  /* ��������ĩ�� */
#define P_val_fcu_M_close_S(f)                              P_g_i(f,13,24)  /* �����ر�ĩ�� */
#define P_val_fcu_S_start_M(f)                              P_g_i(f,13,25)  /* ĩ�˿������� */
#define P_val_fcu_mode_set(f)                               P_g_i(f,13,26)  /* ��ĩģʽ���� */
#define P_val_fcu_egy_cpst(f)                               P_g_i(f,13,27)  /* ĩ���ܵ����� */
#define P_val_PUMPsl_on_TWHt(f)                             P_g_i(f,13,28)	/* ̫���ܿ���ˮ�� */
#define P_val_PUMPsl_THWt_dT(f)                             P_g_i(f,13,29)	/* ̫���ܻز� */
#define P_val_PUMPsl_on_TSL(f)                              P_g_i(f,13,30)	/* ̫���ܿ���ˮ�� */
#define P_val_PUMPsl_TSL_dT(f)                              P_g_i(f,13,31)	/* ̫���ܿ����ز� */
#define P_val_recycle_enter(f)                              P_g_i(f,13,32)  /* �շ��������� */
#define P_val_recycle_eev_step(f)                           P_g_i(f,13,33)  /* �շ��������� */
#define P_val_recycle_lp_dly(f)                             P_g_i(f,13,34)  /* �շ���ѹ���� */
#define P_val_S3V3_act_period(f)                            P_g_i(f,13,35)	/* S3V3�������� */
#define P_val_S3V3_ext_circue_min_time(f)                   P_g_i(f,13,36)	/* ��ѭ����Сʱ�� */
#define P_val_bak_13_38(f)                                  P_g_i(f,13,37)	/* ���� */
#define P_val_bak_13_39(f)                                  P_g_i(f,13,38)	/* ���� */
#define P_val_pcb_num(f)                                    P_g_i(f,13,39)	/* ģ����� */
#define P_val_hotw_pcb_num(f)                               P_g_i(f,13,40)	/* ��ˮģ���� */
#define P_val_bak_13_42(f)                                  P_g_i(f,13,41)	/* ���� */
#define P_val_bak_13_43(f)                                  P_g_i(f,13,42)	/* ���� */
#define P_val_bak_13_44(f)                                  P_g_i(f,13,43)	/* ���� */
#define P_val_bak_13_45(f)                                  P_g_i(f,13,44)	/* ���� */
#define P_val_bms_format(f)                                 P_g_i(f,13,45)	/* BMS���ݸ�ʽ */
#define P_val_bms_baud_rate(f)                              P_g_i(f,13,46)	/* BMS������ */
#define P_val_bms_addr(f)                                   P_g_i(f,13,47)	/* BMSͨѶ��ַ */
#define val_PUMPf_ctrl                                      (P_val(val_PUMPf_ctrl                     ))/* 01 ��Ƶˮ�ÿ��� */
#define val_bak_13_02                                       (P_val(val_bak_13_02                      ))/* 02 ���� */
#define val_pump_sv                                         (P_val(val_pump_sv                        ))/* 03 ˮ��Ŀ��ת�� */
#define val_PUMPf_model                                     (P_val(val_PUMPf_model                    ))/* 04 ��Ƶˮ�ñ��� */
#define val_pump_sv_flow                                    (P_val(val_pump_sv_flow                   ))/* 05 ˮ��Ŀ������ */
#define val_PUMPret_run_time                             (60*P_val(val_PUMPret_run_time               ))/* 06 �»�ˮ������ */
#define val_PUMPret_disinfect                               (P_val(val_PUMPret_disinfect              ))/* 07 �»�ˮ��ɱ�� */
#define val_PUMPret_timer                                   (P_val(val_PUMPret_timer                  ))/* 08 �»�ˮ�ö�ʱ */
#define val_floor_preheat_temp                           (10*P_val(val_floor_preheat_temp             ))/* 09 ��ůԤ���¶� */
#define val_floor_preheat_interval                    (QK_60*P_val(val_floor_preheat_interval         ))/* 10 ��ůԤ�ȼ�� */
#define val_floor_preheat_max                               (P_val(val_floor_preheat_max              ))/* 11 ��ůԤ��ʱ�� */
#define val_S3V3_on_TFLin_dT                             (10*P_val(val_S3V3_on_TFLin_dT               ))/* 12 ��ůˮ�»ز� */
#define val_S3V3_on_TR_dT                                (10*P_val(val_S3V3_on_TR_dT                  ))/* 13 ��ů���»ز� */
#define val_floor_dry_head                                  (P_val(val_floor_dry_head                 ))/* 14 ��ů���ǰ */
#define val_floor_dry_mid                                   (P_val(val_floor_dry_mid                  ))/* 15 ��ů����� */
#define val_floor_dry_tail                                  (P_val(val_floor_dry_tail                 ))/* 16 ��ů��ɺ� */
#define val_floor_dry_temp                               (10*P_val(val_floor_dry_temp                 ))/* 17 ��ů����¶� */
#define val_zoneA_cl_terminal_device                        (P_val(val_zoneA_cl_terminal_device       ))/* 18 A������ĩ�� */
#define val_zoneA_ht_terminal_device                        (P_val(val_zoneA_ht_terminal_device       ))/* 19 A������ĩ�� */
#define val_zoneB_cl_terminal_device                        (P_val(val_zoneB_cl_terminal_device       ))/* 20 B������ĩ�� */
#define val_zoneB_ht_terminal_device                        (P_val(val_zoneB_ht_terminal_device       ))/* 21 B������ĩ�� */
#define val_fcu_num                                         (P_val(val_fcu_num                        ))/* 22 ĩ�˸���    */
#define val_fcu_base_addr                                   (P_val(val_fcu_base_addr                  ))/* 23 ĩ����ʼ��ַ */
#define val_fcu_M_start_S                                   (P_val(val_fcu_M_start_S                  ))/* 24 ��������ĩ�� */
#define val_fcu_M_close_S                                   (P_val(val_fcu_M_close_S                  ))/* 25 �����ر�ĩ�� */
#define val_fcu_S_start_M                                   (P_val(val_fcu_S_start_M                  ))/* 26 ĩ�˿������� */
#define val_fcu_mode_set                                    (P_val(val_fcu_mode_set                   ))/* 27 ��ĩģʽ���� */
#define val_fcu_egy_cpst                                 (10*P_val(val_fcu_egy_cpst                   ))/* 28 ĩ���ܵ����� */
#define val_PUMPsl_on_TWHt                               (10*P_val(val_PUMPsl_on_TWHt                 ))/* 29 ̫������ˮˮ�� */
#define val_PUMPsl_THWt_dT                               (10*P_val(val_PUMPsl_THWt_dT                 ))/* 30 ̫������ˮ�ز� */
#define val_PUMPsl_on_TSL                                (10*P_val(val_PUMPsl_on_TSL                  ))/* 31 ̫���ܿ���ˮ�� */
#define val_PUMPsl_TSL_dT                                (10*P_val(val_PUMPsl_TSL_dT                  ))/* 32 ̫���ܿ����ز� */
#define val_recycle_enter                                   (P_val(val_recycle_enter                  ))/* 33 �շ��������� */
#define val_recycle_eev_step                                (P_val(val_recycle_eev_step               ))/* 34 �շ��������� */
#define val_recycle_lp_dly                                  (P_val(val_recycle_lp_dly                 ))/* 35 �շ���ѹ���� */
#define val_S3V3_act_period                                 (P_val(val_S3V3_act_period                ))/* 36 S3V3�������� */
#define val_S3V3_ext_circue_min_time                        (P_val(val_S3V3_ext_circue_min_time       ))/* 37 ��ѭ����Сʱ�� */
#define val_bak_13_38                                       (P_val(val_bak_13_38                      ))/* 38 ���� */
#define val_bak_13_39                                       (P_val(val_bak_13_39                      ))/* 39 ���� */
#define val_pcb_num                                         (P_val(val_pcb_num                        ))/* 40 ģ����� */
#define val_hotw_pcb_num                                    (P_val(val_hotw_pcb_num                   ))/* 41 ��ˮģ���� */
#define val_bak_13_42                                       (P_val(val_bak_13_42                      ))/* 42 ���� */
#define val_bak_13_43                                       (P_val(val_bak_13_43                      ))/* 43 ���� */
#define val_bak_13_44                                       (P_val(val_bak_13_44                      ))/* 44 ���� */
#define val_bak_13_45                                       (P_val(val_bak_13_45                      ))/* 45 ���� */
#define val_bms_format                                      (P_val(val_bms_format                     ))/* 46 BMS���ݸ�ʽ */
#define val_bms_baud_rate                                   (P_val(val_bms_baud_rate                  ))/* 47 BMS������ */
#define val_bms_addr                                        (P_val(val_bms_addr                       ))/* 48 BMSͨѶ��ַ */

#define val_PUMPf_init_stable                               (10)	/* ˮ�ó�ʼά�� */
#define val_PUMPf_ctrl_period                               (3)		/* ˮ�õ������� */
#define val_PUMPf_flow_diff                                 (50)	/* ˮ�������ز� */
#define val_PUMPf_power_diff                                (5)	    /* ˮ�ù��ʻز� */

#define val_ster_start_week                                 0/* 14 ɱ���������� */
#define val_ster_start_hour                                 0/* 15 ɱ������ʱ */
#define val_ster_start_minute                               0/* 16 ɱ�������� */

#define S3V_MIN_PERIOD  360		/* ��ͨ����С�������� */
#define S3V_MIN_ON      20		/* ��ͨ����С����ʱ�� */
#define S3V_MIN_OFF     20		/* ��ͨ����С�ر�ʱ�� */

#define USE_PUMPf_CTRL (TF_VAL(val_PUMPf_ctrl, PUMPF_CTRL_RPM))

enum {
    TD_RAD,
    TD_FCU,
    TD_FLH,
};

#define ZONE_A_TERMINAL_DEVICE         ((ac_mode==MODE_COOL)?val_zoneA_cl_terminal_device:val_zoneA_ht_terminal_device)
#define ZONE_B_TERMINAL_DEVICE         ((ac_mode==MODE_COOL)?val_zoneB_cl_terminal_device:val_zoneB_ht_terminal_device)
#define ZONE_A_HEAT_USE_FLH            (val_zoneA_ht_terminal_device==TD_FLH)
#define ZONE_B_HEAT_USE_FLH            (val_zoneB_ht_terminal_device==TD_FLH)
#define ONE_ZONE_USE_FLH               ((!USE_DUAL_ZONE_CTRL)&&ZONE_A_HEAT_USE_FLH)
#define DUAL_ZONE_USE_FLH              ((USE_DUAL_ZONE_CTRL)&&(ZONE_A_HEAT_USE_FLH||ZONE_B_HEAT_USE_FLH))
#define HEAT_USE_FLH                   (ONE_ZONE_USE_FLH||DUAL_ZONE_USE_FLH)

/* 14����Ƶ�ʱ� */
#ifndef USE_WF_OLD
    #define TAB_UP_X_CL 4	/* ����ά�� */
    #define TAB_UP_Y_CL 4	/* ˮ��ά�� */
    #define TAB_DN_X_CL 4	/* ����ά�� */
    #define TAB_DN_Y_CL 4	/* ˮ��ά�� */
     
    #define TAB_UP_X_HT 6	/* ����ά�� */
    #define TAB_UP_Y_HT 4	/* ˮ��ά�� */
    #define TAB_DN_X_HT 4	/* ����ά�� */
    #define TAB_DN_Y_HT 4	/* ˮ��ά�� */
     
    /* ��Ӧpara.c��ʼ��� */
    #define COOL_UP_BGN             0
    #define COOL_DN_BGN             16
    #define HEAT_UP_BGN             20
    #define HEAT_DN_BGN             44
     
    #define val_cl_up_env_in(y, x)      (100*para_C[GROUP_WF][COOL_UP_BGN+(y)*TAB_UP_X_CL+(x)])   // ���ϻ�X��Y
    #define val_ht_up_env_in(y, x)      (100*para_C[GROUP_WF][HEAT_UP_BGN+(y)*TAB_UP_X_HT+(x)])   // ���ϻ�X��Y
    #define val_cl_dn_env_in(y, x)      (100*para_C[GROUP_WF][COOL_DN_BGN+(y-y)*TAB_DN_X_CL+(x)]) // ���»�X��Y:���л���һά����
    #define val_ht_dn_env_in(y, x)      (100*para_C[GROUP_WF][HEAT_DN_BGN+(y-y)*TAB_DN_X_HT+(x)]) // ���»�X��Y:���л���һά����
#else
    #define TAB_UP_X_CL 4	/* ����ά�� */
    #define TAB_UP_Y_CL 3	/* ˮ��ά�� */
    #define TAB_DN_X_CL 4	/* ����ά�� */
    #define TAB_DN_Y_CL 3	/* ˮ��ά�� */
     
    #define TAB_UP_X_HT 6	/* ����ά�� */
    #define TAB_UP_Y_HT 3	/* ˮ��ά�� */
    #define TAB_DN_X_HT 4	/* ����ά�� */
    #define TAB_DN_Y_HT 3	/* ˮ��ά�� */
     
    /* ��Ӧpara.c��ʼ��� */
    #define COOL_UP_BGN             0
    #define COOL_DN_BGN             12
    #define HEAT_UP_BGN             16
    #define HEAT_DN_BGN             34
     
    #define val_cl_up_env_in(y, x)      (100*para_C[GROUP_WF][COOL_UP_BGN+(y)*TAB_UP_X_CL+(x)])   // ���ϻ�X��Y
    #define val_ht_up_env_in(y, x)      (100*para_C[GROUP_WF][HEAT_UP_BGN+(y)*TAB_UP_X_HT+(x)])   // ���ϻ�X��Y
    #define val_cl_dn_env_in(y, x)      (100*para_C[GROUP_WF][COOL_DN_BGN+(y-y)*TAB_DN_X_CL+(x)]) // ���»�X��Y:���л���һά����
    #define val_ht_dn_env_in(y, x)      (100*para_C[GROUP_WF][HEAT_DN_BGN+(y)*TAB_DN_X_HT+(x)])   // ���»�X��Y
#endif

//-----------------------------------------------------------------------------------------------------
//--�̶�����-------------------------------------------------------------------------------------------

#define val_00_board_alarm      (0)                     // 0--ϵͳ��Դ����  1--00�ŵ�Դ����
#define val_hold_evi_time       (0)                     // ��ͣ����ʱ��0������ͣ��������
#define val_hold_evi_cyc        (0)                     // ��ͣ��������
#define val_antifrez_alarmtemp  (30)                    // ���������¶�
#define val_antifrez_resettemp  (70)                    // ������λ�¶�
#define sv_economy_mode         (0)                     // ����ģʽ:1����0�ر�

#define val_defrost_fan_dly             (10)        /* �˳�˪�������ʱ */
#define val_defrost_cm_on_time          (5*60)      /* ��˪����ʱ�� */

/* �˳�˪ʱ��ʱ��ڵ� */
#define val_defr_freq_reduce_time   20      /* Ԥ���˳�˪ʱ�Ľ�Ƶʱ�� */
#define DEFROST_DLY_1    (val_defr_freq_reduce_time)
#define DEFROST_DLY_2    (DEFROST_DLY_1+val_defrost_stop_cm_tm_exit)
#define DEFROST_DLY_3    (DEFROST_DLY_2+val_defrost_four_tm_exit)

#define DEFR_CHG_FREQ(cm)    (s_get_cm_status(cm, CM_DEFROST_OFF)\
                              ? val_invt_rps_min : val_defrost_chg_freq)

PARA_EXT I16 para_splash;	/* ����ҳ */

PARA_EXT U08 sh_language;	/* ͯ�� */

PARA_EXT U08 sh_temp_unit;	/* �¶ȵ�λ */

#define TEMP_IS_CENT		(sh_temp_unit == SH_SET_UNIT_C)
#define TEMP_IS_FARA		(sh_temp_unit == SH_SET_UNIT_F)

//======================================================================================


#define GET_INIT_PARA()             (gIO.io_init)      // ��ȡ������ʼ��״̬
#define SET_INIT_PARA(x)            (gIO.io_init=x)    // ���ò�����ʼ��״̬

#define ACTION_IDLE     0   //
#define ACTION_START    1   // ����ִ�п�ʼ
#define ACTION_ING      2   // ����ִ����
#define ACTION_SUCC     3   // ����ִ�гɹ�
#define ACTION_FAIL     4   // ����ִ��ʧ��


//======================================================================================
extern PARAMITEM ParamPri[];
PARA_EXT I16 F_To_C(I16 F, U16 sign);
PARA_EXT I16 C_To_F(I16 C, U16 sign);
//PARA_EXT U08 para_is_shift(U08 n, U08 m);
#define     CTUS_10         10
#define     CTUS_100        30
#define     CTUS_1000       80

#define     CAL_CYCLE       0   // ѭ������
#define     CAL_ETOE        1   // end to end ��ѭ������
PARA_EXT I16 key_continuous_add(I16 val, I16 max, I16 min, U08 opt);
PARA_EXT I16 key_continuous_sub(I16 val, I16 max, I16 min, U08 opt);

PARA_EXT U08 PARA_Modify(U08 *buf, U16 addr, U16 num);
PARA_EXT U08 PARA_GetVal(U08 *buf, U16 addr, U16 num);
PARA_EXT U08 PARA_GetAttrib(U08 *buf, U16 addr, U16 num);
PARA_EXT U08 PARA_GetInfo(U08 *buf, U16 addr, U16 num);

PARA_EXT void para_check(void);

PARA_EXT void Task_ID_Init(void);
PARA_EXT void Task_ID_Read(void);

PARA_EXT I16 para_get_val(U08 n,U08 m);
PARA_EXT U16 para_get_sign(U08 n, U08 m);
PARA_EXT U08 para_is_hide(U08 n, U08 m, U08 grade);
PARA_EXT void PARA_SetAttrib(U08 n, U08 m, U16 *result);
PARA_EXT U08 para_is_nexch(U08 n, U08 m);
PARA_EXT void para_add(U08 n, U08 m, I16 *v, U08 bit_pos);
PARA_EXT void para_sub(U08 n, U08 m, I16 *v, U08 bit_pos);
PARA_EXT U08 para_is_bit(U08 n, U08 m);

// �����ӵ����ݷ��ʽӿ�
PARA_EXT void DataAccess_AUTO_RUN(U08 opt);
PARA_EXT void DataAccess_CompRuntime(U08 opt);
PARA_EXT void DataAccess_SysRuntime(U08 opt);
PARA_EXT void DataAccess_Language(U08 opt);
PARA_EXT void DataAccess_Temp_unit(U08 opt);
PARA_EXT void DataAccess_HistoryErr(U08 opt);
PARA_EXT void DataAccess_InvtExceptInfo(U08 opt, U08 idx);
PARA_EXT void DataAccess_Timer(U08 opt);
PARA_EXT void DataAccess_FacPara(U08 opt);
PARA_EXT void DataAccess_LimitPara(U08 opt);
PARA_EXT void DataStore_Parameters(void);
PARA_EXT void DataStore_EEPROM(void);
PARA_EXT void Password_ReadAll(void);
PARA_EXT void Password_InitAll(void);
PARA_EXT void para_check_invt_value(void);
PARA_EXT void Password_super_cal(U16 Passcode);
PARA_EXT void DataAccess_SkipPreheatCnt(U08 opt);
PARA_EXT void DataAccess_holiday(U08 opt);

#define GET_MIN 0       // ��ȡ��Сֵ
#define GET_MAX 1       // ��ȡ���ֵ
PARA_EXT I16 GetSettingRange(I16 CtrlObject, U08 DataType, U08 Mode);

PARA_EXT PARAMITEM* PG_GetAddr_ParamPri(U08 n);
PARA_EXT PARAMITEM* PG_GetAddr_ParamLimit(U08 n);
PARA_EXT U08 PG_GetGroupNum(const PARAMITEM *ParamGroup);
PARA_EXT BOOL PG_Is_Show(const PARAMITEM *ParamGroup, U16 ParaGrade);

PARA_EXT void para_set_real(U08 n, U08 m, I16 v, U08 temp_unit);
PARA_EXT I16 para_get_min_real(U08 n, U08 m, U08 temp_unit);
PARA_EXT I16 para_get_max_real(U08 n, U08 m, U08 temp_unit);
PARA_EXT void para_set_cent(U08 n, U08 m, I16 v);
PARA_EXT I16 para_get_min_cent(U08 n, U08 m);
PARA_EXT I16 para_get_max_cent(U08 n, U08 m);
PARA_EXT void para_set(U08 n, U08 m, I16 v);
PARA_EXT I16 para_get_min(U08 n, U08 m);
PARA_EXT I16 para_get_max(U08 n, U08 m);

PARA_EXT BOOL PARA_ParaWithinMacType(U08 n, U08 m, U16 *idx);
PARA_EXT void pwd_time_write_to_eeprom(void);
PARA_EXT void pwd_time_read_from_eeprom(void);
PARA_EXT void DataAccess_PowerUsageWeek(U08 opt);
PARA_EXT void DataAccess_PowerUsageYear(U08 opt);
PARA_EXT void eeprom_limit_time_write(void);
PARA_EXT U08 para_get_dot(U08 grp_id, U08 item_id);
PARA_EXT U08 para_get_set_temp_addr(void);

PARA_EXT U08 password_information(U08 *buf, U16 addr, U16 num, U08 opt);
PARA_EXT U08 para_broadcast(U08 com);
PARA_EXT U08 m_password_broadcast(U08 com);
PARA_EXT I16 para_AT_use_get(void);
PARA_EXT void para_AT_update_ht(void);
PARA_EXT U08 para_AT_get_group_ht(void);
#endif

