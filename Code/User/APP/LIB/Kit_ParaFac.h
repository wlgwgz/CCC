/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: Kit_ParaFac.h
 �ļ�����: ����Ͳ����׼�����(��ϲ�����������PC���ʹ��)
 ������  : JH
 ��������: 2022.05.10
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
