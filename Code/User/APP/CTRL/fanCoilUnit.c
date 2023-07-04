/******************************************************************************

                  ��Ȩ���� (C), 2001-2017, �����а��յ��Լ����������޹�˾

 ******************************************************************************
  �� �� ��   : fanCoilUnit.h
  �� �� ��   : ����
  ��    ��   : kjh
  ��������   : 2021��4��7��
  ����޸�   :
  ��������   : fanCoilUnit.h ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2021��4��7��
    ��    ��   : kjh
    �޸�����   : �����ļ����ο�App_EcFan

  2.��    ��   : 2021-7-9
    ��    ��   : kjh
    �޸�����   : a ���̸������ӵ�16��
                 b ��ĩ�˿������������ӿ�ѡ����������������������ĩȫ����
                 c �޸�fcu_commu(),fcu_slave_start_master(),fcu_logic()
				 d ���̹��ϸ�Ϊ�Զ���λ

******************************************************************************/

#define GBL_FCU     1
/*-----------------����ͷ�ļ� -----------------------------*/
#include "Includes.h"
#ifdef COM_FCU_FBUS 
/*-----------------�ⲿ����˵��----------------------------*/



/*-----------------�ⲿ����ԭ��˵��------------------------*/



/*-----------------�ڲ�����ԭ��˵��------------------------*/



/*-----------------ȫ�ֱ���--------------------------------*/



/*-----------------ģ�鼶����------------------------------*/

#define FCU_COMMU_INTERVAL 10				    /* ͨ���� */
U08 fcu_commu_interval_100ms[FCU_NUM_MAX];      // ͨѶ�����ʱ

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

U08 fcu_err_chk_dly[FCU_NUM_MAX];       // ���ϼ����ʱ
U08 fcu_err_commu_dly[FCU_NUM_MAX];     // ͨѶ������ʱ
U16 fcu_drive_err[FCU_ARRAY_SIZE_MAX];
U08 fcu_s_start_m_num;                  // ĩ�˺ţ�ĩ�˿���������ĩ�˺�
typedef struct FCU_CMD
{
    U08 start  : 1;     // ����
    U08 close  : 1;     // �ػ�
    U08 mode_m : 1;     // ģʽ���ã���������
    U08 mode_s : 1;     // ģʽ���ã�ĩ�˷���
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

U16 fcu_sv_mode_slv;		/* ĩ�˷����ģʽ���ã������Ž� */

/*-----------------��������--------------------------------*/



/*-----------------�궨�� ---------------------------------*/


/*****************************************************************************
��  �� : fcu_inused()
��  �� : ����Ƿ�����
��  �� : U08 i
��  �� : ��
��  �� : ��
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
��  �� : fcu_can_use()
��  �� : �Ƿ��ʹ��
��  �� : U08 i : 
��  �� : ��
��  �� : ��
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
��  �� : fcu_rd_update()
��  �� : ��ȡ���ݸ���
��  �� : ��
��  �� : ��
��  �� : һ���log_xxx_begin��
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
��  �� : fcu_wr_update()
��  �� : д�����ݸ���
��  �� : ��
��  �� : ��
��  �� : ��
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
                /* ���ػ� */
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

                /* �趨ģʽ */
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
                            /* �ݲ�֧������ģʽ */
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
��  �� : fcu_commu_rd_sta()
��  �� : ��ȡ״̬(��̨)
��  �� : U08 i : 
��  �� : ��
��  �� : ��
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
                /* ֻ�� */
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
��  �� : fcu_commu_rd_ctrl()
��  �� : ��ȡ������Ϣ(��̨)
��  �� : U08 i : 
��  �� : ��
��  �� : ��
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
                /* ��д */
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
��  �� : fcu_commu_wr_ctrl()
��  �� : д�����(��̨)
��  �� : U08 i : 
��  �� : ��
��  �� : ��
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
��  �� : fcu_commu_wr_reset()
��  �� : ���ϸ�λָ��(����)
��  �� : U08 i
��  �� : ��
��  �� : һ��Ÿ�λ������
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
            case FCU_PUNP:         /* �޸�λ���� */		
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
��  �� : fcu_commu()
��  �� : ͨ��
��  �� : ��
��  �� : ͨ�Ŵ������
��  �� : ��
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
            /* ʵʱд */
            succ &= fcu_commu_wr_ctrl(i);
            
            /* ��ʱ�� */
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
��  �� : fcu_temp_indoor_get()
��  �� : ��ȡ�����¶�
��  �� : ��
��  �� : ��
��  �� : ��
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
��  �� : fcu_slave_start_master()
��  �� : ĩ�˿�������
��  �� : ��
��  �� : ��
��  �� : ��
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
                if (bak_onoff[i]==TRUE      /* �����ɹػ��䵽���� */
                && (StepIsIdle||StepIsDelay)	/* �����ǿ���̬ */
                )
                {
                    fg_act = TRUE;
                    
                    switch (val_fcu_S_start_M)
                    {
                        case S_START_M_NONE:/* ������ */
                        default:
                            fcu_s_start_m_num = 0xff;
                            break;
                            
                        case S_START_M_ONLY:/* �������� */
                            FCU_CMD_RUN;
                            fcu_s_start_m_num = i;
                            break;
                            
                        case S_START_M_ALL:	/* ��ĩȫ�� */
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
��  �� : fcu_master_onoff_slave()
��  �� : ��������ĩ��
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void fcu_master_onoff_slave(void)
{
    U08 i;

    /* ���� */
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

    /* �ػ� */
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
��  �� : fcu_mode_set()
��  �� : ��ĩģʽ����
��  �� : ��
��  �� : ��
��  �� : 
        1���ϵ���ȴ���m_mode_bak����ʹ�ϵ�ʱ�Դ�һ���·�����ģʽ��Ȼ���ٴ���s_mode_bak��
           ����mode_m������
*****************************************************************************/
void fcu_mode_set(void)
{
    U08 i, j;
    BOL fg_act = FALSE;

    /* ĩ�˷���ģʽ���� */
    if (MODE_IS_S_MAIN || MODE_IS_LINK)
    {
        static U08 s_mode_bak[FCU_NUM_MAX];
        
        for (i=0; i<FCU_NUM_USE; i++)
        {
            if (s_mode_bak[i] != fcu_ctrl_rd(i, FCU_ADDR_WR_SET_MODE))
            {
                s_mode_bak[i] = fcu_ctrl_rd(i, FCU_ADDR_WR_SET_MODE);

                if (!fg_act
                &&  !fcu_ctrl_fg_wr(i, FCU_ADDR_WR_SET_MODE)/* ���������ڷ���ģʽ������ */
                )
                {
                    /* ��������ģʽ */
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

                    /* ����ĩ��ģʽ */
                    if (MODE_IS_LINK && fg_act)
                    {
                        fcu_sv_mode_slv = fcu_ctrl_rd(i, FCU_ADDR_WR_SET_MODE);
                        for (j=0; j<FCU_NUM_USE; j++)
                        {
                            /* �����ģ���������·� */
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

    /* ��������ģʽ���� */
    if (MODE_IS_M_MAIN || MODE_IS_LINK)
    {
        static U08 m_mode_bak;
        
        if (m_mode_bak != FCU_MASTER_MODE)
        {
            m_mode_bak = FCU_MASTER_MODE;

            /* Ϊĩ�˷��𣬷��������𣬲����� */
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
��  �� : fcu_logic()
��  �� : �������߼�
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void fcu_logic(void)
{
    fcu_temp_indoor_get();
    fcu_master_onoff_slave();
    fcu_slave_start_master();
    fcu_mode_set();
} 


/*****************************************************************************
��  �� : fcu_err_reset()
��  �� : ���ϸ�λ
��  �� : ��
��  �� : ��
��  �� : ��
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
��  �� : fcu_err_check()
��  �� : ���ϼ��
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void fcu_err_check(void)
{
    U16 i;
    
    for (i=0; i<FCU_NUM_MAX; i++)
    {
        /* ���ô��� */
        if (!fcu_inused(i))
        {
			CLR_FCU_BIT(fcu_drive_err, i);
			CLR_FCU_BIT(fcu_commu_err, i);
            fcu_clr_error(fcu_err_error[i]);
            fcu_clr_error(fcu_err_commu[i]); 
        }

        /* ͨѶ���� */
        if (!GET_FCU_BIT(fcu_commu_err, i))
        {   // ͨ�Ź����Զ���λ
            fcu_clr_error(fcu_err_commu[i]);
            fcu_err_commu_dly[i] = 0;
        }
        else if (fcu_err_commu_dly[i] > POWER_DOWN_DLY)	/* ���ذ����������磬���ڹ��˵����ӳ� */
        {   
            fcu_set_error(fcu_err_commu[i]);
            fcu_clr_error(fcu_err_error[i]);
        }

        /* ���� */
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
��  �� : fcu_time()
��  �� : ��ʱ
��  �� : ��
��  �� : ��
��  �� : ��
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
	
	if (F_TIME_BASIC_SEC_BACK)				/* �� */
	{
        for (i=0; i<FCU_NUM_MAX; i++) 
        {
            _CNT(fcu_err_chk_dly[i]); 
            _CNT(fcu_err_commu_dly[i]); 
        }
	}

	if (F_TIME_BASIC_MIN_BACK)				/* �� */
	{
        ;
	}
}

/*****************************************************************************
��  �� : fcu_init()
��  �� : ��ʼ��
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void fcu_init(void)
{
    fcu_s_start_m_num = 0xff;
}

/*****************************************************************************
��  �� : fcu_deal()
��  �� : �߼�����
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void fcu_deal(void)
{
    fcu_err_check();
    fcu_logic();
    fcu_wr_update();
}

/*****************************************************************************
��  �� : fcu_has_egy_need()
��  �� : �Ƿ�������
��  �� : ��
��  �� : TRUE��������FALSE��������
��  �� : ��
*****************************************************************************/
BOL fcu_has_egy_need(void)
{
    BOL result = FALSE;		/* Ĭ�������� */
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
 �� ��: fcu_energy_cpst()
 �� ��: ����ĩ���ܵ�����ֵ
 �� ��: ��
 �� ��: >0 ���͸���������<0 ���߸���������=0 ��������
 �� ��: ��
*******************************************************************************/
I16 fcu_energy_cpst(void)
{
    I16 result = 0;			                /* ����ֵ */
    U08 i;
    I08 need_max = FCU_NUM_USE;				/* ��������� */
    I08 need_blc = (need_max+1)/2;			/* ���������� */
    I08 need_now = 0;		                /* ��ǰ������ */

    if (val_fcu_egy_cpst!=0 
    &&  FCU_NUM_USE>=2                      /* �����������̲�ʹ��ĩ���ܵ����� */
    &&  STEP_IS_RUN
    )              
    {
        /* ���㵱ǰ������ */
        for (i=0; i<FCU_NUM_USE; i++)
        {
            if (fcu_egy_need(i) == TRUE)
            {
                need_now++;
            }
        }

        /* �������ֵ */
        result = _DIVUROUND(val_fcu_egy_cpst*(need_blc-need_now), need_blc);/* >0:�͸������� */
//        result = val_fcu_egy_cpst*(need_now-need_blc)/need_blc;/* >0:�߸������� */
    }
    

    return result;
}
#define FCU_STA_LOST    0
#define FCU_STA_ERR     1
#define FCU_STA_IDLE    2
#define FCU_STA_NEED    3
#define FCU_STA_NONEED  4
#define FCU_STA_MAX     5	/* ��״̬��֮ǰ���� */
/*******************************************************************************
 �� ��: fcu_state_get()
 �� ��: ��ȡ����״̬
 �� ��: i--; 
 �� ��: 
 �� ��: ��
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
