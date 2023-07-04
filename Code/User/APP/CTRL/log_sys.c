#define LOG_SYS_GLOBAL

#include "includes.h"

/* ========================================================================================== */

#define JOINT_ENERGY_CTRL        (0)          /* �����ܵ� */
#define GROUP_ENERGY_CTRL        (1)          /* �����ܵ� */
static U08 energy_ctrl_type;                  /* �ܵ����� */

#define ANTI_ALL                    0           //��������
#define ANTI_NON_ENV                1           //�����ǻ����¶ȵķ���
#define ANTI_UNUSUAL                2           //���������ˮ��һֱ��

/* ˮ�ü�ʱ���� */
#define PM_F_ANY                   (0)		/* ����ģ���Ƶˮ�� */
#define PM_RET                     (1)		/* �»�ˮ�� */
#define PM_F_HW_RATFZ              (2)		/* ��������̬��ˮˮ·���������п�����ˮģ���Ƶˮ�� */
#define PM_F_NHW_RATFZ             (3)		/* ��������̬�յ�ˮ·���������в�������ˮģ���Ƶˮ�� */
#define PM_F_AC_HW                 (4)		/* ���мȿ�����ˮ�ֿ�����(������)ģ��ı�Ƶˮ�� */
#define PM_O_TATFZ                 (5)      /* ����A����ʱ��������ѭ���� */
#define PM_M_TATFZ                 (6)      /* ����B����ʱ�����Ļ��ˮ�� */
#define PM_HW_TATFZ                (7)		/* ���пɲ�����ˮ��ʱ������Ƶˮ�� */
#define PM_MAX                     (8)
//#define AC_WT                1   /* ��ˮ������������ʱ�� */


/* �ⲿ��Դ */
static RTIME AHS_time;

#define HEAT_ANTI_TEMP_DIFF         (10*2)      //���ȷ����²�
#define EXIT_ANTI_ENV_DIFF          (10*1)      //�˷������²�
#define EXIT_ANTI_TEMP_DIFF         (10*9)      //�˷���ˮ���²�

#define ANTI_UNUSUAL_DLY       600	                /* �������ʱ�յ�/��ˮˮ·�л�ʱ�� */
#define ANTI_PRE_DLY           180                  /* ����ʱˮ��Ԥ��ʱ��(��) */   
#define ANTI_CHG_DLY           900                  /* ����ʱˮ·ǿ���л�ʱ��(��) */ 
#define ANTI_EXIT_DLY          (ANTI_CHG_DLY*4)     /* ����ǿ���˳�ʱ��(��) */ 

//#define val_defrost_comp_max              (50)        /* ��˪ѹ���ٷ��� */



#define  cm_runtime(x, y)           (cell[x].cell_runtime[y])

static BOOL fg_have_stop;
static U08 ctrl_mode_bak[SUB_SYS_TYPE_MAX];


GSTATIC U16 power_down_time_space = 0xffff;         /* ����ʱ��洢�����ʱ(��һ���ϵ�Ԥ����ɺ󣬱���һ��) */

//static U08 bak_ac_need =_KEEP;
/* ========================================================================================== */
#define QUICK_PERIOD    1   /* ��ͣ�������� */
#define OPEN_PERIOD     4   /* ������������ */
static U16 m_ctrl_time;		/* ��ģ���ܵ��������ڼ�ʱ */
static U16 s_ctrl_time;		/* ��ģ���ܵ��������ڼ�ʱ */
static U16 m_ctrl_period;	/* ��ģ���ܵ��������� */
static U16 s_ctrl_period;	/* ��ģ���ܵ��������� */

/* ========================================================================================== */

static U08 air_pump_dly[SUB_SYS_TYPE_MAX];	/* ��ϵͳ��Ƶˮ���л������͹ر���ʱ */
static U16 pump_inter_on_off_time[SUB_SYS_TYPE_MAX];      //ˮ�ü������ʱ��
//static BOL fg_ac_need_enable[SUB_SYS_TYPE_MAX] = {TRUE, TRUE};     // �����������(����ˮ�ü�Ъͣ��ģʽ)

static U08 cnt_elec_stop = 0xff;
static U16 cnt_send_fan_stop  = 0;  // �ͷ����ͣ��ʱ
static U16 avoid_cold_fan_cnt;      // ������ʱ

//static BOOL fg_ws_pm_open;                /* ��ȴˮ�ñ�־ */
//static BOOL fg_ac_pm_open;              /* �յ����ѿ���(����ˮ�������ʱ����λ) */
static RTIME pump_time[PM_MAX];   /* 0: �յ���; 1:��ȴ��*/   
U08 m_cm_time_on[MAX_PCB][MAX_COMP];    /* ѹ������ʱ���ʱ */


/* ========================================================================================== */
/* �������� */
enum STATUS_ANTIFREEZE 
{
    ANTIFREEZE_IDLE, 
    ANTIFREEZE_PUMP, 
    ANTIFREEZE_DEAL, 
    ANTIFREEZE_OPEN_COMP, 
    ANTIFREEZE_CLOSE_COMP, 
    ANTIFREEZE_CLOSE_PUMP
};
static U08 idle_sta_antifreeze_status;		/* ����̬����״̬ */
static U08 run_sta_ac_antifreeze_status;	/* ����̬�յ�����״̬ */
static U08 run_sta_hotw_antifreeze_status;	/* ����̬��ˮ����״̬ */

static U16  freeze_energy_time[PM_MAX];         /* �����������ڼ�ʱ�յ���(0: �յ���; 2: ˮԴ��) */
static U16  freeze_act_time;         /* �����������ڼ�ʱ�յ���(0: �յ���; 2: ˮԴ��) */

// ��ʱ�������� ����ʹ��GetTime����������������� 
//static U16 commu_cmd_comp_mode_interval;            /* �����ʱ���� */
static U16 commu_cmd_comp_all_stop_interval;        /* �����ʱ���� */ 
static U16 commu_cmd_err_reset_interval;            /* �����ʱ���� */
static U16 commu_cmd_stop_all_mac_interval;         /* �����ʱ���� */
static U16 commu_cmd_fac_para_init_interval;        /* �����ʱ���� */ 
static U16 commu_cmd_invt_init_interval;            /* �����ʱ���� */ 
static U16 commu_cmd_limit_para_init_interval;      /* �����ʱ���� */ 
static U16 commu_cmd_password_save_interval;        /* �����ʱ���� */ 
static U16 commu_cmd_runtime_init_interval;         /* �����ʱ���� */
static U16 commu_cmd_systime_init_interval;         /* �����ʱ���� */
static U16 commu_cmd_systime_save_interval;         /* �����ʱ���� */
static U16 commu_cmd_comp_quik_stop_interval[MAX_PCB][MAX_COMP];         /* �����ʱ���� */
static U16 commu_cmd_comp_system_stop_interval[MAX_PCB][MAX_COMP];       /* �����ʱ���� */  
static U16 commu_cmd_comp_allow_defrost_interval[MAX_PCB][MAX_COMP];     /* �����ʱ���� */    
static U16 commu_cmd_comp_force_defrost_interval[MAX_PCB][MAX_COMP];     /* �����ʱ���� */ 
static U16 commu_cmd_cell_open[MAX_PCB];
static U16 commu_cmd_cell_close[MAX_PCB];

U08  ac_heater_dly;         /* ��˪����ʱ���յ����ȼ�ʱ */
U16  sys_have_run_time;         /* ϵͳ����ʱ���ʱ */

//static BOOL fg_solar_limit_comp_run = FALSE;    // ̫��������ʱ����ѹ��������ˮ
static U16 cnt_HDIsl_on = 0;
static U16 cnt_HDIsl_off = 0xffff;

static U16 cnt_TSL_on = 0;
static U16 cnt_TSL_off = 0xffff;

static U08 cnt_IBH_on;
static U08 cnt_test_run_hotw_TBH_on;

static U08 m_run_mode_need[MAX_PCB][MAX_COMP];       /* ѹ������ģʽ(ϵͳʹ��) */
static U08 fg_need_comp_to_run[MAX_COMP];	/* ѹ���п�������(����ͺ�ñ�־����) */

//static U16 fg_cell_on[_DIVUCARRY(MAX_PCB, 16)];				/* ģ�鿪����־(����ͺ�ñ�־����) */
//static U16 fg_cell_off[_DIVUCARRY(MAX_PCB, 16)];			/* ģ��رձ�־(����ͺ�ñ�־����) */

//static RTIME comp_time;         /* ѹ��ʱ�� */

static RTIME floor_preheat_time;
static U16 peak_elec_air_runtime;

static BOL fg_defrost_run[SUB_SYS_TYPE_MAX];/* ��˪���б�־ */

//static BOL fg_00_comp_normal;				/* ѹ������״̬��־ */
//static BOL fg_00_comp_abnormal;				/* ѹ���쳣״̬��־ */

static BOL fg_ac_hotw_comp_normal;          /* ��ˮģ��ѹ������״̬��־ */
static BOL fg_ac_hotw_comp_abnormal;        /* ��ˮģ��ѹ���쳣״̬��־ */

static U08 fg_TA_limit_comp_open[SUB_SYS_TYPE_MAX];           /* �����¶�����ѹ�������� */
static BOL fg_HDIsl_limit_comp_open;                        /* ̫�����ź�����ѹ�������� */
static BOL fg_TSL_limit_comp_open;                          /* ̫�����¶�����ѹ�������� */
static BOL fg_floor_preheat_limit_comp_open;                /* ��ůԤ������ѹ�������� */

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
 �� ��: log_sys_init()
 �� ��: ϵͳ������ʼ��
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_init(void)
{
    power_time = 0;         //�ϵ�ʱ��
    punp_fg = FALSE;        //���Ա�־ 
    test_allow = FALSE;     //��������
//    disp_mode = MODE_HEAT;
    log_sys_init_work();
}

/*****************************************************
    ���ܣ�ѹ��ģʽ����
******************************************************/
void cmd_comp_mode(U08 pcb_num, U08 to_mode)
{
    cell[pcb_num].cell_cmd[MAX_COMP] |= ((to_mode&0x07)<<CMD_MODE);   /* ���ϸ�λ���� */
    SET_BIT(cmd_modify, pcb_num);   /* �����޸ı�ʶ */
}
/*****************************************************
    ���ܣ�ѹ��������
******************************************************/
void cmd_comp_run(U08 pcb_num, U08 comp_num, U08 to_mode)
{
    cell[pcb_num].cell_cmd[comp_num] |= ((to_mode&0x07)<<CMD_CM_MODE);         /* ����ģʽ */
    cell[pcb_num].cell_cmd[comp_num] |= CMD_OPEN_COMP;   /* ��ѹ������ */
    SET_BIT(cmd_modify, pcb_num);   /* �����޸ı�ʶ */
}
/*****************************************************
    ���ܣ�ѹ��������
******************************************************/
void cmd_comp_close(U08 pcb_num, U08 comp_num)
{
    cell[pcb_num].cell_cmd[comp_num] |= CMD_CLOSE_COMP;   /* ��ѹ������ */
    
    SET_BIT(cmd_modify, pcb_num);   /* �����޸ı�ʶ */
}
/*****************************************************
    ���ܣ�ѹ����ͣ����
******************************************************/
void cmd_comp_quik_stop(U08 pcb_num, U08 comp_num)
{
    if (commu_cmd_comp_quik_stop_interval[pcb_num][comp_num] > 10)
    {
        if (M_CM_GetStatus(pcb_num, comp_num, COMP_RUN_NEED))
        {
            cell[pcb_num].cell_cmd[comp_num] |= CMD_QUICK_STOP_COMP;   /* ��ͣѹ������ */
            
            SET_BIT(cmd_modify, pcb_num);   /* �����޸ı�ʶ */
        }
        commu_cmd_comp_quik_stop_interval[pcb_num][comp_num] = 0;
    }
}
/*****************************************************
    ���ܣ�ͣѹ��ϵͳ����
******************************************************/
void cmd_comp_system_stop(U08 pcb_num, U08 comp_num)
{
    if (commu_cmd_comp_system_stop_interval[pcb_num][comp_num] > 10)
    {
        cell[pcb_num].cell_cmd[comp_num] |= CMD_STOP_COMP_SYSTEM;   /* ͣѹ��ϵͳ���� */
            
        SET_BIT(cmd_modify, pcb_num);   /* �����޸ı�ʶ */
        commu_cmd_comp_system_stop_interval[pcb_num][comp_num] = 0;
    }
}
/*****************************************************
    ���ܣ�ѹ���л�ģʽ
******************************************************/
//void cmd_comp_switch(U08 pcb_num, U08 comp_num, U08 to_mode)
//{
//  cell[pcb_num].cell_cmd[comp_num] |= to_mode;           /* ����ģʽ */
//  cell[pcb_num].cell_cmd[comp_num] |= CMD_SWITCH_COMP;   /* ѹ���л�ģʽ���� */
//  
//  SET_BIT(cmd_modify, pcb_num);   /* �����޸ı�ʶ */
//}
/*****************************************************
    ���ܣ�ѹ����˪����
        ˵���� ��˪������ָ�ӻ������˪���������͵������˪ָ��
******************************************************/
void cmd_comp_allow_defrost(U08 pcb_num, U08 comp_num)
{
    if (commu_cmd_comp_allow_defrost_interval[pcb_num][comp_num] > 10)
    {
        cell[pcb_num].cell_cmd[comp_num] |= CMD_ALLOW_DEFROST;   /* ѹ�������˪���� */
//      cell[pcb_num].cell_cmd[comp_num] |= ((U16)allow_num << CMD_ALLOW_DEFRO_NUM); /* �����˪ѹ���� */
        
        SET_BIT(cmd_modify, pcb_num);                           /* �����޸ı�ʶ */
        commu_cmd_comp_allow_defrost_interval[pcb_num][comp_num] = 0;     /* ���¼�ʱ */
        
        //fg_trans_control_cmd = TRUE;
        M_CM_SetStatus(pcb_num, comp_num, COMP_DEFROST);    /* ͬ����־ */
//      SETBIT(fg_comp_defrost[comp_num], pcb_num);  /* ���������λ������־ */
    }
}
/*****************************************************
    ���ܣ�ѹ��ǿ�Ƴ�˪����
        
******************************************************/
void cmd_comp_force_defrost(U08 pcb_num, U08 comp_num)
{
    if (commu_cmd_comp_force_defrost_interval[pcb_num][comp_num] > 5)
    {
        cell[pcb_num].cell_cmd[comp_num] |= CMD_FORCE_DEFROST;   /* ѹ����˪���� */
        
        SET_BIT(cmd_modify, pcb_num);   /* �����޸ı�ʶ */
        commu_cmd_comp_force_defrost_interval[pcb_num][comp_num] = 0;
    }
}
/*****************************************************
    ���ܣ�ѹ��ȫͣ����
        ˵���� ���������ڹ㲥����
******************************************************/
void cmd_stop_all_comp(void)
{
    if (commu_cmd_comp_all_stop_interval > 3)
    {
        cell[MAX_PCB].cell_cmd[MAX_COMP] |= CMD_STOP_ALL_COMP;   /* ѹ��ȫͣ���� */
        //      cell[0].cell_cmd[MAX_COMP] |= CMD_STOP_ALL_COMP;   /* ѹ��ȫͣ���� */
        cmd_broadcast = 1;        /* �ù㲥��־ */
        commu_cmd_comp_all_stop_interval = 0;
    }
}

/*****************************************************
    ���ܣ� ���ϸ�λ����
        ˵���� ���������ڹ㲥����
               10��ŷ���һ�Σ���ֹ�����ķ���Ӱ���������ͨѶ
******************************************************/
void cmd_err_reset(void)
{
    if (commu_cmd_err_reset_interval > 5)
    {
        cell[MAX_PCB].cell_cmd[MAX_COMP] |= CMD_ERR_RESET;   /* ���ϸ�λ���� */
        //      cell[0].cell_cmd[MAX_COMP] |= CMD_ERR_RESET;        /* ���ϸ�λ���� */
        cmd_broadcast = 1;        /* �ù㲥��־ */
        commu_cmd_err_reset_interval = 0;
        key_reset = FALSE;
    }
}
/*****************************************************
    ���ܣ� ��������ȫͣ����
        ˵���� ���������ڹ㲥����
******************************************************/
void cmd_stop_all_mac(void)
{
    if (commu_cmd_stop_all_mac_interval > 3)
    {
        cell[MAX_PCB].cell_cmd[MAX_COMP] |= CMD_STOP_ALL_MAC;   /* ��������ȫͣ���� */
        //      cell[0].cell_cmd[MAX_COMP] |= CMD_STOP_ALL_MAC;   /* ��������ȫͣ���� */
        cmd_broadcast = 1;        /* �ù㲥��־ */
        commu_cmd_stop_all_mac_interval = 0;
    }
}
/*****************************************************
    ���ܣ� ���Ҳ�����ʼ������
        ˵���� ���������ڹ㲥����
               10��ŷ���һ�Σ���ֹ�����ķ���Ӱ���������ͨѶ
******************************************************/
void cmd_fac_para_init(void)
{
    if (commu_cmd_fac_para_init_interval > 10)
    {
        cell[MAX_PCB].cell_cmd[MAX_COMP] |= CMD_FAC_PARA_INIT;   /* ��ӻ����ͳ��Ҳ�����ʼ������ */
        //      cell[0].cell_cmd[MAX_COMP] |= CMD_FAC_PARA_INIT;        /* ��ӻ����ͳ��Ҳ�����ʼ������ */
        cmd_broadcast = 1;        /* �ù㲥��־ */
        //      debug_7[0] = key_fac_para_init;
        key_fac_para_init = FALSE;
        //      para_init_all();               /* ����������ʼ�� */
        commu_cmd_fac_para_init_interval = 0;
    }
}

/*******************************************************************************
 �� ��: cmd_invt_init()
 �� ��: ��Ƶ����ʼ������
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void cmd_invt_init(void)
{
    if (commu_cmd_invt_init_interval > 10)
    {
        cell[MAX_PCB].cell_cmd[MAX_COMP] |= CMD_INVT_INIT;   /* ��ӻ����ͱ�Ƶ����ʼ������ */
        cmd_broadcast = 1;        /* �ù㲥��־ */
        key_fac_invt_init = FALSE;
        commu_cmd_invt_init_interval = 0;
    }
}

/*****************************************************
    ���ܣ� ϵͳά��������ʼ������
        ˵���� ���������ڹ㲥����
               10��ŷ���һ�Σ���ֹ�����ķ���Ӱ���������ͨѶ
******************************************************/
void cmd_limit_init(void)
{
    if (commu_cmd_limit_para_init_interval > 10)
    {
        cell[MAX_PCB].cell_cmd[MAX_COMP] |= CMD_LIMIT_INIT;   /* ��ӻ����ͳ��Ҳ�����ʼ������ */
        //      cell[0].cell_cmd[MAX_COMP] |= CMD_LIMIT_INIT;       /* ��ӻ����ͳ��Ҳ�����ʼ������ */
        cmd_broadcast = 1;        /* �ù㲥��־ */
        key_fac_limit_init = FALSE;
        //      para_init_all();               /* ����������ʼ�� */
        commu_cmd_limit_para_init_interval = 0;
    }
}


/*****************************************************
    ���ܣ� ѹ���ۼ�����ʱ���ʼ������
        ˵���� ���������ڹ㲥����
               10��ŷ���һ�Σ���ֹ�����ķ���Ӱ���������ͨѶ
******************************************************/
void cmd_runtime_init(void)
{
    if (commu_cmd_runtime_init_interval > 10)
    { 
        cell[MAX_PCB].cell_cmd[MAX_COMP] |= CMD_RUNTIME_INIT;   /* ��ӻ���������ʱ���ʼ������ */
        //      cell[0].cell_cmd[MAX_COMP] |= CMD_RUNTIME_INIT;   /* ��ӻ���������ʱ���ʼ������ */
        cmd_broadcast = 1;        /* �ù㲥��־ */
        key_runtime_init = FALSE;
        commu_cmd_runtime_init_interval = 0;
    }
}
/*****************************************************
    ���ܣ� ϵͳ�ۼ�����ʱ���ʼ������
        ˵���� ���������ڹ㲥����
               10��ŷ���һ�Σ���ֹ�����ķ���Ӱ���������ͨѶ
******************************************************/
void cmd_systime_init(void)
{
    if (commu_cmd_systime_init_interval > 10)
    {
        cell[MAX_PCB].cell_cmd[MAX_COMP] |= CMD_SYSTIME_INIT;   /* ��ӻ���������ʱ���ʼ������ */
        //        cell[0].cell_cmd[MAX_COMP] |= CMD_SYSTIME_INIT;   /* ��ӻ���������ʱ���ʼ������ */
        cmd_broadcast = 1;        /* �ù㲥��־ */
        key_systime_init = FALSE;
        commu_cmd_systime_init_interval = 0;
    }
}

/*****************************************************
    ���ܣ� �����ʼ������
        ˵���� ���������ڹ㲥����
               10��ŷ���һ�Σ���ֹ�����ķ���Ӱ���������ͨѶ
******************************************************/
void cmd_password_init(void)
{
    if (commu_cmd_password_save_interval > 10)
    {
        cell[MAX_PCB].cell_cmd[MAX_COMP] |= CMD_PASSWORD_INIT;   /* ��ӻ���������ʱ���ʼ������ */
        //      cell[0].cell_cmd[MAX_COMP] |= CMD_PASSWORD_INIT;        /* ��ӻ���������ʱ���ʼ������ */
        cmd_broadcast = 1;        /* �ù㲥��־ */
        key_password_init = FALSE;
        commu_cmd_password_save_interval = 0;
    }
}

/*****************************************************
    ���ܣ� ����ϵͳ�ۼ�����ʱ������
        ˵���� ���������ڹ㲥����
               10��ŷ���һ�Σ���ֹ�����ķ���Ӱ���������ͨѶ
               �㲥��ÿ��ģ�鱣��
******************************************************/
void cmd_systime_save(void)
{
    if (commu_cmd_systime_save_interval > 10)
    {
        cell[MAX_PCB].cell_cmd[MAX_COMP] |= CMD_SYSTIME_SAVE;   /* ��ӻ���������ʱ���ʼ������ */
        //      cell[0].cell_cmd[MAX_COMP] |= CMD_SYSTIME_SAVE;   /* ��ӻ���������ʱ���ʼ������ */
        cmd_broadcast = 1;        /* �ù㲥��־ */
        commu_cmd_systime_save_interval = 0;
    }
}

/*******************************************************************************
 �� ��: cmd_cell_open()
 �� ��: ģ�鿪������
 �� ��: pcb_num--; 
		to_mode--; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void cmd_cell_open(U08 pcb_num, U08 to_mode)
{
    if (commu_cmd_cell_open[pcb_num] > 3)
    {
        cell[pcb_num].cell_cmd[MAX_COMP] |= ((to_mode&0x07)<<CMD_MODE);	/* ����ģʽ */
        cell[pcb_num].cell_cmd[MAX_COMP] |= CMD_OPEN_CELL;
        SET_BIT(cmd_modify, pcb_num);			/* �����޸ı�ʶ */
        commu_cmd_cell_open[pcb_num] = 0;
    }
}

/*******************************************************************************
 �� ��: cmd_cell_close()
 �� ��: ģ��ر�����
 �� ��: pcb_num--; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void cmd_cell_close(U08 pcb_num)
{
    if (commu_cmd_cell_close[pcb_num] > 3)
    {
        cell[pcb_num].cell_cmd[MAX_COMP] |= CMD_CLOSE_CELL;
        SET_BIT(cmd_modify, pcb_num);			/* �����޸ı�ʶ */
        commu_cmd_cell_close[pcb_num] = 0;
    }
}

#if 0
/*******************************************************************************
 �� ��: log_sys_is_silent()
 �� ��: �ж��Ƿ�Ϊ����ģʽ
 �� ��: ��
 �� ��: FALSE��TRUE
 �� ��: ��
*******************************************************************************/
BOL log_sys_is_silent(void)
{
    /* ��ģ����£���ģ�����ȡ */
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
                        I16 bgn_time = val_night_bgn_h*60 + val_night_bgn_m;    /* ת��Ϊ�� */
                        I16 end_time = val_night_end_h*60 + val_night_end_m;    /* ת��Ϊ�� */
                        I16 cur_time = clk->hour*60 + clk->min;         /* ת��Ϊ�� */

                        if (end_time > bgn_time)
                        {
                            if (cur_time >= bgn_time && cur_time < end_time)
                            {
                                fg_silent = TRUE;
                            }
                        }
                        else if (end_time < bgn_time) /* ���� */
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
 �� ��: log_sys_defrost_comp_num()
 �� ��: ��ȡ��ϵͳ�еĳ�˪ѹ����
 �� ��: type--; 
 �� ��: ��˪ѹ����
 �� ��: ��
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
 �� ��: log_sys_merge_mode()
 �� ��: ��ǰģʽת��Ϊѹ��ģʽ��ѹ��ģʽֻ�����䡢����
 �� ��: sys_mode--; 
 �� ��: 
 �� ��: ��
*******************************************************************************/
U08 log_sys_merge_mode(U08 sys_mode)
{
    U08 comp_mode = MODE_NULL;

    switch (sys_mode)
    {
        /* �䷽�� */
        case MODE_COOL:  // ����
            comp_mode = MODE_COOL;
            break;

        /* �ȷ��� */
        case MODE_HEAT:  // ����
        case MODE_HOTW:  // ��ˮ
            comp_mode = MODE_HEAT;
            break;
    }

    return comp_mode;
}

/************************************************************
������ log_sys_cmd_deal
���ܣ� ��������ϴ���
������ ��
����ֵ���� 
*************************************************************/
void log_sys_cmd_deal(U08 pcb_num)
{
    U08 i;
    U08 fg_system_ready;
    U08 sub_sys_type;
    BOL fg_cell_on = FALSE;
    BOL fg_cell_off = FALSE;
    
    fg_system_ready = log_sys_is_rdy();  /* һ��Ҫ�ڡ�if������������øú���, ������ж�ʧ�� */

    /* ȷ����ǰģ��������ϵͳ */
    sub_sys_type = GET_SUB_SYS(pcb_num);

    /* ���� �յ��ÿ������ �� ģ�鿪��/�رձ�־ */
    if (fg_pump_on[sub_sys_type])
    {
        if (!M_CM_GetStatus(pcb_num, MAX_COMP, CELL_PUMP_OPEN))
        {
            if (log_sys_cell_pump_can_open(pcb_num))        /* ģ��ˮ�ÿɿ��� */
            {
                fg_cell_on = TRUE;
            }     
        } 
    }
    else
    {
        if (M_CM_GetStatus(pcb_num, MAX_COMP, CELL_PUMP_OPEN))
        {
            if (log_sys_cell_pump_can_close(pcb_num))       /* ģ��ˮ�ÿɹر� */
            {
                fg_cell_off = TRUE;
            }
        }
    }
    
    /* ѡ��ѹ����������ת���򿪻���Ϊ�˵ȴ�ˮ�õ�ϵͳ���������� */
    for(i=0; i<m_val_comp_num(pcb_num); i++)
    {
        /* ѹ������ */
        if (GET_BIT(fg_need_comp_to_run[i], pcb_num))        
        {
            if (fg_system_ready && !STEP_IS_ALARM)
            {
                cmd_comp_run(pcb_num, i, log_sys_merge_mode(m_run_mode_need[pcb_num][i]));
                CLR_BIT(fg_need_comp_to_run[i], pcb_num);   /* ����������������־ */    

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

        /* ģ��ر� */
        if (fg_cell_off)
        {
            cmd_cell_close(pcb_num);
        }
        else
        {
            /* ģ�鿪�� */
            if (fg_cell_on)
            {
                if (fg_system_ready && !STEP_IS_ALARM)
                {
                    cmd_cell_open(pcb_num, log_sys_merge_mode(m_run_mode_need[pcb_num][i]));
                }
            }
        }
    }
    
    /* ģʽ�ı䵥�� */
    if (!log_sys_comp_run_num(sub_sys_type))        /* ����ѹ��ͣ����Ź㲥ģʽ����ά��ͣ��ʱ�� */
    {
        if (ctrl_mode_bak[sub_sys_type] != ctrl_mode[sub_sys_type])
        {
            for (i = PCB_BGN(sub_sys_type); i < PCB_END(sub_sys_type); i++)
            {
                cmd_comp_mode(i, log_sys_merge_mode(ctrl_mode[sub_sys_type]));/* ע��: ѹ��ģʽֻ����������ȣ��м�ת�� */
            }
            
            ctrl_mode_bak[sub_sys_type] = ctrl_mode[sub_sys_type];              /* ����ͳɹ�����ģʽ�������� */
        }
    }
}

/*******************************************************************************
 �� ��: log_sys_get_cmd_mux_bit()
 �� ��: ��ȡһ���������־��ʹ�ú��־����
 �� ��: id--�����־ID; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOL log_sys_get_cmd_mux_bit(U08 id)
{
    BOL result;
    
    if (id >= CMD_MAX) 
    {
        result = FALSE;		/* ������� */
    }
    else
    {
        result = GETBIT_N(cmd_mux, id) ? TRUE : FALSE;
        CLRBIT_N(cmd_mux, id);
    }
    
    return result;
}

/*******************************************
������ log_sys_cmd_get()
���ܣ� �û������ȡ(�߿ؿ��ء��ֲ�������λ����)
������ pcb_num
����ֵ������
********************************************/
void log_sys_cmd_get(U08 pcb_num)
{
    U08 i;
    
    if (cmd_normal[0] & KEY_CMD_RUN)            /* ���� */
    {
        commu_run = TRUE;
    }
    if (cmd_normal[0] & KEY_CMD_STOP)           /* �ػ� */
    {
        commu_stop = TRUE;
    }
    if (cmd_normal[0] & KEY_CMD_RESET)          /* ��λ */
    {
        key_reset = TRUE;
    }
    if (cmd_normal[0] & KEY_CMD_WARM_CANCEL)    /* ȡ��Ԥ�� */
    {
        key_warm_cancel = TRUE;
    }
    if (cmd_normal[0] & KEY_CMD_SOUND_CANCEL)   /* ���� */
    {
        key_sound_cancel = TRUE;
    }
//    if (cmd_normal[0] & KEY_CMD_ENTER_FLOOR_DRYUP)   /* �����ů��ɹ��� */
//    {
//        key_enter_floor_dryup = TRUE;
//    }
//    if (cmd_normal[0] & KEY_CMD_FORCE_IBH)   /* ǿ�ƿ���IBH */
//    {
//        key_IBH_force_on = TRUE;
//    }
//    if (cmd_normal[0] & KEY_CMD_FORCE_TBH)   /* ǿ�ƿ���TBH */
//    {
//        key_TBH_force_on = TRUE;
//    }
//    if (cmd_normal[0] & KEY_CMD_FAST_HOTW)   /* ǿ������ˮ(��������ˮ) */
//    {
//        key_fast_hotw_on = TRUE;
//    }
    
    cmd_normal[0] = 0;                       /* �������� */

    if (GET_BIT(cmd_normal[1], pcb_num))       /* ǿ�Ƴ�˪ */
    {
        SETCMD_N(ABILITY_TEST_OFF);			/* �ֶ���˪�˳��������� */
        for (i=0; i<m_val_comp_num(pcb_num); i++)
        {
            cmd_comp_force_defrost(pcb_num, i);
        }
    }
    CLR_BIT(cmd_normal[1], pcb_num);           /* �������� */
    
    if (cmd_system[0] & KEY_CMD_USER_PARA_INIT)       /* �û�������ʼ�� */
    {
        key_user_para_init = TRUE;
    }
    if (cmd_system[0] & KEY_CMD_FAC_PARA_INIT)       /* ���Ҳ�����ʼ�� */
    {
        key_fac_para_init = TRUE;
    }
    if (cmd_system[0] & KEY_CMD_LIMIT_INIT)         /* ϵͳά��������ʼ�� */
    {
        key_fac_limit_init = TRUE;
    }
    if (cmd_system[0] & KEY_CMD_FAC_INVT_INIT)       /* ��Ƶ����ʼ�� */
    {
        key_fac_invt_init = TRUE;
    }
//    if (cmd_system[0] & KEY_CMD_FAC_ABILITY_TEST)       /* ������������ */
//    {
//        key_fac_enter_ability_test = TRUE;
//    }
    if (cmd_system[0] & KEY_CMD_RUNTIME_INIT)       /* �ۼ�����ʱ���ʼ�� */
    {
        key_runtime_init = TRUE;
    }
    if (cmd_system[0] & KEY_CMD_SYSTIME_INIT)       /* ϵͳ�ۼ�����ʱ���ʼ�� */
    {
        key_systime_init = TRUE;
    }
    if (cmd_system[0] & KEY_CMD_PASSWORD_INIT)      // �����ʼ��
    {
        key_password_init = TRUE;
    }
    if (cmd_system[0] & KEY_CMD_ALLDATA_INIT)       // ȫ�����ݳ�ʼ��
    {
        key_alldata_init = TRUE;
    }

    
    cmd_system[0] = 0;                            /* �������� */
    
    /* �������� */
//  if (cmd_system[1] & KEY_CMD_SYS_LIMIT)          /* ʹ�����޵� */
//  {
//      key_sys_limit = TRUE;
//  }
    
//  cmd_system[1] = 0;                            /* �������� */


    /* A������ */
    if (GETCMD_N(ZONE_A_ON))		
    {
        fg_zone_A_on = TRUE;
    }
    /* A���ػ� */
    if (GETCMD_N(ZONE_A_OFF))		
    {
        fg_zone_A_off = TRUE;
    }
    
    if (USE_DUAL_ZONE_CTRL)
    {
        /* B������ */
        if (GETCMD_N(ZONE_B_ON))     
        {
            fg_zone_B_on = TRUE;
        }
        /* B���ػ� */
        if (GETCMD_N(ZONE_B_OFF))		
        {
            fg_zone_B_off = TRUE;
        }
    }
    
    /* ��ˮ���� */
    if (GETCMD_N(HOTW_ON))			
    {
        fg_hotw_on = TRUE;
    }
    /* ��ˮ�ػ� */
    if (GETCMD_N(HOTW_OFF))		
    {
        fg_hotw_off = TRUE;
    }

    /* ����: ��ʱ�»�ˮ�� */
    if (GETCMD_N(TIMED_PUMPRET_ON))
    {
        fg_timed_PUMPret_run = TRUE;
    }
    /* �ر�: ��ʱ�»�ˮ�� */
    if (GETCMD_N(TIMED_PUMPRET_OFF))
    {
        fg_timed_PUMPret_run = FALSE;
    }

    /* ����: ǿ����ˮģʽ */
    if (GETCMD_N(FORCE_HOTW_ON))
    {
        /* ǿ����ˮģʽ�ɽ��� */
        if (log_sys_can_enter_force_hotw())
        {
            fg_force_hotw_mode_run = TRUE;
            /* ��ˮδ���У�����ˮ���� */
            if (!fg_hotw_run)
            {
                fg_hotw_on = TRUE;
            }
        }
    }
    /* �ر�: ǿ����ˮģʽ */
    if (GETCMD_N(FORCE_HOTW_OFF))
    {
        fg_force_hotw_mode_run = FALSE;
    }

    /* ����: �»�ˮ��ɱ�� */
    if (GETCMD_N(PUMPRET_DISINFECT_ON))
    {
        fg_PUMPret_disinfect_run = TRUE;
    }
    /* �ر�: �»�ˮ��ɱ�� */
    if (GETCMD_N(PUMPRET_DISINFECT_OFF))
    {
        fg_PUMPret_disinfect_run = FALSE;
    }

    /* ����: �����뿪ģʽ */
    if (GETCMD_N(HOLIDAY_AWAY_ON))
    {
        if (fg_holiday_home_run == FALSE	/* δ��������ڼ�ģʽ */
        && fg_timed_disinfect_run == FALSE	/* δ���붨ʱɱ������ */
        )
        {
            fg_holiday_away_run = TRUE;
            /* A��δ���У���A������ */
            if (!fg_zone_A_run
            && fg_holiday_away_heat_mode_used
            )
            {
                fg_zone_A_on = TRUE;
            }
            /* B��δ���У���B������ */
            if (!fg_zone_B_run
            && fg_holiday_away_heat_mode_used
            )
            {
                fg_zone_B_on = TRUE;
            }
            /* ��ˮδ���У�����ˮ���� */
            if (!fg_hotw_run
            && fg_holiday_away_hotw_mode_used
            )
            {
                fg_hotw_on = TRUE;
            }
        }
    }
    /* �ر�: �����뿪ģʽ */
    if (GETCMD_N(HOLIDAY_AWAY_OFF))
    {
        fg_holiday_away_run = FALSE;
    }
    
    /* ����: �����ڼ�ģʽ */
    if (GETCMD_N(HOLIDAY_HOME_ON))
    {
        if (fg_holiday_away_run == FALSE	/* δ��������뿪ģʽ */
        && fg_timed_disinfect_run == FALSE	/* δ���붨ʱɱ������ */
        )
        {
            fg_holiday_home_run= TRUE;
        }
    }
    /* �ر�: �����ڼ�ģʽ */
    if (GETCMD_N(HOLIDAY_HOME_OFF))
    {
        fg_holiday_home_run = FALSE;
    }

    /* ʹ��: �����������ģʽ */
    if (GETCMD_N(HOLIDAY_AWAY_HEAT_ON))
    {
        fg_holiday_away_heat_mode_used = TRUE;

        /* ����ģʽʹ�ã�������ģʽ���趨Ϊ"����ģʽ" */
        para_set(P_GaI(sv_mode), SET_HEAT);
    }
    /* ����: �����������ģʽ */
    if (GETCMD_N(HOLIDAY_AWAY_HEAT_OFF))
    {
        fg_holiday_away_heat_mode_used = FALSE;
    }

    /* ʹ��: ���������ˮģʽ */
    if (GETCMD_N(HOLIDAY_AWAY_HOTW_ON))
    {
        fg_holiday_away_hotw_mode_used = TRUE;
    }
    /* ����: ���������ˮģʽ */
    if (GETCMD_N(HOLIDAY_AWAY_HOTW_OFF))
    {
        fg_holiday_away_hotw_mode_used = FALSE;
    }
    
    /* ����: ǿ��ˮ����� */
    if (GETCMD_N(FORCE_TBH_ON))
    {
        fg_force_TBH_run = TRUE;
    }
    /* �ر�: ǿ��ˮ����� */
    if (GETCMD_N(FORCE_TBH_OFF))
    {
        fg_force_TBH_run = FALSE;
    }

    /* ����: ǿ���������� */
    if (GETCMD_N(FORCE_IBH_ON))
    {
        fg_force_IBH_run = TRUE;
    }
    /* �ر�: ǿ���������� */
    if (GETCMD_N(FORCE_IBH_OFF))
    {
        fg_force_IBH_run = FALSE;
    }

    /* ����: ǿ���ⲿ��Դ */
    if (GETCMD_N(FORCE_AHS_ON))
    {
        fg_force_AHS_run = TRUE;
    }
    /* �ر�: ǿ���ⲿ��Դ */
    if (GETCMD_N(FORCE_AHS_OFF))
    {
        fg_force_AHS_run = FALSE;
    }

    /* ����õ�ǿ����ˮ���� */
    if (GETCMD_N(FREE_ELEC_FORCE_HOTW_ON))
    {
        /* ��ˮδ���У�����ˮ���� */
        if (!fg_hotw_run)
        {
            fg_hotw_on = TRUE;
        }
    }

    /* �߷��õ���ˮ�ػ� */
    if (GETCMD_N(PEAK_ELEC_HOTW_OFF))
    {
        if (fg_hotw_run)
        {
            fg_hotw_off = TRUE;
        }
    }

    /* �߷��õ�յ��ػ� */
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

    /* �͹��õ�յ��ػ� */
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

    /* �͹��õ���ˮ�ػ� */
    if (GETCMD_N(VALLEY_ELEC_HOTW_OFF))
    {
        if (fg_hotw_run)
        {
            fg_hotw_off = TRUE;
        }
    }

    /* �շ� */
    if (GETCMD_N(REFRI_RECY_ON))
    {
        cmd_refri_recy_enter = TRUE;
    }
    if (GETCMD_N(REFRI_RECY_OFF))
    {
        cmd_refri_recy_stop = TRUE;
    }

    /* ����ģʽ */
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

    /* �ֶ����� */
    if (GETCMD_N(TEST_DO_ON))
    {
        test_data.wo.DO_mnl_mode = TRUE;
    }
    if (GETCMD_N(TEST_DO_OFF))
    {
        test_data.wo.DO_mnl_mode = FALSE;
    }

    /* Ԥ�� */
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
    ������ log_sys_cell_can_run
    ���ܣ� �ж�ģ���Ƿ����
    ������ ģ���
    ����ֵ�� ģ��״̬  1: ����; 0: ������
*********************************************************/
BOOL log_sys_cell_can_run(U08 pcb_num)
{
    if (CELL_Is_used(pcb_num)==0)               return FALSE;
    
    if (err_sys_get_s_commu_err(pcb_num))       return FALSE;

    if (m_get_error(pcb_num, S_ERR_EEPROM))     return FALSE;

    return TRUE;
}

/*******************************************************************************
 �� ��: log_sys_cell_pump_can_open()
 �� ��: ģ��ˮ�ÿɿ���
 �� ��: pcb_num--; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
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
 �� ��: log_sys_cell_comp_run()
 �� ��: �ж�ģ���Ƿ���ѹ������
 �� ��: pcb_num--; 
 �� ��: 
 �� ��: ��
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
 �� ��: log_sys_cell_pump_can_close()
 �� ��: ģ��ˮ�ÿɹر�
 �� ��: pcb_num--; 
 �� ��: 
 �� ��: ��
*******************************************************************************/
BOL log_sys_cell_pump_can_close(U08 pcb_num)
{
    if (log_sys_cell_comp_run(pcb_num))             return FALSE;

    return TRUE;
}

/*******************************************************************************
 �� ��: log_sys_cell_forbid_deal()
 �� ��: ����ģ��Ĵ���:ģ����ú󣬷���ģ�������У����3S��һ̨ѹ��
                       ģ����ú������
                       ģ����ú���״̬
 �� ��: pcb_num--ģ�����; 
 �� ��: ��
 �� ��: ��
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

    /* ����ģ�鴦�� */
    for (i=0; i<val_pcb_num; i++)
    {
        if (CELL_Is_used(i)==0)
        {
            for (j=0; j<MAX_ERR_WORD; j++)
            {
                cell[i].cell_error[j] = 0;              /* ģ����ã������ */
            }
            if (!M_CM_GetStatus(i, MAX_COMP, CELL_RUN))   /* ģ��ֹͣ���� */
            {
                for (j=0; j<MAX_STATUS; j++)                /* ģ����ã�ͣ����Ϻ���״̬ */
                {
                    cell[i].cell_status[j] = 0;
					cell[i].cell_relay[j] = 0;
                }
            }
            CLRBIT(slave_commu_error,i);                // ��������ϣ���Ҫ���־ 
        }
    }

    /* ����ģ�鴦�� */
    for (i=val_pcb_num; i<MAX_PCB; i++)
    {
        for (j=0; j<MAX_ERR_WORD; j++)
        {
            cell[i].cell_error[j] = 0;              /* �޴�ģ�� ������� */
        } 
        for (j=0; j<MAX_STATUS; j++)            /* �޴�ģ�飬��״̬ */
        {
            cell[i].cell_status[j] = 0;
            cell[i].cell_relay[j] = 0;
        } 
        CLRBIT(slave_commu_error,i);                // ��������ϣ���Ҫ���־ 
    }
}

/*******************************************************************************
 �� ��: log_sys_comp_mode()
 �� ��: �ж�ѹ����ʹ��ģʽ
 �� ��: pcb_num--; 
		comp_num--; 
		mode--; 
		type--; 
 �� ��: 
 �� ��: ��
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
    ���ܣ��жϴ�ģʽ���Ƿ���ѹ�����Կ���
    ������
    ����ֵ��TRUE: ��ѹ����������; FALSE: ��ѹ����������
    ע�⣺��ģ���С�ѹ������ʱ�������ģʽ���Ĵ���
****************************************************/
BOOL log_sys_have_comp_mode_canuse(U16 mode)
{
    U08 i,j ;
    BOOL fg_have_comp_canuse = FALSE;

    for (i=0; i<val_pcb_num; i++)
    {
        if (!log_sys_cell_can_run(i)) /* ������(���á�ͨѶ����) */
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
������ log_sys_comp_use_status
���ܣ� ѹ����ǰ��ʹ��״̬
       �ɿ����ɹء��ɼ�ͣ����ģʽת��
������ ģ��š�ѹ���š���ѯ����
����ֵ�� ѹ����ʹ��״̬
******************************************************/
BOOL log_sys_comp_use_status(U08 pcb_num, U08 comp_num, U16 attrib)
{
    return (cell[pcb_num].cell_status[comp_num] & (attrib)) ? TRUE : FALSE; 
}

/*********************************************
    ������stop_comp_system
    ���ܣ�ͣѹ��ϵͳ
          ����һ��ͣһ��ѹ����ϵͳ����˳��ͣ������������ͣ������������
    ��������
    ����ֵ����
**********************************************/
void log_sys_select_comp_stop(U08 run_type)
{
    static U08 i=0, j=0;    /* i, j�м��书�ܣ���Ϊ�˱������е�ѹ�����Է�ֹͬһ̨ѹ��������ѡ�� */
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
            
            if (run_type == MODE_NULL                       /* ������ģʽ */
                || run_type == M_GET_CompMode(i, k))        /* ģʽƥ�� */
            {
                cmd_comp_system_stop(i, k);          /* ��ͣѹ���� */
                
                if (M_CM_GetStatus(i, k, COMP_RUN))
                {
                    

                    return;                              /* �˳�ѭ�� */
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
 �� ��: log_sys_select_comp_stop_once()
 �� ��: ����ģ������(��/��)��һ����ͣ����ѹ��
 �� ��: mode--ģʽ; 
		type--ģ������(��/��); 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_select_comp_stop_once(U08 mode, U08 type)
{
    U08 i = 0;
    U08 j = 0;

    for (i = PCB_BGN(type); i < PCB_END(type); i++)
    {
        for (j = 0; j < m_val_comp_num(i); j++)
        {
            if (mode == MODE_NULL           /* ������ģʽ */
            || mode == M_GET_CompMode(i, j) /* ģʽƥ�� */
            )
            {
                cmd_comp_system_stop(i, j);          /* ��ͣѹ���� */
            }
        }
    }
}

/*******************************************************************************
 �� ��: log_sys_have_comp_run()
 �� ��: ����ģ������(��/��)���ж�ĳ��ģʽ���Ƿ���ѹ������
 �� ��: mode--ģʽ; 
		type--ģ������(��/��); 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
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
                if (mode == MODE_NULL                       /* ������ģʽ */
                || mode == M_GET_CompMode(i, j))            /* ģʽƥ�� */
                {
                    fg_have_comp_run = TRUE;
                }
            }
        }
    }

    return fg_have_comp_run;
}

/*******************************************************************************
 �� ��: log_sys_have_non_defrosting_comp_run()
 �� ��: ����ģ����ϵͳ����(�յ�+��ˮ/���յ�)���ж�ĳ��ģʽ���Ƿ��зǳ�˪ģ���ѹ������
 �� ��: mode--ģʽ; 
		type--��ϵͳ����(�յ�+��ˮ/���յ�); 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOL log_sys_have_non_defrosting_comp_run(U08 mode, U08 type)
{
    U08 i,j;
    BOL fg_cell_have_comp_run = FALSE;		/* ģ���ж�Ӧģʽѹ�����б�־ */
    BOL fg_cell_is_defrosting = FALSE;		/* ģ���ڳ�˪��־ */
    
    for (i = PCB_BGN(type); i < PCB_END(type); i++)			/* ����ģ�� */
    {
        for (j=0; j<m_val_comp_num(i); j++)	/* ����ģ���ѹ�� */
        {
            if (M_CM_GetStatus(i, j, COMP_RUN)||M_CM_GetStatus(i, j, COMP_RUN_NEED))
            {
                if (mode == MODE_NULL                       /* ������ģʽ */
                || mode == M_GET_CompMode(i, j))            /* ģʽƥ�� */
                {
                    /* ģ���ж�Ӧģʽѹ������ */
                    fg_cell_have_comp_run = TRUE;
                }
            }

            if (M_CM_GetStatus(i, j, COMP_DEFROST))			/* iģ����ѹ����˪ */
            {
                /* ��ѹ����˪����Ϊģ���ڳ�˪ */
                fg_cell_is_defrosting = TRUE;				
            }
        }

        /* ��ģ�鲻�ڳ�˪ �� �ж�Ӧģʽѹ������ */
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
 �� ��: log_sys_is_00_comp_err()
 �� ��: 00#ѹ���Ƿ����(ģʽ�Ƿ����)
 �� ��: ��
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
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
 �� ��: log_sys_is_ac_hotw_comp_err()
 �� ��: ��ˮģ��ѹ���Ƿ�ȫ������(ģʽ�Ƿ����)
 �� ��: ��
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOL log_sys_is_all_ac_hotw_comp_err(void)
{
    U08 hotwIndex;			/* ��ˮģ���±� */
    U08 hotwCompIndex;		/* ��ˮģ���е�ѹ���±� */
    
    /* ������ˮģ�� */
    for (hotwIndex = PCB_BGN(SUB_SYS_AC_HOTW); hotwIndex < PCB_END(SUB_SYS_AC_HOTW); ++hotwIndex)
    {
        /* ������ˮģ���ѹ�� */
        for (hotwCompIndex = 0; hotwCompIndex < m_val_comp_num(hotwIndex); ++hotwCompIndex)
        {
            /* ����һ����ˮģ���ѹ��ģʽ���� ���� �� */
            if (m_comp_use_mode(hotwIndex, hotwCompIndex, ctrl_mode[SUB_SYS_AC_HOTW]))
            {
                return FALSE;
            }
        }
    }

    /* �������ʾ ��ˮģ��ȫ��ѹ��ģʽ������ */
    return TRUE;
}

/***************************************************
    ������log_sys_all_comp_defro
    ���ܣ��ж�ϵͳ���Ƿ�����ѹ������˪
    ��������
    ����ֵ��TRUE: ����ѹ����˪; FALSE: ��ѹ������˪
****************************************************/
BOOL log_sys_all_comp_defro(void)
{
    U08 i,j ;
    BOOL fg_all_defro = FALSE;
    U08 cm_run_num = 0;                 // ����ѹ����
    U08 cm_defrost_num = 0;             // ��˪ѹ����

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
 �� ��: log_sys_have_fan_on()
 �� ��:
 �� ��: ��
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOL log_sys_have_fan_on(void)
{
    BOL result = FALSE;
    
    if (val_pcb_num == 1)
    {
        result = fg_have_fan_on;
    }
    else    /* ���ģ��ʱ������ϵͳ״̬û�з����Ϣ����ʹ��ѹ����Ϣ������ */
    {
        result = log_sys_have_comp_run(MODE_NULL, SUB_SYS_TYPE_MAX);
    }
    return result;
}

/*******************************************************************************
 �� ��: log_sys_have_comp_status()
 �� ��: ����ģ������(��/��)���ж��Ƿ����ĳ��ѹ��״̬
 �� ��: sta--ѹ��״̬; 
		type--ģ������(��/��); 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL log_sys_have_comp_status(U16 sta, U08 type)
{
    U08 i,j;
    BOOL fg_have_sta = FALSE;

    for (i = PCB_BGN(type); i < PCB_END(type); i++)
    {
        /* ģ�鲻���� */
        if (!log_sys_cell_can_run(i))   continue;

        for (j=0; j<m_val_comp_num(i); j++)
        {
            /* ѹ������ */
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
    ���ܣ��ж��Ƿ���ѹ�����Կ���
    ������
    ����ֵ��TRUE: ��ѹ����������; FALSE: ��ѹ����������
****************************************************/
BOOL log_sys_have_comp_can_run(void)
{
    return log_sys_have_comp_status(COMP_RUN, SUB_SYS_TYPE_MAX);
}

/*******************************************************************************
 �� ��: log_sys_comp_run_num()
 �� ��: ͳ�������ϵͳ�����е�ѹ����
 �� ��: type--; 
 �� ��: 
 �� ��: ��
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
 �� ��: log_sys_have_comp_num()
 �� ��: ����ģ������(��/��)��ͳ����Ӧ�����ѹ����
 �� ��: type--ģ������(��/��); 
 �� ��: 
 �� ��: ��
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
 �� ��: log_sys_s3v1_is_hw()
 �� ��: �ж�s3v1�Ƿ�����ˮ��
 �� ��: ��
 �� ��: 
 �� ��: ��
*******************************************************************************/
BOL log_sys_s3v1_is_hw(void)
{
    return fg_S3V1_is_hw;
}

/*******************************************************************************
 �� ��: log_sys_have_AHS_on()
 �� ��: �ж��Ƿ����ⲿ��Դ����
 �� ��: zone--����
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOL log_sys_have_AHS_on(U08 zone)
{
    BOL fg_result = FALSE;
    BOL fg_need = FALSE;
    
    /* ����ˮ�� */
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
 �� ��: log_sys_have_comp_run_in_zone()
 �� ��: �����ж��Ƿ���ѹ������
 �� ��: ��
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOL log_sys_have_comp_run_in_zone(U08 zone)
{ 
    U08 pcb;
    U08 need_type = SUB_SYS_TYPE_MAX;		/* Ĭ��: ȡ����ģ�� */
    BOL fg_result = FALSE;
    BOL fg_none = FALSE;
    U08 cm = 0;
    
    /* ����ˮ�� */
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
    /* �ڿյ��� */
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
 �� ��: log_sys_ac_have_comp_run()
 �� ��: �жϿյ��Ƿ���ѹ������
 �� ��: ��
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOL log_sys_ac_have_comp_run(void)
{
    return log_sys_have_comp_run_in_zone(ZONE_AC);
}

/*******************************************************************************
 �� ��: log_sys_hotw_have_comp_run()
 �� ��: �ж���ˮ�Ƿ���ѹ������
 �� ��: ��
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOL log_sys_hotw_have_comp_run(void)
{
    return log_sys_have_comp_run_in_zone(ZONE_HOTW);
}

/*******************************************************************************
 �� ��: log_sys_have_IBH_on()
 �� ��: �ж��Ƿ��и������ȿ���
 �� ��: ��
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOL log_sys_have_IBH_on(U08 zone)
{ 
    U08 i;
    U08 need_type = SUB_SYS_TYPE_MAX;		/* Ĭ��: ȡ����ģ�� */
    BOL fg_have_IBH_on = FALSE;
    BOL fg_none = FALSE;
    
    /* ����ˮ�� */
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
    /* �ڿյ��� */
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
 �� ��: log_sys_ac_have_IBH_on()
 �� ��: �жϿյ��Ƿ��и������ȿ���
 �� ��: ��
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOL log_sys_ac_have_IBH_on(void)
{
    return log_sys_have_IBH_on(ZONE_AC);
}

/*******************************************************************************
 �� ��: log_sys_hotw_have_IBH_on()
 �� ��: �ж���ˮ�Ƿ��и������ȿ���
 �� ��: ��
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOL log_sys_hotw_have_IBH_on(void)
{
    return log_sys_have_IBH_on(ZONE_HOTW);
}

/*******************************************************************************
 �� ��: log_sys_cell_pumpf_on()
 �� ��: �ж��Ƿ�ģ��ˮ�ÿ���
 �� ��: ��
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
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
 �� ��: log_sys_cell_pump_run()
 �� ��: ģ��ˮ���ȶ�����
 �� ��: pcb_num--; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
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
 �� ��: log_sys_have_cell_pump_run()
 �� ��: ����ģ����ȶ�����
 �� ��: type--; 
 �� ��: 
 �� ��: ��
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
    ������m_have_cell_run
    ���ܣ��ж��Ƿ���ģ��������
    ��������
    ����ֵ��TRUE: ��ģ������; FALSE: ģ��ֹͣ����
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
    ������m_cell_is_err
    ���ܣ��ж�ģ���Ƿ��й���
    ��������
    ����ֵ��TRUE: ģ������; FALSE: ģ��ֹͣ����
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

#define ELC_OFF          0           /* �ر��� */
#define ELC_RUN          1           /* ������ */
#define ELC_KEEP         2           /* ������ */
static U08 cnt_sys_comp_limit_TA_dly[SUB_SYS_TYPE_MAX][CTRL_MODE_MAX];
/*******************************************************************************
 �� ��: log_sys_env_limit_comp_area()
 �� ��: ��ȡ�����¶����Ʒ���
 �� ��: mode--ģʽ; 
		type--��������; 
 �� ��: result--����
 �� ��: ��
*******************************************************************************/
U08 log_sys_env_limit_comp_area(U08 mode, U08 type)
{
    BOL fg_env_limit_used;
    I16 point1,point2,point3,point4; // �Ӵ�С����
    U08 mode_idx = 0;
    U08 grp_idx = 0;
    U08 area_new = AREA_00;
    static U08 area_cur[SUB_SYS_TYPE_MAX][CTRL_MODE_MAX] = {{AREA_00,AREA_00,AREA_00},{AREA_00,AREA_00,AREA_00}};
    static U08 result[SUB_SYS_TYPE_MAX][CTRL_MODE_MAX] = {{ELC_KEEP,ELC_KEEP,ELC_KEEP},{ELC_KEEP,ELC_KEEP,ELC_KEEP}};

    /* ������俪����ctrl_mode��ֵ���¿��ܲ���ʱ */
    if (mode == MODE_NULL)
    {
        return COMP_KEEP_AREA;
    } 

    /* ȷ��ģʽ */
    switch (mode)
    {
        case MODE_COOL:		/* ���� */
            fg_env_limit_used = (val_cool_max_TA == val_cool_min_TA) ? FALSE : TRUE;
            point1 = 10*(COOL_RUN_ENV_HLIMIT);
            point2 = val_cool_max_TA;
            point3 = val_cool_min_TA;
            point4 = 10*(COOL_RUN_ENV_LLIMIT);
            mode_idx = 0;
            break;
            
        case MODE_HEAT:		/* ���� */
            fg_env_limit_used = (val_heat_max_TA == val_heat_min_TA) ? FALSE : TRUE;
            point1 = 10*(HEAT_RUN_ENV_HLIMIT);
            point2 = val_heat_max_TA;
            point3 = val_heat_min_TA;
            point4 = 10*(HEAT_RUN_ENV_LLIMIT);
            mode_idx = 1;
            break;
            
        case MODE_HOTW:		/* ����ˮ */
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
 �� ��: log_sys_TA_limit_comp_is_off()
 �� ��: �жϻ����¶��Ƿ��ڹر���
 �� ��: mode--; 
		type--; 
 �� ��: TRUE/FALSE--��/��
 �� ��: ��
*******************************************************************************/
BOL log_sys_TA_limit_comp_is_off(U08 mode, U08 type)
{
    return (log_sys_env_limit_comp_area(mode, type) == ELC_OFF);
}

/*******************************************************************************
 �� ��: log_sys_TA_limit_comp_is_run()
 �� ��: �жϻ����¶��Ƿ���������
 �� ��: mode--; 
		type--; 
 �� ��: TRUE/FALSE--��/��
 �� ��: ��
*******************************************************************************/
BOL log_sys_TA_limit_comp_is_run(U08 mode, U08 type)
{
    return (log_sys_env_limit_comp_area(mode, type) == ELC_RUN);
}

/*******************************************************************************
 �� ��: log_sys_HDIsl_limit_comp_is_off()
 �� ��: ̫��������ˮʱ�����̫�����ź�����ѹ��������
 �� ��: ��
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOL log_sys_HDIsl_limit_comp_is_off(void)
{
    static BOL fg_solar_limit = FALSE;

    if (!val_hotw_used                         /* ������ˮ */
    || !fg_hotw_run                            /* ��ˮδ���� */   
    || !MODE_IS_HOTW(SUB_SYS_AC_HOTW)          /* ����ˮģʽ */
    || !val_solar_used                         /* ����̫���� */
    || (val_solar_used&&val_TSL_sensor_used)   /* ʹ��̫���ܣ���ʹ��̫�����¶�̽ͷ */
    )
    {
        fg_solar_limit = FALSE;
        return fg_solar_limit;
    }

    /* δ��⵽̫�����źţ��ҳ���12min */
    if (cnt_HDIsl_off > 12*60)
    {
        fg_solar_limit = FALSE;
    }

    /* ��⵽̫�����źţ��ҳ���5s */
    if (cnt_HDIsl_on > 5)
    {
        fg_solar_limit = TRUE;
    }

    return fg_solar_limit;
}

/*******************************************************************************
 �� ��: log_sys_TSL_limit_comp_is_off()
 �� ��: ̫��������ˮʱ������̫�����¶�����ѹ��������
 �� ��: ��
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOL log_sys_TSL_limit_comp_is_off(void)
{
    static BOL fg_solar_limit = FALSE;

    if (!val_hotw_used                          /* ������ˮ */
    || !fg_hotw_run			                    /* δ������ˮ */
    || !MODE_IS_HOTW(SUB_SYS_AC_HOTW)           /* ����ˮģʽ */
    || !val_solar_used                          /* ����̫���� */
    || (val_solar_used&&!val_TSL_sensor_used)   /* ʹ��̫���ܣ��ҽ���̫�����¶�̽ͷ */
    )
    {
        fg_solar_limit = FALSE;
        return fg_solar_limit;
    }

    /* ̫�����¶�TSL �ܡ�̫���ܿ���ˮ�¡�-��̫���ܿ����ز���ҳ���12min */
    if (cnt_TSL_off > 12*60)
    {
        fg_solar_limit = FALSE;
    }

    /* ̫�����¶�TSL ����̫���ܿ���ˮ�¡����ҳ���5S */
    if (cnt_TSL_on > 5)
    {
        fg_solar_limit = TRUE;
    }

    return fg_solar_limit;
}


/*******************************************************************************
 �� ��: log_sys_floor_preheat_limit_comp_is_off()
 �� ��: ��ůԤ��ʱ����ѹ��������
 �� ��: ��
 �� ��: 
 �� ��: ��
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
    ������get_comp_run_time
    ���ܣ�����ѹ���ۼ�����ʱ��
    ������ģ�����;ѹ�����
    ����ֵ������ʱ�䳤: 1; ����ʱ���: 2
******************************************************/
U08 log_sys_enegry_prio(U08 x1, U08 y1, U08 x2, U08 y2)
{
    return  (cell[x1].cell_runtime[y1] >= cell[x2].cell_runtime[y2]) ? 1 : 2;
}

/*******************************************************************************
 �� ��: log_sys_enegry_sub()
 �� ��: ѡ��ɹرյ�ѹ��
 �� ��: type--type--ģ������(��/��); 
 �� ��: 
 �� ��: ��
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
            if (log_sys_comp_use_status(x, y, COMP_CAN_CLOSE))       /* ѹ�����ɹ� */
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
 �� ��: log_sys_enegry_add()
 �� ��: ѡ��ɿ�����ѹ��
 �� ��: mode--ģʽ; 
		type--ģ������(��/��); 
 �� ��: 
 �� ��: ��
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
    
    for (x = PCB_BGN(type); x < PCB_END(type); x++)              /* �ټ���������ģʽͣ����ѹ���� */
    {
        if (!log_sys_cell_can_run(x)) /* ������(���á�ͨѶ����) */
        {
            continue;        
        }   
            
        for (y = 0; y < m_val_comp_num(x); y++)
        {
            if (get_comp_err(y, x)) 
            {
                continue;
            }   

            if (log_sys_comp_use_status(x, y, COMP_CAN_OPEN))       /* ѹ�����ɿ� */
            if (log_sys_comp_mode(x, y, mode, type))                /* ѹ������ģʽ���� */
            if (fg_warm_complete)                                   /* Ԥ��ʱ�䵽 */
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
        M_CM_SetStatus(k1, k2, COMP_RUN_NEED);     /* ͬ����־ */
        m_run_mode_need[k1][k2] = mode;
        return 1;
    }
    
    return 0;
}

/*******************************************************************************
 �� ��: log_sys_energy_range()
 �� ��: �жϿյ�/��ˮ�¶�����
 �� ��: mode--ģʽ; 
        pv--�����¶�; 
        sv--Ŀ���¶�; 
        e1--����ƫ��; 
        e2--ж��ƫ��; 
 �� ��: 
 �� ��: ��
*******************************************************************************/
U08 log_sys_energy_range(U08 mode, I16 pv, I16 sv, I16 e1, I16 e2)
{
    U08 result = _QUICK;
    
    switch (mode)
    {
        case MODE_COOL: /* �䷽�� */
            if (pv >= (sv + e1))        result = _ADD;  /* ������ */
            else if (pv < (sv - e2))    result = _SUBB; /* ж���� */
            else                        result = _KEEP; /* ������ */
            break;

        case MODE_HOTW:
        case MODE_HEAT: /* �ȷ��� */
            if (pv <= (sv - e1))        result = _ADD;  /* ������ */
            else if (pv > (sv + e2))    result = _SUBB; /* ж���� */
            else                        result = _KEEP; /* ������ */
            break;
            
        default :
            break;
    }
    
    return result;  /* ��ͣ�� */
}

/*******************************************************************************
 �� ��: CountCompRequire()
 �� ��: ����ģ������(��/��)�������״ο���ѹ����
 �� ��: mode--ģʽ; 
		type--ģ������(��/��); 
 �� ��: 
 �� ��: ��
*******************************************************************************/
U08 CountCompRequire(U08 mode, U08 type)
{
    U08 comp_required = 0;
    U08 comp_max = log_sys_have_comp_num(type);

    switch (mode)
    {
        case MODE_COOL:		/* ���� */
            if (ctrl_temp_energy_ac > sv_temp_energy_cool)
            {
                comp_required =(U08)(((ctrl_temp_energy_ac-(I16)sv_temp_energy_cool)*comp_max)/(I16)val_cm_temp_diff);
            }
            break;

        case MODE_HEAT:		/* ���� */
            if (ctrl_temp_energy_ac < sv_temp_energy_heat)
            {
                comp_required =(U08)((((I16)sv_temp_energy_heat-ctrl_temp_energy_ac)*comp_max)/(I16)val_cm_temp_diff);
            }
            break;

        case MODE_HOTW:		/* ��ˮ */
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
 �� ��: log_sys_init_test_run_air_empty()
 �� ��: ��ʼ�������ſ������б���
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_init_test_run_air_empty(void)
{
    CLRBIT_N(fg_test_run, TEST_RUN_AIR_EMPTY);
    ARRCLR(fg_test_air_empty_on);           /* ȫ�� */
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
 �� ��: log_sys_init_test_run_pump()
 �� ��: ��ʼ��ˮ�������б���
 �� ��: ��
 �� ��: ��
 �� ��: ��
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
 �� ��: log_sys_init_test_run_cool_mode()
 �� ��: ��ʼ�����������б���
 �� ��: ��
 �� ��: ��
 �� ��: ��
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
 �� ��: log_sys_init_test_run_heat_mode()
 �� ��: ��ʼ�����������б���
 �� ��: ��
 �� ��: ��
 �� ��: ��
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
 �� ��: log_sys_init_test_run_hotw_mode()
 �� ��: ��ʼ����ˮ�����б���
 �� ��: ��
 �� ��: ��
 �� ��: ��
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
 �� ��: log_sys_stop_test_run_prev_need()
 �� ��: �ж��Ƿ��йر���һ�������й������󣬲��ر���Ӧ�������й���
 �� ��: test_run_next--; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
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
 �� ��: log_sys_test_run_air_empty()
 �� ��: �����ſ������й���
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
#define TEST_AIR_EMPTY_TS_BEGIN     (600)
#define TEST_AIR_EMPTY_TS_STOP      (660)
void log_sys_test_run_air_empty(void)
{
    static BOL fg_wait = FALSE;
    
    /* ������ */
    if (!fg_wait
    && GETBIT_N(cmd_mux, TEST_RUN_AIR_EMPTY_ON)            /* �յ������ſ����������� */
    && !GETBIT_N(fg_test_run, TEST_RUN_AIR_EMPTY)           /* δ��������ſ������� */
    && log_sys_stop_test_run_prev_need(TEST_RUN_AIR_EMPTY)  /* ��Ҫ����һ�������� */
    )
    {
        fg_wait = TRUE;
        return;
    }

    /* ���ɽ��������й��� */
    if (fg_can_enter_test_run)
    {
        fg_wait = FALSE;
    }

    /* ����: �����ſ������� */
    if (!fg_wait && GETCMD_N(TEST_RUN_AIR_EMPTY_ON))	
    {
        /* �����й����л� */
        ARRCLR(fg_test_run);
        SETBIT_N(fg_test_run, TEST_RUN_AIR_EMPTY);
    }

    /* �ر�: �����ſ������� */
    if (GETCMD_N(TEST_RUN_AIR_EMPTY_OFF))	
    {
        CLRBIT_N(fg_test_run, TEST_RUN_AIR_EMPTY);

        /* ���ػ����� */
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

    /* ����ʱ���� */
    if (GETBIT_N(fg_test_run, TEST_RUN_AIR_EMPTY))
    {
        switch (test_run_air_empty_stage)
        {
            /* ���� */
            case TRAE_IDLE:
                test_run_air_empty_stage = TRAE_IDU;
                cnt_test_run_air_empty = 0;
                break;

            /* �ڻ� */
            case TRAE_IDU:
                /* ��鿪��״̬ */
                if (!fg_zone_A_run)
                {
                    SETCMD_N(ZONE_A_ON);
                }

                /* �������� */
                if (cnt_test_run_air_empty < TEST_AIR_EMPTY_TS_BEGIN)
                {
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V1_ON);    /* S3V1 ON��� */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V2_ON);    /* S3V2 OFF��� */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V3_ON);    /* S3V3 ON��� */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPF_ON);   /* PUMPf�� */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPO_ON);   /* PUMPo�� */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPM_ON);   /* PUMPm�� */
                }
                else if (cnt_test_run_air_empty < TEST_AIR_EMPTY_TS_STOP)
                {
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V1_ON);    /* S3V1 ON��� */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V2_ON);    /* S3V2 OFF��� */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V3_ON);    /* S3V3 ON��� */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPF_ON);   /* PUMPf�� */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPO_ON);   /* PUMPo�� */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPM_ON);   /* PUMPm�� */
                }
                else
                {
                    test_run_air_empty_stage = TRAE_HW;
                    cnt_test_run_air_empty = 0;
                }
                break;
                
            /* ����ˮ */
            case TRAE_HW:
                /* ����ˮʱ���л�����һ�׶� */
                if (!val_hotw_used)
                {
                    test_run_air_empty_stage = TRAE_A_HT;
                    cnt_test_run_air_empty = 0;
                    break;
                }

                /* ��鿪��״̬ */
                if (!fg_hotw_run)
                {
                    SETCMD_N(ZONE_A_OFF);
                    SETCMD_N(HOTW_ON);
                }

                /* �������� */
                if (cnt_test_run_air_empty < TEST_AIR_EMPTY_TS_BEGIN)
                {
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V1_ON);    /* S3V1 OFF��� */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V2_ON);    /* S3V2 OFF��� */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V3_ON);    /* S3V3 ON��� */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPF_ON);   /* PUMPf�� */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPO_ON);   /* PUMPo�� */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPM_ON);   /* PUMPm�� */
                }
                else if (cnt_test_run_air_empty < TEST_AIR_EMPTY_TS_STOP)
                {
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V1_ON);    /* S3V1 ON��� */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V2_ON);    /* S3V2 OFF��� */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V3_ON);    /* S3V3 ON��� */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPF_ON);   /* PUMPf�� */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPO_ON);   /* PUMPo�� */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPM_ON);   /* PUMPm�� */
                }
                else
                {
                    test_run_air_empty_stage = TRAE_A_HT;
                    cnt_test_run_air_empty = 0;
                }
                break;

            /* A������ */
            case TRAE_A_HT:
                /* ������ʱ���л�����һ�׶� */
                if (USE_COOL_ONLY)
                {
                    test_run_air_empty_stage = TRAE_A_CL;
                    cnt_test_run_air_empty = 0;
                    break;
                }

                /* ��鿪��״̬ */
                if (!fg_zone_A_run)
                {
                    SETCMD_N(HOTW_OFF);
                    SETCMD_N(ZONE_A_ON);
                }

                /* �������� */
                if (cnt_test_run_air_empty < TEST_AIR_EMPTY_TS_BEGIN)
                {
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V1_ON);    /* S3V1 ON��� */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V2_ON);    /* S3V2 OFF��� */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V3_ON);    /* S3V3 ON��� */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPF_ON);   /* PUMPf�� */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPO_ON);   /* PUMPo�� */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPM_ON);   /* PUMPm�� */
                }
                else if (cnt_test_run_air_empty < TEST_AIR_EMPTY_TS_STOP)
                {
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V1_ON);    /* S3V1 ON��� */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V2_ON);    /* S3V2 OFF��� */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V3_ON);    /* S3V3 ON��� */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPF_ON);   /* PUMPf�� */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPO_ON);   /* PUMPo�� */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPM_ON);   /* PUMPm�� */
                }
                else
                {
                    test_run_air_empty_stage = TRAE_A_CL;
                    cnt_test_run_air_empty = 0;
                }
                break;

            /* A������ */
            case TRAE_A_CL:
                /* ������ʱ���л�����һ�׶� */
                if (USE_HEAT_ONLY)
                {
                    test_run_air_empty_stage = TRAE_B_HT;
                    cnt_test_run_air_empty = 0;
                    break;
                }
                
                /* ��鿪��״̬ */
                if (!fg_zone_A_run)
                {
                    SETCMD_N(HOTW_OFF);
                    SETCMD_N(ZONE_A_ON);
                }

                /* �������� */
                if (cnt_test_run_air_empty < TEST_AIR_EMPTY_TS_BEGIN)
                {
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V1_ON);    /* S3V1 ON��� */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V2_ON);    /* S3V2 ON��� */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V3_ON);    /* S3V3 ON��� */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPF_ON);   /* PUMPf�� */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPO_ON);   /* PUMPo�� */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPM_ON);   /* PUMPm�� */
                }
                else if (cnt_test_run_air_empty < TEST_AIR_EMPTY_TS_STOP)
                {
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V1_ON);    /* S3V1 ON��� */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V2_ON);    /* S3V2 OFF��� */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V3_ON);    /* S3V3 OFF��� */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPF_ON);   /* PUMPf�� */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPO_ON);   /* PUMPo�� */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPM_ON);   /* PUMPm�� */
                }
                else
                {
                    test_run_air_empty_stage = TRAE_B_HT;
                    cnt_test_run_air_empty = 0;
                }
                break;

            /* B������ */
            case TRAE_B_HT:
                /* ��˫�����ƻ������ʱ���л�����һ�׶� */
                if (!USE_DUAL_ZONE_CTRL
                || USE_COOL_ONLY
                )
                {
                    test_run_air_empty_stage = TRAE_IDLE;
                    SETCMD_N(TEST_RUN_AIR_EMPTY_OFF);
                    break;
                }

                /* ��鿪��״̬ */
                if (!fg_zone_B_run)
                {
                    cnt_test_run_air_empty = 0;
                    SETCMD_N(ZONE_A_OFF);
                    SETCMD_N(ZONE_B_ON);
                }

                /* �������� */
                if (cnt_test_run_air_empty < TEST_AIR_EMPTY_TS_BEGIN)
                {
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V1_ON);    /* S3V1 ON��� */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V2_ON);    /* S3V2 OFF��� */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V3_ON);    /* S3V3 OFF��� */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPF_ON);   /* PUMPf�� */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPO_ON);   /* PUMPo�� */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPM_ON);   /* PUMPm�� */
                }
                else if (cnt_test_run_air_empty < TEST_AIR_EMPTY_TS_STOP)
                {
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V1_ON);    /* S3V1 ON��� */
                    SETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V2_ON);    /* S3V2 OFF��� */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V3_ON);    /* S3V3 ON��� */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPF_ON);   /* PUMPf�� */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPO_ON);   /* PUMPo�� */
                    CLRBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPM_ON);   /* PUMPm�� */
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
        ARRCLR(fg_test_air_empty_on);           /* ȫ�� */
        test_run_air_empty_stage = TRAE_IDLE;
    }
}

/*******************************************************************************
 �� ��: log_sys_test_run_pump()
 �� ��: ˮ�������й���
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
#define TEST_PUMP_TS_BEGIN      (120)
#define TEST_PUMP_TS_STABLE     (300)
#define TEST_PUMP_TS_STOP       (360)
void log_sys_test_run_pump(void)
{
    U08 i;
    static BOL fg_wait = FALSE;
    
    /* ������ */
    if (!fg_wait
    && GETBIT_N(cmd_mux, TEST_RUN_PUMP_ON)                 /* �յ�ˮ������������ */
    && !GETBIT_N(fg_test_run, TEST_RUN_PUMP)                /* δ����ˮ�������� */
    && log_sys_stop_test_run_prev_need(TEST_RUN_PUMP)       /* ��Ҫ����һ�������� */
    )
    {
        fg_wait = TRUE;
        return;
    }

    /* ���ɽ��������й��� */
    if (fg_can_enter_test_run)
    {
        fg_wait = FALSE;
    }
    
    /* 1. ����� */
    if (!fg_wait && GETCMD_N(TEST_RUN_PUMP_ON))         /* ����: ˮ�������� */
    {
        /* �����й����л� */
        ARRCLR(fg_test_run);
        SETBIT_N(fg_test_run, TEST_RUN_PUMP);
    }
    if (GETCMD_N(TEST_RUN_PUMP_OFF))		/* �ر�: ˮ�������� */
    {
        CLRBIT_N(fg_test_run, TEST_RUN_PUMP);

        /* ���ػ����� */
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

    /* 2. ����ʱ���� */
    if (GETBIT_N(fg_test_run, TEST_RUN_PUMP))
    {
        switch (test_run_pump_stage)
        {
            /* ���� */
            case TRP_IDLE:
                test_run_pump_stage = TRP_HW;
                cnt_test_run_pump = 0;
                break;
                
            /* ����ˮ */
            case TRP_HW:
                /* ����ˮʱ���л�����һ�׶� */
                if (!val_hotw_used)
                {
                    test_run_pump_stage = TRP_A_HT;
                    cnt_test_run_pump = 0;
                    break;
                }

                /* ��鿪��״̬ */
                if (!fg_hotw_run)
                {
                    SETCMD_N(HOTW_ON);
                }

                /* �������� */
                if (cnt_test_run_pump < TEST_PUMP_TS_BEGIN)
                {
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_S3V1_ON);    /* S3V1 OFF��� */
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_S3V2_ON);    /* S3V2 OFF��� */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_S3V3_ON);    /* S3V3 ON��� */
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_PUMPF_ON);   /* PUMPf�� */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_PUMPO_ON);   /* PUMPo�� */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_PUMPM_ON);   /* PUMPm�� */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_CHECK_LDIW); /* �����LDIw */
                }
                else if (cnt_test_run_pump < TEST_PUMP_TS_STABLE)
                {
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_CHECK_LDIW); /* ���LDIw */
                }
                else if (cnt_test_run_pump < TEST_PUMP_TS_STOP)
                {
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_S3V1_ON);    /* S3V1 ON��� */
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_S3V2_ON);    /* S3V2 OFF��� */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_S3V3_ON);    /* S3V3 ON��� */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_PUMPF_ON);   /* PUMPf�� */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_PUMPO_ON);   /* PUMPo�� */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_PUMPM_ON);   /* PUMPm�� */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_CHECK_LDIW); /* �����LDIw */
                }
                else
                {
                    test_run_pump_stage = TRP_A_HT;
                    cnt_test_run_pump = 0;
                }
                
                break;

            /* A������ */
            case TRP_A_HT:
                /* ������ʱ���л�����һ�׶� */
                if (USE_COOL_ONLY)
                {
                    test_run_pump_stage = TRP_A_CL;
                    cnt_test_run_pump = 0;
                    break;
                }

                /* ��鿪��״̬ */
                if (!fg_zone_A_run)
                {
                    SETCMD_N(HOTW_OFF);
                    SETCMD_N(ZONE_A_ON);
                }

                /* �������� */
                if (cnt_test_run_pump < TEST_PUMP_TS_BEGIN)
                {
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_S3V1_ON);    /* S3V1 ON��� */
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_S3V2_ON);    /* S3V2 OFF��� */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_S3V3_ON);    /* S3V3 ON��� */
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_PUMPF_ON);   /* PUMPf�� */
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_PUMPO_ON);   /* PUMPo�� */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_PUMPM_ON);   /* PUMPm�� */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_CHECK_LDIW); /* �����LDIw */
                }
                else if (cnt_test_run_pump < TEST_PUMP_TS_STABLE)
                {
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_CHECK_LDIW); /* ���LDIw */
                }
                else if (cnt_test_run_pump < TEST_PUMP_TS_STOP)
                {
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_S3V1_ON);    /* S3V1 ON��� */
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_S3V2_ON);    /* S3V2 OFF��� */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_S3V3_ON);    /* S3V3 ON��� */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_PUMPF_ON);   /* PUMPf��*/
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_PUMPO_ON);   /* PUMPo�� */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_PUMPM_ON);   /* PUMPm�� */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_CHECK_LDIW); /* �����LDIw */
                }
                else
                {
                    test_run_pump_stage = TRP_A_CL;
                    cnt_test_run_pump = 0;
                }
                break;

            /* A������ */
            case TRP_A_CL:
                /* ������ʱ���л�����һ�׶� */
                if (USE_HEAT_ONLY)
                {
                    test_run_pump_stage = TRP_B_HT;
                    cnt_test_run_pump = 0;
                    break;
                }
                                
                /* ��鿪��״̬ */
                if (!fg_zone_A_run)
                {
                    SETCMD_N(HOTW_OFF);
                    SETCMD_N(ZONE_A_ON);
                }

                /* �������� */
                if (cnt_test_run_pump < TEST_PUMP_TS_BEGIN)
                {
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_S3V1_ON);    /* S3V1 ON��� */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_S3V2_ON);    /* S3V2 ON��� */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_S3V3_ON);    /* S3V3 ON��� */
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_PUMPF_ON);   /* PUMPf�� */
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_PUMPO_ON);   /* PUMPo�� */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_PUMPM_ON);   /* PUMPm�� */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_CHECK_LDIW); /* �����LDIw */
                }
                else if (cnt_test_run_pump < TEST_PUMP_TS_STABLE)
                {
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_CHECK_LDIW); /* ���LDIw */
                }
                else if (cnt_test_run_pump < TEST_PUMP_TS_STOP)
                {
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_S3V1_ON);    /* S3V1 ON��� */
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_S3V2_ON);    /* S3V2 OFF��� */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_S3V3_ON);    /* S3V3 ON��� */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_PUMPF_ON);   /* PUMPf��*/
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_PUMPO_ON);   /* PUMPo�� */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_PUMPM_ON);   /* PUMPm�� */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_CHECK_LDIW); /* �����LDIw */
                }
                else
                {
                    test_run_pump_stage = TRP_B_HT;
                    cnt_test_run_pump = 0;
                }
                break;

            /* B������ */
            case TRP_B_HT:
                /* ��˫�����ƻ������ʱ���л�����һ�׶� */
                if (!USE_DUAL_ZONE_CTRL
                || USE_COOL_ONLY
                )
                {
                    test_run_pump_stage = TRP_IDLE;
                    SETCMD_N(TEST_RUN_PUMP_OFF);
                    break;
                }

                /* ��鿪��״̬ */
                if (!fg_zone_B_run)
                {
                    cnt_test_run_pump = 0;
                    SETCMD_N(ZONE_A_OFF);
                    SETCMD_N(ZONE_B_ON);
                }

                /* �������� */
                if (cnt_test_run_pump < TEST_PUMP_TS_BEGIN)
                {
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_S3V1_ON);    /* S3V1 ON��� */
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_S3V2_ON);    /* S3V2 OFF��� */
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_S3V3_ON);    /* S3V3 OFF��� */
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_PUMPF_ON);   /* PUMPf�� */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_PUMPO_ON);   /* PUMPo�� */
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_PUMPM_ON);   /* PUMPm�� */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_CHECK_LDIW); /* �����LDIw */
                }
                else if (cnt_test_run_pump < TEST_PUMP_TS_STABLE)
                {
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_CHECK_LDIW); /* ���LDIw */
                }
                else if (cnt_test_run_pump < TEST_PUMP_TS_STOP)
                {
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_S3V1_ON);    /* S3V1 ON��� */
                    SETBIT_N(fg_test_pump_on, TEST_PUMP_S3V2_ON);    /* S3V2 OFF��� */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_S3V3_ON);    /* S3V3 ON��� */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_PUMPF_ON);   /* PUMPf��*/
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_PUMPO_ON);   /* PUMPo�� */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_PUMPM_ON);   /* PUMPm�� */
                    CLRBIT_N(fg_test_pump_on, TEST_PUMP_CHECK_LDIW); /* �����LDIw */
                }
                else
                {
                    test_run_pump_stage = TRP_IDLE;
                    SETCMD_N(TEST_RUN_PUMP_OFF);
                }
                break;
        }

        /* �˳�ˮ�������й��� */
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
 �� ��: log_sys_test_run_cool_mode()
 �� ��: ���������й���
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_test_run_cool_mode(void)
{
    static BOL fg_wait = FALSE;
    
    /* ������ */
    if (!fg_wait
    && GETBIT_N(cmd_mux, TEST_RUN_COOL_MODE_ON)				/* �յ��������������� */
    && !GETBIT_N(fg_test_run, TEST_RUN_COOL_MODE)			/* δ�������������� */
    && log_sys_stop_test_run_prev_need(TEST_RUN_COOL_MODE)	/* ��Ҫ����һ�������� */
    )
    {
        fg_wait = TRUE;
        return;
    }

    /* ���ɽ��������й��� */
    if (fg_can_enter_test_run)
    {
        fg_wait = FALSE;
    }
    
    /* 1. ����� */
    if (!fg_wait && GETCMD_N(TEST_RUN_COOL_MODE_ON))	/* ����: ���������� */
    {
        /* ����������������� */
        if (SensorCanUse(AC_NRG_CTRL_TEMP)
        && AC_NRG_CTRL_TEMP > TEST_RUN_COOL_MODE_SV_COOL
        )
        {
            /* �����й����л� */
            ARRCLR(fg_test_run);
            SETBIT_N(fg_test_run, TEST_RUN_COOL_MODE);

            /* ���ػ����� */
            if (!fg_zone_A_run)
            {
                SETCMD_N(ZONE_A_ON);		/* A������ */
            }
            if (val_hotw_used
            && fg_hotw_run
            )
            {
                SETCMD_N(HOTW_OFF);         /* ��ˮ�ػ� */
            }
        }
    }
    if (GETCMD_N(TEST_RUN_COOL_MODE_OFF))	/* �ر�: ���������� */
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

    /* 2. ����ʱ���� */
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
 �� ��: log_sys_test_run_heat_mode()
 �� ��: ���������й���
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_test_run_heat_mode(void)
{
    static BOL fg_IBH_first_on = TRUE;
    static BOL fg_wait = FALSE;
    
    /* ������ */
    if (!fg_wait
    && GETBIT_N(cmd_mux, TEST_RUN_HEAT_MODE_ON)				/* �յ��������������� */
    && !GETBIT_N(fg_test_run, TEST_RUN_HEAT_MODE)			/* δ�������������� */
    && log_sys_stop_test_run_prev_need(TEST_RUN_HEAT_MODE)	/* ��Ҫ����һ�������� */
    )
    {
        fg_wait = TRUE;
        return;
    }

    /* ���ɽ��������й��� */
    if (fg_can_enter_test_run)
    {
        fg_wait = FALSE;
    }

    /* 1. ����� */
    if (!fg_wait && GETCMD_N(TEST_RUN_HEAT_MODE_ON))	/* ����: ���������� */
    {
        /* ����������������� */
        if (SensorCanUse(AC_NRG_CTRL_TEMP)
        && AC_NRG_CTRL_TEMP < TEST_RUN_HEAT_MODE_SV_HEAT
        )
        {
            /* �����б�־���� */
            ARRCLR(fg_test_run);
            SETBIT_N(fg_test_run, TEST_RUN_HEAT_MODE);

            /* ���ػ����� */
            if (!fg_zone_A_run)
            {
                SETCMD_N(ZONE_A_ON);		/* A������ */
            }
            if (USE_DUAL_ZONE_CTRL
            && !fg_zone_B_run
            )
            {
                SETCMD_N(ZONE_B_ON);        /* B������ */
            }
            if (val_hotw_used
            && fg_hotw_run
            )
            {
                SETCMD_N(HOTW_OFF);         /* ��ˮ�ػ� */
            }
        }
    }    
    if (GETCMD_N(TEST_RUN_HEAT_MODE_OFF))	/* �ر�: ���������� */	
    {
        if (GETBIT_N(fg_test_run, TEST_RUN_HEAT_MODE))
        {
            /* �����б�־���� */
            CLRBIT_N(fg_test_run, TEST_RUN_HEAT_MODE);

            /* ���ػ����� */
            if (fg_zone_A_run)
            {
                SETCMD_N(ZONE_A_OFF);
            }
            if (fg_zone_B_run)
            {
                SETCMD_N(ZONE_B_OFF);
            }
//            SETCMD_N(ZONE_A_OFF);			/* A���ػ� */
//            SETCMD_N(ZONE_B_OFF);			/* B���ػ� */
        }
    }
    if (GETCMD_N(TEST_RUN_HEAT_IBH_ON))		/* ����: ���������и����������� */
    {
        fg_test_run_heat_IBH_run = TRUE;
    }
    if (GETCMD_N(TEST_RUN_HEAT_IBH_OFF))	/* �ر�: ���������и����������� */
    {
        fg_test_run_heat_IBH_run = FALSE;
    }

    /* 2. ����ʱ���� */
    if (GETBIT_N(fg_test_run, TEST_RUN_HEAT_MODE))
    {
        /* ����IBH */
        if (fg_IBH_first_on
        && heat_runtime[SUB_SYS_AC_HOTW].on > 10*60
        && !fg_test_run_heat_IBH_run
        )
        {
            fg_IBH_first_on = FALSE;
            SETCMD_N(TEST_RUN_HEAT_IBH_ON);
        }

        /* IBH����3min��ر� */
        if (cnt_IBH_on > 3*60)
        {
            SETCMD_N(TEST_RUN_HEAT_IBH_OFF);
        }

        /* �˳����������� */
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
 �� ��: log_sys_test_run_hotw_mode()
 �� ��: ����ˮ�����й���
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_test_run_hotw_mode(void)
{
    static BOL fg_TBH_first_on = TRUE;
    static BOL fg_wait = FALSE;
    
    /* 0. ������ */
    if (!fg_wait
    && GETBIT_N(cmd_mux, TEST_RUN_HOTW_MODE_ON)             /* �յ���ˮ���������� */
    && !GETBIT_N(fg_test_run, TEST_RUN_HOTW_MODE)           /* δ������ˮ������ */
    && log_sys_stop_test_run_prev_need(TEST_RUN_HOTW_MODE)  /* ��Ҫ����һ�������� */
    )
    {
        fg_wait = TRUE;
        return;
    }

    /* ���ɽ��������й��� */
    if (fg_can_enter_test_run)
    {
        fg_wait = FALSE;
    }
      
    /* 1. ����� */
    if (!fg_wait && GETCMD_N(TEST_RUN_HOTW_MODE_ON))	/* ����: ����ˮ������ */
    {
        /* �����������ˮ������ */
        if (val_hotw_used
        && SensorCanUse(ctrl_temp_energy_hotw)
        && ctrl_temp_energy_hotw < TEST_RUN_HOTW_MODE_SV_HOTW
        )
        {
            /* �����б�־���� */
            ARRCLR(fg_test_run);
            SETBIT_N(fg_test_run, TEST_RUN_HOTW_MODE);

            /* ���ػ����� */
            if (val_hotw_used
            && !fg_hotw_run
            )
            {
                SETCMD_N(HOTW_ON);         /* ��ˮ���� */
            }
            if (fg_zone_A_run)
            {
                SETCMD_N(ZONE_A_OFF);		/* A���ػ� */
            }
            if (USE_DUAL_ZONE_CTRL
            && fg_zone_B_run
            )
            {
                SETCMD_N(ZONE_B_OFF);        /* B���ػ� */
            }
        }
    }    
    if (GETCMD_N(TEST_RUN_HOTW_MODE_OFF))	/* �ر�: ����ˮ������ */	
    {
        if (GETBIT_N(fg_test_run, TEST_RUN_HOTW_MODE))
        {
            /* �����б�־���� */
            CLRBIT_N(fg_test_run, TEST_RUN_HOTW_MODE);

            /* ���ػ����� */
            if (fg_hotw_run)
            {
                SETCMD_N(HOTW_OFF);
            }
        }
    }
    if (GETCMD_N(TEST_RUN_HOTW_TBH_ON))		/* ����: ����ˮ������ˮ��������� */
    {
        fg_test_run_hotw_TBH_run = TRUE;
    }
    if (GETCMD_N(TEST_RUN_HOTW_TBH_OFF))	/* �ر�: ����ˮ������ˮ��������� */
    {
        fg_test_run_hotw_TBH_run = FALSE;
    }

    /* 2. ����ʱ���� */
    if (GETBIT_N(fg_test_run, TEST_RUN_HOTW_MODE))
    {
        /* ����TBH */
        if (fg_TBH_first_on
        && hotw_runtime.on > 10*60
        && !fg_test_run_hotw_TBH_run
        )
        {
            fg_TBH_first_on = FALSE;
            SETCMD_N(TEST_RUN_HOTW_TBH_ON);
        }

        /* TBH����3min��ر� */
        if (cnt_test_run_hotw_TBH_on > 3*60)
        {
            SETCMD_N(TEST_RUN_HOTW_TBH_OFF);
        }

        /* �˳�����ˮ������ */
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
 �� ��: log_sys_test_run_status()
 �� ��: ������״̬����
 �� ��: ��
 �� ��: ��
 �� ��: ��
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

    /* �������Ƿ���ˮ������ */
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

    /* �������Ƿ�ر�PUMPf */
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
 �� ��: log_sys_test_run()
 �� ��: �����й���
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_test_run(void)
{
    log_sys_test_run_status();
    log_sys_test_run_air_empty();			/* �����ſ������� */
    log_sys_test_run_pump();                /* ˮ�������� */
    log_sys_test_run_cool_mode();			/* ���������� */
    log_sys_test_run_heat_mode();			/* ���������� */
    log_sys_test_run_hotw_mode();			/* ����ˮ������ */
}

#define USE_EGY_INC         /* ʹ������ʽ�ܵ��㷨 */
/*******************************************************************************
 �� ��: log_sys_sub_sys_ac_hotw_energy()
 �� ��: �յ�+��ˮ��ϵͳ��������(�����������󡢿���ѹ������ͣ)
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_sub_sys_ac_hotw_energy(void)
{
    U08 energy_area = _NOTHING;    /* ����������� */
    U08 RequireNum, comp_need_now, sub_sys_type;

    sub_sys_type = (energy_ctrl_type == GROUP_ENERGY_CTRL) ? SUB_SYS_AC_HOTW : SUB_SYS_TYPE_MAX;
    RequireNum = CountCompRequire(ctrl_mode[SUB_SYS_AC_HOTW], sub_sys_type);
    comp_need_now = need_comp_num_in_system(sub_sys_type);
    
    if (!STEP_IS_RUN                        /* δ����ʱ�������������� */
    || fg_run_sta_hotw_antifreeze           /* ����̬��ˮˮ·����ʱ�������������� */
    )
    {
        return;
    }

#if 0        
    if( val_pump_mode==PUMP_MODE_INTER && MODE_IS_HEAT(SUB_SYS_AC_HOTW) && !fg_ac_need_enable[SUB_SYS_AC_HOTW])  
    {
//        bak_ac_need = _NOTHING;
        fg_energy_need[SUB_SYS_AC_HOTW] = FALSE;
            
        return; // ������ˮ�ü�Ъ����δ���������������������ڡ�     
    }
#endif


    /* �¶����� */
    switch (ctrl_mode[SUB_SYS_AC_HOTW])
    {
        case MODE_COOL :    /* ����ģʽ */
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

        case MODE_HEAT :    /* ����ģʽ */
        case MODE_HOTW:     /* ����ˮģʽ */
            if (log_sys_have_comp_run(MODE_COOL, sub_sys_type))
            {
//              ac_need = _QUICK;
                log_sys_select_comp_stop_once(MODE_COOL, sub_sys_type);
            }
            else
            {
                energy_area = (ctrl_mode[SUB_SYS_AC_HOTW] == MODE_HEAT) ? heat_area : hotw_area;
            }

            /* ������ˮ�����Ȳ�ͣ���л� */
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

    /* ��ʱ����ʱǿ���޸�����Ϊ������ */
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

    /* ����յ�+��ˮ��ϵͳ�������� */
    if (energy_area == _ADD)      fg_energy_need[SUB_SYS_AC_HOTW] = TRUE;
    if (energy_area == _SUBB)     fg_energy_need[SUB_SYS_AC_HOTW] = FALSE;
    
#ifndef USE_EGY_INC
    /* ѹ��ȫ���²��ܵ��߼� */
    if (comp_need_now > RequireNum)
    {
        energy_area = _SUBB;
    }
    else if (comp_need_now == RequireNum)
    {
        if (energy_area == _ADD)    energy_area = _KEEP;
    }
#endif  
    
    /* �¶�̽ͷ���� */
    if (SensorCannotUse(ctrl_temp_energy[SUB_SYS_AC_HOTW])) 
    {
        m_ctrl_time = 0;
        energy_area = _KEEP;
    }
    if ((fg_ac_sys_err && fg_hotw_sys_err)                  /* �յ�ϵͳ����ʱ������ж�� */
    || fg_TA_limit_comp_open[SUB_SYS_AC_HOTW]               /* �յ�+��ˮ��ϵͳ�ܵ������¶�����ѹ������ */
    || fg_HDIsl_limit_comp_open                             /* ̫�����ź�����ѹ������ */
    || fg_TSL_limit_comp_open                               /* ̫�����¶�����ѹ������ */
    || fg_floor_preheat_limit_comp_open                     /* ��ůԤ������ѹ������ */
    || GETBIT_N(fg_test_run, TEST_RUN_AIR_EMPTY)            /* �����ſ������� */
    || GETBIT_N(fg_test_run, TEST_RUN_PUMP)                 /* ˮ�������� */
    )
    {
        energy_area = _QUICK;
    }       
//    bak_ac_need = energy_area;
    /* ����ģ������ */
    if(fg_have_stop == TRUE)  /* ��һ�ο��� */
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
        /* ����ѹ��ʱ������Ƶ�� */
        switch (energy_area)
        {
            case _ADD:
                /* ������Ƶʱ�ſ���ѹ�� */
                if (log_sys_have_comp_status(COMP_CAN_ADD, sub_sys_type))
                {
                    energy_area = _KEEP;
                }
                break;
//            case _SUBB:
//                /* ���ɽ�Ƶʱ�Źر�ѹ����ж��ʱ�����жϣ���ֹ���º󲻼�ʱͣ�� */
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
 �� ��: log_sys_sub_sys_ac_energy()
 �� ��: ���յ���ϵͳ��������(�����������󡢿���ѹ������ͣ)
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_sub_sys_ac_energy(void)
{
    U08 energy_area = _NOTHING;    /* ����������� */
    U08 RequireNum = CountCompRequire(ctrl_mode[SUB_SYS_AC_ONLY], SUB_SYS_AC_ONLY);
    U08 comp_need_now = need_comp_num_in_system(SUB_SYS_AC_ONLY);
    
    if (!STEP_IS_RUN                        /* δ����ʱ�������������� */
    || energy_ctrl_type != GROUP_ENERGY_CTRL/* �Ƿ����ܵ�ʱ�������������� */
    || fg_run_sta_ac_antifreeze				/* ����̬�յ�ˮ·������������������ */
    )
    {
        return;       
    }

#if 0        
    if( val_pump_mode==PUMP_MODE_INTER && (MODE_IS_HEAT(SUB_SYS_AC_ONLY)) && !fg_ac_need_enable[SUB_SYS_AC_ONLY])  
    {
//        bak_ac_need = _NOTHING;
        fg_energy_need[SUB_SYS_AC_ONLY] = FALSE;
            
        return; // ������ˮ�ü�Ъ����δ���������������������ڡ�     
    }
#endif

    /* �¶����� */
    switch (ctrl_mode[SUB_SYS_AC_ONLY])
    {
        case MODE_COOL :    /* ����ģʽ */
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

        case MODE_HEAT :    /* ����ģʽ */
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

    /* ���㵥�յ���ϵͳ�������� */
    if (energy_area == _ADD)      fg_energy_need[SUB_SYS_AC_ONLY] = TRUE;
    if (energy_area == _SUBB)     fg_energy_need[SUB_SYS_AC_ONLY] = FALSE;
    
#ifndef USE_EGY_INC
    /* ѹ��ȫ���²��ܵ��߼� */
    if (comp_need_now > RequireNum)
    {
        energy_area = _SUBB;
    }
    else if (comp_need_now == RequireNum)
    {
        if (energy_area == _ADD)    energy_area = _KEEP;
    }
#endif  
    
    /* �¶�̽ͷ���� */
    if (SensorCannotUse(ctrl_temp_energy_ac)) 
    {
        s_ctrl_time = 0;
        energy_area = _KEEP;
    }
    if (fg_ac_sys_err                                       /* �յ�ϵͳ����ʱ������ж�� */
    || fg_TA_limit_comp_open[SUB_SYS_AC_ONLY]               /* ���յ���ϵͳ�ܵ������¶�����ѹ������ */
    || fg_floor_preheat_limit_comp_open                     /* ��ůԤ������ѹ������ */
    || GETBIT_N(fg_test_run, TEST_RUN_AIR_EMPTY)            /* �����ſ������� */
    || GETBIT_N(fg_test_run, TEST_RUN_PUMP)                 /* ˮ�������� */
    || (!fg_zone_A_run && !fg_zone_B_run)                   /* A����B����δ���� */
    )
    {
        energy_area = _QUICK;
    }       
//    bak_ac_need = energy_area;
    /* ����ģ������ */
    if(fg_have_stop == TRUE)  /* ��һ�ο��� */
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
        /* ����ѹ��ʱ������Ƶ�� */
        switch (energy_area)
        {
            case _ADD:
                /* ������Ƶʱ�ſ���ѹ�� */
                if (log_sys_have_comp_status(COMP_CAN_ADD, SUB_SYS_AC_ONLY))
                {
                    energy_area = _KEEP;
                }
                break;
//            case _SUBB:
//                /* ���ɽ�Ƶʱ�Źر�ѹ����ж��ʱ�����жϣ���ֹ���º󲻼�ʱͣ�� */
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
    ����: ����յ�ˮ�ÿ���
    ע�⣺
********************************************************/
/*******************************************************************************
 �� ��: log_sys_PUMP()
 �� ��: ϵͳˮ�ÿ��ƣ�����ԭ�յ����߼���
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
#if 0
#define  PUMP_INTER_ON_TIME     ( 6*QK_60)  // ˮ�ü�Ъ��ʱ��
#define  PUMP_INTER_OFF_TIME    (10*QK_60)  // ˮ�ü�Ъͣʱ��
#define  PUMP_INTER_TIME        (PUMP_INTER_ON_TIME+PUMP_INTER_OFF_TIME)    // ˮ�ü�Ъ����
#define  PUMP_AC_NEED_TIME      (2*60)  // ˮ�ü�Ъ�������������ʱ��
#define  PUMP_INTER_ENV_TEMP    -100    // ˮ�ó�������-10��
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

    /* �����������˵�ǰ��ϵͳ��Ƶˮ�ü��� */
    if (sub_sys_type == SUB_SYS_AC_HOTW)	/* ��ˮ��ϵͳ */
    {
        /* ��ˮ��ϵͳ�б�Ƶˮ������ */
        #define FG_SUB_SYS_AC_HOTW_NEED_PUMPF (fg_hotw_run   /* ��ˮ���� ->ע���ұߵ�б�߲���ʡ */  \
        || (fg_zone_A_run || fg_zone_B_run)                  /* �յ������� */   \
        || FG_IDLE_OR_HOTW_ANTIFREEZE)                       /* ����̬���� �� ����̬��ˮˮ·���� */

        if (!FG_SUB_SYS_AC_HOTW_NEED_PUMPF
        &&  TF_IS_IDLE			/* �̼첻�����ж� */
        )
        {
            /* �����������˳� */
            *ptr_air_pump_dly = 0xff;
            *ptr_fg_pump_on = FALSE;
            *ptr_pump_inter_on_off_time = 0;
//            *ptr_fg_ac_need_enable = FALSE;
            return;
        }
    }
    else					/* ���յ���ϵͳ */
    {
        /* ���յ���ϵͳ�б�Ƶˮ������ */
        #define FG_SUB_SYS_AC_ONLY_NEED_PUMPF ((fg_zone_A_run||fg_zone_B_run)  /* �յ������� ->ע���ұߵ�б�߲���ʡ */ \
        || FG_IDLE_OR_AC_ANTIFREEZE)                                           /* ����̬���� �� ����̬�յ�ˮ·���� */
        
        if (!FG_SUB_SYS_AC_ONLY_NEED_PUMPF
        &&  TF_IS_IDLE			/* �̼첻�����ж� */
        )
        {
            /* �����������˳� */
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
    && !fg_idle_sta_antifreeze				/* ����̬�����������ж� */
    && TF_IS_IDLE			                /* �̼첻�����ж� */
    )
    {
        *ptr_air_pump_dly = 0; 
    }
    
    if (fg_idle_sta_antifreeze_pump_need    /* �������յ��� */
    ||  TF_OPEN_PUMP		                /* �̼쿪�յ��� */
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
    else           /* ˮ�ù��� */
    {
        *ptr_fg_pump_on = FALSE;
    }

#if 0
//    if (!AIR_IS_WATER) return;
                       
//    if (status_run == _IDLE)    /* ����״̬ */
//    {
//        *ptr_air_pump_dly = 0xff;
//        *ptr_fg_pump_on = FALSE;
//        
//        if (!fg_idle_sta_antifreeze /* ������δ�������˳� */    
//        && !debug_mode
//        && TF_IS_IDLE			/* �̼첻�����ж� */
//        )       
//        {
//            //fg_ac_pm_open = FALSE;        /* �˳���������ˮ�ÿ�����־ */
//            return;
//        }
//    }

    if (((status_run == _READY && status_ready > READY_IDLE) 
      || (status_run == _RUN)                                  
      || (status_run == _DELAY && status_delay < DELAY_PUMP && *ptr_air_pump_dly < AC_PUMP_OFF_DLY))
    && !fg_idle_sta_antifreeze				/* ����̬�����������ж� */
    && TF_IS_IDLE			/* �̼첻�����ж� */
    )
    {                   
        switch (ctrl_mode[sub_sys_type])
        {
            case MODE_COOL:
            case MODE_HEAT:
            case MODE_HOTW:
                if (pump_mode == PUMP_MODE_ON)      // ģʽһ ˮ��һֱ��ͣ 
                {
                    *ptr_air_pump_dly = 0;         
                }
                else if (pump_mode == PUMP_MODE_OFF)    // ģʽ�� �¶ȵ�ˮ��ͣ 
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
                else if (pump_mode == PUMP_MODE_INTER)  /* ģʽ�� ��Ъ�Կ� */
                {   
                    if (ctrl_mode[sub_sys_type] == MODE_COOL) // ����ģʽ�³���
                    {
                        *ptr_air_pump_dly = 0;         /* �յ���һֱ���� */
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
                            *ptr_fg_pump_inter_on = TRUE;    /* ��Ъ���յ��ñ�־��λ */

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

                            // ˮ�ÿ���2���Ӻ�ż������
                            if(pump_time[PM_F_ANY].on < PUMP_AC_NEED_TIME)  
                            {
                                *ptr_fg_ac_need_enable = FALSE;
                            }
                        }   
                    }
                }
                break;
                
            default:                    /* δѡ��ģʽ��ˮ�ò��� */
                *ptr_air_pump_dly = 0xff;
                break;
        }
        *ptr_fg_ac_need_enable = TRUE;
    }
    
    if (fg_idle_sta_antifreeze_pump_need    /* �������յ��� */
    ||  TF_OPEN_PUMP		        /* �̼쿪�յ��� */
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
//        set_output(MAX_PCB, M_DO_PUMP);     /* ���յ��� */
        *ptr_fg_pump_on = TRUE;
    }
    else           /* ˮ�ù��� */
    {
//        fg_ac_pm_open = FALSE;
        *ptr_fg_pump_on = FALSE;
    }

//    if (pump_time[PM_F_ANY].on >= TF_VAL(AC_PUMP_ON_DLY, TF_PUMP_ON_DLY) + TF_VAL(val_err_ac_flux_delay, TF_ERR_FLOW_DLY)+2 )   /*30s+15s+2s*/
//    {
//        fg_ac_pm_open = TRUE;        /* ����ʱ����������ı�־ */
//    }

//    if(debug_mode)    
//    {
//        fg_ac_pm_open = FALSE;//����״̬������ʽ���д���,����������
//        *ptr_fg_pump_on = FALSE;
//    }
#endif
}

#if 0
/*******************************************************************************
 �� ��: log_sys_PUMPf()
 �� ��: ϵͳ��Ƶˮ�ÿ���
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_PUMPf(void)
{
    BOOL fg_open = FALSE;

    /* ��ʹ��ϵͳ��Ƶˮ�ã��˴������� */
    if (USE_SYS_PUMPF == FALSE)
    {
        return;
    }
    
    if (get_output(MAX_PCB, M_DO_PUMP))
    {
        fg_open = TRUE;
        set_output(MAX_PCB, M_DO_PUMPf);
    }

    /* ��Ƶˮ��PID���� */
    PUMPF_ctrl(fg_open); 
}
#endif

#if 0
/*******************************************************************************
 �� ��: log_sys_PUMP_FREQ()
 �� ��: ��Ƶˮ�ÿ���
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static U08 cnt_pumpf_act_Period = 0;
static U16 cnt_pumpf_stab = 0;
void log_sys_PUMP_FREQ(void)
{
    #define val_pumpf_pid_calculate_period  8   // ��������0.8s
    #define val_pumpf_pid_Kp                8   // ����ϵ��1.0
    #define val_pumpf_pid_i                 4   // ����ʱ��50
    #define val_pumpf_pid_d                 0   // ΢��ʱ��
    #define val_pumpf_pid_p                 6000 // �����ñ�����
    #define val_pumpf_pid_flexi             10  // ����ϵ��
    
    PID_ID_ENUM PID_id = PID_ID_PUMPF;
    BOOL fg_open = FALSE;
    static I16 pid_sv;
    static I16 pid_pv;
    static I16 Pid_Residue=0;       // PID����ʣ����
    I32 pid_uk,pid_kc;
    I16 speed_incr = 0;
    I16 speed_min;
    I16 speed_max;
    // ����PUMPI���� ��ʹ�ñ�Ƶˮ��
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
                
                pid_set_ctrl(PID_id, PID_CTRL_INCREMENT);       //����ʽ
                pid_set_Ts(PID_id, val_pumpf_pid_calculate_period); //��������
                pid_set_Kp(PID_id, val_pumpf_pid_Kp);//����ϵ��
                pid_set_Ti(PID_id, val_pumpf_pid_i);//����ʱ��
                pid_set_Td(PID_id, val_pumpf_pid_d);//΢��ʱ��
                pid_set_Kc(PID_id, val_pumpf_pid_p);//�����ñ�����
                pid_set_Ai(PID_id, val_pumpf_pid_flexi);//����ϵ��
                pid_set_sv(PID_id, &pid_sv);//Ŀ��ֵ
                pid_set_pv(PID_id, &pid_pv);//��ǰֵ
                
                pid_turn_on(PID_id);
                if (cnt_pumpf_act_Period >= 5)
                {
                    cnt_pumpf_act_Period = 0;
                    pid_uk = pid_read_Uk(PID_id); //PID����
                    pid_kc = pid_get_Kc(PID_id);

                    speed_incr = _Math_divide(&Pid_Residue,pid_uk*pump_speedsv,pid_kc);
                    speed_incr = fun_pv_limit(speed_incr, -200, 200);   /* �޷� */
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

/* PUMPo�������� */
enum                    
{
    PUMPo_ON_SIG_TEST_RUN_AIR_EMPTY      = 0,  /* �����ſ�������ʱ���� */
    PUMPo_ON_SIG_TEST_RUN_PUMP           = 1,  /* ˮ��������ʱ���� */
    PUMPo_ON_SIG_FOLLOW_A_RUN            = 2,  /* ����A������ */
    PUMPo_ON_SIG_IDLE_STA_ANTIFREEZE     = 3,  /* ����̬���� */
    PUMPo_ON_SIG_RUN_STA_AC_ANTIFREEZE   = 4,  /* ����̬�յ�ˮ·���� */
    PUMPo_ON_SIG_ZONE_A_TIMED_ANTIFREEZE = 5,  /* A����ʱ���� */
    PUMPo_ON_SIG_MAX,
};
static U16 fg_PUMPo_on[_DIVUCARRY(PUMPo_ON_SIG_MAX, 16)];
/*******************************************************************************
 �� ��: log_sys_PUMPo()
 �� ��: ��ѭ���ÿ����߼�
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_PUMPo(void)
{
    U08 i;

    /* �����ſ������п�PUMPo */
    if (GETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPO_ON))
    {
        SETBIT_N(fg_PUMPo_on, PUMPo_ON_SIG_TEST_RUN_AIR_EMPTY);
    }
    else
    {
        CLRBIT_N(fg_PUMPo_on, PUMPo_ON_SIG_TEST_RUN_AIR_EMPTY);
    }

    /* ˮ�������п�PUMPo */
    if (GETBIT_N(fg_test_pump_on, TEST_PUMP_PUMPO_ON))
    {
        SETBIT_N(fg_PUMPo_on, PUMPo_ON_SIG_TEST_RUN_PUMP);
    }
    else
    {
        CLRBIT_N(fg_PUMPo_on, PUMPo_ON_SIG_TEST_RUN_PUMP);
    }

    /* ����A������ */
    if (!GETBIT_N(fg_test_run, TEST_RUN_AIR_EMPTY)
    && !GETBIT_N(fg_test_run, TEST_RUN_PUMP)
//    && get_output(MAX_PCB, M_DO_PUMP)	/* 2022-9-1ɾ�����������յ��ÿ����е���ͣ�ĳ������ʲ��жϿյ������ */
//    && MODE_IS_AC                     /* 2022-9-1ɾ��������������A�����أ������ǵ�ǰ����ģʽ */
    && fg_zone_A_run
    )
    {
        SETBIT_N(fg_PUMPo_on, PUMPo_ON_SIG_FOLLOW_A_RUN);
    }
    else
    {
        CLRBIT_N(fg_PUMPo_on, PUMPo_ON_SIG_FOLLOW_A_RUN);
    }

    /* ����̬���� */
    if (fg_idle_sta_antifreeze)
    {
        SETBIT_N(fg_PUMPo_on, PUMPo_ON_SIG_IDLE_STA_ANTIFREEZE);
    }
    else
    {
        CLRBIT_N(fg_PUMPo_on, PUMPo_ON_SIG_IDLE_STA_ANTIFREEZE);
    }

    /* ����̬�յ�ˮ·���� */
    if (fg_run_sta_ac_antifreeze)
    {
        SETBIT_N(fg_PUMPo_on, PUMPo_ON_SIG_RUN_STA_AC_ANTIFREEZE);
    }
    else
    {
        CLRBIT_N(fg_PUMPo_on, PUMPo_ON_SIG_RUN_STA_AC_ANTIFREEZE);
    }

    /* A����ʱ���� */
    if (fg_zone_A_timed_antifreeze_PUMPo_need)
    {
        SETBIT_N(fg_PUMPo_on, PUMPo_ON_SIG_ZONE_A_TIMED_ANTIFREEZE);
    }
    else
    {
        CLRBIT_N(fg_PUMPo_on, PUMPo_ON_SIG_ZONE_A_TIMED_ANTIFREEZE);
    }

    /* ������־������Ƿ���Ҫ���� */
    for (i = 0; i < PUMPo_ON_SIG_MAX; i++)
    {
        if (GETBIT_N(fg_PUMPo_on, i))
        {
            set_output(MAX_PCB, M_DO_PUMPo);
        }
    }
}

/* PUMPm�������� */
enum                    
{
    PUMPm_ON_SIG_UI35_ZONE_B_KEY_ON      = 0,  /* ͨ���߿���B������ʱ */
    PUMPm_ON_SIG_UIRTB_KEY_ON            = 1,  /* ͨ���¿���B����ʱ */
    PUMPm_ON_SIG_IDLE_STA_ANTIFREEZE     = 2,  /* ����̬����ʱ */
    PUMPm_ON_SIG_TEST_RUN_PUMP           = 3,  /* ˮ��������ʱ */
    PUMPm_ON_SIG_TEST_RUN_AIR_EMPTY      = 4,  /* �����ſ�������ʱ */
    PUMPm_ON_SIG_RUN_STA_AC_ANTIFREEZE   = 5,  /* ����̬�յ�ˮ·���� */
    PUMPm_ON_SIG_ZONE_B_TIMED_ANTIFREEZE = 6,  /* B����ʱ���� */
    PUMPm_ON_SIG_MAX,
};
static U16 fg_PUMPm_on[_DIVUCARRY(PUMPm_ON_SIG_MAX, 16)];
/*******************************************************************************
 �� ��: log_sys_PUMPm_need()
 �� ��: 
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_PUMPm_need(void)
{
    if (!FG_IDLE_OR_AC_ANTIFREEZE                                   /* δ������̬���� �� ����̬�յ�ˮ·������ */
    && !fg_zone_B_timed_antifreeze_PUMPm_need                       /* ��B����ʱ���������ˮ������ */
    && (   ((val_pcb_num==1) && MODE_IS_HOTW(SUB_SYS_AC_HOTW))      /* ��������ˮ���� */
        || ((val_pcb_num==1) && MODE_IS_COOL(SUB_SYS_AC_HOTW))      /* ������������ */
        || !fg_zone_B_run                                           /* B��δ���� */
        || ac_mode == MODE_COOL                                     /* �յ�����ģʽΪ����ģʽ */
       )
    )    
    {
        ARRCLR(fg_PUMPm_on);
        return;
    }

    /* ˮ�������п�PUMPm */
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
        return;				/* ��������ʱ��������������� */
    }
    else
    {
        CLRBIT_N(fg_PUMPm_on, PUMPm_ON_SIG_TEST_RUN_PUMP);
    }

    /* �����ſ������п�PUMPm */
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
        return;				/* ��������ʱ��������������� */
    }
    else
    {
        CLRBIT_N(fg_PUMPm_on, PUMPm_ON_SIG_TEST_RUN_AIR_EMPTY);
    }
    
    /* �߿�����B�� */
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

    /* �¿�����B�� */
    if (USE_UIRT_DUAL_ZONE_ON_OFF			                /* �¿���˫������ */
    && m_get_input(IN_RT_HEAT)                              /* �¿��������ź� */
    )
    {
        SETBIT_N(fg_PUMPm_on, PUMPm_ON_SIG_UIRTB_KEY_ON);
    }
    else
    {
        CLRBIT_N(fg_PUMPm_on, PUMPm_ON_SIG_UIRTB_KEY_ON);
    }

    /* ����̬���� */
    if (fg_idle_sta_antifreeze)
    {
        SETBIT_N(fg_PUMPm_on, PUMPm_ON_SIG_IDLE_STA_ANTIFREEZE);
    }
    else
    {
        CLRBIT_N(fg_PUMPm_on, PUMPm_ON_SIG_IDLE_STA_ANTIFREEZE);
    }

    /* ����̬�յ�ˮ·���� */
    if (fg_run_sta_ac_antifreeze)
    {
        SETBIT_N(fg_PUMPm_on, PUMPm_ON_SIG_RUN_STA_AC_ANTIFREEZE);
    }
    else
    {
        CLRBIT_N(fg_PUMPm_on, PUMPm_ON_SIG_RUN_STA_AC_ANTIFREEZE);
    }

    /* B����ʱ���� */
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
 �� ��: log_sys_PUMPm()
 �� ��: ���ˮ�ÿ���
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_PUMPm(void)
{
    U08 i;

    log_sys_PUMPm_need();
    
    /* ������־������Ƿ���Ҫ���� */
    for (i = 0; i < PUMPm_ON_SIG_MAX; i++)
    {
        if (GETBIT_N(fg_PUMPm_on, i))
        {
            set_output(MAX_PCB, M_DO_PUMPm);
        }
    } 
}

/* PUMPret�������� */
enum                    
{
    PUMPret_ON_SIG_TIMER                   = 0,             /* ��ʱ��ʽ����ʱ */
    PUMPret_ON_SIG_DISINFECT               = 1,             /* ɱ����ʽ����ʱ */
    PUMPret_ON_SIG_IDLE_STA_ANTIFREEZE     = 2,             /* ����̬����ʱ��ʼ */
    PUMPret_ON_SIG_RUN_STA_HOTW_ANTIFREEZE = 3,             /* ����̬��ˮˮ·����ʱ��ʼ */
    PUMPret_ON_SIG_TIMED_HOTW_ANTIFREEZE   = 4,             /* ��ˮ��ʱ���� */
    PUMPret_ON_SIG_MAX,
};
static U16 fg_PUMPret_on[_DIVUCARRY(PUMPret_ON_SIG_MAX, 16)];
/*******************************************************************************
 �� ��: log_sys_PUMPret()
 �� ��: �»�ˮ�ÿ���
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_PUMPret(void)
{
    U08 i;
    U16 cnt_PUMPret_on;

    /* PUMPret���� */
    if (!val_use_PUMPret    /* �����»�ˮ�� */
    || !val_hotw_used       /* ������ˮ */
    )   
    {
        ARRCLR(fg_PUMPret_on);
        return;
    }
    
    /* �»�ˮ�ö�ʱ */
    cnt_PUMPret_on = val_PUMPret_run_time;
    if (val_PUMPret_timer	/* �»�ˮ�ö�ʱ */
    && fg_timed_PUMPret_run	/* �»�ˮ�ÿ��� */
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
    
    /* �»�ˮ��ɱ�� */
    /* ����PUMPret��THWt�����¶����� */
    cnt_PUMPret_on = val_PUMPret_run_time + 5*60;
    if (val_PUMPret_disinfect				/* ɱ������ */
    && fg_PUMPret_disinfect_run				/* ����ɱ�� */
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
    
    /* ����̬���� */
    if (fg_idle_sta_antifreeze)
    {
        SETBIT_N(fg_PUMPret_on, PUMPret_ON_SIG_IDLE_STA_ANTIFREEZE);
    }
    else
    {
        CLRBIT_N(fg_PUMPret_on, PUMPret_ON_SIG_IDLE_STA_ANTIFREEZE);
    }

    /* ����̬��ˮˮ·���� */
    if (fg_run_sta_hotw_antifreeze)
    {
        SETBIT_N(fg_PUMPret_on, PUMPret_ON_SIG_RUN_STA_HOTW_ANTIFREEZE);
    }
    else
    {
        CLRBIT_N(fg_PUMPret_on, PUMPret_ON_SIG_RUN_STA_HOTW_ANTIFREEZE);
    }   

    /* ��ˮ��ʱ���� */
    if (fg_hotw_timed_antifreeze)
    {
        SETBIT_N(fg_PUMPret_on, PUMPret_ON_SIG_TIMED_HOTW_ANTIFREEZE);
    }
    else
    {
        CLRBIT_N(fg_PUMPret_on, PUMPret_ON_SIG_TIMED_HOTW_ANTIFREEZE);
    } 

    /* ������־������Ƿ���Ҫ���� */
    for (i = 0; i < PUMPret_ON_SIG_MAX; i++)
    {
        if (GETBIT_N(fg_PUMPret_on, i))
        {
            set_output(MAX_PCB, M_DO_PUMPret);
        }
    } 
}

/* PUMPsl�������� */
enum                    
{
    PUMPsl_ON_SIG_RUN_HOTW_MODE = 0,        /* ����ˮ����ʱ���� */
    PUMPsl_ON_SIG_IDLE_STA_ANTIFREEZE     = 1,              /* ����̬����ʱ���� */
    PUMPsl_ON_SIG_RUN_STA_HOTW_ANTIFREEZE = 2,              /* ����̬��ˮˮ·���� */
    PUMPsl_ON_SIG_TIMED_HOTW_ANTIFREEZE = 3,                /* ��ˮ��ʱ���� */
    PUMPsl_ON_SIG_MAX,
};
static U16 fg_PUMPsl_on[_DIVUCARRY(PUMPsl_ON_SIG_MAX, 16)];
/*******************************************************************************
 �� ��: log_sys_PUMPsl()
 �� ��: ̫����ˮ�ÿ���
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_PUMPsl(void)
{
    U08 i;
    I16 temp_PUMPsl_on, temp_PUMPsl_off;
    /* ���ֹر� */
    if (!val_solar_used                     /* ����̫���� */
    || !val_hotw_used                       /* ������ˮ */
    || SensorCannotUse(pv_sys_THWt)         /* THWt������ */
    )
    {
        ARRCLR(fg_PUMPsl_on);
        return;
    }

    /* ��������ˮ����ʱ���� */
    if (fg_hotw_run)
    {
        /* ���㿪���¶�&�ر��¶� */
        temp_PUMPsl_on = val_PUMPsl_on_TWHt - val_PUMPsl_THWt_dT;
        temp_PUMPsl_off = val_PUMPsl_on_TWHt;
    
        /* �������� */
        if (pv_sys_THWt < temp_PUMPsl_on)
        {
            if ((!val_TSL_sensor_used&&m_get_input(IN_SOLAR))                       /* ʹ��̫�����ź� */
            || (val_TSL_sensor_used&&(pv_sys_TSL>val_PUMPsl_on_TSL))                /* ʹ��̫�����¶� */
            )
            {
                SETBIT_N(fg_PUMPsl_on, PUMPsl_ON_SIG_RUN_HOTW_MODE);
            }
        }
    
        /* �ر����� */
        if ((!val_TSL_sensor_used&&!m_get_input(IN_SOLAR))                          /* ʹ��̫�����ź� */
        || (val_TSL_sensor_used&&(pv_sys_TSL<=val_PUMPsl_on_TSL-val_PUMPsl_TSL_dT)) /* ʹ��̫�����¶� */
        )
        {
            CLRBIT_N(fg_PUMPsl_on, PUMPsl_ON_SIG_RUN_HOTW_MODE);
        }
    
        /* �ر����� */
        if (pv_sys_THWt >= temp_PUMPsl_off)
        {
            CLRBIT_N(fg_PUMPsl_on, PUMPsl_ON_SIG_RUN_HOTW_MODE);
        }
    }
    else
    {
        CLRBIT_N(fg_PUMPsl_on, PUMPsl_ON_SIG_RUN_HOTW_MODE);
    }
    
    /* ����̬���� */
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

    /* ����̬��ˮˮ·���� */
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

    /* ��ˮ��ʱ���� */
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

    /* ������־������Ƿ���Ҫ���� */
    for (i = 0; i < PUMPsl_ON_SIG_MAX; i++)
    {
        if (GETBIT_N(fg_PUMPsl_on, i))
        {
            set_output(MAX_PCB, M_DO_PUMPsl);
        }
    } 
}

/*******************************************************************************
 �� ��: log_sys_cell_PUMPf_broadcast()
 �� ��: ģ���Ƶˮ�ÿ��ƹ㲥
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_cell_PUMPf_broadcast(void)
{
//    U08 pcb_num;
    
    fg_PUMPf_test_run = (GETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_PUMPF_ON)/* �����ſ������� */
                        || GETBIT_N(fg_test_pump_on, TEST_PUMP_PUMPF_ON)        /* ˮ�������� */
                        );
    
//    for (pcb_num = 0; pcb_num < val_pcb_num; pcb_num++)
//    {
//        if (GET_SUB_SYS_ON(pcb_num) && fg_pump_on)
//        {
//            if (!M_CM_GetStatus(pcb_num, MAX_COMP, CELL_PUMP_OPEN))
//            {
//                if (!log_sys_cell_pump_can_open(pcb_num))	/* ģ��ˮ�ò��ɿ��� */
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
//                if (!log_sys_cell_pump_can_close(pcb_num))  /* ģ��ˮ�ò��ɹر� */
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
 �� ��: log_sys_pump()
 �� ��: ˮ�ÿ����߼�����:
                        ����ѭ���á�
                        ����ѭ���á�
                        ��ϱá�
                        ̫����ˮ�á�
                        ��Ƶˮ�á�
                        ����ˮ��ˮ�á�
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_pump(void)
{
    log_sys_sub_sys_PUMPf_ctrl(SUB_SYS_AC_HOTW);         /* �յ�+��ˮ��ϵͳ��Ƶˮ�� */
    log_sys_sub_sys_PUMPf_ctrl(SUB_SYS_AC_ONLY);         /* ���յ���ϵͳ��Ƶˮ�� */
    
    log_sys_PUMPo();        /* ��ѭ���� */
    log_sys_PUMPm();        /* ���ˮ�� */
    log_sys_PUMPret();      /* �»�ˮ�� */
    log_sys_PUMPsl();       /* ̫����ˮ�� */

    log_sys_cell_PUMPf_broadcast();
}

/*******************************************************************************
 �� ��: log_sys_send_fan_cool_elec()
 �� ��: ���ͷ����ȴ���ȣ�����ж�غ͹ػ�������ȵ��ȹر�60S���󣬲ſ��ǹ��ͷ��
 �� ��: ��
 �� ��: TRUE/FALSE--��Ҫ��ȴ����/����Ҫ��ȴ����
 �� ��: ��
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
        if (cnt_elec_stop < 60) // ���ȹر�60S�ڣ������ͷ��
        {
            fg_need_cool_elec = TRUE;
        }
    }
    
    
    return fg_need_cool_elec;
}

/*******************************************************************************
 �� ��: log_sys_send_fan_avoid_cold()
 �� ��: ����磬�ػ������¿�������������������ٿ��ͷ��
 �� ��: ��
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL log_sys_send_fan_avoid_cold(void)
{
    U08 comp_run_num = 0;
    BOOL fg_need_cool_elec = FALSE;
    static BOOL fg_need_avoid_cold = TRUE;

    // ����粻�û��߷����ȣ���������
    if (val_avoid_cold == UNUSED
//    || ctrl_mode != MODE_HEAT
    || !AIR_IS_AIR)     
    {
        fg_need_avoid_cold = FALSE;
        return fg_need_avoid_cold;
    }

    comp_run_num = log_sys_comp_run_num(SUB_SYS_TYPE_MAX);
    fg_need_cool_elec = log_sys_send_fan_cool_elec();

    if (STEP_IS_IDLE || STEP_IS_ALARM)  // ÿ��ͣ�����ٿ�������������
    {
        fg_need_avoid_cold = TRUE;
    }
    else
    {
        if (comp_run_num || fg_need_cool_elec)  // ������ѹ����������к󣬲���������
        {
            fg_need_avoid_cold = FALSE;
        }
    }

    return fg_need_avoid_cold;
}

#if 0
/********************************************************
    ����: �����ͷ������
    ע�⣺
********************************************************/
void log_sys_send_fan(void)
{
    BOOL fg_avoid_cold = FALSE;         // �����
    BOOL fg_send_fan_pause = FALSE;     // �ͷ����ͣ
    U08 comp_run_num = 0;
    BOOL fg_need_cool_elec = FALSE;     // ��Ҫ��ȴ���ȱ�־

    if (!AIR_IS_AIR)    return;
    
    comp_run_num = log_sys_comp_run_num();
    fg_need_cool_elec = log_sys_send_fan_cool_elec();

    fg_avoid_cold = log_sys_send_fan_avoid_cold();
    fg_send_fan_pause = log_sys_send_fan_pause(comp_run_num, fg_need_cool_elec);

    debug_xx[0] = fg_avoid_cold;
    if (fg_avoid_cold)  avoid_cold_fan_cnt = 0;
    
    if (STEP_IS_IDLE || STEP_IS_ALARM)    // ��������ϣ������ͷ��
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
                // �����ʱ�䵽�����ͷ��
                if(avoid_cold_fan_cnt >= val_avoid_cold_fan_time)   
                {
                    air_pump_dly = 0;
                }

                // �����ͷ����ͣʱ ���ͷ��
                if (fg_send_fan_pause)  
                {
                    air_pump_dly = 0xff;
                }
            }            

            // ��˪ͣ�ͷ�� 
            if (log_sys_all_comp_defro())       
            {
                air_pump_dly = 0xff;
            }
        }
    }
    
    if (air_pump_dly <= AC_PUMP_OFF_DLY && !fg_pump_err && !fg_stop_all_mac)
    {
        set_output(MAX_PCB, SW_AC_PM);     /* ���ͷ�� */
        pump_time[PM_F_ANY].off = 0;
    } 
}
#endif

#if 0
/*******************************************************************************
 �� ��: log_sys_AC_is_ready()
 �� ��: �ͷ��/�յ����Ƿ�׼����
 �� ��: ��
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL log_sys_AC_is_ready(void)
{
    if (AIR_IS_WATER)
    {
        if (fg_ac_pm_open)      return TRUE;
    }
    else
    {
        // �ͷ��������ʱ�����߷���磬��Ϊ�ͷ��׼������
        if (pump_time[PM_F_ANY].on >= TF_VAL(AC_PUMP_ON_DLY, TF_PUMP_ON_DLY) + 2) 
        {
            return TRUE;
        }
    }
    return FALSE;
}
#endif

/* TBH�������� */
enum                    
{
    TBH_ON_SIG_RUN_AIR_HW_NON_PRIORITY     = 0,             /* ����/�������У�����ˮ������ʱ���� */
    TBH_ON_SIG_RUN_AIR_HW_PRIORITY         = 1,             /* ����/�������У�����ˮ����ʱ���� */
    TBH_ON_SIG_RUN_HOTW_COMP_NORMAL        = 2,             /* ����ˮ���У�ѹ�������ܵ�ʱ */
    TBH_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL   = 3,             /* ����ˮ���У�ѹ����������������ʱ���� */
    TBH_ON_SIG_RUN_HOTW_COMP_ABNORMAL      = 4,             /* ����ˮ���У�ѹ�����������з�Χ�����ʱ */
    TBH_ON_SIG_RUN_HOTW_COMP_KEEP_ABNORMAL = 5,             /* ����ˮ���У�ѹ�����쳣��������ʱ���� */
    TBH_ON_SIG_DEFROST                     = 6,             /* ��˪ʱ */
    TBH_ON_SIG_FORCE_HOTW                  = 7,             /* ǿ����ˮģʽʱ���� */
    TBH_ON_SIG_FORCE_ON                    = 8,             /* ǿ��ˮ�����ʱ���� */
    TBH_ON_SIG_SMART_GRID_FREE_FORCE_RUN   = 9,             /* ����õ�ǿ������ʱ���� */
    TBH_ON_SIG_SMART_GRID_FREE_RUN         = 10,            /* ����õ�����ʱ���� */
    TBH_ON_SIG_DISINFECT                   = 11,            /* ��ʱɱ��ʱ���� */
    TBH_ON_SIG_TEST_RUN_HOTW_MODE_ON       = 12,            /* ����ˮ������ʱ���� */
    TBH_ON_SIG_REMOTE_SWITCH               = 13,            /* �߿�ˮ�����ʱ���� */
    TBH_ON_SIG_MAX,
};
static U16 fg_TBH_on[_DIVUCARRY(TBH_ON_SIG_MAX, 16)];
/*******************************************************************************
 �� ��: log_sys_TBH_ctrl()
 �� ��: ˮ����ȿ���
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_TBH(void)
{
    #define THWt_MAX          (800)         /* ˮ������¶� */

    U08 i;
    I16 sv_THWt_TBH_on, sv_THWt_TBH_off; 
    static BOL fg_TA_high = FALSE;

    /* ��ˮ����ȿ�����ʱ�� */
    U16 TBH_open_dly = val_TBH_open_dly;

    /* ��TBH������־ */
    fg_TBH_run = FALSE;

    if (SensorCanUse(pv_sys_TA))
    {
        /* TA >=��ˮ����ȿ����� */
        if (pv_sys_TA >= val_TBH_on_TA)
        {
            fg_TA_high = TRUE;
        }
        /* TA <��ˮ����ȿ�����-1�� */
        if (pv_sys_TA < val_TBH_on_TA-10)
        {
            fg_TA_high = FALSE;
        }
    }
    else
    {
        fg_TA_high = FALSE;
    }

    /* ��ʱɱ��ʱ���������ܵ���״̬3��4ʱ������TA */
    if (fg_timed_disinfect_TBH_need
    || FREE_ELEC_RUN                        /* ��ǰ�������ܵ���״̬3 */
    || FREE_ELEC_FORCE_RUN                  /* ��ǰ�������ܵ���״̬4 */
    )
    {
        fg_TA_high = FALSE;
    }

    /* ˮ����ȱ��ֹر� */
    if ((!STEP_IS_RUN)		                /* ����δ���� */
    || (!val_TBH_used)		                /* ��TBH */
    || SensorCannotUse(pv_sys_THWt)			/* THWt������ */
    || fg_TA_high                           /* TA�� */
    || !fg_hotw_run			                /* ����ˮ�ر� */
    || PEAK_ELEC_LIMIT_RUN			        /* �߷��õ��������� */
    || (fg_ability_test_run)				/* �������� */
    || m_get_error(MAX_PCB, M_ERR_TBH_OL)	/* ˮ����ȹ��� */
    )
    {
        ARRCLR(fg_TBH_on);
        return;
    }

#if defined(M_SHENGNENG)
    /* ���������߼�-ˮ����ȿ�����ʱ���ݻ��·ֶ� */
    if (pv_sys_TA < 50)				/* 5�滷�·ֶ� */
    {
        TBH_open_dly = val_TBH_open_dly;
    }
    else
    {
        TBH_open_dly = QK_60*120;	/* 120min */
    }
#endif

    /* �������л���������ʱ������ˮ������ʱ���� */
    if (MODE_IS_AC(SUB_SYS_AC_HOTW)
    && !USE_HOTW_PRIORITY
    && !fg_defrost_run[SUB_SYS_AC_HOTW]
    && !fg_enter_test_run
    )
    {   
        /* ����/�ر��¶ȼ��� */
        sv_THWt_TBH_on = sv_hotw - val_hotw_diff;
        sv_THWt_TBH_off = sv_hotw;
        
        /* TBH�������� */
        if (fg_hotw_need
        && pv_sys_THWt < sv_THWt_TBH_on
        )
        {
            SETBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_AIR_HW_NON_PRIORITY);          
        }

        /* TBH�ر����� */
        if (pv_sys_THWt >= sv_THWt_TBH_off)
        {
            CLRBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_AIR_HW_NON_PRIORITY);
        }
    }
    else
    {
        CLRBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_AIR_HW_NON_PRIORITY);
    }

    /* �������л���������ʱ������ˮ����ʱ���� */
    if (MODE_IS_AC(SUB_SYS_AC_HOTW)
    && USE_HOTW_PRIORITY
    && !fg_defrost_run[SUB_SYS_AC_HOTW]
    && !fg_enter_test_run
    )
    {
        /* �ر��¶ȼ��� */
        sv_THWt_TBH_off = sv_hotw;

        /* TBH�ر��ж� */
        if (pv_sys_THWt >= sv_THWt_TBH_off)
        {
            CLRBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_AIR_HW_PRIORITY);
        }
    }
    else
    {
        CLRBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_AIR_HW_PRIORITY);
    }

    /* ��������ˮ����ʱ���� */
    if (MODE_IS_HOTW(SUB_SYS_AC_HOTW)
    && !FREE_ELEC_FORCE_RUN
    && !FREE_ELEC_RUN
    && !fg_defrost_run[SUB_SYS_AC_HOTW]
    && !fg_enter_test_run
    )
    {    
        /* �¶ȼ��� */
        sv_THWt_TBH_on = sv_hotw - 10;
        sv_THWt_TBH_off = sv_hotw;
    
        /* TBH��������: ѹ������״̬ */
        if (fg_ac_hotw_comp_normal)
        {
            CLRBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_HOTW_COMP_KEEP_ABNORMAL);
        
            /* �������� */
            if (FG_AC_NEED                                  /* �пյ����� */
            && USE_HOTW_PRIORITY                            /* ��ˮģʽ���� */
            && hotw_runtime.run >= TBH_open_dly 			/* ѹ������ʱ�䵽 */
            && pv_sys_THWt < sv_THWt_TBH_on                 /* THWt���㿪������ */
            ) 
            {
                SETBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_HOTW_COMP_NORMAL);
                SETBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_AIR_HW_PRIORITY);
                SETBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL);
            }

            /* �������� */
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

        /* TBH�رմ���: ѹ������״̬ */
        if (GETBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_HOTW_COMP_NORMAL)
        || GETBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL)
        )
        {
            /* �ر����� */
            if (pv_sys_THWt >= sv_THWt_TBH_off)
            {
                CLRBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_HOTW_COMP_NORMAL);
                CLRBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL);
            }
        }

        /* �¶ȼ��� */
        sv_THWt_TBH_on = sv_hotw - val_hotw_diff;
        sv_THWt_TBH_off = MIN((sv_hotw+val_hotw_diff), THWt_MAX);

        /* TBH��������: ѹ���쳣״̬ */
        if (fg_ac_hotw_comp_abnormal)
        {
            CLRBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL);
         
            /* �������� */
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

        /* TBH�رմ���: ѹ���쳣״̬ */
        if (GETBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_HOTW_COMP_ABNORMAL)
        || GETBIT_N(fg_TBH_on, TBH_ON_SIG_RUN_HOTW_COMP_KEEP_ABNORMAL)
        )
        {
            /* �ر����� */
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
    
    /* �����˪ʱ���� */
    if (fg_defrost_run[SUB_SYS_AC_HOTW])
    {
        /* �¶ȼ��� */
        sv_THWt_TBH_on = sv_hotw - 10;
        sv_THWt_TBH_off = sv_hotw;

        /* TBH�������� */
        if (pv_sys_THWt < sv_THWt_TBH_on)
        {
            SETBIT_N(fg_TBH_on, TBH_ON_SIG_DEFROST);
        }
    
        /* TBH�ر����� */
        if (pv_sys_THWt >= sv_THWt_TBH_off)
        {
            CLRBIT_N(fg_TBH_on, TBH_ON_SIG_DEFROST);
        }
    }
    else
    {
        CLRBIT_N(fg_TBH_on, TBH_ON_SIG_DEFROST);
    }

    /* ǿ����ˮģʽ����� */
    if (fg_force_hotw_mode_run)
    {
        /* �¶ȼ��� */
        sv_THWt_TBH_on = sv_hotw - 10;
        sv_THWt_TBH_off = sv_hotw;

        /* TBH�������� */
        if (pv_sys_THWt < sv_THWt_TBH_on)
        {
            SETBIT_N(fg_TBH_on, TBH_ON_SIG_FORCE_HOTW);
        }
    
        /* TBH�ر����� */
        if (pv_sys_THWt >= sv_THWt_TBH_off)
        {
            CLRBIT_N(fg_TBH_on, TBH_ON_SIG_FORCE_HOTW);
        }
    }
    else
    {
        CLRBIT_N(fg_TBH_on, TBH_ON_SIG_FORCE_HOTW);
    }
    
    /* ǿ��ˮ���������� */
    if (fg_force_TBH_run)
    { 
        /* �¶ȼ��� */
        sv_THWt_TBH_on = sv_hotw - 10;
        sv_THWt_TBH_off = sv_hotw;
    
        /* TBH�������� */
        if (pv_sys_THWt < sv_THWt_TBH_on)
        {
            SETBIT_N(fg_TBH_on, TBH_ON_SIG_FORCE_ON);
        }
    
        /* TBH�ر����� */
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

    /* ����õ�ǿ������ʱ���� */
    if (FREE_ELEC_FORCE_RUN)
    {
        /* ����/�ر��¶ȼ��� */
        sv_THWt_TBH_on = 690;
        sv_THWt_TBH_off = 700;

        /* TBH�������� */
        if (pv_sys_THWt < sv_THWt_TBH_on)
        {
            SETBIT_N(fg_TBH_on, TBH_ON_SIG_SMART_GRID_FREE_FORCE_RUN);
        }

        /* TBH�ر����� */
        if (pv_sys_THWt >= sv_THWt_TBH_off)
        {
            CLRBIT_N(fg_TBH_on, TBH_ON_SIG_SMART_GRID_FREE_FORCE_RUN);
        }
    }
    else
    {
        CLRBIT_N(fg_TBH_on, TBH_ON_SIG_SMART_GRID_FREE_FORCE_RUN);
    }

    /* ����õ�����ʱ���� */
    if (FREE_ELEC_RUN)
    {
        /* ����/�ر��¶ȼ��� */
        sv_THWt_TBH_on = sv_hotw - 20;
        sv_THWt_TBH_off = sv_hotw + 30;

        /* TBH�������� */
        if (pv_sys_THWt < sv_THWt_TBH_on)
        {
            SETBIT_N(fg_TBH_on, TBH_ON_SIG_SMART_GRID_FREE_RUN);
        }

        /* TBH�ر����� */
        if (pv_sys_THWt >= sv_THWt_TBH_off)
        {
            CLRBIT_N(fg_TBH_on, TBH_ON_SIG_SMART_GRID_FREE_RUN);
        }
    }
    else
    {
        CLRBIT_N(fg_TBH_on, TBH_ON_SIG_SMART_GRID_FREE_RUN);
    }

    /* ��ʱɱ��ʱ���� */
    if (fg_timed_disinfect_TBH_need
    && SensorCanUse(pv_sys_THWt)
    )
    {
        /* ����/�ر��¶ȼ��� */
        sv_THWt_TBH_on = val_ster_temp - 20;
        sv_THWt_TBH_off= MIN(700, val_ster_temp+10);
    
        /* TBH�������� */
        if (pv_sys_THWt <= sv_THWt_TBH_on
        && cnt_ster_cyc < val_ster_max_cycle
        && cnt_ster_high_temp < val_ster_high_temp_Time
        )
        {
            SETBIT_N(fg_TBH_on, TBH_ON_SIG_DISINFECT);
        }

        /* TBH�ر����� */
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

    /* ����ˮ������ʱ���� */
    if (fg_test_run_hotw_TBH_run)
    {
        SETBIT_N(fg_TBH_on, TBH_ON_SIG_TEST_RUN_HOTW_MODE_ON);
    }
    else
    {
        CLRBIT_N(fg_TBH_on, TBH_ON_SIG_TEST_RUN_HOTW_MODE_ON);
    }

    /* �߿�ˮ�����ʱ���� */           
    if (REMOTE_IS_TBH		/* �߿�ˮ����� */
    && (cnt_LDIrm_dly > 3)	/* ��⵽LDIrm���ҳ���3�� */
    )
    {
        /* �¶ȼ��� */
        sv_THWt_TBH_on = sv_hotw - 10;
        sv_THWt_TBH_off = sv_hotw;

        /* TBH�������� */
        if (pv_sys_THWt < sv_THWt_TBH_on)
        {
            SETBIT_N(fg_TBH_on, TBH_ON_SIG_REMOTE_SWITCH);
        }
    
        /* TBH�ر����� */
        if (pv_sys_THWt >= sv_THWt_TBH_off)
        {
            CLRBIT_N(fg_TBH_on, TBH_ON_SIG_REMOTE_SWITCH);
        }
    }
    else
    {
        CLRBIT_N(fg_TBH_on, TBH_ON_SIG_REMOTE_SWITCH);
    }
    
    /* ������־������Ƿ���Ҫ���� */
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
 �� ��: log_sys_check_force_IBH_off()
 �� ��: ����ģ��ǿ���������Ⱥ��Ƿ�ɹر�
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_check_force_IBH_off(void)
{
    U08 i;

    /* ��������ģ�飬������ǿ���������ȿɹأ���ر����и������� */
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
 �� ��: log_sys_all_hotw_water_flow_err()
 �� ��: ��ˮģ��ȫ��ˮ����������ж�
 �� ��: ��
 �� ��: TRUE/FALSE--ȫ����ˮģ�鱨ˮ���������/��ȫ����ˮģ�鱨ˮ���������
 �� ��: ��
*******************************************************************************/
BOL log_sys_all_hotw_water_flow_err(void)
{
    U08 i;
    
    /* ������ˮģ��ˮ��������� */
    for (i = PCB_BGN(SUB_SYS_AC_HOTW); i < PCB_END(SUB_SYS_AC_HOTW); ++i)
    {
        if (!m_get_error(i, S_ERR_WATER_FLOW))
        {
            /* ����һ���������ϣ��򷵻�FALSE */
            return FALSE;
        }
    }

    return TRUE;
}

/* AHS�������� */
enum                    
{
    AHS_ON_SIG_RUN_HEAT_COMP_NORMAL        = 0,				/* �������У�ѹ�������ܵ�ʱ���� */
    AHS_ON_SIG_RUN_HEAT_COMP_KEEP_NORMAL   = 1,				/* �������У�ѹ����������������ʱ���� */
    AHS_ON_SIG_RUN_HEAT_COMP_ABNORMAL      = 2,				/* �������У�ѹ�����������з�Χ�����ʱ���� */
    AHS_ON_SIG_RUN_HEAT_COMP_KEEP_ABNORMAL = 3,				/* �������У�ѹ�����쳣��������ʱ���� */
    AHS_ON_SIG_RUN_HOTW_COMP_NORMAL        = 4,				/* ����ˮ���У�ѹ�������ܵ�ʱ���� */
    AHS_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL   = 5,				/* ����ˮ���У�ѹ����������������ʱ���� */
    AHS_ON_SIG_RUN_HOTW_COMP_ABNORMAL      = 6,				/* ����ˮ���У�ѹ�����������з�Χ�����ʱ���� */
    AHS_ON_SIG_RUN_HOTW_COMP_KEEP_ABNORMAL = 7,				/* ����ˮ���У�ѹ�����쳣��������ʱ���� */
    AHS_ON_SIG_DEFROST                     = 8,				/* ��˪ʱ���� */
    AHS_ON_SIG_FORCE_HOTW                  = 9,				/* ǿ����ˮģʽʱ���� */
    AHS_ON_SIG_RUN_HEAT_FORCE_ON           = 10,			/* �������У�ǿ���ⲿ��Դʱ���� */
    AHS_ON_SIG_RUN_HOTW_FORCE_ON           = 11,			/* ����ˮ���У�ǿ���ⲿ��Դʱ���� */
    AHS_ON_SIG_DISINFECT                   = 12,			/* ��ʱɱ��ʱ���� */
    AHS_ON_SIG_RUN_HEAT_REMOTE_SWITCH      = 13,			/* �������У��߿��ⲿ��Դ����ʱ���� */
    AHS_ON_SIG_RUN_HOTW_REMOTE_SWITCH      = 14,			/* ����ˮ���У��߿��ⲿ��Դ����ʱ���� */
    AHS_ON_SIG_MAX,
};
static U16 fg_AHS_on[_DIVUCARRY(AHS_ON_SIG_MAX, 16)];
/*******************************************************************************
 �� ��: log_sys_AHS()
 �� ��: �ⲿ��Դ����
 �� ��: ��
 �� ��: ��
 �� ��: ��
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
    pv_ctrl_ac  = USE_AC_TEMP_IN_CTRL ? pv_TACt : pv_TWout3;	/* �յ������¶� */

    fg_AHS_run = FALSE;                     /* ��AHS������־ */
    fg_timed_disinfect_AHS_on = FALSE;      /* ��ɱ��AHS������־ */

    /* �ⲿ��Դ���ֹر� */
    if (!val_TWout3_sensor_used                              /* ����TWout3 */
    || !val_use_AHS                                          /* ����AHS */
    || (MODE_IS_HEAT(SUB_SYS_AC_HOTW) && !HEAT_USE_AHS)      /* ����������AHS������ˮʹ�� */
    || (MODE_IS_HOTW(SUB_SYS_AC_HOTW) && !HOTW_USE_AHS)      /* ����ˮ������AHS������ʹ�� */
    || ((val_pcb_num==1) && MODE_IS_COOL(SUB_SYS_AC_HOTW))   /* ����ģʽ */
    || SensorCannotUse(pv_TWout3)                            /* TWout3������ */
    || (SensorCanUse(pv_TWout3)&&(pv_TWout3>=700))           /* TWout3>=70�� */
    || (fg_TBH_run && MODE_IS_HOTW(SUB_SYS_AC_HOTW))         /* TBH����ʱ */
    || log_sys_all_hotw_water_flow_err()                     /* ��ˮģ��ȫ��ˮ��������� */
    || fg_ability_test_run                                   /* �������� */
    || fg_floor_preheat_run                                  /* ��ůԤ�� */
    || fg_floor_dryup_run	                                 /* ��ů��� */
    )
    {
        ARRCLR(fg_AHS_on);
        return;
    }

    /* ������������ */
    if (MODE_IS_HEAT(SUB_SYS_AC_HOTW)
    && !fg_defrost_run[SUB_SYS_AC_HOTW]
    && fg_heat_need			                /* �������� */
    //&& !fg_timed_disinfect_run
    )
    {
        /* AHS����/�ر��¶ȼ��� */
        sv_TA_AHS_on = val_AHS_open_env - 20;
        sv_TA_AHS_off = val_AHS_open_env;
        sv_TWout3_AHS_on = sv_temp_freq[SUB_SYS_AC_HOTW] - val_AHS_open_diff;
        sv_TWout3_AHS_off = sv_temp_freq[SUB_SYS_AC_HOTW] + val_AHS_close_diff;
 
        /* AHS��������: ѹ������״̬ */
        if (fg_ac_hotw_comp_normal)
        {
            CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HEAT_COMP_KEEP_ABNORMAL);
        
            /* AHS�������� */
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

        /* AHS�رմ���: ѹ������״̬ */
        if (GETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HEAT_COMP_NORMAL)
        || GETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HEAT_COMP_KEEP_NORMAL)
        )
        {
            /* AHS�ر����� */
            if (pv_sys_TA >= sv_TA_AHS_off
            || pv_ctrl_ac >= sv_TWout3_AHS_off
            )
            {
                CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HEAT_COMP_NORMAL);
                CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HEAT_COMP_KEEP_NORMAL);
            }
        }

        /* AHS����/�ر��¶ȼ��� */
        sv_TWout3_AHS_on = sv_temp_freq[SUB_SYS_AC_HOTW] - 30;
        sv_TWout3_AHS_off = MIN(sv_temp_freq[SUB_SYS_AC_HOTW]+val_sub_diff_energy[SUB_SYS_AC_HOTW], 700);

        /* AHS��������: ѹ���쳣״̬ */
        if (fg_ac_hotw_comp_abnormal)
        {
            CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HEAT_COMP_KEEP_NORMAL);

            /* AHS�������� */
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

        /* AHS�رմ���: ѹ���쳣״̬ */
        if (GETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HEAT_COMP_ABNORMAL)
        || GETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HEAT_COMP_KEEP_ABNORMAL)
        )
        {
            /* AHS�ر����� */
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

    /* ��������ˮ���� */
    if (MODE_IS_HOTW(SUB_SYS_AC_HOTW)
    && !fg_defrost_run[SUB_SYS_AC_HOTW]
    && !fg_timed_disinfect_AHS_need
    && fg_hotw_need
    )
    {            
        /* AHS����/�ر��¶ȼ��� */
        sv_THWt_AHS_on = 400;
        sv_THWt_AHS_off = sv_hotw;
        sv_TWout3_AHS_on = 700 - val_AHS_open_diff;
        sv_TWout3_AHS_off = 700;

        /* AHS��������: ѹ������״̬ */
        if (fg_ac_hotw_comp_normal)
        {
            CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_COMP_KEEP_ABNORMAL);
			
            /* AHS�������� */
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

        /* AHS�رմ���: ѹ������״̬ */
        if (GETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_COMP_NORMAL)
        || GETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL)
        )
        {
            /* AHS�ر����� */
            if (pv_sys_THWt > sv_THWt_AHS_off
            || pv_TWout3 >= sv_TWout3_AHS_off
            )
            {
                CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_COMP_NORMAL);
                CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL);
            }
        }

        /* AHS����/�ر��¶ȼ��� */
        sv_THWt_AHS_on = sv_hotw - val_hotw_diff;
        sv_THWt_AHS_off = sv_hotw;
        sv_TWout3_AHS_on = 700 - val_AHS_open_diff;
        sv_TWout3_AHS_off = 700;

        /* AHS��������: ѹ���쳣״̬ */
        if (fg_ac_hotw_comp_abnormal)
        {
            CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL);
			
            /* AHS�������� */
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

        /* AHS�رմ���: ѹ���쳣״̬ */
        if (GETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_COMP_ABNORMAL)
        || GETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_COMP_KEEP_ABNORMAL)
        )
        {
            /* AHS�ر����� */
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

    /* �����˪ʱ���� */
    if (fg_defrost_run[SUB_SYS_AC_HOTW])
    {
        /* AHS����/�ر��¶ȼ��� */
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

    /* ��������ˮ����� */
    if (fg_force_hotw_mode_run)
    {
        /* AHS�����¶ȼ��� */
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

    /* �������У�ǿ���ⲿ��Դ����� */
    if (fg_force_AHS_run
    && RUN_HEAT(SUB_SYS_AC_HOTW)
    )
    { 
        /* AHS����/�ر��¶ȼ��� */
        sv_TWout3_AHS_on = sv_temp_freq[SUB_SYS_AC_HOTW] - 20;
        sv_TWout3_AHS_off = sv_temp_freq[SUB_SYS_AC_HOTW] + val_sub_diff_energy[SUB_SYS_AC_HOTW];

        /* AHS�������� */
        if (pv_ctrl_ac < sv_TWout3_AHS_on)
        {
            SETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HEAT_FORCE_ON);
        }

        /* AHS�ر����� */
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

    /* ����ˮ���У�ǿ���ⲿ��Դ����� */
    if (fg_force_AHS_run
    && RUN_HOTW(SUB_SYS_AC_HOTW)
    )
    {    
        /* AHS����/�ر��¶ȼ��� */
        sv_THWt_AHS_on = sv_hotw - 10;
        sv_THWt_AHS_off = sv_hotw;
        
        /* AHS�������� */
        if (pv_sys_THWt < sv_THWt_AHS_on)
        {
            SETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_FORCE_ON);
        }
        
        /* AHS�ر����� */
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

    /* ��ʱɱ��ʱ���� */
    if (fg_timed_disinfect_run				/* ��ʱɱ������ */
    && SensorCanUse(pv_sys_THWt)			/* THWt���� */
    )
    {
        #define DISINFECT_STAGE_1   1		/* fg_timed_disinfect_AHS_need==FALSE */
        #define DISINFECT_STAGE_2   2		/* fg_timed_disinfect_AHS_need==TRUE */
    
        //static BOL disinfect_stage_1_to_2 = FALSE;		/* ɱ���׶�1���ɵ�2��־����ѯ��λ */
        static BOL disinfect_stage_2_to_1 = FALSE;		/* ɱ���׶�2���ɵ�1��־����ѯ��λ������ʹ�ò��ᴥ�� */
        static U08 disinfect_stage_bak    = DISINFECT_STAGE_1;		/* ɱ���׶θ��� */
        
        /* AHS����/�ر��¶ȼ��� */
        sv_THWt_AHS_on = val_ster_temp - 20;
        sv_THWt_AHS_off= MIN(700, val_ster_temp+10);
        sv_TWout3_AHS_on = 700 - val_AHS_open_diff;
        sv_TWout3_AHS_off = 700;

        /* ɱ���׶ο�Խ�ж� */
        if (!fg_timed_disinfect_AHS_need)	/* �׶�1 */
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

        /* �׶ο�Խ���������׶�1����AHS������Ϊ�Ƕ�ʱɱ����AHS���� */
        if (disinfect_stage_bak == DISINFECT_STAGE_1)/* �׶�1 */
        {
        	/* ɱ���׶�1��ˮ����AHS */
            if (GETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_COMP_NORMAL)
            ||  GETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_COMP_KEEP_NORMAL)
        	||	GETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_COMP_ABNORMAL)
        	||  GETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_COMP_KEEP_ABNORMAL))
        	{
        	    /* ��Ϊ�׶�1����AHSҲ��ɱ����AHS���� */
        	    SETBIT_N(fg_AHS_on, AHS_ON_SIG_DISINFECT);
        	}

        	/* �׶�2��1��Խʱ�ر�AHS */
        	if (disinfect_stage_2_to_1 == TRUE)
        	{
        	    disinfect_stage_2_to_1 = FALSE;
                CLRBIT_N(fg_AHS_on, AHS_ON_SIG_DISINFECT);
        	}

            /* �׶�1ҲҪ��ֹ��������³��£���ֹ��������� */
            /* ��ʱɱ������ѹ��û������С����ʱ��COMP_NEED_ON_TIME���¶ȳ�ɱ���¶� */
        	/* AHS�ر����� */
            if (cnt_ster_cyc >= val_ster_max_cycle
            || cnt_ster_high_temp >= val_ster_high_temp_Time
            || pv_sys_THWt >= sv_THWt_AHS_off
            || pv_TWout3 >= sv_TWout3_AHS_off
            )
            {
                CLRBIT_N(fg_AHS_on, AHS_ON_SIG_DISINFECT);
            }
        }
        else				                        /* �׶�2 */
        {
            /* AHS�������� */
            if (pv_sys_THWt <= sv_THWt_AHS_on
            && pv_TWout3 <= sv_TWout3_AHS_on
            && cnt_ster_cyc < val_ster_max_cycle
            && cnt_ster_high_temp < val_ster_high_temp_Time
            )
            {
                SETBIT_N(fg_AHS_on, AHS_ON_SIG_DISINFECT);
            }

            /* AHS�ر����� */
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

    /* �������У��߿��ⲿ��Դ����ʱ���� */
    if (REMOTE_IS_AHS		/* �߿��ⲿ��Դ */
    && (cnt_LDIrm_dly > 3)	/* ��⵽LDIrm���ҳ���3�� */
    && RUN_HEAT(SUB_SYS_AC_HOTW)
    )
    { 
        /* AHS����/�ر��¶ȼ��� */
        sv_TWout3_AHS_on = sv_temp_freq[SUB_SYS_AC_HOTW] - 20;
        sv_TWout3_AHS_off = sv_temp_freq[SUB_SYS_AC_HOTW] + val_sub_diff_energy[SUB_SYS_AC_HOTW];

        /* AHS�������� */
        if (pv_ctrl_ac < sv_TWout3_AHS_on)
        {
            SETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HEAT_REMOTE_SWITCH);
        }

        /* AHS�ر����� */
        if (pv_ctrl_ac >= sv_TWout3_AHS_off)
        {
            CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HEAT_REMOTE_SWITCH);
        }
    }
    else
    {
        CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HEAT_REMOTE_SWITCH); 
    }

    /* ����ˮ���У��߿��ⲿ��Դ����ʱ���� */
    if (REMOTE_IS_AHS		/* �߿��ⲿ��Դ */
    && (cnt_LDIrm_dly > 3)	/* ��⵽LDIrm���ҳ���3�� */
    && RUN_HOTW(SUB_SYS_AC_HOTW)
    )
    {    
        /* AHS����/�ر��¶ȼ��� */
        sv_THWt_AHS_on = sv_hotw - 10;
        sv_THWt_AHS_off = sv_hotw;
        
        /* AHS�������� */
        if (pv_sys_THWt < sv_THWt_AHS_on)
        {
            SETBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_REMOTE_SWITCH);
        }
        
        /* AHS�ر����� */
        if (pv_sys_THWt >= sv_THWt_AHS_off)
        {
            CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_REMOTE_SWITCH);
        }
    }
    else
    {
        CLRBIT_N(fg_AHS_on, AHS_ON_SIG_RUN_HOTW_REMOTE_SWITCH); 
    }

    /* ��� */
    sub_sys_type = (energy_ctrl_type == GROUP_ENERGY_CTRL) ? SUB_SYS_AC_HOTW : SUB_SYS_TYPE_MAX;
    if (log_sys_have_cell_pump_run(sub_sys_type))
    {
        /* ������־������Ƿ���Ҫ���� */
        for (i = 0; i < AHS_ON_SIG_MAX; i++)
        {
            if (GETBIT_N(fg_AHS_on, i))
            {
                set_output(MAX_PCB, M_DO_AHS);
                fg_AHS_run = TRUE;
            }
        }
    }

    /* ���¶�����־����ģ��ʹ�� */
    fg_timed_disinfect_AHS_on = GETBIT_N(fg_AHS_on, AHS_ON_SIG_DISINFECT);    
}

#if 0
/*******************************************************************************
 �� ��: log_sys_can_enter_timed_disinfect()
 �� ��: ��ʱɱ�������Ƿ�ɽ���
 �� ��: ��
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOL log_sys_can_enter_timed_disinfect(void)
{
    /* ��ʱɱ��������Ч */
    if (!val_hotw_used                      /* ����ˮ���� */
    || !val_timed_ster_fun                  /* �޶�ʱɱ������ */
    || SensorCannotUse(pv_sys_THWt)         /* THWt̽ͷ���� */
    || PEAK_ELEC_LIMIT_RUN                  /* �߷��õ��������� */
    || fg_floor_dryup_run                   /* �����ů��ɹ��� */
    ) 
    {
        return FALSE;
    }

    return TRUE;
}
#endif

/*******************************************************************************
 �� ��: log_sys_timed_disinfect()
 �� ��: ��ʱɱ������
 �� ��: ��
 �� ��: 
 �� ��: ��
*******************************************************************************/
void log_sys_timed_disinfect(void)
{   
    I16 temp_PUMPret_on;
    static BOL fg_on_off_remember = FALSE;
    static BOL fg_need_stop = FALSE;
    static BOL fg_enter_stage_two = FALSE;
    static BOL fg_has_open_PUMRret = FALSE;
    static BOL fg_has_enter_HA_disinfect = FALSE;			/* �����뿪ģʽ�ѽ���ɱ�� */

    temp_PUMPret_on = val_TBH_used ? (val_ster_temp - 20) : 580; 

    /* δ����������ģʽ */
    if (!fg_holiday_away_run)
    {
        fg_has_enter_HA_disinfect = FALSE;
    }

    /* 1. ����� */
    if (GETCMD_N(TIMED_DISINFECT_ON))		/* ����: ��ʱɱ������ */
    {
        /* 1.1 ��ʱɱ�����ܿɽ��� */
        if (val_hotw_used                   /* ʹ����ˮ���� */
        && val_timed_ster_fun               /* ʹ�ö�ʱɱ������ */
        && SensorCanUse(pv_sys_THWt)		/* THWt̽ͷ���� */
        && !PEAK_ELEC_LIMIT_RUN             /* �Ǹ߷��õ��������� */
        && !fg_floor_dryup_run              /* δ�����ů��ɹ��� */
        && !fg_floor_preheat_run            /* δ�����ůԤ�ȹ��� */
        && !fg_has_enter_HA_disinfect		/* δ��������뿪ɱ�� */
        ) 
        {            
            fg_timed_disinfect_run = TRUE;
            /* ��ˮδ���У�����ˮ���� */
            if (!fg_hotw_run)
            {
                SETCMD_N(HOTW_ON);
            }

            /* ��������뿪ģʽ�󣬶�ʱɱ������ֻ����һ�� */
            if (fg_holiday_away_run)
            {
               fg_has_enter_HA_disinfect = TRUE;
            }
        }

        /* 1.2 ���붨ʱɱ������ǰ�����俪�ػ�״̬ */
        if (fg_timed_disinfect_run && !fg_on_off_remember)
        {
            fg_on_off_remember = TRUE;
            
            /* ���붨ʱɱ������ǰ��������״̬����ִ�йػ� */
            if (status_run == _READY || status_run == _RUN)
            {
                fg_need_stop = fg_hotw_run ? FALSE : TRUE;		/* ����ˮ��������������ˮ�ػ� */
            }
            /* ���붨ʱɱ������ǰ���ڴ�����ͣ��״̬��ִ�йػ� */
            else if (status_run == _IDLE || status_run == _DELAY)
            {
                fg_need_stop = TRUE;
            }
        }
    }
    if (GETCMD_N(TIMED_DISINFECT_OFF))		/* �ر�: ��ʱɱ������ */
    {
        fg_timed_disinfect_run = FALSE;
        
        /* ��ʱɱ�����ܽ�����ִ����ˮ�ػ� */
        if (fg_need_stop)
        {
            SETCMD_N(HOTW_OFF);
            fg_need_stop = FALSE;
        }
    }
    
    /* 2. δ���붨ʱɱ�����ܴ��� */
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

    /* 3. �ж�ɱ���׶��Ƿ�Ϊ�׶�2 */
    if (!STEP_IS_RUN                                                    /* δ��������̬ */
    || (STEP_IS_RUN && status_ready >= READY_PUMP)                      /* ����: ������̬�л�������̬˲�� */
    || log_sys_have_non_defrosting_comp_run(MODE_NULL, SUB_SYS_AC_HOTW) /* �п�����ˮ�ķǳ�˪ģ���ѹ������ */
    || (MODE_IS_HOTW(SUB_SYS_AC_HOTW) && fg_energy_need[SUB_SYS_AC_HOTW])    /* �յ�+��ˮ��ϵͳ������ˮģʽ�������������� */
    )
    {
        fg_enter_stage_two = FALSE;
    }
    else
    {
        fg_enter_stage_two = TRUE;
    }
    
    /* 4. �ж�TBH��IBH�Ƿ��п������� */
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

    /* 5. �ж�ɱ����ʱ */
    if ((cnt_ster_cyc >= val_ster_max_cycle)                /* ɱ��ʱ�� >=��ɱ��������ڡ� */
    || (cnt_ster_high_temp >= val_ster_high_temp_Time)      /* ����ɱ��ʱ�� >=��ɱ������ʱ�䡿 */
    )
    {
        SETCMD_N(TIMED_DISINFECT_OFF);
    }

    /* 6. �����»�ˮ��ɱ�� */
    if (SensorCanUse(pv_sys_THWt)			/* THWt���� */
    && pv_sys_THWt >= temp_PUMPret_on		/* THWt >= �»�ˮ��ɱ�������¶� */
    && !fg_has_open_PUMRret	                /* ���붨ʱɱ�����ܺ��״ο���PUMPret */
    )
    {
        fg_has_open_PUMRret = TRUE;
        SETCMD_N(PUMPRET_DISINFECT_ON);
    }
}

/*******************************************************************************
 �� ��: log_sys_can_enter_force_hotw()
 �� ��: ǿ����ˮ�����Ƿ�ɽ���
 �� ��: ��
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOL log_sys_can_enter_force_hotw(void)
{
    if (!val_hotw_used                      /* ����ˮ���� */
    || SensorCannotUse(pv_sys_THWt)         /* THWt̽ͷ���� */
    || fg_force_hotw_mode_run		                /* TBH�ѿ� */
    )
    {
        return FALSE;
    }

    return TRUE;
}

/*******************************************************************************
 �� ��: log_sys_force_hotw()
 �� ��: ǿ����ˮģʽ����
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_force_hotw(void)
{   
    /* δ����ǿ����ˮģʽ���� */
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
 �� ��: log_sys_floor_dryup()
 �� ��: ��ů��ɿ���
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static I16 dryup_sv_temp = SENSOR_NOEXIST;
void log_sys_floor_dryup(void)
{
    I16 tmp;
    U08 day;
    static U08 day_bak = 0;
    static U08 cnt_day = 1;
    day = clock[hmi_t_num].day;     /* �� */

    /* 1. ����� */
    if (GETCMD_N(FLOOR_DRYUP_ON))			/* ����: ��ů��ɹ��� */
    {
        if (val_TFLin_sensor_used			/* ʹ��TFLin */
        && HEAT_USE_FLH		                /* ����ĩ�˴��ڵ�ů */
        && (STEP_IS_IDLE||STEP_IS_DELAY)	/* ������ͣ�� */
        && !fg_floor_preheat_run	        /* δ�����ůԤ�ȹ��� */
        )
        {
            fg_floor_dryup_run = TRUE;
            if (ZONE_A_HEAT_USE_FLH)
            {
                SETCMD_N(ZONE_A_ON);		/* A������ */
            }
            if (ZONE_B_HEAT_USE_FLH)
            {
                SETCMD_N(ZONE_B_ON);		/* B������ */
            }
        }
    }
    if (GETCMD_N(FLOOR_DRYUP_OFF))			/* �ر�: ��ů��ɹ��� */
    {
        fg_floor_dryup_run = FALSE;
        
        SETCMD_N(ZONE_A_OFF);				/* A���ػ� */
        SETCMD_N(ZONE_B_OFF);				/* B���ػ� */
    }

    /* 2. ����ʱ���� */
    if (fg_floor_dryup_run)
    {       
        /* �������¶� */
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
        else				/* ������� */
        {
            SETCMD_N(FLOOR_DRYUP_OFF);
        }
    
        if ((!log_sys_have_comp_run(MODE_NULL, SUB_SYS_TYPE_MAX)&&(log_sys_cell_is_err()))/* ���ڹ��ϵ�������ѹ���������� */
        && !HEAT_USE_IBH                                                /* �޸������ȿ��� */
        && !HEAT_USE_AHS	                                            /* ���ⲿ��Դ���� */
        )
        {
            SETCMD_N(FLOOR_DRYUP_OFF);
        }

        /* �ۼ����� */
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
 �� ��: log_sys_floor_preheat()
 �� ��: ��ůԤ�ȹ���
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_floor_preheat(void)
{
    /* 1. ����� */
    if (GETCMD_N(FLOOR_PREHEAT_ON))			/* ����: ��ůԤ�ȹ��� */
    {
        if (val_TFLin_sensor_used			/* ʹ��TFLin */
        && HEAT_USE_FLH                     /* ����ĩ�˴��ڵ�ů */
        && (STEP_IS_IDLE || STEP_IS_DELAY)	/* ������ͣ�� */
        && !fg_floor_dryup_run				/* δ�����ů��ɹ��� */
        )
        {
            fg_floor_preheat_run = TRUE;
            if (ZONE_A_HEAT_USE_FLH)
            {
                SETCMD_N(ZONE_A_ON);		/* A������ */
            }
            if (ZONE_B_HEAT_USE_FLH)
            {
                SETCMD_N(ZONE_B_ON);		/* B������ */
            }
        }
    }
    if (GETCMD_N(FLOOR_PREHEAT_OFF))		/* �ر�: ��ůԤ�ȹ��� */
    {
        fg_floor_preheat_run = FALSE;

        SETCMD_N(ZONE_A_OFF);				/* A���ػ� */
        SETCMD_N(ZONE_B_OFF);				/* B���ػ� */
    }

    /* 2. ����ʱ���� */
    if (fg_floor_preheat_run)
    {
        if (floor_preheat_time.run > val_floor_preheat_max*60)
        {
            SETCMD_N(FLOOR_PREHEAT_OFF);
        }
    }
}


/*******************************************************************************
 �� ��: log_sys_holiday_away()
 �� ��: �������ģʽ
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_holiday_away(void)
{

}

#if 0
/*******************************************************************************
 �� ��: log_sys_holiday()
 �� ��: �ݼ�ģʽ
 �� ��: ��
 �� ��: ��
 �� ��: ��
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
        if ((fun_time_diff_day(&clock[hmi_t_num], &start_date) <= 0)  // �����ݼ��ڼ�
            && (fun_time_diff_day(&clock[hmi_t_num], &end_date) > 0)
        )
        {
            if (!fg_timed_disinfect_run)   // �ȴ�ɱ��������Ž���
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
//        case HOT_WATER_ONLY:    // ��ˮ������������ˮ
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
 �� ��: log_sys_holiday_on_off()
 �� ��: �ݼ�ģʽ���ػ�
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_holiday_on_off(void)
{
    static BOOL fg_enter_holiday_bak = FALSE;
    static BOOL fg_need_stop = FALSE;
    static BOOL fg_need_juge = TRUE;

    if (fg_need_juge)
    {
        // �����ݼ�ǰ��������̬����ִ�йػ�
        if (status_run == _READY || status_run == _RUN)
        {
            fg_need_stop = FALSE;
        }
        // �����ݼ�ǰ���ڴ�����ִ�йػ�
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
                Logic_machine_start();    /* ִ�п������� */
            }
        }
        else
        {
            fg_need_juge = TRUE;
            if ((status_run == _READY || status_run == _RUN) && fg_need_stop)
            {
                Logic_machine_stop();     /* ִ�йػ����� */
            }
        }
    }
}
#endif 

/*******************************************************************************
 �� ��: log_sys_ability_test()
 �� ��: �������������߼�
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_ability_test(void)
{
    if (para_AT_use_get() == AT_UNUSED)	/* ����ʱ */
    {
        /* ���������Իָ������� */
        fg_ability_test_run = FALSE;

        /* �ն�����֮ǰ���������Ե��������л���������ѡ��ʱ���ܻᴥ���������� */
        GETCMD_N(ABILITY_TEST_ON);
    }
    else
    {
        /* 1. ����� */
        if (GETCMD_N(ABILITY_TEST_ON))			/* ����: ������������ */
        {
    //        /* ������ͣ��״̬�£���������������� */
    //        if (status_run == _IDLE || status_run == _DELAY)
            {
                fg_ability_test_run = TRUE;
                SETCMD_N(ZONE_A_ON);			/* A������ */
            }
        }
        if (GETCMD_N(ABILITY_TEST_OFF))			/* �ر�: ������������ */
        {
            if (fg_ability_test_run)
            {
                fg_ability_test_run = FALSE;
    //            SETCMD_N(ZONE_A_OFF);				/* A���ػ� */
            }
        }

        /* 2. ����ʱ���� */
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
/* S3V1�������� */
enum                    
{
    S3V1_ON_SIG_TEST_RUN_AIR_EMPTY      = 0,  /* �����ſ�������ʱ */
    S3V1_ON_SIG_TEST_RUN_PUMP           = 1,  /* ˮ��������ʱ */
    S3V1_ON_SIG_HW_MODE_NEED            = 2,  /* �л�������ˮ����ʱ */
    S3V1_ON_SIG_TIMED_DISINFECT         = 3,  /* ��ʱɱ��ʱ */
    S3V1_ON_SIG_IDLE_STA_ANTIFREEZE     = 4,  /* ����̬����ʱ */
    S3V1_ON_SIG_RUN_STA_HOTW_ANTIFREEZE = 5,  /* ����̬��ˮˮ·���� */
    S3V1_ON_SIG_HOTW_TIMED_ANTIFREEZE   = 6,  /* ��ˮ��ʱ���� */
    S3V1_ON_SIG_MAX,
};
static U16 fg_S3V1_on[_DIVUCARRY(S3V1_ON_SIG_MAX, 16)];
/*******************************************************************************
 �� ��: log_sys_S3V1()
 �� ��: �����ͨ��SV1���л�ˮ·
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_S3V1(void)
{
    U08 i;

    /* �����ſ������п�S3V1 */
    if (GETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V1_ON)
    )
    {
        SETBIT_N(fg_S3V1_on, S3V1_ON_SIG_TEST_RUN_AIR_EMPTY);
    }
    else
    {
        CLRBIT_N(fg_S3V1_on, S3V1_ON_SIG_TEST_RUN_AIR_EMPTY);
    }

    /* ˮ�������п�S3V1 */
    if (GETBIT_N(fg_test_pump_on, TEST_PUMP_S3V1_ON)
    )
    {
        SETBIT_N(fg_S3V1_on, S3V1_ON_SIG_TEST_RUN_PUMP);
    }
    else
    {
        CLRBIT_N(fg_S3V1_on, S3V1_ON_SIG_TEST_RUN_PUMP);
    }

    /* ����ˮ���п�S3V1 */
    if (STEP_IS_RUN
    && MODE_IS_HOTW(SUB_SYS_AC_HOTW)
    && !fg_air_empty_test_run
    && !fg_pump_test_run
    )
    {
        /* ������ˮ���� */
        SETBIT_N(fg_S3V1_on, S3V1_ON_SIG_HW_MODE_NEED);
    }
    else
    {
        CLRBIT_N(fg_S3V1_on, S3V1_ON_SIG_HW_MODE_NEED);
    }

    /* ��ʱɱ��ʱ��S3V1 */
    if (fg_timed_disinfect_run)
    {
        SETBIT_N(fg_S3V1_on, S3V1_ON_SIG_TIMED_DISINFECT);
    }
    else
    {
        CLRBIT_N(fg_S3V1_on, S3V1_ON_SIG_TIMED_DISINFECT);
    }
    
    /* ����̬���� */
    if (fg_idle_sta_antifreeze)
    {
        /* ����ʱ������������󣬽��ɷ������� */
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

    /* ����̬��ˮˮ·���� */
    if (fg_run_sta_hotw_antifreeze)
    {
        /* ����ʱ������������󣬽��ɷ������� */
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

    /* A����ʱ���� */
    if (fg_zone_A_timed_antifreeze_S3V1_ON_need)
    {
        ARRCLR(fg_S3V1_on);
    }

    /* B����ʱ���� */
    if (fg_zone_B_timed_antifreeze_S3V1_ON_need)
    {
        ARRCLR(fg_S3V1_on);
    }

    /* ��ˮ��ʱ���� */
    if (fg_hotw_timed_antifreeze_S3V1_OFF_need)
    {
        SETBIT_N(fg_S3V1_on, S3V1_ON_SIG_HOTW_TIMED_ANTIFREEZE);
    }
    else
    {
        CLRBIT_N(fg_S3V1_on, S3V1_ON_SIG_HOTW_TIMED_ANTIFREEZE);
    }

    /* ������־������Ƿ���Ҫ���� */
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

/* S3V2�������� */
enum                    
{
    S3V2_ON_SIG_TEST_RUN_AIR_EMPTY = 0,     /* �����ſ�������ʱ���� */
    S3V2_ON_SIG_TEST_RUN_PUMP      = 1,		/* ˮ��������ʱ���� */
    S3V2_ON_SIG_RUN_HT_MODE        = 2,		/* ��������ʱ���� */
    S3V2_ON_SIG_IDLE_STA_ANTIFREEZE   = 3,	/* ����̬��������ʱ���� */
    S3V2_ON_SIG_RUN_STA_AC_ANTIFREEZE = 4,	/* ����̬�յ�ˮ·����ʱ���� */
    S3V2_ON_SIG_TIMED_AC_ANTIFREEZE = 5,	/* A����ʱ���� */
    S3V2_ON_SIG_MAX,
};
static U16 fg_S3V2_on[_DIVUCARRY(S3V2_ON_SIG_MAX, 16)];
/*******************************************************************************
 �� ��: log_sys_S3V2()
 �� ��: �����ͨ��S3V2
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_S3V2(void)
{
    U08 i;

    /* S3V2���ֹر� */
    if (!GETBIT_N(fg_test_run, TEST_RUN_AIR_EMPTY)  /* δ��������ſ������� */
    && !GETBIT_N(fg_test_run, TEST_RUN_PUMP)        /* δ����ˮ�������� */
    && USE_COOL_ONLY                                /* ������ */
    )
    {
        ARRCLR(fg_S3V2_on);
        return;
    }

    /* �����ſ������п�S3V2 */
    if (GETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V2_ON)
    )
    {
        SETBIT_N(fg_S3V2_on, S3V2_ON_SIG_TEST_RUN_AIR_EMPTY);
    }
    else
    {
        CLRBIT_N(fg_S3V2_on, S3V2_ON_SIG_TEST_RUN_AIR_EMPTY);
    }

    /* ˮ�������п�S3V2 */
    if (GETBIT_N(fg_test_pump_on, TEST_PUMP_S3V2_ON)
    )
    {
        SETBIT_N(fg_S3V2_on, S3V2_ON_SIG_TEST_RUN_PUMP);
    }
    else
    {
        CLRBIT_N(fg_S3V2_on, S3V2_ON_SIG_TEST_RUN_PUMP);
    }

    /* ���ȿ�S3V2 */
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

    /* ����̬���� */
    if (fg_idle_sta_antifreeze)
    {
        /* ��ȡһ�����ڼ�ʱ */
        U16 cnt_freeze_cycle = freeze_act_time%ANTI_PRE_DLY;

        /* ����ʱ������������󣬽��ɷ������� */
        ARRCLR(fg_S3V2_on);	

        /* 50%ռ�ձ�������� */
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

    /* ����̬�յ�ˮ·���� */
    if (fg_run_sta_ac_antifreeze)
    {
        /* ��ȡһ�����ڼ�ʱ */
        U16 cnt_freeze_cycle = freeze_act_time%ANTI_PRE_DLY;

        /* ����ʱ������������󣬽��ɷ������� */
        ARRCLR(fg_S3V2_on);	

        /* 50%ռ�ձ�������� */
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

    /* A����ʱ���� */
    if (fg_zone_A_timed_antifreeze)
    {
        /* ��ȡһ�����ڼ�ʱ */
        U16 cnt_freeze_cycle = freeze_act_time%ANTI_PRE_DLY;

        /* ����ʱ������������󣬽��ɷ������� */
        ARRCLR(fg_S3V2_on);	

        /* 50%ռ�ձ�������� */
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

    /* ������־������Ƿ���Ҫ���� */
    for (i = 0; i < S3V2_ON_SIG_MAX; i++)
    {
        if (GETBIT_N(fg_S3V2_on, i))
        {
            set_output(MAX_PCB, M_DO_S3V2);
        }
    } 
}

/* S3V3�������� */
enum                    
{
    S3V3_ON_SIG_UI35_DUAL_WATER_ZONE_B_KEY_ON  = 0,			/* �߿���˫��ˮ�¿���-B������ʱ */
    S3V3_ON_SIG_UI35_DUAL_HYBRID_ZONE_B_KEY_ON = 1,			/* �߿���˫����Ͽ���-B������ʱ */
    S3V3_ON_SIG_UIRT_DUAL_ON_OFF_UIRTB_KEY_ON  = 2,			/* �¿���˫������-�¿���B���ؿ���ʱ */
    S3V3_ON_SIG_TEST_RUN_AIR_EMPTY             = 3,			/* �����ſ�������ʱ���� */
    S3V3_ON_SIG_IDLE_STA_ANTIFREEZE            = 4,			/* ����̬����ʱ���� */
    S3V3_ON_SIG_TEST_RUN_PUMP                  = 5,			/* ˮ��������ʱ���� */
    S3V3_ON_SIG_RUN_STA_AC_ANTIFREEZE          = 6,			/* ����̬�յ�ˮ·����ʱ */
    S3V3_ON_SIG_ZONE_B_TIMED_ANTIFREEZE        = 7,			/* B����ʱ����ʱ���� */
    S3V3_ON_SIG_MAX,
};
static U16 fg_S3V3_on[_DIVUCARRY(S3V3_ON_SIG_MAX, 16)];
/*******************************************************************************
 �� ��: log_sys_S3V3()
 �� ��: �����ͨ��S3V3(B����ů)
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_S3V3(void)
{
    U08 i;
    I16 ctrl_temp_S3V3, temp_S3V3_on, temp_S3V3_off;
    BOOL is_overheat_protect;

    /* ѡS3V3�����¶ȡ�����S3V3����&�ر��¶� */
    if (fg_sv_temp_energy_overwrite_heat)	/* �������� */
    {
        /* ʹ��TFLin�ж� */
        ctrl_temp_S3V3 = pv_sys_TFLin;
        temp_S3V3_on = sv_temp_energy_heat - val_S3V3_on_TFLin_dT;
        temp_S3V3_off = sv_temp_energy_heat;
    }
    else if (USE_UI35_DUAL_ZONE_HYBRID_CTRL)
    {
        /* ʹ��TR�ж� */
        ctrl_temp_S3V3 = pv_sys_TR;
        temp_S3V3_on = sv_room_heat_zone_B - val_S3V3_on_TR_dT;
        temp_S3V3_off = sv_room_heat_zone_B;
    }
    else
    {
        /* ʹ��TFLin�ж� */
        ctrl_temp_S3V3 = pv_sys_TFLin;
        temp_S3V3_on = sv_heat_zone_B - val_S3V3_on_TFLin_dT;
        temp_S3V3_off = sv_heat_zone_B;
    }

    /* ��ů���ȱ��� */
    is_overheat_protect = (pv_sys_TFLin>=val_TFLin_protect) ? TRUE : FALSE;

    /* TFLin����:�޸��߼����� */
    if (!val_TFLin_sensor_used)
    {
        ARRCLR(fg_S3V3_on);
        return;
    }

    /* S3V3���ֹر� */
    if (!FG_IDLE_OR_AC_ANTIFREEZE                   /* �ǣ�����̬���� �� ����̬�յ�ˮ·������ */
    && !fg_zone_B_timed_antifreeze			        /* ��B����ʱ���� */
    && !GETBIT_N(fg_test_run, TEST_RUN_AIR_EMPTY)   /* δ��������ſ������� */
    && !GETBIT_N(fg_test_run, TEST_RUN_PUMP)        /* δ����ˮ�������� */
    && (   !STEP_IS_RUN                             /* ����δ���� */
        || !fg_zone_B_run                           /* B��δ���� */
        || ac_mode == MODE_COOL                     /* �յ�����ģʽΪ����ģʽ */
        || !SensorCanUse(ctrl_temp_S3V3)            /* S3V3�����¶Ȳ����� */
        )
    )
    {
        ARRCLR(fg_S3V3_on);
        return;
    }

    /* ��ů���ȱ��ֹر� */
    if (is_overheat_protect)
    {
        ARRCLR(fg_S3V3_on);
        return;
    }

    /* �߿���˫��ˮ�¿��� */
    if (USE_UI35_DUAL_ZONE_WATER_TEMP_CTLR
    && !fg_air_empty_test_run
    && !fg_pump_test_run
    )
    {
        /* S3V3���� */
        if (ctrl_temp_S3V3 <= temp_S3V3_on
        &&  cnt_S3V3_in_circue_s >= S3V3_in_circue_min_time
        )
        {
            SETBIT_N(fg_S3V3_on, S3V3_ON_SIG_UI35_DUAL_WATER_ZONE_B_KEY_ON);
        }

        /* S3V3�ر� */
        if ((ctrl_temp_S3V3>temp_S3V3_off && cnt_S3V3_ext_circue_s >= val_S3V3_ext_circue_min_time)
        || is_overheat_protect
        )     
        {
            /* ȷ���ɿ�����ִֻ��һ�� */
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
    
    /* �߿���˫����Ͽ��� */
    if (USE_UI35_DUAL_ZONE_HYBRID_CTRL
    && !fg_air_empty_test_run
    && !fg_pump_test_run
    )
    {
        /* S3V3���� */
        if (ctrl_temp_S3V3 <= temp_S3V3_on
        &&  cnt_S3V3_in_circue_s >= S3V3_in_circue_min_time
        )
        {
            SETBIT_N(fg_S3V3_on, S3V3_ON_SIG_UI35_DUAL_HYBRID_ZONE_B_KEY_ON);
        }

        /* S3V3�ر� */
        if ((ctrl_temp_S3V3>temp_S3V3_off && cnt_S3V3_ext_circue_s >= val_S3V3_ext_circue_min_time)
        || is_overheat_protect
        )     
        {
            /* ȷ���ɿ�����ִֻ��һ�� */
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

    /* �¿���˫������ */
    if (USE_UIRT_DUAL_ZONE_ON_OFF
    && !fg_air_empty_test_run
    && !fg_pump_test_run
    )
    {
        /* S3V3���� */
        if (ctrl_temp_S3V3 <= temp_S3V3_on
        &&  cnt_S3V3_in_circue_s >= S3V3_in_circue_min_time
        )
        {
            SETBIT_N(fg_S3V3_on, S3V3_ON_SIG_UIRT_DUAL_ON_OFF_UIRTB_KEY_ON);
        }

        /* S3V3�ر� */
        if ((ctrl_temp_S3V3>temp_S3V3_off && cnt_S3V3_ext_circue_s >= val_S3V3_ext_circue_min_time)
        || is_overheat_protect
        )     
        {
            /* ȷ���ɿ�����ִֻ��һ�� */
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

    /* ����̬���� */
    if (fg_idle_sta_antifreeze)
    {
        /* ��ȡһ�����ڼ�ʱ */
        U16 cnt_freeze_cycle = freeze_act_time%ANTI_PRE_DLY;

        /* ����ʱ������������󣬽��ɷ������� */
        ARRCLR(fg_S3V3_on);	

        /* 50%ռ�ձ�������� */
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

    /* ����̬�յ�ˮ·���� */
    if (fg_run_sta_ac_antifreeze)
    {
        /* ��ȡһ�����ڼ�ʱ */
        U16 cnt_freeze_cycle = freeze_act_time%ANTI_PRE_DLY;

        /* ����ʱ������������󣬽��ɷ������� */
        ARRCLR(fg_S3V3_on);	

        /* 50%ռ�ձ�������� */
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

    /* B����ʱ����ʱ */
    if (fg_zone_B_timed_antifreeze_S3V3_need)
    {
       /* ��ʱ����ʱ������������󣬽��ɷ������� */
        ARRCLR(fg_S3V3_on);	
        
        SETBIT_N(fg_S3V3_on, S3V3_ON_SIG_ZONE_B_TIMED_ANTIFREEZE);
    }
    else
    {
        CLRBIT_N(fg_S3V3_on, S3V3_ON_SIG_ZONE_B_TIMED_ANTIFREEZE);
    }

    /* �����ſ������п�S3V3 */
    if (GETBIT_N(fg_test_air_empty_on, TEST_AIR_EMPTY_S3V3_ON))
    {
        SETBIT_N(fg_S3V3_on, S3V3_ON_SIG_TEST_RUN_AIR_EMPTY);
    }
    else
    {
        CLRBIT_N(fg_S3V3_on, S3V3_ON_SIG_TEST_RUN_AIR_EMPTY);
    }

    /* ˮ�������п�S3V3 */
    if (GETBIT_N(fg_test_pump_on, TEST_PUMP_S3V3_ON))
    {
        SETBIT_N(fg_S3V3_on, S3V3_ON_SIG_TEST_RUN_PUMP);
    }
    else
    {
        CLRBIT_N(fg_S3V3_on, S3V3_ON_SIG_TEST_RUN_PUMP);
    }

    /* ������־������Ƿ���Ҫ���� */
    for (i = 0; i < S3V3_ON_SIG_MAX; i++)
    {
        if (GETBIT_N(fg_S3V3_on, i))
        {
            set_output(MAX_PCB, M_DO_S3V3); 
        }
    }
}

/*******************************************************************************
 �� ��: log_sys_valve()
 �� ��: ��������
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_valve(void)
{
    log_sys_S3V1();         /* �����ͨ��S3V1(��ˮ) */
    log_sys_S3V2();         /* �����ͨ��S3V2(A����ů�л�) */
    log_sys_S3V3();         /* �����ͨ��S3V3(B����ů) */
}

/*******************************************************************************
 �� ��: log_sys_UIRT()
 �� ��: �¿����߼�
 �� ��: ��
 �� ��: ��
 �� ��: ��������������Ч������ˮ��Ч
*******************************************************************************/
void log_sys_UIRT(void)
{
    BOL HDIrtc = FALSE;
    BOL HDIrth = FALSE;
    static BOL HDIrtc_bak = FALSE;
    static BOL HDIrth_bak = FALSE;
    BOL HDIrtc_is_changed = FALSE;/* �¿����źű��ı�ʱ��ΪTRUE */
    BOL HDIrth_is_changed = FALSE;
    static BOL fg_is_first_enter_UIRT = TRUE; /* �����¿������һ�ν���˺�����־��ȷ����һ������ */

    /* �������¿��� */
    if (!USE_UIRT_CTRL		                                                /* �����¿��� */
    || fg_holiday_away_run	                                                /* ����������ģʽ */
    || STEP_IS_ALARM		                                                /* ����̬ */
    || (smart_grid_act == SG_ACT_ALL_FORCE_OFF)				                
    )
    {
        HDIrtc_bak = FALSE;
        HDIrth_bak = FALSE;
        fg_is_first_enter_UIRT = TRUE;
        return;
    }
    
    if (m_get_input(IN_RT_HEAT))    HDIrth = TRUE;/* �¿��������ź� */
    if (m_get_input(IN_RT_COOL))    HDIrtc = TRUE;/* �¿��������ź� */

    /* HDIrtc���ı� */
    if (HDIrtc != HDIrtc_bak)
    {
        HDIrtc_bak = HDIrtc;
        HDIrtc_is_changed = TRUE;
    }

    /* HDIrth���ı� */
    if (HDIrth != HDIrth_bak)
    {
        HDIrth_bak = HDIrth;
        HDIrth_is_changed = TRUE;
    }

     /* A��ģʽ�л� */
    if (USE_UIRT_ZONE_A_MODE_SWITCH)
    {
        /* �¿����źű��ı� �� ���ú��һ�ν��뺯�� �ŷ������� */
        if (HDIrtc_is_changed || HDIrth_is_changed || fg_is_first_enter_UIRT)
        {
            if (HDIrtc)			        /* �����¿��������ź� */
            {
                para_set_cent(P_GaI(sv_mode), SET_COOL);
                SETCMD_N(ZONE_A_ON);
            }
            else if (!HDIrtc && HDIrth) /* �����¿��������ź� */
            {
                para_set_cent(P_GaI(sv_mode), SET_HEAT);
                SETCMD_N(ZONE_A_ON);
            }
            else				        /* ���¿��������źš����¿��������ź� */
            {
                SETCMD_N(ZONE_A_OFF);
            }
        }
    }

    /* A������ */
    if (USE_UIRT_ZONE_A_ON_OFF || USE_UIRT_DUAL_ZONE_ON_OFF)
    {
        /* �¿���c�źű��ı� �� ���ú��һ�ν��뺯�� �ŷ������� */
        if (HDIrtc_is_changed || fg_is_first_enter_UIRT)
        {
            /* A�� */
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

    /* B������ */
    if (USE_UIRT_DUAL_ZONE_ON_OFF)
    {
        /* �¿���h�źű��ı� �� ���ú��һ�ν��뺯�� �ŷ������� */
        if (HDIrth_is_changed || fg_is_first_enter_UIRT)
        {
            /* B�� */
            if (HDIrth)
            {
                /* �յ�����ģʽΪ����ʱ��B�����ɿ��� */
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
    

    if (m_get_input(IN_RT_HEAT))    // �����źſ�����
    {
        room_ther_input_h = TRUE;
    }

    if (m_get_input(IN_RT_COOL))    // �����źſ�����
    {
        room_ther_input_c = TRUE;
    }
    
    switch (val_RT_ctrl_type)
    {
        case UIRT_ZONE_A_MODE_SWITCHING:
            if ((room_ther_input_c)                 // ֻҪ�����źſ���������������
                && (val_air_type != HEAT_ONLY)     // ��������ģʽ
//                && (val_air_type != HOT_WATER_ONLY)
            )
            {
                room_therostat_mode = MODE_COOL;
            }
            else if ((room_ther_input_h)                // �����������źſ���������������
                    && (val_air_type != COOL_ONLY)     // ��������ģʽ
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

            if (fg_need_on_bak != fg_need_on)   // ���ػ�����
            {
                fg_need_on_bak = fg_need_on;
                
                if (fg_need_on)
                {
                    if (status_run == _IDLE || status_run == _DELAY)
                    {
                        Logic_machine_start();    /* ִ�п������� */
                    }
                }
                else
                {
                    if (status_run == _READY || status_run == _RUN)
                    {
                        Logic_machine_stop();     /* ִ�йػ����� */
                    }
                }
            }
            break;
            
        case UIRT_ZONE_A_ON_OFF:  // ����Ϊ���ػ���ʹ�������źſ�����
        case UIRT_DUAL_ZONE_ON_OFF:// ʹ�������źſ����� ����
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
                            Logic_machine_start();    /* ִ�п������� */
                        }
                        CMD_ZONE_B_ON;
                    }
                    else
                    {
                        if (status_run == _READY || status_run == _RUN)
                        {
                            Logic_machine_stop();     /* ִ�йػ����� */
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
 �� ��: log_sys_get_TWin1_max()
 �� ��: ��ȡ��ϵͳ������ģ���TWin1����ȡ���ֵ
 �� ��: type--; 
 �� ��: 
 �� ��: ��
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
 �� ��: log_sys_get_TWin1_min()
 �� ��: ��ȡ��ϵͳ������ģ���TWin1����ȡ��Сֵ
 �� ��: type--; 
 �� ��: 
 �� ��: ��
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
 �� ��: log_sys_get_TWout1_min()
 �� ��: ��ȡ��ϵͳ������ģ���TWout1����ȡ��Сֵ
 �� ��: type--; 
 �� ��: ��ϵͳ������ģ���TWout1
 �� ��: ��
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
 �� ��: log_sys_get_TWout2_min()
 �� ��: ��ȡ��ϵͳ������ģ���TWout2����ȡ��Сֵ
 �� ��: type--; 
 �� ��: ��ϵͳ������ģ��TWout2��Сֵ
 �� ��: ��
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
 �� ��: log_sys_init_idle_sta_antifreeze()
 �� ��: ��ʼ������̬��������
 �� ��: ��
 �� ��: ��
 �� ��: ��
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
 �� ��: log_sys_init_run_sta_ac_antifreeze()
 �� ��: ��ʼ������̬�յ�ˮ·��������
 �� ��: ��
 �� ��: ��
 �� ��: ��
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
 �� ��: log_sys_init_run_sta_hotw_antifreeze()
 �� ��: ��ʼ������̬��ˮˮ·��������
 �� ��: ��
 �� ��: ��
 �� ��: ��
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
 �� ��: log_sys_init_run_sta_antifreeze()
 �� ��: ��ʼ������̬��������
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_init_run_sta_antifreeze(void)
{
    log_sys_init_run_sta_ac_antifreeze();
    log_sys_init_run_sta_hotw_antifreeze();
    run_sta_antifreeze_stage = AF_STAGE_IDLE;
}

/*******************************************************************************
 �� ��: log_sys_init_zone_A_timed_antifreeze()
 �� ��: ��ʼ��A����ʱ��������
 �� ��: ��
 �� ��: ��
 �� ��: ��
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
 �� ��: log_sys_init_zone_B_timed_antifreeze()
 �� ��: ��ʼ��B����ʱ��������
 �� ��: ��
 �� ��: ��
 �� ��: ��
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
 �� ��: log_sys_init_hotw_timed_antifreeze()
 �� ��: ��ʼ����ˮ��ʱ��������
 �� ��: ��
 �� ��: ��
 �� ��: ��
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
 �� ��: log_sys_init_timed_antifreeze()
 �� ��: ��ʼ����ʱ��������
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_init_timed_antifreeze(void)
{
    log_sys_init_zone_A_timed_antifreeze();
    log_sys_init_zone_B_timed_antifreeze();
    log_sys_init_hotw_timed_antifreeze();
}

/*******************************************************************************
 �� ��: log_sys_zone_A_timed_antifreeze()
 �� ��: A����ʱ����
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_zone_A_timed_antifreeze(void)
{   
    BOL fg_no_zone_A_timed_antifreeze = FALSE;
    I16 antifreeze_temp = SENSOR_NOEXIST;
    I16 exit_antifreeze_temp = SENSOR_NOEXIST;
    
    if (fg_zone_A_run)		                                /* A������ */
    {
        fg_no_zone_A_timed_antifreeze = TRUE;
    }
    
    if (IS_DUAL_SYS
    && (!fg_zone_A_run && !fg_zone_B_run && fg_hotw_run)	/* A��δ���У�B��δ���У���ˮ���� */
    )
    {
        fg_no_zone_A_timed_antifreeze = TRUE;
    }

    /* ��A����ʱ����ʱ����ʼ����ر��� */
    if (fg_no_zone_A_timed_antifreeze)
    {
        log_sys_init_zone_A_timed_antifreeze();
        return;
    }

    /* ��ʱ�����¶� */
    antifreeze_temp = fun_pv_min(pv_sys_TACt1, pv_sys_TACt2);

    /* ��Ҫ����PUMPo */
    if ((pump_time[PM_O_TATFZ].off >= val_timed_antifreeze_interval)/* PUMPoͣ��ʱ�� �ݡ���ʱ��������� */
    && ((SensorCanUse(pv_sys_TA) && (pv_sys_TA <= val_anti_env_in)) /* TA���ã���TA �ܡ����������¡� */
       || SensorCannotUse(pv_sys_TA)                                /* TA������ */
       )
    )
    {
        fg_zone_A_timed_antifreeze_PUMPo_need = TRUE;
    }

    /* ����A����ʱ���� */
    if (SensorCannotUse(pv_sys_TA)    
    || (pv_sys_TA <= val_anti_env_in) 
    )
    {
        if (((pump_time[PM_O_TATFZ].on > 180)               /* PUMPo��������180S */
            && SensorCanUse(antifreeze_temp)                /* �����¶ȿ��� */
            && (antifreeze_temp <= val_anti_pump_on)        /* �����¶� �ܡ�ˮ�÷����¶ȡ� */
            )                                               
        || ((pump_time[PM_O_TATFZ].on > 0)                  /* PUMPo�ѿ��� */
           && SensorCannotUse(antifreeze_temp)              /* �����¶Ȳ����� */
           )                                                
        )
        {
            fg_zone_A_timed_antifreeze = TRUE;

            /* �����ͨ��1����ON������� */
            if (!fg_zone_A_run && !fg_zone_B_run)               /* A����B����δ����ʱ */
            {
                fg_zone_A_timed_antifreeze_S3V1_ON_need = TRUE;
            }
            else
            {
                fg_zone_A_timed_antifreeze_S3V1_ON_need = FALSE;
            }

            /* ѹ���̶�ת������ */
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

    /* �˶�ʱ�����¶� */
    exit_antifreeze_temp = fg_zone_A_timed_antifreeze_COMPf_need ? val_anti_comp_off : val_anti_pump_on;

    /* �˳�A����ʱ���� */
    if ((SensorCanUse(pv_sys_TA)                                /* TA���� */
        && (pv_sys_TA > val_anti_env_in + val_anti_env_diff)    /* TA �������������¡������˷������² */
        )                                               
    || ((pump_time[PM_O_TATFZ].on > 180)                        /* PUMPo��������180S */
       && SensorCanUse(antifreeze_temp)                         /* �����¶ȿ��� */
       && (antifreeze_temp > exit_antifreeze_temp)              /* �����¶� ���˷����¶� */
       )                                                
    || (pump_time[PM_O_TATFZ].on > val_timed_antifreeze_max)    /* PUMPo��������ʱ�� ������ʱ����ʱ���� */
    )
    {
        fg_zone_A_timed_antifreeze = FALSE;
        fg_zone_A_timed_antifreeze_PUMPo_need = FALSE;
        fg_zone_A_timed_antifreeze_S3V1_ON_need = FALSE;
        fg_zone_A_timed_antifreeze_COMPf_need = FALSE;
    }
}

/*******************************************************************************
 �� ��: log_sys_zone_B_timed_antifreeze()
 �� ��: B����ʱ����
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_zone_B_timed_antifreeze(void)
{
    BOL fg_no_zone_B_timed_antifreeze = FALSE;
    I16 antifreeze_temp = SENSOR_NOEXIST;
    I16 exit_antifreeze_temp = SENSOR_NOEXIST;

    if (!USE_DUAL_ZONE_CTRL                 /* ��˫������ */
    || fg_zone_B_run		                /* B������ */
    || fg_run_sta_ac_antifreeze				/* ��������̬�յ�ˮ·���� */
    )
    {
        fg_no_zone_B_timed_antifreeze = TRUE;
    }
    
    if (IS_DUAL_SYS
    && (!fg_zone_A_run && !fg_zone_B_run && fg_hotw_run)	/* A��δ���У�B��δ���У���ˮ���� */
    )
    {
        fg_no_zone_B_timed_antifreeze = TRUE;
    }

    /* ��B����ʱ����ʱ����ʼ����ر��� */
    if (fg_no_zone_B_timed_antifreeze)
    {
        log_sys_init_zone_B_timed_antifreeze();
        return;
    }

    /* ��ʱ�����¶� */
    antifreeze_temp = fun_pv_min(pv_sys_TACt1, pv_sys_TACt2);
    antifreeze_temp = fun_pv_min(antifreeze_temp, pv_sys_TFLin); 
     
    /* ��Ҫ����PUMPm */
    if ((pump_time[PM_M_TATFZ].off >= val_timed_antifreeze_interval)/* PUMPmͣ��ʱ�� �ݡ���ʱ��������� */
    && ((SensorCanUse(pv_sys_TA) && (pv_sys_TA <= val_anti_env_in)) /* TA���ã���TA �ܡ����������¡� */
       || SensorCannotUse(pv_sys_TA)                                /* TA������ */
       )
    )
    {
        fg_zone_B_timed_antifreeze_PUMPm_need = TRUE;
    }

    /* ����B����ʱ���� */
    if (SensorCannotUse(pv_sys_TA)    
    || (pv_sys_TA <= val_anti_env_in) 
    )
    {
        if (((pump_time[PM_M_TATFZ].on > 180)               /* PUMPm��������180S */
            && SensorCanUse(antifreeze_temp)                /* �����¶ȿ��� */
            && (antifreeze_temp <= val_anti_pump_on)        /* �����¶� �ܡ�ˮ�÷����¶ȡ� */
            )                                               
        || ((pump_time[PM_M_TATFZ].on > 0)                  /* PUMPm�ѿ��� */
           && SensorCannotUse(antifreeze_temp)              /* �����¶Ȳ����� */
           )                                                
        )
        {
            fg_zone_B_timed_antifreeze = TRUE;
            fg_zone_B_timed_antifreeze_S3V3_need = TRUE;

            /* �����ͨ��1����ON������� */
            if (!fg_zone_A_run && !fg_zone_B_run)               /* A����B����δ����ʱ */
            {
                fg_zone_B_timed_antifreeze_S3V1_ON_need = TRUE;
            }
            else
            {
                fg_zone_B_timed_antifreeze_S3V1_ON_need = FALSE;
            }

            /* ѹ���̶�ת������ */
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

    /* �˶�ʱ�����¶� */
    exit_antifreeze_temp = fg_zone_B_timed_antifreeze_COMPf_need ? val_anti_comp_off : val_anti_pump_on;

    /* �˳�B����ʱ���� */
    if ((SensorCanUse(pv_sys_TA)                                /* TA���� */
        && (pv_sys_TA > val_anti_env_in + val_anti_env_diff)    /* TA �������������¡������˷������² */
        )                                               
    || ((pump_time[PM_M_TATFZ].on > 180)                        /* PUMPm��������180S */
       && SensorCanUse(antifreeze_temp)                         /* �����¶ȿ��� */
       && (antifreeze_temp > exit_antifreeze_temp)              /* �����¶� ���˷����¶� */
       )                                                
    || (pump_time[PM_M_TATFZ].on > val_timed_antifreeze_max)    /* PUMPm��������ʱ�� ������ʱ����ʱ���� */
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
 �� ��: log_sys_hotw_timed_antifreeze()
 �� ��: ��ˮ��ʱ����
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_hotw_timed_antifreeze(void)
{
    BOL fg_no_hotw_timed_antifreeze = FALSE;
    I16 antifreeze_temp = SENSOR_NOEXIST;
    I16 exit_antifreeze_temp = SENSOR_NOEXIST;

    if (!val_hotw_used		                /* ����ˮ���� */
    || fg_hotw_run			                /* ��ˮ���� */
    || fg_run_sta_hotw_antifreeze			/* ��������̬��ˮˮ·���� */
    || IS_DUAL_SYS			                /* ˫ˮ·ϵͳ */
    )
    {
        fg_no_hotw_timed_antifreeze = TRUE;
    }

    /* ����ˮ��ʱ����ʱ����ʼ����ر��� */
    if (fg_no_hotw_timed_antifreeze)
    {
        log_sys_init_hotw_timed_antifreeze();
        return;
    }

    /* ��ʱ�����¶� */
    antifreeze_temp = pv_sys_THWt;

    /* ��ҪS3V1 */
    if ((pump_time[PM_HW_TATFZ].off >= val_timed_antifreeze_interval)   /* PUMPf��ˮ��ʱ����ͣ��ʱ�� �ݡ���ʱ��������� */
    && ((SensorCanUse(pv_sys_TA) && (pv_sys_TA <= val_anti_env_in))     /* TA���ã���TA �ܡ����������¡� */
       || SensorCannotUse(pv_sys_TA)                                    /* TA������ */
       )
    && (SensorCannotUse(antifreeze_temp) || antifreeze_temp<AF_HOTW_ENTER_TEMP)
    )
    {
        fg_hotw_timed_antifreeze_S3V1_OFF_need = TRUE;
    }

    /* ������ˮ��ʱ���� */
    if (SensorCannotUse(pv_sys_TA)    
    || (pv_sys_TA <= val_anti_env_in) 
    )
    {
        if (((pump_time[PM_HW_TATFZ].on > 180)                              /* PUMPf��������180S */
            && SensorCanUse(antifreeze_temp)                                /* THWt���� */
            && (antifreeze_temp <= val_anti_pump_on)                        /* ��ˮˮ���¶�THWt �ܡ�ˮ�÷����¶ȡ� */
            )   
        || ((pump_time[PM_HW_TATFZ].on > 0)                                 /* PUMPf�ѿ��� */
           && SensorCannotUse(antifreeze_temp)                              /* �����¶Ȳ����� */
           )                                        
        )
        {
            fg_hotw_timed_antifreeze = TRUE;

            /* ѹ���̶�ת������ */
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

    /* �˶�ʱ�����¶� */
    exit_antifreeze_temp = fg_hotw_timed_antifreeze_COMPf_need ? val_anti_comp_off : val_anti_pump_on;

    /* �˳���ˮ��ʱ���� */
    if ((SensorCanUse(pv_sys_TA)                                        /* TA���� */
        && (pv_sys_TA > val_anti_env_in + val_anti_env_diff)            /* TA �������������¡������˷������² */
        )                         
    || ((pump_time[PM_HW_TATFZ].on > 180)                               /* PUMPf��������180S */
       && SensorCanUse(antifreeze_temp)                                 /* �����¶ȿ��� */
       && (antifreeze_temp > exit_antifreeze_temp)                          /* �����¶� ����ˮ�÷����¶ȡ� */
       )
    || (pump_time[PM_HW_TATFZ].on > val_timed_antifreeze_max)           /* PUMPf��ˮ��ʱ��������ʱ�� ������ʱ����ʱ���� */
    )
    {
        fg_hotw_timed_antifreeze = FALSE;
        fg_hotw_timed_antifreeze_S3V1_OFF_need = FALSE;
        fg_hotw_timed_antifreeze_COMPf_need = FALSE;
    }
}

/*******************************************************************************
 �� ��: log_sys_timed_antifreeze()
 �� ��: ��ʱ�����߼�
 �� ��: ��
 �� ��: ��
 �� ��: ��
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
    || (STEP_IS_READY && status_ready==READY_IDLE)	        /* ���������׶Σ��ɴ���̬�������� */
    || (ac_mode == MODE_COOL)
    || fg_enter_test_run	                                /* �ѽ��������� */
    )
    {
        fg_no_timed_antifreeze = TRUE;
    }
    else if ((STEP_IS_READY && status_ready > READY_IDLE) 
    || STEP_IS_RUN
    || STEP_IS_DELAY
    )
    {
        if (fg_run_sta_ac_antifreeze        /* �ѽ�������̬�յ�ˮ·���� */
        || fg_run_sta_hotw_antifreeze       /* �ѽ�������̬��ˮˮ·���� */
        )
        {
            fg_no_timed_antifreeze = TRUE;
        }
    }

    /* �������� */
    if (m_get_error(MAX_PCB, M_ERR_PHASE_REVERSE)//����������ϲ��������
    ||  m_get_error(MAX_PCB, M_ERR_PHASE_LACK)   //�����ȱ����ϲ��������
    )
    {
        fg_no_timed_antifreeze = TRUE;
    }

    /* A����B������ˮ��δ���� */
    if (!fg_zone_A_run
    && !fg_zone_B_run
    && !fg_hotw_run
    )
    {
        fg_no_timed_antifreeze = TRUE;
    }

    /* �޶�ʱ�������� */
    if (fg_no_timed_antifreeze)
    {
        log_sys_init_timed_antifreeze();
        return;
    }

    /* A����ʱ���� */
    log_sys_zone_A_timed_antifreeze();

    /* B����ʱ���� */
    log_sys_zone_B_timed_antifreeze();

    /* ��ˮ��ʱ���� */
    log_sys_hotw_timed_antifreeze();

}

/*******************************************************************************
 �� ��: log_sys_antifreeze()
 �� ��: �����߼�
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_antifreeze(void)
{
    I16 pv_TWout2_min, pv_TWout1_min, pv_TWin1_min, pv_TWin1_max;
    I16 antifreeze_temp_enter = SENSOR_NOEXIST;             /* �������¶� */
    I16 antifreeze_temp_exit = SENSOR_NOEXIST;              /* �˷����¶� */
    static I16 antifreeze_temp_exit_bak[AF_TYPE_MAX];       /* �˷����¶ȱ��� */
    static U16 cnt_freeze_energy_time[PM_MAX];
    I16 temp_env = pv_sys_TA;                               /* �����¶� */
    U16 antifreeze_cycle = 3*QK_60;	                        /* ������� */
    U08 antifreeze_level = ANTI_ALL;        /* �����ȼ� */
    U08 pm_type = PM_F_ANY;
    U08 antifreeze_type = AF_IDLE_STA;
    U08 sub_sys_type = SUB_SYS_TYPE_MAX;
    
    U08* antifreeze_stage;                  /* �����׶�ָ�� */
    U08* antifreeze_status;                 /* ����״ָ̬�� */
    BOL* fg_antifreeze;                     /* ������־ָ�� */
    BOL* fg_antifreeze_pump_need;           /* ����������Ƶˮ������ */
    BOL* fg_antifreeze_IBH_need;            /* �������������������� */
    BOL* fg_antifreeze_COMPf_need;          /* ����������Ƶѹ������ָ�� */

    #define AF_TIME_INIT_MAX 0xffff

    /* ��ʼ�� */        
    antifreeze_stage = &idle_sta_antifreeze_stage;
    antifreeze_status = &idle_sta_antifreeze_status;
    fg_antifreeze = &fg_idle_sta_antifreeze;
    fg_antifreeze_pump_need = &fg_idle_sta_antifreeze_pump_need;
    fg_antifreeze_IBH_need = &fg_idle_sta_antifreeze_IBH_need;
    fg_antifreeze_COMPf_need = &fg_idle_sta_antifreeze_COMPf_need;

    if (val_anti_used == UNUSED
    ||  m_get_error(MAX_PCB, M_ERR_PHASE_REVERSE)//��Դ���ϲ��������
    ||  m_get_error(MAX_PCB, M_ERR_PHASE_LACK)   //��Դ���ϲ��������
    ||  !AIR_IS_WATER
    ||  power_time <= POWER_ON_FREEZ
    || !TF_IS_IDLE
    || fg_enter_test_run                                        /* �ѽ��������� */
    )   
    {
        log_sys_init_idle_sta_antifreeze();
        log_sys_init_run_sta_antifreeze();
        return;
    }
    else if (STEP_IS_IDLE
         || (STEP_IS_READY && status_ready==READY_IDLE)
    )/* ����̬���� */
    {
        /* ��ִ�д���̬�����߼������ */
        if (fg_zone_A_run || fg_zone_B_run || fg_hotw_run)		/* �յ�����ˮ���� */
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
    )/* ����̬���� */   
    {
        /* ��ִ������̬�����߼������ */
        if (((fg_zone_A_run || fg_zone_B_run) && fg_hotw_run)		/* �յ�����ˮͬʱ���� */
        || !val_hotw_used	                                        /* ����ˮ���� */
        || !IS_DUAL_SYS		                                        /* ��˫ˮ·ϵͳ */
        )
        {
            log_sys_init_idle_sta_antifreeze();
            log_sys_init_run_sta_antifreeze();
            return;
        }

        log_sys_init_idle_sta_antifreeze();
    
        if (!fg_zone_A_run	/* A��δ���� */
        && !fg_zone_B_run	/* B��δ���� */
        && fg_hotw_run		/* ��ˮ���� */
        )                   
        {
            /* ����̬�յ�ˮ·���� */
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
            /* ����̬��ˮˮ·���� */
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

            /* ��ִ������̬��ˮ��������� */
            if (SensorCanUse(pv_sys_THWt) && pv_sys_THWt>=AF_HOTW_ENTER_TEMP)
            {
                log_sys_init_run_sta_hotw_antifreeze();
                return;
            }
        }
    }
    
    /* �����¶� */
    if (antifreeze_type == AF_IDLE_STA)
    {
        pv_TWin1_max  = log_sys_get_TWin1_max(sub_sys_type);        /* ����ģ��TWin1���ֵ */
        
        pv_TWin1_min  = log_sys_get_TWin1_min(sub_sys_type);        /* ����ģ��TWin1��Сֵ */
        pv_TWout1_min = log_sys_get_TWout1_min(sub_sys_type);       /* ����ģ��TWout1��Сֵ */
        pv_TWout2_min = log_sys_get_TWout2_min(sub_sys_type);       /* ����ģ��TWout2��Сֵ */

        antifreeze_temp_enter = fun_pv_min(pv_TWin1_min, pv_TWout1_min);
        antifreeze_temp_enter = fun_pv_min(antifreeze_temp_enter, pv_TWout2_min);
        antifreeze_temp_enter = fun_pv_min(antifreeze_temp_enter, pv_sys_TWout3);   /* ϵͳ�ܳ�ˮ�¶� */
        antifreeze_temp_enter = fun_pv_min(antifreeze_temp_enter, pv_sys_TACt1);    /* ����ˮ���ϲ��¶� */
        antifreeze_temp_enter = fun_pv_min(antifreeze_temp_enter, pv_sys_TACt2);    /* ����ˮ���²��¶� */
        antifreeze_temp_enter = fun_pv_min(antifreeze_temp_enter, pv_sys_TFLin);    /* ��ů��ˮ�¶� */
        antifreeze_temp_enter = fun_pv_min(antifreeze_temp_enter, pv_sys_THWt);     /* ��ˮˮ���¶� */

        antifreeze_temp_exit = fun_pv_min(pv_TWin1_min, pv_TWout1_min);
        antifreeze_temp_exit = fun_pv_min(antifreeze_temp_exit, pv_TWout2_min);
        /* 2023-1-11 !!! �˷����¶Ȳ��ɿ��������¶� */
//      antifreeze_temp_exit = fun_pv_min(antifreeze_temp_exit, pv_sys_TWout3);   /* ϵͳ�ܳ�ˮ�¶� */        
//      antifreeze_temp_exit = fun_pv_min(antifreeze_temp_exit, pv_sys_TACt1);    /* ����ˮ���ϲ��¶� */
//      antifreeze_temp_exit = fun_pv_min(antifreeze_temp_exit, pv_sys_TACt2);    /* ����ˮ���²��¶� */
//      antifreeze_temp_exit = fun_pv_min(antifreeze_temp_exit, pv_sys_TFLin);    /* ��ů��ˮ�¶� */
    }
    else if (antifreeze_type == AF_RUN_STA_AC)
    {
        pv_TWin1_max  = log_sys_get_TWin1_max(sub_sys_type);     /* ���в�������ˮģ��TWin1���ֵ */
        
        pv_TWin1_min  = log_sys_get_TWin1_min(sub_sys_type);     /* ���в�������ˮģ��TWin1��Сֵ  */
        pv_TWout1_min = log_sys_get_TWout1_min(sub_sys_type);    /* ���в�������ˮģ��TWout1��Сֵ */
        pv_TWout2_min = log_sys_get_TWout2_min(sub_sys_type);    /* ���в�������ˮģ��TWout2��Сֵ */
    
        antifreeze_temp_enter = fun_pv_min(pv_TWin1_min, pv_TWout1_min);
        antifreeze_temp_enter = fun_pv_min(antifreeze_temp_enter, pv_TWout2_min);
        antifreeze_temp_enter = fun_pv_min(antifreeze_temp_enter, pv_sys_TWout3);   /* ϵͳ�ܳ�ˮ�¶� */
        antifreeze_temp_enter = fun_pv_min(antifreeze_temp_enter, pv_sys_TACt1);    /* ����ˮ���ϲ��¶� */
        antifreeze_temp_enter = fun_pv_min(antifreeze_temp_enter, pv_sys_TACt2);    /* ����ˮ���²��¶� */
        antifreeze_temp_enter = fun_pv_min(antifreeze_temp_enter, pv_sys_TFLin);    /* ��ů��ˮ�¶� */

        antifreeze_temp_exit = fun_pv_min(pv_TWin1_min, pv_TWout1_min);
        antifreeze_temp_exit = fun_pv_min(antifreeze_temp_exit, pv_TWout2_min);
    }
    else if (antifreeze_type == AF_RUN_STA_HOTW)
    {
        pv_TWin1_max  = log_sys_get_TWin1_max(sub_sys_type);    /* ���п�����ˮģ��TWin1���ֵ */
        
        pv_TWin1_min  = log_sys_get_TWin1_min(sub_sys_type);	/* ���п�����ˮģ��TWin1��Сֵ  */
        pv_TWout1_min = log_sys_get_TWout1_min(sub_sys_type);	/* ���п�����ˮģ��TWout1��Сֵ */
        pv_TWout2_min = log_sys_get_TWout2_min(sub_sys_type);	/* ���п�����ˮģ��TWout2��Сֵ */

        antifreeze_temp_enter = fun_pv_min(pv_TWin1_min, pv_TWout1_min);
        antifreeze_temp_enter = fun_pv_min(antifreeze_temp_enter, pv_TWout2_min);
        antifreeze_temp_enter = fun_pv_min(antifreeze_temp_enter, pv_sys_TWout3);   /* ϵͳ�ܳ�ˮ�¶� */
        antifreeze_temp_enter = fun_pv_min(antifreeze_temp_enter, pv_sys_THWt);     /* ��ˮˮ���¶� */

        antifreeze_temp_exit = fun_pv_min(pv_TWin1_min, pv_TWout1_min);
        antifreeze_temp_exit = fun_pv_min(antifreeze_temp_exit, pv_TWout2_min);
    }
    
    /* ��������ѡ�� */
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

    /* ��������ж� */
    if (SensorCannotUse(antifreeze_temp_exit))           /* �յ�����ˮ�¶ȶ��й��ϣ�������� */
    {
        antifreeze_level = ANTI_UNUSUAL;
    }
    else if (SensorCannotUse(temp_env))      /* �����¶ȹ���ʱ��������������Ϊ�����¶�ȱʡ���� */                         
    {
        antifreeze_level = ANTI_NON_ENV;
    }

    /* �жϽ������ */
    if (*fg_antifreeze == FALSE)
    {
        *antifreeze_status = ANTIFREEZE_IDLE;
        *antifreeze_stage = AF_STAGE_IDLE;
        freeze_energy_time[pm_type] = 0;       
        
        if (pump_time[pm_type].off >= antifreeze_cycle)
        {
            if (antifreeze_level == ANTI_UNUSUAL)         /* ����������ʱ�Ĵ���  */ 
            {       
                if (SensorCanUse(temp_env))    
                {
                    if (temp_env <= val_anti_env_in)    // �����¶�С��2�Ž������
                    {
                        *fg_antifreeze = TRUE;
                    }
                }
                else                 /* �����¶ȹ���ʱ��ֱ�ӽ������ */
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
            else          /* �������� */
            {
                if (temp_env < val_anti_env_in)
                {
                    *fg_antifreeze = TRUE;
                }
            }
        }
    }
    
    /* ����������� */
    if (antifreeze_level == ANTI_UNUSUAL)            /* �������������ص�������ֱ�ӷ��� */
    {
        /* ��ȡһ�����ڼ�ʱ */
        U16 cnt_freeze_cycle = freeze_act_time%ANTI_UNUSUAL_DLY;

        if (*fg_antifreeze)
        {
            *fg_antifreeze_pump_need = TRUE;
            
            if (SensorCanUse(temp_env))       /* �����¶��޹���ʱ */                        
            {
                if (temp_env > val_anti_env_in + val_anti_env_diff)       /* �����¶ȴ���4ʱ�˳� */
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

                    /* 50%ռ�ձ�������� */
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

    /* ���ȷ������� */
    if (*antifreeze_status > ANTIFREEZE_PUMP
    &&  *antifreeze_status < ANTIFREEZE_CLOSE_COMP
    &&  antifreeze_level != ANTI_UNUSUAL
    &&  temp_env <= val_anti_env_in+val_anti_env_diff		/* δ�����˷����������� */
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

    /* ˮ�á�ѹ���������� */
    switch(*antifreeze_status)
    {
        case ANTIFREEZE_IDLE:
            if (*fg_antifreeze)
            {
                if ((fg_ac_pump_firstopen==0 && freeze_energy_time[pm_type]>random_time)  // �״ο�ˮ�ã���Ҫ��ֹ�ϵ�ͬʱ������    
                || fg_ac_pump_firstopen==1) 
                {
                    *antifreeze_status = ANTIFREEZE_PUMP;
                    
                    if (antifreeze_type == AF_IDLE_STA
                    || antifreeze_type == AF_RUN_STA_AC
                    )
                    {
                        *antifreeze_stage = AF_STAGE_AC;    /* ��ִ�пյ�ˮ·���� */
                    }
                    else
                    {
                        *antifreeze_stage = AF_STAGE_HOTW;  /* ��ִ����ˮˮ·���� */
                    }
                    freeze_energy_time[pm_type] = 0;
                }
                
                if (log_sys_have_cell_pump_run(sub_sys_type))
                {
                    if (log_sys_have_comp_run(MODE_COOL, sub_sys_type))
                    {
                        /* �����ǰѹ�����������У�����Ҫ��ͣ��ϵͳ�е�ѹ�� */
                        log_sys_select_comp_stop_once(MODE_COOL, sub_sys_type);
                    }
                }
            }
            break;
        case ANTIFREEZE_PUMP:
            fg_ac_pump_firstopen=1;         // �Ѿ�����ˮ��  
            *fg_antifreeze_pump_need = TRUE;    /* ���յ��� */
            
            if (freeze_energy_time[pm_type] > ANTI_PRE_DLY)    
            {
                freeze_energy_time[pm_type] = 0;
                *antifreeze_status = ANTIFREEZE_DEAL;
            }
            break;
        case ANTIFREEZE_DEAL:            /* ���ݷ����¶�ֵ����ˮ�ö������� */
            if (antifreeze_temp_exit <= val_anti_comp_on)            /* ��ѹ���� */
            {
                freeze_energy_time[pm_type] = AF_TIME_INIT_MAX;
                antifreeze_temp_exit_bak[antifreeze_type] = antifreeze_temp_exit;
                *antifreeze_status = ANTIFREEZE_OPEN_COMP;
            }
            else if (antifreeze_temp_exit <= val_anti_pump_on)       /* ά��ˮ�ÿ��� */
            {
                if (!(*fg_antifreeze_IBH_need))
                {
                    if (freeze_energy_time[pm_type] > ANTI_CHG_DLY)
                    {
                        *antifreeze_status = ANTIFREEZE_CLOSE_COMP;		/* ��ѹ���˷��� */
                        freeze_energy_time[pm_type] = 0;
                    }
                }
                else		/* ���ȷ��� */
                {
                    if (freeze_energy_time[pm_type] > ANTI_EXIT_DLY)
                    {
                        *antifreeze_status = ANTIFREEZE_CLOSE_COMP;		/* ��ѹ���˷��� */
                        freeze_energy_time[pm_type] = 0;
                    }
                }
            }
            else                                                /*  ͣˮ�ã��˳����� */
            {
                if (!(*fg_antifreeze_IBH_need))
                {
                    *antifreeze_status = ANTIFREEZE_CLOSE_COMP;		/* ��ѹ���˷��� */
                    freeze_energy_time[pm_type] = 0;
                }
                else		/* ���ȷ��� */
                {
                    if (freeze_energy_time[pm_type] > ANTI_EXIT_DLY)
                    {
                        *antifreeze_status = ANTIFREEZE_CLOSE_COMP;		/* ��ѹ���˷��� */
                        freeze_energy_time[pm_type] = 0;
                    }
                }
            }
            
            if (temp_env > val_anti_env_in + val_anti_env_diff      //�����¶��˷��� */ 
            ||  pv_TWin1_max > AF_SV_TEMP_FREQ
            )
            {            
                *antifreeze_status = ANTIFREEZE_CLOSE_COMP;		/* ��ѹ���˷��� */
                freeze_energy_time[pm_type] = 0;
            }
            break;
        case ANTIFREEZE_OPEN_COMP:
            if (log_sys_have_cell_pump_run(sub_sys_type))
            {
                if (freeze_energy_time[pm_type] >= 480)               /*ÿ��8���Ӽ���һ̨ѹ�� */
                {
                    if (log_sys_have_comp_run(MODE_COOL, sub_sys_type))
                    {
                        /* �����ǰѹ�����������У�����Ҫ��ͣ��ϵͳ�е�ѹ�� */
                        log_sys_select_comp_stop_once(MODE_COOL, sub_sys_type);
                    }
                    else
                    {                   
                        if (antifreeze_temp_exit - antifreeze_temp_exit_bak[antifreeze_type] >= 10)   /* ��������1 */
                        {
                            ;
                        }
                        else        /* ����С��1ʱ���� */
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

                    /* ��freeze_energy_time���浽cnt_freeze_energy_time�� */
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

                *fg_antifreeze_COMPf_need = TRUE;	/* ��ѹ���������� */
            }
            else
            {
                /* 2023-1-13���������˹��޸ı����Żᴥ�����������������ֹ��ʱ�˳����� */
                freeze_energy_time[pm_type] = 480;
            }
            
            if (antifreeze_temp_exit > val_anti_comp_off
            ||  freeze_energy_time[pm_type] > ANTI_EXIT_DLY
            ||  cnt_freeze_energy_time[pm_type] > ANTI_EXIT_DLY
            ||  temp_env > val_anti_env_in + val_anti_env_diff
            ||  pv_TWin1_max > AF_SV_TEMP_FREQ
            )
            {
                *antifreeze_status = ANTIFREEZE_CLOSE_COMP;		/* ��ѹ���˷��� */
                freeze_energy_time[pm_type] = 0;
                cnt_freeze_energy_time[pm_type] = 0;
            }
            break;
        case ANTIFREEZE_CLOSE_COMP:
            *fg_antifreeze_IBH_need = FALSE;
            
            if (freeze_energy_time[pm_type] >= 2)   /* ÿ����ͣһ̨ѹ�� */
            {
                freeze_energy_time[pm_type] = 0;
                log_sys_select_comp_stop_once(MODE_HEAT, sub_sys_type);
            }
            
            if (!log_sys_have_comp_run(MODE_HEAT, sub_sys_type))      /* ����ѹ��ͣ����ת��ͣˮ�� */
            {
                if (antifreeze_type == AF_IDLE_STA
                && *antifreeze_stage == AF_STAGE_AC
                &&  val_hotw_used
                )
                {
                    *antifreeze_stage = AF_STAGE_HOTW;	    /* �л�����ˮ����� */
                    *antifreeze_status = ANTIFREEZE_PUMP;
                    freeze_energy_time[pm_type] = 0;
                }
                else
                {
                    *antifreeze_status = ANTIFREEZE_CLOSE_PUMP;   /*  ��ˮ���˷��� */
                    freeze_energy_time[pm_type] = 0;
                }
                *fg_antifreeze_COMPf_need = FALSE;          /* ��ѹ���������� */
            }
            break;
        case ANTIFREEZE_CLOSE_PUMP:
            if (freeze_energy_time[pm_type] > 60)  /* ����ѹ�����رպ���ʱ60��ͣˮ�ã��˳����� */ 
            {
                *fg_antifreeze = FALSE;
                *fg_antifreeze_pump_need = FALSE;    /* �ؿյ��� */
                *antifreeze_status = ANTIFREEZE_IDLE;
                *antifreeze_stage = AF_STAGE_IDLE;
            }
            break;
        default:
            break;
    }
}

/*******************************************************************************
 �� ��: log_sys_get_flag_area_antifreeze()
 �� ��: ��ȡ���������־����������־
 �� ��: ��
 �� ��: ��
 �� ��: ��
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
 �� ��: log_sys_refri_recycle()
 �� ��: �շ�����ý����
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_refri_recycle(void)
{
    static U16 cnt_refri;

    /* �շ��ж� */
    if (val_recycle_enter || cmd_refri_recy_enter)
    {
        cmd_refri_recy_enter = FALSE;
        
        if (!fg_idle_sta_antifreeze
        &&  !fg_refri_recy_act
        )
        {
            /* ������ͣ��״̬�£���������շ� */
            if (status_run == _IDLE || status_run == _DELAY)
            {
                fg_refri_recy_act = TRUE;       /* �����շ� */
                SETCMD_N(ZONE_A_ON);			/* A������ */
            }
        }
    }

    /* �շ���ʱ�˳� */
    if (cnt_refri >= 8*60)
    {
        cnt_refri = 0;
        cmd_refri_recy_stop = TRUE;
    }

    /* �����˳��շ� */
    if (cmd_refri_recy_stop)
    {
        cmd_refri_recy_stop = FALSE;

        if (fg_refri_recy_act)
        {
            fg_refri_recy_act = FALSE;

            cmd_refri_recy_enter = FALSE;
            para_set_cent(P_GaI(val_recycle_enter), FALSE);
            SETCMD_N(ZONE_A_OFF);			/* A���ػ� */
        }
    }

    /* ��ʱ���� */
    if (F_TIME_BASIC_SEC_BACK)
    {
        _CNT_WHEN(cnt_refri, fg_refri_recy_act);
    }
}

/*******************************************************************************
 �� ��: log_sys_need_by_user()
 �� ��: ĩ��ʹ���߿����󣬻�ȡ����״̬(������������)
 �� ��: ��
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: �Ƿ��ܹ�����
*******************************************************************************/
BOOL log_sys_need_by_user(void)
{
    BOOL fg_switch_together_run = TRUE;     // ��������������

    if (REMOTE_IS_UNION_KEEP && !m_get_input(IN_REMOTE)     /* �������ضϿ� */
    ||  !fcu_has_egy_need()                                 /* ����̹������� */
    )
    {
        fg_switch_together_run = FALSE;
    }

    return fg_switch_together_run;
}

#if 0
/*******************************************************************************
 �� ��: log_sys_interlock_run()
 �� ��: ��������_READY״̬�£����ж�ͨ�������պ������ж�
 �� ��: ��
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: �Ƿ��ܹ�����
*******************************************************************************/
BOOL log_sys_interlock_run(void)
{
    BOOL fg_interlock_closed;
    BOOL fg_link_switch = LINK_UNLOCK;            /* ��ͨ���Ͽ���ͣ�� */

    if (ctrl_mode==MODE_COOL                    /* ����ʱ����ͨ������������Ч */
    || (ctrl_mode==MODE_HEAT && val_link_en))       /* �����ҡ����ȶ�ͨ������Ϊʹ��ʱ����ͨ������������Ч */
    {
        fg_link_en = TRUE;  /* ��ͨ��������Ч */
    }
    else    /* �����ҡ����ȶ�ͨ������Ϊ����ʱ����ͨ������������Ч */
    {
        fg_link_en = FALSE;  /* ��ͨ��������Ч */
    }
    
    /* ��ͨ������ */
    if (REMOTE_IS_INTERLOCK)
    {
        if (!fg_link_en)    /* ����ʧЧ��ֱ�ӿ��� */
        {
            fg_interlock_closed = TRUE;
        }
        else    /* ������Ч��ʱ�䵽֮�󿪻� */
        {
            fg_interlock_closed = FALSE;    /* ��λ */

            /* ��ͨ�������պ�(=0) */
            if (!m_get_input(IN_LINK))   /* ��ͨ���źţ�=1Ϊ�Ͽ���=0Ϊ�պ� */
            {
                fg_link_switch = LINK_LOCK; 
            }

            /* �պϳ���1���� */
            if ( fg_link_switch == LINK_LOCK && link_on_cnt >= LINK_LAST) 
            {
                fg_interlock_closed = TRUE;
            }
        }        
    }
    else    /* ��ʹ�ö�ͨ������ֱ��ͨ�� */
    {
        fg_interlock_closed = TRUE;
    }  

    return fg_interlock_closed;
}

/*******************************************************************************
 �� ��: log_sys_interlock_stop()
 �� ��: ��������_READY/_RUN״̬�£����ж�ͨ�������Ͽ�ͣ���ж�
 �� ��: ��
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL log_sys_interlock_stop(void)
{
    BOOL fg_interlock_opened;
    BOOL fg_link_switch = LINK_LOCK;            /* ��ͨ����ס���������� */

    if (ctrl_mode==MODE_COOL                    /* ����ʱ����ͨ������������Ч */
    || (ctrl_mode==MODE_HEAT && val_link_en))       /* �����ҡ����ȶ�ͨ������Ϊʹ��ʱ����ͨ������������Ч */
    {
       fg_link_en = TRUE;  /* ��ͨ��������Ч */
    }
    else    /* �����ҡ����ȶ�ͨ������Ϊ����ʱ����ͨ������������Ч */
    {
        fg_link_en = FALSE;  /* ��ͨ��������Ч */
    }

    /* ��ͨ��������Ч */
    if (REMOTE_IS_INTERLOCK)
    {
        if (!fg_link_en)    /* ����ʧЧ������ͣ�� */
        {
            fg_interlock_opened = FALSE;
        }
        else    /* ������Ч��ʱ�䵽֮��ͣ�� */
        {
            fg_interlock_opened = FALSE;
        
            /* ��ͨ�������Ͽ�(=1) */
            if (m_get_input(IN_LINK))   /* ��ͨ���źţ�=1Ϊ�Ͽ���=0Ϊ�պ� */
            {
                fg_link_switch = LINK_UNLOCK; 
            }

            /* �Ͽ�����1���� */
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
 �� ��: log_sys_to_ready_by_together()
 �� ��: �������ضϿ���ִ����ת��׼��̬
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_to_ready_by_together(void)
{
    static BOOL fg_step_to_ready;
    static U08 to_ready_stop_time;              // ת��׼��̬ʱͣ�������ʱ

    if (STEP_IS_ALARM || STEP_IS_IDLE || STEP_IS_DELAY)
    {
        fg_step_to_ready = FALSE;
    }
    else if (StepIsReady && status_ready > READY_IDLE
         ||  StepIsRun)
    {
        // �ж��Ƿ���Ҫת����ready
        if (!log_sys_need_by_user())
        {
            fg_step_to_ready = TRUE;
        }
    }

    // ��ת��ִ�й��̣��ȹ�ѹ�����ȣ������ʱ��ˮ��
    if (fg_step_to_ready)
    {
        if (to_ready_stop_time > 3)                         // ͣѹ�� 
        {
            to_ready_stop_time = 0;
            
            if (!log_sys_have_comp_run(MODE_NULL, SUB_SYS_TYPE_MAX))
            {
                status_run = _READY;            // ѹ������ͣ��ת��_READY
                status_ready = READY_IDLE;      // _READY̬Ϊ����̬��׼����
                fg_step_to_ready = FALSE;
            }
            
            log_sys_select_comp_stop(MODE_NULL);
        }
    }

    /* ��ʱ */
    if (F_TIME_BASIC_SEC_BACK)
    {
        _CNT(to_ready_stop_time);
    }
}

#if 0
void log_sys_two_way_valve_interlock(void)
{
    static BOOL fg_step_to_ready = FALSE;
    static U08 to_ready_stop_time;              // ת��׼��̬ʱͣ�������ʱ

    if (STEP_IS_ALARM || STEP_IS_IDLE)
    {
        fg_link_en = FALSE;  /* ��ͨ��������Ч */
        fg_step_to_ready = FALSE;
    }
    else if (StepIsReady && status_ready > READY_IDLE
         ||  StepIsRun)
    {
        // �ж��Ƿ���Ҫת����ready
        if (log_sys_interlock_stop())
        {
            fg_step_to_ready = TRUE;
        }
        else
        {
            fg_step_to_ready = FALSE;
        }
    }

    // ��ת��ִ�й��̣��ȹ�ѹ�����ȣ������ʱ��ˮ��
    if (fg_step_to_ready)
    {
        if (to_ready_stop_time > 3)                         // ͣѹ�� 
        {
            to_ready_stop_time = 0;
            
            if (!log_sys_have_comp_run(MODE_NULL))
            {
                status_run = _READY;            // ѹ������ͣ��ת��_READY
                status_ready = READY_IDLE;      // _READY̬Ϊ����̬��׼����
                fg_step_to_ready = FALSE;
            }
            
            log_sys_select_comp_stop(MODE_NULL);
        }
    }

    /* ��ʱ */
    if (F_TIME_BASIC_SEC_BACK)
    {
        _CNT(to_ready_stop_time);
    }

}
#endif

/*==========================================
    ����: ��������
/==========================================*/
void log_sys_ready(void)
{
    switch(status_ready)
    {
        case READY_IDLE:
            if ((fg_ac_pump_firstopen==0 && ready_time>random_time) // �״ο�ˮ�ã���Ҫ��ֹ�ϵ�ͬʱ������    
                || fg_ac_pump_firstopen==1                          // ���״ο�ˮ��
            )
            {
                ready_time = 0;
                if (fg_warm_complete
                 && power_time > POWER_ON_PREHEAT+1			/* ����ԭ��POWER_ON_PREHEAT��POWER_ON_INTO_WORK */
                 && !fg_idle_sta_antifreeze
                 && log_sys_need_by_user()
                 )
                {
                    status_ready = READY_PUMP;
                }
            }
            break;
        case READY_PUMP:
            fg_ac_pump_firstopen=1; // �Ѿ�����ˮ��  
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
//        case READY_COOL_PUMP:   /* ȡ������ʱ����ȴ����ʱ */
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
 �� ��: log_sys_delay()
 �� ��: ͣ�����̴���
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_delay(U08 pcb_num)
{
    U08 i;

    fg_have_stop = TRUE;    /* ͣ��Ҳ��λ�ñ�־ */
    
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
            if (delay_time > 3)                  /* ͣѹ�� */
            {
                delay_time = 0;
                
                if (!log_sys_have_comp_run(MODE_NULL, SUB_SYS_TYPE_MAX))
                {
                    status_delay = DELAY_COOL_PUMP;       /* ѹ������ͣ������ʱͣˮ�� */
//                    ws_pump_dly = 0;
                }
                
                log_sys_select_comp_stop(MODE_NULL);  /* ע��: �˾�Ӧ�����ж�ѹ������֮��, ����ĳЩѹ�������޷��ص���ŷ� */
            }
            break;
            
        case DELAY_COOL_PUMP:                   /* ͣ��ȴˮ�� */
        //  if (ws_pump_dly >= WS_PUMP_OFF_DLY)
            {
                status_delay = DELAY_PUMP;
            }
            break;
            
        case DELAY_PUMP:                         /* ͣ�յ�ˮ�� */
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
            else    /* ��ֹѹ����δͣ�� */
            {
                status_delay = DELAY_COMP;
            }
            break;
            
        default:
            break;
    }
}
/*****************************************************
    ����: ����̬
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
    ������Logic_Reset
    ���ܣ�����������������ʱ�ĸ�λ
    ��������
    ��������
*******************************************************/
void log_sys_idle(U08 pcb_num)
{
    U08 i;

    fg_have_stop = TRUE;
    
    /* ����ʱ����ʼ�����±��� */
    if (fg_idle_sta_antifreeze				/* ����ʱ����ʼ�����±��� */
    ||  TF_EN               /* �̼�ʱ����ʼ�����±��� */
    )
    {
        return;
    }
    
    if (log_sys_have_cell_run())    /* ����ģ��������, ת��ͣ��̬ */
    {
        status_run = _DELAY;
    }

    /* ����ģ�����г�ʼ�� */
    m_ctrl_time = 0xffff;   /* ��ģ���ܵ�����ʱ�� */
    s_ctrl_time = 0xffff;   /* ��ģ���ܵ�����ʱ�� */
    
//    fg_ac_pm_open = FALSE; 
    
    for(i=0; i<m_val_comp_num(pcb_num); i++)
    {
        fg_need_comp_to_run[i] = 0;
    }
}
/***********************************************
������ log_sys_is_rdy
���ܣ� ϵͳģʽת��ʱ��ˮ�ÿ��������ת������Ӧ��ģʽ
������ ģ�����
����ֵ�� ����ģʽ�ɿ�
         TRUE: �ɿ�; FALSE: ���ɿ�
************************************************/
U08 log_sys_is_rdy(void)
{
//  U08 j;
    U08 fg_comp_mode_wait = FALSE;
//  static U08 sys_mode_last;  /* ԭ����ģʽ */
    
    if (status_run == _IDLE)    /* ����״̬ */
    {
        if (!fg_idle_sta_antifreeze         /* δ����̬���� */
        &&  !TF_EN                          /* δ�̼� */
        )
        {
//          sys_mode_last = 0xff;
            fg_comp_mode_wait = TRUE;
        }
    }
    
//    if (!log_sys_AC_is_ready() ) /* �յ�����ȴ��δ�����ȴ� */
//    {
//        fg_comp_mode_wait = TRUE;
//    }
        

    return !fg_comp_mode_wait;
}

/*******************************************************************************
 �� ��: m_set_di()
 �� ��: ϵͳ���������ǳ������պ�Ŀ���������
 �� ��: input ���������  di_para �������ղ���
 di_para: 0 ���� �� 1 ����
 �� ��: ��
 �� ��: 
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
 �� ��: log_sys_ctrl_temp_cpst()
 �� ��: ���ݳ�ˮ�¶Ȳ���ѭ��ˮ·�¶�
 �� ��: tmp_ctrl--�������¶�; 
 �� ��: �������¶�
 �� ��: ��
*******************************************************************************/
I16 log_sys_circle_water_cpst(I16 tmp_ctrl)
{
    #define TMP_ENV1 (10*30)
    #define TMP_ENV2 (10*23)
    #define TMP_DIFF (10*2)    

    #define val_cpst_coef_high              (3)     // ˮ�²���ϵ����
    #define val_cpst_coef_mid               (2)     // ˮ�²���ϵ����
    #define val_cpst_coef_low               (1)     // ˮ�²���ϵ����

    I16 result = tmp_ctrl;
    BOL fg_en = FALSE;
    I16 tmp_coef = 0;
    I16 tmp_out = pv_sys_TWout2;
    I16 tmp_env = pv_sys_TA;

    /* �Ƿ���Ҫ���� */
    if (SensorCanUse(tmp_out) 
    &&  tmp_out>val_cpst_bgn_tmp
    &&  val_cpst_bgn_tmp!=0 /* Ϊ0��ʾ���øù��� */
    )
    {
        fg_en = TRUE;
    }

    if (fg_en)
    {
        /* ȷ������ϵ�� */
        tmp_coef = tmp_env>TMP_ENV1 ? val_cpst_coef_high :
                    tmp_env>TMP_ENV2 ? val_cpst_coef_mid :
                     val_cpst_coef_low;
                     
        /* ȷ���������¶� */
        result = fun_pv_cmpst(result, (tmp_out-val_cpst_bgn_tmp)*tmp_coef/5);
    }

    return result;
}
#endif

#if 0
/*******************************************************************************
 �� ��: log_sys_get_load_cpst_value()
 �� ��: ���ݸ��ɸߵͻ�ȡ����ֵ
 �� ��: ��
 �� ��: val_cpst��>0 ���͸���������<0 ���߸���������=0 ��������
 �� ��: 
    1����ʹ�����ڲ���ʱ���ʸú���һ�δ�ѭ��������ִ��һ��
    2������������������ֵ������ֵ���ʹ�����ⲿ����
*******************************************************************************/
I16 log_sys_get_load_cpst_value(void)
{
    static I16 val_cpst;        /* ����ֵ */
    
    static U16 time_low_load;   /* �͸��ɼ�ʱ */
    static U16 time_high_load;  /* �߸��ɼ�ʱ */
    static U16 cnt_low_load;    /* �͸��ɼ��� */
    static U16 cnt_high_load;   /* �߸��ɼ��� */
    #define LOAD_CPST_NUM 3     /* ������������ */

    static BOL fg_enable;       /* �����ж�ʹ�ܱ�־ */
    static I16 sv_temp_bak;     /* Ŀ���¶ȱ��� */


    /* ��ʱ */
    {
        if (F_TIME_BASIC_MIN_BACK)          /* �ּ�ʱ */
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
                /* �͸��� */
                if (fg_enable)
                {
                    if (time_low_load < val_load_revise_time)
                    {
                        _CNT(cnt_low_load);
                        cnt_high_load = 0;  /* �͸���ʱ��ո߸��ɼ�������ʱ */
                        time_high_load = 0;
                    }
                }

                /* ����ͣ������ */
                {
                    time_low_load = 0;
                    time_high_load = 0;
                    fg_enable = FALSE;
                }
                
            }

            /* �߸��� */
            if (time_high_load >= val_load_revise_time)
            {
                time_high_load = 0;
                
                _CNT(cnt_high_load);
                cnt_low_load = 0;           /* �߸���ʱ��յ͸��ɼ�������ʱ */
                time_low_load = 0;
            }
        }
    }

    /* ��������ֵ */
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

    /* ���� */
    {
        sv_temp_bak = sv_temp_energy;
    }

    return val_cpst;
}

/*******************************************************************************
 �� ��: log_sys_use_load_cpst_value()
 �� ��: ʹ������ֵ����������ģʽ�жϡ��¿ط�Χ���Ƶ�
 �� ��: ��
 �� ��: ��ֱ��ʹ�õ�����ֵ
 �� ��: ��
*******************************************************************************/
I16 log_sys_use_load_cpst_value(U08 cpst_set)
{
    I16 cpst_load = 0;          /* ԭʼ����ֵ */
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
        /* �з�����ʹ�÷����жϣ��޷�����ʹ���㷨�ж� */
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
                /* ����ֵʹ�÷����� */
                if (ac_mode == MODE_COOL)     cpst_load = -cpst_load; /* ���������Ȳ��������෴ */

                /* ����ֵ�޷�����:Ŀ����ȷ������ʵˮ�¿������趨�¶������޷�Χ�� */
                cpst_add_max = sv_temp_energy - sv_min;           /* ���Ӳ��� */
                cpst_sub_max = sv_temp_energy - sv_max;           /* �������������� */
                break;
                
            case CPST_SV:
                /* ����ֵʹ�÷����� */
                if (ac_mode != MODE_COOL)     cpst_load = -cpst_load; /* ���������Ȳ��������෴ */

                /* ����ֵ�޷�����:Ŀ����ȷ������ʵˮ�¿������趨�¶������޷�Χ�� */
                cpst_add_max = sv_max - sv_temp_energy;           /* ���Ӳ��� */
                cpst_sub_max = sv_min - sv_temp_energy;           /* �������������� */
                break;
        }

        cpst_load = fun_pv_limit(cpst_load, cpst_sub_max, cpst_add_max);
    }    
    
    return cpst_load;
}
#endif

/*******************************************************************************
 �� ��: log_sys_limit()
 �� ��: ϵͳģ��������
 �� ��: ��
 �� ��: 
 �� ��: ��
*******************************************************************************/
static void log_sys_limit(void)
{
    /* �ڻ��̼�ʱǿ�Ƽ���⼸���¶� */
    if (!(TF_HAS_INNER&&TF_EN_NTC))  
    {
        // ��ʹ��ˮ���¶�
        if (val_hotw_used == UNUSED)
        {
            pv_sys_THWt = SENSOR_NOEXIST;
        }

        // ��ʹ�õ�ů��ˮ�¶�
        if (val_TFLin_sensor_used == UNUSED)
        {
            pv_sys_TFLin = SENSOR_NOEXIST;
        }

        // ��ʹ��ϵͳ�ܳ�ˮ�¶�
        if (val_TWout3_sensor_used == UNUSED)
        {
            pv_sys_TWout3 = SENSOR_NOEXIST;
        }
    }

    /* ��ʹ��̫�����¶� */
    if (!val_TSL_sensor_used)
    {
        pv_sys_TSL = SENSOR_NOEXIST;
    }

    /* ���û���ˮ���ϲ��¶�̽ͷ */
    if (!val_TACt1_sensor_used)
    {
        pv_sys_TACt1 = SENSOR_NOEXIST;
    }

    /* ����ƽ��ˮ���²��¶�̽ͷ */
    if (!val_TACt2_sensor_used)
    {
        pv_sys_TACt2 = SENSOR_NOEXIST;
    }
}

/*******************************************************************************
 �� ��: log_sys_get_TA()
 �� ��: ��ȡȥ�������С��ȡƽ���Ļ����¶�
 �� ��: ��
 �� ��: ȥ�������С��ȡƽ���Ļ����¶�
 �� ��: ��
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
 �� ��: log_sys_get_TWout2_avg()
 �� ��: ��ȡ����ģ���TWout2����ȡƽ��ֵ
 �� ��: ��
 �� ��: 
 �� ��: ��
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
 �� ��: log_sys_get_TWout2_ac()
 �� ��: �յ��ܳ�ˮ�¶ȼ��㣬�����в�������(������)ģ���TWout2ȡƽ��ֵ
 �� ��: ��
 �� ��: 
 �� ��: ��
*******************************************************************************/
static I16 log_sys_get_TWout2_ac(void)
{
    U08 pcb = 0;
    I16 temp[MAX_PCB];
    I16 temp_ac_TWout2 = SENSOR_NOEXIST;
    U08 sub_sys_type = SUB_SYS_TYPE_MAX;	/* Ĭ��: ����ģ�������(������) */

    /* ��ʼ�� */
    ARRSET(temp, SENSOR_NOEXIST);

    /* ������ϵͳ��Χ */
    if ((energy_ctrl_type == GROUP_ENERGY_CTRL)
    && fg_hotw_run
    )
    {
        /* ��ˮ����ʱ���ɿյ�+��ˮ��ϵͳ��ǰ�Ŀ���ģʽ����������(������)ģ��ľ��巶Χ */
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
 �� ��: log_sys_get_TWout1_avg()
 �� ��: ��ȡ���ú�ȡƽ���ĳ�ˮ�¶�
 �� ��: ��
 �� ��: ��ˮ�¶�
 �� ��: ��
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
 �� ��: log_sys_get_TWin1_avg()
 �� ��: ��ȡȥ�������С��ȡƽ���Ļ�ˮ�¶�
 �� ��: ��
 �� ��: ��ˮ�¶�
 �� ��: ��
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
 �� ��: log_sys_get_elec()
 �� ��: �����ģʽ�µ��ܹ���
 �� ��: ��
 �� ��: 
 �� ��: ��λ��0.01kw
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
        /* �ۼ�ģ�鹦��ǰ�������� */
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
 �� ��: log_sys_begin_reset()
 �� ��: ϵͳ��Դ���ݸ�λ
 �� ��: ��
 �� ��: 
 �� ��: ��
*******************************************************************************/
static void log_sys_begin_reset(void)
{
    U08 i;

    sys_input = 0;                          /* ����������ˢ��(���������պ��) */
    for (i=0; i<MAX_COMP+1; i++)
    {
        cell[MAX_PCB].cell_relay[i] = 0;    /* �̵����������� */
        cell[MAX_PCB].cell_input[i] = 0;    /* ����������ˢ��(������ԭʼֵ) */
    }
}

/*******************************************************************************
 �� ��: log_sys_begin_input()
 �� ��: ϵͳ����������ˢ��
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void log_sys_begin_input(void)
{
    /* ����̼�ʱ������ڻ������� */
    if (!TF_IS_OUT)
    {
        /* ������ԭʼֵ���� */
        /*-------����λ��--------; --------�߼�λ��-------*/  
        // ��������ǰ
//        if (GET_DI(DI_01))  m_set_input_first(M_IN_WATER_FLOW);	/* ˮ������ */
        if (GET_DI(DI_02))  m_set_input_first(IN_REMOTE);       /* �߿ؿ��� */
        if (GET_DI(DI_06))  m_set_input_first(IN_SG);           /* SG�ź� */
        if (GET_DI(DI_07))  m_set_input_first(IN_EVU);          /* EVU�ź� */
        
        if (GET_DI(DI_09))  m_set_input_first(M_DI_TBH_OL);     /* ˮ����ȷ��� */
        if (GET_DI(DI_10))  m_set_input_first(IN_RT_HEAT);      /* �¿��������ź� */
        if (GET_DI(DI_11))  m_set_input_first(IN_RT_COOL);      /* �¿��������ź� */
        if (GET_DI(DI_12))  m_set_input_first(IN_SOLAR);        /* ̫�����ź� */

        // �������պ�
//        m_set_di(M_IN_WATER_FLOW, val_DI_type_air_flux);		/* ˮ������ */
        m_set_di(IN_REMOTE, NORMAL_OPEN);                       /* �߿ؿ���(�̶�����) */
        m_set_di(IN_SG, val_DI_type_SG);                        /* SG�ź� */
        m_set_di(IN_EVU, val_DI_type_EVU);                      /* EVU�ź� */
        m_set_di(M_DI_TBH_OL, NORMAL_CLOSE);		                /* ˮ����ȷ���(�̶�����) */
        m_set_di(IN_RT_HEAT, NORMAL_OPEN);                      /* �¿��������ź�(�̶�����) */
        m_set_di(IN_RT_COOL, NORMAL_OPEN);                      /* �¿��������ź�(�̶�����) */
        m_set_di(IN_SOLAR, NORMAL_OPEN);                        /* ̫�����ź�(�̶�����) */
    }
}

/*******************************************************************************
 �� ��: log_sys_exio_begin_pv()
 �� ��: IO��չ��ϵͳģ��������
 �� ��: ��
 �� ��: 
 �� ��: ��
*******************************************************************************/
static void log_sys_exio_begin_pv(void)
{
    pv_sys_TA  = log_sys_get_TA();      /* ϵͳ���� */
    if (USE_GB_DBG)
    {
        pv_sys_TA  = fun_pv_cmpst(pv_sys_TA, val_drytemp_cpst);    // �����¶�
    }
    // �����¶��ȶ�3S���ٸ���ʹ�á�
    // ��ֹ̽ͷ����ʱǿ���л���0��ʱ����Ӱ���Զ������ȵļ��㣨���ܻ�Ӱ���趨ֵ���ƶ����ݣ�
    if(pv_sys_TA!=pv_sys_environment_tmp)  // �����¶�����
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
 �� ��: log_sys_begin_pv()
 �� ��: ϵͳģ��������ˢ��
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void log_sys_begin_pv(void)
{
    /* ģ����ˢ�� */
    pv_sys_TR               = room_temp;                /* �����¶� */

#ifdef COM_FCU_FBUS
    fcu_rd_update();
#endif

//    pv_sys_TWout2           = log_sys_get_TWout2_avg();		/* �ܳ�ˮ�¶� */
    pv_sys_TWout2_ac        = log_sys_get_TWout2_ac();		/* �յ��ܳ�ˮ�¶� */

    pv_sys_TACt1            = GET_TEMP(TEMP_03);			/* ����ˮ���ϲ��¶� */
    pv_sys_TACt2            = GET_TEMP(TEMP_02);			/* ����ˮ���²��¶� */
    pv_sys_THWt             = GET_TEMP(TEMP_10);            /* ��ˮˮ���¶� */
    pv_sys_TSL              = GET_TEMP(TEMP_04);            /* ̫�����¶� */
    pv_sys_TWout3           = GET_TEMP(TEMP_11);            /* ϵͳ�ܳ�ˮ�¶� */
    pv_sys_TFLin            = GET_TEMP(TEMP_12);            /* ��ů��ˮ�¶� */

    pv_sys_S3V1_state       = msg_sta_S3V1;	                /* �����ͨ��1״̬(�ַ�����) */
    pv_sys_S3V2_state       = msg_sta_S3V2;	                /* �����ͨ��2״̬(�ַ�����) */
    pv_sys_S3V3_state       = msg_sta_S3V3;	                /* �����ͨ��3״̬(�ַ�����) */

    log_sys_get_elec();
    
    log_sys_limit();
}

/*******************************************************************************
 �� ��: log_sys_begin_init()
 �� ��: ϵͳ��ʼ������
 �� ��: ��
 �� ��: 
 �� ��: ��
*******************************************************************************/
static void log_sys_begin_init(void)
{
    if (STEP_IS_IDLE || STEP_IS_ALARM)      /* ����ʱ�ſɳ�ʼ�� */
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
 �� ��: log_sys_begin()
 �� ��: ϵͳ��ʼ������:ϵͳ��Դ���ݸ�λ
                       ϵͳ����������ˢ��
                       ϵͳģ��������ˢ��
                       ϵͳ��ʼ������
 �� ��: ��
 �� ��: 
 �� ��: ��
*******************************************************************************/
static void log_sys_begin(void)
{
    log_sys_begin_reset();
    log_sys_begin_input();
    log_sys_begin_pv();
    log_sys_exio_begin_pv();
    log_sys_begin_init();
    
    /* �������ڷ�ʽȷ�� */
    if (IS_DUAL_SYS
    && val_hotw_used		                /* ��ˮ����ʹ�� */
//    && fg_hotw_run
    )
    {
        energy_ctrl_type = GROUP_ENERGY_CTRL;				/* �����ܵ� */
    }
    else
    {
        energy_ctrl_type = JOINT_ENERGY_CTRL;				/* Ĭ��: �����ܵ� */
    }

    /* �յ�+��ˮ��ϵͳ�ܵ������¶�����ѹ������ */
    fg_TA_limit_comp_open[SUB_SYS_AC_HOTW] = log_sys_TA_limit_comp_is_off(ctrl_mode[SUB_SYS_AC_HOTW], SUB_SYS_AC_HOTW);

    /* ���յ���ϵͳ�ܵ������¶�����ѹ������ */
    fg_TA_limit_comp_open[SUB_SYS_AC_ONLY] = log_sys_TA_limit_comp_is_off(ctrl_mode[SUB_SYS_AC_ONLY], SUB_SYS_AC_ONLY);

    /* ̫�����ź�����ѹ�������� */
    fg_HDIsl_limit_comp_open = log_sys_HDIsl_limit_comp_is_off();

    /* ̫�����¶�����ѹ�������� */
    fg_TSL_limit_comp_open = log_sys_TSL_limit_comp_is_off();

    /* ��ůԤ������ѹ�������� */
    fg_floor_preheat_limit_comp_open = log_sys_floor_preheat_limit_comp_is_off();
}

/***********************************************
������Logic_OutputChange
���ܣ��������ת���������״̬���������������
������ģ�����
����ֵ����
*************************************************/
void log_sys_end(void)
{
    SHC_set_sys_test_do();
       
    /* ��˪ָʾ DFR */
    if (get_output(MAX_PCB, M_DO_DFR))               
    {
        SET_DO(DO_16);
    }
    /* ����ָʾ ERR */
    if (get_output(MAX_PCB, M_DO_ERR))               
    {
        SET_DO(DO_17);
    }
    /* �ⲿ��Դ AHS */
    if (get_output(MAX_PCB, M_DO_AHS))               
    {
        SET_DO(DO_18);
    }
    /* �����ͨ��1 S3V1 */
    if (get_output(MAX_PCB, M_DO_S3V1))
    {
        SET_DO(DO_02);
        msg_sta_S3V1 = msg_S3V_state_no;
    }
    else
    {
        msg_sta_S3V1 = msg_S3V_state_nc;
    }
    /* �����ͨ��3 S3V3 */
    if (get_output(MAX_PCB, M_DO_S3V3))              
    {
        SET_DO(DO_03);
        msg_sta_S3V3 = msg_S3V_state_no;
    }
    else
    {
        msg_sta_S3V3 = msg_S3V_state_nc;
    }
    /* �����ͨ��2 S3V2 */
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
        /* ˮ������ TBH */
        if (get_output(MAX_PCB, M_DO_TBH))                            
        {
            SET_DO(DO_07);
        }
    }

    /* �»�ˮ�� PUMPret */
    if (get_output(MAX_PCB, M_DO_PUMPret))           
    {
        SET_DO(DO_09);
    }
    /* ̫����ˮ�� PUMPsl */
    if (get_output(MAX_PCB, M_DO_PUMPsl))
    {
        SET_DO(DO_10);
    }
    /* ���ˮ�� PUMPm */
    if (get_output(MAX_PCB, M_DO_PUMPm))
    {
        SET_DO(DO_11);
    }
    /* ��ѭ���� PUMPo */
    if (get_output(MAX_PCB, M_DO_PUMPo))
    {
        SET_DO(DO_12);
    }
//    /* ��Ƶˮ�� PUMPf */
//    if (get_output(MAX_PCB, M_DO_PUMPf))             SET_DO(DO_01);
    
}

/*******************************************************************************
 �� ��: log_sys_get_ac_add_diff()
 �� ��: ����յ��ܵ�����ƫ��
 �� ��: mode--; 
 �� ��: 
 �� ��: ��
*******************************************************************************/
I16 log_sys_get_ac_add_diff(U08 mode)
{
    I16 add_diff;
    BOL fg_outlet_water_ctrl = TRUE;		/* �س�ˮ��־ */

    /* ���¿��� */
    if (ONE_ZONE_ROOM_TEMP_CTRL
    && !fg_sv_temp_energy_overwrite_ac           /* ���¶ȸ���ʱ */
    )
    {
        return val_TR_diff;
    }

    if (USE_AC_TEMP_IN_CTRL)
    {
        /* ʹ�û�ˮ���� */
        fg_outlet_water_ctrl = FALSE;
    }

    /* ��ȡԭʼ����ƫ�� */
    if (mode == MODE_COOL)
    {
        add_diff = val_cool_add_diff;
    }
    else
    {
        add_diff = val_heat_add_diff;
    }

    /* �س�ˮ */
    if (fg_outlet_water_ctrl)
    {
//        add_diff += INOUT_10DIFF*(Cmax-Cnow)/Cmax;
        add_diff += INOUT_10DIFF;			/* ��Է�������ȹ̶�Ϊ5�� */
    }
    
    return add_diff;
}

/*******************************************************************************
 �� ��: log_sys_get_ac_sub_diff()
 �� ��: ����յ��ܵ�ж��ƫ��
 �� ��: mode--; 
 �� ��: 
 �� ��: ��
*******************************************************************************/
I16 log_sys_get_ac_sub_diff(U08 mode)
{
    if (ONE_ZONE_ROOM_TEMP_CTRL
    && !fg_sv_temp_energy_overwrite_ac           /* ���¶ȸ���ʱ */
    )
    {
        return 0;
    }
    
    return (mode == MODE_COOL) ? val_cool_sub_diff : val_heat_sub_diff;
}

/*******************************************************************************
 �� ��: log_sys_get_sv_temp_energy_cool()
 �� ��: 
 �� ��: ��
 �� ��: 
 �� ��: ��
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

    /* ���������� */
    if (GETBIT_N(fg_test_run, TEST_RUN_COOL_MODE))
    {
        fg_sv_temp_energy_overwrite_cool = TRUE;
        sv_temp = temp_sv_disp_zone_A_cool = temp_sv_disp_zone_B_cool = TEST_RUN_COOL_MODE_SV_COOL;
        return sv_temp;
    }
    
    /* �շ� */
    if (fg_refri_recy_act)
    {
        fg_sv_temp_energy_overwrite_cool = TRUE;
        sv_temp = temp_sv_disp_zone_A_cool = temp_sv_disp_zone_B_cool = 50;
        return sv_temp;
    }

    /* �������� */
    if (fg_ability_test_run)
    {
#ifndef USE_AT_SV_FIX
        temp_set_can_edit_zone_A_cool = TRUE;
#endif
        fg_sv_temp_energy_overwrite_cool = TRUE;
        sv_temp = temp_sv_disp_zone_A_cool = temp_sv_disp_zone_B_cool = EXIT_ABILITY_TEST_COOL_SV;
        return sv_temp;
    }

    /* ����Ԥ��Ŀ���¶�/Ԥ��Ŀ���¶�(B) */
    if (!fg_holiday_away_run    /* �Ǽ������ģʽ */
    && !fg_holiday_home_run	    /* �Ǽ����ڼ�ģʽ */
    )
    {
        sv_temp_zone_A_preset = WC_get_weather_curve_sv_temp(pv_sys_TA, val_zone_A_cool_weather_curve, MODE_COOL);
        sv_temp_zone_B_preset = WC_get_weather_curve_sv_temp(pv_sys_TA, val_zone_B_cool_weather_curve, MODE_COOL);
    }

    /* ���������ܵ� */
    if (ONE_ZONE_ROOM_TEMP_CTRL)
    {      
        temp_ctrl_type_zone_A_cool = ROOM_TEMP_CTRL;		/* ���¿��� */
        temp_set_can_edit_zone_A_cool = TRUE;
        temp_sv_disp_zone_A_cool = sv_room_cool;		/*�������趨���¡�*/
    }

    /* ����ˮ���ܵ� */
    if (ONE_ZONE_WATER_TEMP_CTRL)
    {
        temp_ctrl_type_zone_A_cool = WATER_TEMP_CTRL;		/* ˮ�¿��� */
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

    /* ˫��ˮ���ܵ� */
    if (DUAL_ZONE_WATER_TEMP_CTRL)
    {
        /* A�� */
        temp_ctrl_type_zone_A_cool = WATER_TEMP_CTRL;		/* ˮ�¿��� */
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

        /* B�� */
        temp_ctrl_type_zone_B_cool = WATER_TEMP_CTRL;		/* ˮ�¿��� */
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

    /* ˫������ܵ� */
    if (DUAL_ZONE_HYBRID_CTRL)
    {
        /* A�� */
        temp_ctrl_type_zone_A_cool = WATER_TEMP_CTRL;		/* ˮ�¿��� */
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
   
        /* B�� */
        temp_ctrl_type_zone_B_cool = ROOM_TEMP_CTRL;		/* ���¿��� */
        if (SensorNoExist(sv_temp_zone_B_preset))
        {
            temp_set_can_edit_zone_B_cool = TRUE;
            temp_sv_disp_zone_B_cool = sv_room_cool_zone_B;
            sv_temp_zone_B_preset = WC_get_weather_curve_sv_temp(pv_sys_TA, TEMP_CURVE_LO_4, MODE_COOL);/* ������Ϊ����ʱ��ʹ��Ĭ������ */
        }
        else
        {
            temp_set_can_edit_zone_B_cool = FALSE;
            temp_sv_disp_zone_B_cool = sv_temp_zone_B_preset;
        }
    }
    
    /* �߿���˫����Ͽ��� */
    if (USE_UI35_DUAL_ZONE_HYBRID_CTRL)
    {
        if (!fg_zone_B_run)                         /* B��δ���� */
        {
            sv_temp = temp_sv_disp_zone_A_cool;
        }
        else if (fg_zone_B_run && !fg_zone_A_run)   /* B�����У���A��δ���� */
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
        if (!fg_zone_B_run)                         /* B��δ���� */
        {
            sv_temp = temp_sv_disp_zone_A_cool;
        }
        else if (fg_zone_B_run && !fg_zone_A_run)   /* B�����У���A��δ���� */
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
 �� ��: log_sys_get_sv_temp_energy_heat()
 �� ��: ��ȡ�����ܵ�Ŀ���¶�
 �� ��: ��
 �� ��: 
 �� ��: ��
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
    
    /* ���������� */
    if (GETBIT_N(fg_test_run, TEST_RUN_HEAT_MODE))
    {
        fg_sv_temp_energy_overwrite_heat = TRUE;
        sv_temp = temp_sv_disp_zone_A_heat = temp_sv_disp_zone_B_heat = TEST_RUN_HEAT_MODE_SV_HEAT;
        return sv_temp;
    }

    /* �������� */
    if (fg_ability_test_run)
    {
#ifndef USE_AT_SV_FIX
        temp_set_can_edit_zone_A_heat = TRUE;
#endif
        fg_sv_temp_energy_overwrite_heat = TRUE;
        sv_temp = temp_sv_disp_zone_A_heat = temp_sv_disp_zone_B_heat = EXIT_ABILITY_TEST_HEAT_SV;
        return sv_temp;
    }

    /* �����뿪ģʽ */
    if (fg_holiday_away_run
    && fg_holiday_away_heat_mode_used
    )
    {
        fg_sv_temp_energy_overwrite_heat = TRUE;
        sv_temp = temp_sv_disp_zone_A_heat = temp_sv_disp_zone_B_heat = val_holiday_away_sv_heat;
        return sv_temp;
    }

    /* ��ů��ɹ��� */
    if (fg_floor_dryup_run)
    {
        fg_sv_temp_energy_overwrite_heat = TRUE;
        sv_temp = temp_sv_disp_zone_A_heat = temp_sv_disp_zone_B_heat = dryup_sv_temp;
        return sv_temp;
    }

    /* ��ůԤ�ȹ��� */
    if (fg_floor_preheat_run)
    {
        fg_sv_temp_energy_overwrite_heat = TRUE;
        sv_temp = temp_sv_disp_zone_A_heat = temp_sv_disp_zone_B_heat = val_floor_preheat_temp;
        return sv_temp;
    }

    /* ����Ԥ��Ŀ���¶�/Ԥ��Ŀ���¶�(B) */
    if (!fg_holiday_away_run    /* �Ǽ������ģʽ */
    && !fg_holiday_home_run	    /* �Ǽ����ڼ�ģʽ */
    )
    {
        sv_temp_zone_A_preset = WC_get_weather_curve_sv_temp(pv_sys_TA, val_zone_A_heat_weather_curve, MODE_HEAT);
        sv_temp_zone_B_preset = WC_get_weather_curve_sv_temp(pv_sys_TA, val_zone_B_heat_weather_curve, MODE_HEAT);
    }

    /* ���������ܵ� */
    if (ONE_ZONE_ROOM_TEMP_CTRL)
    {
        temp_ctrl_type_zone_A_heat = ROOM_TEMP_CTRL;		/* ���¿��� */
        temp_set_can_edit_zone_A_heat = TRUE;
        temp_sv_disp_zone_A_heat = sv_room_heat;		/*�������趨���¡�*/
    }

    /* ����ˮ���ܵ� */
    if (ONE_ZONE_WATER_TEMP_CTRL)
    {
        temp_ctrl_type_zone_A_heat = WATER_TEMP_CTRL;		/* ˮ�¿��� */
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

    /* ˫��ˮ���ܵ� */
    if (DUAL_ZONE_WATER_TEMP_CTRL)
    {
        /* A�� */
        temp_ctrl_type_zone_A_heat = WATER_TEMP_CTRL;		/* ˮ�¿��� */
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

        /* B�� */
        temp_ctrl_type_zone_B_heat = WATER_TEMP_CTRL;		/* ˮ�¿��� */
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

    /* ˫������ܵ� */
    if (DUAL_ZONE_HYBRID_CTRL)
    {
        /* A�� */
        temp_ctrl_type_zone_A_heat = WATER_TEMP_CTRL;		/* ˮ�¿��� */
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

        /* B�� */
        temp_ctrl_type_zone_B_heat = ROOM_TEMP_CTRL;		/* ���¿��� */
        if (SensorNoExist(sv_temp_zone_B_preset))
        {
            temp_set_can_edit_zone_B_heat = TRUE;
            temp_sv_disp_zone_B_heat = sv_room_heat_zone_B; 
            sv_temp_zone_B_preset = WC_get_weather_curve_sv_temp(pv_sys_TA, TEMP_CURVE_LO_4, MODE_HEAT);/* ������Ϊ����ʱ��ʹ��Ĭ������ */
        }
        else
        {
            temp_set_can_edit_zone_B_heat = FALSE;
            temp_sv_disp_zone_B_heat = sv_temp_zone_B_preset;
        }
    }

    /* �߿���˫����Ͽ��� */
    if (USE_UI35_DUAL_ZONE_HYBRID_CTRL)
    {
        if (!fg_zone_B_run)                         /* B��δ���� */
        {
            sv_temp = temp_sv_disp_zone_A_heat;
        }
        else if (fg_zone_B_run && !fg_zone_A_run)   /* B�����У���A��δ���� */
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
        if (!fg_zone_B_run)                         /* B��δ���� */
        {
            sv_temp = temp_sv_disp_zone_A_heat;
        }
        else if (fg_zone_B_run && !fg_zone_A_run)   /* B�����У���A��δ���� */
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
 �� ��: log_sys_get_sv_disp_zone_A()
 �� ��: ��ȡA���趨�¶�����ʾ
 �� ��: grp--;
		idx--;
		value--;
 �� ��: ��
 �� ��: ���ǹ̶�ֵ���дvalue�����ǲ���ֵ���дgrp��idx
*******************************************************************************/
void log_sys_get_sv_disp_zone_A(U08 *grp, U08 *idx, I16 *value)
{
    I16 sv_temp_zone_A_preset = SENSOR_NOEXIST;

	if (ac_mode == MODE_COOL)
	{
		/* ���������� */
	    if (GETBIT_N(fg_test_run, TEST_RUN_COOL_MODE))
	    {
	    	/* �̶�ֵ */
	        *value = TEST_RUN_COOL_MODE_SV_COOL;
	        return;
	    }
	    /* �շ� */
	    if (fg_refri_recy_act)
	    {
	    	/* �̶�ֵ */
	        *value = 50;
	        return;
	    }

	    /* �������� */
	    if (fg_ability_test_run)
	    {
	    	/* �̶�ֵ */
	        *value = EXIT_ABILITY_TEST_COOL_SV;
	        return;
	    }
	    /* ���������ܵ� */
	    if (ONE_ZONE_ROOM_TEMP_CTRL)
	    {
	    	/* ����ֵ */
	    	*grp = P_grp(sv_room_cool);
	    	*idx = P_idx(sv_room_cool);
	        return;
	    }

	    /* ����Ԥ��Ŀ���¶�/Ԥ��Ŀ���¶�(B) */
	    if (!fg_holiday_away_run    /* �Ǽ������ģʽ */
	    && !fg_holiday_home_run	    /* �Ǽ����ڼ�ģʽ */
	    )
	    {
	        sv_temp_zone_A_preset = WC_get_weather_curve_sv_temp(pv_sys_TA, val_zone_A_cool_weather_curve, MODE_COOL);
	    }

	    /* ����ˮ���ܵ� */
	    if (ONE_ZONE_WATER_TEMP_CTRL)
	    {
	        if (SensorNoExist(sv_temp_zone_A_preset))
	        {
		    	/* ����ֵ */
		    	*grp = P_grp(sv_cool);
		    	*idx = P_idx(sv_cool);
		        return;
	        }
	        else
	        {
		    	/* �̶�ֵ */
	            *value = sv_temp_zone_A_preset;
		        return;
	        }
	    }

	    /* ˫��ˮ���ܵ� */
	    if (DUAL_ZONE_WATER_TEMP_CTRL)
	    {
	        if (SensorNoExist(sv_temp_zone_A_preset))
	        {
		    	/* ����ֵ */
		    	*grp = P_grp(sv_cool);
		    	*idx = P_idx(sv_cool);
		        return;
	        }
	        else
	        {
		    	/* �̶�ֵ */
	            *value = sv_temp_zone_A_preset;
		        return;
	        }
	    }

	    /* ˫������ܵ� */
	    if (DUAL_ZONE_HYBRID_CTRL)
	    {
	        /* A�� */
	        if (SensorNoExist(sv_temp_zone_A_preset))
	        {
		    	/* ����ֵ */
		    	*grp = P_grp(sv_cool);
		    	*idx = P_idx(sv_cool);
		        return;
	        }
	        else
	        {
		    	/* �̶�ֵ */
	            *value  = sv_temp_zone_A_preset;
		        return;
	        }
	    }
	}
    else
    {
	    /* ���������� */
	    if (GETBIT_N(fg_test_run, TEST_RUN_HEAT_MODE))
	    {
	        /* �̶�ֵ */
	        *value = TEST_RUN_HEAT_MODE_SV_HEAT;
	        return;
	    }

	    /* �������� */
	    if (fg_ability_test_run)
	    {
	        /* �̶�ֵ */
	        *value = EXIT_ABILITY_TEST_HEAT_SV;
	        return;
	    }

	    /* �����뿪ģʽ */
	    if (fg_holiday_away_run
	    && fg_holiday_away_heat_mode_used
	    )
	    {
	        /* �̶�ֵ */
	        *value = val_holiday_away_sv_heat;
	        return;
	    }

	    /* ��ů��ɹ��� */
	    if (fg_floor_dryup_run)
	    {
	        /* �̶�ֵ */
	        *value = dryup_sv_temp;
	        return;
	    }

	    /* ��ůԤ�ȹ��� */
	    if (fg_floor_preheat_run)
	    {
	        /* �̶�ֵ */
	        *value = val_floor_preheat_temp;
	        return;
	    }

	    /* ����Ԥ��Ŀ���¶�/Ԥ��Ŀ���¶�(B) */
	    if (!fg_holiday_away_run    /* �Ǽ������ģʽ */
	    && !fg_holiday_home_run	    /* �Ǽ����ڼ�ģʽ */
	    )
	    {
	        sv_temp_zone_A_preset = WC_get_weather_curve_sv_temp(pv_sys_TA, val_zone_A_heat_weather_curve, MODE_HEAT);
	    }

	    /* ���������ܵ� */
	    if (ONE_ZONE_ROOM_TEMP_CTRL)
	    {
	        /* ����ֵ */
	        *grp = P_grp(sv_room_heat);
	        *idx = P_idx(sv_room_heat);
	        return;
	    }

	    /* ����ˮ���ܵ� */
	    if (ONE_ZONE_WATER_TEMP_CTRL)
	    {
	        if (SensorNoExist(sv_temp_zone_A_preset))
	        {
		        /* ����ֵ */
		        *grp = P_grp(sv_heat);
		        *idx = P_idx(sv_heat);
		        return;
	        }
	        else
	        {
	            /* �̶�ֵ */
	            *value = sv_temp_zone_A_preset;
		        return;
	        }
	    }

	    /* ˫��ˮ���ܵ� */
	    if (DUAL_ZONE_WATER_TEMP_CTRL)
	    {
	        /* A�� */
	        if (SensorNoExist(sv_temp_zone_A_preset))
	        {
	            /* ����ֵ */
	            *grp = P_grp(sv_heat);
	            *idx = P_idx(sv_heat);
		        return;
	        }
	        else
	        {
	        	/* �̶�ֵ */
	            *value = sv_temp_zone_A_preset;
		        return;
	        }
	    }

	    /* ˫������ܵ� */
	    if (DUAL_ZONE_HYBRID_CTRL)
	    {
	        /* A�� */
	        if (SensorNoExist(sv_temp_zone_A_preset))
	        {
	            /* ����ֵ */
	            *grp = P_grp(sv_heat);
	            *idx = P_idx(sv_heat);
		        return;
	        }
	        else
	        {
	            /* �̶�ֵ */
	            *value = sv_temp_zone_A_preset;
		        return;
	        }
	    }
    }
}

/*******************************************************************************
 �� ��: log_sys_get_sv_disp_zone_B()
 �� ��: ��ȡB���趨�¶�����ʾ
 �� ��: grp--;
		idx--;
		value--;
 �� ��: ��
 �� ��: ���ǹ̶�ֵ���дvalue�����ǲ���ֵ���дgrp��idx
*******************************************************************************/
void log_sys_get_sv_disp_zone_B(U08 *grp, U08 *idx, I16 *value)
{
    I16 sv_temp_zone_B_preset = SENSOR_NOEXIST;

	if (ac_mode == MODE_COOL)
	{
		/* ���������� */
	    if (GETBIT_N(fg_test_run, TEST_RUN_COOL_MODE))
	    {
	    	/* �̶�ֵ */
	        *value = TEST_RUN_COOL_MODE_SV_COOL;
	        return;
	    }
	    /* �շ� */
	    if (fg_refri_recy_act)
	    {
	    	/* �̶�ֵ */
	        *value = 50;
	        return;
	    }

	    /* �������� */
	    if (fg_ability_test_run)
	    {
	    	/* �̶�ֵ */
	        *value = EXIT_ABILITY_TEST_COOL_SV;
	        return;
	    }

	    /* ����Ԥ��Ŀ���¶�/Ԥ��Ŀ���¶�(B) */
	    if (!fg_holiday_away_run    /* �Ǽ������ģʽ */
	    && !fg_holiday_home_run	    /* �Ǽ����ڼ�ģʽ */
	    )
	    {
	        sv_temp_zone_B_preset = WC_get_weather_curve_sv_temp(pv_sys_TA, val_zone_B_cool_weather_curve, MODE_COOL);
	    }
	    /* ˫��ˮ���ܵ� */
	    if (DUAL_ZONE_WATER_TEMP_CTRL)
	    {
	        if (SensorNoExist(sv_temp_zone_B_preset))
	        {
		    	/* ����ֵ */
		    	*grp = P_grp(sv_cool_zone_B);
		    	*idx = P_idx(sv_cool_zone_B);
		        return;
	        }
	        else
	        {
		    	/* �̶�ֵ */
	            *value = sv_temp_zone_B_preset;
		        return;
	        }
	    }

	    /* ˫������ܵ� */
	    if (DUAL_ZONE_HYBRID_CTRL)
	    {
	        /* B�� */
	        if (SensorNoExist(sv_temp_zone_B_preset))
	        {
		    	/* ����ֵ */
		    	*grp = P_grp(sv_room_cool_zone_B);
		    	*idx = P_idx(sv_room_cool_zone_B);
		        return;
	        }
	        else
	        {
		    	/* �̶�ֵ */
	            *value  = sv_temp_zone_B_preset;
		        return;
	        }
	    }
	}
    else
    {
	    /* ���������� */
	    if (GETBIT_N(fg_test_run, TEST_RUN_HEAT_MODE))
	    {
	        /* �̶�ֵ */
	        *value = TEST_RUN_HEAT_MODE_SV_HEAT;
	        return;
	    }

	    /* �������� */
	    if (fg_ability_test_run)
	    {
	        /* �̶�ֵ */
	        *value = EXIT_ABILITY_TEST_HEAT_SV;
	        return;
	    }

	    /* �����뿪ģʽ */
	    if (fg_holiday_away_run
	    && fg_holiday_away_heat_mode_used
	    )
	    {
	        /* �̶�ֵ */
	        *value = val_holiday_away_sv_heat;
	        return;
	    }

	    /* ��ů��ɹ��� */
	    if (fg_floor_dryup_run)
	    {
	        /* �̶�ֵ */
	        *value = dryup_sv_temp;
	        return;
	    }

	    /* ��ůԤ�ȹ��� */
	    if (fg_floor_preheat_run)
	    {
	        /* �̶�ֵ */
	        *value = val_floor_preheat_temp;
	        return;
	    }

	    /* ����Ԥ��Ŀ���¶�/Ԥ��Ŀ���¶�(B) */
	    if (!fg_holiday_away_run    /* �Ǽ������ģʽ */
	    && !fg_holiday_home_run	    /* �Ǽ����ڼ�ģʽ */
	    )
	    {
	        sv_temp_zone_B_preset = WC_get_weather_curve_sv_temp(pv_sys_TA, val_zone_B_heat_weather_curve, MODE_HEAT);
	    }

	    /* ˫��ˮ���ܵ� */
	    if (DUAL_ZONE_WATER_TEMP_CTRL)
	    {
	        /* B�� */
	        if (SensorNoExist(sv_temp_zone_B_preset))
	        {
	            /* ����ֵ */
	            *grp = P_grp(sv_heat_zone_B);
	            *idx = P_idx(sv_heat_zone_B);
		        return;
	        }
	        else
	        {
	        	/* �̶�ֵ */
	            *value = sv_temp_zone_B_preset;
		        return;
	        }
	    }

	    /* ˫������ܵ� */
	    if (DUAL_ZONE_HYBRID_CTRL)
	    {
	        /* B�� */
	        if (SensorNoExist(sv_temp_zone_B_preset))
	        {
	            /* ����ֵ */
	            *grp = P_grp(sv_room_heat_zone_B);
	            *idx = P_idx(sv_room_heat_zone_B);
		        return;
	        }
	        else
	        {
	            /* �̶�ֵ */
	            *value = sv_temp_zone_B_preset;
		        return;
	        }
	    }
    }
}

/*******************************************************************************
 �� ��: log_sys_get_sv_disp_zone_hot()
 �� ��: ��ȡ��ˮ���趨�¶�����ʾ
 �� ��: grp--;
		idx--;
		value--;
 �� ��: ��
 �� ��: ���ǹ̶�ֵ���дvalue�����ǲ���ֵ���дgrp��idx
*******************************************************************************/
void log_sys_get_sv_disp_zone_hot(U08 *grp, U08 *idx, I16 *value)
{
    /* ����ˮ������ */
    if (GETBIT_N(fg_test_run, TEST_RUN_HOTW_MODE))
    {
        /* �̶�ֵ */
        *value = TEST_RUN_HOTW_MODE_SV_HOTW;
        return;
    }

    /* ��ʱɱ������ */
    if (fg_timed_disinfect_run)
    {
        /* ����ֵ */
        *grp = P_grp(sv_hotw);
        *idx = P_idx(sv_hotw);
        return;
    }

    /* �����뿪ģʽ */
    if (fg_holiday_away_run
    && fg_holiday_away_hotw_mode_used
    )
    {
        /* �̶�ֵ */
        *value = val_holiday_away_sv_hotw;
        return;
    }

    /* ����õ�ǿ������ */
    if (FREE_ELEC_FORCE_RUN)
    {
        /* �̶�ֵ */
        *value = FREE_ELEC_I_SV_HOTW;
        return;
    }

    /* ����ֵ */
    *grp = P_grp(sv_hotw);
    *idx = P_idx(sv_hotw);

    return;
}

/*******************************************************************************
 �� ��: log_sys_get_ctrl_temp_energy_ac()
 �� ��: ��ȡ�յ��ܵ������¶�
 �� ��: ��
 �� ��: 
 �� ��: ��
*******************************************************************************/
I16 log_sys_get_ctrl_temp_energy_ac(void)
{
    /* �ض����ܣ�Ĭ��ʹ��TWout2��Ϊ�ܵ������¶� */
    if (fg_sv_temp_energy_overwrite_ac)
    {
        ctrl_temp_type_energy_ac = AC_NRG_CTRL_TEMP_AC_TWOUT2;
        return AC_NRG_CTRL_TEMP;                            /* �յ��ܳ�ˮ�¶� */
    }

    /* �������� */
    if (ONE_ZONE_ROOM_TEMP_CTRL)
    {
        ctrl_temp_type_energy_ac = AC_NRG_CTRL_TEMP_TR;
        return pv_sys_TR;	                                /* �����¶�TR */
    }
    else if (USE_AC_TEMP_TACT1)			                
    {
        ctrl_temp_type_energy_ac = AC_NRG_CTRL_TEMP_TACT1;
        return pv_sys_TACt1;                                /* ����ˮ���ϲ��¶�TACt1 */
    }
    else if (USE_AC_TEMP_TACT2)		                
    {
        ctrl_temp_type_energy_ac = AC_NRG_CTRL_TEMP_TACT2;
        return pv_sys_TACt2;                                /* ����ˮ���²��¶�TACt2 */
    }
    else if (USE_AC_TEMP_TACT1_TACT2)				
    {
        ctrl_temp_type_energy_ac = AC_NRG_CTRL_TEMP_TACT1_TACT2;
        return fun_pv_avg(pv_sys_TACt1, pv_sys_TACt2);		/* ����ˮ��ƽ���¶�AVG(TACt1,TACt2) */
    }
    else                                    
    {
        ctrl_temp_type_energy_ac = AC_NRG_CTRL_TEMP_AC_TWOUT2;
        return pv_sys_TWout2_ac;                            /* �յ��ܳ�ˮ�¶� */
    }
}

/*******************************************************************************
 �� ��: log_sys_get_sv_temp_energy_hotw()
 �� ��: ��ȡ��ˮ�ܵ�Ŀ���¶�
 �� ��: ��
 �� ��: ��ˮ�ܵ�Ŀ���¶�
 �� ��: ��
*******************************************************************************/
I16 log_sys_get_sv_temp_energy_hotw(void)
{
    fg_sv_temp_energy_overwrite_hotw = FALSE;
    
    /* ����ˮ������ */
    if (GETBIT_N(fg_test_run, TEST_RUN_HOTW_MODE))
    {
        fg_sv_temp_energy_overwrite_hotw = TRUE;
        return TEST_RUN_HOTW_MODE_SV_HOTW;
    }

    /* ��ʱɱ������ */
    if (fg_timed_disinfect_run)
    {
        return sv_hotw;
    }

    /* �����뿪ģʽ */
    if (fg_holiday_away_run
    && fg_holiday_away_hotw_mode_used
    )
    {
        fg_sv_temp_energy_overwrite_hotw = TRUE;
        return val_holiday_away_sv_hotw;
    }

    /* ����õ�ǿ������ */
    if (FREE_ELEC_FORCE_RUN)
    {
        fg_sv_temp_energy_overwrite_hotw = TRUE;
        return FREE_ELEC_I_SV_HOTW;
    }

    return sv_hotw;
}

/*******************************************************************************
 �� ��: log_sys_get_sv_temp_freq_ac()
 �� ��: ��ȡ����/���ȵ�ƵĿ���¶�
 �� ��: mode--; 
 �� ��: 
 �� ��: ��
*******************************************************************************/
I16 log_sys_get_sv_temp_freq_ac(U08 mode)
{
    U08 curve_id = TEMP_CURVE_NULL;

    /* �����ƵĿ���¶� */
    if (temp_ctrl_type_zone_A == ROOM_TEMP_CTRL)
    {
        curve_id = GET_ZONE_A_WEATHER_CURVE(mode);             
        /* ������Ϊ����ʱ��ʹ��Ĭ������ */
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
 �� ��: log_sys_mode_temp_confirm()
 �� ��: ��ģʽ���ܵ�Ŀ���¶ȡ���ƵĿ���¶ȡ��ܵ������¶�ȷ��
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_mode_temp_confirm(void)
{
    /* �ܵ�Ŀ���¶� */
    sv_temp_energy_cool = log_sys_get_sv_temp_energy_cool();        /* ���� */
    sv_temp_energy_heat = log_sys_get_sv_temp_energy_heat();        /* ���� */
    sv_temp_energy_hotw = log_sys_get_sv_temp_energy_hotw();        /* ��ˮ */

    /* �ܵ������¶� */
    ctrl_temp_energy_ac = log_sys_get_ctrl_temp_energy_ac();        /* ����/���� */
    ctrl_temp_energy_hotw = pv_sys_THWt;	                        /* ��ˮ */

    /* �ܵ�����ƫ�� */    
    val_add_diff_energy_cool = log_sys_get_ac_add_diff(MODE_COOL);	/* ���� */
    val_add_diff_energy_heat = log_sys_get_ac_add_diff(MODE_HEAT);  /* ���� */
    val_add_diff_energy_hotw = val_hotw_diff;				        /* ��ˮ */

    /* �ܵ�ж��ƫ�� */
    val_sub_diff_energy_cool = log_sys_get_ac_sub_diff(MODE_COOL);	/* ���� */
    val_sub_diff_energy_heat = log_sys_get_ac_sub_diff(MODE_HEAT);	/* ���� */
    val_sub_diff_energy_hotw = 0;			                        /* ��ˮ */


    /* ��ƵĿ���¶� */
    sv_temp_freq_cool = log_sys_get_sv_temp_freq_ac(MODE_COOL);	    /* ���� */
    sv_temp_freq_heat = log_sys_get_sv_temp_freq_ac(MODE_HEAT);	    /* ���� */
    sv_temp_freq_hotw = sv_temp_energy_hotw;	                    /* ��ˮ */
}

/*******************************************************************************
 �� ��: log_sys_mode_need_confirm()
 �� ��: ģʽ����ȷ�ϣ��ֱ�������������������������ˮ����
 �� ��: ��
 �� ��: ��
 �� ��: ��
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

    /* �����ܵ������¶��������� */
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
    
    /* �������� */
    fg_cool_need = (fg_zone_A_run||fg_zone_B_run) && fg_cool_add;
    
    /* �������� */
    fg_heat_need = (fg_zone_A_run||fg_zone_B_run) && fg_heat_add;
    
    /* ����ˮ���� */
    fg_hotw_need = fg_hotw_run && fg_hotw_add;
                                                                   
    /* ���������� */
    if (USE_COOL_ONLY			            /* ������ */
    || (ac_mode == MODE_COOL)				/* ����ģʽ */
    )
    {
        fg_heat_need = FALSE;
    }
    
    /* ���������� */
    if (USE_HEAT_ONLY                       /* ������ */
    || (ac_mode == MODE_HEAT)				/* ����ģʽ */
    )          
    {
        fg_cool_need = FALSE;
    }
    
    /* ������ˮ���� */
    if (!val_hotw_used      /* ��ˮ���ܽ��� */
    || fg_sg_hotw_forbid    /* �߷��õ� */
    )
    {
        fg_hotw_need = FALSE;
    }
}

/*******************************************************************************
 �� ��: log_sys_disp_mode_confirm()
 �� ��: ��ʾģʽȷ��(disp_mode)
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_disp_mode_confirm(void)
{
    BOL fg_switch_need = FALSE;		                        /* �л������־ */
    U08 fg_switch_allow = TRUE;		                        /* �л������־ */
    static U08 T4_area = AREA_00;			                /* ���·��� */
    static BOL fg_enter_ac_switch_cycle = FALSE;		/* ����յ�ģʽ�л�״̬��־ */
    static BOL fg_enter_hotw_switch_cycle = FALSE;		/* ������ˮ�л�״̬��־ */

    /* ���������������ģʽ�л� */
    if (STEP_IS_IDLE		/* ����״̬ */
    || STEP_IS_ALARM		/* ����״̬ */
    || !val_hotw_used		/* ������ˮ���� */
    )
    {
        fg_enter_ac_switch_cycle = FALSE;
        fg_enter_hotw_switch_cycle = FALSE;
    }

    /* ���㻷������ */
    T4_area = fun_area(pv_sys_TA, T4_area, 10, (val_auto_cool_min_TA+10),
                                               val_auto_heat_max_TA,
                                               SENSOR_NOEXIST);

    /* ȷ�Ͽյ�ģʽ */
    if (sv_mode == SET_AUTO)
    {
        if (T4_area == AREA_00)
        {
            ac_mode = MODE_COOL;			/* ����ģʽ */
            if (USE_DUAL_ZONE_CTRL)
            {     
                /* B���ػ� */
                SETCMD_N(ZONE_B_OFF);
            }
        }
        else if (T4_area == AREA_02)  
        {
            ac_mode = MODE_HEAT;			/* ����ģʽ */
        }
        else if (T4_area == AREA_01 && ac_mode == MODE_NULL) /* ��ֹ����ģʽ���� */
        {
            ac_mode = MODE_HEAT;
        }
    }
    else if (sv_mode == SET_COOL)
    {
        ac_mode = MODE_COOL;				/* ����ģʽ */
    }
    else if (sv_mode == SET_HEAT)
    {
        ac_mode = MODE_HEAT;				/* ����ģʽ */
    }

    if (GETBIT_N(fg_test_run, TEST_RUN_COOL_MODE))          /* ���������� */
    {
        ac_mode = MODE_COOL;
    }
    else if (GETBIT_N(fg_test_run, TEST_RUN_HEAT_MODE))     /* ���������� */
    {
        ac_mode = MODE_HEAT;
    }


    /* ���յ���ϵͳ */
    disp_mode[SUB_SYS_AC_ONLY] = ac_mode;

    /* �յ�+��ˮ��ϵͳ */
    if (!fg_zone_A_run && !fg_zone_B_run && !fg_hotw_run)           /* A��/B��/��ˮ���ػ� */
    {
        disp_mode[SUB_SYS_AC_HOTW] = ac_mode;
        fg_enter_ac_switch_cycle = FALSE;
        fg_enter_hotw_switch_cycle = FALSE;
    }
    else if ((fg_zone_A_run || fg_zone_B_run) && !fg_hotw_run)      /* ��A��/B������ */
    {
        disp_mode[SUB_SYS_AC_HOTW] = ac_mode;
        fg_enter_ac_switch_cycle = FALSE;
        fg_enter_hotw_switch_cycle = FALSE;
    }
    else if (!fg_zone_A_run && !fg_zone_B_run && fg_hotw_run)       /* ����ˮ���� */
    {
        disp_mode[SUB_SYS_AC_HOTW] = MODE_HOTW;
        fg_enter_ac_switch_cycle = FALSE;
        fg_enter_hotw_switch_cycle = FALSE;
    }
    else                                                            /* A��/B��/��ˮ������ */
    {
        if (fg_defrost_run[SUB_SYS_AC_HOTW])
        {
            fg_switch_allow = FALSE;
        }
        
        if (IS_DUAL_SYS)
        {
            /* ����(������)����ʱ���� */
            if (MODE_IS_AC(SUB_SYS_AC_HOTW))
            {
                /* ���л�������ˮ���� */
                if (fg_hotw_need                            /* ������ˮ���� */
                && pump_time[PM_F_AC_HW].on > 60*2          /* ˮ������2min */
                && fg_switch_allow		                    /* �����л� */
                )
                {
                    disp_mode[SUB_SYS_AC_HOTW] = MODE_HOTW;
                }
            }

            /* ����ˮ����ʱ���� */
            if (MODE_IS_HOTW(SUB_SYS_AC_HOTW))
            {
                /* ���л�������(������)���� */
                if (!fg_hotw_need                           /* ������ˮ���� */
                && pump_time[PM_F_AC_HW].on > 60*2          /* ˮ������2min */
                && fg_switch_allow		                    /* �����л� */
                )
                {
                    disp_mode[SUB_SYS_AC_HOTW] = ac_mode;
                } 
            }
        }
        else if (USE_HOTW_PRIORITY)              /* ��ˮģʽ���� */
        {           
            if (!fg_enter_hotw_switch_cycle)	/* δ������ˮģʽ�л� */
            {
                if (MODE_IS_HOTW(SUB_SYS_AC_HOTW))		/* ȷ����ǰ����ģʽ�ѱ�Ϊ��ˮģʽʱ���ſɽ��������л� */
                {
                    fg_enter_hotw_switch_cycle = TRUE;
                    fg_enter_ac_switch_cycle = FALSE;
                }
                disp_mode[SUB_SYS_AC_HOTW] = MODE_HOTW;
            }
            else				/* ������ˮģʽ�л� */
            {
                /* ��������ˮ����ʱ���� */
                if (MODE_IS_HOTW(SUB_SYS_AC_HOTW))
                {
                    disp_mode[SUB_SYS_AC_HOTW] = MODE_HOTW;
                                     
                    if (USE_AC_TEMP_TWOUT2                          /* ʹ��TWout2(��ʱ�޷���⵽�յ�����) */
                    || (USE_AC_TEMP_TACT1 && FG_AC_NEED)            /* ��TACt1�����пյ����� */
                    || (USE_AC_TEMP_TACT2 && FG_AC_NEED)            /* ��TACt2�����пյ����� */
                    || (USE_AC_TEMP_TACT1_TACT2 && FG_AC_NEED)      /* ��TACt1��TACt2�����пյ����� */
                    )
                    {
                        fg_switch_need = TRUE;
                    }

                    /* ����ǰ�л�������(������)���� */
                    if (fg_switch_need                                           /* ���л����� */
                    && fg_switch_allow		                                     /* �����л� */
                    && !fg_hotw_need                                             /* ������ˮ���� */
                    && pump_time[PM_F_AC_HW].on > 60*2                           /* ˮ������2min */
                    && !log_sys_TA_limit_comp_is_off(MODE_HOTW, SUB_SYS_AC_HOTW) /* ���´���ѹ���ɿ�����ѹ�������� */
                    )
                    {
                        disp_mode[SUB_SYS_AC_HOTW] = ac_mode;
                    }    

                    /* ����ˮ����ʱ�䵽 */
                    if (hotw_runtime.on > val_hotw_comp_on_max
                    && fg_switch_need                           /* ���л����� */
                    &&  fg_switch_allow		                    /* �����л� */
                    )       
                    {
                        disp_mode[SUB_SYS_AC_HOTW] = ac_mode;
                    }                    
                }
    
                /* ��������(������)����ʱ���� */
                if (MODE_IS_AC(SUB_SYS_AC_HOTW))
                {
                    disp_mode[SUB_SYS_AC_HOTW] = ac_mode;

                    /* ����ǰ�л�������ˮ���� */
                    if (!FG_AC_NEED                                             /* �޿յ����� */
                    && fg_switch_allow		                                    /* �����л� */
                    && fg_hotw_need                                             /* ������ˮ���� */
                    && pump_time[PM_F_AC_HW].on > 60*2                          /* ˮ������2min */
                    && !log_sys_TA_limit_comp_is_off(ac_mode, SUB_SYS_AC_HOTW)  /* ���´���ѹ���ɿ�����ѹ�������� */
                    )
                    {
                        disp_mode[SUB_SYS_AC_HOTW] = MODE_HOTW;
                    }

                    /* �յ�����ʱ�䵽 */
                    if (fg_switch_allow		                    /* �����л� */
                    &&  fg_hotw_need                            /* ������ˮ���� */
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
        else                /* ��ˮģʽ������ */
        {
            if (!fg_enter_ac_switch_cycle)	/* δ����յ�ģʽ�л� */
            {
                if (MODE_IS_AC(SUB_SYS_AC_HOTW))		/* ȷ����ǰ����ģʽ�ѱ�Ϊ����/����ģʽʱ���ſɽ��������л� */
                {
                    fg_enter_ac_switch_cycle = TRUE;
                    fg_enter_hotw_switch_cycle = FALSE;
                }
                disp_mode[SUB_SYS_AC_HOTW] = ac_mode;
            }
            else
            {
                /* ��������(������)����ʱ���� */
                if (MODE_IS_AC(SUB_SYS_AC_HOTW))
                {
//                    disp_mode = ac_mode;
                    disp_mode[SUB_SYS_AC_HOTW] = ac_mode;

                    /* ����ǰ�л�������ˮ���� */
                    if (!FG_AC_NEED                                             /* �޿յ����� */
                    && fg_switch_allow		                                    /* �����л� */
                    && fg_hotw_need                                             /* ������ˮ���� */
                    && pump_time[PM_F_AC_HW].on > 60*2                          /* ˮ������2min */
                    && !log_sys_TA_limit_comp_is_off(ac_mode, SUB_SYS_AC_HOTW)  /* ���´���ѹ���ɿ�����ѹ�������� */
                    )
                    {
                        disp_mode[SUB_SYS_AC_HOTW] = MODE_HOTW;
                    }
                }

                /* ��������ˮ����ʱ���� */
                if (MODE_IS_HOTW(SUB_SYS_AC_HOTW))
                {
                    disp_mode[SUB_SYS_AC_HOTW] = MODE_HOTW;
                                     
                    if (USE_AC_TEMP_TWOUT2)			        /* ʹ��TWout2����TACt1��TACt2 */
                    {
                        if (fg_switch_allow		                        /* �����л� */
                        && (!fg_hotw_need                               /* ������ˮ���� */
                            || hotw_runtime.on > val_hotw_comp_on_max	/* ����ˮ����ʱ�䵽 */
                            )
                        )
                        {
                            disp_mode[SUB_SYS_AC_HOTW] = ac_mode;
                        }      
                    }
                    else                                    /* ����TACt1��TACt2 */
                    {
                        /* ����ǰ�л�������(������)���� */
                        if (FG_AC_NEED                                               /* �пյ����� */
//                        && !fg_hotw_need                                             /* ������ˮ���� */
                        && pump_time[PM_F_AC_HW].on > 60*2                           /* ˮ������2min */
                        && !log_sys_TA_limit_comp_is_off(MODE_HOTW, SUB_SYS_AC_HOTW) /* ���´���ѹ���ɿ�����ѹ�������� */
                        && fg_switch_allow		                                     /* �����л� */
                        )
                        {
                            disp_mode[SUB_SYS_AC_HOTW] = ac_mode;
                        }    
                    }
                }
            }
        }

        /* ��ˮϵͳ���� */
        if (fg_hotw_sys_err)
        {
            disp_mode[SUB_SYS_AC_HOTW] = ac_mode;
        }
    }

    /* ǿ�Ƹ�����ʾģʽ����� */
    {
        /* ǿ����ˮģʽ���� */
        if (fg_force_hotw_mode_run)                              
        {
            disp_mode[SUB_SYS_AC_HOTW] = MODE_HOTW;
        }
        
        /* ��ʱɱ������ */
        if (fg_timed_disinfect_run)
        {
            disp_mode[SUB_SYS_AC_HOTW] = MODE_HOTW;
        }
         
        if (fg_ability_test_run                 /* �������� */
        ||  TF_EN                               /* �̼� */
        )              
        {
            ARRSET(disp_mode, ac_mode);
        }

        /* �շ� */
        if (fg_refri_recy_act)
        {
            ac_mode = MODE_COOL;
            ARRSET(disp_mode, ac_mode);
        }

        /* �����ſ������� */
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

        /* ˮ�������� */
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

        /* ���������� */
        if (GETBIT_N(fg_test_run, TEST_RUN_COOL_MODE))
        {
            ac_mode = MODE_COOL;
            ARRSET(disp_mode, ac_mode);
        }

        /* ���������� */
        if (GETBIT_N(fg_test_run, TEST_RUN_HEAT_MODE))
        {
            ac_mode = MODE_HEAT;
            ARRSET(disp_mode, ac_mode);
        }
     
        /* ����ˮ������ */
        if (GETBIT_N(fg_test_run, TEST_RUN_HOTW_MODE))
        {
            disp_mode[SUB_SYS_AC_HOTW] = MODE_HOTW;
        }

//        if (fg_enter_holiday)       // �ݼ�ģʽ
//        {
//            disp_mode = holiday_mode;
//        }

//        if (room_therostat_mode != MODE_NULL)   // �¿���ȷ��ģʽ
//        {
//            disp_mode = room_therostat_mode;
//        }

        if (fg_floor_preheat_run/* ��ůԤ�ȹ��� */
        || fg_floor_dryup_run	/* ��ů��ɹ��� */
        )   
        {
            ac_mode = MODE_HEAT;
            ARRSET(disp_mode, ac_mode);
        }
    }

    /* ������ˮ�����Ȳ�ͣ���л� */
    {
        /* ��ˮ�л������� */
        if (MODE_IS_HOTW(SUB_SYS_AC_HOTW) && (disp_mode[SUB_SYS_AC_HOTW]==MODE_HEAT))
        {
            if (ctrl_temp_energy[SUB_SYS_AC_HOTW] < val_err_ac_out_high-val_err_wt_exit)
            {
                SET_HOTW_HEAT_SWITCH_DLY;
            }
        }
        /* �����л�����ˮ */
        if (MODE_IS_HEAT(SUB_SYS_AC_HOTW) && (disp_mode[SUB_SYS_AC_HOTW]==MODE_HOTW))
        {
            if (ctrl_temp_energy[SUB_SYS_AC_HOTW] < val_err_ac_out_high-val_err_wt_exit)
            {
                SET_HOTW_HEAT_SWITCH_DLY;
            }
        }
        /* ������ʱֱ����� */
        if (!fg_heat_need && !fg_hotw_need)
        {
            CLR_HOTW_HEAT_SWITCH_DLY;
        }
    }
}

/*******************************************************************************
 �� ��: log_sys_ctrl_mode_confirm()
 �� ��: ȷ����������ģʽ(���п���ģʽ)
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_ctrl_mode_confirm(void)
{
    #define PV_FILT 3               /* ���� */
    static U08 confirm_mode_bak[SUB_SYS_TYPE_MAX];    // ȷ�ϵ�ģʽ������

    /* �յ�+��ˮ��ϵͳ */
    {
        if (confirm_mode_bak[SUB_SYS_AC_HOTW] != disp_mode[SUB_SYS_AC_HOTW])
        {
            confirm_mode_bak[SUB_SYS_AC_HOTW] = disp_mode[SUB_SYS_AC_HOTW];
            ctrl_mode_dly[SUB_SYS_AC_HOTW] = 0;
        }
    
        /* ȷ������ģʽ */
        if (!STEP_IS_RUN		                                /* ������̬��ֱ���� */
        || ctrl_mode_dly[SUB_SYS_AC_HOTW] > 10
        || GETBIT_N(fg_test_run, TEST_RUN_AIR_EMPTY)			/* �����ſ������У�����ѹ������ֱ����ģʽ */
        || GETBIT_N(fg_test_run, TEST_RUN_PUMP)	                /* ˮ�������У�����ѹ������ֱ����ģʽ */
        )  
        {
            ctrl_mode[SUB_SYS_AC_HOTW] = disp_mode[SUB_SYS_AC_HOTW];
        }
    }

    /* ���յ���ϵͳ */
    {
        if (confirm_mode_bak[SUB_SYS_AC_ONLY] != disp_mode[SUB_SYS_AC_ONLY])
        {
            confirm_mode_bak[SUB_SYS_AC_ONLY] = disp_mode[SUB_SYS_AC_ONLY];
            ctrl_mode_dly[SUB_SYS_AC_ONLY] = 0;
        }
    
        /* ȷ������ģʽ */
        if (!STEP_IS_RUN		                                /* ������̬��ֱ���� */
        || ctrl_mode_dly[SUB_SYS_AC_ONLY] > 10
        || GETBIT_N(fg_test_run, TEST_RUN_AIR_EMPTY)			/* �����ſ������У�����ѹ������ֱ����ģʽ */
        || GETBIT_N(fg_test_run, TEST_RUN_PUMP)	                /* ˮ�������У�����ѹ������ֱ����ģʽ */
        )  
        {
            ctrl_mode[SUB_SYS_AC_ONLY] = disp_mode[SUB_SYS_AC_ONLY];
        }
    }
}

/*******************************************************************************
 �� ��: log_sys_ctrl_mode_get()
 �� ��: ��װ�������ģʽ�ӿ�
 �� ��: type��ѡ���ȡ��ʽ
 �� ��: �������ģʽ
 �� ��: ��
*******************************************************************************/
U08 log_sys_ctrl_mode_get(U08 type)
{
    return MODE_NULL;
}

/*******************************************************************************
 �� ��: log_sys_get_ctrl_object()
 �� ��: ��ȡ���ƶ���
 �� ��: ��
 �� ��: 
 �� ��: ��
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
 �� ��: log_sys_ctrl_mode_temp_confirm()
 �� ��: ȷ��: ���ܵ�/��Ƶ�����¶ȡ����ܵ�/��ƵĿ���¶ȡ��ۼ���/ж��ƫ��
 �� ��: ��
 �� ��: ��
 �� ��: ��
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
                /* �ܵ� */
                {
                    sv_temp_energy[i] = sv_temp_energy_cool;            /* �ܵ�Ŀ���¶� */
                    ctrl_temp_energy[i] = ctrl_temp_energy_ac;          /* �ܵ������¶� */

//                    val_add_diff_energy[i] = val_add_diff_energy_cool;	/* �ܵ�����ƫ�� */
                    val_sub_diff_energy[i] = val_sub_diff_energy_cool;	/* �ܵ�ж��ƫ�� */
                }
    
                /* ��Ƶ */
                if (FG_AF_ALL)
                {
                    sv_temp_freq[i] = AF_SV_TEMP_FREQ;
                }
                else
                {
                    sv_temp_freq[i] = sv_temp_freq_cool;                /* ��ƵĿ���¶� */
                }
                break;
                
            case MODE_HEAT:       
                /* �ܵ� */
                {
                    sv_temp_energy[i] = sv_temp_energy_heat;            /* �ܵ�Ŀ���¶� */
                    ctrl_temp_energy[i] = ctrl_temp_energy_ac;          /* �ܵ������¶� */

//                    val_add_diff_energy[i] = val_add_diff_energy_heat;  /* �ܵ�����ƫ�� */
                    val_sub_diff_energy[i] = val_sub_diff_energy_heat;  /* �ܵ�ж��ƫ�� */
                }          
    
                /* ��Ƶ */
                {
                    sv_temp_freq[i] = sv_temp_freq_heat;			    /* ��ƵĿ���¶� */
                }
                break;
                
            case MODE_HOTW:
                /* �ܵ� */
                {
                    sv_temp_energy[i] = sv_temp_energy_hotw;            /* �ܵ�Ŀ���¶� */
                    ctrl_temp_energy[i] = ctrl_temp_energy_hotw;        /* �ܵ������¶� */

//                    val_add_diff_energy[i] = val_add_diff_energy_hotw;  /* �ܵ�����ƫ�� */
                    val_sub_diff_energy[i] = val_sub_diff_energy_hotw;  /* �ܵ�ж��ƫ�� */
                }
    
                /* ��Ƶ */
                {
                    sv_temp_freq[i] = sv_temp_freq_hotw;			    /* ��ƵĿ���¶� */
                }
                break;
        }
    }
}

/*********************************************
������ 
���ܣ� ����ϵͳ�Ŀ�����ֹͣ
��������
����ֵ����
*********************************************/
void log_sys_start_and_stop(void)
{ 
    BOOL fg_stop = FALSE;

    if (power_time <= POWER_ON_ONOFF) 
    {
        return;
    }

    /* ǿ����ˮģʽ���� */
    on_off_force_hotw_mode();
    
    /* �߿ؿ��� */
    if (REMOTE_IS_ONOFF)       on_off_remote();
    if (REMOTE_IS_UNION_ACT)
    {
        on_off_union_act();
//        if (fg_onoff_union_act) key_stop = FALSE;   /* ����������Ч���������ر��ֱպ�ʱ�����鲻��ͨ���߿������йػ� */

        /* ����������Ч���������ر��ֱպ�ʱ�����鲻��ͨ���߿������йػ� */
        if (fg_onoff_union_act)
        {
            fg_zone_A_off = FALSE;
            fg_zone_B_off = FALSE;
            fg_hotw_off = FALSE;
        }
    }
       
    /* �������������� */
    if (!fg_floor_preheat_run
    && !fg_floor_dryup_run
    && !fg_enter_test_run
    )
    {
        static U08  auto_run_bit_bak = 0;

        if (val_remember_run == USED)   /* ����������ʹ�� */
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
                // δ����ɱ����δ�����ů��δ����ǿ����ˮģʽ��δ�������ģʽ�ű���
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
    
    /* ������ͣ���ƴ��� */
    switch(val_on_off_control_type)
    {
        case ONOFF_SWITCH:          /* �߿� */
            commu_run = FALSE;
            commu_stop = FALSE;
//          break;
        case ONOFF_REMOTE:          /* Զ�� */
            key_run = FALSE;
            key_stop = FALSE;
            break;      
            
        case ONOFF_COMMU:           /* ���� */
            key_run = FALSE;
            key_stop = FALSE;
            break;
            
        case ONOFF_SELF:            /* ���� */
            commu_run = FALSE;
            commu_stop = FALSE;
            break;
    }
    
    /* �ֲ����ؿ��ػ� */
    if (key_run || commu_run)       /* �յ��������� */
    {
        key_run = FALSE;
        commu_run = FALSE;
//        Logic_machine_start();      /* ִ�п������� */

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
    if (key_stop || commu_stop)     /* �յ��ػ����� */
    {
        key_stop = FALSE;
        commu_stop = FALSE;
//        Logic_machine_stop();       /* ִ�йػ����� */

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

    /* A������״̬ */
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
            Logic_machine_start();          /* ִ�п������� */
        }
    }

    /* B������״̬ */
    if (!USE_DUAL_ZONE_CTRL                                                                 /* ��˫������ */
    || ((ctrl_mode[SUB_SYS_AC_HOTW]==MODE_COOL) && (ctrl_mode[SUB_SYS_AC_ONLY]==MODE_COOL))	/* ��������ʱ */
    || (MODE_IS_HOTW(SUB_SYS_AC_HOTW) && ac_mode == MODE_COOL)				                /* ����ˮ����ʱ���ҿյ�ģʽΪ���� */
    )
    {
        /* B������������Ч */
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
            Logic_machine_start();          /* ִ�п������� */
        }
    }

    /* ��ˮ����״̬ */
    if (!val_hotw_used)		/* ����ˮ����ˮ���ػ�������Ч */
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
            Logic_machine_start();          /* ִ�п������� */
        }
    }

    /* ִ�йػ����� */
    if (fg_stop
    && !fg_zone_A_run
    && !fg_zone_B_run
    && !fg_hotw_run
    )
    {
        Logic_machine_stop();               
    }
    
    on_off_limit(); // ���ػ�����
}


/*******************************************************************************
 �� ��: log_sys_cmd_time()
 �� ��: ����ģ���ʱ
 �� ��: ��; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_cmd_time(void)
{
    U08 i;
    U08 j;
    
    //----------------------���ʱ----------------------------------
    if (F_TIME_BASIC_SEC_BACK)
    {
        // ͨѶ��ʱ��ʱ 
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
    ����: ��ʱ
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
          
        /* ˮ��ͣ��ʱ�� */
        for (i=0; i<PM_MAX; i++)      /* ˮ�ø��� */
        {
            _CNT(pump_time[i].off);
            _CNT(pump_time[i].on);
            _CNT(freeze_energy_time[i]);
        } 
        
        /* ��������ʱ�� */
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

        /* ��ʱ: ����ģ��ı�Ƶˮ�� */
        if (log_sys_have_cell_pump_run(SUB_SYS_TYPE_MAX) == FALSE)  /* ����ģ���Ƶˮ��δ�� */
        {
            pump_time[PM_F_ANY].on = 0;
        }
        else                /* ����ģ���Ƶˮ�ÿ��� */
        {
            pump_time[PM_F_ANY].off = 0;
        }

        /* ��ʱ: ���п�����ˮģ��ı�Ƶˮ�� */
        if ((log_sys_have_cell_pump_run(SUB_SYS_AC_HOTW) == FALSE)  /* ���п�����ˮģ���Ƶˮ��δ�� */
        || (log_sys_have_cell_pump_run(SUB_SYS_AC_HOTW)             /* ������ˮģ���Ƶˮ�ÿ��� */
           && !MODE_IS_HOTW(SUB_SYS_AC_HOTW)                        /* �յ�+��ˮ��ϵͳ������ˮ���� */
           && !fg_run_sta_hotw_antifreeze                           /* ������̬��ˮˮ·���� */
           )
        )
        {
            pump_time[PM_F_HW_RATFZ].on = 0;
        }
        else                                                        /* ���п�����ˮģ���������Ƶˮ�ÿ��� */
        {
            pump_time[PM_F_HW_RATFZ].off = 0;
        }

        /* ��ʱ: ���в�������ˮģ��ı�Ƶˮ�� */
        if (!fg_zone_A_run                  /* A��δ���� */
        && !fg_zone_B_run                   /* B��δ���� */
        && fg_hotw_run                      /* ��ˮ���� */
        && !fg_run_sta_ac_antifreeze        /* δ������̬�յ�ˮ·���� */
        )  /* ���в�������ˮģ���Ƶˮ��δ�� */
        {
            pump_time[PM_F_NHW_RATFZ].on = 0;
        }
        else                                                        /* ���в�������ˮģ���������Ƶˮ�ÿ��� */
        {
            pump_time[PM_F_NHW_RATFZ].off = 0;
        }

        /* ����̬����ʱ */
        if (fg_idle_sta_antifreeze)
        {
            /* �������̬������������¼�ʱ��������ֹ����̬������;�������ܻ����̽�������̬���� */
            pump_time[PM_F_HW_RATFZ].off = 0;
            pump_time[PM_F_NHW_RATFZ].off = 0;
            
            /* �������̬������������¼�ʱ��������ֹ����̬������ˮ��δ�ȶ����У�47�룩��;�������ܻ����̽������̬����(����ӵ�һ������) */
			pump_time[PM_F_ANY].off = 0;
        }
		
        /* ��ʱ: ���мȿ�����ˮ�ֿ�����(������)ģ��ı�Ƶˮ�� */
        if (log_sys_have_cell_pump_run(SUB_SYS_AC_HOTW) == FALSE)   /* ���мȿ�����ˮ�ֿ�����(������)ģ���Ƶˮ��δ�� */
        {
            pump_time[PM_F_AC_HW].on = 0;
        }
        else                                                        /* ���мȿ�����ˮ�ֿ�����(������)ģ���������Ƶˮ�ÿ��� */
        {
            pump_time[PM_F_AC_HW].off = 0;
        }

        /* ��ʱ: �»�ˮ�� */
        if (!get_output(MAX_PCB, M_DO_PUMPret))               
        {
            pump_time[PM_RET].on = 0;
        }
        else
        {
            pump_time[PM_RET].off = 0;
        }

        /* ��ʱ: ��ѭ���� */
        if (!get_output(MAX_PCB, M_DO_PUMPo))
        {
            pump_time[PM_O_TATFZ].on = 0;
        }
        else
        {
            pump_time[PM_O_TATFZ].off = 0;
        }

        /* ��ʱ: ���ˮ�� */
        if (!get_output(MAX_PCB, M_DO_PUMPm))
        {
            pump_time[PM_M_TATFZ].on = 0;
        }
        else
        {
            pump_time[PM_M_TATFZ].off = 0;
        }

        /* ��ʱ: ���пɲ�����ˮ��ʱ������Ƶˮ�� */
        if (!(HOTW_TAF||fg_hotw_run))
        {
            pump_time[PM_HW_TATFZ].on = 0;
        }
        else
        {
            pump_time[PM_HW_TATFZ].off = 0;
        }

        /* ��ʱ: �����¶�����ѹ���������� */
        for (i = 0; i < SUB_SYS_TYPE_MAX; i++)
        {
            for (j = 0; j < CTRL_MODE_MAX; j++)
            {
                _CNT(cnt_sys_comp_limit_TA_dly[i][j]);
            }
        }

        /* ��ʱ: ���ܵ���ָ�� */
        for (i = 0; i < SG_CMD_MAX; i++)
        {
            _CNT_WHEN(cnt_sg_cmd_on[i], (smart_grid_cmd == i));
        }

        /* ��ʱ: ���ܵ���״̬ */
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

        /* �ⲿ��Դ��ʱ */
        _CNT(AHS_time.off);
        _CNT(AHS_time.on);
        if (!get_output(MAX_PCB, M_DO_AHS))             AHS_time.on = 0;
        else                                            AHS_time.off = 0;
        
        /* ��������ʱ�� */
        for (i=0; i<SUB_SYS_TYPE_MAX; i++)
        {
            _CNT_WHEN(cool_runtime[i].on, RUN_COOL(i));
            _CNT_WHEN(cool_runtime[i].off, !RUN_COOL(i));
            if (RUN_COOL(i))
            {
                _CNT_WHEN(cool_runtime[i].run, fg_cool_need);
            }
        }

        /* ��������ʱ�� */
        for (i=0; i<SUB_SYS_TYPE_MAX; i++)
        {
            _CNT_WHEN(heat_runtime[i].on, RUN_HEAT(i));
            _CNT_WHEN(heat_runtime[i].off, !RUN_HEAT(i));
            if (RUN_HEAT(i))
            {
                _CNT_WHEN(heat_runtime[i].run, fg_heat_need);
            }
        }
        
        /* ��ˮ����ʱ�� */
        _CNT_WHEN(hotw_runtime.on, RUN_HOTW(SUB_SYS_AC_HOTW));				
        _CNT_WHEN(hotw_runtime.off, !RUN_HOTW(SUB_SYS_AC_HOTW));				
        if (RUN_HOTW(SUB_SYS_AC_HOTW))
        {
            _CNT_WHEN(hotw_runtime.run, fg_hotw_need);
        }

        _CNTDOWN(cnt_hotw_heat_switch_dly);

        /* ����ۼ�����ʱ�� */
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

        /* ��ʱɱ����ʱ */
        _CNT_WHEN(cnt_ster_cyc, fg_timed_disinfect_run);
        _CNT_WHEN(cnt_ster_high_temp, fg_timed_disinfect_run&&(pv_sys_THWt>=val_ster_temp-20));

        /* �����м�ʱ */
        _CNT_WHEN(cnt_test_run_air_empty, GETBIT_N(fg_test_run, TEST_RUN_AIR_EMPTY));
        _CNT_WHEN(cnt_test_run_pump, GETBIT_N(fg_test_run, TEST_RUN_PUMP));

        /* ��ůԤ��ѹ����ʱ */
        _CNT_WHEN(floor_preheat_time.on, fg_floor_preheat_run&&log_sys_have_comp_run(MODE_NULL, SUB_SYS_TYPE_MAX));
        _CNT_WHEN(floor_preheat_time.off, fg_floor_preheat_run&&!log_sys_have_comp_run(MODE_NULL, SUB_SYS_TYPE_MAX));
        
        /* ̫�����źż�ʱ */
        _CNT_WHEN(cnt_HDIsl_on, m_get_input(IN_SOLAR));
        _CNT_WHEN(cnt_HDIsl_off, !m_get_input(IN_SOLAR));

        /* ̫�����¶ȼ�ʱ */
        _CNT_WHEN(cnt_TSL_on, (pv_sys_TSL>val_PUMPsl_on_TSL));
        _CNT_WHEN(cnt_TSL_off, (pv_sys_TSL<=val_PUMPsl_on_TSL-val_PUMPsl_TSL_dT));

        /* �߷��õ��������м�ʱ */
        if (PEAK_ELEC_LIMIT_RUN)
        {
            _CNT_WITH(peak_elec_air_runtime, log_sys_have_comp_run(MODE_NULL, SUB_SYS_TYPE_MAX));
        }
        else
        {
            peak_elec_air_runtime = 0;
        }
        
		/* ͿѻAPP�����л����ʱ */
		_CNT_WITH(cnt_wait_tuya_lang, fg_tuya_lang_switch);

		/* ͿѻAPP�¶ȵ�λ�л����ʱ */
		_CNT_WITH(cnt_wait_tuya_temp_unit, fg_tuya_temp_unit_switch);

		/* ���������и����������м�ʱ */
		_CNT_WHEN(cnt_IBH_on, fg_test_run_heat_IBH_run);

        /* ����ˮ������ˮ��������м�ʱ */
		_CNT_WHEN(cnt_test_run_hotw_TBH_on, fg_test_run_hotw_TBH_run);

		/* �߿ؿ����źż�ʱ */
		_CNT_WHEN(cnt_LDIrm_dly, m_get_input(IN_REMOTE));

        /* ��ͨ��3���ؼ�ʱ */
        _CNT_WHEN(cnt_S3V3_ext_circue_s, get_output(MAX_PCB, M_DO_S3V3));
        _CNT_WHEN(cnt_S3V3_in_circue_s, !get_output(MAX_PCB, M_DO_S3V3));

        /* �õ���������ʱ���룩 */
        _CNT(cnt_pwru_cal_sec);
        _CNT(dbg_cnt_pwru_cal_10s);
        _CNT(dbg_cnt_pwru_cal_20s);
        _CNT(dbg_cnt_pwru_cal_60s);
    }

/*----------------------�ּ�ʱ----------------------------------*/  
    
    if (F_TIME_BASIC_MIN_BACK)
    {
        if (sys_warm_time != 0xffff)  sys_warm_time++;
        if (power_down_time_space != 0xffff)  power_down_time_space++;
//        password_super_save();

        
        /* ϵͳ����ʱ�� */
        if (status_run != _IDLE && status_run != _ALARM)
        {
            if (++sys_time.run >= 60)                       /* 1Сʱʱ�䵽 */
            {
//              if (cell[MAX_PCB].cell_runtime[LIMIT_RUNTIME] < 0xffff && limit_value != 0) 
//              {
//                  cell[MAX_PCB].cell_runtime[LIMIT_RUNTIME]++;
//                  cmd_systime_save();         /* ϵͳ����ʱ���ۼ�(1��Сʱ����һ��) */
//              }
                if (cell[MAX_PCB].cell_runtime[SYS_RUNTIME] < 0xffff )  
                {
                    cell[MAX_PCB].cell_runtime[SYS_RUNTIME]++; 
                    cmd_systime_save();         /* ϵͳ����ʱ���ۼ�(1��Сʱ����һ��) */
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

        /* ��ůԤ���ۼ����м�ʱ(��) */
        _CNT_WHEN(floor_preheat_time.run, fg_floor_preheat_run);

        /* �õ���������ʱ���֣� */
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
����: log_sys_public_info
����: ��������Ϣ
����: ��
����ֵ: ��
***************************************************/
void log_sys_public_info(void)
{
    U08 i, k;
    static U16 sv_cell_unable;      /* ģ��ʹ�ã�λģʽ */
    
    for (i=0; i<CTRL_INFO_NUM ;i++ )     
    {
        ctrl_info[i] = 0;
    }
                
    SET_BIT(ctrl_info[0], status_run&0x07);  /* ��������״̬ */

    if (!fg_warm_complete)                                      // ����Ԥ�� 
    {
        ctrl_info[0] |= 0x0100;
    }
    
    if (fg_error)                                               /* �������״̬ */
    {
        ctrl_info[0] |= 0x0200;
    }
    /* �������״̬ */
    if (FG_AF_ALL)  
    {
        ctrl_info[0] |= 0x0400;
    }
    if(fg_defrost_run[SUB_SYS_AC_HOTW] || fg_defrost_run[SUB_SYS_AC_ONLY])//�����˪
    {
        ctrl_info[0] |= 0x0800;
    }
//    if(m_get_input(IN_LINK))//��ͨ������
//    {
//        ctrl_info[0] |= 0x8000;
//    }

    for (i=0; i<val_pcb_num; i++)
    {
        if (CELL_Is_used(i))        SETBIT(sv_cell_unable, i);
        else                        CLRBIT(sv_cell_unable, i);
    }

    /* ���������ɳ���ʵ�����������������ֲʱ��ע���޸�(�����ڵ��¶���Ϊ��SENSOR_NOEXIST��) */
    /* ����������Ϣ */
    k = 1;      /* ��ע��: ����ctrl_info[]�����СCTRL_INFO_NUM����Ҫ���� */                          
    ctrl_info[k++] = m_val_comp_num(0);                     // 01 ѹ����                        
    ctrl_info[k++] = sv_mode;                               // 02 �趨ģʽ
    ctrl_info[k++] = ctrl_mode[SUB_SYS_AC_HOTW];			/* 03 �յ�+��ˮ��ϵͳ����ģʽ */
    ctrl_info[k++] = sv_temp_energy[SUB_SYS_AC_HOTW];		/* 04 �յ�+��ˮ��ϵͳ�ܵ�Ŀ���¶� */
    ctrl_info[k++] = ctrl_temp_energy[SUB_SYS_AC_HOTW];		/* 05 �յ�+��ˮ��ϵͳ�ܵ������¶� */
    ctrl_info[k++] = m_motor_sv_sheat;                      // 06 ��·���ù��ȶ�            
    ctrl_info[k++] = m_motor_pv_sheat;                      // 07 ��·��ǰ���ȶ�                            
    ctrl_info[k++] = GET_EXIO_ST_SV(0, EEV_ID_1);           // 08 ��·����Ŀ�꿪��                                          
    ctrl_info[k++] = GET_EXIO_ST_PV(0, EEV_ID_1);           // 09 ��·����ǰ����                                          
    ctrl_info[k++] = s_motor_sv_sheat;                      // 10 ��·���ù��ȶ�
    ctrl_info[k++] = s_motor_pv_sheat;                      // 11 ��·��ǰ���ȶ�
    ctrl_info[k++] = GET_EXIO_ST_SV(0, EEV_ID_2);           // 12 ��·����Ŀ�꿪��                                          
    ctrl_info[k++] = GET_EXIO_ST_PV(0, EEV_ID_2);           // 13 ��·����ǰ����                                          
    ctrl_info[k++] = m_motor_judge_sheat;                   // 14 ��·��ǰ���ȹ��ȶ� 
    ctrl_info[k++] = test_buf[0];                           // 15 ������������                              
    ctrl_info[k++] = test_buf[1];                           // 16 ������������                          
    ctrl_info[k++] = test_buf[2];                           // 17 �������ټ�ʱ                          
    ctrl_info[k++] = test_buf[3];                           // 18 �������ټ�ʱ                      
    ctrl_info[k++] = s_motor_pvnow_sheat;                   // 19 ������ǰ���ȶ�    
    ctrl_info[k++] = fg_forbid_rise[0];                     // 20 ��ֹ��Ƶ��־  
    ctrl_info[k++] = fg_force_drop[0];                      // 21 ǿ�ƽ�Ƶ��־  
    ctrl_info[k++] = skip_preheat_cnt;                      // 22 ����Ԥ�ȴ��� 
    ctrl_info[k++] = sv_cell_unable;                        // 23 ģ��ʹ��
    ctrl_info[k++] = val_pcb_num;			                /* 24 ģ���� */
    ctrl_info[k++] = status_run;			                /* 25 ����״̬ */	
    ctrl_info[k++] = SHC_get_set_tmp_max_A();               // 26 A���趨�¶�����
    ctrl_info[k++] = SHC_get_set_tmp_min_A();               // 27 A���趨�¶�����
    ctrl_info[k++] = SHC_get_set_tmp_max_B();               // 28 B���趨�¶�����
    ctrl_info[k++] = SHC_get_set_tmp_min_B();               // 29 B���趨�¶�����
    ctrl_info[k++] = SHC_get_hot_tmp_set_max();	            /* 30 ��ˮ�趨�¶����� */
    ctrl_info[k++] = SHC_get_hot_tmp_set_min();	            /* 31 ��ˮ�趨�¶����� */
    ctrl_info[k++] = para_get_max(P_GaI(sv_cool));          // 32 �����趨�¶�����
    ctrl_info[k++] = para_get_min(P_GaI(sv_cool));          // 33 �����趨�¶�����
    ctrl_info[k++] = para_get_max(P_GaI(sv_heat));          // 34 �����趨�¶�����
    ctrl_info[k++] = para_get_min(P_GaI(sv_heat));          // 35 �����趨�¶�����
    ctrl_info[k++] = para_get_max(P_GaI(sv_cool_zone_B));   // 36 B�������趨�¶�����
    ctrl_info[k++] = para_get_min(P_GaI(sv_cool_zone_B));   // 37 B�������趨�¶�����
    ctrl_info[k++] = para_get_max(P_GaI(sv_heat_zone_B));   // 38 B�������趨�¶�����
    ctrl_info[k++] = para_get_min(P_GaI(sv_heat_zone_B));   // 39 B�������趨�¶�����
    ctrl_info[k++] = log_sys_warm_time_get();               // 40 Ԥ��ʣ�������
    ctrl_info[k++] = GET_INIT_PARA();                       // 41 ������ʼ��״̬
    ctrl_info[k++] = log_sys_get_flag_area_antifreeze();    // 42 ��ȡ���������־
    /* ������ע�⣺���ܳ���CTRL_INFO_NUM */


    /* �������� */
    k = 0;      /* ��ע��: ����assist_info[]�����С����Ҫ���� */
    /* ϵͳ������ģʽ */
    if (USE_COOL_ONLY)		/* ������ */
    {
        assist_info[k] = SYS_COOL_CAN_USE;
    }
    else if (USE_HEAT_ONLY)	/* ������ */
    {
        assist_info[k] = SYS_HEAT_CAN_USE;
    }
    else					/* ����+���� */
    {
        assist_info[k] = SYS_COOL_CAN_USE|SYS_HEAT_CAN_USE|SYS_AUTO_CAN_USE;
    }

    if (GETBIT_N(fg_test_run, TEST_RUN_COOL_MODE)			        /* ����������ʱ */
    || (USE_UIRT_ZONE_A_MODE_SWITCH&&m_get_input(IN_RT_COOL))		/* ����ģʽ�л������¿���H�źűպ� */
    )
    {
        assist_info[k] = SYS_COOL_CAN_USE;
    }
    else if (fg_floor_preheat_run			                        /* ��ůԤ������ʱ */
    || fg_floor_dryup_run	                                        /* ��ů�������ʱ */
    || GETBIT_N(fg_test_run, TEST_RUN_HEAT_MODE)			        /* ����������ʱ */
    || (USE_UIRT_ZONE_A_MODE_SWITCH&&m_get_input(IN_RT_HEAT))		/* ����ģʽ�л������¿���C�źűպ� */
    )
    {
        assist_info[k] = SYS_HEAT_CAN_USE;
    }
    
    /* �����ʹ��(�Ƿ���ڸ������) */
    
    /* ѹ����� */
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

    /* ģ����� */
    k++;
    assist_info[2] = 0x00;
    /* ��Ƶˮ�� */
    if (USE_PUMPf_CTRL != PUMPF_CTRL_OFF)
    {
        SETBIT(assist_info[2], S_DO_PUMPf);
    }
    /* �������� */
    if (val_pcb_num == 1)
    {
        if (val_use_IBH)
        {
            if (IBH1_IS_USE)
            {
                SETBIT(assist_info[2], S_DO_IBH1);				/* ��������1 */
            }
            
            if (IBH2_IS_USE)
            {
                SETBIT(assist_info[2], S_DO_IBH2);				/* ��������2 */
            }
        }
    }
    else
    {
        /* ����ʱ������׼�жϣ�ֱ��ȫ�� */
        SETBIT(assist_info[2], S_DO_IBH1);				/* ��������1 */
        SETBIT(assist_info[2], S_DO_IBH2);				/* ��������2 */
    }
    
    SETBIT(assist_info[2], S_DO_DPH);		/* ���̵���� */
    if (val_EXDO7_selfdef == EXDO7_IS_DPH2)
    {
        SETBIT(assist_info[2], S_DO_DPH2);		/* ���̵����2 */
    }
    SETBIT(assist_info[2], S_DO_HT);		/* ���������ȴ� */

    /* ϵͳ���1 */
    k++;
//    assist_info[3] = 0xffff;
//    CLRBIT(assist_info[3], M_DO_PUMP);
    assist_info[3] = 0x0000;
    SETBIT(assist_info[3], M_DO_DFR);		/* ��˪ָʾ */
    SETBIT(assist_info[3], M_DO_ERR);		/* ����ָʾ */

    /* �ⲿ��Դ */
    if (val_use_AHS)
    {
        SETBIT(assist_info[3], M_DO_AHS);	
    }

    /* ��ˮ��� */
    if (val_hotw_used)
    {
        /* �����ͨ��1 */
        SETBIT(assist_info[3], M_DO_S3V1);
        /* ˮ����� */
        if (val_TBH_used)
        {
            SETBIT(assist_info[3], M_DO_TBH);
        }
        /* �»�ˮ�� */
        if (val_use_PUMPret)
        {
            SETBIT(assist_info[3], M_DO_PUMPret);
        }
        /* ̫����ˮ�� */
        if (val_solar_used)
        {
            SETBIT(assist_info[3], M_DO_PUMPsl);
        }
    }
    
    SETBIT(assist_info[3], M_DO_S3V2);		/* �����ͨ��2 */    
    SETBIT(assist_info[3], M_DO_PUMPo);		/* ��ѭ���� */

    /* ˫��������� */
    if (USE_DUAL_ZONE_CTRL)
    {  
        SETBIT(assist_info[3], M_DO_S3V3);	/* �����ͨ��3 */
        SETBIT(assist_info[3], M_DO_PUMPm);	/* ���ˮ�� */
    }
    
    /* ϵͳ���2 */
    k++;
    assist_info[4] = 0;         
    /* �����ʹ��(�Ƿ���ڸ������) */

    /* ѹ������ */
    k++;
    assist_info[5] = 0;      
    /* ģ������ */
    k++;
    assist_info[6] = 0;      /* N0004 */
    /* ϵͳ����1 */
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
    //assist_info[7] = 0x02;        /* N0004 */     /* ԭֻ��ʾ"�߿ؿ���"������ʾ"��ͨ����" */
    /* ϵͳ����2 */
    k++;
    assist_info[8] = 0x00;  
    /* ģ����� */
    k++;        
    for (i=0; i<val_pcb_num; i++)
    {
        if (M_CM_GetStatus(i, MAX_COMP, CELL_ERR) || err_sys_get_s_commu_err(i))
        {
            assist_info[k] |= 0x0001 << i;
        }
		pcb_comp_num[i] = m_val_comp_num(i);		
    }

    //��Ƶ���ϲ�����Ϣ����ȡ��FP�������һ�ι���ʱ�����������ѹ����һ����ַ����ȡ��FP�����ڴ˴�����õ�ַ���ú�������ʾ����
#define     INVT_EER_PARA_V_IN      3           //ǰ��λ��ʾ�����ѹ
#define     INVT_EER_PARA_A_IN      0xC000      //����λ��ʾ�������
    k = 0;
    inv_err_para_info[k++] = invt_para_fp[INVT_ID_1][0];     /* ���һ�ι������� */
    inv_err_para_info[k++] = invt_para_fp[INVT_ID_1][1];     /* ���һ�ι���ʱ�ۼ�����ʱ�� */    
    inv_err_para_info[k++] = invt_para_fp[INVT_ID_1][2];     /* ���һ�ι���ʱ������Ƶ�� */
    inv_err_para_info[k++] = invt_para_fp[INVT_ID_1][3];     /* ���һ�ι���ʱ�ĸ���Ƶ�� */
    inv_err_para_info[k++] = invt_para_fp[INVT_ID_1][4];     /* ���һ�ι���ʱ��������� */
    inv_err_para_info[k++] = invt_para_fp[INVT_ID_1][5];     /* ���һ�ι���ʱ�������ѹ */
    inv_err_para_info[k++] = invt_para_fp[INVT_ID_1][6];     /* ���һ�ι���ʱ��������� */
    inv_err_para_info[k++] = invt_para_fp[INVT_ID_1][7];     /* ���һ�ι���ʱ��ĸ�ߵ�ѹ */
    inv_err_para_info[k++] = invt_para_fp[INVT_ID_1][8];     /* ���һ�ι���ʱ��������¶� */
    inv_err_para_info[k++] = invt_para_fp[INVT_ID_1][9];     /* ���һ�ι���ʱ����Ƶ��Ϣ */
    inv_err_para_info[k++] = 
        (invt_para_fp[INVT_ID_1][10] & INVT_EER_PARA_A_IN) >> 14;    /* ���һ�ι���ʱ��������� */
    inv_err_para_info[k++] = 
        GETLOWBITS(invt_para_fp[INVT_ID_1][10],INVT_EER_PARA_V_IN);  /* ���һ�ι���ʱ�������ѹ */
    inv_err_para_info[k++] = invt_para_fp[INVT_ID_1][11];   /* �����ڶ��ι������� */
    inv_err_para_info[k++] = invt_para_fp[INVT_ID_1][12];   /* �����ڶ��ι���ʱ�ۼ�����ʱ�� */
    inv_err_para_info[k++] = invt_para_fp[INVT_ID_1][13];   /* ���������ι������� */
    inv_err_para_info[k++] = invt_para_fp[INVT_ID_1][14];   /* ���������ι���ʱ�ۼ�����ʱ�� */
    inv_err_para_info[k++] = invt_para_fp[INVT_ID_1][15];   /* �������Ĵι������� */
    inv_err_para_info[k++] = invt_para_fp[INVT_ID_1][16];   /* �������Ĵι���ʱ�ۼ�����ʱ�� */
    inv_err_para_info[k++] = invt_para_fp[INVT_ID_1][17];   /* ��������ι������� */
    inv_err_para_info[k++] = invt_para_fp[INVT_ID_1][18];   /* ��������ι���ʱ�ۼ�����ʱ�� */
    inv_err_para_info[k++] = invt_para_fp[INVT_ID_1][19];   /* ����ʱ������ʱ�� */ 

    /* BMSHMI */
    {
        ARRCLR(bmsHMI.info_bit);
        
        SET_BMSHMI_N( 0, fg_zone_A_run);		            /* A�����б�־  */    
        SET_BMSHMI_N( 1, fg_zone_B_run);		            /* B�����б�־  */    
        SET_BMSHMI_N( 2, fg_hotw_run);		                /* ��ˮ���б�־ */
        SET_BMSHMI_N( 3, fg_timed_PUMPret_run);				/* ��ʱ�»�ˮ�����б�־ */
        SET_BMSHMI_N( 4, fg_force_hotw_mode_run);			/* ǿ����ˮģʽ�������б�־ */
        SET_BMSHMI_N( 5, fg_timed_disinfect_run);			/* ��ʱɱ���������б�־ */
        SET_BMSHMI_N( 6, fg_PUMPret_disinfect_run);			/* �»�ˮ��ɱ�����б�־ */
        SET_BMSHMI_N( 7, fg_holiday_away_run);				/* �����뿪ģʽ���б�־ */
        SET_BMSHMI_N( 8, fg_holiday_home_run);				/* �����ڼ�ģʽ���б�־ */
        SET_BMSHMI_N( 9, fg_force_TBH_run);	                /* ǿ��ˮ��������б�־ */
        SET_BMSHMI_N(10, fg_force_IBH_run);	                /* ǿ�������������б�־ */
        SET_BMSHMI_N(11, fg_force_AHS_run);	                /* ǿ���ⲿ��Դ���б�־ */
        SET_BMSHMI_N(12, fg_floor_preheat_run);				/* ��ůԤ�ȹ������б�־ */
        SET_BMSHMI_N(13, fg_floor_dryup_run);               /* ��ů��ɹ������б�־ */ 
        SET_BMSHMI_N(14, fg_ability_test_run);				/* �����������б�־ */
        SET_BMSHMI_N(15, fg_refri_recy_act);				/* �շ����б�־ */    
        SET_BMSHMI_N(16, fg_silent_mode);				    /* ����ģʽ��־ */
        SET_BMSHMI_N(17, silent_mode_level);				/* ����ģʽ����0������һ��1������� */
    }
}

/*******************************************************************************
 �� ��: log_sys_err()
 �� ��: 
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_err(void)
{
    U08 i, j;
    static U16 bak_cell_error[(MAX_PCB+1)*MAX_ERR]={0};
//  LED_SET(LED_GREEN, (status_run==_RUN)?LED_ON:LED_OFF);
    
//    if ((status_run != _IDLE) && (status_run != _ALARM)) set_output(MAX_PCB, SW_RUN);
        
    fg_error = FALSE;

    for (i=0; i<val_pcb_num; i++)   /* ģ���й��� */
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
    for (j=0; j<MAX_ERR; j++)  /* ϵͳ�й��� */
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
 �� ��: log_sys_defrost()
 �� ��: 
 �� ��: ��
 �� ��: ��
 �� ��: ��
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
 �� ��: log_sys_00_comp_status()
 �� ��: ��ȡ00#ѹ��״̬
 �� ��: ��
 �� ��: ��
 �� ��: ��
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
 �� ��: log_sys_ac_hotw_comp_status()
 �� ��: ��ȡ��ˮģ��ѹ��״̬
 �� ��: ��
 �� ��: ��
 �� ��: ��
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
        BOL have_ac_hotw_comp_run = FALSE;/* TRUE/FALSE:������ˮģ��ѹ������/��ˮģ��û��ѹ������ */

        /* ������ˮģ�� */
        for (i = PCB_BGN(SUB_SYS_AC_HOTW); i < PCB_END(SUB_SYS_AC_HOTW); ++i)
        {
            /* ֻҪ��ˮģ����һ̨ѹ�����У����ǡ�������ˮģ��ѹ�����С� */
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
 �� ��: log_sys_status_indicate()
 �� ��: ϵͳ״ָ̬ʾ
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_status_indicate(void)
{
    log_sys_err();			       /* ϵͳ���� */
    log_sys_defrost();		       /* ��˪ָʾ */
//    log_sys_00_comp_status();	   /* 00#ѹ��״̬ */
    log_sys_ac_hotw_comp_status(); /* ��ˮģ��ѹ��״̬ */
}


/*******************************************************************************
 �� ��: log_sys_smart_grid()
 �� ��: ���ܵ����߼�������SG�źź�EVU�źţ���������ģʽ
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_smart_grid(void)
{
    U08 fg_use_sta_1, fg_use_sta_2, fg_use_sta_3, fg_use_sta_4;
    
    /* ��ʼ�� */
    smart_grid_cmd = SG_CMD_NULL;
    sg_power_mode = POWER_MODE_MAX;
    fg_sg_hotw_priority = FALSE;
    fg_sg_hotw_forbid = FALSE;
    smart_grid_act = SG_ACT_NULL;
    
    if (!val_smart_grid_used/* ���ܵ������� */
    || !fg_warm_complete    /* Ԥ��δ��� */
    )
    {
        smart_grid_sta = SG_STA_NULL;
        return;
    }

    /* ���ܵ���ָ�� */
    if (m_get_input(IN_SG) && !m_get_input(IN_EVU))
    {
        smart_grid_cmd = SG_CMD_1;          /* ���ܵ���ָ��1 */
    }
    else if (!m_get_input(IN_SG) && !m_get_input(IN_EVU))
    {
        smart_grid_cmd = SG_CMD_2;          /* ���ܵ���ָ��2 */
    }
    else if (!m_get_input(IN_SG) && m_get_input(IN_EVU))
    {
        smart_grid_cmd = SG_CMD_3;          /* ���ܵ���ָ��3 */
    }
    else
    {
        smart_grid_cmd = SG_CMD_4;          /* ���ܵ���ָ��4 */
    }  

    /* ���ܵ���״̬ */
    if (smart_grid_sta != SG_STA_1
    && cnt_sg_sta_off[SG_STA_1] > 10*60
    && smart_grid_cmd == SG_CMD_1
    && cnt_sg_cmd_on[SG_CMD_1] > 10*60
    )
    {
        smart_grid_sta = SG_STA_1;          /* �������ܵ���״̬1 */
    }
    else if (smart_grid_sta != SG_STA_2
    && cnt_sg_sta_off[SG_STA_2] > 60
    && smart_grid_cmd == SG_CMD_2
    && cnt_sg_cmd_on[SG_CMD_2] > 60
    )
    {
        smart_grid_sta = SG_STA_2;          /* �������ܵ���״̬2 */
    }
    else if (smart_grid_sta != SG_STA_3
    && cnt_sg_sta_off[SG_STA_3] > 10*60
    && smart_grid_cmd == SG_CMD_3
    && cnt_sg_cmd_on[SG_CMD_3] > 10*60
    )
    {
        smart_grid_sta = SG_STA_3;          /* �������ܵ���״̬3 */
    }
    else if (smart_grid_sta != SG_STA_4
    && cnt_sg_sta_off[SG_STA_4] > 10*60
    && smart_grid_cmd == SG_CMD_4
    && cnt_sg_cmd_on[SG_CMD_4] > 10*60
    )
    {
        smart_grid_sta = SG_STA_4;			/* �������ܵ���״̬4 */
    }

    /* ���ܵ������� */
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

    /* ���ܵ������� */
    if (fg_use_sta_1 && VALLEY_ELEC_FORCE_STOP)             /* �͹��õ�ǿ�ƹر� */
    {
        smart_grid_act = SG_ACT_ALL_FORCE_OFF;
        sg_power_mode = POWER_MODE_STD;  
        fg_sg_hotw_priority = FALSE;
        fg_sg_hotw_forbid = FALSE;
        SETCMD_N(VALLEY_ELEC_AC_OFF);
        SETCMD_N(VALLEY_ELEC_HOTW_OFF);
    }
    else if (fg_use_sta_2 && PEAK_ELEC_LIMIT_RUN)           /* �߷��õ��������� */
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
    else if (fg_use_sta_3 && FREE_ELEC_RUN)                 /* ����õ����� */
    {
        smart_grid_act = SG_ACT_HOTW_PRIOR_RUN;
        sg_power_mode = POWER_MODE_STR;
        fg_sg_hotw_priority = TRUE;
        fg_sg_hotw_forbid = FALSE;
    }
    else if (fg_use_sta_4 && FREE_ELEC_FORCE_RUN)           /* ����õ�ǿ������ */
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
 �� ��: log_sys_get_power_mode()
 �� ��: ��ȡ����ģʽ
 �� ��: ��
 �� ��: 
 �� ��: ��
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
������ m_comp_use_mode
���ܣ� �ж�ѹ����ʹ��ģʽ
������ ��
����ֵ�� ��
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

#define val_defrost_comp_max            (50)        /* ��˪ѹ���ٷ��� */
/*******************************************************************************
 �� ��: log_sys_get_defrost_comp_max()
 �� ��: ���ݳ�˪ѹ���ٷ�����������˪ѹ������
 �� ��: per--��˪ѹ���ٷ���; 
		to_mode--��ǰ����ģʽ; 
		type--��ϵͳ����; 
 �� ��: 
 �� ��: ��
*******************************************************************************/
U08 log_sys_get_defrost_comp_max(U16 per, U08 to_mode, U08 type)
{
    U08 x = 0;
    U08 y = 0;
    U08 result = 0;
    U08 defrost_total_num = 0;  /* ��˪ѹ������ */
    
    U16 tmp = 0;
    
    /*
     * ��������ģ����õ�ѹ������   -->   ģ���� X ��ģ��ѹ����
     * ģ����õ�ѹ����ȥ��
     * ģ�鲻�ܿ�����ѹ����ȥ�꡾����ѹ����ѹ���ϻ��ѹ���ϵȵȡ�
     */
    for (x = PCB_BGN(type); x < PCB_END(type); x++)
    {
        if (!log_sys_cell_can_run(x)) continue;        /* ������(���á�ͨѶ����) */
            
            for (y = 0; y < m_val_comp_num(x); y++)
            {
                if (log_sys_comp_mode(x, y, to_mode, SUB_SYS_AC_HOTW))    /* ѹ������ģʽ���� */
                {
                    defrost_total_num++;
                }
            }
    }
 
    /*
     * ��������˪ѹ������   ����ѹ������ X ��˪ѹ���ٷ���
     * ������ĳ�˪ѹ������Ϊ������Ӧ���ϼӣ�ʹ֮Ϊ����
     * ���磺ѹ��������Ϊ32�������ó�˪ѹ����Ϊ60%�����㵽
     *       ����ͬʱ�����˪ѹ������Ϊ19.2������ʱӦ����
     *       20��ѹ��ͬʱ�����˪
     */
    tmp = defrost_total_num * per; /* ��ֹ����С��10���� */
    if (tmp >= 10) {
        tmp = tmp / 10; /* �˴���1/10���㣬�����ٳ�10��Ϊ 1/100 */
        if ((tmp%10) > 0)   result = (U08)(tmp/10 + 1);
        else                result = (U08)(tmp/10);
    } else {
        result = 1;
    }
 
    /* ��ֹ��˪ѹ�������������ѹ������ */
    if (result > defrost_total_num) result = defrost_total_num;
    
    return result;
}

/*******************************************************************************
 �� ��: log_sys_set_warm_time_get()
 �� ��: ��ȡԤ��ʱ��
 �� ��: ��
 �� ��: Ԥ��ʱ�䣨���ӣ�
 �� ��: 
*******************************************************************************/
U16 log_sys_set_warm_time_get(U16 pd_time)
{
    U16 warm_time_m;        /* ����Ԥ��ʱ�� */

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
 �� ��: log_sys_powerdown_time()
 �� ��: �������ʱ���
 �� ��: ��
 �� ��: ��
 �� ��: ��
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
            pd_time = 0xffff;               /* ����ʱ���޷�����Ĭ��������ʱ������ʹ��Ԥ�ȹ��� */
        }
    }

    return pd_time;
}
/*******************************************************************************
 �� ��: log_sys_warm()
 �� ��: Ԥ���߼�
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_warm(void)
{
    BOOL fg_complete = FALSE;   
    static BOOL fg_warm_en = FALSE; /* Ԥ��ʹ�ܱ�־ */
    U16 pd_time = log_sys_powerdown_time();
    U16 warm_time_need = log_sys_set_warm_time_get(pd_time);
    static BOL fg_confirm = FALSE;

    /* ����̬����ʱ�˳�Ԥ�ȣ���ͬ���޸�˵���� */
    #define EXIT_WARM_ANTIFREEZE (fg_idle_sta_antifreeze && idle_sta_antifreeze_status==ANTIFREEZE_OPEN_COMP)    

    sys_warm_time_need  = warm_time_need;

    /* �������ȡ��Ԥ�� */
    if (EXIT_WARM_ANTIFREEZE)
    {
        key_warm_cancel = TRUE;
    }

    if (power_time <= POWER_ON_PREHEAT
    || SensorNoExist(pv_sys_TA)
    || !TF_IS_IDLE
    || fg_refri_recy_act	/* �շ� */
    )
    {
        fg_warm_complete = TRUE;
        return;
    }

    // Ԥ��ʹ��
    if (pv_sys_TA<val_warm_env				/* �����¶� */
    &&  warm_time_need>0	                /* ����ʱ�� */
    &&  !fg_confirm			                /* δȷ�ϱ�־����ֹ���±仯����ǹ���� */
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

    if (power_down_time_space>60 && fg_warm_complete==TRUE)/* ��ʱ����һ��ʱ�� */
    {
        power_down_time_space = 0;
        pwd_time_write_to_eeprom();
    }
}

/*******************************************************************************
 �� ��: log_sys_step_run()
 �� ��: �������й���
 �� ��: pcb_num--;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_step_run(void)
{
    /* �յ�+��ˮ��ϵͳ�������� */
    log_sys_sub_sys_ac_hotw_energy(); 

    /* ���յ���ϵͳ�������� */
    log_sys_sub_sys_ac_energy();
}

/*******************************************************************************
 �� ��: log_sys_warm_time_get()
 �� ��: ��ȡԤ��ʣ��ʱ��
 �� ��: ��
 �� ��: 
 �� ��: ��
*******************************************************************************/
U16 log_sys_warm_time_get(void)
{
    U16 warm_time = fg_warm_complete ? 0 : sys_warm_time_need-sys_warm_time;

    return warm_time;
}

/*******************************************************************************
 �� ��: log_sys_defrost_deal()
 �� ��: �����˪����
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void log_sys_defrost_deal(U08 pcb_num)
{
    U08 i;
    U08 defrost_total_num;  /* ��˪ѹ������ */
    U08 sub_sys_type;

    // ��˪����
    for (i = 0; i < m_val_comp_num(pcb_num); i++)
    {
        /* ȷ����ǰģ��������ϵͳ */
        sub_sys_type = GET_SUB_SYS(pcb_num);
        
        if (M_CM_GetStatus(pcb_num, i, COMP_DEFROST_APPLY))
        {
            defrost_total_num = log_sys_defrost_comp_num(sub_sys_type); // ��˪ģ����
            
            if (defrost_total_num < log_sys_get_defrost_comp_max(val_defrost_comp_max, ctrl_mode[sub_sys_type], sub_sys_type))
            {
                cmd_comp_allow_defrost(pcb_num, i);
            }
        }
    }
}

#ifdef M_DEBUG
/*******************************************************************************
 �� ��: log_sys_debug()
 �� ��: �����ڲ�����ʱ�鿴״̬ʹ��
 �� ��: ��
 �� ��: ��
 �� ��: ��
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
    ����: �߼�����
/==========================================*/
void log_sys(U08 pcb_num)
{   
    log_sys_time();         /* ��ŵ�begin֮ǰ����begin����ռ̵�������time��ʱ���õ��̵��� */
    log_sys_begin();
    
    if (power_time < POWER_ON_INTO_WORK) return;

    if (power_time == POWER_ON_INTO_WORK)
    {
        random_time=clock[hmi_r_num].sec;   
        random_time=(random_time%10);
        random_time=(random_time<=4) ? random_time : (random_time%5);
        random_time=random_time*2;
    }   
        
    log_sys_cmd_get(pcb_num);       //ϵͳ����

    log_sys_public_info();

    log_sys_mode_temp_confirm();			/* ģʽ�¶�ȷ�� */
    log_sys_mode_need_confirm();            /* ģʽ����ȷ�� */
    log_sys_disp_mode_confirm();            /* ��ʾģʽȷ�� */
    log_sys_ctrl_mode_confirm();            /* ����ģʽ�����ƶ���ȷ�� */
    log_sys_ctrl_mode_temp_confirm();		/* ��ǰ����ģʽ�Ŀ���&Ŀ���¶�ȷ��  */
    
    log_sys_start_and_stop();    
    log_sys_to_ready_by_together();     // ����������׼��̬
//    log_sys_two_way_valve_interlock();
         
    log_sys_cell_forbid_deal(pcb_num);  // ģ����ô���
    
    log_sys_antifreeze();	                /* ������� */
    log_sys_timed_antifreeze();				/* ��ʱ���� */
    
    log_sys_defrost_deal(pcb_num);      // ��˪����

    log_sys_warm();                     // Ԥ���ж�
    
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

    log_sys_refri_recycle();                /* �շ� */
    log_sys_TBH();                          /* ˮ������ */
    log_sys_check_force_IBH_off();          /* ���ǿ��IBH�Ƿ�ر� */
    log_sys_AHS();                          /* �ⲿ��Դ */
    log_sys_pump();                         /* ˮ�ÿ��� */
    log_sys_valve();                        /* ��ͨ������ */
    log_sys_UIRT();                         /* �¿������� */
    log_sys_timed_disinfect();              /* ɱ������ */
    log_sys_force_hotw();                   /* ǿ����ˮģʽ */
    log_sys_floor_preheat();                /* ��ůԤ�ȹ��� */
    log_sys_floor_dryup();                  /* ��ů��ɹ��� */
//    log_sys_holiday();                      /* �ݼ�ģʽ */
    log_sys_ability_test();                 /* �������� */
    log_sys_smart_grid();                   /* ���ܵ��� */
    PU_cal_power_usage();                   /* �õ���� */
    log_sys_test_run();                     /* ������ */
    log_sys_status_indicate();              /* ϵͳ״ָ̬ʾ */
    fcu_deal();                             /* ����̹� */
    log_sys_cmd_deal(pcb_num);              /* ѹ��������ģʽת�� */

    log_sys_end();                          /* ���ת�� */

#ifdef M_DEBUG
    log_sys_debug();
#endif
}

/*==========================================
    ����: ������ʼ��
/==========================================*/
void log_sys_init_work(void)
{
    U08 i;

    key_reset = TRUE;

    fg_force_TBH_run = FALSE;
    fg_force_IBH_run = FALSE;
    fg_force_AHS_run = FALSE;

    /* ˮ��ʱ�� */
    for (i=0; i<PM_MAX; i++)      /* ˮ�ø��� */
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

    log_sys_init_idle_sta_antifreeze();		/* ����̬����������ʼ�� */
    log_sys_init_run_sta_ac_antifreeze();	/* ����̬�յ�ˮ·����������ʼ�� */
    log_sys_init_run_sta_hotw_antifreeze();	/* ����̬��ˮˮ·����������ʼ�� */
    run_sta_antifreeze_stage = AF_STAGE_IDLE;
    log_sys_init_timed_antifreeze();		/* ��ʱ����������ʼ�� */
    
//    runtime_read_from_eeprom();

    Password_ReadAll(); // password_read_from_eeprom();
    
    //auto_run_read_from_eeprom();
    DataAccess_SysRuntime(DATA_READ);           /* ��ȡϵͳ����ʱ�� */
    pwd_time_read_from_eeprom();                /* ��ȡ�ϴε���ʱ�� */

//  limit_val_bak = limit_value;
}

/*******************************************************************************
 �� ��: need_comp_num_in_system()
 �� ��: ����ģ������(��/��)�����������������ѹ����
 �� ��: type--ģ������(��/��); 
 �� ��: 
 �� ��: ��
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
������ menu_work
���ܣ� ������ 
������ ��
����ֵ�� ��
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

