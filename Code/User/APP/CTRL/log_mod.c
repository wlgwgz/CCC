
#define LOG_MOD_GLOBAL

#include "includes.h"

/* ========================================================================================== */

#define ID_CELL     MAX_COMP

#define VALVE_READY         0       /* 膨胀阀准备好 */
#define VALVE_BUSY          1       /* 膨胀阀忙 */

/* ========================================================================================== */
#ifndef USE_QUICK_TIME
#define val_cool_fan_on_dly     10      // 制冷风机开延时
#define val_cool_fan_off_dly    -10     // 制冷风机关延时   
#define val_heat_fan_on_dly     10      // 制热风机开延时
#define val_heat_fan_off_dly    -10     // 制热风机关延时 
#else
#define val_cool_fan_on_dly     (fg_dbg_slow?10:2)       // 制冷风机开延时
#define val_cool_fan_off_dly    (fg_dbg_slow?-10:-2)     // 制冷风机关延时    
#define val_heat_fan_on_dly     (fg_dbg_slow?10:2)       // 制热风机开延时
#define val_heat_fan_off_dly    (fg_dbg_slow?-10:-2)     // 制热风机关延时 
#endif

    
#define byway_val_temp_diff     2*10    // 关旁通环境温度偏差

//U08 fg_defrost_apply[MAX_COMP];             /* 申请除霜标志 */

//U16 frost_space[MAX_COMP];        /* 除霜间隔计时 */
//U16 frost_long[MAX_COMP];     /* 最长除霜间隔计时 */
//U16 cnt_time_fan_delay[MAX_COMP];             /* 结束除霜前开风机延时 */
//U16 frost_time[MAX_COMP];     /* 除霜时间计时 */
//U08 frost_out_dly[MAX_COMP];                  /* 结束除霜的出水温度计时*/

/* ========================================================================================== */
/*============================================================================================*/
//#define S_MODE_COOL               1
//#define S_MODE_HEAT               2
//#define S_MODE_HOT                3
//#define S_MODE_COOL_AND_HOT_PART  4
//#define S_MODE_COOL_AND_HOT           5
/*============================================================================================*/

/* ========================================================================================== */


/* ========================================================================================== */

static BOOL fg_para_init;           /* 参数初始化标志 */
static BOOL fg_limit_init;          /* 系统维护参数初始化标志 */

static BOOL fg_invt_init;           /* 变频器初始化标志 */

static BOOL fg_runtime_init;        /* 参数初始化标志 */
static BOOL fg_systime_init;        /* 系统运行时间初始化标志 */
static BOOL fg_systime_save;        /* 系统运行时间保存标志 */
static BOOL fg_password_init;       // 密码初始化标志

static U08  fg_comp_valve_reset[MAX_COMP];            // 压机对应的膨胀阀正在进行复位操作 (防止EEV还没归零完成，压机就开了) 
static BOOL fg_vavle_reset[EEV_ID_MAX]= {TRUE,TRUE} ; // 电子膨胀阀归零初始化限制标志(停机一次只初始化一次)

//static void s_Logic_StatusIndicate(void);
//U08 sensor_used_temp_out(U08 comp_num);
//U08 sensor_used_temp_cond_out(U08 comp_num);


/* IBH开启条件 */
enum                    
{
    IBH_ON_SIG_RUN_HEAT_COMP_NORMAL        = 0,             /* 制热运行，压机正常状态时开启 */
    IBH_ON_SIG_RUN_HEAT_COMP_KEEP_NORMAL   = 1,             /* 制热运行，压机正常状态保持时开启 */
    IBH_ON_SIG_RUN_HEAT_COMP_ABNORMAL      = 2,             /* 制热运行，压机异常状态时开启 */
    IBH_ON_SIG_RUN_HEAT_COMP_KEEP_ABNORMAL = 3,             /* 制热运行，压机异常状态保持时开启 */
    IBH_ON_SIG_RUN_HOTW_COMP_NORMAL        = 4,             /* 制热水运行，压机正常状态时开启 */
    IBH_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL   = 5,             /* 制热水运行，压机正常状态保持时开启 */
    IBH_ON_SIG_RUN_HOTW_COMP_ABNORMAL      = 6,             /* 制热水运行，压机异常状态时开启 */
    IBH_ON_SIG_RUN_HOTW_COMP_KEEP_ABNORMAL = 7,             /* 制热水运行，压机异常状态保持时开启 */
    IBH_ON_SIG_IDLE_STA_AC_ANTIFREEZE      = 8,             /* 待机态空调水路防冻时开启 */
    IBH_ON_SIG_IDLE_STA_HOTW_ANTIFREEZE    = 9,             /* 待机态热水水路防冻时开启 */
    IBH_ON_SIG_RUN_STA_AC_ANTIFREEZE       = 10,            /* 运行态空调水路防冻时开启 */
    IBH_ON_SIG_RUN_STA_HOTW_ANTIFREEZE     = 11,            /* 运行态热水水路防冻时开启 */
    IBH_ON_SIG_RUN_HEAT_DEFROST            = 12,            /* 制热除霜时开启 */
    IBH_ON_SIG_RUN_HOTW_DEFROST            = 13,            /* 制热水除霜时开启 */
    IBH_ON_SIG_FORCE_HOTW                  = 14,            /* 强开热水模式时开启 */
    IBH_ON_SIG_RUN_HEAT_FORCE_ON           = 15,            /* 制热运行，强开辅助电热时开启 */
    IBH_ON_SIG_RUN_HOTW_FORCE_ON           = 16,            /* 制热水运行，强开辅助电热时开启 */
    IBH_ON_SIG_DISINFECT                   = 17,            /* 定时杀菌时开启 */
    IBH_ON_SIG_TF                          = 18,            /* 商检时开启 */
    IBH_ON_SIG_TEST_RUN_HEAT_MODE_ON       = 19,            /* 制热试运行，辅助电热开启 */
    IBH_ON_SIG_MAX
};
static U16 fg_IBH_on[_DIVUCARRY(IBH_ON_SIG_MAX, 16)];



/* ========================================================================================== */

/* 以下内容与模块类型相关，不同类型的模块请按要求分别填写 */
/*-----------------------模块定制开始------------------------------------*/
/*****************************************************
函数： s_comp_can_use
功能： 判断压机在各模式下的使用状态
参数： 压机号
返回值： 压机使用状态(模式对应)
        说明： 返回值的每一位表示一种模式的可使用状态，模式对应顺序如下表：
        ------------------------------------------------------------------
        bit0| bit1| bit2|          bit3|        bit4|    bit5| bit6| bit7
        制冷| 制热| 热水| 制冷+部分热水| 制冷+全热水| 送水/风| 加湿| 除湿
        ------------------------------------------------------------------
        1: 可用
        0: 不可用
******************************************************/
U08 s_comp_can_use(U08 comp_num)
{
    U08 comp_can_use = 0xff;     /* 初始化为所有模式可用 */
                    
    if (mod_err_comp[comp_num])       /* 压机故障(空调模式不可开) */
    {
        comp_can_use &= 0xfc;
    }

    comp_can_use &= 0x03;                 /* 屏蔽该模块不存在的模式 */

    return comp_can_use;
}
U08 s_have_comp_defrost(void)
{
    U08 comp_defrost = FALSE;
    U08 i;
                    
    for (i=0; i<self_comp_num; i++)
    {
        if (s_get_cm_status(i, CM_NEED_DEFROST) && s_get_cm_status(i, CM_DEFROST_ALLOW))
        {
            comp_defrost = TRUE;
            break;
        }
    }

    return comp_defrost;
}

/*******************************************************************************
 函 数: log_mod_cell_have_mode()
 功 能: 判断模块是否是否有指定的运行模式
 参 数: mode--模式; 
 返 回: TRUE/FALSE--有指定模式/无指定模式
 其 它: 空
*******************************************************************************/
BOOL log_mod_cell_have_mode(U08 mode)
{
    U08 i;
    BOOL cell_have_mode = FALSE;
    
    for (i=0; i<val_comp_num; i++)
    {
        if (Cm[i].run_mode == mode)
        {
            cell_have_mode = TRUE;
            break;
        }
    }
    
    return cell_have_mode;
}

/*********************************************
函数：Logic_eev_sv_cool
功能：制热时吸气过热度目标值计算
参数：压机序号
返回值：过热度目标值
**********************************************/
I16 Logic_eev_sv_cool(I16 eva_side_temp)
{
    I16 pid_sv;
    
    if (SensorCannotUse(eva_side_temp))    /* 蒸发侧探头故障，取固定过热度目标值 */
    {
        pid_sv = val_motor_sv_cool1;            /* 初始化 */
    }
    else 
    {
        static U08 env_area;

        env_area = fun_area(pv_cell_TA(pcb_addr), env_area, 5, val_cool_sv_chg_env, SENSOR_NOEXIST);

        switch (env_area)
        {
            default:
            case AREA_00:
                switch(in_area_cool)
                {
                    default:
                    case AREA_00:       
                        pid_sv = val_motor_sv_cool1;
                        break;
                    case AREA_01:   
                        pid_sv = val_motor_sv_cool2;
                        break;
                    case AREA_02:
                        pid_sv = val_motor_sv_cool3;
                        break;
                }
                break;
            case AREA_01:
                switch(in_area_cool)
                {
                    default:
                    case AREA_00:       
                        pid_sv = val_low_env_sv_cool1;
                        break;
                    case AREA_01:   
                        pid_sv = val_low_env_sv_cool2;
                        break;
                    case AREA_02:
                        pid_sv = val_low_env_sv_cool3;
                        break;
                }
                break;
        }
    }
    
    return pid_sv;
}


/*********************************************
函数：Logic_eev_sv_heat
功能：制热时吸气过热度目标值计算
参数：压机序号
返回值：过热度目标值
**********************************************/
I16 Logic_eev_sv_heat(I16 eva_side_temp)
{
    I16 pid_sv;
    
    if (SensorCannotUse(eva_side_temp))   /* 蒸发侧探头故障，取固定过热度目标值 */
    {
        pid_sv = val_motor_sv_heat1;
    }
    else 
    {
#ifdef USE_HT_ENV_NEW
        switch(env_area_heat)
        {
            default:
            case AREA_00:       
                pid_sv = val_motor_sv_heat1;
                break;
            case AREA_01:   
                pid_sv = val_motor_sv_heat2;
                break;
            case AREA_02:
                pid_sv = val_motor_sv_heat3;
                break;
            case AREA_03:
                pid_sv = val_motor_sv_heat4;
                break;
            case AREA_04:
                pid_sv = val_motor_sv_heat5;
                break;
            case AREA_05:   
                pid_sv = val_motor_sv_heat6;
                break;
            case AREA_06:
                pid_sv = val_motor_sv_heat7;
                break;
            case AREA_07:
                pid_sv = val_motor_sv_heat8;
                break;
            case AREA_08:
                pid_sv = val_motor_sv_heat9;
                break;
            case AREA_09:
                pid_sv = val_motor_sv_heat10;
                break;
        }
#else
        switch(env_area_heat)
        {
            default:
            case AREA_00:       
                pid_sv = val_motor_sv_heat1;
                break;
            case AREA_01:   
                pid_sv = val_motor_sv_heat2;
                break;
            case AREA_02:
                pid_sv = val_motor_sv_heat3;
                break;
            case AREA_03:
                pid_sv = val_motor_sv_heat4;
                break;
            case AREA_04:
                pid_sv = val_motor_sv_heat5;
                break;
            case AREA_05:   
                pid_sv = val_motor_sv_heat6;
                break;
            case AREA_06:
                pid_sv = val_motor_sv_heat7;
                break;
            case AREA_07:
                pid_sv = val_motor_sv_heat8;
                break;
            case AREA_08:
                pid_sv = val_motor_sv_heat9;
                break;
        }
#endif
    }
    
    return pid_sv;
}

/*******************************************************************************
 函 数: log_mod_get_temp_cond()
 功 能: 获取冷凝温度
 参 数: x--; 
 返 回: 
 其 它: 空
*******************************************************************************/
U16 log_mod_get_temp_cond(U08 x)
{
    I16 temp_cond = SENSOR_NOEXIST;
    I16 wing_temp = pv_cell_wing(x);
    I16 tmp_out = pv_cell_TWout1(pcb_addr);
    
    if (val_s_motor_use==EVI_EXH_OVERHEAT_PRS && SensorCanUse(pv_cell_cond_temp(x)))
    {
        temp_cond = pv_cell_cond_temp(x);
    }
    else
    {
        if (CmIsCool(x))    temp_cond = wing_temp;
        else                temp_cond = tmp_out;
    }

    return temp_cond;
}

/*********************************************
函数：Logic_eev_step_min
功能：获取主阀最小开度
参数：
返回值：最小开度%
**********************************************/
I16 Logic_eev_step_min(U08 x)
{
    I16 result;

    I16 temp_cond = log_mod_get_temp_cond(x);
    I16 temp_exh = pv_gas_out_high(x);
    I16 exh_sh = temp_exh - temp_cond;

    static U08 area = AREA_00;

    if (CmIsCool(x))        /* 制冷 */
    {
        area = fun_area(exh_sh, area, 20, 
                        380, 320, 260, 200,
                        SENSOR_NOEXIST);
        
        switch(area)
        {
            case AREA_00:       
            case AREA_01:   
            case AREA_02:
            case AREA_03:
            case AREA_04:
                result = val_motor_step_min_incl(area);
                break;
            default:
                result = val_motor_step_min_incl1;
                break;
        }
    }
    else                    /* 制热 */
    {
        area = fun_area(exh_sh, area, 20, 
                        380, 320, 260, 200,
                        SENSOR_NOEXIST);
        
        switch(area)
        {
            case AREA_00:       
            case AREA_01:   
            case AREA_02:
            case AREA_03:
            case AREA_04:
                result = val_motor_step_min_inht(area);
                break;
            default:
                result = val_motor_step_min_inht1;
                break;
        }
    }

    return result;
}

/*******************************************************************************
 函 数: log_mod_comp_gb_freq_heat()
 功 能: 国标调试：获取当前干球温度下压机制热运行频率
 参 数: env_temp--干球温度即环境温度; 
 返 回: 运行频率
 其 它: 空
*******************************************************************************/
I16 log_mod_comp_gb_freq_heat(I16 env_temp)
{
    I16 result;
    static U08 area;
    
    if (SensorCannotUse(env_temp))    /* 蒸发侧探头故障，取固定过热度目标值 */
    {
        result = val_invt_rps_heatrun_freq1;            /* 初始化 */
    }
    else 
    {
        area = fun_area(env_temp, area, 10, 30, -30, -90, SENSOR_NOEXIST);

        switch(area)
        {
            default:
            case AREA_00:       /*  */
                result = val_invt_rps_heatrun_freq1;
                break;
            case AREA_01:       /* */
                result = val_invt_rps_heatrun_freq2;
                break;
            case AREA_02:       /* */
                result = val_invt_rps_heatrun_freq3;
                break;
            case AREA_03:       /* */
                result = val_invt_rps_heatrun_freq4;
                break;
        }
    }

    return result;
}

/*******************************************************************************
 函 数: log_mod_comp_gb_freq_cool()
 功 能: 国标调试：获取当前干球温度下压机制冷运行频率
 参 数: env_temp--干球温度即环境温度; 
 返 回: 运行频率
 其 它: 空
*******************************************************************************/
I16 log_mod_comp_gb_freq_cool(I16 env_temp)
{
    I16 result;
    static U08 area;
    
    if (SensorCannotUse(env_temp))    /* 蒸发侧探头故障，取固定过热度目标值 */
    {
        result = val_invt_rps_coolrun_freq1;            /* 初始化 */
    }
    else 
    {
        area = fun_area(env_temp, area, 5, 332, 298, 263, SENSOR_NOEXIST);
                    
        switch(area)
        {
            default:
            case AREA_00:       /*  */
                result = val_invt_rps_coolrun_freq1;
                break;
            case AREA_01:       /* */
                result = val_invt_rps_coolrun_freq2;
                break;
            case AREA_02:       /* */
                result = val_invt_rps_coolrun_freq3;
                break;
            case AREA_03:       /* */
                result = val_invt_rps_coolrun_freq4;
                break;
        }
    }

    return result;
}

/*******************************************************************************
 函 数: log_mod_comp_gb_sv_heat()
 功 能: 国标调试：获取当前干球温度下压机制热过热度
 参 数: env_temp--干球温度即环境温度; 
 返 回: 运行频率
 其 它: 空
*******************************************************************************/
I16 log_mod_comp_gb_sv_heat(I16 env_temp)
{
    I16 result;
    static U08 area;
    
    if (SensorCannotUse(env_temp))    /* 蒸发侧探头故障，取固定过热度目标值 */
    {
        result = val_debug_sh_sv1;          /* 初始化 */
    }
    else 
    {
        area = fun_area(env_temp, area, 10, 30, -30, -90, SENSOR_NOEXIST);

        switch(area)
        {
            default:
            case AREA_00:       /*  */
                result = val_debug_sh_sv1;
                break;
            case AREA_01:       /* */
                result = val_debug_sh_sv2;
                break;
            case AREA_02:       /* */
                result = val_debug_sh_sv3;
                break;
            case AREA_03:       /* */
                result = val_debug_sh_sv4;
                break;
        }
    }

    return result;
}

/*******************************************************************************
 函 数: log_mod_comp_gb_sv_cool()
 功 能: 国标调试：获取当前干球温度下压机制冷过热度
 参 数: env_temp--干球温度即环境温度; 
 返 回: 运行频率
 其 它: 空
*******************************************************************************/
I16 log_mod_comp_gb_sv_cool(I16 env_temp)
{
    I16 result;
    static U08 area;
    
    if (SensorCannotUse(env_temp))    /* 蒸发侧探头故障，取固定过热度目标值 */
    {
        result = val_debug_sc_sv1;          /* 初始化 */
    }
    else 
    {
        area = fun_area(env_temp, area, 5, 332, 298, 263, SENSOR_NOEXIST);
                    
        switch(area)
        {
            default:
            case AREA_00:       /*  */
                result = val_debug_sc_sv1;
                break;
            case AREA_01:       /* */
                result = val_debug_sc_sv2;
                break;
            case AREA_02:       /* */
                result = val_debug_sc_sv3;
                break;
            case AREA_03:       /* */
                result = val_debug_sc_sv4;
                break;
        }
    }

    return result;
}

/*******************************************************************************
 函 数: calculate_gb_dbg_freq()
 功 能: 计算当前调试模式下频率值
 参 数: comp_num--; 
        Mode--; 
        NowFreNeed--; 
 返 回: 
 其 它: 空
*******************************************************************************/
U16 calculate_gb_dbg_freq(U08 Mode,U16 NowFreNeed) 
{
    U16 tmp = NowFreNeed;
    I16 temp_env = pv_cell_TA(pcb_addr);

    if (USE_GB_18430)
    {
        if (Mode==MODE_HEAT)
        {
            tmp = val_invt_rps_sheatrun_freq;
        }
        else 
        {
            tmp = log_mod_comp_gb_freq_cool(temp_env);
        }
        
    }
    else if (USE_GB_25127)
    {
        if (Mode==MODE_HEAT)
        {
            tmp = log_mod_comp_gb_freq_heat(temp_env);
        }
        else 
        {
           tmp = log_mod_comp_gb_freq_cool(temp_env);
        }
    }
    return tmp;

}

/*******************************************************************************
 函 数: calculate_gb_dbg_sv()
 功 能: 计算当前调试模式下过热度
 参 数: Mode--; 
        NowSNeed--; 
 返 回: 
 其 它: 空
*******************************************************************************/
I16 calculate_gb_dbg_sv(U08 Mode,I16 NowSvNeed) 
{
    U16 tmp = NowSvNeed;
    I16 temp_env = pv_cell_TA(pcb_addr);

    if(USE_GB_18430)
    {
        if(Mode==MODE_HEAT)
        {
            tmp = val_debug_ssh_sv;
        }
        else 
        {
            tmp = log_mod_comp_gb_sv_cool(temp_env);
        }
    }
    else if(USE_GB_25127)
    {
        if(Mode==MODE_HEAT)
        {
            tmp = log_mod_comp_gb_sv_heat(temp_env);
        }
        else 
        {
           tmp = log_mod_comp_gb_sv_cool(temp_env);
        }
    }
    return tmp;
}

/*******************************************************************************
 函 数: log_mod_init()
 功 能: 模块变量初始化
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_mod_init(void)
{
    U08  x;
    
    pCell = &cell[pcb_addr];
    
    fg_err_reset = TRUE;
    
    for (x=0; x<MAX_COMP; x++)
    {
        cm_time[x].off = 0;     /* 停机时间初始化 */
        cm_time[x].defrost_exit = 0xffff;       /* 共同退出除霜时间初始化 */
        cm_time[x].defrost_exit_sin = 0xffff;       /* 单独退出除霜时间初始化 */
        cm_time[x].run = 0;     /* 运行时间初始化 */
//      valve_time[x].off = 0xffff; 
//      valve_time[x].on = 0;
        fg_defrost_apply[x] = 0;
        cm_time[x].cnt_time_fan_delay = 0xffff;
        cm_time[x].defrost_space = 0;       /* 除霜间隔初始化 */

        fg_comp_valve_reset[x] = VALVE_BUSY;        /* 初始化为膨胀阀忙 */

        ARRSET(mod_rec_gas_out[x], SENSOR_NOEXIST);

        _SETMAX(cnt_reoil_end[x]);
    }
    
    
    for (x=0; x<MAX_COMP+1; x++)
    {
        Cm[x].status = 0;
        
        Cm[x].run_mode = MODE_NULL;         /* 运行模式初始化 */
        Cm[x].run_mode_need = 0xff;         /* 运行模式需求初始化 */
            
    }

    fg_cell_pump_open = FALSE;
    fg_force_IBH_off = FALSE;

    PUMPF_init();

//    runtime_read_from_eeprom();    /* 读取压机运行时间 */
    DataAccess_CompRuntime(DATA_READ);/* 读取压机运行时间 */
}

/*******************************************************************************
 函 数: log_mod_pump_is_run()
 功 能: 模块泵稳定运行标志
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
BOL log_mod_pump_is_run(void)
{
    BOL fg_result = FALSE;

    if (pCell->cell_status[MAX_COMP] & CELL_PUMP_RUN)
    {
        fg_result = TRUE;
    }
    return fg_result;
}

/*****************************************
    函数：s_cell_is_err
    功能：判断模块是否故障
    参数：无
    返回值：TRUE: 模块故障; FALSE: 模块未故障
******************************************/
BOOL log_mod_cell_is_err(void)
{
    U08 i;
    
    BOOL cell_is_err = FALSE;
    
    for (i=0; i<MAX_ERR; i++)
    {
        if (pCell->cell_error[i])
        {
            cell_is_err = TRUE;
            break;
        }
    }

    
    return cell_is_err;
}

/*****************************************
    函数：s_cell_have_mode
    功能：判断模块是否是否有指定的运行模式
    参数：指定模式
    返回值：TRUE: 包含指定模式; FALSE: 无指定模式
******************************************/
BOOL s_cell_have_mode(U08 mode)
{
    U08 i;
    BOOL cell_have_mode = FALSE;
    
    for (i=0; i<val_comp_num; i++)
    {
        if (Cm[i].run_mode == mode)
        {
            cell_have_mode = TRUE;
            break;
        }
    }
    
    return cell_have_mode;
}
/*******************************************************************************
 函 数: log_mod_get_comp_limit_out_up()
 功 能: 获取出温上限值
 参 数: area--环温分段区间; 
 返 回: 
 其 它: 空
*******************************************************************************/
I16 log_mod_get_comp_limit_out_up(I16 temp_env)
{
    I16 result = 0; 
    static U08 area_env = AREA_00;

	area_env = fun_area(temp_env, area_env, 5,
						val_heat_chg_env1, val_heat_chg_env2,
						val_heat_chg_env3, val_heat_chg_env4,
						val_heat_chg_env5, SENSOR_NOEXIST);
	
    switch(area_env)
    {
        default:
        case AREA_00:          
            result = val_TWout1_limit_1; 
            break;
            
        case AREA_01:
            result = fun_line_value(temp_env, 
                                    val_heat_chg_env1, val_heat_chg_env2, 
                                    val_TWout1_limit_1, val_TWout1_limit_2);
            break;
            
        case AREA_02:
            result = fun_line_value(temp_env, 
                                    val_heat_chg_env2, val_heat_chg_env3, 
                                    val_TWout1_limit_2, val_TWout1_limit_3);
            break;
            
        case AREA_03:
            result = fun_line_value(temp_env, 
                                    val_heat_chg_env3, val_heat_chg_env4, 
                                    val_TWout1_limit_3, val_TWout1_limit_4);
            break;
            
        case AREA_04:
            result = fun_line_value(temp_env, 
                                    val_heat_chg_env4, val_heat_chg_env5, 
                                    val_TWout1_limit_4, val_TWout1_limit_5);
            break;
        
        case AREA_05:
            result = val_TWout1_limit_5; 
            break;
    }

    return result;
}
#ifdef USE_OUT_UP_DBG
/*******************************************************************************
 函 数: log_mod_get_comp_limit_out_up_UT()
 功 能: 单元测试：log_mod_get_comp_limit_out_up
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_mod_get_comp_limit_out_up_UT(void)
{
    I16 env_ht[] = {430, 300, 200, 70, 0, 
                    -70, -120, -150, -180, -250};
    static I16 limit[10];
    U08 a;
    
    for (a=0; a<ARRELE(env_ht); a++)
    {
		limit[a] = log_mod_get_comp_limit_out_up(env_ht[a]);
    }
}  
#endif
/*******************************************************************************
 函 数: log_mod_comp_temp_out_limit()
 功 能: 出温限制
 参 数: x--压机序号; 
 返 回: TRUE/FALSE--是/否
 其 它: 
*******************************************************************************/
BOOL log_mod_comp_temp_out_limit(U08 x)
{
    #define IN_DIFF         (20)
    #define OUT_DIFF        (IN_DIFF+50)
        
    static BOL fg_limit[MAX_COMP];
    I16 temp_out = pv_cell_TWout1(pcb_addr);
//    I16 temp_in = pv_cell_TWin1(pcb_addr);

    if (SensorCannotUse(temp_out)
//    ||  SensorCannotUse(temp_in)
    )
    {
        ARRCLR(fg_limit);

        return FALSE;
    }

    if (Cm[x].run_mode == MODE_COOL)
    {
        I16 out_down = val_TWout1_limit_cool;
//        I16 in_down = out_down+50;

        if (temp_out < out_down
//        ||  temp_in < in_down
        )
        {
            fg_limit[x] = TRUE;
        }
        else if (temp_out > out_down+OUT_DIFF
//             &&  temp_in > in_down+IN_DIFF
             )
        {
            fg_limit[x] = FALSE;
        }
    }
    else
    {
        I16 out_up = log_mod_get_comp_limit_out_up(pv_cell_TA(pcb_addr));
//        I16 in_up = out_up-50;

        if (temp_out > out_up
//        ||  temp_in > in_up
        )
        {
            fg_limit[x] = TRUE;
        }
        else if (temp_out < out_up-OUT_DIFF
//             &&  temp_in < in_up-IN_DIFF
             )
        {
            fg_limit[x] = FALSE;
        }
    }

#ifdef USE_OUT_UP_DBG
    log_mod_get_comp_limit_out_up_UT();
#endif

    return fg_limit[x];
}

/*****************************************************
函数： log_mod_comp_can_open
功能： 压机是否可以开
参数： 压机号
返回值： TRUE: 压机可开; FALSE: 压机不可开      
******************************************************/
BOOL log_mod_comp_can_open(U08 comp_num)
{
    BOOL fg_comp_can_open = FALSE; 
    U16 cm_off_time = val_cm_off_time+CM_off_speed_down_time;

    /* 防频繁启动特殊处理 */
    if (!TF_IS_IDLE			/* 商检 */
    ||  fg_refri_recy_act	/* 收氟 */
    )
    {
        cm_off_time = 0;
    }

    if (log_mod_pump_is_run())				        /* 变频水泵稳定运行 */
    if (cm_time[comp_num].off > cm_off_time)        /* 防频繁启动时间到 */
    if (!s_get_cm_status(comp_num, CM_ON))          /* 压机未开启 */
    if (!mod_err_comp[comp_num] && fg_comp_valve_reset[comp_num] == VALVE_READY) //NOTE-CXW 待测2017-5-12
    if (GET_EXIO_HDW_STATUS_READY(0))
    if (!log_mod_comp_temp_out_limit(comp_num))
    if (TA_ALLOW_COMP_ON_OFF)
    {
        fg_comp_can_open = TRUE;
    }

    return fg_comp_can_open;
}

/*****************************************************
函数： log_mod_comp_can_close
功能： 压机是否可以关(指正常卸载)
参数： 压机号
返回值： TRUE: 压机可关; FALSE: 压机不可关
        
******************************************************/
BOOL log_mod_comp_can_close(U08 comp_num)
{
    BOOL fg_comp_can_close = FALSE;  
    
    if (cm_time[comp_num].on > COMP_NEED_ON_TIME)                 /* 压机最小运行时间到 */
    if (s_get_cm_status(comp_num, CM_NEED))                       /* 压机已开启 */
    if (!s_get_cm_status(comp_num, CM_DEFROST))                   /* 压机未除霜 */
    {
        fg_comp_can_close = TRUE;
    }
    
    return fg_comp_can_close;
}

/*****************************************************
函数： log_mod_comp_can_quick_stop
功能： 压机是否可以急停
参数： 压机号
返回值： TRUE: 压机可急停; FALSE: 压机不可急停
        
******************************************************/
BOOL log_mod_comp_can_quick_stop(U08 comp_num)
{
    BOOL fg_comp_can_quick_stop = FALSE;  

    if (s_get_cm_status(comp_num, CM_NEED))                       /* 压机已开启 */
    {
        fg_comp_can_quick_stop = TRUE;
    }
    
    return fg_comp_can_quick_stop;
}
/***************************************************
    函数：s_comp_is_run
    功能：判断是否有压机运行
    参数：运行模式(表示寻找以何种模式运行的压机)
    返回值：TRUE: 有压机运行; FALSE: 无压机运行
****************************************************/
BOOL log_mod_comp_is_run(U08 run_type)
{
    U08 i ;
    BOOL fg_have_comp_run = FALSE;

    for (i=0; i<self_comp_num; i++)
    {
        if (s_get_cm_status(i, CM_NEED))
        {
            if (run_type == MODE_NULL               /* 不限制模式 */
                || run_type == Cm[i].run_mode)      /* 模式匹配 */
            {
                fg_have_comp_run = TRUE;
                break;
            }
        }
    }

    return fg_have_comp_run;
}
/*****************************************
    函数： log_mod_cell_is_run
    功能： 判断模块是否在运行
    参数： 运行模式(表示寻找以何种模式运行的压机)
    返回值： TRUE: 模块运行; FALSE: 模块未运行
******************************************/
BOOL log_mod_cell_is_run(U08 run_type)
{
    BOOL cell_is_run = FALSE;
    
    if (log_mod_comp_is_run(run_type))
    {
        cell_is_run = TRUE;
    }
    
    return cell_is_run;
}
/*-------------------------模块定制结束-----------------------------------*/

/********************************************************
函数：s_comp_add_sub_prio
功能：判断当前压机是否具有加/卸载优先级
参数：压机序号
返回值：TRUE: 有优先级; FALSE: 无优先级
*********************************************************/
BOOL log_mod_comp_prio(U08 comp_num, U08 prio_type)
{
    BOOL fg_have_prio = FALSE;
    U08 i;

    comp_num = comp_num;    /* N0001 */

    for (i=0; i<self_comp_num; i++ )
    {
        if (prio_type == _ADD)
        {
            if (s_get_output(i, SW_CM) || s_get_cm_status(i, CM_NEED))  /* 同组有压机运行，加载优先 */
            {
                fg_have_prio = TRUE;
                break;
            }
        }
        else if (prio_type == _SUBB)
        {
            if (!(s_get_output(i, SW_CM) || s_get_cm_status(i, CM_NEED)))   /* 同组有压机未运行，卸载优先 */
            {
                fg_have_prio = TRUE;
                break;
            }
        }   
    }
            
    
    return fg_have_prio;
}

/*******************************************************************************
 函 数: log_mod_get_rfg_temp_corr()
 功 能: 冷媒温度修正逻辑
 参 数: 空
 返 回: 温度修正值
 其 它: 空
*******************************************************************************/
I16 log_mod_get_rfg_temp_corr(void)
{
    static BOOL fg_first_corr = TRUE;
    static U08 t2b_area = AREA_MAX;
    static U08 t2_area = AREA_MAX;

    U08 cm = 0;

    /* 不使用冷媒温度修正 */
    if (val_rfg_temp_corr_en == UNUSED)
    {
        fg_first_corr = TRUE;
        t2b_area = AREA_00;
        t2_area = AREA_00;
        rfg_corr_interval = 0;
        return 0;
    }
 
    if (CmIsCool(cm))
    {
        if (cm_time[cm].on < 60 && !fg_first_corr)
        {
            rfg_corr_interval = 0;
        }
        
        if (cm_time[cm].on >= 60
        && rfg_corr_interval >= 120)
        {
            fg_first_corr = FALSE;
            rfg_corr_interval = 0;
            
            /* 冷媒气侧温度分段 */
            t2b_area = fun_area(pv_cell_TRFgas(0), t2b_area, 10, \
                                160, 140, 80, 60, SENSOR_NOEXIST);
        }
        
        switch (t2b_area)
        {
            default:       return 0;
            case AREA_00:  return val_rfg_vap_corr_1;
            case AREA_01:  return val_rfg_vap_corr_2;
            case AREA_02:  return val_rfg_vap_corr_3;
            case AREA_03:  return val_rfg_vap_corr_4;
            case AREA_04:  return val_rfg_vap_corr_5;
        }
    }
    else
    {       
        if (cm_time[cm].on < 120 && !fg_first_corr)
        {
            rfg_corr_interval = 0;
        }
    
        if (cm_time[cm].on >= 120
        && rfg_corr_interval >= 120)
        {
            fg_first_corr = FALSE;
            rfg_corr_interval = 0;
         
            /* 冷媒液侧温度分段 */
            t2_area = fun_area(pv_cell_TRFliq(0), t2_area, 10, \
                               540, 520, 500, 460, 430, 400, SENSOR_NOEXIST);
        }
        
        switch (t2_area)
        {
            default:       return 0;
            case AREA_00:  return val_rfg_liq_corr_1;
            case AREA_01:  return val_rfg_liq_corr_2;
            case AREA_02:  return val_rfg_liq_corr_3;
            case AREA_03:  return val_rfg_liq_corr_4;
            case AREA_04:  return val_rfg_liq_corr_5;
            case AREA_05:  return val_rfg_liq_corr_6;
            case AREA_06:  return val_rfg_liq_corr_7;
        }
    }
}

/*****************************************************************************
函  数 : log_mod_get_freq_max()
功  能 : 获取频率上限
参  数 : I16 temp_env : 环温
         I16 temp_in : 回温
返  回 : 压机频率上限
其  他 : 频率上限总调用接口，包含各种处理
*****************************************************************************/
I16 log_mod_get_freq_max(U08 mode, I16 temp_env, I16 temp_in)
{
    I16 freq_up = wf_get_freq_max_by_env_in(mode, temp_env, temp_in);   /* 工况频率表 */

    /* 功率模式 */
    {
        #define val_invt_rps_up_min (100*55)    /* 最小上限转速 */
        I16 freq_adj = ModeIsEco    
                       ? val_invt_rps_adj_eco
                       : ModeIsStr
                            ? val_invt_rps_adj_strong
                            : val_invt_rps_adj_std;

        /* 冷热调整系数 */
        if (mode == MODE_COOL)
        {
            freq_adj = (I32)freq_adj*val_cl_ht_adj_ratio/100;
        }
        
        if (freq_adj < 0)
        {
            if (freq_up > val_invt_rps_up_min)
            {
                freq_up += freq_adj;
                freq_up = fun_pv_max(freq_up, val_invt_rps_up_min);
            }
        }
        else
        {
            freq_up += freq_adj;
        }
    }

    /* 冷媒气测、液侧温度限制 */
    {
        freq_up += log_mod_get_rfg_temp_corr();
    }

    /* 静音模式限速：2022-1-21-待优化：权限应高于除霜、回油等，暂放到此处执行 */
    {
        BOL fg_silent = fg_silent_mode;
        if (fg_silent)
        {
            switch (silent_mode_level)
            {
                default:
                case SILENT_LEVEL_1:
                    freq_up = MIN(freq_up, val_invt_rps_silent_max);
                    break;
                case SILENT_LEVEL_2:
                    freq_up = MIN(freq_up, val_invt_rps_str_silent_max);
                    break;
            }
        }
    }

    /* 变频器频率限定 */
    freq_up = MIN(freq_up, val_invt_rps_max);

    return freq_up;
}

/*****************************************************************************
函  数 : log_mod_get_freq_min()
功  能 : 获取频率下限
参  数 : I16 temp_env : 环温
         I16 temp_in : 回温
返  回 : 压机频率下限
其  他 : 频率下限总调用接口，包含各种处理
*****************************************************************************/
I16 log_mod_get_freq_min(U08 mode, I16 temp_env, I16 temp_in)
{
    I16 freq_dn = wf_get_freq_min_by_env_in(mode, temp_env, temp_in);   /* 工况频率表 */

    /* 变频器频率限定 */
    freq_dn = MAX(freq_dn, val_invt_rps_min);

    return freq_dn;
}

/*******************************************************************************
 函 数: log_mod_comp_get_temp_cond()
 功 能: 压机获取冷凝温度，用于限降频
 参 数: x--压机号; 
 返 回: 
 其 它: 空
*******************************************************************************/
U16 log_mod_comp_get_temp_cond(U08 x)
{
    I16 temp_cond = SENSOR_NOEXIST;

#ifdef USE_FREQ_LIMIT_TC_TE
    if (CmIsHeat(x))
    {
        /* 冷凝温度：冷凝饱和温度 或 板换出水温度（不使用高压传感器时） */
        temp_cond = fun_pv_get(pv_cell_cond_temp(x), pv_cell_TWout1(pcb_addr));
    }

    if (CmIsCool(x))
    {
        /* 冷凝温度：冷凝饱和温度 或 翅片温度（不使用低压传感器时） */
        temp_cond = fun_pv_get(pv_cell_cond_temp(x), pv_cell_wing(x));              
    }
#else
    /* 不影响已出货产品，沿用旧逻辑 */
    if (CmIsHeat(x))
    {
        temp_cond = pv_cell_TWout1(pcb_addr);    /* 冷凝温度：固定为板换出水温度 */
    }

    if (CmIsCool(x))
    {
        temp_cond = pv_cell_wing(x);             /* 冷凝温度：固定为翅片温度 */           
    }
#endif

    return temp_cond;
}

/*******************************************************************************
 函 数: log_mod_comp_get_temp_evap()
 功 能: 压机获取蒸发温度，用于限降频
 参 数: x--压机号; 
 返 回: 
 其 它: 空
*******************************************************************************/
U16 log_mod_comp_get_temp_evap(U08 x)
{
    I16 temp_evap = SENSOR_NOEXIST;
    
#ifdef USE_FREQ_LIMIT_TC_TE
    if (CmIsHeat(x))
    {
        /* 蒸发温度：蒸发饱和温度 或 翅片温度（不使用低压传感器时） */
        temp_evap = fun_pv_get(pv_cell_evap_temp(x), pv_cell_wing(x));
    }

    if (CmIsCool(x))
    {
        /* 蒸发温度：蒸发饱和温度 或 阀后温度（不使用低压传感器时） */
        temp_evap = fun_pv_get(pv_cell_evap_temp(x), pv_cell_evap(x));              
    }
#else
    /* 不影响已出货产品，沿用旧逻辑 */
    if (CmIsHeat(x))
    {      
        temp_evap = pv_cell_wing(x);    /* 蒸发温度：固定为翅片温度 */
    }

    if (CmIsCool(x))
    {
        /* 蒸发温度：蒸发饱和温度 或 阀后温度（不使用低压传感器时） */
        temp_evap = fun_pv_get(pv_cell_evap_temp(x), pv_cell_evap(x));
    }
#endif

    return temp_evap;
}

/*******************************************************************************
 函 数: log_mod_comp_can_add()
 功 能: 压机可升频
 参 数: comp_num--; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL log_mod_comp_can_add(U08 cm)
{
    BOOL fg_can_add = FALSE;
    I16 temp_env = pv_cell_TA(pcb_addr);
    I16 temp_in = pv_cell_TWin1(pcb_addr);
    
    if (S_CM_GetStatus(cm, CM_NEED)
    && !USE_FIX_COMP		                                                    /* 变频压机 */
    && CmRotation[cm] < log_mod_get_freq_max(Cm[cm].run_mode, temp_env, temp_in)/* 频率小于最高频率 */
    && !S_CM_GetStatus(cm, CM_DEFROST)		                                    /* 非除霜 */
    && !fg_idle_sta_antifreeze				                                    /* 非待机态防冻:使防冻时更易于开启压机 */
    && !fg_forbid_rise[cm]                                                      /* 非主控限频 */
    && !fg_force_drop[cm]                                                       /* 非主控降频 */
    && cnt_no_freq_limit[cm]>10                                                 /* 非驱动限降频 */
    )
    {
        fg_can_add = TRUE;
    }

    return fg_can_add;
}

/*******************************************************************************
 函 数: log_mod_comp_can_sub()
 功 能: 压机可降频
 参 数: comp_num--; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL log_mod_comp_can_sub(U08 cm)
{
    BOOL fg_can_sub = FALSE;
    I16 temp_env = pv_cell_TA(pcb_addr);
    I16 temp_in = pv_cell_TWin1(pcb_addr);
    
    if (   S_CM_GetStatus(cm, CM_NEED)
        && !USE_FIX_COMP                  // 变频压机     
        && CmRotation[cm] > log_mod_get_freq_min(Cm[cm].run_mode, temp_env, temp_in) // 频率大于最小频率
        && !S_CM_GetStatus(cm, CM_DEFROST)           // 非除霜
//        && log_mod_comp_is_on_stab(cm)               // 已稳定
//        && !comp_freq_limit[cm].forbid_sub           // 非禁降速
//        && !Cm[cm].fg_reoil                          // 非回油
//        && comp_freq_step[cm] == COMP_FREQ_STEP_RUN  // 运行        
    )
    {
        fg_can_sub = TRUE;
    }

    return fg_can_sub;
}

/*******************************************************************************
 函 数: log_mod_comp_status()
 功 能: 压机状态处理(包括当前运行状态、可使用状态)
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_mod_comp_status(void)
{
    U08 i;
    U16 comp_status[MAX_STATUS];         /* 压机状态临时值 */
    
    
    //压机状态
    for (i=0; i<self_comp_num; i++)         /* 前一组信息 */
    {
        comp_status[i] = 0;
        
        /* 压机运行状态 */
        if (s_get_cm_status(i, CM_NEED))       /* 压机运行状态 */
        {
            comp_status[i] |= COMP_RUN_NEED;
        }
        if (s_get_cm_status(i, CM_ON)) //  
//      if (s_get_output(i, SW_CM))       /* 压机运行状态 */
        {
            comp_status[i] |= COMP_RUN;
        }

        if (s_get_cm_status(i, CM_DEFROST))  /* 除霜状态 */
        {
            comp_status[i] |= COMP_DEFROST;
        }
        if (defrost_fg[i].defrost_apply)            /* 除霜申请 */
        {
            comp_status[i] |= COMP_DEFROST_APPLY;
        }
        if (s_get_cm_status(i, CM_FORCE_DEFROST))  /* 强制除霜状态 */
        {
            comp_status[i] |= COMP_FORCE_DEFROST;
        }

        /* 压机可使用状态 */
        if (log_mod_comp_can_open(i))          /* 压机可开 */
        {
            comp_status[i] |= COMP_CAN_OPEN;
        }
        if (log_mod_comp_can_close(i))          /* 压机可关  */
        {
            comp_status[i] |= COMP_CAN_CLOSE;
        }
        
        /* 加/卸载优先 */
        if (log_mod_comp_prio(i, _ADD))
        {
            comp_status[i] |= COMP_ADD_PRIO;
        }
        if (log_mod_comp_prio(i, _SUBB))
        {
            comp_status[i] |= COMP_SUB_PRIO;
        }

        /* 当前运行模式 */
        comp_status[i] |= ((U16)(Cm[i].run_mode & 0x07) << COMP_MODE);
        
        /* 压机可使用模式 */
        comp_status[i] |= ((U16)(s_comp_can_use(i)) << 9);
        
        if (log_mod_comp_can_add(i))           /* 压机可加载 */
        {
            comp_status[i] |= COMP_CAN_ADD;
        }
        if (log_mod_comp_can_sub(i))           /* 压机可卸载 */
        {
            comp_status[i] |= COMP_CAN_SUB;
        }
        
        pCell->cell_status[i] = comp_status[i];
    }

    //模块状态
    comp_status[MAX_COMP] = 0;
    /* 模块状态(是否运行) */
    if (log_mod_cell_is_run(MODE_NULL))
    {
        comp_status[MAX_COMP] |= CELL_RUN;
    }
    /* 模块水泵开启 */
    if (fg_cell_pump_open)
    {
        comp_status[MAX_COMP] |= CELL_PUMP_OPEN;
    }
    /* 模块故障状态(是否故障) */
    if (log_mod_cell_is_err())
    {
        comp_status[MAX_COMP] |= CELL_ERR;
    }
    /* 模块压机个数 */
    comp_status[MAX_COMP] |= (((U16)(self_comp_num<<4)) & CELL_COMP);
    /* 模块可运行热水 */
    if (val_hotw_used && (pcb_addr==0))
    {
        comp_status[MAX_COMP] |= CELL_CAN_RUN_HOTW;
    }
    /* 单元强开辅助电热关闭 */
    if (fg_force_IBH_off)
    {
        comp_status[MAX_COMP] |= CELL_FORCE_IBH_OFF;
    }
    /* 模块水泵状态 */
    if ((fg_PUMPf_run
        &&!fg_water_lack_err
        &&(PUMPf_time.on >= (TF_VAL(AC_PUMP_ON_DLY,TF_PUMP_ON_DLY) + TF_VAL(val_err_ac_flux_delay,TF_ERR_FLOW_DLY) + 2)))
    ||  TF_IS_OUT				/* 商检外机时忽略空调泵开延时 */
    )
    {
        comp_status[MAX_COMP] |= CELL_PUMP_RUN;
    }
    
    pCell->cell_status[MAX_COMP] = comp_status[MAX_COMP];
}


extern U08 cnt_mod_comp_limit_TA_dly[SUB_SYS_TYPE_MAX][CTRL_MODE_MAX];
/*******************************************************************************
 函 数: log_mod_limit_comp_TA_area()
 功 能: 获取限制压机环境温度分区
 参 数: mode--模式
        type--子系统类型
 返 回: result--分区
 其 它: 空
*******************************************************************************/
U08 log_mod_limit_comp_TA_area(U08 mode, U08 type)
{
    BOL fg_env_limit_used;
    I16 point1, point2, point3, point4; // 从大到小排列
    U08 mode_idx = 0;
    U08 grp_idx = 0;
    U08 area_new = AREA_00;
    static U08 area_cur[SUB_SYS_TYPE_MAX][CTRL_MODE_MAX] = {{AREA_00,AREA_00,AREA_00},{AREA_00,AREA_00,AREA_00}};
    static BOL result[SUB_SYS_TYPE_MAX][CTRL_MODE_MAX] = {{COMP_KEEP_AREA,COMP_KEEP_AREA,COMP_KEEP_AREA},{COMP_KEEP_AREA,COMP_KEEP_AREA,COMP_KEEP_AREA}};

    /* 掉电记忆开机后，ctrl_mode的值更新可能不及时 */
    if (mode == MODE_NULL)
    {
        return COMP_KEEP_AREA;
    } 

    /* 确定模式 */
    switch (mode)
    {
        default:
        case MODE_COOL:
            fg_env_limit_used = (val_cool_max_TA == val_cool_min_TA) ? FALSE : TRUE;
            point1 = 10*(COOL_RUN_ENV_HLIMIT);
            point2 = val_cool_max_TA;
            point3 = val_cool_min_TA;
            point4 = 10*(COOL_RUN_ENV_LLIMIT);
            mode_idx = 0;
            break;
            
        case MODE_HEAT:
            fg_env_limit_used = (val_heat_max_TA == val_heat_min_TA) ? FALSE : TRUE;
            point1 = 10*(HEAT_RUN_ENV_HLIMIT);
            point2 = val_heat_max_TA;
            point3 = val_heat_min_TA;
            point4 = 10*(HEAT_RUN_ENV_LLIMIT);
            mode_idx = 1;
            break;
            
        case MODE_HOTW:
            fg_env_limit_used = (val_hotw_max_TA == val_hotw_min_TA) ? FALSE : TRUE;
            point1 = 10*(HOT_WATER_RUN_ENV_HLIMIT);
            point2 = val_hotw_max_TA;
            point3 = val_hotw_min_TA;
            point4 = 10*(HOT_WATER_RUN_ENV_LLIMIT);
            mode_idx = 2;
            break;    
    }

    /* 确定子组类型 */
    switch (type)
    {
        case SUB_SYS_TYPE_MAX:
        case SUB_SYS_AC_HOTW:               /* 空调+热水子系统 */
            grp_idx = 0;
            break;

        case SUB_SYS_AC_ONLY:               /* 单空调子系统 */
            grp_idx = 1;
            break;
    }
    
    area_new = area_cur[grp_idx][mode_idx];
    if (SensorCanUse(pv_cell_TA(pcb_addr)))
    {
        /* AREA_00 point1 AREA_01 point2 AREA_02 point3 AREA_03 point4 AREA_04 */
        area_new = fun_area(pv_cell_TA(pcb_addr), area_cur[grp_idx][mode_idx], 10, point1, point2, point3, point4, SENSOR_NOEXIST);
    }
    
    if (area_new == area_cur[grp_idx][mode_idx])
    {
        cnt_mod_comp_limit_TA_dly[grp_idx][mode_idx] = 0;
    }

    /* 3秒消抖后，正式更新环温区间 */
    if (cnt_mod_comp_limit_TA_dly[grp_idx][mode_idx] > 3)
    {
        area_cur[grp_idx][mode_idx] = area_new;
        if (area_cur[grp_idx][mode_idx]==AREA_00 || area_cur[grp_idx][mode_idx]==AREA_04)
        {
            result[grp_idx][mode_idx] = COMP_OFF_AREA;
        }
        else if (area_cur[grp_idx][mode_idx]==AREA_02)
        {
            result[grp_idx][mode_idx] = COMP_RUN_AREA;
        }
        else
        {
            result[grp_idx][mode_idx] = COMP_KEEP_AREA;
        }
    }

    if (!fg_env_limit_used)
    {
        result[grp_idx][mode_idx] = COMP_RUN_AREA;
    }
    
    return result[grp_idx][mode_idx];
}

/*****************************************************
    函数：
    功能：读取风机的累计运行时间
******************************************************/
//static U08 fan_read_runtime(void)
//{
//  U08  x;
//  
//  if (reeprom_2((U08 *)&fan_runtime[0], MEM_FAN_ADDR, FAN_RUNTIME_SIZE) == 2) {
//      for (x = 0; x<MAX_COMP; x++) fan_runtime[x] = 0;
//  }
//}
///*****************************************************
//  功能：初始化风机的累计运行时间
//******************************************************/
//static void fan_init_runtime(void)
//{
//  U08  x;
//
//  for (x=0; x<MAX_COMP; x++) fan_runtime[x] = 0;
//  
//  weeprom_2((U08 *)&fan_runtime[0], MEM_FAN_ADDR, FAN_RUNTIME_SIZE);
//}

#if 0
/********************************************************
    功能: 处理制热电磁阀
    注意：
********************************************************/
void log_mod_Heater_Valve(void)
{
    static U08 heat_valve_out = FALSE;
    

    if (!(Cm[MAX_COMP].run_mode==MODE_HEAT) || status_run!=_RUN )   /* 非制热也非防冻 */     
    {
        return;
    }
    
    if (SensorCannotUse(pv_sys_TA))    /* 环境温度探头故障退出*/              
    {   
        heat_valve_out = FALSE;
        return;
    }
    if (pv_sys_TA>=0)
    {
        heat_valve_out = TRUE;
    }
    else if(pv_sys_TA<=-10)
    {
        heat_valve_out = FALSE;
    }
    
    if (heat_valve_out)
    {
        s_set_output(MAX_COMP, SW_HEAT_SV);     /* 开制热电磁阀 */   
    }
    else
    {
        s_clr_output(MAX_COMP, SW_HEAT_SV);
    }
    
}
#endif

/*==============================================
函数:处理曲轴加热逻辑
==================================================*/
static void Logic_crank(U08 x)
{
    static U08 fg_crank_on[MAX_COMP] = {FALSE,FALSE};
    I16 temp_env = pv_cell_TA(pcb_addr);

    if (power_time < POWER_ON_INTO_WORK+2) return;  

    if (SensorCanUse(temp_env))  
    {
        if (s_get_cm_status(x, CM_ON) ||  temp_env >= val_close_val_temp)   fg_crank_on[x] = FALSE;
        else if (!s_get_cm_status(x, CM_ON) && temp_env < val_open_val_temp)  fg_crank_on[x] = TRUE;
    }
    else
    {
        fg_crank_on[x] = FALSE;
    }
    
    if(fg_crank_on[x] || !fg_warm_complete)  
    {
        s_set_output(x, SW_crank1);
        s_set_output(x, SW_crank2);
    }
        
}

#if 0
/*******************************************************************************
 函 数: Logic_valve_defr()
 功 能: 除霜阀逻辑
 参 数: x--; 
 返 回: 
 其 它: 空
*******************************************************************************/
static void Logic_valve_defr(U08 x)
{
    BOL fg_on = FALSE;

    if (Cm[x].defrost_stage == DS_DEFROST)  
    {
        fg_on = TRUE;
    }

    if (fg_on)          s_set_output(MAX_COMP, SW_valve_defr);
}
#endif

/*==============================================
函数:处理喷液阀逻辑
==================================================*/
static void Logic_buway_vavle(U08 x)
{
    static U08  fg_byway_have_run[MAX_COMP];

    I16 gas_temp = pv_gas_out_high(x);

    if (SensorCannotUse(gas_temp))   return;
    
    if (s_get_output(x, SW_CM) /* 开压机 */
    && !CmIsCool(x)         /* 2022-6-15 客户要求制冷时不用喷液阀 */
    ) 
    {
        if (gas_temp >= val_open_v_exh)                             fg_byway_have_run[x] = TRUE;
        else if (gas_temp < val_open_v_exh-val_close_v_exh_diff)    fg_byway_have_run[x] = FALSE;
    }
    else
    {
        fg_byway_have_run[x] = FALSE;
    }

    if (fg_byway_have_run[x])    s_set_output(x, SW_byway);
}
/*******************************************************************************
 函 数: log_mod_cond_type()
 功 能: 判断模块压机使用的冷凝器参数
 参 数: comp_num--压机数量; 
 返 回: 1 台 --> 只返回独立
        2 台 --> 返回val_cond_type1 参数
 其 它: 空
*******************************************************************************/
U08 log_mod_cond_type(U08 comp_num)
{
    U08 cond_type = 0;

    switch (comp_num) 
    {
        case 1 : cond_type = TYPE_INDEPENDENCE;     break;  
        default: cond_type = val_cond_type;         break;  
    }

    return (cond_type);
}
/************************************************************************************
* 函数：fan_comp_group_count()
* 功能：根据设置的风机共用方式计算共用风机的一组压机开始和结束压机序号
* 参数：fan_type: 冷凝器类型；&begin、&end(共用风机的一组压机的开始和结束序号存放地址)、num(当前压机的序号)
        
* 返回：无
************************************************************************************/
void fan_comp_group_count(U08 fan_type, U08 *begin, U08 *end, U08 num)
{
    switch (fan_type)
    {
        case TYPE_INDEPENDENCE:
            *begin = num;
            *end = num + 1;
            break;
            
        case TYPE_DEPENDENCE:
            *begin = 0;
            *end = 2;
            break;
            
        default:
            break;  
    }
    
    if (*end > val_comp_num)    *end = val_comp_num;    /* 范围判断 */
}




/*******************************************************************************
 函 数: Condition_judge()
 功 能: 风机开关判定
 参 数: curt_state----当前开关状态; 
        curt_temp--当前值; 
        borderline--边界值; 
        Diff--回差值; 
        direction--判断方向(DRCT_UNDER：小于边界值为真；DRCT_ABOVE：大于边界值为真); 
        
 返 回: TRUE/FALSE--新的开关状态为真/假
 其 它: 
*******************************************************************************/
typedef enum tagDIRECTION
{
    DRCT_UNDER,
    DRCT_ABOVE
}DIRECTION;

BOOL Condition_judge(BOOL curt_state, I16 curt_temp, I16 borderline, I16 Diff, DIRECTION direction)
{  
    BOOL new_state = curt_state;    //新状态
    
    if(SensorCannotUse(curt_temp))  
    {
        new_state = FALSE;
    }
    else
    {
        if(direction == DRCT_ABOVE)
        {
            if(curt_temp >= borderline)             new_state = TRUE;
            else if(curt_temp < borderline-Diff)   new_state = FALSE;
        }
        else if(direction == DRCT_UNDER)
        {
            if(curt_temp <= borderline)             new_state = TRUE;
            else if(curt_temp > borderline+Diff)   new_state = FALSE;
        }
    }

    return new_state;
}
/*******************************************************************************
 函 数: Condition_midjudge()
 功 能: 判断温度是否在给定范围内容
 参 数: curt_state----当前开关状态; 
        curt_temp--当前值; 
        borderupline--边界值上限定; 
        borderdownline--边界值下限
        Diff--回差值; 
        
 返 回: 0: 不在给定值范围 1:在给定值范围
 其 它: 
*******************************************************************************/

U08 Condition_midjudge(U08 curt_state, I16 curt_temp, I16 borderupline, I16 borderdownline,I16 Diff)
{  
    U08 new_state = curt_state;    //新状态
    
    if(SensorCannotUse(curt_temp))  
    {
        new_state = FALSE;
    }
    else
    {
        if((curt_temp > borderdownline)&&(curt_temp <= borderupline))
        {
            new_state = 1;
        }
        else if((curt_temp > (borderupline+Diff))||(curt_temp <= (borderdownline-Diff)))
        {
            new_state = 0;
        }
    }

    return new_state;
}

/*******************************************************************************
 函 数: Condition_judge()
 功 能: 延时3秒判断为真状态
 参 数: curt_state----当前开关状态; 
        curt_temp--当前值; 
        borderline--边界值; 
        Diff--回差值; 
        direction--判断方向(DRCT_UNDER：小于边界值为真；DRCT_ABOVE：大于边界值为真); 
        judgid--第几次判断
 返 回: TRUE/FALSE--新的开关状态为真/假
 其 它: 
*******************************************************************************/
BOOL Condition_2judge(BOOL curt_state, I16 curt_temp, I16 borderline, I16 Diff, DIRECTION direction,U16 judgid)
{  
    BOOL new_state = curt_state;    //新状态

    if(judgid>=TIME_CNT2_MAX)judgid=TIME_CNT2_MAX-1; //防止传入数据超界
    
    if(SensorCannotUse(curt_temp))  
    {
        new_state = FALSE;
    }
    else
    {
        if(direction == DRCT_ABOVE)
        {
            if(curt_temp >= borderline)      
            {
                if (condition_time_cnt_2[judgid]>3) new_state = TRUE;
            }
            
            else if(curt_temp <= borderline-Diff)  
            {
                condition_time_cnt_2[judgid] = 0;
                new_state = FALSE;
            }
            else
            {
                condition_time_cnt_2[judgid] = 0;
            }
        }
        else if(direction == DRCT_UNDER)
        {
            if(curt_temp <= borderline)     
            {
                if (condition_time_cnt_2[judgid]>3) new_state = TRUE;
            }
            else if(curt_temp >= borderline+Diff)  
            {
                new_state = FALSE;
                condition_time_cnt_2[judgid] = 0;
            }
            else
            {
                condition_time_cnt_2[judgid] = 0;
            }
        }
    }
    return new_state;
}
/*******************************************************************************
 函 数: Condition_judge()
 功 能: 延时judgdly秒判断为假状态
 参 数: curt_state----当前开关状态; 
        curt_temp--当前值; 
        borderline--边界值; 
        Diff--回差值; 
        direction--判断方向(DRCT_UNDER：小于边界值为真；DRCT_ABOVE：大于边界值为真); 
        judgdly---延时时间
        judgid--第几次判断
 返 回: TRUE/FALSE--新的开关状态为真/假
 其 它: 
*******************************************************************************/
BOOL Condition_3judge(BOOL curt_state, I16 curt_temp, I16 borderline, I16 Diff, DIRECTION direction,U16 judgdly, U16 judgid)
{  
    BOOL new_state = curt_state;    //新状态
//    static U16 time_cnt[20] = {0.0};    //新状态

    if(judgid>=TIME_CNT3_MAX)judgid=TIME_CNT3_MAX-1; //防止传入数据超界
    
    if(SensorCannotUse(curt_temp))  
    {
        new_state = FALSE;
    }
    else
    {
        if(direction == DRCT_ABOVE)
        {
            if(curt_temp >= borderline)      
            {
                new_state = TRUE;
                condition_time_cnt_3[judgid] = 0;
            }
            else if(curt_temp <= borderline-Diff)  
            {
                if (condition_time_cnt_3[judgid] > judgdly) new_state = FALSE;
            }
            else
            {
                condition_time_cnt_3[judgid] = 0;
            }
        }
        else if(direction == DRCT_UNDER)
        {
            if(curt_temp <= borderline)     
            {
                new_state = TRUE;
                condition_time_cnt_3[judgid] = 0;
            }
            else if(curt_temp >= borderline+Diff)  
            {
                if (condition_time_cnt_3[judgid] > judgdly) new_state = FALSE;
                
            }
            else
            {
                condition_time_cnt_3[judgid] = 0;
            }
        }
    }
    return new_state;
}

/*******************************************************************************
 函 数: pwm_fan_speed_to_percent()
 功 能: pwm风机控速--由目标转速、当前转速计算输出百分比
 参 数: num--风机序号; 
        speed_sv--目标转速; 
        speed_pv--当前转速; 
 返 回: 输出百分比
 其 它: 空
*******************************************************************************/
I16 pwm_fan_speed_to_percent(U08 num, I16 speed_sv, I16 speed_pv)
{
    static I16 per[FREQSPEEDNUM];                                   /* 转换结果：百分比 */

    #define PWM_CTRL_PERIOD (val_pwm_fan_period)                    /* 调速间隔：秒 */
    #define PWM_DEAD_AREA   (val_pwm_fan_area*60/val_fan_pwm_pulse) /* 调速死区：2个脉冲的rpm偏差 */

    #define SPEED_MAX       (1200)                                  /* 最大风速，后用参数替换：rpm */
    #define PWM_STEP_MAX    (10*pow(10, fg_has_dot))                /* 最大调节步幅：% */
    #define PWM_INIT        (val_pwm_fan_init*pow(10, fg_has_dot))  /* 初始值：% */
    #define PWM_COEF        (val_pwm_fan_coef*pow(10, fg_has_dot))  /* 偏差系数：% */
    #define PWM_PCT_MAX     (100*pow(10, fg_has_dot))               /* 百分比最大原始值 */
    #define PWM_PCT_MIN     (fg_has_dot?110:0)                      /* 百分比最小原始值 */

    BOL fg_has_dot = TRUE;  /* 默认0.1精度 */

    if (val_freq_fan_set == FREQ_FAN_SET_PWM_EX
    &&  invt_ec_fan_fg[INVT_ID_1] == 0
        )
    {
        fg_has_dot = FALSE;
    }

    if (speed_sv == 0
    ||  SensorCannotUse(speed_pv)
        )
    {
        per[num] = 0;
    }
    else
    {
        /* 调节前准备 */
        if (per[num] == 0)
        {
            per[num] = PWM_INIT;
        }
        else
        {
            I16 speed_e = speed_pv-speed_sv;/* 偏差 */
            /* 调节步幅：偏差越大，调节输出越大 */
            U08 pwm_step = fun_pv_limit(Fun_AbsI32(speed_e)*PWM_COEF/SPEED_MAX, 1, PWM_STEP_MAX);
            
            if (cnt_pwm_fan_ctrl_period[num] > PWM_CTRL_PERIOD)
            {
                cnt_pwm_fan_ctrl_period[num] = 0;
                
                if (speed_e > PWM_DEAD_AREA)                /* 超调 */
                {
                    per[num] -= pwm_step;
                }
                else if (speed_e < -PWM_DEAD_AREA)          /* 欠调 */
                {
                    per[num] += pwm_step;
                }
                else                                        /* 死区 */
                {
                    ;       
                }
            }
        }
    }

    /* 范围限定 */
    if (per[num] != 0)
    {
        per[num] = fun_pv_limit(per[num], PWM_PCT_MIN, PWM_PCT_MAX);
    }
    
    return per[num];
}

/*******************************************************************************
 函 数: open_fan_by_env_linear()
 功 能: 环温线性控制风速
 参 数: cm--压机; 
 返 回: 风速
 其 它: 空
*******************************************************************************/
I16 open_fan_by_env_linear(U08 cm, I16 min_speed, I16 max_speed, I16 mid_speed)
{
    I16 temp_env = pv_cell_TA(pcb_addr);
    I16 fan_speed = min_speed;
    static BOL fg_open = TRUE;
    static BOL fg_open_bak = TRUE;

    if (SensorCanUse(temp_env))
    {
        if (CmIsCool(cm))
        {
            if (val_close_fan_diff_cl != 0
            &&  temp_env < val_low_fan_env_cold-val_close_fan_diff_cl
                )
            {
                if (fg_open)/* 停风机时清空计时 */
                {
                    fan_restart_cnt = 0;
                }
                fg_open = FALSE;
            }
            else if (temp_env > val_low_fan_env_cold-val_close_fan_diff_cl+val_close_fan_D
                 &&  fan_restart_cnt>=val_close_fan_dly_cl
                 )
            {
                fg_open = TRUE;
            }

            if (fg_open)
            {
                if (fg_open_bak == FALSE)
                {
                    fan_speed = min_speed;  /* 以最低风速进行重新开 */
                }
                else if (temp_env < val_low_fan_env_cold)
                {
                    fan_speed = min_speed;
                }
                else if (temp_env > val_hig_fan_env_cold)
                {
                    fan_speed = max_speed;
                }
                else
                {
                    if (mid_speed == 0)
                    {
                        fan_speed = fun_line_value(temp_env, 
                                                val_hig_fan_env_cold, val_low_fan_env_cold, 
                                                max_speed, min_speed);
                    }
                    else
                    {
                        if (temp_env > val_mid_fan_temp_cold)
                        {
                            fan_speed = fun_line_value(temp_env, 
                                                val_hig_fan_env_cold, val_mid_fan_temp_cold, 
                                                max_speed, mid_speed);
                        }
                        else
                        {
                            fan_speed = fun_line_value(temp_env, 
                                                val_mid_fan_temp_cold, val_low_fan_env_cold, 
                                                mid_speed, min_speed);
                        }
                    }
                }
            }
            else
            {
                fan_speed = 0;
            }
        }
        else
        {
            if (val_close_fan_diff_ht != 0
            &&  temp_env > val_low_fan_env_heat+val_close_fan_diff_ht
                )
            {
                if (fg_open)/* 停风机时清空计时 */
                {
                    fan_restart_cnt = 0;
                }
                fg_open = FALSE;
            }
            else if (temp_env < val_low_fan_env_heat+val_close_fan_diff_ht-val_close_fan_D
                 &&  fan_restart_cnt>=val_close_fan_dly_ht
                 )
            {
                fg_open = TRUE;
            }
            
            if (fg_open)
            {
                if (fg_open_bak == FALSE)
                {
                    fan_speed = min_speed;  /* 以最低风速进行重新开 */
                }
                else if (temp_env > val_low_fan_env_heat)
                {
                    fan_speed = min_speed;
                }
                else if (temp_env < val_hig_fan_env_heat)
                {
                    fan_speed = max_speed;
                }
                else
                {
                    if (mid_speed == 0)
                    {
                        fan_speed = fun_line_value(temp_env, 
                                                val_low_fan_env_heat, val_hig_fan_env_heat, 
                                                min_speed, max_speed);
                    }
                    else
                    {
                        if (temp_env > val_mid_fan_temp_heat)
                        {
                            fan_speed = fun_line_value(temp_env, 
                                                val_low_fan_env_heat, val_mid_fan_temp_heat, 
                                                min_speed, mid_speed);
                        }
                        else
                        {
                            fan_speed = fun_line_value(temp_env, 
                                                val_mid_fan_temp_heat, val_hig_fan_env_heat, 
                                                mid_speed, max_speed);
                        }
                    }
                }
            }
            else
            {
                fan_speed = 0;
            }
        }
    }
    
    /* 备份 */
    fg_open_bak = fg_open;    

    return fan_speed;
}

/*******************************************************************************
 函 数: open_fan_by_wing_linear()
 功 能: 翅温线性控制风速
 参 数: cm--压机; 
 返 回: 风速
 其 它: 空
*******************************************************************************/
I16 open_fan_by_wing_linear(U08 cm, I16 min_speed, I16 max_speed, I16 mid_speed)
{
    I16 temp_wing_cool, temp_wing_heat, temp_wing;
    I16 fan_speed = min_speed;
    static BOL fg_open = TRUE;
    static BOL fg_open_bak = TRUE;

    if (USE_SUCTION_PRS || USE_EXHAUST_PRS)
    {
        temp_wing_cool = temp_wing_heat = pv_cell_wing(cm);
    }
    else
    {
        temp_wing_cool = fun_pv_get(pv_cell_cond_temp(cm), pv_cell_wing(cm));
        temp_wing_heat = fun_pv_get(pv_cell_evap_temp(cm), pv_cell_wing(cm));
    }

    temp_wing = CmIsCool(cm) ? temp_wing_cool : temp_wing_heat;

    if (SensorCanUse(temp_wing))
    {
        if (CmIsCool(cm))
        {
            if (val_close_fan_diff_cl != 0
            &&  temp_wing < val_low_fan_wing_cold-val_close_fan_diff_cl
                )
            {
                if (fg_open)/* 停风机时清空计时 */
                {
                    fan_restart_cnt = 0;
                }
                fg_open = FALSE;
            }
            else if (temp_wing > val_low_fan_wing_cold-val_close_fan_diff_cl+val_close_fan_D
                 &&  fan_restart_cnt>=val_close_fan_dly_cl
                 )
            {
                fg_open = TRUE;
            }

            if (fg_open)
            {
                if (fg_open_bak == FALSE)
                {
                    fan_speed = min_speed;  /* 以最低风速进行重新开 */
                }
                else if (temp_wing < val_low_fan_wing_cold)
                {
                    fan_speed = min_speed;
                }
                else if (temp_wing > val_hig_fan_wing_cold)
                {
                    fan_speed = max_speed;
                }
                else
                {
                    if (mid_speed == 0)
                    {
                        fan_speed = fun_line_value(temp_wing, 
                                                val_hig_fan_wing_cold, val_low_fan_wing_cold, 
                                                max_speed, min_speed);
                    }
                    else
                    {
                        if (temp_wing > val_mid_fan_temp_cold)
                        {
                            fan_speed = fun_line_value(temp_wing, 
                                                val_hig_fan_wing_cold, val_mid_fan_temp_cold, 
                                                max_speed, mid_speed);
                        }
                        else
                        {
                            fan_speed = fun_line_value(temp_wing, 
                                                val_mid_fan_temp_cold, val_low_fan_wing_cold, 
                                                mid_speed, min_speed);
                        }
                    }
                }
            }
            else
            {
                fan_speed = 0;
            }
        }
        else
        {
            if (val_close_fan_diff_ht != 0
            &&  temp_wing > val_low_fan_wing_heat+val_close_fan_diff_ht
                )
            {
                if (fg_open)/* 停风机时清空计时 */
                {
                    fan_restart_cnt = 0;
                }
                fg_open = FALSE;
            }
            else if (temp_wing < val_low_fan_wing_heat+val_close_fan_diff_ht-val_close_fan_D
                 &&  fan_restart_cnt>=val_close_fan_dly_ht
                 )
            {
                fg_open = TRUE;
            }
            
            if (fg_open)
            {
                if (fg_open_bak == FALSE)
                {
                    fan_speed = min_speed;  /* 以最低风速进行重新开 */
                }
                else if (temp_wing > val_low_fan_wing_heat)
                {
                    fan_speed = min_speed;
                }
                else if (temp_wing < val_hig_fan_wing_heat)
                {
                    fan_speed = max_speed;
                }
                else
                {
                    if (mid_speed == 0)
                    {
                        fan_speed = fun_line_value(temp_wing, 
                                                val_low_fan_wing_heat, val_hig_fan_wing_heat, 
                                                min_speed, max_speed);
                    }
                    else
                    {
                        if (temp_wing > val_mid_fan_temp_heat)
                        {
                            fan_speed = fun_line_value(temp_wing, 
                                                val_low_fan_wing_heat, val_mid_fan_temp_heat, 
                                                min_speed, mid_speed);
                        }
                        else
                        {
                            fan_speed = fun_line_value(temp_wing, 
                                                val_mid_fan_temp_heat, val_hig_fan_wing_heat, 
                                                mid_speed, max_speed);
                        }
                    }
                    
                }
            }
            else
            {
                fan_speed = 0;
            }
        }
    }

    /* 备份 */
    fg_open_bak = fg_open;

    return fan_speed;
}

/*******************************************************************************
 函 数: log_mod_get_invt_id_max()
 功 能: 获取实际INVT_ID_MAX，不处理多余循环，避免出错
 参 数: 空
 返 回: 实际INVT_ID_MAX
 其 它: 空
*******************************************************************************/
U16 log_mod_get_invt_id_max(void)
{
    U16 invt_max = 1;

    if (val_freq_fan_set==FREQ_FAN_SET_EC)
    {
        invt_max += val_freq_fan_num;
    }

    return MIN(invt_max, INVT_ID_MAX);
}

/*******************************************************************************
 函 数: freq_fan_speed_get()
 功 能: 变频风速处理
 参 数: cm--压机号; 
 返 回: 变频风速
 其 它: 空
*******************************************************************************/
U16 freq_fan_speed_get(U08 cm, I16 speed_cur)
{
    #define ALLOW_CORRECT_DLY 3*60
    
    I16 speed = 0;
    I16 freq_min = CmIsCool(cm) ? val_freq_fan_cool_min : val_freq_fan_heat_min;
    I16 freq_max = CmIsCool(cm) ? val_freq_fan_cool_max : val_freq_fan_heat_max;
    I16 freq_mid = CmIsCool(cm) ? val_freq_fan_cool_mid : val_freq_fan_heat_mid;
    I16 freq_min_cpst = (I32)freq_min*val_min_Cpst_Coef/100;
    I16 invt_temp = pv_cell_invt_tmp(cm);    // 变频器温度
    I16 temp_env = pv_cell_TA(pcb_addr);
    I16 temp_in  = pv_cell_TWin1(pcb_addr);
    static U08 fg_invt_temp;
    BOL fg_silent = fg_silent_mode;
    static U08 Tp_corr_area = AREA_00;

#if 0
    /* 允许关风机时，风速下限改为0 */
    if (val_close_fan_diff_cl != 0
    ||  val_close_fan_diff_ht != 0
    )
    {
        freq_min_cpst = 0;
    }
#endif

    if (invt_temp > val_ivnt_prot_temp)             fg_invt_temp = 1;
    else if (invt_temp < val_ivnt_prot_temp-50)     fg_invt_temp = 0;

    /* 风速处理 */
    if (val_freq_fan_set == FREQ_FAN_SET_UNUSED)
    {
        speed = 0;
    }
    else if (val_freq_fan_ctrl == FREQ_FAN_CTRL_MANUAL)     /* 手动 */
    {
        speed = val_freq_fan_hand_speed;
    }
    else
    {
        if (fg_invt_temp               /* 变频器温度控制 */
        || (Cm[cm].defrost_stage==DS_QUIT && Cm[cm].defrost_dly<DEFROST_DLY_3)  /* 退除霜 */
        )
        {
            speed = freq_max;
            SETBIT(fan_adjust_fg[cm], FAN_ADJ_FG_HIGH);
        }
        else
        {
            I16 speed_tmp;

            /* 风速基本调节 */
            if (val_freq_fan_ctrl == FREQ_FAN_CTRL_ENV)
            {
                speed_tmp = open_fan_by_env_linear(cm, freq_min, freq_max, freq_mid); /* 环温线性 */
            }
            else
            {
                speed_tmp = open_fan_by_wing_linear(cm, freq_min, freq_max, freq_mid);/* 翅温线性 */
            }
            fan_adjust_tmp[cm][FAN_ADJ_TMP_BASE] = speed_tmp;

            /* 压机频率修正风速调节 */
            if (cm_time[cm].on >= ALLOW_CORRECT_DLY)
            {
                /* 提示：这里是为了得到工况频率上限，而不是为了限频，加级联整合频率上限时注意不用处理这里 */
                I16 invt_max = wf_get_freq_max_by_env_in(Cm[cm].run_mode, temp_env, temp_in);
                U16 coef = fun_pv_limit(100-val_freq_Cpst_Coef+(I32)CmRotation[cm]*val_freq_Cpst_Coef/invt_max, val_min_Cpst_Coef, 100);    /* % */
                speed_tmp = (I32)speed_tmp*coef/100;
                fan_adjust_tmp[cm][FAN_ADJ_TMP_COMP] = speed_tmp;
            }

            /* 功率模式 */
            if (ModeIsEco)
            {
                speed_tmp = (I32)speed_tmp*val_freq_fan_eco_adj/100;
            }
            else if (ModeIsStr)
            {
                speed_tmp = (I32)speed_tmp*val_freq_fan_strong_adj/100;
            }
            else if (ModeIsStd)
            {
                speed_tmp = (I32)speed_tmp*val_freq_fan_std_adj/100;
            }
            fan_adjust_tmp[cm][FAN_ADJ_TMP_POWER] = speed_tmp;

            /* 排温修正风速调节 */
            if (cm_time[cm].on >= ALLOW_CORRECT_DLY
            && ((CmIsCool(cm)&&FAN_TP_COOL_USE) || (CmIsHeat(cm)&&FAN_TP_HEAT_USE))
            )
            {
                Tp_corr_area = fun_area(pv_cell_gas_out(0), Tp_corr_area, 50, 850, 720, SENSOR_NOEXIST);
                speed_tmp = (Tp_corr_area > AREA_02) ? speed_tmp \
                                                     : ((I32)speed_tmp*val_Tp_fan_rpm_corr(Tp_corr_area)/100);

                fan_adjust_tmp[cm][FAN_ADJ_TMP_EXH] = speed_tmp;
            }

            if (speed_cur == 0)             /* 从无到有马上输出 */
            {
                speed = speed_tmp;
            }
            else if (speed_tmp > 0)         /* 等于0时直接关闭，不执行【风速动作比例】 */
            {
                if (fan_speed_adjust_cnt > val_fan_speed_period)
                {
                    fan_speed_adjust_cnt = 0;
                    if (speed_tmp >= speed_cur)
                    {
                        speed = speed_cur + _DIVUCARRY((I32)(speed_tmp-speed_cur)*val_fan_speed_percent, 100);
                    }
                    else
                    {
                        speed = speed_cur - _DIVUCARRY((I32)(speed_cur-speed_tmp)*val_fan_speed_percent, 100);
                    }
                }
                else
                {
                    speed = speed_cur;
                }
            }

            /* 静音模式限速 */
            if (fg_silent)
            {
                switch (silent_mode_level)
                {
                    default:
                    case SILENT_LEVEL_1:
                        speed = MIN(speed, val_freq_fan_silent_max);
                        break;
                    case SILENT_LEVEL_2:
                        speed = MIN(speed, val_freq_fan_str_silent_max);
                        break;
                }
                fan_adjust_tmp[cm][FAN_ADJ_TMP_SILENT] = speed_tmp;
            }

            /* 范围限定 */
            if (speed != 0)
            {
                speed = fun_pv_limit(speed, freq_min_cpst, freq_max);
            }
        }

        speed = AT_VAL(speed, val_ability_test_fan_rpm);
        speed = TF_VAL(speed, TF_fan_freq);
    }

    return speed;
}

/*******************************************************************************
 函 数: fix_fan_speed_get()
 功 能: 定频风机风速处理
 参 数: cm--压机号; 
 返 回: 定频风速
 其 它: 空
*******************************************************************************/
U08 fix_fan_speed_get(U08 cm)
{
    #define FAN_ONOFF_DIFF     20  //关风机回差(0.1℃)
    enum
    {
        FAN_ENVM,   //环境温度开风机
        FAN_COIL,   //翅片温度开风机
        FAN_INVT,   //变频温度开风机
        FAN_MAX
    };
    static U08 fg_fan_mid_on[FAN_MAX];  //中速风机开启
    static U08 fg_fan_hig_on[FAN_MAX];  //高速风机开启
    
    U08 speed = FAN_NONE ;   

    I16 temp_env = pv_cell_TA(pcb_addr);

    if (CmIsCool(cm))
    {
        //环境温度风速需求
        fg_fan_hig_on[FAN_ENVM] = Condition_judge(fg_fan_hig_on[FAN_ENVM], temp_env, val_hig_fan_env_cold, FAN_ONOFF_DIFF, DRCT_ABOVE);
        fg_fan_mid_on[FAN_ENVM] = Condition_judge(fg_fan_mid_on[FAN_ENVM], temp_env, val_low_fan_env_cold, FAN_ONOFF_DIFF, DRCT_ABOVE);

        //翅片温度风速需求
        fg_fan_hig_on[FAN_COIL] = Condition_judge(fg_fan_hig_on[FAN_COIL], pv_cell_wing(cm), val_hig_fan_wing_cold, FAN_ONOFF_DIFF, DRCT_ABOVE);
        fg_fan_mid_on[FAN_COIL] = Condition_judge(fg_fan_mid_on[FAN_COIL], pv_cell_wing(cm), val_low_fan_wing_cold, FAN_ONOFF_DIFF, DRCT_ABOVE);
    }
    else if (CmIsHeat(cm))
    {
        //环境温度风速需求
        fg_fan_hig_on[FAN_ENVM] = Condition_judge(fg_fan_hig_on[FAN_ENVM], temp_env, val_hig_fan_env_heat, FAN_ONOFF_DIFF, DRCT_UNDER);
        fg_fan_mid_on[FAN_ENVM] = Condition_judge(fg_fan_mid_on[FAN_ENVM], temp_env, val_low_fan_env_heat, FAN_ONOFF_DIFF, DRCT_UNDER);

        //翅片温度风速需求
        fg_fan_hig_on[FAN_COIL] = Condition_judge(fg_fan_hig_on[FAN_COIL], pv_cell_wing(cm), val_hig_fan_wing_heat, FAN_ONOFF_DIFF, DRCT_UNDER);
        fg_fan_mid_on[FAN_COIL] = Condition_judge(fg_fan_mid_on[FAN_COIL], pv_cell_wing(cm), val_low_fan_wing_heat, FAN_ONOFF_DIFF, DRCT_UNDER);
    }
    //变频温度风速需求
    fg_fan_hig_on[FAN_INVT] = Condition_judge(fg_fan_hig_on[FAN_INVT], pv_cell_invt_tmp(cm), val_ivnt_prot_temp, FAN_ONOFF_DIFF, DRCT_ABOVE);
    fg_fan_mid_on[FAN_INVT] = FALSE;
    

    if(fg_fan_hig_on[FAN_ENVM] || fg_fan_hig_on[FAN_COIL] || fg_fan_hig_on[FAN_INVT])
    {
        speed = FAN_HIGH;  // 高风速
    }
    else if(fg_fan_mid_on[FAN_ENVM] || fg_fan_mid_on[FAN_COIL] || fg_fan_mid_on[FAN_INVT])
    {
        speed = FAN_MID;  // 中风速
    }
    else
    {
        speed = FAN_LOW;  // 低风速
    }

    return speed;
}

/*******************************************************************************
 函 数: fix_fan_out()
 功 能: 定频风机输出
 参 数: cm--压机号; 
        fan_type--定频风机设置; 
        speed--风速; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void fix_fan_out(U08 cm, U08 fan_type, U08 speed)
{
    if (speed == FAN_NONE
     || fan_type==FAN_SPEED_UNUSED
        )
    {
        return;
    }
    
    if(fan_type==FAN_SPEED_SINGLE)
    {
        s_set_output(cm, SW_FAN_LOW);
        s_set_output(cm, SW_FAN_HIG);
    }
    else if(fan_type==FAN_SPEED_DOUBLE_COMBO)
    {
        if(speed == FAN_HIGH)
        {
            s_set_output(cm, SW_FAN_LOW);
            s_set_output(cm, SW_FAN_HIG);
        }
        else                        s_set_output(cm, SW_FAN_LOW);
    }
    else if(fan_type==FAN_SPEED_DOUBLE_SELF)
    {
        if(speed == FAN_HIGH)       
        {
            fan_low_dly = 0;
            if (fan_high_dly > FAN_CHANGE_DELAY)
            {
                s_set_output(cm, SW_FAN_HIG);
            }
        }
        else                        
        {
            fan_high_dly = 0;
            if (fan_low_dly > FAN_CHANGE_DELAY)
            {
                s_set_output(cm, SW_FAN_LOW);
            }
        }
    }
    else if(fan_type==FAN_SPEED_THREE_COMBO)
    {
        if(speed == FAN_HIGH)
        {
            s_set_output(cm, SW_FAN_LOW);
            s_set_output(cm, SW_FAN_HIG);
        }
        else if(speed == FAN_MID)
        {
            fan_low_dly = 0;
            if (fan_high_dly > FAN_CHANGE_DELAY)
            {
                s_set_output(cm, SW_FAN_HIG);
            }
        }
        else if(speed == FAN_LOW)
        {
            fan_high_dly = 0;
            if (fan_low_dly > FAN_CHANGE_DELAY)
            {
                s_set_output(cm, SW_FAN_LOW);
            }
        }
    }
}

/*==========================================
    功能: 模块的风机动作
/==========================================*/
static void fan_logic(void)
{
    U08 cm = 0;
    I16 speed_freq = 0;             /* 变频风速 */
    I16 speed_fix = FAN_NONE;       /* 定频风速 */

    extern const U08 err_pwm_fan1[];
    extern const U08 err_pwm_fan2[];
    
    /* 手动开风机:待机也允许开，但除霜不允许开 */
    {
        if (val_freq_fan_ctrl == FREQ_FAN_CTRL_MANUAL
        &&  !s_get_cm_status(cm, CM_NEED)
        )
        {
            s_set_cm_status(cm, CM_FAN);
        }
    }
    
    /* 风速获取 */
    /* 风机除霜在Logic_comp_defrost_inverter中处理 */
    if ((!s_get_cm_status(cm, CM_FAN))
//    ||  s_get_error(S_ERR_COOL_FLUX_OL)
    ||  s_get_error(err_pwm_fan1[cm])                   /* 风机1转速异常 */      
    ||  s_get_error(err_pwm_fan2[cm])
        )  
    {
        speed_freq = 0;
        speed_fix = FAN_NONE;
    }
    else
    {
        speed_freq = freq_fan_speed_get(cm, speedsv[0]);    /* 变频 */
        speed_fix  = fix_fan_speed_get(cm);                 /* 定频 */
    }

    /* 风速输出 */
    ARRSET(speedsv, speed_freq);                            /* 变频 */
    fix_fan_out(cm, val_fan_speed_type, speed_fix);         /* 定频 */

    if (speed_fix != FAN_NONE
    ||  speedsv[FREQ_FAN1]>0
    ||  speedsv[FREQ_FAN2]>0
        )
    {
        fg_have_fan_on = TRUE;      /* 只要有一个风机有输出就置为TRUE */
    }
    else
    {
        fg_have_fan_on = FALSE;
    }
}

/*==========================================
    功能: 模块的压机动作
/==========================================*/
// 压机系统除霜动作过程处理(变频压机)
void Logic_comp_defrost_inverter(U08 x, BOOL fg_four_way_valve)
{
    #define MAX_UNLOAD_TIME     60  //卸载超时时间（秒）
        
    switch(Cm[x].defrost_stage) // 压机除霜步骤
    {
        case DS_IDLE:       // 压机未除霜
            Cm[x].defrost_dly = 0;
            Cm[x].defrost_stage = DS_READY;
            cm_time[x].cnt_time_fan_delay=0;
//          break;
            
        case DS_READY:      // 压机准备除霜
            if(s_get_cm_status(x, CM_DEFROST))
            {
                if (invt_info_freq_now(INVT_ID_1) <= DEFR_CHG_FREQ(x)
                ||  Cm[x].defrost_dly > MAX_UNLOAD_TIME
                ||  (USE_FIX_COMP && Cm[x].defrost_dly>MAX_UNLOAD_TIME/3
                ||  fg_ability_test_run
                )
                 )
                {
                    Cm[x].defrost_stage = DS_DEFROST;
                    Cm[x].defrost_dly = 0;
                }
            }           
            else    //NOTE-CXW 准备过程中满足退出？
            {
                Cm[x].defrost_stage = DS_QUIT;
                Cm[x].defrost_dly = 0;
            }
            cm_time[x].cnt_time_fan_delay=0;    // 除霜过程中，退出时间清零（如果此处不清，达不到后面“开风机1S再关压机”的效果）    
            break;  
            
        case DS_DEFROST:    // 压机正式除霜
            if(s_get_cm_status(x, CM_NEED)) // 有运行需求的压机才切换除霜                   
            {
                // 压机运行
                s_set_cm_status(x, CM_ON);  

                /* 压机降频后才处理风机与四通阀 */
                if (Cm[x].defrost_dly >= val_defrost_stop_cm_tm_entry
                || fg_ability_test_run
                )
                {
                    // 关风机
                    s_clr_cm_status(x, CM_FAN);

                    // 四通阀制冷
                    if(fg_four_way_valve)
                    {
                        s_clr_cm_status(x, CM_VALVE_S);
                    }
                    else
                    {
                        s_set_cm_status(x, CM_VALVE_S);
                    }
                }
            }
            if((!s_get_cm_status(x, CM_NEED_DEFROST) && s_get_cm_status(x, CM_DEFROST))
                || (!s_get_cm_status(x, CM_DEFROST)))   // 直接退出除霜           
            {
                Cm[x].defrost_stage = DS_QUIT;
//              if(!s_get_cm_status(x, CM_NEED))    // 共用时单独退除霜停机
//              {
//                  Cm[x].defrost_stage = DS_IDLE;
//              }
                Cm[x].defrost_dly = 0;
            }

            cm_time[x].cnt_time_fan_delay=0;    // 除霜过程中，退出时间清零（如果此处不清，达不到后面“开风机1S再关压机”的效果）    
            break;  
            
        case DS_QUIT:       // 压机退出除霜
            // 开风机：等待停风滴水时间
            if (Cm[x].defrost_dly >= val_defrost_dripping_time)
            {
                s_set_cm_status(x, CM_FAN); // 开风机
            }
            
            // 四通阀切换回制热
            if (Cm[x].defrost_dly >= DEFROST_DLY_2
            ||  fg_ability_test_run
                )
            {
                if(fg_four_way_valve)   
                {
                    s_set_cm_status(x, CM_VALVE_S); // 开四通阀
                }
                else
                {
                    s_clr_cm_status(x, CM_VALVE_S);
                }
            }
            
            
            // 延时满足后退出除霜
            if(Cm[x].defrost_dly >= DEFROST_DLY_3+2*x) // 每个压机错开2秒
            {
                Cm[x].defrost_stage = DS_IDLE;
                Cm[x].defrost_dly = 0;
            }

            break;  
    }
}

/********************************************************
    功能: 处理底盘电加热逻辑
    注意：
********************************************************/
void Logic_Anti_Icing_Heater(void)
{
    #define val_exit_defro_delay            (360)   /* 关除霜电热延时 */
    static U08 flag_open = FALSE;   /* 开标志 */
    I16 open_env = val_anti_ice_heat_env;
    I16 temp_env = pv_cell_TA(pcb_addr);

    if (val_chassis_heat_mode == HEATER_MODE_DEFROST)
    {
        if (s_get_cm_status(0, CM_DEFROST))
        {
            /* 环温小于开启0℃  */
            if (temp_env <= open_env )
            {   
                flag_open = TRUE;
            }
            else if (temp_env > open_env+2*10)
            {
                flag_open = FALSE;
            }
            close_defro_heat_cnt = 0;   // 除霜时，计时清零
        }
        else 
        {
            if (flag_open)  /* 判断除霜时有没有开电加热 */
            {
                if (close_defro_heat_cnt >= val_exit_defro_delay)   flag_open = FALSE;
            }
                
        }
    }
    else if (val_chassis_heat_mode == HEATER_MODE_LOWENV)
    {
        if (s_get_cm_status(0, CM_ON))
        {
            /* 环温小于开启0℃  */
        	if (temp_env <= open_env)
        	{	
        		flag_open = TRUE;
        	}
        	else if (temp_env > open_env + 2*10)
        	{
        		flag_open = FALSE;
        	}
            close_defro_heat_cnt = 0;	// 计时清零
        }
        else
        {
            if (flag_open)
    		{
    			if (close_defro_heat_cnt >= val_exit_defro_delay)	flag_open = FALSE;
    		}
        }
    }
    
    if (flag_open
    && !fg_ability_test_run
        )
    {
        s_set_output(MAX_COMP, S_DO_DPH);   /* 开底盘电加热 */
        s_set_output(MAX_COMP, S_DO_DPH2);  /* 开底盘电加热2 */
    }
}

/*******************************************************************************
 函 数: log_comp_freq_pid()
 功 能: pid频率调节
 参 数: pv--当前水温; 
        sv--目标水温; 
        NowFreNeed--当前频率; 
 返 回: 频率增量
 其 它: 空
*******************************************************************************/
I32 log_comp_freq_pid(I16 pv, I16 sv, I16 NowFreNeed, U08 Mode)
{
    static I16 pid_pv;  /* !!pv与sv使用传址方式，不能定义为自动变量 */
    static I16 pid_sv;
    
    PID_ID_ENUM PID_id = PID_ID_INVT_1;

    I32 pid_kc;
    I32 pid_uk;
    static I16 Pid_Residue = 0;       // PID调节剩余量

    I32 pid_result = 0;

    pid_pv = pv;
    pid_sv = sv;
    
    pid_set_ctrl(PID_id, PID_CTRL_INCREMENT);       //增量式
    pid_set_Ts(PID_id, val_f_pid_calculate_period); //计算周期
    pid_set_Kp(PID_id, val_f_pid_Kp);//比例系数
    pid_set_Ti(PID_id, val_f_pid_i);//积分时间
    pid_set_Td(PID_id, val_f_pid_d);//微分时间
    pid_set_Kc(PID_id, val_f_pid_p);//反作用比例带
    pid_set_Ai(PID_id, val_f_pid_flexi);//积分系数
    pid_set_sv(PID_id, &pid_sv);//目标值
    pid_set_pv(PID_id, &pid_pv);//当前值
    
    pid_turn_on(PID_id);
   
    DebugINVT_ID_1PID[DEBUG_PID_SV] = pid_sv;
    DebugINVT_ID_1PID[DEBUG_PID_PV] = pid_pv;

    if (AdjustFreqPeriod >= val_f_pid_period)    // 变频调节周期调节一次
    {   
        AdjustFreqPeriod = 0;

        pid_uk = pid_read_Uk(PID_id); //PID调节
        pid_kc = pid_get_Kc(PID_id);

        pid_result = _Math_divide(&Pid_Residue, pid_uk*NowFreNeed, pid_kc);
    }

    DebugINVT_ID_1PID[DEBUG_PID_MIDDL]=pid_uk*NowFreNeed;

    return (Mode==MODE_HEAT ? -pid_result : pid_result);    /* 冷热处理 */
}

enum
{
    FORBID_ADD_GAS,  //禁止升频排温
    FORBID_ADD_LP,   //禁止升频低压
    FORBID_ADD_HP,   //禁止升频高压
    FORBID_ADD_COND,   //禁止升频冷凝
    FORBID_ADD_EVAP,   //禁止升频蒸发
    FORBID_ADD_MAX
};
enum
{
    FORBID_SUB_OUT,         //禁止降频出温
    FORBID_SUB_MAX
};
enum
{
    FORCE_SUB_GAS,              //强制降频排气温度
    FORCE_SUB_LP,               //强制降频低压
    FORCE_SUB_HP,               //强制降频高压
    FORCE_SUB_COND,         //强制降频冷凝
    FORCE_SUB_EVAP,         //强制降频蒸发
    FORCE_SUB_MAX
};

//#define PRESS_DIFF          1   /* 回差 */
//#define LP_UNIT             2   // 低压单元
//#define HP_UNIT             10  // 高压单元

/* 低压限制加载 */
#define VAL_FORBID_ADD_LP(mode)     ((mode == MODE_COOL ? val_lp_value_incl \
                                                        : val_lp_value_inht) + val_lp_forbid_add_prs_diff)
/* 高压限制加载 */
#define VAL_FORBID_ADD_HP(mode)     ((mode==MODE_COOL ? val_hp_value_incl \
                                                      : val_hp_value_inht) + val_hp_forbid_add_prs_diff)
/* 低压强制卸载 */
#define VAL_FORCE_SUB_LP(mode)     ((mode==MODE_COOL ? val_lp_value_incl \
                                                     : val_lp_value_inht) + val_lp_force_sub_prs_diff)
/* 高压强制卸载 */
#define VAL_FORCE_SUB_HP(mode)     ((mode==MODE_COOL ? val_hp_value_incl \
                                                     : val_hp_value_inht) + val_hp_force_sub_prs_diff)

#define COOL_EVAP_DIFF 20

static BOOL fg_forbid_add[MAX_COMP][FORBID_ADD_MAX];//禁止升频标志
static BOOL fg_force_sub[MAX_COMP][FORCE_SUB_MAX];//强制降频标志
/*******************************************************************************
 函 数: log_mod_freq_limit_check()
 功 能: 限降频等标志处理
 参 数: comp_num--压机号; 
		Mode--模式; 
 返 回: true：处于限降频；false：正常
 其 它: 空
*******************************************************************************/
BOL log_mod_freq_limit_check(U08 comp_num, U08 Mode)
{
    BOL fg_result = FALSE;
    U08 i;
    I16 temp_cond = log_mod_comp_get_temp_cond(comp_num);  /* 获取冷凝温度 */
    I16 temp_evap = log_mod_comp_get_temp_evap(comp_num);  /* 获取蒸发温度 */
    
    /* 禁止升频 */
    fg_forbid_add[comp_num][FORBID_ADD_GAS] = Condition_2judge(fg_forbid_add[comp_num][FORBID_ADD_GAS], pv_gas_out_high(comp_num), val_force_ctrl_gas_out-GAS_OUT_HIGH_DIFF1, 50, DRCT_ABOVE,CDT_FORBID_ADD_GAS);
    fg_forbid_add[comp_num][FORBID_ADD_LP]  = Condition_judge(fg_forbid_add[comp_num][FORBID_ADD_LP], pv_cell_lp(comp_num), VAL_FORBID_ADD_LP(Mode),  val_lp_pre_adju_diff, DRCT_UNDER);
    fg_forbid_add[comp_num][FORBID_ADD_HP]  = Condition_judge(fg_forbid_add[comp_num][FORBID_ADD_HP], pv_cell_hp(comp_num), VAL_FORBID_ADD_HP(Mode), -val_hp_pre_adju_diff, DRCT_ABOVE);
    
    if (Mode == MODE_COOL)
    {
        fg_forbid_add[comp_num][FORBID_ADD_COND]  = Condition_judge(fg_forbid_add[comp_num][FORBID_ADD_COND], temp_cond, val_cool_cond_temphigh-20, 20, DRCT_ABOVE);
        fg_forbid_add[comp_num][FORBID_ADD_EVAP]  = Condition_judge(fg_forbid_add[comp_num][FORBID_ADD_EVAP], temp_evap, val_evap_low_cool+COOL_EVAP_DIFF, 20, DRCT_UNDER);
    }
    else
    {
        fg_forbid_add[comp_num][FORBID_ADD_COND]  = Condition_judge(fg_forbid_add[comp_num][FORBID_ADD_COND], temp_cond, val_heat_cond_temphigh-20, 20, DRCT_ABOVE);
        fg_forbid_add[comp_num][FORBID_ADD_EVAP]  = Condition_judge(fg_forbid_add[comp_num][FORBID_ADD_EVAP], temp_evap, val_heat_evap_templow+30, 20, DRCT_UNDER);
    }
    
    if (comp_realrun_time[comp_num] <= 180)
    {
        fg_forbid_add[comp_num][FORBID_ADD_EVAP]  = FALSE;
    }

    /* 强制降频 */
    fg_force_sub[comp_num][FORCE_SUB_GAS] = Condition_2judge(fg_force_sub[comp_num][FORCE_SUB_GAS], pv_gas_out_high(comp_num), val_force_ctrl_gas_out, 50,DRCT_ABOVE,CDT_FORCE_SUB_GAS);
    fg_force_sub[comp_num][FORCE_SUB_LP]  = Condition_judge(fg_force_sub[comp_num][FORCE_SUB_LP], pv_cell_lp(comp_num), VAL_FORCE_SUB_LP(Mode), val_lp_pre_adju_diff, DRCT_UNDER);
    fg_force_sub[comp_num][FORCE_SUB_HP]  = Condition_judge(fg_force_sub[comp_num][FORCE_SUB_HP], pv_cell_hp(comp_num), VAL_FORCE_SUB_HP(Mode), -val_hp_pre_adju_diff, DRCT_ABOVE);

    if (Mode == MODE_COOL)
    {
        fg_force_sub[comp_num][FORCE_SUB_COND] = Condition_2judge(fg_force_sub[comp_num][FORCE_SUB_COND], temp_cond, val_cool_cond_temphigh, 20, DRCT_ABOVE,CDT_FORCE_SUB_COOLCOND);
        fg_force_sub[comp_num][FORCE_SUB_EVAP] = Condition_2judge(fg_force_sub[comp_num][FORCE_SUB_EVAP], temp_evap, val_evap_low_cool+COOL_EVAP_DIFF, 0, DRCT_UNDER, CDT_FORCE_SUB_COOLEVAP);
    }
    else
    {
        fg_force_sub[comp_num][FORCE_SUB_COND] = Condition_2judge(fg_force_sub[comp_num][FORCE_SUB_COND], temp_cond, val_heat_cond_temphigh, 20, DRCT_ABOVE,CDT_FORCE_SUB_HEATCOND);
        fg_force_sub[comp_num][FORCE_SUB_EVAP] = Condition_2judge(fg_force_sub[comp_num][FORCE_SUB_EVAP], temp_evap, val_heat_evap_templow, 20, DRCT_UNDER,CDT_FORCE_SUB_HEATEVAP);
    }
    
    if (comp_realrun_time[comp_num] <= 180)
    {
        fg_force_sub[comp_num][FORCE_SUB_EVAP]  = FALSE;
    }


    /* bms：禁止升频标志 */
    for (i=0; i<FORBID_ADD_MAX; i++)
    {
        if (fg_forbid_add[comp_num][i])
        {
            SETBIT(fg_forbid_rise[comp_num],i);
            fg_result = TRUE;
        }
        else
        {
            CLRBIT(fg_forbid_rise[comp_num],i);
        }
    }

    for (i=0; i<FORCE_SUB_MAX; i++)
    {
        if (fg_force_sub[comp_num][i])
        {
            SETBIT(fg_force_drop[comp_num],i);
            fg_result = TRUE;
        }
        else
        {
            CLRBIT(fg_force_drop[comp_num],i);
        }
    }

    return fg_result;
}

/*******************************************************************************
 函 数: Calculate_Frequency_Need()
 功 能: 计算频率需求
 参 数: U08 comp_num--压机数量，U08 Mode--压机当前模式，I16 InletTemp--回水温度，I16 EnvTemp--环境温度
               NowFreNeed:当前转速 
 返 回: 频率
 其 它: 空
*******************************************************************************/
U16 Calculate_Frequency_Need(U08 comp_num, U08 Mode,I16 NowFreNeed) 
{
    U16 CalFrequency = NowFreNeed; 

    PID_ID_ENUM PID_id = PID_ID_INVT_1;
    I16 pid_pv = 0;
    I16 pid_sv = 0;
    
    I32 pid_freq=0;
//    static U16 AdjustFreqPeriod;    // 频率调节周期
    
    I16 temp_env = pv_cell_TA(pcb_addr);
    I16 temp_in  = pv_cell_TWin1(pcb_addr);
    I16 temp_out = pv_cell_TWout1(pcb_addr);
 
//    extern const U08 err_sub_freq[];    /* N0004 */

    I16 gas_temp = pv_gas_out_high(comp_num);
    U08 gas_out_trend = fun_value_trend(gas_temp, mod_rec_gas_out_avg[comp_num], 1);

    
    if (CalFrequency==0) // 没有运行
    {
        pid_turn_off(PID_id);
        AdjustFreqPeriod = 0;      // 间隔计时清零
    }
    else 
    {
        /* 调频控制温度 */
        if (MODE_IS_HOTW(GET_SUB_SYS(pcb_addr)))
        {
            pid_pv = temp_in;				                        /* 热水模式下，固定使用回水控制 */
        }
        else
        {
            pid_pv = USE_AC_TEMP_IN_CTRL ? temp_in : temp_out;		/* 空调模式下，由是否使用缓冲水箱探头决定 */
        }
        /* 调频目标温度 */
        pid_sv = sv_temp_freq[GET_SUB_SYS(pcb_addr)];
    
        /* 频率需求计算 */
        pid_freq = log_comp_freq_pid(pid_pv, pid_sv, CalFrequency, Mode);
        

        /* 后处理 */
        {
            /* 限幅 */
            if (fg_freq_had_force_sub)
            {
                /* 强制降频过，则缓慢升频 */
                pid_freq = fun_pv_limit(pid_freq, -val_invt_run_speed, val_invt_run_speed/5);
            }
            else
            {
                pid_freq = fun_pv_limit(pid_freq, -val_invt_run_speed, val_invt_run_speed); 
            }
            
            DebugINVT_ID_1PID[DEBUG_STEP] = pid_freq;
            
            // 强制卸载
            if (fg_force_sub[comp_num][FORCE_SUB_COND]==TRUE
            ||  fg_force_sub[comp_num][FORCE_SUB_EVAP]==TRUE
            ||  (fg_force_sub[comp_num][FORCE_SUB_GAS]==TRUE && TREND_IS_RISE(gas_out_trend))
            ||  fg_force_sub[comp_num][FORCE_SUB_LP]==TRUE
            ||  fg_force_sub[comp_num][FORCE_SUB_HP]==TRUE
//            ||  s_get_error(err_sub_freq[comp_num]) /* N0004 */
                )
            {
                if (cnt_force_sub_freq[comp_num] >= val_invt_rps_force_sub_period)
                {
                    cnt_force_sub_freq[comp_num] = 0;
                    CalFrequency = (I32)CalFrequency*val_invt_rps_force_sub_percent/100; // 频率降低为原运行频率的【强制降频比例】

                    fg_freq_had_force_sub = TRUE;
                }
                else
                {
                    if (pid_freq<0)  // 只能降频
                    {
                        CalFrequency += pid_freq;
                    }                    
                }
            }
            // 限制加载
            else if (fg_forbid_add[comp_num][FORBID_ADD_GAS]==TRUE 
                     || fg_forbid_add[comp_num][FORBID_ADD_LP]==TRUE 
                     || fg_forbid_add[comp_num][FORBID_ADD_HP]==TRUE 
                     || fg_forbid_add[comp_num][FORBID_ADD_COND]==TRUE
                     || fg_forbid_add[comp_num][FORBID_ADD_EVAP]==TRUE)
            {
                if (pid_freq<0)  // 只能降频
                {
                    CalFrequency += pid_freq;
                }
            }
            // 正常控制
            else
            {
                CalFrequency += pid_freq;
            }

            /* 到温预降频:搭配pid使用 */
            if ((Mode==MODE_HEAT && pid_pv>=pid_sv)
            ||  (Mode==MODE_COOL && pid_pv<=pid_sv)
                )
            {
                if (fg_freq_pre_sub_en)
                {
                    fg_freq_pre_sub_en = FALSE;

                    CalFrequency -= val_invt_freq_pre_sub;
                    pid_set_AveAcrossCnt(PID_ID_EEV_1, 0);
                }
            }  
        }
    }

    /* 变频器频率限定 */
    {
        I16 FreqMax = log_mod_get_freq_max(Mode, temp_env, temp_in);         /* 变频器最大频率 */
        I16 FreqMin = log_mod_get_freq_min(Mode, temp_env, temp_in);            /* 变频器最小频率 */
        CalFrequency = fun_pv_limit(CalFrequency, FreqMin, FreqMax);
    
        gdebugIO.freq_max = FreqMax;
    }

    // 国标测试用
    CalFrequency = calculate_gb_dbg_freq(Mode, CalFrequency); 

    return CalFrequency;
}




// 压机系统动作过程处理
void Logic_comp_action(U08 x)
{
    I16 val_fan_on_dly=0,val_fan_off_dly=0;
    I16 val_commu_on_dly=0,val_commu_off_dly=0;
    
    BOOL fg_four_way_valve=0;   // 0-表示当前模式不用开阀   1-表示当前模式需用开阀
    if(val_valve_type == HEAT_MODE_ON && Cm[x].run_mode == MODE_HEAT )  fg_four_way_valve=1;    // 制热模式开阀
    if(val_valve_type == COOL_MODE_ON && Cm[x].run_mode == MODE_COOL )  fg_four_way_valve=1;    // 制冷模式开阀 
    
    
    switch(Cm[x].stage) // 压机开机步骤
    {
        case CM_IDLE:       // 压机待机
            if(s_get_cm_status(x, CM_NEED)) // 压机有需求
            {
                Cm[x].stage = CM_STARTING;  //NOTE-CXW 要测试此跳转
                Cm[x].dly = 0;
                s_clr_cm_status(x, CM_DEFROST_EXIT);    //NOTE-CXW 要测试退除霜后是否不受防频繁保护影响
            }
            else    //NOTE-CXW 这里这样清标志是否合适？
            {
                s_clr_cm_status(x, CM_VALVE_S); // 关四通阀（如果程序做成停机不需要停阀就屏蔽此句）
                s_clr_cm_status(x, CM_ON);  // 关压机
                s_clr_cm_status(x, CM_FAN); // 关风机               
            }
            Cm[x].defrost_stage = DS_IDLE;
            break;
            
        case CM_STARTING:   // 压机开机过程
            // 开机延时
            val_fan_on_dly      = (Cm[x].run_mode == MODE_HEAT) ? TF_VAL(val_heat_fan_on_dly, TF_FAN_ON_DLY) : TF_VAL(val_cool_fan_on_dly, TF_FAN_ON_DLY);
            val_commu_on_dly    = (fg_four_way_valve==1)        ? TF_VAL(val_valve_on_dly, TF_4V_ON_DLY) : TF_VAL(val_valve_on_dly, TF_4V_ON_DLY);
            
            
            if(s_get_cm_status(x, CM_NEED)) // 压机有需求
            {
                if(val_fan_on_dly<0 && val_commu_on_dly<0)
                {
                    s_set_cm_status(x, CM_ON);  // 开压机
                    
                    if(Cm[x].dly >= -val_fan_on_dly)
                    {
                        s_set_cm_status(x, CM_FAN); // 开风机
                    }
                    
                    if(Cm[x].dly >= -val_commu_on_dly && fg_four_way_valve==1)
                    {
                        s_set_cm_status(x, CM_VALVE_S); // 开四通阀
                    }
                    
                    if(Cm[x].dly >= -val_fan_on_dly && Cm[x].dly >= -val_commu_on_dly)
                    {
                        Cm[x].stage = CM_RUNNING;   //NOTE-CXW 要测试此跳转
                        Cm[x].dly = 0;
                    }
                }
                else if(val_fan_on_dly >= val_commu_on_dly)
                {
                    s_set_cm_status(x, CM_FAN); // 开风机
                
                    // 按模式正常处理四通阀
                    if(fg_four_way_valve==1)    
                    {
                        if(Cm[x].dly >= val_fan_on_dly-val_commu_on_dly)
                        {
                            s_set_cm_status(x, CM_VALVE_S); // 开四通阀
                        }
                    }
                    
                    if(Cm[x].dly >= val_fan_on_dly)
                    {
                        s_set_cm_status(x, CM_ON);  // 开压机
                    }
                    
                    if(val_commu_on_dly<0 && Cm[x].dly >= val_fan_on_dly-val_commu_on_dly
                        || val_commu_on_dly>=0 && Cm[x].dly >= val_fan_on_dly)
                    {
                        Cm[x].stage = CM_RUNNING;   //NOTE-CXW 要测试此跳转
                        Cm[x].dly = 0;
                    }
                }
                else if(val_fan_on_dly < val_commu_on_dly)
                {
                    // 按模式正常处理四通阀
                    if(fg_four_way_valve==1)    
                    {
                        s_set_cm_status(x, CM_VALVE_S); // 开四通阀
                    }
                    if(Cm[x].dly >= val_commu_on_dly-val_fan_on_dly)
                    {
                        s_set_cm_status(x, CM_FAN); // 开风机
                    }
                    
                    if(Cm[x].dly >= val_commu_on_dly)
                    {
                        s_set_cm_status(x, CM_ON);  // 开压机
                    }
                    
                    if(val_fan_on_dly<0 && Cm[x].dly >= val_commu_on_dly-val_fan_on_dly
                        || val_fan_on_dly>=0 && Cm[x].dly >= val_commu_on_dly)
                    {
                        Cm[x].stage = CM_RUNNING;   //NOTE-CXW 要测试此跳转
                        Cm[x].dly = 0;
                    }
                }
            }
            else
            {
                Cm[x].stage = CM_STOPPING;  //NOTE-CXW 要测试此跳转
                if(!s_get_cm_status(x, CM_ON))  // 压机未开，直接转IDLE
                {
                    Cm[x].stage = CM_IDLE;      
                }
                Cm[x].dly = 0;
            }
            break;
            
        case CM_RUNNING:    // 压机运行过程
            if(s_get_cm_status(x, CM_NEED)) // 压机有需求
            {
                // 四通阀制冷/制热
                if(fg_four_way_valve==1)    
                {
                    s_set_cm_status(x, CM_VALVE_S); // 开四通阀
                }
                else		/* 2022-8-21增加清状态处理，使得运行过程中等够响应模式变化 */
                {
                    s_clr_cm_status(x, CM_VALVE_S); // 关四通阀
                }
                // 压机正常运行
                s_set_cm_status(x, CM_ON);  // 开压机
                // 风机正常运行（必要时有风速调节？）
                s_set_cm_status(x, CM_FAN); // 开风机

                if(s_get_cm_status(x, CM_NEED_DEFROST) && s_get_cm_status(x, CM_DEFROST_ALLOW)) // 满足除霜条件
                {
                    Cm[x].stage = CM_DEFROSTING;
                    Cm[x].dly = 0;
                    Cm[x].defrost_stage = DS_IDLE;
                }
            }
            else
            {
                Cm[x].stage = CM_STOPPING;  //NOTE-CXW 要测试此跳转
                Cm[x].dly = 0;
                Cm[x].defrost_dly = 0;
            }
            break;
            
        case CM_DEFROSTING: // 压机除霜过程
            if(s_get_cm_status(x, CM_NEED)) // 压机有需求         
            {
                Logic_comp_defrost_inverter(x,fg_four_way_valve);   // 压机除霜过程动作处理
                if(Cm[x].defrost_stage == DS_IDLE)
                {
                    Cm[x].stage = CM_RUNNING;
                    Cm[x].dly = 0;  //NOTE-CXW 这里要不要清零？
                    s_clr_cm_status(x, CM_DEFROST_OFF);
                }
            }
            else
            {
                Cm[x].stage = CM_STOPPING;  //NOTE-CXW 要测试此跳转
                Cm[x].dly = 0;
                Cm[x].defrost_dly = 0;

            }
            break;
            
        case CM_STOPPING:   // 压机关机过程
            {
                /* 滤掉变频减速停机时间 */
                U16 off_dly = Cm[x].dly>CM_off_speed_down_time ? Cm[x].dly-CM_off_speed_down_time : 0;
                // 关机延时
                val_fan_off_dly     = (Cm[x].run_mode == MODE_HEAT) ? val_heat_fan_off_dly : val_cool_fan_off_dly;
                val_commu_off_dly   = (fg_four_way_valve==1)        ? TF_VAL(val_valve_off_dly, -10) : TF_VAL(val_valve_off_dly, -10) ;
                    
                if(val_fan_off_dly<0 && val_commu_off_dly<0)
                {
                    s_clr_cm_status(x, CM_ON);  /* 关压机 */
                
                    if(off_dly >= -val_fan_off_dly)
                    {
                        s_clr_cm_status(x, CM_FAN); /* 关风机 */
                    }
                
                    if(off_dly >= -val_commu_off_dly)
                    {
                        s_clr_cm_status(x, CM_VALVE_S); /* 关四通阀 */
                    }
                    if(off_dly >= -val_fan_off_dly && off_dly >= -val_commu_off_dly)
                    {
                        Cm[x].stage = CM_IDLE;  //NOTE-CXW 要测试此跳转
                        Cm[x].dly = 0;
                    }
                }
                else if(val_fan_off_dly >= val_commu_off_dly)
                {
                    s_clr_cm_status(x, CM_FAN); /* 关风机 */
            
                    if(off_dly >= val_fan_off_dly-val_commu_off_dly)
                    {
                        s_clr_cm_status(x, CM_VALVE_S); /* 关四通阀 */
                    }
            
                    if(off_dly >= val_fan_off_dly)
                    {
                        s_clr_cm_status(x, CM_ON);  /* 关压机 */
                    }
                    if(val_commu_off_dly<0 && off_dly >= val_fan_off_dly-val_commu_off_dly
                    || val_commu_off_dly>=0 && off_dly >= val_fan_off_dly)
                    {
                        Cm[x].stage = CM_IDLE;  //NOTE-CXW 要测试此跳转
                        Cm[x].dly = 0;
                    }
                    
                }
                else if(val_fan_off_dly < val_commu_off_dly)
                {
                    s_clr_cm_status(x, CM_VALVE_S); /* 关四通阀 */
            
                    if(off_dly >= val_commu_off_dly-val_fan_off_dly)
                    {
                        s_clr_cm_status(x, CM_FAN); /* 关风机 */
                    }
            
                    if(off_dly >= val_commu_off_dly)
                    {
                        s_clr_cm_status(x, CM_ON);  /* 关压机 */
                    }
                    if(val_fan_off_dly<0 && off_dly >= val_commu_off_dly-val_fan_off_dly
                    || val_fan_off_dly>=0 && off_dly >= val_commu_off_dly)
                    {
                        Cm[x].stage = CM_IDLE;  //NOTE-CXW 要测试此跳转
                        Cm[x].dly = 0;
                    }
                }
            }
                
            break;
            
        default:    // 非预知状态
            s_clr_cm_status(x, CM_VALVE_S); // 关四通阀
            s_clr_cm_status(x, CM_ON);  // 关压机
            s_clr_cm_status(x, CM_FAN); // 关风机
            break;          
    }
    
    if (s_get_cm_status(x, CM_ON)           // 开压机
    && !s_get_cm_status(x, CM_DEFROST_OFF)  /* 开压机 */
        ) 
    {
        s_set_output(x, SW_CM); 
    }
    
    if (s_get_cm_status(x, CM_VALVE_S)) // 开四通阀
    {
        s_set_output(x, SW_S1);     
    }
}


/****************************************************************
函数：log_cmp_is_reoil()
功能：判断回油逻辑：根据低负荷运行时间控制。
参数：id
返回：0--不回油 / 其它值--回油转速
其它：以额定转速回油
*****************************************************************/
U16 log_cmp_is_reoil(U08 cm)
{   
    static BOOL fg_reoil = FALSE;       // 回油状态

    I16 freq_oil   = 0;                 //压机回油转速(0表示不需回油)
    BOL fg_TA_low = TRUE;
    BOL fg_allow = FALSE;

#if defined(M_DISEN)
    I16 sv_TA_reoil = Cm[cm].run_mode == MODE_HEAT ? val_heat_reoil_env : val_cool_reoil_env;
    fg_TA_low = (pv_cell_TA(pcb_addr) <= sv_TA_reoil) ? TRUE : FALSE;
#endif

    if (s_get_cm_status(cm, CM_ON)
    &&  Cm[cm].stage!=CM_DEFROSTING
    &&  val_reoil_time!=0
    &&  !USE_FIX_COMP
    &&  !fg_ability_test_run
    )  
    {
        fg_allow = TRUE;
    }

    if (fg_allow)  
    {
        /* 进入回油 */
        if (cnt_reoil_cycle > val_reoil_period && fg_TA_low)
        {
            fg_reoil = TRUE;
        }

        /* 退出回油 */
        if (cnt_reoil_time > val_reoil_time)
        {
            fg_reoil = FALSE;
        }

        /* 回油动作 */
        if (fg_reoil)
        {
            freq_oil = val_reoil_inventrps;
        }
    }
    else
    {
        /* 处理静态变量 */
        cnt_reoil_time  = 0;
        cnt_reoil_cycle = 0;
        fg_reoil        = FALSE;
    }

    /* 回油相关计时 */
    {
        if (invt_info_freq_now((INVT_ID_ENUM)cm) >= val_reoil_inventrps 
        ||  fg_reoil)
        {
            cnt_reoil_cycle = 0;
        }
        if (!fg_reoil)
        {
            cnt_reoil_time = 0;
        }
    }

    return freq_oil;    
}       
/****************************************************************
函数：void log_realrun_time(U08 s)
功能：压机正常调节情况下计时。
参数：U08 fun_fg--0:清零时间  非零正常计数
                U08 comp_id--第几个压机
返回：无
其它：
*****************************************************************/
void log_realrun_time(U08 comp_id,U08 fun_fg)
{
    if(comp_id<MAX_COMP)  //防止超界
    {
        if(fun_fg==0) comp_realrun_time[comp_id] = 0;
    }

}

/*******************************************************************************
 函 数: log_mod_set_avoid_freq()
 功 能: 回避频率序列赋值
 参 数: avoid_freq[]--; 
        num--; 
        cm--; 
 返 回: 
 其 它: 空
*******************************************************************************/
void log_mod_set_avoid_freq(I16 avoid_freq[], U08 num, U08 cm)
{
    U08 i = 0;

    cm = cm;

    for (i=0; i<num; i++)
    {
        avoid_freq[i] = val_avoid_freq(i);
    }
}

#define AVOID_FREQ_MAX 4            /* 最大跳频点数目 */
/*******************************************************************************
 函 数: log_mod_freq_is_avoid()
 功 能: 判断一个频率是否处于回避频率区间内
 参 数: freq--频率; 
 返 回: TRUE、FALSE
 其 它: 空
*******************************************************************************/
BOL log_mod_freq_is_avoid(I16 freq, U08 cm)
{
    BOL result = FALSE;
    I16 range = val_avoid_freq_range;
    I16 avoid_freq[AVOID_FREQ_MAX];
    U08 i = 0;

    log_mod_set_avoid_freq(avoid_freq, AVOID_FREQ_MAX, cm);

    for (i=0; i<AVOID_FREQ_MAX; i++)
    {
        I16 avoid_min = avoid_freq[i]-(_DIVUCARRY(range, 2));
        I16 avoid_max = avoid_freq[i]+(_DIVUCARRY(range, 2));
        if (fun_pv_in_open(freq, avoid_min, avoid_max))
        {
            result = TRUE;
            break;
        }
    }

    return result;
}
/*******************************************************************************
 函 数: log_mod_avoid_freq()
 功 能: 处理回避频率(机组共振频率、跳频点)
 参 数: freq_need--需求频率; 
 返 回: 输出频率
 其 它: 
    1、需求频率已做相应上下限判断，故此函数不再判断上下限
*******************************************************************************/
I16 log_mod_avoid_freq(I16 freq_need, U08 cm)
{ 
    I16 freq_out = freq_need;
    I16 range = val_avoid_freq_range;
    I16 avoid_freq[AVOID_FREQ_MAX];
    U08 i = 0;
    I16 temp_env = pv_cell_TA(pcb_addr);
    I16 temp_in = pv_cell_TWin1(pcb_addr);
    
    if (range == 0          /* 设为0表示不使用跳频功能 */
    ||  freq_need == 0          
        )
    {
        freq_out = freq_need;
    }
    else
    {
        /* 预处理 */
        log_mod_set_avoid_freq(avoid_freq, AVOID_FREQ_MAX, cm);
        
        fun_sort_bubble(avoid_freq, ARRELE(avoid_freq), DIR_DCD);

        /* 跳频处理 */
        for (i=0; i<AVOID_FREQ_MAX; i++)
        {
            I16 avoid_min = avoid_freq[i]-(_DIVUCARRY(range, 2));
            I16 avoid_max = avoid_freq[i]+(_DIVUCARRY(range, 2));
            if (fun_pv_in_open(freq_out, avoid_min, avoid_max))
            {
                /* 2021-12-15-增加对跳频区间的范围判断 */
                if (avoid_max > log_mod_get_freq_max(Cm[cm].run_mode, temp_env, temp_in))
                {
                    freq_out = avoid_min;
                }
                else if (avoid_min < log_mod_get_freq_min(Cm[cm].run_mode, temp_env, temp_in))
                {
                    freq_out = avoid_max;
                }
                else
                {
                    /* 配合跳频点降序序列使用 */
                    if (freq_out > avoid_freq[i])
                    {
                        freq_out = log_mod_freq_is_avoid(avoid_max, cm) ? avoid_min : avoid_max;
                    }
                    else
                    {
                        freq_out = avoid_min;
                    }
                }
            }
        }
    }

    return freq_out;
}

/****************************************************************
函数：U16 log_mod_comp_defrost_freq(void)
功能：除霜时压机运行逻辑
参数：无
返回：压机运行频率
其它：
*****************************************************************/
U16 log_mod_comp_defrost_freq(U08 x)
{
    U16 tmp = DEFR_CHG_FREQ(x);
    BOL fg_stop_cm_need_enter = 0, fg_stop_cm_need_exit = 0;

    if ((Cm[x].defrost_stage==DS_READY))
    {
        tmp = DEFR_CHG_FREQ(x);
        fg_stop_cm_need_enter = TRUE;
    }
    else if((Cm[x].defrost_stage==DS_DEFROST))
    {
        #define DEFR_CHG_DLY (val_defrost_stop_cm_tm_entry+val_defrost_four_tm_entry)
        if (Cm[x].defrost_dly < DEFR_CHG_DLY)
        {
            tmp = DEFR_CHG_FREQ(x);
            fg_stop_cm_need_enter = TRUE;
        }
        else
        {
            tmp = MIN(DEFR_CHG_FREQ(x) + MIN(30, ((Cm[x].defrost_dly-DEFR_CHG_DLY)/val_invt_start_period))*val_invt_defr_speed, val_invt_rps_defrost);
//            tmp = val_invt_rps_defrost;
        }
    }
    else if((Cm[x].defrost_stage==DS_QUIT))
    {
        if(Cm[x].defrost_dly<DEFROST_DLY_3)
        {
            tmp = DEFR_CHG_FREQ(x);
            fg_stop_cm_need_exit = TRUE;
        }
        else
        {
            tmp = val_invt_rps_heat;
        }
    }

    /* 处理停机除霜状态 */
    if ((fg_stop_cm_need_enter && (val_defrost_stop_cm_en==DF_STOP_CM_ALL
                                  || val_defrost_stop_cm_en==DF_STOP_CM_ENTER))
    ||  (fg_stop_cm_need_exit  && (val_defrost_stop_cm_en==DF_STOP_CM_ALL
                                  || val_defrost_stop_cm_en==DF_STOP_CM_EXIT))
    )
    {
        s_set_cm_status(x, CM_DEFROST_OFF);
    }
    else
    {
        s_clr_cm_status(x, CM_DEFROST_OFF);
    }
                    
    return AT_VAL(tmp, val_ability_test_freq);
}

static void log_mod_comp(void)
{
    U08 x;
    static I16 freq_abnormal[MAX_COMP];
    static I16 freq_before_reoil[MAX_COMP] = {COMP_INIT(SENSOR_NOEXIST)};   /* 回油前转速 */
    I16 temp_env = pv_cell_TA(pcb_addr);
    I16 temp_in = pv_cell_TWin1(pcb_addr);
    
    for (x=0; x<self_comp_num; x++)
    {
        I16 freq_up = log_mod_get_freq_max(Cm[x].run_mode, temp_env, temp_in);
        I16 freq_dn = log_mod_get_freq_min(Cm[x].run_mode, temp_env, temp_in);
        I16 freq_rated = Cm[x].run_mode==MODE_COOL ? val_invt_rps_cool : val_invt_rps_heat; /* 额定转速 */
        I16 freq_start = val_invt_start_freq;   /* 启动转速 */

        /* 工况频率限制：临时这么处理，后续可优化实现 */
        freq_rated = fun_pv_limit(freq_rated, freq_dn, freq_up);
        freq_start = fun_pv_limit(freq_start, freq_start, freq_up); /* 只限制上限 */
        
        Logic_comp_action(x);    
        Logic_buway_vavle(x);   // 喷液阀逻辑
        Logic_crank(x);
//        Logic_valve_defr(x);
    
//        if (val_inverter_used==USED && pcb_type==TYPE_MASTER && x==0)
        {
            
            BOOL fg_adjust_allow = FALSE;                   // 进入调节允许标志
            BOOL fg_adjust_run = FALSE;                     /* 进入调节标志 */
            BOOL fg_limit = FALSE;                          /* 进入限降频标志 */
            static BOOL fg_adjust_run_bak[MAX_COMP];
            BOOL fg_common_run = FALSE;                     /* 正常运行标志 */
            static BOOL fg_starting = FALSE;                /* 启动过程标志 */
            U16 freq_start_dly_max;                         /* 启动维持时间 */
            U16 freq_start_dly_1;                           /* 启动平台1时间 */
            U16 freq_rated_dly = val_freq_rated_time;       /* 额定维持时间 */
            INVT_ID_ENUM invt_id = (INVT_ID_ENUM)x;

            /* 计算: 启动维持时间、启动平台1时间 */
            if (invt_para_fa[invt_id][FA_FREQ_START_TIME]/10 <= 0)
            {
                freq_start_dly_max = freq_start_dly_1 = val_freq_stably_ctrl_time;
            }
            else
            {
                freq_start_dly_max = MAX((invt_para_fa[invt_id][FA_FREQ_START_TIME]/10), val_freq_stably_ctrl_time);   
                freq_start_dly_1 = invt_para_fa[invt_id][FA_FREQ_START_TIME]/10;
            }
                
            /* 限降频标志处理 */
            if (log_mod_freq_limit_check(x, Cm[x].run_mode))
            {
                fg_limit = TRUE;
            }
            
            if (s_get_cm_status(x, CM_ON))    
            {
                /* 启动过程 */
                if (cm_time[x].on <= freq_start_dly_max)
                {
                    fg_starting = TRUE;
                }
                else if (CmRotation[x] >= freq_rated
                     ||  fg_limit
                    )
                {
                    fg_starting = FALSE;
                }

//                if (invt_common_run_time[x] >= val_m_motor_start_time
//                &&  (CmRotation[x]>=freq_rated || fg_adjust_run_bak[x])
//                    )
                /* 进入调节允许标志处理 */
                if ((CmRotation[x]==freq_rated && invt_common_run_time[x] >= freq_rated_dly)
                 || fg_adjust_run_bak[x]
                 || invt_common_run_time[x] >= freq_start_dly_max
                 || SensorCanUse(freq_before_reoil[x])      /* 回油结束不必回到额定转速 */
                 || fg_limit
                 )
                {
                    fg_adjust_allow = TRUE;
                    /* 超时未恢复到额定转速则直接给额定转速 */
                    if (invt_common_run_time[x] >= freq_start_dly_max
                    &&  !fg_adjust_run_bak[x]
                        )
                    {
                        CmRotation[x] = freq_rated;
                    }
                }

                // 判断压机是否需要回油  
                ReOilFrequency[x] = log_cmp_is_reoil(x);
                
                /* 处理压机频率 */
                if (TF_OPEN_COMP)                               /* 商检 */
                {
                    CmRotation[x] = TF_comp_freq;
                }
                else if (Cm[x].stage==CM_DEFROSTING)            // 压机除霜
                {
                    CmRotation[x]= log_mod_comp_defrost_freq(x);
                }
                else if(val_invt_manual_mode == USED)           // 手动
                {
                    CmRotation[x] = val_invt_manual_rps;
                }
                else if (fg_ability_test_run)                   /* 能力测试 */
                {
                    CmRotation[x] = AT_VAL(val_invt_manual_rps, val_ability_test_invt_rps);
                }
                else if (fg_refri_recy_act)                     /* 收氟 */
                {
                    CmRotation[x] = freq_rated;
                }
                else if(ReOilFrequency[x]!=0)                   // 回油
                {
                    if (CmRotation[x] != ReOilFrequency[x])
                    {
                        freq_before_reoil[x] = CmRotation[x];   /* 记录回油前转速 */
                    }
                    CmRotation[x] = ReOilFrequency[x];
                }
                else if (CmRotation[x]==0                       // 启动调节
                ||  fg_starting)
                {
                    static I16 freq_start_2;/* 启动平台2转速 */
                    if (cm_time[x].on <= freq_start_dly_1)
                    {
                        CmRotation[x] = MAX(freq_start, invt_info_freq_now(invt_id));
                        if (temp_env <= -2)
                        {
                            freq_start_2 = val_low_env_start_freq;	/* 【低环启动维持转速】 */
                        }
                        else
                        {
                            freq_start_2 = val_invt_start_freq2;	/* 【压机启动维持转速】 */
                        }
                    }
                    else if (cm_time[x].on <= freq_start_dly_max)
                    {
                        CmRotation[x] = freq_start_2;
                    }
                    else
                    {
                        CmRotation[x] = fun_pv_limit(freq_start_2 + MIN(30, (_DIVUCARRY(cm_time[x].on-freq_start_dly_max, val_invt_start_period)))*val_invt_start_speed, freq_start_2, freq_rated);
                    }
                }
                else
                {
                    /* 正常运行 */
                    fg_common_run = TRUE;
                    
                    if (!fg_adjust_allow)
                    {
                        I16 freq_common_first = MAX(freq_abnormal[x], freq_start);  /* 从异常切到正常的起始频率 */
                        CmRotation[x] = MIN(freq_common_first + MIN(30, _DIVUCARRY(invt_common_run_time[x], val_invt_start_period))*val_invt_start_speed, freq_rated);
                    }
                    else
                    {
                        /* 进入调节 */
                        fg_adjust_run = TRUE;

                        /* 回油结束回到回油前转速 */
                        if (SensorCanUse(freq_before_reoil[x]))
                        {
                            CmRotation[x] = freq_before_reoil[x];
                            freq_before_reoil[x] = SENSOR_NOEXIST;
                        }

                        {
                            U16 freq = Calculate_Frequency_Need(x,Cm[x].run_mode, CmRotation[x]);
                            
                            if (freq > CmRotation[x])
                            {
                                invt_cm_sta[x] = INVT_CM_UP;
                            }
                            else if (freq < CmRotation[x])
                            {
                                invt_cm_sta[x] = INVT_CM_DOWN;
                            }
                            else
                            {
                                invt_cm_sta[x] = INVT_CM_KEEP;
                            }

                            CmRotation[x] = freq;
                        }
                    }
                }
            }
            else
            {
                CmRotation[x] = 0;
            }

            /* 压机未进入正常运行的处理 */
            if (!fg_common_run)
            {
                invt_common_run_time[x] = 0;
                log_realrun_time(x, 0);
                freq_abnormal[x] = CmRotation[x];
                fg_freq_pre_sub_en = TRUE;
                fg_freq_had_force_sub = FALSE;
            }

            /* 压机未进入调节的处理 */
            if (!fg_adjust_run)
            {
                pid_set_AveAcrossCnt(PID_ID_INVT_1, 0);
                pid_read_Uk(PID_ID_INVT_1);                     /* 读取并清除Uk */
            }

            fg_adjust_run_bak[x] = fg_adjust_run;
        }
    }
    
    /* 风机 */
    fan_logic();
    ec_fan();           /* EC风机：其他厂家驱动 */
    
    log_mod_comp_status();
}

/*******************************************************************************
 函 数: log_mod_unit_to_comp_group()
 功 能: 通过单元号获取压机组
 参 数: n_comp--一个单元包含的压机数;
        begin--压机开始序号;
        end--压机结束序号;
        exio_no--单元号;
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_mod_unit_to_comp_group(U08 n_comp, U08 *begin, U08 *end, U08 exio_no)
{
    *begin = exio_no * n_comp;
    *end = MIN(*begin+n_comp, val_comp_num);            /* end最大只能为val_comp_num */
}

/*******************************************************************************
 函 数: log_mod_cmd_by_hmi()
 功 能: 显示屏命令处理
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
static void log_mod_cmd_by_hmi(void)
{   
    /* 以下处理手操器命令 */
    if (!(pCell->cell_status[MAX_COMP] & CELL_RUN))      /*  模块无压机运行时才可初始化 */
    {
        if (fg_para_init) 
        {
            fg_para_init = FALSE;
            DataAccess_FacPara(DATA_INIT);
        }

        if (fg_invt_init) 
        {
            fg_invt_init = FALSE;
            invt_para_init();
        }
    }
    if (fg_limit_init) 
    {
        fg_limit_init = FALSE;
        DataAccess_LimitPara(DATA_INIT);
    }

    
    if (fg_runtime_init)                                 /* 累计运行时间初始化 */
    {
        fg_runtime_init = FALSE;
        DataAccess_CompRuntime(DATA_INIT);              // 初始化压机运行时间
    }
    
    if (fg_systime_init)                                 /* 系统累计运行时间初始化 */
    {
        fg_systime_init = FALSE;
//      systime_initial();
        DataAccess_SysRuntime(DATA_INIT);
    }
    if (fg_systime_save)                                 /* 系统累计运行时间保存 */
    {
        fg_systime_save = FALSE;
//      systime_write_to_eeprom();
        DataAccess_SysRuntime(DATA_WRITE);
    }

    
    if (fg_password_init)                                 // 密码初始化
    {
        fg_password_init = FALSE;
        Password_InitAll();
    }
    

}

/*******************************************************************************
 函 数: s_set_di()
 功 能: 压机开关量考虑常开常闭后的开关量处理
 参 数: input 开关量序号  di_para 常开常闭参数
 di_para: 0 常开 ； 1 常闭
 返 回: 空
 其 它: 
*******************************************************************************/
void s_set_di(U08 input, I16 di_para, U08 comp_per)
{
    if (s_get_input_first(comp_per, input) ^ di_para)
    {
        s_set_input(comp_per, input);
    }
}

/*******************************************************************************
 函 数: log_mod_begin_pv_cpst()
 功 能: 处理温度后补偿
 参 数: 空
 返 回: 空
 其 它: 
*******************************************************************************/
void log_mod_begin_pv_cpst(void)
{
    /* 水温补偿值：防冻不补偿 */
    if (!fg_idle_sta_antifreeze				/* 非待机态防冻 */
    && !MOD_IS_RUN_STA_AC_AF(pcb_addr)		/* 非运行态空调水路防冻 */
    && !MOD_IS_RUN_STA_HOTW_AF(pcb_addr)	/* 非运行态热水水路防冻 */
    )
    {
        I16 temp_cpst = CmIsCool(0) ? val_ctrl_cpst_cool : val_ctrl_cpst_heat;

        pv_cell_TWin1(pcb_addr) = fun_pv_cmpst(pv_cell_TWin1(pcb_addr),  temp_cpst);
        pv_cell_TWout1(pcb_addr) = fun_pv_cmpst(pv_cell_TWout1(pcb_addr),  temp_cpst);
    }

#if 0
    /* 高水温补偿值 */
    pv_cell_TWin1(pcb_addr) = log_sys_circle_water_cpst(pv_cell_TWin1(pcb_addr));
    pv_cell_TWout1(pcb_addr) = log_sys_circle_water_cpst(pv_cell_TWout1(pcb_addr));
#endif
}

/*****************************************************************************
函  数 : log_mod_limit()
功  能 : 根据条件处理模块输入限制,和计算相关数据
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
static void log_mod_limit(void)
{
    U08  i;
    
    for (i=0; i<self_comp_num; i++)
    {
//        /* 高压 */
//        if (val_J6_prs_sensor_type == UNUSED)
//        {
//            pv_cell_hp(i)     = SENSOR_NOEXIST;            
//            pv_cell_cond_temp(i)   = SENSOR_NOEXIST;
//        }
//        /* 低压 */
//        if (val_J5_prs_sensor_type == UNUSED) 
//        {
//            pv_cell_lp(i)      = SENSOR_NOEXIST;
//            pv_cell_evap_temp(i)   = SENSOR_NOEXIST;
//        }

#if defined(M_DISEN)
        /* 阀后温度 */
        if (USE_COOL_LP || val_s_motor_use == EVI_ECO_OVERHEAT)
        {
            pv_cell_evap(i) = SENSOR_NOEXIST;
        }

        /* 经济器出口温度&经济器入口温度 */
        if (val_s_motor_use != EVI_ECO_OVERHEAT)
        {
            pv_cell_evi_in(i)  = SENSOR_NOEXIST;
            pv_cell_evi_out(i) = SENSOR_NOEXIST;
        }
#else
        /* 阀后温度 */
        if (USE_COOL_LP)
        {
            pv_cell_evap(i) = SENSOR_NOEXIST;
        }

        pv_cell_evi_in(i)  = SENSOR_NOEXIST;
        pv_cell_evi_out(i) = SENSOR_NOEXIST;
#endif

        /* 处理电膨数据 */
        if(val_s_motor_use == EVI_UNUSED)
        {
            pv_cell_step_s(i) = SENSOR_NOEXIST;
        }
        if (val_motor_use == EEV_UNUSED)
        {
            pv_cell_step_m(i) = SENSOR_NOEXIST;
            pv_cell_gas_in(i) = SENSOR_NOEXIST;
        }

        /* 内机商检时强制检测这几个温度 */
        if (!(TF_HAS_INNER&&TF_EN_NTC)) 
        {
            /* 不使用冷媒液侧温度、不使用冷媒气侧温度 */
            if (val_rfg_temp_corr_en == UNUSED)
            {
                pv_cell_TRFliq(i) = SENSOR_NOEXIST;
                pv_cell_TRFgas(i) = SENSOR_NOEXIST;
            }
        }

//        /* 处理电流 */
//        if (NO_CUR_SENSOR)
//      {
//            pv_cell_curr(i) = SENSOR_NOEXIST;
//      } 

//        /* 压顶温度 */
//        if (val_s_motor_use==EVI_ECO_OVERHEAT
//         || !val_seneor_use_comp_top
//            )
//        {
//            pv_cell_comp_top(i) = SENSOR_NOEXIST;
//        }

//        // 经济器
//        if (val_s_motor_use != EVI_ECO_OVERHEAT
//        &&  val_seneor_disp_evi_temp == UNUSED)
//      {
//            pv_cell_evi_in(i) = SENSOR_NOEXIST;
//            pv_cell_evi_out(i) = SENSOR_NOEXIST;
//        }
    }    
    
//  /* 单元 */  /* N0004 */
//    if (!(val_s_motor_use!=EVI_ECO_OVERHEAT && val_seneor_use_anti_ice==ANTI_ICE_AI7
//          || val_motor_use!=EEV_OVERHEAT_TEMP && val_motor_use!=EEV_OVERHEAT_TEMP_H && val_seneor_use_anti_ice==ANTI_ICE_AI8
//          ))
//    {
//        pv_cell_antifreez(pcb_addr) = SENSOR_NOEXIST;
//    }

    if (val_freq_fan_set == FREQ_FAN_SET_UNUSED)
    {
        pv_cell_fan_set(FREQ_FAN1) = SENSOR_NOEXIST;
        pv_cell_fan_set(FREQ_FAN2) = SENSOR_NOEXIST;
        pv_cell_fan_now(FREQ_FAN1) = SENSOR_NOEXIST;
        pv_cell_fan_now(FREQ_FAN2) = SENSOR_NOEXIST;
    }

    if (val_freq_fan_num == 1)
    {
        pv_cell_fan_set(FREQ_FAN2) = SENSOR_NOEXIST;
        pv_cell_fan_now(FREQ_FAN2) = SENSOR_NOEXIST;
    }

    /* 不使用变频水泵 */
    if (USE_PUMPf_CTRL == PUMPF_CTRL_OFF)
    {
        pv_cell_PUMPf_set(pcb_addr) = SENSOR_NOEXIST;
        pv_cell_PUMPf_cur(pcb_addr) = SENSOR_NOEXIST;
        pv_cell_PUMPf_pow(pcb_addr) = SENSOR_NOEXIST;
    }

    /* 变频水泵警告不显示流量 */
    if (PUMPF_is_warn())
    {
        pv_cell_PUMPf_cur(pcb_addr) = SENSOR_NOEXIST;
        pv_cell_PUMPf_pow(pcb_addr) = SENSOR_NOEXIST;
    }
}

/*******************************************************************************
 函 数: log_mod_get_press()
 功 能: 压力获取&转换
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
static void log_mod_get_press(void)
{
    I16 lp = SENSOR_NOEXIST;
    I16 hp = SENSOR_NOEXIST;
    I16 lp_temp = SENSOR_NOEXIST;
    I16 hp_temp = SENSOR_NOEXIST;
    
    I16 prs_src_j5 = GET_EXIO_PRS(0, EXIO_AI_01);
    I16 prs_src_j6 = GET_EXIO_PRS(0, EXIO_AI_02);

    switch (val_prs_sensor_config)
    {
        case PRS_CFG_REGULAR:
            fg_press_use_j5 = UNUSED;
            fg_press_use_j6 = USED;
            
            if (Cm[0].run_mode == MODE_COOL)
            {
                lp = AiSignalConvert(prs_src_j6, AI_PRS_LOW, 0);
            }
            else if (Cm[0].run_mode == MODE_HEAT)
            {
                hp = AiSignalConvert(prs_src_j6, AI_PRS_HIGH, 0);
            }
            break;
            
        case PRS_CFG_SUCTION:
            fg_press_use_j5 = UNUSED;
            fg_press_use_j6 = USED;
            
            lp = AiSignalConvert(prs_src_j6, AI_PRS_LOW, 0);
            break;
            
        case PRS_CFG_SUC_EXH:
            fg_press_use_j5 = USED;
            fg_press_use_j6 = USED;
            
            lp = AiSignalConvert(prs_src_j6, AI_PRS_LOW, 0);
            hp = AiSignalConvert(prs_src_j5, AI_PRS_HIGH, 0);
            break;
            
        case PRS_CFG_EXHAUST:
            fg_press_use_j5 = USED;
            fg_press_use_j6 = UNUSED;
            
            hp = AiSignalConvert(prs_src_j5, AI_PRS_HIGH, 0);
            break;

        case PRS_CFG_UNUSED:
            fg_press_use_j5 = UNUSED;
            fg_press_use_j6 = UNUSED;
            break;
            
        default:
            break;
    }

    lp_temp = Press_to_Temp(lp, val_refrigerant_type, PRESS_TEMP_DEW);
    hp_temp = Press_to_Temp(hp, val_refrigerant_type, PRESS_TEMP_BUBBLE);
                
    pv_cell_lp(0) = fun_pv_div_10(lp);              /* 低压 */
    pv_cell_evap_temp(0) = fun_pv_div_10(lp_temp);  /* 蒸发饱和温度 */

    pv_cell_hp(0) = fun_pv_div_10(hp);              /* 高压 */
    pv_cell_cond_temp(0) = fun_pv_div_10(hp_temp);  /* 冷凝饱和温度 */
}


static void log_mod_exio_begin(void)
{    
    if (GET_EXIO_HDW_STATUS_READY(0))       /* 状态信息可用 */
    {
        /* 开关量原始值处理 */
        if (GET_EXIO_DI(0, EXIO_DI_02))  s_set_input_first(0, IN_LP);   /* 1#压机低压 */
        if (GET_EXIO_DI(0, EXIO_DI_01))  s_set_input_first(0, IN_HP);   /* 1#压机高压 */
    
        /* 处理常开常闭 */
        s_set_di(IN_LP, val_DI_type_lp, 0);                             /* 1#压机低压 */     
        s_set_di(IN_HP, val_DI_type_hp, 0);                             /* 1#压机高压 */     
    
        /* 压力 */
        log_mod_get_press();
    
        pv_cell_TA(pcb_addr) = GET_EXIO_TMP(0, EXIO_TEMP_01);       /* 环境温度 */
        pv_cell_wing(0)      = GET_EXIO_TMP(0, EXIO_TEMP_02);       /* 翅片温度 */
        pv_cell_gas_out(0)   = GET_EXIO_TMP(0, EXIO_TEMP_04);       /* 排气温度 */
        pv_cell_gas_in(0)    = GET_EXIO_TMP(0, EXIO_TEMP_03);       /* 吸气温度 */
        pv_cell_evap(0)      = GET_EXIO_TMP(0, EXIO_TEMP_05);       /* 阀后温度 */
        pv_cell_evi_in(0)    = GET_EXIO_TMP(0, EXIO_TEMP_05);       /* 经济器入口温度,复用原阀后温度 */
        pv_cell_evi_out(0)   = GET_EXIO_TMP(0, EXIO_TEMP_06);       /* 经济器出口温度 */
        pv_cell_step_m(0)    = GET_EXIO_ST_PV(0, EEV_ID_1);         /* 主阀步数 */
        pv_cell_step_s(0)    = GET_EXIO_ST_PV(0, EEV_ID_2);         /* 辅阀步数 */
    }
    else                    /* 通讯故障，状态信息不可用时，清外机模拟量 */
    {   
        /* 开关量: 已被统一清掉 */

        /* 压力 */
        pv_cell_lp(0)        = SENSOR_NOEXIST;              /* 低压 */
        pv_cell_evap_temp(0) = SENSOR_NOEXIST;              /* 蒸发饱和温度 */        
        pv_cell_hp(0)        = SENSOR_NOEXIST;              /* 高压 */
        pv_cell_cond_temp(0) = SENSOR_NOEXIST;              /* 冷凝饱和温度 */

        pv_cell_TA(pcb_addr) = SENSOR_NOEXIST;              /* 环境温度 */
        pv_cell_wing(0)      = SENSOR_NOEXIST;              /* 翅片温度 */
        pv_cell_gas_out(0)   = SENSOR_NOEXIST;              /* 排气温度 */
        pv_cell_gas_in(0)    = SENSOR_NOEXIST;              /* 吸气温度 */
        pv_cell_evap(0)      = SENSOR_NOEXIST;              /* 阀后温度 */
        pv_cell_evi_in(0)    = SENSOR_NOEXIST;              /* 经济器入口温度 */
        pv_cell_evi_out(0)   = SENSOR_NOEXIST;              /* 经济器出口温度 */
        pv_cell_step_m(0)    = SENSOR_NOEXIST;              /* 主阀步数 */
        pv_cell_step_s(0)    = SENSOR_NOEXIST;              /* 辅阀步数 */      
    }
}
 
/*******************************************************************************
 函 数: log_mod_begin()
 功 能: 模块逻辑起始
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
static void log_mod_begin(void)
{
    U08 i;

    for (i=0; i<MAX_COMP+1; i++)
    {
        mod_input[i] = 0;           /* 开产量输入刷新(处理常开常闭后的) */
        pCell->cell_relay[i] = 0;   /* 继电器数据清零 */
        pCell->cell_input[i] = 0;   /* 开产量输入刷新(开关量原始值) */
    }

    /* 外机板 */
    log_mod_exio_begin();

    /* 外机商检时不检测内机开关量 */
    if (!TF_IS_OUT)
    {
        /* 开关量原始值处理 */
        /*-------物理位置--------; --------逻辑位置-------*/
        if (GET_DI(DI_01))  s_set_input_first(MAX_COMP, S_IN_WATER_FLOW); /* 水流开关 */
        if (GET_DI(DI_08))  s_set_input_first(MAX_COMP, S_IN_IBH_OL);     /* 辅助电热反馈 */

        /* 处理常开常闭 */
        s_set_di(S_IN_WATER_FLOW, val_DI_type_air_flux, MAX_COMP);        /* 水流开关 */
        s_set_di(S_IN_IBH_OL, NORMAL_CLOSE, MAX_COMP);			          /* 辅助电热反馈(固定常闭) */
    }
    
    
    
    /* 单元模拟量 */
    pv_cell_TWin1(pcb_addr)  = GET_TEMP(TEMP_09);           /* 板换回水温度 */
    pv_cell_TWout1(pcb_addr) = GET_TEMP(TEMP_08);           /* 板换出水温度 */
	if (val_TWout2_sensor_used)
	{
    	pv_cell_TWout2(pcb_addr) = GET_TEMP(TEMP_07);           /* 总出水温度 */
	}
	else
	{
    	pv_cell_TWout2(pcb_addr) = GET_TEMP(TEMP_08);           /* 总出水温度探头被设置成禁用，使用板换出水温度替换 */
	}

    pv_cell_PUMPf_set(pcb_addr)      = PUMPf_rpm_sv;		/* 变频水泵目标转速 */
    pv_cell_PUMPf_cur(pcb_addr)      = PUMPf_flow_pv;		/* 变频水泵当前流量 */
    pv_cell_PUMPf_pow(pcb_addr)      = PUMPf_power_pv;		/* 变频水泵当前功率 */
    pv_cell_PUMPf_pwm_out(pcb_addr)  = PUMPf_pwm_out;		/* 变频水泵PWM控制信号(板->泵) */
    pv_cell_PUMPf_pwm_in(pcb_addr)   = PUMPf_pwm_in;		/* 变频水泵PWM反馈信号(泵->板) */
    pv_cell_PUMPf_err_code(pcb_addr) = PUMPf_err_id;		/* 变频水泵故障信息 */
    
    {
        /* 机组标定能力 & 机组当前能力 */
        I16 water_diff = pv_cell_TWout1(pcb_addr) - pv_cell_TWin1(pcb_addr); 
        
        if (ABILITY_TEST_HAS_DEFR)
        {
            if (motor_run_time[0] < 15*60)      /* 压机运行到15min时确定下来 */
            {
                pv_cell_mac_power(pcb_addr) = defrost_cal_power(water_diff);
            }
            pv_cell_mac_power_now(pcb_addr) = defrost_cal_power(water_diff);
        }
#ifdef USE_MAC_POWER        
        else if (USE_ELEC_METERING)
        {
            pv_cell_mac_power_now(pcb_addr) = defrost_cal_power(water_diff);
        }
#endif        
        else
        {
            pv_cell_mac_power(pcb_addr) = SENSOR_NOEXIST;
            pv_cell_mac_power_now(pcb_addr) = SENSOR_NOEXIST;
        }
    }

    /* 压机模拟量 */
    pv_cell_TRFgas(0) = GET_TEMP(TEMP_06);  /* 冷媒气侧温度 */
    pv_cell_TRFliq(0) = GET_TEMP(TEMP_05);  /* 冷媒液侧温度 */

    //风机速度更新
    if (val_freq_fan_set == FREQ_FAN_SET_PWM)
    {
        
        pv_cell_fan_now(FREQ_FAN1) = speedpv[FREQ_FAN1];
        pv_cell_fan_now(FREQ_FAN2) = speedpv[FREQ_FAN2];

        pv_cell_fan_set(FREQ_FAN1) = speedsv[FREQ_FAN1];
        pv_cell_fan_set(FREQ_FAN2) = speedsv[FREQ_FAN2];
    }
    else if (val_freq_fan_set == FREQ_FAN_SET_PWM_EX)
    {
//        /* rpm = pulse ÷ 每转脉冲数 × 60 */
        pv_cell_fan_now(FREQ_FAN1) = (U32)invt_info_pulse_ec1(INVT_ID_1)*60/val_fan_pwm_pulse;
        pv_cell_fan_now(FREQ_FAN2) = (U32)invt_info_pulse_ec2(INVT_ID_1)*60/val_fan_pwm_pulse;

//        pv_cell_fan_now(FREQ_FAN1) = invt_info_pulse_ec_filt(INVT_ID_1, FREQ_FAN1);
//        pv_cell_fan_now(FREQ_FAN2) = invt_info_pulse_ec_filt(INVT_ID_1, FREQ_FAN2);

        pv_cell_fan_set(FREQ_FAN1) = speedsv[FREQ_FAN1];
        pv_cell_fan_set(FREQ_FAN2) = speedsv[FREQ_FAN2];
    }
    else if (val_freq_fan_set == FREQ_FAN_SET_EC)
    {
        /* 风速显示为1RPM: 1RPS = 60RPM */
        pv_cell_fan_now(FREQ_FAN1) = (U32)invt_info_freq_now(INVT_FAN1)*60/100;
        pv_cell_fan_now(FREQ_FAN2) = (U32)invt_info_freq_now(INVT_FAN2)*60/100;
        
        pv_cell_fan_set(FREQ_FAN1) = speedsv[FREQ_FAN1];
        pv_cell_fan_set(FREQ_FAN2) = speedsv[FREQ_FAN2];
    }
    ec_fan_rd_update();

    /* 变频信息 */
    {
        INVT_ID_ENUM id;
        
        {
            id = INVT_ID_1;
#ifdef USE_LAB
            if (1)
#else
            if (val_inverter_used)
#endif
            {
                pv_cell_invt_set(id)   = CmRotation[id]/10;            // 压机目标转速(0.1rps)   
                pv_cell_invt_cur(id)   = invt_info_freq_now(id)/10;    // 压机当前转速(0.1rps)
                pv_cell_invt_volt(id)     = invt_info_voltage_out(id);    // 电压   
                pv_cell_invt_curr(id)     = invt_info_current_out(id);    // 电流   
                pv_cell_invt_tmp(id)       = invt_info_temperature(id);    // 变频器温度
                pv_cell_invt_msg(id)      = invt_info_freq_limit_id(id);  // 变频器限频信息     
                pv_cell_invt_curr_in(id)   = invt_info_errorcode1_id(id);  // 变频输入电流 
                pv_cell_invt_power_out(id) = invt_info_powerout(id);       // 变频输出功率   
            }                          
            else                       
            {                          
                pv_cell_invt_set(id)        = SENSOR_NOEXIST; // 压机目标转速(0.1rps)    
                pv_cell_invt_cur(id)        = SENSOR_NOEXIST; // 压机当前转速(0.1rps)
                pv_cell_invt_volt(id)       = SENSOR_NOEXIST; // 电压       
                pv_cell_invt_curr(id)       = SENSOR_NOEXIST; // 电流     
                pv_cell_invt_tmp(id)        = SENSOR_NOEXIST; // 变频器温度    
                pv_cell_invt_msg(id)        = SENSOR_NOEXIST; // 变频器限频信息 
                pv_cell_invt_curr_in(id)    = SENSOR_NOEXIST; // 变频输入电流 
                pv_cell_invt_power_out(id)  = SENSOR_NOEXIST; // 变频输出功率 
            }
        }
        
        for (id=INVT_ID_1; id<INVT_ID_MAX; id++)
        {
            CELL_INVT_INFO(pcb_addr, id,  0) = invt_info_freq_state(id);
            CELL_INVT_INFO(pcb_addr, id,  1) = invt_info_freq_run(id)/10;   // 压机目标转速(0.1rps)  ;
            CELL_INVT_INFO(pcb_addr, id,  2) = invt_info_temperature(id);
            CELL_INVT_INFO(pcb_addr, id,  3) = invt_info_powerout(id);
            CELL_INVT_INFO(pcb_addr, id,  4) = invt_get_rps_out(id)/10;
            CELL_INVT_INFO(pcb_addr, id,  5) = invt_info_current_out(id);
            CELL_INVT_INFO(pcb_addr, id,  6) = invt_info_torque_out(id);
            CELL_INVT_INFO(pcb_addr, id,  7) = invt_info_voltage_out(id);     
            CELL_INVT_INFO(pcb_addr, id,  8) = invt_info_volt_generatrix(id);
            CELL_INVT_INFO(pcb_addr, id,  9) = invt_info_error_id(id);
            CELL_INVT_INFO(pcb_addr, id, 10) = invt_info_errorcode1_id(id);
            CELL_INVT_INFO(pcb_addr, id, 11) = invt_info_temp_pfc(id);
            CELL_INVT_INFO(pcb_addr, id, 12) = invt_info_freq_now(id)/10;
            CELL_INVT_INFO(pcb_addr, id, 13) = invt_info_freq_limit_id(id);
            CELL_INVT_INFO(pcb_addr, id, 14) = 14;
        }
    }

//    /* 模块功率 */
//    pv_cell_power(pcb_addr) = pv_cell_invt_power_out(INVT_ID_1);

    /* 变频水泵状态 */
    PUMPF_status();

    /* 限制压机环温分段 */
    limit_comp_TA_area = log_mod_limit_comp_TA_area(ctrl_mode[GET_SUB_SYS(pcb_addr)], GET_SUB_SYS(pcb_addr));
    
    /* 制热环温分段 */
#ifdef USE_HT_ENV_NEW
    env_area_heat = fun_area(pv_cell_TA(pcb_addr), env_area_heat, 5, 
                            val_heat_sv_chg_env1, val_heat_sv_chg_env2,
                            val_heat_sv_chg_env3, val_heat_sv_chg_env4,
                            val_heat_sv_chg_env5, val_heat_sv_chg_env6,
                            val_heat_sv_chg_env7, val_heat_sv_chg_env8,
                            val_heat_sv_chg_env9, SENSOR_NOEXIST);
#else
    env_area_heat = fun_area(pv_cell_TA(pcb_addr), env_area_heat, 5, 
                            val_heat_sv_chg_env1, val_heat_sv_chg_env2,
                            val_heat_sv_chg_env3, val_heat_sv_chg_env4,
                            val_heat_sv_chg_env5, val_heat_sv_chg_env6,
                            val_heat_sv_chg_env7, val_heat_sv_chg_env8,
                            SENSOR_NOEXIST);
#endif

    /* 制冷回温分段 */
    in_area_cool = fun_area(pv_cell_TWin1(pcb_addr), in_area_cool, 5, 
                            val_cool_sv_chg_in1, val_cool_sv_chg_in2,
                            SENSOR_NOEXIST);    

    log_mod_limit();

    /* 温度后补偿 */
    log_mod_begin_pv_cpst();

	/* 模块禁用或通讯故障时，清除相关广播特殊数据，防止影响从机 */
	if (!CELL_Is_used(pcb_addr) || self_commu_err)
	{
		log_sys_init_idle_sta_antifreeze();
		log_sys_init_run_sta_antifreeze();
		log_sys_init_timed_antifreeze();	
	}	

    /* 调试信息归零 */
    {
        ARRCLR(fan_adjust_fg);
        ARR2SET(fan_adjust_tmp, 0);
        ARR2SET(eev_adjust_tmp, 0);
        ARR2SET(evi_adjust_tmp, 0);
    }
}

/*******************************************************************************
 函 数: log_mod_exio_end()
 功 能: 外机输出处理
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
static void log_mod_exio_end(void)
{
    /* 通讯故障，状态信息不可用时，清外机输出 */
    if (!GET_EXIO_HDW_STATUS_READY(0))
    {
        s_clr_output(0, SW_byway);
        s_clr_output(0, SW_S1);
        s_clr_output(MAX_COMP, S_DO_DPH);
        s_clr_output(MAX_COMP, S_DO_DPH2);
        s_clr_output(0, SW_crank2);
        s_clr_output(0, SW_crank1);
    }
    
    /* 喷液阀 */
    if (s_get_output(0, SW_byway))
    {
        SET_EXIO_DO(0, EXIO_DO_03); 
    }
    else
    {
        CLR_EXIO_DO(0, EXIO_DO_03);
    }
    
    /* 四通阀  */
    if (s_get_output(0, SW_S1))
    {
        SET_EXIO_DO(0, EXIO_DO_04); 
    }
    else
    {
        CLR_EXIO_DO(0, EXIO_DO_04);
    }
    
    /* 底盘电加热 */
    if (s_get_output(MAX_COMP, S_DO_DPH))
    {
        SET_EXIO_DO(0, EXIO_DO_05);
    }
    else
    {
        CLR_EXIO_DO(0, EXIO_DO_05);
    }

    /* EXDO7 */
    {
        BOL fg_exdo7 = FALSE;

        switch (val_EXDO7_selfdef)
        {
            default:
            case EXDO7_IS_CRANK2:
                /* 曲轴电加热2 */
                if (s_get_output(0, SW_crank2))         fg_exdo7 = TRUE;
                break;
                
            case EXDO7_IS_DPH2:
                /* 底盘电加热2 */
                if (s_get_output(MAX_COMP, S_DO_DPH2))  fg_exdo7 = TRUE;
                break;
        }

        if (fg_exdo7)
        {
            SET_EXIO_DO(0, EXIO_DO_07);
        }
        else
        {
            CLR_EXIO_DO(0, EXIO_DO_07);
        }
    }
        
    /* 曲轴电加热1 */
    if (s_get_output(0, SW_crank1))
    {
        SET_EXIO_DO(0, EXIO_DO_08);
    }
    else
    {
        CLR_EXIO_DO(0, EXIO_DO_08);
    }
}

/***********************************************
函数：Logic_OutputChange
功能：处理输出转换，将输出状态传给最终输出变量
参数：模块序号
返回值：无
*************************************************/
static void log_mod_end(void)
{
	U08 i;
    SET_DO(DO_13);          /* IBH/TBH零线总开关 */

    SHC_set_mod_test_do();

	/* 模块禁用或通讯故障时，关闭模块输出 */
	if (!CELL_Is_used(pcb_addr) || self_commu_err)
	{
		for (i=0; i<MAX_COMP+1; i++)
		{
        	pCell->cell_relay[i] = 0;   /* 继电器数据清零 */
			pCell->cell_status[i] = 0;   /* 继电器数据清零 */
		}
	}

    /* 变频压机 */
    {
        INVT_ID_ENUM id = INVT_ID_1;
        //启停变频器
        if (s_get_output(0, SW_CM)
        && (!invt_info_is_running(id) && !invt_info_is_alarm(id))
        )    
        {
            invt_cmd_on(id);
        }
        
        if (invt_info_is_running(id)
        && (!s_get_output(0, SW_CM) || invt_info_is_alarm(id))
        )
        {
            I16 freq_min = val_invt_rps_min;
            
            CmRotation[id] = 0;
            if (invt_off_speed_down_cnt[id] >= val_off_speed_down_time
            ||  invt_info_freq_now(id) <= freq_min+COMP_CLOSE_FREQ_DIFF
                )
            {
                invt_cmd_off_free(id);
            }
        }
        else
        {
            invt_off_speed_down_cnt[id] = 0;
        }

        //发送压机转速给变频器
        CmRotation_out[id] = log_mod_avoid_freq(CmRotation[id], id);
        invt_set_rps_out(id,CmRotation_out[id]);

        /* pfc设置：性能模式时按需控制，退出性能模式时恢复pfc开启 */
        {
            BOL fg_cmd_fpc = TRUE;
            static BOL fg_cmd_fpc_bak = TRUE;
            
            if (fg_ability_test_run)
            {
                switch (val_ability_test_pfc_set)
                {
                    case PFC_SET_OPEN:
                        fg_cmd_fpc = TRUE;
                        break;
                    case PFC_SET_CLOSE:
                        fg_cmd_fpc = FALSE;
                        break;
                    case PFC_SET_AUTO:
                        if (CmRotation_out[id] > 60*100)
                        {
                            fg_cmd_fpc = TRUE;
                        }
                        else
                        {
                            fg_cmd_fpc = FALSE;
                        }
                        break;
                }
            }

            if (fg_cmd_fpc_bak != fg_cmd_fpc)
            {
                if (fg_cmd_fpc)
                {
                    invt_cmd_pfc_on(INVT_ID_1);
                }
                else
                {
                    invt_cmd_pfc_off(INVT_ID_1);
                }

                fg_cmd_fpc_bak = fg_cmd_fpc;
            }
        }
    }

    // EC风机/* N0004 */
    if (val_freq_fan_set == FREQ_FAN_SET_EC)
    {
        INVT_ID_ENUM id;
        for (id=INVT_FAN1; id<=INVT_FAN2; id++)
        {
            if (!invt_info_is_running(id) && !invt_info_is_alarm(id))
            {
                if (speedsv[id-INVT_FAN1+FREQ_FAN1] > 0)
                {
                    invt_cmd_on(id);
                }
            }

            if (invt_info_is_running(id))
            {
                if (speedsv[id-INVT_FAN1+FREQ_FAN1]== 0 || invt_info_is_alarm(id))
                {
                    invt_cmd_off_free(id);
                }
            }

            // 发送转速
            {
                U16 speed_sv = (I32)speedsv[id-INVT_FAN1+FREQ_FAN1]*100/60; /* 1rpm->0.01rps */
                invt_set_rps_out(id, speed_sv);
            }
        }
    }
    else if (val_freq_fan_set == FREQ_FAN_SET_PWM_EX)   /* 发送ec风机转速给变频器 */
    {
        U08 i = 0;

        for (i=0; i<FREQSPEEDNUM; i++)
        {
            speed_percent[i] = pwm_fan_speed_to_percent(i, speedsv[i], pv_cell_fan_now(i));
            invt_set_rps_ec(INVT_ID_1, i, speed_percent[i]);
        }
    }
    else if (val_freq_fan_set == FREQ_FAN_SET_PWM)          /* 主控PWM风机 */
    {
        U08 i;
        
        for (i=0; i<FREQSPEEDNUM; i++)
        {
            SET_EXIO_FAN_SV(0, i, speed_percent[i]);
        }
    }
    
//    if (s_get_output(0, SW_CM))   
//    {
//      SET_EXIO_DO(0, EXIO_DO_01);
//  }
//  else
//  {
//      CLR_EXIO_DO(0, EXIO_DO_01);
//  }

//  if (s_get_output(0, SW_FAN_LOW))
//  {
//      SET_EXIO_DO(0, EXIO_DO_02); // 低速风机 
//  }
//  else
//  {
//      CLR_EXIO_DO(0, EXIO_DO_02); // 低速风机 
//  }

    /* 变频水泵 */
    if (s_get_output(MAX_COMP, S_DO_PUMPf))             
    {
        SET_DO(DO_01);
    }

    /* 辅助电热未过载 */
    if (!s_get_input(MAX_COMP, S_IN_IBH_OL))
    {
        /* 辅助电热1 */
        if (s_get_output(MAX_COMP, S_DO_IBH1))
        {
            SET_DO(DO_05);
        }
    
        /* 辅助电热2 */
        if (s_get_output(MAX_COMP, S_DO_IBH2))
        {
            SET_DO(DO_06);
        }
    }

    /* 防冻结电加热带 */
    if (s_get_output(MAX_COMP, S_DO_HT))
    {
        SET_DO(DO_08);
        SET_DO(DO_14);
    }

    /* 外机板输出 */
    log_mod_exio_end();
}

/******************************************************
函数：Logic_Reset
功能：处理当机组待机或故障时的复位
参数：无
参数：无
*******************************************************/
//static void s_Logic_idle(void)
//{
//  U08 x, y;
//  
//  if (fg_freeze_ac_act || fg_wt_antifreeze_action)   /* 防冻时不初始化以下变量 */
//  {
//      return;
//  }
//
//  /* 压机初始化 */
//  for (x=0; x<MAX_COMP+1; x++)
//  {
//      Cm[x].status = 0;
//      
//      Cm[x].run_mode = 0xff;         /* 运行模式初始化 */
//      Cm[x].run_mode_need = 0xff;         /* 运行模式需求初始化 */
//          
//  }
//  
//  Cm[MAX_COMP].fan_speed = 0;         /* 风速初始化 */
//  cnt_fan_force_dly = 0;
//  
//  for (x=0; x<MAX_COMP; x++)
//  {
//      frost_space[x] = 0;
//      cnt_vavle_delay[x] = 0xff;  /* 阀关闭后延时初始化 */
//      fg_defrost_comp_stop[x] = 0; /* 除霜时的压机停机标志初始化 */
//      fg_first_dfrost[x] = 0;
//  }
//}

/*******************************************************************************
 函 数: log_mod_comp_quick_stop()
 功 能: 急停压机
 参 数: comp_num--压机序号; 
 返 回: 空
 其 它: 空
*******************************************************************************/
//void log_mod_comp_quick_stop(U08 comp_num)
//{
//
//    if (s_get_cm_status(comp_num, CM_NEED))      /* 压机开启，关闭压机，并延时关阀 */
//  {
//      s_clr_cm_status(comp_num, CM_NEED);
//      
//      //s_clr_cm_status(comp_num, CM_NEED_DEFROST);        /* 除霜需求 */
//        //s_clr_cm_status(comp_num, CM_DEFROST);        /* 除霜标志 */ 
//        
//        //s_clr_cm_status(comp_num, CM_VALVE_S);    /* 关四通阀 */
//  }
//  else                               /* 压机未开启，直接关阀 */
//  {
//      if (s_get_cm_status(comp_num, CM_VALVE_S))
//      {
//              //s_clr_cm_status(comp_num, CM_VALVE_S);    /* 关四通阀 */
//          }
//  }
//
////    fg_first_dfrost[comp_num] = 0;  /* 清压机首次除霜标志 */
//  fg_cell_run = 0;
//
//
//
//}
/*******************************************************************************
 函 数: log_mod_comp_stop()
 功 能: 停压机
 参 数: comp_num--压机序号; stop_type--停压机类型
 返 回: 空
 其 它: 空
*******************************************************************************/
#define NORMAL_STOP             0   // 正常停制冷系统，风机、四通阀根据延时处理
#define QUICK_ONE_COMP_STOP     1   // 急停1个压机
#define QUICK_ONE_CIRCLE_STOP   2   // 急停1个制冷系统
#define QUICK_ALL_STOP          3   // 急停本模块所有制冷系统

void log_mod_comp_stop(U08 comp_num,U08 stop_type)   
{
    U08 i;
    
    if(stop_type == NORMAL_STOP)    // 正常停制冷系统
    {
        if (s_get_cm_status(comp_num, CM_NEED))      /* 压机开启，则关闭压机，四通阀和风机按时间顺序处理 */
        {
            s_clr_cm_status(comp_num, CM_NEED);
        }
    }
    else if(stop_type == QUICK_ONE_COMP_STOP)   // 急停1压机    
    {
        s_clr_cm_status(comp_num, CM_NEED);
        s_clr_cm_status(comp_num, CM_ON);       
    }
    else if(stop_type == QUICK_ONE_CIRCLE_STOP) // 急停1制冷系统    
    {
        s_clr_cm_status(comp_num, CM_NEED);
        Cm[comp_num].stage = CM_IDLE;       
    }
    else if(stop_type == QUICK_ALL_STOP)    // 急停本模块所有制冷系统   
    {
        for (i=0; i<self_comp_num; i++)
        {
            s_clr_cm_status(i, CM_NEED);
            Cm[i].stage = CM_IDLE;

            mod_err_comp_risk[i] = 1;
        }
        fg_cell_pump_open = FALSE;
    }
    
}
/*******************************************************************************
 函 数: log_evi_sw()
 功 能: 增焓阀
 参 数: x--; 
        control_type--辅阀功能码; 
 返 回: 
 其 它: 空
*******************************************************************************/
static void log_evi_sw(U08 x, U08 control_type)
{
    BOL fg_open = FALSE;
    
    if (control_type != EEV_CMD_STOP)
    {
        fg_open = TRUE;
    }

    if (fg_open)     s_set_output(x, SW_EVI);
}
/*******************************************************************************
 函 数: log_mod_cmd()
 功 能: 主机发送命令处理
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
static void log_mod_cmd(void)
{
    U08 i;
    U16 cmd;
        
    if (pCell->cell_cmd[MAX_COMP]) 
    {
//      cmd_debug2 = pCell->cell_cmd[MAX_COMP];
//      debug_7[3]++;
        cmd = pCell->cell_cmd[MAX_COMP];
        pCell->cell_cmd[MAX_COMP] = 0;
        
        if (cmd & CMD_STOP_ALL_COMP)      /* 停机(接收广播信息, 急停) */
        {   
//          for (i=0; i<self_comp_num; i++)
//          {
//            log_mod_comp_quick_stop(i);
//            s_clr_cm_status(i, CM_VALVE_S);
//            s_clr_cm_status(i, CM_ON);
//            s_clr_cm_status(i, CM_FAN);
//          }
            log_mod_comp_stop(0,QUICK_ALL_STOP);     
        }

        if (cmd & CMD_STOP_ALL_MAC)       /* 停所有输出器件(接收广播信息, 急停) */
        {   
//          for (i=0; i<self_comp_num; i++)
//          {
//              log_mod_comp_quick_stop(i);
//              s_clr_cm_status(i, CM_VALVE_S);
//              s_clr_cm_status(i, CM_ON);
//              s_clr_cm_status(i, CM_FAN);
////                s_fg_stop_all_mac = TRUE;
//          }
            log_mod_comp_stop(0,QUICK_ALL_STOP);
        }
        if ((cmd >> CMD_MODE) & 0x0007) 
        {   /* 模式 */
            for (i=0; i<self_comp_num; i++)
            {
                Cm[i].run_mode_need = ((cmd >> CMD_MODE) & 0x0007);
            }
            Cm[MAX_COMP].run_mode_need = ((cmd >> CMD_MODE) & 0x0007);   
        }
        
        if (cmd & CMD_ERR_RESET)          /* 复位 */
        {   
            fg_err_reset = TRUE;
        }
        if (cmd & CMD_FAC_PARA_INIT)      /* 参数初始化 */
        {   
            fg_para_init = TRUE;
        }
        if (cmd & CMD_INVT_INIT)      /* 变频器初始化 */
        {   
            fg_invt_init = TRUE;
        }
        if (cmd & CMD_LIMIT_INIT)      /* 系统维护参数初始化 */
        {   
            fg_limit_init = TRUE;
        }
        if (cmd & CMD_RUNTIME_INIT)   /* 累计运行时间初始化 */   
        {   
            fg_runtime_init = TRUE;
        } 
        if (cmd & CMD_SYSTIME_INIT)   /* 系统累计运行时间初始化 */
        {   
            fg_systime_init = TRUE;
        }
        if (cmd & CMD_SYSTIME_SAVE)   /* 系统累计运行时间保存 */
        {   
            fg_systime_save = TRUE;
        }
        if (cmd & CMD_PASSWORD_INIT)   // 密码初始化
        {   
            fg_password_init = TRUE;
        }
        if (cmd & CMD_OPEN_CELL)
        {
            fg_cell_pump_open = TRUE;
        }
        if (cmd & CMD_CLOSE_CELL)
        {
            fg_cell_pump_open = FALSE;
            cnt_PUMPf_off_dly = 0;
        }
    }
    for (i=0; i<self_comp_num; i++)
    {
        if (pCell->cell_cmd[i]) 
        {
//          cmd_debug[i] = pCell->cell_cmd[i];
//          debug_7[0]++;
            cmd = pCell->cell_cmd[i];
            pCell->cell_cmd[i] = 0;
            
            if ((cmd >> CMD_CM_MODE) & 0x07) 
            {   /* 模式 */
                Cm[i].run_mode_need = ((cmd >> CMD_CM_MODE) & 0x07);
            }
            
            if (cmd & CMD_STOP_COMP_SYSTEM)     /* 压缩机、风机、四通阀按时间停 */
            { /* 停压机系统 */
//              log_mod_comp_quick_stop(i);
                log_mod_comp_stop(i,NORMAL_STOP);
            }
            else if (cmd & CMD_QUICK_STOP_COMP) /* 压缩机停，风机延时停，电磁阀不停 */
            { /* 急停 */
////                s_set_cm_status(i, CM_QUICK_STOP);
//              s_clr_cm_status(i, CM_NEED);
//              s_clr_cm_status(i, CM_ON);
                log_mod_comp_stop(i,QUICK_ONE_COMP_STOP);
            }
            else if (cmd & CMD_CLOSE_COMP)      /* 压缩机满足要求时停，风机延时停，电磁阀不停 */
            { /* 关 */
                if (log_mod_comp_can_close(i))        /* 压缩机可关 */
                {
//                  s_clr_cm_status(i, CM_NEED);
                    log_mod_comp_stop(i,NORMAL_STOP);
                }

            }
            else if (cmd & CMD_OPEN_COMP)       /* 压缩机满足要求时开 */
            { /* 开 */                
                if (log_mod_comp_can_open(i))         /* 压缩机可开 */
                {
                    s_set_cm_status(i, CM_NEED);
                }
            }

            if (cmd & CMD_ALLOW_DEFROST) 
            { /* 除霜接受 */
                s_set_cm_status(i, CM_DEFROST_ALLOW);
            }
            if (cmd & CMD_FORCE_DEFROST) 
            { /* 强制除霜 */
                SIGNAL_Launch(&defrost_signal_into_namual[i], TRUE);
//              Defrost_force(i);
            }
        }
        
        Cm[i].run_mode = Cm[i].run_mode_need;

        /* 信号后处理 */
        SIGNAL_LaunchEnd(&defrost_signal_into_namual[i]);
    }
    Cm[MAX_COMP].run_mode = Cm[MAX_COMP].run_mode_need;  
    log_mod_cmd_by_hmi();               //显示器命令
}

/*******************************************************************************
 函 数: log_mod_comp_eco_get_coef()
 功 能: 获取排气过热度系数a
 参 数: eva_side_temp--蒸发温度; 
 返 回: 系数a
 其 它: 排过sv = k*出水 + a
*******************************************************************************/
I16 log_mod_comp_eco_get_coef(I16 eva_side_temp)
{
    I16 result;
    
    if (SensorCannotUse(eva_side_temp))   /* 蒸发侧探头故障，取固定过热度目标值 */
    {
        result = val_gas_overheat_a1;
    }
    else 
    {
#ifdef USE_HT_ENV_NEW
        switch(env_area_heat)
        {
            default:
            case AREA_00:       
            case AREA_01: 
                result = val_gas_overheat_a1;
                break;
            case AREA_02:
            case AREA_03:
                result = val_gas_overheat_a2;
                break;
            case AREA_04:   
            case AREA_05:
                result = val_gas_overheat_a3;
                break;
            case AREA_06:
            case AREA_07:
                result = val_gas_overheat_a4;
                break;
            case AREA_08:
            case AREA_09:
                result = val_gas_overheat_a5;
                break;
        }
#else
        switch(env_area_heat)
        {
            default:
            case AREA_00:       
            case AREA_01:   
            case AREA_02:
                result = val_gas_overheat_a1;
                break;
            case AREA_03:
                result = val_gas_overheat_a2;
                break;
            case AREA_04:
            case AREA_05:   
                result = val_gas_overheat_a3;
                break;
            case AREA_06:
                result = val_gas_overheat_a4;
                break;
            case AREA_07:
            case AREA_08:
                result = val_gas_overheat_a5;
                break;
        }
#endif        
    }
    
    return result;
}

/*******************************************************************************
 函 数: log_mod_exh_oh_sv_limit()
 功 能: 排气过热度目标值范围限制
 参 数: x--压机号; 
		sv--原始排气过热度目标值; 
 返 回: 限制后排气过热度目标值
 其 它: 空
*******************************************************************************/
I16 log_mod_exh_oh_sv_limit(U08 x, I16 sv)
{
    I16 pid_sv = 0;
    
    #define TEMP_EXH_OBJ_MIN (55*10)                // 最低目标排温
    #define TEMP_EXH_OBJ_MAX (val_exh_sv_max)       // 最高目标排温
    
    I16 temp_cond = log_mod_get_temp_cond(x);  /* 冷凝温度 */

    pid_sv = fun_pv_limit(sv, TEMP_EXH_OBJ_MIN-temp_cond,
                          TEMP_EXH_OBJ_MAX-temp_cond);

    return pid_sv;
}
/*******************************************************************************
 函 数: log_mod_get_exh_oh_sv()
 功 能: 获取排气过热度目标值
 参 数: x--压机号; 
 返 回: 排气过热度目标值
 其 它: 空
*******************************************************************************/
I16 log_mod_get_exh_oh_sv(U08 x)
{
    I16 pid_sv = 0;
    I16 tmp_out = pv_cell_TWout1(pcb_addr);
    I16 tmp_env = pv_cell_TA(pcb_addr);

    // 确定过热度目标值
    if(Cm[x].run_mode == MODE_COOL)
    {
        pid_sv = val_s_exh_sv_cold;
    }
    else  
    {
        I16 sv_tmp = log_mod_comp_eco_get_coef(tmp_env);
        pid_sv = (I32)sv_tmp + tmp_out*val_gas_overheat_k/10;
    }

    return pid_sv;
}

/*******************************************************************************
 函 数: log_mod_get_exh_oh_pv()
 功 能: 获取排气过热度当前值
 参 数: x--压机号; 
 返 回: 排气过热度当前值
 其 它: 空
*******************************************************************************/
I16 log_mod_get_exh_oh_pv(U08 x)
{
    I16 pid_pv = 0;
    I16 gas_temp = pv_gas_out_high(x);
    I16 temp_cond = log_mod_get_temp_cond(x);  /* 冷凝温度 */

    pid_pv = gas_temp - temp_cond;

    return pid_pv;
}

/*********************************************
函 数: Logic_evi_exh_overheat_ctrl
功 能: 辅路电子膨胀阀控制（过热度控制）
参 数: EEV_ID_ENUM id:电膨序号           U08 x:压机序号
返 回: 无
**********************************************/
static BOL fg_evi_env_allow;    // 辅阀环温允许标志
static void Logic_evi_exh_overheat_ctrl(EEV_ID_ENUM id, U08 x,U08 control_type)
{
    PID_ID_ENUM PID_id;
    I16 pid_pv = 0;
    I16 pid_sv = 0;
    I16 gas_temp = pv_gas_out_high(x);
    
    {
        if (val_s_motor_use == EVI_ECO_OVERHEAT)  //经济器过热度
        {
            // 确定过热度目标值
            pid_sv = fun_line_value(gas_temp,\
                    val_e_ext_2, val_e_ext_1, val_e_sv_2, val_e_sv_1);

            evi_adjust_tmp[x][EVI_ADJ_TMP_BASE] = pid_sv;
            
            // 确定过热度当前值
            if (SensorCannotUse(pv_cell_evi_in(x)) || SensorCannotUse(pv_cell_evi_out(x))) 
            {
                pid_pv = 0;   // 经济器探头故障则设定当前PV为0目的是关闭经济器
            }
            else
            {
                pid_pv = pv_cell_evi_out(x) - pv_cell_evi_in(x);   //经济器过热度出口-进口
            }
        }
        else if (val_s_motor_use == EVI_EXH_OVERHEAT_TMP
             ||  val_s_motor_use == EVI_EXH_OVERHEAT_PRS)  // 排气过热度
        {
            pid_sv = log_mod_get_exh_oh_sv(x);
            evi_adjust_tmp[x][EVI_ADJ_TMP_BASE] = pid_sv;
            pid_sv = log_mod_exh_oh_sv_limit(x, pid_sv);
            evi_adjust_tmp[x][EVI_ADJ_TMP_RANGE] = pid_sv;
            
            pid_pv = log_mod_get_exh_oh_pv(x);
        }

#ifdef USE_AT_EEVS
        /* 能力测试手动目标过热度时 */
        if (USE_ABILITY_EVI_SSH_SV)    
        {
            pid_sv = val_ability_test_evi_ssh_sv;
            SETBIT(evi_adjust_tmp[x][EVI_ADJ_TMP_USAGE], EVI_USAGE_AT);
        }
#endif

        pv_cell_evi_pv(x) = gdebugIO.out_superheatpv = s_motor_pv_sheat = pid_pv;
        pv_cell_evi_sv(x) = gdebugIO.out_superheatsv = s_motor_sv_sheat = pid_sv;
    }

    // 预设PID参数
    if(x==0)    // 1#压机
    {
        PID_id = PID_ID_EEV_2;      // 2#电膨对应2#PID;
        pid_set_ctrl(PID_id, PID_CTRL_INCREMENT);       //增量式
        pid_set_Ts(PID_id, val_s_pid_calculate_period); //计算周期
        pid_set_Kp(PID_id, val_s_pid_Kp);//比例系数
        pid_set_Ti(PID_id, val_s_pid_i);//积分时间
        pid_set_Td(PID_id, val_s_pid_d);//微分时间
        pid_set_Kc(PID_id, val_s_pid_p);//反作用比例带
        pid_set_Ai(PID_id, val_s_pid_flexi);//积分系数

        eev_task_pid(PID_id, id, (EEV_CMD_ENUM)control_type, pid_pv, pid_sv);     // 膨胀阀调节处理
    }
}

/*******************************************************************************
 函 数: log_mod_get_eev_status()
 功 能: 判断主路电子膨胀阀状态
 参 数: x--压机序号; 
 返 回: 见下宏 
 其 它: 空
*******************************************************************************/
#define EEV_STATUS_NORMAL       0       /* 正常 */
#define EEV_STATUS_CANNOT_ON    1       /* 不可开 */
//#define EVI_STATUS_NOEXIST        2       /* 不存在 */
#define EEV_STATUS_OPEN_MAX     3       /* 已开到最大 */
#define EEV_STATUS_OPEN_MIN     4       /* 已关到最小 */
#define EEV_STATUS_OPEN_FIX     5       /* 定开度 */
static U08 log_mod_get_eev_status()
{
    U08 result = EEV_STATUS_NORMAL;
    I16 eev_step = GET_EXIO_ST_PV(0, EEV_ID_1);

    if (eev_step == 0)                                  
        result = EEV_STATUS_CANNOT_ON;                  /* 不可开 */               
    else if (eev_step >= eev_get_steps_run_max(EEV_ID_1))                         
        result = EEV_STATUS_OPEN_MAX;                   /* 已开到最大 */              
    else if (eev_step <= eev_get_steps_min(EEV_ID_1))                          
        result = EEV_STATUS_OPEN_MIN;                   /* 已关到最小 */         
    else if (val_motor_manual_mode == USED)               
        result = EEV_STATUS_OPEN_FIX;                   /* 定开度 */ 

    return result;
}

/*********************************************
函 数: Logic_evi_control
功 能: 辅路电子膨胀阀控制
参 数: EEV_ID_ENUM id:电膨序号           U08 x:压机序号
返 回: 无
**********************************************/
#define EVI_INITOD_HT_IN1  ((I16)(250))
#define EVI_INITOD_HT_IN2  ((I16)(330))
#define EVI_INITOD_HT_IN3  ((I16)(430))
#define EVI_INITOD_HT_ENV  ((I16)(160))

static void Logic_evi_control(EEV_ID_ENUM id, U08 x)
{
    INVT_ID_ENUM invt_id = (INVT_ID_ENUM)x;
    U16 step_open_max ;
    U16 step_open_min ;
    U16 valve_step_preopen;
    U08 control_type = 0;
//  I16 open_step= 0;           // 计算开度
    I16 gas_temp = pv_gas_out_high(x);
    U16 freq_now = invt_info_freq_now(invt_id); /* 当前转速 */
    I16 temp_env = pv_cell_TA(pcb_addr);
    I16 temp_out = pv_cell_TWout1(pcb_addr);
    I16 temp_in = pv_cell_TWin1(pcb_addr);
    I16 force_step = MAX(1, (I32)GET_EXIO_ST_PV(0, id)*val_s_motor_force_step/1000);

    BOL fg_evi_for_liquid = FALSE;

    U08 gas_out_trend = fun_value_trend(gas_temp, mod_rec_gas_out_avg[x], 1);
 
    // 电膨参数设置
    step_open_max = EVI_RUN_STEP_MAX;       // 最大运行步数
    step_open_min = EVI_RUN_STEP_MIN;       // 最小运行步数
    
    eev_set_steps_max(id, val_s_motor_step_max);        // 总步数         
    eev_set_steps_run_max(id, step_open_max);           // 最大运行步数         
    eev_set_steps_min(id, step_open_min);               // 最小运行步数         
    
    if (val_s_motor_use == EVI_SMALL_EEV)
    {
        U16 step_defrost  = (U32)val_m_defrost_od * val_motor_step_max / 100;   // 除霜开度
        U16 step_idle = (U32)val_motor_idle_open * val_motor_step_max / 100;    /* 待机步数 */

        step_defrost = AT_VAL(step_defrost, val_ability_test_step);
        eev_set_steps_defrost(id, step_defrost);                                // 除霜步数
        eev_set_steps_idle(id, step_idle);                                      // 待机步数   
    }
    else
    {
        eev_set_steps_idle(id, 0);                          // 待机步数   
        eev_set_steps_defrost(id, 0);                // 除霜步数 
    }
              
    eev_set_period_action(id, val_s_pid_period*10);  // 辅阀动作周期  
                                                        
    eev_set_period_add(id, val_s_pid_period*10);     // 辅阀强开动作周期 
    eev_set_steps_add(id, force_step);                  // 辅阀强开步数 
    eev_set_period_sub(id, val_s_pid_period*10);     // 辅阀强关动作周期(100ms)  
    eev_set_steps_sub(id, force_step);                  // 辅阀强关步数 
                                                        
    eev_set_period_hold(id, val_hold_evi_time*60);      // 暂停调阀动作周期(1s)         
    eev_set_period_cyc(id, val_hold_evi_cyc*60);        // 动作周期(1s)       

    /* 初始开度 */
    if (Cm[x].run_mode == MODE_HEAT)    
    {
        I16 init_od = 0;
        static I16 area;
        static BOL fg_env_high;

        if (temp_env >= EVI_INITOD_HT_ENV)        fg_env_high = TRUE;
        else if (temp_env < EVI_INITOD_HT_ENV-20) fg_env_high = FALSE;
        
        area = fun_area(temp_in, area, 10, 
                        EVI_INITOD_HT_IN3,
                        EVI_INITOD_HT_IN2,
                        EVI_INITOD_HT_IN1,
                        SENSOR_NOEXIST);
        switch (area)
        {
            default:
            case AREA_00:
                init_od = val_evi_heat_init_od4;
                break;
            case AREA_01:
                init_od = val_evi_heat_init_od3;
                break;
            case AREA_02:
                init_od = val_evi_heat_init_od2;
                break;
            case AREA_03:
                init_od = val_evi_heat_init_od1;
                break;
        }

        if (fg_env_high)
        {
            valve_step_preopen = EVI_RUN_STEP_MIN;
        }
        else
        {
            valve_step_preopen = (U32)val_s_motor_step_max*init_od/100;
        }
    }
    else if (Cm[x].run_mode == MODE_COOL)   
    {
        valve_step_preopen = (U32)val_s_motor_step_max*val_evi_cool_init_od/100;
    }
    
    eev_set_steps_preopen(id, valve_step_preopen);           // 预开步数(初始开度)
    eev_set_period_preopen(id, val_s_pid_period*10);         // 预开时间(100ms)

    /* 限制条件 */
    {
        // 强制开大
        U08 eev_sta = log_mod_get_eev_status();	
        
        fg_force_motor[id][FORCE_ADD_MOTOR_EXH] = Condition_judge(fg_force_motor[id][FORCE_ADD_MOTOR_EXH], 
                                                              gas_temp, 
                                                              val_force_ctrl_gas_out-GAS_OUT_HIGH_DIFF1, 
                                                              50, DRCT_ABOVE);
  
        /* 辅阀当喷液阀 */
        if (val_evi_for_liquid && fg_force_motor[id][FORCE_ADD_MOTOR_EXH])
        {
            fg_evi_for_liquid = TRUE;
        }

        //主阀优先，主阀为正常状态时，辅阀自动控制。且不影响其作为喷液阀
        if (eev_sta == EEV_STATUS_NORMAL && val_high_gas_eev_evi_order == EEV_PRIOR)
        {
            fg_force_motor[id][FORCE_ADD_MOTOR_EXH] = FALSE;
        }
        
        if(Cm[x].run_mode == MODE_HEAT)
        {
            // 环温开阀
            fg_evi_allow[id][EVI_OPEN_ENV] = Condition_judge(fg_evi_allow[id][EVI_OPEN_ENV], 
                                                             temp_env, 
                                                             val_evi_open_env_heat, 
                                                             20, DRCT_UNDER);

            /* 因为辅阀关闭会清空fg_evi_allow，故另外做一个变量fg_evi_env_allow供其他地方用 */
            fg_evi_env_allow = fg_evi_allow[id][EVI_OPEN_ENV];

            // 开阀排气过热度
            fg_evi_allow[id][EVI_OPEN_EXH_OH] = Condition_judge(fg_evi_allow[id][EVI_OPEN_EXH_OH], 
                                                             gas_temp-temp_out, 
                                                             val_evi_open_exh_overheat_heat, 
                                                             val_evi_open_exh_overheat_diff, DRCT_ABOVE);
        }
        else if(Cm[x].run_mode == MODE_COOL)
        {
            // 环温开阀
            fg_evi_allow[id][EVI_OPEN_ENV] = Condition_judge(fg_evi_allow[id][EVI_OPEN_ENV], 
                                                             temp_env, 
                                                             val_evi_open_env_cool, 
                                                             20, DRCT_ABOVE);

            // 开阀排气过热度
            fg_evi_allow[id][EVI_OPEN_EXH_OH] = Condition_judge(fg_evi_allow[id][EVI_OPEN_EXH_OH], 
                                                             gas_temp-pv_cell_wing(x), 
                                                             val_evi_open_exh_overheat_cool, 
                                                             val_evi_open_exh_overheat_diff, DRCT_ABOVE);
        }

        // 开阀排温
        fg_evi_allow[id][EVI_OPEN_EXH] = Condition_judge(fg_evi_allow[id][EVI_OPEN_EXH], 
                                                         gas_temp, 
                                                         val_evi_open_exh, 
                                                         val_evi_open_exh_diff, DRCT_ABOVE);

        /* 转速开阀 */
        fg_evi_allow[id][EVI_OPEN_RPS] = Condition_judge(fg_evi_allow[id][EVI_OPEN_RPS], 
                                                         freq_now, 
                                                         val_evi_open_rps, 
                                                         (val_evi_open_rps-val_evi_close_rpg), DRCT_ABOVE);
    }


    // 确定电膨切换命令
    if (val_s_motor_use == EVI_SMALL_EEV)
    {
        if (!s_get_cm_status(x, CM_NEED)
        &&  !invt_info_is_running(INVT_ID_1)        /* 滤掉降频停机过程 */
        )
        {
            control_type = EEV_CMD_RESET;
        }
        else if (Cm[x].defrost_stage != DS_IDLE)
        {
            control_type = EEV_CMD_DEFRO;			/* 除霜开到除霜开度 */
        }
        else if (Cm[x].run_mode==MODE_COOL)
        {
            control_type = EEV_CMD_PREV;            /* 制冷维持初始开度 */
        }
        else
        {
            control_type = EEV_CMD_STOP;            // 制热阀关死
        }
    }
    else
    {
        if (!s_get_cm_status(x, CM_NEED)
        ||  Cm[x].run_mode==MODE_HEAT && val_s_motor_use_heat==UNUSED
        ||  Cm[x].run_mode==MODE_COOL && val_s_motor_use_cool==UNUSED
        ||  Cm[x].defrost_stage!=DS_IDLE
        ||  cm_time[x].on<val_evi_open_dly
        ||  (!fg_evi_allow[id][EVI_OPEN_ENV] && !fg_evi_for_liquid)	/* 辅阀当喷液阀使用时，满足强制开大排温时忽略环温条件 */
        ||  !fg_evi_allow[id][EVI_OPEN_EXH_OH] 
        ||  !fg_evi_allow[id][EVI_OPEN_EXH]
        || (!fg_evi_allow[id][EVI_OPEN_RPS] && !USE_FIX_COMP)
        ||  fg_refri_recy_act
        ) 
        {
            control_type = EEV_CMD_STOP;          // 阀关死

            /* 关闭时清空相关标志 */
            ARRCLR(fg_evi_allow[id]);           
            ARRCLR(fg_evi_forbid[id]);
            ARRCLR(fg_force_motor[id]);
        }
        else    
        {   
            if ((fg_force_motor[id][FORCE_ADD_MOTOR_EXH] 
                 && TREND_IS_RISE(gas_out_trend))
            )
            {
                control_type = EEV_CMD_FORCE_UP;            // 强制开大
            } 
            else if (fg_evi_forbid[id][FORBID_EVI_SUB_EXH])
            {
                control_type = EEV_CMD_LIMIT_DOWN;          // 限制关小
            }
            else if (fg_force_motor[id][FORCE_SUB_MOTOR_OVEREXH])
            {
                control_type = EEV_CMD_FORCE_DOWN;          // 强制关小
            }
            else if (fg_evi_forbid[id][FORBID_EVI_ADD_OVEREXH])
            {
                control_type = EEV_CMD_LIMIT_UP;            // 限制开大
            }
            else
            {
                control_type = EEV_CMD_RUN;                 // 正常调节
            }
        }

        /* 正常调阀时间处理 */
        if (control_type < EEV_CMD_PREV)
        {
            motor_run_time[id] = 0;
        }

        /* 低环高水逻辑 */
        {
            U16 evi_lehw_dly;
            /* 辅阀延时开启时间处理 */
            if (cm_time[x].on < LEHW_EEV_START_TIME+10)
            {
                /* 减去辅阀开机延时 */
                evi_lehw_dly = MAX(30, LEHW_EVI_WAIT_TIME-val_evi_open_dly);
            }
            else
            {
                evi_lehw_dly = LEHW_EVI_WAIT_TIME;
            }
            /* 辅阀初始开度：若满足低环温高水温启动，则先以0步启动一段时间，再转为最大运行开度回到正常控制 */
            if (control_type != EEV_CMD_STOP 
             && fg_low_env_high_water
             && motor_run_time[EEV_ID_2] < evi_lehw_dly 
             && motor_run_time[EEV_ID_1] < LEHW_EEV_START_TIME  /* 防止辅阀单独执行低环高水逻辑 */
             )
            {
                eev_set_steps_preopen(id, 0);
                control_type = EEV_CMD_PREV;
            }
        }
    }

    /* 辅阀使用设置 */
    {
        switch (val_s_motor_use)
        {
            default:
            case EVI_UNUSED:
                /* 不处理 */
                break;
            case EVI_ECO_OVERHEAT:
                SETBIT(evi_adjust_tmp[x][EVI_ADJ_TMP_USAGE], EVI_USAGE_OH);
                break;
            case EVI_EXH_OVERHEAT_TMP:
                SETBIT(evi_adjust_tmp[x][EVI_ADJ_TMP_USAGE], EVI_USAGE_OHT);
                break;
            case EVI_EXH_OVERHEAT_PRS:
                SETBIT(evi_adjust_tmp[x][EVI_ADJ_TMP_USAGE], EVI_USAGE_OHP);
                break;
            case EVI_SMALL_EEV:
                SETBIT(evi_adjust_tmp[x][EVI_ADJ_TMP_USAGE], EVI_USAGE_SML);
                break;
        }
        if (val_s_motor_use_heat)
        {
            SETBIT(evi_adjust_tmp[x][EVI_ADJ_TMP_USAGE], EVI_USAGE_HT);
        }
        if (val_s_motor_use_cool)
        {
            SETBIT(evi_adjust_tmp[x][EVI_ADJ_TMP_USAGE], EVI_USAGE_CL);
        }
    }

    Logic_evi_exh_overheat_ctrl(id, x, control_type);
    log_evi_sw(x, control_type);
}

/*********************************************
    函数：Logic_Init_Scale
    功能：初始开度确定
    参数：无
    返回值：初始开度值
**********************************************/
static U16 Logic_Init_Scale(U08 mode, I16 cond_side_temp, I16 eva_side_temp)
{
    I16 init_scale;    
    
    if (SensorIsError(cond_side_temp) || SensorIsError(eva_side_temp))  /* 探头故障时的判断 */
    {
        if (mode == MODE_COOL)  init_scale = 70;            /* 制冷，使用70% */
        else                    init_scale = 50;            /* 制热，使用50% */
    }
    else                            /* 计算可变初始开度 */
    {
        if (mode == MODE_COOL)
        {
            if (val_init_step_chg_env_cl==0 || cond_side_temp<=val_init_step_chg_env_cl)
            {
                init_scale = 100 - cond_side_temp/10 + 6*eva_side_temp/100;  /* 温度以放大10倍表示，这里要除以10 */
            }
            else
            {
                init_scale = 35 + cond_side_temp/10 + 6*eva_side_temp/100;  /* 温度以放大10倍表示，这里要除以10 */
            }
            
            init_scale = val_motor_start_amplify_cool * init_scale / 10;     /* 初始开度放大 */
            
            /* 初始开度限制在30%--80%之间 */
            if (init_scale > val_cool_max_initial_od) init_scale = val_cool_max_initial_od;
            if (init_scale < val_cool_min_initial_od) init_scale = val_cool_min_initial_od;
        }
        else 
        {
            init_scale = 75 - cond_side_temp/10 + 6*eva_side_temp/100;   /* 温度以放大10倍表示，这里要除以10 */
            /* 制热初开放大系数根据回水分段 */
            {
                I16 amp = 0;
                static I16 area;
                
                area = fun_area(cond_side_temp, area, 10, 
                                val_motor_start_change_heat2,
                                val_motor_start_change_heat1,
                                SENSOR_NOEXIST);
                switch (area)
                {
                    default:
                    case AREA_00:
                        amp = val_motor_start_amplify_heat3;
                        break;
                    case AREA_01:
                        amp = val_motor_start_amplify_heat2;
                        break;
                    case AREA_02:
                        amp = val_motor_start_amplify_heat1;
                        break;
                }

                init_scale = amp * init_scale / 10; /* 初始开度放大 */

                /* 关辅初开放大 */
                if (!fg_evi_env_allow
                &&  val_s_motor_use!=EVI_UNUSED
                &&  val_s_motor_use!=EVI_SMALL_EEV
                    )
                {
                    init_scale = val_motor_start_amp_EVI_off * init_scale / 10;
                }
            }
            

            /* 初始开度限制在30%--80%之间 */
            if (init_scale > val_heat_max_initial_od) init_scale = val_heat_max_initial_od;
            if (init_scale < val_heat_min_initial_od) init_scale = val_heat_min_initial_od;
        }
    }
    
    //init_scale = (U32)init_scale * val_motor_step_max / 100;    /*得到初始步数 */
    
    return (U16)init_scale;
}

/*******************************************************************************
 函 数: log_mod_get_evi_status()
 功 能: 判断辅路电子膨胀阀状态
 参 数: x--压机序号; 
 返 回: 见下宏 
 其 它: 空
*******************************************************************************/
#define EVI_STATUS_NORMAL       0       /* 正常 */
#define EVI_STATUS_CANNOT_ON    1       /* 不可开 */
//#define EVI_STATUS_NOEXIST        2       /* 不存在 */
#define EVI_STATUS_OPEN_MAX     3       /* 已开到最大 */
#define EVI_STATUS_OPEN_MIN     4       /* 已关到最小 */
#define EVI_STATUS_OPEN_FIX     5       /* 定开度 */
static U08 log_mod_get_evi_status()
{
    U08 result = EVI_STATUS_NORMAL;
    I16 evi_step = GET_EXIO_ST_PV(0, EEV_ID_2);

    if (evi_step == 0
    ||  val_s_motor_use==EVI_SMALL_EEV)                                  
        result = EVI_STATUS_CANNOT_ON;                  /* 不可开 */               
    else if (evi_step >= EVI_RUN_STEP_MAX)                         
        result = EVI_STATUS_OPEN_MAX;                   /* 已开到最大 */              
    else if (evi_step <= EVI_RUN_STEP_MIN)                          
        result = EVI_STATUS_OPEN_MIN;                   /* 已关到最小 */         
    else if (val_s_motor_manual_mode == USED)               
        result = EVI_STATUS_OPEN_FIX;                   /* 定开度 */ 

    return result;
}

/*******************************************************************************
 函 数: log_mod_low_env_hig_out_judge()
 功 能: 低环温高水温工况判定
 参 数: 空
 返 回: true：低环温高水温工况
 其 它: 1、未显式判断运行模式
*******************************************************************************/
BOL log_mod_low_env_high_water_judge(void)
{
    #define TEMP_DIFF 50
    I16 temp_wat = pv_cell_TWin1(pcb_addr);
    I16 temp_env = pv_cell_TA(pcb_addr);
    
    static BOL fg_lehw = FALSE;             /* 低环温高水温标志 */

    /* 低环温高水温处理动作只在辅路排气过热度及主路吸气过热度控制函数中有处理 */
    if (val_s_motor_use!=EVI_UNUSED 
     && val_s_motor_use!=EVI_SMALL_EEV
     && val_motor_use!=EEV_UNUSED
     && val_press_diff_in!=0                /* 设为0表示禁用该功能 */
     )
    {
        if (temp_wat > val_press_diff_in
         && temp_env < val_press_diff_env
        )
        {
            fg_lehw = TRUE;
        }
        else if (temp_wat < val_press_diff_in-TEMP_DIFF
              || temp_env > val_press_diff_env+TEMP_DIFF
        )
        {
            fg_lehw = FALSE;
        }
    }
    else
    {
        fg_lehw = FALSE;
    }

    return fg_lehw;
}

/*********************************************
函 数: Logic_eev_control
功 能: 主电子膨胀阀控制（过热度控制）
参 数: EEV_ID_ENUM id:电膨序号           U08 x:压机序号
返 回: 无
**********************************************/
#define FORBID_SUB_MOTOR_PRESS_DIFF           10  // 压力回差
#define FORBID_SUB_MOTOR_GAS_DIFF             20  // 排气回差
static void Logic_eev_control(EEV_ID_ENUM id, U08 x)
{
    U16 step_open_max ;
    U16 step_open_min ;
    U16 step_idle, step_defrost;
    U16 valve_degree_preopen, valve_step_preopen;
    PID_ID_ENUM PID_id;

    U08 control_type = 0;
    I16 pid_pv = 0;
    I16 pid_sv = 0;

    I16 cond_side_temp;     /* 冷凝侧温度 */
    I16 evap_side_temp;     /* 蒸发侧温度 */
    I16 step_eev = GET_EXIO_ST_PV(0, EEV_ID_1);
    I16 step_evi = GET_EXIO_ST_PV(0, EEV_ID_2);
    I16 force_step = MAX(1, (I32)step_eev*val_motor_force_step/1000);

    I16 gas_temp = pv_gas_out_high(x);
    I16 temp_env = pv_cell_TA(pcb_addr);
    I16 temp_in = pv_cell_TWin1(pcb_addr);
    U16 freq_now = invt_info_freq_now(INVT_ID_1);
    
    I16 alarm_lp = (Cm[x].run_mode==MODE_COOL ? val_lp_value_incl : val_lp_value_inht);
    I16 alarm_hp = (Cm[x].run_mode==MODE_COOL ? val_hp_value_incl : val_hp_value_inht);   

    I16 limit_lp = alarm_lp + val_lp_forbid_add_prs_diff;	/* 进关小限制低压压力 */
    I16 limit_hp = alarm_hp + val_hp_forbid_add_prs_diff;   /* 进关小限制高压压力 */

    U08 gas_out_trend = fun_value_trend(gas_temp, mod_rec_gas_out_avg[x], 1);

    static I16 step_before_defr[MAX_COMP];  /* 除霜前主阀步数 */
    static I16 step_before_reoil[MAX_COMP]; /* 回油前主阀步数 */


    // 确定用于计算初始开度的冷凝侧及蒸发侧温度
    if (Cm[x].run_mode == MODE_COOL)          // 制冷
    {
        evap_side_temp = temp_in;      // 蒸发侧温度为空调水温
        cond_side_temp = temp_env;    // 冷凝侧温度为环境温度
        step_open_min = (U32)Logic_eev_step_min(x) * val_motor_step_max / 100;  // 最小运行步数
        step_open_max = (U32)val_motor_max_od * val_motor_step_max / 100;       // 最大运行步数
    }
    else // 制热
    {
        cond_side_temp = temp_in;      // 冷凝侧温度为空调水温
        evap_side_temp = temp_env;    // 蒸发侧温度为环境温度
        step_open_min = (U32)Logic_eev_step_min(x) * val_motor_step_max / 100;  // 最小运行步数
        step_open_max = (U32)val_motor_max_od * val_motor_step_max / 100;       // 最大运行步数
    }    


    // 电膨参数设置
    step_defrost  = (U32)val_m_defrost_od * val_motor_step_max / 100;       // 除霜开度
    step_defrost = AT_VAL(step_defrost, val_ability_test_step);
    step_idle = (U32)val_motor_idle_open * val_motor_step_max / 100; /* 待机步数 */
    eev_set_steps_max(id, val_motor_step_max);                  // 总步数
    eev_set_steps_run_max(id, step_open_max);                   // 最大运行步数
    eev_set_steps_min(id, step_open_min);                       // 最小运行步数
    eev_set_steps_idle(id, step_idle);                          // 待机步数
    eev_set_steps_defrost(id, step_defrost);                    // 除霜步数
    eev_set_period_calculate(id, val_m_pid_calculate_period);   // 计算周期(100ms)
    eev_set_period_action(id, val_m_pid_period*10);             // 动作周期(100ms) 
    
    eev_set_period_add(id, val_m_pid_period*10);                // 强制开阀动作周期(100ms) 
    eev_set_steps_add(id, force_step);              
    eev_set_period_sub(id, val_m_pid_period*10);                // 强制关阀动作周期(100ms) 
    eev_set_steps_sub(id, force_step);              
    
    eev_set_period_hold(id, val_hold_evi_time*60);             // 动作周期(1s) 
    eev_set_period_cyc(id, val_hold_evi_cyc*60);             // 动作周期(1s) 

    if (SensorCanUse(step_before_defr[x])
    &&  val_motor_defr_init_cpst!=0
        )
    {
        valve_step_preopen   = fun_pv_limit(step_before_defr[x]+val_motor_defr_init_cpst, step_open_min, step_open_max);
    }
    else if (cnt_reoil_end[x] >= CNT_AFTER_REOIL
         &&  SensorCanUse(step_before_reoil[x])
         &&  step_before_reoil[x] != 0
        )
    {
        valve_step_preopen   = step_before_reoil[x];
    }
    else
    {
        valve_degree_preopen =  Logic_Init_Scale(Cm[x].run_mode, cond_side_temp, evap_side_temp);  // 初始开度 
        valve_step_preopen   = eev_degree_to_steps(id, valve_degree_preopen);
    }
        
    eev_set_steps_preopen(id, valve_step_preopen);              // 预开步数(初始开度)
    eev_set_period_preopen(id, val_m_motor_start_time*10);      // 预开时间(100ms)
    
    // 确定电膨切换命令
    if (!s_get_cm_status(x, CM_NEED)
    &&  !invt_info_is_running(INVT_ID_1)        /* 滤掉降频停机过程 */
        ) 
    {
        control_type = EEV_CMD_RESET;           // 阀复位到待机开度     
        
        step_before_defr[x] = SENSOR_NOEXIST;
        step_before_reoil[x] = SENSOR_NOEXIST;
    }
    else
    {
        // 禁止关阀
        {
            /* 低压 */
            fg_forbid_motor_sub[id][FORBID_SUB_MOTOR_LP]  = Condition_judge(fg_forbid_motor_sub[id][FORBID_SUB_MOTOR_LP], 
                                                                            pv_cell_lp(x), 
                                                                            limit_lp, 
                                                                            val_lp_pre_adju_diff, 
                                                                            DRCT_UNDER);
            /* 高压 */
            fg_forbid_motor_sub[id][FORBID_SUB_MOTOR_HP]  = Condition_judge(fg_forbid_motor_sub[id][FORBID_SUB_MOTOR_HP], 
                                                                            pv_cell_hp(x), 
                                                                            limit_hp, 
                                                                            -val_hp_pre_adju_diff, 
                                                                            DRCT_ABOVE);
            /* 排温 */
            fg_forbid_motor_sub[id][FORBID_SUB_MOTOR_EXH] = Condition_judge(fg_forbid_motor_sub[id][FORBID_SUB_MOTOR_EXH], 
                                                                            gas_temp, 
                                                                            val_force_ctrl_gas_out-GAS_OUT_HIGH_DIFF2, 
                                                                            50, DRCT_ABOVE);

            /* 蒸发侧 */
            if(Cm[x].run_mode==MODE_COOL)   
            {
                fg_forbid_motor_sub[id][FORBID_SUB_MOTOR_WING] = FALSE;
            }
            else
            {
                fg_forbid_motor_sub[id][FORBID_SUB_MOTOR_WING] = Condition_judge(fg_forbid_motor_sub[id][FORBID_SUB_MOTOR_WING], pv_cell_wing(x), val_motor_limit_down_wing, 10, DRCT_UNDER);
            }
        }
        
        // 禁止开阀
        {
            /* 蒸发侧 */
            if(Cm[x].run_mode==MODE_COOL)   //
            {
                fg_forbid_motor_add[id][FORBID_ADD_MOTOR_EVAP] = Condition_judge(fg_forbid_motor_add[id][FORBID_ADD_MOTOR_EVAP], pv_evap_temp(x), val_max_evap_temp-20, 20, DRCT_ABOVE);
            }
            else
            {
                fg_forbid_motor_add[id][FORBID_ADD_MOTOR_EVAP] = Condition_judge(fg_forbid_motor_add[id][FORBID_ADD_MOTOR_EVAP], pv_cell_wing(x), val_max_evap_temp-20, 20, DRCT_ABOVE);
            }

#if defined(M_CHANGFAN)           /* 广东长帆专用 */
            /* 排气过热度 */
            fg_forbid_motor_add[id][FORBID_ADD_MOTOR_OVEREXH] = Condition_judge(fg_forbid_motor_add[id][FORBID_ADD_MOTOR_OVEREXH],
                                                                           log_mod_get_exh_oh_pv(x),
                                                                           170,
                                                                           10,
                                                                           DRCT_UNDER);
#endif
        }

        // 强制开阀
        {
            U08 evi_sta = log_mod_get_evi_status();	
            /* 排温 */			 
            if ((evi_sta != EVI_STATUS_NORMAL && val_high_gas_eev_evi_order == EVI_PRIOR)
             || val_high_gas_eev_evi_order == EEV_EVI_SAME
             || val_high_gas_eev_evi_order == EEV_PRIOR)
            {
                fg_force_motor[id][FORCE_ADD_MOTOR_EXH] = Condition_judge(fg_force_motor[id][FORCE_ADD_MOTOR_EXH], 
                                                                          gas_temp, 
                                                                          val_force_ctrl_gas_out-GAS_OUT_HIGH_DIFF1, 
                                                                          50, DRCT_ABOVE);
            }
            else
            {
                fg_force_motor[id][FORCE_ADD_MOTOR_EXH] = FALSE;
            }
        }

        // 强制关阀
        {
            I16 allow_lp = limit_lp + val_lp_pre_adju_diff;   /* 退关小限制低压压力 */
            I16 allow_hp = limit_hp + val_hp_pre_adju_diff;   /* 退关小限制高压压力 */
            /* 蒸发侧 */
            if(Cm[x].run_mode==MODE_COOL)
            {
                if (SensorCanUse(pv_cell_hp(x)) && pv_cell_hp(x)<allow_hp)
                {
                    /* 蒸发温度 */
                    fg_force_motor[id][FORCE_SUB_MOTOR_EVAP] = Condition_2judge(fg_force_motor[id][FORCE_SUB_MOTOR_EVAP], pv_evap_temp(x), val_max_evap_temp, 10,DRCT_ABOVE,CDT_FORCE_SUB_MOTOR_EVAP);
#if defined(M_CHANGFAN)           /* 广东长帆专用 */
                    /* 排气过热度 */
                    fg_force_motor[id][FORCE_SUB_MOTOR_OVEREXH] = Condition_2judge(fg_force_motor[id][FORCE_SUB_MOTOR_OVEREXH], log_mod_get_exh_oh_pv(x), 150, 10, DRCT_UNDER, CDT_FORCE_SUB_MOTOR_EXH_OH);
#endif
                }
                else
                {
                    fg_force_motor[id][FORCE_SUB_MOTOR_EVAP] = FALSE;
                    fg_force_motor[id][FORCE_SUB_MOTOR_OVEREXH] = FALSE;
                }
            }
            else
            {
                if (SensorCanUse(pv_cell_lp(x)) && pv_cell_lp(x)>allow_lp)
                {
                    /* 蒸发温度 */
                    fg_force_motor[id][FORCE_SUB_MOTOR_EVAP] = Condition_2judge(fg_force_motor[id][FORCE_SUB_MOTOR_EVAP], pv_cell_wing(x), val_max_evap_temp, 10,DRCT_ABOVE,CDT_FORCE_SUB_MOTOR_EVAP);
#if defined(M_CHANGFAN)           /* 广东长帆专用 */
                    /* 排气过热度 */
                    fg_force_motor[id][FORCE_SUB_MOTOR_OVEREXH] = Condition_2judge(fg_force_motor[id][FORCE_SUB_MOTOR_OVEREXH], log_mod_get_exh_oh_pv(x), 150, 10, DRCT_UNDER, CDT_FORCE_SUB_MOTOR_EXH_OH);
#endif
                }
                else
                {
                    fg_force_motor[id][FORCE_SUB_MOTOR_EVAP] = FALSE;
                    fg_force_motor[id][FORCE_SUB_MOTOR_OVEREXH] = FALSE;
                }
            }
            if(cm_time[x].on <= 400)
            {
                fg_force_motor[id][FORCE_SUB_MOTOR_EVAP] = FALSE;
                fg_force_motor[id][FORCE_SUB_MOTOR_OVEREXH] = FALSE;
            }
        }

        if (fg_force_motor[id][FORCE_ADD_MOTOR_EXH]
            && TREND_IS_RISE(gas_out_trend))
        {
            control_type =  EEV_CMD_FORCE_UP;
        }
        else if ((fg_forbid_motor_sub[id][FORBID_SUB_MOTOR_EXH]&&step_evi==0)
             ||  fg_forbid_motor_sub[id][FORBID_SUB_MOTOR_LP]
             ||  fg_forbid_motor_sub[id][FORBID_SUB_MOTOR_HP]
             ||  fg_forbid_motor_sub[id][FORBID_SUB_MOTOR_WING])  
        {
            control_type = EEV_CMD_LIMIT_DOWN;  // 限制关小
        }
        else if (fg_force_motor[id][FORCE_SUB_MOTOR_EVAP]
             ||  fg_force_motor[id][FORCE_SUB_MOTOR_OVEREXH])
        {
            control_type = EEV_CMD_FORCE_DOWN;  // 强制关小
        }
        else if (fg_forbid_motor_add[id][FORBID_ADD_MOTOR_EVAP]
             ||  fg_forbid_motor_add[id][FORBID_ADD_MOTOR_OVEREXH]
        )
        {
            control_type = EEV_CMD_LIMIT_UP;    // 限制开大
        }
        else
        {
            control_type = EEV_CMD_RUN;         // 正常调节
        }

        // 除霜时主回路电子膨胀阀开启到除霜开度
        {
            static BOL fg_before_defr[MAX_COMP];

            if (Cm[x].defrost_stage==DS_DEFROST
            ||  Cm[x].defrost_stage==DS_QUIT && Cm[x].defrost_dly<=DEFROST_DLY_2)
            {
                control_type = EEV_CMD_DEFRO;

                if (!fg_before_defr[x])
                {
                    fg_before_defr[x] = TRUE;
                    step_before_defr[x] = step_eev;
                }
            }
            else
            {
                fg_before_defr[x] = FALSE;
            }
        }

        /* 回油 */
        if (0)              /* 方案待验证，暂不使用 */
        {
            static BOL fg_before_reoil[MAX_COMP];

            if (log_cmp_is_reoil(x))
            {
                if (!fg_before_reoil[x])
                {
                    fg_before_reoil[x] = TRUE;
                    step_before_reoil[x] = step_eev;
                }
            }
            else
            {
                fg_before_reoil[x] = FALSE;

                if (step_before_reoil[x] == step_eev
                &&  cnt_reoil_end[x] >= val_m_motor_start_time
                    )
                {
                    step_before_reoil[x] = SENSOR_NOEXIST;
                    eev_set_period_preopen(id, 0);      // 预开时间(100ms)
                }
            }

            if (cnt_reoil_end[x] <= CNT_AFTER_REOIL)
            {
                control_type = EEV_CMD_PREV;
            }
        }
    }

    /* 停机归零处理 */
    {
        if (Cm[x].stage == CM_IDLE
        && cm_time[x].off > 30+CM_off_speed_down_time // 压机停机30秒钟后膨胀阀归零初始化(停机后只初始化一次)
        && FALSE == fg_vavle_reset[id]
        )
        {
            fg_vavle_reset[id] = TRUE;       
            control_type = EEV_CMD_STOP;
        }
        else if (s_get_cm_status(x, CM_ON))     // 压机开启，复位标志
        {
            fg_vavle_reset[id] = FALSE;
            cm_time[x].off = 0;                 // 立即清零。否则下个循环fg_vavle_reset[id]又会置位，因为停机时间要秒标志到后才能根据压机状态清零。
        }
    }

    /* 过热度目标值、当前值 */
    {
        switch (val_motor_use)
        {
            default:
            case EEV_OVERHEAT_SUC:
                SETBIT(eev_adjust_tmp[x][EEV_ADJ_TMP_USAGE], EEV_USAGE_SUC);
                
                /* 过热度目标值确认 */
                if (Cm[x].run_mode == MODE_COOL)       /* 制冷 */
                {
                    pid_sv = Logic_eev_sv_cool(evap_side_temp);
                }
                else 
                {
                    pid_sv = Logic_eev_sv_heat(evap_side_temp);
                }

                eev_adjust_tmp[x][EEV_ADJ_TMP_BASE] = pid_sv;

                /* 过热度目标值频率补偿 */
                if (!USE_FIX_COMP)
                {
                         if (freq_now <= 40*100) pid_sv += val_motor_sv_freq_cpst*2;    
                    else if (freq_now <= 50*100) pid_sv += val_motor_sv_freq_cpst;

                    eev_adjust_tmp[x][EEV_ADJ_TMP_COMP] = pid_sv;
                }

                /* 过热度目标值排温补偿 */
                if (SensorCanUse(gas_temp))
                {
                         if (gas_temp >= 95*10) pid_sv += val_motor_sv_exh_cpst*2;
                    else if (gas_temp >= 75*10) pid_sv += val_motor_sv_exh_cpst;

                    eev_adjust_tmp[x][EEV_ADJ_TMP_EXH] = pid_sv;
                }

                pid_sv = calculate_gb_dbg_sv(Cm[x].run_mode, pid_sv);// 国标测试用
                eev_adjust_tmp[x][EEV_ADJ_TMP_GB] = pid_sv;

                /* 过热度当前值确认 */
                if (SensorCanUse(pv_cell_evap_temp(x)))
                {
                    pid_pv = fun_pv_sub(pv_cell_gas_in(x), pv_cell_evap_temp(x));   /* 压力过热度 */
                }
                else
                {
                    if (Cm[x].run_mode == MODE_COOL)
                    {
                    	/* 当阀后温度可用时，使用阀后温度计算，当阀后温度不可用时，采用冷媒液侧温度替代阀后温度 */
                    	if(SensorCanUse(pv_cell_evap(x)) )
                    	{
                    		pid_pv = fun_pv_sub(pv_cell_gas_in(x), pv_cell_evap(x));
                    	}
                        else
			{
                    		pid_pv = fun_pv_sub(pv_cell_gas_in(x), pv_cell_TRFliq(x));
			}			
                    }
                    else
                    {
                        pid_pv = fun_pv_sub(pv_cell_gas_in(x), pv_cell_wing(x));
                    }
                }
                break;
                
            case EEV_OVERHEAT_EXH:
                SETBIT(eev_adjust_tmp[x][EEV_ADJ_TMP_USAGE], EEV_USAGE_EXH);
                
                /* 过热度目标值确认 */
                pid_sv = log_mod_get_exh_oh_sv(x);
                eev_adjust_tmp[x][EEV_ADJ_TMP_BASE] = pid_sv;

                /* 过热度目标值频率补偿 */
                if (!USE_FIX_COMP && val_motor_sv_freq_cpst!=0)
                {
                    /* 提示：这里是为了得到工况频率上限，而不是为了限频，加级联整合频率上限时注意不用处理这里 */
                    I16 invt_max = wf_get_freq_max_by_env_in(Cm[x].run_mode, temp_env, temp_in);
                    I16 tmp = MIN((I32)CmRotation[x]*val_motor_sv_freq_cpst/invt_max, 10);	/* 最大值限制为1.0 */

                    pid_sv = _DIVUROUND(pid_sv*tmp, 10);

                    eev_adjust_tmp[x][EEV_ADJ_TMP_COMP] = pid_sv;
                }

                pid_sv = log_mod_exh_oh_sv_limit(x, pid_sv);
                eev_adjust_tmp[x][EEV_ADJ_TMP_RANGE] = pid_sv;

                /* 过热度当前值确认 */
                pid_pv = log_mod_get_exh_oh_pv(x);
                
                break;
        }

        /* 能力测试手动目标过热度时 */
        if (USE_ABILITY_EEV_SSH_SV) 
        {
            pid_sv = val_ability_test_eev_ssh_sv;
            SETBIT(eev_adjust_tmp[x][EEV_ADJ_TMP_USAGE], EEV_USAGE_AT);
        }

#if 0        
        if (val_motor_use==EEV_OVERHEAT_PRS && SensorCanUse(pv_cell_evap_temp(x)))
        {
            pid_pv = pv_cell_gas_in(x) - pv_cell_evap_temp(x);
        }
        else if (val_motor_use==EEV_OVERHEAT_TEMP_H)
        {
            pid_pv = pv_cell_gas_in(x) - pv_cell_evap(x);
        }
        else
        {
            if (Cm[x].run_mode == MODE_COOL)
            {
                pid_pv = pv_cell_gas_in(x) - pv_cell_evap(x);
            }
            else
            {
                pid_pv = pv_cell_gas_in(x) - pv_cell_wing(x);
            }
        }
#endif
    }

    /* 正常调阀时间处理 */
    if (control_type < EEV_CMD_PREV)
    {
        motor_run_time[id] = 0;
    }
    
    /* 主阀初始开度：若满足低环温高水温启动，则先以固定步数启动一段时间，再转为初始开度回到正常控制 */
    if (fg_low_env_high_water
     && S_CM_GetStatus(x, CM_NEED)
     && motor_run_time[EEV_ID_1] < LEHW_EEV_START_TIME
     && control_type!=EEV_CMD_DEFRO
     )
    {
        eev_set_steps_preopen(id, val_press_diff_eev);
        control_type = EEV_CMD_PREV;
    }

    /* 低环温高水温工况时，初开维持时间使用特定值 */
    if (Cm[x].run_mode != MODE_COOL
    &&  fg_low_env_high_water
        )
    {
        eev_set_period_preopen(id, LEHW_EEV_INIT_TIME*10);      // 预开时间(100ms)
    }

    /* 收氟处理 */
    if (fg_refri_recy_act)
    {
        eev_set_steps_preopen(id, val_recycle_eev_step);
        control_type = EEV_CMD_PREV;
    }

    // 预设PID参数
    if(x==0)    // 1#压机
    {
        PID_id = PID_ID_EEV_1;      // 1#电膨对应1#PID
        pid_set_ctrl(PID_id, PID_CTRL_INCREMENT);       //增量式
        pid_set_Ts(PID_id, val_m_pid_calculate_period); //计算周期
        pid_set_Kp(PID_id, val_m_pid_Kp);//比例系数
        pid_set_Ti(PID_id, val_m_pid_i);//积分时间
        pid_set_Td(PID_id, val_m_pid_d);//微分时间
        // 反作用比例带
        if (pid_get_AveAcrossCnt(PID_id) > AVG_ACROSS_NUM2
        &&  pv_cell_step_m(x)<val_eev_step_refer
        )                          
        {
            /* 针对超低温制热或低频运行时主阀震荡不稳问题的优化 */
            U32 kc = MIN((U32)val_m_pid_p*val_eev_refer_coef, 30000);
            pid_set_Kc(PID_id, kc);
        }
        else
        {
            pid_set_Kc(PID_id, val_m_pid_p);
        }
        pid_set_Ai(PID_id, val_m_pid_flexi);//积分系数

        eev_task_pid(PID_id,id, (EEV_CMD_ENUM)control_type, pid_pv, pid_sv);     // 膨胀阀调节处理
    }        

     pv_cell_eev_pv(x) = m_motor_pv_sheat = gdebugIO.in_superheatpv = pid_pv;
     pv_cell_eev_sv(x) = m_motor_sv_sheat = gdebugIO.in_superheatsv = pid_sv;
}

/*******************************************************************************
 函 数: log_mod_eev_freq()
 功 能: 变频调阀
 参 数: en--调阀指令; 
        step_pv--主阀当前步数; 
 返 回: 主阀调整步数
 其 它: 空
*******************************************************************************/
I16 log_mod_eev_freq(U08 en, U16 step_pv)
{
    static I16 freq_old;
    I16 inc_od = 0;
    I16 inc_freq = 0;

    U16 freq_now = _DIVUROUND(invt_info_freq_now(INVT_ID_1), 100);       /* 转化为1rps */

    if (USE_FIX_COMP)
    {
        return 0;
    }

    /* 1分钟频率变化快则补偿调阀 */
    if (cnt_invt_chg_speed > 60)
    {
        if (Fun_AbsI32(fun_pv_sub(freq_now, freq_old)) >= val_eev_invt_rps_diff
        &&  freq_old!=0     /* 过滤首次 */
        &&  cnt_invt_chg_speed < 120		/* 大于120说明很久没执行该函数，如停机等，需过滤 */
        )
        {
            inc_freq = fun_pv_sub(freq_now, freq_old);
        }

        freq_old = freq_now;
        cnt_invt_chg_speed = 0;
    }

    if (inc_freq > 0    // 升频调阀
    && (en==EEV_CMD_RUN || en==EEV_CMD_LIMIT_DOWN)
    )   
    {        
        inc_od = (I32)freq_now*inc_freq*step_pv*val_eev_invt_coef_up/100000;
    }
    else if (inc_freq < 0    // 降频调阀
    && (en==EEV_CMD_RUN || en==EEV_CMD_LIMIT_UP)
    )
    {        
        inc_od = (I32)freq_now*inc_freq*step_pv*val_eev_invt_coef_dn/100000;
    }

    return inc_od;
}

/*******************************************************************************
 函 数: log_mod_work_condition_chg_deal()
 功 能: 工况变化处理
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_mod_work_condition_chg_deal(void)
{
    enum {WCD_ENV, WCD_IN, WCD_FREQ, WCD_EEV_SV, WCD_MAX};
    static U16 wcd_cnt[WCD_MAX];
    
    BOL fg_chg = FALSE;
    PID_ID_ENUM id = PID_ID_EEV_1;
    U08 cm = 0;
    
    static I16 temp_env = SENSOR_NOEXIST;
    static I16 eev_sv = SENSOR_NOEXIST;
    static I16 freq = SENSOR_NOEXIST;

    fg_chg |= fun_pv_filt_t(&temp_env,  pv_cell_TA(pcb_addr),  30,  &wcd_cnt[WCD_ENV],     1800);   /* 3℃ */
    fg_chg |= fun_pv_filt_t(&eev_sv,    pv_cell_eev_sv(cm),    10,  &wcd_cnt[WCD_EEV_SV],  1800);   /* 10步 */
    fg_chg |= fun_pv_filt_t(&freq,      pv_cell_invt_set(cm),  100, &wcd_cnt[WCD_FREQ],    1800);   /* 10转 */

    /* 工况变化，清空主阀穿过次数 */
    if (fg_chg)
    {
        if (pid_get_AveAcrossCnt(id) > AVG_ACROSS_NUM)  pid_set_AveAcrossCnt(id, 0);    
    }

    /* 计时处理 */
    if (F_TIME_BASIC_SEC_BACK)
    {
        U08 i;
        for (i=0; i<WCD_MAX; i++)
        {
            _CNT(wcd_cnt[i]);
        }
    }
}

/*******************************************************************************
 函 数: log_mod_eev()
 功 能: 电子膨胀阀函数：包括主阀与辅阀
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_mod_eev(void)
{
    U08 i;

    /* 工况变化处理 */
    log_mod_work_condition_chg_deal();

    /* 低环温高水温标志 */
    fg_low_env_high_water = log_mod_low_env_high_water_judge();

    for (i=0; i<self_comp_num; i++) 
    {
        fg_comp_valve_reset[i]=VALVE_READY;
        if( i==0 )
        {
            // 确认电膨准备状态
            if (TF_IS_IDLE)	/* 商检时不等待辅阀 */
            {
                if (val_s_motor_use!=EVI_UNUSED  
                   && ((Cm[i].run_mode==MODE_HEAT && val_s_motor_use_heat!=UNUSED)
                       ||(Cm[i].run_mode==MODE_COOL && val_s_motor_use_cool!=UNUSED))
                   && !eev_isReady(EEV_ID_2))
                {
                    fg_comp_valve_reset[i]=VALVE_BUSY;  // 经济器电膨使用 且 未准备好
                }
            }

            if(val_motor_use!=EEV_UNUSED && !eev_isReady(EEV_ID_1))
            {
                fg_comp_valve_reset[i]=VALVE_BUSY;  // 主回路电膨使用 且 未准备好
            }
        }

        if (val_motor_use != EEV_UNUSED)            // 主路电膨控制
        {
            Logic_eev_control(EEV_ID_1, i);       
        }

        if (val_s_motor_use != EVI_UNUSED)          // 辅路电膨控制
        {
            Logic_evi_control(EEV_ID_2, i);
        }
    }
}

/*******************************************************************************
 函 数: log_mod_elec()
 功 能: 模块功率统计
 参 数: 空
 返 回: 空
 其 它: 单位：1w
*******************************************************************************/
void log_mod_elec(void)
{
//    U08 cm;
//    static LOG_RTC time[MAX_COMP];

    I16 power_comp = 0;
    I16 power_fan = 0;
    I16 power_pump = 0;
    I16 power_IBH1 = 0;
    I16 power_IBH2 = 0;
    I16 power_idle = 0;
    I16 power_other_run = 0;

    I16 speed_max_fan = MAX(val_freq_fan_cool_max, val_freq_fan_heat_max);
    I16 speed_max_pump = PUMPf_rpm_max;

    if (!USE_ELEC_METERING)
    {
        pv_cell_power(pcb_addr) = SENSOR_NOEXIST;
        return;
    }

    /* 压机功率 */
#ifndef M_DEBUG
    if (SensorCanUse(pv_cell_invt_power_out(INVT_ID_1)))
    {
        power_comp = pv_cell_invt_power_out(INVT_ID_1)*10;
    }
#else
    if (SensorCanUse(pv_cell_invt_volt(INVT_ID_1)))
    {
        power_comp = pv_cell_invt_volt(INVT_ID_1);				/* 调试时用10A作为输出电流计算输出功率 */
    }
#endif    

    /* 风机功率 */
    {
#ifndef M_DEBUG    
        I32 fan_now1 = pv_cell_fan_now(FREQ_FAN1);
        I32 fan_now2 = pv_cell_fan_now(FREQ_FAN2);
#else    
        I32 fan_now1 = pv_cell_fan_set(FREQ_FAN1);
        I32 fan_now2 = pv_cell_fan_set(FREQ_FAN2);
#endif    
        if (SensorCanUse(fan_now1))
        {
            power_fan += (I16)(pow((float)fan_now1/speed_max_fan, 3)*val_power_fan);
        }
        if (SensorCanUse(fan_now2))
        {
            power_fan += (I16)(pow((float)fan_now2/speed_max_fan, 3)*val_power_fan);
        }
    }

    /* 水泵功率 */
    {
        I32 pump_now = pv_cell_PUMPf_set(pcb_addr);
        
        if (SensorCanUse(pump_now)
        &&  speed_max_pump>0
        )
        {
            power_pump = (I16)(pow((float)pump_now/speed_max_pump, 3)*val_power_pump);
        }
    }

    /* IBH功率 */
    if (s_get_output(MAX_COMP, S_DO_IBH1))
    {
        power_IBH1 = val_power_IBH1;
    }
    if (s_get_output(MAX_COMP, S_DO_IBH2))
    {
        power_IBH2 = val_power_IBH2;
    }

    /* 待机功率 */
    if (MAC_IS_IDLE)
    {
        power_idle = val_power_idle;
    }

    /* 其他运行功率 */
    if (MAC_IS_RUN)
    {
        power_other_run = val_power_other_run;
    }


    /* 模块功率 */
    pv_cell_power(pcb_addr) = power_comp+power_fan+power_pump+power_IBH1+power_IBH2+power_idle+power_other_run;

    gdebugIO.debugbuf[80] = power_comp;
    gdebugIO.debugbuf[81] = power_fan;
    gdebugIO.debugbuf[82] = power_pump;
    gdebugIO.debugbuf[83] = power_IBH1;
    gdebugIO.debugbuf[84] = power_IBH2;
    gdebugIO.debugbuf[111] = power_idle;
    gdebugIO.debugbuf[112] = power_other_run;
}


/* PUMPf开启条件 */
enum
{
    PUMPF_ON_SIG_CELL_RUN                = 0,   /* 模块运行时开启 */
    PUMPF_ON_SIG_IDLE_STA_ANTIFREEZE     = 1,   /* 待机态防冻时开启 */
    PUMPF_ON_SIG_RUN_STA_AC_ANTIFREEZE   = 2,   /* 运行态空调水路防冻时开启 */
    PUMPF_ON_SIG_RUN_STA_HOTW_ANTIFREEZE = 3,   /* 运行态热水水路防冻时开启 */
    PUMPF_ON_SIG_HOTW_TIMED_ANTIFREEZE   = 4,   /* 热水定时防冻时开启 */
    PUMPF_ON_SIG_DISINFECT               = 5,   /* 杀菌时处理 */
    PUMPF_ON_SIG_LONGTIME_OFF            = 6,   /* 长时间未运行时开启 */
    PUMPF_ON_SIG_TEST                    = 7,   /* 试运行功能开启（空气排空试运行或水泵试运行） */
    PUMPF_ON_SIG_DEBUG                   = 8,   /* 调试运行开启 */
    PUMPF_ON_SIG_MAX,
};
static U16 fg_PUMPf_on[_DIVUCARRY(PUMPF_ON_SIG_MAX, 16)];

#define  LONGTIME_NO_RUN_ON     (6*60*QK_60)    // 长时间未运行开启延时
#define  LONGTIME_NO_RUN_OFF    (3*QK_60)       // 长时间未运行开 持续时间
#define  PUMPf_OFF_DLY          (TF_IS_IDLE?((fg_air_empty_test_run||fg_pump_test_run)?0:(60+CM_off_speed_down_time)):30)     //关空调泵延时  // 原来30 
/*******************************************************************************
 函 数: log_mod_PUMPf_interval_on_need()
 功 能: 变频水泵间歇开启需求计算
 参 数: 空
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL log_mod_PUMPf_interval_on_need(void)
{
    U08 sub_sys_type;
    BOL fg_interval_on;

    sub_sys_type = GET_SUB_SYS(pcb_addr);
    fg_interval_on = (cnt_PUMPf_interval_on_off < PUMPF_INTERVAL_ON_TIME) ? TRUE : FALSE;

    /* 强制一直运行的情况 */
    if (MODE_IS_COOL(sub_sys_type)          /* 制冷模式 */
    || fg_energy_need[sub_sys_type]         /* 子系统有能量需求 */
    || log_mod_comp_is_run(MODE_NULL)       /* 模块压机运行 */
    )
    {
        fg_interval_on = TRUE;
        cnt_PUMPf_interval_on_off = 0;
    }

    return fg_interval_on;
}
 
/*******************************************************************************
 函 数: log_mod_PUMPf_need()
 功 能: 模块变频水泵需求计算
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_mod_PUMPf_need(void)
{
    /* 模块运行时 */
    if (fg_cell_pump_open)
    {
        if (val_pump_mode == PUMP_MODE_INTER)           /* 间歇运行 */
        {
            if (log_mod_PUMPf_interval_on_need())
            {
                SETBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_CELL_RUN);
            }
            else
            {
                CLRBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_CELL_RUN);
            }
        }
        else                /* 一直运行 */
        {
            SETBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_CELL_RUN);
        }
    }
    else
    {     
        cnt_PUMPf_interval_on_off = 0;      /* 模块未开时，间歇运行计时不生效 */
    
        if (cnt_PUMPf_off_dly > PUMPf_OFF_DLY)
        {
            CLRBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_CELL_RUN);
        }
    }

    /* 待机态防冻时 */
    if (fg_idle_sta_antifreeze)
    {
        SETBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_IDLE_STA_ANTIFREEZE);
    }
    else
    {
        CLRBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_IDLE_STA_ANTIFREEZE);
    }

    /* 运行态空调水路防冻 */
    if ((GET_SUB_SYS(pcb_addr) == SUB_SYS_AC_ONLY) && fg_run_sta_ac_antifreeze)
    {
        SETBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_RUN_STA_AC_ANTIFREEZE);
    }
    else
    {
        CLRBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_RUN_STA_AC_ANTIFREEZE);
    }

    /* 运行态热水水路防冻 */
    if ((GET_SUB_SYS(pcb_addr) == SUB_SYS_AC_HOTW) && fg_run_sta_hotw_antifreeze)
    {
        SETBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_RUN_STA_HOTW_ANTIFREEZE);
    }
    else
    {
        CLRBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_RUN_STA_HOTW_ANTIFREEZE);
    }

    /* 热水定时防冻 */
    if ((GET_SUB_SYS(pcb_addr) == SUB_SYS_AC_HOTW) && fg_hotw_timed_antifreeze_S3V1_OFF_need)
    {
        SETBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_HOTW_TIMED_ANTIFREEZE);
    }
    else
    {
        CLRBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_HOTW_TIMED_ANTIFREEZE);
    }
    
    /* 定时杀菌时 */
    if ((GET_SUB_SYS(pcb_addr) == SUB_SYS_AC_HOTW)
    && fg_timed_disinfect_run
    )
    {        
        if (!fg_timed_disinfect_TBH_need
        && !fg_timed_disinfect_IBH_need
        && !fg_timed_disinfect_AHS_need
        )
        {
            CLRBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_CELL_RUN);	/* 接管正常启动控制 */
            SETBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_DISINFECT);	/* 杀菌阶段1开启变频水泵 */
        }
		else if(fg_timed_disinfect_IBH_need 					/* 阶段一至阶段二标志位窗口期过渡 */
		    && (GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_NORMAL)
            ||  GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL)
        	||	GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_ABNORMAL)
        	||  GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_KEEP_ABNORMAL))
        )
		{
            CLRBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_CELL_RUN);	/* 接管正常启动控制 */
            SETBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_DISINFECT);	/* 杀菌阶段1开启变频水泵 */			
		}		
        else if (fg_timed_disinfect_IBH_need && GETBIT_N(fg_IBH_on, IBH_ON_SIG_DISINFECT)
        || (fg_timed_disinfect_AHS_need && fg_timed_disinfect_AHS_on)
        )
        {
            SETBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_DISINFECT);	/* 杀菌阶段2开启变频水泵 */
        }
        else
        {
            CLRBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_CELL_RUN);	/* 接管正常启动控制 */
            CLRBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_DISINFECT);	/* 杀菌阶段无需开启变频水泵 */
        }
    }
    else
    {
        CLRBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_DISINFECT);
    }

    /* 检测长时间未运行 */
    if (!fg_cell_pump_open)
    {
        if (PUMPf_time.off >= LONGTIME_NO_RUN_ON)				/* 超过6小时未运行，强制运行3分钟 */
        {
            SETBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_LONGTIME_OFF);
        }
        else if (PUMPf_time.on >= LONGTIME_NO_RUN_OFF)
        {
            CLRBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_LONGTIME_OFF);
        }
    }

    /* 试运行功能（空气排空试运行或水泵试运行有开变频水泵需求） */
    if (fg_PUMPf_test_run)
    {
        SETBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_TEST);
    }
    else
    {
        CLRBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_TEST);
    }

    /* 调试运行 */
    if (USE_DEBUG_PUMP)
    {
        SETBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_DEBUG);
    }
    else
    {
        CLRBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_DEBUG);
    }
}

/*******************************************************************************
 函 数: log_mod_PUMPf_out()
 功 能: 模块变频水泵电源输出
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_mod_PUMPf_out(void)
{
    U08 i;

    /* 清PUMPf开启标志 */
    fg_PUMPf_run = FALSE;

    if (USE_PUMPf_CTRL != PUMPF_CTRL_OFF    /* 使用变频水泵 */
    && !s_get_error(S_ERR_PUMPF)            /* 无变频水泵故障 */
    && !fg_water_lack_err                   /* 无水流不足 */
    && !TF_IS_OUT                           /* 非商检外机时 */
    && !fg_test_run_PUMPf_off               /* 非试运行关变频水泵 */
    )
    {
        /* 遍历标志，检查是否需要开启 */
        for (i = 0; i < PUMPF_ON_SIG_MAX; i++)
        {
            if (GETBIT_N(fg_PUMPf_on, i))
            {
                s_set_output(MAX_COMP, S_DO_PUMPf);
                fg_PUMPf_run = TRUE;
            }
        }
    }
}

/*******************************************************************************
 函 数: log_mod_PUMPf()
 功 能: 模块变频水泵控制
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_mod_PUMPf(void)
{
    log_mod_PUMPf_need();	                /* 需求计算 */
    log_mod_PUMPf_out();	                /* 电源输出 */
    PUMPF_speed_ctrl(fg_PUMPf_run);			/* 调速控制 */
}

/*******************************************************************************
 函 数: log_mod_IBH_level()
 功 能: 获取辅助电热档位需求
 参 数: 
 返 回: 辅助电热档位
 其 它: 空
*******************************************************************************/
U08 log_mod_IBH_level(I16 pv, I16 sv, U08 idx)
{
    static U08 area[IBH_ON_SIG_MAX] = {0};
    U08 level = IBH_L0;   

    /* 根据当前温度分区 */
    area[idx] = fun_area(pv, area[idx], 20, 
                    sv, sv-val_IBH_low_dT, sv-val_IBH_high_dT, SENSOR_NOEXIST);

    switch (area[idx])
    {
        default:
        case AREA_00:       
            level = IBH_L0;
            break;
        case AREA_01:       
            level = IBH_L1;
            break;
        case AREA_02:       
            level = IBH_L2;
            break;
        case AREA_03:   
            level = IBH_L3;
            break;
    }

    return level;
}

/*******************************************************************************
 函 数: log_mod_IBH_need()
 功 能: 辅助电热需求
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
U08 log_mod_IBH_need(void)
{
    BOOL is_defrost, fg_comp_normal, fg_comp_abnormal;
    I16 pv_TA, pv_TWout2, pv_THWt, pv_TACt, pv_ctrl_ac;
    I16 sv_TA_IBH_on, sv_TA_IBH_off;
    I16 sv_TWout2_IBH_on, sv_TWout2_IBH_off;
    I16 sv_THWt_IBH_on, sv_THWt_IBH_off;
    U08 level = IBH_L0;
    static BOOL fg_TWout2_too_high = FALSE;
    static BOL fg_heat_force_IBH_off = FALSE;
    static BOL fg_hotw_force_IBH_off = FALSE;
    static BOL fg_heat_force_IBH_once = FALSE;
    static BOL fg_hotw_force_IBH_once = FALSE;

    /* “辅助电热开启延时” */
    U16 IBH_open_dly = val_IBH_open_dly;

    pv_TA = pv_cell_TA(pcb_addr);           /* 环境温度 */
    pv_TWout2 = pv_cell_TWout2(pcb_addr);   /* 总出水温度 */
    pv_TACt    = fun_pv_avg(pv_sys_TACt1, pv_sys_TACt2);    /* 缓冲水箱温度 */
    pv_ctrl_ac = USE_AC_TEMP_IN_CTRL ? pv_TACt : pv_TWout2;	/* 空调控制温度 */
    pv_THWt = pv_sys_THWt;	                /* 热水水箱温度 */

    is_defrost = s_have_comp_defrost();

    if (!STEP_IS_RUN)
    {
        fg_comp_normal = FALSE;
        fg_comp_abnormal = FALSE;
    }
    else
    {
        fg_comp_normal = log_mod_comp_is_run(MODE_NULL) && !TA_FORCE_COMP_OFF; /* 压机正常状态 */
        fg_comp_abnormal = !log_mod_comp_is_run(MODE_NULL) 
                           && (!TA_ALLOW_COMP_ON_OFF 
                               || mod_err_comp[0]               
                               || log_mod_comp_temp_out_limit(0)
                              );    /* 压机异常状态 */
    }

    /* 出水超温保护 */
    if (pv_TWout2 > 700)        fg_TWout2_too_high = TRUE;
    else if (pv_TWout2 < 650)   fg_TWout2_too_high = FALSE;

    /* 辅助电热保持关闭 */
    if ((!IBH_cfg_type)                     /* IBH配置为0 */
    || val_use_IBH==IBH_UNUSED			    /* IBH设置为禁用 */
    || s_get_error(S_ERR_WATER_FLOW)        /* 水流不足故障 */
    || fg_ability_test_run                  /* 能力测试 */
    || fg_floor_preheat_run                 /* 地暖预热 */
    || fg_floor_dryup_run	                /* 地暖烘干 */
    || fg_TWout2_too_high	                /* 出水超温保护 */
    )
    {
        fg_force_IBH_off = FALSE;
        ARRSET(fg_IBH_on, 0);
        return IBH_L0;
    }
    else if (!fg_idle_sta_antifreeze                        /* 待机态防冻不判断以下条件 */
         && !MOD_IS_RUN_STA_AC_AF(pcb_addr)                 /* 运行态空调水路防冻不判断以下条件 */
         && !MOD_IS_RUN_STA_HOTW_AF(pcb_addr)               /* 运行态热水水路防冻不判断以下条件 */
         && !TF_OPEN_IBH                                    /* 商检不判断以下条件 */
         )           
    {
        /* 辅助电热保持关闭 */
        if ((MODE_IS_HEAT(GET_SUB_SYS(pcb_addr)) && !HEAT_USE_IBH)  /* 子系统制热运行且IBH仅制热水使用 */
        || (MODE_IS_HOTW(GET_SUB_SYS(pcb_addr)) && !HOTW_USE_IBH)   /* 子系统制热水运行且IBH仅制热使用 */
        || MODE_IS_COOL(GET_SUB_SYS(pcb_addr))                      /* 子系统制冷运行 */
        || (GET_SUB_SYS(pcb_addr) == SUB_SYS_AC_HOTW && fg_TBH_run 
            && MODE_IS_HOTW(GET_SUB_SYS(pcb_addr))) /* 当前模块是否制热水运行且TBH运行，仅主模块 */
        || (!val_use_IBH)                                           /* 禁用IBH:2022-8-25TCL需求商检时不考虑该参数设置 */
        )
        {
            fg_force_IBH_off = FALSE;
            ARRSET(fg_IBH_on, 0);
            return IBH_L0;
        }
    }

#if defined(M_SHENGNENG)
    /* 生能特有逻辑-辅助电热开启延时根据环温分段 */
    if (pv_TA < -50)				/* -5℃环温分段 */
    {
        IBH_open_dly = val_IBH_open_dly;
    }
    else
    {
        IBH_open_dly = QK_60*60;	/* 60min */
    }
#endif

    /* 机组制热运行时处理 */
    if (MODE_IS_HEAT(GET_SUB_SYS(pcb_addr)) /* 制热运行 */
    && fg_heat_need			                /* 制热需求 */
    && !fg_idle_sta_antifreeze              /* 非待机态防冻 */
    && !is_defrost			                /* 非除霜 */
    && SensorCanUse(pv_ctrl_ac)				/* pv_ctrl可用 */
    && !fg_enter_test_run	                /* 非试运行 */
    )
    {
        /* IBH开启/关闭温度计算 */
        sv_TA_IBH_on = val_IBH_open_env - 10;
        sv_TA_IBH_off = val_IBH_open_env;
        sv_TWout2_IBH_on = sv_temp_freq[GET_SUB_SYS(pcb_addr)] - val_IBH_open_diff;
        sv_TWout2_IBH_off = sv_temp_freq[GET_SUB_SYS(pcb_addr)];

        /* 检查IBH是否有需求: 压机正常状态 */
        if (fg_comp_normal)
        {   
            CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_COMP_KEEP_ABNORMAL);

            /* IBH有需求条件 */
            if (SensorCanUse(pv_TA)
            && pv_TA < sv_TA_IBH_on
            && pv_ctrl_ac <= sv_TWout2_IBH_on
            && heat_runtime[GET_SUB_SYS(pcb_addr)].run >= IBH_open_dly
            )
            {
                SETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_COMP_NORMAL);
                SETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_COMP_KEEP_NORMAL);
            }
        }
        else
        {
            CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_COMP_NORMAL);
        }

        /* 检查IBH是否无需求 */
        if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_COMP_NORMAL)
        || GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_COMP_KEEP_NORMAL)
        )
        {
            /* IBH无需求条件 */
            if (pv_TA >= sv_TA_IBH_off
            || pv_ctrl_ac > sv_TWout2_IBH_off
            )
            {
                CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_COMP_NORMAL);
                CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_COMP_KEEP_NORMAL);
            }
        }

        /* IBH有需求，则计算IBH档位 */
        if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_COMP_NORMAL)
        || GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_COMP_KEEP_NORMAL)
        )
        {
            level = MAX(level, log_mod_IBH_level(pv_ctrl_ac, sv_TWout2_IBH_off, IBH_ON_SIG_RUN_HEAT_COMP_NORMAL));
        }

        /* 开启/关闭温度计算 */
        sv_TWout2_IBH_on = sv_temp_freq[GET_SUB_SYS(pcb_addr)] - val_IBH_open_diff;
        sv_TWout2_IBH_off = sv_temp_freq[GET_SUB_SYS(pcb_addr)];

        /* 检查IBH是否有需求: 压机异常状态 */
        if (fg_comp_abnormal)
        {
            CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_COMP_KEEP_NORMAL);
            
            /* IBH有需求条件 */
            if (pv_ctrl_ac <= sv_TWout2_IBH_on)
            {
                SETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_COMP_ABNORMAL);
                SETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_COMP_KEEP_ABNORMAL);
            }
        }
        else
        {
            CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_COMP_ABNORMAL);
        }

        /* 检查IBH是否无需求 */
        if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_COMP_ABNORMAL)
        || GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_COMP_KEEP_ABNORMAL)
        )
        {
            /* IBH无需求条件 */
            if (pv_ctrl_ac >= sv_TWout2_IBH_off)
            {
                CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_COMP_ABNORMAL);
                CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_COMP_KEEP_ABNORMAL);
            }
        }

        /* IBH有需求，则计算IBH档位 */
        if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_COMP_ABNORMAL)
        || GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_COMP_KEEP_ABNORMAL)
        )
        {
            level = MAX(level, log_mod_IBH_level(pv_ctrl_ac, sv_TWout2_IBH_off, IBH_ON_SIG_RUN_HEAT_COMP_ABNORMAL));
        }
    }
    else
    {
        CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_COMP_NORMAL);
        CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_COMP_KEEP_NORMAL);
        CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_COMP_ABNORMAL);
        CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_COMP_KEEP_ABNORMAL);
    }

    /* 机组制热水运行时处理 */
    if (MODE_IS_HOTW(GET_SUB_SYS(pcb_addr))
    && fg_hotw_need			                /* 热水需求 */
    && !fg_idle_sta_antifreeze
    && !is_defrost 
    && SensorCanUse(pv_THWt)
    && !fg_enter_test_run
    && !fg_timed_disinfect_IBH_need			/* 无定时杀菌阶段2的IBH需求 */
    )
    {
        /* 开启/关闭温度计算 */
        sv_THWt_IBH_on = 400;
		sv_THWt_IBH_off = sv_hotw;

        /* 检查IBH是否有需求: 压机正常状态 */
        if (fg_comp_normal)
        {
            CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_KEEP_ABNORMAL);

            /* IBH有需求条件 */
            if (hotw_runtime.run >= IBH_open_dly
            && pv_THWt < sv_THWt_IBH_on
            )
            {
                SETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_NORMAL);
                SETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL);
            }
        }
        else
        {
            CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_NORMAL);
        }

        /* 检查IBH是否无需求: 压机正常状态 */
        if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_NORMAL)
        || GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL)   
        )
        {
            /* IBH无需求条件 */
            if (pv_THWt > sv_THWt_IBH_off)
            {
				CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_NORMAL);
            	CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL);
            }
        }

        /* IBH有需求，则计算IBH档位 */
        if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_NORMAL)
        || GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL)
        )
        {
            level = MAX(level, log_mod_IBH_level(pv_THWt, sv_THWt_IBH_off, IBH_ON_SIG_RUN_HOTW_COMP_NORMAL));
        }
        

        /* 开启/关闭温度计算 */
        sv_THWt_IBH_on = sv_hotw - val_hotw_diff;
        sv_THWt_IBH_off = sv_hotw;

        /* 检查IBH是否有需求: 压机异常状态 */
        if (fg_comp_abnormal)
        {
            CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL);

            /* IBH有需求条件 */
            if (pv_THWt < sv_THWt_IBH_on)
            {
                SETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_ABNORMAL);
                SETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_KEEP_ABNORMAL);
            }
        }
        else
        {
            CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_ABNORMAL);
        }

        /* 检查IBH是否无需求: 压机异常状态 */
        if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_ABNORMAL)
        || GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_KEEP_ABNORMAL)
        )
        {
            /* IBH无需求条件 */
            if (pv_THWt > sv_THWt_IBH_off)
            {
                CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_ABNORMAL);
                CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_KEEP_ABNORMAL);
            }
        }

        /* IBH有需求，则计算IBH档位 */
        if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_ABNORMAL)
        || GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_KEEP_ABNORMAL)
        )
        {
            level = MAX(level, log_mod_IBH_level(pv_THWt, sv_THWt_IBH_off, IBH_ON_SIG_RUN_HOTW_COMP_ABNORMAL));
        }
    }
    else
    {
        CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_NORMAL);
        CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL);
        CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_ABNORMAL);
        CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_KEEP_ABNORMAL);
    }

    /* 待机态空调水路防冻时处理 */
    {
        if ((idle_sta_antifreeze_stage == AF_STAGE_AC)
        && fg_idle_sta_antifreeze_IBH_need
        )
        {
            /* IBH直接开启 */
            SETBIT_N(fg_IBH_on, IBH_ON_SIG_IDLE_STA_AC_ANTIFREEZE);
        }
        else
        {
            CLRBIT_N(fg_IBH_on, IBH_ON_SIG_IDLE_STA_AC_ANTIFREEZE);
        }

        /* IBH档位需求 */
        if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_IDLE_STA_AC_ANTIFREEZE))
        {
            level = MAX(level, IBH_L3);
        }
    }

    /* 待机态热水水路防冻时处理 */
    {
        if ((GET_SUB_SYS(pcb_addr) == SUB_SYS_AC_HOTW)
        && (idle_sta_antifreeze_stage == AF_STAGE_HOTW)
        && fg_idle_sta_antifreeze_IBH_need
        )
        {
            /* IBH直接开启 */
            SETBIT_N(fg_IBH_on, IBH_ON_SIG_IDLE_STA_HOTW_ANTIFREEZE);
        }
        else
        {
            CLRBIT_N(fg_IBH_on, IBH_ON_SIG_IDLE_STA_HOTW_ANTIFREEZE);
        }

        /* IBH档位需求 */
        if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_IDLE_STA_HOTW_ANTIFREEZE))
        {
            level = MAX(level, IBH_L3);
        }
    }

    /* 运行态空调水路防冻时处理 */
    {
        if ((GET_SUB_SYS(pcb_addr) == SUB_SYS_AC_ONLY)
        && fg_run_sta_ac_antifreeze_IBH_need
        )
        {
            /* IBH直接开启 */
            SETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_STA_AC_ANTIFREEZE);
        }
        else
        {
            CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_STA_AC_ANTIFREEZE);
        }

        /* IBH档位需求 */
        if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_STA_AC_ANTIFREEZE))
        {
            level = MAX(level, IBH_L3);
        }
    }

    /* 运行态热水水路防冻时处理 */
    {
        if ((GET_SUB_SYS(pcb_addr) == SUB_SYS_AC_HOTW)
        && fg_run_sta_hotw_antifreeze_IBH_need
        )
        {
            /* IBH直接开启 */
            SETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_STA_HOTW_ANTIFREEZE);
        }
        else
        {
            CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_STA_HOTW_ANTIFREEZE);
        }

        /* IBH档位需求 */
        if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_STA_HOTW_ANTIFREEZE))
        {
            level = MAX(level, IBH_L3);
        }
    }

    /* 商检时处理 */
    {
        if (TF_OPEN_IBH)
        {
            /* IBH直接开启 */
            SETBIT_N(fg_IBH_on, IBH_ON_SIG_TF);
        }
        else
        {
            CLRBIT_N(fg_IBH_on, IBH_ON_SIG_TF);
        }

        /* IBH档位需求 */
        if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_TF))
        {
            level = MAX(level, IBH_L3);
        }
    }

    /* 制热除霜时处理 */
    if (MODE_IS_HEAT(GET_SUB_SYS(pcb_addr))
    && is_defrost
    && SensorCanUse(pv_TWout2)
    )
    {
        /* 开启/关闭温度计算 */
        sv_TWout2_IBH_on = 160;
        sv_TWout2_IBH_off = 200;
       
        /* IBH开启条件 */
        if (pv_TWout2 <= sv_TWout2_IBH_on)
        {
            SETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_DEFROST);
        }
       
        /* IBH关闭条件 */
        if (pv_TWout2 >= sv_TWout2_IBH_off)
        {
            CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_DEFROST);
        }

        /* IBH档位需求 */
        if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_DEFROST))
        {
            level = MAX(level, log_mod_IBH_level(pv_TWout2, sv_TWout2_IBH_off, IBH_ON_SIG_RUN_HEAT_DEFROST));
        }
    }
    else
    {
        CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_DEFROST);
    }

    /* 制热水除霜处理 */
    if (MODE_IS_HOTW(GET_SUB_SYS(pcb_addr))
    && is_defrost
    && SensorCanUse(pv_THWt)
    )
    {
        /* 开启/关闭温度计算 */
        sv_THWt_IBH_on = 160;
        sv_THWt_IBH_off = 200;

        /* IBH开启条件 */
        if (pv_THWt <= sv_THWt_IBH_on)
        {
            SETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_DEFROST);
        }

        /* IBH关闭条件 */
        if (pv_THWt >= sv_THWt_IBH_off)
        {
            CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_DEFROST);
        }

        /* IBH档位需求 */
        if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_DEFROST))
        {
            level = MAX(level, log_mod_IBH_level(pv_THWt, sv_THWt_IBH_off, IBH_ON_SIG_RUN_HOTW_DEFROST));
        }
    }
    else
    {
        CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_DEFROST);
    }
    
    /* 强开热水模式时处理 */
    if (fg_force_hotw_mode_run
    && SensorCanUse(pv_THWt)
    && SensorCanUse(pv_TWout2)
    )
    {
        /* IBH开启/关闭温度计算 */
        sv_THWt_IBH_on = sv_hotw - val_hotw_diff;
        sv_THWt_IBH_off = sv_hotw;

        /* IBH开启条件 */
        if (pv_THWt < sv_THWt_IBH_on)
        {
            SETBIT_N(fg_IBH_on, IBH_ON_SIG_FORCE_HOTW);
        }

        /* IBH关闭条件 */
        if (pv_THWt >= sv_THWt_IBH_off)
        {
            CLRBIT_N(fg_IBH_on, IBH_ON_SIG_FORCE_HOTW);
        }

        /* IBH档位需求 */
        if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_FORCE_HOTW))
        {
            level = MAX(level, log_mod_IBH_level(pv_THWt, sv_THWt_IBH_off, IBH_ON_SIG_FORCE_HOTW));
        }
    }
    else
    {
        CLRBIT_N(fg_IBH_on, IBH_ON_SIG_FORCE_HOTW);
    }

#ifdef USE_NEW_FORCE_IBH
    /* 强开辅助电热命令处理 */
    if (fg_force_IBH_run)
    {
        /* 制热运行，强开辅助电热命令处理 */
        if ((fg_zone_A_run || fg_zone_B_run)
        && SensorCanUse(pv_ctrl_ac)
        && !fg_heat_force_IBH_once			/* 已强开过一次 */
        )
        { 
            /* IBH开启/关闭温度计算 */
            sv_TWout2_IBH_on = sv_temp_freq[GET_SUB_SYS(pcb_addr)] - val_IBH_open_diff;
            sv_TWout2_IBH_off = sv_temp_freq[GET_SUB_SYS(pcb_addr)];

            fg_heat_force_IBH_off = FALSE;

            /* IBH开启条件 */
            if (pv_ctrl_ac < sv_TWout2_IBH_on)
            {
                SETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_FORCE_ON);
            }

            /* IBH关闭条件 */
            if (pv_ctrl_ac >= sv_TWout2_IBH_off)
            {
                CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_FORCE_ON);
                fg_heat_force_IBH_off = TRUE;
                fg_heat_force_IBH_once = TRUE;
            }
        }
        else
        {
            CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_FORCE_ON);
            fg_heat_force_IBH_off = TRUE;
        }

        /* 制热水运行，强开辅助电热命令处理 */
        if (fg_hotw_run
        && SensorCanUse(pv_THWt)
        && SensorCanUse(pv_TWout2)
        && !fg_hotw_force_IBH_once			/* 已强开过一次 */
        )
        {
            /* IBH开启/关闭温度计算 */
            sv_THWt_IBH_on = sv_hotw - val_hotw_diff;
            sv_THWt_IBH_off = sv_hotw;

            fg_hotw_force_IBH_off = FALSE;
            
            /* IBH开启条件 */
            if (pv_THWt < sv_THWt_IBH_on)
            {
                SETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_FORCE_ON);
            }
        
            /* IBH关闭条件 */
            if (pv_THWt >= sv_THWt_IBH_off)
            {
                CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_FORCE_ON);
                fg_hotw_force_IBH_off = TRUE;
                fg_hotw_force_IBH_once = TRUE;
            }
        }
        else
        {
            CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_FORCE_ON);
            fg_hotw_force_IBH_off = TRUE;
        }

        /* 补丁：非对应模式清对应需求 */
        if (!MODE_IS_HEAT(GET_SUB_SYS(pcb_addr)))
        {
            CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_FORCE_ON);
        }

        if (!MODE_IS_HOTW(GET_SUB_SYS(pcb_addr)))
        {
            CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_FORCE_ON);
        }

        /* IBH档位需求 */
        if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_FORCE_ON))
        {
            level = MAX(level, log_mod_IBH_level(pv_ctrl_ac, sv_TWout2_IBH_off, IBH_ON_SIG_RUN_HEAT_FORCE_ON));
        }

        if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_FORCE_ON))
        {
            level = MAX(level, log_mod_IBH_level(pv_THWt, sv_THWt_IBH_off, IBH_ON_SIG_RUN_HOTW_FORCE_ON));
        }
            
        /* 退强开判断 */
        if (fg_heat_force_IBH_off && fg_hotw_force_IBH_off)
        {
            fg_force_IBH_off = TRUE;
        }
    }
    else
    {
        CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_FORCE_ON);
        CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_FORCE_ON);
        fg_force_IBH_off = FALSE;
        fg_heat_force_IBH_off = FALSE;
        fg_hotw_force_IBH_off = FALSE;
        fg_heat_force_IBH_once = FALSE;
        fg_hotw_force_IBH_once = FALSE;
    }
#else
    /* 取消编译警告 */
    if (fg_heat_force_IBH_off);
    if (fg_hotw_force_IBH_off);
    if (fg_heat_force_IBH_once);
    if (fg_hotw_force_IBH_once);

    /* 制热运行，强开辅助电热命令处理 */
    if (fg_force_IBH_run
    && MODE_IS_HEAT(GET_SUB_SYS(pcb_addr))
    && SensorCanUse(pv_ctrl_ac)
    )
    { 
        /* IBH开启/关闭温度计算 */
        sv_TWout2_IBH_on = sv_temp_freq[GET_SUB_SYS(pcb_addr)] - val_IBH_open_diff;
        sv_TWout2_IBH_off = sv_temp_freq[GET_SUB_SYS(pcb_addr)];

        /* IBH开启条件 */
        if (pv_ctrl_ac < sv_TWout2_IBH_on)
        {
            SETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_FORCE_ON);
        }

        /* IBH关闭条件 */
        if (pv_ctrl_ac >= sv_TWout2_IBH_off)
        {
            CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_FORCE_ON);
            fg_force_IBH_off = TRUE;
        }

        /* IBH档位需求 */
        if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_FORCE_ON))
        {
            level = MAX(level, log_mod_IBH_level(pv_ctrl_ac, sv_TWout2_IBH_off, IBH_ON_SIG_RUN_HEAT_FORCE_ON));
        }
    }
    else
    {
        CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_FORCE_ON);
        fg_force_IBH_off = FALSE;
    }
    
    /* 制热水运行，强开辅助电热命令处理 */
    if (fg_force_IBH_run
    && MODE_IS_HOTW(GET_SUB_SYS(pcb_addr))
    && SensorCanUse(pv_THWt)
    && SensorCanUse(pv_TWout2)
    )
    {    
        /* IBH开启/关闭温度计算 */
        sv_THWt_IBH_on = sv_hotw - val_hotw_diff;
        sv_THWt_IBH_off = sv_hotw;
        
        /* IBH开启条件 */
        if (pv_THWt < sv_THWt_IBH_on)
        {
            SETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_FORCE_ON);
        }
    
        /* IBH关闭条件 */
        if (pv_THWt >= sv_THWt_IBH_off)
        {
            CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_FORCE_ON);
            fg_force_IBH_off = TRUE;
        }

        /* IBH档位需求 */
        if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_FORCE_ON))
        {
            level = MAX(level, log_mod_IBH_level(pv_THWt, sv_THWt_IBH_off, IBH_ON_SIG_RUN_HOTW_FORCE_ON));
        }
    }
    else
    {
        CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_FORCE_ON);
        fg_force_IBH_off = FALSE;
    }
#endif

    /* 定时杀菌时处理 */
    if ((GET_SUB_SYS(pcb_addr) == SUB_SYS_AC_HOTW)
    && fg_timed_disinfect_run//fg_timed_disinfect_IBH_need
    )
    {
        #define DISINFECT_STAGE_1   1		/* fg_timed_disinfect_IBH_need==FALSE */
        #define DISINFECT_STAGE_2   2		/* fg_timed_disinfect_IBH_need==TRUE */
    
        //static BOL disinfect_stage_1_to_2 = FALSE;		/* 杀菌阶段1过渡到2标志，查询后复位 */
        static BOL disinfect_stage_2_to_1 = FALSE;		/* 杀菌阶段2过渡到1标志，查询后复位，正常使用不会触发 */
        static U08 disinfect_stage_bak    = DISINFECT_STAGE_1;		/* 杀菌阶段副本 */

        /* 开启/关闭温度计算 */
        sv_THWt_IBH_on = val_ster_temp - val_hotw_diff;
        sv_THWt_IBH_off = val_ster_temp;
        
        /* 杀菌阶段跨越判断 */
        if (!fg_timed_disinfect_IBH_need)	/* 阶段1 */
        {
            if (disinfect_stage_bak == DISINFECT_STAGE_2)
            {
                /* 到这里表明阶段2跨越到阶段1 */
                disinfect_stage_2_to_1 = TRUE;
                disinfect_stage_bak = DISINFECT_STAGE_1;
            }
        }
        else                                /* 阶段2 */
        {
            if (disinfect_stage_bak == DISINFECT_STAGE_1)
            {
                /* 到这里表明阶段1跨越到阶段2 */
                //disinfect_stage_1_to_2 = TRUE;
                disinfect_stage_bak = DISINFECT_STAGE_2;
            }
        }
    
        /* 阶段跨越补丁：若阶段1开了IBH，则认为是定时杀菌的IBH需求 */
        if (disinfect_stage_bak == DISINFECT_STAGE_1)/* 阶段1 */
        {
//            /* 杀菌阶段1热水开了IBH */
//            if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_NORMAL)
//            ||  GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL)
//        	||	GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_ABNORMAL)
//        	||  GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_KEEP_ABNORMAL))
//        	{
//        	    /* 认为阶段1开的IBH也是杀菌的IBH需求 */
//        	    SETBIT_N(fg_IBH_on, IBH_ON_SIG_DISINFECT);
//
//        	    /* 开启/关闭温度计算 */
//                sv_THWt_IBH_on = 400;
//        		sv_THWt_IBH_off = sv_hotw;
//        	}
//        	else	/* 阶段1跨越阶段2前一刻，制热水IBH需求清零，需要调整档位计算温度，否则会闪跳 */
//        	{
//                /* 开启/关闭温度计算 */
//                sv_THWt_IBH_on = val_ster_temp - val_hotw_diff;
//                sv_THWt_IBH_off = val_ster_temp;
//        	}

        	/* 杀菌阶段1热水开了IBH */
            if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_NORMAL)
            ||  GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL)
        	||	GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_ABNORMAL)
        	||  GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_KEEP_ABNORMAL))
        	{
        	    /* 认为阶段1开的IBH也是杀菌的IBH需求 */
        	    SETBIT_N(fg_IBH_on, IBH_ON_SIG_DISINFECT);
        	}

        	/* 阶段2到1跨越时关闭IBH */
        	if (disinfect_stage_2_to_1 == TRUE)
        	{
        	    disinfect_stage_2_to_1 = FALSE;
                CLRBIT_N(fg_IBH_on, IBH_ON_SIG_DISINFECT);
        	}
        }
        else				                        /* 阶段2 */
        {
            /* IBH开启条件 */
            if (pv_THWt <= sv_THWt_IBH_on
            && cnt_ster_cyc < val_ster_max_cycle
            && cnt_ster_high_temp < val_ster_high_temp_Time
            )
            {
                SETBIT_N(fg_IBH_on, IBH_ON_SIG_DISINFECT);
            }

            /* IBH关闭条件 */
            if (cnt_ster_cyc >= val_ster_max_cycle
            || cnt_ster_high_temp >= val_ster_high_temp_Time
            || pv_THWt >= sv_THWt_IBH_off
            )
            {
                CLRBIT_N(fg_IBH_on, IBH_ON_SIG_DISINFECT);
            }
        }

        /* IBH档位需求（会覆盖制热水的计算） */
        if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_DISINFECT))
        {
            level = MAX(level, log_mod_IBH_level(pv_THWt, sv_THWt_IBH_off, IBH_ON_SIG_DISINFECT));
        }
    }
    else
    {
        CLRBIT_N(fg_IBH_on, IBH_ON_SIG_DISINFECT);
    }

    /* 制热试运行时处理 */
    if (fg_test_run_heat_IBH_run)
    {
        SETBIT_N(fg_IBH_on, IBH_ON_SIG_TEST_RUN_HEAT_MODE_ON);
        level = IBH_L3;
    }
    else
    {
        CLRBIT_N(fg_IBH_on, IBH_ON_SIG_TEST_RUN_HEAT_MODE_ON);
    }

    return level;
}

/*******************************************************************************
 函 数: log_mod_IBH_out()
 功 能: 辅助电热输出
 参 数: type--辅助电热配置; 
        level--辅助电热需求; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_mod_IBH_out(U08 type, U08 level)
{
    #define IBH_ADD_ITV 3	/* 电热连开间隔 */
    
    if (level == IBH_L0		                /* 不开 */
    ||  type == IBH_NONE                    /* IBH1/2拨码未设置 */
    || !fg_PUMPf_run		                /* PUMPf未运行 */
    || !log_mod_pump_is_run()               /* 模块水泵未稳定运行 */
    || s_get_error(S_ERR_IBH_OVERLOAD)      /* 辅助电热过载 */
    )
    {
        return;
    }
    else if (type==IBH_ONLY1)
    {
        s_set_output(MAX_COMP, S_DO_IBH1);
    }
    else if (type==IBH_ONLY2)
    {
        s_set_output(MAX_COMP, S_DO_IBH2);
    }
    else if (type==IBH_1AND2)
    {
        switch (val_IBH_gear_num)
        {
            default:
            case 1:
                s_set_output(MAX_COMP, S_DO_IBH1);
                if (IBH2_add_dly >= IBH_ADD_ITV)
                {
                    s_set_output(MAX_COMP, S_DO_IBH2);
                }
                break;
            case 2:
                if (level == IBH_L1)
                {
                    s_set_output(MAX_COMP, S_DO_IBH1);
                }
                else
                {
                    s_set_output(MAX_COMP, S_DO_IBH1);
                    if (IBH2_add_dly >= IBH_ADD_ITV)
                    {
                        s_set_output(MAX_COMP, S_DO_IBH2);
                    }
                }
                break;
            case 3:
                if (level == IBH_L1)
                {
                    s_set_output(MAX_COMP, S_DO_IBH1);
                }
                else if (level == IBH_L2)
                {
                    s_set_output(MAX_COMP, S_DO_IBH2);
                }
                else
                {
                    s_set_output(MAX_COMP, S_DO_IBH1);
                    if (IBH2_add_dly >= IBH_ADD_ITV)
                    {
                        s_set_output(MAX_COMP, S_DO_IBH2);
                    }
                }
                break;
        }
    }
}

/*******************************************************************************
 函 数: log_mod_IBH()
 功 能: 辅助电热控制
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_mod_IBH(void)
{
    U08 IBH_level = log_mod_IBH_need();
    
    log_mod_IBH_out(IBH_cfg_type, IBH_level);
}

#ifdef M_DEBUG
/*******************************************************************************
 函 数: log_mod_debug()
 功 能: 仅供内部调试时查看状态使用
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_mod_debug(void)
{
    U08 i;
    
    for (i = 0; i < IBH_ON_SIG_MAX; i++)
    {
        ibhDebug[i] = GETBIT_N(fg_IBH_on, i);
    }
}
#endif

/*******************************************************************************
 函 数: log_mod_HT()
 功 能: 防冻结加热带控制
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_mod_HT(void)
{
    #define OPEN_ENV                (30)    // 开启防冻带环温
    #define CLOSE_EVN               (50)    // 关闭防冻带环温
    #define OPEN_FREZZING_TEMP      (50)    // 开启防冻带防冻温度
    #define CLOSE_FREZZING_TEMP     (100)   // 关闭防冻带防冻温度

    #define COMP_OFF_TIME1          (2*60)  // 停机2min内
    #define COMP_OFF_TIME2          (30*60) // 停机30min内

    static BOL fg_open = FALSE;
    I16 pv_antifreeze = SENSOR_NOEXIST;   /* 防冻温度 */
    I16 pv_TA = SENSOR_NOEXIST;

    /* 获取防冻温度&环境温度 */
    pv_antifreeze = fun_pv_min(pv_cell_TWin1(pcb_addr), pv_cell_TWout1(pcb_addr));
    pv_TA = pv_cell_TA(pcb_addr);

    if (!SensorCanUse(pv_antifreeze)
    && !SensorCanUse(pv_TA)
    )
    {
        /* 防冻温度和环境温度都故障时，有压机运行关闭 */
        if (log_mod_comp_is_run(MODE_NULL))
        {
            fg_open = FALSE;
        }
        else
        {
            fg_open = TRUE;
        }
    }
    else if (!SensorCanUse(pv_antifreeze)
    && SensorCanUse(pv_TA)
    )
    {
        /* 防冻温度故障，环温可用时 */
        if (pv_TA >= CLOSE_EVN)
        {
            fg_open = FALSE;
        }
        else if (pv_TA < OPEN_ENV)
        {
            fg_open = TRUE;
        }
    }
    else if (SensorCanUse(pv_antifreeze)
    && !SensorCanUse(pv_TA)
    )
    {
        /* 防冻温度可用，环温不可用时 */
        if (pv_antifreeze >= CLOSE_FREZZING_TEMP)
        {
            fg_open = FALSE;
        }
        else if (pv_antifreeze < OPEN_FREZZING_TEMP)
        {
            fg_open = TRUE;
        }
    }
    else					/* 都可用时 */
    {
        /* 制冷模式下 */
        if ((MODE_IS_COOL(GET_SUB_SYS(pcb_addr))) && (cm_time[0].off <= COMP_OFF_TIME2))
        {
            if (cm_time[0].off <= COMP_OFF_TIME1)// 2min内
            {
                fg_open = FALSE;
            }
            else    // 2~30min内
            {
                if (pv_antifreeze >= fun_pv_min(sv_temp_energy[GET_SUB_SYS(pcb_addr)]+val_cool_add_diff, CLOSE_FREZZING_TEMP))
                {
                    fg_open = FALSE;
                }
                else if (pv_antifreeze < OPEN_FREZZING_TEMP - 10)
                {
                    fg_open = TRUE;
                }
            }
        }
        else				/* 其余按照如下方式进行防冻 */
        {
            if ((pv_TA >= CLOSE_EVN)
            || (pv_antifreeze >= CLOSE_FREZZING_TEMP)
            )
            {
                fg_open = FALSE;
            }
            else if ((pv_TA < OPEN_ENV)
            && (pv_antifreeze < OPEN_FREZZING_TEMP)
            )
            {
                fg_open = TRUE;
            }
        }
    }

    if (!fg_open)
    {
        cnt_HT_on_dly = 0;
    }
    
    if (fg_open 
    && cnt_HT_on_dly > 2
    )
    {
        s_set_output(MAX_COMP, S_DO_HT);
    }
}

    
/*==========================================
    功能: 逻辑过程
/==========================================*/
void log_mod(void)
{
    log_mod_begin();
    
    if (power_time < POWER_ON_INTO_WORK) return;  

    log_mod_cmd();               /* 命令处理 */
//  log_mod_Heater_Valve();  

    log_mod_PUMPf();		/* 变频水泵 */
    log_mod_IBH();			/* 辅助电热 */
    log_mod_HT();			/* 防冻加热带 */

    Logic_Anti_Icing_Heater();  /* 底盘电加热*/
    log_mod_comp();

    /* 模块禁用或通讯故障时，清除能量系统 */
    if (!CELL_Is_used(pcb_addr) || self_commu_err) 
    {
        U08  i;
        
        for (i=0; i<self_comp_num; i++)
        {
//          log_mod_comp_quick_stop(i);
            log_mod_comp_stop(i, QUICK_ALL_STOP);
        }
    }

    log_mod_eev();

//  s_Defrost_deal();
    defrost();
    log_mod_end();

    /* 模块功率统计:用到get_output，需放到log_mod_end之后 */
    log_mod_elec();			

#ifdef M_DEBUG
    log_mod_debug();
#endif
}


