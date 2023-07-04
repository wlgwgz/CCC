/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: exio_klv_nt_dlg_slp_0.c
 文件描述: IO扩展功能-暖通多联供业务逻辑源文件
 创建人  : J.H.
 创建日期: 2022.01.14
*******************************************************************************/
#define EK_SLP_NT_DLG_GLOBAL

#define EXIO_KLV_STATIC     /* 内部接口部分 */
#define EXIO_KLV_EXTERN     /* 外部接口部分 */

/* EXIO_KLV业务核心版本 */
#define EXIO_KLV_SLP_CORE_VER                     (0)

#ifdef EXIO_KLV_MASTER
/*******************************************************************************
  主 机 部 分
*******************************************************************************/
#ifdef EXIO_KLV_STATIC      /* 内部接口 */

/* IO扩展板硬件状态信息缓存大小 */
#define M_EXIO_CELL_BUF_SIZE                      (100) 

/* IO扩展板状态相关标志 */
static U16 m_fg_exio_klv_ver_err[_DIVUCARRY(EXIO_KLV_MAX, 16)];     /* IO扩展板协议版本不匹配故障标志 */
static U16 m_fg_exio_commu_err[_DIVUCARRY(EXIO_KLV_MAX, 16)];       /* IO扩展板通信故障标志 */
static U16 m_fg_exio_ver_ready[_DIVUCARRY(EXIO_KLV_MAX, 16)];       /* IO扩展板版本号信息可用标志 */
static U16 m_fg_exio_remote_exist[_DIVUCARRY(EXIO_KLV_MAX,16)];     /* IO扩展板存在远端的标志 */
static U16 m_fg_exio_hdw_status_ready[_DIVUCARRY(EXIO_KLV_MAX, 16)];/* IO扩展板硬件状态信息可用标志 */
static U16 m_fg_exio_hdw_cfg_update[_DIVUCARRY(EXIO_KLV_MAX, 16)];  /* IO扩展板硬件配置更新标志 */
static U16 m_fg_exio_init_hdw[_DIVUCARRY(EXIO_KLV_MAX, 16)];        /* IO扩展板硬件初始化标志 */

/* IO扩展板硬件初始化状态 */
static U08 m_exio_hdw_init_sta[EXIO_KLV_MAX];

/* IO扩展板硬件状态/输出读写缓存基地址 */
static U16 m_exio_cell_base_addr[EXIO_KLV_MAX][EK_RSRC_MAX];

/* IO扩展板信息缓存 */
static U16 m_exio_klv_ver_info[EXIO_KLV_MAX];                               /* IO扩展板协议版本信息缓存 */
static U08 m_exio_ver_info[EXIO_KLV_MAX][MAX_VER];                          /* IO扩展板版本号信息缓存 */
static EXIO_HDW_RSRC_INFO m_exio_hdw_rsrc_info[EXIO_KLV_MAX][EK_RSRC_MAX];  /* IO扩展板硬件资源信息缓存 */
static EXIO_HDW_CFG_INFO m_exio_hdw_cfg_info[EXIO_KLV_MAX][EK_CFG_MAX];     /* IO扩展板硬件配置信息缓存 */
static EXIO_HDW_INIT_INFO m_exio_hdw_init_info[EXIO_KLV_MAX][EK_INIT_MAX];  /* IO扩展板硬件初始化信息缓存 */
static I16 m_exio_cell[EXIO_KLV_MAX][M_EXIO_CELL_BUF_SIZE];                 /* IO扩展板硬件状态/输出读写缓存 */

/* IO扩展板硬件相关实际数量 */
static U08 m_exio_hdw_rsrc_info_used[EXIO_KLV_MAX];         /* IO扩展板硬件资源信息实际数量 */
static U08 m_exio_hdw_cfg_info_used[EXIO_KLV_MAX];          /* IO扩展板硬件配置信息实际数量 */
static U08 m_exio_hdw_init_info_used[EXIO_KLV_MAX];         /* IO扩展板硬件初始化信息实际数量 */

/* KLV请求发送标志 */
static U16 fg_exio_klv_req[EXIO_KLV_MAX][_DIVUCARRY(EK_STAGE_MAX,16)];  
#define GET_EXIO_KLV_REQ(exio_id, req_type)       (GETBIT_N(fg_exio_klv_req[exio_id],req_type)) /* 读取 */
#define SET_EXIO_KLV_REQ(exio_id, req_type)       (SETBIT_N(fg_exio_klv_req[exio_id],req_type)) /* 置位 */
#define CLR_EXIO_KLV_REQ(exio_id, req_type)       (CLRBIT_N(fg_exio_klv_req[exio_id],req_type)) /* 清零 */

/* 是否为位变量 */
#define M_IS_BIT_RSRC(exio_id, rsrc_type)         (EK_SLP_mst_get_hdw_rsrc_attr(exio_id,rsrc_type)&EK_BIT)
/* 是否为可读 */
#define M_IS_RD_RSRC(exio_id, rsrc_type)          (EK_SLP_mst_get_hdw_rsrc_attr(exio_id,rsrc_type)&EK_RD)
/* 是否为可写 */
#define M_IS_WR_RSRC(exio_id, rsrc_type)          (EK_SLP_mst_get_hdw_rsrc_attr(exio_id,rsrc_type)&EK_WR)


/*******************************************************************************
 函 数: EK_SLP_mst_get_hdw_rsrc_attr()
 功 能: 获取IO扩展板硬件资源属性(主机)
 参 数: exio_id--IO扩展板号; 
        rsrc_type--硬件资源类型; 
 返 回: IO扩展板硬件资源属性值
 其 它: 空
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
 函 数: EK_SLP_mst_get_exio_cell_base_addr()
 功 能: 获取IO扩展板硬件状态/输出读写缓存基地址(主机)
 参 数: rsrc_type--硬件资源类型; 
        exio_id--IO扩展板号; 
        is_read--是否为可读(反之为可写); 
 返 回: IO扩展板硬件状态/输出读写缓存基地址
 其 它: 空
*******************************************************************************/
static U16 EK_SLP_mst_get_exio_cell_base_addr(U08 rsrc_type, U08 exio_id, U08 is_read)
{
    U08 attr, num, words;
    U16 base_addr;
    
    attr = m_exio_hdw_rsrc_info[exio_id][rsrc_type].attr;
    num = m_exio_hdw_rsrc_info[exio_id][rsrc_type].num;
    words = (attr & EK_BIT) ? _DIVUCARRY(num, 16) : num;

    if (is_read)             /* 可读 */
    {
        if (!M_IS_RD_RSRC(exio_id, rsrc_type))
        {
            return 0xffff;
        }
        base_addr = (attr & EK_RD) ? m_exio_cell_base_addr[exio_id][rsrc_type] : 0xffff;
    }
    else                    /* 可写 */
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
 函 数: EK_SLP_mst_fill_exio_hdw_cfg_data()
 功 能: 填充IO扩展板硬件配置数据(主机)
 参 数: exio_id--IO扩展板号; 
        buf--发送缓存; 
 返 回: 空
 其 它: 空
*******************************************************************************/
static void EK_SLP_mst_fill_exio_hdw_cfg_data(U08 exio_id, U08* buf)
{
    U08 i, rsrc_id;
    U08 rsrc_attr, rsrc_num;
    U08 cfg_type, rsrc_type;
    I16 cfg_val;
    U08 len = 1;

    /* 遍历所有配置 */
    for (i = 0; i < m_exio_hdw_cfg_info_used[exio_id]; i++)
    {
        /* 根据已获取的IO扩展板信息 */
        cfg_type = m_exio_hdw_cfg_info[exio_id][i].cfg_type;
        rsrc_type = m_exio_hdw_cfg_info[exio_id][i].rsrc_type;
        rsrc_attr = m_exio_hdw_rsrc_info[exio_id][rsrc_type].attr;
        rsrc_num = m_exio_hdw_rsrc_info[exio_id][rsrc_type].num;         /* 当前硬件个数 */

        /* 头信息 */
        buf[len++] = cfg_type;                                                    /* 硬件资源配置类型: 1字节 */
        buf[len++] = (rsrc_attr & EK_BIT) ? _DIVUCARRY(rsrc_num, 16) : rsrc_num;  /* 字数: 1字节 */

        /* 填充配置数据 */
        for (rsrc_id = 0; rsrc_id < rsrc_num; rsrc_id++)
        {
            switch (rsrc_type)
            {
                /* 温度探头 */
                case EK_RSRC_TMP:  cfg_val = EKC_mst_get_exio_tmp_cfg(cfg_type, rsrc_id);  break;
        
                /* 电子膨胀阀 */
                case EK_RSRC_EEV:  cfg_val = EKC_mst_get_exio_eev_cfg(cfg_type, rsrc_id);  break;
            }
            buf[len++] = HIBYTE(cfg_val);
            buf[len++] = LOBYTE(cfg_val);
        }
    }

    buf[0] = len - 1;
}


/*******************************************************************************
 函 数: EK_SLP_mst_fill_exio_hdw_init_data()
 功 能: 填充IO扩展板硬件初始化数据(主机)
 参 数: exio_id--IO扩展板号; 
        buf--发送缓存; 
 返 回: 空
 其 它: 空
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

        /* 头信息 */
        buf[len++] = init_type;
        buf[len++] = (rsrc_attr & EK_BIT) ? _DIVUCARRY(rsrc_num, 16) : rsrc_num;

        /* 填充初始化数据 */
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
 函 数: EK_SLP_mst_fill_exio_hdw_output_data()
 功 能: 填充IO扩展板硬件输出数据(主机)
 参 数: exio_id--IO扩展板号; 
        buf--发送缓存; 
 返 回: 空
 其 它: 空
*******************************************************************************/
static void EK_SLP_mst_fill_exio_hdw_output_data(U08 exio_id, U08* buf)
{
    U08 i, j;
    U08 rsrc_id;
    U08 rsrc_type, rsrc_attr, rsrc_num;
    U08 words;
    I16 val;
    U08 len = 1;

    /* 初始化未完成，暂不输出 */
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
            buf[len++] = rsrc_type;         /* 硬件资源类型: 1字节 */
            buf[len++] = words;             /* 字数: 1字节 */

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
 函 数: EK_SLP_mst_cal_exio_cell_base_addr()
 功 能: 计算IO扩展板硬件状态/输出读写缓存基地址(主机)
 参 数: exio_id--IO扩展板号; 
 返 回: 空
 其 它: 空
*******************************************************************************/
static void EK_SLP_mst_cal_exio_cell_base_addr(U08 exio_id)
{
    U08 i, attr, rsrc_type, num, words;
    U16 baseAddr = 0;

    /* 计算数据基地址 */
    for (i = 0; i < m_exio_hdw_rsrc_info_used[exio_id]; i++)
    {
        attr = m_exio_hdw_rsrc_info[exio_id][i].attr;
        rsrc_type = m_exio_hdw_rsrc_info[exio_id][i].rsrc_type;
        num = m_exio_hdw_rsrc_info[exio_id][i].num;
        m_exio_cell_base_addr[exio_id][rsrc_type] = (attr & EK_RD || attr & EK_WR) ? baseAddr : 0xffff;
        words = (attr & EK_BIT) ? _DIVUCARRY(num, 16) : num;
        baseAddr += (attr & EK_RD) ? words : 0;             /* 开辟可读数据空间 */
        baseAddr += (attr & EK_WR) ? words : 0;             /* 开辟可写数据空间 */
    }
}

/*******************************************************************************
 函 数: EK_SLP_mst_parse_exio_hdw_rsrc_info()
 功 能: 解析IO扩展板硬件资源信息(主机)
 参 数: exio_id--IO扩展板号; 
        buf--接收缓存; 
 返 回: 空
 其 它: 空
*******************************************************************************/
static void EK_SLP_mst_parse_exio_hdw_rsrc_info(U08 exio_id, U08* buf)
{
    U08 total_bytes, itemNum;
    U08 rsrc_id = 0;
    U08 len = 0;

    /* 有效字节数 */
    total_bytes = buf[len++];
    itemNum = buf[len++];
    m_exio_hdw_rsrc_info_used[exio_id] = MIN(itemNum, EK_RSRC_MAX);
    
    /* 逐字节接收处理 */
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

        /* 检测是否存在远端(暂未使用) */
//        if (m_exio_hdw_rsrc_info[exio_id][rsrc_id].attr & EK_REMOTE)
//        {
//            SETBIT_N(fg_exio_remote, exio_id);
//        }

        rsrc_id++;
    }

    /* 计算IO扩展板硬件状态信息缓存基地址 */
    EK_SLP_mst_cal_exio_cell_base_addr(exio_id);

    /* 请求IO扩展板硬件配置信息 */
    SET_EXIO_KLV_REQ(exio_id, EK_STAGE_HDW_CFG_INFO);
}

/*******************************************************************************
 函 数: EK_SLP_mst_parse_exio_hdw_cfg_info()
 功 能: 解析IO扩展板硬件配置信息(主机)
 参 数: exio_id--IO扩展板号; 
        buf--接收缓存; 
 返 回: 空
 其 它: 空
*******************************************************************************/
static void EK_SLP_mst_parse_exio_hdw_cfg_info(U08 exio_id, U08* buf)
{
    U08 total_bytes, itemNum;
    U08 cfg_id = 0;
    U08 len = 0;

    /* 有效字节数 */
    total_bytes = buf[len++];
    itemNum = buf[len++];
    m_exio_hdw_cfg_info_used[exio_id] = MIN(itemNum, EK_CFG_MAX);
    
    /* 逐字节接收处理 */
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

    /* 请求IO扩展板硬件初始化信息 */
    SET_EXIO_KLV_REQ(exio_id, EK_STAGE_HDW_INIT_INFO);
}

/*******************************************************************************
 函 数: EK_SLP_mst_parse_exio_hdw_init_info()
 功 能: 解析IO扩展板硬件初始化信息(主机)
 参 数: exio_id--IO扩展板号; 
        buf--接收缓存; 
 返 回: 空
 其 它: 空
*******************************************************************************/
static void EK_SLP_mst_parse_exio_hdw_init_info(U08 exio_id, U08* buf)
{
    U08 total_bytes, itemNum;
    U08 init_id = 0;
    U08 len = 0;

    /* 有效字节数 */
    total_bytes = buf[len++];
    itemNum = buf[len++];
    m_exio_hdw_init_info_used[exio_id] = MIN(itemNum, EK_INIT_MAX);
    
    /* 逐字节接收处理 */
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

    /* 发送IO扩展板硬件配置数据 */
    SET_EXIO_KLV_REQ(exio_id, EK_STAGE_HDW_CFG_DATA); 
}

/*******************************************************************************
 函 数: EK_SLP_mst_parse_exio_hdw_status_info()
 功 能: 解析IO扩展板硬件状态信息(主机)
 参 数: exio_id--IO扩展板号; 
        buf--接收缓存; 
 返 回: 空
 其 它: 空
*******************************************************************************/
static void EK_SLP_mst_parse_exio_hdw_status_info(U08 exio_id, U08* buf)
{
    U08 i;
    U08 total_bytes, rsrc_type, rsrc_num, rsrc_attr; 
    U08 words;
    U16 base_addr, idx;
    U08 len = 0;

    /* 初始化状态 */
    m_exio_hdw_init_sta[exio_id] = buf[len++];

    /* 有效字节数 */
    total_bytes = buf[len++];

    /* 逐字节接收处理 */
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

    /* IO扩展板状态数据可用 */
    SETBIT_N(m_fg_exio_hdw_status_ready, exio_id);

    if (GETBIT_N(m_fg_exio_remote_exist, exio_id))
    {
        SET_EXIO_KLV_REQ(exio_id, EK_STAGE_REMOTE_WR_RD);   /* 切换发送远端数据 */
    }
    else
    {
        SET_EXIO_KLV_REQ(exio_id, EK_STAGE_HDW_WR_RD);      /* 保持发送硬件输出数据 */
    }   
}

#if 0
/*******************************************************************************
 函 数: EK_mst_fill_exio_remote_cfg()
 功 能: 
 参 数: exio_id--; 
        buf--; 
 返 回: 空
 其 它: 空
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
   
    /* 计算一帧中数据单元的最大个数 */
    itemMax = _DIVUINT((EXIO_KLV_DATA_BUF_BYTES-5), EXIO_REMOTE_CFG_BYTES);

    /* 计算配置项总数 */
    for (i = 0; i < EKC_get_remote_type_used_num(); i++)
    {
        cfgTotal += EKC_get_remote_info_listLen(i);
    }

    /* 填充头信息 */
    buf[len++] = _DIVUCARRY((cfgTotal*EXIO_REMOTE_CFG_BYTES), (EXIO_KLV_DATA_BUF_BYTES-5));   /* 总帧数 */
    buf[len++] = frameCur++;                                                            /* 当前帧号 */
    buf[len++] = EXIO_REMOTE_CFG_WORDS;                                                 /* 数据单元字数 */    
    buf[len++] = cfgTotal;                                                              /* 配置项总数 */
    len++;                                                                              /* 总字节数 */

    while (TRUE)
    {   
        /* 遍历完所有远端类型，跳出 */
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
            /* 若超出一帧，则记录中断序号 */
            if (itemCnt++ >= itemMax)
            {
                cfgRec = i;
                isFrameFull = TRUE;
                break;
            }
        
            /* 初始化 */
            MEMCLR(&remoteCfg, EXIO_REMOTE_CFG_BYTES);

            dataAddr = EKC_get_remote_cfg_dataAddr(cfgList[i]);

            /* 更新数据单元 */
            remoteCfg.baseAddr = baseAddr;
            remoteCfg.deviceMax = deviceMax;
            remoteCfg.funcCode = EKC_get_remote_cfg_funcCode(cfgList[i]);
            remoteCfg.commuIntv = EKC_get_remote_cfg_commuIntv(cfgList[i]);
            remoteCfg.prior = EKC_get_remote_cfg_prior(cfgList[i]);
            remoteCfg.dataAddrH = HIBYTE(dataAddr);
            remoteCfg.dataAddrL = LOBYTE(dataAddr);
            remoteCfg.dataNum = EKC_get_remote_cfg_dataNum(cfgList[i]);

            /* 填充 */
            MEMCPY(&buf[len], &remoteCfg, EXIO_REMOTE_CFG_BYTES);
            len += EXIO_REMOTE_CFG_BYTES;
        }

        /* 若当前帧未满，则继续发下一个类型 */
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
    
    frameCnt = buf[len++];                   /* 总帧数 */
    frameIdx = buf[len++];                   /* 当前帧号 */

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
 函 数: EK_SLP_mst_init()
 功 能: 使用IO扩展板前，上电初始化操作(主机)
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
static void EK_SLP_mst_init(void)
{
    U08 i;

    /* IO扩展板相关标志初始化 */
    ARRCLR(m_fg_exio_klv_ver_err);
    ARRCLR(m_fg_exio_commu_err);
    ARRCLR(m_fg_exio_ver_ready);
    ARRCLR(m_fg_exio_remote_exist);
    ARRCLR(m_fg_exio_hdw_status_ready);
    ARRCLR(m_fg_exio_hdw_cfg_update);

    /* IO扩展板硬件初始化状态初始化 */
    ARRSET(m_exio_hdw_init_sta, EK_HDW_INIT_STA_PLAN);

    /* IO扩展板请求发送标志初始化 */
    ARR2SET(fg_exio_klv_req, 0);
    for (i = 0; i < EKC_EXIO_KLV_USED; i++)
    {
       SET_EXIO_KLV_REQ(i, EK_STAGE_KLV_CORE_SLP);
       SETBIT_N(m_fg_exio_commu_err, i);            /* 上电后默认初始化为通讯故障，避免首次上电检测不到内外机通讯故障 */
    }
}

/*******************************************************************************
 函 数: EK_SLP_mst_timer()
 功 能: IO扩展板计时器(主机)
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
static void EK_SLP_mst_timer(void)
{
    ;
}

/*******************************************************************************
 函 数: EK_SLP_mst_commu_err_handler()
 功 能: IO扩展板通信故障处理(主机)
 参 数: exio_id--IO扩展板号; 
 返 回: 空
 其 它: 空
*******************************************************************************/
static void EK_SLP_mst_commu_err_handler(U08 exio_id)
{
    U08 i, rsrc_type, num, bytes;
    U16 base_addr;

    if (!GET_EXIO_COMMU_ERR(exio_id))    return;

    /* 重头开始读 */
    ARRCLR(fg_exio_klv_req[exio_id]);
    SET_EXIO_KLV_REQ(exio_id, EK_STAGE_KLV_CORE_SLP);    

    /* 重新初始化 */
    m_exio_hdw_init_sta[exio_id] = EK_HDW_INIT_STA_PLAN;

    /* 扩展板数据不可用 */
    CLRBIT_N(m_fg_exio_ver_ready, exio_id);
    CLRBIT_N(m_fg_exio_remote_exist, exio_id);
    CLRBIT_N(m_fg_exio_hdw_status_ready, exio_id);
    CLRBIT_N(m_fg_exio_hdw_cfg_update, exio_id);

    /* 清输出 */
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
 函 数: EK_SLP_mst_fill_request_handler()
 功 能: 填充请求数据处理(主机)
 参 数: exio_id--IO扩展板号; 
        exio_klv_data--IO扩展板KLV数据; 
 返 回: 空
 其 它: 空
*******************************************************************************/
static void EK_SLP_mst_fill_request_handler(U08 exio_id, EXIO_KLV_DATA* exio_klv_data)
{
    U08 i;

    /* 通信故障时处理 */
    EK_SLP_mst_commu_err_handler(exio_id);

    /* 特殊跳转处理: 硬件配置更新时触发 */
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
    
    /* 请求发送策略(按阶段顺序请求) */
    for (i = 0; i < EK_STAGE_MAX; i++)
    {
        if (GET_EXIO_KLV_REQ(exio_id, i))
        {
            exio_klv_data->stage = i;
            CLR_EXIO_KLV_REQ(exio_id, i);
            break;
        }
    }

    /* 填充数据内容 */
    switch (exio_klv_data->stage)
    {
        /* IO扩展板KLV核心业务版本 */
        case EK_STAGE_KLV_CORE_SLP:
            /* DO NOTHING */
            break;

        /* IO扩展板微码版本号 */
        case EK_STAGE_APP_VER:
            /* DO NOTHING */
            break;

        /* IO扩展板硬件资源信息 */
        case EK_STAGE_HDW_RSRC_INFO:
            /* DO NOTHING */
            break;

        /* IO扩展板硬件配置信息 */
        case EK_STAGE_HDW_CFG_INFO:
            /* DO NOTHING */
            break;

        /* IO扩展板硬件初始化信息 */
        case EK_STAGE_HDW_INIT_INFO:
            /* DO NOTHING */
            break;

        /* IO扩展板硬件配置数据 */
        case EK_STAGE_HDW_CFG_DATA:
            EK_SLP_mst_fill_exio_hdw_cfg_data(exio_id, &exio_klv_data->buf[0]);
            break;

        /* IO扩展板硬件初始化数据 */
        case EK_STAGE_HDW_INIT_DATA:
            EK_SLP_mst_fill_exio_hdw_init_data(exio_id, &exio_klv_data->buf[0]);
            break;

        /* IO扩展板远端配置数据 */
        case EK_STAGE_REMOTE_CFG_DATA:
//            EK_mst_fill_exio_remote_cfg(exio_id, &exio_klv_data->buf[0]);
            break;

        /* IO扩展板硬件状态/输出读写 */
        case EK_STAGE_HDW_WR_RD:
            EK_SLP_mst_fill_exio_hdw_output_data(exio_id, &exio_klv_data->buf[0]);
            break;

        /* IO扩展板远端状态/输出读写 */
        case EK_STAGE_REMOTE_WR_RD:
//            EKC_mst_fill_exio_fwd_data(exio_id, &exio_klv_data->buf[0]);
            break; 
    }

}

/*******************************************************************************
 函 数: EK_SLP_mst_parse_respond_handler()
 功 能: 解析响应数据处理(主机)
 参 数: exio_id--IO扩展板号; 
        exio_klv_data--IO扩展板KLV数据; 
 返 回: 空
 其 它: 空
*******************************************************************************/
static void EK_SLP_mst_parse_respond_handler(U08 exio_id, EXIO_KLV_DATA* exio_klv_data)
{
    U08 i;
    
    /* 数据帧解析 */
    switch (exio_klv_data->stage)
    {
        /* IO扩展板KLV核心业务版本 */
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
    
        /* IO扩展板微码版本号 */
        case EK_STAGE_APP_VER:
            for (i = 0; i < MAX_VER_BYTE; i++)
            {
                m_exio_ver_info[exio_id][i] = exio_klv_data->buf[i];
            }
            SETBIT_N(m_fg_exio_ver_ready, exio_id);
            SET_EXIO_KLV_REQ(exio_id, EK_STAGE_HDW_RSRC_INFO);
            break;

        /* IO扩展板硬件资源信息 */
        case EK_STAGE_HDW_RSRC_INFO:
            EK_SLP_mst_parse_exio_hdw_rsrc_info(exio_id, &exio_klv_data->buf[0]);
            break;

        /* IO扩展板硬件配置信息 */
        case EK_STAGE_HDW_CFG_INFO:
            EK_SLP_mst_parse_exio_hdw_cfg_info(exio_id, &exio_klv_data->buf[0]);
            break;

        /* IO扩展板硬件初始化信息 */
        case EK_STAGE_HDW_INIT_INFO:
            EK_SLP_mst_parse_exio_hdw_init_info(exio_id, &exio_klv_data->buf[0]);
            break;

        /* IO扩展板硬件配置数据 */
        case EK_STAGE_HDW_CFG_DATA:
            SET_EXIO_KLV_REQ(exio_id, EK_STAGE_HDW_INIT_DATA);
            break;

        /* IO扩展板硬件初始化数据 */
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

        /* IO扩展板远端配置数据 */
        case EK_STAGE_REMOTE_CFG_DATA:
//            EK_mst_parse_exio_rds_cfg_confirm(exio_id, &exio_klv_data->buf[0]);
            SET_EXIO_KLV_REQ(exio_id, EK_STAGE_HDW_WR_RD);
            break;

        /* IO扩展板硬件状态/输出读写 */
        case EK_STAGE_HDW_WR_RD:
            EK_SLP_mst_parse_exio_hdw_status_info(exio_id, &exio_klv_data->buf[0]);
            break;

        /* IO扩展板远端状态/输出读写 */
        case EK_STAGE_REMOTE_WR_RD:
//            EK_mst_parse_exio_rds_data(exio_id, &exio_klv_data->buf[0]);
            SET_EXIO_KLV_REQ(exio_id, EK_STAGE_HDW_WR_RD);      /* 切换发送硬件输出数据 */
            break;
    }
}
#endif

#ifdef EXIO_KLV_EXTERN      /* 外部接口 */
/*******************************************************************************
 函 数: EK_SLP_mst_get_exio_fg()
 功 能: 获取IO扩展板状态相关标志(主机)
 参 数: exio_id--IO扩展板号; 
        fg_type--标志类型; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
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
 函 数: EK_SLP_mst_set_exio_fg()
 功 能: 设置IO扩展板状态相关标志(主机)
 参 数: exio_id--IO扩展板号; 
        fg_type--标志类型; 
        is_clr--是否清零(反之置位); 
 返 回: 空
 其 它: 空
*******************************************************************************/
void EK_SLP_mst_set_exio_fg(U08 exio_id, U08 fg_type, BOOL is_clr)
{
    U16* fg = PNULL;

    if (exio_id > EXIO_KLV_MAX)             return;
    if (fg_type > EK_FG_MAX)                return;
    if (fg_type == EK_FG_VER_READY)         return;  /* IO扩展板版本号信息可用标志不可外部改写 */
    if (fg_type == EK_FG_HDW_STATUS_READY)  return;  /* IO扩展板硬件状态信息可用标志不可外部改写 */
    
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
 函 数: EK_SLP_mst_get_exio_ver()
 功 能: 获取IO扩展板版本号信息(主机)
 参 数: exio_id--IO扩展板号; 
 返 回: IO扩展板版本号指针
 其 它: 空
*******************************************************************************/
U08* EK_SLP_mst_get_exio_ver(U08 exio_id)
{
    U08 id = (exio_id > EXIO_KLV_MAX) ? 0 : exio_id;
    return &m_exio_ver_info[id][0];
}

/*******************************************************************************
 函 数: EK_SLP_mst_get_exio_cell_data()
 功 能: 根据IO扩展板硬件资源类型，读取存储在m_exio_cell中的数据(主机)
 参 数: rsrc_type--硬件资源类型; 
        exio_id--IO扩展板号; 
        idx--通道号; 
        is_read--是否为可读(反之为可写); 
 返 回: 位数据/字数据
 其 它: 空
*******************************************************************************/
I16 EK_SLP_mst_get_exio_cell_data(U08 rsrc_type, U08 exio_id, U08 idx, U08 is_read)
{
    U16 base_addr, loc;

    /* 索引超界 */
    if (exio_id > EKC_EXIO_KLV_USED || rsrc_type > EK_RSRC_MAX)
    {
        return 0;
    }

    /* 无效基地址 */
    if (m_exio_cell_base_addr[exio_id][rsrc_type] == 0xffff)
    {
        return 0;
    }

    /* 计算可读/可写起始地址 */
    base_addr = EK_SLP_mst_get_exio_cell_base_addr(rsrc_type, exio_id, is_read);
    if (base_addr == 0xffff)
    {
        return 0;
    }

    if (M_IS_BIT_RSRC(exio_id, rsrc_type))
    {
        /* 按位处理 */
        loc = base_addr*16 + idx;
        return (_DIVUINT(loc, 16) < M_EXIO_CELL_BUF_SIZE) ? GETBIT_N(m_exio_cell[exio_id], loc) : 0;
    }
    else
    {
        /* 按字处理 */
        loc = base_addr + idx;
        return (loc < M_EXIO_CELL_BUF_SIZE) ? m_exio_cell[exio_id][loc] : 0;
    }
}

/*******************************************************************************
 函 数: EK_SLP_mst_set_exio_cell_data()
 功 能: 根据IO扩展板硬件资源类型，将数据值写入m_exio_cell中(主机)
 参 数: rsrc_type--硬件资源类型; 
        exio_id--IO扩展板号; 
        idx--通道号; 
        val--数据值; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void EK_SLP_mst_set_exio_cell_data(U08 rsrc_type, U08 exio_id, U08 idx, I16 val)
{
    U16 base_addr, loc;

    if (GET_EXIO_COMMU_ERR(exio_id))
    {
        return;
    }
    
    /* 索引超界 */
    if (exio_id > EKC_EXIO_KLV_USED || rsrc_type > EK_RSRC_MAX)
    {
        return;
    }

    /* 无效基地址 */
    if (m_exio_cell_base_addr[exio_id][rsrc_type] == 0xffff)
    {
        return;
    }
    
    /* 计算可写起始地址 */
    base_addr = EK_SLP_mst_get_exio_cell_base_addr(rsrc_type, exio_id, FALSE);
    if (base_addr == 0xffff)
    {
        return;
    }

    if (M_IS_BIT_RSRC(exio_id, rsrc_type))
    {  
        /* 按位处理 */
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
        /* 按字处理 */
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
  从 机 部 分
*******************************************************************************/
extern const EXIO_HDW_RSRC_INFO EKC_EXIO_HDW_RSRC_INFO[];
extern const EXIO_HDW_CFG_INFO EKC_EXIO_HDW_CFG_INFO[];
extern const EXIO_HDW_INIT_INFO EKC_EXIO_HDW_INIT_INFO[];

#ifdef EXIO_KLV_STATIC      /* 内部接口 */
#define S_EXIO_HDW_CFG_BUF_SIZE                  (50)       /* IO扩展板硬件配置缓存大小 */
#define S_EXIO_HDW_INIT_BUF_SIZE                 (50)       /* IO扩展板硬件初始化缓存大小 */
#define S_EXIO_CELL_BUF_SIZE                     (50)       /* IO扩展板硬件输出数据缓存大小 */
//#define EXIO_RDS_CFG_MAX                         (50)       /* 暂未启用 */

/* 通讯故障标志 */
static BOOL s_fg_exio_commu_err;

/* IO扩展板数据缓存基地址 */
static U16 s_exio_hdw_cfg_data_addr[EK_CFG_MAX];            /* IO扩展板硬件配置缓存基地址 */
static U16 s_exio_hdw_init_data_addr[EK_INIT_MAX];          /* IO扩展板硬件初始化缓存基地址 */
static U16 s_exio_cell_data_addr[EK_RSRC_MAX];              /* IO扩展板硬件输出数据缓存基地址 */

/* IO扩展板数据缓存 */
static I16 s_exio_hdw_cfg_data[S_EXIO_HDW_CFG_BUF_SIZE];    /* IO扩展板硬件配置数据缓存 */
static I16 s_exio_hdw_init_data[S_EXIO_HDW_INIT_BUF_SIZE];  /* IO扩展板硬件初始化数据缓存 */
static I16 s_exio_cell[S_EXIO_CELL_BUF_SIZE];               /* IO扩展板硬件状态/输出读写缓存 */

/* IO扩展板硬件初始化状态值 */
static U08 s_exio_hdw_init_sta;                             

//U08 exio_RDS_cfg_frame_cnt;                                 /* 暂未启用 */
//U08 exio_RDS_cfg_frame_idx;                                 /* 暂未启用 */
//EXIO_REMOTE_CFG exio_RDS_cfg[EXIO_RDS_CFG_MAX];             /* 远端数据配置(暂未启用) */

/* 是否为位变量 */
#define S_IS_BIT_RSRC(rsrc_type)                  (EK_SLP_slv_get_exio_hdw_rsrc_attr(rsrc_type) & EK_BIT)
/* 是否为可读 */
#define S_IS_RD_RSRC(rsrc_type)                   (EK_SLP_slv_get_exio_hdw_rsrc_attr(rsrc_type) & EK_RD)
/* 是否为可写 */
#define S_IS_WR_RSRC(rsrc_type)                   (EK_SLP_slv_get_exio_hdw_rsrc_attr(rsrc_type) & EK_WR)

/*******************************************************************************
 函 数: EK_SLP_slv_get_exio_hdw_rsrc_attr()
 功 能: 获取IO扩展板硬件资源属性(从机)
 参 数: rsrc_type--IO扩展板硬件资源类型; 
 返 回: IO扩展板硬件资源属性值
 其 它: 空
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
 函 数: EK_SLP_slv_get_hdw_rsrc_num()
 功 能: 获取IO扩展板硬件资源数(从机)
 参 数: rsrc_type--IO扩展板硬件资源类型; 
 返 回: IO扩展板硬件资源数
 其 它: 空
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
 函 数: EK_SLP_slv_check_exio_hdw_init_done()
 功 能: 检查硬件初始化是否完成(从机)
 参 数: 空
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
static BOOL EK_SLP_slv_check_exio_hdw_init_done(void)
{
    /* 电膨归零完成 */
    if (!EKC_slv_exio_eev_init_done())
    {
        return FALSE;
    }

    return TRUE;
}

/*******************************************************************************
 函 数: EK_SLP_slv_get_exio_cell_base_addr()
 功 能: 获取IO扩展板硬件状态/输出读写缓存基地址(从机)
 参 数: rsrc_type--IO扩展板硬件资源类型; 
 返 回: IO扩展板硬件状态/输出读写缓存基地址
 其 它: 空
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
 函 数: EK_SLP_slv_cal_exio_hdw_cfg_base_addr()
 功 能: 计算IO扩展板硬件配置数据缓存基地址
 参 数: 空
 返 回: 空
 其 它: 空
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
 函 数: EK_SLP_slv_cal_exio_hdw_init_base_addr()
 功 能: 计算扩展板硬件初始化缓存基地址
 参 数: 空
 返 回: 空
 其 它: 空
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
 函 数: EK_SLP_slv_cal_exio_cell_base_addr()
 功 能: 计算扩展板读写数据缓存基地址
 参 数: 空
 返 回: 空
 其 它: 只为具有"可写"属性的条目开辟空间
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
        base_addr += (attr & EK_WR) ? words : 0;             /* 仅开辟可写数据空间 */
    }
}

/*******************************************************************************
 函 数: EK_SLP_slv_rst_exio_cell_data()
 功 能: 复位指定硬件类型的扩展板读写缓存
 参 数: rsrc_type--IO扩展板硬件资源类型; 
        rst_val--复位值; 
 返 回: 空
 其 它: 空
*******************************************************************************/
static void EK_SLP_slv_rst_exio_cell_data(U08 rsrc_type, I16 rst_val)
{
    U16 base_addr;
    U08 num, words;
    U08 i;

    /* 硬件资源超过定义数时，不复位 */
    if (rsrc_type > EK_RSRC_MAX)    return;
    
    base_addr = s_exio_cell_data_addr[rsrc_type];
    num = EK_SLP_slv_get_hdw_rsrc_num(rsrc_type);
    words = S_IS_BIT_RSRC(rsrc_type) ? _DIVUCARRY(num, 16) : num;

    /* 超读写缓存时，不复位 */
    if (base_addr + words > S_EXIO_CELL_BUF_SIZE)    return;

    /* 读写缓存复位 */
    for (i = 0; i <words; i++)
    {
        s_exio_cell[base_addr+i] = rst_val;
    }
}

/*******************************************************************************
 函 数: EK_SLP_slv_parse_exio_hdw_cfg_data()
 功 能: 解析IO扩展板硬件配置数据(从机)
 参 数: buf--接收缓存; 
 返 回: 空
 其 它: 空
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

    /* 有效字节数 */
    total_bytes = buf[len++];

    /* 遍历数据单元 */
    while (TRUE)
    {
        if (len > total_bytes \
        || len > EXIO_KLV_DATA_BUF_BYTES)
        {
            break;
        }
        
        cfg_type = buf[len++];  /* 硬件资源配置类型: 1字节 */
        words = buf[len++];     /* 字数: 1字节 */

        /* 计算每种配置的基地址 */
        base_addr = s_exio_hdw_cfg_data_addr[cfg_type];
       
        /* 处理配置参数值 */
        for (i = 0; i < words; i++)
        {
            cfg_val = MAKEWORD(buf[len+i*2], buf[len+i*2+1]);  /* 参数值: 2字节 */
    
            idx = base_addr + i;
            if (idx < S_EXIO_HDW_CFG_BUF_SIZE)
            {
                s_exio_hdw_cfg_data[idx] = cfg_val;
            }
        }
        len += words*2;
        
    }

    /* 硬件配置处理 */
    EKC_slv_exio_hdw_cfg_handler();                    
}

/*******************************************************************************
 函 数: EK_SLP_slv_parse_exio_hdw_init_data()
 功 能: 解析IO扩展板硬件初始化数据(从机)
 参 数: buf--接收缓存; 
 返 回: 空
 其 它: 空
*******************************************************************************/
static void EK_SLP_slv_parse_exio_hdw_init_data(U08 *buf)
{
    U08 i;
    U08 init_type, words;
    U08 total_bytes;
    I16 init_val;
    U16 base_addr, idx;
    U08 len = 0;

    /* 有效字节数 */
    total_bytes = buf[len++];

    /* 遍历数据单元 */
    while (TRUE)
    {
        if (len > total_bytes \
        || len > EXIO_KLV_DATA_BUF_BYTES)
        {
            break;
        }

        /* 解析头信息 */
        init_type = buf[len++]; /* 硬件资源初始化类型: 1字节 */
        words = buf[len++];     /* 字数: 1字节 */
    
        /* 获取每种初始化的基地址 */
        base_addr = s_exio_hdw_init_data_addr[init_type];
    
        for (i = 0; i < words; i++)
        {
            init_val = MAKEWORD(buf[len+i*2], buf[len+i*2+1]);  /* 参数值: 2字节 */
    
            idx = base_addr + i;
            if (idx < S_EXIO_HDW_INIT_BUF_SIZE)
            {
                s_exio_hdw_init_data[idx] = init_val;
            }
        }
        len += words*2;
    }

    /* 硬件初始化处理 */    
    EKC_slv_exio_hdw_init_handler();

    /* 硬件初始化状态 */
    s_exio_hdw_init_sta = EK_HDW_INIT_STA_DOING;
}

/*******************************************************************************
 函 数: EK_SLP_slv_parse_exio_hdw_output_data()
 功 能: 解析IO扩展板硬件输出数据(从机)
 参 数: buf--接收缓存; 
 返 回: 空
 其 它: 空
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

    /* 有效字节数 */
    total_bytes = buf[len++];

    /* 遍历数据单元 */
    while (TRUE)
    {
        if (len > total_bytes \
        || len > EXIO_KLV_DATA_BUF_BYTES)
        {
            break;
        }

        /* 解析头信息 */
        rsrc_type = buf[len++]; /* 硬件资源类型: 1字节 */
        words = buf[len++];     /* 字数: 1字节 */
    
        base_addr = s_exio_cell_data_addr[rsrc_type];
    
        /* 处理硬件输出值 */
        for (i = 0; i < words; i++)
        {
            val = MAKEWORD(buf[len+i*2], buf[len+i*2+1]);   /* 输出值: 2字节 */
    
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
 函 数: EK_SLP_slv_fill_exio_hdw_rsrc_info()
 功 能: 填充IO扩展板硬件资源信息(从机)
 参 数: buf--发送缓存; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void EK_SLP_slv_fill_exio_hdw_rsrc_info(U08* buf)
{
    U08 i;
    U08 len = 1;            /* 开头预留1字节 */
                
    /* IO扩展板硬件资源 */
    buf[len++] = EKC_get_exio_hdw_rsrc_info_num();
    for (i = 0; i < EKC_get_exio_hdw_rsrc_info_num(); i++)
    {
        MEMCPY(&buf[len], &EKC_EXIO_HDW_RSRC_INFO[i], EXIO_HDW_RSRC_INFO_BYTES);
        len += EXIO_HDW_RSRC_INFO_BYTES;
    }

    buf[0] = len - 1;
}

/*******************************************************************************
 函 数: EK_SLP_slv_fill_exio_hdw_cfg_info()
 功 能: 填充IO扩展板硬件配置信息(从机)
 参 数: buf--发送缓存; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void EK_SLP_slv_fill_exio_hdw_cfg_info(U08* buf)
{
    U08 i;
    U08 len = 1;            /* 开头预留1字节 */

    /* 硬件资源配置项 */
    buf[len++] = EKC_get_exio_hdw_cfg_info_num();
    for (i = 0; i < EKC_get_exio_hdw_cfg_info_num(); i++)
    {
        MEMCPY(&buf[len], &EKC_EXIO_HDW_CFG_INFO[i], EXIO_HDW_CFG_BYTES_INFO);
        len += EXIO_HDW_CFG_BYTES_INFO;
    }

    buf[0] = len - 1;
}

/*******************************************************************************
 函 数: EK_SLP_slv_fill_exio_hdw_init_info()
 功 能: 填充IO扩展板硬件初始化信息(从机)
 参 数: buf--发送缓存; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void EK_SLP_slv_fill_exio_hdw_init_info(U08* buf)
{
    U08 i;
    U08 len = 1;            /* 开头预留1字节 */
                
    /* 硬件初始化项 */
    buf[len++] = EKC_get_exio_hdw_init_info_num();
    for (i = 0; i < EKC_get_exio_hdw_init_info_num(); i++)
    {
        MEMCPY(&buf[len], &EKC_EXIO_HDW_INIT_INFO[i], EXIO_HDW_INIT_BYTES_INFO);
        len += EXIO_HDW_INIT_BYTES_INFO;
    }

    buf[0] = len - 1;
}

/*******************************************************************************
 函 数: EK_SLP_slv_fill_exio_hdw_status_info()
 功 能: 填充IO扩展板硬件状态信息(从机)
 参 数: buf--; 
 返 回: 空
 其 它: 空
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

            /* 计算字数 */
            words = (rsrc_attr & EK_BIT) ? _DIVUCARRY(rsrc_num, 16) : rsrc_num;

            for (j = 0; j < words; j++)
            {
                switch (rsrc_type)
                {
                    case EK_RSRC_DI:   val = EKC_SLV_EXIO_DI(j);      break;/* 开关量 */
                    case EK_RSRC_DO:   val = EKC_SLV_EXIO_DO(j);      break;/* 继电器 */
                    case EK_RSRC_SW:   val = EKC_SLV_EXIO_SW(j);      break;/* 拨码 */                    
                    case EK_RSRC_TMP:  val = EKC_SLV_EXIO_TMP(j);     break;/* 温度 */
                    case EK_RSRC_PRS:  val = EKC_SLV_EXIO_PRS(j);     break;/* 压力 */
                    case EK_RSRC_EEV:  val = EKC_SLV_EXIO_ST_PV(j);   break;/* 电膨开度 */               
                    case EK_RSRC_FAN:  val = EKC_SLV_EXIO_FAN_PV(j);  break;/* 板载PWM风机 */               
                    case EK_RSRC_PH:   val = EKC_SLV_EXIO_PH(j);      break;/* 相序 */
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
 函 数: EK_slv_parse_exio_rds_cfg()
 功 能: 
 参 数: buf--; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void EK_slv_parse_exio_rds_cfg(U08* buf)
{
    U08 i;
    U08 cellWords, cfgTotal, cfgBgn, dataBytes, cfgIdx;
    U16* destBuf = PNULL;
    U08 len = 0;

    exio_RDS_cfg_frame_cnt = buf[len++];    /* 总帧数 */
    exio_RDS_cfg_frame_idx = buf[len++];    /* 当前帧号 */
    cellWords = buf[len++];                 /* 数据单元字数 */
    cfgTotal = buf[len++];                  /* 配置项总数 */
    cfgBgn = buf[len++];                    /* 起始配置项序号 */
    dataBytes = buf[len++];                 /* 数据总字节数 */

    if (cfgBgn >= cfgTotal                  /* 主机定义最大个数 */
    || cfgTotal >= EXIO_RDS_CFG_MAX)        /* 从机支持最大个数 */
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
 函 数: EK_SLP_slv_init()
 功 能: IO扩展板上电初始化操作(从机)
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
static void EK_SLP_slv_init(void)
{
    /* 计算硬件配置缓存基地址 */
    EK_SLP_slv_cal_exio_hdw_cfg_base_addr();

    /* 计算硬件初始化缓存基地址 */
    EK_SLP_slv_cal_exio_hdw_init_base_addr();

    /* 计算数据读写缓存基地址 */
    EK_SLP_slv_cal_exio_cell_base_addr();

    /* 硬件初始化状态初始化 */
    s_exio_hdw_init_sta = EK_HDW_INIT_STA_PLAN;

    /* 扩展板通讯故障标志初始化 */
    s_fg_exio_commu_err = FALSE;

//    /* 其它变量初始化(暂未启用) */    
//    exio_RDS_cfg_frame_cnt = 0;
//    exio_RDS_cfg_frame_idx = 0;
}

/*******************************************************************************
 函 数: EK_SLP_slv_timer()
 功 能: IO扩展板计时器(从机)
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
static void EK_SLP_slv_timer(void)
{
    ;
}

/*******************************************************************************
 函 数: EK_SLP_slv_commu_err_handler()
 功 能: IO扩展板通讯故障处理(从机)
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
static void EK_SLP_slv_commu_err_handler(void)
{
    if (GET_EXIO_COMMU_ERR)
    {
        /* 重新初始化 */
        s_exio_hdw_init_sta = EK_HDW_INIT_STA_PLAN;
    
        /* 清输出 */
        EK_SLP_slv_rst_exio_cell_data(EK_RSRC_DO, 0);
        
        /* 关电膨 */
        EK_SLP_slv_rst_exio_cell_data(EK_RSRC_EEV, 0);
    
        /* 关风机 */
        EK_SLP_slv_rst_exio_cell_data(EK_RSRC_FAN, 0);
    }
}

/*******************************************************************************
 函 数: EK_SLP_slv_parse_request_handler()
 功 能: 解析请求数据处理(从机)
 参 数: exio_klv_data--IO扩展板KLV数据; 
 返 回: 空
 其 它: 空
*******************************************************************************/
static void EK_SLP_slv_parse_request_handler(EXIO_KLV_DATA* exio_klv_data)
{
    switch (exio_klv_data->stage)
    {
        /* IO扩展板KLV核心业务版本 */
        case EK_STAGE_KLV_CORE_SLP:
            /* DO NOTHING */
            break;

        /* IO扩展板微码版本号 */
        case EK_STAGE_APP_VER:
            /* DO NOTHING */
            break;

        /* IO扩展板硬件资源信息 */
        case EK_STAGE_HDW_RSRC_INFO:
            /* DO NOTHING */
            break;

        /* IO扩展板硬件配置信息 */
        case EK_STAGE_HDW_CFG_INFO:
            /* DO NOTHING */
            break;

        /* IO扩展板硬件初始化信息 */
        case EK_STAGE_HDW_INIT_INFO:
            /* DO NOTHING */
            break;

         /* IO扩展板硬件配置数据 */
        case EK_STAGE_HDW_CFG_DATA:
            EK_SLP_slv_parse_exio_hdw_cfg_data(&exio_klv_data->buf[0]);
            break;

        /* IO扩展板硬件初始化数据 */
        case EK_STAGE_HDW_INIT_DATA:
            EK_SLP_slv_parse_exio_hdw_init_data(&exio_klv_data->buf[0]);
            break;           

        /* IO扩展板远端配置数据 */
        case EK_STAGE_REMOTE_CFG_DATA:
//            EK_slv_parse_exio_rds_cfg(&exio_klv_data->buf[0]);
            break;

        /* IO扩展板硬件状态/输出读写 */
        case EK_STAGE_HDW_WR_RD:
            EK_SLP_slv_parse_exio_hdw_output_data(&exio_klv_data->buf[0]);
            break;
            
        /* IO扩展板远端状态/输出读写 */
        case EK_STAGE_REMOTE_WR_RD:
//            EK_slv_parse_exio_rds_data(&exio_klv_data->buf[0]);
            break;
    }
}

/*******************************************************************************
 函 数: EK_SLP_slv_fill_respond_handler()
 功 能: 填充响应数据处理(从机)
 参 数: exio_klv_data--IO扩展板KLV数据; 
 返 回: 空
 其 它: 空
*******************************************************************************/
static void EK_SLP_slv_fill_respond_handler(EXIO_KLV_DATA* exio_klv_data)
{
    switch (exio_klv_data->stage)
    {
        /* IO扩展板KLV核心业务版本 */
        case EK_STAGE_KLV_CORE_SLP:
            exio_klv_data->buf[0] = HIBYTE(EXIO_KLV_SLP_CORE_VER);
            exio_klv_data->buf[1] = LOBYTE(EXIO_KLV_SLP_CORE_VER);
            break;

        /* IO扩展板微码版本号 */
        case EK_STAGE_APP_VER:
            MEMCPY(&exio_klv_data->buf[0], \
                   (U08*)EKC_EXIO_VER, \
                   (sizeof(EKC_EXIO_VER)/sizeof(U08)));
            break;

        /* IO扩展板硬件资源信息 */
        case EK_STAGE_HDW_RSRC_INFO:
            EK_SLP_slv_fill_exio_hdw_rsrc_info(&exio_klv_data->buf[0]);
            break;

        /* IO扩展板硬件配置信息 */
        case EK_STAGE_HDW_CFG_INFO:
            EK_SLP_slv_fill_exio_hdw_cfg_info(&exio_klv_data->buf[0]);
            break;

        /* IO扩展板硬件初始化信息 */
        case EK_STAGE_HDW_INIT_INFO:
            EK_SLP_slv_fill_exio_hdw_init_info(&exio_klv_data->buf[0]);
            break;

        /* IO扩展板硬件配置数据 */
        case EK_STAGE_HDW_CFG_DATA:
            /* DO NOTHING */
            break;

        /* IO扩展板硬件初始化数据 */
        case EK_STAGE_HDW_INIT_DATA:
            /* DO NOTHING */
            break;

        /* IO扩展板远端配置数据 */
        case EK_STAGE_REMOTE_CFG_DATA:
//            EK_slv_fill_exio_rds_cfg_confirm(&exio_klv_data->buf[0]);
            break;

        /* IO扩展板硬件状态/输出读写 */
        case EK_STAGE_HDW_WR_RD:
            EK_SLP_slv_fill_exio_hdw_status_info(&exio_klv_data->buf[0]);
            break;

        /* IO扩展板远端状态/输出读写 */
        case EK_STAGE_REMOTE_WR_RD:
            /* DO NOTHING */
            break;
    }
}
#endif

#ifdef EXIO_KLV_EXTERN      /* 外部接口 */
/*******************************************************************************
 函 数: EK_SLP_slv_get_exio_commu_err()
 功 能: 获取IO扩展板通信故障(从机)
 参 数: 空
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL EK_SLP_slv_get_exio_commu_err(void)
{
    return s_fg_exio_commu_err;
}

/*******************************************************************************
 函 数: EK_SLP_slv_set_exio_fg()
 功 能: 获取IO扩展板状态相关标志(从机)
 参 数: exio_id--IO扩展板号; 
        fg_type--标志类型; 
        is_clr--是否清空(反之置位); 
 返 回: 空
 其 它: 空
*******************************************************************************/
void EK_SLP_slv_set_exio_commu_err(BOOL is_clr)
{
    s_fg_exio_commu_err = is_clr ? FALSE : TRUE;
}

/*******************************************************************************
 函 数: EK_SLP_slv_get_exio_hdw_cfg_data()
 功 能: 获取IO扩展板硬件配置数据(从机)
 参 数: cfg_type--IO扩展板硬件配置类型; 
        idx--通道号; 
 返 回: IO扩展板硬件配置数据
 其 它: 空
*******************************************************************************/
I16 EK_SLP_slv_get_exio_hdw_cfg_data(U08 cfg_type, U08 idx)
{
    U16 loc;

    /* 索引超界 */
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
 函 数: EK_SLP_slv_get_exio_hdw_init_data()
 功 能: 获取IO扩展板硬件初始化数据(从机)
 参 数: init_type--IO扩展板硬件配置类型;
        idx--通道号; 
 返 回: IO扩展板硬件初始化数据
 其 它: 空
*******************************************************************************/
I16 EK_SLP_slv_get_exio_hdw_init_data(U08 init_type, U08 idx)
{
    U16 loc;

    /* 索引超界 */
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
 函 数: EK_SLP_slv_get_exio_cell_data()
 功 能: 根据IO扩展板硬件资源类型，读取存储在m_exio_cell中的数据(从机)
 参 数: rsrc_type--IO扩展板资源类型; 
        idx--通道号; 
 返 回: 位数据/字数据
 其 它: 空
*******************************************************************************/
I16 EK_SLP_slv_get_exio_cell_data(U08 rsrc_type, U08 idx)
{
    U16 base_addr, loc;

    /* 索引超界 */
    if (rsrc_type > EK_RSRC_MAX)
    {
        return 0;
    }

    /* 无效基地址 */
    if (s_exio_cell_data_addr[rsrc_type] == 0xffff)
    {
        return 0;
    }

    /* 计算可写起始地址 */
    base_addr = EK_SLP_slv_get_exio_cell_base_addr(rsrc_type);
    if (base_addr == 0xffff)
    {
        return 0;
    }
    
    if (S_IS_BIT_RSRC(rsrc_type))
    {
        /* 按位处理 */
        loc = base_addr * 16 + idx;
        return (_DIVUINT(loc, 16) < S_EXIO_CELL_BUF_SIZE) ? GETBIT_N(s_exio_cell, loc) : 0;
    }
    else
    {
        /* 按字处理 */
        loc = base_addr + idx;
        return (loc < S_EXIO_CELL_BUF_SIZE) ? s_exio_cell[loc] : 0;
    }
}

/*******************************************************************************
 函 数: EK_SLP_slv_set_exio_cell_data()
 功 能: 根据IO扩展板硬件资源类型，将数据值写入m_exio_cell中(从机)
 参 数: rsrc_type--IO扩展板资源类型; 
        idx--通道号; 
        val--设定值; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void EK_SLP_slv_set_exio_cell_data(U08 rsrc_type, U08 idx, I16 val)
{
    U16 base_addr, loc;

    /* 索引超界 */
    if (rsrc_type > EK_RSRC_MAX)
    {
        return;
    }

    /* 无效基地址 */
    if (s_exio_cell_data_addr[rsrc_type] == 0xffff)
    {
        return;
    }

    /* 计算可写起始地址 */
    base_addr = EK_SLP_slv_get_exio_cell_base_addr(rsrc_type);
    if (base_addr == 0xffff)
    {
        return;
    }

    if (S_IS_BIT_RSRC(rsrc_type))
    {
        /* 按位处理 */
        loc = base_addr * 16 + idx;
        if (_DIVUINT(loc, 16) >= S_EXIO_CELL_BUF_SIZE)
        {
            return;
        }
        val ? SETBIT_N(s_exio_cell, loc) : CLRBIT_N(s_exio_cell, loc);
    }
    else
    {
        /* 按字处理 */
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

