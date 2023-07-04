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

