/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: Tuya_B.c
 文件描述: 涂鸦无线功能(后板部分)
 创建人  : 林浩彬
 创建日期: 2020.12.13
*******************************************************************************/
#define TUYA_B_GLOBAL
#include "includes.h"

#if (TUYA_B_SLP_TYPE == TUYA_B_SLP_NT_HLXXX)                /* 暖通HL系列SLP */
    #include "SLP_NT/Tuya_B_SLP_HLxxx.c"
#elif (TUYA_B_SLP_TYPE == TUYA_B_SLP_NT_HLXXX_MULTI)		/* 暖通HL系列多联供SLP */
    #include "SLP_NT/Tuya_B_SLP_HLxxx_MULTI.c"
#endif


/*******************************************************************************
 函 数: TYB_init()
 功 能: 涂鸦模块的变量初始化
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void TYB_init(void)
{
/* 有"初始化配置" */
#ifdef TUYA_USE_CONFIG_INIT
    TYBC_config_init();
#else
    ;
#endif
    fg_dpid_upload_init = FALSE;
}


static U16 tyb_upload_timer;                                                 /* 定时上报计时 */
/*******************************************************************************
 函 数: TYB_interval_upload_timer()
 功 能: 功能点周期上报计时处理
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TYB_interval_upload_timer(void)
{
    U08 dpid_idx;

    for (dpid_idx = 0; dpid_idx < DPID_USED; dpid_idx++)
    {
        if (DP[dpid_idx].data_type== DATA_WO        /* 只写功能点无需上报 */
        || DP[dpid_idx].func_type == FUNC_CHANNEL)  /* 通道传输功能点无需上报 */
        {
            continue;
        }
        else
        {
            U16 upload_intv = 0;

#if (TYBC_MODULE_TYPE == MODULE_WIFI)
            switch (DP[dpid_idx].freq)
            {
                case FREQ_VERY_LOW:  upload_intv = DP_INTV_1_HOUR;  break;
                case FREQ_LOW:       upload_intv = DP_INTV_30_MIN;  break;
                case FREQ_NORMAL:    upload_intv = DP_INTV_1_MIN;   break;
                case FREQ_FAST:      upload_intv = DP_INTV_10_SEC;  break;
                case FREQ_VERY_FAST: upload_intv = DP_INTV_3_SEC;   break;
                case FREQ_INFINITE:  upload_intv = DP_INTV_1_MIN;   break;  /* 实际无作用，默认1分钟 */
                default:                                            break;
            }
#else
            switch (DP[dpid_idx].freq)
            {
                case FREQ_VERY_LOW:  upload_intv = DP_INTV_1_HOUR;  break;
                case FREQ_LOW:       upload_intv = DP_INTV_30_MIN;  break;
                case FREQ_NORMAL:    upload_intv = DP_INTV_1_MIN;   break;
                case FREQ_FAST:      upload_intv = DP_INTV_10_SEC;  break;
                case FREQ_VERY_FAST: upload_intv = DP_INTV_3_SEC;   break;
                case FREQ_INFINITE:  upload_intv = DP_INTV_1_MIN;   break;  /* 实际无作用，默认1分钟 */
                default:                                            break;
            }
#endif

            if (upload_intv != 0)
            {
                if (tyb_upload_timer % upload_intv == 0)
                {
                    SETBIT_N(fg_dpid_upload_pri_low, dpid_idx);
                }
            }
        }
    }
}


/*******************************************************************************
 函 数: TYB_timer()
 功 能: 1s时间到计数函数
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void TYB_timer(void)
{
    if (TYBC_SEC_FLAG)
    {

        TYB_interval_upload_timer();

        if (++tyb_upload_timer >= 36000)
        {
            tyb_upload_timer = 0;
        }
    }

/* 有"计时配置" */
#ifdef TUYA_USE_CONFIG_TIMER
    TYBC_config_timer();
#endif
}


/*******************************************************************************
 函 数: TYB_upload_product_info()
 功 能: 发送上报功能点总数&PID
 参 数: dest_buf--上报缓冲区;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL TYB_upload_product_info(U08 *dest_buf)
{
    U08 buf_len = 0;

    /* 赋值 */
    dest_buf[buf_len++] = DPID_USED;

    MEMCPY(&dest_buf[buf_len], (far U08 *)TYBC_PID, TUYA_PID_LEN);

    return TRUE;
}

#ifdef TUYA_HAS_TRANS
/*******************************************************************************
 函 数: TYB_get_dpid_idx()
 功 能: 根据功能点ID查找对应的功能点ID所在位置
 参 数: dpid--功能点ID;
 返 回: 功能点ID索引值
 其 它: 空
*******************************************************************************/
U08 TYB_get_dpid_idx(U08 dpid)
{
    U08 i;
    U08 dpid_idx = 0;
    for (i = 0; i < DPID_USED; i++)
    {
        if (dpid == DP[i].dpid)
        {
            dpid_idx = i;
            break;
        }
    }
    return dpid_idx;
}
#endif


#ifdef TUYA_USE_HMI_SLAVE
/* 只支持从机屏 */
/*******************************************************************************
 函 数: TYB_is_dpid_idx_valid()
 功 能: 是否为有效功能点
 参 数: idx--功能点ID;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL TYB_is_dpid_idx_valid(U08 idx)
{
    return ((idx) != DPID_IDX_NULL);
}
#endif

/*******************************************************************************
 函 数: TYB_fill_dp()
 功 能: 填充上报数据
 参 数: tuya_data--涂鸦数据;
        dpid_idx--功能点ID索引号;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL TYB_fill_dp(TUYA_DATA* tuya_data, U08 dpid_idx)
{
    /* 清空 */
    PTRCLR(tuya_data);

    if (dpid_idx == DPID_IDX_NULL || dpid_idx >= DPID_USED)
    {
        tuya_data->dpid = DPID_NULL;
    }
    else
    {
        TYB_SLP_fill_upload_data(tuya_data, dpid_idx);
    }

    return TRUE;
}


/*******************************************************************************
 函 数: TYB_parse_tuya_data()
 功 能: 读取接收缓冲区，并根据下发功能点更新相应的本地数据，并标记快速响应
 参 数: src_buf--接收缓冲区(云端下发的数据);
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL TYB_parse_tuya_data(U08 *src_buf)
{
    U08 i;
    TUYA_DATA tuya_data;
    U08 *dest_buf = NULL;

    /* 读取接收缓冲区 */
    dest_buf = (U08*)&tuya_data;
    for (i = 0; i < sizeof(TUYA_DATA); i++)
    {
        dest_buf[i] = src_buf[i];
    }

    /* 解析功能点并更新至本地 */
    TYB_SLP_parse_dp_data(&tuya_data);

    return TRUE;
}

