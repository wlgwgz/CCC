/**********************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司
 All rights reserved.

 文件名称 : dm603.c
 文件描述 : 处理dm603显示屏界面

 创建人   : 袁星
 创建日期 : 2013.09.24

 修改人   : 陈晓雯
 修改日期 : 2017.02.05
 ============================ 修改记录 ===============================
 修改人  : --
 程序版本: --
 完成日期: --
 修改内容: --

**********************************************************************/

#define DM603_GLOBAL

#include "includes.h"

/* 内部变量定义 */
#define MAX_FACE_LAYER  5       /* 界面层数(界面层数增加时，需要更改该定义) */

#define FECE_ENTER          0               /* 进入下一层界面 */
#define FECE_EXIT           1               /* 退回上一层界面 */

#define     PARA_SELECT     0   /* 选中参数项 */
#define     PARA_SETTING    1   /* 选中参数值 */

#ifdef USE_HMI_DISP
static U08 dm603_menu_sub[MAX_HMI];         /* 子界面 */
static U08 dm603_sub_bak[MAX_HMI];          /* 备份 */
static U08 dm603_menu[MAX_HMI];             /* 界面菜单 */
static U08 dm603_item[MAX_HMI];             /* 界面选项 */
//static U08 dm603_cursor[MAX_HMI];         /* 界面光标 */
//static U08 dm603_sub_item[MAX_HMI];           /* 子界面选项 */


/* 界面状态 */
static U08 FaceStatus[MAX_HMI];         /* 界面状态 */
static U08 FaceStatusBak[MAX_HMI];      /* 界面状态备份 */

static U08 dm603_pass_grade[MAX_HMI];       /* 密码等级，指明输入密码后的界面入口 */
static U08 dm603_pass_buf[MAX_PASS_BIT];    /* 密码缓存 */
static U08 dm603_pass_new[MAX_PASS_BIT];    /* 密码更新 */
static U08 dm603_pass_bak[MAX_PASS_BIT];    /* 密码备份 */

static BOOL fg_limit_cancel = FALSE;
/* 内部变量声明 */
//static U08 para_n_tbl[GROUP_MAX];
static U08 para_m_tbl[GROUP_SIZE];  //NOTE-CXW 会否其它参数组的大小超过GROUP_SIZE，比如联网通讯参数组的大小？怎样做防错？

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

/* 定时设置选项 */
const U16 msg_menu_time_select[] =
{
    dm603_face_time_func_select_01, /* {定时设置} */
    dm603_face_time_func_select_02, /* {一次定时} */
    dm603_face_time_func_select_03, /* {01组定时} */
    dm603_face_time_func_select_04, /* {02组定时} */
    dm603_face_time_func_select_05, /* {03组定时} */
    dm603_face_time_func_select_06, /* {04组定时} */
    dm603_face_time_func_select_07, /* {05组定时} */
    dm603_face_time_func_select_08, /* {06组定时} */
};

#endif

U08 dm603_error_bak[MAX_HMI];       /* 故障标识备份 */
U08 dm603_fg_one_day[MAX_HMI];      /* 一天提醒标志 */

U08 dm603_face_bak[MAX_HMI][MAX_FACE_LAYER];        /* 界面ID备份 */
U08 dm603_item_bak[MAX_HMI][MAX_FACE_LAYER];        /* 界面选项备份 */
U08 dm603_face_index[MAX_HMI];              /* 界面层次索引 */
U08 dm603_face_dir[MAX_HMI];            /* 界面方向(进入、退出，用于保存选项和光标) */

static U08* pdisp_buf;
static U16 dm603_power_time[MAX_HMI];       /* 上电时间 */

//static U08 dm603_fg_screen[MAX_HMI];      /* 屏保标志 */

static U08 dm603_hmi_online[MAX_HMI];


//static U08 ModeBlink[MAX_HMI];                /* 模式闪烁处理*/
//static U08 fg_economy_blink_bak[MAX_HMI]; /* 节能模式闪烁处理*/
//static U08 time_index;            /* 定时设置索引 */

//BOOL fg_machine_need_open = FALSE;  /* 机组需要开启(停机时开机和防冻时开机使用) 调试 */










//U08 fg_test = 0;      /* 测试程序标识 */



/* 图标快捷定义 */
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


/* 参数设置信息 */
/* 参数小项 */
#define para_val_00_00      PNULL
#define DNULL               0xffff
const U16  para_val_01_01[] =
{
    msg_para_val_00_00_00,
    msg_para_val_01_01_01,    /* 01 {制冷模式} */
    msg_para_val_01_01_02,    /* 02 {制热模式} */
    msg_para_val_01_01_03,    /* 03 {自动模式} */
};

const U16 Msg_UseOrNot[] =
{
    msg_use_or_not_01,          /* 01 {禁用} */
    msg_use_or_not_02,          /* 02 {使用} */
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
    msg_input_normal_01,            /* 01 {常开} */
    msg_input_normal_02,            /* 02 {常闭} */
};


const U16 para_val_01_06[] =
{
    msg_para_val_01_06_01,    /* 01 {一直运行} */
    msg_para_val_01_06_02,    /* 02 {间歇运行} */
};

const U16 Msg_CtrlObject[] =
{
    msg_ctrl_object_01, /* 01 {回水} */
    msg_ctrl_object_02, /* 02 {出水} */
};

const U16 Msg_MotorExciType[] =
{
    msg_motor_ExciType_01,  /* 01 {四相八拍} */
    msg_motor_ExciType_02,  /* 02 {四相四拍} */
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
    msg_motor_order_01,   /* 01 { 辅阀优先} */
    msg_motor_order_02,   /* 02 { 主阀优先} */
    msg_motor_order_03,   /* 03 { 主辅同时} */
};
const U16 para_val_15_00[]=
{
    msg_para_val_15_00_00,//{单暖}
    msg_para_val_15_00_01,//{冷暖}
    msg_para_val_15_00_02,//{单冷}
};

const U16 Msg_MotorUseType[] =      /* 主阀使用设置 */
{
    msg_motor_type_00,          // 禁用
    msg_motor_type_01,          // 吸气过热度
    msg_motor_type_02,          // 排气过热度
};

const U16 Msg_S_MotorUseType[] = /* 辅路阀使用设置 */
{
    msg_s_motor_type_00,            // 禁用
    msg_s_motor_type_01,            // 经济器过热度
    msg_s_motor_type_02,            // 温度排气过热度
    msg_s_motor_type_03,            // 压力排气过热度
    msg_s_motor_type_04,            // 主路小阀
};

const U16 Msg_RefrigeType[] =       /* 冷媒类型 */
{
    msg_refrige_type_01,    // R22
    msg_refrige_type_02,    // R407C
    msg_refrige_type_03,    // R410A
    msg_refrige_type_04,    // R32
    msg_refrige_type_05,    // R290
};

const U16 Msg_FourwayType[] =       /* 四通阀设置 */
{
    msg_fourway_type_01,            // 制热开阀
    msg_fourway_type_02,            // 制冷开阀
};


const U16 Msg_AiSensorType[] =      /* 模拟量传感器类型 */
{
    msg_AiSensorType_00,        // 无
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


const U16 data_format[] =   /* 通讯数据格式字符串 */
{
    msg_8N1,
    msg_8N2,
    msg_8O1,
    msg_8E1,
};
const U16 data_bps[] =      /* 波特率字符串 */
{
    msg_2400,
    msg_4800,
    msg_9600,
    msg_19200,
    msg_38400,
    msg_57600,
};

const U16 Msg_LimitType[] = /* 系统维护模式 */
{
    msg_LimitType_00,
    msg_LimitType_01,
};

const U16 Msg_RemoteType[] =        /* 线控开关类型 */
{
    msg_remote_type_01,         // 拨动
    msg_remote_type_02,         // 脉冲
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
    msg_power_mode_01,          /* 标准 */
    msg_power_mode_02,          /* 强劲 */
    msg_power_mode_03,          /* 节能 */
    msg_power_mode_04,          /* 自动 */
};

#if defined(M_TCL)
const U16 Msg_mac_model[] =         /* 机器型号 */
{
    msg_tcl_mac_model_01,// 未设置  
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
const U16 Msg_mac_model[] =         /* 机器型号 */
{
    msg_disen_mac_model_01,// 未设置 
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
const U16 Msg_mac_model[] =         /* 机器型号 */
{
    msg_tcl_mac_model_01,                   // 未设置
};
#endif

const U16 Msg_ClearFrostType[] =        /*  */
{
    msg_cleardrefrost_type_01,          // 禁用
    msg_cleardrefrost_type_02,          //
};
/* N0004 */
const U16 Msg_FreqFanSet[] =        /* 变频风机设置 */
{
    msg_freq_fan_set_01,
    msg_freq_fan_set_02,
    msg_freq_fan_set_03,
    msg_freq_fan_set_04,
    msg_freq_fan_set_05,
    msg_freq_fan_set_06,
};

const U16 Msg_FreqFanCtrl[] =       /* 变频风机调速 */
{
    msg_freq_fan_ctrl_01,   /* 手动 */
    msg_freq_fan_ctrl_02,   /* 环温线性 */
    msg_freq_fan_ctrl_03,   /* 翅温线性 */
};

const U16 Msg_remote_switch[] =      /* 联动功能设置 */
{
    msg_remote_switch_01,	/* 禁用  */
    msg_remote_switch_02,	/* 联动动作有效 */
    msg_remote_switch_03,	/* 联动闭合有效 */
    msg_remote_switch_04,	/* 线控开关机 */
    msg_remote_switch_05,	/* 线控水箱电热 */
    msg_remote_switch_06,	/* 线控外部热源 */
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

//const U16 Msg_gb_dbg_use[] =        /* 国标调试设置 */
//{
//    msg_gb_dbg_use_01,
//    msg_gb_dbg_use_02,
//    msg_gb_dbg_use_03,
//};

const U16 Msg_ability_test[] =      /* 能力测试选择 */
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
    msg_pfc_set1,// 开启pfc
    msg_pfc_set2,// 关闭pfc
    msg_pfc_set3,// 自动
};

const U16 Msg_df_stop_cm[] =        /* 除霜停机切阀 */
{
    msg_df_stop_cm_01,      /* 禁用     */
    msg_df_stop_cm_02,      /* 进退全停 */
    msg_df_stop_cm_03,      /* 进除霜停 */
    msg_df_stop_cm_04,      /* 退除霜停 */
};

const U16 Msg_SilentMode[] =
{
    dm603_para_silent_mode_01,  /* {一级静音}     */
    dm603_para_silent_mode_02,  /* {二级静音} */
};

const U16 Msg_ice_heat_mode[] =
{
    Msg_ice_heat_mode_01,   /* 01 {除霜底盘热} */
    Msg_ice_heat_mode_02,   /* 02 {低环底盘热} */
};

const U16 Msg_fcu_s_start_m[] =
{
    msg_s_start_m_01, /* 不开启 */
    msg_s_start_m_02, /* 仅开主机 */
    msg_s_start_m_03, /* 主末全开 */
};

const U16 Msg_fcu_mode_set[] =
{
    msg_fcu_mode_set_01,  /* 相互独立 */
    msg_fcu_mode_set_02,  /* 主机为主 */
    msg_fcu_mode_set_03,  /* 末端为主 */
    msg_fcu_mode_set_04,  /* 相互联动 */
};

const U16 Msg_fcu_cpst_set[] =
{
    msg_fcu_cpst_set_01,  /* 修正当前温度 */
    msg_fcu_cpst_set_02,  /* 修正目标温度 */
};

const U16 para_alarm_sound[] =
{
    msg_para_alarm_sound_set_01,    /* 01 {不响} */
    msg_para_alarm_sound_set_02,    /* 02 {长响} */
    msg_para_alarm_sound_set_03,    /* 03 {只响十秒} */
    msg_para_alarm_sound_set_04,    /* 04 {周期十秒} */
};

const U16 Msg_room_ther[] =
{
    msg_room_ther_Type_00,      // 禁用
    msg_room_ther_Type_01,      // 模式设定
    msg_room_ther_Type_02,  // 仅开关机
    msg_room_ther_Type_03,  // 双室温控
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
	msg_invt_err_auto_rst_01,	/* 01 部分 */
	msg_invt_err_auto_rst_02,	/* 02 全都 */
	msg_invt_err_auto_rst_03,	/* 03 全不 */
};

/* 字典信息,开始 */
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
    /* 从后面追加 */
};
#define PARA_DICT_NUM   ARRELE(para_dict)
U16 get_dict_num(void) {return PARA_DICT_NUM;}
/* 字典信息,结束 */

const SMALL para_small_01[] =
{
    {msg_para_small_01_01,para_val_01_01},/* 00 控制模式 */
    {msg_para_small_01_02,para_val_00_00},/* 01 制冷设定温度 */
    {msg_para_small_01_03,para_val_00_00},/* 02 制热设定温度 */
    {msg_para_small_01_04,para_val_00_00},/* 03 热水设定温度 */
    {msg_para_small_01_05,para_val_00_00},/* 26 制冷设定室温 */
    {msg_para_small_01_06,para_val_00_00},/* 04 制热设定室温 */
    {msg_para_small_01_07,para_val_00_00},/* 23 制冷设定温度(B) */
    {msg_para_small_01_08,para_val_00_00},/* 44 制冷设定室温(B) */
    {msg_para_small_01_09,para_val_00_00},/* 24 制热设定温度(B) */
    {msg_para_small_01_10,para_val_00_00},/* 25 制热设定室温(B) */
    {msg_para_small_01_11,Msg_power_mode},// 05 功率模式
    {msg_para_small_01_12,Msg_SilentMode},// 06 静音模式
    {msg_para_small_01_13,Msg_UseOrNot  },// 07 定时杀菌功能
    {msg_para_small_01_14,para_val_00_00},// 08 杀菌温度
    {msg_para_small_01_15,para_val_00_00},// 09 杀菌最大周期
    {msg_para_small_01_16,para_val_00_00},// 10 杀菌高温时间
    {msg_para_small_01_17,para_val_00_00},// 11 备用
    {msg_para_small_01_18,para_val_00_00},// 12 备用
    {msg_para_small_01_19,para_val_00_00},// 13 备用
    {msg_para_small_01_20,para_val_00_00},// 14 备用
    {msg_para_small_01_21,para_val_00_00},// 15 备用
    {msg_para_small_01_22,para_val_00_00},// 16 备用
    {msg_para_small_01_23,para_val_00_00},// 17 备用
    {msg_para_small_01_24,Msg_UseOrNot  },// 18 锁屏功能设置
    {msg_para_small_01_25,para_alarm_sound},// 19 报警音设置
    {msg_para_small_01_26,Msg_weather_curve}, /* 21 A区制冷曲线 */
    {msg_para_small_01_27,Msg_weather_curve}, /* 22 A区制热曲线 */
    {msg_para_small_01_28,Msg_weather_curve}, /* 27 B区制冷曲线 */
    {msg_para_small_01_29,Msg_weather_curve}, /* 28 B区制热曲线 */
    {msg_para_small_01_30,para_val_00_00},  /* 27 曲线9制冷环温1 */
    {msg_para_small_01_31,para_val_00_00},  /* 28 曲线9制冷环温2 */
    {msg_para_small_01_32,para_val_00_00},  /* 29 曲线9制冷出温1 */
    {msg_para_small_01_33,para_val_00_00},  /* 30 曲线9制冷出温2 */
    {msg_para_small_01_34,para_val_00_00},  /* 31 曲线9制热环温1 */
    {msg_para_small_01_35,para_val_00_00},  /* 32 曲线9制热环温2 */
    {msg_para_small_01_36,para_val_00_00},  /* 33 曲线9制热出温1 */
    {msg_para_small_01_37,para_val_00_00},  /* 34 曲线9制热出温2 */
    {msg_para_small_01_38,Msg_UseOrNot, },  /*37 00#模块设置 */
    {msg_para_small_01_39,Msg_UseOrNot, },  /*38 01#模块设置 */
    {msg_para_small_01_40,Msg_UseOrNot, },  /*39 02#模块设置 */
    {msg_para_small_01_41,Msg_UseOrNot, },  /*40 03#模块设置 */
    {msg_para_small_01_42,Msg_UseOrNot, },  /*41 04#模块设置 */
    {msg_para_small_01_43,Msg_UseOrNot, },  /*42 05#模块设置 */
    {msg_para_small_01_44,Msg_UseOrNot, },  /*43 06#模块设置 */
    {msg_para_small_01_45,Msg_UseOrNot, },  /*44 07#模块设置 */
};

const SMALL para_small_02[] =
{
    {msg_para_small_02_01,para_val_15_00},      // 00 机组类型
    {msg_para_small_02_02,Msg_RefrigeType},     // 01 冷媒设置 0=R22   1=R407C   2=R410
    {msg_para_small_02_03,Msg_FourwayType},     // 02 四通阀设置
    {msg_para_small_02_04,Msg_mac_model},       // 03 机器型号 para_fac
    {msg_para_small_02_05,Msg_RemoteType},      // 04 线控开关类型 0=拨动开关  1=脉冲开
    {msg_para_small_02_06,Msg_OpenType},        // 05 机组启停控制 请参见机组启停控制
    {msg_para_small_02_07,Msg_UseOrNot},        // 06 掉电记忆开关机   0=禁用  1=使用
    {msg_para_small_02_08,Msg_UseOrNot},        // 07 来电自启动   0=禁用  1=使用
    {msg_para_small_02_09,para_val_01_06},      // 08 水泵运行模式
    {msg_para_small_02_10,Msg_CtrlObject},      // 09 控制对象 0：回水  1：出水
    {msg_para_small_02_11,para_val_00_00},      // 10 制冷水温补偿
    {msg_para_small_02_12,para_val_00_00},      // 11 制热水温补偿
    {msg_para_small_02_13,para_val_00_00},      // 12 四通阀开延时
    {msg_para_small_02_14,para_val_00_00},      // 13 四通阀关延时
    {msg_para_small_02_15,para_val_00_00},      // 14 开喷液阀排温
    {msg_para_small_02_16,para_val_00_00},      // 15 关喷液阀排温差
    {msg_para_small_02_17,Msg_UseOrNot  },      // 16 热水功能
    {msg_para_small_02_18,para_val_00_00},      // 17 热水回差
    {msg_para_small_02_19,para_val_00_00},      // 18 开曲轴环温
    {msg_para_small_02_20,para_val_00_00},      // 19 关曲轴环温
    {msg_para_small_02_21,Msg_UseOrNot},        // 20 制冷模式
    {msg_para_small_02_22,Msg_UseOrNot},        // 21 制热模式
    {msg_para_small_02_23,para_val_00_00},      // 22 最小预热掉电(时)
    {msg_para_small_02_24,para_val_00_00},      // 23 最大预热掉电(时)
    {msg_para_small_02_25,para_val_00_00},      // 24 最小预热时间(分)
    {msg_para_small_02_26,para_val_00_00},      // 25 最大预热时间(分)
    {msg_para_small_02_27,Msg_UseOrNot},        // 26 热水模式优先
    {msg_para_small_02_28,Msg_UseOrNot},        // 27 水箱电热
    {msg_para_small_02_29,Msg_UseOrNot},        // 28 下回水泵
    {msg_para_small_02_30,Msg_UseOrNot},        // 29 太阳能
    {msg_para_small_02_31,Msg_remote_switch},   // 30 联动功能设置
    {msg_para_small_02_32,Msg_UseOrNot },       // 31 变频器使用设置   0=禁用  1=使用
    {msg_para_small_02_33,Msg_UseOrNot },       /* 32 系统变频水泵 */
    {msg_para_small_02_34,Msg_room_ther},       // 33 温控器控制
    {msg_para_small_02_35,Msg_zone_ctrl_type},  /* 34 线控器控制 */
    {msg_para_small_02_36,para_val_00_00},      // 35 负载修正幅度
    {msg_para_small_02_37,para_val_00_00},      // 36 负载修正时间
    {msg_para_small_02_38,Msg_ice_heat_mode},   /* 37 底盘电热模式 */
    {msg_para_small_02_39,para_val_00_00},      /* 38 底盘电热环温 */
    {msg_para_small_02_40,para_val_00_00},      /* 39 启动页类型 */
    {msg_para_small_02_41,Msg_sg_cfg},          /* 40 智能电网配置 */
    {msg_para_small_02_42,Msg_UseOrNot  },      /* 41 智能电网功能 */
    {msg_para_small_02_43,para_val_00_00},      /* 42 冷热调整系数 */
    {msg_para_small_02_44,para_val_00_00},      /* 43 水泵间歇停时长 */
};

const SMALL para_small_03[] =
{
    {msg_para_small_03_01,para_val_00_00},    // 0 变频压机编码
    {msg_para_small_03_02,Msg_UseOrNot},      // 1 手动模式
    {msg_para_small_03_03,para_val_00_00},    // 2 手动设定转速
    {msg_para_small_03_04,para_val_00_00},    // 3 防频繁启动
    {msg_para_small_03_05,para_val_00_00},    // 4 到温预降频
    {msg_para_small_03_06,para_val_00_00},    // 5 驱动转速下限
    {msg_para_small_03_07,para_val_00_00},    // 6 驱动转速上限
    {msg_para_small_03_08,para_val_00_00},    // 7 制冷额定转速
    {msg_para_small_03_09,para_val_00_00},    // 8 制热额定转速
    {msg_para_small_03_10,para_val_00_00},    // 9 启动维持时间
    {msg_para_small_03_11,para_val_00_00},      /* 36 高压限加偏差 */
    {msg_para_small_03_12,para_val_00_00},      /* 37 低压限加偏差 */
    {msg_para_small_03_13,para_val_00_00},      /* 38 高压强卸偏差 */
    {msg_para_small_03_14,para_val_00_00},      /* 39 低压强卸偏差 */
    {msg_para_small_03_15,para_val_00_00},    //14 低频运行最长时间
    {msg_para_small_03_16,para_val_00_00},    //15 强制回油时间
    {msg_para_small_03_17,para_val_00_00},    //16 变频驱动地址
    {msg_para_small_03_18,para_val_00_00},    //17 压机除霜转速
    {msg_para_small_03_19,para_val_00_00},    //18 压机启动转速
    {msg_para_small_03_20,para_val_00_00},    //19 启动升频速率
    {msg_para_small_03_21,para_val_00_00},    //20 运行升频速率
    {msg_para_small_03_22,para_val_00_00},    //21 除霜升频速率
    {msg_para_small_03_23,para_val_00_00},    //22 标准频率调整
    {msg_para_small_03_24,para_val_00_00},    //23 启动升频间隔
    {msg_para_small_03_25,para_val_00_00},    //24 调频比例带
    {msg_para_small_03_26,para_val_00_00},    //25 调频积分时间
    {msg_para_small_03_27,para_val_00_00},    //26 调频微分时间
    {msg_para_small_03_28,para_val_00_00},    //27 调频动作周期
    {msg_para_small_03_29,para_val_00_00},    //28 压机启动维持转速
    {msg_para_small_03_30,para_val_00_00},    //29 调频比例系数
    {msg_para_small_03_31,para_val_00_00},    //30 制冷冷凝上限
    {msg_para_small_03_32,para_val_00_00},    //31 制热冷凝上限
    {msg_para_small_03_33,para_val_00_00},    //32 节能频率调整
    {msg_para_small_03_34,para_val_00_00},    //33 制热蒸发下限
    {msg_para_small_03_35,para_val_00_00},    //34 额定维持时间
    {msg_para_small_03_36,para_val_00_00},    //35 制冷回油环温
    {msg_para_small_03_37,para_val_00_00},    //36 制热回油环温
    {msg_para_small_03_38,para_val_00_00},    //37 强制降频排温
    {msg_para_small_03_39,para_val_00_00},    //38 回油运行频率
    {msg_para_small_03_40,para_val_00_00},    //39 超静音最大转速
    {msg_para_small_03_41,para_val_00_00},    //40 强劲频率调整
    {msg_para_small_03_42,para_val_00_00},    //41 静音最大转速
    {msg_para_small_03_43,para_val_00_00},    //42 回避频率范围
    {msg_para_small_03_44,para_val_00_00},    //43 回避频率1
    {msg_para_small_03_45,para_val_00_00},    //44 回避频率2
    {msg_para_small_03_46,para_val_00_00},    //45 回避频率3
    {msg_para_small_03_47,para_val_00_00},    //46 回避频率4
    {msg_para_small_03_48,para_val_00_00},    //47 低环启动维持转速
};
const SMALL para_small_04[] =
{
    {msg_para_small_04_01,Msg_FreqFanSet},       // 变频风机设置
    {msg_para_small_04_02,para_val_00_00},       // EC风机编码    /* N0004 */
    {msg_para_small_04_03,para_val_00_00},       // 制冷风机最高
    {msg_para_small_04_04,para_val_00_00},       // 制冷风机最低
    {msg_para_small_04_05,para_val_00_00},       // 节能风速调整
    {msg_para_small_04_06,Msg_FreqFanCtrl},      // 变频风机调速
    {msg_para_small_04_07,para_val_00_00},       // 手动转速
    {msg_para_small_04_08,para_val_00_00},       // 高风模块温度
    {msg_para_small_04_09,para_val_00_00},       // 制冷高风环温
    {msg_para_small_04_10,para_val_00_00},       // 制冷低风环温
    {msg_para_small_04_11,para_val_00_00},       // 制热高风环温
    {msg_para_small_04_12,para_val_00_00},       // 制热低风环温
    {msg_para_small_04_13,para_val_00_00},       // 制冷高风冷凝
    {msg_para_small_04_14,para_val_00_00},       // 制冷低风冷凝
    {msg_para_small_04_15,para_val_00_00},       // 制热高风蒸发
    {msg_para_small_04_16,para_val_00_00},       // 制热低风蒸发
    {msg_para_small_04_17,para_val_00_00},       // 制热中风温度
    {msg_para_small_04_18,para_val_00_00},       // 风速调节周期
    {msg_para_small_04_19,para_val_00_00},       // 风速动作比例
    {msg_para_small_04_20,para_val_00_00},       // 制热风机最高
    {msg_para_small_04_21,para_val_00_00},       // 制热风机最低
    {msg_para_small_04_22,para_val_00_00},       // PWM风机脉冲
    {msg_para_small_04_23,para_val_00_00},       // 强劲风速调整
    {msg_para_small_04_24,para_val_00_00},       // EC风机基址
    {msg_para_small_04_25,para_val_00_00},       // 制冷中风温度
    {msg_para_small_04_26,para_val_00_00},       // 超静音最大风速
    {msg_para_small_04_27,para_val_00_00},       // 变频风机个数
    {msg_para_small_04_28,para_val_00_00},       // 静音最大风速
    {msg_para_small_04_29,para_val_00_00},       // pwm调速初始值
    {msg_para_small_04_30,para_val_00_00},       // pwm调速系数
    {msg_para_small_04_31,para_val_00_00},       // pwm调速间隔
    {msg_para_small_04_32,para_val_00_00},       // pwm调速死区
    {msg_para_small_04_33,para_val_00_00},       // 标准风速调整
    {msg_para_small_04_34,Msg_UseOrNot},       // 防冷风功能
    {msg_para_small_04_35,para_val_00_00},       // 送风机暂停
    {msg_para_small_04_36,para_val_00_00},       // 防冷风时间
    {msg_para_small_04_37,para_val_00_00},       // 频率补偿系数
    {msg_para_small_04_38,para_val_00_00},       // 修正系数下限
    {msg_para_small_04_39,para_val_00_00},      /* 39 排温风速修正1 */
    {msg_para_small_04_40,para_val_00_00},      /* 40 排温风速修正2 */
    {msg_para_small_04_41,para_val_00_00},      /* 41 排温风速修正3 */
    {msg_para_small_04_42,Msg_Tp_fan_corr_set}, /* 42 排温修正设置 */
    {msg_para_small_04_43,para_val_00_00},       // 制冷停风温差
    {msg_para_small_04_44,para_val_00_00},       // 制热停风温差
    {msg_para_small_04_45,para_val_00_00},       // 停风回差
    {msg_para_small_04_46,para_val_00_00},       // 制冷风机中速
    {msg_para_small_04_47,para_val_00_00},       // 制热风机中速
};
const SMALL para_small_05[] =
{
    {msg_para_small_05_01,Msg_MotorUseType},  // 1 主阀使用设置
    {msg_para_small_05_02,Msg_MotorExciType}, // 2 主阀励磁方式
    {msg_para_small_05_03,Msg_MotorExciFre},  // 3 主阀励磁频率
    {msg_para_small_05_04,para_val_00_00},    // 4 主阀总步数
    {msg_para_small_05_05,para_val_00_00},    // 5 主强动作比例
    {msg_para_small_05_06,para_val_00_00},    // 6 主阀最大开度
    {msg_para_small_05_07,para_val_00_00},    // 7 主阀过热排温补
    {msg_para_small_05_08,Msg_UseOrNot},      // 8 主阀手动模式
    {msg_para_small_05_09,para_val_00_00},    // 9 主阀手动步数
    {msg_para_small_05_10,para_val_00_00},    //10 主阀过热频率补
    {msg_para_small_05_11,Msg_S_MotorUseType},//11 辅阀使用设置
    {msg_para_small_05_12,Msg_MotorExciType}, //12 辅阀励磁方式
    {msg_para_small_05_13,Msg_MotorExciFre},  //13 辅阀励磁频率
    {msg_para_small_05_14,para_val_00_00},    //14 辅阀总步数
    {msg_para_small_05_15,para_val_00_00},    //15 辅强动作比例
    {msg_para_small_05_16,para_val_00_00},    //16 辅阀最大开度
    {msg_para_small_05_17,para_val_00_00},    //17 辅阀最小开度
    {msg_para_small_05_18,Msg_UseOrNot},      //18 辅阀手动模式
    {msg_para_small_05_19,para_val_00_00},    //19 辅阀手动步数
    {msg_para_small_05_20,para_val_00_00},    //20 低环高水回温
    {msg_para_small_05_21,para_val_00_00},    //21 低环高水环温
    {msg_para_small_05_22,para_val_00_00},    //22 低环高水主阀
    {msg_para_small_05_23,para_val_00_00},    //23 主阀参考开度
    {msg_para_small_05_24,para_val_00_00},    //24 参考开度系数
    {msg_para_small_05_25,para_val_00_00},    //25 制冷过热转换环
    {msg_para_small_05_26,para_val_00_00},    //26 低环制冷过热1
    {msg_para_small_05_27,para_val_00_00},    //27 低环制冷过热2
    {msg_para_small_05_28,para_val_00_00},    //28 低环制冷过热3
    {msg_para_small_05_29,para_val_00_00},    //29 主阀变频增量
    {msg_para_small_05_30,para_val_00_00},    //30 主阀升频系数
    {msg_para_small_05_31,para_val_00_00},    //31 主阀降频系数
    {msg_para_small_05_32,para_val_00_00},    //32 制冷初开趋势环
    {msg_para_small_05_33,para_val_00_00},    //33 制热吸气过热10
    {msg_para_small_05_34,Msg_MotorOrder},    //34 高排温主辅次序
};
const SMALL para_small_06[] =
{
    {msg_para_small_06_01,para_val_00_00},// 1 待机开度
    {msg_para_small_06_02,para_val_00_00},// 2 初开维持时间
    {msg_para_small_06_03,para_val_00_00},// 3 制冷初开放大
    {msg_para_small_06_04,para_val_00_00},// 4 制热初开放大1
    {msg_para_small_06_05,para_val_00_00},// 5 制热初开放大2
    {msg_para_small_06_06,para_val_00_00},// 6 制热初开放大3
    {msg_para_small_06_07,para_val_00_00},// 7 制热初开转换1
    {msg_para_small_06_08,para_val_00_00},// 8 制热初开转换2
    {msg_para_small_06_09,para_val_00_00},// 9 制冷初开下限
    {msg_para_small_06_10,para_val_00_00},//10 制冷初开上限
    {msg_para_small_06_11,para_val_00_00},//11 制热初开下限
    {msg_para_small_06_12,para_val_00_00},//12 制热初开上限
    {msg_para_small_06_13,para_val_00_00},//13 关辅初开放大
    {msg_para_small_06_14,para_val_00_00},//14 制冷吸气过热1
    {msg_para_small_06_15,para_val_00_00},//15 制冷吸气过热2
    {msg_para_small_06_16,para_val_00_00},//16 制冷吸气过热3
    {msg_para_small_06_17,para_val_00_00},//17 制冷过热转换1
    {msg_para_small_06_18,para_val_00_00},//18 制冷过热转换2
    {msg_para_small_06_19,para_val_00_00},//19 制热吸气过热1
    {msg_para_small_06_20,para_val_00_00},//20 制热吸气过热2
    {msg_para_small_06_21,para_val_00_00},//21 制热吸气过热3
    {msg_para_small_06_22,para_val_00_00},//22 制热吸气过热4
    {msg_para_small_06_23,para_val_00_00},//23 制热吸气过热5
    {msg_para_small_06_24,para_val_00_00},//24 制热吸气过热6
    {msg_para_small_06_25,para_val_00_00},//25 制热吸气过热7
    {msg_para_small_06_26,para_val_00_00},//26 制热吸气过热8
    {msg_para_small_06_27,para_val_00_00},//27 制热吸气过热9
    {msg_para_small_06_28,para_val_00_00},//28 强关小蒸发上限
    {msg_para_small_06_29,para_val_00_00},//29 除霜开度
    {msg_para_small_06_30,para_val_00_00},//30 主阀动作限幅
    {msg_para_small_06_31,para_val_00_00},//31 比例带
    {msg_para_small_06_32,para_val_00_00},//32 积分时间
    {msg_para_small_06_33,para_val_00_00},//33 微分时间
    {msg_para_small_06_34,para_val_00_00},//34 pid动作周期
    {msg_para_small_06_35,para_val_00_00},//35 pid计算周期
    {msg_para_small_06_36,para_val_00_00},//36 比例系数
    {msg_para_small_06_37,para_val_00_00},//37 制热最小开度1
    {msg_para_small_06_38,para_val_00_00},//38 制热最小开度2
    {msg_para_small_06_39,para_val_00_00},//39 制热最小开度3
    {msg_para_small_06_40,para_val_00_00},//40 制热最小开度4
    {msg_para_small_06_41,para_val_00_00},//41 制热最小开度5
    {msg_para_small_06_42,para_val_00_00},//42 制冷最小开度1
    {msg_para_small_06_43,para_val_00_00},//43 制冷最小开度2
    {msg_para_small_06_44,para_val_00_00},//44 制冷最小开度3
    {msg_para_small_06_45,para_val_00_00},//45 制冷最小开度4
    {msg_para_small_06_46,para_val_00_00},//46 制冷最小开度5
    {msg_para_small_06_47,para_val_00_00},//47 制热限关翅温
    {msg_para_small_06_48,para_val_00_00},//48 除霜初开补偿
};
const SMALL para_small_07[] =
{
    {msg_para_small_07_01,Msg_UseOrNot},  // 1 经济器制热开启
    {msg_para_small_07_02,Msg_UseOrNot},  // 2 经济器制冷开启
    {msg_para_small_07_03,para_val_00_00},// 3 辅阀开机延时
    {msg_para_small_07_04,para_val_00_00},// 4 制热开阀环温
    {msg_para_small_07_05,para_val_00_00},// 5 辅阀开启排温
    {msg_para_small_07_06,para_val_00_00},// 6 制冷开阀环温
    {msg_para_small_07_07,para_val_00_00},// 7 辅阀过热度偏差
    {msg_para_small_07_08,para_val_00_00},// 8 制冷初始开度
    {msg_para_small_07_09,para_val_00_00},// 9 制热初始开度 1
    {msg_para_small_07_10,para_val_00_00},//10 制热初始开度 2
    {msg_para_small_07_11,para_val_00_00},//11 制热初始开度 3
    {msg_para_small_07_12,para_val_00_00},//12 制热初始开度 4
    {msg_para_small_07_13,para_val_00_00},//13 制冷开启排过
    {msg_para_small_07_14,para_val_00_00},//14 制热开启排过
    {msg_para_small_07_15,para_val_00_00},//15 备用
    {msg_para_small_07_16,para_val_00_00},//16 比例带
    {msg_para_small_07_17,para_val_00_00},//17 积分时间
    {msg_para_small_07_18,para_val_00_00},//18 微分时间
    {msg_para_small_07_19,para_val_00_00},//19 pid动作周期
    {msg_para_small_07_20,para_val_00_00},//20 pid计算周期
    {msg_para_small_07_21,para_val_00_00},//21 比例系数
    {msg_para_small_07_22,para_val_00_00},//22 积分系数
    {msg_para_small_07_23,para_val_00_00},//23 过热度转换1
    {msg_para_small_07_24,para_val_00_00},//24 过热度转换2
    {msg_para_small_07_25,para_val_00_00},//25 过热度设定1
    {msg_para_small_07_26,para_val_00_00},//26 过热度设定2
    {msg_para_small_07_27,para_val_00_00},//27 最高目标排温
    {msg_para_small_07_28,para_val_00_00},//28 制冷排气过热
    {msg_para_small_07_29,para_val_00_00},//29 排过水温系数
    {msg_para_small_07_30,para_val_00_00},//30 制热排气过热1
    {msg_para_small_07_31,para_val_00_00},//31 制热排气过热2
    {msg_para_small_07_32,para_val_00_00},//32 制热排气过热3
    {msg_para_small_07_33,para_val_00_00},//33 制热排气过热4
    {msg_para_small_07_34,para_val_00_00},//34 制热排气过热5
    {msg_para_small_07_35,para_val_00_00},//35 辅阀开启转速
    {msg_para_small_07_36,para_val_00_00},//36 辅阀关闭转速
    {msg_para_small_07_37,para_val_00_00},//37 辅阀开排过偏差
    {msg_para_small_07_38,para_val_00_00},//38 辅阀开排温偏差
    {msg_para_small_07_39,Msg_UseOrNot},  //39 辅阀当喷液阀
    {msg_para_small_07_40,para_val_00_00},//40 制热辅阀控制A 
    {msg_para_small_07_41,para_val_00_00},//41 制热辅阀控制B
    {msg_para_small_07_42,para_val_00_00},//42 制热辅阀控制E
    {msg_para_small_07_43,para_val_00_00},//43 制热辅阀控制G
    {msg_para_small_07_44,para_val_00_00},//44 制热辅阀控制C
    {msg_para_small_07_45,para_val_00_00},//45 制热辅阀控制D
    {msg_para_small_07_46,para_val_00_00},//46 制冷辅阀控制
};
const SMALL para_small_08[] =
{
    {msg_para_small_08_01,para_val_00_00},    // 1 首次除霜间隔
    {msg_para_small_08_02,para_val_00_00},    // 2 除霜时间
    {msg_para_small_08_03,Msg_ClearFrostType},// 3 积霜计时选择
    {msg_para_small_08_04,para_val_00_00},    // 4 允许除霜环温
    {msg_para_small_08_05,para_val_00_00},    // 5 允许除霜翅温
    {msg_para_small_08_06,para_val_00_00},    // 6 退除霜翅温
    {msg_para_small_08_07,para_val_00_00},    // 7 退除霜出水温度
    {msg_para_small_08_08,Msg_UseOrNot},      // 8 环翅智能除霜
    {msg_para_small_08_09,para_val_00_00},    // 9 中度结霜温差
    {msg_para_small_08_10,para_val_00_00},    //10 重度结霜温差
    {msg_para_small_08_11,para_val_00_00},    //11 温度确认时间
    {msg_para_small_08_12,para_val_00_00},    //12 除霜环温1
    {msg_para_small_08_13,para_val_00_00},    //13 除霜环温2
    {msg_para_small_08_14,para_val_00_00},    //14 除霜环温3
    {msg_para_small_08_15,para_val_00_00},    //15 除霜环温4
    {msg_para_small_08_16,para_val_00_00},    //16 除霜间隔1
    {msg_para_small_08_17,para_val_00_00},    //17 除霜间隔2
    {msg_para_small_08_18,para_val_00_00},    //18 除霜间隔3
    {msg_para_small_08_19,para_val_00_00},    //19 除霜间隔4
    {msg_para_small_08_20,para_val_00_00},    //20 除霜间隔5
    {msg_para_small_08_21,para_val_00_00},    //21 除霜环翅差1
    {msg_para_small_08_22,para_val_00_00},    //22 除霜环翅差2
    {msg_para_small_08_23,para_val_00_00},    //23 除霜环翅差3
    {msg_para_small_08_24,para_val_00_00},    //24 除霜环翅差4
    {msg_para_small_08_25,para_val_00_00},    //25 除霜环翅差5
    {msg_para_small_08_26,Msg_UseOrNot},      //26 低压除霜设置
    {msg_para_small_08_27,para_val_00_00},    //27 低压除霜检测延时
    {msg_para_small_08_28,para_val_00_00},    //28 低压除霜间隔
    {msg_para_small_08_29,para_val_00_00},    //29 低压除霜翅温
    {msg_para_small_08_30,para_val_00_00},    //30 定时除霜间隔
    {msg_para_small_08_31,para_val_00_00},    //31 紧急除霜环翅
    {msg_para_small_08_32,Msg_df_stop_cm},    //32 除霜停机切阀
    {msg_para_small_08_33,para_val_00_00},    //33 进除霜压机延时
    {msg_para_small_08_34,para_val_00_00},    //34 进除霜四通延时
    {msg_para_small_08_35,para_val_00_00},    //35 退除霜压机延时
    {msg_para_small_08_36,para_val_00_00},    //36 退除霜四通延时
    {msg_para_small_08_37,para_val_00_00},    //37 除霜切阀频率
    {msg_para_small_08_38,para_val_00_00},    //38 停风滴水时间
    {msg_para_small_08_39,para_val_00_00},    //39 最小除霜间隔
    {msg_para_small_08_40,para_val_00_00},    //40 紧急除霜间隔
};
const SMALL para_small_09[] =
{
    {msg_para_small_09_01,para_val_00_00},   // 0 防冻间隔1
    {msg_para_small_09_02,para_val_00_00},   // 1 防冻间隔2
    {msg_para_small_09_03,para_val_00_00},   // 2 水泵防冻温度
    {msg_para_small_09_04,para_val_00_00},   // 3 电热防冻温度
    {msg_para_small_09_05,para_val_00_00},   // 4 压机防冻温度
    {msg_para_small_09_06,para_val_00_00},   // 5 电热退防冻温度
    {msg_para_small_09_07,para_val_00_00},   // 6 压机退防冻温度
    {msg_para_small_09_08,para_val_00_00},   // 7 防冻间隔环温
    {msg_para_small_09_09,para_val_00_00},   // 8 进防冻环温
    {msg_para_small_09_10,para_val_00_00},   // 9 退防冻环温差
    {msg_para_small_09_11,Msg_UseOrNot},     // 10 防冻功能设置
    {msg_para_small_09_12,Msg_UseOrNot},     // 11 太阳能水泵防冻 
    {msg_para_small_09_13,para_val_00_00},   // 12 定时防冻间隔 
    {msg_para_small_09_14,para_val_00_00},   // 13 定时防冻时长 
    {msg_para_small_09_15,para_val_00_00},   // 14 备用
    {msg_para_small_09_16,para_val_00_00},   // 15 备用
    {msg_para_small_09_17,para_val_00_00},   // 16 备用
    {msg_para_small_09_18,para_val_00_00},   // 17 备用
    {msg_para_small_09_19,para_val_00_00},   // 18 备用
    {msg_para_small_09_20,para_val_00_00},   // 19 备用
    {msg_para_small_09_21,para_val_00_00},   // 20 备用
    {msg_para_small_09_22,para_val_00_00},   // 21 备用
    {msg_para_small_09_23,para_val_00_00},   // 22 备用
    {msg_para_small_09_24,para_val_00_00},   // 23 备用
    {msg_para_small_09_25,para_val_00_00},   // 24 备用
    {msg_para_small_09_26,para_val_00_00},   // 25 备用
    {msg_para_small_09_27,para_val_00_00},   // 26 备用
    {msg_para_small_09_28,para_val_00_00},   // 27 备用
    {msg_para_small_09_29,para_val_00_00},   // 28 备用
    {msg_para_small_09_30,para_val_00_00},   // 29 备用
    {msg_para_small_09_31,para_val_00_00},   // 30 备用
    {msg_para_small_09_32,para_val_00_00},   // 31 制热转换环1
    {msg_para_small_09_33,para_val_00_00},   // 32 制热转换环2
    {msg_para_small_09_34,para_val_00_00},   // 33 制热转换环3
    {msg_para_small_09_35,para_val_00_00},   // 34 制热转换环4
    {msg_para_small_09_36,para_val_00_00},   // 35 制热转换环5

};
const SMALL para_small_10[] =
{
    {msg_para_small_10_01,para_val_00_00},  /* 1 一般故障延时 */
    {msg_para_small_10_02,para_val_00_00},  /* 2 水流不足检测延时 */
    {msg_para_small_10_03,para_val_00_00},  /* 3 故障复位时间 */
    {msg_para_small_10_04,para_val_00_00},  /* 4 自动复位允许时间 */
    {msg_para_small_10_05,para_val_00_00},  /* 5 低压故障检测延时 */
    {msg_para_small_10_06,para_val_00_00},  /* 6 退除霜低压检延 */
    {msg_para_small_10_07,para_val_00_00},  /* 7 制冷低压消抖延时 */
    {msg_para_small_10_08,para_val_00_00},  /* 8 制热低压消抖延时 */
    {msg_para_small_10_09,para_val_00_00},  /* 9 退出温保护温差 */
    {msg_para_small_10_10,para_val_00_00},  /* 10 制冷出温过低 */
    {msg_para_small_10_11,para_val_00_00},  /* 11 制热出温过高 */
    {msg_para_small_10_12,para_val_00_00},  /* 12 制冷禁开环温 */
    {msg_para_small_10_13,para_val_00_00},  /* 13 出回温差异常 */
    {msg_para_small_10_14,para_val_00_00},  /* 14 吸排异常检延 */
    {msg_para_small_10_15,Msg_UseOrNot},    /* 15 水流不足开泵 */
    {msg_para_small_10_16,para_val_00_00},  /* 16 强制降频比例 */
    {msg_para_small_10_17,para_val_00_00},  /* 17 强制降频周期 */
    {msg_para_small_10_18,para_val_00_00},  /* 18 出回温差过大 */
    {msg_para_small_10_19,para_val_00_00},  /* 19 制冷蒸发过低 */
    {msg_para_small_10_20,para_val_00_00},  /* 20 排气温度过高 */
    {msg_para_small_10_21,para_val_00_00},  /* 21 退排温过高温差 */
    {msg_para_small_10_22,para_val_00_00},  /* 22 制冷吸气过低 */
    {msg_para_small_10_23,para_val_00_00},  /* 23 PWM风速过低 */
    {msg_para_small_10_24,para_val_00_00},  /* 24 PWM风速检测延时 */
    {msg_para_small_10_25,Msg_UseOrNot},    /* 25 待机检测低压 */
    {msg_para_small_10_26,para_val_00_00},  /* 26 制热出温限制1  */
    {msg_para_small_10_27,para_val_00_00},  /* 27 制热出温限制2  */
    {msg_para_small_10_28,para_val_00_00},  /* 28 制热出温限制3  */
    {msg_para_small_10_29,para_val_00_00},  /* 29 制热出温限制4  */
    {msg_para_small_10_30,para_val_00_00},  /* 30 制热出温限制5 */
    {msg_para_small_10_31,para_val_00_00},  /* 31 地暖过热保护 */
    {msg_para_small_10_32,para_val_00_00},  /* 32 禁止低压环温 */
    {msg_para_small_10_33,para_val_00_00},  /* 33 制冷低压过低（范围以低压量程为准）  */
    {msg_para_small_10_34,para_val_00_00},  /* 34 制热低压过低（范围以低压量程为准）  */
    {msg_para_small_10_35,para_val_00_00},  /* 35 制冷高压过高（范围以高压量程为准）  */
    {msg_para_small_10_36,para_val_00_00},  /* 36 制热高压过高（范围以高压量程为准）  */
    {msg_para_small_10_37,para_val_00_00},  /* 37 低压报警复位压差 */
    {msg_para_small_10_38,para_val_00_00},  /* 38 高压报警复位压差 */
    {msg_para_small_10_39,para_val_00_00},  /* 39 低温制冷环温 */
    {msg_para_small_10_40,para_val_00_00},  /* 40 低温制冷检延 */
    {msg_para_small_10_41,Msg_UseOrNot},    /* 41 机型设置校验 */
    {msg_para_small_10_42,para_val_00_00},  /* 42 制冷出温限制 */
    {msg_para_small_10_43,Msg_UseOrNot},	/* 43 线控器通讯检测 */
    {msg_para_small_10_44,Msg_invt_err_auto_rst},//44 变频故障自复位
    {msg_para_small_10_45,para_val_00_00},  // 45 电热热水上限
    {msg_para_small_10_46,para_val_00_00},  // 46 高压辅调回差
    {msg_para_small_10_47,para_val_00_00},  // 47 低压辅调回差
};
const SMALL para_small_11[] =
{
    {msg_para_small_11_01,Msg_InputNormal},   //00 压机低压
    {msg_para_small_11_02,Msg_InputNormal},   //01 压机高压
    {msg_para_small_11_03,Msg_InputNormal},   //02 内置电热反馈
    {msg_para_small_11_04,Msg_InputNormal},   //03 水流开关
    {msg_para_small_11_05,Msg_InputNormal},   //04 水箱电热反馈
    {msg_para_small_11_06,Msg_UseOrNot},      //05 冷媒温度修正
    {msg_para_small_11_07,para_val_00_00},    //06 冷媒气侧修正1
    {msg_para_small_11_08,para_val_00_00},    //07 冷媒气侧修正2
    {msg_para_small_11_09,para_val_00_00},    //08 冷媒气侧修正3
    {msg_para_small_11_10,para_val_00_00},    //09 冷媒气侧修正4
    {msg_para_small_11_11,para_val_00_00},    //10 冷媒气侧修正5
    {msg_para_small_11_12,para_val_00_00},    //11 冷媒液侧修正1
    {msg_para_small_11_13,para_val_00_00},    //12 冷媒液侧修正2
    {msg_para_small_11_14,para_val_00_00},    //13 冷媒液侧修正3
    {msg_para_small_11_15,para_val_00_00},    //14 冷媒液侧修正4
    {msg_para_small_11_16,para_val_00_00},    //15 冷媒液侧修正5
    {msg_para_small_11_17,para_val_00_00},    //16 冷媒液侧修正6
    {msg_para_small_11_18,para_val_00_00},    //17 冷媒液侧修正7
    {msg_para_small_11_19,Msg_UseOrNot},      //18 水箱温度探头
    {msg_para_small_11_20,Msg_UseOrNot},      //19 地暖进水温度探头
    {msg_para_small_11_21,Msg_UseOrNot},      //20 系统总出水温度探头
    {msg_para_small_11_22,Msg_InputNormal},   //21 EUV信号
    {msg_para_small_11_23,Msg_InputNormal},   //22 SG信号
    {msg_para_small_11_24,Msg_sensor_cfg},  /* 35 温度探头配置 */
    {msg_para_small_11_25,Msg_UseOrNot},    /* 32 单元水流开关 */
    {msg_para_small_11_26,Msg_UseOrNot},    /* 33 太阳能温度探头 */
    {msg_para_small_11_27,Msg_AiSensorType},/* 00 J5传感器类型 */
    {msg_para_small_11_28,Msg_AiSensorType},/* 01 J6传感器类型 */
    {msg_para_small_11_29,para_val_00_00},  /* 02 J5最小量程  */
    {msg_para_small_11_30,para_val_00_00},  /* 03 J5最大量程  */
    {msg_para_small_11_31,para_val_00_00}, //04 J6最小量程
    {msg_para_small_11_32,para_val_00_00}, //05 J6最大量程
    {msg_para_small_11_33,Msg_UseOrNot},	/* 缓冲水箱上部温度探头 */
    {msg_para_small_11_34,Msg_UseOrNot},	/* 缓冲水箱下部温度探头 */
    {msg_para_small_11_35,Msg_press_cfg},	/* 压力传感器配置 */
    {msg_para_small_11_36,para_val_00_00},	/* 风机功率 */
    {msg_para_small_11_37,para_val_00_00},	/* 水泵功率 */
    {msg_para_small_11_38,para_val_00_00},	/* 电热1功率 */
    {msg_para_small_11_39,para_val_00_00},	/* 电热2功率 */
    {msg_para_small_11_40,para_val_00_00},	/* 待机功率 */
    {msg_para_small_11_41,para_val_00_00},	/* 其他运行功率 */
    {msg_para_small_11_42,Msg_UseOrNot},	/* 总出水温度探头 */
};
const SMALL para_small_12[] =
{
    {msg_para_small_12_01,Msg_ability_test},//01 {能力测试选择}
    {msg_para_small_12_02,para_val_00_00},  //02 {制热压机转速A}
    {msg_para_small_12_03,para_val_00_00},  //03 {制热风机转速A}
    {msg_para_small_12_04,para_val_00_00},  //04 {制热主阀控制A}
    {msg_para_small_12_05,para_val_00_00},  //05 {制热压机转速B}
    {msg_para_small_12_06,para_val_00_00},  //06 {制热风机转速B}
    {msg_para_small_12_07,para_val_00_00},  //07 {制热主阀控制B}
    {msg_para_small_12_08,para_val_00_00},  //08 {制热压机转速E}
    {msg_para_small_12_09,para_val_00_00},  //09 {制热风机转速E}
    {msg_para_small_12_10,para_val_00_00},  //10 {制热主阀控制E}
    {msg_para_small_12_11,para_val_00_00},  //11 {制热压机转速G}
    {msg_para_small_12_12,para_val_00_00},  //12 {制热风机转速G}
    {msg_para_small_12_13,para_val_00_00},  //13 {制热主阀控制G}
    {msg_para_small_12_14,para_val_00_00},  //14 {制热压机转速C}
    {msg_para_small_12_15,para_val_00_00},  //15 {制热风机转速C}
    {msg_para_small_12_16,para_val_00_00},  //16 {制热主阀控制C}
    {msg_para_small_12_17,para_val_00_00},  //17 {制热压机转速D}
    {msg_para_small_12_18,para_val_00_00},  //18 {制热风机转速D}
    {msg_para_small_12_19,para_val_00_00},  //19 {制热主阀控制D}
    {msg_para_small_12_20,para_val_00_00},  //20 {制冷压机转速}
    {msg_para_small_12_21,para_val_00_00},  //21 {制冷风机转速}
    {msg_para_small_12_22,para_val_00_00},  //22 {制冷主阀控制}
    {msg_para_small_12_23,para_val_00_00},  //23 {制热除霜环翅A}
    {msg_para_small_12_24,para_val_00_00},  //24 {制热除霜间隔A}
    {msg_para_small_12_25,para_val_00_00},  //25 {制热除霜开度A}
    {msg_para_small_12_26,para_val_00_00},  //26 {制热除霜转速A}
    {msg_para_small_12_27,para_val_00_00},  //27 {制热退除霜翅温A}
    {msg_para_small_12_28,para_val_00_00},  //28 {制热除霜时间A}
    {msg_para_small_12_29,para_val_00_00},  //29 {制热除霜环翅B}
    {msg_para_small_12_30,para_val_00_00},  //30 {制热除霜间隔B}
    {msg_para_small_12_31,para_val_00_00},  //31 {制热除霜开度B}
    {msg_para_small_12_32,para_val_00_00},  //32 {制热除霜转速B}
    {msg_para_small_12_33,para_val_00_00},  //33 {制热退除霜翅温B}
    {msg_para_small_12_34,para_val_00_00},  //34 {制热除霜时间B}
    {msg_para_small_12_35,para_val_00_00},  //35 {制热除霜环翅E}
    {msg_para_small_12_36,para_val_00_00},  //36 {制热除霜间隔E}
    {msg_para_small_12_37,para_val_00_00},  //37 {制热除霜开度E}
    {msg_para_small_12_38,para_val_00_00},  //38 {制热除霜转速E}
    {msg_para_small_12_39,para_val_00_00},  //39 {制热退除霜翅温E}
    {msg_para_small_12_40,para_val_00_00},  //40 {制热除霜时间E}
    {msg_para_small_12_41,para_val_00_00},  //41 {制热除霜环翅G}
    {msg_para_small_12_42,para_val_00_00},  //42 {制热除霜间隔G}
    {msg_para_small_12_43,para_val_00_00},  //43 {制热除霜开度G}
    {msg_para_small_12_44,para_val_00_00},  //44 {制热除霜转速G}
    {msg_para_small_12_45,para_val_00_00},  //45 {制热退除霜翅温G}
    {msg_para_small_12_46,para_val_00_00},  //46 {制热除霜时间G}
    {msg_para_small_12_47,Msg_pfc_set},     //47 {PFC设置}
    {msg_para_small_12_48,para_val_00_00},  //48 {手动变频水泵}
};

const SMALL para_small_13[] =
{
    {msg_para_small_13_01,para_val_00_00},   //01 能量控制周期
    {msg_para_small_13_02,para_val_00_00},   /*02 制冷加载偏差  */
    {msg_para_small_13_03,para_val_00_00},   /*03 制冷卸载偏差  */
    {msg_para_small_13_04,para_val_00_00},   /*04 制热加载偏差  */
    {msg_para_small_13_05,para_val_00_00},   /*05 制热卸载偏差  */
    {msg_para_small_13_06,para_val_00_00},   /*06 室温回差  */
    {msg_para_small_13_07,para_val_00_00},   //07 制冷回温下限
    {msg_para_small_13_08,para_val_00_00},   //08 制冷回温上限
    {msg_para_small_13_09,para_val_00_00},   //09 制热回温下限
    {msg_para_small_13_10,para_val_00_00},   //10 制热回温上限
    {msg_para_small_13_11,para_val_00_00},   //11 制冷最低环温
    {msg_para_small_13_12,para_val_00_00},   //12 制冷最高环温
    {msg_para_small_13_13,para_val_00_00},   //13 制热最低环温
    {msg_para_small_13_14,para_val_00_00},   //14 制热最高环温
    {msg_para_small_13_15,para_val_00_00},   //15 热水最低环温
    {msg_para_small_13_16,para_val_00_00},   //16 热水最高环温
    {msg_para_small_13_17,para_val_00_00},   //17 自动制冷最低环
    {msg_para_small_13_18,para_val_00_00},   //18 自动制热最高环
    {msg_para_small_13_19,para_val_00_00},   /*19 假日离家制热 */
    {msg_para_small_13_20,para_val_00_00},   /*20 假日离家热水 */
    {msg_para_small_13_21,para_val_00_00},   /*21 节能运行时间 */
    {msg_para_small_13_22,Msg_ibh_ahs_use},  //22 辅助电热
    {msg_para_small_13_23,Msg_ibh_ahs_use},  //23 外部热源
    {msg_para_small_13_24,para_val_00_00},   /*24 辅热开启高温差 */
    {msg_para_small_13_25,para_val_00_00},   /*25 辅热开启低温差 */
    {msg_para_small_13_26,para_val_00_00},   //26 辅助电热调档
    {msg_para_small_13_27,para_val_00_00},   //27 辅助电热开启延
    {msg_para_small_13_28,para_val_00_00},   //28 辅助电热开环温
    {msg_para_small_13_29,para_val_00_00},   //29 辅助电热开温差
    {msg_para_small_13_30,para_val_00_00},   //30 水箱电热开启延
    {msg_para_small_13_31,para_val_00_00},   //31 水箱电热开环温
    {msg_para_small_13_32,para_val_00_00},   //32 外部热源开启延
    {msg_para_small_13_33,para_val_00_00},   //33 外部热源开温差
    {msg_para_small_13_34,para_val_00_00},   //34 外部热源关温差
    {msg_para_small_13_35,para_val_00_00},   //35 外部热源开环温
    {msg_para_small_13_36,para_val_00_00},   /*36 优先制热水时长 */
    {msg_para_small_13_37,para_val_00_00},   /*37 优先制热水间隔 */
    {msg_para_small_13_38,Msg_fcu_cpst_set}, //38 能调修正对象
    {msg_para_small_13_39,para_val_00_00},   //39 压机全开温差
    {msg_para_small_13_40,para_val_00_00},   //40 商检制热步数
    {msg_para_small_13_41,para_val_00_00},   //41 商检制热频率
    {msg_para_small_13_42,para_val_00_00},   //42 商检制热时间
    {msg_para_small_13_43,para_val_00_00},   //43 商检制冷步数
    {msg_para_small_13_44,para_val_00_00},   //44 商检制冷频率

    {msg_para_small_13_45,para_val_00_00},   //45 商检制冷时间
    {msg_para_small_13_46,para_val_00_00},   //46 商检电热时间
    {msg_para_small_13_47,para_val_00_00},   //47 商检制热风速
    {msg_para_small_13_48,para_val_00_00},   //48 商检制冷风速
};

const SMALL para_small_14[] =
{
    {msg_para_small_14_01,Msg_PUMPf_ctrl_type},             /* 01 变频水泵控制 */
    {msg_para_small_14_02,para_val_00_00},                  /* 02 水泵额定转速 */
    {msg_para_small_14_03,para_val_00_00},                  /* 03 水泵目标转速 */
    {msg_para_small_14_04,para_val_00_00},                  /* 04 变频水泵厂家 */
    {msg_para_small_14_05,para_val_00_00},                  /* 05 水泵目标流量 */
    {msg_para_small_14_06,para_val_00_00},                  /* 06 下回水泵运行 */
    {msg_para_small_14_07,Msg_UseOrNot},                    /* 07 下回水泵杀菌 */
    {msg_para_small_14_08,Msg_UseOrNot},                    /* 08 下回水泵定时 */
    {msg_para_small_14_09,para_val_00_00},                  /* 09 地暖预热温度 */
    {msg_para_small_14_10,para_val_00_00},                  /* 10 地暖预热间隔 */
    {msg_para_small_14_11,para_val_00_00},                  /* 11 地暖预热时长 */
    {msg_para_small_14_12,para_val_00_00},                  /* 12 地暖水温回差 */
    {msg_para_small_14_13,para_val_00_00},                  /* 13 地暖室温回差 */
    {msg_para_small_14_14,para_val_00_00},                  /* 14 地暖烘干前 */
    {msg_para_small_14_15,para_val_00_00},                  /* 15 地暖烘干中 */
    {msg_para_small_14_16,para_val_00_00},                  /* 16 地暖烘干后 */
    {msg_para_small_14_17,para_val_00_00},                  /* 17 地暖烘干温度 */
    {msg_para_small_14_18,Msg_termial_device_type},         /* 18 A区制冷末端 */
    {msg_para_small_14_19,Msg_termial_device_type},         /* 19 A区制热末端 */
    {msg_para_small_14_20,Msg_termial_device_type},         /* 20 B区制冷末端 */
    {msg_para_small_14_21,Msg_termial_device_type},         /* 21 B区制热末端 */
    {msg_para_small_14_22,para_val_00_00},                  /* 22 末端个数 */
    {msg_para_small_14_23,para_val_00_00},                  /* 23 末端起始地址 */
    {msg_para_small_14_24,Msg_UseOrNot},                    /* 24 主机开启末端 */
    {msg_para_small_14_25,Msg_UseOrNot},                    /* 25 主机关闭末端 */
    {msg_para_small_14_26,Msg_fcu_s_start_m},               /* 26 末端开启主机 */
    {msg_para_small_14_27,Msg_fcu_mode_set},                /* 27 主末模式设置 */
    {msg_para_small_14_28,para_val_00_00},                  /* 28 末端能调修正 */
    {msg_para_small_14_29,para_val_00_00},                  /* 29 太阳能热水水温 */
    {msg_para_small_14_30,para_val_00_00},                  /* 30 太阳能热水回差 */
    {msg_para_small_14_31,para_val_00_00},                  /* 31 太阳能开启水温 */
    {msg_para_small_14_32,para_val_00_00},                  /* 32 太阳能开启回差 */
    {msg_para_small_14_33,Msg_UseOrNot  },                  /* 33 收氟功能设置 */
    {msg_para_small_14_34,para_val_00_00},                  /* 34 收氟主阀步数 */
    {msg_para_small_14_35,para_val_00_00},                  /* 35 收氟低压延退 */
    {msg_para_small_14_36,para_val_00_00},                  /* 36 S3V3动作周期 */
    {msg_para_small_14_37,para_val_00_00},                  /* 37 外循环最小时间 */
    {msg_para_small_14_38,para_val_00_00},                  /* 38 备用 */
    {msg_para_small_14_39,para_val_00_00},                  /* 39 备用 */
    {msg_para_small_14_40,para_val_00_00},                  /* 40 模块个数 */
    {msg_para_small_14_41,para_val_00_00},                  /* 41 热水模块数 */
    {msg_para_small_14_42,para_val_00_00},                  /* 42 备用 */
    {msg_para_small_14_43,para_val_00_00},                  /* 43 备用 */
    {msg_para_small_14_44,para_val_00_00},                  /* 44 备用 */
    {msg_para_small_14_45,para_val_00_00},                  /* 45 备用 */
    {msg_para_small_14_46,data_format},                     /* 46 BMS数据格式 */
    {msg_para_small_14_47,data_bps},                        /* 47 BMS波特率   */
    {msg_para_small_14_48,para_val_00_00},                  /* 48 BMS通讯地址 */
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
/* 参数小项 */
const SMALL *para_small[GROUP_MAX] =
{
    para_small_01,  // 01 用户参数
    para_small_02,  // 02 系统参数1
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
/* 参数大项 */      // 此处停用，去修改MenuItem_ParaGroup
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



const SMALL limit_set_msg[] =   // 系统维护设置字符串
{
    {   msg_limit_set_01,   para_val_00_00, },  /* 01 {系统维护设置    } */
    {   msg_limit_set_02,   Msg_LimitType,  },  /* 02 {系统维护模式    } */
};

/* 故障信息 */
/* 系统板故障 */
const U16 msg_m_error[] =
{
    msg_m_error_00, // {}
    msg_m_error_01, // {错相保护}
    msg_m_error_02, // {EEPROM数据错故障}
    msg_m_error_03, // {系统环温故障}
    msg_m_error_04, // {系统回温故障}
    msg_m_error_05, // {系统出温故障}
    msg_m_error_06, // {系统维护数据错}
    msg_m_error_07, // {缺相保护}
    msg_m_error_08, // {送风压差}
    msg_m_error_09, // {水流不足}
    msg_m_error_10, // {内置电热过载}
    msg_m_error_11, // {水箱电热过载}
    msg_m_error_12, // {水箱温度故障}
    msg_m_error_13, // {总出水温度故障}
    msg_m_error_14, // {系统总出温故障}
    msg_m_error_15,  // {地暖总进温故障}
    msg_m_error_16,  // {风盘01通讯故障}
    msg_m_error_17,  // {风盘02通讯故障}
    msg_m_error_18, // {风盘03通讯故障}
    msg_m_error_19, // {风盘04通讯故障}
    msg_m_error_20, // {风盘05通讯故障}
    msg_m_error_21, // {风盘06通讯故障}
    msg_m_error_22, // {风盘07通讯故障}
    msg_m_error_23, // {风盘08通讯故障}
    msg_m_error_24, // {风盘09通讯故障}
    msg_m_error_25, // {风盘10通讯故障}
    msg_m_error_26, // {风盘11通讯故障}
    msg_m_error_27, // {风盘12通讯故障}
    msg_m_error_28, // {风盘13通讯故障}
    msg_m_error_29, // {风盘14通讯故障}
    msg_m_error_30, // {风盘15通讯故障}
    msg_m_error_31, // {风盘16通讯故障}
    msg_m_error_32, // {风盘01故障}
    msg_m_error_33, // {风盘02故障}
    msg_m_error_34, // {风盘03故障}
    msg_m_error_35, // {风盘04故障}
    msg_m_error_36, // {风盘05故障}
    msg_m_error_37, // {风盘06故障}
    msg_m_error_38, // {风盘07故障}
    msg_m_error_39, // {风盘08故障}
    msg_m_error_40, // {风盘09故障}
    msg_m_error_41, // {风盘10故障}
    msg_m_error_42, // {风盘11故障}
    msg_m_error_43, // {风盘12故障}
    msg_m_error_44, // {风盘13故障}
    msg_m_error_45, // {风盘14故障}
    msg_m_error_46, // {风盘15故障}
    msg_m_error_47, // {风盘16故障}
    msg_m_error_48, // {风盘17通讯故障}
    msg_m_error_49, // {风盘18通讯故障}
    msg_m_error_50, // {风盘19通讯故障}
    msg_m_error_51, // {风盘20通讯故障}
    msg_m_error_52, // {风盘21通讯故障}
    msg_m_error_53, // {风盘22通讯故障}
    msg_m_error_54, // {风盘23通讯故障}
    msg_m_error_55, // {风盘24通讯故障}
    msg_m_error_56, // {风盘25通讯故障}
    msg_m_error_57, // {风盘26通讯故障}
    msg_m_error_58, // {风盘27通讯故障}
    msg_m_error_59, // {风盘28通讯故障}
    msg_m_error_60, // {风盘29通讯故障}
    msg_m_error_61, // {风盘30通讯故障}
    msg_m_error_62, // {风盘31通讯故障}
    msg_m_error_63, // {风盘32通讯故障}
    msg_m_error_64, // {风盘17故障}
    msg_m_error_65, // {风盘18故障}
    msg_m_error_66, // {风盘19故障}
    msg_m_error_67, // {风盘20故障}
    msg_m_error_68, // {风盘21故障}
    msg_m_error_69, // {风盘22故障}
    msg_m_error_70, // {风盘23故障}
    msg_m_error_71, // {风盘24故障}
    msg_m_error_72, // {风盘25故障}
    msg_m_error_73, // {风盘26故障}
    msg_m_error_74, // {风盘27故障}
    msg_m_error_75, // {风盘28故障}
    msg_m_error_76, // {风盘29故障}
    msg_m_error_77, // {风盘30故障}
    msg_m_error_78, // {风盘31故障}
    msg_m_error_79, // {风盘32故障}
    msg_m_error_80, // {缓冲水箱上部温度故障}
    msg_m_error_81, // {缓冲水箱下部温度故障}
    msg_m_error_82, // {太阳能温度故障}
    msg_m_error_83, // {线控器通讯故障}
    msg_m_error_84, // {空调总出水温度故障}
    msg_m_error_85, // {室内温度故障}
    msg_m_error_86, // {备用}
};


/* 变频器故障 */
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

/* 变频器故障代号 */
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


/* 模块板故障 */
const U16 msg_s_error[] =
{
    msg_s_error_00, // {压机低压}
    msg_s_error_01, // {压机高压}
    msg_s_error_02, // {压机电流过大}
    msg_s_error_03, // {压机电流过小}
    msg_s_error_04, // {翅片探头故障}
    msg_s_error_05, // {排气探头故障}
    msg_s_error_06, // {排气温度过高}
    msg_s_error_07, // {低压压传故障}
    msg_s_error_08, // {高压压传故障}
    msg_s_error_09, // {压传低压过低}
    msg_s_error_10, // {压传高压过高}
    msg_s_error_11, // {吸气温度故障}
    msg_s_error_12, // {阀后温度故障}
    msg_s_error_13, // {吸气温度过低}
    msg_s_error_14, // {紧急除霜频繁}
    msg_s_error_15, // {吸排温差异常}
    msg_s_error_16, // {制冷蒸发过低}
    msg_s_error_17, // {降频开关}
    msg_s_error_18, // {备用}
    msg_s_error_19, // {回温温度过低}
    msg_s_error_20, // {回温温度过高}
    msg_s_error_21, // {风机1转速异常}
    msg_s_error_22, // {风机2转速异常}
    msg_s_error_23, // {EC1通讯故障}
    msg_s_error_24, // {EC1故障}
    msg_s_error_25, // {EC2通讯故障}
    msg_s_error_26, // {EC2故障}
    msg_s_error_27, // {变频通讯故障}
    msg_s_error_28, // {变频故障}
    msg_s_error_29, // {变频型号设置中}
    msg_s_error_30, // 备用
    msg_s_error_31, // 备用

    msg_s_error_32, // 冷媒气侧温度故障
    msg_s_error_33, // 冷媒液侧温度故障
    msg_s_error_34, // 蒸发温度过高
    msg_s_error_35, // 备用
    msg_s_error_36, // 备用
    msg_s_error_37, // 备用
    msg_s_error_38, // 备用
    msg_s_error_39, // 备用
    msg_s_error_40, // 备用
    msg_s_error_41, // 备用
    msg_s_error_42, // 备用
    msg_s_error_43, // 备用
    msg_s_error_44, // 备用
    msg_s_error_45, // 备用
    msg_s_error_46, // 备用
    msg_s_error_47, // 备用
    msg_s_error_48, // 备用
    msg_s_error_49, // 备用
    msg_s_error_50, // 备用
    msg_s_error_51, // 备用
    msg_s_error_52, // 备用
    msg_s_error_53, // 备用
    msg_s_error_54, // 备用
    msg_s_error_55, // 备用
    msg_s_error_56, // 备用
    msg_s_error_57, // 备用
    msg_s_error_58, // 备用
    msg_s_error_59, // 备用
    msg_s_error_60, // 备用
    msg_s_error_61, // 备用
    msg_s_error_62, // 备用
    msg_s_error_63, // 备用

    msg_cell_error_00,  // {模块环温故障}
    msg_cell_error_01,  // {通讯故障}
    msg_cell_error_02,  // {EEPROM 数据错}
    msg_cell_error_03,  // {缺相保护}
    msg_cell_error_04,  // {逆相保护}
    msg_cell_error_05,  // {模块出温过低}
    msg_cell_error_06,  // {模块出温过高}
    msg_cell_error_07,  // {模块板换出温故障}
    msg_cell_error_08,  // {水流不足}
    msg_cell_error_09,  // {风机过载}
    msg_cell_error_10,  // {模块板换回温故障}
    msg_cell_error_11,  // {防冰温度过低}
    msg_cell_error_12,  // {防冰探头故障}
    msg_cell_error_13,  // {出回温差过大}
    msg_cell_error_14,  // {出回温差异常}
#if defined(M_TCL)
    msg_cell_error_tcl_15,// {变频水泵警告[80%]}
#else
    msg_cell_error_15,  // {变频水泵警告}
#endif
    msg_cell_error_16,  // {通讯异常}
    msg_cell_error_17,   // {外机通讯故障}
    msg_cell_error_18,   // {协议版本过低}
    msg_cell_error_19,      /* 变频水泵故障 */
    msg_cell_error_20,      /* 机型设置异常 */
};

/* 变频水泵子故障 */
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
///* 状态查询变量及字符串 */
//static U08 cell_o_tbl[MAX_RELAY * RLY_BIT_NUM];   /* 模块输出状态值 */
//static U08 cell_p_tbl[MAX_PV];            /* 模块模拟状态值 */
//static U08 sys_cell_o_tbl[MAX_RELAY * RLY_BIT_NUM];   //系统输出状态
//static U08 sys_cell_p_tbl[MAX_PV];                    //

/* 模块压机输出 */
const U16 msg_uint_o_comp[] = // 对应
{
    msg_comp_out_01,    // 压机
    msg_comp_out_02,    // 四通
    msg_comp_out_03,    // 喷液阀
    msg_comp_out_04,    // 风机1
    msg_comp_out_05,    // 风机2
    msg_comp_out_06,    // 曲轴1
    msg_comp_out_07,    // 增焓阀
    msg_comp_out_08,    // 曲轴2
};

/* 模块单元输出 */
const U16 msg_uint_o_cell[]=
{
    msg_uint_out_01,		/* 变频水泵 */
    msg_uint_out_02,		/* 辅助电热1 */
    msg_uint_out_03,		/* 辅助电热2 */
    msg_uint_out_04,		/* 底盘电加热 */
    msg_uint_out_05,		/* 防冻结电加热带 */
    msg_uint_out_06,		/* 底盘电加热2 */
};

/* 系统输出 */
const U16 msg_sys_o[] =
{
    msg_sys_out_01,			/* 除霜指示 */
    msg_sys_out_02,			/* 故障指示 */
    msg_sys_out_03,			/* 外部热源 */
    msg_sys_out_04,			/* 电磁三通阀1 */
    msg_sys_out_05,			/* 电磁三通阀3 */
    msg_sys_out_06,			/* 电磁三通阀2 */
    msg_sys_out_07,			/* 水箱电热 */
    msg_sys_out_08,			/* 备用 */
    msg_sys_out_09,			/* 下回水泵 */
    msg_sys_out_10,			/* 太阳能水泵 */
    msg_sys_out_11,			/* 混合水泵 */
    msg_sys_out_12,			/* 外循环泵 */
    msg_sys_out_13,			/* 空调泵 */
    msg_sys_out_14,			/* 备用 */
};

/* 系统模拟量 */
const MSG_ITEM_INFO msg_sys_p[] =
{
    { UINT_C|D_DOT1,   msg_sys_pv_01 },		/* 室内温度 */
    { UINT_C|D_DOT1,   msg_sys_pv_02 },		/* 备用 */
    { UINT_C|D_DOT1,   msg_sys_pv_03 },		/* 环境温度' */
    { UINT_C|D_DOT1,   msg_sys_pv_04 },		/* 热水水箱温度 */
    { UINT_C|D_DOT1,   msg_sys_pv_05 },		/* 总出水温度' */
    { UINT_C|D_DOT1,   msg_sys_pv_06 },		/* 系统总出水温度 */
    { UINT_C|D_DOT1,   msg_sys_pv_07 },		/* 太阳能温度 */
    { UINT_C|D_DOT1,   msg_sys_pv_08 },		/* 缓冲水箱上部温度 */
    { UINT_C|D_DOT1,   msg_sys_pv_09 },		/* 缓冲水箱下部温度 */
    { UINT_C|D_DOT1,   msg_sys_pv_10 },		/* 空调总出水温度 */
    { UINT_C|D_DOT1,   msg_sys_pv_11 },		/* 备用 */
    { UINT_C|D_DOT1,   msg_sys_pv_12 },		/* 备用 */
    { UINT_C|D_DOT1,   msg_sys_pv_13 },		/* 备用 */
    { UINT_C|D_DOT1,   msg_sys_pv_14 },		/* 备用 */
    { UINT_C|D_DOT1,   msg_sys_pv_15 },		/* 地暖进水温度 */
    { 0xFFFF,          msg_sys_pv_16 },		/* 电磁三通阀1输出 */
    { 0xFFFF,          msg_sys_pv_17 },		/* 电磁三通阀2输出 */
    { 0xFFFF,          msg_sys_pv_18 },		/* 电磁三通阀3输出 */
    { UINT_PE|D_DOT1,  msg_sys_pv_19 },		/* 备用 */
    { UINT_0|D_DOT2,   msg_sys_pv_20 },		/* 制冷功率 */
    { UINT_0|D_DOT2,   msg_sys_pv_21 },		/* 制热功率 */
    { UINT_0|D_DOT2,   msg_sys_pv_22 },		/* 制热水功率 */
};
U16 get_sys_p_num(void)
{
    return ARRELE(msg_sys_p);
}

/* 模块压机模拟量 */
const MSG_ITEM_INFO msg_uint_p_comp[] = // 对应cell[pcb_addr].cell_pv[x]的顺序及定义
{
    { UINT_A|D_DOT1,        msg_comp_pv_01 },				/* {电流}            */
    { UINT_ST,              msg_comp_pv_02 },				/* {主电膨开度}      */
    { UINT_ST,              msg_comp_pv_03 },				/* {辅电膨开度}      */
    { UINT_C|D_DOT1,        msg_comp_pv_04 },				/* {翅片温度}        */
    { UINT_C|D_DOT1,        msg_comp_pv_05 },				/* {排气温度}        */
    { UINT_C|D_DOT1,        msg_comp_pv_06 },				/* {吸气温度}        */
    { UINT_C|D_DOT1,        msg_comp_pv_07 },				/* {阀后温度}        */
    { UINT_C|D_DOT1,        msg_comp_pv_08 },				/* {压顶温度}        */
    { UINT_C|D_DOT1,        msg_comp_pv_09 },				/* {蒸发饱和温度}    */
    { UINT_C|D_DOT1,        msg_comp_pv_10 },				/* {冷凝饱和温度}    */
    { UINT_PR|D_DOT1,       msg_comp_pv_11 },				/* {低压压力}        */
    { UINT_PR|D_DOT1,   	msg_comp_pv_12 },				/* {高压压力}        */
    { UINT_C|D_DE|D_DOT1,	msg_comp_pv_13 },				/* {主阀目标过热}    */
    { UINT_C|D_DE|D_DOT1,   msg_comp_pv_14 },				/* {主阀当前过热}    */
    { UINT_C|D_DE|D_DOT1,   msg_comp_pv_15 },				/* {辅阀目标过热}    */
    { UINT_C|D_DE|D_DOT1,   msg_comp_pv_16 },				/* {辅阀当前过热}    */
    { UINT_C|D_DOT1,    	msg_comp_pv_17 },				/* {经济器入口温度}  */
    { UINT_C|D_DOT1,    	msg_comp_pv_18 },				/* {经济器出口温度}  */
    { UINT_C|D_DE|D_DOT1,   msg_comp_pv_19 },				/* {无霜运行环翅差}  */
    { UINT_RS|D_DOT1,       msg_comp_pv_20 },				/* {变频目标转速}    */
    { UINT_RS|D_DOT1,       msg_comp_pv_21 },				/* {变频当前转速}    */
    { UINT_V|D_DOT1,        msg_comp_pv_22 },				/* {变频输出电压}    */
    { UINT_A|D_DOT1,        msg_comp_pv_23 },				/* {变频输出电流}    */
    { UINT_C|D_DOT1,        msg_comp_pv_24 },				/* {变频模块温度}    */
    { UINT_0,               msg_comp_pv_25 },				/* {变频限频信息}    */
    { UINT_A|D_DOT1,        msg_comp_pv_26 },				/* {变频输入电流}    */
    { UINT_0|D_DOT2,        msg_comp_pv_27 },				/* {变频输出功率}    */
    { UINT_RM,              msg_comp_pv_28 },				/* {风机1目标运行}   */
    { UINT_RM,              msg_comp_pv_29 },				/* {风机1当前转速}   */
    { UINT_RM,              msg_comp_pv_30 },				/* {风机2目标运行}   */
    { UINT_RM,              msg_comp_pv_31 },				/* {风机2当前转速}   */
    { UINT_0,               msg_comp_pv_32 },				/* {备用} */
    { UINT_0,               msg_comp_pv_33 },				/* {备用} */
    { UINT_C|D_DOT1,        msg_comp_pv_34 },				/* 冷媒气侧温度 */
    { UINT_C|D_DOT1,        msg_comp_pv_35 },				/* 冷媒液侧温度 */
    { UINT_0,               msg_comp_pv_36 },				/* 模块功率 */
};
U16 get_comp_p_num(void)
{
    return ARRELE(msg_uint_p_comp);
}

/* 模块单元模拟量 */
const MSG_ITEM_INFO msg_uint_p_cell[] =
{
    { UINT_C|D_DOT1,  msg_uint_pv_01 },     /* 板换回水温度 */
    { UINT_C|D_DOT1,  msg_uint_pv_02 },     /* 板换出水温度 */
    { UINT_C|D_DOT1,  msg_uint_pv_03 },     /* 总出水温度 */
    { UINT_C|D_DOT1,  msg_uint_pv_04 },     /* 环境温度 */
    { UINT_RM,        msg_uint_pv_05 },     /* 变频水泵目标转速 */
    { UINT_LH,        msg_uint_pv_06 },     /* 变频水泵当前流量 */
    { UINT_PE|D_DOT1, msg_uint_pv_07 },     /* 变频水泵控制信号 */
    { UINT_PE|D_DOT1, msg_uint_pv_08 },     /* 变频水泵反馈信号 */
    { UINT_0,         msg_uint_pv_09 },     /* 变频水泵异常码 */
    { UINT_0|D_DOT2,  msg_uint_pv_10 },     /* 机组标定能力 */
    { UINT_0|D_DOT2,  msg_uint_pv_11 },     /* 机组当前能力 */
    { UINT_0,         msg_uint_pv_12 },     /* 变频水泵当前流量 */
};
U16 get_unit_p_num(void)
{
    return ARRELE(msg_uint_p_cell);
}


/* 模块压机输入 */
const U16 msg_uint_i_comp[] = // 待更改
{
    msg_nothing,
    msg_nothing,
};

/* 模块单元输入 */
const U16 msg_uint_i_cell[]=/* 待更改 */
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

/* 系统输入 */
const U16 msg_sys_i[] =/* 待更改 */
{
    msg_nothing,
    msg_nothing,
    msg_nothing,
};

/**变频故障参数**************************************************/
const MSG_ITEM_INFO msg_invt_para[] =   // 变频故障参数信息
{
    {   0xFFFF,         msg_invt_fp_para_01,    },   /* 最近一次故障类型 */
    {   UINT_H,         msg_invt_fp_para_02,    },   /* 最近一次故障时累计运行时间 */
    {   UINT_F|D_DOT2,  msg_invt_fp_para_03,    },   /* 最近一次故障时的运行频率 */
    {   UINT_F|D_DOT2,  msg_invt_fp_para_04,    },   /* 最近一次故障时的给定频率 */
    {   UINT_A|D_DOT1,  msg_invt_fp_para_05,    },   /* 最近一次故障时的输出电流 */
    {   UINT_V|D_DOT1,  msg_invt_fp_para_06,    },   /* 最近一次故障时的输出电压 */
    {   D_DOT2,         msg_invt_fp_para_07,    },   /* 最近一次故障时的输出功率 */
    {   UINT_V|D_DOT1,  msg_invt_fp_para_08,    },   /* 最近一次故障时的母线电压 */
    {   UINT_C|D_DOT1,  msg_invt_fp_para_09,    },   /* 最近一次故障时的逆变桥温度 */
    {   UINT_0,         msg_invt_fp_para_10,    },   /* 最近一次故障时的限频信息 */
    {   UINT_A,         msg_invt_fp_para_11,    },   /* 最近一次故障时的输入电流 */
    {   UINT_V,         msg_invt_fp_para_12,    },   /* 最近一次故障时的输入电压 */
    {   0xFFFF,         msg_invt_fp_para_13,    },   /* 倒数第二次故障类型 */
    {   UINT_H,         msg_invt_fp_para_02,    },   /* 倒数第二次故障时累计运行时间 */
    {   0xFFFF,         msg_invt_fp_para_15,    },   /* 倒数第三次故障类型 */
    {   UINT_H,         msg_invt_fp_para_02,    },   /* 倒数第三次故障时累计运行时间 */
    {   0xFFFF,         msg_invt_fp_para_17,    },   /* 倒数第四次故障类型 */
    {   UINT_H,         msg_invt_fp_para_02,    },   /* 倒数第四次故障时累计运行时间 */
    {   0xFFFF,         msg_invt_fp_para_19,    },   /* 倒数第五次故障类型 */
    {   UINT_H,         msg_invt_fp_para_02,    },   /* 倒数第五次故障时累计运行时间 */
    {   UINT_H,         msg_invt_fp_para_21,    },   /* 故障时的单次运行时间 */
};



/* dm603_common_XXX表示公共函数 */

/**
 * 函数 : dm603_common_disp_flash()
 * 功能 : 刷新
 *
 * 参数 : --
 * 返回 : --
 *
 * 备注 : --
 */
static void dm603_common_disp_flash(void)
{
    fg_flash_all = TRUE;
    fg_is_auto_flash_all = FALSE;   /* 非自动刷新 */
}

/**
 * 函数 : dm603_common_disp_clr_line()
 * 功能 : 刷新
 *
 * 参数 : --
 * 返回 : --
 *
 * 备注 : --
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
 * 函数 : dm603_common_toggle_byte_bit()
 * 功能 : 取反字节对应的位值
 *
 * 参数 : @pd 字节
 *        @b  位
 * 返回 : --
 *
 * 备注 : --
 */
void dm603_common_toggle_byte_bit(U08 *pd, U08 b)
{
    if (GET_BIT(*pd, b))    CLR_BIT(*pd, b);
    else                    SET_BIT(*pd, b);
}

/**
 * 函数 : dm603_common_disp_menu()
 * 功能 : 菜单栏显示
 *
 * 参数 : @licon 左侧图标
 *        @lstr  左侧字符串
 *        @ricon 右侧图标
 *        @rstr  右侧字符串
 * 返回 : --
 *
 * 备注 : --
 */
static void dm603_common_disp_menu(U16 licon, U16 lstr, U16 ricon, U16 rstr, U08 *disp_buf)
{
//  U16 tmp_str;
//  U16 tmp_icon;

//NOTE-CXW 1.已屏蔽以下调转语句,所有用到的地方已调转
//NOTE-CXW 2.需把48改为ROW4,或ITEM_PER_PAGE*16（考虑做成参数传入）-------------
//NOTE-CXW 3.需确认并统一提示按键的通用字符串，不需存在多个。包括返回、确认等-------------

//  tmp_str = lstr;
//  tmp_icon = licon;
//
//  lstr = rstr;
//  licon = ricon;
//
//  rstr = tmp_str;
//  ricon = tmp_icon;
// ---------------------------------------

    /* 左侧图标和字符串 */
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

    /* 右侧图标和字符串 */
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
 * 函数：dm603_common_disp_select()
 * 功能 : 项切换
 *
 * 参数 : @item 当前项值
 *        @opt 向上或向下
 *        @min 最小项数目
 *        @max 最大项数目
 *        @cursor 光标选项
 * 返回 : 需要修改的项值
 *
 * 备注 : ①光标只有3项值:0->表示最上反白; 2->表示最下反白
 */
U08 dm603_common_disp_select(U08 item, U08 opt, U08 min, U08 max)
{
    switch (opt)
    {
        case MENU_UP :  /* 向上翻页 */
            if (item-- <= min)
            {
                item = max - 1; /* 翻转到最末项 */
            }
            break;

        case MENU_DOWN :    /* 向下翻页 */
            if (++item >= max)
            {
                item = min;     /* 翻转到第一项 */
            }
            break;
    }

    return item;
}

/**
 * 函数 : dm603_face_menu_eeprom()
 * 功能 : 定时时间及历史故障保存
 *
 * 参数 : @opt 表示数据处理类型
 *        MENU_WR 0 表示保存
 *        MENU_RD 1 表示读取
 *        MENU_IN 2 表示初始化并保存
 *        @tbl 表示操作数据
 * 返回 : --
 *
 * 备注 : ①每次处理都是对所有数据进行操作
 */
//void dm603_face_menu_eeprom(U08 opt, U08 *tbl)
//{
//  U08 i;
//  U08 max = 0;
//  U16 addr = 0;
//  U08 init_value = 0;
//
//  max  = sizeof(his_err_tbl)/sizeof(U08); /* 组 */
//  addr = MEM_HIS_ERROR_ADDR;
//  if (tbl == dm603_time_tbl)
//  {
//      max = sizeof(dm603_time_tbl)/sizeof(U08);   /* 组 */
//      addr = MEM_DM603_TIME_ADDR;
//  }
//  else if (tbl == &dm603_language)
//  {
//      max = 1;    /* 组 */
//      addr = MEM_LANGUAGE_ADDR;
//  }
//
//  switch (opt)
//  {
//      case MENU_RD :  /* 读取 */
////            if (tbl == dm603_time_tbl)  debug_7[3]++;
////            if (rdeeprom(tbl, addr, max, 0) == IIC_NORMAL) break;
////            if (rdeeprom(tbl, addr, max, 0) == IIC_NORMAL) break;
//          if (eprom_rd_near_crc(tbl, addr, max)) break;    -LXH
//          if (eprom_rd_near_crc(tbl, addr, max)) break;
//          /* 两次读取错误则重新初始化 */
//          /* break; */
//
//      case MENU_IN :  /* 初始化 */
//          if (tbl == his_err_tbl)     init_value = F_NO_ERR;
//          else                        init_value = 0;
//
//          for (i = 0; i < max; i++)
//          {
//              tbl[i] = init_value;
//          }
//          /* break; */
//
//      case MENU_WR :  /* 保存 */
////            if (tbl == dm603_time_tbl)  debug_7[4]++;
////            wreeprom(tbl, addr, max);
//          eprom_wr_near_crc(tbl, addr, max);   -LXH
//          break;
//  }
//}


/**
 * 函数 : dm603_common_disp_bound()
 * 功能 : 确定显示项的范围
 *
 * 参数 : @item 当前项值
 *        @min 最小项数目
 *        @max 最大项数目
 *        @cursor 光标选项
 *        @new_min 新的最小项值
 *        @new_max 新的最大项值
 * 返回 : --
 *
 * 备注 : ①确定需要显示项的最小值及最大值
 *        ②反白显示的项需要减去最小项值
 */
U08 dm603_common_disp_bound(U08 item, U08 min, U08 max, U08 mod_page_max, U08 *new_min, U08 *new_max)
{
    U08 cursor = 0;

    /* 溢出处理 */
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
 * 函数 : dm603_common_disp_arrow()
 * 功能 : 向上或向下箭头
 *
 * 参数 : @cursor 当前项
 *        @min 最小项
 *        @max 最大项
 * 返回 : --
 *
 * 备注 : ①可依据不同的行位置来显示箭头
 *        ②行值均为第一行与第三行 ROW1 ROW3
 *        ③列值均为最右边 128-8
 */
void dm603_common_disp_arrow(U08 cursor, U08 min, U08 max, U08 *disp_buf)
{
    BOOL fg_up_arrow = FALSE;       /* 是否显示向上箭头 */
    BOOL fg_down_arrow = FALSE;     /* 是否显示向下箭头显示 */

    if (min + 1 < max)              /* 只有一项时不显示箭头 */
    {
        if (cursor == min)          /* 只显示向下箭头 */
        {
            fg_down_arrow = TRUE;
        }
        else if (cursor == max-1)   /* 只显示向上箭头 */
        {
            fg_up_arrow = TRUE;
        }
        else                        /* 同时显示向上和向下箭头 */
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

/* 状态查询应用子函数接口 */
/**
 * 函数 : dm603_common_count_out()
 * 功能 : 计算输出数据有效位
 *
 * 参数 : @ptb 存入有效位的数据值
 *        @a1 压机输出有效位的辅助信息
 *        @a2 单元输出有效位辅助信息
 *        @comp 模块压机个数
 *                0xff -> 表示为系统板,需要赋值comp=1
 *                非ff -> 表示为模块板,直接使用压机数即可
 * 返回 : 有效位总数
 *
 * 备注 : ①由辅助信息转换当前所有数据的有效位
 *          使用时,直接取ptb数组中的值来用即可
 */
U08 dm603_common_count_out(U08 *ptb, U16 a1, U16 a2, U08 comp)
{
    U08 i, j;
    U08 tmp = 0;
    U08 valid_out_num = 0;  /* 第1个输出属性值的有效位个数 */
    U08 cursor_comp = 0;    /* 压机有效位相对位置 */
    U08 cursor_cell = 0;    /* 单元有效位相对位置 */
    BOOL fg_sys_pcb = FALSE;// 系统板

    /* 是否系统板? */
    if (comp == 0xff)
    {
        fg_sys_pcb = TRUE;
        comp = 1;
    }

    /* 计算相对位置 */
    cursor_comp = 0;
    for (i = 0; i < RLY_BIT_NUM; i++)   //获得一台压机的输出个数(压机和四通阀，所以为2)
    {
        if (a1 & (0x0001 << i))     valid_out_num++;
    }

    /* 有效位计算 */
    for (i=0; i<RLY_BIT_NUM; i++)       /* 第1个输出属性值 */   //压机输出
    {
        if (a1 & (0x0001 << i))
        {
            for (j=0; j<comp; j++)
            {
                tmp = j*valid_out_num + cursor_comp;
                ptb[tmp] = i + (j * RLY_BIT_NUM);
                cursor_cell++;                      //输出个数
            }
            cursor_comp++;
        }
    }

    for (i=0; i<RLY_BIT_NUM; i++)       /* 第2个输出属性值 */   //模块输出
    {
        if (a2 & (0x0001 << i))
        {
            if (fg_sys_pcb)// 系统板两个字节输出
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
 * 函数 : dm603_common_count_pv()
 * 功能 : 计算模拟量数据有效位
 *
 * 参数 : @dest 有效模拟量存入数组
 *        @src  有效模拟量计算数组
 * 返回 : 有效模拟量个数
 *
 * 备注 : --
 */
U08 dm603_common_count_pv(U08 *dest, I16 *src, U08 max_num)
{
    U08 i;
    U08 cursor = 0;

    /* 有效模拟量 */
    for (i = 0; i < max_num; i++)
    {
//        if(i==PV_SUC_ST_INDEX || i==PV_EXH_ST_INDEX)    continue;   // 跳过饱和温度不显示

        if (*src++ != SENSOR_NOEXIST)   /* 无效模拟量判断 */
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
//  /* 有效模拟量 */
//  for (i = 0; i < MAX_INVT_DIS_INFO; i++)
//  {
//      if (*src++ != SENSOR_NOEXIST)   /* 无效模拟量判断 */
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
 * 函数 : dm603_common_disp_val()
 * 功能 : 值显示
 *
 * 参数 : @val 值
 *        @row 行
 *        @col 列
 *        @sign 值单位属性
 *        @color 是否反白
 * 返回 : --
 *
 * 备注 : ①显示值及对应的单位
 *        ②其中值考虑占用5个ASC字符，单位为3个ASC字符，共4个汉字位置
 *        ③如果为负小数值，有效值只有3位，即-99.9
 *        ④只能显示包含一位小数的值
 */
void dm603_common_disp_val(I16 val, U08 row, U08 col, U16 sign, U16 color, U08 *disp_buf)
{
    U16 neg = 0;
    U16 dot = 0;
    U16 dig = DNS_DIGIT5;
    U16 attrib = DCS_NORMAL | DTS_RIGHT;

    /* 单位字符串定义 */
    const U16 msg_uint[] =
    {
        dm603_disp_val_UINT_0 , /* 无 */
        dm603_disp_val_UINT_C ,   /* 温度() */
        dm603_disp_val_UINT_A ,   /* 电流(A) */
        dm603_disp_val_UINT_PR,   /* 压力(bar) */
        dm603_disp_val_UINT_PE,   /* 百分比(%) */
        dm603_disp_val_UINT_F ,   /* 频率(HZ) */
        dm603_disp_val_UINT_RS,   /* 转速(rps) */
        dm603_disp_val_UINT_RM,   /* 转速(rpm) */
        dm603_disp_val_UINT_S ,   /* 秒 */
        dm603_disp_val_UINT_M ,   /* 分 */
        dm603_disp_val_UINT_H ,   /* 时 */
        dm603_disp_val_UINT_D ,   /* 天 */
        dm603_disp_val_UINT_ST,   /* 步 */
        dm603_disp_val_UINT_V,    /* 电压(V) */
        dm603_disp_val_UINT_FL,   /* 水流量(L/H) */
    };

    /* 判断是否为负数? */
    if (val < 0) neg = 1;
    if (neg) dig--;

    if (sign & D_DOTMASK) dot = ((sign & D_DOTMASK) == D_DOT1) ? DNS_FLOAT1 : DNS_FLOAT2;
    if (dot) dig--;
    attrib |= dig | dot;
//  switch (sign & UINT_MASK)
//  {
//      case UINT_0  : /* 无 */
//
//      case UINT_PE : /* 百分比(%) */
//      case UINT_RS : /* 转速(rps) */
//      case UINT_RM : /* 转速(rpm) */
//      case UINT_S  : /* 秒 */
//      case UINT_M  : /* 分 */
//      case UINT_H  : /* 时 */
//      case UINT_D  : /* 天 */
//      case UINT_ST : /* 步 */
//          //if (neg) dig--;
//          attrib |= dig;
//          break;
//
//      case UINT_PR : /* 压力(bar) */
//      case UINT_C  : /* 温度() */
//      case UINT_A  : /* 电流(A) */
//      case UINT_F  : /* 频率(HZ) */
//          //if (neg) dig--;
//          if (dot) dig--;
//          attrib |= dig | dot;
//          break;
//
//      default :
//          break;
//  }

    /* 显示值和单位 */
    //if (!SensorIsError(val)) disp_value_I16(val, row, col, attrib | color, disp_buf);
    //else                   disp_string(dm603_disp_val_UINT_ERROR, row, col, color, disp_buf);
    //disp_string(msg_uint[(U08)((sign&UINT_MASK) >> 4)], row, col+5*8, DCS_NORMAL, disp_buf);
    col = col - 3*8;
    if (!SensorIsError(val)) disp_value_I16(val, row, col, attrib | color, disp_buf);
    else                     disp_string(dm603_disp_val_UINT_ERROR, row, col-5*8, color, disp_buf);
    disp_string(msg_uint[(U08)((sign&UINT_MASK) >> 4)], row, col, DCS_NORMAL, disp_buf);
}

/**
 * 函数 : dm603_face_open()
 * 功能 : 开机界面
 *
 * 参数 : --
 * 返回 : --
 *
 * 备注 : --
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
            //disp_snd_100ms(); /* 开机响 */
            /* break; */

        case DM603_MENU_PROC :
//          if (fg_test != 2)
//          {
                /* 前2秒刷新，防止刷新标志丢失 */
                if (dm603_power_time[hmi_t_num] < 1) dm603_common_disp_flash();

                if (dm603_power_time[hmi_t_num] > DM603A_OPEN_TIME)
                {
                    dm603_face[hmi_t_num] = FACE_MAIN;
                    return;
                }
                /* 返回主界面 */
                if (Main_Key[hmi_t_num] == DM603_KEY_FUNC)
                {
                    dm603_face[hmi_t_num] = FACE_MAIN;
                    return;
                }
                /* 进入使用期限设置 */
                if (Main_Key[hmi_t_num] == DM603_KEY_OK_FUNC)
                {
                    dm603_push_face_status();   /* 保存界面状态 */
                    dm603_set_nextface_status(FACE_LIMIT);  // 确认密码正确后要跳转的界面
                    dm603_face[hmi_t_num] = FACE_PASS;
                    dm603_PassNeed[hmi_t_num] = PASS_LMT;

                    return;
                }

                /* 进入测试程序 */
                if(Main_Key[hmi_t_num] == DM603_KEY_ONOFF_3S)
                {
                    test_allow = TRUE;
                }
                if (test_allow == TRUE && Main_Key[hmi_t_num] == DM603_KEY_ONOFF_UP)
                {
                    punp_fg = TRUE;
                    disp_clr_screen(disp_buf);      //进入前先清屏
                    dm603_face[hmi_t_num] = FACE_PUNP_INIT;
                }


                /* 切换语言 */
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

            /* 显示调试 */
            #ifdef USE_DISP_DBGVER
            {
                disp_string_ptr8(DEBUG_VER, ROW3, 0, DTS_CENTER, disp_buf);
            }
            #endif

            break;
    }
}

//#define OUTPUT_NUM    16  // 输出个数     //NOTE-CXW 看下底层是否有相关的宏定义，如有则替换
//#define ANALOG_NUM    16  // 模拟量个数
/* pcb_num模块序号 */
static void DispStatus_Analog(U08 Pcb_num, U08 Page_now, const U08 *Tbl, U08 *disp_buf)
{
    U08 x, tmp,col;
    U16 attrib = DCS_NORMAL;

    // 状态标题
    if (Pcb_num == MAX_PCB)
    {
        disp_string(dm603_face_menu_status_sys, ROW1, 0,   DCS_NORMAL, disp_buf);
        disp_string(dm603_face_menu_status_pv,  ROW1, 4*8, DCS_NORMAL, disp_buf);
    }
    else    // 模块
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
        //弃用attrib = dm603_common_get_val_unit(msg_sys_p[x].msg);
        dm603_common_disp_val(cell[Pcb_num].cell_pv[x], ROW3, COL_END - COL_ASC, attrib, \
                              DCS_NORMAL, disp_buf);
    }
    else    // 模块
    {

        if(x >= CELL_PV_BEGIN)  // 单元模拟量
        {
            disp_string(msg_uint_p_cell[x-CELL_PV_BEGIN].msg, ROW2, 0, DCS_NORMAL, disp_buf);
            attrib = msg_uint_p_cell[x-CELL_PV_BEGIN].attr;
            //弃用attrib = dm603_common_get_val_unit(msg_uint_p_cell[x-CELL_PV_BEGIN].msg);
            dm603_common_disp_val(cell[Pcb_num].cell_pv[x], ROW3, COL_END - COL_ASC, attrib, \
                                  DCS_NORMAL, disp_buf);
        }
        else    // 单台压机模拟量
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
            //弃用attrib = dm603_common_get_val_unit(msg_uint_p_comp[tmp].msg);
            dm603_common_disp_val(cell[Pcb_num].cell_pv[x], ROW3, COL_END - COL_ASC, attrib, \
                                      DCS_NORMAL, disp_buf);
        }
    }
}

#define MAX_OUT_DISP    4   // 每页最多显示几个输出
static void DispStatus_Output(U08 Pcb_num, U08 Page_now, U08 Item_num, const U08 *Tbl, U08 *disp_buf)
{
    U08 i, x, tmp;
    U16 attrib = DCS_NORMAL;

    U08 out_index;  // 本页显示的输出开始序号
    U08 max;        // 本页显示的输出个数
    U08 row, col, col2;

    // 状态标题
    if (Pcb_num == MAX_PCB)
    {
        disp_string(dm603_face_menu_status_sys, ROW1, 0,   DCS_NORMAL, disp_buf);
        disp_string(dm603_face_menu_status_out, ROW1, 4*8, DCS_NORMAL, disp_buf);
    }
    else    // 模块
    {
        disp_value_U08(Pcb_num, ROW1, 0, DCS_NORMAL | DNS_DIGIT2 | DNS_ZERO, disp_buf);
        disp_string_ptr8("#", ROW1, 2*8, DCS_NORMAL, disp_buf);
        disp_string(dm603_face_menu_status_uint, ROW1, 3*8, DCS_NORMAL, disp_buf);
        disp_string(dm603_face_menu_status_out,  ROW1, 7*8, DCS_NORMAL, disp_buf);
    }

    max = MAX_OUT_DISP;
    out_index = Page_now * MAX_OUT_DISP;        // 本页输出的开始序号
    if ((out_index + MAX_OUT_DISP) > Item_num)  // 已到输出最后一页
    {
        max = Item_num - out_index;             // 本页实际输出个数
    }

    for(i=0; i<max; i++)
    {
        x = Tbl[out_index+i];   /* 取输出代号 */

        /* 第二行或第三行 */
        if (i < 2)  row = ROW2;
        else        row = ROW3;

        col = (i&0x01)*8*8;

        /* 反白显示 */
        attrib = DCS_NORMAL;
        if (get_output(Pcb_num, x)) attrib = DCS_REVERSE;

        if (Pcb_num == MAX_PCB)
        {
            disp_string(msg_sys_o[x], row, col, attrib, disp_buf);
        }
        else
        {
            /* 压机输出 */
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
            else    /* 单元输出 */
            {
                tmp = x - (MAX_COMP * RLY_BIT_NUM);
                disp_string(msg_uint_o_cell[tmp], row, col, attrib|DCS_FLASH, disp_buf);
            }
        }

    }

}

/**
 * 函数 : dm603_face_menu_status()
 * 功能 : 模块状态
 *
 * 参数 : --
 * 返回 : --
 *
 * 备注 : --
 */
static void dm603_face_menu_status(U08 *disp_buf)
{
    U08 s_o_max;                                /* 最大输出个数 */
    U08 s_p_max;                                /* 最大模拟量个数 */
    U08 s_page_max;                             /* 最大页数 */
    U08 cell_o_tbl[MAX_RELAY * RLY_BIT_NUM];    /* 输出状态值 */
    U08 cell_p_tbl[MAX_PV];                     /* 模拟状态值 */

    static U08 page_index;                      /* 所有查询页索引 */
    static U08 s_pcb = 0;                       /* 模块序号 */

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

            /* 清除无效模拟量值 */
//          dm603_common_clr_pv(s_pcb);
            /* break; */

        case DM603_MENU_PROC :
            /* 有效数据初始化 */
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
                s_page_max = s_p_max + _DIVUCARRY(s_o_max, 4);      //计算模块页数：输出每页显示4项
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
                        dm603_pop_face_status();    // 恢复上层界面状态
                        return;
//                      break;

                    case DM603_KEY_OK:      // 切换模块
                        {
                            if (s_pcb == MAX_PCB)               s_pcb = 0;
                            else if (++s_pcb >= val_pcb_num)    s_pcb = MAX_PCB;

                            page_index = 0;
                            disp_clr_screen(disp_buf);  // 切换模块后清屏返回，以便确认模块通讯后再显示
                            dm603_common_disp_flash();
                            return;
                        }
//                      break;

                    case DM603_KEY_UP:      // 向上翻页
                        if (!fg_mod_cannot_use)
                        {
                            if (page_index-- <= 0)  page_index = s_page_max - 1;
                        }
                        break;

                    case DM603_KEY_DOWN:    // 向下翻页
                        if (!fg_mod_cannot_use)
                        {
                            if (++page_index >= s_page_max)   page_index = 0;
                        }
                        break;

                    case DM603_KEY_OK_FUNC:
                        dm603_push_face_status();   /* 保存界面状态 */
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
            if (fg_mod_cannot_use)      /* 不可用，做提示 */
            {
                disp_value_U08(s_pcb, ROW2, 0, DCS_NORMAL | DNS_DIGIT2 | DNS_ZERO, disp_buf);
                disp_string(msg_mod_cannot_use, ROW2, 16, DCS_NORMAL, disp_buf);
            }
            else                /* 无通讯故障时才显示状态 */
            {
                if (page_index < s_p_max)   // 模拟量
                {
                    DispStatus_Analog(s_pcb, page_index, cell_p_tbl, disp_buf);                     // 显示当前页模拟量
                }
                else    // 输出
                {
                    U08 out_page_index = page_index-s_p_max;
                    DispStatus_Output(s_pcb, out_page_index, s_o_max, cell_o_tbl, disp_buf);    // 显示当前页输出
                }
                /* 箭头 */
                dm603_common_disp_arrow(page_index, 0, s_page_max, disp_buf);
            }

            /* 菜单栏 */
            dm603_common_disp_menu(BMP_FN, dm603_face_return, BMP_OK, dm603_face_menu_status_chg, disp_buf);
            break;
    }
}

/**
 * 函数 : dm603_face_menu_clock()
 * 功能 : 时钟查询
 *
 * 参数 : --
 * 返回 : --
 *
 * 备注 : --
 */
static void dm603_face_menu_clock(U08 *disp_buf)
{
//  U08 i = 0;
//  U16 attrib = 0;

//  static U08 cursor = 0;  /* 项 */
//  static RTC s_clock; /* 时钟备份 */

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
                if (Main_Key[hmi_t_num] == DM603_KEY_OK)    /* 返回 */
                {
                    dm603_pop_face_status();
                    return;
                }
            }
            /* break; */

        case DM603_MENU_PAINT :
            disp_set_clk(disp_buf);

            /* 防止屏保 */
            cnt_no_operate[hmi_t_num] = 0;
            cnt_no_operate_sec[hmi_t_num] = 0;

            /* 菜单栏 */
            disp_string(dm603_face_menu_clock_info, ROW1, 0, DCS_NORMAL | DTS_CENTER, disp_buf);
            dm603_common_disp_menu(BMP_FN, dm603_face_menu_clock_chg, BMP_OK, dm603_face_ok, disp_buf);
            break;
    }
}
#ifdef COM_FCU_FBUS
/*******************************************************************************
 函 数: dm603_face_menu_fcu()
 功 能: 风盘信息
 参 数: disp_buf--;
 返 回:
 其 它: 空
*******************************************************************************/
const U16 fcu_status[] =
{
    dm603_face_menu_fcu_sta_00, // 未连接
    dm603_face_menu_fcu_sta_01, // 故障态
    dm603_face_menu_fcu_sta_02, // 待机态
    dm603_face_menu_fcu_sta_03, // 有需求
    dm603_face_menu_fcu_sta_04, // 无需求
    dm603_face_menu_fcu_sta_05, // 未定义
};
const U16 fcu_mode[] =
{
    dm603_nothing,
    dm603_face_menu_fcu_mode_01,    // 制冷
    dm603_face_menu_fcu_mode_02,    // 制热
    dm603_face_menu_fcu_mode_03,    // 通风
    dm603_face_menu_fcu_mode_04,    // 地暖
    dm603_face_menu_fcu_mode_05,    // 未知
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
                if (Main_Key[hmi_t_num] == DM603_KEY_FUNC)  /* 返回 */
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


            /* 箭头 */
            dm603_common_disp_arrow(num, 0, val_fcu_num, disp_buf);
            /* 菜单栏 */
            dm603_common_disp_menu(BMP_FN, dm603_face_return, BMP_OK, dm603_face_ok, disp_buf);
            break;
    }
}
#endif
/*******************************************************************************
 函 数: invt_code_to_version()
 功 能: 变频器版本号转换
 参 数: ver--版本号字符串;
        func_code--功能码;
        ver_code--版本码;
 返 回: 空
 其 它: 空
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
        char symbol[] = {'X', 'K', 'F', 'T', 'Q'};      /* 洗涤、空调、辅机、通用、其他 */

        MEMCPY(ver, "X1.VF$$$$.$##.V100###", INVT_VER_NUM); /* 例：X1.VF250A.F01.V100A00 */

        /* 版本号转换 */
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
 函 数: dm603_disp_version()
 功 能: 显示版本
 参 数: disp_buf--缓存;
        row--行;
        p_ver--版本字符串;
 返 回: 空
 其 它: 空
*******************************************************************************/
void dm603_disp_version(U08 *disp_buf, U08 row, U08 *p_ver)
{
    U08 CODE_STR[MAX_VER_LINE+1];

    //显示第一行
    ARRCLR(CODE_STR);
    MEMCPY(CODE_STR, p_ver, MAX_VER_LINE1);
    disp_string_ptr8(CODE_STR, row, 0, DCS_NORMAL, disp_buf);

    //显示第二行
    ARRCLR(CODE_STR);
    MEMCPY(CODE_STR, &p_ver[MAX_VER_LINE1], MAX_VER_LINE2(p_ver));
    disp_string_ptr8(CODE_STR, row+16, 0, DCS_NORMAL, disp_buf);
}
/**
 * 函数 : dm603_face_menu_version()
 * 功能 : 版本查询
 *
 * 参数 : --
 * 返回 : --
 *
 * 备注 : --
 */
static void dm603_face_menu_version(U08 *disp_buf)
{
    #define MAX_VER_LINE        15  /* 一行最多字符数 */
    #define MAX_VER_LINE1       14  /* 第一行字符数 */
    #define MAX_VER_LINE2(p)    MIN(STRLEN(p)-MAX_VER_LINE1, MAX_VER_LINE)

    U08 CODE_STR[MAX_VER_LINE+1];
    static I08 cursor;  /* 项 */


    enum VERSION_ITEM   /* 用于处理版本号的有无 */
    {
        VER_IN_0,           /* 内机板0 */
        VER_OUT_0,          /* 外机板0 */
        VER_IN_1,           /* 内机板1 */
        VER_OUT_1,          /* 外机板1 */
        VER_IN_2,           /* 内机板2 */
        VER_OUT_2,          /* 外机板2 */
        VER_IN_3,           /* 内机板3 */
        VER_OUT_3,          /* 外机板3 */
        VER_IN_4,           /* 内机板4 */
        VER_OUT_4,          /* 外机板4 */
        VER_IN_5,           /* 内机板5 */
        VER_OUT_5,          /* 外机板5 */
        VER_IN_6,           /* 内机板6 */
        VER_OUT_6,          /* 外机板6 */
        VER_IN_7,           /* 内机板7 */
        VER_OUT_7,          /* 外机板7 */
        VER_FRONT,          /* 前板 */
        VER_INVT_CM,        /* 变频压机 */
        VER_INVT_FAN1,      /* EC风机1 */
        VER_INVT_FAN2,      /* EC风机2 */
        VER_MAX
    };

    U08 disp_ver_max = VER_MAX;

    /* 用于箭头指示 */
    if (!val_inverter_used)                     disp_ver_max--;/* 变频压机 */
    if (val_freq_fan_set != FREQ_FAN_SET_EC)    disp_ver_max--;/* EC风机1 */
    if (val_freq_fan_set != FREQ_FAN_SET_EC)    disp_ver_max--;/* EC风机2 */

    switch (dm603_menu[hmi_t_num])
    {
        case DM603_MENU_CREATE :
            dm603_menu[hmi_t_num] = DM603_MENU_PROC;
            key_delay_time[hmi_t_num]=0;
            dm603_common_disp_flash();
            cursor = 0; /* 项 */
            /* break; */

        case DM603_MENU_PROC :
            /* 返回主界面 */
            if(key_delay_time[hmi_t_num]>=KEY_DELAY)
            {
                if (Main_Key[hmi_t_num] == DM603_KEY_FUNC)
                {
                    dm603_pop_face_status();    /* 恢复上层界面状态 */
                    return;
                }

                /* 向上或向下切换 */
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
//                            cursor = VER_IN_7+1;          /* 从开始 */
//                        }
                    }

                    /* 处理没有的版本 */
                    switch (cursor)         /* 从小到大判断 */
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
                    /* 处理没有的版本 */
                    switch (cursor)         /* 从大到小判断 */
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
                        //显示第一行
                        ARRCLR(CODE_STR);
                        MEMCPY(CODE_STR, p_ver, MAX_VER_LINE1);
                        disp_string_ptr8(CODE_STR, ROW2, 0, DCS_NORMAL, disp_buf);
                        //显示第二行
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
                        //显示第一行
                        ARRCLR(CODE_STR);
                        MEMCPY(CODE_STR, p_ver, MAX_VER_LINE1);
                        disp_string_ptr8(CODE_STR, ROW2, 0, DCS_NORMAL, disp_buf);
                        //显示第二行
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
                    /* 正常显示编码 */
//                  disp_string_ptr8(dm603_code_buf, ROW2, 0, DCS_NORMAL, disp_buf);
//                    break;
                default:
                    break;
            }

            /* 箭头 */
            dm603_common_disp_arrow(cursor, 0, disp_ver_max, disp_buf);
            /* 菜单栏 */
            dm603_common_disp_menu(BMP_FN, dm603_face_return, BMP_NU, NULL, disp_buf);

            break;
    }
}


/**
 * 函数 : dm603_face_menu_time_set()
 * 功能 : 定时时间设置
 *
 * 参数 : @num 定时参数组
 *        ①00    表示一次定时
 *        ②01~10 表示XX组定时，一共10组
 * 返回 : --
 *
 * 备注 : --
 */
static void dm603_face_menu_time_set(U08 *disp_buf)
{
    U08 i;
    U16 attrib = 0;
    U08 num = dm603_item[hmi_t_num];

    /* 光标位 */
    static U08 s_cursor;
    static U08 s_min_cursor;
    static U08 s_max_cursor;

    /* 数据值 */
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

            /* 定时光标选项 */
            s_cursor = 0;
            s_min_cursor = 0;
            if (num) s_max_cursor = 10;
            else     s_max_cursor = 3;
            if (num == 0)   //定时使用否
            {
                s_max_cursor = 1;
            }
            else if (num == 1)  //一次定时
            {
                s_max_cursor = 4;
            }
            else                //星期定时
            {
                s_max_cursor = 10;
            }
            msg = msg_menu_time_select[num];

            /* 定时时间值 */
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
                /* 返回上级界面 */
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

                    /* 保存定时时间 */
//                  dm603_face_menu_eeprom(MENU_WR, dm603_time_tbl);
                    DataAccess_Timer(DATA_WRITE);

                    dm603_pop_face_status();    /* 恢复上层界面状态 */
                    return;
                }
                /* 切换定时项 */
                if (Main_Key[hmi_t_num] == DM603_KEY_OK)
                {
                    if (++s_cursor >= s_max_cursor) s_cursor = s_min_cursor;
                }

                /* 向上或向下切换 */
                if (Main_Key[hmi_t_num] == DM603_KEY_UP || Main_Key[hmi_t_num] == DM603_KEY_DOWN)
                {
                    if (num == 0)   //定时使用否设置
                    {

    //                  if (Main_Key[hmi_t_num] == DM603_KEY_UP || Main_Key[hmi_t_num] == DM603_KEY_DOWN)
    //                  {
                            if (s_cursor == 0)  dm603_common_toggle_byte_bit(&s_time, 0);           /* 定时开关 */
    //                  }
                    }
                    else
                    {
                        if (s_cursor == 1)/* 定时时 */
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
                        else if (s_cursor == 2)/* 定时分 */
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
                        else    /* 定时开关位 */
                        {
                            /* 向上或向下切换 */
    //                      if (Main_Key[hmi_t_num] == DM603_KEY_UP || Main_Key[hmi_t_num] == DM603_KEY_DOWN)
    //                      {
                                if (s_cursor == 0)                      dm603_common_toggle_byte_bit(&s_time, 7);           /* 定时开关 */
                                else if((s_cursor == 3)&&(num == 1))        dm603_common_toggle_byte_bit(&s_time, 6);           /* 一次定时使用开关 */
                                else                                    dm603_common_toggle_byte_bit(&s_time, s_cursor-3);  /* 定时星期 */
    //                      }
                        }
                    }
//                  dm603_common_disp_flash();  // 刷新界面
                }

            }
            /* 刷新界面 */
//          if (Main_Key[hmi_t_num] == DM603_KEY_UP || Main_Key[hmi_t_num] == DM603_KEY_DOWN) dm603_common_disp_flash();
            /* break; */

        case DM603_MENU_PAINT :
            if (Main_Key[hmi_t_num] != KEY_NULL)    dm603_common_disp_flash();
            /* 定时组 */
            disp_string(msg, ROW1, 0, DCS_NORMAL, disp_buf);

            /* 定时时间 */
            for (i = 0; i < s_max_cursor; i++)
            {
                /* 是否反色 */
                attrib = DCS_NORMAL;
                if (i == s_cursor) attrib = DCS_REVERSE;

                /* 定时开关 */
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
                else if (i == 1)        /* 定时时 */
                {
                    disp_value_U08(s_hour, ROW2, 3*16+32, DNS_ZERO | DNS_DIGIT2 | attrib, disp_buf);
                }
                else if (i == 2)        /* 定时分 */
                {
                    disp_string_ptr8(":", ROW2, 3*16+32+2*8, DCS_NORMAL, disp_buf);
                    disp_value_U08(s_min, ROW2, 3*16+32+2*8+8, DNS_ZERO | DNS_DIGIT2 | attrib, disp_buf);
                }
                else                    /* 定时星期 */
                {   if((i == 3)&&(num == 1))    //一定定时的使用开关 jxj2016.01.15
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

            /* 菜单栏 */
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
 * 函数 : dm603_face_menu_time()
 * 功能 : 定时设置
 *
 * 参数 : --
 * 返回 : --
 *
 * 备注 : ①定时时间设置
 */
static void dm603_face_menu_time(U08 *disp_buf)
{
    U08 i = 0;
//  U08 tmp = 0;
    U08 row = 0;
    U08 min, max;
    U16 attrib = 0;
    U08 menu_opt = MENU_DOWN;
//  static BOOL fg_enable_set = FALSE;  /* 定时使用设置 */
    U08 mod_page_max = 3;                   /* 每页最大项目数 */
    U08 item_max = MENU_TIME_SELECE_MAX;    //最大定时项数
    static U08 s_time_en;               //定时使用否，使用为1，不使用为0
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
                    case DM603_KEY_FUNC:        /* 返回上层界面 */
                        dm603_pop_face_status();    /* 恢复上层界面状态 */
                        return;
//                      break;

                    case DM603_KEY_OK:      /* 进入下层界面 */
                        dm603_push_face_status();   /* 保存界面状态 */
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
//                        if ((status_run==_IDLE || status_run==_ALARM)   // 待机时可调试水泵
//                        && (!fg_freeze_ac_act)                          /* 防冻时不处理 */
//                        )
//                        {
//                            debug_mode = TRUE;
//                            dm603_pop_face_status();    /* 恢复上层界面状态 */
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
                                            mod_page_max, &min, &max);      //求出显示始末，并得出反白显示项
            for (i=min; i<max; i++)
            {
                attrib = DCS_NORMAL;
                if ((i-min) == cursor)                  attrib = DCS_REVERSE;   /* 反白显示项 */
//              if (i == TIME_FUNC && fg_enable_set)    attrib = DCS_NORMAL;

                row = ROW1 + (i-min)*16;
                disp_string(msg_menu_time_select[i], row, 0, attrib | DTS_CLR_TRAIL, disp_buf);

            }

            /* 箭头 */
            dm603_common_disp_arrow(dm603_item[hmi_t_num], MENU_TIME_SELECE_MIN, item_max, \
                                    disp_buf);

            /* 菜单栏 */
            dm603_common_disp_menu(BMP_FN, dm603_face_return,  \
                                   BMP_OK, dm603_face_menu_time_select_ent, disp_buf);

            break;
    }
}


/**
 * 函数 : dm603_face_menu_func_runtime()
 * 功能 : 运行时间查询
 *
 * 参数 : --
 * 返回 : --
 *
 * 备注 : --
 */
static void dm603_face_menu_func_runtime(U08 *disp_buf)
{
    U08 i;
    U08 row;

    static U08 s_pcb;   /* 模块号 */
    static U08 cursor;  /* 压机页指针 */

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
                        dm603_pop_face_status();    /* 恢复上层界面状态 */
                        return;
//                      break;


                    case DM603_KEY_OK:      /* 切换模块 */
                        if (++s_pcb >= val_pcb_num) s_pcb = 0;
                        break;

                    /* 向上或向下切换 */
                    case DM603_KEY_UP:
                    case DM603_KEY_DOWN:
                        break;
                }
            }
            /* break; */

        case DM603_MENU_PAINT :
            if (Main_Key[hmi_t_num] != KEY_NULL)    dm603_common_disp_flash();

            /* 模块信息 */
            disp_value_U08(s_pcb, ROW1, 0, DNS_ZERO | DNS_DIGIT2, disp_buf);
            disp_string(dm603_face_menu_func_runtime_unit, ROW1, 2*8, DCS_NORMAL, disp_buf);

            /* 模块不可用 */
            if (!CELL_Is_used(s_pcb)
            ||  err_sys_get_s_commu_err(s_pcb)
                )
            {
                disp_string(msg_mod_cannot_use, ROW2, 0, DTS_CENTER|DTS_CLR_HEAD|DTS_CLR_TRAIL, disp_buf);
                break;
            }

            /* 压机时间 */
            for (i = 0; i < m_val_comp_num(s_pcb); i++)
            {
                row = ROW2 + i*16;

                disp_value_U08(i+1, row, 0*8, DCS_NORMAL|DNS_DIGIT1, disp_buf);
                disp_string_ptr8("#", row, 1*8, DCS_NORMAL, disp_buf);
                disp_string(dm603_face_menu_func_runtime_unit_comp, row, 2*8, DCS_NORMAL, disp_buf);
                dm603_common_disp_val(cell[s_pcb].cell_runtime[i], row, COL_END - COL_ASC, UINT_H, DCS_NORMAL, disp_buf);   /* 压机运行时间 */
            }


            /* 箭头 */
            dm603_common_disp_arrow(cursor, 0, 0, disp_buf);
            /* 菜单栏 */
            dm603_common_disp_menu(BMP_FN, dm603_face_return,  \
                                   BMP_OK, dm603_face_menu_func_runtime_ent, disp_buf);
            break;
    }
}

/**
 * 函数 : dm603_face_menu_func_his_err()
 * 功能 : 历史故障查询
 *
 * 参数 : --
 * 返回 : --
 *
 * 备注 : --
 */
static void dm603_face_menu_func_his_err(U08 *disp_buf)
{
    U08 i;

    U08 s_id = 0;
    U08 s_id_tag = 0; // 历史故障附加号
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
                    s_max_err++;    /* 计算历史故障数量 */
                }
            }
            /* break; */

        case DM603_MENU_PROC :
            /* 切换子界面 */
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
                            dm603_pop_face_status();    /* 恢复上层界面状态 */
                            return;
//                          break;

                        case DM603_KEY_OK:  /* 清空 */
                            if (s_max_err)              //有故障时才允许进入
                            {
                                dm603_menu_sub[hmi_t_num] = FECE_SUB_ACT;
                            }
                            return;
//                          break;

                        case DM603_KEY_UP:  /* 向上切换 */
                            if (s_cursor-- <= s_min_err) s_cursor = s_max_err - 1;
                            break;

                        case DM603_KEY_DOWN:    /* 向下切换 */
                            if (++s_cursor >= s_max_err) s_cursor = s_min_err;
                            break;

                        default:
                            break;
                    }
                }
            }
            else
            {
                if (Main_Key[hmi_t_num] == DM603_KEY_FUNC)      /* 取消 */
                {
                    dm603_menu[hmi_t_num] = DM603_MENU_CREATE;
                }
                else if (Main_Key[hmi_t_num] == DM603_KEY_OK)   /* 确认 */
                {
                    DataAccess_HistoryErr(DATA_INIT);       // 初始化历史故障信息
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
                if (s_max_err)      /* 有历史故障 */
                {
                    /* 历史故障信息 */
                    s_id  = his_err_tbl[s_cursor*HIS_ERR_SIZE+0];
                    s_pcb = his_err_tbl[s_cursor*HIS_ERR_SIZE+1];
                    s_id_tag = his_err_tbl[s_cursor*HIS_ERR_SIZE+2];

                    dm603_disp_err(s_pcb, s_id, s_id_tag, ROW2, disp_buf);

                    /* 历史故障时间 */

                    ClockTmp[hmi_t_num].month = his_err_tbl[s_cursor*HIS_ERR_SIZE+3];
                    ClockTmp[hmi_t_num].day   = his_err_tbl[s_cursor*HIS_ERR_SIZE+4];
                    ClockTmp[hmi_t_num].hour  = his_err_tbl[s_cursor*HIS_ERR_SIZE+5];
                    ClockTmp[hmi_t_num].min   = his_err_tbl[s_cursor*HIS_ERR_SIZE+6];
                    ClockTmp[hmi_t_num].sec   = his_err_tbl[s_cursor*HIS_ERR_SIZE+7];

                    DispInfo_Clock(ROW1,  0, ClockTmp[hmi_t_num], MON|DAY, 0, disp_buf);
                    DispInfo_Clock(ROW1, 48, ClockTmp[hmi_t_num], HHMMSS, 0, disp_buf);
                    disp_value_U08(s_cursor+1, ROW1, 128-16, DNS_ZERO|DNS_DIGIT2|DCS_REVERSE, disp_buf);
                }
                else            /* 无历史故障 */
                {
                    disp_string(dm603_face_menu_func_his_err_00, ROW1, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);
                }
                /* 菜单栏 */

                dm603_common_disp_menu(BMP_FN, dm603_face_return, \
                                       s_max_err ? BMP_OK : BMP_NU, dm603_face_menu_func_his_err_ent, disp_buf);
            }
            else
            {
                /* 提示栏 */
                disp_string(dm603_face_query_his_err_01, ROW1, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);
                disp_string(dm603_face_query_his_err_02, ROW2, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);

                /* 菜单栏 */
                dm603_common_disp_menu(BMP_FN, dm603_face_query_ret, BMP_OK, dm603_face_ok, disp_buf);
            }
            break;

        default:
            break;
    }
}

/*******************************************************************************
 函 数: dm603_face_invt_para()
 功 能: 变频参数界面
 参 数: disp_buf--;
 返 回:
 其 它: 空
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
    #define IPA_INIT -1     /* 初始值-1，表示未读 */

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
                    case DM603_KEY_FUNC:    /* 返回 */
                        dm603_pop_face_status();
                        return;
//                        break;

                    case DM603_KEY_OK:    /* 下一个 */
                        cursor++;
                        if (cursor > IPA_NULL) cursor = 0;
                        break;

                    case DM603_KEY_DOWN: /* 减 */
                        if (cursor != IPA_NULL)
                        {
                            *(ipa_set[cursor].pdata) = key_continuous_sub(*(ipa_set[cursor].pdata), ipa_set[cursor].max, ipa_set[cursor].min, CAL_CYCLE);
                            if (fun_pv_in(cursor, IPA_RD, IPA_WR))
                            {
                                cursor = IPA_NULL;
                            }
                        }
                        break;

                    case DM603_KEY_UP: /* 加 */
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
                /* 防止屏保 */
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

                /* 运行提示 */
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
 函 数: dm603_face_invt_except_info()
 功 能: 变频异常信息查询
 参 数: disp_buf--;
 返 回:
 其 它: 空
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
                    case DM603_KEY_FUNC:    /* 返回 */
                        dm603_pop_face_status();
                        return;
//                        break;

                    case DM603_KEY_DOWN: /* 下翻 */
                        PunpDispClearScreen();
                        cursor++;
                        if (cursor == INFO_MAX) cursor = 0;
                        break;

                    case DM603_KEY_UP: /* 上翻 */
                        PunpDispClearScreen();
                        cursor--;
                        if (cursor == _MAX_(cursor)) cursor = INFO_MAX-1;
                        break;

                    case DM603_KEY_OK_3S:   /* 清空 */
                        DataAccess_InvtExceptInfo(DATA_INIT, _MAX_(U08));
                        break;
                }
            }
            /* break; */

        case DM603_MENU_PAINT :
            /* 异常信息显示 */
            {
                U08 *info_str[INFO_MAX] = {"200", "004", "007", "008",
                                           "E02", "E03", "E04", "E12",
                                           "E19", "E20", "E31", "EX1",
                                           "EX2", "EX3", "EX4", "EX5",
                                           };
                InvtEXCEPT *pInfo = info_invt_except;
                /* ROW1 */
                disp_string_ptr8(info_str[cursor], ROW1, 0, DCS_NORMAL|DCS_REVERSE, disp_buf);
                pInfo[cursor].info = MIN(pInfo[cursor].info, 9999); /* 只够显示4位 */
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
 * 函数 : dm603_face_menu_func_defrost()
 * 功能 : 手动除霜
 *
 * 参数 : --
 * 返回 : --
 *
 * 备注 : --
 */
static void dm603_face_menu_func_defrost(U08 *disp_buf)
{
    U08 i;
    U16 pmsg[MAX_HMI];
    static U08 s_pcb[MAX_HMI];  /* 模块号 */

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
                /* 返回主界面 */
                if (Main_Key[hmi_t_num] == DM603_KEY_FUNC)
                {
                        dm603_pop_face_status();    /* 恢复上层界面状态 */
                        return;
                }

                /* 强制除霜 */
                if (Main_Key[hmi_t_num] == DM603_KEY_OK)
                {
                    dm603_common_disp_flash();
                    DM603_CMD_DEFROST(s_pcb[hmi_t_num]);
                }

                /* 向上或向下切换模块 */
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
            /* 提示信息 */
            disp_string(dm603_face_menu_func_defrost_info, ROW1, 0, DCS_NORMAL, disp_buf);
            disp_bmp(BMP_UP, ROW1, 16, DCS_NORMAL, disp_buf);
            disp_bmp(BMP_DN, ROW1, 16*2, DCS_NORMAL, disp_buf);

            /* 模块信息 */
            disp_value_U08(s_pcb[hmi_t_num], ROW2, 0, DNS_ZERO | DNS_DIGIT2, disp_buf);
            disp_string(dm603_face_menu_func_defrost_unit, ROW2, 2*8, DCS_NORMAL, disp_buf);

            /* 除霜信息 */
            pmsg[hmi_t_num] = DNULL;
            if (flash_500ms) dm603_common_disp_flash();
            for (i = 0; i < m_val_comp_num(s_pcb[hmi_t_num]); i++)
            {
                if (M_CM_GetStatus(s_pcb[hmi_t_num], i, COMP_FORCE_DEFROST)) pmsg[hmi_t_num] = dm603_face_menu_func_defrost_yes;
                if (M_CM_GetStatus(s_pcb[hmi_t_num], i, COMP_DEFROST))        pmsg[hmi_t_num] = dm603_face_menu_func_defrost_now;
            }
            if (pmsg[hmi_t_num] == DNULL)
            {
                /* 模块运行指示 */
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

            /* 箭头 */
            dm603_common_disp_arrow(s_pcb[hmi_t_num], 0, val_pcb_num, disp_buf);

            /* 菜单栏 */
            dm603_common_disp_menu(BMP_FN, dm603_face_return, \
                                   BMP_OK, dm603_face_menu_func_defrost_ent, disp_buf);
            break;
    }
}

/**
 * 函数 : dm603_face_cloud_tuya()
 * 功能 : 涂鸦WiFi屏查询
 *
 * 参数 : --
 * 返回 : --
 *
 * 备注 : --
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
                /* 返回主界面 */
                if (Main_Key[hmi_t_num] == DM603_KEY_FUNC)
                {
                    dm603_pop_face_status();    /* 恢复上层界面状态 */
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
                    /* 菜单栏 */
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
                    /* 菜单栏 */
                    dm603_common_disp_menu( BMP_FN, dm603_face_return, \
                                            BMP_NU, NULL, disp_buf);
                    break;
            }

            break;
    }
}

/*******************************************************************************
 函 数: dm603_face_menu_func_user_mode()
 功 能: 模式设置界面
 参 数:
 返 回: --
 其 它: --
*******************************************************************************/

static void dm603_face_menu_func_user_mode(U08 *disp_buf)
{
     PARAMITEM *ParamGroup;         // 参数组
     ParamGroup = PG_GetAddr_ParamPri(0);

    if(dm603_face_para_SetValue(ParamGroup,para_small[0], 0, 1, disp_buf)==PARA_OPT_M)  // 设置第0组第0项参数
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
     PARAMITEM *ParamGroup;         // 参数组
     ParamGroup = PG_GetAddr_ParamPri(0);

    if(dm603_face_para_SetValue(ParamGroup,para_small[0], 0, 4, disp_buf)==PARA_OPT_M)  // 设置第0组第0项参数
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
        /* 箭头 */
        dm603_common_disp_arrow(dm603_item[hmi_t_num], 0, 4, disp_buf);
    }
}

/**
 * 函数 : dm603_face_pass_clr()
 * 功能 : 清空密码值
 *
 * 参数 : @pass_buf 密码输入寄存器
 * 返回 : --
 *
 * 备注 : ①全部清空为0
 */
static void dm603_face_pass_clr(U08 *pass_buf)
{
    U08 i;

    /* 清空为0 */
    for (i = 0; i < MAX_PASS_BIT; i++)
    {
        *pass_buf++ = 0;
    }
}

/**
 * 函数 : dm603_face_pass_cmp()
 * 功能 : 密码比较
 *
 * 参数 : @new_pass 输入密码
 *        @old_pass 比较密码
 * 返回 : TRUE 密码相同
 *        FALSE 密码不同
 *
 * 备注 : --
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
 * 函数 : dm603_face_pass_chg()
 * 功能 : 更改密码
 *
 * 参数 : @pass 密码更改值
 *        @opt 向上或向下更改选项
 * 返回 : --
 *
 * 备注 : --
 */
static void dm603_face_pass_chg(U08 *pass, U08 opt)
{
    U08 val = 0;

    val = *pass;

    if (val == 0) /* 空密码值 */
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
 * 函数 : dm603_face_pass_disp()
 * 功能 : 密码显示
 *
 * 参数 : @cursor 密码更改光标
 *        @pass 密码更改值
 *        @row 密码显示行
 * 返回 : --
 *
 * 备注 : --
 */
static void dm603_face_pass_disp(U08 cursor, U08 pass, U08 row, U08 *disp_buf)
{
    U08 tmp;
    U08 col;
    const U08 *msg_star = "*******";

    col = (4+cursor) * 8;

    /* 空密码显示 '-', 其它显示对应的密码值 */
    if (pass == 0)
    {
        disp_string_ptr8("-", row, col, DCS_REVERSE | DCS_FLASH, disp_buf);
    }
    else
    {
        disp_value_U08(pass-'0', row, col, DCS_REVERSE | DNS_DIGIT1 | DCS_FLASH, disp_buf);
    }

    /* 显示 '*' 提示符 */
    tmp = 7 - cursor;
    col = 4 * 8;
    disp_string_ptr8((const U08 *)(&msg_star[tmp]), row, col, DCS_NORMAL, disp_buf);

    tmp = 7 - (7-cursor);
    col = (4+cursor+1) * 8;
    disp_string_ptr8((const U08 *)(&msg_star[tmp]), row, col, DCS_NORMAL, disp_buf);
}
/**
 * 函数 : dm603_face_code_disp()
 * 功能 : 机组编码显示
 *
 * 参数 : @cursor 密码更改光标
 *        @pass 密码更改值
 *        @row 密码显示行
 * 返回 : --
 *
 * 备注 : --
 */
//static void dm603_face_code_disp(U08 cursor, U08 row, U08 *disp_buf)
//{
//  U08 col;
//  col = cursor * 8;
//  disp_string_ptr8(dm603_code_buf, row, 0, DCS_NORMAL| DCS_FLASH , disp_buf);
//  /* 修改项反白显示 */
//  if(cursor == 15)    disp_string_ptr8(&dm603_code_buf[cursor], row, col, DCS_REVERSE| DCS_FLASH , disp_buf);
//  else                disp_value_U08(dm603_code_buf[cursor]-'0', row, col, DCS_REVERSE  | DNS_DIGIT1, disp_buf);
//}


/*******************************************************************************
 函 数: dm603_face_pass()
 功 能: 密码输入界面
 参 数: U16 PassValid: 当前允许的密码权限
 返 回: --
 其 它: 在有PASS_SUPER权限的密码界面，长按开关键可获取并显示验证码（用于计算动态密码）
*******************************************************************************/
static U08 dm603_face_pass(U16 PassValid, U08 *disp_buf)
{

    static U08 dm603_pass_input;        /* TRUE 密码输入界面 / FALSE 密码重试界面 */
    static U08 dm603_pass_input_bak;
    static U08 dm603_pass_cursor;       /* 密码输入的移位光标 */

    static U08 PassOK = 0xFF;   // 密码通过级别 0xff表示没通过

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
            /* 密码输入 */
            if(key_delay_time[hmi_t_num]>=KEY_DELAY)
            {
                if (dm603_pass_input)
                {
                    if (Main_Key[hmi_t_num] == DM603_KEY_OK)            /* 密码确认 */
                    {
                        // 按以下顺序验证：用户、工程、维修、厂家
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
                            if(ValidCode!=0)    // 验证码不为0才计算并使用动态密码
                            {
                                Password_super_cal(ValidCode);   // 计算超级密码
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
                            dm603_pass_input = FALSE;   /* 密码错误 */
                        }
                        else
                        {
                            dm603_get_nextface_status();    /* 跳转到下一界面 */

                            return PassOK;
                        }
                    }
                    /* 密码移位 */
                    if (Main_Key[hmi_t_num] == DM603_KEY_FUNC)
                    {
                        dm603_common_disp_flash();
                        if (++dm603_pass_cursor >= MAX_PASS_BIT) dm603_pass_cursor = 0;
                    }

                    /* 密码选择数字 */
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

                    if (Main_Key[hmi_t_num] == DM603_KEY_ONOFF_3S && PassValid&(0x0001<<PASS_SUPER))    // 有动态密码权限的界面才可获取和显示验证码
                    {
                        ValidCode = USER_GetTime(C_5MS);
                    }
                }
                else    /* 密码重试 */
                {
                    if (Main_Key[hmi_t_num] == DM603_KEY_OK)
                    {
                        dm603_menu[hmi_t_num] = DM603_MENU_CREATE;          /* 密码重试 */
                    }
                    else if (Main_Key[hmi_t_num] == DM603_KEY_FUNC)
                    {
                        dm603_pop_face_status();    /* 返回进入前的界面状态 */
                        return PassOK;
                    }
                }
            }
            /* 切换界面 */
            if (dm603_pass_input != dm603_pass_input_bak)
            {
                dm603_pass_input_bak = dm603_pass_input;
                dm603_common_disp_clr_line(ROW1, ROW2, RNUL, RNUL, disp_buf);
                dm603_common_disp_flash();
            }
            /* break; */

        case DM603_MENU_PAINT :
            if (dm603_pass_input) /* 密码输入 */
            {
                /* 提示信息 */
                disp_string(dm603_face_pass_input, ROW1, 0, DCS_NORMAL, disp_buf);

                /* 显示密码 */
                dm603_face_pass_disp(dm603_pass_cursor, \
                                     dm603_pass_buf[dm603_pass_cursor], \
                                     ROW2, disp_buf);

                /* 菜单栏 */
                dm603_common_disp_menu(BMP_FN, dm603_face_pass_input_ret, \
                                       BMP_OK, dm603_face_pass_input_ent, disp_buf);
            }
            else        /* 密码重试 */
            {
                /* 提示信息 */
                disp_string(dm603_face_pass_input_error_01, ROW1, 0, DCS_NORMAL | DTS_CENTER, disp_buf);
                disp_string(dm603_face_pass_input_error_02, ROW2, 0, DCS_NORMAL | DTS_CENTER, disp_buf);

                /* 菜单栏 */
                dm603_common_disp_menu(BMP_FN, dm603_face_return, \
                                       BMP_OK, dm603_face_pass_input_error_ent, disp_buf);
            }

            if(ValidCode!=0 && PassValid&(0x0001<<PASS_SUPER))  // 有动态密码权限的界面才可获取和显示验证码
            {
                disp_value_U16(ValidCode, ROW3, 128, DTS_RIGHT|DNS_DIGIT5, disp_buf);
            }

            break;
    }

    return PassOK;
}

/********************************************
函数: Face_ParaGroup_hide
功能: 判断哪些参数组可以显示
参数: ParamGroup: PARAMITEM数组地址;  ItemAll: 全部组数; new_tbl: 可显示的菜单数组;                 PassGrade: 当前密码级别
返回值: TRUE 允许显示 / FALSE 不允许显示
*********************************************/
static U08 Face_ParaGroup_hide(const PARAMITEM *ParamGroup, U08 *Item_tbl, U08 Tbl_size, U16 PassGrade)
{
    U08 n, cur;
    PARAMITEM *pp1;
    U08 GroupNum = 0;

    GroupNum = PG_GetGroupNum(ParamGroup);  // 获取组数

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
        else if (PassGrade == PASS_LIMIT)   // 使用期限特殊处理，暂使用D_USER级别
        {
            if (!PG_Is_Show(pp1, D_USER))   continue;
        }
        else
        {
            continue;
        }

        if(cur<Tbl_size) // 防止超界
        {
            Item_tbl[cur++] = n;
        }
    }

    return cur;
}

/**
 * 函数 : dm603_face_para_set_chg_m()
 * 功能 : 调整参数项
 *
 * 参数 : @n 组
 *        @new_tbl 数据新位置
 *        @grade 级别
 * 返回 : 调整后的可显示项个数
 *
 * 备注 : ①按级别重新整理参数组数据
 */




/*******************************************************************************
 函 数: Face_para_item_count()
 功 能: 计算可显示的参数项
 参 数: new_tbl: 存放可显示的项序号的数组；tbl_size：new_tbl指向的数组的大小； grade:当前密码级别
 返 回: --
 其 它: --
*******************************************************************************/
static U08 Face_para_item_count(PARAMITEM* ParamGroup, U08 *new_tbl, U08 tbl_size, U16 grade)
{
    U08 m;
    U08 cur;

    cur = 0;

    for (m = 0; m < ParamGroup->items; m++)
    {
        /* 该项参数是否隐藏 */
        if (para_is_hide(ParamGroup, m, grade)) continue;

        if (cur >= tbl_size)  break; // 参数数量超过显示菜单允许的数量(why:防止new_tbl使用超界)

        new_tbl[cur++] = m;

    }


    return cur;
}

#if 0
/**
 * 函数 : dm603_face_para_set_bit_count()
 * 功能 : 计算位参数项个数
 *
 * 参数 : @n 组
 *        @m 项
 * 返回 : 该位参数项可显示的位数
 *
 * 备注 : ①该值为对应的个数，使用时需要减一
 */
static U08 dm603_face_para_set_bit_count(U08 n, U08 m)
{
    U08 total = 16;

//  /* 模块禁用 */
//  if (n == 1 && m == 0)
//  {
//      total = val_pcb_num;
//  }

    return total - 1;
}
#endif

/**
 * 函数 : dm603_face_para_set_disp_v()
 * 功能 : 参数项或值显示
 *
 * 参数 : @opt 项或值选择
 *        @val 当前更改值
 *        @num 序号
 *        @sign 参数属性
 *        @para 参数信息
 *
 * 返回 : --
 *
 * 备注 : ①只显示参数项或值内容
 *        ②仅用于显示整数类型参数
 */
static void dm603_face_para_set_disp_v(U08 opt, I16 val, U08 num, U16 sign, const SMALL *para, U08 *disp_buf)
{
    U08 col = 0;
    U08 row = ROW1;

    U08 is_num = 0;
    U16 attrib = 0;

    /* 第一行 提示信息 */
    row = ROW1;
    col = 0;
    disp_value_U08(num, row, col, DNS_DIGIT2 | DNS_ZERO, disp_buf);
    col = 2*8;
    disp_string(para->msg, row, col, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);

    /* 第二行 项或值 */
    row = ROW2;
    //col = 0;
    col = COL_END - COL_ASC;
    attrib = (opt == PARA_OPT_M) ? DCS_NORMAL : DCS_REVERSE;    /* 项或值 */
    is_num = (para->pmsg == PNULL) ? 1 : 0; /* 数字或字符 */

    /* 数字 */
    if (is_num) dm603_common_disp_val(val, row, col, sign, attrib, disp_buf);
    //else      disp_string(para->pmsg[val], row, col, attrib | DTS_CLR_TRAIL, disp_buf);
    //else      disp_string(para->pmsg[val], row, col, attrib | DTS_RIGHT, disp_buf);
    else        disp_string(para->pmsg[val], row, col, attrib | DTS_RIGHT | DTS_CLR_HEAD, disp_buf);
}

/**
 * 函数 : dm603_face_para_set_disp_b()
 * 功能 : 参数位显示
 *
 * 参数 : @opt 项或值选择
 *        @val 当前更改值
 *        @num 序号
 *        @b   位号
 *        @msg 位信息
 *        @para 参数信息
 *
 * 返回 : --
 *
 * 备注 : ①只显示参数位项内容
 */
//static void dm603_face_para_set_disp_b(U08 opt, I16 val, U08 num, U08 b, const U16 **msg, SMALL *para, U08 *disp_buf)
//{
//  U08 col = 0;
//  U08 row = ROW1;
//
//  U16 attrib = 0;
//
//  /* 第一行 项提示信息 */
//  row = ROW1;
//  col = 0;
//  disp_value_U08(num, row, col, DNS_DIGIT2 | DNS_ZERO, disp_buf);
//  col = 2*8;
//  disp_string(para->msg, row, col, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);
//
//  /* 第二行 值提示信息 */
//  row = ROW2;
//  col = 0;
//  disp_value_U08(b, row, col, DNS_DIGIT2 | DNS_ZERO, disp_buf);
//  col = 2*8;
//  disp_string(para->pmsg[b], row, col, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);
//
//  /* 第三行 位提示信息 */
//  row = ROW3;
//  col = COL_END - COL_ASC;
//  attrib = (opt == PARA_OPT_M) ? DCS_NORMAL : DCS_REVERSE;    /* 项或值 */
//  //disp_string(msg[((U08)GET_BIT(val, b))], row, col, attrib | DTS_CLR_TRAIL, disp_buf);
//  disp_string(msg[((U08)GET_BIT(val, b))], row, col, attrib | DTS_RIGHT, disp_buf);
//}


/*******************************************************************************
 函 数: dm603_face_para_SetValue()
 功 能: 计算可显示的参数项
 参 数: SMALL *para_msg:参数项名称数组, U08 Item_min：起始项, U08 Item_num：指定的显示个数
 返 回: value_opt 当前是否在设置状态
 其 它: 注意Item_num大小(为0时根据属性计算可显示的项，非0时可显示项为从Item_min开始计算的Item_num个参数)
*******************************************************************************/
static U08 dm603_face_para_SetValue(PARAMITEM *ParamGroup, const SMALL *para_msg, U08 Item_min, U08 Item_num,U08 *disp_buf)
{
    U08 i;
    U08 value_opt = PARA_OPT_M; /* 当前显示参数项还是参数值选择 */


    static I16 value;       // 值
    static U08 min_para_m;  // 最小项
    static U08 max_para_m;  // 最大项

    if (Item_num > sizeof(para_m_tbl))   // 参数数量超过显示菜单允许的数量(why:防止para_m_tbl使用超界)
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
            if(Item_num==0) // 根据属性计算范围
            {
                max_para_m = Face_para_item_count(ParamGroup, para_m_tbl, sizeof(para_m_tbl), dm603_pass_grade[hmi_t_num]);//NOTE-CXW 待测2017.4.11
            }
            else    // 指定范围
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
            /* 切换子界面 */
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
                    case PARA_SELECT :  /* 参数项 */
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
//                              break;  /* 跳出切换界面 */

                            case DM603_KEY_UP:      /* 切换到上一参数项 */
                            case DM603_KEY_DOWN:    /* 切换到下一项 */
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

                    case PARA_SETTING : /* 参数值项 */
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

                            case DM603_KEY_UP:      /* 参数加 */
                                para_add(ParamGroup, para_m_tbl[dm603_item[hmi_t_num]], &value, 0);
                                break;

                            case DM603_KEY_DOWN:    /* 参数减 */
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
                case PARA_SELECT :  /* 参数项 */
                    value_opt = PARA_OPT_M;
                    /* 菜单栏 */
                    dm603_common_disp_menu(BMP_FN, dm603_face_return, \
                                           BMP_OK, dm603_face_para_set_m_ent, disp_buf);
//                  break;

                case PARA_SETTING : /* 参数值 */
                    if (FaceStatus[hmi_t_num] == PARA_SETTING)
                    {
                        value_opt = PARA_OPT_V;
                        /* 菜单栏 */
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
 * 函数：dm603_face_pass_modify()
 * 功能 : 密码修改
 *
 * 参数 : U08 Pass_toModify 需要修改的密码级别
 * 返回 : --
 *
 * 备注 : ①更改对应参数级别密码
 */

/*******************************************************************************
 函 数: dm603_face_pass_modify()
 功 能: 密码修改界面，根据指定的密码级别修改密码
 参 数: U08 Pass_toModify 需要修改的密码级别
 返 回: --
 其 它: --
*******************************************************************************/
static void dm603_face_pass_modify(U08 Pass_toModify,U08 *disp_buf)
{
    U08 i = 0;

    static U08 *pass_buf_m;         /* 密码指针 */
    static U08 pass_ok;             /* 密码错误 */
    static U08 cursor;
    U08 menu_opt = PASS_CHG_UP;

    const U16 msg_pass[] =
    {
        dm603_face_para_pass_m_old, /* {请输入旧密码:}   */
        dm603_face_para_pass_m_new, /* {请输入新密码:}   */
        dm603_face_para_pass_m_agn, /* {再次输入新密码:} */
    };

    switch (dm603_menu[hmi_t_num])
    {
        case DM603_MENU_CREATE :
            dm603_menu[hmi_t_num] = DM603_MENU_PROC;
            key_delay_time[hmi_t_num]=0;
            dm603_item[hmi_t_num] = 0;
            cursor = 0;

            /* 密码清空 */
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
                            case 0 :    /* 输入旧密码 */
                                cursor = 0;
                                pass_ok = dm603_face_pass_cmp(pass_buf_m, (U08 *)password[Pass_toModify]);

                                if ((Pass_toModify == PASS_USER) && (pass_ok == 0)) // 用户密码可由厂家密码更改和维修者密码
                                {
                                    pass_ok = dm603_face_pass_cmp(pass_buf_m, (U08 *)password[PASS_SER]);
                                    pass_ok |= dm603_face_pass_cmp(pass_buf_m, (U08 *)password[PASS_FAC]);

                                }
                                else if ((Pass_toModify == PASS_FAC) && (pass_ok == 0)) // 厂家密码可由动态密码更改
                                {
                                    pass_ok = dm603_face_pass_cmp(pass_buf_m, (U08 *)password[PASS_SUPER]);
                                }
                                else if ((Pass_toModify == PASS_SER) && (pass_ok == 0)) // 维护密码可由厂家密码更改
                                {
                                    pass_ok = dm603_face_pass_cmp(pass_buf_m, (U08 *)password[PASS_FAC]);
                                }



                                if (pass_ok)    dm603_item[hmi_t_num] = 1;
                                else            dm603_item[hmi_t_num] = 3;
                                pass_buf_m = dm603_pass_new;
                                break;

                            case 1 :    /* 输入新密码 */
                                cursor = 0;
                                dm603_item[hmi_t_num] = 2;
                                pass_buf_m = dm603_pass_bak;
                                break;

                            case 2 :    /* 再次输入新密码 */
                                cursor = 0;
                                pass_ok = dm603_face_pass_cmp(pass_buf_m, dm603_pass_new);
                                dm603_item[hmi_t_num] = 4;

                                /* 保存新密码*/
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

                            case 3 :    /* 旧密码错误 */
                            case 4 :    /* 新密码错误 */
                                if (!pass_ok)   dm603_menu[hmi_t_num] = DM603_MENU_CREATE;  /* 重新输入 */
                                break;
                        }
                        break;

                    case DM603_KEY_FUNC: /* 返回主界面或移位 */
                        if (dm603_item[hmi_t_num] < 3)
                        {
                            if (++cursor >= MAX_PASS_BIT)   cursor = 0;
                        }
                        else
                        {
                            dm603_pop_face_status();    /* 恢复上层界面状态 */
                            return;
                        }
                        break;

                    /* 密码更改 */
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

            if (dm603_item[hmi_t_num] < 3)      /* 显示密码 */
            {
                disp_string(msg_pass[dm603_item[hmi_t_num]], ROW1, 0, DCS_NORMAL, disp_buf);
                /* 显示密码 */
                dm603_face_pass_disp(cursor, pass_buf_m[cursor], ROW2, disp_buf);
                /* 菜单栏 */
                dm603_common_disp_menu(BMP_FN, dm603_face_para_pass_m_ret, \
                                       BMP_OK, dm603_face_para_pass_m_ent, disp_buf);
            }
            else                        /* 显示提示信息 */
            {
                if (dm603_item[hmi_t_num] == 3) /* 旧密码输入错误 */
                {
                    disp_string(dm603_face_para_pass_m_err_old, ROW1, 0, DCS_NORMAL | DTS_CENTER, disp_buf);
                    disp_string(dm603_face_para_pass_m_err, ROW2, 0, DCS_NORMAL | DTS_CENTER, disp_buf);

                    /* 菜单栏 */
                    dm603_common_disp_menu(BMP_FN, dm603_face_return, \
                                           BMP_OK, dm603_face_para_pass_m_err_ent, disp_buf);
                }
                else    /* 新密码输入 */
                {
                    if (pass_ok)    /* 新密码正确 */
                    {
                        disp_string(dm603_face_para_pass_m_yes_agn, ROW1, 0, DCS_NORMAL | DTS_CENTER, disp_buf);
                        /* 菜单栏 */
                        dm603_common_disp_menu(BMP_FN, dm603_face_return, \
                                               BMP_NU, NULL, disp_buf);
                    }
                    else                    /* 新密码错误 */
                    {
                        disp_string(dm603_face_para_pass_m_err_agn, ROW1, 0, DCS_NORMAL | DTS_CENTER, disp_buf);
                        disp_string(dm603_face_para_pass_m_err, ROW2, 0, DCS_NORMAL | DTS_CENTER, disp_buf);
                        /* 菜单栏 */
                        dm603_common_disp_menu(BMP_FN, dm603_face_return, \
                                               BMP_OK, dm603_face_para_pass_m_err_ent, disp_buf);
                    }
                }
            }
            break;
    }
}



/*******************************************************************************
 函 数: dm603_face_para_initP()
 功 能: 初始化参数界面
 参 数: --
 返 回: --
 其 它: --
*******************************************************************************/

static void dm603_face_para_initP(U08 *disp_buf)
{
    static BOOL fg_init_act = FALSE;    /* 是否已开始初始化 */

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
                            DM603_CMD_PARA_INIT;        /* 厂家参数初始化 */
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

            if (fg_init_act)        /* 已进入初始化 */
            {
                switch (GET_INIT_PARA())
                {
                    case ACTION_IDLE  : /* break; */
                    case ACTION_START :
                    case ACTION_ING :
                        disp_string(dm603_face_para_init_now, ROW2, 0, \
                                    DTS_CENTER | DTS_CLR_HEAD | DTS_CLR_TRAIL | DCS_FLASH, disp_buf);
                        break;

                    case ACTION_SUCC : /* 成功 */
                        disp_string(dm603_face_para_init_yes, ROW2, 0, \
                                    DTS_CENTER | DTS_CLR_HEAD | DTS_CLR_TRAIL | DCS_FLASH, disp_buf);
                        break;

                    case ACTION_FAIL : /* 失败 */
                        disp_string(dm603_face_para_init_err, ROW2, 0, \
                                    DTS_CENTER | DTS_CLR_HEAD | DTS_CLR_TRAIL | DCS_FLASH, disp_buf);
                        break;
                }

                /* 菜单栏 */
                dm603_common_disp_menu(BMP_FN, dm603_face_return,   BMP_NU, NULL,  disp_buf);
            }
            else    /* 询问是否初始化 */
            {
                disp_string(dm603_face_query_init, ROW1, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);
                if(!(STEP_IS_IDLE || STEP_IS_ALARM))
                {
                    disp_string(dm603_face_operation_tip, ROW2, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);
                }

                /* 菜单栏 */
                dm603_common_disp_menu(BMP_FN, dm603_face_query_ret,    BMP_OK, dm603_face_ok,  disp_buf);
            }
            break;

        default:
            break;
    }
}

/*******************************************************************************
 函 数: dm603_face_para_initC()
 功 能: 初始化压机时间界面
 参 数: --
 返 回: --
 其 它: --
*******************************************************************************/
static void dm603_face_para_initC(U08 *disp_buf)
{
    static BOOL fg_init_act = FALSE;    /* 是否已开始初始化 */

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
                            DM603_CMD_RUNTIME_INIT;     /* 压机累计运行时间初始化 */
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

            if (fg_init_act)        /* 已进入初始化 */
            {
                disp_string(dm603_face_para_init_yes, ROW2, 0, \
                                DTS_CENTER | DTS_CLR_HEAD | DTS_CLR_TRAIL | DCS_FLASH, disp_buf);

                /* 菜单栏 */
                dm603_common_disp_menu(BMP_FN, dm603_face_return,   BMP_NU, NULL,  disp_buf);
            }
            else    /* 询问是否初始化 */
            {
                disp_string(dm603_face_query_init, ROW1, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);
                if(!(STEP_IS_IDLE || STEP_IS_ALARM))
                {
                    disp_string(dm603_face_operation_tip, ROW2, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);
                }

                /* 菜单栏 */
                dm603_common_disp_menu(BMP_FN, dm603_face_query_ret,    BMP_OK, dm603_face_ok,  disp_buf);
            }
            break;

        default:
            break;
    }
}

/*******************************************************************************
 函 数: dm603_face_sysruntime_init()
 功 能: 初始化系统累计运行时间界面
 参 数: --
 返 回: --
 其 它: --
*******************************************************************************/

static void dm603_face_sysruntime_init(U08 *disp_buf)
{
    static BOOL fg_init_act = FALSE;    /* 是否已开始初始化 */

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
                            DM603_CMD_SYSTIME_INIT;     /* 系统累计运行时间初始化 */
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

            if (fg_init_act)        /* 已进入初始化 */
            {
                disp_string(dm603_face_para_init_yes, ROW2, 0, \
                                DTS_CENTER | DTS_CLR_HEAD | DTS_CLR_TRAIL | DCS_FLASH, disp_buf);

                /* 菜单栏 */
                dm603_common_disp_menu(BMP_FN, dm603_face_return,   BMP_NU, NULL,  disp_buf);
            }
            else    /* 询问是否初始化 */
            {
                disp_string(dm603_face_query_init, ROW1, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);
                if(!(STEP_IS_IDLE || STEP_IS_ALARM))
                {
                    disp_string(dm603_face_operation_tip, ROW2, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);
                }

                /* 菜单栏 */
                dm603_common_disp_menu(BMP_FN, dm603_face_query_ret,    BMP_OK, dm603_face_ok,  disp_buf);
            }
            break;

        default:
            break;
    }
}

/*******************************************************************************
 函 数: dm603_face_limit_init()
 功 能: 初始化系统维护参数界面
 参 数: --
 返 回: --
 其 它: --
*******************************************************************************/
static void dm603_face_limit_init(U08 *disp_buf)
{
    static BOOL fg_init_act = FALSE;    /* 是否已开始初始化 */

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
                            DM603_CMD_LIMIT_INIT;       /* 厂家参数初始化 */
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

            if (fg_init_act)        /* 已进入初始化 */
            {
                switch (GET_INIT_PARA())
                {
                    case ACTION_IDLE  : /* break; */
                    case ACTION_START :
                    case ACTION_ING :
                        disp_string(dm603_face_para_init_now, ROW2, 0, \
                                    DTS_CENTER | DTS_CLR_HEAD | DTS_CLR_TRAIL | DCS_FLASH, disp_buf);
                        break;

                    case ACTION_SUCC : /* 成功 */
                        disp_string(dm603_face_para_init_yes, ROW2, 0, \
                                    DTS_CENTER | DTS_CLR_HEAD | DTS_CLR_TRAIL | DCS_FLASH, disp_buf);
                        break;

                    case ACTION_FAIL : /* 失败 */
                        disp_string(dm603_face_para_init_err, ROW2, 0, \
                                    DTS_CENTER | DTS_CLR_HEAD | DTS_CLR_TRAIL | DCS_FLASH, disp_buf);
                        break;
                }

                /* 菜单栏 */
                dm603_common_disp_menu(BMP_FN, dm603_face_return,   BMP_NU, NULL,  disp_buf);
            }
            else    /* 询问是否初始化 */
            {
                disp_string(dm603_face_query_init, ROW1, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);

                /* 菜单栏 */
                dm603_common_disp_menu(BMP_FN, dm603_face_query_ret,    BMP_OK, dm603_face_ok,  disp_buf);
            }
            break;

        default:
            break;
    }
}

/*******************************************************************************
 函 数: dm603_face_para_initInvt()
 功 能: 初始化变频器界面
 参 数: --
 返 回: --
 其 它: --
*******************************************************************************/
static void dm603_face_para_initInvt(U08 *disp_buf)
{
    static BOOL fg_init_act = FALSE;    /* 是否已开始初始化 */

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
                            DM603_CMD_INVT_INIT;        /* 变频器初始化 */
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

            if (fg_init_act)        /* 已进入初始化 */
            {
                disp_string(dm603_face_para_init_cmd, ROW2, 0, \
                                DTS_CENTER | DTS_CLR_HEAD | DTS_CLR_TRAIL | DCS_FLASH, disp_buf);

                /* 菜单栏 */
                dm603_common_disp_menu(BMP_FN, dm603_face_return,   BMP_NU, NULL,  disp_buf);
            }
            else    /* 询问是否初始化 */
            {
                disp_string(dm603_face_query_init, ROW1, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);
                if(!(STEP_IS_IDLE || STEP_IS_ALARM))
                {
                    disp_string(dm603_face_operation_tip, ROW2, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);
                }

                /* 菜单栏 */
                dm603_common_disp_menu(BMP_FN, dm603_face_query_ret,    BMP_OK, dm603_face_ok,  disp_buf);
            }
            break;

        default:
            break;
    }
}

/*******************************************************************************
 函 数: dm603_face_enter_ability_test()
 功 能: 能力测试
 参 数: --
 返 回: --
 其 它: --
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

                /* 菜单栏 */
                dm603_common_disp_menu(BMP_FN, dm603_face_return,   BMP_NU, NULL,  disp_buf);
            }
            else    /* 询问 */
            {
                disp_string(dm603_face_query_enter_ability_test, ROW1, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);

                /* 菜单栏 */
                dm603_common_disp_menu(BMP_FN, dm603_face_query_ret,    BMP_OK, dm603_face_ok,  disp_buf);
            }
            break;

        default:
            break;
    }
}

#if 0
/*******************************************************************************
 函 数: dm603_face_enter_floor_dryup()
 功 能: 地暖烘干功能
 参 数: disp_buf--;
 返 回:
 其 它: 空
*******************************************************************************/
static void dm603_face_enter_floor_dryup(U08 *disp_buf)
{
    static BOOL fg_entered = FALSE; /* 是否已进入 */

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
                            && (val_air_type != HOT_WATER_ONLY)    // 存在制热模式
                            && (val_air_type != COOL_ONLY)
                        )
                        {
                            DM603_CMD_RNTER_FLOOR_DRYUP;        /* 发送命令 */
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

            if ((val_air_type == HOT_WATER_ONLY)   // 不存在制热模式
                || (val_air_type == COOL_ONLY)
            )
            {
                disp_string(dm603_face_enter_floor_dryup5, ROW1, 0, \
                                DTS_CENTER | DTS_CLR_HEAD | DTS_CLR_TRAIL | DCS_FLASH, disp_buf);
                /* 菜单栏 */
                dm603_common_disp_menu(BMP_FN, dm603_face_return,   BMP_NU, NULL,  disp_buf);
            }
            else if (fg_entered)        /* 已进入 */
            {
                disp_string(dm603_face_enter_floor_dryup3, ROW1, 0, \
                                DTS_CENTER | DTS_CLR_HEAD | DTS_CLR_TRAIL | DCS_FLASH, disp_buf);
                disp_string(dm603_face_enter_floor_dryup4, ROW2, 0, \
                                DTS_CENTER | DTS_CLR_HEAD | DTS_CLR_TRAIL | DCS_FLASH, disp_buf);
                /* 菜单栏 */
                dm603_common_disp_menu(BMP_FN, dm603_face_return,   BMP_NU, NULL,  disp_buf);
            }
            else    /* 询问 */
            {
                disp_string(dm603_face_enter_floor_dryup1, ROW1, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);
                disp_string(dm603_face_enter_floor_dryup2, ROW2, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);

                /* 菜单栏 */
                dm603_common_disp_menu(BMP_FN, dm603_face_query_ret,    BMP_OK, dm603_face_ok,  disp_buf);
            }
            break;

        default:
            break;
    }
}
#endif

/*******************************************************************************
 函 数: dm603_face_force_ibh()
 功 能: 强开内置电热
 参 数: disp_buf--;
 返 回:
 其 它: 空
*******************************************************************************/
static void dm603_face_force_ibh(U08 *disp_buf)
{
    static BOOL fg_entered = FALSE; /* 是否已进入 */

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
//                            && (STEP_IS_RUN)                        // 运行状态下
//                            && ((fg_hotw_need && HOTW_USE_IBH && !val_TBH_used)   // 热水强制开
//                                || (fg_heat_need && HEAT_USE_IBH))// 制热强制开
//                        )
//                        {
//                            DM603_CMD_FORCE_IBH;        /* 发送命令 */
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
                /* 菜单栏 */
                dm603_common_disp_menu(BMP_FN, dm603_face_return,   BMP_NU, NULL,  disp_buf);
            }
            else if (fg_entered)        /* 已进入 */
            {
                disp_string(dm603_face_force2, ROW1, 0, \
                                DTS_CENTER | DTS_CLR_HEAD | DTS_CLR_TRAIL | DCS_FLASH, disp_buf);
                /* 菜单栏 */
                dm603_common_disp_menu(BMP_FN, dm603_face_return,   BMP_NU, NULL,  disp_buf);
            }
            else    /* 询问 */
            {
                disp_string(dm603_face_force1, ROW1, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);

                /* 菜单栏 */
                dm603_common_disp_menu(BMP_FN, dm603_face_query_ret,    BMP_OK, dm603_face_ok,  disp_buf);
            }
            break;

        default:
            break;
    }
}

/*******************************************************************************
 函 数: dm603_face_force_tbh()
 功 能: 强制开水箱电加热
 参 数: disp_buf--;
 返 回:
 其 它: 空
*******************************************************************************/
static void dm603_face_force_tbh(U08 *disp_buf)
{
    static BOOL fg_entered = FALSE; /* 是否已进入 */

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
//                            && (STEP_IS_RUN)                        // 运行状态下
//                            && (fg_hotw_need && val_TBH_used)  // 热水强制开
//                        )
//                        {
//                            DM603_CMD_FORCE_TBH;        /* 发送命令 */
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
                /* 菜单栏 */
                dm603_common_disp_menu(BMP_FN, dm603_face_return,   BMP_NU, NULL,  disp_buf);
            }
            else if (fg_entered)        /* 已进入 */
            {
                disp_string(dm603_face_force2, ROW1, 0, \
                                DTS_CENTER | DTS_CLR_HEAD | DTS_CLR_TRAIL | DCS_FLASH, disp_buf);
                /* 菜单栏 */
                dm603_common_disp_menu(BMP_FN, dm603_face_return,   BMP_NU, NULL,  disp_buf);
            }
            else    /* 询问 */
            {
                disp_string(dm603_face_force1, ROW1, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);

                /* 菜单栏 */
                dm603_common_disp_menu(BMP_FN, dm603_face_query_ret,    BMP_OK, dm603_face_ok,  disp_buf);
            }
            break;

        default:
            break;
    }
}

#if 0
/*******************************************************************************
 函 数: dm603_face_force_hot_water()
 功 能: 强制制热水(快速制热水)
 参 数: disp_buf--;
 返 回:
 其 它: 空
*******************************************************************************/
static void dm603_face_force_hot_water(U08 *disp_buf)
{
    static BOOL fg_entered = FALSE; /* 是否已进入 */

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
                            DM603_CMD_FAST_HOTW;      /* 发送命令 */
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
                /* 菜单栏 */
                dm603_common_disp_menu(BMP_FN, dm603_face_return,   BMP_NU, NULL,  disp_buf);
            }
            else if (fg_entered)        /* 已进入 */
            {
                disp_string(dm603_face_force2, ROW1, 0, \
                                DTS_CENTER | DTS_CLR_HEAD | DTS_CLR_TRAIL | DCS_FLASH, disp_buf);
                /* 菜单栏 */
                dm603_common_disp_menu(BMP_FN, dm603_face_return,   BMP_NU, NULL,  disp_buf);
            }
            else    /* 询问 */
            {
                disp_string(dm603_face_force1, ROW1, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);

                /* 菜单栏 */
                dm603_common_disp_menu(BMP_FN, dm603_face_query_ret,    BMP_OK, dm603_face_ok,  disp_buf);
            }
            break;

        default:
            break;
    }
}

/*******************************************************************************
 函 数: dm603_face_holiday_mode_set()
 功 能: 假日模式
 参 数: disp_buf--;
 返 回:
 其 它: 空
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
                if (val_air_type == COOL_ONLY) // 单冷模式假日模式无效
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
                    // 假日模式:使用
                    disp_string(dm603_holiday_mode, ROW1, COL_BGN, DCS_NORMAL, disp_buf);
                    if (set_iteam == HOLIDAY_USED)
                    {
                        disp_string(used_or_unused[val], ROW1, COL_END-56, attribute|DCS_REVERSE, disp_buf);
                    }
                    else
                    {
                        disp_string(used_or_unused[dm603_holiday_tbl[HOLIDAY_USED]], ROW1, COL_END-56, DCS_NORMAL, disp_buf);
                    }

                    // 制热设定:  00 ℃
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

                    // 热水设定:  00 ℃
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
                    // 开始: 2022/01/01
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

                    // 结束: 2022/01/01
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
 函 数: dm603_face_error()
 功 能: 故障查询及复位界面
 参 数: --
 返 回: --
 其 它: --
*******************************************************************************/
static void dm603_face_error(U08 *disp_buf)
{
    static U08 dm603_error_pcb;
    static U08 dm603_error_id;
    static U08 dm603_error_id_bak;

    U16 err_tag = 0;

    /* 用于箭头显示 */
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

            /* 未查询到任何故障,返回主界面 */
            if ((dm603_error_pcb == F_NO_ERR) && (dm603_error_id == F_NO_ERR)|| !fg_error)
            {
                dm603_face[hmi_t_num] = FACE_MAIN;
                return;
            }
            if(key_delay_time[hmi_t_num]>=KEY_DELAY)
            {
                /* 返回主界面 */
                if ((Main_Key[hmi_t_num] == DM603_KEY_FUNC) )
                {
                    dm603_face[hmi_t_num] = FACE_MAIN;
                    return;
                }

                /* 复位 */
                if (Main_Key[hmi_t_num] == DM603_KEY_OK)
                {
                    if (fg_error) DM603_CMD_RESET;      /* 复位 */
                    dm603_menu[hmi_t_num] = DM603_MENU_CREATE;
                }

                /* 向上或向下查询故障 */
                if (Main_Key[hmi_t_num] == DM603_KEY_DOWN)
                {
                    dm603_common_disp_flash();

                    if (dm603_error_id_bak == arrow_err_max)    /* 已翻页到最大值 */
                    {
                        dm603_error_pcb = menu_error_find_next(FIND_ERR_PCB_NEXT, dm603_error_pcb, 0);
                        dm603_error_id  = menu_error_find_next(FIND_ERR_MIN, dm603_error_pcb, 0);

                    }
                    else
                    {
                        dm603_error_id  = menu_error_find_next(FIND_ERR_NEXT, dm603_error_pcb, dm603_error_id);
                    }
                    dm603_error_id_bak = dm603_error_id;
                    break;  // 不直接到DM603_MENU_PAINT，以免下一模块的最大最小故障号还未更新就判断是否显示箭头

                }
                if (Main_Key[hmi_t_num] == DM603_KEY_UP)
                {
                    dm603_common_disp_flash();

                    if (dm603_error_id_bak == arrow_err_min)    /* 已翻页到最小值 */
                    {
                        dm603_error_pcb = menu_error_find_next(FIND_ERR_PCB_PREV, dm603_error_pcb, 0);
                        dm603_error_id  = menu_error_find_next(FIND_ERR_MAX, dm603_error_pcb, 0);

                    }
                    else
                    {
                        dm603_error_id  = menu_error_find_next(FIND_ERR_PREV, dm603_error_pcb, dm603_error_id);
                    }
                    dm603_error_id_bak = dm603_error_id;
                    break;  // 不直接到DM603_MENU_PAINT，以免下一模块的最大最小故障号还未更新就判断是否显示箭头
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

            /* 箭头提示 */
            if ((arrow_pcb_min == dm603_error_pcb) && (arrow_err_min == dm603_error_id))
            {
                if (arrow_pcb_min == arrow_pcb_max && arrow_err_max == arrow_err_min)
                {
                    dm603_common_disp_arrow(0, 0, 0, disp_buf); // 只有一项故障不显示箭头
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

            /* 菜单栏 */
            dm603_common_disp_menu(BMP_FN, dm603_face_return, BMP_OK, dm603_face_error_reset, disp_buf);
            break;
    }
}

 /*******************************************************************************
 函 数: dm603_face_limit_time()
 功 能: 系统累计运行时间（系统维护时间）查询界面
 参 数: --
 返 回: --
 其 它: --
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
                if (Main_Key[hmi_t_num] == DM603_KEY_FUNC) // 返回上层界面
                {
                    dm603_pop_face_status();    /* 恢复上层界面状态 */
                    return;
                }
            }

        case DM603_MENU_PAINT :
            disp_string(dm603_face_limit_select_03, ROW1, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);
            dm603_common_disp_val(cell[MAX_PCB].cell_runtime[SYS_RUNTIME]/24, \
                                        ROW2, COL_END - COL_ASC, UINT_D, DCS_NORMAL, disp_buf);

            /* 菜单栏 */
            dm603_common_disp_menu(BMP_FN, dm603_face_return, \
                                   BMP_NU, NULL, disp_buf);
            break;
    }
}


/*******************************************************************************
 函 数: dm603_face_pre()
 功 能: 系统维护提醒界面
 参 数: --
 返 回: --
 其 它: --
*******************************************************************************/
static void dm603_face_pre(U08 *disp_buf)
{
    if (fg_limit_cancel)    /* 取消使用期限，直接返回主界面 */
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
                if (Main_Key[hmi_t_num] == DM603_KEY_FUNC)  /* 返回主界面 */
                {
                    dm603_face[hmi_t_num] = FACE_MAIN;
                    return;
                }
                else if (Main_Key[hmi_t_num] == DM603_KEY_OK_FUNC)
                {
                    dm603_push_face_status();   /* 保存界面状态 */
                    dm603_set_nextface_status(FACE_PRE);   // 确认密码正确后要跳转的界面
                    dm603_face[hmi_t_num] = FACE_PASS;
                    dm603_PassNeed[hmi_t_num] = PASS_LMT;   // 当前密码界面需要的权限
                    return;
                }
            }
            /* break; */

        case DM603_MENU_PAINT :
            /* 提示信息 */
            disp_value_U16(pre_limit_time, ROW1, 0, DNS_DIGIT3 | DNS_ZERO | DCS_NORMAL, disp_buf);

            disp_string(dm603_face_limit_pre_01, ROW1, 4*8, DCS_NORMAL, disp_buf);
            disp_string(dm603_face_limit_pre_02, ROW2, 0, DCS_NORMAL, disp_buf);
            disp_string(dm603_face_limit_pre_03, ROW3, 0, DCS_NORMAL, disp_buf);
            disp_string(dm603_face_limit_pre_04, ROW4, 0, DCS_NORMAL, disp_buf);
            break;
    }
}


/*******************************************************************************
 函 数: dm603_face_off()
 功 能: 系统维护时间到界面
 参 数: --
 返 回: --
 其 它: --
*******************************************************************************/
static void dm603_face_off(U08 *disp_buf)
{
    static U08 dm603_sound_bak;

    if (fg_limit_cancel)    /* 取消使用期限，直接返回主界面 */
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
            dm603_common_disp_flash();  /* 掉电刷新 */
            /* break; */

        case DM603_MENU_PROC :
            /* 返回主界面 */    //NOTE-CXW 要测试
            if (!fg_sys_limit)
            {
                dm603_face[hmi_t_num] = FACE_MAIN;
                return;
            }
            if(key_delay_time[hmi_t_num]>=KEY_DELAY)
            {
                /* 使用期限设置 */
                if (Main_Key[hmi_t_num] == DM603_KEY_OK_FUNC)
                {
                    dm603_push_face_status();   /* 保存界面状态 */
                    dm603_set_nextface_status(FACE_OFF);   // 确认密码正确后要跳转的界面
                    dm603_face[hmi_t_num] = FACE_PASS;
                    dm603_PassNeed[hmi_t_num] = PASS_LMT;   // 当前密码界面需要的权限
                    return;


                }
                /* 故障消音 */
                if (Main_Key[hmi_t_num] == DM603_KEY_OK)
                {
                    dm603_sound[hmi_t_num] = FALSE;
                }
                /* 可关机 */
                if (Main_Key[hmi_t_num] == DM603_KEY_ONOFF)
                {
                    if ((status_run == _RUN || status_run ==_READY))
                    {
                        dm603_face[hmi_t_num] = FACE_ONOFF;
                        return;
                    }
                }
            }
            /* 刷新界面 */
            if (dm603_sound_bak != dm603_sound[hmi_t_num])
            {
                dm603_sound_bak = dm603_sound[hmi_t_num];
                dm603_common_disp_flash();
            }
            /* break; */

        case DM603_MENU_PAINT :
            /* 提示信息 */
            disp_string(dm603_face_limit_off_01, ROW1, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);
            disp_string(dm603_face_limit_off_02, ROW2, 0, DCS_NORMAL | DTS_CLR_TRAIL, disp_buf);

            /* 菜单栏 */
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
 函 数: dm603_time_routine()
 功 能: 定时循环处理
 参 数: --
 返 回: --
 其 它: --
*******************************************************************************/

static void dm603_time_routine(void)
{
    U08 i;

    U08 tweek, thour, tmin;     /* 定时时间 */
    U08 cweek, chour, cmin;     /* 当前时间 */

    cweek = clock[hmi_t_num].week;      /* 周 */
    chour = clock[hmi_t_num].hour;      /* 时 */
    cmin  = clock[hmi_t_num].min ;      /* 分 */

    /* 定时功能启用 */
    if (GET_TIME_EN())
    {
        /* 星期处理 */
        if (cweek == 1) cweek = 6;  /* 星期天 */
        else            cweek -= 2; /* 其它天 */

        for (i = 0; i < (sizeof(dm603_time_tbl)/sizeof(U08))/3; i++)
        {
            tweek = dm603_time_tbl[i*3+0];
            thour = dm603_time_tbl[i*3+1];
            tmin  = dm603_time_tbl[i*3+2];

            /* 无效定时或时间未到 */
        //  if (thour == 0 && tmin == 0)        continue;   //jxj 2016.01.15
            if (thour != chour || tmin != cmin) continue;

            /* 一次定时 */
            if (i == 0)
            {
                if(GET_BIT(tweek, 6))   //一次定时使用开关jxj 2016.01.15
                {
                    if (!fg_enter_holiday) // 处于休假模式时，定时开关机无效
                    {
                        if (GET_BIT(tweek, 7))      DM603_CMD_RUN;
                        else                        DM603_CMD_STOP;
                    }

                    /* 保存定时时间 */
                    CLR_BIT(tweek, 7);  /* 清定时开关 */
                    CLR_BIT(tweek, 6);  /* 清定时使用开关jxj 2016.01.15 */
                    thour = tmin = 0;       /* 清空时间 */
                    dm603_time_tbl[0] = tweek;
                    dm603_time_tbl[1] = thour;
                    dm603_time_tbl[2] = tmin ;

//                  dm603_face_menu_eeprom(MENU_WR, dm603_time_tbl);
                    DataAccess_Timer(DATA_WRITE);
                }
            }
            /* 循环定时 */
            else
            {
                //if (GET_BIT(tweek, cweek))    DM603_CMD_RUN;
                //else                          DM603_CMD_STOP;
                if (!fg_enter_holiday) // 处于休假模式时，定时开关机无效
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
 函 数: dm603_onoff_routine()
 功 能: 开关机循环处理调用
 参 数: --
 返 回: --
 其 它: --
*******************************************************************************/
static void dm603_onoff_routine(void)
{
    if (dm603_face[hmi_t_num] == FACE_LOCK) return;
    if (dm603_face[hmi_t_num] == FACE_LOCK1) return;
    if (dm603_face[hmi_t_num] != FACE_MAIN) return;    // 系统维护界面允许通过开关键关机
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
 函 数: dm603_limit_routine()
 功 能: 系统维护处理
 参 数: --
 返 回: --
 其 它: --
*******************************************************************************/
static void dm603_limit_routine(void)
{
    static BOOL fg_first_in = TRUE; /*  第一次进入标志(上电时使用期限提醒用) */

    switch(dm603_face[hmi_t_num])// 以下界面不强制切换到维护或提醒界面
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

    if (fg_sys_limit)/* 使用期限已到 */
    {
        if (dm603_face[hmi_t_num] != FACE_ONOFF)
        {
            dm603_face[hmi_t_num] = FACE_OFF;
        }
    }
    else if (pre_limit_time != 0  )/* 需要提醒 */
    {
        if (fg_first_in)
        {
            dm603_face[hmi_t_num] = FACE_PRE;
            fg_first_in = FALSE;
        }
        if (clock[hmi_t_num].hour==9 && clock[hmi_t_num].min==0 && clock[hmi_t_num].sec==0  )   /* 每天早上9点整提示 */
        {
            if (!dm603_fg_one_day[hmi_t_num])               /* 一天之内还未提醒 */
            {
                dm603_face[hmi_t_num] = FACE_PRE;
                dm603_fg_one_day[hmi_t_num] = TRUE;
            }
        }
        else                                    /* 一天时间过，清标志，准备下一天提醒 */
        {
            dm603_fg_one_day[hmi_t_num] = FALSE;
        }
    }
}
/*******************************************************************************
 函 数: DispInfo_Temperature()
 功 能: 显示温度信息
 参 数: U16 value_attrib 温度值的显示属性
 返 回: --
 其 它: 适用于名称只有5个字符的情况
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
 函 数: DispInfo_Clock()
 功 能: 显示时间信息
 参 数: RTC ClockDisp: 时钟值, U16 clock_type: 要显示的时间模式
        attrib 每1位代表1个时钟值的显示属性, 0-正常显示, 1-反白显示；
               按时钟项目的顺序(lcd.h)，第6~0位分别代表 年、星期(此处没用)、月、日、时、分、秒
 返 回: --
 其 它:
*******************************************************************************/

void DispInfo_Clock(U16 row, U16 col, RTC ClockDisp, U16 clock_type, U08 attrib, U08 *disp_buf)
{
    U08 Type = 0;
    U16 col_disp;

    U16 attrib_colon = DCS_NORMAL;  // 冒号的显示属性  (是否闪烁)
    U16 attrib_clock = DCS_NORMAL;  // 时钟值的显示属性(是否反白)


    col_disp = col; // 显示初始位置

    if(clock_type&CHN)  Type = 1;   // 1-日期显示中文单位

    if(clock_type&YYYY)
    {
        disp_string_ptr8("20", row, col_disp, DCS_NORMAL, disp_buf);
        col_disp = col_disp+2*8;
    }
    if((clock_type&YY)||(clock_type&YYYY))  // 年
    {
        attrib_clock = (attrib&(0x0001<<RTC_YEAR))?DCS_REVERSE:DCS_NORMAL;
        disp_value_U16(ClockDisp.year, row, col_disp,   DNS_DIGIT2 | DNS_ZERO | attrib_clock, disp_buf);
        col_disp = col_disp+2*8;

        if(Type==1 && dm603_language == LANGUE_CHINESE)// 中文
        {
            disp_string(msg_date_y, row, col_disp, DCS_NORMAL, disp_buf);
            col_disp = col_disp+2*8;
        }
        else if(clock_type&MON) // 后面还有月份要显示
        {
            disp_string_ptr8("/", row, col_disp, DCS_NORMAL, disp_buf);
            col_disp = col_disp+8;
        }
    }
    if(clock_type&MON)  // 月
    {
        attrib_clock = (attrib&(0x0001<<RTC_MONTH))?DCS_REVERSE:DCS_NORMAL;
        disp_value_U16(ClockDisp.month, row, col_disp,  DNS_DIGIT2 | DNS_ZERO | attrib_clock, disp_buf);
        col_disp = col_disp+2*8;

        if(Type==1 && dm603_language == LANGUE_CHINESE)// 中文
        {
            disp_string(msg_date_m, row, col_disp, DCS_NORMAL, disp_buf);
            col_disp = col_disp+2*8;
        }
        else if(clock_type&DAY) // 后面还有日子要显示
        {
            disp_string_ptr8("/", row, col_disp, DCS_NORMAL, disp_buf);
            col_disp = col_disp+8;
        }
    }

    if(clock_type&DAY)  // 日
    {
        attrib_clock = (attrib&(0x0001<<RTC_DAY))?DCS_REVERSE:DCS_NORMAL;
        disp_value_U16(ClockDisp.day, row, col_disp,    DNS_DIGIT2 | DNS_ZERO | attrib_clock, disp_buf);
        col_disp = col_disp+2*8;

        if(Type==1 && dm603_language == LANGUE_CHINESE)// 中文
        {
            disp_string(msg_date_d, row, col_disp, DCS_NORMAL, disp_buf);
            col_disp = col_disp+2*8;
        }
    }

    if(clock_type&HR)   // 时
    {
        attrib_clock = (attrib&(0x0001<<RTC_HOUR))?DCS_REVERSE:DCS_NORMAL;
        disp_value_U16(ClockDisp.hour, row, col_disp,   DNS_DIGIT2 | DNS_ZERO | attrib_clock, disp_buf);
        col_disp = col_disp+2*8;

        if(clock_type&MINU) // 后面还有分钟要显示
        {
            attrib_colon=(clock_type&COLON_BINK)?DCS_BLINK:DCS_NORMAL;  // 冒号是否闪烁

            disp_string_ptr8(":", row, col_disp, attrib_colon, disp_buf);
            col_disp = col_disp+8;
        }
    }

    if(clock_type&MINU) // 分
    {
        attrib_clock = (attrib&(0x0001<<RTC_MIN))?DCS_REVERSE:DCS_NORMAL;
        disp_value_U16(ClockDisp.min, row, col_disp,    DNS_DIGIT2 | DNS_ZERO | attrib_clock, disp_buf);
        col_disp = col_disp+2*8;

        if(clock_type&SEC)  // 后面还有秒钟要显示
        {
            attrib_colon=(clock_type&COLON_BINK)?DCS_BLINK:DCS_NORMAL;  // 冒号是否闪烁

            disp_string_ptr8(":", row, col_disp, attrib_colon, disp_buf);
            col_disp = col_disp+8;
        }
    }

    if(clock_type&SEC)  // 秒
    {
        attrib_clock = (attrib&(0x0001<<RTC_SEC))?DCS_REVERSE:DCS_NORMAL;
        disp_value_U16(ClockDisp.sec, row, col_disp,    DNS_DIGIT2 | DNS_ZERO | attrib_clock, disp_buf);

    }


}


/*******************************************************************************
 函 数: DispString_switch()
 功 能: 闪烁字符串切换
 参 数: SwitchNow：当前显示第几项； SwitchNum：一共有几项需要闪烁显示； TimeBak：  1秒切换同步标志
 返 回: 空
 其 它: 空
*******************************************************************************/
void DispString_switch(U08* SwitchNow, U08 SwitchNum, BOOL* TimeBak)
{
    if(f_blink_1S!= *TimeBak)   // 不能直接用F_TIME_BASIC_SEC_BACK，因当前函数不是每循环调用，调用到时该标志已清零
    {
        if (f_blink_1S==TRUE )
        {
            *SwitchNow = (*SwitchNow>=(SwitchNum-1))?0:(*SwitchNow+1);
        }
        *TimeBak = f_blink_1S;  // 切换标志同步
        dm603_common_disp_flash();
    }
}

/*******************************************************************************
 函 数: DispInfo_Wifi()
 功 能: 显示云服务信息
 参 数:
 返 回: --
 其 它: --
*******************************************************************************/
void DispInfo_TuyaWifi(U16 row, U16 col, U08 *disp_buf)
{
    #define TUYA_SIGNAL_LIEVE_1  0
    #define TUYA_SIGNAL_LIEVE_2  1
    #define TUYA_SIGNAL_LIEVE_3  2
    #define TUYA_SIGNAL_LIEVE_4  3
    #define TUYA_SIGNAL_LIEVE_5  4

    U08 signal_level=TUYA_SIGNAL_LIEVE_1;   // 路由信号强度等级
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
 函 数: dm603_face_main()
 功 能: 主界面
 参 数: --
 返 回: --
 其 它: --
*******************************************************************************/
#define SET_TEMP        0   /* 设置温度 */
#define SET_MODE        1   /* 设置模式 */

#define ADDR_SET_GROUP          0   // 用户设置参数组地址
#define ADDR_MODE               0   // 模式设定地址
#define ADDR_TEMP_COOL          1   // 制冷设定地址
#define ADDR_TEMP_HEAT          2   // 制热设定地址
#define ADDR_TEMP_HOT_WATER     3   // 热水设定地址

const U16 msg_status[] =
{   /* 状态 */
    dm603_face_main_status_01,  // 待机
    dm603_face_main_status_02,  // 启动
    dm603_face_main_status_03,  // 运行
    dm603_face_main_status_04,  // 停机
    dm603_face_main_status_05,  // 报警
    dm603_face_main_status_06,  // 除霜
    dm603_face_main_status_07,  // 防冻
    dm603_face_main_status_08,  // 测试
    dm603_face_main_status_09,  // 预热
    dm603_face_main_status_10,  // 静音
    dm603_face_main_status_11,  // 强劲
    dm603_face_main_status_12,  // 节能
    dm603_face_main_status_13,  // 杀菌
    dm603_face_main_status_14,  // 烘干
    dm603_face_main_status_15,  // 假日
};

const U16 msg_mode[] =
{   /* 模式 */
    dm603_face_main_mode_00,        // 节能
    dm603_face_main_mode_01,        // 制热
    dm603_face_main_mode_02,        // 制冷
    dm603_face_main_mode_03,        // 自动
    dm603_face_main_mode_04,        // 调试
    dm603_face_main_mode_05,        // 热水
};

static void dm603_face_main(U08 *disp_buf)
{
    // 显示模式下标
    static const U08 msg_mode_index[] = {0, 1, 2, 3, 5};

    U08 time_updown = 35;   /* 设定温度退出计时(100ms) */
    I16 value[MAX_HMI];

//  U08 mac_status = _IDLE;     /* 运行状态显示 */
    U16 attrib_mode = DCS_NORMAL;
    U16 attrib_temp = DCS_NORMAL;
    U16 sv_temp = 0;

    static U08 cursor_n[MAX_HMI], cursor_m[MAX_HMI];
    static U08 status_run_bak[MAX_HMI];
    static U08 fg_error_bak[MAX_HMI];

    static U08 main_cursor[MAX_HMI];    // 主界面设置光标
    static U08 fg_nexch[MAX_HMI];       // 不可改设置标志
    static U08 fg_main_set[MAX_HMI];    /* 主界面设置标志 */

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
                    /* 不做主动锁定的功能：因主界面点击确认键会进入定时设置，交互有重叠 */
//                    fg_disp_lock = !fg_disp_lock;
                    fg_disp_lock = FALSE;
                }
            }

            /* 锁屏时无法操作界面 */
            if (!fg_disp_lock)
            {
                switch (Main_Key[hmi_t_num])
                {
                    case DM603_KEY_FUNC:        /* 进入状态查询界面 */
                        dm603_push_face_status();   /* 保存界面状态 */
                        dm603_face[hmi_t_num] = FACE_MENU_USER_SHORTCUT;
//                      dm603_face[hmi_t_num] = FACE_MENU_MODE;
                        return;
//                      break;

                    case DM603_KEY_OK:
                        if (dm603_cnt_key_updwn[hmi_t_num] <= time_updown)  /* 正在设置，切换更改项 */
                        {
                            cursor_n[hmi_t_num] = ADDR_SET_GROUP;
                            if (main_cursor[hmi_t_num] == SET_TEMP)     /* 设定温度 */
                            {
                                cursor_m[hmi_t_num] = GET_TEMP_SET_ADDR();

                            }

                            para_get_val(PG_GetAddr_ParamPri(cursor_n[hmi_t_num]), cursor_m[hmi_t_num], &value[hmi_t_num]);

                            fg_nexch[hmi_t_num] = 0;
                            fg_main_set[hmi_t_num] = 0;
                            dm603_cnt_key_updwn[hmi_t_num] = 0xff;
                        }
                        else if (fg_error) /* 有故障，消音并查询故障 */
                        {
                            dm603_push_face_status();   /* 保存界面状态 */

                            dm603_sound[hmi_t_num] = FALSE;     /* 消音 */
                            dm603_face[hmi_t_num] = FACE_ERROR;
                            return;
                        }
                        else    //定时设置
                        {
                            dm603_push_face_status();   /* 保存界面状态 */

                            dm603_sound[hmi_t_num] = FALSE;     /* 消音 */
                            dm603_face[hmi_t_num] = FACE_MENU_TIME;
                            return;

                        }
                        break;

                    case DM603_KEY_OK_FUNC: /* 参数设置 */
                        dm603_push_face_status();   /* 保存界面状态 */
                        dm603_set_nextface_status(FACE_PARA);   // 确认密码正确后要跳转的界面
                        dm603_face[hmi_t_num] = FACE_PASS;
                        dm603_PassNeed[hmi_t_num] = PASS_PARA;      // 当前密码界面需要的权限
                        return;
//                      break;

                    case DM603_KEY_UP:
                    case DM603_KEY_DOWN:
                        if (dm603_cnt_key_updwn[hmi_t_num] < time_updown)   /* 已进入设置界面 */
                        {
                            if (para_is_nexch(PG_GetAddr_ParamPri(cursor_n[hmi_t_num]), cursor_m[hmi_t_num]))
                            {
                                fg_nexch[hmi_t_num] = 1;
                            }
                            else
                            {
                                if(sv_economy_mode==USED && main_cursor[hmi_t_num] == SET_TEMP) // 节能模式下不允许主界面设定温度
                                    break;

                                para_get_val(PG_GetAddr_ParamPri(cursor_n[hmi_t_num]), cursor_m[hmi_t_num], &value[hmi_t_num]);

                                if (Main_Key[hmi_t_num] == DM603_KEY_UP)    /* 参数加 */
                                {
                                    para_add(PG_GetAddr_ParamPri(cursor_n[hmi_t_num]), cursor_m[hmi_t_num], &value[hmi_t_num], 0);
                                }
                                else            /* 参数减 */
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

            if (fg_error_bak[hmi_t_num] != fg_error)        // 故障状态变化，刷新
            {
                fg_error_bak[hmi_t_num] = fg_error;
                dm603_common_disp_flash();
            }
            if (status_run_bak[hmi_t_num] != status_run)    // 状态变化，刷新
            {
                status_run_bak[hmi_t_num] = status_run;
                dm603_common_disp_flash();
            }

            // 状态处理
            {
                /* 注意：检查status位数 */
                enum {STA_IDLE,     STA_READY,  STA_RUN,    STA_DELAY,
                      STA_ALARM,    STA_DEFR,   STA_ANTI,   STA_TEST,
                      STA_WARM,     STA_SILENT, STA_STRONG, STA_ECO,
                      STA_STER,     STA_DRYUP,  STA_HOLIDAY,STA_MAX};

                U08 mac_status = status_run;        /* 运行状态 */
                U16 status = 0;

                if (Link_remeber_on)                                    mac_status = _RUN;// 原来是启动运行状态，保持显示“运行”   （显示“联锁”的同时）
                else if (status_run==_DELAY && fg_machine_need_open)    mac_status = _READY;

                                                SETBIT(status, mac_status);
                if (ctrl_info[0] & 0x0800)      SETBIT(status, STA_DEFR);   /* 除霜 */
                if (ctrl_info[0] & 0x0400)      SETBIT(status, STA_ANTI);   /* 防冻 */
                if (fg_enter_ability_test)      SETBIT(status, STA_TEST);   // 测试
                if (!fg_warm_complete)          SETBIT(status, STA_WARM);   /* 预热 */
                if (log_sys_is_silent())        SETBIT(status, STA_SILENT); /* 静音 */
                if (ModeIsStr)                  SETBIT(status, STA_STRONG); /* 强劲 */
                if (ModeIsEco)                  SETBIT(status, STA_ECO);    /* 节能 */
                if (fg_timed_disinfect_run)    SETBIT(status, STA_STER);   /* 杀菌 */
                if (fg_enter_floor_dryup)       SETBIT(status, STA_DRYUP);  /* 烘干 */
                if (fg_enter_holiday)           SETBIT(status, STA_HOLIDAY);/* 假日 */

                {
                    U08 i, sta_num=0;
                    U08 sta_list[STA_MAX];
                    static U08 sta_idx[MAX_HMI];
                    static BOL falsh_bak[MAX_HMI];
                    static BOL sta_warm_disp;
                    /* 提取有效状态 */
                    for (i=0; i<STA_MAX; i++)
                    {
                        if (GETBIT(status, i))
                        {
                            sta_list[sta_num++] = i;
                        }
                    }
                    /* 切换显示 */
                    if (falsh_bak[hmi_t_num] != f_blink_1S)
                    {
                        falsh_bak[hmi_t_num] = f_blink_1S;
                        sta_idx[hmi_t_num]++;

                        if (sta_list[sta_idx[hmi_t_num]] == STA_WARM)                        sta_warm_disp = !sta_warm_disp;
                    }
                    if (sta_idx[hmi_t_num] >= sta_num)            sta_idx[hmi_t_num] = 0;
                    /* 显示 */
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
                            /* 显示 */
                            disp_string(msg_status[sta_list[sta_idx[hmi_t_num]]], ROW1, 128, DTS_RIGHT|DCS_FLASH, disp_buf);
                        }
                    }
                }
            }

            // 处理被设置项的显示模式（一般是[模式]和[设定温度]）
            attrib_mode = DCS_FLASH;    //  // 一直刷新，防止由于闪烁导致退出设置后不显示
            attrib_temp = DCS_NORMAL;
            if (dm603_cnt_key_updwn[hmi_t_num] < time_updown)   // 正在设置
            {
                if (main_cursor[hmi_t_num] == SET_MODE) // 设置模式，闪烁显示
                {
                    if (fg_nexch[hmi_t_num])    /* 模式不能修改提示 */
                    {
//                      disp_string(dm603_face_query_para_mode_nexch_01, ROW2, COL_BGN, DCS_NORMAL, disp_buf);
//                      disp_string(dm603_face_query_para_mode_nexch_02, ROW3, COL_BGN, DCS_NORMAL, disp_buf);
                    }
                    else
                    {
                        attrib_mode |= DCS_BLINK;   /* 可以修改，闪烁显示 */
                    }
                }
                else
                {
                    attrib_temp |= DCS_BLINK;       // 设置温度，闪烁显示
                }
            }
            else    // 已退出设置
            {
                fg_nexch[hmi_t_num] = 0;
                fg_main_set[hmi_t_num] = 0;
                main_cursor[hmi_t_num] = SET_TEMP;
                cursor_m[hmi_t_num] = GET_TEMP_SET_ADDR();
            }

            /* 状态区（ROW1）------------------------------------------------------------------------*/
            if(debug_mode == TRUE)  // 水泵调试模式
            {
                disp_string(msg_mode[4], ROW1, COL_BGN, attrib_mode, disp_buf);
            }
//            else    // 正常运行模式
//            {
//                disp_string(msg_mode[msg_mode_index[disp_mode]], ROW1, COL_BGN, attrib_mode, disp_buf);
//            }

            DispInfo_TuyaWifi(ROW1, 5*16-8, disp_buf);  // 显示涂鸦信号


            /* 温度区（ROW2、ROW3）-------------------------------------------------------------------*/
            // 当前温度
//            DispInfo_Temperature(now_text, ctrl_temp_energy, ROW2, COL_BGN+16, DNS_FLOAT1|DNS_DIGIT3, disp_buf);

            // 设定温度
//            if (fg_main_set[hmi_t_num])
//            {
//                sv_temp =  (GET_TEMP_SET_VAL()) / 10;
//            }
//            else
//            {
//                sv_temp =  sv_temp_energy / 10;
//            }
            DispInfo_Temperature(set,      sv_temp,   ROW3, COL_BGN+16, attrib_temp|DNS_DIGIT3, disp_buf);

            /* 提示区（ROW4）------------------------------------------------------------------------*/
            if (fg_error)           // 故障，提示区右侧显示故障按键
            {
                dm603_common_disp_menu(BMP_FN, dm603_face_main_menu2, BMP_OK, NULL, disp_buf);
                disp_string(dm603_face_alarm_query, ROW4, COL_END-48, DCS_BLINK, disp_buf);     // “故障”字符闪烁
            }
            else    // 无故障，提示区右侧显示时钟
            {
                dm603_common_disp_menu(BMP_FN, dm603_face_main_menu2, NULL, NULL, disp_buf);
                DispInfo_Clock(ROW4, COL_END-40, clock[hmi_t_num], HHMM|COLON_BINK, 0, disp_buf);
            }

            if (fg_disp_lock)       /* 锁屏，提示区中间显示锁屏图标  */
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
函数：dm603_error_routine
功能：故障声音状态更新
参数：无
返回值：无
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
 * 函数 : dm603_routine()
 * 功能 : 循环调用
 *
 * 参数 : --
 * 返回 : --
 *
 * 备注 : --
 */
void dm603_routine(U08 *disp_buf)
{
    disp_buf = disp_buf;    /* N0001 */

//  dm603_face_flash();     /* 闪烁标志处理 */

    //dm603_online_routine();   /* 上电循环处理 */


    dm603_error_routine();  /* 故障循环调用 */

//  dm603_onoff_routine();  /* 开关机处理调用 */


    dm603_limit_routine();  /* 使用期限循环调用 */   //cxc

    dm603_time_routine();   /* 定时处理调用 */


//  dm603_key_routine();    /* 按键处理调用 */

//  dm603_other_routine(disp_buf);  /* 其它处理调用 */
}

/**
 * 函数 : dm603_face_start()
 * 功能 : 界面处理前
 *
 * 参数 : --
 * 返回 : --
 *
 * 备注 : --
 */
static void dm603_face_start(U08 *disp_buf)
{
    disp_buf = disp_buf;    /* N0001 */

    if (cnt_menu_lock[hmi_t_num] == 0)          /* 界面未有锁定 */
    {
        cnt_lock_clue[hmi_t_num] = 0;
    }
    else if (Main_Key[hmi_t_num] != KEY_NULL)   /* 界面锁定但有按键 */
    {
        cnt_lock_clue[hmi_t_num] = LOCK_T_CLUE;
//      dm603_fg_lock_clue[hmi_t_num] = TRUE;   /* 锁定提示标志 */
    }
}

/**
 * 函数 : dm603_face_end()
 * 功能 : 界面处理后
 *
 * 参数 : --
 * 返回 : --
 *
 * 备注 : --
 */
static void dm603_face_end(U08 *disp_buf)
{
    static U08 flash_cnt[MAX_HMI];

    /* 锁定提示放到最后面显示 */
    if (cnt_lock_clue[hmi_t_num] > 0)   /* 显示提示信息 */
    {
        flash_cnt[hmi_t_num] = 2;
        disp_string(lock_clue_1, ROW2, 0, DCS_BLINK, disp_buf);
        disp_value_U08(cnt_menu_lock[hmi_t_num], ROW3, 0, DNS_ZERO | DNS_DIGIT2 | DCS_BLINK, disp_buf);
        disp_string(lock_clue_2, ROW3, 16, DCS_BLINK, disp_buf);
    }
    else
    {
        /* 刷新提示信息，防止残留 */
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

//      dm603_fg_lock_clue[hmi_t_num] = FALSE;  /* 锁定提示标志 */
    }

}

/**
 * 函数 : dm603_online_routine()
 * 功能 : 循环掉电处理
 *
 * 参数 : --
 * 返回 : --
 *
 * 备注 : ①该函数调用在lcd.c文件的disp_routine()函数中。
 *          因为，程序如何进入屏保后，会不能刷新掉电界面。切记！！！
 */
void dm603_online_routine(U08 *disp_buf)
{
//  static U16 dm603_dly[MAX_HMI];
    static U08 dm603_is_online[MAX_HMI];
    static U08 dm603_dark_bak[MAX_HMI];

    /* 显示屏掉线退出 */
    if (GET_BIT(hmi_commu_error, hmi_t_num)) return;

    if (dm603_hmi_online[hmi_t_num] >= 8) dm603_is_online[hmi_t_num] = 0;   /* 掉线 */

    if (dm603_hmi_online[hmi_t_num] >= 3)
    {
        disp_clr_screen(disp_buf);
        dm603_common_disp_flash();          /* 掉电刷新 */
    }

    dm603_hmi_online[hmi_t_num] = 0;
    if (dm603_is_online[hmi_t_num] == 0)        /* 上线 */
    {
        Main_Key[hmi_t_num] = KEY_NULL;
        dm603_error_bak[hmi_t_num] = 0;
        dm603_is_online[hmi_t_num] = 1;
        dm603_sound[hmi_t_num] = FALSE;
        dm603_face[hmi_t_num] = FACE_OPEN;
        dm603_power_time[hmi_t_num] = 0;

        cnt_no_operate[hmi_t_num] = 0;
        cnt_no_operate_sec[hmi_t_num] = 0;
        dm603_common_disp_flash();          /* 掉电刷新 */
    }

    /* 判断如果有故障，则需要提示 */
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
const MENUITEM  MenuItem_MainMenu[] = // 主功能菜单
{
    // 菜单类型     /*// 显示级别   */  // 进入权限     // 功能ID               // 菜单名                       // 菜单属性
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_MENU_STATUS,       dm603_face_menu_select_02_,     0,  },      // 0 状态查询
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_MENU_CLOCK,        dm603_face_menu_select_03_,     0,  },      // 1 时钟查询
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_MENU_VERSION,      dm603_face_menu_select_04_,     0,  },      // 2 版本查询
    {MENU_FACE,     /*PASS_NULL,    */  PASS_FUNC,      FACE_MENU_FUNC,         dm603_face_menu_select_05_,     0,  },      // 3 高级功能
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_MENU_CLOUD,        dm603_face_menu_select_06_,     0,  },      // 4 无线功能
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_MENU_FCU,          dm603_face_menu_select_07_,     0,  },      // 5 风盘信息
};
#define FACE_MENU_CLOUD_INDEX   4  // 无线功能-菜单序号
#define FACE_MENU_FCU_INDEX     5  // 风盘信息-菜单序号


//--------------------------------------------------------------------------
const MENUITEM  MenuItem_Wifi[] = // 无线功能菜单
{
    // 菜单类型     /*// 显示级别   */  // 进入权限     // 功能ID               // 菜单名                       // 菜单属性
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_MENU_CLOUD_CMD,    dm603_face_menu_wifi_cmd,       0,  },      // 无线设置
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_MENU_CLOUD_STATUS, dm603_face_menu_wifi_status,    0,  },      // 无线状态
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_MENU_CLOUD_INFO,   dm603_face_menu_wifi_info,      0,  },      // 无线信息

};


//--------------------------------------------------------------------------
const MENUITEM  MenuItem_WifiCMD[] = // 无线设置菜单
{
    // 菜单类型     /*// 显示级别   */  // 进入权限     // 功能ID               // 菜单名                       // 菜单属性
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_CLOUD_CMD_RESET,   dm603_face_menu_wifi_cmd_01,        0,  },      // 无线复位
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_CLOUD_CMD_CLR,     dm603_face_menu_wifi_cmd_02,        0,  },      // 清空数据
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_CLOUD_CMD_RELEASE, dm603_face_menu_wifi_cmd_03,        0,  },      // 解除绑定

};


//--------------------------------------------------------------------------
const MENUITEM  MenuItem_Function[] = // 高级功能菜单
{
    // 菜单类型     /*// 显示级别   */  // 进入权限     // 功能ID               // 菜单名                           // 菜单属性
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_MENU_FUNC_USER,    dm603_face_menu_func_select_01,     0,  },      // 用户设置
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_MENU_FUNC_DEFROST, dm603_face_menu_func_select_02,     0,  },      // 手动除霜
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_MENU_FUNC_RUNTIME, dm603_face_menu_func_select_03,     0,  },      // 运行时间查询
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_MENU_FUNC_HIS_ERR, dm603_face_menu_func_select_04,     0,  },      // 历史故障查询
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PASS_M_USER,       dm603_face_menu_func_select_05,     0,  },      // 修改密码
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_MENU_INVT_PARA,    dm603_face_menu_func_select_06,     0,  },      // 变频参数
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_ENTER_FLOOR_DRYUP, dm603_face_menu_func_select_07,     0,  },      // 地暖烘干功能
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_FORCE_IBH,         dm603_face_menu_func_select_08,     0,  },      // 强开内置电加热
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_FORCE_TBH,         dm603_face_menu_func_select_09,     0,  },      // 强开水箱电加热
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_FORCE_HOT_WATER,   dm603_face_menu_func_select_10,     0,  },      // 强制制热水(快速制热水)
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_HOLIDAY_MODE_SET,  dm603_face_menu_func_select_11,     0,  },      // 休假模式设置
};


//--------------------------------------------------------------------------
const MENUITEM  MenuItem_Limit[] = // 系统维护菜单
{
    // 菜单类型     /*// 显示级别   */  // 进入权限     // 功能ID               // 菜单名                       // 菜单属性
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_LIMIT_SET,         dm603_face_limit_select_01,     0,  },      // 系统维护设置
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PASS_M_LMT,        dm603_face_limit_select_02,     0,  },      // 修改密码（系统维护）
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_LIMIT_RUNTIME,     dm603_face_limit_select_03,     0,  },      // 运行时间查询
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_LIMIT_RUNTIME_INIT,dm603_face_limit_select_04,     0,  },      // 运行时间清零（初始化）
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_LIMIT_INIT,        dm603_face_limit_select_05,     0,  },      // 初始化系统维护
};


//--------------------------------------------------------------------------
const MENUITEM  MenuItem_ParaFace[] = // 参数界面菜单
{
    // 菜单类型     /*// 显示级别   */  // 进入权限     // 功能ID               // 菜单名                       // 菜单属性
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PARA_SET,          dm603_face_para_select_01,      0,  },      // 参数设置
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PASS_M_FAC,        dm603_face_para_select_02,      0,  },      // 修改密码     //NOTE-CXW 澳信没有，要屏蔽
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PARA_INIT_P,       dm603_face_para_select_03,      0,  },      // 初始化参数
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PARA_INIT_C,       dm603_face_para_select_04,      0,  },      // 初始化压机
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_INVT_INIT,         dm603_face_para_select_05,      0,  },      // 初始化变频器
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_MACHINE_CODE,      dm603_face_para_select_06,      0,  },      // 机组编码
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_ABILITY_TEST,      dm603_face_para_select_07,      0,  },      // 能力测试

};
#define FACE_MACHINE_CODE_INDEX  5  // 机组编码-菜单序号


//--------------------------------------------------------------------------
const MENUITEM  MenuItem_ParaGroup[GROUP_MAX] = // 参数组菜单
{
    // 菜单类型     /*// 显示级别   */  // 进入权限     // 功能ID               // 菜单名                   // 菜单属性
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PARA_SET_VALUE,    dm603_para_large_01,        0,  },      // {用户参数}
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PARA_SET_VALUE,    dm603_para_large_02,        0,  },      // {系统参数}
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PARA_SET_VALUE,    dm603_para_large_03,        0,  },      // {压机设置}
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PARA_SET_VALUE,    dm603_para_large_04,        0,  },      // {风机设置}
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PARA_SET_VALUE,    dm603_para_large_05,        0,  },      // {电膨设置}
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PARA_SET_VALUE,    dm603_para_large_06,        0,  },      // {主阀设置}
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PARA_SET_VALUE,    dm603_para_large_07,        0,  },      // {辅阀设置}
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PARA_SET_VALUE,    dm603_para_large_08,        0,  },      // {除霜设置}
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PARA_SET_VALUE,    dm603_para_large_09,        0,  },      // {防冻设置}
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PARA_SET_VALUE,    dm603_para_large_10,        0,  },      // {保护设置}
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PARA_SET_VALUE,    dm603_para_large_11,        0,  },      // {开关量及探头}
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PARA_SET_VALUE,    dm603_para_large_12,        0,  },      // {调试及测试}
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PARA_SET_VALUE,    dm603_para_large_13,        0,  },      // {压传及其它}
    {MENU_FACE,     /*PASS_NULL,    */  PASS_NULL,      FACE_PARA_SET_VALUE,    dm603_para_large_14,        0,  },      // {工况频率表}
};
#define PASS_SER_FORBID     2   // MenuItem_ParaFace从第几项开始禁止维修权限查看

//--------------------------------------------------------------------------
const MENUITEM  MenuItem_PunpInit[] = // 邦普初始化菜单
{
    // 菜单类型     /*// 显示级别   */  // 进入权限     // 功能ID               // 菜单名                   // 菜单属性
    {MENU_ACTION,       /*PASS_NULL,    */  PASS_NULL,      PUNP_INIT_ALL,      msg_punp_init_01,       0,  },      // 初始化全部
    {MENU_ACTION,       /*PASS_NULL,    */  PASS_NULL,      PUNP_INIT_PARA,     msg_punp_init_02,       0,  },      // 初始化参数
    {MENU_ACTION,       /*PASS_NULL,    */  PASS_NULL,      PUNP_INIT_PASSWD,   msg_punp_init_03,       0,  },      // 初始化密码
};


/*******************************************************************************
 函 数: Face_debug()
 功 能: 调试界面
 参 数: --
 返 回: --
 其 它: --
*******************************************************************************/
static void Face_debug(U08 *disp_buf)
{
    static U08 s_page[MAX_HMI]; // 页序号
    U08 s_page_max; // 页面数
    U16 DebugData;  // 临时计算调试数据

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
                /* 返回主界面 */
                if (Main_Key[hmi_t_num] == DM603_KEY_FUNC)
                {
                    dm603_pop_face_status();
                    return;
                }

                /* 向上或向下翻页 */
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

                if(s_page[hmi_t_num]==0)    // 第1页——PID计算值
                {
                    disp_string_ptr8("UK:", ROW1, COL_BGN, DCS_NORMAL, disp_buf);
                    disp_value_I16(DebugPID[DEBUG_UK], ROW1, 24, DNS_DIGIT5, disp_buf);


                    disp_value_I16(pv_cell_gas_in(0), ROW1, 72, DNS_FLOAT1|DNS_DIGIT4, disp_buf);       // 吸气温度
                    disp_value_I16(pv_cell_evap_temp(0), ROW2, 72, DNS_FLOAT1|DNS_DIGIT4, disp_buf);   // 蒸发饱和温度

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
                else if(s_page[hmi_t_num]==1)   // 第2页——变频数据
                {
                    DebugData = invt_info_powerout(INVT_ID_1);
                    disp_string_ptr8("PW:", ROW1, COL_BGN, DCS_NORMAL, disp_buf);
                    disp_value_I16(DebugData, ROW1, 24, DNS_DIGIT5|DNS_FLOAT2, disp_buf);

                }

                dm603_common_disp_arrow(s_page[hmi_t_num], 0, s_page_max, disp_buf);

                /* 菜单栏 */
//              dm603_common_disp_menu(BMP_FN, dm603_face_return, BMP_NU, NULL, disp_buf);
            break;
    }
}

/******************************************************************************
功能:开关机界面
参数:on_off_action-开关机动作,0-开机，1-关机
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
            /* --状态判断-- */
            if (status_run==_READY && !fg_warm_complete)
            {
                if (fg_warm_quick)          run_or_stop = CAN_QUICK_WARM;
                else                        run_or_stop = CAN_NORMAL_WARM;
            }
//            else if (FG_FREEZE_ACT && !STEP_IS_ALARM)
//            {
//                run_or_stop = CAN_STOP;     /* 与模块机保持一致 */
//  //              run_or_stop = CAN_RUN;
//            }
            else if (fg_onoff_union_act)
            {
                run_or_stop = CAN_UNION_ON;     /* 联动开机中 */
            }
            else
            {
                switch (status_run)
                {
                    case _IDLE  : /* break; */
                    case _DELAY : run_or_stop = CAN_RUN;    break;  /* 开机 */
                    case _READY : /* break; */
                    case _RUN   : run_or_stop = CAN_STOP;   break;  /* 关机 */
                    default     : run_or_stop = CAN_NOT;
                }
            }

            /* 按键处理 */
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
//                      DM603_CMD_WARM_CANCEL;  /* 取消预热 */
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
                            run_or_stop = CAN_NORMAL_WARM;          /* 提示是否退出预热 */
                            dm603_common_disp_flash();
                        }
                        else
                        {
                            dm603_face[hmi_t_num] = FACE_MAIN;
                        }
                    }
                    else        // 严重故障时，返回主界面
                    {
                        dm603_face[hmi_t_num] = FACE_MAIN;
                    }
                    return;
//                  break;
            }
            //break;

        case DM603_MENU_PAINT:
            /* 提示栏 */
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
 函 数: dm603_tasking()
 功 能: 显示任务调度界面
 参 数: --
 返 回: --
 其 它: --
*******************************************************************************/

void dm603_tasking(U08 *disp_buf)
{
    static U08 face_bak[MAX_HMI];   /* 界面备份，以方便刷新 */

    PARAMITEM *ParamGroup;

    pdisp_buf = disp_buf;
    //if (GET_BIT(hmi_commu_error, COM_HMI) && GET_BIT(hmi_commu_error, COM_HMI_2)) return;
    if (GET_BIT(hmi_commu_error, hmi_t_num)) return;

    dm603_onoff_routine();  /* 开关机处理调用 */

    /* 切换界面 */
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
            if (val_lock_screen)    fg_disp_lock = TRUE;    /* 锁屏 */
        }
    }
#endif

    if (dm603_face[hmi_t_num] == FACE_MAIN)     dm603_clr_face_status(hmi_t_num);

    dm603_face_start(disp_buf);

    /* 界面 */
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


        case FACE_MENU_TIME:            dm603_face_menu_time(disp_buf);     break;  /* 定时设置 */
        case FACE_MENU_TIME_SET:        dm603_face_menu_time_set(disp_buf); break;  /* 定时时间设置 */
        case FACE_MENU_STATUS:          dm603_face_menu_status(disp_buf);   break;  /* 状态查询 */
        case FACE_MENU_CLOCK:           dm603_face_menu_clock(disp_buf);    break;  /* 时钟查询 */

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
        case FACE_MENU_FCU:             dm603_face_menu_fcu(disp_buf);  break;  /* 风盘信息 */
#endif

        //-------------------------------------------------------------------------------------------------------------------------
        case FACE_MENU_VERSION:         dm603_face_menu_version(disp_buf);      break;  /* 版本查询 */

        //-------------------------------------------------------------------------------------------------------------------------
        case FACE_MENU_FUNC:            Face_menu(ITEM_PER_PAGE, MenuItem_Function, sizeof(MenuItem_Function)/sizeof(MENUITEM), 0,disp_buf);
                                        break;

        //-------------------------------------------------------------------------------------------------------------------------
        case FACE_MENU_FUNC_USER:       ParamGroup = PG_GetAddr_ParamPri(0);
                                        dm603_face_para_SetValue(ParamGroup, para_small[0], 0, 0, disp_buf);
                                        break;

        //-------------------------------------------------------------------------------------------------------------------------
        case FACE_MENU_FUNC_DEFROST:    dm603_face_menu_func_defrost(disp_buf); break;  /* 手动除霜 */
//      case FACE_MENU_FUNC_UINT:       dm603_face_menu_func_unit(disp_buf);    break;  /* 模块使用设置 */
        case FACE_MENU_FUNC_RUNTIME:    dm603_face_menu_func_runtime(disp_buf); break;  /* 运行时间查询 */
        case FACE_MENU_FUNC_HIS_ERR:    dm603_face_menu_func_his_err(disp_buf); break;  /* 历史故障查询 */
//      case FACE_ENTER_FLOOR_DRYUP:    dm603_face_enter_floor_dryup(disp_buf); break;  /* 地暖烘干功能 */
        case FACE_FORCE_IBH:            dm603_face_force_ibh(disp_buf);         break;  /* 强制开启内置电加热 */
        case FACE_FORCE_TBH:            dm603_face_force_tbh(disp_buf);         break;  /* 强制开启水箱电加热 */
//      case FACE_FORCE_HOT_WATER:      dm603_face_force_hot_water(disp_buf);   break;  /* 强制制热水 */
//      case FACE_HOLIDAY_MODE_SET:     dm603_face_holiday_mode_set(disp_buf);  break;  /* 休假模式设置 */


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

                                        // 维护及提醒界面，密码正确后直接解除系统维护功能，返回主界面（在维护及提醒界面中处理）
                                        if(dm603_pass_grade[hmi_t_num]==PASS_LIMIT
                                            && (dm603_face[hmi_t_num]==FACE_OFF || dm603_face[hmi_t_num]==FACE_PRE) )
                                        {
                                            fg_limit_cancel = TRUE;
                                        }
                                        break;

        //-------------------------------------------------------------------------------------------------------------------------
        //case FACE_PASS_M_FAC:         dm603_face_pass_modify(PASS_FAC,disp_buf);  break;      /* 修改厂家密码 */
        case FACE_PASS_M_FAC:           dm603_face_pass_modify(dm603_pass_grade[hmi_t_num],disp_buf);   break;      /* 修改厂家密码 */


        case FACE_PASS_M_USER:          dm603_face_pass_modify(PASS_USER,disp_buf); break;      /* 修改用户密码 */
        case FACE_PASS_M_LMT:           dm603_face_pass_modify(PASS_LIMIT,disp_buf);    break;  /* 修改系统维护密码 */
        case FACE_MENU_MODE:            dm603_face_menu_func_user_mode(disp_buf);  break;       /* 模式设置 */
        case FACE_MENU_USER_SHORTCUT:   dm603_face_menu_func_user_shortcut(disp_buf); break;    /* 快捷设置 */
        //-------------------------------------------------------------------------------------------------------------------------
//      case DM603_FACE_FORMT :         ParamGroup = PG_GetAddr_ParamModbus(0);
//                                        dm603_face_para_SetValue(ParamGroup,bms_data_msg, 0, 0, disp_buf);
//                                      break;

        //-------------------------------------------------------------------------------------------------------------------------
        case FACE_PUNP_INIT:            Face_menu(ITEM_PER_PAGE, MenuItem_PunpInit, sizeof(MenuItem_PunpInit)/sizeof(MENUITEM), 0,disp_buf);
                                        break;

        //-------------------------------------------------------------------------------------------------------------------------
        case FACE_DEBUG:                Face_debug(disp_buf);   break;
        case FACE_INVT_EXCEPT_INFO:     dm603_face_invt_except_info(disp_buf);  break;  /* 变频异常信息查询 */
        case FACE_MENU_INVT_PARA:       dm603_face_invt_para(disp_buf); break;          /* 变频参数界面 */

        default : break;
    }

    dm603_face_end(disp_buf);

}

/*************************************************
函数：dm603_push_face_status
功能：保存界面状态，方便退回时找到断点(类似于堆栈应用)
    密码界面不保存(原因：密码的下一层退出时应该是恢复到进入密码
    前的界面，而非密码界面)
参数：无
返回值：无
**************************************************/
static void dm603_push_face_status(void)
{
    dm603_face_bak[hmi_t_num][dm603_face_index[hmi_t_num]] = dm603_face[hmi_t_num];
    dm603_item_bak[hmi_t_num][dm603_face_index[hmi_t_num]] = dm603_item[hmi_t_num];

    if (dm603_face_index[hmi_t_num] < MAX_FACE_LAYER-1) dm603_face_index[hmi_t_num]++;
    dm603_face_dir[hmi_t_num] = FECE_ENTER;
}

/*************************************************
函数：dm603_pop_face_status
功能：弹出界面状态，退回上一层界面时恢复状态(类似于堆栈应用)
参数：无
返回值：无
**************************************************/
static void dm603_pop_face_status(void)
{
    if (dm603_face_index[hmi_t_num] > 0)    dm603_face_index[hmi_t_num]--;

    dm603_face[hmi_t_num] = dm603_face_bak[hmi_t_num][dm603_face_index[hmi_t_num]];
    dm603_item[hmi_t_num] = dm603_item_bak[hmi_t_num][dm603_face_index[hmi_t_num]];

    dm603_face_dir[hmi_t_num] = FECE_EXIT;
}

/*************************************************
函数：dm603_set_nextface_status
功能：设置下一界面状态，用于密码界面输入正确时跳转到下一界面
参数：无
返回值：无
其它：此界面状态只有一层
**************************************************/
static void dm603_set_nextface_status(U16 FaceId)
{
    dm603_nextface[hmi_t_num]= FaceId;
}

/*************************************************
函数：dm603_get_nextface_status
功能：获取下一界面状态，用于密码界面输入正确时跳转到下一界面
参数：无
返回值：无
其它：密码界面输入错误时，用dm603_pop_face_status返回进入前的界面
**************************************************/
static void dm603_get_nextface_status(void)
{
    dm603_face[hmi_t_num] = dm603_nextface[hmi_t_num];
    dm603_face_dir[hmi_t_num] = FECE_ENTER;
}

/***************************************************************
函数：dm603_disp_err
功能：在指定的行显示故障，占用两行
参数：err_pcb: 故障板； err_num：故障号；err_tag:故障附加号(非0值) row：起始行
返回：无
****************************************************************/
static void dm603_disp_err(U08 err_pcb, U08 err_num, U08 err_tag, U08 row, U08 *disp_buf)
{
    U08 row_err_txt = 0;    /* 故障文本显示行位置 */
    U08 disp_err_num = 0;   /* 显示的故障号(显示的数字可能和实际的err_num不同) */

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
                    else    // 未定义故障（防止与变频器程序未同步升级，故障超出范围）
                    {
                        disp_string(dm603_face_error_unknown, row_err_txt, 128, DTS_RIGHT | DTS_CLR_HEAD, disp_buf);
                    }
                }
            }
        }
        disp_err_num = err_num;
    }

    /* 故障号 */
    disp_string_ptr8(":", row, 56, DCS_NORMAL, disp_buf);
    disp_value_U08(disp_err_num, row, 64, DNS_DIGIT3 | DNS_ZERO| DCS_NORMAL, disp_buf); /* 故障号 */
    if(err_tag!=0)
    {
        disp_string_ptr8("-", row, 88, DCS_NORMAL, disp_buf);
        disp_value_U08(err_tag, row, 96, DNS_DIGIT3 | DNS_ZERO|DCS_NORMAL, disp_buf);   /* 故障号 */
    }
    else
    {
        disp_string_ptr8("    ", row, 88, DCS_NORMAL, disp_buf);    // 无附加号则清空
    }
}


/*******************************************************************************
 函 数: Face_menu()
 功 能: 菜单型界面(非参数组)
 参 数: ItemPage 每页最大项目数;
 返 回: 空
 其 它: 空
*******************************************************************************/
void Face_menu(U08 ItemPage, const MENUITEM *MenuItem,  U08 ItemAll, U08 PassGrade,U08 *disp_buf)   //NOTE-CXW PassGrade最终可能不需要，因进入前已有判断
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
            if (dm603_face_dir[hmi_t_num] == FECE_ENTER)    /* 从上层界面进入该界面才初始化项目选中序号(从下层界面返回时保留) */
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
                    case DM603_KEY_FUNC:    /* 返回主界面 */

                        dm603_pop_face_status();
                        return;
//                      break;

                    case DM603_KEY_OK:
                        if(MenuItem[Item_tbl[hmi_t_num][dm603_item[hmi_t_num]]].MenuType==MENU_FACE)    // 子界面菜单
                        {
                            dm603_push_face_status();   /* 保存进入前界面状态 */

                            if (MenuItem[Item_tbl[hmi_t_num][dm603_item[hmi_t_num]]].MenuPassEnt != PASS_NULL )     /* 需要密码才能进入 */
                            {
                                dm603_set_nextface_status(MenuItem[Item_tbl[hmi_t_num][dm603_item[hmi_t_num]]].FuncId); // 确认密码正确后要跳转的界面
                                dm603_face[hmi_t_num] = FACE_PASS;  // 进入密码界面

                                dm603_PassNeed[hmi_t_num] = MenuItem[Item_tbl[hmi_t_num][dm603_item[hmi_t_num]]].MenuPassEnt;   // 当前密码界面需要的权限
                            }
                            else
                            {
                                dm603_face[hmi_t_num] = MenuItem[Item_tbl[hmi_t_num][dm603_item[hmi_t_num]]].FuncId;
                            }
                            return;
                        }
                        if((MenuItem[Item_tbl[hmi_t_num][dm603_item[hmi_t_num]]].MenuType==MENU_ONOFF)      // 单项ONOFF参数菜单
                            ||(MenuItem[Item_tbl[hmi_t_num][dm603_item[hmi_t_num]]].MenuType==MENU_ACTION)) // 单纯执行动作菜单
                        {
                            Face_menu_action(MenuItem[Item_tbl[hmi_t_num][dm603_item[hmi_t_num]]].FuncId, 1);
                        }
                        break;  /* 跳出切换界面 */

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
            //NOTE-CXW 此部分是否可优化？否则每次都显示
            for (i=min; i<max; i++)
            {
                attrib = DCS_NORMAL;
                if ((i-min) == cursor) attrib = DCS_REVERSE;    /* 反白显示项 */

                row = ROW1 + (i-min)*16;

                // 菜单序号
                disp_value_U08(i+1,     row, 0,     attrib|DNS_DIGIT2|DNS_ZERO, disp_buf);
                disp_string_ptr8(">", row, 16, attrib, disp_buf);

                // 菜单名称
                disp_string(MenuItem[Item_tbl[hmi_t_num][i]].MenuMsg, row, 24, attrib, disp_buf);

                // 单项ONOFF参数菜单 显示值
                if(MenuItem[Item_tbl[hmi_t_num][i]].MenuType==MENU_ONOFF)
                {
#if 0   // 正式图库中没有该图，暂时屏蔽不用
                    Item_value[hmi_t_num][i-min]=Face_menu_action(MenuItem[Item_tbl[hmi_t_num][i]].FuncId, 0);

                    if(Item_value[hmi_t_num][i-min]==1)
                        disp_bmp(idx_bmp_checkon_16_16, row, 104, attrib, disp_buf);
                    else
                        disp_bmp(idx_bmp_checkoff_16_16, row,104, attrib, disp_buf);
#endif
                }
            }

            /* 箭头 */
            dm603_common_disp_arrow(dm603_item[hmi_t_num], 0, ItemMax, disp_buf);

            /* 菜单栏 */
            if(ItemPage<4)
            {
                dm603_common_disp_menu( BMP_FN, dm603_face_return, \
                                        BMP_OK, dm603_face_menu_select_ent, disp_buf);
            }
            break;
    }
}

/*******************************************************************************
 函 数: Face_ParaGroup()
 功 能: 参数组界面
 参 数: ItemPage 每页最大项目数;
 返 回: 空
 其 它: 空
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
            if (dm603_face_dir[hmi_t_num] == FECE_ENTER)    /* 从上层界面进入该界面才初始化项目选中序号(从下层界面返回时保留) */
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
                    case DM603_KEY_FUNC:    /* 返回主界面 */
                        dm603_pop_face_status();
                        return;
//                      break;

                    case DM603_KEY_OK:
                        if(MenuItem[Item_tbl[hmi_t_num][dm603_item[hmi_t_num]]].MenuType==MENU_FACE)    // 子界面菜单
                        {
                            if((MenuItem[Item_tbl[hmi_t_num][dm603_item[hmi_t_num]]].MenuPassEnt == PASS_NULL ) // 不需要对应权限即可进入的参数组
                                ||((dm603_pass_grade[hmi_t_num]!=0xFFFF)    // 当前权限有效     // 需要对应权限才能进入的参数组
                                    &&((0x0001<<dm603_pass_grade[hmi_t_num]) & MenuItem[Item_tbl[hmi_t_num][dm603_item[hmi_t_num]]].MenuPassEnt ))  // 当前权限 匹配 进入权限
                              )
                            {
                                dm603_push_face_status();   /* 保存进入前界面状态 */
                                dm603_face[hmi_t_num] = MenuItem[Item_tbl[hmi_t_num][dm603_item[hmi_t_num]]].FuncId;
                                ParaGroup_Now[hmi_t_num] = Item_tbl[hmi_t_num][dm603_item[hmi_t_num]];
                            }

                            return;
                        }

                        break;  /* 跳出切换界面 */

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
            //NOTE-CXW 此部分是否可优化？否则每次都显示
            for (i=min; i<max; i++)
            {
                attrib = DCS_NORMAL;
                if ((i-min) == cursor) attrib = DCS_REVERSE;    /* 反白显示项 */

                row = ROW1 + (i-min)*16;

                // 菜单序号
                disp_value_U08(i+1,     row, 0,     attrib|DNS_DIGIT2|DNS_ZERO, disp_buf);
                disp_string_ptr8(">", row, 16, attrib, disp_buf);

                // 菜单名称
                disp_string(MenuItem[Item_tbl[hmi_t_num][i]].MenuMsg, row, 24, attrib, disp_buf);
            }

            /* 箭头 */
            dm603_common_disp_arrow(dm603_item[hmi_t_num], 0, ItemMax, disp_buf);

            /* 菜单栏 */
            if(ItemPage<4)
            {
                dm603_common_disp_menu( BMP_FN, dm603_face_return, \
                                        BMP_OK, dm603_face_menu_select_ent, disp_buf);
            }
            break;
    }
}

/*******************************************************************************
 函 数: Face_menu_hide()
 功 能: 界面菜单隐藏
 参 数:
 返 回: 空
 其 它: 空
*******************************************************************************/
U08 Face_menu_hide(U08 ItemAll, U08 *Item_tbl, U08 Tbl_size)
{
    U08 n;
    U08 cur;

    cur = 0;

    for (n = 0; n < ItemAll; n++) /* 该项是否隐藏 */
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
                if (n==FACE_MACHINE_CODE_INDEX && hmi_board_type[hmi_t_num]!= NEW) continue;    // 旧屏没有机组编码菜单
                if (dm603_pass_grade[hmi_t_num] == PASS_SER && n>=PASS_SER_FORBID)continue;     // 根据实际情况修改
                break;

            default:
                break;
        }

        if(cur<Tbl_size) // 防止超界
        {
            Item_tbl[cur++] = n;
        }
    }

    return cur; // 返回菜单项数量
}


/*******************************************************************************
 函 数: Face_menu_action()
 功 能: 菜单界面操作
 参 数: Act_opt=0：返回值   Act_opt=1：执行动作并返回新值
 返 回: value 1）设置参数类动作，返回参数值；2）非设置参数类动作，返回0；
 其 它: 空
*******************************************************************************/
I16 Face_menu_action(U16 FuncID, U08 Act_opt)
{
    I16 value=0;
//    PARAMITEM *ParamGroup;            // 参数组


    switch(FuncID)
    {
//      case SET_SV_ECONOMY_MODE:   // 设置节能模式
//          ParamGroup = PG_GetAddr_ParamPri(0);
//          para_get_val(ParamGroup, 4, &value);    // 获取参数值
//          if(Act_opt==1)
//          {
//              if (!para_is_nexch(ParamGroup, 4))
//              {
//                  value = (value==0)?1:0;
//                  para_set(ParamGroup, 4, value);
//              }
//          }
//          break;

//      /**模板：修改ONOFF参数********************************/
//      case SET_XXX:
//
//          para_get_val(PG_GetAddr_ParamPri(n), m, &value);;   // 获取参数值，根据不同参数，获取值的方法可能不一样。非参数类的动作，可置value=0;
//          if(Act_opt==1)
//          {
//              if (/*参数允许修改*/)
//              {
//                  value = (value==0)?1:0; // 修改参数
//                  para_set(n, m, value);  // 保存参数
//              }
//          }
//          break;
//      /**模板：修改ONOFF参数end*****************************/


        case PUNP_INIT_ALL: // 初始化全部
            if(Act_opt==1)
            {
                DM603_CMD_ALLDATA_INIT;
            }
            break;
        case PUNP_INIT_PARA:    // 初始化参数
            if(Act_opt==1)
            {
                DM603_CMD_PARA_INIT;
            }
            break;
        case PUNP_INIT_PASSWD:  // 初始化密码
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
函数：dm603_clr_face_status
功能：清除界面状态(主界面时清除索引)
参数：无
返回值：无
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
 * 函数 : dm603_init()
 * 功能 : 初始化
 *
 * 参数 : --
 * 返回 : --
 *
 * 备注 : --
 */
void dm603_init(void)
{
    U08 i;

//    DataAccess_Timer(DATA_READ);        // 读取定时时间
    DataAccess_HistoryErr(DATA_READ);   // 读取历史故障信息
    DataAccess_Language(DATA_READ);     // 读取语言信息
    DataAccess_Temp_unit(DATA_READ);     // 读取温度单位信息
    DataAccess_InvtExceptInfo(DATA_READ, _MAX_(U08));   // 读取变频异常信息
//    DataAccess_SkipPreheatCnt(DATA_READ);   /* 读取跳过预热次数 */
//    DataAccess_holiday(DATA_READ);

    if (dm603_language != LANGUE_CHINESE && dm603_language != LANGUE_ENGLISH)//语言不对，直接初始化为中文
    {
        dm603_language = LANGUE_CHINESE;
        DataAccess_Language(DATA_WRITE);    /* 读取语言信息 */
    }
    disp_set_langue(dm603_language);

    for (i = 0; i < MAX_HMI; i++)
    {
        /* 防止连续键错误计时 */
        key_continuous_time[i] = 0;

        dm603_fg_one_day[i] = FALSE;        /* 一天提醒标志 */
//      dm603_fg_screen[i] = FALSE;         /* 屏保标志 */
        dm603_sound[i] = 0;                 /* 消音 */
        dm603_error_bak[i] = 0;             /* 故障标识备份 */
//      dm603_fg_lock_clue[i] = FALSE;      /* 锁定提示标志 */
        dm603_face[hmi_t_num] = FACE_OPEN;
        fg_disp_ver_back[i] = FALSE;
        dm603_clr_face_status(i);
    }
}

/*******************************************************************************
 函 数: blink_time()
 功 能: 闪烁时间(现1S闪烁，可加其他需要时间)
 参 数: 空
 返 回: 空
 其 它: f_blink_1S 1秒切换的标志
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
函数：disp_time
功能：显示中的计时。
参数：无
返回值：无
*****************************************************/
void disp_time(void)
{
    U08 i;

    if (F_TIME_BASIC_100MS_BACK)    /* 100毫秒 */
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
    if (F_TIME_BASIC_SEC_BACK)      /* 1秒 */
    {
        for (i=0; i<MAX_HMI; i++)
        {
            if (cnt_disp_steady[i] != 0xff)     cnt_disp_steady[i]++;
            if (cnt_non_commu[i] != 0xff)       cnt_non_commu[i]++;
            if (cnt_screen_power[i] != 0)       cnt_screen_power[i]--;  /* 倒计时 */
            if (cnt_menu_lock[i] != 0)          cnt_menu_lock[i]--;     /* 倒计时 */
            if (cnt_lock_clue[i] != 0)          cnt_lock_clue[i]--;     /* 倒计时 */

            if (cnt_no_operate_sec[i] != 0xff)  cnt_no_operate_sec[i]++;
            _CNT(dm603_alarm_sound_cnt[i]);     // 蜂鸣器

            _CNT(dm603_power_time[i]);
            _CNT(dm603_hmi_online[i]);
        }
        if (cnt_com_forbid != 0xff)     cnt_com_forbid++;       /*  */

    }
    if (F_TIME_BASIC_MIN_BACK)      /* 1分 */
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
函  数 : PunpDispFlashAll()
功  能 : 刷新所有控件
参  数 : 无
返  回 : 无
其  他 : 修改控件内容前,需要先刷新所有控件
*****************************************************************************/
void PunpDispFlashAll(void)
{
    dm603_common_disp_flash();
}

/*****************************************************************************
函  数 : PunpDispClearScreen()
功  能 : 清屏
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
void PunpDispClearScreen(void)
{
    CHECK_DISPBUF();
    disp_clr_screen(pdisp_buf);
    dm603_common_disp_flash();
}

/*****************************************************************************
函  数 : PunpDispClearLine()
功  能 : 清行,最多一次可清4行
参  数 : U08 line1 : 行号
         U08 line2 : 行号
         U08 line3 : 行号
         U08 line4 : 行号
返  回 : 无
其  他 : 行号无顺序要求,不清的行传入LNULL
*****************************************************************************/
void PunpDispClearLine(U08 line1, U08 line2, U08 line3, U08 line4)
{
    CHECK_DISPBUF();
    dm603_common_disp_clr_line(LINE2ROW(line1), LINE2ROW(line2), LINE2ROW(line3), LINE2ROW(line4), pdisp_buf);
}

/*****************************************************************************
函  数 : PunpDispAscii()
功  能 : 显示ASCII字符串
参  数 : U08* string_ptr : ASCII字符串指针
         U16 line        : 行号
         U16 pos         : 列号
         U16 attrib      : 显示属性
返  回 : 无
其  他 : 无
*****************************************************************************/
void PunpDispAscii(U08* string_ptr, U16 line, U16 pos, U16 attrib)
{
    CHECK_DISPBUF();
    disp_string_ptr8(string_ptr, LINE2ROW(line), POS2COL(pos), attrib, pdisp_buf);
}

/*****************************************************************************
函  数 : PunpDispNum()
功  能 : 显示数值
参  数 : I16 num    : 数值
         U16 line   : 行号
         U16 pos    : 列号
         U16 format : 数值格式
         U16 attrib : 显示属性
返  回 : 无
其  他 : 无
*****************************************************************************/
void PunpDispNum(I16 num, U16 line, U16 pos, U16 format, U16 attrib)
{
    CHECK_DISPBUF();
    disp_value_16(num, LINE2ROW(line), POS2COL(pos), format, attrib, pdisp_buf);
}

/*****************************************************************************
函  数 : PunpDispPv()
功  能 : 显示数值
参  数 : I16 pv     : 模拟量
         U16 line   : 行号
         U16 pos    : 列号
         U16 format : 数值格式
         U16 attrib : 显示属性
返  回 : 无
其  他 : 无
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
函  数 : PunpDispText()
功  能 : 根据字串编号,显示用户自定义字符串
参  数 : U16 string_index : 用户自定义文本编号
         U16 line         : 行号
         U16 pos          : 列号
         U16 attrib       : 显示属性
返  回 : 无
其  他 : 无
*****************************************************************************/
void PunpDispText(U16 string_index, U16 line, U16 pos, U16 attrib)
{
    CHECK_DISPBUF();
    disp_string(string_index, LINE2ROW(line), POS2COL(pos), attrib, pdisp_buf);
}

/*****************************************************************************
函  数 : PunpDispBmp()
功  能 : 根据图片编码显示图片
参  数 : U16 bmp_code : 图片编码
         U16 line     : 行号
         U16 pos      : 列号
         U16 attrib   : 显示属性
返  回 : 无
其  他 : 无
*****************************************************************************/
void PunpDispBmp(U16 bmp_code, U16 line, U16 pos, U16 attrib)
{
    CHECK_DISPBUF();
    disp_bmp(bmp_code, LINE2ROW(line), POS2COL(pos), attrib, pdisp_buf);
}

/*****************************************************************************
函  数 : PunpDispMenu()
功  能 :
参  数 : U16 licon :
         U16 lstr  :
         U16 ricon :
         U16 rstr  :
返  回 : 无
其  他 : 无
*****************************************************************************/
void PunpDispMenu(U16 licon, U16 lstr, U16 ricon, U16 rstr)
{
    CHECK_DISPBUF();
    dm603_common_disp_menu(licon, lstr, ricon, rstr, pdisp_buf);
}
/*****************************************************************************
函  数 : PunpDispSetLanguage()
功  能 : 设定语言
参  数 : U08 language : 语言
返  回 : 无
其  他 : 无
*****************************************************************************/
void PunpDispSetLanguage(U08 language)
{
    if (language <= LANGUE_ENGLISH)
    {
        dm603_language = language;
    }
}

/*****************************************************************************
函  数 : PunpDispGetLanguage()
功  能 : 获取当前语言
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
U08 PunpDispGetLanguage(void)
{
    return dm603_language<=LANGUE_ENGLISH ? dm603_language:LANGUE_CHINESE;
}
#if 0
/*****************************************************************************
函  数 : PunpDispSetLcdAutoDarkTime()
功  能 : 设置自动关背光时间
参  数 : U16 time_out : 0表示不自动关背光,范围10~6000秒
返  回 : 无
其  他 : 无
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
函  数 : PunpDispSetLcdLight()
功  能 : 设置显示器背光开关
参  数 : U08 sta : 开关
返  回 : 无
其  他 : 无
*****************************************************************************/
void PunpDispSetLcdLight(U08 sta)
{
    disp_lcd_light(sta);
}

/*****************************************************************************
函  数 : PunpDispSetSound()
功  能 : 设置显示器报警声
参  数 : U08 sta : 报警声开关
返  回 : 无
其  他 : 无
*****************************************************************************/
void PunpDispSetSound(U08 sta)
{
    fg_sound = sta;
}

/*****************************************************************************
函  数 : PunpDispGetClock()
功  能 : 获取显示器时钟
参  数 : 无
返  回 : 时钟数据结构
其  他 : 无
*****************************************************************************/
RTC PunpDispGetClock(void)
{
    return clock[hmi_r_num];
}

/*****************************************************************************
函  数 : PunpDispSetClock()
功  能 : 设置显示器时钟
参  数 : RTC* pClock : 时钟源数据结构指针
返  回 : 无
其  他 : 无
*****************************************************************************/
void PunpDispSetClock(RTC* pClock)
{
    CHECK_DISPBUF();
    disp_wr_hmi_clk(pdisp_buf, *pClock);
}
#endif
/*****************************************************************************
函  数 : PunpDispGetKey()
功  能 : 读取显示器按键信息
参  数 : 无
返  回 : 按键信息
其  他 : 无
*****************************************************************************/
KEY PunpDispGetKey(void)
{
    return Key[hmi_t_num];
}

/*******************************************************************************
 函 数: dm603_get_vfd_ver()
 功 能:
 参 数: ver_str--;
 返 回: 空
 其 它: 空
*******************************************************************************/
void dm603_get_vfd_ver(U08 *ver_str)
{
    U08 ver[32] = "X1.VF$$$$.$##.V100###";                  /* 例：X1.VF250A.F01.V100A00 */
    char symbol[] = {'X', 'K', 'F', 'T', 'Q'};              /* 洗涤、空调、辅机、通用、其他 */

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

        /* 版本号转换 */
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

/* 打印参数信息 */
#ifdef USE_LOG_PRINT
extern PARAMITEM  ParamPri[];
const U08 str_unit[][20]=
{
 {""     },
 {"℃"   },
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

// 拼接字符串
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
    U08 const *temp_ptr = IDX_TO_C_PTR(str_idx);/* 获取首地址 */
#elif defined (USE_UNICODE)
    U08 const *temp_ptr = chinese_uncompress_text(str_idx);/* 获取首地址 */
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

        /* 中文 */
        if (gbk >= ASC_END)
        {
            str[n++] = HIBYTE(gbk);
            str[n++] = LOBYTE(gbk);
        }
        /* 英文 */
        else
        {
            str[n++] = LOBYTE(gbk);
        }
    }
    while (1);

    str[n] = '\0';
        
    return n;
}



// 打印参数
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

			if ((ParamPri[grp].sign & D_HIDE))                  continue;   /* 跳过隐藏的参数组 */
//            if ((ParamPri[grp].array[item].sign & D_HIDE))      continue;   /* 跳过隐藏的参数 */  
            if (P_isP(grp, item, val_mac_model))                continue;	/* 跳过机器型号 */

            idx = 0;
            // 参数名
            idx += DBG_GetDM603GBK(&str[idx], para_grp_item->msg);//参数名gbk
            idx += DBG_StrCat(&str[idx], "\t");//"

            // 参数信息
//	        mb_addr = ADDR_PARA_BGN + (grp*GROUP_SIZE) + item;
//            mb_addr = BMS_PARA_START + (grp*GROUP_SIZE) + item;
            mb_addr = ADDR_PARA_BGN + (grp*PARA_SIGLE_NUM) + item;

            
            min = pitems->min;
            max = pitems->max;
            dot = pitems->sign & D_DOTMASK;
            unit = (pitems->sign & UINT_MASK) >> 4;
            
            idx += Fun_FormatNum(&str[idx], mb_addr, FNS_UNSIGNED);
            idx += DBG_StrCat(str, "\t");//Modbus地址
            
            idx += Fun_FormatNum(&str[idx], max, FNS_FLOAT(dot));
            idx += DBG_StrCat(str, "\t");//最大值
            
            idx += Fun_FormatNum(&str[idx], min, FNS_FLOAT(dot));
            idx += DBG_StrCat(str, "\t");//最小值
            
            idx += Fun_FormatNum(&str[idx], dot, FNS_UNSIGNED);
            idx += DBG_StrCat(str, "\t");//小数位
            
            idx += DBG_StrCat(str, str_unit[unit]);//单位
            idx += DBG_StrCat(str, "\t");//
            
            idx += DBG_StrCat(str, (U08*)((ParamPri[grp].array[item].sign & D_HIDE)?"Yes":"No"));//隐藏
            idx += DBG_StrCat(str, "\t");

            // 参数字典
            if (para_grp_item->pmsg != NULL)
            {
                U16 dict_num = max;
                U16 d;

                idx += DBG_StrCat(&str[idx], "{");// {

                for (d=0; d<dict_num; d++)
                {
                    idx += DBG_StrCat(&str[idx], "\"");//"
                    idx += Fun_FormatNum(&str[idx], d, FNS_UNSIGNED);// 参数字典key
                    idx += DBG_StrCat(&str[idx], "\":\"");//":"
                    idx += DBG_GetDM603GBK(&str[idx], para_grp_item->pmsg[d]);//参数字典字符串Unicode
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

        DBG_PrintText("\r\n");//每组换行
    }

    while(1)    {WDG_Refresh();}
}

#endif


