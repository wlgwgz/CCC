/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: Tuya_B.h
 文件描述: 涂鸦无线功能(后板部分)
 创建人  : 林浩彬
 创建日期: 2020.12.13
*******************************************************************************/

#ifndef __TUYA_B_H__
#define __TUYA_B_H__

#ifdef TUYA_B_GLOBAL
    #define TUYA_B_EXT
    #define TUYA_B_SLP_EXT
#else
    #define TUYA_B_EXT extern
    #define TUYA_B_SLP_EXT extern
#endif

/* 兼容性处理 */
#ifndef far
    #define far
#endif

/* 产品ID长度 */
#define TUYA_PID_LEN                    (16)   

#ifndef TUYA_F_H                        /* 此部分默认前板已定义 */
/* 产品信息数据 */
typedef struct tag_TUYA_INFO
{
    U08 dpid_num;                       /* 需要上报的功能点个数 */
    U08 product_id[TUYA_PID_LEN];       /* 产品ID */
    U08 bak;                            /* 备用，使结构体字节数为偶数 */
}TUYA_INFO;
#define TUYA_INFO_SIZE                  (sizeof(TUYA_INFO)+1) / sizeof(U16)

/* 功能点数据 */
typedef struct tag_TUYA_DATA
{
    U08 dpid;                           /* DPID */
    U08 dp_type;                        /* 功能点类型 */
    U08 data_buf[244];                  /* 传输数据 */
}TUYA_DATA;
#define TUYA_DATA_SIZE                  (sizeof(TUYA_DATA)+1) / sizeof(U16)

/* "功能指令"部分长度(字节数) */
#define RAW_DATA_BYTES                  (244)  /* 透传型 */
#define BOOL_DATA_BYTES                 (1)    /* 布尔型 */
#define NUMBER_DATA_BYTES               (4)    /* 数值型 */
#define STRING_DATA_BYTES               (244)  /* 字符型 */
#define ENUM_DATA_BYTES                 (1)    /* 枚举型 */
#define FAULT_DATA_BYTES                (1)    /* 故障型 */
#endif

/* 功能点属性 */
typedef struct tag_TUYA_DP_PROPS
{
    U08 dpid;                           /* DPID */
    U08 dp_type;                        /* 功能点类型 */
    U08 data_type;                      /* 数据传输类型 */
    U08 func_type;                      /* 功能类型 */
    U08 freq;                           /* 上报频率 */
} TUYA_DP_PROPS;

/* 功能点类型 */
#define DP_RAW                          (0)    /* 透传型 */
#define DP_BOOL                         (1)    /* 布尔型 */
#define DP_NUMBER                       (2)    /* 数值型 */
#define DP_STRING                       (3)    /* 字符型 */
#define DP_ENUM                         (4)    /* 枚举型 */
#define DP_FAULT                        (5)    /* 故障型 */

/* 数据传输类型 */
#define DATA_RO                         (0)    /* 只上报 */
#define DATA_WO                         (1)    /* 只下发 */
#define DATA_RW                         (2)    /* 可上报可下发 */

/* 功能类型 */
#define FUNC_CUSTOM                     (0)    /* 自定义功能 */
#define FUNC_STANDARD                   (1)    /* 标准功能 */
#define FUNC_CHANNEL                    (2)    /* 通道传输功能 */

/* 上报频率类型(越大越快) */
#define FREQ_VERY_LOW                   (0)    /* 很慢 */
#define FREQ_LOW                        (1)    /* 慢 */
#define FREQ_NORMAL                     (2)    /* 常规 */
#define FREQ_FAST                       (3)    /* 快 */
#define FREQ_VERY_FAST                  (4)    /* 很快 */
#define FREQ_INFINITE                   (5)    /* 连续 */
#define FREQ_TYPE_MAX                   (6)    /* 最大上报频率类型数 */

/* 功能点上报周期 */
#define DP_INTV_1_HOUR                  (3600) /* 1h */
#define DP_INTV_30_MIN                  (1800) /* 30min */
#define DP_INTV_1_MIN                   (60)   /* 1min */
#define DP_INTV_10_SEC                  (10)   /* 10s */
#define DP_INTV_3_SEC                   (3)    /* 3s */

/* 无效功能点索引 */
#define DPID_IDX_NULL                   (0xFF)                              

/* 字符编码类型 */
#define P_CODE                          (0)    /* 内码 */
#define GBK_CODE                        (1)    /* GBK2312码 */
#define UNI_CODE                        (2)    /* UNICODE码 */

/* 模组类型 */
#define MODULE_WIFI                     (0)    /* WIFI模组 */
#define MODULE_4G                       (1)    /* 4G模组 */

/* 业务逻辑类型 */
#define TUYA_B_SLP_NT_HLXXX             (0)    /* 暖通HL系列常规业务 */
#define TUYA_B_SLP_NT_HLXXX_MULTI       (1)    /* 暖通HL系列多联供业务 */

/* ================================================================================ */
/* 功 能 传 输 协 议 相 关 定 义                                                 */
/* ================================================================================ */
/* 字段类型 */
#define FIELD_NULL                      (0x00) /* 无效 */
#define FIELD_BOOL                      (0x01) /* 布尔型 */
#define FIELD_NUMBER                    (0x02) /* 数值型 */
#define FIELD_ENUM                      (0x03) /* 枚举型 */
#define FIELD_RO_NUM                    (0x04) /* 只读数值型 */
#define FIELD_RO_MSG                    (0x05) /* 只读字符串型 */
#define FIELD_ONE_MSG                   (0x06) /* 单字符串型 */
#define FIELD_ONE_NUM                   (0x07) /* 单数值型 */

/* 字段使能 */
#define FIELD_DISABLED                  (0x00) /* 禁用(字段上传但无效) */
#define FIELD_ENABLE                    (0x01) /* 使用 */

/* 字段属性 */
#define FIELD_READONLY                  (0)    /* 只读(只上报) */
#define FIELD_WRITABLE                  (1)    /* 可写(可上报可下发) */

/* ================================================================================ */
/* 以 下 接 口 仅 供 业 务 逻 辑 部 分 使 用 (Tuya_B_SLP_XXXXX.c)                   */
/* ================================================================================ */
TUYA_B_EXT BOOL fg_dpid_upload_init;        /* 云端存储的协议版本清空标志 */
TUYA_B_EXT BOOL fg_dpid_upload_all;         /* 全部上报标志 */
TUYA_B_EXT void TYB_create_bool_field(U08 *buf, U08 *acc_len, U08 func_attr, U08 func_id, U08 field_type, U16 title, BOOL value);
TUYA_B_EXT void TYB_create_number_field(U08 *buf, U08 *acc_len, U08 func_attr, U08 func_id, U08 field_type, U16 title, U16 attr, I16 value, I16 max, I16 min);
TUYA_B_EXT void TYB_create_enum_field(U08 *buf, U08 *acc_len, U08 func_attr, U08 func_id, U08 field_type, U16 title, U16 *range, U08 range_len, I16 value);
TUYA_B_EXT void TYB_create_ro_num_field(U08 *buf, U08 *acc_len, U08 func_attr, U08 func_id, U08 field_type, U16 title, U16 attr, I16 value);
TUYA_B_EXT void TYB_create_ro_msg_field(U08 *buf, U08 *acc_len, U08 func_attr, U08 func_id, U08 field_type, U16 title, U16 msg);
TUYA_B_EXT void TYB_create_one_msg_field(far U08 *buf, far U08 *acc_len, far U08* content, U08 content_size);

/* ================================================================================ */
/* 外 部 接 口                                                                      */
/* ================================================================================ */
TUYA_B_EXT void TYB_init(void);                                     /* 涂鸦功能初始化 */
TUYA_B_EXT void TYB_timer(void);                                    /* 涂鸦功能计时 */
TUYA_B_EXT BOOL TYB_upload_product_info(U08 *dest_buf);             /* 产品信息上报 */
TUYA_B_EXT BOOL TYB_fill_dp(TUYA_DATA* tuya_data, U08 dpid_idx);    /* 数据填充功能点 */
TUYA_B_EXT BOOL TYB_parse_tuya_data(U08 *src_buf);                  /* 功能点解析 */
TUYA_B_EXT U08 TYB_get_dpid_idx(U08 dpid);                          /* 获取功能点在表中的索引 */
TUYA_B_EXT BOOL TYB_is_dpid_idx_valid(U08 idx);                     /* 有效功能点判断 */

#define TYB_SET_FG_DPID_UPDATE_ALL    (fg_dpid_upload_all = TRUE)   /* 设置功能点全部上传 */

/* 配置头文件 */
#include "Tuya_B_Config.h"

/* 业务头文件 */
#if (TUYA_B_SLP_TYPE == TUYA_B_SLP_NT_HLXXX)                /* 暖通HL系列常规SLP */
    #include "SLP_NT/Tuya_B_SLP_HLxxx.h"
#elif (TUYA_B_SLP_TYPE == TUYA_B_SLP_NT_HLXXX_MULTI)		/* 暖通HL系列多联供SLP */
    #include "SLP_NT/Tuya_B_SLP_HLxxx_MULTI.h"
#endif

#endif

