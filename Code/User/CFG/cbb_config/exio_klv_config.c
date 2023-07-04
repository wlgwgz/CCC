/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: exio_klv_config.c
 �ļ�����: IO��չ��������
 ������  : J.H.
 ��������: 2022.01.14
*******************************************************************************/
#define EXIO_KLV_CONFIG_GLOBAL
#include "includes.h"           /* �����ƶ������λ��! */

#ifdef EXIO_KLV_MASTER
/* ========================================================================================== */
/* �� �� �� ��                                                                                */
/* ========================================================================================== */
#if 0
const U08 EKC_RDS_CFG_INVT[] =
{
    EKC_REMOTE_CFG_INVT_AD,
    EKC_REMOTE_CFG_INVT_STATES,
    EKC_REMOTE_CFG_INVT_CUR_WAVE,
    EKC_REMOTE_CFG_INVT_PARAM,
    EKC_REMOTE_CFG_INVT_PARAM_FA_1,
    EKC_REMOTE_CFG_INVT_PARAM_FA_2,
    EKC_REMOTE_CFG_INVT_PARAM_FB,
    EKC_REMOTE_CFG_INVT_PARAM_F10,
    EKC_REMOTE_CFG_INVT_EC_FAN_FG,
    EKC_REMOTE_CFG_INVT_PARAM_F9,
};

const U08 EKC_RDS_CFG_INVT_FAN[] = 
{
    EKC_REMOTE_CFG_INVT_AD,
    EKC_REMOTE_CFG_INVT_STATES,
    EKC_REMOTE_CFG_INVT_CUR_WAVE,
    EKC_REMOTE_CFG_INVT_PARAM,
    EKC_REMOTE_CFG_INVT_PARAM_FA_1,
    EKC_REMOTE_CFG_INVT_PARAM_FA_2,
    EKC_REMOTE_CFG_INVT_PARAM_FB,
    EKC_REMOTE_CFG_INVT_PARAM_F10,
    EKC_REMOTE_CFG_INVT_EC_FAN_FG,
    EKC_REMOTE_CFG_INVT_PARAM_F9,
};

/* ��������: IO��չ����֧�ֵ�Զ������ */
const U08 EKC_REMOTE_TYPE_USED[] = 
{
    EK_REMOTE_INVT,         /* ���ձ�Ƶѹ������ */
    EK_REMOTE_INVT_FAN,     /* ���ձ�Ƶ������� */
};

U08 EKC_get_remote_type_used_num(void)
{
    return ARRELE(EKC_REMOTE_TYPE_USED);
}

U08 EKC_get_remote_info_baseAddr(U08 remoteType)
{
    switch (remoteType)
    {
        case EK_REMOTE_INVT:           return val_invt_addr;                /* ���ձ�Ƶ����(ѹ��������PWM���) */
        case EK_REMOTE_INVT_FAN:       return 110;                          /* ���ձ�Ƶ�������(����EC���) */
        case EK_REMOTE_KM_INVT_FAN:    return val_ec_fan_base_addr;         /* ��óEC��� */
        case EK_REMOTE_ZSFN_INVT_FAN:  return val_ec_fan_base_addr;         /* ˳���ĸ�EC��� */          
    }

    return 0;
}


U08 EKC_get_remote_info_deviceMax(U08 remoteType)
{
    switch (remoteType)
    {
        case EK_REMOTE_INVT:           return 1;                            /* ���ձ�Ƶ����(ѹ��������PWM���) */
        case EK_REMOTE_INVT_FAN:       return 2;                            /* ���ձ�Ƶ�������(����EC���) */
        case EK_REMOTE_KM_INVT_FAN:    return ECFAN_MAX;                    /* ��óEC��� */   
        case EK_REMOTE_ZSFN_INVT_FAN:  return ECFAN_MAX;                    /* ˳���ĸ�EC��� */   
    }

    return 0;
}

const U08* EKC_get_remote_info_cfgList(U08 remoteType)
{
    switch (remoteType)
    {
        case EK_REMOTE_INVT:           return EKC_RDS_CFG_INVT;             /* ���ձ�Ƶ����(ѹ��������PWM���) */
        case EK_REMOTE_INVT_FAN:       return EKC_RDS_CFG_INVT_FAN;         /* ���ձ�Ƶ�������(����EC���) */
        case EK_REMOTE_KM_INVT_FAN:    return PNULL;                        /* ��óEC��� */
        case EK_REMOTE_ZSFN_INVT_FAN:  return PNULL;                        /* ˳���ĸ�EC��� */
    }

    return PNULL;
}

U08 EKC_get_remote_info_listLen(U08 remoteType)
{
    switch (remoteType)
    {
        case EK_REMOTE_INVT:           return ARRELE(EKC_RDS_CFG_INVT);     /* ���ձ�Ƶ����(ѹ��������PWM���) */
        case EK_REMOTE_INVT_FAN:       return ARRELE(EKC_RDS_CFG_INVT_FAN); /* ���ձ�Ƶ�������(����EC���) */
        case EK_REMOTE_KM_INVT_FAN:    return 0;                            /* ��óEC��� */
        case EK_REMOTE_ZSFN_INVT_FAN:  return 0;                            /* ˳���ĸ�EC��� */
    }

    return 0;    
}

U16 EKC_get_remote_cfg_funcCode(U08 cfg_id)
{
    switch (cfg_id)
    {
        case EKC_REMOTE_CFG_INVT_AD:                 /* 4635 �����е�����ADֵ */
        case EKC_REMOTE_CFG_INVT_STATES:             /* ��Ƶ��״̬ */
        case EKC_REMOTE_CFG_INVT_CUR_WAVE:           /* 3337 �����Ʋ� */
        case EKC_REMOTE_CFG_INVT_PARAM:              /* �������� */      
        case EKC_REMOTE_CFG_INVT_PARAM_FA_1:         /* FA����(��1��) */
        case EKC_REMOTE_CFG_INVT_PARAM_FA_2:         /* FA����(��2��) */
        case EKC_REMOTE_CFG_INVT_PARAM_FB:           /* FB���� */
        case EKC_REMOTE_CFG_INVT_PARAM_F10:          /* F10���� */        
        case EKC_REMOTE_CFG_INVT_EC_FAN_FG:          /* 1537 ec���ָ���ѹ�ֱ��� */
        case EKC_REMOTE_CFG_INVT_PARAM_F9:           /* F10���� */
            return MB_RD_HOLDING_REGISTERS;
    }

    return 0xff;
}

U16 EKC_get_remote_cfg_commuIntv(U08 cfg_id)
{
    switch (cfg_id)
    {
        case EKC_REMOTE_CFG_INVT_AD:                 /* 4635 �����е�����ADֵ */
        case EKC_REMOTE_CFG_INVT_STATES:             /* ��Ƶ��״̬ */
        case EKC_REMOTE_CFG_INVT_CUR_WAVE:           /* 3337 �����Ʋ� */
            return EK_INTV_1_SEC;

        case EKC_REMOTE_CFG_INVT_PARAM:              /* �������� */      
        case EKC_REMOTE_CFG_INVT_PARAM_FA_1:         /* FA����(��1��) */
        case EKC_REMOTE_CFG_INVT_PARAM_FA_2:         /* FA����(��2��) */
        case EKC_REMOTE_CFG_INVT_PARAM_FB:           /* FB���� */
        case EKC_REMOTE_CFG_INVT_PARAM_F10:          /* F10���� */        
        case EKC_REMOTE_CFG_INVT_EC_FAN_FG:          /* 1537 ec���ָ���ѹ�ֱ��� */
        case EKC_REMOTE_CFG_INVT_PARAM_F9:           /* F10���� */
            return EK_INTV_10_SEC;
    }

    return EK_INTV_INF;
}

U16 EKC_get_remote_cfg_prior(U08 cfg_id)
{
    switch (cfg_id)
    {
        case EKC_REMOTE_CFG_INVT_AD:                 /* 4635 �����е�����ADֵ */
        case EKC_REMOTE_CFG_INVT_STATES:             /* ��Ƶ��״̬ */
        case EKC_REMOTE_CFG_INVT_CUR_WAVE:           /* 3337 �����Ʋ� */
        case EKC_REMOTE_CFG_INVT_PARAM:              /* �������� */      
        case EKC_REMOTE_CFG_INVT_PARAM_FA_1:         /* FA����(��1��) */
        case EKC_REMOTE_CFG_INVT_PARAM_FA_2:         /* FA����(��2��) */
        case EKC_REMOTE_CFG_INVT_PARAM_FB:           /* FB���� */
        case EKC_REMOTE_CFG_INVT_PARAM_F10:          /* F10���� */        
        case EKC_REMOTE_CFG_INVT_EC_FAN_FG:          /* 1537 ec���ָ���ѹ�ֱ��� */
        case EKC_REMOTE_CFG_INVT_PARAM_F9:           /* F10���� */
            return Q_PRIO_LOW;
    }

    return Q_PRIO_LOW;
}

U16 EKC_get_remote_cfg_dataAddr(U08 cfg_id)
{
    switch (cfg_id)
    {
        case EKC_REMOTE_CFG_INVT_AD:          return INVT_ADDR_AD;               /* 4635 �����е�����ADֵ */
        case EKC_REMOTE_CFG_INVT_STATES:      return invt_states_get_addr();     /* ��Ƶ��״̬ */
        case EKC_REMOTE_CFG_INVT_CUR_WAVE:    return INVT_ADDR_CUR_WAVE;         /* 3337 �����Ʋ� */
        case EKC_REMOTE_CFG_INVT_PARAM:       return invt_paras_get_addr();      /* �������� */
        case EKC_REMOTE_CFG_INVT_PARAM_FA_1:  return invt_paras_fa_get_addr();   /* FA����(��1��) */
        case EKC_REMOTE_CFG_INVT_PARAM_FA_2:  return FA_ADDR(FA_NUM1);           /* FA����(��2��) */
        case EKC_REMOTE_CFG_INVT_PARAM_FB:    return invt_paras_fb_get_addr();   /* FB���� */
        case EKC_REMOTE_CFG_INVT_PARAM_F10:   return invt_paras_f10_get_addr();  /* F10���� */
        case EKC_REMOTE_CFG_INVT_EC_FAN_FG:   return INVT_ADDR_EC_FAN_FG;        /* 1537 ec���ָ���ѹ�ֱ��� */
        case EKC_REMOTE_CFG_INVT_PARAM_F9:    return invt_paras_f9_get_addr();   /* F10���� */
    }

    return 0xffff;
}

U16 EKC_get_remote_cfg_dataNum(U08 cfg_id)
{
    switch (cfg_id)
    {
        case EKC_REMOTE_CFG_INVT_AD:          return INVT_AD_SIZE;               /* 4635 �����е�����ADֵ */       /* 2 */
        case EKC_REMOTE_CFG_INVT_STATES:      return invt_states_get_size();     /* ��Ƶ��״̬ */                  /* ��19����17 */
        case EKC_REMOTE_CFG_INVT_CUR_WAVE:    return INVT_CUR_WAVE_SIZE;         /* 3337 �����Ʋ� */               /* 1 */
        case EKC_REMOTE_CFG_INVT_PARAM:       return invt_paras_get_size();      /* �������� */                    /* 16 */
        case EKC_REMOTE_CFG_INVT_PARAM_FA_1:  return FA_NUM1;                    /* FA����(��1��) */               /* 40 */
        case EKC_REMOTE_CFG_INVT_PARAM_FA_2:  return FA_MAX - FA_NUM1;           /* FA����(��2��) */               /* 25 */
        case EKC_REMOTE_CFG_INVT_PARAM_FB:    return invt_paras_fb_get_size();   /* FB���� */                      /* 10 */
        case EKC_REMOTE_CFG_INVT_PARAM_F10:   return invt_paras_f10_get_size();  /* F10���� */                     /* 10 */
        case EKC_REMOTE_CFG_INVT_EC_FAN_FG:   return INVT_EC_FAN_FG_SIZE;        /* 1537 ec���ָ���ѹ�ֱ��� */   /* 1 */
        case EKC_REMOTE_CFG_INVT_PARAM_F9:    return invt_paras_f9_get_size();   /* F10���� */                     /* 1 */
    }

    return 0;
}
#endif

/*******************************************************************************
 �� ��: EKC_mst_get_exio_tmp_cfg()
 �� ��: ��ȡIO��չ���¶�̽ͷ��������(����)
 �� ��: cfg_type--IO��չ��Ӳ����������; 
		rsrc_id--IO��չ��Ӳ����ԴID; 
 �� ��: IO��չ���¶�̽ͷ��������
 �� ��: ��
*******************************************************************************/
I16 EKC_mst_get_exio_tmp_cfg(U08 cfg_type, U08 rsrc_id)
{
#ifdef USE_ODU_HL081B		/* HL081B */
    if (USE_PUNP_NTC)
    {
        return NTC_PUNP_NORMAL_10K;
    }

    if (USE_TCL_NTC)
    {
        return (rsrc_id == EXIO_TEMP_04) ? NTC_TCL_DSC_5K : NTC_TCL_NORMAL_10K;
    }

    if (USE_CHANGHONG_NTC)
    {
        return (rsrc_id == EXIO_TEMP_04) ? NTC_CHANGHONG_DSC_3d77K : NTC_PUNP_NORMAL_10K;
    }

    if (USE_PAIWO_NTC)
    {
        return NTC_PAIWO_NORMAL_10K;
    }

    if (USE_NIUENTAI_NTC)
    {
        return (rsrc_id == EXIO_TEMP_04) ? NTC_NIUENTAI_DSC_50K : NTC_NIUENTAI_NORMAL_5K;
    }
#else						/* HL081A */
    if (USE_PUNP_NTC)
    {
        return NTC_PUNP_NORMAL_10K;
    }

    if (USE_TCL_NTC)
    {
        return (rsrc_id == EXIO_TEMP_04) ? NTC_TCL_DSC_5K : NTC_TCL_NORMAL_10K;
    }

    if (USE_CHANGHONG_NTC)
    {
        return (rsrc_id == EXIO_TEMP_04) ? NTC_CHANGHONG_DSC_3d77K : NTC_PUNP_NORMAL_10K;
    }    
#endif
   
    return 0xffff;          
}

/*******************************************************************************
 �� ��: EKC_mst_get_exio_eev_cfg()
 �� ��: ��ȡIO��չ�������������(����)
 �� ��: cfg_type--IO��չ��Ӳ����������; 
        rsrc_id--IO��չ��Ӳ����ԴID; 
 �� ��: IO��չ�������������
 �� ��: ��
*******************************************************************************/
I16 EKC_mst_get_exio_eev_cfg(U08 cfg_type, U08 rsrc_id)
{
    switch (rsrc_id)
    {
        /* ���� */
        case EEV_ID_1:
            switch (cfg_type)
            {
                /* �������ŷ�ʽ */
                case EK_CFG_EEV_FREQ_TYPE:  return val_motor_fre_type;

                /* ��������Ƶ�� */
                case EK_CFG_EEV_FREQ:       return val_motor_step_fre;

                /* ��������� */
                case EK_CFG_EEV_MAX_STEP:   return val_motor_step_max;
            }

        /* ���� */
        case EEV_ID_2:
            switch (cfg_type)
            {
                /* �������ŷ�ʽ */
                case EK_CFG_EEV_FREQ_TYPE:  return val_s_motor_fre_type;

                /* ��������Ƶ�� */
                case EK_CFG_EEV_FREQ:       return val_s_motor_step_fre;
                
                /* ��������� */
                case EK_CFG_EEV_MAX_STEP:   return val_s_motor_step_max;
            }
    }
    
    return 0xffff;          
}

/*******************************************************************************
 �� ��: EKC_mst_get_exio_hdw_init()
 �� ��: ��ȡIO��չ������ʼ������(����)
 �� ��: init_type--IO��չ��Ӳ����ʼ������; 
		rsrc_id--IO��չ��Ӳ����ԴID; 
 �� ��: IO��չ������ʼ������
 �� ��: ��
*******************************************************************************/
I16 EKC_mst_get_exio_hdw_init(U08 init_type, U08 rsrc_id)
{
    static BOL fg_first_init[EEV_ID_MAX] = {TRUE, TRUE};
    
    switch (rsrc_id)
    {
        /* ���� */
        case EEV_ID_1:
            switch (init_type)
            {
                /* �����ʼ��ǰ���� */
                case EK_INIT_EEV_ST_PV:
                    /* ��������ʱ����ִ�г�ʼ�� */
                    if (val_motor_use == EEV_UNUSED)
                    {
                        fg_first_init[EEV_ID_1] = TRUE;
                        return 0;
                    }
                    
                    if (GET_EXIO_HDW_STATUS_READY(0)
                    && fg_first_init[EEV_ID_1] == FALSE
                    )
                    {
                        return eev_get_init_step_pv(EEV_ID_1);
                    }
                    else
                    {
                        fg_first_init[EEV_ID_1] = FALSE;
                        return (U32)val_motor_step_max*12/10;
                    }
            }

        /* ���� */
        case EEV_ID_2:
            switch (init_type)
            {
                /* �����ʼ��ǰ���� */
                case EK_INIT_EEV_ST_PV:  
                    /* ��������ʱ����ִ�г�ʼ�� */
                    if (val_s_motor_use == EVI_UNUSED)
                    {
                        fg_first_init[EEV_ID_2] = TRUE;
                        return 0;
                    }

                    if (GET_EXIO_HDW_STATUS_READY(0)
                    && fg_first_init[EEV_ID_2] == FALSE
                    )
                    {
                        return eev_get_init_step_pv(EEV_ID_2);
                    }
                    else
                    {
                        fg_first_init[EEV_ID_2] = FALSE;
                        return (U32)val_s_motor_step_max*12/10;
                    }
            }
    }

    return 0xffff;
}

/*******************************************************************************
 �� ��: EKC_mst_insert_request_node()
 �� ��: ���в��뷢�ͽڵ�(����)
 �� ��: com_id--���ں�; 
        exio_id--IO��չ���; 
 �� ��: �ڵ����״ֵ̬
 �� ��: ��
*******************************************************************************/
U08 EKC_mst_insert_request_node(U08 com_id, U08 exio_id)
{
    U08 rd_succ = COM_SUCC;
    if (exio_id >= 0 && exio_id < EXIO_KLV_MAX)
    {
        QNODE node; 

        node.driverType = DRIVER_MB_MASTER;
        node.data.driverNo = EXIO_KLV_ADDR+exio_id;
        node.data.cmd = MB_USER_DEFINED_EXIO_KLV;
//      node.data.startAddr = ADDR_EXIO_STA_BGN;
//      node.data.length_wr = EXIO_KLV_DATA_SIZE;
//      node.data.length_rd = EXIO_KLV_DATA_SIZE;
//      node.data.pData_wr = (U16 *)&m_exio_klv_data[exio_id];
//      node.data.pData_rd = (U16 *)&m_exio_klv_data[exio_id];
        
        rd_succ = COM_QInsertNode(com_id, Q_PRIO_LOW, &node);
    }
    else
    {
        rd_succ = COM_ERR_MAC_ADDR;     /* �豸��ַ�� */
    }
    
    return rd_succ;
}

#else
/* ========================================================================================== */
/* �� �� �� �� ( IO �� չ �� )                                                                */
/* ========================================================================================== */

/* IO��չ��Ӳ����Դ��Ϣ */
const EXIO_HDW_RSRC_INFO EKC_EXIO_HDW_RSRC_INFO[] =
{
    { EK_RSRC_DI,   EK_BIT|EK_RD,        MAX_DI_NUM,    },/* ����������, 7· */
    { EK_RSRC_DO,   EK_BIT|EK_RD|EK_WR,  MAX_DO_NUM,    },/* �̵������, 8· */
    { EK_RSRC_SW,   EK_BIT|EK_RD,        MAX_SW_BIT,    },/* ���뿪��, 6λ */
    { EK_RSRC_TMP,  EK_RD,               MAX_TEMP_NUM,  },/* �¶�̽ͷ, 10·  */
    { EK_RSRC_PRS,  EK_RD,               MAX_AI_NUM,    },/* ѹ��������, 2· */
    { EK_RSRC_EEV,  EK_RD|EK_WR|EK_INIT, MAX_MOTOR_NUM, },/* �������ͷ�, 2· */
    { EK_RSRC_FAN,  EK_RD|EK_WR,         FREQSPEEDNUM,  },/* ����PWM���, 2· */
    { EK_RSRC_PH,   EK_RD,               1,             },/* ������, 1· */
//    { EK_RSRC_INVT, EK_REMOTE,           1,             },/* ��Ƶ����, 1· */
};

/*******************************************************************************
 �� ��: EKC_get_exio_hdw_rsrc_info_num()
 �� ��: ��ȡIO��չ��Ӳ����Դ��Ϣ��(�ӻ�)
 �� ��: ��
 �� ��: IO��չ��Ӳ����Դ��Ϣ��
 �� ��: ��
*******************************************************************************/
U08 EKC_get_exio_hdw_rsrc_info_num(void)
{
    return ARRELE(EKC_EXIO_HDW_RSRC_INFO);
}

/* IO��չ��Ӳ��������Ϣ */
const EXIO_HDW_CFG_INFO EKC_EXIO_HDW_CFG_INFO[] = 
{
    { EK_CFG_TMP_NTC_TYPE,  EK_RSRC_TMP, }, /* �¶�̽ͷ���� */
    { EK_CFG_EEV_FREQ_TYPE, EK_RSRC_EEV, },	/* �������ŷ�ʽ */
    { EK_CFG_EEV_FREQ,      EK_RSRC_EEV, },	/* ��������Ƶ�� */
    { EK_CFG_EEV_MAX_STEP,  EK_RSRC_EEV, },	/* ��������� */
};

/*******************************************************************************
 �� ��: EKC_get_exio_hdw_cfg_info_num()
 �� ��: ��ȡIO��չ��Ӳ��������Ϣ��(�ӻ�)
 �� ��: ��
 �� ��: IO��չ��Ӳ��������Ϣ��
 �� ��: ��
*******************************************************************************/
U08 EKC_get_exio_hdw_cfg_info_num(void)
{
    return ARRELE(EKC_EXIO_HDW_CFG_INFO);
}

/* IO��չ��Ӳ����ʼ����Ϣ */
const EXIO_HDW_INIT_INFO EKC_EXIO_HDW_INIT_INFO[] = 
{
    { EK_INIT_EEV_ST_PV, EK_RSRC_EEV, },    /* �����ʼ��ǰ���� */
};

/*******************************************************************************
 �� ��: EKC_get_exio_hdw_init_info_num()
 �� ��: ��ȡIO��չ��Ӳ����ʼ����Ϣ��(�ӻ�)
 �� ��: ��
 �� ��: IO��չ��Ӳ����ʼ����Ϣ��
 �� ��: ��
*******************************************************************************/
U08 EKC_get_exio_hdw_init_info_num(void)
{
    return ARRELE(EKC_EXIO_HDW_INIT_INFO);
}

/*******************************************************************************
 �� ��: EKC_slv_exio_hdw_cfg_handler()
 �� ��: IO��չ��Ӳ�����ô���(�ӻ�)
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void EKC_slv_exio_hdw_cfg_handler(void)
{
    U08 i;
    I16 val;

    /* �¶�̽ͷ���� */
    for (i = 0; i < MAX_TEMP_NUM; i++)
    {
        val = GET_EXIO_TMP_NTC_TYPE(i);
        Hdw_Temp_NTC_Type[i] = (val != 0xffff) ? val : 0;
    }

    /* ��·�������ͷ� */
    eev_set_motor_type(EEV_ID_1, GET_EXIO_EEV_FREQ_TYPE(EEV_ID_1));
    eev_set_step_speed(EEV_ID_1, GET_EXIO_EEV_FREQ(EEV_ID_1));
    eev_set_steps_max(EEV_ID_1, GET_EXIO_EEV_MAX_STEP(EEV_ID_1));
    

    /* ��·�������ͷ� */
    eev_set_motor_type(EEV_ID_2, GET_EXIO_EEV_FREQ_TYPE(EEV_ID_2));
    eev_set_step_speed(EEV_ID_2, GET_EXIO_EEV_FREQ(EEV_ID_2));
    eev_set_steps_max(EEV_ID_2, GET_EXIO_EEV_MAX_STEP(EEV_ID_2));
}

/*******************************************************************************
 �� ��: EKC_slv_exio_hdw_init_handler()
 �� ��: IO��չ��Ӳ����ʼ������(�ӻ�)
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void EKC_slv_exio_hdw_init_handler(void)
{
    U08 i;
    
    for(i = 0; i < EEV_ID_MAX; i++)
    {
        CLR_EXIO_EEV_ST_SV(i);
        gIO.io_step_pv[i] = GET_EXIO_EEV_ST_PV_INIT(i);
    }
}

/*******************************************************************************
 �� ��: EKC_slv_exio_eev_init_done()
 �� ��: �ж�IO��չ������ʼ���Ƿ����(�ӻ�)
 �� ��: ��
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL EKC_slv_exio_eev_init_done(void)
{
    U08 i;

    for (i = 0; i < EEV_ID_MAX; i++)
    {
        if (gIO.io_step_sv[i] != gIO.io_step_pv[i])
        {
            return FALSE;
        }
    }

    return TRUE;
}


#endif

