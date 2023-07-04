#define ERR_MOD_GLOBAL
#include "includes.h"

const U08 err_lp[USE_COMP]    =         {ERR_LP_1,              };
const U08 err_lp_AI[USE_COMP] =         {ERR_LP_AI_1,           };
const U08 err_hp[USE_COMP]    =         {ERR_HP_1,              };
const U08 err_hp_AI[USE_COMP] =         {ERR_HP_AI_1,           };
//const U08 err_curr_high[USE_COMP] =     {ERR_CURR_HIGH_1,       };
//const U08 err_curr_low[USE_COMP] =      {ERR_CURR_LOW_1,        };
const U08 err_wing[USE_COMP]=           {ERR_WING_1,            };
const U08 err_gas_out[USE_COMP]=        {ERR_GAS_OUT_1,         };
const U08 err_exh_high[USE_COMP]=      {ERR_EVAP_HIGH_1,       };   
const U08 err_j5_prs_sensor[USE_COMP]=  {ERR_J5_SENSOR_1,       };
const U08 err_j6_prs_sensor[USE_COMP]=  {ERR_J6_SENSOR_1,       };
const U08 err_gas_in[USE_COMP]=         {ERR_GAS_IN_1,          };
const U08 err_evap_in[USE_COMP]=        {ERR_EVAP_IN_1,         };
const U08 err_gas_in_low[USE_COMP]=     {ERR_GAS_IN_LOW_1,      };
const U08 err_emerg_defrost[USE_COMP]=  {ERR_EMERG_DEFROST_1,   };
const U08 err_gas_in_out_err[USE_COMP]= {ERR_GAS_IN_OUT_ERR_1,  };
const U08 err_evap_low[USE_COMP]=       {ERR_EVAP_LOW_1,        };
const U08 err_evap_high1[USE_COMP]=     {ERR_EVAP_HIGH,         };
//const U08 err_sub_freq[USE_COMP]=       {ERR_IN_SUB_FREQ_1,     };/* N0004 */
const U08 err_TA_limit_comp_on[USE_COMP]= {ERR_TA_LIMIT_COMP_ON,};
const U08 err_in_low[USE_COMP]=         {ERR_IN_LOW,         };
const U08 err_in_high[USE_COMP]=        {ERR_IN_HIGH,         };

const U08 err_pwm_fan1[USE_COMP]=       {ERR_PWM_FAN1_EXCEPT,   };
const U08 err_pwm_fan2[USE_COMP]=       {ERR_PWM_FAN2_EXCEPT,   };
const U08 err_ecfan1_commu[USE_COMP]=   {ERR_EC1_COMMU,         };
const U08 err_ecfan1_err[USE_COMP]=     {ERR_EC1_ERR ,          };
const U08 err_ecfan2_commu[USE_COMP]=   {ERR_EC2_COMMU,         };
const U08 err_ecfan2_err[USE_COMP]=     {ERR_EC2_ERR ,          };
const U08 err_invt_commu[USE_COMP]=     {ERR_INVT_COMMU,        };
const U08 err_invt_err[USE_COMP]=       {ERR_INVT_ERR,          };
const U08 err_invt_model[USE_COMP]=     {ERR_INVT_MODEL,        };
const U08 err_evi_in[USE_COMP]=         {ERR_TEMP_EVI_IN,       };
const U08 err_evi_out[USE_COMP]=        {ERR_TEMP_EVI_OUT,      };
const U08 err_TRFgas_sensor[USE_COMP]=  {ERR_TRFgas_SENSOR,     };
const U08 err_TRFliq_sensor[USE_COMP]=  {ERR_TRFliq_SENSOR,     };

const U08 err_inout_commu[USE_EXIO] = {S_ERR_COMMU_INOUT};
const U08 err_inout_ver[USE_EXIO] = {S_ERR_VER_INOUT};

/* ========================================================================================== */
/* 故障处理 */
#define S_MAX_ERR_TIME_S    20
static U16 mod_err_time[MAX_COMP+1][S_MAX_ERR_TIME_S];              /* 时间消抖 */
#define cnt_err_gas_out_high(x)             (mod_err_time[x][0])
#define cnt_err_gas_in_low(x)               (mod_err_time[x][1])
#define cnt_err_lp_detect_dly(x)            (mod_err_time[x][2])
#define cnt_err_lp_onoff_dly(x)             (mod_err_time[x][3])
#define cnt_err_lp_press_dly(x)             (mod_err_time[x][4])
#define cnt_err_gas_in_out(x)               (mod_err_time[x][5])
#define cnt_err_evap_low(x)                 (mod_err_time[x][6])
#define cnt_err_in_low(x)                   (mod_err_time[x][7])
#define cnt_err_pwm_fan1_except(x)          (mod_err_time[x][8])
#define cnt_err_pwm_fan2_except(x)          (mod_err_time[x][9])
#define cnt_err_invt_model_ok(x)            (mod_err_time[x][10])
#define cnt_err_invt_err(x)                 (mod_err_time[x][11])
#define cnt_err_invt_model_dly(x)           (mod_err_time[x][12])
#define cnt_err_evap_high(x)                (mod_err_time[x][13])
#define cnt_err_invt_except(x)              (mod_err_time[x][14])
#define cnt_err_invt_exc_up(x)              (mod_err_time[x][15])
/* 单元故障 */
#define cnt_err_out_high                    (mod_err_time[MAX_COMP][0])
#define cnt_mod_err_PUMP_on                 (mod_err_time[MAX_COMP][1]) /* 水泵开启延时计时 */
#define cnt_err_ac_flux                     (mod_err_time[MAX_COMP][2])
#define cnt_err_power                       (mod_err_time[MAX_COMP][3])
#define cnt_err_anti_ice                    (mod_err_time[MAX_COMP][4])
#define cnt_err_sub_freq                    (mod_err_time[MAX_COMP][5])/* N0004 */
#define cnt_err_outin_high                  (mod_err_time[MAX_COMP][6])
#define cnt_err_outin_err                   (mod_err_time[MAX_COMP][7])
#define cnt_err_phase_mod                   (mod_err_time[MAX_COMP][8])
#define cnt_mod_err_PUMPf                   (mod_err_time[MAX_COMP][9])
#define cnt_mod_warn_PUMPf                  (mod_err_time[MAX_COMP][10])
#define cnt_mod_mac_set_err                 (mod_err_time[MAX_COMP][11])

#define S_MAX_ERR_TIME_100MS    5                       /* 100毫秒计时 */
static U16 mod_err_time_100ms[MAX_COMP+1][S_MAX_ERR_TIME_100MS];    /* 时间消抖(100ms) */
#define cnt_err_hp_onoff(x)                 (mod_err_time_100ms[x][0])
#define cnt_err_hp_press(x)                 (mod_err_time_100ms[x][1])
/* 单元故障 */
#define cnt_err_ibh_ol                      (mod_err_time_100ms[MAX_COMP][0])

static U16 current_err_time[MAX_COMP];      /* 电流延时检测 */
static U16 current_high_time[MAX_COMP];     /* 压机电流过高检测延时(1S计时) */
static U16 current_alarm_time[MAX_COMP];    /* 压机电流过高检测计时 */


U08 wing_temp_confirm[MAX_COMP];
//#define val_auto_reset_time1       (60*60)    /* 自动复位允许时间 (固定为60分钟)*/
//#define val_lp_detect_dly             (60)    /* 低压检测延时(固定为60s) */
#define ERR_RESET_TIME                  180     //故障持续保持时间
//#define val_cool_lp_alarm_delay           (5)     /* 制冷低压确认延时 */ 
//#define val_heat_lp_alarm_delay           (5)     /* 制热低压确认延时 */ 

//#define val_defrost_lp_limit_dly      (60)    /* 退除霜低压故障检测延时 */

U08 cnt_mod_comp_limit_TA_dly[SUB_SYS_TYPE_MAX][CTRL_MODE_MAX];		/* 环境温度限制压机开启消抖计时 */

/*******************************************************************************
 函 数: err_mod_init()
 功 能: 模块故障变量初始化
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void err_mod_init(void)
{
    U08 x, y;

    /* 故障变量 */
    for (x=0; x<MAX_COMP+1; x++)
    {
        for (y=0; y<S_MAX_ERR_TIME_S; y++)
        {
            mod_err_time[x][y] = 0;
        }
        for (y=0; y<S_MAX_ERR_TIME_100MS; y++)
        {
            mod_err_time_100ms[x][y] = 0;
        }
    }
    
    for (x=0; x<MAX_COMP; x++)
    {
        _SETMAX(cnt_err_lp_detect_dly(x));  /* 待机检测低压时滤过低压检测延时 */
    }
    
    err_limit_auto_reset_init();
}

/* ========================================================================================== */

/*******************************************************************************
 函 数: err_invt_id_to_err_id()
 功 能: 获取控制板的变频器故障号
 参 数: id--变频器; 
        type--故障类型; 
 返 回: 
 其 它: 空
*******************************************************************************/
U16 err_invt_id_to_err_id(INVT_ID_ENUM id, U08 type)
{
    U16 err_id;

    switch (type)
    {
        default:
        case INVT_ERR_ERROR:
            switch (id)
            {
                default:
                case INVT_ID_1:
                    err_id = err_invt_err[0];
                    break;
                case INVT_FAN1:
                    err_id = err_ecfan1_err[0];
                    break;
                case INVT_FAN2:
                    err_id = err_ecfan2_err[0];
                    break;
            }
            break;
        case INVT_ERR_COMMU:
            switch (id)
            {
                default:
                case INVT_ID_1:
                    err_id = err_invt_commu[0];
                    break;
                case INVT_FAN1:
                    err_id = err_ecfan1_commu[0];
                    break;
                case INVT_FAN2:
                    err_id = err_ecfan2_commu[0];
                    break;
            }
            break;
    }

    return err_id;
}
#if 0
/***********************************************
    函数：err_mod_judge_curr
    功能：检测压缩机电流
    参数：当前电流值
    返回值：是否报警
************************************************/
#define CURR_VALID              3       /* 最低有效电流(0.1A) */
#define CURR_ERR_DLY            2       /* 电流传感器故障延时(1S) */
const U16 cur_alarm_time[7] = {600, 480, 240, 80, 50, 10, 0};  /* 电流报警反时限曲线表(0.1S计数) */
static U08 err_mod_judge_curr(I16 current_value, U08 x)
{

    U08 current_ratio = 1;                 /* 电流倍数: I实/I额 */
    U08 cnt_curr_err_dly = 2;           
    
    cnt_curr_err_dly=CURR_ERR_DLY;
    
    if (!s_get_output(x, SW_CM)) 
    {
        current_err_time[x] = 0;    /* 压机运行时才检测 */
        current_high_time[x] = 0;   /* 压机运行时才检测 */   
    }
    
    /* 未检测到电流(可能原因：接错线、缺相、传感器坏) */
    if (current_value > CURR_VALID)                     current_err_time[x] = 0;
    if (current_err_time[x] > cnt_curr_err_dly)         s_set_error(err_curr_low[x]);    
    
    if (current_high_time[x] > CURRENT_DLY)    /* 运行时间超过延时时间才开始检测电流 */ 
    {     
        if (current_value >= MAX_CURR)
        {
            current_ratio = 40;     /* 电流大于测量范围时，强制置于4倍 */
        }
        else                         /* 电流传感器无故障时才检测电流大小 */
        {
            current_ratio = current_value * 10 / val_current_rated;
        }
    }
    
    
    /* 计数器减计数，不同电流情况下取小值 */
    if      (current_ratio < 12) current_alarm_time[x] = 0xffff;        /* 未过载，计数器置到最大 */
    else if (current_ratio < 13) {
        if (current_alarm_time[x] > cur_alarm_time[0]) current_alarm_time[x] = cur_alarm_time[0];
    }
    else if (current_ratio < 15) {
        if (current_alarm_time[x] > cur_alarm_time[1]) current_alarm_time[x] = cur_alarm_time[1];
    }
    else if (current_ratio < 16) {
        if (current_alarm_time[x] > cur_alarm_time[2]) current_alarm_time[x] = cur_alarm_time[2];
    }
    else if (current_ratio < 20) {
        if (current_alarm_time[x] > cur_alarm_time[3]) current_alarm_time[x] = cur_alarm_time[3];
    }
    else if (current_ratio < 30) {
        if (current_alarm_time[x] > cur_alarm_time[4]) current_alarm_time[x] = cur_alarm_time[4];
    }
    else if (current_ratio < 40) {
        if (current_alarm_time[x] > cur_alarm_time[5]) current_alarm_time[x] = cur_alarm_time[5];
    }
    else {
        if (current_alarm_time[x] > cur_alarm_time[6]) current_alarm_time[x] = cur_alarm_time[6];
    }
    
    return (current_alarm_time[x] == 0) ? TRUE : FALSE;

    return 0;
}
#endif
/******************************************
函数：sensor_used_temp_evap_out()
功能：蒸发出水温度使用判断
参数：压机序号
返回值：使用; 不使用
******************************************/
//U08 sensor_used_temp_out(U08 comp_num)
//{
//  return (para_C[TEMP_USE_N][EVAP_OUT_TEMP_USE + comp_num]);
//}
/******************************************
函数：sensor_used_temp_cond_out()
功能：冷凝出水温度使用判断
参数：压机序号
返回值：使用; 不使用
******************************************/
//U08 sensor_used_temp_cond_out(U08 comp_num)
//{
//  return (para_C[TEMP_USE_N][COND_OUT_TEMP_USE + comp_num]);
//}

/*******************************************************
函数：Error_temp_too_low_special()
功能：处理出温过低故障的报警与自动复位: 故障变量操作
参数：当前值; 退出值; 报警值; 故障变量; 故障延时
返回值：无
*******************************************************/
void err_mod_temp_too_low_spec(I16 p_value, I16 exit_value, I16 err_value, U08 *err, U16 *err_time)
{
    if (p_value > exit_value || SensorCannotUse(p_value))               /* 达到退出条件，自动复位 */
    {
        *err = FALSE;
        *err_time = 0;
    }
    else if (p_value > err_value)               /* 未达到报警条件，计时清零 */
    {
        *err_time = 0;
    }
    else if (*err_time >= ERR_DLY)  /* 计时时间到，报警 */
    {
        *err = TRUE;
    }
}
/****************************************************
函数：Error_temp_too_low()
功能：处理出温过低故障的报警与自动复位：故障号操作
参数：当前值; 退出值; 报警值; 故障号; 故障延时
返回值：无
*****************************************************/
void err_mod_temp_too_low(I16 p_value, I16 exit_value, I16 err_value, U16 err_no, U16 *err_time)
{
    if (p_value > exit_value)               /* 达到退出条件，自动复位 */
    {
        s_clr_error(err_no);
    }
    
    if (p_value > err_value)                /* 未达到报警条件，计时清零 */
    {
        *err_time = 0;
    }
    else if (*err_time >= ERR_DLY)  /* 计时时间到，报警 */
    {
        s_set_error(err_no);
    }
}
/******************************************
函数：Error_temp_too_high()
功能：处理出温过低故障的报警与自动复位
参数：当前值; 退出值; 报警值; 故障号;
返回值：报警; 不报警
******************************************/
void err_mod_temp_too_high(I16 p_value, I16 exit_value, I16 err_value, U08 err_no, U16 *err_time)
{
    if (p_value < exit_value)               /* 达到退出条件，自动复位 */
    {
        s_clr_error(err_no);
    }
    
    if (p_value < err_value)                /* 未达到报警条件，计时清零 */
    {
        *err_time = 0;
    }
    else if (*err_time >= ERR_DLY)  /* 计时时间到，报警 */
    {
        s_set_error(err_no);
    }
}

/******************************************
函数：err_mod_invt_except()
功能：压机转速异常判断
参数：压机号
返回值：异常; 正常
******************************************/
#define INVT_EXCEPT_NULL         (0) // 无故障
#define INVT_EXCEPT_ONLY_RESET   (1) // 仅发送变频器复位命令
#define INVT_EXCEPT_STOP_RESET   (2) // 停机复位
U08 invt_except_info[MAX_COMP];

void err_mod_invt_except(U08 cm)
{
    #define INVT_EXCEPT_MIN     (20)        // 压机转速异常最低转速
    #define INVT_EXCEPT_DLY     (5*QK_60)   // 压机转速异常检测延时

    U08 except_info = INVT_EXCEPT_NULL;
    if (!(((INVT_ID_ENUM)cm)==INVT_ID_1 && val_inverter_used)   // 变频器不使用
        || pv_cell_invt_set(cm) == 0                            // 目标转速无效
        || pv_cell_invt_cur(cm) >= INVT_EXCEPT_MIN              // 当前转速正常
        || s_get_error(err_invt_err[cm])                        // 变频故障
    )
    {
        cnt_err_invt_except(cm) = 0;
        except_info = INVT_EXCEPT_NULL;
    }
    else if (cnt_err_invt_except(cm) > INVT_EXCEPT_DLY)
    {
        except_info = INVT_EXCEPT_ONLY_RESET;
        if (cnt_err_invt_except(cm) > 2*INVT_EXCEPT_DLY)
            except_info = INVT_EXCEPT_STOP_RESET;
    }

    if (cnt_err_invt_exc_up(cm) > 10    // 用于监控结果变化
        && except_info != invt_except_info[cm]
    )
    {
        cnt_err_invt_exc_up(cm) = 0;
        invt_except_info[cm] = except_info;
        if (invt_except_info[cm] != INVT_EXCEPT_NULL)
            invt_cmd_err_reset(INVT_ID_1);
    }

    SET_BMS_DEBUG_VAL(BDV_INVT_EXCEPT_INFO, invt_except_info[cm]);
}

/*****************************************
    函数：s_cell_have_mode
    功能：判断模块是否是否有指定的运行模式
    参数：指定模式
    返回值：TRUE: 包含指定模式; FALSE: 无指定模式
******************************************/
BOOL err_mod_judge_cell_mode(U08 mode)
{
    U08 i;
    BOOL cell_have_mode = FALSE;
    
    for (i=0; i<self_comp_num; i++)
    {
        if (Cm[i].run_mode == mode)
        {
            cell_have_mode = TRUE;
            break;
        }
    }
    
    return cell_have_mode;
}
/****************************************************
函数：Error_temp_too_low()
功能：处理出温过低故障的报警与自动复位：故障号操作
参数：当前值; 退出值; 报警值; 故障号; 故障延时
返回值：无
*****************************************************/
void Error_temp_too_low(I16 p_value, I16 exit_value, I16 err_value, U16 err_no, U16 *err_time)
{
    if (p_value > exit_value)               /* 达到退出条件，自动复位 */
    {
        s_clr_error(err_no);
    }
    
    if (p_value > err_value)                /* 未达到报警条件，计时清零 */
    {
        *err_time = 0;
    }
    else if (*err_time >= ERR_DLY)  /* 计时时间到，报警 */
    {
        s_set_error(err_no);
    }
}

/******************************************
函数：Error_temp_too_high()
功能：处理出温过低故障的报警与自动复位
参数：当前值; 退出值; 报警值; 故障号;
返回值：报警; 不报警
******************************************/
void Error_temp_too_high(I16 p_value, I16 exit_value, I16 err_value, U08 err_no, U16 *err_time)
{
    if (p_value < exit_value)               /* 达到退出条件，自动复位 */
    {
        s_clr_error(err_no);
    }
    
    if (p_value < err_value)                /* 未达到报警条件，计时清零 */
    {
        *err_time = 0;
    }
    else if (*err_time >= ERR_DLY)  /* 计时时间到，报警 */
    {
        s_set_error(err_no);
    }
}
/******************************************
函数：Error_lp_detect_allow()
功能：低压检测环境温度限制
参数：
返回值：检测; 不检测
******************************************/
BOOL Error_lp_detect_allow(void)
{
    I16 temp_env = pv_cell_TA(pcb_addr);
    static BOOL lp_detect=FALSE;
    if(SensorCannotUse(temp_env))       return TRUE;     
    
    if (temp_env < val_lpcheck_evn_temp)                
    {
        lp_detect=FALSE;
    }
    else if (temp_env > (val_lpcheck_evn_temp+50))      
    {
        lp_detect=TRUE;
    }
    
    return lp_detect;
}
static BOL err_mod_signal_err_lp(U08 x, BOL is_lp)
{
    return (SIGNAL_Launch(&defrost_signal_into_lowpress[x], is_lp));
}

static BOL err_mod_signal_err_hp(U08 x, BOL is_hp)
{
    return (SIGNAL_Launch(&defrost_signal_exit_highpress[x], is_hp));  
}

/*******************************************************************************
 函 数: err_mod_out_in_diff()
 功 能: 出回温差过大、出回温差异常检测
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void err_mod_out_in_diff(void)
{
    static BOL fg_high = FALSE;                     /* 出回温差过大 */
    static BOL fg_except = FALSE;                   /* 出回温差异常 */

    #define TEMP_DIFF 50    /* 恢复回差 */

    I16 temp_high = SENSOR_NOEXIST;
    I16 temp_low  = SENSOR_NOEXIST;

    if (CmIsCool(0))
    {
        temp_high = pv_cell_TWin1(pcb_addr);
        temp_low  = pv_cell_TWout1(pcb_addr);
    }
    else if (CmIsHeat(0))
    {
        temp_high = pv_cell_TWout1(pcb_addr);
        temp_low  = pv_cell_TWin1(pcb_addr);
    }

    if (SensorCanUse(temp_high)
    &&  SensorCanUse(temp_low)
    &&  log_mod_comp_is_run(MODE_NULL)
    &&  Cm[0].defrost_stage==DS_IDLE
        )
    {
        if (temp_high-temp_low >= val_out_in_diff_high)
        {
            if (cnt_err_outin_high > val_err_ac_flux_delay)
            {
                fg_high = TRUE;
            }
        }
        else
        {
            cnt_err_outin_high = 0;
        }

        if (temp_low-temp_high >= val_out_in_diff_err)
        {
            if (cnt_err_outin_err > 10*60)
            {
                fg_except = TRUE;
            }
        }
        else
        {
            cnt_err_outin_err = 0;
        }
    }
    else
    {
        /* 出回温差过大 */
        if (temp_high-temp_low <= MAX(val_out_in_diff_high-TEMP_DIFF, 10))
        {
            fg_high = FALSE;
        }
        
        /* 出回温差异常 */
        fg_except = FALSE;
        
        cnt_err_outin_high = 0;
        cnt_err_outin_err = 0;
    }

    /* 出回温差过大 */
    if (fg_high)
    {
        s_set_error(S_ERR_OUT_IN_DIFF);
        fg_out_in_diff_high = TRUE;
    }
    else
    {
        fg_out_in_diff_high = FALSE;
    }

    /* 出回温差异常 */
    if (fg_except)
    {
        s_set_error(S_ERR_OUT_IN_ERR);
    }

    /* 禁用出回温差过大 */
    if (val_out_in_diff_high == 0)  s_clr_error(S_ERR_OUT_IN_DIFF);
    /* 禁用出回温差异常 */
    if (val_out_in_diff_err == 0)  s_clr_error(S_ERR_OUT_IN_ERR);
}

#ifdef USE_SW_MAC
enum {MAC_IS_OK, MAC_IS_NONE, MAC_IS_MISMATCH};
#endif

#if defined(M_DEBUG)
/*******************************************************************************
 函 数: err_mod_debug_mac_match()
 功 能: DEBUG机型校验
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
U08 err_mod_debug_mac_match(void)
{
    U08 err_id = MAC_IS_OK;
    
    switch (sw_mac_model)
    {
        
        case MAC_NONE:
            err_id = MAC_IS_NONE;  /* 机型未设置或超出范围 */
            break;
            
        case MAC_MULTI_MODEL_1:
            if (sw_mac_model_EXIO != sw_mac_model)
            {
                err_id = MAC_IS_MISMATCH;  /* 内外机机型不匹配 */
            }
            break;
            
        case MAC_MULTI_MODEL_2:
            if (sw_mac_model_EXIO != sw_mac_model)
            {
                err_id = MAC_IS_MISMATCH;  /* 内外机机型不匹配 */
            }
            break;
            
        default:
            break;
    }

    return err_id;
}
#elif defined(M_TCL)
/*******************************************************************************
 函 数: err_mod_tcl_mac_match()
 功 能: TCL机型校验
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
U08 err_mod_tcl_mac_match(void)
{
    U08 err_id = MAC_IS_OK;
    
    switch (sw_mac_model)
    {
        default:
        case MAC_NONE:
            err_id = MAC_IS_NONE;  /* 机型未设置或超出范围 */
            break;
            
        case MAC_M1_4KW:
        case MAC_M1_6KW:
        case MAC_M1_8KW:
        case MAC_M1_10KW:
        case MAC_M1_12KW:
        case MAC_M1_14KW:
        case MAC_M1_16KW:
        case MAC_M3_10KW:
        case MAC_M3_12KW:
        case MAC_M3_14KW:
        case MAC_M3_16KW:
            /* 整体式机型无须校验内外机拨码 */
            break;
            
        case MAC_F1_4KW:
        case MAC_F1_6KW:
        case MAC_F1_8KW:
        case MAC_F1_10KW:
        case MAC_F1_12KW:
        case MAC_F1_14KW:
        case MAC_F1_16KW:
        case MAC_F3_12KW:
        case MAC_F3_14KW:
        case MAC_F3_16KW:
            if (sw_mac_model_EXIO != sw_mac_model)
            {
                err_id = MAC_IS_MISMATCH;  /* 内外机机型不匹配 */
            }
            break;
            
        case MAC_F3_10KW:
            if (sw_mac_model_EXIO != sw_mac_model
            &&  sw_mac_model_EXIO != MAC_F1_10KW
            )
            {
                err_id = MAC_IS_MISMATCH;  /* 内外机机型不匹配 */
            }
            break;

        case MAC_F3_8KW:
            if (sw_mac_model_EXIO != sw_mac_model
            &&  sw_mac_model_EXIO != MAC_F1_8KW
            )
            {
                err_id = MAC_IS_MISMATCH;  /* 内外机机型不匹配 */
            }
            break;
    }

    return err_id;
}
#elif defined(M_DISEN)
/*******************************************************************************
 函 数: err_mod_disen_mac_match()
 功 能: 迪森机型校验
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
U08 err_mod_disen_mac_match(void)
{
    U08 err_id = MAC_IS_OK;
    
    switch (sw_mac_model)
    {
        
        case MAC_NONE:
            err_id = MAC_IS_NONE;  /* 机型未设置或超出范围 */
            break;
            
        default:
            /* 2022-11-17 迪森反馈一体、分体校验规则统一为内外机都要拨码 */
            if (sw_mac_model_EXIO != sw_mac_model)
            {
                err_id = MAC_IS_MISMATCH;  /* 内外机机型不匹配 */
            }
            break;
    }

    return err_id;
}
#elif defined(M_PAIWO)
/*******************************************************************************
 函 数: err_mod_paiwo_mac_match()
 功 能: 派沃机型校验
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
U08 err_mod_paiwo_mac_match(void)
{
    U08 err_id = MAC_IS_OK;
    
    switch (sw_mac_model)
    {
        
        case MAC_NONE:
            err_id = MAC_IS_NONE;  /* 机型未设置或超出范围 */
            break;
            
        default:
            /* 2022-11-19 派沃反馈一体、分体校验规则统一为内机拨码，外机不拨码 */
//            if (sw_mac_model_EXIO != sw_mac_model)
//            {
//                err_id = MAC_IS_MISMATCH;  /* 内外机机型不匹配 */
//            }
            break;
    }

    return err_id;
}
#elif defined(M_NIUENTAI)
/*******************************************************************************
 函 数: err_mod_NET_mac_match()
 功 能: 纽恩泰机型校验
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
U08 err_mod_NET_mac_match(void)
{
    U08 err_id = MAC_IS_OK;
    
    switch (sw_mac_model)
    {
        
        case MAC_NONE:
            err_id = MAC_IS_NONE;  /* 机型未设置或超出范围 */
            break;
            
        default:
            /* 2022-12-23暂定：一体、分体校验规则统一为内机拨码，外机不拨码 */
//            if (sw_mac_model_EXIO != sw_mac_model)
//            {
//                err_id = MAC_IS_MISMATCH;  /* 内外机机型不匹配 */
//            }
            break;
    }

    return err_id;
}
#elif defined(M_SHENGNENG)
/*******************************************************************************
 函 数: err_mod_SN_mac_match()
 功 能: 生能机型校验
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
U08 err_mod_SN_mac_match(void)
{
    U08 err_id = MAC_IS_OK;
    
    switch (sw_mac_model)
    {
        
        case MAC_NONE:
            err_id = MAC_IS_NONE;  /* 机型未设置或超出范围 */
            break;
            
        default:
            /* 一体、分体校验规则统一为内机拨码，外机不拨码 */
//            if (sw_mac_model_EXIO != sw_mac_model)
//            {
//                err_id = MAC_IS_MISMATCH;  /* 内外机机型不匹配 */
//            }
            break;
    }

    return err_id;
}
#endif

/*******************************************************************************
 函 数: err_mod_mac_set()
 功 能: 机型设置异常
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void err_mod_mac_set(void)
{
#ifdef USE_SW_MAC
    U08 err_id = MAC_IS_OK;
    U08 i = 0;
    BOL fg_err = FALSE;

    if (val_mac_set_check)
    {
        /* 获取外机机型拨码 */
        for (i=0; i<SW_MAC_NUM; i++)
        {
            if (GET_EXIO_SW(0, i+MAX((6-SW_MAC_NUM), 0)))	/* HL081拨码位序：012345 */
            {
                SET_BIT(sw_mac_model_EXIO, SW_MAC_NUM-1-i);
            }
            else
            {
                CLR_BIT(sw_mac_model_EXIO, SW_MAC_NUM-1-i);
            }
        } 

        /* 判断读取到的外机机器型号是否超界 */
        if (sw_mac_model_EXIO > MAC_MAX
        ||  sw_mac_model_EXIO == MAC_MAX
        ||  sw_mac_model_EXIO < MAC_NONE
        )
        {
            sw_mac_model_err = TRUE;
        }

        /* 匹配规则 */
#if defined(M_DEBUG)
        err_id = err_mod_debug_mac_match();
#elif defined(M_TCL)
        err_id = err_mod_tcl_mac_match();
#elif defined(M_DISEN)
        err_id = err_mod_disen_mac_match();
#elif defined(M_PAIWO)
        err_id = err_mod_paiwo_mac_match();
#elif defined(M_NIUENTAI)
        err_id = err_mod_NET_mac_match();
#elif defined(M_SHENGNENG)
        err_id = err_mod_SN_mac_match();
#else
    #warning "未定义机型匹配规则！" 
#endif
    }

    /* 统一归类为“机型设置异常” */
    if (err_id == MAC_IS_NONE
    || (err_id==MAC_IS_MISMATCH && GET_EXIO_HDW_STATUS_READY(0))
    ||  sw_mac_model_err == TRUE
    )
    {
        fg_err = TRUE;
    }
    
    if (!fg_err)
    {
        cnt_mod_mac_set_err = 0;
    }
    else if (cnt_mod_mac_set_err > ERR_DLY)
    {
        s_set_error(S_ERR_MAC_ERR);
    }
    
    /* 禁用（若机器型号参数未超界） */
    if (val_mac_set_check == UNUSED
    &&  sw_mac_model_err == FALSE
    )
    {
        s_clr_error(S_ERR_MAC_ERR);
    }
#endif
}

/*******************************************************************************
 函 数: err_mod_low_env_cool_detect_allow()
 功 能: 低温制冷刚启动时屏蔽某些故障的检测
 参 数: 空
 返 回: TRUE/FALSE--允许检测/不检测
 其 它: 空
*******************************************************************************/
BOOL err_mod_low_env_cool_detect_allow(void)
{
    static BOOL fg_detect = TRUE;
    U08 cm = 0;
    I16 temp_env = pv_cell_TA(pcb_addr);
    I16 cm_run_time = cm_time[cm].on;
    
    
    if (SensorCanUse(temp_env)
    &&  CmIsCool(cm)
    &&  cm_run_time <= val_low_temp_cool_dly
    )
    {
        if (temp_env < val_low_temp_cool_env)                
        {
            fg_detect = FALSE;
        }
        else if (temp_env > val_low_temp_cool_env+20)      
        {
            fg_detect = TRUE;
        }
    }
    else
    {
        fg_detect = TRUE;
    }    
    
    return fg_detect;
}

/*****************************************************
    功能：模块的故障处理
******************************************************/
static void err_mod_judge(void)
{
    U08 x;
    
    if (self_commu_err) return;           /* 通讯故障，不处理 */
    
/* =========== 模块故障 =========== */

    /* 错缺相保护 */
    if (pcb_addr==0 && val_00_board_alarm==SYS_POWER_ERR)
    {
        s_clr_error(S_ERR_PHASE_REVERSE);           //逆相
        s_clr_error(S_ERR_PHASE_LACK);              //缺相
    }
    else
    {
        #define ALARM_PHASE_NULL    (0)     // 无故障(用于记录相序故障)
        #define ALARM_PHASE_ERR     (1)     // 逆相(用于记录相序故障)
        #define ALARM_PHASE_LACK    (2)     // 缺相(用于记录相序故障)
        
        BOL fg_allow = FALSE;
        static U08 alarm_phase_bak = ALARM_PHASE_NULL;  // 记录相序故障

        switch (gIO.phase)
        {
            default:
            case PHASE_STA_OK:
                if (alarm_phase_bak == ALARM_PHASE_NULL) cnt_err_phase_mod = 0;
                break;
            case PHASE_STA_ERR:
                alarm_phase_bak = ALARM_PHASE_ERR;
                break;
            case PHASE_STA_LACK:
                alarm_phase_bak = ALARM_PHASE_LACK;
                break;
        }

        /* 刚上电马上报警，否则延时报警，防止断电时误报 */
        if (power_time < POWER_ON_JUST || cnt_err_phase_mod > POWER_DOWN_DLY)
        {
            fg_allow = TRUE;
        }

        if (fg_allow)
        {
            if (alarm_phase_bak == ALARM_PHASE_ERR)
            {
                s_set_error(S_ERR_PHASE_REVERSE);       //逆相
                alarm_phase_bak = ALARM_PHASE_NULL;
            }
		    if (alarm_phase_bak == ALARM_PHASE_LACK)
		    {
		        s_set_error(S_ERR_PHASE_LACK);         	//缺相
		        alarm_phase_bak = ALARM_PHASE_NULL;
		    }
        }
    }


    /* 内外机故障 */
    for (x = 0; x < USE_EXIO; x++)
    {
        if (GET_EXIO_COMMU_ERR(x))
        {
            s_set_error(err_inout_commu[x]);
        }

        if (GET_EXIO_KLV_VER_ERR(x))
        {
            s_set_error(err_inout_ver[x]);
        }
    }
    
    /* =========== 压机故障 =========== */
    for (x=0; x<self_comp_num; x++)
    {
        BOL is_lp_low = FALSE;
        BOL is_lp_sw = FALSE;
        BOL is_hp_sw =FALSE;
        BOL is_hp_high =FALSE;

        static U08 fg_comp_on_bak[MAX_COMP];

        /* 压机不运行时不检测 */ 
        if (!s_get_output(x, SW_CM))    
        {
            cnt_err_hp_onoff(x) = 0;
            cnt_err_hp_press(x) = 0;
        }   
        
        /* 压机高压 */ 
        // 压机高压开关
        if (!s_get_input(x, IN_HP))  
        {
            cnt_err_hp_onoff(x) = 0;
        }   
        else if (cnt_err_hp_onoff(x) >= ERR_DLY) 
        {
            if (!s_get_error(err_hp[x]))     /* 故障未确定时，才再进入判断 */
            {   
                is_hp_sw = TRUE;
            }
        }
        // 高压传感器
        if(!SensorCanUse(pv_cell_hp(x)))         //传感器不正常
        {
            cnt_err_hp_press(x) = 0;     // 高压压传消抖清零
//            s_clr_error(err_hp_AI[x]);	/* 2022-9-9 因高低压共用传感器会切换，不能这样清除故障 */
        }
        else
        {
            if(Cm[x].run_mode == MODE_COOL)        //制冷高压 
            {
                if(pv_cell_hp(x) < val_hp_value_incl)  
                {
                    cnt_err_hp_press(x) = 0;  
                }
                else if(cnt_err_hp_press(x) > ERR_DLY)
                {
                    is_hp_high = TRUE;
                }
            }
            else    //制热高压
            {
                if(pv_cell_hp(x) < val_hp_value_inht)  
                {
                    cnt_err_hp_press(x) = 0;  
                }
                else if(cnt_err_hp_press(x) > ERR_DLY)
                {
                    is_hp_high = TRUE;
                }
            }
        }

        /* 高压信号处理 */
        if (err_mod_signal_err_hp(x, is_hp_high || is_hp_sw))
        {
            if (is_hp_sw)   s_set_error(err_hp[x]);
            if (is_hp_high) s_set_error(err_hp_AI[x]);
        }

        /* 压机低压 */
        if (!val_check_lp_when_off)
        {
            if (!s_get_output(x, SW_CM))    /* 压机不运行，低压检测延时清零 */
            {
                cnt_err_lp_detect_dly(x) = 0;
            } 
        }
        else
        {
            /* 待机时也检测低压，不影响原运行中的低压检测逻辑 */
            if (s_get_output(x, SW_CM) != fg_comp_on_bak[x])
            {
                fg_comp_on_bak[x] = s_get_output(x, SW_CM);
                if (s_get_output(x, SW_CM))
                {
                    cnt_err_lp_detect_dly(x) = 0;
                }
            }
        }    
        
        if (cnt_err_lp_detect_dly(x) <= TF_VAL(val_lp_detect_dly, TF_LP_CHK_DLY)    /* 低压检测延时不到，消抖延时清零 */
        ||  s_get_cm_status(x, CM_DEFROST)          /* 除霜时屏蔽低压检测 */
        ||  cm_time[x].cnt_time_fan_delay < val_defrost_fan_dly+val_defrost_lp_limit_dly
            )
        {
            cnt_err_lp_onoff_dly(x) = 0;     // 低压开关消抖清零
            cnt_err_lp_press_dly(x) = 0;     // 低压压传消抖清零
        }
        
        /* 低压开关 */
        if (Error_lp_detect_allow()
        && err_mod_low_env_cool_detect_allow()
        )
        {
            // 压机低压开关
            if (!s_get_input(x, IN_LP) )  //|| (pv_cell_lp(x) > val_lp_value_incl)
            {
                cnt_err_lp_onoff_dly(x) = 0;
            }
            else 
            {
                if (Cm[x].run_mode == MODE_COOL)    /* 制冷低压 */
                {
                    if (cnt_err_lp_onoff_dly(x) > TF_VAL(val_cool_lp_alarm_delay, TF_LP_ERR_DLY))
                    {
                        is_lp_sw = TRUE;
                    }
                }
                else                                /* 制热低压 */
                {
                    if (cnt_err_lp_onoff_dly(x) > TF_VAL(val_heat_lp_alarm_delay, TF_LP_ERR_DLY))
                    {
                        if (!s_get_error(err_lp[x]))     /* 故障未确定时，才再进入判断 */
                        {
                            is_lp_sw = TRUE;
                        }
                    }
                }
            }
        }
        else
        {
            s_clr_error(err_lp[x]);
            cnt_err_lp_onoff_dly(x) = 0;
        }
        
        // 低压传感器
        if (!SensorCanUse(pv_cell_lp(x))         //传感器不正常
        ||  !err_mod_low_env_cool_detect_allow()
        )
        {
            cnt_err_lp_press_dly(x) = 0;     // 低压压传消抖清零
//            s_clr_error(err_lp_AI[x]);    /* 2022-9-9 因高低压共用传感器会切换，不能这样清除故障 */
        }
        else
        {
            if(Cm[x].run_mode == MODE_COOL)        //制冷低压 
            {
                if(pv_cell_lp(x) > val_lp_value_incl)  
                {
                    cnt_err_lp_press_dly(x) = 0;  
                }
                else if(cnt_err_lp_press_dly(x) > TF_VAL(val_cool_lp_alarm_delay, TF_LP_ERR_DLY))
                {
                    is_lp_low = TRUE;
                }
            }
            else    //制热低压
            {
                if(pv_cell_lp(x) > val_lp_value_inht)  
                {
                    cnt_err_lp_press_dly(x) = 0;  
                }
                else if(cnt_err_lp_press_dly(x) > TF_VAL(val_heat_lp_alarm_delay, TF_LP_ERR_DLY))
                {
                    if (!s_get_error(err_lp_AI[x]))     /* 故障未确定时，才再进入判断 */
                    {
                        is_lp_low = TRUE;
                    }
                }
            }
        }

        /* 低压信号处理 */
        if (err_mod_signal_err_lp(x, is_lp_low || is_lp_sw))
        {
            if (is_lp_sw)   s_set_error(err_lp[x]);
            if (is_lp_low)  s_set_error(err_lp_AI[x]);
        }
        
//        /* 压机电流过大 */
//      if (NO_CUR_SENSOR)
//      {
//            s_clr_error(err_curr_low[x]);
//          s_clr_error(err_curr_high[x]);
//      }
//      else
//      {
//          if (err_mod_judge_curr(pv_cell_curr(x), x)) 
//          {
//              s_set_error(err_curr_high[x]);  //电流过大  
//          }   
//      }
   
        //温度故障
        if (SensorIsError(pv_cell_wing(x)))         /* 翅片探头故障 */
        {
            s_set_error(err_wing[x]);   
        }
        /* 排气探头故障 */
        if (SensorIsExist(pv_cell_comp_top(x)) && SensorIsError(pv_gas_out_high(x))         
        ||  SensorNoExist(pv_cell_comp_top(x)) && SensorIsError(pv_cell_gas_out(x))
            )
        {
            s_set_error(err_gas_out[x]);    
        }
        /* J5压传故障 */
        if (fg_press_use_j5)
        {
            if (SensorIsError(GET_EXIO_PRS(0, EXIO_AI_01)))      
            {
                s_set_error(err_j5_prs_sensor[x]);    
            }
        }
        else
        {
            s_clr_error(err_j5_prs_sensor[x]);
        }
        /* J6压传故障 */
        if (fg_press_use_j6)
        {
            if (SensorIsError(GET_EXIO_PRS(0, EXIO_AI_02)))     
            {
                s_set_error(err_j6_prs_sensor[x]);    
            }
        }
        else
        {
             s_clr_error(err_j6_prs_sensor[x]); 
        }
        if (SensorIsError(pv_cell_gas_in(x)))       /* 吸气探头故障 */
        {
            s_set_error(err_gas_in[x]); 
        }
        if (SensorIsError(pv_cell_evap(x))&&SensorCannotUse(pv_cell_TRFliq(x))) 		/* 蒸发探头故障 */
	    {
		    s_set_error(err_evap_in[x]);	
	    }
        if (SensorIsError(pv_cell_TWin1(pcb_addr))) 
        {
            s_set_error(S_ERR_TEMP_TWIN1);
        }
        else
        {
            s_clr_error(S_ERR_TEMP_TWIN1);
        }

        /* 环境温度故障 */
        if (SensorIsError(pv_cell_TA(pcb_addr))
        ) 
        {
            s_set_error(S_ERR_TEMP_TA);
        }
        else
        {
            s_clr_error(S_ERR_TEMP_TA);
        }

        if (log_mod_comp_is_run(MODE_NULL) && SensorCanUse(pv_cell_TWin1(pcb_addr)))    /* 运行且回温可用时才检测 */
        {
            if (s_cell_have_mode(MODE_HEAT)) 
            {   
                s_clr_error(err_in_low[x]);     /* 屏蔽制冷温度保护 */

                if (pv_cell_TWin1(pcb_addr) < val_err_ac_out_high - TEMP_IN_DIFF)             /* 未达到报警条件，计时清零 */
                {
                    cnt_err_in_low(x) = 0;
                }
                else if (cnt_err_in_low(x) >= ERR_DLY)  /* 计时时间到，报警 */
                {
                    s_set_error(err_in_high[x]);
                }
            }
            else if (s_cell_have_mode(MODE_COOL)) 
            {   
                s_clr_error(err_in_high[x]);        /* 屏蔽制热温度保护 */
                
                if (pv_cell_TWin1(pcb_addr) >= val_err_ac_out_low+TEMP_IN_DIFF)             /* 未达到报警条件，计时清零 */
                {
                    cnt_err_in_low(x) = 0;
                }
                else if (cnt_err_in_low(x) >= ERR_DLY)  /* 计时时间到，报警 */
                {
                    s_set_error(err_in_low[x]); 
                }
            }           
        }

        /* 环温过低限制压机开启 */
        if (SensorCanUse(pv_cell_TA(pcb_addr))
        && TA_FORCE_COMP_OFF
        )
        {
            s_set_error(err_TA_limit_comp_on[x]);
        }
        
        //排气温度过高故障
        if(SensorCanUse(pv_gas_out_high(x)))
        {
            if (pv_gas_out_high(x) < val_gas_out_high)              /* 未达到报警条件，计时清零 */
            {
                cnt_err_gas_out_high(x) = 0;
            }
            else if (cnt_err_gas_out_high(x) >= ERR_DLY)    /* 计时时间到，报警 */
            {
                s_set_error(err_exh_high[x]);
            }                           
        }
        else     
        {
            s_clr_error(err_exh_high[x]);
        }

        // 吸气温度过低故障
        if (SensorCanUse(pv_cell_gas_in(x)) && CmIsCool(x))
        {
            if ((cm_time[x].on<180 && pv_cell_gas_in(x)>val_gas_in_low_cool-50)
            ||  (cm_time[x].on>=180 && pv_cell_gas_in(x)>val_gas_in_low_cool)
            ||  !err_mod_low_env_cool_detect_allow()
                )/* 未达到报警条件，计时清零 */
            {
                cnt_err_gas_in_low(x) = 0;
            }
            else if (cnt_err_gas_in_low(x) >= 30    /* 计时时间到，报警 */
                 &&  CmIsOn(x)
                 )   
            {
                s_set_error(err_gas_in_low[x]);
            }                           
        }
        else     
        {
            s_clr_error(err_gas_in_low[x]);
        }

        // 制冷蒸发过低
        if (SensorCanUse(pv_evap_temp(x)) && CmIsCool(x))
        {
            if ((cm_time[x].on<180 && pv_evap_temp(x)>val_evap_low_cool-30)
            ||  (cm_time[x].on>=180 && pv_evap_temp(x)>val_evap_low_cool)
            ||  !err_mod_low_env_cool_detect_allow()
                )/* 未达到报警条件，计时清零 */
            {
                cnt_err_evap_low(x) = 0;
            }
            else if (cnt_err_evap_low(x) >= 30      /* 计时时间到，报警 */
                 &&  CmIsOn(x)
                 )
            {
                s_set_error(err_evap_low[x]);
            }                           
        }
        else     
        {
            s_clr_error(err_evap_low[x]);
        }

        // 蒸发温度过高
        if (SensorCanUse(pv_evap_temp(x)) && val_evap_high_err!=0)
        {
            if (pv_evap_temp(x) < val_evap_high_err)/* 未达到报警条件，计时清零 */
            {
                cnt_err_evap_high(x) = 0;
            }
            else if (cnt_err_evap_high(x) >= 300      /* 计时时间到，报警 */
                 &&  CmIsOn(x)
                 )
            {
                s_set_error(err_evap_high1[x]);
            }                           
        }
        else     
        {
            s_clr_error(err_evap_high1[x]);
        }
        
        /* 紧急除霜频繁 */
        if (val_err_emerg_defrost_use)
        {
            if (defrost_fg[x].emergency_defrost_frequent)
            {
                s_set_error(err_emerg_defrost[x]);
            }
        }
        else
        {
            defrost_fg[x].emergency_defrost_frequent = FALSE;
            s_clr_error(err_emerg_defrost[x]);
        }

        // 吸排温差异常
        if (SensorCanUse(pv_cell_gas_in(x)) 
        &&  SensorCanUse(pv_cell_gas_out(x)) 
        &&  val_gas_in_out_err_dly != 0)
        {
            if (cm_time[x].on < val_gas_in_out_err_dly
            ||  pv_cell_gas_in(x) < pv_cell_gas_out(x)+50
            ||  Cm[x].defrost_stage!=DS_IDLE
            ||  ((Cm[x].defrost_dly < val_gas_in_out_err_dly+300)
                  && (power_time > val_gas_in_out_err_dly+300))		/* 防止退除霜后误报 */
                )/* 未达到报警条件，计时清零 */
            {
                cnt_err_gas_in_out(x) = 0;
            }
            else if (cnt_err_gas_in_out(x) >= ERR_DLY)  /* 计时时间到，报警 */
            {
                s_set_error(err_gas_in_out_err[x]);
            }                           
        }
        else     
        {
            s_clr_error(err_gas_in_out_err[x]);
        }

#if 0
        // 降频开关 /* N0004 */
        if (!s_get_input(MAX_COMP, IN_SUB_FREQ))
        {
            cnt_err_sub_freq = 0;
            s_clr_error(err_sub_freq[x]);
        }
        else if (cnt_err_sub_freq >= ERR_DLY) 
        {
            s_set_error(err_sub_freq[x]);
        }
#endif

        /* PWM风机转速异常 */   /* N0004 */
        if (val_freq_fan_set==FREQ_FAN_SET_PWM
        || (val_freq_fan_set==FREQ_FAN_SET_PWM_EX && val_inverter_used)
        )
        {
            if (speedsv[FREQ_FAN1]==0 || pv_cell_fan_now(FREQ_FAN1)>=val_pwm_speed_too_low)
            {
                cnt_err_pwm_fan1_except(x) = 0;
            }
            else if (cnt_err_pwm_fan1_except(x) > val_pwm_speed_check_dly)
            {
                s_set_error(err_pwm_fan1[x]);
            }
            if (speedsv[FREQ_FAN2]==0 || pv_cell_fan_now(FREQ_FAN2)>=val_pwm_speed_too_low)
            {
                cnt_err_pwm_fan2_except(x) = 0;
            }
            else if (cnt_err_pwm_fan2_except(x) > val_pwm_speed_check_dly)
            {
                s_set_error(err_pwm_fan2[x]);
            }
        }   
        else
        {
            s_clr_error(err_pwm_fan1[x]);
            s_clr_error(err_pwm_fan2[x]);
        }
        if (val_freq_fan_num < 2) 
        {
            s_clr_error(err_pwm_fan2[x]);
            s_clr_error(ERR_EC2_COMMU);  
            s_clr_error(ERR_EC2_ERR); 
        }

        // 变频器故障 
        {
            INVT_ID_ENUM id;
            for (id=INVT_ID_1; id<log_mod_get_invt_id_max(); id++)
            {
                if (!USE_INVT)
                {
                    CLR_BIT(fg_invt_commu_error, id);
                    s_clr_error(err_invt_id_to_err_id(id, INVT_ERR_COMMU));
                    s_clr_error(err_invt_id_to_err_id(id, INVT_ERR_ERROR));
                    continue;
                }

                if(GET_BIT(fg_invt_commu_error, id))  
                {
                    s_set_error(err_invt_id_to_err_id(id, INVT_ERR_COMMU));
                    s_clr_error(err_invt_id_to_err_id(id, INVT_ERR_ERROR));
                }
                else
                {
                    U16 err_invt = invt_info_error_id(id);
                    U16 err = err_invt_id_to_err_id(id, INVT_ERR_ERROR);
                    if (err_invt==0 || power_time<INIT_INVT_ERR_TIME)
                    {
                        s_clr_error(err);
                        CELL_INVT_ERR(pcb_addr, id) = 0;
                        if (id==INVT_ID_1)       cnt_err_invt_err(x) = 0;
                    }
                    else if ((id==INVT_ID_1 && cnt_err_invt_err(x)>ERR_DLY)
                         ||  id!=INVT_ID_1
                        )
                    {
                        s_set_error(err);
                        CELL_INVT_ERR(pcb_addr, id) = (U08)err_invt;
                    }
                }
            }

            /* 变频型号设置中:仅做压机 */
            {
                I16 model = invt_para_read(INVT_ID_1, INVT_PARA_MOTOR_TYPE);

                if (GET_BIT(fg_invt_commu_error, INVT_ID_1)
                ||  !val_inverter_used
                ||  model==INVT_PARA_VALUE_NULL
                )
                {
                    cnt_err_invt_model_dly(x) = 0;
                }
                else if (cnt_err_invt_model_dly(x) > 40)
                {
                    /* 因设置与读取时会操作同一个变频变量，该条件不能作为上面清空延时的条件 */
                    if (model != val_invt_motor_type)
                    {
                        s_set_error(err_invt_model[x]);
                        cnt_err_invt_model_ok(x) = 0;
                    }
                }
                
                /* “120秒”是个经验值，与变频通讯有关 */
                if (cnt_err_invt_model_ok(x) > 120)
                {
                    s_clr_error(err_invt_model[x]);
                }

                // 变频压机转速异常
                err_mod_invt_except(x);
            }
        }

        /* 经济器探头相关故障 */
        if (val_s_motor_use==EVI_ECO_OVERHEAT)
        {
            if (SensorIsError(pv_cell_evi_in(x))) 
            {
                s_set_error(err_evi_in[x]);
            }
            if (SensorIsError(pv_cell_evi_out(x))) 
            {
                s_set_error(err_evi_out[x]);
            }
        }
        else
        {
            s_clr_error(err_evi_in[x]);
            s_clr_error(err_evi_out[x]);
        }

        /* 冷媒气侧温度故障 目前仅报警 */
        if (SensorIsError(pv_cell_TRFgas(x)))
        {
            s_set_error(err_TRFgas_sensor[x]);
        }
        else
        {
            s_clr_error(err_TRFgas_sensor[x]);
        }

        /* 冷媒液侧温度故障 目前仅报警 */
        if (SensorIsError(pv_cell_TRFliq(x)))
        {
            s_set_error(err_TRFliq_sensor[x]);
        }
        else
        {
            s_clr_error(err_TRFliq_sensor[x]);
        }

    }

    /* 总出水温度故障 */

    if (val_pcb_num > 1
    && SensorCannotUse(pv_cell_TWout2(pcb_addr))
    && val_TWout2_sensor_used
	)
    {
        s_set_error(S_ERR_TEMP_TWOUT2);
    }
    else
    {
        s_clr_error(S_ERR_TEMP_TWOUT2);
    }

    
    /* 出温相关故障 */
    {
        I16 temp_out = pv_cell_TWout1(pcb_addr);
        if (SensorIsError(pv_cell_TWout1(pcb_addr))) 
        {
            s_set_error(S_ERR_TEMP_TWOUT1);
        }
        else
        {
            s_clr_error(S_ERR_TEMP_TWOUT1);
        }
        
        if((log_mod_comp_is_run(MODE_NULL) == TRUE)&& (SensorCanUse(temp_out)))   /* 运行且出温无故障时才检测 */
        {
            if (s_cell_have_mode(MODE_HEAT)) 
            {   
                s_clr_error(S_ERR_OUT_LOW);     /* 屏蔽制冷温度保护 */
                
                if (temp_out < val_err_ac_out_high)             /* 未达到报警条件，计时清零 */
                {
                    cnt_err_out_high = 0;
                }
                else if (cnt_err_out_high >= ERR_DLY)   /* 计时时间到，报警 */
                {
                    s_set_error(S_ERR_OUT_HIGH);
                }
            }
            else if (s_cell_have_mode(MODE_COOL)) 
            {   
                s_clr_error(S_ERR_OUT_HIGH);        /* 屏蔽制热温度保护 */
                
                 
                if (temp_out > val_err_ac_out_low)              /* 未达到报警条件，计时清零 */
                {
                    cnt_err_out_high = 0;
                }
                else if (cnt_err_out_high >= ERR_DLY)   /* 计时时间到，报警 */
                {
                    s_set_error(S_ERR_OUT_LOW); 
                }
            }           
        }
    }

    /* 辅助电热过载 */
    if (IBH_cfg_type > 0 && val_use_IBH != IBH_UNUSED)
    {
        if (!s_get_input(MAX_COMP, S_IN_IBH_OL))			
        {
            cnt_err_ibh_ol = 0;
        }
	    else if (cnt_err_ibh_ol >= ERR_OL_TIME)		        
	    {
	        s_set_error(S_ERR_IBH_OVERLOAD);
	    }
    }
    else
    {
        s_clr_error(S_ERR_IBH_OVERLOAD);
    }
    
    /* 变频水泵未开，延时清零 */
    if (!s_get_output(MAX_COMP, S_DO_PUMPf)) 
    {
        cnt_mod_err_PUMP_on = 0;
    }

    /* 模块水流开关: 水流不足检测 */
//    if (USE_UNIT_WATER_FLOW)
    {
        if (GETBIT_N(fg_test_run, TEST_RUN_AIR_EMPTY)
        || (fg_pump_test_run&&!fg_test_run_check_LDIw)
        )                          
        {
            cnt_err_ac_flux = 0;
        }
        
        if (cnt_mod_err_PUMP_on <= TF_VAL(AC_PUMP_ON_DLY, TF_PUMP_ON_DLY))
        {
            cnt_err_ac_flux = 0;
        }
        else if (!s_get_input(MAX_COMP, S_IN_WATER_FLOW)||s_get_error(S_ERR_WATER_FLOW))    // 故障清零，复位后重新计    
        {
            cnt_err_ac_flux = 0;
        }   
        else if (cnt_err_ac_flux >= TF_VAL(val_err_ac_flux_delay, TF_ERR_FLOW_DLY)) 
        {
            s_set_error(S_ERR_WATER_FLOW);
        }
    }
//    else
//    {
//        s_clr_error(S_ERR_PUMPF);
//        s_clr_error(S_ERR_WATER_FLOW);
//    }

    /* 变频水泵故障 */
    if (USE_PUMPf_CTRL != PUMPF_CTRL_OFF)
    {
        if (cnt_mod_err_PUMP_on <= TF_VAL(AC_PUMP_ON_DLY, TF_PUMP_ON_DLY))        
        {
            cnt_mod_err_PUMPf = 0;
        }
        else if (!PUMPF_is_err())
        {
            cnt_mod_err_PUMPf = 0;
        }
        else if (cnt_mod_err_PUMPf >= 1)
        {
            s_set_error(S_ERR_PUMPF);
        }
    }
    else
    {
        s_clr_error(S_ERR_PUMPF);
    }

    /* 变频水泵警告 */
    if (USE_PUMPf_CTRL != PUMPF_CTRL_OFF)
    {
        if (cnt_mod_err_PUMP_on <= TF_VAL(AC_PUMP_ON_DLY, TF_PUMP_ON_DLY))        
        {
            cnt_mod_warn_PUMPf = 0;
        }
        else if (!PUMPF_is_warn())
        {
            cnt_mod_warn_PUMPf = 0;
        }
        else if (cnt_mod_warn_PUMPf >= 1)
        {
            s_set_error(S_ERR_PUMPF_WARN);
        }
    }
    else
    {
        s_clr_error(S_ERR_PUMPF_WARN);
    }


#if 0   
     //风机过载保护
    if (!s_get_input(MAX_COMP, IN_FAN_OL)) 
    {
        cnt_err_fan_ol = 0;
    }   
    else if (cnt_err_fan_ol >= ERR_DLY) 
    {
        s_set_error(S_ERR_COOL_FLUX_OL);
    }
    
    // 电源故障
    if (!s_get_input(MAX_COMP, IN_POWER_ERR))
    {
        cnt_err_power = 0;
    }
    else if (cnt_err_power >= ERR_DLY) 
    {
        s_set_error(S_ERR_POWER);
    }


    /* 防冰探头相关故障 */
    if (SensorNoExist(pv_cell_antifreez(pcb_addr)))
    {
        s_clr_error(S_ERR_ANTIFREZZ);
        s_clr_error(S_ERR_TEMP_ANTIFREZZ);
    }
    else
    {
        if (SensorIsError(pv_cell_antifreez(pcb_addr))) 
        {
            s_set_error(S_ERR_TEMP_ANTIFREZZ);
        }
        else
        {
            s_clr_error(S_ERR_TEMP_ANTIFREZZ);
        }
        
        if (SensorCanUse(pv_cell_antifreez(pcb_addr))
        &&  Cm[MAX_COMP].run_mode == MODE_COOL
            )
        {
            if (pv_cell_antifreez(pcb_addr) > val_antifrez_alarmtemp)               /* 未达到报警条件，计时清零 */
            {
                cnt_err_anti_ice = 0;
            }
            else if (cnt_err_anti_ice >= ERR_DLY)   /* 计时时间到，报警 */
            {
                s_set_error(S_ERR_ANTIFREZZ);
            }
        }
        else
        {
            s_clr_error(S_ERR_ANTIFREZZ);
        }
    }
#endif

    /* 禁用EC风机时屏蔽其故障 */
    if (!USE_EX_EC_FAN
    &&  val_freq_fan_set != FREQ_FAN_SET_EC
        )
    {
        s_clr_error(ERR_EC1_COMMU);  
        s_clr_error(ERR_EC1_ERR);
        s_clr_error(ERR_EC2_COMMU);  
        s_clr_error(ERR_EC2_ERR);  
    }
    
    err_mod_out_in_diff();
    err_mod_mac_set();
}

/*******************************************************************************
 函 数: err_mod_class()
 功 能: 模块故障归类
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
static void err_mod_class(void)
{
    U08 x, exio_id;
    U08 bgn, end;
    
    if (self_commu_err) return;     /* 无通讯故障时才做模块故障处理 */

    for (exio_id = 0; exio_id < USE_EXIO; exio_id++)
    {
        if (s_get_error(err_inout_commu[exio_id]) /* 内外机通讯异常 */
        || s_get_error(err_inout_ver[exio_id]))  /* 协议版本过低 */
        {
            bgn = end = 0;
            log_mod_unit_to_comp_group(val_comp_num, &bgn, &end, exio_id);
            for (x=bgn; x<end; x++)
            {
                mod_err_comp[x] = 1;
            }
        }
    }
    
    /* 单元故障 */  
    // 变频水泵故障
//    if (s_get_error(S_ERR_PUMPF))
//    {
//        mod_err_comp[x] = 1;
//      fg_pump_err = TRUE; 
//      fg_ac_sys_err = TRUE; 
//    }
    
    /* 收氟时故障特殊处理 */
    if (fg_refri_recy_act)
    {
        if (s_get_error(S_ERR_EEPROM)           // EERPOM数据错
//        ||  s_get_error(S_ERR_POWER)            /* 电源故障 */ 
        ||  s_get_error(S_ERR_WATER_FLOW)           // 空调水流不足
        ||  s_get_error(S_ERR_OUT_LOW)          // 出水过低
        || s_get_error(S_ERR_PUMPF)				/* 变频水泵故障 */
        || s_get_error(S_ERR_MAC_ERR)		    /* 机型设置异常 */
//      ||  s_get_error(S_ERR_ANTIFREZZ)        // 防冰温度过低
        )
        {
            for (x=0; x<self_comp_num; x++)
            {
                mod_err_comp[x] = 1;
            }
            cmd_refri_recy_stop = TRUE;
        }
    }    /* 正常故障处理 */
    else if (s_get_error(S_ERR_WATER_FLOW)           // 空调水流不足
    ||  s_get_error(S_ERR_PHASE_LACK)          // 缺相 
    ||  s_get_error(S_ERR_PHASE_REVERSE)        // 逆相
    ||	gIO.phase == PHASE_STA_ERR          /* 缺错相保护做延时报警处理，但依然及时保护 */
	||  gIO.phase == PHASE_STA_LACK
//    ||  s_get_error(S_ERR_POWER)            /* 电源故障 */
    ||  s_get_error(S_ERR_OUT_HIGH)         // 出水过高
    ||  s_get_error(S_ERR_OUT_LOW)          // 出水过低
    ||  s_get_error(S_ERR_EEPROM)           // EERPOM数据错 
//    ||  s_get_error(S_ERR_IBH_OVERLOAD)		/* 辅助电热过载 */
//    ||  s_get_error(S_ERR_TEMP_ANTIFREZZ)   /* 防冰温度故障 */
//  ||  s_get_error(S_ERR_ANTIFREZZ)        // 防冰温度过低
    ||  s_get_error(S_ERR_OUT_IN_DIFF)      /* 板换出回温差过大 */
    ||  s_get_error(S_ERR_OUT_IN_ERR)       /* 板换出回温差过大 */
    ||  s_get_error(S_ERR_TEMP_TWOUT1)		/* 板换出水温度故障 */
    ||  s_get_error(S_ERR_TEMP_TWIN1)		/* 板换回水温度故障 */
    ||  s_get_error(S_ERR_TEMP_TWOUT2)		/* 总出水温度故障 */
    ||  s_get_error(S_ERR_TEMP_TA)			/* 环境温度故障 */
    || s_get_error(S_ERR_PUMPF)				/* 变频水泵故障 */
    || s_get_error(S_ERR_MAC_ERR)		    /* 机型设置异常 */
    )
    {
        for (x=0; x<self_comp_num; x++)
        {
            mod_err_comp[x] = 1;
        }
    }
    
    /* 压机严重故障 */
    if (s_get_error(S_ERR_OUT_LOW)          // 出水过低
    || s_get_error(S_ERR_PUMPF)				/* 变频水泵故障 */
    ||  s_get_error(S_ERR_PHASE_LACK)           // 缺相 
    ||  s_get_error(S_ERR_PHASE_REVERSE)        // 逆相
    ||	gIO.phase == PHASE_STA_ERR          /* 缺错相保护做延时报警处理，但依然及时保护 */
	||  gIO.phase == PHASE_STA_LACK
//    ||  s_get_error(S_ERR_POWER)            /* 电源故障 */
//  ||  s_get_error(S_ERR_ANTIFREZZ)        // 防冰温度过低
    )
    {
        for (x=0; x<self_comp_num; x++)
        {
            mod_err_comp_risk[x] = 1;
        }
    }
    
    /* 压缩机故障 */
    for (x=0; x<self_comp_num; x++)
    {
        /* 收氟时故障特殊处理 */
        if (fg_refri_recy_act)
        {
            if (s_get_error(err_lp[x])&&cnt_refri_recy_lp[x]>val_recycle_lp_dly     /* 压机低压 */
            ||  s_get_error(err_lp_AI[x])&&cnt_refri_recy_lp[x]>val_recycle_lp_dly  // 低压过低
            ||  s_get_error(err_hp[x])                   /* 压机高压 */
            ||  s_get_error(err_hp_AI[x])                // 高压过高
            ||  s_get_error(err_exh_high[x])            /* 排气温度过高 */
            ||  s_get_error(err_in_low[x])               /* 回水温度过低 */
            )                   
            {
                mod_err_comp[x] = 1;
                cmd_refri_recy_stop = TRUE;
            }
        }/* 正常故障处理 */
        else if ((s_get_error(err_lp[x])&&!TF_EN)   /* 压机低压 */
        || s_get_error(err_hp[x])                   /* 压机高压 */
//      || s_get_error(err_curr_low[x])             /* 压机电流过小 */
//      || s_get_error(err_curr_high[x])            /* 压机电流过大 */
        || s_get_error(err_wing[x])                 /* 翅片温度故障 */
        || s_get_error(err_gas_out[x])              /* 排气温度故障 */
        || s_get_error(err_gas_in[x])               /* 吸气温度故障 */
        || s_get_error(err_evap_in[x])              /* 阀后温度故障 */
        || s_get_error(err_exh_high[x])             /* 排气温度过高 */
        || s_get_error(err_j5_prs_sensor[x])        /* J5压传故障 */
        || s_get_error(err_j6_prs_sensor[x])        /* J6压传故障 */
        || (s_get_error(err_lp_AI[x])&&!TF_EN)      // 低压过低
        || s_get_error(err_hp_AI[x])                // 高压过高
        || s_get_error(err_gas_in_low[x])           /* 吸气温度过低 */
        || s_get_error(err_evap_low[x])             /* 制冷蒸发过低 */
        || s_get_error(err_evap_high1[x])           /* 蒸发温度过高 */
        || s_get_error(err_emerg_defrost[x])        /* 紧急除霜频繁 */
        || s_get_error(err_gas_in_out_err[x])       /* 吸排温差异常 */
        || s_get_error(err_in_low[x])               /* 回水温度过低 */
        || s_get_error(err_in_high[x])              /* 回水温度过高 */
        || s_get_error(err_pwm_fan1[x])             /* 风机1转速异常 */             
        || s_get_error(err_pwm_fan2[x])             /* 风机2转速异常 */ 
        || s_get_error(err_ecfan1_commu[x])         /* EC1通讯故障 */       /* N0004 */                  
        || s_get_error(err_ecfan1_err[x])           /* EC1故障 */   /* N0004 */         
        || s_get_error(err_ecfan2_commu[x])         /* EC2通讯故障 */       /* N0004 */             
        || s_get_error(err_ecfan2_err[x])           /* EC2故障 */   /* N0004 */ 
        || s_get_error(err_invt_commu[x])           /* 变频通讯故障 */                
        || s_get_error(err_invt_err[x])             /* 变频故障 */            
        || s_get_error(err_invt_model[x])           /* 变频型号设置中 */ 
        || s_get_error(err_evi_in[x])               /* 经济器入口温度故障 */
        || s_get_error(err_evi_out[x])              /* 经济器出口温度故障 */
        || log_mod_comp_temp_out_limit(x)
        || s_get_error(err_TA_limit_comp_on[x])				    /* 环温过低限制压机开启 */
        || (invt_except_info[x] == INVT_EXCEPT_STOP_RESET)	    /* 压机转速异常 */
        )                   
        {
            mod_err_comp[x] = 1;
        }
    }
    /* 压机严重故障 */
    for (x=0; x<self_comp_num; x++)
    {
        if(s_get_error(err_hp[x])                   /* 压机高压 */
        || s_get_error(err_exh_high[x])            /* 排气温度过高 */
        || s_get_error(err_hp_AI[x])                // 高压过高
        || s_get_error(err_gas_in_low[x])           /* 吸气温度过低 */
        || s_get_error(err_evap_low[x])             /* 制冷蒸发过低 */
        || s_get_error(err_gas_in_out_err[x])       /* 吸排温差异常 */
        || s_get_error(err_in_low[x])               /* 回水温度过低 */
        )                   
        {
            mod_err_comp_risk[x] = 1;
        }
    }

    /* 水流不足开泵 */
    if (s_get_error(S_ERR_WATER_FLOW))
    {
        fg_water_lack_err = TRUE;
        if (val_err_flux_open_pump && pv_cell_TA(pcb_addr)<20) fg_water_lack_err = FALSE;
    }
}


/*******************************************************************************
 函 数: err_mod_deal()
 功 能: 模块故障处理
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
static void err_mod_deal(void)
{
    U08 x;
    
    if (self_commu_err) return;     /* 无通讯故障时才做模块故障处理 */

    /* 故障停机处理 */
    for (x=0; x<self_comp_num; x++)
    {
        if (mod_err_comp[x])         /* 空调侧压机故障，停空调侧 */
        {
            if (Cm[x].run_mode == MODE_COOL || Cm[x].run_mode == MODE_HEAT) 
            {
                s_clr_cm_status(x, CM_NEED);
            }
        }

        if (mod_err_comp_risk[x])
        {
            mod_err_comp_risk[x] = 0;
            /* 2020-10-27会议沟通：严重故障时，直接停掉压缩机 */
            _SETMAX(invt_off_speed_down_cnt[x]);
        }
    }
}

/*******************************************************************************
 函 数: err_mod_auto_reset()
 功 能: 自动复位模块故障
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void err_mod_auto_reset(void)
{
    U08 x = 0;
    /* 探头故障 */

    for (x=0; x<self_comp_num; x++)
    {
        //经济器进口复位
        if (!SensorIsError(pv_cell_evi_in(x)))
        {
            s_clr_error(err_evi_in[x]);
        }
        //经济器出口复位
        if (!SensorIsError(pv_cell_evi_out(x)))
        {
            s_clr_error(err_evi_out[x]);
        }

        /* 环温限制压机开启自动复位 */
        if (!TA_FORCE_COMP_OFF)
        {
            s_clr_error(err_TA_limit_comp_on[x]);
        }
    }
 
    
#if 0
    // 防冰温度过低
    if(pv_cell_antifreez(pcb_addr) > val_antifrez_resettemp)
    {
        s_clr_error(S_ERR_ANTIFREZZ);
    }
#endif

    /* 内外机通信故障 */
    for (x = 0; x < USE_EXIO; x++)
    {
        if (!GET_EXIO_COMMU_ERR(x))
        {
            s_clr_error(err_inout_commu[x]);
        }

        if (!GET_EXIO_KLV_VER_ERR(x))
        {
            s_clr_error(err_inout_ver[x]);
        }
    }

    /* 变频通讯故障 */
    {
        INVT_ID_ENUM id;
        static U08 fg_invt_commu_error_bak = 0;
        for (id=INVT_ID_1; id<log_mod_get_invt_id_max(); id++)
        {
            if (!GET_BIT(fg_invt_commu_error, id))
            {
                s_clr_error(err_invt_id_to_err_id(id, INVT_ERR_COMMU));
            } 
        }

        /* 防止刚上电误触发初始化 */
        if (power_time < POWER_ON_INTO_WORK)
        {
            fg_invt_commu_error_bak = fg_invt_commu_error;
        }
        else
        {
            /* 发生变频通讯故障后，需执行一次初始化操作 */
            if (fg_invt_commu_error_bak != fg_invt_commu_error)		/* 变频通讯故障标志发生变化 */
            {
                fg_invt_commu_error_bak = fg_invt_commu_error;
                if (fg_invt_commu_error != 0)				/* 仅限发生故障后(故障尚未消除时) */
                {
                    invt_init();
                }
            }
        }
    }

    /* 变频水泵警告 */
    if (!PUMPF_is_warn())
    {
        s_clr_error(S_ERR_PUMPF_WARN);
    }
}

/*******************************************************************************
 函 数: err_mod_invt_err_reset()
 功 能: 主控端变频故障复位
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void err_mod_invt_err_reset(INVT_ID_ENUM id)
{
    invt_cmd_err_reset(id);
    invt_clr_error_id(id);
    s_clr_error(err_invt_id_to_err_id(id, INVT_ERR_ERROR));
}

/**************************************************************************
* 函数： Error_deal_auto_reset()
* 功能：处理自动复位故障
* 参数：故障值(复位前的值);当前故障值;故障号;故障恢复后清故障号还是故障恢复前清故障号
* 返回值：1 - 自动复位, 0 - 无操作
* 说明：设定时间内出现故障超过3次就需要手动复位。
**************************************************************************/
//BOOL err_limit_auto_reset_deal(U08 x, U08 err_old, U08 err_new, BOOL fg_reset_mode)    
BOOL err_limit_auto_reset_deal(U08 x, U08 err_old, U08 err_new)
{   
    U08  y;

    if(val_auto_reset_time1 == 0)       //设置为0，表示不使用自动复位
    {
        return FALSE;
    }

    /* 锁定一段时间后，自动复位 */
    if (err_new || !err_old)        autoERR[x].err_time = 0;
    else if (autoERR[x].err_time >= autoERR[x].err_time_lock)
    {
        autoERR[x].en = 1;
        autoERR[x].auto_en = 1;     //jxj 2016.01.26
        autoERR[x].have_reset = 1;
        autoERR[x].cnt = 0;
        ARRCLR(autoERR[x].reset_time);
        
        autoERR[x].err_time_lock = 
            MIN(autoERR[x].err_time_lock+val_am_lock_time_add, val_am_lock_time_max);
        
        return TRUE;
    } 
    
    if (err_new || !err_old)    autoERR[x].dly = 0;

    if (autoERR[x].dly >= val_auto_reset_time && err_old)
    {
        if (autoERR[x].en)
        {        
            if (autoERR[x].have_reset)   /* 已经复位过,再出现故障时故障次数加 1 */
            {
                if (++autoERR[x].cnt > 2*MAX_ALLOW_TIMES) autoERR[x].cnt = MAX_ALLOW_TIMES + 1;
                
                for (y=0; y<MAX_ALLOW_TIMES; y++)
                {
                    if (autoERR[x].cnt == MAX_ALLOW_TIMES + 1 + y)
                    {
                        if (autoERR[x].reset_time[y] <= val_auto_reset_time1)   /* 时间小于设定时间，需要手动复位 */
                        {
                            autoERR[x].en = 0;     /* 置位禁止自动复位标志，在手动复位前不再进入自动复位处理 */
                        }
                        else  /* 时间大于设定时间 */
                        {
                               autoERR[x].reset_time[y] = 0;        /* 故障次数超过3次,但已超过设定时间，重新计时 */
                        }   
                        break;
                    }
                }
                autoERR[x].have_reset = 0;
            }
            
            if (autoERR[x].en)  /* 这个条件不能省，因为在这之前有对该标志的处理 */
            {               
                if (err_new == 0) /* 故障已消除 */
                {
                    autoERR[x].have_reset = 1;
                    return TRUE;    /* 复位该故障 */
                }
            }
        }
    }

    /*判断是否已经连续三次故障*/
    if (err_old) 
    {
        if (autoERR[x].en)
        {
            if (autoERR[x].cnt == 2)
            {
                if (autoERR[x].reset_time[0] <= val_auto_reset_time1)   /* 时间小于设定时间，需要手动复位 */
                    {
                        autoERR[x].auto_en = FALSE;     /* 置位禁止自动复位标志，在手动复位前不再进入自动复位处理 */
                    }
            }

            if (autoERR[x].cnt == 3)
            {
                if (autoERR[x].reset_time[1] <= val_auto_reset_time1)   /* 时间小于设定时间，需要手动复位 */
                {
                    autoERR[x].auto_en = FALSE;      /* 置位禁止自动复位标志，在手动复位前不再进入自动复位处理 */
                }  
            }
        
            if (autoERR[x].cnt == 4)
            {
                if (autoERR[x].reset_time[0] <= val_auto_reset_time1)   /* 时间小于设定时间，需要手动复位 */
                {
                    autoERR[x].auto_en = FALSE;      /* 置位禁止自动复位标志，在手动复位前不再进入自动复位处理 */
                }  
            }
        }
    }

    if(autoERR[x].en == FALSE)      autoERR[x].auto_en=FALSE;
    
    return FALSE;
}
/***************************************************************************
* 函数： Error_initial_auto_reset_arg()
* 功能：初始化自动复位相关变量
* 参数：某个故障的故障号
* 返回值：无
****************************************************************************/
void err_limit_auto_reset_init(void)
{
    U08 x;
    
    for (x=0; x<MAX_AUTO_RESET_NUM; x++)
    {
        autoERR[x].en = 1;
        autoERR[x].auto_en = 1;     //jxj 2016.01.26
        autoERR[x].have_reset = 1;
        autoERR[x].cnt = 0;
        ARRCLR(autoERR[x].reset_time);

        autoERR[x].err_time_lock = val_am_lock_time_min;
    }
}


/*******************************************************************************
 函 数: err_mod_limit_auto_reset()
 功 能: 处理限制性自动复位故障
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void err_mod_limit_auto_reset(void)  
{
    U08 x;
//  U08 fg_err_resume_sysout_high=0;    // 出水过高恢复标志  
    U08 fg_err_resume=0;                // 故障恢复标志 （统一用一个即可）
    
    
    for (x=0; x<self_comp_num; x++)  
    {
        //压机低压（开关）
        if (err_limit_auto_reset_deal((AUTOERR_LP_DI_1), s_get_error(err_lp[x]), s_get_input(x, IN_LP))) 
        {
            s_clr_error(err_lp[x]);
        }
        //压机高压（开关）
        if (err_limit_auto_reset_deal((AUTOERR_HP_DI_1), s_get_error(err_hp[x]), s_get_input(x, IN_HP))) 
        {
            s_clr_error(err_hp[x]);
        }
        
        //压机低压（压传）
        if (Cm[x].run_mode==MODE_COOL && pv_cell_lp(x)>=(val_lp_value_incl + val_lp_prs_exit_diff)
        ||  Cm[x].run_mode==MODE_HEAT && pv_cell_lp(x)>=(val_lp_value_inht + val_lp_prs_exit_diff)
        ||  SensorCannotUse(pv_cell_lp(x))
        )
        {   
            fg_err_resume = FALSE;
        }   
        else
        {
            fg_err_resume = TRUE;
        }
        
        if (err_limit_auto_reset_deal((AUTOERR_LP_AI_1), s_get_error(err_lp_AI[x]), fg_err_resume)) 
        {
            s_clr_error(err_lp_AI[x]);
        }

        //压机高压（压传）
        if (Cm[x].run_mode==MODE_COOL && pv_cell_hp(x)<=(val_hp_value_incl - val_hp_prs_exit_diff)
        ||  Cm[x].run_mode==MODE_HEAT && pv_cell_hp(x)<=(val_hp_value_inht - val_hp_prs_exit_diff)
        ||  SensorCannotUse(pv_cell_hp(x))
        )
        {   
            fg_err_resume = FALSE;
        }   
        else
        {
            fg_err_resume = TRUE;
        }
        
        if (err_limit_auto_reset_deal((AUTOERR_HP_AI_1), s_get_error(err_hp_AI[x]), fg_err_resume)) 
        {
            s_clr_error(err_hp_AI[x]);
        }


        //压机排气温度过高
        if (pv_gas_out_high(x) < (val_gas_out_high - ERR_WT_EXIT))
        {   
            fg_err_resume = FALSE;
        }   
        else
        {
            fg_err_resume = TRUE;
        }
        
        if (err_limit_auto_reset_deal((AUTOERR_GAS_1), s_get_error(err_exh_high[x]), fg_err_resume)) 
        {
            s_clr_error(err_exh_high[x]);
        }
    
        /* 变频故障自复位:部分故障不允许自复位 */
        if (val_invt_err_auto_reset != INVT_RST_NONE)
        {
            BOL fg_err = FALSE;
            U16 invt_err_id = CELL_INVT_ERR(pcb_addr, INVT_ID_1);
            
            if (val_invt_err_auto_reset == INVT_RST_PART
            && (invt_err_id == 11               /* 变频输出缺相 */
                || invt_err_id == 12   			/* 功率器件保护 */
            ))
            {
                fg_err = TRUE;
            }
            else
            {
                fg_err = FALSE;
            }
            if (err_limit_auto_reset_deal(AUTOERR_S_INVT, s_get_error(err_invt_id_to_err_id(INVT_ID_1, INVT_ERR_ERROR)), fg_err))
            {
                err_mod_invt_err_reset(INVT_ID_1);
            }
        }

        // 回水温度过高
        if (pv_cell_TWin1(pcb_addr) < val_err_ac_out_high - val_err_wt_exit - TEMP_IN_DIFF)
        {   
            fg_err_resume = FALSE;
        }   
        else
        {
            fg_err_resume = TRUE;
        }
        if (err_limit_auto_reset_deal(AUTOERR_S_IN_HIGH, s_get_error(err_in_high[x]), fg_err_resume)) 
        {
            s_clr_error(err_in_high[x]);
        }
        
        // 回水温度过低
        if (pv_cell_TWin1(pcb_addr) > val_err_ac_out_low+val_err_wt_exit+TEMP_IN_DIFF)
        {   
            fg_err_resume = FALSE;
        }   
        else
        {
            fg_err_resume = TRUE;
        }
        if (err_limit_auto_reset_deal(AUTOERR_S_IN_LOW, s_get_error(err_in_low[x]), fg_err_resume)) 
        {
            s_clr_error(err_in_low[x]);
        }

        /* 风机转速异常 */
        if (err_limit_auto_reset_deal(AUTOERR_S_PWM_FAN_EXCEPT, s_get_error(err_pwm_fan1[x])||s_get_error(err_pwm_fan2[x]), FALSE))
        {
            s_clr_error(err_pwm_fan1[x]);
            s_clr_error(err_pwm_fan2[x]);
        }
    }

    // 吸气温度过低
    if (pv_cell_gas_in(0) > (val_gas_in_low_cool+ERR_TEMP_DIFF))
    {   
        fg_err_resume = FALSE;
    }   
    else
    {
        fg_err_resume = TRUE;
    }
    if (err_limit_auto_reset_deal(AUTOERR_GASIN_LOW_1, s_get_error(err_gas_in_low[0]), fg_err_resume)) 
    {
        s_clr_error(err_gas_in_low[0]);
    }
    
    /* 制冷蒸发过低 */
    if (pv_evap_temp(0) > (val_evap_low_cool+ERR_TEMP_DIFF))
    {   
        fg_err_resume = FALSE;
    }   
    else
    {
        fg_err_resume = TRUE;
    }
    if (err_limit_auto_reset_deal(AUTOERR_EVAP_LOW_1, s_get_error(err_evap_low[0]), fg_err_resume)) 
    {
        s_clr_error(err_evap_low[0]);
    }

    /* 蒸发温度过低 */
    if (pv_evap_temp(0) < (val_evap_high_err - ERR_TEMP_DIFF))
    {   
        fg_err_resume = FALSE;
    }   
    else
    {
        fg_err_resume = TRUE;
    }
    if (err_limit_auto_reset_deal(AUTOERR_EVAP_HIGH, s_get_error(err_evap_high1[0]), fg_err_resume)) 
    {
        s_clr_error(err_evap_high1[0]);
    }
    
//    /* 压机电流过高 */
//    if (err_limit_auto_reset_deal(AUTOERR_S_CUR_HIGH, s_get_error(err_curr_high[0]), FALSE)) 
//    {
//        s_clr_error(err_curr_high[0]);
//    }
    // 水流开关保护：故障没恢复也认为无新故障
    if (err_limit_auto_reset_deal(AUTOERR_S_AC_FLUX, s_get_error(S_ERR_WATER_FLOW), FALSE)) 
    {
        s_clr_error(S_ERR_WATER_FLOW);
    }


    //出水温度过高
    if(pv_cell_TWout1(pcb_addr) < val_err_ac_out_high - val_err_wt_exit)
    {   
        fg_err_resume = FALSE;
    }   
    else
    {
        fg_err_resume = TRUE;
    }
    if (err_limit_auto_reset_deal(AUTOERR_S_OUT_HIGH, s_get_error(S_ERR_OUT_HIGH), fg_err_resume)) 
    {
        s_clr_error(S_ERR_OUT_HIGH);
    }
    
    //出水温度过低
    if(pv_cell_TWout1(pcb_addr) > val_err_ac_out_low + val_err_wt_exit)
    {   
        fg_err_resume = FALSE;
    }   
    else
    {
        fg_err_resume = TRUE;
    }
    if (err_limit_auto_reset_deal(AUTOERR_S_OUT_LOW, s_get_error(S_ERR_OUT_LOW), fg_err_resume)) 
    {
        s_clr_error(S_ERR_OUT_LOW);
    }

    if (err_limit_auto_reset_deal(AUTOERR_S_OUT_IN_DIFF, s_get_error(S_ERR_OUT_IN_DIFF), fg_out_in_diff_high))
    {
        s_clr_error(S_ERR_OUT_IN_DIFF);
    }

    if (err_limit_auto_reset_deal(AUTOERR_S_OUT_IN_ERR, s_get_error(S_ERR_OUT_IN_ERR), 0))
    {
        s_clr_error(S_ERR_OUT_IN_ERR);
    }
}

/************************************************
函数：s_Error_flag_init
功能：初始化各部件故障标志
参数：模块序号
      模块序号为MAX_PCB时为清除所有模块
返回值：无
*************************************************/
void err_mod_flag_init(void)
{
    U08 i;

    for (i=0; i<self_comp_num; i++)
    {
        mod_err_comp[i] = 0;
    }

    fg_water_lack_err = FALSE;
}
/******************************************************
函数：Error_reset
功能；处理故障复位
参数：无
返回值：无
******************************************************/
/* 以下为开关量与相应故障对应表,err_order_mod为模块板; err_order_sys为扩展板 */
//const U08 err_mod_input_order[16] = {ERR_WATER_LACK , ERR_BAK  ,  ERR_BAK , ERR_BAK , 
//                             ERR_BAK , ERR_BAK  ,  ERR_BAK , ERR_COOL_WATER_LACK ,
//                             ERR_BAK, ERR_OL_2 ,  ERR_HP_2, ERR_LP_2,
//                             ERR_BAK, ERR_OL_1 ,  ERR_HP_1, ERR_LP_1};

#if 0
const U08 err_mod_input_order[MAX_INPUT1] = {
                                                ERR_LP_1,   ERR_HP_1 ,  ERR_LP_2 , ERR_HP_2 , 
                                                ERR_BAK,    ERR_BAK,    S_ERR_COOL_FLUX_OL,    S_ERR_WATER_FLOW,
                                            };
#endif

//const U08 err_mod_input_order[MAX_INPUT1] = 
//{
//  S_ERR_WATER_FLOW,   S_ERR_COOL_FLUX_OL, ERR_BAK,    ERR_BAK,
//  ERR_HP_2,       ERR_LP_2,           ERR_HP_1,   ERR_LP_1,    
//};
                                   
/*******************************************************************************
 函 数: err_mod_manual_reset()
 功 能: 手动复位模块故障
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void err_mod_manual_reset(void)
{
    U08 x;

    // 开关量故障,故障消除后才可复位
    {
        if(!(s_get_input(0, IN_LP)))                        s_clr_error(err_lp[0]);
        if(!(s_get_input(0, IN_HP)))                        s_clr_error(err_hp[0]);

        s_clr_error(S_ERR_IBH_OVERLOAD); 
    
        /* 水流开关直接复位，因为水泵不开启的情况下水流开关无法恢复到正常位置 */
        s_clr_error(S_ERR_WATER_FLOW);
    }

    /* 变频水泵故障复位 */
    if (!s_get_output(MAX_COMP, S_DO_PUMPf)
    || PUMPF_can_reset_err()
    )
    {
        s_clr_error(S_ERR_PUMPF);
    }

    /* 三相电检测模块故障复位 */
    if (gIO.phase != PHASE_STA_ERR)    /* 错相保护复位 */
    {
        s_clr_error(S_ERR_PHASE_REVERSE);
    }
    if (gIO.phase != PHASE_STA_LACK)
    {
        s_clr_error(S_ERR_PHASE_LACK);
    }

    
//  s_fg_stop_all_mac = FALSE;
       
    for (x=0; x<USE_COMP; x++)
    {       
//      s_clr_error(err_curr_high[x]);           /* 压机电流过大 */
//      s_clr_error(err_curr_low[x]);            /* 压机电流过小 */ 

        s_clr_error(err_pwm_fan1[x]);
        s_clr_error(err_pwm_fan2[x]);

        //探头复位
        if (!SensorIsError(pv_cell_wing(x)))              s_clr_error(err_wing[x]);
        if (!SensorIsError(pv_gas_out_high(x)))           s_clr_error(err_gas_out[x]);
        if (!SensorIsError(pv_cell_gas_in(x)))            s_clr_error(err_gas_in[x]);
        if (!(SensorIsError(pv_cell_evap(x))&&SensorCannotUse(pv_cell_TRFliq(x))))              s_clr_error(err_evap_in[x]);
        if (!SensorIsError(GET_EXIO_PRS(0, EXIO_AI_01)))  s_clr_error(err_j5_prs_sensor[x]);   /* J5压传故障 */
        if (!SensorIsError(GET_EXIO_PRS(0, EXIO_AI_02)))  s_clr_error(err_j6_prs_sensor[x]);   /* J6压传故障 */

        if (Cm[x].run_mode==MODE_COOL && pv_cell_lp(x)>=(val_lp_value_incl + val_lp_prs_exit_diff)
        ||  Cm[x].run_mode==MODE_HEAT && pv_cell_lp(x)>=(val_lp_value_inht + val_lp_prs_exit_diff)
        ||  SensorCannotUse(pv_cell_lp(x))
        )
        {   
            s_clr_error(err_lp_AI[x]);
        }   
        if (Cm[x].run_mode==MODE_COOL && pv_cell_hp(x)<=(val_hp_value_incl - val_hp_prs_exit_diff)
        ||  Cm[x].run_mode==MODE_HEAT && pv_cell_hp(x)<=(val_hp_value_inht - val_hp_prs_exit_diff)
        ||  SensorCannotUse(pv_cell_hp(x))
        )
        {   
            s_clr_error(err_hp_AI[x]);
        }
        if (pv_gas_out_high(x) < (val_gas_out_high - ERR_WT_EXIT))
        {   
            s_clr_error(err_exh_high[x]);
        }
        
        if (pv_cell_gas_in(x) > (val_gas_in_low_cool + ERR_TEMP_DIFF))
        {   
            s_clr_error(err_gas_in_low[x]);
        }

        if (pv_evap_temp(x) > (val_evap_low_cool + ERR_TEMP_DIFF))
        {   
            s_clr_error(err_evap_low[x]);
        }

        if (pv_evap_temp(x) < (val_evap_high_err - ERR_TEMP_DIFF))
        {   
            s_clr_error(err_evap_high1[x]);
        }

        if (pv_cell_gas_in(x) <= pv_cell_gas_out(x)+20)
        {   
            s_clr_error(err_gas_in_out_err[x]);
        }

        /* 紧急除霜频繁 */
        defrost_fg[x].emergency_defrost_frequent = FALSE;
        s_clr_error(err_emerg_defrost[x]);

        /* 回温过低、过高复位 */
        if(pv_cell_TWin1(pcb_addr) < val_err_ac_out_high-val_err_wt_exit-TEMP_IN_DIFF)  s_clr_error(err_in_high[x]);
        if(pv_cell_TWin1(pcb_addr) > val_err_ac_out_low+val_err_wt_exit+TEMP_IN_DIFF)   s_clr_error(err_in_low[x]); 

        /* 环温过低限制压机开启复位 */
        if (!TA_FORCE_COMP_OFF) s_clr_error(err_TA_limit_comp_on[x]);
    }
    //单元出温故障复位
    if (!SensorIsError(pv_cell_TWout1(pcb_addr)))
    {
        s_clr_error(S_ERR_TEMP_TWOUT1);
    }
    if(pv_cell_TWout1(pcb_addr) < val_err_ac_out_high - val_err_wt_exit)    s_clr_error(S_ERR_OUT_HIGH);
    if(pv_cell_TWout1(pcb_addr) > val_err_ac_out_low  + val_err_wt_exit)    s_clr_error(S_ERR_OUT_LOW);      

        
         
    //单元 回温故障复位
    if (!SensorIsError(pv_cell_TWin1(pcb_addr)))
    {
        s_clr_error(S_ERR_TEMP_TWIN1);
    }

    if (!fg_out_in_diff_high)
    {
        s_clr_error(S_ERR_OUT_IN_DIFF);
    }
    s_clr_error(S_ERR_OUT_IN_ERR);

    /* 变频器故障复位 */
    invt_err_reset();
#if 0
    {
        INVT_ID_ENUM id;
        for (id=INVT_FAN1; id<INVT_ID_MAX; id++)            /* 变频风机 */
        {
            invt_cmd_err_reset(id);
        }

        err_mod_invt_err_reset(INVT_ID_1);          /* 变频压机 */

//        m_clr_error(MAX_PCB, ERR_INVT_MODEL);

        fg_invt_commu_error = 0;
    }
#endif

    /* 内外机通信故障 */
    for (x = 0; x < USE_EXIO; x++)
    {
        if (!GET_EXIO_COMMU_ERR(x))
        {
            s_clr_error(err_inout_commu[x]);
        }

        if (!GET_EXIO_KLV_VER_ERR(x))
        {
            s_clr_error(err_inout_ver[x]);
        }
    }
    ec_fan_err_reset(); 

    self_commu_err = FALSE;
}
/*==========================================
    功能: 故障总接口
/==========================================*/
void err_mod(void)
{
//  U08  i;

    err_mod_auto_reset();           //部分模块故障自动复位
        
    err_mod_limit_auto_reset();     //部分模块故障限制性自动复位(要放到查询故障的前面) 

    if (fg_err_reset)   /* 手动复位*/
    {
        fg_err_reset = FALSE;
        

        err_mod_manual_reset();     //部分故障手动复位
        err_limit_auto_reset_init();
    }
        
    
    if (power_time < POWER_ON_INTO_WORK) return;    //上电前10秒不检测故障  
    
    err_mod_judge();                //检测模块故障
    
    err_mod_flag_init();        
    err_mod_class();                //模块故障归类
    err_mod_deal();                 //处理模块故障  


}

/*****************************************************
    功能：写累计运行时间到EEPROM
******************************************************/
//void runtime_write_to_eeprom(U08 x)
//{
////    U16 addr;
//  
//  pCell->cell_runtime[x]++;   /* 运行时间累加 */
//  
////    addr = MEM_RUNTIME_ADDR ;
////    eprom_wr_near_crc((U08 *)&pCell->cell_runtime[0], addr, MAX_RUNTIME*2);
//
//    DataAccess_CompRuntime(DATA_WRITE);
//}

/*****************************************************
    功能：模块各种计时器
******************************************************/
void err_mod_time(void)
{
    U08  x, y;
    
    if (power_time < POWER_ON_INTO_WORK) return;  
            
    if (F_TIME_BASIC_100MS_BACK)
    {
        for (x = 0; x< self_comp_num; x++) {
            if (current_alarm_time[x] != 0) current_alarm_time[x]--;
        }
        
        for (x = 0; x<MAX_COMP+1; x++)
        {
            for (y=0; y<S_MAX_ERR_TIME_100MS; y++)
            {
                if (mod_err_time_100ms[x][y] != 0xffff)         mod_err_time_100ms[x][y]++;
            }
        }

        _CNT(fan_high_dly);
        _CNT(fan_low_dly);
    }
    
    if (F_TIME_BASIC_SEC_BACK)
    {
        // 自动复位
        for (x=0; x<MAX_AUTO_RESET_NUM; x++)
        {
            for (y=0; y<MAX_ALLOW_TIMES; y++)
            {               
                if (autoERR[x].cnt > y && autoERR[x].reset_time[y] != 0xffff)
                {
                    autoERR[x].reset_time[y]++;
                }           
            }

            _CNT(autoERR[x].dly);
            _CNT(autoERR[x].err_time);
        }
        
        for (x = 0; x< self_comp_num; x++) 
        {
            _CNT(comp_realrun_time[x]);
            _CNT(cnt_force_sub_freq[x]);

            if (invt_info_freq_limit_id(INVT_ID_1))         /* ！！！多变频器时需更改 */
            {
                VALCLR(cnt_no_freq_limit[x]);
            }
            else
            {
                _CNT(cnt_no_freq_limit[x]);
            }

            _CNT_WHEN(cnt_refri_recy_lp[x], (   s_get_error(err_lp[x])
                                             || s_get_error(err_lp_AI[x])
                                             ));
        }
        
        
        /* 故障检测时间 */
        for (x=0; x<MAX_COMP+1; x++)
        {
            for (y=0; y<S_MAX_ERR_TIME_S; y++)
            {
                if (mod_err_time[x][y] != 0xffff) mod_err_time[x][y]++;
            }
        }

        for (x=0; x<TIME_CNT2_MAX; x++)
        {
            _CNT(condition_time_cnt_2[x]);
        }
        for (x=0; x<TIME_CNT3_MAX; x++)
        {
            _CNT(condition_time_cnt_3[x]);
        }

        _CNT(AdjustFreqPeriod);
        _CNT(para_init_dly);

        _CNT(rfg_corr_interval);
        
        /* 回油 */
        {
            _CNT(cnt_reoil_cycle);
            _CNT(cnt_reoil_time);
        }

        /* PUMPf */     
        if (USE_PUMPf_CTRL != PUMPF_CTRL_OFF)
        {
            _CNT(PUMPf_time.on);
            _CNT(PUMPf_time.off);
            if (fg_PUMPf_run == FALSE)             PUMPf_time.on = 0;
            else                                   PUMPf_time.off = 0;

            _CNT(cnt_PUMPf_off_dly);
            if (!fg_cell_pump_open && !fg_PUMPf_run)
            {
                _SETMAX(cnt_PUMPf_off_dly);
            }
            _CNT_WHEN(cnt_PUMPf_ctrl, fg_PUMPf_run);

            /* 间隔运行计时 */
            _CNT_WHEN(cnt_PUMPf_interval_on_off, (cnt_PUMPf_interval_on_off<PUMPF_INTERVAL_TIME));
        }

        /* 环境温度限制压机开启消抖计时 */
        for (x = 0; x < SUB_SYS_TYPE_MAX; x++)
        {
            for (y = 0; y < CTRL_MODE_MAX; y++)
            {
                _CNT(cnt_mod_comp_limit_TA_dly[x][y]);
            }
        }

        /* IBH2加载延时 */
        _CNT_WHEN(IBH2_add_dly, s_get_output(MAX_COMP, S_DO_IBH1));

		/* 防冻结电加热带计时 */
		_CNT(cnt_HT_on_dly);

        for (x = 0; x< self_comp_num; x++)
        {
            if (current_err_time[x]             != 0xffff)  current_err_time[x]++;
            if (current_high_time[x]            != 0xffff)  current_high_time[x]++;      
            if (cm_time[x].cnt_time_fan_delay   != 0xffff)  cm_time[x].cnt_time_fan_delay++;

            if (cm_time[x].defrost_time != 0xffff)  cm_time[x].defrost_time++;
            if (!s_get_cm_status(x, CM_DEFROST))    cm_time[x].defrost_time = 0;
                
            if (cm_time[x].frost_out_dly != 0xff)   cm_time[x].frost_out_dly++;
            
            if(val_defrost_time_sel == 0) /* 积霜累积运行时间 */
            {
                wing_temp_confirm[x]++;         /* 运行60S后才开始检测翅片温度 */
                if ((cm_time[x].on <= 60) || (pv_cell_wing(x) <= 0)) wing_temp_confirm[x] = 0;
                if (wing_temp_confirm[x] > 30) cm_time[x].defrost_space = 0;        /* 翅片温度持续30S大于0时除霜累计时间清零 */
            }
           
            /* 温度数据记录 */
            {
                /* 排气温度 */
                _CNT(mod_rec_gas_out_cnt[x]);
                if (mod_rec_gas_out_cnt[x] >= REC_NORMAL_TEMP_INTV)
                {           
                    mod_rec_gas_out_cnt[x] = 0;
                    mod_rec_gas_out_avg[x] = fun_rec_avg(pv_gas_out_high(x), &mod_rec_gas_out[x][0], 
                                                ARRELE(mod_rec_gas_out[x]));
                }
            }
        }

        /* 压机相关时间 */
        for (x = 0; x< self_comp_num; x++)
        {
            if (Cm[x].dly != 0xffff)            Cm[x].dly++;
            if (Cm[x].defrost_dly != 0xffff)    Cm[x].defrost_dly++;

//          if(defrost_time_count[x]!=0xffff)       defrost_time_count[x]++;     
            
            if (s_get_cm_status(x, CM_ON)||s_get_cm_status(x, CM_DEFROST)) // 除霜过程会短时间关压机，为保持计时连续，增加CM_DEFROST条件  
//          if (s_get_output(x, SW_CM)) 
            {
#ifndef DEBUG   //DEBUG-CXW
                if (++cm_time[x].run > 3600) 
                {
                    cm_time[x].run = 0;

                    pCell->cell_runtime[x]++;   /* 运行时间累加 */
                    DataAccess_CompRuntime(DATA_WRITE);
                }
#else
                if (++cm_time[x].run > 360) // 调试，6分钟存入1小时
                {
                    cm_time[x].run = 0;
                    pCell->cell_runtime[x]++;   /* 运行时间累加 */
                    DataAccess_CompRuntime(DATA_WRITE);
                }
#endif              
                
                cm_time[x].off = 0;
                if (cm_time[x].on != 0xffff) cm_time[x].on++;
            }
            else 
            {
                cm_time[x].on = 0;
                if (cm_time[x].off != 0xffff) cm_time[x].off++;
            }
            
             
            if (s_get_output(x, SW_CM)) // 以继电器输出为准，除霜时若关压机则清零不计时
            {
                if (cm_time[x].sw_on != 0xffff) cm_time[x].sw_on++;
            }
            else
            {
                cm_time[x].sw_on=0;
            }

            _CNT_WHEN(cnt_reoil_end[x], !log_cmp_is_reoil(x));

        }
        /* 共同退出除霜时间 */
        for (x = 0; x< self_comp_num; x++)
        {
            if(s_have_comp_defrost())
            {
                cm_time[x].defrost_exit = 0;
            }
            else
            {
                if (cm_time[x].defrost_exit != 0xffff)  cm_time[x].defrost_exit++;
            }
       }
       /* 单独退出除霜时间 */
        for (x = 0; x< self_comp_num; x++)
        {
     //     if(s_get_cm_status(x, CM_DEFROST_EXIT))
     //     {
                if (cm_time[x].defrost_exit_sin != 0xffff)  cm_time[x].defrost_exit_sin++;
     //     }
      //    if(s_get_cm_status(x, CM_DEFROST))
      //    {
      //        
      //    }
       }
        /* 四通阀相关时间 */
//      for (x = 0; x< self_comp_num; x++)   
//      {   
//          if (s_get_output(x, SW_S1)) 
//          {
//              valve_time[x].off = 0;
//              if (valve_time[x].on != 0xffff) valve_time[x].on++;
//          }
//          else 
//          {
//              valve_time[x].on = 0;
//              if (valve_time[x].off != 0xffff) valve_time[x].off++;
//          }
//        } 
        
         
        if (pv_environment_stable_dly != 0xffff) pv_environment_stable_dly++;   // 环温消抖时间

        _CNT(invt_common_run_time[INVT_ID_1]);
        _CNT(fan_speed_adjust_cnt);
        _CNT(fan_restart_cnt);

        for (x=0; x<FREQSPEEDNUM; x++)
        {
            _CNT(cnt_pwm_fan_ctrl_period[x]);
        }

        for (x=0; x<EEV_ID_MAX; x++)
        {
            _CNT(motor_run_time[x]);
        }

        _CNT(invt_off_speed_down_cnt[INVT_ID_1]);
        _CNT(close_defro_heat_cnt);

        _CNT(cnt_invt_chg_speed);
    }
/*----------------------分计时----------------------------------*/  
    if (F_TIME_BASIC_MIN_BACK)
    {
         for( x=0; x<self_comp_num; x++)
        {
            /* 正在除霜 */
           if(val_defrost_time_sel == 0)
           {
                /* 正在除霜 */
                if (s_get_cm_status(x, CM_DEFROST)) 
                {
                    cm_time[x].defrost_space = 0;  /* 除霜间隔 */
                }
                else if (cm_time[x].on > 60 && (pv_cell_wing(x) <= 0))  /*运行时才计时 */
                {
                    if (cm_time[x].defrost_space != 0xffff)  cm_time[x].defrost_space++;
                }
           }
           else if(val_defrost_time_sel == 1)
           {
                /* 正在除霜 */
                if (s_get_cm_status(x, CM_DEFROST)) 
                {
                    cm_time[x].defrost_space = 0;  /* 除霜间隔 */
                }
                else if (s_get_cm_status(x, CM_ON))  /*运行时就计时 */
                {
                    if (cm_time[x].defrost_space != 0xffff)  cm_time[x].defrost_space++;
                }
           }
        }

        
    }
}
