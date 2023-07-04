/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: exio_klv_nt_dlg_slp_0.c
 �ļ�����: IO��չ����-ůͨ������ҵ���߼�Դ�ļ�
 ������  : J.H.
 ��������: 2022.01.14
*******************************************************************************/
#define EK_SLP_NT_DLG_GLOBAL

#define EXIO_KLV_STATIC     /* �ڲ��ӿڲ��� */
#define EXIO_KLV_EXTERN     /* �ⲿ�ӿڲ��� */

/* EXIO_KLVҵ����İ汾 */
#define EXIO_KLV_SLP_CORE_VER                     (0)

#ifdef EXIO_KLV_MASTER
/*******************************************************************************
  �� �� �� ��
*******************************************************************************/
#ifdef EXIO_KLV_STATIC      /* �ڲ��ӿ� */

/* IO��չ��Ӳ��״̬��Ϣ�����С */
#define M_EXIO_CELL_BUF_SIZE                      (100) 

/* IO��չ��״̬��ر�־ */
static U16 m_fg_exio_klv_ver_err[_DIVUCARRY(EXIO_KLV_MAX, 16)];     /* IO��չ��Э��汾��ƥ����ϱ�־ */
static U16 m_fg_exio_commu_err[_DIVUCARRY(EXIO_KLV_MAX, 16)];       /* IO��չ��ͨ�Ź��ϱ�־ */
static U16 m_fg_exio_ver_ready[_DIVUCARRY(EXIO_KLV_MAX, 16)];       /* IO��չ��汾����Ϣ���ñ�־ */
static U16 m_fg_exio_remote_exist[_DIVUCARRY(EXIO_KLV_MAX,16)];     /* IO��չ�����Զ�˵ı�־ */
static U16 m_fg_exio_hdw_status_ready[_DIVUCARRY(EXIO_KLV_MAX, 16)];/* IO��չ��Ӳ��״̬��Ϣ���ñ�־ */
static U16 m_fg_exio_hdw_cfg_update[_DIVUCARRY(EXIO_KLV_MAX, 16)];  /* IO��չ��Ӳ�����ø��±�־ */
static U16 m_fg_exio_init_hdw[_DIVUCARRY(EXIO_KLV_MAX, 16)];        /* IO��չ��Ӳ����ʼ����־ */

/* IO��չ��Ӳ����ʼ��״̬ */
static U08 m_exio_hdw_init_sta[EXIO_KLV_MAX];

/* IO��չ��Ӳ��״̬/�����д�������ַ */
static U16 m_exio_cell_base_addr[EXIO_KLV_MAX][EK_RSRC_MAX];

/* IO��չ����Ϣ���� */
static U16 m_exio_klv_ver_info[EXIO_KLV_MAX];                               /* IO��չ��Э��汾��Ϣ���� */
static U08 m_exio_ver_info[EXIO_KLV_MAX][MAX_VER];                          /* IO��չ��汾����Ϣ���� */
static EXIO_HDW_RSRC_INFO m_exio_hdw_rsrc_info[EXIO_KLV_MAX][EK_RSRC_MAX];  /* IO��չ��Ӳ����Դ��Ϣ���� */
static EXIO_HDW_CFG_INFO m_exio_hdw_cfg_info[EXIO_KLV_MAX][EK_CFG_MAX];     /* IO��չ��Ӳ��������Ϣ���� */
static EXIO_HDW_INIT_INFO m_exio_hdw_init_info[EXIO_KLV_MAX][EK_INIT_MAX];  /* IO��չ��Ӳ����ʼ����Ϣ���� */
static I16 m_exio_cell[EXIO_KLV_MAX][M_EXIO_CELL_BUF_SIZE];                 /* IO��չ��Ӳ��״̬/�����д���� */

/* IO��չ��Ӳ�����ʵ������ */
static U08 m_exio_hdw_rsrc_info_used[EXIO_KLV_MAX];         /* IO��չ��Ӳ����Դ��Ϣʵ������ */
static U08 m_exio_hdw_cfg_info_used[EXIO_KLV_MAX];          /* IO��չ��Ӳ��������Ϣʵ������ */
static U08 m_exio_hdw_init_info_used[EXIO_KLV_MAX];         /* IO��չ��Ӳ����ʼ����Ϣʵ������ */

/* KLV�����ͱ�־ */
static U16 fg_exio_klv_req[EXIO_KLV_MAX][_DIVUCARRY(EK_STAGE_MAX,16)];  
#define GET_EXIO_KLV_REQ(exio_id, req_type)       (GETBIT_N(fg_exio_klv_req[exio_id],req_type)) /* ��ȡ */
#define SET_EXIO_KLV_REQ(exio_id, req_type)       (SETBIT_N(fg_exio_klv_req[exio_id],req_type)) /* ��λ */
#define CLR_EXIO_KLV_REQ(exio_id, req_type)       (CLRBIT_N(fg_exio_klv_req[exio_id],req_type)) /* ���� */

/* �Ƿ�Ϊλ���� */
#define M_IS_BIT_RSRC(exio_id, rsrc_type)         (EK_SLP_mst_get_hdw_rsrc_attr(exio_id,rsrc_type)&EK_BIT)
/* �Ƿ�Ϊ�ɶ� */
#define M_IS_RD_RSRC(exio_id, rsrc_type)          (EK_SLP_mst_get_hdw_rsrc_attr(exio_id,rsrc_type)&EK_RD)
/* �Ƿ�Ϊ��д */
#define M_IS_WR_RSRC(exio_id, rsrc_type)          (EK_SLP_mst_get_hdw_rsrc_attr(exio_id,rsrc_type)&EK_WR)


/*******************************************************************************
 �� ��: EK_SLP_mst_get_hdw_rsrc_attr()
 �� ��: ��ȡIO��չ��Ӳ����Դ����(����)
 �� ��: exio_id--IO��չ���; 
        rsrc_type--Ӳ����Դ����; 
 �� ��: IO��չ��Ӳ����Դ����ֵ
 �� ��: ��
*******************************************************************************/
static U08 EK_SLP_mst_get_hdw_rsrc_attr(U08 exio_id, U08 rsrc_type)
{
    U08 i;
    U08 attr = 0;

    for (i = 0; i < m_exio_hdw_rsrc_info_used[exio_id]; i++)
    {
        if (m_exio_hdw_rsrc_info[exio_id][i].rsrc_type == rsrc_type)
        {
            attr = m_exio_hdw_rsrc_info[exio_id][i].attr;
        }
    }

    return attr;
}

/*******************************************************************************
 �� ��: EK_SLP_mst_get_exio_cell_base_addr()
 �� ��: ��ȡIO��չ��Ӳ��״̬/�����д�������ַ(����)
 �� ��: rsrc_type--Ӳ����Դ����; 
        exio_id--IO��չ���; 
        is_read--�Ƿ�Ϊ�ɶ�(��֮Ϊ��д); 
 �� ��: IO��չ��Ӳ��״̬/�����д�������ַ
 �� ��: ��
*******************************************************************************/
static U16 EK_SLP_mst_get_exio_cell_base_addr(U08 rsrc_type, U08 exio_id, U08 is_read)
{
    U08 attr, num, words;
    U16 base_addr;
    
    attr = m_exio_hdw_rsrc_info[exio_id][rsrc_type].attr;
    num = m_exio_hdw_rsrc_info[exio_id][rsrc_type].num;
    words = (attr & EK_BIT) ? _DIVUCARRY(num, 16) : num;

    if (is_read)             /* �ɶ� */
    {
        if (!M_IS_RD_RSRC(exio_id, rsrc_type))
        {
            return 0xffff;
        }
        base_addr = (attr & EK_RD) ? m_exio_cell_base_addr[exio_id][rsrc_type] : 0xffff;
    }
    else                    /* ��д */
    {
        if (!M_IS_WR_RSRC(exio_id, rsrc_type))
        {
            return 0xffff;
        }
        base_addr = (attr & EK_RD && attr & EK_WR) ? m_exio_cell_base_addr[exio_id][rsrc_type] + words \
                                                 : m_exio_cell_base_addr[exio_id][rsrc_type];
    }
    
    return base_addr;
}

/*******************************************************************************
 �� ��: EK_SLP_mst_fill_exio_hdw_cfg_data()
 �� ��: ���IO��չ��Ӳ����������(����)
 �� ��: exio_id--IO��չ���; 
        buf--���ͻ���; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void EK_SLP_mst_fill_exio_hdw_cfg_data(U08 exio_id, U08* buf)
{
    U08 i, rsrc_id;
    U08 rsrc_attr, rsrc_num;
    U08 cfg_type, rsrc_type;
    I16 cfg_val;
    U08 len = 1;

    /* ������������ */
    for (i = 0; i < m_exio_hdw_cfg_info_used[exio_id]; i++)
    {
        /* �����ѻ�ȡ��IO��չ����Ϣ */
        cfg_type = m_exio_hdw_cfg_info[exio_id][i].cfg_type;
        rsrc_type = m_exio_hdw_cfg_info[exio_id][i].rsrc_type;
        rsrc_attr = m_exio_hdw_rsrc_info[exio_id][rsrc_type].attr;
        rsrc_num = m_exio_hdw_rsrc_info[exio_id][rsrc_type].num;         /* ��ǰӲ������ */

        /* ͷ��Ϣ */
        buf[len++] = cfg_type;                                                    /* Ӳ����Դ��������: 1�ֽ� */
        buf[len++] = (rsrc_attr & EK_BIT) ? _DIVUCARRY(rsrc_num, 16) : rsrc_num;  /* ����: 1�ֽ� */

        /* ����������� */
        for (rsrc_id = 0; rsrc_id < rsrc_num; rsrc_id++)
        {
            switch (rsrc_type)
            {
                /* �¶�̽ͷ */
                case EK_RSRC_TMP:  cfg_val = EKC_mst_get_exio_tmp_cfg(cfg_type, rsrc_id);  break;
        
                /* �������ͷ� */
                case EK_RSRC_EEV:  cfg_val = EKC_mst_get_exio_eev_cfg(cfg_type, rsrc_id);  break;
            }
            buf[len++] = HIBYTE(cfg_val);
            buf[len++] = LOBYTE(cfg_val);
        }
    }

    buf[0] = len - 1;
}


/*******************************************************************************
 �� ��: EK_SLP_mst_fill_exio_hdw_init_data()
 �� ��: ���IO��չ��Ӳ����ʼ������(����)
 �� ��: exio_id--IO��չ���; 
        buf--���ͻ���; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void EK_SLP_mst_fill_exio_hdw_init_data(U08 exio_id, U08 *buf)
{
    U08 i, rsrc_id;
    U08 init_type, rsrc_type, rsrc_attr, rsrc_num;
    I16 init_val;
    U08 len = 1;

    for (i = 0; i < m_exio_hdw_init_info_used[exio_id]; i++)
    {
        init_type = m_exio_hdw_init_info[exio_id][i].init_type;
        rsrc_type = m_exio_hdw_init_info[exio_id][i].rsrc_type;
        rsrc_attr = m_exio_hdw_rsrc_info[exio_id][rsrc_type].attr;
        rsrc_num = m_exio_hdw_rsrc_info[exio_id][rsrc_type].num;

        /* ͷ��Ϣ */
        buf[len++] = init_type;
        buf[len++] = (rsrc_attr & EK_BIT) ? _DIVUCARRY(rsrc_num, 16) : rsrc_num;

        /* ����ʼ������ */
        for (rsrc_id = 0; rsrc_id < rsrc_num; rsrc_id++)
        {
            init_val = EKC_mst_get_exio_hdw_init(init_type, rsrc_id);
            buf[len++] = HIBYTE(init_val);
            buf[len++] = LOBYTE(init_val);
        }
    }
    buf[0] = len - 1;

    m_exio_hdw_init_sta[exio_id] = EK_HDW_INIT_STA_DOING;
}

/*******************************************************************************
 �� ��: EK_SLP_mst_fill_exio_hdw_output_data()
 �� ��: ���IO��չ��Ӳ���������(����)
 �� ��: exio_id--IO��չ���; 
        buf--���ͻ���; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void EK_SLP_mst_fill_exio_hdw_output_data(U08 exio_id, U08* buf)
{
    U08 i, j;
    U08 rsrc_id;
    U08 rsrc_type, rsrc_attr, rsrc_num;
    U08 words;
    I16 val;
    U08 len = 1;

    /* ��ʼ��δ��ɣ��ݲ���� */
    if (m_exio_hdw_init_sta[exio_id] != EK_HDW_INIT_STA_DONE)
    {
        return;
    }

    for (rsrc_id = 0; rsrc_id < m_exio_hdw_rsrc_info_used[exio_id]; rsrc_id++)
    {
        rsrc_type = m_exio_hdw_rsrc_info[exio_id][rsrc_id].rsrc_type;
        rsrc_attr = m_exio_hdw_rsrc_info[exio_id][rsrc_id].attr;
        rsrc_num = m_exio_hdw_rsrc_info[exio_id][rsrc_type].num;
        
        words = (rsrc_attr & EK_BIT) ? _DIVUCARRY(rsrc_num, 16) : rsrc_num;

        if (rsrc_attr & EK_WR)
        {
            buf[len++] = rsrc_type;         /* Ӳ����Դ����: 1�ֽ� */
            buf[len++] = words;             /* ����: 1�ֽ� */

            for (i = 0; i < words; i++)
            {
                val = 0;
                if (M_IS_BIT_RSRC(exio_id, rsrc_type))
                {
                    for (j = 0; j < rsrc_num; j++)
                    {
                        if (EK_SLP_mst_get_exio_cell_data(rsrc_type, exio_id, j, FALSE))
                        {
                            SETBIT(val, j);
                        }
                    }
                }
                else
                {
                    val = EK_SLP_mst_get_exio_cell_data(rsrc_type, exio_id, i, FALSE);
                }       
                buf[len++] = HIBYTE(val);
                buf[len++] = LOBYTE(val);
            }     
        }     
    }

    buf[0] = len - 1;
}

/*******************************************************************************
 �� ��: EK_SLP_mst_cal_exio_cell_base_addr()
 �� ��: ����IO��չ��Ӳ��״̬/�����д�������ַ(����)
 �� ��: exio_id--IO��չ���; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void EK_SLP_mst_cal_exio_cell_base_addr(U08 exio_id)
{
    U08 i, attr, rsrc_type, num, words;
    U16 baseAddr = 0;

    /* �������ݻ���ַ */
    for (i = 0; i < m_exio_hdw_rsrc_info_used[exio_id]; i++)
    {
        attr = m_exio_hdw_rsrc_info[exio_id][i].attr;
        rsrc_type = m_exio_hdw_rsrc_info[exio_id][i].rsrc_type;
        num = m_exio_hdw_rsrc_info[exio_id][i].num;
        m_exio_cell_base_addr[exio_id][rsrc_type] = (attr & EK_RD || attr & EK_WR) ? baseAddr : 0xffff;
        words = (attr & EK_BIT) ? _DIVUCARRY(num, 16) : num;
        baseAddr += (attr & EK_RD) ? words : 0;             /* ���ٿɶ����ݿռ� */
        baseAddr += (attr & EK_WR) ? words : 0;             /* ���ٿ�д���ݿռ� */
    }
}

/*******************************************************************************
 �� ��: EK_SLP_mst_parse_exio_hdw_rsrc_info()
 �� ��: ����IO��չ��Ӳ����Դ��Ϣ(����)
 �� ��: exio_id--IO��չ���; 
        buf--���ջ���; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void EK_SLP_mst_parse_exio_hdw_rsrc_info(U08 exio_id, U08* buf)
{
    U08 total_bytes, itemNum;
    U08 rsrc_id = 0;
    U08 len = 0;

    /* ��Ч�ֽ��� */
    total_bytes = buf[len++];
    itemNum = buf[len++];
    m_exio_hdw_rsrc_info_used[exio_id] = MIN(itemNum, EK_RSRC_MAX);
    
    /* ���ֽڽ��մ��� */
    while (TRUE)
    {
        if ((len > total_bytes) \
        || (len > EXIO_KLV_DATA_BUF_BYTES) \
        || (rsrc_id >= m_exio_hdw_rsrc_info_used[exio_id]))
        {
            break;
        }

        MEMCPY(&m_exio_hdw_rsrc_info[exio_id][rsrc_id], &buf[len], EXIO_HDW_RSRC_INFO_BYTES);
        len += EXIO_HDW_RSRC_INFO_BYTES;

        /* ����Ƿ����Զ��(��δʹ��) */
//        if (m_exio_hdw_rsrc_info[exio_id][rsrc_id].attr & EK_REMOTE)
//        {
//            SETBIT_N(fg_exio_remote, exio_id);
//        }

        rsrc_id++;
    }

    /* ����IO��չ��Ӳ��״̬��Ϣ�������ַ */
    EK_SLP_mst_cal_exio_cell_base_addr(exio_id);

    /* ����IO��չ��Ӳ��������Ϣ */
    SET_EXIO_KLV_REQ(exio_id, EK_STAGE_HDW_CFG_INFO);
}

/*******************************************************************************
 �� ��: EK_SLP_mst_parse_exio_hdw_cfg_info()
 �� ��: ����IO��չ��Ӳ��������Ϣ(����)
 �� ��: exio_id--IO��չ���; 
        buf--���ջ���; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void EK_SLP_mst_parse_exio_hdw_cfg_info(U08 exio_id, U08* buf)
{
    U08 total_bytes, itemNum;
    U08 cfg_id = 0;
    U08 len = 0;

    /* ��Ч�ֽ��� */
    total_bytes = buf[len++];
    itemNum = buf[len++];
    m_exio_hdw_cfg_info_used[exio_id] = MIN(itemNum, EK_CFG_MAX);
    
    /* ���ֽڽ��մ��� */
    while (TRUE)
    {
        if ((len > total_bytes) \
        || (len > EXIO_KLV_DATA_BUF_BYTES) \
        || (cfg_id >= m_exio_hdw_cfg_info_used[exio_id]))
        {
            break;
        }
        
        MEMCPY(&m_exio_hdw_cfg_info[exio_id][cfg_id], &buf[len], EXIO_HDW_CFG_BYTES_INFO);
        len += EXIO_HDW_CFG_BYTES_INFO;

        cfg_id++;    
    }

    /* ����IO��չ��Ӳ����ʼ����Ϣ */
    SET_EXIO_KLV_REQ(exio_id, EK_STAGE_HDW_INIT_INFO);
}

/*******************************************************************************
 �� ��: EK_SLP_mst_parse_exio_hdw_init_info()
 �� ��: ����IO��չ��Ӳ����ʼ����Ϣ(����)
 �� ��: exio_id--IO��չ���; 
        buf--���ջ���; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void EK_SLP_mst_parse_exio_hdw_init_info(U08 exio_id, U08* buf)
{
    U08 total_bytes, itemNum;
    U08 init_id = 0;
    U08 len = 0;

    /* ��Ч�ֽ��� */
    total_bytes = buf[len++];
    itemNum = buf[len++];
    m_exio_hdw_init_info_used[exio_id] = MIN(itemNum, EK_INIT_MAX);
    
    /* ���ֽڽ��մ��� */
    while (TRUE)
    {
        if ((len > total_bytes) \
        || (len > EXIO_KLV_DATA_BUF_BYTES) \
        || (init_id >= m_exio_hdw_init_info_used[exio_id]))
        {
            break;
        }
                
        MEMCPY(&m_exio_hdw_init_info[exio_id][init_id], &buf[len], EXIO_HDW_INIT_BYTES_INFO);
        len += EXIO_HDW_INIT_BYTES_INFO;

        init_id++;
    }

    /* ����IO��չ��Ӳ���������� */
    SET_EXIO_KLV_REQ(exio_id, EK_STAGE_HDW_CFG_DATA); 
}

/*******************************************************************************
 �� ��: EK_SLP_mst_parse_exio_hdw_status_info()
 �� ��: ����IO��չ��Ӳ��״̬��Ϣ(����)
 �� ��: exio_id--IO��չ���; 
        buf--���ջ���; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void EK_SLP_mst_parse_exio_hdw_status_info(U08 exio_id, U08* buf)
{
    U08 i;
    U08 total_bytes, rsrc_type, rsrc_num, rsrc_attr; 
    U08 words;
    U16 base_addr, idx;
    U08 len = 0;

    /* ��ʼ��״̬ */
    m_exio_hdw_init_sta[exio_id] = buf[len++];

    /* ��Ч�ֽ��� */
    total_bytes = buf[len++];

    /* ���ֽڽ��մ��� */
    while (TRUE)
    {
        if ((len > total_bytes) || (len >= EXIO_KLV_DATA_BUF_BYTES))
        {
            break;
        }

        rsrc_type = buf[len++];
        if (rsrc_type < EK_RSRC_MAX)
        {
            rsrc_num = m_exio_hdw_rsrc_info[exio_id][rsrc_type].num;
            rsrc_attr = m_exio_hdw_rsrc_info[exio_id][rsrc_type].attr;
        }
        else
        {
            break;
        }

        words = (rsrc_attr & EK_BIT) ? _DIVUCARRY(rsrc_num, 16) : rsrc_num;

        base_addr = m_exio_cell_base_addr[exio_id][rsrc_type];

        for (i = 0; i < words; i++)
        {
            idx = base_addr + i;
            if (idx < M_EXIO_CELL_BUF_SIZE)
            {
                m_exio_cell[exio_id][idx] = MAKEWORD(buf[len+i*2], buf[len+i*2+1]);
            }
        }

        len += (rsrc_attr & EK_BIT) ? _DIVUCARRY(rsrc_num, 16)*2 : rsrc_num*2;
    }

    /* IO��չ��״̬���ݿ��� */
    SETBIT_N(m_fg_exio_hdw_status_ready, exio_id);

    if (GETBIT_N(m_fg_exio_remote_exist, exio_id))
    {
        SET_EXIO_KLV_REQ(exio_id, EK_STAGE_REMOTE_WR_RD);   /* �л�����Զ������ */
    }
    else
    {
        SET_EXIO_KLV_REQ(exio_id, EK_STAGE_HDW_WR_RD);      /* ���ַ���Ӳ��������� */
    }   
}

#if 0
/*******************************************************************************
 �� ��: EK_mst_fill_exio_remote_cfg()
 �� ��: 
 �� ��: exio_id--; 
        buf--; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void EK_mst_fill_exio_remote_cfg(U08 exio_id, U08 *buf)
{
    U08 i;
    U08 cfgTotal, itemMax, baseAddr, deviceMax, listLen;
    const U08 *cfgList;
    U16 dataAddr;
    EXIO_REMOTE_CFG remoteCfg;
    U08 itemCnt = 0;
    BOOL isFrameFull = FALSE;
    U08 len = 0;

    static U08 frameCur, typeRec, cfgRec;
   
    /* ����һ֡�����ݵ�Ԫ�������� */
    itemMax = _DIVUINT((EXIO_KLV_DATA_BUF_BYTES-5), EXIO_REMOTE_CFG_BYTES);

    /* �������������� */
    for (i = 0; i < EKC_get_remote_type_used_num(); i++)
    {
        cfgTotal += EKC_get_remote_info_listLen(i);
    }

    /* ���ͷ��Ϣ */
    buf[len++] = _DIVUCARRY((cfgTotal*EXIO_REMOTE_CFG_BYTES), (EXIO_KLV_DATA_BUF_BYTES-5));   /* ��֡�� */
    buf[len++] = frameCur++;                                                            /* ��ǰ֡�� */
    buf[len++] = EXIO_REMOTE_CFG_WORDS;                                                 /* ���ݵ�Ԫ���� */    
    buf[len++] = cfgTotal;                                                              /* ���������� */
    len++;                                                                              /* ���ֽ��� */

    while (TRUE)
    {   
        /* ����������Զ�����ͣ����� */
        if (typeRec >= EKC_get_remote_type_used_num())
        {
            frameCur = 0;
            typeRec = 0;
            cfgRec = 0;
            break;
        }
       
        baseAddr = EKC_get_remote_info_baseAddr(typeRec);
        deviceMax = EKC_get_remote_info_deviceMax(typeRec);
        cfgList = EKC_get_remote_info_cfgList(typeRec);
        listLen = EKC_get_remote_info_listLen(typeRec);
        
        for (i = cfgRec; i < listLen; i++)
        {
            /* ������һ֡�����¼�ж���� */
            if (itemCnt++ >= itemMax)
            {
                cfgRec = i;
                isFrameFull = TRUE;
                break;
            }
        
            /* ��ʼ�� */
            MEMCLR(&remoteCfg, EXIO_REMOTE_CFG_BYTES);

            dataAddr = EKC_get_remote_cfg_dataAddr(cfgList[i]);

            /* �������ݵ�Ԫ */
            remoteCfg.baseAddr = baseAddr;
            remoteCfg.deviceMax = deviceMax;
            remoteCfg.funcCode = EKC_get_remote_cfg_funcCode(cfgList[i]);
            remoteCfg.commuIntv = EKC_get_remote_cfg_commuIntv(cfgList[i]);
            remoteCfg.prior = EKC_get_remote_cfg_prior(cfgList[i]);
            remoteCfg.dataAddrH = HIBYTE(dataAddr);
            remoteCfg.dataAddrL = LOBYTE(dataAddr);
            remoteCfg.dataNum = EKC_get_remote_cfg_dataNum(cfgList[i]);

            /* ��� */
            MEMCPY(&buf[len], &remoteCfg, EXIO_REMOTE_CFG_BYTES);
            len += EXIO_REMOTE_CFG_BYTES;
        }

        /* ����ǰ֡δ�������������һ������ */
        if (!isFrameFull)
        {
            cfgRec = 0;
            typeRec++;
        }
    }
    buf[0] = len - 1;
}

void EK_mst_parse_exio_rds_data(U08 exio_id, U08* buf)
{
    SET_EXIO_KLV_REQ(exio_id, EK_STAGE_HDW_WR_RD);
}

void EK_mst_parse_exio_rds_cfg_confirm(U08 exio_id, U08* buf)
{
    U08 frameCnt, frameIdx;
    U08 len = 1;
    
    frameCnt = buf[len++];                   /* ��֡�� */
    frameIdx = buf[len++];                   /* ��ǰ֡�� */

    if (frameIdx+1 >= frameCnt)
    {
        SET_EXIO_KLV_REQ(exio_id, EK_STAGE_HDW_WR_RD);
    }
    else
    {
        SET_EXIO_KLV_REQ(exio_id, EK_STAGE_REMOTE_CFG_DATA);
    }
}
#endif

/*******************************************************************************
 �� ��: EK_SLP_mst_init()
 �� ��: ʹ��IO��չ��ǰ���ϵ��ʼ������(����)
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void EK_SLP_mst_init(void)
{
    U08 i;

    /* IO��չ����ر�־��ʼ�� */
    ARRCLR(m_fg_exio_klv_ver_err);
    ARRCLR(m_fg_exio_commu_err);
    ARRCLR(m_fg_exio_ver_ready);
    ARRCLR(m_fg_exio_remote_exist);
    ARRCLR(m_fg_exio_hdw_status_ready);
    ARRCLR(m_fg_exio_hdw_cfg_update);

    /* IO��չ��Ӳ����ʼ��״̬��ʼ�� */
    ARRSET(m_exio_hdw_init_sta, EK_HDW_INIT_STA_PLAN);

    /* IO��չ�������ͱ�־��ʼ�� */
    ARR2SET(fg_exio_klv_req, 0);
    for (i = 0; i < EKC_EXIO_KLV_USED; i++)
    {
       SET_EXIO_KLV_REQ(i, EK_STAGE_KLV_CORE_SLP);
       SETBIT_N(m_fg_exio_commu_err, i);            /* �ϵ��Ĭ�ϳ�ʼ��ΪͨѶ���ϣ������״��ϵ��ⲻ�������ͨѶ���� */
    }
}

/*******************************************************************************
 �� ��: EK_SLP_mst_timer()
 �� ��: IO��չ���ʱ��(����)
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void EK_SLP_mst_timer(void)
{
    ;
}

/*******************************************************************************
 �� ��: EK_SLP_mst_commu_err_handler()
 �� ��: IO��չ��ͨ�Ź��ϴ���(����)
 �� ��: exio_id--IO��չ���; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void EK_SLP_mst_commu_err_handler(U08 exio_id)
{
    U08 i, rsrc_type, num, bytes;
    U16 base_addr;

    if (!GET_EXIO_COMMU_ERR(exio_id))    return;

    /* ��ͷ��ʼ�� */
    ARRCLR(fg_exio_klv_req[exio_id]);
    SET_EXIO_KLV_REQ(exio_id, EK_STAGE_KLV_CORE_SLP);    

    /* ���³�ʼ�� */
    m_exio_hdw_init_sta[exio_id] = EK_HDW_INIT_STA_PLAN;

    /* ��չ�����ݲ����� */
    CLRBIT_N(m_fg_exio_ver_ready, exio_id);
    CLRBIT_N(m_fg_exio_remote_exist, exio_id);
    CLRBIT_N(m_fg_exio_hdw_status_ready, exio_id);
    CLRBIT_N(m_fg_exio_hdw_cfg_update, exio_id);

    /* ����� */
    for (i = 0; i < EK_RSRC_MAX; i++)
    {
        rsrc_type = m_exio_hdw_rsrc_info[exio_id][i].rsrc_type;
        num = m_exio_hdw_rsrc_info[exio_id][i].num;
        bytes = M_IS_BIT_RSRC(exio_id, rsrc_type) ? _DIVUCARRY(num, 16)*2 : num*2;
        base_addr = EK_SLP_mst_get_exio_cell_base_addr(rsrc_type, exio_id, FALSE);
        if (base_addr != 0xffff)
        {
            MEMCLR(&m_exio_cell[exio_id][base_addr], bytes);
        }
    }  
}

/*******************************************************************************
 �� ��: EK_SLP_mst_fill_request_handler()
 �� ��: ����������ݴ���(����)
 �� ��: exio_id--IO��չ���; 
        exio_klv_data--IO��չ��KLV����; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void EK_SLP_mst_fill_request_handler(U08 exio_id, EXIO_KLV_DATA* exio_klv_data)
{
    U08 i;

    /* ͨ�Ź���ʱ���� */
    EK_SLP_mst_commu_err_handler(exio_id);

    /* ������ת����: Ӳ�����ø���ʱ���� */
    if (GETBIT_N(m_fg_exio_hdw_cfg_update, exio_id))
    {
        SET_EXIO_KLV_REQ(exio_id, EK_STAGE_HDW_CFG_DATA);
        CLRBIT_N(m_fg_exio_hdw_cfg_update, exio_id);
    }
    else if (GETBIT_N(m_fg_exio_init_hdw, exio_id))
    {
        SET_EXIO_KLV_REQ(exio_id, EK_STAGE_HDW_INIT_DATA);
        CLRBIT_N(m_fg_exio_init_hdw, exio_id);
    }
    
    /* �����Ͳ���(���׶�˳������) */
    for (i = 0; i < EK_STAGE_MAX; i++)
    {
        if (GET_EXIO_KLV_REQ(exio_id, i))
        {
            exio_klv_data->stage = i;
            CLR_EXIO_KLV_REQ(exio_id, i);
            break;
        }
    }

    /* ����������� */
    switch (exio_klv_data->stage)
    {
        /* IO��չ��KLV����ҵ��汾 */
        case EK_STAGE_KLV_CORE_SLP:
            /* DO NOTHING */
            break;

        /* IO��չ��΢��汾�� */
        case EK_STAGE_APP_VER:
            /* DO NOTHING */
            break;

        /* IO��չ��Ӳ����Դ��Ϣ */
        case EK_STAGE_HDW_RSRC_INFO:
            /* DO NOTHING */
            break;

        /* IO��չ��Ӳ��������Ϣ */
        case EK_STAGE_HDW_CFG_INFO:
            /* DO NOTHING */
            break;

        /* IO��չ��Ӳ����ʼ����Ϣ */
        case EK_STAGE_HDW_INIT_INFO:
            /* DO NOTHING */
            break;

        /* IO��չ��Ӳ���������� */
        case EK_STAGE_HDW_CFG_DATA:
            EK_SLP_mst_fill_exio_hdw_cfg_data(exio_id, &exio_klv_data->buf[0]);
            break;

        /* IO��չ��Ӳ����ʼ������ */
        case EK_STAGE_HDW_INIT_DATA:
            EK_SLP_mst_fill_exio_hdw_init_data(exio_id, &exio_klv_data->buf[0]);
            break;

        /* IO��չ��Զ���������� */
        case EK_STAGE_REMOTE_CFG_DATA:
//            EK_mst_fill_exio_remote_cfg(exio_id, &exio_klv_data->buf[0]);
            break;

        /* IO��չ��Ӳ��״̬/�����д */
        case EK_STAGE_HDW_WR_RD:
            EK_SLP_mst_fill_exio_hdw_output_data(exio_id, &exio_klv_data->buf[0]);
            break;

        /* IO��չ��Զ��״̬/�����д */
        case EK_STAGE_REMOTE_WR_RD:
//            EKC_mst_fill_exio_fwd_data(exio_id, &exio_klv_data->buf[0]);
            break; 
    }

}

/*******************************************************************************
 �� ��: EK_SLP_mst_parse_respond_handler()
 �� ��: ������Ӧ���ݴ���(����)
 �� ��: exio_id--IO��չ���; 
        exio_klv_data--IO��չ��KLV����; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void EK_SLP_mst_parse_respond_handler(U08 exio_id, EXIO_KLV_DATA* exio_klv_data)
{
    U08 i;
    
    /* ����֡���� */
    switch (exio_klv_data->stage)
    {
        /* IO��չ��KLV����ҵ��汾 */
        case EK_STAGE_KLV_CORE_SLP:
            m_exio_klv_ver_info[exio_id] = MAKEWORD(exio_klv_data->buf[0], exio_klv_data->buf[1]);
            if (m_exio_klv_ver_info[exio_id] > EXIO_KLV_SLP_CORE_VER)
            {
                SET_EXIO_KLV_VER_ERR(exio_id);
                SET_EXIO_KLV_REQ(exio_id, EK_STAGE_KLV_CORE_SLP);
            }
            else
            {
                SET_EXIO_KLV_REQ(exio_id, EK_STAGE_APP_VER);
            }
            break;
    
        /* IO��չ��΢��汾�� */
        case EK_STAGE_APP_VER:
            for (i = 0; i < MAX_VER_BYTE; i++)
            {
                m_exio_ver_info[exio_id][i] = exio_klv_data->buf[i];
            }
            SETBIT_N(m_fg_exio_ver_ready, exio_id);
            SET_EXIO_KLV_REQ(exio_id, EK_STAGE_HDW_RSRC_INFO);
            break;

        /* IO��չ��Ӳ����Դ��Ϣ */
        case EK_STAGE_HDW_RSRC_INFO:
            EK_SLP_mst_parse_exio_hdw_rsrc_info(exio_id, &exio_klv_data->buf[0]);
            break;

        /* IO��չ��Ӳ��������Ϣ */
        case EK_STAGE_HDW_CFG_INFO:
            EK_SLP_mst_parse_exio_hdw_cfg_info(exio_id, &exio_klv_data->buf[0]);
            break;

        /* IO��չ��Ӳ����ʼ����Ϣ */
        case EK_STAGE_HDW_INIT_INFO:
            EK_SLP_mst_parse_exio_hdw_init_info(exio_id, &exio_klv_data->buf[0]);
            break;

        /* IO��չ��Ӳ���������� */
        case EK_STAGE_HDW_CFG_DATA:
            SET_EXIO_KLV_REQ(exio_id, EK_STAGE_HDW_INIT_DATA);
            break;

        /* IO��չ��Ӳ����ʼ������ */
        case EK_STAGE_HDW_INIT_DATA:
            if (GETBIT_N(m_fg_exio_remote_exist, exio_id))
            {
                SET_EXIO_KLV_REQ(exio_id, EK_STAGE_REMOTE_CFG_DATA);
            }
            else
            {
                SET_EXIO_KLV_REQ(exio_id, EK_STAGE_HDW_WR_RD);
            }
            break;

        /* IO��չ��Զ���������� */
        case EK_STAGE_REMOTE_CFG_DATA:
//            EK_mst_parse_exio_rds_cfg_confirm(exio_id, &exio_klv_data->buf[0]);
            SET_EXIO_KLV_REQ(exio_id, EK_STAGE_HDW_WR_RD);
            break;

        /* IO��չ��Ӳ��״̬/�����д */
        case EK_STAGE_HDW_WR_RD:
            EK_SLP_mst_parse_exio_hdw_status_info(exio_id, &exio_klv_data->buf[0]);
            break;

        /* IO��չ��Զ��״̬/�����д */
        case EK_STAGE_REMOTE_WR_RD:
//            EK_mst_parse_exio_rds_data(exio_id, &exio_klv_data->buf[0]);
            SET_EXIO_KLV_REQ(exio_id, EK_STAGE_HDW_WR_RD);      /* �л�����Ӳ��������� */
            break;
    }
}
#endif

#ifdef EXIO_KLV_EXTERN      /* �ⲿ�ӿ� */
/*******************************************************************************
 �� ��: EK_SLP_mst_get_exio_fg()
 �� ��: ��ȡIO��չ��״̬��ر�־(����)
 �� ��: exio_id--IO��չ���; 
        fg_type--��־����; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL EK_SLP_mst_get_exio_fg(U08 exio_id, U08 fg_type)
{
    U08 id = (exio_id > EXIO_KLV_MAX) ? 0 : exio_id;
    U16* fg = PNULL;
    
    switch (fg_type)
    {
         case EK_FG_KLV_VER_ERR:        fg = m_fg_exio_klv_ver_err;         break;
         case EK_FG_COMMU_ERR:          fg = m_fg_exio_commu_err;           break;
         case EK_FG_VER_READY:          fg = m_fg_exio_ver_ready;           break;
         case EK_FG_REMOTE_EXIST:       fg = m_fg_exio_remote_exist;        break;
         case EK_FG_HDW_STATUS_READY:   fg = m_fg_exio_hdw_status_ready;    break;
    }

    return (fg == PNULL) ? FALSE : GETBIT_N(fg, id);
}

/*******************************************************************************
 �� ��: EK_SLP_mst_set_exio_fg()
 �� ��: ����IO��չ��״̬��ر�־(����)
 �� ��: exio_id--IO��չ���; 
        fg_type--��־����; 
        is_clr--�Ƿ�����(��֮��λ); 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void EK_SLP_mst_set_exio_fg(U08 exio_id, U08 fg_type, BOOL is_clr)
{
    U16* fg = PNULL;

    if (exio_id > EXIO_KLV_MAX)             return;
    if (fg_type > EK_FG_MAX)                return;
    if (fg_type == EK_FG_VER_READY)         return;  /* IO��չ��汾����Ϣ���ñ�־�����ⲿ��д */
    if (fg_type == EK_FG_HDW_STATUS_READY)  return;  /* IO��չ��Ӳ��״̬��Ϣ���ñ�־�����ⲿ��д */
    
    switch (fg_type)
    {
         case EK_FG_KLV_VER_ERR:        fg = m_fg_exio_klv_ver_err;         break;
         case EK_FG_COMMU_ERR:          fg = m_fg_exio_commu_err;           break;
         case EK_FG_REMOTE_EXIST:       fg = m_fg_exio_remote_exist;        break;
         case EK_FG_UPDATE_HDW_CFG:     fg = m_fg_exio_hdw_cfg_update;      break;
         case EK_FG_INIT_HDW:           fg = m_fg_exio_init_hdw;            break;
    }

    is_clr ? CLRBIT_N(fg, exio_id) : SETBIT_N(fg, exio_id);
}

/*******************************************************************************
 �� ��: EK_SLP_mst_get_exio_ver()
 �� ��: ��ȡIO��չ��汾����Ϣ(����)
 �� ��: exio_id--IO��չ���; 
 �� ��: IO��չ��汾��ָ��
 �� ��: ��
*******************************************************************************/
U08* EK_SLP_mst_get_exio_ver(U08 exio_id)
{
    U08 id = (exio_id > EXIO_KLV_MAX) ? 0 : exio_id;
    return &m_exio_ver_info[id][0];
}

/*******************************************************************************
 �� ��: EK_SLP_mst_get_exio_cell_data()
 �� ��: ����IO��չ��Ӳ����Դ���ͣ���ȡ�洢��m_exio_cell�е�����(����)
 �� ��: rsrc_type--Ӳ����Դ����; 
        exio_id--IO��չ���; 
        idx--ͨ����; 
        is_read--�Ƿ�Ϊ�ɶ�(��֮Ϊ��д); 
 �� ��: λ����/������
 �� ��: ��
*******************************************************************************/
I16 EK_SLP_mst_get_exio_cell_data(U08 rsrc_type, U08 exio_id, U08 idx, U08 is_read)
{
    U16 base_addr, loc;

    /* �������� */
    if (exio_id > EKC_EXIO_KLV_USED || rsrc_type > EK_RSRC_MAX)
    {
        return 0;
    }

    /* ��Ч����ַ */
    if (m_exio_cell_base_addr[exio_id][rsrc_type] == 0xffff)
    {
        return 0;
    }

    /* ����ɶ�/��д��ʼ��ַ */
    base_addr = EK_SLP_mst_get_exio_cell_base_addr(rsrc_type, exio_id, is_read);
    if (base_addr == 0xffff)
    {
        return 0;
    }

    if (M_IS_BIT_RSRC(exio_id, rsrc_type))
    {
        /* ��λ���� */
        loc = base_addr*16 + idx;
        return (_DIVUINT(loc, 16) < M_EXIO_CELL_BUF_SIZE) ? GETBIT_N(m_exio_cell[exio_id], loc) : 0;
    }
    else
    {
        /* ���ִ��� */
        loc = base_addr + idx;
        return (loc < M_EXIO_CELL_BUF_SIZE) ? m_exio_cell[exio_id][loc] : 0;
    }
}

/*******************************************************************************
 �� ��: EK_SLP_mst_set_exio_cell_data()
 �� ��: ����IO��չ��Ӳ����Դ���ͣ�������ֵд��m_exio_cell��(����)
 �� ��: rsrc_type--Ӳ����Դ����; 
        exio_id--IO��չ���; 
        idx--ͨ����; 
        val--����ֵ; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void EK_SLP_mst_set_exio_cell_data(U08 rsrc_type, U08 exio_id, U08 idx, I16 val)
{
    U16 base_addr, loc;

    if (GET_EXIO_COMMU_ERR(exio_id))
    {
        return;
    }
    
    /* �������� */
    if (exio_id > EKC_EXIO_KLV_USED || rsrc_type > EK_RSRC_MAX)
    {
        return;
    }

    /* ��Ч����ַ */
    if (m_exio_cell_base_addr[exio_id][rsrc_type] == 0xffff)
    {
        return;
    }
    
    /* �����д��ʼ��ַ */
    base_addr = EK_SLP_mst_get_exio_cell_base_addr(rsrc_type, exio_id, FALSE);
    if (base_addr == 0xffff)
    {
        return;
    }

    if (M_IS_BIT_RSRC(exio_id, rsrc_type))
    {  
        /* ��λ���� */
        loc = base_addr * 16 + idx;
        if (_DIVUINT(loc, 16) >= M_EXIO_CELL_BUF_SIZE)
        {
            return;
        }
        else
        {
            val ? SETBIT_N(m_exio_cell[exio_id], loc) : CLRBIT_N(m_exio_cell[exio_id], loc);
        }
    }
    else
    {
        /* ���ִ��� */
        loc = base_addr + idx;
        if (loc >= M_EXIO_CELL_BUF_SIZE)
        {
            return;
        }
        else
        {
            m_exio_cell[exio_id][loc] = val;
        }
    }
}
#endif

#else
/*******************************************************************************
  �� �� �� ��
*******************************************************************************/
extern const EXIO_HDW_RSRC_INFO EKC_EXIO_HDW_RSRC_INFO[];
extern const EXIO_HDW_CFG_INFO EKC_EXIO_HDW_CFG_INFO[];
extern const EXIO_HDW_INIT_INFO EKC_EXIO_HDW_INIT_INFO[];

#ifdef EXIO_KLV_STATIC      /* �ڲ��ӿ� */
#define S_EXIO_HDW_CFG_BUF_SIZE                  (50)       /* IO��չ��Ӳ�����û����С */
#define S_EXIO_HDW_INIT_BUF_SIZE                 (50)       /* IO��չ��Ӳ����ʼ�������С */
#define S_EXIO_CELL_BUF_SIZE                     (50)       /* IO��չ��Ӳ��������ݻ����С */
//#define EXIO_RDS_CFG_MAX                         (50)       /* ��δ���� */

/* ͨѶ���ϱ�־ */
static BOOL s_fg_exio_commu_err;

/* IO��չ�����ݻ������ַ */
static U16 s_exio_hdw_cfg_data_addr[EK_CFG_MAX];            /* IO��չ��Ӳ�����û������ַ */
static U16 s_exio_hdw_init_data_addr[EK_INIT_MAX];          /* IO��չ��Ӳ����ʼ���������ַ */
static U16 s_exio_cell_data_addr[EK_RSRC_MAX];              /* IO��չ��Ӳ��������ݻ������ַ */

/* IO��չ�����ݻ��� */
static I16 s_exio_hdw_cfg_data[S_EXIO_HDW_CFG_BUF_SIZE];    /* IO��չ��Ӳ���������ݻ��� */
static I16 s_exio_hdw_init_data[S_EXIO_HDW_INIT_BUF_SIZE];  /* IO��չ��Ӳ����ʼ�����ݻ��� */
static I16 s_exio_cell[S_EXIO_CELL_BUF_SIZE];               /* IO��չ��Ӳ��״̬/�����д���� */

/* IO��չ��Ӳ����ʼ��״ֵ̬ */
static U08 s_exio_hdw_init_sta;                             

//U08 exio_RDS_cfg_frame_cnt;                                 /* ��δ���� */
//U08 exio_RDS_cfg_frame_idx;                                 /* ��δ���� */
//EXIO_REMOTE_CFG exio_RDS_cfg[EXIO_RDS_CFG_MAX];             /* Զ����������(��δ����) */

/* �Ƿ�Ϊλ���� */
#define S_IS_BIT_RSRC(rsrc_type)                  (EK_SLP_slv_get_exio_hdw_rsrc_attr(rsrc_type) & EK_BIT)
/* �Ƿ�Ϊ�ɶ� */
#define S_IS_RD_RSRC(rsrc_type)                   (EK_SLP_slv_get_exio_hdw_rsrc_attr(rsrc_type) & EK_RD)
/* �Ƿ�Ϊ��д */
#define S_IS_WR_RSRC(rsrc_type)                   (EK_SLP_slv_get_exio_hdw_rsrc_attr(rsrc_type) & EK_WR)

/*******************************************************************************
 �� ��: EK_SLP_slv_get_exio_hdw_rsrc_attr()
 �� ��: ��ȡIO��չ��Ӳ����Դ����(�ӻ�)
 �� ��: rsrc_type--IO��չ��Ӳ����Դ����; 
 �� ��: IO��չ��Ӳ����Դ����ֵ
 �� ��: ��
*******************************************************************************/
static U08 EK_SLP_slv_get_exio_hdw_rsrc_attr(U08 rsrc_type)
{
    U08 i;
    U08 attr = 0;

    for (i = 0; i < EKC_get_exio_hdw_rsrc_info_num(); i++)
    {
        if (EKC_EXIO_HDW_RSRC_INFO[i].rsrc_type == rsrc_type)
        {
            attr = EKC_EXIO_HDW_RSRC_INFO[i].attr;
        }
    }

    return attr;
}

/*******************************************************************************
 �� ��: EK_SLP_slv_get_hdw_rsrc_num()
 �� ��: ��ȡIO��չ��Ӳ����Դ��(�ӻ�)
 �� ��: rsrc_type--IO��չ��Ӳ����Դ����; 
 �� ��: IO��չ��Ӳ����Դ��
 �� ��: ��
*******************************************************************************/
static U08 EK_SLP_slv_get_hdw_rsrc_num(U08 rsrc_type)
{
    U08 i;
    U08 num = 0;

    for (i = 0; i < EKC_get_exio_hdw_rsrc_info_num(); i++)
    {
        if (EKC_EXIO_HDW_RSRC_INFO[i].rsrc_type == rsrc_type)
        {
            num = EKC_EXIO_HDW_RSRC_INFO[i].num;
        }
    }

    return num;
}

/*******************************************************************************
 �� ��: EK_SLP_slv_check_exio_hdw_init_done()
 �� ��: ���Ӳ����ʼ���Ƿ����(�ӻ�)
 �� ��: ��
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
static BOOL EK_SLP_slv_check_exio_hdw_init_done(void)
{
    /* ���������� */
    if (!EKC_slv_exio_eev_init_done())
    {
        return FALSE;
    }

    return TRUE;
}

/*******************************************************************************
 �� ��: EK_SLP_slv_get_exio_cell_base_addr()
 �� ��: ��ȡIO��չ��Ӳ��״̬/�����д�������ַ(�ӻ�)
 �� ��: rsrc_type--IO��չ��Ӳ����Դ����; 
 �� ��: IO��չ��Ӳ��״̬/�����д�������ַ
 �� ��: ��
*******************************************************************************/
static U16 EK_SLP_slv_get_exio_cell_base_addr(U08 rsrc_type)
{
    U16 base_addr;

    if (!S_IS_WR_RSRC(rsrc_type))
    {
        return 0xffff;
    }   
    base_addr = s_exio_cell_data_addr[rsrc_type];

    return base_addr;
}

/*******************************************************************************
 �� ��: EK_SLP_slv_cal_exio_hdw_cfg_base_addr()
 �� ��: ����IO��չ��Ӳ���������ݻ������ַ
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void EK_SLP_slv_cal_exio_hdw_cfg_base_addr(void)
{
    U08 i, item_min, cfg_type, rsrc_type;
    U16 base_addr = 0;

    item_min = MIN(EKC_get_exio_hdw_cfg_info_num(), EK_CFG_MAX);

    for (i = 0; i < item_min; i++)
    {
        cfg_type = EKC_EXIO_HDW_CFG_INFO[i].cfg_type;
        rsrc_type = EKC_EXIO_HDW_CFG_INFO[i].rsrc_type;
        s_exio_hdw_cfg_data_addr[cfg_type] = base_addr;
        base_addr += EK_SLP_slv_get_hdw_rsrc_num(rsrc_type);
    }
}

/*******************************************************************************
 �� ��: EK_SLP_slv_cal_exio_hdw_init_base_addr()
 �� ��: ������չ��Ӳ����ʼ���������ַ
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void EK_SLP_slv_cal_exio_hdw_init_base_addr(void)
{
    U08 i, item_min, init_type, rsrc_type;
    U16 base_addr = 0;

    item_min = MIN(EKC_get_exio_hdw_init_info_num(), EK_INIT_MAX);

    for (i = 0; i < item_min; i++)
    {
        init_type = EKC_EXIO_HDW_INIT_INFO[i].init_type;
        rsrc_type = EKC_EXIO_HDW_INIT_INFO[i].rsrc_type;
        s_exio_hdw_init_data_addr[init_type] = base_addr;
        base_addr += EK_SLP_slv_get_hdw_rsrc_num(rsrc_type);
    }
}

/*******************************************************************************
 �� ��: EK_SLP_slv_cal_exio_cell_base_addr()
 �� ��: ������չ���д���ݻ������ַ
 �� ��: ��
 �� ��: ��
 �� ��: ֻΪ����"��д"���Ե���Ŀ���ٿռ�
*******************************************************************************/
static void EK_SLP_slv_cal_exio_cell_base_addr(void)
{
    U08 i, item_min, attr, words;
    U16 base_addr = 0;

    item_min = MIN(EKC_get_exio_hdw_rsrc_info_num(), EK_RSRC_MAX);
    
    for (i = 0; i < item_min; i++)
    {
        attr = EKC_EXIO_HDW_RSRC_INFO[i].attr;
        s_exio_cell_data_addr[EKC_EXIO_HDW_RSRC_INFO[i].rsrc_type] = (attr & EK_WR) ? base_addr : 0xffff;
        words = (attr & EK_BIT) ? _DIVUCARRY(EKC_EXIO_HDW_RSRC_INFO[i].num, 16) : EKC_EXIO_HDW_RSRC_INFO[i].num;
        base_addr += (attr & EK_WR) ? words : 0;             /* �����ٿ�д���ݿռ� */
    }
}

/*******************************************************************************
 �� ��: EK_SLP_slv_rst_exio_cell_data()
 �� ��: ��λָ��Ӳ�����͵���չ���д����
 �� ��: rsrc_type--IO��չ��Ӳ����Դ����; 
        rst_val--��λֵ; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void EK_SLP_slv_rst_exio_cell_data(U08 rsrc_type, I16 rst_val)
{
    U16 base_addr;
    U08 num, words;
    U08 i;

    /* Ӳ����Դ����������ʱ������λ */
    if (rsrc_type > EK_RSRC_MAX)    return;
    
    base_addr = s_exio_cell_data_addr[rsrc_type];
    num = EK_SLP_slv_get_hdw_rsrc_num(rsrc_type);
    words = S_IS_BIT_RSRC(rsrc_type) ? _DIVUCARRY(num, 16) : num;

    /* ����д����ʱ������λ */
    if (base_addr + words > S_EXIO_CELL_BUF_SIZE)    return;

    /* ��д���渴λ */
    for (i = 0; i <words; i++)
    {
        s_exio_cell[base_addr+i] = rst_val;
    }
}

/*******************************************************************************
 �� ��: EK_SLP_slv_parse_exio_hdw_cfg_data()
 �� ��: ����IO��չ��Ӳ����������(�ӻ�)
 �� ��: buf--���ջ���; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void EK_SLP_slv_parse_exio_hdw_cfg_data(U08 *buf)
{
    U08 i;
    U08 cfg_type, words;
    U08 total_bytes;
    I16 cfg_val;
    U16 idx;
    U16 base_addr = 0;
    U08 len = 0;

    /* ��Ч�ֽ��� */
    total_bytes = buf[len++];

    /* �������ݵ�Ԫ */
    while (TRUE)
    {
        if (len > total_bytes \
        || len > EXIO_KLV_DATA_BUF_BYTES)
        {
            break;
        }
        
        cfg_type = buf[len++];  /* Ӳ����Դ��������: 1�ֽ� */
        words = buf[len++];     /* ����: 1�ֽ� */

        /* ����ÿ�����õĻ���ַ */
        base_addr = s_exio_hdw_cfg_data_addr[cfg_type];
       
        /* �������ò���ֵ */
        for (i = 0; i < words; i++)
        {
            cfg_val = MAKEWORD(buf[len+i*2], buf[len+i*2+1]);  /* ����ֵ: 2�ֽ� */
    
            idx = base_addr + i;
            if (idx < S_EXIO_HDW_CFG_BUF_SIZE)
            {
                s_exio_hdw_cfg_data[idx] = cfg_val;
            }
        }
        len += words*2;
        
    }

    /* Ӳ�����ô��� */
    EKC_slv_exio_hdw_cfg_handler();                    
}

/*******************************************************************************
 �� ��: EK_SLP_slv_parse_exio_hdw_init_data()
 �� ��: ����IO��չ��Ӳ����ʼ������(�ӻ�)
 �� ��: buf--���ջ���; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void EK_SLP_slv_parse_exio_hdw_init_data(U08 *buf)
{
    U08 i;
    U08 init_type, words;
    U08 total_bytes;
    I16 init_val;
    U16 base_addr, idx;
    U08 len = 0;

    /* ��Ч�ֽ��� */
    total_bytes = buf[len++];

    /* �������ݵ�Ԫ */
    while (TRUE)
    {
        if (len > total_bytes \
        || len > EXIO_KLV_DATA_BUF_BYTES)
        {
            break;
        }

        /* ����ͷ��Ϣ */
        init_type = buf[len++]; /* Ӳ����Դ��ʼ������: 1�ֽ� */
        words = buf[len++];     /* ����: 1�ֽ� */
    
        /* ��ȡÿ�ֳ�ʼ���Ļ���ַ */
        base_addr = s_exio_hdw_init_data_addr[init_type];
    
        for (i = 0; i < words; i++)
        {
            init_val = MAKEWORD(buf[len+i*2], buf[len+i*2+1]);  /* ����ֵ: 2�ֽ� */
    
            idx = base_addr + i;
            if (idx < S_EXIO_HDW_INIT_BUF_SIZE)
            {
                s_exio_hdw_init_data[idx] = init_val;
            }
        }
        len += words*2;
    }

    /* Ӳ����ʼ������ */    
    EKC_slv_exio_hdw_init_handler();

    /* Ӳ����ʼ��״̬ */
    s_exio_hdw_init_sta = EK_HDW_INIT_STA_DOING;
}

/*******************************************************************************
 �� ��: EK_SLP_slv_parse_exio_hdw_output_data()
 �� ��: ����IO��չ��Ӳ���������(�ӻ�)
 �� ��: buf--���ջ���; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void EK_SLP_slv_parse_exio_hdw_output_data(U08 *buf)
{
    U08 i;
    U08 rsrc_type, words;
    U08 total_bytes;
    I16 val;
    U16 idx;
    U16 base_addr = 0;
    U08 len = 0;

    /* ��Ч�ֽ��� */
    total_bytes = buf[len++];

    /* �������ݵ�Ԫ */
    while (TRUE)
    {
        if (len > total_bytes \
        || len > EXIO_KLV_DATA_BUF_BYTES)
        {
            break;
        }

        /* ����ͷ��Ϣ */
        rsrc_type = buf[len++]; /* Ӳ����Դ����: 1�ֽ� */
        words = buf[len++];     /* ����: 1�ֽ� */
    
        base_addr = s_exio_cell_data_addr[rsrc_type];
    
        /* ����Ӳ�����ֵ */
        for (i = 0; i < words; i++)
        {
            val = MAKEWORD(buf[len+i*2], buf[len+i*2+1]);   /* ���ֵ: 2�ֽ� */
    
            idx = base_addr + i;
            if (idx < S_EXIO_CELL_BUF_SIZE)
            {
                s_exio_cell[idx] = val;
            }    
        }
        len += words*2;
    }
}

/*******************************************************************************
 �� ��: EK_SLP_slv_fill_exio_hdw_rsrc_info()
 �� ��: ���IO��չ��Ӳ����Դ��Ϣ(�ӻ�)
 �� ��: buf--���ͻ���; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void EK_SLP_slv_fill_exio_hdw_rsrc_info(U08* buf)
{
    U08 i;
    U08 len = 1;            /* ��ͷԤ��1�ֽ� */
                
    /* IO��չ��Ӳ����Դ */
    buf[len++] = EKC_get_exio_hdw_rsrc_info_num();
    for (i = 0; i < EKC_get_exio_hdw_rsrc_info_num(); i++)
    {
        MEMCPY(&buf[len], &EKC_EXIO_HDW_RSRC_INFO[i], EXIO_HDW_RSRC_INFO_BYTES);
        len += EXIO_HDW_RSRC_INFO_BYTES;
    }

    buf[0] = len - 1;
}

/*******************************************************************************
 �� ��: EK_SLP_slv_fill_exio_hdw_cfg_info()
 �� ��: ���IO��չ��Ӳ��������Ϣ(�ӻ�)
 �� ��: buf--���ͻ���; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void EK_SLP_slv_fill_exio_hdw_cfg_info(U08* buf)
{
    U08 i;
    U08 len = 1;            /* ��ͷԤ��1�ֽ� */

    /* Ӳ����Դ������ */
    buf[len++] = EKC_get_exio_hdw_cfg_info_num();
    for (i = 0; i < EKC_get_exio_hdw_cfg_info_num(); i++)
    {
        MEMCPY(&buf[len], &EKC_EXIO_HDW_CFG_INFO[i], EXIO_HDW_CFG_BYTES_INFO);
        len += EXIO_HDW_CFG_BYTES_INFO;
    }

    buf[0] = len - 1;
}

/*******************************************************************************
 �� ��: EK_SLP_slv_fill_exio_hdw_init_info()
 �� ��: ���IO��չ��Ӳ����ʼ����Ϣ(�ӻ�)
 �� ��: buf--���ͻ���; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void EK_SLP_slv_fill_exio_hdw_init_info(U08* buf)
{
    U08 i;
    U08 len = 1;            /* ��ͷԤ��1�ֽ� */
                
    /* Ӳ����ʼ���� */
    buf[len++] = EKC_get_exio_hdw_init_info_num();
    for (i = 0; i < EKC_get_exio_hdw_init_info_num(); i++)
    {
        MEMCPY(&buf[len], &EKC_EXIO_HDW_INIT_INFO[i], EXIO_HDW_INIT_BYTES_INFO);
        len += EXIO_HDW_INIT_BYTES_INFO;
    }

    buf[0] = len - 1;
}

/*******************************************************************************
 �� ��: EK_SLP_slv_fill_exio_hdw_status_info()
 �� ��: ���IO��չ��Ӳ��״̬��Ϣ(�ӻ�)
 �� ��: buf--; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void EK_SLP_slv_fill_exio_hdw_status_info(U08* buf)
{
    U08 i, j;
    U08 rsrc_type, rsrc_attr, rsrc_num, words;
    U16 val;
    U08 len = 2;

    if (s_exio_hdw_init_sta == EK_HDW_INIT_STA_DOING \
    && EK_SLP_slv_check_exio_hdw_init_done())
    {
        s_exio_hdw_init_sta = EK_HDW_INIT_STA_DONE;
    }

    for (i = 0; i < EKC_get_exio_hdw_rsrc_info_num(); i++)
    {
        rsrc_type = EKC_EXIO_HDW_RSRC_INFO[i].rsrc_type;
        rsrc_attr = EKC_EXIO_HDW_RSRC_INFO[i].attr;
        rsrc_num = EKC_EXIO_HDW_RSRC_INFO[i].num;
        
        if (rsrc_attr & EK_RD)
        {
            buf[len++] = rsrc_type;

            /* �������� */
            words = (rsrc_attr & EK_BIT) ? _DIVUCARRY(rsrc_num, 16) : rsrc_num;

            for (j = 0; j < words; j++)
            {
                switch (rsrc_type)
                {
                    case EK_RSRC_DI:   val = EKC_SLV_EXIO_DI(j);      break;/* ������ */
                    case EK_RSRC_DO:   val = EKC_SLV_EXIO_DO(j);      break;/* �̵��� */
                    case EK_RSRC_SW:   val = EKC_SLV_EXIO_SW(j);      break;/* ���� */                    
                    case EK_RSRC_TMP:  val = EKC_SLV_EXIO_TMP(j);     break;/* �¶� */
                    case EK_RSRC_PRS:  val = EKC_SLV_EXIO_PRS(j);     break;/* ѹ�� */
                    case EK_RSRC_EEV:  val = EKC_SLV_EXIO_ST_PV(j);   break;/* ���򿪶� */               
                    case EK_RSRC_FAN:  val = EKC_SLV_EXIO_FAN_PV(j);  break;/* ����PWM��� */               
                    case EK_RSRC_PH:   val = EKC_SLV_EXIO_PH(j);      break;/* ���� */
                }        
                buf[len+j*2] = HIBYTE(val);
                buf[len+j*2+1] = LOBYTE(val);
            }            
            len += words*2;
        }
    }  
    buf[0] = s_exio_hdw_init_sta;
    buf[1] = len - 2;
}

#if 0
/*******************************************************************************
 �� ��: EK_slv_parse_exio_rds_cfg()
 �� ��: 
 �� ��: buf--; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void EK_slv_parse_exio_rds_cfg(U08* buf)
{
    U08 i;
    U08 cellWords, cfgTotal, cfgBgn, dataBytes, cfgIdx;
    U16* destBuf = PNULL;
    U08 len = 0;

    exio_RDS_cfg_frame_cnt = buf[len++];    /* ��֡�� */
    exio_RDS_cfg_frame_idx = buf[len++];    /* ��ǰ֡�� */
    cellWords = buf[len++];                 /* ���ݵ�Ԫ���� */
    cfgTotal = buf[len++];                  /* ���������� */
    cfgBgn = buf[len++];                    /* ��ʼ��������� */
    dataBytes = buf[len++];                 /* �������ֽ��� */

    if (cfgBgn >= cfgTotal                  /* �������������� */
    || cfgTotal >= EXIO_RDS_CFG_MAX)        /* �ӻ�֧�������� */
    {
        return;
    }

    cfgIdx = cfgBgn;
    
    while (TRUE)
    {
        if (len >= dataBytes+6 \
        || len >= EXIO_KLV_DATA_BUF_BYTES
        || cfgIdx >= cfgTotal)
        {
            break;
        }

        destBuf = (U16*)&exio_RDS_cfg[cfgIdx];
        for (i = 0; i < cellWords; i++)
        {
            destBuf[i] = MAKEWORD(buf[len], buf[len+1]);
            len += 2;
        }

        cfgIdx++;
    }
}

void EK_slv_parse_exio_rds_data(U08* buf)
{

}

void EK_slv_fill_exio_rds_cfg_confirm(U08* buf)
{
    U08 len = 0;

    buf[len++] = exio_RDS_cfg_frame_cnt;
    buf[len++] = exio_RDS_cfg_frame_idx;
    
}
#endif

/*******************************************************************************
 �� ��: EK_SLP_slv_init()
 �� ��: IO��չ���ϵ��ʼ������(�ӻ�)
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void EK_SLP_slv_init(void)
{
    /* ����Ӳ�����û������ַ */
    EK_SLP_slv_cal_exio_hdw_cfg_base_addr();

    /* ����Ӳ����ʼ���������ַ */
    EK_SLP_slv_cal_exio_hdw_init_base_addr();

    /* �������ݶ�д�������ַ */
    EK_SLP_slv_cal_exio_cell_base_addr();

    /* Ӳ����ʼ��״̬��ʼ�� */
    s_exio_hdw_init_sta = EK_HDW_INIT_STA_PLAN;

    /* ��չ��ͨѶ���ϱ�־��ʼ�� */
    s_fg_exio_commu_err = FALSE;

//    /* ����������ʼ��(��δ����) */    
//    exio_RDS_cfg_frame_cnt = 0;
//    exio_RDS_cfg_frame_idx = 0;
}

/*******************************************************************************
 �� ��: EK_SLP_slv_timer()
 �� ��: IO��չ���ʱ��(�ӻ�)
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void EK_SLP_slv_timer(void)
{
    ;
}

/*******************************************************************************
 �� ��: EK_SLP_slv_commu_err_handler()
 �� ��: IO��չ��ͨѶ���ϴ���(�ӻ�)
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void EK_SLP_slv_commu_err_handler(void)
{
    if (GET_EXIO_COMMU_ERR)
    {
        /* ���³�ʼ�� */
        s_exio_hdw_init_sta = EK_HDW_INIT_STA_PLAN;
    
        /* ����� */
        EK_SLP_slv_rst_exio_cell_data(EK_RSRC_DO, 0);
        
        /* �ص��� */
        EK_SLP_slv_rst_exio_cell_data(EK_RSRC_EEV, 0);
    
        /* �ط�� */
        EK_SLP_slv_rst_exio_cell_data(EK_RSRC_FAN, 0);
    }
}

/*******************************************************************************
 �� ��: EK_SLP_slv_parse_request_handler()
 �� ��: �����������ݴ���(�ӻ�)
 �� ��: exio_klv_data--IO��չ��KLV����; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void EK_SLP_slv_parse_request_handler(EXIO_KLV_DATA* exio_klv_data)
{
    switch (exio_klv_data->stage)
    {
        /* IO��չ��KLV����ҵ��汾 */
        case EK_STAGE_KLV_CORE_SLP:
            /* DO NOTHING */
            break;

        /* IO��չ��΢��汾�� */
        case EK_STAGE_APP_VER:
            /* DO NOTHING */
            break;

        /* IO��չ��Ӳ����Դ��Ϣ */
        case EK_STAGE_HDW_RSRC_INFO:
            /* DO NOTHING */
            break;

        /* IO��չ��Ӳ��������Ϣ */
        case EK_STAGE_HDW_CFG_INFO:
            /* DO NOTHING */
            break;

        /* IO��չ��Ӳ����ʼ����Ϣ */
        case EK_STAGE_HDW_INIT_INFO:
            /* DO NOTHING */
            break;

         /* IO��չ��Ӳ���������� */
        case EK_STAGE_HDW_CFG_DATA:
            EK_SLP_slv_parse_exio_hdw_cfg_data(&exio_klv_data->buf[0]);
            break;

        /* IO��չ��Ӳ����ʼ������ */
        case EK_STAGE_HDW_INIT_DATA:
            EK_SLP_slv_parse_exio_hdw_init_data(&exio_klv_data->buf[0]);
            break;           

        /* IO��չ��Զ���������� */
        case EK_STAGE_REMOTE_CFG_DATA:
//            EK_slv_parse_exio_rds_cfg(&exio_klv_data->buf[0]);
            break;

        /* IO��չ��Ӳ��״̬/�����д */
        case EK_STAGE_HDW_WR_RD:
            EK_SLP_slv_parse_exio_hdw_output_data(&exio_klv_data->buf[0]);
            break;
            
        /* IO��չ��Զ��״̬/�����д */
        case EK_STAGE_REMOTE_WR_RD:
//            EK_slv_parse_exio_rds_data(&exio_klv_data->buf[0]);
            break;
    }
}

/*******************************************************************************
 �� ��: EK_SLP_slv_fill_respond_handler()
 �� ��: �����Ӧ���ݴ���(�ӻ�)
 �� ��: exio_klv_data--IO��չ��KLV����; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void EK_SLP_slv_fill_respond_handler(EXIO_KLV_DATA* exio_klv_data)
{
    switch (exio_klv_data->stage)
    {
        /* IO��չ��KLV����ҵ��汾 */
        case EK_STAGE_KLV_CORE_SLP:
            exio_klv_data->buf[0] = HIBYTE(EXIO_KLV_SLP_CORE_VER);
            exio_klv_data->buf[1] = LOBYTE(EXIO_KLV_SLP_CORE_VER);
            break;

        /* IO��չ��΢��汾�� */
        case EK_STAGE_APP_VER:
            MEMCPY(&exio_klv_data->buf[0], \
                   (U08*)EKC_EXIO_VER, \
                   (sizeof(EKC_EXIO_VER)/sizeof(U08)));
            break;

        /* IO��չ��Ӳ����Դ��Ϣ */
        case EK_STAGE_HDW_RSRC_INFO:
            EK_SLP_slv_fill_exio_hdw_rsrc_info(&exio_klv_data->buf[0]);
            break;

        /* IO��չ��Ӳ��������Ϣ */
        case EK_STAGE_HDW_CFG_INFO:
            EK_SLP_slv_fill_exio_hdw_cfg_info(&exio_klv_data->buf[0]);
            break;

        /* IO��չ��Ӳ����ʼ����Ϣ */
        case EK_STAGE_HDW_INIT_INFO:
            EK_SLP_slv_fill_exio_hdw_init_info(&exio_klv_data->buf[0]);
            break;

        /* IO��չ��Ӳ���������� */
        case EK_STAGE_HDW_CFG_DATA:
            /* DO NOTHING */
            break;

        /* IO��չ��Ӳ����ʼ������ */
        case EK_STAGE_HDW_INIT_DATA:
            /* DO NOTHING */
            break;

        /* IO��չ��Զ���������� */
        case EK_STAGE_REMOTE_CFG_DATA:
//            EK_slv_fill_exio_rds_cfg_confirm(&exio_klv_data->buf[0]);
            break;

        /* IO��չ��Ӳ��״̬/�����д */
        case EK_STAGE_HDW_WR_RD:
            EK_SLP_slv_fill_exio_hdw_status_info(&exio_klv_data->buf[0]);
            break;

        /* IO��չ��Զ��״̬/�����д */
        case EK_STAGE_REMOTE_WR_RD:
            /* DO NOTHING */
            break;
    }
}
#endif

#ifdef EXIO_KLV_EXTERN      /* �ⲿ�ӿ� */
/*******************************************************************************
 �� ��: EK_SLP_slv_get_exio_commu_err()
 �� ��: ��ȡIO��չ��ͨ�Ź���(�ӻ�)
 �� ��: ��
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL EK_SLP_slv_get_exio_commu_err(void)
{
    return s_fg_exio_commu_err;
}

/*******************************************************************************
 �� ��: EK_SLP_slv_set_exio_fg()
 �� ��: ��ȡIO��չ��״̬��ر�־(�ӻ�)
 �� ��: exio_id--IO��չ���; 
        fg_type--��־����; 
        is_clr--�Ƿ����(��֮��λ); 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void EK_SLP_slv_set_exio_commu_err(BOOL is_clr)
{
    s_fg_exio_commu_err = is_clr ? FALSE : TRUE;
}

/*******************************************************************************
 �� ��: EK_SLP_slv_get_exio_hdw_cfg_data()
 �� ��: ��ȡIO��չ��Ӳ����������(�ӻ�)
 �� ��: cfg_type--IO��չ��Ӳ����������; 
        idx--ͨ����; 
 �� ��: IO��չ��Ӳ����������
 �� ��: ��
*******************************************************************************/
I16 EK_SLP_slv_get_exio_hdw_cfg_data(U08 cfg_type, U08 idx)
{
    U16 loc;

    /* �������� */
    if (cfg_type > EK_CFG_MAX)
    {
        return 0;
    }

    loc = s_exio_hdw_cfg_data_addr[cfg_type] + idx;

    if (loc > S_EXIO_HDW_CFG_BUF_SIZE)
    {
        return 0;
    }
    
    return s_exio_hdw_cfg_data[loc];
}

/*******************************************************************************
 �� ��: EK_SLP_slv_get_exio_hdw_init_data()
 �� ��: ��ȡIO��չ��Ӳ����ʼ������(�ӻ�)
 �� ��: init_type--IO��չ��Ӳ����������;
        idx--ͨ����; 
 �� ��: IO��չ��Ӳ����ʼ������
 �� ��: ��
*******************************************************************************/
I16 EK_SLP_slv_get_exio_hdw_init_data(U08 init_type, U08 idx)
{
    U16 loc;

    /* �������� */
    if (init_type > EK_INIT_MAX)
    {
        return 0;
    }

    loc = s_exio_hdw_init_data_addr[init_type] + idx;

    if (loc > S_EXIO_HDW_INIT_BUF_SIZE)
    {
        return 0;
    }
    
    return s_exio_hdw_init_data[loc];
}

/*******************************************************************************
 �� ��: EK_SLP_slv_get_exio_cell_data()
 �� ��: ����IO��չ��Ӳ����Դ���ͣ���ȡ�洢��m_exio_cell�е�����(�ӻ�)
 �� ��: rsrc_type--IO��չ����Դ����; 
        idx--ͨ����; 
 �� ��: λ����/������
 �� ��: ��
*******************************************************************************/
I16 EK_SLP_slv_get_exio_cell_data(U08 rsrc_type, U08 idx)
{
    U16 base_addr, loc;

    /* �������� */
    if (rsrc_type > EK_RSRC_MAX)
    {
        return 0;
    }

    /* ��Ч����ַ */
    if (s_exio_cell_data_addr[rsrc_type] == 0xffff)
    {
        return 0;
    }

    /* �����д��ʼ��ַ */
    base_addr = EK_SLP_slv_get_exio_cell_base_addr(rsrc_type);
    if (base_addr == 0xffff)
    {
        return 0;
    }
    
    if (S_IS_BIT_RSRC(rsrc_type))
    {
        /* ��λ���� */
        loc = base_addr * 16 + idx;
        return (_DIVUINT(loc, 16) < S_EXIO_CELL_BUF_SIZE) ? GETBIT_N(s_exio_cell, loc) : 0;
    }
    else
    {
        /* ���ִ��� */
        loc = base_addr + idx;
        return (loc < S_EXIO_CELL_BUF_SIZE) ? s_exio_cell[loc] : 0;
    }
}

/*******************************************************************************
 �� ��: EK_SLP_slv_set_exio_cell_data()
 �� ��: ����IO��չ��Ӳ����Դ���ͣ�������ֵд��m_exio_cell��(�ӻ�)
 �� ��: rsrc_type--IO��չ����Դ����; 
        idx--ͨ����; 
        val--�趨ֵ; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void EK_SLP_slv_set_exio_cell_data(U08 rsrc_type, U08 idx, I16 val)
{
    U16 base_addr, loc;

    /* �������� */
    if (rsrc_type > EK_RSRC_MAX)
    {
        return;
    }

    /* ��Ч����ַ */
    if (s_exio_cell_data_addr[rsrc_type] == 0xffff)
    {
        return;
    }

    /* �����д��ʼ��ַ */
    base_addr = EK_SLP_slv_get_exio_cell_base_addr(rsrc_type);
    if (base_addr == 0xffff)
    {
        return;
    }

    if (S_IS_BIT_RSRC(rsrc_type))
    {
        /* ��λ���� */
        loc = base_addr * 16 + idx;
        if (_DIVUINT(loc, 16) >= S_EXIO_CELL_BUF_SIZE)
        {
            return;
        }
        val ? SETBIT_N(s_exio_cell, loc) : CLRBIT_N(s_exio_cell, loc);
    }
    else
    {
        /* ���ִ��� */
        loc = base_addr + idx;
        if (loc >= S_EXIO_CELL_BUF_SIZE)
        {
            return;
        }
        s_exio_cell[loc] = val;
    }
}
#endif

#endif

