/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: exio_klv.c
 �ļ�����: IO��չ�������Դ�ļ�
 ������  : J.H.
 ��������: 2022.01.14
*******************************************************************************/
#include "includes.h"

#if (EXIO_KLV_SLP_TYPE == EXIO_KLV_SLP_NT_DLG)              /* ůͨ������ */
    #include "slp/nt_dlg/exio_klv_nt_dlg.c"
#endif

#if (EXIO_KLV_CORE_USED == EXIO_KLV_CORE_0)                 /* ����0 */
    #include "core/0/exio_klv_core_0.c"
#endif

