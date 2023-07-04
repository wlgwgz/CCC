#define LOG_SYS_GLOBAL

#include "includes.h"

/* ========================================================================================== */

#define JOINT_ENERGY_CTRL        (0)          /* 联合能调 */
#define GROUP_ENERGY_CTRL        (1)          /* 分组能调 */
static U08 energy_ctrl_type;                  /* 能调类型 */

#define ANTI_ALL                    0           //正常防冻
#define ANTI_NON_ENV                1           //不考虑环境温度的防冻
#define ANTI_UNUSUAL                2           //特殊防冻，水泵一直开

/* 水泵计时定义 */
#define PM_F_ANY                   (0)		/* 任意模块变频水泵 */
#define PM_RET                     (1)		/* 下回水泵 */
#define PM_F_HW_RATFZ              (2)		/* 用于运行态热水水路防冻的所有可制热水模块变频水泵 */
#define PM_F_NHW_RATFZ             (3)		/* 用于运行态空调水路防冻的所有不可制热水模块变频水泵 */
#define PM_F_AC_HW                 (4)		/* 所有既可制热水又可制冷(或制热)模块的变频水泵 */
#define PM_O_TATFZ                 (5)      /* 用于A区定时防冻的外循环泵 */
#define PM_M_TATFZ                 (6)      /* 用于B区定时防冻的混合水泵 */
#define PM_HW_TATFZ                (7)		/* 所有可参与热水定时防冻变频水泵 */
#define PM_MAX                     (8)
//#define AC_WT                1   /* 热水防冻能量调节时间 */


/* 外部热源 */
static RTIME AHS_time;

#define HEAT_ANTI_TEMP_DIFF         (10*2)      //电热防冻温差
#define EXIT_ANTI_ENV_DIFF          (10*1)      //退防冻环温差
#define EXIT_ANTI_TEMP_DIFF         (10*9)      //退防冻水温温差

#define ANTI_UNUSUAL_DLY       600	                /* 特殊防冻时空调/热水水路切换时间 */
#define ANTI_PRE_DLY           180                  /* 防冻时水泵预开时间(秒) */   
#define ANTI_CHG_DLY           900                  /* 防冻时水路强制切换时间(秒) */ 
#define ANTI_EXIT_DLY          (ANTI_CHG_DLY*4)     /* 防冻强制退出时间(秒) */ 

//#define val_defrost_comp_max              (50)        /* 除霜压机百分数 */



#define  cm_runtime(x, y)           (cell[x].cell_runtime[y])

static BOOL fg_have_stop;
static U08 ctrl_mode_bak[SUB_SYS_TYPE_MAX];


GSTATIC U16 power_down_time_space = 0xffff;         /* 掉电时间存储间隔计时(第一次上电预热完成后，保存一次) */

//static U08 bak_ac_need =_KEEP;
/* ========================================================================================== */
#define QUICK_PERIOD    1   /* 急停控制周期 */
#define OPEN_PERIOD     4   /* 开机控制周期 */
static U16 m_ctrl_time;		/* 主模块能调控制周期计时 */
static U16 s_ctrl_time;		/* 从模块能调控制周期计时 */
static U16 m_ctrl_period;	/* 主模块能调控制周期 */
static U16 s_ctrl_period;	/* 从模块能调控制周期 */

/* ========================================================================================== */

static U08 air_pump_dly[SUB_SYS_TYPE_MAX];	/* 子系统变频水泵切换开启和关闭延时 */
static U16 pump_inter_on_off_time[SUB_SYS_TYPE_MAX];      //水泵间隔运行时间
//static BOL fg_ac_need_enable[SUB_SYS_TYPE_MAX] = {TRUE, TRUE};     // 允许计算需求(用于水泵间歇停的模式)

static U08 cnt_elec_stop = 0xff;
static U16 cnt_send_fan_stop  = 0;  // 送风机暂停延时
static U16 avoid_cold_fan_cnt;      // 防冷风计时

//static BOOL fg_ws_pm_open;                /* 冷却水泵标志 */
//static BOOL fg_ac_pm_open;              /* 空调泵已开启(经过水流检测延时后置位) */
static RTIME pump_time[PM_MAX];   /* 0: 空调泵; 1:冷却泵*/   
U08 m_cm_time_on[MAX_PCB][MAX_COMP];    /* 压机开启时间计时 */


/* ========================================================================================== */
/* 防冻变量 */
enum STATUS_ANTIFREEZE 
{
    ANTIFREEZE_IDLE, 
    ANTIFREEZE_PUMP, 
    ANTIFREEZE_DEAL, 
    ANTIFREEZE_OPEN_COMP, 
    ANTIFREEZE_CLOSE_COMP, 
    ANTIFREEZE_CLOSE_PUMP
};
static U08 idle_sta_antifreeze_status;		/* 待机态防冻状态 */
static U08 run_sta_ac_antifreeze_status;	/* 运行态空调防冻状态 */
static U08 run_sta_hotw_antifreeze_status;	/* 运行态热水防冻状态 */

static U16  freeze_energy_time[PM_MAX];         /* 防冻能量调节计时空调侧(0: 空调侧; 2: 水源侧) */
static U16  freeze_act_time;         /* 防冻能量调节计时空调侧(0: 空调侧; 2: 水源侧) */

// 计时变量定义 禁用使用GetTime函数，存在溢出风险 
//static U16 commu_cmd_comp_mode_interval;            /* 命令发送时间间隔 */
static U16 commu_cmd_comp_all_stop_interval;        /* 命令发送时间间隔 */ 
static U16 commu_cmd_err_reset_interval;            /* 命令发送时间间隔 */
static U16 commu_cmd_stop_all_mac_interval;         /* 命令发送时间间隔 */
static U16 commu_cmd_fac_para_init_interval;        /* 命令发送时间间隔 */ 
static U16 commu_cmd_invt_init_interval;            /* 命令发送时间间隔 */ 
static U16 commu_cmd_limit_para_init_interval;      /* 命令发送时间间隔 */ 
static U16 commu_cmd_password_save_interval;        /* 命令发送时间间隔 */ 
static U16 commu_cmd_runtime_init_interval;         /* 命令发送时间间隔 */
static U16 commu_cmd_systime_init_interval;         /* 命令发送时间间隔 */
static U16 commu_cmd_systime_save_interval;         /* 命令发送时间间隔 */
static U16 commu_cmd_comp_quik_stop_interval[MAX_PCB][MAX_COMP];         /* 命令发送时间间隔 */
static U16 commu_cmd_comp_system_stop_interval[MAX_PCB][MAX_COMP];       /* 命令发送时间间隔 */  
static U16 commu_cmd_comp_allow_defrost_interval[MAX_PCB][MAX_COMP];     /* 命令发送时间间隔 */    
static U16 commu_cmd_comp_force_defrost_interval[MAX_PCB][MAX_COMP];     /* 命令发送时间间隔 */ 
static U16 commu_cmd_cell_open[MAX_PCB];
static U16 commu_cmd_cell_close[MAX_PCB];

U08  ac_heater_dly;         /* 除霜结束时开空调电热计时 */
U16  sys_have_run_time;         /* 系统运行时间计时 */

//static BOOL fg_solar_limit_comp_run = FALSE;    // 太阳能运行时限制压缩机制热水
static U16 cnt_HDIsl_on = 0;
static U16 cnt_HDIsl_off = 0xffff;

static U16 cnt_TSL_on = 0;
static U16 cnt_TSL_off = 0xffff;

static U08 cnt_IBH_on;
static U08 cnt_test_run_hotw_TBH_on;

static U08 m_run_mode_need[MAX_PCB][MAX_COMP];       /* 压机需求模式(系统使用) */
static U08 fg_need_comp_to_run[MAX_COMP];	/* 压机有开启需求(命令发送后该标志清零) */

//static U16 fg_cell_on[_DIVUCARRY(MAX_PCB, 16)];				/* 模块开启标志(命令发送后该标志清零) */
//static U16 fg_cell_off[_DIVUCARRY(MAX_PCB, 16)];			/* 模块关闭标志(命令发送后该标志清零) */

//static RTIME comp_time;         /* 压机时间 */

static RTIME floor_preheat_time;
static U16 peak_elec_air_runtime;

static BOL fg_defrost_run[SUB_SYS_TYPE_MAX];/* 除霜运行标志 */

//static BOL fg_00_comp_normal;				/* 压机正常状态标志 */
//static BOL fg_00_comp_abnormal;				/* 压机异常状态标志 */

static BOL fg_ac_hotw_comp_normal;          /* 热水模块压机正常状态标志 */
static BOL fg_ac_hotw_comp_abnormal;        /* 热水模块压机异常状态标志 */

static U08 fg_TA_limit_comp_open[SUB_SYS_TYPE_MAX];           /* 环境温度限制压缩机开启 */
static BOL fg_HDIsl_limit_comp_open;                        /* 太阳能信号限制压缩机开启 */
static BOL fg_TSL_limit_comp_open;                          /* 太阳能温度限制压缩机开启 */
static BOL fg_floor_preheat_limit_comp_open;                /* 地暖预热限制压缩机开启 */

enum {TRAE_IDLE, TRAE_IDU, TRAE_HW, TRAE_A_HT, TRAE_A_CL, TRAE_B_HT};
static U08 test_run_air_empty_stage = TRAE_IDLE;

enum {TRP_IDLE, TRP_HW, TRP_A_HT, TRP_A_CL, TRP_B_HT};
static U08 test_run_pump_stage = TRP_IDLE;

/* ========================================================================================== */


/* ========================================================================================== */


/*----------------------------------------*/

U08 log_sys_is_rdy(void);
BOOL log_sys_comp_use_status(U08 pcb_num, U08 comp_num, U16 attrib);
U08 need_comp_num_in_system(U08 type);
BOL log_sys_TA_limit_comp_is_off(U08 mode, U08 type);
BOL log_sys_TA_limit_comp_is_run(U08 mode, U08 type);
BOL m_comp_use_mode(U08 pcb_num, U08 comp_num, U08 mode);

BOL log_sys_can_enter_timed_disinfect(void);
BOL log_sys_can_enter_force_hotw(void);

BOOL log_sys_cell_pump_can_open(U08 pcb_num);
BOL log_sys_cell_pump_can_close(U08 pcb_num);

/*******************************************************************************
 函 数: log_sys_init()
 功 能: 系统变量初始化
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_init(void)
{
    power_time = 0;         //上电时间
    punp_fg = FALSE;        //测试标志 
    test_allow = FALSE;     //测试允许
//    disp_mode = MODE_HEAT;
    log_sys_init_work();
}

/*****************************************************
    功能：压机模式命令
******************************************************/
void cmd_comp_mode(U08 pcb_num, U08 to_mode)
{
    cell[pcb_num].cell_cmd[MAX_COMP] |= ((to_mode&0x07)<<CMD_MODE);   /* 故障复位命令 */
    SET_BIT(cmd_modify, pcb_num);   /* 命令修改标识 */
}
/*****************************************************
    功能：压机开命令
******************************************************/
void cmd_comp_run(U08 pcb_num, U08 comp_num, U08 to_mode)
{
    cell[pcb_num].cell_cmd[comp_num] |= ((to_mode&0x07)<<CMD_CM_MODE);         /* 运行模式 */
    cell[pcb_num].cell_cmd[comp_num] |= CMD_OPEN_COMP;   /* 开压机命令 */
    SET_BIT(cmd_modify, pcb_num);   /* 命令修改标识 */
}
/*****************************************************
    功能：压机关命令
******************************************************/
void cmd_comp_close(U08 pcb_num, U08 comp_num)
{
    cell[pcb_num].cell_cmd[comp_num] |= CMD_CLOSE_COMP;   /* 关压机命令 */
    
    SET_BIT(cmd_modify, pcb_num);   /* 命令修改标识 */
}
/*****************************************************
    功能：压机急停命令
******************************************************/
void cmd_comp_quik_stop(U08 pcb_num, U08 comp_num)
{
    if (commu_cmd_comp_quik_stop_interval[pcb_num][comp_num] > 10)
    {
        if (M_CM_GetStatus(pcb_num, comp_num, COMP_RUN_NEED))
        {
            cell[pcb_num].cell_cmd[comp_num] |= CMD_QUICK_STOP_COMP;   /* 急停压机命令 */
            
            SET_BIT(cmd_modify, pcb_num);   /* 命令修改标识 */
        }
        commu_cmd_comp_quik_stop_interval[pcb_num][comp_num] = 0;
    }
}
/*****************************************************
    功能：停压机系统命令
******************************************************/
void cmd_comp_system_stop(U08 pcb_num, U08 comp_num)
{
    if (commu_cmd_comp_system_stop_interval[pcb_num][comp_num] > 10)
    {
        cell[pcb_num].cell_cmd[comp_num] |= CMD_STOP_COMP_SYSTEM;   /* 停压机系统命令 */
            
        SET_BIT(cmd_modify, pcb_num);   /* 命令修改标识 */
        commu_cmd_comp_system_stop_interval[pcb_num][comp_num] = 0;
    }
}
/*****************************************************
    功能：压机切换模式
******************************************************/
//void cmd_comp_switch(U08 pcb_num, U08 comp_num, U08 to_mode)
//{
//  cell[pcb_num].cell_cmd[comp_num] |= to_mode;           /* 运行模式 */
//  cell[pcb_num].cell_cmd[comp_num] |= CMD_SWITCH_COMP;   /* 压机切换模式命令 */
//  
//  SET_BIT(cmd_modify, pcb_num);   /* 命令修改标识 */
//}
/*****************************************************
    功能：压机除霜命令
        说明： 除霜命令是指从机申请除霜后，主机发送的允许除霜指令
******************************************************/
void cmd_comp_allow_defrost(U08 pcb_num, U08 comp_num)
{
    if (commu_cmd_comp_allow_defrost_interval[pcb_num][comp_num] > 10)
    {
        cell[pcb_num].cell_cmd[comp_num] |= CMD_ALLOW_DEFROST;   /* 压机允许除霜命令 */
//      cell[pcb_num].cell_cmd[comp_num] |= ((U16)allow_num << CMD_ALLOW_DEFRO_NUM); /* 允许除霜压机数 */
        
        SET_BIT(cmd_modify, pcb_num);                           /* 命令修改标识 */
        commu_cmd_comp_allow_defrost_interval[pcb_num][comp_num] = 0;     /* 重新计时 */
        
        //fg_trans_control_cmd = TRUE;
        M_CM_SetStatus(pcb_num, comp_num, COMP_DEFROST);    /* 同步标志 */
//      SETBIT(fg_comp_defrost[comp_num], pcb_num);  /* 发送命令，置位辅助标志 */
    }
}
/*****************************************************
    功能：压机强制除霜命令
        
******************************************************/
void cmd_comp_force_defrost(U08 pcb_num, U08 comp_num)
{
    if (commu_cmd_comp_force_defrost_interval[pcb_num][comp_num] > 5)
    {
        cell[pcb_num].cell_cmd[comp_num] |= CMD_FORCE_DEFROST;   /* 压机除霜命令 */
        
        SET_BIT(cmd_modify, pcb_num);   /* 命令修改标识 */
        commu_cmd_comp_force_defrost_interval[pcb_num][comp_num] = 0;
    }
}
/*****************************************************
    功能：压机全停命令
        说明： 该命令属于广播命令
******************************************************/
void cmd_stop_all_comp(void)
{
    if (commu_cmd_comp_all_stop_interval > 3)
    {
        cell[MAX_PCB].cell_cmd[MAX_COMP] |= CMD_STOP_ALL_COMP;   /* 压机全停命令 */
        //      cell[0].cell_cmd[MAX_COMP] |= CMD_STOP_ALL_COMP;   /* 压机全停命令 */
        cmd_broadcast = 1;        /* 置广播标志 */
        commu_cmd_comp_all_stop_interval = 0;
    }
}

/*****************************************************
    功能： 故障复位命令
        说明： 该命令属于广播命令
               10秒才发送一次，防止连续的发送影响其它板的通讯
******************************************************/
void cmd_err_reset(void)
{
    if (commu_cmd_err_reset_interval > 5)
    {
        cell[MAX_PCB].cell_cmd[MAX_COMP] |= CMD_ERR_RESET;   /* 故障复位命令 */
        //      cell[0].cell_cmd[MAX_COMP] |= CMD_ERR_RESET;        /* 故障复位命令 */
        cmd_broadcast = 1;        /* 置广播标志 */
        commu_cmd_err_reset_interval = 0;
        key_reset = FALSE;
    }
}
/*****************************************************
    功能： 所有器件全停命令
        说明： 该命令属于广播命令
******************************************************/
void cmd_stop_all_mac(void)
{
    if (commu_cmd_stop_all_mac_interval > 3)
    {
        cell[MAX_PCB].cell_cmd[MAX_COMP] |= CMD_STOP_ALL_MAC;   /* 所有器件全停命令 */
        //      cell[0].cell_cmd[MAX_COMP] |= CMD_STOP_ALL_MAC;   /* 所有器件全停命令 */
        cmd_broadcast = 1;        /* 置广播标志 */
        commu_cmd_stop_all_mac_interval = 0;
    }
}
/*****************************************************
    功能： 厂家参数初始化命令
        说明： 该命令属于广播命令
               10秒才发送一次，防止连续的发送影响其它板的通讯
******************************************************/
void cmd_fac_para_init(void)
{
    if (commu_cmd_fac_para_init_interval > 10)
    {
        cell[MAX_PCB].cell_cmd[MAX_COMP] |= CMD_FAC_PARA_INIT;   /* 向从机发送厂家参数初始化命令 */
        //      cell[0].cell_cmd[MAX_COMP] |= CMD_FAC_PARA_INIT;        /* 向从机发送厂家参数初始化命令 */
        cmd_broadcast = 1;        /* 置广播标志 */
        //      debug_7[0] = key_fac_para_init;
        key_fac_para_init = FALSE;
        //      para_init_all();               /* 主机参数初始化 */
        commu_cmd_fac_para_init_interval = 0;
    }
}

/*******************************************************************************
 函 数: cmd_invt_init()
 功 能: 变频器初始化命令
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void cmd_invt_init(void)
{
    if (commu_cmd_invt_init_interval > 10)
    {
        cell[MAX_PCB].cell_cmd[MAX_COMP] |= CMD_INVT_INIT;   /* 向从机发送变频器初始化命令 */
        cmd_broadcast = 1;        /* 置广播标志 */
        key_fac_invt_init = FALSE;
        commu_cmd_invt_init_interval = 0;
    }
}

/*****************************************************
    功能： 系统维护参数初始化命令
        说明： 该命令属于广播命令
               10秒才发送一次，防止连续的发送影响其它板的通讯
******************************************************/
void cmd_limit_init(void)
{
    if (commu_cmd_limit_para_init_interval > 10)
    {
        cell[MAX_PCB].cell_cmd[MAX_COMP] |= CMD_LIMIT_INIT;   /* 向从机发送厂家参数初始化命令 */
        //      cell[0].cell_cmd[MAX_COMP] |= CMD_LIMIT_INIT;       /* 向从机发送厂家参数初始化命令 */
        cmd_broadcast = 1;        /* 置广播标志 */
        key_fac_limit_init = FALSE;
        //      para_init_all();               /* 主机参数初始化 */
        commu_cmd_limit_para_init_interval = 0;
    }
}


/*****************************************************
    功能： 压机累计运行时间初始化命令
        说明： 该命令属于广播命令
               10秒才发送一次，防止连续的发送影响其它板的通讯
******************************************************/
void cmd_runtime_init(void)
{
    if (commu_cmd_runtime_init_interval > 10)
    { 
        cell[MAX_PCB].cell_cmd[MAX_COMP] |= CMD_RUNTIME_INIT;   /* 向从机发送运行时间初始化命令 */
        //      cell[0].cell_cmd[MAX_COMP] |= CMD_RUNTIME_INIT;   /* 向从机发送运行时间初始化命令 */
        cmd_broadcast = 1;        /* 置广播标志 */
        key_runtime_init = FALSE;
        commu_cmd_runtime_init_interval = 0;
    }
}
/*****************************************************
    功能： 系统累计运行时间初始化命令
        说明： 该命令属于广播命令
               10秒才发送一次，防止连续的发送影响其它板的通讯
******************************************************/
void cmd_systime_init(void)
{
    if (commu_cmd_systime_init_interval > 10)
    {
        cell[MAX_PCB].cell_cmd[MAX_COMP] |= CMD_SYSTIME_INIT;   /* 向从机发送运行时间初始化命令 */
        //        cell[0].cell_cmd[MAX_COMP] |= CMD_SYSTIME_INIT;   /* 向从机发送运行时间初始化命令 */
        cmd_broadcast = 1;        /* 置广播标志 */
        key_systime_init = FALSE;
        commu_cmd_systime_init_interval = 0;
    }
}

/*****************************************************
    功能： 密码初始化命令
        说明： 该命令属于广播命令
               10秒才发送一次，防止连续的发送影响其它板的通讯
******************************************************/
void cmd_password_init(void)
{
    if (commu_cmd_password_save_interval > 10)
    {
        cell[MAX_PCB].cell_cmd[MAX_COMP] |= CMD_PASSWORD_INIT;   /* 向从机发送运行时间初始化命令 */
        //      cell[0].cell_cmd[MAX_COMP] |= CMD_PASSWORD_INIT;        /* 向从机发送运行时间初始化命令 */
        cmd_broadcast = 1;        /* 置广播标志 */
        key_password_init = FALSE;
        commu_cmd_password_save_interval = 0;
    }
}

/*****************************************************
    功能： 保存系统累计运行时间命令
        说明： 该命令属于广播命令
               10秒才发送一次，防止连续的发送影响其它板的通讯
               广播给每个模块保存
******************************************************/
void cmd_systime_save(void)
{
    if (commu_cmd_systime_save_interval > 10)
    {
        cell[MAX_PCB].cell_cmd[MAX_COMP] |= CMD_SYSTIME_SAVE;   /* 向从机发送运行时间初始化命令 */
        //      cell[0].cell_cmd[MAX_COMP] |= CMD_SYSTIME_SAVE;   /* 向从机发送运行时间初始化命令 */
        cmd_broadcast = 1;        /* 置广播标志 */
        commu_cmd_systime_save_interval = 0;
    }
}

/*******************************************************************************
 函 数: cmd_cell_open()
 功 能: 模块开启命令
 参 数: pcb_num--; 
		to_mode--; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void cmd_cell_open(U08 pcb_num, U08 to_mode)
{
    if (commu_cmd_cell_open[pcb_num] > 3)
    {
        cell[pcb_num].cell_cmd[MAX_COMP] |= ((to_mode&0x07)<<CMD_MODE);	/* 运行模式 */
        cell[pcb_num].cell_cmd[MAX_COMP] |= CMD_OPEN_CELL;
        SET_BIT(cmd_modify, pcb_num);			/* 命令修改标识 */
        commu_cmd_cell_open[pcb_num] = 0;
    }
}

/*******************************************************************************
 函 数: cmd_cell_close()
 功 能: 模块关闭命令
 参 数: pcb_num--; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void cmd_cell_close(U08 pcb_num)
{
    if (commu_cmd_cell_close[pcb_num] > 3)
    {
        cell[pcb_num].cell_cmd[MAX_COMP] |= CMD_CLOSE_CELL;
        SET_BIT(cmd_modify, pcb_num);			/* 命令修改标识 */
        commu_cmd_cell_close[pcb_num] = 0;
    }
}

#if 0
/*******************************************************************************
 函 数: log_sys_is_silent()
 功 能: 判断是否为静音模式
 参 数: 空
 返 回: FALSE、TRUE
 其 它: 空
*******************************************************************************/
BOL log_sys_is_silent(void)
{
    /* 主模块更新，从模块仅获取 */
    if (pcb_type == TYPE_MASTER)
    {
        BOL fg_silent = FALSE;
        
        switch (silent_mode_level)
        {
            default:
            case SILENT_UNUSED:
                fg_silent = FALSE;
                break;
            case SILENT_LEVEL_1:
            case SILENT_LEVEL_2:
                fg_silent = TRUE;
                break;
#if 0
            case SILENT_NIGHT:
                {
                    RTC *clk = &clock[clk_base];
                
                    if (clk->year != 0)
                    {
                        I16 bgn_time = val_night_bgn_h*60 + val_night_bgn_m;    /* 转换为分 */
                        I16 end_time = val_night_end_h*60 + val_night_end_m;    /* 转换为分 */
                        I16 cur_time = clk->hour*60 + clk->min;         /* 转换为分 */

                        if (end_time > bgn_time)
                        {
                            if (cur_time >= bgn_time && cur_time < end_time)
                            {
                                fg_silent = TRUE;
                            }
                        }
                        else if (end_time < bgn_time) /* 跨天 */
                        {
                            if (cur_time >= bgn_time || cur_time < end_time)
                            {
                                fg_silent = TRUE;
                            }
                        }
                    }
                }
                break;
#endif
        }

        fg_silent_mode = fg_silent;
    }

    return fg_silent_mode;
}
#endif

/*******************************************************************************
 函 数: log_sys_defrost_comp_num()
 功 能: 获取子系统中的除霜压机数
 参 数: type--; 
 返 回: 除霜压机数
 其 它: 空
*******************************************************************************/
U08 log_sys_defrost_comp_num(U08 type)
{
    U08 i,j;
    U08 type_all;
    type_all = 0;

    for (i = PCB_BGN(type); i < PCB_END(type); i++)
    {
        for (j = 0; j < m_val_comp_num(i); j++)
        {
            if (M_CM_GetStatus(i, j, COMP_DEFROST))
            {
                type_all++;
            }
        }
    }

    return type_all;
}

/*******************************************************************************
 函 数: log_sys_merge_mode()
 功 能: 当前模式转换为压机模式，压机模式只有制冷、制热
 参 数: sys_mode--; 
 返 回: 
 其 它: 空
*******************************************************************************/
U08 log_sys_merge_mode(U08 sys_mode)
{
    U08 comp_mode = MODE_NULL;

    switch (sys_mode)
    {
        /* 冷方向 */
        case MODE_COOL:  // 制冷
            comp_mode = MODE_COOL;
            break;

        /* 热方向 */
        case MODE_HEAT:  // 制热
        case MODE_HOTW:  // 热水
            comp_mode = MODE_HEAT;
            break;
    }

    return comp_mode;
}

/************************************************************
函数： log_sys_cmd_deal
功能： 各种命令集合处理
参数： 无
返回值：无 
*************************************************************/
void log_sys_cmd_deal(U08 pcb_num)
{
    U08 i;
    U08 fg_system_ready;
    U08 sub_sys_type;
    BOL fg_cell_on = FALSE;
    BOL fg_cell_off = FALSE;
    
    fg_system_ready = log_sys_is_rdy();  /* 一定要在“if”条件外面调用该函数, 否则会判断失误 */

    /* 确定当前模块所在子系统 */
    sub_sys_type = GET_SUB_SYS(pcb_num);

    /* 计算 空调泵开启标记 和 模块开启/关闭标志 */
    if (fg_pump_on[sub_sys_type])
    {
        if (!M_CM_GetStatus(pcb_num, MAX_COMP, CELL_PUMP_OPEN))
        {
            if (log_sys_cell_pump_can_open(pcb_num))        /* 模块水泵可开启 */
            {
                fg_cell_on = TRUE;
            }     
        } 
    }
    else
    {
        if (M_CM_GetStatus(pcb_num, MAX_COMP, CELL_PUMP_OPEN))
        {
            if (log_sys_cell_pump_can_close(pcb_num))       /* 模块水泵可关闭 */
            {
                fg_cell_off = TRUE;
            }
        }
    }
    
    /* 选择压缩机后不立即转换或开机是为了等待水泵等系统条件的满足 */
    for(i=0; i<m_val_comp_num(pcb_num); i++)
    {
        /* 压机开启 */
        if (GET_BIT(fg_need_comp_to_run[i], pcb_num))        
        {
            if (fg_system_ready && !STEP_IS_ALARM)
            {
                cmd_comp_run(pcb_num, i, log_sys_merge_mode(m_run_mode_need[pcb_num][i]));
                CLR_BIT(fg_need_comp_to_run[i], pcb_num);   /* 发送命令后清需求标志 */    

                if (sub_sys_type == SUB_SYS_AC_HOTW)
                {
                    m_ctrl_time = 0;
                }
                else if (sub_sys_type == SUB_SYS_AC_ONLY)
                {
                    s_ctrl_time = 0;
                }
            }
        }

        /* 模块关闭 */
        if (fg_cell_off)
        {
            cmd_cell_close(pcb_num);
        }
        else
        {
            /* 模块开启 */
            if (fg_cell_on)
            {
                if (fg_system_ready && !STEP_IS_ALARM)
                {
                    cmd_cell_open(pcb_num, log_sys_merge_mode(m_run_mode_need[pcb_num][i]));
                }
            }
        }
    }
    
    /* 模式改变单播 */
    if (!log_sys_comp_run_num(sub_sys_type))        /* 所有压机停机后才广播模式，以维持停机时序 */
    {
        if (ctrl_mode_bak[sub_sys_type] != ctrl_mode[sub_sys_type])
        {
            for (i = PCB_BGN(sub_sys_type); i < PCB_END(sub_sys_type); i++)
            {
                cmd_comp_mode(i, log_sys_merge_mode(ctrl_mode[sub_sys_type]));/* 注意: 压机模式只有制冷或制热，切记转换 */
            }
            
            ctrl_mode_bak[sub_sys_type] = ctrl_mode[sub_sys_type];              /* 命令发送成功后，清模式发送需求 */
        }
    }
}

/*******************************************************************************
 函 数: log_sys_get_cmd_mux_bit()
 功 能: 获取一次性命令标志，使用后标志清零
 参 数: id--命令标志ID; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 无
*******************************************************************************/
BOL log_sys_get_cmd_mux_bit(U08 id)
{
    BOL result;
    
    if (id >= CMD_MAX) 
    {
        result = FALSE;		/* 超出最大 */
    }
    else
    {
        result = GETBIT_N(cmd_mux, id) ? TRUE : FALSE;
        CLRBIT_N(cmd_mux, id);
    }
    
    return result;
}

/*******************************************
函数： log_sys_cmd_get()
功能： 用户命令读取(线控开关、手操器、上位机等)
参数： pcb_num
返回值：　无
********************************************/
void log_sys_cmd_get(U08 pcb_num)
{
    U08 i;
    
    if (cmd_normal[0] & KEY_CMD_RUN)            /* 开机 */
    {
        commu_run = TRUE;
    }
    if (cmd_normal[0] & KEY_CMD_STOP)           /* 关机 */
    {
        commu_stop = TRUE;
    }
    if (cmd_normal[0] & KEY_CMD_RESET)          /* 复位 */
    {
        key_reset = TRUE;
    }
    if (cmd_normal[0] & KEY_CMD_WARM_CANCEL)    /* 取消预热 */
    {
        key_warm_cancel = TRUE;
    }
    if (cmd_normal[0] & KEY_CMD_SOUND_CANCEL)   /* 消音 */
    {
        key_sound_cancel = TRUE;
    }
//    if (cmd_normal[0] & KEY_CMD_ENTER_FLOOR_DRYUP)   /* 进入地暖烘干功能 */
//    {
//        key_enter_floor_dryup = TRUE;
//    }
//    if (cmd_normal[0] & KEY_CMD_FORCE_IBH)   /* 强制开启IBH */
//    {
//        key_IBH_force_on = TRUE;
//    }
//    if (cmd_normal[0] & KEY_CMD_FORCE_TBH)   /* 强制开启TBH */
//    {
//        key_TBH_force_on = TRUE;
//    }
//    if (cmd_normal[0] & KEY_CMD_FAST_HOTW)   /* 强制制热水(快速制热水) */
//    {
//        key_fast_hotw_on = TRUE;
//    }
    
    cmd_normal[0] = 0;                       /* 命令清零 */

    if (GET_BIT(cmd_normal[1], pcb_num))       /* 强制除霜 */
    {
        SETCMD_N(ABILITY_TEST_OFF);			/* 手动除霜退出能力测试 */
        for (i=0; i<m_val_comp_num(pcb_num); i++)
        {
            cmd_comp_force_defrost(pcb_num, i);
        }
    }
    CLR_BIT(cmd_normal[1], pcb_num);           /* 命令清零 */
    
    if (cmd_system[0] & KEY_CMD_USER_PARA_INIT)       /* 用户参数初始化 */
    {
        key_user_para_init = TRUE;
    }
    if (cmd_system[0] & KEY_CMD_FAC_PARA_INIT)       /* 厂家参数初始化 */
    {
        key_fac_para_init = TRUE;
    }
    if (cmd_system[0] & KEY_CMD_LIMIT_INIT)         /* 系统维护参数初始化 */
    {
        key_fac_limit_init = TRUE;
    }
    if (cmd_system[0] & KEY_CMD_FAC_INVT_INIT)       /* 变频器初始化 */
    {
        key_fac_invt_init = TRUE;
    }
//    if (cmd_system[0] & KEY_CMD_FAC_ABILITY_TEST)       /* 进入能力测试 */
//    {
//        key_fac_enter_ability_test = TRUE;
//    }
    if (cmd_system[0] & KEY_CMD_RUNTIME_INIT)       /* 累计运行时间初始化 */
    {
        key_runtime_init = TRUE;
    }
    if (cmd_system[0] & KEY_CMD_SYSTIME_INIT)       /* 系统累计运行时间初始化 */
    {
        key_systime_init = TRUE;
    }
    if (cmd_system[0] & KEY_CMD_PASSWORD_INIT)      // 密码初始化
    {
        key_password_init = TRUE;
    }
    if (cmd_system[0] & KEY_CMD_ALLDATA_INIT)       // 全部数据初始化
    {
        key_alldata_init = TRUE;
    }

    
    cmd_system[0] = 0;                            /* 命令清零 */
    
    /* 特殊命令 */
//  if (cmd_system[1] & KEY_CMD_SYS_LIMIT)          /* 使用期限到 */
//  {
//      key_sys_limit = TRUE;
//  }
    
//  cmd_system[1] = 0;                            /* 命令清零 */


    /* A区开机 */
    if (GETCMD_N(ZONE_A_ON))		
    {
        fg_zone_A_on = TRUE;
    }
    /* A区关机 */
    if (GETCMD_N(ZONE_A_OFF))		
    {
        fg_zone_A_off = TRUE;
    }
    
    if (USE_DUAL_ZONE_CTRL)
    {
        /* B区开机 */
        if (GETCMD_N(ZONE_B_ON))     
        {
            fg_zone_B_on = TRUE;
        }
        /* B区关机 */
        if (GETCMD_N(ZONE_B_OFF))		
        {
            fg_zone_B_off = TRUE;
        }
    }
    
    /* 热水开机 */
    if (GETCMD_N(HOTW_ON))			
    {
        fg_hotw_on = TRUE;
    }
    /* 热水关机 */
    if (GETCMD_N(HOTW_OFF))		
    {
        fg_hotw_off = TRUE;
    }

    /* 开启: 定时下回水泵 */
    if (GETCMD_N(TIMED_PUMPRET_ON))
    {
        fg_timed_PUMPret_run = TRUE;
    }
    /* 关闭: 定时下回水泵 */
    if (GETCMD_N(TIMED_PUMPRET_OFF))
    {
        fg_timed_PUMPret_run = FALSE;
    }

    /* 开启: 强开热水模式 */
    if (GETCMD_N(FORCE_HOTW_ON))
    {
        /* 强开热水模式可进入 */
        if (log_sys_can_enter_force_hotw())
        {
            fg_force_hotw_mode_run = TRUE;
            /* 热水未运行，则热水开机 */
            if (!fg_hotw_run)
            {
                fg_hotw_on = TRUE;
            }
        }
    }
    /* 关闭: 强开热水模式 */
    if (GETCMD_N(FORCE_HOTW_OFF))
    {
        fg_force_hotw_mode_run = FALSE;
    }

    /* 开启: 下回水泵杀菌 */
    if (GETCMD_N(PUMPRET_DISINFECT_ON))
    {
        fg_PUMPret_disinfect_run = TRUE;
    }
    /* 关闭: 下回水泵杀菌 */
    if (GETCMD_N(PUMPRET_DISINFECT_OFF))
    {
        fg_PUMPret_disinfect_run = FALSE;
    }

    /* 开启: 假日离开模式 */
    if (GETCMD_N(HOLIDAY_AWAY_ON))
    {
        if (fg_holiday_home_run == FALSE	/* 未进入假日在家模式 */
        && fg_timed_disinfect_run == FALSE	/* 未进入定时杀菌功能 */
        )
        {
            fg_holiday_away_run = TRUE;
            /* A区未运行，则A区开机 */
            if (!fg_zone_A_run
            && fg_holiday_away_heat_mode_used
            )
            {
                fg_zone_A_on = TRUE;
            }
            /* B区未运行，则B区开机 */
            if (!fg_zone_B_run
            && fg_holiday_away_heat_mode_used
            )
            {
                fg_zone_B_on = TRUE;
            }
            /* 热水未运行，则热水开机 */
            if (!fg_hotw_run
            && fg_holiday_away_hotw_mode_used
            )
            {
                fg_hotw_on = TRUE;
            }
        }
    }
    /* 关闭: 假日离开模式 */
    if (GETCMD_N(HOLIDAY_AWAY_OFF))
    {
        fg_holiday_away_run = FALSE;
    }
    
    /* 开启: 假日在家模式 */
    if (GETCMD_N(HOLIDAY_HOME_ON))
    {
        if (fg_holiday_away_run == FALSE	/* 未进入假日离开模式 */
        && fg_timed_disinfect_run == FALSE	/* 未进入定时杀菌功能 */
        )
        {
            fg_holiday_home_run= TRUE;
        }
    }
    /* 关闭: 假日在家模式 */
    if (GETCMD_N(HOLIDAY_HOME_OFF))
    {
        fg_holiday_home_run = FALSE;
    }

    /* 使用: 假日离家制热模式 */
    if (GETCMD_N(HOLIDAY_AWAY_HEAT_ON))
    {
        fg_holiday_away_heat_mode_used = TRUE;

        /* 制热模式使用，【控制模式】设定为"制热模式" */
        para_set(P_GaI(sv_mode), SET_HEAT);
    }
    /* 禁用: 假日离家制热模式 */
    if (GETCMD_N(HOLIDAY_AWAY_HEAT_OFF))
    {
        fg_holiday_away_heat_mode_used = FALSE;
    }

    /* 使用: 假日离家热水模式 */
    if (GETCMD_N(HOLIDAY_AWAY_HOTW_ON))
    {
        fg_holiday_away_hotw_mode_used = TRUE;
    }
    /* 禁用: 假日离家热水模式 */
    if (GETCMD_N(HOLIDAY_AWAY_HOTW_OFF))
    {
        fg_holiday_away_hotw_mode_used = FALSE;
    }
    
    /* 开启: 强开水箱电热 */
    if (GETCMD_N(FORCE_TBH_ON))
    {
        fg_force_TBH_run = TRUE;
    }
    /* 关闭: 强开水箱电热 */
    if (GETCMD_N(FORCE_TBH_OFF))
    {
        fg_force_TBH_run = FALSE;
    }

    /* 开启: 强开辅助电热 */
    if (GETCMD_N(FORCE_IBH_ON))
    {
        fg_force_IBH_run = TRUE;
    }
    /* 关闭: 强开辅助电热 */
    if (GETCMD_N(FORCE_IBH_OFF))
    {
        fg_force_IBH_run = FALSE;
    }

    /* 开启: 强开外部热源 */
    if (GETCMD_N(FORCE_AHS_ON))
    {
        fg_force_AHS_run = TRUE;
    }
    /* 关闭: 强开外部热源 */
    if (GETCMD_N(FORCE_AHS_OFF))
    {
        fg_force_AHS_run = FALSE;
    }

    /* 免费用电强制热水开机 */
    if (GETCMD_N(FREE_ELEC_FORCE_HOTW_ON))
    {
        /* 热水未运行，则热水开机 */
        if (!fg_hotw_run)
        {
            fg_hotw_on = TRUE;
        }
    }

    /* 高峰用电热水关机 */
    if (GETCMD_N(PEAK_ELEC_HOTW_OFF))
    {
        if (fg_hotw_run)
        {
            fg_hotw_off = TRUE;
        }
    }

    /* 高峰用电空调关机 */
    if (GETCMD_N(PEAK_ELEC_AC_OFF))
    {
        if (fg_zone_A_run)
        {
            fg_zone_A_off = TRUE;
        }
        if (fg_zone_B_run)
        {
            fg_zone_B_off = TRUE;
        }
    }  

    /* 低谷用电空调关机 */
    if (GETCMD_N(VALLEY_ELEC_AC_OFF))
    {
        if (fg_zone_A_run)
        {
            fg_zone_A_off = TRUE;
        }
        if (fg_zone_B_run)
        {
            fg_zone_B_off = TRUE;
        }
    }

    /* 低谷用电热水关机 */
    if (GETCMD_N(VALLEY_ELEC_HOTW_OFF))
    {
        if (fg_hotw_run)
        {
            fg_hotw_off = TRUE;
        }
    }

    /* 收氟 */
    if (GETCMD_N(REFRI_RECY_ON))
    {
        cmd_refri_recy_enter = TRUE;
    }
    if (GETCMD_N(REFRI_RECY_OFF))
    {
        cmd_refri_recy_stop = TRUE;
    }

    /* 静音模式 */
    if (GETCMD_N(SILENT_MODE_ON))
    {
        fg_silent_mode = TRUE;
    }
    if (GETCMD_N(SILENT_MODE_OFF))
    {
        fg_silent_mode = FALSE;
    }
    if (GETCMD_N(SILENT_MODE_L1))
    {
        silent_mode_level = SILENT_LEVEL_1;
    }
    if (GETCMD_N(SILENT_MODE_L2))
    {
        silent_mode_level = SILENT_LEVEL_2;
    }

    /* 手动调试 */
    if (GETCMD_N(TEST_DO_ON))
    {
        test_data.wo.DO_mnl_mode = TRUE;
    }
    if (GETCMD_N(TEST_DO_OFF))
    {
        test_data.wo.DO_mnl_mode = FALSE;
    }

    /* 预热 */
    if (GETCMD_N(WARM_CMD_CANCEL))
    {
        key_warm_cancel = TRUE;
    }
    if (GETCMD_N(WARM_CMD_QUICK))
    {
        fg_warm_quick = TRUE;
    }
}
/********************************************************
    函数： log_sys_cell_can_run
    功能： 判断模块是否可用
    参数： 模块号
    返回值： 模块状态  1: 可用; 0: 不可用
*********************************************************/
BOOL log_sys_cell_can_run(U08 pcb_num)
{
    if (CELL_Is_used(pcb_num)==0)               return FALSE;
    
    if (err_sys_get_s_commu_err(pcb_num))       return FALSE;

    if (m_get_error(pcb_num, S_ERR_EEPROM))     return FALSE;

    return TRUE;
}

/*******************************************************************************
 函 数: log_sys_cell_pump_can_open()
 功 能: 模块水泵可开启
 参 数: pcb_num--; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL log_sys_cell_pump_can_open(U08 pcb_num)
{
    if (CELL_Is_used(pcb_num)==0)               return FALSE;
    
    if (err_sys_get_s_commu_err(pcb_num))       return FALSE;

    if (m_get_error(pcb_num, S_ERR_EEPROM))     return FALSE;

    if (m_get_error(pcb_num, S_ERR_WATER_FLOW)) return FALSE;

    if (m_get_error(pcb_num, S_ERR_PUMPF))      return FALSE;

    if (m_get_error(pcb_num, S_ERR_MAC_ERR))    return FALSE;

    return TRUE;
}

/*******************************************************************************
 函 数: log_sys_cell_comp_run()
 功 能: 判断模块是否有压机运行
 参 数: pcb_num--; 
 返 回: 
 其 它: 空
*******************************************************************************/
BOL log_sys_cell_comp_run(U08 pcb_num)
{
    U08 comp_num;
    BOL fg_cell_comp_run = FALSE;

    for (comp_num = 0; comp_num < m_val_comp_num(pcb_num); comp_num++)
    {
        if (M_CM_GetStatus(pcb_num, comp_num, COMP_RUN_NEED)
        || M_CM_GetStatus(pcb_num, comp_num, COMP_RUN))
        {
            fg_cell_comp_run = TRUE;
        }
    }
    
    return fg_cell_comp_run;
}

/*******************************************************************************
 函 数: log_sys_cell_pump_can_close()
 功 能: 模块水泵可关闭
 参 数: pcb_num--; 
 返 回: 
 其 它: 空
*******************************************************************************/
BOL log_sys_cell_pump_can_close(U08 pcb_num)
{
    if (log_sys_cell_comp_run(pcb_num))             return FALSE;

    return TRUE;
}

/*******************************************************************************
 函 数: log_sys_cell_forbid_deal()
 功 能: 禁用模块的处理:模块禁用后，发现模块有运行，间隔3S关一台压机
                       模块禁用后，清故障
                       模块禁用后，清状态
 参 数: pcb_num--模块序号; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_cell_forbid_deal(U08 pcb_num)
{
    U08 i, j;
    static U08 comp_close[MAX_PCB];
    
    if (CELL_Is_used(pcb_num)==0)
    {
        if (comp_close[pcb_num] >= m_val_comp_num(pcb_num))
        {
            comp_close[pcb_num] = 0;
        }
        
        if (M_CM_GetStatus(pcb_num, MAX_COMP, CELL_RUN))
        {
            for (; comp_close[pcb_num]<m_val_comp_num(pcb_num); comp_close[pcb_num]++)
            {
                if (delay_time > 3)
                {
                    if (M_CM_GetStatus(pcb_num, comp_close[pcb_num], COMP_RUN))
                    {
                        delay_time = 0;
                        cmd_comp_system_stop(pcb_num, comp_close[pcb_num]);
                        break;
                    }
                }
            }
        }
    }

    /* 禁用模块处理 */
    for (i=0; i<val_pcb_num; i++)
    {
        if (CELL_Is_used(i)==0)
        {
            for (j=0; j<MAX_ERR_WORD; j++)
            {
                cell[i].cell_error[j] = 0;              /* 模块禁用，清故障 */
            }
            if (!M_CM_GetStatus(i, MAX_COMP, CELL_RUN))   /* 模块停止运行 */
            {
                for (j=0; j<MAX_STATUS; j++)                /* 模块禁用，停机完毕后清状态 */
                {
                    cell[i].cell_status[j] = 0;
					cell[i].cell_relay[j] = 0;
                }
            }
            CLRBIT(slave_commu_error,i);                // 除了清故障，还要清标志 
        }
    }

    /* 多余模块处理 */
    for (i=val_pcb_num; i<MAX_PCB; i++)
    {
        for (j=0; j<MAX_ERR_WORD; j++)
        {
            cell[i].cell_error[j] = 0;              /* 无此模块 ，清故障 */
        } 
        for (j=0; j<MAX_STATUS; j++)            /* 无此模块，清状态 */
        {
            cell[i].cell_status[j] = 0;
            cell[i].cell_relay[j] = 0;
        } 
        CLRBIT(slave_commu_error,i);                // 除了清故障，还要清标志 
    }
}

/*******************************************************************************
 函 数: log_sys_comp_mode()
 功 能: 判断压机可使用模式
 参 数: pcb_num--; 
		comp_num--; 
		mode--; 
		type--; 
 返 回: 
 其 它: 空
*******************************************************************************/
BOL log_sys_comp_mode(U08 pcb_num, U08 comp_num, U08 mode, U08 type)
{
    BOL fg_mode_can_use = FALSE;
    
    if (log_sys_TA_limit_comp_is_run(mode, type))
    {
        switch(mode)
        {
            case MODE_COOL:
                if (cell[pcb_num].cell_status[comp_num] & C_MODE_COOL)
                {
                    fg_mode_can_use = TRUE;
                }
                break;
            case MODE_HOTW:
            case MODE_HEAT:
                if (cell[pcb_num].cell_status[comp_num] & C_MODE_HEAT)
                {
                    fg_mode_can_use = TRUE;
                }
                break;
    
        }
    }
    
    return fg_mode_can_use;
}

#if 0
/***************************************************
    log_sys_have_comp_mode_canuse
    功能：判断此模式下是否有压机可以开启
    参数：
    返回值：TRUE: 有压机可以运行; FALSE: 无压机可以运行
    注意：子模块有“压机故障时清除可用模式”的处理
****************************************************/
BOOL log_sys_have_comp_mode_canuse(U16 mode)
{
    U08 i,j ;
    BOOL fg_have_comp_canuse = FALSE;

    for (i=0; i<val_pcb_num; i++)
    {
        if (!log_sys_cell_can_run(i)) /* 不可用(禁用、通讯故障) */
        {
            continue;        
        }
        for (j=0; j<m_val_comp_num(i); j++)
        {
            if (get_comp_err(j, i)) 
            {
                continue;
            }
            if (log_sys_comp_mode(i, j, mode)
            &&  log_sys_comp_use_status(i, j, COMP_CAN_OPEN)
            )
            {
                fg_have_comp_canuse = TRUE;
                return fg_have_comp_canuse;     
            }
        }
    }

    return fg_have_comp_canuse;
}
#endif

/*****************************************************
函数： log_sys_comp_use_status
功能： 压机当前可使用状态
       可开、可关、可急停、可模式转换
参数： 模块号、压机号、查询属性
返回值： 压机可使用状态
******************************************************/
BOOL log_sys_comp_use_status(U08 pcb_num, U08 comp_num, U16 attrib)
{
    return (cell[pcb_num].cell_status[comp_num] & (attrib)) ? TRUE : FALSE; 
}

/*********************************************
    函数：stop_comp_system
    功能：停压机系统
          调用一次停一个压缩机系统，按顺序停，不考虑其他停机限制条件。
    参数：无
    返回值：无
**********************************************/
void log_sys_select_comp_stop(U08 run_type)
{
    static U08 i=0, j=0;    /* i, j有记忆功能，是为了遍历所有的压机，以防止同一台压机被连续选择 */
    U08 k;
    
    if (i >= val_pcb_num)
    {
        i = 0;
    }

    for (; i<val_pcb_num; i++)
    {
        for (; j<m_val_comp_num(i);)
        {
            k = j++;
            
            if (run_type == MODE_NULL                       /* 不限制模式 */
                || run_type == M_GET_CompMode(i, k))        /* 模式匹配 */
            {
                cmd_comp_system_stop(i, k);          /* 急停压缩机 */
                
                if (M_CM_GetStatus(i, k, COMP_RUN))
                {
                    

                    return;                              /* 退出循环 */
                }
            }
        }
        
        if (j >= m_val_comp_num(i))
        {
            j = 0;
        }
    }   
}

/*******************************************************************************
 函 数: log_sys_select_comp_stop_once()
 功 能: 根据模块类型(主/从)，一次性停所有压机
 参 数: mode--模式; 
		type--模块类型(主/从); 
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_select_comp_stop_once(U08 mode, U08 type)
{
    U08 i = 0;
    U08 j = 0;

    for (i = PCB_BGN(type); i < PCB_END(type); i++)
    {
        for (j = 0; j < m_val_comp_num(i); j++)
        {
            if (mode == MODE_NULL           /* 不限制模式 */
            || mode == M_GET_CompMode(i, j) /* 模式匹配 */
            )
            {
                cmd_comp_system_stop(i, j);          /* 急停压缩机 */
            }
        }
    }
}

/*******************************************************************************
 函 数: log_sys_have_comp_run()
 功 能: 根据模块类型(主/从)，判断某种模式下是否有压机运行
 参 数: mode--模式; 
		type--模块类型(主/从); 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL log_sys_have_comp_run(U08 mode, U08 type)
{
    U08 i,j ;
    BOL fg_have_comp_run = FALSE;

    for (i = PCB_BGN(type); i < PCB_END(type); i++)
    {
        for (j=0; j<m_val_comp_num(i); j++)
        {
            if (M_CM_GetStatus(i, j, COMP_RUN)||M_CM_GetStatus(i, j, COMP_RUN_NEED))
            {
                if (mode == MODE_NULL                       /* 不限制模式 */
                || mode == M_GET_CompMode(i, j))            /* 模式匹配 */
                {
                    fg_have_comp_run = TRUE;
                }
            }
        }
    }

    return fg_have_comp_run;
}

/*******************************************************************************
 函 数: log_sys_have_non_defrosting_comp_run()
 功 能: 根据模块子系统类型(空调+热水/单空调)，判断某种模式下是否有非除霜模块的压机运行
 参 数: mode--模式; 
		type--子系统类型(空调+热水/单空调); 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOL log_sys_have_non_defrosting_comp_run(U08 mode, U08 type)
{
    U08 i,j;
    BOL fg_cell_have_comp_run = FALSE;		/* 模块有对应模式压机运行标志 */
    BOL fg_cell_is_defrosting = FALSE;		/* 模块在除霜标志 */
    
    for (i = PCB_BGN(type); i < PCB_END(type); i++)			/* 遍历模块 */
    {
        for (j=0; j<m_val_comp_num(i); j++)	/* 遍历模块的压机 */
        {
            if (M_CM_GetStatus(i, j, COMP_RUN)||M_CM_GetStatus(i, j, COMP_RUN_NEED))
            {
                if (mode == MODE_NULL                       /* 不限制模式 */
                || mode == M_GET_CompMode(i, j))            /* 模式匹配 */
                {
                    /* 模块有对应模式压机运行 */
                    fg_cell_have_comp_run = TRUE;
                }
            }

            if (M_CM_GetStatus(i, j, COMP_DEFROST))			/* i模块有压机除霜 */
            {
                /* 有压机除霜就认为模块在除霜 */
                fg_cell_is_defrosting = TRUE;				
            }
        }

        /* 此模块不在除霜 且 有对应模式压机运行 */
        if (!fg_cell_is_defrosting && fg_cell_have_comp_run)
        {
            return TRUE;
        }

        fg_cell_have_comp_run = FALSE;
        fg_cell_is_defrosting = FALSE;
    }

    return FALSE;
}

/*******************************************************************************
 函 数: log_sys_is_00_comp_err()
 功 能: 00#压机是否故障(模式是否可用)
 参 数: 空
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL log_sys_is_00_comp_err(void)
{
    BOOL fg_is_00_comp_err = FALSE;
    if (!m_comp_use_mode(0, 0, ctrl_mode[SUB_SYS_AC_HOTW]))
    {
        fg_is_00_comp_err = TRUE;
    }
//    if (!log_sys_comp_use_status(0, 0, COMP_CAN_OPEN))
//    {
//        fg_is_00_comp_err = TRUE;
//    }
    return fg_is_00_comp_err;
}

/*******************************************************************************
 函 数: log_sys_is_ac_hotw_comp_err()
 功 能: 热水模块压机是否全部故障(模式是否可用)
 参 数: 空
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOL log_sys_is_all_ac_hotw_comp_err(void)
{
    U08 hotwIndex;			/* 热水模块下标 */
    U08 hotwCompIndex;		/* 热水模块中的压机下标 */
    
    /* 遍历热水模块 */
    for (hotwIndex = PCB_BGN(SUB_SYS_AC_HOTW); hotwIndex < PCB_END(SUB_SYS_AC_HOTW); ++hotwIndex)
    {
        /* 遍历热水模块的压机 */
        for (hotwCompIndex = 0; hotwCompIndex < m_val_comp_num(hotwIndex); ++hotwCompIndex)
        {
            /* 存在一个热水模块的压机模式可用 返回 假 */
            if (m_comp_use_mode(hotwIndex, hotwCompIndex, ctrl_mode[SUB_SYS_AC_HOTW]))
            {
                return FALSE;
            }
        }
    }

    /* 到这里表示 热水模块全部压机模式不可用 */
    return TRUE;
}

/***************************************************
    函数：log_sys_all_comp_defro
    功能：判断系统中是否所有压机均除霜
    参数：无
    返回值：TRUE: 所有压机除霜; FALSE: 有压机不除霜
****************************************************/
BOOL log_sys_all_comp_defro(void)
{
    U08 i,j ;
    BOOL fg_all_defro = FALSE;
    U08 cm_run_num = 0;                 // 运行压机数
    U08 cm_defrost_num = 0;             // 除霜压机数

    for (i=0; i<val_pcb_num; i++)
    {
        for (j=0; j<m_val_comp_num(i); j++)
        {
            if (M_CM_GetStatus(i, j, COMP_DEFROST))
                cm_defrost_num++;

            if (M_CM_GetStatus(i, j, COMP_RUN) || M_CM_GetStatus(i, j, COMP_RUN_NEED))
                cm_run_num++;
        }
    }

    if (cm_defrost_num > 0 
        && cm_run_num > 0 
        && cm_defrost_num == cm_run_num)
    {
        fg_all_defro = TRUE;
    }
    
    return fg_all_defro;
}

/*******************************************************************************
 函 数: log_sys_have_fan_on()
 功 能:
 参 数: 空
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOL log_sys_have_fan_on(void)
{
    BOL result = FALSE;
    
    if (val_pcb_num == 1)
    {
        result = fg_have_fan_on;
    }
    else    /* 多个模块时，由于系统状态没有风机信息，故使用压机信息来近似 */
    {
        result = log_sys_have_comp_run(MODE_NULL, SUB_SYS_TYPE_MAX);
    }
    return result;
}

/*******************************************************************************
 函 数: log_sys_have_comp_status()
 功 能: 根据模块类型(主/从)，判断是否存在某种压机状态
 参 数: sta--压机状态; 
		type--模块类型(主/从); 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL log_sys_have_comp_status(U16 sta, U08 type)
{
    U08 i,j;
    BOOL fg_have_sta = FALSE;

    for (i = PCB_BGN(type); i < PCB_END(type); i++)
    {
        /* 模块不可用 */
        if (!log_sys_cell_can_run(i))   continue;

        for (j=0; j<m_val_comp_num(i); j++)
        {
            /* 压机故障 */
            if (get_comp_err(j, i)) continue;

            if (M_CM_GetStatus(i, j, sta))
            {
                fg_have_sta = TRUE;
            }
        }
    }

    return fg_have_sta;
}

/***************************************************
    log_sys_have_comp_can_run
    功能：判断是否有压机可以开启
    参数：
    返回值：TRUE: 有压机可以运行; FALSE: 无压机可以运行
****************************************************/
BOOL log_sys_have_comp_can_run(void)
{
    return log_sys_have_comp_status(COMP_RUN, SUB_SYS_TYPE_MAX);
}

/*******************************************************************************
 函 数: log_sys_comp_run_num()
 功 能: 统计子组或系统中运行的压机数
 参 数: type--; 
 返 回: 
 其 它: 空
*******************************************************************************/
U08 log_sys_comp_run_num(U08 type)
{
    U08 i, j;
    U08 run_num = 0;
    
    for (i = PCB_BGN(type); i < PCB_END(type); i++)
    {
        for (j=0; j<m_val_comp_num(i); j++)
        {
            if (M_CM_GetStatus(i, j, COMP_RUN)) 
            {
                run_num++;
            }
        }
    }
    
    return run_num;
}

/*******************************************************************************
 函 数: log_sys_have_comp_num()
 功 能: 根据模块类型(主/从)，统计相应的最大压机数
 参 数: type--模块类型(主/从); 
 返 回: 
 其 它: 空
*******************************************************************************/
U08 log_sys_have_comp_num(U08 type)
{
    U08 i;
    U08 comp_max = 0;

    for (i = PCB_BGN(type); i < PCB_END(type); i++)
    {
        if (CELL_Is_used(i))
        {
            comp_max += m_val_comp_num(i);
        }
    }

    return comp_max;
}

/*******************************************************************************
 函 数: log_sys_s3v1_is_hw()
 功 能: 判断s3v1是否处于热水区
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
BOL log_sys_s3v1_is_hw(void)
{
    return fg_S3V1_is_hw;
}

/*******************************************************************************
 函 数: log_sys_have_AHS_on()
 功 能: 判断是否有外部热源开启
 参 数: zone--区域
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOL log_sys_have_AHS_on(U08 zone)
{
    BOL fg_result = FALSE;
    BOL fg_need = FALSE;
    
    /* 在热水区 */
    if ((log_sys_s3v1_is_hw() && zone==ZONE_HOTW)
    ||  (!log_sys_s3v1_is_hw() && zone==ZONE_AC)
    )
    {
        fg_need = TRUE;
    }

    if (fg_need)
    {
        if (get_output(MAX_PCB, M_DO_AHS))
        {
            fg_result = TRUE;
        }
    }    

    return fg_result;
}

/*******************************************************************************
 函 数: log_sys_have_comp_run_in_zone()
 功 能: 按区判断是否有压机开启
 参 数: 空
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOL log_sys_have_comp_run_in_zone(U08 zone)
{ 
    U08 pcb;
    U08 need_type = SUB_SYS_TYPE_MAX;		/* 默认: 取所有模块 */
    BOL fg_result = FALSE;
    BOL fg_none = FALSE;
    U08 cm = 0;
    
    /* 在热水区 */
    if (log_sys_s3v1_is_hw())
    {
        switch (zone)
        {
            case ZONE_AC:
                if (IS_DUAL_SYS)
                {
                    need_type = SUB_SYS_AC_ONLY;
                }
                else
                {
                    fg_none = TRUE;
                }
                break;
            case ZONE_HOTW:
                if (IS_DUAL_SYS)
                {
                    need_type = SUB_SYS_AC_HOTW;
                }
                else
                {
                    need_type = SUB_SYS_TYPE_MAX;
                }
                break;
            default:
                fg_none = TRUE;
                break;
        }
    }
    /* 在空调区 */
    else
    {
        switch (zone)
        {
            case ZONE_AC:
                if (IS_DUAL_SYS)
                {
                    need_type = SUB_SYS_TYPE_MAX;
                }
                else
                {
                    need_type = SUB_SYS_TYPE_MAX;
                }
                break;
            case ZONE_HOTW:
                if (IS_DUAL_SYS)
                {
                    fg_none = TRUE;
                }
                else
                {
                    fg_none = TRUE;
                }
                break;
            default:
                fg_none = TRUE;
                break;
        }
    }

    if (!fg_none)
    {
        for (pcb = PCB_BGN(need_type); pcb < PCB_END(need_type); pcb++)
        {
            if (m_get_output(pcb, cm, SW_CM))
            {
                fg_result = TRUE;
                break;
            }
        }
    }    

    return fg_result;
}

/*******************************************************************************
 函 数: log_sys_ac_have_comp_run()
 功 能: 判断空调是否有压机开启
 参 数: 空
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOL log_sys_ac_have_comp_run(void)
{
    return log_sys_have_comp_run_in_zone(ZONE_AC);
}

/*******************************************************************************
 函 数: log_sys_hotw_have_comp_run()
 功 能: 判断热水是否有压机开启
 参 数: 空
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOL log_sys_hotw_have_comp_run(void)
{
    return log_sys_have_comp_run_in_zone(ZONE_HOTW);
}

/*******************************************************************************
 函 数: log_sys_have_IBH_on()
 功 能: 判断是否有辅助电热开启
 参 数: 空
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOL log_sys_have_IBH_on(U08 zone)
{ 
    U08 i;
    U08 need_type = SUB_SYS_TYPE_MAX;		/* 默认: 取所有模块 */
    BOL fg_have_IBH_on = FALSE;
    BOL fg_none = FALSE;
    
    /* 在热水区 */
    if (log_sys_s3v1_is_hw())
    {
        switch (zone)
        {
            case ZONE_AC:
                if (IS_DUAL_SYS)
                {
                    need_type = SUB_SYS_AC_ONLY;
                }
                else
                {
                    fg_none = TRUE;
                }
                break;
            case ZONE_HOTW:
                if (IS_DUAL_SYS)
                {
                    need_type = SUB_SYS_AC_HOTW;
                }
                else
                {
                    need_type = SUB_SYS_TYPE_MAX;
                }
                break;
            default:
                fg_none = TRUE;
                break;
        }
    }
    /* 在空调区 */
    else
    {
        switch (zone)
        {
            case ZONE_AC:
                if (IS_DUAL_SYS)
                {
                    need_type = SUB_SYS_TYPE_MAX;
                }
                else
                {
                    need_type = SUB_SYS_TYPE_MAX;
                }
                break;
            case ZONE_HOTW:
                if (IS_DUAL_SYS)
                {
                    fg_none = TRUE;
                }
                else
                {
                    fg_none = TRUE;
                }
                break;
            default:
                fg_none = TRUE;
                break;
        }
    }

    if (!fg_none)
    {
        for (i = PCB_BGN(need_type); i < PCB_END(need_type); i++)
        {
            if (m_get_output(i, MAX_COMP, S_DO_IBH1)
            || m_get_output(i, MAX_COMP, S_DO_IBH2))
            {
                fg_have_IBH_on = TRUE;
                break;
            }
        }
    }    

    return fg_have_IBH_on;
}

/*******************************************************************************
 函 数: log_sys_ac_have_IBH_on()
 功 能: 判断空调是否有辅助电热开启
 参 数: 空
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOL log_sys_ac_have_IBH_on(void)
{
    return log_sys_have_IBH_on(ZONE_AC);
}

/*******************************************************************************
 函 数: log_sys_hotw_have_IBH_on()
 功 能: 判断热水是否有辅助电热开启
 参 数: 空
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOL log_sys_hotw_have_IBH_on(void)
{
    return log_sys_have_IBH_on(ZONE_HOTW);
}

/*******************************************************************************
 函 数: log_sys_cell_pumpf_on()
 功 能: 判断是否模块水泵开启
 参 数: 空
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOL log_sys_cell_pumpf_on(void)
{
	U08 i;
	U08 need_type = SUB_SYS_TYPE_MAX;	
	BOL fg_have_pumpf_on = FALSE;

	for (i = PCB_BGN(need_type); i < PCB_END(need_type); i++)
    {
        if (m_get_output(i, MAX_COMP, S_DO_PUMPf))
        {
            fg_have_pumpf_on = TRUE;
            break;
        }
    }

    return fg_have_pumpf_on;
}

/*******************************************************************************
 函 数: log_sys_cell_pump_run()
 功 能: 模块水泵稳定运行
 参 数: pcb_num--; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOL log_sys_cell_pump_run(U08 pcb_num)
{
    BOL fg_is_cell_pump_run = FALSE;

    if (M_CM_GetStatus(pcb_num, MAX_COMP, CELL_PUMP_RUN))
    {
        fg_is_cell_pump_run = TRUE;
    }

    return fg_is_cell_pump_run;
}

/*******************************************************************************
 函 数: log_sys_have_cell_pump_run()
 功 能: 已有模块泵稳定运行
 参 数: type--; 
 返 回: 
 其 它: 空
*******************************************************************************/
BOL log_sys_have_cell_pump_run(U08 type)
{
    U08 i;
    BOL fg_have_cell_pump_run = FALSE;

    for (i = PCB_BGN(type); i < PCB_END(type); i++)
    {
        if (M_CM_GetStatus(i, MAX_COMP, CELL_PUMP_RUN))
        {
            fg_have_cell_pump_run = TRUE;
            break;
        }
    }

    return fg_have_cell_pump_run;
}

/***********************************************
    函数：m_have_cell_run
    功能：判断是否有模块在运行
    参数：无
    返回值：TRUE: 有模块运行; FALSE: 模块停止运行
************************************************/

BOOL log_sys_have_cell_run(void)
{
    U08 i;
    BOOL have_cell_run = FALSE;
    
    for (i=0; i<val_pcb_num; i++)
    {
        if (M_CM_GetStatus(i, MAX_COMP, CELL_RUN))
        {
            have_cell_run = TRUE;
            break;
        }
    }

    return have_cell_run;
}
/***********************************************
    函数：m_cell_is_err
    功能：判断模块是否有故障
    参数：无
    返回值：TRUE: 模块运行; FALSE: 模块停止运行
************************************************/

BOOL log_sys_cell_is_err(void)
{
    U08 i;
    BOOL cell_is_err = FALSE;
    
    for (i=0; i<val_pcb_num; i++)
    {
        if (M_CM_GetStatus(i, MAX_COMP, CELL_ERR))
        {
            cell_is_err = TRUE;
            break;
        }
    }

    return cell_is_err;
}

#define ELC_OFF          0           /* 关闭区 */
#define ELC_RUN          1           /* 运行区 */
#define ELC_KEEP         2           /* 保持区 */
static U08 cnt_sys_comp_limit_TA_dly[SUB_SYS_TYPE_MAX][CTRL_MODE_MAX];
/*******************************************************************************
 函 数: log_sys_env_limit_comp_area()
 功 能: 获取环境温度限制分区
 参 数: mode--模式; 
		type--分组类型; 
 返 回: result--分区
 其 它: 空
*******************************************************************************/
U08 log_sys_env_limit_comp_area(U08 mode, U08 type)
{
    BOL fg_env_limit_used;
    I16 point1,point2,point3,point4; // 从大到小排列
    U08 mode_idx = 0;
    U08 grp_idx = 0;
    U08 area_new = AREA_00;
    static U08 area_cur[SUB_SYS_TYPE_MAX][CTRL_MODE_MAX] = {{AREA_00,AREA_00,AREA_00},{AREA_00,AREA_00,AREA_00}};
    static U08 result[SUB_SYS_TYPE_MAX][CTRL_MODE_MAX] = {{ELC_KEEP,ELC_KEEP,ELC_KEEP},{ELC_KEEP,ELC_KEEP,ELC_KEEP}};

    /* 掉电记忆开机后，ctrl_mode的值更新可能不及时 */
    if (mode == MODE_NULL)
    {
        return COMP_KEEP_AREA;
    } 

    /* 确定模式 */
    switch (mode)
    {
        case MODE_COOL:		/* 制冷 */
            fg_env_limit_used = (val_cool_max_TA == val_cool_min_TA) ? FALSE : TRUE;
            point1 = 10*(COOL_RUN_ENV_HLIMIT);
            point2 = val_cool_max_TA;
            point3 = val_cool_min_TA;
            point4 = 10*(COOL_RUN_ENV_LLIMIT);
            mode_idx = 0;
            break;
            
        case MODE_HEAT:		/* 制热 */
            fg_env_limit_used = (val_heat_max_TA == val_heat_min_TA) ? FALSE : TRUE;
            point1 = 10*(HEAT_RUN_ENV_HLIMIT);
            point2 = val_heat_max_TA;
            point3 = val_heat_min_TA;
            point4 = 10*(HEAT_RUN_ENV_LLIMIT);
            mode_idx = 1;
            break;
            
        case MODE_HOTW:		/* 制热水 */
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
    if (SensorCanUse(pv_sys_TA))
    {
        /* AREA_00 point1 AREA_01 point2 AREA_02 point3 AREA_03 point4 AREA_04 */
        area_new = fun_area(pv_sys_TA, area_cur[grp_idx][mode_idx], 10, point1, point2, point3, point4, SENSOR_NOEXIST);
    }

    if (area_new == area_cur[grp_idx][mode_idx])
    {
        cnt_sys_comp_limit_TA_dly[grp_idx][mode_idx] = 0;
    }

    if (cnt_sys_comp_limit_TA_dly[grp_idx][mode_idx] > 3)
    {
        area_cur[grp_idx][mode_idx] = area_new;
        if (area_cur[grp_idx][mode_idx]==AREA_00 || area_cur[grp_idx][mode_idx]==AREA_04)
        {
            result[grp_idx][mode_idx] = ELC_OFF;
        }
        else if (area_cur[grp_idx][mode_idx]==AREA_02)
        {
            result[grp_idx][mode_idx] = ELC_RUN;
        }
        else
        {
            result[grp_idx][mode_idx] = ELC_KEEP;
        }
    }

    if (!fg_env_limit_used)
    {
        result[grp_idx][mode_idx] = ELC_RUN;
    }
    
    return result[grp_idx][mode_idx];
}

/*******************************************************************************
 函 数: log_sys_TA_limit_comp_is_off()
 功 能: 判断环境温度是否处于关闭区
 参 数: mode--; 
		type--; 
 返 回: TRUE/FALSE--是/否
 其 它: 空
*******************************************************************************/
BOL log_sys_TA_limit_comp_is_off(U08 mode, U08 type)
{
    return (log_sys_env_limit_comp_area(mode, type) == ELC_OFF);
}

/*******************************************************************************
 函 数: log_sys_TA_limit_comp_is_run()
 功 能: 判断环境温度是否处于运行区
 参 数: mode--; 
		type--; 
 返 回: TRUE/FALSE--是/否
 其 它: 空
*******************************************************************************/
BOL log_sys_TA_limit_comp_is_run(U08 mode, U08 type)
{
    return (log_sys_env_limit_comp_area(mode, type) == ELC_RUN);
}

/*******************************************************************************
 函 数: log_sys_HDIsl_limit_comp_is_off()
 功 能: 太阳能制热水时，检测太阳能信号限制压缩机运行
 参 数: 空
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOL log_sys_HDIsl_limit_comp_is_off(void)
{
    static BOL fg_solar_limit = FALSE;

    if (!val_hotw_used                         /* 禁用热水 */
    || !fg_hotw_run                            /* 热水未运行 */   
    || !MODE_IS_HOTW(SUB_SYS_AC_HOTW)          /* 非热水模式 */
    || !val_solar_used                         /* 禁用太阳能 */
    || (val_solar_used&&val_TSL_sensor_used)   /* 使用太阳能，且使用太阳能温度探头 */
    )
    {
        fg_solar_limit = FALSE;
        return fg_solar_limit;
    }

    /* 未检测到太阳能信号，且持续12min */
    if (cnt_HDIsl_off > 12*60)
    {
        fg_solar_limit = FALSE;
    }

    /* 检测到太阳能信号，且持续5s */
    if (cnt_HDIsl_on > 5)
    {
        fg_solar_limit = TRUE;
    }

    return fg_solar_limit;
}

/*******************************************************************************
 函 数: log_sys_TSL_limit_comp_is_off()
 功 能: 太阳能制热水时，根据太阳能温度限制压缩机运行
 参 数: 空
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOL log_sys_TSL_limit_comp_is_off(void)
{
    static BOL fg_solar_limit = FALSE;

    if (!val_hotw_used                          /* 禁用热水 */
    || !fg_hotw_run			                    /* 未运行热水 */
    || !MODE_IS_HOTW(SUB_SYS_AC_HOTW)           /* 非热水模式 */
    || !val_solar_used                          /* 禁用太阳能 */
    || (val_solar_used&&!val_TSL_sensor_used)   /* 使用太阳能，且禁用太阳能温度探头 */
    )
    {
        fg_solar_limit = FALSE;
        return fg_solar_limit;
    }

    /* 太阳能温度TSL ≤【太阳能开启水温】-【太阳能开启回差】，且持续12min */
    if (cnt_TSL_off > 12*60)
    {
        fg_solar_limit = FALSE;
    }

    /* 太阳能温度TSL ＞【太阳能开启水温】，且持续5S */
    if (cnt_TSL_on > 5)
    {
        fg_solar_limit = TRUE;
    }

    return fg_solar_limit;
}


/*******************************************************************************
 函 数: log_sys_floor_preheat_limit_comp_is_off()
 功 能: 地暖预热时限制压缩机运行
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
BOL log_sys_floor_preheat_limit_comp_is_off(void)
{
    static BOL fg_comp_off = FALSE;

    if (!fg_floor_preheat_run)
    {
        fg_comp_off = FALSE;
        return fg_comp_off;
    }
    
    if (floor_preheat_time.on > val_floor_preheat_interval)
    {
        fg_comp_off = TRUE;
    }

    if (floor_preheat_time.off > 5*60)
    {
        fg_comp_off = FALSE;
    }

    return fg_comp_off;
}


/*****************************************************
    函数：get_comp_run_time
    功能：计算压机累计运行时间
    参数：模块序号;压机序号
    返回值：运行时间长: 1; 运行时间短: 2
******************************************************/
U08 log_sys_enegry_prio(U08 x1, U08 y1, U08 x2, U08 y2)
{
    return  (cell[x1].cell_runtime[y1] >= cell[x2].cell_runtime[y2]) ? 1 : 2;
}

/*******************************************************************************
 函 数: log_sys_enegry_sub()
 功 能: 选择可关闭的压机
 参 数: type--type--模块类型(主/从); 
 返 回: 
 其 它: 空
*******************************************************************************/
U08 log_sys_enegry_sub(U08 type)
{
    U08  x = 0;
    U08  y = 0;
    U08  k1 = 0xff;
    U08  k2 = 0xff;
    BOOL Fg_Init = TRUE;
    
    for (x = PCB_BGN(type); x < PCB_END(type); x++)
    {
        for (y = 0; y < m_val_comp_num(x); y++)
        {
            if (log_sys_comp_use_status(x, y, COMP_CAN_CLOSE))       /* 压缩机可关 */
            {
                if (Fg_Init) 
                {   
                    k1 = x;
                    k2 = y; 
                    Fg_Init = FALSE;
                    continue;
                }
                
                if (log_sys_enegry_prio(x, y, k1, k2) == 1)
                {
                    k1 = x; 
                    k2 = y; 
                }
                if (log_sys_comp_use_status(x, y, COMP_SUB_PRIO))   
                {
                    k1 = x; 
                    k2 = y; 
                    x = MAX_PCB;
                    break;
                }
            }
        }
    }
    
    
    if (k1 != 0xff) 
    {
        if (type == SUB_SYS_AC_HOTW)
        {
            m_ctrl_time = 0;
        }
        else if (type == SUB_SYS_AC_ONLY)
        {
            s_ctrl_time = 0;
        }
        else
        {
            m_ctrl_time = 0;
            s_ctrl_time = 0;
        }
        cmd_comp_close(k1, k2);
        return 1;
    }   

    return 0;
}

/*******************************************************************************
 函 数: log_sys_enegry_add()
 功 能: 选择可开启的压机
 参 数: mode--模式; 
		type--模块类型(主/从); 
 返 回: 
 其 它: 空
*******************************************************************************/
U08 log_sys_enegry_add(U08 mode, U08 type)
{
    U08  x = 0;
    U08  y = 0;
    U08  k1 = 0xff;
    U08  k2 = 0xff;
    BOOL Fg_Init = TRUE;
    
    if(STEP_IS_ALARM)
    {
        return 0;
    }
    
    for (x = PCB_BGN(type); x < PCB_END(type); x++)              /* 再加载以其他模式停机的压缩机 */
    {
        if (!log_sys_cell_can_run(x)) /* 不可用(禁用、通讯故障) */
        {
            continue;        
        }   
            
        for (y = 0; y < m_val_comp_num(x); y++)
        {
            if (get_comp_err(y, x)) 
            {
                continue;
            }   

            if (log_sys_comp_use_status(x, y, COMP_CAN_OPEN))       /* 压缩机可开 */
            if (log_sys_comp_mode(x, y, mode, type))                /* 压机需求模式可用 */
            if (fg_warm_complete)                                   /* 预热时间到 */
            {
                if (Fg_Init) 
                {
                    k1 = x; 
                    k2 = y; 
                    Fg_Init = FALSE;
                    continue;
                }
                
                if (log_sys_enegry_prio(x, y, k1, k2) == 2) 
                {
                    k1 = x; 
                    k2 = y; 
                }
                    
                if (log_sys_comp_use_status(x, y, COMP_ADD_PRIO))   
                {
                    k1 = x; 
                    k2 = y; 
                    x = MAX_PCB;
                    break;
                }
                    
            }
        }
    }
    
    if (!Fg_Init) 
    {
        SET_BIT(fg_need_comp_to_run[k2], k1);
        M_CM_SetStatus(k1, k2, COMP_RUN_NEED);     /* 同步标志 */
        m_run_mode_need[k1][k2] = mode;
        return 1;
    }
    
    return 0;
}

/*******************************************************************************
 函 数: log_sys_energy_range()
 功 能: 判断空调/热水温度区域
 参 数: mode--模式; 
        pv--控制温度; 
        sv--目标温度; 
        e1--加载偏差; 
        e2--卸载偏差; 
 返 回: 
 其 它: 空
*******************************************************************************/
U08 log_sys_energy_range(U08 mode, I16 pv, I16 sv, I16 e1, I16 e2)
{
    U08 result = _QUICK;
    
    switch (mode)
    {
        case MODE_COOL: /* 冷方向 */
            if (pv >= (sv + e1))        result = _ADD;  /* 加载区 */
            else if (pv < (sv - e2))    result = _SUBB; /* 卸载区 */
            else                        result = _KEEP; /* 保持区 */
            break;

        case MODE_HOTW:
        case MODE_HEAT: /* 热方向 */
            if (pv <= (sv - e1))        result = _ADD;  /* 加载区 */
            else if (pv > (sv + e2))    result = _SUBB; /* 卸载区 */
            else                        result = _KEEP; /* 保持区 */
            break;
            
        default :
            break;
    }
    
    return result;  /* 急停区 */
}

/*******************************************************************************
 函 数: CountCompRequire()
 功 能: 根据模块类型(主/从)，计算首次开启压机数
 参 数: mode--模式; 
		type--模块类型(主/从); 
 返 回: 
 其 它: 空
*******************************************************************************/
U08 CountCompRequire(U08 mode, U08 type)
{
    U08 comp_required = 0;
    U08 comp_max = log_sys_have_comp_num(type);

    switch (mode)
    {
        case MODE_COOL:		/* 制冷 */
            if (ctrl_temp_energy_ac > sv_temp_energy_cool)
            {
                comp_required =(U08)(((ctrl_temp_energy_ac-(I16)sv_temp_energy_cool)*comp_max)/(I16)val_cm_temp_diff);
            }
            break;

        case MODE_HEAT:		/* 制热 */
            if (ctrl_temp_energy_ac < sv_temp_energy_heat)
            {
                comp_required =(U08)((((I16)sv_temp_energy_heat-ctrl_temp_energy_ac)*comp_max)/(I16)val_cm_temp_diff);
            }
            break;

        case MODE_HOTW:		/* 热水 */
            if (ctrl_temp_energy_hotw < sv_temp_energy_hotw)
            {
                comp_required =(U08)((((I16)sv_temp_energy_hotw-ctrl_temp_energy_hotw)*comp_max)/(I16)val_cm_temp_diff);
            }
            break;
    }

    if (comp_required < 1)
    {
        comp_required = 1;
    }
    else if (comp_required > comp_max)
    {
        comp_required = comp_max;
    }

    return comp_max;
}

/*******************************************************************************
 函 数: log_sys_init_test_run_air_empty()
 功 能: 初始化空气排空试运行变量
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_init_test_run_air_empty(void)
{
    CLRBIT_N(fg_test_run, TEST_RUN_AIR_EMPTY);
    ARRCLR(fg_test_air_empty_on);           /* 全关 */
    test_run_air_empty_stage = TRAE_IDLE;
    if (fg_zone_A_run)
    {
        SETCMD_N(ZONE_A_OFF);
    }
    if (fg_zone_B_run)
    {
        SETCMD_N(ZONE_B_OFF);
    }
    if (fg_hotw_run)
    {
        SETCMD_N(HOTW_OFF);
    }
}

/*******************************************************************************
 函 数: log_sys_init_test_run_pump()
 功 能: 初始化水泵试运行变量
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_init_test_run_pump(void)
{
    CLRBIT_N(fg_test_run, TEST_RUN_PUMP);
    ARRCLR(fg_test_pump_on);
    test_run_pump_stage = TRP_IDLE;
    if (fg_zone_A_run)
    {
        SETCMD_N(ZONE_A_OFF);
    }
    if (fg_zone_B_run)
    {
        SETCMD_N(ZONE_B_OFF);
    }
    if (fg_hotw_run)
    {
        SETCMD_N(HOTW_OFF);
    }
}

/*******************************************************************************
 函 数: log_sys_init_test_run_cool_mode()
 功 能: 初始化制冷试运行变量
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_init_test_run_cool_mode(void)
{
    CLRBIT_N(fg_test_run, TEST_RUN_COOL_MODE);
    if (fg_zone_A_run)
    {
        SETCMD_N(ZONE_A_OFF);
    }
}

/*******************************************************************************
 函 数: log_sys_init_test_run_heat_mode()
 功 能: 初始化制热试运行变量
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_init_test_run_heat_mode(void)
{
    CLRBIT_N(fg_test_run, TEST_RUN_HEAT_MODE);
    fg_test_run_heat_IBH_run = FALSE;
    if (fg_zone_A_run)
    {
        SETCMD_N(ZONE_A_OFF);
    }
    if (fg_zone_B_run)
    {
        SETCMD_N(ZONE_B_OFF);
    }
}

/*******************************************************************************
 函 数: log_sys_init_test_run_hotw_mode()
 功 能: 初始化热水试运行变量
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_init_test_run_hotw_mode(void)
{
    CLRBIT_N(fg_test_run, TEST_RUN_HOTW_MODE);
    fg_test_run_hotw_TBH_run = FALSE;
    if (fg_hotw_run)
    {
        SETCMD_N(HOTW_OFF);
    }
}

/*******************************************************************************
 函 数: log_sys_stop_test_run_prev_need()
 功 能: 判断是否有关闭上一个试运行功能需求，并关闭相应的试运行功能
 参 数: test_run_next--; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOL log_sys_stop_test_run_prev_need(U08 test_run_next)
{
    if (fg_test_run_cur >= TEST_RUN_MAX
    || test_run_next >= TEST_RUN_MAX
    )
    {
        return FALSE;
    }
    else
    {  
        if (fg_test_run_cur != test_run_next)
        {
            switch (fg_test_run_cur)
            {
                case TEST_RUN_AIR_EMPTY:  log_sys_init_test_run_air_empty();  break;
                case TEST_RUN_PUMP:       log_sys_init_test_run_pump();       break; 
                case TEST_RUN_COOL_MODE:  log_sys_init_test_run_cool_mode();  break;
                case TEST_RUN_HEAT_MODE:  log_sys_init_test_run_heat_mode();  break;
                case TEST_RUN_HOTW_MODE:  log_sys_init_test_run_hotw_mode();  break;
            }
        
            return TRUE;
        }
    }

    return FALSE;
}

/*******************************************************************************
 函 数: log_sys_test_run_air_empty()
 功 能: 空气排空试运行功能
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
#define TEST_AIR_EMPTY_TS_BEGIN     (600)
#define TEST_AIR_EMPTY_TS_STOP      (660)
void log_sys_test_run_air_empty(void)
{
    static BOL fg_wait = FALSE;
    
    /* 命令检查 */
    if (!fg_wait
    && GETBIT_N(cmd_mux, TEST_RUN_AIR_EMPTY_ON)            /* 收到空气排空试运行命令 */
    && !GETBIT_N(fg_test_run, TEST_RUN_AIR_EMPTY)           /* 未进入空气排空试运行 */
    && log_sys_stop_test_run_prev_need(TEST_RUN_AIR_EMPTY)  /* 需要关上一个试运行 */
    )
    {
        fg_wait = TRUE;
        return;
    }

    /* 不可进入试运行功能 */
    if (fg_can_enter_test_run)
    {
        fg_wait = FALSE;
    }

    /* 开启: 空气排空试运行 */
    if (!fg_wait && GETCMD_N(TEST_RUN_AIR_EMPTY_ON))	
    {
        /* 试运行功能切换 */
        ARRCLR(fg_test_run);
        SETBIT_N(fg_test_run, TEST_RUN_AIR_EMPTY);
    }

    /* 关闭: 空气排空试运行 */
    if (GETCMD_N(TEST_RUN_AIR_EMPTY_OFF))	
    {
        CLRBIT_N(fg_test_run, TEST_RUN_AIR_EMPTY);

        /* 开关机处理 */
        if (fg_zone_A_run)
        {
            SETCMD_N(ZONE_A_OFF);
        }
        if (fg_zone_B_run)
        {
            SETCMD_N(ZONE_B_OFF);
        }
        if (fg_hotw_run)
        {
            SETCMD_N(HOTW_OFF);
        }
        ARRSET(air_pump_dly, 0xff);
    }

    /* 运行时处理 */
    if (GETBIT_N(fg_test_run, TEST_RUN_AIR_EMPTY))
    {
        switch (test_run_air_empty_stage)
        {
            /* 空闲 */
            case TRAE_IDLE:
                test_run_air_empty_stage = TRAE_IDU;
                cnt_test_run_air_empty = 0;
                break;

            /* 内机 */
            case TRAE_IDU:
                /* 检查开机状态 */
                if (!fg_zone_A_run)
                {
                    SETCMD_N(ZONE_A_ON);
                }

                /* 动作处理 */
                if (cnt_test_run_air_empty < TEST_AIR_EMPTY_TS_BEGIN)
                {
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V1_ON);    /* S3V1 ON输出 */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V2_ON);    /* S3V2 OFF输出 */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V3_ON);    /* S3V3 ON输出 */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPF_ON);   /* PUMPf开 */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPO_ON);   /* PUMPo关 */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPM_ON);   /* PUMPm关 */
                }
                else if (cnt_test_run_air_empty < TEST_AIR_EMPTY_TS_STOP)
                {
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V1_ON);    /* S3V1 ON输出 */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V2_ON);    /* S3V2 OFF输出 */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V3_ON);    /* S3V3 ON输出 */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPF_ON);   /* PUMPf关 */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPO_ON);   /* PUMPo关 */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPM_ON);   /* PUMPm关 */
                }
                else
                {
                    test_run_air_empty_stage = TRAE_HW;
                    cnt_test_run_air_empty = 0;
                }
                break;
                
            /* 制热水 */
            case TRAE_HW:
                /* 无热水时，切换至下一阶段 */
                if (!val_hotw_used)
                {
                    test_run_air_empty_stage = TRAE_A_HT;
                    cnt_test_run_air_empty = 0;
                    break;
                }

                /* 检查开机状态 */
                if (!fg_hotw_run)
                {
                    SETCMD_N(ZONE_A_OFF);
                    SETCMD_N(HOTW_ON);
                }

                /* 动作处理 */
                if (cnt_test_run_air_empty < TEST_AIR_EMPTY_TS_BEGIN)
                {
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V1_ON);    /* S3V1 OFF输出 */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V2_ON);    /* S3V2 OFF输出 */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V3_ON);    /* S3V3 ON输出 */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPF_ON);   /* PUMPf开 */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPO_ON);   /* PUMPo关 */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPM_ON);   /* PUMPm关 */
                }
                else if (cnt_test_run_air_empty < TEST_AIR_EMPTY_TS_STOP)
                {
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V1_ON);    /* S3V1 ON输出 */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V2_ON);    /* S3V2 OFF输出 */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V3_ON);    /* S3V3 ON输出 */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPF_ON);   /* PUMPf关 */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPO_ON);   /* PUMPo关 */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPM_ON);   /* PUMPm关 */
                }
                else
                {
                    test_run_air_empty_stage = TRAE_A_HT;
                    cnt_test_run_air_empty = 0;
                }
                break;

            /* A区制热 */
            case TRAE_A_HT:
                /* 仅制冷时，切换至下一阶段 */
                if (USE_COOL_ONLY)
                {
                    test_run_air_empty_stage = TRAE_A_CL;
                    cnt_test_run_air_empty = 0;
                    break;
                }

                /* 检查开机状态 */
                if (!fg_zone_A_run)
                {
                    SETCMD_N(HOTW_OFF);
                    SETCMD_N(ZONE_A_ON);
                }

                /* 动作处理 */
                if (cnt_test_run_air_empty < TEST_AIR_EMPTY_TS_BEGIN)
                {
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V1_ON);    /* S3V1 ON输出 */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V2_ON);    /* S3V2 OFF输出 */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V3_ON);    /* S3V3 ON输出 */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPF_ON);   /* PUMPf开 */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPO_ON);   /* PUMPo开 */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPM_ON);   /* PUMPm关 */
                }
                else if (cnt_test_run_air_empty < TEST_AIR_EMPTY_TS_STOP)
                {
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V1_ON);    /* S3V1 ON输出 */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V2_ON);    /* S3V2 OFF输出 */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V3_ON);    /* S3V3 ON输出 */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPF_ON);   /* PUMPf关 */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPO_ON);   /* PUMPo关 */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPM_ON);   /* PUMPm关 */
                }
                else
                {
                    test_run_air_empty_stage = TRAE_A_CL;
                    cnt_test_run_air_empty = 0;
                }
                break;

            /* A区制冷 */
            case TRAE_A_CL:
                /* 仅制热时，切换至下一阶段 */
                if (USE_HEAT_ONLY)
                {
                    test_run_air_empty_stage = TRAE_B_HT;
                    cnt_test_run_air_empty = 0;
                    break;
                }
                
                /* 检查开机状态 */
                if (!fg_zone_A_run)
                {
                    SETCMD_N(HOTW_OFF);
                    SETCMD_N(ZONE_A_ON);
                }

                /* 动作处理 */
                if (cnt_test_run_air_empty < TEST_AIR_EMPTY_TS_BEGIN)
                {
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V1_ON);    /* S3V1 ON输出 */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V2_ON);    /* S3V2 ON输出 */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V3_ON);    /* S3V3 ON输出 */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPF_ON);   /* PUMPf开 */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPO_ON);   /* PUMPo开 */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPM_ON);   /* PUMPm关 */
                }
                else if (cnt_test_run_air_empty < TEST_AIR_EMPTY_TS_STOP)
                {
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V1_ON);    /* S3V1 ON输出 */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V2_ON);    /* S3V2 OFF输出 */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V3_ON);    /* S3V3 OFF输出 */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPF_ON);   /* PUMPf关 */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPO_ON);   /* PUMPo关 */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPM_ON);   /* PUMPm关 */
                }
                else
                {
                    test_run_air_empty_stage = TRAE_B_HT;
                    cnt_test_run_air_empty = 0;
                }
                break;

            /* B区制热 */
            case TRAE_B_HT:
                /* 非双区控制或仅制冷时，切换至下一阶段 */
                if (!USE_DUAL_ZONE_CTRL
                || USE_COOL_ONLY
                )
                {
                    test_run_air_empty_stage = TRAE_IDLE;
                    SETCMD_N(TEST_RUN_AIR_EMPTY_OFF);
                    break;
                }

                /* 检查开机状态 */
                if (!fg_zone_B_run)
                {
                    cnt_test_run_air_empty = 0;
                    SETCMD_N(ZONE_A_OFF);
                    SETCMD_N(ZONE_B_ON);
                }

                /* 动作处理 */
                if (cnt_test_run_air_empty < TEST_AIR_EMPTY_TS_BEGIN)
                {
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V1_ON);    /* S3V1 ON输出 */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V2_ON);    /* S3V2 OFF输出 */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V3_ON);    /* S3V3 OFF输出 */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPF_ON);   /* PUMPf开 */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPO_ON);   /* PUMPo关 */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPM_ON);   /* PUMPm开 */
                }
                else if (cnt_test_run_air_empty < TEST_AIR_EMPTY_TS_STOP)
                {
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V1_ON);    /* S3V1 ON输出 */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V2_ON);    /* S3V2 OFF输出 */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V3_ON);    /* S3V3 ON输出 */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPF_ON);   /* PUMPf关 */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPO_ON);   /* PUMPo关 */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPM_ON);   /* PUMPm关 */
                }
                else
                {
                    test_run_air_empty_stage = TRAE_IDLE;
                    SETCMD_N(TEST_RUN_AIR_EMPTY_OFF);
                }
                break;
        }
    }
    else
    {
        ARRCLR(fg_test_air_empty_on);           /* 全关 */
        test_run_air_empty_stage = TRAE_IDLE;
    }
}

/*******************************************************************************
 函 数: log_sys_test_run_pump()
 功 能: 水泵试运行功能
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
#define TEST_PUMP_TS_BEGIN      (120)
#define TEST_PUMP_TS_STABLE     (300)
#define TEST_PUMP_TS_STOP       (360)
void log_sys_test_run_pump(void)
{
    U08 i;
    static BOL fg_wait = FALSE;
    
    /* 命令检查 */
    if (!fg_wait
    && GETBIT_N(cmd_mux, TEST_RUN_PUMP_ON)                 /* 收到水泵试运行命令 */
    && !GETBIT_N(fg_test_run, TEST_RUN_PUMP)                /* 未进入水泵试运行 */
    && log_sys_stop_test_run_prev_need(TEST_RUN_PUMP)       /* 需要关上一个试运行 */
    )
    {
        fg_wait = TRUE;
        return;
    }

    /* 不可进入试运行功能 */
    if (fg_can_enter_test_run)
    {
        fg_wait = FALSE;
    }
    
    /* 1. 命令处理 */
    if (!fg_wait && GETCMD_N(TEST_RUN_PUMP_ON))         /* 开启: 水泵试运行 */
    {
        /* 试运行功能切换 */
        ARRCLR(fg_test_run);
        SETBIT_N(fg_test_run, TEST_RUN_PUMP);
    }
    if (GETCMD_N(TEST_RUN_PUMP_OFF))		/* 关闭: 水泵试运行 */
    {
        CLRBIT_N(fg_test_run, TEST_RUN_PUMP);

        /* 开关机处理 */
        if (fg_zone_A_run)
        {
            SETCMD_N(ZONE_A_OFF);
        }
        if (fg_zone_B_run)
        {
            SETCMD_N(ZONE_B_OFF);
        }
        if (fg_hotw_run)
        {
            SETCMD_N(HOTW_OFF);
        }
        ARRSET(air_pump_dly, 0xff);
    }

    /* 2. 运行时处理 */
    if (GETBIT_N(fg_test_run, TEST_RUN_PUMP))
    {
        switch (test_run_pump_stage)
        {
            /* 空闲 */
            case TRP_IDLE:
                test_run_pump_stage = TRP_HW;
                cnt_test_run_pump = 0;
                break;
                
            /* 制热水 */
            case TRP_HW:
                /* 无热水时，切换至下一阶段 */
                if (!val_hotw_used)
                {
                    test_run_pump_stage = TRP_A_HT;
                    cnt_test_run_pump = 0;
                    break;
                }

                /* 检查开机状态 */
                if (!fg_hotw_run)
                {
                    SETCMD_N(HOTW_ON);
                }

                /* 动作处理 */
                if (cnt_test_run_pump < TEST_PUMP_TS_BEGIN)
                {
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_S3V1_ON);    /* S3V1 OFF输出 */
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_S3V2_ON);    /* S3V2 OFF输出 */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_S3V3_ON);    /* S3V3 ON输出 */
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_PUMPF_ON);   /* PUMPf开 */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_PUMPO_ON);   /* PUMPo关 */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_PUMPM_ON);   /* PUMPm关 */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_CHECK_LDIW); /* 不检测LDIw */
                }
                else if (cnt_test_run_pump < TEST_PUMP_TS_STABLE)
                {
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_CHECK_LDIW); /* 检测LDIw */
                }
                else if (cnt_test_run_pump < TEST_PUMP_TS_STOP)
                {
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_S3V1_ON);    /* S3V1 ON输出 */
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_S3V2_ON);    /* S3V2 OFF输出 */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_S3V3_ON);    /* S3V3 ON输出 */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_PUMPF_ON);   /* PUMPf关 */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_PUMPO_ON);   /* PUMPo关 */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_PUMPM_ON);   /* PUMPm关 */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_CHECK_LDIW); /* 不检测LDIw */
                }
                else
                {
                    test_run_pump_stage = TRP_A_HT;
                    cnt_test_run_pump = 0;
                }
                
                break;

            /* A区制热 */
            case TRP_A_HT:
                /* 仅制冷时，切换至下一阶段 */
                if (USE_COOL_ONLY)
                {
                    test_run_pump_stage = TRP_A_CL;
                    cnt_test_run_pump = 0;
                    break;
                }

                /* 检查开机状态 */
                if (!fg_zone_A_run)
                {
                    SETCMD_N(HOTW_OFF);
                    SETCMD_N(ZONE_A_ON);
                }

                /* 动作处理 */
                if (cnt_test_run_pump < TEST_PUMP_TS_BEGIN)
                {
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_S3V1_ON);    /* S3V1 ON输出 */
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_S3V2_ON);    /* S3V2 OFF输出 */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_S3V3_ON);    /* S3V3 ON输出 */
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_PUMPF_ON);   /* PUMPf开 */
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_PUMPO_ON);   /* PUMPo开 */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_PUMPM_ON);   /* PUMPm关 */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_CHECK_LDIW); /* 不检测LDIw */
                }
                else if (cnt_test_run_pump < TEST_PUMP_TS_STABLE)
                {
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_CHECK_LDIW); /* 检测LDIw */
                }
                else if (cnt_test_run_pump < TEST_PUMP_TS_STOP)
                {
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_S3V1_ON);    /* S3V1 ON输出 */
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_S3V2_ON);    /* S3V2 OFF输出 */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_S3V3_ON);    /* S3V3 ON输出 */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_PUMPF_ON);   /* PUMPf关*/
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_PUMPO_ON);   /* PUMPo关 */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_PUMPM_ON);   /* PUMPm关 */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_CHECK_LDIW); /* 不检测LDIw */
                }
                else
                {
                    test_run_pump_stage = TRP_A_CL;
                    cnt_test_run_pump = 0;
                }
                break;

            /* A区制冷 */
            case TRP_A_CL:
                /* 仅制热时，切换至下一阶段 */
                if (USE_HEAT_ONLY)
                {
                    test_run_pump_stage = TRP_B_HT;
                    cnt_test_run_pump = 0;
                    break;
                }
                                
                /* 检查开机状态 */
                if (!fg_zone_A_run)
                {
                    SETCMD_N(HOTW_OFF);
                    SETCMD_N(ZONE_A_ON);
                }

                /* 动作处理 */
                if (cnt_test_run_pump < TEST_PUMP_TS_BEGIN)
                {
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_S3V1_ON);    /* S3V1 ON输出 */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_S3V2_ON);    /* S3V2 ON输出 */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_S3V3_ON);    /* S3V3 ON输出 */
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_PUMPF_ON);   /* PUMPf开 */
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_PUMPO_ON);   /* PUMPo开 */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_PUMPM_ON);   /* PUMPm关 */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_CHECK_LDIW); /* 不检测LDIw */
                }
                else if (cnt_test_run_pump < TEST_PUMP_TS_STABLE)
                {
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_CHECK_LDIW); /* 检测LDIw */
                }
                else if (cnt_test_run_pump < TEST_PUMP_TS_STOP)
                {
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_S3V1_ON);    /* S3V1 ON输出 */
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_S3V2_ON);    /* S3V2 OFF输出 */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_S3V3_ON);    /* S3V3 ON输出 */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_PUMPF_ON);   /* PUMPf关*/
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_PUMPO_ON);   /* PUMPo关 */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_PUMPM_ON);   /* PUMPm关 */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_CHECK_LDIW); /* 不检测LDIw */
                }
                else
                {
                    test_run_pump_stage = TRP_B_HT;
                    cnt_test_run_pump = 0;
                }
                break;

            /* B区制热 */
            case TRP_B_HT:
                /* 非双区控制或仅制冷时，切换至下一阶段 */
                if (!USE_DUAL_ZONE_CTRL
                || USE_COOL_ONLY
                )
                {
                    test_run_pump_stage = TRP_IDLE;
                    SETCMD_N(TEST_RUN_PUMP_OFF);
                    break;
                }

                /* 检查开机状态 */
                if (!fg_zone_B_run)
                {
                    cnt_test_run_pump = 0;
                    SETCMD_N(ZONE_A_OFF);
                    SETCMD_N(ZONE_B_ON);
                }

                /* 动作处理 */
                if (cnt_test_run_pump < TEST_PUMP_TS_BEGIN)
                {
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_S3V1_ON);    /* S3V1 ON输出 */
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_S3V2_ON);    /* S3V2 OFF输出 */
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_S3V3_ON);    /* S3V3 OFF输出 */
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_PUMPF_ON);   /* PUMPf开 */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_PUMPO_ON);   /* PUMPo关 */
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_PUMPM_ON);   /* PUMPm开 */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_CHECK_LDIW); /* 不检测LDIw */
                }
                else if (cnt_test_run_pump < TEST_PUMP_TS_STABLE)
                {
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_CHECK_LDIW); /* 检测LDIw */
                }
                else if (cnt_test_run_pump < TEST_PUMP_TS_STOP)
                {
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_S3V1_ON);    /* S3V1 ON输出 */
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_S3V2_ON);    /* S3V2 OFF输出 */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_S3V3_ON);    /* S3V3 ON输出 */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_PUMPF_ON);   /* PUMPf关*/
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_PUMPO_ON);   /* PUMPo关 */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_PUMPM_ON);   /* PUMPm关 */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_CHECK_LDIW); /* 不检测LDIw */
                }
                else
                {
                    test_run_pump_stage = TRP_IDLE;
                    SETCMD_N(TEST_RUN_PUMP_OFF);
                }
                break;
        }

        /* 退出水泵试运行功能 */
        for (i = 0; i < val_pcb_num; i++)
        {
            if (m_get_error(i, S_ERR_WATER_FLOW))
            {
                SETCMD_N(TEST_RUN_PUMP_OFF);
            }
        }
    }
    else
    {
        ARRCLR(fg_test_pump_on);
        test_run_pump_stage = TRP_IDLE;
    }
}

/*******************************************************************************
 函 数: log_sys_test_run_cool_mode()
 功 能: 制冷试运行功能
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_test_run_cool_mode(void)
{
    static BOL fg_wait = FALSE;
    
    /* 命令检查 */
    if (!fg_wait
    && GETBIT_N(cmd_mux, TEST_RUN_COOL_MODE_ON)				/* 收到制冷试运行命令 */
    && !GETBIT_N(fg_test_run, TEST_RUN_COOL_MODE)			/* 未进入制冷试运行 */
    && log_sys_stop_test_run_prev_need(TEST_RUN_COOL_MODE)	/* 需要关上一个试运行 */
    )
    {
        fg_wait = TRUE;
        return;
    }

    /* 不可进入试运行功能 */
    if (fg_can_enter_test_run)
    {
        fg_wait = FALSE;
    }
    
    /* 1. 命令处理 */
    if (!fg_wait && GETCMD_N(TEST_RUN_COOL_MODE_ON))	/* 开启: 制冷试运行 */
    {
        /* 允许进入制冷试运行 */
        if (SensorCanUse(AC_NRG_CTRL_TEMP)
        && AC_NRG_CTRL_TEMP > TEST_RUN_COOL_MODE_SV_COOL
        )
        {
            /* 试运行功能切换 */
            ARRCLR(fg_test_run);
            SETBIT_N(fg_test_run, TEST_RUN_COOL_MODE);

            /* 开关机处理 */
            if (!fg_zone_A_run)
            {
                SETCMD_N(ZONE_A_ON);		/* A区开机 */
            }
            if (val_hotw_used
            && fg_hotw_run
            )
            {
                SETCMD_N(HOTW_OFF);         /* 热水关机 */
            }
        }
    }
    if (GETCMD_N(TEST_RUN_COOL_MODE_OFF))	/* 关闭: 制冷试运行 */
    {
        if (GETBIT_N(fg_test_run, TEST_RUN_COOL_MODE))
        {
            CLRBIT_N(fg_test_run, TEST_RUN_COOL_MODE);
            if (fg_zone_A_run)
            {
                SETCMD_N(ZONE_A_OFF);
            }
        }
    }

    /* 2. 运行时处理 */
    if (GETBIT_N(fg_test_run, TEST_RUN_COOL_MODE))
    {
        if (SensorCannotUse(AC_NRG_CTRL_TEMP)
        || (AC_NRG_CTRL_TEMP <= TEST_RUN_COOL_MODE_SV_COOL)
        || log_sys_cell_is_err()
        )
        {
            SETCMD_N(TEST_RUN_COOL_MODE_OFF);
        }
    }  
}

/*******************************************************************************
 函 数: log_sys_test_run_heat_mode()
 功 能: 制热试运行功能
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_test_run_heat_mode(void)
{
    static BOL fg_IBH_first_on = TRUE;
    static BOL fg_wait = FALSE;
    
    /* 命令检查 */
    if (!fg_wait
    && GETBIT_N(cmd_mux, TEST_RUN_HEAT_MODE_ON)				/* 收到制热试运行命令 */
    && !GETBIT_N(fg_test_run, TEST_RUN_HEAT_MODE)			/* 未进入制热试运行 */
    && log_sys_stop_test_run_prev_need(TEST_RUN_HEAT_MODE)	/* 需要关上一个试运行 */
    )
    {
        fg_wait = TRUE;
        return;
    }

    /* 不可进入试运行功能 */
    if (fg_can_enter_test_run)
    {
        fg_wait = FALSE;
    }

    /* 1. 命令处理 */
    if (!fg_wait && GETCMD_N(TEST_RUN_HEAT_MODE_ON))	/* 开启: 制热试运行 */
    {
        /* 允许进入制热试运行 */
        if (SensorCanUse(AC_NRG_CTRL_TEMP)
        && AC_NRG_CTRL_TEMP < TEST_RUN_HEAT_MODE_SV_HEAT
        )
        {
            /* 试运行标志处理 */
            ARRCLR(fg_test_run);
            SETBIT_N(fg_test_run, TEST_RUN_HEAT_MODE);

            /* 开关机处理 */
            if (!fg_zone_A_run)
            {
                SETCMD_N(ZONE_A_ON);		/* A区开机 */
            }
            if (USE_DUAL_ZONE_CTRL
            && !fg_zone_B_run
            )
            {
                SETCMD_N(ZONE_B_ON);        /* B区开机 */
            }
            if (val_hotw_used
            && fg_hotw_run
            )
            {
                SETCMD_N(HOTW_OFF);         /* 热水关机 */
            }
        }
    }    
    if (GETCMD_N(TEST_RUN_HEAT_MODE_OFF))	/* 关闭: 制热试运行 */	
    {
        if (GETBIT_N(fg_test_run, TEST_RUN_HEAT_MODE))
        {
            /* 试运行标志处理 */
            CLRBIT_N(fg_test_run, TEST_RUN_HEAT_MODE);

            /* 开关机处理 */
            if (fg_zone_A_run)
            {
                SETCMD_N(ZONE_A_OFF);
            }
            if (fg_zone_B_run)
            {
                SETCMD_N(ZONE_B_OFF);
            }
//            SETCMD_N(ZONE_A_OFF);			/* A区关机 */
//            SETCMD_N(ZONE_B_OFF);			/* B区关机 */
        }
    }
    if (GETCMD_N(TEST_RUN_HEAT_IBH_ON))		/* 开启: 制热试运行辅助电热运行 */
    {
        fg_test_run_heat_IBH_run = TRUE;
    }
    if (GETCMD_N(TEST_RUN_HEAT_IBH_OFF))	/* 关闭: 制热试运行辅助电热运行 */
    {
        fg_test_run_heat_IBH_run = FALSE;
    }

    /* 2. 运行时处理 */
    if (GETBIT_N(fg_test_run, TEST_RUN_HEAT_MODE))
    {
        /* 开启IBH */
        if (fg_IBH_first_on
        && heat_runtime[SUB_SYS_AC_HOTW].on > 10*60
        && !fg_test_run_heat_IBH_run
        )
        {
            fg_IBH_first_on = FALSE;
            SETCMD_N(TEST_RUN_HEAT_IBH_ON);
        }

        /* IBH运行3min后关闭 */
        if (cnt_IBH_on > 3*60)
        {
            SETCMD_N(TEST_RUN_HEAT_IBH_OFF);
        }

        /* 退出制热试运行 */
        if (SensorCannotUse(AC_NRG_CTRL_TEMP)
        || AC_NRG_CTRL_TEMP >= TEST_RUN_HEAT_MODE_SV_HEAT
        || log_sys_cell_is_err()
        )
        {
            SETCMD_N(TEST_RUN_HEAT_MODE_OFF);
        }
    }
    else
    {
        fg_IBH_first_on = TRUE;
        fg_test_run_heat_IBH_run = FALSE;
    }
}

/*******************************************************************************
 函 数: log_sys_test_run_hotw_mode()
 功 能: 制热水试运行功能
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_test_run_hotw_mode(void)
{
    static BOL fg_TBH_first_on = TRUE;
    static BOL fg_wait = FALSE;
    
    /* 0. 命令检查 */
    if (!fg_wait
    && GETBIT_N(cmd_mux, TEST_RUN_HOTW_MODE_ON)             /* 收到热水试运行命令 */
    && !GETBIT_N(fg_test_run, TEST_RUN_HOTW_MODE)           /* 未进入热水试运行 */
    && log_sys_stop_test_run_prev_need(TEST_RUN_HOTW_MODE)  /* 需要关上一个试运行 */
    )
    {
        fg_wait = TRUE;
        return;
    }

    /* 不可进入试运行功能 */
    if (fg_can_enter_test_run)
    {
        fg_wait = FALSE;
    }
      
    /* 1. 命令处理 */
    if (!fg_wait && GETCMD_N(TEST_RUN_HOTW_MODE_ON))	/* 开启: 制热水试运行 */
    {
        /* 允许进入制热水试运行 */
        if (val_hotw_used
        && SensorCanUse(ctrl_temp_energy_hotw)
        && ctrl_temp_energy_hotw < TEST_RUN_HOTW_MODE_SV_HOTW
        )
        {
            /* 试运行标志处理 */
            ARRCLR(fg_test_run);
            SETBIT_N(fg_test_run, TEST_RUN_HOTW_MODE);

            /* 开关机处理 */
            if (val_hotw_used
            && !fg_hotw_run
            )
            {
                SETCMD_N(HOTW_ON);         /* 热水开机 */
            }
            if (fg_zone_A_run)
            {
                SETCMD_N(ZONE_A_OFF);		/* A区关机 */
            }
            if (USE_DUAL_ZONE_CTRL
            && fg_zone_B_run
            )
            {
                SETCMD_N(ZONE_B_OFF);        /* B区关机 */
            }
        }
    }    
    if (GETCMD_N(TEST_RUN_HOTW_MODE_OFF))	/* 关闭: 制热水试运行 */	
    {
        if (GETBIT_N(fg_test_run, TEST_RUN_HOTW_MODE))
        {
            /* 试运行标志处理 */
            CLRBIT_N(fg_test_run, TEST_RUN_HOTW_MODE);

            /* 开关机处理 */
            if (fg_hotw_run)
            {
                SETCMD_N(HOTW_OFF);
            }
        }
    }
    if (GETCMD_N(TEST_RUN_HOTW_TBH_ON))		/* 开启: 制热水试运行水箱电热运行 */
    {
        fg_test_run_hotw_TBH_run = TRUE;
    }
    if (GETCMD_N(TEST_RUN_HOTW_TBH_OFF))	/* 关闭: 制热水试运行水箱电热运行 */
    {
        fg_test_run_hotw_TBH_run = FALSE;
    }

    /* 2. 运行时处理 */
    if (GETBIT_N(fg_test_run, TEST_RUN_HOTW_MODE))
    {
        /* 开启TBH */
        if (fg_TBH_first_on
        && hotw_runtime.on > 10*60
        && !fg_test_run_hotw_TBH_run
        )
        {
            fg_TBH_first_on = FALSE;
            SETCMD_N(TEST_RUN_HOTW_TBH_ON);
        }

        /* TBH运行3min后关闭 */
        if (cnt_test_run_hotw_TBH_on > 3*60)
        {
            SETCMD_N(TEST_RUN_HOTW_TBH_OFF);
        }

        /* 退出制热水试运行 */
        if (SensorCannotUse(ctrl_temp_energy_hotw)
        || ctrl_temp_energy_hotw >= TEST_RUN_HOTW_MODE_SV_HOTW
        || log_sys_cell_is_err()
        )
        {
            SETCMD_N(TEST_RUN_HOTW_MODE_OFF);
        }
    }
    else
    {
        fg_TBH_first_on = TRUE;
        fg_test_run_hotw_TBH_run = FALSE;
    }
}

/*******************************************************************************
 函 数: log_sys_test_run_status()
 功 能: 试运行状态处理
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_test_run_status(void)
{
    U08 i;

    fg_enter_test_run = FALSE;
    fg_test_run_cur = TEST_RUN_MAX;
    for (i = 0; i < TEST_RUN_MAX; i++)
    {
        if (GETBIT_N(fg_test_run, i))
        {
            fg_enter_test_run = TRUE;
            fg_test_run_cur = i;
            break;
        }
    }

    fg_can_enter_test_run = (STEP_IS_IDLE||STEP_IS_ALARM);

    fg_air_empty_test_run = GETBIT_N(fg_test_run, TEST_RUN_AIR_EMPTY);
    fg_pump_test_run = GETBIT_N(fg_test_run, TEST_RUN_PUMP);

    /* 试运行是否检测水流开关 */
    if (GETBIT_N(fg_test_run, TEST_RUN_COOL_MODE)
    || GETBIT_N(fg_test_run, TEST_RUN_HEAT_MODE)
    || GETBIT_N(fg_test_run, TEST_RUN_HOTW_MODE)
    || (GETBIT_N(fg_test_run, TEST_RUN_PUMP)&&GETBIT_N(fg_test_pump_on, TEST_PUMP_CHECK_LDIW))
    )
    {
        fg_test_run_check_LDIw = TRUE;
    }
    else
    {
        fg_test_run_check_LDIw = FALSE;
    }

    /* 试运行是否关闭PUMPf */
    if ((fg_air_empty_test_run&&!GETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPF_ON))
    || (fg_pump_test_run&&!GETBIT_N(fg_test_pump_on, TEST_PUMP_PUMPF_ON))
    )
    {
        fg_test_run_PUMPf_off = TRUE;
    }
    else
    {
        fg_test_run_PUMPf_off = FALSE;
    }
}

/*******************************************************************************
 函 数: log_sys_test_run()
 功 能: 试运行功能
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_test_run(void)
{
    log_sys_test_run_status();
    log_sys_test_run_air_empty();			/* 空气排空试运行 */
    log_sys_test_run_pump();                /* 水泵试运行 */
    log_sys_test_run_cool_mode();			/* 制冷试运行 */
    log_sys_test_run_heat_mode();			/* 制热试运行 */
    log_sys_test_run_hotw_mode();			/* 制热水试运行 */
}

#define USE_EGY_INC         /* 使用增量式能调算法 */
/*******************************************************************************
 函 数: log_sys_sub_sys_ac_hotw_energy()
 功 能: 空调+热水子系统能量调节(计算能量需求、控制压缩机启停)
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_sub_sys_ac_hotw_energy(void)
{
    U08 energy_area = _NOTHING;    /* 能量需求分区 */
    U08 RequireNum, comp_need_now, sub_sys_type;

    sub_sys_type = (energy_ctrl_type == GROUP_ENERGY_CTRL) ? SUB_SYS_AC_HOTW : SUB_SYS_TYPE_MAX;
    RequireNum = CountCompRequire(ctrl_mode[SUB_SYS_AC_HOTW], sub_sys_type);
    comp_need_now = need_comp_num_in_system(sub_sys_type);
    
    if (!STEP_IS_RUN                        /* 未运行时不进行能量调节 */
    || fg_run_sta_hotw_antifreeze           /* 运行态热水水路防冻时不进行能量调节 */
    )
    {
        return;
    }

#if 0        
    if( val_pump_mode==PUMP_MODE_INTER && MODE_IS_HEAT(SUB_SYS_AC_HOTW) && !fg_ac_need_enable[SUB_SYS_AC_HOTW])  
    {
//        bak_ac_need = _NOTHING;
        fg_energy_need[SUB_SYS_AC_HOTW] = FALSE;
            
        return; // 制热且水泵间歇开，未满足条件不进行能量调节。     
    }
#endif


    /* 温度区域 */
    switch (ctrl_mode[SUB_SYS_AC_HOTW])
    {
        case MODE_COOL :    /* 制冷模式 */
            if (log_sys_have_comp_run(MODE_HEAT, sub_sys_type))
            {
//              ac_need = _QUICK;
                log_sys_select_comp_stop_once(MODE_HEAT, sub_sys_type);
            }
            else
            {
                energy_area = cool_area;
            }
            
            break;

        case MODE_HEAT :    /* 制热模式 */
        case MODE_HOTW:     /* 制热水模式 */
            if (log_sys_have_comp_run(MODE_COOL, sub_sys_type))
            {
//              ac_need = _QUICK;
                log_sys_select_comp_stop_once(MODE_COOL, sub_sys_type);
            }
            else
            {
                energy_area = (ctrl_mode[SUB_SYS_AC_HOTW] == MODE_HEAT) ? heat_area : hotw_area;
            }

            /* 处理热水、制热不停机切换 */
            if (cnt_hotw_heat_switch_dly > 0)
            {
                if (energy_area == _SUBB)
                {
                    energy_area = _KEEP;
                }
            }
            break;

        default:
            energy_area = _QUICK;
            break;
    }

    /* 定时防冻时强制修改需求为加载区 */
    if (fg_zone_A_timed_antifreeze_COMPf_need
    ||  fg_zone_B_timed_antifreeze_COMPf_need
    ||  fg_hotw_timed_antifreeze_COMPf_need
    )
    {
        if (energy_area==_SUBB || energy_area==_QUICK)
        {
            energy_area = _ADD;
        }
    }

    /* 计算空调+热水子系统能量需求 */
    if (energy_area == _ADD)      fg_energy_need[SUB_SYS_AC_HOTW] = TRUE;
    if (energy_area == _SUBB)     fg_energy_need[SUB_SYS_AC_HOTW] = FALSE;
    
#ifndef USE_EGY_INC
    /* 压机全开温差能调逻辑 */
    if (comp_need_now > RequireNum)
    {
        energy_area = _SUBB;
    }
    else if (comp_need_now == RequireNum)
    {
        if (energy_area == _ADD)    energy_area = _KEEP;
    }
#endif  
    
    /* 温度探头故障 */
    if (SensorCannotUse(ctrl_temp_energy[SUB_SYS_AC_HOTW])) 
    {
        m_ctrl_time = 0;
        energy_area = _KEEP;
    }
    if ((fg_ac_sys_err && fg_hotw_sys_err)                  /* 空调系统故障时，能量卸载 */
    || fg_TA_limit_comp_open[SUB_SYS_AC_HOTW]               /* 空调+热水子系统能调环境温度限制压机开启 */
    || fg_HDIsl_limit_comp_open                             /* 太阳能信号限制压机开启 */
    || fg_TSL_limit_comp_open                               /* 太阳能温度限制压机开启 */
    || fg_floor_preheat_limit_comp_open                     /* 地暖预热限制压机开启 */
    || GETBIT_N(fg_test_run, TEST_RUN_AIR_EMPTY)            /* 空气排空试运行 */
    || GETBIT_N(fg_test_run, TEST_RUN_PUMP)                 /* 水泵试运行 */
    )
    {
        energy_area = _QUICK;
    }       
//    bak_ac_need = energy_area;
    /* 能量模糊调节 */
    if(fg_have_stop == TRUE)  /* 第一次开机 */
    {
        if (comp_need_now < RequireNum)
        {
            m_ctrl_period = OPEN_PERIOD;
        }
        else
        {
            fg_have_stop =FALSE;
        }
    }
    else
    {
        if (energy_area == _QUICK)
        {
            m_ctrl_period = QUICK_PERIOD;
        }
        else
        {
            m_ctrl_period = val_ctrl_period;
        }

#ifdef USE_EGY_INC
        /* 开关压机时，考虑频率 */
        switch (energy_area)
        {
            case _ADD:
                /* 不可升频时才开启压机 */
                if (log_sys_have_comp_status(COMP_CAN_ADD, sub_sys_type))
                {
                    energy_area = _KEEP;
                }
                break;
//            case _SUBB:
//                /* 不可降频时才关闭压机：卸载时不做判断，防止超温后不及时停机 */
//                if (log_sys_have_comp_status(COMP_CAN_SUB, SUB_SYS_AC_HOTW))
//                {
//                    ac_need = _KEEP;
//                }
//                break;
        }
#endif
    }
    
    if (m_ctrl_time >= m_ctrl_period) 
    {
        switch (energy_area)
        {
            case _ADD   : 
                log_sys_enegry_add(ctrl_mode[SUB_SYS_AC_HOTW], sub_sys_type);  
                break;
                
            case _SUBB  : 
            case _QUICK : 
                log_sys_enegry_sub(sub_sys_type);
                break;
                
            default :   
                break;
        }
    }
}

/*******************************************************************************
 函 数: log_sys_sub_sys_ac_energy()
 功 能: 单空调子系统能量调节(计算能量需求、控制压缩机启停)
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_sub_sys_ac_energy(void)
{
    U08 energy_area = _NOTHING;    /* 能量需求分区 */
    U08 RequireNum = CountCompRequire(ctrl_mode[SUB_SYS_AC_ONLY], SUB_SYS_AC_ONLY);
    U08 comp_need_now = need_comp_num_in_system(SUB_SYS_AC_ONLY);
    
    if (!STEP_IS_RUN                        /* 未运行时不进行能量调节 */
    || energy_ctrl_type != GROUP_ENERGY_CTRL/* 非分组能调时不进行能量调节 */
    || fg_run_sta_ac_antifreeze				/* 运行态空调水路防冻不进行能量调节 */
    )
    {
        return;       
    }

#if 0        
    if( val_pump_mode==PUMP_MODE_INTER && (MODE_IS_HEAT(SUB_SYS_AC_ONLY)) && !fg_ac_need_enable[SUB_SYS_AC_ONLY])  
    {
//        bak_ac_need = _NOTHING;
        fg_energy_need[SUB_SYS_AC_ONLY] = FALSE;
            
        return; // 制热且水泵间歇开，未满足条件不进行能量调节。     
    }
#endif

    /* 温度区域 */
    switch (ctrl_mode[SUB_SYS_AC_ONLY])
    {
        case MODE_COOL :    /* 制冷模式 */
            if (log_sys_have_comp_run(MODE_HEAT, SUB_SYS_AC_ONLY))
            {
//              ac_need = _QUICK;
                log_sys_select_comp_stop_once(MODE_HEAT, SUB_SYS_AC_ONLY);
            }
            else
            {
                energy_area = cool_area;
            }
            break;

        case MODE_HEAT :    /* 制热模式 */
            if (log_sys_have_comp_run(MODE_COOL, SUB_SYS_AC_ONLY))
            {
//              ac_need = _QUICK;
                log_sys_select_comp_stop_once(MODE_COOL, SUB_SYS_AC_ONLY);
            }
            else
            {
                energy_area = heat_area;
            }
            break;

        default:
            energy_area = _QUICK;
            break;
    }

    /* 计算单空调子系统能量需求 */
    if (energy_area == _ADD)      fg_energy_need[SUB_SYS_AC_ONLY] = TRUE;
    if (energy_area == _SUBB)     fg_energy_need[SUB_SYS_AC_ONLY] = FALSE;
    
#ifndef USE_EGY_INC
    /* 压机全开温差能调逻辑 */
    if (comp_need_now > RequireNum)
    {
        energy_area = _SUBB;
    }
    else if (comp_need_now == RequireNum)
    {
        if (energy_area == _ADD)    energy_area = _KEEP;
    }
#endif  
    
    /* 温度探头故障 */
    if (SensorCannotUse(ctrl_temp_energy_ac)) 
    {
        s_ctrl_time = 0;
        energy_area = _KEEP;
    }
    if (fg_ac_sys_err                                       /* 空调系统故障时，能量卸载 */
    || fg_TA_limit_comp_open[SUB_SYS_AC_ONLY]               /* 单空调子系统能调环境温度限制压机开启 */
    || fg_floor_preheat_limit_comp_open                     /* 地暖预热限制压机开启 */
    || GETBIT_N(fg_test_run, TEST_RUN_AIR_EMPTY)            /* 空气排空试运行 */
    || GETBIT_N(fg_test_run, TEST_RUN_PUMP)                 /* 水泵试运行 */
    || (!fg_zone_A_run && !fg_zone_B_run)                   /* A区和B区均未运行 */
    )
    {
        energy_area = _QUICK;
    }       
//    bak_ac_need = energy_area;
    /* 能量模糊调节 */
    if(fg_have_stop == TRUE)  /* 第一次开机 */
    {
        if (comp_need_now < RequireNum)
        {
            s_ctrl_period = OPEN_PERIOD;
        }
        else
        {
            fg_have_stop =FALSE;
        }
    }
    else
    {
        if (energy_area == _QUICK)
        {
            s_ctrl_period = QUICK_PERIOD;
        }
        else
        {
            s_ctrl_period = val_ctrl_period;
        }

#ifdef USE_EGY_INC
        /* 开关压机时，考虑频率 */
        switch (energy_area)
        {
            case _ADD:
                /* 不可升频时才开启压机 */
                if (log_sys_have_comp_status(COMP_CAN_ADD, SUB_SYS_AC_ONLY))
                {
                    energy_area = _KEEP;
                }
                break;
//            case _SUBB:
//                /* 不可降频时才关闭压机：卸载时不做判断，防止超温后不及时停机 */
//                if (log_sys_have_comp_status(COMP_CAN_SUB, SUB_SYS_AC_ONLY))
//                {
//                    ac_need = _KEEP;
//                }
//                break;
        }
#endif
    }
    
    if (s_ctrl_time >= s_ctrl_period) 
    {
        switch (energy_area)
        {
            case _ADD   : 
                log_sys_enegry_add(ctrl_mode[SUB_SYS_AC_ONLY], SUB_SYS_AC_ONLY);  
                break;
                
            case _SUBB  : 
            case _QUICK : 
                log_sys_enegry_sub(SUB_SYS_AC_ONLY);
                break;
                
            default :   
                break;
        }
    }

}


/********************************************************
    功能: 处理空调水泵控制
    注意：
********************************************************/
/*******************************************************************************
 函 数: log_sys_PUMP()
 功 能: 系统水泵控制，沿用原空调泵逻辑。
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
#if 0
#define  PUMP_INTER_ON_TIME     ( 6*QK_60)  // 水泵间歇开时间
#define  PUMP_INTER_OFF_TIME    (10*QK_60)  // 水泵间歇停时间
#define  PUMP_INTER_TIME        (PUMP_INTER_ON_TIME+PUMP_INTER_OFF_TIME)    // 水泵间歇周期
#define  PUMP_AC_NEED_TIME      (2*60)  // 水泵间歇开启后检测需求的时间
#define  PUMP_INTER_ENV_TEMP    -100    // 水泵常开环温-10℃
#endif

void log_sys_sub_sys_PUMPf_ctrl(U08 sub_sys_type)
{
//    I16 pump_mode = val_pump_mode;
    U08 *ptr_air_pump_dly;
    BOL *ptr_fg_pump_on;
    BOL *ptr_fg_pump_inter_on;
    U16 *ptr_pump_inter_on_off_time;
//    BOL *ptr_fg_ac_need_enable;

    ptr_air_pump_dly = &air_pump_dly[sub_sys_type];
    ptr_fg_pump_on = &fg_pump_on[sub_sys_type];
    ptr_fg_pump_inter_on = &fg_pump_inter_on[sub_sys_type];
    ptr_pump_inter_on_off_time = &pump_inter_on_off_time[sub_sys_type];
//    ptr_fg_ac_need_enable = &fg_ac_need_enable[sub_sys_type];

    *ptr_fg_pump_inter_on = FALSE;

    /* 满足条件则退当前子系统变频水泵计算 */
    if (sub_sys_type == SUB_SYS_AC_HOTW)	/* 热水子系统 */
    {
        /* 热水子系统有变频水泵需求 */
        #define FG_SUB_SYS_AC_HOTW_NEED_PUMPF (fg_hotw_run   /* 热水开机 ->注意右边的斜线不能省 */  \
        || (fg_zone_A_run || fg_zone_B_run)                  /* 空调区开机 */   \
        || FG_IDLE_OR_HOTW_ANTIFREEZE)                       /* 待机态防冻 或 运行态热水水路防冻 */

        if (!FG_SUB_SYS_AC_HOTW_NEED_PUMPF
        &&  TF_IS_IDLE			/* 商检不作此判断 */
        )
        {
            /* 不计算需求并退出 */
            *ptr_air_pump_dly = 0xff;
            *ptr_fg_pump_on = FALSE;
            *ptr_pump_inter_on_off_time = 0;
//            *ptr_fg_ac_need_enable = FALSE;
            return;
        }
    }
    else					/* 单空调子系统 */
    {
        /* 单空调子系统有变频水泵需求 */
        #define FG_SUB_SYS_AC_ONLY_NEED_PUMPF ((fg_zone_A_run||fg_zone_B_run)  /* 空调区开机 ->注意右边的斜线不能省 */ \
        || FG_IDLE_OR_AC_ANTIFREEZE)                                           /* 待机态防冻 或 运行态空调水路防冻 */
        
        if (!FG_SUB_SYS_AC_ONLY_NEED_PUMPF
        &&  TF_IS_IDLE			/* 商检不作此判断 */
        )
        {
            /* 不计算需求并退出 */
            *ptr_air_pump_dly = 0xff;
            *ptr_fg_pump_on = FALSE;
            *ptr_pump_inter_on_off_time = 0;
//            *ptr_fg_ac_need_enable = FALSE;
            return;
        }
    }

    if (((status_run == _READY && status_ready > READY_IDLE) 
        || (status_run == _RUN)                                  
        || (status_run == _DELAY && status_delay < DELAY_PUMP && *ptr_air_pump_dly < AC_PUMP_OFF_DLY))
    && !fg_idle_sta_antifreeze				/* 待机态防冻不作此判断 */
    && TF_IS_IDLE			                /* 商检不作此判断 */
    )
    {
        *ptr_air_pump_dly = 0; 
    }
    
    if (fg_idle_sta_antifreeze_pump_need    /* 防冻开空调泵 */
    ||  TF_OPEN_PUMP		                /* 商检开空调泵 */
    )
    {
        if (AC_PUMP_OFF_DLY > 1)
        {
            *ptr_air_pump_dly = AC_PUMP_OFF_DLY - 1;
        }
        else
        {
            *ptr_air_pump_dly = 0;
        }
    }

    if ((*ptr_air_pump_dly <= AC_PUMP_OFF_DLY) && (!fg_pump_err))     
    {
        *ptr_fg_pump_on = TRUE;
    }
    else           /* 水泵故障 */
    {
        *ptr_fg_pump_on = FALSE;
    }

#if 0
//    if (!AIR_IS_WATER) return;
                       
//    if (status_run == _IDLE)    /* 待机状态 */
//    {
//        *ptr_air_pump_dly = 0xff;
//        *ptr_fg_pump_on = FALSE;
//        
//        if (!fg_idle_sta_antifreeze /* 待机且未防冻，退出 */    
//        && !debug_mode
//        && TF_IS_IDLE			/* 商检不作此判断 */
//        )       
//        {
//            //fg_ac_pm_open = FALSE;        /* 退出防冻，清水泵开启标志 */
//            return;
//        }
//    }

    if (((status_run == _READY && status_ready > READY_IDLE) 
      || (status_run == _RUN)                                  
      || (status_run == _DELAY && status_delay < DELAY_PUMP && *ptr_air_pump_dly < AC_PUMP_OFF_DLY))
    && !fg_idle_sta_antifreeze				/* 待机态防冻不作此判断 */
    && TF_IS_IDLE			/* 商检不作此判断 */
    )
    {                   
        switch (ctrl_mode[sub_sys_type])
        {
            case MODE_COOL:
            case MODE_HEAT:
            case MODE_HOTW:
                if (pump_mode == PUMP_MODE_ON)      // 模式一 水泵一直不停 
                {
                    *ptr_air_pump_dly = 0;         
                }
                else if (pump_mode == PUMP_MODE_OFF)    // 模式二 温度到水泵停 
                {
                    if ((status_run == _READY && status_ready > READY_IDLE \
                        || status_run == _DELAY && status_delay < DELAY_PUMP))
                    {
                        *ptr_air_pump_dly = 0;
                    }
                    if ((log_sys_have_comp_run(MODE_NULL, SUB_SYS_TYPE_MAX) || (fg_energy_need[SUB_SYS_AC_HOTW]||fg_energy_need[SUB_SYS_AC_ONLY])) \
                        && status_run == _RUN)
                    {
                        *ptr_air_pump_dly = 0;
                    }
                }
                else if (pump_mode == PUMP_MODE_INTER)  /* 模式三 间歇性开 */
                {   
                    if (ctrl_mode[sub_sys_type] == MODE_COOL) // 制冷模式下常开
                    {
                        *ptr_air_pump_dly = 0;         /* 空调泵一直运行 */
                    }
                    else
                    {
                        if ((status_run == _READY && status_ready > READY_IDLE \
                            || status_run == _DELAY && status_delay < DELAY_PUMP))
                        {
                            *ptr_air_pump_dly = 0;
                            *ptr_pump_inter_on_off_time = 0;
                        }

                        if (log_sys_have_comp_run(MODE_NULL, SUB_SYS_TYPE_MAX) || (fg_energy_need[SUB_SYS_AC_HOTW]||fg_energy_need[SUB_SYS_AC_ONLY]))
                        {
                            *ptr_air_pump_dly = 0;
                            *ptr_pump_inter_on_off_time = 0;
                        }
                        else if (status_run  == _RUN)
                        {   
                            *ptr_fg_pump_inter_on = TRUE;    /* 间歇开空调泵标志置位 */

                            if (*ptr_pump_inter_on_off_time < PUMP_INTER_ON_TIME)
                            {
                                *ptr_air_pump_dly = 0;
                            }
                            else if (*ptr_pump_inter_on_off_time < PUMP_INTER_TIME)
                            {
                                *ptr_air_pump_dly = 0xff;
                            }
                            else
                            {
                                *ptr_pump_inter_on_off_time = 0;
                            }

                            // 水泵开启2分钟后才检测需求
                            if(pump_time[PM_F_ANY].on < PUMP_AC_NEED_TIME)  
                            {
                                *ptr_fg_ac_need_enable = FALSE;
                            }
                        }   
                    }
                }
                break;
                
            default:                    /* 未选择模式，水泵不开 */
                *ptr_air_pump_dly = 0xff;
                break;
        }
        *ptr_fg_ac_need_enable = TRUE;
    }
    
    if (fg_idle_sta_antifreeze_pump_need    /* 防冻开空调泵 */
    ||  TF_OPEN_PUMP		        /* 商检开空调泵 */
    )
    {
        if (AC_PUMP_OFF_DLY > 1)
        {
            *ptr_air_pump_dly = AC_PUMP_OFF_DLY - 1;
        }
        else
        {
            *ptr_air_pump_dly = 0;
        }
    }
    
    if ((*ptr_air_pump_dly <= AC_PUMP_OFF_DLY) && (!fg_pump_err))     
    {
//        set_output(MAX_PCB, M_DO_PUMP);     /* 开空调泵 */
        *ptr_fg_pump_on = TRUE;
    }
    else           /* 水泵故障 */
    {
//        fg_ac_pm_open = FALSE;
        *ptr_fg_pump_on = FALSE;
    }

//    if (pump_time[PM_F_ANY].on >= TF_VAL(AC_PUMP_ON_DLY, TF_PUMP_ON_DLY) + TF_VAL(val_err_ac_flux_delay, TF_ERR_FLOW_DLY)+2 )   /*30s+15s+2s*/
//    {
//        fg_ac_pm_open = TRUE;        /* 运行时防冻有另外的标志 */
//    }

//    if(debug_mode)    
//    {
//        fg_ac_pm_open = FALSE;//调试状态不做正式运行处理,即不报故障
//        *ptr_fg_pump_on = FALSE;
//    }
#endif
}

#if 0
/*******************************************************************************
 函 数: log_sys_PUMPf()
 功 能: 系统变频水泵控制
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_PUMPf(void)
{
    BOOL fg_open = FALSE;

    /* 不使用系统变频水泵，此处不处理 */
    if (USE_SYS_PUMPF == FALSE)
    {
        return;
    }
    
    if (get_output(MAX_PCB, M_DO_PUMP))
    {
        fg_open = TRUE;
        set_output(MAX_PCB, M_DO_PUMPf);
    }

    /* 变频水泵PID调节 */
    PUMPF_ctrl(fg_open); 
}
#endif

#if 0
/*******************************************************************************
 函 数: log_sys_PUMP_FREQ()
 功 能: 变频水泵控制
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
static U08 cnt_pumpf_act_Period = 0;
static U16 cnt_pumpf_stab = 0;
void log_sys_PUMP_FREQ(void)
{
    #define val_pumpf_pid_calculate_period  8   // 计算周期0.8s
    #define val_pumpf_pid_Kp                8   // 比例系数1.0
    #define val_pumpf_pid_i                 4   // 积分时间50
    #define val_pumpf_pid_d                 0   // 微分时间
    #define val_pumpf_pid_p                 6000 // 反作用比例带
    #define val_pumpf_pid_flexi             10  // 积分系数
    
    PID_ID_ENUM PID_id = PID_ID_PUMPF;
    BOOL fg_open = FALSE;
    static I16 pid_sv;
    static I16 pid_pv;
    static I16 Pid_Residue=0;       // PID调节剩余量
    I32 pid_uk,pid_kc;
    I16 speed_incr = 0;
    I16 speed_min;
    I16 speed_max;
    // 跟随PUMPI动作 且使用变频水泵
    if (get_output(MAX_PCB, M_DO_PUMP)
        && val_PUMPf_used
    )
    {
        fg_open = TRUE;
    }
    
    switch (val_PUMPf_model)
    {
        default:
        case PUMP_FACTORY_WILO:
            speed_min = PUMP_WILO_RPM_MIN;
            speed_max = PUMP_WILO_RPM_MAX;
            break;
        case PUMP_FACTORY_SHIMGE:
            speed_min = PUMP_SHIMGE_RPM_MIN;
            speed_max = PUMP_SHIMGE_RPM_MAX;
            break;
    }
    
    if (fg_open)
    {
        if (val_PUMPf_used == PUMPF_CONTROL_TYPE_SPEED)
        {
            speed_incr = _DIVUCARRY(val_pump_sv, AC_PUMP_ON_DLY);
            pump_speedsv = MIN(MAX(speed_incr*cnt_pumpf_act_Period,speed_min),val_pump_sv);
        }
        else
        {
            if (cnt_pumpf_stab <= val_err_ac_flux_delay+AC_PUMP_ON_DLY)
            {
                speed_incr = _DIVUCARRY(val_pump_rated, AC_PUMP_ON_DLY);
                pump_speedsv = MIN(MAX(speed_incr*cnt_pumpf_act_Period,speed_min),val_pump_rated);
            }
            else
            {
                pid_sv = val_pump_sv_flow;
                pid_pv = pump_flow_pv;
                
                pid_set_ctrl(PID_id, PID_CTRL_INCREMENT);       //增量式
                pid_set_Ts(PID_id, val_pumpf_pid_calculate_period); //计算周期
                pid_set_Kp(PID_id, val_pumpf_pid_Kp);//比例系数
                pid_set_Ti(PID_id, val_pumpf_pid_i);//积分时间
                pid_set_Td(PID_id, val_pumpf_pid_d);//微分时间
                pid_set_Kc(PID_id, val_pumpf_pid_p);//反作用比例带
                pid_set_Ai(PID_id, val_pumpf_pid_flexi);//积分系数
                pid_set_sv(PID_id, &pid_sv);//目标值
                pid_set_pv(PID_id, &pid_pv);//当前值
                
                pid_turn_on(PID_id);
                if (cnt_pumpf_act_Period >= 5)
                {
                    cnt_pumpf_act_Period = 0;
                    pid_uk = pid_read_Uk(PID_id); //PID调节
                    pid_kc = pid_get_Kc(PID_id);

                    speed_incr = _Math_divide(&Pid_Residue,pid_uk*pump_speedsv,pid_kc);
                    speed_incr = fun_pv_limit(speed_incr, -200, 200);   /* 限幅 */
                }
                pump_speedsv -= speed_incr;
                pump_speedsv = fun_pv_limit(pump_speedsv, speed_min, speed_max);
            }
        }
    }
    else
    {
        pid_turn_off(PID_id);
        pump_speedsv = 0;
        cnt_pumpf_act_Period = 0;
        cnt_pumpf_stab = 0;
    }

    if (fg_open)
    {
        set_output(MAX_PCB, M_DO_PUMPf);
    }
}
#endif

/* PUMPo开启条件 */
enum                    
{
    PUMPo_ON_SIG_TEST_RUN_AIR_EMPTY      = 0,  /* 空气排空试运行时开启 */
    PUMPo_ON_SIG_TEST_RUN_PUMP           = 1,  /* 水泵试运行时开启 */
    PUMPo_ON_SIG_FOLLOW_A_RUN            = 2,  /* 跟随A区运行 */
    PUMPo_ON_SIG_IDLE_STA_ANTIFREEZE     = 3,  /* 待机态防冻 */
    PUMPo_ON_SIG_RUN_STA_AC_ANTIFREEZE   = 4,  /* 运行态空调水路防冻 */
    PUMPo_ON_SIG_ZONE_A_TIMED_ANTIFREEZE = 5,  /* A区定时防冻 */
    PUMPo_ON_SIG_MAX,
};
static U16 fg_PUMPo_on[_DIVUCARRY(PUMPo_ON_SIG_MAX, 16)];
/*******************************************************************************
 函 数: log_sys_PUMPo()
 功 能: 外循环泵控制逻辑
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_PUMPo(void)
{
    U08 i;

    /* 空气排空试运行开PUMPo */
    if (GETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPO_ON))
    {
        SETBIT_N(fg_PUMPo_on, PUMPo_ON_SIG_TEST_RUN_AIR_EMPTY);
    }
    else
    {
        CLRBIT_N(fg_PUMPo_on, PUMPo_ON_SIG_TEST_RUN_AIR_EMPTY);
    }

    /* 水泵试运行开PUMPo */
    if (GETBIT_N(fg_test_pump_on, TEST_PUMP_PUMPO_ON))
    {
        SETBIT_N(fg_PUMPo_on, PUMPo_ON_SIG_TEST_RUN_PUMP);
    }
    else
    {
        CLRBIT_N(fg_PUMPo_on, PUMPo_ON_SIG_TEST_RUN_PUMP);
    }

    /* 跟随A区运行 */
    if (!GETBIT_N(fg_test_run, TEST_RUN_AIR_EMPTY)
    && !GETBIT_N(fg_test_run, TEST_RUN_PUMP)
//    && get_output(MAX_PCB, M_DO_PUMP)	/* 2022-9-1删除此条件，空调泵可能有到温停的场景，故不判断空调泵输出 */
//    && MODE_IS_AC                     /* 2022-9-1删除此条件，跟随A区开关，而不是当前控制模式 */
    && fg_zone_A_run
    )
    {
        SETBIT_N(fg_PUMPo_on, PUMPo_ON_SIG_FOLLOW_A_RUN);
    }
    else
    {
        CLRBIT_N(fg_PUMPo_on, PUMPo_ON_SIG_FOLLOW_A_RUN);
    }

    /* 待机态防冻 */
    if (fg_idle_sta_antifreeze)
    {
        SETBIT_N(fg_PUMPo_on, PUMPo_ON_SIG_IDLE_STA_ANTIFREEZE);
    }
    else
    {
        CLRBIT_N(fg_PUMPo_on, PUMPo_ON_SIG_IDLE_STA_ANTIFREEZE);
    }

    /* 运行态空调水路防冻 */
    if (fg_run_sta_ac_antifreeze)
    {
        SETBIT_N(fg_PUMPo_on, PUMPo_ON_SIG_RUN_STA_AC_ANTIFREEZE);
    }
    else
    {
        CLRBIT_N(fg_PUMPo_on, PUMPo_ON_SIG_RUN_STA_AC_ANTIFREEZE);
    }

    /* A区定时防冻 */
    if (fg_zone_A_timed_antifreeze_PUMPo_need)
    {
        SETBIT_N(fg_PUMPo_on, PUMPo_ON_SIG_ZONE_A_TIMED_ANTIFREEZE);
    }
    else
    {
        CLRBIT_N(fg_PUMPo_on, PUMPo_ON_SIG_ZONE_A_TIMED_ANTIFREEZE);
    }

    /* 遍历标志，检查是否需要开启 */
    for (i = 0; i < PUMPo_ON_SIG_MAX; i++)
    {
        if (GETBIT_N(fg_PUMPo_on, i))
        {
            set_output(MAX_PCB, M_DO_PUMPo);
        }
    }
}

/* PUMPm开启条件 */
enum                    
{
    PUMPm_ON_SIG_UI35_ZONE_B_KEY_ON      = 0,  /* 通过线控器B区开关时 */
    PUMPm_ON_SIG_UIRTB_KEY_ON            = 1,  /* 通过温控器B开关时 */
    PUMPm_ON_SIG_IDLE_STA_ANTIFREEZE     = 2,  /* 待机态防冻时 */
    PUMPm_ON_SIG_TEST_RUN_PUMP           = 3,  /* 水泵试运行时 */
    PUMPm_ON_SIG_TEST_RUN_AIR_EMPTY      = 4,  /* 空气排空试运行时 */
    PUMPm_ON_SIG_RUN_STA_AC_ANTIFREEZE   = 5,  /* 运行态空调水路防冻 */
    PUMPm_ON_SIG_ZONE_B_TIMED_ANTIFREEZE = 6,  /* B区定时防冻 */
    PUMPm_ON_SIG_MAX,
};
static U16 fg_PUMPm_on[_DIVUCARRY(PUMPm_ON_SIG_MAX, 16)];
/*******************************************************************************
 函 数: log_sys_PUMPm_need()
 功 能: 
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_PUMPm_need(void)
{
    if (!FG_IDLE_OR_AC_ANTIFREEZE                                   /* 未（待机态防冻 或 运行态空调水路防冻） */
    && !fg_zone_B_timed_antifreeze_PUMPm_need                       /* 无B区定时防冻开混合水泵需求 */
    && (   ((val_pcb_num==1) && MODE_IS_HOTW(SUB_SYS_AC_HOTW))      /* 机组制热水运行 */
        || ((val_pcb_num==1) && MODE_IS_COOL(SUB_SYS_AC_HOTW))      /* 机组制冷运行 */
        || !fg_zone_B_run                                           /* B区未开启 */
        || ac_mode == MODE_COOL                                     /* 空调区域模式为制冷模式 */
       )
    )    
    {
        ARRCLR(fg_PUMPm_on);
        return;
    }

    /* 水泵试运行开PUMPm */
    if (GETBIT_N(fg_test_run, TEST_RUN_PUMP))
    {
        ARRCLR(fg_PUMPm_on);
        if (GETBIT_N(fg_test_pump_on, TEST_PUMP_PUMPM_ON))
        {
            SETBIT_N(fg_PUMPm_on, PUMPm_ON_SIG_TEST_RUN_PUMP);
        }
        else
        {
            CLRBIT_N(fg_PUMPm_on, PUMPm_ON_SIG_TEST_RUN_PUMP);
        }
        return;				/* 特殊运行时，不处理其它情况 */
    }
    else
    {
        CLRBIT_N(fg_PUMPm_on, PUMPm_ON_SIG_TEST_RUN_PUMP);
    }

    /* 空气排空试运行开PUMPm */
    if (GETBIT_N(fg_test_run, TEST_RUN_AIR_EMPTY))
    {
        ARRCLR(fg_PUMPm_on);
        if (GETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPM_ON))
        {
            SETBIT_N(fg_PUMPm_on, PUMPm_ON_SIG_TEST_RUN_AIR_EMPTY);
        }
        else
        {
            CLRBIT_N(fg_PUMPm_on, PUMPm_ON_SIG_TEST_RUN_AIR_EMPTY);
        }
        return;				/* 特殊运行时，不处理其它情况 */
    }
    else
    {
        CLRBIT_N(fg_PUMPm_on, PUMPm_ON_SIG_TEST_RUN_AIR_EMPTY);
    }
    
    /* 线控器开B区 */
    if (USE_UI35_DUAL_ZONE_CTRL
    && fg_zone_B_run
    )
    {
        SETBIT_N(fg_PUMPm_on, PUMPm_ON_SIG_UI35_ZONE_B_KEY_ON);
    }
    else
    {
        CLRBIT_N(fg_PUMPm_on, PUMPm_ON_SIG_UI35_ZONE_B_KEY_ON);
    }

    /* 温控器开B区 */
    if (USE_UIRT_DUAL_ZONE_ON_OFF			                /* 温控器双区控制 */
    && m_get_input(IN_RT_HEAT)                              /* 温控器制热信号 */
    )
    {
        SETBIT_N(fg_PUMPm_on, PUMPm_ON_SIG_UIRTB_KEY_ON);
    }
    else
    {
        CLRBIT_N(fg_PUMPm_on, PUMPm_ON_SIG_UIRTB_KEY_ON);
    }

    /* 待机态防冻 */
    if (fg_idle_sta_antifreeze)
    {
        SETBIT_N(fg_PUMPm_on, PUMPm_ON_SIG_IDLE_STA_ANTIFREEZE);
    }
    else
    {
        CLRBIT_N(fg_PUMPm_on, PUMPm_ON_SIG_IDLE_STA_ANTIFREEZE);
    }

    /* 运行态空调水路防冻 */
    if (fg_run_sta_ac_antifreeze)
    {
        SETBIT_N(fg_PUMPm_on, PUMPm_ON_SIG_RUN_STA_AC_ANTIFREEZE);
    }
    else
    {
        CLRBIT_N(fg_PUMPm_on, PUMPm_ON_SIG_RUN_STA_AC_ANTIFREEZE);
    }

    /* B区定时防冻 */
    if (fg_zone_B_timed_antifreeze_PUMPm_need)
    {
        SETBIT_N(fg_PUMPm_on, PUMPm_ON_SIG_ZONE_B_TIMED_ANTIFREEZE);
    }
    else
    {
        CLRBIT_N(fg_PUMPm_on, PUMPm_ON_SIG_ZONE_B_TIMED_ANTIFREEZE);
    }
}

/*******************************************************************************
 函 数: log_sys_PUMPm()
 功 能: 混合水泵控制
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_PUMPm(void)
{
    U08 i;

    log_sys_PUMPm_need();
    
    /* 遍历标志，检查是否需要开启 */
    for (i = 0; i < PUMPm_ON_SIG_MAX; i++)
    {
        if (GETBIT_N(fg_PUMPm_on, i))
        {
            set_output(MAX_PCB, M_DO_PUMPm);
        }
    } 
}

/* PUMPret开启条件 */
enum                    
{
    PUMPret_ON_SIG_TIMER                   = 0,             /* 定时方式开启时 */
    PUMPret_ON_SIG_DISINFECT               = 1,             /* 杀菌方式开启时 */
    PUMPret_ON_SIG_IDLE_STA_ANTIFREEZE     = 2,             /* 待机态防冻时开始 */
    PUMPret_ON_SIG_RUN_STA_HOTW_ANTIFREEZE = 3,             /* 运行态热水水路防冻时开始 */
    PUMPret_ON_SIG_TIMED_HOTW_ANTIFREEZE   = 4,             /* 热水定时防冻 */
    PUMPret_ON_SIG_MAX,
};
static U16 fg_PUMPret_on[_DIVUCARRY(PUMPret_ON_SIG_MAX, 16)];
/*******************************************************************************
 函 数: log_sys_PUMPret()
 功 能: 下回水泵控制
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_PUMPret(void)
{
    U08 i;
    U16 cnt_PUMPret_on;

    /* PUMPret禁用 */
    if (!val_use_PUMPret    /* 禁用下回水泵 */
    || !val_hotw_used       /* 禁用热水 */
    )   
    {
        ARRCLR(fg_PUMPret_on);
        return;
    }
    
    /* 下回水泵定时 */
    cnt_PUMPret_on = val_PUMPret_run_time;
    if (val_PUMPret_timer	/* 下回水泵定时 */
    && fg_timed_PUMPret_run	/* 下回水泵开启 */
    )
    {
        if (pump_time[PM_RET].on < cnt_PUMPret_on)
        {
            SETBIT_N(fg_PUMPret_on, PUMPret_ON_SIG_TIMER);
        }
        else
        {
            SETCMD_N(TIMED_PUMPRET_OFF);
            CLRBIT_N(fg_PUMPret_on, PUMPret_ON_SIG_TIMER);
        }
    }
    else
    {
        CLRBIT_N(fg_PUMPret_on, PUMPret_ON_SIG_TIMER);
    }
    
    /* 下回水泵杀菌 */
    /* 计算PUMPret的THWt开启温度条件 */
    cnt_PUMPret_on = val_PUMPret_run_time + 5*60;
    if (val_PUMPret_disinfect				/* 杀菌运行 */
    && fg_PUMPret_disinfect_run				/* 进入杀菌 */
    )
    {
        if (pump_time[PM_RET].on < cnt_PUMPret_on)
        {
            SETBIT_N(fg_PUMPret_on, PUMPret_ON_SIG_DISINFECT); 
        }
        else
        {
            SETCMD_N(PUMPRET_DISINFECT_OFF);
            CLRBIT_N(fg_PUMPret_on, PUMPret_ON_SIG_DISINFECT); 
        }
    }
    else
    {
        CLRBIT_N(fg_PUMPret_on, PUMPret_ON_SIG_DISINFECT); 
    }
    
    /* 待机态防冻 */
    if (fg_idle_sta_antifreeze)
    {
        SETBIT_N(fg_PUMPret_on, PUMPret_ON_SIG_IDLE_STA_ANTIFREEZE);
    }
    else
    {
        CLRBIT_N(fg_PUMPret_on, PUMPret_ON_SIG_IDLE_STA_ANTIFREEZE);
    }

    /* 运行态热水水路防冻 */
    if (fg_run_sta_hotw_antifreeze)
    {
        SETBIT_N(fg_PUMPret_on, PUMPret_ON_SIG_RUN_STA_HOTW_ANTIFREEZE);
    }
    else
    {
        CLRBIT_N(fg_PUMPret_on, PUMPret_ON_SIG_RUN_STA_HOTW_ANTIFREEZE);
    }   

    /* 热水定时防冻 */
    if (fg_hotw_timed_antifreeze)
    {
        SETBIT_N(fg_PUMPret_on, PUMPret_ON_SIG_TIMED_HOTW_ANTIFREEZE);
    }
    else
    {
        CLRBIT_N(fg_PUMPret_on, PUMPret_ON_SIG_TIMED_HOTW_ANTIFREEZE);
    } 

    /* 遍历标志，检查是否需要开启 */
    for (i = 0; i < PUMPret_ON_SIG_MAX; i++)
    {
        if (GETBIT_N(fg_PUMPret_on, i))
        {
            set_output(MAX_PCB, M_DO_PUMPret);
        }
    } 
}

/* PUMPsl开启条件 */
enum                    
{
    PUMPsl_ON_SIG_RUN_HOTW_MODE = 0,        /* 制热水运行时开启 */
    PUMPsl_ON_SIG_IDLE_STA_ANTIFREEZE     = 1,              /* 待机态防冻时开启 */
    PUMPsl_ON_SIG_RUN_STA_HOTW_ANTIFREEZE = 2,              /* 运行态热水水路防冻 */
    PUMPsl_ON_SIG_TIMED_HOTW_ANTIFREEZE = 3,                /* 热水定时防冻 */
    PUMPsl_ON_SIG_MAX,
};
static U16 fg_PUMPsl_on[_DIVUCARRY(PUMPsl_ON_SIG_MAX, 16)];
/*******************************************************************************
 函 数: log_sys_PUMPsl()
 功 能: 太阳能水泵控制
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_PUMPsl(void)
{
    U08 i;
    I16 temp_PUMPsl_on, temp_PUMPsl_off;
    /* 保持关闭 */
    if (!val_solar_used                     /* 禁用太阳能 */
    || !val_hotw_used                       /* 禁用热水 */
    || SensorCannotUse(pv_sys_THWt)         /* THWt不可用 */
    )
    {
        ARRCLR(fg_PUMPsl_on);
        return;
    }

    /* 机组制热水运行时处理 */
    if (fg_hotw_run)
    {
        /* 计算开启温度&关闭温度 */
        temp_PUMPsl_on = val_PUMPsl_on_TWHt - val_PUMPsl_THWt_dT;
        temp_PUMPsl_off = val_PUMPsl_on_TWHt;
    
        /* 开启条件 */
        if (pv_sys_THWt < temp_PUMPsl_on)
        {
            if ((!val_TSL_sensor_used&&m_get_input(IN_SOLAR))                       /* 使用太阳能信号 */
            || (val_TSL_sensor_used&&(pv_sys_TSL>val_PUMPsl_on_TSL))                /* 使用太阳能温度 */
            )
            {
                SETBIT_N(fg_PUMPsl_on, PUMPsl_ON_SIG_RUN_HOTW_MODE);
            }
        }
    
        /* 关闭条件 */
        if ((!val_TSL_sensor_used&&!m_get_input(IN_SOLAR))                          /* 使用太阳能信号 */
        || (val_TSL_sensor_used&&(pv_sys_TSL<=val_PUMPsl_on_TSL-val_PUMPsl_TSL_dT)) /* 使用太阳能温度 */
        )
        {
            CLRBIT_N(fg_PUMPsl_on, PUMPsl_ON_SIG_RUN_HOTW_MODE);
        }
    
        /* 关闭条件 */
        if (pv_sys_THWt >= temp_PUMPsl_off)
        {
            CLRBIT_N(fg_PUMPsl_on, PUMPsl_ON_SIG_RUN_HOTW_MODE);
        }
    }
    else
    {
        CLRBIT_N(fg_PUMPsl_on, PUMPsl_ON_SIG_RUN_HOTW_MODE);
    }
    
    /* 待机态防冻 */
    if (val_PUMPsl_antifreeze
    && fg_idle_sta_antifreeze
    )
    {
        SETBIT_N(fg_PUMPsl_on, PUMPsl_ON_SIG_IDLE_STA_ANTIFREEZE);
    }
    else
    {
        CLRBIT_N(fg_PUMPsl_on, PUMPsl_ON_SIG_IDLE_STA_ANTIFREEZE);
    }

    /* 运行态热水水路防冻 */
    if (val_PUMPsl_antifreeze
    && fg_run_sta_hotw_antifreeze
    )
    {
        SETBIT_N(fg_PUMPsl_on, PUMPsl_ON_SIG_RUN_STA_HOTW_ANTIFREEZE);
    }
    else
    {
        CLRBIT_N(fg_PUMPsl_on, PUMPsl_ON_SIG_RUN_STA_HOTW_ANTIFREEZE);
    }

    /* 热水定时防冻 */
    if (val_PUMPsl_antifreeze
    && fg_hotw_timed_antifreeze
    )
    {
        SETBIT_N(fg_PUMPsl_on, PUMPsl_ON_SIG_TIMED_HOTW_ANTIFREEZE);
    }
    else
    {
        CLRBIT_N(fg_PUMPsl_on, PUMPsl_ON_SIG_TIMED_HOTW_ANTIFREEZE);
    }

    /* 遍历标志，检查是否需要开启 */
    for (i = 0; i < PUMPsl_ON_SIG_MAX; i++)
    {
        if (GETBIT_N(fg_PUMPsl_on, i))
        {
            set_output(MAX_PCB, M_DO_PUMPsl);
        }
    } 
}

/*******************************************************************************
 函 数: log_sys_cell_PUMPf_broadcast()
 功 能: 模块变频水泵控制广播
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_cell_PUMPf_broadcast(void)
{
//    U08 pcb_num;
    
    fg_PUMPf_test_run = (GETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPF_ON)/* 空气排空试运行 */
                        || GETBIT_N(fg_test_pump_on, TEST_PUMP_PUMPF_ON)        /* 水泵试运行 */
                        );
    
//    for (pcb_num = 0; pcb_num < val_pcb_num; pcb_num++)
//    {
//        if (GET_SUB_SYS_ON(pcb_num) && fg_pump_on)
//        {
//            if (!M_CM_GetStatus(pcb_num, MAX_COMP, CELL_PUMP_OPEN))
//            {
//                if (!log_sys_cell_pump_can_open(pcb_num))	/* 模块水泵不可开启 */
//                {
//                    continue;        
//                }
//        
//                SETBIT_N(fg_cell_on, pcb_num);
//            }
//        }
//        else
//        {
//            if (M_CM_GetStatus(pcb_num, MAX_COMP, CELL_PUMP_OPEN))
//            {
//                if (!log_sys_cell_pump_can_close(pcb_num))  /* 模块水泵不可关闭 */
//                {
//                    continue;
//                }
//                
//                SETBIT_N(fg_cell_off, pcb_num);
//            }
//        }
//    }
}

/*******************************************************************************
 函 数: log_sys_pump()
 功 能: 水泵控制逻辑包括:
                        内置循环泵、
                        外置循环泵、
                        混合泵、
                        太阳能水泵、
                        变频水泵、
                        生活水箱水泵。
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_pump(void)
{
    log_sys_sub_sys_PUMPf_ctrl(SUB_SYS_AC_HOTW);         /* 空调+热水子系统变频水泵 */
    log_sys_sub_sys_PUMPf_ctrl(SUB_SYS_AC_ONLY);         /* 单空调子系统变频水泵 */
    
    log_sys_PUMPo();        /* 外循环泵 */
    log_sys_PUMPm();        /* 混合水泵 */
    log_sys_PUMPret();      /* 下回水泵 */
    log_sys_PUMPsl();       /* 太阳能水泵 */

    log_sys_cell_PUMPf_broadcast();
}

/*******************************************************************************
 函 数: log_sys_send_fan_cool_elec()
 功 能: 开送风机冷却电热，正常卸载和关机，都需等电热关闭60S过后，才考虑关送风机
 参 数: 空
 返 回: TRUE/FALSE--需要冷却电热/不需要冷却电热
 其 它: 空
*******************************************************************************/
BOOL log_sys_send_fan_cool_elec(void)
{
    BOOL fg_need_cool_elec = FALSE;

    if (STEP_IS_IDLE || STEP_IS_ALARM)
    {
        cnt_elec_stop = 0xff;
    }
    else
    {
        if (cnt_elec_stop < 60) // 电热关闭60S内，不关送风机
        {
            fg_need_cool_elec = TRUE;
        }
    }
    
    
    return fg_need_cool_elec;
}

/*******************************************************************************
 函 数: log_sys_send_fan_avoid_cold()
 功 能: 防冷风，关机后重新开机，需等能量开启后，再开送风机
 参 数: 空
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL log_sys_send_fan_avoid_cold(void)
{
    U08 comp_run_num = 0;
    BOOL fg_need_cool_elec = FALSE;
    static BOOL fg_need_avoid_cold = TRUE;

    // 防冷风不用或者非制热，不需防冷风
    if (val_avoid_cold == UNUSED
//    || ctrl_mode != MODE_HEAT
    || !AIR_IS_AIR)     
    {
        fg_need_avoid_cold = FALSE;
        return fg_need_avoid_cold;
    }

    comp_run_num = log_sys_comp_run_num(SUB_SYS_TYPE_MAX);
    fg_need_cool_elec = log_sys_send_fan_cool_elec();

    if (STEP_IS_IDLE || STEP_IS_ALARM)  // 每次停机后再开启，都需防冷风
    {
        fg_need_avoid_cold = TRUE;
    }
    else
    {
        if (comp_run_num || fg_need_cool_elec)  // 开机后，压机或电热运行后，不再需防冷风
        {
            fg_need_avoid_cold = FALSE;
        }
    }

    return fg_need_avoid_cold;
}

#if 0
/********************************************************
    功能: 处理送风机控制
    注意：
********************************************************/
void log_sys_send_fan(void)
{
    BOOL fg_avoid_cold = FALSE;         // 防冷风
    BOOL fg_send_fan_pause = FALSE;     // 送风机暂停
    U08 comp_run_num = 0;
    BOOL fg_need_cool_elec = FALSE;     // 需要冷却电热标志

    if (!AIR_IS_AIR)    return;
    
    comp_run_num = log_sys_comp_run_num();
    fg_need_cool_elec = log_sys_send_fan_cool_elec();

    fg_avoid_cold = log_sys_send_fan_avoid_cold();
    fg_send_fan_pause = log_sys_send_fan_pause(comp_run_num, fg_need_cool_elec);

    debug_xx[0] = fg_avoid_cold;
    if (fg_avoid_cold)  avoid_cold_fan_cnt = 0;
    
    if (STEP_IS_IDLE || STEP_IS_ALARM)    // 待机或故障，不开送风机
    {
        air_pump_dly = 0xff;
        avoid_cold_fan_cnt = 0xffff;
    }
    else
    {
        if ((status_run == _READY && status_ready > READY_IDLE 
        || status_run == _RUN 
        || status_run == _DELAY && status_delay < DELAY_PUMP))       
        {
            
            if (status_run == _READY)
            {
                if (fg_avoid_cold == FALSE)     air_pump_dly = 0;
            }
            else if (status_run == _RUN)
            {
                // 防冷风时间到，开送风机
                if(avoid_cold_fan_cnt >= val_avoid_cold_fan_time)   
                {
                    air_pump_dly = 0;
                }

                // 满足送风机暂停时 关送风机
                if (fg_send_fan_pause)  
                {
                    air_pump_dly = 0xff;
                }
            }            

            // 除霜停送风机 
            if (log_sys_all_comp_defro())       
            {
                air_pump_dly = 0xff;
            }
        }
    }
    
    if (air_pump_dly <= AC_PUMP_OFF_DLY && !fg_pump_err && !fg_stop_all_mac)
    {
        set_output(MAX_PCB, SW_AC_PM);     /* 开送风机 */
        pump_time[PM_F_ANY].off = 0;
    } 
}
#endif

#if 0
/*******************************************************************************
 函 数: log_sys_AC_is_ready()
 功 能: 送风机/空调泵是否准备好
 参 数: 空
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL log_sys_AC_is_ready(void)
{
    if (AIR_IS_WATER)
    {
        if (fg_ac_pm_open)      return TRUE;
    }
    else
    {
        // 送风机运行延时到或者防冷风，认为送风机准备好了
        if (pump_time[PM_F_ANY].on >= TF_VAL(AC_PUMP_ON_DLY, TF_PUMP_ON_DLY) + 2) 
        {
            return TRUE;
        }
    }
    return FALSE;
}
#endif

/* TBH开启条件 */
enum                    
{
    TBH_ON_SIG_RUN_AIR_HW_NON_PRIORITY     = 0,             /* 制冷/制热运行，制热水非优先时开启 */
    TBH_ON_SIG_RUN_AIR_HW_PRIORITY         = 1,             /* 制冷/制热运行，制热水优先时开启 */
    TBH_ON_SIG_RUN_HOTW_COMP_NORMAL        = 2,             /* 制热水运行，压机正常能调时 */
    TBH_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL   = 3,             /* 制热水运行，压机从正常至保持区时开启 */
    TBH_ON_SIG_RUN_HOTW_COMP_ABNORMAL      = 4,             /* 制热水运行，压机超环温运行范围或故障时 */
    TBH_ON_SIG_RUN_HOTW_COMP_KEEP_ABNORMAL = 5,             /* 制热水运行，压机从异常至保持区时开启 */
    TBH_ON_SIG_DEFROST                     = 6,             /* 除霜时 */
    TBH_ON_SIG_FORCE_HOTW                  = 7,             /* 强开热水模式时开启 */
    TBH_ON_SIG_FORCE_ON                    = 8,             /* 强开水箱电热时开启 */
    TBH_ON_SIG_SMART_GRID_FREE_FORCE_RUN   = 9,             /* 免费用电强制运行时开启 */
    TBH_ON_SIG_SMART_GRID_FREE_RUN         = 10,            /* 免费用电运行时开启 */
    TBH_ON_SIG_DISINFECT                   = 11,            /* 定时杀菌时开启 */
    TBH_ON_SIG_TEST_RUN_HOTW_MODE_ON       = 12,            /* 制热水试运行时开启 */
    TBH_ON_SIG_REMOTE_SWITCH               = 13,            /* 线控水箱电热时开启 */
    TBH_ON_SIG_MAX,
};
static U16 fg_TBH_on[_DIVUCARRY(TBH_ON_SIG_MAX, 16)];
/*******************************************************************************
 函 数: log_sys_TBH_ctrl()
 功 能: 水箱电热控制
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_TBH(void)
{
    #define THWt_MAX          (800)         /* 水箱最高温度 */

    U08 i;
    I16 sv_THWt_TBH_on, sv_THWt_TBH_off; 
    static BOL fg_TA_high = FALSE;

    /* “水箱电热开启延时” */
    U16 TBH_open_dly = val_TBH_open_dly;

    /* 清TBH开启标志 */
    fg_TBH_run = FALSE;

    if (SensorCanUse(pv_sys_TA))
    {
        /* TA >=【水箱电热开环】 */
        if (pv_sys_TA >= val_TBH_on_TA)
        {
            fg_TA_high = TRUE;
        }
        /* TA <【水箱电热开环】-1℃ */
        if (pv_sys_TA < val_TBH_on_TA-10)
        {
            fg_TA_high = FALSE;
        }
    }
    else
    {
        fg_TA_high = FALSE;
    }

    /* 定时杀菌时、处于智能电网状态3和4时不考虑TA */
    if (fg_timed_disinfect_TBH_need
    || FREE_ELEC_RUN                        /* 当前处于智能电网状态3 */
    || FREE_ELEC_FORCE_RUN                  /* 当前处于智能电网状态4 */
    )
    {
        fg_TA_high = FALSE;
    }

    /* 水箱电热保持关闭 */
    if ((!STEP_IS_RUN)		                /* 机组未运行 */
    || (!val_TBH_used)		                /* 无TBH */
    || SensorCannotUse(pv_sys_THWt)			/* THWt不可用 */
    || fg_TA_high                           /* TA高 */
    || !fg_hotw_run			                /* 制热水关闭 */
    || PEAK_ELEC_LIMIT_RUN			        /* 高峰用电限制运行 */
    || (fg_ability_test_run)				/* 能力测试 */
    || m_get_error(MAX_PCB, M_ERR_TBH_OL)	/* 水箱电热过载 */
    )
    {
        ARRCLR(fg_TBH_on);
        return;
    }

#if defined(M_SHENGNENG)
    /* 生能特有逻辑-水箱电热开启延时根据环温分段 */
    if (pv_sys_TA < 50)				/* 5℃环温分段 */
    {
        TBH_open_dly = val_TBH_open_dly;
    }
    else
    {
        TBH_open_dly = QK_60*120;	/* 120min */
    }
#endif

    /* 制冷运行或制热运行时，制热水非优先时处理 */
    if (MODE_IS_AC(SUB_SYS_AC_HOTW)
    && !USE_HOTW_PRIORITY
    && !fg_defrost_run[SUB_SYS_AC_HOTW]
    && !fg_enter_test_run
    )
    {   
        /* 开启/关闭温度计算 */
        sv_THWt_TBH_on = sv_hotw - val_hotw_diff;
        sv_THWt_TBH_off = sv_hotw;
        
        /* TBH开启条件 */
        if (fg_hotw_need
        && pv_sys_THWt < sv_THWt_TBH_on
        )
        {
            SETBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_AIR_HW_NON_PRIORITY);          
        }

        /* TBH关闭条件 */
        if (pv_sys_THWt >= sv_THWt_TBH_off)
        {
            CLRBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_AIR_HW_NON_PRIORITY);
        }
    }
    else
    {
        CLRBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_AIR_HW_NON_PRIORITY);
    }

    /* 制冷运行或制热运行时，制热水优先时处理 */
    if (MODE_IS_AC(SUB_SYS_AC_HOTW)
    && USE_HOTW_PRIORITY
    && !fg_defrost_run[SUB_SYS_AC_HOTW]
    && !fg_enter_test_run
    )
    {
        /* 关闭温度计算 */
        sv_THWt_TBH_off = sv_hotw;

        /* TBH关闭判断 */
        if (pv_sys_THWt >= sv_THWt_TBH_off)
        {
            CLRBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_AIR_HW_PRIORITY);
        }
    }
    else
    {
        CLRBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_AIR_HW_PRIORITY);
    }

    /* 机组制热水运行时处理 */
    if (MODE_IS_HOTW(SUB_SYS_AC_HOTW)
    && !FREE_ELEC_FORCE_RUN
    && !FREE_ELEC_RUN
    && !fg_defrost_run[SUB_SYS_AC_HOTW]
    && !fg_enter_test_run
    )
    {    
        /* 温度计算 */
        sv_THWt_TBH_on = sv_hotw - 10;
        sv_THWt_TBH_off = sv_hotw;
    
        /* TBH开启处理: 压机正常状态 */
        if (fg_ac_hotw_comp_normal)
        {
            CLRBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_HOTW_COMP_KEEP_ABNORMAL);
        
            /* 开启条件 */
            if (FG_AC_NEED                                  /* 有空调需求 */
            && USE_HOTW_PRIORITY                            /* 热水模式优先 */
            && hotw_runtime.run >= TBH_open_dly 			/* 压机运行时间到 */
            && pv_sys_THWt < sv_THWt_TBH_on                 /* THWt满足开启条件 */
            ) 
            {
                SETBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_HOTW_COMP_NORMAL);
                SETBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_AIR_HW_PRIORITY);
                SETBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL);
            }

            /* 开启条件 */
            if (!FG_AC_NEED
            && hotw_runtime.run >= TBH_open_dly
            && pv_sys_THWt < sv_THWt_TBH_on
            )
            {
                SETBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_HOTW_COMP_NORMAL);
                SETBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_AIR_HW_PRIORITY);
                SETBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_AIR_HW_NON_PRIORITY);
                SETBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL);
            }
        }
        else
        {
            CLRBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_HOTW_COMP_NORMAL);
        }

        /* TBH关闭处理: 压机正常状态 */
        if (GETBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_HOTW_COMP_NORMAL)
        || GETBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL)
        )
        {
            /* 关闭条件 */
            if (pv_sys_THWt >= sv_THWt_TBH_off)
            {
                CLRBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_HOTW_COMP_NORMAL);
                CLRBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL);
            }
        }

        /* 温度计算 */
        sv_THWt_TBH_on = sv_hotw - val_hotw_diff;
        sv_THWt_TBH_off = MIN((sv_hotw+val_hotw_diff), THWt_MAX);

        /* TBH开启处理: 压机异常状态 */
        if (fg_ac_hotw_comp_abnormal)
        {
            CLRBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL);
         
            /* 开启条件 */
            if (pv_sys_THWt < sv_THWt_TBH_on)
            {
                SETBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_HOTW_COMP_ABNORMAL);
                SETBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_HOTW_COMP_KEEP_ABNORMAL);
            }
        }
        else
        {
            CLRBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_HOTW_COMP_ABNORMAL);
        }

        /* TBH关闭处理: 压机异常状态 */
        if (GETBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_HOTW_COMP_ABNORMAL)
        || GETBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_HOTW_COMP_KEEP_ABNORMAL)
        )
        {
            /* 关闭条件 */
            if (pv_sys_THWt >= sv_THWt_TBH_off)
            {
                CLRBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_HOTW_COMP_ABNORMAL);
                CLRBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_HOTW_COMP_KEEP_ABNORMAL);
            }
        }
    }
    else
    {
        CLRBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_HOTW_COMP_NORMAL);
        CLRBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL);
        CLRBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_HOTW_COMP_ABNORMAL);
        CLRBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_HOTW_COMP_KEEP_ABNORMAL);
    }
    
    /* 进入除霜时处理 */
    if (fg_defrost_run[SUB_SYS_AC_HOTW])
    {
        /* 温度计算 */
        sv_THWt_TBH_on = sv_hotw - 10;
        sv_THWt_TBH_off = sv_hotw;

        /* TBH开启条件 */
        if (pv_sys_THWt < sv_THWt_TBH_on)
        {
            SETBIT_N(fg_TBH_on, TBH_ON_SIG_DEFROST);
        }
    
        /* TBH关闭条件 */
        if (pv_sys_THWt >= sv_THWt_TBH_off)
        {
            CLRBIT_N(fg_TBH_on, TBH_ON_SIG_DEFROST);
        }
    }
    else
    {
        CLRBIT_N(fg_TBH_on, TBH_ON_SIG_DEFROST);
    }

    /* 强开热水模式命令处理 */
    if (fg_force_hotw_mode_run)
    {
        /* 温度计算 */
        sv_THWt_TBH_on = sv_hotw - 10;
        sv_THWt_TBH_off = sv_hotw;

        /* TBH开启条件 */
        if (pv_sys_THWt < sv_THWt_TBH_on)
        {
            SETBIT_N(fg_TBH_on, TBH_ON_SIG_FORCE_HOTW);
        }
    
        /* TBH关闭条件 */
        if (pv_sys_THWt >= sv_THWt_TBH_off)
        {
            CLRBIT_N(fg_TBH_on, TBH_ON_SIG_FORCE_HOTW);
        }
    }
    else
    {
        CLRBIT_N(fg_TBH_on, TBH_ON_SIG_FORCE_HOTW);
    }
    
    /* 强开水箱电热命令处理 */
    if (fg_force_TBH_run)
    { 
        /* 温度计算 */
        sv_THWt_TBH_on = sv_hotw - 10;
        sv_THWt_TBH_off = sv_hotw;
    
        /* TBH开启条件 */
        if (pv_sys_THWt < sv_THWt_TBH_on)
        {
            SETBIT_N(fg_TBH_on, TBH_ON_SIG_FORCE_ON);
        }
    
        /* TBH关闭条件 */
        if (pv_sys_THWt >= sv_THWt_TBH_off)
        {
            CLRBIT_N(fg_TBH_on, TBH_ON_SIG_FORCE_ON);
            SETCMD_N(FORCE_TBH_OFF);
        }
    }
    else
    {
        CLRBIT_N(fg_TBH_on, TBH_ON_SIG_FORCE_ON);
    }

    /* 免费用电强制运行时处理 */
    if (FREE_ELEC_FORCE_RUN)
    {
        /* 开启/关闭温度计算 */
        sv_THWt_TBH_on = 690;
        sv_THWt_TBH_off = 700;

        /* TBH开启条件 */
        if (pv_sys_THWt < sv_THWt_TBH_on)
        {
            SETBIT_N(fg_TBH_on, TBH_ON_SIG_SMART_GRID_FREE_FORCE_RUN);
        }

        /* TBH关闭条件 */
        if (pv_sys_THWt >= sv_THWt_TBH_off)
        {
            CLRBIT_N(fg_TBH_on, TBH_ON_SIG_SMART_GRID_FREE_FORCE_RUN);
        }
    }
    else
    {
        CLRBIT_N(fg_TBH_on, TBH_ON_SIG_SMART_GRID_FREE_FORCE_RUN);
    }

    /* 免费用电运行时处理 */
    if (FREE_ELEC_RUN)
    {
        /* 开启/关闭温度计算 */
        sv_THWt_TBH_on = sv_hotw - 20;
        sv_THWt_TBH_off = sv_hotw + 30;

        /* TBH开启条件 */
        if (pv_sys_THWt < sv_THWt_TBH_on)
        {
            SETBIT_N(fg_TBH_on, TBH_ON_SIG_SMART_GRID_FREE_RUN);
        }

        /* TBH关闭条件 */
        if (pv_sys_THWt >= sv_THWt_TBH_off)
        {
            CLRBIT_N(fg_TBH_on, TBH_ON_SIG_SMART_GRID_FREE_RUN);
        }
    }
    else
    {
        CLRBIT_N(fg_TBH_on, TBH_ON_SIG_SMART_GRID_FREE_RUN);
    }

    /* 定时杀菌时处理 */
    if (fg_timed_disinfect_TBH_need
    && SensorCanUse(pv_sys_THWt)
    )
    {
        /* 开启/关闭温度计算 */
        sv_THWt_TBH_on = val_ster_temp - 20;
        sv_THWt_TBH_off= MIN(700, val_ster_temp+10);
    
        /* TBH开启条件 */
        if (pv_sys_THWt <= sv_THWt_TBH_on
        && cnt_ster_cyc < val_ster_max_cycle
        && cnt_ster_high_temp < val_ster_high_temp_Time
        )
        {
            SETBIT_N(fg_TBH_on, TBH_ON_SIG_DISINFECT);
        }

        /* TBH关闭条件 */
        if (cnt_ster_cyc >= val_ster_max_cycle
        || cnt_ster_high_temp >= val_ster_high_temp_Time
        || pv_sys_THWt >= sv_THWt_TBH_off
        )
        {
            CLRBIT_N(fg_TBH_on, TBH_ON_SIG_DISINFECT);
        }
    }
    else
    {
        CLRBIT_N(fg_TBH_on, TBH_ON_SIG_DISINFECT);
    }

    /* 制热水试运行时处理 */
    if (fg_test_run_hotw_TBH_run)
    {
        SETBIT_N(fg_TBH_on, TBH_ON_SIG_TEST_RUN_HOTW_MODE_ON);
    }
    else
    {
        CLRBIT_N(fg_TBH_on, TBH_ON_SIG_TEST_RUN_HOTW_MODE_ON);
    }

    /* 线控水箱电热时处理 */           
    if (REMOTE_IS_TBH		/* 线控水箱电热 */
    && (cnt_LDIrm_dly > 3)	/* 检测到LDIrm，且持续3秒 */
    )
    {
        /* 温度计算 */
        sv_THWt_TBH_on = sv_hotw - 10;
        sv_THWt_TBH_off = sv_hotw;

        /* TBH开启条件 */
        if (pv_sys_THWt < sv_THWt_TBH_on)
        {
            SETBIT_N(fg_TBH_on, TBH_ON_SIG_REMOTE_SWITCH);
        }
    
        /* TBH关闭条件 */
        if (pv_sys_THWt >= sv_THWt_TBH_off)
        {
            CLRBIT_N(fg_TBH_on, TBH_ON_SIG_REMOTE_SWITCH);
        }
    }
    else
    {
        CLRBIT_N(fg_TBH_on, TBH_ON_SIG_REMOTE_SWITCH);
    }
    
    /* 遍历标志，检查是否需要开启 */
    for (i = 0; i < TBH_ON_SIG_MAX; i++)
    {
        if (GETBIT_N(fg_TBH_on, i))
        {
            set_output(MAX_PCB, M_DO_TBH);
            fg_TBH_run = TRUE;
        }
    } 
}

/*******************************************************************************
 函 数: log_sys_check_force_IBH_off()
 功 能: 检查各模块强开辅助电热后是否可关闭
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_check_force_IBH_off(void)
{
    U08 i;

    /* 遍历所有模块，若存在强开辅助电热可关，则关闭所有辅助电热 */
    for (i=0; i<val_pcb_num; i++)
    {
        if (M_CM_GetStatus(i, MAX_COMP, CELL_FORCE_IBH_OFF))
        {
            SETCMD_N(FORCE_IBH_OFF);
            break;
        }
    }
}

/*******************************************************************************
 函 数: log_sys_all_hotw_water_flow_err()
 功 能: 热水模块全报水流不足故障判断
 参 数: 空
 返 回: TRUE/FALSE--全部热水模块报水流不足故障/非全部热水模块报水流不足故障
 其 它: 空
*******************************************************************************/
BOL log_sys_all_hotw_water_flow_err(void)
{
    U08 i;
    
    /* 遍历热水模块水流不足故障 */
    for (i = PCB_BGN(SUB_SYS_AC_HOTW); i < PCB_END(SUB_SYS_AC_HOTW); ++i)
    {
        if (!m_get_error(i, S_ERR_WATER_FLOW))
        {
            /* 存在一个不报故障，则返回FALSE */
            return FALSE;
        }
    }

    return TRUE;
}

/* AHS开启条件 */
enum                    
{
    AHS_ON_SIG_RUN_HEAT_COMP_NORMAL        = 0,				/* 制热运行，压机正常能调时开启 */
    AHS_ON_SIG_RUN_HEAT_COMP_KEEP_NORMAL   = 1,				/* 制热运行，压机从正常至保持区时开启 */
    AHS_ON_SIG_RUN_HEAT_COMP_ABNORMAL      = 2,				/* 制热运行，压机超环温运行范围或故障时开启 */
    AHS_ON_SIG_RUN_HEAT_COMP_KEEP_ABNORMAL = 3,				/* 制热运行，压机从异常至保持区时开启 */
    AHS_ON_SIG_RUN_HOTW_COMP_NORMAL        = 4,				/* 制热水运行，压机正常能调时开启 */
    AHS_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL   = 5,				/* 制热水运行，压机从正常至保持区时开启 */
    AHS_ON_SIG_RUN_HOTW_COMP_ABNORMAL      = 6,				/* 制热水运行，压机超环温运行范围或故障时开启 */
    AHS_ON_SIG_RUN_HOTW_COMP_KEEP_ABNORMAL = 7,				/* 制热水运行，压机从异常至保持区时开启 */
    AHS_ON_SIG_DEFROST                     = 8,				/* 除霜时开启 */
    AHS_ON_SIG_FORCE_HOTW                  = 9,				/* 强开热水模式时开启 */
    AHS_ON_SIG_RUN_HEAT_FORCE_ON           = 10,			/* 制热运行，强开外部热源时开启 */
    AHS_ON_SIG_RUN_HOTW_FORCE_ON           = 11,			/* 制热水运行，强开外部热源时开启 */
    AHS_ON_SIG_DISINFECT                   = 12,			/* 定时杀菌时开启 */
    AHS_ON_SIG_RUN_HEAT_REMOTE_SWITCH      = 13,			/* 制热运行，线控外部热源控制时开启 */
    AHS_ON_SIG_RUN_HOTW_REMOTE_SWITCH      = 14,			/* 制热水运行，线控外部热源控制时开启 */
    AHS_ON_SIG_MAX,
};
static U16 fg_AHS_on[_DIVUCARRY(AHS_ON_SIG_MAX, 16)];
/*******************************************************************************
 函 数: log_sys_AHS()
 功 能: 外部热源控制
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_AHS(void)
{
    U08 i;
    U08 sub_sys_type;
    I16 sv_TA_AHS_on, sv_TA_AHS_off;
    I16 sv_TWout3_AHS_on, sv_TWout3_AHS_off;
    I16 sv_THWt_AHS_on, sv_THWt_AHS_off;  
    I16 pv_TWout3, pv_TACt, pv_ctrl_ac;

    pv_TWout3   = pv_sys_TWout3;
    pv_TACt     = fun_pv_avg(pv_sys_TACt1, pv_sys_TACt2);
    pv_ctrl_ac  = USE_AC_TEMP_IN_CTRL ? pv_TACt : pv_TWout3;	/* 空调控制温度 */

    fg_AHS_run = FALSE;                     /* 清AHS开启标志 */
    fg_timed_disinfect_AHS_on = FALSE;      /* 清杀菌AHS开启标志 */

    /* 外部热源保持关闭 */
    if (!val_TWout3_sensor_used                              /* 禁用TWout3 */
    || !val_use_AHS                                          /* 禁用AHS */
    || (MODE_IS_HEAT(SUB_SYS_AC_HOTW) && !HEAT_USE_AHS)      /* 制热运行且AHS仅制热水使用 */
    || (MODE_IS_HOTW(SUB_SYS_AC_HOTW) && !HOTW_USE_AHS)      /* 制热水运行且AHS仅制热使用 */
    || ((val_pcb_num==1) && MODE_IS_COOL(SUB_SYS_AC_HOTW))   /* 制冷模式 */
    || SensorCannotUse(pv_TWout3)                            /* TWout3不可用 */
    || (SensorCanUse(pv_TWout3)&&(pv_TWout3>=700))           /* TWout3>=70℃ */
    || (fg_TBH_run && MODE_IS_HOTW(SUB_SYS_AC_HOTW))         /* TBH开启时 */
    || log_sys_all_hotw_water_flow_err()                     /* 热水模块全部水流不足故障 */
    || fg_ability_test_run                                   /* 能力测试 */
    || fg_floor_preheat_run                                  /* 地暖预热 */
    || fg_floor_dryup_run	                                 /* 地暖烘干 */
    )
    {
        ARRCLR(fg_AHS_on);
        return;
    }

    /* 机组制热运行 */
    if (MODE_IS_HEAT(SUB_SYS_AC_HOTW)
    && !fg_defrost_run[SUB_SYS_AC_HOTW]
    && fg_heat_need			                /* 制热需求 */
    //&& !fg_timed_disinfect_run
    )
    {
        /* AHS开启/关闭温度计算 */
        sv_TA_AHS_on = val_AHS_open_env - 20;
        sv_TA_AHS_off = val_AHS_open_env;
        sv_TWout3_AHS_on = sv_temp_freq[SUB_SYS_AC_HOTW] - val_AHS_open_diff;
        sv_TWout3_AHS_off = sv_temp_freq[SUB_SYS_AC_HOTW] + val_AHS_close_diff;
 
        /* AHS开启处理: 压机正常状态 */
        if (fg_ac_hotw_comp_normal)
        {
            CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HEAT_COMP_KEEP_ABNORMAL);
        
            /* AHS开启条件 */
            if (pv_sys_TA < sv_TA_AHS_on
            && pv_ctrl_ac <= sv_TWout3_AHS_on
            && ((AHS_time.on>0 && AHS_time.off==0) || AHS_time.off >= val_AHS_open_dly)
            )
            {
                SETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HEAT_COMP_NORMAL);
                SETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HEAT_COMP_KEEP_NORMAL);
            }
        }
        else
        {
            CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HEAT_COMP_NORMAL);
        }

        /* AHS关闭处理: 压机正常状态 */
        if (GETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HEAT_COMP_NORMAL)
        || GETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HEAT_COMP_KEEP_NORMAL)
        )
        {
            /* AHS关闭条件 */
            if (pv_sys_TA >= sv_TA_AHS_off
            || pv_ctrl_ac >= sv_TWout3_AHS_off
            )
            {
                CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HEAT_COMP_NORMAL);
                CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HEAT_COMP_KEEP_NORMAL);
            }
        }

        /* AHS开启/关闭温度计算 */
        sv_TWout3_AHS_on = sv_temp_freq[SUB_SYS_AC_HOTW] - 30;
        sv_TWout3_AHS_off = MIN(sv_temp_freq[SUB_SYS_AC_HOTW]+val_sub_diff_energy[SUB_SYS_AC_HOTW], 700);

        /* AHS开启处理: 压机异常状态 */
        if (fg_ac_hotw_comp_abnormal)
        {
            CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HEAT_COMP_KEEP_NORMAL);

            /* AHS开启条件 */
            if (pv_ctrl_ac <= sv_TWout3_AHS_on
            && ((AHS_time.on>0 && AHS_time.off==0) || AHS_time.off >= 5*60)
            )
            {
                SETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HEAT_COMP_ABNORMAL);
                SETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HEAT_COMP_KEEP_ABNORMAL);
            }
        }
        else
        {
            CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HEAT_COMP_ABNORMAL);
        }

        /* AHS关闭处理: 压机异常状态 */
        if (GETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HEAT_COMP_ABNORMAL)
        || GETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HEAT_COMP_KEEP_ABNORMAL)
        )
        {
            /* AHS关闭条件 */
            if (pv_ctrl_ac >= sv_TWout3_AHS_off)
            {
                CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HEAT_COMP_ABNORMAL);
                CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HEAT_COMP_KEEP_ABNORMAL);
            }
        }
    }
    else
    {
        CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HEAT_COMP_NORMAL);
        CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HEAT_COMP_KEEP_NORMAL);
        CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HEAT_COMP_ABNORMAL);
        CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HEAT_COMP_KEEP_ABNORMAL);
    }

    /* 机组制热水运行 */
    if (MODE_IS_HOTW(SUB_SYS_AC_HOTW)
    && !fg_defrost_run[SUB_SYS_AC_HOTW]
    && !fg_timed_disinfect_AHS_need
    && fg_hotw_need
    )
    {            
        /* AHS开启/关闭温度计算 */
        sv_THWt_AHS_on = 400;
        sv_THWt_AHS_off = sv_hotw;
        sv_TWout3_AHS_on = 700 - val_AHS_open_diff;
        sv_TWout3_AHS_off = 700;

        /* AHS开启处理: 压机正常状态 */
        if (fg_ac_hotw_comp_normal)
        {
            CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_COMP_KEEP_ABNORMAL);
			
            /* AHS开启条件 */
            if (pv_sys_THWt < sv_THWt_AHS_on
            && pv_TWout3 <= sv_TWout3_AHS_on
            && ((AHS_time.on>0 && AHS_time.off==0) || AHS_time.off >= val_AHS_open_dly)
            )
            {
                SETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_COMP_NORMAL);
                SETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL);			
            } 
        }
        else
        {
            CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_COMP_NORMAL);
        }

        /* AHS关闭处理: 压机正常状态 */
        if (GETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_COMP_NORMAL)
        || GETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL)
        )
        {
            /* AHS关闭条件 */
            if (pv_sys_THWt > sv_THWt_AHS_off
            || pv_TWout3 >= sv_TWout3_AHS_off
            )
            {
                CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_COMP_NORMAL);
                CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL);
            }
        }

        /* AHS开启/关闭温度计算 */
        sv_THWt_AHS_on = sv_hotw - val_hotw_diff;
        sv_THWt_AHS_off = sv_hotw;
        sv_TWout3_AHS_on = 700 - val_AHS_open_diff;
        sv_TWout3_AHS_off = 700;

        /* AHS开启处理: 压机异常状态 */
        if (fg_ac_hotw_comp_abnormal)
        {
            CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL);
			
            /* AHS开启条件 */
            if (pv_sys_THWt < sv_THWt_AHS_on
            && pv_TWout3 <= sv_TWout3_AHS_on
            && ((AHS_time.on>0 && AHS_time.off==0) || AHS_time.off >= 5*60)
            )
            {
            	SETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_COMP_ABNORMAL);
            	SETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_COMP_KEEP_ABNORMAL);				
            }
        }
        else
        {
            CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_COMP_ABNORMAL);
        }

        /* AHS关闭处理: 压机异常状态 */
        if (GETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_COMP_ABNORMAL)
        || GETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_COMP_KEEP_ABNORMAL)
        )
        {
            /* AHS关闭条件 */
            if (pv_sys_THWt > sv_THWt_AHS_off
            || pv_TWout3 >= sv_TWout3_AHS_off
            )
            {
                CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_COMP_ABNORMAL);
                CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_COMP_KEEP_ABNORMAL);
            }
        }
    }
    else
    {
        CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_COMP_NORMAL);
        CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL);
        CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_COMP_ABNORMAL);
        CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_COMP_KEEP_ABNORMAL);
    }

    /* 进入除霜时处理 */
    if (fg_defrost_run[SUB_SYS_AC_HOTW])
    {
        /* AHS开启/关闭温度计算 */
        sv_TWout3_AHS_on = 160;
        sv_TWout3_AHS_off = 200;   

        if (pv_TWout3 <= sv_TWout3_AHS_on)
        {
            SETBIT_N(fg_AHS_on, AHS_ON_SIG_DEFROST);
        }

        if (pv_TWout3 >= sv_TWout3_AHS_off)
        {
            CLRBIT_N(fg_AHS_on, AHS_ON_SIG_DEFROST);
        }
    }
    else
    {
        CLRBIT_N(fg_AHS_on, AHS_ON_SIG_DEFROST);
    }

    /* 快速制热水命令处理 */
    if (fg_force_hotw_mode_run)
    {
        /* AHS开启温度计算 */
        sv_THWt_AHS_on = sv_hotw;
            
        if (pv_sys_THWt < sv_THWt_AHS_on)
        {
            SETBIT_N(fg_AHS_on, AHS_ON_SIG_FORCE_HOTW);
        }
        else
        {
            CLRBIT_N(fg_AHS_on, AHS_ON_SIG_FORCE_HOTW);
        }
    }
    else
    {
        CLRBIT_N(fg_AHS_on, AHS_ON_SIG_FORCE_HOTW);
    }

    /* 制热运行，强开外部热源命令处理 */
    if (fg_force_AHS_run
    && RUN_HEAT(SUB_SYS_AC_HOTW)
    )
    { 
        /* AHS开启/关闭温度计算 */
        sv_TWout3_AHS_on = sv_temp_freq[SUB_SYS_AC_HOTW] - 20;
        sv_TWout3_AHS_off = sv_temp_freq[SUB_SYS_AC_HOTW] + val_sub_diff_energy[SUB_SYS_AC_HOTW];

        /* AHS开启条件 */
        if (pv_ctrl_ac < sv_TWout3_AHS_on)
        {
            SETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HEAT_FORCE_ON);
        }

        /* AHS关闭条件 */
        if (pv_ctrl_ac >= sv_TWout3_AHS_off)
        {
            CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HEAT_FORCE_ON);
            SETCMD_N(FORCE_AHS_OFF);
        }
    }
    else
    {
        CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HEAT_FORCE_ON); 
    }

    /* 制热水运行，强开外部热源命令处理 */
    if (fg_force_AHS_run
    && RUN_HOTW(SUB_SYS_AC_HOTW)
    )
    {    
        /* AHS开启/关闭温度计算 */
        sv_THWt_AHS_on = sv_hotw - 10;
        sv_THWt_AHS_off = sv_hotw;
        
        /* AHS开启条件 */
        if (pv_sys_THWt < sv_THWt_AHS_on)
        {
            SETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_FORCE_ON);
        }
        
        /* AHS关闭条件 */
        if (pv_sys_THWt >= sv_THWt_AHS_off)
        {
            CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_FORCE_ON);
            SETCMD_N(FORCE_AHS_OFF);
        }
    }
    else
    {
        CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_FORCE_ON); 
    }

    /* 定时杀菌时处理 */
    if (fg_timed_disinfect_run				/* 定时杀菌运行 */
    && SensorCanUse(pv_sys_THWt)			/* THWt可用 */
    )
    {
        #define DISINFECT_STAGE_1   1		/* fg_timed_disinfect_AHS_need==FALSE */
        #define DISINFECT_STAGE_2   2		/* fg_timed_disinfect_AHS_need==TRUE */
    
        //static BOL disinfect_stage_1_to_2 = FALSE;		/* 杀菌阶段1过渡到2标志，查询后复位 */
        static BOL disinfect_stage_2_to_1 = FALSE;		/* 杀菌阶段2过渡到1标志，查询后复位，正常使用不会触发 */
        static U08 disinfect_stage_bak    = DISINFECT_STAGE_1;		/* 杀菌阶段副本 */
        
        /* AHS开启/关闭温度计算 */
        sv_THWt_AHS_on = val_ster_temp - 20;
        sv_THWt_AHS_off= MIN(700, val_ster_temp+10);
        sv_TWout3_AHS_on = 700 - val_AHS_open_diff;
        sv_TWout3_AHS_off = 700;

        /* 杀菌阶段跨越判断 */
        if (!fg_timed_disinfect_AHS_need)	/* 阶段1 */
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

        /* 阶段跨越补丁：若阶段1开了AHS，则认为是定时杀菌的AHS需求 */
        if (disinfect_stage_bak == DISINFECT_STAGE_1)/* 阶段1 */
        {
        	/* 杀菌阶段1热水开了AHS */
            if (GETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_COMP_NORMAL)
            ||  GETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL)
        	||	GETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_COMP_ABNORMAL)
        	||  GETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_COMP_KEEP_ABNORMAL))
        	{
        	    /* 认为阶段1开的AHS也是杀菌的AHS需求 */
        	    SETBIT_N(fg_AHS_on, AHS_ON_SIG_DISINFECT);
        	}

        	/* 阶段2到1跨越时关闭AHS */
        	if (disinfect_stage_2_to_1 == TRUE)
        	{
        	    disinfect_stage_2_to_1 = FALSE;
                CLRBIT_N(fg_AHS_on, AHS_ON_SIG_DISINFECT);
        	}

            /* 阶段1也要防止大火力导致超温，防止极端情况： */
            /* 定时杀菌，开压机没到“最小运行时间COMP_NEED_ON_TIME，温度超杀菌温度 */
        	/* AHS关闭条件 */
            if (cnt_ster_cyc >= val_ster_max_cycle
            || cnt_ster_high_temp >= val_ster_high_temp_Time
            || pv_sys_THWt >= sv_THWt_AHS_off
            || pv_TWout3 >= sv_TWout3_AHS_off
            )
            {
                CLRBIT_N(fg_AHS_on, AHS_ON_SIG_DISINFECT);
            }
        }
        else				                        /* 阶段2 */
        {
            /* AHS开启条件 */
            if (pv_sys_THWt <= sv_THWt_AHS_on
            && pv_TWout3 <= sv_TWout3_AHS_on
            && cnt_ster_cyc < val_ster_max_cycle
            && cnt_ster_high_temp < val_ster_high_temp_Time
            )
            {
                SETBIT_N(fg_AHS_on, AHS_ON_SIG_DISINFECT);
            }

            /* AHS关闭条件 */
            if (cnt_ster_cyc >= val_ster_max_cycle
            || cnt_ster_high_temp >= val_ster_high_temp_Time
            || pv_sys_THWt >= sv_THWt_AHS_off
            || pv_TWout3 >= sv_TWout3_AHS_off
            )
            {
                CLRBIT_N(fg_AHS_on, AHS_ON_SIG_DISINFECT);
            }
        }
    }
    else
    {
        CLRBIT_N(fg_AHS_on, AHS_ON_SIG_DISINFECT);
    }

    /* 制热运行，线控外部热源控制时开启 */
    if (REMOTE_IS_AHS		/* 线控外部热源 */
    && (cnt_LDIrm_dly > 3)	/* 检测到LDIrm，且持续3秒 */
    && RUN_HEAT(SUB_SYS_AC_HOTW)
    )
    { 
        /* AHS开启/关闭温度计算 */
        sv_TWout3_AHS_on = sv_temp_freq[SUB_SYS_AC_HOTW] - 20;
        sv_TWout3_AHS_off = sv_temp_freq[SUB_SYS_AC_HOTW] + val_sub_diff_energy[SUB_SYS_AC_HOTW];

        /* AHS开启条件 */
        if (pv_ctrl_ac < sv_TWout3_AHS_on)
        {
            SETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HEAT_REMOTE_SWITCH);
        }

        /* AHS关闭条件 */
        if (pv_ctrl_ac >= sv_TWout3_AHS_off)
        {
            CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HEAT_REMOTE_SWITCH);
        }
    }
    else
    {
        CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HEAT_REMOTE_SWITCH); 
    }

    /* 制热水运行，线控外部热源控制时开启 */
    if (REMOTE_IS_AHS		/* 线控外部热源 */
    && (cnt_LDIrm_dly > 3)	/* 检测到LDIrm，且持续3秒 */
    && RUN_HOTW(SUB_SYS_AC_HOTW)
    )
    {    
        /* AHS开启/关闭温度计算 */
        sv_THWt_AHS_on = sv_hotw - 10;
        sv_THWt_AHS_off = sv_hotw;
        
        /* AHS开启条件 */
        if (pv_sys_THWt < sv_THWt_AHS_on)
        {
            SETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_REMOTE_SWITCH);
        }
        
        /* AHS关闭条件 */
        if (pv_sys_THWt >= sv_THWt_AHS_off)
        {
            CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_REMOTE_SWITCH);
        }
    }
    else
    {
        CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_REMOTE_SWITCH); 
    }

    /* 输出 */
    sub_sys_type = (energy_ctrl_type == GROUP_ENERGY_CTRL) ? SUB_SYS_AC_HOTW : SUB_SYS_TYPE_MAX;
    if (log_sys_have_cell_pump_run(sub_sys_type))
    {
        /* 遍历标志，检查是否需要开启 */
        for (i = 0; i < AHS_ON_SIG_MAX; i++)
        {
            if (GETBIT_N(fg_AHS_on, i))
            {
                set_output(MAX_PCB, M_DO_AHS);
                fg_AHS_run = TRUE;
            }
        }
    }

    /* 更新独立标志，供模块使用 */
    fg_timed_disinfect_AHS_on = GETBIT_N(fg_AHS_on, AHS_ON_SIG_DISINFECT);    
}

#if 0
/*******************************************************************************
 函 数: log_sys_can_enter_timed_disinfect()
 功 能: 定时杀菌功能是否可进入
 参 数: 空
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOL log_sys_can_enter_timed_disinfect(void)
{
    /* 定时杀菌功能无效 */
    if (!val_hotw_used                      /* 无热水功能 */
    || !val_timed_ster_fun                  /* 无定时杀菌功能 */
    || SensorCannotUse(pv_sys_THWt)         /* THWt探头故障 */
    || PEAK_ELEC_LIMIT_RUN                  /* 高峰用电限制运行 */
    || fg_floor_dryup_run                   /* 进入地暖烘干功能 */
    ) 
    {
        return FALSE;
    }

    return TRUE;
}
#endif

/*******************************************************************************
 函 数: log_sys_timed_disinfect()
 功 能: 定时杀菌功能
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
void log_sys_timed_disinfect(void)
{   
    I16 temp_PUMPret_on;
    static BOL fg_on_off_remember = FALSE;
    static BOL fg_need_stop = FALSE;
    static BOL fg_enter_stage_two = FALSE;
    static BOL fg_has_open_PUMRret = FALSE;
    static BOL fg_has_enter_HA_disinfect = FALSE;			/* 假日离开模式已进入杀菌 */

    temp_PUMPret_on = val_TBH_used ? (val_ster_temp - 20) : 580; 

    /* 未进入假日离家模式 */
    if (!fg_holiday_away_run)
    {
        fg_has_enter_HA_disinfect = FALSE;
    }

    /* 1. 命令处理 */
    if (GETCMD_N(TIMED_DISINFECT_ON))		/* 开启: 定时杀菌功能 */
    {
        /* 1.1 定时杀菌功能可进入 */
        if (val_hotw_used                   /* 使用热水功能 */
        && val_timed_ster_fun               /* 使用定时杀菌功能 */
        && SensorCanUse(pv_sys_THWt)		/* THWt探头正常 */
        && !PEAK_ELEC_LIMIT_RUN             /* 非高峰用电限制运行 */
        && !fg_floor_dryup_run              /* 未进入地暖烘干功能 */
        && !fg_floor_preheat_run            /* 未进入地暖预热功能 */
        && !fg_has_enter_HA_disinfect		/* 未进入假日离开杀菌 */
        ) 
        {            
            fg_timed_disinfect_run = TRUE;
            /* 热水未运行，则热水开机 */
            if (!fg_hotw_run)
            {
                SETCMD_N(HOTW_ON);
            }

            /* 进入假日离开模式后，定时杀菌处理只进行一次 */
            if (fg_holiday_away_run)
            {
               fg_has_enter_HA_disinfect = TRUE;
            }
        }

        /* 1.2 进入定时杀菌功能前，记忆开关机状态 */
        if (fg_timed_disinfect_run && !fg_on_off_remember)
        {
            fg_on_off_remember = TRUE;
            
            /* 进入定时杀菌功能前处于运行状态，不执行关机 */
            if (status_run == _READY || status_run == _RUN)
            {
                fg_need_stop = fg_hotw_run ? FALSE : TRUE;		/* 若热水开机，则无需热水关机 */
            }
            /* 进入定时杀菌功能前处于待机或停机状态，执行关机 */
            else if (status_run == _IDLE || status_run == _DELAY)
            {
                fg_need_stop = TRUE;
            }
        }
    }
    if (GETCMD_N(TIMED_DISINFECT_OFF))		/* 关闭: 定时杀菌功能 */
    {
        fg_timed_disinfect_run = FALSE;
        
        /* 定时杀菌功能结束后，执行热水关机 */
        if (fg_need_stop)
        {
            SETCMD_N(HOTW_OFF);
            fg_need_stop = FALSE;
        }
    }
    
    /* 2. 未进入定时杀菌功能处理 */
    if (!fg_timed_disinfect_run)
    {
        fg_on_off_remember = FALSE;
        fg_need_stop = FALSE;
        fg_has_open_PUMRret = FALSE;
        fg_timed_disinfect_TBH_need = FALSE;
        fg_timed_disinfect_IBH_need = FALSE;
        fg_timed_disinfect_AHS_need = FALSE;
        SETCMD_N(PUMPRET_DISINFECT_OFF);
        return;
    }

    /* 3. 判断杀菌阶段是否为阶段2 */
    if (!STEP_IS_RUN                                                    /* 未进入运行态 */
    || (STEP_IS_RUN && status_ready >= READY_PUMP)                      /* 补丁: 从启动态切换至运行态瞬间 */
    || log_sys_have_non_defrosting_comp_run(MODE_NULL, SUB_SYS_AC_HOTW) /* 有可制热水的非除霜模块的压机运行 */
    || (MODE_IS_HOTW(SUB_SYS_AC_HOTW) && fg_energy_need[SUB_SYS_AC_HOTW])    /* 空调+热水子系统运行热水模式，且有能量需求 */
    )
    {
        fg_enter_stage_two = FALSE;
    }
    else
    {
        fg_enter_stage_two = TRUE;
    }
    
    /* 4. 判断TBH或IBH是否有开启需求 */
    fg_timed_disinfect_TBH_need = FALSE;
    fg_timed_disinfect_IBH_need = FALSE;
    fg_timed_disinfect_AHS_need = FALSE;
    if (fg_enter_stage_two)
    {
        if (val_TBH_used)
        {
            fg_timed_disinfect_TBH_need = TRUE;
        }
        else if (HOTW_USE_IBH)
        {
            fg_timed_disinfect_IBH_need = TRUE;
        }
        else if (HOTW_USE_AHS)
        {
            fg_timed_disinfect_AHS_need = TRUE;
        }
    }

    /* 5. 判断杀菌计时 */
    if ((cnt_ster_cyc >= val_ster_max_cycle)                /* 杀菌时间 >=【杀菌最大周期】 */
    || (cnt_ster_high_temp >= val_ster_high_temp_Time)      /* 高温杀菌时间 >=【杀菌高温时间】 */
    )
    {
        SETCMD_N(TIMED_DISINFECT_OFF);
    }

    /* 6. 触发下回水泵杀菌 */
    if (SensorCanUse(pv_sys_THWt)			/* THWt可用 */
    && pv_sys_THWt >= temp_PUMPret_on		/* THWt >= 下回水泵杀菌开启温度 */
    && !fg_has_open_PUMRret	                /* 进入定时杀菌功能后首次开启PUMPret */
    )
    {
        fg_has_open_PUMRret = TRUE;
        SETCMD_N(PUMPRET_DISINFECT_ON);
    }
}

/*******************************************************************************
 函 数: log_sys_can_enter_force_hotw()
 功 能: 强开热水功能是否可进入
 参 数: 空
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOL log_sys_can_enter_force_hotw(void)
{
    if (!val_hotw_used                      /* 无热水功能 */
    || SensorCannotUse(pv_sys_THWt)         /* THWt探头故障 */
    || fg_force_hotw_mode_run		                /* TBH已开 */
    )
    {
        return FALSE;
    }

    return TRUE;
}

/*******************************************************************************
 函 数: log_sys_force_hotw()
 功 能: 强开热水模式功能
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_force_hotw(void)
{   
    /* 未进入强开热水模式功能 */
    if (!fg_force_hotw_mode_run)
    {
        return;
    }

    if (SensorCanUse(pv_sys_THWt)
    && (pv_sys_THWt >= sv_hotw)
    )
    {
        SETCMD_N(FORCE_HOTW_OFF);
    }
}

/*******************************************************************************
 函 数: log_sys_floor_dryup()
 功 能: 地暖烘干控制
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
static I16 dryup_sv_temp = SENSOR_NOEXIST;
void log_sys_floor_dryup(void)
{
    I16 tmp;
    U08 day;
    static U08 day_bak = 0;
    static U08 cnt_day = 1;
    day = clock[hmi_t_num].day;     /* 日 */

    /* 1. 命令处理 */
    if (GETCMD_N(FLOOR_DRYUP_ON))			/* 开启: 地暖烘干功能 */
    {
        if (val_TFLin_sensor_used			/* 使用TFLin */
        && HEAT_USE_FLH		                /* 制热末端存在地暖 */
        && (STEP_IS_IDLE||STEP_IS_DELAY)	/* 待机或停机 */
        && !fg_floor_preheat_run	        /* 未进入地暖预热功能 */
        )
        {
            fg_floor_dryup_run = TRUE;
            if (ZONE_A_HEAT_USE_FLH)
            {
                SETCMD_N(ZONE_A_ON);		/* A区开机 */
            }
            if (ZONE_B_HEAT_USE_FLH)
            {
                SETCMD_N(ZONE_B_ON);		/* B区开机 */
            }
        }
    }
    if (GETCMD_N(FLOOR_DRYUP_OFF))			/* 关闭: 地暖烘干功能 */
    {
        fg_floor_dryup_run = FALSE;
        
        SETCMD_N(ZONE_A_OFF);				/* A区关机 */
        SETCMD_N(ZONE_B_OFF);				/* B区关机 */
    }

    /* 2. 运行时处理 */
    if (fg_floor_dryup_run)
    {       
        /* 计算烘干温度 */
        if (cnt_day < val_floor_dry_head)
        {
            tmp = _DIVUCARRY(val_floor_dry_temp - 250, val_floor_dry_head);
            dryup_sv_temp = 250 + (cnt_day-1)*tmp;
            dryup_sv_temp = MIN(dryup_sv_temp, val_floor_dry_temp);
        }
        else if (cnt_day < (val_floor_dry_head + val_floor_dry_mid))
        {
            dryup_sv_temp = val_floor_dry_temp;
        }
        else if (cnt_day < (val_floor_dry_head + val_floor_dry_mid + val_floor_dry_tail))
        {
            tmp = _DIVUCARRY(val_floor_dry_temp - 300, val_floor_dry_tail);
            dryup_sv_temp = val_floor_dry_temp - (cnt_day-val_floor_dry_head-val_floor_dry_mid)*tmp;
            dryup_sv_temp = MAX(dryup_sv_temp, 300);
        }
        else				/* 结束烘干 */
        {
            SETCMD_N(FLOOR_DRYUP_OFF);
        }
    
        if ((!log_sys_have_comp_run(MODE_NULL, SUB_SYS_TYPE_MAX)&&(log_sys_cell_is_err()))/* 由于故障导致所有压机不能运行 */
        && !HEAT_USE_IBH                                                /* 无辅助电热可用 */
        && !HEAT_USE_AHS	                                            /* 无外部热源可用 */
        )
        {
            SETCMD_N(FLOOR_DRYUP_OFF);
        }

        /* 累计天数 */
        if (day_bak != day) 
        {
            day_bak = day;
            _CNT(cnt_day);
        }
    }
    else
    {
        day_bak = day;
        cnt_day = 1;
    }
}

/*******************************************************************************
 函 数: log_sys_floor_preheat()
 功 能: 地暖预热功能
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_floor_preheat(void)
{
    /* 1. 命令处理 */
    if (GETCMD_N(FLOOR_PREHEAT_ON))			/* 开启: 地暖预热功能 */
    {
        if (val_TFLin_sensor_used			/* 使用TFLin */
        && HEAT_USE_FLH                     /* 制热末端存在地暖 */
        && (STEP_IS_IDLE || STEP_IS_DELAY)	/* 待机或停机 */
        && !fg_floor_dryup_run				/* 未进入地暖烘干功能 */
        )
        {
            fg_floor_preheat_run = TRUE;
            if (ZONE_A_HEAT_USE_FLH)
            {
                SETCMD_N(ZONE_A_ON);		/* A区开机 */
            }
            if (ZONE_B_HEAT_USE_FLH)
            {
                SETCMD_N(ZONE_B_ON);		/* B区开机 */
            }
        }
    }
    if (GETCMD_N(FLOOR_PREHEAT_OFF))		/* 关闭: 地暖预热功能 */
    {
        fg_floor_preheat_run = FALSE;

        SETCMD_N(ZONE_A_OFF);				/* A区关机 */
        SETCMD_N(ZONE_B_OFF);				/* B区关机 */
    }

    /* 2. 运行时处理 */
    if (fg_floor_preheat_run)
    {
        if (floor_preheat_time.run > val_floor_preheat_max*60)
        {
            SETCMD_N(FLOOR_PREHEAT_OFF);
        }
    }
}


/*******************************************************************************
 函 数: log_sys_holiday_away()
 功 能: 假日离家模式
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_holiday_away(void)
{

}

#if 0
/*******************************************************************************
 函 数: log_sys_holiday()
 功 能: 休假模式
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_holiday(void)
{
    RTC start_date,end_date;
    static BOOL fg_enter_holiday_bak = FALSE; 

    if (fg_enter_holiday_bak != fg_enter_holiday)
    {
        fg_enter_holiday_bak = fg_enter_holiday;
        if (!fg_enter_holiday)
        {
            dm603_holiday_tbl[HOLIDAY_USED] = 0;
            DataAccess_holiday(DATA_WRITE);
        }
    }

    if (dm603_holiday_tbl[HOLIDAY_USED])
    {
        VALCLR(start_date);
        start_date.year = dm603_holiday_tbl[HOLIDAY_START_YEAR];
        start_date.month = dm603_holiday_tbl[HOLIDAY_START_MONTH];
        start_date.day = dm603_holiday_tbl[HOLIDAY_START_DAY];
        VALCLR(end_date);
        end_date.year = dm603_holiday_tbl[HOLIDAY_END_YEAR];
        end_date.month = dm603_holiday_tbl[HOLIDAY_END_MONTH];
        end_date.day = dm603_holiday_tbl[HOLIDAY_END_DAY];
        if ((fun_time_diff_day(&clock[hmi_t_num], &start_date) <= 0)  // 处于休假期间
            && (fun_time_diff_day(&clock[hmi_t_num], &end_date) > 0)
        )
        {
            if (!fg_timed_disinfect_run)   // 等待杀菌结束后才进入
            {
                fg_enter_holiday = TRUE;
            }
        }
        else if (fun_time_diff_day(&clock[hmi_t_num], &end_date) <= 0)
        {
            fg_enter_holiday = FALSE;
        }
    }
    else
    {
        fg_enter_holiday = FALSE;
    }

//    switch (val_air_type)
    {
//        case HOT_WATER_ONLY:    // 热水机，运行制热水
//            holiday_mode = MODE_HOTW;
//            holiday_sv = dm603_holiday_tbl[HOLIDAY_HW_SET]*10;
//            break;
//        default:
            holiday_mode = MODE_HEAT;
            holiday_sv = dm603_holiday_tbl[HOLIDAY_HEAT_SET]*10;
//            break;
    }
}

/*******************************************************************************
 函 数: log_sys_holiday_on_off()
 功 能: 休假模式开关机
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_holiday_on_off(void)
{
    static BOOL fg_enter_holiday_bak = FALSE;
    static BOOL fg_need_stop = FALSE;
    static BOOL fg_need_juge = TRUE;

    if (fg_need_juge)
    {
        // 进入休假前处于运行态，不执行关机
        if (status_run == _READY || status_run == _RUN)
        {
            fg_need_stop = FALSE;
        }
        // 进入休假前处于待机，执行关机
        else if (status_run == _IDLE || status_run == _DELAY)
        {
            fg_need_stop = TRUE;
        }
    }

    if (fg_enter_holiday_bak != fg_enter_holiday)
    {
        fg_enter_holiday_bak = fg_enter_holiday;
        
        if (fg_enter_holiday)
        {
            fg_need_juge = FALSE;
            if (status_run == _IDLE || status_run == _DELAY)
            {
                Logic_machine_start();    /* 执行开机操作 */
            }
        }
        else
        {
            fg_need_juge = TRUE;
            if ((status_run == _READY || status_run == _RUN) && fg_need_stop)
            {
                Logic_machine_stop();     /* 执行关机操作 */
            }
        }
    }
}
#endif 

/*******************************************************************************
 函 数: log_sys_ability_test()
 功 能: 能力测试运行逻辑
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_ability_test(void)
{
    if (para_AT_use_get() == AT_UNUSED)	/* 禁用时 */
    {
        /* 从能力测试恢复自由跑 */
        fg_ability_test_run = FALSE;

        /* 空读，把之前记忆的命令吃掉，否则切换能力测试选择时可能会触发开启命令 */
        GETCMD_N(ABILITY_TEST_ON);
    }
    else
    {
        /* 1. 命令处理 */
        if (GETCMD_N(ABILITY_TEST_ON))			/* 开启: 能力测试运行 */
        {
    //        /* 待机或停机状态下，允许进入能力测试 */
    //        if (status_run == _IDLE || status_run == _DELAY)
            {
                fg_ability_test_run = TRUE;
                SETCMD_N(ZONE_A_ON);			/* A区开机 */
            }
        }
        if (GETCMD_N(ABILITY_TEST_OFF))			/* 关闭: 能力测试运行 */
        {
            if (fg_ability_test_run)
            {
                fg_ability_test_run = FALSE;
    //            SETCMD_N(ZONE_A_OFF);				/* A区关机 */
            }
        }

        /* 2. 运行时处理 */
        if (fg_ability_test_run)
        {
            switch (ctrl_mode[SUB_SYS_AC_HOTW])
            {
                case MODE_COOL:
                    if (!SensorCanUse(AC_NRG_CTRL_TEMP)
                    || (AC_NRG_CTRL_TEMP <= EXIT_ABILITY_TEST_COOL_SV)
                    )
                    {
                        SETCMD_N(ABILITY_TEST_OFF);
                    }
                    break;
                default:
                    if (!SensorCanUse(AC_NRG_CTRL_TEMP)
                    || (AC_NRG_CTRL_TEMP >= EXIT_ABILITY_TEST_HEAT_SV)
                    )
                    {
                        SETCMD_N(ABILITY_TEST_OFF);
                    }
                    break;
            }
        }
    }
}
 
/* ========================================================================================== */
/* S3V1开启条件 */
enum                    
{
    S3V1_ON_SIG_TEST_RUN_AIR_EMPTY      = 0,  /* 空气排空试运行时 */
    S3V1_ON_SIG_TEST_RUN_PUMP           = 1,  /* 水泵试运行时 */
    S3V1_ON_SIG_HW_MODE_NEED            = 2,  /* 有机组制热水需求时 */
    S3V1_ON_SIG_TIMED_DISINFECT         = 3,  /* 定时杀菌时 */
    S3V1_ON_SIG_IDLE_STA_ANTIFREEZE     = 4,  /* 待机态防冻时 */
    S3V1_ON_SIG_RUN_STA_HOTW_ANTIFREEZE = 5,  /* 运行态热水水路防冻 */
    S3V1_ON_SIG_HOTW_TIMED_ANTIFREEZE   = 6,  /* 热水定时防冻 */
    S3V1_ON_SIG_MAX,
};
static U16 fg_S3V1_on[_DIVUCARRY(S3V1_ON_SIG_MAX, 16)];
/*******************************************************************************
 函 数: log_sys_S3V1()
 功 能: 电磁三通阀SV1，切换水路
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_S3V1(void)
{
    U08 i;

    /* 空气排空试运行开S3V1 */
    if (GETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V1_ON)
    )
    {
        SETBIT_N(fg_S3V1_on, S3V1_ON_SIG_TEST_RUN_AIR_EMPTY);
    }
    else
    {
        CLRBIT_N(fg_S3V1_on, S3V1_ON_SIG_TEST_RUN_AIR_EMPTY);
    }

    /* 水泵试运行开S3V1 */
    if (GETBIT_N(fg_test_pump_on, TEST_PUMP_S3V1_ON)
    )
    {
        SETBIT_N(fg_S3V1_on, S3V1_ON_SIG_TEST_RUN_PUMP);
    }
    else
    {
        CLRBIT_N(fg_S3V1_on, S3V1_ON_SIG_TEST_RUN_PUMP);
    }

    /* 制热水运行开S3V1 */
    if (STEP_IS_RUN
    && MODE_IS_HOTW(SUB_SYS_AC_HOTW)
    && !fg_air_empty_test_run
    && !fg_pump_test_run
    )
    {
        /* 有制热水需求 */
        SETBIT_N(fg_S3V1_on, S3V1_ON_SIG_HW_MODE_NEED);
    }
    else
    {
        CLRBIT_N(fg_S3V1_on, S3V1_ON_SIG_HW_MODE_NEED);
    }

    /* 定时杀菌时开S3V1 */
    if (fg_timed_disinfect_run)
    {
        SETBIT_N(fg_S3V1_on, S3V1_ON_SIG_TIMED_DISINFECT);
    }
    else
    {
        CLRBIT_N(fg_S3V1_on, S3V1_ON_SIG_TIMED_DISINFECT);
    }
    
    /* 待机态防冻 */
    if (fg_idle_sta_antifreeze)
    {
        /* 防冻时先清空其他需求，交由防冻控制 */
        ARRCLR(fg_S3V1_on);	

        if (idle_sta_antifreeze_stage == AF_STAGE_HOTW)
        {
            SETBIT_N(fg_S3V1_on, S3V1_ON_SIG_IDLE_STA_ANTIFREEZE);
        }
        else
        {
            CLRBIT_N(fg_S3V1_on, S3V1_ON_SIG_IDLE_STA_ANTIFREEZE);
        }
    }
    else
    {
        CLRBIT_N(fg_S3V1_on, S3V1_ON_SIG_IDLE_STA_ANTIFREEZE);
    }

    /* 运行态热水水路防冻 */
    if (fg_run_sta_hotw_antifreeze)
    {
        /* 防冻时先清空其他需求，交由防冻控制 */
        ARRCLR(fg_S3V1_on);	

        if (run_sta_antifreeze_stage == AF_STAGE_HOTW)
        {
            SETBIT_N(fg_S3V1_on, S3V1_ON_SIG_RUN_STA_HOTW_ANTIFREEZE);
        }
        else
        {
            CLRBIT_N(fg_S3V1_on, S3V1_ON_SIG_RUN_STA_HOTW_ANTIFREEZE);
        }
    }
    else
    {
        CLRBIT_N(fg_S3V1_on, S3V1_ON_SIG_RUN_STA_HOTW_ANTIFREEZE);
    }

    /* A区定时防冻 */
    if (fg_zone_A_timed_antifreeze_S3V1_ON_need)
    {
        ARRCLR(fg_S3V1_on);
    }

    /* B区定时防冻 */
    if (fg_zone_B_timed_antifreeze_S3V1_ON_need)
    {
        ARRCLR(fg_S3V1_on);
    }

    /* 热水定时防冻 */
    if (fg_hotw_timed_antifreeze_S3V1_OFF_need)
    {
        SETBIT_N(fg_S3V1_on, S3V1_ON_SIG_HOTW_TIMED_ANTIFREEZE);
    }
    else
    {
        CLRBIT_N(fg_S3V1_on, S3V1_ON_SIG_HOTW_TIMED_ANTIFREEZE);
    }

    /* 遍历标志，检查是否需要开启 */
    fg_S3V1_is_hw = FALSE;
    for (i = 0; i < S3V1_ON_SIG_MAX; i++)
    {
        if (GETBIT_N(fg_S3V1_on, i))
        {
            set_output(MAX_PCB, M_DO_S3V1);
            fg_S3V1_is_hw = TRUE;
        }
    } 
}

/* S3V2开启条件 */
enum                    
{
    S3V2_ON_SIG_TEST_RUN_AIR_EMPTY = 0,     /* 空气排空试运行时开启 */
    S3V2_ON_SIG_TEST_RUN_PUMP      = 1,		/* 水泵试运行时开启 */
    S3V2_ON_SIG_RUN_HT_MODE        = 2,		/* 制热运行时开启 */
    S3V2_ON_SIG_IDLE_STA_ANTIFREEZE   = 3,	/* 待机态防冻运行时开启 */
    S3V2_ON_SIG_RUN_STA_AC_ANTIFREEZE = 4,	/* 运行态空调水路防冻时开启 */
    S3V2_ON_SIG_TIMED_AC_ANTIFREEZE = 5,	/* A区定时防冻 */
    S3V2_ON_SIG_MAX,
};
static U16 fg_S3V2_on[_DIVUCARRY(S3V2_ON_SIG_MAX, 16)];
/*******************************************************************************
 函 数: log_sys_S3V2()
 功 能: 电磁三通阀S3V2
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_S3V2(void)
{
    U08 i;

    /* S3V2保持关闭 */
    if (!GETBIT_N(fg_test_run, TEST_RUN_AIR_EMPTY)  /* 未进入空气排空试运行 */
    && !GETBIT_N(fg_test_run, TEST_RUN_PUMP)        /* 未进入水泵试运行 */
    && USE_COOL_ONLY                                /* 仅制冷 */
    )
    {
        ARRCLR(fg_S3V2_on);
        return;
    }

    /* 空气排空试运行开S3V2 */
    if (GETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V2_ON)
    )
    {
        SETBIT_N(fg_S3V2_on, S3V2_ON_SIG_TEST_RUN_AIR_EMPTY);
    }
    else
    {
        CLRBIT_N(fg_S3V2_on, S3V2_ON_SIG_TEST_RUN_AIR_EMPTY);
    }

    /* 水泵试运行开S3V2 */
    if (GETBIT_N(fg_test_pump_on, TEST_PUMP_S3V2_ON)
    )
    {
        SETBIT_N(fg_S3V2_on, S3V2_ON_SIG_TEST_RUN_PUMP);
    }
    else
    {
        CLRBIT_N(fg_S3V2_on, S3V2_ON_SIG_TEST_RUN_PUMP);
    }

    /* 制热开S3V2 */
    if (ac_mode == MODE_HEAT
    && !fg_air_empty_test_run
    && !fg_pump_test_run
    )
    {
        SETBIT_N(fg_S3V2_on, S3V2_ON_SIG_RUN_HT_MODE);
//        if (!fg_heat_need)
//        {
//            CLRBIT_N(fg_S3V2_on, S3V2_ON_SIG_RUN_HT_MODE);
//        }
    }
    else
    {
        CLRBIT_N(fg_S3V2_on, S3V2_ON_SIG_RUN_HT_MODE);
    }

    /* 待机态防冻 */
    if (fg_idle_sta_antifreeze)
    {
        /* 截取一个周期计时 */
        U16 cnt_freeze_cycle = freeze_act_time%ANTI_PRE_DLY;

        /* 防冻时先清空其他需求，交由防冻控制 */
        ARRCLR(fg_S3V2_on);	

        /* 50%占空比轮流输出 */
        if (cnt_freeze_cycle < ANTI_PRE_DLY/2)
        {
            SETBIT_N(fg_S3V2_on, S3V2_ON_SIG_IDLE_STA_ANTIFREEZE);
        }
        else
        {
            CLRBIT_N(fg_S3V2_on, S3V2_ON_SIG_IDLE_STA_ANTIFREEZE);
        }
    }
    else
    {
        CLRBIT_N(fg_S3V2_on, S3V2_ON_SIG_IDLE_STA_ANTIFREEZE);
    }

    /* 运行态空调水路防冻 */
    if (fg_run_sta_ac_antifreeze)
    {
        /* 截取一个周期计时 */
        U16 cnt_freeze_cycle = freeze_act_time%ANTI_PRE_DLY;

        /* 防冻时先清空其他需求，交由防冻控制 */
        ARRCLR(fg_S3V2_on);	

        /* 50%占空比轮流输出 */
        if (cnt_freeze_cycle < ANTI_PRE_DLY/2)
        {
            SETBIT_N(fg_S3V2_on, S3V2_ON_SIG_RUN_STA_AC_ANTIFREEZE);
        }
        else
        {
            CLRBIT_N(fg_S3V2_on, S3V2_ON_SIG_RUN_STA_AC_ANTIFREEZE);
        }
    }
    else
    {
        CLRBIT_N(fg_S3V2_on, S3V2_ON_SIG_RUN_STA_AC_ANTIFREEZE);
    }

    /* A区定时防冻 */
    if (fg_zone_A_timed_antifreeze)
    {
        /* 截取一个周期计时 */
        U16 cnt_freeze_cycle = freeze_act_time%ANTI_PRE_DLY;

        /* 防冻时先清空其他需求，交由防冻控制 */
        ARRCLR(fg_S3V2_on);	

        /* 50%占空比轮流输出 */
        if (cnt_freeze_cycle < ANTI_PRE_DLY/2)
        {
            SETBIT_N(fg_S3V2_on, S3V2_ON_SIG_TIMED_AC_ANTIFREEZE);
        }
        else
        {
            CLRBIT_N(fg_S3V2_on, S3V2_ON_SIG_TIMED_AC_ANTIFREEZE);
        }
    }
    else
    {
        CLRBIT_N(fg_S3V2_on, S3V2_ON_SIG_TIMED_AC_ANTIFREEZE);
    }

    /* 遍历标志，检查是否需要开启 */
    for (i = 0; i < S3V2_ON_SIG_MAX; i++)
    {
        if (GETBIT_N(fg_S3V2_on, i))
        {
            set_output(MAX_PCB, M_DO_S3V2);
        }
    } 
}

/* S3V3开启条件 */
enum                    
{
    S3V3_ON_SIG_UI35_DUAL_WATER_ZONE_B_KEY_ON  = 0,			/* 线控器双区水温控制-B区开启时 */
    S3V3_ON_SIG_UI35_DUAL_HYBRID_ZONE_B_KEY_ON = 1,			/* 线控器双区混合控制-B区开启时 */
    S3V3_ON_SIG_UIRT_DUAL_ON_OFF_UIRTB_KEY_ON  = 2,			/* 温控器双区控制-温控器B开关开启时 */
    S3V3_ON_SIG_TEST_RUN_AIR_EMPTY             = 3,			/* 空气排空试运行时开启 */
    S3V3_ON_SIG_IDLE_STA_ANTIFREEZE            = 4,			/* 待机态防冻时开启 */
    S3V3_ON_SIG_TEST_RUN_PUMP                  = 5,			/* 水泵试运行时开启 */
    S3V3_ON_SIG_RUN_STA_AC_ANTIFREEZE          = 6,			/* 运行态空调水路防冻时 */
    S3V3_ON_SIG_ZONE_B_TIMED_ANTIFREEZE        = 7,			/* B区定时防冻时开启 */
    S3V3_ON_SIG_MAX,
};
static U16 fg_S3V3_on[_DIVUCARRY(S3V3_ON_SIG_MAX, 16)];
/*******************************************************************************
 函 数: log_sys_S3V3()
 功 能: 电磁三通阀S3V3(B区地暖)
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_S3V3(void)
{
    U08 i;
    I16 ctrl_temp_S3V3, temp_S3V3_on, temp_S3V3_off;
    BOOL is_overheat_protect;

    /* 选S3V3控制温度、计算S3V3开启&关闭温度 */
    if (fg_sv_temp_energy_overwrite_heat)	/* 特殊运行 */
    {
        /* 使用TFLin判断 */
        ctrl_temp_S3V3 = pv_sys_TFLin;
        temp_S3V3_on = sv_temp_energy_heat - val_S3V3_on_TFLin_dT;
        temp_S3V3_off = sv_temp_energy_heat;
    }
    else if (USE_UI35_DUAL_ZONE_HYBRID_CTRL)
    {
        /* 使用TR判断 */
        ctrl_temp_S3V3 = pv_sys_TR;
        temp_S3V3_on = sv_room_heat_zone_B - val_S3V3_on_TR_dT;
        temp_S3V3_off = sv_room_heat_zone_B;
    }
    else
    {
        /* 使用TFLin判断 */
        ctrl_temp_S3V3 = pv_sys_TFLin;
        temp_S3V3_on = sv_heat_zone_B - val_S3V3_on_TFLin_dT;
        temp_S3V3_off = sv_heat_zone_B;
    }

    /* 地暖过热保护 */
    is_overheat_protect = (pv_sys_TFLin>=val_TFLin_protect) ? TRUE : FALSE;

    /* TFLin禁用:无该逻辑功能 */
    if (!val_TFLin_sensor_used)
    {
        ARRCLR(fg_S3V3_on);
        return;
    }

    /* S3V3保持关闭 */
    if (!FG_IDLE_OR_AC_ANTIFREEZE                   /* 非（待机态防冻 或 运行态空调水路防冻） */
    && !fg_zone_B_timed_antifreeze			        /* 非B区定时防冻 */
    && !GETBIT_N(fg_test_run, TEST_RUN_AIR_EMPTY)   /* 未进入空气排空试运行 */
    && !GETBIT_N(fg_test_run, TEST_RUN_PUMP)        /* 未进入水泵试运行 */
    && (   !STEP_IS_RUN                             /* 机组未运行 */
        || !fg_zone_B_run                           /* B区未开启 */
        || ac_mode == MODE_COOL                     /* 空调区域模式为制冷模式 */
        || !SensorCanUse(ctrl_temp_S3V3)            /* S3V3控制温度不可用 */
        )
    )
    {
        ARRCLR(fg_S3V3_on);
        return;
    }

    /* 地暖过热保持关闭 */
    if (is_overheat_protect)
    {
        ARRCLR(fg_S3V3_on);
        return;
    }

    /* 线控器双区水温控制 */
    if (USE_UI35_DUAL_ZONE_WATER_TEMP_CTLR
    && !fg_air_empty_test_run
    && !fg_pump_test_run
    )
    {
        /* S3V3开启 */
        if (ctrl_temp_S3V3 <= temp_S3V3_on
        &&  cnt_S3V3_in_circue_s >= S3V3_in_circue_min_time
        )
        {
            SETBIT_N(fg_S3V3_on, S3V3_ON_SIG_UI35_DUAL_WATER_ZONE_B_KEY_ON);
        }

        /* S3V3关闭 */
        if ((ctrl_temp_S3V3>temp_S3V3_off && cnt_S3V3_ext_circue_s >= val_S3V3_ext_circue_min_time)
        || is_overheat_protect
        )     
        {
            /* 确保由开到关只执行一次 */
            if (GETBIT_N(fg_S3V3_on, S3V3_ON_SIG_UI35_DUAL_WATER_ZONE_B_KEY_ON))
            {
                S3V3_in_circue_min_time = MAX(val_S3V3_act_period-cnt_S3V3_ext_circue_s, S3V_MIN_OFF);
            }
            
            CLRBIT_N(fg_S3V3_on, S3V3_ON_SIG_UI35_DUAL_WATER_ZONE_B_KEY_ON);
        }
    }
    else
    {
        CLRBIT_N(fg_S3V3_on, S3V3_ON_SIG_UI35_DUAL_WATER_ZONE_B_KEY_ON);
    }
    
    /* 线控器双区混合控制 */
    if (USE_UI35_DUAL_ZONE_HYBRID_CTRL
    && !fg_air_empty_test_run
    && !fg_pump_test_run
    )
    {
        /* S3V3开启 */
        if (ctrl_temp_S3V3 <= temp_S3V3_on
        &&  cnt_S3V3_in_circue_s >= S3V3_in_circue_min_time
        )
        {
            SETBIT_N(fg_S3V3_on, S3V3_ON_SIG_UI35_DUAL_HYBRID_ZONE_B_KEY_ON);
        }

        /* S3V3关闭 */
        if ((ctrl_temp_S3V3>temp_S3V3_off && cnt_S3V3_ext_circue_s >= val_S3V3_ext_circue_min_time)
        || is_overheat_protect
        )     
        {
            /* 确保由开到关只执行一次 */
            if (GETBIT_N(fg_S3V3_on, S3V3_ON_SIG_UI35_DUAL_HYBRID_ZONE_B_KEY_ON))
            {
                S3V3_in_circue_min_time = MAX(val_S3V3_act_period-cnt_S3V3_ext_circue_s, S3V_MIN_OFF);
            }
            
            CLRBIT_N(fg_S3V3_on, S3V3_ON_SIG_UI35_DUAL_HYBRID_ZONE_B_KEY_ON);
        }
    }
    else
    {
        CLRBIT_N(fg_S3V3_on, S3V3_ON_SIG_UI35_DUAL_HYBRID_ZONE_B_KEY_ON);
    }

    /* 温控器双区控制 */
    if (USE_UIRT_DUAL_ZONE_ON_OFF
    && !fg_air_empty_test_run
    && !fg_pump_test_run
    )
    {
        /* S3V3开启 */
        if (ctrl_temp_S3V3 <= temp_S3V3_on
        &&  cnt_S3V3_in_circue_s >= S3V3_in_circue_min_time
        )
        {
            SETBIT_N(fg_S3V3_on, S3V3_ON_SIG_UIRT_DUAL_ON_OFF_UIRTB_KEY_ON);
        }

        /* S3V3关闭 */
        if ((ctrl_temp_S3V3>temp_S3V3_off && cnt_S3V3_ext_circue_s >= val_S3V3_ext_circue_min_time)
        || is_overheat_protect
        )     
        {
            /* 确保由开到关只执行一次 */
            if (GETBIT_N(fg_S3V3_on, S3V3_ON_SIG_UIRT_DUAL_ON_OFF_UIRTB_KEY_ON))
            {
                S3V3_in_circue_min_time = MAX(val_S3V3_act_period-cnt_S3V3_ext_circue_s, S3V_MIN_OFF);
            }
            
            CLRBIT_N(fg_S3V3_on, S3V3_ON_SIG_UIRT_DUAL_ON_OFF_UIRTB_KEY_ON);
        }
    }
    else
    {
        CLRBIT_N(fg_S3V3_on, S3V3_ON_SIG_UIRT_DUAL_ON_OFF_UIRTB_KEY_ON);
    }

    /* 待机态防冻 */
    if (fg_idle_sta_antifreeze)
    {
        /* 截取一个周期计时 */
        U16 cnt_freeze_cycle = freeze_act_time%ANTI_PRE_DLY;

        /* 防冻时先清空其他需求，交由防冻控制 */
        ARRCLR(fg_S3V3_on);	

        /* 50%占空比轮流输出 */
        if (cnt_freeze_cycle >= ANTI_PRE_DLY/2)
        {
            SETBIT_N(fg_S3V3_on, S3V3_ON_SIG_IDLE_STA_ANTIFREEZE);
        }
        else
        {
            CLRBIT_N(fg_S3V3_on, S3V3_ON_SIG_IDLE_STA_ANTIFREEZE);
        }
    }
    else
    {
        CLRBIT_N(fg_S3V3_on, S3V3_ON_SIG_IDLE_STA_ANTIFREEZE);
    }

    /* 运行态空调水路防冻 */
    if (fg_run_sta_ac_antifreeze)
    {
        /* 截取一个周期计时 */
        U16 cnt_freeze_cycle = freeze_act_time%ANTI_PRE_DLY;

        /* 防冻时先清空其他需求，交由防冻控制 */
        ARRCLR(fg_S3V3_on);	

        /* 50%占空比轮流输出 */
        if (cnt_freeze_cycle >= ANTI_PRE_DLY/2)
        {
            SETBIT_N(fg_S3V3_on, S3V3_ON_SIG_RUN_STA_AC_ANTIFREEZE);
        }
        else
        {
            CLRBIT_N(fg_S3V3_on, S3V3_ON_SIG_RUN_STA_AC_ANTIFREEZE);
        }
    }
    else
    {
        CLRBIT_N(fg_S3V3_on, S3V3_ON_SIG_RUN_STA_AC_ANTIFREEZE);
    }

    /* B区定时防冻时 */
    if (fg_zone_B_timed_antifreeze_S3V3_need)
    {
       /* 定时防冻时先清空其他需求，交由防冻控制 */
        ARRCLR(fg_S3V3_on);	
        
        SETBIT_N(fg_S3V3_on, S3V3_ON_SIG_ZONE_B_TIMED_ANTIFREEZE);
    }
    else
    {
        CLRBIT_N(fg_S3V3_on, S3V3_ON_SIG_ZONE_B_TIMED_ANTIFREEZE);
    }

    /* 空气排空试运行开S3V3 */
    if (GETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V3_ON))
    {
        SETBIT_N(fg_S3V3_on, S3V3_ON_SIG_TEST_RUN_AIR_EMPTY);
    }
    else
    {
        CLRBIT_N(fg_S3V3_on, S3V3_ON_SIG_TEST_RUN_AIR_EMPTY);
    }

    /* 水泵试运行开S3V3 */
    if (GETBIT_N(fg_test_pump_on, TEST_PUMP_S3V3_ON))
    {
        SETBIT_N(fg_S3V3_on, S3V3_ON_SIG_TEST_RUN_PUMP);
    }
    else
    {
        CLRBIT_N(fg_S3V3_on, S3V3_ON_SIG_TEST_RUN_PUMP);
    }

    /* 遍历标志，检查是否需要开启 */
    for (i = 0; i < S3V3_ON_SIG_MAX; i++)
    {
        if (GETBIT_N(fg_S3V3_on, i))
        {
            set_output(MAX_PCB, M_DO_S3V3); 
        }
    }
}

/*******************************************************************************
 函 数: log_sys_valve()
 功 能: 阀件控制
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_valve(void)
{
    log_sys_S3V1();         /* 电磁三通阀S3V1(热水) */
    log_sys_S3V2();         /* 电磁三通阀S3V2(A区冷暖切换) */
    log_sys_S3V3();         /* 电磁三通阀S3V3(B区地暖) */
}

/*******************************************************************************
 函 数: log_sys_UIRT()
 功 能: 温控器逻辑
 参 数: 空
 返 回: 空
 其 它: 仅对制冷制热有效，对热水无效
*******************************************************************************/
void log_sys_UIRT(void)
{
    BOL HDIrtc = FALSE;
    BOL HDIrth = FALSE;
    static BOL HDIrtc_bak = FALSE;
    static BOL HDIrth_bak = FALSE;
    BOL HDIrtc_is_changed = FALSE;/* 温控器信号被改变时设为TRUE */
    BOL HDIrth_is_changed = FALSE;
    static BOL fg_is_first_enter_UIRT = TRUE; /* 启用温控器后第一次进入此函数标志，确保发一次命令 */

    /* 不处理温控器 */
    if (!USE_UIRT_CTRL		                                                /* 禁用温控器 */
    || fg_holiday_away_run	                                                /* 进入假日离家模式 */
    || STEP_IS_ALARM		                                                /* 故障态 */
    || (smart_grid_act == SG_ACT_ALL_FORCE_OFF)				                
    )
    {
        HDIrtc_bak = FALSE;
        HDIrth_bak = FALSE;
        fg_is_first_enter_UIRT = TRUE;
        return;
    }
    
    if (m_get_input(IN_RT_HEAT))    HDIrth = TRUE;/* 温控器制热信号 */
    if (m_get_input(IN_RT_COOL))    HDIrtc = TRUE;/* 温控器制冷信号 */

    /* HDIrtc被改变 */
    if (HDIrtc != HDIrtc_bak)
    {
        HDIrtc_bak = HDIrtc;
        HDIrtc_is_changed = TRUE;
    }

    /* HDIrth被改变 */
    if (HDIrth != HDIrth_bak)
    {
        HDIrth_bak = HDIrth;
        HDIrth_is_changed = TRUE;
    }

     /* A区模式切换 */
    if (USE_UIRT_ZONE_A_MODE_SWITCH)
    {
        /* 温控器信号被改变 或 启用后第一次进入函数 才发送命令 */
        if (HDIrtc_is_changed || HDIrth_is_changed || fg_is_first_enter_UIRT)
        {
            if (HDIrtc)			        /* 存在温控器制冷信号 */
            {
                para_set_cent(P_GaI(sv_mode), SET_COOL);
                SETCMD_N(ZONE_A_ON);
            }
            else if (!HDIrtc && HDIrth) /* 仅有温控器制热信号 */
            {
                para_set_cent(P_GaI(sv_mode), SET_HEAT);
                SETCMD_N(ZONE_A_ON);
            }
            else				        /* 无温控器制冷信号、无温控器制热信号 */
            {
                SETCMD_N(ZONE_A_OFF);
            }
        }
    }

    /* A区开关 */
    if (USE_UIRT_ZONE_A_ON_OFF || USE_UIRT_DUAL_ZONE_ON_OFF)
    {
        /* 温控器c信号被改变 或 启用后第一次进入函数 才发送命令 */
        if (HDIrtc_is_changed || fg_is_first_enter_UIRT)
        {
            /* A区 */
            if (HDIrtc)
            {
                SETCMD_N(ZONE_A_ON);
            }
            else
            {
                SETCMD_N(ZONE_A_OFF);
            }
        }
    }

    /* B区开关 */
    if (USE_UIRT_DUAL_ZONE_ON_OFF)
    {
        /* 温控器h信号被改变 或 启用后第一次进入函数 才发送命令 */
        if (HDIrth_is_changed || fg_is_first_enter_UIRT)
        {
            /* B区 */
            if (HDIrth)
            {
                /* 空调区域模式为制冷时，B区不可开启 */
                if (ac_mode != MODE_COOL)
                {
                    SETCMD_N(ZONE_B_ON);
                }
            }
            else
            {
                SETCMD_N(ZONE_B_OFF);
            }
        }
    }

    fg_is_first_enter_UIRT = FALSE;
    
#if 0
    BOOL room_ther_input_h = FALSE;
    BOOL room_ther_input_c = FALSE;
    BOOL fg_need_on = FALSE;
    static BOOL room_ther_input_h_bak = FALSE;
    static BOOL fg_need_on_bak = FALSE;             
    

    if (m_get_input(IN_RT_HEAT))    // 制热信号口输入
    {
        room_ther_input_h = TRUE;
    }

    if (m_get_input(IN_RT_COOL))    // 制冷信号口输入
    {
        room_ther_input_c = TRUE;
    }
    
    switch (val_RT_ctrl_type)
    {
        case UIRT_ZONE_A_MODE_SWITCHING:
            if ((room_ther_input_c)                 // 只要制冷信号口有输入运行制冷
                && (val_air_type != HEAT_ONLY)     // 存在制冷模式
//                && (val_air_type != HOT_WATER_ONLY)
            )
            {
                room_therostat_mode = MODE_COOL;
            }
            else if ((room_ther_input_h)                // 否者有制热信号口有输入运行制热
                    && (val_air_type != COOL_ONLY)     // 存在制热模式
//                    && (val_air_type != HOT_WATER_ONLY)
            )
            {
                room_therostat_mode = MODE_HEAT;
            }
            else
            {
                room_therostat_mode = MODE_NULL;
            }

            if (room_therostat_mode != MODE_NULL)
            {
                fg_need_on = TRUE;
            }

            if (fg_need_on_bak != fg_need_on)   // 开关机处理
            {
                fg_need_on_bak = fg_need_on;
                
                if (fg_need_on)
                {
                    if (status_run == _IDLE || status_run == _DELAY)
                    {
                        Logic_machine_start();    /* 执行开机操作 */
                    }
                }
                else
                {
                    if (status_run == _READY || status_run == _RUN)
                    {
                        Logic_machine_stop();     /* 执行关机操作 */
                    }
                }
            }
            break;
            
        case UIRT_ZONE_A_ON_OFF:  // 仅作为开关机，使用制热信号口输入
        case UIRT_DUAL_ZONE_ON_OFF:// 使用制热信号口输入 开机
            room_therostat_mode = MODE_NULL;
            if (!MODE_IS_HOTW)
            {
                if (room_ther_input_h_bak != room_ther_input_h)
                {
                    room_ther_input_h_bak = room_ther_input_h;
                    
                    if (room_ther_input_h)
                    {
                        if (status_run == _IDLE || status_run == _DELAY)
                        {
                            Logic_machine_start();    /* 执行开机操作 */
                        }
                        CMD_ZONE_B_ON;
                    }
                    else
                    {
                        if (status_run == _READY || status_run == _RUN)
                        {
                            Logic_machine_stop();     /* 执行关机操作 */
                        }
                    }
                }
            }
            break;
            
        default:
            room_therostat_mode = MODE_NULL;
            break;
    }
#endif
}

/*******************************************************************************
 函 数: log_sys_get_TWin1_max()
 功 能: 获取子系统中所有模块的TWin1，并取最大值
 参 数: type--; 
 返 回: 
 其 它: 空
*******************************************************************************/
static I16 log_sys_get_TWin1_max(U08 type)
{
    U08 pcb = 0;
    I16 pv_TWin1_max = SENSOR_NOEXIST;

    for (pcb = PCB_BGN(type); pcb < PCB_END(type); pcb++)
    {
        if (CELL_Is_used(pcb) && !err_sys_get_s_commu_err(pcb))
        {
            pv_TWin1_max = fun_pv_max(pv_TWin1_max, pv_cell_TWin1(pcb));
        }
    }

    return pv_TWin1_max;
}

/*******************************************************************************
 函 数: log_sys_get_TWin1_min()
 功 能: 获取子系统中所有模块的TWin1，并取最小值
 参 数: type--; 
 返 回: 
 其 它: 空
*******************************************************************************/
static I16 log_sys_get_TWin1_min(U08 type)
{
    U08 pcb = 0;
    I16 pv_TWin1_min = SENSOR_NOEXIST;

    for (pcb = PCB_BGN(type); pcb < PCB_END(type); pcb++)
    {
        if (CELL_Is_used(pcb) && !err_sys_get_s_commu_err(pcb))
        {
            pv_TWin1_min = fun_pv_min(pv_TWin1_min, pv_cell_TWin1(pcb));
        }
    }

    return pv_TWin1_min;
}

/*******************************************************************************
 函 数: log_sys_get_TWout1_min()
 功 能: 获取子系统中所有模块的TWout1，并取最小值
 参 数: type--; 
 返 回: 子系统中所有模块的TWout1
 其 它: 空
*******************************************************************************/
static I16 log_sys_get_TWout1_min(U08 type)
{
    U08 pcb = 0;
    I16 pv_TWout1_min = SENSOR_NOEXIST;

    for (pcb = PCB_BGN(type); pcb < PCB_END(type); pcb++)
    {
        if (CELL_Is_used(pcb) && !err_sys_get_s_commu_err(pcb))
        {
            pv_TWout1_min = fun_pv_min(pv_TWout1_min, pv_cell_TWout1(pcb));
        }
    }

    return pv_TWout1_min;
}

/*******************************************************************************
 函 数: log_sys_get_TWout2_min()
 功 能: 获取子系统中所有模块的TWout2，并取最小值
 参 数: type--; 
 返 回: 子系统中所有模块TWout2最小值
 其 它: 空
*******************************************************************************/
static I16 log_sys_get_TWout2_min(U08 type)
{
    U08 pcb = 0;
    I16 pv_TWout2_min = SENSOR_NOEXIST;

    for (pcb = PCB_BGN(type); pcb < PCB_END(type); pcb++)
    {
        if (CELL_Is_used(pcb) && !err_sys_get_s_commu_err(pcb))
        {
            pv_TWout2_min = fun_pv_min(pv_TWout2_min, pv_cell_TWout2(pcb));
        }
    }

    return pv_TWout2_min;
}

/* ========================================================================================== */
/*******************************************************************************
 函 数: log_sys_init_idle_sta_antifreeze()
 功 能: 初始化待机态防冻变量
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_init_idle_sta_antifreeze(void)
{
    fg_idle_sta_antifreeze = FALSE;
    fg_idle_sta_antifreeze_pump_need = FALSE;
    fg_idle_sta_antifreeze_IBH_need = FALSE;
    fg_idle_sta_antifreeze_COMPf_need = FALSE;
    
    idle_sta_antifreeze_status = ANTIFREEZE_IDLE;
    idle_sta_antifreeze_stage = AF_STAGE_IDLE;

	pump_time[PM_F_ANY].on = 0;
    pump_time[PM_F_ANY].off = 0;
}

/*******************************************************************************
 函 数: log_sys_init_run_sta_ac_antifreeze()
 功 能: 初始化运行态空调水路防冻变量
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_init_run_sta_ac_antifreeze(void)
{
    fg_run_sta_ac_antifreeze = FALSE;
    fg_run_sta_ac_antifreeze_pump_need = FALSE;
    fg_run_sta_ac_antifreeze_IBH_need = FALSE;
    fg_run_sta_ac_antifreeze_COMPf_need = FALSE;
    
    run_sta_ac_antifreeze_status = ANTIFREEZE_IDLE;

    pump_time[PM_F_NHW_RATFZ].on = 0;
    pump_time[PM_F_NHW_RATFZ].off = 0;
}

/*******************************************************************************
 函 数: log_sys_init_run_sta_hotw_antifreeze()
 功 能: 初始化运行态热水水路防冻变量
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_init_run_sta_hotw_antifreeze(void)
{
    fg_run_sta_hotw_antifreeze = FALSE;
    fg_run_sta_hotw_antifreeze_pump_need = FALSE;
    fg_run_sta_hotw_antifreeze_IBH_need = FALSE;
    fg_run_sta_hotw_antifreeze_COMPf_need = FALSE;
    
    run_sta_hotw_antifreeze_status = ANTIFREEZE_IDLE;

    pump_time[PM_F_HW_RATFZ].on = 0;
    pump_time[PM_F_HW_RATFZ].off = 0;
}

/*******************************************************************************
 函 数: log_sys_init_run_sta_antifreeze()
 功 能: 初始化运行态防冻变量
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_init_run_sta_antifreeze(void)
{
    log_sys_init_run_sta_ac_antifreeze();
    log_sys_init_run_sta_hotw_antifreeze();
    run_sta_antifreeze_stage = AF_STAGE_IDLE;
}

/*******************************************************************************
 函 数: log_sys_init_zone_A_timed_antifreeze()
 功 能: 初始化A区定时防冻变量
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_init_zone_A_timed_antifreeze(void)
{
    fg_zone_A_timed_antifreeze = FALSE;
    fg_zone_A_timed_antifreeze_PUMPo_need = FALSE;
    fg_zone_A_timed_antifreeze_S3V1_ON_need = FALSE;
    fg_zone_A_timed_antifreeze_COMPf_need = FALSE;
    
    pump_time[PM_O_TATFZ].on = 0;
    pump_time[PM_O_TATFZ].off = 0;
}

/*******************************************************************************
 函 数: log_sys_init_zone_B_timed_antifreeze()
 功 能: 初始化B区定时防冻变量
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_init_zone_B_timed_antifreeze(void)
{
    fg_zone_B_timed_antifreeze = FALSE;
    fg_zone_B_timed_antifreeze_PUMPm_need = FALSE;
    fg_zone_B_timed_antifreeze_S3V3_need = FALSE;
    fg_zone_B_timed_antifreeze_S3V1_ON_need = FALSE;
    fg_zone_B_timed_antifreeze_COMPf_need = FALSE;
    
    pump_time[PM_M_TATFZ].on = 0;
    pump_time[PM_M_TATFZ].off = 0;
}

/*******************************************************************************
 函 数: log_sys_init_hotw_timed_antifreeze()
 功 能: 初始化热水定时防冻变量
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_init_hotw_timed_antifreeze(void)
{
    fg_hotw_timed_antifreeze = FALSE;
    fg_hotw_timed_antifreeze_S3V1_OFF_need = FALSE;
    fg_hotw_timed_antifreeze_COMPf_need = FALSE;
    
    pump_time[PM_HW_TATFZ].on = 0;
    pump_time[PM_HW_TATFZ].off = 0;
}

/*******************************************************************************
 函 数: log_sys_init_timed_antifreeze()
 功 能: 初始化定时防冻变量
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_init_timed_antifreeze(void)
{
    log_sys_init_zone_A_timed_antifreeze();
    log_sys_init_zone_B_timed_antifreeze();
    log_sys_init_hotw_timed_antifreeze();
}

/*******************************************************************************
 函 数: log_sys_zone_A_timed_antifreeze()
 功 能: A区定时防冻
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_zone_A_timed_antifreeze(void)
{   
    BOL fg_no_zone_A_timed_antifreeze = FALSE;
    I16 antifreeze_temp = SENSOR_NOEXIST;
    I16 exit_antifreeze_temp = SENSOR_NOEXIST;
    
    if (fg_zone_A_run)		                                /* A区运行 */
    {
        fg_no_zone_A_timed_antifreeze = TRUE;
    }
    
    if (IS_DUAL_SYS
    && (!fg_zone_A_run && !fg_zone_B_run && fg_hotw_run)	/* A区未运行，B区未运行，热水运行 */
    )
    {
        fg_no_zone_A_timed_antifreeze = TRUE;
    }

    /* 无A区定时防冻时，初始化相关变量 */
    if (fg_no_zone_A_timed_antifreeze)
    {
        log_sys_init_zone_A_timed_antifreeze();
        return;
    }

    /* 定时防冻温度 */
    antifreeze_temp = fun_pv_min(pv_sys_TACt1, pv_sys_TACt2);

    /* 需要开启PUMPo */
    if ((pump_time[PM_O_TATFZ].off >= val_timed_antifreeze_interval)/* PUMPo停机时间 ≥【定时防冻间隔】 */
    && ((SensorCanUse(pv_sys_TA) && (pv_sys_TA <= val_anti_env_in)) /* TA可用，且TA ≤【进防冻环温】 */
       || SensorCannotUse(pv_sys_TA)                                /* TA不可用 */
       )
    )
    {
        fg_zone_A_timed_antifreeze_PUMPo_need = TRUE;
    }

    /* 进入A区定时防冻 */
    if (SensorCannotUse(pv_sys_TA)    
    || (pv_sys_TA <= val_anti_env_in) 
    )
    {
        if (((pump_time[PM_O_TATFZ].on > 180)               /* PUMPo持续运行180S */
            && SensorCanUse(antifreeze_temp)                /* 防冻温度可用 */
            && (antifreeze_temp <= val_anti_pump_on)        /* 防冻温度 ≤【水泵防冻温度】 */
            )                                               
        || ((pump_time[PM_O_TATFZ].on > 0)                  /* PUMPo已开启 */
           && SensorCannotUse(antifreeze_temp)              /* 防冻温度不可用 */
           )                                                
        )
        {
            fg_zone_A_timed_antifreeze = TRUE;

            /* 电磁三通阀1保持ON输出需求 */
            if (!fg_zone_A_run && !fg_zone_B_run)               /* A区、B区均未运行时 */
            {
                fg_zone_A_timed_antifreeze_S3V1_ON_need = TRUE;
            }
            else
            {
                fg_zone_A_timed_antifreeze_S3V1_ON_need = FALSE;
            }

            /* 压机固定转速需求 */
            if (!IS_DUAL_SYS)
            {
                fg_zone_A_timed_antifreeze_COMPf_need = TRUE;
            }
            else
            {
                fg_zone_A_timed_antifreeze_COMPf_need = FALSE;
            }
        }
    }

    /* 退定时防冻温度 */
    exit_antifreeze_temp = fg_zone_A_timed_antifreeze_COMPf_need ? val_anti_comp_off : val_anti_pump_on;

    /* 退出A区定时防冻 */
    if ((SensorCanUse(pv_sys_TA)                                /* TA可用 */
        && (pv_sys_TA > val_anti_env_in + val_anti_env_diff)    /* TA ＞【进防冻环温】＋【退防冻环温差】 */
        )                                               
    || ((pump_time[PM_O_TATFZ].on > 180)                        /* PUMPo持续运行180S */
       && SensorCanUse(antifreeze_temp)                         /* 防冻温度可用 */
       && (antifreeze_temp > exit_antifreeze_temp)              /* 防冻温度 ＞退防冻温度 */
       )                                                
    || (pump_time[PM_O_TATFZ].on > val_timed_antifreeze_max)    /* PUMPo防冻运行时间 ＞【定时防冻时长】 */
    )
    {
        fg_zone_A_timed_antifreeze = FALSE;
        fg_zone_A_timed_antifreeze_PUMPo_need = FALSE;
        fg_zone_A_timed_antifreeze_S3V1_ON_need = FALSE;
        fg_zone_A_timed_antifreeze_COMPf_need = FALSE;
    }
}

/*******************************************************************************
 函 数: log_sys_zone_B_timed_antifreeze()
 功 能: B区定时防冻
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_zone_B_timed_antifreeze(void)
{
    BOL fg_no_zone_B_timed_antifreeze = FALSE;
    I16 antifreeze_temp = SENSOR_NOEXIST;
    I16 exit_antifreeze_temp = SENSOR_NOEXIST;

    if (!USE_DUAL_ZONE_CTRL                 /* 非双区控制 */
    || fg_zone_B_run		                /* B区运行 */
    || fg_run_sta_ac_antifreeze				/* 进入运行态空调水路防冻 */
    )
    {
        fg_no_zone_B_timed_antifreeze = TRUE;
    }
    
    if (IS_DUAL_SYS
    && (!fg_zone_A_run && !fg_zone_B_run && fg_hotw_run)	/* A区未运行，B区未运行，热水运行 */
    )
    {
        fg_no_zone_B_timed_antifreeze = TRUE;
    }

    /* 无B区定时防冻时，初始化相关变量 */
    if (fg_no_zone_B_timed_antifreeze)
    {
        log_sys_init_zone_B_timed_antifreeze();
        return;
    }

    /* 定时防冻温度 */
    antifreeze_temp = fun_pv_min(pv_sys_TACt1, pv_sys_TACt2);
    antifreeze_temp = fun_pv_min(antifreeze_temp, pv_sys_TFLin); 
     
    /* 需要开启PUMPm */
    if ((pump_time[PM_M_TATFZ].off >= val_timed_antifreeze_interval)/* PUMPm停机时间 ≥【定时防冻间隔】 */
    && ((SensorCanUse(pv_sys_TA) && (pv_sys_TA <= val_anti_env_in)) /* TA可用，且TA ≤【进防冻环温】 */
       || SensorCannotUse(pv_sys_TA)                                /* TA不可用 */
       )
    )
    {
        fg_zone_B_timed_antifreeze_PUMPm_need = TRUE;
    }

    /* 进入B区定时防冻 */
    if (SensorCannotUse(pv_sys_TA)    
    || (pv_sys_TA <= val_anti_env_in) 
    )
    {
        if (((pump_time[PM_M_TATFZ].on > 180)               /* PUMPm持续运行180S */
            && SensorCanUse(antifreeze_temp)                /* 防冻温度可用 */
            && (antifreeze_temp <= val_anti_pump_on)        /* 防冻温度 ≤【水泵防冻温度】 */
            )                                               
        || ((pump_time[PM_M_TATFZ].on > 0)                  /* PUMPm已开启 */
           && SensorCannotUse(antifreeze_temp)              /* 防冻温度不可用 */
           )                                                
        )
        {
            fg_zone_B_timed_antifreeze = TRUE;
            fg_zone_B_timed_antifreeze_S3V3_need = TRUE;

            /* 电磁三通阀1保持ON输出需求 */
            if (!fg_zone_A_run && !fg_zone_B_run)               /* A区、B区均未运行时 */
            {
                fg_zone_B_timed_antifreeze_S3V1_ON_need = TRUE;
            }
            else
            {
                fg_zone_B_timed_antifreeze_S3V1_ON_need = FALSE;
            }

            /* 压机固定转速需求 */
            if (!IS_DUAL_SYS)
            {
                fg_zone_B_timed_antifreeze_COMPf_need = TRUE;
            }
            else
            {
                fg_zone_B_timed_antifreeze_COMPf_need = FALSE;
            }
        }
    }

    /* 退定时防冻温度 */
    exit_antifreeze_temp = fg_zone_B_timed_antifreeze_COMPf_need ? val_anti_comp_off : val_anti_pump_on;

    /* 退出B区定时防冻 */
    if ((SensorCanUse(pv_sys_TA)                                /* TA可用 */
        && (pv_sys_TA > val_anti_env_in + val_anti_env_diff)    /* TA ＞【进防冻环温】＋【退防冻环温差】 */
        )                                               
    || ((pump_time[PM_M_TATFZ].on > 180)                        /* PUMPm持续运行180S */
       && SensorCanUse(antifreeze_temp)                         /* 防冻温度可用 */
       && (antifreeze_temp > exit_antifreeze_temp)              /* 防冻温度 ＞退防冻温度 */
       )                                                
    || (pump_time[PM_M_TATFZ].on > val_timed_antifreeze_max)    /* PUMPm防冻运行时间 ＞【定时防冻时长】 */
    )
    {
        fg_zone_B_timed_antifreeze = FALSE;
        fg_zone_B_timed_antifreeze_PUMPm_need = FALSE;
        fg_zone_B_timed_antifreeze_S3V3_need = FALSE;
        fg_zone_B_timed_antifreeze_S3V1_ON_need = FALSE;
        fg_zone_B_timed_antifreeze_COMPf_need = FALSE;
    }
}

/*******************************************************************************
 函 数: log_sys_hotw_timed_antifreeze()
 功 能: 热水定时防冻
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_hotw_timed_antifreeze(void)
{
    BOL fg_no_hotw_timed_antifreeze = FALSE;
    I16 antifreeze_temp = SENSOR_NOEXIST;
    I16 exit_antifreeze_temp = SENSOR_NOEXIST;

    if (!val_hotw_used		                /* 无热水功能 */
    || fg_hotw_run			                /* 热水运行 */
    || fg_run_sta_hotw_antifreeze			/* 进入运行态热水水路防冻 */
    || IS_DUAL_SYS			                /* 双水路系统 */
    )
    {
        fg_no_hotw_timed_antifreeze = TRUE;
    }

    /* 无热水定时防冻时，初始化相关变量 */
    if (fg_no_hotw_timed_antifreeze)
    {
        log_sys_init_hotw_timed_antifreeze();
        return;
    }

    /* 定时防冻温度 */
    antifreeze_temp = pv_sys_THWt;

    /* 需要S3V1 */
    if ((pump_time[PM_HW_TATFZ].off >= val_timed_antifreeze_interval)   /* PUMPf热水定时防冻停机时间 ≥【定时防冻间隔】 */
    && ((SensorCanUse(pv_sys_TA) && (pv_sys_TA <= val_anti_env_in))     /* TA可用，且TA ≤【进防冻环温】 */
       || SensorCannotUse(pv_sys_TA)                                    /* TA不可用 */
       )
    && (SensorCannotUse(antifreeze_temp) || antifreeze_temp<AF_HOTW_ENTER_TEMP)
    )
    {
        fg_hotw_timed_antifreeze_S3V1_OFF_need = TRUE;
    }

    /* 进入热水定时防冻 */
    if (SensorCannotUse(pv_sys_TA)    
    || (pv_sys_TA <= val_anti_env_in) 
    )
    {
        if (((pump_time[PM_HW_TATFZ].on > 180)                              /* PUMPf持续运行180S */
            && SensorCanUse(antifreeze_temp)                                /* THWt可用 */
            && (antifreeze_temp <= val_anti_pump_on)                        /* 热水水箱温度THWt ≤【水泵防冻温度】 */
            )   
        || ((pump_time[PM_HW_TATFZ].on > 0)                                 /* PUMPf已开启 */
           && SensorCannotUse(antifreeze_temp)                              /* 防冻温度不可用 */
           )                                        
        )
        {
            fg_hotw_timed_antifreeze = TRUE;

            /* 压机固定转速需求 */
            if (!IS_DUAL_SYS)
            {
                fg_hotw_timed_antifreeze_COMPf_need = TRUE;
            }
            else
            {
                fg_hotw_timed_antifreeze_COMPf_need = FALSE;
            }
        }
    }

    /* 退定时防冻温度 */
    exit_antifreeze_temp = fg_hotw_timed_antifreeze_COMPf_need ? val_anti_comp_off : val_anti_pump_on;

    /* 退出热水定时防冻 */
    if ((SensorCanUse(pv_sys_TA)                                        /* TA可用 */
        && (pv_sys_TA > val_anti_env_in + val_anti_env_diff)            /* TA ＞【进防冻环温】＋【退防冻环温差】 */
        )                         
    || ((pump_time[PM_HW_TATFZ].on > 180)                               /* PUMPf持续运行180S */
       && SensorCanUse(antifreeze_temp)                                 /* 防冻温度可用 */
       && (antifreeze_temp > exit_antifreeze_temp)                          /* 防冻温度 ＞【水泵防冻温度】 */
       )
    || (pump_time[PM_HW_TATFZ].on > val_timed_antifreeze_max)           /* PUMPf热水定时防冻运行时间 ＞【定时防冻时长】 */
    )
    {
        fg_hotw_timed_antifreeze = FALSE;
        fg_hotw_timed_antifreeze_S3V1_OFF_need = FALSE;
        fg_hotw_timed_antifreeze_COMPf_need = FALSE;
    }
}

/*******************************************************************************
 函 数: log_sys_timed_antifreeze()
 功 能: 定时防冻逻辑
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_timed_antifreeze(void)
{   
    BOL fg_no_timed_antifreeze = FALSE;
    
    if ((val_anti_used == UNUSED)
    || !AIR_IS_WATER
    || (power_time <= POWER_ON_FREEZ)
    || !TF_IS_IDLE
    || (val_timed_antifreeze_interval == 0)
    || STEP_IS_IDLE
    || (STEP_IS_READY && status_ready==READY_IDLE)	        /* 卡在启动阶段，由待机态防冻覆盖 */
    || (ac_mode == MODE_COOL)
    || fg_enter_test_run	                                /* 已进入试运行 */
    )
    {
        fg_no_timed_antifreeze = TRUE;
    }
    else if ((STEP_IS_READY && status_ready > READY_IDLE) 
    || STEP_IS_RUN
    || STEP_IS_DELAY
    )
    {
        if (fg_run_sta_ac_antifreeze        /* 已进入运行态空调水路防冻 */
        || fg_run_sta_hotw_antifreeze       /* 已进入运行态热水水路防冻 */
        )
        {
            fg_no_timed_antifreeze = TRUE;
        }
    }

    /* 三相电故障 */
    if (m_get_error(MAX_PCB, M_ERR_PHASE_REVERSE)//三相电错相故障不进入防冻
    ||  m_get_error(MAX_PCB, M_ERR_PHASE_LACK)   //三相电缺相故障不进入防冻
    )
    {
        fg_no_timed_antifreeze = TRUE;
    }

    /* A区、B区、热水均未运行 */
    if (!fg_zone_A_run
    && !fg_zone_B_run
    && !fg_hotw_run
    )
    {
        fg_no_timed_antifreeze = TRUE;
    }

    /* 无定时防冻功能 */
    if (fg_no_timed_antifreeze)
    {
        log_sys_init_timed_antifreeze();
        return;
    }

    /* A区定时防冻 */
    log_sys_zone_A_timed_antifreeze();

    /* B区定时防冻 */
    log_sys_zone_B_timed_antifreeze();

    /* 热水定时防冻 */
    log_sys_hotw_timed_antifreeze();

}

/*******************************************************************************
 函 数: log_sys_antifreeze()
 功 能: 防冻逻辑
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_antifreeze(void)
{
    I16 pv_TWout2_min, pv_TWout1_min, pv_TWin1_min, pv_TWin1_max;
    I16 antifreeze_temp_enter = SENSOR_NOEXIST;             /* 进防冻温度 */
    I16 antifreeze_temp_exit = SENSOR_NOEXIST;              /* 退防冻温度 */
    static I16 antifreeze_temp_exit_bak[AF_TYPE_MAX];       /* 退防冻温度备份 */
    static U16 cnt_freeze_energy_time[PM_MAX];
    I16 temp_env = pv_sys_TA;                               /* 环境温度 */
    U16 antifreeze_cycle = 3*QK_60;	                        /* 防冻间隔 */
    U08 antifreeze_level = ANTI_ALL;        /* 防冻等级 */
    U08 pm_type = PM_F_ANY;
    U08 antifreeze_type = AF_IDLE_STA;
    U08 sub_sys_type = SUB_SYS_TYPE_MAX;
    
    U08* antifreeze_stage;                  /* 防冻阶段指针 */
    U08* antifreeze_status;                 /* 防冻状态指针 */
    BOL* fg_antifreeze;                     /* 防冻标志指针 */
    BOL* fg_antifreeze_pump_need;           /* 防冻开启变频水泵需求 */
    BOL* fg_antifreeze_IBH_need;            /* 防冻开启辅助电热需求 */
    BOL* fg_antifreeze_COMPf_need;          /* 防冻开启变频压机需求指针 */

    #define AF_TIME_INIT_MAX 0xffff

    /* 初始化 */        
    antifreeze_stage = &idle_sta_antifreeze_stage;
    antifreeze_status = &idle_sta_antifreeze_status;
    fg_antifreeze = &fg_idle_sta_antifreeze;
    fg_antifreeze_pump_need = &fg_idle_sta_antifreeze_pump_need;
    fg_antifreeze_IBH_need = &fg_idle_sta_antifreeze_IBH_need;
    fg_antifreeze_COMPf_need = &fg_idle_sta_antifreeze_COMPf_need;

    if (val_anti_used == UNUSED
    ||  m_get_error(MAX_PCB, M_ERR_PHASE_REVERSE)//电源故障不进入防冻
    ||  m_get_error(MAX_PCB, M_ERR_PHASE_LACK)   //电源故障不进入防冻
    ||  !AIR_IS_WATER
    ||  power_time <= POWER_ON_FREEZ
    || !TF_IS_IDLE
    || fg_enter_test_run                                        /* 已进入试运行 */
    )   
    {
        log_sys_init_idle_sta_antifreeze();
        log_sys_init_run_sta_antifreeze();
        return;
    }
    else if (STEP_IS_IDLE
         || (STEP_IS_READY && status_ready==READY_IDLE)
    )/* 待机态防冻 */
    {
        /* 不执行待机态防冻逻辑的情况 */
        if (fg_zone_A_run || fg_zone_B_run || fg_hotw_run)		/* 空调或热水运行 */
        {
            log_sys_init_idle_sta_antifreeze();
            log_sys_init_run_sta_antifreeze();
            return;
        }
		
        log_sys_init_run_sta_antifreeze();
    
        pm_type = PM_F_ANY;
        antifreeze_type = AF_IDLE_STA;

        sub_sys_type = (IS_DUAL_SYS && idle_sta_antifreeze_stage==AF_STAGE_HOTW) ? SUB_SYS_AC_HOTW : SUB_SYS_TYPE_MAX;
        
        antifreeze_stage = &idle_sta_antifreeze_stage;
        antifreeze_status = &idle_sta_antifreeze_status;
        fg_antifreeze = &fg_idle_sta_antifreeze;
        fg_antifreeze_pump_need = &fg_idle_sta_antifreeze_pump_need;
        fg_antifreeze_IBH_need = &fg_idle_sta_antifreeze_IBH_need;
        fg_antifreeze_COMPf_need = &fg_idle_sta_antifreeze_COMPf_need;
    }
    else if ((STEP_IS_READY && status_ready>READY_IDLE) 
    || STEP_IS_RUN
    || STEP_IS_DELAY
    )/* 运行态防冻 */   
    {
        /* 不执行运行态防冻逻辑的情况 */
        if (((fg_zone_A_run || fg_zone_B_run) && fg_hotw_run)		/* 空调、热水同时运行 */
        || !val_hotw_used	                                        /* 无热水功能 */
        || !IS_DUAL_SYS		                                        /* 无双水路系统 */
        )
        {
            log_sys_init_idle_sta_antifreeze();
            log_sys_init_run_sta_antifreeze();
            return;
        }

        log_sys_init_idle_sta_antifreeze();
    
        if (!fg_zone_A_run	/* A区未运行 */
        && !fg_zone_B_run	/* B区未运行 */
        && fg_hotw_run		/* 热水运行 */
        )                   
        {
            /* 运行态空调水路防冻 */
            pm_type = PM_F_NHW_RATFZ;
            antifreeze_type = AF_RUN_STA_AC;
            sub_sys_type = SUB_SYS_AC_ONLY;
            
            antifreeze_stage = &run_sta_antifreeze_stage;
            antifreeze_status = &run_sta_ac_antifreeze_status;
            fg_antifreeze = &fg_run_sta_ac_antifreeze;
            fg_antifreeze_pump_need = &fg_run_sta_ac_antifreeze_pump_need;
            fg_antifreeze_IBH_need = &fg_run_sta_ac_antifreeze_IBH_need;
            fg_antifreeze_COMPf_need = &fg_run_sta_ac_antifreeze_COMPf_need;

            log_sys_init_run_sta_hotw_antifreeze();
        }
        else                
        {
            /* 运行态热水水路防冻 */
            pm_type = PM_F_HW_RATFZ;
            antifreeze_type = AF_RUN_STA_HOTW;
            sub_sys_type = SUB_SYS_AC_HOTW;
            
            antifreeze_stage = &run_sta_antifreeze_stage;
            antifreeze_status = &run_sta_hotw_antifreeze_status;
            fg_antifreeze = &fg_run_sta_hotw_antifreeze;
            fg_antifreeze_pump_need = &fg_run_sta_hotw_antifreeze_pump_need;
            fg_antifreeze_IBH_need = &fg_run_sta_hotw_antifreeze_IBH_need;
            fg_antifreeze_COMPf_need = &fg_run_sta_hotw_antifreeze_COMPf_need;

            log_sys_init_run_sta_ac_antifreeze();

            /* 不执行运行态热水防冻的情况 */
            if (SensorCanUse(pv_sys_THWt) && pv_sys_THWt>=AF_HOTW_ENTER_TEMP)
            {
                log_sys_init_run_sta_hotw_antifreeze();
                return;
            }
        }
    }
    
    /* 防冻温度 */
    if (antifreeze_type == AF_IDLE_STA)
    {
        pv_TWin1_max  = log_sys_get_TWin1_max(sub_sys_type);        /* 所有模块TWin1最大值 */
        
        pv_TWin1_min  = log_sys_get_TWin1_min(sub_sys_type);        /* 所有模块TWin1最小值 */
        pv_TWout1_min = log_sys_get_TWout1_min(sub_sys_type);       /* 所有模块TWout1最小值 */
        pv_TWout2_min = log_sys_get_TWout2_min(sub_sys_type);       /* 所有模块TWout2最小值 */

        antifreeze_temp_enter = fun_pv_min(pv_TWin1_min, pv_TWout1_min);
        antifreeze_temp_enter = fun_pv_min(antifreeze_temp_enter, pv_TWout2_min);
        antifreeze_temp_enter = fun_pv_min(antifreeze_temp_enter, pv_sys_TWout3);   /* 系统总出水温度 */
        antifreeze_temp_enter = fun_pv_min(antifreeze_temp_enter, pv_sys_TACt1);    /* 缓冲水箱上部温度 */
        antifreeze_temp_enter = fun_pv_min(antifreeze_temp_enter, pv_sys_TACt2);    /* 缓冲水箱下部温度 */
        antifreeze_temp_enter = fun_pv_min(antifreeze_temp_enter, pv_sys_TFLin);    /* 地暖进水温度 */
        antifreeze_temp_enter = fun_pv_min(antifreeze_temp_enter, pv_sys_THWt);     /* 热水水箱温度 */

        antifreeze_temp_exit = fun_pv_min(pv_TWin1_min, pv_TWout1_min);
        antifreeze_temp_exit = fun_pv_min(antifreeze_temp_exit, pv_TWout2_min);
        /* 2023-1-11 !!! 退防冻温度不可开放以下温度 */
//      antifreeze_temp_exit = fun_pv_min(antifreeze_temp_exit, pv_sys_TWout3);   /* 系统总出水温度 */        
//      antifreeze_temp_exit = fun_pv_min(antifreeze_temp_exit, pv_sys_TACt1);    /* 缓冲水箱上部温度 */
//      antifreeze_temp_exit = fun_pv_min(antifreeze_temp_exit, pv_sys_TACt2);    /* 缓冲水箱下部温度 */
//      antifreeze_temp_exit = fun_pv_min(antifreeze_temp_exit, pv_sys_TFLin);    /* 地暖进水温度 */
    }
    else if (antifreeze_type == AF_RUN_STA_AC)
    {
        pv_TWin1_max  = log_sys_get_TWin1_max(sub_sys_type);     /* 所有不可制热水模块TWin1最大值 */
        
        pv_TWin1_min  = log_sys_get_TWin1_min(sub_sys_type);     /* 所有不可制热水模块TWin1最小值  */
        pv_TWout1_min = log_sys_get_TWout1_min(sub_sys_type);    /* 所有不可制热水模块TWout1最小值 */
        pv_TWout2_min = log_sys_get_TWout2_min(sub_sys_type);    /* 所有不可制热水模块TWout2最小值 */
    
        antifreeze_temp_enter = fun_pv_min(pv_TWin1_min, pv_TWout1_min);
        antifreeze_temp_enter = fun_pv_min(antifreeze_temp_enter, pv_TWout2_min);
        antifreeze_temp_enter = fun_pv_min(antifreeze_temp_enter, pv_sys_TWout3);   /* 系统总出水温度 */
        antifreeze_temp_enter = fun_pv_min(antifreeze_temp_enter, pv_sys_TACt1);    /* 缓冲水箱上部温度 */
        antifreeze_temp_enter = fun_pv_min(antifreeze_temp_enter, pv_sys_TACt2);    /* 缓冲水箱下部温度 */
        antifreeze_temp_enter = fun_pv_min(antifreeze_temp_enter, pv_sys_TFLin);    /* 地暖进水温度 */

        antifreeze_temp_exit = fun_pv_min(pv_TWin1_min, pv_TWout1_min);
        antifreeze_temp_exit = fun_pv_min(antifreeze_temp_exit, pv_TWout2_min);
    }
    else if (antifreeze_type == AF_RUN_STA_HOTW)
    {
        pv_TWin1_max  = log_sys_get_TWin1_max(sub_sys_type);    /* 所有可制热水模块TWin1最大值 */
        
        pv_TWin1_min  = log_sys_get_TWin1_min(sub_sys_type);	/* 所有可制热水模块TWin1最小值  */
        pv_TWout1_min = log_sys_get_TWout1_min(sub_sys_type);	/* 所有可制热水模块TWout1最小值 */
        pv_TWout2_min = log_sys_get_TWout2_min(sub_sys_type);	/* 所有可制热水模块TWout2最小值 */

        antifreeze_temp_enter = fun_pv_min(pv_TWin1_min, pv_TWout1_min);
        antifreeze_temp_enter = fun_pv_min(antifreeze_temp_enter, pv_TWout2_min);
        antifreeze_temp_enter = fun_pv_min(antifreeze_temp_enter, pv_sys_TWout3);   /* 系统总出水温度 */
        antifreeze_temp_enter = fun_pv_min(antifreeze_temp_enter, pv_sys_THWt);     /* 热水水箱温度 */

        antifreeze_temp_exit = fun_pv_min(pv_TWin1_min, pv_TWout1_min);
        antifreeze_temp_exit = fun_pv_min(antifreeze_temp_exit, pv_TWout2_min);
    }
    
    /* 防冻周期选择 */
    if (SensorCanUse(antifreeze_temp_enter) && antifreeze_temp_enter<val_anti_comp_on)
    {
        antifreeze_cycle = MIN(3*QK_60, val_anti_off_time2);
    }
    else if (SensorCanUse(temp_env) && temp_env>=val_anti_embient)
    {
        antifreeze_cycle = val_anti_off_time1;
    }
    else
    {
        antifreeze_cycle = val_anti_off_time2;
    }

    /* 特殊防冻判断 */
    if (SensorCannotUse(antifreeze_temp_exit))           /* 空调出回水温度都有故障，特殊防冻 */
    {
        antifreeze_level = ANTI_UNUSUAL;
    }
    else if (SensorCannotUse(temp_env))      /* 环境温度故障时，正常防冻降级为环境温度缺省防冻 */                         
    {
        antifreeze_level = ANTI_NON_ENV;
    }

    /* 判断进入防冻 */
    if (*fg_antifreeze == FALSE)
    {
        *antifreeze_status = ANTIFREEZE_IDLE;
        *antifreeze_stage = AF_STAGE_IDLE;
        freeze_energy_time[pm_type] = 0;       
        
        if (pump_time[pm_type].off >= antifreeze_cycle)
        {
            if (antifreeze_level == ANTI_UNUSUAL)         /* 非正常防冻时的处理  */ 
            {       
                if (SensorCanUse(temp_env))    
                {
                    if (temp_env <= val_anti_env_in)    // 环境温度小于2才进入防冻
                    {
                        *fg_antifreeze = TRUE;
                    }
                }
                else                 /* 环境温度故障时，直接进入防冻 */
                {
                    *fg_antifreeze = TRUE;
                }
            }
            else if (antifreeze_level == ANTI_NON_ENV)
            {
               if (antifreeze_temp_enter <= val_anti_pump_on)
               {
                   *fg_antifreeze = TRUE;
               }
            }
            else          /* 正常防冻 */
            {
                if (temp_env < val_anti_env_in)
                {
                    *fg_antifreeze = TRUE;
                }
            }
        }
    }
    
    /* 特殊防冻处理 */
    if (antifreeze_level == ANTI_UNUSUAL)            /* 非正常防冻，关掉能量，直接返回 */
    {
        /* 截取一个周期计时 */
        U16 cnt_freeze_cycle = freeze_act_time%ANTI_UNUSUAL_DLY;

        if (*fg_antifreeze)
        {
            *fg_antifreeze_pump_need = TRUE;
            
            if (SensorCanUse(temp_env))       /* 环境温度无故障时 */                        
            {
                if (temp_env > val_anti_env_in + val_anti_env_diff)       /* 环境温度大于4时退出 */
                {
                    *fg_antifreeze = FALSE;
                }
            }

            switch (antifreeze_type)
            {
                default:
                case AF_IDLE_STA:
                    if (*antifreeze_stage == AF_STAGE_IDLE)
                    {
                        *antifreeze_stage = AF_STAGE_AC;
                    }

                    /* 50%占空比轮流输出 */
                    if (cnt_freeze_cycle < ANTI_UNUSUAL_DLY/2)
                    {
                        *antifreeze_stage = AF_STAGE_AC;
                    }
                    else
                    {
                        *antifreeze_stage = AF_STAGE_HOTW;
                    }
                    break;
                case AF_RUN_STA_AC:
                    *antifreeze_stage = AF_STAGE_AC;
                    break;
                case AF_RUN_STA_HOTW:
                    *antifreeze_stage = AF_STAGE_HOTW;
                    break;
            }           
        }
        else
        {
            *fg_antifreeze_pump_need = FALSE;
            *antifreeze_stage = AF_STAGE_IDLE;
        }
        
        return;
    }

    /* 电热防冻处理 */
    if (*antifreeze_status > ANTIFREEZE_PUMP
    &&  *antifreeze_status < ANTIFREEZE_CLOSE_COMP
    &&  antifreeze_level != ANTI_UNUSUAL
    &&  temp_env <= val_anti_env_in+val_anti_env_diff		/* 未满足退防冻环温条件 */
    &&  pv_TWin1_max <= AF_SV_TEMP_FREQ
    )
    {
        if (antifreeze_temp_exit <= val_anti_heat_on)
        {
            *fg_antifreeze_IBH_need = TRUE;
        }
        else if (antifreeze_temp_exit > val_anti_heat_off)
        {
            *fg_antifreeze_IBH_need = FALSE;
        }
    }
    else 
    {
        *fg_antifreeze_IBH_need = FALSE;
    }

    /* 水泵、压机防冻处理 */
    switch(*antifreeze_status)
    {
        case ANTIFREEZE_IDLE:
            if (*fg_antifreeze)
            {
                if ((fg_ac_pump_firstopen==0 && freeze_energy_time[pm_type]>random_time)  // 首次开水泵（主要防止上电同时启动）    
                || fg_ac_pump_firstopen==1) 
                {
                    *antifreeze_status = ANTIFREEZE_PUMP;
                    
                    if (antifreeze_type == AF_IDLE_STA
                    || antifreeze_type == AF_RUN_STA_AC
                    )
                    {
                        *antifreeze_stage = AF_STAGE_AC;    /* 先执行空调水路防冻 */
                    }
                    else
                    {
                        *antifreeze_stage = AF_STAGE_HOTW;  /* 先执行热水水路防冻 */
                    }
                    freeze_energy_time[pm_type] = 0;
                }
                
                if (log_sys_have_cell_pump_run(sub_sys_type))
                {
                    if (log_sys_have_comp_run(MODE_COOL, sub_sys_type))
                    {
                        /* 如果当前压机在制冷运行，则需要先停子系统中的压机 */
                        log_sys_select_comp_stop_once(MODE_COOL, sub_sys_type);
                    }
                }
            }
            break;
        case ANTIFREEZE_PUMP:
            fg_ac_pump_firstopen=1;         // 已经开过水泵  
            *fg_antifreeze_pump_need = TRUE;    /* 开空调泵 */
            
            if (freeze_energy_time[pm_type] > ANTI_PRE_DLY)    
            {
                freeze_energy_time[pm_type] = 0;
                *antifreeze_status = ANTIFREEZE_DEAL;
            }
            break;
        case ANTIFREEZE_DEAL:            /* 根据防冻温度值进行水泵动作处理 */
            if (antifreeze_temp_exit <= val_anti_comp_on)            /* 开压缩机 */
            {
                freeze_energy_time[pm_type] = AF_TIME_INIT_MAX;
                antifreeze_temp_exit_bak[antifreeze_type] = antifreeze_temp_exit;
                *antifreeze_status = ANTIFREEZE_OPEN_COMP;
            }
            else if (antifreeze_temp_exit <= val_anti_pump_on)       /* 维持水泵开启 */
            {
                if (!(*fg_antifreeze_IBH_need))
                {
                    if (freeze_energy_time[pm_type] > ANTI_CHG_DLY)
                    {
                        *antifreeze_status = ANTIFREEZE_CLOSE_COMP;		/* 关压机退防冻 */
                        freeze_energy_time[pm_type] = 0;
                    }
                }
                else		/* 电热防冻 */
                {
                    if (freeze_energy_time[pm_type] > ANTI_EXIT_DLY)
                    {
                        *antifreeze_status = ANTIFREEZE_CLOSE_COMP;		/* 关压机退防冻 */
                        freeze_energy_time[pm_type] = 0;
                    }
                }
            }
            else                                                /*  停水泵，退出防冻 */
            {
                if (!(*fg_antifreeze_IBH_need))
                {
                    *antifreeze_status = ANTIFREEZE_CLOSE_COMP;		/* 关压机退防冻 */
                    freeze_energy_time[pm_type] = 0;
                }
                else		/* 电热防冻 */
                {
                    if (freeze_energy_time[pm_type] > ANTI_EXIT_DLY)
                    {
                        *antifreeze_status = ANTIFREEZE_CLOSE_COMP;		/* 关压机退防冻 */
                        freeze_energy_time[pm_type] = 0;
                    }
                }
            }
            
            if (temp_env > val_anti_env_in + val_anti_env_diff      //环境温度退防冻 */ 
            ||  pv_TWin1_max > AF_SV_TEMP_FREQ
            )
            {            
                *antifreeze_status = ANTIFREEZE_CLOSE_COMP;		/* 关压机退防冻 */
                freeze_energy_time[pm_type] = 0;
            }
            break;
        case ANTIFREEZE_OPEN_COMP:
            if (log_sys_have_cell_pump_run(sub_sys_type))
            {
                if (freeze_energy_time[pm_type] >= 480)               /*每过8分钟加载一台压机 */
                {
                    if (log_sys_have_comp_run(MODE_COOL, sub_sys_type))
                    {
                        /* 如果当前压机在制冷运行，则需要先停子系统中的压机 */
                        log_sys_select_comp_stop_once(MODE_COOL, sub_sys_type);
                    }
                    else
                    {                   
                        if (antifreeze_temp_exit - antifreeze_temp_exit_bak[antifreeze_type] >= 10)   /* 温升大于1 */
                        {
                            ;
                        }
                        else        /* 温升小于1时加载 */
                        {
                            if(log_sys_enegry_add(MODE_HEAT, sub_sys_type))   
                            {
    //                          fg_energy_have_add = TRUE;
                            }
                            else
                            {
    //                          fg_energy_have_add = FALSE;
                                *fg_antifreeze_IBH_need = TRUE;
                            }
                        }
                    }

                    /* 把freeze_energy_time保存到cnt_freeze_energy_time中 */
                    if (freeze_energy_time[pm_type] == AF_TIME_INIT_MAX)
                    {
                        cnt_freeze_energy_time[pm_type] = 0;
                    }
                    else
                    {
                        cnt_freeze_energy_time[pm_type] += freeze_energy_time[pm_type];
                    }
                    freeze_energy_time[pm_type] = 0;
                    antifreeze_temp_exit_bak[antifreeze_type] = antifreeze_temp_exit;
                }

                *fg_antifreeze_COMPf_need = TRUE;	/* 有压机开启需求 */
            }
            else
            {
                /* 2023-1-13：仅仿真人工修改变量才会触发到这里来，清零防止因超时退出防冻 */
                freeze_energy_time[pm_type] = 480;
            }
            
            if (antifreeze_temp_exit > val_anti_comp_off
            ||  freeze_energy_time[pm_type] > ANTI_EXIT_DLY
            ||  cnt_freeze_energy_time[pm_type] > ANTI_EXIT_DLY
            ||  temp_env > val_anti_env_in + val_anti_env_diff
            ||  pv_TWin1_max > AF_SV_TEMP_FREQ
            )
            {
                *antifreeze_status = ANTIFREEZE_CLOSE_COMP;		/* 关压机退防冻 */
                freeze_energy_time[pm_type] = 0;
                cnt_freeze_energy_time[pm_type] = 0;
            }
            break;
        case ANTIFREEZE_CLOSE_COMP:
            *fg_antifreeze_IBH_need = FALSE;
            
            if (freeze_energy_time[pm_type] >= 2)   /* 每两秒停一台压机 */
            {
                freeze_energy_time[pm_type] = 0;
                log_sys_select_comp_stop_once(MODE_HEAT, sub_sys_type);
            }
            
            if (!log_sys_have_comp_run(MODE_HEAT, sub_sys_type))      /* 制热压机停机，转到停水泵 */
            {
                if (antifreeze_type == AF_IDLE_STA
                && *antifreeze_stage == AF_STAGE_AC
                &&  val_hotw_used
                )
                {
                    *antifreeze_stage = AF_STAGE_HOTW;	    /* 切换到热水侧防冻 */
                    *antifreeze_status = ANTIFREEZE_PUMP;
                    freeze_energy_time[pm_type] = 0;
                }
                else
                {
                    *antifreeze_status = ANTIFREEZE_CLOSE_PUMP;   /*  关水泵退防冻 */
                    freeze_energy_time[pm_type] = 0;
                }
                *fg_antifreeze_COMPf_need = FALSE;          /* 无压机开启需求 */
            }
            break;
        case ANTIFREEZE_CLOSE_PUMP:
            if (freeze_energy_time[pm_type] > 60)  /* 所有压机都关闭后延时60秒停水泵，退出防冻 */ 
            {
                *fg_antifreeze = FALSE;
                *fg_antifreeze_pump_need = FALSE;    /* 关空调泵 */
                *antifreeze_status = ANTIFREEZE_IDLE;
                *antifreeze_stage = AF_STAGE_IDLE;
            }
            break;
        default:
            break;
    }
}

/*******************************************************************************
 函 数: log_sys_get_flag_area_antifreeze()
 功 能: 获取区域防冻标志，及防冻标志
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
U16 log_sys_get_flag_area_antifreeze(void)
{
    #define NULL_AF         0x0000
    #define AREA_A_AF       0X0001
    #define AREA_B_AF       0X0002
    #define AREA_HOTW_AF    0X0004
    
    #define IDEL_AF         0x0010
    #define RUN_AC_AF       0x0020
    #define RUN_HOTW_AF     0x0040
    #define AREA_A_TAF      0x0080
    #define AREA_B_TAF      0x0100
    #define AREA_HOTW_TAF   0x0200
    
    U16 area_antifreeze_info = NULL_AF;

    if (ZONE_A_AF) area_antifreeze_info |= AREA_A_AF;
    if (ZONE_B_AF) area_antifreeze_info |= AREA_B_AF;
    if (HOTW_AF)   area_antifreeze_info |= AREA_HOTW_AF;
    
    if (fg_idle_sta_antifreeze) area_antifreeze_info |= IDEL_AF;
    if (fg_run_sta_ac_antifreeze) area_antifreeze_info |= RUN_AC_AF;
    if (fg_run_sta_hotw_antifreeze) area_antifreeze_info |= RUN_HOTW_AF;
    if (ZONE_A_TAF) area_antifreeze_info |= AREA_A_TAF;
    if (ZONE_B_TAF) area_antifreeze_info |= AREA_B_TAF;
    if (HOTW_TAF) area_antifreeze_info |= AREA_HOTW_TAF;
        
    return area_antifreeze_info;
}

/*******************************************************************************
 函 数: log_sys_refri_recycle()
 功 能: 收氟：冷媒回收
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_refri_recycle(void)
{
    static U16 cnt_refri;

    /* 收氟判断 */
    if (val_recycle_enter || cmd_refri_recy_enter)
    {
        cmd_refri_recy_enter = FALSE;
        
        if (!fg_idle_sta_antifreeze
        &&  !fg_refri_recy_act
        )
        {
            /* 待机或停机状态下，允许进入收氟 */
            if (status_run == _IDLE || status_run == _DELAY)
            {
                fg_refri_recy_act = TRUE;       /* 进入收氟 */
                SETCMD_N(ZONE_A_ON);			/* A区开机 */
            }
        }
    }

    /* 收氟超时退出 */
    if (cnt_refri >= 8*60)
    {
        cnt_refri = 0;
        cmd_refri_recy_stop = TRUE;
    }

    /* 处理退出收氟 */
    if (cmd_refri_recy_stop)
    {
        cmd_refri_recy_stop = FALSE;

        if (fg_refri_recy_act)
        {
            fg_refri_recy_act = FALSE;

            cmd_refri_recy_enter = FALSE;
            para_set_cent(P_GaI(val_recycle_enter), FALSE);
            SETCMD_N(ZONE_A_OFF);			/* A区关机 */
        }
    }

    /* 计时处理 */
    if (F_TIME_BASIC_SEC_BACK)
    {
        _CNT_WHEN(cnt_refri, fg_refri_recy_act);
    }
}

/*******************************************************************************
 函 数: log_sys_need_by_user()
 功 能: 末端使用者开机后，获取开机状态(用于真正开启)
 参 数: 空
 返 回: TRUE/FALSE--成功/失败
 其 它: 是否能够开启
*******************************************************************************/
BOOL log_sys_need_by_user(void)
{
    BOOL fg_switch_together_run = TRUE;     // 联动允许启动否

    if (REMOTE_IS_UNION_KEEP && !m_get_input(IN_REMOTE)     /* 联动开关断开 */
    ||  !fcu_has_egy_need()                                 /* 风机盘管无需求 */
    )
    {
        fg_switch_together_run = FALSE;
    }

    return fg_switch_together_run;
}

#if 0
/*******************************************************************************
 函 数: log_sys_interlock_run()
 功 能: 开机后，在_READY状态下，进行二通阀联锁闭合启动判断
 参 数: 空
 返 回: TRUE/FALSE--成功/失败
 其 它: 是否能够开启
*******************************************************************************/
BOOL log_sys_interlock_run(void)
{
    BOOL fg_interlock_closed;
    BOOL fg_link_switch = LINK_UNLOCK;            /* 二通阀断开，停机 */

    if (ctrl_mode==MODE_COOL                    /* 制冷时，二通阀联锁功能有效 */
    || (ctrl_mode==MODE_HEAT && val_link_en))       /* 制热且【制热二通联锁】为使用时，二通阀联锁功能有效 */
    {
        fg_link_en = TRUE;  /* 二通阀联锁有效 */
    }
    else    /* 制热且【制热二通联锁】为禁用时，二通阀联锁功能无效 */
    {
        fg_link_en = FALSE;  /* 二通阀联锁无效 */
    }
    
    /* 二通阀联锁 */
    if (REMOTE_IS_INTERLOCK)
    {
        if (!fg_link_en)    /* 联锁失效，直接开机 */
        {
            fg_interlock_closed = TRUE;
        }
        else    /* 联锁有效，时间到之后开机 */
        {
            fg_interlock_closed = FALSE;    /* 复位 */

            /* 二通阀联锁闭合(=0) */
            if (!m_get_input(IN_LINK))   /* 二通阀信号，=1为断开，=0为闭合 */
            {
                fg_link_switch = LINK_LOCK; 
            }

            /* 闭合持续1分钟 */
            if ( fg_link_switch == LINK_LOCK && link_on_cnt >= LINK_LAST) 
            {
                fg_interlock_closed = TRUE;
            }
        }        
    }
    else    /* 不使用二通联锁，直接通过 */
    {
        fg_interlock_closed = TRUE;
    }  

    return fg_interlock_closed;
}

/*******************************************************************************
 函 数: log_sys_interlock_stop()
 功 能: 开机后，在_READY/_RUN状态下，进行二通阀联锁断开停机判断
 参 数: 空
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL log_sys_interlock_stop(void)
{
    BOOL fg_interlock_opened;
    BOOL fg_link_switch = LINK_LOCK;            /* 二通阀锁住，正常运行 */

    if (ctrl_mode==MODE_COOL                    /* 制冷时，二通阀联锁功能有效 */
    || (ctrl_mode==MODE_HEAT && val_link_en))       /* 制热且【制热二通联锁】为使用时，二通阀联锁功能有效 */
    {
       fg_link_en = TRUE;  /* 二通阀联锁有效 */
    }
    else    /* 制热且【制热二通联锁】为禁用时，二通阀联锁功能无效 */
    {
        fg_link_en = FALSE;  /* 二通阀联锁无效 */
    }

    /* 二通阀联锁有效 */
    if (REMOTE_IS_INTERLOCK)
    {
        if (!fg_link_en)    /* 联锁失效，不能停机 */
        {
            fg_interlock_opened = FALSE;
        }
        else    /* 联锁有效，时间到之后停机 */
        {
            fg_interlock_opened = FALSE;
        
            /* 二通阀联锁断开(=1) */
            if (m_get_input(IN_LINK))   /* 二通阀信号，=1为断开，=0为闭合 */
            {
                fg_link_switch = LINK_UNLOCK; 
            }

            /* 断开持续1分钟 */
            if ( fg_link_switch == LINK_UNLOCK && link_off_cnt >= LINK_LAST) 
            {
                fg_interlock_opened = TRUE;
            }
        }
    }
    else
    {
        fg_interlock_opened = FALSE;
    }  

    return fg_interlock_opened;
}
#endif

/*******************************************************************************
 函 数: log_sys_to_ready_by_together()
 功 能: 联动开关断开后，执行跳转到准备态
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_to_ready_by_together(void)
{
    static BOOL fg_step_to_ready;
    static U08 to_ready_stop_time;              // 转到准备态时停机间隔计时

    if (STEP_IS_ALARM || STEP_IS_IDLE || STEP_IS_DELAY)
    {
        fg_step_to_ready = FALSE;
    }
    else if (StepIsReady && status_ready > READY_IDLE
         ||  StepIsRun)
    {
        // 判断是否需要转换到ready
        if (!log_sys_need_by_user())
        {
            fg_step_to_ready = TRUE;
        }
    }

    // 跳转的执行过程，先关压机电热，最后延时关水泵
    if (fg_step_to_ready)
    {
        if (to_ready_stop_time > 3)                         // 停压机 
        {
            to_ready_stop_time = 0;
            
            if (!log_sys_have_comp_run(MODE_NULL, SUB_SYS_TYPE_MAX))
            {
                status_run = _READY;            // 压机都已停，转到_READY
                status_ready = READY_IDLE;      // _READY态为空闲态，准备中
                fg_step_to_ready = FALSE;
            }
            
            log_sys_select_comp_stop(MODE_NULL);
        }
    }

    /* 计时 */
    if (F_TIME_BASIC_SEC_BACK)
    {
        _CNT(to_ready_stop_time);
    }
}

#if 0
void log_sys_two_way_valve_interlock(void)
{
    static BOOL fg_step_to_ready = FALSE;
    static U08 to_ready_stop_time;              // 转到准备态时停机间隔计时

    if (STEP_IS_ALARM || STEP_IS_IDLE)
    {
        fg_link_en = FALSE;  /* 二通阀联锁无效 */
        fg_step_to_ready = FALSE;
    }
    else if (StepIsReady && status_ready > READY_IDLE
         ||  StepIsRun)
    {
        // 判断是否需要转换到ready
        if (log_sys_interlock_stop())
        {
            fg_step_to_ready = TRUE;
        }
        else
        {
            fg_step_to_ready = FALSE;
        }
    }

    // 跳转的执行过程，先关压机电热，最后延时关水泵
    if (fg_step_to_ready)
    {
        if (to_ready_stop_time > 3)                         // 停压机 
        {
            to_ready_stop_time = 0;
            
            if (!log_sys_have_comp_run(MODE_NULL))
            {
                status_run = _READY;            // 压机都已停，转到_READY
                status_ready = READY_IDLE;      // _READY态为空闲态，准备中
                fg_step_to_ready = FALSE;
            }
            
            log_sys_select_comp_stop(MODE_NULL);
        }
    }

    /* 计时 */
    if (F_TIME_BASIC_SEC_BACK)
    {
        _CNT(to_ready_stop_time);
    }

}
#endif

/*==========================================
    功能: 开机过程
/==========================================*/
void log_sys_ready(void)
{
    switch(status_ready)
    {
        case READY_IDLE:
            if ((fg_ac_pump_firstopen==0 && ready_time>random_time) // 首次开水泵（主要防止上电同时启动）    
                || fg_ac_pump_firstopen==1                          // 非首次开水泵
            )
            {
                ready_time = 0;
                if (fg_warm_complete
                 && power_time > POWER_ON_PREHEAT+1			/* 增加原因：POWER_ON_PREHEAT＞POWER_ON_INTO_WORK */
                 && !fg_idle_sta_antifreeze
                 && log_sys_need_by_user()
                 )
                {
                    status_ready = READY_PUMP;
                }
            }
            break;
        case READY_PUMP:
            fg_ac_pump_firstopen=1; // 已经开过水泵  
            if (ready_time > 5)
            {
                status_run = _RUN;
            }
//            if (ready_time > AC_PUMP_ON_DLY || log_sys_send_fan_avoid_cold())
//            {
//                status_ready = READY_COOL_PUMP;
//                ready_time = 0;
//            }
            break;
//        case READY_COOL_PUMP:   /* 取消开机时的冷却泵延时 */
//            if (AIR_WATER)
//            {
//                if (ready_time > val_err_ac_flux_delay +2)
//                    status_run = _RUN;
//            }
//            else
//                status_run = _RUN;
//            break;
        default: break; 
    }
}


/*******************************************************************************
 函 数: log_sys_delay()
 功 能: 停机过程处理
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_delay(U08 pcb_num)
{
    U08 i;

    fg_have_stop = TRUE;    /* 停机也置位该标志 */
    
    for(i=0; i<m_val_comp_num(pcb_num); i++)
    {
        fg_need_comp_to_run[i] = 0;
    }
        
    switch (status_delay)
    {
        case DELAY_IDLE :
            delay_time = 0xff;
            status_delay = DELAY_COMP;
            break;
            
        case DELAY_COMP:        
            if (delay_time > 3)                  /* 停压机 */
            {
                delay_time = 0;
                
                if (!log_sys_have_comp_run(MODE_NULL, SUB_SYS_TYPE_MAX))
                {
                    status_delay = DELAY_COOL_PUMP;       /* 压机都已停机，延时停水泵 */
//                    ws_pump_dly = 0;
                }
                
                log_sys_select_comp_stop(MODE_NULL);  /* 注意: 此句应放在判断压机运行之后, 否则某些压机可能无法关掉电磁阀 */
            }
            break;
            
        case DELAY_COOL_PUMP:                   /* 停冷却水泵 */
        //  if (ws_pump_dly >= WS_PUMP_OFF_DLY)
            {
                status_delay = DELAY_PUMP;
            }
            break;
            
        case DELAY_PUMP:                         /* 停空调水泵 */
            if (air_pump_dly[SUB_SYS_AC_HOTW] >= AC_PUMP_OFF_DLY
            && air_pump_dly[SUB_SYS_AC_ONLY] >= AC_PUMP_OFF_DLY
            )
            {
                status_delay = DELAY_VALVE;
            }

            if (!log_sys_have_cell_run())
            {
              if(fg_machine_need_open)
                status_run = _READY;
            }
            break;
            
        case DELAY_VALVE:
            if (!log_sys_have_cell_run())
            {
                if (fg_machine_need_open == TRUE)
                {
                    status_run = _READY;
                }
                else
                {
                    status_run = _IDLE;
                }
            }
            else    /* 防止压机还未停完 */
            {
                status_delay = DELAY_COMP;
            }
            break;
            
        default:
            break;
    }
}
/*****************************************************
    功能: 故障态
*****************************************************/
void log_sys_alarm(void)
{   
    if (log_sys_have_cell_run())
    {
        if (fg_stop_all_mac)
        {
            cmd_stop_all_mac();
        }
        else
        {
            cmd_stop_all_comp();
        }
    }

    fg_have_stop = TRUE;
}

/******************************************************
    函数：Logic_Reset
    功能：处理当机组待机或故障时的复位
    参数：无
    参数：无
*******************************************************/
void log_sys_idle(U08 pcb_num)
{
    U08 i;

    fg_have_stop = TRUE;
    
    /* 防冻时不初始化以下变量 */
    if (fg_idle_sta_antifreeze				/* 防冻时不初始化以下变量 */
    ||  TF_EN               /* 商检时不初始化以下变量 */
    )
    {
        return;
    }
    
    if (log_sys_have_cell_run())    /* 还有模块在运行, 转到停机态 */
    {
        status_run = _DELAY;
    }

    /* 独立模块运行初始化 */
    m_ctrl_time = 0xffff;   /* 主模块能调控制时间 */
    s_ctrl_time = 0xffff;   /* 从模块能调控制时间 */
    
//    fg_ac_pm_open = FALSE; 
    
    for(i=0; i<m_val_comp_num(pcb_num); i++)
    {
        fg_need_comp_to_run[i] = 0;
    }
}
/***********************************************
函数： log_sys_is_rdy
功能： 系统模式转换时，水泵开启后才能转换到对应的模式
参数： 模块序号
返回值： 需求模式可开
         TRUE: 可开; FALSE: 不可开
************************************************/
U08 log_sys_is_rdy(void)
{
//  U08 j;
    U08 fg_comp_mode_wait = FALSE;
//  static U08 sys_mode_last;  /* 原来的模式 */
    
    if (status_run == _IDLE)    /* 待机状态 */
    {
        if (!fg_idle_sta_antifreeze         /* 未待机态防冻 */
        &&  !TF_EN                          /* 未商检 */
        )
        {
//          sys_mode_last = 0xff;
            fg_comp_mode_wait = TRUE;
        }
    }
    
//    if (!log_sys_AC_is_ready() ) /* 空调泵冷却泵未开，等待 */
//    {
//        fg_comp_mode_wait = TRUE;
//    }
        

    return !fg_comp_mode_wait;
}

/*******************************************************************************
 函 数: m_set_di()
 功 能: 系统开关量考虑常开常闭后的开关量处理
 参 数: input 开关量序号  di_para 常开常闭参数
 di_para: 0 常开 ； 1 常闭
 返 回: 空
 其 它: 
*******************************************************************************/
void m_set_di(U08 input, I16 di_para)
{
    if (m_get_input_first(input) ^ di_para)
    {
        m_set_input(input);
    }
}

#if 0
/*******************************************************************************
 函 数: log_sys_ctrl_temp_cpst()
 功 能: 根据出水温度补偿循环水路温度
 参 数: tmp_ctrl--待补偿温度; 
 返 回: 补偿后温度
 其 它: 空
*******************************************************************************/
I16 log_sys_circle_water_cpst(I16 tmp_ctrl)
{
    #define TMP_ENV1 (10*30)
    #define TMP_ENV2 (10*23)
    #define TMP_DIFF (10*2)    

    #define val_cpst_coef_high              (3)     // 水温补偿系数高
    #define val_cpst_coef_mid               (2)     // 水温补偿系数中
    #define val_cpst_coef_low               (1)     // 水温补偿系数低

    I16 result = tmp_ctrl;
    BOL fg_en = FALSE;
    I16 tmp_coef = 0;
    I16 tmp_out = pv_sys_TWout2;
    I16 tmp_env = pv_sys_TA;

    /* 是否需要补偿 */
    if (SensorCanUse(tmp_out) 
    &&  tmp_out>val_cpst_bgn_tmp
    &&  val_cpst_bgn_tmp!=0 /* 为0表示禁用该功能 */
    )
    {
        fg_en = TRUE;
    }

    if (fg_en)
    {
        /* 确定补偿系数 */
        tmp_coef = tmp_env>TMP_ENV1 ? val_cpst_coef_high :
                    tmp_env>TMP_ENV2 ? val_cpst_coef_mid :
                     val_cpst_coef_low;
                     
        /* 确定补偿后温度 */
        result = fun_pv_cmpst(result, (tmp_out-val_cpst_bgn_tmp)*tmp_coef/5);
    }

    return result;
}
#endif

#if 0
/*******************************************************************************
 函 数: log_sys_get_load_cpst_value()
 功 能: 根据负荷高低获取修正值
 参 数: 空
 返 回: val_cpst：>0 →低负荷修正；<0 →高负荷修正；=0 →无修正
 其 它: 
    1、因使用了内部计时，故该函数一次大循环仅允许被执行一次
    2、本函数仅计算修正值，修正值如何使用由外部处理
*******************************************************************************/
I16 log_sys_get_load_cpst_value(void)
{
    static I16 val_cpst;        /* 修正值 */
    
    static U16 time_low_load;   /* 低负荷计时 */
    static U16 time_high_load;  /* 高负荷计时 */
    static U16 cnt_low_load;    /* 低负荷计数 */
    static U16 cnt_high_load;   /* 高负荷计数 */
    #define LOAD_CPST_NUM 3     /* 连续触发次数 */

    static BOL fg_enable;       /* 允许判断使能标志 */
    static I16 sv_temp_bak;     /* 目标温度备份 */


    /* 计时 */
    {
        if (F_TIME_BASIC_MIN_BACK)          /* 分计时 */
        {
            _CNT(time_low_load);
            _CNT(time_high_load);
        }

        if (!STEP_IS_RUN)
        {
            time_low_load = 0;
            time_high_load = 0;
        }
        else
        {
            if (bak_ac_need==_ADD)
            {
                fg_enable = TRUE;
            }
            
            if (bak_ac_need==_SUBB || bak_ac_need==_QUICK)
            {
                /* 低负荷 */
                if (fg_enable)
                {
                    if (time_low_load < val_load_revise_time)
                    {
                        _CNT(cnt_low_load);
                        cnt_high_load = 0;  /* 低负荷时清空高负荷计数、计时 */
                        time_high_load = 0;
                    }
                }

                /* 到温停机处理 */
                {
                    time_low_load = 0;
                    time_high_load = 0;
                    fg_enable = FALSE;
                }
                
            }

            /* 高负荷 */
            if (time_high_load >= val_load_revise_time)
            {
                time_high_load = 0;
                
                _CNT(cnt_high_load);
                cnt_low_load = 0;           /* 高负荷时清空低负荷计数、计时 */
                time_low_load = 0;
            }
        }
    }

    /* 计算修正值 */
    if (val_load_revise_range == 0
    ||  !STEP_IS_RUN
    ||  sv_temp_bak != sv_temp_energy
        )
    {
        val_cpst = 0;
    }
    else
    {
        if (cnt_low_load >= LOAD_CPST_NUM)
        {
            cnt_low_load = 0;
            val_cpst += 10;
        }
        else if (cnt_high_load >= LOAD_CPST_NUM)
        {
            cnt_high_load = 0;
            val_cpst -= 10;
        }

        val_cpst = fun_pv_limit(val_cpst, -val_load_revise_range, val_load_revise_range);
    }

    /* 后处理 */
    {
        sv_temp_bak = sv_temp_energy;
    }

    return val_cpst;
}

/*******************************************************************************
 函 数: log_sys_use_load_cpst_value()
 功 能: 使用修正值：考虑运行模式判断、温控范围限制等
 参 数: 空
 返 回: 供直接使用的修正值
 其 它: 空
*******************************************************************************/
I16 log_sys_use_load_cpst_value(U08 cpst_set)
{
    I16 cpst_load = 0;          /* 原始修正值 */
    I16 sv_max, sv_min;
    I16 cpst_add_max = 0;
    I16 cpst_sub_max = 0;

    if (temp_ctrl_type_zone_A == ROOM_TEMP_CTRL)
    {
        sv_max = (ac_mode == MODE_COOL) ? 10*para_get_max_cent(P_GaI(sv_room_cool)) : 10*para_get_max_cent(P_GaI(sv_room_heat));
        sv_min = (ac_mode == MODE_COOL) ? 10*para_get_min_cent(P_GaI(sv_room_cool)) : 10*para_get_min_cent(P_GaI(sv_room_heat));
    }
    else
    {
        sv_max = 10*GetSettingRange(log_sys_get_ctrl_object(), GET_MAX, ac_mode);
        sv_min = 10*GetSettingRange(log_sys_get_ctrl_object(), GET_MIN, ac_mode);
    }

    if (val_fcu_egy_cpst_obj == cpst_set)
    {
        /* 有风盘则使用风盘判断，无风盘则使用算法判断 */
#ifdef COM_FCU_FBUS        
        if (val_fcu_num > 0)
        {
            cpst_load = fcu_energy_cpst();
        }
        else
#endif            
        {
//            cpst_load = log_sys_get_load_cpst_value();
            cpst_load = 0;
        }

        switch (cpst_set)
        {
            default:
            case CPST_PV:
                /* 修正值使用方向处理 */
                if (ac_mode == MODE_COOL)     cpst_load = -cpst_load; /* 制冷与制热补偿方向相反 */

                /* 修正值限幅处理:目的是确保把真实水温控制在设定温度上下限范围内 */
                cpst_add_max = sv_temp_energy - sv_min;           /* 最大加补偿 */
                cpst_sub_max = sv_temp_energy - sv_max;           /* 最大减补偿：负数 */
                break;
                
            case CPST_SV:
                /* 修正值使用方向处理 */
                if (ac_mode != MODE_COOL)     cpst_load = -cpst_load; /* 制冷与制热补偿方向相反 */

                /* 修正值限幅处理:目的是确保把真实水温控制在设定温度上下限范围内 */
                cpst_add_max = sv_max - sv_temp_energy;           /* 最大加补偿 */
                cpst_sub_max = sv_min - sv_temp_energy;           /* 最大减补偿：负数 */
                break;
        }

        cpst_load = fun_pv_limit(cpst_load, cpst_sub_max, cpst_add_max);
    }    
    
    return cpst_load;
}
#endif

/*******************************************************************************
 函 数: log_sys_limit()
 功 能: 系统模拟量限制
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
static void log_sys_limit(void)
{
    /* 内机商检时强制检测这几个温度 */
    if (!(TF_HAS_INNER&&TF_EN_NTC))  
    {
        // 不使用水箱温度
        if (val_hotw_used == UNUSED)
        {
            pv_sys_THWt = SENSOR_NOEXIST;
        }

        // 不使用地暖进水温度
        if (val_TFLin_sensor_used == UNUSED)
        {
            pv_sys_TFLin = SENSOR_NOEXIST;
        }

        // 不使用系统总出水温度
        if (val_TWout3_sensor_used == UNUSED)
        {
            pv_sys_TWout3 = SENSOR_NOEXIST;
        }
    }

    /* 不使用太阳能温度 */
    if (!val_TSL_sensor_used)
    {
        pv_sys_TSL = SENSOR_NOEXIST;
    }

    /* 禁用缓冲水箱上部温度探头 */
    if (!val_TACt1_sensor_used)
    {
        pv_sys_TACt1 = SENSOR_NOEXIST;
    }

    /* 禁用平衡水箱下部温度探头 */
    if (!val_TACt2_sensor_used)
    {
        pv_sys_TACt2 = SENSOR_NOEXIST;
    }
}

/*******************************************************************************
 函 数: log_sys_get_TA()
 功 能: 获取去除最大最小后取平均的环境温度
 参 数: 空
 返 回: 去除最大最小后取平均的环境温度
 其 它: 空
*******************************************************************************/
static I16 log_sys_get_TA(void)
{
    U08 pcb = 0;
    I16 temp[MAX_PCB];
    I16 envir_temp = SENSOR_NOEXIST;
    ARRSET(temp, SENSOR_NOEXIST);

    for (pcb=0; pcb<val_pcb_num; pcb++)
    {
        if (CELL_Is_used(pcb) && !err_sys_get_s_commu_err(pcb))
        {
            temp[pcb] = pv_cell_TA(pcb);
        }
    }

    envir_temp = fun_filt_avg(temp, val_pcb_num);
    return envir_temp;
}
#if 0

/*******************************************************************************
 函 数: log_sys_get_TWout2_avg()
 功 能: 获取所有模块的TWout2，并取平均值
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
static I16 log_sys_get_TWout2_avg(void)
{
    U08 pcb = 0;
    I16 temp[MAX_PCB];
    I16 temp_TWout2 = SENSOR_NOEXIST;
    ARRSET(temp, SENSOR_NOEXIST);

    for (pcb=0; pcb<val_pcb_num; pcb++)
    {
        if (CELL_Is_used(pcb) && !err_sys_get_s_commu_err(pcb))
        {
            temp[pcb] = pv_cell_TWout2(pcb);
        }
    }

    temp_TWout2 = fun_all_avg(temp, val_pcb_num);
    return temp_TWout2;
}
#endif

/*******************************************************************************
 函 数: log_sys_get_TWout2_ac()
 功 能: 空调总出水温度计算，对所有参与制冷(或制热)模块的TWout2取平均值
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
static I16 log_sys_get_TWout2_ac(void)
{
    U08 pcb = 0;
    I16 temp[MAX_PCB];
    I16 temp_ac_TWout2 = SENSOR_NOEXIST;
    U08 sub_sys_type = SUB_SYS_TYPE_MAX;	/* 默认: 所有模块均制冷(或制热) */

    /* 初始化 */
    ARRSET(temp, SENSOR_NOEXIST);

    /* 更新子系统范围 */
    if ((energy_ctrl_type == GROUP_ENERGY_CTRL)
    && fg_hotw_run
    )
    {
        /* 热水运行时，由空调+热水子系统当前的控制模式决定可制冷(或制热)模块的具体范围 */
        sub_sys_type = MODE_IS_HOTW(SUB_SYS_AC_HOTW) ? SUB_SYS_AC_ONLY : SUB_SYS_TYPE_MAX; 
    }

    for (pcb = PCB_BGN(sub_sys_type); pcb < PCB_END(sub_sys_type); pcb++)
    {
        if (CELL_Is_used(pcb) && !err_sys_get_s_commu_err(pcb))
        {
            temp[pcb] = pv_cell_TWout2(pcb);
        }
    }

    temp_ac_TWout2 = fun_all_avg(temp, val_pcb_num);
    return temp_ac_TWout2;
}

#if 0
/*******************************************************************************
 函 数: log_sys_get_TWout1_avg()
 功 能: 获取可用后取平均的出水温度
 参 数: 空
 返 回: 出水温度
 其 它: 空
*******************************************************************************/
static I16 log_sys_get_TWout1_avg(void)
{
    U08 pcb = 0;
    I16 temp[MAX_PCB];
    I16 temp_out = SENSOR_NOEXIST;
    ARRSET(temp, SENSOR_NOEXIST);

    for (pcb=0; pcb<val_pcb_num; pcb++)
    {
        if (CELL_Is_used(pcb) && !err_sys_get_s_commu_err(pcb))
        {
            temp[pcb] = pv_cell_TWout1(pcb);
        }
    }

    temp_out = fun_all_avg(temp, val_pcb_num);
    return temp_out;
}

/*******************************************************************************
 函 数: log_sys_get_TWin1_avg()
 功 能: 获取去除最大最小后取平均的回水温度
 参 数: 空
 返 回: 回水温度
 其 它: 空
*******************************************************************************/
static I16 log_sys_get_TWin1_avg(void)
{
    U08 pcb = 0;
    I16 temp[MAX_PCB];
    I16 temp_in = SENSOR_NOEXIST;
    ARRSET(temp, SENSOR_NOEXIST);

    for (pcb=0; pcb<val_pcb_num; pcb++)
    {
        if (CELL_Is_used(pcb) && !err_sys_get_s_commu_err(pcb))
        {
            temp[pcb] = pv_cell_TWin1(pcb);
        }
    }

    temp_in = fun_filt_avg(temp, val_pcb_num);
    return temp_in;
}
#endif

/*******************************************************************************
 函 数: log_sys_get_elec()
 功 能: 计算各模式下的总功率
 参 数: 空
 返 回: 
 其 它: 单位：0.01kw
*******************************************************************************/
static void log_sys_get_elec(void)
{
    U08 pcb;

    if (!USE_ELEC_METERING)
    {
        pv_sys_cl_pwr = SENSOR_NOEXIST;
        pv_sys_ht_pwr = SENSOR_NOEXIST;
        pv_sys_hw_pwr = SENSOR_NOEXIST;

        return;
    }
    else
    {
        /* 累计模块功率前，先清零 */
        pv_sys_cl_pwr = 0;
        pv_sys_ht_pwr = 0;
        pv_sys_hw_pwr = 0;
    }
    
    for (pcb = 0; pcb < val_pcb_num; pcb++)
    {
        switch (ctrl_mode[GET_SUB_SYS(pcb)])
        {
            case MODE_COOL:
                pv_sys_cl_pwr += DIVIDE10(pv_cell_power(pcb));
                break;

            case MODE_HEAT:
                pv_sys_ht_pwr += DIVIDE10(pv_cell_power(pcb));
                break;

            case MODE_HOTW:
                pv_sys_hw_pwr += DIVIDE10(pv_cell_power(pcb));
                break;
        }
    }
}


/*******************************************************************************
 函 数: log_sys_begin_reset()
 功 能: 系统资源数据复位
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
static void log_sys_begin_reset(void)
{
    U08 i;

    sys_input = 0;                          /* 开产量输入刷新(处理常开常闭后的) */
    for (i=0; i<MAX_COMP+1; i++)
    {
        cell[MAX_PCB].cell_relay[i] = 0;    /* 继电器数据清零 */
        cell[MAX_PCB].cell_input[i] = 0;    /* 开产量输入刷新(开关量原始值) */
    }
}

/*******************************************************************************
 函 数: log_sys_begin_input()
 功 能: 系统开关量输入刷新
 参 数: 空
 返 回: 无
 其 它: 空
*******************************************************************************/
static void log_sys_begin_input(void)
{
    /* 外机商检时不检测内机开关量 */
    if (!TF_IS_OUT)
    {
        /* 开关量原始值处理 */
        /*-------物理位置--------; --------逻辑位置-------*/  
        // 常开常闭前
//        if (GET_DI(DI_01))  m_set_input_first(M_IN_WATER_FLOW);	/* 水流不足 */
        if (GET_DI(DI_02))  m_set_input_first(IN_REMOTE);       /* 线控开关 */
        if (GET_DI(DI_06))  m_set_input_first(IN_SG);           /* SG信号 */
        if (GET_DI(DI_07))  m_set_input_first(IN_EVU);          /* EVU信号 */
        
        if (GET_DI(DI_09))  m_set_input_first(M_DI_TBH_OL);     /* 水箱电热反馈 */
        if (GET_DI(DI_10))  m_set_input_first(IN_RT_HEAT);      /* 温控器制热信号 */
        if (GET_DI(DI_11))  m_set_input_first(IN_RT_COOL);      /* 温控器制冷信号 */
        if (GET_DI(DI_12))  m_set_input_first(IN_SOLAR);        /* 太阳能信号 */

        // 常开常闭后
//        m_set_di(M_IN_WATER_FLOW, val_DI_type_air_flux);		/* 水流不足 */
        m_set_di(IN_REMOTE, NORMAL_OPEN);                       /* 线控开关(固定常开) */
        m_set_di(IN_SG, val_DI_type_SG);                        /* SG信号 */
        m_set_di(IN_EVU, val_DI_type_EVU);                      /* EVU信号 */
        m_set_di(M_DI_TBH_OL, NORMAL_CLOSE);		                /* 水箱电热反馈(固定常闭) */
        m_set_di(IN_RT_HEAT, NORMAL_OPEN);                      /* 温控器制热信号(固定常开) */
        m_set_di(IN_RT_COOL, NORMAL_OPEN);                      /* 温控器制冷信号(固定常开) */
        m_set_di(IN_SOLAR, NORMAL_OPEN);                        /* 太阳能信号(固定常开) */
    }
}

/*******************************************************************************
 函 数: log_sys_exio_begin_pv()
 功 能: IO扩展板系统模拟量更新
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
static void log_sys_exio_begin_pv(void)
{
    pv_sys_TA  = log_sys_get_TA();      /* 系统环温 */
    if (USE_GB_DBG)
    {
        pv_sys_TA  = fun_pv_cmpst(pv_sys_TA, val_drytemp_cpst);    // 环境温度
    }
    // 环境温度稳定3S后再更新使用。
    // 防止探头故障时强制切换到0度时渐变影响自动补偿等的计算（可能会影响设定值及云端数据）
    if(pv_sys_TA!=pv_sys_environment_tmp)  // 环境温度消抖
    {
        pv_environment_stable_dly = 0;
        pv_sys_environment_tmp=pv_sys_TA;
        
    }
    if(pv_environment_stable_dly >= 3 && pv_sys_environment_update!=pv_sys_environment_tmp) 
    {
        pv_sys_environment_update=pv_sys_environment_tmp;       
    }
}

/*******************************************************************************
 函 数: log_sys_begin_pv()
 功 能: 系统模拟量输入刷新
 参 数: 空
 返 回: 无
 其 它: 空
*******************************************************************************/
static void log_sys_begin_pv(void)
{
    /* 模拟量刷新 */
    pv_sys_TR               = room_temp;                /* 室内温度 */

#ifdef COM_FCU_FBUS
    fcu_rd_update();
#endif

//    pv_sys_TWout2           = log_sys_get_TWout2_avg();		/* 总出水温度 */
    pv_sys_TWout2_ac        = log_sys_get_TWout2_ac();		/* 空调总出水温度 */

    pv_sys_TACt1            = GET_TEMP(TEMP_03);			/* 缓冲水箱上部温度 */
    pv_sys_TACt2            = GET_TEMP(TEMP_02);			/* 缓冲水箱下部温度 */
    pv_sys_THWt             = GET_TEMP(TEMP_10);            /* 热水水箱温度 */
    pv_sys_TSL              = GET_TEMP(TEMP_04);            /* 太阳能温度 */
    pv_sys_TWout3           = GET_TEMP(TEMP_11);            /* 系统总出水温度 */
    pv_sys_TFLin            = GET_TEMP(TEMP_12);            /* 地暖进水温度 */

    pv_sys_S3V1_state       = msg_sta_S3V1;	                /* 电磁三通阀1状态(字符串型) */
    pv_sys_S3V2_state       = msg_sta_S3V2;	                /* 电磁三通阀2状态(字符串型) */
    pv_sys_S3V3_state       = msg_sta_S3V3;	                /* 电磁三通阀3状态(字符串型) */

    log_sys_get_elec();
    
    log_sys_limit();
}

/*******************************************************************************
 函 数: log_sys_begin_init()
 功 能: 系统初始化命令
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
static void log_sys_begin_init(void)
{
    if (STEP_IS_IDLE || STEP_IS_ALARM)      /* 待机时才可初始化 */
    {
        if (key_fac_para_init) 
        {
            cmd_fac_para_init();    
        }
        if (key_fac_limit_init) 
        {
            cmd_limit_init();   
        }
        if (key_fac_invt_init) 
        {
            cmd_invt_init();
        }
        if (key_runtime_init) 
        {
            cmd_runtime_init();         
        }
        if (key_systime_init) 
        {
            cmd_systime_init();         
        }
        
        if (key_password_init) 
        {
            cmd_password_init();            
        }
        if (key_alldata_init) 
        {
//          cmd_alldata_init();    
            cmd_fac_para_init();    
            cmd_runtime_init();  
            cmd_password_init();   

            key_alldata_init = FALSE;

        }
    }
    else 
    {
        key_fac_para_init = FALSE;
        key_runtime_init = FALSE;
        key_systime_init = FALSE;
    }
}

/*******************************************************************************
 函 数: log_sys_begin()
 功 能: 系统起始，包含:系统资源数据复位
                       系统开关量输入刷新
                       系统模拟量输入刷新
                       系统初始化命令
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
static void log_sys_begin(void)
{
    log_sys_begin_reset();
    log_sys_begin_input();
    log_sys_begin_pv();
    log_sys_exio_begin_pv();
    log_sys_begin_init();
    
    /* 能量调节方式确认 */
    if (IS_DUAL_SYS
    && val_hotw_used		                /* 热水功能使用 */
//    && fg_hotw_run
    )
    {
        energy_ctrl_type = GROUP_ENERGY_CTRL;				/* 分组能调 */
    }
    else
    {
        energy_ctrl_type = JOINT_ENERGY_CTRL;				/* 默认: 联合能调 */
    }

    /* 空调+热水子系统能调环境温度限制压机开启 */
    fg_TA_limit_comp_open[SUB_SYS_AC_HOTW] = log_sys_TA_limit_comp_is_off(ctrl_mode[SUB_SYS_AC_HOTW], SUB_SYS_AC_HOTW);

    /* 单空调子系统能调环境温度限制压机开启 */
    fg_TA_limit_comp_open[SUB_SYS_AC_ONLY] = log_sys_TA_limit_comp_is_off(ctrl_mode[SUB_SYS_AC_ONLY], SUB_SYS_AC_ONLY);

    /* 太阳能信号限制压缩机开启 */
    fg_HDIsl_limit_comp_open = log_sys_HDIsl_limit_comp_is_off();

    /* 太阳能温度限制压缩机开启 */
    fg_TSL_limit_comp_open = log_sys_TSL_limit_comp_is_off();

    /* 地暖预热限制压缩机开启 */
    fg_floor_preheat_limit_comp_open = log_sys_floor_preheat_limit_comp_is_off();
}

/***********************************************
函数：Logic_OutputChange
功能：处理输出转换，将输出状态传给最终输出变量
参数：模块序号
返回值：无
*************************************************/
void log_sys_end(void)
{
    SHC_set_sys_test_do();
       
    /* 除霜指示 DFR */
    if (get_output(MAX_PCB, M_DO_DFR))               
    {
        SET_DO(DO_16);
    }
    /* 故障指示 ERR */
    if (get_output(MAX_PCB, M_DO_ERR))               
    {
        SET_DO(DO_17);
    }
    /* 外部热源 AHS */
    if (get_output(MAX_PCB, M_DO_AHS))               
    {
        SET_DO(DO_18);
    }
    /* 电磁三通阀1 S3V1 */
    if (get_output(MAX_PCB, M_DO_S3V1))
    {
        SET_DO(DO_02);
        msg_sta_S3V1 = msg_S3V_state_no;
    }
    else
    {
        msg_sta_S3V1 = msg_S3V_state_nc;
    }
    /* 电磁三通阀3 S3V3 */
    if (get_output(MAX_PCB, M_DO_S3V3))              
    {
        SET_DO(DO_03);
        msg_sta_S3V3 = msg_S3V_state_no;
    }
    else
    {
        msg_sta_S3V3 = msg_S3V_state_nc;
    }
    /* 电磁三通阀2 S3V2 */
    if (get_output(MAX_PCB, M_DO_S3V2))              
    {
        SET_DO(DO_04);
        msg_sta_S3V2 = msg_S3V_state_no;
    }
    else
    {
        msg_sta_S3V2 = msg_S3V_state_nc;
    }
    
    if (!m_get_input(M_DI_TBH_OL))
    {
        /* 水箱电加热 TBH */
        if (get_output(MAX_PCB, M_DO_TBH))                            
        {
            SET_DO(DO_07);
        }
    }

    /* 下回水泵 PUMPret */
    if (get_output(MAX_PCB, M_DO_PUMPret))           
    {
        SET_DO(DO_09);
    }
    /* 太阳能水泵 PUMPsl */
    if (get_output(MAX_PCB, M_DO_PUMPsl))
    {
        SET_DO(DO_10);
    }
    /* 混合水泵 PUMPm */
    if (get_output(MAX_PCB, M_DO_PUMPm))
    {
        SET_DO(DO_11);
    }
    /* 外循环泵 PUMPo */
    if (get_output(MAX_PCB, M_DO_PUMPo))
    {
        SET_DO(DO_12);
    }
//    /* 变频水泵 PUMPf */
//    if (get_output(MAX_PCB, M_DO_PUMPf))             SET_DO(DO_01);
    
}

/*******************************************************************************
 函 数: log_sys_get_ac_add_diff()
 功 能: 计算空调能调加载偏差
 参 数: mode--; 
 返 回: 
 其 它: 空
*******************************************************************************/
I16 log_sys_get_ac_add_diff(U08 mode)
{
    I16 add_diff;
    BOL fg_outlet_water_ctrl = TRUE;		/* 控出水标志 */

    /* 室温控制 */
    if (ONE_ZONE_ROOM_TEMP_CTRL
    && !fg_sv_temp_energy_overwrite_ac           /* 无温度覆盖时 */
    )
    {
        return val_TR_diff;
    }

    if (USE_AC_TEMP_IN_CTRL)
    {
        /* 使用回水控制 */
        fg_outlet_water_ctrl = FALSE;
    }

    /* 获取原始加载偏差 */
    if (mode == MODE_COOL)
    {
        add_diff = val_cool_add_diff;
    }
    else
    {
        add_diff = val_heat_add_diff;
    }

    /* 控出水 */
    if (fg_outlet_water_ctrl)
    {
//        add_diff += INOUT_10DIFF*(Cmax-Cnow)/Cmax;
        add_diff += INOUT_10DIFF;			/* 针对分体机，先固定为5℃ */
    }
    
    return add_diff;
}

/*******************************************************************************
 函 数: log_sys_get_ac_sub_diff()
 功 能: 计算空调能调卸载偏差
 参 数: mode--; 
 返 回: 
 其 它: 空
*******************************************************************************/
I16 log_sys_get_ac_sub_diff(U08 mode)
{
    if (ONE_ZONE_ROOM_TEMP_CTRL
    && !fg_sv_temp_energy_overwrite_ac           /* 无温度覆盖时 */
    )
    {
        return 0;
    }
    
    return (mode == MODE_COOL) ? val_cool_sub_diff : val_heat_sub_diff;
}

/*******************************************************************************
 函 数: log_sys_get_sv_temp_energy_cool()
 功 能: 
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
I16 log_sys_get_sv_temp_energy_cool(void)
{
    I16 sv_temp_zone_A_preset = SENSOR_NOEXIST;
    I16 sv_temp_zone_B_preset = SENSOR_NOEXIST;
    I16 sv_temp = SENSOR_NOEXIST;

    temp_ctrl_type_zone_A_cool = WATER_TEMP_CTRL;
    temp_set_can_edit_zone_A_cool = FALSE;
    temp_set_can_edit_zone_B_cool = FALSE;
    fg_sv_temp_energy_overwrite_cool = FALSE;

    /* 制冷试运行 */
    if (GETBIT_N(fg_test_run, TEST_RUN_COOL_MODE))
    {
        fg_sv_temp_energy_overwrite_cool = TRUE;
        sv_temp = temp_sv_disp_zone_A_cool = temp_sv_disp_zone_B_cool = TEST_RUN_COOL_MODE_SV_COOL;
        return sv_temp;
    }
    
    /* 收氟 */
    if (fg_refri_recy_act)
    {
        fg_sv_temp_energy_overwrite_cool = TRUE;
        sv_temp = temp_sv_disp_zone_A_cool = temp_sv_disp_zone_B_cool = 50;
        return sv_temp;
    }

    /* 能力测试 */
    if (fg_ability_test_run)
    {
#ifndef USE_AT_SV_FIX
        temp_set_can_edit_zone_A_cool = TRUE;
#endif
        fg_sv_temp_energy_overwrite_cool = TRUE;
        sv_temp = temp_sv_disp_zone_A_cool = temp_sv_disp_zone_B_cool = EXIT_ABILITY_TEST_COOL_SV;
        return sv_temp;
    }

    /* 计算预设目标温度/预设目标温度(B) */
    if (!fg_holiday_away_run    /* 非假日离家模式 */
    && !fg_holiday_home_run	    /* 非假日在家模式 */
    )
    {
        sv_temp_zone_A_preset = WC_get_weather_curve_sv_temp(pv_sys_TA, val_zone_A_cool_weather_curve, MODE_COOL);
        sv_temp_zone_B_preset = WC_get_weather_curve_sv_temp(pv_sys_TA, val_zone_B_cool_weather_curve, MODE_COOL);
    }

    /* 单区室温能调 */
    if (ONE_ZONE_ROOM_TEMP_CTRL)
    {      
        temp_ctrl_type_zone_A_cool = ROOM_TEMP_CTRL;		/* 室温控制 */
        temp_set_can_edit_zone_A_cool = TRUE;
        temp_sv_disp_zone_A_cool = sv_room_cool;		/*【制冷设定室温】*/
    }

    /* 单区水温能调 */
    if (ONE_ZONE_WATER_TEMP_CTRL)
    {
        temp_ctrl_type_zone_A_cool = WATER_TEMP_CTRL;		/* 水温控制 */
        if (SensorNoExist(sv_temp_zone_A_preset))
        {
            temp_set_can_edit_zone_A_cool = TRUE;
            temp_sv_disp_zone_A_cool = sv_cool;
        }
        else
        {
            temp_set_can_edit_zone_A_cool = FALSE;
            temp_sv_disp_zone_A_cool = sv_temp_zone_A_preset;
        }
    }

    /* 双区水温能调 */
    if (DUAL_ZONE_WATER_TEMP_CTRL)
    {
        /* A区 */
        temp_ctrl_type_zone_A_cool = WATER_TEMP_CTRL;		/* 水温控制 */
        if (SensorNoExist(sv_temp_zone_A_preset))
        {
            temp_set_can_edit_zone_A_cool = TRUE;
            temp_sv_disp_zone_A_cool = sv_cool;
        }
        else
        {
            temp_set_can_edit_zone_A_cool = FALSE;
            temp_sv_disp_zone_A_cool = sv_temp_zone_A_preset;
        }

        /* B区 */
        temp_ctrl_type_zone_B_cool = WATER_TEMP_CTRL;		/* 水温控制 */
        if (SensorNoExist(sv_temp_zone_B_preset))
        {
            temp_set_can_edit_zone_B_cool = TRUE;
            temp_sv_disp_zone_B_cool = sv_cool_zone_B;
        }
        else
        {
            temp_set_can_edit_zone_B_cool = FALSE;
            temp_sv_disp_zone_B_cool = sv_temp_zone_B_preset;
        }
    }

    /* 双区混合能调 */
    if (DUAL_ZONE_HYBRID_CTRL)
    {
        /* A区 */
        temp_ctrl_type_zone_A_cool = WATER_TEMP_CTRL;		/* 水温控制 */
        if (SensorNoExist(sv_temp_zone_A_preset))
        {
            temp_set_can_edit_zone_A_cool = TRUE;
            temp_sv_disp_zone_A_cool = sv_cool;
        }
        else
        {
            temp_set_can_edit_zone_A_cool = FALSE;
            temp_sv_disp_zone_A_cool = sv_temp_zone_A_preset;
        }
   
        /* B区 */
        temp_ctrl_type_zone_B_cool = ROOM_TEMP_CTRL;		/* 室温控制 */
        if (SensorNoExist(sv_temp_zone_B_preset))
        {
            temp_set_can_edit_zone_B_cool = TRUE;
            temp_sv_disp_zone_B_cool = sv_room_cool_zone_B;
            sv_temp_zone_B_preset = WC_get_weather_curve_sv_temp(pv_sys_TA, TEMP_CURVE_LO_4, MODE_COOL);/* 曲线设为禁用时，使用默认曲线 */
        }
        else
        {
            temp_set_can_edit_zone_B_cool = FALSE;
            temp_sv_disp_zone_B_cool = sv_temp_zone_B_preset;
        }
    }
    
    /* 线控器双区混合控制 */
    if (USE_UI35_DUAL_ZONE_HYBRID_CTRL)
    {
        if (!fg_zone_B_run)                         /* B区未运行 */
        {
            sv_temp = temp_sv_disp_zone_A_cool;
        }
        else if (fg_zone_B_run && !fg_zone_A_run)   /* B区运行，且A区未运行 */
        {
            sv_temp = sv_temp_zone_B_preset;
        }
        else
        {
            sv_temp = fun_pv_min(temp_sv_disp_zone_A_cool, sv_temp_zone_B_preset);
        }
    }
    else
    {
        if (!fg_zone_B_run)                         /* B区未运行 */
        {
            sv_temp = temp_sv_disp_zone_A_cool;
        }
        else if (fg_zone_B_run && !fg_zone_A_run)   /* B区运行，且A区未运行 */
        {
            sv_temp = temp_sv_disp_zone_B_cool;
        }
        else
        {
            sv_temp = fun_pv_min(temp_sv_disp_zone_A_cool, temp_sv_disp_zone_B_cool);
        }
    }

    return sv_temp;
}

/*******************************************************************************
 函 数: log_sys_get_sv_temp_energy_heat()
 功 能: 获取制热能调目标温度
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
I16 log_sys_get_sv_temp_energy_heat(void)
{
    I16 sv_temp_zone_A_preset = SENSOR_NOEXIST;
    I16 sv_temp_zone_B_preset = SENSOR_NOEXIST;
    I16 sv_temp = SENSOR_NOEXIST;

    temp_ctrl_type_zone_A_heat = WATER_TEMP_CTRL;
    temp_set_can_edit_zone_A_heat = FALSE;
    temp_set_can_edit_zone_B_heat = FALSE;
    fg_sv_temp_energy_overwrite_heat = FALSE;
    
    /* 制热试运行 */
    if (GETBIT_N(fg_test_run, TEST_RUN_HEAT_MODE))
    {
        fg_sv_temp_energy_overwrite_heat = TRUE;
        sv_temp = temp_sv_disp_zone_A_heat = temp_sv_disp_zone_B_heat = TEST_RUN_HEAT_MODE_SV_HEAT;
        return sv_temp;
    }

    /* 能力测试 */
    if (fg_ability_test_run)
    {
#ifndef USE_AT_SV_FIX
        temp_set_can_edit_zone_A_heat = TRUE;
#endif
        fg_sv_temp_energy_overwrite_heat = TRUE;
        sv_temp = temp_sv_disp_zone_A_heat = temp_sv_disp_zone_B_heat = EXIT_ABILITY_TEST_HEAT_SV;
        return sv_temp;
    }

    /* 假日离开模式 */
    if (fg_holiday_away_run
    && fg_holiday_away_heat_mode_used
    )
    {
        fg_sv_temp_energy_overwrite_heat = TRUE;
        sv_temp = temp_sv_disp_zone_A_heat = temp_sv_disp_zone_B_heat = val_holiday_away_sv_heat;
        return sv_temp;
    }

    /* 地暖烘干功能 */
    if (fg_floor_dryup_run)
    {
        fg_sv_temp_energy_overwrite_heat = TRUE;
        sv_temp = temp_sv_disp_zone_A_heat = temp_sv_disp_zone_B_heat = dryup_sv_temp;
        return sv_temp;
    }

    /* 地暖预热功能 */
    if (fg_floor_preheat_run)
    {
        fg_sv_temp_energy_overwrite_heat = TRUE;
        sv_temp = temp_sv_disp_zone_A_heat = temp_sv_disp_zone_B_heat = val_floor_preheat_temp;
        return sv_temp;
    }

    /* 计算预设目标温度/预设目标温度(B) */
    if (!fg_holiday_away_run    /* 非假日离家模式 */
    && !fg_holiday_home_run	    /* 非假日在家模式 */
    )
    {
        sv_temp_zone_A_preset = WC_get_weather_curve_sv_temp(pv_sys_TA, val_zone_A_heat_weather_curve, MODE_HEAT);
        sv_temp_zone_B_preset = WC_get_weather_curve_sv_temp(pv_sys_TA, val_zone_B_heat_weather_curve, MODE_HEAT);
    }

    /* 单区室温能调 */
    if (ONE_ZONE_ROOM_TEMP_CTRL)
    {
        temp_ctrl_type_zone_A_heat = ROOM_TEMP_CTRL;		/* 室温控制 */
        temp_set_can_edit_zone_A_heat = TRUE;
        temp_sv_disp_zone_A_heat = sv_room_heat;		/*【制热设定室温】*/
    }

    /* 单区水温能调 */
    if (ONE_ZONE_WATER_TEMP_CTRL)
    {
        temp_ctrl_type_zone_A_heat = WATER_TEMP_CTRL;		/* 水温控制 */
        if (SensorNoExist(sv_temp_zone_A_preset))
        {
            temp_set_can_edit_zone_A_heat = TRUE;
            temp_sv_disp_zone_A_heat = sv_heat;
        }
        else
        {
            temp_set_can_edit_zone_A_heat = FALSE;
            temp_sv_disp_zone_A_heat = sv_temp_zone_A_preset;
        }
    }

    /* 双区水温能调 */
    if (DUAL_ZONE_WATER_TEMP_CTRL)
    {
        /* A区 */
        temp_ctrl_type_zone_A_heat = WATER_TEMP_CTRL;		/* 水温控制 */
        if (SensorNoExist(sv_temp_zone_A_preset))
        {
            temp_set_can_edit_zone_A_heat = TRUE;
            temp_sv_disp_zone_A_heat = sv_heat;
        }
        else
        {
            temp_set_can_edit_zone_A_heat = FALSE;
            temp_sv_disp_zone_A_heat = sv_temp_zone_A_preset;
        }

        /* B区 */
        temp_ctrl_type_zone_B_heat = WATER_TEMP_CTRL;		/* 水温控制 */
        if (SensorNoExist(sv_temp_zone_B_preset))
        {
            temp_set_can_edit_zone_B_heat = TRUE;
            temp_sv_disp_zone_B_heat = sv_heat_zone_B;
        }
        else
        {
            temp_set_can_edit_zone_B_heat = FALSE;
            temp_sv_disp_zone_B_heat = sv_temp_zone_B_preset;
        }
    }

    /* 双区混合能调 */
    if (DUAL_ZONE_HYBRID_CTRL)
    {
        /* A区 */
        temp_ctrl_type_zone_A_heat = WATER_TEMP_CTRL;		/* 水温控制 */
        if (SensorNoExist(sv_temp_zone_A_preset))
        {
            temp_set_can_edit_zone_A_heat = TRUE;
            temp_sv_disp_zone_A_heat = sv_heat;
        }
        else
        {
            temp_set_can_edit_zone_A_heat = FALSE;
            temp_sv_disp_zone_A_heat = sv_temp_zone_A_preset;
        }

        /* B区 */
        temp_ctrl_type_zone_B_heat = ROOM_TEMP_CTRL;		/* 室温控制 */
        if (SensorNoExist(sv_temp_zone_B_preset))
        {
            temp_set_can_edit_zone_B_heat = TRUE;
            temp_sv_disp_zone_B_heat = sv_room_heat_zone_B; 
            sv_temp_zone_B_preset = WC_get_weather_curve_sv_temp(pv_sys_TA, TEMP_CURVE_LO_4, MODE_HEAT);/* 曲线设为禁用时，使用默认曲线 */
        }
        else
        {
            temp_set_can_edit_zone_B_heat = FALSE;
            temp_sv_disp_zone_B_heat = sv_temp_zone_B_preset;
        }
    }

    /* 线控器双区混合控制 */
    if (USE_UI35_DUAL_ZONE_HYBRID_CTRL)
    {
        if (!fg_zone_B_run)                         /* B区未运行 */
        {
            sv_temp = temp_sv_disp_zone_A_heat;
        }
        else if (fg_zone_B_run && !fg_zone_A_run)   /* B区运行，且A区未运行 */
        {
            sv_temp = sv_temp_zone_B_preset;
        }
        else
        {
            sv_temp = fun_pv_max(temp_sv_disp_zone_A_heat, sv_temp_zone_B_preset);
        }
    }
    else
    {
        if (!fg_zone_B_run)                         /* B区未运行 */
        {
            sv_temp = temp_sv_disp_zone_A_heat;
        }
        else if (fg_zone_B_run && !fg_zone_A_run)   /* B区运行，且A区未运行 */
        {
            sv_temp = temp_sv_disp_zone_B_heat;
        }
        else
        {
            sv_temp = fun_pv_max(temp_sv_disp_zone_A_heat, temp_sv_disp_zone_B_heat);
        }
    }

    return sv_temp;
}

/*******************************************************************************
 函 数: log_sys_get_sv_disp_zone_A()
 功 能: 获取A区设定温度来显示
 参 数: grp--;
		idx--;
		value--;
 返 回: 空
 其 它: 若是固定值则改写value，若是参数值则改写grp、idx
*******************************************************************************/
void log_sys_get_sv_disp_zone_A(U08 *grp, U08 *idx, I16 *value)
{
    I16 sv_temp_zone_A_preset = SENSOR_NOEXIST;

	if (ac_mode == MODE_COOL)
	{
		/* 制冷试运行 */
	    if (GETBIT_N(fg_test_run, TEST_RUN_COOL_MODE))
	    {
	    	/* 固定值 */
	        *value = TEST_RUN_COOL_MODE_SV_COOL;
	        return;
	    }
	    /* 收氟 */
	    if (fg_refri_recy_act)
	    {
	    	/* 固定值 */
	        *value = 50;
	        return;
	    }

	    /* 能力测试 */
	    if (fg_ability_test_run)
	    {
	    	/* 固定值 */
	        *value = EXIT_ABILITY_TEST_COOL_SV;
	        return;
	    }
	    /* 单区室温能调 */
	    if (ONE_ZONE_ROOM_TEMP_CTRL)
	    {
	    	/* 参数值 */
	    	*grp = P_grp(sv_room_cool);
	    	*idx = P_idx(sv_room_cool);
	        return;
	    }

	    /* 计算预设目标温度/预设目标温度(B) */
	    if (!fg_holiday_away_run    /* 非假日离家模式 */
	    && !fg_holiday_home_run	    /* 非假日在家模式 */
	    )
	    {
	        sv_temp_zone_A_preset = WC_get_weather_curve_sv_temp(pv_sys_TA, val_zone_A_cool_weather_curve, MODE_COOL);
	    }

	    /* 单区水温能调 */
	    if (ONE_ZONE_WATER_TEMP_CTRL)
	    {
	        if (SensorNoExist(sv_temp_zone_A_preset))
	        {
		    	/* 参数值 */
		    	*grp = P_grp(sv_cool);
		    	*idx = P_idx(sv_cool);
		        return;
	        }
	        else
	        {
		    	/* 固定值 */
	            *value = sv_temp_zone_A_preset;
		        return;
	        }
	    }

	    /* 双区水温能调 */
	    if (DUAL_ZONE_WATER_TEMP_CTRL)
	    {
	        if (SensorNoExist(sv_temp_zone_A_preset))
	        {
		    	/* 参数值 */
		    	*grp = P_grp(sv_cool);
		    	*idx = P_idx(sv_cool);
		        return;
	        }
	        else
	        {
		    	/* 固定值 */
	            *value = sv_temp_zone_A_preset;
		        return;
	        }
	    }

	    /* 双区混合能调 */
	    if (DUAL_ZONE_HYBRID_CTRL)
	    {
	        /* A区 */
	        if (SensorNoExist(sv_temp_zone_A_preset))
	        {
		    	/* 参数值 */
		    	*grp = P_grp(sv_cool);
		    	*idx = P_idx(sv_cool);
		        return;
	        }
	        else
	        {
		    	/* 固定值 */
	            *value  = sv_temp_zone_A_preset;
		        return;
	        }
	    }
	}
    else
    {
	    /* 制热试运行 */
	    if (GETBIT_N(fg_test_run, TEST_RUN_HEAT_MODE))
	    {
	        /* 固定值 */
	        *value = TEST_RUN_HEAT_MODE_SV_HEAT;
	        return;
	    }

	    /* 能力测试 */
	    if (fg_ability_test_run)
	    {
	        /* 固定值 */
	        *value = EXIT_ABILITY_TEST_HEAT_SV;
	        return;
	    }

	    /* 假日离开模式 */
	    if (fg_holiday_away_run
	    && fg_holiday_away_heat_mode_used
	    )
	    {
	        /* 固定值 */
	        *value = val_holiday_away_sv_heat;
	        return;
	    }

	    /* 地暖烘干功能 */
	    if (fg_floor_dryup_run)
	    {
	        /* 固定值 */
	        *value = dryup_sv_temp;
	        return;
	    }

	    /* 地暖预热功能 */
	    if (fg_floor_preheat_run)
	    {
	        /* 固定值 */
	        *value = val_floor_preheat_temp;
	        return;
	    }

	    /* 计算预设目标温度/预设目标温度(B) */
	    if (!fg_holiday_away_run    /* 非假日离家模式 */
	    && !fg_holiday_home_run	    /* 非假日在家模式 */
	    )
	    {
	        sv_temp_zone_A_preset = WC_get_weather_curve_sv_temp(pv_sys_TA, val_zone_A_heat_weather_curve, MODE_HEAT);
	    }

	    /* 单区室温能调 */
	    if (ONE_ZONE_ROOM_TEMP_CTRL)
	    {
	        /* 参数值 */
	        *grp = P_grp(sv_room_heat);
	        *idx = P_idx(sv_room_heat);
	        return;
	    }

	    /* 单区水温能调 */
	    if (ONE_ZONE_WATER_TEMP_CTRL)
	    {
	        if (SensorNoExist(sv_temp_zone_A_preset))
	        {
		        /* 参数值 */
		        *grp = P_grp(sv_heat);
		        *idx = P_idx(sv_heat);
		        return;
	        }
	        else
	        {
	            /* 固定值 */
	            *value = sv_temp_zone_A_preset;
		        return;
	        }
	    }

	    /* 双区水温能调 */
	    if (DUAL_ZONE_WATER_TEMP_CTRL)
	    {
	        /* A区 */
	        if (SensorNoExist(sv_temp_zone_A_preset))
	        {
	            /* 参数值 */
	            *grp = P_grp(sv_heat);
	            *idx = P_idx(sv_heat);
		        return;
	        }
	        else
	        {
	        	/* 固定值 */
	            *value = sv_temp_zone_A_preset;
		        return;
	        }
	    }

	    /* 双区混合能调 */
	    if (DUAL_ZONE_HYBRID_CTRL)
	    {
	        /* A区 */
	        if (SensorNoExist(sv_temp_zone_A_preset))
	        {
	            /* 参数值 */
	            *grp = P_grp(sv_heat);
	            *idx = P_idx(sv_heat);
		        return;
	        }
	        else
	        {
	            /* 固定值 */
	            *value = sv_temp_zone_A_preset;
		        return;
	        }
	    }
    }
}

/*******************************************************************************
 函 数: log_sys_get_sv_disp_zone_B()
 功 能: 获取B区设定温度来显示
 参 数: grp--;
		idx--;
		value--;
 返 回: 空
 其 它: 若是固定值则改写value，若是参数值则改写grp、idx
*******************************************************************************/
void log_sys_get_sv_disp_zone_B(U08 *grp, U08 *idx, I16 *value)
{
    I16 sv_temp_zone_B_preset = SENSOR_NOEXIST;

	if (ac_mode == MODE_COOL)
	{
		/* 制冷试运行 */
	    if (GETBIT_N(fg_test_run, TEST_RUN_COOL_MODE))
	    {
	    	/* 固定值 */
	        *value = TEST_RUN_COOL_MODE_SV_COOL;
	        return;
	    }
	    /* 收氟 */
	    if (fg_refri_recy_act)
	    {
	    	/* 固定值 */
	        *value = 50;
	        return;
	    }

	    /* 能力测试 */
	    if (fg_ability_test_run)
	    {
	    	/* 固定值 */
	        *value = EXIT_ABILITY_TEST_COOL_SV;
	        return;
	    }

	    /* 计算预设目标温度/预设目标温度(B) */
	    if (!fg_holiday_away_run    /* 非假日离家模式 */
	    && !fg_holiday_home_run	    /* 非假日在家模式 */
	    )
	    {
	        sv_temp_zone_B_preset = WC_get_weather_curve_sv_temp(pv_sys_TA, val_zone_B_cool_weather_curve, MODE_COOL);
	    }
	    /* 双区水温能调 */
	    if (DUAL_ZONE_WATER_TEMP_CTRL)
	    {
	        if (SensorNoExist(sv_temp_zone_B_preset))
	        {
		    	/* 参数值 */
		    	*grp = P_grp(sv_cool_zone_B);
		    	*idx = P_idx(sv_cool_zone_B);
		        return;
	        }
	        else
	        {
		    	/* 固定值 */
	            *value = sv_temp_zone_B_preset;
		        return;
	        }
	    }

	    /* 双区混合能调 */
	    if (DUAL_ZONE_HYBRID_CTRL)
	    {
	        /* B区 */
	        if (SensorNoExist(sv_temp_zone_B_preset))
	        {
		    	/* 参数值 */
		    	*grp = P_grp(sv_room_cool_zone_B);
		    	*idx = P_idx(sv_room_cool_zone_B);
		        return;
	        }
	        else
	        {
		    	/* 固定值 */
	            *value  = sv_temp_zone_B_preset;
		        return;
	        }
	    }
	}
    else
    {
	    /* 制热试运行 */
	    if (GETBIT_N(fg_test_run, TEST_RUN_HEAT_MODE))
	    {
	        /* 固定值 */
	        *value = TEST_RUN_HEAT_MODE_SV_HEAT;
	        return;
	    }

	    /* 能力测试 */
	    if (fg_ability_test_run)
	    {
	        /* 固定值 */
	        *value = EXIT_ABILITY_TEST_HEAT_SV;
	        return;
	    }

	    /* 假日离开模式 */
	    if (fg_holiday_away_run
	    && fg_holiday_away_heat_mode_used
	    )
	    {
	        /* 固定值 */
	        *value = val_holiday_away_sv_heat;
	        return;
	    }

	    /* 地暖烘干功能 */
	    if (fg_floor_dryup_run)
	    {
	        /* 固定值 */
	        *value = dryup_sv_temp;
	        return;
	    }

	    /* 地暖预热功能 */
	    if (fg_floor_preheat_run)
	    {
	        /* 固定值 */
	        *value = val_floor_preheat_temp;
	        return;
	    }

	    /* 计算预设目标温度/预设目标温度(B) */
	    if (!fg_holiday_away_run    /* 非假日离家模式 */
	    && !fg_holiday_home_run	    /* 非假日在家模式 */
	    )
	    {
	        sv_temp_zone_B_preset = WC_get_weather_curve_sv_temp(pv_sys_TA, val_zone_B_heat_weather_curve, MODE_HEAT);
	    }

	    /* 双区水温能调 */
	    if (DUAL_ZONE_WATER_TEMP_CTRL)
	    {
	        /* B区 */
	        if (SensorNoExist(sv_temp_zone_B_preset))
	        {
	            /* 参数值 */
	            *grp = P_grp(sv_heat_zone_B);
	            *idx = P_idx(sv_heat_zone_B);
		        return;
	        }
	        else
	        {
	        	/* 固定值 */
	            *value = sv_temp_zone_B_preset;
		        return;
	        }
	    }

	    /* 双区混合能调 */
	    if (DUAL_ZONE_HYBRID_CTRL)
	    {
	        /* B区 */
	        if (SensorNoExist(sv_temp_zone_B_preset))
	        {
	            /* 参数值 */
	            *grp = P_grp(sv_room_heat_zone_B);
	            *idx = P_idx(sv_room_heat_zone_B);
		        return;
	        }
	        else
	        {
	            /* 固定值 */
	            *value = sv_temp_zone_B_preset;
		        return;
	        }
	    }
    }
}

/*******************************************************************************
 函 数: log_sys_get_sv_disp_zone_hot()
 功 能: 获取热水区设定温度来显示
 参 数: grp--;
		idx--;
		value--;
 返 回: 空
 其 它: 若是固定值则改写value，若是参数值则改写grp、idx
*******************************************************************************/
void log_sys_get_sv_disp_zone_hot(U08 *grp, U08 *idx, I16 *value)
{
    /* 制热水试运行 */
    if (GETBIT_N(fg_test_run, TEST_RUN_HOTW_MODE))
    {
        /* 固定值 */
        *value = TEST_RUN_HOTW_MODE_SV_HOTW;
        return;
    }

    /* 定时杀菌功能 */
    if (fg_timed_disinfect_run)
    {
        /* 参数值 */
        *grp = P_grp(sv_hotw);
        *idx = P_idx(sv_hotw);
        return;
    }

    /* 假日离开模式 */
    if (fg_holiday_away_run
    && fg_holiday_away_hotw_mode_used
    )
    {
        /* 固定值 */
        *value = val_holiday_away_sv_hotw;
        return;
    }

    /* 免费用电强制运行 */
    if (FREE_ELEC_FORCE_RUN)
    {
        /* 固定值 */
        *value = FREE_ELEC_I_SV_HOTW;
        return;
    }

    /* 参数值 */
    *grp = P_grp(sv_hotw);
    *idx = P_idx(sv_hotw);

    return;
}

/*******************************************************************************
 函 数: log_sys_get_ctrl_temp_energy_ac()
 功 能: 获取空调能调控制温度
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
I16 log_sys_get_ctrl_temp_energy_ac(void)
{
    /* 特定功能，默认使用TWout2作为能调控制温度 */
    if (fg_sv_temp_energy_overwrite_ac)
    {
        ctrl_temp_type_energy_ac = AC_NRG_CTRL_TEMP_AC_TWOUT2;
        return AC_NRG_CTRL_TEMP;                            /* 空调总出水温度 */
    }

    /* 正常运行 */
    if (ONE_ZONE_ROOM_TEMP_CTRL)
    {
        ctrl_temp_type_energy_ac = AC_NRG_CTRL_TEMP_TR;
        return pv_sys_TR;	                                /* 室内温度TR */
    }
    else if (USE_AC_TEMP_TACT1)			                
    {
        ctrl_temp_type_energy_ac = AC_NRG_CTRL_TEMP_TACT1;
        return pv_sys_TACt1;                                /* 缓冲水箱上部温度TACt1 */
    }
    else if (USE_AC_TEMP_TACT2)		                
    {
        ctrl_temp_type_energy_ac = AC_NRG_CTRL_TEMP_TACT2;
        return pv_sys_TACt2;                                /* 缓冲水箱下部温度TACt2 */
    }
    else if (USE_AC_TEMP_TACT1_TACT2)				
    {
        ctrl_temp_type_energy_ac = AC_NRG_CTRL_TEMP_TACT1_TACT2;
        return fun_pv_avg(pv_sys_TACt1, pv_sys_TACt2);		/* 缓冲水箱平均温度AVG(TACt1,TACt2) */
    }
    else                                    
    {
        ctrl_temp_type_energy_ac = AC_NRG_CTRL_TEMP_AC_TWOUT2;
        return pv_sys_TWout2_ac;                            /* 空调总出水温度 */
    }
}

/*******************************************************************************
 函 数: log_sys_get_sv_temp_energy_hotw()
 功 能: 获取热水能调目标温度
 参 数: 空
 返 回: 热水能调目标温度
 其 它: 空
*******************************************************************************/
I16 log_sys_get_sv_temp_energy_hotw(void)
{
    fg_sv_temp_energy_overwrite_hotw = FALSE;
    
    /* 制热水试运行 */
    if (GETBIT_N(fg_test_run, TEST_RUN_HOTW_MODE))
    {
        fg_sv_temp_energy_overwrite_hotw = TRUE;
        return TEST_RUN_HOTW_MODE_SV_HOTW;
    }

    /* 定时杀菌功能 */
    if (fg_timed_disinfect_run)
    {
        return sv_hotw;
    }

    /* 假日离开模式 */
    if (fg_holiday_away_run
    && fg_holiday_away_hotw_mode_used
    )
    {
        fg_sv_temp_energy_overwrite_hotw = TRUE;
        return val_holiday_away_sv_hotw;
    }

    /* 免费用电强制运行 */
    if (FREE_ELEC_FORCE_RUN)
    {
        fg_sv_temp_energy_overwrite_hotw = TRUE;
        return FREE_ELEC_I_SV_HOTW;
    }

    return sv_hotw;
}

/*******************************************************************************
 函 数: log_sys_get_sv_temp_freq_ac()
 功 能: 获取制冷/制热调频目标温度
 参 数: mode--; 
 返 回: 
 其 它: 空
*******************************************************************************/
I16 log_sys_get_sv_temp_freq_ac(U08 mode)
{
    U08 curve_id = TEMP_CURVE_NULL;

    /* 计算调频目标温度 */
    if (temp_ctrl_type_zone_A == ROOM_TEMP_CTRL)
    {
        curve_id = GET_ZONE_A_WEATHER_CURVE(mode);             
        /* 曲线设为禁用时，使用默认曲线 */
        if (curve_id == 0)
        {
            curve_id = TEMP_CURVE_LO_4;
        }
        return WC_get_weather_curve_sv_temp(pv_sys_TA, curve_id, mode);
    }
    else
    {
        return (mode == MODE_COOL) ? sv_temp_energy_cool : sv_temp_energy_heat;
    }
}

/*******************************************************************************
 函 数: log_sys_mode_temp_confirm()
 功 能: 各模式下能调目标温度、调频目标温度、能调控制温度确认
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_mode_temp_confirm(void)
{
    /* 能调目标温度 */
    sv_temp_energy_cool = log_sys_get_sv_temp_energy_cool();        /* 制冷 */
    sv_temp_energy_heat = log_sys_get_sv_temp_energy_heat();        /* 制热 */
    sv_temp_energy_hotw = log_sys_get_sv_temp_energy_hotw();        /* 热水 */

    /* 能调控制温度 */
    ctrl_temp_energy_ac = log_sys_get_ctrl_temp_energy_ac();        /* 制冷/制热 */
    ctrl_temp_energy_hotw = pv_sys_THWt;	                        /* 热水 */

    /* 能调加载偏差 */    
    val_add_diff_energy_cool = log_sys_get_ac_add_diff(MODE_COOL);	/* 制冷 */
    val_add_diff_energy_heat = log_sys_get_ac_add_diff(MODE_HEAT);  /* 制热 */
    val_add_diff_energy_hotw = val_hotw_diff;				        /* 热水 */

    /* 能调卸载偏差 */
    val_sub_diff_energy_cool = log_sys_get_ac_sub_diff(MODE_COOL);	/* 制冷 */
    val_sub_diff_energy_heat = log_sys_get_ac_sub_diff(MODE_HEAT);	/* 制热 */
    val_sub_diff_energy_hotw = 0;			                        /* 热水 */


    /* 调频目标温度 */
    sv_temp_freq_cool = log_sys_get_sv_temp_freq_ac(MODE_COOL);	    /* 制冷 */
    sv_temp_freq_heat = log_sys_get_sv_temp_freq_ac(MODE_HEAT);	    /* 制热 */
    sv_temp_freq_hotw = sv_temp_energy_hotw;	                    /* 热水 */
}

/*******************************************************************************
 函 数: log_sys_mode_need_confirm()
 功 能: 模式需求确认，分别计算制冷需求、制热需求和制热水需求
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_mode_need_confirm(void)
{
    static BOL fg_cool_add = FALSE;
    static BOL fg_heat_add = FALSE;
    static BOL fg_hotw_add = FALSE;

    if (STEP_IS_IDLE)
    {
        fg_cool_need = FALSE;
        fg_heat_need = FALSE;
        fg_hotw_need = FALSE;
        return;
    }

    /* 计算能调控制温度所在区域 */
    cool_area = log_sys_energy_range(MODE_COOL, ctrl_temp_energy_ac, \
                                                sv_temp_energy_cool, \
                                                val_add_diff_energy_cool, \
                                                val_sub_diff_energy_cool);
    
    heat_area = log_sys_energy_range(MODE_HEAT, ctrl_temp_energy_ac, \
                                                sv_temp_energy_heat, \
                                                val_add_diff_energy_heat, \
                                                val_sub_diff_energy_heat);
    
    hotw_area = log_sys_energy_range(MODE_HOTW, ctrl_temp_energy_hotw, \
                                                sv_temp_energy_hotw, \
                                                val_add_diff_energy_hotw, \
                                                val_sub_diff_energy_hotw);
    
    if (cool_area == _ADD)      fg_cool_add = TRUE;
    if (cool_area == _SUBB)     fg_cool_add = FALSE;
    
    if (heat_area == _ADD)      fg_heat_add = TRUE;
    if (heat_area == _SUBB)     fg_heat_add = FALSE;
    
    if (hotw_area == _ADD)      fg_hotw_add = TRUE;
    if (hotw_area == _SUBB)     fg_hotw_add = FALSE;    
    
    /* 制冷需求 */
    fg_cool_need = (fg_zone_A_run||fg_zone_B_run) && fg_cool_add;
    
    /* 制热需求 */
    fg_heat_need = (fg_zone_A_run||fg_zone_B_run) && fg_heat_add;
    
    /* 制热水需求 */
    fg_hotw_need = fg_hotw_run && fg_hotw_add;
                                                                   
    /* 无制热需求 */
    if (USE_COOL_ONLY			            /* 仅制冷 */
    || (ac_mode == MODE_COOL)				/* 制冷模式 */
    )
    {
        fg_heat_need = FALSE;
    }
    
    /* 无制冷需求 */
    if (USE_HEAT_ONLY                       /* 仅制热 */
    || (ac_mode == MODE_HEAT)				/* 制热模式 */
    )          
    {
        fg_cool_need = FALSE;
    }
    
    /* 无制热水需求 */
    if (!val_hotw_used      /* 热水功能禁用 */
    || fg_sg_hotw_forbid    /* 高峰用电 */
    )
    {
        fg_hotw_need = FALSE;
    }
}

/*******************************************************************************
 函 数: log_sys_disp_mode_confirm()
 功 能: 显示模式确认(disp_mode)
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_disp_mode_confirm(void)
{
    BOL fg_switch_need = FALSE;		                        /* 切换需求标志 */
    U08 fg_switch_allow = TRUE;		                        /* 切换允许标志 */
    static U08 T4_area = AREA_00;			                /* 环温分区 */
    static BOL fg_enter_ac_switch_cycle = FALSE;		/* 进入空调模式切换状态标志 */
    static BOL fg_enter_hotw_switch_cycle = FALSE;		/* 进入热水切换状态标志 */

    /* 以下情况，不进行模式切换 */
    if (STEP_IS_IDLE		/* 待机状态 */
    || STEP_IS_ALARM		/* 报警状态 */
    || !val_hotw_used		/* 禁用热水功能 */
    )
    {
        fg_enter_ac_switch_cycle = FALSE;
        fg_enter_hotw_switch_cycle = FALSE;
    }

    /* 计算环温区间 */
    T4_area = fun_area(pv_sys_TA, T4_area, 10, (val_auto_cool_min_TA+10),
                                               val_auto_heat_max_TA,
                                               SENSOR_NOEXIST);

    /* 确认空调模式 */
    if (sv_mode == SET_AUTO)
    {
        if (T4_area == AREA_00)
        {
            ac_mode = MODE_COOL;			/* 制冷模式 */
            if (USE_DUAL_ZONE_CTRL)
            {     
                /* B区关机 */
                SETCMD_N(ZONE_B_OFF);
            }
        }
        else if (T4_area == AREA_02)  
        {
            ac_mode = MODE_HEAT;			/* 制热模式 */
        }
        else if (T4_area == AREA_01 && ac_mode == MODE_NULL) /* 防止死区模式运行 */
        {
            ac_mode = MODE_HEAT;
        }
    }
    else if (sv_mode == SET_COOL)
    {
        ac_mode = MODE_COOL;				/* 制冷模式 */
    }
    else if (sv_mode == SET_HEAT)
    {
        ac_mode = MODE_HEAT;				/* 制热模式 */
    }

    if (GETBIT_N(fg_test_run, TEST_RUN_COOL_MODE))          /* 制冷试运行 */
    {
        ac_mode = MODE_COOL;
    }
    else if (GETBIT_N(fg_test_run, TEST_RUN_HEAT_MODE))     /* 制热试运行 */
    {
        ac_mode = MODE_HEAT;
    }


    /* 单空调子系统 */
    disp_mode[SUB_SYS_AC_ONLY] = ac_mode;

    /* 空调+热水子系统 */
    if (!fg_zone_A_run && !fg_zone_B_run && !fg_hotw_run)           /* A区/B区/热水均关机 */
    {
        disp_mode[SUB_SYS_AC_HOTW] = ac_mode;
        fg_enter_ac_switch_cycle = FALSE;
        fg_enter_hotw_switch_cycle = FALSE;
    }
    else if ((fg_zone_A_run || fg_zone_B_run) && !fg_hotw_run)      /* 仅A区/B区开机 */
    {
        disp_mode[SUB_SYS_AC_HOTW] = ac_mode;
        fg_enter_ac_switch_cycle = FALSE;
        fg_enter_hotw_switch_cycle = FALSE;
    }
    else if (!fg_zone_A_run && !fg_zone_B_run && fg_hotw_run)       /* 仅热水开机 */
    {
        disp_mode[SUB_SYS_AC_HOTW] = MODE_HOTW;
        fg_enter_ac_switch_cycle = FALSE;
        fg_enter_hotw_switch_cycle = FALSE;
    }
    else                                                            /* A区/B区/热水均开机 */
    {
        if (fg_defrost_run[SUB_SYS_AC_HOTW])
        {
            fg_switch_allow = FALSE;
        }
        
        if (IS_DUAL_SYS)
        {
            /* 制冷(或制热)运行时处理 */
            if (MODE_IS_AC(SUB_SYS_AC_HOTW))
            {
                /* 可切换至制热水运行 */
                if (fg_hotw_need                            /* 有制热水需求 */
                && pump_time[PM_F_AC_HW].on > 60*2          /* 水泵运行2min */
                && fg_switch_allow		                    /* 允许切换 */
                )
                {
                    disp_mode[SUB_SYS_AC_HOTW] = MODE_HOTW;
                }
            }

            /* 制热水运行时处理 */
            if (MODE_IS_HOTW(SUB_SYS_AC_HOTW))
            {
                /* 可切换至制冷(或制热)运行 */
                if (!fg_hotw_need                           /* 无制热水需求 */
                && pump_time[PM_F_AC_HW].on > 60*2          /* 水泵运行2min */
                && fg_switch_allow		                    /* 允许切换 */
                )
                {
                    disp_mode[SUB_SYS_AC_HOTW] = ac_mode;
                } 
            }
        }
        else if (USE_HOTW_PRIORITY)              /* 热水模式优先 */
        {           
            if (!fg_enter_hotw_switch_cycle)	/* 未进入热水模式切换 */
            {
                if (MODE_IS_HOTW(SUB_SYS_AC_HOTW))		/* 确保当前控制模式已变为热水模式时，才可进入周期切换 */
                {
                    fg_enter_hotw_switch_cycle = TRUE;
                    fg_enter_ac_switch_cycle = FALSE;
                }
                disp_mode[SUB_SYS_AC_HOTW] = MODE_HOTW;
            }
            else				/* 进入热水模式切换 */
            {
                /* 机组制热水运行时处理 */
                if (MODE_IS_HOTW(SUB_SYS_AC_HOTW))
                {
                    disp_mode[SUB_SYS_AC_HOTW] = MODE_HOTW;
                                     
                    if (USE_AC_TEMP_TWOUT2                          /* 使用TWout2(此时无法检测到空调需求) */
                    || (USE_AC_TEMP_TACT1 && FG_AC_NEED)            /* 有TACt1，且有空调需求 */
                    || (USE_AC_TEMP_TACT2 && FG_AC_NEED)            /* 有TACt2，且有空调需求 */
                    || (USE_AC_TEMP_TACT1_TACT2 && FG_AC_NEED)      /* 有TACt1和TACt2，且有空调需求 */
                    )
                    {
                        fg_switch_need = TRUE;
                    }

                    /* 可提前切换至制冷(或制热)运行 */
                    if (fg_switch_need                                           /* 有切换需求 */
                    && fg_switch_allow		                                     /* 允许切换 */
                    && !fg_hotw_need                                             /* 无制热水需求 */
                    && pump_time[PM_F_AC_HW].on > 60*2                           /* 水泵运行2min */
                    && !log_sys_TA_limit_comp_is_off(MODE_HOTW, SUB_SYS_AC_HOTW) /* 环温处于压机可开区或压机保持区 */
                    )
                    {
                        disp_mode[SUB_SYS_AC_HOTW] = ac_mode;
                    }    

                    /* 制热水运行时间到 */
                    if (hotw_runtime.on > val_hotw_comp_on_max
                    && fg_switch_need                           /* 有切换需求 */
                    &&  fg_switch_allow		                    /* 允许切换 */
                    )       
                    {
                        disp_mode[SUB_SYS_AC_HOTW] = ac_mode;
                    }                    
                }
    
                /* 机组制冷(或制热)运行时处理 */
                if (MODE_IS_AC(SUB_SYS_AC_HOTW))
                {
                    disp_mode[SUB_SYS_AC_HOTW] = ac_mode;

                    /* 可提前切换至制热水运行 */
                    if (!FG_AC_NEED                                             /* 无空调需求 */
                    && fg_switch_allow		                                    /* 允许切换 */
                    && fg_hotw_need                                             /* 有制热水需求 */
                    && pump_time[PM_F_AC_HW].on > 60*2                          /* 水泵运行2min */
                    && !log_sys_TA_limit_comp_is_off(ac_mode, SUB_SYS_AC_HOTW)  /* 环温处于压机可开区或压机保持区 */
                    )
                    {
                        disp_mode[SUB_SYS_AC_HOTW] = MODE_HOTW;
                    }

                    /* 空调运行时间到 */
                    if (fg_switch_allow		                    /* 允许切换 */
                    &&  fg_hotw_need                            /* 有制热水需求 */
                    && ((MODE_IS_HEAT(SUB_SYS_AC_HOTW) && heat_runtime[SUB_SYS_AC_HOTW].on > val_air_comp_on_max)
                        || (MODE_IS_COOL(SUB_SYS_AC_HOTW) && cool_runtime[SUB_SYS_AC_HOTW].on > val_air_comp_on_max)
                        )
                    )		
                    {
                        disp_mode[SUB_SYS_AC_HOTW] = MODE_HOTW;
                    }
                }     
            }
        }
        else                /* 热水模式非优先 */
        {
            if (!fg_enter_ac_switch_cycle)	/* 未进入空调模式切换 */
            {
                if (MODE_IS_AC(SUB_SYS_AC_HOTW))		/* 确保当前控制模式已变为制冷/制热模式时，才可进入周期切换 */
                {
                    fg_enter_ac_switch_cycle = TRUE;
                    fg_enter_hotw_switch_cycle = FALSE;
                }
                disp_mode[SUB_SYS_AC_HOTW] = ac_mode;
            }
            else
            {
                /* 机组制冷(或制热)运行时处理 */
                if (MODE_IS_AC(SUB_SYS_AC_HOTW))
                {
//                    disp_mode = ac_mode;
                    disp_mode[SUB_SYS_AC_HOTW] = ac_mode;

                    /* 可提前切换至制热水运行 */
                    if (!FG_AC_NEED                                             /* 无空调需求 */
                    && fg_switch_allow		                                    /* 允许切换 */
                    && fg_hotw_need                                             /* 有制热水需求 */
                    && pump_time[PM_F_AC_HW].on > 60*2                          /* 水泵运行2min */
                    && !log_sys_TA_limit_comp_is_off(ac_mode, SUB_SYS_AC_HOTW)  /* 环温处于压机可开区或压机保持区 */
                    )
                    {
                        disp_mode[SUB_SYS_AC_HOTW] = MODE_HOTW;
                    }
                }

                /* 机组制热水运行时处理 */
                if (MODE_IS_HOTW(SUB_SYS_AC_HOTW))
                {
                    disp_mode[SUB_SYS_AC_HOTW] = MODE_HOTW;
                                     
                    if (USE_AC_TEMP_TWOUT2)			        /* 使用TWout2，无TACt1、TACt2 */
                    {
                        if (fg_switch_allow		                        /* 允许切换 */
                        && (!fg_hotw_need                               /* 无制热水需求 */
                            || hotw_runtime.on > val_hotw_comp_on_max	/* 制热水运行时间到 */
                            )
                        )
                        {
                            disp_mode[SUB_SYS_AC_HOTW] = ac_mode;
                        }      
                    }
                    else                                    /* 存在TACt1、TACt2 */
                    {
                        /* 可提前切换至制冷(或制热)运行 */
                        if (FG_AC_NEED                                               /* 有空调需求 */
//                        && !fg_hotw_need                                             /* 无制热水需求 */
                        && pump_time[PM_F_AC_HW].on > 60*2                           /* 水泵运行2min */
                        && !log_sys_TA_limit_comp_is_off(MODE_HOTW, SUB_SYS_AC_HOTW) /* 环温处于压机可开区或压机保持区 */
                        && fg_switch_allow		                                     /* 允许切换 */
                        )
                        {
                            disp_mode[SUB_SYS_AC_HOTW] = ac_mode;
                        }    
                    }
                }
            }
        }

        /* 热水系统故障 */
        if (fg_hotw_sys_err)
        {
            disp_mode[SUB_SYS_AC_HOTW] = ac_mode;
        }
    }

    /* 强制覆盖显示模式的情况 */
    {
        /* 强开热水模式功能 */
        if (fg_force_hotw_mode_run)                              
        {
            disp_mode[SUB_SYS_AC_HOTW] = MODE_HOTW;
        }
        
        /* 定时杀菌功能 */
        if (fg_timed_disinfect_run)
        {
            disp_mode[SUB_SYS_AC_HOTW] = MODE_HOTW;
        }
         
        if (fg_ability_test_run                 /* 能力测试 */
        ||  TF_EN                               /* 商检 */
        )              
        {
            ARRSET(disp_mode, ac_mode);
        }

        /* 收氟 */
        if (fg_refri_recy_act)
        {
            ac_mode = MODE_COOL;
            ARRSET(disp_mode, ac_mode);
        }

        /* 空气排空试运行 */
        if (GETBIT_N(fg_test_run, TEST_RUN_AIR_EMPTY))
        {
            switch (test_run_air_empty_stage)
            {
                case TRAE_IDU:
                case TRAE_A_HT:
                case TRAE_B_HT:
                    ac_mode = MODE_HEAT;
                    ARRSET(disp_mode, ac_mode);
                    break;

                case TRAE_HW:
                    disp_mode[SUB_SYS_AC_HOTW] = MODE_HOTW;
                    break;

                case TRAE_A_CL:
                    ac_mode = MODE_COOL;
                    ARRSET(disp_mode, ac_mode);
                    break;
            }       
        } 

        /* 水泵试运行 */
        if (GETBIT_N(fg_test_run, TEST_RUN_PUMP))
        {
            switch (test_run_pump_stage)
            {
                case TRP_HW:
                    disp_mode[SUB_SYS_AC_HOTW] = MODE_HOTW;
                    break;

                case TRP_A_HT:
                case TRP_B_HT:
                    ac_mode = MODE_HEAT;
                    ARRSET(disp_mode, ac_mode);
                    break;

                case TRP_A_CL:
                    ac_mode = MODE_COOL;
                    ARRSET(disp_mode, ac_mode);
                    break;
            }
        }

        /* 制冷试运行 */
        if (GETBIT_N(fg_test_run, TEST_RUN_COOL_MODE))
        {
            ac_mode = MODE_COOL;
            ARRSET(disp_mode, ac_mode);
        }

        /* 制热试运行 */
        if (GETBIT_N(fg_test_run, TEST_RUN_HEAT_MODE))
        {
            ac_mode = MODE_HEAT;
            ARRSET(disp_mode, ac_mode);
        }
     
        /* 制热水试运行 */
        if (GETBIT_N(fg_test_run, TEST_RUN_HOTW_MODE))
        {
            disp_mode[SUB_SYS_AC_HOTW] = MODE_HOTW;
        }

//        if (fg_enter_holiday)       // 休假模式
//        {
//            disp_mode = holiday_mode;
//        }

//        if (room_therostat_mode != MODE_NULL)   // 温控器确定模式
//        {
//            disp_mode = room_therostat_mode;
//        }

        if (fg_floor_preheat_run/* 地暖预热功能 */
        || fg_floor_dryup_run	/* 地暖烘干功能 */
        )   
        {
            ac_mode = MODE_HEAT;
            ARRSET(disp_mode, ac_mode);
        }
    }

    /* 处理热水、制热不停机切换 */
    {
        /* 热水切换到制热 */
        if (MODE_IS_HOTW(SUB_SYS_AC_HOTW) && (disp_mode[SUB_SYS_AC_HOTW]==MODE_HEAT))
        {
            if (ctrl_temp_energy[SUB_SYS_AC_HOTW] < val_err_ac_out_high-val_err_wt_exit)
            {
                SET_HOTW_HEAT_SWITCH_DLY;
            }
        }
        /* 制热切换到热水 */
        if (MODE_IS_HEAT(SUB_SYS_AC_HOTW) && (disp_mode[SUB_SYS_AC_HOTW]==MODE_HOTW))
        {
            if (ctrl_temp_energy[SUB_SYS_AC_HOTW] < val_err_ac_out_high-val_err_wt_exit)
            {
                SET_HOTW_HEAT_SWITCH_DLY;
            }
        }
        /* 无需求时直接清空 */
        if (!fg_heat_need && !fg_hotw_need)
        {
            CLR_HOTW_HEAT_SWITCH_DLY;
        }
    }
}

/*******************************************************************************
 函 数: log_sys_ctrl_mode_confirm()
 功 能: 确定机组运行模式(集中控制模式)
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_ctrl_mode_confirm(void)
{
    #define PV_FILT 3               /* 消抖 */
    static U08 confirm_mode_bak[SUB_SYS_TYPE_MAX];    // 确认的模式：备份

    /* 空调+热水子系统 */
    {
        if (confirm_mode_bak[SUB_SYS_AC_HOTW] != disp_mode[SUB_SYS_AC_HOTW])
        {
            confirm_mode_bak[SUB_SYS_AC_HOTW] = disp_mode[SUB_SYS_AC_HOTW];
            ctrl_mode_dly[SUB_SYS_AC_HOTW] = 0;
        }
    
        /* 确定控制模式 */
        if (!STEP_IS_RUN		                                /* 非运行态就直接切 */
        || ctrl_mode_dly[SUB_SYS_AC_HOTW] > 10
        || GETBIT_N(fg_test_run, TEST_RUN_AIR_EMPTY)			/* 空气排空试运行，不开压机，可直接切模式 */
        || GETBIT_N(fg_test_run, TEST_RUN_PUMP)	                /* 水泵试运行，不开压机，可直接切模式 */
        )  
        {
            ctrl_mode[SUB_SYS_AC_HOTW] = disp_mode[SUB_SYS_AC_HOTW];
        }
    }

    /* 单空调子系统 */
    {
        if (confirm_mode_bak[SUB_SYS_AC_ONLY] != disp_mode[SUB_SYS_AC_ONLY])
        {
            confirm_mode_bak[SUB_SYS_AC_ONLY] = disp_mode[SUB_SYS_AC_ONLY];
            ctrl_mode_dly[SUB_SYS_AC_ONLY] = 0;
        }
    
        /* 确定控制模式 */
        if (!STEP_IS_RUN		                                /* 非运行态就直接切 */
        || ctrl_mode_dly[SUB_SYS_AC_ONLY] > 10
        || GETBIT_N(fg_test_run, TEST_RUN_AIR_EMPTY)			/* 空气排空试运行，不开压机，可直接切模式 */
        || GETBIT_N(fg_test_run, TEST_RUN_PUMP)	                /* 水泵试运行，不开压机，可直接切模式 */
        )  
        {
            ctrl_mode[SUB_SYS_AC_ONLY] = disp_mode[SUB_SYS_AC_ONLY];
        }
    }
}

/*******************************************************************************
 函 数: log_sys_ctrl_mode_get()
 功 能: 封装后板运行模式接口
 参 数: type：选择获取方式
 返 回: 后板运行模式
 其 它: 空
*******************************************************************************/
U08 log_sys_ctrl_mode_get(U08 type)
{
    return MODE_NULL;
}

/*******************************************************************************
 函 数: log_sys_get_ctrl_object()
 功 能: 获取控制对象
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
U16 log_sys_get_ctrl_object(void)
{
    U16 result = TEMP_OUT;

    if (USE_AC_TEMP_IN_CTRL)
    {
        result = TEMP_IN;
    }

    return result;
}

/*******************************************************************************
 函 数: log_sys_ctrl_mode_temp_confirm()
 功 能: 确定: ①能调/调频控制温度、②能调/调频目标温度、③加载/卸载偏差
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_ctrl_mode_temp_confirm(void)  
{   
    U08 i;
    
    switch (ac_mode)
    {
        case MODE_COOL:
            temp_ctrl_type_zone_A = temp_ctrl_type_zone_A_cool;
            temp_ctrl_type_zone_B = temp_ctrl_type_zone_B_cool;
            temp_set_can_edit_zone_A = temp_set_can_edit_zone_A_cool;
            temp_set_can_edit_zone_B = temp_set_can_edit_zone_B_cool;
            fg_sv_temp_energy_overwrite_ac = fg_sv_temp_energy_overwrite_cool;
            break;

        case MODE_HEAT:
            temp_ctrl_type_zone_A = temp_ctrl_type_zone_A_heat;
            temp_ctrl_type_zone_B = temp_ctrl_type_zone_B_heat;
            temp_set_can_edit_zone_A = temp_set_can_edit_zone_A_heat;
            temp_set_can_edit_zone_B = temp_set_can_edit_zone_B_heat;
            fg_sv_temp_energy_overwrite_ac = fg_sv_temp_energy_overwrite_heat;
            break;
    }

    for (i = 0; i < SUB_SYS_TYPE_MAX; i++)
    {
        switch (ctrl_mode[i])
        {
            default:
            case MODE_COOL:
                /* 能调 */
                {
                    sv_temp_energy[i] = sv_temp_energy_cool;            /* 能调目标温度 */
                    ctrl_temp_energy[i] = ctrl_temp_energy_ac;          /* 能调控制温度 */

//                    val_add_diff_energy[i] = val_add_diff_energy_cool;	/* 能调加载偏差 */
                    val_sub_diff_energy[i] = val_sub_diff_energy_cool;	/* 能调卸载偏差 */
                }
    
                /* 调频 */
                if (FG_AF_ALL)
                {
                    sv_temp_freq[i] = AF_SV_TEMP_FREQ;
                }
                else
                {
                    sv_temp_freq[i] = sv_temp_freq_cool;                /* 调频目标温度 */
                }
                break;
                
            case MODE_HEAT:       
                /* 能调 */
                {
                    sv_temp_energy[i] = sv_temp_energy_heat;            /* 能调目标温度 */
                    ctrl_temp_energy[i] = ctrl_temp_energy_ac;          /* 能调控制温度 */

//                    val_add_diff_energy[i] = val_add_diff_energy_heat;  /* 能调加载偏差 */
                    val_sub_diff_energy[i] = val_sub_diff_energy_heat;  /* 能调卸载偏差 */
                }          
    
                /* 调频 */
                {
                    sv_temp_freq[i] = sv_temp_freq_heat;			    /* 调频目标温度 */
                }
                break;
                
            case MODE_HOTW:
                /* 能调 */
                {
                    sv_temp_energy[i] = sv_temp_energy_hotw;            /* 能调目标温度 */
                    ctrl_temp_energy[i] = ctrl_temp_energy_hotw;        /* 能调控制温度 */

//                    val_add_diff_energy[i] = val_add_diff_energy_hotw;  /* 能调加载偏差 */
                    val_sub_diff_energy[i] = val_sub_diff_energy_hotw;  /* 能调卸载偏差 */
                }
    
                /* 调频 */
                {
                    sv_temp_freq[i] = sv_temp_freq_hotw;			    /* 调频目标温度 */
                }
                break;
        }
    }
}

/*********************************************
函数： 
功能： 处理系统的开启与停止
参数：无
返回值：无
*********************************************/
void log_sys_start_and_stop(void)
{ 
    BOOL fg_stop = FALSE;

    if (power_time <= POWER_ON_ONOFF) 
    {
        return;
    }

    /* 强制热水模式功能 */
    on_off_force_hotw_mode();
    
    /* 线控开关 */
    if (REMOTE_IS_ONOFF)       on_off_remote();
    if (REMOTE_IS_UNION_ACT)
    {
        on_off_union_act();
//        if (fg_onoff_union_act) key_stop = FALSE;   /* 联动动作有效：联动开关保持闭合时：机组不能通过线控器进行关机 */

        /* 联动动作有效：联动开关保持闭合时：机组不能通过线控器进行关机 */
        if (fg_onoff_union_act)
        {
            fg_zone_A_off = FALSE;
            fg_zone_B_off = FALSE;
            fg_hotw_off = FALSE;
        }
    }
       
    /* 处理来电自启动 */
    if (!fg_floor_preheat_run
    && !fg_floor_dryup_run
    && !fg_enter_test_run
    )
    {
        static U08  auto_run_bit_bak = 0;

        if (val_remember_run == USED)   /* 来电自启动使用 */
        {
            if ((status_run==_RUN||status_run == _READY)
            &&  TF_IS_IDLE
            )
            {
                fg_zone_A_run ? SETBIT(auto_run_bit_bak, ACT_ZONE_A)
                              : CLRBIT(auto_run_bit_bak, ACT_ZONE_A);

                fg_zone_B_run ? SETBIT(auto_run_bit_bak, ACT_ZONE_B)
                              : CLRBIT(auto_run_bit_bak, ACT_ZONE_B);

                fg_hotw_run ? SETBIT(auto_run_bit_bak, ACT_HOTW)
                            : CLRBIT(auto_run_bit_bak, ACT_HOTW);
                
//                auto_run_bit_bak = ACT_RUN; 
            }
            else
            {
                CLRBIT(auto_run_bit_bak, ACT_ZONE_A);
                CLRBIT(auto_run_bit_bak, ACT_ZONE_B);
                CLRBIT(auto_run_bit_bak, ACT_HOTW);
            }   
            
            if (auto_run_bit != auto_run_bit_bak)
            {
                auto_run_bit = auto_run_bit_bak;
                //auto_run_write_to_eeprom();
                // 未进入杀菌且未进入地暖且未进入强开热水模式且未进入假日模式才保存
                if (fg_floor_dryup_run == FALSE 
                && fg_timed_disinfect_run == FALSE
                && fg_force_hotw_mode_run == FALSE
                && fg_holiday_away_run == FALSE
                && fg_holiday_home_run == FALSE
                )
                {
                    DataAccess_AUTO_RUN(DATA_WRITE);
                }
            }
        }
    }
    
    /* 机组启停控制处理 */
    switch(val_on_off_control_type)
    {
        case ONOFF_SWITCH:          /* 线控 */
            commu_run = FALSE;
            commu_stop = FALSE;
//          break;
        case ONOFF_REMOTE:          /* 远程 */
            key_run = FALSE;
            key_stop = FALSE;
            break;      
            
        case ONOFF_COMMU:           /* 网控 */
            key_run = FALSE;
            key_stop = FALSE;
            break;
            
        case ONOFF_SELF:            /* 本地 */
            commu_run = FALSE;
            commu_stop = FALSE;
            break;
    }
    
    /* 手操器关开关机 */
    if (key_run || commu_run)       /* 收到开机命令 */
    {
        key_run = FALSE;
        commu_run = FALSE;
//        Logic_machine_start();      /* 执行开机操作 */

        fg_zone_A_on = TRUE;
        if (USE_DUAL_ZONE_CTRL)
        {
            fg_zone_B_on = TRUE;
        }
        if (val_hotw_used)
        {
            fg_hotw_on = TRUE;
        }
    }
    if (key_stop || commu_stop)     /* 收到关机命令 */
    {
        key_stop = FALSE;
        commu_stop = FALSE;
//        Logic_machine_stop();       /* 执行关机操作 */

        fg_zone_A_off = TRUE;
        fg_zone_B_off = TRUE;
        fg_hotw_off = TRUE;
    }

    if (fg_accept_remote_upgrade == SH_REMOTE_REQ_OK)
    {
    	fg_zone_A_off = TRUE;
        fg_zone_B_off = TRUE;
        fg_hotw_off = TRUE;
    }

    /* A区运行状态 */
    if (fg_zone_A_off)
    {
        fg_zone_A_off = FALSE;
        fg_zone_A_run = FALSE;
        if (!fg_zone_A_run && !fg_zone_B_run && !fg_hotw_run)
        {
            fg_stop = TRUE;
        }
    }
    else
    {
        if (fg_zone_A_on)
        {
            fg_zone_A_on = FALSE;
            fg_zone_A_run = TRUE;
            Logic_machine_start();          /* 执行开机操作 */
        }
    }

    /* B区运行状态 */
    if (!USE_DUAL_ZONE_CTRL                                                                 /* 非双区控制 */
    || ((ctrl_mode[SUB_SYS_AC_HOTW]==MODE_COOL) && (ctrl_mode[SUB_SYS_AC_ONLY]==MODE_COOL))	/* 制冷运行时 */
    || (MODE_IS_HOTW(SUB_SYS_AC_HOTW) && ac_mode == MODE_COOL)				                /* 制热水运行时，且空调模式为制冷 */
    )
    {
        /* B区开机命令无效 */
        fg_zone_B_on = FALSE;
//        fg_zone_B_off = FALSE;
    }
    if (fg_zone_B_off)
    {
        fg_zone_B_off = FALSE;
        fg_zone_B_run = FALSE;
        if (!fg_zone_A_run && !fg_zone_B_run && !fg_hotw_run)
        {
            fg_stop = TRUE;
        }
    }
    else
    {
        if (fg_zone_B_on)
        {
            fg_zone_B_on = FALSE;
            fg_zone_B_run = TRUE;
            Logic_machine_start();          /* 执行开机操作 */
        }
    }

    /* 热水运行状态 */
    if (!val_hotw_used)		/* 无热水，热水开关机命令无效 */
    {
        fg_hotw_on = FALSE;
        fg_hotw_off = FALSE;
    }
    if (fg_hotw_off)
    {
        fg_hotw_off = FALSE;
        fg_hotw_run = FALSE;
        if (!fg_zone_A_run && !fg_zone_B_run && !fg_hotw_run)
        {
            fg_stop = TRUE;
        }
    }
    else
    {
        if (fg_hotw_on)
        {
            fg_hotw_on = FALSE;
            fg_hotw_run = TRUE;
            Logic_machine_start();          /* 执行开机操作 */
        }
    }

    /* 执行关机操作 */
    if (fg_stop
    && !fg_zone_A_run
    && !fg_zone_B_run
    && !fg_hotw_run
    )
    {
        Logic_machine_stop();               
    }
    
    on_off_limit(); // 开关机限制
}


/*******************************************************************************
 函 数: log_sys_cmd_time()
 功 能: 命令模块计时
 参 数: 空; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_cmd_time(void)
{
    U08 i;
    U08 j;
    
    //----------------------秒计时----------------------------------
    if (F_TIME_BASIC_SEC_BACK)
    {
        // 通讯计时延时 
//        _CNT(commu_cmd_comp_mode_interval);
        _CNT(commu_cmd_comp_all_stop_interval);
        _CNT(commu_cmd_err_reset_interval);
        _CNT(commu_cmd_stop_all_mac_interval);
        _CNT(commu_cmd_fac_para_init_interval);
        _CNT(commu_cmd_invt_init_interval);
        _CNT(commu_cmd_limit_para_init_interval);
        _CNT(commu_cmd_password_save_interval);
        _CNT(commu_cmd_runtime_init_interval);
        _CNT(commu_cmd_systime_init_interval);
        _CNT(commu_cmd_systime_save_interval);

        for (i = 0; i < MAX_PCB; i++)
        {
            _CNT(commu_cmd_cell_open[i]);
            _CNT(commu_cmd_cell_close[i]);
            for (j = 0; j < MAX_COMP; j++)
            {
                _CNT(commu_cmd_comp_quik_stop_interval[i][j]);
                _CNT(commu_cmd_comp_system_stop_interval[i][j]);
                _CNT(commu_cmd_comp_allow_defrost_interval[i][j]);
                _CNT(commu_cmd_comp_force_defrost_interval[i][j]);
            }
        }
    }
}

/*==========================================
    功能: 计时
/==========================================*/
void log_sys_time(void)
{
    U08 i, j;
    
    if (power_time < POWER_ON_INTO_WORK) return;
    
    if (F_TIME_BASIC_SEC_BACK)
    {
//      if (screen_time < 0xfffe) screen_time++;
//      if (key_continuous_time < 0xfffe) key_continuous_time++;
        
        if (ready_time != 0xffff)  ready_time++;
        if (delay_time != 0xff)  delay_time++;

        for (i = 0; i < SUB_SYS_TYPE_MAX; i++)
        {
            _CNT(ctrl_mode_dly[i]);
        }
        
        if (m_ctrl_time != 0xffff)           m_ctrl_time++;
        if (s_ctrl_time != 0xffff)           s_ctrl_time++;
                        
//        if (air_pump_dly != 0xff)        air_pump_dly++;
//      if (ws_pump_dly!= 0xff)     ws_pump_dly++;
        if (ac_heater_dly != 0xff)    ac_heater_dly++;

        if (get_output(MAX_PCB, SW_AC_HT))  
        {
            cnt_elec_stop = 0;
        }
        else
        {
            if(cnt_elec_stop != 0xff) cnt_elec_stop++;
        }

        if(cnt_send_fan_stop != 0xffff) cnt_send_fan_stop++;

        if(avoid_cold_fan_cnt!= 0xffff) avoid_cold_fan_cnt++;
          
//        if(pump_inter_on_off_time!= 0xffff) pump_inter_on_off_time++;  

        for (i = 0; i < SUB_SYS_TYPE_MAX; i++)
        {
            _CNT(air_pump_dly[i]);
            _CNT(pump_inter_on_off_time[i]);
        }
          
        /* 水泵停机时间 */
        for (i=0; i<PM_MAX; i++)      /* 水泵个数 */
        {
            _CNT(pump_time[i].off);
            _CNT(pump_time[i].on);
            _CNT(freeze_energy_time[i]);
        } 
        
        /* 防冻持续时间 */
        if (fg_idle_sta_antifreeze
        || fg_run_sta_ac_antifreeze
        || fg_run_sta_hotw_antifreeze
        || fg_zone_A_timed_antifreeze
        || fg_zone_B_timed_antifreeze
        || fg_hotw_timed_antifreeze
        )
        {
            _CNT(freeze_act_time);
        }
        else
        {
            freeze_act_time = 0;
        }

        /* 计时: 所有模块的变频水泵 */
        if (log_sys_have_cell_pump_run(SUB_SYS_TYPE_MAX) == FALSE)  /* 所有模块变频水泵未开 */
        {
            pump_time[PM_F_ANY].on = 0;
        }
        else                /* 任意模块变频水泵开启 */
        {
            pump_time[PM_F_ANY].off = 0;
        }

        /* 计时: 所有可制热水模块的变频水泵 */
        if ((log_sys_have_cell_pump_run(SUB_SYS_AC_HOTW) == FALSE)  /* 所有可制热水模块变频水泵未开 */
        || (log_sys_have_cell_pump_run(SUB_SYS_AC_HOTW)             /* 可制热水模块变频水泵开启 */
           && !MODE_IS_HOTW(SUB_SYS_AC_HOTW)                        /* 空调+热水子系统非制热水运行 */
           && !fg_run_sta_hotw_antifreeze                           /* 非运行态热水水路防冻 */
           )
        )
        {
            pump_time[PM_F_HW_RATFZ].on = 0;
        }
        else                                                        /* 所有可制热水模块中任意变频水泵开启 */
        {
            pump_time[PM_F_HW_RATFZ].off = 0;
        }

        /* 计时: 所有不可制热水模块的变频水泵 */
        if (!fg_zone_A_run                  /* A区未运行 */
        && !fg_zone_B_run                   /* B区未运行 */
        && fg_hotw_run                      /* 热水运行 */
        && !fg_run_sta_ac_antifreeze        /* 未进运行态空调水路防冻 */
        )  /* 所有不可制热水模块变频水泵未开 */
        {
            pump_time[PM_F_NHW_RATFZ].on = 0;
        }
        else                                                        /* 所有不可制热水模块中任意变频水泵开启 */
        {
            pump_time[PM_F_NHW_RATFZ].off = 0;
        }

        /* 待机态防冻时 */
        if (fg_idle_sta_antifreeze)
        {
            /* 进入待机态防冻后，清空以下计时变量，防止待机态防冻中途开机可能会立刻进入运行态防冻 */
            pump_time[PM_F_HW_RATFZ].off = 0;
            pump_time[PM_F_NHW_RATFZ].off = 0;
            
            /* 进入待机态防冻后，清空以下计时变量，防止待机态防冻的水泵未稳定运行（47秒）中途开机可能会立刻进入待机态防冻(额外加的一道保险) */
			pump_time[PM_F_ANY].off = 0;
        }
		
        /* 计时: 所有既可制热水又可制冷(或制热)模块的变频水泵 */
        if (log_sys_have_cell_pump_run(SUB_SYS_AC_HOTW) == FALSE)   /* 所有既可制热水又可制冷(或制热)模块变频水泵未开 */
        {
            pump_time[PM_F_AC_HW].on = 0;
        }
        else                                                        /* 所有既可制热水又可制冷(或制热)模块中任意变频水泵开启 */
        {
            pump_time[PM_F_AC_HW].off = 0;
        }

        /* 计时: 下回水泵 */
        if (!get_output(MAX_PCB, M_DO_PUMPret))               
        {
            pump_time[PM_RET].on = 0;
        }
        else
        {
            pump_time[PM_RET].off = 0;
        }

        /* 计时: 外循环泵 */
        if (!get_output(MAX_PCB, M_DO_PUMPo))
        {
            pump_time[PM_O_TATFZ].on = 0;
        }
        else
        {
            pump_time[PM_O_TATFZ].off = 0;
        }

        /* 计时: 混合水泵 */
        if (!get_output(MAX_PCB, M_DO_PUMPm))
        {
            pump_time[PM_M_TATFZ].on = 0;
        }
        else
        {
            pump_time[PM_M_TATFZ].off = 0;
        }

        /* 计时: 所有可参与热水定时防冻变频水泵 */
        if (!(HOTW_TAF||fg_hotw_run))
        {
            pump_time[PM_HW_TATFZ].on = 0;
        }
        else
        {
            pump_time[PM_HW_TATFZ].off = 0;
        }

        /* 计时: 环境温度限制压机开启消抖 */
        for (i = 0; i < SUB_SYS_TYPE_MAX; i++)
        {
            for (j = 0; j < CTRL_MODE_MAX; j++)
            {
                _CNT(cnt_sys_comp_limit_TA_dly[i][j]);
            }
        }

        /* 计时: 智能电网指令 */
        for (i = 0; i < SG_CMD_MAX; i++)
        {
            _CNT_WHEN(cnt_sg_cmd_on[i], (smart_grid_cmd == i));
        }

        /* 计时: 智能电网状态 */
        for (i = 0; i < SG_STA_MAX; i++)
        {
            _CNT_WHEN(cnt_sg_sta_off[i], (smart_grid_sta != i));
        }
        
        for (i=0; i<val_pcb_num; i++)
        {
            for (j=0; j<m_val_comp_num(i); j++)
            {
                if (M_CM_GetStatus(i, j, COMP_RUN))
                {
                    if (m_cm_time_on[i][j] != 0xff)         m_cm_time_on[i][j]++;
                        
                }
                else 
                {
                    m_cm_time_on[i][j] = 0;
                }
            }
        }

        /* 外部热源计时 */
        _CNT(AHS_time.off);
        _CNT(AHS_time.on);
        if (!get_output(MAX_PCB, M_DO_AHS))             AHS_time.on = 0;
        else                                            AHS_time.off = 0;
        
        /* 制冷运行时间 */
        for (i=0; i<SUB_SYS_TYPE_MAX; i++)
        {
            _CNT_WHEN(cool_runtime[i].on, RUN_COOL(i));
            _CNT_WHEN(cool_runtime[i].off, !RUN_COOL(i));
            if (RUN_COOL(i))
            {
                _CNT_WHEN(cool_runtime[i].run, fg_cool_need);
            }
        }

        /* 制热运行时间 */
        for (i=0; i<SUB_SYS_TYPE_MAX; i++)
        {
            _CNT_WHEN(heat_runtime[i].on, RUN_HEAT(i));
            _CNT_WHEN(heat_runtime[i].off, !RUN_HEAT(i));
            if (RUN_HEAT(i))
            {
                _CNT_WHEN(heat_runtime[i].run, fg_heat_need);
            }
        }
        
        /* 热水运行时间 */
        _CNT_WHEN(hotw_runtime.on, RUN_HOTW(SUB_SYS_AC_HOTW));				
        _CNT_WHEN(hotw_runtime.off, !RUN_HOTW(SUB_SYS_AC_HOTW));				
        if (RUN_HOTW(SUB_SYS_AC_HOTW))
        {
            _CNT_WHEN(hotw_runtime.run, fg_hotw_need);
        }

        _CNTDOWN(cnt_hotw_heat_switch_dly);

        /* 清空累计运行时间 */
        if (fg_zone_A_run == FALSE 
        && fg_zone_B_run == FALSE
        )
        {
            for (i=0; i<SUB_SYS_TYPE_MAX; i++)
            {
                cool_runtime[i].run = 0;
                heat_runtime[i].run = 0;
            }
        }
        if (fg_hotw_run == FALSE)
        {
            hotw_runtime.run = 0;
        }

        /* 定时杀菌计时 */
        _CNT_WHEN(cnt_ster_cyc, fg_timed_disinfect_run);
        _CNT_WHEN(cnt_ster_high_temp, fg_timed_disinfect_run&&(pv_sys_THWt>=val_ster_temp-20));

        /* 试运行计时 */
        _CNT_WHEN(cnt_test_run_air_empty, GETBIT_N(fg_test_run, TEST_RUN_AIR_EMPTY));
        _CNT_WHEN(cnt_test_run_pump, GETBIT_N(fg_test_run, TEST_RUN_PUMP));

        /* 地暖预热压机计时 */
        _CNT_WHEN(floor_preheat_time.on, fg_floor_preheat_run&&log_sys_have_comp_run(MODE_NULL, SUB_SYS_TYPE_MAX));
        _CNT_WHEN(floor_preheat_time.off, fg_floor_preheat_run&&!log_sys_have_comp_run(MODE_NULL, SUB_SYS_TYPE_MAX));
        
        /* 太阳能信号计时 */
        _CNT_WHEN(cnt_HDIsl_on, m_get_input(IN_SOLAR));
        _CNT_WHEN(cnt_HDIsl_off, !m_get_input(IN_SOLAR));

        /* 太阳能温度计时 */
        _CNT_WHEN(cnt_TSL_on, (pv_sys_TSL>val_PUMPsl_on_TSL));
        _CNT_WHEN(cnt_TSL_off, (pv_sys_TSL<=val_PUMPsl_on_TSL-val_PUMPsl_TSL_dT));

        /* 高峰用电限制运行计时 */
        if (PEAK_ELEC_LIMIT_RUN)
        {
            _CNT_WITH(peak_elec_air_runtime, log_sys_have_comp_run(MODE_NULL, SUB_SYS_TYPE_MAX));
        }
        else
        {
            peak_elec_air_runtime = 0;
        }
        
		/* 涂鸦APP语言切换后计时 */
		_CNT_WITH(cnt_wait_tuya_lang, fg_tuya_lang_switch);

		/* 涂鸦APP温度单位切换后计时 */
		_CNT_WITH(cnt_wait_tuya_temp_unit, fg_tuya_temp_unit_switch);

		/* 制热试运行辅助电热运行计时 */
		_CNT_WHEN(cnt_IBH_on, fg_test_run_heat_IBH_run);

        /* 制热水试运行水箱电热运行计时 */
		_CNT_WHEN(cnt_test_run_hotw_TBH_on, fg_test_run_hotw_TBH_run);

		/* 线控开关信号计时 */
		_CNT_WHEN(cnt_LDIrm_dly, m_get_input(IN_REMOTE));

        /* 三通阀3开关计时 */
        _CNT_WHEN(cnt_S3V3_ext_circue_s, get_output(MAX_PCB, M_DO_S3V3));
        _CNT_WHEN(cnt_S3V3_in_circue_s, !get_output(MAX_PCB, M_DO_S3V3));

        /* 用电计量保存计时（秒） */
        _CNT(cnt_pwru_cal_sec);
        _CNT(dbg_cnt_pwru_cal_10s);
        _CNT(dbg_cnt_pwru_cal_20s);
        _CNT(dbg_cnt_pwru_cal_60s);
    }

/*----------------------分计时----------------------------------*/  
    
    if (F_TIME_BASIC_MIN_BACK)
    {
        if (sys_warm_time != 0xffff)  sys_warm_time++;
        if (power_down_time_space != 0xffff)  power_down_time_space++;
//        password_super_save();

        
        /* 系统运行时间 */
        if (status_run != _IDLE && status_run != _ALARM)
        {
            if (++sys_time.run >= 60)                       /* 1小时时间到 */
            {
//              if (cell[MAX_PCB].cell_runtime[LIMIT_RUNTIME] < 0xffff && limit_value != 0) 
//              {
//                  cell[MAX_PCB].cell_runtime[LIMIT_RUNTIME]++;
//                  cmd_systime_save();         /* 系统运行时间累加(1个小时保存一次) */
//              }
                if (cell[MAX_PCB].cell_runtime[SYS_RUNTIME] < 0xffff )  
                {
                    cell[MAX_PCB].cell_runtime[SYS_RUNTIME]++; 
                    cmd_systime_save();         /* 系统运行时间累加(1个小时保存一次) */
                }
                sys_time.run = 0;
            }
            if (sys_time.on != 0xffff)  sys_time.on++;
            sys_time.off = 0;
        }
        else 
        {
            sys_time.on = 0;
            if (sys_time.off != 0xffff) sys_time.off++;
        }

        /* 地暖预热累计运行计时(分) */
        _CNT_WHEN(floor_preheat_time.run, fg_floor_preheat_run);

        /* 用电计量保存计时（分） */
        _CNT(cnt_pwru_save_min);
    }
      
     
    if(!log_sys_have_comp_run(MODE_NULL, SUB_SYS_TYPE_MAX))
    {
        HaveCompRuntime=0;
        
    }
    else if (F_TIME_BASIC_MIN_BACK)
    {
        if (HaveCompRuntime != 0xffff)  HaveCompRuntime++;
    }
    
    log_sys_cmd_time();
}
/**************************************************
函数: log_sys_public_info
功能: 处理共享信息
参数: 无
返回值: 无
***************************************************/
void log_sys_public_info(void)
{
    U08 i, k;
    static U16 sv_cell_unable;      /* 模块使用：位模式 */
    
    for (i=0; i<CTRL_INFO_NUM ;i++ )     
    {
        ctrl_info[i] = 0;
    }
                
    SET_BIT(ctrl_info[0], status_run&0x07);  /* 机组运行状态 */

    if (!fg_warm_complete)                                      // 机组预热 
    {
        ctrl_info[0] |= 0x0100;
    }
    
    if (fg_error)                                               /* 机组故障状态 */
    {
        ctrl_info[0] |= 0x0200;
    }
    /* 机组防冻状态 */
    if (FG_AF_ALL)  
    {
        ctrl_info[0] |= 0x0400;
    }
    if(fg_defrost_run[SUB_SYS_AC_HOTW] || fg_defrost_run[SUB_SYS_AC_ONLY])//机组除霜
    {
        ctrl_info[0] |= 0x0800;
    }
//    if(m_get_input(IN_LINK))//二通阀联锁
//    {
//        ctrl_info[0] |= 0x8000;
//    }

    for (i=0; i<val_pcb_num; i++)
    {
        if (CELL_Is_used(i))        SETBIT(sv_cell_unable, i);
        else                        CLRBIT(sv_cell_unable, i);
    }

    /* 以下内容由程序实际情况决定，程序移植时请注意修改(不存在的温度项为“SENSOR_NOEXIST”) */
    /* 基本控制信息 */
    k = 1;      /* 请注意: 请检查ctrl_info[]数组大小CTRL_INFO_NUM，不要超界 */                          
    ctrl_info[k++] = m_val_comp_num(0);                     // 01 压机数                        
    ctrl_info[k++] = sv_mode;                               // 02 设定模式
    ctrl_info[k++] = ctrl_mode[SUB_SYS_AC_HOTW];			/* 03 空调+热水子系统控制模式 */
    ctrl_info[k++] = sv_temp_energy[SUB_SYS_AC_HOTW];		/* 04 空调+热水子系统能调目标温度 */
    ctrl_info[k++] = ctrl_temp_energy[SUB_SYS_AC_HOTW];		/* 05 空调+热水子系统能调控制温度 */
    ctrl_info[k++] = m_motor_sv_sheat;                      // 06 主路设置过热度            
    ctrl_info[k++] = m_motor_pv_sheat;                      // 07 主路当前过热度                            
    ctrl_info[k++] = GET_EXIO_ST_SV(0, EEV_ID_1);           // 08 主路电膨目标开度                                          
    ctrl_info[k++] = GET_EXIO_ST_PV(0, EEV_ID_1);           // 09 主路电膨当前开度                                          
    ctrl_info[k++] = s_motor_sv_sheat;                      // 10 辅路设置过热度
    ctrl_info[k++] = s_motor_pv_sheat;                      // 11 辅路当前过热度
    ctrl_info[k++] = GET_EXIO_ST_SV(0, EEV_ID_2);           // 12 辅路电膨目标开度                                          
    ctrl_info[k++] = GET_EXIO_ST_PV(0, EEV_ID_2);           // 13 辅路电膨当前开度                                          
    ctrl_info[k++] = m_motor_judge_sheat;                   // 14 主路当前制热过热度 
    ctrl_info[k++] = test_buf[0];                           // 15 主阀穿过次数                              
    ctrl_info[k++] = test_buf[1];                           // 16 辅阀穿过次数                          
    ctrl_info[k++] = test_buf[2];                           // 17 主阀加速计时                          
    ctrl_info[k++] = test_buf[3];                           // 18 辅阀加速计时                      
    ctrl_info[k++] = s_motor_pvnow_sheat;                   // 19 排气当前过热度    
    ctrl_info[k++] = fg_forbid_rise[0];                     // 20 禁止升频标志  
    ctrl_info[k++] = fg_force_drop[0];                      // 21 强制降频标志  
    ctrl_info[k++] = skip_preheat_cnt;                      // 22 跳过预热次数 
    ctrl_info[k++] = sv_cell_unable;                        // 23 模块使用
    ctrl_info[k++] = val_pcb_num;			                /* 24 模块数 */
    ctrl_info[k++] = status_run;			                /* 25 运行状态 */	
    ctrl_info[k++] = SHC_get_set_tmp_max_A();               // 26 A区设定温度上限
    ctrl_info[k++] = SHC_get_set_tmp_min_A();               // 27 A区设定温度下限
    ctrl_info[k++] = SHC_get_set_tmp_max_B();               // 28 B区设定温度上限
    ctrl_info[k++] = SHC_get_set_tmp_min_B();               // 29 B区设定温度下限
    ctrl_info[k++] = SHC_get_hot_tmp_set_max();	            /* 30 热水设定温度上限 */
    ctrl_info[k++] = SHC_get_hot_tmp_set_min();	            /* 31 热水设定温度下限 */
    ctrl_info[k++] = para_get_max(P_GaI(sv_cool));          // 32 制冷设定温度上限
    ctrl_info[k++] = para_get_min(P_GaI(sv_cool));          // 33 制冷设定温度下限
    ctrl_info[k++] = para_get_max(P_GaI(sv_heat));          // 34 制热设定温度上限
    ctrl_info[k++] = para_get_min(P_GaI(sv_heat));          // 35 制热设定温度下限
    ctrl_info[k++] = para_get_max(P_GaI(sv_cool_zone_B));   // 36 B区制冷设定温度上限
    ctrl_info[k++] = para_get_min(P_GaI(sv_cool_zone_B));   // 37 B区制冷设定温度下限
    ctrl_info[k++] = para_get_max(P_GaI(sv_heat_zone_B));   // 38 B区制热设定温度上限
    ctrl_info[k++] = para_get_min(P_GaI(sv_heat_zone_B));   // 39 B区制热设定温度下限
    ctrl_info[k++] = log_sys_warm_time_get();               // 40 预热剩余分钟数
    ctrl_info[k++] = GET_INIT_PARA();                       // 41 参数初始化状态
    ctrl_info[k++] = log_sys_get_flag_area_antifreeze();    // 42 获取区域防冻标志
    /* ！！！注意：不能超过CTRL_INFO_NUM */


    /* 辅助属性 */
    k = 0;      /* 请注意: 请检查assist_info[]数组大小，不要超界 */
    /* 系统可设置模式 */
    if (USE_COOL_ONLY)		/* 仅制冷 */
    {
        assist_info[k] = SYS_COOL_CAN_USE;
    }
    else if (USE_HEAT_ONLY)	/* 仅制热 */
    {
        assist_info[k] = SYS_HEAT_CAN_USE;
    }
    else					/* 制冷+制热 */
    {
        assist_info[k] = SYS_COOL_CAN_USE|SYS_HEAT_CAN_USE|SYS_AUTO_CAN_USE;
    }

    if (GETBIT_N(fg_test_run, TEST_RUN_COOL_MODE)			        /* 制冷试运行时 */
    || (USE_UIRT_ZONE_A_MODE_SWITCH&&m_get_input(IN_RT_COOL))		/* 单区模式切换，且温控器H信号闭合 */
    )
    {
        assist_info[k] = SYS_COOL_CAN_USE;
    }
    else if (fg_floor_preheat_run			                        /* 地暖预热运行时 */
    || fg_floor_dryup_run	                                        /* 地暖烘干运行时 */
    || GETBIT_N(fg_test_run, TEST_RUN_HEAT_MODE)			        /* 制热试运行时 */
    || (USE_UIRT_ZONE_A_MODE_SWITCH&&m_get_input(IN_RT_HEAT))		/* 单区模式切换，且温控器C信号闭合 */
    )
    {
        assist_info[k] = SYS_HEAT_CAN_USE;
    }
    
    /* 输出可使用(是否存在该输出点) */
    
    /* 压机输出 */
    k++;
    assist_info[1] = 0;
    SETBIT(assist_info[1], SW_CM);
    SETBIT(assist_info[1], SW_S1);
    SETBIT(assist_info[1], SW_crank1);
    if (val_EXDO7_selfdef == EXDO7_IS_CRANK2)
    {
        SETBIT(assist_info[1], SW_crank2);
    }
    SETBIT(assist_info[1], SW_byway);
    
#if 0
    if (val_DO1_selfdef == DO1_IS_V_BYWAY)  SETBIT(assist_info[1], SW_byway);  /* N0004 */
#endif
    if (val_fan_speed_type != FAN_SPEED_UNUSED)
    {
        SETBIT(assist_info[1], SW_FAN_LOW);
//        if (val_DO5_selfdef == DO5_IS_FAN2) SETBIT(assist_info[1], SW_FAN_HIG);   /* N0004 */
    }
#if 0
    if (val_DO3_selfdef == DO3_IS_CRANK)    SETBIT(assist_info[1], SW_crank);/* N0004 */
    if (val_DO9_selfdef == DO9_IS_V_IE)     SETBIT(assist_info[1], SW_EVI);
    if (val_DO9_selfdef == DO9_IS_V_DEFR)   SETBIT(assist_info[1], SW_valve_defr);
#endif

    /* 模块输出 */
    k++;
    assist_info[2] = 0x00;
    /* 变频水泵 */
    if (USE_PUMPf_CTRL != PUMPF_CTRL_OFF)
    {
        SETBIT(assist_info[2], S_DO_PUMPf);
    }
    /* 辅助电热 */
    if (val_pcb_num == 1)
    {
        if (val_use_IBH)
        {
            if (IBH1_IS_USE)
            {
                SETBIT(assist_info[2], S_DO_IBH1);				/* 辅助电热1 */
            }
            
            if (IBH2_IS_USE)
            {
                SETBIT(assist_info[2], S_DO_IBH2);				/* 辅助电热2 */
            }
        }
    }
    else
    {
        /* 级联时不做精准判断，直接全显 */
        SETBIT(assist_info[2], S_DO_IBH1);				/* 辅助电热1 */
        SETBIT(assist_info[2], S_DO_IBH2);				/* 辅助电热2 */
    }
    
    SETBIT(assist_info[2], S_DO_DPH);		/* 底盘电加热 */
    if (val_EXDO7_selfdef == EXDO7_IS_DPH2)
    {
        SETBIT(assist_info[2], S_DO_DPH2);		/* 底盘电加热2 */
    }
    SETBIT(assist_info[2], S_DO_HT);		/* 防冻结电加热带 */

    /* 系统输出1 */
    k++;
//    assist_info[3] = 0xffff;
//    CLRBIT(assist_info[3], M_DO_PUMP);
    assist_info[3] = 0x0000;
    SETBIT(assist_info[3], M_DO_DFR);		/* 除霜指示 */
    SETBIT(assist_info[3], M_DO_ERR);		/* 故障指示 */

    /* 外部热源 */
    if (val_use_AHS)
    {
        SETBIT(assist_info[3], M_DO_AHS);	
    }

    /* 热水相关 */
    if (val_hotw_used)
    {
        /* 电磁三通阀1 */
        SETBIT(assist_info[3], M_DO_S3V1);
        /* 水箱电热 */
        if (val_TBH_used)
        {
            SETBIT(assist_info[3], M_DO_TBH);
        }
        /* 下回水泵 */
        if (val_use_PUMPret)
        {
            SETBIT(assist_info[3], M_DO_PUMPret);
        }
        /* 太阳能水泵 */
        if (val_solar_used)
        {
            SETBIT(assist_info[3], M_DO_PUMPsl);
        }
    }
    
    SETBIT(assist_info[3], M_DO_S3V2);		/* 电磁三通阀2 */    
    SETBIT(assist_info[3], M_DO_PUMPo);		/* 外循环泵 */

    /* 双区控制相关 */
    if (USE_DUAL_ZONE_CTRL)
    {  
        SETBIT(assist_info[3], M_DO_S3V3);	/* 电磁三通阀3 */
        SETBIT(assist_info[3], M_DO_PUMPm);	/* 混合水泵 */
    }
    
    /* 系统输出2 */
    k++;
    assist_info[4] = 0;         
    /* 输入可使用(是否存在该输入点) */

    /* 压机输入 */
    k++;
    assist_info[5] = 0;      
    /* 模块输入 */
    k++;
    assist_info[6] = 0;      /* N0004 */
    /* 系统输入1 */
    k++;
    assist_info[7] = 0; 
//    if (REMOTE_IS_INTERLOCK)  SETBIT(assist_info[7], IN_LINK);  
//    if (REMOTE_IS_UNION_ACT)  SETBIT(assist_info[7], IN_REMOTE);
//    if (REMOTE_IS_UNION_KEEP) SETBIT(assist_info[7], IN_REMOTE);
//    if (REMOTE_IS_ONOFF)     SETBIT(assist_info[7], IN_REMOTE);
    //if (REMOTE_IS_UNUSED)
    //{
    //    CLRBIT(assist_info[7], IN_LINK);
    //    CLRBIT(assist_info[7], IN_REMOTE);
    //}
    //assist_info[7] = 0x02;        /* N0004 */     /* 原只显示"线控开关"，不显示"二通联锁" */
    /* 系统输入2 */
    k++;
    assist_info[8] = 0x00;  
    /* 模块故障 */
    k++;        
    for (i=0; i<val_pcb_num; i++)
    {
        if (M_CM_GetStatus(i, MAX_COMP, CELL_ERR) || err_sys_get_s_commu_err(i))
        {
            assist_info[k] |= 0x0001 << i;
        }
		pcb_comp_num[i] = m_val_comp_num(i);		
    }

    //变频故障参数信息，读取的FP参数最近一次故障时的输入电流电压共用一个地址，读取的FP参数在此处处理该地址共用后再送显示界面
#define     INVT_EER_PARA_V_IN      3           //前三位表示输入电压
#define     INVT_EER_PARA_A_IN      0xC000      //后两位表示输入电流
    k = 0;
    inv_err_para_info[k++] = invt_para_fp[INVT_ID_1][0];     /* 最近一次故障类型 */
    inv_err_para_info[k++] = invt_para_fp[INVT_ID_1][1];     /* 最近一次故障时累计运行时间 */    
    inv_err_para_info[k++] = invt_para_fp[INVT_ID_1][2];     /* 最近一次故障时的运行频率 */
    inv_err_para_info[k++] = invt_para_fp[INVT_ID_1][3];     /* 最近一次故障时的给定频率 */
    inv_err_para_info[k++] = invt_para_fp[INVT_ID_1][4];     /* 最近一次故障时的输出电流 */
    inv_err_para_info[k++] = invt_para_fp[INVT_ID_1][5];     /* 最近一次故障时的输出电压 */
    inv_err_para_info[k++] = invt_para_fp[INVT_ID_1][6];     /* 最近一次故障时的输出功率 */
    inv_err_para_info[k++] = invt_para_fp[INVT_ID_1][7];     /* 最近一次故障时的母线电压 */
    inv_err_para_info[k++] = invt_para_fp[INVT_ID_1][8];     /* 最近一次故障时的逆变桥温度 */
    inv_err_para_info[k++] = invt_para_fp[INVT_ID_1][9];     /* 最近一次故障时的限频信息 */
    inv_err_para_info[k++] = 
        (invt_para_fp[INVT_ID_1][10] & INVT_EER_PARA_A_IN) >> 14;    /* 最近一次故障时的输入电流 */
    inv_err_para_info[k++] = 
        GETLOWBITS(invt_para_fp[INVT_ID_1][10],INVT_EER_PARA_V_IN);  /* 最近一次故障时的输入电压 */
    inv_err_para_info[k++] = invt_para_fp[INVT_ID_1][11];   /* 倒数第二次故障类型 */
    inv_err_para_info[k++] = invt_para_fp[INVT_ID_1][12];   /* 倒数第二次故障时累计运行时间 */
    inv_err_para_info[k++] = invt_para_fp[INVT_ID_1][13];   /* 倒数第三次故障类型 */
    inv_err_para_info[k++] = invt_para_fp[INVT_ID_1][14];   /* 倒数第三次故障时累计运行时间 */
    inv_err_para_info[k++] = invt_para_fp[INVT_ID_1][15];   /* 倒数第四次故障类型 */
    inv_err_para_info[k++] = invt_para_fp[INVT_ID_1][16];   /* 倒数第四次故障时累计运行时间 */
    inv_err_para_info[k++] = invt_para_fp[INVT_ID_1][17];   /* 倒数第五次故障类型 */
    inv_err_para_info[k++] = invt_para_fp[INVT_ID_1][18];   /* 倒数第五次故障时累计运行时间 */
    inv_err_para_info[k++] = invt_para_fp[INVT_ID_1][19];   /* 故障时的运行时间 */ 

    /* BMSHMI */
    {
        ARRCLR(bmsHMI.info_bit);
        
        SET_BMSHMI_N( 0, fg_zone_A_run);		            /* A区运行标志  */    
        SET_BMSHMI_N( 1, fg_zone_B_run);		            /* B区运行标志  */    
        SET_BMSHMI_N( 2, fg_hotw_run);		                /* 热水运行标志 */
        SET_BMSHMI_N( 3, fg_timed_PUMPret_run);				/* 定时下回水泵运行标志 */
        SET_BMSHMI_N( 4, fg_force_hotw_mode_run);			/* 强开热水模式功能运行标志 */
        SET_BMSHMI_N( 5, fg_timed_disinfect_run);			/* 定时杀菌功能运行标志 */
        SET_BMSHMI_N( 6, fg_PUMPret_disinfect_run);			/* 下回水泵杀菌运行标志 */
        SET_BMSHMI_N( 7, fg_holiday_away_run);				/* 假日离开模式运行标志 */
        SET_BMSHMI_N( 8, fg_holiday_home_run);				/* 假日在家模式运行标志 */
        SET_BMSHMI_N( 9, fg_force_TBH_run);	                /* 强开水箱电热运行标志 */
        SET_BMSHMI_N(10, fg_force_IBH_run);	                /* 强开辅助电热运行标志 */
        SET_BMSHMI_N(11, fg_force_AHS_run);	                /* 强开外部热源运行标志 */
        SET_BMSHMI_N(12, fg_floor_preheat_run);				/* 地暖预热功能运行标志 */
        SET_BMSHMI_N(13, fg_floor_dryup_run);               /* 地暖烘干功能运行标志 */ 
        SET_BMSHMI_N(14, fg_ability_test_run);				/* 能力测试运行标志 */
        SET_BMSHMI_N(15, fg_refri_recy_act);				/* 收氟运行标志 */    
        SET_BMSHMI_N(16, fg_silent_mode);				    /* 静音模式标志 */
        SET_BMSHMI_N(17, silent_mode_level);				/* 静音模式级别：0：级别一；1：级别二 */
    }
}

/*******************************************************************************
 函 数: log_sys_err()
 功 能: 
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_err(void)
{
    U08 i, j;
    static U16 bak_cell_error[(MAX_PCB+1)*MAX_ERR]={0};
//  LED_SET(LED_GREEN, (status_run==_RUN)?LED_ON:LED_OFF);
    
//    if ((status_run != _IDLE) && (status_run != _ALARM)) set_output(MAX_PCB, SW_RUN);
        
    fg_error = FALSE;

    for (i=0; i<val_pcb_num; i++)   /* 模块有故障 */
    {
        for (j=0; j<MAX_ERR; j++)  
        {
            if (cell[i].cell_error[j])
            {
                fg_error = TRUE;
                if(cell[i].cell_error[j] > bak_cell_error[i*MAX_ERR+j])  
                {
                    dm603_sound[hmi_t_num] = TRUE;
                }
            //  break;
            }
            bak_cell_error[i*MAX_ERR+j] = cell[i].cell_error[j]; 
        }
    }
    for (j=0; j<MAX_ERR; j++)  /* 系统有故障 */
    {
        if (cell[MAX_PCB].cell_error[j])
        {
            fg_error = TRUE;
            if(cell[MAX_PCB].cell_error[j] > bak_cell_error[MAX_PCB*MAX_ERR+j]) 
            {
                dm603_sound[hmi_t_num] = TRUE;
            }
        //  break;
        }
        bak_cell_error[MAX_PCB*MAX_ERR+j] = cell[MAX_PCB].cell_error[j]; 
    }

    if (fg_error)
    {
        set_output(MAX_PCB, M_DO_ERR);
    }
}

/*******************************************************************************
 函 数: log_sys_defrost()
 功 能: 
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_defrost(void)
{
    U08 i;

    for (i = 0; i < SUB_SYS_TYPE_MAX; i++)
    {
        if (log_sys_defrost_comp_num(i)>0)
        {
            fg_defrost_run[i] = TRUE;
            set_output(MAX_PCB, M_DO_DFR);
        }
        else
        {
            fg_defrost_run[i] = FALSE;
        }
    }    
}

/*******************************************************************************
 函 数: log_sys_00_comp_status()
 功 能: 获取00#压机状态
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
//void log_sys_00_comp_status(void)
//{
//    if (!STEP_IS_RUN
//    || ctrl_mode[SUB_SYS_AC_HOTW] == MODE_NULL
//    )
//    {
//        fg_00_comp_normal = FALSE;
//        fg_00_comp_abnormal = FALSE;
//    }
//    else
//    {
//        fg_00_comp_normal = log_sys_cell_comp_run(0)
//                            && !log_sys_TA_limit_comp_is_off(ctrl_mode[SUB_SYS_AC_HOTW], SUB_SYS_AC_HOTW);
//    
//        fg_00_comp_abnormal = !log_sys_cell_comp_run(0) 
//                              && (!log_sys_TA_limit_comp_is_run(ctrl_mode[SUB_SYS_AC_HOTW], SUB_SYS_AC_HOTW)
//                                 || log_sys_is_00_comp_err());
//    }
//}

/*******************************************************************************
 函 数: log_sys_ac_hotw_comp_status()
 功 能: 获取热水模块压机状态
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_ac_hotw_comp_status(void)
{
    if (!STEP_IS_RUN
    || ctrl_mode[SUB_SYS_AC_HOTW] == MODE_NULL
    )
    {
        fg_ac_hotw_comp_normal = FALSE;
        fg_ac_hotw_comp_abnormal = FALSE;
    }
    else
    {
        U08 i;
        BOL have_ac_hotw_comp_run = FALSE;/* TRUE/FALSE:存在热水模块压机运行/热水模块没有压机运行 */

        /* 遍历热水模块 */
        for (i = PCB_BGN(SUB_SYS_AC_HOTW); i < PCB_END(SUB_SYS_AC_HOTW); ++i)
        {
            /* 只要热水模块有一台压机运行，就是“存在热水模块压机运行” */
            have_ac_hotw_comp_run |= log_sys_cell_comp_run(i);
        }
        
        fg_ac_hotw_comp_normal = have_ac_hotw_comp_run
                            && !log_sys_TA_limit_comp_is_off(ctrl_mode[SUB_SYS_AC_HOTW], SUB_SYS_AC_HOTW);
    
        fg_ac_hotw_comp_abnormal = !have_ac_hotw_comp_run
                              && (!log_sys_TA_limit_comp_is_run(ctrl_mode[SUB_SYS_AC_HOTW], SUB_SYS_AC_HOTW)
                                 || log_sys_is_all_ac_hotw_comp_err());
    }
}

/*******************************************************************************
 函 数: log_sys_status_indicate()
 功 能: 系统状态指示
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_status_indicate(void)
{
    log_sys_err();			       /* 系统故障 */
    log_sys_defrost();		       /* 除霜指示 */
//    log_sys_00_comp_status();	   /* 00#压机状态 */
    log_sys_ac_hotw_comp_status(); /* 热水模块压机状态 */
}


/*******************************************************************************
 函 数: log_sys_smart_grid()
 功 能: 智能电网逻辑，根据SG信号和EVU信号，调整功率模式
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_smart_grid(void)
{
    U08 fg_use_sta_1, fg_use_sta_2, fg_use_sta_3, fg_use_sta_4;
    
    /* 初始化 */
    smart_grid_cmd = SG_CMD_NULL;
    sg_power_mode = POWER_MODE_MAX;
    fg_sg_hotw_priority = FALSE;
    fg_sg_hotw_forbid = FALSE;
    smart_grid_act = SG_ACT_NULL;
    
    if (!val_smart_grid_used/* 智能电网禁用 */
    || !fg_warm_complete    /* 预热未完成 */
    )
    {
        smart_grid_sta = SG_STA_NULL;
        return;
    }

    /* 智能电网指令 */
    if (m_get_input(IN_SG) && !m_get_input(IN_EVU))
    {
        smart_grid_cmd = SG_CMD_1;          /* 智能电网指令1 */
    }
    else if (!m_get_input(IN_SG) && !m_get_input(IN_EVU))
    {
        smart_grid_cmd = SG_CMD_2;          /* 智能电网指令2 */
    }
    else if (!m_get_input(IN_SG) && m_get_input(IN_EVU))
    {
        smart_grid_cmd = SG_CMD_3;          /* 智能电网指令3 */
    }
    else
    {
        smart_grid_cmd = SG_CMD_4;          /* 智能电网指令4 */
    }  

    /* 智能电网状态 */
    if (smart_grid_sta != SG_STA_1
    && cnt_sg_sta_off[SG_STA_1] > 10*60
    && smart_grid_cmd == SG_CMD_1
    && cnt_sg_cmd_on[SG_CMD_1] > 10*60
    )
    {
        smart_grid_sta = SG_STA_1;          /* 进入智能电网状态1 */
    }
    else if (smart_grid_sta != SG_STA_2
    && cnt_sg_sta_off[SG_STA_2] > 60
    && smart_grid_cmd == SG_CMD_2
    && cnt_sg_cmd_on[SG_CMD_2] > 60
    )
    {
        smart_grid_sta = SG_STA_2;          /* 进入智能电网状态2 */
    }
    else if (smart_grid_sta != SG_STA_3
    && cnt_sg_sta_off[SG_STA_3] > 10*60
    && smart_grid_cmd == SG_CMD_3
    && cnt_sg_cmd_on[SG_CMD_3] > 10*60
    )
    {
        smart_grid_sta = SG_STA_3;          /* 进入智能电网状态3 */
    }
    else if (smart_grid_sta != SG_STA_4
    && cnt_sg_sta_off[SG_STA_4] > 10*60
    && smart_grid_cmd == SG_CMD_4
    && cnt_sg_cmd_on[SG_CMD_4] > 10*60
    )
    {
        smart_grid_sta = SG_STA_4;			/* 进入智能电网状态4 */
    }

    /* 智能电网配置 */
    switch (val_smart_grid_cfg)
    {
        default:
        case SG_CFG_REGULAR:
            fg_use_sta_1 = TRUE;
            fg_use_sta_2 = TRUE;
            fg_use_sta_3 = TRUE;
            fg_use_sta_4 = TRUE;
            break;

        case SG_CFG_NO_STA1:
            fg_use_sta_1 = FALSE;
            fg_use_sta_2 = TRUE;
            fg_use_sta_3 = TRUE;
            fg_use_sta_4 = TRUE;
            break;
    }

    /* 智能电网动作 */
    if (fg_use_sta_1 && VALLEY_ELEC_FORCE_STOP)             /* 低谷用电强制关闭 */
    {
        smart_grid_act = SG_ACT_ALL_FORCE_OFF;
        sg_power_mode = POWER_MODE_STD;  
        fg_sg_hotw_priority = FALSE;
        fg_sg_hotw_forbid = FALSE;
        SETCMD_N(VALLEY_ELEC_AC_OFF);
        SETCMD_N(VALLEY_ELEC_HOTW_OFF);
    }
    else if (fg_use_sta_2 && PEAK_ELEC_LIMIT_RUN)           /* 高峰用电限制运行 */
    {
        smart_grid_act = SG_ACT_HOTW_FORBID_ON;
        sg_power_mode = POWER_MODE_ECO;
        fg_sg_hotw_priority = FALSE;
        fg_sg_hotw_forbid = TRUE;
        SETCMD_N(PEAK_ELEC_HOTW_OFF);
        if (val_eco_run_time == 0
        || peak_elec_air_runtime > val_eco_run_time
        )
        {
            SETCMD_N(PEAK_ELEC_AC_OFF);
            smart_grid_act = SG_ACT_ALL_FORCE_OFF;
        }
    }
    else if (fg_use_sta_3 && FREE_ELEC_RUN)                 /* 免费用电运行 */
    {
        smart_grid_act = SG_ACT_HOTW_PRIOR_RUN;
        sg_power_mode = POWER_MODE_STR;
        fg_sg_hotw_priority = TRUE;
        fg_sg_hotw_forbid = FALSE;
    }
    else if (fg_use_sta_4 && FREE_ELEC_FORCE_RUN)           /* 免费用电强制运行 */
    {
        smart_grid_act = SG_ACT_HOTW_FORCE_ON;
        sg_power_mode = POWER_MODE_STR;
        fg_sg_hotw_priority = TRUE;
        fg_sg_hotw_forbid = FALSE;
        if (val_hotw_used)
        {
            SETCMD_N(FREE_ELEC_FORCE_HOTW_ON);
        }
    }
}

/*******************************************************************************
 函 数: log_sys_get_power_mode()
 功 能: 获取功率模式
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
U08 log_sys_get_power_mode(void)
{
    U08 result = POWER_MODE_STD;

    switch (val_power_mode)
    {
        case POWER_MODE_AUTO:
            switch (sg_power_mode)
            {
                case POWER_MODE_STD:
                case POWER_MODE_STR:
                case POWER_MODE_ECO:
                    result = sg_power_mode;
                    break;
                    
                default:
                    result = POWER_MODE_STD;
                    break;
            }
            break;
            
        default:
            result = val_power_mode;
            break;
    }

    return result;
}

/********************************************************
函数： m_comp_use_mode
功能： 判断压机可使用模式
参数： 无
返回值： 无
*********************************************************/
BOL m_comp_use_mode(U08 pcb_num, U08 comp_num, U08 mode)
{
    BOL fg_mode_can_use = FALSE;
    
    switch(mode)
    {
        case MODE_COOL:
            if (cell[pcb_num].cell_status[comp_num] & C_MODE_COOL)
            {
                fg_mode_can_use = TRUE;
            }
            break;
        case MODE_HOTW:
        case MODE_HEAT:
            if (cell[pcb_num].cell_status[comp_num] & C_MODE_HEAT)
            {
                fg_mode_can_use = TRUE;
            }
            break;
    
    }
    
    return fg_mode_can_use;
}

#define val_defrost_comp_max            (50)        /* 除霜压机百分数 */
/*******************************************************************************
 函 数: log_sys_get_defrost_comp_max()
 功 能: 根据除霜压机百分数计算最大除霜压机个数
 参 数: per--除霜压机百分数; 
		to_mode--当前运行模式; 
		type--子系统类型; 
 返 回: 
 其 它: 空
*******************************************************************************/
U08 log_sys_get_defrost_comp_max(U16 per, U08 to_mode, U08 type)
{
    U08 x = 0;
    U08 y = 0;
    U08 result = 0;
    U08 defrost_total_num = 0;  /* 除霜压机总数 */
    
    U16 tmp = 0;
    
    /*
     * 计算所有模块可用的压机个数   -->   模块数 X 单模块压机数
     * 模块禁用的压机数去掉
     * 模块不能开启的压机数去年【包括压机低压故障或高压故障等等】
     */
    for (x = PCB_BGN(type); x < PCB_END(type); x++)
    {
        if (!log_sys_cell_can_run(x)) continue;        /* 不可用(禁用、通讯故障) */
            
            for (y = 0; y < m_val_comp_num(x); y++)
            {
                if (log_sys_comp_mode(x, y, to_mode, SUB_SYS_AC_HOTW))    /* 压机需求模式可用 */
                {
                    defrost_total_num++;
                }
            }
    }
 
    /*
     * 计算最大除霜压机个数   可用压机总数 X 除霜压机百分数
     * 若算出的除霜压机数不为整数，应向上加，使之为整数
     * 例如：压机个数共为32个，设置除霜压机数为60%，计算到
     *       可以同时进入除霜压机个数为19.2个，此时应允许
     *       20个压机同时进入除霜
     */
    tmp = defrost_total_num * per; /* 防止出现小于10的数 */
    if (tmp >= 10) {
        tmp = tmp / 10; /* 此处按1/10计算，下面再除10即为 1/100 */
        if ((tmp%10) > 0)   result = (U08)(tmp/10 + 1);
        else                result = (U08)(tmp/10);
    } else {
        result = 1;
    }
 
    /* 防止除霜压机数大过可运行压机总数 */
    if (result > defrost_total_num) result = defrost_total_num;
    
    return result;
}

/*******************************************************************************
 函 数: log_sys_set_warm_time_get()
 功 能: 获取预热时间
 参 数: 空
 返 回: 预热时间（分钟）
 其 它: 
*******************************************************************************/
U16 log_sys_set_warm_time_get(U16 pd_time)
{
    U16 warm_time_m;        /* 正常预热时间 */

    if (pd_time < val_power_down_time_min)
    {
        warm_time_m = 0;
    }
    else if (fg_warm_quick)
    {
        warm_time_m = val_warm_time_min;
    }
    else
    {
        warm_time_m = fun_line_value_u16(pd_time, val_power_down_time_max, val_power_down_time_min,\
                                val_warm_time_max, val_warm_time_min);  
    }

    return  warm_time_m;
}

/*******************************************************************************
 函 数: log_sys_powerdown_time()
 功 能: 计算掉电时间差
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
U16 log_sys_powerdown_time(void)
{
    static BOL fg_first = TRUE;
    static U16 pd_time = 0xffff;

    if (fg_first)
    {
        if (fun_rtc_check(power_down_time_last) && fun_rtc_check(clock[hmi_t_num]))
        {
            I32 time_diff_temp = fun_time_diff(&power_down_time_last, &clock[hmi_t_num], TIME_DIFF_MINS);
            
            if (time_diff_temp>0xffff || time_diff_temp<0)  pd_time = 0xffff;
            else                                            pd_time = time_diff_temp;

            fg_first = FALSE;
        }
        else
        {
            pd_time = 0xffff;               /* 掉电时间无法计算默认最大掉电时长，即使能预热功能 */
        }
    }

    return pd_time;
}
/*******************************************************************************
 函 数: log_sys_warm()
 功 能: 预热逻辑
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_warm(void)
{
    BOOL fg_complete = FALSE;   
    static BOOL fg_warm_en = FALSE; /* 预热使能标志 */
    U16 pd_time = log_sys_powerdown_time();
    U16 warm_time_need = log_sys_set_warm_time_get(pd_time);
    static BOL fg_confirm = FALSE;

    /* 待机态防冻时退出预热：需同步修改说明书 */
    #define EXIT_WARM_ANTIFREEZE (fg_idle_sta_antifreeze && idle_sta_antifreeze_status==ANTIFREEZE_OPEN_COMP)    

    sys_warm_time_need  = warm_time_need;

    /* 特殊情况取消预热 */
    if (EXIT_WARM_ANTIFREEZE)
    {
        key_warm_cancel = TRUE;
    }

    if (power_time <= POWER_ON_PREHEAT
    || SensorNoExist(pv_sys_TA)
    || !TF_IS_IDLE
    || fg_refri_recy_act	/* 收氟 */
    )
    {
        fg_warm_complete = TRUE;
        return;
    }

    // 预热使能
    if (pv_sys_TA<val_warm_env				/* 环境温度 */
    &&  warm_time_need>0	                /* 掉电时间 */
    &&  !fg_confirm			                /* 未确认标志：防止环温变化回马枪触发 */
    )
    {
        fg_warm_en = TRUE;
    }

    if (fg_warm_en)
    {
        if (key_warm_cancel 
        || sys_warm_time>=warm_time_need
        )
        {
            fg_complete = TRUE;
            fg_confirm = TRUE;
        }
    }
    else
    {
        fg_complete = TRUE;
        fg_confirm = TRUE;
    }

    fg_warm_complete = fg_complete ? TRUE : FALSE;

    if (power_down_time_space>60 && fg_warm_complete==TRUE)/* 定时保存一次时间 */
    {
        power_down_time_space = 0;
        pwd_time_write_to_eeprom();
    }
}

/*******************************************************************************
 函 数: log_sys_step_run()
 功 能: 机组运行过程
 参 数: pcb_num--;
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_step_run(void)
{
    /* 空调+热水子系统能量调节 */
    log_sys_sub_sys_ac_hotw_energy(); 

    /* 单空调子系统能量调节 */
    log_sys_sub_sys_ac_energy();
}

/*******************************************************************************
 函 数: log_sys_warm_time_get()
 功 能: 获取预热剩余时间
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
U16 log_sys_warm_time_get(void)
{
    U16 warm_time = fg_warm_complete ? 0 : sys_warm_time_need-sys_warm_time;

    return warm_time;
}

/*******************************************************************************
 函 数: log_sys_defrost_deal()
 功 能: 处理除霜申请
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_defrost_deal(U08 pcb_num)
{
    U08 i;
    U08 defrost_total_num;  /* 除霜压机总数 */
    U08 sub_sys_type;

    // 除霜处理
    for (i = 0; i < m_val_comp_num(pcb_num); i++)
    {
        /* 确定当前模块所在子系统 */
        sub_sys_type = GET_SUB_SYS(pcb_num);
        
        if (M_CM_GetStatus(pcb_num, i, COMP_DEFROST_APPLY))
        {
            defrost_total_num = log_sys_defrost_comp_num(sub_sys_type); // 除霜模块数
            
            if (defrost_total_num < log_sys_get_defrost_comp_max(val_defrost_comp_max, ctrl_mode[sub_sys_type], sub_sys_type))
            {
                cmd_comp_allow_defrost(pcb_num, i);
            }
        }
    }
}

#ifdef M_DEBUG
/*******************************************************************************
 函 数: log_sys_debug()
 功 能: 仅供内部调试时查看状态使用
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_sys_debug(void)
{
    U08 i;
    
    for (i = 0; i < TBH_ON_SIG_MAX; i++)
    {
        tbhDebug[i] = GETBIT_N(fg_TBH_on, i);
    }

    for (i = 0; i < AHS_ON_SIG_MAX; i++)
    {
        ahsDebug[i] = GETBIT_N(fg_AHS_on, i);
    }

    for (i = 0; i < S3V1_ON_SIG_MAX; i++)
    {
        s3v1Debug[i] = GETBIT_N(fg_S3V1_on, i);
    } 

    for (i = 0; i < S3V2_ON_SIG_MAX; i++)
    {
        s3v2Debug[i] = GETBIT_N(fg_S3V2_on, i);
    } 
    
    for (i = 0; i < S3V3_ON_SIG_MAX; i++)
    {
        s3v3Debug[i] = GETBIT_N(fg_S3V3_on, i);
    }
}
#endif

/*==========================================
    功能: 逻辑过程
/==========================================*/
void log_sys(U08 pcb_num)
{   
    log_sys_time();         /* 需放到begin之前，因begin会清空继电器，而time计时会用到继电器 */
    log_sys_begin();
    
    if (power_time < POWER_ON_INTO_WORK) return;

    if (power_time == POWER_ON_INTO_WORK)
    {
        random_time=clock[hmi_r_num].sec;   
        random_time=(random_time%10);
        random_time=(random_time<=4) ? random_time : (random_time%5);
        random_time=random_time*2;
    }   
        
    log_sys_cmd_get(pcb_num);       //系统命令

    log_sys_public_info();

    log_sys_mode_temp_confirm();			/* 模式温度确认 */
    log_sys_mode_need_confirm();            /* 模式需求确认 */
    log_sys_disp_mode_confirm();            /* 显示模式确认 */
    log_sys_ctrl_mode_confirm();            /* 控制模式及控制对象确定 */
    log_sys_ctrl_mode_temp_confirm();		/* 当前控制模式的控制&目标温度确认  */
    
    log_sys_start_and_stop();    
    log_sys_to_ready_by_together();     // 联动开关跳准备态
//    log_sys_two_way_valve_interlock();
         
    log_sys_cell_forbid_deal(pcb_num);  // 模块禁用处理
    
    log_sys_antifreeze();	                /* 常规防冻 */
    log_sys_timed_antifreeze();				/* 定时防冻 */
    
    log_sys_defrost_deal(pcb_num);      // 除霜处理

    log_sys_warm();                     // 预热判断
    
    if (!STEP_IS_READY)  status_ready = READY_IDLE;
    if (!STEP_IS_DELAY)  status_delay = DELAY_IDLE;
    switch (status_run)
    {
        case _ALARM : 
            log_sys_alarm();        
            break;
            
        case _IDLE  : 
            log_sys_idle(pcb_num);      
            break;
            
        case _READY : 
            log_sys_ready();        
            break;
            
        case _RUN   : 
            log_sys_step_run();
            break;
            
        case _DELAY : 
            log_sys_delay(pcb_num);     
            break;
    }

    log_sys_refri_recycle();                /* 收氟 */
    log_sys_TBH();                          /* 水箱电加热 */
    log_sys_check_force_IBH_off();          /* 检查强开IBH是否关闭 */
    log_sys_AHS();                          /* 外部热源 */
    log_sys_pump();                         /* 水泵控制 */
    log_sys_valve();                        /* 三通阀控制 */
    log_sys_UIRT();                         /* 温控器控制 */
    log_sys_timed_disinfect();              /* 杀菌功能 */
    log_sys_force_hotw();                   /* 强开热水模式 */
    log_sys_floor_preheat();                /* 地暖预热功能 */
    log_sys_floor_dryup();                  /* 地暖烘干功能 */
//    log_sys_holiday();                      /* 休假模式 */
    log_sys_ability_test();                 /* 能力测试 */
    log_sys_smart_grid();                   /* 智能电网 */
    PU_cal_power_usage();                   /* 用电计量 */
    log_sys_test_run();                     /* 试运行 */
    log_sys_status_indicate();              /* 系统状态指示 */
    fcu_deal();                             /* 风机盘管 */
    log_sys_cmd_deal(pcb_num);              /* 压机开启及模式转换 */

    log_sys_end();                          /* 输出转换 */

#ifdef M_DEBUG
    log_sys_debug();
#endif
}

/*==========================================
    功能: 变量初始化
/==========================================*/
void log_sys_init_work(void)
{
    U08 i;

    key_reset = TRUE;

    fg_force_TBH_run = FALSE;
    fg_force_IBH_run = FALSE;
    fg_force_AHS_run = FALSE;

    /* 水泵时间 */
    for (i=0; i<PM_MAX; i++)      /* 水泵个数 */
    {
        pump_time[i].off = 0;
    }
        
    status_run = _IDLE;
    fg_machine_need_open = FALSE;

    fg_have_stop = TRUE;
    ARRSET(ctrl_temp_energy, SENSOR_NOEXIST);
    ac_heater_dly = 0xff;
    room_temp = SENSOR_NOEXIST; 
        
    ARRSET(fg_pump_inter_on, FALSE);
    

    fg_force_hotw_mode_run = FALSE;
    fg_timed_disinfect_run = FALSE;
    fg_floor_preheat_run = FALSE;
    fg_floor_dryup_run = FALSE;
    fg_holiday_away_run = FALSE;
    fg_holiday_home_run = FALSE;

    fg_ability_test_run = FALSE;

    smart_grid_cmd = SG_CMD_NULL;
    ARRSET(cnt_sg_sta_off, 0xffff);
    smart_grid_sta = SG_STA_NULL;
    fg_sg_hotw_priority = FALSE;
    sg_power_mode = POWER_MODE_STD;
    fg_sg_hotw_forbid = FALSE;
    
    cnt_wait_tuya_lang = 6;
	cnt_wait_tuya_temp_unit = 6;

    log_sys_init_idle_sta_antifreeze();		/* 待机态防冻变量初始化 */
    log_sys_init_run_sta_ac_antifreeze();	/* 运行态空调水路防冻变量初始化 */
    log_sys_init_run_sta_hotw_antifreeze();	/* 运行态热水水路防冻变量初始化 */
    run_sta_antifreeze_stage = AF_STAGE_IDLE;
    log_sys_init_timed_antifreeze();		/* 定时防冻变量初始化 */
    
//    runtime_read_from_eeprom();

    Password_ReadAll(); // password_read_from_eeprom();
    
    //auto_run_read_from_eeprom();
    DataAccess_SysRuntime(DATA_READ);           /* 读取系统运行时间 */
    pwd_time_read_from_eeprom();                /* 读取上次掉电时间 */

//  limit_val_bak = limit_value;
}

/*******************************************************************************
 函 数: need_comp_num_in_system()
 功 能: 根据模块类型(主/从)，计算有运行需求的压机数
 参 数: type--模块类型(主/从); 
 返 回: 
 其 它: 空
*******************************************************************************/
U08 need_comp_num_in_system(U08 type)
{
    U08 i, j;
    U08 comp_need = 0;
    
    for (i = PCB_BGN(type); i < PCB_END(type); i++)
    {
        for (j = 0; j < m_val_comp_num(i); j++)
        {
            if (M_CM_GetStatus(i, j, COMP_RUN_NEED)) 
            {
                comp_need++;
            }
        }
    }
    
    return comp_need;
}

/******************************************
函数： menu_work
功能： 调试用 
参数： 无
返回值： 无
******************************************/
//void menu_work(void)
//{
//  U08 x, y;
//  U08 item = 1;
//  
//  U08 pcb_check;
//  
//  pcb_check = pcb_addr;
//  if (pcb_addr == 0)      pcb_check = MAX_PCB;
//  
//  do 
//  {
//      Read_IO_Info(void);
//      sys_task();
//      
//      if (sys_key & TC_DOWN) item++;
//      if (item > 8) item = 1;
//      
//      x = gIO.io_jmp >> 12;
//      y = (gIO.io_jmp >> 8) & 0x0f;
//          
//      switch (item)
//      {
//      case 1 :    /* T */
//          if (y == 0)  LCD_DispNumber(item, 2, DCS_NORMAL);
//          else 
//              display_temp(cell[pcb_check].cell_pv[(x<<4) + y - 1]);
//          
//          break;
//      case 2 :    /* OUT */
//          LCD_SetHex(16);
//          
//          if (y == 0)  LCD_DispNumber(item, 2, DCS_NORMAL);
//              
//          if (y == 1)LCD_DispNumber(cell[MAX_PCB].cell_relay[x]&0xff, 2, DCS_NORMAL);
//          if (y == 2)LCD_DispNumber(cell[MAX_PCB].cell_relay[x] >> 8, 2, DCS_NORMAL);
//          if (y == 3)LCD_DispNumber(cell[1].cell_relay[x]&0xff, 2, DCS_NORMAL);
//          if (y == 4)LCD_DispNumber(cell[1].cell_relay[x] >> 8, 2, DCS_NORMAL);
//              
//          if (y == 5)LCD_DispNumber(pcb_type, 2, DCS_NORMAL); 
//          if (y == 6)LCD_DispNumber(fg_seep_error, 2, DCS_NORMAL);
//          if (y == 7)LCD_DispNumber(debug_2, 2, DCS_NORMAL);
//              
//          if (y == 8) LCD_DispNumber(debug_3, 2, DCS_NORMAL);
//          if (y == 9) LCD_DispNumber(debug_4, 2, DCS_NORMAL);
//          if (y == 10) LCD_DispNumber(debug_6[0], 2, DCS_NORMAL);
//          if (y == 11) LCD_DispNumber(debug_6[1], 2, DCS_NORMAL);
//          if (y == 12) LCD_DispNumber(debug_6[2], 2, DCS_NORMAL);
//          if (y == 13) LCD_DispNumber(debug_5, 2, DCS_NORMAL);
//          if (y == 14) LCD_DispNumber(debug_5>>8, 2, DCS_NORMAL);
//          LCD_SetHex(10);
//          break;
//      case 3 :    /* IN */
//          LCD_SetHex(16);
//          if (y == 0)  LCD_DispNumber(item, 2, DCS_NORMAL);
//              
//          if (y == 1)LCD_DispNumber(cell[MAX_PCB].cell_input[x]&0xff, 2, DCS_NORMAL);
//          if (y == 2)LCD_DispNumber(cell[MAX_PCB].cell_input[x] >> 8, 2, DCS_NORMAL);
//          if (y == 3)LCD_DispNumber(cell[0].cell_input[x]&0xff, 2, DCS_NORMAL);
//          if (y == 4)LCD_DispNumber(cell[0].cell_input[x] >> 8, 2, DCS_NORMAL);
//          if (y == 5)LCD_DispNumber(cell[1].cell_input[x]&0xff, 2, DCS_NORMAL);
//          if (y == 6)LCD_DispNumber(cell[1].cell_input[x] >> 8, 2, DCS_NORMAL);
//          if (y == 7)LCD_DispNumber(gIO.input, 2, DCS_NORMAL);
//          if (y == 8)LCD_DispNumber(self_commu_err, 2, DCS_NORMAL);
//          if (y == 9)LCD_DispNumber(sys_input_second, 2, DCS_NORMAL);
//          if (y == 10)LCD_DispNumber(gIO.input, 2, DCS_NORMAL);
//          
//          LCD_SetHex(10);
//          break;
//      case 4 :    /* STEP */
//          LCD_SetHex(16);
//          
//          if (y == 0)  LCD_DispNumber(item, 2, DCS_NORMAL);
//              
//          if (y == 1)LCD_DispNumber(cell[MAX_PCB].cell_error[x]&0xff, 2, DCS_NORMAL);
//          if (y == 2)LCD_DispNumber(cell[MAX_PCB].cell_error[x] >> 8, 2, DCS_NORMAL);
//          if (y == 3)LCD_DispNumber(cell[0].cell_error[x]&0xff, 2, DCS_NORMAL);
//          if (y == 4)LCD_DispNumber(cell[0].cell_error[x] >> 8, 2, DCS_NORMAL);
//          if (y == 5)LCD_DispNumber(cell[1].cell_error[x]&0xff, 2, DCS_NORMAL);
//          if (y == 6)LCD_DispNumber(cell[1].cell_error[x] >> 8, 2, DCS_NORMAL);
//          if (y == 7)LCD_DispNumber(anti_comp_num, 2, DCS_NORMAL);
//          LCD_SetHex(10);
//          break;
//      case 5:
//          
//          LCD_SetHex(16);
//          if (y == 0)  
//          {
//              LCD_DispNumber(item, 2, DCS_NORMAL);
//          }
//              
//          if (y == 1) LCD_DispNumber(cell[MAX_PCB].cell_status[x]&0xff, 2, DCS_NORMAL);
//          if (y == 2) LCD_DispNumber(cell[MAX_PCB].cell_status[x] >> 8, 2, DCS_NORMAL);
//          if (y == 3) LCD_DispNumber(cell[0].cell_status[x]&0xff, 2, DCS_NORMAL);
//          if (y == 4) LCD_DispNumber(cell[0].cell_status[x] >> 8, 2, DCS_NORMAL);
//          if (y == 5) LCD_DispNumber(cell[1].cell_status[x]&0xff, 2, DCS_NORMAL);
//          if (y == 6) LCD_DispNumber(cell[1].cell_status[x] >> 8, 2, DCS_NORMAL);
//              
//          if (y == 7) LCD_DispNumber(frost_space[0], 2, DCS_NORMAL);
//          if (y == 8) LCD_DispNumber(frost_space[1], 2, DCS_NORMAL);
//          if (y == 9) LCD_DispNumber(debug_2, 2, DCS_NORMAL);
//          if (y == 10) LCD_DispNumber(debug_3, 2, DCS_NORMAL);
//          if (y == 11) LCD_DispNumber(debug_3>>8, 2, DCS_NORMAL);
//          if (y == 12) LCD_DispNumber(debug_4, 2, DCS_NORMAL);
//          if (y == 13) LCD_DispNumber(debug_4>>8, 2, DCS_NORMAL);
//          if (y == 14) ;
//          if (y == 15) LCD_DispNumber(debug_5, 2, DCS_NORMAL);
//          LCD_SetHex(10);
//          break;
//      case 6 :
//          LCD_SetHex(16);
//          if (y == 0)  LCD_DispNumber(item, 2, DCS_NORMAL);
//              
//          if (y == 1) LCD_DispNumber(ctrl_mode, 2, DCS_NORMAL);
//          if (y == 2) LCD_DispNumber(fg_freeze_ac_act, 2, DCS_NORMAL);
//          if (y == 3) ;
//          if (y == 4) ;
//          if (y == 5) LCD_DispNumber(extend_commu_error, 2, DCS_NORMAL);
//          if (y == 6) LCD_DispNumber(slave_commu_error, 2, DCS_NORMAL);
//          if (y == 7) LCD_DispNumber(fg_seep_error, 2, DCS_NORMAL);
//          if (y == 8) LCD_DispNumber(fg_seep_error, 2, DCS_NORMAL);
//          if (y == 9) LCD_DispNumber(warm_complete,2, DCS_NORMAL);
//          if (y == 10) LCD_DispNumber(s_get_error(ERR_GAS_OUT_1), 2, DCS_NORMAL);
//          if (y == 11) LCD_DispNumber(m_run_mode_need[x][1], 2, DCS_NORMAL);
//          if (y == 12) LCD_DispNumber(sys_mode, 2, DCS_NORMAL);
//          if (y == 13) LCD_DispNumber(comp_status_debug_wt, 2, DCS_NORMAL);
//          if (y == 14) LCD_DispNumber(comp_status_debug_ac, 2, DCS_NORMAL);
//          if (y == 15) LCD_DispNumber(status_run, 2, DCS_NORMAL);
//          LCD_SetHex(10);
//          break;
//      case 7 :
//          LCD_SetHex(16);
//
//          if (y == 0) LCD_DispNumber(item, 2, DCS_NORMAL);
//              
//          if (y == 1) LCD_DispNumber(status_ready, 2, DCS_NORMAL);
//          if (y == 2) LCD_DispNumber(debug_1, 2, DCS_NORMAL);
//          if (y == 3) LCD_DispNumber(cmd_debug2&0xff, 2, DCS_NORMAL);
//          if (y == 4) LCD_DispNumber(cmd_debug2>>8, 2, DCS_NORMAL);
//          if (y == 5) LCD_DispNumber(cmd_debug[x]&0xff, 2, DCS_NORMAL);
//          if (y == 6) LCD_DispNumber(cmd_debug[x]>>8, 2, DCS_NORMAL);
//          if (y == 7) LCD_DispNumber(status_delay, 2, DCS_NORMAL);
//          if (y == 8) LCD_DispNumber(status_run, 2, DCS_NORMAL);
//          if (y == 9) LCD_DispNumber(air_pump_dly, 2, DCS_NORMAL);
//          if (y == 10) ;
//          if (y == 11) LCD_DispNumber(fg_freeze_open_ac_pm, 2, DCS_NORMAL);   
//          if (y == 12) LCD_DispNumber(fg_anti_run_open_air_pump, 2, DCS_NORMAL);
//          if (y == 13) ;
//          if (y == 14) ;
//          LCD_SetHex(10);
//          break;
//      case 8 :
//          LCD_SetHex(16);
//
//          if (y == 0) LCD_DispNumber(item, 2, DCS_NORMAL);
//              
//          if (y == 3) LCD_DispNumber(item, 2, DCS_NORMAL);
//          if (y == 4) LCD_DispNumber(mod_err_comp[1], 2, DCS_NORMAL);
//
//          if (y == 7) LCD_DispNumber(s_get_error(ERR_GAS_OUT_1), 2, DCS_NORMAL);
//          if (y == 8) LCD_DispNumber(s_get_error(ERR_GAS_OUT_2), 2, DCS_NORMAL);
//          if (y == 9) 
//          {
//              if (F_TIME_BASIC_SEC_BACK)
//              {
//                  
//              }
//              LCD_DispNumber(debug_6[x], 2, DCS_NORMAL);
//          }
//          if (y == 10) LCD_DispNumber(debug_7[x], 2, DCS_NORMAL);
//          if (y == 11) LCD_DispNumber(cell[0].cell_runtime[x], 2, DCS_NORMAL);
//          if (y == 12) LCD_DispNumber(debug_6[0], 2, DCS_NORMAL);
//          if (y == 13) LCD_DispNumber(cell[1].cell_runtime[x], 2, DCS_NORMAL);
//          if (y == 14) LCD_DispNumber(cell[1].cell_runtime[x]>>8, 2, DCS_NORMAL);
//          LCD_SetHex(10);
//          break;
//      }
//      
//  } while (menu_exit() == 0);
//}

