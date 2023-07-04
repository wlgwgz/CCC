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
/* ���ϴ��� */
#define S_MAX_ERR_TIME_S    20
static U16 mod_err_time[MAX_COMP+1][S_MAX_ERR_TIME_S];              /* ʱ������ */
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
/* ��Ԫ���� */
#define cnt_err_out_high                    (mod_err_time[MAX_COMP][0])
#define cnt_mod_err_PUMP_on                 (mod_err_time[MAX_COMP][1]) /* ˮ�ÿ�����ʱ��ʱ */
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

#define S_MAX_ERR_TIME_100MS    5                       /* 100�����ʱ */
static U16 mod_err_time_100ms[MAX_COMP+1][S_MAX_ERR_TIME_100MS];    /* ʱ������(100ms) */
#define cnt_err_hp_onoff(x)                 (mod_err_time_100ms[x][0])
#define cnt_err_hp_press(x)                 (mod_err_time_100ms[x][1])
/* ��Ԫ���� */
#define cnt_err_ibh_ol                      (mod_err_time_100ms[MAX_COMP][0])

static U16 current_err_time[MAX_COMP];      /* ������ʱ��� */
static U16 current_high_time[MAX_COMP];     /* ѹ���������߼����ʱ(1S��ʱ) */
static U16 current_alarm_time[MAX_COMP];    /* ѹ���������߼���ʱ */


U08 wing_temp_confirm[MAX_COMP];
//#define val_auto_reset_time1       (60*60)    /* �Զ���λ����ʱ�� (�̶�Ϊ60����)*/
//#define val_lp_detect_dly             (60)    /* ��ѹ�����ʱ(�̶�Ϊ60s) */
#define ERR_RESET_TIME                  180     //���ϳ�������ʱ��
//#define val_cool_lp_alarm_delay           (5)     /* �����ѹȷ����ʱ */ 
//#define val_heat_lp_alarm_delay           (5)     /* ���ȵ�ѹȷ����ʱ */ 

//#define val_defrost_lp_limit_dly      (60)    /* �˳�˪��ѹ���ϼ����ʱ */

U08 cnt_mod_comp_limit_TA_dly[SUB_SYS_TYPE_MAX][CTRL_MODE_MAX];		/* �����¶�����ѹ������������ʱ */

/*******************************************************************************
 �� ��: err_mod_init()
 �� ��: ģ����ϱ�����ʼ��
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void err_mod_init(void)
{
    U08 x, y;

    /* ���ϱ��� */
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
        _SETMAX(cnt_err_lp_detect_dly(x));  /* ��������ѹʱ�˹���ѹ�����ʱ */
    }
    
    err_limit_auto_reset_init();
}

/* ========================================================================================== */

/*******************************************************************************
 �� ��: err_invt_id_to_err_id()
 �� ��: ��ȡ���ư�ı�Ƶ�����Ϻ�
 �� ��: id--��Ƶ��; 
        type--��������; 
 �� ��: 
 �� ��: ��
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
    ������err_mod_judge_curr
    ���ܣ����ѹ��������
    ��������ǰ����ֵ
    ����ֵ���Ƿ񱨾�
************************************************/
#define CURR_VALID              3       /* �����Ч����(0.1A) */
#define CURR_ERR_DLY            2       /* ����������������ʱ(1S) */
const U16 cur_alarm_time[7] = {600, 480, 240, 80, 50, 10, 0};  /* ����������ʱ�����߱�(0.1S����) */
static U08 err_mod_judge_curr(I16 current_value, U08 x)
{

    U08 current_ratio = 1;                 /* ��������: Iʵ/I�� */
    U08 cnt_curr_err_dly = 2;           
    
    cnt_curr_err_dly=CURR_ERR_DLY;
    
    if (!s_get_output(x, SW_CM)) 
    {
        current_err_time[x] = 0;    /* ѹ������ʱ�ż�� */
        current_high_time[x] = 0;   /* ѹ������ʱ�ż�� */   
    }
    
    /* δ��⵽����(����ԭ�򣺽Ӵ��ߡ�ȱ�ࡢ��������) */
    if (current_value > CURR_VALID)                     current_err_time[x] = 0;
    if (current_err_time[x] > cnt_curr_err_dly)         s_set_error(err_curr_low[x]);    
    
    if (current_high_time[x] > CURRENT_DLY)    /* ����ʱ�䳬����ʱʱ��ſ�ʼ������ */ 
    {     
        if (current_value >= MAX_CURR)
        {
            current_ratio = 40;     /* �������ڲ�����Χʱ��ǿ������4�� */
        }
        else                         /* �����������޹���ʱ�ż�������С */
        {
            current_ratio = current_value * 10 / val_current_rated;
        }
    }
    
    
    /* ����������������ͬ���������ȡСֵ */
    if      (current_ratio < 12) current_alarm_time[x] = 0xffff;        /* δ���أ��������õ���� */
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
������sensor_used_temp_evap_out()
���ܣ�������ˮ�¶�ʹ���ж�
������ѹ�����
����ֵ��ʹ��; ��ʹ��
******************************************/
//U08 sensor_used_temp_out(U08 comp_num)
//{
//  return (para_C[TEMP_USE_N][EVAP_OUT_TEMP_USE + comp_num]);
//}
/******************************************
������sensor_used_temp_cond_out()
���ܣ�������ˮ�¶�ʹ���ж�
������ѹ�����
����ֵ��ʹ��; ��ʹ��
******************************************/
//U08 sensor_used_temp_cond_out(U08 comp_num)
//{
//  return (para_C[TEMP_USE_N][COND_OUT_TEMP_USE + comp_num]);
//}

/*******************************************************
������Error_temp_too_low_special()
���ܣ�������¹��͹��ϵı������Զ���λ: ���ϱ�������
��������ǰֵ; �˳�ֵ; ����ֵ; ���ϱ���; ������ʱ
����ֵ����
*******************************************************/
void err_mod_temp_too_low_spec(I16 p_value, I16 exit_value, I16 err_value, U08 *err, U16 *err_time)
{
    if (p_value > exit_value || SensorCannotUse(p_value))               /* �ﵽ�˳��������Զ���λ */
    {
        *err = FALSE;
        *err_time = 0;
    }
    else if (p_value > err_value)               /* δ�ﵽ������������ʱ���� */
    {
        *err_time = 0;
    }
    else if (*err_time >= ERR_DLY)  /* ��ʱʱ�䵽������ */
    {
        *err = TRUE;
    }
}
/****************************************************
������Error_temp_too_low()
���ܣ�������¹��͹��ϵı������Զ���λ�����ϺŲ���
��������ǰֵ; �˳�ֵ; ����ֵ; ���Ϻ�; ������ʱ
����ֵ����
*****************************************************/
void err_mod_temp_too_low(I16 p_value, I16 exit_value, I16 err_value, U16 err_no, U16 *err_time)
{
    if (p_value > exit_value)               /* �ﵽ�˳��������Զ���λ */
    {
        s_clr_error(err_no);
    }
    
    if (p_value > err_value)                /* δ�ﵽ������������ʱ���� */
    {
        *err_time = 0;
    }
    else if (*err_time >= ERR_DLY)  /* ��ʱʱ�䵽������ */
    {
        s_set_error(err_no);
    }
}
/******************************************
������Error_temp_too_high()
���ܣ�������¹��͹��ϵı������Զ���λ
��������ǰֵ; �˳�ֵ; ����ֵ; ���Ϻ�;
����ֵ������; ������
******************************************/
void err_mod_temp_too_high(I16 p_value, I16 exit_value, I16 err_value, U08 err_no, U16 *err_time)
{
    if (p_value < exit_value)               /* �ﵽ�˳��������Զ���λ */
    {
        s_clr_error(err_no);
    }
    
    if (p_value < err_value)                /* δ�ﵽ������������ʱ���� */
    {
        *err_time = 0;
    }
    else if (*err_time >= ERR_DLY)  /* ��ʱʱ�䵽������ */
    {
        s_set_error(err_no);
    }
}

/******************************************
������err_mod_invt_except()
���ܣ�ѹ��ת���쳣�ж�
������ѹ����
����ֵ���쳣; ����
******************************************/
#define INVT_EXCEPT_NULL         (0) // �޹���
#define INVT_EXCEPT_ONLY_RESET   (1) // �����ͱ�Ƶ����λ����
#define INVT_EXCEPT_STOP_RESET   (2) // ͣ����λ
U08 invt_except_info[MAX_COMP];

void err_mod_invt_except(U08 cm)
{
    #define INVT_EXCEPT_MIN     (20)        // ѹ��ת���쳣���ת��
    #define INVT_EXCEPT_DLY     (5*QK_60)   // ѹ��ת���쳣�����ʱ

    U08 except_info = INVT_EXCEPT_NULL;
    if (!(((INVT_ID_ENUM)cm)==INVT_ID_1 && val_inverter_used)   // ��Ƶ����ʹ��
        || pv_cell_invt_set(cm) == 0                            // Ŀ��ת����Ч
        || pv_cell_invt_cur(cm) >= INVT_EXCEPT_MIN              // ��ǰת������
        || s_get_error(err_invt_err[cm])                        // ��Ƶ����
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

    if (cnt_err_invt_exc_up(cm) > 10    // ���ڼ�ؽ���仯
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
    ������s_cell_have_mode
    ���ܣ��ж�ģ���Ƿ��Ƿ���ָ��������ģʽ
    ������ָ��ģʽ
    ����ֵ��TRUE: ����ָ��ģʽ; FALSE: ��ָ��ģʽ
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
������Error_temp_too_low()
���ܣ�������¹��͹��ϵı������Զ���λ�����ϺŲ���
��������ǰֵ; �˳�ֵ; ����ֵ; ���Ϻ�; ������ʱ
����ֵ����
*****************************************************/
void Error_temp_too_low(I16 p_value, I16 exit_value, I16 err_value, U16 err_no, U16 *err_time)
{
    if (p_value > exit_value)               /* �ﵽ�˳��������Զ���λ */
    {
        s_clr_error(err_no);
    }
    
    if (p_value > err_value)                /* δ�ﵽ������������ʱ���� */
    {
        *err_time = 0;
    }
    else if (*err_time >= ERR_DLY)  /* ��ʱʱ�䵽������ */
    {
        s_set_error(err_no);
    }
}

/******************************************
������Error_temp_too_high()
���ܣ�������¹��͹��ϵı������Զ���λ
��������ǰֵ; �˳�ֵ; ����ֵ; ���Ϻ�;
����ֵ������; ������
******************************************/
void Error_temp_too_high(I16 p_value, I16 exit_value, I16 err_value, U08 err_no, U16 *err_time)
{
    if (p_value < exit_value)               /* �ﵽ�˳��������Զ���λ */
    {
        s_clr_error(err_no);
    }
    
    if (p_value < err_value)                /* δ�ﵽ������������ʱ���� */
    {
        *err_time = 0;
    }
    else if (*err_time >= ERR_DLY)  /* ��ʱʱ�䵽������ */
    {
        s_set_error(err_no);
    }
}
/******************************************
������Error_lp_detect_allow()
���ܣ���ѹ��⻷���¶�����
������
����ֵ�����; �����
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
 �� ��: err_mod_out_in_diff()
 �� ��: �����²���󡢳����²��쳣���
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void err_mod_out_in_diff(void)
{
    static BOL fg_high = FALSE;                     /* �����²���� */
    static BOL fg_except = FALSE;                   /* �����²��쳣 */

    #define TEMP_DIFF 50    /* �ָ��ز� */

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
        /* �����²���� */
        if (temp_high-temp_low <= MAX(val_out_in_diff_high-TEMP_DIFF, 10))
        {
            fg_high = FALSE;
        }
        
        /* �����²��쳣 */
        fg_except = FALSE;
        
        cnt_err_outin_high = 0;
        cnt_err_outin_err = 0;
    }

    /* �����²���� */
    if (fg_high)
    {
        s_set_error(S_ERR_OUT_IN_DIFF);
        fg_out_in_diff_high = TRUE;
    }
    else
    {
        fg_out_in_diff_high = FALSE;
    }

    /* �����²��쳣 */
    if (fg_except)
    {
        s_set_error(S_ERR_OUT_IN_ERR);
    }

    /* ���ó����²���� */
    if (val_out_in_diff_high == 0)  s_clr_error(S_ERR_OUT_IN_DIFF);
    /* ���ó����²��쳣 */
    if (val_out_in_diff_err == 0)  s_clr_error(S_ERR_OUT_IN_ERR);
}

#ifdef USE_SW_MAC
enum {MAC_IS_OK, MAC_IS_NONE, MAC_IS_MISMATCH};
#endif

#if defined(M_DEBUG)
/*******************************************************************************
 �� ��: err_mod_debug_mac_match()
 �� ��: DEBUG����У��
 �� ��: ��
 �� ��: 
 �� ��: ��
*******************************************************************************/
U08 err_mod_debug_mac_match(void)
{
    U08 err_id = MAC_IS_OK;
    
    switch (sw_mac_model)
    {
        
        case MAC_NONE:
            err_id = MAC_IS_NONE;  /* ����δ���û򳬳���Χ */
            break;
            
        case MAC_MULTI_MODEL_1:
            if (sw_mac_model_EXIO != sw_mac_model)
            {
                err_id = MAC_IS_MISMATCH;  /* ��������Ͳ�ƥ�� */
            }
            break;
            
        case MAC_MULTI_MODEL_2:
            if (sw_mac_model_EXIO != sw_mac_model)
            {
                err_id = MAC_IS_MISMATCH;  /* ��������Ͳ�ƥ�� */
            }
            break;
            
        default:
            break;
    }

    return err_id;
}
#elif defined(M_TCL)
/*******************************************************************************
 �� ��: err_mod_tcl_mac_match()
 �� ��: TCL����У��
 �� ��: ��
 �� ��: 
 �� ��: ��
*******************************************************************************/
U08 err_mod_tcl_mac_match(void)
{
    U08 err_id = MAC_IS_OK;
    
    switch (sw_mac_model)
    {
        default:
        case MAC_NONE:
            err_id = MAC_IS_NONE;  /* ����δ���û򳬳���Χ */
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
            /* ����ʽ��������У����������� */
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
                err_id = MAC_IS_MISMATCH;  /* ��������Ͳ�ƥ�� */
            }
            break;
            
        case MAC_F3_10KW:
            if (sw_mac_model_EXIO != sw_mac_model
            &&  sw_mac_model_EXIO != MAC_F1_10KW
            )
            {
                err_id = MAC_IS_MISMATCH;  /* ��������Ͳ�ƥ�� */
            }
            break;

        case MAC_F3_8KW:
            if (sw_mac_model_EXIO != sw_mac_model
            &&  sw_mac_model_EXIO != MAC_F1_8KW
            )
            {
                err_id = MAC_IS_MISMATCH;  /* ��������Ͳ�ƥ�� */
            }
            break;
    }

    return err_id;
}
#elif defined(M_DISEN)
/*******************************************************************************
 �� ��: err_mod_disen_mac_match()
 �� ��: ��ɭ����У��
 �� ��: ��
 �� ��: 
 �� ��: ��
*******************************************************************************/
U08 err_mod_disen_mac_match(void)
{
    U08 err_id = MAC_IS_OK;
    
    switch (sw_mac_model)
    {
        
        case MAC_NONE:
            err_id = MAC_IS_NONE;  /* ����δ���û򳬳���Χ */
            break;
            
        default:
            /* 2022-11-17 ��ɭ����һ�塢����У�����ͳһΪ�������Ҫ���� */
            if (sw_mac_model_EXIO != sw_mac_model)
            {
                err_id = MAC_IS_MISMATCH;  /* ��������Ͳ�ƥ�� */
            }
            break;
    }

    return err_id;
}
#elif defined(M_PAIWO)
/*******************************************************************************
 �� ��: err_mod_paiwo_mac_match()
 �� ��: ���ֻ���У��
 �� ��: ��
 �� ��: 
 �� ��: ��
*******************************************************************************/
U08 err_mod_paiwo_mac_match(void)
{
    U08 err_id = MAC_IS_OK;
    
    switch (sw_mac_model)
    {
        
        case MAC_NONE:
            err_id = MAC_IS_NONE;  /* ����δ���û򳬳���Χ */
            break;
            
        default:
            /* 2022-11-19 ���ַ���һ�塢����У�����ͳһΪ�ڻ����룬��������� */
//            if (sw_mac_model_EXIO != sw_mac_model)
//            {
//                err_id = MAC_IS_MISMATCH;  /* ��������Ͳ�ƥ�� */
//            }
            break;
    }

    return err_id;
}
#elif defined(M_NIUENTAI)
/*******************************************************************************
 �� ��: err_mod_NET_mac_match()
 �� ��: Ŧ��̩����У��
 �� ��: ��
 �� ��: 
 �� ��: ��
*******************************************************************************/
U08 err_mod_NET_mac_match(void)
{
    U08 err_id = MAC_IS_OK;
    
    switch (sw_mac_model)
    {
        
        case MAC_NONE:
            err_id = MAC_IS_NONE;  /* ����δ���û򳬳���Χ */
            break;
            
        default:
            /* 2022-12-23�ݶ���һ�塢����У�����ͳһΪ�ڻ����룬��������� */
//            if (sw_mac_model_EXIO != sw_mac_model)
//            {
//                err_id = MAC_IS_MISMATCH;  /* ��������Ͳ�ƥ�� */
//            }
            break;
    }

    return err_id;
}
#elif defined(M_SHENGNENG)
/*******************************************************************************
 �� ��: err_mod_SN_mac_match()
 �� ��: ���ܻ���У��
 �� ��: ��
 �� ��: 
 �� ��: ��
*******************************************************************************/
U08 err_mod_SN_mac_match(void)
{
    U08 err_id = MAC_IS_OK;
    
    switch (sw_mac_model)
    {
        
        case MAC_NONE:
            err_id = MAC_IS_NONE;  /* ����δ���û򳬳���Χ */
            break;
            
        default:
            /* һ�塢����У�����ͳһΪ�ڻ����룬��������� */
//            if (sw_mac_model_EXIO != sw_mac_model)
//            {
//                err_id = MAC_IS_MISMATCH;  /* ��������Ͳ�ƥ�� */
//            }
            break;
    }

    return err_id;
}
#endif

/*******************************************************************************
 �� ��: err_mod_mac_set()
 �� ��: ���������쳣
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void err_mod_mac_set(void)
{
#ifdef USE_SW_MAC
    U08 err_id = MAC_IS_OK;
    U08 i = 0;
    BOL fg_err = FALSE;

    if (val_mac_set_check)
    {
        /* ��ȡ������Ͳ��� */
        for (i=0; i<SW_MAC_NUM; i++)
        {
            if (GET_EXIO_SW(0, i+MAX((6-SW_MAC_NUM), 0)))	/* HL081����λ��012345 */
            {
                SET_BIT(sw_mac_model_EXIO, SW_MAC_NUM-1-i);
            }
            else
            {
                CLR_BIT(sw_mac_model_EXIO, SW_MAC_NUM-1-i);
            }
        } 

        /* �ж϶�ȡ������������ͺ��Ƿ񳬽� */
        if (sw_mac_model_EXIO > MAC_MAX
        ||  sw_mac_model_EXIO == MAC_MAX
        ||  sw_mac_model_EXIO < MAC_NONE
        )
        {
            sw_mac_model_err = TRUE;
        }

        /* ƥ����� */
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
    #warning "δ�������ƥ�����" 
#endif
    }

    /* ͳһ����Ϊ�����������쳣�� */
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
    
    /* ���ã��������ͺŲ���δ���磩 */
    if (val_mac_set_check == UNUSED
    &&  sw_mac_model_err == FALSE
    )
    {
        s_clr_error(S_ERR_MAC_ERR);
    }
#endif
}

/*******************************************************************************
 �� ��: err_mod_low_env_cool_detect_allow()
 �� ��: �������������ʱ����ĳЩ���ϵļ��
 �� ��: ��
 �� ��: TRUE/FALSE--������/�����
 �� ��: ��
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
    ���ܣ�ģ��Ĺ��ϴ���
******************************************************/
static void err_mod_judge(void)
{
    U08 x;
    
    if (self_commu_err) return;           /* ͨѶ���ϣ������� */
    
/* =========== ģ����� =========== */

    /* ��ȱ�ౣ�� */
    if (pcb_addr==0 && val_00_board_alarm==SYS_POWER_ERR)
    {
        s_clr_error(S_ERR_PHASE_REVERSE);           //����
        s_clr_error(S_ERR_PHASE_LACK);              //ȱ��
    }
    else
    {
        #define ALARM_PHASE_NULL    (0)     // �޹���(���ڼ�¼�������)
        #define ALARM_PHASE_ERR     (1)     // ����(���ڼ�¼�������)
        #define ALARM_PHASE_LACK    (2)     // ȱ��(���ڼ�¼�������)
        
        BOL fg_allow = FALSE;
        static U08 alarm_phase_bak = ALARM_PHASE_NULL;  // ��¼�������

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

        /* ���ϵ����ϱ�����������ʱ��������ֹ�ϵ�ʱ�� */
        if (power_time < POWER_ON_JUST || cnt_err_phase_mod > POWER_DOWN_DLY)
        {
            fg_allow = TRUE;
        }

        if (fg_allow)
        {
            if (alarm_phase_bak == ALARM_PHASE_ERR)
            {
                s_set_error(S_ERR_PHASE_REVERSE);       //����
                alarm_phase_bak = ALARM_PHASE_NULL;
            }
		    if (alarm_phase_bak == ALARM_PHASE_LACK)
		    {
		        s_set_error(S_ERR_PHASE_LACK);         	//ȱ��
		        alarm_phase_bak = ALARM_PHASE_NULL;
		    }
        }
    }


    /* ��������� */
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
    
    /* =========== ѹ������ =========== */
    for (x=0; x<self_comp_num; x++)
    {
        BOL is_lp_low = FALSE;
        BOL is_lp_sw = FALSE;
        BOL is_hp_sw =FALSE;
        BOL is_hp_high =FALSE;

        static U08 fg_comp_on_bak[MAX_COMP];

        /* ѹ��������ʱ����� */ 
        if (!s_get_output(x, SW_CM))    
        {
            cnt_err_hp_onoff(x) = 0;
            cnt_err_hp_press(x) = 0;
        }   
        
        /* ѹ����ѹ */ 
        // ѹ����ѹ����
        if (!s_get_input(x, IN_HP))  
        {
            cnt_err_hp_onoff(x) = 0;
        }   
        else if (cnt_err_hp_onoff(x) >= ERR_DLY) 
        {
            if (!s_get_error(err_hp[x]))     /* ����δȷ��ʱ�����ٽ����ж� */
            {   
                is_hp_sw = TRUE;
            }
        }
        // ��ѹ������
        if(!SensorCanUse(pv_cell_hp(x)))         //������������
        {
            cnt_err_hp_press(x) = 0;     // ��ѹѹ����������
//            s_clr_error(err_hp_AI[x]);	/* 2022-9-9 ��ߵ�ѹ���ô��������л������������������ */
        }
        else
        {
            if(Cm[x].run_mode == MODE_COOL)        //�����ѹ 
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
            else    //���ȸ�ѹ
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

        /* ��ѹ�źŴ��� */
        if (err_mod_signal_err_hp(x, is_hp_high || is_hp_sw))
        {
            if (is_hp_sw)   s_set_error(err_hp[x]);
            if (is_hp_high) s_set_error(err_hp_AI[x]);
        }

        /* ѹ����ѹ */
        if (!val_check_lp_when_off)
        {
            if (!s_get_output(x, SW_CM))    /* ѹ�������У���ѹ�����ʱ���� */
            {
                cnt_err_lp_detect_dly(x) = 0;
            } 
        }
        else
        {
            /* ����ʱҲ����ѹ����Ӱ��ԭ�����еĵ�ѹ����߼� */
            if (s_get_output(x, SW_CM) != fg_comp_on_bak[x])
            {
                fg_comp_on_bak[x] = s_get_output(x, SW_CM);
                if (s_get_output(x, SW_CM))
                {
                    cnt_err_lp_detect_dly(x) = 0;
                }
            }
        }    
        
        if (cnt_err_lp_detect_dly(x) <= TF_VAL(val_lp_detect_dly, TF_LP_CHK_DLY)    /* ��ѹ�����ʱ������������ʱ���� */
        ||  s_get_cm_status(x, CM_DEFROST)          /* ��˪ʱ���ε�ѹ��� */
        ||  cm_time[x].cnt_time_fan_delay < val_defrost_fan_dly+val_defrost_lp_limit_dly
            )
        {
            cnt_err_lp_onoff_dly(x) = 0;     // ��ѹ������������
            cnt_err_lp_press_dly(x) = 0;     // ��ѹѹ����������
        }
        
        /* ��ѹ���� */
        if (Error_lp_detect_allow()
        && err_mod_low_env_cool_detect_allow()
        )
        {
            // ѹ����ѹ����
            if (!s_get_input(x, IN_LP) )  //|| (pv_cell_lp(x) > val_lp_value_incl)
            {
                cnt_err_lp_onoff_dly(x) = 0;
            }
            else 
            {
                if (Cm[x].run_mode == MODE_COOL)    /* �����ѹ */
                {
                    if (cnt_err_lp_onoff_dly(x) > TF_VAL(val_cool_lp_alarm_delay, TF_LP_ERR_DLY))
                    {
                        is_lp_sw = TRUE;
                    }
                }
                else                                /* ���ȵ�ѹ */
                {
                    if (cnt_err_lp_onoff_dly(x) > TF_VAL(val_heat_lp_alarm_delay, TF_LP_ERR_DLY))
                    {
                        if (!s_get_error(err_lp[x]))     /* ����δȷ��ʱ�����ٽ����ж� */
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
        
        // ��ѹ������
        if (!SensorCanUse(pv_cell_lp(x))         //������������
        ||  !err_mod_low_env_cool_detect_allow()
        )
        {
            cnt_err_lp_press_dly(x) = 0;     // ��ѹѹ����������
//            s_clr_error(err_lp_AI[x]);    /* 2022-9-9 ��ߵ�ѹ���ô��������л������������������ */
        }
        else
        {
            if(Cm[x].run_mode == MODE_COOL)        //�����ѹ 
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
            else    //���ȵ�ѹ
            {
                if(pv_cell_lp(x) > val_lp_value_inht)  
                {
                    cnt_err_lp_press_dly(x) = 0;  
                }
                else if(cnt_err_lp_press_dly(x) > TF_VAL(val_heat_lp_alarm_delay, TF_LP_ERR_DLY))
                {
                    if (!s_get_error(err_lp_AI[x]))     /* ����δȷ��ʱ�����ٽ����ж� */
                    {
                        is_lp_low = TRUE;
                    }
                }
            }
        }

        /* ��ѹ�źŴ��� */
        if (err_mod_signal_err_lp(x, is_lp_low || is_lp_sw))
        {
            if (is_lp_sw)   s_set_error(err_lp[x]);
            if (is_lp_low)  s_set_error(err_lp_AI[x]);
        }
        
//        /* ѹ���������� */
//      if (NO_CUR_SENSOR)
//      {
//            s_clr_error(err_curr_low[x]);
//          s_clr_error(err_curr_high[x]);
//      }
//      else
//      {
//          if (err_mod_judge_curr(pv_cell_curr(x), x)) 
//          {
//              s_set_error(err_curr_high[x]);  //��������  
//          }   
//      }
   
        //�¶ȹ���
        if (SensorIsError(pv_cell_wing(x)))         /* ��Ƭ̽ͷ���� */
        {
            s_set_error(err_wing[x]);   
        }
        /* ����̽ͷ���� */
        if (SensorIsExist(pv_cell_comp_top(x)) && SensorIsError(pv_gas_out_high(x))         
        ||  SensorNoExist(pv_cell_comp_top(x)) && SensorIsError(pv_cell_gas_out(x))
            )
        {
            s_set_error(err_gas_out[x]);    
        }
        /* J5ѹ������ */
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
        /* J6ѹ������ */
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
        if (SensorIsError(pv_cell_gas_in(x)))       /* ����̽ͷ���� */
        {
            s_set_error(err_gas_in[x]); 
        }
        if (SensorIsError(pv_cell_evap(x))&&SensorCannotUse(pv_cell_TRFliq(x))) 		/* ����̽ͷ���� */
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

        /* �����¶ȹ��� */
        if (SensorIsError(pv_cell_TA(pcb_addr))
        ) 
        {
            s_set_error(S_ERR_TEMP_TA);
        }
        else
        {
            s_clr_error(S_ERR_TEMP_TA);
        }

        if (log_mod_comp_is_run(MODE_NULL) && SensorCanUse(pv_cell_TWin1(pcb_addr)))    /* �����һ��¿���ʱ�ż�� */
        {
            if (s_cell_have_mode(MODE_HEAT)) 
            {   
                s_clr_error(err_in_low[x]);     /* ���������¶ȱ��� */

                if (pv_cell_TWin1(pcb_addr) < val_err_ac_out_high - TEMP_IN_DIFF)             /* δ�ﵽ������������ʱ���� */
                {
                    cnt_err_in_low(x) = 0;
                }
                else if (cnt_err_in_low(x) >= ERR_DLY)  /* ��ʱʱ�䵽������ */
                {
                    s_set_error(err_in_high[x]);
                }
            }
            else if (s_cell_have_mode(MODE_COOL)) 
            {   
                s_clr_error(err_in_high[x]);        /* ���������¶ȱ��� */
                
                if (pv_cell_TWin1(pcb_addr) >= val_err_ac_out_low+TEMP_IN_DIFF)             /* δ�ﵽ������������ʱ���� */
                {
                    cnt_err_in_low(x) = 0;
                }
                else if (cnt_err_in_low(x) >= ERR_DLY)  /* ��ʱʱ�䵽������ */
                {
                    s_set_error(err_in_low[x]); 
                }
            }           
        }

        /* ���¹�������ѹ������ */
        if (SensorCanUse(pv_cell_TA(pcb_addr))
        && TA_FORCE_COMP_OFF
        )
        {
            s_set_error(err_TA_limit_comp_on[x]);
        }
        
        //�����¶ȹ��߹���
        if(SensorCanUse(pv_gas_out_high(x)))
        {
            if (pv_gas_out_high(x) < val_gas_out_high)              /* δ�ﵽ������������ʱ���� */
            {
                cnt_err_gas_out_high(x) = 0;
            }
            else if (cnt_err_gas_out_high(x) >= ERR_DLY)    /* ��ʱʱ�䵽������ */
            {
                s_set_error(err_exh_high[x]);
            }                           
        }
        else     
        {
            s_clr_error(err_exh_high[x]);
        }

        // �����¶ȹ��͹���
        if (SensorCanUse(pv_cell_gas_in(x)) && CmIsCool(x))
        {
            if ((cm_time[x].on<180 && pv_cell_gas_in(x)>val_gas_in_low_cool-50)
            ||  (cm_time[x].on>=180 && pv_cell_gas_in(x)>val_gas_in_low_cool)
            ||  !err_mod_low_env_cool_detect_allow()
                )/* δ�ﵽ������������ʱ���� */
            {
                cnt_err_gas_in_low(x) = 0;
            }
            else if (cnt_err_gas_in_low(x) >= 30    /* ��ʱʱ�䵽������ */
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

        // ������������
        if (SensorCanUse(pv_evap_temp(x)) && CmIsCool(x))
        {
            if ((cm_time[x].on<180 && pv_evap_temp(x)>val_evap_low_cool-30)
            ||  (cm_time[x].on>=180 && pv_evap_temp(x)>val_evap_low_cool)
            ||  !err_mod_low_env_cool_detect_allow()
                )/* δ�ﵽ������������ʱ���� */
            {
                cnt_err_evap_low(x) = 0;
            }
            else if (cnt_err_evap_low(x) >= 30      /* ��ʱʱ�䵽������ */
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

        // �����¶ȹ���
        if (SensorCanUse(pv_evap_temp(x)) && val_evap_high_err!=0)
        {
            if (pv_evap_temp(x) < val_evap_high_err)/* δ�ﵽ������������ʱ���� */
            {
                cnt_err_evap_high(x) = 0;
            }
            else if (cnt_err_evap_high(x) >= 300      /* ��ʱʱ�䵽������ */
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
        
        /* ������˪Ƶ�� */
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

        // �����²��쳣
        if (SensorCanUse(pv_cell_gas_in(x)) 
        &&  SensorCanUse(pv_cell_gas_out(x)) 
        &&  val_gas_in_out_err_dly != 0)
        {
            if (cm_time[x].on < val_gas_in_out_err_dly
            ||  pv_cell_gas_in(x) < pv_cell_gas_out(x)+50
            ||  Cm[x].defrost_stage!=DS_IDLE
            ||  ((Cm[x].defrost_dly < val_gas_in_out_err_dly+300)
                  && (power_time > val_gas_in_out_err_dly+300))		/* ��ֹ�˳�˪���� */
                )/* δ�ﵽ������������ʱ���� */
            {
                cnt_err_gas_in_out(x) = 0;
            }
            else if (cnt_err_gas_in_out(x) >= ERR_DLY)  /* ��ʱʱ�䵽������ */
            {
                s_set_error(err_gas_in_out_err[x]);
            }                           
        }
        else     
        {
            s_clr_error(err_gas_in_out_err[x]);
        }

#if 0
        // ��Ƶ���� /* N0004 */
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

        /* PWM���ת���쳣 */   /* N0004 */
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

        // ��Ƶ������ 
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

            /* ��Ƶ�ͺ�������:����ѹ�� */
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
                    /* ���������ȡʱ�����ͬһ����Ƶ������������������Ϊ���������ʱ������ */
                    if (model != val_invt_motor_type)
                    {
                        s_set_error(err_invt_model[x]);
                        cnt_err_invt_model_ok(x) = 0;
                    }
                }
                
                /* ��120�롱�Ǹ�����ֵ�����ƵͨѶ�й� */
                if (cnt_err_invt_model_ok(x) > 120)
                {
                    s_clr_error(err_invt_model[x]);
                }

                // ��Ƶѹ��ת���쳣
                err_mod_invt_except(x);
            }
        }

        /* ������̽ͷ��ع��� */
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

        /* ��ý�����¶ȹ��� Ŀǰ������ */
        if (SensorIsError(pv_cell_TRFgas(x)))
        {
            s_set_error(err_TRFgas_sensor[x]);
        }
        else
        {
            s_clr_error(err_TRFgas_sensor[x]);
        }

        /* ��ýҺ���¶ȹ��� Ŀǰ������ */
        if (SensorIsError(pv_cell_TRFliq(x)))
        {
            s_set_error(err_TRFliq_sensor[x]);
        }
        else
        {
            s_clr_error(err_TRFliq_sensor[x]);
        }

    }

    /* �ܳ�ˮ�¶ȹ��� */

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

    
    /* ������ع��� */
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
        
        if((log_mod_comp_is_run(MODE_NULL) == TRUE)&& (SensorCanUse(temp_out)))   /* �����ҳ����޹���ʱ�ż�� */
        {
            if (s_cell_have_mode(MODE_HEAT)) 
            {   
                s_clr_error(S_ERR_OUT_LOW);     /* ���������¶ȱ��� */
                
                if (temp_out < val_err_ac_out_high)             /* δ�ﵽ������������ʱ���� */
                {
                    cnt_err_out_high = 0;
                }
                else if (cnt_err_out_high >= ERR_DLY)   /* ��ʱʱ�䵽������ */
                {
                    s_set_error(S_ERR_OUT_HIGH);
                }
            }
            else if (s_cell_have_mode(MODE_COOL)) 
            {   
                s_clr_error(S_ERR_OUT_HIGH);        /* ���������¶ȱ��� */
                
                 
                if (temp_out > val_err_ac_out_low)              /* δ�ﵽ������������ʱ���� */
                {
                    cnt_err_out_high = 0;
                }
                else if (cnt_err_out_high >= ERR_DLY)   /* ��ʱʱ�䵽������ */
                {
                    s_set_error(S_ERR_OUT_LOW); 
                }
            }           
        }
    }

    /* �������ȹ��� */
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
    
    /* ��Ƶˮ��δ������ʱ���� */
    if (!s_get_output(MAX_COMP, S_DO_PUMPf)) 
    {
        cnt_mod_err_PUMP_on = 0;
    }

    /* ģ��ˮ������: ˮ�������� */
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
        else if (!s_get_input(MAX_COMP, S_IN_WATER_FLOW)||s_get_error(S_ERR_WATER_FLOW))    // �������㣬��λ�����¼�    
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

    /* ��Ƶˮ�ù��� */
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

    /* ��Ƶˮ�þ��� */
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
     //������ر���
    if (!s_get_input(MAX_COMP, IN_FAN_OL)) 
    {
        cnt_err_fan_ol = 0;
    }   
    else if (cnt_err_fan_ol >= ERR_DLY) 
    {
        s_set_error(S_ERR_COOL_FLUX_OL);
    }
    
    // ��Դ����
    if (!s_get_input(MAX_COMP, IN_POWER_ERR))
    {
        cnt_err_power = 0;
    }
    else if (cnt_err_power >= ERR_DLY) 
    {
        s_set_error(S_ERR_POWER);
    }


    /* ����̽ͷ��ع��� */
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
            if (pv_cell_antifreez(pcb_addr) > val_antifrez_alarmtemp)               /* δ�ﵽ������������ʱ���� */
            {
                cnt_err_anti_ice = 0;
            }
            else if (cnt_err_anti_ice >= ERR_DLY)   /* ��ʱʱ�䵽������ */
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

    /* ����EC���ʱ��������� */
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
 �� ��: err_mod_class()
 �� ��: ģ����Ϲ���
 �� ��: ��
 �� ��: 
 �� ��: ��
*******************************************************************************/
static void err_mod_class(void)
{
    U08 x, exio_id;
    U08 bgn, end;
    
    if (self_commu_err) return;     /* ��ͨѶ����ʱ����ģ����ϴ��� */

    for (exio_id = 0; exio_id < USE_EXIO; exio_id++)
    {
        if (s_get_error(err_inout_commu[exio_id]) /* �����ͨѶ�쳣 */
        || s_get_error(err_inout_ver[exio_id]))  /* Э��汾���� */
        {
            bgn = end = 0;
            log_mod_unit_to_comp_group(val_comp_num, &bgn, &end, exio_id);
            for (x=bgn; x<end; x++)
            {
                mod_err_comp[x] = 1;
            }
        }
    }
    
    /* ��Ԫ���� */  
    // ��Ƶˮ�ù���
//    if (s_get_error(S_ERR_PUMPF))
//    {
//        mod_err_comp[x] = 1;
//      fg_pump_err = TRUE; 
//      fg_ac_sys_err = TRUE; 
//    }
    
    /* �շ�ʱ�������⴦�� */
    if (fg_refri_recy_act)
    {
        if (s_get_error(S_ERR_EEPROM)           // EERPOM���ݴ�
//        ||  s_get_error(S_ERR_POWER)            /* ��Դ���� */ 
        ||  s_get_error(S_ERR_WATER_FLOW)           // �յ�ˮ������
        ||  s_get_error(S_ERR_OUT_LOW)          // ��ˮ����
        || s_get_error(S_ERR_PUMPF)				/* ��Ƶˮ�ù��� */
        || s_get_error(S_ERR_MAC_ERR)		    /* ���������쳣 */
//      ||  s_get_error(S_ERR_ANTIFREZZ)        // �����¶ȹ���
        )
        {
            for (x=0; x<self_comp_num; x++)
            {
                mod_err_comp[x] = 1;
            }
            cmd_refri_recy_stop = TRUE;
        }
    }    /* �������ϴ��� */
    else if (s_get_error(S_ERR_WATER_FLOW)           // �յ�ˮ������
    ||  s_get_error(S_ERR_PHASE_LACK)          // ȱ�� 
    ||  s_get_error(S_ERR_PHASE_REVERSE)        // ����
    ||	gIO.phase == PHASE_STA_ERR          /* ȱ���ౣ������ʱ������������Ȼ��ʱ���� */
	||  gIO.phase == PHASE_STA_LACK
//    ||  s_get_error(S_ERR_POWER)            /* ��Դ���� */
    ||  s_get_error(S_ERR_OUT_HIGH)         // ��ˮ����
    ||  s_get_error(S_ERR_OUT_LOW)          // ��ˮ����
    ||  s_get_error(S_ERR_EEPROM)           // EERPOM���ݴ� 
//    ||  s_get_error(S_ERR_IBH_OVERLOAD)		/* �������ȹ��� */
//    ||  s_get_error(S_ERR_TEMP_ANTIFREZZ)   /* �����¶ȹ��� */
//  ||  s_get_error(S_ERR_ANTIFREZZ)        // �����¶ȹ���
    ||  s_get_error(S_ERR_OUT_IN_DIFF)      /* �廻�����²���� */
    ||  s_get_error(S_ERR_OUT_IN_ERR)       /* �廻�����²���� */
    ||  s_get_error(S_ERR_TEMP_TWOUT1)		/* �廻��ˮ�¶ȹ��� */
    ||  s_get_error(S_ERR_TEMP_TWIN1)		/* �廻��ˮ�¶ȹ��� */
    ||  s_get_error(S_ERR_TEMP_TWOUT2)		/* �ܳ�ˮ�¶ȹ��� */
    ||  s_get_error(S_ERR_TEMP_TA)			/* �����¶ȹ��� */
    || s_get_error(S_ERR_PUMPF)				/* ��Ƶˮ�ù��� */
    || s_get_error(S_ERR_MAC_ERR)		    /* ���������쳣 */
    )
    {
        for (x=0; x<self_comp_num; x++)
        {
            mod_err_comp[x] = 1;
        }
    }
    
    /* ѹ�����ع��� */
    if (s_get_error(S_ERR_OUT_LOW)          // ��ˮ����
    || s_get_error(S_ERR_PUMPF)				/* ��Ƶˮ�ù��� */
    ||  s_get_error(S_ERR_PHASE_LACK)           // ȱ�� 
    ||  s_get_error(S_ERR_PHASE_REVERSE)        // ����
    ||	gIO.phase == PHASE_STA_ERR          /* ȱ���ౣ������ʱ������������Ȼ��ʱ���� */
	||  gIO.phase == PHASE_STA_LACK
//    ||  s_get_error(S_ERR_POWER)            /* ��Դ���� */
//  ||  s_get_error(S_ERR_ANTIFREZZ)        // �����¶ȹ���
    )
    {
        for (x=0; x<self_comp_num; x++)
        {
            mod_err_comp_risk[x] = 1;
        }
    }
    
    /* ѹ�������� */
    for (x=0; x<self_comp_num; x++)
    {
        /* �շ�ʱ�������⴦�� */
        if (fg_refri_recy_act)
        {
            if (s_get_error(err_lp[x])&&cnt_refri_recy_lp[x]>val_recycle_lp_dly     /* ѹ����ѹ */
            ||  s_get_error(err_lp_AI[x])&&cnt_refri_recy_lp[x]>val_recycle_lp_dly  // ��ѹ����
            ||  s_get_error(err_hp[x])                   /* ѹ����ѹ */
            ||  s_get_error(err_hp_AI[x])                // ��ѹ����
            ||  s_get_error(err_exh_high[x])            /* �����¶ȹ��� */
            ||  s_get_error(err_in_low[x])               /* ��ˮ�¶ȹ��� */
            )                   
            {
                mod_err_comp[x] = 1;
                cmd_refri_recy_stop = TRUE;
            }
        }/* �������ϴ��� */
        else if ((s_get_error(err_lp[x])&&!TF_EN)   /* ѹ����ѹ */
        || s_get_error(err_hp[x])                   /* ѹ����ѹ */
//      || s_get_error(err_curr_low[x])             /* ѹ��������С */
//      || s_get_error(err_curr_high[x])            /* ѹ���������� */
        || s_get_error(err_wing[x])                 /* ��Ƭ�¶ȹ��� */
        || s_get_error(err_gas_out[x])              /* �����¶ȹ��� */
        || s_get_error(err_gas_in[x])               /* �����¶ȹ��� */
        || s_get_error(err_evap_in[x])              /* �����¶ȹ��� */
        || s_get_error(err_exh_high[x])             /* �����¶ȹ��� */
        || s_get_error(err_j5_prs_sensor[x])        /* J5ѹ������ */
        || s_get_error(err_j6_prs_sensor[x])        /* J6ѹ������ */
        || (s_get_error(err_lp_AI[x])&&!TF_EN)      // ��ѹ����
        || s_get_error(err_hp_AI[x])                // ��ѹ����
        || s_get_error(err_gas_in_low[x])           /* �����¶ȹ��� */
        || s_get_error(err_evap_low[x])             /* ������������ */
        || s_get_error(err_evap_high1[x])           /* �����¶ȹ��� */
        || s_get_error(err_emerg_defrost[x])        /* ������˪Ƶ�� */
        || s_get_error(err_gas_in_out_err[x])       /* �����²��쳣 */
        || s_get_error(err_in_low[x])               /* ��ˮ�¶ȹ��� */
        || s_get_error(err_in_high[x])              /* ��ˮ�¶ȹ��� */
        || s_get_error(err_pwm_fan1[x])             /* ���1ת���쳣 */             
        || s_get_error(err_pwm_fan2[x])             /* ���2ת���쳣 */ 
        || s_get_error(err_ecfan1_commu[x])         /* EC1ͨѶ���� */       /* N0004 */                  
        || s_get_error(err_ecfan1_err[x])           /* EC1���� */   /* N0004 */         
        || s_get_error(err_ecfan2_commu[x])         /* EC2ͨѶ���� */       /* N0004 */             
        || s_get_error(err_ecfan2_err[x])           /* EC2���� */   /* N0004 */ 
        || s_get_error(err_invt_commu[x])           /* ��ƵͨѶ���� */                
        || s_get_error(err_invt_err[x])             /* ��Ƶ���� */            
        || s_get_error(err_invt_model[x])           /* ��Ƶ�ͺ������� */ 
        || s_get_error(err_evi_in[x])               /* ����������¶ȹ��� */
        || s_get_error(err_evi_out[x])              /* �����������¶ȹ��� */
        || log_mod_comp_temp_out_limit(x)
        || s_get_error(err_TA_limit_comp_on[x])				    /* ���¹�������ѹ������ */
        || (invt_except_info[x] == INVT_EXCEPT_STOP_RESET)	    /* ѹ��ת���쳣 */
        )                   
        {
            mod_err_comp[x] = 1;
        }
    }
    /* ѹ�����ع��� */
    for (x=0; x<self_comp_num; x++)
    {
        if(s_get_error(err_hp[x])                   /* ѹ����ѹ */
        || s_get_error(err_exh_high[x])            /* �����¶ȹ��� */
        || s_get_error(err_hp_AI[x])                // ��ѹ����
        || s_get_error(err_gas_in_low[x])           /* �����¶ȹ��� */
        || s_get_error(err_evap_low[x])             /* ������������ */
        || s_get_error(err_gas_in_out_err[x])       /* �����²��쳣 */
        || s_get_error(err_in_low[x])               /* ��ˮ�¶ȹ��� */
        )                   
        {
            mod_err_comp_risk[x] = 1;
        }
    }

    /* ˮ�����㿪�� */
    if (s_get_error(S_ERR_WATER_FLOW))
    {
        fg_water_lack_err = TRUE;
        if (val_err_flux_open_pump && pv_cell_TA(pcb_addr)<20) fg_water_lack_err = FALSE;
    }
}


/*******************************************************************************
 �� ��: err_mod_deal()
 �� ��: ģ����ϴ���
 �� ��: ��
 �� ��: 
 �� ��: ��
*******************************************************************************/
static void err_mod_deal(void)
{
    U08 x;
    
    if (self_commu_err) return;     /* ��ͨѶ����ʱ����ģ����ϴ��� */

    /* ����ͣ������ */
    for (x=0; x<self_comp_num; x++)
    {
        if (mod_err_comp[x])         /* �յ���ѹ�����ϣ�ͣ�յ��� */
        {
            if (Cm[x].run_mode == MODE_COOL || Cm[x].run_mode == MODE_HEAT) 
            {
                s_clr_cm_status(x, CM_NEED);
            }
        }

        if (mod_err_comp_risk[x])
        {
            mod_err_comp_risk[x] = 0;
            /* 2020-10-27���鹵ͨ�����ع���ʱ��ֱ��ͣ��ѹ���� */
            _SETMAX(invt_off_speed_down_cnt[x]);
        }
    }
}

/*******************************************************************************
 �� ��: err_mod_auto_reset()
 �� ��: �Զ���λģ�����
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void err_mod_auto_reset(void)
{
    U08 x = 0;
    /* ̽ͷ���� */

    for (x=0; x<self_comp_num; x++)
    {
        //���������ڸ�λ
        if (!SensorIsError(pv_cell_evi_in(x)))
        {
            s_clr_error(err_evi_in[x]);
        }
        //���������ڸ�λ
        if (!SensorIsError(pv_cell_evi_out(x)))
        {
            s_clr_error(err_evi_out[x]);
        }

        /* ��������ѹ�������Զ���λ */
        if (!TA_FORCE_COMP_OFF)
        {
            s_clr_error(err_TA_limit_comp_on[x]);
        }
    }
 
    
#if 0
    // �����¶ȹ���
    if(pv_cell_antifreez(pcb_addr) > val_antifrez_resettemp)
    {
        s_clr_error(S_ERR_ANTIFREZZ);
    }
#endif

    /* �����ͨ�Ź��� */
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

    /* ��ƵͨѶ���� */
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

        /* ��ֹ���ϵ��󴥷���ʼ�� */
        if (power_time < POWER_ON_INTO_WORK)
        {
            fg_invt_commu_error_bak = fg_invt_commu_error;
        }
        else
        {
            /* ������ƵͨѶ���Ϻ���ִ��һ�γ�ʼ������ */
            if (fg_invt_commu_error_bak != fg_invt_commu_error)		/* ��ƵͨѶ���ϱ�־�����仯 */
            {
                fg_invt_commu_error_bak = fg_invt_commu_error;
                if (fg_invt_commu_error != 0)				/* ���޷������Ϻ�(������δ����ʱ) */
                {
                    invt_init();
                }
            }
        }
    }

    /* ��Ƶˮ�þ��� */
    if (!PUMPF_is_warn())
    {
        s_clr_error(S_ERR_PUMPF_WARN);
    }
}

/*******************************************************************************
 �� ��: err_mod_invt_err_reset()
 �� ��: ���ض˱�Ƶ���ϸ�λ
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void err_mod_invt_err_reset(INVT_ID_ENUM id)
{
    invt_cmd_err_reset(id);
    invt_clr_error_id(id);
    s_clr_error(err_invt_id_to_err_id(id, INVT_ERR_ERROR));
}

/**************************************************************************
* ������ Error_deal_auto_reset()
* ���ܣ������Զ���λ����
* ����������ֵ(��λǰ��ֵ);��ǰ����ֵ;���Ϻ�;���ϻָ�������ϺŻ��ǹ��ϻָ�ǰ����Ϻ�
* ����ֵ��1 - �Զ���λ, 0 - �޲���
* ˵�����趨ʱ���ڳ��ֹ��ϳ���3�ξ���Ҫ�ֶ���λ��
**************************************************************************/
//BOOL err_limit_auto_reset_deal(U08 x, U08 err_old, U08 err_new, BOOL fg_reset_mode)    
BOOL err_limit_auto_reset_deal(U08 x, U08 err_old, U08 err_new)
{   
    U08  y;

    if(val_auto_reset_time1 == 0)       //����Ϊ0����ʾ��ʹ���Զ���λ
    {
        return FALSE;
    }

    /* ����һ��ʱ����Զ���λ */
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
            if (autoERR[x].have_reset)   /* �Ѿ���λ��,�ٳ��ֹ���ʱ���ϴ����� 1 */
            {
                if (++autoERR[x].cnt > 2*MAX_ALLOW_TIMES) autoERR[x].cnt = MAX_ALLOW_TIMES + 1;
                
                for (y=0; y<MAX_ALLOW_TIMES; y++)
                {
                    if (autoERR[x].cnt == MAX_ALLOW_TIMES + 1 + y)
                    {
                        if (autoERR[x].reset_time[y] <= val_auto_reset_time1)   /* ʱ��С���趨ʱ�䣬��Ҫ�ֶ���λ */
                        {
                            autoERR[x].en = 0;     /* ��λ��ֹ�Զ���λ��־�����ֶ���λǰ���ٽ����Զ���λ���� */
                        }
                        else  /* ʱ������趨ʱ�� */
                        {
                               autoERR[x].reset_time[y] = 0;        /* ���ϴ�������3��,���ѳ����趨ʱ�䣬���¼�ʱ */
                        }   
                        break;
                    }
                }
                autoERR[x].have_reset = 0;
            }
            
            if (autoERR[x].en)  /* �����������ʡ����Ϊ����֮ǰ�жԸñ�־�Ĵ��� */
            {               
                if (err_new == 0) /* ���������� */
                {
                    autoERR[x].have_reset = 1;
                    return TRUE;    /* ��λ�ù��� */
                }
            }
        }
    }

    /*�ж��Ƿ��Ѿ��������ι���*/
    if (err_old) 
    {
        if (autoERR[x].en)
        {
            if (autoERR[x].cnt == 2)
            {
                if (autoERR[x].reset_time[0] <= val_auto_reset_time1)   /* ʱ��С���趨ʱ�䣬��Ҫ�ֶ���λ */
                    {
                        autoERR[x].auto_en = FALSE;     /* ��λ��ֹ�Զ���λ��־�����ֶ���λǰ���ٽ����Զ���λ���� */
                    }
            }

            if (autoERR[x].cnt == 3)
            {
                if (autoERR[x].reset_time[1] <= val_auto_reset_time1)   /* ʱ��С���趨ʱ�䣬��Ҫ�ֶ���λ */
                {
                    autoERR[x].auto_en = FALSE;      /* ��λ��ֹ�Զ���λ��־�����ֶ���λǰ���ٽ����Զ���λ���� */
                }  
            }
        
            if (autoERR[x].cnt == 4)
            {
                if (autoERR[x].reset_time[0] <= val_auto_reset_time1)   /* ʱ��С���趨ʱ�䣬��Ҫ�ֶ���λ */
                {
                    autoERR[x].auto_en = FALSE;      /* ��λ��ֹ�Զ���λ��־�����ֶ���λǰ���ٽ����Զ���λ���� */
                }  
            }
        }
    }

    if(autoERR[x].en == FALSE)      autoERR[x].auto_en=FALSE;
    
    return FALSE;
}
/***************************************************************************
* ������ Error_initial_auto_reset_arg()
* ���ܣ���ʼ���Զ���λ��ر���
* ������ĳ�����ϵĹ��Ϻ�
* ����ֵ����
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
 �� ��: err_mod_limit_auto_reset()
 �� ��: �����������Զ���λ����
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void err_mod_limit_auto_reset(void)  
{
    U08 x;
//  U08 fg_err_resume_sysout_high=0;    // ��ˮ���߻ָ���־  
    U08 fg_err_resume=0;                // ���ϻָ���־ ��ͳһ��һ�����ɣ�
    
    
    for (x=0; x<self_comp_num; x++)  
    {
        //ѹ����ѹ�����أ�
        if (err_limit_auto_reset_deal((AUTOERR_LP_DI_1), s_get_error(err_lp[x]), s_get_input(x, IN_LP))) 
        {
            s_clr_error(err_lp[x]);
        }
        //ѹ����ѹ�����أ�
        if (err_limit_auto_reset_deal((AUTOERR_HP_DI_1), s_get_error(err_hp[x]), s_get_input(x, IN_HP))) 
        {
            s_clr_error(err_hp[x]);
        }
        
        //ѹ����ѹ��ѹ����
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

        //ѹ����ѹ��ѹ����
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


        //ѹ�������¶ȹ���
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
    
        /* ��Ƶ�����Ը�λ:���ֹ��ϲ������Ը�λ */
        if (val_invt_err_auto_reset != INVT_RST_NONE)
        {
            BOL fg_err = FALSE;
            U16 invt_err_id = CELL_INVT_ERR(pcb_addr, INVT_ID_1);
            
            if (val_invt_err_auto_reset == INVT_RST_PART
            && (invt_err_id == 11               /* ��Ƶ���ȱ�� */
                || invt_err_id == 12   			/* ������������ */
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

        // ��ˮ�¶ȹ���
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
        
        // ��ˮ�¶ȹ���
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

        /* ���ת���쳣 */
        if (err_limit_auto_reset_deal(AUTOERR_S_PWM_FAN_EXCEPT, s_get_error(err_pwm_fan1[x])||s_get_error(err_pwm_fan2[x]), FALSE))
        {
            s_clr_error(err_pwm_fan1[x]);
            s_clr_error(err_pwm_fan2[x]);
        }
    }

    // �����¶ȹ���
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
    
    /* ������������ */
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

    /* �����¶ȹ��� */
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
    
//    /* ѹ���������� */
//    if (err_limit_auto_reset_deal(AUTOERR_S_CUR_HIGH, s_get_error(err_curr_high[0]), FALSE)) 
//    {
//        s_clr_error(err_curr_high[0]);
//    }
    // ˮ�����ر���������û�ָ�Ҳ��Ϊ���¹���
    if (err_limit_auto_reset_deal(AUTOERR_S_AC_FLUX, s_get_error(S_ERR_WATER_FLOW), FALSE)) 
    {
        s_clr_error(S_ERR_WATER_FLOW);
    }


    //��ˮ�¶ȹ���
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
    
    //��ˮ�¶ȹ���
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
������s_Error_flag_init
���ܣ���ʼ�����������ϱ�־
������ģ�����
      ģ�����ΪMAX_PCBʱΪ�������ģ��
����ֵ����
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
������Error_reset
���ܣ�������ϸ�λ
��������
����ֵ����
******************************************************/
/* ����Ϊ����������Ӧ���϶�Ӧ��,err_order_modΪģ���; err_order_sysΪ��չ�� */
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
 �� ��: err_mod_manual_reset()
 �� ��: �ֶ���λģ�����
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void err_mod_manual_reset(void)
{
    U08 x;

    // ����������,����������ſɸ�λ
    {
        if(!(s_get_input(0, IN_LP)))                        s_clr_error(err_lp[0]);
        if(!(s_get_input(0, IN_HP)))                        s_clr_error(err_hp[0]);

        s_clr_error(S_ERR_IBH_OVERLOAD); 
    
        /* ˮ������ֱ�Ӹ�λ����Ϊˮ�ò������������ˮ�������޷��ָ�������λ�� */
        s_clr_error(S_ERR_WATER_FLOW);
    }

    /* ��Ƶˮ�ù��ϸ�λ */
    if (!s_get_output(MAX_COMP, S_DO_PUMPf)
    || PUMPF_can_reset_err()
    )
    {
        s_clr_error(S_ERR_PUMPF);
    }

    /* �������ģ����ϸ�λ */
    if (gIO.phase != PHASE_STA_ERR)    /* ���ౣ����λ */
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
//      s_clr_error(err_curr_high[x]);           /* ѹ���������� */
//      s_clr_error(err_curr_low[x]);            /* ѹ��������С */ 

        s_clr_error(err_pwm_fan1[x]);
        s_clr_error(err_pwm_fan2[x]);

        //̽ͷ��λ
        if (!SensorIsError(pv_cell_wing(x)))              s_clr_error(err_wing[x]);
        if (!SensorIsError(pv_gas_out_high(x)))           s_clr_error(err_gas_out[x]);
        if (!SensorIsError(pv_cell_gas_in(x)))            s_clr_error(err_gas_in[x]);
        if (!(SensorIsError(pv_cell_evap(x))&&SensorCannotUse(pv_cell_TRFliq(x))))              s_clr_error(err_evap_in[x]);
        if (!SensorIsError(GET_EXIO_PRS(0, EXIO_AI_01)))  s_clr_error(err_j5_prs_sensor[x]);   /* J5ѹ������ */
        if (!SensorIsError(GET_EXIO_PRS(0, EXIO_AI_02)))  s_clr_error(err_j6_prs_sensor[x]);   /* J6ѹ������ */

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

        /* ������˪Ƶ�� */
        defrost_fg[x].emergency_defrost_frequent = FALSE;
        s_clr_error(err_emerg_defrost[x]);

        /* ���¹��͡����߸�λ */
        if(pv_cell_TWin1(pcb_addr) < val_err_ac_out_high-val_err_wt_exit-TEMP_IN_DIFF)  s_clr_error(err_in_high[x]);
        if(pv_cell_TWin1(pcb_addr) > val_err_ac_out_low+val_err_wt_exit+TEMP_IN_DIFF)   s_clr_error(err_in_low[x]); 

        /* ���¹�������ѹ��������λ */
        if (!TA_FORCE_COMP_OFF) s_clr_error(err_TA_limit_comp_on[x]);
    }
    //��Ԫ���¹��ϸ�λ
    if (!SensorIsError(pv_cell_TWout1(pcb_addr)))
    {
        s_clr_error(S_ERR_TEMP_TWOUT1);
    }
    if(pv_cell_TWout1(pcb_addr) < val_err_ac_out_high - val_err_wt_exit)    s_clr_error(S_ERR_OUT_HIGH);
    if(pv_cell_TWout1(pcb_addr) > val_err_ac_out_low  + val_err_wt_exit)    s_clr_error(S_ERR_OUT_LOW);      

        
         
    //��Ԫ ���¹��ϸ�λ
    if (!SensorIsError(pv_cell_TWin1(pcb_addr)))
    {
        s_clr_error(S_ERR_TEMP_TWIN1);
    }

    if (!fg_out_in_diff_high)
    {
        s_clr_error(S_ERR_OUT_IN_DIFF);
    }
    s_clr_error(S_ERR_OUT_IN_ERR);

    /* ��Ƶ�����ϸ�λ */
    invt_err_reset();
#if 0
    {
        INVT_ID_ENUM id;
        for (id=INVT_FAN1; id<INVT_ID_MAX; id++)            /* ��Ƶ��� */
        {
            invt_cmd_err_reset(id);
        }

        err_mod_invt_err_reset(INVT_ID_1);          /* ��Ƶѹ�� */

//        m_clr_error(MAX_PCB, ERR_INVT_MODEL);

        fg_invt_commu_error = 0;
    }
#endif

    /* �����ͨ�Ź��� */
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
    ����: �����ܽӿ�
/==========================================*/
void err_mod(void)
{
//  U08  i;

    err_mod_auto_reset();           //����ģ������Զ���λ
        
    err_mod_limit_auto_reset();     //����ģ������������Զ���λ(Ҫ�ŵ���ѯ���ϵ�ǰ��) 

    if (fg_err_reset)   /* �ֶ���λ*/
    {
        fg_err_reset = FALSE;
        

        err_mod_manual_reset();     //���ֹ����ֶ���λ
        err_limit_auto_reset_init();
    }
        
    
    if (power_time < POWER_ON_INTO_WORK) return;    //�ϵ�ǰ10�벻������  
    
    err_mod_judge();                //���ģ�����
    
    err_mod_flag_init();        
    err_mod_class();                //ģ����Ϲ���
    err_mod_deal();                 //����ģ�����  


}

/*****************************************************
    ���ܣ�д�ۼ�����ʱ�䵽EEPROM
******************************************************/
//void runtime_write_to_eeprom(U08 x)
//{
////    U16 addr;
//  
//  pCell->cell_runtime[x]++;   /* ����ʱ���ۼ� */
//  
////    addr = MEM_RUNTIME_ADDR ;
////    eprom_wr_near_crc((U08 *)&pCell->cell_runtime[0], addr, MAX_RUNTIME*2);
//
//    DataAccess_CompRuntime(DATA_WRITE);
//}

/*****************************************************
    ���ܣ�ģ����ּ�ʱ��
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
        // �Զ���λ
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

            if (invt_info_freq_limit_id(INVT_ID_1))         /* ���������Ƶ��ʱ����� */
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
        
        
        /* ���ϼ��ʱ�� */
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
        
        /* ���� */
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

            /* ������м�ʱ */
            _CNT_WHEN(cnt_PUMPf_interval_on_off, (cnt_PUMPf_interval_on_off<PUMPF_INTERVAL_TIME));
        }

        /* �����¶�����ѹ������������ʱ */
        for (x = 0; x < SUB_SYS_TYPE_MAX; x++)
        {
            for (y = 0; y < CTRL_MODE_MAX; y++)
            {
                _CNT(cnt_mod_comp_limit_TA_dly[x][y]);
            }
        }

        /* IBH2������ʱ */
        _CNT_WHEN(IBH2_add_dly, s_get_output(MAX_COMP, S_DO_IBH1));

		/* ���������ȴ���ʱ */
		_CNT(cnt_HT_on_dly);

        for (x = 0; x< self_comp_num; x++)
        {
            if (current_err_time[x]             != 0xffff)  current_err_time[x]++;
            if (current_high_time[x]            != 0xffff)  current_high_time[x]++;      
            if (cm_time[x].cnt_time_fan_delay   != 0xffff)  cm_time[x].cnt_time_fan_delay++;

            if (cm_time[x].defrost_time != 0xffff)  cm_time[x].defrost_time++;
            if (!s_get_cm_status(x, CM_DEFROST))    cm_time[x].defrost_time = 0;
                
            if (cm_time[x].frost_out_dly != 0xff)   cm_time[x].frost_out_dly++;
            
            if(val_defrost_time_sel == 0) /* ��˪�ۻ�����ʱ�� */
            {
                wing_temp_confirm[x]++;         /* ����60S��ſ�ʼ����Ƭ�¶� */
                if ((cm_time[x].on <= 60) || (pv_cell_wing(x) <= 0)) wing_temp_confirm[x] = 0;
                if (wing_temp_confirm[x] > 30) cm_time[x].defrost_space = 0;        /* ��Ƭ�¶ȳ���30S����0ʱ��˪�ۼ�ʱ������ */
            }
           
            /* �¶����ݼ�¼ */
            {
                /* �����¶� */
                _CNT(mod_rec_gas_out_cnt[x]);
                if (mod_rec_gas_out_cnt[x] >= REC_NORMAL_TEMP_INTV)
                {           
                    mod_rec_gas_out_cnt[x] = 0;
                    mod_rec_gas_out_avg[x] = fun_rec_avg(pv_gas_out_high(x), &mod_rec_gas_out[x][0], 
                                                ARRELE(mod_rec_gas_out[x]));
                }
            }
        }

        /* ѹ�����ʱ�� */
        for (x = 0; x< self_comp_num; x++)
        {
            if (Cm[x].dly != 0xffff)            Cm[x].dly++;
            if (Cm[x].defrost_dly != 0xffff)    Cm[x].defrost_dly++;

//          if(defrost_time_count[x]!=0xffff)       defrost_time_count[x]++;     
            
            if (s_get_cm_status(x, CM_ON)||s_get_cm_status(x, CM_DEFROST)) // ��˪���̻��ʱ���ѹ����Ϊ���ּ�ʱ����������CM_DEFROST����  
//          if (s_get_output(x, SW_CM)) 
            {
#ifndef DEBUG   //DEBUG-CXW
                if (++cm_time[x].run > 3600) 
                {
                    cm_time[x].run = 0;

                    pCell->cell_runtime[x]++;   /* ����ʱ���ۼ� */
                    DataAccess_CompRuntime(DATA_WRITE);
                }
#else
                if (++cm_time[x].run > 360) // ���ԣ�6���Ӵ���1Сʱ
                {
                    cm_time[x].run = 0;
                    pCell->cell_runtime[x]++;   /* ����ʱ���ۼ� */
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
            
             
            if (s_get_output(x, SW_CM)) // �Լ̵������Ϊ׼����˪ʱ����ѹ�������㲻��ʱ
            {
                if (cm_time[x].sw_on != 0xffff) cm_time[x].sw_on++;
            }
            else
            {
                cm_time[x].sw_on=0;
            }

            _CNT_WHEN(cnt_reoil_end[x], !log_cmp_is_reoil(x));

        }
        /* ��ͬ�˳���˪ʱ�� */
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
       /* �����˳���˪ʱ�� */
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
        /* ��ͨ�����ʱ�� */
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
        
         
        if (pv_environment_stable_dly != 0xffff) pv_environment_stable_dly++;   // ��������ʱ��

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
/*----------------------�ּ�ʱ----------------------------------*/  
    if (F_TIME_BASIC_MIN_BACK)
    {
         for( x=0; x<self_comp_num; x++)
        {
            /* ���ڳ�˪ */
           if(val_defrost_time_sel == 0)
           {
                /* ���ڳ�˪ */
                if (s_get_cm_status(x, CM_DEFROST)) 
                {
                    cm_time[x].defrost_space = 0;  /* ��˪��� */
                }
                else if (cm_time[x].on > 60 && (pv_cell_wing(x) <= 0))  /*����ʱ�ż�ʱ */
                {
                    if (cm_time[x].defrost_space != 0xffff)  cm_time[x].defrost_space++;
                }
           }
           else if(val_defrost_time_sel == 1)
           {
                /* ���ڳ�˪ */
                if (s_get_cm_status(x, CM_DEFROST)) 
                {
                    cm_time[x].defrost_space = 0;  /* ��˪��� */
                }
                else if (s_get_cm_status(x, CM_ON))  /*����ʱ�ͼ�ʱ */
                {
                    if (cm_time[x].defrost_space != 0xffff)  cm_time[x].defrost_space++;
                }
           }
        }

        
    }
}
