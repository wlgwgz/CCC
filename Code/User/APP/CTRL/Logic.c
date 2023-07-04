/******************************************************************************

          ��Ȩ���� (C), 2001-2016, �����а��յ��Լ����������޹�˾

 ******************************************************************************
  �� �� ��   : Logic.c
  �� �� ��   : ����
  ��    ��   : ��־��
  ��������   : 2016��11��14������һ
  ����޸�   :
  ��������   : �����߼�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��11��14������һ
    ��    ��   : ��־��
    �޸�����   : �����ļ�

******************************************************************************/
#define _LOGIC_GLOBAL_
#include  "Includes.h"


I16 Dbgdebug_xx;

//U08 hl009a_ver[MAX_VER] = APPFUNCODEVER;  //NOTE-CXW �Ƿ��ڴ˴����Ƿ�cloud.c

/*-----------------����ͷ�ļ� -----------------------------*/



/*-----------------�ⲿ����˵��----------------------------*/



/*-----------------�ⲿ����ԭ��˵��------------------------*/



/*-----------------�ڲ�����ԭ��˵��------------------------*/



/*-----------------ȫ�ֱ���--------------------------------*/



/*-----------------ģ�鼶����------------------------------*/



/*-----------------��������--------------------------------*/



/*-----------------�궨�� ---------------------------------*/
/*****************************************************************************
��  �� : USER_1ms_Deal()
��  �� : �û�1ms��ʱ����
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void USER_1ms_Deal(void)
{
    U08 u;
    static U16 cnt_1ms = 0;

    static U08 cnt5ms = 0;
    static U08 cnt100ms = 0;
    static U08 cnt500ms = 0;
    static U08 cnt1s = 0;
    static U08 cnt1m = 0;

    if (++cnt_1ms >= 1000)  // ��1S
    {
        cnt_1ms = 0;

        if (F_TIME_SWITCH_1S == 0)  F_TIME_SWITCH_1S = 1;   // F_TIME_SWITCH_1S��0��1Ϊ����1�룬0��1��0 ��2��
        else                        F_TIME_SWITCH_1S = 0;

        COM_ProtectTimer();
        
        if (++hmi_power_time >= 100) hmi_power_time = 0;
    }

    for (u = 0; u < MAX_UART; u++)
    {
//        if (fg_baudrate_confirm[u])
//        {
            switch (com_baudrate[u])
            {
                case B_2400:
                    if (cnt_1ms % 2 == 1)
                    {
                        COM_2MsTimer(u);       
                    }
                    if (cnt_1ms % 5 == 1)
                    {
                        COM_VariableTimer(u);
                    }
                    break;
                default:
                    if (cnt_1ms % 2 == 1)
                    {
                        COM_2MsTimer(u);
                        COM_VariableTimer(u);
                    }
                    break;
            }
//        }
//        else                /* �����ʲ�ȷ�� */
//        {
//            if (cnt_1ms % 5 == 1)
//            {
//                COM_2MsTimer(u);
//                COM_VariableTimer(u);
//            }
//        }    
    }

    
//  if (cnt_1ms % 2 == 1)
//  {
//      COM_2MsTimer();
//  }

//  if (cnt_1ms % 5 == 1)
//  {
//        COM_5MsTimer();
//  }

    /* ���²��� LY520A ��ʱ�Ӵ���д�� */
    if (++cnt5ms >= 5) /* 5ms */
    {       
        cnt5ms = 0;
        SysMsCount[C_5MS]++;
    }

    if (++cnt100ms >= 100) /* 100ms */
    {   
        cnt100ms = 0;
        F_TIME_BASIC_100MS = 1;
        SysMsCount[C_100MS]++;
        sys_status |= 0x01<<Is100Ms;

        if (++cnt500ms >= 5) /* 500ms */
        {   
            cnt500ms = 0;
            SysMsCount[C_500MS]++;
            F_TIME_BASIC_500MS = 1;

            flash_500ms = !flash_500ms;
        }

        if ( ++cnt1s >= 10 ) /* 1s */
        {       
            cnt1s = 0;
            SysMsCount[C_1S]++;
            sys_status |= 0x01<<IsSecond;
            F_TIME_BASIC_SEC = 1;

            flash_1s = !flash_1s;

            if (power_time != 0xffff) power_time++;

            if( ++cnt1m >= 60 ) /* 1min */
            {   
                cnt1m = 0;
                SysMsCount[C_1M]++;
                sys_status |= 0x01<<IsMinute;
                F_TIME_BASIC_MIN = 1;
            }
        }
    }
}

/*****************************************************************************
��  �� : USER_Init()
��  �� : �û���ʼ��
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
#define     HWIO_COLLECT_DLY        (500)
void USER_Init(void)
{
    /* �ϵ����ݲɼ�ȷ����ʱ */
    while (HDW_1ms_Tick() < HWIO_COLLECT_DLY)
    {
        ;
    }   

    /* ��ʼ�� */
    PunpGetSwitch(&gIO.jmp[0], MAX_SW_BIT);
    PunpGetSwPl(&gIO.SwPlate, MAX_SWPL_BIT);

    /* SW1: �������ȵ�λ�� */
    IBH_cfg_type = read_IBH_config();

    /* SW2: �����ͺ� */
#ifdef USE_SW_MAC
    sw_mac_model_err = FALSE;
    sw_mac_model = read_mac_model();
    /* �ж϶�ȡ���Ļ����ͺ��Ƿ񳬽磬�������á�δ���á����Ͳ������и��� */
    if (sw_mac_model > MAC_MAX
    ||  sw_mac_model == MAC_MAX
    ||  sw_mac_model < MAC_NONE
    )
    {
        sw_mac_model = MAC_NONE;
        sw_mac_model_err = TRUE;
    }
#endif

    /* ������ַ */
    FBUS_ADDR = read_address();
    FBUS_ADDR %= MAX_PCB;

    /* ѹ������ */
    val_comp_num=1; //read_comp_num();

    if (FBUS_ADDR != 0) 
    {
        pcb_type = TYPE_SLAVER;
    }
    else
    {
        pcb_type = TYPE_MASTER;
    }
    
    pcb_addr = FBUS_ADDR;

    pmac = &cell[MAX_PCB];

//    ARRCLR(fg_commu_connect);

    /* Ӧ���߼� */
    User_main_init();

    COM_Init();

    /* �汾�� */
    STRCPY(&idu_mac_ver[FBUS_ADDR][0], APPFUNCODEVER);
    if (pcb_addr != 0)
    {
        /* ����ģ����Ҫ���汾�Ÿ�ֵ����ģ��汾�ŵĴ��λ�ã��Ա��ģ���ϵ�4Gģ�����������ȡ */
        STRCPY(&idu_mac_ver[0][0], APPFUNCODEVER);
    }

    /* ����������(δʹ���¿���ʱ��Ч) */
    if (val_auto_run == USED && !USE_UIRT_CTRL)
    {
//        key_run = TRUE;
        if (USE_DUAL_ZONE_CTRL)
        {
            fg_zone_A_on = TRUE;
            fg_zone_B_on = TRUE;
        }
        else
        {
            fg_zone_A_on = TRUE;
        }

        if (val_hotw_used)
        {
            fg_hotw_on = TRUE;
        }
    }
    /* �������(δʹ���¿���ʱ��Ч) */
    if (val_remember_run == USED && !USE_UIRT_CTRL)
    {
//        if(auto_run_bit == ACT_RUN)
//        {
//            key_run = TRUE;
//        }
        /* A�� */
        if (GETBIT(auto_run_bit, ACT_ZONE_A))
        {
            fg_zone_A_on = TRUE;
        }

        /* B�� */
        if (GETBIT(auto_run_bit, ACT_ZONE_B))
        {
            fg_zone_B_on = TRUE;
        }

        /* ��ˮ */
        if (GETBIT(auto_run_bit, ACT_HOTW))
        {
            fg_hotw_on = TRUE;
        }
    }
    
    controller_is_punp = TRUE;

    SH_init();
    TYB_init();
    EK_mst_init();
}

/*****************************************************************************
��  �� : USER_GetTime()
��  �� : ʱ�Ӻ���
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
U16 USER_GetTime(U08 num)
{
    U16 Count;

    DISABLE_IRQ();
        Count = SysMsCount[num];
    ENABLE_IRQ();

    return  Count;
}

/*****************************************************************************
��  �� : USER_time_basic_set()
��  �� : ��ʱ���׼
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void USER_time_basic_set(void)
{
    if (F_TIME_BASIC_MIN) 
    {
        F_TIME_BASIC_MIN = 0;
        F_TIME_BASIC_MIN_BACK = 1;
    }
    if (F_TIME_BASIC_SEC) 
    {
        F_TIME_BASIC_SEC = 0;
        F_TIME_BASIC_SEC_BACK = 1;
    }
    if (F_TIME_BASIC_500MS) 
    {
        F_TIME_BASIC_500MS = 0;
        F_TIME_BASIC_500MS_BACK = 1;
    }
    if (F_TIME_BASIC_100MS) 
    {
        F_TIME_BASIC_100MS = 0;
        F_TIME_BASIC_100MS_BACK = 1;
    }
}

/*****************************************************************************
��  �� : USER_time_basic_reset()
��  �� : ��ʱ���׼
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void USER_time_basic_reset(void)
{
    if (F_TIME_BASIC_MIN_BACK)   F_TIME_BASIC_MIN_BACK   = 0;
    if (F_TIME_BASIC_SEC_BACK)   F_TIME_BASIC_SEC_BACK   = 0;
    if (F_TIME_BASIC_500MS_BACK) F_TIME_BASIC_500MS_BACK = 0;
    if (F_TIME_BASIC_100MS_BACK) F_TIME_BASIC_100MS_BACK = 0;
        
//  F_TIME_SWITCH_1S_BAK=F_TIME_SWITCH_1S;          // 1���л���־������ͬ��     
}

/****************************************************************
������work_mac_rd_sta
���ܣ�����Ƶ��
�������豸
���أ���
������
*****************************************************************/
static U16 fg_100ms_invt_rd_sta;
void USER_invt_rd_sta(INVT_ID_ENUM id, U08 com)
{
    static U08 cnt1s = 0;
    static U16 cnt10s = 0;
    
    if (fg_100ms_invt_rd_sta
    &&  COM_QGetNodeCnt(com) < MAX_L_NODE_NUM
    )
    {
        fg_100ms_invt_rd_sta = 0;			/* 2022-9-29 �ñ�־��һ��100ms����һ�Σ����¿�λʱ����ʵ��ͨѶ����й� */
        
        if (++cnt1s >= 10)      cnt1s = 0;
        if (++cnt10s >= 100)    cnt10s = 0;

        /* 2022-9-29 ��ʱ���� �̼�ʱ����ÿ�ζ�ȡһ�α�Ƶ��״̬ */
        if (TF_OPEN_COMP)
        {
            com_invt_rd(com, id, invt_states_get_addr(), invt_states_get_size(), Q_PRIO_LOW);
        }

        /* һ����λ1�� */
        switch (cnt1s)
        {
            case 0  :   
                com_invt_rd(com, id, INVT_ADDR_AD, INVT_AD_SIZE, Q_PRIO_LOW);
                break;
            case 1  :  
                com_invt_rd(com, id, invt_states_get_addr(), invt_states_get_size(), Q_PRIO_LOW);
                break;      
            case 2  : 
                com_invt_rd(com, id, INVT_ADDR_CUR_WAVE, INVT_CUR_WAVE_SIZE, Q_PRIO_LOW);
                break;
            case 3  : 
                if (info_invt_para[id].fg_rd)
                {
                    com_invt_rd(com, id, info_invt_para[id].addr, 1, Q_PRIO_LOW);
                }
                break;   

            case 4  : 
                com_invt_rd(com, id, invt_paras_get_addr(), invt_paras_get_size(), Q_PRIO_LOW);
                break;    

            /* �����չ��9������10����λ���޸���ش��� */
            default :   
                break;
        }

        /* һ����λ10�� */
        switch (cnt10s)
        {
            case 0  :  
//                com_invt_rd(com, id, invt_paras_get_addr(), invt_paras_get_size(), Q_PRIO_LOW);
                break;
            case 1  :  
                com_invt_rd(com, id, invt_paras_fa_get_addr(), FA_NUM1, Q_PRIO_LOW);
                break; 
            case 2  :  
                com_invt_rd(com, id, FA_ADDR(FA_NUM1), FA_MAX-FA_NUM1, Q_PRIO_LOW);
                break; 
            case 3  : 
                com_invt_rd(com, id, invt_paras_fb_get_addr(), invt_paras_fb_get_size(), Q_PRIO_LOW);
                break;
            case 4  : 
                com_invt_rd(com, id, invt_paras_f10_get_addr(), invt_paras_f10_get_size(), Q_PRIO_LOW);
                break;
            case 5  :    
                com_invt_rd(com, id, INVT_ADDR_EC_FAN_FG, INVT_EC_FAN_FG_SIZE, Q_PRIO_LOW);
                break;
            case 6  :  
                com_invt_rd(com, id, invt_paras_f9_get_addr(), invt_paras_f9_get_size(), Q_PRIO_LOW);
                break;
            case 7  :
                com_invt_rd(com, id, invt_paras_f6_get_addr(), invt_paras_f6_get_size(), Q_PRIO_LOW);
                break;
            case 8  :  
                com_invt_rd(com, id, invt_paras_fp_get_addr(), invt_paras_fp_get_size(), Q_PRIO_LOW);
                break; 
            /* �����չ��99������100����λ���޸���ش��� */
            default :   
                break;
        }
    }
}

/****************************************************************
������cmd_in_inverter 
���ܣ��ж��Ƿ��跢�������Ƶ��������ͨѶ��㣨�����б仯��������5��ŷ�����
��������Ƶ�����
���أ���
��������������������ı�����ֵ��������ͨѶ��㡣
*****************************************************************/
void USER_invt_wr_cmd(INVT_ID_ENUM id, U08 com)
{
    U16 fg_invt_cmd_chg=FALSE, fg_invt_para_chg=FALSE;
    U16 fg_invt_para_fa_chg=FALSE, fg_invt_para_fb_chg=FALSE, fg_invt_para_f10_chg=FALSE;
    U16 fg_invt_para_f9_chg=FALSE, fg_invt_para_f6_chg=FALSE;
    U16 addr;

    // �����Ƶ��������д��
    {
        fg_invt_para_chg = invt_para_isChanged(id); //�жϻ�������
        fg_invt_para_fa_chg = invt_para_fa_isChanged(id);//�ж�fa����
        fg_invt_para_fb_chg = invt_para_fb_isChanged(id);//�ж�fb����
        fg_invt_para_f10_chg = invt_para_f10_isChanged(id);//�ж�f10����
        fg_invt_para_f9_chg = invt_para_f9_isChanged(id);//�ж�f9����
        fg_invt_para_f6_chg = invt_para_f6_isChanged(id);//�ж�f6����

        if (fg_invt_para_chg!=FALSE)
        {
            addr = invt_paras_get_addr()+fg_invt_para_chg-1;   

            if (com_invt_wr(com, id, addr , 1, Q_PRIO_HIGH))
            {
                invt_paras_backup(id, fg_invt_para_chg-1);
            }
        }
        else if (fg_invt_para_fa_chg!=FALSE)
        {
            addr = invt_paras_fa_get_addr()+fg_invt_para_fa_chg-1;   

            if (com_invt_wr(com, id, addr , 1, Q_PRIO_HIGH))
            {
                invt_paras_fa_backup(id, fg_invt_para_fa_chg-1);
            }
        }
        else if (fg_invt_para_fb_chg!=FALSE)
        {
            addr = invt_paras_fb_get_addr()+fg_invt_para_fb_chg-1;   

            if (com_invt_wr(com, id, addr , 1, Q_PRIO_HIGH))
            {
                invt_paras_fb_backup(id, fg_invt_para_fb_chg-1);
            }
        }
        else if (fg_invt_para_f10_chg!=FALSE)
        {
            addr = invt_paras_f10_get_addr()+fg_invt_para_f10_chg-1;   

            if (com_invt_wr(com, id, addr , 1, Q_PRIO_HIGH))
            {
                invt_paras_f10_backup(id, fg_invt_para_f10_chg-1);
            }
        }
        else if (fg_invt_para_f9_chg!=FALSE)
        {
            addr = invt_paras_f9_get_addr()+fg_invt_para_f9_chg-1;   

            if (com_invt_wr(com, id, addr , 1, Q_PRIO_HIGH))
            {
                invt_paras_f9_backup(id, fg_invt_para_f9_chg-1);
            }
        }
        else if (fg_invt_para_f6_chg!=FALSE)
        {
            addr = invt_paras_f6_get_addr()+fg_invt_para_f6_chg-1;
            
            if (com_invt_wr(com, id, addr , 1, Q_PRIO_HIGH))
            {
                invt_paras_f6_backup(id, fg_invt_para_f6_chg-1);
            }
        }
    }
    
    // �����Ƶ������(�ȴ�����������ٴ��������Ϊ����������漰���������)
    if (!fg_invt_para_chg
    &&  !fg_invt_para_fa_chg
    &&  !fg_invt_para_fb_chg
    &&  !fg_invt_para_f10_chg
    &&  !fg_invt_para_f9_chg
    &&  !fg_invt_para_f6_chg
    )
    {
        fg_invt_cmd_chg = invt_cmd_isChanged(id);

        if (fg_invt_cmd_chg!=FALSE)
        {
            addr = invt_cmd_get_addr()+fg_invt_cmd_chg-1;   

            if (com_invt_wr(com, id, addr , 1, Q_PRIO_HIGH))
            {
                invt_cmd_backup(id, fg_invt_cmd_chg-1);

            }
        }
        else if (info_invt_para[id].fg_wr)
        {
            com_invt_wr(com, id, info_invt_para[id].addr , 1, Q_PRIO_HIGH);
        }
    }
    
    
}

/*******************************************************************************
 �� ��: get_scan_period()
 �� ��: ��ȡɨ������
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void get_scan_period(void)
{
    static BOL fg_first = TRUE;
    static U32 s_tick;

    /* ����ϵͳɨ������ */
    if (fg_first)
    {
        fg_first = FALSE;
        MinScanPeriod = 0xffff;
        MaxScanPeriod = 0;
        s_tick = SYS_GetTick();
    }
    else
    {
        CurScanPeriod = (U16)(SYS_GetTick() - s_tick);
        s_tick = SYS_GetTick();
        if (CurScanPeriod < MinScanPeriod)
        {
            MinScanPeriod = CurScanPeriod;
        }
        if (CurScanPeriod > MaxScanPeriod)
        {
            MaxScanPeriod = CurScanPeriod;
        }
    }

}

#if 0
/*****************************************************************************
��  �� : USER_COMMU_invt()
��  �� : ͨѶ�����
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
//�ϵ���ȡEX�İ汾��Ϣʱ��
#define READ_VERSION_TIME_BGN 4  
#define READ_VERSION_TIME_END 6

BOOL USER_COMMU_invt(U08 com)
{
    BOOL fg_node_inserted = FALSE;

    if (F_TIME_BASIC_100MS_BACK)
    {
        _CNT(fg_100ms_invt_rd_sta);
    }
    
    if ( TestProgStaus != TPS_NO_TEST_CMD
//      || power_time < INIT_TIME 
      )
    {
        return TRUE;//��������ʱ��������
    }


    /* ��Ƶ������Ƶ��� */
    if (COM_QGetNodeCnt(com) <= 2)
    {
        enum 
        {
            FBUS_STEP_INVT, 
            FBUS_STEP_EX_EC
        };
        static U08 standy_step;

        switch (standy_step)
        {
            default:
            case FBUS_STEP_INVT:/* N0004 */
                {
                    static INVT_ID_ENUM id;
                    if ((id==INVT_ID_1 && val_inverter_used)
                    ||  (id==INVT_FAN1 && val_freq_fan_set==FREQ_FAN_SET_EC)
                    ||  (id==INVT_FAN2 && val_freq_fan_set==FREQ_FAN_SET_EC && val_freq_fan_num>1)
                    )
                    {
                        USER_invt_rd_sta(id, com);
                        USER_invt_wr_cmd(id, com);
                    }
                    if (++id >= INVT_ID_MAX) 
                    {
                        id = (INVT_ID_ENUM)0;
                        standy_step = FBUS_STEP_EX_EC;
                    }
                }
                break;
            case FBUS_STEP_EX_EC:
                if (USE_EX_EC_FAN)
                {
                    if (ec_fan_commu())
                    {
                        standy_step = FBUS_STEP_INVT;
                    }
                }
                else
                {
                    standy_step = FBUS_STEP_INVT;
                }
                break;
        } 
        fg_node_inserted = TRUE;
    }

    return fg_node_inserted;
}
#endif

extern U08 ec_fan_commu_interval_100ms[ECFAN_MAX];
//U08 cnt_intv_idu_odu_commu;

void USER_Commu_ODU_INVT(U08 com)
{
    static U08 commu_id = 0;
    
    if (F_TIME_BASIC_100MS_BACK)
    {
        _CNT(fg_100ms_invt_rd_sta);
    }

//    if (F_TIME_BASIC_SEC_BACK)
//    {
//        _CNT_WHEN(cnt_intv_idu_odu_commu, (commu_id==10));
//    }

    if ( TestProgStaus != TPS_NO_TEST_CMD
//      || power_time < INIT_TIME 
      )
    {
        return;//��������ʱ��������
    }
   
    switch (commu_id)
    {
        case 0 :    
            if (power_time >= 5)
            {
                commu_id = 10;
            }
            break;
            
        case 10:            /* ͨѶ�ڶ̽ӷ��� */
            /* ���ñ�Ƶ�� */
            if (!USE_INVT)
            {
                commu_id = 11;
                break;
            }

//            /* ����������ӣ���������Ƶ�� */
//            if ((!GET_INVT_CONNECT && GET_INOUT_CONNECT)
//            && (cnt_intv_idu_odu_commu > 2)
//            )
//            {
//                commu_id = 11;
//                cnt_intv_idu_odu_commu = 0;
//                break;
//            }

            /* ��Ƶ������Ƶ��� */
//            if (!GET_INVT_CONNECT || COM_QGetNodeCnt(com) <= 2)
            if (COM_QGetNodeCnt(com) <= 2)
            {
                enum 
                {
                    FBUS_STEP_INVT,
                    FBUS_STEP_INVT_FAN1,
                    FBUS_STEP_INVT_FAN2, 
                    FBUS_STEP_EX_EC1,
                    FBUS_STEP_EX_EC2,
                };
                
                static INVT_ID_ENUM invt_id;
                static ECFAN_ENUM ecfan_id;
                static U08 standy_step = FBUS_STEP_INVT;
                
                /* ѡ��ǰ���������豸��� */
                switch (standy_step)
                {
                    default:
                    case FBUS_STEP_INVT:       invt_id = INVT_ID_1;  break;
                    case FBUS_STEP_INVT_FAN1:  invt_id = INVT_FAN1;  break;
                    case FBUS_STEP_INVT_FAN2:  invt_id = INVT_FAN2;  break;
                    case FBUS_STEP_EX_EC1:     ecfan_id = ECFAN_1;   break;
                    case FBUS_STEP_EX_EC2:     ecfan_id = ECFAN_2;   break;
                }
    
                /* �ڵ������� */
                switch (standy_step)
                {
                    case FBUS_STEP_INVT:
                    case FBUS_STEP_INVT_FAN1:
                    case FBUS_STEP_INVT_FAN2:
                        USER_invt_rd_sta(invt_id, com);
                        USER_invt_wr_cmd(invt_id, com);
                        break;
                    case FBUS_STEP_EX_EC1:
                    case FBUS_STEP_EX_EC2:
                        if (ec_fan_commu_interval_100ms[ecfan_id] >= EC_FAN_COMMU_INTERVAL)
                        {
                            ec_fan_commu_interval_100ms[ecfan_id] = 0;
                            
                            ec_fan_commu_rd_sta(ecfan_id);
                            ec_fan_commu_wr_ctrl(ecfan_id);
                        }
                        break;
                }
    
                /* �л�����һ���� */
                switch (standy_step)
                {
                    case FBUS_STEP_INVT:
                        standy_step = USE_INVT_FAN1 ? FBUS_STEP_INVT_FAN1 \
                                                    : (USE_ECFAN1 ? FBUS_STEP_EX_EC1 \
                                                                  : FBUS_STEP_INVT);
                        break;
                    case FBUS_STEP_INVT_FAN1:
                        standy_step = USE_INVT_FAN2 ? FBUS_STEP_INVT_FAN2 \
                                                    : FBUS_STEP_INVT;                   
                        break;
                    case FBUS_STEP_INVT_FAN2:
                        standy_step = FBUS_STEP_INVT;
                        break;
                    case FBUS_STEP_EX_EC1:
                        standy_step = USE_ECFAN2 ? FBUS_STEP_EX_EC2 \
                                                 : FBUS_STEP_INVT;
                        break;
                    case FBUS_STEP_EX_EC2:
                        standy_step = FBUS_STEP_INVT;
                        break;
                }

//                if (GET_INVT_CONNECT)/* ����������Ӧ��� */
//                {
                    /* ��Ϊ���ձ�Ƶѹ������ */
                    if (standy_step == FBUS_STEP_INVT)
                    {
                        commu_id = 11;      /* ��Ƶ���ͽ������л������������ */
                    }
//                }
//                else
//                {
//                    standy_step = FBUS_STEP_INVT;
//                    invt_id = INVT_ID_1;
//                }
            }
            break;
    
        case 11:
//            if ((!GET_INOUT_CONNECT && COM_QEmpty(com)) || COM_QEmpty(com))
            if (COM_QEmpty(com))
            {
                U08 i;

                for (i=0; i<EXIO_KLV_MAX; i++)
                {
                    EK_mst_create_request(com, i);
                }

//                if (INVT_INOUT_BOTH_CONNECT)
//                {
                    commu_id = USE_INVT ? 10 : 11;
//                }

//                if (!INVT_INOUT_BOTH_CONNECT)
//                {
//                    commu_id = 10;
//                }
            }
            break;
    }
}


/****************************************************************
��������ȡ����(R_LAN�ӻ�)״̬
���ܣ���RLAN��ȡ��RLAN��ϵͳ״̬
��������
���أ���
��������ȡ��ʽΪѭ����ȡ�������ȼ���
*****************************************************************/
#define     MAX_RD_DATA_NUM     120         /* ÿ�θ���״̬���ݵ����ֵ �� */   
U08 work_mac_rd_sta(U08 com)
{
    U16 data_addr = 0;
    static U08 mac_now = 0; /* ��ǰͨѶ�Ļ��� */
        
    if (COM_QGetNodeCnt(com) < MAX_L_NODE_NUM)
    {
        U08 max_cell_rd = 0;    /* CELL �ṹ���ȡ������ */
        U08 fg_rd_overflow_max = 0;     /* ��ȡ����������һ�ηǶ�ȡ������ݱ�־ */
        static U08 cell_rd_cnt = 0;     /* CELL �ṹ��ȡ�Ĵ��� */

        /* CELL�ṹ�����ݶ�ȡ���� */
        max_cell_rd = CELL_SIZE / MAX_RD_DATA_NUM;
        if (CELL_SIZE % MAX_RD_DATA_NUM)
        {
            max_cell_rd = max_cell_rd + 1;
            fg_rd_overflow_max = 1;
        }

        /* 
         * 2017/12/18--1027 XMacro 
         * ģ����ã�����������ģ��ͨѶ��ʡ����Ҫ��ͨѶʱ�䡣
         */
        if (!CELL_Is_used(mac_now))
        {
            if (!M_CM_GetStatus(mac_now, MAX_COMP, CELL_RUN))//������ȡ����ģ��״̬�������ģ����ȫ�رղ���
            {
                cell_rd_cnt = max_cell_rd;
            }
        }

        /* �ִδ���ṹ������ͨѶ */
        if (cell_rd_cnt < max_cell_rd)
        {
            U08 read_num;
            
            /* ���ݶ�ȡ */
            data_addr = (U16)FBUS_MAC_BGN + ((U16)CELL_SIZE*mac_now) + cell_rd_cnt*MAX_RD_DATA_NUM;
            read_num = MAX_RD_DATA_NUM;
            if (((cell_rd_cnt+1) >= max_cell_rd) && fg_rd_overflow_max == 1)
            {
                read_num = CELL_SIZE % MAX_RD_DATA_NUM;
            }
            /* 2023-1-9����������cell_cmd������Ӵӻ���ȡ��cell_cmd�Ḳ������д������� */
            if (cell_rd_cnt == 0)
            {
                data_addr += MAX_CMD;
                read_num -= MAX_CMD;
            }
            com_FBUS_rd(com, mac_now, data_addr, read_num, Q_PRIO_LOW);

            cell_rd_cnt++;

            /* 
             * 2017/12/18--1027 XMacro 
             * ͨѶ���Ϻ�ֻ��ȡһ�ζ�Ӧ���жϡ�
             * ���������Խ�ʡͨѶʱ�䡣
             */
            if (GETBIT(slave_commu_error, mac_now))
            {
                cell_rd_cnt = max_cell_rd;
            }
        }
        else
        {
            /* ����ģ���cell���ݶ�ȡ��� */
            cell_rd_cnt = 0;
            if (++mac_now >= val_pcb_num)       
            {
                mac_now = 1;
                
                return 1;
            }
        }
    }

    return 0;
}

/****************************************************************
������work_mac_rd_ver()
���ܣ���RLAN��ȡ��RLAN�İ汾��
��������
���أ���
��������ȡ��ʽΪѭ����ȡ�������ȼ���
*****************************************************************/
static U16 fg_mac_ver_rd_cplt;              /* �汾�Ŷ�ȡ��ɱ�־��ÿһλ����һ��ģ�� */
U08 work_mac_rd_ver(U08 com)
{   
    U16 data_addr = 0;
    static U08 mac_now = 0; /* ��ǰͨѶ�Ļ��� */

    if (COM_QGetNodeCnt(com) < MAX_L_NODE_NUM)
    {
        U08 max_data_rd_cnt = 0;            /* ���ݶ�ȡ������ */
        U08 fg_rd_overflow_max = 0;     /* ��ȡ����������һ�ηǶ�ȡ������ݱ�־ */
        static U08 data_rd_cnt = 0;     /* ���ݶ�ȡ�Ĵ��� */

        /* CELL�ṹ�����ݶ�ȡ���� */
        max_data_rd_cnt = ONE_VER_SIZE / MAX_RD_DATA_NUM;
        if (ONE_VER_SIZE % MAX_RD_DATA_NUM)
        {
            max_data_rd_cnt++;
            fg_rd_overflow_max = 1;
        }

        /* �ִδ���ṹ������ͨѶ */
        if (data_rd_cnt < max_data_rd_cnt)
        {
            U08 read_num;
            
            /* ���ݶ�ȡ */
            data_addr = (U16)FBUS_VER_BGN + ((U16)ONE_VER_SIZE*mac_now) + data_rd_cnt*MAX_RD_DATA_NUM;
            read_num = MAX_RD_DATA_NUM;
            if (((data_rd_cnt+1) >= max_data_rd_cnt) && fg_rd_overflow_max == 1)
            {
                read_num = ONE_VER_SIZE % MAX_RD_DATA_NUM;
            }
            com_FBUS_rd(com, mac_now, data_addr, read_num, Q_PRIO_LOW);

            /* ���ݶ�ȡ */
            data_addr = (U16)FBUS_ODU_VER_BGN + ((U16)ONE_ODU_VER_SIZE*mac_now) + data_rd_cnt*MAX_RD_DATA_NUM;
            read_num = MAX_RD_DATA_NUM;
            if (((data_rd_cnt+1) >= max_data_rd_cnt) && fg_rd_overflow_max == 1)
            {
                read_num = ONE_ODU_VER_SIZE % MAX_RD_DATA_NUM;
            }
            com_FBUS_rd(com, mac_now, data_addr, read_num, Q_PRIO_LOW);

            data_rd_cnt++;

            /* 
             * 2017/12/18--1027 XMacro 
             * ͨѶ���Ϻ�ֻ��ȡһ�ζ�Ӧ���жϡ�
             * ���������Խ�ʡͨѶʱ�䡣
             */
            if (GETBIT(slave_commu_error, mac_now))
            {
                data_rd_cnt = max_data_rd_cnt;
            }
        }
        else
        {
            /* ����ģ������ݶ�ȡ��� */
            data_rd_cnt = 0;
            SETBIT(fg_mac_ver_rd_cplt, mac_now);
            if (++mac_now >= val_pcb_num)       
            {
                mac_now = 1;
                return 1;
            }           
        }
    }

    return 0;
}

/*******************************************************************************
 �� ��: work_mac_rd_mod_para()
 �� ��: ��ģ���ȡĳ����ģ��Ĳ���ֵ
 �� ��: com--; 
 �� ��: 
 �� ��: ��
*******************************************************************************/
U08 work_mac_rd_mod_para(U08 com)
{   
    U16 data_addr = 0;
    U08 mac_now = modParaRdIdDebug; /* ��ǰͨѶ�Ļ��� */

    if (COM_QGetNodeCnt(com) < MAX_L_NODE_NUM)
    {
        U08 max_data_rd_cnt = 0;            /* ���ݶ�ȡ������ */
        U08 fg_rd_overflow_max = 0;         /* ��ȡ����������һ�ηǶ�ȡ������ݱ�־ */
        static U08 data_rd_cnt = 0;         /* ���ݶ�ȡ�Ĵ��� */

        /* para_C���ݶ�ȡ���� */
        max_data_rd_cnt = PARA_SIZE / MAX_RD_DATA_NUM;
        if (PARA_SIZE % MAX_RD_DATA_NUM)
        {
            max_data_rd_cnt++;
            fg_rd_overflow_max = 1;
        }

        /* �ִδ���ṹ������ͨѶ */
        if (data_rd_cnt < max_data_rd_cnt)
        {
            U08 read_num;
            
            /* ���ݶ�ȡ */
            data_addr = (U16)ADDR_PARA_BGN + data_rd_cnt*MAX_RD_DATA_NUM;
            read_num = MAX_RD_DATA_NUM;
            if (((data_rd_cnt+1) >= max_data_rd_cnt) && fg_rd_overflow_max == 1)
            {
                read_num = PARA_SIZE % MAX_RD_DATA_NUM;
            }
            com_FBUS_rd(com, mac_now, data_addr, read_num, Q_PRIO_LOW);

            data_rd_cnt++;

            /* 
             * 2017/12/18--1027 XMacro 
             * ͨѶ���Ϻ�ֻ��ȡһ�ζ�Ӧ���жϡ�
             * ���������Խ�ʡͨѶʱ�䡣
             */
            if (GETBIT(slave_commu_error, mac_now))
            {
                data_rd_cnt = max_data_rd_cnt;
            }
        }
        else
        {
            /* ��ǰģ������ݶ�ȡ��� */
            data_rd_cnt = 0;
            return 1;          
        }
    }

    return 0;
}

/****************************************************************
����������ϵͳ״̬���ӻ�(cmd_mac_sta)
���ܣ��ӻ���״̬�仯��ʱ����״̬���ӻ���(ͬʱ����״̬��ϵͳ�¶�)
�����������
���أ���
��������������������ı�����ֵ��������ͨѶ��㡣
*****************************************************************/
static U08 cnt_commu_step_ot;               /* ��ȡ״̬���ʱ���ʱ */
U08 cmd_mac_sta(U08 com)
{
    /* �㲥ϵͳ״̬ �� ����3���ǿ���˳� */
    if (COM_QGetNodeCnt(com) < MAX_L_NODE_NUM)
    {
        if (com_FBUS_bro(com, FBUS_SYS_STA_BGN, FBUS_SYS_STA_SIZE, Q_PRIO_LOW))
        {
            return 1;
        }
    }

    return 0;
}

/*******************************************************************************
 �� ��: cmd_bro_special()
 �� ��: �������ͣ��㲥��������
 �� ��: com--; 
 �� ��: 
 �� ��: ʹ��bro_specialͨ��ʱ���˴�Ҫʹ����ͬ�������д��ݣ�������ģ���޷�ͨ����
        ��ͨ�ŵķ�ʽ���н���
*******************************************************************************/
U08 cmd_bro_special(U08 com)
{   
    static U08 cnt_bro_itv;
    /* �㲥ϵͳ״̬ �� ����3���ǿ���˳� */
    if (COM_QGetNodeCnt(com) < MAX_L_NODE_NUM
    &&  cnt_bro_itv >= 5
        )
    {
        if (com_FBUS_bro(com, ADDR_SPECIAL_BGN, WR_SPECIAL_NUM, Q_PRIO_LOW))
        {
            cnt_bro_itv = 0;
                
            bro_special[0] = fg_idle_sta_antifreeze;
            bro_special[1] = cell[MAX_PCB].cell_runtime[SYS_RUNTIME];
            bro_special[2] = limit_value;
            bro_special[3] = limit_type;
            bro_special[4] = sv_temp_energy[SUB_SYS_AC_HOTW];
            bro_special[5] = ctrl_temp_energy[SUB_SYS_AC_HOTW];
            bro_special[6] = fg_warm_complete;
            bro_special[7] = fg_timed_disinfect_run;
            bro_special[8] = MacTypeMaster;
            bro_special[9] = fg_silent_mode;
            bro_special[10] = fg_ability_test_run;
            bro_special[11] = ctrl_mode[SUB_SYS_AC_HOTW];
            bro_special[12] = sv_temp_freq[SUB_SYS_AC_HOTW];
            bro_special[13] = fg_TBH_run;   /* TBH�ѿ��� */
            bro_special[14] = ac_mode;
            bro_special[15] = fg_idle_sta_antifreeze_IBH_need;
            bro_special[16] = fg_force_IBH_run;             /* ǿ���������� */
            bro_special[17] = fg_AHS_run;
            bro_special[18] = fg_PUMPf_test_run;
            bro_special[19] = fg_refri_recy_act;
            bro_special[20] = heat_runtime[SUB_SYS_AC_HOTW].run;
            bro_special[21] = hotw_runtime.run;
            bro_special[22] = fg_timed_disinfect_IBH_need;
            bro_special[23] = fg_enter_test_run;
            bro_special[24] = fg_test_run_heat_IBH_run;
            bro_special[25] = fg_pump_test_run;
            bro_special[26] = fg_test_run_check_LDIw;
            bro_special[27] = fg_test_run_PUMPf_off;
            bro_special[28] = fg_floor_preheat_run;
            bro_special[29] = fg_floor_dryup_run;            
            bro_special[30] = fg_air_empty_test_run;
            bro_special[31] = sg_power_mode;
            bro_special[32] = fg_timed_disinfect_TBH_need;
            bro_special[33] = fg_timed_disinfect_AHS_need;
            bro_special[34] = fg_timed_disinfect_AHS_on;
            bro_special[35] = ctrl_mode[SUB_SYS_AC_ONLY];
            bro_special[36] = sv_temp_energy[SUB_SYS_AC_ONLY];
            bro_special[37] = ctrl_temp_energy[SUB_SYS_AC_ONLY];
            bro_special[38] = sv_temp_freq[SUB_SYS_AC_ONLY];
            bro_special[39] = status_run;
            bro_special[40] = fg_run_sta_ac_antifreeze_IBH_need;
            bro_special[41] = fg_run_sta_hotw_antifreeze_IBH_need;
            bro_special[42] = idle_sta_antifreeze_stage;
            bro_special[43] = fg_run_sta_ac_antifreeze;
            bro_special[44] = fg_run_sta_hotw_antifreeze;
            bro_special[45] = fg_zone_A_timed_antifreeze_COMPf_need;
            bro_special[46] = fg_zone_B_timed_antifreeze_COMPf_need;
            bro_special[47] = fg_hotw_timed_antifreeze_COMPf_need;
            bro_special[48] = fg_cool_need;
            bro_special[49] = fg_heat_need;
            bro_special[50] = fg_hotw_need;
            bro_special[51] = silent_mode_level;
            bro_special[52] = fg_sv_temp_energy_overwrite_ac;
            bro_special[53] = ctrl_temp_type_energy_ac;
            bro_special[54] = heat_runtime[SUB_SYS_AC_ONLY].run;
            bro_special[55] = fg_energy_need[SUB_SYS_AC_HOTW];
            bro_special[56] = fg_energy_need[SUB_SYS_AC_ONLY];
            bro_special[57] = fg_hotw_timed_antifreeze_S3V1_OFF_need;
            bro_special[58] = fg_zone_A_run;
            bro_special[59] = fg_zone_B_run;
            bro_special[60] = fg_hotw_run;

            return 1;
        }
    }

    if (F_TIME_BASIC_SEC_BACK)
    {
        _CNT(cnt_bro_itv);
    }

    return 0;
}

/*****************************************************************************
��  �� : USER_Commu_Fbus()
��  �� : ����ͨѶ����
��  �� : ��
��  �� : ��
��  �� : �߼����Ż�-2021-7-14-kjh����ͬ���蹲��ͨѶ�ڣ���ģ�黯
*****************************************************************************/
#define     MAC_RW_INTVL        60          /* ��ȡ��ʱ */
#define     MAC_BRO_INTVL       3           /* �㲥��ʱ */
void USER_Commu_Fbus(U08 com)
{
    U08 i, j;
    U08 no = 0;
    U08 cmd_need = 0;
    static U08  commu_id = 0;
    static U08  commu_id_bak = 0;
    static U16 pcb_num_bak = 0;
    U16 *pfg_mac_ver_rd_cplt = &fg_mac_ver_rd_cplt;

    switch (commu_id)
    {
        case 0 :    /* ��ʼ�� */
            commu_id = 1;           

            break;

        case 1 :    /* �㲥�������ݣ�Ҫ�ڼ�������֮ǰ */
            if (cmd_bro_special(com) || cnt_commu_step_ot>MAC_BRO_INTVL)
            {
                commu_id = 10;
            }
            break;          

        case 10 :   
            if (m_password_broadcast(com)) break;   /* ����㲥 */
            if (para_broadcast(com)) break;         /* �����㲥 */

            /* ����㲥 �� ״̬��ȡ */
            {
                CLRBIT(cmd_modify, 0);  /* 0#ģ�������ҪͨѶ */

                if (cmd_broadcast != 0)
                {
                    /* ע������ֻ������0#ģ��Ĺ㲥����ģ�������ܴ��� */
                    if (com_FBUS_bro(com, FBUS_CMD_BGN, FBUS_CMD_SIZE, Q_PRIO_HIGH))
                    {
                        cmd_broadcast = 0;        /* �ù㲥��־ */

                        no = MAX_PCB;      /* �㲥��Ϣλ��MAX_PCB�� */
                        for (i=0; i<MAX_CMD; i++)         /* ��ģ��ֱ��ʹ��, ����ҪͨѶ */
                        {
                            cell[0].cell_cmd[i] |= cell[no].cell_cmd[i];
                            bro_cell_cmd[i] = cell[no].cell_cmd[i]; /* �������� */

                            cell[no].cell_cmd[i] = 0;   /* ʹ�ú����� */
                        }
                    }
                    break;
                }
                else if (cmd_modify != 0)
                {
                    for (j = 0; j < val_pcb_num; j++)
                    {
                        if (GETBIT(cmd_modify, j))
                        {
                            no = j;
                            cmd_need = 1;
                            break;
                        }
                    }
                    if (cmd_need == 1)
                    {
                        if (com_FBUS_wr(com, no, FBUS_CMD_BGN, FBUS_CMD_SIZE, Q_PRIO_HIGH))
                        {
                            CLRBIT(cmd_modify, no);

                            /* ��ģ��ֱ��ʹ��, ����ҪͨѶ */
                            for (i=0; i<MAX_CMD; i++)         
                            {
                                nor_cell_cmd[no][i] = cell[no].cell_cmd[i]; /* �������� */
                                cell[no].cell_cmd[i] = 0;   /* ʹ�ú����� */
                            }
                            if (fg_trans_control_sta[no] == TRUE)
                            {
                                fg_trans_control_sta[no] = FALSE;
                            }
                        }
                    }
                    break;
                }
                else if (val_pcb_num > 1)   /* ����ͨѶ���� ״̬��ȡ */
                {
                    enum {STEP_RD_STA, STEP_RD_VER, STEP_RD_MOD_PARA, STEP_RD_MAX};
                    static U08 step_rd = STEP_RD_STA;
                    
                    switch (step_rd)
                    {
                        case STEP_RD_STA:
                            if (work_mac_rd_sta(com) || cnt_commu_step_ot>MAC_RW_INTVL) 
                            {
                                step_rd++;
                            }
                            break;
                        case STEP_RD_VER:
                            /* ��־��ʼ����ģ�����ı� */
                            if (pcb_num_bak != val_pcb_num)
                            {
                                pcb_num_bak = val_pcb_num;
                                
                                *pfg_mac_ver_rd_cplt = 0;

                                for (i=val_pcb_num; i<16; i++)
                                {
                                    SETBIT(*pfg_mac_ver_rd_cplt, i);
                                }
                            }
                            /* ��־���⴦��ͨѶ����Ҫ���¶�ȡ */
                            for (i=0; i<val_pcb_num; i++)
                            {
                                // ����������
                                if (i == 0) continue;
                                
                                if (GETBIT(slave_commu_error, i))
                                {
                                    CLRBIT(*pfg_mac_ver_rd_cplt, i);
                                }

                                // ����ȡһ�ε�����£��ᵼ�¶�ȡ�����ӻ������汾��
                                if (idu_mac_ver[i][0] == 0 || odu_mac_ver[i][0] == 0)
                                {
                                    CLRBIT(*pfg_mac_ver_rd_cplt, i);
                                }
                            }
                            
                            /* ֻ��ȡһ��汾�ţ���ȡ��ɺ��ٶ�ȡ */
                            if (*pfg_mac_ver_rd_cplt != 0xffff)
                            {
                                if (work_mac_rd_ver(com) || cnt_commu_step_ot>MAC_RW_INTVL) 
                                {
                                    step_rd++;
                                }
                            }
                            else    
                            {
                                step_rd++;
                            }
                            break;

                        /* ��ĳ����ģ��Ĳ���ֵ */
                        case STEP_RD_MOD_PARA:
                            if (modParaRdDebug)
                            {
                                if (work_mac_rd_mod_para(com) || cnt_commu_step_ot>MAC_RW_INTVL)
                                {
                                    step_rd++;
                                    modParaRdDebug = FALSE;
                                }
                            }
                            else
                            {
                                step_rd++;
                            }
                            break;
                            
                        case STEP_RD_MAX:   
                            /* break; */
                        default:
                            step_rd = STEP_RD_STA;
                            commu_id = 20;
                            break;
                    }
                }
                else    
                {
                    commu_id = 30;          /* ��������ͨѶ */
                }
            }
            break;

        case 20 :   /* ϵͳ״̬�㲥 */
            if (cmd_mac_sta(com) || cnt_commu_step_ot>MAC_BRO_INTVL)
            {
                commu_id = 30;
            }
            break;

        case 30 :   /* �������ͨѶ */
#ifdef COM_FCU_FBUS            
            if (fcu_commu() || cnt_commu_step_ot>MAC_RW_INTVL)
#endif                   
            {
                commu_id = 1;               /* ���һ��ͨѶ��������󷵻� */
            }
            break;
    }

    if (commu_id_bak != commu_id)
    {
        commu_id_bak = commu_id;
        cnt_commu_step_ot = 0;
    }

    if (F_TIME_BASIC_100MS_BACK)
    {
        if (commu_m_password_broadcast_interval <= 0xfffe)
        {
            commu_m_password_broadcast_interval++;
        }
    }
    
    if (F_TIME_BASIC_SEC_BACK)
    {
        if (cnt_commu_step_ot <= 0xfe) cnt_commu_step_ot++;
    }
}

#ifdef USE_LOG_PRINT
    extern void DBG_LoggingPara(void);
#endif

/*******************************************************************************
 �� ��: Timer_deal()
 �� ��: �����ʱ�����
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void Timer_deal(void)
{
    disp_time();    /* DM603A�������ʱ�� */
    ec_fan_time();
    fcu_time();
    invt_timer(F_TIME_BASIC_100MS_BACK);
    eev_timer(F_TIME_BASIC_100MS_BACK);   //NOTE-CXW ����2017-5-12
    pid_timer(F_TIME_BASIC_100MS_BACK);
    SH_timer();
    TYB_timer();
    EK_mst_timer();
    modbus_time();
}

/*******************************************************************************
 �� ��: COM_deal()
 �� ��: ����ͨѶ�����
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void COM_deal(void)
{
    /* ��ʾ��(�ı�/����/wifi��)ͨѶ */
    SH_MB_commu_adjust();

    /* �����&��ƵͨѶ */
    if (TF_IS_IN)           /* �ڻ��̼�ʱ������COM_INVT */
    {
        VALCLR(fg_invt_commu_error);
        CLR_EXIO_COMMU_ERR(0);
        s_clr_error(ERR_INVT_COMMU);
        s_clr_error(S_ERR_COMMU_INOUT);
    }
    else
    {
        USER_Commu_ODU_INVT(COM_INVT);
        COM_MasterProg(COM_INVT);
    }

    /* ����ͨѶ */
    if (TestProgStaus != TPS_NO_TEST_CMD
    ||  pcb_type == TYPE_SLAVER
        )
    {
        COM_SlaverProg(COM_FCU_FBUS);
    }
    else
    {
        USER_Commu_Fbus(COM_FCU_FBUS);
        COM_MasterProg(COM_FCU_FBUS);
    }

    /* ������� */
    COM_SlaverProg(COM_BMS); 


#ifdef COM_4G
    COM_SlaverProg(COM_4G);
#endif
}

/*******************************************************************************
 �� ��: DataStore_deal()
 �� ��: ����������棬���뱣��������
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void DataStore_deal(void)
{
    DataStore_Parameters();
    DataStore_EEPROM();
}

/*****************************************************************************
��  �� : USER_Main()
��  �� : �û�������
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
extern void TEST_Main(void);
void USER_Main(void)
{
    USER_time_basic_set();
    WDG_Refresh();
    if (F_TIME_SWITCH_1S == 0)  Dbgdebug_xx = 0xaaaa;
    else                        Dbgdebug_xx = 0;
    
    if (TestProgStaus != TPS_NO_TEST_CMD || power_time < POWER_ON_INTO_TEST)
    {
        TEST_Main();
    }

    /* ����汾����׼���� */
    if (GET_EXIO_VER_READY(0))
    {
        MEMCPY(&odu_mac_ver[FBUS_ADDR], GET_EXIO_VER(0), MAX_VER);
    }
    
    Timer_deal();
    COM_deal();
    DataStore_deal();

    if(power_time==INIT_INVT_RST_TIME)  //����ʱ���һ�븴λ��Ƶ�����Ϸ�ֹ��ͬ���ϵ������ͨѶ����
    {
        static U08 fg_reset = FALSE;
        if(!fg_reset)
        {
            fg_reset = TRUE;
            invt_err_reset();
        }
    }
    
    /* Ӧ���߼� */
    if (TestProgStaus == TPS_NO_TEST_CMD && power_time > POWER_ON_INTO_TEST)
    {
        app_4g_ro.cmd = INTO_4G_APP;
        User_main_task();
        get_scan_period();
    }

    app_4g();

#ifdef USE_LOG_PRINT
    if (power_time > 10)
    {
	    LOG_Server();
        DBG_LoggingPara();
    }
#endif

#ifdef M_DEBUG
    /* DEBUG΢�룺sw2.4Ϊoff��ʾ���٣�Ϊon��ʾ���٣�֧���Ȳ��룻����΢��ֻҪ����USE_QUICK_TIME��Ϊ���� */
    {
        U16 jmp[MAX_SW];
        PunpGetSwitch(&jmp[0], MAX_SW_BIT);
        fg_dbg_slow = !GETBIT_N(jmp, 5);
    }
#endif

    USER_time_basic_reset();
}

