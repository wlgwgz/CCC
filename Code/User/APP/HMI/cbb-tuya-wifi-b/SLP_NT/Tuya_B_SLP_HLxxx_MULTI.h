/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: Tuya_B_SLP_HLxxx_MULTI.h
 文件描述: (暖通HL系列)涂鸦(B)多联供业务逻辑头文件
 创建人  : 林浩彬
 创建日期: 2020.12.13
*******************************************************************************/

/* 供调试使用 */
#ifdef TUYA_USE_DBG_CONSOLE
    #define TUYA_ARGS    "-dbg"
    #warning "TUYA-DEBUG模式!发布时一定要屏蔽 "TUYA_USE_DBG_CONSOLE" !!!"
#else
    #define TUYA_ARGS    ""
#endif

/* ================================================================================ */
/* 剪 裁 组 合                                                                      */
/* ================================================================================ */
/* 是否存在"状态查询" */
#if (defined(TUYA_USE_SYS_STAT) || defined(TUYA_USE_MOD_STAT))
    #define TUYA_HAS_STAT
#endif

/* 是否存在"通道传输" */
#if (defined(TUYA_USE_MOD_STAT) || defined(TUYA_USE_PARAM_SET) || defined(TUYA_USE_ELE_STAT) || defined(TUYA_USE_ERR_HIS))
    #define TUYA_HAS_TRANS
#endif

/* 是否存在"通用设置" */
//#if (defined(TUYA_USE_LANG_SET) || defined(TUYA_USE_SPLASH) || defined(TUYA_USE_CHILDLOCK))
    #define TUYA_HAS_GEN_SET
//#endif

/* 是否存在"扩展功能" */
#if (defined(TUYA_USE_WIND) || defined(TUYA_USE_HUMI) || defined(TUYA_USE_HOT_WATER))
    #define TUYA_HAS_EXT_FUNC
#endif

/* ================================================================================ */
/* RN 面 板 枚 举                                                                   */
/* ================================================================================ */
/* 单个枚举量映射关系 */
typedef struct tag_RN_ENUM_MAP
{
    U08 local_enum;                     /* 本地枚举量 */
    U08 rn_enum;                        /* RN面板枚举量 */
    U16 msg_id;                         /* 字符串号 */
} RN_ENUM_MAP;

/* 开关状态 */
#define RN_SWITCH_OFF                   (0)    /* 关 */
#define RN_SWITCH_ON                    (1)    /* 开 */

/* 运行状态 */
#define RN_RUN_STATUS_IDLE              (0)    /* 准备就绪 */
#define RN_RUN_STATUS_LAUNCH            (1)    /* 正在启动 */
#define RN_RUN_STATUS_RUN               (2)    /* 运行中 */
#define RN_RUN_STATUS_STOP              (3)    /* 正在停机 */
#define RN_RUN_STATUS_ALERT             (4)    /* 故障告警 */
#define RN_RUN_STATUS_MODE_SWITCHING    (5)    /* 模式切换中 */
#define RN_RUN_STATUS_ANTIFREEZE        (6)    /* 防冻中 */
#define RN_RUN_STATUS_DEFROST           (7)    /* 除霜中 */
#define RN_RUN_STATUS_PREHEAT           (8)    /* 预热中 */
#define RN_RUN_STATUS_CLR_SNOW          (9)    /* 除雪中 */
#define RN_RUN_STATUS_MAX               (10)   /* 最大运行模式数 */

/* 模式 */
#define RN_MODE_AUTO                    (0)    /* 自动 */
#define RN_MODE_COOL                    (1)    /* 制冷 */
#define RN_MODE_HEAT                    (2)    /* 制热 */
#define RN_MODE_FLOOR                   (3)    /* 地暖 */
#define RN_MODE_VENT                    (4)    /* 通风 */
#define RN_MODE_HOTW                    (5)    /* 热水 */
#define RN_MODE_HEAT_FLOOR              (6)    /* 制热+地暖 */
#define RN_MODE_DEHUMI                  (7)    /* 除湿 */
#define RN_MODE_COOL_HOTW               (8)	   /* 制冷+热水 */
#define RN_MODE_HEAT_HOTW               (9)	   /* 制热+热水 */
#define RN_MODE_AUTO_HOTW               (10)   /* 自动+热水 */
#define RN_MODE_MAX                     (11)   /* 最大设定模式数 */

/* 风速 */
#define RN_WIND_LOW                     (0)    /* 低风速 */
#define RN_WIND_MID                     (1)    /* 中风速 */
#define RN_WIND_HIGH                    (2)    /* 高风速 */
#define RN_WIND_AUTO                    (3)    /* 自动 */
#define RN_WIND_SET_MAX                 (4)    /* 最大可设风速数 */
#define RN_WIND_NULL                    (5)    /* 无风速 */
#define RN_WIND_MAX                     (6)    /* 最大风速数 */

/* 语言 */
#define RN_LANGUAGE_CN                  (0)	    /* 中文 */
#define RN_LANGUAGE_EN                  (1)	    /* 英文 */
#define RN_LANGUAGE_SP                  (2)	    /* 西班牙语 */
#define RN_LANGUAGE_GR                  (3)	    /* 希腊语 */
#define RN_LANGUAGE_FR                  (4)	    /* 法语 */
#define RN_LANGUAGE_ITL                 (5)	    /* 意大利语 */
#define RN_LANGUAGE_POL                 (6)	    /* 波兰语 */
#define RN_LANGUAGE_HG                  (7)     /* 匈牙利语 */
#define RN_LANGUAGE_CZ					(8)	    /* 捷克 */
#define RN_LANGUAGE_PT					(9)	    /* 葡萄牙 */
#define RN_LANGUAGE_TR					(10)	/* 土耳其 */
#define RN_LANGUAGE_GE					(11)    /* 德语 */
#define RN_LANGUAGE_NL					(12)    /* 荷兰 */
#define RN_LANGUAGE_SE					(13)    /* 瑞典 */
#define RN_LANGUAGE_DK					(14)    /* 丹麦 */
#define RN_LANGUAGE_NO					(15)    /* 挪威 */
#define RN_LANGUAGE_MAX                 (16)    /* 最大语言数 */

/* 温度单位 */
#define RN_TEMP_UNIT_C					(SH_SET_UNIT_C)
#define RN_TEMP_UNIT_F					(SH_SET_UNIT_F)
#define RN_TEMP_UNIT_MAX				(RN_TEMP_UNIT_F+1)

/* 步进属性[10]~[13] */
#define STEP_BIT                        (10)              /* 步进起始位 */
#define STEP_1                          (1<<STEP_BIT)     /* 步进1 */
#define STEP_5                          (5<<STEP_BIT)     /* 步进5 */
#define STEP_10                         (10<<STEP_BIT)    /* 步进10 */

/* 可选功能 */
#define FUNC_EN_MORE_GEN_SET_BIT        (0) /* "更多"-"通用设置"功能使能位 */
#define FUNC_EN_ADVANCE_ERR_HIS_BIT     (1) /* "高级"-"历史故障"功能使能位 */
#define FUNC_EN_ADVANCE_PARAM_SET_BIT   (2) /* "高级"-"参数设置"功能使能位 */
#define FUNC_EN_MAIN_STAT_BIT           (3) /* "主界面"-"状态查询"功能使能位 */
#define FUNC_EN_ADVANCE_STAT_BIT        (4)	/* "高级"-"状态查询"功能使能位 */
#define FUNC_EN_MAIN_ELE_STAT_BIT       (5) /* "主界面"-"用电查询"功能使能位 */
#define FUNC_EN_MORE_CLO_TIME_BIT       (6) /* "更多"-"云定时"功能使能位 */

/* LOGO列表(后续有新的LOGO可在尾部增加，注意保持唯一性) */
#define SPLASH_LOGO_NULL                (0)	/* 空白(无启动页) */
#define SPLASH_LOGO_PUNP                (1)	/* 广州邦普 */
#define SPLASH_LOGO_AMA                 (2)	/* 浙江正理生能 */
#define SPLASH_LOGO_VECK                (3)	/* 天津维克 */
#define SPLASH_LOGO_TCL                 (4)	/* 中山TCL */

/* 固化字符串ID 与APP唯一对应 */
//起始位置
#define TY_MSGID_STA_BGN			    (1)
#define TY_MSGID_TITTLE_BGN			    (50)
#define TY_MSGID_MODE_BGN			    (200)
#define TY_MSGID_LANG_BGN			    (250)
#define TY_MSGID_TEMP_UNIT_BGN			(300)

#define	TY_MSGID_NULL					(0)  /* 空字符 */
//运行状态
#define	TY_MSGID_STA_READY				(TY_MSGID_STA_BGN+0)  /* 准备就绪 */
#define	TY_MSGID_STA_ON					(TY_MSGID_STA_BGN+1)  /* 正在启动 */
#define	TY_MSGID_STA_RUNNING				(TY_MSGID_STA_BGN+2)  /* 运行中*/
#define	TY_MSGID_STA_OFF				(TY_MSGID_STA_BGN+3)  /* 正在停机 */
#define	TY_MSGID_STA_ALERT				(TY_MSGID_STA_BGN+4)  /* 故障告警 */
#define	TY_MSGID_STA_MODE_SW			(TY_MSGID_STA_BGN+5)  /* 模式切换中 */
#define	TY_MSGID_STA_ANTI_FREEZ			(TY_MSGID_STA_BGN+6)  /* 正在防冻 */
#define	TY_MSGID_STA_DEFRO				(TY_MSGID_STA_BGN+7)  /* 正在除霜 */
#define	TY_MSGID_STA_PRE_HEAT			(TY_MSGID_STA_BGN+8)  /* 正在预热 */
#define	TY_MSGID_STA_SNOW_CLEAR			(TY_MSGID_STA_BGN+9)  /* 正在除雪 */

//标题
#define	TY_MSGID_TIT_SWITCH					(TY_MSGID_TITTLE_BGN+0)  /* 开关 */
#define	TY_MSGID_TIT_SET_TEMP				(TY_MSGID_TITTLE_BGN+1)  /* 设定温度 */
#define	TY_MSGID_TIT_SET_MODE				(TY_MSGID_TITTLE_BGN+2)  /* 设定模式 */
#define	TY_MSGID_TIT_CUR_TEMP				(TY_MSGID_TITTLE_BGN+3)  /* 当前温度 */
#define	TY_MSGID_TIT_SET_DHW_TEMP			(TY_MSGID_TITTLE_BGN+4)  /* 设定热水温度 */
#define	TY_MSGID_TIT_CUR_DHW_TEMP			(TY_MSGID_TITTLE_BGN+5)  /* 当前热水温度 */
#define	TY_MSGID_TIT_ZONE1					(TY_MSGID_TITTLE_BGN+6)  /* 区域1 */
#define	TY_MSGID_TIT_ZONE2					(TY_MSGID_TITTLE_BGN+7)  /* 区域2 */
#define	TY_MSGID_TIT_DHW					(TY_MSGID_TITTLE_BGN+8)  /* 生活热水 */
#define	TY_MSGID_TIT_LANG					(TY_MSGID_TITTLE_BGN+9)  /* 语言 */
#define	TY_MSGID_TIT_SPLASH					(TY_MSGID_TITTLE_BGN+10)  /* 启动页 */
#define	TY_MSGID_TIT_CHILDLOCK				(TY_MSGID_TITTLE_BGN+11)  /* 童锁 */
#define	TY_MSGID_TIT_TEMP_UNIT				(TY_MSGID_TITTLE_BGN+12)  /* 温度单位 */


//模式
#define	TY_MSGID_MODE_AUTO					(TY_MSGID_MODE_BGN+0)	/* 自 动 */
#define	TY_MSGID_MODE_COOL					(TY_MSGID_MODE_BGN+1)	/* 制 冷 */
#define	TY_MSGID_MODE_HEAT					(TY_MSGID_MODE_BGN+2)	/* 制 热 */

//语言
#define	TY_MSGID_LANG_CHN					(TY_MSGID_LANG_BGN+0)	/* 中文 */
#define	TY_MSGID_LANG_EN					(TY_MSGID_LANG_BGN+1)	/* English */
#define	TY_MSGID_LANG_GR					(TY_MSGID_LANG_BGN+2)
#define	TY_MSGID_LANG_FR					(TY_MSGID_LANG_BGN+3)
#define	TY_MSGID_LANG_ITL					(TY_MSGID_LANG_BGN+4)
#define	TY_MSGID_LANG_POL					(TY_MSGID_LANG_BGN+5)
#define	TY_MSGID_LANG_ESP					(TY_MSGID_LANG_BGN+6)
#define	TY_MSGID_LANG_HG					(TY_MSGID_LANG_BGN+7)
#define	TY_MSGID_LANG_GE					(TY_MSGID_LANG_BGN+8)

/* 温度单位 */
#define TY_MSGID_TEMP_UNIT_C				(TY_MSGID_TEMP_UNIT_BGN+0)
#define TY_MSGID_TEMP_UNIT_F				(TY_MSGID_TEMP_UNIT_BGN+1)

/* ================================================================================ */
/* I. 功 能 传 输 协 议 相 关 定 义                                                 */
/* ================================================================================ */
/* 允许上报的故障最大数量限制 */
#define TY_CUR_ERR_MAX                  (10)   /* 允许上报的当前故障最大个数 */
#define TY_HIS_ERR_MAX                  (10)   /* 允许上报的历史故障最大个数 */

/* [0x01] 布尔型字段 数据单元结构 */
typedef struct tag_DP_FIELD_BOOL
{
    U16 type;                           /* [0]:功能禁用,[1]~[7]:备用,[8]~[15]:字段类型值 */
    U16 props;                          /* [0]:可下发,[1]~[7]:备用,[8]~[15]:功能ID */
    I16 value;                          /* 当前值 */
    U16 tittle_msgid;                   /* 标题字符串ID */
} DP_FIELD_BOOL;
#define DP_FIELD_BOOL_SIZE              ((sizeof(DP_FIELD_BOOL)+1)/sizeof(U16))      /* 3个字 */
#define DP_FIELD_BOOL_BYTES             (DP_FIELD_BOOL_SIZE*2)                       /* 6个字节 */

/* [0x02] 数值型字段 数据单元结构 */
typedef struct tag_DP_FIELD_NUMBER
{
    U16 type;                           /* [0]:功能禁用,[1]~[7]:备用,[8]~[15]:字段类型值 */
    U16 props;                          /* [0]:可下发,[1]~[7]:备用,[8]~[15]:功能ID */
    U16 attr;                           /* 属性,[0]~[7]:单位,[8]~[9]:小数点,[10]~[13]:精度 */
    I16 value;                          /* 当前值 */
    I16 max;                            /* 最大值 */
    I16 min;                            /* 最小值 */
    U16 tittle_msgid;                   /* 标题字符串ID */
} DP_FIELD_NUMBER;
#define DP_FIELD_NUMBER_SIZE            ((sizeof(DP_FIELD_NUMBER)+1)/sizeof(U16))    /* 6个字 */
#define DP_FIELD_NUMBER_BYTES           (DP_FIELD_NUMBER_SIZE*2)                     /* 12个字节 */

/* [0x03] 枚举型字段 数据单元结构 */
typedef struct tag_DP_FIELD_ENUM
{
    U16 type;                           /* [0]:功能禁用,[1]~[7]:备用,[8]~[15]:字段类型值 */
    U16 props;                          /* [0]:可下发,[1]~[7]:备用,[8]~[15]:功能ID */
    I16 value;                          /* 当前值 */
    U16 range_len;                      /* 枚举总数(字典中条目个数) */
	U16 tittle_msgid;                   /* 标题字符串ID */
} DP_FIELD_ENUM;
#define DP_FIELD_ENUM_SIZE              ((sizeof(DP_FIELD_ENUM)+1)/sizeof(U16))      /* 4个字 */
#define DP_FIELD_ENUM_BYTES             (DP_FIELD_ENUM_SIZE*2)                       /* 8个字节 */

/* [0x04] 只读数值型字段 数据单元结构 */
typedef struct tag_DP_FIELD_RO_NUM
{
    U16 type;                           /* [0]:功能禁用,[1]~[7]:备用,[8]~[15]:字段类型值 */
    U16 props;                          /* [0]:可下发,[1]~[7]:备用,[8]~[15]:功能ID */
    U16 attr;                           /* 属性,[0]~[7]:单位,[8]~[9]:小数点,[10]~[13]:精度 */
    I16 value;                          /* 当前值 */
    U16 tittle_msgid;                      	/* 标题字符串ID */
} DP_FIELD_RO_NUM;
#define DP_FIELD_RO_NUM_SIZE            ((sizeof(DP_FIELD_RO_NUM)+1)/sizeof(U16))    /* 4个字 */
#define DP_FIELD_RO_NUM_BYTES           (DP_FIELD_RO_NUM_SIZE*2)                     /* 8个字节 */

/* [0x05] 只读字符串型字段 数据单元结构 */
typedef struct tag_DP_FIELD_RO_MSG
{
    U16 type;                           /* [0]:功能禁用,[1]~[7]:备用,[8]~[15]:字段类型值 */
    U16 props;                          /* [0]:可下发,[1]~[7]:备用,[8]~[15]:功能ID */
    U16 tittle_msgid;                   /* 标题字符串ID */
    U16 content_msgid;                  /* 内容字符串ID */
} DP_FIELD_RO_MSG;
#define DP_FIELD_RO_MSG_SIZE            ((sizeof(DP_FIELD_RO_MSG)+1)/sizeof(U16))    /* 3个字 */
#define DP_FIELD_RO_MSG_BYTES           (DP_FIELD_RO_MSG_SIZE*2)                     /* 6个字节 */

/* [0x06] 单字符串型字段  */
typedef struct tag_DP_FIELD_ONE_MSG
{
    U08 type_H;                         /* 字段类型高字节: 字段类型值 */
    U08 type_L;                         /* 字段类型低字节: [0]:功能禁用,[1]~[7]:备用 */
    U08 msg_len;                        /* 内容字符串长度 */
} DP_FIELD_ONE_MSG;
#define DP_FIELD_ONE_MSG_HEADER_BYTES   (sizeof(DP_FIELD_ONE_MSG))                   /* 帧头部分字节数: 3个字节 */

/* [0x07] 单数值型字段 */
typedef struct tag_DP_FIELD_ONE_NUM
{
    U08 type_H;                         /* 字段类型高字节: 字段类型值 */
    U08 type_L;                         /* 字段类型低字节: [0]:功能禁用,[1]~[7]:备用 */
	U08 props_H;
	U08 props_L;
    U08 value_H;                        /* 当前值高字节 */
    U08 value_L;                        /* 当前值低字节 */
} DP_FIELD_ONE_NUM;
#define DP_FIELD_ONE_NUM_BYTES          (sizeof(DP_FIELD_ONE_NUM))                   /* 总字节数: 4个字节 */

/* 系统状态字符串 */
typedef struct tag_DP_FIELD_SYS_MSG_DATA
{
    U16 msg_id;                           /* id */
    U16 msg_len;                          /* 长度 */
} DP_FIELD_SYS_MSG_DATA;
#define DP_FIELD_SYS_MSG_DATA_SIZE     ((sizeof(DP_FIELD_SYS_MSG_DATA)+1)/sizeof(U16))   /* 2个字 */
#define DP_FIELD_SYS_MSG_DATA_BYTES     (DP_FIELD_SYS_MSG_DATA_SIZE*2)                    /* 4个字节 */

/* 系统状态信息字段 */
typedef struct tag_DP_FIELD_SYS_STAT_INFO
{
    U16 attr;                           /* 属性 */
    I16 value;                          /* 数值 */
    U16 msg_len;                        /* 系统状态字符串长度 */
} DP_FIELD_SYS_STAT_INFO;
#define DP_FIELD_SYS_STAT_INFO_SIZE     ((sizeof(DP_FIELD_SYS_STAT_INFO)+1)/sizeof(U16))   /* 3个字 */
#define DP_FIELD_SYS_STAT_INFO_BYTES    (DP_FIELD_SYS_STAT_INFO_SIZE*2)                    /* 6个字节 */

/* 系统状态数据字段 */
typedef struct tag_DP_FIELD_SYS_STAT_DATA
{
    U16 attr;                           /* 属性 */
    I16 value;                          /* 数值 */
} DP_FIELD_SYS_STAT_DATA;
#define DP_FIELD_SYS_STAT_DATA_SIZE     ((sizeof(DP_FIELD_SYS_STAT_DATA)+1)/sizeof(U16))   /* 2个字 */
#define DP_FIELD_SYS_STAT_DATA_BYTES    (DP_FIELD_SYS_STAT_DATA_SIZE*2)                    /* 4个字节 */

/* 当前故障数据字段 */
typedef struct tag_DP_FIELD_ERR_CUR_INFO
{
    U16 id;                             /* 序号(高8位为板号，低8位为单元号，为0xff时不显示序号) */
    U16 err_id;                         /* 故障号 */
} DP_FIELD_ERR_CUR_INFO;
#define DP_FIELD_ERR_CUR_INFO_SIZE      ((sizeof(DP_FIELD_ERR_CUR_INFO)+1)/sizeof(U16))    /* 2个字 */
#define DP_FIELD_ERR_CUR_INFO_BYTES     (DP_FIELD_ERR_CUR_INFO_SIZE*2)                     /* 4个字节 */

/* 历史故障数据字段 */
typedef struct tag_DP_FIELD_ERR_HIS_INFO
{
    U16 id;                             /* 序号(高8位为板号，低8位为单元号，为0xff时不显示序号) */
    U16 mm_dd;                          /* 高8位为月份，低8位为日期 */
    U16 hh_mm;                          /* 高8位为小时，低8位为分钟 */
    U16 msg_len;                        /* 故障字符串长度 */
} DP_FIELD_ERR_HIS_INFO;
#define DP_FIELD_HIS_ERR_INFO_SIZE      ((sizeof(DP_FIELD_ERR_HIS_INFO)+1)/sizeof(U16))    /* 2个字 */
#define DP_FIELD_HIS_ERR_INFO_BYTES     (DP_FIELD_HIS_ERR_INFO_SIZE*2)                     /* 4个字节 */

/* 功能传输命令字段字节数 */
#define DP_CMD_FIELD_LEN                (2)

/* ================================================================================ */
/* II. 通 道 传 输 协 议 相 关 定 义                                                */
/* ================================================================================ */
/* 通道传输帧头字节数 */
#define TRANS_01_HEADER_BYTES           (8)    /* 0x01传送码帧头字节数 */
#define TRANS_02_HEADER_BYTES           (8)    /* 0x02传送码帧头字节数 */
#define TRANS_03_HEADER_BYTES           (9)    /* 0x03传送码帧头字节数 */
#define TRANS_04_HEADER_BYTES           (9)    /* 0x04传送码帧头字节数 */
#define TRANS_05_HEADER_BYTES           (9)    /* 0x05传送码帧头字节数 */
#define TRANS_06_HEADER_BYTES           (9)    /* 0x06传送码帧头字节数 */
#define TRANS_07_HEADER_BYTES           (9)    /* 0x07传送码帧头字节数 */
#define TRANS_08_HEADER_BYTES           (8)    /* 0x08传送码帧头字节数 */
#define TRANS_09_HEADER_BYTES           (8)    /* 0x09传送码帧头字节数 */
#define TRANS_10_HEADER_BYTES           (8)    /* 0x0A传送码帧头字节数 */
#define TRANS_11_HEADER_BYTES           (9)    /* 0x0B传送码帧头字节数 */
#define TRANS_12_HEADER_BYTES           (9)    /* 0x0C传送码帧头字节数 */
#define TRANS_13_HEADER_BYTES           (9)    /* 0x0D传送码帧头字节数 */
#define TRANS_14_HEADER_BYTES           (9)    /* 0x0E传送码帧头字节数 */

/* 通道传输内容字节数 */
#define TRANS_01_CONTENT_BYTES          (RAW_DATA_BYTES - TRANS_01_HEADER_BYTES)    /* 0x01传送码内容字节数 */
#define TRANS_02_CONTENT_BYTES          (RAW_DATA_BYTES - TRANS_02_HEADER_BYTES)    /* 0x02传送码内容字节数 */
#define TRANS_03_CONTENT_BYTES          (RAW_DATA_BYTES - TRANS_03_HEADER_BYTES)    /* 0x03传送码内容字节数 */
#define TRANS_04_CONTENT_BYTES          (RAW_DATA_BYTES - TRANS_04_HEADER_BYTES)    /* 0x04传送码内容字节数 */
#define TRANS_05_CONTENT_BYTES          (RAW_DATA_BYTES - TRANS_05_HEADER_BYTES)    /* 0x05传送码内容字节数 */
#define TRANS_06_CONTENT_BYTES          (RAW_DATA_BYTES - TRANS_06_HEADER_BYTES)    /* 0x06传送码内容字节数 */
#define TRANS_07_CONTENT_BYTES          (RAW_DATA_BYTES - TRANS_07_HEADER_BYTES)    /* 0x07传送码内容字节数 */
#define TRANS_08_CONTENT_BYTES          (RAW_DATA_BYTES - TRANS_08_HEADER_BYTES)    /* 0x08传送码内容字节数 */
#define TRANS_09_CONTENT_BYTES          (RAW_DATA_BYTES - TRANS_09_HEADER_BYTES)    /* 0x09传送码内容字节数 */
#define TRANS_10_CONTENT_BYTES          (RAW_DATA_BYTES - TRANS_10_HEADER_BYTES)    /* 0x0A传送码内容字节数 */
#define TRANS_11_CONTENT_BYTES          (RAW_DATA_BYTES - TRANS_11_HEADER_BYTES)    /* 0x0B传送码内容字节数 */
#define TRANS_12_CONTENT_BYTES          (RAW_DATA_BYTES - TRANS_12_HEADER_BYTES)    /* 0x0C传送码内容字节数 */
#define TRANS_13_CONTENT_BYTES          (RAW_DATA_BYTES - TRANS_13_HEADER_BYTES)    /* 0x0D传送码内容字节数 */
#define TRANS_14_CONTENT_BYTES          (RAW_DATA_BYTES - TRANS_14_HEADER_BYTES)    /* 0x0E传送码内容字节数 */

/* 通道传输内容 */
#define TRANS_CONTENT_NULL              (0)    /* 无效 */
#define TRANS_PARAM                     (1)    /* 参数设置 */
#define TRANS_MOD_STAT                  (2)    /* 模块状态 */
#define TRANS_ELE_STAT                  (3)    /* 电费状态 */
#define TRANS_HIS_ERR                   (4)    /* 历史故障 */
#define TRANS_MAX                       (5)    /* 传输内容类型总数 */

/* 传送码类型 */
#define TRANS_NULL                      (0x00)    /* 无效传送码 */
#define TRANS_PARAM_GRP_DISP_RSRC       (0x01)    /* 传送参数组显示资源 */
#define TRANS_PARAM_GRP_DISP_CFG        (0x02)    /* 传送参数组配置信息 */
#define TRANS_PARAM_RANGE_DISP_RSRC     (0x03)    /* 传送参数选项显示资源 */
#define TRANS_PARAM_ITEM_DISP_RSRC      (0x04)    /* 传送当前组参数显示资源 */
#define TRANS_PARAM_ITEM_DISP_CFG       (0x05)    /* 传送当前组参数配置信息 */
#define TRANS_MOD_STAT_DISP_RSRC        (0x06)    /* 传送模块状态显示资源 */
#define TRANS_MOD_STAT_DISP_CFG         (0x07)    /* 传送模块状态配置信息 */
#define TRANS_ELE_STAT_DISP_RSRC        (0x08)    /* 传送用电状态显示资源 */
#define TRANS_ELE_STAT_DISP_CFG         (0x09)    /* 传送用电状态配置信息 */
#define TRANS_ERR_HIS_DISP_RSRC         (0x0A)    /* 传送历史故障显示资源 */
#define TRANS_SYS_STAT_DISP_RSRC        (0x0B)    /* 传送系统状态显示资源 */
#define TRANS_SYS_STAT_DISP_CFG         (0x0C)    /* 传送系统状态配置信息 */
#define TRANS_SYS_MSG_DISP_RSRC         (0x0D)    /* 传送系统状态字符串显示资源 */
#define TRANS_MOD_MSG_DISP_RSRC         (0x0E)    /* 传送模块状态字符串显示资源 */

/* 传送命令类型(解析码) */
#define TRANS_CMD_NULL                  (0x00)    /* 无效传送命令 */
#define TRANS_CMD_PARAM_GRP_DISP        (0x01)    /* 请求参数组命令 */
#define TRANS_CMD_PARAM_RANGE_DISP      (0x02)    /* 请求参数选项命令 */
#define TRANS_CMD_PARAM_ITEM_DISP       (0x03)    /* 请求当前组参数命令 */
#define TRANS_CMD_PARAM_VAL_SET         (0x04)    /* 设定参数命令 */
#define TRANS_CMD_MOD_STATUS_DISP       (0x05)    /* 请求模块状态命令 */
#define TRANS_CMD_ELE_STATUS_DISP       (0x06)    /* 请求用电状态命令 */
#define TRANS_CMD_HIS_ERR_DISP          (0x07)    /* 请求历史故障命令 */
#define TRANS_CMD_SYS_STATUS_DISP       (0x08)    /* 请求系统状态命令 */

/* 请求类型 */
#define UPLOAD_DISP_ALL                 (0x00)    /* 请求显示资源&配置信息 */
#define UPLOAD_ONE_DISP_RSRC            (0x01)    /* 请求显示资源中的某包数据 */
#define UPLOAD_ONE_DISP_CFG             (0x02)    /* 请求配置信息中的某包数据 */
#define UPLOAD_ALL_DISP_RSRC            (0x03)    /* 请求显示资源 */
#define UPLOAD_ALL_DISP_CFG             (0x04)    /* 请求配置信息 */

/* 通道传输请求类型 */
#define RST_GRP_DISP_ALL                (0)       /* 上传参数组名显示资源&配置信息 */
#define RST_GRP_DISP_RSRC               (1)       /* 只上传参数组名显示资源 */
#define RST_GRP_DISP_CFG                (2)       /* 只上传参数组名配置信息 */
#define CONT_GRP_DISP_RSRC              (3)       /* 指定重传参数组名显示资源中的某包数据 */
#define CONT_GRP_DISP_CFG               (4)       /* 指定重传参数组名配置信息中的某包数据 */
#define RST_RANGE_DISP_RSRC             (5)       /* 上传所有参数选项显示资源 */
#define CONT_RANGE_DISP_RSRC            (6)       /* 指定重传所有参数选项显示资源中的某包数据 */
#define RST_ITEM_DISP_ALL               (7)       /* 上传当前组参数显示资源&配置信息 */
#define RST_ITEM_DISP_RSRC              (8)       /* 只上传当前组参数名显示资源 */
#define RST_ITEM_DISP_CFG               (9)       /* 只上传当前组参数名配置信息 */
#define CONT_ITEM_DISP_RSRC             (10)      /* 指定重传当前组参数名显示资源中的某包数据 */
#define CONT_ITEM_DISP_CFG              (11)      /* 指定重传当前组参数名配置信息中的某包数据 */
#define RST_MOD_STAT_DISP_ALL           (12)      /* 上传模块状态显示资源&配置信息 */
#define RST_MOD_STAT_DISP_RSRC          (13)      /* 只上传模块状态显示资源 */
#define RST_MOD_STAT_DISP_CFG           (14)      /* 只上传模块状态配置信息 */
#define CONT_MOD_STAT_DISP_RSRC         (15)      /* 指定重传模块状态显示资源中的某包数据 */
#define CONT_MOD_STAT_DISP_CFG          (16)      /* 指定重传模块状态配置信息中的某包数据 */
#define RST_ELE_STAT_DISP_ALL           (17)      /* 上传用电状态显示资源&配置信息 */
#define RST_ELE_STAT_DISP_RSRC          (18)      /* 只上传用电状态显示资源 */
#define RST_ELE_STAT_DISP_CFG           (19)      /* 只上传用电状态配置信息 */
#define CONT_ELE_STAT_DISP_RSRC         (20)      /* 指定重传用电状态显示资源中的某包数据 */
#define CONT_ELE_STAT_DISP_CFG          (21)      /* 指定重传用电状态配置信息中的某包数据 */
#define RST_ERR_HIS_DISP_RSRC           (22)      /* 只上传历史故障显示资源 */
#define CONT_ERR_HIS_DISP_RSRC          (23)      /* 指定重传历史故障显示资源中的某包数据 */
#define RST_SYS_STAT_DISP_ALL           (24)      /* 上传系统状态显示资源&配置信息 */
#define RST_SYS_MSG_DISP_RSRC           (25)      /* 只上传系统状态字符串显示资源 */
#define RST_SYS_STAT_DISP_RSRC          (26)      /* 只上传系统状态显示资源 */
#define RST_SYS_STAT_DISP_CFG           (27)      /* 只上传系统状态配置信息 */
#define CONT_SYS_MSG_DISP_RSRC          (28)      /* 指定重传系统状态字符串显示资源中的某包数据 */
#define CONT_SYS_STAT_DISP_RSRC         (29)      /* 指定重传系统状态显示资源中的某包数据 */
#define CONT_SYS_STAT_DISP_CFG          (30)      /* 指定重传系统状态配置信息中的某包数据 */
#define RST_MOD_MSG_DISP_RSRC           (31)      /* 只上传模块状态显示资源 */
#define CONT_MOD_MSG_DISP_RSRC          (32)      /* 指定重传模块状态显示资源中的某包数据 */
#define TRANS_REQUEST_MAX               (33)      /* 最大通道传输请求类型数 */

/* 用电状态属性 */
#define CHART_BGN_BIT                   (2)	/* 所处统计表        起始位 */
#define ELE_DECIMAL_BGN_BIT             (8) /* 小数位数      起始位 */
#define SH_UNIT_kWH                     (0) /* 0 用电量(kWH) */

/* 通道传输请求信息 */
#define TRANS_VERIF_CODE_LEN            (3)       /* 验证码字节数 */
#define TRANS_ADDR_INFO_MAX             (2)       /* 地址信息个数 */
typedef struct tag_TRANS_INFO
{
    U08 trigger_src;                                       /* 触发源 */
    U08 verif_code[TRANS_VERIF_CODE_LEN];                  /* 验证码 */
    U08 parse_code;                                        /* 解析码 */
    U08 request_code;                                      /* 请求码 */
    U08 trans_code;                                        /* 传送码 */
    U08 pkg_id;                                            /* 起始数据包号 */
    U08 addr[TRANS_ADDR_INFO_MAX];                         /* 地址信息 */
    U08 request_type[_DIVUCARRY(TRANS_REQUEST_MAX, 8)];    /* 通道传输请求类型 */
} TYB_TRANS_INFO;

/* 队列信息 */
typedef struct {
    U08 head;    /* 头游标 */
    U08 end;     /* 尾游标 */
    U08 cnt;     /* 节点计数 */
} TYB_TRANS_QUEUE;

/* 通道传输请求队列 */
#define TYB_TRANS_NODE_MAX                    (5)  /* 队列节点最大个数 */
typedef struct {
    TYB_TRANS_QUEUE queue;                         /* 队列信息 */
    TYB_TRANS_INFO node[TYB_TRANS_NODE_MAX];           /* 节点信息 */
} TYB_TRANS_CACHE;

/* 显示资源 数据单元结构 */
typedef struct tag_DP_FIELD_DISP_RSRC
{
    U16 msg_id;     /* 字符串ID */
    U16 msg_len;    /* 字符串长度 */
} DP_FIELD_DISP_RSRC;
#define DP_FIELD_DISP_RSRC_SIZE               ((sizeof(DP_FIELD_DISP_RSRC)+1)/sizeof(U16))              /* 2个字 */
#define DP_FIELD_DISP_RSRC_BYTES              (DP_FIELD_DISP_RSRC_SIZE*2)                               /* 4个字节 */

/* 参数组配置信息 数据单元结构 */
typedef struct tag_DP_FIELD_PARAM_GRP_DISP_CFG
{
    U16 msg_id;    /* 字符串ID */
    U16 attr;      /* 参数组属性(与SmartHmi一致,[1]~[4]:权限,[12]:隐藏) */
} DP_FIELD_PARAM_GRP_DISP_CFG;
#define DP_FIELD_PARAM_GRP_DISP_CFG_SIZE      ((sizeof(DP_FIELD_PARAM_GRP_DISP_CFG)+1)/sizeof(U16))     /* 2个字 */
#define DP_FIELD_PARAM_GRP_DISP_CFG_BYTES     (DP_FIELD_PARAM_GRP_DISP_CFG_SIZE*2)                      /* 4个字节 */

/* 当前组参数配置信息 数据单元结构 */
typedef struct tag_DP_FIELD_PARAM_ITEM_DISP_CFG
{
    U16 msg_id;     /* 字符串ID */
    U16 attr;       /* 参数属性(与SmartHmi一致,[0]~[7]:单位,[8]~[9]:小数位,[10]:隐藏,[11]:不可修改,[12]~[14]:权限,[15]:位变量) */
    I16 value;      /* 当前值 */
    I16 max;        /* 最大值 */
    I16 min;        /* 最小值 */
} DP_FIELD_PARAM_ITEM_DISP_CFG;
#define DP_FIELD_PARAM_ITEM_DISP_CFG_SIZE     ((sizeof(DP_FIELD_PARAM_ITEM_DISP_CFG)+1)/sizeof(U16))    /* 5个字 */
#define DP_FIELD_PARAM_ITEM_DISP_CFG_BYTES    (DP_FIELD_PARAM_ITEM_DISP_CFG_SIZE*2)                     /* 10个字节 */

/* 模块状态配置信息 数据单元结构 */
typedef struct tag_TY_MOD_STAT_DISP_CFG
{
    U16 msg_id;    /* 字符串ID */
    U16 id;        /* 序号(高8位为模块号，低8位为压机号，为0xff时不显示序号) */
    U16 attr;      /* 模拟量属性(与SmartHmi一致,[0]~[7]:单位,[8]~[9]:小数位,[10]:隐藏,[11]:不可修改,[12]~[14]:权限,[15]:位变量) */
    U16 value;     /* 当前值 */
} DP_FIELD_MOD_STAT_DISP_CFG;
#define DP_FIELD_MOD_STAT_DISP_CFG_SIZE       ((sizeof(DP_FIELD_MOD_STAT_DISP_CFG)+1)/sizeof(U16))      /* 4个字 */
#define DP_FIELD_MOD_STAT_DISP_CFG_BYTES      (DP_FIELD_MOD_STAT_DISP_CFG_SIZE*2)                       /* 8个字节 */

/* 用电状态配置信息 数据单元结构 */
typedef struct tag_TY_ELE_STAT_DISP_CFG
{
    U16 msg_id;    /* 字符串ID */
    U16 mode_id;
    U16 attr;      /* 用电量属性 */
    U16 value;     /* 当前值 */
} DP_FIELD_ELE_STAT_DISP_CFG;
#define DP_FIELD_ELE_STAT_DISP_CFG_SIZE       ((sizeof(DP_FIELD_ELE_STAT_DISP_CFG)+1)/sizeof(U16))      /* 4个字 */
#define DP_FIELD_ELE_STAT_DISP_CFG_BYTES      (DP_FIELD_ELE_STAT_DISP_CFG_SIZE*2)                       /* 8个字节 */

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
/* 外 部 接 口 ( 暖 通 H L 系 列 业 务 逻 辑 )                                      */
/* ================================================================================ */
/* SmartHMI调用 */
TUYA_B_SLP_EXT BOOL TYB_SLP_SH_upload_tuya_data(U08 *dest_buf, U16 data_addr);  /* 功能点上报 */

/* HMI或SegHMI调用 */
TUYA_B_SLP_EXT U08 TYB_SLP_select_upload_dpid_idx(void);                        /* 选择需要上报的功能点 */
TUYA_B_SLP_EXT void TYB_create_one_num_field(U08 *buf, U08 *acc_len, I16 value, U08 func_attr, U08 func_id);

