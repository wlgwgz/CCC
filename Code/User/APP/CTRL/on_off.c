#define ON_OFF_GLOBAL
#include "includes.h"

/*==========================================
    功能: 处理停止按钮
/==========================================*/
void Logic_machine_stop(void)
{

    if (fg_idle_sta_antifreeze) /* 防冻时，进入关机处理 */
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

    /* 清空运行标志 */
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

    //log_sys_init_idle_sta_antifreeze();     /* 关待机态防冻 */

    Link_remeber_on = FALSE;
    cmd_refri_recy_stop = TRUE;
#ifdef COM_FCU_FBUS    
    SETBIT(fcu_semaphore, FCU_SEM_M_CLOSE_S);
#endif
}

/*==========================================
    功能: 处理机组启动
/==========================================*/
void Logic_machine_start(void)
{   
    if (fg_sys_limit)       return;     /* 使用期限到不允许开机 */

    if (fg_idle_sta_antifreeze)  /* 防冻时，先关再开 */
    {
        status_run = _DELAY;
    }
    
    switch (status_run)
    {
        case _IDLE  : 
            if (!TF_EN                      /* 商检时不允许启动 */
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

    //log_sys_init_idle_sta_antifreeze();     /* 关待机态防冻：是否多余？可以不要？：可以处理自动开机时关闭防冻 */
    Link_remeber_on = FALSE;    
#ifdef COM_FCU_FBUS    
    SETBIT(fcu_semaphore, FCU_SEM_M_START_S);
#endif
}

/*******************************************************************************
 函 数: on_off_init()
 功 能: 开关机初始化
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void on_off_init(void)
{
//  auto_run_read_from_eeprom();
    DataAccess_AUTO_RUN(DATA_READ);
//  limit_time_read_from_eeprom();
}

/*********************************************************
函数：sys_limit_deal
功能：处理使用期限
参数：无
返回值：无
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
    
    if (limit_value != 0)       /* 不为0时才使用该功能 */
    {
        if (cell[MAX_PCB].cell_runtime[SYS_RUNTIME]/24 >= limit_value)      /* 使用期限到 */
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
    /* 使用期限到 */
    if (fg_sys_limit)
    {
        if(limit_type==STOP_UNIT && (status_run == _RUN || status_run ==_READY))    // 强制关机，且正在运行
        {
            Logic_machine_stop();     /* 执行关机操作 */
        }
    }
}

/*******************************************************************************
 函 数: on_off_remote()
 功 能: 线控开关开关机
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void on_off_remote(void)
{
    BOOL remote_switch = FALSE;             /* 线控开关当前状态(初始化为无效状态) */
    static BOOL remote_switch_bak;          /* 线控开关先前状态 */
    static BOOL fg_first = TRUE;            // 第一次进入

    if (m_get_input(IN_REMOTE))  
    {
        remote_switch = TRUE;
    }
    if (fg_first)
    {
        fg_first = FALSE;
        remote_switch_bak = remote_switch;
    }
    
    /* 线控开关开关机 */
    if (val_on_off_control_type != ONOFF_SELF && val_on_off_control_type != ONOFF_COMMU)
    {
        if (remote_switch_bak != remote_switch)
        {
            if (val_remote_switch_type == TYPE_SWITCH)       /* 拨动开关 */
            {
                if (remote_switch)    /* 开关有效  */
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
                        Logic_machine_start();    /* 执行开机操作 */
                    }
                }
                else
                {
                    if (status_run == _READY || status_run == _RUN)
                    {
                        fg_zone_A_run = FALSE;
                        fg_zone_B_run = FALSE;
                        fg_hotw_run = FALSE;
                        Logic_machine_stop();     /* 执行关机操作 */
                    }
                }
            }
            else                                             /* 脉冲开关 */
            {
                if (remote_switch_bak && !remote_switch)     /* 开关由有效状态-->无效状态(下降沿) */
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
                        Logic_machine_start();    /* 执行开机操作 */
                    }
                    else if (status_run == _READY || status_run == _RUN)
                    {
                        fg_zone_A_run = FALSE;
                        fg_zone_B_run = FALSE;
                        fg_hotw_run = FALSE;
                        Logic_machine_stop();     /* 执行关机操作 */
                    }
                }
            }
            
            remote_switch_bak = remote_switch;
        }
    }
}

/*******************************************************************************
 函 数: on_off_union_act()
 功 能: 联动动作有效
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void on_off_union_act(void)
{
    BOOL remote_switch = FALSE;             /* 线控开关当前状态(初始化为无效状态) */
    static BOOL remote_switch_bak;          /* 线控开关先前状态 */
    static BOOL fg_first = TRUE;            // 第一次进入

    if (m_get_input(IN_REMOTE))  
    {
        remote_switch = TRUE;
    }
    if (fg_first)
    {
        fg_first = FALSE;
        remote_switch_bak = remote_switch;
    }
    
    /* 线控开关开关机 */
    if (val_on_off_control_type != ONOFF_SELF && val_on_off_control_type != ONOFF_COMMU)
    {
        if (remote_switch_bak != remote_switch)
        {
            if (remote_switch)    /* 开关有效  */
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
                    Logic_machine_start();    /* 执行开机操作 */
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
                    Logic_machine_stop();     /* 执行关机操作 */
                }
                fg_onoff_union_act = FALSE;
            }
            
            remote_switch_bak = remote_switch;
        }
    }
}

/*******************************************************************************
 函 数: log_sys_ster_fun_onoff()
 功 能: 定时杀菌功能控制开关机
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void on_off_timed_disinfect(void)
{
    static BOL fg_enter = FALSE;
    static BOL fg_need_stop = FALSE;

    if (fg_timed_disinfect_run && !fg_enter)
    {
        fg_enter = TRUE;
        
        /* 进入定时杀菌功能前处于运行态，不执行关机 */
        if (status_run == _READY || status_run == _RUN)
        {
            fg_need_stop = fg_hotw_run ? FALSE : TRUE;		/* 若热水开机，则无需热水关机 */
        }
        /* 进入定时杀菌功能前处于待机，执行关机 */
        else if (status_run == _IDLE || status_run == _DELAY)
        {
            fg_need_stop = TRUE;
        }
    }

    /* 定时杀菌功能结束后，执行热水关机 */
    if (!fg_timed_disinfect_run && fg_need_stop)
    {
        fg_hotw_off = TRUE;
        fg_need_stop = FALSE;
    }

    /* 未进入定时杀菌功能 */
    if (!fg_timed_disinfect_run)
    {
        fg_enter = FALSE;
    }
}


/*******************************************************************************
 函 数: on_off_force_hotw_mode()
 功 能: 强开热水模式功能开关机
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void on_off_force_hotw_mode(void)
{
    static BOL fg_enter = FALSE;
    static BOL fg_need_stop = FALSE;

    /* 进入强开热水模式功能前 */
    if (fg_force_hotw_mode_run && !fg_enter)
    {
        fg_enter = TRUE;
        
        /* 进入强开热水模式前处于运行态 */
        if (status_run == _READY || status_run == _RUN)
        {
            fg_need_stop = fg_hotw_run ? FALSE : TRUE;		/* 若热水已开机，则不执行关机，否则反之 */
        }
        /* 进入强开热水模式前处于待机，执行关机 */
        else if (status_run == _IDLE || status_run == _DELAY)
        {
            fg_need_stop = TRUE;
        }
    }

    /* 强开热水模式结束后，执行热水关机 */
    if (!fg_force_hotw_mode_run && fg_need_stop)
    {
        fg_hotw_off = TRUE;
        fg_need_stop = FALSE;
    }

    /* 未进入强开热水模式 */
    if (!fg_force_hotw_mode_run)
    {
        fg_enter = FALSE;
    }
}


/*******************************************************************************
 函 数: on_off_floor_preheat()
 功 能: 地暖预热功能开关机
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void on_off_floor_preheat(void)
{
    static BOL fg_enter = FALSE;
    static BOL fg_need_stop = FALSE;

    if (fg_floor_preheat_run && !fg_enter)
    {
        fg_enter = TRUE;
        
        /* 进入地暖预热前处于运行态，不执行关机 */
        if (status_run == _READY || status_run == _RUN)
        {
            fg_need_stop = FALSE;
        }
        /* 进入地暖预热前处于待机，执行关机 */
        else if (status_run == _IDLE || status_run == _DELAY)
        {
            fg_need_stop = TRUE;
        }
    }

    /* 地暖预热功能结束后，执行关机 */
    if (!fg_floor_preheat_run && fg_need_stop)
    {
        fg_zone_A_off = TRUE;
        fg_need_stop = FALSE;
    }

    /* 未进入地暖预热功能 */
    if (!fg_floor_preheat_run)
    {
        fg_enter = FALSE;
    }

}


/*******************************************************************************
 函 数: on_off_floor_dryup()
 功 能: 地暖烘干开关机
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void on_off_floor_dryup(void)
{
    static BOL fg_enter = FALSE;
    static BOL fg_need_stop = FALSE;

    if (fg_floor_dryup_run && !fg_enter)
    {
        fg_enter = TRUE;
        
        /* 进入地暖烘干前处于运行态，不执行关机 */
        if (status_run == _READY || status_run == _RUN)
        {
            fg_need_stop = FALSE;
        }
        /* 进入地暖烘干前处于待机，执行关机 */
        else if (status_run == _IDLE || status_run == _DELAY)
        {
            fg_need_stop = TRUE;
        }
    }

    /* 地暖烘干功能结束后，执行关机 */
    if (!fg_floor_dryup_run && fg_need_stop)
    {
        fg_zone_A_off = TRUE;
        fg_need_stop = FALSE;
    }

    /* 未进入地暖烘干功能 */
    if (!fg_floor_dryup_run)
    {
        fg_enter = FALSE;
    }
}


/*******************************************************************************
 函 数: on_off_holiday_away()
 功 能: 假日离开模式开关机
 参 数: 空
 返 回: 空
 其 它: 空
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
//        /* 进入假日离开模式处于运行态，不执行关机 */
//        if (status_run == _READY || status_run == _RUN)
//        {
//            fg_need_stop = FALSE;
//        }
//        /* 进入假日离开模式处于待机，执行关机 */
//        else if (status_run == _IDLE || status_run == _DELAY)
//        {
//            fg_need_stop = TRUE;
//        }
//    }
//
//    /* 假日离开模式结束后，执行关机 */
//    if (!fg_holiday_away_run && fg_need_stop)
//    {
//        fg_zone_A_off = TRUE;
//        fg_need_stop = FALSE;
//    }
//
//    /* 未进入假日离开模式 */
//    if (!fg_holiday_away_run)
//    {
//        fg_enter = FALSE;
//    }
}

/*******************************************************************************
 函 数: on_off_holiday_home()
 功 能: 假日在家模式开关机
 参 数: 空
 返 回: 空
 其 它: 空
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
//        /* 进入假日在家模式处于运行态，不执行关机 */
//        if (status_run == _READY || status_run == _RUN)
//        {
//            fg_need_stop = FALSE;
//        }
//        /* 进入假日在家模式处于待机，执行关机 */
//        else if (status_run == _IDLE || status_run == _DELAY)
//        {
//            fg_need_stop = TRUE;
//        }
//    }
//
//    /* 假日在家模式结束后，执行关机 */
//    if (!fg_holiday_home_run && fg_need_stop)
//    {
//        fg_zone_A_off = TRUE;
//        fg_need_stop = FALSE;
//    }
//
//    /* 未进入假日在家模式 */
//    if (!fg_holiday_home_run)
//    {
//        fg_enter = FALSE;
//    }
}

