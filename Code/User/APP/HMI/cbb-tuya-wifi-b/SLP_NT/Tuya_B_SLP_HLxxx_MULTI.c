/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: Tuya_B_SLP_HLxxx_MULTI.c
 文件描述: (暖通HL系列)涂鸦(B)多联供业务逻辑源文件
 创建人  : 林浩彬
 创建日期: 2020.12.13
*******************************************************************************/

/* ========================================================================================== */
/* 涂 鸦 数 据 通 讯 协 议 相 关 定 义 (以下内容所有程序一致，不可擅自修改!!!!!)              */
/* ========================================================================================== */
/* 涂鸦逻辑业务版本 */
#define TYB_SLP_CORE_VER                 "1.0.0"

/* 面板协议版本 */
#define TYB_SLP_PANEL_VER                "1.0.0"
#define TYB_SLP_PROTOCOL_VER             TYB_SLP_PANEL_VER""TUYA_ARGS

/* 功能点ID(DPID) (有效功能点ID从1开始编号，最大199) */
#define DPID_NULL                       (0)    /* 无效功能点(控制器发给HMI后不会上报模组、可供HMI下发显示屏版本使用) */
#define DPID_SWITCH                     (1)    /* 开关 (支持云定时 & 智能语音) */
#define DPID_MODE                       (2)    /* 设定模式 (支持智能语音) */
#define DPID_TEMP_SET                   (16)   /* 设定温度 (支持智能语音) */
#define DPID_TEMP_CUR                   (24)   /* 当前温度 (支持智能语音) */
#define DPID_VER_INFO                   (101)  /* 版本信息 (显示屏&控制器) */
#define DPID_SYS_INFO                   (102)  /* 系统信息 */
#define DPID_PROTOCOL_VER               (103)  /* 协议版本 */
#define DPID_GENERAL_SET_INFO           (104)  /* 通用设置信息 */
#define DPID_GENERAL_SET_CMD            (105)  /* 通用设置命令 */
#define DPID_COMMON_FUNC_INFO           (106)  /* 常规功能信息 */
#define DPID_COMMON_FUNC_CMD            (107)  /* 常规功能命令 */
#define DPID_EXTEND_FUNC_INFO           (108)  /* 扩展功能信息 */
#define DPID_EXTEND_FUNC_CMD            (109)  /* 扩展功能命令 */

#define DPID_ZONEA_FUNC_INFO            (106)  /* 区域一功能信息 */
#define DPID_ZONEA_FUNC_CMD             (107)  /* 区域一功能命令 */
#define DPID_ZONEB_FUNC_INFO            (108)  /* 区域二功能信息 */
#define DPID_ZONEB_FUNC_CMD             (109)  /* 区域二功能命令 */
#define DPID_DHW_FUNC_INFO              (110)  /* 热水功能信息 */
#define DPID_DHW_FUNC_CMD               (111)  /* 热水功能命令 */

#define DPID_SYS_STAT_INFO_1            (112)  /* 系统状态信息1(原单模块的"系统状态标题1") */
#define DPID_SYS_STAT_INFO_2            (113)  /* 系统状态信息2(原单模块的"系统状态数据") */
#define DPID_SYS_STAT_INFO_3            (114)  /* 系统状态信息3(原单模块的"模块状态标题1") */
#define DPID_SYS_STAT_INFO_4            (115)  /* 系统状态信息4(原单模块的"模块状态标题2") */
#define DPID_SYS_STAT_DATA              (116)  /* 系统状态数据(原单模块的"00#模块状态数据") */
#define DPID_MOD_STAT_CMD               (117)  /* 模块状态传输命令 */
#define DPID_DATA_CHANNEL_01            (118)  /* 数据通道01 */
#define DPID_DATA_CHANNEL_02            (119)  /* 数据通道02 */
#define DPID_DATA_CHANNEL_03            (120)  /* 数据通道03 */
#define DPID_DATA_CHANNEL_04            (121)  /* 数据通道04 */
#define DPID_DATA_CHANNEL_05            (122)  /* 数据通道05 */
#define DPID_DATA_CHANNEL_06            (123)  /* 数据通道06 */
#define DPID_ERR_CUR_INFO               (133)  /* 当前故障信息(注: 当前故障只有标题，没有时间) */
#define DPID_ERR_HIS_INFO               (134)  /* 历史故障信息 */
#define DPID_ELE_STAT_CMD               (136)  /* 用电状态传输命令 */
#define DPID_SYS_STAT_CMD               (137)  /* 系统状态传输命令 */
#define DPID_ERR_HIS_CMD                (139)  /* 历史故障传输命令 */
#define DPID_PARAM_CMD                  (140)  /* 参数传输命令 */

/* 功能点位置索引 */
enum DPID_IDX
{
    PROTOCOL_VER = 0,
    VER_INFO,
    SYS_INFO,
/* 有"通用设置功能" */
#ifdef TUYA_HAS_GEN_SET
    GENERAL_SET_INFO,
    GENERAL_SET_CMD,
#endif

#if 0
    COMMON_FUNC_INFO,
    COMMON_FUNC_CMD,
/* 有"扩展功能" */
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
/* 有"系统状态" */
#ifdef TUYA_USE_SYS_STAT
    SYS_STAT_INFO_1,
    SYS_STAT_INFO_2,
    SYS_STAT_INFO_3,
    SYS_STAT_INFO_4,
    SYS_STAT_DATA,
#endif
/* 有"模块状态" */
#ifdef TUYA_USE_MOD_STAT
    MOD_STAT_CMD,
#endif
/* 有"当前故障" */
#ifdef TUYA_USE_ERR_CUR
    ERR_CUR_INFO,
#endif
/* 有"历史故障" */
#ifdef TUYA_USE_ERR_HIS
    ERR_HIS_INFO,
#endif
/* 有通道传输(有"参数设置"/"模块状态") */
#ifdef TUYA_HAS_TRANS
    DATA_CHANNEL_01,
    DATA_CHANNEL_02,
    DATA_CHANNEL_03,
    DATA_CHANNEL_04,
    DATA_CHANNEL_05,
    DATA_CHANNEL_06,
#endif
/* 有"参数设置" */
#ifdef TUYA_USE_PARAM_SET
    PARAM_CMD,
#endif
/* 有"用电查询" */
#ifdef TUYA_USE_ELE_STAT
    ELE_STA_CMD,
#endif
    DPID_IDX_MAX,
};
/* 功能点属性配置 */
const TUYA_DP_PROPS DP[] =
{
    { DPID_PROTOCOL_VER,     DP_RAW,    DATA_RO, FUNC_CUSTOM,   FREQ_VERY_LOW,  }, /* DPID_103: 协议版本 */
    { DPID_VER_INFO,         DP_RAW,    DATA_RO, FUNC_CUSTOM,   FREQ_VERY_LOW,  }, /* DPID_101: 版本信息 */
    { DPID_SYS_INFO,         DP_RAW,    DATA_RO, FUNC_CUSTOM,   FREQ_NORMAL,    }, /* DPID_102: 系统信息 */
/* 有"通用设置功能" */
#ifdef TUYA_HAS_GEN_SET
    { DPID_GENERAL_SET_INFO, DP_RAW,    DATA_RO, FUNC_CUSTOM,   FREQ_NORMAL,    }, /* DPID_104: 通用设置信息 */
    { DPID_GENERAL_SET_CMD,  DP_RAW,    DATA_WO, FUNC_CUSTOM,   FREQ_VERY_LOW,  }, /* DPID_105: 通用设置命令 */
#endif
    { DPID_ZONEA_FUNC_INFO, DP_RAW,    DATA_RO, FUNC_CUSTOM,   FREQ_VERY_FAST, }, /* DPID_106: 区域一功能信息 */
    { DPID_ZONEA_FUNC_CMD,  DP_RAW,    DATA_WO, FUNC_CUSTOM,   FREQ_VERY_LOW,  }, /* DPID_107: 区域一功能命令 */
    { DPID_ZONEB_FUNC_INFO, DP_RAW,    DATA_RO, FUNC_CUSTOM,   FREQ_VERY_FAST, }, /* DPID_108: 区域二功能信息 */
    { DPID_ZONEB_FUNC_CMD,  DP_RAW,    DATA_WO, FUNC_CUSTOM,   FREQ_VERY_LOW,  }, /* DPID_109: 区域二功能命令 */
    { DPID_DHW_FUNC_INFO,   DP_RAW,    DATA_RO, FUNC_CUSTOM,   FREQ_VERY_FAST, }, /* DPID_110: 热水功能信息 */
    { DPID_DHW_FUNC_CMD,    DP_RAW,    DATA_WO, FUNC_CUSTOM,   FREQ_VERY_LOW,  }, /* DPID_111: 热水功能命令 */

    { DPID_SWITCH,           DP_BOOL,   DATA_RW, FUNC_STANDARD, FREQ_VERY_LOW,  }, /* DPID_1:   开关 */
    { DPID_MODE,             DP_ENUM,   DATA_RW, FUNC_STANDARD, FREQ_VERY_LOW,  }, /* DPID_2:   设定模式 */
    { DPID_TEMP_SET,         DP_NUMBER, DATA_RW, FUNC_STANDARD, FREQ_VERY_LOW,  }, /* DPID_16:  设定温度 */
    { DPID_TEMP_CUR,         DP_NUMBER, DATA_RO, FUNC_STANDARD, FREQ_VERY_FAST, }, /* DPID_24:  当前温度 */
/* 有"系统状态" */
#ifdef TUYA_USE_SYS_STAT
    { DPID_SYS_STAT_INFO_1,  DP_RAW,    DATA_RO, FUNC_CUSTOM,   FREQ_LOW,       }, /* DPID_112: 系统状态信息1 */
    { DPID_SYS_STAT_INFO_2,  DP_RAW,    DATA_RO, FUNC_CUSTOM,   FREQ_LOW,       }, /* DPID_113: 系统状态信息2 */
    { DPID_SYS_STAT_INFO_3,  DP_RAW,    DATA_RO, FUNC_CUSTOM,   FREQ_LOW,       }, /* DPID_114: 系统状态信息3 */
    { DPID_SYS_STAT_INFO_4,  DP_RAW,    DATA_RO, FUNC_CUSTOM,   FREQ_LOW,       }, /* DPID_115: 系统状态信息4 */
    { DPID_SYS_STAT_DATA,    DP_RAW,    DATA_RO, FUNC_CUSTOM,   FREQ_FAST,      }, /* DPID_116: 系统状态数据 */
#endif
/* 有"模块状态" */
#ifdef TUYA_USE_MOD_STAT
    { DPID_MOD_STAT_CMD,     DP_RAW,    DATA_WO, FUNC_CUSTOM,   FREQ_VERY_LOW,  }, /* DPID_117: 模块状态传输命令 */
#endif
/* 有"当前故障" */
#ifdef TUYA_USE_ERR_CUR
    { DPID_ERR_CUR_INFO,     DP_RAW,    DATA_RO, FUNC_CUSTOM,   FREQ_LOW,       }, /* DPID_133: 当前故障标题 */
#endif
/* 有"历史故障" */
#ifdef TUYA_USE_ERR_HIS
    { DPID_ERR_HIS_INFO,     DP_RAW,    DATA_RO, FUNC_CUSTOM,   FREQ_NORMAL,    }, /* DPID_134: 历史故障标题 */
    { DPID_ERR_HIS_CMD,      DP_RAW,    DATA_RO, FUNC_CUSTOM,   FREQ_VERY_LOW,  }, /* DPID_139: 历史故障传输命令 */
#endif
/* 有通道传输(有"参数设置"/"模块状态") */
#ifdef TUYA_HAS_TRANS
    { DPID_DATA_CHANNEL_01,  DP_RAW,    DATA_RO, FUNC_CHANNEL,  FREQ_INFINITE,  }, /* DPID_118: 数据通道01 */
    { DPID_DATA_CHANNEL_02,  DP_RAW,    DATA_RO, FUNC_CHANNEL,  FREQ_INFINITE,  }, /* DPID_119: 数据通道02 */
    { DPID_DATA_CHANNEL_03,  DP_RAW,    DATA_RO, FUNC_CHANNEL,  FREQ_INFINITE,  }, /* DPID_120: 数据通道03 */
    { DPID_DATA_CHANNEL_04,  DP_RAW,    DATA_RO, FUNC_CHANNEL,  FREQ_INFINITE,  }, /* DPID_121: 数据通道04 */
    { DPID_DATA_CHANNEL_05,  DP_RAW,    DATA_RO, FUNC_CHANNEL,  FREQ_INFINITE,  }, /* DPID_122: 数据通道05 */
    { DPID_DATA_CHANNEL_06,  DP_RAW,    DATA_RO, FUNC_CHANNEL,  FREQ_INFINITE,  }, /* DPID_123: 数据通道06 */
#endif
/* 有"参数设置" */
#ifdef TUYA_USE_PARAM_SET
    { DPID_PARAM_CMD,        DP_RAW,    DATA_WO, FUNC_CUSTOM,   FREQ_VERY_LOW,  }, /* DPID_140: 参数传输命令 */
#endif
/* 有"用电查询" */
#ifdef TUYA_USE_ELE_STAT
    { DPID_ELE_STAT_CMD,     DP_RAW,    DATA_WO, FUNC_CUSTOM,   FREQ_VERY_LOW,  }, /* DPID_136: 用电传输命令 */
#endif
};
#define DPID_USED                       (sizeof(DP)/sizeof(TUYA_DP_PROPS))    /* 已使用的功能点个数 */

/* 功能点上报相关标志 */
U16 fg_dpid_upload_pri_high[_DIVUCARRY(DPID_USED, 16)];       /* 高优先级上报标志 */
U16 fg_dpid_upload_pri_low[_DIVUCARRY(DPID_USED, 16)];        /* 低优先级上报标志 */

/* 字段功能定义 */
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
    FIELD_FUNC_MAX,              /* 注: 请在FIELD_MAX前添加新的字段功能定义 */
};

/* 系统信息字段功能定义 */
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
    FIELD_SYS_INFO_MAX,              /* 注: 请在FIELD_MAX前添加新的字段功能定义 */
};


/* 区域字段功能定义 */
enum DP_FIELD_AREA_FUNC_ID
{
    FIELD_AREA_SWITCH   	 	= 1,
    FIELD_AREA_TEMP_SET      	= 2,
    FIELD_AREA_TEMP_CUR      	= 3,
	FIELD_AREA_MODE_SET			= 4,
	FIELD_AREA_MODE_CUR			= 5,
    FIELD_AREA_CTRL_WAY      	= 6,	//控制方式
    FIELD_AREA_CTRL_obj   		= 7,	//控制对象

    FIELD_AREA_COMP_STA      	= 8,
    FIELD_AREA_DEFRO_STA        = 9,
    FIELD_AREA_ANFREZ_STA     	= 10,	//防冻状态
    FIELD_AREA_OUTHEAT_STA      = 11,	//外部热源
    FIELD_AREA_STERILIZE_STA    = 12, 	//杀菌
    FIELD_AREA_AUXHEAT_STA      = 13,	//辅助电加热
    FIELD_AREA_TANKHEAT_STA     = 14,	//水箱电加热
    FIELD_AREA_SILENT_STA		= 15,	//静音模式
    FIELD_AREA_ENESAVING_STA 	= 16,	//节能模式
    FIELD_AREA_HOLIDAY_STA 	 	= 17,	//假日模式
    FIELD_AREA_SOLAR_STA 		= 18,	//太阳能

    FIELD_AREA_MAX,              			/* 注: 请在FIELD_MAX前添加新的字段功能定义 */
};

/* 旧值存储变量分配(可根据需要增加,注意切勿增加像"当前温度"这类会频繁变化的数据)*/
enum DP_FIELD_DATA_PREV
{
    SWITCH_IDX        = 0,  /* 开关机 */
    TEMP_SET_IDX      = 1,  /* 设定温度 */
    MODE_SET_IDX      = 2,  /* 设定模式 */
    MODE_CUR_IDX      = 3,  /* 当前模式 */
    RUN_STATUS_IDX    = 4,  /* 运行状态 */
    ERR_ALERT_IDX     = 5,  /* 错误告警 */
    CUR_ERR_NUM_IDX   = 6,  /* 当前故障数 */
    SPLASH_IDX        = 7,  /* 启动页 */
    CHILDLOCK_IDX     = 8,  /* 童锁 */
    LANGUAGE_IDX      = 9,  /* 语言 */
    WIND_SET_IDX      = 10, /* 设定风速 */
    HUMI_SET_IDX      = 11, /* 设定湿度 */
    HOT_WATER_SET_IDX = 12, /* 热水设定温度 */

    /* 区域一 */
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

	/* 区域二 */
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

	/* 热水 */
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

    DATA_PREV_MAX,          /* 最大旧值个数 */
};
I16 tyb_slp_data_prev[DATA_PREV_MAX];                                  /* 旧值存储数组(用于与新值进行比较，帮助判断是否需要主动发送) */

/* ========================================================================================== */
/* 涂 鸦 数 据 字 段 填 充 函 数              */
/* ========================================================================================== */
/* 有"缓冲区超界保护" */
#ifdef TUYA_USE_BUF_SAFETY
/*******************************************************************************
 函 数: TYB_create_bool_field()
 功 能: 构建布尔型字段，带长度预判
 参 数: buf--字符串缓存区;
        acc_len--当前累计长度;
        func_attr--字段功能属性;
        func_id--字段功能ID;
        field_type--字段类型;
        title--标题字符串ID;
        value--当前数值;
 返 回: 空
 其 它: 空
*******************************************************************************/
void TYB_create_bool_field(U08 *buf, U08 *acc_len, U08 func_attr, U08 func_id, U08 field_type, U16 title, BOOL value)
{
    U08 i;
    U08 buf_len = 0;
    //U08 useless_cache[30];    /* 单次最大支持15个中文字符或30个英文字符 */
    U16 acc_bytes = 0;
    U16 *src_buf = NULL;
    DP_FIELD_BOOL field;

    //U08 title_msg_len = TYB_SLP_GET_CODE_STR(useless_cache, title);

    /* 计算拼接后的总长度 */
    acc_bytes = *acc_len + DP_FIELD_BOOL_BYTES;

    /* 已存在异常或剩余空间不够，则中止拼接 */
    if (acc_bytes > RAW_DATA_BYTES)
    {
        return;
    }
    else
    {
        MEMCLR(&field, DP_FIELD_BOOL_BYTES);

        /* 数据更新 */
        field.type = (field_type == FIELD_BOOL) ? MAKEWORD(FIELD_BOOL, FIELD_ENABLE) : MAKEWORD(FIELD_BOOL, FIELD_DISABLED);
        field.props = MAKEWORD(func_id, func_attr);
        field.tittle_msgid = title;
        field.value = value;

        /* 赋值 */
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
 函 数: TYB_create_number_field()
 功 能: 构建数值型字段，带长度预判
 参 数: buf--字符串缓存区;
        acc_len--当前累计长度;
        func_attr--字段功能属性;
        func_id--字段功能ID;
        field_type--字段类型;
        title--标题字符串ID;
        attr--当前数值属性;
        value--当前数值;
        max--最大值;
        min--最小值;
 返 回: 空
 其 它: 空
*******************************************************************************/
void TYB_create_number_field(U08 *buf, U08 *acc_len, U08 func_attr, U08 func_id, U08 field_type, U16 title, U16 attr, I16 value, I16 max, I16 min)
{
    U08 i;
    U08 buf_len = 0;
    //U08 useless_cache[30];    /* 单次最大支持15个中文字符或30个英文字符 */
    U16 acc_bytes = 0;
    U16 *src_buf = NULL;
    DP_FIELD_NUMBER field;

    //U08 title_msg_len = TYB_SLP_GET_CODE_STR(useless_cache, title);

    /* 计算拼接后的总长度 */
    acc_bytes = *acc_len + DP_FIELD_NUMBER_BYTES;

    /* 已存在异常或剩余空间不够，则中止拼接 */
    if (acc_bytes > RAW_DATA_BYTES)
    {
        return;
    }
    else
    {
        MEMCLR(&field, DP_FIELD_NUMBER_BYTES);

        /* 数据更新 */
        field.type = (field_type == FIELD_NUMBER) ? MAKEWORD(FIELD_NUMBER, FIELD_ENABLE) : MAKEWORD(FIELD_NUMBER, FIELD_DISABLED);
        field.props = MAKEWORD(func_id, func_attr);
        field.attr = attr;
        field.value = value;
        field.max = max;
        field.min = min;
        field.tittle_msgid = title;

        /* 赋值 */
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
 函 数: TYB_create_enum_field()
 功 能: 构建枚举型字段，带长度预判
 参 数: buf--字符串缓存区;
        acc_len--当前累计长度;
        func_attr--字段功能属性;
        func_id--字段功能ID;
        field_type--字段类型;
        title--标题字符串ID;
        range--一组枚举字符串;
        range_len--枚举字符串数量;
        value--当前数值;
 返 回: 空
 其 它: 空
*******************************************************************************/
void TYB_create_enum_field(U08 *buf, U08 *acc_len, U08 func_attr, U08 func_id, U08 field_type, U16 title, U16 *range, U08 range_len, I16 value)
{
    U08 i;
    U08 buf_len = 0;
    //U08 useless_cache[30];    /* 单次最大支持15个中文字符或30个英文字符 */
    U16 acc_bytes = 0;
    U16 *src_buf = NULL;
    DP_FIELD_ENUM field;

    //U08 title_msg_len = TYB_SLP_GET_CODE_STR(useless_cache, title);

    /* 计算拼接后的总长度 */
    acc_bytes = *acc_len + DP_FIELD_ENUM_BYTES + range_len;

    /* 已存在异常或剩余空间不够，则中止拼接 */
    if (acc_bytes > RAW_DATA_BYTES)
    {
        return;
    }
    else
    {
        MEMCLR(&field, DP_FIELD_ENUM_BYTES);

        /* 数据更新 */
        field.type = (field_type == FIELD_ENUM) ? MAKEWORD(FIELD_ENUM, FIELD_ENABLE) : MAKEWORD(FIELD_ENUM, FIELD_DISABLED);
        field.props = MAKEWORD(func_id, func_attr);
        field.value = value;
        field.tittle_msgid = title;
        field.range_len = range_len;

        /* 赋值 */
        src_buf = (U16*)&field;
        for (i = 0; i < DP_FIELD_ENUM_SIZE; i++)
        {
            buf[buf_len++] = HIBYTE(src_buf[i]);
            buf[buf_len++] = LOBYTE(src_buf[i]);
        }

        /* 填充枚举值 */
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
 函 数: TYB_create_ro_num_field()
 功 能: 构建只读数值型字段，带长度预判
 参 数: buf--字符串缓存区;
        acc_len--当前累计长度;
        func_attr--字段功能属性;
        func_id--字段功能ID;
        field_type--字段类型;
        title--标题字符串ID;
        attr--当前数值属性;
        value--当前数值;
 返 回: 空
 其 它: 空
*******************************************************************************/
void TYB_create_ro_num_field(U08 *buf, U08 *acc_len, U08 func_attr, U08 func_id, U08 field_type, U16 title, U16 attr, I16 value)
{
    U08 i;
    U08 buf_len = 0;
    //U08 useless_cache[30];   /* 单次最大支持15个中文字符或30个英文字符 */
    U16 acc_bytes = 0;
    U16 *src_buf = NULL;
    DP_FIELD_RO_NUM field;

    //U08 title_msg_len = TYB_SLP_GET_CODE_STR(useless_cache, title);

    /* 计算拼接后的总长度 */
    acc_bytes = *acc_len + DP_FIELD_RO_NUM_BYTES;
    //acc_bytes += title_msg_len;

    /* 已存在异常或剩余空间不够，则中止拼接 */
    if (acc_bytes > RAW_DATA_BYTES)
    {
        return;
    }
    else
    {
        MEMCLR(&field, DP_FIELD_RO_NUM_BYTES);

        /* 数据更新 */
        field.type = (field_type == FIELD_RO_NUM) ? MAKEWORD(FIELD_RO_NUM, FIELD_ENABLE) : MAKEWORD(FIELD_RO_NUM, FIELD_DISABLED);
        field.props = MAKEWORD(func_id, func_attr);
        field.attr = attr;
        field.value = value;
        field.tittle_msgid = title;

        /* 赋值 */
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
 函 数: TYB_create_ro_msg_field()
 功 能: 构建只读字符串型字段，带长度预判
 参 数: buf--字符串缓存区;
        acc_len--当前累计长度;
        func_attr--字段功能属性;
        func_id--字段功能ID;
        field_type--字段类型;
        title--标题字符串ID;
        msg--当前字符串ID;
 返 回: 空
 其 它: 空
*******************************************************************************/
void TYB_create_ro_msg_field(U08 *buf, U08 *acc_len, U08 func_attr, U08 func_id, U08 field_type, U16 title, U16 msg)
{
    U08 i;
    U08 buf_len = 0;
    //U08 useless_cache[30];    /* 单次最大支持15个中文字符或30个英文字符 */
    U16 acc_bytes = 0;
    U16 *src_buf = NULL;
    DP_FIELD_RO_MSG field;

    //U08 title_msg_len = TYB_SLP_GET_CODE_STR(useless_cache, title);
    //U08 msg_len = TYB_SLP_GET_CODE_STR(useless_cache, msg);

    /* 计算拼接后的总长度 */
    acc_bytes = *acc_len + DP_FIELD_RO_MSG_BYTES;

    //acc_bytes += title_msg_len;
    //acc_bytes += msg_len;

    /* 已存在异常或剩余空间不够，则中止拼接 */
    if (acc_bytes > RAW_DATA_BYTES)
    {
        return;
    }
    else
    {
        MEMCLR(&field, DP_FIELD_RO_MSG_BYTES);

        /* 数据更新 */
        field.type = (field_type == FIELD_RO_MSG) ? MAKEWORD(FIELD_RO_MSG, FIELD_ENABLE) : MAKEWORD(FIELD_RO_MSG, FIELD_DISABLED);
        field.props = MAKEWORD(func_id, func_attr);
        field.tittle_msgid = title;
        field.content_msgid = msg;

        /* 赋值 */
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
 函 数: TYB_create_one_msg_field()
 功 能: 构建单字符串字段，带长度预判
 参 数: buf--字符串缓存区;
        acc_len--当前累计长度;
        content--字符串;
        content_size--字符串长度(字节数);
 返 回: 空
 其 它: 空
*******************************************************************************/
void TYB_create_one_msg_field(far U08 *buf, far U08 *acc_len, far U08 *content, U08 content_size)
{
    U08 buf_len = 0;
    U16 acc_bytes = 0;

    /* 计算拼接后的总长度 */
    acc_bytes = *acc_len + DP_FIELD_ONE_MSG_HEADER_BYTES;
    acc_bytes = *acc_len + content_size;

    /* 已存在异常或剩余空间不够，则中止拼接 */
    if (acc_bytes > RAW_DATA_BYTES)
    {
        return;
    }
    else
    {
        acc_bytes = 0;
        if (content != PNULL)
        {
            /* 填充数据 */
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
 函 数: TYB_create_one_num_field()
 功 能: 构建单数值字段，带长度预判
 参 数: buf--字符串缓存区;
        acc_len--累计长度;
        value--数据;
        func_attr--字段功能属性;
        func_id--字段功能ID;
 返 回: 空
 其 它: 空
*******************************************************************************/
void TYB_create_one_num_field(U08 *buf, U08 *acc_len, I16 value, U08 func_attr, U08 func_id )
{
    U08 buf_len = 0;
    U16 acc_bytes = 0;
	U16 props;
	props = MAKEWORD(func_id, func_attr);
    acc_bytes = *acc_len + DP_FIELD_ONE_NUM_BYTES;

    /* 已存在异常或剩余空间不够，则中止拼接 */
    if (acc_bytes > RAW_DATA_BYTES)
    {
        return;
    }
    else
    {
        /* 填充数据 */
        buf[buf_len++] = FIELD_ONE_NUM;
        buf[buf_len++] = FIELD_ENABLE;

	    /* 字段属性 */
	    buf[buf_len++] = HIBYTE(props);
	    buf[buf_len++] = LOBYTE(props);

        buf[buf_len++] = HIBYTE(value);
        buf[buf_len++] = LOBYTE(value);

        *acc_len += buf_len;
    }
}
#else
/*******************************************************************************
 函 数: TYB_create_bool_field()
 功 能: 构建布尔型字段，带长度预判
 参 数: buf--字符串缓存区;
        acc_len--当前累计长度;
        func_attr--字段功能属性;
        func_id--字段功能ID;
        field_type--字段类型;
        title--标题字符串ID;
        value--当前数值;
 返 回: 空
 其 它: 空
*******************************************************************************/
void TYB_create_bool_field(U08 *buf, U08 *acc_len, U08 func_attr, U08 func_id, U08 field_type, U16 title, BOOL value)
{
    U08 buf_len = 0;
    U08 title_len;
    U16 type, props;

    title_len = 0;
    type = (field_type == FIELD_BOOL) ? MAKEWORD(FIELD_BOOL, FIELD_ENABLE) : MAKEWORD(FIELD_BOOL, FIELD_DISABLED);
    props = MAKEWORD(func_id, func_attr);

    /* 字段类型 */
    buf[buf_len++] = HIBYTE(type);
    buf[buf_len++] = LOBYTE(type);

    /* 字段属性 */
    buf[buf_len++] = HIBYTE(props);
    buf[buf_len++] = LOBYTE(props);

    /* 当前值 */
    buf[buf_len++] = 0;
    buf[buf_len++] = value;

    /* 标题字符串 */
    title_len = TYB_SLP_GET_CODE_STR(&buf[buf_len+2], title);

    /* 标题字符串长度 */
    buf[buf_len++] = HIBYTE(title_len);
    buf[buf_len++] = LOBYTE(title_len);
    buf_len += title_len;

    *acc_len += buf_len;
}


/*******************************************************************************
 函 数: TYB_create_number_field()
 功 能: 构建数值型字段，带长度预判
 参 数: buf--字符串缓存区;
        acc_len--当前累计长度;
        func_attr--字段功能属性;
        func_id--字段功能ID;
        field_type--字段类型;
        title--标题字符串ID;
        attr--当前数值属性;
        value--当前数值;
        max--最大值;
        min--最小值;
 返 回: 空
 其 它: 空
*******************************************************************************/
void TYB_create_number_field(U08 *buf, U08 *acc_len, U08 func_attr, U08 func_id, U08 field_type, U16 title, U16 attr, I16 value, I16 max, I16 min)
{
    U08 buf_len = 0;
    U08 title_len;
    U16 type, props;

    title_len = 0;
    type = (field_type == FIELD_NUMBER) ? MAKEWORD(FIELD_NUMBER, FIELD_ENABLE) : MAKEWORD(FIELD_NUMBER, FIELD_DISABLED);
    props = MAKEWORD(func_id, func_attr);

    /* 字段类型 */
    buf[buf_len++] = HIBYTE(type);
    buf[buf_len++] = LOBYTE(type);

    /* 字段属性 */
    buf[buf_len++] = HIBYTE(props);
    buf[buf_len++] = LOBYTE(props);

    /* 属性 */
    buf[buf_len++] = HIBYTE(attr);
    buf[buf_len++] = LOBYTE(attr);

    /* 当前值 */
    buf[buf_len++] = HIBYTE(value);
    buf[buf_len++] = LOBYTE(value);

    /* 最大值 */
    buf[buf_len++] = HIBYTE(max);
    buf[buf_len++] = LOBYTE(max);

    /* 最小值 */
    buf[buf_len++] = HIBYTE(min);
    buf[buf_len++] = LOBYTE(min);

    /* 标题字符串 */
    title_len = TYB_SLP_GET_CODE_STR(&buf[buf_len+2], title);

    /* 标题字符串长度 */
    buf[buf_len++] = HIBYTE(title_len);
    buf[buf_len++] = LOBYTE(title_len);
    buf_len += title_len;

    *acc_len += buf_len;
}


/*******************************************************************************
 函 数: TYB_create_enum_field()
 功 能: 构建枚举型字段，带长度预判
 参 数: buf--字符串缓存区;
        acc_len--当前累计长度;
        func_attr--字段功能属性;
        func_id--字段功能ID;
        field_type--字段类型;
        title--标题字符串ID;
        range--一组枚举字符串;
        range_len--枚举字符串数量;
        value--当前数值;
 返 回: 空
 其 它: 空
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

    /* 字段类型 */
    buf[buf_len++] = HIBYTE(type);
    buf[buf_len++] = LOBYTE(type);

    /* 字段属性 */
    buf[buf_len++] = HIBYTE(props);
    buf[buf_len++] = LOBYTE(props);

    /* 当前值 */
    buf[buf_len++] = HIBYTE(value);
    buf[buf_len++] = LOBYTE(value);

    /* 标题字符串 */
    title_len = TYB_SLP_GET_CODE_STR(&buf[buf_len+4], title);

    /* 标题字符串长度 */
    buf[buf_len++] = HIBYTE(title_len);
    buf[buf_len++] = LOBYTE(title_len);

    /* 枚举总数 */
    buf[buf_len++] = HIBYTE(range_len);
    buf[buf_len++] = LOBYTE(range_len);
    buf_len += title_len;

    /* 枚举字符串 */
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
 函 数: TYB_create_ro_num_field()
 功 能: 构建只读数值型字段，带长度预判
 参 数: buf--字符串缓存区;
        acc_len--当前累计长度;
        func_attr--字段功能属性;
        func_id--字段功能ID;
        field_type--字段类型;
        title--标题字符串ID;
        attr--当前数值属性;
        value--当前数值;
 返 回: 空
 其 它: 空
*******************************************************************************/
void TYB_create_ro_num_field(U08 *buf, U08 *acc_len, U08 func_attr, U08 func_id, U08 field_type, U16 title, U16 attr, I16 value)
{
    U08 buf_len = 0;
    U08 title_len;
    U16 type, props;

    title_len = 0;
    type = (field_type == FIELD_RO_NUM) ? MAKEWORD(FIELD_RO_NUM, FIELD_ENABLE) : MAKEWORD(FIELD_RO_NUM, FIELD_DISABLED);
    props = MAKEWORD(func_id, func_attr);

    /* 字段类型 */
    buf[buf_len++] = HIBYTE(type);
    buf[buf_len++] = LOBYTE(type);

    /* 字段属性 */
    buf[buf_len++] = HIBYTE(props);
    buf[buf_len++] = LOBYTE(props);

    /* 属性 */
    buf[buf_len++] = HIBYTE(attr);
    buf[buf_len++] = LOBYTE(attr);

    /* 当前值 */
    buf[buf_len++] = HIBYTE(value);
    buf[buf_len++] = LOBYTE(value);

    /* 标题字符串 */
    title_len = TYB_SLP_GET_CODE_STR(&buf[buf_len+2], title);

    /* 标题字符串长度 */
    buf[buf_len++] = HIBYTE(title_len);
    buf[buf_len++] = LOBYTE(title_len);
    buf_len += title_len;

    *acc_len += buf_len;
}


/*******************************************************************************
 函 数: TYB_create_ro_msg_field()
 功 能: 构建只读字符串型字段，带长度预判
 参 数: buf--字符串缓存区;
        acc_len--当前累计长度;
        func_attr--字段功能属性;
        func_id--字段功能ID;
        field_type--字段类型;
        title--标题字符串ID;
        msg--当前字符串ID;
 返 回: 空
 其 它: 空
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

    /* 字段类型 */
    buf[buf_len++] = HIBYTE(type);
    buf[buf_len++] = LOBYTE(type);

    /* 字段属性 */
    buf[buf_len++] = HIBYTE(props);
    buf[buf_len++] = LOBYTE(props);

    /* 标题字符串 */
    title_len = TYB_SLP_GET_CODE_STR(&buf[buf_len+4], title);

    /* 信息字符串 */
    msg_len = TYB_SLP_GET_CODE_STR(&buf[buf_len+4+title_len], msg);

    /* 标题字符串长度 */
    buf[buf_len++] = HIBYTE(title_len);
    buf[buf_len++] = LOBYTE(title_len);

    /* 信息字符串长度 */
    buf[buf_len++] = HIBYTE(msg_len);
    buf[buf_len++] = LOBYTE(msg_len);

    buf_len = buf_len + title_len + msg_len;

    *acc_len += buf_len;
}


/*******************************************************************************
 函 数: TYB_create_one_msg_field()
 功 能: 构建单字符串字段，带长度预判
 参 数: buf--字符串缓存区;
        acc_len--当前累计长度;
        content--字符串;
        content_size--字符串长度(字节数);
 返 回: 空
 其 它: 空
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
 函 数: TYB_create_one_num_field()
 功 能: 构建单数值字段，带长度预判
 参 数: buf--字符串缓存区;
        acc_len--累计长度;
        value--数据;
 返 回: 空
 其 它: 空
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

/* 显示"显示屏版本号" */
#if (TYBC_DISP_HMI_VER_EN == TRUE)
#ifndef MAX_VER_BYTE
#define MAX_VER_BYTE          (32)                                /* 版本号字节数 */
#endif
U08 tyb_hmi_ver[MAX_VER_BYTE];                                     /* 显示屏版本 */
#if (TYBC_STR_CODE_TYPE == UNI_CODE)
U08 tyb_hmi_ver_unicode[MAX_VER_BYTE*2];
U08 tyb_bkbd_ver_unicode[MAX_VER_BYTE*2];
#endif
#endif

/* 有"状态查询" */
#ifdef TUYA_HAS_STAT
U16 fg_upload_pv[_DIVUCARRY(TYBC_PV_MAX, 16)];                     /* 模拟量上报标志 */
#endif

#ifdef TUYA_USE_FAC_PWD
#if (TYBC_STR_CODE_TYPE == UNI_CODE)
U08 pass_word_unicode[MAX_PASS_WORD*2];
#endif
#endif

/* 使用"内码" */
#if (TYBC_STR_CODE_TYPE == P_CODE)
#define MSG_IDX_END                          (MSG_IDX_MAX)
/*******************************************************************************
 函 数: TYB_SLP_get_PUNP_code_str()
 功 能: 根据字符串索引, 获取字符串内码数组，去除末尾空格后拷贝到字符串缓冲区。
 参 数: buf--字符串缓存区;
        str_idx--字符串索引;
 返 回: 拷贝字节数(除去'\0'及末尾全空格,若只有一个空格情况时除外)
 其 它: 如果字符串中只有一个空格字符，则空格保留
*******************************************************************************/
U08 TYB_SLP_get_PUNP_code_str(U08* buf, U16 str_idx)
{
    const U08* str_ptr;
    const U08* ptr_bgn;
    U08 byte;
    U08 space_cnt = 0;
    U08 len = 0;
    BOOL fg_trim_end_space;

    /* 防超界 */
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

        if (byte >= 0x80)  /* 按双字节处理 */
        {
            buf[len++] = byte;
            buf[len++] = *str_ptr++;
        }
        else  /* 单字节处理 */
        {
            /* 检测到空格字符 */
            if (byte == ' ')
            {
                /* 遍历至末尾，检测是否为全空格 */
                while (*ptr_bgn != '\0')
                {
                    /* 后面存在非空格字符，则需要保留此空格 */
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
            else  /* 非空格字符 */
            {
                buf[len++] = byte;
            }
        }
    }
    return len;
}
#define TYB_SLP_GET_CODE_STR(buf, msg_id)    (TYB_SLP_get_PUNP_code_str(buf, msg_id))
/* 使用"GBK码" */
#elif (TYBC_STR_CODE_TYPE == GBK_CODE)
#define MSG_IDX_BGN                          (MSG_IDX_OFFSET)
#define MSG_IDX_END                          (MSG_MAX)         /* MSG_MAX 定义为 xxx(彩屏字符串最后一个)+20000 */
/*******************************************************************************
 函 数: TYB_SLP_get_GBK_code_str()
 功 能: 根据字符串索引, 获取字符串内码数组，并拷贝到字符串缓冲区
 参 数: buf--字符串缓存区;
        str_idx--字符串索引;
 返 回: 拷贝字节数(除去'\0')
 其 它: 空
*******************************************************************************/
U08 TYB_SLP_get_GBK_code_str(U08* buf, U16 str_idx)
{
    const U08* str_ptr = PNULL;
    U08 byte;
    U08 len = 0;

    /* 防超界 */
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
 函 数: TY_get_UNI_code_str()
 功 能: 根据字符串索引, 获取字符串内码数组，并拷贝到字符串缓冲区
 参 数: 2个
 @ buf--字符串缓存区
 @ str_idx--字符串索引
 返 回: 拷贝字节数(除去'\0')
 其 它: 空
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
		if (len >= 2*97)		//已经超出97个字符，截断后面字符并用...代替
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
 函 数: TYB_SLP_ascii_to_unicode()
 功 能: 转换单字节ASCII为Unicode，并拷贝到字符串缓冲区
 参 数: 1个
 @ buf--字符串缓存区
 返 回: 转换后字节数
 其 它: 空
*******************************************************************************/
U08 TYB_SLP_ascii_to_unicode(U08* buf_unicode, U08* buf_ascii, U08 max_length)
{
	U16 len = 0;
	U16 i=0;

	while (buf_ascii[i] != '\0' && len < max_length)		//密码有空字符，不能用该函数
	{
		buf_unicode[len++] = 0x00;			//高8位为0x00
		buf_unicode[len++] = buf_ascii[i];	//低8位原始数据
		i++;
	}
	return len;
}
#endif


/* 有通道传输(有"参数设置"/"模块状态") */
#ifdef TUYA_HAS_TRANS
BOOL fg_has_trans_task;                     /* 通道传输标志 */
TYB_TRANS_INFO tyb_trans_info;	                /* 通道传输请求信息 */
TYB_TRANS_CACHE tyb_trans_cache;			/* 通道传输请求信息节点缓存队列 */

/*******************************************************************************
 函 数: TYB_SLP_cache_append()
 功 能: 低优先级通道传输请求队列_从尾部增加一个节点
 参 数: p_src--加入队列的源数据指针;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
static BOOL TYB_SLP_cache_append(TYB_TRANS_INFO* p_src)
{
    // 非满
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
 函 数: TYB_SLP_cache_shift()
 功 能: 低优先级通道传输请求队列_从头部移出一个节点
 参 数: p_dst--移出队列的目标数据指针;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
static BOOL TYB_SLP_cache_shift(TYB_TRANS_INFO* p_dst)
{
    // 非空
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
 函 数: TYB_SLP_is_node_unique()
 功 能: 判断新节点在队列中是否存在
 参 数: node--新通道传输请求节点;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
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
 函 数: TYB_SLP_check_channel_transmit()
 功 能: 检查是否进行通道传输
 参 数: channel_trans_dpid_idx--通道传输功能点ID索引号;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
static BOOL TYB_SLP_check_channel_transmit(U08 *channel_trans_dpid_idx)
{
    BOOL channel_trans_request = FALSE;
    static U08 channel_dpid = DPID_DATA_CHANNEL_01;


    /* 当前无通道传送任务 */
    if (!fg_has_trans_task)
    {
       /* 有全部上报需求，等本次通道传输完成后，再进行全部上报 */
       if (fg_dpid_upload_all)
       {
           return channel_trans_request;
       }

       /* 队列非空 */
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
 函 数: TYB_SLP_check_data_updated()
 功 能: 检查本地数据是否发生更新，通过对比新旧值，若不同则用新值更新旧值
 参 数: cur_val--新值(当前值);
        prev_val--旧值指针;
 返 回: TRUE/FALSE--数据无更新/数据已更新
 其 它: 空
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
 函 数: TYB_SLP_check_all_upload()
 功 能: 全部上报一次,用于掉电/更新联网/语言变化/参数变化时
 参 数: all_upload_dpid_idx--需要全部响应的功能点ID索引;
 返 回: TRUE/FALSE--需要全部响应/不需要全部响应
 其 它: 空
*******************************************************************************/
static BOOL TYB_SLP_check_all_upload(U08 *all_upload_dpid_idx)
{
    BOOL all_upload_request = FALSE;
    static U08 all_dpid_idx;

    if (fg_dpid_upload_all)
    {
        /* 跳过通道传输专用功能点 */
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
 函 数: TYB_SLP_check_local_data_update()
 功 能: 检查本地数据是否发生变化，通过新旧值对比检查，若数据发生变化则将相应功能
        点标记高优先级后再顺序上报
 参 数: local_update_dpid_idx--需要上报的功能点ID索引;
 返 回: TRUE/FALSE--需要快速响应/不需要快速响应
 其 它: 注意不可以对"系统/模块状态"等使用此方法，否则将频繁地优先上报
*******************************************************************************/
static BOOL TYB_SLP_check_local_data_update(U08 *local_update_dpid_idx)
{
    U08 dpid_idx;
    BOOL update_cloud_request = FALSE;

/* 通用设置信息 */
    /* 区域一使能 */
    if (TYB_SLP_check_data_updated(TYBC_ZoneA_EN_VAL, ZONE1_EN_IDX))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEA_FUNC_INFO);
    }
	/* 区域二使能 */
    if (TYB_SLP_check_data_updated(TYBC_ZoneB_EN_VAL, ZONE2_EN_IDX))
    {
        SETBIT_N(fg_dpid_upload_pri_high, GENERAL_SET_INFO);
    }
	/* 热水使能 */
    if (TYB_SLP_check_data_updated(TYBC_DHW_EN_VAL, DHW_EN_IDX))
    {
        SETBIT_N(fg_dpid_upload_pri_high, GENERAL_SET_INFO);
    }

/* 区域一 */
    /* 开关机 */
    if (TYB_SLP_check_data_updated(TYBC_SWITCH_VAL, ZONEA_SWITCH_IDX))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEA_FUNC_INFO);
    }
    /* 设定温度 */
    if (TYB_SLP_check_data_updated(TYBC_TEMP_SET_VAL, ZONEA_TEMP_SET_IDX))
    {
        //SETBIT_N(fg_dpid_upload_pri_high, STD_TEMP_SET);
        SETBIT_N(fg_dpid_upload_pri_high, ZONEA_FUNC_INFO);
    }
    /* 设定模式 */
    if (TYB_SLP_check_data_updated(TYBC_MODE_SET_VAL, ZONEA_MODE_SET_IDX))
    {
        //SETBIT_N(fg_dpid_upload_pri_high, STD_MODE);
        SETBIT_N(fg_dpid_upload_pri_high, ZONEA_FUNC_INFO);
    }
    /* 当前模式 */
    if (TYB_SLP_check_data_updated(TYBC_MODE_CUR_VAL, ZONEA_MODE_CUR_IDX))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEA_FUNC_INFO);
    }

    /* 控制末端 */
    if (TYB_SLP_check_data_updated(TYBC_CTRL_END_VALUE, ZONEA_CTRL_END))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEA_FUNC_INFO);
    }
	/* 控制对象 */
    if (TYB_SLP_check_data_updated(TYBC_CTRL_OBJ, ZONEA_CTRL_OBJ))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEA_FUNC_INFO);
    }
	/* 压机状态 */
    if (TYB_SLP_check_data_updated(TYBC_COMP_STA, ZONEA_COMP_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEA_FUNC_INFO);
    }
	/* 除霜状态 */
    if (TYB_SLP_check_data_updated(TYBC_DEFRO_STA, ZONEA_DEFRO_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEA_FUNC_INFO);
    }
	/* 防冻状态 */
    if (TYB_SLP_check_data_updated(TYBC_ANFREZ_STA, ZONEA_ANFREZ_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEA_FUNC_INFO);
    }
	/* 外部热源输出状态 */
    if (TYB_SLP_check_data_updated(TYBC_OUTHEAT_STA, ZONEA_OUTHEAT_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEA_FUNC_INFO);
    }
	/* 辅助电加热状态 */
    if (TYB_SLP_check_data_updated(TYBC_AUXHEAT_STA, ZONEA_AUXHEAT_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEA_FUNC_INFO);
    }
	/* 静音模式状态 */
    if (TYB_SLP_check_data_updated(TYBC_SILENT_STA, ZONEA_SILENT_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEA_FUNC_INFO);
    }
	/* 节能模式状态 */
    if (TYB_SLP_check_data_updated(TYBC_ENESAVING_STA, ZONEA_ENESAVING_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEA_FUNC_INFO);
    }
	/* 假日模式状态 */
    if (TYB_SLP_check_data_updated(TYBC_HOLIDAY_STA, ZONEA_HOLIDAY_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEA_FUNC_INFO);
    }

/* 区域二 */
    /* 开关机 */
    if (TYB_SLP_check_data_updated(TYBC_ZoneB_SWITCH_VAL, ZONEB_SWITCH_IDX))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEB_FUNC_INFO);
    }
    /* 设定温度 */
    if (TYB_SLP_check_data_updated(TYBC_ZoneB_TEMP_SET_VAL, ZONEB_TEMP_SET_IDX))
    {
        //SETBIT_N(fg_dpid_upload_pri_high, STD_TEMP_SET);
        SETBIT_N(fg_dpid_upload_pri_high, ZONEB_FUNC_INFO);
    }
    /* 控制末端 */
    if (TYB_SLP_check_data_updated(TYBC_ZoneB_CTRL_END_VALUE, ZONEB_CTRL_END))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEB_FUNC_INFO);
    }
	/* 控制对象 */
    if (TYB_SLP_check_data_updated(TYBC_ZoneB_CTRL_OBJ, ZONEB_CTRL_OBJ))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEB_FUNC_INFO);
    }
	/* 压机状态 */
    if (TYB_SLP_check_data_updated(TYBC_ZoneB_COMP_STA, ZONEB_COMP_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEB_FUNC_INFO);
    }
	/* 除霜状态 */
    if (TYB_SLP_check_data_updated(TYBC_ZoneB_DEFRO_STA, ZONEB_DEFRO_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEB_FUNC_INFO);
    }
	/* 防冻状态 */
    if (TYB_SLP_check_data_updated(TYBC_ZoneB_ANFREZ_STA, ZONEB_ANFREZ_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEB_FUNC_INFO);
    }
	/* 外部热源输出状态 */
    if (TYB_SLP_check_data_updated(TYBC_ZoneB_OUTHEAT_STA, ZONEB_OUTHEAT_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEB_FUNC_INFO);
    }
	/* 辅助电加热状态 */
    if (TYB_SLP_check_data_updated(TYBC_ZoneB_AUXHEAT_STA, ZONEB_AUXHEAT_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEB_FUNC_INFO);
    }
	/* 静音模式状态 */
    if (TYB_SLP_check_data_updated(TYBC_ZoneB_SILENT_STA, ZONEB_SILENT_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEB_FUNC_INFO);
    }
	/* 节能模式状态 */
    if (TYB_SLP_check_data_updated(TYBC_ZoneB_ENESAVING_STA, ZONEB_ENESAVING_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEB_FUNC_INFO);
    }
	/* 假日模式状态 */
    if (TYB_SLP_check_data_updated(TYBC_ZoneB_HOLIDAY_STA, ZONEB_HOLIDAY_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, ZONEB_FUNC_INFO);
    }

/* 生活热水 */
	/* 开关机 */
    if (TYB_SLP_check_data_updated(TYBC_DHW_SWITCH_VAL, DHW_SWITCH_IDX))
    {
        SETBIT_N(fg_dpid_upload_pri_high, DHW_FUNC_INFO);
    }
    /* 设定温度 */
    if (TYB_SLP_check_data_updated(TYBC_DHW_TEMP_SET_VAL, DHW_TEMP_SET_IDX))
    {
        //SETBIT_N(fg_dpid_upload_pri_high, STD_TEMP_SET);
        SETBIT_N(fg_dpid_upload_pri_high, DHW_FUNC_INFO);
    }
    /* 控制末端 */
    if (TYB_SLP_check_data_updated(TYBC_DHW_CTRL_END_VALUE, DHW_CTRL_END))
    {
        SETBIT_N(fg_dpid_upload_pri_high, DHW_FUNC_INFO);
    }
	/* 压机状态 */
    if (TYB_SLP_check_data_updated(TYBC_DHW_COMP_STA, DHW_COMP_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, DHW_FUNC_INFO);
    }
	/* 除霜状态 */
    if (TYB_SLP_check_data_updated(TYBC_DHW_DEFRO_STA, DHW_DEFRO_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, DHW_FUNC_INFO);
    }
	/* 防冻状态 */
    if (TYB_SLP_check_data_updated(TYBC_DHW_ANFREZ_STA, DHW_ANFREZ_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, DHW_FUNC_INFO);
    }
	/* 外部热源输出状态 */
    if (TYB_SLP_check_data_updated(TYBC_DHW_OUTHEAT_STA, DHW_OUTHEAT_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, DHW_FUNC_INFO);
    }

	/* 辅助电加热状态 */
    if (TYB_SLP_check_data_updated(TYBC_DHW_AUXHEAT_STA, DHW_AUXHEAT_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, DHW_FUNC_INFO);
    }
	/* 静音模式状态 */
    if (TYB_SLP_check_data_updated(TYBC_DHW_SILENT_STA, DHW_SILENT_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, DHW_FUNC_INFO);
    }
	/* 节能模式状态 */
    if (TYB_SLP_check_data_updated(TYBC_DHW_ENESAVING_STA, DHW_ENESAVING_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, DHW_FUNC_INFO);
    }
	/* 假日模式状态 */
    if (TYB_SLP_check_data_updated(TYBC_DHW_HOLIDAY_STA, DHW_HOLIDAY_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, DHW_FUNC_INFO);
    }
	/* 太阳能信号状态 */
    if (TYB_SLP_check_data_updated(TYBC_DHW_SOLAR_STA, DHW_AREA_SOLAR_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, DHW_FUNC_INFO);
    }
	/* 杀菌状态 */
    if (TYB_SLP_check_data_updated(TYBC_DHW_STERILIZE_STA, DHW_STERILIZE_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, DHW_FUNC_INFO);
    }
	/* 水箱电热状态 */
    if (TYB_SLP_check_data_updated(TYBC_DHW_TANKHEAT_STA, DHW_TANKHEAT_STA))
    {
        SETBIT_N(fg_dpid_upload_pri_high, DHW_FUNC_INFO);
    }

#if 0
/* 有"扩展功能" */
#ifdef TUYA_HAS_EXT_FUNC
/* 有"风速功能" */
#ifdef TUYA_USE_WIND
    /* 设定风速 */
    if (TYB_SLP_check_data_updated(TYBC_WIND_SET_VAL, WIND_SET_IDX))
    {
        SETBIT_N(fg_dpid_upload_pri_high, EXTEND_FUNC_INFO);
    }
#endif

/* 有"湿度功能" */
#ifdef TUYA_USE_HUMI
    /* 设定湿度 */
    if (TYB_SLP_check_data_updated(TYBC_HUMI_SET_VAL, HUMI_SET_IDX))
    {
        SETBIT_N(fg_dpid_upload_pri_high, EXTEND_FUNC_INFO);
    }
#endif

/* 有"热水功能" */
#ifdef TUYA_USE_HOT_WATER
    /* 热水设定温度 */
    if (TYB_SLP_check_data_updated(TYBC_HOT_WATER_SET_VAL, HOT_WATER_SET_IDX))
    {
        SETBIT_N(fg_dpid_upload_pri_high, EXTEND_FUNC_INFO);
    }
#endif
#endif
#endif

/* 有"当前故障" */
#ifdef TUYA_USE_ERR_CUR
    /* 故障告警 */
    if (TYB_SLP_check_data_updated(TYBC_ERROR_ALERT_VAL, ERR_ALERT_IDX))
    {
        SETBIT_N(fg_dpid_upload_pri_high, SYS_INFO);
        SETBIT_N(fg_dpid_upload_pri_high, ERR_CUR_INFO);
    }
    /* 当前故障数 */
    if (TYB_SLP_check_data_updated(TYBC_CUR_ERR_NUM, CUR_ERR_NUM_IDX))
    {
        SETBIT_N(fg_dpid_upload_pri_high, SYS_INFO);
        SETBIT_N(fg_dpid_upload_pri_high, ERR_CUR_INFO);
    }
#endif

/* 有"厂家密码" */
#ifdef TUYA_USE_FAC_PWD
    /* 厂家密码 */
    if (TYBC_check_password_modified())
    {
        SETBIT_N(fg_dpid_upload_pri_high, SYS_INFO);
    }
#endif

/* 有"启动页" */
#ifdef TUYA_USE_SPLASH
	/* 启动页LOGO */
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
 函 数: TYB_SLP_check_interval_upload()
 功 能: 检查是否进行周期上报
 参 数: timer_dpid_idx--周期上报功能点索引号;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
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
        // 下一个准备
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
 函 数: TYB_SLP_select_upload_dpid_idx()
 功 能: 选择需要上报的功能点，快速响应上报优先级最高、数据变动主动上报其次、轮询
        上报优先级最低
 参 数: 空
 返 回: 需要上报的功能点ID索引
 其 它: 判断哪些需要优先上传
*******************************************************************************/
U08 TYB_SLP_select_upload_dpid_idx(void)
{
#ifdef TUYA_HAS_TRANS
    U08 channel_trans_dpid_idx = DATA_CHANNEL_01;  /* 供通道传输时使用 */
#endif
    U08 regular_upload_dpid_idx = 0;                                        /* 供正常上报时使用 */
    U08 all_upload_dpid_idx = 0;                                            /* 供需全部上报时使用 */

    if (!fg_dpid_upload_init)
    {
        fg_dpid_upload_init = TRUE;
        return PROTOCOL_VER;
    }

    /* 检测控制器语言是否发生改变 */
    if (TYB_SLP_check_data_updated(TYBC_LANGUAGE_VAL, LANGUAGE_IDX))
    {
        fg_dpid_upload_all = TRUE;
    }

/* 有通道传输(有"参数设置"/"模块状态") */
#ifdef TUYA_HAS_TRANS
    /* 通道传输 */
    if (TYB_SLP_check_channel_transmit(&channel_trans_dpid_idx))
    {
        return channel_trans_dpid_idx;
    }
    /* 常规上报 */
    else
    {
        /* 全部上报 */
        if (TYB_SLP_check_all_upload(&all_upload_dpid_idx))
        {
            return all_upload_dpid_idx;
        }
        /* 正常速度上报 */
        else
        {
            /* 数据有变化主动上报 */
            if (TYB_SLP_check_local_data_update(&regular_upload_dpid_idx))
            {
                return regular_upload_dpid_idx;
            }
            /* 功能点轮询上报 */
            else
            {
                if (TYB_SLP_check_interval_upload(&regular_upload_dpid_idx))
                {
                    return regular_upload_dpid_idx;
                }
            }
        }
    }
// 已裁剪参数设置功能, 并且已裁剪模块状态时, 无通道传输
#else
    /* 全部上报 */
    if (TYB_SLP_check_all_upload(&all_upload_dpid_idx))
    {
        return all_upload_dpid_idx;
    }
    /* 正常速度上报 */
    else
    {
        /* 数据有变化主动上报 */
        if (TYB_SLP_check_local_data_update(&regular_upload_dpid_idx))
        {
            return regular_upload_dpid_idx;
        }
        /* 功能点轮询上报 */
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
 函 数: TYB_SLP_concat_ver_info()
 功 能: 版本信息拼接，带拼接长度预判
 参 数: buf--字符串缓存区;
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TYB_SLP_concat_ver_info(U08 *buf)
{
    U08 buf_len = 0;

/* 显示显示屏版本 */
#if (TYBC_DISP_HMI_VER_EN == TRUE)
#if (TYBC_STR_CODE_TYPE == UNI_CODE)
    TYB_SLP_ascii_to_unicode(&tyb_hmi_ver_unicode[0], &tyb_hmi_ver[0], MAX_VER_BYTE*2);
    /* 字段0: 显示屏版本 */
    TYB_create_one_msg_field(&buf[buf_len], \
                             &buf_len, \
                             tyb_hmi_ver_unicode, \
                             MAX_VER_BYTE*2);
#else
    /* 字段0: 显示屏版本 */
    TYB_create_one_msg_field(&buf[buf_len], \
                             &buf_len, \
                             tyb_hmi_ver, \
                             MAX_VER_BYTE);
#endif
/* 不显示显示屏版本 */
#else
    buf[buf_len++] = FIELD_ONE_MSG;
    buf[buf_len++] = FIELD_DISABLED;
    buf[buf_len++] = 0;
#endif

/* 显示控制器版本 */
#if (TYBC_DISP_BKBD_VER_EN == TRUE)
#if (TYBC_STR_CODE_TYPE == UNI_CODE)
	TYB_SLP_ascii_to_unicode(&tyb_bkbd_ver_unicode[0], (far U08 *)TYBC_BKBD_VER, MAX_VER_BYTE*2);
    /* 字段0: 显示屏版本 */
    TYB_create_one_msg_field(&buf[buf_len], \
                             &buf_len, \
                             tyb_bkbd_ver_unicode, \
                             MAX_VER_BYTE*2);
#else
    /* 字段1: 控制器版本 */
    TYB_create_one_msg_field(&buf[buf_len], \
                             &buf_len, \
                             (far U08 *)TYBC_BKBD_VER, \
                             (sizeof(TYBC_BKBD_VER)/sizeof(U08)));
#endif
/* 不显示控制器版本 */
#else
    buf[buf_len++] = FIELD_ONE_MSG;
    buf[buf_len++] = FIELD_DISABLED;
    buf[buf_len++] = 0;
#endif
}


/*******************************************************************************
 函 数: TYB_SLP_concat_sys_info()
 功 能: 系统信息拼接，带拼接长度预判
 参 数: buf--字符串缓存区;
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TYB_SLP_concat_sys_info(U08 *buf)
{
    U08 buf_len = 0;
    U16 fg_func_en = 0;

    /* 字段0: 字符编码类型 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_STR_CODE_TYPE, FIELD_READONLY, FIELD_STR_CODE_TYPE);

    /* 字段1: 系统维护 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_SYS_MAINTENANCE_VAL, FIELD_READONLY, FIELD_SYS_MAINT);

    /* 字段2: 语言 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_LANGUAGE_VAL, FIELD_READONLY, FIELD_CNTRL_LANG);

    /* 字段3: 故障告警 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_ERROR_ALERT_VAL, FIELD_READONLY, FIELD_ERR_ALERT);

    /* 字段4: 模块数 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_MOD_NUM, FIELD_READONLY, FIELD_MOD_NUM);

    /* 字段5: 厂家密码 */
    TYB_create_one_msg_field(&buf[buf_len], &buf_len, TYBC_PASSWORD_POINTER, TYBC_PWD_MAX);

    /* 字段6: 运行切换模式允许 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_RUN_SWITCH_MODE_EN, FIELD_READONLY, FIELD_RUN_STATUS);

    /* 字段7: 启动页LOGO */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_SPLASH_LOGO_VAL, FIELD_READONLY,FIELD_SPLASH_LOGO);

    /* 字段8: 智能语音 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_SMART_SPEAKER_EN, FIELD_READONLY, FIELD_SPEECH_EN);

/* 有"参数设置" */
#ifdef TUYA_USE_PARAM_SET
    /* 字段9: 参数组总数(预留最大数) */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_PARAM_GRP_MAX, FIELD_READONLY, FIELD_GRP_MAX);

    /* 字段10: 参数项总数(预留最大数) */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_PARAM_GRP_SIZE, FIELD_READONLY, FIELD_ITEM_MAX);
#else
    /* 字段9: 参数组总数(0) */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, 0, FIELD_READONLY, FIELD_GRP_MAX);

    /* 字段10: 参数项总数(0) */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, 0, FIELD_READONLY, FIELD_ITEM_MAX);
#endif

    /* 字段11: 可选功能 */
/* 有"通用设置" */
#ifdef TUYA_HAS_GEN_SET
    fg_func_en |= TYBC_FUNC_EN_MORE_GEN_SET << FUNC_EN_MORE_GEN_SET_BIT;
#endif
/* 有"历史故障" */
#ifdef TUYA_USE_ERR_HIS
    fg_func_en |= TYBC_FUNC_EN_ADVANCE_ERR_HIS << FUNC_EN_ADVANCE_ERR_HIS_BIT;
#endif
/* 有"参数设置" */
#ifdef TUYA_USE_PARAM_SET
    fg_func_en |= TYBC_FUNC_EN_ADVANCE_PARAM_SET << FUNC_EN_ADVANCE_PARAM_SET_BIT;
#endif
/* 有"状态查询" */
#ifdef TUYA_HAS_STAT
    fg_func_en |= TYBC_FUNC_EN_MAIN_STAT << FUNC_EN_MAIN_STAT_BIT;
    fg_func_en |= TYBC_FUNC_EN_ADVANCE_STAT << FUNC_EN_ADVANCE_STAT_BIT;
#endif
/* 有"用电查询" */
#ifdef TUYA_USE_ELE_STAT
    fg_func_en |= TYBC_FUNC_EN_MAIN_ELE_STAT << FUNC_EN_MAIN_ELE_STAT_BIT;
#endif
/* 有"云定时" */
#ifdef TUYA_USE_CLO_TIME
    fg_func_en |= TYBC_FUNC_EN_MORE_CLO_TIME << FUNC_EN_MORE_CLO_TIME_BIT;
#endif
    TYB_create_one_num_field(&buf[buf_len], &buf_len, fg_func_en, FIELD_READONLY, FIELD_FUNC_OPT);
}


/*******************************************************************************
 函 数: TYB_SLP_concat_protocol_ver()
 功 能: 协议版本拼接，带拼接长度预判
 参 数: buf--字符串缓存区;
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TYB_SLP_concat_protocol_ver(U08 *buf)
{
    U08 buf_len = 0;

    /* 字段0: 协议版本 */
    TYB_create_one_msg_field(&buf[buf_len], \
                             &buf_len, \
                             (far U08 *)TYB_SLP_PROTOCOL_VER, \
                             (sizeof(TYB_SLP_PROTOCOL_VER)/sizeof(U08)));
}


/* 有"通用设置功能" */
#ifdef TUYA_HAS_GEN_SET
/*******************************************************************************
 函 数: TYB_SLP_concat_general_set_info()
 功 能: 设置信息字段拼接，带拼接长度预判
 参 数: buf--字符串缓存区;
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TYB_SLP_concat_general_set_info(U08 *buf)
{
    U08 buf_len = 0;

/* 有"语言功能" */
#ifdef TUYA_USE_LANG_SET
    /* 字段0: 语言 */
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

/* 有"启动页功能" */
#ifdef TUYA_USE_SPLASH
    /* 字段1: 启动页 */
    TYB_create_bool_field(&buf[buf_len], \
                          &buf_len, \
                          FIELD_WRITABLE, \
                          FIELD_SPLASH, \
                          TYBC_SPLASH_TYPE, \
                          TYBC_SPLASH_TITLE, \
                          TYBC_SPLASH_VAL);
#endif

/* 有"童锁功能" */
#ifdef TUYA_USE_CHILDLOCK
    /* 字段2: 童锁 */
    TYB_create_bool_field(&buf[buf_len], \
                          &buf_len, \
                          FIELD_WRITABLE, \
                          FIELD_CHILDLOCK, \
                          TYBC_CHILDLOCK_TYPE, \
                          TYBC_CHILD_LOCK_TITLE, \
                          TYBC_CHILDLOCK_VAL);
#endif

	/* 字段3: 区域一使能 */
    TYB_create_bool_field(&buf[buf_len], \
                          &buf_len, \
                          FIELD_READONLY, \
                          FIELD_ZoneA_EN, \
                          TYBC_ZoneA_EN_TYPE, \
                          TYBC_ZoneA_EN_TITLE, \
                          TYBC_ZoneA_EN_VAL);

	/* 字段4: 区域二使能 */
    TYB_create_bool_field(&buf[buf_len], \
                          &buf_len, \
                          FIELD_READONLY, \
                          FIELD_ZoneB_EN, \
                          TYBC_ZoneB_EN_TYPE, \
                          TYBC_ZoneB_EN_TITLE, \
                          TYBC_ZoneB_EN_VAL);

	/* 字段5: 热水使能 */
    TYB_create_bool_field(&buf[buf_len], \
                          &buf_len, \
                          FIELD_READONLY, \
                          FIELD_DHW_EN, \
                          TYBC_DHW_EN_TYPE, \
                          TYBC_DHW_EN_TITLE, \
                          TYBC_DHW_EN_VAL);
/* 有"语言功能" */
#ifdef TUYA_USE_TEMP_UNIT_SET
    /* 字段0: 语言 */
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
 函 数: TYB_SLP_concat_zone1_func_info()
 功 能: 区域一功能信息字段拼接，带拼接长度预判
 参 数: buf--字符串缓存区;
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TYB_SLP_concat_zone1_func_info(U08 *buf)
{
    U08 buf_len = 0;

    /* 字段0: 开关机 */
    TYB_create_bool_field(&buf[buf_len], \
                          &buf_len, \
                          FIELD_WRITABLE, \
                          FIELD_AREA_SWITCH, \
                          TYBC_SWITCH_TYPE, \
                          TYBC_SWITCH_TITLE, \
                          TYBC_SWITCH_VAL);

    /* 字段1: 设定温度 */
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

    /* 字段2: 当前温度 */
    TYB_create_ro_num_field(&buf[buf_len], \
                            &buf_len, \
                            FIELD_READONLY, \
                            FIELD_AREA_TEMP_CUR, \
                            TYBC_TEMP_CUR_TYPE, \
                            TYBC_TEMP_CUR_TITLE, \
                            TYBC_TEMP_CUR_ATTR, \
                            TYBC_TEMP_CUR_VAL);

    /* 字段3: 设定模式 */
    TYB_create_enum_field(&buf[buf_len], \
                          &buf_len, \
                          FIELD_WRITABLE, \
                          FIELD_AREA_MODE_SET,\
                          TYBC_MODE_SET_TYPE, \
                          TYBC_MODE_SET_TITLE, \
                          TYBC_MODE_SET_RANGE, \
                          TYBC_MODE_SET_MAX_NUM, \
                          TYBC_MODE_SET_VAL);

    /* 字段4: 当前模式 */
    TYB_create_enum_field(&buf[buf_len], \
                          &buf_len, \
                          FIELD_READONLY, \
                          FIELD_AREA_MODE_CUR, \
                          TYBC_MODE_CUR_TYPE, \
                          TYBC_MODE_CUR_TITLE, \
                          TYBC_MODE_CUR_RANGE, \
                          TYBC_MODE_CUR_MAX_NUM, \
                          TYBC_MODE_CUR_VAL);

    /* 字段5: 控制末端 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_CTRL_END_VALUE, FIELD_READONLY, FIELD_AREA_CTRL_WAY);

    /* 字段6: 控制对象 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_CTRL_OBJ, FIELD_READONLY, FIELD_AREA_CTRL_obj);

    /* 字段7: 压机状态 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_COMP_STA, FIELD_READONLY, FIELD_AREA_COMP_STA);

    /* 字段8: 除霜状态 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_DEFRO_STA, FIELD_READONLY, FIELD_AREA_DEFRO_STA);

    /* 字段9: 防冻状态 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_ANFREZ_STA, FIELD_READONLY, FIELD_AREA_ANFREZ_STA);

    /* 字段10: 外部热源输出状态 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_OUTHEAT_STA, FIELD_READONLY, FIELD_AREA_OUTHEAT_STA);

    /* 字段11: 杀菌状态 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_STERILIZE_STA, FIELD_READONLY, FIELD_AREA_STERILIZE_STA);

    /* 字段12: 辅助电热状态 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_AUXHEAT_STA, FIELD_READONLY, FIELD_AREA_AUXHEAT_STA);

    /* 字段13: 水箱电热状态 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_TANKHEAT_STA, FIELD_READONLY, FIELD_AREA_TANKHEAT_STA);

    /* 字段14: 静音模式状态 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_SILENT_STA, FIELD_READONLY, FIELD_AREA_SILENT_STA);

    /* 字段15: 节能模式状态 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_ENESAVING_STA, FIELD_READONLY, FIELD_AREA_ENESAVING_STA);

    /* 字段16: 假日模式状态 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_HOLIDAY_STA, FIELD_READONLY, FIELD_AREA_HOLIDAY_STA);

    /* 字段17: 太阳能信号状态 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_SOLAR_STA, FIELD_READONLY, FIELD_AREA_SOLAR_STA);
}

/*******************************************************************************
 函 数: TYB_SLP_concat_zone2_func_info()
 功 能: 区域二功能信息字段拼接，带拼接长度预判
 参 数: buf--字符串缓存区;
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TYB_SLP_concat_zone2_func_info(U08 *buf)
{
    U08 buf_len = 0;

    /* 字段0: 开关机 */
    TYB_create_bool_field(&buf[buf_len], \
                          &buf_len, \
                          FIELD_WRITABLE, \
                          FIELD_AREA_SWITCH, \
                          TYBC_ZoneB_SWITCH_TYPE, \
                          TYBC_ZoneB_SWITCH_TITLE, \
                          TYBC_ZoneB_SWITCH_VAL);

    /* 字段1: 设定温度 */
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

    /* 字段2: 当前温度 */
    TYB_create_ro_num_field(&buf[buf_len], \
                            &buf_len, \
                            FIELD_READONLY, \
                            FIELD_AREA_TEMP_CUR, \
                            TYBC_ZoneB_TEMP_CUR_TYPE, \
                            TYBC_ZoneB_TEMP_CUR_TITLE, \
                            TYBC_ZoneB_TEMP_CUR_ATTR, \
                            TYBC_ZoneB_TEMP_CUR_VAL);

    /* 字段3: 设定模式 */
    TYB_create_enum_field(&buf[buf_len], \
                          &buf_len, \
                          FIELD_WRITABLE, \
                          FIELD_AREA_MODE_SET,\
                          TYBC_ZoneB_MODE_SET_TYPE, \
                          TYBC_ZoneB_MODE_SET_TITLE, \
                          TYBC_ZoneB_MODE_SET_RANGE, \
                          TYBC_ZoneB_MODE_SET_MAX_NUM, \
                          TYBC_ZoneB_MODE_SET_VAL);

    /* 字段4: 当前模式 */
    TYB_create_enum_field(&buf[buf_len], \
                          &buf_len, \
                          FIELD_READONLY, \
                          FIELD_AREA_MODE_CUR, \
                          TYBC_ZoneB_MODE_CUR_TYPE, \
                          TYBC_ZoneB_MODE_CUR_TITLE, \
                          TYBC_ZoneB_MODE_CUR_RANGE, \
                          TYBC_ZoneB_MODE_CUR_MAX_NUM, \
                          TYBC_ZoneB_MODE_CUR_VAL);

    /* 字段5: 控制末端 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_ZoneB_CTRL_END_VALUE, FIELD_READONLY, FIELD_AREA_CTRL_WAY);

    /* 字段6: 控制对象 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_ZoneB_CTRL_OBJ, FIELD_READONLY, FIELD_AREA_CTRL_obj);

    /* 字段7: 压机状态 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_ZoneB_COMP_STA, FIELD_READONLY, FIELD_AREA_COMP_STA);

    /* 字段8: 除霜状态 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_ZoneB_DEFRO_STA, FIELD_READONLY, FIELD_AREA_DEFRO_STA);

    /* 字段9: 防冻状态 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_ZoneB_ANFREZ_STA, FIELD_READONLY, FIELD_AREA_ANFREZ_STA);

    /* 字段10: 外部热源输出状态 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_ZoneB_OUTHEAT_STA, FIELD_READONLY, FIELD_AREA_OUTHEAT_STA);

    /* 字段11: 杀菌状态 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_ZoneB_STERILIZE_STA, FIELD_READONLY, FIELD_AREA_STERILIZE_STA);

    /* 字段12: 辅助电热状态 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_ZoneB_AUXHEAT_STA, FIELD_READONLY, FIELD_AREA_AUXHEAT_STA);

    /* 字段13: 水箱电热状态 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_ZoneB_TANKHEAT_STA, FIELD_READONLY, FIELD_AREA_TANKHEAT_STA);

    /* 字段14: 静音模式状态 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_ZoneB_SILENT_STA, FIELD_READONLY, FIELD_AREA_SILENT_STA);

    /* 字段15: 节能模式状态 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_ZoneB_ENESAVING_STA, FIELD_READONLY, FIELD_AREA_ENESAVING_STA);

    /* 字段16: 假日模式状态 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_ZoneB_HOLIDAY_STA, FIELD_READONLY, FIELD_AREA_HOLIDAY_STA);

    /* 字段17: 太阳能信号状态 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_ZoneB_SOLAR_STA, FIELD_READONLY, FIELD_AREA_SOLAR_STA);

}

/*******************************************************************************
 函 数: TYB_SLP_concat_DHW_func_info()
 功 能: 热水功能信息字段拼接，带拼接长度预判
 参 数: buf--字符串缓存区;
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TYB_SLP_concat_DHW_func_info(U08 *buf)
{
    U08 buf_len = 0;

    /* 字段0: 开关机 */
    TYB_create_bool_field(&buf[buf_len], \
                          &buf_len, \
                          FIELD_WRITABLE, \
                          FIELD_AREA_SWITCH, \
                          TYBC_DHW_SWITCH_TYPE, \
                          TYBC_DHW_SWITCH_TITLE, \
                          TYBC_DHW_SWITCH_VAL);

    /* 字段1: 设定温度 */
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

    /* 字段2: 当前温度 */
    TYB_create_ro_num_field(&buf[buf_len], \
                            &buf_len, \
                            FIELD_READONLY, \
                            FIELD_AREA_TEMP_CUR, \
                            TYBC_DHW_TEMP_CUR_TYPE, \
                            TYBC_DHW_TEMP_CUR_TITLE, \
                            TYBC_DHW_TEMP_CUR_ATTR, \
                            TYBC_DHW_TEMP_CUR_VAL);

    /* 字段3: 设定模式 */
    TYB_create_enum_field(&buf[buf_len], \
                          &buf_len, \
                          FIELD_WRITABLE, \
                          FIELD_AREA_MODE_SET,\
                          TYBC_DHW_MODE_SET_TYPE, \
                          TYBC_DHW_MODE_SET_TITLE, \
                          TYBC_DHW_MODE_SET_RANGE, \
                          TYBC_DHW_MODE_SET_MAX_NUM, \
                          TYBC_DHW_MODE_SET_VAL);

    /* 字段4: 当前模式 */
    TYB_create_enum_field(&buf[buf_len], \
                          &buf_len, \
                          FIELD_READONLY, \
                          FIELD_AREA_MODE_CUR, \
                          TYBC_DHW_MODE_CUR_TYPE, \
                          TYBC_DHW_MODE_CUR_TITLE, \
                          TYBC_DHW_MODE_CUR_RANGE, \
                          TYBC_DHW_MODE_CUR_MAX_NUM, \
                          TYBC_DHW_MODE_CUR_VAL);

    /* 字段5: 控制末端 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_DHW_CTRL_END_VALUE, FIELD_READONLY, FIELD_AREA_CTRL_WAY);

    /* 字段6: 控制对象 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_DHW_CTRL_OBJ, FIELD_READONLY, FIELD_AREA_CTRL_obj);

    /* 字段7: 压机状态 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_DHW_COMP_STA, FIELD_READONLY, FIELD_AREA_COMP_STA);

    /* 字段8: 除霜状态 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_DHW_DEFRO_STA, FIELD_READONLY, FIELD_AREA_DEFRO_STA);

    /* 字段9: 防冻状态 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_DHW_ANFREZ_STA, FIELD_READONLY, FIELD_AREA_ANFREZ_STA);

    /* 字段10: 外部热源输出状态 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_DHW_OUTHEAT_STA, FIELD_READONLY, FIELD_AREA_OUTHEAT_STA);

    /* 字段11: 杀菌状态 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_DHW_STERILIZE_STA, FIELD_READONLY, FIELD_AREA_STERILIZE_STA);

    /* 字段12: 辅助电热状态 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_DHW_AUXHEAT_STA, FIELD_READONLY, FIELD_AREA_AUXHEAT_STA);

    /* 字段13: 水箱电热状态 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_DHW_TANKHEAT_STA, FIELD_READONLY, FIELD_AREA_TANKHEAT_STA);

    /* 字段14: 静音模式状态 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_DHW_SILENT_STA, FIELD_READONLY, FIELD_AREA_SILENT_STA);

    /* 字段15: 节能模式状态 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_DHW_ENESAVING_STA, FIELD_READONLY, FIELD_AREA_ENESAVING_STA);

    /* 字段16: 假日模式状态 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_DHW_HOLIDAY_STA, FIELD_READONLY, FIELD_AREA_HOLIDAY_STA);

    /* 字段17: 太阳能信号状态 */
    TYB_create_one_num_field(&buf[buf_len], &buf_len, TYBC_DHW_SOLAR_STA, FIELD_READONLY, FIELD_AREA_SOLAR_STA);

}

/* 有"扩展功能" */
#ifdef TUYA_HAS_EXT_FUNC
/*******************************************************************************
 函 数: TYB_SLP_concat_extend_func_info()
 功 能: 扩展功能信息字段拼接，带拼接长度预判
 参 数: buf--字符串缓存区;
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TYB_SLP_concat_extend_func_info(U08 *buf)
{
    U08 buf_len = 0;

/* 有"风速功能" */
#ifdef TUYA_USE_WIND
    /* 字段0: 设定风速 */
    TYB_create_enum_field(&buf[buf_len], \
                          &buf_len, \
                          FIELD_WRITABLE, \
                          FIELD_WIND_SET, \
                          TYBC_WIND_SET_TYPE, \
                          TYBC_WIND_SET_TITLE, \
                          TYBC_WIND_SET_RANGE, \
                          TYBC_WIND_SET_MAX_NUM, \
                          TYBC_WIND_SET_VAL);

    /* 字段1: 当前风速 */
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

/* 有"湿度功能" */
#ifdef TUYA_USE_HUMI
    /* 字段2: 设定湿度 */
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

    /* 字段3: 当前湿度 */
    TYB_create_ro_num_field(&buf[buf_len], \
                            &buf_len, \
                            FIELD_READONLY, \
                            FIELD_HUMI_CUR, \
                            TYBC_HUMI_CUR_TYPE, \
                            TYBC_HUMI_CUR_TITLE, \
                            TYBC_HUMI_CUR_ATTR, \
                            TYBC_HUMI_CUR_VAL);
#endif

/* 有"热水功能" */
#ifdef TUYA_USE_HOT_WATER
    /* 字段4: 热水设定温度 */
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

    /* 字段5: 当前热水温度 */
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

/* 有"当前故障" */
#ifdef TUYA_USE_ERR_CUR
/*******************************************************************************
 函 数: TYB_SLP_concat_err_cur_info()
 功 能: 当前故障信息拼接，带拼接长度预判
 参 数: buf--字符串缓存区;
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TYB_SLP_concat_err_cur_info(U08 *buf)
{
    U16 i,mod_id,sys_err_id,mod_err_id;
    TYBC_ERR_CUR err;                                 /* 故障信息 */
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

            /* 空间不够，则中止数据填充 */
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


/* 有"历史故障" */
#ifdef TUYA_USE_ERR_HIS
/*******************************************************************************
 函 数: TYB_SLP_concat_err_his_info()
 功 能: 历史故障信息拼接，带拼接长度预判
 参 数: buf--字符串缓存区;
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TYB_SLP_concat_err_his_info(U08 *buf)
{
    U08 i;
    TYBC_ERR_HIS err_record;                           /* 故障信息 */
    U08 useless_cache[TYBC_STR_BYTE_MAX];
    U16 msg_len = 0;
    U16 acc_bytes = 0;
    U08 buf_len = 0;

    MEMCLR(&err_record, ((sizeof(TYBC_ERR_HIS)+1)/sizeof(U16))*2);

    for (i = 0; i < TY_HIS_ERR_MAX; i++)
    {
        /* 获取历史故障 */
        TYBC_GET_ERR_HIS(&err_record,i);

        /* 有故障 */
        if (err_record.err_id < TYBC_NO_ERR)
        {
            acc_bytes = buf_len + DP_FIELD_HIS_ERR_INFO_BYTES + TYB_SLP_GET_CODE_STR(useless_cache, err_record.name);

            /* 空间不够，则中止数据填充 */
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
 函 数: TYB_SLP_mark_upload_pv()
 功 能: 标记需要上传的模拟量
 参 数: mod_id--模块号;
        upload_pv_total--需要上传的模拟量总数;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
static BOOL TYB_SLP_mark_upload_pv(U08 mod_id, U08 *upload_pv_total)
{
    U16 i,j,idx;
    U16 comp_pv_id = 0;
    U08 upload_cnt = 0;

    /* 模块号校验 */
    if (mod_id > TYBC_PCB_MAX)
    {
        return FALSE;
    }

    /* 初始化 */
    ARRCLR(fg_upload_pv);

    /* 记录需要上报的模拟量 */
    if (mod_id == TYBC_PCB_MAX)    /* 系统 */
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
    else    /* 模块 */
    {
        for (i = 0; i < TYBC_PV_MAX; i++)
        {
            if (TYBC_PV_VAL(mod_id, i) != SENSOR_NOEXIST)
            {
                SETBIT_N(fg_upload_pv, i);
                upload_cnt++;

                if (i < TYBC_UNIT_PV_BEGIN)    /* 压机模拟量，同步标记 */
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


/* 有"系统状态" */
#ifdef TUYA_USE_SYS_STAT
/*******************************************************************************
 函 数: TYB_SLP_concat_sys_stat_info()
 功 能: 系统状态信息拼接，带字段数量限制判断
 参 数: buf--字符串缓存区;
        dpid_idx--功能点ID索引;
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TYB_SLP_concat_sys_stat_info(U08 *buf, U08 dpid_idx)
{
    U08 i;
    static U08 idx_rec = 0;              /* 记录下一个功能点的起始数据索引 */
    static BOOL fg_all_upload_pv = FALSE;
    U08 upload_pv_num = 0;
    U08 useless_cache[20];
    U16 acc_bytes = 0;
    BOOL fg_buf_full = FALSE;
    U16 attr, msg_len;
    U08 buf_len = 0;

    /* 标记需要上传的模拟量 */
    if (!TYB_SLP_mark_upload_pv(TYBC_PCB_MAX, &upload_pv_num))
    {
        return;
    }

    /* 有效模拟量未上传完 */
    if (!fg_all_upload_pv)
    {
        for (i = idx_rec; i < TYBC_SYS_PV_MAX; i++)
        {
            msg_len = 0;
            /* 有效模拟量 */
            if (GETBIT_N(fg_upload_pv, i))
            {
                msg_len = TYB_SLP_GET_CODE_STR(useless_cache, TYBC_SYS_PV_TITLE(i));
                acc_bytes = acc_bytes + DP_FIELD_SYS_STAT_INFO_BYTES + msg_len;

                /* 写入上报功能点缓存区 */
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

    /* 已传完有效内容 */
    if (!fg_buf_full)
    {
        fg_all_upload_pv = TRUE;
        idx_rec = 0;
    }

    /* 4个功能点完成一次轮询，记录清零 */
    if (DP[dpid_idx].dpid == DPID_SYS_STAT_INFO_4)
    {
        fg_all_upload_pv = FALSE;
        idx_rec = 0;
    }

    /* 结束标志 */
    if (buf_len != 0 && buf_len <= RAW_DATA_BYTES - 2)
    {
        buf[buf_len++] = 0xff;
        buf[buf_len++] = 0xff;
    }
}


/*******************************************************************************
 函 数: TYB_SLP_concat_sys_stat_data()
 功 能: 系统状态数据拼接，带字段数量限制判断
 参 数: buf--字符串缓存区;
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TYB_SLP_concat_sys_stat_data(U08 *buf)
{
    U08 i;
    static U08 upload_pv_num = 0;
    U08 upload_pv_num_prev;
    U16 acc_bytes = 0;
    U16 attr;
    U08 buf_len = 0;

    /* 模拟量总数备份 */
    upload_pv_num_prev = upload_pv_num;

    /* 标记需要上传的模拟量 */
    if (!TYB_SLP_mark_upload_pv(TYBC_PCB_MAX, &upload_pv_num))
    {
        return;
    }

    /* 模拟量总数变化检测 */
    if (upload_pv_num_prev != upload_pv_num)
    {
        SETBIT_N(fg_dpid_upload_pri_high, SYS_STAT_INFO_1);
        SETBIT_N(fg_dpid_upload_pri_high, SYS_STAT_INFO_2);
        SETBIT_N(fg_dpid_upload_pri_high, SYS_STAT_INFO_3);
        SETBIT_N(fg_dpid_upload_pri_high, SYS_STAT_INFO_4);
    }

    for (i = 0; i < TYBC_SYS_PV_MAX; i++)
    {
        /* 有效模拟量 */
        if (GETBIT_N(fg_upload_pv, i))
        {
            /* 空间计算: 新增后的累计字节数 */
            acc_bytes = buf_len + DP_FIELD_SYS_STAT_DATA_BYTES;

            /* 写入上报功能点缓存区 */
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

    /* 结束标志 */
    if (buf_len <= RAW_DATA_BYTES - 2)
    {
        buf[buf_len++] = 0xff;
        buf[buf_len++] = 0xff;
    }
}
#endif

/* 有"参数设置" */
#ifdef TUYA_USE_PARAM_SET
/*******************************************************************************
 函 数: TYB_SLP_trans_param_grp_disp_rsrc()
 功 能: 分包上传参数组名显示资源(通道传输协议，传送码:0x01)
 参 数: buf--功能点缓存区;
        info--传输请求信息;
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TYB_SLP_trans_param_grp_disp_rsrc(U08 *buf, TYB_TRANS_INFO *info)
{
    U16 i,j;
    U08 useless_cache[TYBC_STR_BYTE_MAX];	/* 无用缓存 */
    DP_FIELD_DISP_RSRC disp_rsrc;  /* 参数组名显示资源 */
    U16 upload_bytes = 0;          /* 上传数据总字节数 */
    U08 package_total = 1;         /* 总包数(至少1包) */
    BOOL fg_multi_pkg = FALSE;     /* 多包传输需求标志 */
    U16 *src_buf = NULL;
    U08 buf_len = 0;

    static U16 idx_rec = 0;        /* 记录下一包数据的起始数据索引 */


    /* 数据包重发起始数据索引 */
    if (GETBIT_N(info->request_type, RST_GRP_DISP_RSRC))
    {
        idx_rec = 0;
        CLRBIT_N(info->request_type, RST_GRP_DISP_RSRC);
    }

    /* 寻找当前数据包的起始数据索引(第0包数据) */
    if (GETBIT_N(info->request_type, CONT_GRP_DISP_RSRC))
    {
        if (info->pkg_id == 0)
        {
            idx_rec = 0;
        }
    }

    /* 计算总包数、数据起始索引 */
    for (i = 0; i < TYBC_PARAM_GRP_MAX; i++)
    {
        U08 msg_len = TYB_SLP_GET_CODE_STR(useless_cache, TYBC_PARAM_GRP_TITLE(i));
        upload_bytes += DP_FIELD_DISP_RSRC_BYTES + msg_len;

        if (upload_bytes > TRANS_01_CONTENT_BYTES)
        {
            package_total++;  /* 总包数累加 */
            upload_bytes = DP_FIELD_DISP_RSRC_BYTES + msg_len;

            /* 寻找当前数据包的起始数据索引(非第0包数据) */
            if (GETBIT_N(info->request_type, CONT_GRP_DISP_RSRC))
            {
                /* 当数据更新至请求包号时，记录此时的起始索引 */
                if (info->pkg_id == (package_total-1))
                {
                    idx_rec = i;
                }
            }
        }
    }

    /* 填充帧头 */
    buf[0] = info->trigger_src;          /* 触发源 */
    buf[1] = info->verif_code[0];        /* 验证码 */
    buf[2] = info->verif_code[1];        /* 验证码 */
    buf[3] = info->verif_code[2];        /* 验证码 */
    buf[4] = package_total;              /* 总包数 */
    buf[5] = info->pkg_id;               /* 当前包号 */
    buf[6] = info->trans_code;           /* 传送码 */

    /* 填充中间信息: 实际参数组总数 */
    buf[7] = TYBC_PARAM_GRP_NUM;

    buf_len += TRANS_01_HEADER_BYTES;

    /* 填充内容: 参数组名显示资源(全集) */
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

    /* 传输下一包 */
    info->pkg_id++;

    /* 所有参数组名显示资源上传完成 */
    if (!fg_multi_pkg)
    {
        idx_rec = 0;                                           /* 记录清空 */
        info->pkg_id = 0;                                      /* 当前包ID复位 */
        if (GETBIT_N(info->request_type, RST_GRP_DISP_ALL))    /* 请求所有时，接着发下一个传送码 */
        {
            info->trans_code = TRANS_PARAM_GRP_DISP_CFG;       /* 准备发下一个传送码 */
            CLRBIT_N(info->request_type, RST_GRP_DISP_ALL);
        }
        else    /* 重发最后一包数据或请求所有显示资源数据，结束传输 */
        {
            info->trans_code = TRANS_NULL;
            fg_has_trans_task = FALSE;
        }
    }

    /* 重传数据包时，不继续传后续传送码，结束通道传输 */
    if (GETBIT_N(info->request_type, CONT_GRP_DISP_RSRC))
    {
        fg_has_trans_task = FALSE;
        CLRBIT_N(info->request_type, CONT_GRP_DISP_RSRC);
    }
}


/*******************************************************************************
 函 数: TYB_SLP_trans_param_grp_disp_cfg()
 功 能: 分包上传参数组名配置信息(通道传输协议，传送码:0x02)
 参 数: buf--功能点缓存区;
        info--传输请求信息;
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TYB_SLP_trans_param_grp_disp_cfg(U08 *buf, TYB_TRANS_INFO *info)
{
    U16 i,j;
    DP_FIELD_PARAM_GRP_DISP_CFG disp_cfg;    /* 参数组名显示资源 */
    U16 upload_bytes = 0;                    /* 上传数据总字节数 */
    U08 package_total = 1;                   /* 总包数(至少1包) */
    BOOL fg_multi_pkg = FALSE;               /* 多包传输需求标志 */
    U16 *src_buf = NULL;
    U08 buf_len = 0;

    static U16 idx_rec = 0;                  /* 记录下一包数据的起始数据索引 */


    /* 数据包重发起始数据索引 */
    if (GETBIT_N(info->request_type, RST_GRP_DISP_CFG))
    {
        idx_rec = 0;
        CLRBIT_N(info->request_type, RST_GRP_DISP_CFG);
    }

    /* 寻找当前数据包的起始数据索引(第0包数据) */
    if (GETBIT_N(info->request_type, CONT_GRP_DISP_CFG))
    {
        if (info->pkg_id == 0)
        {
            idx_rec = 0;
        }
    }

    /* 计算总包数、数据起始索引 */
    for (i = 0; i < TYBC_PARAM_GRP_NUM; i++)
    {
        upload_bytes += DP_FIELD_PARAM_GRP_DISP_CFG_BYTES;

        if (upload_bytes > TRANS_02_CONTENT_BYTES)
        {
           package_total++;    /* 总包数累加 */
           upload_bytes = DP_FIELD_PARAM_GRP_DISP_CFG_BYTES;

           /* 寻找当前数据包的起始数据索引(非第0包数据) */
           if (GETBIT_N(info->request_type, CONT_GRP_DISP_CFG))
           {
               if (info->pkg_id == (package_total-1))
               {
                   idx_rec = i;
               }
           }
        }
    }

    /* 清空上报字节数计数 */
    upload_bytes = 0;

    /* 填充帧头 */
    buf[0] = info->trigger_src;      /* 触发源 */
    buf[1] = info->verif_code[0];    /* 验证码 */
    buf[2] = info->verif_code[1];    /* 验证码 */
    buf[3] = info->verif_code[2];    /* 验证码 */
    buf[4] = package_total;          /* 总包数 */
    buf[5] = info->pkg_id;           /* 当前包号 */
    buf[6] = info->trans_code;       /* 传送码 */

    /* 填充中间信息: 实际参数组总数 */
    buf[7] = TYBC_PARAM_GRP_NUM;

    buf_len += TRANS_02_HEADER_BYTES;

    /* 填充内容: 参数组名配置信息  */
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

    /* 传输下一包 */
    info->pkg_id++;

    /* 所有参数组名配置信息上传完成 */
    if (!fg_multi_pkg)
    {
        idx_rec = 0;                         /* 记录清空 */
        info->pkg_id = 0;                    /* 当前包ID复位 */
        info->trans_code = TRANS_NULL;
        fg_has_trans_task = FALSE;
    }

    /* 重传数据包时，不继续传后续传送码，结束通道传输 */
    if (GETBIT_N(info->request_type, CONT_GRP_DISP_CFG))
    {
        fg_has_trans_task = FALSE;
        CLRBIT_N(info->request_type, CONT_GRP_DISP_CFG);
    }
}


/*******************************************************************************
 函 数: TYB_SLP_trans_param_range_disp_rsrc()
 功 能: 分包上传参数选项名显示资源 (通道传输协议，传送码:0x03)
 参 数: buf--功能点缓存区;
        info--传输请求信息;
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TYB_SLP_trans_param_range_disp_rsrc(U08 *buf, TYB_TRANS_INFO *info)
{
    U16 i,j;
    U08 useless_cache[TYBC_STR_BYTE_MAX];          /* 无用缓存 */
    U16 item_total = 0;             /* 选项字符串实际个数 */
    U16 upload_bytes = 0;           /* 上传数据总字节数 */
    U08 package_total = 1;          /* 总包数(至少1包) */
    BOOL fg_multi_pkg = FALSE;      /* 多包传输需求标志 */
    BOOL fg_enum_split = FALSE;     /* 一组选项字符串分包传输标志 */
    U08 buf_len = 0;

    static U16 dict_idx_rec = 0;
    static U16 item_idx_rec = 0;    /* 记录下一包数据的参数项起始索引 */


    /* 数据包重发起始数据索引 */
    if (GETBIT_N(info->request_type, RST_RANGE_DISP_RSRC))
    {
        dict_idx_rec = 0;
        item_idx_rec = 0;
        CLRBIT_N(info->request_type, RST_RANGE_DISP_RSRC);
    }

    /* 寻找当前数据包的起始数据索引(第0包数据) */
    if (GETBIT_N(info->request_type, CONT_RANGE_DISP_RSRC))
    {
        if (info->pkg_id == 0)
        {
            dict_idx_rec = 0;
            item_idx_rec = 0;
        }
    }


    /* 计算总包数、数据起始索引 */
    for (i = 0; i < TYBC_PARAM_DICT_NUM; i++)
    {
        for (j = 0; j < TYBC_PARAM_DICT_ITEM_NUM(i); j++)
        {
            U08 item_msg_len = TYB_SLP_GET_CODE_STR(useless_cache, TYBC_PARAM_DICT_ITEM(i, j));

            upload_bytes += DP_FIELD_DISP_RSRC_BYTES + item_msg_len;

            if (upload_bytes > TRANS_03_CONTENT_BYTES)
            {
                package_total++;    /* 总包数累加 */
                upload_bytes = DP_FIELD_DISP_RSRC_BYTES + item_msg_len;

                /* 寻找当前数据包的起始数据索引(非第0包数据) */
                if (GETBIT_N(info->request_type, CONT_RANGE_DISP_RSRC))
                {
                    /* 当数据更新至请求包号时，记录此时的起始索引 */
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

    /* 填充帧头 */
    buf[0] = info->trigger_src;      /* 触发源 */
    buf[1] = info->verif_code[0];    /* 验证码 */
    buf[2] = info->verif_code[1];    /* 验证码 */
    buf[3] = info->verif_code[2];    /* 验证码 */
    buf[4] = package_total;          /* 总包数 */
    buf[5] = info->pkg_id;           /* 当前包号 */
    buf[6] = info->trans_code;       /* 传送码 */

    /* 填充中间信息: 选项字符串个数 */
    buf[7] = HIBYTE(item_total);     /* 选项字符串实际个数 */
    buf[8] = LOBYTE(item_total);     /* 选项字符串实际个数 */

    buf_len += TRANS_03_HEADER_BYTES;

    /* 传输当前组的枚举型参数的取值范围字符串 */
    for (i = dict_idx_rec; i < TYBC_PARAM_DICT_NUM; i++)
    {
        for (j = item_idx_rec; j < TYBC_PARAM_DICT_ITEM_NUM(i); j++)
        {
            U16 msg_id = TYBC_PARAM_DICT_ITEM(i, j);
            U08 msg_len = TYB_SLP_GET_CODE_STR(useless_cache, msg_id);
            U08 range_dict_id = i;

            upload_bytes = buf_len + DP_FIELD_DISP_RSRC_BYTES + msg_len;

            if (upload_bytes <= RAW_DATA_BYTES)  // 因为buf_len中已算入帧头字节，此处要相应增加帧头字节
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


    /* 传输下一包 */
    info->pkg_id++;


    /* 参数名字符串&当前值字符串全部填充完成 */
    if (!fg_multi_pkg)
    {
        dict_idx_rec = 0;
        item_idx_rec = 0;                         /* 清空枚举起始索引 */
        info->pkg_id = 0;                         /* 当前包号复位 */
        info->trans_code = TRANS_NULL;
        fg_has_trans_task = FALSE;
    }

    /* 重传数据包时，不继续传后续传送码，结束通道传输 */
    if (GETBIT_N(info->request_type, CONT_RANGE_DISP_RSRC))
    {
        fg_has_trans_task = FALSE;
        CLRBIT_N(info->request_type, CONT_RANGE_DISP_RSRC);
    }
}


/*******************************************************************************
 函 数: TYB_SLP_trans_param_item_disp_rsrc()
 功 能: 分包上传当前组参数名显示资源 (通道传输协议，传送码:0x04)
 参 数: buf--功能点缓存区;
        info--传输请求信息;
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TYB_SLP_trans_param_item_disp_rsrc(U08 *buf, TYB_TRANS_INFO *info)
{
    U16 i,j;
    U08 useless_cache[TYBC_STR_BYTE_MAX];               /* 无用缓存 */
    DP_FIELD_DISP_RSRC disp_rsrc;        /* 当前组参数名显示资源 */
    U16 upload_bytes = 0;                /* 上传数据总字节数 */
    U08 package_total = 1;               /* 总包数(至少1包) */
    BOOL fg_multi_pkg = FALSE;           /* 多包传输需求标志 */
    U16 *src_buf = NULL;
    U08 buf_len = 0;
    U08 param_grp_id = info->addr[0];

    static U16 idx_rec = 0;              /* 记录下一包数据的参数项起始索引 */


    /* 数据包重发起始数据索引 */
    if (GETBIT_N(info->request_type, RST_ITEM_DISP_RSRC))
    {
        idx_rec = 0;
        CLRBIT_N(info->request_type, RST_ITEM_DISP_RSRC);
    }

    /* 寻找当前数据包的起始数据索引(第0包数据) */
    if (GETBIT_N(info->request_type, CONT_ITEM_DISP_RSRC))
    {
        if (info->pkg_id == 0)
        {
            idx_rec = 0;
        }
    }

    /* 计算总包数、数据起始索引 */
    for (i = 0; i < TYBC_PARAM_GRP_SIZE; i++)
    {
        if (i < TYBC_PARAM_ITEM_NUM(param_grp_id))
        {
            U08 msg_len = TYB_SLP_GET_CODE_STR(useless_cache, TYBC_PARAM_ITEM_TITLE(param_grp_id,i));

            /* 计算总字节数 */
            upload_bytes += DP_FIELD_DISP_RSRC_BYTES + msg_len;

            if (upload_bytes > TRANS_04_CONTENT_BYTES)
            {
                package_total++;    /* 总包数累加 */
                upload_bytes = DP_FIELD_DISP_RSRC_BYTES + msg_len;

                /* 寻找当前数据包的起始数据索引(非第0包数据) */
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

    /* 清空上报字节数计数 */
    upload_bytes = 0;

    /* 填充帧头 */
    buf[0] = info->trigger_src;          /* 触发源 */
    buf[1] = info->verif_code[0];        /* 验证码 */
    buf[2] = info->verif_code[1];        /* 验证码 */
    buf[3] = info->verif_code[2];        /* 验证码 */
    buf[4] = package_total;              /* 总包数 */
    buf[5] = info->pkg_id;               /* 当前包号 */
    buf[6] = info->trans_code;           /* 传送码 */

    /* 填充中间信息: 当前参数组ID */
    buf[7] = param_grp_id;                        /* 当前参数组ID */
    buf[8] = TYBC_PARAM_ITEM_NUM(param_grp_id);    /* 当前组参数实际个数 */

    buf_len += TRANS_04_HEADER_BYTES;

    /* 填充内容: 当前组参数名 */
    for (i = idx_rec; i < TYBC_PARAM_GRP_SIZE; i++)
    {
        if (i < TYBC_PARAM_ITEM_NUM(param_grp_id))
        {
            disp_rsrc.msg_id = TYBC_PARAM_ITEM_TITLE(param_grp_id,i);
            disp_rsrc.msg_len = TYB_SLP_GET_CODE_STR(useless_cache, TYBC_PARAM_ITEM_TITLE(param_grp_id,i));

            /* 空间计算: 新增后的累计字节数 */
            upload_bytes = buf_len + DP_FIELD_DISP_RSRC_BYTES + disp_rsrc.msg_len;

            if (upload_bytes <= RAW_DATA_BYTES)
            {
                /* 写入上报功能点缓存区 */
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


    /* 传输下一包 */
    info->pkg_id++;


    /* 参数名字符串&当前值字符串全部填充完成 */
    if (!fg_multi_pkg)
    {
        idx_rec = 0;                         /* 清空枚举起始索引 */
        info->pkg_id = 0;                         /* 当前包号复位 */
        if (GETBIT_N(info->request_type, RST_ITEM_DISP_ALL))    /* 请求所有时，接着发下一个传送码 */
        {
            info->trans_code = TRANS_PARAM_ITEM_DISP_CFG;             /* 准备发下一个传送码 */
            CLRBIT_N(info->request_type, RST_ITEM_DISP_ALL);
        }
        else    /* 重发最后一包数据或请求所有显示资源数据，结束传输 */
        {
            info->trans_code = TRANS_NULL;
            fg_has_trans_task = FALSE;
        }
    }

    /* 重传数据包时，不继续传后续传送码，结束通道传输 */
    if (GETBIT_N(info->request_type, CONT_ITEM_DISP_RSRC))
    {
        fg_has_trans_task = FALSE;
        CLRBIT_N(info->request_type, CONT_ITEM_DISP_RSRC);
    }
}


/*******************************************************************************
 函 数: TYB_SLP_trans_param_item_disp_cfg()
 功 能: 分包上传当前组参数名(+当前值字符串)配置信息 (通道传输协议，传送码:0x05)
 参 数: buf--功能点缓存区;
        info--传输请求信息;
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TYB_SLP_trans_param_item_disp_cfg(U08 *buf, TYB_TRANS_INFO *info)
{
    U16 i,j;
    DP_FIELD_PARAM_ITEM_DISP_CFG disp_cfg;    /* 当前组参数配置信息 */
    U16 upload_bytes = 0;                     /* 上传数据总字节数 */
    U08 package_total = 1;                    /* 总包数(至少1包) */
    BOOL fg_multi_pkg = FALSE;                /* 多包传输需求标志 */
    U16 *src_buf = NULL;
    U08 buf_len = 0;
    U08 param_grp_id = info->addr[0];

    static U16 idx_rec = 0;             /* 记录下一包数据的起始数据索引 */


    /* 数据包重发起始数据索引 */
    if (GETBIT_N(info->request_type, RST_ITEM_DISP_CFG))
    {
        idx_rec = 0;
        CLRBIT_N(info->request_type, RST_ITEM_DISP_CFG);
    }

    /* 寻找当前数据包的起始数据索引(第0包数据) */
    if (GETBIT_N(info->request_type, CONT_ITEM_DISP_CFG))
    {
        if (info->pkg_id == 0)
        {
            idx_rec = 0;
        }
    }


    /* 计算总包数、数据起始索引 */
    for (i = 0; i < TYBC_PARAM_GRP_SIZE; i++)
    {
        if (i < TYBC_PARAM_ITEM_NUM(param_grp_id))
        {
            upload_bytes += DP_FIELD_PARAM_ITEM_DISP_CFG_BYTES;

            if (upload_bytes > TRANS_05_CONTENT_BYTES)
            {
                package_total++;  // 总包数累加
                upload_bytes = DP_FIELD_PARAM_ITEM_DISP_CFG_BYTES;

                /* 寻找当前数据包的起始数据索引(非第0包数据) */
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

    /* 数据填充功能点 */
    // 帧头信息
    buf[0] = info->trigger_src;
    buf[1] = info->verif_code[0];        /* 验证码 */
    buf[2] = info->verif_code[1];        /* 验证码 */
    buf[3] = info->verif_code[2];        /* 验证码 */
    buf[4] = package_total;              /* 总包数 */
    buf[5] = info->pkg_id;               /* 当前包号 */
    buf[6] = info->trans_code;           /* 传送码 */

    /* 填充中间信息 */
    buf[7] = param_grp_id;                        /* 当前参数组号 */
    buf[8] = TYBC_PARAM_ITEM_NUM(param_grp_id);    /* 当前组参数实际个数 */

    buf_len += TRANS_05_HEADER_BYTES;

    /* 传输内容 */
    for (i = idx_rec; i < TYBC_PARAM_GRP_SIZE; i++)
    {
        if (i < TYBC_PARAM_ITEM_NUM(param_grp_id))
        {

            disp_cfg.msg_id = TYBC_PARAM_ITEM_TITLE(param_grp_id,i);    /* 参数名ID */
            disp_cfg.attr = TYBC_PARAM_ITEM_ATTR(param_grp_id,(U08)i);  /* 参数属性 */
            disp_cfg.value = TYBC_PARAM_ITEM_VAL(param_grp_id,i);       /* 参数当前值 */
            disp_cfg.max = TYBC_PARAM_MAX(param_grp_id,i);              /* 参数最大值 */
            disp_cfg.min = TYBC_PARAM_MIN(param_grp_id,i);              /* 参数最小值 */

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

    /* 传输下一包 */
    info->pkg_id++;

    /* 参数名字符串&当前值字符串全部填充完成 */
    if (!fg_multi_pkg)
    {
        idx_rec = 0;                          /* 记录清空 */
        info->pkg_id = 0;                     /* 当前包ID复位 */
        info->trans_code = TRANS_NULL;
        fg_has_trans_task = FALSE;
    }

    /* 重传数据包时，不继续传后续传送码，结束通道传输 */
    if (GETBIT_N(info->request_type, CONT_ITEM_DISP_CFG))
    {
        fg_has_trans_task = FALSE;
        CLRBIT_N(info->request_type, CONT_ITEM_DISP_CFG);
    }
}


/*******************************************************************************
 函 数: TYB_SLP_parse_param_cmd()
 功 能: 解析参数设置命令
 参 数: buf--接收缓冲区(数据源);
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TYB_SLP_parse_param_cmd(U08 *buf)
{
    TYB_TRANS_INFO info;
    U08 buf_len = 0;

    /* 初始化 */
    ARRCLR(info.addr);
    ARRCLR(info.request_type);

    /* 触发源不为"参数设置"，则不继续解析命令 */
    if (buf[buf_len] != TRANS_PARAM)
    {
        return;
    }

    /* 第1部分:触发源("状态查询"界面) */
    info.trigger_src = buf[buf_len++];

    /* 第2部分:验证码 */
    MEMCPY(info.verif_code, &buf[buf_len], TRANS_VERIF_CODE_LEN);
    buf_len += TRANS_VERIF_CODE_LEN;

    /* 第3部分: 解析码 */
    info.parse_code = buf[buf_len++];

    /* 第4部分: 请求码 */
    info.request_code = buf[buf_len++];

    /* 第5部分: 请求包号 */
    info.pkg_id = buf[buf_len++];   // 从指定数据包开始，总包数内部计算

    /* 第6部分: 参数组号 */
    info.addr[0] = buf[buf_len++];

    /* 第7部分: 参数项号 */
    info.addr[1] = buf[buf_len++];


    switch (info.parse_code)
    {
        /* 加载参数组名列表 */
        case TRANS_CMD_PARAM_GRP_DISP:
            switch (info.request_code)
            {
                /* 从头开始、全部发送 */
                case UPLOAD_DISP_ALL:
                    info.trans_code = TRANS_PARAM_GRP_DISP_RSRC;
                    SETBIT_N(info.request_type, RST_GRP_DISP_ALL);
                    SETBIT_N(info.request_type, RST_GRP_DISP_RSRC);
                    SETBIT_N(info.request_type, RST_GRP_DISP_CFG);
                    break;

                /* 发送显示资源中的某包数据 */
                case UPLOAD_ONE_DISP_RSRC:
                    info.trans_code = TRANS_PARAM_GRP_DISP_RSRC;
                    SETBIT_N(info.request_type, CONT_GRP_DISP_RSRC);
                    break;

                /* 发送配置信息中的某包数据 */
                case UPLOAD_ONE_DISP_CFG:
                    info.trans_code = TRANS_PARAM_GRP_DISP_CFG;
                    SETBIT_N(info.request_type, CONT_GRP_DISP_CFG);
                    break;

                /* 发送显示资源的所有数据包(参数组列表) */
                case UPLOAD_ALL_DISP_RSRC:
                    info.trans_code = TRANS_PARAM_GRP_DISP_RSRC;
                    SETBIT_N(info.request_type, RST_GRP_DISP_RSRC);
                    break;

                /* 发送配置信息的所有数据包(参数组列表) */
                case UPLOAD_ALL_DISP_CFG:
                    info.trans_code = TRANS_PARAM_GRP_DISP_CFG;
                    SETBIT_N(info.request_type, RST_GRP_DISP_CFG);
                    break;
            }

            /* 推入队列 */
            if (TYB_SLP_is_node_unique(&info))
            {
                TYB_SLP_cache_append(&info);
            }
            break;


        /* 加载参数选项 */
        case TRANS_CMD_PARAM_RANGE_DISP:
            switch (info.request_code)
            {
                /* 从头开始、全部发送 */
                case UPLOAD_DISP_ALL:
                    info.trans_code = TRANS_PARAM_RANGE_DISP_RSRC;
                    SETBIT_N(info.request_type, RST_RANGE_DISP_RSRC);
                    break;

                /* 发送显示资源中的某包数据 */
                case UPLOAD_ONE_DISP_RSRC:
                    info.trans_code = TRANS_PARAM_RANGE_DISP_RSRC;
                    SETBIT_N(info.request_type, CONT_RANGE_DISP_RSRC);
                    break;
            }

            /* 推入队列 */
            if (TYB_SLP_is_node_unique(&info))
            {
                TYB_SLP_cache_append(&info);
            }

            break;

        /* 选择参数组，加载参数项列表 */
        case TRANS_CMD_PARAM_ITEM_DISP:
            switch (info.request_code)
            {
                /* 从头开始、全部发送 */
                case UPLOAD_DISP_ALL:
                    info.trans_code = TRANS_PARAM_ITEM_DISP_RSRC;
                    SETBIT_N(info.request_type, RST_ITEM_DISP_ALL);
                    SETBIT_N(info.request_type, RST_ITEM_DISP_RSRC);
                    SETBIT_N(info.request_type, RST_ITEM_DISP_CFG);
                    break;

                /* 发送显示资源中的某包数据 */
                case UPLOAD_ONE_DISP_RSRC:
                    info.trans_code = TRANS_PARAM_ITEM_DISP_RSRC;
                    SETBIT_N(info.request_type, CONT_ITEM_DISP_RSRC);
                    break;

                /* 发送配置信息中的某包数据 */
                case UPLOAD_ONE_DISP_CFG:
                    info.trans_code = TRANS_PARAM_ITEM_DISP_CFG;
                    SETBIT_N(info.request_type, CONT_ITEM_DISP_CFG);
                    break;

                /* 发送显示资源的所有数据包(当前组参数列表) */
                case UPLOAD_ALL_DISP_RSRC:
                    info.trans_code = TRANS_PARAM_ITEM_DISP_RSRC;
                    SETBIT_N(info.request_type, RST_ITEM_DISP_RSRC);
                    break;

                /* 发送配置信息的所有数据包(当前组参数列表) */
                case UPLOAD_ALL_DISP_CFG:
                    info.trans_code = TRANS_PARAM_ITEM_DISP_CFG;
                    SETBIT_N(info.request_type, RST_ITEM_DISP_CFG);
                    break;
            }

            /* 推入队列 */
            if (TYB_SLP_is_node_unique(&info))
            {
                TYB_SLP_cache_append(&info);
            }

            break;

        /* 设定参数(注意: 不推入队列) */
        case TRANS_CMD_PARAM_VAL_SET:
            TYBC_SET_PARAM(info.addr[0],info.addr[1],MAKEWORD(buf[buf_len],buf[buf_len+1]));
            break;
    }
}

#endif

#ifdef TUYA_USE_SYS_STAT

/*******************************************************************************
 函 数: TYB_SLP_trans_sys_msg_disp_rsrc()
 功 能: 分包上传系统状态字符串显示资源(通道传输协议，传送码:)
 参 数: buf--功能点缓存区;
        info--传输请求信息;
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TYB_SLP_trans_sys_msg_disp_rsrc(U08 *buf, TYB_TRANS_INFO *info)
{
    U16 i, j;
    U08 useless_cache[TYBC_STR_BYTE_MAX];	/* 无用缓存 */
    U16 upload_bytes = 0;          /* 上传数据总字节数 */
    U08 package_total = 1;         /* 总包数(至少1包) */
    BOOL fg_multi_pkg = FALSE;     /* 多包传输需求标志 */
    U08 buf_len = 0;

    static U16 idx_rec = 0;        /* 记录下一包数据的起始数据索引 */


    /* 数据包重发起始数据索引 */
    if (GETBIT_N(info->request_type, RST_SYS_MSG_DISP_RSRC))
    {
        idx_rec = 0;
        CLRBIT_N(info->request_type, RST_SYS_MSG_DISP_RSRC);
    }

    /* 寻找当前数据包的起始数据索引(第0包数据) */
    if (GETBIT_N(info->request_type, CONT_SYS_MSG_DISP_RSRC))
    {
        if (info->pkg_id == 0)
        {
            idx_rec = 0;
        }
    }

    /* 计算总包数、数据起始索引 */
    for (i = 0; i < TYBC_SYS_PV_IN_USED; i++)
    {
        U08 msg_len = 0;
		U16 str_idx;
		U16 str_idx_before;
		BOOL fg_fill_str;
        /* 值为字符串的模拟量 */
        if (TYBC_SYS_PV_ATTR(i) == 0xFFFE)
        {
        	str_idx = (U16)(TYBC_PV_VAL(TYBC_PCB_MAX, i));
	        msg_len = TYB_SLP_GET_CODE_STR(useless_cache, str_idx);
			fg_fill_str = TRUE;
			//有重复字符串，不填充
			for (j=0; j<i; j++)
			{
				str_idx_before = (U16)(TYBC_PV_VAL(TYBC_PCB_MAX, j));
				if (str_idx == str_idx_before)
				{
					fg_fill_str = FALSE;
					break;
				}
			}
			//始终填充第一个
			if (i == 0)
			{
				fg_fill_str= TRUE;
			}

			if (fg_fill_str)
			{
			    upload_bytes += DP_FIELD_SYS_MSG_DATA_BYTES + msg_len;

			    if (upload_bytes > TRANS_13_CONTENT_BYTES)
			    {
			        package_total++;    /* 总包数累加 */
			        upload_bytes = DP_FIELD_SYS_MSG_DATA_BYTES + msg_len;

			        /* 寻找当前数据包的起始数据索引(非第0包数据) */
			        if (GETBIT_N(info->request_type, CONT_SYS_MSG_DISP_RSRC))
			        {
			            /* 当数据更新至请求包号时，记录此时的起始索引 */
			            if (info->pkg_id == (package_total-1))
			            {
			                idx_rec = i;
			            }
			        }
			    }
			}


		}
    }

    /* 填充帧头 */
    buf[0] = info->trigger_src;          /* 触发源 */
    buf[1] = info->verif_code[0];        /* 验证码 */
    buf[2] = info->verif_code[1];        /* 验证码 */
    buf[3] = info->verif_code[2];        /* 验证码 */
    buf[4] = package_total;              /* 总包数 */
    buf[5] = info->pkg_id;               /* 当前包号 */
    buf[6] = info->trans_code;           /* 传送码 */

    /* 填充中间信息 */
    buf[7] = 0;               /* 当前模块号 */
    buf[8] = 0;              /* 上报状态数 */

    buf_len += TRANS_13_HEADER_BYTES;

    /* 填充内容: 当前模块状态的显示资源 */
    for (i = idx_rec; i < TYBC_SYS_PV_IN_USED; i++)
    {
		U08 msg_len = 0;
		U16 msg_id = 0;
		U16 str_idx;
		U16 str_idx_before;
		BOOL fg_fill_str;
        /* 值为字符串的模拟量 */
        if (TYBC_SYS_PV_ATTR(i) == 0xFFFE)
        {
        	str_idx = (U16)(TYBC_PV_VAL(TYBC_PCB_MAX, i));
			msg_len = TYB_SLP_GET_CODE_STR(useless_cache, str_idx);
			msg_id = TYBC_PV_VAL(TYBC_PCB_MAX, i);
			fg_fill_str = TRUE;
			//有重复字符串，不填充
			for (j=0; j<i ;j++)
			{
				str_idx_before = (U16)(TYBC_PV_VAL(TYBC_PCB_MAX, j));
				if (str_idx == str_idx_before)
				{
					fg_fill_str = FALSE;
					break;
				}
			}
			//始终填充第一个
			if (i == 0)
			{
				fg_fill_str= TRUE;
			}

			if (fg_fill_str)
			{
		        /* 空间计算: 新增后的累计字节数 */
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

    /* 传输下一包 */
    info->pkg_id++;

    /* 所有参数组名显示资源上传完成 */
    if (!fg_multi_pkg)
    {
        idx_rec = 0;                                           /* 记录清空 */
        info->pkg_id = 0;                                      /* 当前包ID复位 */
        if (GETBIT_N(info->request_type, RST_SYS_STAT_DISP_ALL))    /* 请求所有时，接着发下一个传送码 */
        {
            info->trans_code = TRANS_SYS_STAT_DISP_RSRC;       /* 准备发下一个传送码 */
            //CLRBIT_N(info->request_type, RST_SYS_STAT_DISP_ALL);
        }
        else    /* 重发最后一包数据或请求所有显示资源数据，结束传输 */
        {
            info->trans_code = TRANS_NULL;
            fg_has_trans_task = FALSE;
        }
    }

    /* 重传数据包时，不继续传后续传送码，结束通道传输 */
    if (GETBIT_N(info->request_type, CONT_SYS_MSG_DISP_RSRC))
    {
        fg_has_trans_task = FALSE;
        CLRBIT_N(info->request_type, CONT_SYS_MSG_DISP_RSRC);
    }
}

/*******************************************************************************
 函 数: TYB_SLP_trans_sys_stat_disp_rsrc()
 功 能: 分包上传当前模块状态显示资源(通道传输协议，传送码:0x06)
 参 数: buf--功能点缓存区;
        info--传输请求信息;
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TYB_SLP_trans_sys_stat_disp_rsrc(U08 *buf, TYB_TRANS_INFO *info)
{
    U16 i;
    U08 useless_cache[TYBC_STR_BYTE_MAX];        /* 无用缓存 */
    U16 upload_bytes = 0;         /* 上传数据总字节数 */
    U08 package_total = 1;        /* 总包数(至少1包) */
    U08 upload_pv_num = 0;        /* 模拟量上报总数 */
    BOOL fg_multi_pkg = FALSE;    /* 多包传输需求标志 */
    U08 buf_len = 0;
	U16 attr;

    static U16 idx_rec = 0;       /* 记录下一包数据的起始数据索引 */


    /* 数据包重发起始数据索引 */
    if (GETBIT_N(info->request_type, RST_SYS_STAT_DISP_RSRC))
    {
        idx_rec = 0;
        CLRBIT_N(info->request_type, RST_SYS_STAT_DISP_RSRC);
    }

    /* 寻找当前数据包的起始数据索引(第0包数据) */
    if (GETBIT_N(info->request_type, CONT_SYS_STAT_DISP_RSRC))
    {
        if (info->pkg_id == 0)
        {
            idx_rec = 0;
        }
    }


    /* 标记需要上传的模拟量 */
    if (!TYB_SLP_mark_upload_pv(TYBC_PCB_MAX, &upload_pv_num))
    {
        return;
    }

    /* 计算总包数、数据起始索引 */
    for (i = 0; i < TYBC_SYS_PV_MAX; i++)
    {
        U08 msg_len = 0;
        /* 有效模拟量 */
        if (GETBIT_N(fg_upload_pv, i))
        {
	        msg_len = TYB_SLP_GET_CODE_STR(useless_cache, TYBC_SYS_PV_TITLE(i));

	        upload_bytes += DP_FIELD_SYS_STAT_INFO_BYTES+ msg_len;

	        if (upload_bytes > TRANS_06_CONTENT_BYTES)
	        {
	            package_total++;    /* 总包数累加 */
	            upload_bytes = DP_FIELD_SYS_STAT_INFO_BYTES + msg_len;

	            /* 寻找当前数据包的起始数据索引(非第0包数据) */
	            if (GETBIT_N(info->request_type, CONT_SYS_STAT_DISP_RSRC))
	            {
	                /* 当数据更新至请求包号时，记录此时的起始索引 */
	                if (info->pkg_id == (package_total-1))
	                {
	                    idx_rec = i;
	                }
	            }
	        }
		}
    }

    /* 清空上报字节数计数 */
    upload_bytes = 0;

    /* 填充帧头 */
    buf[0] = info->trigger_src;          /* 触发源 */
    buf[1] = info->verif_code[0];        /* 验证码 */
    buf[2] = info->verif_code[1];        /* 验证码 */
    buf[3] = info->verif_code[2];        /* 验证码 */
    buf[4] = package_total;              /* 总包数 */
    buf[5] = info->pkg_id;               /* 当前包号 */
    buf[6] = info->trans_code;           /* 传送码 */

    /* 填充中间信息 */
    buf[7] = TYBC_PCB_MAX;               /* 当前模块号 */
    buf[8] = upload_pv_num;              /* 上报状态数 */

    buf_len += TRANS_06_HEADER_BYTES;

    /* 填充内容: 当前模块状态的显示资源 */
    for (i = idx_rec; i < TYBC_SYS_PV_MAX; i++)
    {
		U08 msg_len = 0;
        /* 有效模拟量 */
        if (GETBIT_N(fg_upload_pv, i))
        {
			msg_len = TYB_SLP_GET_CODE_STR(useless_cache, TYBC_SYS_PV_TITLE(i));
            /* 空间计算: 新增后的累计字节数 */
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

    /* 传输下一包 */
    info->pkg_id++;

    /* 结束标志 */
    if (buf_len != 0 && buf_len <= RAW_DATA_BYTES - 2)
    {
        buf[buf_len++] = 0xff;
        buf[buf_len++] = 0xff;
    }

    /* 所有模块状态显示资源上传完成 */
    if (!fg_multi_pkg)
    {
        idx_rec = 0;          /* 记录清空 */
        info->pkg_id = 0;     /* 当前包ID复位 */

        if (GETBIT_N(info->request_type, RST_SYS_STAT_DISP_ALL))    /* 请求所有时，接着发下一个传送码 */
        {
            info->trans_code = TRANS_SYS_STAT_DISP_CFG;
            CLRBIT_N(info->request_type, RST_SYS_STAT_DISP_ALL);
        }
        else    /* 重发最后一包数据或请求所有显示资源数据，结束传输 */
        {
            info->trans_code = TRANS_NULL;
            fg_has_trans_task = FALSE;
        }
    }

    /* 重传某包数据时，不继续传后续传送码，结束通道传输 */
    if (GETBIT_N(info->request_type, CONT_SYS_STAT_DISP_RSRC))
    {
        fg_has_trans_task = FALSE;
        CLRBIT_N(info->request_type, CONT_SYS_STAT_DISP_RSRC);
    }
}


/*******************************************************************************
 函 数: TYB_SLP_trans_sys_stat_disp_cfg()
 功 能: 分包上传当前模块状态配置信息 (通道传输协议，传送码:0x07)
 参 数: buf--功能点缓存区;
        info--传输请求信息;
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TYB_SLP_trans_sys_stat_disp_cfg(U08 *buf, TYB_TRANS_INFO *info)
{
    U16 i;
    U16 upload_bytes = 0;                   /* 上传数据总字节数 */
    U08 package_total = 1;                  /* 总包数(至少1包) */
    U08 upload_pv_num = 0;                  /* 模拟量上报总数 */
    BOOL fg_multi_pkg = FALSE;              /* 多包传输需求标志 */
    U08 buf_len = 0;
	U16 attr;
    static U16 idx_rec = 0;                 /* 记录下一包数据的起始数据索引 */


    /* 数据包重发起始数据索引 */
    if (GETBIT_N(info->request_type, RST_SYS_STAT_DISP_CFG))
    {
        idx_rec = 0;
        CLRBIT_N(info->request_type, RST_SYS_STAT_DISP_CFG);
    }

    /* 寻找当前数据包的起始数据索引(第0包数据) */
    if (GETBIT_N(info->request_type, CONT_SYS_STAT_DISP_CFG))
    {
        if (info->pkg_id == 0)
        {
            idx_rec = 0;
        }
    }

    /* 标记需要上传的模拟量 */
    if (!TYB_SLP_mark_upload_pv(TYBC_PCB_MAX, &upload_pv_num))
    {
        return;
    }

    /* 计算总包数、数据起始索引 */
    for (i = 0; i < TYBC_PCB_MAX; i++)
    {
        if (GETBIT_N(fg_upload_pv, i))  /* 有效模拟量 */
        {
            upload_bytes += DP_FIELD_SYS_STAT_DATA_BYTES;

            if (upload_bytes > TRANS_07_CONTENT_BYTES)
            {
                package_total++;  // 总包数累加
                upload_bytes = DP_FIELD_SYS_STAT_DATA_BYTES;

                /* 寻找当前数据包的起始数据索引(非第0包数据) */
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

    /* 清空上报字节数计数 */
    upload_bytes = 0;

    /* 填充帧头 */
    buf[0] = info->trigger_src;      /* 触发源 */
    buf[1] = info->verif_code[0];    /* 验证码 */
    buf[2] = info->verif_code[1];    /* 验证码 */
    buf[3] = info->verif_code[2];    /* 验证码 */
    buf[4] = package_total;          /* 总包数 */
    buf[5] = info->pkg_id;           /* 当前包号 */
    buf[6] = info->trans_code;       /* 传送码 */

    /* 填充中间信息 */
    buf[7] = TYBC_PCB_MAX;           /* 当前模块号 */
    buf[8] = upload_pv_num;          /* 上报状态数 */

    buf_len += TRANS_07_HEADER_BYTES;

    /* 填充内容: 当前模块状态的配置信息 */
    for (i = idx_rec; i < TYBC_SYS_PV_MAX; i++)
    {
        if (GETBIT_N(fg_upload_pv, i))
        {
            /* 空间计算: 新增后的累计字节数 */
            upload_bytes = buf_len + DP_FIELD_SYS_STAT_DATA_BYTES;

            /* 写入上报功能点缓存区 */
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

    /* 传输下一包 */
    info->pkg_id++;

    /* 结束标志 */
    if (buf_len != 0 && buf_len <= RAW_DATA_BYTES - 2)
    {
        buf[buf_len++] = 0xff;
        buf[buf_len++] = 0xff;
    }

    /* 所有参数组名配置信息上传完成 */
    if (!fg_multi_pkg)
    {
        idx_rec = 0;                                // 记录清空
        info->pkg_id = 0;                           // 当前包ID复位
        info->trans_code = TRANS_NULL;
        fg_has_trans_task = FALSE;
    }

    /* 重传数据包时，不继续传后续传送码，结束通道传输 */
    if (GETBIT_N(info->request_type, CONT_SYS_STAT_DISP_CFG))
    {
        fg_has_trans_task = FALSE;
        CLRBIT_N(info->request_type, CONT_SYS_STAT_DISP_CFG);
    }

}



/*******************************************************************************
 函 数: TYB_SLP_parse_sys_stat_cmd()
 功 能: 解析模块状态命令
 参 数: buf--接收缓冲区(数据源);
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TYB_SLP_parse_sys_stat_cmd(U08 *buf)
{
    TYB_TRANS_INFO info;
    U08 buf_len = 0;

    /* 初始化 */
    ARRCLR(info.addr);
    ARRCLR(info.request_type);

    /* 触发源不为"状态查询"，则不继续解析命令 */
    if (buf[buf_len] != TRANS_MOD_STAT)
    {
        return;
    }

    /* 第1部分:触发源("状态查询"界面) */
    info.trigger_src = buf[buf_len++];

    /* 第2部分:验证码 */
    MEMCPY(info.verif_code, &buf[buf_len], TRANS_VERIF_CODE_LEN);
    buf_len += TRANS_VERIF_CODE_LEN;

    /* 第3部分: 解析码 */
    info.parse_code = buf[buf_len++];

    /* 第4部分: 请求码 */
    info.request_code = buf[buf_len++];

    /* 第5部分: 请求包号 */
    info.pkg_id = buf[buf_len++];   // 从指定数据包开始，总包数内部计算

    /* 第6部分: 模块号 */
    info.addr[0] = buf[buf_len++];

    switch (info.parse_code)
    {
        /* 加载模块状态信息 */
        case TRANS_CMD_SYS_STATUS_DISP:
            switch (info.request_code)
            {
                /* 从头开始、全部发送 */
                case UPLOAD_DISP_ALL:
                    info.trans_code = TRANS_SYS_MSG_DISP_RSRC;
                    SETBIT_N(info.request_type, RST_SYS_STAT_DISP_ALL);
					SETBIT_N(info.request_type, RST_SYS_MSG_DISP_RSRC);
                    SETBIT_N(info.request_type, RST_SYS_STAT_DISP_RSRC);
                    SETBIT_N(info.request_type, RST_SYS_STAT_DISP_CFG);
                    break;

                /* 发送配置信息的所有数据包(当前模块) */
                case UPLOAD_ALL_DISP_CFG:
                    info.trans_code = TRANS_MOD_STAT_DISP_CFG;
                    SETBIT_N(info.request_type, RST_SYS_STAT_DISP_CFG);
                    break;
            }

            /* 推入队列 */
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

/* 有"模块状态" */
#ifdef TUYA_USE_MOD_STAT

/*******************************************************************************
 函 数: TYB_SLP_trans_mod_msg_disp_rsrc()
 功 能: 分包上传模块状态字符串显示资源(通道传输协议，传送码:)
 参 数: buf--功能点缓存区;
        info--传输请求信息;
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TYB_SLP_trans_mod_msg_disp_rsrc(U08 *buf, TYB_TRANS_INFO *info)
{
    U16 i, j, pv_id;
    U08 useless_cache[TYBC_STR_BYTE_MAX];	/* 无用缓存 */
    U16 upload_bytes = 0;          /* 上传数据总字节数 */
    U08 package_total = 1;         /* 总包数(至少1包) */
    U08 upload_pv_num = 0;         /* 模拟量上报总数 */
    BOOL fg_multi_pkg = FALSE;     /* 多包传输需求标志 */
    U08 buf_len = 0;

    static U16 idx_rec = 0;        /* 记录下一包数据的起始数据索引 */
	U08 mod_id = info->addr[0];

    /* 数据包重发起始数据索引 */
    if (GETBIT_N(info->request_type, RST_MOD_MSG_DISP_RSRC))
    {
        idx_rec = 0;
        CLRBIT_N(info->request_type, RST_MOD_MSG_DISP_RSRC);
    }

    /* 寻找当前数据包的起始数据索引(第0包数据) */
    if (GETBIT_N(info->request_type, CONT_MOD_MSG_DISP_RSRC))
    {
        if (info->pkg_id == 0)
        {
            idx_rec = 0;
        }
    }

    /* 标记需要上传的模拟量 */
    if (!TYB_SLP_mark_upload_pv(mod_id, &upload_pv_num))
    {
        return;
    }

    /* 计算总包数、数据起始索引 */
    for (i = 0; i < TYBC_PV_MAX; i++)
    {
        U08 msg_len = 0;
		U16 str_idx;
		U16 str_idx_before;
		BOOL fg_fill_str;

        if (i < TYBC_UNIT_PV_BEGIN)	//压机模拟量
        {
            pv_id = i % TYBC_COMP_PV_MAX;
		}
		else		//模块模拟量
		{
            pv_id = i - TYBC_UNIT_PV_BEGIN;
		}

        /* 值为字符串的模拟量 */
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
			//有重复字符串，不填充
			for (j=0; j<i; j++)
			{
				str_idx_before = (U16)(TYBC_PV_VAL(mod_id, j));
				if (str_idx == str_idx_before)
				{
					fg_fill_str = FALSE;
					break;
				}
			}
			//始终填充第一个
			if (i == 0)
			{
				fg_fill_str= TRUE;
			}

			if (fg_fill_str)
			{
			    upload_bytes += DP_FIELD_DISP_RSRC_BYTES + msg_len;

			    if (upload_bytes > TRANS_14_CONTENT_BYTES)
			    {
			        package_total++;    /* 总包数累加 */
			        upload_bytes = DP_FIELD_DISP_RSRC_BYTES + msg_len;

			        /* 寻找当前数据包的起始数据索引(非第0包数据) */
			        if (GETBIT_N(info->request_type, CONT_MOD_MSG_DISP_RSRC))
			        {
			            /* 当数据更新至请求包号时，记录此时的起始索引 */
			            if (info->pkg_id == (package_total-1))
			            {
			                idx_rec = i;
			            }
			        }
			    }
			}


		}
    }

    /* 填充帧头 */
    buf[0] = info->trigger_src;          /* 触发源 */
    buf[1] = info->verif_code[0];        /* 验证码 */
    buf[2] = info->verif_code[1];        /* 验证码 */
    buf[3] = info->verif_code[2];        /* 验证码 */
    buf[4] = package_total;              /* 总包数 */
    buf[5] = info->pkg_id;               /* 当前包号 */
    buf[6] = info->trans_code;           /* 传送码 */

    /* 填充中间信息 */
    buf[7] = mod_id;               /* 当前模块号 */
    buf[8] = 0;              		/* 上报状态数 */

    buf_len += TRANS_14_HEADER_BYTES;

    /* 填充内容: 当前模块状态的显示资源 */
    for (i = idx_rec; i < TYBC_PV_MAX; i++)
    {
		U08 msg_len = 0;
		U16 msg_id = 0;
		U16 str_idx;
		U16 str_idx_before;
		BOOL fg_fill_str;
        if (i < TYBC_UNIT_PV_BEGIN)	//压机模拟量
        {
            pv_id = i % TYBC_COMP_PV_MAX;
		}
		else		//模块模拟量
		{
            pv_id = i - TYBC_UNIT_PV_BEGIN;
		}


        /* 值为字符串的模拟量 */
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
			//有重复字符串，不填充
			for (j=0; j<i ;j++)
			{
				str_idx_before = (U16)(TYBC_PV_VAL(mod_id, j));
				if (str_idx == str_idx_before)
				{
					fg_fill_str = FALSE;
					break;
				}
			}
			//始终填充第一个
			if (i == 0)
			{
				fg_fill_str= TRUE;
			}

			if (fg_fill_str)
			{
		        /* 空间计算: 新增后的累计字节数 */
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

    /* 传输下一包 */
    info->pkg_id++;

    /* 所有参数组名显示资源上传完成 */
    if (!fg_multi_pkg)
    {
        idx_rec = 0;                                           /* 记录清空 */
        info->pkg_id = 0;                                      /* 当前包ID复位 */
        if (GETBIT_N(info->request_type, RST_MOD_STAT_DISP_ALL))    /* 请求所有时，接着发下一个传送码 */
        {
            info->trans_code = TRANS_MOD_STAT_DISP_RSRC;       /* 准备发下一个传送码 */
            //CLRBIT_N(info->request_type, RST_SYS_STAT_DISP_ALL);
        }
        else    /* 重发最后一包数据或请求所有显示资源数据，结束传输 */
        {
            info->trans_code = TRANS_NULL;
            fg_has_trans_task = FALSE;
        }
    }

    /* 重传数据包时，不继续传后续传送码，结束通道传输 */
    if (GETBIT_N(info->request_type, CONT_MOD_MSG_DISP_RSRC))
    {
        fg_has_trans_task = FALSE;
        CLRBIT_N(info->request_type, CONT_MOD_MSG_DISP_RSRC);
    }
}

/*******************************************************************************
 函 数: TYB_SLP_trans_mod_stat_disp_rsrc()
 功 能: 分包上传当前模块状态显示资源(通道传输协议，传送码:0x06)
 参 数: buf--功能点缓存区;
        info--传输请求信息;
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TYB_SLP_trans_mod_stat_disp_rsrc(U08 *buf, TYB_TRANS_INFO *info)
{
    U16 i,j,pv_id;
    U08 useless_cache[TYBC_STR_BYTE_MAX];        /* 无用缓存 */
    DP_FIELD_DISP_RSRC disp_rsrc; /* 模块状态显示资源 */
    U16 upload_bytes = 0;         /* 上传数据总字节数 */
    U08 package_total = 1;        /* 总包数(至少1包) */
    U08 upload_pv_num = 0;        /* 模拟量上报总数 */
    BOOL fg_multi_pkg = FALSE;    /* 多包传输需求标志 */
    U16 *src_buf = NULL;
    U08 buf_len = 0;
    U08 mod_id = info->addr[0];

    static U16 idx_rec = 0;       /* 记录下一包数据的起始数据索引 */


    /* 数据包重发起始数据索引 */
    if (GETBIT_N(info->request_type, RST_MOD_STAT_DISP_RSRC))
    {
        idx_rec = 0;
        CLRBIT_N(info->request_type, RST_MOD_STAT_DISP_RSRC);
    }

    /* 寻找当前数据包的起始数据索引(第0包数据) */
    if (GETBIT_N(info->request_type, CONT_MOD_STAT_DISP_RSRC))
    {
        if (info->pkg_id == 0)
        {
            idx_rec = 0;
        }
    }


    /* 标记需要上传的模拟量 */
    if (!TYB_SLP_mark_upload_pv(mod_id, &upload_pv_num))
    {
        return;
    }

    /* 计算总包数、数据起始索引 */
    for (i = 0; i < TYBC_MOD_PV_MSG_MAX; i++)
    {
        U08 msg_len = 0;

        if (i < TYBC_COMP_PV_IN_USED)    /* 压机模拟量 */
        {
            pv_id = i;    /* 计算压机模拟量内部索引 */
            msg_len = TYB_SLP_GET_CODE_STR(useless_cache, TYBC_COMP_PV_TITLE(pv_id));
        }
        else if (i < TYBC_COMP_PV_IN_USED + TYBC_UNIT_PV_IN_USED)    /* 单元模拟量 */
        {
            pv_id = i - TYBC_COMP_PV_IN_USED;    /* 计算单元模拟量内部索引 */
            msg_len = TYB_SLP_GET_CODE_STR(useless_cache, TYBC_UNIT_PV_TITLE(pv_id));
        }
        else
        {
            break;
        }

        upload_bytes += DP_FIELD_DISP_RSRC_BYTES + msg_len;

        if (upload_bytes > TRANS_06_CONTENT_BYTES)
        {
            package_total++;    /* 总包数累加 */
            upload_bytes = DP_FIELD_DISP_RSRC_BYTES + msg_len;

            /* 寻找当前数据包的起始数据索引(非第0包数据) */
            if (GETBIT_N(info->request_type, CONT_MOD_STAT_DISP_RSRC))
            {
                /* 当数据更新至请求包号时，记录此时的起始索引 */
                if (info->pkg_id == (package_total-1))
                {
                    idx_rec = i;
                }
            }
        }
    }

    /* 清空上报字节数计数 */
    upload_bytes = 0;

    /* 填充帧头 */
    buf[0] = info->trigger_src;          /* 触发源 */
    buf[1] = info->verif_code[0];        /* 验证码 */
    buf[2] = info->verif_code[1];        /* 验证码 */
    buf[3] = info->verif_code[2];        /* 验证码 */
    buf[4] = package_total;              /* 总包数 */
    buf[5] = info->pkg_id;               /* 当前包号 */
    buf[6] = info->trans_code;           /* 传送码 */

    /* 填充中间信息 */
    buf[7] = mod_id;                     /* 当前模块号 */
    buf[8] = upload_pv_num;              /* 上报状态数 */

    buf_len += TRANS_06_HEADER_BYTES;

    /* 填充内容: 当前模块状态的显示资源 */
    for (i = idx_rec; i < TYBC_MOD_PV_MSG_MAX; i++)
    {
        if (i < TYBC_COMP_PV_IN_USED)    /* 压机模拟量 */
        {
            pv_id = i;    /* 计算压机模拟量内部索引 */
            disp_rsrc.msg_id = TYBC_COMP_PV_TITLE(pv_id);
            disp_rsrc.msg_len = TYB_SLP_GET_CODE_STR(useless_cache, TYBC_COMP_PV_TITLE(pv_id));
        }
        else if (i < TYBC_COMP_PV_IN_USED + TYBC_UNIT_PV_IN_USED)    /* 单元模拟量 */
        {
            pv_id = i - TYBC_COMP_PV_IN_USED;    /* 计算单元模拟量内部索引 */
            disp_rsrc.msg_id = TYBC_UNIT_PV_TITLE(pv_id);
            disp_rsrc.msg_len = TYB_SLP_GET_CODE_STR(useless_cache, TYBC_UNIT_PV_TITLE(pv_id));
        }
        else
        {
            disp_rsrc.msg_id = 0;
            disp_rsrc.msg_len = 0;
        }

        /* 字符串ID非0 */
        if (disp_rsrc.msg_id != 0 && disp_rsrc.msg_len != 0)
        {
            /* 空间计算: 新增后的累计字节数 */
            upload_bytes = buf_len + DP_FIELD_DISP_RSRC_BYTES + disp_rsrc.msg_len;

            /* 写入上报功能点缓存区 */
            src_buf = (U16*)&disp_rsrc;
            if (upload_bytes <= RAW_DATA_BYTES)
            {
                for (j = 0; j < DP_FIELD_DISP_RSRC_SIZE; j++)
                {
                    buf[buf_len++] = HIBYTE(src_buf[j]);
                    buf[buf_len++] = LOBYTE(src_buf[j]);
                }

                /* 写入字符串 */
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

    /* 传输下一包 */
    info->pkg_id++;

    /* 所有模块状态显示资源上传完成 */
    if (!fg_multi_pkg)
    {
        idx_rec = 0;          /* 记录清空 */
        info->pkg_id = 0;     /* 当前包ID复位 */

        if (GETBIT_N(info->request_type, RST_MOD_STAT_DISP_ALL))    /* 请求所有时，接着发下一个传送码 */
        {
            info->trans_code = TRANS_MOD_STAT_DISP_CFG;
            CLRBIT_N(info->request_type, RST_MOD_STAT_DISP_ALL);
        }
        else    /* 重发最后一包数据或请求所有显示资源数据，结束传输 */
        {
            info->trans_code = TRANS_NULL;
            fg_has_trans_task = FALSE;
        }
    }

    /* 重传某包数据时，不继续传后续传送码，结束通道传输 */
    if (GETBIT_N(info->request_type, CONT_MOD_STAT_DISP_RSRC))
    {
        fg_has_trans_task = FALSE;
        CLRBIT_N(info->request_type, CONT_MOD_STAT_DISP_RSRC);
    }
}


/*******************************************************************************
 函 数: TYB_SLP_trans_mod_stat_disp_cfg()
 功 能: 分包上传当前模块状态配置信息 (通道传输协议，传送码:0x07)
 参 数: buf--功能点缓存区;
        info--传输请求信息;
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TYB_SLP_trans_mod_stat_disp_cfg(U08 *buf, TYB_TRANS_INFO *info)
{
    U16 i,j,pv_id,comp_id;
    DP_FIELD_MOD_STAT_DISP_CFG disp_cfg;    /* 模块状态配置信息 */
    U16 upload_bytes = 0;                   /* 上传数据总字节数 */
    U08 package_total = 1;                  /* 总包数(至少1包) */
    U08 upload_pv_num = 0;                  /* 模拟量上报总数 */
    BOOL fg_multi_pkg = FALSE;              /* 多包传输需求标志 */
    U16 *src_buf = NULL;
    U08 buf_len = 0;
    U08 mod_id = info->addr[0];

    static U16 idx_rec = 0;                 /* 记录下一包数据的起始数据索引 */


    /* 数据包重发起始数据索引 */
    if (GETBIT_N(info->request_type, RST_MOD_STAT_DISP_CFG))
    {
        idx_rec = 0;
        CLRBIT_N(info->request_type, RST_MOD_STAT_DISP_CFG);
    }

    /* 寻找当前数据包的起始数据索引(第0包数据) */
    if (GETBIT_N(info->request_type, CONT_MOD_STAT_DISP_CFG))
    {
        if (info->pkg_id == 0)
        {
            idx_rec = 0;
        }
    }

    /* 标记需要上传的模拟量 */
    if (!TYB_SLP_mark_upload_pv(mod_id, &upload_pv_num))
    {
        return;
    }

    /* 计算总包数、数据起始索引 */
    for (i = 0; i < TYBC_PV_MAX; i++)
    {
        if (GETBIT_N(fg_upload_pv, i))  /* 有效模拟量 */
        {
            upload_bytes += DP_FIELD_MOD_STAT_DISP_CFG_BYTES;

            if (upload_bytes > TRANS_07_CONTENT_BYTES)
            {
                package_total++;  // 总包数累加
                upload_bytes = DP_FIELD_MOD_STAT_DISP_CFG_BYTES;

                /* 寻找当前数据包的起始数据索引(非第0包数据) */
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

    /* 清空上报字节数计数 */
    upload_bytes = 0;

    /* 填充帧头 */
    buf[0] = info->trigger_src;      /* 触发源 */
    buf[1] = info->verif_code[0];    /* 验证码 */
    buf[2] = info->verif_code[1];    /* 验证码 */
    buf[3] = info->verif_code[2];    /* 验证码 */
    buf[4] = package_total;          /* 总包数 */
    buf[5] = info->pkg_id;           /* 当前包号 */
    buf[6] = info->trans_code;       /* 传送码 */

    /* 填充中间信息 */
    buf[7] = mod_id;                 /* 当前模块号 */
    buf[8] = upload_pv_num;          /* 上报状态数 */

    buf_len += TRANS_07_HEADER_BYTES;

    /* 填充内容: 当前模块状态的配置信息 */
    for (i = idx_rec; i < TYBC_PV_MAX; i++)
    {
        if (GETBIT_N(fg_upload_pv, i))
        {
            /* 模拟量值 */
            disp_cfg.value = TYBC_PV_VAL(mod_id,i);

            /* 压机模拟量 */
            if (i < TYBC_UNIT_PV_BEGIN)
            {
                /* 计算压机模拟量中的相对位置&压机号 */
                pv_id = i % TYBC_COMP_PV_MAX;
                comp_id = i / TYBC_COMP_PV_MAX;

                disp_cfg.msg_id = TYBC_COMP_PV_TITLE(pv_id);    /* 名称 */
                disp_cfg.id = MAKEWORD(mod_id, comp_id);       /* 模块号+压机号 */
                disp_cfg.attr = TYBC_COMP_PV_ATTR(pv_id);       /* 属性 */
            }
            else
            {
                /* 计算单元模拟量中的相对位置 */
                pv_id = i - TYBC_UNIT_PV_BEGIN;

                disp_cfg.msg_id = TYBC_UNIT_PV_TITLE(pv_id);    /* 名称 */
                disp_cfg.id = MAKEWORD(mod_id, 0xff);          /* 模块号+0xff(无压机号) */
                disp_cfg.attr = TYBC_UNIT_PV_ATTR(pv_id);       /* 属性 */
            }

            /* 空间计算: 新增后的累计字节数 */
            upload_bytes = buf_len + DP_FIELD_MOD_STAT_DISP_CFG_BYTES;

            /* 写入上报功能点缓存区 */
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

    /* 传输下一包 */
    info->pkg_id++;

    /* 所有参数组名配置信息上传完成 */
    if (!fg_multi_pkg)
    {
        idx_rec = 0;                                // 记录清空
        info->pkg_id = 0;                           // 当前包ID复位
        info->trans_code = TRANS_NULL;
        fg_has_trans_task = FALSE;
    }

    /* 重传数据包时，不继续传后续传送码，结束通道传输 */
    if (GETBIT_N(info->request_type, CONT_MOD_STAT_DISP_CFG))
    {
        fg_has_trans_task = FALSE;
        CLRBIT_N(info->request_type, CONT_MOD_STAT_DISP_CFG);
    }
}


/*******************************************************************************
 函 数: TYB_SLP_parse_mod_stat_cmd()
 功 能: 解析模块状态命令
 参 数: buf--接收缓冲区(数据源);
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TYB_SLP_parse_mod_stat_cmd(U08 *buf)
{
    TYB_TRANS_INFO info;
    U08 buf_len = 0;

    /* 初始化 */
    ARRCLR(info.addr);
    ARRCLR(info.request_type);

    /* 触发源不为"状态查询"，则不继续解析命令 */
    if (buf[buf_len] != TRANS_MOD_STAT)
    {
        return;
    }

    /* 第1部分:触发源("状态查询"界面) */
    info.trigger_src = buf[buf_len++];

    /* 第2部分:验证码 */
    MEMCPY(info.verif_code, &buf[buf_len], TRANS_VERIF_CODE_LEN);
    buf_len += TRANS_VERIF_CODE_LEN;

    /* 第3部分: 解析码 */
    info.parse_code = buf[buf_len++];

    /* 第4部分: 请求码 */
    info.request_code = buf[buf_len++];

    /* 第5部分: 请求包号 */
    info.pkg_id = buf[buf_len++];   // 从指定数据包开始，总包数内部计算

    /* 第6部分: 模块号 */
    info.addr[0] = buf[buf_len++];

    switch (info.parse_code)
    {
        /* 加载模块状态信息 */
        case TRANS_CMD_MOD_STATUS_DISP:
            switch (info.request_code)
            {
                /* 从头开始、全部发送 */
                case UPLOAD_DISP_ALL:
                    info.trans_code = TRANS_MOD_MSG_DISP_RSRC;
                    SETBIT_N(info.request_type, RST_MOD_STAT_DISP_ALL);
                    SETBIT_N(info.request_type, RST_MOD_STAT_DISP_RSRC);
                    SETBIT_N(info.request_type, RST_MOD_STAT_DISP_CFG);
					SETBIT_N(info.request_type, RST_MOD_MSG_DISP_RSRC);
                    break;

                /* 发送显示资源中的某包数据 */
                case UPLOAD_ONE_DISP_RSRC:
                    info.trans_code = TRANS_MOD_STAT_DISP_RSRC;
                    SETBIT_N(info.request_type, CONT_MOD_STAT_DISP_RSRC);
                    break;

                /* 发送配置信息中的某包数据 */
                case UPLOAD_ONE_DISP_CFG:
                    info.trans_code = TRANS_MOD_STAT_DISP_CFG;
                    SETBIT_N(info.request_type, CONT_MOD_STAT_DISP_CFG);
                    break;

                /* 发送显示资源的所有数据包(所有模块状态) */
                case UPLOAD_ALL_DISP_RSRC:
                    info.trans_code = TRANS_MOD_STAT_DISP_RSRC;
                    SETBIT_N(info.request_type, RST_MOD_STAT_DISP_RSRC);
                    break;

                /* 发送配置信息的所有数据包(当前模块) */
                case UPLOAD_ALL_DISP_CFG:
                    info.trans_code = TRANS_MOD_STAT_DISP_CFG;
                    SETBIT_N(info.request_type, RST_MOD_STAT_DISP_CFG);
                    break;
            }

            /* 推入队列 */
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

/* 有"用电状态" */
#ifdef TUYA_USE_ELE_STAT
/*****************************************************************************
函  数 : SH_sign_local2sh()
功  能 : 用电量属性处理
参  数 : U16 sign_local :
返  回 : 无
其  他 : 无
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

I16 week_cache[] =  {0 ,1 ,2 ,3 ,4 ,5 ,6};                  //原始周
I16 year_cache[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}; //原始年

I16 week_ele_val[][MAX_WEEK_VAL] = {
	{123, 234, 345 ,456 , 567, 456, 234,},/* 热水 */
	{234, 345, 456 ,567 , 456, 345, 567,},/* 制热 */
	{345, 456, 567 ,345 , 345, 567, 572,},/* 制冷 */
};

I16 year_ele_val[][MAX_YEAR_VAL] = {
	{4560, 2340, 3450 ,4560, 5670, 4560, 3450, 2340, 3450 ,4560 , 5670, 4560,},	/* 热水 */
	{5670, 4560, 3450 ,4560 , 5670, 4560, 3450, 2340, 3450 ,4560 , 5670, 4560,},/* 制热 */
	{3450, 2340, 3450 ,4560 , 5670, 4560, 3450, 2340, 3450 ,4560 , 5670, 4560,},/* 制冷 */
};

U16 TYB_ELE_STA_ATTR(U16 attr_chart)
{
	U16 sign_sh;
    U16 unit_sh = 0;         /* 单位 */
    U16 chart_sh = 0;        /* 所在统计表 */
    U16 decimal_sh = 0;      /* 小数位 */

    /* 单位 */
  	unit_sh = SH_UNIT_kWH;         /* kWH */
	/* 所在统计表 */
	chart_sh = attr_chart;
    /* 小数位 */
    decimal_sh = 0;

    /* 合成完整属性 */
    sign_sh = (unit_sh << 0)
            | (chart_sh << CHART_BGN_BIT)
            | (decimal_sh << ELE_DECIMAL_BGN_BIT);

    return sign_sh;
}

/*******************************************************************************
 函 数: TYB_SLP_trans_ele_stat_disp_rsrc()
 功 能: 分包上传当前用电状态显示资源(通道传输协议，传送码:0x08)
 参 数: buf--功能点缓存区;
        info--传输请求信息;
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TYB_SLP_trans_ele_stat_disp_rsrc(U08 *buf, TYB_TRANS_INFO *info)
{
    U16 i,j;
    U08 useless_cache[TYBC_STR_BYTE_MAX];        /* 无用缓存 */
    DP_FIELD_DISP_RSRC disp_rsrc; /* 模块状态显示资源 */
    U16 upload_bytes = 0;         /* 上传数据总字节数 */
    U08 package_total = 1;        /* 总包数(至少1包) */
    U08 upload_pv_num = MAX_MODE_ELE*(MAX_YEAR_VAL+MAX_WEEK_VAL);        /* 模拟量上报总数 */
    BOOL fg_multi_pkg = FALSE;    /* 多包传输需求标志 */
    U16 *src_buf = NULL;
    U08 buf_len = 0;
	U16 msg_index_cache;		  /* 存储字符串索引 */

    static U16 idx_rec = 0;       /* 记录下一包数据的起始数据索引 */

    /* 更新cache */
    {
        I08 idx = 0;
        LOG_RTC time_week = fun_time_localtime(pwru_week_cache.node[pwru_week_cache.queue.end].timestamp);
        LOG_RTC time_year = fun_time_localtime(pwru_year_cache.node[pwru_year_cache.queue.end].timestamp);
        /* end即当前，始终放在最右边显示，即放到week_cache数组的最右边 */
        for (idx=POWER_USAGE_WEEK_SIZE-1; idx>=0; idx--)
        {
            week_cache[idx] = (time_week.week+idx+1)%POWER_USAGE_WEEK_SIZE;
        }

        for (idx=POWER_USAGE_YEAR_SIZE-1; idx>=0; idx--)
        {
            year_cache[idx] = (time_year.month+idx)%POWER_USAGE_YEAR_SIZE;
        }
    }


    /* 数据包重发起始数据索引 */
    if (GETBIT_N(info->request_type, RST_ELE_STAT_DISP_RSRC))
    {
        idx_rec = 0;
        CLRBIT_N(info->request_type, RST_ELE_STAT_DISP_RSRC);
    }

    /* 寻找当前数据包的起始数据索引(第0包数据) */
    if (GETBIT_N(info->request_type, CONT_ELE_STAT_DISP_RSRC))
    {
        if (info->pkg_id == 0)
        {
            idx_rec = 0;
        }
    }

    /* 计算总包数、数据起始索引 */
    for (i = 0; i < MAX_MODE_ELE*(MAX_YEAR_VAL+MAX_WEEK_VAL); i++)
    {
        U08 msg_len = 0;

    	if (i >= 0 && i < MAX_MODE_ELE*MAX_WEEK_VAL)			//最近一周用电数据
    	{
			msg_len = TYB_SLP_GET_CODE_STR(useless_cache, msg_title_week[week_cache[i%MAX_WEEK_VAL]]);
    	}
		else if (i < MAX_MODE_ELE*(MAX_YEAR_VAL+MAX_WEEK_VAL)) 	//最近一年用电数据
		{
			U16 offset_year = MAX_MODE_ELE*MAX_WEEK_VAL;
			msg_len = TYB_SLP_GET_CODE_STR(useless_cache, msg_title_month[year_cache[(i-offset_year)%MAX_YEAR_VAL]]);
		}

        upload_bytes += DP_FIELD_DISP_RSRC_BYTES + msg_len;

        if (upload_bytes > TRANS_08_CONTENT_BYTES)
        {
            package_total++;    /* 总包数累加 */
            upload_bytes = DP_FIELD_DISP_RSRC_BYTES + msg_len;

            /* 寻找当前数据包的起始数据索引(非第0包数据) */
            if (GETBIT_N(info->request_type, CONT_ELE_STAT_DISP_RSRC))
            {
                /* 当数据更新至请求包号时，记录此时的起始索引 */
                if (info->pkg_id == (package_total-1))
                {
                    idx_rec = i;
                }
            }
        }
    }

    /* 清空上报字节数计数 */
    upload_bytes = 0;

    /* 填充帧头 */
    buf[0] = info->trigger_src;          /* 触发源 */
    buf[1] = info->verif_code[0];        /* 验证码 */
    buf[2] = info->verif_code[1];        /* 验证码 */
    buf[3] = info->verif_code[2];        /* 验证码 */
    buf[4] = package_total;              /* 总包数 */
    buf[5] = info->pkg_id;               /* 当前包号 */
    buf[6] = info->trans_code;           /* 传送码 */

    /* 填充中间信息 */
    buf[7] = upload_pv_num;              /* 上报状态数 */

    buf_len += TRANS_08_HEADER_BYTES;

    /* 填充内容: 当前模块状态的显示资源 */
    for (i = idx_rec; i < MAX_MODE_ELE*(MAX_YEAR_VAL+MAX_WEEK_VAL); i++)
    {

    	if (i >= 0 && i < MAX_MODE_ELE*MAX_WEEK_VAL)			//最近一周用电数据
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

       /* 空间计算: 新增后的累计字节数 */
       upload_bytes = buf_len + DP_FIELD_DISP_RSRC_BYTES + disp_rsrc.msg_len;

       /* 写入上报功能点缓存区 */
       src_buf = (U16*)&disp_rsrc;
       if (upload_bytes <= RAW_DATA_BYTES)
       {
           for (j = 0; j < DP_FIELD_DISP_RSRC_SIZE; j++)
           {
               buf[buf_len++] = HIBYTE(src_buf[j]);
               buf[buf_len++] = LOBYTE(src_buf[j]);
           }

           /* 写入字符串 */
		   buf_len += TYB_SLP_GET_CODE_STR(&buf[buf_len], msg_index_cache);

       }
       else
       {
           fg_multi_pkg = TRUE;
           idx_rec = i;
           break;
       }

    }

    /* 传输下一包 */
    info->pkg_id++;

    /* 所有模块状态显示资源上传完成 */
    if (!fg_multi_pkg)
    {
        idx_rec = 0;          /* 记录清空 */
        info->pkg_id = 0;     /* 当前包ID复位 */

        if (GETBIT_N(info->request_type, RST_ELE_STAT_DISP_ALL))    /* 请求所有时，接着发下一个传送码 */
        {
            info->trans_code = TRANS_ELE_STAT_DISP_CFG;
            CLRBIT_N(info->request_type, RST_ELE_STAT_DISP_ALL);
        }
        else    /* 重发最后一包数据或请求所有显示资源数据，结束传输 */
        {
            info->trans_code = TRANS_NULL;
            fg_has_trans_task = FALSE;
        }
    }

    /* 重传某包数据时，不继续传后续传送码，结束通道传输 */
    if (GETBIT_N(info->request_type, CONT_ELE_STAT_DISP_RSRC))
    {
        fg_has_trans_task = FALSE;
        CLRBIT_N(info->request_type, CONT_ELE_STAT_DISP_RSRC);
    }
}


/*******************************************************************************
 函 数: TYB_SLP_trans_ele_stat_disp_cfg()
 功 能: 分包上传当前模块状态配置信息 (通道传输协议，传送码:0x09)
 参 数: buf--功能点缓存区;
        info--传输请求信息;
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TYB_SLP_trans_ele_stat_disp_cfg(U08 *buf, TYB_TRANS_INFO *info)
{
    U16 i,j;
    DP_FIELD_ELE_STAT_DISP_CFG disp_cfg;    /* 模块状态配置信息 */
    U16 upload_bytes = 0;                   /* 上传数据总字节数 */
    U08 package_total = 1;                  /* 总包数(至少1包) */
	/* 模拟量上报总数 */
    U08 upload_pv_num = (MAX_WEEK_VAL+MAX_YEAR_VAL)*MAX_MODE_ELE;
    BOOL fg_multi_pkg = FALSE;              /* 多包传输需求标志 */
    U16 *src_buf = NULL;
    U08 buf_len = 0;

    static U16 idx_rec = 0;                 /* 记录下一包数据的起始数据索引 */

    /* 更新ele_val */
    {
        U08 end = 0;
        I08 idx;
        /* end即当前，从end往前（左）向node取数据 */
        for (idx=POWER_USAGE_WEEK_SIZE-1; idx>=0; idx--)
        {
            end = (pwru_week_cache.queue.end+idx+1)%POWER_USAGE_WEEK_SIZE;

            week_ele_val[0][idx] = pwru_week_cache.node[end].val_dhw; /* 热水 */
            week_ele_val[1][idx] = pwru_week_cache.node[end].val_heat;/* 制热 */
            week_ele_val[2][idx] = pwru_week_cache.node[end].val_cool;/* 制冷 */
        }

        for (idx=POWER_USAGE_YEAR_SIZE-1; idx>=0; idx--)
        {
            end = (pwru_year_cache.queue.end+idx+1)%POWER_USAGE_YEAR_SIZE;

            year_ele_val[0][idx] = pwru_year_cache.node[end].val_dhw; /* 热水 */
            year_ele_val[1][idx] = pwru_year_cache.node[end].val_heat;/* 制热 */
            year_ele_val[2][idx] = pwru_year_cache.node[end].val_cool;/* 制冷 */
        }
    }


    /* 数据包重发起始数据索引 */
    if (GETBIT_N(info->request_type, RST_ELE_STAT_DISP_CFG))
    {
        idx_rec = 0;
        CLRBIT_N(info->request_type, RST_ELE_STAT_DISP_CFG);
    }

    /* 寻找当前数据包的起始数据索引(第0包数据) */
    if (GETBIT_N(info->request_type, CONT_ELE_STAT_DISP_CFG))
    {
        if (info->pkg_id == 0)
        {
            idx_rec = 0;
        }
    }

    /* 计算总包数、数据起始索引 */
    for (i = 0; i < MAX_MODE_ELE*(MAX_YEAR_VAL+MAX_WEEK_VAL); i++)
    {
        upload_bytes += DP_FIELD_ELE_STAT_DISP_CFG_BYTES;

        if (upload_bytes > TRANS_09_CONTENT_BYTES)
        {
            package_total++;  // 总包数累加
            upload_bytes = DP_FIELD_ELE_STAT_DISP_CFG_BYTES;

            /* 寻找当前数据包的起始数据索引(非第0包数据) */
            if (GETBIT_N(info->request_type, CONT_ELE_STAT_DISP_CFG))
            {
                if (info->pkg_id == (package_total-1))
                {
                    idx_rec = i;
                }
            }
        }
    }

    /* 清空上报字节数计数 */
    upload_bytes = 0;

    /* 填充帧头 */
    buf[0] = info->trigger_src;      /* 触发源 */
    buf[1] = info->verif_code[0];    /* 验证码 */
    buf[2] = info->verif_code[1];    /* 验证码 */
    buf[3] = info->verif_code[2];    /* 验证码 */
    buf[4] = package_total;          /* 总包数 */
    buf[5] = info->pkg_id;           /* 当前包号 */
    buf[6] = info->trans_code;       /* 传送码 */

    /* 填充中间信息 */
    buf[7] = upload_pv_num;          /* 上报状态数 */

    buf_len += TRANS_09_HEADER_BYTES;

    /* 填充内容: 当前模块状态的配置信息 */
    for (i = idx_rec; i < (MAX_WEEK_VAL+MAX_YEAR_VAL)*MAX_MODE_ELE; i++)
    {
    	U16 attr_chart;
    	if (i >= 0 && i < MAX_MODE_ELE*MAX_WEEK_VAL)			//最近一周用电数据
    	{
			/*  用电数值 & 字符串id & 模式id */
			if (i < MODE_HEAT_ELE*MAX_WEEK_VAL)
			{
				disp_cfg.value = week_ele_val[MODE_HOT_ELE][i];										//热水周用电
				disp_cfg.msg_id = 1 + i;
				disp_cfg.mode_id = MODE_HOT_ELE;
			}
			else if (i < MODE_COOL_ELE*MAX_WEEK_VAL)
			{
				disp_cfg.value = week_ele_val[MODE_HEAT_ELE][i - MODE_HEAT_ELE*MAX_WEEK_VAL];	 		//制热周用电
				disp_cfg.msg_id = 1 + i - MODE_HEAT_ELE*MAX_WEEK_VAL;
				disp_cfg.mode_id = MODE_HEAT_ELE;
			}
			else if (i < MAX_MODE_ELE*MAX_WEEK_VAL)
			{
				disp_cfg.value = week_ele_val[MODE_COOL_ELE][i - MODE_COOL_ELE*MAX_WEEK_VAL];		 	//制冷周用电
				disp_cfg.msg_id = 1 + i - MODE_COOL_ELE*MAX_WEEK_VAL;
				disp_cfg.mode_id = MODE_COOL_ELE;
			}
			/* 属性 */
			attr_chart = 0;
			disp_cfg.attr =  TYB_ELE_STA_ATTR(attr_chart);
		}
		else if (i < (MAX_WEEK_VAL+MAX_YEAR_VAL)*MAX_MODE_ELE)		//最近一年用电数据
		{
			U16 offest_year = MAX_MODE_ELE*MAX_WEEK_VAL;
			/*  用电数值 & 字符串id & 模式id */
			if (i < offest_year + MODE_HEAT_ELE*MAX_YEAR_VAL)
			{
				disp_cfg.value = year_ele_val[MODE_HOT_ELE][i - offest_year];									//热水年用电
				disp_cfg.msg_id = 1 + MAX_WEEK_VAL + i - offest_year;
				disp_cfg.mode_id = MODE_HOT_ELE;
			}
			else if (i < offest_year + MODE_COOL_ELE*MAX_YEAR_VAL)
			{
				disp_cfg.value = year_ele_val[MODE_HEAT_ELE][i - offest_year - MODE_HEAT_ELE*MAX_YEAR_VAL];   //制热年用电
				disp_cfg.msg_id = 1 + MAX_WEEK_VAL + i - offest_year - MODE_HEAT_ELE*MAX_YEAR_VAL;
				disp_cfg.mode_id = MODE_HEAT_ELE;
			}
			else if (i < offest_year + MAX_MODE_ELE*MAX_YEAR_VAL)
			{
				disp_cfg.value = year_ele_val[MODE_COOL_ELE][i - offest_year - MODE_COOL_ELE*MAX_YEAR_VAL];	//制冷年用电
				disp_cfg.msg_id = 1 + MAX_WEEK_VAL + i - offest_year - MODE_COOL_ELE*MAX_YEAR_VAL;
				disp_cfg.mode_id = MODE_COOL_ELE;
			}
			/* 属性 */
			attr_chart = 1;
			disp_cfg.attr =  TYB_ELE_STA_ATTR(attr_chart);
		}



        /* 空间计算: 新增后的累计字节数 */
        upload_bytes = buf_len + DP_FIELD_ELE_STAT_DISP_CFG_BYTES;

        /* 写入上报功能点缓存区 */
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

    /* 传输下一包 */
    info->pkg_id++;

    /* 所有配置信息上传完成 */
    if (!fg_multi_pkg)
    {
        idx_rec = 0;                                // 记录清空
        info->pkg_id = 0;                           // 当前包ID复位
        info->trans_code = TRANS_NULL;
        fg_has_trans_task = FALSE;
    }

    /* 重传数据包时，不继续传后续传送码，结束通道传输 */
    if (GETBIT_N(info->request_type, CONT_ELE_STAT_DISP_CFG))
    {
        fg_has_trans_task = FALSE;
        CLRBIT_N(info->request_type, CONT_ELE_STAT_DISP_CFG);
    }
}


/*******************************************************************************
 函 数: TYB_SLP_parse_ele_stat_cmd()
 功 能: 解析用电状态命令
 参 数: buf--接收缓冲区(数据源);
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TYB_SLP_parse_ele_stat_cmd(U08 *buf)
{
    TYB_TRANS_INFO info;
    U08 buf_len = 0;

    /* 初始化 */
    ARRCLR(info.addr);
    ARRCLR(info.request_type);

    /* 触发源不为"用电查询"，则不继续解析命令 */
    if (buf[buf_len] != TRANS_ELE_STAT)
    {
        return;
    }

    /* 第1部分:触发源("用电查询"界面) */
    info.trigger_src = buf[buf_len++];

    /* 第2部分:验证码 */
    MEMCPY(info.verif_code, &buf[buf_len], TRANS_VERIF_CODE_LEN);
    buf_len += TRANS_VERIF_CODE_LEN;

    /* 第3部分: 解析码 */
    info.parse_code = buf[buf_len++];

    /* 第4部分: 请求码 */
    info.request_code = buf[buf_len++];

    /* 第5部分: 请求包号 */
    info.pkg_id = buf[buf_len++];   // 从指定数据包开始，总包数内部计算


    switch (info.parse_code)
    {
        /* 加载模块状态信息 */
        case TRANS_CMD_ELE_STATUS_DISP:
            switch (info.request_code)
            {
                /* 从头开始、全部发送 */
                case UPLOAD_DISP_ALL:
                    info.trans_code = TRANS_ELE_STAT_DISP_RSRC;
                    SETBIT_N(info.request_type, RST_ELE_STAT_DISP_ALL);
                    SETBIT_N(info.request_type, RST_ELE_STAT_DISP_RSRC);
                    SETBIT_N(info.request_type, RST_ELE_STAT_DISP_CFG);
                    break;

                /* 发送显示资源中的某包数据 */
                case UPLOAD_ONE_DISP_RSRC:
                    info.trans_code = TRANS_ELE_STAT_DISP_RSRC;
                    SETBIT_N(info.request_type, CONT_ELE_STAT_DISP_RSRC);
                    break;

                /* 发送配置信息中的某包数据 */
                case UPLOAD_ONE_DISP_CFG:
                    info.trans_code = TRANS_ELE_STAT_DISP_CFG;
                    SETBIT_N(info.request_type, CONT_ELE_STAT_DISP_CFG);
                    break;

                /* 发送显示资源的所有数据包(所有用电状态) */
                case UPLOAD_ALL_DISP_RSRC:
                    info.trans_code = TRANS_ELE_STAT_DISP_RSRC;
                    SETBIT_N(info.request_type, RST_ELE_STAT_DISP_RSRC);
                    break;

                /* 发送配置信息的所有数据包 */
                case UPLOAD_ALL_DISP_CFG:
                    info.trans_code = TRANS_ELE_STAT_DISP_CFG;
                    SETBIT_N(info.request_type, RST_ELE_STAT_DISP_CFG);
                    break;
            }

            /* 推入队列 */
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
 函 数: TYB_SLP_trans_err_his_disp_rsrc()
 功 能: 分包上传历史故障显示资源(通道传输协议，传送码:0x0A)
 参 数: buf--功能点缓存区;
        info--传输请求信息;
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TYB_SLP_trans_err_his_disp_rsrc(U08 *buf, TYB_TRANS_INFO *info)
{
    U16 i;
    U08 useless_cache[200];	/* 无用缓存 */
	TYBC_ERR_HIS err_record;
    U16 upload_bytes = 0;          /* 上传数据总字节数 */
    U08 package_total = 1;         /* 总包数(至少1包) */
    BOOL fg_multi_pkg = FALSE;     /* 多包传输需求标志 */
    U08 buf_len = 0;

    static U16 idx_rec = 0;        /* 记录下一包数据的起始数据索引 */

	MEMCLR(&err_record, ((sizeof(TYBC_ERR_HIS)+1)/sizeof(U16))*2);

    /* 数据包重发起始数据索引 */
    if (GETBIT_N(info->request_type, RST_ERR_HIS_DISP_RSRC))
    {
        idx_rec = 0;
        CLRBIT_N(info->request_type, RST_ERR_HIS_DISP_RSRC);
    }

    /* 寻找当前数据包的起始数据索引(第0包数据) */
    if (GETBIT_N(info->request_type, CONT_ERR_HIS_DISP_RSRC))
    {
        if (info->pkg_id == 0)
        {
            idx_rec = 0;
        }
    }

    /* 计算总包数、数据起始索引 */
    for (i = 0; i < TY_HIS_ERR_MAX; i++)
    {
   		U08 msg_len = 0;
        /* 获取历史故障 */
        TYBC_GET_ERR_HIS(&err_record,i);
        /* 有故障 */
        if (err_record.err_id < ERR_TOTAL)
        {
			msg_len = TYB_SLP_GET_CODE_STR(useless_cache, err_record.name);
	        upload_bytes += DP_FIELD_HIS_ERR_INFO_BYTES + msg_len;

	        if (upload_bytes > TRANS_10_CONTENT_BYTES)
	        {
	            package_total++;  /* 总包数累加 */
	            upload_bytes = DP_FIELD_HIS_ERR_INFO_BYTES + msg_len;

	            /* 寻找当前数据包的起始数据索引(非第0包数据) */
	            if (GETBIT_N(info->request_type, CONT_ERR_HIS_DISP_RSRC))
	            {
	                /* 当数据更新至请求包号时，记录此时的起始索引 */
	                if (info->pkg_id == (package_total-1))
	                {
	                    idx_rec = i;
	                }
	            }
	        }
		}
    }

    /* 清空上报字节数计数 */
    upload_bytes = 0;

    /* 填充帧头 */
    buf[0] = info->trigger_src;          /* 触发源 */
    buf[1] = info->verif_code[0];        /* 验证码 */
    buf[2] = info->verif_code[1];        /* 验证码 */
    buf[3] = info->verif_code[2];        /* 验证码 */
    buf[4] = package_total;              /* 总包数 */
    buf[5] = info->pkg_id;               /* 当前包号 */
    buf[6] = info->trans_code;           /* 传送码 */

    /* 填充中间信息: 实际上传历史故障总数 */
    buf[7] = TY_HIS_ERR_MAX;

    buf_len += TRANS_10_HEADER_BYTES;

    /* 填充内容:历史故障显示资源(全集) */
    for (i = idx_rec; i < TY_HIS_ERR_MAX; i++)
    {
        /* 获取历史故障 */
        TYBC_GET_ERR_HIS(&err_record,i);

        /* 有故障 */
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

    /* 传输下一包 */
    info->pkg_id++;

    /* 所有参数组名显示资源上传完成 */
    if (!fg_multi_pkg)
    {
        idx_rec = 0;                                           /* 记录清空 */
        info->pkg_id = 0;                                      /* 当前包ID复位 */
        info->trans_code = TRANS_NULL;
        fg_has_trans_task = FALSE;
    }

    /* 重传数据包时，不继续传后续传送码，结束通道传输 */
    if (GETBIT_N(info->request_type, CONT_ERR_HIS_DISP_RSRC))
    {
        fg_has_trans_task = FALSE;
        CLRBIT_N(info->request_type, CONT_ERR_HIS_DISP_RSRC);
    }
}


/*******************************************************************************
 函 数: TYB_SLP_parse_err_his_cmd()
 功 能: 解析历史故障命令
 参 数: buf--接收缓冲区(数据源);
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TYB_SLP_parse_err_his_cmd(U08 *buf)
{
    TYB_TRANS_INFO info;
    U08 buf_len = 0;

    /* 初始化 */
    ARRCLR(info.addr);
    ARRCLR(info.request_type);

    /* 触发源不为"历史故障"，则不继续解析命令 */
    if (buf[buf_len] != TRANS_HIS_ERR)
    {
        return;
    }

    /* 第1部分:触发源("历史故障"界面) */
    info.trigger_src = buf[buf_len++];

    /* 第2部分:验证码 */
    MEMCPY(info.verif_code, &buf[buf_len], TRANS_VERIF_CODE_LEN);
    buf_len += TRANS_VERIF_CODE_LEN;

    /* 第3部分: 解析码 */
    info.parse_code = buf[buf_len++];

    /* 第4部分: 请求码 */
    info.request_code = buf[buf_len++];

    /* 第5部分: 请求包号 */
    info.pkg_id = buf[buf_len++];   // 从指定数据包开始，总包数内部计算


    switch (info.parse_code)
    {
        /* 加载模块状态信息 */
        case TRANS_CMD_HIS_ERR_DISP:
            switch (info.request_code)
            {
                /* 发送显示资源的所有数据包(所有历史故障) */
                case UPLOAD_DISP_ALL:
                    info.trans_code = TRANS_ERR_HIS_DISP_RSRC;
                    SETBIT_N(info.request_type, RST_ERR_HIS_DISP_RSRC);
                    break;

                /* 发送显示资源中的某包数据 */
                case UPLOAD_ONE_DISP_RSRC:
                    info.trans_code = TRANS_ERR_HIS_DISP_RSRC;
                    SETBIT_N(info.request_type, CONT_ERR_HIS_DISP_RSRC);
                    break;
            }

            /* 推入队列 */
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
 函 数: TYB_SLP_concat_trans_data()
 功 能: 通道传输数据拼接
 参 数: buf--字符串缓存区;
        info--传输请求信息;
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TYB_SLP_concat_trans_data(U08 *buf, TYB_TRANS_INFO* info)
{
    switch (info->trans_code)
    {
        case TRANS_NULL:
            break;

/* 有"参数设置" */
#ifdef TUYA_USE_PARAM_SET
        /* 传送参数组显示资源 */
        case TRANS_PARAM_GRP_DISP_RSRC:
            TYB_SLP_trans_param_grp_disp_rsrc(&buf[0],info);
            break;

        /* 传送参数组配置信息 */
        case TRANS_PARAM_GRP_DISP_CFG:
            TYB_SLP_trans_param_grp_disp_cfg(&buf[0],info);
            break;

        /* 传送参数选项显示资源 */
        case TRANS_PARAM_RANGE_DISP_RSRC:
            TYB_SLP_trans_param_range_disp_rsrc(&buf[0],info);
            break;

        /* 传送当前组参数显示资源 */
        case TRANS_PARAM_ITEM_DISP_RSRC:
            TYB_SLP_trans_param_item_disp_rsrc(&buf[0],info);
            break;

        /* 传送当前组参数配置信息 */
        case TRANS_PARAM_ITEM_DISP_CFG:
            TYB_SLP_trans_param_item_disp_cfg(&buf[0],info);
            break;
#endif

/* 有"系统状态" */
#ifdef TUYA_USE_SYS_STAT

        /* 传送系统状态字符显示资源 */
        case TRANS_SYS_MSG_DISP_RSRC:
            TYB_SLP_trans_sys_msg_disp_rsrc(&buf[0],info);
            break;

        /* 传送系统状态显示资源 */
        case TRANS_SYS_STAT_DISP_RSRC:
            TYB_SLP_trans_sys_stat_disp_rsrc(&buf[0],info);
            break;

        /* 传送系统状态配置信息 */
        case TRANS_SYS_STAT_DISP_CFG:
            TYB_SLP_trans_sys_stat_disp_cfg(&buf[0],info);
            break;

#endif

/* 有"模块状态" */
#ifdef TUYA_USE_MOD_STAT
		case TRANS_MOD_MSG_DISP_RSRC:
			TYB_SLP_trans_mod_msg_disp_rsrc(&buf[0],info);
			break;

        /* 传送模块状态显示资源 */
        case TRANS_MOD_STAT_DISP_RSRC:
            TYB_SLP_trans_mod_stat_disp_rsrc(&buf[0],info);
            break;

        /* 传送模块状态配置信息 */
        case TRANS_MOD_STAT_DISP_CFG:
            TYB_SLP_trans_mod_stat_disp_cfg(&buf[0],info);
            break;
#endif

/* 有"用电状态" */
#ifdef TUYA_USE_ELE_STAT
		case TRANS_ELE_STAT_DISP_RSRC:
			TYB_SLP_trans_ele_stat_disp_rsrc(&buf[0],info);
			break;

		case TRANS_ELE_STAT_DISP_CFG:
			TYB_SLP_trans_ele_stat_disp_cfg(&buf[0],info);
			break;
#endif

/* 有"历史故障" */
#ifdef TUYA_USE_ERR_HIS
		/* 传送历史故障显示资源 */
		case TRANS_ERR_HIS_DISP_RSRC:
		TYB_SLP_trans_err_his_disp_rsrc(&buf[0],info);
		break;
#endif
    }
}
#endif

/*******************************************************************************
 函 数: TYB_SLP_fill_upload_data()
 功 能:
 参 数: tuya_data--;
        dpid_idx--;
 返 回:
 其 它: 空
*******************************************************************************/
static void TYB_SLP_fill_upload_data(TUYA_DATA* tuya_data, U08 dpid_idx)
{
    U08 byte_idx = 0;

    tuya_data->dpid = DP[dpid_idx].dpid;
    tuya_data->dp_type = DP[dpid_idx].dp_type;
    switch (DP[dpid_idx].dpid)
    {
        /* DPID_1 开关 */
        case DPID_SWITCH:
            tuya_data->data_buf[byte_idx++] = TYBC_SWITCH_VAL;
            break;

        /* DPID_2 温度设置 */
        case DPID_TEMP_SET:
            tuya_data->data_buf[byte_idx++] = 0;
            tuya_data->data_buf[byte_idx++] = 0;
            tuya_data->data_buf[byte_idx++] = HIBYTE(TYBC_TEMP_SET_VAL);
            tuya_data->data_buf[byte_idx++] = LOBYTE(TYBC_TEMP_SET_VAL);
            break;

        /* DPID_3 当前温度 */
        case DPID_TEMP_CUR:
            tuya_data->data_buf[byte_idx++] = 0;
            tuya_data->data_buf[byte_idx++] = 0;
            tuya_data->data_buf[byte_idx++] = HIBYTE(TYBC_TEMP_CUR_VAL);
            tuya_data->data_buf[byte_idx++] = LOBYTE(TYBC_TEMP_CUR_VAL);
            break;

        /* DPID_4 设定模式 */
        case DPID_MODE:
            tuya_data->data_buf[byte_idx++] = LOBYTE(TYBC_MODE_SET_VAL);
            break;

        /* DPID_101 版本信息 */
        case DPID_VER_INFO:
            TYB_SLP_concat_ver_info(&tuya_data->data_buf[byte_idx]);
            break;

        /* DPID_102 系统信息 */
        case DPID_SYS_INFO:
            TYB_SLP_concat_sys_info(&tuya_data->data_buf[byte_idx]);
            break;

        /* DPID_103 协议版本 */
        case DPID_PROTOCOL_VER:
            TYB_SLP_concat_protocol_ver(&tuya_data->data_buf[byte_idx]);
            break;

/* 有"通用设置功能" */
#ifdef TUYA_HAS_GEN_SET
        /* DPID_104 通用设置信息 */
        case DPID_GENERAL_SET_INFO:
            TYB_SLP_concat_general_set_info(&tuya_data->data_buf[byte_idx]);
            break;
#endif

        /* DPID_106 区域一功能信息 */
        case DPID_ZONEA_FUNC_INFO:
            TYB_SLP_concat_zone1_func_info(&tuya_data->data_buf[byte_idx]);
            break;
        /* DPID_108 区域二功能信息 */
        case DPID_ZONEB_FUNC_INFO:
            TYB_SLP_concat_zone2_func_info(&tuya_data->data_buf[byte_idx]);
            break;
        /* DPID_110 热水功能信息 */
        case DPID_DHW_FUNC_INFO:
            TYB_SLP_concat_DHW_func_info(&tuya_data->data_buf[byte_idx]);
            break;
#if 0
/* 有"扩展功能" */
#ifdef TUYA_HAS_EXT_FUNC
        /* DPID_108 扩展功能信息 */
        case DPID_EXTEND_FUNC_INFO:
            TYB_SLP_concat_extend_func_info(&tuya_data->data_buf[byte_idx]);
            break;
#endif
#endif

/* 有"系统状态" */
#if 0
        /* DPID_112~115 系统状态信息1~4 */
        case DPID_SYS_STAT_INFO_1:
        case DPID_SYS_STAT_INFO_2:
        case DPID_SYS_STAT_INFO_3:
        case DPID_SYS_STAT_INFO_4:
            TYB_SLP_concat_sys_stat_info(&tuya_data->data_buf[byte_idx], dpid_idx);
            break;

        /* DPID_116 系统状态数据 */
        case DPID_SYS_STAT_DATA:
            TYB_SLP_concat_sys_stat_data(&tuya_data->data_buf[byte_idx]);
            break;
#endif

/* 有"当前故障" */
#ifdef TUYA_USE_ERR_CUR
        /* DPID_133 当前故障信息 */
        case DPID_ERR_CUR_INFO:
            TYB_SLP_concat_err_cur_info(&tuya_data->data_buf[byte_idx]);
            break;
#endif

#if 0
/* 有"历史故障" */
#ifdef TUYA_USE_ERR_HIS
        /* DPID_134 历史故障信息 */
        case DPID_ERR_HIS_INFO:
            TYB_SLP_concat_err_his_info(&tuya_data->data_buf[byte_idx]);
            break;
#endif
#endif

/* 有通道传输(有"参数设置"/"模块状态") */
#ifdef TUYA_HAS_TRANS
        /* DPID_118~123 通道传输 */
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
 函 数: TYB_SLP_SH_upload_tuya_data()
 功 能: 选择上报功能点，并将相应的上报内容填充至目标缓冲区
 参 数: dest_buf--目标缓存区(上报云端的数据);
        data_addr--Modubs数据地址;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL TYB_SLP_SH_upload_tuya_data(U08 *dest_buf, U16 data_addr)
{
    U08 dpid_idx = 0;
    TUYA_DATA tuya_data;
    U08 *src_buf = NULL;
    U08 idx = 0;
    U08 i;

    VALCLR(tuya_data);

    /* 输入检查 */
    if ((data_addr - ADDR_SH_TUYA_DATA_BGN) % SH_TUYA_DATA_SIZE != 0)
    {
        return FALSE;
    }

    /* 由后板决定上传哪个功能点 */
    dpid_idx = TYB_SLP_select_upload_dpid_idx();

    /* 填充功能点内容 */
    TYB_fill_dp(&tuya_data, dpid_idx);

    /* 赋值 */
    src_buf = (U08*)&tuya_data; //注意:大小端问题,统一用U08类型发送可规避
    for (i = 0; i < TUYA_DATA_SIZE*2; i++)
    {
        dest_buf[idx++] = src_buf[i];
    }

    return TRUE;
}
#endif

//***显示显示屏版本号时***//
#if (TYBC_DISP_HMI_VER_EN == TRUE)
/*******************************************************************************
 函 数: TYB_SLP_store_hmi_ver()
 功 能: 存储控制器、显示屏版本
 参 数: buf--接收缓冲区(数据源);
 返 回: 已读取字节数
 其 它: 空
*******************************************************************************/
static void TYB_SLP_store_hmi_ver(U08 *buf)
{
    U08 i;

    ARRCLR(tyb_hmi_ver);

    /* 存储显示屏版本 */
    for (i=0; i<MAX_VER_BYTE; i++)
    {
        tyb_hmi_ver[i] = buf[i];
    }
}
#endif


/*******************************************************************************
 函 数: TYB_SLP_parse_switch()
 功 能: 开关机解析
 参 数: buf--接收缓冲区(数据源);
        func_type--功能类型;
 返 回: 已读取字节数
 其 它: 空
*******************************************************************************/
static U08 TYB_SLP_parse_switch(U08 *buf, U08 func_type, U08 dpid)
{
    I16 val = 0;

    /* 内容解析 */
    if (func_type == FUNC_STANDARD)     /* 标准功能 */
    {
        val = buf[0];
    }
    else if (func_type == FUNC_CUSTOM)  /* 自定义功能 */
    {
        val = MAKEWORD(buf[0], buf[1]);
    }

    /* 命令触发 */
    if (val == RN_SWITCH_ON)
    {
    	switch (dpid)
    	{
    		case DPID_SWITCH:
				if(!fg_zone_A_run)	TYBC_CMD_RUN; 			 /* 区域一开机 */
				TYB_SLP_check_data_updated(TYBC_SWITCH_VAL, SWITCH_IDX);    /* 新值更新旧值 */
				break;
			case DPID_ZONEA_FUNC_CMD:
				if(!fg_zone_A_run)	TYBC_CMD_RUN;    	     /* 区域一开机 */
				TYB_SLP_check_data_updated(TYBC_SWITCH_VAL, ZONEA_SWITCH_IDX);    /* 新值更新旧值 */
				break;
			case DPID_ZONEB_FUNC_CMD:
				if(!fg_zone_B_run)	TYBC_ZoneB_CMD_RUN;      /* 区域二开机 */
				TYB_SLP_check_data_updated(TYBC_ZoneB_SWITCH_VAL, ZONEB_SWITCH_IDX);    /* 新值更新旧值 */
				break;
			case DPID_DHW_FUNC_CMD:
				if(!fg_hotw_run)	TYBC_DHW_CMD_RUN;       /* 热水开机 */
				TYB_SLP_check_data_updated(TYBC_DHW_SWITCH_VAL, DHW_SWITCH_IDX);    /* 新值更新旧值 */
				break;
		}
    }
    else
    {
        switch (dpid)
    	{
    		case DPID_SWITCH:
				if(fg_zone_A_run)	TYBC_CMD_STOP; 			 /* 区域一关机 */
				TYB_SLP_check_data_updated(TYBC_SWITCH_VAL, SWITCH_IDX);    /* 新值更新旧值 */
				break;
			case DPID_ZONEA_FUNC_CMD:
				if(fg_zone_A_run)	TYBC_CMD_STOP;    	     /* 区域一关机 */
				TYB_SLP_check_data_updated(TYBC_SWITCH_VAL, ZONEA_SWITCH_IDX);    /* 新值更新旧值 */
				break;
			case DPID_ZONEB_FUNC_CMD:
				if(fg_zone_B_run)	TYBC_ZoneB_CMD_STOP;     /* 区域二关机 */
				TYB_SLP_check_data_updated(TYBC_ZoneB_SWITCH_VAL, ZONEB_SWITCH_IDX);    /* 新值更新旧值 */
				break;
			case DPID_DHW_FUNC_CMD:
				if(fg_hotw_run)	TYBC_DHW_CMD_STOP;      /* 热水关机 */
				TYB_SLP_check_data_updated(TYBC_DHW_SWITCH_VAL, DHW_SWITCH_IDX);    /* 新值更新旧值 */
				break;
        }
    }

    return DP_CMD_FIELD_LEN;
}


/*******************************************************************************
 函 数: TYB_SLP_parse_temp_set()
 功 能: 设定温度解析
 参 数: buf--接收缓冲区(数据源);
        func_type--功能类型;
 返 回: 已读取字节数
 其 它: 空
*******************************************************************************/
static U08 TYB_SLP_parse_temp_set(U08 *buf, U08 func_type, U08 dpid)
{
    I32 raw_val = 0;
    I16 val = 0;

    /* 内容解析 */
    if (func_type == FUNC_STANDARD)     /* 标准功能 */
    {
        /* 温度设定值 */
        raw_val = MAKEDWORD(buf[0], buf[1], buf[2], buf[3]);
        val = (I16)raw_val;
    }
    else if (func_type == FUNC_CUSTOM)  /* 自定义功能 */
    {
        /* 解析温度设定值，将字符串转换为数值 */
        val = MAKEWORD(buf[0], buf[1]);
    }

    TYBC_temp_set(val, dpid);

    /* 新值更新旧值 */
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
 函 数: TYB_SLP_parse_mode_set()
 功 能: 设定模式解析
 参 数: buf--接收缓冲区(数据源);
        func_type--功能类型;
 返 回: 已读取字节数
 其 它: 空
*******************************************************************************/
static U08 TYB_SLP_parse_mode_set(U08 *buf, U08 func_type)
{
    I16 val = 0;

    /* 内容解析 */
    if (func_type == FUNC_STANDARD)     /* 标准功能 */
    {
        val = buf[0];
    }
    else if (func_type == FUNC_CUSTOM)  /* 自定义功能 */
    {
        val = MAKEWORD(buf[0], buf[1]);
    }

    TYBC_mode_set(val);

    TYB_SLP_check_data_updated(TYBC_MODE_SET_VAL, ZONEA_MODE_SET_IDX);    /* 新值更新旧值 */

    return DP_CMD_FIELD_LEN;
}

/* 有"扩展功能" */
#ifdef TUYA_HAS_EXT_FUNC
/* 有"风速功能" */
#ifdef TUYA_USE_WIND
/*******************************************************************************
 函 数: TYB_SLP_parse_wind_set()
 功 能: 设定风速解析
 参 数: buf--接收缓冲区(数据源);
        func_type--功能类型;
 返 回: 已读取字节数
 其 它: 空
*******************************************************************************/
static U08 TYB_SLP_parse_wind_set(U08 *buf, U08 func_type)
{
    I16 val = 0;

    /* 内容解析 */
    if (func_type == FUNC_STANDARD)     /* 标准功能 */
    {
        val = buf[0];
    }
    else if (func_type == FUNC_CUSTOM)  /* 自定义功能 */
    {
        val = MAKEWORD(buf[0], buf[1]);
    }

    TYBC_wind_set(val);

    TYB_SLP_check_data_updated(TYBC_WIND_SET_VAL, WIND_SET_IDX);    /* 新值更新旧值 */

    return DP_CMD_FIELD_LEN;
}
#endif

/* 有"湿度功能" */
#ifdef TUYA_USE_HUMI
/*******************************************************************************
 函 数: TYB_SLP_parse_humi_set()
 功 能: 设定湿度解析
 参 数: buf--接收缓冲区(数据源);
        func_type--功能类型;
 返 回: 已读取字节数
 其 它: 空
*******************************************************************************/
static U08 TYB_SLP_parse_humi_set(U08 *buf, U08 func_type)
{
    I16 val = 0;

    /* 内容解析 */
    if (func_type == FUNC_STANDARD)     /* 标准功能 */
    {
        val = buf[0];
    }
    else if (func_type == FUNC_CUSTOM)  /* 自定义功能 */
    {
        val = MAKEWORD(buf[0], buf[1]);
    }

    TYBC_humi_set(val);

    TYB_SLP_check_data_updated(TYBC_HUMI_SET_VAL, HUMI_SET_IDX);    /* 新值更新旧值 */

    return DP_CMD_FIELD_LEN;
}
#endif

/* 有"热水功能" */
#ifdef TUYA_USE_HOT_WATER
/*******************************************************************************
 函 数: TYB_SLP_parse_hot_water_set()
 功 能: 热水设定温度解析
 参 数: buf--接收缓冲区(数据源);
        func_type--功能类型;
 返 回: 已读取字节数
 其 它: 空
*******************************************************************************/
static U08 TYB_SLP_parse_hot_water_set(U08 *buf, U08 func_type)
{
    I16 val = 0;

    /* 内容解析 */
    if (func_type == FUNC_STANDARD)     /* 标准功能 */
    {
        val = buf[0];
    }
    else if (func_type == FUNC_CUSTOM)  /* 自定义功能 */
    {
        val = MAKEWORD(buf[0], buf[1]);
    }

    TYBC_hot_water_set(val);

    TYB_SLP_check_data_updated(TYBC_HOT_WATER_SET_VAL, HOT_WATER_SET_IDX);    /* 新值更新旧值 */

    return DP_CMD_FIELD_LEN;
}
#endif
#endif

/* 有"通用设置功能" */
#ifdef TUYA_HAS_GEN_SET
/*******************************************************************************
 函 数: TYB_SLP_parse_general_set_cmd()
 功 能: 解析 "通用设置命令" 透传型功能点
 参 数: buf--接收缓冲区(数据源);
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TYB_SLP_parse_general_set_cmd(U08 *buf)
{
    U08 buf_len = 0;

/* 有"语言功能" */
#ifdef TUYA_USE_LANG_SET
    /* 字段0: 语言 */
	if (sh_language != MAKEWORD(buf[buf_len], buf[buf_len+1]))
	{
		fg_tuya_lang_switch = TRUE;
	    switch (MAKEWORD(buf[buf_len], buf[buf_len+1]))
	    {
	        default:
	            break;

	        case RN_LANGUAGE_CN:    /* 中文显示 */
	        case RN_LANGUAGE_EN:    /* 英文显示 */
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

/* 有"启动页功能" */
#ifdef TUYA_USE_SPLASH
    /* 字段1: 启动页 */
    switch (MAKEWORD(buf[buf_len], buf[buf_len+1]))
    {
        case RN_SWITCH_ON:  /* 使用启动页 */
            TYBC_CMD_SPLASH_ON;
            break;

        case RN_SWITCH_OFF: /* 关闭启动页 */
            TYBC_CMD_SPLASH_OFF;
            break;
    }
    TYB_SLP_check_data_updated(TYBC_SPLASH_VAL, SPLASH_IDX);    /* 新值更新旧值 */
    buf_len += DP_CMD_FIELD_LEN;
#endif

/* 有"童锁功能" */
#ifdef TUYA_USE_CHILDLOCK
    /* 字段2: 童锁 */
    switch (MAKEWORD(buf[buf_len], buf[buf_len+1]))
    {
        case RN_SWITCH_ON:              /* 打开童锁 */
            TYBC_CMD_CHILDLOCK_ON;
            break;

        case RN_SWITCH_OFF:             /* 关闭童锁 */
            TYBC_CMD_CHILDLOCK_OFF;
            break;
    }
    TYB_SLP_check_data_updated(TYBC_CHILDLOCK_VAL, CHILDLOCK_IDX);  /* 新值更新旧值 */
    buf_len += DP_CMD_FIELD_LEN;
#endif

	/* 区域一、区域二、热水使能不用管 */
    buf_len += DP_CMD_FIELD_LEN;
    buf_len += DP_CMD_FIELD_LEN;
    buf_len += DP_CMD_FIELD_LEN;

/* 有"温度单位设置功能" */
#ifdef TUYA_USE_TEMP_UNIT_SET
    /* 字段: 温度单位 */
	if (sh_temp_unit != MAKEWORD(buf[buf_len], buf[buf_len+1]))
	{
		fg_tuya_temp_unit_switch = TRUE;
	    switch (MAKEWORD(buf[buf_len], buf[buf_len+1]))
	    {
	        default:
	            break;

	        case RN_TEMP_UNIT_C:    /* 摄氏 */
	        case RN_TEMP_UNIT_F:    /* 华氏 */
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
 函 数: TYB_SLP_parse_zone1_func_cmd()
 功 能: 解析常规功能命令透传型功能点
 参 数: buf--接收缓冲区(数据源) dpid:功能点;
 返 回: 空
 其 它: 注意! 此处先解析设定温度，再解析设定模式!!!
        (默认前提: APP显示的模式即为控制器的当前模式，APP上的设定温度与设定/当前
        模式有着对应关系。)
*******************************************************************************/
static void TYB_SLP_parse_zone_func_cmd(U08 *buf, U08 dpid)
{
    U08 buf_len = 0;

    /* 字段0: 解析开关机 */
    buf_len += TYB_SLP_parse_switch(&buf[buf_len], FUNC_CUSTOM, dpid);

    /* 字段1: 解析设定温度 */
    buf_len += TYB_SLP_parse_temp_set(&buf[buf_len], FUNC_CUSTOM, dpid);

	/* 字段2: 解析设定模式 */
	if (dpid == DPID_ZONEA_FUNC_CMD)
    	buf_len += TYB_SLP_parse_mode_set(&buf[buf_len], FUNC_CUSTOM);
}

/* 有"扩展功能" */
#ifdef TUYA_HAS_EXT_FUNC
/*******************************************************************************
 函 数: TYB_SLP_parse_extend_func_cmd()
 功 能: 解析扩展功能命令透传型功能点
 参 数: buf--接收缓冲区(数据源);
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TYB_SLP_parse_extend_func_cmd(U08 *buf)
{
    U08 buf_len = 0;

/* 有"风速功能" */
#ifdef TUYA_USE_WIND
    /* 字段0: 解析设定风速 */
    buf_len += TYB_SLP_parse_wind_set(&buf[buf_len], FUNC_CUSTOM);
#endif

/* 有"湿度功能" */
#ifdef TUYA_USE_HUMI
    /* 字段1: 解析设定湿度 */
    buf_len += TYB_SLP_parse_humi_set(&buf[buf_len], FUNC_CUSTOM);
#endif

/* 有"热水功能" */
#ifdef TUYA_USE_HOT_WATER
    /* 字段2: 解析热水设定温度 */
    buf_len += TYB_SLP_parse_hot_water_set(&buf[buf_len], FUNC_CUSTOM);
#endif
}
#endif


/*******************************************************************************
 函 数: TYB_SLP_parse_dp_data()
 功 能: 解析下发功能点
 参 数: tuya_data--涂鸦数据;
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TYB_SLP_parse_dp_data(TUYA_DATA* tuya_data)
{

    /* 彩屏版本号存储 */
    if (tuya_data->dpid == DPID_NULL)
    {
/* 显示"显示屏版本号" */
#if (TYBC_DISP_HMI_VER_EN == TRUE)
        TYB_SLP_store_hmi_ver(tuya_data->data_buf);
/* 不显示"显示屏版本号" */
#else
        ;//DO NOTHING
#endif
    }
    /* 云端下发功能点解析 */
    else
    {
        /* 解析功能点并更新至本地 */
        switch (tuya_data->dpid)
        {
            /* DPID_1 开关 */
            case DPID_SWITCH:
                TYB_SLP_parse_switch(tuya_data->data_buf, FUNC_STANDARD, DPID_SWITCH);
                SETBIT_N(fg_dpid_upload_pri_high, STD_SWITCH);
                break;

            /* DPID_2 温度设置 */
            case DPID_TEMP_SET:
                TYB_SLP_parse_temp_set(tuya_data->data_buf, FUNC_STANDARD, DPID_TEMP_SET);
                SETBIT_N(fg_dpid_upload_pri_high, STD_TEMP_SET);
                break;

            /* DPID_4 设定模式 */
            case DPID_MODE:
                TYB_SLP_parse_mode_set(tuya_data->data_buf, FUNC_STANDARD);
                SETBIT_N(fg_dpid_upload_pri_high, STD_MODE);
                break;

/* 有"通用设置功能" */
#ifdef TUYA_HAS_GEN_SET
            /* DPID_105 通用设置命令 */
            case DPID_GENERAL_SET_CMD:
                TYB_SLP_parse_general_set_cmd(tuya_data->data_buf);
                SETBIT_N(fg_dpid_upload_pri_high, GENERAL_SET_INFO);
                break;
#endif

            /* DPID_107 区域一功能命令 */
            case DPID_ZONEA_FUNC_CMD:
                TYB_SLP_parse_zone_func_cmd(tuya_data->data_buf, DPID_ZONEA_FUNC_CMD);
                SETBIT_N(fg_dpid_upload_pri_high, ZONEA_FUNC_INFO);
                break;
            /* DPID_109 区域二功能命令 */
            case DPID_ZONEB_FUNC_CMD:
                TYB_SLP_parse_zone_func_cmd(tuya_data->data_buf, DPID_ZONEB_FUNC_CMD);
                SETBIT_N(fg_dpid_upload_pri_high, ZONEB_FUNC_INFO);
                break;
            /* DPID_111 热水功能命令 */
            case DPID_DHW_FUNC_CMD:
                TYB_SLP_parse_zone_func_cmd(tuya_data->data_buf, DPID_DHW_FUNC_CMD);
                SETBIT_N(fg_dpid_upload_pri_high, DHW_FUNC_INFO);
                break;

#if 0
            /* DPID_107 常规功能命令 */
            case DPID_COMMON_FUNC_CMD:
                TYB_SLP_parse_common_func_cmd(tuya_data->data_buf);
                SETBIT_N(fg_dpid_upload_pri_high, COMMON_FUNC_INFO);
                break;

/* 有"扩展功能" */
#ifdef TUYA_HAS_EXT_FUNC
            /* DPID_109 扩展功能命令 */
            case DPID_EXTEND_FUNC_CMD:
                TYB_SLP_parse_extend_func_cmd(tuya_data->data_buf);
                SETBIT_N(fg_dpid_upload_pri_high, EXTEND_FUNC_INFO);
                break;
#endif
#endif

/* 有"模块状态" */
#ifdef TUYA_USE_MOD_STAT
            /* DPID_117 模块状态传输命令 */
            case DPID_MOD_STAT_CMD:
                TYB_SLP_parse_mod_stat_cmd(tuya_data->data_buf);
                break;
#endif

/* 有"参数设置" */
#ifdef TUYA_USE_PARAM_SET
            /* DPID_140 参数传输命令*/
            case DPID_PARAM_CMD:
                TYB_SLP_parse_param_cmd(tuya_data->data_buf);
                break;
#endif

/* 有"用电查询" */
#ifdef TUYA_USE_ELE_STAT
            /* DPID_136 用电状态传输命令*/
            case DPID_ELE_STAT_CMD:
                TYB_SLP_parse_ele_stat_cmd(tuya_data->data_buf);
                break;
#endif

#ifdef TUYA_USE_SYS_STAT
            /* DPID_137 系统状态传输命令*/
            case DPID_SYS_STAT_CMD:
                TYB_SLP_parse_sys_stat_cmd(tuya_data->data_buf);
                break;
#endif

/* 有"历史故障" */
#ifdef TUYA_USE_ERR_HIS
            /* DPID_139 历史故障传输命令*/
            case DPID_ERR_HIS_CMD:
                TYB_SLP_parse_err_his_cmd(tuya_data->data_buf);
                break;
#endif
            default:
                break;
        }
            /* 标记需要快速响应的功能点 */
    }
}


