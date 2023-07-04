/**********************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾
 All rights reserved.

 �ļ����� : dm603.c
 �ļ����� : ����dm603��ʾ������

 ������   : Ԭ��
 �������� : 2013.09.24

 �޸���   : ������
 �޸����� : 2017.02.05
 ============================ �޸ļ�¼ ===============================
 �޸���  : --
 ����汾: --
 �������: --
 �޸�����: --

**********************************************************************/

#define DM603_GLOBAL

#include "includes.h"

/* �ڲ��������� */
#define MAX_FACE_LAYER  5       /* �������(�����������ʱ����Ҫ���ĸö���) */

#define FECE_ENTER          0               /* ������һ����� */
#define FECE_EXIT           1               /* �˻���һ����� */

#define     PARA_SELECT     0   /* ѡ�в����� */
#define     PARA_SETTING    1   /* ѡ�в���ֵ */

#ifdef USE_HMI_DISP
static U08 dm603_menu_sub[MAX_HMI];         /* �ӽ��� */
static U08 dm603_sub_bak[MAX_HMI];          /* ���� */
static U08 dm603_menu[MAX_HMI];             /* ����˵� */
static U08 dm603_item[MAX_HMI];             /* ����ѡ�� */
//static U08 dm603_cursor[MAX_HMI];         /* ������ */
//static U08 dm603_sub_item[MAX_HMI];           /* �ӽ���ѡ�� */


/* ����״̬ */
static U08 FaceStatus[MAX_HMI];         /* ����״̬ */
static U08 FaceStatusBak[MAX_HMI];      /* ����״̬���� */

static U08 dm603_pass_grade[MAX_HMI];       /* ����ȼ���ָ�����������Ľ������ */
static U08 dm603_pass_buf[MAX_PASS_BIT];    /* ���뻺�� */
static U08 dm603_pass_new[MAX_PASS_BIT];    /* ������� */
static U08 dm603_pass_bak[MAX_PASS_BIT];    /* ���뱸�� */

static BOOL fg_limit_cancel = FALSE;
/* �ڲ��������� */
//static U08 para_n_tbl[GROUP_MAX];
static U08 para_m_tbl[GROUP_SIZE];  //NOTE-CXW �������������Ĵ�С����GROUP_SIZE����������ͨѶ������Ĵ�С������������

static void dm603_face_pass_modify(U08 Pass_toModify,U08 *disp_buf);
//static U08 dm603_face_para_set_chg_m(U08 n, U08 *new_tbl, U16 grade);
static void dm603_face_para_set_disp_v(U08 opt, I16 val, U08 num, U16 sign, const SMALL *para, U08 *disp_buf);
//static void dm603_face_para_set_disp_b(U08 opt, I16 val, U08 num, U08 b, const U16 **msg, SMALL *para, U08 *disp_buf);
static void dm603_push_face_status(void);
static void dm603_pop_face_status(void);
static void dm603_clr_face_status(U08 hmi_num);
static U08 dm603_face_para_SetValue(PARAMITEM *ParamGroup,const SMALL *para_msg, U08 Item_min, U08 Item_max,U08 *disp_buf);
static void dm603_face_menu_func_user_mode(U08 *disp_buf);
static void dm603_set_nextface_status(U16 FaceId);
static void dm603_get_nextface_status(void);
static void dm603_disp_err(U08 err_pcb, U08 err_num, U08 err_tag, U08 row, U08 *disp_buf);

/* ��ʱ����ѡ�� */
const U16 msg_menu_time_select[] =
{
    dm603_face_time_func_select_01, /* {��ʱ����} */
    dm603_face_time_func_select_02, /* {һ�ζ�ʱ} */
    dm603_face_time_func_select_03, /* {01�鶨ʱ} */
    dm603_face_time_func_select_04, /* {02�鶨ʱ} */
    dm603_face_time_func_select_05, /* {03�鶨ʱ} */
    dm603_face_time_func_select_06, /* {04�鶨ʱ} */
    dm603_face_time_func_select_07, /* {05�鶨ʱ} */
    dm603_face_time_func_select_08, /* {06�鶨ʱ} */
};

#endif

U08 dm603_error_bak[MAX_HMI];       /* ���ϱ�ʶ���� */
U08 dm603_fg_one_day[MAX_HMI];      /* һ�����ѱ�־ */

U08 dm603_face_bak[MAX_HMI][MAX_FACE_LAYER];        /* ����ID���� */
U08 dm603_item_bak[MAX_HMI][MAX_FACE_LAYER];        /* ����ѡ��� */
U08 dm603_face_index[MAX_HMI];              /* ���������� */
U08 dm603_face_dir[MAX_HMI];            /* ���淽��(���롢�˳������ڱ���ѡ��͹��) */

static U08* pdisp_buf;
static U16 dm603_power_time[MAX_HMI];       /* �ϵ�ʱ�� */

//static U08 dm603_fg_screen[MAX_HMI];      /* ������־ */

static U08 dm603_hmi_online[MAX_HMI];


//static U08 ModeBlink[MAX_HMI];                /* ģʽ��˸����*/
//static U08 fg_economy_blink_bak[MAX_HMI]; /* ����ģʽ��˸����*/
//static U08 time_index;            /* ��ʱ�������� */

//BOOL fg_machine_need_open = FALSE;  /* ������Ҫ����(ͣ��ʱ�����ͷ���ʱ����ʹ��) ���� */










//U08 fg_test = 0;      /* ���Գ����ʶ */



/* ͼ���ݶ��� */
#define BMP_NU      0
#define BMP_ON      idx_bmp_onoff_16_16
#define BMP_OK      idx_bmp_enter_16_16
//#define   BMP_OK      idx_bmp_ok_16_16
#define BMP_UP      idx_bmp_up_16_16
#define BMP_DN      idx_bmp_down_16_16
//#define   BMP_FN      idx_bmp_mkey_16_16
#define BMP_FN      idx_bmp_fn_16_16

//static U16 key_menu[] = { DM603_KEY_ONOFF, DM603_KEY_FUNC, DM603_KEY_UP, DM603_KEY_DOWN, DM603_KEY_OK };
//static U16 key_icon[] = { BMP_ON, BMP_FN, BMP_UP, BMP_DN, BMP_OK };


/* ����������Ϣ */
/* ����С�� */
#define para_val_00_00      PNULL
#define DNULL               0xffff
const U16  para_val_01_01[] =
{
    msg_para_val_00_00_00,
    msg_para_val_01_01_01,    /* 01 {����ģʽ} */
    msg_para_val_01_01_02,    /* 02 {����ģʽ} */
    msg_para_val_01_01_03,    /* 03 {�Զ�ģʽ} */
};

const U16 Msg_UseOrNot[] =
{
    msg_use_or_not_01,          /* 01 {����} */
    msg_use_or_not_02,          /* 02 {ʹ��} */
};

const U16 Msg_weather_curve[] =
{
    msg_weather_curve_01,
    msg_weather_curve_02,
    msg_weather_curve_03,
    msg_weather_curve_04,
    msg_weather_curve_05,
    msg_weather_curve_06,
    msg_weather_curve_07,
    msg_weather_curve_08,
    msg_weather_curve_09,
    msg_weather_curve_10,
    msg_weather_curve_11,
    msg_weather_curve_12,
    msg_weather_curve_13,
    msg_weather_curve_14,
    msg_weather_curve_15,
    msg_weather_curve_16,
    msg_weather_curve_17,
    msg_weather_curve_18,
};

const U16 Msg_zone_ctrl_type[] =
{
    msg_zone_ctrl_type_01,
    msg_zone_ctrl_type_02,
    msg_zone_ctrl_type_03,
    msg_zone_ctrl_type_04,
};

const U16 Msg_InputNormal[] =
{
    msg_input_normal_01,            /* 01 {����} */
    msg_input_normal_02,            /* 02 {����} */
};


const U16 para_val_01_06[] =
{
    msg_para_val_01_06_01,    /* 01 {һֱ����} */
    msg_para_val_01_06_02,    /* 02 {��Ъ����} */
};

const U16 Msg_CtrlObject[] =
{
    msg_ctrl_object_01, /* 01 {��ˮ} */
    msg_ctrl_object_02, /* 02 {��ˮ} */
};

const U16 Msg_MotorExciType[] =
{
    msg_motor_ExciType_01,  /* 01 {�������} */
    msg_motor_ExciType_02,  /* 02 {��������} */
};
const U16 Msg_MotorExciFre[] =
{
    msg_motor_ExciFre_01,   /* 01 { 31PPS} */
    msg_motor_ExciFre_02,   /* 02 { 62PPS} */
    msg_motor_ExciFre_03,   /* 03 { 83PPS} */
    msg_motor_ExciFre_04,   /* 04 {100PPS} */
    msg_motor_ExciFre_05,   /* 05 {125PPS} */
    msg_motor_ExciFre_06,   /* 06 {166PPS} */
    msg_motor_ExciFre_07,   /* 07 {250PPS} */
};
const U16 Msg_MotorOrder[] =
{
    msg_motor_order_01,   /* 01 { ��������} */
    msg_motor_order_02,   /* 02 { ��������} */
    msg_motor_order_03,   /* 03 { ����ͬʱ} */
};
const U16 para_val_15_00[]=
{
    msg_para_val_15_00_00,//{��ů}
    msg_para_val_15_00_01,//{��ů}
    msg_para_val_15_00_02,//{����}
};

const U16 Msg_MotorUseType[] =      /* ����ʹ������ */
{
    msg_motor_type_00,          // ����
    msg_motor_type_01,          // �������ȶ�
    msg_motor_type_02,          // �������ȶ�
};

const U16 Msg_S_MotorUseType[] = /* ��·��ʹ������ */
{
    msg_s_motor_type_00,            // ����
    msg_s_motor_type_01,            // ���������ȶ�
    msg_s_motor_type_02,            // �¶��������ȶ�
    msg_s_motor_type_03,            // ѹ���������ȶ�
    msg_s_motor_type_04,            // ��·С��
};

const U16 Msg_RefrigeType[] =       /* ��ý���� */
{
    msg_refrige_type_01,    // R22
    msg_refrige_type_02,    // R407C
    msg_refrige_type_03,    // R410A
    msg_refrige_type_04,    // R32
    msg_refrige_type_05,    // R290
};

const U16 Msg_FourwayType[] =       /* ��ͨ������ */
{
    msg_fourway_type_01,            // ���ȿ���
    msg_fourway_type_02,            // ���俪��
};


const U16 Msg_AiSensorType[] =      /* ģ�������������� */
{
    msg_AiSensorType_00,        // ��
    msg_AiSensorType_01,        // 0.5V~4.5V
};

const U16 Msg_FanSpeedType[] =
{
    msg_FanSpeedType_00,
    msg_FanSpeedType_01,
    msg_FanSpeedType_02,
    msg_FanSpeedType_03,
    msg_FanSpeedType_04,
};


const U16 data_format[] =   /* ͨѶ���ݸ�ʽ�ַ��� */
{
    msg_8N1,
    msg_8N2,
    msg_8O1,
    msg_8E1,
};
const U16 data_bps[] =      /* �������ַ��� */
{
    msg_2400,
    msg_4800,
    msg_9600,
    msg_19200,
    msg_38400,
    msg_57600,
};

const U16 Msg_LimitType[] = /* ϵͳά��ģʽ */
{
    msg_LimitType_00,
    msg_LimitType_01,
};

const U16 Msg_RemoteType[] =        /* �߿ؿ������� */
{
    msg_remote_type_01,         // ����
    msg_remote_type_02,         // ����
};
const U16 Msg_OpenType[] =      /*  */
{
    msg_open_type_01,           //
    msg_open_type_02,           //
    msg_open_type_03,           //
    msg_open_type_04,           //
    msg_open_type_05,           //
};
const U16 Msg_power_mode[] =        /*  */
{
    msg_power_mode_01,          /* ��׼ */
    msg_power_mode_02,          /* ǿ�� */
    msg_power_mode_03,          /* ���� */
    msg_power_mode_04,          /* �Զ� */
};

#if defined(M_TCL)
const U16 Msg_mac_model[] =         /* �����ͺ� */
{
    msg_tcl_mac_model_01,// δ����  
    msg_tcl_mac_model_02,// MAC_F1_4KW  
    msg_tcl_mac_model_03,// MAC_F1_6KW  
    msg_tcl_mac_model_04,// MAC_F1_8KW  
    msg_tcl_mac_model_05,// MAC_F1_10KW 
    msg_tcl_mac_model_06,// MAC_F1_12KW 
    msg_tcl_mac_model_07,// MAC_F1_14KW 
    msg_tcl_mac_model_08,// MAC_F1_16KW 
    msg_tcl_mac_model_09,// MAC_F3_10KW 
    msg_tcl_mac_model_10,// MAC_F3_12KW 
    msg_tcl_mac_model_11,// MAC_F3_14KW 
    msg_tcl_mac_model_12,// MAC_F3_16KW 
    msg_tcl_mac_model_13,// MAC_M1_4KW  
    msg_tcl_mac_model_14,// MAC_M1_6KW  
    msg_tcl_mac_model_15,// MAC_M1_8KW  
    msg_tcl_mac_model_16,// MAC_M1_10KW 
    msg_tcl_mac_model_17,// MAC_M1_12KW 
    msg_tcl_mac_model_18,// MAC_M1_14KW 
    msg_tcl_mac_model_19,// MAC_M1_16KW 
    msg_tcl_mac_model_20,// MAC_M3_10KW 
    msg_tcl_mac_model_21,// MAC_M3_12KW 
    msg_tcl_mac_model_22,// MAC_M3_14KW 
    msg_tcl_mac_model_23,// MAC_M3_16KW 
    msg_tcl_mac_model_24,// MAC_F3_8KW  
    msg_tcl_mac_model_25,// M25  
    msg_tcl_mac_model_26,// M26  
    msg_tcl_mac_model_27,// M27  
    msg_tcl_mac_model_28,// M28  
    msg_tcl_mac_model_29,// M29  
    msg_tcl_mac_model_30,// M30  
    msg_tcl_mac_model_31,// M31  
    msg_tcl_mac_model_32,// M32  
};
#elif defined(M_DISEN)
const U16 Msg_mac_model[] =         /* �����ͺ� */
{
    msg_disen_mac_model_01,// δ���� 
    msg_disen_mac_model_02,// DSKDLNR8BpN3HD
    msg_disen_mac_model_03,// DSKDLNR10BpN3H
    msg_disen_mac_model_04,// DSKDLNR12BpN3H
    msg_disen_mac_model_05,// DSKDLNR14BpN3H
    msg_disen_mac_model_06,// DSKDLNR16BpN3H
    msg_disen_mac_model_07,// DSKDLNR28BpN3SH
    msg_disen_mac_model_08,// DSKDLNR35BpN3SH
    msg_disen_mac_model_09,// DSKDLNR10BhN5H 
    msg_disen_mac_model_10,// DSKDLNR16BhN5H 
    msg_disen_mac_model_11,// M11 
    msg_disen_mac_model_12,// M12 
    msg_disen_mac_model_13,// M13 
    msg_disen_mac_model_14,// M14 
    msg_disen_mac_model_15,// M15 
    msg_disen_mac_model_16,// M16 
    msg_disen_mac_model_17,// M17 
    msg_disen_mac_model_18,// M18 
    msg_disen_mac_model_19,// M19 
    msg_disen_mac_model_20,// M20 
    msg_disen_mac_model_21,// M21 
    msg_disen_mac_model_22,// M22 
    msg_disen_mac_model_23,// M23 
    msg_disen_mac_model_24,// M24 
    msg_disen_mac_model_25,// M25 
    msg_disen_mac_model_26,// M26 
    msg_disen_mac_model_27,// M27 
    msg_disen_mac_model_28,// M28 
    msg_disen_mac_model_29,// M29 
    msg_disen_mac_model_30,// M30 
    msg_disen_mac_model_31,// M31 
    msg_disen_mac_model_32,// M32 
};
#elif defined(M_PAIWO)
const U16 Msg_mac_model[] = 
{
    msg_paiwo_mac_model_01,
    msg_paiwo_mac_model_02,
    msg_paiwo_mac_model_03,
    msg_paiwo_mac_model_04,
    msg_paiwo_mac_model_05,
    msg_paiwo_mac_model_06,
};
#elif defined(M_NIUENTAI) || defined(M_DEBUG)
const U16 Msg_mac_model[] = 
{
    msg_NET_mac_model_01,
    msg_NET_mac_model_02,
    msg_NET_mac_model_03,
    msg_NET_mac_model_04,
    msg_NET_mac_model_05,
    msg_NET_mac_model_06,
    msg_NET_mac_model_07,
    msg_NET_mac_model_08,
    msg_NET_mac_model_09,
    msg_NET_mac_model_10,
    msg_NET_mac_model_11,
    msg_NET_mac_model_12,
    msg_NET_mac_model_13,
    msg_NET_mac_model_14,
    msg_NET_mac_model_15,
    msg_NET_mac_model_16,
    msg_NET_mac_model_17,
    msg_NET_mac_model_18,
    msg_NET_mac_model_19,
    msg_NET_mac_model_20,
    msg_NET_mac_model_21,
    msg_NET_mac_model_22,
    msg_NET_mac_model_23,
    msg_NET_mac_model_24,
    msg_NET_mac_model_25,
    msg_NET_mac_model_26,
    msg_NET_mac_model_27,
    msg_NET_mac_model_28,
    msg_NET_mac_model_29,
    msg_NET_mac_model_30,
    msg_NET_mac_model_31,
    msg_NET_mac_model_32,
    msg_NET_mac_model_33,
    msg_NET_mac_model_34,
    msg_NET_mac_model_35,
    msg_NET_mac_model_36,
    msg_NET_mac_model_37,
    msg_NET_mac_model_38,
    msg_NET_mac_model_39,
};
#elif defined(M_HONGHE)
const U16 Msg_mac_model[] = 
{
    msg_honghe_mac_model_01,
    msg_honghe_mac_model_02,
    msg_honghe_mac_model_03,
    msg_honghe_mac_model_04,
    msg_honghe_mac_model_05,
    msg_honghe_mac_model_06,
    msg_honghe_mac_model_07,
    msg_honghe_mac_model_08,
    msg_honghe_mac_model_09,
    msg_honghe_mac_model_10,
};
#elif defined(M_SHENGNENG)
const U16 Msg_mac_model[] = 
{
    msg_shengneng_mac_model_01,
    msg_shengneng_mac_model_02,
    msg_shengneng_mac_model_03,
};
#else
const U16 Msg_mac_model[] =         /* �����ͺ� */
{
    msg_tcl_mac_model_01,                   // δ����
};
#endif

const U16 Msg_ClearFrostType[] =        /*  */
{
    msg_cleardrefrost_type_01,          // ����
    msg_cleardrefrost_type_02,          //
};
/* N0004 */
const U16 Msg_FreqFanSet[] =        /* ��Ƶ������� */
{
    msg_freq_fan_set_01,
    msg_freq_fan_set_02,
    msg_freq_fan_set_03,
    msg_freq_fan_set_04,
    msg_freq_fan_set_05,
    msg_freq_fan_set_06,
};

const U16 Msg_FreqFanCtrl[] =       /* ��Ƶ������� */
{
    msg_freq_fan_ctrl_01,   /* �ֶ� */
    msg_freq_fan_ctrl_02,   /* �������� */
    msg_freq_fan_ctrl_03,   /* �������� */
};

const U16 Msg_remote_switch[] =      /* ������������ */
{
    msg_remote_switch_01,	/* ����  */
    msg_remote_switch_02,	/* ����������Ч */
    msg_remote_switch_03,	/* �����պ���Ч */
    msg_remote_switch_04,	/* �߿ؿ��ػ� */
    msg_remote_switch_05,	/* �߿�ˮ����� */
    msg_remote_switch_06,	/* �߿��ⲿ��Դ */
};

const U16 Msg_sensor_cfg[] =
{
    msg_sensor_cfg_01,
    msg_sensor_cfg_02,
    msg_sensor_cfg_03,
    msg_sensor_cfg_04,
    msg_sensor_cfg_05,
};

const U16 Msg_press_cfg[] =
{
    msg_press_cfg_01,
    msg_press_cfg_02,
    msg_press_cfg_03,
    msg_press_cfg_04,
    msg_press_cfg_05,
};

//const U16 Msg_gb_dbg_use[] =        /* ����������� */
//{
//    msg_gb_dbg_use_01,
//    msg_gb_dbg_use_02,
//    msg_gb_dbg_use_03,
//};

const U16 Msg_ability_test[] =      /* ��������ѡ�� */
{
    msg_ability_test_01,
    msg_ability_test_02,
    msg_ability_test_03,
    msg_ability_test_04,
    msg_ability_test_05,
    msg_ability_test_06,
    msg_ability_test_07,
    msg_ability_test_08,
    msg_ability_test_09,
    msg_ability_test_10,
    msg_ability_test_11,
    msg_ability_test_12,
    msg_ability_test_13,
    msg_ability_test_14,
    msg_ability_test_15,
    msg_ability_test_16,
    msg_ability_test_17,
    msg_ability_test_18,
    msg_ability_test_19,
    msg_ability_test_20,
    msg_ability_test_21,
    msg_ability_test_22,
    msg_ability_test_23,
    msg_ability_test_24,
    msg_ability_test_25,
    msg_ability_test_26,
    msg_ability_test_27,
    msg_ability_test_28,
    msg_ability_test_29,
    msg_ability_test_30,
    msg_ability_test_31,
    msg_ability_test_32,
    msg_ability_test_33,
    msg_ability_test_34,
    msg_ability_test_35,
    msg_ability_test_36,
    msg_ability_test_37,
    msg_ability_test_38,
    msg_ability_test_39,
    msg_ability_test_40,
    msg_ability_test_41,
    msg_ability_test_42,
    msg_ability_test_43,
    msg_ability_test_44,
    msg_ability_test_45,
    msg_ability_test_46,
    msg_ability_test_47,
    msg_ability_test_48,
    msg_ability_test_49,
    msg_ability_test_50,
    msg_ability_test_51,
    msg_ability_test_52,
    msg_ability_test_53,
    msg_ability_test_54,
    msg_ability_test_55,
    msg_ability_test_56,
    msg_ability_test_57,
    msg_ability_test_58,
    msg_ability_test_59,
    msg_ability_test_60,
    msg_ability_test_61,
    msg_ability_test_62,
    msg_ability_test_63,
    msg_ability_test_64,
    msg_ability_test_65,
    msg_ability_test_66,
    msg_ability_test_67,
    msg_ability_test_68,
    msg_ability_test_69,
    msg_ability_test_70,
    msg_ability_test_71,
    msg_ability_test_72,
    msg_ability_test_73,
    msg_ability_test_74,
};

const U16 Msg_pfc_set[] =
{
    msg_pfc_set1,// ����pfc
    msg_pfc_set2,// �ر�pfc
    msg_pfc_set3,// �Զ�
};

const U16 Msg_df_stop_cm[] =        /* ��˪ͣ���з� */
{
    msg_df_stop_cm_01,      /* ����     */
    msg_df_stop_cm_02,      /* ����ȫͣ */
    msg_df_stop_cm_03,      /* ����˪ͣ */
    msg_df_stop_cm_04,      /* �˳�˪ͣ */
};

const U16 Msg_SilentMode[] =
{
    dm603_para_silent_mode_01,  /* {һ������}     */
    dm603_para_silent_mode_02,  /* {��������} */
};

const U16 Msg_ice_heat_mode[] =
{
    Msg_ice_heat_mode_01,   /* 01 {��˪������} */
    Msg_ice_heat_mode_02,   /* 02 {�ͻ�������} */
};

const U16 Msg_fcu_s_start_m[] =
{
    msg_s_start_m_01, /* ������ */
    msg_s_start_m_02, /* �������� */
    msg_s_start_m_03, /* ��ĩȫ�� */
};

const U16 Msg_fcu_mode_set[] =
{
    msg_fcu_mode_set_01,  /* �໥���� */
    msg_fcu_mode_set_02,  /* ����Ϊ�� */
    msg_fcu_mode_set_03,  /* ĩ��Ϊ�� */
    msg_fcu_mode_set_04,  /* �໥���� */
};

const U16 Msg_fcu_cpst_set[] =
{
    msg_fcu_cpst_set_01,  /* ������ǰ�¶� */
    msg_fcu_cpst_set_02,  /* ����Ŀ���¶� */
};

const U16 para_alarm_sound[] =
{
    msg_para_alarm_sound_set_01,    /* 01 {����} */
    msg_para_alarm_sound_set_02,    /* 02 {����} */
    msg_para_alarm_sound_set_03,    /* 03 {ֻ��ʮ��} */
    msg_para_alarm_sound_set_04,    /* 04 {����ʮ��} */
};

const U16 Msg_room_ther[] =
{
    msg_room_ther_Type_00,      // ����
    msg_room_ther_Type_01,      // ģʽ�趨
    msg_room_ther_Type_02,  // �����ػ�
    msg_room_ther_Type_03,  // ˫���¿�
};

const U16 Msg_PUMPf_ctrl_type[] =
{
    msg_PUMPf_ctrl_type_01,
    msg_PUMPf_ctrl_type_02,
    msg_PUMPf_ctrl_type_03,
    msg_PUMPf_ctrl_type_04,
    msg_PUMPf_ctrl_type_05,
};

const U16 Msg_termial_device_type[] =
{
    msg_termial_device_type_01,
    msg_termial_device_type_02,
    msg_termial_device_type_03,
};

const U16 Msg_ibh_ahs_use[] =
{
    msg_ibh_ahs_use_01,
    msg_ibh_ahs_use_02,
    msg_ibh_ahs_use_03,
    msg_ibh_ahs_use_04,
};

const U16 Msg_Tp_fan_corr_set[] =
{
    msg_Tp_fan_corr_set_01,
    msg_Tp_fan_corr_set_02,
    msg_Tp_fan_corr_set_03,
    msg_Tp_fan_corr_set_04,
};

const U16 Msg_sg_cfg[] =
{
    msg_sg_cfg_01,
    msg_sg_cfg_02,
};

const U16 Msg_invt_err_auto_rst[] = 
{
	msg_invt_err_auto_rst_01,	/* 01 ���� */
	msg_invt_err_auto_rst_02,	/* 02 ȫ�� */
	msg_invt_err_auto_rst_03,	/* 03 ȫ�� */
};

/* �ֵ���Ϣ,��ʼ */
#define _para_dict_item(para_val_xx_xx)     para_val_xx_xx, ARRELE(para_val_xx_xx)
const PARADICT para_dict[] =
{
    {_para_dict_item(para_val_01_01     )},
    {_para_dict_item(Msg_UseOrNot       )},
    {_para_dict_item(Msg_InputNormal    )},
    {_para_dict_item(para_val_01_06     )},
    {_para_dict_item(Msg_CtrlObject     )},
    {_para_dict_item(Msg_MotorExciType  )},
    {_para_dict_item(Msg_MotorExciFre   )},
    {_para_dict_item(para_val_15_00     )},
    {_para_dict_item(Msg_MotorUseType   )},
    {_para_dict_item(Msg_S_MotorUseType )},
    {_para_dict_item(Msg_RefrigeType    )},
    {_para_dict_item(Msg_FourwayType    )},
    {_para_dict_item(Msg_AiSensorType   )},
    {_para_dict_item(Msg_FanSpeedType   )},
    {_para_dict_item(data_format        )},
    {_para_dict_item(data_bps           )},
    {_para_dict_item(Msg_LimitType      )},
    {_para_dict_item(Msg_RemoteType     )},
    {_para_dict_item(Msg_OpenType       )},
    {_para_dict_item(Msg_power_mode     )},
    {_para_dict_item(Msg_mac_model      )},
    {_para_dict_item(Msg_ClearFrostType )},
    {_para_dict_item(Msg_FreqFanSet     )},
    {_para_dict_item(Msg_FreqFanCtrl    )},
    {_para_dict_item(Msg_remote_switch  )},
    {_para_dict_item(Msg_ability_test)},
    {_para_dict_item(Msg_pfc_set)},
    {_para_dict_item(Msg_df_stop_cm )},
    {_para_dict_item(Msg_SilentMode )},
    {_para_dict_item(Msg_ice_heat_mode)},
    {_para_dict_item(Msg_fcu_s_start_m)},
    {_para_dict_item(Msg_fcu_mode_set)},
    {_para_dict_item(Msg_fcu_cpst_set)},
    {_para_dict_item(para_alarm_sound   )},
    {_para_dict_item(Msg_room_ther)},
    {_para_dict_item(Msg_weather_curve)},
    {_para_dict_item(Msg_zone_ctrl_type)},
    {_para_dict_item(Msg_sensor_cfg)},
    {_para_dict_item(Msg_press_cfg)},
    {_para_dict_item(Msg_termial_device_type)},
    {_para_dict_item(Msg_ibh_ahs_use)},
    {_para_dict_item(Msg_PUMPf_ctrl_type)},
    {_para_dict_item(Msg_Tp_fan_corr_set)},
    {_para_dict_item(Msg_sg_cfg)},
    {_para_dict_item(Msg_invt_err_auto_rst)},
    {_para_dict_item(Msg_MotorOrder)},
    /* �Ӻ���׷�� */
};
#define PARA_DICT_NUM   ARRELE(para_dict)
U16 get_dict_num(void) {return PARA_DICT_NUM;}
/* �ֵ���Ϣ,���� */

const SMALL para_small_01[] =
{
    {msg_para_small_01_01,para_val_01_01},/* 00 ����ģʽ */
    {msg_para_small_01_02,para_val_00_00},/* 01 �����趨�¶� */
    {msg_para_small_01_03,para_val_00_00},/* 02 �����趨�¶� */
    {msg_para_small_01_04,para_val_00_00},/* 03 ��ˮ�趨�¶� */
    {msg_para_small_01_05,para_val_00_00},/* 26 �����趨���� */
    {msg_para_small_01_06,para_val_00_00},/* 04 �����趨���� */
    {msg_para_small_01_07,para_val_00_00},/* 23 �����趨�¶�(B) */
    {msg_para_small_01_08,para_val_00_00},/* 44 �����趨����(B) */
    {msg_para_small_01_09,para_val_00_00},/* 24 �����趨�¶�(B) */
    {msg_para_small_01_10,para_val_00_00},/* 25 �����趨����(B) */
    {msg_para_small_01_11,Msg_power_mode},// 05 ����ģʽ
    {msg_para_small_01_12,Msg_SilentMode},// 06 ����ģʽ
    {msg_para_small_01_13,Msg_UseOrNot  },// 07 ��ʱɱ������
    {msg_para_small_01_14,para_val_00_00},// 08 ɱ���¶�
    {msg_para_small_01_15,para_val_00_00},// 09 ɱ���������
    {msg_para_small_01_16,para_val_00_00},// 10 ɱ������ʱ��
    {msg_para_small_01_17,para_val_00_00},// 11 ����
    {msg_para_small_01_18,para_val_00_00},// 12 ����
    {msg_para_small_01_19,para_val_00_00},// 13 ����
    {msg_para_small_01_20,para_val_00_00},// 14 ����
    {msg_para_small_01_21,para_val_00_00},// 15 ����
    {msg_para_small_01_22,para_val_00_00},// 16 ����
    {msg_para_small_01_23,para_val_00_00},// 17 ����
    {msg_para_small_01_24,Msg_UseOrNot  },// 18 ������������
    {msg_para_small_01_25,para_alarm_sound},// 19 ����������
    {msg_para_small_01_26,Msg_weather_curve}, /* 21 A���������� */
    {msg_para_small_01_27,Msg_weather_curve}, /* 22 A���������� */
    {msg_para_small_01_28,Msg_weather_curve}, /* 27 B���������� */
    {msg_para_small_01_29,Msg_weather_curve}, /* 28 B���������� */
    {msg_para_small_01_30,para_val_00_00},  /* 27 ����9���价��1 */
    {msg_para_small_01_31,para_val_00_00},  /* 28 ����9���价��2 */
    {msg_para_small_01_32,para_val_00_00},  /* 29 ����9�������1 */
    {msg_para_small_01_33,para_val_00_00},  /* 30 ����9�������2 */
    {msg_para_small_01_34,para_val_00_00},  /* 31 ����9���Ȼ���1 */
    {msg_para_small_01_35,para_val_00_00},  /* 32 ����9���Ȼ���2 */
    {msg_para_small_01_36,para_val_00_00},  /* 33 ����9���ȳ���1 */
    {msg_para_small_01_37,para_val_00_00},  /* 34 ����9���ȳ���2 */
    {msg_para_small_01_38,Msg_UseOrNot, },  /*37 00#ģ������ */
    {msg_para_small_01_39,Msg_UseOrNot, },  /*38 01#ģ������ */
    {msg_para_small_01_40,Msg_UseOrNot, },  /*39 02#ģ������ */
    {msg_para_small_01_41,Msg_UseOrNot, },  /*40 03#ģ������ */
    {msg_para_small_01_42,Msg_UseOrNot, },  /*41 04#ģ������ */
    {msg_para_small_01_43,Msg_UseOrNot, },  /*42 05#ģ������ */
    {msg_para_small_01_44,Msg_UseOrNot, },  /*43 06#ģ������ */
    {msg_para_small_01_45,Msg_UseOrNot, },  /*44 07#ģ������ */
};

const SMALL para_small_02[] =
{
    {msg_para_small_02_01,para_val_15_00},      // 00 ��������
    {msg_para_small_02_02,Msg_RefrigeType},     // 01 ��ý���� 0=R22   1=R407C   2=R410
    {msg_para_small_02_03,Msg_FourwayType},     // 02 ��ͨ������
    {msg_para_small_02_04,Msg_mac_model},       // 03 �����ͺ� para_fac
    {msg_para_small_02_05,Msg_RemoteType},      // 04 �߿ؿ������� 0=��������  1=���忪
    {msg_para_small_02_06,Msg_OpenType},        // 05 ������ͣ���� ��μ�������ͣ����
    {msg_para_small_02_07,Msg_UseOrNot},        // 06 ������俪�ػ�   0=����  1=ʹ��
    {msg_para_small_02_08,Msg_UseOrNot},        // 07 ����������   0=����  1=ʹ��
    {msg_para_small_02_09,para_val_01_06},      // 08 ˮ������ģʽ
    {msg_para_small_02_10,Msg_CtrlObject},      // 09 ���ƶ��� 0����ˮ  1����ˮ
    {msg_para_small_02_11,para_val_00_00},      // 10 ����ˮ�²���
    {msg_para_small_02_12,para_val_00_00},      // 11 ����ˮ�²���
    {msg_para_small_02_13,para_val_00_00},      // 12 ��ͨ������ʱ
    {msg_para_small_02_14,para_val_00_00},      // 13 ��ͨ������ʱ
    {msg_para_small_02_15,para_val_00_00},      // 14 ����Һ������
    {msg_para_small_02_16,para_val_00_00},      // 15 ����Һ�����²�
    {msg_para_small_02_17,Msg_UseOrNot  },      // 16 ��ˮ����
    {msg_para_small_02_18,para_val_00_00},      // 17 ��ˮ�ز�
    {msg_para_small_02_19,para_val_00_00},      // 18 �����ỷ��
    {msg_para_small_02_20,para_val_00_00},      // 19 �����ỷ��
    {msg_para_small_02_21,Msg_UseOrNot},        // 20 ����ģʽ
    {msg_para_small_02_22,Msg_UseOrNot},        // 21 ����ģʽ
    {msg_para_small_02_23,para_val_00_00},      // 22 ��СԤ�ȵ���(ʱ)
    {msg_para_small_02_24,para_val_00_00},      // 23 ���Ԥ�ȵ���(ʱ)
    {msg_para_small_02_25,para_val_00_00},      // 24 ��СԤ��ʱ��(��)
    {msg_para_small_02_26,para_val_00_00},      // 25 ���Ԥ��ʱ��(��)
    {msg_para_small_02_27,Msg_UseOrNot},        // 26 ��ˮģʽ����
    {msg_para_small_02_28,Msg_UseOrNot},        // 27 ˮ�����
    {msg_para_small_02_29,Msg_UseOrNot},        // 28 �»�ˮ��
    {msg_para_small_02_30,Msg_UseOrNot},        // 29 ̫����
    {msg_para_small_02_31,Msg_remote_switch},   // 30 ������������
    {msg_para_small_02_32,Msg_UseOrNot },       // 31 ��Ƶ��ʹ������   0=����  1=ʹ��
    {msg_para_small_02_33,Msg_UseOrNot },       /* 32 ϵͳ��Ƶˮ�� */
    {msg_para_small_02_34,Msg_room_ther},       // 33 �¿�������
    {msg_para_small_02_35,Msg_zone_ctrl_type},  /* 34 �߿������� */
    {msg_para_small_02_36,para_val_00_00},      // 35 ������������
    {msg_para_small_02_37,para_val_00_00},      // 36 ��������ʱ��
    {msg_para_small_02_38,Msg_ice_heat_mode},   /* 37 ���̵���ģʽ */
    {msg_para_small_02_39,para_val_00_00},      /* 38 ���̵��Ȼ��� */
    {msg_para_small_02_40,para_val_00_00},      /* 39 ����ҳ���� */
    {msg_para_small_02_41,Msg_sg_cfg},          /* 40 ���ܵ������� */
    {msg_para_small_02_42,Msg_UseOrNot  },      /* 41 ���ܵ������� */
    {msg_para_small_02_43,para_val_00_00},      /* 42 ���ȵ���ϵ�� */
    {msg_para_small_02_44,para_val_00_00},      /* 43 ˮ�ü�Ъͣʱ�� */
};

const SMALL para_small_03[] =
{
    {msg_para_small_03_01,para_val_00_00},    // 0 ��Ƶѹ������
    {msg_para_small_03_02,Msg_UseOrNot},      // 1 �ֶ�ģʽ
    {msg_para_small_03_03,para_val_00_00},    // 2 �ֶ��趨ת��
    {msg_para_small_03_04,para_val_00_00},    // 3 ��Ƶ������
    {msg_para_small_03_05,para_val_00_00},    // 4 ����Ԥ��Ƶ
    {msg_para_small_03_06,para_val_00_00},    // 5 ����ת������
    {msg_para_small_03_07,para_val_00_00},    // 6 ����ת������
    {msg_para_small_03_08,para_val_00_00},    // 7 ����ת��
    {msg_para_small_03_09,para_val_00_00},    // 8 ���ȶת��
    {msg_para_small_03_10,para_val_00_00},    // 9 ����ά��ʱ��
    {msg_para_small_03_11,para_val_00_00},      /* 36 ��ѹ�޼�ƫ�� */
    {msg_para_small_03_12,para_val_00_00},      /* 37 ��ѹ�޼�ƫ�� */
    {msg_para_small_03_13,para_val_00_00},      /* 38 ��ѹǿжƫ�� */
    {msg_para_small_03_14,para_val_00_00},      /* 39 ��ѹǿжƫ�� */
    {msg_para_small_03_15,para_val_00_00},    //14 ��Ƶ�����ʱ��
    {msg_para_small_03_16,para_val_00_00},    //15 ǿ�ƻ���ʱ��
    {msg_para_small_03_17,para_val_00_00},    //16 ��Ƶ������ַ
    {msg_para_small_03_18,para_val_00_00},    //17 ѹ����˪ת��
    {msg_para_small_03_19,para_val_00_00},    //18 ѹ������ת��
    {msg_para_small_03_20,para_val_00_00},    //19 ������Ƶ����
    {msg_para_small_03_21,para_val_00_00},    //20 ������Ƶ����
    {msg_para_small_03_22,para_val_00_00},    //21 ��˪��Ƶ����
    {msg_para_small_03_23,para_val_00_00},    //22 ��׼Ƶ�ʵ���
    {msg_para_small_03_24,para_val_00_00},    //23 ������Ƶ���
    {msg_para_small_03_25,para_val_00_00},    //24 ��Ƶ������
    {msg_para_small_03_26,para_val_00_00},    //25 ��Ƶ����ʱ��
    {msg_para_small_03_27,para_val_00_00},    //26 ��Ƶ΢��ʱ��
    {msg_para_small_03_28,para_val_00_00},    //27 ��Ƶ��������
    {msg_para_small_03_29,para_val_00_00},    //28 ѹ������ά��ת��
    {msg_para_small_03_30,para_val_00_00},    //29 ��Ƶ����ϵ��
    {msg_para_small_03_31,para_val_00_00},    //30 ������������
    {msg_para_small_03_32,para_val_00_00},    //31 ������������
    {msg_para_small_03_33,para_val_00_00},    //32 ����Ƶ�ʵ���
    {msg_para_small_03_34,para_val_00_00},    //33 ������������
    {msg_para_small_03_35,para_val_00_00},    //34 �ά��ʱ��
    {msg_para_small_03_36,para_val_00_00},    //35 ������ͻ���
    {msg_para_small_03_37,para_val_00_00},    //36 ���Ȼ��ͻ���
    {msg_para_small_03_38,para_val_00_00},    //37 ǿ�ƽ�Ƶ����
    {msg_para_small_03_39,para_val_00_00},    //38 ��������Ƶ��
    {msg_para_small_03_40,para_val_00_00},    //39 ���������ת��
    {msg_para_small_03_41,para_val_00_00},    //40 ǿ��Ƶ�ʵ���
    {msg_para_small_03_42,para_val_00_00},    //41 �������ת��
    {msg_para_small_03_43,para_val_00_00},    //42 �ر�Ƶ�ʷ�Χ
    {msg_para_small_03_44,para_val_00_00},    //43 �ر�Ƶ��1
    {msg_para_small_03_45,para_val_00_00},    //44 �ر�Ƶ��2
    {msg_para_small_03_46,para_val_00_00},    //45 �ر�Ƶ��3
    {msg_para_small_03_47,para_val_00_00},    //46 �ر�Ƶ��4
    {msg_para_small_03_48,para_val_00_00},    //47 �ͻ�����ά��ת��
};
const SMALL para_small_04[] =
{
    {msg_para_small_04_01,Msg_FreqFanSet},       // ��Ƶ�������
    {msg_para_small_04_02,para_val_00_00},       // EC�������    /* N0004 */
    {msg_para_small_04_03,para_val_00_00},       // ���������
    {msg_para_small_04_04,para_val_00_00},       // ���������
    {msg_para_small_04_05,para_val_00_00},       // ���ܷ��ٵ���
    {msg_para_small_04_06,Msg_FreqFanCtrl},      // ��Ƶ�������
    {msg_para_small_04_07,para_val_00_00},       // �ֶ�ת��
    {msg_para_small_04_08,para_val_00_00},       // �߷�ģ���¶�
    {msg_para_small_04_09,para_val_00_00},       // ����߷绷��
    {msg_para_small_04_10,para_val_00_00},       // ����ͷ绷��
    {msg_para_small_04_11,para_val_00_00},       // ���ȸ߷绷��
    {msg_para_small_04_12,para_val_00_00},       // ���ȵͷ绷��
    {msg_para_small_04_13,para_val_00_00},       // ����߷�����
    {msg_para_small_04_14,para_val_00_00},       // ����ͷ�����
    {msg_para_small_04_15,para_val_00_00},       // ���ȸ߷�����
    {msg_para_small_04_16,para_val_00_00},       // ���ȵͷ�����
    {msg_para_small_04_17,para_val_00_00},       // �����з��¶�
    {msg_para_small_04_18,para_val_00_00},       // ���ٵ�������
    {msg_para_small_04_19,para_val_00_00},       // ���ٶ�������
    {msg_para_small_04_20,para_val_00_00},       // ���ȷ�����
    {msg_para_small_04_21,para_val_00_00},       // ���ȷ�����
    {msg_para_small_04_22,para_val_00_00},       // PWM�������
    {msg_para_small_04_23,para_val_00_00},       // ǿ�����ٵ���
    {msg_para_small_04_24,para_val_00_00},       // EC�����ַ
    {msg_para_small_04_25,para_val_00_00},       // �����з��¶�
    {msg_para_small_04_26,para_val_00_00},       // ������������
    {msg_para_small_04_27,para_val_00_00},       // ��Ƶ�������
    {msg_para_small_04_28,para_val_00_00},       // ����������
    {msg_para_small_04_29,para_val_00_00},       // pwm���ٳ�ʼֵ
    {msg_para_small_04_30,para_val_00_00},       // pwm����ϵ��
    {msg_para_small_04_31,para_val_00_00},       // pwm���ټ��
    {msg_para_small_04_32,para_val_00_00},       // pwm��������
    {msg_para_small_04_33,para_val_00_00},       // ��׼���ٵ���
    {msg_para_small_04_34,Msg_UseOrNot},       // ����繦��
    {msg_para_small_04_35,para_val_00_00},       // �ͷ����ͣ
    {msg_para_small_04_36,para_val_00_00},       // �����ʱ��
    {msg_para_small_04_37,para_val_00_00},       // Ƶ�ʲ���ϵ��
    {msg_para_small_04_38,para_val_00_00},       // ����ϵ������
    {msg_para_small_04_39,para_val_00_00},      /* 39 ���·�������1 */
    {msg_para_small_04_40,para_val_00_00},      /* 40 ���·�������2 */
    {msg_para_small_04_41,para_val_00_00},      /* 41 ���·�������3 */
    {msg_para_small_04_42,Msg_Tp_fan_corr_set}, /* 42 ������������ */
    {msg_para_small_04_43,para_val_00_00},       // ����ͣ���²�
    {msg_para_small_04_44,para_val_00_00},       // ����ͣ���²�
    {msg_para_small_04_45,para_val_00_00},       // ͣ��ز�
    {msg_para_small_04_46,para_val_00_00},       // ����������
    {msg_para_small_04_47,para_val_00_00},       // ���ȷ������
};
const SMALL para_small_05[] =
{
    {msg_para_small_05_01,Msg_MotorUseType},  // 1 ����ʹ������
    {msg_para_small_05_02,Msg_MotorExciType}, // 2 �������ŷ�ʽ
    {msg_para_small_05_03,Msg_MotorExciFre},  // 3 ��������Ƶ��
    {msg_para_small_05_04,para_val_00_00},    // 4 �����ܲ���
    {msg_para_small_05_05,para_val_00_00},    // 5 ��ǿ��������
    {msg_para_small_05_06,para_val_00_00},    // 6 ������󿪶�
    {msg_para_small_05_07,para_val_00_00},    // 7 �����������²�
    {msg_para_small_05_08,Msg_UseOrNot},      // 8 �����ֶ�ģʽ
    {msg_para_small_05_09,para_val_00_00},    // 9 �����ֶ�����
    {msg_para_small_05_10,para_val_00_00},    //10 ��������Ƶ�ʲ�
    {msg_para_small_05_11,Msg_S_MotorUseType},//11 ����ʹ������
    {msg_para_small_05_12,Msg_MotorExciType}, //12 �������ŷ�ʽ
    {msg_para_small_05_13,Msg_MotorExciFre},  //13 ��������Ƶ��
    {msg_para_small_05_14,para_val_00_00},    //14 �����ܲ���
    {msg_para_small_05_15,para_val_00_00},    //15 ��ǿ��������
    {msg_para_small_05_16,para_val_00_00},    //16 ������󿪶�
    {msg_para_small_05_17,para_val_00_00},    //17 ������С����
    {msg_para_small_05_18,Msg_UseOrNot},      //18 �����ֶ�ģʽ
    {msg_para_small_05_19,para_val_00_00},    //19 �����ֶ�����
    {msg_para_small_05_20,para_val_00_00},    //20 �ͻ���ˮ����
    {msg_para_small_05_21,para_val_00_00},    //21 �ͻ���ˮ����
    {msg_para_small_05_22,para_val_00_00},    //22 �ͻ���ˮ����
    {msg_para_small_05_23,para_val_00_00},    //23 �����ο�����
    {msg_para_small_05_24,para_val_00_00},    //24 �ο�����ϵ��
    {msg_para_small_05_25,para_val_00_00},    //25 �������ת����
    {msg_para_small_05_26,para_val_00_00},    //26 �ͻ��������1
    {msg_para_small_05_27,para_val_00_00},    //27 �ͻ��������2
    {msg_para_small_05_28,para_val_00_00},    //28 �ͻ��������3
    {msg_para_small_05_29,para_val_00_00},    //29 ������Ƶ����
    {msg_para_small_05_30,para_val_00_00},    //30 ������Ƶϵ��
    {msg_para_small_05_31,para_val_00_00},    //31 ������Ƶϵ��
    {msg_para_small_05_32,para_val_00_00},    //32 ����������ƻ�
    {msg_para_small_05_33,para_val_00_00},    //33 ������������10
    {msg_para_small_05_34,Msg_MotorOrder},    //34 ��������������
};
const SMALL para_small_06[] =
{
    {msg_para_small_06_01,para_val_00_00},// 1 ��������
    {msg_para_small_06_02,para_val_00_00},// 2 ����ά��ʱ��
    {msg_para_small_06_03,para_val_00_00},// 3 ��������Ŵ�
    {msg_para_small_06_04,para_val_00_00},// 4 ���ȳ����Ŵ�1
    {msg_para_small_06_05,para_val_00_00},// 5 ���ȳ����Ŵ�2
    {msg_para_small_06_06,para_val_00_00},// 6 ���ȳ����Ŵ�3
    {msg_para_small_06_07,para_val_00_00},// 7 ���ȳ���ת��1
    {msg_para_small_06_08,para_val_00_00},// 8 ���ȳ���ת��2
    {msg_para_small_06_09,para_val_00_00},// 9 �����������
    {msg_para_small_06_10,para_val_00_00},//10 �����������
    {msg_para_small_06_11,para_val_00_00},//11 ���ȳ�������
    {msg_para_small_06_12,para_val_00_00},//12 ���ȳ�������
    {msg_para_small_06_13,para_val_00_00},//13 �ظ������Ŵ�
    {msg_para_small_06_14,para_val_00_00},//14 ������������1
    {msg_para_small_06_15,para_val_00_00},//15 ������������2
    {msg_para_small_06_16,para_val_00_00},//16 ������������3
    {msg_para_small_06_17,para_val_00_00},//17 �������ת��1
    {msg_para_small_06_18,para_val_00_00},//18 �������ת��2
    {msg_para_small_06_19,para_val_00_00},//19 ������������1
    {msg_para_small_06_20,para_val_00_00},//20 ������������2
    {msg_para_small_06_21,para_val_00_00},//21 ������������3
    {msg_para_small_06_22,para_val_00_00},//22 ������������4
    {msg_para_small_06_23,para_val_00_00},//23 ������������5
    {msg_para_small_06_24,para_val_00_00},//24 ������������6
    {msg_para_small_06_25,para_val_00_00},//25 ������������7
    {msg_para_small_06_26,para_val_00_00},//26 ������������8
    {msg_para_small_06_27,para_val_00_00},//27 ������������9
    {msg_para_small_06_28,para_val_00_00},//28 ǿ��С��������
    {msg_para_small_06_29,para_val_00_00},//29 ��˪����
    {msg_para_small_06_30,para_val_00_00},//30 ���������޷�
    {msg_para_small_06_31,para_val_00_00},//31 ������
    {msg_para_small_06_32,para_val_00_00},//32 ����ʱ��
    {msg_para_small_06_33,para_val_00_00},//33 ΢��ʱ��
    {msg_para_small_06_34,para_val_00_00},//34 pid��������
    {msg_para_small_06_35,para_val_00_00},//35 pid��������
    {msg_para_small_06_36,para_val_00_00},//36 ����ϵ��
    {msg_para_small_06_37,para_val_00_00},//37 ������С����1
    {msg_para_small_06_38,para_val_00_00},//38 ������С����2
    {msg_para_small_06_39,para_val_00_00},//39 ������С����3
    {msg_para_small_06_40,para_val_00_00},//40 ������С����4
    {msg_para_small_06_41,para_val_00_00},//41 ������С����5
    {msg_para_small_06_42,para_val_00_00},//42 ������С����1
    {msg_para_small_06_43,para_val_00_00},//43 ������С����2
    {msg_para_small_06_44,para_val_00_00},//44 ������С����3
    {msg_para_small_06_45,para_val_00_00},//45 ������С����4
    {msg_para_small_06_46,para_val_00_00},//46 ������С����5
    {msg_para_small_06_47,para_val_00_00},//47 �����޹س���
    {msg_para_small_06_48,para_val_00_00},//48 ��˪��������
};
const SMALL para_small_07[] =
{
    {msg_para_small_07_01,Msg_UseOrNot},  // 1 ���������ȿ���
    {msg_para_small_07_02,Msg_UseOrNot},  // 2 ���������俪��
    {msg_para_small_07_03,para_val_00_00},// 3 ����������ʱ
    {msg_para_small_07_04,para_val_00_00},// 4 ���ȿ�������
    {msg_para_small_07_05,para_val_00_00},// 5 ������������
    {msg_para_small_07_06,para_val_00_00},// 6 ���俪������
    {msg_para_small_07_07,para_val_00_00},// 7 �������ȶ�ƫ��
    {msg_para_small_07_08,para_val_00_00},// 8 �����ʼ����
    {msg_para_small_07_09,para_val_00_00},// 9 ���ȳ�ʼ���� 1
    {msg_para_small_07_10,para_val_00_00},//10 ���ȳ�ʼ���� 2
    {msg_para_small_07_11,para_val_00_00},//11 ���ȳ�ʼ���� 3
    {msg_para_small_07_12,para_val_00_00},//12 ���ȳ�ʼ���� 4
    {msg_para_small_07_13,para_val_00_00},//13 ���俪���Ź�
    {msg_para_small_07_14,para_val_00_00},//14 ���ȿ����Ź�
    {msg_para_small_07_15,para_val_00_00},//15 ����
    {msg_para_small_07_16,para_val_00_00},//16 ������
    {msg_para_small_07_17,para_val_00_00},//17 ����ʱ��
    {msg_para_small_07_18,para_val_00_00},//18 ΢��ʱ��
    {msg_para_small_07_19,para_val_00_00},//19 pid��������
    {msg_para_small_07_20,para_val_00_00},//20 pid��������
    {msg_para_small_07_21,para_val_00_00},//21 ����ϵ��
    {msg_para_small_07_22,para_val_00_00},//22 ����ϵ��
    {msg_para_small_07_23,para_val_00_00},//23 ���ȶ�ת��1
    {msg_para_small_07_24,para_val_00_00},//24 ���ȶ�ת��2
    {msg_para_small_07_25,para_val_00_00},//25 ���ȶ��趨1
    {msg_para_small_07_26,para_val_00_00},//26 ���ȶ��趨2
    {msg_para_small_07_27,para_val_00_00},//27 ���Ŀ������
    {msg_para_small_07_28,para_val_00_00},//28 ������������
    {msg_para_small_07_29,para_val_00_00},//29 �Ź�ˮ��ϵ��
    {msg_para_small_07_30,para_val_00_00},//30 ������������1
    {msg_para_small_07_31,para_val_00_00},//31 ������������2
    {msg_para_small_07_32,para_val_00_00},//32 ������������3
    {msg_para_small_07_33,para_val_00_00},//33 ������������4
    {msg_para_small_07_34,para_val_00_00},//34 ������������5
    {msg_para_small_07_35,para_val_00_00},//35 ��������ת��
    {msg_para_small_07_36,para_val_00_00},//36 �����ر�ת��
    {msg_para_small_07_37,para_val_00_00},//37 �������Ź�ƫ��
    {msg_para_small_07_38,para_val_00_00},//38 ����������ƫ��
    {msg_para_small_07_39,Msg_UseOrNot},  //39 ��������Һ��
    {msg_para_small_07_40,para_val_00_00},//40 ���ȸ�������A 
    {msg_para_small_07_41,para_val_00_00},//41 ���ȸ�������B
    {msg_para_small_07_42,para_val_00_00},//42 ���ȸ�������E
    {msg_para_small_07_43,para_val_00_00},//43 ���ȸ�������G
    {msg_para_small_07_44,para_val_00_00},//44 ���ȸ�������C
    {msg_para_small_07_45,para_val_00_00},//45 ���ȸ�������D
    {msg_para_small_07_46,para_val_00_00},//46 ���丨������
};
const SMALL para_small_08[] =
{
    {msg_para_small_08_01,para_val_00_00},    // 1 �״γ�˪���
    {msg_para_small_08_02,para_val_00_00},    // 2 ��˪ʱ��
    {msg_para_small_08_03,Msg_ClearFrostType},// 3 ��˪��ʱѡ��
    {msg_para_small_08_04,para_val_00_00},    // 4 �����˪����
    {msg_para_small_08_05,para_val_00_00},    // 5 �����˪����
    {msg_para_small_08_06,para_val_00_00},    // 6 �˳�˪����
    {msg_para_small_08_07,para_val_00_00},    // 7 �˳�˪��ˮ�¶�
    {msg_para_small_08_08,Msg_UseOrNot},      // 8 �������ܳ�˪
    {msg_para_small_08_09,para_val_00_00},    // 9 �жȽ�˪�²�
    {msg_para_small_08_10,para_val_00_00},    //10 �ضȽ�˪�²�
    {msg_para_small_08_11,para_val_00_00},    //11 �¶�ȷ��ʱ��
    {msg_para_small_08_12,para_val_00_00},    //12 ��˪����1
    {msg_para_small_08_13,para_val_00_00},    //13 ��˪����2
    {msg_para_small_08_14,para_val_00_00},    //14 ��˪����3
    {msg_para_small_08_15,para_val_00_00},    //15 ��˪����4
    {msg_para_small_08_16,para_val_00_00},    //16 ��˪���1
    {msg_para_small_08_17,para_val_00_00},    //17 ��˪���2
    {msg_para_small_08_18,para_val_00_00},    //18 ��˪���3
    {msg_para_small_08_19,para_val_00_00},    //19 ��˪���4
    {msg_para_small_08_20,para_val_00_00},    //20 ��˪���5
    {msg_para_small_08_21,para_val_00_00},    //21 ��˪�����1
    {msg_para_small_08_22,para_val_00_00},    //22 ��˪�����2
    {msg_para_small_08_23,para_val_00_00},    //23 ��˪�����3
    {msg_para_small_08_24,para_val_00_00},    //24 ��˪�����4
    {msg_para_small_08_25,para_val_00_00},    //25 ��˪�����5
    {msg_para_small_08_26,Msg_UseOrNot},      //26 ��ѹ��˪����
    {msg_para_small_08_27,para_val_00_00},    //27 ��ѹ��˪�����ʱ
    {msg_para_small_08_28,para_val_00_00},    //28 ��ѹ��˪���
    {msg_para_small_08_29,para_val_00_00},    //29 ��ѹ��˪����
    {msg_para_small_08_30,para_val_00_00},    //30 ��ʱ��˪���
    {msg_para_small_08_31,para_val_00_00},    //31 ������˪����
    {msg_para_small_08_32,Msg_df_stop_cm},    //32 ��˪ͣ���з�
    {msg_para_small_08_33,para_val_00_00},    //33 ����˪ѹ����ʱ
    {msg_para_small_08_34,para_val_00_00},    //34 ����˪��ͨ��ʱ
    {msg_para_small_08_35,para_val_00_00},    //35 �˳�˪ѹ����ʱ
    {msg_para_small_08_36,para_val_00_00},    //36 �˳�˪��ͨ��ʱ
    {msg_para_small_08_37,para_val_00_00},    //37 ��˪�з�Ƶ��
    {msg_para_small_08_38,para_val_00_00},    //38 ͣ���ˮʱ��
    {msg_para_small_08_39,para_val_00_00},    //39 ��С��˪���
    {msg_para_small_08_40,para_val_00_00},    //40 ������˪���
};
const SMALL para_small_09[] =
{
    {msg_para_small_09_01,para_val_00_00},   // 0 �������1
    {msg_para_small_09_02,para_val_00_00},   // 1 �������2
    {msg_para_small_09_03,para_val_00_00},   // 2 ˮ�÷����¶�
    {msg_para_small_09_04,para_val_00_00},   // 3 ���ȷ����¶�
    {msg_para_small_09_05,para_val_00_00},   // 4 ѹ�������¶�
    {msg_para_small_09_06,para_val_00_00},   // 5 �����˷����¶�
    {msg_para_small_09_07,para_val_00_00},   // 6 ѹ���˷����¶�
    {msg_para_small_09_08,para_val_00_00},   // 7 �����������
    {msg_para_small_09_09,para_val_00_00},   // 8 ����������
    {msg_para_small_09_10,para_val_00_00},   // 9 �˷������²�
    {msg_para_small_09_11,Msg_UseOrNot},     // 10 ������������
    {msg_para_small_09_12,Msg_UseOrNot},     // 11 ̫����ˮ�÷��� 
    {msg_para_small_09_13,para_val_00_00},   // 12 ��ʱ������� 
    {msg_para_small_09_14,para_val_00_00},   // 13 ��ʱ����ʱ�� 
    {msg_para_small_09_15,para_val_00_00},   // 14 ����
    {msg_para_small_09_16,para_val_00_00},   // 15 ����
    {msg_para_small_09_17,para_val_00_00},   // 16 ����
    {msg_para_small_09_18,para_val_00_00},   // 17 ����
    {msg_para_small_09_19,para_val_00_00},   // 18 ����
    {msg_para_small_09_20,para_val_00_00},   // 19 ����
    {msg_para_small_09_21,para_val_00_00},   // 20 ����
    {msg_para_small_09_22,para_val_00_00},   // 21 ����
    {msg_para_small_09_23,para_val_00_00},   // 22 ����
    {msg_para_small_09_24,para_val_00_00},   // 23 ����
    {msg_para_small_09_25,para_val_00_00},   // 24 ����
    {msg_para_small_09_26,para_val_00_00},   // 25 ����
    {msg_para_small_09_27,para_val_00_00},   // 26 ����
    {msg_para_small_09_28,para_val_00_00},   // 27 ����
    {msg_para_small_09_29,para_val_00_00},   // 28 ����
    {msg_para_small_09_30,para_val_00_00},   // 29 ����
    {msg_para_small_09_31,para_val_00_00},   // 30 ����
    {msg_para_small_09_32,para_val_00_00},   // 31 ����ת����1
    {msg_para_small_09_33,para_val_00_00},   // 32 ����ת����2
    {msg_para_small_09_34,para_val_00_00},   // 33 ����ת����3
    {msg_para_small_09_35,para_val_00_00},   // 34 ����ת����4
    {msg_para_small_09_36,para_val_00_00},   // 35 ����ת����5

};
const SMALL para_small_10[] =
{
    {msg_para_small_10_01,para_val_00_00},  /* 1 һ�������ʱ */
    {msg_para_small_10_02,para_val_00_00},  /* 2 ˮ����������ʱ */
    {msg_para_small_10_03,para_val_00_00},  /* 3 ���ϸ�λʱ�� */
    {msg_para_small_10_04,para_val_00_00},  /* 4 �Զ���λ����ʱ�� */
    {msg_para_small_10_05,para_val_00_00},  /* 5 ��ѹ���ϼ����ʱ */
    {msg_para_small_10_06,para_val_00_00},  /* 6 �˳�˪��ѹ���� */
    {msg_para_small_10_07,para_val_00_00},  /* 7 �����ѹ������ʱ */
    {msg_para_small_10_08,para_val_00_00},  /* 8 ���ȵ�ѹ������ʱ */
    {msg_para_small_10_09,para_val_00_00},  /* 9 �˳��±����²� */
    {msg_para_small_10_10,para_val_00_00},  /* 10 ������¹��� */
    {msg_para_small_10_11,para_val_00_00},  /* 11 ���ȳ��¹��� */
    {msg_para_small_10_12,para_val_00_00},  /* 12 ����������� */
    {msg_para_small_10_13,para_val_00_00},  /* 13 �����²��쳣 */
    {msg_para_small_10_14,para_val_00_00},  /* 14 �����쳣���� */
    {msg_para_small_10_15,Msg_UseOrNot},    /* 15 ˮ�����㿪�� */
    {msg_para_small_10_16,para_val_00_00},  /* 16 ǿ�ƽ�Ƶ���� */
    {msg_para_small_10_17,para_val_00_00},  /* 17 ǿ�ƽ�Ƶ���� */
    {msg_para_small_10_18,para_val_00_00},  /* 18 �����²���� */
    {msg_para_small_10_19,para_val_00_00},  /* 19 ������������ */
    {msg_para_small_10_20,para_val_00_00},  /* 20 �����¶ȹ��� */
    {msg_para_small_10_21,para_val_00_00},  /* 21 �����¹����²� */
    {msg_para_small_10_22,para_val_00_00},  /* 22 ������������ */
    {msg_para_small_10_23,para_val_00_00},  /* 23 PWM���ٹ��� */
    {msg_para_small_10_24,para_val_00_00},  /* 24 PWM���ټ����ʱ */
    {msg_para_small_10_25,Msg_UseOrNot},    /* 25 ��������ѹ */
    {msg_para_small_10_26,para_val_00_00},  /* 26 ���ȳ�������1  */
    {msg_para_small_10_27,para_val_00_00},  /* 27 ���ȳ�������2  */
    {msg_para_small_10_28,para_val_00_00},  /* 28 ���ȳ�������3  */
    {msg_para_small_10_29,para_val_00_00},  /* 29 ���ȳ�������4  */
    {msg_para_small_10_30,para_val_00_00},  /* 30 ���ȳ�������5 */
    {msg_para_small_10_31,para_val_00_00},  /* 31 ��ů���ȱ��� */
    {msg_para_small_10_32,para_val_00_00},  /* 32 ��ֹ��ѹ���� */
    {msg_para_small_10_33,para_val_00_00},  /* 33 �����ѹ���ͣ���Χ�Ե�ѹ����Ϊ׼��  */
    {msg_para_small_10_34,para_val_00_00},  /* 34 ���ȵ�ѹ���ͣ���Χ�Ե�ѹ����Ϊ׼��  */
    {msg_para_small_10_35,para_val_00_00},  /* 35 �����ѹ���ߣ���Χ�Ը�ѹ����Ϊ׼��  */
    {msg_para_small_10_36,para_val_00_00},  /* 36 ���ȸ�ѹ���ߣ���Χ�Ը�ѹ����Ϊ׼��  */
    {msg_para_small_10_37,para_val_00_00},  /* 37 ��ѹ������λѹ�� */
    {msg_para_small_10_38,para_val_00_00},  /* 38 ��ѹ������λѹ�� */
    {msg_para_small_10_39,para_val_00_00},  /* 39 �������价�� */
    {msg_para_small_10_40,para_val_00_00},  /* 40 ����������� */
    {msg_para_small_10_41,Msg_UseOrNot},    /* 41 ��������У�� */
    {msg_para_small_10_42,para_val_00_00},  /* 42 ����������� */
    {msg_para_small_10_43,Msg_UseOrNot},	/* 43 �߿���ͨѶ��� */
    {msg_para_small_10_44,Msg_invt_err_auto_rst},//44 ��Ƶ�����Ը�λ
    {msg_para_small_10_45,para_val_00_00},  // 45 ������ˮ����
    {msg_para_small_10_46,para_val_00_00},  // 46 ��ѹ�����ز�
    {msg_para_small_10_47,para_val_00_00},  // 47 ��ѹ�����ز�
};
const SMALL para_small_11[] =
{
    {msg_para_small_11_01,Msg_InputNormal},   //00 ѹ����ѹ
    {msg_para_small_11_02,Msg_InputNormal},   //01 ѹ����ѹ
    {msg_para_small_11_03,Msg_InputNormal},   //02 ���õ��ȷ���
    {msg_para_small_11_04,Msg_InputNormal},   //03 ˮ������
    {msg_para_small_11_05,Msg_InputNormal},   //04 ˮ����ȷ���
    {msg_para_small_11_06,Msg_UseOrNot},      //05 ��ý�¶�����
    {msg_para_small_11_07,para_val_00_00},    //06 ��ý��������1
    {msg_para_small_11_08,para_val_00_00},    //07 ��ý��������2
    {msg_para_small_11_09,para_val_00_00},    //08 ��ý��������3
    {msg_para_small_11_10,para_val_00_00},    //09 ��ý��������4
    {msg_para_small_11_11,para_val_00_00},    //10 ��ý��������5
    {msg_para_small_11_12,para_val_00_00},    //11 ��ýҺ������1
    {msg_para_small_11_13,para_val_00_00},    //12 ��ýҺ������2
    {msg_para_small_11_14,para_val_00_00},    //13 ��ýҺ������3
    {msg_para_small_11_15,para_val_00_00},    //14 ��ýҺ������4
    {msg_para_small_11_16,para_val_00_00},    //15 ��ýҺ������5
    {msg_para_small_11_17,para_val_00_00},    //16 ��ýҺ������6
    {msg_para_small_11_18,para_val_00_00},    //17 ��ýҺ������7
    {msg_para_small_11_19,Msg_UseOrNot},      //18 ˮ���¶�̽ͷ
    {msg_para_small_11_20,Msg_UseOrNot},      //19 ��ů��ˮ�¶�̽ͷ
    {msg_para_small_11_21,Msg_UseOrNot},      //20 ϵͳ�ܳ�ˮ�¶�̽ͷ
    {msg_para_small_11_22,Msg_InputNormal},   //21 EUV�ź�
    {msg_para_small_11_23,Msg_InputNormal},   //22 SG�ź�
    {msg_para_small_11_24,Msg_sensor_cfg},  /* 35 �¶�̽ͷ���� */
    {msg_para_small_11_25,Msg_UseOrNot},    /* 32 ��Ԫˮ������ */
    {msg_para_small_11_26,Msg_UseOrNot},    /* 33 ̫�����¶�̽ͷ */
    {msg_para_small_11_27,Msg_AiSensorType},/* 00 J5���������� */
    {msg_para_small_11_28,Msg_AiSensorType},/* 01 J6���������� */
    {msg_para_small_11_29,para_val_00_00},  /* 02 J5��С����  */
    {msg_para_small_11_30,para_val_00_00},  /* 03 J5�������  */
    {msg_para_small_11_31,para_val_00_00}, //04 J6��С����
    {msg_para_small_11_32,para_val_00_00}, //05 J6�������
    {msg_para_small_11_33,Msg_UseOrNot},	/* ����ˮ���ϲ��¶�̽ͷ */
    {msg_para_small_11_34,Msg_UseOrNot},	/* ����ˮ���²��¶�̽ͷ */
    {msg_para_small_11_35,Msg_press_cfg},	/* ѹ������������ */
    {msg_para_small_11_36,para_val_00_00},	/* ������� */
    {msg_para_small_11_37,para_val_00_00},	/* ˮ�ù��� */
    {msg_para_small_11_38,para_val_00_00},	/* ����1���� */
    {msg_para_small_11_39,para_val_00_00},	/* ����2���� */
    {msg_para_small_11_40,para_val_00_00},	/* �������� */
    {msg_para_small_11_41,para_val_00_00},	/* �������й��� */
    {msg_para_small_11_42,Msg_UseOrNot},	/* �ܳ�ˮ�¶�̽ͷ */
};
const SMALL para_small_12[] =
{
    {msg_para_small_12_01,Msg_ability_test},//01 {��������ѡ��}
    {msg_para_small_12_02,para_val_00_00},  //02 {����ѹ��ת��A}
    {msg_para_small_12_03,para_val_00_00},  //03 {���ȷ��ת��A}
    {msg_para_small_12_04,para_val_00_00},  //04 {������������A}
    {msg_para_small_12_05,para_val_00_00},  //05 {����ѹ��ת��B}
    {msg_para_small_12_06,para_val_00_00},  //06 {���ȷ��ת��B}
    {msg_para_small_12_07,para_val_00_00},  //07 {������������B}
    {msg_para_small_12_08,para_val_00_00},  //08 {����ѹ��ת��E}
    {msg_para_small_12_09,para_val_00_00},  //09 {���ȷ��ת��E}
    {msg_para_small_12_10,para_val_00_00},  //10 {������������E}
    {msg_para_small_12_11,para_val_00_00},  //11 {����ѹ��ת��G}
    {msg_para_small_12_12,para_val_00_00},  //12 {���ȷ��ת��G}
    {msg_para_small_12_13,para_val_00_00},  //13 {������������G}
    {msg_para_small_12_14,para_val_00_00},  //14 {����ѹ��ת��C}
    {msg_para_small_12_15,para_val_00_00},  //15 {���ȷ��ת��C}
    {msg_para_small_12_16,para_val_00_00},  //16 {������������C}
    {msg_para_small_12_17,para_val_00_00},  //17 {����ѹ��ת��D}
    {msg_para_small_12_18,para_val_00_00},  //18 {���ȷ��ת��D}
    {msg_para_small_12_19,para_val_00_00},  //19 {������������D}
    {msg_para_small_12_20,para_val_00_00},  //20 {����ѹ��ת��}
    {msg_para_small_12_21,para_val_00_00},  //21 {������ת��}
    {msg_para_small_12_22,para_val_00_00},  //22 {������������}
    {msg_para_small_12_23,para_val_00_00},  //23 {���ȳ�˪����A}
    {msg_para_small_12_24,para_val_00_00},  //24 {���ȳ�˪���A}
    {msg_para_small_12_25,para_val_00_00},  //25 {���ȳ�˪����A}
    {msg_para_small_12_26,para_val_00_00},  //26 {���ȳ�˪ת��A}
    {msg_para_small_12_27,para_val_00_00},  //27 {�����˳�˪����A}
    {msg_para_small_12_28,para_val_00_00},  //28 {���ȳ�˪ʱ��A}
    {msg_para_small_12_29,para_val_00_00},  //29 {���ȳ�˪����B}
    {msg_para_small_12_30,para_val_00_00},  //30 {���ȳ�˪���B}
    {msg_para_small_12_31,para_val_00_00},  //31 {���ȳ�˪����B}
    {msg_para_small_12_32,para_val_00_00},  //32 {���ȳ�˪ת��B}
    {msg_para_small_12_33,para_val_00_00},  //33 {�����˳�˪����B}
    {msg_para_small_12_34,para_val_00_00},  //34 {���ȳ�˪ʱ��B}
    {msg_para_small_12_35,para_val_00_00},  //35 {���ȳ�˪����E}
    {msg_para_small_12_36,para_val_00_00},  //36 {���ȳ�˪���E}
    {msg_para_small_12_37,para_val_00_00},  //37 {���ȳ�˪����E}
    {msg_para_small_12_38,para_val_00_00},  //38 {���ȳ�˪ת��E}
    {msg_para_small_12_39,para_val_00_00},  //39 {�����˳�˪����E}
    {msg_para_small_12_40,para_val_00_00},  //40 {���ȳ�˪ʱ��E}
    {msg_para_small_12_41,para_val_00_00},  //41 {���ȳ�˪����G}
    {msg_para_small_12_42,para_val_00_00},  //42 {���ȳ�˪���G}
    {msg_para_small_12_43,para_val_00_00},  //43 {���ȳ�˪����G}
    {msg_para_small_12_44,para_val_00_00},  //44 {���ȳ�˪ת��G}
    {msg_para_small_12_45,para_val_00_00},  //45 {�����˳�˪����G}
    {msg_para_small_12_46,para_val_00_00},  //46 {���ȳ�˪ʱ��G}
    {msg_para_small_12_47,Msg_pfc_set},     //47 {PFC����}
    {msg_para_small_12_48,para_val_00_00},  //48 {�ֶ���Ƶˮ��}
};

const SMALL para_small_13[] =
{
    {msg_para_small_13_01,para_val_00_00},   //01 ������������
    {msg_para_small_13_02,para_val_00_00},   /*02 �������ƫ��  */
    {msg_para_small_13_03,para_val_00_00},   /*03 ����ж��ƫ��  */
    {msg_para_small_13_04,para_val_00_00},   /*04 ���ȼ���ƫ��  */
    {msg_para_small_13_05,para_val_00_00},   /*05 ����ж��ƫ��  */
    {msg_para_small_13_06,para_val_00_00},   /*06 ���»ز�  */
    {msg_para_small_13_07,para_val_00_00},   //07 �����������
    {msg_para_small_13_08,para_val_00_00},   //08 �����������
    {msg_para_small_13_09,para_val_00_00},   //09 ���Ȼ�������
    {msg_para_small_13_10,para_val_00_00},   //10 ���Ȼ�������
    {msg_para_small_13_11,para_val_00_00},   //11 ������ͻ���
    {msg_para_small_13_12,para_val_00_00},   //12 ������߻���
    {msg_para_small_13_13,para_val_00_00},   //13 ������ͻ���
    {msg_para_small_13_14,para_val_00_00},   //14 ������߻���
    {msg_para_small_13_15,para_val_00_00},   //15 ��ˮ��ͻ���
    {msg_para_small_13_16,para_val_00_00},   //16 ��ˮ��߻���
    {msg_para_small_13_17,para_val_00_00},   //17 �Զ�������ͻ�
    {msg_para_small_13_18,para_val_00_00},   //18 �Զ�������߻�
    {msg_para_small_13_19,para_val_00_00},   /*19 ����������� */
    {msg_para_small_13_20,para_val_00_00},   /*20 ���������ˮ */
    {msg_para_small_13_21,para_val_00_00},   /*21 ��������ʱ�� */
    {msg_para_small_13_22,Msg_ibh_ahs_use},  //22 ��������
    {msg_para_small_13_23,Msg_ibh_ahs_use},  //23 �ⲿ��Դ
    {msg_para_small_13_24,para_val_00_00},   /*24 ���ȿ������²� */
    {msg_para_small_13_25,para_val_00_00},   /*25 ���ȿ������²� */
    {msg_para_small_13_26,para_val_00_00},   //26 �������ȵ���
    {msg_para_small_13_27,para_val_00_00},   //27 �������ȿ�����
    {msg_para_small_13_28,para_val_00_00},   //28 �������ȿ�����
    {msg_para_small_13_29,para_val_00_00},   //29 �������ȿ��²�
    {msg_para_small_13_30,para_val_00_00},   //30 ˮ����ȿ�����
    {msg_para_small_13_31,para_val_00_00},   //31 ˮ����ȿ�����
    {msg_para_small_13_32,para_val_00_00},   //32 �ⲿ��Դ������
    {msg_para_small_13_33,para_val_00_00},   //33 �ⲿ��Դ���²�
    {msg_para_small_13_34,para_val_00_00},   //34 �ⲿ��Դ���²�
    {msg_para_small_13_35,para_val_00_00},   //35 �ⲿ��Դ������
    {msg_para_small_13_36,para_val_00_00},   /*36 ��������ˮʱ�� */
    {msg_para_small_13_37,para_val_00_00},   /*37 ��������ˮ��� */
    {msg_para_small_13_38,Msg_fcu_cpst_set}, //38 �ܵ���������
    {msg_para_small_13_39,para_val_00_00},   //39 ѹ��ȫ���²�
    {msg_para_small_13_40,para_val_00_00},   //40 �̼����Ȳ���
    {msg_para_small_13_41,para_val_00_00},   //41 �̼�����Ƶ��
    {msg_para_small_13_42,para_val_00_00},   //42 �̼�����ʱ��
    {msg_para_small_13_43,para_val_00_00},   //43 �̼����䲽��
    {msg_para_small_13_44,para_val_00_00},   //44 �̼�����Ƶ��

    {msg_para_small_13_45,para_val_00_00},   //45 �̼�����ʱ��
    {msg_para_small_13_46,para_val_00_00},   //46 �̼����ʱ��
    {msg_para_small_13_47,para_val_00_00},   //47 �̼����ȷ���
    {msg_para_small_13_48,para_val_00_00},   //48 �̼��������
};

const SMALL para_small_14[] =
{
    {msg_para_small_14_01,Msg_PUMPf_ctrl_type},             /* 01 ��Ƶˮ�ÿ��� */
    {msg_para_small_14_02,para_val_00_00},                  /* 02 ˮ�öת�� */
    {msg_para_small_14_03,para_val_00_00},                  /* 03 ˮ��Ŀ��ת�� */
    {msg_para_small_14_04,para_val_00_00},                  /* 04 ��Ƶˮ�ó��� */
    {msg_para_small_14_05,para_val_00_00},                  /* 05 ˮ��Ŀ������ */
    {msg_para_small_14_06,para_val_00_00},                  /* 06 �»�ˮ������ */
    {msg_para_small_14_07,Msg_UseOrNot},                    /* 07 �»�ˮ��ɱ�� */
    {msg_para_small_14_08,Msg_UseOrNot},                    /* 08 �»�ˮ�ö�ʱ */
    {msg_para_small_14_09,para_val_00_00},                  /* 09 ��ůԤ���¶� */
    {msg_para_small_14_10,para_val_00_00},                  /* 10 ��ůԤ�ȼ�� */
    {msg_para_small_14_11,para_val_00_00},                  /* 11 ��ůԤ��ʱ�� */
    {msg_para_small_14_12,para_val_00_00},                  /* 12 ��ůˮ�»ز� */
    {msg_para_small_14_13,para_val_00_00},                  /* 13 ��ů���»ز� */
    {msg_para_small_14_14,para_val_00_00},                  /* 14 ��ů���ǰ */
    {msg_para_small_14_15,para_val_00_00},                  /* 15 ��ů����� */
    {msg_para_small_14_16,para_val_00_00},                  /* 16 ��ů��ɺ� */
    {msg_para_small_14_17,para_val_00_00},                  /* 17 ��ů����¶� */
    {msg_para_small_14_18,Msg_termial_device_type},         /* 18 A������ĩ�� */
    {msg_para_small_14_19,Msg_termial_device_type},         /* 19 A������ĩ�� */
    {msg_para_small_14_20,Msg_termial_device_type},         /* 20 B������ĩ�� */
    {msg_para_small_14_21,Msg_termial_device_type},         /* 21 B������ĩ�� */
    {msg_para_small_14_22,para_val_00_00},                  /* 22 ĩ�˸��� */
    {msg_para_small_14_23,para_val_00_00},                  /* 23 ĩ����ʼ��ַ */
    {msg_para_small_14_24,Msg_UseOrNot},                    /* 24 ��������ĩ�� */
    {msg_para_small_14_25,Msg_UseOrNot},                    /* 25 �����ر�ĩ�� */
    {msg_para_small_14_26,Msg_fcu_s_start_m},               /* 26 ĩ�˿������� */
    {msg_para_small_14_27,Msg_fcu_mode_set},                /* 27 ��ĩģʽ���� */
    {msg_para_small_14_28,para_val_00_00},                  /* 28 ĩ���ܵ����� */
    {msg_para_small_14_29,para_val_00_00},                  /* 29 ̫������ˮˮ�� */
    {msg_para_small_14_30,para_val_00_00},                  /* 30 ̫������ˮ�ز� */
    {msg_para_small_14_31,para_val_00_00},                  /* 31 ̫���ܿ���ˮ�� */
    {msg_para_small_14_32,para_val_00_00},                  /* 32 ̫���ܿ����ز� */
    {msg_para_small_14_33,Msg_UseOrNot  },                  /* 33 �շ��������� */
    {msg_para_small_14_34,para_val_00_00},                  /* 34 �շ��������� */
    {msg_para_small_14_35,para_val_00_00},                  /* 35 �շ���ѹ���� */
    {msg_para_small_14_36,para_val_00_00},                  /* 36 S3V3�������� */
    {msg_para_small_14_37,para_val_00_00},                  /* 37 ��ѭ����Сʱ�� */
    {msg_para_small_14_38,para_val_00_00},                  /* 38 ���� */
    {msg_para_small_14_39,para_val_00_00},                  /* 39 ���� */
    {msg_para_small_14_40,para_val_00_00},                  /* 40 ģ����� */
    {msg_para_small_14_41,para_val_00_00},                  /* 41 ��ˮģ���� */
    {msg_para_small_14_42,para_val_00_00},                  /* 42 ���� */
    {msg_para_small_14_43,para_val_00_00},                  /* 43 ���� */
    {msg_para_small_14_44,para_val_00_00},                  /* 44 ���� */
    {msg_para_small_14_45,para_val_00_00},                  /* 45 ���� */
    {msg_para_small_14_46,data_format},                     /* 46 BMS���ݸ�ʽ */
    {msg_para_small_14_47,data_bps},                        /* 47 BMS������   */
    {msg_para_small_14_48,para_val_00_00},                  /* 48 BMSͨѶ��ַ */
};

const SMALL para_small_15[] =
#if defined(USE_WF_OLD)
{
    {msg_para_small_15_01,para_val_00_00},
    {msg_para_small_15_02,para_val_00_00},
    {msg_para_small_15_03,para_val_00_00},
    {msg_para_small_15_04,para_val_00_00},
    {msg_para_small_15_05,para_val_00_00},
    {msg_para_small_15_06,para_val_00_00},
    {msg_para_small_15_07,para_val_00_00},
    {msg_para_small_15_08,para_val_00_00},
    {msg_para_small_15_09,para_val_00_00},
    {msg_para_small_15_10,para_val_00_00},
    {msg_para_small_15_11,para_val_00_00},
    {msg_para_small_15_12,para_val_00_00},
    {msg_para_small_15_13,para_val_00_00},
    {msg_para_small_15_14,para_val_00_00},
    {msg_para_small_15_15,para_val_00_00},
    {msg_para_small_15_16,para_val_00_00},
    {msg_para_small_15_17,para_val_00_00},
    {msg_para_small_15_18,para_val_00_00},
    {msg_para_small_15_19,para_val_00_00},
    {msg_para_small_15_20,para_val_00_00},
    {msg_para_small_15_21,para_val_00_00},
    {msg_para_small_15_22,para_val_00_00},
    {msg_para_small_15_23,para_val_00_00},
    {msg_para_small_15_24,para_val_00_00},
    {msg_para_small_15_25,para_val_00_00},
    {msg_para_small_15_26,para_val_00_00},
    {msg_para_small_15_27,para_val_00_00},
    {msg_para_small_15_28,para_val_00_00},
    {msg_para_small_15_29,para_val_00_00},
    {msg_para_small_15_30,para_val_00_00},
    {msg_para_small_15_31,para_val_00_00},
    {msg_para_small_15_32,para_val_00_00},
    {msg_para_small_15_33,para_val_00_00},
    {msg_para_small_15_34,para_val_00_00},
    {msg_para_small_15_35,para_val_00_00},
    {msg_para_small_15_36,para_val_00_00},
    {msg_para_small_15_37,para_val_00_00},
    {msg_para_small_15_38,para_val_00_00},
    {msg_para_small_15_39,para_val_00_00},
    {msg_para_small_15_40,para_val_00_00},
    {msg_para_small_15_41,para_val_00_00},
    {msg_para_small_15_42,para_val_00_00},
    {msg_para_small_15_43,para_val_00_00},
    {msg_para_small_15_44,para_val_00_00},
    {msg_para_small_15_45,para_val_00_00},
    {msg_para_small_15_46,para_val_00_00},
};
#elif defined(USE_WF_SHENGNENG)
{
    {msg_para_small_15_01_shengneng,para_val_00_00},
    {msg_para_small_15_02_shengneng,para_val_00_00},
    {msg_para_small_15_03_shengneng,para_val_00_00},
    {msg_para_small_15_04_shengneng,para_val_00_00},
    {msg_para_small_15_05_shengneng,para_val_00_00},
    {msg_para_small_15_06_shengneng,para_val_00_00},
    {msg_para_small_15_07_shengneng,para_val_00_00},
    {msg_para_small_15_08_shengneng,para_val_00_00},
    {msg_para_small_15_09_shengneng,para_val_00_00},
    {msg_para_small_15_10_shengneng,para_val_00_00},
    {msg_para_small_15_11_shengneng,para_val_00_00},
    {msg_para_small_15_12_shengneng,para_val_00_00},
    {msg_para_small_15_13_shengneng,para_val_00_00},
    {msg_para_small_15_14_shengneng,para_val_00_00},
    {msg_para_small_15_15_shengneng,para_val_00_00},
    {msg_para_small_15_16_shengneng,para_val_00_00},
    {msg_para_small_15_17_shengneng,para_val_00_00},
    {msg_para_small_15_18_shengneng,para_val_00_00},
    {msg_para_small_15_19_shengneng,para_val_00_00},
    {msg_para_small_15_20_shengneng,para_val_00_00},
    {msg_para_small_15_21_shengneng,para_val_00_00},
    {msg_para_small_15_22_shengneng,para_val_00_00},
    {msg_para_small_15_23_shengneng,para_val_00_00},
    {msg_para_small_15_24_shengneng,para_val_00_00},
    {msg_para_small_15_25_shengneng,para_val_00_00},
    {msg_para_small_15_26_shengneng,para_val_00_00},
    {msg_para_small_15_27_shengneng,para_val_00_00},
    {msg_para_small_15_28_shengneng,para_val_00_00},
    {msg_para_small_15_29_shengneng,para_val_00_00},
    {msg_para_small_15_30_shengneng,para_val_00_00},
    {msg_para_small_15_31_shengneng,para_val_00_00},
    {msg_para_small_15_32_shengneng,para_val_00_00},
    {msg_para_small_15_33_shengneng,para_val_00_00},
    {msg_para_small_15_34_shengneng,para_val_00_00},
    {msg_para_small_15_35_shengneng,para_val_00_00},
    {msg_para_small_15_36_shengneng,para_val_00_00},
    {msg_para_small_15_37_shengneng,para_val_00_00},
    {msg_para_small_15_38_shengneng,para_val_00_00},
    {msg_para_small_15_39_shengneng,para_val_00_00},
    {msg_para_small_15_40_shengneng,para_val_00_00},
    {msg_para_small_15_41_shengneng,para_val_00_00},
    {msg_para_small_15_42_shengneng,para_val_00_00},
    {msg_para_small_15_43_shengneng,para_val_00_00},
    {msg_para_small_15_44_shengneng,para_val_00_00},
    {msg_para_small_15_45_shengneng,para_val_00_00},
    {msg_para_small_15_46_shengneng,para_val_00_00},
    {msg_para_small_15_47_shengneng,para_val_00_00},
    {msg_para_small_15_48_shengneng,para_val_00_00},
};
#else
{
    {msg_para_small_15_01_new,para_val_00_00},
    {msg_para_small_15_02_new,para_val_00_00},
    {msg_para_small_15_03_new,para_val_00_00},
    {msg_para_small_15_04_new,para_val_00_00},
    {msg_para_small_15_05_new,para_val_00_00},
    {msg_para_small_15_06_new,para_val_00_00},
    {msg_para_small_15_07_new,para_val_00_00},
    {msg_para_small_15_08_new,para_val_00_00},
    {msg_para_small_15_09_new,para_val_00_00},
    {msg_para_small_15_10_new,para_val_00_00},
    {msg_para_small_15_11_new,para_val_00_00},
    {msg_para_small_15_12_new,para_val_00_00},
    {msg_para_small_15_13_new,para_val_00_00},
    {msg_para_small_15_14_new,para_val_00_00},
    {msg_para_small_15_15_new,para_val_00_00},
    {msg_para_small_15_16_new,para_val_00_00},
    {msg_para_small_15_17_new,para_val_00_00},
    {msg_para_small_15_18_new,para_val_00_00},
    {msg_para_small_15_19_new,para_val_00_00},
    {msg_para_small_15_20_new,para_val_00_00},
    {msg_para_small_15_21_new,para_val_00_00},
    {msg_para_small_15_22_new,para_val_00_00},
    {msg_para_small_15_23_new,para_val_00_00},
    {msg_para_small_15_24_new,para_val_00_00},
    {msg_para_small_15_25_new,para_val_00_00},
    {msg_para_small_15_26_new,para_val_00_00},
    {msg_para_small_15_27_new,para_val_00_00},
    {msg_para_small_15_28_new,para_val_00_00},
    {msg_para_small_15_29_new,para_val_00_00},
    {msg_para_small_15_30_new,para_val_00_00},
    {msg_para_small_15_31_new,para_val_00_00},
    {msg_para_small_15_32_new,para_val_00_00},
    {msg_para_small_15_33_new,para_val_00_00},
    {msg_para_small_15_34_new,para_val_00_00},
    {msg_para_small_15_35_new,para_val_00_00},
    {msg_para_small_15_36_new,para_val_00_00},
    {msg_para_small_15_37_new,para_val_00_00},
    {msg_para_small_15_38_new,para_val_00_00},
    {msg_para_small_15_39_new,para_val_00_00},
    {msg_para_small_15_40_new,para_val_00_00},
    {msg_para_small_15_41_new,para_val_00_00},
    {msg_para_small_15_42_new,para_val_00_00},
    {msg_para_small_15_43_new,para_val_00_00},
    {msg_para_small_15_44_new,para_val_00_00},
    {msg_para_small_15_45_new,para_val_00_00},
    {msg_para_small_15_46_new,para_val_00_00},
    {msg_para_small_15_47_new,para_val_00_00},
    {msg_para_small_15_48_new,para_val_00_00},
};
#endif
/* ����С�� */
const SMALL *para_small[GROUP_MAX] =
{
    para_small_01,  // 01 �û�����
    para_small_02,  // 02 ϵͳ����1
    para_small_03,  // 03
    para_small_04,  // 04
    para_small_05,  // 05
    para_small_06,  // 06
    para_small_07,  // 07
    para_small_08,  // 08
    para_small_09,  // 09
    para_small_10,  // 10
    para_small_11,  // 11
    para_small_12,  // 12
    para_small_13,  // 13
    para_small_14,  // 14
    para_small_15,  // 15
};
/* �������� */      // �˴�ͣ�ã�ȥ�޸�MenuItem_ParaGroup
const U16 para_large[] =
{
    msg_para_large_01,
    msg_para_large_02,
    msg_para_large_03,
    msg_para_large_04,
    msg_para_large_05,
    msg_para_large_06,
    msg_para_large_07,
    msg_para_large_08,
    msg_para_large_09,
    msg_para_large_10,
    msg_para_large_11,
    msg_para_large_12,
    msg_para_large_13,
    msg_para_large_14,
    msg_para_large_15,
};



const SMALL limit_set_msg[] =   // ϵͳά�������ַ���
{
    {   msg_limit_set_01,   para_val_00_00, },  /* 01 {ϵͳά������    } */
    {   msg_limit_set_02,   Msg_LimitType,  },  /* 02 {ϵͳά��ģʽ    } */
};

/* ������Ϣ */
/* ϵͳ����� */
const U16 msg_m_error[] =
{
    msg_m_error_00, // {}
    msg_m_error_01, // {���ౣ��}
    msg_m_error_02, // {EEPROM���ݴ����}
    msg_m_error_03, // {ϵͳ���¹���}
    msg_m_error_04, // {ϵͳ���¹���}
    msg_m_error_05, // {ϵͳ���¹���}
    msg_m_error_06, // {ϵͳά�����ݴ�}
    msg_m_error_07, // {ȱ�ౣ��}
    msg_m_error_08, // {�ͷ�ѹ��}
    msg_m_error_09, // {ˮ������}
    msg_m_error_10, // {���õ��ȹ���}
    msg_m_error_11, // {ˮ����ȹ���}
    msg_m_error_12, // {ˮ���¶ȹ���}
    msg_m_error_13, // {�ܳ�ˮ�¶ȹ���}
    msg_m_error_14, // {ϵͳ�ܳ��¹���}
    msg_m_error_15,  // {��ů�ܽ��¹���}
    msg_m_error_16,  // {����01ͨѶ����}
    msg_m_error_17,  // {����02ͨѶ����}
    msg_m_error_18, // {����03ͨѶ����}
    msg_m_error_19, // {����04ͨѶ����}
    msg_m_error_20, // {����05ͨѶ����}
    msg_m_error_21, // {����06ͨѶ����}
    msg_m_error_22, // {����07ͨѶ����}
    msg_m_error_23, // {����08ͨѶ����}
    msg_m_error_24, // {����09ͨѶ����}
    msg_m_error_25, // {����10ͨѶ����}
    msg_m_error_26, // {����11ͨѶ����}
    msg_m_error_27, // {����12ͨѶ����}
    msg_m_error_28, // {����13ͨѶ����}
    msg_m_error_29, // {����14ͨѶ����}
    msg_m_error_30, // {����15ͨѶ����}
    msg_m_error_31, // {����16ͨѶ����}
    msg_m_error_32, // {����01����}
    msg_m_error_33, // {����02����}
    msg_m_error_34, // {����03����}
    msg_m_error_35, // {����04����}
    msg_m_error_36, // {����05����}
    msg_m_error_37, // {����06����}
    msg_m_error_38, // {����07����}
    msg_m_error_39, // {����08����}
    msg_m_error_40, // {����09����}
    msg_m_error_41, // {����10����}
    msg_m_error_42, // {����11����}
    msg_m_error_43, // {����12����}
    msg_m_error_44, // {����13����}
    msg_m_error_45, // {����14����}
    msg_m_error_46, // {����15����}
    msg_m_error_47, // {����16����}
    msg_m_error_48, // {����17ͨѶ����}
    msg_m_error_49, // {����18ͨѶ����}
    msg_m_error_50, // {����19ͨѶ����}
    msg_m_error_51, // {����20ͨѶ����}
    msg_m_error_52, // {����21ͨѶ����}
    msg_m_error_53, // {����22ͨѶ����}
    msg_m_error_54, // {����23ͨѶ����}
    msg_m_error_55, // {����24ͨѶ����}
    msg_m_error_56, // {����25ͨѶ����}
    msg_m_error_57, // {����26ͨѶ����}
    msg_m_error_58, // {����27ͨѶ����}
    msg_m_error_59, // {����28ͨѶ����}
    msg_m_error_60, // {����29ͨѶ����}
    msg_m_error_61, // {����30ͨѶ����}
    msg_m_error_62, // {����31ͨѶ����}
    msg_m_error_63, // {����32ͨѶ����}
    msg_m_error_64, // {����17����}
    msg_m_error_65, // {����18����}
    msg_m_error_66, // {����19����}
    msg_m_error_67, // {����20����}
    msg_m_error_68, // {����21����}
    msg_m_error_69, // {����22����}
    msg_m_error_70, // {����23����}
    msg_m_error_71, // {����24����}
    msg_m_error_72, // {����25����}
    msg_m_error_73, // {����26����}
    msg_m_error_74, // {����27����}
    msg_m_error_75, // {����28����}
    msg_m_error_76, // {����29����}
    msg_m_error_77, // {����30����}
    msg_m_error_78, // {����31����}
    msg_m_error_79, // {����32����}
    msg_m_error_80, // {����ˮ���ϲ��¶ȹ���}
    msg_m_error_81, // {����ˮ���²��¶ȹ���}
    msg_m_error_82, // {̫�����¶ȹ���}
    msg_m_error_83, // {�߿���ͨѶ����}
    msg_m_error_84, // {�յ��ܳ�ˮ�¶ȹ���}
    msg_m_error_85, // {�����¶ȹ���}
    msg_m_error_86, // {����}
};


/* ��Ƶ������ */
const U16 msg_invt_error[] =
{
    msg_invt_error_00,
    msg_invt_error_01,
    msg_invt_error_02,
    msg_invt_error_03,
    msg_invt_error_04,
    msg_invt_error_05,
    msg_invt_error_06,
    msg_invt_error_07,
    msg_invt_error_08,
    msg_invt_error_09,
    msg_invt_error_10,
    msg_invt_error_11,
    msg_invt_error_12,
    msg_invt_error_13,
    msg_invt_error_14,
    msg_invt_error_15,
    msg_invt_error_16,
    msg_invt_error_17,
    msg_invt_error_18,
    msg_invt_error_19,
    msg_invt_error_20,
    msg_invt_error_21,
    msg_invt_error_22,
    msg_invt_error_23,
    msg_invt_error_24,
    msg_invt_error_25,
    msg_invt_error_26,
    msg_invt_error_27,
    msg_invt_error_28,
    msg_invt_error_29,
    msg_invt_error_30,
    msg_invt_error_31,
    msg_invt_error_32,
    msg_invt_error_33,
    msg_invt_error_34,
};

/* ��Ƶ�����ϴ��� */
const U16 msg_invt_error_code[] =
{
    msg_invt_error_code_00,
    msg_invt_error_code_01,
    msg_invt_error_code_02,
    msg_invt_error_code_03,
    msg_invt_error_code_04,
    msg_invt_error_code_05,
    msg_invt_error_code_06,
    msg_invt_error_code_07,
    msg_invt_error_code_08,
    msg_invt_error_code_09,
    msg_invt_error_code_10,
    msg_invt_error_code_11,
    msg_invt_error_code_12,
    msg_invt_error_code_13,
    msg_invt_error_code_14,
    msg_invt_error_code_15,
    msg_invt_error_code_16,
    msg_invt_error_code_17,
    msg_invt_error_code_18,
    msg_invt_error_code_19,
    msg_invt_error_code_20,
    msg_invt_error_code_21,
    msg_invt_error_code_22,
    msg_invt_error_code_23,
    msg_invt_error_code_24,
    msg_invt_error_code_25,
    msg_invt_error_code_26,
    msg_invt_error_code_27,
    msg_invt_error_code_28,
    msg_invt_error_code_29,
    msg_invt_error_code_30,
    msg_invt_error_code_31,
    msg_invt_error_code_32,
    msg_invt_error_code_33,
    msg_invt_error_code_34,
};


/* ģ������ */
const U16 msg_s_error[] =
{
    msg_s_error_00, // {ѹ����ѹ}
    msg_s_error_01, // {ѹ����ѹ}
    msg_s_error_02, // {ѹ����������}
    msg_s_error_03, // {ѹ��������С}
    msg_s_error_04, // {��Ƭ̽ͷ����}
    msg_s_error_05, // {����̽ͷ����}
    msg_s_error_06, // {�����¶ȹ���}
    msg_s_error_07, // {��ѹѹ������}
    msg_s_error_08, // {��ѹѹ������}
    msg_s_error_09, // {ѹ����ѹ����}
    msg_s_error_10, // {ѹ����ѹ����}
    msg_s_error_11, // {�����¶ȹ���}
    msg_s_error_12, // {�����¶ȹ���}
    msg_s_error_13, // {�����¶ȹ���}
    msg_s_error_14, // {������˪Ƶ��}
    msg_s_error_15, // {�����²��쳣}
    msg_s_error_16, // {������������}
    msg_s_error_17, // {��Ƶ����}
    msg_s_error_18, // {����}
    msg_s_error_19, // {�����¶ȹ���}
    msg_s_error_20, // {�����¶ȹ���}
    msg_s_error_21, // {���1ת���쳣}
    msg_s_error_22, // {���2ת���쳣}
    msg_s_error_23, // {EC1ͨѶ����}
    msg_s_error_24, // {EC1����}
    msg_s_error_25, // {EC2ͨѶ����}
    msg_s_error_26, // {EC2����}
    msg_s_error_27, // {��ƵͨѶ����}
    msg_s_error_28, // {��Ƶ����}
    msg_s_error_29, // {��Ƶ�ͺ�������}
    msg_s_error_30, // ����
    msg_s_error_31, // ����

    msg_s_error_32, // ��ý�����¶ȹ���
    msg_s_error_33, // ��ýҺ���¶ȹ���
    msg_s_error_34, // �����¶ȹ���
    msg_s_error_35, // ����
    msg_s_error_36, // ����
    msg_s_error_37, // ����
    msg_s_error_38, // ����
    msg_s_error_39, // ����
    msg_s_error_40, // ����
    msg_s_error_41, // ����
    msg_s_error_42, // ����
    msg_s_error_43, // ����
    msg_s_error_44, // ����
    msg_s_error_45, // ����
    msg_s_error_46, // ����
    msg_s_error_47, // ����
    msg_s_error_48, // ����
    msg_s_error_49, // ����
    msg_s_error_50, // ����
    msg_s_error_51, // ����
    msg_s_error_52, // ����
    msg_s_error_53, // ����
    msg_s_error_54, // ����
    msg_s_error_55, // ����
    msg_s_error_56, // ����
    msg_s_error_57, // ����
    msg_s_error_58, // ����
    msg_s_error_59, // ����
    msg_s_error_60, // ����
    msg_s_error_61, // ����
    msg_s_error_62, // ����
    msg_s_error_63, // ����

    msg_cell_error_00,  // {ģ�黷�¹���}
    msg_cell_error_01,  // {ͨѶ����}
    msg_cell_error_02,  // {EEPROM ���ݴ�}
    msg_cell_error_03,  // {ȱ�ౣ��}
    msg_cell_error_04,  // {���ౣ��}
    msg_cell_error_05,  // {ģ����¹���}
    msg_cell_error_06,  // {ģ����¹���}
    msg_cell_error_07,  // {ģ��廻���¹���}
    msg_cell_error_08,  // {ˮ������}
    msg_cell_error_09,  // {�������}
    msg_cell_error_10,  // {ģ��廻���¹���}
    msg_cell_error_11,  // {�����¶ȹ���}
    msg_cell_error_12,  // {����̽ͷ����}
    msg_cell_error_13,  // {�����²����}
    msg_cell_error_14,  // {�����²��쳣}
#if defined(M_TCL)
    msg_cell_error_tcl_15,// {��Ƶˮ�þ���[80%]}
#else
    msg_cell_error_15,  // {��Ƶˮ�þ���}
#endif
    msg_cell_error_16,  // {ͨѶ�쳣}
    msg_cell_error_17,   // {���ͨѶ����}
    msg_cell_error_18,   // {Э��汾����}
    msg_cell_error_19,      /* ��Ƶˮ�ù��� */
    msg_cell_error_20,      /* ���������쳣 */
};

/* ��Ƶˮ���ӹ��� */
#if defined(M_TCL)
const U16 msg_PUMPf_error[] =
{
    msg_PUMPf_error_tcl_01,
    msg_PUMPf_error_tcl_02,
    msg_PUMPf_error_tcl_03,
    msg_PUMPf_error_tcl_04,
    msg_PUMPf_error_tcl_05,
};
#else
const U16 msg_PUMPf_error[] =
{
    msg_PUMPf_error_01,
    msg_PUMPf_error_02,
    msg_PUMPf_error_03,
    msg_PUMPf_error_04,
    msg_PUMPf_error_05,
};
#endif

//
///* ״̬��ѯ�������ַ��� */
//static U08 cell_o_tbl[MAX_RELAY * RLY_BIT_NUM];   /* ģ�����״ֵ̬ */
//static U08 cell_p_tbl[MAX_PV];            /* ģ��ģ��״ֵ̬ */
//static U08 sys_cell_o_tbl[MAX_RELAY * RLY_BIT_NUM];   //ϵͳ���״̬
//static U08 sys_cell_p_tbl[MAX_PV];                    //

/* ģ��ѹ����� */
const U16 msg_uint_o_comp[] = // ��Ӧ
{
    msg_comp_out_01,    // ѹ��
    msg_comp_out_02,    // ��ͨ
    msg_comp_out_03,    // ��Һ��
    msg_comp_out_04,    // ���1
    msg_comp_out_05,    // ���2
    msg_comp_out_06,    // ����1
    msg_comp_out_07,    // ���ʷ�
    msg_comp_out_08,    // ����2
};

/* ģ�鵥Ԫ��� */
const U16 msg_uint_o_cell[]=
{
    msg_uint_out_01,		/* ��Ƶˮ�� */
    msg_uint_out_02,		/* ��������1 */
    msg_uint_out_03,		/* ��������2 */
    msg_uint_out_04,		/* ���̵���� */
    msg_uint_out_05,		/* ���������ȴ� */
    msg_uint_out_06,		/* ���̵����2 */
};

/* ϵͳ��� */
const U16 msg_sys_o[] =
{
    msg_sys_out_01,			/* ��˪ָʾ */
    msg_sys_out_02,			/* ����ָʾ */
    msg_sys_out_03,			/* �ⲿ��Դ */
    msg_sys_out_04,			/* �����ͨ��1 */
    msg_sys_out_05,			/* �����ͨ��3 */
    msg_sys_out_06,			/* �����ͨ��2 */
    msg_sys_out_07,			/* ˮ����� */
    msg_sys_out_08,			/* ���� */
    msg_sys_out_09,			/* �»�ˮ�� */
    msg_sys_out_10,			/* ̫����ˮ�� */
    msg_sys_out_11,			/* ���ˮ�� */
    msg_sys_out_12,			/* ��ѭ���� */
    msg_sys_out_13,			/* �յ��� */
    msg_sys_out_14,			/* ���� */
};

/* ϵͳģ���� */
const MSG_ITEM_INFO msg_sys_p[] =
{
    { UINT_C|D_DOT1,   msg_sys_pv_01 },		/* �����¶� */
    { UINT_C|D_DOT1,   msg_sys_pv_02 },		/* ���� */
    { UINT_C|D_DOT1,   msg_sys_pv_03 },		/* �����¶�' */
    { UINT_C|D_DOT1,   msg_sys_pv_04 },		/* ��ˮˮ���¶� */
    { UINT_C|D_DOT1,   msg_sys_pv_05 },		/* �ܳ�ˮ�¶�' */
    { UINT_C|D_DOT1,   msg_sys_pv_06 },		/* ϵͳ�ܳ�ˮ�¶� */
    { UINT_C|D_DOT1,   msg_sys_pv_07 },		/* ̫�����¶� */
    { UINT_C|D_DOT1,   msg_sys_pv_08 },		/* ����ˮ���ϲ��¶� */
    { UINT_C|D_DOT1,   msg_sys_pv_09 },		/* ����ˮ���²��¶� */
    { UINT_C|D_DOT1,   msg_sys_pv_10 },		/* �յ��ܳ�ˮ�¶� */
    { UINT_C|D_DOT1,   msg_sys_pv_11 },		/* ���� */
    { UINT_C|D_DOT1,   msg_sys_pv_12 },		/* ���� */
    { UINT_C|D_DOT1,   msg_sys_pv_13 },		/* ���� */
    { UINT_C|D_DOT1,   msg_sys_pv_14 },		/* ���� */
    { UINT_C|D_DOT1,   msg_sys_pv_15 },		/* ��ů��ˮ�¶� */
    { 0xFFFF,          msg_sys_pv_16 },		/* �����ͨ��1��� */
    { 0xFFFF,          msg_sys_pv_17 },		/* �����ͨ��2��� */
    { 0xFFFF,          msg_sys_pv_18 },		/* �����ͨ��3��� */
    { UINT_PE|D_DOT1,  msg_sys_pv_19 },		/* ���� */
    { UINT_0|D_DOT2,   msg_sys_pv_20 },		/* ���书�� */
    { UINT_0|D_DOT2,   msg_sys_pv_21 },		/* ���ȹ��� */
    { UINT_0|D_DOT2,   msg_sys_pv_22 },		/* ����ˮ���� */
};
U16 get_sys_p_num(void)
{
    return ARRELE(msg_sys_p);
}

/* ģ��ѹ��ģ���� */
const MSG_ITEM_INFO msg_uint_p_comp[] = // ��Ӧcell[pcb_addr].cell_pv[x]��˳�򼰶���
{
    { UINT_A|D_DOT1,        msg_comp_pv_01 },				/* {����}            */
    { UINT_ST,              msg_comp_pv_02 },				/* {�����򿪶�}      */
    { UINT_ST,              msg_comp_pv_03 },				/* {�����򿪶�}      */
    { UINT_C|D_DOT1,        msg_comp_pv_04 },				/* {��Ƭ�¶�}        */
    { UINT_C|D_DOT1,        msg_comp_pv_05 },				/* {�����¶�}        */
    { UINT_C|D_DOT1,        msg_comp_pv_06 },				/* {�����¶�}        */
    { UINT_C|D_DOT1,        msg_comp_pv_07 },				/* {�����¶�}        */
    { UINT_C|D_DOT1,        msg_comp_pv_08 },				/* {ѹ���¶�}        */
    { UINT_C|D_DOT1,        msg_comp_pv_09 },				/* {���������¶�}    */
    { UINT_C|D_DOT1,        msg_comp_pv_10 },				/* {���������¶�}    */
    { UINT_PR|D_DOT1,       msg_comp_pv_11 },				/* {��ѹѹ��}        */
    { UINT_PR|D_DOT1,   	msg_comp_pv_12 },				/* {��ѹѹ��}        */
    { UINT_C|D_DE|D_DOT1,	msg_comp_pv_13 },				/* {����Ŀ�����}    */
    { UINT_C|D_DE|D_DOT1,   msg_comp_pv_14 },				/* {������ǰ����}    */
    { UINT_C|D_DE|D_DOT1,   msg_comp_pv_15 },				/* {����Ŀ�����}    */
    { UINT_C|D_DE|D_DOT1,   msg_comp_pv_16 },				/* {������ǰ����}    */
    { UINT_C|D_DOT1,    	msg_comp_pv_17 },				/* {����������¶�}  */
    { UINT_C|D_DOT1,    	msg_comp_pv_18 },				/* {�����������¶�}  */
    { UINT_C|D_DE|D_DOT1,   msg_comp_pv_19 },				/* {��˪���л����}  */
    { UINT_RS|D_DOT1,       msg_comp_pv_20 },				/* {��ƵĿ��ת��}    */
    { UINT_RS|D_DOT1,       msg_comp_pv_21 },				/* {��Ƶ��ǰת��}    */
    { UINT_V|D_DOT1,        msg_comp_pv_22 },				/* {��Ƶ�����ѹ}    */
    { UINT_A|D_DOT1,        msg_comp_pv_23 },				/* {��Ƶ�������}    */
    { UINT_C|D_DOT1,        msg_comp_pv_24 },				/* {��Ƶģ���¶�}    */
    { UINT_0,               msg_comp_pv_25 },				/* {��Ƶ��Ƶ��Ϣ}    */
    { UINT_A|D_DOT1,        msg_comp_pv_26 },				/* {��Ƶ�������}    */
    { UINT_0|D_DOT2,        msg_comp_pv_27 },				/* {��Ƶ�������}    */
    { UINT_RM,              msg_comp_pv_28 },				/* {���1Ŀ������}   */
    { UINT_RM,              msg_comp_pv_29 },				/* {���1��ǰת��}   */
    { UINT_RM,              msg_comp_pv_30 },				/* {���2Ŀ������}   */
    { UINT_RM,              msg_comp_pv_31 },				/* {���2��ǰת��}   */
    { UINT_0,               msg_comp_pv_32 },				/* {����} */
    { UINT_0,               msg_comp_pv_33 },				/* {����} */
    { UINT_C|D_DOT1,        msg_comp_pv_34 },				/* ��ý�����¶� */
    { UINT_C|D_DOT1,        msg_comp_pv_35 },				/* ��ýҺ���¶� */
    { UINT_0,               msg_comp_pv_36 },				/* ģ�鹦�� */
};
U16 get_comp_p_num(void)
{
    return ARRELE(msg_uint_p_comp);
}

/* ģ�鵥Ԫģ���� */
const MSG_ITEM_INFO msg_uint_p_cell[] =
{
    { UINT_C|D_DOT1,  msg_uint_pv_01 },     /* �廻��ˮ�¶� */
    { UINT_C|D_DOT1,  msg_uint_pv_02 },     /* �廻��ˮ�¶� */
    { UINT_C|D_DOT1,  msg_uint_pv_03 },     /* �ܳ�ˮ�¶� */
    { UINT_C|D_DOT1,  msg_uint_pv_04 },     /* �����¶� */
    { UINT_RM,        msg_uint_pv_05 },     /* ��Ƶˮ��Ŀ��ת�� */
    { UINT_LH,        msg_uint_pv_06 },     /* ��Ƶˮ�õ�ǰ���� */
    { UINT_PE|D_DOT1, msg_uint_pv_07 },     /* ��Ƶˮ�ÿ����ź� */
    { UINT_PE|D_DOT1, msg_uint_pv_08 },     /* ��Ƶˮ�÷����ź� */
    { UINT_0,         msg_uint_pv_09 },     /* ��Ƶˮ���쳣�� */
    { UINT_0|D_DOT2,  msg_uint_pv_10 },     /* ����궨���� */
    { UINT_0|D_DOT2,  msg_uint_pv_11 },     /* ���鵱ǰ���� */
    { UINT_0,         msg_uint_pv_12 },     /* ��Ƶˮ�õ�ǰ���� */
};
U16 get_unit_p_num(void)
{
    return ARRELE(msg_uint_p_cell);
}


/* ģ��ѹ������ */
const U16 msg_uint_i_comp[] = // ������
{
    msg_nothing,
    msg_nothing,
};

/* ģ�鵥Ԫ���� */
const U16 msg_uint_i_cell[]=/* ������ */
{
    msg_nothing,
    msg_nothing,
    msg_nothing,
    msg_nothing,
    msg_nothing,
    msg_nothing,
    msg_nothing,
    msg_nothing,
};

/* ϵͳ���� */
const U16 msg_sys_i[] =/* ������ */
{
    msg_nothing,
    msg_nothing,
    msg_nothing,
};

/**��Ƶ���ϲ���**************************************************/
const MSG_ITEM_INFO msg_invt_para[] =   // ��Ƶ���ϲ�����Ϣ
{
    {   0xFFFF,         msg_invt_fp_para_01,    },   /* ���һ�ι������� */
    {   UINT_H,         msg_invt_fp_para_02,    },   /* ���һ�ι���ʱ�ۼ�����ʱ�� */
    {   UINT_F|D_DOT2,  msg_invt_fp_para_03,    },   /* ���һ�ι���ʱ������Ƶ�� */
    {   UINT_F|D_DOT2,  msg_invt_fp_para_04,    },   /* ���һ�ι���ʱ�ĸ���Ƶ�� */
    {   UINT_A|D_DOT1,  msg_invt_fp_para_05,    },   /* ���һ�ι���ʱ��������� */
    {   UINT_V|D_DOT1,  msg_invt_fp_para_06,    },   /* ���һ�ι���ʱ�������ѹ */
    {   D_DOT2,         msg_invt_fp_para_07,    },   /* ���һ�ι���ʱ��������� */
    {   UINT_V|D_DOT1,  msg_invt_fp_para_08,    },   /* ���һ�ι���ʱ��ĸ�ߵ�ѹ */
    {   UINT_C|D_DOT1,  msg_invt_fp_para_09,    },   /* ���һ�ι���ʱ��������¶� */
    {   UINT_0,         msg_invt_fp_para_10,    },   /* ���һ�ι���ʱ����Ƶ��Ϣ */
    {   UINT_A,         msg_invt_fp_para_11,    },   /* ���һ�ι���ʱ��������� */
    {   UINT_V,         msg_invt_fp_para_12,    },   /* ���һ�ι���ʱ�������ѹ */
    {   0xFFFF,         msg_invt_fp_para_13,    },   /* �����ڶ��ι������� */
    {   UINT_H,         msg_invt_fp_para_02,    },   /* �����ڶ��ι���ʱ�ۼ�����ʱ�� */
    {   0xFFFF,         msg_invt_fp_para_15,    },   /* ���������ι������� */
    {   UINT_H,         msg_invt_fp_para_02,    },   /* ���������ι���ʱ�ۼ�����ʱ�� */
    {   0xFFFF,         msg_invt_fp_para_17,    },   /* �������Ĵι������� */
    {   UINT_H,         msg_invt_fp_para_02,    },   /* �������Ĵι���ʱ�ۼ�����ʱ�� */
    {   0xFFFF,         msg_invt_fp_para_19,    },   /* ��������ι������� */
    {   UINT_H,         msg_invt_fp_para_02,    },   /* ��������ι���ʱ�ۼ�����ʱ�� */
    {   UINT_H,         msg_invt_fp_para_21,    },   /* ����ʱ�ĵ�������ʱ�� */
};



/* dm603_common_XXX��ʾ�������� */

/**
 * ���� : dm603_common_disp_flash()
 * ���� : ˢ��
 *
 * ���� : --
 * ���� : --
 *
 * ��ע : --
 */
static void dm603_common_disp_flash(void)
{
    fg_flash_all = TRUE;
    fg_is_auto_flash_all = FALSE;   /* ���Զ�ˢ�� */
}

/**
 * ���� : dm603_common_disp_clr_line()
 * ���� : ˢ��
 *
 * ���� : --
 * ���� : --
 *
 * ��ע : --
 */
static void dm603_common_disp_clr_line(U08 row1, U08 row2, U08 row3, U08 row4, U08 *disp_buf)
{
    U08 row;

    if ((row = row1) != RNUL) disp_clr_line(row, disp_buf);
    if ((row = row2) != RNUL) disp_clr_line(row, disp_buf);
    if ((row = row3) != RNUL) disp_clr_line(row, disp_buf);
    if ((row = row4) != RNUL) disp_clr_line(row, disp_buf);
}

/**
 * ���� : dm603_common_toggle_byte_bit()
 * ���� : ȡ���ֽڶ�Ӧ��λֵ
 *
 * ���� : @pd �ֽ�
 *        @b  λ
 * ���� : --
 *
 * ��ע : --
 */
void dm603_common_toggle_byte_bit(U08 *pd, U08 b)
{
    if (GET_BIT(*pd, b))    CLR_BIT(*pd, b);
    else                    SET_BIT(*pd, b);
}

/**
 * ���� : dm603_common_disp_menu()
 * ���� : �˵�����ʾ
 *
 * ���� : @licon ���ͼ��
 *        @lstr  ����ַ���
 *        @ricon �Ҳ�ͼ��
 *        @rstr  �Ҳ��ַ���
 * ���� : --
 *
 * ��ע : --
 */
static void dm603_common_disp_menu(U16 licon, U16 lstr, U16 ricon, U16 rstr, U08 *disp_buf)
{
//  U16 tmp_str;
//  U16 tmp_icon;

//NOTE-CXW 1.���������µ�ת���,�����õ��ĵط��ѵ�ת
//NOTE-CXW 2.���48��ΪROW4,��ITEM_PER_PAGE*16���������ɲ������룩-------------
//NOTE-CXW 3.��ȷ�ϲ�ͳһ��ʾ������ͨ���ַ�����������ڶ�����������ء�ȷ�ϵ�-------------

//  tmp_str = lstr;
//  tmp_icon = licon;
//
//  lstr = rstr;
//  licon = ricon;
//
//  rstr = tmp_str;
//  ricon = tmp_icon;
// ---------------------------------------

    /* ���ͼ����ַ��� */
    if (licon)
    {
        //disp_bmp(licon, 48, 0, DCS_NORMAL, disp_buf);
        //disp_string(lstr, 48, 16, DCS_NORMAL, disp_buf);
        if ((void *)lstr != NULL)
        {
            disp_string(lstr, 48, 0, DCS_NORMAL, disp_buf);
        }
        else
        {
            disp_string_ptr8("    ", 48, 0, DCS_NORMAL, disp_buf);
        }
        disp_bmp(licon, 48, 32, DCS_NORMAL, disp_buf);
    }
    else
    {
        disp_string_ptr8("      ", 48, 0, DCS_NORMAL, disp_buf);
    }

    /* �Ҳ�ͼ����ַ��� */
    if (ricon)
    {
        //disp_bmp(ricon, 48, 128-16*2-16, DCS_NORMAL, disp_buf);
        //disp_string(rstr, 48, 128-16*2, DCS_NORMAL, disp_buf);
        if ((void *)rstr != NULL)
        {
            disp_string(rstr, 48, 80, DCS_NORMAL, disp_buf);
        }
        else
        {
            disp_string_ptr8("    ", 48, 80, DCS_NORMAL, disp_buf);
        }
        disp_bmp(ricon, 48, 112, DCS_NORMAL, disp_buf);
    }
    else
    {
        disp_string_ptr8("      ", 48, 80, DCS_NORMAL, disp_buf);
    }
}

/**
 * ������dm603_common_disp_select()
 * ���� : ���л�
 *
 * ���� : @item ��ǰ��ֵ
 *        @opt ���ϻ�����
 *        @min ��С����Ŀ
 *        @max �������Ŀ
 *        @cursor ���ѡ��
 * ���� : ��Ҫ�޸ĵ���ֵ
 *
 * ��ע : �ٹ��ֻ��3��ֵ:0->��ʾ���Ϸ���; 2->��ʾ���·���
 */
U08 dm603_common_disp_select(U08 item, U08 opt, U08 min, U08 max)
{
    switch (opt)
    {
        case MENU_UP :  /* ���Ϸ�ҳ */
            if (item-- <= min)
            {
                item = max - 1; /* ��ת����ĩ�� */
            }
            break;

        case MENU_DOWN :    /* ���·�ҳ */
            if (++item >= max)
            {
                item = min;     /* ��ת����һ�� */
            }
            break;
    }

    return item;
}

/**
 * ���� : dm603_face_menu_eeprom()
 * ���� : ��ʱʱ�估��ʷ���ϱ���
 *
 * ���� : @opt ��ʾ���ݴ�������
 *        MENU_WR 0 ��ʾ����
 *        MENU_RD 1 ��ʾ��ȡ
 *        MENU_IN 2 ��ʾ��ʼ��������
 *        @tbl ��ʾ��������
 * ���� : --
 *
 * ��ע : ��ÿ�δ����Ƕ��������ݽ��в���
 */
//void dm603_face_menu_eeprom(U08 opt, U08 *tbl)
//{
//  U08 i;
//  U08 max = 0;
//  U16 addr = 0;
//  U08 init_value = 0;
//
//  max  = sizeof(his_err_tbl)/sizeof(U08); /* �� */
//  addr = MEM_HIS_ERROR_ADDR;
//  if (tbl == dm603_time_tbl)
//  {
//      max = sizeof(dm603_time_tbl)/sizeof(U08);   /* �� */
//      addr = MEM_DM603_TIME_ADDR;
//  }
//  else if (tbl == &dm603_language)
//  {
//      max = 1;    /* �� */
//      addr = MEM_LANGUAGE_ADDR;
//  }
//
//  switch (opt)
//  {
//      case MENU_RD :  /* ��ȡ */
////            if (tbl == dm603_time_tbl)  debug_7[3]++;
////            if (rdeeprom(tbl, addr, max, 0) == IIC_NORMAL) break;
////            if (rdeeprom(tbl, addr, max, 0) == IIC_NORMAL) break;
//          if (eprom_rd_near_crc(tbl, addr, max)) break;    -LXH
//          if (eprom_rd_near_crc(tbl, addr, max)) break;
//          /* ���ζ�ȡ���������³�ʼ�� */
//          /* break; */
//
//      case MENU_IN :  /* ��ʼ�� */
//          if (tbl == his_err_tbl)     init_value = F_NO_ERR;
//          else                        init_value = 0;
//
//          for (i = 0; i < max; i++)
//          {
//              tbl[i] = init_value;
//          }
//          /* break; */
//
//      case MENU_WR :  /* ���� */
////            if (tbl == dm603_time_tbl)  debug_7[4]++;
////            wreeprom(tbl, addr, max);
//          eprom_wr_near_crc(tbl, addr, max);   -LXH
//          break;
//  }
//}


/**
 * ���� : dm603_common_disp_bound()
 * ���� : ȷ����ʾ��ķ�Χ
 *
 * ���� : @item ��ǰ��ֵ
 *        @min ��С����Ŀ
 *        @max �������Ŀ
 *        @cursor ���ѡ��
 *        @new_min �µ���С��ֵ
 *        @new_max �µ������ֵ
 * ���� : --
 *
 * ��ע : ��ȷ����Ҫ��ʾ�����Сֵ�����ֵ
 *        �ڷ�����ʾ������Ҫ��ȥ��С��ֵ
 */
U08 dm603_common_disp_bound(U08 item, U08 min, U08 max, U08 mod_page_max, U08 *new_min, U08 *new_max)
{
    U08 cursor = 0;

    /* ������� */
    if (min == 0 && max == 0)
    {
        *new_min = 0;
        *new_max = 0;
    }
    else
    {
        *new_min = (item / mod_page_max) * mod_page_max;
        *new_max = *new_min + mod_page_max;
        cursor = item - *new_min;

        if (*new_max > max) *new_max = max;
    }

    return cursor;
}


/**
 * ���� : dm603_common_disp_arrow()
 * ���� : ���ϻ����¼�ͷ
 *
 * ���� : @cursor ��ǰ��
 *        @min ��С��
 *        @max �����
 * ���� : --
 *
 * ��ע : �ٿ����ݲ�ͬ����λ������ʾ��ͷ
 *        ����ֵ��Ϊ��һ��������� ROW1 ROW3
 *        ����ֵ��Ϊ���ұ� 128-8
 */
void dm603_common_disp_arrow(U08 cursor, U08 min, U08 max, U08 *disp_buf)
{
    BOOL fg_up_arrow = FALSE;       /* �Ƿ���ʾ���ϼ�ͷ */
    BOOL fg_down_arrow = FALSE;     /* �Ƿ���ʾ���¼�ͷ��ʾ */

    if (min + 1 < max)              /* ֻ��һ��ʱ����ʾ��ͷ */
    {
        if (cursor == min)          /* ֻ��ʾ���¼�ͷ */
        {
            fg_down_arrow = TRUE;
        }
        else if (cursor == max-1)   /* ֻ��ʾ���ϼ�ͷ */
        {
            fg_up_arrow = TRUE;
        }
        else                        /* ͬʱ��ʾ���Ϻ����¼�ͷ */
        {
            fg_up_arrow = TRUE;
            fg_down_arrow = TRUE;
        }
    }

    if (fg_up_arrow)    disp_bmp(idx_bmp_up_8_16, ROW1, 128-8, DCS_NORMAL, disp_buf);
    else                disp_string_ptr8(" ", ROW1, 128-8, DCS_NORMAL, disp_buf);

    if (fg_down_arrow)  disp_bmp(idx_bmp_down_8_16, ROW3, 128-8, DCS_NORMAL, disp_buf);
    else                disp_string_ptr8(" ", ROW3, 128-8, DCS_NORMAL, disp_buf);
}

/* ״̬��ѯӦ���Ӻ����ӿ� */
/**
 * ���� : dm603_common_count_out()
 * ���� : �������������Чλ
 *
 * ���� : @ptb ������Чλ������ֵ
 *        @a1 ѹ�������Чλ�ĸ�����Ϣ
 *        @a2 ��Ԫ�����Чλ������Ϣ
 *        @comp ģ��ѹ������
 *                0xff -> ��ʾΪϵͳ��,��Ҫ��ֵcomp=1
 *                ��ff -> ��ʾΪģ���,ֱ��ʹ��ѹ��������
 * ���� : ��Чλ����
 *
 * ��ע : ���ɸ�����Ϣת����ǰ�������ݵ���Чλ
 *          ʹ��ʱ,ֱ��ȡptb�����е�ֵ���ü���
 */
U08 dm603_common_count_out(U08 *ptb, U16 a1, U16 a2, U08 comp)
{
    U08 i, j;
    U08 tmp = 0;
    U08 valid_out_num = 0;  /* ��1���������ֵ����Чλ���� */
    U08 cursor_comp = 0;    /* ѹ����Чλ���λ�� */
    U08 cursor_cell = 0;    /* ��Ԫ��Чλ���λ�� */
    BOOL fg_sys_pcb = FALSE;// ϵͳ��

    /* �Ƿ�ϵͳ��? */
    if (comp == 0xff)
    {
        fg_sys_pcb = TRUE;
        comp = 1;
    }

    /* �������λ�� */
    cursor_comp = 0;
    for (i = 0; i < RLY_BIT_NUM; i++)   //���һ̨ѹ�����������(ѹ������ͨ��������Ϊ2)
    {
        if (a1 & (0x0001 << i))     valid_out_num++;
    }

    /* ��Чλ���� */
    for (i=0; i<RLY_BIT_NUM; i++)       /* ��1���������ֵ */   //ѹ�����
    {
        if (a1 & (0x0001 << i))
        {
            for (j=0; j<comp; j++)
            {
                tmp = j*valid_out_num + cursor_comp;
                ptb[tmp] = i + (j * RLY_BIT_NUM);
                cursor_cell++;                      //�������
            }
            cursor_comp++;
        }
    }

    for (i=0; i<RLY_BIT_NUM; i++)       /* ��2���������ֵ */   //ģ�����
    {
        if (a2 & (0x0001 << i))
        {
            if (fg_sys_pcb)// ϵͳ�������ֽ����
            {
                ptb[cursor_cell++] = RLY_BIT_NUM + i;
            }
            else
            {
                ptb[cursor_cell++] = i + (MAX_COMP * RLY_BIT_NUM);
            }
        }
    }

    return cursor_cell;
}


/**
 * ���� : dm603_common_count_pv()
 * ���� : ����ģ����������Чλ
 *
 * ���� : @dest ��Чģ������������
 *        @src  ��Чģ������������
 * ���� : ��Чģ��������
 *
 * ��ע : --
 */
U08 dm603_common_count_pv(U08 *dest, I16 *src, U08 max_num)
{
    U08 i;
    U08 cursor = 0;

    /* ��Чģ���� */
    for (i = 0; i < max_num; i++)
    {
//        if(i==PV_SUC_ST_INDEX || i==PV_EXH_ST_INDEX)    continue;   // ���������¶Ȳ���ʾ

        if (*src++ != SENSOR_NOEXIST)   /* ��Чģ�����ж� */
        {
            dest[cursor] = i;
            cursor++;
        }
    }

    return cursor;
}

//static U08 dm603_common_count_invtinfo(U08 *dest, I16 *src)
//{
//  U08 i;
//  U08 cursor = 0;
//
//  /* ��Чģ���� */
//  for (i = 0; i < MAX_INVT_DIS_INFO; i++)
//  {
//      if (*src++ != SENSOR_NOEXIST)   /* ��Чģ�����ж� */
//      {
//          dest[cursor] = i;
//          cursor++;
//      }
//  }
//
//  return cursor;
//}
#ifdef USE_HMI_DISP
/**
 * ���� : dm603_common_disp_val()
 * ���� : ֵ��ʾ
 *
 * ���� : @val ֵ
 *        @row ��
 *        @col ��
 *        @sign ֵ��λ����
 *        @color �Ƿ񷴰�
 * ���� : --
 *
 * ��ע : ����ʾֵ����Ӧ�ĵ�λ
 *        ������ֵ����ռ��5��ASC�ַ�����λΪ3��ASC�ַ�����4������λ��
 *        �����Ϊ��С��ֵ����Чֵֻ��3λ����-99.9
 *        ��ֻ����ʾ����һλС����ֵ
 */
void dm603_common_disp_val(I16 val, U08 row, U08 col, U16 sign, U16 color, U08 *disp_buf)
{
    U16 neg = 0;
    U16 dot = 0;
    U16 dig = DNS_DIGIT5;
    U16 attrib = DCS_NORMAL | DTS_RIGHT;

    /* ��λ�ַ������� */
    const U16 msg_uint[] =
    {
        dm603_disp_val_UINT_0 , /* �� */
        dm603_disp_val_UINT_C ,   /* �¶�() */
        dm603_disp_val_UINT_A ,   /* ����(A) */
        dm603_disp_val_UINT_PR,   /* ѹ��(bar) */
        dm603_disp_val_UINT_PE,   /* �ٷֱ�(%) */
        dm603_disp_val_UINT_F ,   /* Ƶ��(HZ) */
        dm603_disp_val_UINT_RS,   /* ת��(rps) */
        dm603_disp_val_UINT_RM,   /* ת��(rpm) */
        dm603_disp_val_UINT_S ,   /* �� */
        dm603_disp_val_UINT_M ,   /* �� */
        dm603_disp_val_UINT_H ,   /* ʱ */
        dm603_disp_val_UINT_D ,   /* �� */
        dm603_disp_val_UINT_ST,   /* �� */
        dm603_disp_val_UINT_V,    /* ��ѹ(V) */
        dm603_disp_val_UINT_FL,   /* ˮ����(L/H) */
    };

    /* �ж��Ƿ�Ϊ����? */
    if (val < 0) neg = 1;
    if (neg) dig--;

    if (sign & D_DOTMASK) dot = ((sign & D_DOTMASK) == D_DOT1) ? DNS_FLOAT1 : DNS_FLOAT2;
    if (dot) dig--;
    attrib |= dig | dot;
//  switch (sign & UINT_MASK)
//  {
//      case UINT_0  : /* �� */
//
//      case UINT_PE : /* �ٷֱ�(%) */
//      case UINT_RS : /* ת��(rps) */
//      case UINT_RM : /* ת��(rpm) */
//      case UINT_S  : /* �� */
//      case UINT_M  : /* �� */
//      case UINT_H  : /* ʱ */
//      case UINT_D  : /* �� */
//      case UINT_ST : /* �� */
//          //if (neg) dig--;
//          attrib |= dig;
//          break;
//
//      case UINT_PR : /* ѹ��(bar) */
//      case UINT_C  : /* �¶�() */
//      case UINT_A  : /* ����(A) */
//      case UINT_F  : /* Ƶ��(HZ) */
//          //if (neg) dig--;
//          if (dot) dig--;
//          attrib |= dig | dot;
//          break;
//
//      default :
//          break;
//  }

    /* ��ʾֵ�͵�λ */
    //if (!SensorIsError(val)) disp_value_I16(val, row, col, attrib | color, disp_buf);
    //else                   disp_string(dm603_disp_val_UINT_ERROR, row, col, color, disp_buf);
    //disp_string(msg_uint[(U08)((sign&UINT_MASK) >> 4)], row, col+5*8, DCS_NORMAL, disp_buf);
    col = col - 3*8;
    if (!SensorIsError(val)) disp_value_I16(val, row, col, attrib | color, disp_buf);
    else                     disp_string(dm603_disp_val_UINT_ERROR, row, col-5*8, color, disp_buf);
    disp_string(msg_uint[(U08)((sign&UINT_MASK) >> 4)], row, col, DCS_NORMAL, disp_buf);
}

/**
 * ���� : dm603_face_open()
 * ���� : ��������
 *
 * ���� : --
 * ���� : --
 *
 * ��ע : --
 */
#define     DM603A_OPEN_TIME    6
static void dm603_face_open(U08 *disp_buf)
{
    U08 i;
//  U16 tt[MAX_HMI];
    U16 attrib = 0;

    const U16 msg_language[] = { dm603_face_open_language_zh, dm603_face_open_language_en, };

    switch (dm603_menu[hmi_t_num])
    {
        case DM603_MENU_CREATE :
            dm603_power_time[hmi_t_num] = 0;
            dm603_menu[hmi_t_num] = DM603_MENU_PROC;
            disp_clr_screen(disp_buf);
            dm603_common_disp_flash();
            //disp_snd_100ms(); /* ������ */
            /* break; */

        case DM603_MENU_PROC :
//          if (fg_test != 2)
//          {
                /* ǰ2��ˢ�£���ֹˢ�±�־��ʧ */
                if (dm603_power_time[hmi_t_num] < 1) dm603_common_disp_flash();

                if (dm603_power_time[hmi_t_num] > DM603A_OPEN_TIME)
                {
                    dm603_face[hmi_t_num] = FACE_MAIN;
                    return;
                }
                /* ���������� */
                if (Main_Key[hmi_t_num] == DM603_KEY_FUNC)
                {
                    dm603_face[hmi_t_num] = FACE_MAIN;
                    return;
                }
                /* ����ʹ���������� */
                if (Main_Key[hmi_t_num] == DM603_KEY_OK_FUNC)
                {
                    dm603_push_face_status();   /* �������״̬ */
                    dm603_set_nextface_status(FACE_LIMIT);  // ȷ��������ȷ��Ҫ��ת�Ľ���
                    dm603_face[hmi_t_num] = FACE_PASS;
                    dm603_PassNeed[hmi_t_num] = PASS_LMT;

                    return;
                }

                /* ������Գ��� */
                if(Main_Key[hmi_t_num] == DM603_KEY_ONOFF_3S)
                {
                    test_allow = TRUE;
                }
                if (test_allow == TRUE && Main_Key[hmi_t_num] == DM603_KEY_ONOFF_UP)
                {
                    punp_fg = TRUE;
                    disp_clr_screen(disp_buf);      //����ǰ������
                    dm603_face[hmi_t_num] = FACE_PUNP_INIT;
                }


                /* �л����� */
                if (Main_Key[hmi_t_num] == DM603_KEY_UP || Main_Key[hmi_t_num] == DM603_KEY_DOWN)
                {
                    dm603_common_disp_flash();
                    if (dm603_language == LANGUE_ENGLISH)   dm603_language = LANGUE_CHINESE;
                    else                                        dm603_language = LANGUE_ENGLISH;

//                  dm603_face_menu_eeprom(MENU_WR, &dm603_language);
                    DataAccess_Language(DATA_WRITE);
                }
                disp_set_langue(dm603_language);
//          }
            /* break; */

        case DM603_MENU_PAINT :
            if (fg_disp_back[hmi_t_num])
            {
//              if (fg_test != 2)
//              {
                    for (i = 0; i < 2; i++)
                    {
                        attrib = DCS_NORMAL;
                        if (i == dm603_language - 1)    attrib = DCS_REVERSE;
                        disp_string(msg_language[i], ROW1, i*16, attrib | DCS_FLASH, disp_buf);
                    }

                    disp_value_U08(DM603A_OPEN_TIME-dm603_power_time[hmi_t_num], 0, 128-8, \
                                  ((DM603A_OPEN_TIME <= 9) ? DNS_DIGIT1 : DNS_DIGIT2) | DCS_REVERSE | DCS_FLASH, disp_buf);
                    disp_string(dm603_face_open_welcome, 16, 0, DTS_CENTER | DCS_FLASH | DTS_CLR_ALL, disp_buf);

                    disp_string(dm603_face_open_welcome, 16, 0, DTS_CENTER | DCS_FLASH | DTS_CLR_ALL, disp_buf);

                    if(fg_seep_crc_err!=0)
                    {
                        disp_string_ptr8("CRC", 32, 0, DCS_REVERSE, disp_buf);
                        disp_value_U16(fg_seep_crc_iderr, 32, 4*8,DNS_DIGIT2 | DCS_REVERSE, disp_buf);
                    }
                    if(fg_seep_overflow!=0)
                    {
                        disp_string_ptr8("OV", 48, 0, DCS_REVERSE, disp_buf);
                        disp_value_U16(fg_seep_overflow_iderr, 48, 4*8,DNS_DIGIT2 | DCS_REVERSE, disp_buf);
                        disp_value_U16(fg_seep_overflow_item, 48, 8*8,DNS_DIGIT2 | DCS_REVERSE, disp_buf);
                    }
                    //disp_value_U08(debug_buf[0], 32, 4*8,DNS_DIGIT3 | DCS_REVERSE | DCS_FLASH, disp_buf);
                    //disp_value_U08(debug_buf[1], 32, 8*8,DNS_DIGIT3 | DCS_REVERSE | DCS_FLASH, disp_buf);
                    //disp_value_U08(debug_buf[2], 32, 12*8,DNS_DIGIT3 | DCS_REVERSE | DCS_FLASH, disp_buf);
                    //disp_value_U08(debug_buf[3], 48, 4*8,DNS_DIGIT3 | DCS_REVERSE | DCS_FLASH, disp_buf);
                    //disp_value_U08(debug_buf[4], 48, 8*8,DNS_DIGIT3 | DCS_REVERSE | DCS_FLASH, disp_buf);
                    //disp_value_U08(debug_buf[5], 48, 12*8,DNS_DIGIT3 | DCS_REVERSE | DCS_FLASH, disp_buf);

                    #ifdef DEBUG
                        disp_string_ptr8("DEBUG", ROW4, 40, DCS_NORMAL, disp_buf);
                    #endif
//              }
//              else
//              {
//                  disp_version_f(ROW1, 0, DCS_NORMAL | DCS_REVERSE, disp_buf);
//                  dm603_common_disp_clr_line(ROW3, ROW4, RNUL, RNUL, disp_buf);
//              }
            }

            /* ��ʾ���� */
            #ifdef USE_DISP_DBGVER
            {
                disp_string_ptr8(DEBUG_VER, ROW3, 0, DTS_CENTER, disp_buf);
            }
            #endif

            break;
    }
}

//#define OUTPUT_NUM    16  // �������     //NOTE-CXW ���µײ��Ƿ�����صĺ궨�壬�������滻
//#define ANALOG_NUM    16  // ģ��������
/* pcb_numģ����� */
static void DispStatus_Analog(U08 Pcb_num, U08 Page_now, const U08 *Tbl, U08 *disp_buf)
{
    U08 x, tmp,col;
    U16 attrib = DCS_NORMAL;

    // ״̬����
    if (Pcb_num == MAX_PCB)
    {
        disp_string(dm603_face_menu_status_sys, ROW1, 0,   DCS_NORMAL, disp_buf);
        disp_string(dm603_face_menu_status_pv,  ROW1, 4*8, DCS_NORMAL, disp_buf);
    }
    else    // ģ��
    {
        disp_value_U08(Pcb_num, ROW1, 0, DCS_NORMAL | DNS_DIGIT2 | DNS_ZERO, disp_buf);
        disp_string_ptr8("#", ROW1, 2*8, DCS_NORMAL, disp_buf);
        disp_string(dm603_face_menu_status_uint, ROW1, 3*8, DCS_NORMAL, disp_buf);
        disp_string(dm603_face_menu_status_pv,   ROW1, 7*8, DCS_NORMAL, disp_buf);
    }

    x = Tbl[Page_now];
    if (Pcb_num == MAX_PCB)
    {

        disp_string(msg_sys_p[x].msg, ROW2, 0, attrib, disp_buf);
        attrib = msg_sys_p[x].attr;
        //����attrib = dm603_common_get_val_unit(msg_sys_p[x].msg);
        dm603_common_disp_val(cell[Pcb_num].cell_pv[x], ROW3, COL_END - COL_ASC, attrib, \
                              DCS_NORMAL, disp_buf);
    }
    else    // ģ��
    {

        if(x >= CELL_PV_BEGIN)  // ��Ԫģ����
        {
            disp_string(msg_uint_p_cell[x-CELL_PV_BEGIN].msg, ROW2, 0, DCS_NORMAL, disp_buf);
            attrib = msg_uint_p_cell[x-CELL_PV_BEGIN].attr;
            //����attrib = dm603_common_get_val_unit(msg_uint_p_cell[x-CELL_PV_BEGIN].msg);
            dm603_common_disp_val(cell[Pcb_num].cell_pv[x], ROW3, COL_END - COL_ASC, attrib, \
                                  DCS_NORMAL, disp_buf);
        }
        else    // ��̨ѹ��ģ����
        {
            tmp = x % COMP_PV_NUM;
            col = 0;
            if(val_comp_num>1)
            {
                disp_value_U08((x/COMP_PV_NUM+1), ROW2, col, DCS_NORMAL|DNS_DIGIT1, disp_buf);
                col = 1*8;
                disp_string_ptr8("#", ROW2, col, DCS_NORMAL, disp_buf);
                col = 2*8;
            }
            disp_string(msg_uint_p_comp[tmp].msg, ROW2, col, DCS_NORMAL, disp_buf);

            attrib = msg_uint_p_comp[tmp].attr;
            //����attrib = dm603_common_get_val_unit(msg_uint_p_comp[tmp].msg);
            dm603_common_disp_val(cell[Pcb_num].cell_pv[x], ROW3, COL_END - COL_ASC, attrib, \
                                      DCS_NORMAL, disp_buf);
        }
    }
}

#define MAX_OUT_DISP    4   // ÿҳ�����ʾ�������
static void DispStatus_Output(U08 Pcb_num, U08 Page_now, U08 Item_num, const U08 *Tbl, U08 *disp_buf)
{
    U08 i, x, tmp;
    U16 attrib = DCS_NORMAL;

    U08 out_index;  // ��ҳ��ʾ�������ʼ���
    U08 max;        // ��ҳ��ʾ���������
    U08 row, col, col2;

    // ״̬����
    if (Pcb_num == MAX_PCB)
    {
        disp_string(dm603_face_menu_status_sys, ROW1, 0,   DCS_NORMAL, disp_buf);
        disp_string(dm603_face_menu_status_out, ROW1, 4*8, DCS_NORMAL, disp_buf);
    }
    else    // ģ��
    {
        disp_value_U08(Pcb_num, ROW1, 0, DCS_NORMAL | DNS_DIGIT2 | DNS_ZERO, disp_buf);
        disp_string_ptr8("#", ROW1, 2*8, DCS_NORMAL, disp_buf);
        disp_string(dm603_face_menu_status_uint, ROW1, 3*8, DCS_NORMAL, disp_buf);
        disp_string(dm603_face_menu_status_out,  ROW1, 7*8, DCS_NORMAL, disp_buf);
    }

    max = MAX_OUT_DISP;
    out_index = Page_now * MAX_OUT_DISP;        // ��ҳ����Ŀ�ʼ���
    if ((out_index + MAX_OUT_DISP) > Item_num)  // �ѵ�������һҳ
    {
        max = Item_num - out_index;             // ��ҳʵ���������
    }

    for(i=0; i<max; i++)
    {
        x = Tbl[out_index+i];   /* ȡ������� */

        /* �ڶ��л������ */
        if (i < 2)  row = ROW2;
        else        row = ROW3;

        col = (i&0x01)*8*8;

        /* ������ʾ */
        attrib = DCS_NORMAL;
        if (get_output(Pcb_num, x)) attrib = DCS_REVERSE;

        if (Pcb_num == MAX_PCB)
        {
            disp_string(msg_sys_o[x], row, col, attrib, disp_buf);
        }
        else
        {
            /* ѹ����� */
            if (x < (MAX_COMP * RLY_BIT_NUM))
            {
                tmp = x % RLY_BIT_NUM;
                col2 = col;
                if(val_comp_num>1)
                {
                    disp_value_U08((x/RLY_BIT_NUM+1), row, col2, DCS_NORMAL|DNS_DIGIT1, disp_buf);
                    col2 = col+1*8;
                    disp_string_ptr8("#", row, col2, DCS_NORMAL, disp_buf);
                    col2 = col+2*8;
                }
                disp_string(msg_uint_o_comp[tmp], row, col2, attrib|DCS_FLASH, disp_buf);
            }
            else    /* ��Ԫ��� */
            {
                tmp = x - (MAX_COMP * RLY_BIT_NUM);
                disp_string(msg_uint_o_cell[tmp], row, col, attrib|DCS_FLASH, disp_buf);
            }
        }

    }

}

/**
 * ���� : dm603_face_menu_status()
 * ���� : ģ��״̬
 *
 * ���� : --
 * ���� : --
 *
 * ��ע : --
 */
static void dm603_face_menu_status(U08 *disp_buf)
{
    U08 s_o_max;                                /* ���������� */
    U08 s_p_max;                                /* ���ģ�������� */
    U08 s_page_max;                             /* ���ҳ�� */
    U08 cell_o_tbl[MAX_RELAY * RLY_BIT_NUM];    /* ���״ֵ̬ */
    U08 cell_p_tbl[MAX_PV];                     /* ģ��״ֵ̬ */

    static U08 page_index;                      /* ���в�ѯҳ���� */
    static U08 s_pcb = 0;                       /* ģ����� */

    BOOL fg_mod_cannot_use = FALSE;
    static BOOL fg_com_err_bak = FALSE;

    switch (dm603_menu[hmi_t_num])
    {
        case DM603_MENU_CREATE :
            dm603_menu[hmi_t_num] = DM603_MENU_PROC;
            key_delay_time[hmi_t_num]=0;
            disp_clr_screen(disp_buf);
            dm603_common_disp_flash();

            s_pcb = 0;
            page_index = 0;

            /* �����Чģ����ֵ */
//          dm603_common_clr_pv(s_pcb);
            /* break; */

        case DM603_MENU_PROC :
            /* ��Ч���ݳ�ʼ�� */
            {
                if (s_pcb == MAX_PCB)
                {
                    s_o_max = dm603_common_count_out(cell_o_tbl, assist_info[3], assist_info[4], 0xff);
                }
                else
                {
                    s_o_max = dm603_common_count_out(cell_o_tbl, assist_info[1], assist_info[2], m_val_comp_num(s_pcb));
                }

                s_p_max = dm603_common_count_pv (cell_p_tbl, &cell[s_pcb].cell_pv[0], MAX_PV);
                s_page_max = s_p_max + _DIVUCARRY(s_o_max, 4);      //����ģ��ҳ�������ÿҳ��ʾ4��
            }

            if (s_pcb != MAX_PCB)
            {
                if (GET_BIT(slave_commu_error, s_pcb)
                ||  !CELL_Is_used(s_pcb)
                )
                {
                    fg_mod_cannot_use = TRUE;
                }
            }

            if (fg_com_err_bak != fg_mod_cannot_use)
            {
                dm603_common_disp_flash();
                disp_clr_screen(disp_buf);
                fg_com_err_bak = fg_mod_cannot_use;
            }
            if(key_delay_time[hmi_t_num]>=KEY_DELAY)
            {
                switch(Main_Key[hmi_t_num])
                {
                    case DM603_KEY_FUNC:
                        dm603_pop_face_status();    // �ָ��ϲ����״̬
                        return;
//                      break;

                    case DM603_KEY_OK:      // �л�ģ��
                        {
                            if (s_pcb == MAX_PCB)               s_pcb = 0;
                            else if (++s_pcb >= val_pcb_num)    s_pcb = MAX_PCB;

                            page_index = 0;
                            disp_clr_screen(disp_buf);  // �л�ģ����������أ��Ա�ȷ��ģ��ͨѶ������ʾ
                            dm603_common_disp_flash();
                            return;
                        }
//                      break;

                    case DM603_KEY_UP:      // ���Ϸ�ҳ
                        if (!fg_mod_cannot_use)
                        {
                            if (page_index-- <= 0)  page_index = s_page_max - 1;
                        }
                        break;

                    case DM603_KEY_DOWN:    // ���·�ҳ
                        if (!fg_mod_cannot_use)
                        {
                            if (++page_index >= s_page_max)   page_index = 0;
                        }
                        break;

                    case DM603_KEY_OK_FUNC:
                        dm603_push_face_status();   /* �������״̬ */
                        dm603_face[hmi_t_num] = FACE_INVT_EXCEPT_INFO;
                        return;
//                      break;

                    default:
                        break;
                }
            }
        case DM603_MENU_PAINT :
            if (Main_Key[hmi_t_num] != KEY_NULL)
            {
                disp_clr_screen(disp_buf);
                dm603_common_disp_flash();
            }
            if (fg_mod_cannot_use)      /* �����ã�����ʾ */
            {
                disp_value_U08(s_pcb, ROW2, 0, DCS_NORMAL | DNS_DIGIT2 | DNS_ZERO, disp_buf);
                disp_string(msg_mod_cannot_use, ROW2, 16, DCS_NORMAL, disp_buf);
            }
            else                /* ��ͨѶ����ʱ����ʾ״̬ */
            {
                if (page_index < s_p_max)   // ģ����
                {
                    DispStatus_Analog(s_pcb, page_index, cell_p_tbl, disp_buf);                     // ��ʾ��ǰҳģ����
                }
                else    // ���
                {
                    U08 out_page_index = page_index-s_p_max;
                    DispStatus_Output(s_pcb, out_page_index, s_o_max, cell_o_tbl, disp_buf);    // ��ʾ��ǰҳ���
                }
                /* ��ͷ */
                dm603_common_disp_arrow(page_index, 0, s_page_max, disp_buf);
            }

            /* �˵��� */
            dm603_common_disp_menu(BMP_FN, dm603_face_return, BMP_OK, dm603_face_menu_status_chg, disp_buf);
            break;
    }
}

/**
 * ���� : dm603_face_menu_clock()
 * ���� : ʱ�Ӳ�ѯ
 *
 * ���� : --
 * ���� : --
 *
 * ��ע : --
 */
static void dm603_face_menu_clock(U08 *disp_buf)
{
//  U08 i = 0;
//  U16 attrib = 0;

//  static U08 cursor = 0;  /* �� */
//  static RTC s_clock; /* ʱ�ӱ��� */

    switch (dm603_menu[hmi_t_num])
    {
        case DM603_MENU_CREATE :
            dm603_menu[hmi_t_num] = DM603_MENU_PROC;
            key_delay_time[hmi_t_num]=0;
            dm603_common_disp_flash();
            /* break; */

        case DM603_MENU_PROC :
            if(key_delay_time[hmi_t_num]>=KEY_DELAY)
            {
                if (Main_Key[hmi_t_num] == DM603_KEY_OK)    /* ���� */
                {
                    dm603_pop_face_status();
                    return;
                }
            }
            /* break; */

        case DM603_MENU_PAINT :
            disp_set_clk(disp_buf);

            /* ��ֹ���� */
            cnt_no_operate[hmi_t_num] = 0;
            cnt_no_operate_sec[hmi_t_num] = 0;

            /* �˵��� */
            disp_string(dm603_face_menu_clock_info, ROW1, 0, DCS_NORMAL | DTS_CENTER, disp_buf);
            dm603_common_disp_menu(BMP_FN, dm603_face_menu_clock_chg, BMP_OK, dm603_face_ok, disp_buf);
            break;
    }
}
#ifdef COM_FCU_FBUS
/*******************************************************************************
 �� ��: dm603_face_menu_fcu()
 �� ��: ������Ϣ
 �� ��: disp_buf--;
 �� ��:
 �� ��: ��
*******************************************************************************/
const U16 fcu_status[] =
{
    dm603_face_menu_fcu_sta_00, // δ����
    dm603_face_menu_fcu_sta_01, // ����̬
    dm603_face_menu_fcu_sta_02, // ����̬
    dm603_face_menu_fcu_sta_03, // ������
    dm603_face_menu_fcu_sta_04, // ������
    dm603_face_menu_fcu_sta_05, // δ����
};
const U16 fcu_mode[] =
{
    dm603_nothing,
    dm603_face_menu_fcu_mode_01,    // ����
    dm603_face_menu_fcu_mode_02,    // ����
    dm603_face_menu_fcu_mode_03,    // ͨ��
    dm603_face_menu_fcu_mode_04,    // ��ů
    dm603_face_menu_fcu_mode_05,    // δ֪
};
static void dm603_face_menu_fcu(U08 *disp_buf)
{
    static I08 num = 0;

    switch (dm603_menu[hmi_t_num])
    {
        case DM603_MENU_CREATE :
            dm603_menu[hmi_t_num] = DM603_MENU_PROC;
            key_delay_time[hmi_t_num]=0;
            dm603_common_disp_flash();

            num = 0;
            /* break; */

        case DM603_MENU_PROC :
            if (key_delay_time[hmi_t_num] >= KEY_DELAY)
            {
                if (Main_Key[hmi_t_num] == DM603_KEY_FUNC)  /* ���� */
                {
                    dm603_pop_face_status();
                    return;
                }
                else if (Main_Key[hmi_t_num] == DM603_KEY_DOWN)
                {
                    num++;
                    if (num >= val_fcu_num)
                    {
                        num = 0;
                    }
                    dm603_common_disp_flash();
                }
                else if (Main_Key[hmi_t_num] == DM603_KEY_UP)
                {
                    num--;
                    if (num < 0)
                    {
                        num = val_fcu_num-1;
                    }
                    dm603_common_disp_flash();
                }
            }

            /* break; */

        case DM603_MENU_PAINT :
            {
                U08 row = ROW1;
                U08 status_max = ARRELE(fcu_status);
                U08 status_idx = MIN(fcu_state_get(num), status_max-1);
                U08 mode_max = ARRELE(fcu_mode);
                U08 mode_idx = MIN(fcu_sta_mode(num), mode_max-1);

                disp_value_U08(num+1, row, 0, DNS_DIGIT2|DNS_ZERO, disp_buf);
                disp_string(dm603_face_menu_fcu_title, row, 2*8, DCS_NORMAL, disp_buf);
                disp_string(fcu_status[status_idx], row, 8*8, DCS_NORMAL, disp_buf);

                row = ROW2;
                disp_string(dm603_face_menu_fcu_now, row, 0, DCS_NORMAL, disp_buf);
                disp_value_I16(fcu_temp_indoor(num), row,  9*8, DNS_DIGIT4|DNS_FLOAT1, disp_buf);


                row = ROW3;
                disp_string(dm603_face_menu_fcu_mode, row, 0, DCS_NORMAL, disp_buf);
                disp_string(fcu_mode[mode_idx], row, 5*8, DCS_NORMAL, disp_buf);
            }


            /* ��ͷ */
            dm603_common_disp_arrow(num, 0, val_fcu_num, disp_buf);
            /* �˵��� */
            dm603_common_disp_menu(BMP_FN, dm603_face_return, BMP_OK, dm603_face_ok, disp_buf);
            break;
    }
}
#endif
/*******************************************************************************
 �� ��: invt_code_to_version()
 �� ��: ��Ƶ���汾��ת��
 �� ��: ver--�汾���ַ���;
        func_code--������;
        ver_code--�汾��;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
#define INVT_VER_NUM 21
void invt_code_to_version(U08 *ver, U16 func_code, U16 ver_code)
{
    if (func_code==0 && ver_code==0
    ||  func_code==(U16)INVT_PARA_VALUE_NULL && ver_code==(U16)INVT_PARA_VALUE_NULL
        )
    {
        MEMCLR(ver, INVT_VER_NUM);
        ver[0] = '0';
    }
    else
    {
        char symbol[] = {'X', 'K', 'F', 'T', 'Q'};      /* ϴ�ӡ��յ���������ͨ�á����� */

        MEMCPY(ver, "X1.VF$$$$.$##.V100###", INVT_VER_NUM); /* ����X1.VF250A.F01.V100A00 */

        /* �汾��ת�� */
        {
            ver[5] = GET_DIGIT(func_code, 4)+'0';
            ver[6] = GET_DIGIT(func_code, 3)+'0';
            ver[7] = GET_DIGIT(func_code, 2)+'0';
            ver[8] = GET_DIGIT(func_code, 1)-1+'A';
            ver[10] = symbol[GET_DIGIT(func_code, 0)-1];

            ver[11] = GET_DIGIT(ver_code, 4)+'0';
            ver[12] = GET_DIGIT(ver_code, 3)+'0';
            if (GET_DIGIT(ver_code, 2))
            {
                ver[18] = GET_DIGIT(ver_code, 2)-1+'A';
                ver[19] = GET_DIGIT(ver_code, 1)+'0';
                ver[20] = GET_DIGIT(ver_code, 0)+'0';
            }
            else
            {
                ver[18] = GET_DIGIT(ver_code, 1)-1+'A';
                ver[19] = GET_DIGIT(ver_code, 0)+'0';
                ver[20] = 0;
            }
        }
    }
}

/*******************************************************************************
 �� ��: dm603_disp_version()
 �� ��: ��ʾ�汾
 �� ��: disp_buf--����;
        row--��;
        p_ver--�汾�ַ���;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void dm603_disp_version(U08 *disp_buf, U08 row, U08 *p_ver)
{
    U08 CODE_STR[MAX_VER_LINE+1];

    //��ʾ��һ��
    ARRCLR(CODE_STR);
    MEMCPY(CODE_STR, p_ver, MAX_VER_LINE1);
    disp_string_ptr8(CODE_STR, row, 0, DCS_NORMAL, disp_buf);

    //��ʾ�ڶ���
    ARRCLR(CODE_STR);
    MEMCPY(CODE_STR, &p_ver[MAX_VER_LINE1], MAX_VER_LINE2(p_ver));
    disp_string_ptr8(CODE_STR, row+16, 0, DCS_NORMAL, disp_buf);
}
/**
 * ���� : dm603_face_menu_version()
 * ���� : �汾��ѯ
 *
 * ���� : --
 * ���� : --
 *
 * ��ע : --
 */
static void dm603_face_menu_version(U08 *disp_buf)
{
    #define MAX_VER_LINE        15  /* һ������ַ��� */
    #define MAX_VER_LINE1       14  /* ��һ���ַ��� */
    #define MAX_VER_LINE2(p)    MIN(STRLEN(p)-MAX_VER_LINE1, MAX_VER_LINE)

    U08 CODE_STR[MAX_VER_LINE+1];
    static I08 cursor;  /* �� */


    enum VERSION_ITEM   /* ���ڴ���汾�ŵ����� */
    {
        VER_IN_0,           /* �ڻ���0 */
        VER_OUT_0,          /* �����0 */
        VER_IN_1,           /* �ڻ���1 */
        VER_OUT_1,          /* �����1 */
        VER_IN_2,           /* �ڻ���2 */
        VER_OUT_2,          /* �����2 */
        VER_IN_3,           /* �ڻ���3 */
        VER_OUT_3,          /* �����3 */
        VER_IN_4,           /* �ڻ���4 */
        VER_OUT_4,          /* �����4 */
        VER_IN_5,           /* �ڻ���5 */
        VER_OUT_5,          /* �����5 */
        VER_IN_6,           /* �ڻ���6 */
        VER_OUT_6,          /* �����6 */
        VER_IN_7,           /* �ڻ���7 */
        VER_OUT_7,          /* �����7 */
        VER_FRONT,          /* ǰ�� */
        VER_INVT_CM,        /* ��Ƶѹ�� */
        VER_INVT_FAN1,      /* EC���1 */
        VER_INVT_FAN2,      /* EC���2 */
        VER_MAX
    };

    U08 disp_ver_max = VER_MAX;

    /* ���ڼ�ͷָʾ */
    if (!val_inverter_used)                     disp_ver_max--;/* ��Ƶѹ�� */
    if (val_freq_fan_set != FREQ_FAN_SET_EC)    disp_ver_max--;/* EC���1 */
    if (val_freq_fan_set != FREQ_FAN_SET_EC)    disp_ver_max--;/* EC���2 */

    switch (dm603_menu[hmi_t_num])
    {
        case DM603_MENU_CREATE :
            dm603_menu[hmi_t_num] = DM603_MENU_PROC;
            key_delay_time[hmi_t_num]=0;
            dm603_common_disp_flash();
            cursor = 0; /* �� */
            /* break; */

        case DM603_MENU_PROC :
            /* ���������� */
            if(key_delay_time[hmi_t_num]>=KEY_DELAY)
            {
                if (Main_Key[hmi_t_num] == DM603_KEY_FUNC)
                {
                    dm603_pop_face_status();    /* �ָ��ϲ����״̬ */
                    return;
                }

                /* ���ϻ������л� */
                if (Main_Key[hmi_t_num] == DM603_KEY_DOWN)
                {
                    if (++cursor >= VER_MAX)   cursor = 0;

                    if (cursor >= VER_IN_1 && cursor <= VER_OUT_7)
                    {
                        if (cursor/2 >= val_pcb_num)
                        {
                            cursor = VER_FRONT;
                        }

//                        if (cursor > VER_IN_1+val_pcb_num-2)
//                        {
//                            cursor = VER_IN_7+1;          /* �ӿ�ʼ */
//                        }
                    }

                    /* ����û�еİ汾 */
                    switch (cursor)         /* ��С�����ж� */
                    {
                        case VER_INVT_CM:
                            if (!val_inverter_used)
                            {
                                if (++cursor >= VER_MAX)
                                {
                                    cursor = 0;
                                    break;
                                }
                            }
                            else
                            {
                                break;
                            }
                            //break;
                        case VER_INVT_FAN1:
                            if (val_freq_fan_set != FREQ_FAN_SET_EC)
                            {
                                if (++cursor >= VER_MAX)
                                {
                                    cursor = 0;
                                    break;
                                }
                            }
                            else
                            {
                                break;
                            }
                            //break;
                        case VER_INVT_FAN2:
                            if (val_freq_fan_set != FREQ_FAN_SET_EC)
                            {
                                if (++cursor >= VER_MAX)
                                {
                                    cursor = 0;
                                    break;
                                }
                            }
                            else
                            {
                                break;
                            }
                            //break;
                        default:
                            break;
                    }
                    dm603_common_disp_flash();
                    disp_clr_screen(disp_buf);
                }
                if (Main_Key[hmi_t_num] == DM603_KEY_UP)
                {
                    if (--cursor < 0)    cursor = VER_MAX-1;
                    /* ����û�еİ汾 */
                    switch (cursor)         /* �Ӵ�С�ж� */
                    {
                        case VER_INVT_FAN2:
                            if (val_freq_fan_set != FREQ_FAN_SET_EC)
                            {
                                if (--cursor < 0)
                                {
                                    cursor = VER_MAX-1;
                                    break;
                                }
                            }
                            else
                            {
                                break;
                            }
                            //break;
                        case VER_INVT_FAN1:
                            if (val_freq_fan_set != FREQ_FAN_SET_EC)
                            {
                                if (--cursor < 0)
                                {
                                    cursor = VER_MAX-1;
                                    break;
                                }
                            }
                            else
                            {
                                break;
                            }
                            //break;
                        case VER_INVT_CM:
                            if (!val_inverter_used)
                            {
                                if (--cursor < 0)
                                {
                                    cursor = VER_MAX-1;
                                    break;
                                }
                            }
                            else
                            {
                                break;
                            }
                            //break;
                        default:
                            break;
                    }

                    if (cursor >= VER_IN_1 && cursor <= VER_OUT_7)
                    {
                        if (cursor/2 >= val_pcb_num)
                        {
                            cursor = val_pcb_num*2-1;
                        }
                    }

//                    if (cursor>=VER_IN_1 && cursor<=VER_IN_7)
//                    {
//                        if (cursor > VER_IN_1+val_pcb_num-2)
//                        {
//                            cursor = VER_IN_1+val_pcb_num-2;
//                        }
//                    }

                    dm603_common_disp_flash();
                    disp_clr_screen(disp_buf);
                }
            }
            /* break; */

        case DM603_MENU_PAINT :
            switch (cursor)
            {
                case VER_IN_0:case VER_IN_1:case VER_IN_2:case VER_IN_3:
                case VER_IN_4:case VER_IN_5:case VER_IN_6:case VER_IN_7:
                    disp_value_U08(cursor/2, ROW1, 0, DCS_NORMAL|DNS_DIGIT1, disp_buf);
                    disp_string(dm603_face_menu_version_in, ROW1, 1*8, DCS_NORMAL, disp_buf);
                    {
                        U08* p_ver = idu_mac_ver[cursor/2];
                        //��ʾ��һ��
                        ARRCLR(CODE_STR);
                        MEMCPY(CODE_STR, p_ver, MAX_VER_LINE1);
                        disp_string_ptr8(CODE_STR, ROW2, 0, DCS_NORMAL, disp_buf);
                        //��ʾ�ڶ���
                        ARRCLR(CODE_STR);
                        MEMCPY(CODE_STR, &p_ver[MAX_VER_LINE1], MAX_VER_LINE2(p_ver));
                        disp_string_ptr8(CODE_STR, ROW3, 0, DCS_NORMAL, disp_buf);
                    }
//                  disp_string_ptr8(FUNC_APP1, ROW2, 0, DCS_NORMAL, disp_buf);
//                  disp_string_ptr8(FUNC_APP2, ROW3, 0, DCS_NORMAL, disp_buf);
                    break;
                case VER_OUT_0:case VER_OUT_1:case VER_OUT_2:case VER_OUT_3:
                case VER_OUT_4:case VER_OUT_5:case VER_OUT_6:case VER_OUT_7:
                    disp_value_U08(cursor/2, ROW1, 0, DCS_NORMAL|DNS_DIGIT1, disp_buf);
                    disp_string(dm603_face_menu_version_out, ROW1, 1*8, DCS_NORMAL, disp_buf);
                    {
                        U08* p_ver = odu_mac_ver[cursor/2];
                        //��ʾ��һ��
                        ARRCLR(CODE_STR);
                        MEMCPY(CODE_STR, p_ver, MAX_VER_LINE1);
                        disp_string_ptr8(CODE_STR, ROW2, 0, DCS_NORMAL, disp_buf);
                        //��ʾ�ڶ���
                        ARRCLR(CODE_STR);
                        MEMCPY(CODE_STR, &p_ver[MAX_VER_LINE1], MAX_VER_LINE2(p_ver));
                        disp_string_ptr8(CODE_STR, ROW3, 0, DCS_NORMAL, disp_buf);
                    }
                    break;

                case VER_FRONT:
                    disp_string(dm603_face_menu_version_f, ROW1, 0, DCS_NORMAL, disp_buf);
                    disp_version_f(ROW2, 0, DCS_NORMAL, disp_buf);
                    break;
                case VER_INVT_CM:
                    {
                        U16 func_code = invt_info_function(INVT_ID_1);
                        U16 ver_code = invt_info_version(INVT_ID_1);
                        U08 ver[INVT_VER_NUM+1];

                        ARRCLR(ver);
                        invt_code_to_version(ver, func_code, ver_code);

                        disp_string(dm603_face_menu_version_i, ROW1, 0, DCS_NORMAL, disp_buf);
                        dm603_disp_version(disp_buf, ROW2, ver);
                    }
                    break;
                case VER_INVT_FAN1:
                    {
                        U16 func_code = invt_info_function(INVT_FAN1);
                        U16 ver_code = invt_info_version(INVT_FAN1);
                        U08 ver[INVT_VER_NUM+1];

                        ARRCLR(ver);
                        invt_code_to_version(ver, func_code, ver_code);

                        disp_string(dm603_face_menu_version_f1, ROW1, 0, DCS_NORMAL, disp_buf);
                        dm603_disp_version(disp_buf, ROW2, ver);
                    }
                    break;
                case VER_INVT_FAN2:
                    {
                        U16 func_code = invt_info_function(INVT_FAN2);
                        U16 ver_code = invt_info_version(INVT_FAN2);
                        U08 ver[INVT_VER_NUM+1];

                        ARRCLR(ver);
                        invt_code_to_version(ver, func_code, ver_code);

                        disp_string(dm603_face_menu_version_f2, ROW1, 0, DCS_NORMAL, disp_buf);
                        dm603_disp_version(disp_buf, ROW2, ver);
                    }
                    break;
//                case VER_MAC_CODE:
//                    disp_string(dm603_face_menu_machine_code, ROW1, 0, DCS_NORMAL, disp_buf);
                    /* ������ʾ���� */
//                  disp_string_ptr8(dm603_code_buf, ROW2, 0, DCS_NORMAL, disp_buf);
//                    break;
                default:
                    break;
            }

            /* ��ͷ */
            dm603_common_disp_arrow(cursor, 0, disp_ver_max, disp_buf);
            /* �˵��� */
            dm603_common_disp_menu(BMP_FN, dm603_face_return, BMP_NU, NULL, disp_buf);

            break;
    }
}


/**
 * ���� : dm603_face_menu_time_set()
 * ���� : ��ʱʱ������
 *
 * ���� : @num ��ʱ������
 *        ��00    ��ʾһ�ζ�ʱ
 *        ��01~10 ��ʾXX�鶨ʱ��һ��10��
 * ���� : --
 *
 * ��ע : --
 */
static void dm603_face_menu_time_set(U08 *disp_buf)
{
    U08 i;
    U16 attrib = 0;
    U08 num = dm603_item[hmi_t_num];

    /* ���λ */
    static U08 s_cursor;
    static U08 s_min_cursor;
    static U08 s_max_cursor;

    /* ����ֵ */
    static U08 s_time;
    static U08 s_hour;
    static U08 s_min ;

    static U16 msg;

    switch (dm603_menu[hmi_t_num])
    {
        case DM603_MENU_CREATE :
            dm603_menu[hmi_t_num] = DM603_MENU_PROC;
            key_delay_time[hmi_t_num]=0;
            disp_clr_screen(disp_buf);
            dm603_common_disp_flash();

            /* ��ʱ���ѡ�� */
            s_cursor = 0;
            s_min_cursor = 0;
            if (num) s_max_cursor = 10;
            else     s_max_cursor = 3;
            if (num == 0)   //��ʱʹ�÷�
            {
                s_max_cursor = 1;
            }
            else if (num == 1)  //һ�ζ�ʱ
            {
                s_max_cursor = 4;
            }
            else                //���ڶ�ʱ
            {
                s_max_cursor = 10;
            }
            msg = msg_menu_time_select[num];

            /* ��ʱʱ��ֵ */
            if (num ==0)
            {
                s_time = dm603_time_tbl[0];
            }
            else
            {
                s_time = dm603_time_tbl[(num-1)*3 + 0];
                s_hour = dm603_time_tbl[(num-1)*3 + 1];
                s_min  = dm603_time_tbl[(num-1)*3 + 2];
            }

            /* break; */

        case DM603_MENU_PROC :
            if(key_delay_time[hmi_t_num]>=KEY_DELAY)
            {
                /* �����ϼ����� */
                if (Main_Key[hmi_t_num] == DM603_KEY_FUNC)
                {
                    if (num ==0)
                    {
                        dm603_time_tbl[0] = s_time;
                    }
                    else
                    {
                        dm603_time_tbl[(num-1)*3 + 0] = s_time;
                        dm603_time_tbl[(num-1)*3 + 1] = s_hour;
                        dm603_time_tbl[(num-1)*3 + 2] = s_min ;
                    }

                    /* ���涨ʱʱ�� */
//                  dm603_face_menu_eeprom(MENU_WR, dm603_time_tbl);
                    DataAccess_Timer(DATA_WRITE);

                    dm603_pop_face_status();    /* �ָ��ϲ����״̬ */
                    return;
                }
                /* �л���ʱ�� */
                if (Main_Key[hmi_t_num] == DM603_KEY_OK)
                {
                    if (++s_cursor >= s_max_cursor) s_cursor = s_min_cursor;
                }

                /* ���ϻ������л� */
                if (Main_Key[hmi_t_num] == DM603_KEY_UP || Main_Key[hmi_t_num] == DM603_KEY_DOWN)
                {
                    if (num == 0)   //��ʱʹ�÷�����
                    {

    //                  if (Main_Key[hmi_t_num] == DM603_KEY_UP || Main_Key[hmi_t_num] == DM603_KEY_DOWN)
    //                  {
                            if (s_cursor == 0)  dm603_common_toggle_byte_bit(&s_time, 0);           /* ��ʱ���� */
    //                  }
                    }
                    else
                    {
                        if (s_cursor == 1)/* ��ʱʱ */
                        {
                            if (Main_Key[hmi_t_num] == DM603_KEY_UP)
                            {
                                if (++s_hour >= 24) s_hour = 0;
                            }
                            if (Main_Key[hmi_t_num] == DM603_KEY_DOWN)
                            {
                                if (s_hour-- <= 0) s_hour = 23;
                            }
                        }
                        else if (s_cursor == 2)/* ��ʱ�� */
                        {
                            if (Main_Key[hmi_t_num] == DM603_KEY_UP)
                            {
                                if (++s_min >= 60) s_min  = 0;
                            }
                            if (Main_Key[hmi_t_num] == DM603_KEY_DOWN)
                            {
                                if (s_min-- <= 0) s_min = 59;
                            }
                        }
                        else    /* ��ʱ����λ */
                        {
                            /* ���ϻ������л� */
    //                      if (Main_Key[hmi_t_num] == DM603_KEY_UP || Main_Key[hmi_t_num] == DM603_KEY_DOWN)
    //                      {
                                if (s_cursor == 0)                      dm603_common_toggle_byte_bit(&s_time, 7);           /* ��ʱ���� */
                                else if((s_cursor == 3)&&(num == 1))        dm603_common_toggle_byte_bit(&s_time, 6);           /* һ�ζ�ʱʹ�ÿ��� */
                                else                                    dm603_common_toggle_byte_bit(&s_time, s_cursor-3);  /* ��ʱ���� */
    //                      }
                        }
                    }
//                  dm603_common_disp_flash();  // ˢ�½���
                }

            }
            /* ˢ�½��� */
//          if (Main_Key[hmi_t_num] == DM603_KEY_UP || Main_Key[hmi_t_num] == DM603_KEY_DOWN) dm603_common_disp_flash();
            /* break; */

        case DM603_MENU_PAINT :
            if (Main_Key[hmi_t_num] != KEY_NULL)    dm603_common_disp_flash();
            /* ��ʱ�� */
            disp_string(msg, ROW1, 0, DCS_NORMAL, disp_buf);

            /* ��ʱʱ�� */
            for (i = 0; i < s_max_cursor; i++)
            {
                /* �Ƿ�ɫ */
                attrib = DCS_NORMAL;
                if (i == s_cursor) attrib = DCS_REVERSE;

                /* ��ʱ���� */
                if (i == 0)
                {
                    if (num == 0)
                    {
                        disp_string(GET_BIT(s_time, 0)? \
                                dm603_face_time_func_ok : dm603_face_time_func_no, \
                                ROW2, 5*16+8, attrib, disp_buf);
                    }
                    else
                    {
                        disp_string(GET_BIT(s_time, 7)? \
                                dm603_face_menu_time_set_ok : dm603_face_menu_time_set_no, \
                                ROW2, 16, attrib, disp_buf);
                    }

                }
                else if (i == 1)        /* ��ʱʱ */
                {
                    disp_value_U08(s_hour, ROW2, 3*16+32, DNS_ZERO | DNS_DIGIT2 | attrib, disp_buf);
                }
                else if (i == 2)        /* ��ʱ�� */
                {
                    disp_string_ptr8(":", ROW2, 3*16+32+2*8, DCS_NORMAL, disp_buf);
                    disp_value_U08(s_min, ROW2, 3*16+32+2*8+8, DNS_ZERO | DNS_DIGIT2 | attrib, disp_buf);
                }
                else                    /* ��ʱ���� */
                {   if((i == 3)&&(num == 1))    //һ����ʱ��ʹ�ÿ��� jxj2016.01.15
                    {
                        disp_string(GET_BIT(s_time, 6)? dm603_face_time_func_ok : dm603_face_time_func_no, ROW3, 1*16, attrib, disp_buf);
                    }
                    else
                    {
                        if (attrib == DCS_REVERSE)  attrib = DCS_BLINK;
                        else                        attrib = DCS_NORMAL;

                        if (GET_BIT(s_time, i-3))   attrib |= DCS_REVERSE;

                        disp_value_U08(i-2, ROW3, (i-3+1)*16, DNS_DIGIT1 | attrib, disp_buf);
                    }
                }
            }

            /* �˵��� */
            if (num == 0)
            {
                dm603_common_disp_menu(BMP_FN, dm603_face_return, \
                                   BMP_NU, NULL, \
                                   disp_buf);
            }
            else
            {
                dm603_common_disp_menu(BMP_FN, dm603_face_return, \
                                   BMP_OK, dm603_face_menu_time_set_ent, \
                                   disp_buf);
            }

            break;
    }
}

/**
 * ���� : dm603_face_menu_time()
 * ���� : ��ʱ����
 *
 * ���� : --
 * ���� : --
 *
 * ��ע : �ٶ�ʱʱ������
 */
static void dm603_face_menu_time(U08 *disp_buf)
{
    U08 i = 0;
//  U08 tmp = 0;
    U08 row = 0;
    U08 min, max;
    U16 attrib = 0;
    U08 menu_opt = MENU_DOWN;
//  static BOOL fg_enable_set = FALSE;  /* ��ʱʹ������ */
    U08 mod_page_max = 3;                   /* ÿҳ�����Ŀ�� */
    U08 item_max = MENU_TIME_SELECE_MAX;    //���ʱ����
    static U08 s_time_en;               //��ʱʹ�÷�ʹ��Ϊ1����ʹ��Ϊ0
    U08 cursor = 0;

    switch (dm603_menu[hmi_t_num])
    {
        case DM603_MENU_CREATE :
            dm603_menu[hmi_t_num] = DM603_MENU_PROC;
            key_delay_time[hmi_t_num]=0;
            if (dm603_face_dir[hmi_t_num] == FECE_ENTER)
            {
                dm603_item[hmi_t_num] = 0;
            }
            s_time_en = GET_TIME_EN();
            dm603_common_disp_flash();
            /* break; */

        case DM603_MENU_PROC :
            if (s_time_en)
            {
                item_max = MENU_TIME_SELECE_MAX;
            }
            else
            {
                item_max = 1;
            }
            if(key_delay_time[hmi_t_num]>=KEY_DELAY)
            {
                switch (Main_Key[hmi_t_num])
                {
                    case DM603_KEY_FUNC:        /* �����ϲ���� */
                        dm603_pop_face_status();    /* �ָ��ϲ����״̬ */
                        return;
//                      break;

                    case DM603_KEY_OK:      /* �����²���� */
                        dm603_push_face_status();   /* �������״̬ */
                        dm603_face[hmi_t_num] = FACE_MENU_TIME_SET;
                        break;  /*  */

                    case DM603_KEY_UP:
                    case DM603_KEY_DOWN:
                        if (Main_Key[hmi_t_num] == DM603_KEY_DOWN)
                        {
                            menu_opt = MENU_DOWN;
                        }
                        else
                        {
                            menu_opt = MENU_UP;
                        }
                        dm603_item[hmi_t_num] = dm603_common_disp_select(dm603_item[hmi_t_num], menu_opt, MENU_TIME_SELECE_MIN, \
                                                                            item_max);
                        disp_clr_screen(disp_buf);
                        break;

//                    case DM603_KEY_OK_3S:
//                        if ((status_run==_IDLE || status_run==_ALARM)   // ����ʱ�ɵ���ˮ��
//                        && (!fg_freeze_ac_act)                          /* ����ʱ������ */
//                        )
//                        {
//                            debug_mode = TRUE;
//                            dm603_pop_face_status();    /* �ָ��ϲ����״̬ */
//                            return;
//                        }
//                        break;
                }
            }
            /* break; */

        case DM603_MENU_PAINT :
            if (Main_Key[hmi_t_num] != KEY_NULL)
            {
                dm603_common_disp_flash();
            }
            cursor = dm603_common_disp_bound(dm603_item[hmi_t_num], MENU_TIME_SELECE_MIN, item_max, \
                                            mod_page_max, &min, &max);      //�����ʾʼĩ�����ó�������ʾ��
            for (i=min; i<max; i++)
            {
                attrib = DCS_NORMAL;
                if ((i-min) == cursor)                  attrib = DCS_REVERSE;   /* ������ʾ�� */
//              if (i == TIME_FUNC && fg_enable_set)    attrib = DCS_NORMAL;

                row = ROW1 + (i-min)*16;
                disp_string(msg_menu_time_select[i], row, 0, attrib | DTS_CLR_TRAIL, disp_buf);

            }

            /* ��ͷ */
            dm603_common_disp_arrow(dm603_item[hmi_t_num], MENU_TIME_SELECE_MIN, item_max, \
                                    disp_buf);

            /* �˵��� */
            dm603_common_disp_menu(BMP_FN, dm603_face_return,  \
                                   BMP_OK, dm603_face_menu_time_select_ent, disp_buf);

            break;
    }
}


/**
 * ���� : dm603_face_menu_func_runtime()
 * ���� : ����ʱ���ѯ
 *
 * ���� : --
 * ���� : --
 *
 * ��ע : --
 */
static void dm603_face_menu_func_runtime(U08 *disp_buf)
{
    U08 i;
    U08 row;

    static U08 s_pcb;   /* ģ��� */
    static U08 cursor;  /* ѹ��ҳָ�� */

    switch (dm603_menu[hmi_t_num])
    {
        case DM603_MENU_CREATE :
            dm603_menu[hmi_t_num] = DM603_MENU_PROC;
            key_delay_time[hmi_t_num]=0;
            dm603_common_disp_flash();
            s_pcb = 0;
            cursor = 0;
            /* break; */

        case DM603_MENU_PROC :
            if(key_delay_time[hmi_t_num]>=KEY_DELAY)
            {
                switch (Main_Key[hmi_t_num])
                {
                    case DM603_KEY_FUNC:
                        dm603_pop_face_status();    /* �ָ��ϲ����״̬ */
                        return;
//                      break;


                    case DM603_KEY_OK:      /* �л�ģ�� */
                        if (++s_pcb >= val_pcb_num) s_pcb = 0;
                        break;

                    /* ���ϻ������л� */
                    case DM603_KEY_UP:
                    case DM603_KEY_DOWN:
                        break;
                }
            }
            /* break; */

        case DM603_MENU_PAINT :
            if (Main_Key[hmi_t_num] != KEY_NULL)    dm603_common_disp_flash();

            /* ģ����Ϣ */
            disp_value_U08(s_pcb, ROW1, 0, DNS_ZERO | DNS_DIGIT2, disp_buf);
            disp_string(dm603_face_menu_func_runtime_unit, ROW1, 2*8, DCS_NORMAL, disp_buf);

            /* ģ�鲻���� */
            if (!CELL_Is_used(s_pcb)
            ||  err_sys_get_s_commu_err(s_pcb)
                )
            {
                disp_string(msg_mod_cannot_use, ROW2, 0, DTS_CENTER|DTS_CLR_HEAD|DTS_CLR_TRAIL, disp_buf);
                break;
            }

            /* ѹ��ʱ�� */
            for (i = 0; i < m_val_comp_num(s_pcb); i++)
            {
                row = ROW2 + i*16;

                disp_value_U08(i+1, row, 0*8, DCS_NORMAL|DNS_DIGIT1, disp_buf);
                disp_string_ptr8("#", row, 1*8, DCS_NORMAL, disp_buf);
                disp_string(dm603_face_menu_func_runtime_unit_comp, row, 2*8, DCS_NORMAL, disp_buf);
                dm603_common_disp_val(cell[s_pcb].cell_runtime[i], row, COL_END - COL_ASC, UINT_H, DCS_NORMAL, disp_buf);   /* ѹ������ʱ�� */
            }


            /* ��ͷ */
            dm603_common_disp_arrow(cursor, 0, 0, disp_buf);
            /* �˵��� */
            dm603_common_disp_menu(BMP_FN, dm603_face_return,  \
                                   BMP_OK, dm603_face_menu_func_runtime_ent, disp_buf);
            break;
    }
}

/**
 * ���� : dm603_face_menu_func_his_err()
 * ���� : ��ʷ���ϲ�ѯ
 *
 * ���� : --
 * ���� : --
 *
 * ��ע : --
 */
static void dm603_face_menu_func_his_err(U08 *disp_buf)
{
    U08 i;

    U08 s_id = 0;
    U08 s_id_tag = 0; // ��ʷ���ϸ��Ӻ�
    U08 s_pcb = 0;

//  U08 s_year  = 0;
//  U08 s_month = 0;
//  U08 s_day   = 0;
//  U08 s_hour  = 0;
//  U08 s_min   = 0;

    static U08 s_cursor;
    static U08 s_min_err;
    static U08 s_max_err;
//  static U08 fg_flash;

    RTC ClockTmp[MAX_HMI];

    switch (dm603_menu[hmi_t_num])
    {
        case DM603_MENU_CREATE :
            dm603_menu[hmi_t_num] = DM603_MENU_PROC;
            key_delay_time[hmi_t_num]=0;
            dm603_menu_sub[hmi_t_num] = FACE_SUB_MENU_SELECT;
            dm603_common_disp_flash();

            s_cursor = 0;
            s_min_err = 0;
            s_max_err = 0;
            for (i = 0; i < MAX_HIS_ERROR; i++)
            {
                if (his_err_tbl[i*HIS_ERR_SIZE] != F_NO_ERR)
                {
                    s_max_err++;    /* ������ʷ�������� */
                }
            }
            /* break; */

        case DM603_MENU_PROC :
            /* �л��ӽ��� */
            if (dm603_sub_bak[hmi_t_num] != dm603_menu_sub[hmi_t_num])
            {
                disp_clr_screen(disp_buf);
                dm603_common_disp_flash();
                dm603_sub_bak[hmi_t_num] = dm603_menu_sub[hmi_t_num];
            }

            if (dm603_menu_sub[hmi_t_num]  == FACE_SUB_MENU_SELECT)
            {
                if(key_delay_time[hmi_t_num]>=KEY_DELAY)
                {
                    switch (Main_Key[hmi_t_num])
                    {
                        case DM603_KEY_FUNC:
                            dm603_pop_face_status();    /* �ָ��ϲ����״̬ */
                            return;
//                          break;

                        case DM603_KEY_OK:  /* ��� */
                            if (s_max_err)              //�й���ʱ���������
                            {
                                dm603_menu_sub[hmi_t_num] = FECE_SUB_ACT;
                            }
                            return;
//                          break;

                        case DM603_KEY_UP:  /* �����л� */
                            if (s_cursor-- <= s_min_err) s_cursor = s_max_err - 1;
                            break;

                        case DM603_KEY_DOWN:    /* �����л� */
                            if (++s_cursor >= s_max_err) s_cursor = s_min_err;
                            break;

                        default:
                            break;
                    }
                }
            }
            else
            {
                if (Main_Key[hmi_t_num] == DM603_KEY_FUNC)      /* ȡ�� */
                {
                    dm603_menu[hmi_t_num] = DM603_MENU_CREATE;
                }
                else if (Main_Key[hmi_t_num] == DM603_KEY_OK)   /* ȷ�� */
                {
                    DataAccess_HistoryErr(DATA_INIT);       // ��ʼ����ʷ������Ϣ
                    dm603_menu[hmi_t_num] = DM603_MENU_CREATE;
                    break;
                }
            }
            /* break; */

        case DM603_MENU_PAINT :
            if (Main_Key[hmi_t_num] != KEY_NULL)
            {
                disp_clr_screen(disp_buf);
                dm603_common_disp_flash();
            }

            if (dm603_menu_sub[hmi_t_num]  == FACE_SUB_MENU_SELECT)
            {
                if (s_max_err)      /* ����ʷ���� */
                {
                    /* ��ʷ������Ϣ */
                    s_id  = his_err_tbl[s_cursor*HIS_ERR_SIZE+0];
                    s_pcb = his_err_tbl[s_cursor*HIS_ERR_SIZE+1];
                    s_id_tag = his_err_tbl[s_cursor*HIS_ERR_SIZE+2];

                    dm603_disp_err(s_pcb, s_id, s_id_tag, ROW2, disp_buf);

                    /* ��ʷ����ʱ�� */

                    ClockTmp[hmi_t_num].month = his_err_tbl[s_cursor*HIS_ERR_SIZE+3];
                    ClockTmp[hmi_t_num].day   = his_err_tbl[s_cursor*HIS_ERR_SIZE+4];
                    ClockTmp[hmi_t_num].hour  = his_err_tbl[s_cursor*HIS_ERR_SIZE+5];
                    ClockTmp[hmi_t_num].min   = his_err_tbl[s_cursor*HIS_ERR_SIZE+6];
                    ClockTmp[hmi_t_num].sec   = his_err_tbl[s_cursor*HIS_ERR_SIZE+7];

                    DispInfo_Clock(ROW1,  0, ClockTmp[hmi_t_num], MON|DAY, 0, disp_buf);
                    DispInfo_Clock(ROW1, 48, ClockTmp[hmi_t_num], HHMMSS, 0, disp_buf);
                    disp_value_U08(s_cursor+1, ROW1, 128-16, DNS_ZERO|DNS_DIGIT2|DCS_REVERSE, disp_buf);
                }
                else            /* ����ʷ���� */
                {
                    disp_string(dm603_face_menu_func_his_err_00, ROW1, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);
                }
                /* �˵��� */

                dm603_common_disp_menu(BMP_FN, dm603_face_return, \
                                       s_max_err ? BMP_OK : BMP_NU, dm603_face_menu_func_his_err_ent, disp_buf);
            }
            else
            {
                /* ��ʾ�� */
                disp_string(dm603_face_query_his_err_01, ROW1, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);
                disp_string(dm603_face_query_his_err_02, ROW2, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);

                /* �˵��� */
                dm603_common_disp_menu(BMP_FN, dm603_face_query_ret, BMP_OK, dm603_face_ok, disp_buf);
            }
            break;

        default:
            break;
    }
}

/*******************************************************************************
 �� ��: dm603_face_invt_para()
 �� ��: ��Ƶ��������
 �� ��: disp_buf--;
 �� ��:
 �� ��: ��
*******************************************************************************/
typedef struct INVT_BI_SET
{
    I16 *pdata;
    I16 min;
    I16 max;
}INVT_BI_SET_STRU;

static void dm603_face_invt_para(U08 *disp_buf)
{
    enum {IPA_ADDR, IPA_VAL, IPA_RD, IPA_WR,
          IPA_NULL};

    static U08 cursor;
    BOL fg_key = TRUE;
    static I16 addr_bak;

    #define IPA_BLINK(S) (cursor==S?DCS_BLINK:0)
    #define IPA_INIT -1     /* ��ʼֵ-1����ʾδ�� */

    INVT_BI_SET_STRU ipa_set[IPA_NULL] =
    {
        {&info_invt_para[INVT_ID_1].addr,      0,      30000   },
        {&info_invt_para[INVT_ID_1].val,       -30000, 30000   },
        {&info_invt_para[INVT_ID_1].fg_rd,     0,      1       },
        {&info_invt_para[INVT_ID_1].fg_wr,     0,      1       },
    };

    switch (dm603_menu[hmi_t_num])
    {
        case DM603_MENU_CREATE :
            dm603_menu[hmi_t_num] = DM603_MENU_PROC;
            key_delay_time[hmi_t_num]=0;
            cursor = IPA_NULL;
            PunpDispClearScreen();

            info_invt_para[INVT_ID_1].val = IPA_INIT;
            /* break; */

        case DM603_MENU_PROC :
            if(key_delay_time[hmi_t_num]>=5)
            {
                switch (Main_Key[hmi_t_num])
                {
                    case DM603_KEY_FUNC:    /* ���� */
                        dm603_pop_face_status();
                        return;
//                        break;

                    case DM603_KEY_OK:    /* ��һ�� */
                        cursor++;
                        if (cursor > IPA_NULL) cursor = 0;
                        break;

                    case DM603_KEY_DOWN: /* �� */
                        if (cursor != IPA_NULL)
                        {
                            *(ipa_set[cursor].pdata) = key_continuous_sub(*(ipa_set[cursor].pdata), ipa_set[cursor].max, ipa_set[cursor].min, CAL_CYCLE);
                            if (fun_pv_in(cursor, IPA_RD, IPA_WR))
                            {
                                cursor = IPA_NULL;
                            }
                        }
                        break;

                    case DM603_KEY_UP: /* �� */
                        if (cursor != IPA_NULL)
                        {
                            *(ipa_set[cursor].pdata) = key_continuous_add(*(ipa_set[cursor].pdata), ipa_set[cursor].max, ipa_set[cursor].min, CAL_CYCLE);
                            if (fun_pv_in(cursor, IPA_RD, IPA_WR))
                            {
                                cursor = IPA_NULL;
                            }
                        }
                        break;

                    default:
                        fg_key = FALSE;
                        break;
                }

                if (fg_key)
                {
                    dm603_common_disp_flash();
                }

                if (addr_bak != info_invt_para[INVT_ID_1].addr)
                {
                    addr_bak = info_invt_para[INVT_ID_1].addr;

                    info_invt_para[INVT_ID_1].val = IPA_INIT;
                }
            }

            /* break; */

        case DM603_MENU_PAINT :
            {
                /* ��ֹ���� */
                cnt_no_operate[hmi_t_num] = 0;
                cnt_no_operate_sec[hmi_t_num] = 0;

                /* ROW1 */
                disp_string_ptr8("Addr:",           ROW1,  0,   DCS_NORMAL,                     disp_buf);
                disp_value_U16(info_invt_para[INVT_ID_1].addr, ROW1,  8*8, DNS_DIGIT5|IPA_BLINK(IPA_ADDR), disp_buf);

                /* ROW2 */
                disp_string_ptr8("Val:",            ROW2,  0,   DCS_NORMAL,                     disp_buf);
                disp_value_I16(info_invt_para[INVT_ID_1].val,  ROW2,  8*8, DNS_DIGIT5|IPA_BLINK(IPA_VAL),  disp_buf);

                /* ROW3 */
                disp_string_ptr8("Read:",           ROW3,  0,   DCS_NORMAL,                     disp_buf);
                disp_value_I16(info_invt_para[INVT_ID_1].fg_rd,ROW3,  8*8, DNS_DIGIT1|IPA_BLINK(IPA_RD),   disp_buf);

                /* ROW4 */
                disp_string_ptr8("Write:",          ROW4,  0,   DCS_NORMAL,                     disp_buf);
                disp_value_I16(info_invt_para[INVT_ID_1].fg_wr,ROW4,  8*8, DNS_DIGIT1|IPA_BLINK(IPA_WR),   disp_buf);

                /* ������ʾ */
                if (invt_info_is_running(INVT_ID_1))
                {
                    disp_string_ptr8("RUN", ROW4, COL_END-8*3, DCS_NORMAL, disp_buf);
                }
                else
                {
                    disp_string_ptr8("OFF", ROW4, COL_END-8*3, DCS_NORMAL, disp_buf);
                }
            }
            break;
    }
}


/*******************************************************************************
 �� ��: dm603_face_invt_except_info()
 �� ��: ��Ƶ�쳣��Ϣ��ѯ
 �� ��: disp_buf--;
 �� ��:
 �� ��: ��
*******************************************************************************/
static void dm603_face_invt_except_info(U08 *disp_buf)
{
    static U08 cursor;

    switch (dm603_menu[hmi_t_num])
    {
        case DM603_MENU_CREATE :
            dm603_menu[hmi_t_num] = DM603_MENU_PROC;
            key_delay_time[hmi_t_num]=0;
            cursor = 0;
            PunpDispClearScreen();
            /* break; */

        case DM603_MENU_PROC :
            if(key_delay_time[hmi_t_num]>=5)
            {
                switch (Main_Key[hmi_t_num])
                {
                    case DM603_KEY_FUNC:    /* ���� */
                        dm603_pop_face_status();
                        return;
//                        break;

                    case DM603_KEY_DOWN: /* �·� */
                        PunpDispClearScreen();
                        cursor++;
                        if (cursor == INFO_MAX) cursor = 0;
                        break;

                    case DM603_KEY_UP: /* �Ϸ� */
                        PunpDispClearScreen();
                        cursor--;
                        if (cursor == _MAX_(cursor)) cursor = INFO_MAX-1;
                        break;

                    case DM603_KEY_OK_3S:   /* ��� */
                        DataAccess_InvtExceptInfo(DATA_INIT, _MAX_(U08));
                        break;
                }
            }
            /* break; */

        case DM603_MENU_PAINT :
            /* �쳣��Ϣ��ʾ */
            {
                U08 *info_str[INFO_MAX] = {"200", "004", "007", "008",
                                           "E02", "E03", "E04", "E12",
                                           "E19", "E20", "E31", "EX1",
                                           "EX2", "EX3", "EX4", "EX5",
                                           };
                InvtEXCEPT *pInfo = info_invt_except;
                /* ROW1 */
                disp_string_ptr8(info_str[cursor], ROW1, 0, DCS_NORMAL|DCS_REVERSE, disp_buf);
                pInfo[cursor].info = MIN(pInfo[cursor].info, 9999); /* ֻ����ʾ4λ */
                disp_value_U16(pInfo[cursor].info, ROW1, 3*8, DNS_DIGIT4|DNS_ZERO|DCS_NORMAL, disp_buf);
                disp_value_I16(pInfo[cursor].state[6], ROW1,  7*8, DNS_DIGIT3|DNS_FLOAT1|DCS_REVERSE, disp_buf);
                disp_value_I16(pInfo[cursor].state[7], ROW1, 12*8, DNS_DIGIT3|DNS_FLOAT1, disp_buf);
                /* ROW2 */
                DispInfo_Clock(ROW2,  0, pInfo[cursor].time, MON|DAY, 0, disp_buf);
                DispInfo_Clock(ROW2, 48, pInfo[cursor].time, HHMMSS, 0, disp_buf);
//                DispInfo_Clock(ROW2, 0, pInfo[cursor].time, YYYYMMDD|HHMM, 0, disp_buf);
                /* ROW3 */
                disp_value_I16(pInfo[cursor].state[0], ROW3,  0*8, DNS_DIGIT4|DNS_FLOAT1, disp_buf);
                disp_value_I16(pInfo[cursor].state[1], ROW3,  5*8, DNS_DIGIT4|DNS_FLOAT1|DCS_REVERSE, disp_buf);
                disp_value_I16(pInfo[cursor].state[2], ROW3, 10*8, DNS_DIGIT4|DNS_FLOAT1, disp_buf);
                if (cursor > 0)             disp_bmp(idx_bmp_up_8_16, ROW3, COL_END-8, DCS_NORMAL, disp_buf);
                else                        disp_string_ptr8(" ", ROW3, COL_END-8, DCS_NORMAL, disp_buf);
                /* ROW4 */
                disp_value_I16(pInfo[cursor].state[3], ROW4,  0*8, DNS_DIGIT4|DNS_FLOAT1, disp_buf);
                disp_value_I16(pInfo[cursor].state[4], ROW4,  5*8, DNS_DIGIT4|DNS_FLOAT1|DCS_REVERSE, disp_buf);
                disp_value_I16(pInfo[cursor].state[5], ROW4, 10*8, DNS_DIGIT4|DNS_FLOAT1, disp_buf);
                if (cursor < INFO_MAX-1)    disp_bmp(idx_bmp_down_8_16, ROW4, COL_END-8, DCS_NORMAL, disp_buf);
                else                        disp_string_ptr8(" ", ROW4, COL_END-8, DCS_NORMAL, disp_buf);
            }
            break;
    }
}

/**
 * ���� : dm603_face_menu_func_defrost()
 * ���� : �ֶ���˪
 *
 * ���� : --
 * ���� : --
 *
 * ��ע : --
 */
static void dm603_face_menu_func_defrost(U08 *disp_buf)
{
    U08 i;
    U16 pmsg[MAX_HMI];
    static U08 s_pcb[MAX_HMI];  /* ģ��� */

    switch (dm603_menu[hmi_t_num])
    {
        case DM603_MENU_CREATE :
            dm603_menu[hmi_t_num] = DM603_MENU_PROC;
            key_delay_time[hmi_t_num]=0;
            dm603_common_disp_flash();
            s_pcb[hmi_t_num] = 0;
            /* break; */

        case DM603_MENU_PROC :
            if(key_delay_time[hmi_t_num]>=KEY_DELAY)
            {
                /* ���������� */
                if (Main_Key[hmi_t_num] == DM603_KEY_FUNC)
                {
                        dm603_pop_face_status();    /* �ָ��ϲ����״̬ */
                        return;
                }

                /* ǿ�Ƴ�˪ */
                if (Main_Key[hmi_t_num] == DM603_KEY_OK)
                {
                    dm603_common_disp_flash();
                    DM603_CMD_DEFROST(s_pcb[hmi_t_num]);
                }

                /* ���ϻ������л�ģ�� */
                if (Main_Key[hmi_t_num] == DM603_KEY_DOWN)
                {
                    dm603_common_disp_flash();
                    if (++s_pcb[hmi_t_num] >= val_pcb_num) s_pcb[hmi_t_num] = 0;
                }
                if (Main_Key[hmi_t_num] == DM603_KEY_UP)
                {
                    dm603_common_disp_flash();
                    if (s_pcb[hmi_t_num]-- <= 0) s_pcb[hmi_t_num] = val_pcb_num - 1;
                }
            }
            /* break; */

        case DM603_MENU_PAINT :
            /* ��ʾ��Ϣ */
            disp_string(dm603_face_menu_func_defrost_info, ROW1, 0, DCS_NORMAL, disp_buf);
            disp_bmp(BMP_UP, ROW1, 16, DCS_NORMAL, disp_buf);
            disp_bmp(BMP_DN, ROW1, 16*2, DCS_NORMAL, disp_buf);

            /* ģ����Ϣ */
            disp_value_U08(s_pcb[hmi_t_num], ROW2, 0, DNS_ZERO | DNS_DIGIT2, disp_buf);
            disp_string(dm603_face_menu_func_defrost_unit, ROW2, 2*8, DCS_NORMAL, disp_buf);

            /* ��˪��Ϣ */
            pmsg[hmi_t_num] = DNULL;
            if (flash_500ms) dm603_common_disp_flash();
            for (i = 0; i < m_val_comp_num(s_pcb[hmi_t_num]); i++)
            {
                if (M_CM_GetStatus(s_pcb[hmi_t_num], i, COMP_FORCE_DEFROST)) pmsg[hmi_t_num] = dm603_face_menu_func_defrost_yes;
                if (M_CM_GetStatus(s_pcb[hmi_t_num], i, COMP_DEFROST))        pmsg[hmi_t_num] = dm603_face_menu_func_defrost_now;
            }
            if (pmsg[hmi_t_num] == DNULL)
            {
                /* ģ������ָʾ */
                if (M_CM_GetStatus(s_pcb[hmi_t_num], MAX_COMP, CELL_RUN))
                {
                    pmsg[hmi_t_num] = dm603_face_menu_func_defrost_run;
                }
                else
                {
                    pmsg[hmi_t_num] = dm603_face_menu_func_defrost_stp;
                }
            }

            if (pmsg[hmi_t_num] == DNULL)   dm603_common_disp_clr_line(ROW3, RNUL, RNUL, RNUL, disp_buf);
            else
            {
                disp_string(pmsg[hmi_t_num], ROW3, 128-16*6-8, DCS_NORMAL, disp_buf);
            }

            /* ��ͷ */
            dm603_common_disp_arrow(s_pcb[hmi_t_num], 0, val_pcb_num, disp_buf);

            /* �˵��� */
            dm603_common_disp_menu(BMP_FN, dm603_face_return, \
                                   BMP_OK, dm603_face_menu_func_defrost_ent, disp_buf);
            break;
    }
}

/**
 * ���� : dm603_face_cloud_tuya()
 * ���� : ͿѻWiFi����ѯ
 *
 * ���� : --
 * ���� : --
 *
 * ��ע : --
 */
static void dm603_face_cloud_tuya(U08 *disp_buf)
{

    static U08 configed;
    static const U16 ty_status[]=
    {
        dm603_face_menu_ty_wifi_status_1,
        dm603_face_menu_ty_wifi_status_2,
        dm603_face_menu_ty_wifi_status_3,
        dm603_face_menu_ty_wifi_status_4,
        dm603_face_menu_ty_wifi_status_5,
        dm603_face_menu_ty_wifi_status_6,
    };
    #define CLOUD_TUYA_PAGE_NORMAL      0
    #define CLOUD_TUYA_PAGE_MAC         1
    #define CLOUD_TUYA_PAGE_MAX         2

    static U08 page;

    switch (dm603_menu[hmi_t_num])
    {
        case DM603_MENU_CREATE :
            dm603_menu[hmi_t_num] = DM603_MENU_PROC;
            key_delay_time[hmi_t_num]=0;
            dm603_common_disp_flash();
            disp_clr_screen(disp_buf);
            configed = FALSE;
            page = CLOUD_TUYA_PAGE_NORMAL;
            /* break; */

        case DM603_MENU_PROC :
            if(key_delay_time[hmi_t_num]>=KEY_DELAY)
            {
                /* ���������� */
                if (Main_Key[hmi_t_num] == DM603_KEY_FUNC)
                {
                    dm603_pop_face_status();    /* �ָ��ϲ����״̬ */
                    return;
                }
                if (Main_Key[hmi_t_num] == DM603_KEY_OK_3S)
                {
                    if (page == CLOUD_TUYA_PAGE_NORMAL)
                    {
                        configed = TRUE;
                        dm603_common_disp_flash();
                        disp_trans_ty_reset(disp_buf);
                    }
                }
                if (Main_Key[hmi_t_num] == DM603_KEY_UP)
                {
                    if (++page >= CLOUD_TUYA_PAGE_MAX)
                    {
                        page = CLOUD_TUYA_PAGE_NORMAL;
                    }
                    dm603_common_disp_flash();
                    disp_clr_screen(disp_buf);
                }
                if (Main_Key[hmi_t_num] == DM603_KEY_DOWN)
                {
                    if (page == CLOUD_TUYA_PAGE_NORMAL)
                    {
                        page = CLOUD_TUYA_PAGE_MAC;
                    }
                    else
                    {
                        page--;
                    }
                    dm603_common_disp_flash();
                    disp_clr_screen(disp_buf);
                }
            }


        case DM603_MENU_PAINT :
            switch (page)
            {
                default:
                case CLOUD_TUYA_PAGE_NORMAL:
                    disp_string(dm603_face_menu_ty_wifi_signal, ROW1, 0, DCS_NORMAL, disp_buf);
                    disp_value_U08(hmi_ty_signal, ROW1, 12*8, DNS_DIGIT3 | DCS_NORMAL, disp_buf);
                    if (hmi_ty_status < ARRELE(ty_status))
                    {
                        disp_string(ty_status[hmi_ty_status], ROW2, 0, DCS_NORMAL|DTS_CLR_TRAIL, disp_buf);
                    }

                    if (configed)
                    {
                        disp_string(dm603_face_menu_ty_wifi_config_res, ROW3, 0, DTS_CENTER|DTS_CLR_ALL, disp_buf);
                    }
                    else
                    {
                        disp_string(dm603_face_menu_ty_wifi_config, ROW3, 0, DTS_CENTER|DTS_CLR_ALL, disp_buf);
                    }
                    /* �˵��� */
                    dm603_common_disp_menu( BMP_FN, dm603_face_return, \
                                            BMP_OK, dm603_face_ok, disp_buf);
                    break;
                case CLOUD_TUYA_PAGE_MAC:
                    disp_string(dm603_face_menu_ty_wifi_mac, ROW1, 0, DCS_NORMAL|DTS_CLR_TRAIL, disp_buf);
                    {
                        static const U08 hex[]={"0123456789ABCDEF"};
                        U08 line;
                        U08 n;
                        U08 mac;
                        U08 idx;
                        U08 ascii_buf[10];
                        for (line=0; line<2; line++)
                        {
                            ARRCLR(ascii_buf);
                            for (n=0,idx=0; n<3; n++)
                            {
                                mac = hmi_ty_mac[line*3 + n];
                                ascii_buf[idx++] = hex[mac/16];
                                ascii_buf[idx++] = hex[mac%16];
                                if (n < 2)
                                {
                                    ascii_buf[idx++] = '-';
                                }
                            }
                            disp_string_ptr8(ascii_buf, line==0?ROW2:ROW3, 0, DTS_CENTER|DTS_CLR_ALL, disp_buf);
                        }
                    }
                    /* �˵��� */
                    dm603_common_disp_menu( BMP_FN, dm603_face_return, \
                                            BMP_NU, NULL, disp_buf);
                    break;
            }

            break;
    }
}

/*******************************************************************************
 �� ��: dm603_face_menu_func_user_mode()
 �� ��: ģʽ���ý���
 �� ��:
 �� ��: --
 �� ��: --
*******************************************************************************/

static void dm603_face_menu_func_user_mode(U08 *disp_buf)
{
     PARAMITEM *ParamGroup;         // ������
     ParamGroup = PG_GetAddr_ParamPri(0);

    if(dm603_face_para_SetValue(ParamGroup,para_small[0], 0, 1, disp_buf)==PARA_OPT_M)  // ���õ�0���0�����
    {
        if(dm603_menu[hmi_t_num]==DM603_MENU_PROC)
        {
            if (Main_Key[hmi_t_num] == DM603_KEY_FUNC_3S)
            {
                dm603_face[hmi_t_num] = FACE_MENU;
            }
            else if (Main_Key[hmi_t_num] == DM603_KEY_UP_DOWN)
            {
                dm603_face[hmi_t_num] = FACE_DEBUG;
            }

        }

    }
}

static void dm603_face_menu_func_user_shortcut(U08 *disp_buf)
{
     PARAMITEM *ParamGroup;         // ������
     ParamGroup = PG_GetAddr_ParamPri(0);

    if(dm603_face_para_SetValue(ParamGroup,para_small[0], 0, 4, disp_buf)==PARA_OPT_M)  // ���õ�0���0�����
    {
        if(dm603_menu[hmi_t_num]==DM603_MENU_PROC)
        {
            if (Main_Key[hmi_t_num] == DM603_KEY_FUNC_3S)
            {
                dm603_face[hmi_t_num] = FACE_MENU;
            }
            else if (Main_Key[hmi_t_num] == DM603_KEY_UP_DOWN)
            {
                dm603_face[hmi_t_num] = FACE_DEBUG;
            }

        }
        /* ��ͷ */
        dm603_common_disp_arrow(dm603_item[hmi_t_num], 0, 4, disp_buf);
    }
}

/**
 * ���� : dm603_face_pass_clr()
 * ���� : �������ֵ
 *
 * ���� : @pass_buf ��������Ĵ���
 * ���� : --
 *
 * ��ע : ��ȫ�����Ϊ0
 */
static void dm603_face_pass_clr(U08 *pass_buf)
{
    U08 i;

    /* ���Ϊ0 */
    for (i = 0; i < MAX_PASS_BIT; i++)
    {
        *pass_buf++ = 0;
    }
}

/**
 * ���� : dm603_face_pass_cmp()
 * ���� : ����Ƚ�
 *
 * ���� : @new_pass ��������
 *        @old_pass �Ƚ�����
 * ���� : TRUE ������ͬ
 *        FALSE ���벻ͬ
 *
 * ��ע : --
 */
static BOOL dm603_face_pass_cmp(U08 *new_pass, U08 *old_pass)
{
    U08 i;

    for (i = 0; i < MAX_PASS_BIT; i++)
    {
        if (*new_pass++ != *old_pass++)     return FALSE;
    }

    return TRUE;
}

/**
 * ���� : dm603_face_pass_chg()
 * ���� : ��������
 *
 * ���� : @pass �������ֵ
 *        @opt ���ϻ����¸���ѡ��
 * ���� : --
 *
 * ��ע : --
 */
static void dm603_face_pass_chg(U08 *pass, U08 opt)
{
    U08 val = 0;

    val = *pass;

    if (val == 0) /* ������ֵ */
    {
        val = '0';
    }
    else
    {
        if (opt == PASS_CHG_UP)
        {
            if (++val > '9') val = '0';
        }
        else if (opt == PASS_CHG_DOWN)
        {
            if (val-- <= '0') val = '9';
        }
    }

    *pass = val;
}


/**
 * ���� : dm603_face_pass_disp()
 * ���� : ������ʾ
 *
 * ���� : @cursor ������Ĺ��
 *        @pass �������ֵ
 *        @row ������ʾ��
 * ���� : --
 *
 * ��ע : --
 */
static void dm603_face_pass_disp(U08 cursor, U08 pass, U08 row, U08 *disp_buf)
{
    U08 tmp;
    U08 col;
    const U08 *msg_star = "*******";

    col = (4+cursor) * 8;

    /* ��������ʾ '-', ������ʾ��Ӧ������ֵ */
    if (pass == 0)
    {
        disp_string_ptr8("-", row, col, DCS_REVERSE | DCS_FLASH, disp_buf);
    }
    else
    {
        disp_value_U08(pass-'0', row, col, DCS_REVERSE | DNS_DIGIT1 | DCS_FLASH, disp_buf);
    }

    /* ��ʾ '*' ��ʾ�� */
    tmp = 7 - cursor;
    col = 4 * 8;
    disp_string_ptr8((const U08 *)(&msg_star[tmp]), row, col, DCS_NORMAL, disp_buf);

    tmp = 7 - (7-cursor);
    col = (4+cursor+1) * 8;
    disp_string_ptr8((const U08 *)(&msg_star[tmp]), row, col, DCS_NORMAL, disp_buf);
}
/**
 * ���� : dm603_face_code_disp()
 * ���� : ���������ʾ
 *
 * ���� : @cursor ������Ĺ��
 *        @pass �������ֵ
 *        @row ������ʾ��
 * ���� : --
 *
 * ��ע : --
 */
//static void dm603_face_code_disp(U08 cursor, U08 row, U08 *disp_buf)
//{
//  U08 col;
//  col = cursor * 8;
//  disp_string_ptr8(dm603_code_buf, row, 0, DCS_NORMAL| DCS_FLASH , disp_buf);
//  /* �޸������ʾ */
//  if(cursor == 15)    disp_string_ptr8(&dm603_code_buf[cursor], row, col, DCS_REVERSE| DCS_FLASH , disp_buf);
//  else                disp_value_U08(dm603_code_buf[cursor]-'0', row, col, DCS_REVERSE  | DNS_DIGIT1, disp_buf);
//}


/*******************************************************************************
 �� ��: dm603_face_pass()
 �� ��: �����������
 �� ��: U16 PassValid: ��ǰ���������Ȩ��
 �� ��: --
 �� ��: ����PASS_SUPERȨ�޵�������棬�������ؼ��ɻ�ȡ����ʾ��֤�루���ڼ��㶯̬���룩
*******************************************************************************/
static U08 dm603_face_pass(U16 PassValid, U08 *disp_buf)
{

    static U08 dm603_pass_input;        /* TRUE ����������� / FALSE �������Խ��� */
    static U08 dm603_pass_input_bak;
    static U08 dm603_pass_cursor;       /* �����������λ��� */

    static U08 PassOK = 0xFF;   // ����ͨ������ 0xff��ʾûͨ��

    switch (dm603_menu[hmi_t_num])
    {
        case DM603_MENU_CREATE :
            dm603_menu[hmi_t_num] = DM603_MENU_PROC;
            key_delay_time[hmi_t_num]=0;
            dm603_pass_cursor = 0;
            dm603_pass_input = TRUE;
            dm603_pass_input_bak = 0xff;
            dm603_face_pass_clr(dm603_pass_buf);

            key_delay_time_pass[hmi_t_num]=0;
            PassOK = 0xFF;

            /* break; */

        case DM603_MENU_PROC :
            /* �������� */
            if(key_delay_time[hmi_t_num]>=KEY_DELAY)
            {
                if (dm603_pass_input)
                {
                    if (Main_Key[hmi_t_num] == DM603_KEY_OK)            /* ����ȷ�� */
                    {
                        // ������˳����֤���û������̡�ά�ޡ�����
                        if (PassOK == 0xFF && PassValid&(0x0001<<PASS_USER))
                        {
                            if (dm603_face_pass_cmp(dm603_pass_buf, (U08 *)password[PASS_USER]))
                            {
                                PassOK=PASS_USER;
                            }
                        }
                        if (PassOK == 0xFF && PassValid&(0x0001<<PASS_PRO))
                        {
                            if (dm603_face_pass_cmp(dm603_pass_buf, (U08 *)password[PASS_PRO]))
                            {
                                PassOK=PASS_PRO;
                            }
                        }
                        if (PassOK == 0xFF && PassValid&(0x0001<<PASS_SER))
                        {
                            if (dm603_face_pass_cmp(dm603_pass_buf, (U08 *)password[PASS_SER]))
                            {
                                PassOK=PASS_SER;
                            }
                        }
                        if (PassOK == 0xFF && PassValid&(0x0001<<PASS_FAC))
                        {
                            if (dm603_face_pass_cmp(dm603_pass_buf, (U08 *)password[PASS_FAC]))
                            {
                                PassOK=PASS_FAC;
                            }
                        }
                        if (PassOK == 0xFF && PassValid&(0x0001<<PASS_SUPER))
                        {
                            if(ValidCode!=0)    // ��֤�벻Ϊ0�ż��㲢ʹ�ö�̬����
                            {
                                Password_super_cal(ValidCode);   // ���㳬������
                                if (dm603_face_pass_cmp(dm603_pass_buf, (U08 *)password[PASS_SUPER]))
                                {
                                    PassOK=PASS_SUPER;
                                }
                            }
                        }
                        if (PassOK == 0xFF && PassValid&(0x0001<<PASS_LIMIT))
                        {
                            if (dm603_face_pass_cmp(dm603_pass_buf, (U08 *)password[PASS_LIMIT]))
                            {
                                PassOK=PASS_LIMIT;
                            }
                        }

                        if (PassOK == 0xFF )
                        {
                            dm603_pass_input = FALSE;   /* ������� */
                        }
                        else
                        {
                            dm603_get_nextface_status();    /* ��ת����һ���� */

                            return PassOK;
                        }
                    }
                    /* ������λ */
                    if (Main_Key[hmi_t_num] == DM603_KEY_FUNC)
                    {
                        dm603_common_disp_flash();
                        if (++dm603_pass_cursor >= MAX_PASS_BIT) dm603_pass_cursor = 0;
                    }

                    /* ����ѡ������ */
                    if (Main_Key[hmi_t_num] == DM603_KEY_UP)
                    {
                        //dm603_common_disp_flash();
                        dm603_face_pass_chg(&dm603_pass_buf[dm603_pass_cursor], PASS_CHG_UP);
                    }
                    if (Main_Key[hmi_t_num] == DM603_KEY_DOWN)
                    {
                        //dm603_common_disp_flash();
                        dm603_face_pass_chg(&dm603_pass_buf[dm603_pass_cursor], PASS_CHG_DOWN);
                    }

                    if (Main_Key[hmi_t_num] == DM603_KEY_ONOFF_3S && PassValid&(0x0001<<PASS_SUPER))    // �ж�̬����Ȩ�޵Ľ���ſɻ�ȡ����ʾ��֤��
                    {
                        ValidCode = USER_GetTime(C_5MS);
                    }
                }
                else    /* �������� */
                {
                    if (Main_Key[hmi_t_num] == DM603_KEY_OK)
                    {
                        dm603_menu[hmi_t_num] = DM603_MENU_CREATE;          /* �������� */
                    }
                    else if (Main_Key[hmi_t_num] == DM603_KEY_FUNC)
                    {
                        dm603_pop_face_status();    /* ���ؽ���ǰ�Ľ���״̬ */
                        return PassOK;
                    }
                }
            }
            /* �л����� */
            if (dm603_pass_input != dm603_pass_input_bak)
            {
                dm603_pass_input_bak = dm603_pass_input;
                dm603_common_disp_clr_line(ROW1, ROW2, RNUL, RNUL, disp_buf);
                dm603_common_disp_flash();
            }
            /* break; */

        case DM603_MENU_PAINT :
            if (dm603_pass_input) /* �������� */
            {
                /* ��ʾ��Ϣ */
                disp_string(dm603_face_pass_input, ROW1, 0, DCS_NORMAL, disp_buf);

                /* ��ʾ���� */
                dm603_face_pass_disp(dm603_pass_cursor, \
                                     dm603_pass_buf[dm603_pass_cursor], \
                                     ROW2, disp_buf);

                /* �˵��� */
                dm603_common_disp_menu(BMP_FN, dm603_face_pass_input_ret, \
                                       BMP_OK, dm603_face_pass_input_ent, disp_buf);
            }
            else        /* �������� */
            {
                /* ��ʾ��Ϣ */
                disp_string(dm603_face_pass_input_error_01, ROW1, 0, DCS_NORMAL | DTS_CENTER, disp_buf);
                disp_string(dm603_face_pass_input_error_02, ROW2, 0, DCS_NORMAL | DTS_CENTER, disp_buf);

                /* �˵��� */
                dm603_common_disp_menu(BMP_FN, dm603_face_return, \
                                       BMP_OK, dm603_face_pass_input_error_ent, disp_buf);
            }

            if(ValidCode!=0 && PassValid&(0x0001<<PASS_SUPER))  // �ж�̬����Ȩ�޵Ľ���ſɻ�ȡ����ʾ��֤��
            {
                disp_value_U16(ValidCode, ROW3, 128, DTS_RIGHT|DNS_DIGIT5, disp_buf);
            }

            break;
    }

    return PassOK;
}

/********************************************
����: Face_ParaGroup_hide
����: �ж���Щ�����������ʾ
����: ParamGroup: PARAMITEM�����ַ;  ItemAll: ȫ������; new_tbl: ����ʾ�Ĳ˵�����;                 PassGrade: ��ǰ���뼶��
����ֵ: TRUE ������ʾ / FALSE ��������ʾ
*********************************************/
static U08 Face_ParaGroup_hide(const PARAMITEM *ParamGroup, U08 *Item_tbl, U08 Tbl_size, U16 PassGrade)
{
    U08 n, cur;
    PARAMITEM *pp1;
    U08 GroupNum = 0;

    GroupNum = PG_GetGroupNum(ParamGroup);  // ��ȡ����

    cur = 0;
    for (n = 0; n < GroupNum; n++)
    {
        pp1 = (PARAMITEM *)&ParamGroup[n];

        if (PassGrade == PASS_FAC || PassGrade == PASS_SUPER)
        {
            if (!PG_Is_Show(pp1, D_FAC))    continue;
        }
        else if (PassGrade == PASS_SER)
        {
            if (!PG_Is_Show(pp1, D_SER))    continue;
        }
        else if (PassGrade == PASS_PRO)
        {
            if (!PG_Is_Show(pp1, D_PRO))    continue;
        }
        else if (PassGrade == PASS_USER)
        {
            if (!PG_Is_Show(pp1, D_USER))   continue;
        }
        else if (PassGrade == PASS_LIMIT)   // ʹ���������⴦����ʹ��D_USER����
        {
            if (!PG_Is_Show(pp1, D_USER))   continue;
        }
        else
        {
            continue;
        }

        if(cur<Tbl_size) // ��ֹ����
        {
            Item_tbl[cur++] = n;
        }
    }

    return cur;
}

/**
 * ���� : dm603_face_para_set_chg_m()
 * ���� : ����������
 *
 * ���� : @n ��
 *        @new_tbl ������λ��
 *        @grade ����
 * ���� : ������Ŀ���ʾ�����
 *
 * ��ע : �ٰ����������������������
 */




/*******************************************************************************
 �� ��: Face_para_item_count()
 �� ��: �������ʾ�Ĳ�����
 �� ��: new_tbl: ��ſ���ʾ������ŵ����飻tbl_size��new_tblָ�������Ĵ�С�� grade:��ǰ���뼶��
 �� ��: --
 �� ��: --
*******************************************************************************/
static U08 Face_para_item_count(PARAMITEM* ParamGroup, U08 *new_tbl, U08 tbl_size, U16 grade)
{
    U08 m;
    U08 cur;

    cur = 0;

    for (m = 0; m < ParamGroup->items; m++)
    {
        /* ��������Ƿ����� */
        if (para_is_hide(ParamGroup, m, grade)) continue;

        if (cur >= tbl_size)  break; // ��������������ʾ�˵����������(why:��ֹnew_tblʹ�ó���)

        new_tbl[cur++] = m;

    }


    return cur;
}

#if 0
/**
 * ���� : dm603_face_para_set_bit_count()
 * ���� : ����λ���������
 *
 * ���� : @n ��
 *        @m ��
 * ���� : ��λ���������ʾ��λ��
 *
 * ��ע : �ٸ�ֵΪ��Ӧ�ĸ�����ʹ��ʱ��Ҫ��һ
 */
static U08 dm603_face_para_set_bit_count(U08 n, U08 m)
{
    U08 total = 16;

//  /* ģ����� */
//  if (n == 1 && m == 0)
//  {
//      total = val_pcb_num;
//  }

    return total - 1;
}
#endif

/**
 * ���� : dm603_face_para_set_disp_v()
 * ���� : �������ֵ��ʾ
 *
 * ���� : @opt ���ֵѡ��
 *        @val ��ǰ����ֵ
 *        @num ���
 *        @sign ��������
 *        @para ������Ϣ
 *
 * ���� : --
 *
 * ��ע : ��ֻ��ʾ�������ֵ����
 *        �ڽ�������ʾ�������Ͳ���
 */
static void dm603_face_para_set_disp_v(U08 opt, I16 val, U08 num, U16 sign, const SMALL *para, U08 *disp_buf)
{
    U08 col = 0;
    U08 row = ROW1;

    U08 is_num = 0;
    U16 attrib = 0;

    /* ��һ�� ��ʾ��Ϣ */
    row = ROW1;
    col = 0;
    disp_value_U08(num, row, col, DNS_DIGIT2 | DNS_ZERO, disp_buf);
    col = 2*8;
    disp_string(para->msg, row, col, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);

    /* �ڶ��� ���ֵ */
    row = ROW2;
    //col = 0;
    col = COL_END - COL_ASC;
    attrib = (opt == PARA_OPT_M) ? DCS_NORMAL : DCS_REVERSE;    /* ���ֵ */
    is_num = (para->pmsg == PNULL) ? 1 : 0; /* ���ֻ��ַ� */

    /* ���� */
    if (is_num) dm603_common_disp_val(val, row, col, sign, attrib, disp_buf);
    //else      disp_string(para->pmsg[val], row, col, attrib | DTS_CLR_TRAIL, disp_buf);
    //else      disp_string(para->pmsg[val], row, col, attrib | DTS_RIGHT, disp_buf);
    else        disp_string(para->pmsg[val], row, col, attrib | DTS_RIGHT | DTS_CLR_HEAD, disp_buf);
}

/**
 * ���� : dm603_face_para_set_disp_b()
 * ���� : ����λ��ʾ
 *
 * ���� : @opt ���ֵѡ��
 *        @val ��ǰ����ֵ
 *        @num ���
 *        @b   λ��
 *        @msg λ��Ϣ
 *        @para ������Ϣ
 *
 * ���� : --
 *
 * ��ע : ��ֻ��ʾ����λ������
 */
//static void dm603_face_para_set_disp_b(U08 opt, I16 val, U08 num, U08 b, const U16 **msg, SMALL *para, U08 *disp_buf)
//{
//  U08 col = 0;
//  U08 row = ROW1;
//
//  U16 attrib = 0;
//
//  /* ��һ�� ����ʾ��Ϣ */
//  row = ROW1;
//  col = 0;
//  disp_value_U08(num, row, col, DNS_DIGIT2 | DNS_ZERO, disp_buf);
//  col = 2*8;
//  disp_string(para->msg, row, col, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);
//
//  /* �ڶ��� ֵ��ʾ��Ϣ */
//  row = ROW2;
//  col = 0;
//  disp_value_U08(b, row, col, DNS_DIGIT2 | DNS_ZERO, disp_buf);
//  col = 2*8;
//  disp_string(para->pmsg[b], row, col, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);
//
//  /* ������ λ��ʾ��Ϣ */
//  row = ROW3;
//  col = COL_END - COL_ASC;
//  attrib = (opt == PARA_OPT_M) ? DCS_NORMAL : DCS_REVERSE;    /* ���ֵ */
//  //disp_string(msg[((U08)GET_BIT(val, b))], row, col, attrib | DTS_CLR_TRAIL, disp_buf);
//  disp_string(msg[((U08)GET_BIT(val, b))], row, col, attrib | DTS_RIGHT, disp_buf);
//}


/*******************************************************************************
 �� ��: dm603_face_para_SetValue()
 �� ��: �������ʾ�Ĳ�����
 �� ��: SMALL *para_msg:��������������, U08 Item_min����ʼ��, U08 Item_num��ָ������ʾ����
 �� ��: value_opt ��ǰ�Ƿ�������״̬
 �� ��: ע��Item_num��С(Ϊ0ʱ�������Լ������ʾ�����0ʱ����ʾ��Ϊ��Item_min��ʼ�����Item_num������)
*******************************************************************************/
static U08 dm603_face_para_SetValue(PARAMITEM *ParamGroup, const SMALL *para_msg, U08 Item_min, U08 Item_num,U08 *disp_buf)
{
    U08 i;
    U08 value_opt = PARA_OPT_M; /* ��ǰ��ʾ������ǲ���ֵѡ�� */


    static I16 value;       // ֵ
    static U08 min_para_m;  // ��С��
    static U08 max_para_m;  // �����

    if (Item_num > sizeof(para_m_tbl))   // ��������������ʾ�˵����������(why:��ֹpara_m_tblʹ�ó���)
    {
        return value_opt;
    }

    switch (dm603_menu[hmi_t_num])
    {
        case DM603_MENU_CREATE :
            dm603_menu[hmi_t_num] = DM603_MENU_PROC;
            key_delay_time[hmi_t_num]=0;

            dm603_item[hmi_t_num] = 0;
            FaceStatus[hmi_t_num] = PARA_SELECT;

            min_para_m = 0;
            if(Item_num==0) // �������Լ��㷶Χ
            {
                max_para_m = Face_para_item_count(ParamGroup, para_m_tbl, sizeof(para_m_tbl), dm603_pass_grade[hmi_t_num]);//NOTE-CXW ����2017.4.11
            }
            else    // ָ����Χ
            {
                max_para_m = Item_num;
                for(i=0; i<Item_num; i++)
                {
                    para_m_tbl[i]=Item_min+i;
                }
            }

            para_get_val(ParamGroup, para_m_tbl[0], &value);
            dm603_common_disp_flash();
            /* break; */

        case DM603_MENU_PROC :
            /* �л��ӽ��� */
            if (FaceStatusBak[hmi_t_num] != FaceStatus[hmi_t_num])
            {
                disp_clr_screen(disp_buf);
                dm603_common_disp_flash();
                FaceStatusBak[hmi_t_num] = FaceStatus[hmi_t_num];
            }
            if(key_delay_time[hmi_t_num]>=KEY_DELAY)
            {
                switch (FaceStatus[hmi_t_num])
                {
                    case PARA_SELECT :  /* ������ */
                        switch(Main_Key[hmi_t_num])
                        {
                            case DM603_KEY_FUNC:
                                dm603_pop_face_status();
                                return value_opt;
//                              break;

                            case DM603_KEY_OK:
                                if (!para_is_nexch(ParamGroup, para_m_tbl[dm603_item[hmi_t_num]]))
                                {
                                    FaceStatus[hmi_t_num] = PARA_SETTING;
                                }
                                return value_opt;
//                              break;  /* �����л����� */

                            case DM603_KEY_UP:      /* �л�����һ������ */
                            case DM603_KEY_DOWN:    /* �л�����һ�� */
                                dm603_common_disp_clr_line(ROW3, ROW2, RNUL, RNUL, disp_buf);

                                if (Main_Key[hmi_t_num]==DM603_KEY_UP)
                                {
                                    dm603_item[hmi_t_num]=(dm603_item[hmi_t_num]> min_para_m)?(--dm603_item[hmi_t_num]):(max_para_m - 1);
                                }
                                else if (Main_Key[hmi_t_num]==DM603_KEY_DOWN)
                                {
                                    dm603_item[hmi_t_num]=(dm603_item[hmi_t_num]< (max_para_m-1))?(++dm603_item[hmi_t_num]):min_para_m;
                                }
                                para_get_val(ParamGroup, para_m_tbl[dm603_item[hmi_t_num]], &value);
                                break;

                            default:
                                break;
                        }
                        break;

                    case PARA_SETTING : /* ����ֵ�� */
                        switch(Main_Key[hmi_t_num])
                        {
                            case DM603_KEY_FUNC:
                                para_get_val(ParamGroup, para_m_tbl[dm603_item[hmi_t_num]], &value);

                                FaceStatus[hmi_t_num] = PARA_SELECT;
                                return value_opt;
//                              break;

                            case DM603_KEY_OK:
                                para_set(ParamGroup, para_m_tbl[dm603_item[hmi_t_num]], value);
                                FaceStatus[hmi_t_num] = PARA_SELECT;
                                break;

                            case DM603_KEY_UP:      /* ������ */
                                para_add(ParamGroup, para_m_tbl[dm603_item[hmi_t_num]], &value, 0);
                                break;

                            case DM603_KEY_DOWN:    /* ������ */
                                para_sub(ParamGroup, para_m_tbl[dm603_item[hmi_t_num]], &value, 0);
                                break;

                            default:
                                break;
                        }
                        break;

                    default :
                        break;
                }
            }
            /* break; */

        case DM603_MENU_PAINT :
            if (Main_Key[hmi_t_num] != KEY_NULL)
            {
                if (FaceStatus[hmi_t_num] == PARA_SELECT)   disp_clr_line(ROW2, disp_buf);
                else                                        disp_clr_screen(disp_buf);
                dm603_common_disp_flash();
            }

            switch (FaceStatus[hmi_t_num])
            {
                case PARA_SELECT :  /* ������ */
                    value_opt = PARA_OPT_M;
                    /* �˵��� */
                    dm603_common_disp_menu(BMP_FN, dm603_face_return, \
                                           BMP_OK, dm603_face_para_set_m_ent, disp_buf);
//                  break;

                case PARA_SETTING : /* ����ֵ */
                    if (FaceStatus[hmi_t_num] == PARA_SETTING)
                    {
                        value_opt = PARA_OPT_V;
                        /* �˵��� */
                        dm603_common_disp_menu(BMP_FN, dm603_face_para_set_v_ret, \
                                            BMP_OK, dm603_face_ok, disp_buf);
                    }


                    dm603_face_para_set_disp_v(value_opt, value, para_m_tbl[dm603_item[hmi_t_num]]+1, \
                                                para_get_sign(ParamGroup, para_m_tbl[dm603_item[hmi_t_num]]), \
                                               &para_msg[para_m_tbl[dm603_item[hmi_t_num]]], disp_buf);

                    break;

                default :
                    break;
            }
            break;
    }

    return value_opt;

}

/**
 * ������dm603_face_pass_modify()
 * ���� : �����޸�
 *
 * ���� : U08 Pass_toModify ��Ҫ�޸ĵ����뼶��
 * ���� : --
 *
 * ��ע : �ٸ��Ķ�Ӧ������������
 */

/*******************************************************************************
 �� ��: dm603_face_pass_modify()
 �� ��: �����޸Ľ��棬����ָ�������뼶���޸�����
 �� ��: U08 Pass_toModify ��Ҫ�޸ĵ����뼶��
 �� ��: --
 �� ��: --
*******************************************************************************/
static void dm603_face_pass_modify(U08 Pass_toModify,U08 *disp_buf)
{
    U08 i = 0;

    static U08 *pass_buf_m;         /* ����ָ�� */
    static U08 pass_ok;             /* ������� */
    static U08 cursor;
    U08 menu_opt = PASS_CHG_UP;

    const U16 msg_pass[] =
    {
        dm603_face_para_pass_m_old, /* {�����������:}   */
        dm603_face_para_pass_m_new, /* {������������:}   */
        dm603_face_para_pass_m_agn, /* {�ٴ�����������:} */
    };

    switch (dm603_menu[hmi_t_num])
    {
        case DM603_MENU_CREATE :
            dm603_menu[hmi_t_num] = DM603_MENU_PROC;
            key_delay_time[hmi_t_num]=0;
            dm603_item[hmi_t_num] = 0;
            cursor = 0;

            /* ������� */
            pass_buf_m = dm603_pass_buf;
            dm603_face_pass_clr(dm603_pass_buf);
            dm603_face_pass_clr(dm603_pass_new);
            dm603_face_pass_clr(dm603_pass_bak);

            dm603_common_disp_flash();
            disp_clr_screen(disp_buf);
            /* break; */

        case DM603_MENU_PROC :
            if(key_delay_time[hmi_t_num]>=KEY_DELAY)
            {
                switch(Main_Key[hmi_t_num])
                {
                    case DM603_KEY_OK:
                        switch (dm603_item[hmi_t_num])
                        {
                            case 0 :    /* ��������� */
                                cursor = 0;
                                pass_ok = dm603_face_pass_cmp(pass_buf_m, (U08 *)password[Pass_toModify]);

                                if ((Pass_toModify == PASS_USER) && (pass_ok == 0)) // �û�������ɳ���������ĺ�ά��������
                                {
                                    pass_ok = dm603_face_pass_cmp(pass_buf_m, (U08 *)password[PASS_SER]);
                                    pass_ok |= dm603_face_pass_cmp(pass_buf_m, (U08 *)password[PASS_FAC]);

                                }
                                else if ((Pass_toModify == PASS_FAC) && (pass_ok == 0)) // ����������ɶ�̬�������
                                {
                                    pass_ok = dm603_face_pass_cmp(pass_buf_m, (U08 *)password[PASS_SUPER]);
                                }
                                else if ((Pass_toModify == PASS_SER) && (pass_ok == 0)) // ά��������ɳ����������
                                {
                                    pass_ok = dm603_face_pass_cmp(pass_buf_m, (U08 *)password[PASS_FAC]);
                                }



                                if (pass_ok)    dm603_item[hmi_t_num] = 1;
                                else            dm603_item[hmi_t_num] = 3;
                                pass_buf_m = dm603_pass_new;
                                break;

                            case 1 :    /* ���������� */
                                cursor = 0;
                                dm603_item[hmi_t_num] = 2;
                                pass_buf_m = dm603_pass_bak;
                                break;

                            case 2 :    /* �ٴ����������� */
                                cursor = 0;
                                pass_ok = dm603_face_pass_cmp(pass_buf_m, dm603_pass_new);
                                dm603_item[hmi_t_num] = 4;

                                /* ����������*/
                                if (pass_ok)
                                {
                                    pass_buf_m = (U08 *)password[Pass_toModify];
                                    for (i = 0; i < MAX_PASS_BIT; i++)
                                    {
                                        pass_buf_m[i] = dm603_pass_new[i];
                                    }

                                    SET_BIT(passwd_modify, Pass_toModify);
                                    SET_BIT(passwd_broadcast, Pass_toModify);
                                }
                                break;

                            case 3 :    /* ��������� */
                            case 4 :    /* ��������� */
                                if (!pass_ok)   dm603_menu[hmi_t_num] = DM603_MENU_CREATE;  /* �������� */
                                break;
                        }
                        break;

                    case DM603_KEY_FUNC: /* �������������λ */
                        if (dm603_item[hmi_t_num] < 3)
                        {
                            if (++cursor >= MAX_PASS_BIT)   cursor = 0;
                        }
                        else
                        {
                            dm603_pop_face_status();    /* �ָ��ϲ����״̬ */
                            return;
                        }
                        break;

                    /* ������� */
                    case DM603_KEY_UP:
                        menu_opt = PASS_CHG_UP;
    //                  break;

                    case DM603_KEY_DOWN:
                        if (Main_Key[hmi_t_num] == DM603_KEY_DOWN)  menu_opt = PASS_CHG_DOWN;
                        dm603_face_pass_chg(&pass_buf_m[cursor], menu_opt);
                        break;
                }
            }
            /* break; */

        case DM603_MENU_PAINT :
            if (Main_Key[hmi_t_num] != KEY_NULL)
            {
                dm603_common_disp_clr_line(ROW1, ROW2, RNUL, RNUL, disp_buf);
                dm603_common_disp_flash();
            }

            if (dm603_item[hmi_t_num] < 3)      /* ��ʾ���� */
            {
                disp_string(msg_pass[dm603_item[hmi_t_num]], ROW1, 0, DCS_NORMAL, disp_buf);
                /* ��ʾ���� */
                dm603_face_pass_disp(cursor, pass_buf_m[cursor], ROW2, disp_buf);
                /* �˵��� */
                dm603_common_disp_menu(BMP_FN, dm603_face_para_pass_m_ret, \
                                       BMP_OK, dm603_face_para_pass_m_ent, disp_buf);
            }
            else                        /* ��ʾ��ʾ��Ϣ */
            {
                if (dm603_item[hmi_t_num] == 3) /* ������������� */
                {
                    disp_string(dm603_face_para_pass_m_err_old, ROW1, 0, DCS_NORMAL | DTS_CENTER, disp_buf);
                    disp_string(dm603_face_para_pass_m_err, ROW2, 0, DCS_NORMAL | DTS_CENTER, disp_buf);

                    /* �˵��� */
                    dm603_common_disp_menu(BMP_FN, dm603_face_return, \
                                           BMP_OK, dm603_face_para_pass_m_err_ent, disp_buf);
                }
                else    /* ���������� */
                {
                    if (pass_ok)    /* ��������ȷ */
                    {
                        disp_string(dm603_face_para_pass_m_yes_agn, ROW1, 0, DCS_NORMAL | DTS_CENTER, disp_buf);
                        /* �˵��� */
                        dm603_common_disp_menu(BMP_FN, dm603_face_return, \
                                               BMP_NU, NULL, disp_buf);
                    }
                    else                    /* ��������� */
                    {
                        disp_string(dm603_face_para_pass_m_err_agn, ROW1, 0, DCS_NORMAL | DTS_CENTER, disp_buf);
                        disp_string(dm603_face_para_pass_m_err, ROW2, 0, DCS_NORMAL | DTS_CENTER, disp_buf);
                        /* �˵��� */
                        dm603_common_disp_menu(BMP_FN, dm603_face_return, \
                                               BMP_OK, dm603_face_para_pass_m_err_ent, disp_buf);
                    }
                }
            }
            break;
    }
}



/*******************************************************************************
 �� ��: dm603_face_para_initP()
 �� ��: ��ʼ����������
 �� ��: --
 �� ��: --
 �� ��: --
*******************************************************************************/

static void dm603_face_para_initP(U08 *disp_buf)
{
    static BOOL fg_init_act = FALSE;    /* �Ƿ��ѿ�ʼ��ʼ�� */

    switch (dm603_menu[hmi_t_num])
    {
        case DM603_MENU_CREATE :
            dm603_menu[hmi_t_num] = DM603_MENU_PROC;
            key_delay_time[hmi_t_num]=0;
            fg_init_act = FALSE;
            dm603_common_disp_flash();
            /* break; */

        case DM603_MENU_PROC :
            if(key_delay_time[hmi_t_num]>=KEY_DELAY)
            {
                switch(Main_Key[hmi_t_num])
                {
                    case DM603_KEY_FUNC:
                        dm603_pop_face_status();
                        fg_init_act=FALSE;

                        return;
//                      break;

                    case DM603_KEY_OK:
                        if(!fg_init_act && (STEP_IS_IDLE || STEP_IS_ALARM))
                        {
                            DM603_CMD_PARA_INIT;        /* ���Ҳ�����ʼ�� */
                            fg_init_act=TRUE;
                        }

    //                  return;
                        break;

                    default:
                        break;
                }
            }
            /* break; */

        case DM603_MENU_PAINT :
            if (Main_Key[hmi_t_num] != KEY_NULL)
            {
                disp_clr_screen(disp_buf);
                dm603_common_disp_flash();
            }

            if (fg_init_act)        /* �ѽ����ʼ�� */
            {
                switch (GET_INIT_PARA())
                {
                    case ACTION_IDLE  : /* break; */
                    case ACTION_START :
                    case ACTION_ING :
                        disp_string(dm603_face_para_init_now, ROW2, 0, \
                                    DTS_CENTER | DTS_CLR_HEAD | DTS_CLR_TRAIL | DCS_FLASH, disp_buf);
                        break;

                    case ACTION_SUCC : /* �ɹ� */
                        disp_string(dm603_face_para_init_yes, ROW2, 0, \
                                    DTS_CENTER | DTS_CLR_HEAD | DTS_CLR_TRAIL | DCS_FLASH, disp_buf);
                        break;

                    case ACTION_FAIL : /* ʧ�� */
                        disp_string(dm603_face_para_init_err, ROW2, 0, \
                                    DTS_CENTER | DTS_CLR_HEAD | DTS_CLR_TRAIL | DCS_FLASH, disp_buf);
                        break;
                }

                /* �˵��� */
                dm603_common_disp_menu(BMP_FN, dm603_face_return,   BMP_NU, NULL,  disp_buf);
            }
            else    /* ѯ���Ƿ��ʼ�� */
            {
                disp_string(dm603_face_query_init, ROW1, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);
                if(!(STEP_IS_IDLE || STEP_IS_ALARM))
                {
                    disp_string(dm603_face_operation_tip, ROW2, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);
                }

                /* �˵��� */
                dm603_common_disp_menu(BMP_FN, dm603_face_query_ret,    BMP_OK, dm603_face_ok,  disp_buf);
            }
            break;

        default:
            break;
    }
}

/*******************************************************************************
 �� ��: dm603_face_para_initC()
 �� ��: ��ʼ��ѹ��ʱ�����
 �� ��: --
 �� ��: --
 �� ��: --
*******************************************************************************/
static void dm603_face_para_initC(U08 *disp_buf)
{
    static BOOL fg_init_act = FALSE;    /* �Ƿ��ѿ�ʼ��ʼ�� */

    switch (dm603_menu[hmi_t_num])
    {
        case DM603_MENU_CREATE :
            dm603_menu[hmi_t_num] = DM603_MENU_PROC;
            key_delay_time[hmi_t_num]=0;
            fg_init_act = FALSE;
            dm603_common_disp_flash();
            /* break; */

        case DM603_MENU_PROC :
            if(key_delay_time[hmi_t_num]>=KEY_DELAY)
            {
                switch(Main_Key[hmi_t_num])
                {
                    case DM603_KEY_FUNC:
                        dm603_pop_face_status();
                        fg_init_act=FALSE;

                        return;
//                      break;

                    case DM603_KEY_OK:
                        if(!fg_init_act && (STEP_IS_IDLE || STEP_IS_ALARM))
                        {
                            DM603_CMD_RUNTIME_INIT;     /* ѹ���ۼ�����ʱ���ʼ�� */
                            fg_init_act=TRUE;
                        }

    //                  return;
                        break;

                    default:
                        break;
                }
            }
            /* break; */

        case DM603_MENU_PAINT :
            if (Main_Key[hmi_t_num] != KEY_NULL)
            {
                disp_clr_screen(disp_buf);
                dm603_common_disp_flash();
            }

            if (fg_init_act)        /* �ѽ����ʼ�� */
            {
                disp_string(dm603_face_para_init_yes, ROW2, 0, \
                                DTS_CENTER | DTS_CLR_HEAD | DTS_CLR_TRAIL | DCS_FLASH, disp_buf);

                /* �˵��� */
                dm603_common_disp_menu(BMP_FN, dm603_face_return,   BMP_NU, NULL,  disp_buf);
            }
            else    /* ѯ���Ƿ��ʼ�� */
            {
                disp_string(dm603_face_query_init, ROW1, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);
                if(!(STEP_IS_IDLE || STEP_IS_ALARM))
                {
                    disp_string(dm603_face_operation_tip, ROW2, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);
                }

                /* �˵��� */
                dm603_common_disp_menu(BMP_FN, dm603_face_query_ret,    BMP_OK, dm603_face_ok,  disp_buf);
            }
            break;

        default:
            break;
    }
}

/*******************************************************************************
 �� ��: dm603_face_sysruntime_init()
 �� ��: ��ʼ��ϵͳ�ۼ�����ʱ�����
 �� ��: --
 �� ��: --
 �� ��: --
*******************************************************************************/

static void dm603_face_sysruntime_init(U08 *disp_buf)
{
    static BOOL fg_init_act = FALSE;    /* �Ƿ��ѿ�ʼ��ʼ�� */

    switch (dm603_menu[hmi_t_num])
    {
        case DM603_MENU_CREATE :
            dm603_menu[hmi_t_num] = DM603_MENU_PROC;
            key_delay_time[hmi_t_num]=0;
            fg_init_act = FALSE;
            dm603_common_disp_flash();
            /* break; */

        case DM603_MENU_PROC :
            if(key_delay_time[hmi_t_num]>=KEY_DELAY)
            {
                switch(Main_Key[hmi_t_num])
                {
                    case DM603_KEY_FUNC:
                        dm603_pop_face_status();
                        fg_init_act=FALSE;

                        return;
//                      break;

                    case DM603_KEY_OK:
                        if(!fg_init_act && (STEP_IS_IDLE || STEP_IS_ALARM))
                        {
                            DM603_CMD_SYSTIME_INIT;     /* ϵͳ�ۼ�����ʱ���ʼ�� */
                            fg_init_act=TRUE;
                        }

    //                  return;
                        break;

                    default:
                        break;
                }
            }
            /* break; */

        case DM603_MENU_PAINT :
            if (Main_Key[hmi_t_num] != KEY_NULL)
            {
                disp_clr_screen(disp_buf);
                dm603_common_disp_flash();
            }

            if (fg_init_act)        /* �ѽ����ʼ�� */
            {
                disp_string(dm603_face_para_init_yes, ROW2, 0, \
                                DTS_CENTER | DTS_CLR_HEAD | DTS_CLR_TRAIL | DCS_FLASH, disp_buf);

                /* �˵��� */
                dm603_common_disp_menu(BMP_FN, dm603_face_return,   BMP_NU, NULL,  disp_buf);
            }
            else    /* ѯ���Ƿ��ʼ�� */
            {
                disp_string(dm603_face_query_init, ROW1, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);
                if(!(STEP_IS_IDLE || STEP_IS_ALARM))
                {
                    disp_string(dm603_face_operation_tip, ROW2, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);
                }

                /* �˵��� */
                dm603_common_disp_menu(BMP_FN, dm603_face_query_ret,    BMP_OK, dm603_face_ok,  disp_buf);
            }
            break;

        default:
            break;
    }
}

/*******************************************************************************
 �� ��: dm603_face_limit_init()
 �� ��: ��ʼ��ϵͳά����������
 �� ��: --
 �� ��: --
 �� ��: --
*******************************************************************************/
static void dm603_face_limit_init(U08 *disp_buf)
{
    static BOOL fg_init_act = FALSE;    /* �Ƿ��ѿ�ʼ��ʼ�� */

    switch (dm603_menu[hmi_t_num])
    {
        case DM603_MENU_CREATE :
            dm603_menu[hmi_t_num] = DM603_MENU_PROC;
            key_delay_time[hmi_t_num]=0;
            fg_init_act = FALSE;
            dm603_common_disp_flash();
            /* break; */

        case DM603_MENU_PROC :
            if(key_delay_time[hmi_t_num]>=KEY_DELAY)
            {
                switch(Main_Key[hmi_t_num])
                {
                    case DM603_KEY_FUNC:
                        dm603_pop_face_status();
                        fg_init_act=FALSE;

                        return;
//                      break;

                    case DM603_KEY_OK:
                        if(!fg_init_act)
                        {
                            DM603_CMD_LIMIT_INIT;       /* ���Ҳ�����ʼ�� */
                            fg_init_act=TRUE;
                        }

    //                  return;
                        break;

                    default:
                        break;
                }
            }
            /* break; */

        case DM603_MENU_PAINT :
            if (Main_Key[hmi_t_num] != KEY_NULL)
            {
                disp_clr_screen(disp_buf);
                dm603_common_disp_flash();
            }

            if (fg_init_act)        /* �ѽ����ʼ�� */
            {
                switch (GET_INIT_PARA())
                {
                    case ACTION_IDLE  : /* break; */
                    case ACTION_START :
                    case ACTION_ING :
                        disp_string(dm603_face_para_init_now, ROW2, 0, \
                                    DTS_CENTER | DTS_CLR_HEAD | DTS_CLR_TRAIL | DCS_FLASH, disp_buf);
                        break;

                    case ACTION_SUCC : /* �ɹ� */
                        disp_string(dm603_face_para_init_yes, ROW2, 0, \
                                    DTS_CENTER | DTS_CLR_HEAD | DTS_CLR_TRAIL | DCS_FLASH, disp_buf);
                        break;

                    case ACTION_FAIL : /* ʧ�� */
                        disp_string(dm603_face_para_init_err, ROW2, 0, \
                                    DTS_CENTER | DTS_CLR_HEAD | DTS_CLR_TRAIL | DCS_FLASH, disp_buf);
                        break;
                }

                /* �˵��� */
                dm603_common_disp_menu(BMP_FN, dm603_face_return,   BMP_NU, NULL,  disp_buf);
            }
            else    /* ѯ���Ƿ��ʼ�� */
            {
                disp_string(dm603_face_query_init, ROW1, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);

                /* �˵��� */
                dm603_common_disp_menu(BMP_FN, dm603_face_query_ret,    BMP_OK, dm603_face_ok,  disp_buf);
            }
            break;

        default:
            break;
    }
}

/*******************************************************************************
 �� ��: dm603_face_para_initInvt()
 �� ��: ��ʼ����Ƶ������
 �� ��: --
 �� ��: --
 �� ��: --
*******************************************************************************/
static void dm603_face_para_initInvt(U08 *disp_buf)
{
    static BOOL fg_init_act = FALSE;    /* �Ƿ��ѿ�ʼ��ʼ�� */

    switch (dm603_menu[hmi_t_num])
    {
        case DM603_MENU_CREATE :
            dm603_menu[hmi_t_num] = DM603_MENU_PROC;
            key_delay_time[hmi_t_num]=0;
            fg_init_act = FALSE;
            dm603_common_disp_flash();
            /* break; */

        case DM603_MENU_PROC :
            if(key_delay_time[hmi_t_num]>=KEY_DELAY)
            {
                switch(Main_Key[hmi_t_num])
                {
                    case DM603_KEY_FUNC:
                        dm603_pop_face_status();
                        fg_init_act=FALSE;

                        return;
//                      break;

                    case DM603_KEY_OK:
                        if(!fg_init_act && (STEP_IS_IDLE || STEP_IS_ALARM))
                        {
                            DM603_CMD_INVT_INIT;        /* ��Ƶ����ʼ�� */
                            fg_init_act=TRUE;
                        }

//                      return;
                        break;

                    default:
                        break;
                }
            }
            /* break; */

        case DM603_MENU_PAINT :
            if (Main_Key[hmi_t_num] != KEY_NULL)
            {
                disp_clr_screen(disp_buf);
                dm603_common_disp_flash();
            }

            if (fg_init_act)        /* �ѽ����ʼ�� */
            {
                disp_string(dm603_face_para_init_cmd, ROW2, 0, \
                                DTS_CENTER | DTS_CLR_HEAD | DTS_CLR_TRAIL | DCS_FLASH, disp_buf);

                /* �˵��� */
                dm603_common_disp_menu(BMP_FN, dm603_face_return,   BMP_NU, NULL,  disp_buf);
            }
            else    /* ѯ���Ƿ��ʼ�� */
            {
                disp_string(dm603_face_query_init, ROW1, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);
                if(!(STEP_IS_IDLE || STEP_IS_ALARM))
                {
                    disp_string(dm603_face_operation_tip, ROW2, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);
                }

                /* �˵��� */
                dm603_common_disp_menu(BMP_FN, dm603_face_query_ret,    BMP_OK, dm603_face_ok,  disp_buf);
            }
            break;

        default:
            break;
    }
}

/*******************************************************************************
 �� ��: dm603_face_enter_ability_test()
 �� ��: ��������
 �� ��: --
 �� ��: --
 �� ��: --
*******************************************************************************/
static void dm603_face_enter_ability_test(U08 *disp_buf)
{
    static BOOL fg_enter = FALSE;

    switch (dm603_menu[hmi_t_num])
    {
        case DM603_MENU_CREATE :
            dm603_menu[hmi_t_num] = DM603_MENU_PROC;
            key_delay_time[hmi_t_num]=0;
            fg_enter = FALSE;
            dm603_common_disp_flash();
            /* break; */

        case DM603_MENU_PROC :
            if(key_delay_time[hmi_t_num]>=KEY_DELAY)
            {
                switch(Main_Key[hmi_t_num])
                {
                    case DM603_KEY_FUNC:
                        dm603_pop_face_status();
                        fg_enter=FALSE;

                        return;
//                      break;

                    case DM603_KEY_OK:
                        if(!fg_enter)
                        {
                            DM603_CMD_ABILITY_TEST;
                            fg_enter=TRUE;
                        }

//                      return;
                        break;

                    default:
                        break;
                }
            }
            /* break; */

        case DM603_MENU_PAINT :
            if (Main_Key[hmi_t_num] != KEY_NULL)
            {
                disp_clr_screen(disp_buf);
                dm603_common_disp_flash();
            }

            if (fg_enter)
            {
                disp_string(dm603_face_para_init_cmd, ROW2, 0, \
                                DTS_CENTER | DTS_CLR_HEAD | DTS_CLR_TRAIL | DCS_FLASH, disp_buf);

                /* �˵��� */
                dm603_common_disp_menu(BMP_FN, dm603_face_return,   BMP_NU, NULL,  disp_buf);
            }
            else    /* ѯ�� */
            {
                disp_string(dm603_face_query_enter_ability_test, ROW1, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);

                /* �˵��� */
                dm603_common_disp_menu(BMP_FN, dm603_face_query_ret,    BMP_OK, dm603_face_ok,  disp_buf);
            }
            break;

        default:
            break;
    }
}

#if 0
/*******************************************************************************
 �� ��: dm603_face_enter_floor_dryup()
 �� ��: ��ů��ɹ���
 �� ��: disp_buf--;
 �� ��:
 �� ��: ��
*******************************************************************************/
static void dm603_face_enter_floor_dryup(U08 *disp_buf)
{
    static BOOL fg_entered = FALSE; /* �Ƿ��ѽ��� */

    switch (dm603_menu[hmi_t_num])
    {
        case DM603_MENU_CREATE :
            dm603_menu[hmi_t_num] = DM603_MENU_PROC;
            key_delay_time[hmi_t_num]=0;
            fg_entered = FALSE;
            dm603_common_disp_flash();
            /* break; */

        case DM603_MENU_PROC :
            if(key_delay_time[hmi_t_num]>=KEY_DELAY)
            {
                switch(Main_Key[hmi_t_num])
                {
                    case DM603_KEY_FUNC:
                        dm603_pop_face_status();
                        fg_entered=FALSE;

                        return;
//                      break;

                    case DM603_KEY_OK:

                        if(!fg_entered
                            && (val_air_type != HOT_WATER_ONLY)    // ��������ģʽ
                            && (val_air_type != COOL_ONLY)
                        )
                        {
                            DM603_CMD_RNTER_FLOOR_DRYUP;        /* �������� */
                            fg_entered=TRUE;
                        }

//                      return;
                        break;

                    default:
                        break;
                }
            }
            /* break; */

        case DM603_MENU_PAINT :
            if (Main_Key[hmi_t_num] != KEY_NULL)
            {
                disp_clr_screen(disp_buf);
                dm603_common_disp_flash();
            }

            if ((val_air_type == HOT_WATER_ONLY)   // ����������ģʽ
                || (val_air_type == COOL_ONLY)
            )
            {
                disp_string(dm603_face_enter_floor_dryup5, ROW1, 0, \
                                DTS_CENTER | DTS_CLR_HEAD | DTS_CLR_TRAIL | DCS_FLASH, disp_buf);
                /* �˵��� */
                dm603_common_disp_menu(BMP_FN, dm603_face_return,   BMP_NU, NULL,  disp_buf);
            }
            else if (fg_entered)        /* �ѽ��� */
            {
                disp_string(dm603_face_enter_floor_dryup3, ROW1, 0, \
                                DTS_CENTER | DTS_CLR_HEAD | DTS_CLR_TRAIL | DCS_FLASH, disp_buf);
                disp_string(dm603_face_enter_floor_dryup4, ROW2, 0, \
                                DTS_CENTER | DTS_CLR_HEAD | DTS_CLR_TRAIL | DCS_FLASH, disp_buf);
                /* �˵��� */
                dm603_common_disp_menu(BMP_FN, dm603_face_return,   BMP_NU, NULL,  disp_buf);
            }
            else    /* ѯ�� */
            {
                disp_string(dm603_face_enter_floor_dryup1, ROW1, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);
                disp_string(dm603_face_enter_floor_dryup2, ROW2, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);

                /* �˵��� */
                dm603_common_disp_menu(BMP_FN, dm603_face_query_ret,    BMP_OK, dm603_face_ok,  disp_buf);
            }
            break;

        default:
            break;
    }
}
#endif

/*******************************************************************************
 �� ��: dm603_face_force_ibh()
 �� ��: ǿ�����õ���
 �� ��: disp_buf--;
 �� ��:
 �� ��: ��
*******************************************************************************/
static void dm603_face_force_ibh(U08 *disp_buf)
{
    static BOOL fg_entered = FALSE; /* �Ƿ��ѽ��� */

    switch (dm603_menu[hmi_t_num])
    {
        case DM603_MENU_CREATE :
            dm603_menu[hmi_t_num] = DM603_MENU_PROC;
            key_delay_time[hmi_t_num]=0;
            fg_entered = FALSE;
            dm603_common_disp_flash();
            /* break; */

        case DM603_MENU_PROC :
            if(key_delay_time[hmi_t_num]>=KEY_DELAY)
            {
                switch(Main_Key[hmi_t_num])
                {
                    case DM603_KEY_FUNC:
                        dm603_pop_face_status();
                        fg_entered=FALSE;

                        return;
//                      break;

                    case DM603_KEY_OK:

//                        if((!fg_entered)
//                            && (STEP_IS_RUN)                        // ����״̬��
//                            && ((fg_hotw_need && HOTW_USE_IBH && !val_TBH_used)   // ��ˮǿ�ƿ�
//                                || (fg_heat_need && HEAT_USE_IBH))// ����ǿ�ƿ�
//                        )
//                        {
//                            DM603_CMD_FORCE_IBH;        /* �������� */
//                            fg_entered=TRUE;
//                        }

//                      return;
                        break;

                    default:
                        break;
                }
            }
            /* break; */

        case DM603_MENU_PAINT :
            if (Main_Key[hmi_t_num] != KEY_NULL)
            {
                disp_clr_screen(disp_buf);
                dm603_common_disp_flash();
            }

            if ((!STEP_IS_RUN)
//                || (MODE_IS_COOL)
//                || (MODE_IS_HOTW && (!HOTW_USE_IBH || val_TBH_used))
//                || (MODE_IS_HEAT && !HEAT_USE_IBH)
            )
            {
                disp_string(dm603_face_force3, ROW1, 0, \
                                DTS_CENTER | DTS_CLR_HEAD | DTS_CLR_TRAIL | DCS_FLASH, disp_buf);
                /* �˵��� */
                dm603_common_disp_menu(BMP_FN, dm603_face_return,   BMP_NU, NULL,  disp_buf);
            }
            else if (fg_entered)        /* �ѽ��� */
            {
                disp_string(dm603_face_force2, ROW1, 0, \
                                DTS_CENTER | DTS_CLR_HEAD | DTS_CLR_TRAIL | DCS_FLASH, disp_buf);
                /* �˵��� */
                dm603_common_disp_menu(BMP_FN, dm603_face_return,   BMP_NU, NULL,  disp_buf);
            }
            else    /* ѯ�� */
            {
                disp_string(dm603_face_force1, ROW1, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);

                /* �˵��� */
                dm603_common_disp_menu(BMP_FN, dm603_face_query_ret,    BMP_OK, dm603_face_ok,  disp_buf);
            }
            break;

        default:
            break;
    }
}

/*******************************************************************************
 �� ��: dm603_face_force_tbh()
 �� ��: ǿ�ƿ�ˮ������
 �� ��: disp_buf--;
 �� ��:
 �� ��: ��
*******************************************************************************/
static void dm603_face_force_tbh(U08 *disp_buf)
{
    static BOOL fg_entered = FALSE; /* �Ƿ��ѽ��� */

    switch (dm603_menu[hmi_t_num])
    {
        case DM603_MENU_CREATE :
            dm603_menu[hmi_t_num] = DM603_MENU_PROC;
            key_delay_time[hmi_t_num]=0;
            fg_entered = FALSE;
            dm603_common_disp_flash();
            /* break; */

        case DM603_MENU_PROC :
            if(key_delay_time[hmi_t_num]>=KEY_DELAY)
            {
                switch(Main_Key[hmi_t_num])
                {
                    case DM603_KEY_FUNC:
                        dm603_pop_face_status();
                        fg_entered=FALSE;

                        return;
//                      break;

                    case DM603_KEY_OK:

//                        if((!fg_entered)
//                            && (STEP_IS_RUN)                        // ����״̬��
//                            && (fg_hotw_need && val_TBH_used)  // ��ˮǿ�ƿ�
//                        )
//                        {
//                            DM603_CMD_FORCE_TBH;        /* �������� */
//                            fg_entered=TRUE;
//                        }
//
//                      return;
                        break;

                    default:
                        break;
                }
            }
            /* break; */

        case DM603_MENU_PAINT :
            if (Main_Key[hmi_t_num] != KEY_NULL)
            {
                disp_clr_screen(disp_buf);
                dm603_common_disp_flash();
            }

            if ((!STEP_IS_RUN)
//                || (!fg_hotw_need)
                || (!val_TBH_used)
            )
            {
                disp_string(dm603_face_force3, ROW1, 0, \
                                DTS_CENTER | DTS_CLR_HEAD | DTS_CLR_TRAIL | DCS_FLASH, disp_buf);
                /* �˵��� */
                dm603_common_disp_menu(BMP_FN, dm603_face_return,   BMP_NU, NULL,  disp_buf);
            }
            else if (fg_entered)        /* �ѽ��� */
            {
                disp_string(dm603_face_force2, ROW1, 0, \
                                DTS_CENTER | DTS_CLR_HEAD | DTS_CLR_TRAIL | DCS_FLASH, disp_buf);
                /* �˵��� */
                dm603_common_disp_menu(BMP_FN, dm603_face_return,   BMP_NU, NULL,  disp_buf);
            }
            else    /* ѯ�� */
            {
                disp_string(dm603_face_force1, ROW1, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);

                /* �˵��� */
                dm603_common_disp_menu(BMP_FN, dm603_face_query_ret,    BMP_OK, dm603_face_ok,  disp_buf);
            }
            break;

        default:
            break;
    }
}

#if 0
/*******************************************************************************
 �� ��: dm603_face_force_hot_water()
 �� ��: ǿ������ˮ(��������ˮ)
 �� ��: disp_buf--;
 �� ��:
 �� ��: ��
*******************************************************************************/
static void dm603_face_force_hot_water(U08 *disp_buf)
{
    static BOOL fg_entered = FALSE; /* �Ƿ��ѽ��� */

    switch (dm603_menu[hmi_t_num])
    {
        case DM603_MENU_CREATE :
            dm603_menu[hmi_t_num] = DM603_MENU_PROC;
            key_delay_time[hmi_t_num]=0;
            fg_entered = FALSE;
            dm603_common_disp_flash();
            /* break; */

        case DM603_MENU_PROC :
            if(key_delay_time[hmi_t_num]>=KEY_DELAY)
            {
                switch(Main_Key[hmi_t_num])
                {
                    case DM603_KEY_FUNC:
                        dm603_pop_face_status();
                        fg_entered=FALSE;

                        return;
//                      break;

                    case DM603_KEY_OK:

                        if((!fg_entered)
                            && (val_air_type == HOT_WATER_ONLY || val_air_type == REFR_HEAT_HW)
                        )
                        {
                            DM603_CMD_FAST_HOTW;      /* �������� */
                            fg_entered=TRUE;
                        }

//                      return;
                        break;

                    default:
                        break;
                }
            }
            /* break; */

        case DM603_MENU_PAINT :
            if (Main_Key[hmi_t_num] != KEY_NULL)
            {
                disp_clr_screen(disp_buf);
                dm603_common_disp_flash();
            }

            if (!(val_air_type == HOT_WATER_ONLY || val_air_type == REFR_HEAT_HW))
            {
                disp_string(dm603_face_force3, ROW1, 0, \
                                DTS_CENTER | DTS_CLR_HEAD | DTS_CLR_TRAIL | DCS_FLASH, disp_buf);
                /* �˵��� */
                dm603_common_disp_menu(BMP_FN, dm603_face_return,   BMP_NU, NULL,  disp_buf);
            }
            else if (fg_entered)        /* �ѽ��� */
            {
                disp_string(dm603_face_force2, ROW1, 0, \
                                DTS_CENTER | DTS_CLR_HEAD | DTS_CLR_TRAIL | DCS_FLASH, disp_buf);
                /* �˵��� */
                dm603_common_disp_menu(BMP_FN, dm603_face_return,   BMP_NU, NULL,  disp_buf);
            }
            else    /* ѯ�� */
            {
                disp_string(dm603_face_force1, ROW1, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);

                /* �˵��� */
                dm603_common_disp_menu(BMP_FN, dm603_face_query_ret,    BMP_OK, dm603_face_ok,  disp_buf);
            }
            break;

        default:
            break;
    }
}

/*******************************************************************************
 �� ��: dm603_face_holiday_mode_set()
 �� ��: ����ģʽ
 �� ��: disp_buf--;
 �� ��:
 �� ��: ��
*******************************************************************************/
static void dm603_face_holiday_mode_set(U08 *disp_buf)
{
    static U08 max = 0,min = 0;
    static U08 set_iteam = HOLIDAY_USED;
    static U08 val = 0;
    static const U16 used_or_unused[]={dm603_holiday_mode_unused,dm603_holiday_mode_used};
    static BOOL is_setting = FALSE;
    U16 attribute = 0;

    switch (dm603_menu[hmi_t_num])
    {
        case DM603_MENU_CREATE :
            dm603_menu[hmi_t_num] = DM603_MENU_PROC;
            key_delay_time[hmi_t_num]=0;
            set_iteam = HOLIDAY_USED;
            val = dm603_holiday_tbl[set_iteam];
            max = 1;
            min = 0;
            dm603_common_disp_flash();
            /* break; */

        case DM603_MENU_PROC :
            if(key_delay_time[hmi_t_num]>=KEY_DELAY)
            {
                if (val_air_type == COOL_ONLY) // ����ģʽ����ģʽ��Ч
                {
                    if (Main_Key[hmi_t_num] == DM603_KEY_FUNC)
                    {
                        dm603_pop_face_status();
                        return;
                    }
                }
                else
                {
                    if (is_setting)
                    {
                        switch(Main_Key[hmi_t_num])
                        {
                            case DM603_KEY_FUNC:
                                dm603_holiday_tbl[set_iteam] = val;
                                if (++set_iteam >= HOLIDAY_MAX)
                                {
                                    set_iteam = HOLIDAY_USED;
                                }
                                val = dm603_holiday_tbl[set_iteam];
                                DataAccess_holiday(DATA_WRITE);
                                break;

                            case DM603_KEY_OK:
                                is_setting = FALSE;
                                dm603_holiday_tbl[set_iteam] = val;
                                DataAccess_holiday(DATA_WRITE);
                                break;

                            case DM603_KEY_UP:
                                if (++val > max)
                                {
                                    val = min;
                                }
                                break;

                            case DM603_KEY_DOWN:
                                if (val-- <= min)
                                {
                                    val = max;
                                }
                                break;
                        }
                    }
                    else
                    {
                        switch(Main_Key[hmi_t_num])
                        {
                            case DM603_KEY_FUNC:
                                dm603_pop_face_status();
                                return;
//                              break;

                            case DM603_KEY_OK:
                                is_setting = TRUE;
                                break;

                            case DM603_KEY_UP:
                                if (set_iteam-- <= HOLIDAY_USED)
                                {
                                    set_iteam = HOLIDAY_END_DAY;
                                }
                                val = dm603_holiday_tbl[set_iteam];
                                break;

                            case DM603_KEY_DOWN:
                                if (++set_iteam >= HOLIDAY_MAX)
                                {
                                    set_iteam = HOLIDAY_USED;
                                }
                                val = dm603_holiday_tbl[set_iteam];
                                break;
                        }
                    }
                }
            }
            /* break; */

        case DM603_MENU_PAINT :
            if (val_air_type == COOL_ONLY)
            {
                disp_string(dm603_holiday_mode_not_exist, ROW1, 0, DCS_REVERSE, disp_buf);
                dm603_common_disp_menu(BMP_FN, dm603_face_return,   BMP_NU, NULL,  disp_buf);
            }
            else
            {
                if (Main_Key[hmi_t_num] != KEY_NULL)
                {
                    disp_clr_screen(disp_buf);
                    dm603_common_disp_flash();
                }

                switch (set_iteam)
                {
                    case HOLIDAY_USED:
                        min = 0; max = 1;
                        break;
                    case HOLIDAY_HEAT_SET:
                        min = 20; max = 25;
                        break;
                    case HOLIDAY_HW_SET:
                        min = 20; max = 25;
                        break;
                    case HOLIDAY_START_YEAR:
                        min = 0; max = 99;
                        break;
                    case HOLIDAY_START_MONTH:
                        min = 1; max = 12;
                        break;
                    case HOLIDAY_START_DAY:
                        min = 1; max = 31;
                        break;
                    case HOLIDAY_END_YEAR:
                        min = 0; max = 99;
                        break;
                    case HOLIDAY_END_MONTH:
                        min = 1; max = 12;
                        break;
                    case HOLIDAY_END_DAY:
                        min = 1; max = 31;
                        break;
                }
                attribute = is_setting ? DCS_BLINK : DCS_NORMAL;
                if (set_iteam >= HOLIDAY_USED && set_iteam <= HOLIDAY_HW_SET)
                {
                    // ����ģʽ:ʹ��
                    disp_string(dm603_holiday_mode, ROW1, COL_BGN, DCS_NORMAL, disp_buf);
                    if (set_iteam == HOLIDAY_USED)
                    {
                        disp_string(used_or_unused[val], ROW1, COL_END-56, attribute|DCS_REVERSE, disp_buf);
                    }
                    else
                    {
                        disp_string(used_or_unused[dm603_holiday_tbl[HOLIDAY_USED]], ROW1, COL_END-56, DCS_NORMAL, disp_buf);
                    }

                    // �����趨:  00 ��
                    disp_string(dm603_holiday_mode_heat_set, ROW2, COL_BGN, DCS_NORMAL, disp_buf);
                    if (set_iteam == HOLIDAY_HEAT_SET)
                    {
                        disp_value_U08(val, ROW2, COL_END-32, attribute|DNS_DIGIT2|DCS_REVERSE, disp_buf);
                        disp_string(dm603_holiday_mode_unit, ROW2, COL_END-16, DCS_NORMAL, disp_buf);
                    }
                    else
                    {
                        disp_value_U08(dm603_holiday_tbl[HOLIDAY_HEAT_SET], ROW2, COL_END-32, DNS_DIGIT2, disp_buf);
                        disp_string(dm603_holiday_mode_unit, ROW2, COL_END-16, DCS_NORMAL, disp_buf);
                    }

                    // ��ˮ�趨:  00 ��
                    disp_string(dm603_holiday_mode_hw_set, ROW3, COL_BGN, DCS_NORMAL, disp_buf);
                    if (set_iteam == HOLIDAY_HW_SET)
                    {
                        disp_value_U08(val, ROW3, COL_END-32, attribute|DNS_DIGIT2|DCS_REVERSE, disp_buf);
                        disp_string(dm603_holiday_mode_unit, ROW3, COL_END-16, DCS_NORMAL, disp_buf);
                    }
                    else
                    {
                        disp_value_U08(dm603_holiday_tbl[HOLIDAY_HW_SET], ROW3, COL_END-32, DNS_DIGIT2, disp_buf);
                        disp_string(dm603_holiday_mode_unit, ROW3, COL_END-16, DCS_NORMAL, disp_buf);
                    }
                }
                else
                {
                    // ��ʼ: 2022/01/01
                    disp_string(dm603_holiday_mode_start, ROW1, 0, DCS_NORMAL, disp_buf);
                    disp_value_U08(20, ROW1, 48, DNS_DIGIT2, disp_buf);
                    if (set_iteam == HOLIDAY_START_YEAR)
                    {
                        disp_value_U08(val, ROW1, 64, attribute|DNS_DIGIT2|DNS_ZERO|DCS_REVERSE, disp_buf);
                    }
                    else
                    {
                        disp_value_U08(dm603_holiday_tbl[HOLIDAY_START_YEAR], ROW1, 64, DNS_DIGIT2|DNS_ZERO, disp_buf);
                    }
                    disp_string_ptr8("/", ROW1, 80, DCS_NORMAL, disp_buf);
                    if (set_iteam == HOLIDAY_START_MONTH)
                    {
                        disp_value_U08(val, ROW1, 88, attribute|DNS_DIGIT2|DNS_ZERO|DCS_REVERSE, disp_buf);
                    }
                    else
                    {
                        disp_value_U08(dm603_holiday_tbl[HOLIDAY_START_MONTH], ROW1, 88, DNS_DIGIT2|DNS_ZERO, disp_buf);
                    }
                    disp_string_ptr8("/", ROW1, 104, DCS_NORMAL, disp_buf);
                    if (set_iteam == HOLIDAY_START_DAY)
                    {
                        disp_value_U08(val, ROW1, 112, attribute|DNS_DIGIT2|DNS_ZERO|DCS_REVERSE, disp_buf);
                    }
                    else
                    {
                        disp_value_U08(dm603_holiday_tbl[HOLIDAY_START_DAY], ROW1, 112, DNS_DIGIT2|DNS_ZERO, disp_buf);
                    }

                    // ����: 2022/01/01
                    disp_string(dm603_holiday_mode_end, ROW2, 0, DCS_NORMAL, disp_buf);
                    disp_value_U08(20, ROW2, 48, DNS_DIGIT2, disp_buf);
                    if (set_iteam == HOLIDAY_END_YEAR)
                    {
                        disp_value_U08(val, ROW2, 64, attribute|DNS_DIGIT2|DNS_ZERO|DCS_REVERSE, disp_buf);
                    }
                    else
                    {
                        disp_value_U08(dm603_holiday_tbl[HOLIDAY_END_YEAR], ROW2, 64, DNS_DIGIT2|DNS_ZERO, disp_buf);
                    }
                    disp_string_ptr8("/", ROW2, 80, DCS_NORMAL, disp_buf);
                    if (set_iteam == HOLIDAY_END_MONTH)
                    {
                        disp_value_U08(val, ROW2, 88, attribute|DNS_DIGIT2|DNS_ZERO|DCS_REVERSE, disp_buf);
                    }
                    else
                    {
                        disp_value_U08(dm603_holiday_tbl[HOLIDAY_END_MONTH], ROW2, 88, DNS_DIGIT2|DNS_ZERO, disp_buf);
                    }
                    disp_string_ptr8("/", ROW2, 104, DCS_NORMAL, disp_buf);
                    if (set_iteam == HOLIDAY_END_DAY)
                    {
                        disp_value_U08(val, ROW2, 112, attribute|DNS_DIGIT2|DNS_ZERO|DCS_REVERSE, disp_buf);
                    }
                    else
                    {
                        disp_value_U08(dm603_holiday_tbl[HOLIDAY_END_DAY], ROW2, 112, DNS_DIGIT2|DNS_ZERO, disp_buf);
                    }
                }
                if (is_setting)
                {
                    dm603_common_disp_menu(BMP_FN, dm603_face_menu_time_set_ent,    BMP_OK, dm603_face_ok,  disp_buf);
                }
                else
                {
                    dm603_common_disp_menu(BMP_FN, dm603_face_return,   BMP_OK, dm603_face_para_set_m_ent,  disp_buf);
                }
            }
            break;

        default:
            break;
    }
}
#endif

 /*******************************************************************************
 �� ��: dm603_face_error()
 �� ��: ���ϲ�ѯ����λ����
 �� ��: --
 �� ��: --
 �� ��: --
*******************************************************************************/
static void dm603_face_error(U08 *disp_buf)
{
    static U08 dm603_error_pcb;
    static U08 dm603_error_id;
    static U08 dm603_error_id_bak;

    U16 err_tag = 0;

    /* ���ڼ�ͷ��ʾ */
    U08 arrow_pcb_min;
    U08 arrow_err_min;
    U08 arrow_pcb_max;
    U08 arrow_err_max;

    arrow_pcb_min = menu_error_find_next(FIND_ERR_PCB_MIN, dm603_error_pcb, 0);
    arrow_pcb_max = menu_error_find_next(FIND_ERR_PCB_MAX, dm603_error_pcb, 0);

    if (dm603_menu[hmi_t_num] == DM603_MENU_CREATE)
    {
        dm603_error_pcb = arrow_pcb_min;
    }
    arrow_err_min = menu_error_find_next(FIND_ERR_MIN, dm603_error_pcb, 0);
    arrow_err_max = menu_error_find_next(FIND_ERR_MAX, dm603_error_pcb, 0);

    switch (dm603_menu[hmi_t_num])
    {
        case DM603_MENU_CREATE :
            dm603_menu[hmi_t_num] = DM603_MENU_PROC;
            key_delay_time[hmi_t_num]=0;
            dm603_error_id  = arrow_err_min;
            dm603_error_id_bak = dm603_error_id;
            dm603_common_disp_flash();
            /* break; */

        case DM603_MENU_PROC :

            /* δ��ѯ���κι���,���������� */
            if ((dm603_error_pcb == F_NO_ERR) && (dm603_error_id == F_NO_ERR)|| !fg_error)
            {
                dm603_face[hmi_t_num] = FACE_MAIN;
                return;
            }
            if(key_delay_time[hmi_t_num]>=KEY_DELAY)
            {
                /* ���������� */
                if ((Main_Key[hmi_t_num] == DM603_KEY_FUNC) )
                {
                    dm603_face[hmi_t_num] = FACE_MAIN;
                    return;
                }

                /* ��λ */
                if (Main_Key[hmi_t_num] == DM603_KEY_OK)
                {
                    if (fg_error) DM603_CMD_RESET;      /* ��λ */
                    dm603_menu[hmi_t_num] = DM603_MENU_CREATE;
                }

                /* ���ϻ����²�ѯ���� */
                if (Main_Key[hmi_t_num] == DM603_KEY_DOWN)
                {
                    dm603_common_disp_flash();

                    if (dm603_error_id_bak == arrow_err_max)    /* �ѷ�ҳ�����ֵ */
                    {
                        dm603_error_pcb = menu_error_find_next(FIND_ERR_PCB_NEXT, dm603_error_pcb, 0);
                        dm603_error_id  = menu_error_find_next(FIND_ERR_MIN, dm603_error_pcb, 0);

                    }
                    else
                    {
                        dm603_error_id  = menu_error_find_next(FIND_ERR_NEXT, dm603_error_pcb, dm603_error_id);
                    }
                    dm603_error_id_bak = dm603_error_id;
                    break;  // ��ֱ�ӵ�DM603_MENU_PAINT��������һģ��������С���ϺŻ�δ���¾��ж��Ƿ���ʾ��ͷ

                }
                if (Main_Key[hmi_t_num] == DM603_KEY_UP)
                {
                    dm603_common_disp_flash();

                    if (dm603_error_id_bak == arrow_err_min)    /* �ѷ�ҳ����Сֵ */
                    {
                        dm603_error_pcb = menu_error_find_next(FIND_ERR_PCB_PREV, dm603_error_pcb, 0);
                        dm603_error_id  = menu_error_find_next(FIND_ERR_MAX, dm603_error_pcb, 0);

                    }
                    else
                    {
                        dm603_error_id  = menu_error_find_next(FIND_ERR_PREV, dm603_error_pcb, dm603_error_id);
                    }
                    dm603_error_id_bak = dm603_error_id;
                    break;  // ��ֱ�ӵ�DM603_MENU_PAINT��������һģ��������С���ϺŻ�δ���¾��ж��Ƿ���ʾ��ͷ
                }

            }
            /* break; */

        case DM603_MENU_PAINT :     /* N0004 */
            {
                INVT_ID_ENUM id;
                for (id=INVT_ID_1; id<INVT_ID_MAX; id++)
                {
                    if (dm603_error_id == err_invt_id_to_err_id(id, INVT_ERR_ERROR))
                    {
                        err_tag = CELL_INVT_ERR(dm603_error_pcb, id);
                    }
                }
            }


            dm603_disp_err(dm603_error_pcb, dm603_error_id, err_tag, ROW1, disp_buf);

            /* ��ͷ��ʾ */
            if ((arrow_pcb_min == dm603_error_pcb) && (arrow_err_min == dm603_error_id))
            {
                if (arrow_pcb_min == arrow_pcb_max && arrow_err_max == arrow_err_min)
                {
                    dm603_common_disp_arrow(0, 0, 0, disp_buf); // ֻ��һ����ϲ���ʾ��ͷ
                }
                else
                {
                    dm603_common_disp_arrow(0, 0, 3, disp_buf);
                }
            }
            else if ((arrow_pcb_max == dm603_error_pcb) && (arrow_err_max == dm603_error_id))
            {
                dm603_common_disp_arrow(2, 0, 3, disp_buf);
            }
            else
            {
                dm603_common_disp_arrow(1, 0, 3, disp_buf);
            }

            /* �˵��� */
            dm603_common_disp_menu(BMP_FN, dm603_face_return, BMP_OK, dm603_face_error_reset, disp_buf);
            break;
    }
}

 /*******************************************************************************
 �� ��: dm603_face_limit_time()
 �� ��: ϵͳ�ۼ�����ʱ�䣨ϵͳά��ʱ�䣩��ѯ����
 �� ��: --
 �� ��: --
 �� ��: --
*******************************************************************************/

static void dm603_face_limit_time(U08 *disp_buf)
{
    switch (dm603_menu[hmi_t_num])
    {
        case DM603_MENU_CREATE :
            dm603_menu[hmi_t_num] = DM603_MENU_PROC;
            key_delay_time[hmi_t_num]=0;
            dm603_common_disp_flash();
            disp_clr_screen(disp_buf);

            /* break; */

        case DM603_MENU_PROC :
            if(key_delay_time[hmi_t_num]>=KEY_DELAY)
            {
                if (Main_Key[hmi_t_num] == DM603_KEY_FUNC) // �����ϲ����
                {
                    dm603_pop_face_status();    /* �ָ��ϲ����״̬ */
                    return;
                }
            }

        case DM603_MENU_PAINT :
            disp_string(dm603_face_limit_select_03, ROW1, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);
            dm603_common_disp_val(cell[MAX_PCB].cell_runtime[SYS_RUNTIME]/24, \
                                        ROW2, COL_END - COL_ASC, UINT_D, DCS_NORMAL, disp_buf);

            /* �˵��� */
            dm603_common_disp_menu(BMP_FN, dm603_face_return, \
                                   BMP_NU, NULL, disp_buf);
            break;
    }
}


/*******************************************************************************
 �� ��: dm603_face_pre()
 �� ��: ϵͳά�����ѽ���
 �� ��: --
 �� ��: --
 �� ��: --
*******************************************************************************/
static void dm603_face_pre(U08 *disp_buf)
{
    if (fg_limit_cancel)    /* ȡ��ʹ�����ޣ�ֱ�ӷ��������� */
    {
        para_set(PG_GetAddr_ParamLimit(0), 0, 0);
        dm603_face[hmi_t_num] = FACE_MAIN;
        return;
    }

    switch (dm603_menu[hmi_t_num])
    {
        case DM603_MENU_CREATE :
            dm603_menu[hmi_t_num] = DM603_MENU_PROC;
            key_delay_time[hmi_t_num]=0;
            dm603_common_disp_flash();
            /* break; */

        case DM603_MENU_PROC :
            if(key_delay_time[hmi_t_num]>=KEY_DELAY)
            {
                if (Main_Key[hmi_t_num] == DM603_KEY_FUNC)  /* ���������� */
                {
                    dm603_face[hmi_t_num] = FACE_MAIN;
                    return;
                }
                else if (Main_Key[hmi_t_num] == DM603_KEY_OK_FUNC)
                {
                    dm603_push_face_status();   /* �������״̬ */
                    dm603_set_nextface_status(FACE_PRE);   // ȷ��������ȷ��Ҫ��ת�Ľ���
                    dm603_face[hmi_t_num] = FACE_PASS;
                    dm603_PassNeed[hmi_t_num] = PASS_LMT;   // ��ǰ���������Ҫ��Ȩ��
                    return;
                }
            }
            /* break; */

        case DM603_MENU_PAINT :
            /* ��ʾ��Ϣ */
            disp_value_U16(pre_limit_time, ROW1, 0, DNS_DIGIT3 | DNS_ZERO | DCS_NORMAL, disp_buf);

            disp_string(dm603_face_limit_pre_01, ROW1, 4*8, DCS_NORMAL, disp_buf);
            disp_string(dm603_face_limit_pre_02, ROW2, 0, DCS_NORMAL, disp_buf);
            disp_string(dm603_face_limit_pre_03, ROW3, 0, DCS_NORMAL, disp_buf);
            disp_string(dm603_face_limit_pre_04, ROW4, 0, DCS_NORMAL, disp_buf);
            break;
    }
}


/*******************************************************************************
 �� ��: dm603_face_off()
 �� ��: ϵͳά��ʱ�䵽����
 �� ��: --
 �� ��: --
 �� ��: --
*******************************************************************************/
static void dm603_face_off(U08 *disp_buf)
{
    static U08 dm603_sound_bak;

    if (fg_limit_cancel)    /* ȡ��ʹ�����ޣ�ֱ�ӷ��������� */
    {
//      limit_value = 0;
        para_set(PG_GetAddr_ParamLimit(0), 0, 0);
        dm603_face[hmi_t_num] = FACE_MAIN;
        return;
    }

    switch (dm603_menu[hmi_t_num])
    {
        case DM603_MENU_CREATE :
            dm603_menu[hmi_t_num] = DM603_MENU_PROC;
            key_delay_time[hmi_t_num]=0;
            dm603_sound_bak = dm603_sound[hmi_t_num];
            dm603_common_disp_flash();  /* ����ˢ�� */
            /* break; */

        case DM603_MENU_PROC :
            /* ���������� */    //NOTE-CXW Ҫ����
            if (!fg_sys_limit)
            {
                dm603_face[hmi_t_num] = FACE_MAIN;
                return;
            }
            if(key_delay_time[hmi_t_num]>=KEY_DELAY)
            {
                /* ʹ���������� */
                if (Main_Key[hmi_t_num] == DM603_KEY_OK_FUNC)
                {
                    dm603_push_face_status();   /* �������״̬ */
                    dm603_set_nextface_status(FACE_OFF);   // ȷ��������ȷ��Ҫ��ת�Ľ���
                    dm603_face[hmi_t_num] = FACE_PASS;
                    dm603_PassNeed[hmi_t_num] = PASS_LMT;   // ��ǰ���������Ҫ��Ȩ��
                    return;


                }
                /* �������� */
                if (Main_Key[hmi_t_num] == DM603_KEY_OK)
                {
                    dm603_sound[hmi_t_num] = FALSE;
                }
                /* �ɹػ� */
                if (Main_Key[hmi_t_num] == DM603_KEY_ONOFF)
                {
                    if ((status_run == _RUN || status_run ==_READY))
                    {
                        dm603_face[hmi_t_num] = FACE_ONOFF;
                        return;
                    }
                }
            }
            /* ˢ�½��� */
            if (dm603_sound_bak != dm603_sound[hmi_t_num])
            {
                dm603_sound_bak = dm603_sound[hmi_t_num];
                dm603_common_disp_flash();
            }
            /* break; */

        case DM603_MENU_PAINT :
            /* ��ʾ��Ϣ */
            disp_string(dm603_face_limit_off_01, ROW1, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);
            disp_string(dm603_face_limit_off_02, ROW2, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);

            /* �˵��� */
            if (dm603_sound[hmi_t_num])
            {
                dm603_common_disp_menu(BMP_NU, NULL, BMP_OK, dm603_face_limit_off_ok, disp_buf);
            }
            else
            {
                dm603_common_disp_menu(BMP_NU, NULL, BMP_NU, NULL, disp_buf);
            }
            break;
    }
}



/*******************************************************************************
 �� ��: dm603_time_routine()
 �� ��: ��ʱѭ������
 �� ��: --
 �� ��: --
 �� ��: --
*******************************************************************************/

static void dm603_time_routine(void)
{
    U08 i;

    U08 tweek, thour, tmin;     /* ��ʱʱ�� */
    U08 cweek, chour, cmin;     /* ��ǰʱ�� */

    cweek = clock[hmi_t_num].week;      /* �� */
    chour = clock[hmi_t_num].hour;      /* ʱ */
    cmin  = clock[hmi_t_num].min ;      /* �� */

    /* ��ʱ�������� */
    if (GET_TIME_EN())
    {
        /* ���ڴ��� */
        if (cweek == 1) cweek = 6;  /* ������ */
        else            cweek -= 2; /* ������ */

        for (i = 0; i < (sizeof(dm603_time_tbl)/sizeof(U08))/3; i++)
        {
            tweek = dm603_time_tbl[i*3+0];
            thour = dm603_time_tbl[i*3+1];
            tmin  = dm603_time_tbl[i*3+2];

            /* ��Ч��ʱ��ʱ��δ�� */
        //  if (thour == 0 && tmin == 0)        continue;   //jxj 2016.01.15
            if (thour != chour || tmin != cmin) continue;

            /* һ�ζ�ʱ */
            if (i == 0)
            {
                if(GET_BIT(tweek, 6))   //һ�ζ�ʱʹ�ÿ���jxj 2016.01.15
                {
                    if (!fg_enter_holiday) // �����ݼ�ģʽʱ����ʱ���ػ���Ч
                    {
                        if (GET_BIT(tweek, 7))      DM603_CMD_RUN;
                        else                        DM603_CMD_STOP;
                    }

                    /* ���涨ʱʱ�� */
                    CLR_BIT(tweek, 7);  /* �嶨ʱ���� */
                    CLR_BIT(tweek, 6);  /* �嶨ʱʹ�ÿ���jxj 2016.01.15 */
                    thour = tmin = 0;       /* ���ʱ�� */
                    dm603_time_tbl[0] = tweek;
                    dm603_time_tbl[1] = thour;
                    dm603_time_tbl[2] = tmin ;

//                  dm603_face_menu_eeprom(MENU_WR, dm603_time_tbl);
                    DataAccess_Timer(DATA_WRITE);
                }
            }
            /* ѭ����ʱ */
            else
            {
                //if (GET_BIT(tweek, cweek))    DM603_CMD_RUN;
                //else                          DM603_CMD_STOP;
                if (!fg_enter_holiday) // �����ݼ�ģʽʱ����ʱ���ػ���Ч
                {
                    if (GET_BIT(tweek, cweek))
                    {
                        if (GET_BIT(tweek, 7))  DM603_CMD_RUN;
                        else                        DM603_CMD_STOP;
                    }
                }
            }
        }
    }
}


 /*******************************************************************************
 �� ��: dm603_onoff_routine()
 �� ��: ���ػ�ѭ���������
 �� ��: --
 �� ��: --
 �� ��: --
*******************************************************************************/
static void dm603_onoff_routine(void)
{
    if (dm603_face[hmi_t_num] == FACE_LOCK) return;
    if (dm603_face[hmi_t_num] == FACE_LOCK1) return;
    if (dm603_face[hmi_t_num] != FACE_MAIN) return;    // ϵͳά����������ͨ�����ؼ��ػ�
    if (fg_disp_lock)                       return;
    if (Main_Key[hmi_t_num] == DM603_KEY_ONOFF)
    {
        if(debug_mode == TRUE)
        {
            debug_mode = FALSE ;
            return;
        }

        dm603_face[hmi_t_num] = FACE_ONOFF;
    }
}

/*******************************************************************************
 �� ��: dm603_limit_routine()
 �� ��: ϵͳά������
 �� ��: --
 �� ��: --
 �� ��: --
*******************************************************************************/
static void dm603_limit_routine(void)
{
    static BOOL fg_first_in = TRUE; /*  ��һ�ν����־(�ϵ�ʱʹ������������) */

    switch(dm603_face[hmi_t_num])// ���½��治ǿ���л���ά�������ѽ���
    {
        case FACE_OPEN:
        case FACE_LIMIT:
        case FACE_LIMIT_SET:
        case FACE_LIMIT_RUNTIME:
        case FACE_LIMIT_RUNTIME_INIT:
        case FACE_LIMIT_INIT:
        case FACE_PASS:
        case FACE_PASS_M_LMT:
        case FACE_PASS_M_FAC:
        case FACE_PASS_M_USER:
            return;
//            break;

    }

    if (fg_sys_limit)/* ʹ�������ѵ� */
    {
        if (dm603_face[hmi_t_num] != FACE_ONOFF)
        {
            dm603_face[hmi_t_num] = FACE_OFF;
        }
    }
    else if (pre_limit_time != 0  )/* ��Ҫ���� */
    {
        if (fg_first_in)
        {
            dm603_face[hmi_t_num] = FACE_PRE;
            fg_first_in = FALSE;
        }
        if (clock[hmi_t_num].hour==9 && clock[hmi_t_num].min==0 && clock[hmi_t_num].sec==0  )   /* ÿ������9������ʾ */
        {
            if (!dm603_fg_one_day[hmi_t_num])               /* һ��֮�ڻ�δ���� */
            {
                dm603_face[hmi_t_num] = FACE_PRE;
                dm603_fg_one_day[hmi_t_num] = TRUE;
            }
        }
        else                                    /* һ��ʱ��������־��׼����һ������ */
        {
            dm603_fg_one_day[hmi_t_num] = FALSE;
        }
    }
}
/*******************************************************************************
 �� ��: DispInfo_Temperature()
 �� ��: ��ʾ�¶���Ϣ
 �� ��: U16 value_attrib �¶�ֵ����ʾ����
 �� ��: --
 �� ��: ����������ֻ��5���ַ������
*******************************************************************************/
void DispInfo_Temperature(U16 string_index, I16 temp_value, U16 row, U16 col, U16 value_attrib, U08 *disp_buf)
{

    disp_string(string_index, row, col, DCS_NORMAL, disp_buf);

    if (!SensorCannotUse(temp_value))
    {
        disp_value_I16(temp_value, row, col+40, value_attrib, disp_buf);
    }
    else
    {
        disp_string_ptr8("...  ", row, col+40, DCS_NORMAL, disp_buf);
    }
    disp_bmp(idx_bmp_celsius_16_16, row, col+80, DCS_NORMAL, disp_buf);

}


/*******************************************************************************
 �� ��: DispInfo_Clock()
 �� ��: ��ʾʱ����Ϣ
 �� ��: RTC ClockDisp: ʱ��ֵ, U16 clock_type: Ҫ��ʾ��ʱ��ģʽ
        attrib ÿ1λ����1��ʱ��ֵ����ʾ����, 0-������ʾ, 1-������ʾ��
               ��ʱ����Ŀ��˳��(lcd.h)����6~0λ�ֱ���� �ꡢ����(�˴�û��)���¡��ա�ʱ���֡���
 �� ��: --
 �� ��:
*******************************************************************************/

void DispInfo_Clock(U16 row, U16 col, RTC ClockDisp, U16 clock_type, U08 attrib, U08 *disp_buf)
{
    U08 Type = 0;
    U16 col_disp;

    U16 attrib_colon = DCS_NORMAL;  // ð�ŵ���ʾ����  (�Ƿ���˸)
    U16 attrib_clock = DCS_NORMAL;  // ʱ��ֵ����ʾ����(�Ƿ񷴰�)


    col_disp = col; // ��ʾ��ʼλ��

    if(clock_type&CHN)  Type = 1;   // 1-������ʾ���ĵ�λ

    if(clock_type&YYYY)
    {
        disp_string_ptr8("20", row, col_disp, DCS_NORMAL, disp_buf);
        col_disp = col_disp+2*8;
    }
    if((clock_type&YY)||(clock_type&YYYY))  // ��
    {
        attrib_clock = (attrib&(0x0001<<RTC_YEAR))?DCS_REVERSE:DCS_NORMAL;
        disp_value_U16(ClockDisp.year, row, col_disp,   DNS_DIGIT2 | DNS_ZERO | attrib_clock, disp_buf);
        col_disp = col_disp+2*8;

        if(Type==1 && dm603_language == LANGUE_CHINESE)// ����
        {
            disp_string(msg_date_y, row, col_disp, DCS_NORMAL, disp_buf);
            col_disp = col_disp+2*8;
        }
        else if(clock_type&MON) // ���滹���·�Ҫ��ʾ
        {
            disp_string_ptr8("/", row, col_disp, DCS_NORMAL, disp_buf);
            col_disp = col_disp+8;
        }
    }
    if(clock_type&MON)  // ��
    {
        attrib_clock = (attrib&(0x0001<<RTC_MONTH))?DCS_REVERSE:DCS_NORMAL;
        disp_value_U16(ClockDisp.month, row, col_disp,  DNS_DIGIT2 | DNS_ZERO | attrib_clock, disp_buf);
        col_disp = col_disp+2*8;

        if(Type==1 && dm603_language == LANGUE_CHINESE)// ����
        {
            disp_string(msg_date_m, row, col_disp, DCS_NORMAL, disp_buf);
            col_disp = col_disp+2*8;
        }
        else if(clock_type&DAY) // ���滹������Ҫ��ʾ
        {
            disp_string_ptr8("/", row, col_disp, DCS_NORMAL, disp_buf);
            col_disp = col_disp+8;
        }
    }

    if(clock_type&DAY)  // ��
    {
        attrib_clock = (attrib&(0x0001<<RTC_DAY))?DCS_REVERSE:DCS_NORMAL;
        disp_value_U16(ClockDisp.day, row, col_disp,    DNS_DIGIT2 | DNS_ZERO | attrib_clock, disp_buf);
        col_disp = col_disp+2*8;

        if(Type==1 && dm603_language == LANGUE_CHINESE)// ����
        {
            disp_string(msg_date_d, row, col_disp, DCS_NORMAL, disp_buf);
            col_disp = col_disp+2*8;
        }
    }

    if(clock_type&HR)   // ʱ
    {
        attrib_clock = (attrib&(0x0001<<RTC_HOUR))?DCS_REVERSE:DCS_NORMAL;
        disp_value_U16(ClockDisp.hour, row, col_disp,   DNS_DIGIT2 | DNS_ZERO | attrib_clock, disp_buf);
        col_disp = col_disp+2*8;

        if(clock_type&MINU) // ���滹�з���Ҫ��ʾ
        {
            attrib_colon=(clock_type&COLON_BINK)?DCS_BLINK:DCS_NORMAL;  // ð���Ƿ���˸

            disp_string_ptr8(":", row, col_disp, attrib_colon, disp_buf);
            col_disp = col_disp+8;
        }
    }

    if(clock_type&MINU) // ��
    {
        attrib_clock = (attrib&(0x0001<<RTC_MIN))?DCS_REVERSE:DCS_NORMAL;
        disp_value_U16(ClockDisp.min, row, col_disp,    DNS_DIGIT2 | DNS_ZERO | attrib_clock, disp_buf);
        col_disp = col_disp+2*8;

        if(clock_type&SEC)  // ���滹������Ҫ��ʾ
        {
            attrib_colon=(clock_type&COLON_BINK)?DCS_BLINK:DCS_NORMAL;  // ð���Ƿ���˸

            disp_string_ptr8(":", row, col_disp, attrib_colon, disp_buf);
            col_disp = col_disp+8;
        }
    }

    if(clock_type&SEC)  // ��
    {
        attrib_clock = (attrib&(0x0001<<RTC_SEC))?DCS_REVERSE:DCS_NORMAL;
        disp_value_U16(ClockDisp.sec, row, col_disp,    DNS_DIGIT2 | DNS_ZERO | attrib_clock, disp_buf);

    }


}


/*******************************************************************************
 �� ��: DispString_switch()
 �� ��: ��˸�ַ����л�
 �� ��: SwitchNow����ǰ��ʾ�ڼ�� SwitchNum��һ���м�����Ҫ��˸��ʾ�� TimeBak��  1���л�ͬ����־
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void DispString_switch(U08* SwitchNow, U08 SwitchNum, BOOL* TimeBak)
{
    if(f_blink_1S!= *TimeBak)   // ����ֱ����F_TIME_BASIC_SEC_BACK����ǰ��������ÿѭ�����ã����õ�ʱ�ñ�־������
    {
        if (f_blink_1S==TRUE )
        {
            *SwitchNow = (*SwitchNow>=(SwitchNum-1))?0:(*SwitchNow+1);
        }
        *TimeBak = f_blink_1S;  // �л���־ͬ��
        dm603_common_disp_flash();
    }
}

/*******************************************************************************
 �� ��: DispInfo_Wifi()
 �� ��: ��ʾ�Ʒ�����Ϣ
 �� ��:
 �� ��: --
 �� ��: --
*******************************************************************************/
void DispInfo_TuyaWifi(U16 row, U16 col, U08 *disp_buf)
{
    #define TUYA_SIGNAL_LIEVE_1  0
    #define TUYA_SIGNAL_LIEVE_2  1
    #define TUYA_SIGNAL_LIEVE_3  2
    #define TUYA_SIGNAL_LIEVE_4  3
    #define TUYA_SIGNAL_LIEVE_5  4

    U08 signal_level=TUYA_SIGNAL_LIEVE_1;   // ·���ź�ǿ�ȵȼ�
    U16 bmp_code=idx_bmp_signal_00_16_16;
    static const U16 bmp_signal_level[]={
        idx_bmp_signal_01_16_16,
        idx_bmp_signal_02_16_16,
        idx_bmp_signal_03_16_16,
        idx_bmp_signal_04_16_16,
        idx_bmp_signal_05_16_16,
        };
    static const U16 bmp_net_level[]={
        idx_bmp_signal_01_16_16,
        idx_bmp_signal_02_e_16_16,
        idx_bmp_signal_03_e_16_16,
        idx_bmp_signal_04_e_16_16,
        idx_bmp_signal_05_e_16_16,
        };

    if (hmi_type[hmi_t_num] == HMI_TYPE_TEXT_WIFI_TY)
    {
        if (hmi_ty_signal == 0)         signal_level = TUYA_SIGNAL_LIEVE_1;
        else if (hmi_ty_signal <= 20)   signal_level = TUYA_SIGNAL_LIEVE_2;
        else if (hmi_ty_signal <= 40)   signal_level = TUYA_SIGNAL_LIEVE_3;
        else if (hmi_ty_signal <= 60)   signal_level = TUYA_SIGNAL_LIEVE_4;
        else                            signal_level = TUYA_SIGNAL_LIEVE_5;


        switch (hmi_ty_status)
        {
            case TY_STATUS_LINK_ROTER:
                bmp_code = bmp_signal_level[signal_level];
                break;
            case TY_STATUS_LINK_CLOUD:
                bmp_code = bmp_net_level[signal_level];
                break;
            default:
                bmp_code = idx_bmp_signal_00_16_16;
                break;
        }

        disp_bmp(bmp_code, row, col, DCS_NORMAL, disp_buf);

    }
}


/*******************************************************************************
 �� ��: dm603_face_main()
 �� ��: ������
 �� ��: --
 �� ��: --
 �� ��: --
*******************************************************************************/
#define SET_TEMP        0   /* �����¶� */
#define SET_MODE        1   /* ����ģʽ */

#define ADDR_SET_GROUP          0   // �û����ò������ַ
#define ADDR_MODE               0   // ģʽ�趨��ַ
#define ADDR_TEMP_COOL          1   // �����趨��ַ
#define ADDR_TEMP_HEAT          2   // �����趨��ַ
#define ADDR_TEMP_HOT_WATER     3   // ��ˮ�趨��ַ

const U16 msg_status[] =
{   /* ״̬ */
    dm603_face_main_status_01,  // ����
    dm603_face_main_status_02,  // ����
    dm603_face_main_status_03,  // ����
    dm603_face_main_status_04,  // ͣ��
    dm603_face_main_status_05,  // ����
    dm603_face_main_status_06,  // ��˪
    dm603_face_main_status_07,  // ����
    dm603_face_main_status_08,  // ����
    dm603_face_main_status_09,  // Ԥ��
    dm603_face_main_status_10,  // ����
    dm603_face_main_status_11,  // ǿ��
    dm603_face_main_status_12,  // ����
    dm603_face_main_status_13,  // ɱ��
    dm603_face_main_status_14,  // ���
    dm603_face_main_status_15,  // ����
};

const U16 msg_mode[] =
{   /* ģʽ */
    dm603_face_main_mode_00,        // ����
    dm603_face_main_mode_01,        // ����
    dm603_face_main_mode_02,        // ����
    dm603_face_main_mode_03,        // �Զ�
    dm603_face_main_mode_04,        // ����
    dm603_face_main_mode_05,        // ��ˮ
};

static void dm603_face_main(U08 *disp_buf)
{
    // ��ʾģʽ�±�
    static const U08 msg_mode_index[] = {0, 1, 2, 3, 5};

    U08 time_updown = 35;   /* �趨�¶��˳���ʱ(100ms) */
    I16 value[MAX_HMI];

//  U08 mac_status = _IDLE;     /* ����״̬��ʾ */
    U16 attrib_mode = DCS_NORMAL;
    U16 attrib_temp = DCS_NORMAL;
    U16 sv_temp = 0;

    static U08 cursor_n[MAX_HMI], cursor_m[MAX_HMI];
    static U08 status_run_bak[MAX_HMI];
    static U08 fg_error_bak[MAX_HMI];

    static U08 main_cursor[MAX_HMI];    // ���������ù��
    static U08 fg_nexch[MAX_HMI];       // ���ɸ����ñ�־
    static U08 fg_main_set[MAX_HMI];    /* ���������ñ�־ */

    switch (dm603_menu[hmi_t_num])
    {
        case DM603_MENU_CREATE :
            fg_nexch[hmi_t_num] = 0;
            fg_main_set[hmi_t_num] = 0;
            main_cursor[hmi_t_num] = SET_TEMP;
            cursor_n[hmi_t_num] = ADDR_SET_GROUP;
            cursor_m[hmi_t_num] = GET_TEMP_SET_ADDR();
            fg_error_bak[hmi_t_num] = 0;
            dm603_menu[hmi_t_num] = DM603_MENU_PROC;
            dm603_common_disp_flash();
            dm603_cnt_key_updwn[hmi_t_num] = 0xff;
            /* break; */

        case DM603_MENU_PROC :

            if (val_lock_screen)
            {
                if (Main_Key[hmi_t_num] == DM603_KEY_OK_3S)
                {
                    /* �������������Ĺ��ܣ�����������ȷ�ϼ�����붨ʱ���ã��������ص� */
//                    fg_disp_lock = !fg_disp_lock;
                    fg_disp_lock = FALSE;
                }
            }

            /* ����ʱ�޷��������� */
            if (!fg_disp_lock)
            {
                switch (Main_Key[hmi_t_num])
                {
                    case DM603_KEY_FUNC:        /* ����״̬��ѯ���� */
                        dm603_push_face_status();   /* �������״̬ */
                        dm603_face[hmi_t_num] = FACE_MENU_USER_SHORTCUT;
//                      dm603_face[hmi_t_num] = FACE_MENU_MODE;
                        return;
//                      break;

                    case DM603_KEY_OK:
                        if (dm603_cnt_key_updwn[hmi_t_num] <= time_updown)  /* �������ã��л������� */
                        {
                            cursor_n[hmi_t_num] = ADDR_SET_GROUP;
                            if (main_cursor[hmi_t_num] == SET_TEMP)     /* �趨�¶� */
                            {
                                cursor_m[hmi_t_num] = GET_TEMP_SET_ADDR();

                            }

                            para_get_val(PG_GetAddr_ParamPri(cursor_n[hmi_t_num]), cursor_m[hmi_t_num], &value[hmi_t_num]);

                            fg_nexch[hmi_t_num] = 0;
                            fg_main_set[hmi_t_num] = 0;
                            dm603_cnt_key_updwn[hmi_t_num] = 0xff;
                        }
                        else if (fg_error) /* �й��ϣ���������ѯ���� */
                        {
                            dm603_push_face_status();   /* �������״̬ */

                            dm603_sound[hmi_t_num] = FALSE;     /* ���� */
                            dm603_face[hmi_t_num] = FACE_ERROR;
                            return;
                        }
                        else    //��ʱ����
                        {
                            dm603_push_face_status();   /* �������״̬ */

                            dm603_sound[hmi_t_num] = FALSE;     /* ���� */
                            dm603_face[hmi_t_num] = FACE_MENU_TIME;
                            return;

                        }
                        break;

                    case DM603_KEY_OK_FUNC: /* �������� */
                        dm603_push_face_status();   /* �������״̬ */
                        dm603_set_nextface_status(FACE_PARA);   // ȷ��������ȷ��Ҫ��ת�Ľ���
                        dm603_face[hmi_t_num] = FACE_PASS;
                        dm603_PassNeed[hmi_t_num] = PASS_PARA;      // ��ǰ���������Ҫ��Ȩ��
                        return;
//                      break;

                    case DM603_KEY_UP:
                    case DM603_KEY_DOWN:
                        if (dm603_cnt_key_updwn[hmi_t_num] < time_updown)   /* �ѽ������ý��� */
                        {
                            if (para_is_nexch(PG_GetAddr_ParamPri(cursor_n[hmi_t_num]), cursor_m[hmi_t_num]))
                            {
                                fg_nexch[hmi_t_num] = 1;
                            }
                            else
                            {
                                if(sv_economy_mode==USED && main_cursor[hmi_t_num] == SET_TEMP) // ����ģʽ�²������������趨�¶�
                                    break;

                                para_get_val(PG_GetAddr_ParamPri(cursor_n[hmi_t_num]), cursor_m[hmi_t_num], &value[hmi_t_num]);

                                if (Main_Key[hmi_t_num] == DM603_KEY_UP)    /* ������ */
                                {
                                    para_add(PG_GetAddr_ParamPri(cursor_n[hmi_t_num]), cursor_m[hmi_t_num], &value[hmi_t_num], 0);
                                }
                                else            /* ������ */
                                {
                                    para_sub(PG_GetAddr_ParamPri(cursor_n[hmi_t_num]), cursor_m[hmi_t_num], &value[hmi_t_num], 0);
                                }

                                para_set(PG_GetAddr_ParamPri(cursor_n[hmi_t_num]), cursor_m[hmi_t_num], value[hmi_t_num]);
                            }
                        }
                        dm603_cnt_key_updwn[hmi_t_num] = 0;
                        fg_main_set[hmi_t_num] = 1;
                        break;
                }
            }
            /* break; */

        case DM603_MENU_PAINT :
            if (Main_Key[hmi_t_num] != KEY_NULL)    dm603_common_disp_flash();

            if (fg_error_bak[hmi_t_num] != fg_error)        // ����״̬�仯��ˢ��
            {
                fg_error_bak[hmi_t_num] = fg_error;
                dm603_common_disp_flash();
            }
            if (status_run_bak[hmi_t_num] != status_run)    // ״̬�仯��ˢ��
            {
                status_run_bak[hmi_t_num] = status_run;
                dm603_common_disp_flash();
            }

            // ״̬����
            {
                /* ע�⣺���statusλ�� */
                enum {STA_IDLE,     STA_READY,  STA_RUN,    STA_DELAY,
                      STA_ALARM,    STA_DEFR,   STA_ANTI,   STA_TEST,
                      STA_WARM,     STA_SILENT, STA_STRONG, STA_ECO,
                      STA_STER,     STA_DRYUP,  STA_HOLIDAY,STA_MAX};

                U08 mac_status = status_run;        /* ����״̬ */
                U16 status = 0;

                if (Link_remeber_on)                                    mac_status = _RUN;// ԭ������������״̬��������ʾ�����С�   ����ʾ����������ͬʱ��
                else if (status_run==_DELAY && fg_machine_need_open)    mac_status = _READY;

                                                SETBIT(status, mac_status);
                if (ctrl_info[0] & 0x0800)      SETBIT(status, STA_DEFR);   /* ��˪ */
                if (ctrl_info[0] & 0x0400)      SETBIT(status, STA_ANTI);   /* ���� */
                if (fg_enter_ability_test)      SETBIT(status, STA_TEST);   // ����
                if (!fg_warm_complete)          SETBIT(status, STA_WARM);   /* Ԥ�� */
                if (log_sys_is_silent())        SETBIT(status, STA_SILENT); /* ���� */
                if (ModeIsStr)                  SETBIT(status, STA_STRONG); /* ǿ�� */
                if (ModeIsEco)                  SETBIT(status, STA_ECO);    /* ���� */
                if (fg_timed_disinfect_run)    SETBIT(status, STA_STER);   /* ɱ�� */
                if (fg_enter_floor_dryup)       SETBIT(status, STA_DRYUP);  /* ��� */
                if (fg_enter_holiday)           SETBIT(status, STA_HOLIDAY);/* ���� */

                {
                    U08 i, sta_num=0;
                    U08 sta_list[STA_MAX];
                    static U08 sta_idx[MAX_HMI];
                    static BOL falsh_bak[MAX_HMI];
                    static BOL sta_warm_disp;
                    /* ��ȡ��Ч״̬ */
                    for (i=0; i<STA_MAX; i++)
                    {
                        if (GETBIT(status, i))
                        {
                            sta_list[sta_num++] = i;
                        }
                    }
                    /* �л���ʾ */
                    if (falsh_bak[hmi_t_num] != f_blink_1S)
                    {
                        falsh_bak[hmi_t_num] = f_blink_1S;
                        sta_idx[hmi_t_num]++;

                        if (sta_list[sta_idx[hmi_t_num]] == STA_WARM)                        sta_warm_disp = !sta_warm_disp;
                    }
                    if (sta_idx[hmi_t_num] >= sta_num)            sta_idx[hmi_t_num] = 0;
                    /* ��ʾ */
                    {
                        disp_string_ptr8(" ", ROW1, 128-5*8, DCS_FLASH, disp_buf);

                        if (sta_list[sta_idx[hmi_t_num]] == STA_WARM)
                        {
                            if (sta_warm_disp)
                            {
                                U08 hour = log_sys_warm_time_get()/60;
                                U08 min  = log_sys_warm_time_get()%60;

                                disp_value_U08(hour, ROW1, 128-5*8, DCS_FLASH|DNS_ZERO|DNS_DIGIT2, disp_buf);
                                disp_string_ptr8(":", ROW1, 128-3*8, DCS_FLASH, disp_buf);
                                disp_value_U08(min, ROW1, 128-2*8, DCS_FLASH|DNS_ZERO|DNS_DIGIT2, disp_buf);
                            }
                            else
                            {
                                disp_string(msg_status[sta_list[sta_idx[hmi_t_num]]], ROW1, 128, DTS_RIGHT|DCS_FLASH, disp_buf);
                            }
                        }
                        else
                        {
                            /* ��ʾ */
                            disp_string(msg_status[sta_list[sta_idx[hmi_t_num]]], ROW1, 128, DTS_RIGHT|DCS_FLASH, disp_buf);
                        }
                    }
                }
            }

            // �������������ʾģʽ��һ����[ģʽ]��[�趨�¶�]��
            attrib_mode = DCS_FLASH;    //  // һֱˢ�£���ֹ������˸�����˳����ú���ʾ
            attrib_temp = DCS_NORMAL;
            if (dm603_cnt_key_updwn[hmi_t_num] < time_updown)   // ��������
            {
                if (main_cursor[hmi_t_num] == SET_MODE) // ����ģʽ����˸��ʾ
                {
                    if (fg_nexch[hmi_t_num])    /* ģʽ�����޸���ʾ */
                    {
//                      disp_string(dm603_face_query_para_mode_nexch_01, ROW2, COL_BGN, DCS_NORMAL, disp_buf);
//                      disp_string(dm603_face_query_para_mode_nexch_02, ROW3, COL_BGN, DCS_NORMAL, disp_buf);
                    }
                    else
                    {
                        attrib_mode |= DCS_BLINK;   /* �����޸ģ���˸��ʾ */
                    }
                }
                else
                {
                    attrib_temp |= DCS_BLINK;       // �����¶ȣ���˸��ʾ
                }
            }
            else    // ���˳�����
            {
                fg_nexch[hmi_t_num] = 0;
                fg_main_set[hmi_t_num] = 0;
                main_cursor[hmi_t_num] = SET_TEMP;
                cursor_m[hmi_t_num] = GET_TEMP_SET_ADDR();
            }

            /* ״̬����ROW1��------------------------------------------------------------------------*/
            if(debug_mode == TRUE)  // ˮ�õ���ģʽ
            {
                disp_string(msg_mode[4], ROW1, COL_BGN, attrib_mode, disp_buf);
            }
//            else    // ��������ģʽ
//            {
//                disp_string(msg_mode[msg_mode_index[disp_mode]], ROW1, COL_BGN, attrib_mode, disp_buf);
//            }

            DispInfo_TuyaWifi(ROW1, 5*16-8, disp_buf);  // ��ʾͿѻ�ź�


            /* �¶�����ROW2��ROW3��-------------------------------------------------------------------*/
            // ��ǰ�¶�
//            DispInfo_Temperature(now_text, ctrl_temp_energy, ROW2, COL_BGN+16, DNS_FLOAT1|DNS_DIGIT3, disp_buf);

            // �趨�¶�
//            if (fg_main_set[hmi_t_num])
//            {
//                sv_temp =  (GET_TEMP_SET_VAL()) / 10;
//            }
//            else
//            {
//                sv_temp =  sv_temp_energy / 10;
//            }
            DispInfo_Temperature(set,      sv_temp,   ROW3, COL_BGN+16, attrib_temp|DNS_DIGIT3, disp_buf);

            /* ��ʾ����ROW4��------------------------------------------------------------------------*/
            if (fg_error)           // ���ϣ���ʾ���Ҳ���ʾ���ϰ���
            {
                dm603_common_disp_menu(BMP_FN, dm603_face_main_menu2, BMP_OK, NULL, disp_buf);
                disp_string(dm603_face_alarm_query, ROW4, COL_END-48, DCS_BLINK, disp_buf);     // �����ϡ��ַ���˸
            }
            else    // �޹��ϣ���ʾ���Ҳ���ʾʱ��
            {
                dm603_common_disp_menu(BMP_FN, dm603_face_main_menu2, NULL, NULL, disp_buf);
                DispInfo_Clock(ROW4, COL_END-40, clock[hmi_t_num], HHMM|COLON_BINK, 0, disp_buf);
            }

            if (fg_disp_lock)       /* ��������ʾ���м���ʾ����ͼ��  */
            {
                disp_bmp(idx_bmp_lock_16_16, ROW4, 16*3, DCS_NORMAL, disp_buf);
            }
            else
            {
                disp_string_ptr8("  ", ROW4, 16*3, DCS_NORMAL, disp_buf);
            }
            break;
    }
}


/***********************************************
������dm603_error_routine
���ܣ���������״̬����
��������
����ֵ����
***********************************************/
static void dm603_error_routine(void)
{
    if (fg_error == 0)
    {
        dm603_sound[hmi_t_num] = FALSE;
    }
    else
    {
        if (dm603_error_bak[hmi_t_num] != fg_error)
        {
            dm603_sound[hmi_t_num] = TRUE;
            dm603_alarm_sound_cnt[hmi_t_num] = 0;
        }
    }
    dm603_error_bak[hmi_t_num] = fg_error;

    if (dm603_sound[hmi_t_num])
    {
        if (val_alarm_sound_set == ALARM_SOUND_SET_ON
        || (val_alarm_sound_set == ALARM_SOUND_SET_10S_ONCE
            && dm603_alarm_sound_cnt[hmi_t_num] < 10)
        || (val_alarm_sound_set == ALARM_SOUND_SET_10S_CYCLE
            && dm603_alarm_sound_cnt[hmi_t_num]%ALARM_SOUND_CYCLE < 10)
        )
        {
            disp_set_snd();
        }
        else
        {
            disp_clr_snd();
        }
    }
    else
    {
        disp_clr_snd();
    }
}

/**
 * ���� : dm603_routine()
 * ���� : ѭ������
 *
 * ���� : --
 * ���� : --
 *
 * ��ע : --
 */
void dm603_routine(U08 *disp_buf)
{
    disp_buf = disp_buf;    /* N0001 */

//  dm603_face_flash();     /* ��˸��־���� */

    //dm603_online_routine();   /* �ϵ�ѭ������ */


    dm603_error_routine();  /* ����ѭ������ */

//  dm603_onoff_routine();  /* ���ػ�������� */


    dm603_limit_routine();  /* ʹ������ѭ������ */   //cxc

    dm603_time_routine();   /* ��ʱ������� */


//  dm603_key_routine();    /* ����������� */

//  dm603_other_routine(disp_buf);  /* ����������� */
}

/**
 * ���� : dm603_face_start()
 * ���� : ���洦��ǰ
 *
 * ���� : --
 * ���� : --
 *
 * ��ע : --
 */
static void dm603_face_start(U08 *disp_buf)
{
    disp_buf = disp_buf;    /* N0001 */

    if (cnt_menu_lock[hmi_t_num] == 0)          /* ����δ������ */
    {
        cnt_lock_clue[hmi_t_num] = 0;
    }
    else if (Main_Key[hmi_t_num] != KEY_NULL)   /* �����������а��� */
    {
        cnt_lock_clue[hmi_t_num] = LOCK_T_CLUE;
//      dm603_fg_lock_clue[hmi_t_num] = TRUE;   /* ������ʾ��־ */
    }
}

/**
 * ���� : dm603_face_end()
 * ���� : ���洦���
 *
 * ���� : --
 * ���� : --
 *
 * ��ע : --
 */
static void dm603_face_end(U08 *disp_buf)
{
    static U08 flash_cnt[MAX_HMI];

    /* ������ʾ�ŵ��������ʾ */
    if (cnt_lock_clue[hmi_t_num] > 0)   /* ��ʾ��ʾ��Ϣ */
    {
        flash_cnt[hmi_t_num] = 2;
        disp_string(lock_clue_1, ROW2, 0, DCS_BLINK, disp_buf);
        disp_value_U08(cnt_menu_lock[hmi_t_num], ROW3, 0, DNS_ZERO | DNS_DIGIT2 | DCS_BLINK, disp_buf);
        disp_string(lock_clue_2, ROW3, 16, DCS_BLINK, disp_buf);
    }
    else
    {
        /* ˢ����ʾ��Ϣ����ֹ���� */
        if (flash_cnt[hmi_t_num] == 2)
        {
            flash_cnt[hmi_t_num] = 1;
            dm603_common_disp_clr_line(ROW3, ROW2, RNUL, RNUL, disp_buf);
        }
        else if (flash_cnt[hmi_t_num] == 1)
        {
            flash_cnt[hmi_t_num] = 0;
            dm603_common_disp_flash();
        }

//      dm603_fg_lock_clue[hmi_t_num] = FALSE;  /* ������ʾ��־ */
    }

}

/**
 * ���� : dm603_online_routine()
 * ���� : ѭ�����紦��
 *
 * ���� : --
 * ���� : --
 *
 * ��ע : �ٸú���������lcd.c�ļ���disp_routine()�����С�
 *          ��Ϊ��������ν��������󣬻᲻��ˢ�µ�����档�мǣ�����
 */
void dm603_online_routine(U08 *disp_buf)
{
//  static U16 dm603_dly[MAX_HMI];
    static U08 dm603_is_online[MAX_HMI];
    static U08 dm603_dark_bak[MAX_HMI];

    /* ��ʾ�������˳� */
    if (GET_BIT(hmi_commu_error, hmi_t_num)) return;

    if (dm603_hmi_online[hmi_t_num] >= 8) dm603_is_online[hmi_t_num] = 0;   /* ���� */

    if (dm603_hmi_online[hmi_t_num] >= 3)
    {
        disp_clr_screen(disp_buf);
        dm603_common_disp_flash();          /* ����ˢ�� */
    }

    dm603_hmi_online[hmi_t_num] = 0;
    if (dm603_is_online[hmi_t_num] == 0)        /* ���� */
    {
        Main_Key[hmi_t_num] = KEY_NULL;
        dm603_error_bak[hmi_t_num] = 0;
        dm603_is_online[hmi_t_num] = 1;
        dm603_sound[hmi_t_num] = FALSE;
        dm603_face[hmi_t_num] = FACE_OPEN;
        dm603_power_time[hmi_t_num] = 0;

        cnt_no_operate[hmi_t_num] = 0;
        cnt_no_operate_sec[hmi_t_num] = 0;
        dm603_common_disp_flash();          /* ����ˢ�� */
    }

    /* �ж�����й��ϣ�����Ҫ��ʾ */
    if (fg_error && (dm603_sound[hmi_t_num] == FALSE))
    {
        if (dm603_dark_bak[hmi_t_num])
        {
            dm603_dark_bak[hmi_t_num] = 0;
            cnt_no_operate[hmi_t_num] = 0;
            cnt_no_operate_sec[hmi_t_num] = 0;
        }
    }
    else
    {
        dm603_dark_bak[hmi_t_num] = 1;
    }
}



//--------------------------------------------------------------------------
const MENUITEM  MenuItem_MainMenu[] = // �����ܲ˵�
{
    // �˵�����     /*// ��ʾ����   */  // ����Ȩ��     // ����ID               // �˵���                       // �˵�����
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_MENU_STATUS,       dm603_face_menu_select_02_,     0,  },      // 0 ״̬��ѯ
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_MENU_CLOCK,        dm603_face_menu_select_03_,     0,  },      // 1 ʱ�Ӳ�ѯ
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_MENU_VERSION,      dm603_face_menu_select_04_,     0,  },      // 2 �汾��ѯ
    {MENU_FACE,     /*PASS_NULL,    */  PASS_FUNC,      FACE_MENU_FUNC,         dm603_face_menu_select_05_,     0,  },      // 3 �߼�����
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_MENU_CLOUD,        dm603_face_menu_select_06_,     0,  },      // 4 ���߹���
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_MENU_FCU,          dm603_face_menu_select_07_,     0,  },      // 5 ������Ϣ
};
#define FACE_MENU_CLOUD_INDEX   4  // ���߹���-�˵����
#define FACE_MENU_FCU_INDEX     5  // ������Ϣ-�˵����


//--------------------------------------------------------------------------
const MENUITEM  MenuItem_Wifi[] = // ���߹��ܲ˵�
{
    // �˵�����     /*// ��ʾ����   */  // ����Ȩ��     // ����ID               // �˵���                       // �˵�����
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_MENU_CLOUD_CMD,    dm603_face_menu_wifi_cmd,       0,  },      // ��������
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_MENU_CLOUD_STATUS, dm603_face_menu_wifi_status,    0,  },      // ����״̬
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_MENU_CLOUD_INFO,   dm603_face_menu_wifi_info,      0,  },      // ������Ϣ

};


//--------------------------------------------------------------------------
const MENUITEM  MenuItem_WifiCMD[] = // �������ò˵�
{
    // �˵�����     /*// ��ʾ����   */  // ����Ȩ��     // ����ID               // �˵���                       // �˵�����
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_CLOUD_CMD_RESET,   dm603_face_menu_wifi_cmd_01,        0,  },      // ���߸�λ
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_CLOUD_CMD_CLR,     dm603_face_menu_wifi_cmd_02,        0,  },      // �������
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_CLOUD_CMD_RELEASE, dm603_face_menu_wifi_cmd_03,        0,  },      // �����

};


//--------------------------------------------------------------------------
const MENUITEM  MenuItem_Function[] = // �߼����ܲ˵�
{
    // �˵�����     /*// ��ʾ����   */  // ����Ȩ��     // ����ID               // �˵���                           // �˵�����
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_MENU_FUNC_USER,    dm603_face_menu_func_select_01,     0,  },      // �û�����
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_MENU_FUNC_DEFROST, dm603_face_menu_func_select_02,     0,  },      // �ֶ���˪
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_MENU_FUNC_RUNTIME, dm603_face_menu_func_select_03,     0,  },      // ����ʱ���ѯ
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_MENU_FUNC_HIS_ERR, dm603_face_menu_func_select_04,     0,  },      // ��ʷ���ϲ�ѯ
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PASS_M_USER,       dm603_face_menu_func_select_05,     0,  },      // �޸�����
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_MENU_INVT_PARA,    dm603_face_menu_func_select_06,     0,  },      // ��Ƶ����
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_ENTER_FLOOR_DRYUP, dm603_face_menu_func_select_07,     0,  },      // ��ů��ɹ���
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_FORCE_IBH,         dm603_face_menu_func_select_08,     0,  },      // ǿ�����õ����
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_FORCE_TBH,         dm603_face_menu_func_select_09,     0,  },      // ǿ��ˮ������
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_FORCE_HOT_WATER,   dm603_face_menu_func_select_10,     0,  },      // ǿ������ˮ(��������ˮ)
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_HOLIDAY_MODE_SET,  dm603_face_menu_func_select_11,     0,  },      // �ݼ�ģʽ����
};


//--------------------------------------------------------------------------
const MENUITEM  MenuItem_Limit[] = // ϵͳά���˵�
{
    // �˵�����     /*// ��ʾ����   */  // ����Ȩ��     // ����ID               // �˵���                       // �˵�����
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_LIMIT_SET,         dm603_face_limit_select_01,     0,  },      // ϵͳά������
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PASS_M_LMT,        dm603_face_limit_select_02,     0,  },      // �޸����루ϵͳά����
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_LIMIT_RUNTIME,     dm603_face_limit_select_03,     0,  },      // ����ʱ���ѯ
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_LIMIT_RUNTIME_INIT,dm603_face_limit_select_04,     0,  },      // ����ʱ�����㣨��ʼ����
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_LIMIT_INIT,        dm603_face_limit_select_05,     0,  },      // ��ʼ��ϵͳά��
};


//--------------------------------------------------------------------------
const MENUITEM  MenuItem_ParaFace[] = // ��������˵�
{
    // �˵�����     /*// ��ʾ����   */  // ����Ȩ��     // ����ID               // �˵���                       // �˵�����
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PARA_SET,          dm603_face_para_select_01,      0,  },      // ��������
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PASS_M_FAC,        dm603_face_para_select_02,      0,  },      // �޸�����     //NOTE-CXW ����û�У�Ҫ����
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PARA_INIT_P,       dm603_face_para_select_03,      0,  },      // ��ʼ������
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PARA_INIT_C,       dm603_face_para_select_04,      0,  },      // ��ʼ��ѹ��
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_INVT_INIT,         dm603_face_para_select_05,      0,  },      // ��ʼ����Ƶ��
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_MACHINE_CODE,      dm603_face_para_select_06,      0,  },      // �������
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_ABILITY_TEST,      dm603_face_para_select_07,      0,  },      // ��������

};
#define FACE_MACHINE_CODE_INDEX  5  // �������-�˵����


//--------------------------------------------------------------------------
const MENUITEM  MenuItem_ParaGroup[GROUP_MAX] = // ������˵�
{
    // �˵�����     /*// ��ʾ����   */  // ����Ȩ��     // ����ID               // �˵���                   // �˵�����
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PARA_SET_VALUE,    dm603_para_large_01,        0,  },      // {�û�����}
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PARA_SET_VALUE,    dm603_para_large_02,        0,  },      // {ϵͳ����}
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PARA_SET_VALUE,    dm603_para_large_03,        0,  },      // {ѹ������}
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PARA_SET_VALUE,    dm603_para_large_04,        0,  },      // {�������}
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PARA_SET_VALUE,    dm603_para_large_05,        0,  },      // {��������}
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PARA_SET_VALUE,    dm603_para_large_06,        0,  },      // {��������}
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PARA_SET_VALUE,    dm603_para_large_07,        0,  },      // {��������}
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PARA_SET_VALUE,    dm603_para_large_08,        0,  },      // {��˪����}
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PARA_SET_VALUE,    dm603_para_large_09,        0,  },      // {��������}
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PARA_SET_VALUE,    dm603_para_large_10,        0,  },      // {��������}
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PARA_SET_VALUE,    dm603_para_large_11,        0,  },      // {��������̽ͷ}
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PARA_SET_VALUE,    dm603_para_large_12,        0,  },      // {���Լ�����}
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PARA_SET_VALUE,    dm603_para_large_13,        0,  },      // {ѹ��������}
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PARA_SET_VALUE,    dm603_para_large_14,        0,  },      // {����Ƶ�ʱ�}
};
#define PASS_SER_FORBID     2   // MenuItem_ParaFace�ӵڼ��ʼ��ֹά��Ȩ�޲鿴

//--------------------------------------------------------------------------
const MENUITEM  MenuItem_PunpInit[] = // ���ճ�ʼ���˵�
{
    // �˵�����     /*// ��ʾ����   */  // ����Ȩ��     // ����ID               // �˵���                   // �˵�����
    {MENU_ACTION,       /*PASS_NULL,    */  PASS_NULL,      PUNP_INIT_ALL,      msg_punp_init_01,       0,  },      // ��ʼ��ȫ��
    {MENU_ACTION,       /*PASS_NULL,    */  PASS_NULL,      PUNP_INIT_PARA,     msg_punp_init_02,       0,  },      // ��ʼ������
    {MENU_ACTION,       /*PASS_NULL,    */  PASS_NULL,      PUNP_INIT_PASSWD,   msg_punp_init_03,       0,  },      // ��ʼ������
};


/*******************************************************************************
 �� ��: Face_debug()
 �� ��: ���Խ���
 �� ��: --
 �� ��: --
 �� ��: --
*******************************************************************************/
static void Face_debug(U08 *disp_buf)
{
    static U08 s_page[MAX_HMI]; // ҳ���
    U08 s_page_max; // ҳ����
    U16 DebugData;  // ��ʱ�����������

    s_page_max = 2;

    switch (dm603_menu[hmi_t_num])
    {
        case DM603_MENU_CREATE :
            dm603_menu[hmi_t_num] = DM603_MENU_PROC;
            key_delay_time[hmi_t_num]=0;
            dm603_common_disp_flash();
            s_page[hmi_t_num] = 0;

            /* break; */

        case DM603_MENU_PROC :
            if(key_delay_time[hmi_t_num]>=KEY_DELAY)
            {
                /* ���������� */
                if (Main_Key[hmi_t_num] == DM603_KEY_FUNC)
                {
                    dm603_pop_face_status();
                    return;
                }

                /* ���ϻ����·�ҳ */
                if (Main_Key[hmi_t_num] == DM603_KEY_UP)
                {
                    if (s_page[hmi_t_num]-- <= 0) s_page[hmi_t_num] = s_page_max-1;
                    dm603_common_disp_clr_line(ROW2, ROW3, ROW1, ROW4, disp_buf);
                    dm603_common_disp_flash();
                }
                if (Main_Key[hmi_t_num] == DM603_KEY_DOWN)
                {
                    if (++s_page[hmi_t_num] >= s_page_max) s_page[hmi_t_num] = 0;
                    dm603_common_disp_flash();
                    dm603_common_disp_clr_line(ROW2, ROW3, ROW1, ROW4, disp_buf);
                }
            }

        case DM603_MENU_PAINT :

                if(s_page[hmi_t_num]==0)    // ��1ҳ����PID����ֵ
                {
                    disp_string_ptr8("UK:", ROW1, COL_BGN, DCS_NORMAL, disp_buf);
                    disp_value_I16(DebugPID[DEBUG_UK], ROW1, 24, DNS_DIGIT5, disp_buf);


                    disp_value_I16(pv_cell_gas_in(0), ROW1, 72, DNS_FLOAT1|DNS_DIGIT4, disp_buf);       // �����¶�
                    disp_value_I16(pv_cell_evap_temp(0), ROW2, 72, DNS_FLOAT1|DNS_DIGIT4, disp_buf);   // ���������¶�

                    disp_string_ptr8("UP:", ROW2, COL_BGN, DCS_NORMAL, disp_buf);
                    disp_value_I16(DebugPID[DEBUG_UP], ROW2, 24, DNS_DIGIT5, disp_buf);

                    disp_string_ptr8("UI:", ROW3, COL_BGN, DCS_NORMAL, disp_buf);
                    disp_value_I16(DebugPID[DEBUG_UI], ROW3, 24, DNS_DIGIT5, disp_buf);

                    disp_string_ptr8("ST:", ROW3, 72, DCS_NORMAL, disp_buf);
                    disp_value_I16(pv_cell_step_m(0), ROW3, 96, DNS_DIGIT3, disp_buf);

                    disp_string_ptr8("UD:", ROW4, COL_BGN, DCS_NORMAL, disp_buf);
                    disp_value_I16(DebugPID[DEBUG_UD], ROW4, 24, DNS_DIGIT5, disp_buf);

                    disp_string_ptr8("+ST:", ROW4, 64, DCS_NORMAL, disp_buf);
                    disp_value_I16(DebugPID[DEBUG_STEP], ROW4, 96, DNS_DIGIT3, disp_buf);

                }
                else if(s_page[hmi_t_num]==1)   // ��2ҳ������Ƶ����
                {
                    DebugData = invt_info_powerout(INVT_ID_1);
                    disp_string_ptr8("PW:", ROW1, COL_BGN, DCS_NORMAL, disp_buf);
                    disp_value_I16(DebugData, ROW1, 24, DNS_DIGIT5|DNS_FLOAT2, disp_buf);

                }

                dm603_common_disp_arrow(s_page[hmi_t_num], 0, s_page_max, disp_buf);

                /* �˵��� */
//              dm603_common_disp_menu(BMP_FN, dm603_face_return, BMP_NU, NULL, disp_buf);
            break;
    }
}

/******************************************************************************
����:���ػ�����
����:on_off_action-���ػ�����,0-������1-�ػ�
******************************************************************************/
static void dm603_face_onoff(U08 *disp_buf)
{
//  U08 col = 0;
//  static U08 on_off_action;

    #define CAN_NOT         0
    #define CAN_RUN         1
    #define CAN_STOP        2
    #define CAN_NORMAL_WARM 3
    #define CAN_QUICK_WARM  4
    #define CAN_UNION_ON    5

    U08 run_or_stop = CAN_NOT;

    switch (dm603_menu[hmi_t_num])
    {
        case DM603_MENU_CREATE:
            dm603_menu[hmi_t_num] = DM603_MENU_PROC;
            key_delay_time[hmi_t_num]=0;
            dm603_common_disp_flash();
            //break;

        case DM603_MENU_PROC:
            /* --״̬�ж�-- */
            if (status_run==_READY && !fg_warm_complete)
            {
                if (fg_warm_quick)          run_or_stop = CAN_QUICK_WARM;
                else                        run_or_stop = CAN_NORMAL_WARM;
            }
//            else if (FG_FREEZE_ACT && !STEP_IS_ALARM)
//            {
//                run_or_stop = CAN_STOP;     /* ��ģ�������һ�� */
//  //              run_or_stop = CAN_RUN;
//            }
            else if (fg_onoff_union_act)
            {
                run_or_stop = CAN_UNION_ON;     /* ���������� */
            }
            else
            {
                switch (status_run)
                {
                    case _IDLE  : /* break; */
                    case _DELAY : run_or_stop = CAN_RUN;    break;  /* ���� */
                    case _READY : /* break; */
                    case _RUN   : run_or_stop = CAN_STOP;   break;  /* �ػ� */
                    default     : run_or_stop = CAN_NOT;
                }
            }

            /* �������� */
            switch (Main_Key[hmi_t_num])
            {
                case DM603_KEY_FUNC:
                    dm603_face[hmi_t_num] = FACE_MAIN;
                    return;
//                  break;

                case DM603_KEY_OK:
                    if (run_or_stop == CAN_NORMAL_WARM)
                    {
                        fg_warm_quick = TRUE;
//                      DM603_CMD_WARM_CANCEL;  /* ȡ��Ԥ�� */
                        dm603_face[hmi_t_num] = FACE_MAIN;

                        skip_preheat_cnt++;
                        DataAccess_SkipPreheatCnt(DATA_WRITE);
                    }
                    else if (run_or_stop == CAN_STOP)
                    {
                        DM603_CMD_STOP;
                        dm603_face[hmi_t_num] = FACE_MAIN;
                    }
                    else if (run_or_stop == CAN_RUN)
                    {
                        DM603_CMD_RUN;
                        if (!fg_warm_complete)
                        {
                            run_or_stop = CAN_NORMAL_WARM;          /* ��ʾ�Ƿ��˳�Ԥ�� */
                            dm603_common_disp_flash();
                        }
                        else
                        {
                            dm603_face[hmi_t_num] = FACE_MAIN;
                        }
                    }
                    else        // ���ع���ʱ������������
                    {
                        dm603_face[hmi_t_num] = FACE_MAIN;
                    }
                    return;
//                  break;
            }
            //break;

        case DM603_MENU_PAINT:
            /* ��ʾ�� */
            if (run_or_stop == CAN_QUICK_WARM)
            {
                disp_string(msg_mac_warm_quick1, ROW1, 0, DTS_CENTER | DTS_CLR_TRAIL | DTS_CLR_HEAD, disp_buf);
                disp_string(msg_mac_warm_quick2, ROW2, 0, DTS_CENTER | DTS_CLR_TRAIL | DTS_CLR_HEAD, disp_buf);
                disp_string(msg_mac_warm_quick3, ROW3, 0, DTS_CENTER | DTS_CLR_TRAIL | DTS_CLR_HEAD, disp_buf);
            }
            else if (run_or_stop == CAN_NORMAL_WARM)
            {
                disp_string(msg_mac_warm_clue1, ROW1, 0, DTS_CENTER | DTS_CLR_TRAIL | DTS_CLR_HEAD, disp_buf);
                disp_string(msg_mac_warm_clue2, ROW2, 0, DTS_CENTER | DTS_CLR_TRAIL | DTS_CLR_HEAD, disp_buf);
                disp_string(msg_mac_warm_clue3, ROW3, 0, DTS_CENTER | DTS_CLR_TRAIL | DTS_CLR_HEAD, disp_buf);
            }
            else if (run_or_stop == CAN_STOP)
            {
                disp_string(msg_mac_off_clue, ROW2, 0, DTS_CENTER | DTS_CLR_TRAIL | DTS_CLR_HEAD, disp_buf);
            }
            else if (run_or_stop == CAN_RUN)
            {
                disp_string(msg_mac_on_clue, ROW2, 0, DTS_CENTER | DTS_CLR_TRAIL | DTS_CLR_HEAD, disp_buf);
            }
            else if (run_or_stop == CAN_NOT)
            {
                disp_string(msg_mac_onoff_forbid, ROW2, 0, DTS_CENTER | DTS_CLR_TRAIL | DTS_CLR_HEAD, disp_buf);
            }
            else if (run_or_stop == CAN_UNION_ON)
            {
                disp_string(msg_mac_onoff_union, ROW2, 0, DTS_CENTER | DTS_CLR_TRAIL | DTS_CLR_HEAD, disp_buf);
            }

            dm603_common_disp_menu(BMP_FN, dm603_face_return, BMP_OK, dm603_face_para_pass_m_ent,disp_buf);
            break;
    }
}

/*******************************************************************************
 �� ��: dm603_tasking()
 �� ��: ��ʾ������Ƚ���
 �� ��: --
 �� ��: --
 �� ��: --
*******************************************************************************/

void dm603_tasking(U08 *disp_buf)
{
    static U08 face_bak[MAX_HMI];   /* ���汸�ݣ��Է���ˢ�� */

    PARAMITEM *ParamGroup;

    pdisp_buf = disp_buf;
    //if (GET_BIT(hmi_commu_error, COM_HMI) && GET_BIT(hmi_commu_error, COM_HMI_2)) return;
    if (GET_BIT(hmi_commu_error, hmi_t_num)) return;

    dm603_onoff_routine();  /* ���ػ�������� */

    /* �л����� */
    if (face_bak[hmi_t_num] != dm603_face[hmi_t_num])
    {
        disp_clr_screen(disp_buf);
        dm603_common_disp_flash();

        face_bak[hmi_t_num] = dm603_face[hmi_t_num];
        dm603_menu[hmi_t_num] = DM603_MENU_CREATE;
    }

#ifndef DEBUG
    if (cnt_no_operate_sec[hmi_t_num] > LOCK_T_IN_OTHER
        || cnt_menu_lock[hmi_next] == 0 && hmi_t_num != hmi_next)
    {
        if (dm603_face[hmi_t_num] != FACE_OPEN
        && dm603_face[hmi_t_num] != FACE_PRE
        && dm603_face[hmi_t_num] != FACE_OFF)
        {
            dm603_face[hmi_t_num] = FACE_MAIN;
            if (val_lock_screen)    fg_disp_lock = TRUE;    /* ���� */
        }
    }
#endif

    if (dm603_face[hmi_t_num] == FACE_MAIN)     dm603_clr_face_status(hmi_t_num);

    dm603_face_start(disp_buf);

    /* ���� */
    switch (dm603_face[hmi_t_num])
    {
        case FACE_PRE   :               dm603_face_pre(disp_buf);   break;
        case FACE_OFF   :               dm603_face_off(disp_buf);   break;
        case FACE_OPEN  :               dm603_face_open(disp_buf);  break;
//      case FACE_LOCK  :               dm603_face_lock(disp_buf);  break;
        case FACE_MAIN  :               dm603_face_main(disp_buf);  break;
        case FACE_ONOFF  :              dm603_face_onoff(disp_buf); break;
//        case FACE_STATUS_CHECK:         UserFace_DemoCheck();       break;
        //-------------------------------------------------------------------------------------------------------------------------
        case FACE_MENU  :               Face_menu(ITEM_PER_PAGE, MenuItem_MainMenu, sizeof(MenuItem_MainMenu)/sizeof(MENUITEM), 0,disp_buf);
                                        break;


        case FACE_MENU_TIME:            dm603_face_menu_time(disp_buf);     break;  /* ��ʱ���� */
        case FACE_MENU_TIME_SET:        dm603_face_menu_time_set(disp_buf); break;  /* ��ʱʱ������ */
        case FACE_MENU_STATUS:          dm603_face_menu_status(disp_buf);   break;  /* ״̬��ѯ */
        case FACE_MENU_CLOCK:           dm603_face_menu_clock(disp_buf);    break;  /* ʱ�Ӳ�ѯ */

        //-------------------------------------------------------------------------------------------------------------------------
        case FACE_MENU_CLOUD:
                                        if (hmi_board_type[hmi_t_num] == NEW)
                                        {
                                            Face_menu(ITEM_PER_PAGE, MenuItem_Wifi, sizeof(MenuItem_Wifi)/sizeof(MENUITEM), 0,disp_buf);
                                        }
                                        else
                                        {
                                            dm603_face_cloud_tuya(disp_buf);
                                        }
                                        break;
        //-------------------------------------------------------------------------------------------------------------------------
        case FACE_MENU_CLOUD_CMD:       Face_menu(ITEM_PER_PAGE, MenuItem_WifiCMD, sizeof(MenuItem_WifiCMD)/sizeof(MENUITEM), 0,disp_buf);
                                        break;

        //-------------------------------------------------------------------------------------------------------------------------
#ifdef COM_FCU_FBUS
        case FACE_MENU_FCU:             dm603_face_menu_fcu(disp_buf);  break;  /* ������Ϣ */
#endif

        //-------------------------------------------------------------------------------------------------------------------------
        case FACE_MENU_VERSION:         dm603_face_menu_version(disp_buf);      break;  /* �汾��ѯ */

        //-------------------------------------------------------------------------------------------------------------------------
        case FACE_MENU_FUNC:            Face_menu(ITEM_PER_PAGE, MenuItem_Function, sizeof(MenuItem_Function)/sizeof(MENUITEM), 0,disp_buf);
                                        break;

        //-------------------------------------------------------------------------------------------------------------------------
        case FACE_MENU_FUNC_USER:       ParamGroup = PG_GetAddr_ParamPri(0);
                                        dm603_face_para_SetValue(ParamGroup, para_small[0], 0, 0, disp_buf);
                                        break;

        //-------------------------------------------------------------------------------------------------------------------------
        case FACE_MENU_FUNC_DEFROST:    dm603_face_menu_func_defrost(disp_buf); break;  /* �ֶ���˪ */
//      case FACE_MENU_FUNC_UINT:       dm603_face_menu_func_unit(disp_buf);    break;  /* ģ��ʹ������ */
        case FACE_MENU_FUNC_RUNTIME:    dm603_face_menu_func_runtime(disp_buf); break;  /* ����ʱ���ѯ */
        case FACE_MENU_FUNC_HIS_ERR:    dm603_face_menu_func_his_err(disp_buf); break;  /* ��ʷ���ϲ�ѯ */
//      case FACE_ENTER_FLOOR_DRYUP:    dm603_face_enter_floor_dryup(disp_buf); break;  /* ��ů��ɹ��� */
        case FACE_FORCE_IBH:            dm603_face_force_ibh(disp_buf);         break;  /* ǿ�ƿ������õ���� */
        case FACE_FORCE_TBH:            dm603_face_force_tbh(disp_buf);         break;  /* ǿ�ƿ���ˮ������ */
//      case FACE_FORCE_HOT_WATER:      dm603_face_force_hot_water(disp_buf);   break;  /* ǿ������ˮ */
//      case FACE_HOLIDAY_MODE_SET:     dm603_face_holiday_mode_set(disp_buf);  break;  /* �ݼ�ģʽ���� */


        //-------------------------------------------------------------------------------------------------------------------------
        case FACE_PARA:                 Face_menu(ITEM_PER_PAGE, MenuItem_ParaFace, sizeof(MenuItem_ParaFace)/sizeof(MENUITEM), dm603_pass_grade[hmi_t_num],disp_buf);
                                        break;

        //-------------------------------------------------------------------------------------------------------------------------
        case FACE_PARA_SET:             ParamGroup = PG_GetAddr_ParamPri(0);
                                        Face_ParaGroup(ITEM_PER_PAGE, MenuItem_ParaGroup, ParamGroup, 0,disp_buf);
                                        break;

        //-------------------------------------------------------------------------------------------------------------------------
        case FACE_PARA_SET_VALUE:       ParamGroup = PG_GetAddr_ParamPri(ParaGroup_Now[hmi_t_num]);
                                        dm603_face_para_SetValue(ParamGroup, para_small[ParaGroup_Now[hmi_t_num]], 0, 0, disp_buf);
                                        break;

        //-------------------------------------------------------------------------------------------------------------------------
        case FACE_PARA_INIT_P:          dm603_face_para_initP(disp_buf);        break;
        case FACE_PARA_INIT_C:          dm603_face_para_initC(disp_buf);        break;
        case FACE_INVT_INIT:            dm603_face_para_initInvt(disp_buf);     break;
        case FACE_ABILITY_TEST:         dm603_face_enter_ability_test(disp_buf);        break;
        case FACE_ERROR :               dm603_face_error(disp_buf);             break;


        case FACE_LIMIT :               Face_menu(ITEM_PER_PAGE, MenuItem_Limit, sizeof(MenuItem_Limit)/sizeof(MENUITEM), 0,disp_buf);
                                        break;

        //-------------------------------------------------------------------------------------------------------------------------
        case FACE_LIMIT_SET     :       ParamGroup = PG_GetAddr_ParamLimit(0);
                                        dm603_face_para_SetValue(ParamGroup, limit_set_msg, 0, 0, disp_buf);
                                        break;

        //-------------------------------------------------------------------------------------------------------------------------
        case FACE_LIMIT_RUNTIME :       dm603_face_limit_time(disp_buf);        break;
        case FACE_LIMIT_RUNTIME_INIT:   dm603_face_sysruntime_init(disp_buf);   break;
        case FACE_LIMIT_INIT:           dm603_face_limit_init(disp_buf);        break;

        //-------------------------------------------------------------------------------------------------------------------------
        case FACE_PASS  :               dm603_pass_grade[hmi_t_num]=dm603_face_pass(dm603_PassNeed[hmi_t_num],disp_buf);

                                        // ά�������ѽ��棬������ȷ��ֱ�ӽ��ϵͳά�����ܣ����������棨��ά�������ѽ����д���
                                        if(dm603_pass_grade[hmi_t_num]==PASS_LIMIT
                                            && (dm603_face[hmi_t_num]==FACE_OFF || dm603_face[hmi_t_num]==FACE_PRE) )
                                        {
                                            fg_limit_cancel = TRUE;
                                        }
                                        break;

        //-------------------------------------------------------------------------------------------------------------------------
        //case FACE_PASS_M_FAC:         dm603_face_pass_modify(PASS_FAC,disp_buf);  break;      /* �޸ĳ������� */
        case FACE_PASS_M_FAC:           dm603_face_pass_modify(dm603_pass_grade[hmi_t_num],disp_buf);   break;      /* �޸ĳ������� */


        case FACE_PASS_M_USER:          dm603_face_pass_modify(PASS_USER,disp_buf); break;      /* �޸��û����� */
        case FACE_PASS_M_LMT:           dm603_face_pass_modify(PASS_LIMIT,disp_buf);    break;  /* �޸�ϵͳά������ */
        case FACE_MENU_MODE:            dm603_face_menu_func_user_mode(disp_buf);  break;       /* ģʽ���� */
        case FACE_MENU_USER_SHORTCUT:   dm603_face_menu_func_user_shortcut(disp_buf); break;    /* ������� */
        //-------------------------------------------------------------------------------------------------------------------------
//      case DM603_FACE_FORMT :         ParamGroup = PG_GetAddr_ParamModbus(0);
//                                        dm603_face_para_SetValue(ParamGroup,bms_data_msg, 0, 0, disp_buf);
//                                      break;

        //-------------------------------------------------------------------------------------------------------------------------
        case FACE_PUNP_INIT:            Face_menu(ITEM_PER_PAGE, MenuItem_PunpInit, sizeof(MenuItem_PunpInit)/sizeof(MENUITEM), 0,disp_buf);
                                        break;

        //-------------------------------------------------------------------------------------------------------------------------
        case FACE_DEBUG:                Face_debug(disp_buf);   break;
        case FACE_INVT_EXCEPT_INFO:     dm603_face_invt_except_info(disp_buf);  break;  /* ��Ƶ�쳣��Ϣ��ѯ */
        case FACE_MENU_INVT_PARA:       dm603_face_invt_para(disp_buf); break;          /* ��Ƶ�������� */

        default : break;
    }

    dm603_face_end(disp_buf);

}

/*************************************************
������dm603_push_face_status
���ܣ��������״̬�������˻�ʱ�ҵ��ϵ�(�����ڶ�ջӦ��)
    ������治����(ԭ���������һ���˳�ʱӦ���ǻָ�����������
    ǰ�Ľ��棬�����������)
��������
����ֵ����
**************************************************/
static void dm603_push_face_status(void)
{
    dm603_face_bak[hmi_t_num][dm603_face_index[hmi_t_num]] = dm603_face[hmi_t_num];
    dm603_item_bak[hmi_t_num][dm603_face_index[hmi_t_num]] = dm603_item[hmi_t_num];

    if (dm603_face_index[hmi_t_num] < MAX_FACE_LAYER-1) dm603_face_index[hmi_t_num]++;
    dm603_face_dir[hmi_t_num] = FECE_ENTER;
}

/*************************************************
������dm603_pop_face_status
���ܣ���������״̬���˻���һ�����ʱ�ָ�״̬(�����ڶ�ջӦ��)
��������
����ֵ����
**************************************************/
static void dm603_pop_face_status(void)
{
    if (dm603_face_index[hmi_t_num] > 0)    dm603_face_index[hmi_t_num]--;

    dm603_face[hmi_t_num] = dm603_face_bak[hmi_t_num][dm603_face_index[hmi_t_num]];
    dm603_item[hmi_t_num] = dm603_item_bak[hmi_t_num][dm603_face_index[hmi_t_num]];

    dm603_face_dir[hmi_t_num] = FECE_EXIT;
}

/*************************************************
������dm603_set_nextface_status
���ܣ�������һ����״̬�������������������ȷʱ��ת����һ����
��������
����ֵ����
�������˽���״ֻ̬��һ��
**************************************************/
static void dm603_set_nextface_status(U16 FaceId)
{
    dm603_nextface[hmi_t_num]= FaceId;
}

/*************************************************
������dm603_get_nextface_status
���ܣ���ȡ��һ����״̬�������������������ȷʱ��ת����һ����
��������
����ֵ����
��������������������ʱ����dm603_pop_face_status���ؽ���ǰ�Ľ���
**************************************************/
static void dm603_get_nextface_status(void)
{
    dm603_face[hmi_t_num] = dm603_nextface[hmi_t_num];
    dm603_face_dir[hmi_t_num] = FECE_ENTER;
}

/***************************************************************
������dm603_disp_err
���ܣ���ָ��������ʾ���ϣ�ռ������
������err_pcb: ���ϰ壻 err_num�����Ϻţ�err_tag:���ϸ��Ӻ�(��0ֵ) row����ʼ��
���أ���
****************************************************************/
static void dm603_disp_err(U08 err_pcb, U08 err_num, U08 err_tag, U08 row, U08 *disp_buf)
{
    U08 row_err_txt = 0;    /* �����ı���ʾ��λ�� */
    U08 disp_err_num = 0;   /* ��ʾ�Ĺ��Ϻ�(��ʾ�����ֿ��ܺ�ʵ�ʵ�err_num��ͬ) */

    row_err_txt = row + 16;

    if (err_pcb == MAX_PCB)
    {
        disp_string(dm603_face_error_sys, row, 0, DCS_NORMAL , disp_buf);
        if(err_tag==0)
        {
            disp_string(msg_m_error[err_num], row_err_txt, 128, DTS_RIGHT | DTS_CLR_HEAD, disp_buf);
        }
        disp_err_num = err_num;
    }
    else
    {
        disp_value_U08(err_pcb, row, 0, DNS_DIGIT2 | DNS_ZERO | DCS_NORMAL, disp_buf);
        disp_string_ptr8("#", row, 16, DCS_NORMAL, disp_buf);
        disp_string(dm603_face_error_pcb, row, 24, DCS_NORMAL, disp_buf);
        if(err_tag==0)
        {
            disp_string(msg_s_error[err_num], row_err_txt, 128, DTS_RIGHT | DTS_CLR_HEAD, disp_buf);
        }
        else
        {
            INVT_ID_ENUM id;
            for (id=INVT_ID_1; id<INVT_ID_MAX; id++)
            {
                if(err_num==err_invt_id_to_err_id(id, INVT_ERR_ERROR))
                {
                    if(err_tag<=INVT_ERR_ID_MAX && err_tag>=INVT_ERR_ID_MIN)
                    {
                        disp_string(msg_invt_error[err_tag-1], row_err_txt, 128, DTS_RIGHT | DTS_CLR_HEAD, disp_buf);
                    }
                    else    // δ������ϣ���ֹ���Ƶ������δͬ�����������ϳ�����Χ��
                    {
                        disp_string(dm603_face_error_unknown, row_err_txt, 128, DTS_RIGHT | DTS_CLR_HEAD, disp_buf);
                    }
                }
            }
        }
        disp_err_num = err_num;
    }

    /* ���Ϻ� */
    disp_string_ptr8(":", row, 56, DCS_NORMAL, disp_buf);
    disp_value_U08(disp_err_num, row, 64, DNS_DIGIT3 | DNS_ZERO| DCS_NORMAL, disp_buf); /* ���Ϻ� */
    if(err_tag!=0)
    {
        disp_string_ptr8("-", row, 88, DCS_NORMAL, disp_buf);
        disp_value_U08(err_tag, row, 96, DNS_DIGIT3 | DNS_ZERO|DCS_NORMAL, disp_buf);   /* ���Ϻ� */
    }
    else
    {
        disp_string_ptr8("    ", row, 88, DCS_NORMAL, disp_buf);    // �޸��Ӻ������
    }
}


/*******************************************************************************
 �� ��: Face_menu()
 �� ��: �˵��ͽ���(�ǲ�����)
 �� ��: ItemPage ÿҳ�����Ŀ��;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void Face_menu(U08 ItemPage, const MENUITEM *MenuItem,  U08 ItemAll, U08 PassGrade,U08 *disp_buf)   //NOTE-CXW PassGrade���տ��ܲ���Ҫ�������ǰ�����ж�
{
    U08 i = 0;
    U08 row = 0;
    U08 min, max;
    U16 attrib = 0;
    U08 menu_opt = MENU_DOWN;
    U08 cursor = 0;
    static U08 ItemMax = 0;

    PassGrade = PassGrade;  /* N0001 */

    switch (dm603_menu[hmi_t_num])
    {
        case DM603_MENU_CREATE :
            dm603_menu[hmi_t_num] = DM603_MENU_PROC;
            key_delay_time[hmi_t_num]=0;
            if (dm603_face_dir[hmi_t_num] == FECE_ENTER)    /* ���ϲ�������ý���ų�ʼ����Ŀѡ�����(���²���淵��ʱ����) */
            {
                dm603_item[hmi_t_num] = 0;
            }
            ItemMax = Face_menu_hide(ItemAll, Item_tbl[hmi_t_num], MENUITEM_SIZE);
            dm603_common_disp_flash();
            /* break; */

        case DM603_MENU_PROC :
            if(key_delay_time[hmi_t_num]>=KEY_DELAY)
            {
                switch (Main_Key[hmi_t_num])
                {
                    case DM603_KEY_FUNC:    /* ���������� */

                        dm603_pop_face_status();
                        return;
//                      break;

                    case DM603_KEY_OK:
                        if(MenuItem[Item_tbl[hmi_t_num][dm603_item[hmi_t_num]]].MenuType==MENU_FACE)    // �ӽ���˵�
                        {
                            dm603_push_face_status();   /* �������ǰ����״̬ */

                            if (MenuItem[Item_tbl[hmi_t_num][dm603_item[hmi_t_num]]].MenuPassEnt != PASS_NULL )     /* ��Ҫ������ܽ��� */
                            {
                                dm603_set_nextface_status(MenuItem[Item_tbl[hmi_t_num][dm603_item[hmi_t_num]]].FuncId); // ȷ��������ȷ��Ҫ��ת�Ľ���
                                dm603_face[hmi_t_num] = FACE_PASS;  // �����������

                                dm603_PassNeed[hmi_t_num] = MenuItem[Item_tbl[hmi_t_num][dm603_item[hmi_t_num]]].MenuPassEnt;   // ��ǰ���������Ҫ��Ȩ��
                            }
                            else
                            {
                                dm603_face[hmi_t_num] = MenuItem[Item_tbl[hmi_t_num][dm603_item[hmi_t_num]]].FuncId;
                            }
                            return;
                        }
                        if((MenuItem[Item_tbl[hmi_t_num][dm603_item[hmi_t_num]]].MenuType==MENU_ONOFF)      // ����ONOFF�����˵�
                            ||(MenuItem[Item_tbl[hmi_t_num][dm603_item[hmi_t_num]]].MenuType==MENU_ACTION)) // ����ִ�ж����˵�
                        {
                            Face_menu_action(MenuItem[Item_tbl[hmi_t_num][dm603_item[hmi_t_num]]].FuncId, 1);
                        }
                        break;  /* �����л����� */

                    case DM603_KEY_UP:
                    case DM603_KEY_DOWN:
                        if (Main_Key[hmi_t_num] == DM603_KEY_UP)    menu_opt = MENU_UP;
                        if (Main_Key[hmi_t_num] == DM603_KEY_DOWN)  menu_opt = MENU_DOWN;

                        dm603_item[hmi_t_num] = dm603_common_disp_select(dm603_item[hmi_t_num], menu_opt, 0, ItemMax);

                        break;

                    default:
                        break;
                }
            }
//          break;

        case DM603_MENU_PAINT :
            if (Main_Key[hmi_t_num] != KEY_NULL)
            {
                disp_clr_screen(disp_buf);
                dm603_common_disp_flash();
            }

            cursor = dm603_common_disp_bound(dm603_item[hmi_t_num], 0, ItemMax,ItemPage, &min, &max);
            //NOTE-CXW �˲����Ƿ���Ż�������ÿ�ζ���ʾ
            for (i=min; i<max; i++)
            {
                attrib = DCS_NORMAL;
                if ((i-min) == cursor) attrib = DCS_REVERSE;    /* ������ʾ�� */

                row = ROW1 + (i-min)*16;

                // �˵����
                disp_value_U08(i+1,     row, 0,     attrib|DNS_DIGIT2|DNS_ZERO, disp_buf);
                disp_string_ptr8(">", row, 16, attrib, disp_buf);

                // �˵�����
                disp_string(MenuItem[Item_tbl[hmi_t_num][i]].MenuMsg, row, 24, attrib, disp_buf);

                // ����ONOFF�����˵� ��ʾֵ
                if(MenuItem[Item_tbl[hmi_t_num][i]].MenuType==MENU_ONOFF)
                {
#if 0   // ��ʽͼ����û�и�ͼ����ʱ���β���
                    Item_value[hmi_t_num][i-min]=Face_menu_action(MenuItem[Item_tbl[hmi_t_num][i]].FuncId, 0);

                    if(Item_value[hmi_t_num][i-min]==1)
                        disp_bmp(idx_bmp_checkon_16_16, row, 104, attrib, disp_buf);
                    else
                        disp_bmp(idx_bmp_checkoff_16_16, row,104, attrib, disp_buf);
#endif
                }
            }

            /* ��ͷ */
            dm603_common_disp_arrow(dm603_item[hmi_t_num], 0, ItemMax, disp_buf);

            /* �˵��� */
            if(ItemPage<4)
            {
                dm603_common_disp_menu( BMP_FN, dm603_face_return, \
                                        BMP_OK, dm603_face_menu_select_ent, disp_buf);
            }
            break;
    }
}

/*******************************************************************************
 �� ��: Face_ParaGroup()
 �� ��: ���������
 �� ��: ItemPage ÿҳ�����Ŀ��;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void Face_ParaGroup(U08 ItemPage, const MENUITEM *MenuItem,  const PARAMITEM *ParamGroup, U08 PassGrade, U08 *disp_buf)
{
    U08 i = 0;
    U08 row = 0;
    U08 min, max;
    U16 attrib = 0;
    U08 menu_opt = MENU_DOWN;
    U08 cursor = 0;
    static U08 ItemMax = 0;

    PassGrade = PassGrade;  /* N0001 */

    switch (dm603_menu[hmi_t_num])
    {
        case DM603_MENU_CREATE :
            dm603_menu[hmi_t_num] = DM603_MENU_PROC;
            key_delay_time[hmi_t_num]=0;
            if (dm603_face_dir[hmi_t_num] == FECE_ENTER)    /* ���ϲ�������ý���ų�ʼ����Ŀѡ�����(���²���淵��ʱ����) */
            {
                dm603_item[hmi_t_num] = 0;
            }
            ItemMax = Face_ParaGroup_hide(ParamGroup, Item_tbl[hmi_t_num], MENUITEM_SIZE, dm603_pass_grade[hmi_t_num]);
            dm603_common_disp_flash();
            /* break; */

        case DM603_MENU_PROC :
            if(key_delay_time[hmi_t_num]>=KEY_DELAY)
            {
                switch (Main_Key[hmi_t_num])
                {
                    case DM603_KEY_FUNC:    /* ���������� */
                        dm603_pop_face_status();
                        return;
//                      break;

                    case DM603_KEY_OK:
                        if(MenuItem[Item_tbl[hmi_t_num][dm603_item[hmi_t_num]]].MenuType==MENU_FACE)    // �ӽ���˵�
                        {
                            if((MenuItem[Item_tbl[hmi_t_num][dm603_item[hmi_t_num]]].MenuPassEnt == PASS_NULL ) // ����Ҫ��ӦȨ�޼��ɽ���Ĳ�����
                                ||((dm603_pass_grade[hmi_t_num]!=0xFFFF)    // ��ǰȨ����Ч     // ��Ҫ��ӦȨ�޲��ܽ���Ĳ�����
                                    &&((0x0001<<dm603_pass_grade[hmi_t_num]) & MenuItem[Item_tbl[hmi_t_num][dm603_item[hmi_t_num]]].MenuPassEnt ))  // ��ǰȨ�� ƥ�� ����Ȩ��
                              )
                            {
                                dm603_push_face_status();   /* �������ǰ����״̬ */
                                dm603_face[hmi_t_num] = MenuItem[Item_tbl[hmi_t_num][dm603_item[hmi_t_num]]].FuncId;
                                ParaGroup_Now[hmi_t_num] = Item_tbl[hmi_t_num][dm603_item[hmi_t_num]];
                            }

                            return;
                        }

                        break;  /* �����л����� */

                    case DM603_KEY_UP:
                    case DM603_KEY_DOWN:
                        if (Main_Key[hmi_t_num] == DM603_KEY_UP)    menu_opt = MENU_UP;
                        if (Main_Key[hmi_t_num] == DM603_KEY_DOWN)  menu_opt = MENU_DOWN;

                        dm603_item[hmi_t_num] = dm603_common_disp_select(dm603_item[hmi_t_num], menu_opt, 0, ItemMax);

                        break;

                    default:
                        break;
                }
            }
//          break;

        case DM603_MENU_PAINT :
            if (Main_Key[hmi_t_num] != KEY_NULL)
            {
                disp_clr_screen(disp_buf);
                dm603_common_disp_flash();
            }

            cursor = dm603_common_disp_bound(dm603_item[hmi_t_num], 0, ItemMax,ItemPage, &min, &max);
            //NOTE-CXW �˲����Ƿ���Ż�������ÿ�ζ���ʾ
            for (i=min; i<max; i++)
            {
                attrib = DCS_NORMAL;
                if ((i-min) == cursor) attrib = DCS_REVERSE;    /* ������ʾ�� */

                row = ROW1 + (i-min)*16;

                // �˵����
                disp_value_U08(i+1,     row, 0,     attrib|DNS_DIGIT2|DNS_ZERO, disp_buf);
                disp_string_ptr8(">", row, 16, attrib, disp_buf);

                // �˵�����
                disp_string(MenuItem[Item_tbl[hmi_t_num][i]].MenuMsg, row, 24, attrib, disp_buf);
            }

            /* ��ͷ */
            dm603_common_disp_arrow(dm603_item[hmi_t_num], 0, ItemMax, disp_buf);

            /* �˵��� */
            if(ItemPage<4)
            {
                dm603_common_disp_menu( BMP_FN, dm603_face_return, \
                                        BMP_OK, dm603_face_menu_select_ent, disp_buf);
            }
            break;
    }
}

/*******************************************************************************
 �� ��: Face_menu_hide()
 �� ��: ����˵�����
 �� ��:
 �� ��: ��
 �� ��: ��
*******************************************************************************/
U08 Face_menu_hide(U08 ItemAll, U08 *Item_tbl, U08 Tbl_size)
{
    U08 n;
    U08 cur;

    cur = 0;

    for (n = 0; n < ItemAll; n++) /* �����Ƿ����� */
    {

        switch (dm603_face[hmi_t_num])
        {
            case FACE_MENU:
                if (n==FACE_MENU_CLOUD_INDEX)
                {
                    BOL is_wifi_hmi = FALSE;
                    if (hmi_type[hmi_t_num] == HMI_TYPE_TEXT_WIFI_TY)
                    {
                        is_wifi_hmi = TRUE;
                    }

                    if (!is_wifi_hmi) continue;
                }
                else if (n==FACE_MENU_FCU_INDEX)
                {
                    BOL is_fcu_hmi = FALSE;
                    if (val_fcu_num > 0)
                    {
                        is_fcu_hmi = TRUE;
                    }

                    if (!is_fcu_hmi) continue;
                }

                break;

            case FACE_PARA:
                if (n==FACE_MACHINE_CODE_INDEX && hmi_board_type[hmi_t_num]!= NEW) continue;    // ����û�л������˵�
                if (dm603_pass_grade[hmi_t_num] == PASS_SER && n>=PASS_SER_FORBID)continue;     // ����ʵ������޸�
                break;

            default:
                break;
        }

        if(cur<Tbl_size) // ��ֹ����
        {
            Item_tbl[cur++] = n;
        }
    }

    return cur; // ���ز˵�������
}


/*******************************************************************************
 �� ��: Face_menu_action()
 �� ��: �˵��������
 �� ��: Act_opt=0������ֵ   Act_opt=1��ִ�ж�����������ֵ
 �� ��: value 1�����ò����ද�������ز���ֵ��2�������ò����ද��������0��
 �� ��: ��
*******************************************************************************/
I16 Face_menu_action(U16 FuncID, U08 Act_opt)
{
    I16 value=0;
//    PARAMITEM *ParamGroup;            // ������


    switch(FuncID)
    {
//      case SET_SV_ECONOMY_MODE:   // ���ý���ģʽ
//          ParamGroup = PG_GetAddr_ParamPri(0);
//          para_get_val(ParamGroup, 4, &value);    // ��ȡ����ֵ
//          if(Act_opt==1)
//          {
//              if (!para_is_nexch(ParamGroup, 4))
//              {
//                  value = (value==0)?1:0;
//                  para_set(ParamGroup, 4, value);
//              }
//          }
//          break;

//      /**ģ�壺�޸�ONOFF����********************************/
//      case SET_XXX:
//
//          para_get_val(PG_GetAddr_ParamPri(n), m, &value);;   // ��ȡ����ֵ�����ݲ�ͬ��������ȡֵ�ķ������ܲ�һ�����ǲ�����Ķ���������value=0;
//          if(Act_opt==1)
//          {
//              if (/*���������޸�*/)
//              {
//                  value = (value==0)?1:0; // �޸Ĳ���
//                  para_set(n, m, value);  // �������
//              }
//          }
//          break;
//      /**ģ�壺�޸�ONOFF����end*****************************/


        case PUNP_INIT_ALL: // ��ʼ��ȫ��
            if(Act_opt==1)
            {
                DM603_CMD_ALLDATA_INIT;
            }
            break;
        case PUNP_INIT_PARA:    // ��ʼ������
            if(Act_opt==1)
            {
                DM603_CMD_PARA_INIT;
            }
            break;
        case PUNP_INIT_PASSWD:  // ��ʼ������
            if(Act_opt==1)
            {
                DM603_CMD_PASSWORD_INIT;
            }
            break;

        default:
            break;
    }

    return value;
}
#endif

/*************************************************
������dm603_clr_face_status
���ܣ��������״̬(������ʱ�������)
��������
����ֵ����
**************************************************/
static void dm603_clr_face_status(U08 hmi_num)
{
    U08 i;

    dm603_face_index[hmi_t_num] = 0;
    dm603_face_dir[hmi_t_num] = FECE_ENTER;

    for (i=0; i<MAX_FACE_LAYER; i++)
    {
        dm603_face_bak[hmi_num][i] = FACE_MAIN;
        dm603_item_bak[hmi_num][i] = 0;
    }
}

/**
 * ���� : dm603_init()
 * ���� : ��ʼ��
 *
 * ���� : --
 * ���� : --
 *
 * ��ע : --
 */
void dm603_init(void)
{
    U08 i;

//    DataAccess_Timer(DATA_READ);        // ��ȡ��ʱʱ��
    DataAccess_HistoryErr(DATA_READ);   // ��ȡ��ʷ������Ϣ
    DataAccess_Language(DATA_READ);     // ��ȡ������Ϣ
    DataAccess_Temp_unit(DATA_READ);     // ��ȡ�¶ȵ�λ��Ϣ
    DataAccess_InvtExceptInfo(DATA_READ, _MAX_(U08));   // ��ȡ��Ƶ�쳣��Ϣ
//    DataAccess_SkipPreheatCnt(DATA_READ);   /* ��ȡ����Ԥ�ȴ��� */
//    DataAccess_holiday(DATA_READ);

    if (dm603_language != LANGUE_CHINESE && dm603_language != LANGUE_ENGLISH)//���Բ��ԣ�ֱ�ӳ�ʼ��Ϊ����
    {
        dm603_language = LANGUE_CHINESE;
        DataAccess_Language(DATA_WRITE);    /* ��ȡ������Ϣ */
    }
    disp_set_langue(dm603_language);

    for (i = 0; i < MAX_HMI; i++)
    {
        /* ��ֹ�����������ʱ */
        key_continuous_time[i] = 0;

        dm603_fg_one_day[i] = FALSE;        /* һ�����ѱ�־ */
//      dm603_fg_screen[i] = FALSE;         /* ������־ */
        dm603_sound[i] = 0;                 /* ���� */
        dm603_error_bak[i] = 0;             /* ���ϱ�ʶ���� */
//      dm603_fg_lock_clue[i] = FALSE;      /* ������ʾ��־ */
        dm603_face[hmi_t_num] = FACE_OPEN;
        fg_disp_ver_back[i] = FALSE;
        dm603_clr_face_status(i);
    }
}

/*******************************************************************************
 �� ��: blink_time()
 �� ��: ��˸ʱ��(��1S��˸���ɼ�������Ҫʱ��)
 �� ��: ��
 �� ��: ��
 �� ��: f_blink_1S 1���л��ı�־
*******************************************************************************/
void blink_time(void)
{
    if (F_TIME_BASIC_SEC_BACK)
    {
        if (f_blink_1S == FALSE)
        {
            f_blink_1S = TRUE;
        }
        else
        {
            f_blink_1S = FALSE;
        }
    }
}

/******************************************************
������disp_time
���ܣ���ʾ�еļ�ʱ��
��������
����ֵ����
*****************************************************/
void disp_time(void)
{
    U08 i;

    if (F_TIME_BASIC_100MS_BACK)    /* 100���� */
    {
        for (i=0; i<MAX_HMI; i++)
        {
            if (cnt_key_static[i] != 0xff)      cnt_key_static[i]++;
            if (cnt_cntnus_add[i] != 0xff)      cnt_cntnus_add[i]++;

            if (dm603_cnt_key_space[i] != 0xff)     dm603_cnt_key_space[i]++;
            if (dm603_cnt_key_updwn[i] != 0xff)     dm603_cnt_key_updwn[i]++;

            if (key_delay_time[i] != 0xff)          key_delay_time[i]++;
        }

    }
    if (F_TIME_BASIC_SEC_BACK)      /* 1�� */
    {
        for (i=0; i<MAX_HMI; i++)
        {
            if (cnt_disp_steady[i] != 0xff)     cnt_disp_steady[i]++;
            if (cnt_non_commu[i] != 0xff)       cnt_non_commu[i]++;
            if (cnt_screen_power[i] != 0)       cnt_screen_power[i]--;  /* ����ʱ */
            if (cnt_menu_lock[i] != 0)          cnt_menu_lock[i]--;     /* ����ʱ */
            if (cnt_lock_clue[i] != 0)          cnt_lock_clue[i]--;     /* ����ʱ */

            if (cnt_no_operate_sec[i] != 0xff)  cnt_no_operate_sec[i]++;
            _CNT(dm603_alarm_sound_cnt[i]);     // ������

            _CNT(dm603_power_time[i]);
            _CNT(dm603_hmi_online[i]);
        }
        if (cnt_com_forbid != 0xff)     cnt_com_forbid++;       /*  */

    }
    if (F_TIME_BASIC_MIN_BACK)      /* 1�� */
    {
        for (i=0; i<MAX_HMI; i++)
        {
            if (cnt_no_operate[i] != 0xff)      cnt_no_operate[i]++;
        }
    }

    blink_time();
}

#define LINE2ROW(line)    (line==(U08)LNULL?RNUL:line*16)
#define POS2COL(pos)      ((U16)pos*8)
#define CHECK_DISPBUF()   do{if(pdisp_buf==NULL)return;}while(0)

/*****************************************************************************
��  �� : PunpDispFlashAll()
��  �� : ˢ�����пؼ�
��  �� : ��
��  �� : ��
��  �� : �޸Ŀؼ�����ǰ,��Ҫ��ˢ�����пؼ�
*****************************************************************************/
void PunpDispFlashAll(void)
{
    dm603_common_disp_flash();
}

/*****************************************************************************
��  �� : PunpDispClearScreen()
��  �� : ����
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void PunpDispClearScreen(void)
{
    CHECK_DISPBUF();
    disp_clr_screen(pdisp_buf);
    dm603_common_disp_flash();
}

/*****************************************************************************
��  �� : PunpDispClearLine()
��  �� : ����,���һ�ο���4��
��  �� : U08 line1 : �к�
         U08 line2 : �к�
         U08 line3 : �к�
         U08 line4 : �к�
��  �� : ��
��  �� : �к���˳��Ҫ��,������д���LNULL
*****************************************************************************/
void PunpDispClearLine(U08 line1, U08 line2, U08 line3, U08 line4)
{
    CHECK_DISPBUF();
    dm603_common_disp_clr_line(LINE2ROW(line1), LINE2ROW(line2), LINE2ROW(line3), LINE2ROW(line4), pdisp_buf);
}

/*****************************************************************************
��  �� : PunpDispAscii()
��  �� : ��ʾASCII�ַ���
��  �� : U08* string_ptr : ASCII�ַ���ָ��
         U16 line        : �к�
         U16 pos         : �к�
         U16 attrib      : ��ʾ����
��  �� : ��
��  �� : ��
*****************************************************************************/
void PunpDispAscii(U08* string_ptr, U16 line, U16 pos, U16 attrib)
{
    CHECK_DISPBUF();
    disp_string_ptr8(string_ptr, LINE2ROW(line), POS2COL(pos), attrib, pdisp_buf);
}

/*****************************************************************************
��  �� : PunpDispNum()
��  �� : ��ʾ��ֵ
��  �� : I16 num    : ��ֵ
         U16 line   : �к�
         U16 pos    : �к�
         U16 format : ��ֵ��ʽ
         U16 attrib : ��ʾ����
��  �� : ��
��  �� : ��
*****************************************************************************/
void PunpDispNum(I16 num, U16 line, U16 pos, U16 format, U16 attrib)
{
    CHECK_DISPBUF();
    disp_value_16(num, LINE2ROW(line), POS2COL(pos), format, attrib, pdisp_buf);
}

/*****************************************************************************
��  �� : PunpDispPv()
��  �� : ��ʾ��ֵ
��  �� : I16 pv     : ģ����
         U16 line   : �к�
         U16 pos    : �к�
         U16 format : ��ֵ��ʽ
         U16 attrib : ��ʾ����
��  �� : ��
��  �� : ��
*****************************************************************************/
void PunpDispPv(I16 pv, U16 line, U16 pos, U16 format, U16 attrib)
{
    static const U08 pv_break[]={"..*.."};
    static const U08 pv_short[]={"..-.."};
    static const U08 pv_cannotuse[]={"....."};
    const U08 * pv_text = NULL;

    CHECK_DISPBUF();
    if(pv == SENSOR_BREAKED)        pv_text = pv_break;
    else if(pv == SENSOR_SHORTED)   pv_text = pv_short;
    else if(!SensorCanUse(pv))      pv_text = pv_cannotuse;

    if(pv_text != NULL)
    {
        disp_string_ptr8(pv_text, LINE2ROW(line), POS2COL(pos), attrib, pdisp_buf);
    }
    else
    {
        disp_value_16(pv, LINE2ROW(line), POS2COL(pos), format, attrib, pdisp_buf);
    }
}

/*****************************************************************************
��  �� : PunpDispText()
��  �� : �����ִ����,��ʾ�û��Զ����ַ���
��  �� : U16 string_index : �û��Զ����ı����
         U16 line         : �к�
         U16 pos          : �к�
         U16 attrib       : ��ʾ����
��  �� : ��
��  �� : ��
*****************************************************************************/
void PunpDispText(U16 string_index, U16 line, U16 pos, U16 attrib)
{
    CHECK_DISPBUF();
    disp_string(string_index, LINE2ROW(line), POS2COL(pos), attrib, pdisp_buf);
}

/*****************************************************************************
��  �� : PunpDispBmp()
��  �� : ����ͼƬ������ʾͼƬ
��  �� : U16 bmp_code : ͼƬ����
         U16 line     : �к�
         U16 pos      : �к�
         U16 attrib   : ��ʾ����
��  �� : ��
��  �� : ��
*****************************************************************************/
void PunpDispBmp(U16 bmp_code, U16 line, U16 pos, U16 attrib)
{
    CHECK_DISPBUF();
    disp_bmp(bmp_code, LINE2ROW(line), POS2COL(pos), attrib, pdisp_buf);
}

/*****************************************************************************
��  �� : PunpDispMenu()
��  �� :
��  �� : U16 licon :
         U16 lstr  :
         U16 ricon :
         U16 rstr  :
��  �� : ��
��  �� : ��
*****************************************************************************/
void PunpDispMenu(U16 licon, U16 lstr, U16 ricon, U16 rstr)
{
    CHECK_DISPBUF();
    dm603_common_disp_menu(licon, lstr, ricon, rstr, pdisp_buf);
}
/*****************************************************************************
��  �� : PunpDispSetLanguage()
��  �� : �趨����
��  �� : U08 language : ����
��  �� : ��
��  �� : ��
*****************************************************************************/
void PunpDispSetLanguage(U08 language)
{
    if (language <= LANGUE_ENGLISH)
    {
        dm603_language = language;
    }
}

/*****************************************************************************
��  �� : PunpDispGetLanguage()
��  �� : ��ȡ��ǰ����
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
U08 PunpDispGetLanguage(void)
{
    return dm603_language<=LANGUE_ENGLISH ? dm603_language:LANGUE_CHINESE;
}
#if 0
/*****************************************************************************
��  �� : PunpDispSetLcdAutoDarkTime()
��  �� : �����Զ��ر���ʱ��
��  �� : U16 time_out : 0��ʾ���Զ��ر���,��Χ10~6000��
��  �� : ��
��  �� : ��
*****************************************************************************/
void PunpDispSetLcdAutoDarkTime(U16 time_out)
{
    if (time_out == 0
        || (time_out >= 10 && time_out <= 6000))
    {
        time_auto_dark = time_out;
    }

}
/*****************************************************************************
��  �� : PunpDispSetLcdLight()
��  �� : ������ʾ�����⿪��
��  �� : U08 sta : ����
��  �� : ��
��  �� : ��
*****************************************************************************/
void PunpDispSetLcdLight(U08 sta)
{
    disp_lcd_light(sta);
}

/*****************************************************************************
��  �� : PunpDispSetSound()
��  �� : ������ʾ��������
��  �� : U08 sta : ����������
��  �� : ��
��  �� : ��
*****************************************************************************/
void PunpDispSetSound(U08 sta)
{
    fg_sound = sta;
}

/*****************************************************************************
��  �� : PunpDispGetClock()
��  �� : ��ȡ��ʾ��ʱ��
��  �� : ��
��  �� : ʱ�����ݽṹ
��  �� : ��
*****************************************************************************/
RTC PunpDispGetClock(void)
{
    return clock[hmi_r_num];
}

/*****************************************************************************
��  �� : PunpDispSetClock()
��  �� : ������ʾ��ʱ��
��  �� : RTC* pClock : ʱ��Դ���ݽṹָ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void PunpDispSetClock(RTC* pClock)
{
    CHECK_DISPBUF();
    disp_wr_hmi_clk(pdisp_buf, *pClock);
}
#endif
/*****************************************************************************
��  �� : PunpDispGetKey()
��  �� : ��ȡ��ʾ��������Ϣ
��  �� : ��
��  �� : ������Ϣ
��  �� : ��
*****************************************************************************/
KEY PunpDispGetKey(void)
{
    return Key[hmi_t_num];
}

/*******************************************************************************
 �� ��: dm603_get_vfd_ver()
 �� ��:
 �� ��: ver_str--;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void dm603_get_vfd_ver(U08 *ver_str)
{
    U08 ver[32] = "X1.VF$$$$.$##.V100###";                  /* ����X1.VF250A.F01.V100A00 */
    char symbol[] = {'X', 'K', 'F', 'T', 'Q'};              /* ϴ�ӡ��յ���������ͨ�á����� */

    U16 func_code = invt_info_function(INVT_ID_1);
    U16 ver_code = invt_info_version(INVT_ID_1);

    if (func_code == (U16)INVT_PARA_VALUE_NULL
        && ver_code == (U16)INVT_PARA_VALUE_NULL
        )
    {
        ver_str[0] = '0';
        ver_str[1] = '.';
        ver_str[2] = '0';
    }
    else
    {
        MEMCPY(&ver_str[0], &ver[0], 5);

        /* �汾��ת�� */
        ver_str[5] = GET_DIGIT(func_code, 4)+'0';
        ver_str[6] = GET_DIGIT(func_code, 3)+'0';
        ver_str[7] = GET_DIGIT(func_code, 2)+'0';
        ver_str[8] = GET_DIGIT(func_code, 1)-1+'A';
        ver_str[9] = '.';
        ver_str[10] = symbol[GET_DIGIT(func_code, 0)-1];

        ver_str[11] = GET_DIGIT(ver_code, 4)+'0';
        ver_str[12] = GET_DIGIT(ver_code, 3)+'0';

        MEMCPY(&ver_str[13], &ver[13], 5);

        if (GET_DIGIT(ver_code, 2))
        {
            ver_str[18] = GET_DIGIT(ver_code, 2)-1+'A';
            ver_str[19] = GET_DIGIT(ver_code, 1)+'0';
            ver_str[20] = GET_DIGIT(ver_code, 0)+'0';
        }
        else
        {
            ver_str[18] = GET_DIGIT(ver_code, 1)-1+'A';
            ver_str[19] = GET_DIGIT(ver_code, 0)+'0';
            ver_str[20] = 0;
        }
    }
}

/* ��ӡ������Ϣ */
#ifdef USE_LOG_PRINT
extern PARAMITEM  ParamPri[];
const U08 str_unit[][20]=
{
 {""     },
 {"��"   },
 {"A"    },
 {"bar"  },
 {"%"    },
 {"Hz"   },
 {"rps"  },
 {"rpm"  },
 {"s"   },
 {"m"   },
 {"h"   },
 {"d"   },
 {"st"   },
 {"V"    },
 {"L/H"  },
};

void DBG_PrintText(U08 str[])
{
    LOG_LOGGINGFPRINT("s", str);
}

U16 DBG_GetPowerTime(void)
{
    U16 volatile time_wait = power_time;
    return time_wait;
}

U16 Fun_FormatNum(U08* buf, U16 num, U16 attrib)
{
    return Fun_NumFormat(num, buf, attrib);
}

// ƴ���ַ���
U16 DBG_StrCat(U08* str1, const U08* str2)
{
    STRCAT(str1, str2);
    return STRLEN(str2);
}

#ifndef __PNI2GBK_H__
#include "pni2gbk.c"
#include "pni2gbk.h"
#endif

U16 DBG_GetDM603GBK(U08 str[], U16 str_idx)
{
#ifdef USE_GBK
    U08 const *temp_ptr = IDX_TO_C_PTR(str_idx);/* ��ȡ�׵�ַ */
#elif defined (USE_UNICODE)
    U08 const *temp_ptr = chinese_uncompress_text(str_idx);/* ��ȡ�׵�ַ */
#endif    

    U16 n=0;
    U08 high, low;
    U16 hzk=0;
    U16 gbk=0;
#define ASC_END 0x80

    do
    {
        low = *temp_ptr++;
        high = *temp_ptr++;
        hzk = MAKEWORD(high, low);

        if (hzk==0 || hzk==0xffff)    break;
        
        gbk = pni2gbk_tab[hzk];

        /* ���� */
        if (gbk >= ASC_END)
        {
            str[n++] = HIBYTE(gbk);
            str[n++] = LOBYTE(gbk);
        }
        /* Ӣ�� */
        else
        {
            str[n++] = LOBYTE(gbk);
        }
    }
    while (1);

    str[n] = '\0';
        
    return n;
}



// ��ӡ����
void DBG_LoggingPara(void)
{
    U16 grp;
    U16 item;
    U16 para_grp_len;
    const SMALL* para_grp;
    static U08 logged = FALSE;
    static U08 str[MAX_LOG_BUF];
    U16 mb_addr;
    I16 min,max;
    U16 dot;
    U16 unit;


    if (logged)          return;

    while(DBG_GetPowerTime() < 10) {WDG_Refresh();}

    
    logged = TRUE;
    DBG_PrintText("name\tModbus\tmax\tmin\tdot\tunit\thide\tdict\r\n");



    for (grp=0; grp<ARRAY_ELEMENTS(para_small); grp++)
    {
        U16 idx=0;
        PARAMITEM* pgrp = &ParamPri[grp];
        para_grp = para_small[grp];
        para_grp_len = ParamPri[grp].items;

        for (item=0; item<para_grp_len; item++)
        {
            PARAM* pitems = &pgrp->array[item];
            const SMALL* para_grp_item = &para_grp[item];

			if ((ParamPri[grp].sign & D_HIDE))                  continue;   /* �������صĲ����� */
//            if ((ParamPri[grp].array[item].sign & D_HIDE))      continue;   /* �������صĲ��� */  
            if (P_isP(grp, item, val_mac_model))                continue;	/* ���������ͺ� */

            idx = 0;
            // ������
            idx += DBG_GetDM603GBK(&str[idx], para_grp_item->msg);//������gbk
            idx += DBG_StrCat(&str[idx], "\t");//"

            // ������Ϣ
//	        mb_addr = ADDR_PARA_BGN + (grp*GROUP_SIZE) + item;
//            mb_addr = BMS_PARA_START + (grp*GROUP_SIZE) + item;
            mb_addr = ADDR_PARA_BGN + (grp*PARA_SIGLE_NUM) + item;

            
            min = pitems->min;
            max = pitems->max;
            dot = pitems->sign & D_DOTMASK;
            unit = (pitems->sign & UINT_MASK) >> 4;
            
            idx += Fun_FormatNum(&str[idx], mb_addr, FNS_UNSIGNED);
            idx += DBG_StrCat(str, "\t");//Modbus��ַ
            
            idx += Fun_FormatNum(&str[idx], max, FNS_FLOAT(dot));
            idx += DBG_StrCat(str, "\t");//���ֵ
            
            idx += Fun_FormatNum(&str[idx], min, FNS_FLOAT(dot));
            idx += DBG_StrCat(str, "\t");//��Сֵ
            
            idx += Fun_FormatNum(&str[idx], dot, FNS_UNSIGNED);
            idx += DBG_StrCat(str, "\t");//С��λ
            
            idx += DBG_StrCat(str, str_unit[unit]);//��λ
            idx += DBG_StrCat(str, "\t");//
            
            idx += DBG_StrCat(str, (U08*)((ParamPri[grp].array[item].sign & D_HIDE)?"Yes":"No"));//����
            idx += DBG_StrCat(str, "\t");

            // �����ֵ�
            if (para_grp_item->pmsg != NULL)
            {
                U16 dict_num = max;
                U16 d;

                idx += DBG_StrCat(&str[idx], "{");// {

                for (d=0; d<dict_num; d++)
                {
                    idx += DBG_StrCat(&str[idx], "\"");//"
                    idx += Fun_FormatNum(&str[idx], d, FNS_UNSIGNED);// �����ֵ�key
                    idx += DBG_StrCat(&str[idx], "\":\"");//":"
                    idx += DBG_GetDM603GBK(&str[idx], para_grp_item->pmsg[d]);//�����ֵ��ַ���Unicode
                    idx += DBG_StrCat(&str[idx], "\"");//"

                    if (d+1 != dict_num)
                    {
                        idx += DBG_StrCat(&str[idx], ",");// ,
                    }
                }

                idx += DBG_StrCat(&str[idx], "}");//}
            }

            idx += DBG_StrCat(&str[idx], "\r\n");//}

            if (idx >= ARRELE(str))
            {
                DBG_PrintText("str FULL!\r\n");
                return;
            }
            DBG_PrintText(str);
        }

        DBG_PrintText("\r\n");//ÿ�黻��
    }

    while(1)    {WDG_Refresh();}
}

#endif


