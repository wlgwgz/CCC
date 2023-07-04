/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: Kit_ParaFac.h
 文件描述: 多机型参数套件功能(配合参数管理助手PC软件使用)
 创建人  : JH
 创建日期: 2022.05.10
*******************************************************************************/

#ifndef __KIT_PARAFAC_H__
#define __KIT_PARAFAC_H__

#ifdef KIT_PARAFAC_GLOBAL
    #define KPF_EXT
#else
    #define KPF_EXT extern
#endif

KPF_EXT U08 com_slv_BMS_aux(U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal);

#endif 
