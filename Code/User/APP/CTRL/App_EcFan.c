/******************************************************************************

          ��Ȩ���� (C), 2001-2016, �����а��յ��Լ����������޹�˾

 ******************************************************************************
  �� �� ��   : App_EcFan.c
  �� �� ��   : ����
  ��    ��   : zzp
  ��������   : 2019��8��21��
  ����޸�   :
  ��������   : EC��������߼�
  �����б�   :
              ec_fan
              ec_fan_com_recv
              ec_fan_com_tras
              ec_fan_speed
  �޸���ʷ   :
  1.��    ��   : 2019��8��21��
    ��    ��   : zzp
    �޸�����   : �����ļ�

******************************************************************************/
#define GBL_ECFAN     1
/*-----------------����ͷ�ļ� -----------------------------*/
#include "Includes.h"

/*-----------------�ⲿ����˵��----------------------------*/



/*-----------------�ⲿ����ԭ��˵��------------------------*/



/*-----------------�ڲ�����ԭ��˵��------------------------*/



/*-----------------ȫ�ֱ���--------------------------------*/



/*-----------------ģ�鼶����------------------------------*/

const U08 ec_fan_err_commu[]  = {ERR_EC1_COMMU, ERR_EC2_COMMU};
const U08 ec_fan_err_error[]  = {ERR_EC1_ERR, ERR_EC2_ERR};

U08 ec_fan_commu_interval_100ms[ECFAN_MAX];     // ͨѶ�����ʱ

U08 ec_fan_err_chk_dly[ECFAN_MAX];   // ���ϼ����ʱ
//U08 ec_fan_flit_intv[ECFAN_MAX];     // �����˲���ʱ
U16 EcFan_drive_err;

/*-----------------��������--------------------------------*/



/*-----------------�궨�� ---------------------------------*/


/*****************************************************************************
��  �� : ec_fan_inused()
��  �� : EC�������Ƿ�����
��  �� : U08 ec : 
��  �� : ��
��  �� : ��
*****************************************************************************/
BOL ec_fan_inused(U08 ec)
{
    BOL is_inused = FALSE;

	if (ec < NUM_ECFAN_USE)
	{
	    switch (val_ec_fan_drive)
	    {
	        case EC_FAN_DRIVE_WK:
            case EC_FAN_DRIVE_ZSFN:	/* N0004 */
	            is_inused = TRUE;
	            break;
                
	        default:
	            is_inused = FALSE;
                break;
	    }
	}
    
    return is_inused;
}

/*****************************************************************************
��  �� : ec_fan_can_run()
��  �� : ����Ƿ������
��  �� : U08 ec : 
��  �� : ��
��  �� : ��
*****************************************************************************/
BOL ec_fan_can_run(U08 ec)
{
    BOL can_run = FALSE;
    
    if (ec_fan_inused(ec)
        && !ec_get_error(ec_fan_err_error[ec])
        && !ec_get_error(ec_fan_err_commu[ec])
        )
    {
        can_run = TRUE;
    }

    return can_run;
}



/*****************************************************************************
��  �� : ec_fan_rd_update()
��  �� : ��ȡ���ݸ���
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void ec_fan_rd_update(void)
{
    U16 ec;
    
    for (ec=0; ec<ECFAN_MAX; ec++)
    {
        if (!ec_fan_inused(ec))
        {
            continue;
        }
        
        switch (val_ec_fan_drive)
        {
            case EC_FAN_DRIVE_PUNP: // ���շ������
                /* ���ٴ˴����� */
                break;
            case EC_FAN_DRIVE_WK:
                pv_ec_fan_now(ec) = EcFan_wk_rd_sta(ec, 2001);        // rpm
//                pv_cell_ECn_current(pcb_addr, ec) = EcFan_wk_rd_sta(ec, 2007)/10;   // ������� 0.1A
//                pv_cell_ECn_temp(pcb_addr, ec) = EcFan_wk_rd_sta(ec, 2009);         // ����¶� 0.1��
                ECFan[ec].err_word[0]  |= EcFan_wk_rd_sta(ec, 2002);
                ECFan[ec].err_word[1]  |= EcFan_wk_rd_sta(ec, 2003);
                ECFan[ec].warn_word[0] = EcFan_wk_rd_sta(ec, 2004);  // ���治�ܰ���λ��,�ʲ���,������
                ECFan[ec].warn_word[1] = EcFan_wk_rd_sta(ec, 2005);
                {
                    U16 err_idx = 0xFFFF;
                    I16 err_no = SENSOR_NOEXIST;
                    CLRBIT(EcFan_drive_err, ec);
                    // ��һ�����Ϻ�
                    if ((err_idx = fun_bit_first_1(ECFan[ec].err_word, ARRELE(ECFan[ec].err_word)))
                                 != 0xFFFF)
                    {
                        err_no = err_idx+1;
                        SETBIT(EcFan_drive_err, ec);
                    } // ��һ���澯��
                    else if((err_idx = fun_bit_first_1(ECFan[ec].warn_word, ARRELE(ECFan[ec].warn_word)))
                                     != 0xFFFF)
                    {
                        err_no = err_idx+1 + 100; // �����ƫ��100
                    }

                    pv_ec_warn(ec) = err_no;
                }
                break;
            case EC_FAN_DRIVE_ZSFN:	/* N0004 */
                pv_ec_fan_now(ec) = EcFan_zsfn_rd_sta(ec, 1017);        // rpm
//                pv_cell_ECn_current(pcb_addr, ec) = EcFan_wk_rd_sta(ec, 2007)/10;   // ������� 0.1A
//                pv_cell_ECn_temp(pcb_addr, ec) = EcFan_wk_rd_sta(ec, 2009);         // ����¶� 0.1��
                ECFan[ec].err_word[0]  = EcFan_zsfn_rd_sta(ec, 1019);
//                ECFan[ec].err_word[1]  |= EcFan_wk_rd_sta(ec, 1020);
                ECFan[ec].warn_word[0] = EcFan_zsfn_rd_sta(ec, 1020);  // ���治�ܰ���λ��,�ʲ���,������
//                ECFan[ec].warn_word[1] = EcFan_wk_rd_sta(ec, 2005);
                {
                    I16 err_no = SENSOR_NOEXIST;
                    
                    CLRBIT(EcFan_drive_err, ec);
                    if (GET_BIT(ECFan[ec].err_word[0], 0))
                    {
                        SETBIT(EcFan_drive_err, ec);
                    }

                    if (ECFan[ec].warn_word[0])
                    {
                        err_no = ECFan[ec].warn_word[0];
                    }

                    pv_ec_warn(ec) = err_no;
                }
                break;
        }
    }
}

/*****************************************************************************
��  �� : ec_fan_wr_update()
��  �� : д�����ݸ���
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void ec_fan_wr_update(void)
{
    U16 ec;

    for (ec=0; ec<ECFAN_MAX; ec++)
    {
        if (!ec_fan_inused(ec))
        {
            continue;
        }
        
        switch (val_ec_fan_drive)
        {
            case EC_FAN_DRIVE_PUNP:
                /* ���ٴ˴����� */
                break;

            case EC_FAN_DRIVE_WK:
                EcFan_wk_wr_ctrl(ec, 1002) = ECFan[ec].speed > 0 ? 1 : 0;
                EcFan_wk_wr_ctrl(ec, 1003) = ECFan[ec].speed;   // RMP
                break;
            case EC_FAN_DRIVE_ZSFN:	/* N0004 */
                EcFan_zsfn_wr_ctrl(ec, 1011) = ECFan[ec].speed > 0 ? 1 : 0;
                EcFan_zsfn_wr_ctrl(ec, 1012) = ECFan[ec].speed;   // RMP
                break;
        }


        pv_ec_fan_set(ec) = ECFan[ec].speed;
//        pv_ec_fan_set(ec) = speedsv[ec];

//        #ifdef USE_SIM_ECFAN 
//        pv_cell_ECn_now_rpm(pcb_addr, ec) = ECFan[ec].speed;
//        #endif
    }
}

/*****************************************************************************
��  �� : ec_fan_commu_rd_sta()
��  �� : EC�����ȡ״̬(��̨)
��  �� : U08 ec : 
��  �� : ��
��  �� : ��
*****************************************************************************/
BOL ec_fan_commu_rd_sta(U08 ec)
{
    U16 *buf = NULL;
    U16 rd_addr;
    U16 rd_num;
    U08 ec_addr = ec + val_ec_fan_base_addr;
    BOL succ = TRUE;
    BOL is_row = TRUE;  // ֻ����
    
    if (ec_addr > 0 && ec_addr <= MAX_EC_ADDR)
    {
        switch (val_ec_fan_drive)
        {
            case EC_FAN_DRIVE_PUNP:
                /* ���ٴ˴����� */
                break;
            case EC_FAN_DRIVE_WK:
                buf = (U16 *)&EcFan_rd_array[ec][0];
                rd_addr = ECFAN_WK_ADDR_RD_STA-1;			/* +1��ַ */
                rd_num = 9;
                is_row = FALSE; // ����д��
                break;
            case EC_FAN_DRIVE_ZSFN:	/* N0004 */
                buf = (U16 *)&EcFan_rd_array[ec][0];
                rd_addr = ECFAN_ZSFN_ADDR_RD_STA;			/* +0��ַ */
                rd_num = 14;
                is_row = FALSE; // ����д��
                break;
        }

        if (buf != NULL)
        {
            if (is_row)
            {
                succ &= COM_ReadROwords(COM_EC_FAN, ec_addr, rd_addr, rd_num, buf, Q_PRIO_LOW);
            }
            else
            {
                succ &= COM_ReadRWwords(COM_EC_FAN, ec_addr, rd_addr, rd_num, buf, Q_PRIO_LOW);
            }
        }
    }

    return succ;
}

/*****************************************************************************
��  �� : ec_fan_commu_wr_ctrl()
��  �� : EC���д�����(��̨)
��  �� : U08 ec : 
��  �� : ��
��  �� : ��
*****************************************************************************/
BOL ec_fan_commu_wr_ctrl(U08 ec)
{
    U16 *buf = NULL;
    U16 wr_addr;
    U16 wr_num;
    U08 ec_addr = ec + val_ec_fan_base_addr;
    BOL succ = TRUE;
    
    if (ec_addr > 0 && ec_addr <= MAX_EC_ADDR)
    {
        switch (val_ec_fan_drive)
        {
            case EC_FAN_DRIVE_PUNP:
                /* ���ٴ˴����� */
                break;
             case EC_FAN_DRIVE_WK:
                buf = (U16 *)&EcFan_wr_array[ec][0];
                wr_addr = ECFAN_WK_ADDR_WR_CTRL-1;
                wr_num = 2;
                break;
             case EC_FAN_DRIVE_ZSFN:	/* N0004 */	
                buf = (U16 *)&EcFan_wr_array[ec][0];
                wr_addr = ECFAN_ZSFN_ADDR_WR_CTRL;
                wr_num = 2;
                break;
        }

        if (buf != NULL)
        {
            succ &= COM_WriteWords(COM_EC_FAN, ec_addr, wr_addr, wr_num, buf, Q_PRIO_LOW);
        }
    }

    return succ;
}

/*****************************************************************************
��  �� : ec_fan_commu_wr_reset()
��  �� : EC������ϸ�λָ��(����)
��  �� : U08 ec : 
��  �� : ��
��  �� : ��
*****************************************************************************/
BOL ec_fan_commu_wr_reset(U08 ec)
{
//  U16 *buf = NULL;
    U16 wr_addr;
//  U16 wr_num;
    U16 wr_val;
    U08 ec_addr = ec + val_ec_fan_base_addr;
    BOL succ = TRUE;
    BOL one_word = FALSE;
    
    if (ec_addr > 0 && ec_addr <= MAX_EC_ADDR)
    {
        switch (val_ec_fan_drive)
        {
            case EC_FAN_DRIVE_PUNP:         // ���ڴ˴�����
            case EC_FAN_DRIVE_ZSFN:			/* �޸�λ���� *//* N0004 */
                break;
            case EC_FAN_DRIVE_WK:
                wr_addr = ECFAN_WK_ADDR_WR_RST-1;
                wr_val = 0x000C;
                one_word = 1;
                break;
        }

        if (one_word)
        {
            succ &= COM_WriteOneWord(COM_EC_FAN, ec_addr, wr_addr, wr_val, Q_PRIO_HIGH);
        }
    }

    return succ;
}

/*****************************************************************************
��  �� : ec_fan_commu()
��  �� : EC���ͨ��
��  �� : ��
��  �� : ͨ�Ŵ������
��  �� : ��
*****************************************************************************/
BOL ec_fan_commu(void)
{
    static U08 ec = 0;
    BOL succ = TRUE;
    BOL finish = FALSE;

    if (ec < ECFAN_MAX)
    {
        if (ec_fan_inused(ec))
        {
            if (ec_fan_commu_interval_100ms[ec] >= EC_FAN_COMMU_INTERVAL)
            {
                ec_fan_commu_interval_100ms[ec] = 0;
                
                succ &= ec_fan_commu_rd_sta(ec);
                succ &= ec_fan_commu_wr_ctrl(ec);
            }
        }
    }

    if (++ec >= ECFAN_MAX)
    {
        ec = 0;
        finish = TRUE;
    }

    return finish;
}

/*****************************************************************************
��  �� : ec_fan_speed_get()
��  �� : ��ȡ����
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void ec_fan_speed_get(void)
{
    U08 ec;
//    U16 speed_max = val_ec_fan_max_speed;
    
	for (ec=0; ec<ECFAN_MAX; ec++)
	{
	    if (!ec_fan_inused(ec))
        {
            continue;
        }

        if (ec_fan_can_run(ec))
        {
//            ECFan[ec].speed = (U32)speed_max * speedsv[ec]/100;
            ECFan[ec].speed = speedsv[ec];
        }
        else
        {
            ECFan[ec].speed = 0;
        }
	}
}

/*****************************************************************************
��  �� : ec_fan_speed()
��  �� : ����EC�������
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void ec_fan_speed(void)
{
    ec_fan_speed_get();
} 


/*****************************************************************************
��  �� : ec_fan_err_reset()
��  �� : EC������ϸ�λ
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void ec_fan_err_reset(void)
{
    U16 ec;
    
    for (ec=0; ec<ECFAN_MAX; ec++)
    {
        if (!ec_fan_inused(ec))
        {
            continue;
        }

        ec_fan_commu_wr_reset(ec);
        if (ec_get_error(ec_fan_err_error[ec]))
        {
            ec_fan_err_chk_dly[ec] = 0;
        }
        ARRCLR(ECFan[ec].err_word);
        ARRCLR(ECFan[ec].warn_word);       
//      s_clr_error(ec_fan_err_error[ec]);
//      CLRBIT(EcFan_drive_err, ec);
    }

    
}

/*****************************************************************************
��  �� : ec_fan_err_check()
��  �� : 
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void ec_fan_err_check(void)
{
    U16 ec;
    
    for (ec=0; ec<ECFAN_MAX; ec++)
    {
        if (!ec_fan_inused(ec))
        {
            continue;
            /* 2021-12-10-����Ϻ�������������ã��ʴ˴�������������������� */
//            CLRBIT(EcFan_drive_err, ec);
//            CLRBIT(EcFan_commu_err, ec);
//            ec_clr_error(ec_fan_err_error[ec]);
//            ec_clr_error(ec_fan_err_commu[ec]); 
        }

        if (GETBIT(EcFan_commu_err, ec))
        {
            ec_set_error(ec_fan_err_commu[ec]);
            ec_clr_error(ec_fan_err_error[ec]);
        }
        else
        {   // ͨ�Ź����Զ���λ
            ec_clr_error(ec_fan_err_commu[ec]);
        }

        if (ec_fan_err_chk_dly[ec] > 10)
        {
            if (GETBIT(EcFan_drive_err, ec))
            {
                ec_set_error(ec_fan_err_error[ec]);
            }
        }
        else // 10���ڹ�������,��λ����
        {
            if (!GETBIT(EcFan_drive_err, ec))
            {
                ec_clr_error(ec_fan_err_error[ec]);
            }
        }
    }
}


/*****************************************************************************
��  �� : ec_fan_time()
��  �� : EC�����ʱ
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void ec_fan_time(void)
{
    U16 ec;
       
	if (power_time < POWER_ON_INTO_WORK) return;  
			
	if (F_TIME_BASIC_100MS_BACK)
	{
        for (ec=0; ec<ECFAN_MAX; ec++)
        {
            _CNT(ec_fan_commu_interval_100ms[ec]);
        }
	}
	
	if (F_TIME_BASIC_SEC_BACK)
	{
        for (ec=0; ec<ECFAN_MAX; ec++) 
        {
            _CNT(ec_fan_err_chk_dly[ec]); 
        }
	}

	if (F_TIME_BASIC_MIN_BACK)
	{
        ;
	}
}

/*****************************************************************************
��  �� : ec_fan_init()
��  �� : EC���ģ���ʼ��
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void ec_fan_init(void)
{
    ;
}

/*****************************************************************************
��  �� : ec_fan()
��  �� : EC�������
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void ec_fan(void)
{
    ec_fan_err_check();
    ec_fan_speed();
    ec_fan_wr_update();
}


