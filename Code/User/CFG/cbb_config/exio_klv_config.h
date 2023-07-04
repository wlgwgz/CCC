/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: exio_klv_config.h
 文件描述: IO扩展功能配置
 创建人  : J.H.
 创建日期: 2022.01.14
*******************************************************************************/

#ifdef EXIO_KLV_CONFIG_GLOBAL
    #define EXIO_KLV_CFG_EXT
#else
    #define EXIO_KLV_CFG_EXT extern
#endif

/* 配置项 */
#define EXIO_KLV_MASTER                                             /* KLV主机 */
#define EXIO_KLV_CORE_USED                  (EXIO_KLV_CORE_0)       /* KLV核心版本: 核心0 */
#define EXIO_KLV_SLP_TYPE                   (EXIO_KLV_SLP_NT_DLG)	/* KLV业务类型: 暖通多联供 */
#define EXIO_KLV_SLP_USED                   (EXIO_KLV_SLP_0)        /* KLV业务版本: 业务0 */

/* KLV协议主机 */
#ifdef EXIO_KLV_MASTER
#define EKC_EXIO_KLV_USED                   (USE_EXIO)

#if 0
/* 配置定义 */
enum EKC_RDS_CFG_ID
{
    EKC_REMOTE_CFG_INVT_AD = 0,
    EKC_REMOTE_CFG_INVT_STATES = 1,
    EKC_REMOTE_CFG_INVT_CUR_WAVE = 2,
    EKC_REMOTE_CFG_INVT_PARAM = 3,
    EKC_REMOTE_CFG_INVT_PARAM_FA_1 = 4,
    EKC_REMOTE_CFG_INVT_PARAM_FA_2 = 5,
    EKC_REMOTE_CFG_INVT_PARAM_FB = 6,
    EKC_REMOTE_CFG_INVT_PARAM_F10 = 7,
    EKC_REMOTE_CFG_INVT_EC_FAN_FG = 8,
    EKC_REMOTE_CFG_INVT_PARAM_F9 = 9,
    EKC_REMOTE_CFG_MAX,              /* 注: 请在ERDSC_MAX前添加 */
};

EXIO_KLV_CFG_EXT U08 EKC_get_remote_type_used_num(void);
EXIO_KLV_CFG_EXT U08 EKC_get_remote_info_baseAddr(U08 remoteType);
EXIO_KLV_CFG_EXT U08 EKC_get_remote_info_deviceMax(U08 remoteType);
EXIO_KLV_CFG_EXT const U08* EKC_get_remote_info_cfgList(U08 remoteType);
EXIO_KLV_CFG_EXT U08 EKC_get_remote_info_listLen(U08 remoteType);
EXIO_KLV_CFG_EXT U16 EKC_get_remote_cfg_funcCode(U08 cfg_id);
EXIO_KLV_CFG_EXT U16 EKC_get_remote_cfg_commuIntv(U08 cfg_id);
EXIO_KLV_CFG_EXT U16 EKC_get_remote_cfg_prior(U08 cfg_id);
EXIO_KLV_CFG_EXT U16 EKC_get_remote_cfg_dataAddr(U08 cfg_id);
EXIO_KLV_CFG_EXT U16 EKC_get_remote_cfg_dataNum(U08 cfg_id);
#endif

EXIO_KLV_CFG_EXT I16 EKC_mst_get_exio_tmp_cfg(U08 cfg_type, U08 rsrc_id);
EXIO_KLV_CFG_EXT I16 EKC_mst_get_exio_eev_cfg(U08 cfg_type, U08 rsrc_id);
EXIO_KLV_CFG_EXT I16 EKC_mst_get_exio_hdw_init(U08 init_type, U08 rsrc_id);
EXIO_KLV_CFG_EXT U08 EKC_mst_insert_request_node(U08 com_id, U08 exio_id);

/* KLV协议从机 */
#else
#define EKC_EXIO_VER                        (APPFUNCODEVER)	/* IO扩展板版本号 */
#define EKC_EXIO_VER_BYTES                  (MAX_VER_BYTE)	/* 版本号字节数 */

#define EKC_SLV_EXIO_DI(n)                  (gIO.input.word)	        /* 开关量输入 */
#define EKC_SLV_EXIO_DO(n)                  (gIO.output.word)		    /* 继电器输出 */
#define EKC_SLV_EXIO_SW(n)                  (gIO.jmp.word)	            /* 拨码开关 */
#define EKC_SLV_EXIO_TMP(n)                 (fun_pv_div_10(gIO.temp[n]))/* 温度模拟量 */
#define EKC_SLV_EXIO_PRS(n)                 (gIO.ai[n])		            /* 压力模拟量 */
#define EKC_SLV_EXIO_ST_PV(n)               (gIO.io_step_pv[n])		    /* 电膨步数 */
#define EKC_SLV_EXIO_FAN_PV(n)              (pwmInput[n])	            /* 板载PWM风机(脉冲数) */
#define EKC_SLV_EXIO_PH(n)                  (gIO.phase)		            /* 相序 */

EXIO_KLV_CFG_EXT U08 EKC_get_exio_hdw_rsrc_info_num(void);
EXIO_KLV_CFG_EXT U08 EKC_get_exio_hdw_cfg_info_num(void);
EXIO_KLV_CFG_EXT U08 EKC_get_exio_hdw_init_info_num(void);

EXIO_KLV_CFG_EXT void EKC_slv_exio_hdw_cfg_handler(void);
EXIO_KLV_CFG_EXT void EKC_slv_exio_hdw_init_handler(void);
EXIO_KLV_CFG_EXT BOOL EKC_slv_exio_eev_init_done(void);


#endif

