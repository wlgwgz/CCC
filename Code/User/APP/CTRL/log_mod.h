#ifndef LOG_MOD_H
#define LOG_MOD

#ifdef LOG_MOD_GLOBAL
#define LOG_MOD_EXT
#else
#define LOG_MOD_EXT extern
#endif

enum STEP           { CM_IDLE, CM_STARTING, CM_RUNNING, CM_DEFROSTING,  CM_STOPPING};    
enum STEP_DEFROST   { DS_IDLE, DS_READY,    DS_DEFROST, DS_QUIT};    

// -------------------------------------------------------------------------------------------------------------
// ѹ�����ݽṹ˵��--------------------------------------------------------------------------------------------
// Cm[ѹ����]:      ѹ���������    
// Cm[MAX_COMP]:    ģ���������(ģ���Ƿ���Ҫ�����ֿ��Ϻ�)          
// -------------------------------------------------------------------------------------------------------------
typedef struct tagCOMP
{
    U08 stage;          // ��������()
    U16 dly;            // ѹ����ʱ(����ʱ��������֮�����ʱ)
                        
    U08 run_mode_need;  // ��ǰ����ģʽ
    U08 run_mode;       // ��ǰ����ģʽ
                     
    U16 status;         // ѹ��״̬
    U08 fan_speed;      // ��ѹ���������   
    
    U16 defrost_stage;  // ѹ����˪����
    U16 defrost_dly;    // ѹ����˪��ʱ(��˪ʱ��������֮�����ʱ)

//    U16 rotation;     // ѹ��ת��(0.01rps) ����Ƶѹ���ã�
    
} COMP;

LOG_MOD_EXT COMP  Cm[MAX_COMP + 1];
LOG_MOD_EXT COMP *pComp;




typedef struct tagRTIME_CM      // ѹ��ʱ��ṹ
{
    U16 run;                // �ۼ�����ʱ��(��)     
    U16 on;                 // ��ǰ����ʱ��(�룬��˪�������йػ��л�ʱ�Լ�����)     
    U16 off;                // ��ǰֹͣʱ��(��)     
    U16 defrost_exit;       // ��ͬ�˳���˪ʱ��(��) 
    U16 defrost_exit_sin;   // �����˳���˪ʱ��(��) 
    
    U16 sw_on;              // ��ǰ���ʱ��(�룬�Լ̵������Ϊ׼)
    
    U16 defrost_space;      // ��˪�����ʱ         //NOTE-CXW ԭfrost_space[MAX_COMP]
//  U16 defrost_dly;        // ��˪�����м�ʱ       //NOTE-CXW ԭdefrost_time_count[MAX_COMP]
    U16 defrost_time;       // ��˪ʱ���ʱ         //NOTE-CXW ԭfrost_time[MAX_COMP]
    U16 cnt_time_fan_delay; // ������˪ǰ�������ʱ //NOTE-CXW ԭcnt_time_fan_delay[MAX_COMP]
    U08 frost_out_dly;      // ������˪�ĳ�ˮ�¶ȼ�ʱ   //NOTE-CXW ԭfrost_out_dly
    U16 defrost_quittime;
} RTIME_CM;
LOG_MOD_EXT RTIME_CM  cm_time[MAX_COMP];                //NOTE-CXW ԭcm_time[MAX_COMP + 1]

typedef struct tagRTIME // 2��ʱ��ṹ   
{
    U16 run;    // �ۼ�����ʱ��(��)     
    U16 on;     // ��ǰ����ʱ��(��)
    U16 off;    // ��ǰֹͣʱ��(��)
} RTIME;

// ��Ƶ�쳣��Ϣ 
typedef struct tagInvtEXCEPT    
{
    U16 info;       // �쳣��Ϣ�ۼƷ�����������Ϻ�
    RTC time;       // �쳣��Ϣ�������ʱ��
    I16 state[8];   // �쳣��Ϣ�������״̬:
                    // --����ת�١�˲ʱת�١������ѹ���������
                    /* --ĸ�ߵ�ѹ�������¶ȡ���ˮ�¶ȡ������¶� */
} InvtEXCEPT;
enum { INFO_200, INFO_004, INFO_007, INFO_008, 
       INFO_E02, INFO_E03, INFO_E04, INFO_E12,
       INFO_E19, INFO_E20, INFO_E31, INFO_EX1,
       INFO_EX2, INFO_EX3, INFO_EX4, INFO_EX5,
       INFO_MAX};           /* Ŀǰ���֧��16���ϵ���Ϣ����ʹ��U16ֻ��16λ */
LOG_MOD_EXT InvtEXCEPT  info_invt_except[INFO_MAX];

// ��Ƶ��������
typedef struct tagInvtPARA  
{
    I16 addr;       // ������ַ
    I16 val;        // ����ֵ
    I16 fg_rd;      // ����־
    I16 fg_wr;      // д��־             
} InvtPARA;
LOG_MOD_EXT InvtPARA  info_invt_para[INVT_ID_MAX];
#define InvtPARA_Num 4      /* InvtPARA��Ա�� */

LOG_MOD_EXT U16  CmRotation[INVT_ID_MAX];       // ��Ƶ���ת��
LOG_MOD_EXT U16  CmRotation_out[INVT_ID_MAX];   // ��Ƶ���ת��:ʵ�����

#define INVT_CM_KEEP    0   /* ���� */
#define INVT_CM_UP      1   /* ��Ƶ */
#define INVT_CM_DOWN    2   /* ��Ƶ */
LOG_MOD_EXT U08 invt_cm_sta[INVT_ID_MAX];   // ѹ����Ƶ״̬
#define CM_IS_UP(cm)    (invt_cm_sta[cm] == INVT_CM_UP)
#define CM_IS_KEEP(cm)  (invt_cm_sta[cm] == INVT_CM_KEEP)
#define CM_IS_DOWN(cm)  (invt_cm_sta[cm] == INVT_CM_DOWN)

/* ��Ƶ�����Ը�λ */
#define INVT_RST_PART       0				/* ���� */
#define INVT_RST_ALL        1				/* ȫ�� */
#define INVT_RST_NONE       2				/* ȫ�� */
#define INVT_RST_MAX        3

/* ѹ��ͣ��ʱ���ȼ���һ��ʱ�䣬��ͣ��
 * 1������ת��Ϊ0ʱ����Ƶ���ᵱ�����ת��������
 * 2��ͣ������ͺ󣬱�Ƶ���Źر�ѹ��
*/
LOG_MOD_EXT U16 invt_off_speed_down_cnt[INVT_ID_MAX];   // ��Ƶ����ͣ����ʱ
LOG_MOD_EXT U16 close_defro_heat_cnt;

LOG_MOD_EXT U16 comp_realrun_time[MAX_COMP];    /* ѹ�������������������ʱ�� */

LOG_MOD_EXT U08 rfg_corr_interval;           /* ��ý�¶�����ʱ���� */

#define FORCE_SUB_FREQ_INTERVAL (2*60)          /* ǿ�ƽ�Ƶ��� */
LOG_MOD_EXT U16 cnt_force_sub_freq[MAX_COMP];   /* ǿ�ƽ�Ƶ��ʱ */

LOG_MOD_EXT U16 cnt_no_freq_limit[MAX_COMP];

//LOG_MOD_EXT U16   defrost_time_count[MAX_COMP];   // ����˪������ʱ���ʱ
LOG_MOD_EXT U16     mod_input[MAX_COMP+1];          // ģ������
//LOG_MOD_EXT U16   cm_time_sw_on[MAX_COMP];        /* ��ǰ���ʱ��(��) ������ѹ������ѹ�����SW_CMʵ�ʽ�ͨ����*/    

LOG_MOD_EXT U16 pv_environment_stable_dly;          // �����¶�����ʱ��
LOG_MOD_EXT I16 pv_sys_environment_tmp;             // �����¶���������ֵ
LOG_MOD_EXT I16 pv_sys_environment_update;          // �����¶����������ֵ������Ҫ��ֹ�����¶Ⱦ��ұ仯�ĳ��ϣ�����̽ͷ����ʱ��
LOG_MOD_EXT U16 ReOilFrequency[INVT_ID_MAX];  
LOG_MOD_EXT U16 invt_common_run_time[INVT_ID_MAX];  // ѹ���������м�ʱ

LOG_MOD_EXT U16 cnt_invt_chg_speed;  // ��Ƶ���ʼ�ʱ

LOG_MOD_EXT BOL fg_freq_pre_sub_en;     /* ����Ԥ��Ƶ */

LOG_MOD_EXT BOL fg_freq_had_force_sub;      /* ǿ�ƽ�Ƶ�� */

//LOG_MOD_EXT U16 work_freq_update_cnt; /* ����Ƶ�ʱ���¼�ʱ */

#define REC_NORMAL_TEMP_INTV        10                  /* ��¼һ���¶ȼ�� */
#define REC_NORMAL_TEMP_NUM         10                  /* ��¼һ���¶ȸ��� */
LOG_MOD_EXT U08 mod_rec_gas_out_cnt[MAX_COMP];                      /* ��¼�����¶ȼ�ʱ */
LOG_MOD_EXT I16 mod_rec_gas_out[MAX_COMP][REC_NORMAL_TEMP_NUM];     /* ��¼�����¶Ȼ��� */
LOG_MOD_EXT I16 mod_rec_gas_out_avg[MAX_COMP];                      /* �����¶�ƽ��ֵ */

LOG_MOD_EXT I16 test_buf[10];        

#define FREQ_FAN1       0
#define FREQ_FAN2       1
#define FREQSPEEDNUM    2
LOG_MOD_EXT I16 speedsv[FREQSPEEDNUM];          //Ƶ���趨ֵ:rpm
LOG_MOD_EXT I16 speedpv[FREQSPEEDNUM];          //Ƶ�ʵ�ǰֵ/* N0004 */
LOG_MOD_EXT I16 speed_percent[FREQSPEEDNUM];    //pwm����ֵ:�ٷֱ�   

LOG_MOD_EXT U16 fan_speed_adjust_cnt;
LOG_MOD_EXT U16 cnt_pwm_fan_ctrl_period[FREQSPEEDNUM];

LOG_MOD_EXT U16 fan_restart_cnt;

//��������������
#define TYPE_INDEPENDENCE       0       /* ���� */
#define TYPE_DEPENDENCE         1       /* ���� */

/* ���������� */
#define CURR_25A                0       // 25A
#define CURR_35A                1       // 35A
#define CURR_50A                2       // 50A
#define CURR_TYPE_MAX           3

#define TEST_CURR_TYPE          CURR_50A    /* �����û��������� */

#define val_cond_type           TYPE_DEPENDENCE     // ����������(�̶�Ϊ����) 
#define val_evap_type           TYPE_DEPENDENCE

#define CM_ON                   0x0001      /* ѹ������ */
#define CM_ERR                  0x0002      /* ѹ������ */
#define CM_NEED                 0x0004      /* ѹ������ */
#define CM_QUICK_STOP           0x0008      /* ѹ����ͣ(�������ǳ�˪��ѹ���������ʱ�������) */
                                
#define CM_NEED_DEFROST         0x0010      /* ѹ����˪���� */
#define CM_DEFROST              0x0020      /* ѹ�����ڳ�˪ */
#define CM_FORCE_DEFROST        0x0040      /* ѹ��ǿ�Ƴ�˪ */
#define CM_ALLOW_FORCE_DEFROST  0x0080      /* ѹ��ǿ�Ƴ�˪�������� */
#define CM_DEFROST_ALLOW        0x0100      /* ѹ����˪���� */
#define CM_DEFROST_EXIT        0x0200       /* ѹ����˪�˳�*/
                                
#define CM_VALVE_S              0x0400      /* ��ͨ�� */
#define CM_FAN                  0x0800      /* ��� */
#define CM_VALVE_P              0x0800      /* ��Һ�� */
#define CM_DEFROST_OFF          0x2000      /* ��˪ͣ�� */

/* ���̵���ģʽ */
#define HEATER_MODE_DEFROST     0           /* ��˪������ */
#define HEATER_MODE_LOWENV      1           /* �ͻ������� */
#define HEATER_MODE_MAX         2

/* �ͻ��¸�ˮ�µ�����ƣ��Ƿ���Ҫ����ѹ������ά��ʱ�䣿 */
LOG_MOD_EXT BOL fg_low_env_high_water;                  /* �ͻ��¸�ˮ�±�־ */
#define LEHW_EVI_WAIT_TIME  60
#define LEHW_EEV_START_TIME (LEHW_EVI_WAIT_TIME+90)         
//#define LEHW_JUST_OPEN_TIME   (LEHW_EVI_WAIT_TIME+120)    /* ѹ������ȷ��ʱ�䣺����ʱ��С�ڸ�ֵ��Ϊ������ */

#define LEHW_EEV_INIT_TIME  30              /* �ͻ��¸�ˮ��������ת���ʼ���ȵ��ض�������ά��ʱ�䡱 */

LOG_MOD_EXT U16 motor_run_time[EEV_ID_MAX];     /* ��������ʱ�� */

#define CNT_AFTER_REOIL 20                      /* ���ͽ���20���ص�����ǰ���� */
LOG_MOD_EXT U16 cnt_reoil_end[MAX_COMP];        /* ���ͽ�����ʱ */

/* ��Ƶ������� *//* N0004 */
#define FREQ_FAN_SET_UNUSED             0       /* ���� */
#define FREQ_FAN_SET_PWM                1       /* ����PWM���    */
#define FREQ_FAN_SET_PWM_EX             2       /* ����PWM���    */
#define FREQ_FAN_SET_EC                 3       /* Ec������� */
#define FREQ_FAN_SET_EC_KeMao           4       /* Ec�����ó */
#define FREQ_FAN_SET_EC_ZSFN            5       /* ˳���ĸ������ */
#define FREQ_FAN_SET_MAX                6       /* ע�⣺���ӷ��ʱ��ע�������º궨���Ƿ���Ҫ���� */
/* N0004 */
/* ʹ��PWM��� */
#define USE_PWM_FAN (val_freq_fan_set==FREQ_FAN_SET_PWM\
                  || val_freq_fan_set==FREQ_FAN_SET_PWM_EX)

/* ʹ������PWM��� */
#define USE_PWM_FAN_EX (val_freq_fan_set==FREQ_FAN_SET_PWM_EX)

/* N0004 */
#define USE_EX_EC_FAN   (val_freq_fan_set != FREQ_FAN_SET_UNUSED\
                     &&  !USE_PWM_FAN\
                     &&  val_freq_fan_set != FREQ_FAN_SET_EC)


/* ʹ�ð��ձ�Ƶ���� */
#define USE_INVT         (val_inverter_used)
#define USE_INVT_FAN1    (val_freq_fan_set == FREQ_FAN_SET_EC)
#define USE_INVT_FAN2    (val_freq_fan_set == FREQ_FAN_SET_EC && val_freq_fan_num > 1)

#define USE_ECFAN1       (USE_EX_EC_FAN)
#define USE_ECFAN2       (USE_EX_EC_FAN && val_freq_fan_num > 1)

/* ��Ƶ������� */
#define FREQ_FAN_CTRL_MANUAL            0       /* �ֶ� */
#define FREQ_FAN_CTRL_ENV               1       /* �������� */
#define FREQ_FAN_CTRL_WING              2       /* �������� */
#define FREQ_FAN_CTRL_MAX               3       

/* ������������ */
#define FAN_TP_CORR_NONE                0	/* ���� */
#define FAN_TP_CORR_ALL                 1	/* ȫ�� */
#define FAN_TP_CORR_COOL                2	/* ������ */
#define FAN_TP_CORR_HEAT                3	/* ������ */
#define FAN_TP_CORR_MAX                 4
#define FAN_TP_COOL_USE     (val_Tp_fan_rpm_corr_set==FAN_TP_CORR_ALL || val_Tp_fan_rpm_corr_set==FAN_TP_CORR_COOL)
#define FAN_TP_HEAT_USE     (val_Tp_fan_rpm_corr_set==FAN_TP_CORR_ALL || val_Tp_fan_rpm_corr_set==FAN_TP_CORR_HEAT)

/* ���� */
#define FAN_NONE    0   // ����
#define FAN_LOW     1   // ���ٷ�
#define FAN_MID     2   // ���ٷ�
#define FAN_HIGH    3   // ���ٷ�

/* ����ߵ����л���ʱ����ֹ��� */
#define FAN_CHANGE_DELAY    6  // 600MS
LOG_MOD_EXT U16 fan_high_dly;
LOG_MOD_EXT U16 fan_low_dly;

// ��Ƶ�������
#define FAN_SPEED_SINGLE            0       // ���ٷ��
#define FAN_SPEED_DOUBLE_COMBO      1       // ���˫�ٷ��
#define FAN_SPEED_DOUBLE_SELF       2       // ����˫�ٷ��
#define FAN_SPEED_THREE_COMBO       3       // ������ٷ��
#define FAN_SPEED_UNUSED            4       /* ���� */
#define FAN_SPEED_MAX               5  

/* ��˪ͣ���з� */
#define DF_STOP_CM_NONE             0       /* ����     */
#define DF_STOP_CM_ALL              1       /* ����ȫͣ */
#define DF_STOP_CM_ENTER            2       /* ����˪ͣ */
#define DF_STOP_CM_EXIT             3       /* �˳�˪ͣ */
#define DF_STOP_CM_MAX              4

#define CmIsCool(cm)            (Cm[cm].run_mode == MODE_COOL)
#define CmIsHeat(cm)            (Cm[cm].run_mode == MODE_HEAT)

#define CmIsOn(cm)              (S_CM_GetStatus(cm, CM_ON))

/* �������� */
#define IBH1                        (0)
#define IBH2                        (1)
#define IBH_MAX                     (2)

/* �������ȵ�λ���� */
#define IBH_L0    0   // ����
#define IBH_L1    1   // �͵�
#define IBH_L2    2   // �е�
#define IBH_L3    3   // �ߵ�

/* �������ȵ�λ���� */
#define IBH_NONE     0   // ��
#define IBH_ONLY1    1   // IBH1
#define IBH_ONLY2    2   // IBH2
#define IBH_1AND2    3   // IBH1&IBH2

/* �����̼��ã�δ�ж�val_use_IBH���� */
#define IBH1_IS_USE  (IBH_cfg_type==IBH_ONLY1 || IBH_cfg_type==IBH_1AND2)
#define IBH2_IS_USE  (IBH_cfg_type==IBH_ONLY2 || IBH_cfg_type==IBH_1AND2)

LOG_MOD_EXT U16 IBH2_add_dly;    /* IBH2������ʱ */

/* ���������ȴ� */
LOG_MOD_EXT U08 cnt_HT_on_dly;

// DO1�Զ��� /* N0004 */
enum {
    DO1_IS_V_BYWAY,         /* ��Һ�� */
    DO1_IS_V_THREE,         /* ��ͨ�� */
    DO1_IS_MAX
};

// DO3�Զ���/* N0004 */
enum {
    DO3_IS_CM,
    DO3_IS_CRANK, 
    DO3_IS_MAX
};

// DO5�Զ���/* N0004 */
enum {
    DO5_IS_FAN2,
    DO5_IS_CM, 
    DO5_IS_MAX
};
// DO9�Զ���
enum {
    DO9_IS_V_IE,
    DO9_IS_V_DEFR, 
    DO9_IS_V_FLOOR, 
    DO9_IS_END_PUMP,
    DO9_IS_MAX
};

LOG_MOD_EXT U16 cnt_reoil_cycle;         // ��������
LOG_MOD_EXT U16 cnt_reoil_time;          // ����ʱ��

LOG_MOD_EXT BOL fg_have_fan_on;

enum {
    PUMP_IN_USED_SINGLE = 0,
    PUMP_IN_USED_DUAL,
    PUMP_IN_USED_MAX
};

/* ��Ƶˮ�ü�����в��� */
#define PUMPF_INTERVAL_ON_TIME     ( 5*QK_60)  // ˮ�ü�Ъ��ʱ��
#define PUMPF_INTERVAL_TIME        (PUMPF_INTERVAL_ON_TIME + val_PUMPf_interval_off_time)    // ˮ�ü�Ъ���ڣ�ˮ�ü�Ъ��ʱ��+ˮ�ü�Ъͣʱ����
LOG_MOD_EXT U16 cnt_PUMPf_interval_on_off;
LOG_MOD_EXT U16 mod_pump_inter_on_off_time[SUB_SYS_TYPE_MAX];      //ˮ�ü������ʱ��

#define s_get_cm_status(x, y)       (Cm[x].status & (y)  )
#define s_set_cm_status(x, y)       (Cm[x].status |= (y) )
#define s_clr_cm_status(x, y)       (Cm[x].status &= ~(y))

#define S_CM_GetStatus              s_get_cm_status
#define S_CM_SetStatus              s_set_cm_status
#define S_CM_ClrStatus              s_clr_cm_status

//#define s_get_input_first(comp_num, in_num)           (pCell->cell_input[comp_num] & (in_num))
//#define s_set_input_first(comp_num, in_num)           (pCell->cell_input[comp_num] |= (in_num))
//#define s_get_input(comp_num, in_num)             (mod_input[comp_num] & (in_num))
//#define s_set_input(comp_num, in_num)             (mod_input[comp_num] |= (in_num))

#define s_get_input_first(comp_num, in_num)         GETBIT(pCell->cell_input[comp_num], (in_num)&0x0f)
#define s_set_input_first(comp_num, in_num)         SETBIT(pCell->cell_input[comp_num], (in_num)&0x0f)
#define s_get_input(comp_num, in_num)               GETBIT(mod_input[comp_num], (in_num)&0x0f)
#define s_set_input(comp_num, in_num)               SETBIT(mod_input[comp_num], (in_num)&0x0f)

LOG_MOD_EXT BOL fg_force_IBH_off;           /* ģ��ǿ���������ȹرձ�־ */

enum
{
    CDT_FORBID_ADD_GAS,
    CDT_FORCE_SUB_GAS,
    CDT_FORCE_SUB_COOLCOND,
    CDT_FORCE_SUB_HEATCOND,
    CDT_FORCE_SUB_HEATEVAP,
    CDT_FORCE_SUB_COOLEVAP,
    CDT_FORCE_SUB_MOTOR_EVAP,
    CDT_FORCE_SUB_MOTOR_EXH_OH,
    TIME_CNT2_MAX    
};

//#define TIME_CNT2_MAX 20
LOG_MOD_EXT U16 condition_time_cnt_2[TIME_CNT2_MAX];
#define TIME_CNT3_MAX 20
LOG_MOD_EXT U16 condition_time_cnt_3[TIME_CNT3_MAX];

LOG_MOD_EXT U16 AdjustFreqPeriod;
LOG_MOD_EXT U16 para_init_dly;

LOG_MOD_EXT U16 fg_forbid_rise[MAX_COMP];
LOG_MOD_EXT U16 fg_force_drop[MAX_COMP];
//LOG_MOD_EXT U16 fg_forbid_drop[MAX_COMP];

/* ���������Ϣ */
enum
{
    FAN_ADJ_FG_HIGH,			/* �߷���ڱ�־ */
    FAN_ADJ_FG_MAX
};
LOG_MOD_EXT U16 fan_adjust_fg[MAX_COMP];
enum
{
    FAN_ADJ_TMP_BASE,			/* �������� */
    FAN_ADJ_TMP_COMP,			/* ѹ��Ƶ������ */
    FAN_ADJ_TMP_POWER,			/* ����ģʽ */
    FAN_ADJ_TMP_EXH,			/* �������� */
    FAN_ADJ_TMP_SILENT,			/* ����ģʽ */
    FAN_ADJ_TMP_MAX
};
LOG_MOD_EXT U16 fan_adjust_tmp[MAX_COMP][FAN_ADJ_TMP_MAX];

/* ����������Ϣ */
enum
{
    EEV_USAGE_SUC,			    /* �������ȶ� */
    EEV_USAGE_EXH,			    /* �������ȶ� */
    EEV_USAGE_AT,			    /* ���������ֶ�Ŀ����ȶ� */
    EEV_USAGE_MAX
};
enum
{
    EEV_ADJ_TMP_BASE,			/* ԭʼ���ȶ� */
    EEV_ADJ_TMP_COMP,			/* ���ȶ�Ƶ�ʲ��� */
    EEV_ADJ_TMP_EXH,			/* ���ȶ����²��� */
    EEV_ADJ_TMP_GB,			    /* ������� */
    EEV_ADJ_TMP_RANGE,		    /* ��Χ���� */
    EEV_ADJ_TMP_USAGE,			/* ����ʹ������ */
    EEV_ADJ_TMP_MAX
};
LOG_MOD_EXT U16 eev_adjust_tmp[MAX_COMP][EEV_ADJ_TMP_MAX];

/* ����������Ϣ */
enum
{
    EVI_USAGE_OH,			    /* ���������ȶ� */
    EVI_USAGE_OHT,			    /* �¶��������ȶ� */
    EVI_USAGE_OHP,			    /* ѹ���������ȶ� */
    EVI_USAGE_SML,			    /* ��·С�� */
    EVI_USAGE_HT,			    /* ����ʹ�� */
    EVI_USAGE_CL,			    /* ����ʹ�� */
    EVI_USAGE_AT,			    /* ���������ֶ�Ŀ����ȶ� */
    EVI_USAGE_MAX
};
enum
{
    EVI_ADJ_TMP_BASE,			/* ԭʼ���ȶ� */
    EVI_ADJ_TMP_RANGE,		    /* ��Χ���� */
    EVI_ADJ_TMP_USAGE,			/* ����ʹ������ */
    EVI_ADJ_TMP_MAX
};
LOG_MOD_EXT U16 evi_adjust_tmp[MAX_COMP][EVI_ADJ_TMP_MAX];


/* �������������ڱ�־ */
enum
{
    FORBID_EVI_SUB_EXH,         // ��ֹ��С����
    FORBID_EVI_ADD_EXH,         // ��ֹ��������
    FORBID_EVI_ADD_OVEREXH,     // ��ֹ�����������ȶ�
     
    FORBID_EVI_MAX
};
LOG_MOD_EXT U08 fg_evi_forbid[EEV_ID_MAX][FORBID_EVI_MAX];       // ��ֹ����evi���������С����־

enum
{
    FORCE_ADD_MOTOR_EXH,        // ǿ�ƿ�������
    FORCE_SUB_MOTOR_EVAP,       // ǿ�ƹط������¶�
    FORCE_SUB_MOTOR_OVEREXH,    // ǿ�ƹ�С�������ȶ�
    FORCE_ADD_MOTOR_OVEREXH,    // ǿ�ƿ����������ȶ�
    
    FORCE_MOTOR_MAX
};
LOG_MOD_EXT BOOL fg_force_motor[EEV_ID_MAX][FORCE_MOTOR_MAX];    // ǿ�ƶ�����־

enum
{
    FORBID_SUB_MOTOR_EXH,   // ��ֹ�ط�����
    FORBID_SUB_MOTOR_LP,    // ��ֹ�ط���ѹ
    FORBID_SUB_MOTOR_HP,    // ��ֹ�ط���ѹ
    FORBID_SUB_MOTOR_WING,  // ��ֹ�ط���Ƭ�¶�
    FORBID_SUB_MOTOR_MAX
};
LOG_MOD_EXT BOOL fg_forbid_motor_sub[EEV_ID_MAX][FORBID_SUB_MOTOR_MAX];// ��ֹ��С��־

enum
{
    FORBID_ADD_MOTOR_EXH,     // ��ֹ��������
    FORBID_ADD_MOTOR_EVAP,    // ��ֹ���������¶�
    FORBID_ADD_MOTOR_OVEREXH, // ��ֹ�����������ȶ�
    FORBID_ADD_MOTOR_MAX
};
LOG_MOD_EXT BOOL fg_forbid_motor_add[EEV_ID_MAX][FORBID_ADD_MOTOR_MAX];// ��ֹ�����־ 

enum
{
    EVI_OPEN_ENV,       // �����¶ȿ���
    EVI_OPEN_EXH_OH,    // �������ȿ���
    EVI_OPEN_EXH,       // �����¶ȿ���
    EVI_OPEN_RPS,       // ѹ��ת�ٿ���
    EVI_OPEN_MAX
};
LOG_MOD_EXT U08 fg_evi_allow[EEV_ID_MAX][EVI_OPEN_MAX]; // ������־

/* ���ϸ�λ��־ */
LOG_MOD_EXT BOOL fg_err_reset;              

LOG_MOD_EXT BOL fg_cell_pump_open;

/* ѹ�����ƻ��·ֶ� */
#define COMP_OFF_AREA     0           /* �ر��� */
#define COMP_RUN_AREA     1           /* ������ */
#define COMP_KEEP_AREA    2           /* ������ */
LOG_MOD_EXT U08 limit_comp_TA_area;
#define TA_FORCE_COMP_OFF                (limit_comp_TA_area==COMP_OFF_AREA)
#define TA_ALLOW_COMP_ON_OFF             (limit_comp_TA_area==COMP_RUN_AREA)


/* ���Ȼ��¹����ֶ� */
LOG_MOD_EXT U08 env_area_heat;
#ifdef USE_HT_ENV_NEW
#define val_heat_sv_chg_env1             (380)      // ����ת����1:����    
#define val_heat_sv_chg_env2             (280)      // ����ת����2     
#define val_heat_sv_chg_env3             (100)      // ����ת����3       
#define val_heat_sv_chg_env4             (30)       // ����ת����4  
#define val_heat_sv_chg_env5             (-30)      // ����ת����5   
#define val_heat_sv_chg_env6             (-90)      // ����ת����6 
#define val_heat_sv_chg_env7             (-130)     // ����ת����7  
#define val_heat_sv_chg_env8             (-180)     // ����ת����8
#define val_heat_sv_chg_env9             (-240)     // ����ת����9
#else
#define val_heat_sv_chg_env1             (380)      // ����ת����1:����    
#define val_heat_sv_chg_env2             (280)      // ����ת����2     
#define val_heat_sv_chg_env3             (120)      // ����ת����3       
#define val_heat_sv_chg_env4             (30)       // ����ת����4  
#define val_heat_sv_chg_env5             (-30)      // ����ת����5   
#define val_heat_sv_chg_env6             (-90)      // ����ת����6 
#define val_heat_sv_chg_env7             (-150)     // ����ת����7  
#define val_heat_sv_chg_env8             (-210)     // ����ת����8 
#endif

// EXDO7�Զ���
enum {
    EXDO7_IS_CRANK2,			/* ��������2 */
    EXDO7_IS_DPH2,			    /* ���̵����2 */
    EXDO7_IS_MAX
};

#if defined(M_NIUENTAI)
    #define val_EXDO7_selfdef EXDO7_IS_DPH2

    /* �ֶ�����:���̵����2 */
    #define SHC_EXDO7_MSG   msg_SHC_test_out_DPH2
    #define SHC_EXDO7_PCB   0
    #define SHC_EXDO7_CM    MAX_COMP
    #define SHC_EXDO7_DO    S_DO_DPH2
#else
    #define val_EXDO7_selfdef EXDO7_IS_CRANK2

    /* �ֶ�����:��������2 */
    #define SHC_EXDO7_MSG   msg_SHC_test_out_QZH2
    #define SHC_EXDO7_PCB   0
    #define SHC_EXDO7_CM    0
    #define SHC_EXDO7_DO    SW_crank2
#endif

/* �����ˮ�����ֶ� */
LOG_MOD_EXT U08 in_area_cool;

/* ѹ��ʹ�ý��� */
LOG_MOD_EXT I16 fg_press_use_j5;
LOG_MOD_EXT I16 fg_press_use_j6;

LOG_MOD_EXT void log_mod_init(void);        //ģ�������ʼ��
LOG_MOD_EXT void log_mod(void);
LOG_MOD_EXT void runtime_initial(void);
LOG_MOD_EXT BOOL s_cell_have_mode(U08 mode);
LOG_MOD_EXT U08 s_have_comp_defrost(void);
LOG_MOD_EXT void s_Defrost_exit_act(U08 comp_begin, U08 comp_end, U08 comp_num, U08 attrib);
//LOG_MOD_EXT void log_mod_Heater_Valve(void);
LOG_MOD_EXT void fan_comp_group_count(U08 fan_type, U08 *begin, U08 *end, U08 num);
LOG_MOD_EXT U08 log_mod_cond_type(U08 comp_num);
LOG_MOD_EXT BOOL log_mod_comp_is_run(U08 run_type);
LOG_MOD_EXT BOOL log_mod_cell_is_run(U08 run_type);
LOG_MOD_EXT I16 pwm_fan_speed_to_percent(U08 num, I16 speed_sv, I16 speed_pv);
LOG_MOD_EXT U16 log_cmp_is_reoil(U08 cm);
LOG_MOD_EXT U16 log_mod_get_invt_id_max(void);
LOG_MOD_EXT void log_mod_unit_to_comp_group(U08 n_comp, U08 *begin, U08 *end, U08 exio_no);
LOG_MOD_EXT BOOL log_mod_comp_temp_out_limit(U08 x);
LOG_MOD_EXT I16 log_mod_eev_freq(U08 en, U16 step_pv);
#endif
