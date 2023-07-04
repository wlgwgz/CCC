#ifndef PARA_H
#define PARA_H   1

#ifdef  PARA_GLOBALS
#define PARA_EXT
#else
#define PARA_EXT extern
#endif

//---------------------------------------------------------------------------------
// #define USE_TRAS_F_TEMPERATURE       1           /* 传送的是华氏温度值 */

typedef struct tagPARA
{
    I16  min;       /* 最小值 */
    I16  init;      /* 初始值 */
    I16  max;       /* 最大值 */
    U16  sign;      /* 属性 */
} PARAM;

typedef struct tagPARAMITEM
{
     U08  items;        // 参数个数
     PARAM *array;      // 参数属性地址
     I16  *data_c;      // 参数内存区地址(摄氏)
     I16  *data_f;      // 参数内存区地址(华氏)

     U08  chip;         // 参考下列chip相关属性

     U08  head;         // 参数修改保存的始号
     U08  end;          // 参数修改保存的止号

     U16  sign;         // 参数组的属性/
} PARAMITEM;

// chip相关属性
#define PARA_NEED_STORE     0x01    // (bit0)本参数组需要保存
#define PARA_STORE_PREP     0x08    // (bit3)本参数组还未开始保存
#define PARA_NEED_BRO       0x80    // (bit7)本参数组需要广播

//---------------------------------------------------------------------------------
/* 参数属性的16位含义：*/
/* 第0~1位: 小数点 */
#define D_DOT1      0x0001  /* 1位小数点 */
#define D_DOT2      0x0002  /* 2位小数点 */
#define D_DOT3      0x0003  /* 3位小数点 */
#define D_DOTMASK   0x0003  /* 屏蔽字 */
/* 第2~3位: */
/* 第4~7位: 单位 */
#define UINT_0      (0<<4)      /* 无 */
#define UINT_C      (1<<4)      /* 温度() */
#define UINT_A      (2<<4)      /* 电流(A) */
#define UINT_PR     (3<<4)      /* 压力(bar) */
#define UINT_PE     (4<<4)      /* 百分比(%) */
#define UINT_F      (5<<4)      /* 频率(HZ) */
#define UINT_RS     (6<<4)      /* 转速(rps) */
#define UINT_RM     (7<<4)      /* 转速(rpm) */
#define UINT_S      (8<<4)      /* 秒 */
#define UINT_M      (9<<4)      /* 分 */
#define UINT_H      (10<<4)     /* 时 */
#define UINT_D      (11<<4)     /* 天 */
#define UINT_ST     (12<<4)     /* 步 */
#define UINT_V      (13<<4)     /* 电压(V) */
#define UINT_LH     (14<<4)     /* 水流量(L/H) */
#define UINT_MASK   (0x000f<<4) /* 屏蔽字 */
/* 第8位: */
#define D_BIT       0x0100      /* 位操作 */
/* 第9位: */
#define D_DE        0x0200      /* 温差 */
/* 第10位: */
#define D_HIDE      0x0400      /* 隐藏属性 */
/* 第11位: */
#define D_NEXCH     0x0800      /* 不能修改该项 */
/* 第12~14位: 权限级别(由低到高) */
#define D_USER      (1<<12)     /* 用户设置 */
#define D_PRO       (2<<12)     /* 工程设置 */
#define D_SER       (3<<12)     /* 维修者设置 */
#define D_FAC       (4<<12)     /* 厂家设置 */
#define D_PUNP      (5<<12)     /* 邦普设置 */
#define D_SAFEMASK  (0x0007<<12)    /* 屏蔽字 */
/* 第15位:需要时可删除该位定义 */
#define D_RNEXCH     0x8000     /* 运行中不能修改该项 */

/* 通用隐藏 */
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
    U16  Addr;      // 数据起始地址
    U16  DataNum;   // 数据字节个数（对于参数组，则是每组参数预留的个数）
} EEDATA;           // EEPROM存储数据的结构



#define DATA_READ   0   // 读
#define DATA_WRITE  1   // 写
#define DATA_INIT   2   // 初始化
#define DATA_RESET  3   // 复位内存数据（不写入）


// 0x0400以前的内容不能通过邦普测试初始化
// 0x0400~0x0FFF的内容 只能通过邦普测试初始化，不能由厂家初始化
#define MEM_PASSWORD_ADDR       0x0400      // 密码存储首地址(预留128字节，存储格式：第1组(8字节密码+2字节CRC),第2组密码...，最多可存放12组)
#define MEM_ID_CODE_ADDR        0x0480      // 存放产品识别码(预留16字节，存储格式：产品识别码+2字节CRC)

//#define MEM_           0x0490~0x049F      // 预留存放厂家相关信息(预留16字节，存储格式：厂家相关信息+2字节CRC)
                                            // 比如可用于CDTU的厂家识别码(2字节)
//#define MEM_           0x04A0~0x069F      // 预留存放邦普记录信息(预留512字节)
                                            // 比如可用于停机记录存储(48字节)

//#define MEM_           0x06A0~0x0FFF      // 预留（每次分配新空间后请计算更新）

// 0x1000~0x107F 小数据优先存在此区间
#define MEM_LANGUAGE_ADDR       0x1000      // 语言设置存储首地址(预留4字节，存储格式：1字节语言设置+2字节CRC)
#define MEM_AUTO_RUN_ADDR       0x1004      // 运行状态存储首地址(预留4字节，存储格式：1字节运行状态+2字节CRC)
//#define MEM_POWER_TIME_ADDR       0x1008      // 上电时间存储首地址(预留4字节，存储格式：2字节上电小时+2字节CRC)
#define MEM_TEMP_UNIT_ADDR		0x1008

//#define MEM_           0x100C~0x107F      // 预留（每次分配新空间后请计算更新）


#define MEM_MACHINE_CODE_ADDR   0x1080      // 机组编码存储首地址(预留32字节，存储格式：机组编码+2字节CRC)
#define MEM_RUNTIME_ADDR        0x10A0      // 本模块压机累计运行时间的存储首地址(预留32字节(目前只用到？)，存储格式：压机运行时间数据+2字节CRC，最多可放4压机+1水泵+4风机的累计时间数据)
#define MEM_SYSTIME_ADDR        0x10C0      // 系统累计运行时间的存储首地址(预留8字节(目前只用到？)，存储格式：4字节系统运行时间数据+2字节CRC)
//#define MEM_           0x10C8~0x10CF      // 预留（每次分配新空间后请计算更新）


#define MEM_DM603_TIME_ADDR     0x10D0      // 定时存储首地址(预留64字节，存储格式：定时数据+2字节CRC)
                                            // 一次定时     ->  3字节(B0.b6:一次定时禁用/使用,B0.b7:停机/启动；B1:时、B2:分)
                                            //  N组定时     ->  3字节(B0:周、B1:时、B2:分)
                                            // 一共          =  3+3*6组 = 21字节 + 2字节CRC

#define MEM_HIS_ERROR_ADDR      0x1110      // 历史故障存储首地址(预留96字节，存储格式：80字节故障数据+2字节CRC)
                                            // 历史故障( MAX_HIS_ERROR 个)
                                            // 故障号       -> 1 字节
                                            // 模块号       -> 1 字节
                                            // 附加号      -> 1 字节 （用于特殊用途，比如系统故障中变频故障的更具体故障号。不用置0）
                                            // 月 日        -> 5 字节 （增加时分秒则增加3字节）
                                            // 无时间故障数据：(3+2)*MAX_HIS_ERROR ＝ 50 字节故障数据+2字节CRC
                                            // 有时间故障数据：(3+5)*MAX_HIS_ERROR ＝ 80 字节故障数据+2字节CRC

#define MEM_PWD_TIME_ADDR       0x1170      /* 掉电前时间(共占14字节) */
#define MEM_INVT_EXP_INFO_ADDR  0x1180      /* 变频异常信息:共占544(0x0220)字节(包含CRC) */
#define MEM_SKIP_PREHEAT_ADDR   0x13A0
#define MEM_HOLIDAY_ADDR        0x13A4      // 9字节休假模式数据+2CRC
#define MEM_POWER_USAGE_WEEK_ADDR    0x13B0     /* 最近一周用电量信息(88字节数据+2字节CRC) */
#define MEM_POWER_USAGE_YEAR_ADDR    0x1410	    /* 最近一年用电量信息(148字节数据+2字节CRC) */
//#define MEM_           0x14B0~0x2FFF      // 预留（每次分配新空间后请计算更新）

// 0x3000~0x3FFF分配给参数
#define MEM_LIMIT_ADDR          0x3000      // 系统维护存储首地址(预留16字节，存储格式：4字节使用期限+2字节CRC)
//#define MEM_MODBUS_ADDR           0x3010      // Modbus通讯参数首地址(预留16字节，存储格式：参数+2字节CRC)
//#define MEM_           0x3020~0x30FF      // 预留（每次分配新空间后请计算更新）
#define MEM_PARA_ADDR           0x3100      // 参数存储首地址(预留0x3100~0x3FFF，存储格式：第1组(参数+2字节CRC)，第2组...)


#define PARA_WR_MAX         4       /* 每次只能写4个参数 */
PARA_EXT U08 PARA_GROUP_NUM;        /* 参数组数 */

/* 参数数组大小定义  */
// 每组参数占用空间(参数项+CRC)：   (GROUP_SIZE*2)+2    (最后2字节放CRC)
// 每组CRC存放位置：                (GROUP_SIZE*2)
#define GROUP_MAX   15
#define GROUP_SIZE  48      // 每组预留参数个数（字）
#define CRC_SIZE    1       // CRC占1个字（2字节）

#define GROUP_USER                   (0)	/* 用户参数 */
#define GROUP_SYS                    (1)	/* 系统参数 */
#define GROUP_COMP                   (2)	/* 压机设置 */
#define GROUP_FAN                    (3)	/* 风机设置 */
#define GROUP_EEV                    (4)	/* 电膨设置 */
#define GROUP_EEV_M                  (5)	/* 主阀设置 */
#define GROUP_EEV_S                  (6)	/* 辅阀设置 */
#define GROUP_DEFROST                (7)	/* 除霜设置 */
#define GROUP_ATFZ_FCU               (8)	/* 防冻及风盘 */
#define GROUP_PROTECT                (9)	/* 保护设置 */
#define GROUP_INPUT                  (10)                              // 开关量设置-参数组序号
#define GROUP_DEBUG                  (11)	/* 调试及测试 */
#define GROUP_ENERGY                 (12)
#define GROUP_PRO                    (13)
#define GROUP_WF                     (14)	/* 工况频率表 */

/* 主模块参数缓存 */
PARA_EXT I16  para_C[GROUP_MAX][GROUP_SIZE];        /* 摄氏度: 最后一个为CRC校验值 */
PARA_EXT I16  para_F[GROUP_MAX][GROUP_SIZE];        /* 华氏度: 最后一个为CRC校验值 */

/* 从模块参数缓存 */
PARA_EXT I16  s_para_C[GROUP_MAX][GROUP_SIZE];

// 参数定义形式宏
#define P_g_i(f, g, i)                (f ? g : i)                         // 组号或序号
#define P_grp(name)                   P_##name(TRUE)                      // 组号
#define P_idx(name)                   P_##name(FALSE)                     // 序号
#define P_val(name)                   (para_C[P_grp(##name)][P_idx(##name)])  // 参数值
#define P_ptr(name)                  (&para_C[P_grp(##name)][P_idx(##name)])  // 参数指针
#define P_isP(g, i, name)             (((g)==(P_grp(##name))) && ((i)==(P_idx(##name))))
#define P_GaI(name)                    P_grp(##name),P_idx(##name)

// 系统维护------------------------------------------------------------------------------------------------------------

PARA_EXT const PARAM    para_item_90[];

#define MAX_LMT     2
PARA_EXT I16  limit_para_C[MAX_LMT];
PARA_EXT I16  limit_para_F[MAX_LMT];                    // 华氏(参数格式保持一致)

// 系统维护模式 limit_type
#define STOP_UNIT       0   // 强制关机
#define KEEP_RUN        1   // 保持运行


#define limit_value             (limit_para_C[0])       // 系统维护设置
#define limit_type              (limit_para_C[1])       // 系统维护模式：0强制关机，1保持运行

PARA_EXT U16 limit_val_bak;         /* 使用期限设置值备份 */

//----------------------------------------------------------------------------------------------------------------------

// 机组类型 val_air_type
#define HEAT_ONLY       0//单暖
#define REFR_HEAT       1//冷暖
#define COOL_ONLY       2//单冷
#define MAC_TP_MAX      3
//#define HEAT_ONLY       0//单暖
//#define REFR_HEAT       1//冷暖
//#define COOL_ONLY       2//单冷
//#define HOT_WATER_ONLY  3//热水
//#define REFR_HEAT_HW    4//冷暖+热水
//#define MAC_TP_MAX      5


/* !!注：机型位置需遵守以下原则，否则影响不同程序版本间的主从机型判断
       1、机型位置不能变动
       2、新增机型只能在后面插入，不能在中间插入
       3、新增机型时，注意更新err_mod_mac_set()机型匹配规则
*/
#if (defined(M_DEBUG))
typedef enum MAC_MODEL
{
    MAC_NONE = 0,
    MAC_MULTI_MODEL_1,        /* 三模块使用变频器 */
    MAC_MULTI_MODEL_2,        /* 三模块不使用变频器 */
    MAC_MAX                   /* 新增机型在MAC_MAX之前插入 */
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
    MAC_MAX                     /* 新增机型在MAC_MAX之前插入 */
}MAC_MODEL_ENUM;
#elif (defined(M_DISEN))
typedef enum MAC_MODEL
{
    MAC_NONE = 0,       /* 未设置 */
    MAC_DSKDLNR8BpN3HD, /* DSKDLNR8BpN3HD */
    MAC_DSKDLNR10BpN3H, /* DSKDLNR10BpN3H */
    MAC_DSKDLNR12BpN3H, /* DSKDLNR12BpN3H */
    MAC_DSKDLNR14BpN3H, /* DSKDLNR14BpN3H */
    MAC_DSKDLNR16BpN3H, /* DSKDLNR16BpN3H */
    MAC_DSKDLNR28BpN3SH,/* DSKDLNR28BpN3SH */
    MAC_DSKDLNR35BpN3SH,/* DSKDLNR35BpN3SH */
    MAC_DSKDLNR10BhN5H, /* DSKDLNR10BhN5H */
    MAC_DSKDLNR16BhN5H, /* DSKDLNR16BhN5H */
    MAC_MAX,            /* 新增机型在MAC_MAX之前插入 */
} MAC_MODEL_ENUM;
#elif (defined(M_PAIWO))
typedef enum MAC_MODEL
{
    MAC_NONE = 0,       /* 未设置 */
    MAC_MODEL_1,        /* 单相4P */
    MAC_MODEL_2,        /* 三相4P */
    MAC_MODEL_3,        /* 单相3p */
    MAC_MODEL_4,        /* 三相5p */
    MAC_MODEL_5,        /* 三相6P */
    MAC_MAX,            /* 新增机型在MAC_MAX之前插入 */
} MAC_MODEL_ENUM;
#elif (defined(M_NIUENTAI))
typedef enum MAC_MODEL
{
    MAC_NONE = 0,       /* 未设置 */
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
    MAC_MAX,            /* 新增机型在MAC_MAX之前插入 */
} MAC_MODEL_ENUM;
#elif (defined(M_HONGHE))
typedef enum MAC_MODEL
{
    MAC_NONE = 0,       /* 未设置 */
    MAC_FTD10R32,       /* FTD10R32 */
    MAC_FTD8R32,        /* FTD8R32 */
    MAC_FTD6R32,        /* FTD6R32 */
    MAC_FTS16R32,       /* FTS16R32 */
    MAC_FTS14R32,       /* FTS14R32 */
    MAC_FTS12R32,       /* FTS12R32 */
    MAC_FTD16R32,       /* FTD16R32 */
    MAC_FTD14R32,       /* FTD14R32 */
    MAC_FTD12R32,       /* FTD12R32 */
    MAC_MAX,            /* 新增机型在MAC_MAX之前插入 */
} MAC_MODEL_ENUM;
#elif (defined(M_SHENGNENG))
typedef enum MAC_MODEL
{
    MAC_NONE = 0,       /* 未设置 */
    MAC_WDLRK8IBMA1V1,  /* WDLRK8IBMA1V1 */
    MAC_WDLRK10IBMA1V1, /* WDLRK10IBMA1V1 */
    MAC_MAX,            /* 新增机型在MAC_MAX之前插入 */
} MAC_MODEL_ENUM;
#elif (defined(M_CHANGHONG)||defined(M_TONGYONG)||defined(M_WANHE)||defined(M_CHANGFAN))
typedef enum MAC_MODEL
{
    MAC_NONE = 0,
    MAC_MAX                     /* 新增机型在MAC_MAX之前插入 */
}MAC_MODEL_ENUM;
#endif

typedef struct tagPARA_FAC
{
    U08 grp;
    U08 idx;
    I16 para[MAC_MAX];
}PARA_FAC;
extern const PARA_FAC para_fac[];

PARA_EXT I16 MacTypeMaster;         /* 主机机型 */
#define MACTYPE_IS_SAME (MacTypeMaster==val_mac_model)      /* 主机与从机型号相同 */


#define HIEST_COOL              (35)        /* 制冷上限最大值 */
#define LOEST_COOL             (-10)        /* 制冷下限最小值 */
#define HIEST_HEAT              (80)        /* 制热上限最大值 */
#define LOEST_HEAT               (0)        /* 制热下限最小值 */
#define HIEST_HOTW      (HIEST_HEAT)        /* 热水上限最大值 */
#define LOEST_HOTW      (LOEST_HEAT)        /* 热水下限最小值 */
//=====================================================================================
/* 01用户参数 */
#define P_sv_mode(f)                                        P_g_i(f, 0, 0)	/* 设定控制模式 */
#define P_sv_cool(f)                                        P_g_i(f, 0, 1)	/* 设定制冷温度(度) */
#define P_sv_heat(f)                                        P_g_i(f, 0, 2)	/* 设定制热温度(度) */
#define P_sv_hotw(f)                                        P_g_i(f, 0, 3)	/* 热水设定温度 */
#define P_sv_room_cool(f)                                   P_g_i(f, 0, 4)	/* 制冷设定室温 */
#define P_sv_room_heat(f)                                   P_g_i(f, 0, 5)	/* 制热设定室温 */
#define P_sv_cool_zone_B(f)                                 P_g_i(f, 0, 6)	/* 制冷设定温度(B) */
#define P_sv_room_cool_zone_B(f)                            P_g_i(f, 0, 7)	/* 制冷设定室温(B) */
#define P_sv_heat_zone_B(f)                                 P_g_i(f, 0, 8)	/* 制热设定温度(B) */
#define P_sv_room_heat_zone_B(f)                            P_g_i(f, 0, 9)	/* 制热设定室温(B) */
#define P_val_power_mode(f)                                 P_g_i(f, 0,10)	/* 功率模式 */
#define P_val_mode_silent_bak(f)                            P_g_i(f, 0,11)	/* 静音模式 */
#define P_val_timed_ster_fun(f)                             P_g_i(f, 0,12)	/* 定时杀菌功能 */
#define P_val_ster_temp(f)                                  P_g_i(f, 0,13)	/* 杀菌温度 */
#define P_val_ster_max_cycle(f)                             P_g_i(f, 0,14)	/* 杀菌最大周期 */
#define P_val_ster_high_temp_Time(f)                        P_g_i(f, 0,15)	/* 杀菌高温时间 */
#define P_val_bak_00_11(f)                                  P_g_i(f, 0,16)	/* 备用 */
#define P_val_bak_00_12(f)                                  P_g_i(f, 0,17)	/* 备用 */
#define P_val_bak_00_13(f)                                  P_g_i(f, 0,18)	/* 备用 */
#define P_val_bak_00_14(f)                                  P_g_i(f, 0,19)	/* 备用 */
#define P_val_bak_00_15(f)                                  P_g_i(f, 0,20)	/* 备用 */
#define P_val_bak_00_16(f)                                  P_g_i(f, 0,21)	/* 备用 */
#define P_val_bak_00_17(f)                                  P_g_i(f, 0,22)	/* 备用 */
#define P_val_lock_screen(f)                                P_g_i(f, 0,23)	/* 锁屏功能设置 */
#define P_val_alarm_sound_set(f)                            P_g_i(f, 0,24)	/* 报警音设置 */
#define P_val_zone_A_cool_weather_curve(f)                  P_g_i(f, 0,25)	/* A区制冷曲线 */
#define P_val_zone_A_heat_weather_curve(f)                  P_g_i(f, 0,26)	/* A区制热曲线 */
#define P_val_zone_B_cool_weather_curve(f)                  P_g_i(f, 0,27)	/* B区制冷曲线 */
#define P_val_zone_B_heat_weather_curve(f)                  P_g_i(f, 0,28)	/* B区制热曲线 */
#define P_val_cool_curve_9_TA_1(f)                          P_g_i(f, 0,29)	/* 曲线9制冷环温1 */
#define P_val_cool_curve_9_TA_2(f)                          P_g_i(f, 0,30)	/* 曲线9制冷环温2 */
#define P_val_cool_curve_9_TWout2_1(f)                      P_g_i(f, 0,31)	/* 曲线9制冷出温1 */
#define P_val_cool_curve_9_TWout2_2(f)                      P_g_i(f, 0,32)	/* 曲线9制冷出温2 */
#define P_val_heat_curve_9_TA_1(f)                          P_g_i(f, 0,33)	/* 曲线9制热环温1 */
#define P_val_heat_curve_9_TA_2(f)                          P_g_i(f, 0,34)	/* 曲线9制热环温2 */
#define P_val_heat_curve_9_TWout2_1(f)                      P_g_i(f, 0,35)	/* 曲线9制热出温1 */
#define P_val_heat_curve_9_TWout2_2(f)                      P_g_i(f, 0,36)	/* 曲线9制热出温2 */
#define P_val_cell_enable_00(f)                             P_g_i(f, 0,37)  /* 00#模块设置 */	/* 使用指针访问 */
#define P_val_cell_enable_01(f)                             P_g_i(f, 0,38)	/* 01#模块设置 */
#define P_val_cell_enable_02(f)                             P_g_i(f, 0,39)	/* 02#模块设置 */
#define P_val_cell_enable_03(f)                             P_g_i(f, 0,40)	/* 03#模块设置 */
#define P_val_cell_enable_04(f)                             P_g_i(f, 0,41)	/* 04#模块设置 */
#define P_val_cell_enable_05(f)                             P_g_i(f, 0,42)	/* 05#模块设置 */
#define P_val_cell_enable_06(f)                             P_g_i(f, 0,43)	/* 06#模块设置 */
#define P_val_cell_enable_07(f)                             P_g_i(f, 0,44)	/* 07#模块设置 */
#define sv_mode                                             (P_val(sv_mode                            ))/* 设定控制模式 */
#define sv_cool                                          (10*P_val(sv_cool                            ))/* 设定制冷温度(度) */
#define sv_heat                                          (10*P_val(sv_heat                            ))/* 设定制热温度(度) */
#define sv_hotw                                          (10*P_val(sv_hotw                            ))/* 热水设定温度 */
#define sv_room_heat                                     (10*P_val(sv_room_heat                       ))/* 制热设定室温 */
#define sv_room_cool                                     (10*P_val(sv_room_cool                       ))/* 制冷设定室温 */
#define sv_cool_zone_B                                   (10*P_val(sv_cool_zone_B                     ))/* 制冷设定温度(B) */
#define sv_room_cool_zone_B                              (10*P_val(sv_room_cool_zone_B                ))/* 制冷设定室温(B) */
#define sv_heat_zone_B                                   (10*P_val(sv_heat_zone_B                     ))/* 制热设定温度(B) */
#define sv_room_heat_zone_B                              (10*P_val(sv_room_heat_zone_B                ))/* 制热设定室温(B) */
#define val_power_mode                                      (P_val(val_power_mode                     ))/* 功率模式 */
#define val_mode_silent_bak                                 (P_val(val_mode_silent_bak                ))/* 静音模式 */
#define val_timed_ster_fun                                  (P_val(val_timed_ster_fun                 ))/* 13 定时杀菌功能 */
#define val_ster_temp                                    (10*P_val(val_ster_temp                      ))/* 14 杀菌温度 */
#define val_ster_max_cycle                               (60*P_val(val_ster_max_cycle                 ))/* 15 杀菌最大周期 */
#define val_ster_high_temp_Time                          (60*P_val(val_ster_high_temp_Time            ))/* 16 杀菌高温时间 */
#define val_bak_00_11                                       (P_val(val_bak_00_11                      ))/* 备用 */
#define val_bak_00_12                                       (P_val(val_bak_00_12                      ))/* 备用 */
#define val_bak_00_13                                       (P_val(val_bak_00_13                      ))/* 备用 */
#define val_bak_00_14                                       (P_val(val_bak_00_14                      ))/* 备用 */
#define val_bak_00_15                                       (P_val(val_bak_00_15                      ))/* 备用 */
#define val_bak_00_16                                       (P_val(val_bak_00_16                      ))/* 备用 */
#define val_bak_00_17                                       (P_val(val_bak_00_17                      ))/* 备用 */
#define val_lock_screen                                     (P_val(val_lock_screen                    ))/* 锁屏功能设置 */
#define val_alarm_sound_set                                 (P_val(val_alarm_sound_set                ))/* 报警音设置 */
#define val_zone_A_cool_weather_curve                       (P_val(val_zone_A_cool_weather_curve      ))/* A区制冷曲线 */
#define val_zone_A_heat_weather_curve                       (P_val(val_zone_A_heat_weather_curve      ))/* A区制热曲线 */
#define val_zone_B_cool_weather_curve                       (P_val(val_zone_B_cool_weather_curve      ))/* B区制冷曲线 */
#define val_zone_B_heat_weather_curve                       (P_val(val_zone_B_heat_weather_curve      ))/* B区制热曲线 */
#define val_cool_curve_9_TA_1                            (10*P_val(val_cool_curve_9_TA_1              ))/* 曲线9制冷环温1 */
#define val_cool_curve_9_TA_2                            (10*P_val(val_cool_curve_9_TA_2              ))/* 曲线9制冷环温2 */
#define val_cool_curve_9_TWout2_1                        (10*P_val(val_cool_curve_9_TWout2_1          ))/* 曲线9制冷出温1 */
#define val_cool_curve_9_TWout2_2                        (10*P_val(val_cool_curve_9_TWout2_2          ))/* 曲线9制冷出温2 */
#define val_heat_curve_9_TA_1                            (10*P_val(val_heat_curve_9_TA_1              ))/* 曲线9制热环温1 */
#define val_heat_curve_9_TA_2                            (10*P_val(val_heat_curve_9_TA_2              ))/* 曲线9制热环温2 */
#define val_heat_curve_9_TWout2_1                        (10*P_val(val_heat_curve_9_TWout2_1          ))/* 曲线9制热出温1 */
#define val_heat_curve_9_TWout2_2                        (10*P_val(val_heat_curve_9_TWout2_2          ))/* 曲线9制热出温2 */
#define val_cell_enable_00                                  (P_val(val_cell_enable_00                 ))/* 00#模块设置 */	/* 使用指针访问 */
#define val_cell_enable_01                                  (P_val(val_cell_enable_01                 ))/* 01#模块设置 */
#define val_cell_enable_02                                  (P_val(val_cell_enable_02                 ))/* 02#模块设置 */
#define val_cell_enable_03                                  (P_val(val_cell_enable_03                 ))/* 03#模块设置 */
#define val_cell_enable_04                                  (P_val(val_cell_enable_04                 ))/* 04#模块设置 */
#define val_cell_enable_05                                  (P_val(val_cell_enable_05                 ))/* 05#模块设置 */
#define val_cell_enable_06                                  (P_val(val_cell_enable_06                 ))/* 06#模块设置 */
#define val_cell_enable_07                                  (P_val(val_cell_enable_07                 ))/* 07#模块设置 */

/* 获取A区选用环温-目标曲线ID */
#define GET_ZONE_A_WEATHER_CURVE(mode)  (((mode) == MODE_COOL) ? (val_zone_A_cool_weather_curve) \
                                                               : (val_zone_A_heat_weather_curve))

/* 获取B区选用环温-目标曲线ID */
#define GET_ZONE_B_WEATHER_CURVE(mode)  (((mode) == MODE_COOL) ? (val_zone_B_cool_weather_curve) \
                                                               : (val_zone_B_heat_weather_curve))

#define val_set_temp_curve              0
#define val_link_en                     0    // 制热二通联锁
#define val_duty_hot_water_set          40
#define val_duty_func                   0
#define val_duty_heat_set               30
#define val_duty_cool_set               17
#define val_duty_en_h                   20
#define val_duty_en_m                   0
#define val_duty_dis_h                  5
#define val_duty_dis_m                  0

/*02系统参数*/
#define SYS_ITEM_IDX_MAC_MODEL          3
#define P_val_air_type(f)                                   P_g_i(f, 1, 0)  /* 空调类型 */
#define P_val_refrigerant_type(f)                           P_g_i(f, 1, 1)	/* 冷媒类型 */
#define P_val_valve_type(f)                                 P_g_i(f, 1, 2)	/* 四通阀设置 */
#define P_val_mac_model(f)                                  P_g_i(f, 1, 3)	/* 机器型号 */
#define P_val_remote_switch_type(f)                         P_g_i(f, 1, 4)  /* 线控开关类型,0脉冲/1拔动 */
#define P_val_on_off_control_type_bak(f)                    P_g_i(f, 1, 5)  /* 机组启停控制 */
#define P_val_remember_run(f)                               P_g_i(f, 1, 6)	/* 掉电记忆开关机 */
#define P_val_auto_run(f)                                   P_g_i(f, 1, 7)  /* 来电自启动 */
#define P_val_pump_mode(f)                                  P_g_i(f, 1, 8)	/* 水泵运行模式 */
#define P_val_ctrl_object(f)                                P_g_i(f, 1, 9)	/* 控制对象 */
#define P_val_ctrl_cpst_cool(f)                             P_g_i(f, 1,10)  /* 制冷水温补偿 */
#define P_val_ctrl_cpst_heat(f)                             P_g_i(f, 1,11)  /* 制热水温补偿 */
#define P_val_valve_on_dly(f)                               P_g_i(f, 1,12)	/* 四通阀开延时(正值:提前开,负值:延时开) */
#define P_val_valve_off_dly(f)                              P_g_i(f, 1,13)	/* 四通阀关延时(正值:提前关,负值:延时关) */
#define P_val_open_v_exh(f)                                 P_g_i(f, 1,14)	/* 开喷液阀排温 */
#define P_val_close_v_exh_diff(f)                           P_g_i(f, 1,15)	/* 关喷液阀排温差 */
#define P_val_hotw_used(f)                                  P_g_i(f, 1,16)	/* 热水功能 */
#define P_val_hotw_diff(f)                                  P_g_i(f, 1,17)  /* 热水回差 */
#define P_val_open_val_temp(f)                              P_g_i(f, 1,18)	/* 开曲轴环温 */
#define P_val_close_val_temp(f)                             P_g_i(f, 1,19)	/* 关曲轴环温 */
#define P_val_cool_used(f)                                  P_g_i(f, 1,20)  /* 制冷模式 */
#define P_val_heat_used(f)                                  P_g_i(f, 1,21)  /* 制热模式 */
#define P_val_power_down_time_min(f)                        P_g_i(f, 1,22)	/* 最小预热掉电 */
#define P_val_power_down_time_max(f)                        P_g_i(f, 1,23)	/* 最大预热掉电 */
#define P_val_warm_time_min(f)                              P_g_i(f, 1,24)	/* 最小预热时间 */
#define P_val_warm_time_max(f)                              P_g_i(f, 1,25)	/* 最大预热时间 */
#define P_val_hotw_priority(f)                              P_g_i(f, 1,26)  /* 热水模式优先 */
#define P_val_TBH_used(f)                                   P_g_i(f, 1,27)	/* 水箱电热 */
#define P_val_use_PUMPret(f)                                P_g_i(f, 1,28)	/* 下回水泵 */
#define P_val_solar_used(f)                                 P_g_i(f, 1,29)	/* 太阳能 */
#define P_val_remote_onoff_type(f)                          P_g_i(f, 1,30)	/* 联动功能设置 */
#define P_val_inverter_used(f)                              P_g_i(f, 1,31)	/* 变频器使用：不能删除参数及相应代码，适配变频压机时禁用 */
#define P_val_sys_PUMPf_used(f)                             P_g_i(f, 1,32)	/* 47 系统变频水泵 */
#define P_val_RT_ctrl_type(f)                               P_g_i(f, 1,33)	/* 12 温控器控制 */
#define P_val_UI35_ctrl_type(f)                             P_g_i(f, 1,34)	/* 线控器控制 */
#define P_val_load_revise_range(f)                          P_g_i(f, 1,35)	/* 负载修正幅度 */
#define P_val_load_revise_time(f)                           P_g_i(f, 1,36)	/* 负载修正时间 */
#define P_val_chassis_heat_mode(f)                          P_g_i(f, 1,37)	/* 底盘电热模式 */
#define P_val_anti_ice_heat_env(f)                          P_g_i(f, 1,38)	/* 底盘电热环温 */
#define P_val_TCL_splash_offset(f)                          P_g_i(f, 1,39)	/* 启动页类型 */
#define P_val_smart_grid_cfg(f)                             P_g_i(f, 1,40)	/* 智能电网配置 */
#define P_val_smart_grid_used(f)                            P_g_i(f, 1,41)	/* 智能电网功能 */
#define P_val_cl_ht_adj_ratio(f)                            P_g_i(f, 1,42)	/* 冷热调整系数 */
#define P_val_PUMPf_interval_off_time(f)                    P_g_i(f, 1,43)	/* 水泵间歇停时长 */
#define val_air_type                                        (P_val(val_air_type                       ))/* 01 空调类型 */
#define val_refrigerant_type                                (P_val(val_refrigerant_type               ))/* 02 冷媒类型 */
#define val_valve_type                                      (P_val(val_valve_type                     ))/* 03 四通阀设置 */
#define val_mac_model                                       (P_val(val_mac_model                      ))/* 04 机器型号 */
#define val_remote_switch_type                              (P_val(val_remote_switch_type             ))/* 05 线控开关类型,0脉冲/1拔动 */
#define val_on_off_control_type_bak                         (P_val(val_on_off_control_type_bak        ))/* 06 机组启停控制 */
#define val_remember_run                                    (P_val(val_remember_run                   ))/* 07 掉电记忆开关机 */
#define val_auto_run                                        (P_val(val_auto_run                       ))/* 08 来电自启动 */
#define val_pump_mode                                       (P_val(val_pump_mode                      ))/* 09 水泵运行模式 */
#define val_ctrl_object                                     (P_val(val_ctrl_object                    ))/* 10 控制对象 */
#define val_ctrl_cpst_cool                                  (P_val(val_ctrl_cpst_cool                 ))/* 11 制冷水温补偿 */
#define val_ctrl_cpst_heat                                  (P_val(val_ctrl_cpst_heat                 ))/* 12 制热水温补偿 */
#define val_valve_on_dly                                    (P_val(val_valve_on_dly                   ))/* 13 四通阀开延时(正值:提前开,负值:延时开) */
#define val_valve_off_dly                                   (P_val(val_valve_off_dly                  ))/* 14 四通阀关延时(正值:提前关,负值:延时关) */
#define val_open_v_exh                                   (10*P_val(val_open_v_exh                     ))/* 15 开喷液阀排温 */
#define val_close_v_exh_diff                             (10*P_val(val_close_v_exh_diff               ))/* 16 关喷液阀排温差 */
#define val_hotw_used                                       (P_val(val_hotw_used                      ))/* 17 热水功能 */
#define val_hotw_diff                                       (P_val(val_hotw_diff                      ))/* 18 热水回差 */
#define val_open_val_temp                                (10*P_val(val_open_val_temp                  ))/* 19 开曲轴环温 */
#define val_close_val_temp                               (10*P_val(val_close_val_temp                 ))/* 20 关曲轴环温 */
#define val_cool_used                                       (P_val(val_cool_used                      ))/* 21 制冷模式 */
#define val_heat_used                                       (P_val(val_heat_used                      ))/* 22 制热模式 */
#define val_power_down_time_min                       (QK_60*P_val(val_power_down_time_min            ))/* 23 最小预热掉电 */
#define val_power_down_time_max                       (QK_60*P_val(val_power_down_time_max            ))/* 24 最大预热掉电 */
#define val_warm_time_min                                   (P_val(val_warm_time_min                  ))/* 25 最小预热时间 */
#define val_warm_time_max                                   (P_val(val_warm_time_max                  ))/* 26 最大预热时间 */
#define val_hotw_priority                                   (P_val(val_hotw_priority                  ))/* 27 热水模式优先 */
#define val_TBH_used                                        (P_val(val_TBH_used                       ))/* 28 水箱电热 */
#define val_use_PUMPret                                     (P_val(val_use_PUMPret                    ))/* 29 下回水泵 */
#define val_solar_used                                      (P_val(val_solar_used                     ))/* 30 太阳能 */
#define val_remote_onoff_type                               (P_val(val_remote_onoff_type              ))/* 31 联动功能设置 */
#define val_inverter_used                                   (P_val(val_inverter_used                  ))/* 32 变频器使用：不能删除参数及相应代码，适配变频压机时禁用 */
#define val_sys_PUMPf_used                                  (P_val(val_sys_PUMPf_used                 ))/* 33 系统变频水泵 */
#define val_RT_ctrl_type                                    (P_val(val_RT_ctrl_type                   ))/* 34 温控器控制 */
#define val_UI35_ctrl_type                                  (P_val(val_UI35_ctrl_type                 ))/* 35 线控器控制 */
#define val_load_revise_range                            (10*P_val(val_load_revise_range              ))/* 36 负载修正幅度 */
#define val_load_revise_time                                (P_val(val_load_revise_time               ))/* 37 负载修正时间 */
#define val_chassis_heat_mode                               (P_val(val_chassis_heat_mode              ))/* 38 底盘电热模式 */
#define val_anti_ice_heat_env                            (10*P_val(val_anti_ice_heat_env              ))/* 39 底盘电热环温 */
#define val_TCL_splash_offset                               (P_val(val_TCL_splash_offset              ))/* 40 启动页类型 */
#define val_smart_grid_cfg                                  (P_val(val_smart_grid_cfg                 ))/* 41 智能电网配置 */
#define val_smart_grid_used                                 (P_val(val_smart_grid_used                ))/* 42 智能电网功能 */
#define val_cl_ht_adj_ratio                                 (P_val(val_cl_ht_adj_ratio                ))/* 43 冷热调整系数 */
#define val_PUMPf_interval_off_time                   (QK_60*P_val(val_PUMPf_interval_off_time        ))/* 44 水泵间歇停时长 */

/* 2023-1-9补丁：先固定为“一直运行”，后续完善该功能后再定义到实际参数 */
// #define val_pump_mode PUMP_MODE_ON

/* 固定为“联合” */
#define val_on_off_control_type         (ONOFF_ALL)

/* 使用定频压机 */
#define USE_FIX_COMP                    (!val_inverter_used)
/* 热水模式优先 */
#define USE_HOTW_PRIORITY               (val_hotw_priority||fg_sg_hotw_priority)

/* 使用制冷模式 */
#define USE_COOL                        ((val_air_type==COOL_ONLY)||((val_air_type==REFR_HEAT)&&(val_cool_used==USED)))
/* 使用制热模式 */
#define USE_HEAT                        ((val_air_type==HEAT_ONLY)||((val_air_type==REFR_HEAT)&&(val_heat_used==USED)))

/* 仅制冷 */
#define USE_COOL_ONLY                   ((val_air_type==COOL_ONLY)||((val_air_type==REFR_HEAT)&&(val_cool_used==USED)&&(val_heat_used==UNUSED)))
/* 仅制热 */
#define USE_HEAT_ONLY                   ((val_air_type==HEAT_ONLY)||((val_air_type==REFR_HEAT)&&(val_heat_used==USED)&&(val_cool_used==UNUSED)))

#define val_machine_type                0   // 设备类型
#define val_warm_env                    (val_open_val_temp)     /* 预热环境温度 */
#define val_ac_heater_open (-250)

/* 线控器单区室温控制 */
#define USE_UI35_ONE_ZONE_ROOM_TEMP_CTRL    (!USE_UIRT_CTRL && (get_UI35_ctrl_type()==UI35_ZONE_A_TR))
/* 线控器单区水温控制 */
#define USE_UI35_ONE_ZONE_WATER_TEMP_CTRL   (!USE_UIRT_CTRL && (get_UI35_ctrl_type()==UI35_ZONE_A_TWout2))
/* 线控器双区水温控制 */
#define USE_UI35_DUAL_ZONE_WATER_TEMP_CTLR  (!USE_UIRT_CTRL && (get_UI35_ctrl_type()==UI35_DUAL_ZONE_TWout2))
/* 线控器双区混合控制 */
#define USE_UI35_DUAL_ZONE_HYBRID_CTRL      (!USE_UIRT_CTRL && (get_UI35_ctrl_type()==UI35_DUAL_ZONE_TWout2_TR))
/* 线控器双区控制 */
#define USE_UI35_DUAL_ZONE_CTRL             (USE_UI35_DUAL_ZONE_WATER_TEMP_CTLR || USE_UI35_DUAL_ZONE_HYBRID_CTRL)
/* 线控器室温控制 */
#define USE_UI35_ROOM_TEMP_CTRL             (USE_UI35_ONE_ZONE_ROOM_TEMP_CTRL || USE_UI35_DUAL_ZONE_HYBRID_CTRL)

/* 使用温控器控制 */
#define USE_UIRT_CTRL                       (val_RT_ctrl_type != UIRT_UNUSED)
/* 温控器A区模式切换 */
#define USE_UIRT_ZONE_A_MODE_SWITCH         (val_RT_ctrl_type == UIRT_ZONE_A_MODE_SWITCHING)
/* 温控器A区开关 */
#define USE_UIRT_ZONE_A_ON_OFF              (val_RT_ctrl_type == UIRT_ZONE_A_ON_OFF)
/* 温控器双区开关 */
#define USE_UIRT_DUAL_ZONE_ON_OFF           (val_RT_ctrl_type == UIRT_DUAL_ZONE_ON_OFF)

/* 双区控制 */
#define USE_DUAL_ZONE_CTRL                  (USE_UI35_DUAL_ZONE_CTRL || USE_UIRT_DUAL_ZONE_ON_OFF)

/* 单区室温能调 */
#define ONE_ZONE_ROOM_TEMP_CTRL             (USE_UI35_ONE_ZONE_ROOM_TEMP_CTRL)
/* 单区水温能调 */
#define ONE_ZONE_WATER_TEMP_CTRL            (USE_UIRT_ZONE_A_MODE_SWITCH||USE_UIRT_ZONE_A_ON_OFF||USE_UI35_ONE_ZONE_WATER_TEMP_CTRL)
/* 双区水温能调 */
#define DUAL_ZONE_WATER_TEMP_CTRL           (USE_UIRT_DUAL_ZONE_ON_OFF||USE_UI35_DUAL_ZONE_WATER_TEMP_CTLR)
/* 双区混合能调 */
#define DUAL_ZONE_HYBRID_CTRL               (USE_UI35_DUAL_ZONE_HYBRID_CTRL)

/* 智能电网配置 */
enum
{
    SG_CFG_REGULAR,         /* 常规: SG-Ready标准 */
    SG_CFG_NO_STA1,         /* 配置1: 仅无智能电网状态1 */
    SG_CFG_MAX,
};

/*03变频压机设置*/

#define IDX_INVT_MODEL          0
#define IDX_INVT_MIN            5
#define IDX_INVT_MAX            6
#define IDX_INVT_START          18
#define IDX_INVT_CUR_LMT        34
#define IDX_INVT_LOAD_TIME      35
#define IDX_INVT_BLOCKAGE_TIME  36

#define P_val_invt_motor_type(f)                            P_g_i(f, 2, 0)	/* 变频压机编码 */
#define P_val_invt_manual_mode(f)                           P_g_i(f, 2, 1)	/* 手动模式 */
#define P_val_invt_manual_rps(f)                            P_g_i(f, 2, 2)	/* 手动设定转速 */
#define P_val_cm_off_time(f)                                P_g_i(f, 2, 3)	/* 防频繁启动 */
#define P_val_invt_freq_pre_sub(f)                          P_g_i(f, 2, 4)	/* 到温预降频 */
#define P_val_invt_rps_min(f)                               P_g_i(f, 2, 5)	/* 驱动转速下限(0.01rps) */
#define P_val_invt_rps_max(f)                               P_g_i(f, 2, 6)	/* 驱动转速上限(0.01rps) */
#define P_val_invt_rps_cool(f)                              P_g_i(f, 2, 7)	/* 制冷额定转速(0.01rps) */
#define P_val_invt_rps_heat(f)                              P_g_i(f, 2, 8)	/* 制热额定转速(0.01rps) */
#define P_val_freq_stably_ctrl_time(f)                      P_g_i(f, 2, 9)	/* 启动维持时间（秒） */
#define P_val_hp_forbid_add_prs_diff(f)                     P_g_i(f, 2,10)	/* 高压限加偏差 */
#define P_val_lp_forbid_add_prs_diff(f)                     P_g_i(f, 2,11)	/* 低压限加偏差 */
#define P_val_hp_force_sub_prs_diff(f)                      P_g_i(f, 2,12)	/* 高压强卸偏差 */
#define P_val_lp_force_sub_prs_diff(f)                      P_g_i(f, 2,13)	/* 低压强卸偏差 */
#define P_val_reoil_period(f)                               P_g_i(f, 2,14)	/* 低频运行最长时间 */
#define P_val_reoil_time(f)                                 P_g_i(f, 2,15)	/* 强制回油时间 */
#define P_val_invt_addr(f)                                  P_g_i(f, 2,16)	/* 变频驱动地址 */
#define P_val_invt_rps_defrost(f)                           P_g_i(f, 2,17)	/* 压机除霜转速 */
#define P_val_invt_start_freq(f)                            P_g_i(f, 2,18)	/* 压机启动转速 */
#define P_val_invt_start_speed(f)                           P_g_i(f, 2,19)	/* 启动升频速率 */
#define P_val_invt_run_speed(f)                             P_g_i(f, 2,20)	/* 运行升频速率 */
#define P_val_invt_defr_speed(f)                            P_g_i(f, 2,21)	/* 除霜升频速率 */
#define P_val_invt_rps_adj_std(f)                           P_g_i(f, 2,22)	/* 标准频率调整 */
#define P_val_invt_start_period(f)                          P_g_i(f, 2,23)	/* 启动升频间隔 */
#define P_val_f_pid_p(f)                                    P_g_i(f, 2,24)	/* 调频比例带 单位0.1 */
#define P_val_f_pid_i(f)                                    P_g_i(f, 2,25)	/* 调频积分时间 */
#define P_val_f_pid_d(f)                                    P_g_i(f, 2,26)	/* 调频微分时间 */
#define P_val_f_pid_period(f)                               P_g_i(f, 2,27)	/* 调频动作周期 */
#define P_val_invt_start_freq2(f)                           P_g_i(f, 2,28)	/* 压机启动维持转速 */
#define P_val_f_pid_Kp(f)                                   P_g_i(f, 2,29)	/* 调频比例系数 */
#define P_val_cool_cond_temphigh(f)                         P_g_i(f, 2,30)	/* 制冷冷凝上限 */
#define P_val_heat_cond_temphigh(f)                         P_g_i(f, 2,31)	/* 制热冷凝上限 */
#define P_val_invt_rps_adj_eco(f)                           P_g_i(f, 2,32)	/* 节能频率调整 */
#define P_val_heat_evap_templow(f)                          P_g_i(f, 2,33)	/* 制热蒸发下限 */
#define P_val_freq_rated_time(f)                            P_g_i(f, 2,34)	/* 额定维持时间 */
#define P_val_cool_reoil_env(f)                             P_g_i(f, 2,35)	/* 制冷回油环温 */
#define P_val_heat_reoil_env(f)                             P_g_i(f, 2,36)	/* 制热回油环温 */
#define P_val_force_ctrl_gas_out(f)                         P_g_i(f, 2,37)	/* 强制降频排温 */
#define P_val_reoil_inventrps(f)                            P_g_i(f, 2,38)	/* 回油运行频率 */
#define P_val_invt_rps_str_silent_max(f)                    P_g_i(f, 2,39)	/* 超静音最大转速 */
#define P_val_invt_rps_adj_strong(f)                        P_g_i(f, 2,40)	/* 强劲频率调整 */
#define P_val_invt_rps_silent_max(f)                        P_g_i(f, 2,41)	/* 静音最大转速 */
#define P_val_avoid_freq_range(f)                           P_g_i(f, 2,42)	/* 回避频率范围 */
#define P_val_avoid_freq1(f)                                P_g_i(f, 2,43)	/* 回避频率1 */
#define P_val_avoid_freq2(f)                                P_g_i(f, 2,44)	/* 回避频率2 */
#define P_val_avoid_freq3(f)                                P_g_i(f, 2,45)	/* 回避频率3 */
#define P_val_avoid_freq4(f)                                P_g_i(f, 2,46)	/* 回避频率4 */
#define P_val_low_env_start_freq(f)                         P_g_i(f, 2,47)	/* 低环启动维持转速 */
#define val_invt_motor_type                                 (P_val(val_invt_motor_type                ))/* 01 变频压机编码 */
#define val_invt_manual_mode                                (P_val(val_invt_manual_mode               ))/* 02 手动模式 */
#define val_invt_manual_rps                             (100*P_val(val_invt_manual_rps                ))/* 03 手动设定转速 */
#define val_cm_off_time                                     (P_val(val_cm_off_time                    ))/* 04 防频繁启动 */
#define val_invt_freq_pre_sub                           (100*P_val(val_invt_freq_pre_sub              ))/* 05 到温预降频 */
#define val_invt_rps_min                                (100*P_val(val_invt_rps_min                   ))/* 06 驱动转速下限(0.01rps) */
#define val_invt_rps_max                                (100*P_val(val_invt_rps_max                   ))/* 07 驱动转速上限(0.01rps) */
#define val_invt_rps_cool                               (100*P_val(val_invt_rps_cool                  ))/* 08 制冷额定转速(0.01rps) */
#define val_invt_rps_heat                               (100*P_val(val_invt_rps_heat                  ))/* 09 制热额定转速(0.01rps) */
#define val_freq_stably_ctrl_time                           (P_val(val_freq_stably_ctrl_time          ))/* 10 启动维持时间（秒） */
#define val_hp_forbid_add_prs_diff                          (P_val(val_hp_forbid_add_prs_diff         ))/* 11 高压限加偏差 */
#define val_lp_forbid_add_prs_diff                          (P_val(val_lp_forbid_add_prs_diff         ))/* 12 低压限加偏差 */
#define val_hp_force_sub_prs_diff                           (P_val(val_hp_force_sub_prs_diff          ))/* 13 高压强卸偏差 */
#define val_lp_force_sub_prs_diff                           (P_val(val_lp_force_sub_prs_diff          ))/* 14 低压强卸偏差 */
#define val_reoil_period                                 (60*P_val(val_reoil_period                   ))/* 15 低频运行最长时间 */
#define val_reoil_time                                      (P_val(val_reoil_time                     ))/* 16 强制回油时间 */
#define val_invt_addr                                       (P_val(val_invt_addr                      ))/* 17 变频驱动地址 */
#define val_invt_rps_defrost                            (100*P_val(val_invt_rps_defrost               ))/* 18 压机除霜转速 */
#define val_invt_start_freq                             (100*P_val(val_invt_start_freq                ))/* 19 压机启动转速 */
#define val_invt_start_speed                            (100*P_val(val_invt_start_speed               ))/* 20 启动升频速率 */
#define val_invt_run_speed                              (100*P_val(val_invt_run_speed                 ))/* 21 运行升频速率 */
#define val_invt_defr_speed                             (100*P_val(val_invt_defr_speed                ))/* 22 除霜升频速率 */
#define val_invt_rps_adj_std                            (100*P_val(val_invt_rps_adj_std               ))/* 23 标准频率调整 */
#define val_invt_start_period                               (P_val(val_invt_start_period              ))/* 24 启动升频间隔 */
#define val_f_pid_p                                         (P_val(val_f_pid_p                        ))/* 25 调频比例带 单位0.1 */
#define val_f_pid_i                                         (P_val(val_f_pid_i                        ))/* 26 调频积分时间 */
#define val_f_pid_d                                         (P_val(val_f_pid_d                        ))/* 27 调频微分时间 */
#define val_f_pid_period                                    (P_val(val_f_pid_period                   ))/* 28 调频动作周期 */
#define val_invt_start_freq2                            (100*P_val(val_invt_start_freq2               ))/* 29 压机启动维持转速 */
#define val_f_pid_Kp                                        (P_val(val_f_pid_Kp                       ))/* 30 调频比例系数 */
#define val_cool_cond_temphigh                           (10*P_val(val_cool_cond_temphigh             ))/* 31 制冷冷凝上限 */
#define val_heat_cond_temphigh                           (10*P_val(val_heat_cond_temphigh             ))/* 32 制热冷凝上限 */
#define val_invt_rps_adj_eco                            (100*P_val(val_invt_rps_adj_eco               ))/* 33 节能频率调整 */
#define val_heat_evap_templow                            (10*P_val(val_heat_evap_templow              ))/* 34 制热蒸发下限 */
#define val_freq_rated_time                                 (P_val(val_freq_rated_time                ))/* 35 额定维持时间 */
#define val_cool_reoil_env                                  (P_val(val_cool_reoil_env                 ))/* 36 制冷回油环温 */
#define val_heat_reoil_env                                  (P_val(val_heat_reoil_env                 ))/* 37 制热回油环温 */
#define val_force_ctrl_gas_out                           (10*P_val(val_force_ctrl_gas_out             ))/* 38 强制降频排温 */
#define val_reoil_inventrps                             (100*P_val(val_reoil_inventrps                ))/* 39 回油运行频率 */
#define val_invt_rps_str_silent_max                     (100*P_val(val_invt_rps_str_silent_max        ))/* 40 超静音最大转速 */
#define val_invt_rps_adj_strong                         (100*P_val(val_invt_rps_adj_strong            ))/* 41 强劲频率调整 */
#define val_invt_rps_silent_max                         (100*P_val(val_invt_rps_silent_max            ))/* 42 静音最大转速 */
#define val_avoid_freq_range                            (100*P_val(val_avoid_freq_range               ))/* 43 回避频率范围 */
#define val_avoid_freq1                                 (100*P_val(val_avoid_freq1                    ))/* 44 回避频率1 */
#define val_avoid_freq2                                 (100*P_val(val_avoid_freq2                    ))/* 45 回避频率2 */
#define val_avoid_freq3                                 (100*P_val(val_avoid_freq3                    ))/* 46 回避频率3 */
#define val_avoid_freq4                                 (100*P_val(val_avoid_freq4                    ))/* 47 回避频率4 */
#define val_low_env_start_freq                          (100*P_val(val_low_env_start_freq             ))/* 48 低环启动维持转速 */

#define val_avoid_freq(n)           (100*para_C[GROUP_COMP][43+n])// 回避频率

#define val_f_pid_calculate_period      8	/* 调频计算周期 */
#define val_off_speed_down_time         30 // 停机降频时间
#define CM_off_speed_down_time  (val_inverter_used? val_off_speed_down_time: 0) // 压机停机降频时间

#if defined(M_DISEN)
#define GAS_OUT_HIGH_DIFF1  (pv_cell_TA(pcb_addr) <= -70 ?\
                             0 : pv_cell_TA(pcb_addr) <= 70 ?\
                                 50 : 100)
#else
#define GAS_OUT_HIGH_DIFF1  50
#endif
#define GAS_OUT_HIGH_DIFF2  (GAS_OUT_HIGH_DIFF1+30)

#define val_f_pid_flexi             (10)    // 积分系数
#define val_f_pid_fg                (1)     // 是否使用PID变频控制
//#define val_energy_ctrl_time      ( 60*para_C[GROUP_COMP][8])       // 8开机调节延时（分）

/* 04风机参数设置 */
#define FAN_FREQ_MAX 2000
#define P_val_freq_fan_set(f)                               P_g_i(f, 3, 0)	/* 变频风机设置 */
#define P_val_invt_fan_type(f)                              P_g_i(f, 3, 1)	/* EC风机编码 */
#define P_val_freq_fan_cool_max(f)                          P_g_i(f, 3, 2)	/* 制冷风机最高 */
#define P_val_freq_fan_cool_min(f)                          P_g_i(f, 3, 3)	/* 制冷风机最低 */
#define P_val_freq_fan_eco_adj(f)                           P_g_i(f, 3, 4)	/* 节能风速调整 */
#define P_val_freq_fan_ctrl(f)                              P_g_i(f, 3, 5)	/* 变频风机调速 */
#define P_val_freq_fan_hand_speed(f)                        P_g_i(f, 3, 6)	/* 风机手动运行 */
#define P_val_ivnt_prot_temp(f)                             P_g_i(f, 3, 7)	/* 高风模块温度 */
#define P_val_hig_fan_env_cold(f)                           P_g_i(f, 3, 8)	/* 制冷高风环温 */
#define P_val_low_fan_env_cold(f)                           P_g_i(f, 3, 9)	/* 制冷低风环温 */
#define P_val_hig_fan_env_heat(f)                           P_g_i(f, 3,10)	/* 制热高风环温 */
#define P_val_low_fan_env_heat(f)                           P_g_i(f, 3,11)	/* 制热低风环温 */
#define P_val_hig_fan_wing_cold(f)                          P_g_i(f, 3,12)	/* 制冷高风冷凝 */
#define P_val_low_fan_wing_cold(f)                          P_g_i(f, 3,13)	/* 制冷低风冷凝 */
#define P_val_hig_fan_wing_heat(f)                          P_g_i(f, 3,14)	/* 制热高风蒸发 */
#define P_val_low_fan_wing_heat(f)                          P_g_i(f, 3,15)	/* 制热低风蒸发 */
#define P_val_mid_fan_temp_heat(f)                          P_g_i(f, 3,16)	/* 制热中风温度 */
#define P_val_fan_speed_period(f)                           P_g_i(f, 3,17)	/* 风速调节周期 */
#define P_val_fan_speed_percent(f)                          P_g_i(f, 3,18)	/* 风速动作比例 */
#define P_val_freq_fan_heat_max(f)                          P_g_i(f, 3,19)	/* 制热风机最高 */
#define P_val_freq_fan_heat_min(f)                          P_g_i(f, 3,20)	/* 制热风机最低 */
#define P_val_fan_pwm_pulse(f)                              P_g_i(f, 3,21)	/* PWM风机脉冲  */
#define P_val_freq_fan_strong_adj(f)                        P_g_i(f, 3,22)	/* 强劲风速调整 */
#define P_val_ec_fan_base_addr(f)                           P_g_i(f, 3,23)	/* EC风机基址 */
#define P_val_mid_fan_temp_cold(f)                          P_g_i(f, 3,24)	/* 制冷中风温度 */
#define P_val_freq_fan_str_silent_max(f)                    P_g_i(f, 3,25)	/* 超静音最大风速 */
#define P_val_freq_fan_num(f)                               P_g_i(f, 3,26)	/* 变频风机个数 */
#define P_val_freq_fan_silent_max(f)                        P_g_i(f, 3,27)	/* 静音最大风速 */
#define P_val_pwm_fan_init(f)                               P_g_i(f, 3,28)	/* pwm调速初始值 */
#define P_val_pwm_fan_coef(f)                               P_g_i(f, 3,29)	/* pwm调速系数 */
#define P_val_pwm_fan_period(f)                             P_g_i(f, 3,30)	/* pwm调速间隔 */
#define P_val_pwm_fan_area(f)                               P_g_i(f, 3,31)	/* pwm调速死区 */
#define P_val_freq_fan_std_adj(f)                           P_g_i(f, 3,32)	/* 标准风速调整 */
#define P_val_avoid_cold(f)                                 P_g_i(f, 3,33)	/* 防冷风功能 */
#define P_val_send_fan_pause(f)                             P_g_i(f, 3,34)	/* 送风机暂停 */
#define P_val_avoid_cold_fan_time(f)                        P_g_i(f, 3,35)	/* 防冷风时间 */
#define P_val_freq_Cpst_Coef(f)                             P_g_i(f, 3,36)	/* 频率补偿系数 */
#define P_val_min_Cpst_Coef(f)                              P_g_i(f, 3,37)	/* 修正系数下限 */
#define P_val_Tp_fan_rpm_corr_1(f)                          P_g_i(f, 3,38)	/* 排温风速修正1 */
#define P_val_Tp_fan_rpm_corr_2(f)                          P_g_i(f, 3,39)	/* 排温风速修正2 */
#define P_val_Tp_fan_rpm_corr_3(f)                          P_g_i(f, 3,40)	/* 排温风速修正3 */
#define P_val_Tp_fan_rpm_corr_set(f)                        P_g_i(f, 3,41)	/* 排温修正设置 */
#define P_val_close_fan_set_cl(f)                           P_g_i(f, 3,42)	/* 制冷停风温差 */
#define P_val_close_fan_set_ht(f)                           P_g_i(f, 3,43)	/* 制热停风温差 */
#define P_val_close_fan_D(f)                                P_g_i(f, 3,44)	/* 停风回差 */
#define P_val_freq_fan_cool_mid(f)                          P_g_i(f, 3,45)	/* 制冷风机中速 */
#define P_val_freq_fan_heat_mid(f)                          P_g_i(f, 3,46)	/* 制热风机中速 */
#define val_freq_fan_set                                    (P_val(val_freq_fan_set                   ))/* 变频风机设置 */
#define val_invt_fan_type                                   (P_val(val_invt_fan_type                  ))/* EC风机编码 */
#define val_freq_fan_cool_max                               (P_val(val_freq_fan_cool_max              ))/* 制冷风机最高 */
#define val_freq_fan_cool_min                               (P_val(val_freq_fan_cool_min              ))/* 制冷风机最低 */
#define val_freq_fan_eco_adj                                (P_val(val_freq_fan_eco_adj               ))/* 节能风速调整 */
#define val_freq_fan_ctrl                                   (P_val(val_freq_fan_ctrl                  ))/* 变频风机调速 */
#define val_freq_fan_hand_speed                             (P_val(val_freq_fan_hand_speed            ))/* 风机手动运行 */
#define val_ivnt_prot_temp                               (10*P_val(val_ivnt_prot_temp                 ))/* 高风模块温度 */
#define val_hig_fan_env_cold                             (10*P_val(val_hig_fan_env_cold               ))/* 制冷高风环温 */
#define val_low_fan_env_cold                             (10*P_val(val_low_fan_env_cold               ))/* 制冷低风环温 */
#define val_hig_fan_env_heat                             (10*P_val(val_hig_fan_env_heat               ))/* 制热高风环温 */
#define val_low_fan_env_heat                             (10*P_val(val_low_fan_env_heat               ))/* 制热低风环温 */
#define val_hig_fan_wing_cold                            (10*P_val(val_hig_fan_wing_cold              ))/* 制冷高风冷凝 */
#define val_low_fan_wing_cold                            (10*P_val(val_low_fan_wing_cold              ))/* 制冷低风冷凝 */
#define val_hig_fan_wing_heat                            (10*P_val(val_hig_fan_wing_heat              ))/* 制热高风蒸发 */
#define val_low_fan_wing_heat                            (10*P_val(val_low_fan_wing_heat              ))/* 制热低风蒸发 */
#define val_mid_fan_temp_heat                            (10*P_val(val_mid_fan_temp_heat              ))/* 制热中风温度 */
#define val_fan_speed_period                                (P_val(val_fan_speed_period               ))/* 风速调节周期 */
#define val_fan_speed_percent                               (P_val(val_fan_speed_percent              ))/* 风速动作比例 */
#define val_freq_fan_heat_max                               (P_val(val_freq_fan_heat_max              ))/* 制热风机最高 */
#define val_freq_fan_heat_min                               (P_val(val_freq_fan_heat_min              ))/* 制热风机最低 */
#define val_fan_pwm_pulse                                   (P_val(val_fan_pwm_pulse                  ))/* PWM风机脉冲  */
#define val_freq_fan_strong_adj                             (P_val(val_freq_fan_strong_adj            ))/* 强劲风速调整 */
#define val_ec_fan_base_addr                                (P_val(val_ec_fan_base_addr               ))/* EC风机基址 */
#define val_mid_fan_temp_cold                            (10*P_val(val_mid_fan_temp_cold              ))/* 制冷中风温度 */
#define val_freq_fan_str_silent_max                         (P_val(val_freq_fan_str_silent_max        ))/* 超静音最大风速 */
#define val_freq_fan_num                                    (P_val(val_freq_fan_num                   ))/* 变频风机个数 */
#define val_freq_fan_silent_max                             (P_val(val_freq_fan_silent_max            ))/* 静音最大风速 */
#define val_pwm_fan_init                                    (P_val(val_pwm_fan_init                   ))/* pwm调速初始值 */
#define val_pwm_fan_coef                                    (P_val(val_pwm_fan_coef                   ))/* pwm调速系数 */
#define val_pwm_fan_period                                  (P_val(val_pwm_fan_period                 ))/* pwm调速间隔 */
#define val_pwm_fan_area                                    (P_val(val_pwm_fan_area                   ))/* pwm调速死区 */
#define val_freq_fan_std_adj                                (P_val(val_freq_fan_std_adj               ))/* 标准风速调整 */
#define val_avoid_cold                                      (P_val(val_avoid_cold                     ))/* 防冷风功能 */
#define val_send_fan_pause                                  (P_val(val_send_fan_pause                 ))/* 送风机暂停 */
#define val_avoid_cold_fan_time                             (P_val(val_avoid_cold_fan_time            ))/* 防冷风时间 */
#define val_freq_Cpst_Coef                                  (P_val(val_freq_Cpst_Coef                 ))/* 频率补偿系数 */
#define val_min_Cpst_Coef                                   (P_val(val_min_Cpst_Coef                  ))/* 修正系数下限 */
#define val_Tp_fan_rpm_corr_1                               (P_val(val_Tp_fan_rpm_corr_1              ))/* 排温风速修正1 */
#define val_Tp_fan_rpm_corr_2                               (P_val(val_Tp_fan_rpm_corr_2              ))/* 排温风速修正2 */
#define val_Tp_fan_rpm_corr_3                               (P_val(val_Tp_fan_rpm_corr_3              ))/* 排温风速修正3 */
#define val_Tp_fan_rpm_corr_set                             (P_val(val_Tp_fan_rpm_corr_set            ))/* 排温修正设置 */
#define val_close_fan_set_cl                                (P_val(val_close_fan_set_cl               ))/* 制冷停风温差 */
#define val_close_fan_set_ht                                (P_val(val_close_fan_set_ht               ))/* 制热停风温差 */
#define val_close_fan_D                                  (10*P_val(val_close_fan_D                    ))/* 停风回差 */
#define val_freq_fan_cool_mid                               (P_val(val_freq_fan_cool_mid              ))/* 制冷风机中速 */
#define val_freq_fan_heat_mid                               (P_val(val_freq_fan_heat_mid              ))/* 制热风机中速 */

#define val_close_fan_diff_cl                             (10*(val_close_fan_set_cl%100))/* 制冷停风温差 */
#define val_close_fan_diff_ht                             (10*(val_close_fan_set_ht%100))/* 制热停风温差 */

#define val_close_fan_dly_cl                              (val_close_fan_set_cl/100)/* 制冷停风延时 */
#define val_close_fan_dly_ht                              (val_close_fan_set_ht/100)/* 制热停风延时 */

#define val_Tp_fan_rpm_corr(i)                              (para_C[GROUP_FAN][38+i])

#define val_fan_speed_type                                  (FAN_SPEED_UNUSED)/* 定频风机设置 */

/*05电子膨胀阀设置 */
#define P_val_motor_use(f)                                  P_g_i(f, 4,0)	/* 主阀使用设置 */
#define P_val_motor_fre_type(f)                             P_g_i(f, 4,1)	/* 主阀励磁方式 */
#define P_val_motor_step_fre(f)                             P_g_i(f, 4,2)	/* 主阀励磁频率 */
#define P_val_motor_step_max(f)                             P_g_i(f, 4,3)	/* 主阀总步数 */
#define P_val_motor_force_step(f)                           P_g_i(f, 4,4)	/* 主强动作比例 */
#define P_val_motor_max_od(f)                               P_g_i(f, 4,5)	/* 主阀最大开度 */
#define P_val_motor_sv_exh_cpst(f)                          P_g_i(f, 4,6)	/* 主阀过热排温补 */
#define P_val_motor_manual_mode(f)                          P_g_i(f, 4,7)	/* 主阀手动模式 */
#define P_val_motor_manual_steps(f)                         P_g_i(f, 4,8)	/* 主阀手动步数 */
#define P_val_motor_sv_freq_cpst(f)                         P_g_i(f, 4,9)	/* 主阀过热频率补 */
#define P_val_s_motor_use(f)                                P_g_i(f, 4,10)	/* 辅阀使用设置 */
#define P_val_s_motor_fre_type(f)                           P_g_i(f, 4,11)	/* 辅阀励磁方式 */
#define P_val_s_motor_step_fre(f)                           P_g_i(f, 4,12)	/* 辅阀励磁频率 */
#define P_val_s_motor_step_max(f)                           P_g_i(f, 4,13)	/* 辅阀总步数 */
#define P_val_s_motor_force_step(f)                         P_g_i(f, 4,14)	/* 辅强动作比例 */
#define P_val_s_motor_max_od(f)                             P_g_i(f, 4,15)	/* 辅阀最大开度 */
#define P_val_s_motor_min_od(f)                             P_g_i(f, 4,16)	/* 辅阀最小开度 */
#define P_val_s_motor_manual_mode(f)                        P_g_i(f, 4,17)	/* 辅阀手动模式 */
#define P_val_s_motor_manual_steps(f)                       P_g_i(f, 4,18)	/* 辅阀手动步数 */
#define P_val_press_diff_in(f)                              P_g_i(f, 4,19)	/* 低环高水回温 */
#define P_val_press_diff_env(f)                             P_g_i(f, 4,20)	/* 低环高水环温 */
#define P_val_press_diff_eev(f)                             P_g_i(f, 4,21)	/* 低环高水主阀 */
#define P_val_eev_step_refer(f)                             P_g_i(f, 4,22)	/* 主阀参考开度 */
#define P_val_eev_refer_coef(f)                             P_g_i(f, 4,23)	/* 参考开度系数 */
#define P_val_cool_sv_chg_env(f)                            P_g_i(f, 4,24)	/* 制冷过热转换环 */
#define P_val_low_env_sv_cool1(f)                           P_g_i(f, 4,25)	/* 低环制冷过热1 */
#define P_val_low_env_sv_cool2(f)                           P_g_i(f, 4,26)	/* 低环制冷过热2 */
#define P_val_low_env_sv_cool3(f)                           P_g_i(f, 4,27)	/* 低环制冷过热3 */
#define P_val_eev_invt_rps_diff(f)                          P_g_i(f, 4,28)  /* 主阀变频增量 */
#define P_val_eev_invt_coef_up(f)                           P_g_i(f, 4,29)  /* 主阀升频系数 */
#define P_val_eev_invt_coef_dn(f)                           P_g_i(f, 4,30)  /* 主阀降频系数 */
#define P_val_init_step_chg_env_cl(f)                       P_g_i(f, 4,31)	/* 制冷初开趋势环 */
#define P_val_motor_sv_heat10(f)                            P_g_i(f, 4,32)  /* 制热吸气过热10 */
#define P_val_high_gas_eev_evi_order(f)                     P_g_i(f, 4,33)  /* 高排温主辅次序 */
#define val_motor_use                                       (P_val(val_motor_use                      ))/* 主阀使用设置 */
#define val_motor_fre_type                                  (P_val(val_motor_fre_type                 ))/* 主阀励磁方式 */
#define val_motor_step_fre                                  (P_val(val_motor_step_fre                 ))/* 主阀励磁频率 */
#define val_motor_step_max                                  (P_val(val_motor_step_max                 ))/* 主阀总步数 */
#define val_motor_force_step                                (P_val(val_motor_force_step               ))/* 主强动作比例 */
#define val_motor_max_od                                    (P_val(val_motor_max_od                   ))/* 主阀最大开度 */
#define val_motor_sv_exh_cpst                               (P_val(val_motor_sv_exh_cpst              ))/* 主阀过热排温补 */
#define val_motor_manual_mode                               (P_val(val_motor_manual_mode              ))/* 主阀手动模式 */
#define val_motor_manual_steps                              (P_val(val_motor_manual_steps             ))/* 主阀手动步数 */
#define val_motor_sv_freq_cpst                              (P_val(val_motor_sv_freq_cpst             ))/* 主阀过热频率补 */
#define val_s_motor_use                                     (P_val(val_s_motor_use                    ))/* 辅阀使用设置 */
#define val_s_motor_fre_type                                (P_val(val_s_motor_fre_type               ))/* 辅阀励磁方式 */
#define val_s_motor_step_fre                                (P_val(val_s_motor_step_fre               ))/* 辅阀励磁频率 */
#define val_s_motor_step_max                                (P_val(val_s_motor_step_max               ))/* 辅阀总步数 */
#define val_s_motor_force_step                              (P_val(val_s_motor_force_step             ))/* 辅强动作比例 */
#define val_s_motor_max_od                                  (P_val(val_s_motor_max_od                 ))/* 辅阀最大开度 */
#define val_s_motor_min_od                                  (P_val(val_s_motor_min_od                 ))/* 辅阀最小开度 */
#define val_s_motor_manual_mode                             (P_val(val_s_motor_manual_mode            ))/* 辅阀手动模式 */
#define val_s_motor_manual_steps                            (P_val(val_s_motor_manual_steps           ))/* 辅阀手动步数 */
#define val_press_diff_in                                (10*P_val(val_press_diff_in                  ))/* 低环高水回温 */
#define val_press_diff_env                               (10*P_val(val_press_diff_env                 ))/* 低环高水环温 */
#define val_press_diff_eev                                  (P_val(val_press_diff_eev                 ))/* 低环高水主阀 */
#define val_eev_step_refer                                  (P_val(val_eev_step_refer                 ))/* 主阀参考开度 */
#define val_eev_refer_coef                                  (P_val(val_eev_refer_coef                 ))/* 参考开度系数 */
#define val_cool_sv_chg_env                              (10*P_val(val_cool_sv_chg_env                ))/* 制冷过热转换环 */
#define val_low_env_sv_cool1                                (P_val(val_low_env_sv_cool1               ))/* 低环制冷过热1 */
#define val_low_env_sv_cool2                                (P_val(val_low_env_sv_cool2               ))/* 低环制冷过热2 */
#define val_low_env_sv_cool3                                (P_val(val_low_env_sv_cool3               ))/* 低环制冷过热3 */
#define val_eev_invt_rps_diff                               (P_val(val_eev_invt_rps_diff              ))/* 主阀变频增量 */
#define val_eev_invt_coef_up                                (P_val(val_eev_invt_coef_up               ))/* 主阀升频系数 */
#define val_eev_invt_coef_dn                                (P_val(val_eev_invt_coef_dn               ))/* 主阀降频系数 */
#define val_init_step_chg_env_cl                         (10*P_val(val_init_step_chg_env_cl           ))/* 制冷初开趋势环 */
#define val_motor_sv_heat10                                 (P_val(val_motor_sv_heat10                ))/* 制热吸气过热10 */
#define val_high_gas_eev_evi_order                          (P_val(val_high_gas_eev_evi_order         ))/* 高排温主辅次序 */

#define val_pid_d_type              (1)         // 1 调试参数6是否使用微分项低通滤波0:不是使用 1:使用
#define val_ave_stand               (6)         // 调试参数7  4

#define EVI_RUN_STEP_MAX ((U32)val_s_motor_max_od*val_s_motor_step_max/100) // EVI最大运行步数
#define EVI_RUN_STEP_MIN ((U32)val_s_motor_min_od*val_s_motor_step_max/1000)// EVI最小运行步数

/* 06主路电子膨胀阀控制 */
#define P_val_motor_idle_open(f)                            P_g_i(f, 5, 0)	/* 待机开度 */
#define P_val_m_motor_start_time(f)                         P_g_i(f, 5, 1)	/* 初开维持时间 */
#define P_val_motor_start_amplify_cool(f)                   P_g_i(f, 5, 2)	/* 制冷初开放大 */
#define P_val_motor_start_amplify_heat1(f)                  P_g_i(f, 5, 3)	/* 制热初开放大1 */
#define P_val_motor_start_amplify_heat2(f)                  P_g_i(f, 5, 4)	/* 制热初开放大2 */
#define P_val_motor_start_amplify_heat3(f)                  P_g_i(f, 5, 5)	/* 制热初开放大3 */
#define P_val_motor_start_change_heat1(f)                   P_g_i(f, 5, 6)	/* 制热初开转换1：回温 */
#define P_val_motor_start_change_heat2(f)                   P_g_i(f, 5, 7)	/* 制热初开转换2 */
#define P_val_cool_min_initial_od(f)                        P_g_i(f, 5, 8)	/* 制冷初开下限 */
#define P_val_cool_max_initial_od(f)                        P_g_i(f, 5, 9)	/* 制冷初开上限 */
#define P_val_heat_min_initial_od(f)                        P_g_i(f, 5,10)	/* 制热初开下限 */
#define P_val_heat_max_initial_od(f)                        P_g_i(f, 5,11)	/* 制热初开上限 */
#define P_val_motor_start_amp_EVI_off(f)                    P_g_i(f, 5,12)	/* 关辅初开放大 */
#define P_val_motor_sv_cool1(f)                             P_g_i(f, 5,13)	/* 制冷吸气过热1 */
#define P_val_motor_sv_cool2(f)                             P_g_i(f, 5,14)	/* 制冷吸气过热2 */
#define P_val_motor_sv_cool3(f)                             P_g_i(f, 5,15)	/* 制冷吸气过热3 */
#define P_val_cool_sv_chg_in1(f)                            P_g_i(f, 5,16)	/* 制冷过热转换1：回温 */
#define P_val_cool_sv_chg_in2(f)                            P_g_i(f, 5,17)	/* 制冷过热转换2 */
#define P_val_motor_sv_heat1(f)                             P_g_i(f, 5,18)	/* 制热吸气过热1 */
#define P_val_motor_sv_heat2(f)                             P_g_i(f, 5,19)	/* 制热吸气过热2 */
#define P_val_motor_sv_heat3(f)                             P_g_i(f, 5,20)	/* 制热吸气过热3 */
#define P_val_motor_sv_heat4(f)                             P_g_i(f, 5,21)	/* 制热吸气过热4 */
#define P_val_motor_sv_heat5(f)                             P_g_i(f, 5,22)	/* 制热吸气过热5 */
#define P_val_motor_sv_heat6(f)                             P_g_i(f, 5,23)	/* 制热吸气过热6 */
#define P_val_motor_sv_heat7(f)                             P_g_i(f, 5,24)	/* 制热吸气过热7 */
#define P_val_motor_sv_heat8(f)                             P_g_i(f, 5,25)	/* 制热吸气过热8 */
#define P_val_motor_sv_heat9(f)                             P_g_i(f, 5,26)	/* 制热吸气过热9 */
#define P_val_max_evap_set(f)                               P_g_i(f, 5,27)	/* 强关小蒸发上限 */
#define P_val_m_defrost_od(f)                               P_g_i(f, 5,28)	/* 除霜开度 */
#define P_val_m_limit_range(f)                              P_g_i(f, 5,29)	/* 主阀动作限幅 */
#define P_val_m_pid_p(f)                                    P_g_i(f, 5,30)	/* 比例带 */
#define P_val_m_pid_i(f)                                    P_g_i(f, 5,31)	/* 积分时间 */
#define P_val_m_pid_d(f)                                    P_g_i(f, 5,32)	/* 微分时间 */
#define P_val_m_pid_period(f)                               P_g_i(f, 5,33)	/* pid动作周期 */
#define P_val_m_pid_calculate_period(f)                     P_g_i(f, 5,34)	/* pid计算周期 */
#define P_val_m_pid_Kp(f)                                   P_g_i(f, 5,35)	/* 比例系数 */
#define P_val_motor_step_min_inht1(f)                       P_g_i(f, 5,36)	/* 制热最小开度1 使用指针访问 */
#define P_val_motor_step_min_inht2(f)                       P_g_i(f, 5,37)	/* 制热最小开度2 */
#define P_val_motor_step_min_inht3(f)                       P_g_i(f, 5,38)	/* 制热最小开度3 */
#define P_val_motor_step_min_inht4(f)                       P_g_i(f, 5,39)	/* 制热最小开度4 */
#define P_val_motor_step_min_inht5(f)                       P_g_i(f, 5,40)	/* 制热最小开度5 */
#define P_val_motor_step_min_incl1(f)                       P_g_i(f, 5,41)	/* 制冷最小开度1 使用指针访问 */
#define P_val_motor_step_min_incl2(f)                       P_g_i(f, 5,42)	/* 制冷最小开度2 */
#define P_val_motor_step_min_incl3(f)                       P_g_i(f, 5,43)	/* 制冷最小开度3 */
#define P_val_motor_step_min_incl4(f)                       P_g_i(f, 5,44)	/* 制冷最小开度4 */
#define P_val_motor_step_min_incl5(f)                       P_g_i(f, 5,45)	/* 制冷最小开度5 */
#define P_val_motor_limit_down_wing(f)                      P_g_i(f, 5,46)	/* 制热限关翅温 */
#define P_val_motor_defr_init_cpst(f)                       P_g_i(f, 5,47)	/* 除霜初开补偿 */
#define val_motor_idle_open                                 (P_val(val_motor_idle_open                ))/* 待机开度 */
#define val_m_motor_start_time                              (P_val(val_m_motor_start_time             ))/* 初开维持时间 */
#define val_motor_start_amplify_cool                        (P_val(val_motor_start_amplify_cool       ))/* 制冷初开放大 */
#define val_motor_start_amplify_heat1                       (P_val(val_motor_start_amplify_heat1      ))/* 制热初开放大1 */
#define val_motor_start_amplify_heat2                       (P_val(val_motor_start_amplify_heat2      ))/* 制热初开放大2 */
#define val_motor_start_amplify_heat3                       (P_val(val_motor_start_amplify_heat3      ))/* 制热初开放大3 */
#define val_motor_start_change_heat1                     (10*P_val(val_motor_start_change_heat1       ))/* 制热初开转换1：回温 */
#define val_motor_start_change_heat2                     (10*P_val(val_motor_start_change_heat2       ))/* 制热初开转换2 */
#define val_cool_min_initial_od                             (P_val(val_cool_min_initial_od            ))/* 制冷初开下限 */
#define val_cool_max_initial_od                             (P_val(val_cool_max_initial_od            ))/* 制冷初开上限 */
#define val_heat_min_initial_od                             (P_val(val_heat_min_initial_od            ))/* 制热初开下限 */
#define val_heat_max_initial_od                             (P_val(val_heat_max_initial_od            ))/* 制热初开上限 */
#define val_motor_start_amp_EVI_off                         (P_val(val_motor_start_amp_EVI_off        ))/* 关辅初开放大 */
#define val_motor_sv_cool1                                  (P_val(val_motor_sv_cool1                 ))/* 制冷吸气过热1 */
#define val_motor_sv_cool2                                  (P_val(val_motor_sv_cool2                 ))/* 制冷吸气过热2 */
#define val_motor_sv_cool3                                  (P_val(val_motor_sv_cool3                 ))/* 制冷吸气过热3 */
#define val_cool_sv_chg_in1                              (10*P_val(val_cool_sv_chg_in1                ))/* 制冷过热转换1：回温 */
#define val_cool_sv_chg_in2                              (10*P_val(val_cool_sv_chg_in2                ))/* 制冷过热转换2 */
#define val_motor_sv_heat1                                  (P_val(val_motor_sv_heat1                 ))/* 制热吸气过热1 */
#define val_motor_sv_heat2                                  (P_val(val_motor_sv_heat2                 ))/* 制热吸气过热2 */
#define val_motor_sv_heat3                                  (P_val(val_motor_sv_heat3                 ))/* 制热吸气过热3 */
#define val_motor_sv_heat4                                  (P_val(val_motor_sv_heat4                 ))/* 制热吸气过热4 */
#define val_motor_sv_heat5                                  (P_val(val_motor_sv_heat5                 ))/* 制热吸气过热5 */
#define val_motor_sv_heat6                                  (P_val(val_motor_sv_heat6                 ))/* 制热吸气过热6 */
#define val_motor_sv_heat7                                  (P_val(val_motor_sv_heat7                 ))/* 制热吸气过热7 */
#define val_motor_sv_heat8                                  (P_val(val_motor_sv_heat8                 ))/* 制热吸气过热8 */
#define val_motor_sv_heat9                                  (P_val(val_motor_sv_heat9                 ))/* 制热吸气过热9 */
#define val_max_evap_set                                    (P_val(val_max_evap_set                   ))/* 强关小蒸发上限 */
#define val_m_defrost_od                                    (P_val(val_m_defrost_od                   ))/* 除霜开度 */
#define val_m_limit_range                                   (P_val(val_m_limit_range                  ))/* 主阀动作限幅 */
#define val_m_pid_p                                         (P_val(val_m_pid_p                        ))/* 比例带 */
#define val_m_pid_i                                         (P_val(val_m_pid_i                        ))/* 积分时间 */
#define val_m_pid_d                                         (P_val(val_m_pid_d                        ))/* 微分时间 */
#define val_m_pid_period                                    (P_val(val_m_pid_period                   ))/* pid动作周期 */
#define val_m_pid_calculate_period                          (P_val(val_m_pid_calculate_period         ))/* pid计算周期 */
#define val_m_pid_Kp                                        (P_val(val_m_pid_Kp                       ))/* 比例系数 */
#define val_motor_step_min_inht1                            (P_val(val_motor_step_min_inht1           ))/* 制热最小开度1 使用指针访问 */
#define val_motor_step_min_inht2                            (P_val(val_motor_step_min_inht2           ))/* 制热最小开度2 */
#define val_motor_step_min_inht3                            (P_val(val_motor_step_min_inht3           ))/* 制热最小开度3 */
#define val_motor_step_min_inht4                            (P_val(val_motor_step_min_inht4           ))/* 制热最小开度4 */
#define val_motor_step_min_inht5                            (P_val(val_motor_step_min_inht5           ))/* 制热最小开度5 */
#define val_motor_step_min_incl1                            (P_val(val_motor_step_min_incl1           ))/* 制冷最小开度1 使用指针访问 */
#define val_motor_step_min_incl2                            (P_val(val_motor_step_min_incl2           ))/* 制冷最小开度2 */
#define val_motor_step_min_incl3                            (P_val(val_motor_step_min_incl3           ))/* 制冷最小开度3 */
#define val_motor_step_min_incl4                            (P_val(val_motor_step_min_incl4           ))/* 制冷最小开度4 */
#define val_motor_step_min_incl5                            (P_val(val_motor_step_min_incl5           ))/* 制冷最小开度5 */
#define val_motor_limit_down_wing                        (10*P_val(val_motor_limit_down_wing          ))/* 制热限关翅温 */
#define val_motor_defr_init_cpst                            (P_val(val_motor_defr_init_cpst           ))/* 除霜初开补偿 */

#define val_max_evap_temp                                (10*(val_max_evap_set%100))/* 强关小蒸发上限 */
#define val_evap_high_err                                (10*(val_max_evap_set/100))/* 蒸发上限保护值 */

#define val_motor_step_min_inht(x)          (*(&val_motor_step_min_inht1+(x)))
#define val_motor_step_min_incl(x)          (*(&val_motor_step_min_incl1+(x)))

#define val_m_pid_flexi                     (10)   // 积分系数

/* 7 辅路电膨设置 */
#define P_val_s_motor_use_heat(f)                           P_g_i(f, 6,0)	/* 经济器制热开启 */
#define P_val_s_motor_use_cool(f)                           P_g_i(f, 6,1)	/* 经济器制冷开启 */
#define P_val_evi_open_dly(f)                               P_g_i(f, 6,2)	/* 辅阀开机延时 */
#define P_val_evi_open_env_heat(f)                          P_g_i(f, 6,3)	/* 制热开阀环温 */
#define P_val_evi_open_exh(f)                               P_g_i(f, 6,4)	/* 辅阀开启排温 */
#define P_val_evi_open_env_cool(f)                          P_g_i(f, 6,5)	/* 制冷开阀环温 */
#define P_val_s_motor_sv_dif(f)                             P_g_i(f, 6,6)	/* 辅路过热度偏差 */
#define P_val_evi_cool_init_od(f)                           P_g_i(f, 6,7)	/* 制冷初始开度(EVI) */
#define P_val_evi_heat_init_od1(f)                          P_g_i(f, 6,8)	/* 制热初始开度1(EVI) */
#define P_val_evi_heat_init_od2(f)                          P_g_i(f, 6,9)	/* 制热初始开度2 */
#define P_val_evi_heat_init_od3(f)                          P_g_i(f, 6,10)	/* 制热初始开度3 */
#define P_val_evi_heat_init_od4(f)                          P_g_i(f, 6,11)	/* 制热初始开度4 */
#define P_val_evi_open_exh_overheat_cool(f)                 P_g_i(f, 6,12)	/* 制冷开启排过 */
#define P_val_evi_open_exh_overheat_heat(f)                 P_g_i(f, 6,13)	/* 制热开启排过 */
#define P_val_s_motion_period(f)                            P_g_i(f, 6,14)	/* 备用 */
#define P_val_s_pid_p(f)                                    P_g_i(f, 6,15)	/* 比例带 */
#define P_val_s_pid_i(f)                                    P_g_i(f, 6,16)	/* 积分时间 */
#define P_val_s_pid_d(f)                                    P_g_i(f, 6,17)	/* 微分时间 */
#define P_val_s_pid_period(f)                               P_g_i(f, 6,18)	/* pid动作周期 */
#define P_val_s_pid_calculate_period(f)                     P_g_i(f, 6,19)	/* pid计算周期 */
#define P_val_s_pid_Kp(f)                                   P_g_i(f, 6,20)	/* 比例系数 */
#define P_val_s_pid_flexi(f)                                P_g_i(f, 6,21)	/* 积分系数 */
#define P_val_e_ext_1(f)                                    P_g_i(f, 6,22)	/* 过热度转换1   */
#define P_val_e_ext_2(f)                                    P_g_i(f, 6,23)	/* 过热度转换2   */
#define P_val_e_sv_1(f)                                     P_g_i(f, 6,24)	/* 过热度设定1   */
#define P_val_e_sv_2(f)                                     P_g_i(f, 6,25)	/* 过热度设定2   */
#define P_val_exh_sv_max(f)                                 P_g_i(f, 6,26)	/* 最高目标排温 */
#define P_val_s_exh_sv_cold(f)                              P_g_i(f, 6,27)	/* 制冷排气过热 */
#define P_val_gas_overheat_k(f)                             P_g_i(f, 6,28)	/* 排过水温系数 */
#define P_val_gas_overheat_a1(f)                            P_g_i(f, 6,29)	/* 制热排气过热1 */
#define P_val_gas_overheat_a2(f)                            P_g_i(f, 6,30)	/* 制热排气过热2 */
#define P_val_gas_overheat_a3(f)                            P_g_i(f, 6,31)	/* 制热排气过热3 */
#define P_val_gas_overheat_a4(f)                            P_g_i(f, 6,32)	/* 制热排气过热4 */
#define P_val_gas_overheat_a5(f)                            P_g_i(f, 6,33)	/* 制热排气过热5 */
#define P_val_evi_open_rps(f)                               P_g_i(f, 6,34)	/* 辅阀开启转速 */
#define P_val_evi_close_rpg(f)                              P_g_i(f, 6,35)	/* 辅阀关闭转速 */
#define P_val_evi_open_exh_overheat_diff(f)                 P_g_i(f, 6,36)	/* 辅阀开排过偏差 */
#define P_val_evi_open_exh_diff(f)                          P_g_i(f, 6,37)	/* 辅阀开排温偏差 */
#define P_val_evi_for_liquid(f)                             P_g_i(f, 6,38)	/* 辅阀当喷液阀 */
#define P_val_ability_test_s_motor_ctrl_HA(f)               P_g_i(f, 6,39)	/* 制热辅阀控制A */
#define P_val_ability_test_s_motor_ctrl_HB(f)               P_g_i(f, 6,40)	/* 制热辅阀控制B */
#define P_val_ability_test_s_motor_ctrl_HE(f)               P_g_i(f, 6,41)	/* 制热辅阀控制E */
#define P_val_ability_test_s_motor_ctrl_HG(f)               P_g_i(f, 6,42)	/* 制热辅阀控制G */
#define P_val_ability_test_s_motor_ctrl_HC(f)               P_g_i(f, 6,43)	/* 制热辅阀控制C */
#define P_val_ability_test_s_motor_ctrl_HD(f)               P_g_i(f, 6,44)	/* 制热辅阀控制D */
#define P_val_ability_test_s_motor_ctrl_C(f)                P_g_i(f, 6,45)	/* 制冷辅阀控制  */
#define val_s_motor_use_heat                                (P_val(val_s_motor_use_heat               ))/* 经济器制热开启 */
#define val_s_motor_use_cool                                (P_val(val_s_motor_use_cool               ))/* 经济器制冷开启 */
#define val_evi_open_dly                                    (P_val(val_evi_open_dly                   ))/* 辅阀开机延时 */
#define val_evi_open_env_heat                            (10*P_val(val_evi_open_env_heat              ))/* 制热开阀环温 */
#define val_evi_open_exh                                 (10*P_val(val_evi_open_exh                   ))/* 辅阀开启排温 */
#define val_evi_open_env_cool                            (10*P_val(val_evi_open_env_cool              ))/* 制冷开阀环温 */
#define val_s_motor_sv_dif                               (10*P_val(val_s_motor_sv_dif                 ))/* 辅路过热度偏差        */
#define val_evi_cool_init_od                                (P_val(val_evi_cool_init_od               ))/* 制冷初始开度(EVI) */
#define val_evi_heat_init_od1                               (P_val(val_evi_heat_init_od1              ))/* 制热初始开度1(EVI) */
#define val_evi_heat_init_od2                               (P_val(val_evi_heat_init_od2              ))/* 制热初始开度2 */
#define val_evi_heat_init_od3                               (P_val(val_evi_heat_init_od3              ))/* 制热初始开度3 */
#define val_evi_heat_init_od4                               (P_val(val_evi_heat_init_od4              ))/* 制热初始开度4 */
#define val_evi_open_exh_overheat_cool                   (10*P_val(val_evi_open_exh_overheat_cool     ))/* 制冷开启排过 */
#define val_evi_open_exh_overheat_heat                   (10*P_val(val_evi_open_exh_overheat_heat     ))/* 制热开启排过 */
#define val_s_motion_period                                 (P_val(val_s_motion_period                ))/* 备用 */
#define val_s_pid_p                                         (P_val(val_s_pid_p                        ))/* 比例带 */
#define val_s_pid_i                                         (P_val(val_s_pid_i                        ))/* 积分时间 */
#define val_s_pid_d                                         (P_val(val_s_pid_d                        ))/* 微分时间 */
#define val_s_pid_period                                    (P_val(val_s_pid_period                   ))/* pid动作周期 */
#define val_s_pid_calculate_period                          (P_val(val_s_pid_calculate_period         ))/* pid计算周期 */
#define val_s_pid_Kp                                        (P_val(val_s_pid_Kp                       ))/* 比例系数 */
#define val_s_pid_flexi                                     (P_val(val_s_pid_flexi                    ))/* 积分系数 */
#define val_e_ext_1                                      (10*P_val(val_e_ext_1                        ))/* 过热度转换1   */
#define val_e_ext_2                                      (10*P_val(val_e_ext_2                        ))/* 过热度转换2   */
#define val_e_sv_1                                          (P_val(val_e_sv_1                         ))/* 过热度设定1   */
#define val_e_sv_2                                          (P_val(val_e_sv_2                         ))/* 过热度设定2   */
#define val_exh_sv_max                                   (10*P_val(val_exh_sv_max                     ))/* 最高目标排温 */
#define val_s_exh_sv_cold                                (10*P_val(val_s_exh_sv_cold                  ))/* 制冷排气过热 */
#define val_gas_overheat_k                                  (P_val(val_gas_overheat_k                 ))/* 排过水温系数 */
#define val_gas_overheat_a1                                 (P_val(val_gas_overheat_a1                ))/* 制热排气过热1 */
#define val_gas_overheat_a2                                 (P_val(val_gas_overheat_a2                ))/* 制热排气过热2 */
#define val_gas_overheat_a3                                 (P_val(val_gas_overheat_a3                ))/* 制热排气过热3 */
#define val_gas_overheat_a4                                 (P_val(val_gas_overheat_a4                ))/* 制热排气过热4 */
#define val_gas_overheat_a5                                 (P_val(val_gas_overheat_a5                ))/* 制热排气过热5 */
#define val_evi_open_rps                                (100*P_val(val_evi_open_rps                   ))/* 辅阀开启转速 */
#define val_evi_close_rpg                               (100*P_val(val_evi_close_rpg                  ))/* 辅阀关闭转速 */
#define val_evi_open_exh_overheat_diff                   (10*P_val(val_evi_open_exh_overheat_diff     ))/* 辅阀开排过偏差 */
#define val_evi_open_exh_diff                            (10*P_val(val_evi_open_exh_diff              ))/* 辅阀开排温偏差 */
#define val_evi_for_liquid                                  (P_val(val_evi_for_liquid                 ))/* 辅阀当喷液阀 */
#define val_ability_test_s_motor_ctrl_HA                    (P_val(val_ability_test_s_motor_ctrl_HA   ))/* 制热辅阀控制A */
#define val_ability_test_s_motor_ctrl_HB                    (P_val(val_ability_test_s_motor_ctrl_HB   ))/* 制热辅阀控制B */
#define val_ability_test_s_motor_ctrl_HE                    (P_val(val_ability_test_s_motor_ctrl_HE   ))/* 制热辅阀控制E */
#define val_ability_test_s_motor_ctrl_HG                    (P_val(val_ability_test_s_motor_ctrl_HG   ))/* 制热辅阀控制G */
#define val_ability_test_s_motor_ctrl_HC                    (P_val(val_ability_test_s_motor_ctrl_HC   ))/* 制热辅阀控制C */
#define val_ability_test_s_motor_ctrl_HD                    (P_val(val_ability_test_s_motor_ctrl_HD   ))/* 制热辅阀控制D */
#define val_ability_test_s_motor_ctrl_C                     (P_val(val_ability_test_s_motor_ctrl_C    ))/* 制冷辅阀控制  */

/* 8除霜参数设置 */
#define P_val_defrost_first_run_period(f)                   P_g_i(f, 7, 0)	/* 首次除霜间隔 */
#define P_val_defrost_time(f)                               P_g_i(f, 7, 1)	/* 除霜时间 */
#define P_val_defrost_time_sel(f)                           P_g_i(f, 7, 2)	/* 积霜计时选择 */
#define P_val_defrost_allow_environ(f)                      P_g_i(f, 7, 3)	/* 允许除霜环温 */
#define P_val_defrost_allow_wing(f)                         P_g_i(f, 7, 4)	/* 允许除霜翅温 */
#define P_val_defrost_end_wing(f)                           P_g_i(f, 7, 5)	/* 退除霜翅温 */
#define P_val_defrost_end_out(f)                            P_g_i(f, 7, 6)	/* 退除霜出水温度 */
#define P_val_defrost_env_wing_rise_en(f)                   P_g_i(f, 7, 7)	/* 环翅智能除霜 */
#define P_val_defrost_EWD_rise_diff1(f)                     P_g_i(f, 7, 8)	/* 中度结霜温差 */
#define P_val_defrost_EWD_rise_diff2(f)                     P_g_i(f, 7, 9)	/* 重度结霜温差 */
#define P_val_defrost_temp_sure_time(f)                     P_g_i(f, 7,10)	/* 温度确认时间 */
#define P_val_defrost_environ1(f)                           P_g_i(f, 7,11)	/* 除霜环温1 */
#define P_val_defrost_environ2(f)                           P_g_i(f, 7,12)	/* 除霜环温2 */
#define P_val_defrost_environ3(f)                           P_g_i(f, 7,13)	/* 除霜环温3 */
#define P_val_defrost_environ4(f)                           P_g_i(f, 7,14)	/* 除霜环温4 */
#define P_val_defrost_period1(f)                            P_g_i(f, 7,15)	/* 除霜间隔1 */
#define P_val_defrost_period2(f)                            P_g_i(f, 7,16)	/* 除霜间隔2 */
#define P_val_defrost_period3(f)                            P_g_i(f, 7,17)	/* 除霜间隔3 */
#define P_val_defrost_period4(f)                            P_g_i(f, 7,18)	/* 除霜间隔4 */
#define P_val_defrost_period5(f)                            P_g_i(f, 7,19)	/* 除霜间隔5 */
#define P_val_defrost_allow_diff1(f)                        P_g_i(f, 7,20)	/* 除霜环翅差1 */
#define P_val_defrost_allow_diff2(f)                        P_g_i(f, 7,21)	/* 除霜环翅差2 */
#define P_val_defrost_allow_diff3(f)                        P_g_i(f, 7,22)	/* 除霜环翅差3 */
#define P_val_defrost_allow_diff4(f)                        P_g_i(f, 7,23)	/* 除霜环翅差4 */
#define P_val_defrost_allow_diff5(f)                        P_g_i(f, 7,24)	/* 除霜环翅差5 */
#define P_val_defrost_LP(f)                                 P_g_i(f, 7,25)	/* 低压除霜设置 */
#define P_val_defrost_lp_dly(f)                             P_g_i(f, 7,26)	/* 低压除霜检测延时 */
#define P_val_defrost_lp_time(f)                            P_g_i(f, 7,27)	/* 低压除霜间隔 */
#define P_val_defrost_lp_wing(f)                            P_g_i(f, 7,28)	/* 低压除霜翅温 */
#define P_val_defrost_longtime(f)                           P_g_i(f, 7,29)	/* 定时除霜间隔 */
#define P_val_defrost_emergency_diff(f)                     P_g_i(f, 7,30)	/* 紧急除霜环翅 */
#define P_val_defrost_stop_cm_en(f)                         P_g_i(f, 7,31)	/* 除霜停机切阀 */
#define P_val_defrost_stop_cm_tm_entry(f)                   P_g_i(f, 7,32)	/* 进除霜压机延时 */
#define P_val_defrost_four_tm_entry(f)                      P_g_i(f, 7,33)	/* 进除霜四通延时 */
#define P_val_defrost_stop_cm_tm_exit(f)                    P_g_i(f, 7,34)	/* 退除霜压机延时 */
#define P_val_defrost_four_tm_exit(f)                       P_g_i(f, 7,35)	/* 退除霜四通延时 */
#define P_val_defrost_chg_freq(f)                           P_g_i(f, 7,36)	/* 除霜切阀频率 */
#define P_val_defrost_dripping_time(f)                      P_g_i(f, 7,37)	/* 停风滴水时间 */
#define P_val_defrost_min_time(f)                           P_g_i(f, 7,38)	/* 最小除霜间隔 */
#define P_val_defrost_emergency_time(f)                     P_g_i(f, 7,39)	/* 紧急除霜间隔 */
#define val_defrost_first_run_period                     (60*P_val(val_defrost_first_run_period       ))/* 首次除霜间隔 */
#define val_defrost_time                                    (P_val(val_defrost_time                   ))/* 除霜时间 */
#define val_defrost_time_sel                                (P_val(val_defrost_time_sel               ))/* 积霜计时选择 */
#define val_defrost_allow_environ                        (10*P_val(val_defrost_allow_environ          ))/* 允许除霜环温 */
#define val_defrost_allow_wing                           (10*P_val(val_defrost_allow_wing             ))/* 允许除霜翅温 */
#define val_defrost_end_wing                             (10*P_val(val_defrost_end_wing               ))/* 退除霜翅温 */
#define val_defrost_end_out                              (10*P_val(val_defrost_end_out                ))/* 退除霜出水温度 */
#define val_defrost_env_wing_rise_en                        (P_val(val_defrost_env_wing_rise_en       ))/* 环翅智能除霜 */
#define val_defrost_EWD_rise_diff1                       (10*P_val(val_defrost_EWD_rise_diff1         ))/* 中度结霜温差 */
#define val_defrost_EWD_rise_diff2                       (10*P_val(val_defrost_EWD_rise_diff2         ))/* 重度结霜温差 */
#define val_defrost_temp_sure_time                       (60*P_val(val_defrost_temp_sure_time         ))/* 温度确认时间 */
#define val_defrost_environ1                             (10*P_val(val_defrost_environ1               ))/* 除霜环温1 */
#define val_defrost_environ2                             (10*P_val(val_defrost_environ2               ))/* 除霜环温2 */
#define val_defrost_environ3                             (10*P_val(val_defrost_environ3               ))/* 除霜环温3 */
#define val_defrost_environ4                             (10*P_val(val_defrost_environ4               ))/* 除霜环温4 */
#define val_defrost_period1                              (60*P_val(val_defrost_period1                ))/* 除霜间隔1 */
#define val_defrost_period2                              (60*P_val(val_defrost_period2                ))/* 除霜间隔2 */
#define val_defrost_period3                              (60*P_val(val_defrost_period3                ))/* 除霜间隔3 */
#define val_defrost_period4                              (60*P_val(val_defrost_period4                ))/* 除霜间隔4 */
#define val_defrost_period5                              (60*P_val(val_defrost_period5                ))/* 除霜间隔5 */
#define val_defrost_allow_diff1                          (10*P_val(val_defrost_allow_diff1            ))/* 除霜环翅差1 */
#define val_defrost_allow_diff2                          (10*P_val(val_defrost_allow_diff2            ))/* 除霜环翅差2 */
#define val_defrost_allow_diff3                          (10*P_val(val_defrost_allow_diff3            ))/* 除霜环翅差3 */
#define val_defrost_allow_diff4                          (10*P_val(val_defrost_allow_diff4            ))/* 除霜环翅差4 */
#define val_defrost_allow_diff5                          (10*P_val(val_defrost_allow_diff5            ))/* 除霜环翅差5 */
#define val_defrost_LP                                      (P_val(val_defrost_LP                     ))/* 低压除霜设置 */
#define val_defrost_lp_dly                               (60*P_val(val_defrost_lp_dly                 ))/* 低压除霜检测延时 */
#define val_defrost_lp_time                              (60*P_val(val_defrost_lp_time                ))/* 低压除霜间隔 */
#define val_defrost_lp_wing                              (10*P_val(val_defrost_lp_wing                ))/* 低压除霜翅温 */
#define val_defrost_longtime                           (3600*P_val(val_defrost_longtime               ))/* 定时除霜间隔 */
#define val_defrost_emergency_diff                       (10*P_val(val_defrost_emergency_diff         ))/* 紧急除霜环翅 */
#define val_defrost_stop_cm_en                              (P_val(val_defrost_stop_cm_en             ))/* 除霜停机切阀 */
#define val_defrost_stop_cm_tm_entry                        (P_val(val_defrost_stop_cm_tm_entry       ))/* 进除霜压机延时 */
#define val_defrost_four_tm_entry                           (P_val(val_defrost_four_tm_entry          ))/* 进除霜四通延时 */
#define val_defrost_stop_cm_tm_exit                         (P_val(val_defrost_stop_cm_tm_exit        ))/* 退除霜压机延时 */
#define val_defrost_four_tm_exit                            (P_val(val_defrost_four_tm_exit           ))/* 退除霜四通延时 */
#define val_defrost_chg_freq                            (100*P_val(val_defrost_chg_freq               ))/* 除霜切阀频率 */
#define val_defrost_dripping_time                           (P_val(val_defrost_dripping_time          ))/* 停风滴水时间 */
#define val_defrost_min_time                             (60*P_val(val_defrost_min_time               ))/* 最小除霜间隔 */
#define val_defrost_emergency_time                       (60*P_val(val_defrost_emergency_time         ))/* 紧急除霜间隔 */

#define val_err_emerg_defrost_use       (TRUE)              // 紧急除霜频繁使用
#define val_defrost_env_wing_upper      (200)               /* 环翅差上限 20.0  */
#define val_defrost_env_wing_lower      (-50)               /* 环翅差下限 -5.0  */

#define val_timingdfr_temp_in1          (250)				/* 定时除霜回温1 */
#define val_timingdfr_temp_in2          (400)				/* 定时除霜回温2 */
#define val_defrost_longtime1           (40*60)				/* 定时除霜间隔1 */
#define val_defrost_longtime2           (60*60)				/* 定时除霜间隔2 */
#define val_defrost_longtime3           (90*60)				/* 定时除霜间隔3 */
#define val_exit_dfr_wingT2		        (30)                /* 退除霜翅温2 */
#define val_exit_dfr_wingT2_time        (150)				/* 退除霜翅温维持 */

/* 9防冻参数设置 */
#define P_val_anti_off_time1(f)                             P_g_i(f, 8,0)  /* 防冻间隔1 */
#define P_val_anti_off_time2(f)                             P_g_i(f, 8,1)  /* 防冻间隔2 */
#define P_val_anti_pump_on(f)                               P_g_i(f, 8,2)  /* 水泵防冻温度 */
#define P_val_anti_heat_on(f)                               P_g_i(f, 8,3)  /* 电热防冻温度 */
#define P_val_anti_comp_on(f)                               P_g_i(f, 8,4)  /* 压机防冻温度 */
#define P_val_anti_heat_off(f)                              P_g_i(f, 8,5)  /* 电热退防冻温度 */
#define P_val_anti_comp_off(f)                              P_g_i(f, 8,6)  /* 压机退防冻温度 */
#define P_val_anti_embient(f)                               P_g_i(f, 8,7)  /* 防冻间隔环温 */
#define P_val_anti_env_in(f)                                P_g_i(f, 8,8)  /* 进防冻环温 */
#define P_val_anti_env_diff(f)                              P_g_i(f, 8,9)  /* 退防冻环温差 */
#define P_val_anti_used(f)                                  P_g_i(f, 8,10)  /* 防冻功能设置 */
#define P_val_PUMPsl_antifreeze(f)                          P_g_i(f, 8,11)  /* 太阳能水泵防冻 */
#define P_val_timed_antifreeze_interval(f)                  P_g_i(f, 8,12)  /* 定时防冻间隔 */
#define P_val_timed_antifreeze_max(f)                       P_g_i(f, 8,13)  /* 定时防冻时长 */
#define P_val_anti_bak7(f)                                  P_g_i(f, 8,14)  /* 备用 */
#define P_val_anti_bak8(f)                                  P_g_i(f, 8,15)  /* 备用 */
#define P_val_anti_bak9(f)                                  P_g_i(f, 8,16)  /* 备用 */
#define P_val_anti_bak10(f)                                 P_g_i(f, 8,17)  /* 备用 */
#define P_val_anti_bak11(f)                                 P_g_i(f, 8,18)  /* 备用 */
#define P_val_anti_bak12(f)                                 P_g_i(f, 8,19)  /* 备用 */
#define P_val_anti_bak13(f)                                 P_g_i(f, 8,20)  /* 备用 */
#define P_val_anti_bak14(f)                                 P_g_i(f, 8,21)  /* 备用 */
#define P_val_anti_bak15(f)                                 P_g_i(f, 8,22)  /* 备用 */
#define P_val_anti_bak16(f)                                 P_g_i(f, 8,23)  /* 备用 */
#define P_val_anti_bak17(f)                                 P_g_i(f, 8,24)  /* 备用 */
#define P_val_anti_bak18(f)                                 P_g_i(f, 8,25)  /* 备用 */
#define P_val_anti_bak19(f)                                 P_g_i(f, 8,26)  /* 备用 */
#define P_val_anti_bak20(f)                                 P_g_i(f, 8,27)  /* 备用 */
#define P_val_anti_bak22(f)                                 P_g_i(f, 8,28)  /* 备用 */
#define P_val_anti_bak23(f)                                 P_g_i(f, 8,29)  /* 备用 */
#define P_val_anti_bak24(f)                                 P_g_i(f, 8,30)  /* 备用 */
#define P_val_heat_chg_env1(f)                           	P_g_i(f, 8,31)  /* 制热转换环1 */
#define P_val_heat_chg_env2(f)                           	P_g_i(f, 8,32)  /* 制热转换环2 */
#define P_val_heat_chg_env3(f)                           	P_g_i(f, 8,33)  /* 制热转换环3 */
#define P_val_heat_chg_env4(f)                           	P_g_i(f, 8,34)  /* 制热转换环4 */
#define P_val_heat_chg_env5(f)                           	P_g_i(f, 8,35)  /* 制热转换环5 */

#define val_anti_off_time1                               (60*P_val(val_anti_off_time1                 ))  /* 防冻间隔1      */
#define val_anti_off_time2                               (60*P_val(val_anti_off_time2                 ))  /* 防冻间隔2      */
#define val_anti_pump_on                                 (10*P_val(val_anti_pump_on                   ))  /* 水泵防冻温度   */
#define val_anti_heat_on                                 (10*P_val(val_anti_heat_on                   ))  /* 电热防冻温度   */
#define val_anti_comp_on                                 (10*P_val(val_anti_comp_on                   ))  /* 压机防冻温度   */
#define val_anti_heat_off                                (10*P_val(val_anti_heat_off                  ))  /* 电热退防冻温度 */
#define val_anti_comp_off                                (10*P_val(val_anti_comp_off                  ))  /* 压机退防冻温度 */
#define val_anti_embient                                 (10*P_val(val_anti_embient                   ))  /* 防冻间隔环温   */
#define val_anti_env_in                                  (10*P_val(val_anti_env_in                    ))  /* 进防冻环温     */
#define val_anti_env_diff                                (10*P_val(val_anti_env_diff                  ))  /* 退防冻环温差   */
#define val_anti_used                                       (P_val(val_anti_used                      ))  /* 防冻功能设置   */
#define val_PUMPsl_antifreeze                               (P_val(val_PUMPsl_antifreeze              ))  /* 太阳能水泵防冻 */
#define val_timed_antifreeze_interval                    (60*P_val(val_timed_antifreeze_interval      ))  /* 定时防冻间隔 */
#define val_timed_antifreeze_max                         (60*P_val(val_timed_antifreeze_max           ))  /* 定时防冻时长 */
#define val_anti_bak7                                       (P_val(val_anti_bak7                      ))  /* 备用 */
#define val_anti_bak8                                       (P_val(val_anti_bak8                      ))  /* 备用 */
#define val_anti_bak9                                       (P_val(val_anti_bak9                      ))  /* 备用 */
#define val_anti_bak10                                 		(P_val(val_anti_bak10                     ))  /* 备用 */
#define val_anti_bak11                                 		(P_val(val_anti_bak11                     ))  /* 备用 */
#define val_anti_bak12                                 		(P_val(val_anti_bak12                     ))  /* 备用 */
#define val_anti_bak13                                 		(P_val(val_anti_bak13                     ))  /* 备用 */
#define val_anti_bak14                                 		(P_val(val_anti_bak14                     ))  /* 备用 */
#define val_anti_bak15                                 		(P_val(val_anti_bak15                     ))  /* 备用 */
#define val_anti_bak16                                 		(P_val(val_anti_bak16                     ))  /* 备用 */
#define val_anti_bak17                                 		(P_val(val_anti_bak17                     ))  /* 备用 */
#define val_anti_bak18                                 		(P_val(val_anti_bak18                     ))  /* 备用 */
#define val_anti_bak19                                 		(P_val(val_anti_bak19                     ))  /* 备用 */
#define val_anti_bak20                                 		(P_val(val_anti_bak20                     ))  /* 备用 */
#define val_anti_bak22                                 		(P_val(val_anti_bak22                     ))  /* 备用 */
#define val_anti_bak23                                 		(P_val(val_anti_bak23                     ))  /* 备用 */
#define val_anti_bak24                                 		(P_val(val_anti_bak24                     ))  /* 备用 */
#define val_heat_chg_env1                           		(10*P_val(val_heat_chg_env1            	  ))  /* 制热转换环1 */
#define val_heat_chg_env2                           		(10*P_val(val_heat_chg_env2               ))  /* 制热转换环2 */
#define val_heat_chg_env3                           		(10*P_val(val_heat_chg_env3               ))  /* 制热转换环3 */
#define val_heat_chg_env4                           		(10*P_val(val_heat_chg_env4               ))  /* 制热转换环4 */
#define val_heat_chg_env5                           		(10*P_val(val_heat_chg_env5               ))  /* 制热转换环5 */

/* 10保护参数设置 */
#define P_ERR_DLY(f)                                        P_g_i(f, 9, 0)	/* 一般故障延时 隐藏此参数功能由立即数ERR_DLY代替*/
#define P_val_err_ac_flux_delay(f)                          P_g_i(f, 9, 1)	/* 水流不足故障延时 */
#define P_val_auto_reset_time(f)                            P_g_i(f, 9, 2)	/* 故障复位时间 */
#define P_val_auto_reset_time1(f)                           P_g_i(f, 9, 3)	/* 自动复位允许时间 */
#define P_val_lp_detect_dly(f)                              P_g_i(f, 9, 4)	/* 低压故障检测延时 */
#define P_val_defrost_lp_limit_dly(f)                       P_g_i(f, 9, 5)	/* 退除霜低压检延 */
#define P_val_cool_lp_alarm_delay(f)                        P_g_i(f, 9, 6)	/* 制冷低压确认延时*/
#define P_val_heat_lp_alarm_delay(f)                        P_g_i(f, 9, 7)	/* 制热低压确认延时 */
#define P_val_err_wt_exit(f)                                P_g_i(f, 9, 8)	/* 退水温保护温差 */
#define P_val_err_ac_out_low(f)                             P_g_i(f, 9, 9)	/* 制冷出温过低 */
#define P_val_err_ac_out_high(f)                            P_g_i(f, 9,10)	/* 制热出温过高 */
#define P_val_forbid_on_env_cool(f)                         P_g_i(f, 9,11)	/* 制冷禁开环温 */
#define P_val_out_in_diff_err(f)                            P_g_i(f, 9,12)	/* 出回温差异常 */
#define P_val_gas_in_out_err_dly(f)                         P_g_i(f, 9,13)	/* 吸排异常检延 */
#define P_val_err_flux_open_pump(f)                         P_g_i(f, 9,14)	/* 水流不足开泵 */
#define P_val_invt_rps_force_sub_percent(f)                 P_g_i(f, 9,15)	/* 强制降频比例 */
#define P_val_invt_rps_force_sub_period(f)                  P_g_i(f, 9,16)	/* 强制降频周期  */
#define P_val_out_in_diff_high(f)                           P_g_i(f, 9,17)	/* 出回温差过大 */
#define P_val_evap_low_cool(f)                              P_g_i(f, 9,18)	/* 制冷蒸发过低 */
#define P_val_gas_out_high(f)                               P_g_i(f, 9,19)	/* 排气温度过高 */
#define P_ERR_WT_EXIT(f)                                    P_g_i(f, 9,20)	/* 退排气温度过高(温差) */
#define P_val_gas_in_low_cool(f)                            P_g_i(f, 9,21)	/* 制冷吸气过低 */
#define P_val_pwm_speed_too_low(f)                          P_g_i(f, 9,22)	/* PWM风速过低 */
#define P_val_pwm_speed_check_dly(f)                        P_g_i(f, 9,23)	/* PWM风速检测延时 */
#define P_val_check_lp_when_off(f)                          P_g_i(f, 9,24)	/* 待机检测低压 */
#define P_val_TWout1_limit_1(f)                             P_g_i(f, 9,25)	/* 制热出温限制1 */
#define P_val_TWout1_limit_2(f)                             P_g_i(f, 9,26)	/* 制热出温限制2 */
#define P_val_TWout1_limit_3(f)                             P_g_i(f, 9,27)	/* 制热出温限制3 */
#define P_val_TWout1_limit_4(f)                             P_g_i(f, 9,28)	/* 制热出温限制4 */
#define P_val_TWout1_limit_5(f)                             P_g_i(f, 9,29)	/* 制热出温限制5 */
#define P_val_TFLin_protect(f)                              P_g_i(f, 9,30)	/* 地暖过热保护 */
#define P_val_lpcheck_evn_temp(f)                           P_g_i(f, 9,31)	/* 低压检查环温:低压低于此温度不检测低压开关 */
#define P_val_lp_value_incl(f)                              P_g_i(f, 9,32)	/* 制冷低压过低() */
#define P_val_lp_value_inht(f)                              P_g_i(f, 9,33)	/* 制热低压过低() */
#define P_val_hp_value_incl(f)                              P_g_i(f, 9,34)	/* 制冷高压过高() */
#define P_val_hp_value_inht(f)                              P_g_i(f, 9,35)	/* 制热高压过高() */
#define P_val_lp_prs_exit_diff(f)                           P_g_i(f, 9,36)	/* 低压报警复位压差 */
#define P_val_hp_prs_exit_diff(f)                           P_g_i(f, 9,37)	/* 高压报警复位压差 */
#define P_val_low_temp_cool_env(f)                          P_g_i(f, 9,38)	/* 低温制冷环温 */
#define P_val_low_temp_cool_dly(f)                          P_g_i(f, 9,39)	/* 低温制冷检延 */
#define P_val_mac_set_check(f)                              P_g_i(f, 9,40)	/* 机型设置校验 */
#define P_val_TWout1_limit_cool(f)                          P_g_i(f, 9,41)	/* 制冷出温限制 */
#define P_val_use_UI35_commu_detect(f)                      P_g_i(f, 9,42)	/* 线控器通讯检测 */
#define P_val_invt_err_auto_reset(f)                        P_g_i(f, 9,43)	/* 变频故障自复位 */
#define P_val_hotw_max_TBH(f)                               P_g_i(f, 9,44)	/* 电热热水上限 */
#define P_val_hp_pre_adju_diff(f)                           P_g_i(f, 9,45)  /* 高压辅调回差 */
#define P_val_lp_pre_adju_diff(f)                           P_g_i(f, 9,46)  /* 低压辅调回差 */
#define ERR_DLY                                             (P_val(ERR_DLY                            ))/* 01 一般故障延时 隐藏此参数功能由立即数ERR_DLY代替*/
#define val_err_ac_flux_delay                               (P_val(val_err_ac_flux_delay              ))/* 02 水流不足故障延时 */
#define val_auto_reset_time                              (60*P_val(val_auto_reset_time                ))/* 03 故障复位时间 */
#define val_auto_reset_time1                             (60*P_val(val_auto_reset_time1               ))/* 04 自动复位允许时间 */
#define val_lp_detect_dly                                   (P_val(val_lp_detect_dly                  ))/* 05 低压故障检测延时 */
#define val_defrost_lp_limit_dly                            (P_val(val_defrost_lp_limit_dly           ))/* 06 退除霜低压检延 */
#define val_cool_lp_alarm_delay                             (P_val(val_cool_lp_alarm_delay            ))/* 07 制冷低压确认延时*/
#define val_heat_lp_alarm_delay                             (P_val(val_heat_lp_alarm_delay            ))/* 08 制热低压确认延时 */
#define val_err_wt_exit                                  (10*P_val(val_err_wt_exit                    ))/* 09 退出温保护温差 */
#define val_err_ac_out_low                               (10*P_val(val_err_ac_out_low                 ))/* 10 制冷出温过低 */
#define val_err_ac_out_high                              (10*P_val(val_err_ac_out_high                ))/* 11 制热出温过高 */
#define val_forbid_on_env_cool                           (10*P_val(val_forbid_on_env_cool             ))/* 12 制冷禁开环温 */
#define val_out_in_diff_err                              (10*P_val(val_out_in_diff_err                ))/* 13 出回温差异常 */
#define val_gas_in_out_err_dly                              (P_val(val_gas_in_out_err_dly             ))/* 14 吸排异常检延 */
#define val_err_flux_open_pump                              (P_val(val_err_flux_open_pump             ))/* 15 水流不足开泵 */
#define val_invt_rps_force_sub_percent                      (P_val(val_invt_rps_force_sub_percent     ))/* 16 强制降频比例 */
#define val_invt_rps_force_sub_period                       (P_val(val_invt_rps_force_sub_period      ))/* 17 强制降频周期  */
#define val_out_in_diff_high                             (10*P_val(val_out_in_diff_high               ))/* 18 出回温差过大 */
#define val_evap_low_cool                                (10*P_val(val_evap_low_cool                  ))/* 19 制冷蒸发过低 */
#define val_gas_out_high                                 (10*P_val(val_gas_out_high                   ))/* 20 排气温度过高 */
#define ERR_WT_EXIT                                      (10*P_val(ERR_WT_EXIT                        ))/* 21 退排气温度过高(温差) */
#define val_gas_in_low_cool                              (10*P_val(val_gas_in_low_cool                ))/* 22 制冷吸气过低 */
#define val_pwm_speed_too_low                               (P_val(val_pwm_speed_too_low              ))/* 23 PWM风速过低 */
#define val_pwm_speed_check_dly                             (P_val(val_pwm_speed_check_dly            ))/* 24 PWM风速检测延时 */
#define val_check_lp_when_off                               (P_val(val_check_lp_when_off              ))/* 25 待机检测低压 */
#define val_TWout1_limit_1                               (10*P_val(val_TWout1_limit_1                 ))/* 26 制热出温限制1 */
#define val_TWout1_limit_2                               (10*P_val(val_TWout1_limit_2                 ))/* 27 制热出温限制2 */
#define val_TWout1_limit_3                               (10*P_val(val_TWout1_limit_3                 ))/* 28 制热出温限制3 */
#define val_TWout1_limit_4                               (10*P_val(val_TWout1_limit_4                 ))/* 29 制热出温限制4 */
#define val_TWout1_limit_5                               (10*P_val(val_TWout1_limit_5                 ))/* 30 制热出温限制5 */
#define val_TFLin_protect                                (10*P_val(val_TFLin_protect                  ))/* 31 地暖过热保护 */
#define val_lpcheck_evn_temp                             (10*P_val(val_lpcheck_evn_temp               ))/* 32 低压检查环温:低压低于此温度不检测低压开关 */
#define val_lp_value_incl                                   (P_val(val_lp_value_incl                  ))/* 33 制冷低压过低() */
#define val_lp_value_inht                                   (P_val(val_lp_value_inht                  ))/* 34 制热低压过低() */
#define val_hp_value_incl                                   (P_val(val_hp_value_incl                  ))/* 35 制冷高压过高() */
#define val_hp_value_inht                                   (P_val(val_hp_value_inht                  ))/* 36 制热高压过高() */
#define val_lp_prs_exit_diff                                (P_val(val_lp_prs_exit_diff               ))/* 37 低压报警复位压差 */
#define val_hp_prs_exit_diff                                (P_val(val_hp_prs_exit_diff               ))/* 38 高压报警复位压差 */
#define val_low_temp_cool_env                            (10*P_val(val_low_temp_cool_env              ))/* 39 低温制冷环温 */
#define val_low_temp_cool_dly                               (P_val(val_low_temp_cool_dly              ))/* 40 低温制冷检延 */
#define val_mac_set_check                                   (P_val(val_mac_set_check                  ))/* 41 机型设置校验 */
#define val_TWout1_limit_cool                            (10*P_val(val_TWout1_limit_cool              ))/* 42 制冷出温限制 */
#define val_use_UI35_commu_detect                           (P_val(val_use_UI35_commu_detect          ))/* 43 线控器通讯检测 */
#define val_invt_err_auto_reset 	                        (P_val(val_invt_err_auto_reset            ))/* 44 变频故障自复位 */
#define val_hotw_max_TBH                                    (P_val(val_hotw_max_TBH                   ))/* 45 电热热水上限 */
#define val_hp_pre_adju_diff                                (P_val(val_hp_pre_adju_diff               ))/* 46 高压辅调回差 */
#define val_lp_pre_adju_diff                                (P_val(val_lp_pre_adju_diff               ))/* 47 低压辅调回差 */

#define ERR_TEMP_DIFF                   (10*2)          /* 一般保护退出温差 */
#define TEMP_IN_DIFF                    (1*10)          /* 回温保护与出温保护的差值 */

// 11开关量及探头
#define P_val_DI_type_lp(f)                                 P_g_i(f,10, 0)	/* 压机低压 */
#define P_val_DI_type_hp(f)                                 P_g_i(f,10, 1)	/* 压机高压 */
#define P_val_DI_IBH_feedback(f)                            P_g_i(f,10, 2)	/* 内置电热反馈 */
#define P_val_DI_type_air_flux(f)                           P_g_i(f,10, 3)	/* 水流开关 */
#define P_val_DI_TBH_feedback(f)                            P_g_i(f,10, 4)	/* 水箱电热反馈 */
#define P_val_rfg_temp_corr_en(f)                           P_g_i(f,10, 5)	/* 冷媒温度修正 */
#define P_val_rfg_vap_corr_1(f)                             P_g_i(f,10, 6)	/* 冷媒气侧修正1 */
#define P_val_rfg_vap_corr_2(f)                             P_g_i(f,10, 7)	/* 冷媒气侧修正2 */
#define P_val_rfg_vap_corr_3(f)                             P_g_i(f,10, 8)	/* 冷媒气侧修正3 */
#define P_val_rfg_vap_corr_4(f)                             P_g_i(f,10, 9)	/* 冷媒气侧修正4 */
#define P_val_rfg_vap_corr_5(f)                             P_g_i(f,10,10)	/* 冷媒气侧修正5 */
#define P_val_rfg_liq_corr_1(f)                             P_g_i(f,10,11)	/* 冷媒液侧修正1 */
#define P_val_rfg_liq_corr_2(f)                             P_g_i(f,10,12)	/* 冷媒液侧修正2 */
#define P_val_rfg_liq_corr_3(f)                             P_g_i(f,10,13)	/* 冷媒液侧修正3 */
#define P_val_rfg_liq_corr_4(f)                             P_g_i(f,10,14)	/* 冷媒液侧修正4 */
#define P_val_rfg_liq_corr_5(f)                             P_g_i(f,10,15)	/* 冷媒液侧修正5 */
#define P_val_rfg_liq_corr_6(f)                             P_g_i(f,10,16)	/* 冷媒液侧修正6 */
#define P_val_rfg_liq_corr_7(f)                             P_g_i(f,10,17)	/* 冷媒液侧修正7 */
#define P_val_THWt_sensor_used(f)                           P_g_i(f,10,18)	/* 热水水箱温度探头 */
#define P_val_TFLin_sensor_used(f)                          P_g_i(f,10,19)	/* 地暖进水温度探头 */
#define P_val_TWout3_sensor_used(f)                         P_g_i(f,10,20)	/* 系统总出水温度探头 */
#define P_val_DI_type_EVU(f)                                P_g_i(f,10,21)	/* EVU信号 */
#define P_val_DI_type_SG(f)                                 P_g_i(f,10,22)	/* SG信号 */
#define P_val_ntc_sensor_config(f)                          P_g_i(f,10,23)	/* 温度探头配置 */
#define P_val_unit_LDIw_used(f)                             P_g_i(f,10,24)	/* 单元水流开关 */
#define P_val_TSL_sensor_used(f)                            P_g_i(f,10,25)	/* 太阳能温度探头 */
#define P_val_J5_prs_sensor_type(f)                         P_g_i(f,10,26)	/* J5压传类型() */
#define P_val_J6_prs_sensor_type(f)                         P_g_i(f,10,27)	/* J6压传类型() */
#define P_val_J5_scale_min(f)                               P_g_i(f,10,28)	/* J5最小量程() */
#define P_val_J5_scale_max(f)                               P_g_i(f,10,29)	/* J5最大量程() */
#define P_val_J6_scale_min(f)                               P_g_i(f,10,30)	/* J6最小量程() */
#define P_val_J6_scale_max(f)                               P_g_i(f,10,31)	/* J6最大量程() */
#define P_val_TACt1_sensor_used(f)                          P_g_i(f,10,32)	/* 缓冲水箱上部温度探头 */
#define P_val_TACt2_sensor_used(f)                          P_g_i(f,10,33)	/* 缓冲水箱下部温度探头 */
#define P_val_prs_sensor_config(f)                          P_g_i(f,10,34)	/* 压力传感器配置 */
#define P_val_power_fan(f)                                  P_g_i(f,10,35)	/* 风机功率  */
#define P_val_power_pump(f)                                 P_g_i(f,10,36)	/* 水泵功率  */
#define P_val_power_IBH1(f)                                 P_g_i(f,10,37)	/* 电热1功率 */
#define P_val_power_IBH2(f)                                 P_g_i(f,10,38)	/* 电热2功率 */
#define P_val_power_idle(f)                                 P_g_i(f,10,39)	/* 待机功率 */
#define P_val_power_other_run(f)                            P_g_i(f,10,40)	/* 其他运行功率 */
#define P_val_TWout2_sensor_used(f)                         P_g_i(f,10,41)	/* 总出水温度探头 */
#define val_DI_type_lp                                      (P_val(val_DI_type_lp                     ))/* 01 压机低压 */
#define val_DI_type_hp                                      (P_val(val_DI_type_hp                     ))/* 02 压机高压 */
#define val_DI_IBH_feedback                                 (P_val(val_DI_IBH_feedback                ))/* 03 内置电热反馈 */
#define val_DI_type_air_flux                                (P_val(val_DI_type_air_flux               ))/* 04 水流开关 */
#define val_DI_TBH_feedback                                 (P_val(val_DI_TBH_feedback                ))/* 05 水箱电热反馈 */
#define val_rfg_temp_corr_en                                (P_val(val_rfg_temp_corr_en               ))/* 06 冷媒温度修正 */
#define val_rfg_vap_corr_1                              (100*P_val(val_rfg_vap_corr_1                 ))/* 07 冷媒气侧修正1 */
#define val_rfg_vap_corr_2                              (100*P_val(val_rfg_vap_corr_2                 ))/* 08 冷媒气侧修正2 */
#define val_rfg_vap_corr_3                              (100*P_val(val_rfg_vap_corr_3                 ))/* 09 冷媒气侧修正3 */
#define val_rfg_vap_corr_4                              (100*P_val(val_rfg_vap_corr_4                 ))/* 10 冷媒气侧修正4 */
#define val_rfg_vap_corr_5                              (100*P_val(val_rfg_vap_corr_5                 ))/* 11 冷媒气侧修正5 */
#define val_rfg_liq_corr_1                              (100*P_val(val_rfg_liq_corr_1                 ))/* 12 冷媒液侧修正1 */
#define val_rfg_liq_corr_2                              (100*P_val(val_rfg_liq_corr_2                 ))/* 13 冷媒液侧修正2 */
#define val_rfg_liq_corr_3                              (100*P_val(val_rfg_liq_corr_3                 ))/* 14 冷媒液侧修正3 */
#define val_rfg_liq_corr_4                              (100*P_val(val_rfg_liq_corr_4                 ))/* 15 冷媒液侧修正4 */
#define val_rfg_liq_corr_5                              (100*P_val(val_rfg_liq_corr_5                 ))/* 16 冷媒液侧修正5 */
#define val_rfg_liq_corr_6                              (100*P_val(val_rfg_liq_corr_6                 ))/* 17 冷媒液侧修正6 */
#define val_rfg_liq_corr_7                              (100*P_val(val_rfg_liq_corr_7                 ))/* 18 冷媒液侧修正7 */
#define val_THWt_sensor_used                                (P_val(val_THWt_sensor_used               ))/* 19 热水水箱温度探头 */
#define val_TFLin_sensor_used                               (P_val(val_TFLin_sensor_used              ))/* 20 地暖进水温度探头 */
#define val_TWout3_sensor_used                              (P_val(val_TWout3_sensor_used             ))/* 21 系统总出水温度探头 */
#define val_DI_type_EVU                                     (P_val(val_DI_type_EVU                    ))/* 22 EVU信号 */
#define val_DI_type_SG                                      (P_val(val_DI_type_SG                     ))/* 23 SG信号 */
#define val_ntc_sensor_config                               (P_val(val_ntc_sensor_config              ))/* 24 温度探头配置 */
#define val_unit_LDIw_used                                  (P_val(val_unit_LDIw_used                 ))/* 25 单元水流开关 */
#define val_TSL_sensor_used                                 (P_val(val_TSL_sensor_used                ))/* 26 太阳能温度探头 */
#define val_J5_prs_sensor_type                              (P_val(val_J5_prs_sensor_type             ))/* 27 J5压传类型() */
#define val_J6_prs_sensor_type                              (P_val(val_J6_prs_sensor_type             ))/* 28 J6压传类型() */
#define val_J5_scale_min                                    (P_val(val_J5_scale_min                   ))/* 29 J5最小量程() */
#define val_J5_scale_max                                    (P_val(val_J5_scale_max                   ))/* 30 J5最大量程() */
#define val_J6_scale_min                                    (P_val(val_J6_scale_min                   ))/* 31 J6最小量程() */
#define val_J6_scale_max                                    (P_val(val_J6_scale_max                   ))/* 32 J6最大量程() */
#define val_TACt1_sensor_used                               (P_val(val_TACt1_sensor_used              ))/* 33 缓冲水箱上部温度探头 */
#define val_TACt2_sensor_used                               (P_val(val_TACt2_sensor_used              ))/* 34 缓冲水箱下部温度探头 */
#define val_prs_sensor_config                               (P_val(val_prs_sensor_config              ))/* 35 压力传感器配置 */
#define val_power_fan                                       (P_val(val_power_fan                      ))/* 36 风机功率  */
#define val_power_pump                                      (P_val(val_power_pump                     ))/* 37 水泵功率  */
#define val_power_IBH1                                      (P_val(val_power_IBH1                     ))/* 38 电热1功率 */
#define val_power_IBH2                                      (P_val(val_power_IBH2                     ))/* 39 电热2功率 */
#define val_power_idle                                      (P_val(val_power_idle                     ))/* 40 待机功率 */
#define val_power_other_run                                 (P_val(val_power_other_run                ))/* 41 其他运行功率 */
#define val_TWout2_sensor_used                              (P_val(val_TWout2_sensor_used             ))/* 42 总出水温度探头 */

/* 任一器件功率不为0即表示启用 */
#define USE_ELEC_METERING (val_power_fan     \
                           || val_power_pump \
                           || val_power_IBH1 \
                           || val_power_IBH2 \
                           || val_power_idle \
                           || val_power_other_run \
                           )

#ifdef USE_ODU_HL081B		/* HL081B */
/* 冷媒类型 */
enum {
    RF_R22,   
    RF_R407C, 
    RF_R410A, 
    RF_R32,	
    RF_R290,
    RF_MAX
};

enum {
    NTC_CFG_PUNP,           /* 常规: 邦普探头配置 */
    NTC_CFG_TCL,            /* 配置1: TCL专用探头配置 */
    NTC_CFG_CHANGHONG,      /* 配置2: 长虹专用探头配置 */
    NTC_CFG_PAIWO,			/* 配置3: 派沃专用探头配置 */
    NTC_CFG_NIUENTAI,		/* 配置4: 纽恩泰专用探头配置(50K3950/5K3470) */
    NTC_CFG_MAX,
};

/* 使用邦普探头 */
#define USE_PUNP_NTC                   (val_ntc_sensor_config == NTC_CFG_PUNP)
/* 使用TCL专用探头 */
#define USE_TCL_NTC                    (val_ntc_sensor_config == NTC_CFG_TCL)
/* 使用长虹专用探头 */
#define USE_CHANGHONG_NTC              (val_ntc_sensor_config == NTC_CFG_CHANGHONG)
/* 使用派沃专用探头 */
#define USE_PAIWO_NTC                  (val_ntc_sensor_config == NTC_CFG_PAIWO)
/* 使用纽恩泰专用探头 */
#define USE_NIUENTAI_NTC               (val_ntc_sensor_config == NTC_CFG_NIUENTAI)

#else						/* HL081A */
/* 冷媒类型 */
enum {
    RF_R22,   
    RF_R407C, 
    RF_R410A, 
    RF_R32,	
    RF_R290,
    RF_MAX
};

enum {
    NTC_CFG_PUNP,           /* 常规: 邦普探头配置 */
    NTC_CFG_TCL,            /* 配置1: TCL专用探头配置 */
    NTC_CFG_CHANGHONG,      /* 配置2: 长虹专用探头配置 */
    NTC_CFG_MAX,
};

/* 使用邦普探头 */
#define USE_PUNP_NTC                   (val_ntc_sensor_config == NTC_CFG_PUNP)
/* 使用TCL专用探头 */
#define USE_TCL_NTC                    (val_ntc_sensor_config == NTC_CFG_TCL)
/* 使用长虹专用探头 */
#define USE_CHANGHONG_NTC              (val_ntc_sensor_config == NTC_CFG_CHANGHONG)

#endif

/* 压力传感器配置 */
enum
{
    PRS_CFG_REGULAR,			/* 高低压复用: (仅用1个压传，放置于四通阀与板换间，制冷检测低压、制热检测高压，供主阀控制和风机调速使用) */
    PRS_CFG_SUCTION,			/* 仅低压: (仅用1个压传，位于压机吸气口，制冷/制热均检测低压，仅供主阀控制使用) */
    PRS_CFG_SUC_EXH,			/* 高低压独立: 低压+高压同时使用 */
    PRS_CFG_EXHAUST,			/* 仅高压 */
    PRS_CFG_UNUSED,			    /* 高低压禁用 */
    PRS_CFG_MAX,
};

/* 高低压复用 */
#define USE_REGULAR_PRS                (val_prs_sensor_config == PRS_CFG_REGULAR)
/* 仅低压 */
#define USE_SUCTION_PRS                (val_prs_sensor_config == PRS_CFG_SUCTION)
/* 高低压独立 */
#define USE_SUC_EXH_PRS                (val_prs_sensor_config == PRS_CFG_SUC_EXH)
/* 仅高压 */
#define USE_EXHAUST_PRS                (val_prs_sensor_config == PRS_CFG_EXHAUST)
/* 高低压禁用 */
#define USE_NO_PRS                     (val_prs_sensor_config == PRS_CFG_UNUSED)

/* 使用制冷低压 */
#define USE_COOL_LP (USE_REGULAR_PRS || USE_SUCTION_PRS || USE_SUC_EXH_PRS)

/* 使用总出水温度TWout2 */
#define USE_AC_TEMP_TWOUT2             (!val_TACt1_sensor_used && !val_TACt2_sensor_used)
/* 使用缓冲水箱上部温度TACt1 */
#define USE_AC_TEMP_TACT1              (val_TACt1_sensor_used && !val_TACt2_sensor_used)
/* 使用缓冲水箱下部温度TACt2 */
#define USE_AC_TEMP_TACT2              (!val_TACt1_sensor_used && val_TACt2_sensor_used)
/* 同时使用TACt1和TACt2 */
#define USE_AC_TEMP_TACT1_TACT2        (val_TACt1_sensor_used && val_TACt2_sensor_used)

/* 特殊模式 */
#define USE_SP_OUT_CTRL                (fg_sv_temp_energy_overwrite_ac && AC_TWOUT2_AC_CTRL)

/* 空调使用回水控制 */
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

/* 12调试及测试 */
// GB1：GB/T25127.2-2010
// GB2：GB18430.2-2016
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
#define AT_HT_WC_NUM    6			    /* 能力测试制热每组工况个数 */
#define AT_HT_PARA_NUM  42			    /* 能力测试制热每组参数个数 */
#define AT_HT_GP_ORIGIN 0xff		    /* 能力测试制热工况原始组号 */

enum {
    PFC_SET_OPEN=0,
    PFC_SET_CLOSE,
    PFC_SET_AUTO,
    PFC_SET_MAX
};

#define P_val_ability_test_set(f)                           P_g_i(f,11, 0)  //  能力测试选择
#define P_val_ability_test_invt_rps_HA(f)                   P_g_i(f,11, 1)  //  制热压机转速A
#define P_val_ability_test_fan_rpm_HA(f)                    P_g_i(f,11, 2)  //  制热风机转速A
#define P_val_ability_test_motor_ctrl_HA(f)                 P_g_i(f,11, 3)  //  制热主阀控制A
#define P_val_ability_test_invt_rps_HB(f)                   P_g_i(f,11, 4)  //  制热压机转速B
#define P_val_ability_test_fan_rpm_HB(f)                    P_g_i(f,11, 5)  //  制热风机转速B
#define P_val_ability_test_motor_ctrl_HB(f)                 P_g_i(f,11, 6)  //  制热主阀控制B
#define P_val_ability_test_invt_rps_HE(f)                   P_g_i(f,11, 7)  //  制热压机转速E
#define P_val_ability_test_fan_rpm_HE(f)                    P_g_i(f,11, 8)  //  制热风机转速E
#define P_val_ability_test_motor_ctrl_HE(f)                 P_g_i(f,11, 9)  //  制热主阀控制E
#define P_val_ability_test_invt_rps_HG(f)                   P_g_i(f,11,10)  //  制热压机转速G
#define P_val_ability_test_fan_rpm_HG(f)                    P_g_i(f,11,11)  //  制热风机转速G
#define P_val_ability_test_motor_ctrl_HG(f)                 P_g_i(f,11,12)  //  制热主阀控制G
#define P_val_ability_test_invt_rps_HC(f)                   P_g_i(f,11,13)  //  制热压机转速C
#define P_val_ability_test_fan_rpm_HC(f)                    P_g_i(f,11,14)  //  制热风机转速C
#define P_val_ability_test_motor_ctrl_HC(f)                 P_g_i(f,11,15)  //  制热主阀控制C
#define P_val_ability_test_invt_rps_HD(f)                   P_g_i(f,11,16)  //  制热压机转速D
#define P_val_ability_test_fan_rpm_HD(f)                    P_g_i(f,11,17)  //  制热风机转速D
#define P_val_ability_test_motor_ctrl_HD(f)                 P_g_i(f,11,18)  //  制热主阀控制D
#define P_val_ability_test_invt_rps_C(f)                    P_g_i(f,11,19)  //  制冷压机转速
#define P_val_ability_test_fan_rpm_C(f)                     P_g_i(f,11,20)  //  制冷风机转速
#define P_val_ability_test_motor_ctrl_C(f)                  P_g_i(f,11,21)  //  制冷主阀控制
#define P_val_ability_test_EWD_HA(f)                        P_g_i(f,11,22)  //  制热除霜环翅A
#define P_val_ability_test_interval_HA(f)                   P_g_i(f,11,23)  //  制热除霜间隔A
#define P_val_ability_test_step_HA(f)                       P_g_i(f,11,24)  //  制热除霜开度A
#define P_val_ability_test_freq_HA(f)                       P_g_i(f,11,25)  //  制热除霜转速A
#define P_val_ability_test_wing_HA(f)                       P_g_i(f,11,26)  //  制热退除霜翅温A
#define P_val_ability_test_defr_time_HA(f)                  P_g_i(f,11,27)  //  制热除霜时间A
#define P_val_ability_test_EWD_HB(f)                        P_g_i(f,11,28)  //  制热除霜环翅B
#define P_val_ability_test_interval_HB(f)                   P_g_i(f,11,29)  //  制热除霜间隔B
#define P_val_ability_test_step_HB(f)                       P_g_i(f,11,30)  //  制热除霜开度B
#define P_val_ability_test_freq_HB(f)                       P_g_i(f,11,31)  //  制热除霜转速B
#define P_val_ability_test_wing_HB(f)                       P_g_i(f,11,32)  //  制热退除霜翅温B
#define P_val_ability_test_defr_time_HB(f)                  P_g_i(f,11,33)  //  制热除霜时间B
#define P_val_ability_test_EWD_HE(f)                        P_g_i(f,11,34)  //  制热除霜环翅E
#define P_val_ability_test_interval_HE(f)                   P_g_i(f,11,35)  //  制热除霜间隔E
#define P_val_ability_test_step_HE(f)                       P_g_i(f,11,36)  //  制热除霜开度E
#define P_val_ability_test_freq_HE(f)                       P_g_i(f,11,37)  //  制热除霜转速E
#define P_val_ability_test_wing_HE(f)                       P_g_i(f,11,38)  //  制热退除霜翅温E
#define P_val_ability_test_defr_time_HE(f)                  P_g_i(f,11,39)  //  制热除霜时间E
#define P_val_ability_test_EWD_HG(f)                        P_g_i(f,11,40)  //  制热除霜环翅G
#define P_val_ability_test_interval_HG(f)                   P_g_i(f,11,41)  //  制热除霜间隔G
#define P_val_ability_test_step_HG(f)                       P_g_i(f,11,42)  //  制热除霜开度G
#define P_val_ability_test_freq_HG(f)                       P_g_i(f,11,43)  //  制热除霜转速G
#define P_val_ability_test_wing_HG(f)                       P_g_i(f,11,44)  //  制热退除霜翅温G
#define P_val_ability_test_defr_time_HG(f)                  P_g_i(f,11,45)  //  制热除霜时间G
#define P_val_ability_test_pfc_set(f)                       P_g_i(f,11,46)  //  PFC设置
#define P_val_debug_test_pump(f)                            P_g_i(f,11,47)  //  手动变频水泵
#define val_ability_test_set                                (P_val(val_ability_test_set               ))  //  能力测试选择
#define val_ability_test_invt_rps_HA                        (P_val(val_ability_test_invt_rps_HA       ))  //  制热压机转速A
#define val_ability_test_fan_rpm_HA                         (P_val(val_ability_test_fan_rpm_HA        ))  //  制热风机转速A
#define val_ability_test_motor_ctrl_HA                      (P_val(val_ability_test_motor_ctrl_HA     ))  //  制热主阀控制A
#define val_ability_test_invt_rps_HB                        (P_val(val_ability_test_invt_rps_HB       ))  //  制热压机转速B
#define val_ability_test_fan_rpm_HB                         (P_val(val_ability_test_fan_rpm_HB        ))  //  制热风机转速B
#define val_ability_test_motor_ctrl_HB                      (P_val(val_ability_test_motor_ctrl_HB     ))  //  制热主阀控制B
#define val_ability_test_invt_rps_HE                        (P_val(val_ability_test_invt_rps_HE       ))  //  制热压机转速E
#define val_ability_test_fan_rpm_HE                         (P_val(val_ability_test_fan_rpm_HE        ))  //  制热风机转速E
#define val_ability_test_motor_ctrl_HE                      (P_val(val_ability_test_motor_ctrl_HE     ))  //  制热主阀控制E
#define val_ability_test_invt_rps_HG                        (P_val(val_ability_test_invt_rps_HG       ))  //  制热压机转速G
#define val_ability_test_fan_rpm_HG                         (P_val(val_ability_test_fan_rpm_HG        ))  //  制热风机转速G
#define val_ability_test_motor_ctrl_HG                      (P_val(val_ability_test_motor_ctrl_HG     ))  //  制热主阀控制G
#define val_ability_test_invt_rps_HC                        (P_val(val_ability_test_invt_rps_HC       ))  //  制热压机转速C
#define val_ability_test_fan_rpm_HC                         (P_val(val_ability_test_fan_rpm_HC        ))  //  制热风机转速C
#define val_ability_test_motor_ctrl_HC                      (P_val(val_ability_test_motor_ctrl_HC     ))  //  制热主阀控制C
#define val_ability_test_invt_rps_HD                        (P_val(val_ability_test_invt_rps_HD       ))  //  制热压机转速D
#define val_ability_test_fan_rpm_HD                         (P_val(val_ability_test_fan_rpm_HD        ))  //  制热风机转速D
#define val_ability_test_motor_ctrl_HD                      (P_val(val_ability_test_motor_ctrl_HD     ))  //  制热主阀控制D
#define val_ability_test_invt_rps_C                         (P_val(val_ability_test_invt_rps_C        ))  //  制冷压机转速
#define val_ability_test_fan_rpm_C                          (P_val(val_ability_test_fan_rpm_C         ))  //  制冷风机转速
#define val_ability_test_motor_ctrl_C                       (P_val(val_ability_test_motor_ctrl_C      ))  //  制冷主阀控制
#define val_ability_test_EWD_HA                             (P_val(val_ability_test_EWD_HA            ))  //  制热除霜环翅A
#define val_ability_test_interval_HA                        (P_val(val_ability_test_interval_HA       ))  //  制热除霜间隔A
#define val_ability_test_step_HA                            (P_val(val_ability_test_step_HA           ))  //  制热除霜开度A
#define val_ability_test_freq_HA                            (P_val(val_ability_test_freq_HA           ))  //  制热除霜转速A
#define val_ability_test_wing_HA                            (P_val(val_ability_test_wing_HA           ))  //  制热退除霜翅温A
#define val_ability_test_defr_time_HA                       (P_val(val_ability_test_defr_time_HA      ))  //  制热除霜时间A
#define val_ability_test_EWD_HB                             (P_val(val_ability_test_EWD_HB            ))  //  制热除霜环翅B
#define val_ability_test_interval_HB                        (P_val(val_ability_test_interval_HB       ))  //  制热除霜间隔B
#define val_ability_test_step_HB                            (P_val(val_ability_test_step_HB           ))  //  制热除霜开度B
#define val_ability_test_freq_HB                            (P_val(val_ability_test_freq_HB           ))  //  制热除霜转速B
#define val_ability_test_wing_HB                            (P_val(val_ability_test_wing_HB           ))  //  制热退除霜翅温B
#define val_ability_test_defr_time_HB                       (P_val(val_ability_test_defr_time_HB      ))  //  制热除霜时间B
#define val_ability_test_EWD_HE                             (P_val(val_ability_test_EWD_HE            ))  //  制热除霜环翅E
#define val_ability_test_interval_HE                        (P_val(val_ability_test_interval_HE       ))  //  制热除霜间隔E
#define val_ability_test_step_HE                            (P_val(val_ability_test_step_HE           ))  //  制热除霜开度E
#define val_ability_test_freq_HE                            (P_val(val_ability_test_freq_HE           ))  //  制热除霜转速E
#define val_ability_test_wing_HE                            (P_val(val_ability_test_wing_HE           ))  //  制热退除霜翅温E
#define val_ability_test_defr_time_HE                       (P_val(val_ability_test_defr_time_HE      ))  //  制热除霜时间E
#define val_ability_test_EWD_HG                             (P_val(val_ability_test_EWD_HG            ))  //  制热除霜环翅G
#define val_ability_test_interval_HG                        (P_val(val_ability_test_interval_HG       ))  //  制热除霜间隔G
#define val_ability_test_step_HG                            (P_val(val_ability_test_step_HG           ))  //  制热除霜开度G
#define val_ability_test_freq_HG                            (P_val(val_ability_test_freq_HG           ))  //  制热除霜转速G
#define val_ability_test_wing_HG                            (P_val(val_ability_test_wing_HG           ))  //  制热退除霜翅温G
#define val_ability_test_defr_time_HG                       (P_val(val_ability_test_defr_time_HG      ))  //  制热除霜时间G
#define val_ability_test_pfc_set                            (P_val(val_ability_test_pfc_set           ))  //  PFC设置
#define val_debug_test_pump                                 (P_val(val_debug_test_pump                ))  //  手动变频水泵


#define val_ability_test_invt_rps           (para_AT_use_get()==0 ? 0 \
                                            : 100*para_C[GROUP_DEBUG][1+3*(para_AT_use_get()-1)]) // 压机测试转速
#define val_ability_test_fan_rpm            (para_AT_use_get()==0 ? 0 \
                                            : para_C[GROUP_DEBUG][2+3*(para_AT_use_get()-1)]) // 风机测试风速
#define val_ability_test_motor_ctrl         (para_AT_use_get()==0 ? 0 \
                                            : para_C[GROUP_DEBUG][3+3*(para_AT_use_get()-1)]) // 主阀测试控制
#define val_ability_test_s_motor_ctrl       (para_AT_use_get()==0 ? 0 \
                                            : para_C[GROUP_EEV_S][39+(para_AT_use_get()-1)]) // 辅阀测试控制

#define USE_ABILITY_EEV_SSH_SV              (fg_ability_test_run \
                                            && fun_pv_in(val_ability_test_motor_ctrl, -200, 800))
#define USE_ABILITY_EEV_STEP                (fg_ability_test_run \
                                            && val_ability_test_motor_ctrl>1000)
#define val_ability_test_eev_ssh_sv         (val_ability_test_motor_ctrl)       // 主阀测试过热
#define val_ability_test_motor_steps        (val_ability_test_motor_ctrl-1000)  // 主阀测试步数

#define USE_ABILITY_EVI_SSH_SV              (fg_ability_test_run \
                                            && fun_pv_in(val_ability_test_s_motor_ctrl, -200, 800))
#define USE_ABILITY_EVI_STEP                (fg_ability_test_run \
                                            && val_ability_test_s_motor_ctrl>1000)
#define val_ability_test_evi_ssh_sv         (val_ability_test_s_motor_ctrl)       // 辅阀测试过热
#define val_ability_test_s_motor_steps      (val_ability_test_s_motor_ctrl-1000)  // 辅阀测试步数


#define USE_DEBUG_PUMP                      (val_debug_test_pump>0)

#define val_ability_test_power              (0) // 制热能力变化量：暂禁用

#define val_ability_test_flow               (20) // 制热水流量

#define val_ability_test_EWD                ((para_AT_use_get()==0 || para_AT_use_get()>=5) ? 0\
                                            : 10*para_C[GROUP_DEBUG][22+6*(para_AT_use_get()-1)]) // 制热除霜环翅

#define val_ability_test_interval           ((para_AT_use_get()==0 || para_AT_use_get()>=5) ? 0\
                                            : 60*para_C[GROUP_DEBUG][23+6*(para_AT_use_get()-1)]) // 制热除霜间隔

#define val_ability_test_step               ((para_AT_use_get()==0 || para_AT_use_get()>=5) ? 0\
                                            : para_C[GROUP_DEBUG][24+6*(para_AT_use_get()-1)]) // 制热除霜开度

#define val_ability_test_freq               ((para_AT_use_get()==0 || para_AT_use_get()>=5) ? val_invt_rps_defrost\
                                            : 100*para_C[GROUP_DEBUG][25+6*(para_AT_use_get()-1)]) // 制热除霜转速

#define val_ability_test_wing               ((para_AT_use_get()==0 || para_AT_use_get()>=5) ? 0\
                                            : 10*para_C[GROUP_DEBUG][26+6*(para_AT_use_get()-1)]) // 制热退除霜翅温

#define val_ability_test_defr_time          ((para_AT_use_get()==0 || para_AT_use_get()>=5) ? 0\
                                            : para_C[GROUP_DEBUG][27+6*(para_AT_use_get()-1)]) // 制热除霜时间

#define ABILITY_TEST_HAS_DEFR               (fg_ability_test_run && val_ability_test_step!=0) /* 除霜的工况 */
#define ABILITY_TEST_NO_DEFR                (fg_ability_test_run && val_ability_test_step==0) /* 不除霜的工况 */


#define AT_VAL(v1, v2)                      ((fg_ability_test_run&&(v2)>0) ? (v2) : (v1))

#define val_ability_test_evi_steps          (0)


#define val_gb_dbg_use           (GB_DBG_UNUSED) //  1 国标调试设置
#define val_drytemp_cpst                     (0) //  2 干球温度补偿
#define val_invt_rps_heatrun_freq1      (100*30) //  3 制热转速1    GB1     环温7℃
#define val_invt_rps_heatrun_freq2      (100*30) //  4 制热转速2    GB1     环温0℃
#define val_invt_rps_heatrun_freq3      (100*45) //  5 制热转速3    GB1     环温-6℃
#define val_invt_rps_heatrun_freq4      (100*75) //  6 制热转速4    GB1     环温-12℃
#define val_debug_sh_sv1                    (10) //  7 制热过热1    GB1     环温7℃
#define val_debug_sh_sv2                    (10) //  8 制热过热2    GB1     环温0℃
#define val_debug_sh_sv3                    (20) //  9 制热过热3    GB1     环温-6℃
#define val_debug_sh_sv4                    (20) // 10 制热过热4    GB1     环温-12℃
#define val_invt_rps_coolrun_freq1      (100*75) // 11 制冷转速1    GB1 GB2 环温35℃
#define val_invt_rps_coolrun_freq2      (100*53) // 12 制冷转速2    GB1 GB2 环温31.5℃
#define val_invt_rps_coolrun_freq3      (100*35) // 13 制冷转速3    GB1 GB2 环温28℃
#define val_invt_rps_coolrun_freq4      (100*30) // 14 制冷转速4    GB1 GB2 环温24.5℃
#define val_debug_sc_sv1                    (20) // 15 制冷过热1    GB1 GB2 环温35℃
#define val_debug_sc_sv2                    (20) // 16 制冷过热2    GB1 GB2 环温31.5℃
#define val_debug_sc_sv3                    (20) // 17 制冷过热3    GB1 GB2 环温28℃
#define val_debug_sc_sv4                    (20) // 18 制冷过热4    GB1 GB2 环温24.5℃
#define val_invt_rps_sheatrun_freq      (100*68) // 19 制热标况转速 GB2     环温7℃
#define val_debug_ssh_sv                    (10) // 20 制热标况过热 GB2     环温7℃

#define USE_GB_DBG      (val_gb_dbg_use != GB_DBG_UNUSED)
#define USE_GB_25127    (val_gb_dbg_use == GB_DBG_25127)
#define USE_GB_18430    (val_gb_dbg_use == GB_DBG_18430)

#if defined(M_TCL)||defined(M_CHANGHONG)
#define COOL_RUN_ENV_LLIMIT         (-14)   /* 制冷下限环温 */
#define COOL_RUN_ENV_HLIMIT         (52)    /* 制冷上限环温 */
#define HEAT_RUN_ENV_LLIMIT         (-28)   /* 制热下限环温 */
#define HEAT_RUN_ENV_HLIMIT         (52)    /* 制热上限环温 */
#define HOT_WATER_RUN_ENV_LLIMIT    (-28)   /* 热水下限环温 */
#define HOT_WATER_RUN_ENV_HLIMIT    (52)    /* 热水上限环温 */
#else
#define COOL_RUN_ENV_LLIMIT         (-14)   /* 制冷下限环温 */
#define COOL_RUN_ENV_HLIMIT         (72)    /* 制冷上限环温 */
#define HEAT_RUN_ENV_LLIMIT         (-52)   /* 制热下限环温 */
#define HEAT_RUN_ENV_HLIMIT         (72)    /* 制热上限环温 */
#define HOT_WATER_RUN_ENV_LLIMIT    (-52)   /* 热水下限环温 */
#define HOT_WATER_RUN_ENV_HLIMIT    (72)    /* 热水上限环温 */
#endif

/* 13能量控制 */
#define P_val_ctrl_period(f)                                P_g_i(f,12, 0)	/* 能量控制周期 */
#define P_val_cool_add_diff(f)                              P_g_i(f,12, 1)	/* 制冷加载偏差(度) */
#define P_val_cool_sub_diff(f)                              P_g_i(f,12, 2)	/* 制冷卸载偏差(度) */
#define P_val_heat_add_diff(f)                              P_g_i(f,12, 3)	/* 制热加载偏差(度) */
#define P_val_heat_sub_diff(f)                              P_g_i(f,12, 4)	/* 制热卸载偏差(度) */
#define P_val_TR_diff(f)                                    P_g_i(f,12, 5)	/* 室温回差 */
#define P_val_cool_set_min(f)                               P_g_i(f,12, 6)	/* 制冷回温下限 */
#define P_val_cool_set_max(f)                               P_g_i(f,12, 7)	/* 制冷回温上限 */
#define P_val_heat_set_min(f)                               P_g_i(f,12, 8)	/* 制热回温下限 */
#define P_val_heat_set_max(f)                               P_g_i(f,12, 9)	/* 制热回温上限 */
#define P_val_cool_min_TA(f)                                P_g_i(f,12,10)	/* 制冷最低环温 */
#define P_val_cool_max_TA(f)                                P_g_i(f,12,11)	/* 制冷最高环温 */
#define P_val_heat_min_TA(f)                                P_g_i(f,12,12)	/* 制热最低环温 */
#define P_val_heat_max_TA(f)                                P_g_i(f,12,13)	/* 制热最高环温 */
#define P_val_hotw_min_TA(f)                                P_g_i(f,12,14)	/* 热水最低环温 */
#define P_val_hotw_max_TA(f)                                P_g_i(f,12,15)	/* 热水最高环温 */
#define P_val_auto_cool_min_TA(f)                           P_g_i(f,12,16)	/* 自动制冷最低环 */
#define P_val_auto_heat_max_TA(f)                           P_g_i(f,12,17)	/* 自动制热最高环 */
#define P_val_holiday_away_sv_heat(f)                       P_g_i(f,12,18)	/* 假日离家制热 */
#define P_val_holiday_away_sv_hotw(f)                       P_g_i(f,12,19)	/* 假日离家热水 */
#define P_val_eco_run_time(f)                               P_g_i(f,12,20)	/* 节能运行时间 */
#define P_val_use_IBH(f)                                    P_g_i(f,12,21)	/* 辅助电热 */
#define P_val_use_AHS(f)                                    P_g_i(f,12,22)	/* 外部热源 */
#define P_val_IBH_high_dT(f)                                P_g_i(f,12,23)	/* 辅热开启高温差 */
#define P_val_IBH_low_dT(f)                                 P_g_i(f,12,24)	/* 辅热开启低温差 */
#define P_val_IBH_gear_num(f)                               P_g_i(f,12,25)	/* 辅助电热调档 */
#define P_val_IBH_open_dly(f)                               P_g_i(f,12,26)	/* 辅助电热开启延 */
#define P_val_IBH_open_env(f)                               P_g_i(f,12,27)	/* 辅助电热开环温 */
#define P_val_IBH_open_diff(f)                              P_g_i(f,12,28)	/* 辅助电热开温差 */
#define P_val_TBH_open_dly(f)                               P_g_i(f,12,29)	/* 水箱电热开启延 */
#define P_val_TBH_on_TA(f)                                  P_g_i(f,12,30)	/* 水箱电热开环温 */
#define P_val_AHS_open_dly(f)                               P_g_i(f,12,31)	/* 外部热源开启延 */
#define P_val_AHS_open_diff(f)                              P_g_i(f,12,32)	/* 外部热源开温差 */
#define P_val_AHS_close_diff(f)                             P_g_i(f,12,33)	/* 外部热源关温差 */
#define P_val_AHS_open_env(f)                               P_g_i(f,12,34)	/* 外部热源开环温 */
#define P_val_hotw_comp_on_max(f)                           P_g_i(f,12,35)	/* 制热水最大时长 */
#define P_val_air_comp_on_max(f)                            P_g_i(f,12,36)	/* 制热水限制运行 */
#define P_val_fcu_egy_cpst_obj(f)                           P_g_i(f,12,37)  /* 能调修正对象 */
#define P_val_cm_temp_diff(f)                               P_g_i(f,12,38)	/* 压机全开温差 */
#define P_val_TF_heat_step(f)                               P_g_i(f,12,39)	/* 商检制热步数 */
#define P_val_TF_heat_freq(f)                               P_g_i(f,12,40)	/* 商检制热频率 */
#define P_val_TF_heat_time(f)                               P_g_i(f,12,41)	/* 商检制热时间 */
#define P_val_TF_cool_step(f)                               P_g_i(f,12,42)	/* 商检制冷步数 */
#define P_val_TF_cool_freq(f)                               P_g_i(f,12,43)	/* 商检制冷频率 */
#define P_val_TF_cool_time(f)                               P_g_i(f,12,44)	/* 商检制冷时间 */
#define P_val_TF_IBH_time(f)                                P_g_i(f,12,45)	/* 商检电热时间 */
#define P_val_TF_heat_fan(f)                                P_g_i(f,12,46)	/* 商检制热风速 */
#define P_val_TF_cool_fan(f)                                P_g_i(f,12,47)	/* 商检制冷风速 */
#define val_ctrl_period                                     (P_val(val_ctrl_period                    ))/* 01 能量控制周期 */
#define val_cool_add_diff                                   (P_val(val_cool_add_diff                  ))/* 02 制冷加载偏差(度) */
#define val_cool_sub_diff                                   (P_val(val_cool_sub_diff                  ))/* 03 制冷卸载偏差(度) */
#define val_heat_add_diff                                   (P_val(val_heat_add_diff                  ))/* 04 制热加载偏差(度) */
#define val_heat_sub_diff                                   (P_val(val_heat_sub_diff                  ))/* 05 制热卸载偏差(度) */
#define val_TR_diff                                      (10*P_val(val_TR_diff                        ))/* 06 室温回差 */
#define val_cool_set_min                                    (P_val(val_cool_set_min                   ))/* 07 制冷回温下限 */
#define val_cool_set_max                                    (P_val(val_cool_set_max                   ))/* 08 制冷回温上限 */
#define val_heat_set_min                                    (P_val(val_heat_set_min                   ))/* 09 制热回温下限 */
#define val_heat_set_max                                    (P_val(val_heat_set_max                   ))/* 10 制热回温上限 */
#define val_cool_min_TA                                  (10*P_val(val_cool_min_TA                    ))/* 11 制冷最低环温 */
#define val_cool_max_TA                                  (10*P_val(val_cool_max_TA                    ))/* 12 制冷最高环温 */
#define val_heat_min_TA                                  (10*P_val(val_heat_min_TA                    ))/* 13 制热最低环温 */
#define val_heat_max_TA                                  (10*P_val(val_heat_max_TA                    ))/* 14 制热最高环温 */
#define val_hotw_min_TA                                  (10*P_val(val_hotw_min_TA                    ))/* 15 热水最低环温 */
#define val_hotw_max_TA                                  (10*P_val(val_hotw_max_TA                    ))/* 16 热水最高环温 */
#define val_auto_cool_min_TA                             (10*P_val(val_auto_cool_min_TA               ))/* 17 自动制冷最低环 */
#define val_auto_heat_max_TA                             (10*P_val(val_auto_heat_max_TA               ))/* 18 自动制热最高环 */
#define val_holiday_away_sv_heat                         (10*P_val(val_holiday_away_sv_heat           ))/* 19 假日离家制热 */
#define val_holiday_away_sv_hotw                         (10*P_val(val_holiday_away_sv_hotw           ))/* 20 假日离家热水 */
#define val_eco_run_time                              (QK_60*P_val(val_eco_run_time                   ))/* 21 节能运行时间 */
#define val_use_IBH                                         (P_val(val_use_IBH                        ))/* 22 辅助电热 */
#define val_use_AHS                                         (P_val(val_use_AHS                        ))/* 23 外部热源 */
#define val_IBH_high_dT                                  (10*P_val(val_IBH_high_dT                    ))/* 24 辅热开启高温差 */
#define val_IBH_low_dT                                   (10*P_val(val_IBH_low_dT                     ))/* 25 辅热开启低温差 */
#define val_IBH_gear_num                                    (P_val(val_IBH_gear_num                   ))/* 26 辅助电热调档 */
#define val_IBH_open_dly                              (QK_60*P_val(val_IBH_open_dly                   ))/* 27 辅助电热开启延 */
#define val_IBH_open_env                                 (10*P_val(val_IBH_open_env                   ))/* 28 辅助电热开环温 */
#define val_IBH_open_diff                                (10*P_val(val_IBH_open_diff                  ))/* 29 辅助电热开温差 */
#define val_TBH_open_dly                              (QK_60*P_val(val_TBH_open_dly                   ))/* 30 水箱电热开启延 */
#define val_TBH_on_TA                                    (10*P_val(val_TBH_on_TA                      ))/* 31 水箱电热开环温 */
#define val_AHS_open_dly                              (QK_60*P_val(val_AHS_open_dly                   ))/* 32 外部热源开启延 */
#define val_AHS_open_diff                                (10*P_val(val_AHS_open_diff                  ))/* 33 外部热源开温差 */
#define val_AHS_close_diff                               (10*P_val(val_AHS_close_diff                 ))/* 34 外部热源关温差 */
#define val_AHS_open_env                                 (10*P_val(val_AHS_open_env                   ))/* 35 外部热源开环温 */
#define val_hotw_comp_on_max                          (QK_60*P_val(val_hotw_comp_on_max               ))/* 36 制热水最大时长 */
#define val_air_comp_on_max                           (QK_60*P_val(val_air_comp_on_max                ))/* 37 制热水限制运行 */
#define val_fcu_egy_cpst_obj                                (P_val(val_fcu_egy_cpst_obj               ))/* 38 能调修正对象 */
#define val_cm_temp_diff                                 (10*P_val(val_cm_temp_diff                   ))/* 39 压机全开温差 */
#define val_TF_heat_step                                    (P_val(val_TF_heat_step                   ))/* 40 商检制热步数 */
#define val_TF_heat_freq                                (100*P_val(val_TF_heat_freq                   ))/* 41 商检制热频率 */
#define val_TF_heat_time                                    (P_val(val_TF_heat_time                   ))/* 42 商检制热时间 */
#define val_TF_cool_step                                    (P_val(val_TF_cool_step                   ))/* 43 商检制冷步数 */
#define val_TF_cool_freq                                (100*P_val(val_TF_cool_freq                   ))/* 44 商检制冷频率 */
#define val_TF_cool_time                                    (P_val(val_TF_cool_time                   ))/* 45 商检制冷时间 */
#define val_TF_IBH_time                                     (P_val(val_TF_IBH_time                    ))/* 46 商检电热时间 */
#define val_TF_heat_fan                                     (P_val(val_TF_heat_fan                    ))/* 47 商检制热风速 */
#define val_TF_cool_fan                                     (P_val(val_TF_cool_fan                    ))/* 48 商检制冷风速 */

/* 制热使用辅助电热 */
#define HEAT_USE_IBH                   (((val_use_IBH==IBH_HEAT_ONLY) || (val_use_IBH==IBH_HEAT_HOTW)) && (IBH_cfg_type!=IBH_NONE))
/* 制热水使用辅助电热 */
#define HOTW_USE_IBH                   (((val_use_IBH==IBH_HOTW_ONLY) || (val_use_IBH==IBH_HEAT_HOTW)) && (IBH_cfg_type!=IBH_NONE))
/* 制热使用外部热源 */
#define HEAT_USE_AHS                    ((val_use_AHS==AHS_HEAT_ONLY) || (val_use_AHS==AHS_HEAT_HOTW))
/* 制热水使用外部热源 */
#define HOTW_USE_AHS                    ((val_use_AHS==AHS_HOTW_ONLY) || (val_use_AHS==AHS_HEAT_HOTW))


/* 14工程参数 */
#define P_val_PUMPf_ctrl(f)                                 P_g_i(f,13, 0)	/* 变频水泵控制 */
#define P_val_bak_13_02(f)                                  P_g_i(f,13, 1)	/* 备用 */
#define P_val_pump_sv(f)                                    P_g_i(f,13, 2)	/* 水泵目标转速 */
#define P_val_PUMPf_model(f)                                P_g_i(f,13, 3)	/* 变频水泵编码 */
#define P_val_pump_sv_flow(f)                               P_g_i(f,13, 4)	/* 水泵目标流量 */
#define P_val_PUMPret_run_time(f)                           P_g_i(f,13, 5)	/* 下回水泵运行 */
#define P_val_PUMPret_disinfect(f)                          P_g_i(f,13, 6)	/* 下回水泵杀菌 */
#define P_val_PUMPret_timer(f)                              P_g_i(f,13, 7)	/* 下回水泵定时 */
#define P_val_floor_preheat_temp(f)                         P_g_i(f,13, 8)	/* 地暖预热温度 */
#define P_val_floor_preheat_interval(f)                     P_g_i(f,13, 9)	/* 地暖预热间隔 */
#define P_val_floor_preheat_max(f)                          P_g_i(f,13,10)	/* 地暖预热时长 */
#define P_val_S3V3_on_TFLin_dT(f)                           P_g_i(f,13,11)	/* 地暖水温回差 */
#define P_val_S3V3_on_TR_dT(f)                              P_g_i(f,13,12)	/* 地暖室温回差 */
#define P_val_floor_dry_head(f)                             P_g_i(f,13,13)	/* 地暖烘干前 */
#define P_val_floor_dry_mid(f)                              P_g_i(f,13,14)	/* 地暖烘干中 */
#define P_val_floor_dry_tail(f)                             P_g_i(f,13,15)	/* 地暖烘干后 */
#define P_val_floor_dry_temp(f)                             P_g_i(f,13,16)	/* 地暖烘干温度 */
#define P_val_zoneA_cl_terminal_device(f)                   P_g_i(f,13,17)	/* A区制冷末端 */
#define P_val_zoneA_ht_terminal_device(f)                   P_g_i(f,13,18)	/* A区制热末端 */
#define P_val_zoneB_cl_terminal_device(f)                   P_g_i(f,13,19)	/* B区制冷末端 */
#define P_val_zoneB_ht_terminal_device(f)                   P_g_i(f,13,20)	/* B区制热末端 */
#define P_val_fcu_num(f)                                    P_g_i(f,13,21)  /* 末端个数    */
#define P_val_fcu_base_addr(f)                              P_g_i(f,13,22)  /* 末端起始地址 */
#define P_val_fcu_M_start_S(f)                              P_g_i(f,13,23)  /* 主机开启末端 */
#define P_val_fcu_M_close_S(f)                              P_g_i(f,13,24)  /* 主机关闭末端 */
#define P_val_fcu_S_start_M(f)                              P_g_i(f,13,25)  /* 末端开启主机 */
#define P_val_fcu_mode_set(f)                               P_g_i(f,13,26)  /* 主末模式设置 */
#define P_val_fcu_egy_cpst(f)                               P_g_i(f,13,27)  /* 末端能调修正 */
#define P_val_PUMPsl_on_TWHt(f)                             P_g_i(f,13,28)	/* 太阳能开启水温 */
#define P_val_PUMPsl_THWt_dT(f)                             P_g_i(f,13,29)	/* 太阳能回差 */
#define P_val_PUMPsl_on_TSL(f)                              P_g_i(f,13,30)	/* 太阳能开启水温 */
#define P_val_PUMPsl_TSL_dT(f)                              P_g_i(f,13,31)	/* 太阳能开启回差 */
#define P_val_recycle_enter(f)                              P_g_i(f,13,32)  /* 收氟功能设置 */
#define P_val_recycle_eev_step(f)                           P_g_i(f,13,33)  /* 收氟主阀步数 */
#define P_val_recycle_lp_dly(f)                             P_g_i(f,13,34)  /* 收氟低压延退 */
#define P_val_S3V3_act_period(f)                            P_g_i(f,13,35)	/* S3V3动作周期 */
#define P_val_S3V3_ext_circue_min_time(f)                   P_g_i(f,13,36)	/* 外循环最小时间 */
#define P_val_bak_13_38(f)                                  P_g_i(f,13,37)	/* 备用 */
#define P_val_bak_13_39(f)                                  P_g_i(f,13,38)	/* 备用 */
#define P_val_pcb_num(f)                                    P_g_i(f,13,39)	/* 模块个数 */
#define P_val_hotw_pcb_num(f)                               P_g_i(f,13,40)	/* 热水模块数 */
#define P_val_bak_13_42(f)                                  P_g_i(f,13,41)	/* 备用 */
#define P_val_bak_13_43(f)                                  P_g_i(f,13,42)	/* 备用 */
#define P_val_bak_13_44(f)                                  P_g_i(f,13,43)	/* 备用 */
#define P_val_bak_13_45(f)                                  P_g_i(f,13,44)	/* 备用 */
#define P_val_bms_format(f)                                 P_g_i(f,13,45)	/* BMS数据格式 */
#define P_val_bms_baud_rate(f)                              P_g_i(f,13,46)	/* BMS波特率 */
#define P_val_bms_addr(f)                                   P_g_i(f,13,47)	/* BMS通讯地址 */
#define val_PUMPf_ctrl                                      (P_val(val_PUMPf_ctrl                     ))/* 01 变频水泵控制 */
#define val_bak_13_02                                       (P_val(val_bak_13_02                      ))/* 02 备用 */
#define val_pump_sv                                         (P_val(val_pump_sv                        ))/* 03 水泵目标转速 */
#define val_PUMPf_model                                     (P_val(val_PUMPf_model                    ))/* 04 变频水泵编码 */
#define val_pump_sv_flow                                    (P_val(val_pump_sv_flow                   ))/* 05 水泵目标流量 */
#define val_PUMPret_run_time                             (60*P_val(val_PUMPret_run_time               ))/* 06 下回水泵运行 */
#define val_PUMPret_disinfect                               (P_val(val_PUMPret_disinfect              ))/* 07 下回水泵杀菌 */
#define val_PUMPret_timer                                   (P_val(val_PUMPret_timer                  ))/* 08 下回水泵定时 */
#define val_floor_preheat_temp                           (10*P_val(val_floor_preheat_temp             ))/* 09 地暖预热温度 */
#define val_floor_preheat_interval                    (QK_60*P_val(val_floor_preheat_interval         ))/* 10 地暖预热间隔 */
#define val_floor_preheat_max                               (P_val(val_floor_preheat_max              ))/* 11 地暖预热时长 */
#define val_S3V3_on_TFLin_dT                             (10*P_val(val_S3V3_on_TFLin_dT               ))/* 12 地暖水温回差 */
#define val_S3V3_on_TR_dT                                (10*P_val(val_S3V3_on_TR_dT                  ))/* 13 地暖室温回差 */
#define val_floor_dry_head                                  (P_val(val_floor_dry_head                 ))/* 14 地暖烘干前 */
#define val_floor_dry_mid                                   (P_val(val_floor_dry_mid                  ))/* 15 地暖烘干中 */
#define val_floor_dry_tail                                  (P_val(val_floor_dry_tail                 ))/* 16 地暖烘干后 */
#define val_floor_dry_temp                               (10*P_val(val_floor_dry_temp                 ))/* 17 地暖烘干温度 */
#define val_zoneA_cl_terminal_device                        (P_val(val_zoneA_cl_terminal_device       ))/* 18 A区制冷末端 */
#define val_zoneA_ht_terminal_device                        (P_val(val_zoneA_ht_terminal_device       ))/* 19 A区制热末端 */
#define val_zoneB_cl_terminal_device                        (P_val(val_zoneB_cl_terminal_device       ))/* 20 B区制冷末端 */
#define val_zoneB_ht_terminal_device                        (P_val(val_zoneB_ht_terminal_device       ))/* 21 B区制热末端 */
#define val_fcu_num                                         (P_val(val_fcu_num                        ))/* 22 末端个数    */
#define val_fcu_base_addr                                   (P_val(val_fcu_base_addr                  ))/* 23 末端起始地址 */
#define val_fcu_M_start_S                                   (P_val(val_fcu_M_start_S                  ))/* 24 主机开启末端 */
#define val_fcu_M_close_S                                   (P_val(val_fcu_M_close_S                  ))/* 25 主机关闭末端 */
#define val_fcu_S_start_M                                   (P_val(val_fcu_S_start_M                  ))/* 26 末端开启主机 */
#define val_fcu_mode_set                                    (P_val(val_fcu_mode_set                   ))/* 27 主末模式设置 */
#define val_fcu_egy_cpst                                 (10*P_val(val_fcu_egy_cpst                   ))/* 28 末端能调修正 */
#define val_PUMPsl_on_TWHt                               (10*P_val(val_PUMPsl_on_TWHt                 ))/* 29 太阳能热水水温 */
#define val_PUMPsl_THWt_dT                               (10*P_val(val_PUMPsl_THWt_dT                 ))/* 30 太阳能热水回差 */
#define val_PUMPsl_on_TSL                                (10*P_val(val_PUMPsl_on_TSL                  ))/* 31 太阳能开启水温 */
#define val_PUMPsl_TSL_dT                                (10*P_val(val_PUMPsl_TSL_dT                  ))/* 32 太阳能开启回差 */
#define val_recycle_enter                                   (P_val(val_recycle_enter                  ))/* 33 收氟功能设置 */
#define val_recycle_eev_step                                (P_val(val_recycle_eev_step               ))/* 34 收氟主阀步数 */
#define val_recycle_lp_dly                                  (P_val(val_recycle_lp_dly                 ))/* 35 收氟低压延退 */
#define val_S3V3_act_period                                 (P_val(val_S3V3_act_period                ))/* 36 S3V3动作周期 */
#define val_S3V3_ext_circue_min_time                        (P_val(val_S3V3_ext_circue_min_time       ))/* 37 外循环最小时间 */
#define val_bak_13_38                                       (P_val(val_bak_13_38                      ))/* 38 备用 */
#define val_bak_13_39                                       (P_val(val_bak_13_39                      ))/* 39 备用 */
#define val_pcb_num                                         (P_val(val_pcb_num                        ))/* 40 模块个数 */
#define val_hotw_pcb_num                                    (P_val(val_hotw_pcb_num                   ))/* 41 热水模块数 */
#define val_bak_13_42                                       (P_val(val_bak_13_42                      ))/* 42 备用 */
#define val_bak_13_43                                       (P_val(val_bak_13_43                      ))/* 43 备用 */
#define val_bak_13_44                                       (P_val(val_bak_13_44                      ))/* 44 备用 */
#define val_bak_13_45                                       (P_val(val_bak_13_45                      ))/* 45 备用 */
#define val_bms_format                                      (P_val(val_bms_format                     ))/* 46 BMS数据格式 */
#define val_bms_baud_rate                                   (P_val(val_bms_baud_rate                  ))/* 47 BMS波特率 */
#define val_bms_addr                                        (P_val(val_bms_addr                       ))/* 48 BMS通讯地址 */

#define val_PUMPf_init_stable                               (10)	/* 水泵初始维持 */
#define val_PUMPf_ctrl_period                               (3)		/* 水泵调速周期 */
#define val_PUMPf_flow_diff                                 (50)	/* 水泵流量回差 */
#define val_PUMPf_power_diff                                (5)	    /* 水泵功率回差 */

#define val_ster_start_week                                 0/* 14 杀菌启动星期 */
#define val_ster_start_hour                                 0/* 15 杀菌启动时 */
#define val_ster_start_minute                               0/* 16 杀菌启动分 */

#define S3V_MIN_PERIOD  360		/* 三通阀最小动作周期 */
#define S3V_MIN_ON      20		/* 三通阀最小开启时间 */
#define S3V_MIN_OFF     20		/* 三通阀最小关闭时间 */

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

/* 14工况频率表 */
#ifndef USE_WF_OLD
    #define TAB_UP_X_CL 4	/* 环温维数 */
    #define TAB_UP_Y_CL 4	/* 水温维数 */
    #define TAB_DN_X_CL 4	/* 环温维数 */
    #define TAB_DN_Y_CL 4	/* 水温维数 */
     
    #define TAB_UP_X_HT 6	/* 环温维数 */
    #define TAB_UP_Y_HT 4	/* 水温维数 */
    #define TAB_DN_X_HT 4	/* 环温维数 */
    #define TAB_DN_Y_HT 4	/* 水温维数 */
     
    /* 对应para.c起始序号 */
    #define COOL_UP_BGN             0
    #define COOL_DN_BGN             16
    #define HEAT_UP_BGN             20
    #define HEAT_DN_BGN             44
     
    #define val_cl_up_env_in(y, x)      (100*para_C[GROUP_WF][COOL_UP_BGN+(y)*TAB_UP_X_CL+(x)])   // 冷上环X回Y
    #define val_ht_up_env_in(y, x)      (100*para_C[GROUP_WF][HEAT_UP_BGN+(y)*TAB_UP_X_HT+(x)])   // 热上环X回Y
    #define val_cl_dn_env_in(y, x)      (100*para_C[GROUP_WF][COOL_DN_BGN+(y-y)*TAB_DN_X_CL+(x)]) // 冷下环X回Y:仅有环温一维参数
    #define val_ht_dn_env_in(y, x)      (100*para_C[GROUP_WF][HEAT_DN_BGN+(y-y)*TAB_DN_X_HT+(x)]) // 热下环X回Y:仅有环温一维参数
#else
    #define TAB_UP_X_CL 4	/* 环温维数 */
    #define TAB_UP_Y_CL 3	/* 水温维数 */
    #define TAB_DN_X_CL 4	/* 环温维数 */
    #define TAB_DN_Y_CL 3	/* 水温维数 */
     
    #define TAB_UP_X_HT 6	/* 环温维数 */
    #define TAB_UP_Y_HT 3	/* 水温维数 */
    #define TAB_DN_X_HT 4	/* 环温维数 */
    #define TAB_DN_Y_HT 3	/* 水温维数 */
     
    /* 对应para.c起始序号 */
    #define COOL_UP_BGN             0
    #define COOL_DN_BGN             12
    #define HEAT_UP_BGN             16
    #define HEAT_DN_BGN             34
     
    #define val_cl_up_env_in(y, x)      (100*para_C[GROUP_WF][COOL_UP_BGN+(y)*TAB_UP_X_CL+(x)])   // 冷上环X回Y
    #define val_ht_up_env_in(y, x)      (100*para_C[GROUP_WF][HEAT_UP_BGN+(y)*TAB_UP_X_HT+(x)])   // 热上环X回Y
    #define val_cl_dn_env_in(y, x)      (100*para_C[GROUP_WF][COOL_DN_BGN+(y-y)*TAB_DN_X_CL+(x)]) // 冷下环X回Y:仅有环温一维参数
    #define val_ht_dn_env_in(y, x)      (100*para_C[GROUP_WF][HEAT_DN_BGN+(y)*TAB_DN_X_HT+(x)])   // 热下环X回Y
#endif

//-----------------------------------------------------------------------------------------------------
//--固定参数-------------------------------------------------------------------------------------------

#define val_00_board_alarm      (0)                     // 0--系统电源故障  1--00号电源故障
#define val_hold_evi_time       (0)                     // 暂停调阀时间0禁用暂停调阀功能
#define val_hold_evi_cyc        (0)                     // 暂停调阀周期
#define val_antifrez_alarmtemp  (30)                    // 防冻报警温度
#define val_antifrez_resettemp  (70)                    // 防冻复位温度
#define sv_economy_mode         (0)                     // 节能模式:1开启0关闭

#define val_defrost_fan_dly             (10)        /* 退除霜开风机延时 */
#define val_defrost_cm_on_time          (5*60)      /* 除霜开机时间 */

/* 退除霜时的时间节点 */
#define val_defr_freq_reduce_time   20      /* 预估退除霜时的降频时间 */
#define DEFROST_DLY_1    (val_defr_freq_reduce_time)
#define DEFROST_DLY_2    (DEFROST_DLY_1+val_defrost_stop_cm_tm_exit)
#define DEFROST_DLY_3    (DEFROST_DLY_2+val_defrost_four_tm_exit)

#define DEFR_CHG_FREQ(cm)    (s_get_cm_status(cm, CM_DEFROST_OFF)\
                              ? val_invt_rps_min : val_defrost_chg_freq)

PARA_EXT I16 para_splash;	/* 启动页 */

PARA_EXT U08 sh_language;	/* 童锁 */

PARA_EXT U08 sh_temp_unit;	/* 温度单位 */

#define TEMP_IS_CENT		(sh_temp_unit == SH_SET_UNIT_C)
#define TEMP_IS_FARA		(sh_temp_unit == SH_SET_UNIT_F)

//======================================================================================


#define GET_INIT_PARA()             (gIO.io_init)      // 获取参数初始化状态
#define SET_INIT_PARA(x)            (gIO.io_init=x)    // 设置参数初始化状态

#define ACTION_IDLE     0   //
#define ACTION_START    1   // 动作执行开始
#define ACTION_ING      2   // 动作执行中
#define ACTION_SUCC     3   // 动作执行成功
#define ACTION_FAIL     4   // 动作执行失败


//======================================================================================
extern PARAMITEM ParamPri[];
PARA_EXT I16 F_To_C(I16 F, U16 sign);
PARA_EXT I16 C_To_F(I16 C, U16 sign);
//PARA_EXT U08 para_is_shift(U08 n, U08 m);
#define     CTUS_10         10
#define     CTUS_100        30
#define     CTUS_1000       80

#define     CAL_CYCLE       0   // 循环计算
#define     CAL_ETOE        1   // end to end 不循环计算
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

// 新增加的数据访问接口
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

#define GET_MIN 0       // 获取最小值
#define GET_MAX 1       // 获取最大值
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

