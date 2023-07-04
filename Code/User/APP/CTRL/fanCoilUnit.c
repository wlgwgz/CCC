/******************************************************************************

                  版权所有 (C), 2001-2017, 广州市邦普电脑技术开发有限公司

 ******************************************************************************
  文 件 名   : fanCoilUnit.h
  版 本 号   : 初稿
  作    者   : kjh
  生成日期   : 2021年4月7日
  最近修改   :
  功能描述   : fanCoilUnit.h 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2021年4月7日
    作    者   : kjh
    修改内容   : 创建文件，参考App_EcFan

  2.日    期   : 2021-7-9
    作    者   : kjh
    修改内容   : a 风盘个数增加到16个
                 b 【末端开启主机】增加可选“不开启、仅开主机、主末全开”
                 c 修改fcu_commu(),fcu_slave_start_master(),fcu_logic()
				 d 风盘故障改为自动复位

******************************************************************************/

#define GBL_FCU     1
/*-----------------包含头文件 -----------------------------*/
#include "Includes.h"
#ifdef COM_FCU_FBUS 
/*-----------------外部变量说明----------------------------*/



/*-----------------外部函数原型说明------------------------*/



/*-----------------内部函数原型说明------------------------*/



/*-----------------全局变量--------------------------------*/



/*-----------------模块级变量------------------------------*/

#define FCU_COMMU_INTERVAL 10				    /* 通许间隔 */
U08 fcu_commu_interval_100ms[FCU_NUM_MAX];      // 通讯间隔计时

const U08 fcu_err_commu[FCU_NUM_MAX]  = {M_ERR_FCU_ERR_COMMU1,  M_ERR_FCU_ERR_COMMU2, 
                                         M_ERR_FCU_ERR_COMMU3,  M_ERR_FCU_ERR_COMMU4,
                                         M_ERR_FCU_ERR_COMMU5,  M_ERR_FCU_ERR_COMMU6,
                                         M_ERR_FCU_ERR_COMMU7,  M_ERR_FCU_ERR_COMMU8,
                                         M_ERR_FCU_ERR_COMMU9,  M_ERR_FCU_ERR_COMMU10, 
                                         M_ERR_FCU_ERR_COMMU11, M_ERR_FCU_ERR_COMMU12,
                                         M_ERR_FCU_ERR_COMMU13, M_ERR_FCU_ERR_COMMU14,
                                         M_ERR_FCU_ERR_COMMU15, M_ERR_FCU_ERR_COMMU16,
                                         M_ERR_FCU_ERR_COMMU17, M_ERR_FCU_ERR_COMMU18, 
                                         M_ERR_FCU_ERR_COMMU19, M_ERR_FCU_ERR_COMMU20,
                                         M_ERR_FCU_ERR_COMMU21, M_ERR_FCU_ERR_COMMU22,
                                         M_ERR_FCU_ERR_COMMU23, M_ERR_FCU_ERR_COMMU24,
                                         M_ERR_FCU_ERR_COMMU25, M_ERR_FCU_ERR_COMMU26, 
                                         M_ERR_FCU_ERR_COMMU27, M_ERR_FCU_ERR_COMMU28,
                                         M_ERR_FCU_ERR_COMMU29, M_ERR_FCU_ERR_COMMU30,
                                         M_ERR_FCU_ERR_COMMU31, M_ERR_FCU_ERR_COMMU32,
                                         };
const U08 fcu_err_error[FCU_NUM_MAX]  = {M_ERR_FCU_ERR_ERR1,  M_ERR_FCU_ERR_ERR2, 
                                         M_ERR_FCU_ERR_ERR3,  M_ERR_FCU_ERR_ERR4,
                                         M_ERR_FCU_ERR_ERR5,  M_ERR_FCU_ERR_ERR6,
                                         M_ERR_FCU_ERR_ERR7,  M_ERR_FCU_ERR_ERR8,
                                         M_ERR_FCU_ERR_ERR9,  M_ERR_FCU_ERR_ERR10, 
                                         M_ERR_FCU_ERR_ERR11, M_ERR_FCU_ERR_ERR12,
                                         M_ERR_FCU_ERR_ERR13, M_ERR_FCU_ERR_ERR14,
                                         M_ERR_FCU_ERR_ERR15, M_ERR_FCU_ERR_ERR16,
                                         M_ERR_FCU_ERR_ERR17, M_ERR_FCU_ERR_ERR18, 
                                         M_ERR_FCU_ERR_ERR19, M_ERR_FCU_ERR_ERR20,
                                         M_ERR_FCU_ERR_ERR21, M_ERR_FCU_ERR_ERR22,
                                         M_ERR_FCU_ERR_ERR23, M_ERR_FCU_ERR_ERR24,
                                         M_ERR_FCU_ERR_ERR25, M_ERR_FCU_ERR_ERR26, 
                                         M_ERR_FCU_ERR_ERR27, M_ERR_FCU_ERR_ERR28,
                                         M_ERR_FCU_ERR_ERR29, M_ERR_FCU_ERR_ERR30,
                                         M_ERR_FCU_ERR_ERR31, M_ERR_FCU_ERR_ERR32,
                                         };

U08 fcu_err_chk_dly[FCU_NUM_MAX];       // 故障检测延时
U08 fcu_err_commu_dly[FCU_NUM_MAX];     // 通讯故障延时
U16 fcu_drive_err[FCU_ARRAY_SIZE_MAX];
U08 fcu_s_start_m_num;                  // 末端号：末端开启主机的末端号
typedef struct FCU_CMD
{
    U08 start  : 1;     // 开机
    U08 close  : 1;     // 关机
    U08 mode_m : 1;     // 模式设置：主机发起
    U08 mode_s : 1;     // 模式设置：末端发起
    U08 b4  : 1;
    U08 b5  : 1;
    U08 b6  : 1;
    U08 b7  : 1;
    U08 b8  : 1;
    U08 b9  : 1;
    U08 b10 : 1;
    U08 b11 : 1;
    U08 b12 : 1;
    U08 b13 : 1;
    U08 b14 : 1;
    U08 b15 : 1;
}FCU_CMD_STRU;
FCU_CMD_STRU fcu_cmd[FCU_NUM_MAX];

U16 fcu_sv_mode_slv;		/* 末端发起的模式设置：用于桥接 */

/*-----------------常量定义--------------------------------*/



/*-----------------宏定义 ---------------------------------*/


/*****************************************************************************
函  数 : fcu_inused()
功  能 : 序号是否有用
参  数 : U08 i
返  回 : 无
其  他 : 无
*****************************************************************************/
BOL fcu_inused(U08 i)
{
    BOL is_inused = FALSE;

	if (i < FCU_NUM_USE)
	{
	    switch (fcu_type)
	    {                
	        default:
            case FCU_PUNP:    
	            is_inused = TRUE;
                break;
	    }
	}
    
    return is_inused;
}

/*****************************************************************************
函  数 : fcu_can_use()
功  能 : 是否可使用
参  数 : U08 i : 
返  回 : 无
其  他 : 无
*****************************************************************************/
BOL fcu_can_use(U08 i)
{
    BOL can_run = FALSE;
    
    if (fcu_inused(i)
//        && !fcu_get_error(fcu_err_error[i])
        && !fcu_get_error(fcu_err_commu[i])
        )
    {
        can_run = TRUE;
    }

    return can_run;
}

/*****************************************************************************
函  数 : fcu_rd_update()
功  能 : 读取数据更新
参  数 : 无
返  回 : 无
其  他 : 一般放log_xxx_begin中
*****************************************************************************/
void fcu_rd_update(void)
{
    U16 i;
    
    for (i=0; i<FCU_NUM_MAX; i++)
    {
        if (!fcu_inused(i))
        {
            continue;
        }
        
        switch (fcu_type)
        {
            case FCU_PUNP:
                fcu_temp_indoor(i) = fcu_rd_sta(i, FCU_ADDR_RD_TEMP);
                fcu_egy_need(i)    = fcu_rd_sta(i, FCU_ADDR_RD_EGY);
                fcu_err_code(i)    = fcu_rd_sta(i, FCU_ADDR_RD_ERROR);
#if 0
                fcu[i].err_word[0]  |= fcu_rd_sta(i, 2002);
                
                {
                    fcu_err_code(i) = SENSOR_NOEXIST;
                    CLRBIT(fcu_drive_err, i);
                    if (fcu[i].err_word[0])
                    {
                        fcu_err_code(i) = fcu[i].err_word[0];
                        SETBIT(fcu_drive_err, i);
                    }
                }
#endif
                break;
        }
    }
}

/*****************************************************************************
函  数 : fcu_wr_update()
功  能 : 写入数据更新
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
void fcu_wr_update(void)
{
    U16 i;

    for (i=0; i<FCU_NUM_MAX; i++)
    {
        if (!fcu_inused(i))
        {
            continue;
        }
        
        switch (fcu_type)
        {
            case FCU_PUNP:
                /* 开关机 */
                if (fcu_cmd[i].start)
                {
                    fcu_ctrl_wr(i, FCU_ADDR_WR_ONOFF) = TRUE;
                    fcu_ctrl_fg_wr(i, FCU_ADDR_WR_ONOFF) = TRUE;

                    fcu_cmd[i].start = 0;
                }
                else if (fcu_cmd[i].close)
                {
                    fcu_ctrl_wr(i, FCU_ADDR_WR_ONOFF) = FALSE;
                    fcu_ctrl_fg_wr(i, FCU_ADDR_WR_ONOFF) = TRUE;

                    fcu_cmd[i].close = 0;
                }

                /* 设定模式 */
                if (fcu_cmd[i].mode_m)
                {
                    switch (FCU_MASTER_MODE)
                    {
                        case MODE_COOL:
                            fcu_ctrl_wr(i, FCU_ADDR_WR_SET_MODE) = FCU_MODE_COOL;
                            fcu_ctrl_fg_wr(i, FCU_ADDR_WR_SET_MODE) = TRUE;
                            break;
                        case MODE_HEAT:
                            fcu_ctrl_wr(i, FCU_ADDR_WR_SET_MODE) = FCU_MODE_HEAT;
                            fcu_ctrl_fg_wr(i, FCU_ADDR_WR_SET_MODE) = TRUE;
                            break;
                        default:
                            /* 暂不支持其他模式 */
                            break;
                    }

                    fcu_cmd[i].mode_m = 0;
                }
                else if (fcu_cmd[i].mode_s)
                {
                    fcu_ctrl_wr(i, FCU_ADDR_WR_SET_MODE) = fcu_sv_mode_slv;
                    fcu_ctrl_fg_wr(i, FCU_ADDR_WR_SET_MODE) = TRUE;

                    fcu_cmd[i].mode_s = 0;
                }
                break;
        }
    }
}

/*****************************************************************************
函  数 : fcu_commu_rd_sta()
功  能 : 读取状态(后台)
参  数 : U08 i : 
返  回 : 无
其  他 : 无
*****************************************************************************/
BOL fcu_commu_rd_sta(U08 i)
{
    U08 addr = i + FCU_ADDR_BASE;
    BOL succ = TRUE;
    
    if (addr > 0 && addr <= FCU_ADDR_MAX)
    {
        switch (fcu_type)
        {
            case FCU_PUNP:
                /* 只读 */
                succ &= COM_ReadROwords(FCU_COM, 
                                        addr, 
                                        FCU_ADDR_RD_BASE, 
                                        FCU_ADDR_RD_NUM, 
                                        (U16 *)&fcu_rd_array[i][0], 
                                        Q_PRIO_LOW);
                break;
        }
    }

    return succ;
}

/*****************************************************************************
函  数 : fcu_commu_rd_ctrl()
功  能 : 读取控制信息(后台)
参  数 : U08 i : 
返  回 : 无
其  他 : 无
*****************************************************************************/
BOL fcu_commu_rd_ctrl(U08 i)
{
    U08 addr = i + FCU_ADDR_BASE;
    BOL succ = TRUE;
    
    if (addr > 0 && addr <= FCU_ADDR_MAX)
    {
        switch (fcu_type)
        {
            case FCU_PUNP:
                /* 读写 */
                succ &= COM_ReadRWwords(FCU_COM, 
                                            addr, 
                                            FCU_ADDR_WR_BASE, 
                                            FCU_ADDR_WR_NUM, 
                                            (U16 *)&fcu_rw_array_rd[i][0], 
                                            Q_PRIO_LOW);
                
                break;
        }
    }

    return succ;
}

/*****************************************************************************
函  数 : fcu_commu_wr_ctrl()
功  能 : 写入控制(后台)
参  数 : U08 i : 
返  回 : 无
其  他 : 无
*****************************************************************************/
BOL fcu_commu_wr_ctrl(U08 i)
{
    U16 *buf = NULL;
    U16 wr_addr;
    U16 wr_num;
    U08 addr = i + FCU_ADDR_BASE;
    BOL succ = TRUE;
    
    if (addr > 0 && addr <= FCU_ADDR_MAX)
    {
        switch (fcu_type)
        {
             case FCU_PUNP:
                if (fcu_ctrl_fg_wr(i, FCU_ADDR_WR_ONOFF))
                {
                    fcu_ctrl_fg_wr(i, FCU_ADDR_WR_ONOFF) = FALSE;
                    
                    buf = (U16 *)&fcu_ctrl_wr(i, FCU_ADDR_WR_ONOFF);
                    wr_addr = FCU_ADDR_WR_ONOFF;
                    wr_num = 1;
                }
                else if (fcu_ctrl_fg_wr(i, FCU_ADDR_WR_SET_MODE))
                {
                    fcu_ctrl_fg_wr(i, FCU_ADDR_WR_SET_MODE) = FALSE;
                    
                    buf = (U16 *)&fcu_ctrl_wr(i, FCU_ADDR_WR_SET_MODE);
                    wr_addr = FCU_ADDR_WR_SET_MODE;
                    wr_num = 1;
                }
                
                break;
        }

        if (buf != NULL)
        {
            succ &= COM_WriteWords(FCU_COM, addr, wr_addr, wr_num, buf, Q_PRIO_HIGH);
        }
    }

    return succ;
}

/*****************************************************************************
函  数 : fcu_commu_wr_reset()
功  能 : 故障复位指令(按需)
参  数 : U08 i
返  回 : 无
其  他 : 一般放复位操作中
*****************************************************************************/
BOL fcu_commu_wr_reset(U08 i)
{
    U16 wr_addr;
    U16 wr_val;
    U08 addr = i + FCU_ADDR_BASE;
    BOL succ = TRUE;
    BOL one_word = FALSE;
    
    if (addr > 0 && addr <= FCU_ADDR_MAX)
    {
        switch (fcu_type)
        {
            case FCU_PUNP:         /* 无复位命令 */		
                break;
//            case FCU_PUNP:
//                wr_addr = FCU_ADDR_WR_RST;
//                wr_val = 0x000C;
//                one_word = TRUE;
//                break;
        }

        if (one_word)
        {
            succ &= COM_WriteOneWord(FCU_COM, addr, wr_addr, wr_val, Q_PRIO_HIGH);
        }
    }

    return succ;
}

/*****************************************************************************
函  数 : fcu_commu()
功  能 : 通信
参  数 : 无
返  回 : 通信处理完成
其  他 : 无
*****************************************************************************/
BOL fcu_commu(void)
{
    static U08 i = 0;
    BOL succ = TRUE;
    BOL finish = FALSE;

    if (i < FCU_NUM_USE)
    {
        if (fcu_inused(i))
        {
            /* 实时写 */
            succ &= fcu_commu_wr_ctrl(i);
            
            /* 定时读 */
            if (fcu_commu_interval_100ms[i] >= FCU_COMMU_INTERVAL)
            {
                fcu_commu_interval_100ms[i] = 0;
                
                succ &= fcu_commu_rd_sta(i);
                succ &= fcu_commu_rd_ctrl(i);
            }
        }
    }

    if (++i >= FCU_NUM_USE)
    {
        i = 0;
        finish = TRUE;
    }

    return finish;
}

/*****************************************************************************
函  数 : fcu_temp_indoor_get()
功  能 : 获取室内温度
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
void fcu_temp_indoor_get(void)
{
    U08 i;
    
	for (i=0; i<FCU_NUM_MAX; i++)
	{
	    if (!fcu_inused(i))
        {
            continue;
        }

        if (fcu_can_use(i))
        {
            fcu[i].temp_indoor = fcu_temp_indoor(i);
        }
        else
        {
            fcu[i].temp_indoor = SENSOR_NOEXIST;
        }
	}
}

/*****************************************************************************
函  数 : fcu_slave_start_master()
功  能 : 末端开启主机
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
void fcu_slave_start_master(void)
{
    U08 i;
    static BOL bak_onoff[FCU_NUM_MAX];
    BOL fg_act = FALSE;

    for (i=0; i<FCU_NUM_USE; i++)
    {
        if (bak_onoff[i] != fcu_ctrl_rd(i, FCU_ADDR_WR_ONOFF))
        {
            bak_onoff[i] = fcu_ctrl_rd(i, FCU_ADDR_WR_ONOFF);

            if (!fg_act)
            {
                if (bak_onoff[i]==TRUE      /* 风盘由关机变到开机 */
                && (StepIsIdle||StepIsDelay)	/* 主机非开机态 */
                )
                {
                    fg_act = TRUE;
                    
                    switch (val_fcu_S_start_M)
                    {
                        case S_START_M_NONE:/* 不开启 */
                        default:
                            fcu_s_start_m_num = 0xff;
                            break;
                            
                        case S_START_M_ONLY:/* 仅开主机 */
                            FCU_CMD_RUN;
                            fcu_s_start_m_num = i;
                            break;
                            
                        case S_START_M_ALL:	/* 主末全开 */
                            FCU_CMD_RUN;
                            fcu_s_start_m_num = 0xff;
                            break;
                    }
                }
            }
        }
    }
}

/*****************************************************************************
函  数 : fcu_master_onoff_slave()
功  能 : 主机开关末端
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
void fcu_master_onoff_slave(void)
{
    U08 i;

    /* 开机 */
    if (GETBIT(fcu_semaphore, FCU_SEM_M_START_S))
    {
        CLRBIT(fcu_semaphore, FCU_SEM_M_START_S);
        if (val_fcu_M_start_S)
        {
            if (fcu_s_start_m_num < FCU_NUM_USE)
            {
                fcu_s_start_m_num = 0xff;
            }
            else
            {
                for (i=0; i<FCU_NUM_USE; i++)
                {
                    fcu_cmd[i].start = TRUE;
                }
            }
            
        }
    }

    /* 关机 */
    if (GETBIT(fcu_semaphore, FCU_SEM_M_CLOSE_S))
    {
        CLRBIT(fcu_semaphore, FCU_SEM_M_CLOSE_S);
        if (val_fcu_M_close_S)
        {
            for (i=0; i<FCU_NUM_USE; i++)
            {
                fcu_cmd[i].close = TRUE;
            }
        }
    }
}

/*****************************************************************************
函  数 : fcu_mode_set()
功  能 : 主末模式设置
参  数 : 无
返  回 : 无
其  他 : 
        1、上电后先触发m_mode_bak，致使上电时自带一次下发主机模式，然后再触发s_mode_bak，
           但被mode_m屏蔽了
*****************************************************************************/
void fcu_mode_set(void)
{
    U08 i, j;
    BOL fg_act = FALSE;

    /* 末端发起模式设置 */
    if (MODE_IS_S_MAIN || MODE_IS_LINK)
    {
        static U08 s_mode_bak[FCU_NUM_MAX];
        
        for (i=0; i<FCU_NUM_USE; i++)
        {
            if (s_mode_bak[i] != fcu_ctrl_rd(i, FCU_ADDR_WR_SET_MODE))
            {
                s_mode_bak[i] = fcu_ctrl_rd(i, FCU_ADDR_WR_SET_MODE);

                if (!fg_act
                &&  !fcu_ctrl_fg_wr(i, FCU_ADDR_WR_SET_MODE)/* 且主机不在发起模式更改中 */
                )
                {
                    /* 处理主机模式 */
                    switch (s_mode_bak[i])
                    {
                        case FCU_MODE_COOL:
                            if (FCU_MASTER_MODE != SET_COOL)
                            {
                                FCU_SET_MODE_COOL;
                                fg_act = TRUE;
                            }
                            break;
                            
                        case FCU_MODE_HEAT:
                            if (FCU_MASTER_MODE != SET_HEAT)
                            {
                                FCU_SET_MODE_HEAT;
                                fg_act = TRUE;
                            }
                            break;
                    }

                    /* 处理末端模式 */
                    if (MODE_IS_LINK && fg_act)
                    {
                        fcu_sv_mode_slv = fcu_ctrl_rd(i, FCU_ADDR_WR_SET_MODE);
                        for (j=0; j<FCU_NUM_USE; j++)
                        {
                            /* 发起的模块无需再下发 */
                            if (j != i)	
                            {
                                fcu_cmd[j].mode_s = TRUE;
                            }
                        }
                    }
                }
            }
        }
    }

    /* 主机发起模式设置 */
    if (MODE_IS_M_MAIN || MODE_IS_LINK)
    {
        static U08 m_mode_bak;
        
        if (m_mode_bak != FCU_MASTER_MODE)
        {
            m_mode_bak = FCU_MASTER_MODE;

            /* 为末端发起，非主机发起，不处理 */
            if (!fg_act)	
            {
                for (i=0; i<FCU_NUM_USE; i++)
                {
                    fcu_cmd[i].mode_m = TRUE;
                }
            }
            
        }
    }
}

/*****************************************************************************
函  数 : fcu_logic()
功  能 : 处理功能逻辑
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
void fcu_logic(void)
{
    fcu_temp_indoor_get();
    fcu_master_onoff_slave();
    fcu_slave_start_master();
    fcu_mode_set();
} 


/*****************************************************************************
函  数 : fcu_err_reset()
功  能 : 故障复位
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
void fcu_err_reset(void)
{
    U16 i;
    
    for (i=0; i<FCU_NUM_MAX; i++)
    {
        if (!fcu_inused(i))
        {
            continue;
        }

        fcu_commu_wr_reset(i);

        ARRCLR(fcu[i].err_word);
        ARRCLR(fcu[i].warn_word);       
//      s_clr_error(fcu_err_error[i]);
//      CLRBIT(fcu_drive_err, i);
    }

    
}

/*****************************************************************************
函  数 : fcu_err_check()
功  能 : 故障检测
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
void fcu_err_check(void)
{
    U16 i;
    
    for (i=0; i<FCU_NUM_MAX; i++)
    {
        /* 禁用处理 */
        if (!fcu_inused(i))
        {
			CLR_FCU_BIT(fcu_drive_err, i);
			CLR_FCU_BIT(fcu_commu_err, i);
            fcu_clr_error(fcu_err_error[i]);
            fcu_clr_error(fcu_err_commu[i]); 
        }

        /* 通讯故障 */
        if (!GET_FCU_BIT(fcu_commu_err, i))
        {   // 通信故障自动复位
            fcu_clr_error(fcu_err_commu[i]);
            fcu_err_commu_dly[i] = 0;
        }
        else if (fcu_err_commu_dly[i] > POWER_DOWN_DLY)	/* 主控板由驱动供电，用于过滤掉电延迟 */
        {   
            fcu_set_error(fcu_err_commu[i]);
            fcu_clr_error(fcu_err_error[i]);
        }

        /* 故障 */
        if (fcu_err_chk_dly[i] > 10)
        {
            fcu_err_chk_dly[i] = 0;
            if (GET_FCU_BIT(fcu_drive_err, i))
            {
                fcu_set_error(fcu_err_error[i]);
            }
            else
            {
                fcu_clr_error(fcu_err_error[i]);
            }
        }
    }
}


/*****************************************************************************
函  数 : fcu_time()
功  能 : 计时
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
void fcu_time(void)
{
    U16 i;
       
	if (power_time < FCU_INIT_TIME) return;  
			
	if (F_TIME_BASIC_100MS_BACK)			/* 100ms */
	{
        for (i=0; i<FCU_NUM_MAX; i++)
        {
            _CNT(fcu_commu_interval_100ms[i]);
        }
	}
	
	if (F_TIME_BASIC_SEC_BACK)				/* 秒 */
	{
        for (i=0; i<FCU_NUM_MAX; i++) 
        {
            _CNT(fcu_err_chk_dly[i]); 
            _CNT(fcu_err_commu_dly[i]); 
        }
	}

	if (F_TIME_BASIC_MIN_BACK)				/* 分 */
	{
        ;
	}
}

/*****************************************************************************
函  数 : fcu_init()
功  能 : 初始化
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
void fcu_init(void)
{
    fcu_s_start_m_num = 0xff;
}

/*****************************************************************************
函  数 : fcu_deal()
功  能 : 逻辑调度
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
void fcu_deal(void)
{
    fcu_err_check();
    fcu_logic();
    fcu_wr_update();
}

/*****************************************************************************
函  数 : fcu_has_egy_need()
功  能 : 是否有需求
参  数 : 无
返  回 : TRUE：有需求；FALSE：无需求
其  他 : 无
*****************************************************************************/
BOL fcu_has_egy_need(void)
{
    BOL result = FALSE;		/* 默认无需求 */
    U16 i;

    if (FCU_NUM_USE == 0)
    {
        result = TRUE;
    }
    else
    {
        for (i=0; i<FCU_NUM_USE; i++)
        {
            if (fcu_egy_need(i) == TRUE)
            {
                result = TRUE;
                break;
            }
        }
    }

    return result;
}

/*******************************************************************************
 函 数: fcu_energy_cpst()
 功 能: 计算末端能调修正值
 参 数: 空
 返 回: >0 ：低负荷修正；<0 ：高负荷修正；=0 ：无修正
 其 它: 空
*******************************************************************************/
I16 fcu_energy_cpst(void)
{
    I16 result = 0;			                /* 调整值 */
    U08 i;
    I08 need_max = FCU_NUM_USE;				/* 最大需求数 */
    I08 need_blc = (need_max+1)/2;			/* 均衡需求数 */
    I08 need_now = 0;		                /* 当前需求数 */

    if (val_fcu_egy_cpst!=0 
    &&  FCU_NUM_USE>=2                      /* 至少两个风盘才使用末端能调修正 */
    &&  STEP_IS_RUN
    )              
    {
        /* 计算当前需求数 */
        for (i=0; i<FCU_NUM_USE; i++)
        {
            if (fcu_egy_need(i) == TRUE)
            {
                need_now++;
            }
        }

        /* 计算调整值 */
        result = _DIVUROUND(val_fcu_egy_cpst*(need_blc-need_now), need_blc);/* >0:低负荷修正 */
//        result = val_fcu_egy_cpst*(need_now-need_blc)/need_blc;/* >0:高负荷修正 */
    }
    

    return result;
}
#define FCU_STA_LOST    0
#define FCU_STA_ERR     1
#define FCU_STA_IDLE    2
#define FCU_STA_NEED    3
#define FCU_STA_NONEED  4
#define FCU_STA_MAX     5	/* 新状态在之前插入 */
/*******************************************************************************
 函 数: fcu_state_get()
 功 能: 获取风盘状态
 参 数: i--; 
 返 回: 
 其 它: 空
*******************************************************************************/
U08 fcu_state_get(U08 i)
{
    U08 result = FCU_STA_MAX;
    
    if (!fcu_inused(i) || GET_FCU_BIT(fcu_commu_err, i))
    {
        result = FCU_STA_LOST;
    }
    else if (fcu_get_error(fcu_err_error[i]))
    {
        result = FCU_STA_ERR;
    }
    else if (!fcu_sta_onoff(i))
    {
        result = FCU_STA_IDLE;
    }
    else if (fcu_egy_need(i))
    {
        result = FCU_STA_NEED;
    }
    else
    {
        result = FCU_STA_NONEED;
    }

    return result;
}
#endif
