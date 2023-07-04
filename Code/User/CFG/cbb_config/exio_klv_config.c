/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: exio_klv_config.c
 文件描述: IO扩展功能配置
 创建人  : J.H.
 创建日期: 2022.01.14
*******************************************************************************/
#define EXIO_KLV_CONFIG_GLOBAL
#include "includes.h"           /* 请勿移动此语句位置! */

#ifdef EXIO_KLV_MASTER
/* ========================================================================================== */
/* 主 机 配 置                                                                                */
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

/* 主机定义: IO扩展板所支持的远端类型 */
const U08 EKC_REMOTE_TYPE_USED[] = 
{
    EK_REMOTE_INVT,         /* 邦普变频压机驱动 */
    EK_REMOTE_INVT_FAN,     /* 邦普变频风机驱动 */
};

U08 EKC_get_remote_type_used_num(void)
{
    return ARRELE(EKC_REMOTE_TYPE_USED);
}

U08 EKC_get_remote_info_baseAddr(U08 remoteType)
{
    switch (remoteType)
    {
        case EK_REMOTE_INVT:           return val_invt_addr;                /* 邦普变频驱动(压机、驱动PWM风机) */
        case EK_REMOTE_INVT_FAN:       return 110;                          /* 邦普变频风机驱动(邦普EC风机) */
        case EK_REMOTE_KM_INVT_FAN:    return val_ec_fan_base_addr;         /* 科贸EC风机 */
        case EK_REMOTE_ZSFN_INVT_FAN:  return val_ec_fan_base_addr;         /* 顺德四格EC风机 */          
    }

    return 0;
}


U08 EKC_get_remote_info_deviceMax(U08 remoteType)
{
    switch (remoteType)
    {
        case EK_REMOTE_INVT:           return 1;                            /* 邦普变频驱动(压机、驱动PWM风机) */
        case EK_REMOTE_INVT_FAN:       return 2;                            /* 邦普变频风机驱动(邦普EC风机) */
        case EK_REMOTE_KM_INVT_FAN:    return ECFAN_MAX;                    /* 科贸EC风机 */   
        case EK_REMOTE_ZSFN_INVT_FAN:  return ECFAN_MAX;                    /* 顺德四格EC风机 */   
    }

    return 0;
}

const U08* EKC_get_remote_info_cfgList(U08 remoteType)
{
    switch (remoteType)
    {
        case EK_REMOTE_INVT:           return EKC_RDS_CFG_INVT;             /* 邦普变频驱动(压机、驱动PWM风机) */
        case EK_REMOTE_INVT_FAN:       return EKC_RDS_CFG_INVT_FAN;         /* 邦普变频风机驱动(邦普EC风机) */
        case EK_REMOTE_KM_INVT_FAN:    return PNULL;                        /* 科贸EC风机 */
        case EK_REMOTE_ZSFN_INVT_FAN:  return PNULL;                        /* 顺德四格EC风机 */
    }

    return PNULL;
}

U08 EKC_get_remote_info_listLen(U08 remoteType)
{
    switch (remoteType)
    {
        case EK_REMOTE_INVT:           return ARRELE(EKC_RDS_CFG_INVT);     /* 邦普变频驱动(压机、驱动PWM风机) */
        case EK_REMOTE_INVT_FAN:       return ARRELE(EKC_RDS_CFG_INVT_FAN); /* 邦普变频风机驱动(邦普EC风机) */
        case EK_REMOTE_KM_INVT_FAN:    return 0;                            /* 科贸EC风机 */
        case EK_REMOTE_ZSFN_INVT_FAN:  return 0;                            /* 顺德四格EC风机 */
    }

    return 0;    
}

U16 EKC_get_remote_cfg_funcCode(U08 cfg_id)
{
    switch (cfg_id)
    {
        case EKC_REMOTE_CFG_INVT_AD:                 /* 4635 三相中的两相AD值 */
        case EKC_REMOTE_CFG_INVT_STATES:             /* 变频器状态 */
        case EKC_REMOTE_CFG_INVT_CUR_WAVE:           /* 3337 电流纹波 */
        case EKC_REMOTE_CFG_INVT_PARAM:              /* 基本参数 */      
        case EKC_REMOTE_CFG_INVT_PARAM_FA_1:         /* FA参数(第1段) */
        case EKC_REMOTE_CFG_INVT_PARAM_FA_2:         /* FA参数(第2段) */
        case EKC_REMOTE_CFG_INVT_PARAM_FB:           /* FB参数 */
        case EKC_REMOTE_CFG_INVT_PARAM_F10:          /* F10参数 */        
        case EKC_REMOTE_CFG_INVT_EC_FAN_FG:          /* 1537 ec风机指令电压分辨率 */
        case EKC_REMOTE_CFG_INVT_PARAM_F9:           /* F10参数 */
            return MB_RD_HOLDING_REGISTERS;
    }

    return 0xff;
}

U16 EKC_get_remote_cfg_commuIntv(U08 cfg_id)
{
    switch (cfg_id)
    {
        case EKC_REMOTE_CFG_INVT_AD:                 /* 4635 三相中的两相AD值 */
        case EKC_REMOTE_CFG_INVT_STATES:             /* 变频器状态 */
        case EKC_REMOTE_CFG_INVT_CUR_WAVE:           /* 3337 电流纹波 */
            return EK_INTV_1_SEC;

        case EKC_REMOTE_CFG_INVT_PARAM:              /* 基本参数 */      
        case EKC_REMOTE_CFG_INVT_PARAM_FA_1:         /* FA参数(第1段) */
        case EKC_REMOTE_CFG_INVT_PARAM_FA_2:         /* FA参数(第2段) */
        case EKC_REMOTE_CFG_INVT_PARAM_FB:           /* FB参数 */
        case EKC_REMOTE_CFG_INVT_PARAM_F10:          /* F10参数 */        
        case EKC_REMOTE_CFG_INVT_EC_FAN_FG:          /* 1537 ec风机指令电压分辨率 */
        case EKC_REMOTE_CFG_INVT_PARAM_F9:           /* F10参数 */
            return EK_INTV_10_SEC;
    }

    return EK_INTV_INF;
}

U16 EKC_get_remote_cfg_prior(U08 cfg_id)
{
    switch (cfg_id)
    {
        case EKC_REMOTE_CFG_INVT_AD:                 /* 4635 三相中的两相AD值 */
        case EKC_REMOTE_CFG_INVT_STATES:             /* 变频器状态 */
        case EKC_REMOTE_CFG_INVT_CUR_WAVE:           /* 3337 电流纹波 */
        case EKC_REMOTE_CFG_INVT_PARAM:              /* 基本参数 */      
        case EKC_REMOTE_CFG_INVT_PARAM_FA_1:         /* FA参数(第1段) */
        case EKC_REMOTE_CFG_INVT_PARAM_FA_2:         /* FA参数(第2段) */
        case EKC_REMOTE_CFG_INVT_PARAM_FB:           /* FB参数 */
        case EKC_REMOTE_CFG_INVT_PARAM_F10:          /* F10参数 */        
        case EKC_REMOTE_CFG_INVT_EC_FAN_FG:          /* 1537 ec风机指令电压分辨率 */
        case EKC_REMOTE_CFG_INVT_PARAM_F9:           /* F10参数 */
            return Q_PRIO_LOW;
    }

    return Q_PRIO_LOW;
}

U16 EKC_get_remote_cfg_dataAddr(U08 cfg_id)
{
    switch (cfg_id)
    {
        case EKC_REMOTE_CFG_INVT_AD:          return INVT_ADDR_AD;               /* 4635 三相中的两相AD值 */
        case EKC_REMOTE_CFG_INVT_STATES:      return invt_states_get_addr();     /* 变频器状态 */
        case EKC_REMOTE_CFG_INVT_CUR_WAVE:    return INVT_ADDR_CUR_WAVE;         /* 3337 电流纹波 */
        case EKC_REMOTE_CFG_INVT_PARAM:       return invt_paras_get_addr();      /* 基本参数 */
        case EKC_REMOTE_CFG_INVT_PARAM_FA_1:  return invt_paras_fa_get_addr();   /* FA参数(第1段) */
        case EKC_REMOTE_CFG_INVT_PARAM_FA_2:  return FA_ADDR(FA_NUM1);           /* FA参数(第2段) */
        case EKC_REMOTE_CFG_INVT_PARAM_FB:    return invt_paras_fb_get_addr();   /* FB参数 */
        case EKC_REMOTE_CFG_INVT_PARAM_F10:   return invt_paras_f10_get_addr();  /* F10参数 */
        case EKC_REMOTE_CFG_INVT_EC_FAN_FG:   return INVT_ADDR_EC_FAN_FG;        /* 1537 ec风机指令电压分辨率 */
        case EKC_REMOTE_CFG_INVT_PARAM_F9:    return invt_paras_f9_get_addr();   /* F10参数 */
    }

    return 0xffff;
}

U16 EKC_get_remote_cfg_dataNum(U08 cfg_id)
{
    switch (cfg_id)
    {
        case EKC_REMOTE_CFG_INVT_AD:          return INVT_AD_SIZE;               /* 4635 三相中的两相AD值 */       /* 2 */
        case EKC_REMOTE_CFG_INVT_STATES:      return invt_states_get_size();     /* 变频器状态 */                  /* 新19，旧17 */
        case EKC_REMOTE_CFG_INVT_CUR_WAVE:    return INVT_CUR_WAVE_SIZE;         /* 3337 电流纹波 */               /* 1 */
        case EKC_REMOTE_CFG_INVT_PARAM:       return invt_paras_get_size();      /* 基本参数 */                    /* 16 */
        case EKC_REMOTE_CFG_INVT_PARAM_FA_1:  return FA_NUM1;                    /* FA参数(第1段) */               /* 40 */
        case EKC_REMOTE_CFG_INVT_PARAM_FA_2:  return FA_MAX - FA_NUM1;           /* FA参数(第2段) */               /* 25 */
        case EKC_REMOTE_CFG_INVT_PARAM_FB:    return invt_paras_fb_get_size();   /* FB参数 */                      /* 10 */
        case EKC_REMOTE_CFG_INVT_PARAM_F10:   return invt_paras_f10_get_size();  /* F10参数 */                     /* 10 */
        case EKC_REMOTE_CFG_INVT_EC_FAN_FG:   return INVT_EC_FAN_FG_SIZE;        /* 1537 ec风机指令电压分辨率 */   /* 1 */
        case EKC_REMOTE_CFG_INVT_PARAM_F9:    return invt_paras_f9_get_size();   /* F10参数 */                     /* 1 */
    }

    return 0;
}
#endif

/*******************************************************************************
 函 数: EKC_mst_get_exio_tmp_cfg()
 功 能: 获取IO扩展板温度探头配置数据(主机)
 参 数: cfg_type--IO扩展板硬件配置类型; 
		rsrc_id--IO扩展板硬件资源ID; 
 返 回: IO扩展板温度探头配置数据
 其 它: 空
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
 函 数: EKC_mst_get_exio_eev_cfg()
 功 能: 获取IO扩展板电膨配置数据(主机)
 参 数: cfg_type--IO扩展板硬件配置类型; 
        rsrc_id--IO扩展板硬件资源ID; 
 返 回: IO扩展板电膨配置数据
 其 它: 空
*******************************************************************************/
I16 EKC_mst_get_exio_eev_cfg(U08 cfg_type, U08 rsrc_id)
{
    switch (rsrc_id)
    {
        /* 主阀 */
        case EEV_ID_1:
            switch (cfg_type)
            {
                /* 电膨励磁方式 */
                case EK_CFG_EEV_FREQ_TYPE:  return val_motor_fre_type;

                /* 电膨励磁频率 */
                case EK_CFG_EEV_FREQ:       return val_motor_step_fre;

                /* 电膨最大步数 */
                case EK_CFG_EEV_MAX_STEP:   return val_motor_step_max;
            }

        /* 辅阀 */
        case EEV_ID_2:
            switch (cfg_type)
            {
                /* 电膨励磁方式 */
                case EK_CFG_EEV_FREQ_TYPE:  return val_s_motor_fre_type;

                /* 电膨励磁频率 */
                case EK_CFG_EEV_FREQ:       return val_s_motor_step_fre;
                
                /* 电膨最大步数 */
                case EK_CFG_EEV_MAX_STEP:   return val_s_motor_step_max;
            }
    }
    
    return 0xffff;          
}

/*******************************************************************************
 函 数: EKC_mst_get_exio_hdw_init()
 功 能: 获取IO扩展板电膨初始化数据(主机)
 参 数: init_type--IO扩展板硬件初始化类型; 
		rsrc_id--IO扩展板硬件资源ID; 
 返 回: IO扩展板电膨初始化数据
 其 它: 空
*******************************************************************************/
I16 EKC_mst_get_exio_hdw_init(U08 init_type, U08 rsrc_id)
{
    static BOL fg_first_init[EEV_ID_MAX] = {TRUE, TRUE};
    
    switch (rsrc_id)
    {
        /* 主阀 */
        case EEV_ID_1:
            switch (init_type)
            {
                /* 电膨初始当前步数 */
                case EK_INIT_EEV_ST_PV:
                    /* 主阀禁用时，不执行初始化 */
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

        /* 辅阀 */
        case EEV_ID_2:
            switch (init_type)
            {
                /* 电膨初始当前步数 */
                case EK_INIT_EEV_ST_PV:  
                    /* 辅阀禁用时，不执行初始化 */
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
 函 数: EKC_mst_insert_request_node()
 功 能: 队列插入发送节点(主机)
 参 数: com_id--串口号; 
        exio_id--IO扩展板号; 
 返 回: 节点插入状态值
 其 它: 空
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
        rd_succ = COM_ERR_MAC_ADDR;     /* 设备地址错 */
    }
    
    return rd_succ;
}

#else
/* ========================================================================================== */
/* 从 机 配 置 ( IO 扩 展 板 )                                                                */
/* ========================================================================================== */

/* IO扩展板硬件资源信息 */
const EXIO_HDW_RSRC_INFO EKC_EXIO_HDW_RSRC_INFO[] =
{
    { EK_RSRC_DI,   EK_BIT|EK_RD,        MAX_DI_NUM,    },/* 开关量输入, 7路 */
    { EK_RSRC_DO,   EK_BIT|EK_RD|EK_WR,  MAX_DO_NUM,    },/* 继电器输出, 8路 */
    { EK_RSRC_SW,   EK_BIT|EK_RD,        MAX_SW_BIT,    },/* 拨码开关, 6位 */
    { EK_RSRC_TMP,  EK_RD,               MAX_TEMP_NUM,  },/* 温度探头, 10路  */
    { EK_RSRC_PRS,  EK_RD,               MAX_AI_NUM,    },/* 压力传感器, 2路 */
    { EK_RSRC_EEV,  EK_RD|EK_WR|EK_INIT, MAX_MOTOR_NUM, },/* 电子膨胀阀, 2路 */
    { EK_RSRC_FAN,  EK_RD|EK_WR,         FREQSPEEDNUM,  },/* 板载PWM风机, 2路 */
    { EK_RSRC_PH,   EK_RD,               1,             },/* 相序检测, 1路 */
//    { EK_RSRC_INVT, EK_REMOTE,           1,             },/* 变频驱动, 1路 */
};

/*******************************************************************************
 函 数: EKC_get_exio_hdw_rsrc_info_num()
 功 能: 获取IO扩展板硬件资源信息数(从机)
 参 数: 空
 返 回: IO扩展板硬件资源信息数
 其 它: 空
*******************************************************************************/
U08 EKC_get_exio_hdw_rsrc_info_num(void)
{
    return ARRELE(EKC_EXIO_HDW_RSRC_INFO);
}

/* IO扩展板硬件配置信息 */
const EXIO_HDW_CFG_INFO EKC_EXIO_HDW_CFG_INFO[] = 
{
    { EK_CFG_TMP_NTC_TYPE,  EK_RSRC_TMP, }, /* 温度探头类型 */
    { EK_CFG_EEV_FREQ_TYPE, EK_RSRC_EEV, },	/* 电膨励磁方式 */
    { EK_CFG_EEV_FREQ,      EK_RSRC_EEV, },	/* 电膨励磁频率 */
    { EK_CFG_EEV_MAX_STEP,  EK_RSRC_EEV, },	/* 电膨最大步数 */
};

/*******************************************************************************
 函 数: EKC_get_exio_hdw_cfg_info_num()
 功 能: 获取IO扩展板硬件配置信息数(从机)
 参 数: 空
 返 回: IO扩展板硬件配置信息数
 其 它: 空
*******************************************************************************/
U08 EKC_get_exio_hdw_cfg_info_num(void)
{
    return ARRELE(EKC_EXIO_HDW_CFG_INFO);
}

/* IO扩展板硬件初始化信息 */
const EXIO_HDW_INIT_INFO EKC_EXIO_HDW_INIT_INFO[] = 
{
    { EK_INIT_EEV_ST_PV, EK_RSRC_EEV, },    /* 电膨初始当前步数 */
};

/*******************************************************************************
 函 数: EKC_get_exio_hdw_init_info_num()
 功 能: 获取IO扩展板硬件初始化信息数(从机)
 参 数: 空
 返 回: IO扩展板硬件初始化信息数
 其 它: 空
*******************************************************************************/
U08 EKC_get_exio_hdw_init_info_num(void)
{
    return ARRELE(EKC_EXIO_HDW_INIT_INFO);
}

/*******************************************************************************
 函 数: EKC_slv_exio_hdw_cfg_handler()
 功 能: IO扩展板硬件配置处理(从机)
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void EKC_slv_exio_hdw_cfg_handler(void)
{
    U08 i;
    I16 val;

    /* 温度探头类型 */
    for (i = 0; i < MAX_TEMP_NUM; i++)
    {
        val = GET_EXIO_TMP_NTC_TYPE(i);
        Hdw_Temp_NTC_Type[i] = (val != 0xffff) ? val : 0;
    }

    /* 主路电子膨胀阀 */
    eev_set_motor_type(EEV_ID_1, GET_EXIO_EEV_FREQ_TYPE(EEV_ID_1));
    eev_set_step_speed(EEV_ID_1, GET_EXIO_EEV_FREQ(EEV_ID_1));
    eev_set_steps_max(EEV_ID_1, GET_EXIO_EEV_MAX_STEP(EEV_ID_1));
    

    /* 辅路电子膨胀阀 */
    eev_set_motor_type(EEV_ID_2, GET_EXIO_EEV_FREQ_TYPE(EEV_ID_2));
    eev_set_step_speed(EEV_ID_2, GET_EXIO_EEV_FREQ(EEV_ID_2));
    eev_set_steps_max(EEV_ID_2, GET_EXIO_EEV_MAX_STEP(EEV_ID_2));
}

/*******************************************************************************
 函 数: EKC_slv_exio_hdw_init_handler()
 功 能: IO扩展板硬件初始化处理(从机)
 参 数: 空
 返 回: 空
 其 它: 空
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
 函 数: EKC_slv_exio_eev_init_done()
 功 能: 判断IO扩展板电膨初始化是否完成(从机)
 参 数: 空
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
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

