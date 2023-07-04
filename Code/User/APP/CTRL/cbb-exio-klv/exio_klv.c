/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: exio_klv.c
 文件描述: IO扩展功能入口源文件
 创建人  : J.H.
 创建日期: 2022.01.14
*******************************************************************************/
#include "includes.h"

#if (EXIO_KLV_SLP_TYPE == EXIO_KLV_SLP_NT_DLG)              /* 暖通多联供 */
    #include "slp/nt_dlg/exio_klv_nt_dlg.c"
#endif

#if (EXIO_KLV_CORE_USED == EXIO_KLV_CORE_0)                 /* 核心0 */
    #include "core/0/exio_klv_core_0.c"
#endif

