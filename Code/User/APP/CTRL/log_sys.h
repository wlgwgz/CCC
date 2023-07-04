#ifndef LOG_SYS_H
#define LOG_SYS

#ifdef LOG_SYS_GLOBAL
#define LOG_SYS_EXT
#else
#define LOG_SYS_EXT extern
#endif

/**********************************************************************************/
//#define DEBUG     1   //NOTE-CXW 注意关掉(开放后在开机画面有提示，以做提醒)
//#define TEST1     3
//#define TEST2     4

// 冷媒类型val_refrigerant_type
#define R22                 0   //R22制冷剂
#define R407c               1   //R407C
#define R410A               2   //R410A


#define SWITCH_GOS          0       /* 冷媒 */
#define SWITCH_WATER        1       /* 水路 */

#define IN_MACHINE          0       /* 工程上 */
#define IN_PROJECT          1       /* 机组上 */

#define AIR_WATER               0       /* 风冷送水 */
#define AIR_AIR                 1       /* 风冷送风 */

#define MODE_NULL           0       /* 无 */
#define MODE_COOL           1       /* 制冷 */
#define MODE_HEAT           2       /* 制热 */
#define MODE_AUTO           3       /* 自动:环温在自动切换温度之间 */
#define MODE_HOTW           4       /* 热水 */

#define CTRL_MODE_MAX       3       /* 控制模式数(制冷、制热、热水) */

#define SET_COOL            1               /* 设定制冷 */
#define SET_HEAT            2               /* 设定制热 */
#define SET_AUTO            3               /* 设定自动 */
#define SET_MODE_MAX        4               
//#define SET_HOT_WATER     4               /* 设定热水 */
//#define SET_COOL_HOT_WATER    5               /* 设定制冷&热水 */
//#define SET_HEAT_HOT_WATER    6               /* 设定制热&热水 */
//#define SET_AUTO_HOT_WATER  7             /* 设定自动&热水 */
//#define SET_MODE_MAX        8

#define SILENT_LEVEL_1      0
#define SILENT_LEVEL_2      1
#define SILENT_MAX          2

#define _NOTHING            0       /* 无 */
#define _ADD                1       /* 加载 */
#define _KEEP               2       /* 保持 */
#define _SUBB               3       /* 卸载 */
#define _QUICK              4       /* 急停 */

/* 使用与否 */
#define UNUSED              0       /* 不使用 */
#define USED                1       /* 使用 */

/* 线控器控制方式 */
#define UI35_ZONE_A_TWout2             (0)  /* 单区水温 */
#define UI35_ZONE_A_TR                 (1)  /* 单区室温 */
#define UI35_DUAL_ZONE_TWout2          (2)  /* 双区水温 */
#define UI35_DUAL_ZONE_TWout2_TR       (3)  /* 双区水温&室温 */

/* 温控器控制方式 */
#define UIRT_UNUSED                    (0)  /* 禁用 */
#define UIRT_ZONE_A_MODE_SWITCHING     (1)  /* 单区模式切换 */
#define UIRT_ZONE_A_ON_OFF             (2)  /* 单区开关 */
#define UIRT_DUAL_ZONE_ON_OFF          (3)  /* 双区开关 */

/* 辅助电热 */
#define IBH_UNUSED                     (0)  /* 禁用 */
#define IBH_HEAT_ONLY                  (1)  /* 仅制热 */
#define IBH_HOTW_ONLY                  (2)  /* 仅制热水 */
#define IBH_HEAT_HOTW                  (3)  /* 制热&热水 */

/* 外部热源 */
#define AHS_UNUSED                     (0)  /* 禁用 */
#define AHS_HEAT_ONLY                  (1)  /* 仅制热 */
#define AHS_HOTW_ONLY                  (2)  /* 仅制热水 */
#define AHS_HEAT_HOTW                  (3)  /* 制热&热水 */

/* 出回水 */
#define TEMP_IN             0       // 回水
#define TEMP_OUT            1       // 出水
#define TEMP_NULL           2       // 不区分出回水（无控制对象切换功能）


//00板电源故障
#define SYS_POWER_ERR           0   //系统电源故障
#define MOD_00_POWER_ERR        1   //00号电源故障

/* 电流检测 */
#define CURR_DISP           0       /* 仅显示 */
#define CURR_DISP_ALA       1       /* 显示&报警 */ 
#define CURR_UNUSED         2       /* 不使用 */

// 主阀使用设置
#define EEV_UNUSED          0       // 禁用
#define EEV_OVERHEAT_SUC    1       // 吸气过热度
#define EEV_OVERHEAT_EXH    2       // 排气过热度
#define EEV_CTRL_MAX        3       /* 请在之前插入新的控制方式 */

/* 主电膨能力测试控制 */
#define EEV_ABILITY_AUTO            (0)
#define EEV_ABILITY_MANUAL_STEP     (1)
#define EEV_ABILITY_MANUAL_SSH_SV   (2)
#define EEV_ABILITY_CTRL_TYPE_MAX   (3)

// 辅电膨使用设定
#define EVI_UNUSED              0
#define EVI_ECO_OVERHEAT        1       // 经济器过热度
#define EVI_EXH_OVERHEAT_TMP    2       // 温度排气过热度
#define EVI_EXH_OVERHEAT_PRS    3       // 压力排气过热度
#define EVI_SMALL_EEV           4       // 主路小阀
#define EVI_CTRL_MAX            5       /* 请在之前插入新的控制方式 */

#define HEAT_MODE_ON        0       // 制热开阀
#define COOL_MODE_ON        1       // 制冷开阀
#define VALVE_TYPE_MAX      2

#define NORMAL_OPEN         0       // 常开
#define NORMAL_CLOSE        1       // 常闭

//高排温主辅次序
#define EVI_PRIOR           0       // 辅阀优先
#define EEV_PRIOR			1		// 主阀优先
#define EEV_EVI_SAME		2		// 主阀辅阀同时
#define EEV_EVI_MAX			3

/* 蒸发器/冷凝器类型 */
//#define TYPE_INDEPENDENCE       0       /* 独立 */
//#define TYPE_DEPENDENCE_2       1       /* 2台共用 */
//#define TYPE_DEPENDENCE_3       2       /* 3台共用 */
//#define TYPE_DEPENDENCE_4       3       /* 4台共用 */

///* 二通阀联锁 */
//#define LINK_LOCK               0       // 锁住
//#define LINK_UNLOCK             1       // 断开

//#ifndef USE_QUICK_TIME
//#define LINK_LAST   (60)
//#else
//#define LINK_LAST   (5)
//#endif


//LOG_SYS_EXT BOOL fg_link_en;
//LOG_SYS_EXT U08 link_status;            /* 二通阀状态 */
//LOG_SYS_EXT BOOL fg_link_run;           /* 二通阀记忆机组状态 */
//
//LOG_SYS_EXT BOOL fg_interlock_bak;     /*  */
//LOG_SYS_EXT U08 link_on_cnt;
//LOG_SYS_EXT U08 link_off_cnt;

// 联动功能设置
enum {
    REMOTE_UNUSED,			/* 禁用 */
    REMOTE_UNION_ACT,		/* 联动动作有效 */
    REMOTE_UNION_KEEP,		/* 联动闭合有效 */
    REMOTE_ONOFF,			/* 线控开关机 */
    REMOTE_TBH,				/* 线控水箱电热 */
    REMOTE_AHS,				/* 线控外部热源 */
//    REMOTE_INTERLOCK,		/* 二通阀联锁 */
    REMOTE_MAX
};

#define REMOTE_IS_UNUSED        (val_remote_onoff_type==REMOTE_UNUSED)
#define REMOTE_IS_UNION_ACT     (val_remote_onoff_type==REMOTE_UNION_ACT)
#define REMOTE_IS_UNION_KEEP    (val_remote_onoff_type==REMOTE_UNION_KEEP)
#define REMOTE_IS_ONOFF         (val_remote_onoff_type==REMOTE_ONOFF)
#define REMOTE_IS_TBH           (val_remote_onoff_type==REMOTE_TBH)
#define REMOTE_IS_AHS           (val_remote_onoff_type==REMOTE_AHS)
//#define REMOTE_IS_INTERLOCK     (val_remote_onoff_type==REMOTE_INTERLOCK)

LOG_SYS_EXT BOL fg_onoff_union_act;         /* 联动动作有效：联动开关为主 */
LOG_SYS_EXT U08 cnt_LDIrm_dly;				/* 线控开关信号检测延时 */

/* 线控开关类型 */
#define TYPE_SWITCH                  0       /* 拨动 */
#define TYPE_PULSE                   1       /* 脉冲 */
/* 机组启停控制 */
#define ONOFF_ALL               0           /* 联合 */
#define ONOFF_REMOTE            1           /* 远程 */
#define ONOFF_SELF              2           /* 本地 */
#define ONOFF_SWITCH            3           /* 线控 */
#define ONOFF_COMMU             4           /* 网控 */

#define ACT_ZONE_A        0	/* A区运行状态 */
#define ACT_ZONE_B        1	/* B区运行状态 */
#define ACT_HOTW          2	/* 热水运行状态 */

// 防冰探头使用设置
#define ANTI_ICE_UNUSE   0       // 防冰探头-禁用
#define ANTI_ICE_AI8     1       // 防冰探头-AI8
#define ANTI_ICE_AI7     2       // 防冰探头-AI7
#define ANTI_ICE_MAX     3

/* 带回差的区域划分 */
#define AREA_1                  1           /* 区域一 */ 
#define AREA_2                  2           /* 区域二 */
#define AREA_3                  3           /* 区域三 */
#define AREA_4                  4           /* 区域四 */
#define AREA_5                  5           /* 区域五 */
#define AREA_6                  6           /* 区域六 */

/* 上电时间定义 */
//#define POWER_ON_COMMU           6
#define POWER_ON_INTO_TEST       8      /* 上电进入测试程序截止时间 */
#define POWER_ON_INTO_WORK       10     /* 进入逻辑控制开始时间 */
#define POWER_ON_PREHEAT         15
#define POWER_ON_FREEZ           15
#define POWER_ON_ONOFF           15
#define POWER_ON_CLOUD_TRAS      13     // WiFi屏发送开始时间
#define POWER_ON_CLOUD_RACE      20     // WiFi屏接收开始时间

#define POWER_ON_JUST           (POWER_ON_INTO_WORK+20) /* 此时间前认为刚上电 */
#define POWER_DOWN_DLY          (60)                    /* 掉电延迟：某些主控由变频器供电，存在掉电延迟 */

#ifndef USE_QUICK_TIME
    #define COMMU_EXPT_AUTO_RESET_DLY 30
    #define COMMU_ERR_DETECT_DLY POWER_DOWN_DLY
#else
    #define COMMU_EXPT_AUTO_RESET_DLY (fg_dbg_slow?30:3)
    #define COMMU_ERR_DETECT_DLY      (fg_dbg_slow?POWER_DOWN_DLY:6)
#endif

#define INIT_INVT_RST_TIME      (POWER_ON_INTO_WORK-1)  /* 变频故障复位时间 */
#define INIT_INVT_ERR_TIME      (POWER_ON_INTO_WORK+5)  /* 变频故障检测时间 */

///**********************************************************************************/
///* 用户命令-->常用命令 */
//#define  KEY_CMD_RUN              0x0001  /* 开机命令 */
//#define  KEY_CMD_STOP             0x0002  /* 关机命令 */
//#define  KEY_CMD_RESET            0x0100  /* 故障复位命令 */
//#define  KEY_CMD_SOUND_CANCEL         0x0200  /* 消音命令(此命令为上位机发送) */
//#define  KEY_CMD_WARM_CANCEL      0x0400  /* 取消预热 */
//
///* 厂家命令-->常用命令 */
//#define  KEY_CMD_USER_PARA_INIT       0x0001   /* 用户参数初始化命令 */
//#define  KEY_CMD_FAC_PARA_INIT        0x0002   /* 厂家参数初始化命令 */
//#define  KEY_CMD_RUNTIME_INIT     0x0100   /* 累计运行时间初始化命令 */
//#define  KEY_CMD_SYSTIME_INIT     0x0200   /* 系统运行时间初始化命令 */
///* 厂家命令-->特殊命令 */
//#define  KEY_CMD_SYS_LIMIT            0x0001  /* 使用期限到命令 */

LOG_SYS_EXT BOOL fg_stop_all_mac;   /* 停所有输出故障 */
//LOG_SYS_EXT U16 sys_input_second;

LOG_SYS_EXT I16 sv_temp_energy_cool;        /* 制冷能调目标温度  单位0.1 */
LOG_SYS_EXT I16 sv_temp_energy_heat;        /* 制热能调目标温度  单位0.1 */
LOG_SYS_EXT I16 sv_temp_energy_hotw;        /* 热水能调目标温度  单位0.1 */

LOG_SYS_EXT I16 ctrl_temp_energy_ac;		/* 空调能调控制温度  单位0.1 */
LOG_SYS_EXT I16 ctrl_temp_energy_hotw;		/* 热水能调控制温度  单位0.1 */

LOG_SYS_EXT I16 sv_temp_freq_cool;			/* 制冷调频目标温度  单位0.1 */
LOG_SYS_EXT I16 sv_temp_freq_heat;			/* 制热调频目标温度  单位0.1 */
LOG_SYS_EXT I16 sv_temp_freq_hotw;			/* 热水调频目标温度  单位0.1 */

LOG_SYS_EXT I16 val_add_diff_energy_cool;	/* 制冷能调加载偏差  单位0.1 */
LOG_SYS_EXT I16 val_add_diff_energy_heat;	/* 制热能调加载偏差  单位0.1 */
LOG_SYS_EXT I16 val_add_diff_energy_hotw;	/* 热水能调加载偏差  单位0.1 */

LOG_SYS_EXT I16 val_sub_diff_energy_cool;	/* 制冷能调卸载偏差  单位0.1 */
LOG_SYS_EXT I16 val_sub_diff_energy_heat;	/* 制热能调卸载偏差  单位0.1 */
LOG_SYS_EXT I16 val_sub_diff_energy_hotw;	/* 热水能调卸载偏差  单位0.1 */

LOG_SYS_EXT I16 ctrl_temp_energy[SUB_SYS_TYPE_MAX];         /* 能调控制温度  单位0.1 */
LOG_SYS_EXT I16 sv_temp_energy[SUB_SYS_TYPE_MAX];           /* 能调目标温度  单位0.1 */

//LOG_SYS_EXT I16 val_add_diff_energy[SUB_SYS_TYPE_MAX];      /* 能调加载偏差  单位0.1 */
LOG_SYS_EXT I16 val_sub_diff_energy[SUB_SYS_TYPE_MAX];      /* 能调卸载偏差  单位0.1 */

//LOG_SYS_EXT I16 ctrl_temp_freq;             /* 调频控制温度  单位0.1 */
LOG_SYS_EXT I16 sv_temp_freq[SUB_SYS_TYPE_MAX];               /* 调频目标温度  单位0.1 */

enum AC_ENERGY_CTRL_TEMP_TYPE
{
    AC_NRG_CTRL_TEMP_TR          = 0,		/* 能调控制温度为室内温度TR */
    AC_NRG_CTRL_TEMP_TACT1       = 1,		/* 能调控制温度为缓冲水箱上部温度TACt1 */
    AC_NRG_CTRL_TEMP_TACT2       = 2,		/* 能调控制温度为缓冲水箱下部温度TACt2 */
    AC_NRG_CTRL_TEMP_TACT1_TACT2 = 3,		/* 能调控制温度为AVG(TACt1,TACt2) */
    AC_NRG_CTRL_TEMP_AC_TWOUT2   = 4,		/* 能调控制温度为空调总出水温度 */
};
LOG_SYS_EXT U08 ctrl_temp_type_energy_ac;	/* 空调能调控制温度类型 */

#define TACT1_AC_CTRL           (ctrl_temp_type_energy_ac == AC_NRG_CTRL_TEMP_TACT1)
#define TACT2_AC_CTRL           (ctrl_temp_type_energy_ac == AC_NRG_CTRL_TEMP_TACT2)
#define TACT1_TACT2_AC_CTRL     (ctrl_temp_type_energy_ac == AC_NRG_CTRL_TEMP_TACT1_TACT2)
#define AC_TWOUT2_AC_CTRL       (ctrl_temp_type_energy_ac == AC_NRG_CTRL_TEMP_AC_TWOUT2)

#define AC_NRG_CTRL_TEMP    pv_sys_TWout2_ac    /* 特殊运行模式能调控制温度 */

#define CTRL_SV_BY_PARA     0   /* 由参数设定 */
#define CTRL_SV_BY_CMPS     1   /* 由补偿设定 */
#define CTRL_SV_BY_DUTY     2   /* 由值班设定 */

//LOG_SYS_EXT U08 ctrl_sv_by;     // 制热温度控制值         /* 仅用于测试？ */

/* 命令集合：2022-11-29：已开放到BMS，新增命令时需从后加 */
enum
{
    ZONE_A_ON,              /* A区开机 */
    ZONE_A_OFF,             /* A区关机 */  
    ZONE_B_ON,              /* B区开机 */
    ZONE_B_OFF,             /* B区关机 */  
    HOTW_ON,                /* 热水开机 */
    HOTW_OFF,               /* 热水关机 */

    TIMED_PUMPRET_ON,		/* 下回水泵定时开启 */
    TIMED_PUMPRET_OFF,		/* 下回水泵定时关闭 */
    FORCE_HOTW_ON,          /* 强开热水模式开启 */
    FORCE_HOTW_OFF,         /* 强开热水模式关闭 */
    TIMED_DISINFECT_ON,     /* 定时杀菌功能开启 */
    TIMED_DISINFECT_OFF,    /* 定时杀菌功能关闭 */
    PUMPRET_DISINFECT_ON,	/* 下回水泵杀菌开启 */
    PUMPRET_DISINFECT_OFF,	/* 下回水泵杀菌关闭 */

    HOLIDAY_AWAY_ON,		/* 假日离开模式开启 */
    HOLIDAY_AWAY_OFF,		/* 假日离开模式关闭 */
    HOLIDAY_HOME_ON,		/* 假日在家模式开启 */
    HOLIDAY_HOME_OFF,		/* 假日在家模式关闭 */
    
    FORCE_TBH_ON,           /* 强开水箱电热开启 */
    FORCE_TBH_OFF,          /* 强开水箱电热关闭 */
    FORCE_IBH_ON,           /* 强开辅助电热开启 */
    FORCE_IBH_OFF,          /* 强开辅助电热关闭 */
    FORCE_AHS_ON,           /* 强开外部热源开启 */
    FORCE_AHS_OFF,          /* 强开外部热源关闭 */

    FREE_ELEC_FORCE_HOTW_ON,/* 免费用电强制热水开机 */
    PEAK_ELEC_HOTW_OFF,     /* 高峰用电热水关机 */
    PEAK_ELEC_AC_OFF,       /* 高峰用电空调关机 */

    FLOOR_PREHEAT_ON,		/* 地暖预热功能开启 */
    FLOOR_PREHEAT_OFF,		/* 地暖预热功能关闭 */
    FLOOR_DRYUP_ON,			/* 地暖烘干功能开启 */
    FLOOR_DRYUP_OFF,		/* 地暖烘干功能关闭 */
    
    TEST_RUN_AIR_EMPTY_ON,  /* 空气排空试运行开启 */
    TEST_RUN_AIR_EMPTY_OFF, /* 空气排空试运行关闭 */
    TEST_RUN_PUMP_ON,       /* 水泵试运行开启 */
    TEST_RUN_PUMP_OFF,      /* 水泵试运行关闭 */
    TEST_RUN_COOL_MODE_ON,  /* 制冷试运行开启 */
    TEST_RUN_COOL_MODE_OFF, /* 制冷试运行关闭 */
    TEST_RUN_HEAT_MODE_ON,  /* 制热试运行开启 */
    TEST_RUN_HEAT_MODE_OFF, /* 制热试运行关闭 */
    TEST_RUN_HEAT_IBH_ON,	/* 制热试运行辅助电热开启 */
    TEST_RUN_HEAT_IBH_OFF,	/* 制热试运行辅助电热关闭 */
    TEST_RUN_HOTW_MODE_ON,  /* 制热水试运行开启 */
    TEST_RUN_HOTW_MODE_OFF, /* 制热水试运行关闭 */
    TEST_RUN_HOTW_TBH_ON,	/* 制热水试运行水箱电热开启 */
    TEST_RUN_HOTW_TBH_OFF,	/* 制热水试运行水箱电热关闭 */

    ABILITY_TEST_ON,		/* 能力测试开启 */
    ABILITY_TEST_OFF,		/* 能力测试关闭 */

    REFRI_RECY_ON,		    /* 收氟开启 */
    REFRI_RECY_OFF,		    /* 收氟关闭 */

    SILENT_MODE_ON,		    /* 静音模式开启 */
    SILENT_MODE_OFF,		/* 静音模式关闭 */
    SILENT_MODE_L1,		    /* 静音级别一 */
    SILENT_MODE_L2,		    /* 静音级别二 */

    HOLIDAY_AWAY_HEAT_ON,	/* 假日离家制热模式使用 */
    HOLIDAY_AWAY_HEAT_OFF,	/* 假日离家制热模式禁用 */
    HOLIDAY_AWAY_HOTW_ON,	/* 假日离家热水模式使用 */
    HOLIDAY_AWAY_HOTW_OFF,	/* 假日离家热水模式禁用 */

    TEST_DO_ON,		        /* 手动调试开启 */
    TEST_DO_OFF,		    /* 手动调试关闭 */

    VALLEY_ELEC_AC_OFF,     /* 低谷用电空调关机 */
    VALLEY_ELEC_HOTW_OFF,   /* 低谷用电热水关机 */

    WARM_CMD_CANCEL,		/* 取消预热 */
    WARM_CMD_QUICK,		    /* 快速预热 */
    
    CMD_MAX,
};
LOG_SYS_EXT U16 cmd_mux[_DIVUCARRY(CMD_MAX, 16)];
#define GETCMD_N(cmd_id)              (log_sys_get_cmd_mux_bit(cmd_id))
#define SETCMD_N(cmd_id)              do{                                \
                                          if (cmd_id < CMD_MAX)          \
                                          {                              \
                                              SETBIT_N(cmd_mux, cmd_id); \
                                          }                              \
                                      }while(0)

LOG_SYS_EXT BOL fg_zone_A_on;				/* A区开机标志 */
LOG_SYS_EXT BOL fg_zone_A_off;				/* A区关机标志 */
LOG_SYS_EXT BOL fg_zone_B_on;				/* B区开机标志 */
LOG_SYS_EXT BOL fg_zone_B_off;				/* B区关机标志 */
LOG_SYS_EXT BOL fg_hotw_on;	                /* 热水开机标志 */
LOG_SYS_EXT BOL fg_hotw_off;                /* 热水关机标志 */

LOG_SYS_EXT BOL fg_zone_A_run;				/* A区运行标志 */
LOG_SYS_EXT BOL fg_zone_B_run;				/* B区运行标志 */
LOG_SYS_EXT BOL fg_hotw_run;				/* 热水运行标志 */

LOG_SYS_EXT BOL fg_force_hotw_mode_run;		/* 强开热水模式功能运行标志 */
LOG_SYS_EXT BOL fg_timed_disinfect_run;		/* 定时杀菌功能运行标志 */
LOG_SYS_EXT BOL fg_floor_preheat_run;		/* 地暖预热功能运行标志 */
LOG_SYS_EXT BOL fg_floor_dryup_run;			/* 地暖烘干功能运行标志 */
LOG_SYS_EXT BOL fg_holiday_away_run;		/* 假日离开模式运行标志 */
LOG_SYS_EXT BOL fg_holiday_home_run;		/* 假日在家模式运行标志 */

LOG_SYS_EXT BOL fg_force_TBH_run;			/* 强开水箱电热运行标志 */
LOG_SYS_EXT BOL fg_force_IBH_run;			/* 强开辅助电热运行标志 */
LOG_SYS_EXT BOL fg_force_AHS_run;			/* 强开外部热源运行标志 */

LOG_SYS_EXT BOL fg_TBH_run;                 /* 水箱电热运行标志 */
LOG_SYS_EXT BOL fg_AHS_run;                 /* 外部热源运行标志 */
LOG_SYS_EXT BOL fg_timed_PUMPret_run;       /* 定时下回水泵运行标志 */
LOG_SYS_EXT BOL fg_PUMPret_disinfect_run;	/* 下回水泵杀菌运行标志 */

LOG_SYS_EXT BOL fg_ability_test_run;		/* 能力测试运行标志 */

LOG_SYS_EXT BOL fg_test_run_heat_IBH_run;	/* 制热试运行辅助电热运行标志 */
LOG_SYS_EXT BOL fg_test_run_hotw_TBH_run;	/* 制热水试运行水箱电热运行标志 */

LOG_SYS_EXT BOL fg_holiday_away_heat_mode_used;				/* 假日离家制热模式使用 */
LOG_SYS_EXT BOL fg_holiday_away_hotw_mode_used;				/* 假日离家热水模式使用 */

/* 定时杀菌阶段2，若对应辅助热源可用则置1 */
LOG_SYS_EXT BOL fg_timed_disinfect_TBH_need;/* 定时杀菌水箱电热需求标志 */
LOG_SYS_EXT BOL fg_timed_disinfect_IBH_need;/* 定时杀菌辅助电热需求标志 */
LOG_SYS_EXT BOL fg_timed_disinfect_AHS_need;/* 定时杀菌外部热源需求标志 */

LOG_SYS_EXT BOL fg_timed_disinfect_AHS_on;	/* 定时杀菌外部热源开启标志 */

LOG_SYS_EXT U16 msg_sta_S3V1;               /* 电磁三通阀1状态字符串 */
LOG_SYS_EXT U16 msg_sta_S3V2;               /* 电磁三通阀2状态字符串 */
LOG_SYS_EXT U16 msg_sta_S3V3;               /* 电磁三通阀3状态字符串 */

LOG_SYS_EXT U16 fg_S3V1_is_hw;				/* 电磁三通阀1为热水区 */

LOG_SYS_EXT U16 cnt_S3V3_ext_circue_s;      /* 电磁三通阀3外循环时间 */
LOG_SYS_EXT U16 cnt_S3V3_in_circue_s;       /* 电磁三通阀3内循环时间 */
LOG_SYS_EXT U16 S3V3_in_circue_min_time;    /* 内循环最小时间 */

enum
{
    WATER_TEMP_CTRL,
    ROOM_TEMP_CTRL,
};
LOG_SYS_EXT U08 temp_ctrl_type_zone_A_cool;
LOG_SYS_EXT U08 temp_ctrl_type_zone_B_cool;
LOG_SYS_EXT BOL temp_set_can_edit_zone_A_cool;
LOG_SYS_EXT BOL temp_set_can_edit_zone_B_cool;
LOG_SYS_EXT BOL fg_sv_temp_energy_overwrite_cool;			/* 制冷能调目标温度被改写标志 */

LOG_SYS_EXT U08 temp_ctrl_type_zone_A_heat;
LOG_SYS_EXT U08 temp_ctrl_type_zone_B_heat;
LOG_SYS_EXT BOL temp_set_can_edit_zone_A_heat;
LOG_SYS_EXT BOL temp_set_can_edit_zone_B_heat;
LOG_SYS_EXT BOL fg_sv_temp_energy_overwrite_heat;			/* 制热能调目标温度被改写标志 */

LOG_SYS_EXT U08 temp_ctrl_type_zone_A;
LOG_SYS_EXT U08 temp_ctrl_type_zone_B;
LOG_SYS_EXT BOL temp_set_can_edit_zone_A;
LOG_SYS_EXT BOL temp_set_can_edit_zone_B;
LOG_SYS_EXT BOL fg_sv_temp_energy_overwrite_ac;             /* 空调能调目标温度被改写标志 */
LOG_SYS_EXT BOL fg_sv_temp_energy_overwrite_hotw;			/* 热水能调目标温度被改写标志 */

LOG_SYS_EXT I16 temp_sv_disp_zone_A_cool;
LOG_SYS_EXT I16 temp_sv_disp_zone_B_cool;
LOG_SYS_EXT I16 temp_sv_disp_zone_A_heat;
LOG_SYS_EXT I16 temp_sv_disp_zone_B_heat;

LOG_SYS_EXT BOL fg_enter_test_run;			/* 试运行运行标志 */
LOG_SYS_EXT BOL fg_air_empty_test_run;		/* 进入空气排空试运行 */
LOG_SYS_EXT BOL fg_pump_test_run;		    /* 进入水泵试运行运行标志 */
LOG_SYS_EXT BOL fg_test_run_check_LDIw;		/* 试运行检测水流开关标志 */
LOG_SYS_EXT BOL fg_test_run_PUMPf_off;		/* 试运行变频水泵关闭标志 */
LOG_SYS_EXT BOL fg_can_enter_test_run;
LOG_SYS_EXT U08 fg_test_run_cur;			/* 当前试运行 */

enum
{
    AC_ENRG_CTRL_ZONE_A_ROOM,				/* 单区水温能调 */
    AC_ENRG_CTRL_ZONE_A_WATER,				/* 单区水温能调 */
    AC_ENRG_CTRL_DUAL_ZONE_WATER,			/* 双区水温能调 */
    AC_ENRG_CTRL_DUAL_ZONE_HYBRID,			/* 双区混合能调 */
    AC_ENRG_CTRL_MAX,    
};
LOG_SYS_EXT U08 ac_energy_ctrl_type;

//#define ZONE_A        0
//#define ZONE_B        1
//#define ZONE_MAX      2
//
//typedef struct tag_ZONE_STATUS_FG
//{
//    U08 terminal_type        : 3; // 区域末端类型
//    U08 zone_on              : 1; // 区域是否处于运行状态
//    U08 room_temp_ctrl       : 1; // 区域是否为室内温度控制 
//
//    U08 weather_curve        : 1; // 是否使用环温-目标温度曲线
//    U08 comp_on              : 1; // 压机是否处于开启状态
//    U08 sta_antifreeze       : 1; // 是否处于防冻状态
//    U08 sta_defrost          : 1; // 是否处于除霜状态
//
//    U08 sta_holiday          : 1; // 是否处于假日状态
//    U08 sta_silent           : 1; // 是否处于静音状态
//    U08 sta_eco              : 1; // 是否处于节能状态
//    U08 active_ahs           : 1; // 外部热源是否启动
//
//    U08 active_ibh           : 1; // 辅助电热是否启动
//    U08 active_solar         : 1; // 太阳能是否启动
//    U08 active_tbh           : 1; // 水箱电热是否启动
//    
//    U08 
//} ZONESTA_FG;
//
//typedef struct tag_DHW_STATUS_FG
//{
//    U08 dhw_on               : 1; // 热水是否处于运行状态
//    U08 sta_disinfect        : 1; // 是否处于杀菌状态
//    U08 comp_on              : 1; // 压机是否处于开启状态
//    
//} DHWSTA_FG;
//
//LOG_SYS_EXT ZONESTA_FG zone_fg[ZONE_MAX];

LOG_SYS_EXT BOOL commu_run;
LOG_SYS_EXT BOOL commu_stop;
LOG_SYS_EXT BOOL key_reset;
LOG_SYS_EXT BOOL key_warm_cancel;
LOG_SYS_EXT BOOL key_sound_cancel;
LOG_SYS_EXT BOOL key_user_para_init;
LOG_SYS_EXT BOOL key_fac_para_init;
LOG_SYS_EXT BOOL key_fac_limit_init;

LOG_SYS_EXT BOOL key_fac_invt_init;
LOG_SYS_EXT BOOL key_fac_enter_ability_test;
LOG_SYS_EXT BOOL key_runtime_init;
LOG_SYS_EXT BOOL key_systime_init;
//LOG_SYS_EXT BOOL key_sys_limit;
LOG_SYS_EXT BOOL key_alldata_init;
LOG_SYS_EXT BOOL key_password_init;


/**********************************************************************************/




/**********************************************************************************/
/* cell[].cell_status[]在主模块中写入时作辅助使用，是为了同步从发送命令到读取从机状态带来的时间误差
   写入均是在命令发送后写入 */
#define M_CM_GetStatus(pcb_num, comp_num, attrib)    (cell[pcb_num].cell_status[comp_num] & (attrib))
#define M_CM_SetStatus(pcb_num, comp_num, attrib)    (cell[pcb_num].cell_status[comp_num] |= (attrib))
#define M_CM_ClrStatus(pcb_num, comp_num, attrib)    (cell[pcb_num].cell_status[comp_num] &= ~(attrib))

#define m_get_output(mod, cm, n)                     (GET_BIT(cell[mod].cell_relay[cm], (n)% RLY_BIT_NUM))
#define m_set_output(mod, cm, n)                     (SET_BIT(cell[mod].cell_relay[cm], (n)% RLY_BIT_NUM))
#define m_clr_output(mod, cm, n)                     (CLR_BIT(cell[mod].cell_relay[cm], (n)% RLY_BIT_NUM))

#define CELL_Is_used(x)     (*(&val_cell_enable_00+(x))==USED)          /* 单元使用 */
/* 模块可使用热水 */
#define CELL_USE_HOTW(pcb_num)                      (M_CM_GetStatus(pcb_num, MAX_COMP, CELL_CAN_RUN_HOTW))
/* 模块强开辅助电热可关闭 */
#define CELL_FORCE_IBH_CAN_OFF(pcb_num)             (M_CM_GetStatus(pcb_num, MAX_COMP, CELL_FORCE_IBH_OFF))

/**********************************************************************************/
enum STATUS_RUN { _IDLE, _READY, _RUN, _DELAY, _ALARM };
LOG_SYS_EXT U08 status_run;         /* 运行态 */

LOG_SYS_EXT U16 sys_input;          /* 系统输入 */

//LOG_SYS_EXT U16 relay_output;

LOG_SYS_EXT U08 auto_run_bit;

LOG_SYS_EXT BOL fg_refri_recy_act;              /* 收氟标志 */
LOG_SYS_EXT U16 cmd_refri_recy_enter;           /* 进入收氟 */
LOG_SYS_EXT BOL cmd_refri_recy_stop;            /* 退出收氟 */
LOG_SYS_EXT U16 cnt_refri_recy_lp[MAX_COMP];    /* 低压退收氟计时 */

LOG_SYS_EXT BOL fg_pump_on[SUB_SYS_TYPE_MAX];               /* 子系统变频水泵开启标志 */
LOG_SYS_EXT BOL fg_pump_inter_on[SUB_SYS_TYPE_MAX];         /* 子系统变频水泵间歇开标志 */

enum ANTIFREEZE_TYPE
{
    AF_IDLE_STA,
    AF_RUN_STA_AC,
    AF_RUN_STA_HOTW,
    AF_TYPE_MAX,
};

enum ANTIFREEZE_STAGE
{
    AF_STAGE_IDLE,          /* 空闲 */
    AF_STAGE_AC,            /* 空调水路防冻阶段 */
    AF_STAGE_HOTW,          /* 热水水路防冻阶段 */
};
LOG_SYS_EXT U08 idle_sta_antifreeze_stage;  /* 待机态防冻阶段 */
LOG_SYS_EXT U08 run_sta_antifreeze_stage;   /* 运行态防冻阶段 */

#define GET_AF_STAGE(stage)         ((idle_sta_antifreeze_stage==stage) || (run_sta_antifreeze_stage==stage))

LOG_SYS_EXT BOL fg_idle_sta_antifreeze;                     /* 待机态防冻标志 */
LOG_SYS_EXT BOL fg_idle_sta_antifreeze_pump_need;           /* 待机态防冻开水泵需求 */
LOG_SYS_EXT BOL fg_idle_sta_antifreeze_IBH_need;            /* 待机态防冻开辅助电热需求 */
LOG_SYS_EXT BOL fg_idle_sta_antifreeze_COMPf_need;			/* 待机态防冻开压机需求 */

LOG_SYS_EXT BOL fg_run_sta_ac_antifreeze;                   /* 运行态空调防冻标志 */
LOG_SYS_EXT BOL fg_run_sta_ac_antifreeze_pump_need;         /* 运行态空调防冻开水泵需求 */
LOG_SYS_EXT BOL fg_run_sta_ac_antifreeze_IBH_need;          /* 运行态空调防冻开辅助电热需求 */
LOG_SYS_EXT BOL fg_run_sta_ac_antifreeze_COMPf_need;        /* 运行态空调防冻开压机需求 */

LOG_SYS_EXT BOL fg_run_sta_hotw_antifreeze;                 /* 运行态热水防冻标志 */
LOG_SYS_EXT BOL fg_run_sta_hotw_antifreeze_pump_need;       /* 运行态热水防冻开水泵需求 */
LOG_SYS_EXT BOL fg_run_sta_hotw_antifreeze_IBH_need;        /* 运行态热水防冻开辅助电热需求 */
LOG_SYS_EXT BOL fg_run_sta_hotw_antifreeze_COMPf_need;      /* 运行态防冻开压机需求 */

/* 待机态防冻 或 运行态空调水路防冻 */
#define FG_IDLE_OR_AC_ANTIFREEZE                   (fg_idle_sta_antifreeze || fg_run_sta_ac_antifreeze)
/* 待机态防冻 或 运行态热水水路防冻 */
#define FG_IDLE_OR_HOTW_ANTIFREEZE                 (fg_idle_sta_antifreeze || fg_run_sta_hotw_antifreeze)

/* 模块处于运行态空调水路防冻 */
#define MOD_IS_RUN_STA_AC_AF(pcb_num)      ((GET_SUB_SYS(pcb_num) == SUB_SYS_AC_ONLY) && fg_run_sta_ac_antifreeze)
/* 模块处于运行态热水水路防冻 */
#define MOD_IS_RUN_STA_HOTW_AF(pcb_num)    ((GET_SUB_SYS(pcb_num) == SUB_SYS_AC_HOTW) && fg_run_sta_hotw_antifreeze)

/* A区定时防冻 */
LOG_SYS_EXT BOL fg_zone_A_timed_antifreeze;                 /* A区定时防冻 */
LOG_SYS_EXT BOL fg_zone_A_timed_antifreeze_PUMPo_need;      /* A区定时防冻开外循环泵需求 */
LOG_SYS_EXT BOL fg_zone_A_timed_antifreeze_S3V1_ON_need;    /* A区定时防冻电磁三通阀1保持ON输出需求 */
LOG_SYS_EXT BOL fg_zone_A_timed_antifreeze_COMPf_need;      /* A区定时防冻变频压机需求 */

/* B区定时防冻 */
LOG_SYS_EXT BOL fg_zone_B_timed_antifreeze;                 /* B区定时防冻 */
LOG_SYS_EXT BOL fg_zone_B_timed_antifreeze_PUMPm_need;      /* B区定时防冻开混合水泵需求 */
LOG_SYS_EXT BOL fg_zone_B_timed_antifreeze_S3V3_need;		/* B区定时防冻电磁三通阀3有OFF输出需求 */
LOG_SYS_EXT BOL fg_zone_B_timed_antifreeze_S3V1_ON_need;    /* B区定时防冻电磁三通阀1保持ON输出需求 */
LOG_SYS_EXT BOL fg_zone_B_timed_antifreeze_COMPf_need;      /* B区定时防冻变频压机需求 */

/* 热水定时防冻 */
LOG_SYS_EXT BOL fg_hotw_timed_antifreeze;                   /* 热水定时防冻 */
LOG_SYS_EXT BOL fg_hotw_timed_antifreeze_S3V1_OFF_need;     /* 热水定时防冻电磁三通阀1保持OFF输出需求 */
LOG_SYS_EXT BOL fg_hotw_timed_antifreeze_COMPf_need;        /* 热水定时防冻变频压机需求 */

/* A区定时防冻 */
#define ZONE_A_TAF            (fg_zone_A_timed_antifreeze_PUMPo_need || fg_zone_A_timed_antifreeze)
/* B区定时防冻 */
#define ZONE_B_TAF            (fg_zone_B_timed_antifreeze_PUMPm_need || fg_zone_B_timed_antifreeze)
/* 热水定时防冻 */
#define HOTW_TAF              (fg_hotw_timed_antifreeze_S3V1_OFF_need || fg_hotw_timed_antifreeze)

/* A区防冻 */
#define ZONE_A_AF             (ZONE_A_TAF || GET_AF_STAGE(AF_STAGE_AC))
/* B区防冻 */
#define ZONE_B_AF             (ZONE_B_TAF || GET_AF_STAGE(AF_STAGE_AC))
/* 热水防冻 */
#define HOTW_AF               (HOTW_TAF || GET_AF_STAGE(AF_STAGE_HOTW))

/* 防冻调频目标温度 */
#define AF_SV_TEMP_FREQ       (MAX(val_anti_comp_off+50, 300))
#define AF_HOTW_ENTER_TEMP    (val_anti_comp_off+50)

/* 防冻标志汇总 */
#define FG_AF_ALL (fg_idle_sta_antifreeze           \
                   || fg_run_sta_ac_antifreeze      \
                   || fg_run_sta_hotw_antifreeze    \
                   || ZONE_A_TAF                    \
                   || ZONE_B_TAF                    \
                   || HOTW_TAF                      \
                   )

/* 水泵调试 */
LOG_SYS_EXT U08 debug_mode; /* 水泵调试模式 */

/* 空气排空试运行 */
enum
{
    TEST_RUN_AIR_EMPTY,     /* 空气排空试运行 */
    TEST_RUN_PUMP,          /* 水泵试运行 */
    TEST_RUN_COOL_MODE,     /* 制冷试运行 */
    TEST_RUN_HEAT_MODE,     /* 制热试运行 */
    TEST_RUN_HOTW_MODE,     /* 制热水试运行 */
    TEST_RUN_MAX,
};
LOG_SYS_EXT U16 fg_test_run[_DIVUCARRY(TEST_RUN_MAX, 16)];  /* 进入试运行标志 */
LOG_SYS_EXT U16 cnt_test_run[TEST_RUN_MAX];                 /* 试运行计时 */
#define cnt_test_run_air_empty              (cnt_test_run[TEST_RUN_AIR_EMPTY])
#define cnt_test_run_pump                   (cnt_test_run[TEST_RUN_PUMP])

enum
{
    TEST_AIR_EMPTY_S3V1_ON, /* 开S3V1 */
    TEST_AIR_EMPTY_S3V2_ON, /* 开S3V2 */
    TEST_AIR_EMPTY_S3V3_ON,	/* 开S3V3 */
    TEST_AIR_EMPTY_PUMPF_ON,/* 开PUMPf */
    TEST_AIR_EMPTY_PUMPO_ON,/* 开PUMPo */
    TEST_AIR_EMPTY_PUMPM_ON,/* 开PUMPm */
    TEST_AIR_EMPTY_MAX,
};
LOG_SYS_EXT U16 fg_test_air_empty_on[_DIVUCARRY(TEST_AIR_EMPTY_MAX, 16)];   /* 空气排空试运行开启标志 */

enum
{
    TEST_PUMP_S3V1_ON,      /* 开S3V1 */
    TEST_PUMP_S3V2_ON,      /* 开S3V2 */
    TEST_PUMP_S3V3_ON,      /* 开S3V2 */
    TEST_PUMP_PUMPF_ON,     /* 开PUMPf */
    TEST_PUMP_PUMPO_ON,     /* 开PUMPo */
    TEST_PUMP_PUMPM_ON,     /* 开PUMPm */
    TEST_PUMP_CHECK_LDIW,   /* 检测LDIw */
    TEST_PUMP_MAX,
};
LOG_SYS_EXT U16 fg_test_pump_on[_DIVUCARRY(TEST_PUMP_MAX, 16)];     /* 水泵试运行开启标志 */

#define TEST_RUN_COOL_MODE_SV_COOL           (70)           /* 制冷试运行能调目标温度 */
#define TEST_RUN_HEAT_MODE_SV_HEAT           (350)          /* 制热试运行能调目标温度 */
#define TEST_RUN_HOTW_MODE_SV_HOTW           (550)          /* 制热水试运行能调目标温度 */

LOG_SYS_EXT BOOL fg_machine_need_open;      /* 机组需要开启(停机时开机和防冻时开机使用) */

/* 不能在log_mod、err_mod中使用 */
LOG_SYS_EXT U08 ac_mode;                                    /* 空调运行模式(仅针对A区和B区) */
LOG_SYS_EXT U08 disp_mode[SUB_SYS_TYPE_MAX];                /* 当前显示模式 */
LOG_SYS_EXT U08 ctrl_mode[SUB_SYS_TYPE_MAX];                /* 当前控制模式 */
LOG_SYS_EXT U08 ctrl_mode_dly[SUB_SYS_TYPE_MAX];            /* 当前模式确认延时 */

/* 仅供显示用的模式判断 */
#define DISP_IS_COOL(type)                  (disp_mode[type] == MODE_COOL)	/* 当前是否显示制冷 */
#define DISP_IS_HEAT(type)                  (disp_mode[type] == MODE_HEAT)	/* 当前是否显示制热 */
#define DISP_IS_HOTW(type)                  (disp_mode[type] == MODE_HOTW)	/* 当前是否显示热水 */

/* 供实际控制用的模式判断 */
#define MODE_IS_COOL(type)                  (ctrl_mode[type] == MODE_COOL)  /* 当前控制是否为制冷 */
#define MODE_IS_HEAT(type)                  (ctrl_mode[type] == MODE_HEAT)  /* 当前控制是否为制热 */
#define MODE_IS_HOTW(type)                  (ctrl_mode[type] == MODE_HOTW)  /* 当前控制是否为热水 */
#define MODE_IS_AC(type)                    (MODE_IS_COOL(type) || MODE_IS_HEAT(type))

/* 模式运行 */
#define RUN_COOL(type)                      (STEP_IS_RUN && MODE_IS_COOL(type))	                        /* 制冷运行 */
#define RUN_HEAT(type)                      (STEP_IS_RUN && MODE_IS_HEAT(type))		                    /* 制热运行 */
#define RUN_AC(type)                        (STEP_IS_RUN && (MODE_IS_COOL(type)||MODE_IS_HEAT(type)))	/* 空调运行 */
#define RUN_HOTW(type)                      (STEP_IS_RUN && MODE_IS_HOTW(type))	                        /* 制热水运行 */

/* 能调控制温度所在区间 */
LOG_SYS_EXT U08 cool_area;                  /* 制冷 */
LOG_SYS_EXT U08 heat_area;                  /* 制热 */
LOG_SYS_EXT U08 hotw_area;                  /* 热水 */

/* 模式需求标志 */
LOG_SYS_EXT BOL fg_cool_need;               /* 制冷需求 */
LOG_SYS_EXT BOL fg_heat_need;               /* 制热需求 */
LOG_SYS_EXT BOL fg_hotw_need;               /* 制热水需求 */

/* 空调需求 */
#define FG_AC_NEED                          (fg_cool_need || fg_heat_need)

/* 模式运行时间 */
LOG_SYS_EXT RTIME cool_runtime[SUB_SYS_TYPE_MAX];				/* 制冷运行时间 */
LOG_SYS_EXT RTIME heat_runtime[SUB_SYS_TYPE_MAX];				/* 制热运行时间 */
LOG_SYS_EXT RTIME hotw_runtime;				                    /* 制热水运行时间 */

/* 能量需求标志 */
LOG_SYS_EXT BOL fg_energy_need[SUB_SYS_TYPE_MAX];           /* 子系统能量需求 */

LOG_SYS_EXT BOL fg_tuya_lang_switch;		//语言更新检测使能	
LOG_SYS_EXT U08 cnt_wait_tuya_lang;		//等待涂鸦主界面语言更新

LOG_SYS_EXT BOL fg_tuya_temp_unit_switch;		//温度单位更新检测使能
LOG_SYS_EXT U08 cnt_wait_tuya_temp_unit;		//等待涂鸦温度单位更新

LOG_SYS_EXT U16  HaveCompRuntime;           // 有压机运行的时间

LOG_SYS_EXT BOL fg_warm_complete;               // 预热完成标识
LOG_SYS_EXT BOL fg_warm_quick;                  // 快速预热标识
LOG_SYS_EXT U08 skip_preheat_cnt;              /* 跳过预热计数 */
LOG_SYS_EXT U16 sys_warm_time;                  /* 预热计时 */
LOG_SYS_EXT U16 sys_warm_time_need;             /* 需要预热时间 */
//LOG_SYS_EXT U16 power_down_time;                /* 掉电时间差：分钟 */
LOG_SYS_EXT LOG_RTC power_down_time_last;       /* 上次掉电时间：需确保与存入eeprom的时钟数据类型一致 */


LOG_SYS_EXT U16 cnt_ster_cyc;       // 杀菌周期计时
LOG_SYS_EXT U16 cnt_ster_high_temp; // 杀菌高温段累计计时
LOG_SYS_EXT BOOL fg_enter_floor_dryup;  // 进入地暖烘干功能
//LOG_SYS_EXT BOOL fg_force_ibh;    // 进入地暖烘干功能
//LOG_SYS_EXT BOOL fg_force_tbh;    // 进入地暖烘干功能

//LOG_SYS_EXT BOOL fg_enter_holiday;  // 进入休假模式
//LOG_SYS_EXT I16 holiday_sv; // 休假模式设定温度
//LOG_SYS_EXT U08 holiday_mode;   // 休假模式设定模式

#ifdef USE_AT_SV_FIX
#define EXIT_ABILITY_TEST_HEAT_SV           (620)
#define EXIT_ABILITY_TEST_COOL_SV           (50)
#else
#define EXIT_ABILITY_TEST_HEAT_SV           (sv_heat)
#define EXIT_ABILITY_TEST_COOL_SV           (sv_cool)
#endif

LOG_SYS_EXT I16 room_temp;          // 室内温度
/* ========================================================================================== */
#ifndef USE_QUICK_TIME
#define AC_PUMP_ON_DLY              30          //开空调泵延时
#define AC_PUMP_OFF_DLY             6           //关空调泵延时:系统用，模块不用该宏定义
#define COMP_NEED_ON_TIME           120         //压缩机最少运行
#else
#define AC_PUMP_ON_DLY              (fg_dbg_slow?30:2)           //开空调泵延时
#define AC_PUMP_OFF_DLY             (fg_dbg_slow?6:6)           //关空调泵延时
#define COMP_NEED_ON_TIME           (fg_dbg_slow?120:10)          //压缩机最少运行（测试）
#endif

#define COMP_ON_OFF_SPACE           3           //压缩机启停间隔

#define COOL_LP_DLY                 5       //制冷低压消抖延时
//#define LP_CHECK_DELAY                120     //低压检测延时
//#define ERR_WT_EXIT               (10*5)  //排气保护温差
//#define ERR_DLY                       3       //一般故障延时
//#define ERR_FLUX_DLY              15      //水流不足故障延时

#define INOUT_DIFF      5               // 出回水温差(℃)
#define INOUT_10DIFF    (INOUT_DIFF*10) // 出回水温差(0.1℃ 放大10倍)    

#define CURRENT_DLY                 5       //电流过大开机检测延时   
#define CURRENT_ERR_DLY             2       //开机电流检测延时

#define  PUMP_MODE_ON     0   // 一直运行
#define  PUMP_MODE_INTER  1   // 间歇运行    
#define  PUMP_MODE_MAX    2  

/* 功率模式 */
#define POWER_MODE_STD              0       /* 标准 */
#define POWER_MODE_STR              1       /* 强劲 */
#define POWER_MODE_ECO              2       /* 节能 */
#define POWER_MODE_AUTO             3       /* 自动 */
#define POWER_MODE_MAX              4

LOG_SYS_EXT U08 fg_silent_mode;     // 静音模式
LOG_SYS_EXT U08 silent_mode_level;  // 静音模式级别

#define ModeIsStd       (log_sys_get_power_mode()==POWER_MODE_STD)
#define ModeIsStr       (log_sys_get_power_mode()==POWER_MODE_STR)
#define ModeIsEco       (log_sys_get_power_mode()==POWER_MODE_ECO)

/* ========================================================================================== */
enum STATUS_READY {READY_IDLE, READY_PUMP};
enum STATUS_DELAY { DELAY_IDLE, DELAY_COMP, DELAY_COOL_PUMP,DELAY_PUMP, DELAY_VALVE };
LOG_SYS_EXT U08 status_ready;          /* 准备态 */
LOG_SYS_EXT U08 status_delay;          /* 停机态 */
LOG_SYS_EXT U16 ready_time;            /* 准备态各个阶段的延时 */
LOG_SYS_EXT U08 delay_time;            /* 停机态各个阶段的延时 */

#define STEP_IS_IDLE        (status_run == _IDLE)
#define STEP_IS_READY       (status_run == _READY)
#define STEP_IS_RUN         (status_run == _RUN)
#define STEP_IS_DELAY       (status_run == _DELAY)
#define STEP_IS_ALARM       (status_run == _ALARM)

#define StepIsIdle          (STEP_IS_IDLE)
#define StepIsReady         (STEP_IS_READY)
#define StepIsRun           (STEP_IS_RUN)
#define StepIsDelay         (STEP_IS_DELAY)
#define StepIsAlarm         (STEP_IS_ALARM)

#define MAC_IS_IDLE         (STEP_IS_IDLE || (STEP_IS_READY && status_ready==READY_IDLE))
#define MAC_IS_RUN          ((STEP_IS_READY && status_ready>READY_IDLE) \
                             || STEP_IS_RUN                             \
                             || STEP_IS_DELAY)

#define m_set_input_first(y)        SETBIT(cell[MAX_PCB].cell_input[(y)>>4], (y)&0x0f)
#define m_get_input_first(y)        GETBIT(cell[MAX_PCB].cell_input[(y)>>4], (y)&0x0f)
#define m_set_input(y)              SETBIT(sys_input, (y)&0x0f)
#define m_get_input(y)              GETBIT(sys_input, (y)&0x0f)



#define M_GET_CompMode(pcb_num, comp_num)    ((cell[pcb_num].cell_status[comp_num] >> COMP_MODE) & 0x0007)

LOG_SYS_EXT U08 random_time;            // 随机时间，根据上电后进入控制时的秒数确认；    
LOG_SYS_EXT U08 fg_ac_pump_firstopen;   // 已完成首次开水泵

LOG_SYS_EXT RTIME sys_time;         /* 系统时间 */

#define FREE_ELEC_I_SV_HOTW       (700)
enum
{
    SG_CMD_NULL,
    SG_CMD_1,               /* 智能电网指令1 */
    SG_CMD_2,               /* 智能电网指令2 */
    SG_CMD_3,               /* 智能电网指令3 */
    SG_CMD_4,               /* 智能电网指令4 */
    SG_CMD_MAX,
};
enum
{
    SG_STA_NULL,
    SG_STA_1,               /* 智能电网状态1 */
    SG_STA_2,               /* 智能电网状态2 */
    SG_STA_3,               /* 智能电网状态3 */
    SG_STA_4,               /* 智能电网状态4 */
    SG_STA_MAX,
};
LOG_SYS_EXT U08 smart_grid_cmd;             /* 智能电网指令 */
LOG_SYS_EXT U16 cnt_sg_cmd_on[SG_CMD_MAX];  /* 智能电网指令有效计时 */
LOG_SYS_EXT U16 cnt_sg_sta_off[SG_STA_MAX]; /* 智能电网状态计时 */
LOG_SYS_EXT U08 smart_grid_sta;
LOG_SYS_EXT U08 sg_power_mode;
LOG_SYS_EXT BOOL fg_sg_hotw_priority;
LOG_SYS_EXT BOOL fg_sg_hotw_forbid;

/* 低谷用电强制关闭 */
#define VALLEY_ELEC_FORCE_STOP    (smart_grid_sta==SG_STA_1)
/* 高峰用电限制运行 */
#define PEAK_ELEC_LIMIT_RUN       (smart_grid_sta==SG_STA_2)
/* 免费用电运行 */
#define FREE_ELEC_RUN             (smart_grid_sta==SG_STA_3)
/* 免费用电强制运行 */
#define FREE_ELEC_FORCE_RUN       (smart_grid_sta==SG_STA_4)

enum
{
    SG_ACT_NULL,
    SG_ACT_ALL_FORCE_OFF,	/* 机组强制关闭 */
    SG_ACT_HOTW_FORBID_ON,	/* 禁止热水开机 */
    SG_ACT_HOTW_PRIOR_RUN,	/* 热水已开机，则优先运行 */
    SG_ACT_HOTW_FORCE_ON,	/* 强制热水开机 */
    SG_ACT_MAX,
};
LOG_SYS_EXT U08 smart_grid_act;

LOG_SYS_EXT U16 cnt_hotw_heat_switch_dly;		/* 制热、热水切换延时：倒计时 */
#define SET_HOTW_HEAT_SWITCH_DLY (cnt_hotw_heat_switch_dly = 60)
#define CLR_HOTW_HEAT_SWITCH_DLY (cnt_hotw_heat_switch_dly =  0)
//LOG_SYS_EXT BOOL fg_mode_switching;

/**********************************************************************************/
LOG_SYS_EXT void cmd_comp_quik_stop(U08 pcb_num, U08 comp_num);
LOG_SYS_EXT void cmd_err_reset(void);
LOG_SYS_EXT void cmd_comp_run(U08 pcb_num, U08 comp_num, U08 to_mode);
LOG_SYS_EXT void cmd_systime_save(void);


LOG_SYS_EXT void work_initial(void);

LOG_SYS_EXT U08 OutIsChange(U08 no);
//LOG_SYS_EXT void runtime_initial(void);
//LOG_SYS_EXT void password_initial(void);
//LOG_SYS_EXT void systime_initial(void);
//LOG_SYS_EXT void systime_write_to_eeprom(void);
//LOG_SYS_EXT void mac_attrib_write_to_eeprom(void);
//LOG_SYS_EXT void limit_time_write_to_eeprom(void);
LOG_SYS_EXT BOOL is_thermo_dependece_all(U16 thermo_type, U16 comp_nun_set);

LOG_SYS_EXT void log_sys_init(void);
LOG_SYS_EXT void log_sys(U08 pcb_num);

LOG_SYS_EXT void log_sys_init_work(void);

LOG_SYS_EXT void log_sys_init_idle_sta_antifreeze(void);

LOG_SYS_EXT void menu_pv(void);
LOG_SYS_EXT void menu_work(void);
LOG_SYS_EXT BOL log_sys_have_fan_on(void);
LOG_SYS_EXT U08 log_sys_comp_run_num(U08 type);
LOG_SYS_EXT U08 log_sys_defrost_comp_num(U08 type);
//LOG_SYS_EXT BOL log_sys_is_silent(void);
LOG_SYS_EXT U16 log_sys_warm_time_get(void);
LOG_SYS_EXT U16 log_sys_get_ctrl_object(void);

#define GET_BY_CVS  0    /* 转换值 */
#define GET_BY_FACT 1    /* 实际值 */
LOG_SYS_EXT U08 log_sys_ctrl_mode_get(U08 type);

LOG_SYS_EXT void log_sys_UIRT(void);
LOG_SYS_EXT void log_sys_holiday_on_off(void);
LOG_SYS_EXT BOOL log_sys_have_comp_run(U08 mode, U08 type);
LOG_SYS_EXT BOL log_sys_have_non_defrosting_comp_run(U08 mode, U08 type);
LOG_SYS_EXT BOL log_sys_have_AHS_on(U08 zone);
LOG_SYS_EXT BOL log_sys_ac_have_comp_run(void);
LOG_SYS_EXT BOL log_sys_hotw_have_comp_run(void);
LOG_SYS_EXT BOL log_sys_ac_have_IBH_on(void);
LOG_SYS_EXT BOL log_sys_hotw_have_IBH_on(void);
LOG_SYS_EXT BOL log_sys_cell_pumpf_on(void);
LOG_SYS_EXT BOL log_sys_get_cmd_mux_bit(U08 id);
LOG_SYS_EXT BOL log_sys_have_cell_pump_run(U08 type);
LOG_SYS_EXT U08 log_sys_enegry_add(U08 mode, U08 type);
LOG_SYS_EXT U08 log_sys_get_power_mode(void);

LOG_SYS_EXT void log_sys_get_sv_disp_zone_A(U08 *grp, U08 *idx, I16 *value);
LOG_SYS_EXT void log_sys_get_sv_disp_zone_B(U08 *grp, U08 *idx, I16 *value);
LOG_SYS_EXT void log_sys_get_sv_disp_zone_hot(U08 *grp, U08 *idx, I16 *value);

LOG_SYS_EXT void log_sys_init_idle_sta_antifreeze(void);
LOG_SYS_EXT void log_sys_init_run_sta_antifreeze(void);
LOG_SYS_EXT void log_sys_init_timed_antifreeze(void);
LOG_SYS_EXT BOL log_sys_s3v1_is_hw(void);

/*--------------------------*/
//LOG_SYS_EXT U08 comp_status_debug_ac;   /*  压机状态调试 */
//LOG_SYS_EXT U08 comp_status_debug_wt;   /*  压机状态调试 */
//LOG_SYS_EXT U08 program_run_time;
//LOG_SYS_EXT U08 debug_1;
//LOG_SYS_EXT U16 debug_2;
//LOG_SYS_EXT U16 debug_3;
//LOG_SYS_EXT U16 debug_4;
//LOG_SYS_EXT U16 debug_5;
//LOG_SYS_EXT U16 debug_6[5];
//LOG_SYS_EXT U16 debug[20];    
//LOG_SYS_EXT U16 debug_8[10];
//LOG_SYS_EXT U16 debug_9[10];
//LOG_SYS_EXT U16 cmd_debug[4];
//LOG_SYS_EXT U16 cmd_debug2;
//LOG_SYS_EXT U16 cmd_debug_3;
//LOG_SYS_EXT U16 cmd_debug_4;
//LOG_SYS_EXT I16 anti_freeze_debug1;
//LOG_SYS_EXT I16 anti_freeze_debug2;
//LOG_SYS_EXT I16 anti_freeze_debug3;
/*--------------------------*/

#endif
