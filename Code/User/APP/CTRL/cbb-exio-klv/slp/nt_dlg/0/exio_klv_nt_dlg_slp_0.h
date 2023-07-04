/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: exio_klv_nt_dlg_slp_0.h
 �ļ�����: IO��չ����-ůͨ������ҵ���߼�ͷ�ļ�
 ������  : J.H.
 ��������: 2022.01.14
*******************************************************************************/

#ifdef EK_SLP_NT_DLG_GLOBAL
    #define EK_SLP_NT_DLG_EXT
#else
    #define EK_SLP_NT_DLG_EXT extern
#endif

/* ҵ��汾 */
#define EXIO_KLV_NT_DLG_SLP_0                 (0)           /* ����0: ��ʼ�汾 */

/* IO��չ�������� */
#define EXIO_KLV_MAX                          (1)

/* IO��չ�����ַ */
#define EXIO_KLV_ADDR                         (64)

/* KLVͨ�Ž׶� */
#define EK_STAGE_NULL                         (0)           /* ��Ч */
#define EK_STAGE_KLV_CORE_SLP                 (1)           /* IO��չ��KLV����ҵ��汾 */
#define EK_STAGE_APP_VER                      (2)           /* IO��չ��΢��汾�� */
#define EK_STAGE_HDW_RSRC_INFO                (3)           /* IO��չ��Ӳ����Դ��Ϣ */
#define EK_STAGE_HDW_CFG_INFO                 (4)			/* IO��չ��Ӳ��������Ϣ */
#define EK_STAGE_HDW_INIT_INFO                (5)			/* IO��չ��Ӳ����ʼ����Ϣ */
#define EK_STAGE_HDW_CFG_DATA                 (6)			/* IO��չ��Ӳ���������� */
#define EK_STAGE_HDW_INIT_DATA                (7)			/* IO��չ��Ӳ����ʼ������ */
#define EK_STAGE_REMOTE_CFG_DATA              (8)           /* IO��չ��Զ���������� */
#define EK_STAGE_HDW_WR_RD                    (9)           /* IO��չ��Ӳ��״̬/�����д */
#define EK_STAGE_REMOTE_WR_RD                 (10)          /* IO��չ��Զ��״̬/�����д */
#define EK_STAGE_MAX                          (11)          /* ���KLVͨ�Ž׶��� */

/* IO��չ��Ӳ����Դ���� */
#define EK_RSRC_DI                            (0)           /* ���������� */
#define EK_RSRC_DO                            (1)           /* �̵������ */
#define EK_RSRC_SW                            (2)           /* ���뿪�� */
#define EK_RSRC_TMP                           (3)           /* �¶�̽ͷ */
#define EK_RSRC_PRS                           (4)           /* ѹ�������� */
#define EK_RSRC_EEV                           (5)           /* �������ͷ� */
#define EK_RSRC_FAN                           (6)           /* ����PWM��� */
#define EK_RSRC_PH                            (7)           /* ������ */
#define EK_RSRC_INVT                          (8)           /* ��Ƶ���� */
#define EK_RSRC_MAX                           (9)

/* IO��չ��Ӳ����Դ���� */
#define EK_BIT                                (1<<0)        /* �Ƿ�Ϊλ���� */
#define EK_RD                                 (1<<1)        /* �Ƿ�Ϊ�ɶ� */
#define EK_WR                                 (1<<2)        /* �Ƿ�Ϊ��д */
#define EK_INIT                               (1<<3)        /* �Ƿ���Ҫ��ʼ�� */
#define EK_REMOTE                             (1<<4)        /* �Ƿ�ΪԶ�� */

/* IO��չ��Ӳ����Դ��Ϣ */
typedef struct tag_EXIO_HDW_RSRC_INFO
{
    U08 rsrc_type;                            /* Ӳ����Դ���� */
    U08 attr;                                 /* [0]:�Ƿ�Ϊλ����, [1]: �Ƿ�Ϊ�ɶ�, [2]: �Ƿ�Ϊ��д */
    U08 num;                                  /* ��ǰ��Դ�������� */
} EXIO_HDW_RSRC_INFO;
#define EXIO_HDW_RSRC_INFO_BYTES              (sizeof(EXIO_HDW_RSRC_INFO))

/* IO��չ��Ӳ���������� */
#define EK_CFG_TMP_NTC_TYPE                   (0)           /* �¶�̽ͷ���� */
#define EK_CFG_EEV_FREQ_TYPE                  (1)           /* �������ŷ�ʽ */
#define EK_CFG_EEV_FREQ                       (2)           /* ��������Ƶ�� */
#define EK_CFG_EEV_MAX_STEP                   (3)           /* ��������� */
#define EK_CFG_MAX                            (4)

/* IO��չ��Ӳ��������Ϣ */
typedef struct tag_EXIO_HDW_CFG_INFO
{
    U08 cfg_type;                              /* �������� */
    U08 rsrc_type;                             /* Ӳ����Դ����(���ö���) */
} EXIO_HDW_CFG_INFO;
#define EXIO_HDW_CFG_BYTES_INFO               (sizeof(EXIO_HDW_CFG_INFO))

/* IO��չ��Ӳ����Դ��ʼ������ */
#define EK_INIT_EEV_ST_PV                     (0)           /* �����ʼ��ǰ���� */
#define EK_INIT_MAX                           (1)

/* IO��չ��Ӳ����ʼ����Ϣ */
typedef struct tag_EXIO_HDW_INIT_INFO
{
    U08 init_type;
    U08 rsrc_type;
} EXIO_HDW_INIT_INFO;
#define EXIO_HDW_INIT_BYTES_INFO              (sizeof(EXIO_HDW_INIT_INFO))

/* IO��չ��Ӳ����ʼ��״̬ */
#define EK_HDW_INIT_STA_PLAN                  (0)			/* Ӳ����ʼ���ƻ��׶� */
#define EK_HDW_INIT_STA_DOING                 (1)			/* Ӳ����ʼ�����н׶� */
#define EK_HDW_INIT_STA_DONE                  (2)			/* Ӳ����ʼ����ɽ׶� */

/* IO��չ��״̬��־���� */
#define EK_FG_KLV_VER_ERR                     (0)			/* IO��չ��Э��汾��ƥ����� */
#define EK_FG_COMMU_ERR                       (1)			/* IO��չ��ͨ�Ź��� */
#define EK_FG_VER_READY                       (2)			/* IO��չ��汾����Ϣ���� */
#define EK_FG_REMOTE_EXIST                    (3)			/* IO��չ�����Զ�˿��� */
#define EK_FG_HDW_STATUS_READY                (4)			/* IO��չ��Ӳ��״̬��Ϣ���� */
#define EK_FG_UPDATE_HDW_CFG                  (5)			/* ����IO��չ��Ӳ������ */
#define EK_FG_INIT_HDW                        (6)			/* IO��չ��Ӳ����ʼ�� */
#define EK_FG_MAX                             (7)

#if 0
/* ���ͼ������ */
#define EK_INTV_INF                           (0)           /* ���� */
#define EK_INTV_1_SEC                         (1)           /* 1s */
#define EK_INTV_10_SEC                        (2)           /* 10s */

/* Զ������ */
#define EK_REMOTE_UNKNOWN                     (0)           /* δ֪���� */
#define EK_REMOTE_INVT                        (1)           /* ���ձ�Ƶ����(ѹ��������PWM���) */
#define EK_REMOTE_INVT_FAN                    (2)           /* ���ձ�Ƶ�������(����EC���) */
#define EK_REMOTE_KM_INVT_FAN                 (3)           /* ��óEC��� */
#define EK_REMOTE_ZSFN_INVT_FAN               (4)           /* ˳���ĸ��� */
#define EK_REMOTE_TYPE_MAX                    (5)           /*  */

/* Զ���������� */
typedef struct tag_EXIO_REMOTE_CONFIG
{
    U08 baseAddr;           /* �豸����ַ */
    U08 deviceMax;          /* �豸������ */
    U08 funcCode;           /* ������ */
    U08 commuIntv;          /* �ɼ�Ƶ�� */
    U08 prior;              /* ���ȼ� */
    U08 dataAddrH;          /* ���ݵ�ַ(��8λ) */
    U08 dataAddrL;          /* ���ݵ�ַ(��8λ) */
    U08 dataNum;            /* ���ݸ���(��) */
} EXIO_REMOTE_CFG;
#define EXIO_REMOTE_CFG_WORDS                 ((sizeof(EXIO_REMOTE_CFG)+1)/sizeof(U16))
#define EXIO_REMOTE_CFG_BYTES                 (EXIO_REMOTE_CFG_WORDS*2)
#endif

#ifdef EXIO_KLV_MASTER
/***********************************************************************
 ����Ϊ��ģ���ⲿ�ӿڣ��ɹ�����ģ��ʹ��
************************************************************************/
/* Э��汾��ƥ���־(��д) */
#define GET_EXIO_KLV_VER_ERR(exio_id)         (EK_SLP_mst_get_exio_fg(exio_id,EK_FG_KLV_VER_ERR))	    /* ��ȡ */
#define SET_EXIO_KLV_VER_ERR(exio_id)         (EK_SLP_mst_set_exio_fg(exio_id,EK_FG_KLV_VER_ERR,FALSE)) /* ��λ */
#define CLR_EXIO_KLV_VER_ERR(exio_id)         (EK_SLP_mst_set_exio_fg(exio_id,EK_FG_KLV_VER_ERR,TRUE))	/* ���� */

/* ͨѶ���ϱ�־(��д) */
#define GET_EXIO_COMMU_ERR(exio_id)           (EK_SLP_mst_get_exio_fg(exio_id,EK_FG_COMMU_ERR))         /* ��ȡ */
#define SET_EXIO_COMMU_ERR(exio_id)           (EK_SLP_mst_set_exio_fg(exio_id,EK_FG_COMMU_ERR,FALSE))	/* ��λ */
#define CLR_EXIO_COMMU_ERR(exio_id)           (EK_SLP_mst_set_exio_fg(exio_id,EK_FG_COMMU_ERR,TRUE))	/* ���� */

/* IO��չ��汾��Ϣ���ñ�־(ֻ��) */
#define GET_EXIO_VER_READY(exio_id)           (EK_SLP_mst_get_exio_fg(exio_id,EK_FG_VER_READY))

/* IO��չ��״̬��Ϣ���ñ�־(ֻ��) */
#define GET_EXIO_HDW_STATUS_READY(exio_id)    (EK_SLP_mst_get_exio_fg(exio_id,EK_FG_HDW_STATUS_READY))

/* ����IO��չ��Ӳ������(��д) */
#define SET_EXIO_HDW_CFG_UPDATE(exio_id)      (EK_SLP_mst_set_exio_fg(exio_id,EK_FG_UPDATE_HDW_CFG,FALSE))

/* IO��չ��Ӳ����ʼ��(��д) */
#define SET_EXIO_INIT_HDW(exio_id)            (EK_SLP_mst_set_exio_fg(exio_id,EK_FG_INIT_HDW,FALSE))

/* IO��չ��汾�� */
#define GET_EXIO_VER(exio_id)                 (EK_SLP_mst_get_exio_ver(exio_id))

/* ���������� */
#define GET_EXIO_DI(exio_id, n)               (EK_SLP_mst_get_exio_cell_data(EK_RSRC_DI,exio_id,n,TRUE))  /* ��ȡ */

/* �̵������ */
#define GET_EXIO_DO(exio_id, n)               (EK_SLP_mst_get_exio_cell_data(EK_RSRC_DO,exio_id,n,TRUE))  /* ��ȡ */
#define SET_EXIO_DO(exio_id, n)               (EK_SLP_mst_set_exio_cell_data(EK_RSRC_DO,exio_id,n,1))     /* ��λ */
#define CLR_EXIO_DO(exio_id, n)               (EK_SLP_mst_set_exio_cell_data(EK_RSRC_DO,exio_id,n,0))     /* ���� */

/* ���뿪�� */
#define GET_EXIO_SW(exio_id, n)               (EK_SLP_mst_get_exio_cell_data(EK_RSRC_SW,exio_id,n,TRUE))  /* ��ȡ */

/* �¶�ֵ */
#define GET_EXIO_TMP(exio_id, n)              (EK_SLP_mst_get_exio_cell_data(EK_RSRC_TMP,exio_id,n,TRUE)) /* ��ȡ */

/* ѹ��ֵ */
#define GET_EXIO_PRS(exio_id, n)              (EK_SLP_mst_get_exio_cell_data(EK_RSRC_PRS,exio_id,n,TRUE)) /* ��ȡ */

/* �������ͷ����� */
#define GET_EXIO_ST_PV(exio_id, n)            (EK_SLP_mst_get_exio_cell_data(EK_RSRC_EEV,exio_id,n,TRUE)) /* ��ȡ */
#define GET_EXIO_ST_SV(exio_id, n)            (EK_SLP_mst_get_exio_cell_data(EK_RSRC_EEV,exio_id,n,FALSE))
#define SET_EXIO_ST_SV(exio_id, n, val)       (EK_SLP_mst_set_exio_cell_data(EK_RSRC_EEV,exio_id,n,val))  /* ���� */

/* ���ת�� */
#define GET_EXIO_FAN_PV(exio_id, n)           (EK_SLP_mst_get_exio_cell_data(EK_RSRC_FAN,exio_id,n,TRUE)) /* ��ȡ */
#define SET_EXIO_FAN_SV(exio_id, n, val)      (EK_SLP_mst_set_exio_cell_data(EK_RSRC_FAN,exio_id,n,val))  /* ���� */

/* ����� */
#define GET_EXIO_PH(exio_id)                  (EK_SLP_mst_get_exio_cell_data(EK_RSRC_PH,exio_id,0,TRUE))  /* ��ȡ */

EK_SLP_NT_DLG_EXT BOOL EK_SLP_mst_get_exio_fg(U08 exio_id, U08 fg_type);
EK_SLP_NT_DLG_EXT void EK_SLP_mst_set_exio_fg(U08 exio_id, U08 fg_type, BOOL is_clr);
EK_SLP_NT_DLG_EXT U08* EK_SLP_mst_get_exio_ver(U08 exio_id);
EK_SLP_NT_DLG_EXT I16 EK_SLP_mst_get_exio_cell_data(U08 rsrc_type, U08 exio_id, U08 idx, U08 is_read);
EK_SLP_NT_DLG_EXT void EK_SLP_mst_set_exio_cell_data(U08 rsrc_type, U08 exio_id, U08 idx, I16 val);

#else
/***********************************************************************
 ����Ϊ��ģ���ⲿ�ӿڣ��ɹ�����ģ��ʹ��
************************************************************************/
/* ͨѶ���ϱ�־(��д) */
#define GET_EXIO_COMMU_ERR                    (EK_SLP_slv_get_exio_commu_err())
#define SET_EXIO_COMMU_ERR                    (EK_SLP_slv_set_exio_commu_err(FALSE))
#define CLR_EXIO_COMMU_ERR                    (EK_SLP_slv_set_exio_commu_err(TRUE))

/* ��ȡ: �¶�̽ͷ���� */
#define GET_EXIO_TMP_NTC_TYPE(n)              (EK_SLP_slv_get_exio_hdw_cfg_data(EK_CFG_TMP_NTC_TYPE, n))

/* ��ȡ: �������ŷ�ʽ */
#define GET_EXIO_EEV_FREQ_TYPE(n)             (EK_SLP_slv_get_exio_hdw_cfg_data(EK_CFG_EEV_FREQ_TYPE, n))

/* ��ȡ: ��������Ƶ�� */
#define GET_EXIO_EEV_FREQ(n)                  (EK_SLP_slv_get_exio_hdw_cfg_data(EK_CFG_EEV_FREQ, n))

/* ��ȡ: ��������� */
#define GET_EXIO_EEV_MAX_STEP(n)              (EK_SLP_slv_get_exio_hdw_cfg_data(EK_CFG_EEV_MAX_STEP, n))

/* ��ȡ: �������ͷ���ʼ��ǰ���� */
#define GET_EXIO_EEV_ST_PV_INIT(n)            (EK_SLP_slv_get_exio_hdw_init_data(EK_INIT_EEV_ST_PV, n))

/* ��ȡ: �̵������(λ) */
#define GET_EXIO_DO_BIT(n)                    (EK_SLP_slv_get_exio_cell_data(EK_RSRC_DO, n))

/* ���: �̵������(λ) */
#define CLR_EXIO_DO_BIT(n)                    (EK_SLP_slv_set_exio_cell_data(EK_RSRC_DO, n, 0))

/* ��ȡ: �������ͷ�Ŀ�경�� */
#define GET_EXIO_EEV_ST_SV(n)                 (EK_SLP_slv_get_exio_cell_data(EK_RSRC_EEV, n))

/* ���: �������ͷ�Ŀ�경�� */
#define CLR_EXIO_EEV_ST_SV(n)                 (EK_SLP_slv_set_exio_cell_data(EK_RSRC_EEV, n, 0))

/* ��ȡ: ����PWM���Ŀ�경�� */
#define GET_EXIO_FAN_SV(n)                    (EK_SLP_slv_get_exio_cell_data(EK_RSRC_FAN, n))

EK_SLP_NT_DLG_EXT BOOL EK_SLP_slv_get_exio_commu_err(void);
EK_SLP_NT_DLG_EXT void EK_SLP_slv_set_exio_commu_err(BOOL is_clr);
EK_SLP_NT_DLG_EXT I16 EK_SLP_slv_get_exio_hdw_cfg_data(U08 cfg_type, U08 idx);
EK_SLP_NT_DLG_EXT I16 EK_SLP_slv_get_exio_hdw_init_data(U08 init_type, U08 idx);
EK_SLP_NT_DLG_EXT I16 EK_SLP_slv_get_exio_cell_data(U08 rsrc_type, U08 idx);
EK_SLP_NT_DLG_EXT void EK_SLP_slv_set_exio_cell_data(U08 rsrc_type, U08 idx, I16 val);

#endif

