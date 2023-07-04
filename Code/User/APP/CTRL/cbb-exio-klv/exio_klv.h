/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: exio_klv.h
 �ļ�����: IO��չ�������ͷ�ļ�
 ������  : J.H.
 ��������: 2022.01.14
*******************************************************************************/

#ifndef __EXIO_KLV_H__
#define __EXIO_KLV_H__

/* �����ļ� */
#include "exio_klv_config.h"

/* ���İ汾 */
#define EXIO_KLV_CORE_0                       (0)           /* ����0: ��ʼ�汾 */

/* ҵ���߼����� */
#define EXIO_KLV_SLP_NT_DLG                   (0)           /* ůͨ������ */

/* ����ͷ�ļ� */
#if (EXIO_KLV_CORE_USED == EXIO_KLV_CORE_0)
    #include "core/0/exio_klv_core_0.h"
#endif

/* ҵ��ͷ�ļ� */
#if (EXIO_KLV_SLP_TYPE == EXIO_KLV_SLP_NT_DLG)              /* ůͨ������ */
    #include "slp/nt_dlg/exio_klv_nt_dlg.h"
#endif

#endif

