
#define LOG_MOD_GLOBAL

#include "includes.h"

/* ========================================================================================== */

#define ID_CELL     MAX_COMP

#define VALVE_READY         0       /* ���ͷ�׼���� */
#define VALVE_BUSY          1       /* ���ͷ�æ */

/* ========================================================================================== */
#ifndef USE_QUICK_TIME
#define val_cool_fan_on_dly     10      // ����������ʱ
#define val_cool_fan_off_dly    -10     // ����������ʱ   
#define val_heat_fan_on_dly     10      // ���ȷ������ʱ
#define val_heat_fan_off_dly    -10     // ���ȷ������ʱ 
#else
#define val_cool_fan_on_dly     (fg_dbg_slow?10:2)       // ����������ʱ
#define val_cool_fan_off_dly    (fg_dbg_slow?-10:-2)     // ����������ʱ    
#define val_heat_fan_on_dly     (fg_dbg_slow?10:2)       // ���ȷ������ʱ
#define val_heat_fan_off_dly    (fg_dbg_slow?-10:-2)     // ���ȷ������ʱ 
#endif

    
#define byway_val_temp_diff     2*10    // ����ͨ�����¶�ƫ��

//U08 fg_defrost_apply[MAX_COMP];             /* �����˪��־ */

//U16 frost_space[MAX_COMP];        /* ��˪�����ʱ */
//U16 frost_long[MAX_COMP];     /* ���˪�����ʱ */
//U16 cnt_time_fan_delay[MAX_COMP];             /* ������˪ǰ�������ʱ */
//U16 frost_time[MAX_COMP];     /* ��˪ʱ���ʱ */
//U08 frost_out_dly[MAX_COMP];                  /* ������˪�ĳ�ˮ�¶ȼ�ʱ*/

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

static BOOL fg_para_init;           /* ������ʼ����־ */
static BOOL fg_limit_init;          /* ϵͳά��������ʼ����־ */

static BOOL fg_invt_init;           /* ��Ƶ����ʼ����־ */

static BOOL fg_runtime_init;        /* ������ʼ����־ */
static BOOL fg_systime_init;        /* ϵͳ����ʱ���ʼ����־ */
static BOOL fg_systime_save;        /* ϵͳ����ʱ�䱣���־ */
static BOOL fg_password_init;       // �����ʼ����־

static U08  fg_comp_valve_reset[MAX_COMP];            // ѹ����Ӧ�����ͷ����ڽ��и�λ���� (��ֹEEV��û������ɣ�ѹ���Ϳ���) 
static BOOL fg_vavle_reset[EEV_ID_MAX]= {TRUE,TRUE} ; // �������ͷ������ʼ�����Ʊ�־(ͣ��һ��ֻ��ʼ��һ��)

//static void s_Logic_StatusIndicate(void);
//U08 sensor_used_temp_out(U08 comp_num);
//U08 sensor_used_temp_cond_out(U08 comp_num);


/* IBH�������� */
enum                    
{
    IBH_ON_SIG_RUN_HEAT_COMP_NORMAL        = 0,             /* �������У�ѹ������״̬ʱ���� */
    IBH_ON_SIG_RUN_HEAT_COMP_KEEP_NORMAL   = 1,             /* �������У�ѹ������״̬����ʱ���� */
    IBH_ON_SIG_RUN_HEAT_COMP_ABNORMAL      = 2,             /* �������У�ѹ���쳣״̬ʱ���� */
    IBH_ON_SIG_RUN_HEAT_COMP_KEEP_ABNORMAL = 3,             /* �������У�ѹ���쳣״̬����ʱ���� */
    IBH_ON_SIG_RUN_HOTW_COMP_NORMAL        = 4,             /* ����ˮ���У�ѹ������״̬ʱ���� */
    IBH_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL   = 5,             /* ����ˮ���У�ѹ������״̬����ʱ���� */
    IBH_ON_SIG_RUN_HOTW_COMP_ABNORMAL      = 6,             /* ����ˮ���У�ѹ���쳣״̬ʱ���� */
    IBH_ON_SIG_RUN_HOTW_COMP_KEEP_ABNORMAL = 7,             /* ����ˮ���У�ѹ���쳣״̬����ʱ���� */
    IBH_ON_SIG_IDLE_STA_AC_ANTIFREEZE      = 8,             /* ����̬�յ�ˮ·����ʱ���� */
    IBH_ON_SIG_IDLE_STA_HOTW_ANTIFREEZE    = 9,             /* ����̬��ˮˮ·����ʱ���� */
    IBH_ON_SIG_RUN_STA_AC_ANTIFREEZE       = 10,            /* ����̬�յ�ˮ·����ʱ���� */
    IBH_ON_SIG_RUN_STA_HOTW_ANTIFREEZE     = 11,            /* ����̬��ˮˮ·����ʱ���� */
    IBH_ON_SIG_RUN_HEAT_DEFROST            = 12,            /* ���ȳ�˪ʱ���� */
    IBH_ON_SIG_RUN_HOTW_DEFROST            = 13,            /* ����ˮ��˪ʱ���� */
    IBH_ON_SIG_FORCE_HOTW                  = 14,            /* ǿ����ˮģʽʱ���� */
    IBH_ON_SIG_RUN_HEAT_FORCE_ON           = 15,            /* �������У�ǿ����������ʱ���� */
    IBH_ON_SIG_RUN_HOTW_FORCE_ON           = 16,            /* ����ˮ���У�ǿ����������ʱ���� */
    IBH_ON_SIG_DISINFECT                   = 17,            /* ��ʱɱ��ʱ���� */
    IBH_ON_SIG_TF                          = 18,            /* �̼�ʱ���� */
    IBH_ON_SIG_TEST_RUN_HEAT_MODE_ON       = 19,            /* ���������У��������ȿ��� */
    IBH_ON_SIG_MAX
};
static U16 fg_IBH_on[_DIVUCARRY(IBH_ON_SIG_MAX, 16)];



/* ========================================================================================== */

/* ����������ģ��������أ���ͬ���͵�ģ���밴Ҫ��ֱ���д */
/*-----------------------ģ�鶨�ƿ�ʼ------------------------------------*/
/*****************************************************
������ s_comp_can_use
���ܣ� �ж�ѹ���ڸ�ģʽ�µ�ʹ��״̬
������ ѹ����
����ֵ�� ѹ��ʹ��״̬(ģʽ��Ӧ)
        ˵���� ����ֵ��ÿһλ��ʾһ��ģʽ�Ŀ�ʹ��״̬��ģʽ��Ӧ˳�����±�
        ------------------------------------------------------------------
        bit0| bit1| bit2|          bit3|        bit4|    bit5| bit6| bit7
        ����| ����| ��ˮ| ����+������ˮ| ����+ȫ��ˮ| ��ˮ/��| ��ʪ| ��ʪ
        ------------------------------------------------------------------
        1: ����
        0: ������
******************************************************/
U08 s_comp_can_use(U08 comp_num)
{
    U08 comp_can_use = 0xff;     /* ��ʼ��Ϊ����ģʽ���� */
                    
    if (mod_err_comp[comp_num])       /* ѹ������(�յ�ģʽ���ɿ�) */
    {
        comp_can_use &= 0xfc;
    }

    comp_can_use &= 0x03;                 /* ���θ�ģ�鲻���ڵ�ģʽ */

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
 �� ��: log_mod_cell_have_mode()
 �� ��: �ж�ģ���Ƿ��Ƿ���ָ��������ģʽ
 �� ��: mode--ģʽ; 
 �� ��: TRUE/FALSE--��ָ��ģʽ/��ָ��ģʽ
 �� ��: ��
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
������Logic_eev_sv_cool
���ܣ�����ʱ�������ȶ�Ŀ��ֵ����
������ѹ�����
����ֵ�����ȶ�Ŀ��ֵ
**********************************************/
I16 Logic_eev_sv_cool(I16 eva_side_temp)
{
    I16 pid_sv;
    
    if (SensorCannotUse(eva_side_temp))    /* ������̽ͷ���ϣ�ȡ�̶����ȶ�Ŀ��ֵ */
    {
        pid_sv = val_motor_sv_cool1;            /* ��ʼ�� */
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
������Logic_eev_sv_heat
���ܣ�����ʱ�������ȶ�Ŀ��ֵ����
������ѹ�����
����ֵ�����ȶ�Ŀ��ֵ
**********************************************/
I16 Logic_eev_sv_heat(I16 eva_side_temp)
{
    I16 pid_sv;
    
    if (SensorCannotUse(eva_side_temp))   /* ������̽ͷ���ϣ�ȡ�̶����ȶ�Ŀ��ֵ */
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
 �� ��: log_mod_get_temp_cond()
 �� ��: ��ȡ�����¶�
 �� ��: x--; 
 �� ��: 
 �� ��: ��
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
������Logic_eev_step_min
���ܣ���ȡ������С����
������
����ֵ����С����%
**********************************************/
I16 Logic_eev_step_min(U08 x)
{
    I16 result;

    I16 temp_cond = log_mod_get_temp_cond(x);
    I16 temp_exh = pv_gas_out_high(x);
    I16 exh_sh = temp_exh - temp_cond;

    static U08 area = AREA_00;

    if (CmIsCool(x))        /* ���� */
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
    else                    /* ���� */
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
 �� ��: log_mod_comp_gb_freq_heat()
 �� ��: ������ԣ���ȡ��ǰ�����¶���ѹ����������Ƶ��
 �� ��: env_temp--�����¶ȼ������¶�; 
 �� ��: ����Ƶ��
 �� ��: ��
*******************************************************************************/
I16 log_mod_comp_gb_freq_heat(I16 env_temp)
{
    I16 result;
    static U08 area;
    
    if (SensorCannotUse(env_temp))    /* ������̽ͷ���ϣ�ȡ�̶����ȶ�Ŀ��ֵ */
    {
        result = val_invt_rps_heatrun_freq1;            /* ��ʼ�� */
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
 �� ��: log_mod_comp_gb_freq_cool()
 �� ��: ������ԣ���ȡ��ǰ�����¶���ѹ����������Ƶ��
 �� ��: env_temp--�����¶ȼ������¶�; 
 �� ��: ����Ƶ��
 �� ��: ��
*******************************************************************************/
I16 log_mod_comp_gb_freq_cool(I16 env_temp)
{
    I16 result;
    static U08 area;
    
    if (SensorCannotUse(env_temp))    /* ������̽ͷ���ϣ�ȡ�̶����ȶ�Ŀ��ֵ */
    {
        result = val_invt_rps_coolrun_freq1;            /* ��ʼ�� */
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
 �� ��: log_mod_comp_gb_sv_heat()
 �� ��: ������ԣ���ȡ��ǰ�����¶���ѹ�����ȹ��ȶ�
 �� ��: env_temp--�����¶ȼ������¶�; 
 �� ��: ����Ƶ��
 �� ��: ��
*******************************************************************************/
I16 log_mod_comp_gb_sv_heat(I16 env_temp)
{
    I16 result;
    static U08 area;
    
    if (SensorCannotUse(env_temp))    /* ������̽ͷ���ϣ�ȡ�̶����ȶ�Ŀ��ֵ */
    {
        result = val_debug_sh_sv1;          /* ��ʼ�� */
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
 �� ��: log_mod_comp_gb_sv_cool()
 �� ��: ������ԣ���ȡ��ǰ�����¶���ѹ��������ȶ�
 �� ��: env_temp--�����¶ȼ������¶�; 
 �� ��: ����Ƶ��
 �� ��: ��
*******************************************************************************/
I16 log_mod_comp_gb_sv_cool(I16 env_temp)
{
    I16 result;
    static U08 area;
    
    if (SensorCannotUse(env_temp))    /* ������̽ͷ���ϣ�ȡ�̶����ȶ�Ŀ��ֵ */
    {
        result = val_debug_sc_sv1;          /* ��ʼ�� */
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
 �� ��: calculate_gb_dbg_freq()
 �� ��: ���㵱ǰ����ģʽ��Ƶ��ֵ
 �� ��: comp_num--; 
        Mode--; 
        NowFreNeed--; 
 �� ��: 
 �� ��: ��
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
 �� ��: calculate_gb_dbg_sv()
 �� ��: ���㵱ǰ����ģʽ�¹��ȶ�
 �� ��: Mode--; 
        NowSNeed--; 
 �� ��: 
 �� ��: ��
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
 �� ��: log_mod_init()
 �� ��: ģ�������ʼ��
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_mod_init(void)
{
    U08  x;
    
    pCell = &cell[pcb_addr];
    
    fg_err_reset = TRUE;
    
    for (x=0; x<MAX_COMP; x++)
    {
        cm_time[x].off = 0;     /* ͣ��ʱ���ʼ�� */
        cm_time[x].defrost_exit = 0xffff;       /* ��ͬ�˳���˪ʱ���ʼ�� */
        cm_time[x].defrost_exit_sin = 0xffff;       /* �����˳���˪ʱ���ʼ�� */
        cm_time[x].run = 0;     /* ����ʱ���ʼ�� */
//      valve_time[x].off = 0xffff; 
//      valve_time[x].on = 0;
        fg_defrost_apply[x] = 0;
        cm_time[x].cnt_time_fan_delay = 0xffff;
        cm_time[x].defrost_space = 0;       /* ��˪�����ʼ�� */

        fg_comp_valve_reset[x] = VALVE_BUSY;        /* ��ʼ��Ϊ���ͷ�æ */

        ARRSET(mod_rec_gas_out[x], SENSOR_NOEXIST);

        _SETMAX(cnt_reoil_end[x]);
    }
    
    
    for (x=0; x<MAX_COMP+1; x++)
    {
        Cm[x].status = 0;
        
        Cm[x].run_mode = MODE_NULL;         /* ����ģʽ��ʼ�� */
        Cm[x].run_mode_need = 0xff;         /* ����ģʽ�����ʼ�� */
            
    }

    fg_cell_pump_open = FALSE;
    fg_force_IBH_off = FALSE;

    PUMPF_init();

//    runtime_read_from_eeprom();    /* ��ȡѹ������ʱ�� */
    DataAccess_CompRuntime(DATA_READ);/* ��ȡѹ������ʱ�� */
}

/*******************************************************************************
 �� ��: log_mod_pump_is_run()
 �� ��: ģ����ȶ����б�־
 �� ��: ��
 �� ��: 
 �� ��: ��
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
    ������s_cell_is_err
    ���ܣ��ж�ģ���Ƿ����
    ��������
    ����ֵ��TRUE: ģ�����; FALSE: ģ��δ����
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
    ������s_cell_have_mode
    ���ܣ��ж�ģ���Ƿ��Ƿ���ָ��������ģʽ
    ������ָ��ģʽ
    ����ֵ��TRUE: ����ָ��ģʽ; FALSE: ��ָ��ģʽ
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
 �� ��: log_mod_get_comp_limit_out_up()
 �� ��: ��ȡ��������ֵ
 �� ��: area--���·ֶ�����; 
 �� ��: 
 �� ��: ��
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
 �� ��: log_mod_get_comp_limit_out_up_UT()
 �� ��: ��Ԫ���ԣ�log_mod_get_comp_limit_out_up
 �� ��: ��
 �� ��: ��
 �� ��: ��
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
 �� ��: log_mod_comp_temp_out_limit()
 �� ��: ��������
 �� ��: x--ѹ�����; 
 �� ��: TRUE/FALSE--��/��
 �� ��: 
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
������ log_mod_comp_can_open
���ܣ� ѹ���Ƿ���Կ�
������ ѹ����
����ֵ�� TRUE: ѹ���ɿ�; FALSE: ѹ�����ɿ�      
******************************************************/
BOOL log_mod_comp_can_open(U08 comp_num)
{
    BOOL fg_comp_can_open = FALSE; 
    U16 cm_off_time = val_cm_off_time+CM_off_speed_down_time;

    /* ��Ƶ���������⴦�� */
    if (!TF_IS_IDLE			/* �̼� */
    ||  fg_refri_recy_act	/* �շ� */
    )
    {
        cm_off_time = 0;
    }

    if (log_mod_pump_is_run())				        /* ��Ƶˮ���ȶ����� */
    if (cm_time[comp_num].off > cm_off_time)        /* ��Ƶ������ʱ�䵽 */
    if (!s_get_cm_status(comp_num, CM_ON))          /* ѹ��δ���� */
    if (!mod_err_comp[comp_num] && fg_comp_valve_reset[comp_num] == VALVE_READY) //NOTE-CXW ����2017-5-12
    if (GET_EXIO_HDW_STATUS_READY(0))
    if (!log_mod_comp_temp_out_limit(comp_num))
    if (TA_ALLOW_COMP_ON_OFF)
    {
        fg_comp_can_open = TRUE;
    }

    return fg_comp_can_open;
}

/*****************************************************
������ log_mod_comp_can_close
���ܣ� ѹ���Ƿ���Թ�(ָ����ж��)
������ ѹ����
����ֵ�� TRUE: ѹ���ɹ�; FALSE: ѹ�����ɹ�
        
******************************************************/
BOOL log_mod_comp_can_close(U08 comp_num)
{
    BOOL fg_comp_can_close = FALSE;  
    
    if (cm_time[comp_num].on > COMP_NEED_ON_TIME)                 /* ѹ����С����ʱ�䵽 */
    if (s_get_cm_status(comp_num, CM_NEED))                       /* ѹ���ѿ��� */
    if (!s_get_cm_status(comp_num, CM_DEFROST))                   /* ѹ��δ��˪ */
    {
        fg_comp_can_close = TRUE;
    }
    
    return fg_comp_can_close;
}

/*****************************************************
������ log_mod_comp_can_quick_stop
���ܣ� ѹ���Ƿ���Լ�ͣ
������ ѹ����
����ֵ�� TRUE: ѹ���ɼ�ͣ; FALSE: ѹ�����ɼ�ͣ
        
******************************************************/
BOOL log_mod_comp_can_quick_stop(U08 comp_num)
{
    BOOL fg_comp_can_quick_stop = FALSE;  

    if (s_get_cm_status(comp_num, CM_NEED))                       /* ѹ���ѿ��� */
    {
        fg_comp_can_quick_stop = TRUE;
    }
    
    return fg_comp_can_quick_stop;
}
/***************************************************
    ������s_comp_is_run
    ���ܣ��ж��Ƿ���ѹ������
    ����������ģʽ(��ʾѰ���Ժ���ģʽ���е�ѹ��)
    ����ֵ��TRUE: ��ѹ������; FALSE: ��ѹ������
****************************************************/
BOOL log_mod_comp_is_run(U08 run_type)
{
    U08 i ;
    BOOL fg_have_comp_run = FALSE;

    for (i=0; i<self_comp_num; i++)
    {
        if (s_get_cm_status(i, CM_NEED))
        {
            if (run_type == MODE_NULL               /* ������ģʽ */
                || run_type == Cm[i].run_mode)      /* ģʽƥ�� */
            {
                fg_have_comp_run = TRUE;
                break;
            }
        }
    }

    return fg_have_comp_run;
}
/*****************************************
    ������ log_mod_cell_is_run
    ���ܣ� �ж�ģ���Ƿ�������
    ������ ����ģʽ(��ʾѰ���Ժ���ģʽ���е�ѹ��)
    ����ֵ�� TRUE: ģ������; FALSE: ģ��δ����
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
/*-------------------------ģ�鶨�ƽ���-----------------------------------*/

/********************************************************
������s_comp_add_sub_prio
���ܣ��жϵ�ǰѹ���Ƿ���м�/ж�����ȼ�
������ѹ�����
����ֵ��TRUE: �����ȼ�; FALSE: �����ȼ�
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
            if (s_get_output(i, SW_CM) || s_get_cm_status(i, CM_NEED))  /* ͬ����ѹ�����У��������� */
            {
                fg_have_prio = TRUE;
                break;
            }
        }
        else if (prio_type == _SUBB)
        {
            if (!(s_get_output(i, SW_CM) || s_get_cm_status(i, CM_NEED)))   /* ͬ����ѹ��δ���У�ж������ */
            {
                fg_have_prio = TRUE;
                break;
            }
        }   
    }
            
    
    return fg_have_prio;
}

/*******************************************************************************
 �� ��: log_mod_get_rfg_temp_corr()
 �� ��: ��ý�¶������߼�
 �� ��: ��
 �� ��: �¶�����ֵ
 �� ��: ��
*******************************************************************************/
I16 log_mod_get_rfg_temp_corr(void)
{
    static BOOL fg_first_corr = TRUE;
    static U08 t2b_area = AREA_MAX;
    static U08 t2_area = AREA_MAX;

    U08 cm = 0;

    /* ��ʹ����ý�¶����� */
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
            
            /* ��ý�����¶ȷֶ� */
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
         
            /* ��ýҺ���¶ȷֶ� */
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
��  �� : log_mod_get_freq_max()
��  �� : ��ȡƵ������
��  �� : I16 temp_env : ����
         I16 temp_in : ����
��  �� : ѹ��Ƶ������
��  �� : Ƶ�������ܵ��ýӿڣ��������ִ���
*****************************************************************************/
I16 log_mod_get_freq_max(U08 mode, I16 temp_env, I16 temp_in)
{
    I16 freq_up = wf_get_freq_max_by_env_in(mode, temp_env, temp_in);   /* ����Ƶ�ʱ� */

    /* ����ģʽ */
    {
        #define val_invt_rps_up_min (100*55)    /* ��С����ת�� */
        I16 freq_adj = ModeIsEco    
                       ? val_invt_rps_adj_eco
                       : ModeIsStr
                            ? val_invt_rps_adj_strong
                            : val_invt_rps_adj_std;

        /* ���ȵ���ϵ�� */
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

    /* ��ý���⡢Һ���¶����� */
    {
        freq_up += log_mod_get_rfg_temp_corr();
    }

    /* ����ģʽ���٣�2022-1-21-���Ż���Ȩ��Ӧ���ڳ�˪�����͵ȣ��ݷŵ��˴�ִ�� */
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

    /* ��Ƶ��Ƶ���޶� */
    freq_up = MIN(freq_up, val_invt_rps_max);

    return freq_up;
}

/*****************************************************************************
��  �� : log_mod_get_freq_min()
��  �� : ��ȡƵ������
��  �� : I16 temp_env : ����
         I16 temp_in : ����
��  �� : ѹ��Ƶ������
��  �� : Ƶ�������ܵ��ýӿڣ��������ִ���
*****************************************************************************/
I16 log_mod_get_freq_min(U08 mode, I16 temp_env, I16 temp_in)
{
    I16 freq_dn = wf_get_freq_min_by_env_in(mode, temp_env, temp_in);   /* ����Ƶ�ʱ� */

    /* ��Ƶ��Ƶ���޶� */
    freq_dn = MAX(freq_dn, val_invt_rps_min);

    return freq_dn;
}

/*******************************************************************************
 �� ��: log_mod_comp_get_temp_cond()
 �� ��: ѹ����ȡ�����¶ȣ������޽�Ƶ
 �� ��: x--ѹ����; 
 �� ��: 
 �� ��: ��
*******************************************************************************/
U16 log_mod_comp_get_temp_cond(U08 x)
{
    I16 temp_cond = SENSOR_NOEXIST;

#ifdef USE_FREQ_LIMIT_TC_TE
    if (CmIsHeat(x))
    {
        /* �����¶ȣ����������¶� �� �廻��ˮ�¶ȣ���ʹ�ø�ѹ������ʱ�� */
        temp_cond = fun_pv_get(pv_cell_cond_temp(x), pv_cell_TWout1(pcb_addr));
    }

    if (CmIsCool(x))
    {
        /* �����¶ȣ����������¶� �� ��Ƭ�¶ȣ���ʹ�õ�ѹ������ʱ�� */
        temp_cond = fun_pv_get(pv_cell_cond_temp(x), pv_cell_wing(x));              
    }
#else
    /* ��Ӱ���ѳ�����Ʒ�����þ��߼� */
    if (CmIsHeat(x))
    {
        temp_cond = pv_cell_TWout1(pcb_addr);    /* �����¶ȣ��̶�Ϊ�廻��ˮ�¶� */
    }

    if (CmIsCool(x))
    {
        temp_cond = pv_cell_wing(x);             /* �����¶ȣ��̶�Ϊ��Ƭ�¶� */           
    }
#endif

    return temp_cond;
}

/*******************************************************************************
 �� ��: log_mod_comp_get_temp_evap()
 �� ��: ѹ����ȡ�����¶ȣ������޽�Ƶ
 �� ��: x--ѹ����; 
 �� ��: 
 �� ��: ��
*******************************************************************************/
U16 log_mod_comp_get_temp_evap(U08 x)
{
    I16 temp_evap = SENSOR_NOEXIST;
    
#ifdef USE_FREQ_LIMIT_TC_TE
    if (CmIsHeat(x))
    {
        /* �����¶ȣ����������¶� �� ��Ƭ�¶ȣ���ʹ�õ�ѹ������ʱ�� */
        temp_evap = fun_pv_get(pv_cell_evap_temp(x), pv_cell_wing(x));
    }

    if (CmIsCool(x))
    {
        /* �����¶ȣ����������¶� �� �����¶ȣ���ʹ�õ�ѹ������ʱ�� */
        temp_evap = fun_pv_get(pv_cell_evap_temp(x), pv_cell_evap(x));              
    }
#else
    /* ��Ӱ���ѳ�����Ʒ�����þ��߼� */
    if (CmIsHeat(x))
    {      
        temp_evap = pv_cell_wing(x);    /* �����¶ȣ��̶�Ϊ��Ƭ�¶� */
    }

    if (CmIsCool(x))
    {
        /* �����¶ȣ����������¶� �� �����¶ȣ���ʹ�õ�ѹ������ʱ�� */
        temp_evap = fun_pv_get(pv_cell_evap_temp(x), pv_cell_evap(x));
    }
#endif

    return temp_evap;
}

/*******************************************************************************
 �� ��: log_mod_comp_can_add()
 �� ��: ѹ������Ƶ
 �� ��: comp_num--; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL log_mod_comp_can_add(U08 cm)
{
    BOOL fg_can_add = FALSE;
    I16 temp_env = pv_cell_TA(pcb_addr);
    I16 temp_in = pv_cell_TWin1(pcb_addr);
    
    if (S_CM_GetStatus(cm, CM_NEED)
    && !USE_FIX_COMP		                                                    /* ��Ƶѹ�� */
    && CmRotation[cm] < log_mod_get_freq_max(Cm[cm].run_mode, temp_env, temp_in)/* Ƶ��С�����Ƶ�� */
    && !S_CM_GetStatus(cm, CM_DEFROST)		                                    /* �ǳ�˪ */
    && !fg_idle_sta_antifreeze				                                    /* �Ǵ���̬����:ʹ����ʱ�����ڿ���ѹ�� */
    && !fg_forbid_rise[cm]                                                      /* ��������Ƶ */
    && !fg_force_drop[cm]                                                       /* �����ؽ�Ƶ */
    && cnt_no_freq_limit[cm]>10                                                 /* �������޽�Ƶ */
    )
    {
        fg_can_add = TRUE;
    }

    return fg_can_add;
}

/*******************************************************************************
 �� ��: log_mod_comp_can_sub()
 �� ��: ѹ���ɽ�Ƶ
 �� ��: comp_num--; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL log_mod_comp_can_sub(U08 cm)
{
    BOOL fg_can_sub = FALSE;
    I16 temp_env = pv_cell_TA(pcb_addr);
    I16 temp_in = pv_cell_TWin1(pcb_addr);
    
    if (   S_CM_GetStatus(cm, CM_NEED)
        && !USE_FIX_COMP                  // ��Ƶѹ��     
        && CmRotation[cm] > log_mod_get_freq_min(Cm[cm].run_mode, temp_env, temp_in) // Ƶ�ʴ�����СƵ��
        && !S_CM_GetStatus(cm, CM_DEFROST)           // �ǳ�˪
//        && log_mod_comp_is_on_stab(cm)               // ���ȶ�
//        && !comp_freq_limit[cm].forbid_sub           // �ǽ�����
//        && !Cm[cm].fg_reoil                          // �ǻ���
//        && comp_freq_step[cm] == COMP_FREQ_STEP_RUN  // ����        
    )
    {
        fg_can_sub = TRUE;
    }

    return fg_can_sub;
}

/*******************************************************************************
 �� ��: log_mod_comp_status()
 �� ��: ѹ��״̬����(������ǰ����״̬����ʹ��״̬)
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_mod_comp_status(void)
{
    U08 i;
    U16 comp_status[MAX_STATUS];         /* ѹ��״̬��ʱֵ */
    
    
    //ѹ��״̬
    for (i=0; i<self_comp_num; i++)         /* ǰһ����Ϣ */
    {
        comp_status[i] = 0;
        
        /* ѹ������״̬ */
        if (s_get_cm_status(i, CM_NEED))       /* ѹ������״̬ */
        {
            comp_status[i] |= COMP_RUN_NEED;
        }
        if (s_get_cm_status(i, CM_ON)) //  
//      if (s_get_output(i, SW_CM))       /* ѹ������״̬ */
        {
            comp_status[i] |= COMP_RUN;
        }

        if (s_get_cm_status(i, CM_DEFROST))  /* ��˪״̬ */
        {
            comp_status[i] |= COMP_DEFROST;
        }
        if (defrost_fg[i].defrost_apply)            /* ��˪���� */
        {
            comp_status[i] |= COMP_DEFROST_APPLY;
        }
        if (s_get_cm_status(i, CM_FORCE_DEFROST))  /* ǿ�Ƴ�˪״̬ */
        {
            comp_status[i] |= COMP_FORCE_DEFROST;
        }

        /* ѹ����ʹ��״̬ */
        if (log_mod_comp_can_open(i))          /* ѹ���ɿ� */
        {
            comp_status[i] |= COMP_CAN_OPEN;
        }
        if (log_mod_comp_can_close(i))          /* ѹ���ɹ�  */
        {
            comp_status[i] |= COMP_CAN_CLOSE;
        }
        
        /* ��/ж������ */
        if (log_mod_comp_prio(i, _ADD))
        {
            comp_status[i] |= COMP_ADD_PRIO;
        }
        if (log_mod_comp_prio(i, _SUBB))
        {
            comp_status[i] |= COMP_SUB_PRIO;
        }

        /* ��ǰ����ģʽ */
        comp_status[i] |= ((U16)(Cm[i].run_mode & 0x07) << COMP_MODE);
        
        /* ѹ����ʹ��ģʽ */
        comp_status[i] |= ((U16)(s_comp_can_use(i)) << 9);
        
        if (log_mod_comp_can_add(i))           /* ѹ���ɼ��� */
        {
            comp_status[i] |= COMP_CAN_ADD;
        }
        if (log_mod_comp_can_sub(i))           /* ѹ����ж�� */
        {
            comp_status[i] |= COMP_CAN_SUB;
        }
        
        pCell->cell_status[i] = comp_status[i];
    }

    //ģ��״̬
    comp_status[MAX_COMP] = 0;
    /* ģ��״̬(�Ƿ�����) */
    if (log_mod_cell_is_run(MODE_NULL))
    {
        comp_status[MAX_COMP] |= CELL_RUN;
    }
    /* ģ��ˮ�ÿ��� */
    if (fg_cell_pump_open)
    {
        comp_status[MAX_COMP] |= CELL_PUMP_OPEN;
    }
    /* ģ�����״̬(�Ƿ����) */
    if (log_mod_cell_is_err())
    {
        comp_status[MAX_COMP] |= CELL_ERR;
    }
    /* ģ��ѹ������ */
    comp_status[MAX_COMP] |= (((U16)(self_comp_num<<4)) & CELL_COMP);
    /* ģ���������ˮ */
    if (val_hotw_used && (pcb_addr==0))
    {
        comp_status[MAX_COMP] |= CELL_CAN_RUN_HOTW;
    }
    /* ��Ԫǿ���������ȹر� */
    if (fg_force_IBH_off)
    {
        comp_status[MAX_COMP] |= CELL_FORCE_IBH_OFF;
    }
    /* ģ��ˮ��״̬ */
    if ((fg_PUMPf_run
        &&!fg_water_lack_err
        &&(PUMPf_time.on >= (TF_VAL(AC_PUMP_ON_DLY,TF_PUMP_ON_DLY) + TF_VAL(val_err_ac_flux_delay,TF_ERR_FLOW_DLY) + 2)))
    ||  TF_IS_OUT				/* �̼����ʱ���Կյ��ÿ���ʱ */
    )
    {
        comp_status[MAX_COMP] |= CELL_PUMP_RUN;
    }
    
    pCell->cell_status[MAX_COMP] = comp_status[MAX_COMP];
}


extern U08 cnt_mod_comp_limit_TA_dly[SUB_SYS_TYPE_MAX][CTRL_MODE_MAX];
/*******************************************************************************
 �� ��: log_mod_limit_comp_TA_area()
 �� ��: ��ȡ����ѹ�������¶ȷ���
 �� ��: mode--ģʽ
        type--��ϵͳ����
 �� ��: result--����
 �� ��: ��
*******************************************************************************/
U08 log_mod_limit_comp_TA_area(U08 mode, U08 type)
{
    BOL fg_env_limit_used;
    I16 point1, point2, point3, point4; // �Ӵ�С����
    U08 mode_idx = 0;
    U08 grp_idx = 0;
    U08 area_new = AREA_00;
    static U08 area_cur[SUB_SYS_TYPE_MAX][CTRL_MODE_MAX] = {{AREA_00,AREA_00,AREA_00},{AREA_00,AREA_00,AREA_00}};
    static BOL result[SUB_SYS_TYPE_MAX][CTRL_MODE_MAX] = {{COMP_KEEP_AREA,COMP_KEEP_AREA,COMP_KEEP_AREA},{COMP_KEEP_AREA,COMP_KEEP_AREA,COMP_KEEP_AREA}};

    /* ������俪����ctrl_mode��ֵ���¿��ܲ���ʱ */
    if (mode == MODE_NULL)
    {
        return COMP_KEEP_AREA;
    } 

    /* ȷ��ģʽ */
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

    /* ȷ���������� */
    switch (type)
    {
        case SUB_SYS_TYPE_MAX:
        case SUB_SYS_AC_HOTW:               /* �յ�+��ˮ��ϵͳ */
            grp_idx = 0;
            break;

        case SUB_SYS_AC_ONLY:               /* ���յ���ϵͳ */
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

    /* 3����������ʽ���»������� */
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
    ������
    ���ܣ���ȡ������ۼ�����ʱ��
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
//  ���ܣ���ʼ��������ۼ�����ʱ��
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
    ����: �������ȵ�ŷ�
    ע�⣺
********************************************************/
void log_mod_Heater_Valve(void)
{
    static U08 heat_valve_out = FALSE;
    

    if (!(Cm[MAX_COMP].run_mode==MODE_HEAT) || status_run!=_RUN )   /* ������Ҳ�Ƿ��� */     
    {
        return;
    }
    
    if (SensorCannotUse(pv_sys_TA))    /* �����¶�̽ͷ�����˳�*/              
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
        s_set_output(MAX_COMP, SW_HEAT_SV);     /* �����ȵ�ŷ� */   
    }
    else
    {
        s_clr_output(MAX_COMP, SW_HEAT_SV);
    }
    
}
#endif

/*==============================================
����:������������߼�
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
 �� ��: Logic_valve_defr()
 �� ��: ��˪���߼�
 �� ��: x--; 
 �� ��: 
 �� ��: ��
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
����:������Һ���߼�
==================================================*/
static void Logic_buway_vavle(U08 x)
{
    static U08  fg_byway_have_run[MAX_COMP];

    I16 gas_temp = pv_gas_out_high(x);

    if (SensorCannotUse(gas_temp))   return;
    
    if (s_get_output(x, SW_CM) /* ��ѹ�� */
    && !CmIsCool(x)         /* 2022-6-15 �ͻ�Ҫ������ʱ������Һ�� */
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
 �� ��: log_mod_cond_type()
 �� ��: �ж�ģ��ѹ��ʹ�õ�����������
 �� ��: comp_num--ѹ������; 
 �� ��: 1 ̨ --> ֻ���ض���
        2 ̨ --> ����val_cond_type1 ����
 �� ��: ��
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
* ������fan_comp_group_count()
* ���ܣ��������õķ�����÷�ʽ���㹲�÷����һ��ѹ����ʼ�ͽ���ѹ�����
* ������fan_type: ���������ͣ�&begin��&end(���÷����һ��ѹ���Ŀ�ʼ�ͽ�����Ŵ�ŵ�ַ)��num(��ǰѹ�������)
        
* ���أ���
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
    
    if (*end > val_comp_num)    *end = val_comp_num;    /* ��Χ�ж� */
}




/*******************************************************************************
 �� ��: Condition_judge()
 �� ��: ��������ж�
 �� ��: curt_state----��ǰ����״̬; 
        curt_temp--��ǰֵ; 
        borderline--�߽�ֵ; 
        Diff--�ز�ֵ; 
        direction--�жϷ���(DRCT_UNDER��С�ڱ߽�ֵΪ�棻DRCT_ABOVE�����ڱ߽�ֵΪ��); 
        
 �� ��: TRUE/FALSE--�µĿ���״̬Ϊ��/��
 �� ��: 
*******************************************************************************/
typedef enum tagDIRECTION
{
    DRCT_UNDER,
    DRCT_ABOVE
}DIRECTION;

BOOL Condition_judge(BOOL curt_state, I16 curt_temp, I16 borderline, I16 Diff, DIRECTION direction)
{  
    BOOL new_state = curt_state;    //��״̬
    
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
 �� ��: Condition_midjudge()
 �� ��: �ж��¶��Ƿ��ڸ�����Χ����
 �� ��: curt_state----��ǰ����״̬; 
        curt_temp--��ǰֵ; 
        borderupline--�߽�ֵ���޶�; 
        borderdownline--�߽�ֵ����
        Diff--�ز�ֵ; 
        
 �� ��: 0: ���ڸ���ֵ��Χ 1:�ڸ���ֵ��Χ
 �� ��: 
*******************************************************************************/

U08 Condition_midjudge(U08 curt_state, I16 curt_temp, I16 borderupline, I16 borderdownline,I16 Diff)
{  
    U08 new_state = curt_state;    //��״̬
    
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
 �� ��: Condition_judge()
 �� ��: ��ʱ3���ж�Ϊ��״̬
 �� ��: curt_state----��ǰ����״̬; 
        curt_temp--��ǰֵ; 
        borderline--�߽�ֵ; 
        Diff--�ز�ֵ; 
        direction--�жϷ���(DRCT_UNDER��С�ڱ߽�ֵΪ�棻DRCT_ABOVE�����ڱ߽�ֵΪ��); 
        judgid--�ڼ����ж�
 �� ��: TRUE/FALSE--�µĿ���״̬Ϊ��/��
 �� ��: 
*******************************************************************************/
BOOL Condition_2judge(BOOL curt_state, I16 curt_temp, I16 borderline, I16 Diff, DIRECTION direction,U16 judgid)
{  
    BOOL new_state = curt_state;    //��״̬

    if(judgid>=TIME_CNT2_MAX)judgid=TIME_CNT2_MAX-1; //��ֹ�������ݳ���
    
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
 �� ��: Condition_judge()
 �� ��: ��ʱjudgdly���ж�Ϊ��״̬
 �� ��: curt_state----��ǰ����״̬; 
        curt_temp--��ǰֵ; 
        borderline--�߽�ֵ; 
        Diff--�ز�ֵ; 
        direction--�жϷ���(DRCT_UNDER��С�ڱ߽�ֵΪ�棻DRCT_ABOVE�����ڱ߽�ֵΪ��); 
        judgdly---��ʱʱ��
        judgid--�ڼ����ж�
 �� ��: TRUE/FALSE--�µĿ���״̬Ϊ��/��
 �� ��: 
*******************************************************************************/
BOOL Condition_3judge(BOOL curt_state, I16 curt_temp, I16 borderline, I16 Diff, DIRECTION direction,U16 judgdly, U16 judgid)
{  
    BOOL new_state = curt_state;    //��״̬
//    static U16 time_cnt[20] = {0.0};    //��״̬

    if(judgid>=TIME_CNT3_MAX)judgid=TIME_CNT3_MAX-1; //��ֹ�������ݳ���
    
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
 �� ��: pwm_fan_speed_to_percent()
 �� ��: pwm�������--��Ŀ��ת�١���ǰת�ټ�������ٷֱ�
 �� ��: num--������; 
        speed_sv--Ŀ��ת��; 
        speed_pv--��ǰת��; 
 �� ��: ����ٷֱ�
 �� ��: ��
*******************************************************************************/
I16 pwm_fan_speed_to_percent(U08 num, I16 speed_sv, I16 speed_pv)
{
    static I16 per[FREQSPEEDNUM];                                   /* ת��������ٷֱ� */

    #define PWM_CTRL_PERIOD (val_pwm_fan_period)                    /* ���ټ������ */
    #define PWM_DEAD_AREA   (val_pwm_fan_area*60/val_fan_pwm_pulse) /* ����������2�������rpmƫ�� */

    #define SPEED_MAX       (1200)                                  /* �����٣����ò����滻��rpm */
    #define PWM_STEP_MAX    (10*pow(10, fg_has_dot))                /* �����ڲ�����% */
    #define PWM_INIT        (val_pwm_fan_init*pow(10, fg_has_dot))  /* ��ʼֵ��% */
    #define PWM_COEF        (val_pwm_fan_coef*pow(10, fg_has_dot))  /* ƫ��ϵ����% */
    #define PWM_PCT_MAX     (100*pow(10, fg_has_dot))               /* �ٷֱ����ԭʼֵ */
    #define PWM_PCT_MIN     (fg_has_dot?110:0)                      /* �ٷֱ���Сԭʼֵ */

    BOL fg_has_dot = TRUE;  /* Ĭ��0.1���� */

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
        /* ����ǰ׼�� */
        if (per[num] == 0)
        {
            per[num] = PWM_INIT;
        }
        else
        {
            I16 speed_e = speed_pv-speed_sv;/* ƫ�� */
            /* ���ڲ�����ƫ��Խ�󣬵������Խ�� */
            U08 pwm_step = fun_pv_limit(Fun_AbsI32(speed_e)*PWM_COEF/SPEED_MAX, 1, PWM_STEP_MAX);
            
            if (cnt_pwm_fan_ctrl_period[num] > PWM_CTRL_PERIOD)
            {
                cnt_pwm_fan_ctrl_period[num] = 0;
                
                if (speed_e > PWM_DEAD_AREA)                /* ���� */
                {
                    per[num] -= pwm_step;
                }
                else if (speed_e < -PWM_DEAD_AREA)          /* Ƿ�� */
                {
                    per[num] += pwm_step;
                }
                else                                        /* ���� */
                {
                    ;       
                }
            }
        }
    }

    /* ��Χ�޶� */
    if (per[num] != 0)
    {
        per[num] = fun_pv_limit(per[num], PWM_PCT_MIN, PWM_PCT_MAX);
    }
    
    return per[num];
}

/*******************************************************************************
 �� ��: open_fan_by_env_linear()
 �� ��: �������Կ��Ʒ���
 �� ��: cm--ѹ��; 
 �� ��: ����
 �� ��: ��
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
                if (fg_open)/* ͣ���ʱ��ռ�ʱ */
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
                    fan_speed = min_speed;  /* ����ͷ��ٽ������¿� */
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
                if (fg_open)/* ͣ���ʱ��ռ�ʱ */
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
                    fan_speed = min_speed;  /* ����ͷ��ٽ������¿� */
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
    
    /* ���� */
    fg_open_bak = fg_open;    

    return fan_speed;
}

/*******************************************************************************
 �� ��: open_fan_by_wing_linear()
 �� ��: �������Կ��Ʒ���
 �� ��: cm--ѹ��; 
 �� ��: ����
 �� ��: ��
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
                if (fg_open)/* ͣ���ʱ��ռ�ʱ */
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
                    fan_speed = min_speed;  /* ����ͷ��ٽ������¿� */
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
                if (fg_open)/* ͣ���ʱ��ռ�ʱ */
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
                    fan_speed = min_speed;  /* ����ͷ��ٽ������¿� */
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

    /* ���� */
    fg_open_bak = fg_open;

    return fan_speed;
}

/*******************************************************************************
 �� ��: log_mod_get_invt_id_max()
 �� ��: ��ȡʵ��INVT_ID_MAX�����������ѭ�����������
 �� ��: ��
 �� ��: ʵ��INVT_ID_MAX
 �� ��: ��
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
 �� ��: freq_fan_speed_get()
 �� ��: ��Ƶ���ٴ���
 �� ��: cm--ѹ����; 
 �� ��: ��Ƶ����
 �� ��: ��
*******************************************************************************/
U16 freq_fan_speed_get(U08 cm, I16 speed_cur)
{
    #define ALLOW_CORRECT_DLY 3*60
    
    I16 speed = 0;
    I16 freq_min = CmIsCool(cm) ? val_freq_fan_cool_min : val_freq_fan_heat_min;
    I16 freq_max = CmIsCool(cm) ? val_freq_fan_cool_max : val_freq_fan_heat_max;
    I16 freq_mid = CmIsCool(cm) ? val_freq_fan_cool_mid : val_freq_fan_heat_mid;
    I16 freq_min_cpst = (I32)freq_min*val_min_Cpst_Coef/100;
    I16 invt_temp = pv_cell_invt_tmp(cm);    // ��Ƶ���¶�
    I16 temp_env = pv_cell_TA(pcb_addr);
    I16 temp_in  = pv_cell_TWin1(pcb_addr);
    static U08 fg_invt_temp;
    BOL fg_silent = fg_silent_mode;
    static U08 Tp_corr_area = AREA_00;

#if 0
    /* ����ط��ʱ���������޸�Ϊ0 */
    if (val_close_fan_diff_cl != 0
    ||  val_close_fan_diff_ht != 0
    )
    {
        freq_min_cpst = 0;
    }
#endif

    if (invt_temp > val_ivnt_prot_temp)             fg_invt_temp = 1;
    else if (invt_temp < val_ivnt_prot_temp-50)     fg_invt_temp = 0;

    /* ���ٴ��� */
    if (val_freq_fan_set == FREQ_FAN_SET_UNUSED)
    {
        speed = 0;
    }
    else if (val_freq_fan_ctrl == FREQ_FAN_CTRL_MANUAL)     /* �ֶ� */
    {
        speed = val_freq_fan_hand_speed;
    }
    else
    {
        if (fg_invt_temp               /* ��Ƶ���¶ȿ��� */
        || (Cm[cm].defrost_stage==DS_QUIT && Cm[cm].defrost_dly<DEFROST_DLY_3)  /* �˳�˪ */
        )
        {
            speed = freq_max;
            SETBIT(fan_adjust_fg[cm], FAN_ADJ_FG_HIGH);
        }
        else
        {
            I16 speed_tmp;

            /* ���ٻ������� */
            if (val_freq_fan_ctrl == FREQ_FAN_CTRL_ENV)
            {
                speed_tmp = open_fan_by_env_linear(cm, freq_min, freq_max, freq_mid); /* �������� */
            }
            else
            {
                speed_tmp = open_fan_by_wing_linear(cm, freq_min, freq_max, freq_mid);/* �������� */
            }
            fan_adjust_tmp[cm][FAN_ADJ_TMP_BASE] = speed_tmp;

            /* ѹ��Ƶ���������ٵ��� */
            if (cm_time[cm].on >= ALLOW_CORRECT_DLY)
            {
                /* ��ʾ��������Ϊ�˵õ�����Ƶ�����ޣ�������Ϊ����Ƶ���Ӽ�������Ƶ������ʱע�ⲻ�ô������� */
                I16 invt_max = wf_get_freq_max_by_env_in(Cm[cm].run_mode, temp_env, temp_in);
                U16 coef = fun_pv_limit(100-val_freq_Cpst_Coef+(I32)CmRotation[cm]*val_freq_Cpst_Coef/invt_max, val_min_Cpst_Coef, 100);    /* % */
                speed_tmp = (I32)speed_tmp*coef/100;
                fan_adjust_tmp[cm][FAN_ADJ_TMP_COMP] = speed_tmp;
            }

            /* ����ģʽ */
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

            /* �����������ٵ��� */
            if (cm_time[cm].on >= ALLOW_CORRECT_DLY
            && ((CmIsCool(cm)&&FAN_TP_COOL_USE) || (CmIsHeat(cm)&&FAN_TP_HEAT_USE))
            )
            {
                Tp_corr_area = fun_area(pv_cell_gas_out(0), Tp_corr_area, 50, 850, 720, SENSOR_NOEXIST);
                speed_tmp = (Tp_corr_area > AREA_02) ? speed_tmp \
                                                     : ((I32)speed_tmp*val_Tp_fan_rpm_corr(Tp_corr_area)/100);

                fan_adjust_tmp[cm][FAN_ADJ_TMP_EXH] = speed_tmp;
            }

            if (speed_cur == 0)             /* ���޵���������� */
            {
                speed = speed_tmp;
            }
            else if (speed_tmp > 0)         /* ����0ʱֱ�ӹرգ���ִ�С����ٶ��������� */
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

            /* ����ģʽ���� */
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

            /* ��Χ�޶� */
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
 �� ��: fix_fan_speed_get()
 �� ��: ��Ƶ������ٴ���
 �� ��: cm--ѹ����; 
 �� ��: ��Ƶ����
 �� ��: ��
*******************************************************************************/
U08 fix_fan_speed_get(U08 cm)
{
    #define FAN_ONOFF_DIFF     20  //�ط���ز�(0.1��)
    enum
    {
        FAN_ENVM,   //�����¶ȿ����
        FAN_COIL,   //��Ƭ�¶ȿ����
        FAN_INVT,   //��Ƶ�¶ȿ����
        FAN_MAX
    };
    static U08 fg_fan_mid_on[FAN_MAX];  //���ٷ������
    static U08 fg_fan_hig_on[FAN_MAX];  //���ٷ������
    
    U08 speed = FAN_NONE ;   

    I16 temp_env = pv_cell_TA(pcb_addr);

    if (CmIsCool(cm))
    {
        //�����¶ȷ�������
        fg_fan_hig_on[FAN_ENVM] = Condition_judge(fg_fan_hig_on[FAN_ENVM], temp_env, val_hig_fan_env_cold, FAN_ONOFF_DIFF, DRCT_ABOVE);
        fg_fan_mid_on[FAN_ENVM] = Condition_judge(fg_fan_mid_on[FAN_ENVM], temp_env, val_low_fan_env_cold, FAN_ONOFF_DIFF, DRCT_ABOVE);

        //��Ƭ�¶ȷ�������
        fg_fan_hig_on[FAN_COIL] = Condition_judge(fg_fan_hig_on[FAN_COIL], pv_cell_wing(cm), val_hig_fan_wing_cold, FAN_ONOFF_DIFF, DRCT_ABOVE);
        fg_fan_mid_on[FAN_COIL] = Condition_judge(fg_fan_mid_on[FAN_COIL], pv_cell_wing(cm), val_low_fan_wing_cold, FAN_ONOFF_DIFF, DRCT_ABOVE);
    }
    else if (CmIsHeat(cm))
    {
        //�����¶ȷ�������
        fg_fan_hig_on[FAN_ENVM] = Condition_judge(fg_fan_hig_on[FAN_ENVM], temp_env, val_hig_fan_env_heat, FAN_ONOFF_DIFF, DRCT_UNDER);
        fg_fan_mid_on[FAN_ENVM] = Condition_judge(fg_fan_mid_on[FAN_ENVM], temp_env, val_low_fan_env_heat, FAN_ONOFF_DIFF, DRCT_UNDER);

        //��Ƭ�¶ȷ�������
        fg_fan_hig_on[FAN_COIL] = Condition_judge(fg_fan_hig_on[FAN_COIL], pv_cell_wing(cm), val_hig_fan_wing_heat, FAN_ONOFF_DIFF, DRCT_UNDER);
        fg_fan_mid_on[FAN_COIL] = Condition_judge(fg_fan_mid_on[FAN_COIL], pv_cell_wing(cm), val_low_fan_wing_heat, FAN_ONOFF_DIFF, DRCT_UNDER);
    }
    //��Ƶ�¶ȷ�������
    fg_fan_hig_on[FAN_INVT] = Condition_judge(fg_fan_hig_on[FAN_INVT], pv_cell_invt_tmp(cm), val_ivnt_prot_temp, FAN_ONOFF_DIFF, DRCT_ABOVE);
    fg_fan_mid_on[FAN_INVT] = FALSE;
    

    if(fg_fan_hig_on[FAN_ENVM] || fg_fan_hig_on[FAN_COIL] || fg_fan_hig_on[FAN_INVT])
    {
        speed = FAN_HIGH;  // �߷���
    }
    else if(fg_fan_mid_on[FAN_ENVM] || fg_fan_mid_on[FAN_COIL] || fg_fan_mid_on[FAN_INVT])
    {
        speed = FAN_MID;  // �з���
    }
    else
    {
        speed = FAN_LOW;  // �ͷ���
    }

    return speed;
}

/*******************************************************************************
 �� ��: fix_fan_out()
 �� ��: ��Ƶ������
 �� ��: cm--ѹ����; 
        fan_type--��Ƶ�������; 
        speed--����; 
 �� ��: ��
 �� ��: ��
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
    ����: ģ��ķ������
/==========================================*/
static void fan_logic(void)
{
    U08 cm = 0;
    I16 speed_freq = 0;             /* ��Ƶ���� */
    I16 speed_fix = FAN_NONE;       /* ��Ƶ���� */

    extern const U08 err_pwm_fan1[];
    extern const U08 err_pwm_fan2[];
    
    /* �ֶ������:����Ҳ����������˪������ */
    {
        if (val_freq_fan_ctrl == FREQ_FAN_CTRL_MANUAL
        &&  !s_get_cm_status(cm, CM_NEED)
        )
        {
            s_set_cm_status(cm, CM_FAN);
        }
    }
    
    /* ���ٻ�ȡ */
    /* �����˪��Logic_comp_defrost_inverter�д��� */
    if ((!s_get_cm_status(cm, CM_FAN))
//    ||  s_get_error(S_ERR_COOL_FLUX_OL)
    ||  s_get_error(err_pwm_fan1[cm])                   /* ���1ת���쳣 */      
    ||  s_get_error(err_pwm_fan2[cm])
        )  
    {
        speed_freq = 0;
        speed_fix = FAN_NONE;
    }
    else
    {
        speed_freq = freq_fan_speed_get(cm, speedsv[0]);    /* ��Ƶ */
        speed_fix  = fix_fan_speed_get(cm);                 /* ��Ƶ */
    }

    /* ������� */
    ARRSET(speedsv, speed_freq);                            /* ��Ƶ */
    fix_fan_out(cm, val_fan_speed_type, speed_fix);         /* ��Ƶ */

    if (speed_fix != FAN_NONE
    ||  speedsv[FREQ_FAN1]>0
    ||  speedsv[FREQ_FAN2]>0
        )
    {
        fg_have_fan_on = TRUE;      /* ֻҪ��һ��������������ΪTRUE */
    }
    else
    {
        fg_have_fan_on = FALSE;
    }
}

/*==========================================
    ����: ģ���ѹ������
/==========================================*/
// ѹ��ϵͳ��˪�������̴���(��Ƶѹ��)
void Logic_comp_defrost_inverter(U08 x, BOOL fg_four_way_valve)
{
    #define MAX_UNLOAD_TIME     60  //ж�س�ʱʱ�䣨�룩
        
    switch(Cm[x].defrost_stage) // ѹ����˪����
    {
        case DS_IDLE:       // ѹ��δ��˪
            Cm[x].defrost_dly = 0;
            Cm[x].defrost_stage = DS_READY;
            cm_time[x].cnt_time_fan_delay=0;
//          break;
            
        case DS_READY:      // ѹ��׼����˪
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
            else    //NOTE-CXW ׼�������������˳���
            {
                Cm[x].defrost_stage = DS_QUIT;
                Cm[x].defrost_dly = 0;
            }
            cm_time[x].cnt_time_fan_delay=0;    // ��˪�����У��˳�ʱ�����㣨����˴����壬�ﲻ�����桰�����1S�ٹ�ѹ������Ч����    
            break;  
            
        case DS_DEFROST:    // ѹ����ʽ��˪
            if(s_get_cm_status(x, CM_NEED)) // �����������ѹ�����л���˪                   
            {
                // ѹ������
                s_set_cm_status(x, CM_ON);  

                /* ѹ����Ƶ��Ŵ���������ͨ�� */
                if (Cm[x].defrost_dly >= val_defrost_stop_cm_tm_entry
                || fg_ability_test_run
                )
                {
                    // �ط��
                    s_clr_cm_status(x, CM_FAN);

                    // ��ͨ������
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
                || (!s_get_cm_status(x, CM_DEFROST)))   // ֱ���˳���˪           
            {
                Cm[x].defrost_stage = DS_QUIT;
//              if(!s_get_cm_status(x, CM_NEED))    // ����ʱ�����˳�˪ͣ��
//              {
//                  Cm[x].defrost_stage = DS_IDLE;
//              }
                Cm[x].defrost_dly = 0;
            }

            cm_time[x].cnt_time_fan_delay=0;    // ��˪�����У��˳�ʱ�����㣨����˴����壬�ﲻ�����桰�����1S�ٹ�ѹ������Ч����    
            break;  
            
        case DS_QUIT:       // ѹ���˳���˪
            // ��������ȴ�ͣ���ˮʱ��
            if (Cm[x].defrost_dly >= val_defrost_dripping_time)
            {
                s_set_cm_status(x, CM_FAN); // �����
            }
            
            // ��ͨ���л�������
            if (Cm[x].defrost_dly >= DEFROST_DLY_2
            ||  fg_ability_test_run
                )
            {
                if(fg_four_way_valve)   
                {
                    s_set_cm_status(x, CM_VALVE_S); // ����ͨ��
                }
                else
                {
                    s_clr_cm_status(x, CM_VALVE_S);
                }
            }
            
            
            // ��ʱ������˳���˪
            if(Cm[x].defrost_dly >= DEFROST_DLY_3+2*x) // ÿ��ѹ����2��
            {
                Cm[x].defrost_stage = DS_IDLE;
                Cm[x].defrost_dly = 0;
            }

            break;  
    }
}

/********************************************************
    ����: ������̵�����߼�
    ע�⣺
********************************************************/
void Logic_Anti_Icing_Heater(void)
{
    #define val_exit_defro_delay            (360)   /* �س�˪������ʱ */
    static U08 flag_open = FALSE;   /* ����־ */
    I16 open_env = val_anti_ice_heat_env;
    I16 temp_env = pv_cell_TA(pcb_addr);

    if (val_chassis_heat_mode == HEATER_MODE_DEFROST)
    {
        if (s_get_cm_status(0, CM_DEFROST))
        {
            /* ����С�ڿ���0��  */
            if (temp_env <= open_env )
            {   
                flag_open = TRUE;
            }
            else if (temp_env > open_env+2*10)
            {
                flag_open = FALSE;
            }
            close_defro_heat_cnt = 0;   // ��˪ʱ����ʱ����
        }
        else 
        {
            if (flag_open)  /* �жϳ�˪ʱ��û�п������ */
            {
                if (close_defro_heat_cnt >= val_exit_defro_delay)   flag_open = FALSE;
            }
                
        }
    }
    else if (val_chassis_heat_mode == HEATER_MODE_LOWENV)
    {
        if (s_get_cm_status(0, CM_ON))
        {
            /* ����С�ڿ���0��  */
        	if (temp_env <= open_env)
        	{	
        		flag_open = TRUE;
        	}
        	else if (temp_env > open_env + 2*10)
        	{
        		flag_open = FALSE;
        	}
            close_defro_heat_cnt = 0;	// ��ʱ����
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
        s_set_output(MAX_COMP, S_DO_DPH);   /* �����̵���� */
        s_set_output(MAX_COMP, S_DO_DPH2);  /* �����̵����2 */
    }
}

/*******************************************************************************
 �� ��: log_comp_freq_pid()
 �� ��: pidƵ�ʵ���
 �� ��: pv--��ǰˮ��; 
        sv--Ŀ��ˮ��; 
        NowFreNeed--��ǰƵ��; 
 �� ��: Ƶ������
 �� ��: ��
*******************************************************************************/
I32 log_comp_freq_pid(I16 pv, I16 sv, I16 NowFreNeed, U08 Mode)
{
    static I16 pid_pv;  /* !!pv��svʹ�ô�ַ��ʽ�����ܶ���Ϊ�Զ����� */
    static I16 pid_sv;
    
    PID_ID_ENUM PID_id = PID_ID_INVT_1;

    I32 pid_kc;
    I32 pid_uk;
    static I16 Pid_Residue = 0;       // PID����ʣ����

    I32 pid_result = 0;

    pid_pv = pv;
    pid_sv = sv;
    
    pid_set_ctrl(PID_id, PID_CTRL_INCREMENT);       //����ʽ
    pid_set_Ts(PID_id, val_f_pid_calculate_period); //��������
    pid_set_Kp(PID_id, val_f_pid_Kp);//����ϵ��
    pid_set_Ti(PID_id, val_f_pid_i);//����ʱ��
    pid_set_Td(PID_id, val_f_pid_d);//΢��ʱ��
    pid_set_Kc(PID_id, val_f_pid_p);//�����ñ�����
    pid_set_Ai(PID_id, val_f_pid_flexi);//����ϵ��
    pid_set_sv(PID_id, &pid_sv);//Ŀ��ֵ
    pid_set_pv(PID_id, &pid_pv);//��ǰֵ
    
    pid_turn_on(PID_id);
   
    DebugINVT_ID_1PID[DEBUG_PID_SV] = pid_sv;
    DebugINVT_ID_1PID[DEBUG_PID_PV] = pid_pv;

    if (AdjustFreqPeriod >= val_f_pid_period)    // ��Ƶ�������ڵ���һ��
    {   
        AdjustFreqPeriod = 0;

        pid_uk = pid_read_Uk(PID_id); //PID����
        pid_kc = pid_get_Kc(PID_id);

        pid_result = _Math_divide(&Pid_Residue, pid_uk*NowFreNeed, pid_kc);
    }

    DebugINVT_ID_1PID[DEBUG_PID_MIDDL]=pid_uk*NowFreNeed;

    return (Mode==MODE_HEAT ? -pid_result : pid_result);    /* ���ȴ��� */
}

enum
{
    FORBID_ADD_GAS,  //��ֹ��Ƶ����
    FORBID_ADD_LP,   //��ֹ��Ƶ��ѹ
    FORBID_ADD_HP,   //��ֹ��Ƶ��ѹ
    FORBID_ADD_COND,   //��ֹ��Ƶ����
    FORBID_ADD_EVAP,   //��ֹ��Ƶ����
    FORBID_ADD_MAX
};
enum
{
    FORBID_SUB_OUT,         //��ֹ��Ƶ����
    FORBID_SUB_MAX
};
enum
{
    FORCE_SUB_GAS,              //ǿ�ƽ�Ƶ�����¶�
    FORCE_SUB_LP,               //ǿ�ƽ�Ƶ��ѹ
    FORCE_SUB_HP,               //ǿ�ƽ�Ƶ��ѹ
    FORCE_SUB_COND,         //ǿ�ƽ�Ƶ����
    FORCE_SUB_EVAP,         //ǿ�ƽ�Ƶ����
    FORCE_SUB_MAX
};

//#define PRESS_DIFF          1   /* �ز� */
//#define LP_UNIT             2   // ��ѹ��Ԫ
//#define HP_UNIT             10  // ��ѹ��Ԫ

/* ��ѹ���Ƽ��� */
#define VAL_FORBID_ADD_LP(mode)     ((mode == MODE_COOL ? val_lp_value_incl \
                                                        : val_lp_value_inht) + val_lp_forbid_add_prs_diff)
/* ��ѹ���Ƽ��� */
#define VAL_FORBID_ADD_HP(mode)     ((mode==MODE_COOL ? val_hp_value_incl \
                                                      : val_hp_value_inht) + val_hp_forbid_add_prs_diff)
/* ��ѹǿ��ж�� */
#define VAL_FORCE_SUB_LP(mode)     ((mode==MODE_COOL ? val_lp_value_incl \
                                                     : val_lp_value_inht) + val_lp_force_sub_prs_diff)
/* ��ѹǿ��ж�� */
#define VAL_FORCE_SUB_HP(mode)     ((mode==MODE_COOL ? val_hp_value_incl \
                                                     : val_hp_value_inht) + val_hp_force_sub_prs_diff)

#define COOL_EVAP_DIFF 20

static BOOL fg_forbid_add[MAX_COMP][FORBID_ADD_MAX];//��ֹ��Ƶ��־
static BOOL fg_force_sub[MAX_COMP][FORCE_SUB_MAX];//ǿ�ƽ�Ƶ��־
/*******************************************************************************
 �� ��: log_mod_freq_limit_check()
 �� ��: �޽�Ƶ�ȱ�־����
 �� ��: comp_num--ѹ����; 
		Mode--ģʽ; 
 �� ��: true�������޽�Ƶ��false������
 �� ��: ��
*******************************************************************************/
BOL log_mod_freq_limit_check(U08 comp_num, U08 Mode)
{
    BOL fg_result = FALSE;
    U08 i;
    I16 temp_cond = log_mod_comp_get_temp_cond(comp_num);  /* ��ȡ�����¶� */
    I16 temp_evap = log_mod_comp_get_temp_evap(comp_num);  /* ��ȡ�����¶� */
    
    /* ��ֹ��Ƶ */
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

    /* ǿ�ƽ�Ƶ */
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


    /* bms����ֹ��Ƶ��־ */
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
 �� ��: Calculate_Frequency_Need()
 �� ��: ����Ƶ������
 �� ��: U08 comp_num--ѹ��������U08 Mode--ѹ����ǰģʽ��I16 InletTemp--��ˮ�¶ȣ�I16 EnvTemp--�����¶�
               NowFreNeed:��ǰת�� 
 �� ��: Ƶ��
 �� ��: ��
*******************************************************************************/
U16 Calculate_Frequency_Need(U08 comp_num, U08 Mode,I16 NowFreNeed) 
{
    U16 CalFrequency = NowFreNeed; 

    PID_ID_ENUM PID_id = PID_ID_INVT_1;
    I16 pid_pv = 0;
    I16 pid_sv = 0;
    
    I32 pid_freq=0;
//    static U16 AdjustFreqPeriod;    // Ƶ�ʵ�������
    
    I16 temp_env = pv_cell_TA(pcb_addr);
    I16 temp_in  = pv_cell_TWin1(pcb_addr);
    I16 temp_out = pv_cell_TWout1(pcb_addr);
 
//    extern const U08 err_sub_freq[];    /* N0004 */

    I16 gas_temp = pv_gas_out_high(comp_num);
    U08 gas_out_trend = fun_value_trend(gas_temp, mod_rec_gas_out_avg[comp_num], 1);

    
    if (CalFrequency==0) // û������
    {
        pid_turn_off(PID_id);
        AdjustFreqPeriod = 0;      // �����ʱ����
    }
    else 
    {
        /* ��Ƶ�����¶� */
        if (MODE_IS_HOTW(GET_SUB_SYS(pcb_addr)))
        {
            pid_pv = temp_in;				                        /* ��ˮģʽ�£��̶�ʹ�û�ˮ���� */
        }
        else
        {
            pid_pv = USE_AC_TEMP_IN_CTRL ? temp_in : temp_out;		/* �յ�ģʽ�£����Ƿ�ʹ�û���ˮ��̽ͷ���� */
        }
        /* ��ƵĿ���¶� */
        pid_sv = sv_temp_freq[GET_SUB_SYS(pcb_addr)];
    
        /* Ƶ��������� */
        pid_freq = log_comp_freq_pid(pid_pv, pid_sv, CalFrequency, Mode);
        

        /* ���� */
        {
            /* �޷� */
            if (fg_freq_had_force_sub)
            {
                /* ǿ�ƽ�Ƶ����������Ƶ */
                pid_freq = fun_pv_limit(pid_freq, -val_invt_run_speed, val_invt_run_speed/5);
            }
            else
            {
                pid_freq = fun_pv_limit(pid_freq, -val_invt_run_speed, val_invt_run_speed); 
            }
            
            DebugINVT_ID_1PID[DEBUG_STEP] = pid_freq;
            
            // ǿ��ж��
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
                    CalFrequency = (I32)CalFrequency*val_invt_rps_force_sub_percent/100; // Ƶ�ʽ���Ϊԭ����Ƶ�ʵġ�ǿ�ƽ�Ƶ������

                    fg_freq_had_force_sub = TRUE;
                }
                else
                {
                    if (pid_freq<0)  // ֻ�ܽ�Ƶ
                    {
                        CalFrequency += pid_freq;
                    }                    
                }
            }
            // ���Ƽ���
            else if (fg_forbid_add[comp_num][FORBID_ADD_GAS]==TRUE 
                     || fg_forbid_add[comp_num][FORBID_ADD_LP]==TRUE 
                     || fg_forbid_add[comp_num][FORBID_ADD_HP]==TRUE 
                     || fg_forbid_add[comp_num][FORBID_ADD_COND]==TRUE
                     || fg_forbid_add[comp_num][FORBID_ADD_EVAP]==TRUE)
            {
                if (pid_freq<0)  // ֻ�ܽ�Ƶ
                {
                    CalFrequency += pid_freq;
                }
            }
            // ��������
            else
            {
                CalFrequency += pid_freq;
            }

            /* ����Ԥ��Ƶ:����pidʹ�� */
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

    /* ��Ƶ��Ƶ���޶� */
    {
        I16 FreqMax = log_mod_get_freq_max(Mode, temp_env, temp_in);         /* ��Ƶ�����Ƶ�� */
        I16 FreqMin = log_mod_get_freq_min(Mode, temp_env, temp_in);            /* ��Ƶ����СƵ�� */
        CalFrequency = fun_pv_limit(CalFrequency, FreqMin, FreqMax);
    
        gdebugIO.freq_max = FreqMax;
    }

    // ���������
    CalFrequency = calculate_gb_dbg_freq(Mode, CalFrequency); 

    return CalFrequency;
}




// ѹ��ϵͳ�������̴���
void Logic_comp_action(U08 x)
{
    I16 val_fan_on_dly=0,val_fan_off_dly=0;
    I16 val_commu_on_dly=0,val_commu_off_dly=0;
    
    BOOL fg_four_way_valve=0;   // 0-��ʾ��ǰģʽ���ÿ���   1-��ʾ��ǰģʽ���ÿ���
    if(val_valve_type == HEAT_MODE_ON && Cm[x].run_mode == MODE_HEAT )  fg_four_way_valve=1;    // ����ģʽ����
    if(val_valve_type == COOL_MODE_ON && Cm[x].run_mode == MODE_COOL )  fg_four_way_valve=1;    // ����ģʽ���� 
    
    
    switch(Cm[x].stage) // ѹ����������
    {
        case CM_IDLE:       // ѹ������
            if(s_get_cm_status(x, CM_NEED)) // ѹ��������
            {
                Cm[x].stage = CM_STARTING;  //NOTE-CXW Ҫ���Դ���ת
                Cm[x].dly = 0;
                s_clr_cm_status(x, CM_DEFROST_EXIT);    //NOTE-CXW Ҫ�����˳�˪���Ƿ��ܷ�Ƶ������Ӱ��
            }
            else    //NOTE-CXW �����������־�Ƿ���ʣ�
            {
                s_clr_cm_status(x, CM_VALVE_S); // ����ͨ���������������ͣ������Ҫͣ�������δ˾䣩
                s_clr_cm_status(x, CM_ON);  // ��ѹ��
                s_clr_cm_status(x, CM_FAN); // �ط��               
            }
            Cm[x].defrost_stage = DS_IDLE;
            break;
            
        case CM_STARTING:   // ѹ����������
            // ������ʱ
            val_fan_on_dly      = (Cm[x].run_mode == MODE_HEAT) ? TF_VAL(val_heat_fan_on_dly, TF_FAN_ON_DLY) : TF_VAL(val_cool_fan_on_dly, TF_FAN_ON_DLY);
            val_commu_on_dly    = (fg_four_way_valve==1)        ? TF_VAL(val_valve_on_dly, TF_4V_ON_DLY) : TF_VAL(val_valve_on_dly, TF_4V_ON_DLY);
            
            
            if(s_get_cm_status(x, CM_NEED)) // ѹ��������
            {
                if(val_fan_on_dly<0 && val_commu_on_dly<0)
                {
                    s_set_cm_status(x, CM_ON);  // ��ѹ��
                    
                    if(Cm[x].dly >= -val_fan_on_dly)
                    {
                        s_set_cm_status(x, CM_FAN); // �����
                    }
                    
                    if(Cm[x].dly >= -val_commu_on_dly && fg_four_way_valve==1)
                    {
                        s_set_cm_status(x, CM_VALVE_S); // ����ͨ��
                    }
                    
                    if(Cm[x].dly >= -val_fan_on_dly && Cm[x].dly >= -val_commu_on_dly)
                    {
                        Cm[x].stage = CM_RUNNING;   //NOTE-CXW Ҫ���Դ���ת
                        Cm[x].dly = 0;
                    }
                }
                else if(val_fan_on_dly >= val_commu_on_dly)
                {
                    s_set_cm_status(x, CM_FAN); // �����
                
                    // ��ģʽ����������ͨ��
                    if(fg_four_way_valve==1)    
                    {
                        if(Cm[x].dly >= val_fan_on_dly-val_commu_on_dly)
                        {
                            s_set_cm_status(x, CM_VALVE_S); // ����ͨ��
                        }
                    }
                    
                    if(Cm[x].dly >= val_fan_on_dly)
                    {
                        s_set_cm_status(x, CM_ON);  // ��ѹ��
                    }
                    
                    if(val_commu_on_dly<0 && Cm[x].dly >= val_fan_on_dly-val_commu_on_dly
                        || val_commu_on_dly>=0 && Cm[x].dly >= val_fan_on_dly)
                    {
                        Cm[x].stage = CM_RUNNING;   //NOTE-CXW Ҫ���Դ���ת
                        Cm[x].dly = 0;
                    }
                }
                else if(val_fan_on_dly < val_commu_on_dly)
                {
                    // ��ģʽ����������ͨ��
                    if(fg_four_way_valve==1)    
                    {
                        s_set_cm_status(x, CM_VALVE_S); // ����ͨ��
                    }
                    if(Cm[x].dly >= val_commu_on_dly-val_fan_on_dly)
                    {
                        s_set_cm_status(x, CM_FAN); // �����
                    }
                    
                    if(Cm[x].dly >= val_commu_on_dly)
                    {
                        s_set_cm_status(x, CM_ON);  // ��ѹ��
                    }
                    
                    if(val_fan_on_dly<0 && Cm[x].dly >= val_commu_on_dly-val_fan_on_dly
                        || val_fan_on_dly>=0 && Cm[x].dly >= val_commu_on_dly)
                    {
                        Cm[x].stage = CM_RUNNING;   //NOTE-CXW Ҫ���Դ���ת
                        Cm[x].dly = 0;
                    }
                }
            }
            else
            {
                Cm[x].stage = CM_STOPPING;  //NOTE-CXW Ҫ���Դ���ת
                if(!s_get_cm_status(x, CM_ON))  // ѹ��δ����ֱ��תIDLE
                {
                    Cm[x].stage = CM_IDLE;      
                }
                Cm[x].dly = 0;
            }
            break;
            
        case CM_RUNNING:    // ѹ�����й���
            if(s_get_cm_status(x, CM_NEED)) // ѹ��������
            {
                // ��ͨ������/����
                if(fg_four_way_valve==1)    
                {
                    s_set_cm_status(x, CM_VALVE_S); // ����ͨ��
                }
                else		/* 2022-8-21������״̬����ʹ�����й����еȹ���Ӧģʽ�仯 */
                {
                    s_clr_cm_status(x, CM_VALVE_S); // ����ͨ��
                }
                // ѹ����������
                s_set_cm_status(x, CM_ON);  // ��ѹ��
                // ����������У���Ҫʱ�з��ٵ��ڣ���
                s_set_cm_status(x, CM_FAN); // �����

                if(s_get_cm_status(x, CM_NEED_DEFROST) && s_get_cm_status(x, CM_DEFROST_ALLOW)) // �����˪����
                {
                    Cm[x].stage = CM_DEFROSTING;
                    Cm[x].dly = 0;
                    Cm[x].defrost_stage = DS_IDLE;
                }
            }
            else
            {
                Cm[x].stage = CM_STOPPING;  //NOTE-CXW Ҫ���Դ���ת
                Cm[x].dly = 0;
                Cm[x].defrost_dly = 0;
            }
            break;
            
        case CM_DEFROSTING: // ѹ����˪����
            if(s_get_cm_status(x, CM_NEED)) // ѹ��������         
            {
                Logic_comp_defrost_inverter(x,fg_four_way_valve);   // ѹ����˪���̶�������
                if(Cm[x].defrost_stage == DS_IDLE)
                {
                    Cm[x].stage = CM_RUNNING;
                    Cm[x].dly = 0;  //NOTE-CXW ����Ҫ��Ҫ���㣿
                    s_clr_cm_status(x, CM_DEFROST_OFF);
                }
            }
            else
            {
                Cm[x].stage = CM_STOPPING;  //NOTE-CXW Ҫ���Դ���ת
                Cm[x].dly = 0;
                Cm[x].defrost_dly = 0;

            }
            break;
            
        case CM_STOPPING:   // ѹ���ػ�����
            {
                /* �˵���Ƶ����ͣ��ʱ�� */
                U16 off_dly = Cm[x].dly>CM_off_speed_down_time ? Cm[x].dly-CM_off_speed_down_time : 0;
                // �ػ���ʱ
                val_fan_off_dly     = (Cm[x].run_mode == MODE_HEAT) ? val_heat_fan_off_dly : val_cool_fan_off_dly;
                val_commu_off_dly   = (fg_four_way_valve==1)        ? TF_VAL(val_valve_off_dly, -10) : TF_VAL(val_valve_off_dly, -10) ;
                    
                if(val_fan_off_dly<0 && val_commu_off_dly<0)
                {
                    s_clr_cm_status(x, CM_ON);  /* ��ѹ�� */
                
                    if(off_dly >= -val_fan_off_dly)
                    {
                        s_clr_cm_status(x, CM_FAN); /* �ط�� */
                    }
                
                    if(off_dly >= -val_commu_off_dly)
                    {
                        s_clr_cm_status(x, CM_VALVE_S); /* ����ͨ�� */
                    }
                    if(off_dly >= -val_fan_off_dly && off_dly >= -val_commu_off_dly)
                    {
                        Cm[x].stage = CM_IDLE;  //NOTE-CXW Ҫ���Դ���ת
                        Cm[x].dly = 0;
                    }
                }
                else if(val_fan_off_dly >= val_commu_off_dly)
                {
                    s_clr_cm_status(x, CM_FAN); /* �ط�� */
            
                    if(off_dly >= val_fan_off_dly-val_commu_off_dly)
                    {
                        s_clr_cm_status(x, CM_VALVE_S); /* ����ͨ�� */
                    }
            
                    if(off_dly >= val_fan_off_dly)
                    {
                        s_clr_cm_status(x, CM_ON);  /* ��ѹ�� */
                    }
                    if(val_commu_off_dly<0 && off_dly >= val_fan_off_dly-val_commu_off_dly
                    || val_commu_off_dly>=0 && off_dly >= val_fan_off_dly)
                    {
                        Cm[x].stage = CM_IDLE;  //NOTE-CXW Ҫ���Դ���ת
                        Cm[x].dly = 0;
                    }
                    
                }
                else if(val_fan_off_dly < val_commu_off_dly)
                {
                    s_clr_cm_status(x, CM_VALVE_S); /* ����ͨ�� */
            
                    if(off_dly >= val_commu_off_dly-val_fan_off_dly)
                    {
                        s_clr_cm_status(x, CM_FAN); /* �ط�� */
                    }
            
                    if(off_dly >= val_commu_off_dly)
                    {
                        s_clr_cm_status(x, CM_ON);  /* ��ѹ�� */
                    }
                    if(val_fan_off_dly<0 && off_dly >= val_commu_off_dly-val_fan_off_dly
                    || val_fan_off_dly>=0 && off_dly >= val_commu_off_dly)
                    {
                        Cm[x].stage = CM_IDLE;  //NOTE-CXW Ҫ���Դ���ת
                        Cm[x].dly = 0;
                    }
                }
            }
                
            break;
            
        default:    // ��Ԥ֪״̬
            s_clr_cm_status(x, CM_VALVE_S); // ����ͨ��
            s_clr_cm_status(x, CM_ON);  // ��ѹ��
            s_clr_cm_status(x, CM_FAN); // �ط��
            break;          
    }
    
    if (s_get_cm_status(x, CM_ON)           // ��ѹ��
    && !s_get_cm_status(x, CM_DEFROST_OFF)  /* ��ѹ�� */
        ) 
    {
        s_set_output(x, SW_CM); 
    }
    
    if (s_get_cm_status(x, CM_VALVE_S)) // ����ͨ��
    {
        s_set_output(x, SW_S1);     
    }
}


/****************************************************************
������log_cmp_is_reoil()
���ܣ��жϻ����߼������ݵ͸�������ʱ����ơ�
������id
���أ�0--������ / ����ֵ--����ת��
�������Զת�ٻ���
*****************************************************************/
U16 log_cmp_is_reoil(U08 cm)
{   
    static BOOL fg_reoil = FALSE;       // ����״̬

    I16 freq_oil   = 0;                 //ѹ������ת��(0��ʾ�������)
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
        /* ������� */
        if (cnt_reoil_cycle > val_reoil_period && fg_TA_low)
        {
            fg_reoil = TRUE;
        }

        /* �˳����� */
        if (cnt_reoil_time > val_reoil_time)
        {
            fg_reoil = FALSE;
        }

        /* ���Ͷ��� */
        if (fg_reoil)
        {
            freq_oil = val_reoil_inventrps;
        }
    }
    else
    {
        /* ����̬���� */
        cnt_reoil_time  = 0;
        cnt_reoil_cycle = 0;
        fg_reoil        = FALSE;
    }

    /* ������ؼ�ʱ */
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
������void log_realrun_time(U08 s)
���ܣ�ѹ��������������¼�ʱ��
������U08 fun_fg--0:����ʱ��  ������������
                U08 comp_id--�ڼ���ѹ��
���أ���
������
*****************************************************************/
void log_realrun_time(U08 comp_id,U08 fun_fg)
{
    if(comp_id<MAX_COMP)  //��ֹ����
    {
        if(fun_fg==0) comp_realrun_time[comp_id] = 0;
    }

}

/*******************************************************************************
 �� ��: log_mod_set_avoid_freq()
 �� ��: �ر�Ƶ�����и�ֵ
 �� ��: avoid_freq[]--; 
        num--; 
        cm--; 
 �� ��: 
 �� ��: ��
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

#define AVOID_FREQ_MAX 4            /* �����Ƶ����Ŀ */
/*******************************************************************************
 �� ��: log_mod_freq_is_avoid()
 �� ��: �ж�һ��Ƶ���Ƿ��ڻر�Ƶ��������
 �� ��: freq--Ƶ��; 
 �� ��: TRUE��FALSE
 �� ��: ��
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
 �� ��: log_mod_avoid_freq()
 �� ��: ����ر�Ƶ��(���鹲��Ƶ�ʡ���Ƶ��)
 �� ��: freq_need--����Ƶ��; 
 �� ��: ���Ƶ��
 �� ��: 
    1������Ƶ��������Ӧ�������жϣ��ʴ˺��������ж�������
*******************************************************************************/
I16 log_mod_avoid_freq(I16 freq_need, U08 cm)
{ 
    I16 freq_out = freq_need;
    I16 range = val_avoid_freq_range;
    I16 avoid_freq[AVOID_FREQ_MAX];
    U08 i = 0;
    I16 temp_env = pv_cell_TA(pcb_addr);
    I16 temp_in = pv_cell_TWin1(pcb_addr);
    
    if (range == 0          /* ��Ϊ0��ʾ��ʹ����Ƶ���� */
    ||  freq_need == 0          
        )
    {
        freq_out = freq_need;
    }
    else
    {
        /* Ԥ���� */
        log_mod_set_avoid_freq(avoid_freq, AVOID_FREQ_MAX, cm);
        
        fun_sort_bubble(avoid_freq, ARRELE(avoid_freq), DIR_DCD);

        /* ��Ƶ���� */
        for (i=0; i<AVOID_FREQ_MAX; i++)
        {
            I16 avoid_min = avoid_freq[i]-(_DIVUCARRY(range, 2));
            I16 avoid_max = avoid_freq[i]+(_DIVUCARRY(range, 2));
            if (fun_pv_in_open(freq_out, avoid_min, avoid_max))
            {
                /* 2021-12-15-���Ӷ���Ƶ����ķ�Χ�ж� */
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
                    /* �����Ƶ�㽵������ʹ�� */
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
������U16 log_mod_comp_defrost_freq(void)
���ܣ���˪ʱѹ�������߼�
��������
���أ�ѹ������Ƶ��
������
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

    /* ����ͣ����˪״̬ */
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
    static I16 freq_before_reoil[MAX_COMP] = {COMP_INIT(SENSOR_NOEXIST)};   /* ����ǰת�� */
    I16 temp_env = pv_cell_TA(pcb_addr);
    I16 temp_in = pv_cell_TWin1(pcb_addr);
    
    for (x=0; x<self_comp_num; x++)
    {
        I16 freq_up = log_mod_get_freq_max(Cm[x].run_mode, temp_env, temp_in);
        I16 freq_dn = log_mod_get_freq_min(Cm[x].run_mode, temp_env, temp_in);
        I16 freq_rated = Cm[x].run_mode==MODE_COOL ? val_invt_rps_cool : val_invt_rps_heat; /* �ת�� */
        I16 freq_start = val_invt_start_freq;   /* ����ת�� */

        /* ����Ƶ�����ƣ���ʱ��ô�����������Ż�ʵ�� */
        freq_rated = fun_pv_limit(freq_rated, freq_dn, freq_up);
        freq_start = fun_pv_limit(freq_start, freq_start, freq_up); /* ֻ�������� */
        
        Logic_comp_action(x);    
        Logic_buway_vavle(x);   // ��Һ���߼�
        Logic_crank(x);
//        Logic_valve_defr(x);
    
//        if (val_inverter_used==USED && pcb_type==TYPE_MASTER && x==0)
        {
            
            BOOL fg_adjust_allow = FALSE;                   // ������������־
            BOOL fg_adjust_run = FALSE;                     /* ������ڱ�־ */
            BOOL fg_limit = FALSE;                          /* �����޽�Ƶ��־ */
            static BOOL fg_adjust_run_bak[MAX_COMP];
            BOOL fg_common_run = FALSE;                     /* �������б�־ */
            static BOOL fg_starting = FALSE;                /* �������̱�־ */
            U16 freq_start_dly_max;                         /* ����ά��ʱ�� */
            U16 freq_start_dly_1;                           /* ����ƽ̨1ʱ�� */
            U16 freq_rated_dly = val_freq_rated_time;       /* �ά��ʱ�� */
            INVT_ID_ENUM invt_id = (INVT_ID_ENUM)x;

            /* ����: ����ά��ʱ�䡢����ƽ̨1ʱ�� */
            if (invt_para_fa[invt_id][FA_FREQ_START_TIME]/10 <= 0)
            {
                freq_start_dly_max = freq_start_dly_1 = val_freq_stably_ctrl_time;
            }
            else
            {
                freq_start_dly_max = MAX((invt_para_fa[invt_id][FA_FREQ_START_TIME]/10), val_freq_stably_ctrl_time);   
                freq_start_dly_1 = invt_para_fa[invt_id][FA_FREQ_START_TIME]/10;
            }
                
            /* �޽�Ƶ��־���� */
            if (log_mod_freq_limit_check(x, Cm[x].run_mode))
            {
                fg_limit = TRUE;
            }
            
            if (s_get_cm_status(x, CM_ON))    
            {
                /* �������� */
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
                /* ������������־���� */
                if ((CmRotation[x]==freq_rated && invt_common_run_time[x] >= freq_rated_dly)
                 || fg_adjust_run_bak[x]
                 || invt_common_run_time[x] >= freq_start_dly_max
                 || SensorCanUse(freq_before_reoil[x])      /* ���ͽ������ػص��ת�� */
                 || fg_limit
                 )
                {
                    fg_adjust_allow = TRUE;
                    /* ��ʱδ�ָ����ת����ֱ�Ӹ��ת�� */
                    if (invt_common_run_time[x] >= freq_start_dly_max
                    &&  !fg_adjust_run_bak[x]
                        )
                    {
                        CmRotation[x] = freq_rated;
                    }
                }

                // �ж�ѹ���Ƿ���Ҫ����  
                ReOilFrequency[x] = log_cmp_is_reoil(x);
                
                /* ����ѹ��Ƶ�� */
                if (TF_OPEN_COMP)                               /* �̼� */
                {
                    CmRotation[x] = TF_comp_freq;
                }
                else if (Cm[x].stage==CM_DEFROSTING)            // ѹ����˪
                {
                    CmRotation[x]= log_mod_comp_defrost_freq(x);
                }
                else if(val_invt_manual_mode == USED)           // �ֶ�
                {
                    CmRotation[x] = val_invt_manual_rps;
                }
                else if (fg_ability_test_run)                   /* �������� */
                {
                    CmRotation[x] = AT_VAL(val_invt_manual_rps, val_ability_test_invt_rps);
                }
                else if (fg_refri_recy_act)                     /* �շ� */
                {
                    CmRotation[x] = freq_rated;
                }
                else if(ReOilFrequency[x]!=0)                   // ����
                {
                    if (CmRotation[x] != ReOilFrequency[x])
                    {
                        freq_before_reoil[x] = CmRotation[x];   /* ��¼����ǰת�� */
                    }
                    CmRotation[x] = ReOilFrequency[x];
                }
                else if (CmRotation[x]==0                       // ��������
                ||  fg_starting)
                {
                    static I16 freq_start_2;/* ����ƽ̨2ת�� */
                    if (cm_time[x].on <= freq_start_dly_1)
                    {
                        CmRotation[x] = MAX(freq_start, invt_info_freq_now(invt_id));
                        if (temp_env <= -2)
                        {
                            freq_start_2 = val_low_env_start_freq;	/* ���ͻ�����ά��ת�١� */
                        }
                        else
                        {
                            freq_start_2 = val_invt_start_freq2;	/* ��ѹ������ά��ת�١� */
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
                    /* �������� */
                    fg_common_run = TRUE;
                    
                    if (!fg_adjust_allow)
                    {
                        I16 freq_common_first = MAX(freq_abnormal[x], freq_start);  /* ���쳣�е���������ʼƵ�� */
                        CmRotation[x] = MIN(freq_common_first + MIN(30, _DIVUCARRY(invt_common_run_time[x], val_invt_start_period))*val_invt_start_speed, freq_rated);
                    }
                    else
                    {
                        /* ������� */
                        fg_adjust_run = TRUE;

                        /* ���ͽ����ص�����ǰת�� */
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

            /* ѹ��δ�����������еĴ��� */
            if (!fg_common_run)
            {
                invt_common_run_time[x] = 0;
                log_realrun_time(x, 0);
                freq_abnormal[x] = CmRotation[x];
                fg_freq_pre_sub_en = TRUE;
                fg_freq_had_force_sub = FALSE;
            }

            /* ѹ��δ������ڵĴ��� */
            if (!fg_adjust_run)
            {
                pid_set_AveAcrossCnt(PID_ID_INVT_1, 0);
                pid_read_Uk(PID_ID_INVT_1);                     /* ��ȡ�����Uk */
            }

            fg_adjust_run_bak[x] = fg_adjust_run;
        }
    }
    
    /* ��� */
    fan_logic();
    ec_fan();           /* EC����������������� */
    
    log_mod_comp_status();
}

/*******************************************************************************
 �� ��: log_mod_unit_to_comp_group()
 �� ��: ͨ����Ԫ�Ż�ȡѹ����
 �� ��: n_comp--һ����Ԫ������ѹ����;
        begin--ѹ����ʼ���;
        end--ѹ���������;
        exio_no--��Ԫ��;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_mod_unit_to_comp_group(U08 n_comp, U08 *begin, U08 *end, U08 exio_no)
{
    *begin = exio_no * n_comp;
    *end = MIN(*begin+n_comp, val_comp_num);            /* end���ֻ��Ϊval_comp_num */
}

/*******************************************************************************
 �� ��: log_mod_cmd_by_hmi()
 �� ��: ��ʾ�������
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void log_mod_cmd_by_hmi(void)
{   
    /* ���´����ֲ������� */
    if (!(pCell->cell_status[MAX_COMP] & CELL_RUN))      /*  ģ����ѹ������ʱ�ſɳ�ʼ�� */
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

    
    if (fg_runtime_init)                                 /* �ۼ�����ʱ���ʼ�� */
    {
        fg_runtime_init = FALSE;
        DataAccess_CompRuntime(DATA_INIT);              // ��ʼ��ѹ������ʱ��
    }
    
    if (fg_systime_init)                                 /* ϵͳ�ۼ�����ʱ���ʼ�� */
    {
        fg_systime_init = FALSE;
//      systime_initial();
        DataAccess_SysRuntime(DATA_INIT);
    }
    if (fg_systime_save)                                 /* ϵͳ�ۼ�����ʱ�䱣�� */
    {
        fg_systime_save = FALSE;
//      systime_write_to_eeprom();
        DataAccess_SysRuntime(DATA_WRITE);
    }

    
    if (fg_password_init)                                 // �����ʼ��
    {
        fg_password_init = FALSE;
        Password_InitAll();
    }
    

}

/*******************************************************************************
 �� ��: s_set_di()
 �� ��: ѹ�����������ǳ������պ�Ŀ���������
 �� ��: input ���������  di_para �������ղ���
 di_para: 0 ���� �� 1 ����
 �� ��: ��
 �� ��: 
*******************************************************************************/
void s_set_di(U08 input, I16 di_para, U08 comp_per)
{
    if (s_get_input_first(comp_per, input) ^ di_para)
    {
        s_set_input(comp_per, input);
    }
}

/*******************************************************************************
 �� ��: log_mod_begin_pv_cpst()
 �� ��: �����¶Ⱥ󲹳�
 �� ��: ��
 �� ��: ��
 �� ��: 
*******************************************************************************/
void log_mod_begin_pv_cpst(void)
{
    /* ˮ�²���ֵ������������ */
    if (!fg_idle_sta_antifreeze				/* �Ǵ���̬���� */
    && !MOD_IS_RUN_STA_AC_AF(pcb_addr)		/* ������̬�յ�ˮ·���� */
    && !MOD_IS_RUN_STA_HOTW_AF(pcb_addr)	/* ������̬��ˮˮ·���� */
    )
    {
        I16 temp_cpst = CmIsCool(0) ? val_ctrl_cpst_cool : val_ctrl_cpst_heat;

        pv_cell_TWin1(pcb_addr) = fun_pv_cmpst(pv_cell_TWin1(pcb_addr),  temp_cpst);
        pv_cell_TWout1(pcb_addr) = fun_pv_cmpst(pv_cell_TWout1(pcb_addr),  temp_cpst);
    }

#if 0
    /* ��ˮ�²���ֵ */
    pv_cell_TWin1(pcb_addr) = log_sys_circle_water_cpst(pv_cell_TWin1(pcb_addr));
    pv_cell_TWout1(pcb_addr) = log_sys_circle_water_cpst(pv_cell_TWout1(pcb_addr));
#endif
}

/*****************************************************************************
��  �� : log_mod_limit()
��  �� : ������������ģ����������,�ͼ����������
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
static void log_mod_limit(void)
{
    U08  i;
    
    for (i=0; i<self_comp_num; i++)
    {
//        /* ��ѹ */
//        if (val_J6_prs_sensor_type == UNUSED)
//        {
//            pv_cell_hp(i)     = SENSOR_NOEXIST;            
//            pv_cell_cond_temp(i)   = SENSOR_NOEXIST;
//        }
//        /* ��ѹ */
//        if (val_J5_prs_sensor_type == UNUSED) 
//        {
//            pv_cell_lp(i)      = SENSOR_NOEXIST;
//            pv_cell_evap_temp(i)   = SENSOR_NOEXIST;
//        }

#if defined(M_DISEN)
        /* �����¶� */
        if (USE_COOL_LP || val_s_motor_use == EVI_ECO_OVERHEAT)
        {
            pv_cell_evap(i) = SENSOR_NOEXIST;
        }

        /* �����������¶�&����������¶� */
        if (val_s_motor_use != EVI_ECO_OVERHEAT)
        {
            pv_cell_evi_in(i)  = SENSOR_NOEXIST;
            pv_cell_evi_out(i) = SENSOR_NOEXIST;
        }
#else
        /* �����¶� */
        if (USE_COOL_LP)
        {
            pv_cell_evap(i) = SENSOR_NOEXIST;
        }

        pv_cell_evi_in(i)  = SENSOR_NOEXIST;
        pv_cell_evi_out(i) = SENSOR_NOEXIST;
#endif

        /* ����������� */
        if(val_s_motor_use == EVI_UNUSED)
        {
            pv_cell_step_s(i) = SENSOR_NOEXIST;
        }
        if (val_motor_use == EEV_UNUSED)
        {
            pv_cell_step_m(i) = SENSOR_NOEXIST;
            pv_cell_gas_in(i) = SENSOR_NOEXIST;
        }

        /* �ڻ��̼�ʱǿ�Ƽ���⼸���¶� */
        if (!(TF_HAS_INNER&&TF_EN_NTC)) 
        {
            /* ��ʹ����ýҺ���¶ȡ���ʹ����ý�����¶� */
            if (val_rfg_temp_corr_en == UNUSED)
            {
                pv_cell_TRFliq(i) = SENSOR_NOEXIST;
                pv_cell_TRFgas(i) = SENSOR_NOEXIST;
            }
        }

//        /* ������� */
//        if (NO_CUR_SENSOR)
//      {
//            pv_cell_curr(i) = SENSOR_NOEXIST;
//      } 

//        /* ѹ���¶� */
//        if (val_s_motor_use==EVI_ECO_OVERHEAT
//         || !val_seneor_use_comp_top
//            )
//        {
//            pv_cell_comp_top(i) = SENSOR_NOEXIST;
//        }

//        // ������
//        if (val_s_motor_use != EVI_ECO_OVERHEAT
//        &&  val_seneor_disp_evi_temp == UNUSED)
//      {
//            pv_cell_evi_in(i) = SENSOR_NOEXIST;
//            pv_cell_evi_out(i) = SENSOR_NOEXIST;
//        }
    }    
    
//  /* ��Ԫ */  /* N0004 */
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

    /* ��ʹ�ñ�Ƶˮ�� */
    if (USE_PUMPf_CTRL == PUMPF_CTRL_OFF)
    {
        pv_cell_PUMPf_set(pcb_addr) = SENSOR_NOEXIST;
        pv_cell_PUMPf_cur(pcb_addr) = SENSOR_NOEXIST;
        pv_cell_PUMPf_pow(pcb_addr) = SENSOR_NOEXIST;
    }

    /* ��Ƶˮ�þ��治��ʾ���� */
    if (PUMPF_is_warn())
    {
        pv_cell_PUMPf_cur(pcb_addr) = SENSOR_NOEXIST;
        pv_cell_PUMPf_pow(pcb_addr) = SENSOR_NOEXIST;
    }
}

/*******************************************************************************
 �� ��: log_mod_get_press()
 �� ��: ѹ����ȡ&ת��
 �� ��: ��
 �� ��: ��
 �� ��: ��
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
                
    pv_cell_lp(0) = fun_pv_div_10(lp);              /* ��ѹ */
    pv_cell_evap_temp(0) = fun_pv_div_10(lp_temp);  /* ���������¶� */

    pv_cell_hp(0) = fun_pv_div_10(hp);              /* ��ѹ */
    pv_cell_cond_temp(0) = fun_pv_div_10(hp_temp);  /* ���������¶� */
}


static void log_mod_exio_begin(void)
{    
    if (GET_EXIO_HDW_STATUS_READY(0))       /* ״̬��Ϣ���� */
    {
        /* ������ԭʼֵ���� */
        if (GET_EXIO_DI(0, EXIO_DI_02))  s_set_input_first(0, IN_LP);   /* 1#ѹ����ѹ */
        if (GET_EXIO_DI(0, EXIO_DI_01))  s_set_input_first(0, IN_HP);   /* 1#ѹ����ѹ */
    
        /* ���������� */
        s_set_di(IN_LP, val_DI_type_lp, 0);                             /* 1#ѹ����ѹ */     
        s_set_di(IN_HP, val_DI_type_hp, 0);                             /* 1#ѹ����ѹ */     
    
        /* ѹ�� */
        log_mod_get_press();
    
        pv_cell_TA(pcb_addr) = GET_EXIO_TMP(0, EXIO_TEMP_01);       /* �����¶� */
        pv_cell_wing(0)      = GET_EXIO_TMP(0, EXIO_TEMP_02);       /* ��Ƭ�¶� */
        pv_cell_gas_out(0)   = GET_EXIO_TMP(0, EXIO_TEMP_04);       /* �����¶� */
        pv_cell_gas_in(0)    = GET_EXIO_TMP(0, EXIO_TEMP_03);       /* �����¶� */
        pv_cell_evap(0)      = GET_EXIO_TMP(0, EXIO_TEMP_05);       /* �����¶� */
        pv_cell_evi_in(0)    = GET_EXIO_TMP(0, EXIO_TEMP_05);       /* ����������¶�,����ԭ�����¶� */
        pv_cell_evi_out(0)   = GET_EXIO_TMP(0, EXIO_TEMP_06);       /* �����������¶� */
        pv_cell_step_m(0)    = GET_EXIO_ST_PV(0, EEV_ID_1);         /* �������� */
        pv_cell_step_s(0)    = GET_EXIO_ST_PV(0, EEV_ID_2);         /* �������� */
    }
    else                    /* ͨѶ���ϣ�״̬��Ϣ������ʱ�������ģ���� */
    {   
        /* ������: �ѱ�ͳһ��� */

        /* ѹ�� */
        pv_cell_lp(0)        = SENSOR_NOEXIST;              /* ��ѹ */
        pv_cell_evap_temp(0) = SENSOR_NOEXIST;              /* ���������¶� */        
        pv_cell_hp(0)        = SENSOR_NOEXIST;              /* ��ѹ */
        pv_cell_cond_temp(0) = SENSOR_NOEXIST;              /* ���������¶� */

        pv_cell_TA(pcb_addr) = SENSOR_NOEXIST;              /* �����¶� */
        pv_cell_wing(0)      = SENSOR_NOEXIST;              /* ��Ƭ�¶� */
        pv_cell_gas_out(0)   = SENSOR_NOEXIST;              /* �����¶� */
        pv_cell_gas_in(0)    = SENSOR_NOEXIST;              /* �����¶� */
        pv_cell_evap(0)      = SENSOR_NOEXIST;              /* �����¶� */
        pv_cell_evi_in(0)    = SENSOR_NOEXIST;              /* ����������¶� */
        pv_cell_evi_out(0)   = SENSOR_NOEXIST;              /* �����������¶� */
        pv_cell_step_m(0)    = SENSOR_NOEXIST;              /* �������� */
        pv_cell_step_s(0)    = SENSOR_NOEXIST;              /* �������� */      
    }
}
 
/*******************************************************************************
 �� ��: log_mod_begin()
 �� ��: ģ���߼���ʼ
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void log_mod_begin(void)
{
    U08 i;

    for (i=0; i<MAX_COMP+1; i++)
    {
        mod_input[i] = 0;           /* ����������ˢ��(���������պ��) */
        pCell->cell_relay[i] = 0;   /* �̵����������� */
        pCell->cell_input[i] = 0;   /* ����������ˢ��(������ԭʼֵ) */
    }

    /* ����� */
    log_mod_exio_begin();

    /* ����̼�ʱ������ڻ������� */
    if (!TF_IS_OUT)
    {
        /* ������ԭʼֵ���� */
        /*-------����λ��--------; --------�߼�λ��-------*/
        if (GET_DI(DI_01))  s_set_input_first(MAX_COMP, S_IN_WATER_FLOW); /* ˮ������ */
        if (GET_DI(DI_08))  s_set_input_first(MAX_COMP, S_IN_IBH_OL);     /* �������ȷ��� */

        /* ���������� */
        s_set_di(S_IN_WATER_FLOW, val_DI_type_air_flux, MAX_COMP);        /* ˮ������ */
        s_set_di(S_IN_IBH_OL, NORMAL_CLOSE, MAX_COMP);			          /* �������ȷ���(�̶�����) */
    }
    
    
    
    /* ��Ԫģ���� */
    pv_cell_TWin1(pcb_addr)  = GET_TEMP(TEMP_09);           /* �廻��ˮ�¶� */
    pv_cell_TWout1(pcb_addr) = GET_TEMP(TEMP_08);           /* �廻��ˮ�¶� */
	if (val_TWout2_sensor_used)
	{
    	pv_cell_TWout2(pcb_addr) = GET_TEMP(TEMP_07);           /* �ܳ�ˮ�¶� */
	}
	else
	{
    	pv_cell_TWout2(pcb_addr) = GET_TEMP(TEMP_08);           /* �ܳ�ˮ�¶�̽ͷ�����óɽ��ã�ʹ�ð廻��ˮ�¶��滻 */
	}

    pv_cell_PUMPf_set(pcb_addr)      = PUMPf_rpm_sv;		/* ��Ƶˮ��Ŀ��ת�� */
    pv_cell_PUMPf_cur(pcb_addr)      = PUMPf_flow_pv;		/* ��Ƶˮ�õ�ǰ���� */
    pv_cell_PUMPf_pow(pcb_addr)      = PUMPf_power_pv;		/* ��Ƶˮ�õ�ǰ���� */
    pv_cell_PUMPf_pwm_out(pcb_addr)  = PUMPf_pwm_out;		/* ��Ƶˮ��PWM�����ź�(��->��) */
    pv_cell_PUMPf_pwm_in(pcb_addr)   = PUMPf_pwm_in;		/* ��Ƶˮ��PWM�����ź�(��->��) */
    pv_cell_PUMPf_err_code(pcb_addr) = PUMPf_err_id;		/* ��Ƶˮ�ù�����Ϣ */
    
    {
        /* ����궨���� & ���鵱ǰ���� */
        I16 water_diff = pv_cell_TWout1(pcb_addr) - pv_cell_TWin1(pcb_addr); 
        
        if (ABILITY_TEST_HAS_DEFR)
        {
            if (motor_run_time[0] < 15*60)      /* ѹ�����е�15minʱȷ������ */
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

    /* ѹ��ģ���� */
    pv_cell_TRFgas(0) = GET_TEMP(TEMP_06);  /* ��ý�����¶� */
    pv_cell_TRFliq(0) = GET_TEMP(TEMP_05);  /* ��ýҺ���¶� */

    //����ٶȸ���
    if (val_freq_fan_set == FREQ_FAN_SET_PWM)
    {
        
        pv_cell_fan_now(FREQ_FAN1) = speedpv[FREQ_FAN1];
        pv_cell_fan_now(FREQ_FAN2) = speedpv[FREQ_FAN2];

        pv_cell_fan_set(FREQ_FAN1) = speedsv[FREQ_FAN1];
        pv_cell_fan_set(FREQ_FAN2) = speedsv[FREQ_FAN2];
    }
    else if (val_freq_fan_set == FREQ_FAN_SET_PWM_EX)
    {
//        /* rpm = pulse �� ÿת������ �� 60 */
        pv_cell_fan_now(FREQ_FAN1) = (U32)invt_info_pulse_ec1(INVT_ID_1)*60/val_fan_pwm_pulse;
        pv_cell_fan_now(FREQ_FAN2) = (U32)invt_info_pulse_ec2(INVT_ID_1)*60/val_fan_pwm_pulse;

//        pv_cell_fan_now(FREQ_FAN1) = invt_info_pulse_ec_filt(INVT_ID_1, FREQ_FAN1);
//        pv_cell_fan_now(FREQ_FAN2) = invt_info_pulse_ec_filt(INVT_ID_1, FREQ_FAN2);

        pv_cell_fan_set(FREQ_FAN1) = speedsv[FREQ_FAN1];
        pv_cell_fan_set(FREQ_FAN2) = speedsv[FREQ_FAN2];
    }
    else if (val_freq_fan_set == FREQ_FAN_SET_EC)
    {
        /* ������ʾΪ1RPM: 1RPS = 60RPM */
        pv_cell_fan_now(FREQ_FAN1) = (U32)invt_info_freq_now(INVT_FAN1)*60/100;
        pv_cell_fan_now(FREQ_FAN2) = (U32)invt_info_freq_now(INVT_FAN2)*60/100;
        
        pv_cell_fan_set(FREQ_FAN1) = speedsv[FREQ_FAN1];
        pv_cell_fan_set(FREQ_FAN2) = speedsv[FREQ_FAN2];
    }
    ec_fan_rd_update();

    /* ��Ƶ��Ϣ */
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
                pv_cell_invt_set(id)   = CmRotation[id]/10;            // ѹ��Ŀ��ת��(0.1rps)   
                pv_cell_invt_cur(id)   = invt_info_freq_now(id)/10;    // ѹ����ǰת��(0.1rps)
                pv_cell_invt_volt(id)     = invt_info_voltage_out(id);    // ��ѹ   
                pv_cell_invt_curr(id)     = invt_info_current_out(id);    // ����   
                pv_cell_invt_tmp(id)       = invt_info_temperature(id);    // ��Ƶ���¶�
                pv_cell_invt_msg(id)      = invt_info_freq_limit_id(id);  // ��Ƶ����Ƶ��Ϣ     
                pv_cell_invt_curr_in(id)   = invt_info_errorcode1_id(id);  // ��Ƶ������� 
                pv_cell_invt_power_out(id) = invt_info_powerout(id);       // ��Ƶ�������   
            }                          
            else                       
            {                          
                pv_cell_invt_set(id)        = SENSOR_NOEXIST; // ѹ��Ŀ��ת��(0.1rps)    
                pv_cell_invt_cur(id)        = SENSOR_NOEXIST; // ѹ����ǰת��(0.1rps)
                pv_cell_invt_volt(id)       = SENSOR_NOEXIST; // ��ѹ       
                pv_cell_invt_curr(id)       = SENSOR_NOEXIST; // ����     
                pv_cell_invt_tmp(id)        = SENSOR_NOEXIST; // ��Ƶ���¶�    
                pv_cell_invt_msg(id)        = SENSOR_NOEXIST; // ��Ƶ����Ƶ��Ϣ 
                pv_cell_invt_curr_in(id)    = SENSOR_NOEXIST; // ��Ƶ������� 
                pv_cell_invt_power_out(id)  = SENSOR_NOEXIST; // ��Ƶ������� 
            }
        }
        
        for (id=INVT_ID_1; id<INVT_ID_MAX; id++)
        {
            CELL_INVT_INFO(pcb_addr, id,  0) = invt_info_freq_state(id);
            CELL_INVT_INFO(pcb_addr, id,  1) = invt_info_freq_run(id)/10;   // ѹ��Ŀ��ת��(0.1rps)  ;
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

//    /* ģ�鹦�� */
//    pv_cell_power(pcb_addr) = pv_cell_invt_power_out(INVT_ID_1);

    /* ��Ƶˮ��״̬ */
    PUMPF_status();

    /* ����ѹ�����·ֶ� */
    limit_comp_TA_area = log_mod_limit_comp_TA_area(ctrl_mode[GET_SUB_SYS(pcb_addr)], GET_SUB_SYS(pcb_addr));
    
    /* ���Ȼ��·ֶ� */
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

    /* ������·ֶ� */
    in_area_cool = fun_area(pv_cell_TWin1(pcb_addr), in_area_cool, 5, 
                            val_cool_sv_chg_in1, val_cool_sv_chg_in2,
                            SENSOR_NOEXIST);    

    log_mod_limit();

    /* �¶Ⱥ󲹳� */
    log_mod_begin_pv_cpst();

	/* ģ����û�ͨѶ����ʱ�������ع㲥�������ݣ���ֹӰ��ӻ� */
	if (!CELL_Is_used(pcb_addr) || self_commu_err)
	{
		log_sys_init_idle_sta_antifreeze();
		log_sys_init_run_sta_antifreeze();
		log_sys_init_timed_antifreeze();	
	}	

    /* ������Ϣ���� */
    {
        ARRCLR(fan_adjust_fg);
        ARR2SET(fan_adjust_tmp, 0);
        ARR2SET(eev_adjust_tmp, 0);
        ARR2SET(evi_adjust_tmp, 0);
    }
}

/*******************************************************************************
 �� ��: log_mod_exio_end()
 �� ��: ����������
 �� ��: ��
 �� ��: 
 �� ��: ��
*******************************************************************************/
static void log_mod_exio_end(void)
{
    /* ͨѶ���ϣ�״̬��Ϣ������ʱ���������� */
    if (!GET_EXIO_HDW_STATUS_READY(0))
    {
        s_clr_output(0, SW_byway);
        s_clr_output(0, SW_S1);
        s_clr_output(MAX_COMP, S_DO_DPH);
        s_clr_output(MAX_COMP, S_DO_DPH2);
        s_clr_output(0, SW_crank2);
        s_clr_output(0, SW_crank1);
    }
    
    /* ��Һ�� */
    if (s_get_output(0, SW_byway))
    {
        SET_EXIO_DO(0, EXIO_DO_03); 
    }
    else
    {
        CLR_EXIO_DO(0, EXIO_DO_03);
    }
    
    /* ��ͨ��  */
    if (s_get_output(0, SW_S1))
    {
        SET_EXIO_DO(0, EXIO_DO_04); 
    }
    else
    {
        CLR_EXIO_DO(0, EXIO_DO_04);
    }
    
    /* ���̵���� */
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
                /* ��������2 */
                if (s_get_output(0, SW_crank2))         fg_exdo7 = TRUE;
                break;
                
            case EXDO7_IS_DPH2:
                /* ���̵����2 */
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
        
    /* ��������1 */
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
������Logic_OutputChange
���ܣ��������ת���������״̬���������������
������ģ�����
����ֵ����
*************************************************/
static void log_mod_end(void)
{
	U08 i;
    SET_DO(DO_13);          /* IBH/TBH�����ܿ��� */

    SHC_set_mod_test_do();

	/* ģ����û�ͨѶ����ʱ���ر�ģ����� */
	if (!CELL_Is_used(pcb_addr) || self_commu_err)
	{
		for (i=0; i<MAX_COMP+1; i++)
		{
        	pCell->cell_relay[i] = 0;   /* �̵����������� */
			pCell->cell_status[i] = 0;   /* �̵����������� */
		}
	}

    /* ��Ƶѹ�� */
    {
        INVT_ID_ENUM id = INVT_ID_1;
        //��ͣ��Ƶ��
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

        //����ѹ��ת�ٸ���Ƶ��
        CmRotation_out[id] = log_mod_avoid_freq(CmRotation[id], id);
        invt_set_rps_out(id,CmRotation_out[id]);

        /* pfc���ã�����ģʽʱ������ƣ��˳�����ģʽʱ�ָ�pfc���� */
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

    // EC���/* N0004 */
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

            // ����ת��
            {
                U16 speed_sv = (I32)speedsv[id-INVT_FAN1+FREQ_FAN1]*100/60; /* 1rpm->0.01rps */
                invt_set_rps_out(id, speed_sv);
            }
        }
    }
    else if (val_freq_fan_set == FREQ_FAN_SET_PWM_EX)   /* ����ec���ת�ٸ���Ƶ�� */
    {
        U08 i = 0;

        for (i=0; i<FREQSPEEDNUM; i++)
        {
            speed_percent[i] = pwm_fan_speed_to_percent(i, speedsv[i], pv_cell_fan_now(i));
            invt_set_rps_ec(INVT_ID_1, i, speed_percent[i]);
        }
    }
    else if (val_freq_fan_set == FREQ_FAN_SET_PWM)          /* ����PWM��� */
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
//      SET_EXIO_DO(0, EXIO_DO_02); // ���ٷ�� 
//  }
//  else
//  {
//      CLR_EXIO_DO(0, EXIO_DO_02); // ���ٷ�� 
//  }

    /* ��Ƶˮ�� */
    if (s_get_output(MAX_COMP, S_DO_PUMPf))             
    {
        SET_DO(DO_01);
    }

    /* ��������δ���� */
    if (!s_get_input(MAX_COMP, S_IN_IBH_OL))
    {
        /* ��������1 */
        if (s_get_output(MAX_COMP, S_DO_IBH1))
        {
            SET_DO(DO_05);
        }
    
        /* ��������2 */
        if (s_get_output(MAX_COMP, S_DO_IBH2))
        {
            SET_DO(DO_06);
        }
    }

    /* ���������ȴ� */
    if (s_get_output(MAX_COMP, S_DO_HT))
    {
        SET_DO(DO_08);
        SET_DO(DO_14);
    }

    /* �������� */
    log_mod_exio_end();
}

/******************************************************
������Logic_Reset
���ܣ�����������������ʱ�ĸ�λ
��������
��������
*******************************************************/
//static void s_Logic_idle(void)
//{
//  U08 x, y;
//  
//  if (fg_freeze_ac_act || fg_wt_antifreeze_action)   /* ����ʱ����ʼ�����±��� */
//  {
//      return;
//  }
//
//  /* ѹ����ʼ�� */
//  for (x=0; x<MAX_COMP+1; x++)
//  {
//      Cm[x].status = 0;
//      
//      Cm[x].run_mode = 0xff;         /* ����ģʽ��ʼ�� */
//      Cm[x].run_mode_need = 0xff;         /* ����ģʽ�����ʼ�� */
//          
//  }
//  
//  Cm[MAX_COMP].fan_speed = 0;         /* ���ٳ�ʼ�� */
//  cnt_fan_force_dly = 0;
//  
//  for (x=0; x<MAX_COMP; x++)
//  {
//      frost_space[x] = 0;
//      cnt_vavle_delay[x] = 0xff;  /* ���رպ���ʱ��ʼ�� */
//      fg_defrost_comp_stop[x] = 0; /* ��˪ʱ��ѹ��ͣ����־��ʼ�� */
//      fg_first_dfrost[x] = 0;
//  }
//}

/*******************************************************************************
 �� ��: log_mod_comp_quick_stop()
 �� ��: ��ͣѹ��
 �� ��: comp_num--ѹ�����; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
//void log_mod_comp_quick_stop(U08 comp_num)
//{
//
//    if (s_get_cm_status(comp_num, CM_NEED))      /* ѹ���������ر�ѹ��������ʱ�ط� */
//  {
//      s_clr_cm_status(comp_num, CM_NEED);
//      
//      //s_clr_cm_status(comp_num, CM_NEED_DEFROST);        /* ��˪���� */
//        //s_clr_cm_status(comp_num, CM_DEFROST);        /* ��˪��־ */ 
//        
//        //s_clr_cm_status(comp_num, CM_VALVE_S);    /* ����ͨ�� */
//  }
//  else                               /* ѹ��δ������ֱ�ӹط� */
//  {
//      if (s_get_cm_status(comp_num, CM_VALVE_S))
//      {
//              //s_clr_cm_status(comp_num, CM_VALVE_S);    /* ����ͨ�� */
//          }
//  }
//
////    fg_first_dfrost[comp_num] = 0;  /* ��ѹ���״γ�˪��־ */
//  fg_cell_run = 0;
//
//
//
//}
/*******************************************************************************
 �� ��: log_mod_comp_stop()
 �� ��: ͣѹ��
 �� ��: comp_num--ѹ�����; stop_type--ͣѹ������
 �� ��: ��
 �� ��: ��
*******************************************************************************/
#define NORMAL_STOP             0   // ����ͣ����ϵͳ���������ͨ��������ʱ����
#define QUICK_ONE_COMP_STOP     1   // ��ͣ1��ѹ��
#define QUICK_ONE_CIRCLE_STOP   2   // ��ͣ1������ϵͳ
#define QUICK_ALL_STOP          3   // ��ͣ��ģ����������ϵͳ

void log_mod_comp_stop(U08 comp_num,U08 stop_type)   
{
    U08 i;
    
    if(stop_type == NORMAL_STOP)    // ����ͣ����ϵͳ
    {
        if (s_get_cm_status(comp_num, CM_NEED))      /* ѹ����������ر�ѹ������ͨ���ͷ����ʱ��˳���� */
        {
            s_clr_cm_status(comp_num, CM_NEED);
        }
    }
    else if(stop_type == QUICK_ONE_COMP_STOP)   // ��ͣ1ѹ��    
    {
        s_clr_cm_status(comp_num, CM_NEED);
        s_clr_cm_status(comp_num, CM_ON);       
    }
    else if(stop_type == QUICK_ONE_CIRCLE_STOP) // ��ͣ1����ϵͳ    
    {
        s_clr_cm_status(comp_num, CM_NEED);
        Cm[comp_num].stage = CM_IDLE;       
    }
    else if(stop_type == QUICK_ALL_STOP)    // ��ͣ��ģ����������ϵͳ   
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
 �� ��: log_evi_sw()
 �� ��: ���ʷ�
 �� ��: x--; 
        control_type--����������; 
 �� ��: 
 �� ��: ��
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
 �� ��: log_mod_cmd()
 �� ��: �������������
 �� ��: ��
 �� ��: ��
 �� ��: ��
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
        
        if (cmd & CMD_STOP_ALL_COMP)      /* ͣ��(���չ㲥��Ϣ, ��ͣ) */
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

        if (cmd & CMD_STOP_ALL_MAC)       /* ͣ�����������(���չ㲥��Ϣ, ��ͣ) */
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
        {   /* ģʽ */
            for (i=0; i<self_comp_num; i++)
            {
                Cm[i].run_mode_need = ((cmd >> CMD_MODE) & 0x0007);
            }
            Cm[MAX_COMP].run_mode_need = ((cmd >> CMD_MODE) & 0x0007);   
        }
        
        if (cmd & CMD_ERR_RESET)          /* ��λ */
        {   
            fg_err_reset = TRUE;
        }
        if (cmd & CMD_FAC_PARA_INIT)      /* ������ʼ�� */
        {   
            fg_para_init = TRUE;
        }
        if (cmd & CMD_INVT_INIT)      /* ��Ƶ����ʼ�� */
        {   
            fg_invt_init = TRUE;
        }
        if (cmd & CMD_LIMIT_INIT)      /* ϵͳά��������ʼ�� */
        {   
            fg_limit_init = TRUE;
        }
        if (cmd & CMD_RUNTIME_INIT)   /* �ۼ�����ʱ���ʼ�� */   
        {   
            fg_runtime_init = TRUE;
        } 
        if (cmd & CMD_SYSTIME_INIT)   /* ϵͳ�ۼ�����ʱ���ʼ�� */
        {   
            fg_systime_init = TRUE;
        }
        if (cmd & CMD_SYSTIME_SAVE)   /* ϵͳ�ۼ�����ʱ�䱣�� */
        {   
            fg_systime_save = TRUE;
        }
        if (cmd & CMD_PASSWORD_INIT)   // �����ʼ��
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
            {   /* ģʽ */
                Cm[i].run_mode_need = ((cmd >> CMD_CM_MODE) & 0x07);
            }
            
            if (cmd & CMD_STOP_COMP_SYSTEM)     /* ѹ�������������ͨ����ʱ��ͣ */
            { /* ͣѹ��ϵͳ */
//              log_mod_comp_quick_stop(i);
                log_mod_comp_stop(i,NORMAL_STOP);
            }
            else if (cmd & CMD_QUICK_STOP_COMP) /* ѹ����ͣ�������ʱͣ����ŷ���ͣ */
            { /* ��ͣ */
////                s_set_cm_status(i, CM_QUICK_STOP);
//              s_clr_cm_status(i, CM_NEED);
//              s_clr_cm_status(i, CM_ON);
                log_mod_comp_stop(i,QUICK_ONE_COMP_STOP);
            }
            else if (cmd & CMD_CLOSE_COMP)      /* ѹ��������Ҫ��ʱͣ�������ʱͣ����ŷ���ͣ */
            { /* �� */
                if (log_mod_comp_can_close(i))        /* ѹ�����ɹ� */
                {
//                  s_clr_cm_status(i, CM_NEED);
                    log_mod_comp_stop(i,NORMAL_STOP);
                }

            }
            else if (cmd & CMD_OPEN_COMP)       /* ѹ��������Ҫ��ʱ�� */
            { /* �� */                
                if (log_mod_comp_can_open(i))         /* ѹ�����ɿ� */
                {
                    s_set_cm_status(i, CM_NEED);
                }
            }

            if (cmd & CMD_ALLOW_DEFROST) 
            { /* ��˪���� */
                s_set_cm_status(i, CM_DEFROST_ALLOW);
            }
            if (cmd & CMD_FORCE_DEFROST) 
            { /* ǿ�Ƴ�˪ */
                SIGNAL_Launch(&defrost_signal_into_namual[i], TRUE);
//              Defrost_force(i);
            }
        }
        
        Cm[i].run_mode = Cm[i].run_mode_need;

        /* �źź��� */
        SIGNAL_LaunchEnd(&defrost_signal_into_namual[i]);
    }
    Cm[MAX_COMP].run_mode = Cm[MAX_COMP].run_mode_need;  
    log_mod_cmd_by_hmi();               //��ʾ������
}

/*******************************************************************************
 �� ��: log_mod_comp_eco_get_coef()
 �� ��: ��ȡ�������ȶ�ϵ��a
 �� ��: eva_side_temp--�����¶�; 
 �� ��: ϵ��a
 �� ��: �Ź�sv = k*��ˮ + a
*******************************************************************************/
I16 log_mod_comp_eco_get_coef(I16 eva_side_temp)
{
    I16 result;
    
    if (SensorCannotUse(eva_side_temp))   /* ������̽ͷ���ϣ�ȡ�̶����ȶ�Ŀ��ֵ */
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
 �� ��: log_mod_exh_oh_sv_limit()
 �� ��: �������ȶ�Ŀ��ֵ��Χ����
 �� ��: x--ѹ����; 
		sv--ԭʼ�������ȶ�Ŀ��ֵ; 
 �� ��: ���ƺ��������ȶ�Ŀ��ֵ
 �� ��: ��
*******************************************************************************/
I16 log_mod_exh_oh_sv_limit(U08 x, I16 sv)
{
    I16 pid_sv = 0;
    
    #define TEMP_EXH_OBJ_MIN (55*10)                // ���Ŀ������
    #define TEMP_EXH_OBJ_MAX (val_exh_sv_max)       // ���Ŀ������
    
    I16 temp_cond = log_mod_get_temp_cond(x);  /* �����¶� */

    pid_sv = fun_pv_limit(sv, TEMP_EXH_OBJ_MIN-temp_cond,
                          TEMP_EXH_OBJ_MAX-temp_cond);

    return pid_sv;
}
/*******************************************************************************
 �� ��: log_mod_get_exh_oh_sv()
 �� ��: ��ȡ�������ȶ�Ŀ��ֵ
 �� ��: x--ѹ����; 
 �� ��: �������ȶ�Ŀ��ֵ
 �� ��: ��
*******************************************************************************/
I16 log_mod_get_exh_oh_sv(U08 x)
{
    I16 pid_sv = 0;
    I16 tmp_out = pv_cell_TWout1(pcb_addr);
    I16 tmp_env = pv_cell_TA(pcb_addr);

    // ȷ�����ȶ�Ŀ��ֵ
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
 �� ��: log_mod_get_exh_oh_pv()
 �� ��: ��ȡ�������ȶȵ�ǰֵ
 �� ��: x--ѹ����; 
 �� ��: �������ȶȵ�ǰֵ
 �� ��: ��
*******************************************************************************/
I16 log_mod_get_exh_oh_pv(U08 x)
{
    I16 pid_pv = 0;
    I16 gas_temp = pv_gas_out_high(x);
    I16 temp_cond = log_mod_get_temp_cond(x);  /* �����¶� */

    pid_pv = gas_temp - temp_cond;

    return pid_pv;
}

/*********************************************
�� ��: Logic_evi_exh_overheat_ctrl
�� ��: ��·�������ͷ����ƣ����ȶȿ��ƣ�
�� ��: EEV_ID_ENUM id:�������           U08 x:ѹ�����
�� ��: ��
**********************************************/
static BOL fg_evi_env_allow;    // �������������־
static void Logic_evi_exh_overheat_ctrl(EEV_ID_ENUM id, U08 x,U08 control_type)
{
    PID_ID_ENUM PID_id;
    I16 pid_pv = 0;
    I16 pid_sv = 0;
    I16 gas_temp = pv_gas_out_high(x);
    
    {
        if (val_s_motor_use == EVI_ECO_OVERHEAT)  //���������ȶ�
        {
            // ȷ�����ȶ�Ŀ��ֵ
            pid_sv = fun_line_value(gas_temp,\
                    val_e_ext_2, val_e_ext_1, val_e_sv_2, val_e_sv_1);

            evi_adjust_tmp[x][EVI_ADJ_TMP_BASE] = pid_sv;
            
            // ȷ�����ȶȵ�ǰֵ
            if (SensorCannotUse(pv_cell_evi_in(x)) || SensorCannotUse(pv_cell_evi_out(x))) 
            {
                pid_pv = 0;   // ������̽ͷ�������趨��ǰPVΪ0Ŀ���ǹرվ�����
            }
            else
            {
                pid_pv = pv_cell_evi_out(x) - pv_cell_evi_in(x);   //���������ȶȳ���-����
            }
        }
        else if (val_s_motor_use == EVI_EXH_OVERHEAT_TMP
             ||  val_s_motor_use == EVI_EXH_OVERHEAT_PRS)  // �������ȶ�
        {
            pid_sv = log_mod_get_exh_oh_sv(x);
            evi_adjust_tmp[x][EVI_ADJ_TMP_BASE] = pid_sv;
            pid_sv = log_mod_exh_oh_sv_limit(x, pid_sv);
            evi_adjust_tmp[x][EVI_ADJ_TMP_RANGE] = pid_sv;
            
            pid_pv = log_mod_get_exh_oh_pv(x);
        }

#ifdef USE_AT_EEVS
        /* ���������ֶ�Ŀ����ȶ�ʱ */
        if (USE_ABILITY_EVI_SSH_SV)    
        {
            pid_sv = val_ability_test_evi_ssh_sv;
            SETBIT(evi_adjust_tmp[x][EVI_ADJ_TMP_USAGE], EVI_USAGE_AT);
        }
#endif

        pv_cell_evi_pv(x) = gdebugIO.out_superheatpv = s_motor_pv_sheat = pid_pv;
        pv_cell_evi_sv(x) = gdebugIO.out_superheatsv = s_motor_sv_sheat = pid_sv;
    }

    // Ԥ��PID����
    if(x==0)    // 1#ѹ��
    {
        PID_id = PID_ID_EEV_2;      // 2#�����Ӧ2#PID;
        pid_set_ctrl(PID_id, PID_CTRL_INCREMENT);       //����ʽ
        pid_set_Ts(PID_id, val_s_pid_calculate_period); //��������
        pid_set_Kp(PID_id, val_s_pid_Kp);//����ϵ��
        pid_set_Ti(PID_id, val_s_pid_i);//����ʱ��
        pid_set_Td(PID_id, val_s_pid_d);//΢��ʱ��
        pid_set_Kc(PID_id, val_s_pid_p);//�����ñ�����
        pid_set_Ai(PID_id, val_s_pid_flexi);//����ϵ��

        eev_task_pid(PID_id, id, (EEV_CMD_ENUM)control_type, pid_pv, pid_sv);     // ���ͷ����ڴ���
    }
}

/*******************************************************************************
 �� ��: log_mod_get_eev_status()
 �� ��: �ж���·�������ͷ�״̬
 �� ��: x--ѹ�����; 
 �� ��: ���º� 
 �� ��: ��
*******************************************************************************/
#define EEV_STATUS_NORMAL       0       /* ���� */
#define EEV_STATUS_CANNOT_ON    1       /* ���ɿ� */
//#define EVI_STATUS_NOEXIST        2       /* ������ */
#define EEV_STATUS_OPEN_MAX     3       /* �ѿ������ */
#define EEV_STATUS_OPEN_MIN     4       /* �ѹص���С */
#define EEV_STATUS_OPEN_FIX     5       /* ������ */
static U08 log_mod_get_eev_status()
{
    U08 result = EEV_STATUS_NORMAL;
    I16 eev_step = GET_EXIO_ST_PV(0, EEV_ID_1);

    if (eev_step == 0)                                  
        result = EEV_STATUS_CANNOT_ON;                  /* ���ɿ� */               
    else if (eev_step >= eev_get_steps_run_max(EEV_ID_1))                         
        result = EEV_STATUS_OPEN_MAX;                   /* �ѿ������ */              
    else if (eev_step <= eev_get_steps_min(EEV_ID_1))                          
        result = EEV_STATUS_OPEN_MIN;                   /* �ѹص���С */         
    else if (val_motor_manual_mode == USED)               
        result = EEV_STATUS_OPEN_FIX;                   /* ������ */ 

    return result;
}

/*********************************************
�� ��: Logic_evi_control
�� ��: ��·�������ͷ�����
�� ��: EEV_ID_ENUM id:�������           U08 x:ѹ�����
�� ��: ��
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
//  I16 open_step= 0;           // ���㿪��
    I16 gas_temp = pv_gas_out_high(x);
    U16 freq_now = invt_info_freq_now(invt_id); /* ��ǰת�� */
    I16 temp_env = pv_cell_TA(pcb_addr);
    I16 temp_out = pv_cell_TWout1(pcb_addr);
    I16 temp_in = pv_cell_TWin1(pcb_addr);
    I16 force_step = MAX(1, (I32)GET_EXIO_ST_PV(0, id)*val_s_motor_force_step/1000);

    BOL fg_evi_for_liquid = FALSE;

    U08 gas_out_trend = fun_value_trend(gas_temp, mod_rec_gas_out_avg[x], 1);
 
    // �����������
    step_open_max = EVI_RUN_STEP_MAX;       // ������в���
    step_open_min = EVI_RUN_STEP_MIN;       // ��С���в���
    
    eev_set_steps_max(id, val_s_motor_step_max);        // �ܲ���         
    eev_set_steps_run_max(id, step_open_max);           // ������в���         
    eev_set_steps_min(id, step_open_min);               // ��С���в���         
    
    if (val_s_motor_use == EVI_SMALL_EEV)
    {
        U16 step_defrost  = (U32)val_m_defrost_od * val_motor_step_max / 100;   // ��˪����
        U16 step_idle = (U32)val_motor_idle_open * val_motor_step_max / 100;    /* �������� */

        step_defrost = AT_VAL(step_defrost, val_ability_test_step);
        eev_set_steps_defrost(id, step_defrost);                                // ��˪����
        eev_set_steps_idle(id, step_idle);                                      // ��������   
    }
    else
    {
        eev_set_steps_idle(id, 0);                          // ��������   
        eev_set_steps_defrost(id, 0);                // ��˪���� 
    }
              
    eev_set_period_action(id, val_s_pid_period*10);  // ������������  
                                                        
    eev_set_period_add(id, val_s_pid_period*10);     // ����ǿ���������� 
    eev_set_steps_add(id, force_step);                  // ����ǿ������ 
    eev_set_period_sub(id, val_s_pid_period*10);     // ����ǿ�ض�������(100ms)  
    eev_set_steps_sub(id, force_step);                  // ����ǿ�ز��� 
                                                        
    eev_set_period_hold(id, val_hold_evi_time*60);      // ��ͣ������������(1s)         
    eev_set_period_cyc(id, val_hold_evi_cyc*60);        // ��������(1s)       

    /* ��ʼ���� */
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
    
    eev_set_steps_preopen(id, valve_step_preopen);           // Ԥ������(��ʼ����)
    eev_set_period_preopen(id, val_s_pid_period*10);         // Ԥ��ʱ��(100ms)

    /* �������� */
    {
        // ǿ�ƿ���
        U08 eev_sta = log_mod_get_eev_status();	
        
        fg_force_motor[id][FORCE_ADD_MOTOR_EXH] = Condition_judge(fg_force_motor[id][FORCE_ADD_MOTOR_EXH], 
                                                              gas_temp, 
                                                              val_force_ctrl_gas_out-GAS_OUT_HIGH_DIFF1, 
                                                              50, DRCT_ABOVE);
  
        /* ��������Һ�� */
        if (val_evi_for_liquid && fg_force_motor[id][FORCE_ADD_MOTOR_EXH])
        {
            fg_evi_for_liquid = TRUE;
        }

        //�������ȣ�����Ϊ����״̬ʱ�������Զ����ơ��Ҳ�Ӱ������Ϊ��Һ��
        if (eev_sta == EEV_STATUS_NORMAL && val_high_gas_eev_evi_order == EEV_PRIOR)
        {
            fg_force_motor[id][FORCE_ADD_MOTOR_EXH] = FALSE;
        }
        
        if(Cm[x].run_mode == MODE_HEAT)
        {
            // ���¿���
            fg_evi_allow[id][EVI_OPEN_ENV] = Condition_judge(fg_evi_allow[id][EVI_OPEN_ENV], 
                                                             temp_env, 
                                                             val_evi_open_env_heat, 
                                                             20, DRCT_UNDER);

            /* ��Ϊ�����رջ����fg_evi_allow����������һ������fg_evi_env_allow�������ط��� */
            fg_evi_env_allow = fg_evi_allow[id][EVI_OPEN_ENV];

            // �����������ȶ�
            fg_evi_allow[id][EVI_OPEN_EXH_OH] = Condition_judge(fg_evi_allow[id][EVI_OPEN_EXH_OH], 
                                                             gas_temp-temp_out, 
                                                             val_evi_open_exh_overheat_heat, 
                                                             val_evi_open_exh_overheat_diff, DRCT_ABOVE);
        }
        else if(Cm[x].run_mode == MODE_COOL)
        {
            // ���¿���
            fg_evi_allow[id][EVI_OPEN_ENV] = Condition_judge(fg_evi_allow[id][EVI_OPEN_ENV], 
                                                             temp_env, 
                                                             val_evi_open_env_cool, 
                                                             20, DRCT_ABOVE);

            // �����������ȶ�
            fg_evi_allow[id][EVI_OPEN_EXH_OH] = Condition_judge(fg_evi_allow[id][EVI_OPEN_EXH_OH], 
                                                             gas_temp-pv_cell_wing(x), 
                                                             val_evi_open_exh_overheat_cool, 
                                                             val_evi_open_exh_overheat_diff, DRCT_ABOVE);
        }

        // ��������
        fg_evi_allow[id][EVI_OPEN_EXH] = Condition_judge(fg_evi_allow[id][EVI_OPEN_EXH], 
                                                         gas_temp, 
                                                         val_evi_open_exh, 
                                                         val_evi_open_exh_diff, DRCT_ABOVE);

        /* ת�ٿ��� */
        fg_evi_allow[id][EVI_OPEN_RPS] = Condition_judge(fg_evi_allow[id][EVI_OPEN_RPS], 
                                                         freq_now, 
                                                         val_evi_open_rps, 
                                                         (val_evi_open_rps-val_evi_close_rpg), DRCT_ABOVE);
    }


    // ȷ�������л�����
    if (val_s_motor_use == EVI_SMALL_EEV)
    {
        if (!s_get_cm_status(x, CM_NEED)
        &&  !invt_info_is_running(INVT_ID_1)        /* �˵���Ƶͣ������ */
        )
        {
            control_type = EEV_CMD_RESET;
        }
        else if (Cm[x].defrost_stage != DS_IDLE)
        {
            control_type = EEV_CMD_DEFRO;			/* ��˪������˪���� */
        }
        else if (Cm[x].run_mode==MODE_COOL)
        {
            control_type = EEV_CMD_PREV;            /* ����ά�ֳ�ʼ���� */
        }
        else
        {
            control_type = EEV_CMD_STOP;            // ���ȷ�����
        }
    }
    else
    {
        if (!s_get_cm_status(x, CM_NEED)
        ||  Cm[x].run_mode==MODE_HEAT && val_s_motor_use_heat==UNUSED
        ||  Cm[x].run_mode==MODE_COOL && val_s_motor_use_cool==UNUSED
        ||  Cm[x].defrost_stage!=DS_IDLE
        ||  cm_time[x].on<val_evi_open_dly
        ||  (!fg_evi_allow[id][EVI_OPEN_ENV] && !fg_evi_for_liquid)	/* ��������Һ��ʹ��ʱ������ǿ�ƿ�������ʱ���Ի������� */
        ||  !fg_evi_allow[id][EVI_OPEN_EXH_OH] 
        ||  !fg_evi_allow[id][EVI_OPEN_EXH]
        || (!fg_evi_allow[id][EVI_OPEN_RPS] && !USE_FIX_COMP)
        ||  fg_refri_recy_act
        ) 
        {
            control_type = EEV_CMD_STOP;          // ������

            /* �ر�ʱ�����ر�־ */
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
                control_type = EEV_CMD_FORCE_UP;            // ǿ�ƿ���
            } 
            else if (fg_evi_forbid[id][FORBID_EVI_SUB_EXH])
            {
                control_type = EEV_CMD_LIMIT_DOWN;          // ���ƹ�С
            }
            else if (fg_force_motor[id][FORCE_SUB_MOTOR_OVEREXH])
            {
                control_type = EEV_CMD_FORCE_DOWN;          // ǿ�ƹ�С
            }
            else if (fg_evi_forbid[id][FORBID_EVI_ADD_OVEREXH])
            {
                control_type = EEV_CMD_LIMIT_UP;            // ���ƿ���
            }
            else
            {
                control_type = EEV_CMD_RUN;                 // ��������
            }
        }

        /* ��������ʱ�䴦�� */
        if (control_type < EEV_CMD_PREV)
        {
            motor_run_time[id] = 0;
        }

        /* �ͻ���ˮ�߼� */
        {
            U16 evi_lehw_dly;
            /* ������ʱ����ʱ�䴦�� */
            if (cm_time[x].on < LEHW_EEV_START_TIME+10)
            {
                /* ��ȥ����������ʱ */
                evi_lehw_dly = MAX(30, LEHW_EVI_WAIT_TIME-val_evi_open_dly);
            }
            else
            {
                evi_lehw_dly = LEHW_EVI_WAIT_TIME;
            }
            /* ������ʼ���ȣ�������ͻ��¸�ˮ��������������0������һ��ʱ�䣬��תΪ������п��Ȼص��������� */
            if (control_type != EEV_CMD_STOP 
             && fg_low_env_high_water
             && motor_run_time[EEV_ID_2] < evi_lehw_dly 
             && motor_run_time[EEV_ID_1] < LEHW_EEV_START_TIME  /* ��ֹ��������ִ�еͻ���ˮ�߼� */
             )
            {
                eev_set_steps_preopen(id, 0);
                control_type = EEV_CMD_PREV;
            }
        }
    }

    /* ����ʹ������ */
    {
        switch (val_s_motor_use)
        {
            default:
            case EVI_UNUSED:
                /* ������ */
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
    ������Logic_Init_Scale
    ���ܣ���ʼ����ȷ��
    ��������
    ����ֵ����ʼ����ֵ
**********************************************/
static U16 Logic_Init_Scale(U08 mode, I16 cond_side_temp, I16 eva_side_temp)
{
    I16 init_scale;    
    
    if (SensorIsError(cond_side_temp) || SensorIsError(eva_side_temp))  /* ̽ͷ����ʱ���ж� */
    {
        if (mode == MODE_COOL)  init_scale = 70;            /* ���䣬ʹ��70% */
        else                    init_scale = 50;            /* ���ȣ�ʹ��50% */
    }
    else                            /* ����ɱ��ʼ���� */
    {
        if (mode == MODE_COOL)
        {
            if (val_init_step_chg_env_cl==0 || cond_side_temp<=val_init_step_chg_env_cl)
            {
                init_scale = 100 - cond_side_temp/10 + 6*eva_side_temp/100;  /* �¶��ԷŴ�10����ʾ������Ҫ����10 */
            }
            else
            {
                init_scale = 35 + cond_side_temp/10 + 6*eva_side_temp/100;  /* �¶��ԷŴ�10����ʾ������Ҫ����10 */
            }
            
            init_scale = val_motor_start_amplify_cool * init_scale / 10;     /* ��ʼ���ȷŴ� */
            
            /* ��ʼ����������30%--80%֮�� */
            if (init_scale > val_cool_max_initial_od) init_scale = val_cool_max_initial_od;
            if (init_scale < val_cool_min_initial_od) init_scale = val_cool_min_initial_od;
        }
        else 
        {
            init_scale = 75 - cond_side_temp/10 + 6*eva_side_temp/100;   /* �¶��ԷŴ�10����ʾ������Ҫ����10 */
            /* ���ȳ����Ŵ�ϵ�����ݻ�ˮ�ֶ� */
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

                init_scale = amp * init_scale / 10; /* ��ʼ���ȷŴ� */

                /* �ظ������Ŵ� */
                if (!fg_evi_env_allow
                &&  val_s_motor_use!=EVI_UNUSED
                &&  val_s_motor_use!=EVI_SMALL_EEV
                    )
                {
                    init_scale = val_motor_start_amp_EVI_off * init_scale / 10;
                }
            }
            

            /* ��ʼ����������30%--80%֮�� */
            if (init_scale > val_heat_max_initial_od) init_scale = val_heat_max_initial_od;
            if (init_scale < val_heat_min_initial_od) init_scale = val_heat_min_initial_od;
        }
    }
    
    //init_scale = (U32)init_scale * val_motor_step_max / 100;    /*�õ���ʼ���� */
    
    return (U16)init_scale;
}

/*******************************************************************************
 �� ��: log_mod_get_evi_status()
 �� ��: �жϸ�·�������ͷ�״̬
 �� ��: x--ѹ�����; 
 �� ��: ���º� 
 �� ��: ��
*******************************************************************************/
#define EVI_STATUS_NORMAL       0       /* ���� */
#define EVI_STATUS_CANNOT_ON    1       /* ���ɿ� */
//#define EVI_STATUS_NOEXIST        2       /* ������ */
#define EVI_STATUS_OPEN_MAX     3       /* �ѿ������ */
#define EVI_STATUS_OPEN_MIN     4       /* �ѹص���С */
#define EVI_STATUS_OPEN_FIX     5       /* ������ */
static U08 log_mod_get_evi_status()
{
    U08 result = EVI_STATUS_NORMAL;
    I16 evi_step = GET_EXIO_ST_PV(0, EEV_ID_2);

    if (evi_step == 0
    ||  val_s_motor_use==EVI_SMALL_EEV)                                  
        result = EVI_STATUS_CANNOT_ON;                  /* ���ɿ� */               
    else if (evi_step >= EVI_RUN_STEP_MAX)                         
        result = EVI_STATUS_OPEN_MAX;                   /* �ѿ������ */              
    else if (evi_step <= EVI_RUN_STEP_MIN)                          
        result = EVI_STATUS_OPEN_MIN;                   /* �ѹص���С */         
    else if (val_s_motor_manual_mode == USED)               
        result = EVI_STATUS_OPEN_FIX;                   /* ������ */ 

    return result;
}

/*******************************************************************************
 �� ��: log_mod_low_env_hig_out_judge()
 �� ��: �ͻ��¸�ˮ�¹����ж�
 �� ��: ��
 �� ��: true���ͻ��¸�ˮ�¹���
 �� ��: 1��δ��ʽ�ж�����ģʽ
*******************************************************************************/
BOL log_mod_low_env_high_water_judge(void)
{
    #define TEMP_DIFF 50
    I16 temp_wat = pv_cell_TWin1(pcb_addr);
    I16 temp_env = pv_cell_TA(pcb_addr);
    
    static BOL fg_lehw = FALSE;             /* �ͻ��¸�ˮ�±�־ */

    /* �ͻ��¸�ˮ�´�����ֻ�ڸ�·�������ȶȼ���·�������ȶȿ��ƺ������д��� */
    if (val_s_motor_use!=EVI_UNUSED 
     && val_s_motor_use!=EVI_SMALL_EEV
     && val_motor_use!=EEV_UNUSED
     && val_press_diff_in!=0                /* ��Ϊ0��ʾ���øù��� */
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
�� ��: Logic_eev_control
�� ��: ���������ͷ����ƣ����ȶȿ��ƣ�
�� ��: EEV_ID_ENUM id:�������           U08 x:ѹ�����
�� ��: ��
**********************************************/
#define FORBID_SUB_MOTOR_PRESS_DIFF           10  // ѹ���ز�
#define FORBID_SUB_MOTOR_GAS_DIFF             20  // �����ز�
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

    I16 cond_side_temp;     /* �������¶� */
    I16 evap_side_temp;     /* �������¶� */
    I16 step_eev = GET_EXIO_ST_PV(0, EEV_ID_1);
    I16 step_evi = GET_EXIO_ST_PV(0, EEV_ID_2);
    I16 force_step = MAX(1, (I32)step_eev*val_motor_force_step/1000);

    I16 gas_temp = pv_gas_out_high(x);
    I16 temp_env = pv_cell_TA(pcb_addr);
    I16 temp_in = pv_cell_TWin1(pcb_addr);
    U16 freq_now = invt_info_freq_now(INVT_ID_1);
    
    I16 alarm_lp = (Cm[x].run_mode==MODE_COOL ? val_lp_value_incl : val_lp_value_inht);
    I16 alarm_hp = (Cm[x].run_mode==MODE_COOL ? val_hp_value_incl : val_hp_value_inht);   

    I16 limit_lp = alarm_lp + val_lp_forbid_add_prs_diff;	/* ����С���Ƶ�ѹѹ�� */
    I16 limit_hp = alarm_hp + val_hp_forbid_add_prs_diff;   /* ����С���Ƹ�ѹѹ�� */

    U08 gas_out_trend = fun_value_trend(gas_temp, mod_rec_gas_out_avg[x], 1);

    static I16 step_before_defr[MAX_COMP];  /* ��˪ǰ�������� */
    static I16 step_before_reoil[MAX_COMP]; /* ����ǰ�������� */


    // ȷ�����ڼ����ʼ���ȵ������༰�������¶�
    if (Cm[x].run_mode == MODE_COOL)          // ����
    {
        evap_side_temp = temp_in;      // �������¶�Ϊ�յ�ˮ��
        cond_side_temp = temp_env;    // �������¶�Ϊ�����¶�
        step_open_min = (U32)Logic_eev_step_min(x) * val_motor_step_max / 100;  // ��С���в���
        step_open_max = (U32)val_motor_max_od * val_motor_step_max / 100;       // ������в���
    }
    else // ����
    {
        cond_side_temp = temp_in;      // �������¶�Ϊ�յ�ˮ��
        evap_side_temp = temp_env;    // �������¶�Ϊ�����¶�
        step_open_min = (U32)Logic_eev_step_min(x) * val_motor_step_max / 100;  // ��С���в���
        step_open_max = (U32)val_motor_max_od * val_motor_step_max / 100;       // ������в���
    }    


    // �����������
    step_defrost  = (U32)val_m_defrost_od * val_motor_step_max / 100;       // ��˪����
    step_defrost = AT_VAL(step_defrost, val_ability_test_step);
    step_idle = (U32)val_motor_idle_open * val_motor_step_max / 100; /* �������� */
    eev_set_steps_max(id, val_motor_step_max);                  // �ܲ���
    eev_set_steps_run_max(id, step_open_max);                   // ������в���
    eev_set_steps_min(id, step_open_min);                       // ��С���в���
    eev_set_steps_idle(id, step_idle);                          // ��������
    eev_set_steps_defrost(id, step_defrost);                    // ��˪����
    eev_set_period_calculate(id, val_m_pid_calculate_period);   // ��������(100ms)
    eev_set_period_action(id, val_m_pid_period*10);             // ��������(100ms) 
    
    eev_set_period_add(id, val_m_pid_period*10);                // ǿ�ƿ�����������(100ms) 
    eev_set_steps_add(id, force_step);              
    eev_set_period_sub(id, val_m_pid_period*10);                // ǿ�ƹط���������(100ms) 
    eev_set_steps_sub(id, force_step);              
    
    eev_set_period_hold(id, val_hold_evi_time*60);             // ��������(1s) 
    eev_set_period_cyc(id, val_hold_evi_cyc*60);             // ��������(1s) 

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
        valve_degree_preopen =  Logic_Init_Scale(Cm[x].run_mode, cond_side_temp, evap_side_temp);  // ��ʼ���� 
        valve_step_preopen   = eev_degree_to_steps(id, valve_degree_preopen);
    }
        
    eev_set_steps_preopen(id, valve_step_preopen);              // Ԥ������(��ʼ����)
    eev_set_period_preopen(id, val_m_motor_start_time*10);      // Ԥ��ʱ��(100ms)
    
    // ȷ�������л�����
    if (!s_get_cm_status(x, CM_NEED)
    &&  !invt_info_is_running(INVT_ID_1)        /* �˵���Ƶͣ������ */
        ) 
    {
        control_type = EEV_CMD_RESET;           // ����λ����������     
        
        step_before_defr[x] = SENSOR_NOEXIST;
        step_before_reoil[x] = SENSOR_NOEXIST;
    }
    else
    {
        // ��ֹ�ط�
        {
            /* ��ѹ */
            fg_forbid_motor_sub[id][FORBID_SUB_MOTOR_LP]  = Condition_judge(fg_forbid_motor_sub[id][FORBID_SUB_MOTOR_LP], 
                                                                            pv_cell_lp(x), 
                                                                            limit_lp, 
                                                                            val_lp_pre_adju_diff, 
                                                                            DRCT_UNDER);
            /* ��ѹ */
            fg_forbid_motor_sub[id][FORBID_SUB_MOTOR_HP]  = Condition_judge(fg_forbid_motor_sub[id][FORBID_SUB_MOTOR_HP], 
                                                                            pv_cell_hp(x), 
                                                                            limit_hp, 
                                                                            -val_hp_pre_adju_diff, 
                                                                            DRCT_ABOVE);
            /* ���� */
            fg_forbid_motor_sub[id][FORBID_SUB_MOTOR_EXH] = Condition_judge(fg_forbid_motor_sub[id][FORBID_SUB_MOTOR_EXH], 
                                                                            gas_temp, 
                                                                            val_force_ctrl_gas_out-GAS_OUT_HIGH_DIFF2, 
                                                                            50, DRCT_ABOVE);

            /* ������ */
            if(Cm[x].run_mode==MODE_COOL)   
            {
                fg_forbid_motor_sub[id][FORBID_SUB_MOTOR_WING] = FALSE;
            }
            else
            {
                fg_forbid_motor_sub[id][FORBID_SUB_MOTOR_WING] = Condition_judge(fg_forbid_motor_sub[id][FORBID_SUB_MOTOR_WING], pv_cell_wing(x), val_motor_limit_down_wing, 10, DRCT_UNDER);
            }
        }
        
        // ��ֹ����
        {
            /* ������ */
            if(Cm[x].run_mode==MODE_COOL)   //
            {
                fg_forbid_motor_add[id][FORBID_ADD_MOTOR_EVAP] = Condition_judge(fg_forbid_motor_add[id][FORBID_ADD_MOTOR_EVAP], pv_evap_temp(x), val_max_evap_temp-20, 20, DRCT_ABOVE);
            }
            else
            {
                fg_forbid_motor_add[id][FORBID_ADD_MOTOR_EVAP] = Condition_judge(fg_forbid_motor_add[id][FORBID_ADD_MOTOR_EVAP], pv_cell_wing(x), val_max_evap_temp-20, 20, DRCT_ABOVE);
            }

#if defined(M_CHANGFAN)           /* �㶫����ר�� */
            /* �������ȶ� */
            fg_forbid_motor_add[id][FORBID_ADD_MOTOR_OVEREXH] = Condition_judge(fg_forbid_motor_add[id][FORBID_ADD_MOTOR_OVEREXH],
                                                                           log_mod_get_exh_oh_pv(x),
                                                                           170,
                                                                           10,
                                                                           DRCT_UNDER);
#endif
        }

        // ǿ�ƿ���
        {
            U08 evi_sta = log_mod_get_evi_status();	
            /* ���� */			 
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

        // ǿ�ƹط�
        {
            I16 allow_lp = limit_lp + val_lp_pre_adju_diff;   /* �˹�С���Ƶ�ѹѹ�� */
            I16 allow_hp = limit_hp + val_hp_pre_adju_diff;   /* �˹�С���Ƹ�ѹѹ�� */
            /* ������ */
            if(Cm[x].run_mode==MODE_COOL)
            {
                if (SensorCanUse(pv_cell_hp(x)) && pv_cell_hp(x)<allow_hp)
                {
                    /* �����¶� */
                    fg_force_motor[id][FORCE_SUB_MOTOR_EVAP] = Condition_2judge(fg_force_motor[id][FORCE_SUB_MOTOR_EVAP], pv_evap_temp(x), val_max_evap_temp, 10,DRCT_ABOVE,CDT_FORCE_SUB_MOTOR_EVAP);
#if defined(M_CHANGFAN)           /* �㶫����ר�� */
                    /* �������ȶ� */
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
                    /* �����¶� */
                    fg_force_motor[id][FORCE_SUB_MOTOR_EVAP] = Condition_2judge(fg_force_motor[id][FORCE_SUB_MOTOR_EVAP], pv_cell_wing(x), val_max_evap_temp, 10,DRCT_ABOVE,CDT_FORCE_SUB_MOTOR_EVAP);
#if defined(M_CHANGFAN)           /* �㶫����ר�� */
                    /* �������ȶ� */
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
            control_type = EEV_CMD_LIMIT_DOWN;  // ���ƹ�С
        }
        else if (fg_force_motor[id][FORCE_SUB_MOTOR_EVAP]
             ||  fg_force_motor[id][FORCE_SUB_MOTOR_OVEREXH])
        {
            control_type = EEV_CMD_FORCE_DOWN;  // ǿ�ƹ�С
        }
        else if (fg_forbid_motor_add[id][FORBID_ADD_MOTOR_EVAP]
             ||  fg_forbid_motor_add[id][FORBID_ADD_MOTOR_OVEREXH]
        )
        {
            control_type = EEV_CMD_LIMIT_UP;    // ���ƿ���
        }
        else
        {
            control_type = EEV_CMD_RUN;         // ��������
        }

        // ��˪ʱ����·�������ͷ���������˪����
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

        /* ���� */
        if (0)              /* ��������֤���ݲ�ʹ�� */
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
                    eev_set_period_preopen(id, 0);      // Ԥ��ʱ��(100ms)
                }
            }

            if (cnt_reoil_end[x] <= CNT_AFTER_REOIL)
            {
                control_type = EEV_CMD_PREV;
            }
        }
    }

    /* ͣ�����㴦�� */
    {
        if (Cm[x].stage == CM_IDLE
        && cm_time[x].off > 30+CM_off_speed_down_time // ѹ��ͣ��30���Ӻ����ͷ������ʼ��(ͣ����ֻ��ʼ��һ��)
        && FALSE == fg_vavle_reset[id]
        )
        {
            fg_vavle_reset[id] = TRUE;       
            control_type = EEV_CMD_STOP;
        }
        else if (s_get_cm_status(x, CM_ON))     // ѹ����������λ��־
        {
            fg_vavle_reset[id] = FALSE;
            cm_time[x].off = 0;                 // �������㡣�����¸�ѭ��fg_vavle_reset[id]�ֻ���λ����Ϊͣ��ʱ��Ҫ���־������ܸ���ѹ��״̬���㡣
        }
    }

    /* ���ȶ�Ŀ��ֵ����ǰֵ */
    {
        switch (val_motor_use)
        {
            default:
            case EEV_OVERHEAT_SUC:
                SETBIT(eev_adjust_tmp[x][EEV_ADJ_TMP_USAGE], EEV_USAGE_SUC);
                
                /* ���ȶ�Ŀ��ֵȷ�� */
                if (Cm[x].run_mode == MODE_COOL)       /* ���� */
                {
                    pid_sv = Logic_eev_sv_cool(evap_side_temp);
                }
                else 
                {
                    pid_sv = Logic_eev_sv_heat(evap_side_temp);
                }

                eev_adjust_tmp[x][EEV_ADJ_TMP_BASE] = pid_sv;

                /* ���ȶ�Ŀ��ֵƵ�ʲ��� */
                if (!USE_FIX_COMP)
                {
                         if (freq_now <= 40*100) pid_sv += val_motor_sv_freq_cpst*2;    
                    else if (freq_now <= 50*100) pid_sv += val_motor_sv_freq_cpst;

                    eev_adjust_tmp[x][EEV_ADJ_TMP_COMP] = pid_sv;
                }

                /* ���ȶ�Ŀ��ֵ���²��� */
                if (SensorCanUse(gas_temp))
                {
                         if (gas_temp >= 95*10) pid_sv += val_motor_sv_exh_cpst*2;
                    else if (gas_temp >= 75*10) pid_sv += val_motor_sv_exh_cpst;

                    eev_adjust_tmp[x][EEV_ADJ_TMP_EXH] = pid_sv;
                }

                pid_sv = calculate_gb_dbg_sv(Cm[x].run_mode, pid_sv);// ���������
                eev_adjust_tmp[x][EEV_ADJ_TMP_GB] = pid_sv;

                /* ���ȶȵ�ǰֵȷ�� */
                if (SensorCanUse(pv_cell_evap_temp(x)))
                {
                    pid_pv = fun_pv_sub(pv_cell_gas_in(x), pv_cell_evap_temp(x));   /* ѹ�����ȶ� */
                }
                else
                {
                    if (Cm[x].run_mode == MODE_COOL)
                    {
                    	/* �������¶ȿ���ʱ��ʹ�÷����¶ȼ��㣬�������¶Ȳ�����ʱ��������ýҺ���¶���������¶� */
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
                
                /* ���ȶ�Ŀ��ֵȷ�� */
                pid_sv = log_mod_get_exh_oh_sv(x);
                eev_adjust_tmp[x][EEV_ADJ_TMP_BASE] = pid_sv;

                /* ���ȶ�Ŀ��ֵƵ�ʲ��� */
                if (!USE_FIX_COMP && val_motor_sv_freq_cpst!=0)
                {
                    /* ��ʾ��������Ϊ�˵õ�����Ƶ�����ޣ�������Ϊ����Ƶ���Ӽ�������Ƶ������ʱע�ⲻ�ô������� */
                    I16 invt_max = wf_get_freq_max_by_env_in(Cm[x].run_mode, temp_env, temp_in);
                    I16 tmp = MIN((I32)CmRotation[x]*val_motor_sv_freq_cpst/invt_max, 10);	/* ���ֵ����Ϊ1.0 */

                    pid_sv = _DIVUROUND(pid_sv*tmp, 10);

                    eev_adjust_tmp[x][EEV_ADJ_TMP_COMP] = pid_sv;
                }

                pid_sv = log_mod_exh_oh_sv_limit(x, pid_sv);
                eev_adjust_tmp[x][EEV_ADJ_TMP_RANGE] = pid_sv;

                /* ���ȶȵ�ǰֵȷ�� */
                pid_pv = log_mod_get_exh_oh_pv(x);
                
                break;
        }

        /* ���������ֶ�Ŀ����ȶ�ʱ */
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

    /* ��������ʱ�䴦�� */
    if (control_type < EEV_CMD_PREV)
    {
        motor_run_time[id] = 0;
    }
    
    /* ������ʼ���ȣ�������ͻ��¸�ˮ�������������Թ̶���������һ��ʱ�䣬��תΪ��ʼ���Ȼص��������� */
    if (fg_low_env_high_water
     && S_CM_GetStatus(x, CM_NEED)
     && motor_run_time[EEV_ID_1] < LEHW_EEV_START_TIME
     && control_type!=EEV_CMD_DEFRO
     )
    {
        eev_set_steps_preopen(id, val_press_diff_eev);
        control_type = EEV_CMD_PREV;
    }

    /* �ͻ��¸�ˮ�¹���ʱ������ά��ʱ��ʹ���ض�ֵ */
    if (Cm[x].run_mode != MODE_COOL
    &&  fg_low_env_high_water
        )
    {
        eev_set_period_preopen(id, LEHW_EEV_INIT_TIME*10);      // Ԥ��ʱ��(100ms)
    }

    /* �շ����� */
    if (fg_refri_recy_act)
    {
        eev_set_steps_preopen(id, val_recycle_eev_step);
        control_type = EEV_CMD_PREV;
    }

    // Ԥ��PID����
    if(x==0)    // 1#ѹ��
    {
        PID_id = PID_ID_EEV_1;      // 1#�����Ӧ1#PID
        pid_set_ctrl(PID_id, PID_CTRL_INCREMENT);       //����ʽ
        pid_set_Ts(PID_id, val_m_pid_calculate_period); //��������
        pid_set_Kp(PID_id, val_m_pid_Kp);//����ϵ��
        pid_set_Ti(PID_id, val_m_pid_i);//����ʱ��
        pid_set_Td(PID_id, val_m_pid_d);//΢��ʱ��
        // �����ñ�����
        if (pid_get_AveAcrossCnt(PID_id) > AVG_ACROSS_NUM2
        &&  pv_cell_step_m(x)<val_eev_step_refer
        )                          
        {
            /* ��Գ��������Ȼ��Ƶ����ʱ�����𵴲���������Ż� */
            U32 kc = MIN((U32)val_m_pid_p*val_eev_refer_coef, 30000);
            pid_set_Kc(PID_id, kc);
        }
        else
        {
            pid_set_Kc(PID_id, val_m_pid_p);
        }
        pid_set_Ai(PID_id, val_m_pid_flexi);//����ϵ��

        eev_task_pid(PID_id,id, (EEV_CMD_ENUM)control_type, pid_pv, pid_sv);     // ���ͷ����ڴ���
    }        

     pv_cell_eev_pv(x) = m_motor_pv_sheat = gdebugIO.in_superheatpv = pid_pv;
     pv_cell_eev_sv(x) = m_motor_sv_sheat = gdebugIO.in_superheatsv = pid_sv;
}

/*******************************************************************************
 �� ��: log_mod_eev_freq()
 �� ��: ��Ƶ����
 �� ��: en--����ָ��; 
        step_pv--������ǰ����; 
 �� ��: ������������
 �� ��: ��
*******************************************************************************/
I16 log_mod_eev_freq(U08 en, U16 step_pv)
{
    static I16 freq_old;
    I16 inc_od = 0;
    I16 inc_freq = 0;

    U16 freq_now = _DIVUROUND(invt_info_freq_now(INVT_ID_1), 100);       /* ת��Ϊ1rps */

    if (USE_FIX_COMP)
    {
        return 0;
    }

    /* 1����Ƶ�ʱ仯���򲹳����� */
    if (cnt_invt_chg_speed > 60)
    {
        if (Fun_AbsI32(fun_pv_sub(freq_now, freq_old)) >= val_eev_invt_rps_diff
        &&  freq_old!=0     /* �����״� */
        &&  cnt_invt_chg_speed < 120		/* ����120˵���ܾ�ûִ�иú�������ͣ���ȣ������ */
        )
        {
            inc_freq = fun_pv_sub(freq_now, freq_old);
        }

        freq_old = freq_now;
        cnt_invt_chg_speed = 0;
    }

    if (inc_freq > 0    // ��Ƶ����
    && (en==EEV_CMD_RUN || en==EEV_CMD_LIMIT_DOWN)
    )   
    {        
        inc_od = (I32)freq_now*inc_freq*step_pv*val_eev_invt_coef_up/100000;
    }
    else if (inc_freq < 0    // ��Ƶ����
    && (en==EEV_CMD_RUN || en==EEV_CMD_LIMIT_UP)
    )
    {        
        inc_od = (I32)freq_now*inc_freq*step_pv*val_eev_invt_coef_dn/100000;
    }

    return inc_od;
}

/*******************************************************************************
 �� ��: log_mod_work_condition_chg_deal()
 �� ��: �����仯����
 �� ��: ��
 �� ��: ��
 �� ��: ��
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

    fg_chg |= fun_pv_filt_t(&temp_env,  pv_cell_TA(pcb_addr),  30,  &wcd_cnt[WCD_ENV],     1800);   /* 3�� */
    fg_chg |= fun_pv_filt_t(&eev_sv,    pv_cell_eev_sv(cm),    10,  &wcd_cnt[WCD_EEV_SV],  1800);   /* 10�� */
    fg_chg |= fun_pv_filt_t(&freq,      pv_cell_invt_set(cm),  100, &wcd_cnt[WCD_FREQ],    1800);   /* 10ת */

    /* �����仯����������������� */
    if (fg_chg)
    {
        if (pid_get_AveAcrossCnt(id) > AVG_ACROSS_NUM)  pid_set_AveAcrossCnt(id, 0);    
    }

    /* ��ʱ���� */
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
 �� ��: log_mod_eev()
 �� ��: �������ͷ����������������븨��
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_mod_eev(void)
{
    U08 i;

    /* �����仯���� */
    log_mod_work_condition_chg_deal();

    /* �ͻ��¸�ˮ�±�־ */
    fg_low_env_high_water = log_mod_low_env_high_water_judge();

    for (i=0; i<self_comp_num; i++) 
    {
        fg_comp_valve_reset[i]=VALVE_READY;
        if( i==0 )
        {
            // ȷ�ϵ���׼��״̬
            if (TF_IS_IDLE)	/* �̼�ʱ���ȴ����� */
            {
                if (val_s_motor_use!=EVI_UNUSED  
                   && ((Cm[i].run_mode==MODE_HEAT && val_s_motor_use_heat!=UNUSED)
                       ||(Cm[i].run_mode==MODE_COOL && val_s_motor_use_cool!=UNUSED))
                   && !eev_isReady(EEV_ID_2))
                {
                    fg_comp_valve_reset[i]=VALVE_BUSY;  // ����������ʹ�� �� δ׼����
                }
            }

            if(val_motor_use!=EEV_UNUSED && !eev_isReady(EEV_ID_1))
            {
                fg_comp_valve_reset[i]=VALVE_BUSY;  // ����·����ʹ�� �� δ׼����
            }
        }

        if (val_motor_use != EEV_UNUSED)            // ��·�������
        {
            Logic_eev_control(EEV_ID_1, i);       
        }

        if (val_s_motor_use != EVI_UNUSED)          // ��·�������
        {
            Logic_evi_control(EEV_ID_2, i);
        }
    }
}

/*******************************************************************************
 �� ��: log_mod_elec()
 �� ��: ģ�鹦��ͳ��
 �� ��: ��
 �� ��: ��
 �� ��: ��λ��1w
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

    /* ѹ������ */
#ifndef M_DEBUG
    if (SensorCanUse(pv_cell_invt_power_out(INVT_ID_1)))
    {
        power_comp = pv_cell_invt_power_out(INVT_ID_1)*10;
    }
#else
    if (SensorCanUse(pv_cell_invt_volt(INVT_ID_1)))
    {
        power_comp = pv_cell_invt_volt(INVT_ID_1);				/* ����ʱ��10A��Ϊ������������������ */
    }
#endif    

    /* ������� */
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

    /* ˮ�ù��� */
    {
        I32 pump_now = pv_cell_PUMPf_set(pcb_addr);
        
        if (SensorCanUse(pump_now)
        &&  speed_max_pump>0
        )
        {
            power_pump = (I16)(pow((float)pump_now/speed_max_pump, 3)*val_power_pump);
        }
    }

    /* IBH���� */
    if (s_get_output(MAX_COMP, S_DO_IBH1))
    {
        power_IBH1 = val_power_IBH1;
    }
    if (s_get_output(MAX_COMP, S_DO_IBH2))
    {
        power_IBH2 = val_power_IBH2;
    }

    /* �������� */
    if (MAC_IS_IDLE)
    {
        power_idle = val_power_idle;
    }

    /* �������й��� */
    if (MAC_IS_RUN)
    {
        power_other_run = val_power_other_run;
    }


    /* ģ�鹦�� */
    pv_cell_power(pcb_addr) = power_comp+power_fan+power_pump+power_IBH1+power_IBH2+power_idle+power_other_run;

    gdebugIO.debugbuf[80] = power_comp;
    gdebugIO.debugbuf[81] = power_fan;
    gdebugIO.debugbuf[82] = power_pump;
    gdebugIO.debugbuf[83] = power_IBH1;
    gdebugIO.debugbuf[84] = power_IBH2;
    gdebugIO.debugbuf[111] = power_idle;
    gdebugIO.debugbuf[112] = power_other_run;
}


/* PUMPf�������� */
enum
{
    PUMPF_ON_SIG_CELL_RUN                = 0,   /* ģ������ʱ���� */
    PUMPF_ON_SIG_IDLE_STA_ANTIFREEZE     = 1,   /* ����̬����ʱ���� */
    PUMPF_ON_SIG_RUN_STA_AC_ANTIFREEZE   = 2,   /* ����̬�յ�ˮ·����ʱ���� */
    PUMPF_ON_SIG_RUN_STA_HOTW_ANTIFREEZE = 3,   /* ����̬��ˮˮ·����ʱ���� */
    PUMPF_ON_SIG_HOTW_TIMED_ANTIFREEZE   = 4,   /* ��ˮ��ʱ����ʱ���� */
    PUMPF_ON_SIG_DISINFECT               = 5,   /* ɱ��ʱ���� */
    PUMPF_ON_SIG_LONGTIME_OFF            = 6,   /* ��ʱ��δ����ʱ���� */
    PUMPF_ON_SIG_TEST                    = 7,   /* �����й��ܿ����������ſ������л�ˮ�������У� */
    PUMPF_ON_SIG_DEBUG                   = 8,   /* �������п��� */
    PUMPF_ON_SIG_MAX,
};
static U16 fg_PUMPf_on[_DIVUCARRY(PUMPF_ON_SIG_MAX, 16)];

#define  LONGTIME_NO_RUN_ON     (6*60*QK_60)    // ��ʱ��δ���п�����ʱ
#define  LONGTIME_NO_RUN_OFF    (3*QK_60)       // ��ʱ��δ���п� ����ʱ��
#define  PUMPf_OFF_DLY          (TF_IS_IDLE?((fg_air_empty_test_run||fg_pump_test_run)?0:(60+CM_off_speed_down_time)):30)     //�ؿյ�����ʱ  // ԭ��30 
/*******************************************************************************
 �� ��: log_mod_PUMPf_interval_on_need()
 �� ��: ��Ƶˮ�ü�Ъ�����������
 �� ��: ��
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL log_mod_PUMPf_interval_on_need(void)
{
    U08 sub_sys_type;
    BOL fg_interval_on;

    sub_sys_type = GET_SUB_SYS(pcb_addr);
    fg_interval_on = (cnt_PUMPf_interval_on_off < PUMPF_INTERVAL_ON_TIME) ? TRUE : FALSE;

    /* ǿ��һֱ���е���� */
    if (MODE_IS_COOL(sub_sys_type)          /* ����ģʽ */
    || fg_energy_need[sub_sys_type]         /* ��ϵͳ���������� */
    || log_mod_comp_is_run(MODE_NULL)       /* ģ��ѹ������ */
    )
    {
        fg_interval_on = TRUE;
        cnt_PUMPf_interval_on_off = 0;
    }

    return fg_interval_on;
}
 
/*******************************************************************************
 �� ��: log_mod_PUMPf_need()
 �� ��: ģ���Ƶˮ���������
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_mod_PUMPf_need(void)
{
    /* ģ������ʱ */
    if (fg_cell_pump_open)
    {
        if (val_pump_mode == PUMP_MODE_INTER)           /* ��Ъ���� */
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
        else                /* һֱ���� */
        {
            SETBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_CELL_RUN);
        }
    }
    else
    {     
        cnt_PUMPf_interval_on_off = 0;      /* ģ��δ��ʱ����Ъ���м�ʱ����Ч */
    
        if (cnt_PUMPf_off_dly > PUMPf_OFF_DLY)
        {
            CLRBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_CELL_RUN);
        }
    }

    /* ����̬����ʱ */
    if (fg_idle_sta_antifreeze)
    {
        SETBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_IDLE_STA_ANTIFREEZE);
    }
    else
    {
        CLRBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_IDLE_STA_ANTIFREEZE);
    }

    /* ����̬�յ�ˮ·���� */
    if ((GET_SUB_SYS(pcb_addr) == SUB_SYS_AC_ONLY) && fg_run_sta_ac_antifreeze)
    {
        SETBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_RUN_STA_AC_ANTIFREEZE);
    }
    else
    {
        CLRBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_RUN_STA_AC_ANTIFREEZE);
    }

    /* ����̬��ˮˮ·���� */
    if ((GET_SUB_SYS(pcb_addr) == SUB_SYS_AC_HOTW) && fg_run_sta_hotw_antifreeze)
    {
        SETBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_RUN_STA_HOTW_ANTIFREEZE);
    }
    else
    {
        CLRBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_RUN_STA_HOTW_ANTIFREEZE);
    }

    /* ��ˮ��ʱ���� */
    if ((GET_SUB_SYS(pcb_addr) == SUB_SYS_AC_HOTW) && fg_hotw_timed_antifreeze_S3V1_OFF_need)
    {
        SETBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_HOTW_TIMED_ANTIFREEZE);
    }
    else
    {
        CLRBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_HOTW_TIMED_ANTIFREEZE);
    }
    
    /* ��ʱɱ��ʱ */
    if ((GET_SUB_SYS(pcb_addr) == SUB_SYS_AC_HOTW)
    && fg_timed_disinfect_run
    )
    {        
        if (!fg_timed_disinfect_TBH_need
        && !fg_timed_disinfect_IBH_need
        && !fg_timed_disinfect_AHS_need
        )
        {
            CLRBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_CELL_RUN);	/* �ӹ������������� */
            SETBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_DISINFECT);	/* ɱ���׶�1������Ƶˮ�� */
        }
		else if(fg_timed_disinfect_IBH_need 					/* �׶�һ���׶ζ���־λ�����ڹ��� */
		    && (GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_NORMAL)
            ||  GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL)
        	||	GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_ABNORMAL)
        	||  GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_KEEP_ABNORMAL))
        )
		{
            CLRBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_CELL_RUN);	/* �ӹ������������� */
            SETBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_DISINFECT);	/* ɱ���׶�1������Ƶˮ�� */			
		}		
        else if (fg_timed_disinfect_IBH_need && GETBIT_N(fg_IBH_on, IBH_ON_SIG_DISINFECT)
        || (fg_timed_disinfect_AHS_need && fg_timed_disinfect_AHS_on)
        )
        {
            SETBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_DISINFECT);	/* ɱ���׶�2������Ƶˮ�� */
        }
        else
        {
            CLRBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_CELL_RUN);	/* �ӹ������������� */
            CLRBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_DISINFECT);	/* ɱ���׶����迪����Ƶˮ�� */
        }
    }
    else
    {
        CLRBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_DISINFECT);
    }

    /* ��ⳤʱ��δ���� */
    if (!fg_cell_pump_open)
    {
        if (PUMPf_time.off >= LONGTIME_NO_RUN_ON)				/* ����6Сʱδ���У�ǿ������3���� */
        {
            SETBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_LONGTIME_OFF);
        }
        else if (PUMPf_time.on >= LONGTIME_NO_RUN_OFF)
        {
            CLRBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_LONGTIME_OFF);
        }
    }

    /* �����й��ܣ������ſ������л�ˮ���������п���Ƶˮ������ */
    if (fg_PUMPf_test_run)
    {
        SETBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_TEST);
    }
    else
    {
        CLRBIT_N(fg_PUMPf_on, PUMPF_ON_SIG_TEST);
    }

    /* �������� */
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
 �� ��: log_mod_PUMPf_out()
 �� ��: ģ���Ƶˮ�õ�Դ���
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_mod_PUMPf_out(void)
{
    U08 i;

    /* ��PUMPf������־ */
    fg_PUMPf_run = FALSE;

    if (USE_PUMPf_CTRL != PUMPF_CTRL_OFF    /* ʹ�ñ�Ƶˮ�� */
    && !s_get_error(S_ERR_PUMPF)            /* �ޱ�Ƶˮ�ù��� */
    && !fg_water_lack_err                   /* ��ˮ������ */
    && !TF_IS_OUT                           /* ���̼����ʱ */
    && !fg_test_run_PUMPf_off               /* �������йر�Ƶˮ�� */
    )
    {
        /* ������־������Ƿ���Ҫ���� */
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
 �� ��: log_mod_PUMPf()
 �� ��: ģ���Ƶˮ�ÿ���
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_mod_PUMPf(void)
{
    log_mod_PUMPf_need();	                /* ������� */
    log_mod_PUMPf_out();	                /* ��Դ��� */
    PUMPF_speed_ctrl(fg_PUMPf_run);			/* ���ٿ��� */
}

/*******************************************************************************
 �� ��: log_mod_IBH_level()
 �� ��: ��ȡ�������ȵ�λ����
 �� ��: 
 �� ��: �������ȵ�λ
 �� ��: ��
*******************************************************************************/
U08 log_mod_IBH_level(I16 pv, I16 sv, U08 idx)
{
    static U08 area[IBH_ON_SIG_MAX] = {0};
    U08 level = IBH_L0;   

    /* ���ݵ�ǰ�¶ȷ��� */
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
 �� ��: log_mod_IBH_need()
 �� ��: ������������
 �� ��: ��
 �� ��: ��
 �� ��: ��
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

    /* ���������ȿ�����ʱ�� */
    U16 IBH_open_dly = val_IBH_open_dly;

    pv_TA = pv_cell_TA(pcb_addr);           /* �����¶� */
    pv_TWout2 = pv_cell_TWout2(pcb_addr);   /* �ܳ�ˮ�¶� */
    pv_TACt    = fun_pv_avg(pv_sys_TACt1, pv_sys_TACt2);    /* ����ˮ���¶� */
    pv_ctrl_ac = USE_AC_TEMP_IN_CTRL ? pv_TACt : pv_TWout2;	/* �յ������¶� */
    pv_THWt = pv_sys_THWt;	                /* ��ˮˮ���¶� */

    is_defrost = s_have_comp_defrost();

    if (!STEP_IS_RUN)
    {
        fg_comp_normal = FALSE;
        fg_comp_abnormal = FALSE;
    }
    else
    {
        fg_comp_normal = log_mod_comp_is_run(MODE_NULL) && !TA_FORCE_COMP_OFF; /* ѹ������״̬ */
        fg_comp_abnormal = !log_mod_comp_is_run(MODE_NULL) 
                           && (!TA_ALLOW_COMP_ON_OFF 
                               || mod_err_comp[0]               
                               || log_mod_comp_temp_out_limit(0)
                              );    /* ѹ���쳣״̬ */
    }

    /* ��ˮ���±��� */
    if (pv_TWout2 > 700)        fg_TWout2_too_high = TRUE;
    else if (pv_TWout2 < 650)   fg_TWout2_too_high = FALSE;

    /* �������ȱ��ֹر� */
    if ((!IBH_cfg_type)                     /* IBH����Ϊ0 */
    || val_use_IBH==IBH_UNUSED			    /* IBH����Ϊ���� */
    || s_get_error(S_ERR_WATER_FLOW)        /* ˮ��������� */
    || fg_ability_test_run                  /* �������� */
    || fg_floor_preheat_run                 /* ��ůԤ�� */
    || fg_floor_dryup_run	                /* ��ů��� */
    || fg_TWout2_too_high	                /* ��ˮ���±��� */
    )
    {
        fg_force_IBH_off = FALSE;
        ARRSET(fg_IBH_on, 0);
        return IBH_L0;
    }
    else if (!fg_idle_sta_antifreeze                        /* ����̬�������ж��������� */
         && !MOD_IS_RUN_STA_AC_AF(pcb_addr)                 /* ����̬�յ�ˮ·�������ж��������� */
         && !MOD_IS_RUN_STA_HOTW_AF(pcb_addr)               /* ����̬��ˮˮ·�������ж��������� */
         && !TF_OPEN_IBH                                    /* �̼첻�ж��������� */
         )           
    {
        /* �������ȱ��ֹر� */
        if ((MODE_IS_HEAT(GET_SUB_SYS(pcb_addr)) && !HEAT_USE_IBH)  /* ��ϵͳ����������IBH������ˮʹ�� */
        || (MODE_IS_HOTW(GET_SUB_SYS(pcb_addr)) && !HOTW_USE_IBH)   /* ��ϵͳ����ˮ������IBH������ʹ�� */
        || MODE_IS_COOL(GET_SUB_SYS(pcb_addr))                      /* ��ϵͳ�������� */
        || (GET_SUB_SYS(pcb_addr) == SUB_SYS_AC_HOTW && fg_TBH_run 
            && MODE_IS_HOTW(GET_SUB_SYS(pcb_addr))) /* ��ǰģ���Ƿ�����ˮ������TBH���У�����ģ�� */
        || (!val_use_IBH)                                           /* ����IBH:2022-8-25TCL�����̼�ʱ�����Ǹò������� */
        )
        {
            fg_force_IBH_off = FALSE;
            ARRSET(fg_IBH_on, 0);
            return IBH_L0;
        }
    }

#if defined(M_SHENGNENG)
    /* ���������߼�-�������ȿ�����ʱ���ݻ��·ֶ� */
    if (pv_TA < -50)				/* -5�滷�·ֶ� */
    {
        IBH_open_dly = val_IBH_open_dly;
    }
    else
    {
        IBH_open_dly = QK_60*60;	/* 60min */
    }
#endif

    /* ������������ʱ���� */
    if (MODE_IS_HEAT(GET_SUB_SYS(pcb_addr)) /* �������� */
    && fg_heat_need			                /* �������� */
    && !fg_idle_sta_antifreeze              /* �Ǵ���̬���� */
    && !is_defrost			                /* �ǳ�˪ */
    && SensorCanUse(pv_ctrl_ac)				/* pv_ctrl���� */
    && !fg_enter_test_run	                /* �������� */
    )
    {
        /* IBH����/�ر��¶ȼ��� */
        sv_TA_IBH_on = val_IBH_open_env - 10;
        sv_TA_IBH_off = val_IBH_open_env;
        sv_TWout2_IBH_on = sv_temp_freq[GET_SUB_SYS(pcb_addr)] - val_IBH_open_diff;
        sv_TWout2_IBH_off = sv_temp_freq[GET_SUB_SYS(pcb_addr)];

        /* ���IBH�Ƿ�������: ѹ������״̬ */
        if (fg_comp_normal)
        {   
            CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_COMP_KEEP_ABNORMAL);

            /* IBH���������� */
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

        /* ���IBH�Ƿ������� */
        if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_COMP_NORMAL)
        || GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_COMP_KEEP_NORMAL)
        )
        {
            /* IBH���������� */
            if (pv_TA >= sv_TA_IBH_off
            || pv_ctrl_ac > sv_TWout2_IBH_off
            )
            {
                CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_COMP_NORMAL);
                CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_COMP_KEEP_NORMAL);
            }
        }

        /* IBH�����������IBH��λ */
        if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_COMP_NORMAL)
        || GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_COMP_KEEP_NORMAL)
        )
        {
            level = MAX(level, log_mod_IBH_level(pv_ctrl_ac, sv_TWout2_IBH_off, IBH_ON_SIG_RUN_HEAT_COMP_NORMAL));
        }

        /* ����/�ر��¶ȼ��� */
        sv_TWout2_IBH_on = sv_temp_freq[GET_SUB_SYS(pcb_addr)] - val_IBH_open_diff;
        sv_TWout2_IBH_off = sv_temp_freq[GET_SUB_SYS(pcb_addr)];

        /* ���IBH�Ƿ�������: ѹ���쳣״̬ */
        if (fg_comp_abnormal)
        {
            CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_COMP_KEEP_NORMAL);
            
            /* IBH���������� */
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

        /* ���IBH�Ƿ������� */
        if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_COMP_ABNORMAL)
        || GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_COMP_KEEP_ABNORMAL)
        )
        {
            /* IBH���������� */
            if (pv_ctrl_ac >= sv_TWout2_IBH_off)
            {
                CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_COMP_ABNORMAL);
                CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_COMP_KEEP_ABNORMAL);
            }
        }

        /* IBH�����������IBH��λ */
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

    /* ��������ˮ����ʱ���� */
    if (MODE_IS_HOTW(GET_SUB_SYS(pcb_addr))
    && fg_hotw_need			                /* ��ˮ���� */
    && !fg_idle_sta_antifreeze
    && !is_defrost 
    && SensorCanUse(pv_THWt)
    && !fg_enter_test_run
    && !fg_timed_disinfect_IBH_need			/* �޶�ʱɱ���׶�2��IBH���� */
    )
    {
        /* ����/�ر��¶ȼ��� */
        sv_THWt_IBH_on = 400;
		sv_THWt_IBH_off = sv_hotw;

        /* ���IBH�Ƿ�������: ѹ������״̬ */
        if (fg_comp_normal)
        {
            CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_KEEP_ABNORMAL);

            /* IBH���������� */
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

        /* ���IBH�Ƿ�������: ѹ������״̬ */
        if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_NORMAL)
        || GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL)   
        )
        {
            /* IBH���������� */
            if (pv_THWt > sv_THWt_IBH_off)
            {
				CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_NORMAL);
            	CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL);
            }
        }

        /* IBH�����������IBH��λ */
        if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_NORMAL)
        || GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL)
        )
        {
            level = MAX(level, log_mod_IBH_level(pv_THWt, sv_THWt_IBH_off, IBH_ON_SIG_RUN_HOTW_COMP_NORMAL));
        }
        

        /* ����/�ر��¶ȼ��� */
        sv_THWt_IBH_on = sv_hotw - val_hotw_diff;
        sv_THWt_IBH_off = sv_hotw;

        /* ���IBH�Ƿ�������: ѹ���쳣״̬ */
        if (fg_comp_abnormal)
        {
            CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL);

            /* IBH���������� */
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

        /* ���IBH�Ƿ�������: ѹ���쳣״̬ */
        if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_ABNORMAL)
        || GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_KEEP_ABNORMAL)
        )
        {
            /* IBH���������� */
            if (pv_THWt > sv_THWt_IBH_off)
            {
                CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_ABNORMAL);
                CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_KEEP_ABNORMAL);
            }
        }

        /* IBH�����������IBH��λ */
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

    /* ����̬�յ�ˮ·����ʱ���� */
    {
        if ((idle_sta_antifreeze_stage == AF_STAGE_AC)
        && fg_idle_sta_antifreeze_IBH_need
        )
        {
            /* IBHֱ�ӿ��� */
            SETBIT_N(fg_IBH_on, IBH_ON_SIG_IDLE_STA_AC_ANTIFREEZE);
        }
        else
        {
            CLRBIT_N(fg_IBH_on, IBH_ON_SIG_IDLE_STA_AC_ANTIFREEZE);
        }

        /* IBH��λ���� */
        if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_IDLE_STA_AC_ANTIFREEZE))
        {
            level = MAX(level, IBH_L3);
        }
    }

    /* ����̬��ˮˮ·����ʱ���� */
    {
        if ((GET_SUB_SYS(pcb_addr) == SUB_SYS_AC_HOTW)
        && (idle_sta_antifreeze_stage == AF_STAGE_HOTW)
        && fg_idle_sta_antifreeze_IBH_need
        )
        {
            /* IBHֱ�ӿ��� */
            SETBIT_N(fg_IBH_on, IBH_ON_SIG_IDLE_STA_HOTW_ANTIFREEZE);
        }
        else
        {
            CLRBIT_N(fg_IBH_on, IBH_ON_SIG_IDLE_STA_HOTW_ANTIFREEZE);
        }

        /* IBH��λ���� */
        if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_IDLE_STA_HOTW_ANTIFREEZE))
        {
            level = MAX(level, IBH_L3);
        }
    }

    /* ����̬�յ�ˮ·����ʱ���� */
    {
        if ((GET_SUB_SYS(pcb_addr) == SUB_SYS_AC_ONLY)
        && fg_run_sta_ac_antifreeze_IBH_need
        )
        {
            /* IBHֱ�ӿ��� */
            SETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_STA_AC_ANTIFREEZE);
        }
        else
        {
            CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_STA_AC_ANTIFREEZE);
        }

        /* IBH��λ���� */
        if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_STA_AC_ANTIFREEZE))
        {
            level = MAX(level, IBH_L3);
        }
    }

    /* ����̬��ˮˮ·����ʱ���� */
    {
        if ((GET_SUB_SYS(pcb_addr) == SUB_SYS_AC_HOTW)
        && fg_run_sta_hotw_antifreeze_IBH_need
        )
        {
            /* IBHֱ�ӿ��� */
            SETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_STA_HOTW_ANTIFREEZE);
        }
        else
        {
            CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_STA_HOTW_ANTIFREEZE);
        }

        /* IBH��λ���� */
        if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_STA_HOTW_ANTIFREEZE))
        {
            level = MAX(level, IBH_L3);
        }
    }

    /* �̼�ʱ���� */
    {
        if (TF_OPEN_IBH)
        {
            /* IBHֱ�ӿ��� */
            SETBIT_N(fg_IBH_on, IBH_ON_SIG_TF);
        }
        else
        {
            CLRBIT_N(fg_IBH_on, IBH_ON_SIG_TF);
        }

        /* IBH��λ���� */
        if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_TF))
        {
            level = MAX(level, IBH_L3);
        }
    }

    /* ���ȳ�˪ʱ���� */
    if (MODE_IS_HEAT(GET_SUB_SYS(pcb_addr))
    && is_defrost
    && SensorCanUse(pv_TWout2)
    )
    {
        /* ����/�ر��¶ȼ��� */
        sv_TWout2_IBH_on = 160;
        sv_TWout2_IBH_off = 200;
       
        /* IBH�������� */
        if (pv_TWout2 <= sv_TWout2_IBH_on)
        {
            SETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_DEFROST);
        }
       
        /* IBH�ر����� */
        if (pv_TWout2 >= sv_TWout2_IBH_off)
        {
            CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_DEFROST);
        }

        /* IBH��λ���� */
        if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_DEFROST))
        {
            level = MAX(level, log_mod_IBH_level(pv_TWout2, sv_TWout2_IBH_off, IBH_ON_SIG_RUN_HEAT_DEFROST));
        }
    }
    else
    {
        CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_DEFROST);
    }

    /* ����ˮ��˪���� */
    if (MODE_IS_HOTW(GET_SUB_SYS(pcb_addr))
    && is_defrost
    && SensorCanUse(pv_THWt)
    )
    {
        /* ����/�ر��¶ȼ��� */
        sv_THWt_IBH_on = 160;
        sv_THWt_IBH_off = 200;

        /* IBH�������� */
        if (pv_THWt <= sv_THWt_IBH_on)
        {
            SETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_DEFROST);
        }

        /* IBH�ر����� */
        if (pv_THWt >= sv_THWt_IBH_off)
        {
            CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_DEFROST);
        }

        /* IBH��λ���� */
        if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_DEFROST))
        {
            level = MAX(level, log_mod_IBH_level(pv_THWt, sv_THWt_IBH_off, IBH_ON_SIG_RUN_HOTW_DEFROST));
        }
    }
    else
    {
        CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_DEFROST);
    }
    
    /* ǿ����ˮģʽʱ���� */
    if (fg_force_hotw_mode_run
    && SensorCanUse(pv_THWt)
    && SensorCanUse(pv_TWout2)
    )
    {
        /* IBH����/�ر��¶ȼ��� */
        sv_THWt_IBH_on = sv_hotw - val_hotw_diff;
        sv_THWt_IBH_off = sv_hotw;

        /* IBH�������� */
        if (pv_THWt < sv_THWt_IBH_on)
        {
            SETBIT_N(fg_IBH_on, IBH_ON_SIG_FORCE_HOTW);
        }

        /* IBH�ر����� */
        if (pv_THWt >= sv_THWt_IBH_off)
        {
            CLRBIT_N(fg_IBH_on, IBH_ON_SIG_FORCE_HOTW);
        }

        /* IBH��λ���� */
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
    /* ǿ��������������� */
    if (fg_force_IBH_run)
    {
        /* �������У�ǿ��������������� */
        if ((fg_zone_A_run || fg_zone_B_run)
        && SensorCanUse(pv_ctrl_ac)
        && !fg_heat_force_IBH_once			/* ��ǿ����һ�� */
        )
        { 
            /* IBH����/�ر��¶ȼ��� */
            sv_TWout2_IBH_on = sv_temp_freq[GET_SUB_SYS(pcb_addr)] - val_IBH_open_diff;
            sv_TWout2_IBH_off = sv_temp_freq[GET_SUB_SYS(pcb_addr)];

            fg_heat_force_IBH_off = FALSE;

            /* IBH�������� */
            if (pv_ctrl_ac < sv_TWout2_IBH_on)
            {
                SETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_FORCE_ON);
            }

            /* IBH�ر����� */
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

        /* ����ˮ���У�ǿ��������������� */
        if (fg_hotw_run
        && SensorCanUse(pv_THWt)
        && SensorCanUse(pv_TWout2)
        && !fg_hotw_force_IBH_once			/* ��ǿ����һ�� */
        )
        {
            /* IBH����/�ر��¶ȼ��� */
            sv_THWt_IBH_on = sv_hotw - val_hotw_diff;
            sv_THWt_IBH_off = sv_hotw;

            fg_hotw_force_IBH_off = FALSE;
            
            /* IBH�������� */
            if (pv_THWt < sv_THWt_IBH_on)
            {
                SETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_FORCE_ON);
            }
        
            /* IBH�ر����� */
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

        /* �������Ƕ�Ӧģʽ���Ӧ���� */
        if (!MODE_IS_HEAT(GET_SUB_SYS(pcb_addr)))
        {
            CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_FORCE_ON);
        }

        if (!MODE_IS_HOTW(GET_SUB_SYS(pcb_addr)))
        {
            CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_FORCE_ON);
        }

        /* IBH��λ���� */
        if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_FORCE_ON))
        {
            level = MAX(level, log_mod_IBH_level(pv_ctrl_ac, sv_TWout2_IBH_off, IBH_ON_SIG_RUN_HEAT_FORCE_ON));
        }

        if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_FORCE_ON))
        {
            level = MAX(level, log_mod_IBH_level(pv_THWt, sv_THWt_IBH_off, IBH_ON_SIG_RUN_HOTW_FORCE_ON));
        }
            
        /* ��ǿ���ж� */
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
    /* ȡ�����뾯�� */
    if (fg_heat_force_IBH_off);
    if (fg_hotw_force_IBH_off);
    if (fg_heat_force_IBH_once);
    if (fg_hotw_force_IBH_once);

    /* �������У�ǿ��������������� */
    if (fg_force_IBH_run
    && MODE_IS_HEAT(GET_SUB_SYS(pcb_addr))
    && SensorCanUse(pv_ctrl_ac)
    )
    { 
        /* IBH����/�ر��¶ȼ��� */
        sv_TWout2_IBH_on = sv_temp_freq[GET_SUB_SYS(pcb_addr)] - val_IBH_open_diff;
        sv_TWout2_IBH_off = sv_temp_freq[GET_SUB_SYS(pcb_addr)];

        /* IBH�������� */
        if (pv_ctrl_ac < sv_TWout2_IBH_on)
        {
            SETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_FORCE_ON);
        }

        /* IBH�ر����� */
        if (pv_ctrl_ac >= sv_TWout2_IBH_off)
        {
            CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HEAT_FORCE_ON);
            fg_force_IBH_off = TRUE;
        }

        /* IBH��λ���� */
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
    
    /* ����ˮ���У�ǿ��������������� */
    if (fg_force_IBH_run
    && MODE_IS_HOTW(GET_SUB_SYS(pcb_addr))
    && SensorCanUse(pv_THWt)
    && SensorCanUse(pv_TWout2)
    )
    {    
        /* IBH����/�ر��¶ȼ��� */
        sv_THWt_IBH_on = sv_hotw - val_hotw_diff;
        sv_THWt_IBH_off = sv_hotw;
        
        /* IBH�������� */
        if (pv_THWt < sv_THWt_IBH_on)
        {
            SETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_FORCE_ON);
        }
    
        /* IBH�ر����� */
        if (pv_THWt >= sv_THWt_IBH_off)
        {
            CLRBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_FORCE_ON);
            fg_force_IBH_off = TRUE;
        }

        /* IBH��λ���� */
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

    /* ��ʱɱ��ʱ���� */
    if ((GET_SUB_SYS(pcb_addr) == SUB_SYS_AC_HOTW)
    && fg_timed_disinfect_run//fg_timed_disinfect_IBH_need
    )
    {
        #define DISINFECT_STAGE_1   1		/* fg_timed_disinfect_IBH_need==FALSE */
        #define DISINFECT_STAGE_2   2		/* fg_timed_disinfect_IBH_need==TRUE */
    
        //static BOL disinfect_stage_1_to_2 = FALSE;		/* ɱ���׶�1���ɵ�2��־����ѯ��λ */
        static BOL disinfect_stage_2_to_1 = FALSE;		/* ɱ���׶�2���ɵ�1��־����ѯ��λ������ʹ�ò��ᴥ�� */
        static U08 disinfect_stage_bak    = DISINFECT_STAGE_1;		/* ɱ���׶θ��� */

        /* ����/�ر��¶ȼ��� */
        sv_THWt_IBH_on = val_ster_temp - val_hotw_diff;
        sv_THWt_IBH_off = val_ster_temp;
        
        /* ɱ���׶ο�Խ�ж� */
        if (!fg_timed_disinfect_IBH_need)	/* �׶�1 */
        {
            if (disinfect_stage_bak == DISINFECT_STAGE_2)
            {
                /* ����������׶�2��Խ���׶�1 */
                disinfect_stage_2_to_1 = TRUE;
                disinfect_stage_bak = DISINFECT_STAGE_1;
            }
        }
        else                                /* �׶�2 */
        {
            if (disinfect_stage_bak == DISINFECT_STAGE_1)
            {
                /* ����������׶�1��Խ���׶�2 */
                //disinfect_stage_1_to_2 = TRUE;
                disinfect_stage_bak = DISINFECT_STAGE_2;
            }
        }
    
        /* �׶ο�Խ���������׶�1����IBH������Ϊ�Ƕ�ʱɱ����IBH���� */
        if (disinfect_stage_bak == DISINFECT_STAGE_1)/* �׶�1 */
        {
//            /* ɱ���׶�1��ˮ����IBH */
//            if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_NORMAL)
//            ||  GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL)
//        	||	GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_ABNORMAL)
//        	||  GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_KEEP_ABNORMAL))
//        	{
//        	    /* ��Ϊ�׶�1����IBHҲ��ɱ����IBH���� */
//        	    SETBIT_N(fg_IBH_on, IBH_ON_SIG_DISINFECT);
//
//        	    /* ����/�ر��¶ȼ��� */
//                sv_THWt_IBH_on = 400;
//        		sv_THWt_IBH_off = sv_hotw;
//        	}
//        	else	/* �׶�1��Խ�׶�2ǰһ�̣�����ˮIBH�������㣬��Ҫ������λ�����¶ȣ���������� */
//        	{
//                /* ����/�ر��¶ȼ��� */
//                sv_THWt_IBH_on = val_ster_temp - val_hotw_diff;
//                sv_THWt_IBH_off = val_ster_temp;
//        	}

        	/* ɱ���׶�1��ˮ����IBH */
            if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_NORMAL)
            ||  GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL)
        	||	GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_ABNORMAL)
        	||  GETBIT_N(fg_IBH_on, IBH_ON_SIG_RUN_HOTW_COMP_KEEP_ABNORMAL))
        	{
        	    /* ��Ϊ�׶�1����IBHҲ��ɱ����IBH���� */
        	    SETBIT_N(fg_IBH_on, IBH_ON_SIG_DISINFECT);
        	}

        	/* �׶�2��1��Խʱ�ر�IBH */
        	if (disinfect_stage_2_to_1 == TRUE)
        	{
        	    disinfect_stage_2_to_1 = FALSE;
                CLRBIT_N(fg_IBH_on, IBH_ON_SIG_DISINFECT);
        	}
        }
        else				                        /* �׶�2 */
        {
            /* IBH�������� */
            if (pv_THWt <= sv_THWt_IBH_on
            && cnt_ster_cyc < val_ster_max_cycle
            && cnt_ster_high_temp < val_ster_high_temp_Time
            )
            {
                SETBIT_N(fg_IBH_on, IBH_ON_SIG_DISINFECT);
            }

            /* IBH�ر����� */
            if (cnt_ster_cyc >= val_ster_max_cycle
            || cnt_ster_high_temp >= val_ster_high_temp_Time
            || pv_THWt >= sv_THWt_IBH_off
            )
            {
                CLRBIT_N(fg_IBH_on, IBH_ON_SIG_DISINFECT);
            }
        }

        /* IBH��λ���󣨻Ḳ������ˮ�ļ��㣩 */
        if (GETBIT_N(fg_IBH_on, IBH_ON_SIG_DISINFECT))
        {
            level = MAX(level, log_mod_IBH_level(pv_THWt, sv_THWt_IBH_off, IBH_ON_SIG_DISINFECT));
        }
    }
    else
    {
        CLRBIT_N(fg_IBH_on, IBH_ON_SIG_DISINFECT);
    }

    /* ����������ʱ���� */
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
 �� ��: log_mod_IBH_out()
 �� ��: �����������
 �� ��: type--������������; 
        level--������������; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_mod_IBH_out(U08 type, U08 level)
{
    #define IBH_ADD_ITV 3	/* ����������� */
    
    if (level == IBH_L0		                /* ���� */
    ||  type == IBH_NONE                    /* IBH1/2����δ���� */
    || !fg_PUMPf_run		                /* PUMPfδ���� */
    || !log_mod_pump_is_run()               /* ģ��ˮ��δ�ȶ����� */
    || s_get_error(S_ERR_IBH_OVERLOAD)      /* �������ȹ��� */
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
 �� ��: log_mod_IBH()
 �� ��: �������ȿ���
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_mod_IBH(void)
{
    U08 IBH_level = log_mod_IBH_need();
    
    log_mod_IBH_out(IBH_cfg_type, IBH_level);
}

#ifdef M_DEBUG
/*******************************************************************************
 �� ��: log_mod_debug()
 �� ��: �����ڲ�����ʱ�鿴״̬ʹ��
 �� ��: ��
 �� ��: ��
 �� ��: ��
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
 �� ��: log_mod_HT()
 �� ��: ��������ȴ�����
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_mod_HT(void)
{
    #define OPEN_ENV                (30)    // ��������������
    #define CLOSE_EVN               (50)    // �رշ���������
    #define OPEN_FREZZING_TEMP      (50)    // ���������������¶�
    #define CLOSE_FREZZING_TEMP     (100)   // �رշ����������¶�

    #define COMP_OFF_TIME1          (2*60)  // ͣ��2min��
    #define COMP_OFF_TIME2          (30*60) // ͣ��30min��

    static BOL fg_open = FALSE;
    I16 pv_antifreeze = SENSOR_NOEXIST;   /* �����¶� */
    I16 pv_TA = SENSOR_NOEXIST;

    /* ��ȡ�����¶�&�����¶� */
    pv_antifreeze = fun_pv_min(pv_cell_TWin1(pcb_addr), pv_cell_TWout1(pcb_addr));
    pv_TA = pv_cell_TA(pcb_addr);

    if (!SensorCanUse(pv_antifreeze)
    && !SensorCanUse(pv_TA)
    )
    {
        /* �����¶Ⱥͻ����¶ȶ�����ʱ����ѹ�����йر� */
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
        /* �����¶ȹ��ϣ����¿���ʱ */
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
        /* �����¶ȿ��ã����²�����ʱ */
        if (pv_antifreeze >= CLOSE_FREZZING_TEMP)
        {
            fg_open = FALSE;
        }
        else if (pv_antifreeze < OPEN_FREZZING_TEMP)
        {
            fg_open = TRUE;
        }
    }
    else					/* ������ʱ */
    {
        /* ����ģʽ�� */
        if ((MODE_IS_COOL(GET_SUB_SYS(pcb_addr))) && (cm_time[0].off <= COMP_OFF_TIME2))
        {
            if (cm_time[0].off <= COMP_OFF_TIME1)// 2min��
            {
                fg_open = FALSE;
            }
            else    // 2~30min��
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
        else				/* ���ఴ�����·�ʽ���з��� */
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
    ����: �߼�����
/==========================================*/
void log_mod(void)
{
    log_mod_begin();
    
    if (power_time < POWER_ON_INTO_WORK) return;  

    log_mod_cmd();               /* ����� */
//  log_mod_Heater_Valve();  

    log_mod_PUMPf();		/* ��Ƶˮ�� */
    log_mod_IBH();			/* �������� */
    log_mod_HT();			/* �������ȴ� */

    Logic_Anti_Icing_Heater();  /* ���̵����*/
    log_mod_comp();

    /* ģ����û�ͨѶ����ʱ���������ϵͳ */
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

    /* ģ�鹦��ͳ��:�õ�get_output����ŵ�log_mod_end֮�� */
    log_mod_elec();			

#ifdef M_DEBUG
    log_mod_debug();
#endif
}


