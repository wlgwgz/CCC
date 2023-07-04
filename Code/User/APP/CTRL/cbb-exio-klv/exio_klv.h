/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: exio_klv.h
 文件描述: IO扩展功能入口头文件
 创建人  : J.H.
 创建日期: 2022.01.14
*******************************************************************************/

#ifndef __EXIO_KLV_H__
#define __EXIO_KLV_H__

/* 配置文件 */
#include "exio_klv_config.h"

/* 核心版本 */
#define EXIO_KLV_CORE_0                       (0)           /* 核心0: 初始版本 */

/* 业务逻辑类型 */
#define EXIO_KLV_SLP_NT_DLG                   (0)           /* 暖通多联供 */

/* 核心头文件 */
#if (EXIO_KLV_CORE_USED == EXIO_KLV_CORE_0)
    #include "core/0/exio_klv_core_0.h"
#endif

/* 业务头文件 */
#if (EXIO_KLV_SLP_TYPE == EXIO_KLV_SLP_NT_DLG)              /* 暖通多联供 */
    #include "slp/nt_dlg/exio_klv_nt_dlg.h"
#endif

#endif

