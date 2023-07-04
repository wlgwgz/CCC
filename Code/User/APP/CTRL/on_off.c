#define ON_OFF_GLOBAL
#include "includes.h"

/*==========================================
    ����: ����ֹͣ��ť
/==========================================*/
void Logic_machine_stop(void)
{

    if (fg_idle_sta_antifreeze) /* ����ʱ������ػ����� */
    {
        if(log_sys_comp_run_num(SUB_SYS_TYPE_MAX))
        {
            status_run = _DELAY;
        }
        else
        {
            status_run = _IDLE;
        }
    }
    
    fg_machine_need_open = FALSE;

    /* ������б�־ */
    fg_timed_disinfect_run = FALSE;
    fg_ability_test_run = FALSE;
    fg_floor_preheat_run = FALSE;
    fg_floor_dryup_run = FALSE;
        
    fg_force_hotw_mode_run = FALSE;
//    fg_holiday_away_run = FALSE;
//    fg_holiday_home_run = FALSE;
    

    fg_force_TBH_run = FALSE;
    fg_force_IBH_run = FALSE;    
    fg_force_AHS_run = FALSE;

    ARRCLR(fg_test_run);
    
    switch (status_run)
    {
        case _IDLE  : break;
        case _READY :
        case _RUN   : status_run = _DELAY;      break;
        case _DELAY : break;
        case _ALARM : /* status_run = _IDLE;*/  break;
    }

    //log_sys_init_idle_sta_antifreeze();     /* �ش���̬���� */

    Link_remeber_on = FALSE;
    cmd_refri_recy_stop = TRUE;
#ifdef COM_FCU_FBUS    
    SETBIT(fcu_semaphore, FCU_SEM_M_CLOSE_S);
#endif
}

/*==========================================
    ����: �����������
/==========================================*/
void Logic_machine_start(void)
{   
    if (fg_sys_limit)       return;     /* ʹ�����޵��������� */

    if (fg_idle_sta_antifreeze)  /* ����ʱ���ȹ��ٿ� */
    {
        status_run = _DELAY;
    }
    
    switch (status_run)
    {
        case _IDLE  : 
            if (!TF_EN                      /* �̼�ʱ���������� */
            )
            {
                status_run = _READY;    
            }
            break;
            
        case _READY :
        case _RUN   : 
            break;
            
        case _DELAY :
            fg_machine_need_open = TRUE;
            break;
            
        case _ALARM : 
            break;
    }

    //log_sys_init_idle_sta_antifreeze();     /* �ش���̬�������Ƿ���ࣿ���Բ�Ҫ�������Դ����Զ�����ʱ�رշ��� */
    Link_remeber_on = FALSE;    
#ifdef COM_FCU_FBUS    
    SETBIT(fcu_semaphore, FCU_SEM_M_START_S);
#endif
}

/*******************************************************************************
 �� ��: on_off_init()
 �� ��: ���ػ���ʼ��
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void on_off_init(void)
{
//  auto_run_read_from_eeprom();
    DataAccess_AUTO_RUN(DATA_READ);
//  limit_time_read_from_eeprom();
}

/*********************************************************
������sys_limit_deal
���ܣ�����ʹ������
��������
����ֵ����
*********************************************************/
void on_off_limit(void)
{
//  U16 a,b; 
//  I16 c,d,e,f,g,h;
    
    fg_sys_limit = FALSE;
    pre_limit_time = 0;
    
//  if (key_sys_limit)
//  {
//      fg_sys_limit = TRUE;
//      key_sys_limit = FALSE;
//  }
    
    if (limit_value != 0)       /* ��Ϊ0ʱ��ʹ�øù��� */
    {
        if (cell[MAX_PCB].cell_runtime[SYS_RUNTIME]/24 >= limit_value)      /* ʹ�����޵� */
        {
            fg_sys_limit = TRUE;
        }
        else 
        {
            pre_limit_time = limit_value*24 - cell[MAX_PCB].cell_runtime[SYS_RUNTIME];
            if (pre_limit_time > 168)
            {
                pre_limit_time = 0;
            }
        }
    }
    /* ʹ�����޵� */
    if (fg_sys_limit)
    {
        if(limit_type==STOP_UNIT && (status_run == _RUN || status_run ==_READY))    // ǿ�ƹػ�������������
        {
            Logic_machine_stop();     /* ִ�йػ����� */
        }
    }
}

/*******************************************************************************
 �� ��: on_off_remote()
 �� ��: �߿ؿ��ؿ��ػ�
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void on_off_remote(void)
{
    BOOL remote_switch = FALSE;             /* �߿ؿ��ص�ǰ״̬(��ʼ��Ϊ��Ч״̬) */
    static BOOL remote_switch_bak;          /* �߿ؿ�����ǰ״̬ */
    static BOOL fg_first = TRUE;            // ��һ�ν���

    if (m_get_input(IN_REMOTE))  
    {
        remote_switch = TRUE;
    }
    if (fg_first)
    {
        fg_first = FALSE;
        remote_switch_bak = remote_switch;
    }
    
    /* �߿ؿ��ؿ��ػ� */
    if (val_on_off_control_type != ONOFF_SELF && val_on_off_control_type != ONOFF_COMMU)
    {
        if (remote_switch_bak != remote_switch)
        {
            if (val_remote_switch_type == TYPE_SWITCH)       /* �������� */
            {
                if (remote_switch)    /* ������Ч  */
                {
                    if (status_run == _IDLE || status_run == _DELAY)
                    {
                        if (USE_DUAL_ZONE_CTRL && ac_mode != MODE_COOL)
                        {
                            fg_zone_A_run = TRUE;
                            fg_zone_B_run = TRUE;
                        }
                        else
                        {
                            fg_zone_A_run = TRUE;
                        }
                        if (val_hotw_used)
                        {
                            fg_hotw_run = TRUE;
                        }
                        Logic_machine_start();    /* ִ�п������� */
                    }
                }
                else
                {
                    if (status_run == _READY || status_run == _RUN)
                    {
                        fg_zone_A_run = FALSE;
                        fg_zone_B_run = FALSE;
                        fg_hotw_run = FALSE;
                        Logic_machine_stop();     /* ִ�йػ����� */
                    }
                }
            }
            else                                             /* ���忪�� */
            {
                if (remote_switch_bak && !remote_switch)     /* ��������Ч״̬-->��Ч״̬(�½���) */
                {
                    if (status_run == _IDLE || status_run == _DELAY)
                    {
                        if (USE_DUAL_ZONE_CTRL && ac_mode != MODE_COOL)
                        {
                            fg_zone_A_run = TRUE;
                            fg_zone_B_run = TRUE;
                        }
                        else
                        {
                            fg_zone_A_run = TRUE;
                        }
                        if (val_hotw_used)
                        {
                            fg_hotw_run = TRUE;
                        }
                        Logic_machine_start();    /* ִ�п������� */
                    }
                    else if (status_run == _READY || status_run == _RUN)
                    {
                        fg_zone_A_run = FALSE;
                        fg_zone_B_run = FALSE;
                        fg_hotw_run = FALSE;
                        Logic_machine_stop();     /* ִ�йػ����� */
                    }
                }
            }
            
            remote_switch_bak = remote_switch;
        }
    }
}

/*******************************************************************************
 �� ��: on_off_union_act()
 �� ��: ����������Ч
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void on_off_union_act(void)
{
    BOOL remote_switch = FALSE;             /* �߿ؿ��ص�ǰ״̬(��ʼ��Ϊ��Ч״̬) */
    static BOOL remote_switch_bak;          /* �߿ؿ�����ǰ״̬ */
    static BOOL fg_first = TRUE;            // ��һ�ν���

    if (m_get_input(IN_REMOTE))  
    {
        remote_switch = TRUE;
    }
    if (fg_first)
    {
        fg_first = FALSE;
        remote_switch_bak = remote_switch;
    }
    
    /* �߿ؿ��ؿ��ػ� */
    if (val_on_off_control_type != ONOFF_SELF && val_on_off_control_type != ONOFF_COMMU)
    {
        if (remote_switch_bak != remote_switch)
        {
            if (remote_switch)    /* ������Ч  */
            {
                if (status_run == _IDLE || status_run == _DELAY)
                {
                    if (USE_DUAL_ZONE_CTRL && ac_mode != MODE_COOL)
                    {
                        fg_zone_A_run = TRUE;
                        fg_zone_B_run = TRUE;
                    }
                    else
                    {
                        fg_zone_A_run = TRUE;
                    }
                    if (val_hotw_used)
                    {
                        fg_hotw_run = TRUE;
                    }
                    Logic_machine_start();    /* ִ�п������� */
                }
                fg_onoff_union_act = TRUE;
            }
            else
            {
                if (status_run == _READY || status_run == _RUN)
                {
                    fg_zone_A_run = FALSE;
                    fg_zone_B_run = FALSE;
                    fg_hotw_run = FALSE;
                    Logic_machine_stop();     /* ִ�йػ����� */
                }
                fg_onoff_union_act = FALSE;
            }
            
            remote_switch_bak = remote_switch;
        }
    }
}

/*******************************************************************************
 �� ��: log_sys_ster_fun_onoff()
 �� ��: ��ʱɱ�����ܿ��ƿ��ػ�
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void on_off_timed_disinfect(void)
{
    static BOL fg_enter = FALSE;
    static BOL fg_need_stop = FALSE;

    if (fg_timed_disinfect_run && !fg_enter)
    {
        fg_enter = TRUE;
        
        /* ���붨ʱɱ������ǰ��������̬����ִ�йػ� */
        if (status_run == _READY || status_run == _RUN)
        {
            fg_need_stop = fg_hotw_run ? FALSE : TRUE;		/* ����ˮ��������������ˮ�ػ� */
        }
        /* ���붨ʱɱ������ǰ���ڴ�����ִ�йػ� */
        else if (status_run == _IDLE || status_run == _DELAY)
        {
            fg_need_stop = TRUE;
        }
    }

    /* ��ʱɱ�����ܽ�����ִ����ˮ�ػ� */
    if (!fg_timed_disinfect_run && fg_need_stop)
    {
        fg_hotw_off = TRUE;
        fg_need_stop = FALSE;
    }

    /* δ���붨ʱɱ������ */
    if (!fg_timed_disinfect_run)
    {
        fg_enter = FALSE;
    }
}


/*******************************************************************************
 �� ��: on_off_force_hotw_mode()
 �� ��: ǿ����ˮģʽ���ܿ��ػ�
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void on_off_force_hotw_mode(void)
{
    static BOL fg_enter = FALSE;
    static BOL fg_need_stop = FALSE;

    /* ����ǿ����ˮģʽ����ǰ */
    if (fg_force_hotw_mode_run && !fg_enter)
    {
        fg_enter = TRUE;
        
        /* ����ǿ����ˮģʽǰ��������̬ */
        if (status_run == _READY || status_run == _RUN)
        {
            fg_need_stop = fg_hotw_run ? FALSE : TRUE;		/* ����ˮ�ѿ�������ִ�йػ�������֮ */
        }
        /* ����ǿ����ˮģʽǰ���ڴ�����ִ�йػ� */
        else if (status_run == _IDLE || status_run == _DELAY)
        {
            fg_need_stop = TRUE;
        }
    }

    /* ǿ����ˮģʽ������ִ����ˮ�ػ� */
    if (!fg_force_hotw_mode_run && fg_need_stop)
    {
        fg_hotw_off = TRUE;
        fg_need_stop = FALSE;
    }

    /* δ����ǿ����ˮģʽ */
    if (!fg_force_hotw_mode_run)
    {
        fg_enter = FALSE;
    }
}


/*******************************************************************************
 �� ��: on_off_floor_preheat()
 �� ��: ��ůԤ�ȹ��ܿ��ػ�
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void on_off_floor_preheat(void)
{
    static BOL fg_enter = FALSE;
    static BOL fg_need_stop = FALSE;

    if (fg_floor_preheat_run && !fg_enter)
    {
        fg_enter = TRUE;
        
        /* �����ůԤ��ǰ��������̬����ִ�йػ� */
        if (status_run == _READY || status_run == _RUN)
        {
            fg_need_stop = FALSE;
        }
        /* �����ůԤ��ǰ���ڴ�����ִ�йػ� */
        else if (status_run == _IDLE || status_run == _DELAY)
        {
            fg_need_stop = TRUE;
        }
    }

    /* ��ůԤ�ȹ��ܽ�����ִ�йػ� */
    if (!fg_floor_preheat_run && fg_need_stop)
    {
        fg_zone_A_off = TRUE;
        fg_need_stop = FALSE;
    }

    /* δ�����ůԤ�ȹ��� */
    if (!fg_floor_preheat_run)
    {
        fg_enter = FALSE;
    }

}


/*******************************************************************************
 �� ��: on_off_floor_dryup()
 �� ��: ��ů��ɿ��ػ�
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void on_off_floor_dryup(void)
{
    static BOL fg_enter = FALSE;
    static BOL fg_need_stop = FALSE;

    if (fg_floor_dryup_run && !fg_enter)
    {
        fg_enter = TRUE;
        
        /* �����ů���ǰ��������̬����ִ�йػ� */
        if (status_run == _READY || status_run == _RUN)
        {
            fg_need_stop = FALSE;
        }
        /* �����ů���ǰ���ڴ�����ִ�йػ� */
        else if (status_run == _IDLE || status_run == _DELAY)
        {
            fg_need_stop = TRUE;
        }
    }

    /* ��ů��ɹ��ܽ�����ִ�йػ� */
    if (!fg_floor_dryup_run && fg_need_stop)
    {
        fg_zone_A_off = TRUE;
        fg_need_stop = FALSE;
    }

    /* δ�����ů��ɹ��� */
    if (!fg_floor_dryup_run)
    {
        fg_enter = FALSE;
    }
}


/*******************************************************************************
 �� ��: on_off_holiday_away()
 �� ��: �����뿪ģʽ���ػ�
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void on_off_holiday_away(void)
{
    ;
//    static BOOL fg_enter = FALSE;
//    static BOOL fg_need_stop = FALSE;
//
//    if (fg_holiday_away_run && !fg_enter)
//    {
//        fg_enter = TRUE;
//        
//        /* ��������뿪ģʽ��������̬����ִ�йػ� */
//        if (status_run == _READY || status_run == _RUN)
//        {
//            fg_need_stop = FALSE;
//        }
//        /* ��������뿪ģʽ���ڴ�����ִ�йػ� */
//        else if (status_run == _IDLE || status_run == _DELAY)
//        {
//            fg_need_stop = TRUE;
//        }
//    }
//
//    /* �����뿪ģʽ������ִ�йػ� */
//    if (!fg_holiday_away_run && fg_need_stop)
//    {
//        fg_zone_A_off = TRUE;
//        fg_need_stop = FALSE;
//    }
//
//    /* δ��������뿪ģʽ */
//    if (!fg_holiday_away_run)
//    {
//        fg_enter = FALSE;
//    }
}

/*******************************************************************************
 �� ��: on_off_holiday_home()
 �� ��: �����ڼ�ģʽ���ػ�
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void on_off_holiday_home(void)
{
//    static BOOL fg_enter = FALSE;
//    static BOOL fg_need_stop = FALSE;
//
//    if (fg_holiday_home_run && !fg_enter)
//    {
//        fg_enter = TRUE;
//        
//        /* ��������ڼ�ģʽ��������̬����ִ�йػ� */
//        if (status_run == _READY || status_run == _RUN)
//        {
//            fg_need_stop = FALSE;
//        }
//        /* ��������ڼ�ģʽ���ڴ�����ִ�йػ� */
//        else if (status_run == _IDLE || status_run == _DELAY)
//        {
//            fg_need_stop = TRUE;
//        }
//    }
//
//    /* �����ڼ�ģʽ������ִ�йػ� */
//    if (!fg_holiday_home_run && fg_need_stop)
//    {
//        fg_zone_A_off = TRUE;
//        fg_need_stop = FALSE;
//    }
//
//    /* δ��������ڼ�ģʽ */
//    if (!fg_holiday_home_run)
//    {
//        fg_enter = FALSE;
//    }
}

