/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: exio_klv_config.h
 �ļ�����: IO��չ��������
 ������  : J.H.
 ��������: 2022.01.14
*******************************************************************************/

#ifdef EXIO_KLV_CONFIG_GLOBAL
    #define EXIO_KLV_CFG_EXT
#else
    #define EXIO_KLV_CFG_EXT extern
#endif

/* ������ */
#define EXIO_KLV_MASTER                                             /* KLV���� */
#define EXIO_KLV_CORE_USED                  (EXIO_KLV_CORE_0)       /* KLV���İ汾: ����0 */
#define EXIO_KLV_SLP_TYPE                   (EXIO_KLV_SLP_NT_DLG)	/* KLVҵ������: ůͨ������ */
#define EXIO_KLV_SLP_USED                   (EXIO_KLV_SLP_0)        /* KLVҵ��汾: ҵ��0 */

/* KLVЭ������ */
#ifdef EXIO_KLV_MASTER
#define EKC_EXIO_KLV_USED                   (USE_EXIO)

EXIO_KLV_CFG_EXT I16 EKC_mst_get_exio_tmp_cfg(U08 cfg_type, U08 rsrc_id);
EXIO_KLV_CFG_EXT I16 EKC_mst_get_exio_eev_cfg(U08 cfg_type, U08 rsrc_id);
EXIO_KLV_CFG_EXT I16 EKC_mst_get_exio_hdw_init(U08 init_type, U08 rsrc_id);
EXIO_KLV_CFG_EXT U08 EKC_mst_insert_request_node(U08 com_id, U08 exio_id);

/* KLVЭ��ӻ� */
#else
#define EKC_EXIO_VER                        (APPFUNCODEVER)	/* IO��չ��汾�� */
#define EKC_EXIO_VER_BYTES                  (MAX_VER_BYTE)	/* �汾���ֽ��� */

#define EKC_SLV_EXIO_DI(n)                  (gIO.input.word)	        /* ���������� */
#define EKC_SLV_EXIO_DO(n)                  (gIO.output.word)		    /* �̵������ */
#define EKC_SLV_EXIO_SW(n)                  (gIO.jmp.word)	            /* ���뿪�� */
#define EKC_SLV_EXIO_TMP(n)                 (fun_pv_div_10(gIO.temp[n]))/* �¶�ģ���� */
#define EKC_SLV_EXIO_PRS(n)                 (gIO.ai[n])		            /* ѹ��ģ���� */
#define EKC_SLV_EXIO_ST_PV(n)               (gIO.io_step_pv[n])		    /* ������ */
#define EKC_SLV_EXIO_FAN_PV(n)              (pwmInput[n])	            /* ����PWM���(������) */
#define EKC_SLV_EXIO_PH(n)                  (gIO.phase)		            /* ���� */

EXIO_KLV_CFG_EXT U08 EKC_get_exio_hdw_rsrc_info_num(void);
EXIO_KLV_CFG_EXT U08 EKC_get_exio_hdw_cfg_info_num(void);
EXIO_KLV_CFG_EXT U08 EKC_get_exio_hdw_init_info_num(void);

EXIO_KLV_CFG_EXT void EKC_slv_exio_hdw_cfg_handler(void);
EXIO_KLV_CFG_EXT void EKC_slv_exio_hdw_init_handler(void);
EXIO_KLV_CFG_EXT BOOL EKC_slv_exio_eev_init_done(void);


#endif

