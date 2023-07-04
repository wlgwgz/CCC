/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: exio_klv_nt_dlg_slp_0.h
 文件描述: IO扩展功能-暖通多联供业务逻辑头文件
 创建人  : J.H.
 创建日期: 2022.01.14
*******************************************************************************/

#ifdef EK_SLP_NT_DLG_GLOBAL
    #define EK_SLP_NT_DLG_EXT
#else
    #define EK_SLP_NT_DLG_EXT extern
#endif

/* 业务版本 */
#define EXIO_KLV_NT_DLG_SLP_0                 (0)           /* 核心0: 初始版本 */

/* IO扩展板最大个数 */
#define EXIO_KLV_MAX                          (1)

/* IO扩展板基地址 */
#define EXIO_KLV_ADDR                         (64)

/* KLV通信阶段 */
#define EK_STAGE_NULL                         (0)           /* 无效 */
#define EK_STAGE_KLV_CORE_SLP                 (1)           /* IO扩展板KLV核心业务版本 */
#define EK_STAGE_APP_VER                      (2)           /* IO扩展板微码版本号 */
#define EK_STAGE_HDW_RSRC_INFO                (3)           /* IO扩展板硬件资源信息 */
#define EK_STAGE_HDW_CFG_INFO                 (4)			/* IO扩展板硬件配置信息 */
#define EK_STAGE_HDW_INIT_INFO                (5)			/* IO扩展板硬件初始化信息 */
#define EK_STAGE_HDW_CFG_DATA                 (6)			/* IO扩展板硬件配置数据 */
#define EK_STAGE_HDW_INIT_DATA                (7)			/* IO扩展板硬件初始化数据 */
#define EK_STAGE_REMOTE_CFG_DATA              (8)           /* IO扩展板远端配置数据 */
#define EK_STAGE_HDW_WR_RD                    (9)           /* IO扩展板硬件状态/输出读写 */
#define EK_STAGE_REMOTE_WR_RD                 (10)          /* IO扩展板远端状态/输出读写 */
#define EK_STAGE_MAX                          (11)          /* 最大KLV通信阶段数 */

/* IO扩展板硬件资源类型 */
#define EK_RSRC_DI                            (0)           /* 开关量输入 */
#define EK_RSRC_DO                            (1)           /* 继电器输出 */
#define EK_RSRC_SW                            (2)           /* 拨码开关 */
#define EK_RSRC_TMP                           (3)           /* 温度探头 */
#define EK_RSRC_PRS                           (4)           /* 压力传感器 */
#define EK_RSRC_EEV                           (5)           /* 电子膨胀阀 */
#define EK_RSRC_FAN                           (6)           /* 板载PWM风机 */
#define EK_RSRC_PH                            (7)           /* 相序检测 */
#define EK_RSRC_INVT                          (8)           /* 变频驱动 */
#define EK_RSRC_MAX                           (9)

/* IO扩展板硬件资源属性 */
#define EK_BIT                                (1<<0)        /* 是否为位变量 */
#define EK_RD                                 (1<<1)        /* 是否为可读 */
#define EK_WR                                 (1<<2)        /* 是否为可写 */
#define EK_INIT                               (1<<3)        /* 是否需要初始化 */
#define EK_REMOTE                             (1<<4)        /* 是否为远端 */

/* IO扩展板硬件资源信息 */
typedef struct tag_EXIO_HDW_RSRC_INFO
{
    U08 rsrc_type;                            /* 硬件资源类型 */
    U08 attr;                                 /* [0]:是否为位变量, [1]: 是否为可读, [2]: 是否为可写 */
    U08 num;                                  /* 当前资源类型数量 */
} EXIO_HDW_RSRC_INFO;
#define EXIO_HDW_RSRC_INFO_BYTES              (sizeof(EXIO_HDW_RSRC_INFO))

/* IO扩展板硬件配置类型 */
#define EK_CFG_TMP_NTC_TYPE                   (0)           /* 温度探头类型 */
#define EK_CFG_EEV_FREQ_TYPE                  (1)           /* 电膨励磁方式 */
#define EK_CFG_EEV_FREQ                       (2)           /* 电膨励磁频率 */
#define EK_CFG_EEV_MAX_STEP                   (3)           /* 电膨最大步数 */
#define EK_CFG_MAX                            (4)

/* IO扩展板硬件配置信息 */
typedef struct tag_EXIO_HDW_CFG_INFO
{
    U08 cfg_type;                              /* 配置类型 */
    U08 rsrc_type;                             /* 硬件资源类型(配置对象) */
} EXIO_HDW_CFG_INFO;
#define EXIO_HDW_CFG_BYTES_INFO               (sizeof(EXIO_HDW_CFG_INFO))

/* IO扩展板硬件资源初始化类型 */
#define EK_INIT_EEV_ST_PV                     (0)           /* 电膨初始当前步数 */
#define EK_INIT_MAX                           (1)

/* IO扩展板硬件初始化信息 */
typedef struct tag_EXIO_HDW_INIT_INFO
{
    U08 init_type;
    U08 rsrc_type;
} EXIO_HDW_INIT_INFO;
#define EXIO_HDW_INIT_BYTES_INFO              (sizeof(EXIO_HDW_INIT_INFO))

/* IO扩展板硬件初始化状态 */
#define EK_HDW_INIT_STA_PLAN                  (0)			/* 硬件初始化计划阶段 */
#define EK_HDW_INIT_STA_DOING                 (1)			/* 硬件初始化进行阶段 */
#define EK_HDW_INIT_STA_DONE                  (2)			/* 硬件初始化完成阶段 */

/* IO扩展板状态标志类型 */
#define EK_FG_KLV_VER_ERR                     (0)			/* IO扩展板协议版本不匹配故障 */
#define EK_FG_COMMU_ERR                       (1)			/* IO扩展板通信故障 */
#define EK_FG_VER_READY                       (2)			/* IO扩展板版本号信息可用 */
#define EK_FG_REMOTE_EXIST                    (3)			/* IO扩展板存在远端可用 */
#define EK_FG_HDW_STATUS_READY                (4)			/* IO扩展板硬件状态信息可用 */
#define EK_FG_UPDATE_HDW_CFG                  (5)			/* 更新IO扩展板硬件配置 */
#define EK_FG_INIT_HDW                        (6)			/* IO扩展板硬件初始化 */
#define EK_FG_MAX                             (7)

#if 0
/* 发送间隔周期 */
#define EK_INTV_INF                           (0)           /* 禁用 */
#define EK_INTV_1_SEC                         (1)           /* 1s */
#define EK_INTV_10_SEC                        (2)           /* 10s */

/* 远端类型 */
#define EK_REMOTE_UNKNOWN                     (0)           /* 未知驱动 */
#define EK_REMOTE_INVT                        (1)           /* 邦普变频驱动(压机、驱动PWM风机) */
#define EK_REMOTE_INVT_FAN                    (2)           /* 邦普变频风机驱动(邦普EC风机) */
#define EK_REMOTE_KM_INVT_FAN                 (3)           /* 科贸EC风机 */
#define EK_REMOTE_ZSFN_INVT_FAN               (4)           /* 顺德四格风机 */
#define EK_REMOTE_TYPE_MAX                    (5)           /*  */

/* 远端数据配置 */
typedef struct tag_EXIO_REMOTE_CONFIG
{
    U08 baseAddr;           /* 设备基地址 */
    U08 deviceMax;          /* 设备最大个数 */
    U08 funcCode;           /* 功能码 */
    U08 commuIntv;          /* 采集频率 */
    U08 prior;              /* 优先级 */
    U08 dataAddrH;          /* 数据地址(高8位) */
    U08 dataAddrL;          /* 数据地址(低8位) */
    U08 dataNum;            /* 数据个数(字) */
} EXIO_REMOTE_CFG;
#define EXIO_REMOTE_CFG_WORDS                 ((sizeof(EXIO_REMOTE_CFG)+1)/sizeof(U16))
#define EXIO_REMOTE_CFG_BYTES                 (EXIO_REMOTE_CFG_WORDS*2)
#endif

#ifdef EXIO_KLV_MASTER
/***********************************************************************
 以下为本模块外部接口，可供其他模块使用
************************************************************************/
/* 协议版本不匹配标志(读写) */
#define GET_EXIO_KLV_VER_ERR(exio_id)         (EK_SLP_mst_get_exio_fg(exio_id,EK_FG_KLV_VER_ERR))	    /* 读取 */
#define SET_EXIO_KLV_VER_ERR(exio_id)         (EK_SLP_mst_set_exio_fg(exio_id,EK_FG_KLV_VER_ERR,FALSE)) /* 置位 */
#define CLR_EXIO_KLV_VER_ERR(exio_id)         (EK_SLP_mst_set_exio_fg(exio_id,EK_FG_KLV_VER_ERR,TRUE))	/* 清零 */

/* 通讯故障标志(读写) */
#define GET_EXIO_COMMU_ERR(exio_id)           (EK_SLP_mst_get_exio_fg(exio_id,EK_FG_COMMU_ERR))         /* 读取 */
#define SET_EXIO_COMMU_ERR(exio_id)           (EK_SLP_mst_set_exio_fg(exio_id,EK_FG_COMMU_ERR,FALSE))	/* 置位 */
#define CLR_EXIO_COMMU_ERR(exio_id)           (EK_SLP_mst_set_exio_fg(exio_id,EK_FG_COMMU_ERR,TRUE))	/* 清零 */

/* IO扩展板版本信息可用标志(只读) */
#define GET_EXIO_VER_READY(exio_id)           (EK_SLP_mst_get_exio_fg(exio_id,EK_FG_VER_READY))

/* IO扩展板状态信息可用标志(只读) */
#define GET_EXIO_HDW_STATUS_READY(exio_id)    (EK_SLP_mst_get_exio_fg(exio_id,EK_FG_HDW_STATUS_READY))

/* 更新IO扩展板硬件配置(读写) */
#define SET_EXIO_HDW_CFG_UPDATE(exio_id)      (EK_SLP_mst_set_exio_fg(exio_id,EK_FG_UPDATE_HDW_CFG,FALSE))

/* IO扩展板硬件初始化(读写) */
#define SET_EXIO_INIT_HDW(exio_id)            (EK_SLP_mst_set_exio_fg(exio_id,EK_FG_INIT_HDW,FALSE))

/* IO扩展板版本号 */
#define GET_EXIO_VER(exio_id)                 (EK_SLP_mst_get_exio_ver(exio_id))

/* 开关量输入 */
#define GET_EXIO_DI(exio_id, n)               (EK_SLP_mst_get_exio_cell_data(EK_RSRC_DI,exio_id,n,TRUE))  /* 读取 */

/* 继电器输出 */
#define GET_EXIO_DO(exio_id, n)               (EK_SLP_mst_get_exio_cell_data(EK_RSRC_DO,exio_id,n,TRUE))  /* 读取 */
#define SET_EXIO_DO(exio_id, n)               (EK_SLP_mst_set_exio_cell_data(EK_RSRC_DO,exio_id,n,1))     /* 置位 */
#define CLR_EXIO_DO(exio_id, n)               (EK_SLP_mst_set_exio_cell_data(EK_RSRC_DO,exio_id,n,0))     /* 清零 */

/* 拨码开关 */
#define GET_EXIO_SW(exio_id, n)               (EK_SLP_mst_get_exio_cell_data(EK_RSRC_SW,exio_id,n,TRUE))  /* 读取 */

/* 温度值 */
#define GET_EXIO_TMP(exio_id, n)              (EK_SLP_mst_get_exio_cell_data(EK_RSRC_TMP,exio_id,n,TRUE)) /* 读取 */

/* 压力值 */
#define GET_EXIO_PRS(exio_id, n)              (EK_SLP_mst_get_exio_cell_data(EK_RSRC_PRS,exio_id,n,TRUE)) /* 读取 */

/* 电子膨胀阀步数 */
#define GET_EXIO_ST_PV(exio_id, n)            (EK_SLP_mst_get_exio_cell_data(EK_RSRC_EEV,exio_id,n,TRUE)) /* 读取 */
#define GET_EXIO_ST_SV(exio_id, n)            (EK_SLP_mst_get_exio_cell_data(EK_RSRC_EEV,exio_id,n,FALSE))
#define SET_EXIO_ST_SV(exio_id, n, val)       (EK_SLP_mst_set_exio_cell_data(EK_RSRC_EEV,exio_id,n,val))  /* 设置 */

/* 风机转速 */
#define GET_EXIO_FAN_PV(exio_id, n)           (EK_SLP_mst_get_exio_cell_data(EK_RSRC_FAN,exio_id,n,TRUE)) /* 读取 */
#define SET_EXIO_FAN_SV(exio_id, n, val)      (EK_SLP_mst_set_exio_cell_data(EK_RSRC_FAN,exio_id,n,val))  /* 设置 */

/* 三相电 */
#define GET_EXIO_PH(exio_id)                  (EK_SLP_mst_get_exio_cell_data(EK_RSRC_PH,exio_id,0,TRUE))  /* 读取 */

EK_SLP_NT_DLG_EXT BOOL EK_SLP_mst_get_exio_fg(U08 exio_id, U08 fg_type);
EK_SLP_NT_DLG_EXT void EK_SLP_mst_set_exio_fg(U08 exio_id, U08 fg_type, BOOL is_clr);
EK_SLP_NT_DLG_EXT U08* EK_SLP_mst_get_exio_ver(U08 exio_id);
EK_SLP_NT_DLG_EXT I16 EK_SLP_mst_get_exio_cell_data(U08 rsrc_type, U08 exio_id, U08 idx, U08 is_read);
EK_SLP_NT_DLG_EXT void EK_SLP_mst_set_exio_cell_data(U08 rsrc_type, U08 exio_id, U08 idx, I16 val);

#else
/***********************************************************************
 以下为本模块外部接口，可供其他模块使用
************************************************************************/
/* 通讯故障标志(读写) */
#define GET_EXIO_COMMU_ERR                    (EK_SLP_slv_get_exio_commu_err())
#define SET_EXIO_COMMU_ERR                    (EK_SLP_slv_set_exio_commu_err(FALSE))
#define CLR_EXIO_COMMU_ERR                    (EK_SLP_slv_set_exio_commu_err(TRUE))

/* 获取: 温度探头类型 */
#define GET_EXIO_TMP_NTC_TYPE(n)              (EK_SLP_slv_get_exio_hdw_cfg_data(EK_CFG_TMP_NTC_TYPE, n))

/* 获取: 电膨励磁方式 */
#define GET_EXIO_EEV_FREQ_TYPE(n)             (EK_SLP_slv_get_exio_hdw_cfg_data(EK_CFG_EEV_FREQ_TYPE, n))

/* 获取: 电膨励磁频率 */
#define GET_EXIO_EEV_FREQ(n)                  (EK_SLP_slv_get_exio_hdw_cfg_data(EK_CFG_EEV_FREQ, n))

/* 获取: 电膨最大步数 */
#define GET_EXIO_EEV_MAX_STEP(n)              (EK_SLP_slv_get_exio_hdw_cfg_data(EK_CFG_EEV_MAX_STEP, n))

/* 获取: 电子膨胀阀初始当前步数 */
#define GET_EXIO_EEV_ST_PV_INIT(n)            (EK_SLP_slv_get_exio_hdw_init_data(EK_INIT_EEV_ST_PV, n))

/* 获取: 继电器输出(位) */
#define GET_EXIO_DO_BIT(n)                    (EK_SLP_slv_get_exio_cell_data(EK_RSRC_DO, n))

/* 清空: 继电器输出(位) */
#define CLR_EXIO_DO_BIT(n)                    (EK_SLP_slv_set_exio_cell_data(EK_RSRC_DO, n, 0))

/* 获取: 电子膨胀阀目标步数 */
#define GET_EXIO_EEV_ST_SV(n)                 (EK_SLP_slv_get_exio_cell_data(EK_RSRC_EEV, n))

/* 清空: 电子膨胀阀目标步数 */
#define CLR_EXIO_EEV_ST_SV(n)                 (EK_SLP_slv_set_exio_cell_data(EK_RSRC_EEV, n, 0))

/* 获取: 板载PWM风机目标步数 */
#define GET_EXIO_FAN_SV(n)                    (EK_SLP_slv_get_exio_cell_data(EK_RSRC_FAN, n))

EK_SLP_NT_DLG_EXT BOOL EK_SLP_slv_get_exio_commu_err(void);
EK_SLP_NT_DLG_EXT void EK_SLP_slv_set_exio_commu_err(BOOL is_clr);
EK_SLP_NT_DLG_EXT I16 EK_SLP_slv_get_exio_hdw_cfg_data(U08 cfg_type, U08 idx);
EK_SLP_NT_DLG_EXT I16 EK_SLP_slv_get_exio_hdw_init_data(U08 init_type, U08 idx);
EK_SLP_NT_DLG_EXT I16 EK_SLP_slv_get_exio_cell_data(U08 rsrc_type, U08 idx);
EK_SLP_NT_DLG_EXT void EK_SLP_slv_set_exio_cell_data(U08 rsrc_type, U08 idx, I16 val);

#endif

