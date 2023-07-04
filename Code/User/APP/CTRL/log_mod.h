#ifndef LOG_MOD_H
#define LOG_MOD

#ifdef LOG_MOD_GLOBAL
#define LOG_MOD_EXT
#else
#define LOG_MOD_EXT extern
#endif

enum STEP           { CM_IDLE, CM_STARTING, CM_RUNNING, CM_DEFROSTING,  CM_STOPPING};    
enum STEP_DEFROST   { DS_IDLE, DS_READY,    DS_DEFROST, DS_QUIT};    

// -------------------------------------------------------------------------------------------------------------
// 压机数据结构说明--------------------------------------------------------------------------------------------
// Cm[压机号]:      压机相关数据    
// Cm[MAX_COMP]:    模块相关数据(模块是否需要单独分开较好)          
// -------------------------------------------------------------------------------------------------------------
typedef struct tagCOMP
{
    U08 stage;          // 开机步骤()
    U16 dly;            // 压机计时(开机时各个步骤之间的延时)
                        
    U08 run_mode_need;  // 当前需求模式
    U08 run_mode;       // 当前运行模式
                     
    U16 status;         // 压机状态
    U08 fan_speed;      // 开压机所需风速   
    
    U16 defrost_stage;  // 压机除霜步骤
    U16 defrost_dly;    // 压机除霜计时(除霜时各个步骤之间的延时)

//    U16 rotation;     // 压机转速(0.01rps) （变频压机用）
    
} COMP;

LOG_MOD_EXT COMP  Cm[MAX_COMP + 1];
LOG_MOD_EXT COMP *pComp;




typedef struct tagRTIME_CM      // 压机时间结构
{
    U16 run;                // 累计运行时间(秒)     
    U16 on;                 // 当前运行时间(秒，除霜过程如有关机切换时仍继续计)     
    U16 off;                // 当前停止时间(秒)     
    U16 defrost_exit;       // 共同退出除霜时间(秒) 
    U16 defrost_exit_sin;   // 单独退出除霜时间(秒) 
    
    U16 sw_on;              // 当前输出时间(秒，以继电器输出为准)
    
    U16 defrost_space;      // 除霜间隔计时         //NOTE-CXW 原frost_space[MAX_COMP]
//  U16 defrost_dly;        // 除霜过程中计时       //NOTE-CXW 原defrost_time_count[MAX_COMP]
    U16 defrost_time;       // 除霜时间计时         //NOTE-CXW 原frost_time[MAX_COMP]
    U16 cnt_time_fan_delay; // 结束除霜前开风机延时 //NOTE-CXW 原cnt_time_fan_delay[MAX_COMP]
    U08 frost_out_dly;      // 结束除霜的出水温度计时   //NOTE-CXW 原frost_out_dly
    U16 defrost_quittime;
} RTIME_CM;
LOG_MOD_EXT RTIME_CM  cm_time[MAX_COMP];                //NOTE-CXW 原cm_time[MAX_COMP + 1]

typedef struct tagRTIME // 2类时间结构   
{
    U16 run;    // 累计运行时间(秒)     
    U16 on;     // 当前运行时间(秒)
    U16 off;    // 当前停止时间(秒)
} RTIME;

// 变频异常信息 
typedef struct tagInvtEXCEPT    
{
    U16 info;       // 异常信息累计发生次数或故障号
    RTC time;       // 异常信息最近发生时间
    I16 state[8];   // 异常信息最近发生状态:
                    // --运行转速、瞬时转速、输出电压、输出电流
                    /* --母线电压、环境温度、出水温度、排气温度 */
} InvtEXCEPT;
enum { INFO_200, INFO_004, INFO_007, INFO_008, 
       INFO_E02, INFO_E03, INFO_E04, INFO_E12,
       INFO_E19, INFO_E20, INFO_E31, INFO_EX1,
       INFO_EX2, INFO_EX3, INFO_EX4, INFO_EX5,
       INFO_MAX};           /* 目前最多支持16个断点信息，因使用U16只有16位 */
LOG_MOD_EXT InvtEXCEPT  info_invt_except[INFO_MAX];

// 变频参数界面
typedef struct tagInvtPARA  
{
    I16 addr;       // 参数地址
    I16 val;        // 参数值
    I16 fg_rd;      // 读标志
    I16 fg_wr;      // 写标志             
} InvtPARA;
LOG_MOD_EXT InvtPARA  info_invt_para[INVT_ID_MAX];
#define InvtPARA_Num 4      /* InvtPARA成员数 */

LOG_MOD_EXT U16  CmRotation[INVT_ID_MAX];       // 变频电机转速
LOG_MOD_EXT U16  CmRotation_out[INVT_ID_MAX];   // 变频电机转速:实际输出

#define INVT_CM_KEEP    0   /* 保持 */
#define INVT_CM_UP      1   /* 升频 */
#define INVT_CM_DOWN    2   /* 降频 */
LOG_MOD_EXT U08 invt_cm_sta[INVT_ID_MAX];   // 压机调频状态
#define CM_IS_UP(cm)    (invt_cm_sta[cm] == INVT_CM_UP)
#define CM_IS_KEEP(cm)  (invt_cm_sta[cm] == INVT_CM_KEEP)
#define CM_IS_DOWN(cm)  (invt_cm_sta[cm] == INVT_CM_DOWN)

/* 变频故障自复位 */
#define INVT_RST_PART       0				/* 部分 */
#define INVT_RST_ALL        1				/* 全都 */
#define INVT_RST_NONE       2				/* 全不 */
#define INVT_RST_MAX        3

/* 压机停机时，先减速一段时间，再停机
 * 1、设置转速为0时，变频器会当作最低转速来处理
 * 2、停机命令发送后，变频器才关闭压机
*/
LOG_MOD_EXT U16 invt_off_speed_down_cnt[INVT_ID_MAX];   // 变频减速停机计时
LOG_MOD_EXT U16 close_defro_heat_cnt;

LOG_MOD_EXT U16 comp_realrun_time[MAX_COMP];    /* 压机正常调节情况下运行时间 */

LOG_MOD_EXT U08 rfg_corr_interval;           /* 冷媒温度修正时间间隔 */

#define FORCE_SUB_FREQ_INTERVAL (2*60)          /* 强制降频间隔 */
LOG_MOD_EXT U16 cnt_force_sub_freq[MAX_COMP];   /* 强制降频计时 */

LOG_MOD_EXT U16 cnt_no_freq_limit[MAX_COMP];

//LOG_MOD_EXT U16   defrost_time_count[MAX_COMP];   // 进除霜过程中时间计时
LOG_MOD_EXT U16     mod_input[MAX_COMP+1];          // 模块输入
//LOG_MOD_EXT U16   cm_time_sw_on[MAX_COMP];        /* 当前输出时间(秒) ——对压机：按压机输出SW_CM实际接通来计*/    

LOG_MOD_EXT U16 pv_environment_stable_dly;          // 环境温度消抖时间
LOG_MOD_EXT I16 pv_sys_environment_tmp;             // 环境温度消抖过渡值
LOG_MOD_EXT I16 pv_sys_environment_update;          // 环境温度消抖后更新值，用于要防止环境温度剧烈变化的场合（比如探头故障时）
LOG_MOD_EXT U16 ReOilFrequency[INVT_ID_MAX];  
LOG_MOD_EXT U16 invt_common_run_time[INVT_ID_MAX];  // 压机正常运行计时

LOG_MOD_EXT U16 cnt_invt_chg_speed;  // 变频速率计时

LOG_MOD_EXT BOL fg_freq_pre_sub_en;     /* 到温预降频 */

LOG_MOD_EXT BOL fg_freq_had_force_sub;      /* 强制降频过 */

//LOG_MOD_EXT U16 work_freq_update_cnt; /* 工况频率表更新计时 */

#define REC_NORMAL_TEMP_INTV        10                  /* 记录一般温度间隔 */
#define REC_NORMAL_TEMP_NUM         10                  /* 记录一般温度个数 */
LOG_MOD_EXT U08 mod_rec_gas_out_cnt[MAX_COMP];                      /* 记录排气温度计时 */
LOG_MOD_EXT I16 mod_rec_gas_out[MAX_COMP][REC_NORMAL_TEMP_NUM];     /* 记录排气温度缓存 */
LOG_MOD_EXT I16 mod_rec_gas_out_avg[MAX_COMP];                      /* 排气温度平均值 */

LOG_MOD_EXT I16 test_buf[10];        

#define FREQ_FAN1       0
#define FREQ_FAN2       1
#define FREQSPEEDNUM    2
LOG_MOD_EXT I16 speedsv[FREQSPEEDNUM];          //频率设定值:rpm
LOG_MOD_EXT I16 speedpv[FREQSPEEDNUM];          //频率当前值/* N0004 */
LOG_MOD_EXT I16 speed_percent[FREQSPEEDNUM];    //pwm控制值:百分比   

LOG_MOD_EXT U16 fan_speed_adjust_cnt;
LOG_MOD_EXT U16 cnt_pwm_fan_ctrl_period[FREQSPEEDNUM];

LOG_MOD_EXT U16 fan_restart_cnt;

//冷凝器独立共用
#define TYPE_INDEPENDENCE       0       /* 独立 */
#define TYPE_DEPENDENCE         1       /* 共用 */

/* 互感器类型 */
#define CURR_25A                0       // 25A
#define CURR_35A                1       // 35A
#define CURR_50A                2       // 50A
#define CURR_TYPE_MAX           3

#define TEST_CURR_TYPE          CURR_50A    /* 测试用互感器类型 */

#define val_cond_type           TYPE_DEPENDENCE     // 冷凝器设置(固定为共用) 
#define val_evap_type           TYPE_DEPENDENCE

#define CM_ON                   0x0001      /* 压机运行 */
#define CM_ERR                  0x0002      /* 压机故障 */
#define CM_NEED                 0x0004      /* 压机需求 */
#define CM_QUICK_STOP           0x0008      /* 压机急停(即不考虑除霜和压机最短运行时间等条件) */
                                
#define CM_NEED_DEFROST         0x0010      /* 压机除霜需求 */
#define CM_DEFROST              0x0020      /* 压机正在除霜 */
#define CM_FORCE_DEFROST        0x0040      /* 压机强制除霜 */
#define CM_ALLOW_FORCE_DEFROST  0x0080      /* 压机强制除霜条件满足 */
#define CM_DEFROST_ALLOW        0x0100      /* 压机除霜接受 */
#define CM_DEFROST_EXIT        0x0200       /* 压机除霜退出*/
                                
#define CM_VALVE_S              0x0400      /* 四通阀 */
#define CM_FAN                  0x0800      /* 风机 */
#define CM_VALVE_P              0x0800      /* 喷液阀 */
#define CM_DEFROST_OFF          0x2000      /* 除霜停机 */

/* 底盘电热模式 */
#define HEATER_MODE_DEFROST     0           /* 除霜底盘热 */
#define HEATER_MODE_LOWENV      1           /* 低环底盘热 */
#define HEATER_MODE_MAX         2

/* 低环温高水温电膨控制：是否需要加上压机启动维持时间？ */
LOG_MOD_EXT BOL fg_low_env_high_water;                  /* 低环温高水温标志 */
#define LEHW_EVI_WAIT_TIME  60
#define LEHW_EEV_START_TIME (LEHW_EVI_WAIT_TIME+90)         
//#define LEHW_JUST_OPEN_TIME   (LEHW_EVI_WAIT_TIME+120)    /* 压机启动确认时间：运行时间小于该值认为刚启动 */

#define LEHW_EEV_INIT_TIME  30              /* 低环温高水温启动后转入初始开度的特定“初开维持时间” */

LOG_MOD_EXT U16 motor_run_time[EEV_ID_MAX];     /* 正常调阀时间 */

#define CNT_AFTER_REOIL 20                      /* 回油结束20秒后回到回油前开度 */
LOG_MOD_EXT U16 cnt_reoil_end[MAX_COMP];        /* 回油结束计时 */

/* 变频风机设置 *//* N0004 */
#define FREQ_FAN_SET_UNUSED             0       /* 禁用 */
#define FREQ_FAN_SET_PWM                1       /* 主控PWM风机    */
#define FREQ_FAN_SET_PWM_EX             2       /* 驱动PWM风机    */
#define FREQ_FAN_SET_EC                 3       /* Ec风机邦普 */
#define FREQ_FAN_SET_EC_KeMao           4       /* Ec风机科贸 */
#define FREQ_FAN_SET_EC_ZSFN            5       /* 顺德四格机电风机 */
#define FREQ_FAN_SET_MAX                6       /* 注意：增加风机时，注意检查以下宏定义是否需要更新 */
/* N0004 */
/* 使用PWM风机 */
#define USE_PWM_FAN (val_freq_fan_set==FREQ_FAN_SET_PWM\
                  || val_freq_fan_set==FREQ_FAN_SET_PWM_EX)

/* 使用驱动PWM风机 */
#define USE_PWM_FAN_EX (val_freq_fan_set==FREQ_FAN_SET_PWM_EX)

/* N0004 */
#define USE_EX_EC_FAN   (val_freq_fan_set != FREQ_FAN_SET_UNUSED\
                     &&  !USE_PWM_FAN\
                     &&  val_freq_fan_set != FREQ_FAN_SET_EC)


/* 使用邦普变频驱动 */
#define USE_INVT         (val_inverter_used)
#define USE_INVT_FAN1    (val_freq_fan_set == FREQ_FAN_SET_EC)
#define USE_INVT_FAN2    (val_freq_fan_set == FREQ_FAN_SET_EC && val_freq_fan_num > 1)

#define USE_ECFAN1       (USE_EX_EC_FAN)
#define USE_ECFAN2       (USE_EX_EC_FAN && val_freq_fan_num > 1)

/* 变频风机调速 */
#define FREQ_FAN_CTRL_MANUAL            0       /* 手动 */
#define FREQ_FAN_CTRL_ENV               1       /* 环温线性 */
#define FREQ_FAN_CTRL_WING              2       /* 翅温线性 */
#define FREQ_FAN_CTRL_MAX               3       

/* 排温修正设置 */
#define FAN_TP_CORR_NONE                0	/* 禁用 */
#define FAN_TP_CORR_ALL                 1	/* 全用 */
#define FAN_TP_CORR_COOL                2	/* 仅制冷 */
#define FAN_TP_CORR_HEAT                3	/* 仅制热 */
#define FAN_TP_CORR_MAX                 4
#define FAN_TP_COOL_USE     (val_Tp_fan_rpm_corr_set==FAN_TP_CORR_ALL || val_Tp_fan_rpm_corr_set==FAN_TP_CORR_COOL)
#define FAN_TP_HEAT_USE     (val_Tp_fan_rpm_corr_set==FAN_TP_CORR_ALL || val_Tp_fan_rpm_corr_set==FAN_TP_CORR_HEAT)

/* 风速 */
#define FAN_NONE    0   // 不开
#define FAN_LOW     1   // 低速风
#define FAN_MID     2   // 中速风
#define FAN_HIGH    3   // 高速风

/* 风机高低速切换延时：防止打火 */
#define FAN_CHANGE_DELAY    6  // 600MS
LOG_MOD_EXT U16 fan_high_dly;
LOG_MOD_EXT U16 fan_low_dly;

// 定频风机设置
#define FAN_SPEED_SINGLE            0       // 单速风机
#define FAN_SPEED_DOUBLE_COMBO      1       // 组合双速风机
#define FAN_SPEED_DOUBLE_SELF       2       // 独立双速风机
#define FAN_SPEED_THREE_COMBO       3       // 组合三速风机
#define FAN_SPEED_UNUSED            4       /* 禁用 */
#define FAN_SPEED_MAX               5  

/* 除霜停机切阀 */
#define DF_STOP_CM_NONE             0       /* 禁用     */
#define DF_STOP_CM_ALL              1       /* 进退全停 */
#define DF_STOP_CM_ENTER            2       /* 进除霜停 */
#define DF_STOP_CM_EXIT             3       /* 退除霜停 */
#define DF_STOP_CM_MAX              4

#define CmIsCool(cm)            (Cm[cm].run_mode == MODE_COOL)
#define CmIsHeat(cm)            (Cm[cm].run_mode == MODE_HEAT)

#define CmIsOn(cm)              (S_CM_GetStatus(cm, CM_ON))

/* 辅助电热 */
#define IBH1                        (0)
#define IBH2                        (1)
#define IBH_MAX                     (2)

/* 辅助电热档位需求 */
#define IBH_L0    0   // 不开
#define IBH_L1    1   // 低档
#define IBH_L2    2   // 中档
#define IBH_L3    3   // 高档

/* 辅助电热档位配置 */
#define IBH_NONE     0   // 无
#define IBH_ONLY1    1   // IBH1
#define IBH_ONLY2    2   // IBH2
#define IBH_1AND2    3   // IBH1&IBH2

/* 仅供商检用，未判断val_use_IBH参数 */
#define IBH1_IS_USE  (IBH_cfg_type==IBH_ONLY1 || IBH_cfg_type==IBH_1AND2)
#define IBH2_IS_USE  (IBH_cfg_type==IBH_ONLY2 || IBH_cfg_type==IBH_1AND2)

LOG_MOD_EXT U16 IBH2_add_dly;    /* IBH2加载延时 */

/* 防冻结电加热带 */
LOG_MOD_EXT U08 cnt_HT_on_dly;

// DO1自定义 /* N0004 */
enum {
    DO1_IS_V_BYWAY,         /* 喷液阀 */
    DO1_IS_V_THREE,         /* 三通阀 */
    DO1_IS_MAX
};

// DO3自定义/* N0004 */
enum {
    DO3_IS_CM,
    DO3_IS_CRANK, 
    DO3_IS_MAX
};

// DO5自定义/* N0004 */
enum {
    DO5_IS_FAN2,
    DO5_IS_CM, 
    DO5_IS_MAX
};
// DO9自定义
enum {
    DO9_IS_V_IE,
    DO9_IS_V_DEFR, 
    DO9_IS_V_FLOOR, 
    DO9_IS_END_PUMP,
    DO9_IS_MAX
};

LOG_MOD_EXT U16 cnt_reoil_cycle;         // 回油周期
LOG_MOD_EXT U16 cnt_reoil_time;          // 回油时间

LOG_MOD_EXT BOL fg_have_fan_on;

enum {
    PUMP_IN_USED_SINGLE = 0,
    PUMP_IN_USED_DUAL,
    PUMP_IN_USED_MAX
};

/* 变频水泵间隔运行参数 */
#define PUMPF_INTERVAL_ON_TIME     ( 5*QK_60)  // 水泵间歇开时间
#define PUMPF_INTERVAL_TIME        (PUMPF_INTERVAL_ON_TIME + val_PUMPf_interval_off_time)    // 水泵间歇周期（水泵间歇开时间+水泵间歇停时长）
LOG_MOD_EXT U16 cnt_PUMPf_interval_on_off;
LOG_MOD_EXT U16 mod_pump_inter_on_off_time[SUB_SYS_TYPE_MAX];      //水泵间隔运行时间

#define s_get_cm_status(x, y)       (Cm[x].status & (y)  )
#define s_set_cm_status(x, y)       (Cm[x].status |= (y) )
#define s_clr_cm_status(x, y)       (Cm[x].status &= ~(y))

#define S_CM_GetStatus              s_get_cm_status
#define S_CM_SetStatus              s_set_cm_status
#define S_CM_ClrStatus              s_clr_cm_status

//#define s_get_input_first(comp_num, in_num)           (pCell->cell_input[comp_num] & (in_num))
//#define s_set_input_first(comp_num, in_num)           (pCell->cell_input[comp_num] |= (in_num))
//#define s_get_input(comp_num, in_num)             (mod_input[comp_num] & (in_num))
//#define s_set_input(comp_num, in_num)             (mod_input[comp_num] |= (in_num))

#define s_get_input_first(comp_num, in_num)         GETBIT(pCell->cell_input[comp_num], (in_num)&0x0f)
#define s_set_input_first(comp_num, in_num)         SETBIT(pCell->cell_input[comp_num], (in_num)&0x0f)
#define s_get_input(comp_num, in_num)               GETBIT(mod_input[comp_num], (in_num)&0x0f)
#define s_set_input(comp_num, in_num)               SETBIT(mod_input[comp_num], (in_num)&0x0f)

LOG_MOD_EXT BOL fg_force_IBH_off;           /* 模块强开辅助电热关闭标志 */

enum
{
    CDT_FORBID_ADD_GAS,
    CDT_FORCE_SUB_GAS,
    CDT_FORCE_SUB_COOLCOND,
    CDT_FORCE_SUB_HEATCOND,
    CDT_FORCE_SUB_HEATEVAP,
    CDT_FORCE_SUB_COOLEVAP,
    CDT_FORCE_SUB_MOTOR_EVAP,
    CDT_FORCE_SUB_MOTOR_EXH_OH,
    TIME_CNT2_MAX    
};

//#define TIME_CNT2_MAX 20
LOG_MOD_EXT U16 condition_time_cnt_2[TIME_CNT2_MAX];
#define TIME_CNT3_MAX 20
LOG_MOD_EXT U16 condition_time_cnt_3[TIME_CNT3_MAX];

LOG_MOD_EXT U16 AdjustFreqPeriod;
LOG_MOD_EXT U16 para_init_dly;

LOG_MOD_EXT U16 fg_forbid_rise[MAX_COMP];
LOG_MOD_EXT U16 fg_force_drop[MAX_COMP];
//LOG_MOD_EXT U16 fg_forbid_drop[MAX_COMP];

/* 风机调试信息 */
enum
{
    FAN_ADJ_FG_HIGH,			/* 高风调节标志 */
    FAN_ADJ_FG_MAX
};
LOG_MOD_EXT U16 fan_adjust_fg[MAX_COMP];
enum
{
    FAN_ADJ_TMP_BASE,			/* 基本风速 */
    FAN_ADJ_TMP_COMP,			/* 压机频率修正 */
    FAN_ADJ_TMP_POWER,			/* 功率模式 */
    FAN_ADJ_TMP_EXH,			/* 排温修正 */
    FAN_ADJ_TMP_SILENT,			/* 静音模式 */
    FAN_ADJ_TMP_MAX
};
LOG_MOD_EXT U16 fan_adjust_tmp[MAX_COMP][FAN_ADJ_TMP_MAX];

/* 主阀调试信息 */
enum
{
    EEV_USAGE_SUC,			    /* 吸气过热度 */
    EEV_USAGE_EXH,			    /* 排气过热度 */
    EEV_USAGE_AT,			    /* 能力测试手动目标过热度 */
    EEV_USAGE_MAX
};
enum
{
    EEV_ADJ_TMP_BASE,			/* 原始过热度 */
    EEV_ADJ_TMP_COMP,			/* 过热度频率补偿 */
    EEV_ADJ_TMP_EXH,			/* 过热度排温补偿 */
    EEV_ADJ_TMP_GB,			    /* 国标测试 */
    EEV_ADJ_TMP_RANGE,		    /* 范围限制 */
    EEV_ADJ_TMP_USAGE,			/* 主阀使用设置 */
    EEV_ADJ_TMP_MAX
};
LOG_MOD_EXT U16 eev_adjust_tmp[MAX_COMP][EEV_ADJ_TMP_MAX];

/* 辅阀调试信息 */
enum
{
    EVI_USAGE_OH,			    /* 经济器过热度 */
    EVI_USAGE_OHT,			    /* 温度排气过热度 */
    EVI_USAGE_OHP,			    /* 压力排气过热度 */
    EVI_USAGE_SML,			    /* 主路小阀 */
    EVI_USAGE_HT,			    /* 制热使用 */
    EVI_USAGE_CL,			    /* 制冷使用 */
    EVI_USAGE_AT,			    /* 能力测试手动目标过热度 */
    EVI_USAGE_MAX
};
enum
{
    EVI_ADJ_TMP_BASE,			/* 原始过热度 */
    EVI_ADJ_TMP_RANGE,		    /* 范围限制 */
    EVI_ADJ_TMP_USAGE,			/* 辅阀使用设置 */
    EVI_ADJ_TMP_MAX
};
LOG_MOD_EXT U16 evi_adjust_tmp[MAX_COMP][EVI_ADJ_TMP_MAX];


/* 主辅阀辅助调节标志 */
enum
{
    FORBID_EVI_SUB_EXH,         // 禁止关小排温
    FORBID_EVI_ADD_EXH,         // 禁止开大排温
    FORBID_EVI_ADD_OVEREXH,     // 禁止开大排气过热度
     
    FORBID_EVI_MAX
};
LOG_MOD_EXT U08 fg_evi_forbid[EEV_ID_MAX][FORBID_EVI_MAX];       // 禁止调节evi阀（开大关小）标志

enum
{
    FORCE_ADD_MOTOR_EXH,        // 强制开阀排温
    FORCE_SUB_MOTOR_EVAP,       // 强制关阀蒸发温度
    FORCE_SUB_MOTOR_OVEREXH,    // 强制关小排气过热度
    FORCE_ADD_MOTOR_OVEREXH,    // 强制开大排气过热度
    
    FORCE_MOTOR_MAX
};
LOG_MOD_EXT BOOL fg_force_motor[EEV_ID_MAX][FORCE_MOTOR_MAX];    // 强制动作标志

enum
{
    FORBID_SUB_MOTOR_EXH,   // 禁止关阀排温
    FORBID_SUB_MOTOR_LP,    // 禁止关阀低压
    FORBID_SUB_MOTOR_HP,    // 禁止关阀高压
    FORBID_SUB_MOTOR_WING,  // 禁止关阀翅片温度
    FORBID_SUB_MOTOR_MAX
};
LOG_MOD_EXT BOOL fg_forbid_motor_sub[EEV_ID_MAX][FORBID_SUB_MOTOR_MAX];// 禁止关小标志

enum
{
    FORBID_ADD_MOTOR_EXH,     // 禁止开大排温
    FORBID_ADD_MOTOR_EVAP,    // 禁止开大蒸发温度
    FORBID_ADD_MOTOR_OVEREXH, // 禁止开大排气过热度
    FORBID_ADD_MOTOR_MAX
};
LOG_MOD_EXT BOOL fg_forbid_motor_add[EEV_ID_MAX][FORBID_ADD_MOTOR_MAX];// 禁止开大标志 

enum
{
    EVI_OPEN_ENV,       // 环境温度开阀
    EVI_OPEN_EXH_OH,    // 排气过热开阀
    EVI_OPEN_EXH,       // 排气温度开阀
    EVI_OPEN_RPS,       // 压机转速开阀
    EVI_OPEN_MAX
};
LOG_MOD_EXT U08 fg_evi_allow[EEV_ID_MAX][EVI_OPEN_MAX]; // 开阀标志

/* 故障复位标志 */
LOG_MOD_EXT BOOL fg_err_reset;              

LOG_MOD_EXT BOL fg_cell_pump_open;

/* 压机限制环温分段 */
#define COMP_OFF_AREA     0           /* 关闭区 */
#define COMP_RUN_AREA     1           /* 运行区 */
#define COMP_KEEP_AREA    2           /* 保持区 */
LOG_MOD_EXT U08 limit_comp_TA_area;
#define TA_FORCE_COMP_OFF                (limit_comp_TA_area==COMP_OFF_AREA)
#define TA_ALLOW_COMP_ON_OFF             (limit_comp_TA_area==COMP_RUN_AREA)


/* 制热环温工况分段 */
LOG_MOD_EXT U08 env_area_heat;
#ifdef USE_HT_ENV_NEW
#define val_heat_sv_chg_env1             (380)      // 制热转换温1:环温    
#define val_heat_sv_chg_env2             (280)      // 制热转换温2     
#define val_heat_sv_chg_env3             (100)      // 制热转换温3       
#define val_heat_sv_chg_env4             (30)       // 制热转换温4  
#define val_heat_sv_chg_env5             (-30)      // 制热转换温5   
#define val_heat_sv_chg_env6             (-90)      // 制热转换温6 
#define val_heat_sv_chg_env7             (-130)     // 制热转换温7  
#define val_heat_sv_chg_env8             (-180)     // 制热转换温8
#define val_heat_sv_chg_env9             (-240)     // 制热转换温9
#else
#define val_heat_sv_chg_env1             (380)      // 制热转换温1:环温    
#define val_heat_sv_chg_env2             (280)      // 制热转换温2     
#define val_heat_sv_chg_env3             (120)      // 制热转换温3       
#define val_heat_sv_chg_env4             (30)       // 制热转换温4  
#define val_heat_sv_chg_env5             (-30)      // 制热转换温5   
#define val_heat_sv_chg_env6             (-90)      // 制热转换温6 
#define val_heat_sv_chg_env7             (-150)     // 制热转换温7  
#define val_heat_sv_chg_env8             (-210)     // 制热转换温8 
#endif

// EXDO7自定义
enum {
    EXDO7_IS_CRANK2,			/* 曲轴电加热2 */
    EXDO7_IS_DPH2,			    /* 底盘电加热2 */
    EXDO7_IS_MAX
};

#if defined(M_NIUENTAI)
    #define val_EXDO7_selfdef EXDO7_IS_DPH2

    /* 手动调试:底盘电加热2 */
    #define SHC_EXDO7_MSG   msg_SHC_test_out_DPH2
    #define SHC_EXDO7_PCB   0
    #define SHC_EXDO7_CM    MAX_COMP
    #define SHC_EXDO7_DO    S_DO_DPH2
#else
    #define val_EXDO7_selfdef EXDO7_IS_CRANK2

    /* 手动调试:曲轴电加热2 */
    #define SHC_EXDO7_MSG   msg_SHC_test_out_QZH2
    #define SHC_EXDO7_PCB   0
    #define SHC_EXDO7_CM    0
    #define SHC_EXDO7_DO    SW_crank2
#endif

/* 制冷回水工况分段 */
LOG_MOD_EXT U08 in_area_cool;

/* 压传使用禁用 */
LOG_MOD_EXT I16 fg_press_use_j5;
LOG_MOD_EXT I16 fg_press_use_j6;

LOG_MOD_EXT void log_mod_init(void);        //模块变量初始化
LOG_MOD_EXT void log_mod(void);
LOG_MOD_EXT void runtime_initial(void);
LOG_MOD_EXT BOOL s_cell_have_mode(U08 mode);
LOG_MOD_EXT U08 s_have_comp_defrost(void);
LOG_MOD_EXT void s_Defrost_exit_act(U08 comp_begin, U08 comp_end, U08 comp_num, U08 attrib);
//LOG_MOD_EXT void log_mod_Heater_Valve(void);
LOG_MOD_EXT void fan_comp_group_count(U08 fan_type, U08 *begin, U08 *end, U08 num);
LOG_MOD_EXT U08 log_mod_cond_type(U08 comp_num);
LOG_MOD_EXT BOOL log_mod_comp_is_run(U08 run_type);
LOG_MOD_EXT BOOL log_mod_cell_is_run(U08 run_type);
LOG_MOD_EXT I16 pwm_fan_speed_to_percent(U08 num, I16 speed_sv, I16 speed_pv);
LOG_MOD_EXT U16 log_cmp_is_reoil(U08 cm);
LOG_MOD_EXT U16 log_mod_get_invt_id_max(void);
LOG_MOD_EXT void log_mod_unit_to_comp_group(U08 n_comp, U08 *begin, U08 *end, U08 exio_no);
LOG_MOD_EXT BOOL log_mod_comp_temp_out_limit(U08 x);
LOG_MOD_EXT I16 log_mod_eev_freq(U08 en, U16 step_pv);
#endif
