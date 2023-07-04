#ifndef LOG_SYS_H
#define LOG_SYS

#ifdef LOG_SYS_GLOBAL
#define LOG_SYS_EXT
#else
#define LOG_SYS_EXT extern
#endif

/**********************************************************************************/
//#define DEBUG     1   //NOTE-CXW ע��ص�(���ź��ڿ�����������ʾ����������)
//#define TEST1     3
//#define TEST2     4

// ��ý����val_refrigerant_type
#define R22                 0   //R22�����
#define R407c               1   //R407C
#define R410A               2   //R410A


#define SWITCH_GOS          0       /* ��ý */
#define SWITCH_WATER        1       /* ˮ· */

#define IN_MACHINE          0       /* ������ */
#define IN_PROJECT          1       /* ������ */

#define AIR_WATER               0       /* ������ˮ */
#define AIR_AIR                 1       /* �����ͷ� */

#define MODE_NULL           0       /* �� */
#define MODE_COOL           1       /* ���� */
#define MODE_HEAT           2       /* ���� */
#define MODE_AUTO           3       /* �Զ�:�������Զ��л��¶�֮�� */
#define MODE_HOTW           4       /* ��ˮ */

#define CTRL_MODE_MAX       3       /* ����ģʽ��(���䡢���ȡ���ˮ) */

#define SET_COOL            1               /* �趨���� */
#define SET_HEAT            2               /* �趨���� */
#define SET_AUTO            3               /* �趨�Զ� */
#define SET_MODE_MAX        4               
//#define SET_HOT_WATER     4               /* �趨��ˮ */
//#define SET_COOL_HOT_WATER    5               /* �趨����&��ˮ */
//#define SET_HEAT_HOT_WATER    6               /* �趨����&��ˮ */
//#define SET_AUTO_HOT_WATER  7             /* �趨�Զ�&��ˮ */
//#define SET_MODE_MAX        8

#define SILENT_LEVEL_1      0
#define SILENT_LEVEL_2      1
#define SILENT_MAX          2

#define _NOTHING            0       /* �� */
#define _ADD                1       /* ���� */
#define _KEEP               2       /* ���� */
#define _SUBB               3       /* ж�� */
#define _QUICK              4       /* ��ͣ */

/* ʹ����� */
#define UNUSED              0       /* ��ʹ�� */
#define USED                1       /* ʹ�� */

/* �߿������Ʒ�ʽ */
#define UI35_ZONE_A_TWout2             (0)  /* ����ˮ�� */
#define UI35_ZONE_A_TR                 (1)  /* �������� */
#define UI35_DUAL_ZONE_TWout2          (2)  /* ˫��ˮ�� */
#define UI35_DUAL_ZONE_TWout2_TR       (3)  /* ˫��ˮ��&���� */

/* �¿������Ʒ�ʽ */
#define UIRT_UNUSED                    (0)  /* ���� */
#define UIRT_ZONE_A_MODE_SWITCHING     (1)  /* ����ģʽ�л� */
#define UIRT_ZONE_A_ON_OFF             (2)  /* �������� */
#define UIRT_DUAL_ZONE_ON_OFF          (3)  /* ˫������ */

/* �������� */
#define IBH_UNUSED                     (0)  /* ���� */
#define IBH_HEAT_ONLY                  (1)  /* ������ */
#define IBH_HOTW_ONLY                  (2)  /* ������ˮ */
#define IBH_HEAT_HOTW                  (3)  /* ����&��ˮ */

/* �ⲿ��Դ */
#define AHS_UNUSED                     (0)  /* ���� */
#define AHS_HEAT_ONLY                  (1)  /* ������ */
#define AHS_HOTW_ONLY                  (2)  /* ������ˮ */
#define AHS_HEAT_HOTW                  (3)  /* ����&��ˮ */

/* ����ˮ */
#define TEMP_IN             0       // ��ˮ
#define TEMP_OUT            1       // ��ˮ
#define TEMP_NULL           2       // �����ֳ���ˮ���޿��ƶ����л����ܣ�


//00���Դ����
#define SYS_POWER_ERR           0   //ϵͳ��Դ����
#define MOD_00_POWER_ERR        1   //00�ŵ�Դ����

/* ������� */
#define CURR_DISP           0       /* ����ʾ */
#define CURR_DISP_ALA       1       /* ��ʾ&���� */ 
#define CURR_UNUSED         2       /* ��ʹ�� */

// ����ʹ������
#define EEV_UNUSED          0       // ����
#define EEV_OVERHEAT_SUC    1       // �������ȶ�
#define EEV_OVERHEAT_EXH    2       // �������ȶ�
#define EEV_CTRL_MAX        3       /* ����֮ǰ�����µĿ��Ʒ�ʽ */

/* �������������Կ��� */
#define EEV_ABILITY_AUTO            (0)
#define EEV_ABILITY_MANUAL_STEP     (1)
#define EEV_ABILITY_MANUAL_SSH_SV   (2)
#define EEV_ABILITY_CTRL_TYPE_MAX   (3)

// ������ʹ���趨
#define EVI_UNUSED              0
#define EVI_ECO_OVERHEAT        1       // ���������ȶ�
#define EVI_EXH_OVERHEAT_TMP    2       // �¶��������ȶ�
#define EVI_EXH_OVERHEAT_PRS    3       // ѹ���������ȶ�
#define EVI_SMALL_EEV           4       // ��·С��
#define EVI_CTRL_MAX            5       /* ����֮ǰ�����µĿ��Ʒ�ʽ */

#define HEAT_MODE_ON        0       // ���ȿ���
#define COOL_MODE_ON        1       // ���俪��
#define VALVE_TYPE_MAX      2

#define NORMAL_OPEN         0       // ����
#define NORMAL_CLOSE        1       // ����

//��������������
#define EVI_PRIOR           0       // ��������
#define EEV_PRIOR			1		// ��������
#define EEV_EVI_SAME		2		// ��������ͬʱ
#define EEV_EVI_MAX			3

/* ������/���������� */
//#define TYPE_INDEPENDENCE       0       /* ���� */
//#define TYPE_DEPENDENCE_2       1       /* 2̨���� */
//#define TYPE_DEPENDENCE_3       2       /* 3̨���� */
//#define TYPE_DEPENDENCE_4       3       /* 4̨���� */

///* ��ͨ������ */
//#define LINK_LOCK               0       // ��ס
//#define LINK_UNLOCK             1       // �Ͽ�

//#ifndef USE_QUICK_TIME
//#define LINK_LAST   (60)
//#else
//#define LINK_LAST   (5)
//#endif


//LOG_SYS_EXT BOOL fg_link_en;
//LOG_SYS_EXT U08 link_status;            /* ��ͨ��״̬ */
//LOG_SYS_EXT BOOL fg_link_run;           /* ��ͨ���������״̬ */
//
//LOG_SYS_EXT BOOL fg_interlock_bak;     /*  */
//LOG_SYS_EXT U08 link_on_cnt;
//LOG_SYS_EXT U08 link_off_cnt;

// ������������
enum {
    REMOTE_UNUSED,			/* ���� */
    REMOTE_UNION_ACT,		/* ����������Ч */
    REMOTE_UNION_KEEP,		/* �����պ���Ч */
    REMOTE_ONOFF,			/* �߿ؿ��ػ� */
    REMOTE_TBH,				/* �߿�ˮ����� */
    REMOTE_AHS,				/* �߿��ⲿ��Դ */
//    REMOTE_INTERLOCK,		/* ��ͨ������ */
    REMOTE_MAX
};

#define REMOTE_IS_UNUSED        (val_remote_onoff_type==REMOTE_UNUSED)
#define REMOTE_IS_UNION_ACT     (val_remote_onoff_type==REMOTE_UNION_ACT)
#define REMOTE_IS_UNION_KEEP    (val_remote_onoff_type==REMOTE_UNION_KEEP)
#define REMOTE_IS_ONOFF         (val_remote_onoff_type==REMOTE_ONOFF)
#define REMOTE_IS_TBH           (val_remote_onoff_type==REMOTE_TBH)
#define REMOTE_IS_AHS           (val_remote_onoff_type==REMOTE_AHS)
//#define REMOTE_IS_INTERLOCK     (val_remote_onoff_type==REMOTE_INTERLOCK)

LOG_SYS_EXT BOL fg_onoff_union_act;         /* ����������Ч����������Ϊ�� */
LOG_SYS_EXT U08 cnt_LDIrm_dly;				/* �߿ؿ����źż����ʱ */

/* �߿ؿ������� */
#define TYPE_SWITCH                  0       /* ���� */
#define TYPE_PULSE                   1       /* ���� */
/* ������ͣ���� */
#define ONOFF_ALL               0           /* ���� */
#define ONOFF_REMOTE            1           /* Զ�� */
#define ONOFF_SELF              2           /* ���� */
#define ONOFF_SWITCH            3           /* �߿� */
#define ONOFF_COMMU             4           /* ���� */

#define ACT_ZONE_A        0	/* A������״̬ */
#define ACT_ZONE_B        1	/* B������״̬ */
#define ACT_HOTW          2	/* ��ˮ����״̬ */

// ����̽ͷʹ������
#define ANTI_ICE_UNUSE   0       // ����̽ͷ-����
#define ANTI_ICE_AI8     1       // ����̽ͷ-AI8
#define ANTI_ICE_AI7     2       // ����̽ͷ-AI7
#define ANTI_ICE_MAX     3

/* ���ز�����򻮷� */
#define AREA_1                  1           /* ����һ */ 
#define AREA_2                  2           /* ����� */
#define AREA_3                  3           /* ������ */
#define AREA_4                  4           /* ������ */
#define AREA_5                  5           /* ������ */
#define AREA_6                  6           /* ������ */

/* �ϵ�ʱ�䶨�� */
//#define POWER_ON_COMMU           6
#define POWER_ON_INTO_TEST       8      /* �ϵ������Գ����ֹʱ�� */
#define POWER_ON_INTO_WORK       10     /* �����߼����ƿ�ʼʱ�� */
#define POWER_ON_PREHEAT         15
#define POWER_ON_FREEZ           15
#define POWER_ON_ONOFF           15
#define POWER_ON_CLOUD_TRAS      13     // WiFi�����Ϳ�ʼʱ��
#define POWER_ON_CLOUD_RACE      20     // WiFi�����տ�ʼʱ��

#define POWER_ON_JUST           (POWER_ON_INTO_WORK+20) /* ��ʱ��ǰ��Ϊ���ϵ� */
#define POWER_DOWN_DLY          (60)                    /* �����ӳ٣�ĳЩ�����ɱ�Ƶ�����磬���ڵ����ӳ� */

#ifndef USE_QUICK_TIME
    #define COMMU_EXPT_AUTO_RESET_DLY 30
    #define COMMU_ERR_DETECT_DLY POWER_DOWN_DLY
#else
    #define COMMU_EXPT_AUTO_RESET_DLY (fg_dbg_slow?30:3)
    #define COMMU_ERR_DETECT_DLY      (fg_dbg_slow?POWER_DOWN_DLY:6)
#endif

#define INIT_INVT_RST_TIME      (POWER_ON_INTO_WORK-1)  /* ��Ƶ���ϸ�λʱ�� */
#define INIT_INVT_ERR_TIME      (POWER_ON_INTO_WORK+5)  /* ��Ƶ���ϼ��ʱ�� */

///**********************************************************************************/
///* �û�����-->�������� */
//#define  KEY_CMD_RUN              0x0001  /* �������� */
//#define  KEY_CMD_STOP             0x0002  /* �ػ����� */
//#define  KEY_CMD_RESET            0x0100  /* ���ϸ�λ���� */
//#define  KEY_CMD_SOUND_CANCEL         0x0200  /* ��������(������Ϊ��λ������) */
//#define  KEY_CMD_WARM_CANCEL      0x0400  /* ȡ��Ԥ�� */
//
///* ��������-->�������� */
//#define  KEY_CMD_USER_PARA_INIT       0x0001   /* �û�������ʼ������ */
//#define  KEY_CMD_FAC_PARA_INIT        0x0002   /* ���Ҳ�����ʼ������ */
//#define  KEY_CMD_RUNTIME_INIT     0x0100   /* �ۼ�����ʱ���ʼ������ */
//#define  KEY_CMD_SYSTIME_INIT     0x0200   /* ϵͳ����ʱ���ʼ������ */
///* ��������-->�������� */
//#define  KEY_CMD_SYS_LIMIT            0x0001  /* ʹ�����޵����� */

LOG_SYS_EXT BOOL fg_stop_all_mac;   /* ͣ����������� */
//LOG_SYS_EXT U16 sys_input_second;

LOG_SYS_EXT I16 sv_temp_energy_cool;        /* �����ܵ�Ŀ���¶�  ��λ0.1 */
LOG_SYS_EXT I16 sv_temp_energy_heat;        /* �����ܵ�Ŀ���¶�  ��λ0.1 */
LOG_SYS_EXT I16 sv_temp_energy_hotw;        /* ��ˮ�ܵ�Ŀ���¶�  ��λ0.1 */

LOG_SYS_EXT I16 ctrl_temp_energy_ac;		/* �յ��ܵ������¶�  ��λ0.1 */
LOG_SYS_EXT I16 ctrl_temp_energy_hotw;		/* ��ˮ�ܵ������¶�  ��λ0.1 */

LOG_SYS_EXT I16 sv_temp_freq_cool;			/* �����ƵĿ���¶�  ��λ0.1 */
LOG_SYS_EXT I16 sv_temp_freq_heat;			/* ���ȵ�ƵĿ���¶�  ��λ0.1 */
LOG_SYS_EXT I16 sv_temp_freq_hotw;			/* ��ˮ��ƵĿ���¶�  ��λ0.1 */

LOG_SYS_EXT I16 val_add_diff_energy_cool;	/* �����ܵ�����ƫ��  ��λ0.1 */
LOG_SYS_EXT I16 val_add_diff_energy_heat;	/* �����ܵ�����ƫ��  ��λ0.1 */
LOG_SYS_EXT I16 val_add_diff_energy_hotw;	/* ��ˮ�ܵ�����ƫ��  ��λ0.1 */

LOG_SYS_EXT I16 val_sub_diff_energy_cool;	/* �����ܵ�ж��ƫ��  ��λ0.1 */
LOG_SYS_EXT I16 val_sub_diff_energy_heat;	/* �����ܵ�ж��ƫ��  ��λ0.1 */
LOG_SYS_EXT I16 val_sub_diff_energy_hotw;	/* ��ˮ�ܵ�ж��ƫ��  ��λ0.1 */

LOG_SYS_EXT I16 ctrl_temp_energy[SUB_SYS_TYPE_MAX];         /* �ܵ������¶�  ��λ0.1 */
LOG_SYS_EXT I16 sv_temp_energy[SUB_SYS_TYPE_MAX];           /* �ܵ�Ŀ���¶�  ��λ0.1 */

//LOG_SYS_EXT I16 val_add_diff_energy[SUB_SYS_TYPE_MAX];      /* �ܵ�����ƫ��  ��λ0.1 */
LOG_SYS_EXT I16 val_sub_diff_energy[SUB_SYS_TYPE_MAX];      /* �ܵ�ж��ƫ��  ��λ0.1 */

//LOG_SYS_EXT I16 ctrl_temp_freq;             /* ��Ƶ�����¶�  ��λ0.1 */
LOG_SYS_EXT I16 sv_temp_freq[SUB_SYS_TYPE_MAX];               /* ��ƵĿ���¶�  ��λ0.1 */

enum AC_ENERGY_CTRL_TEMP_TYPE
{
    AC_NRG_CTRL_TEMP_TR          = 0,		/* �ܵ������¶�Ϊ�����¶�TR */
    AC_NRG_CTRL_TEMP_TACT1       = 1,		/* �ܵ������¶�Ϊ����ˮ���ϲ��¶�TACt1 */
    AC_NRG_CTRL_TEMP_TACT2       = 2,		/* �ܵ������¶�Ϊ����ˮ���²��¶�TACt2 */
    AC_NRG_CTRL_TEMP_TACT1_TACT2 = 3,		/* �ܵ������¶�ΪAVG(TACt1,TACt2) */
    AC_NRG_CTRL_TEMP_AC_TWOUT2   = 4,		/* �ܵ������¶�Ϊ�յ��ܳ�ˮ�¶� */
};
LOG_SYS_EXT U08 ctrl_temp_type_energy_ac;	/* �յ��ܵ������¶����� */

#define TACT1_AC_CTRL           (ctrl_temp_type_energy_ac == AC_NRG_CTRL_TEMP_TACT1)
#define TACT2_AC_CTRL           (ctrl_temp_type_energy_ac == AC_NRG_CTRL_TEMP_TACT2)
#define TACT1_TACT2_AC_CTRL     (ctrl_temp_type_energy_ac == AC_NRG_CTRL_TEMP_TACT1_TACT2)
#define AC_TWOUT2_AC_CTRL       (ctrl_temp_type_energy_ac == AC_NRG_CTRL_TEMP_AC_TWOUT2)

#define AC_NRG_CTRL_TEMP    pv_sys_TWout2_ac    /* ��������ģʽ�ܵ������¶� */

#define CTRL_SV_BY_PARA     0   /* �ɲ����趨 */
#define CTRL_SV_BY_CMPS     1   /* �ɲ����趨 */
#define CTRL_SV_BY_DUTY     2   /* ��ֵ���趨 */

//LOG_SYS_EXT U08 ctrl_sv_by;     // �����¶ȿ���ֵ         /* �����ڲ��ԣ� */

/* ����ϣ�2022-11-29���ѿ��ŵ�BMS����������ʱ��Ӻ�� */
enum
{
    ZONE_A_ON,              /* A������ */
    ZONE_A_OFF,             /* A���ػ� */  
    ZONE_B_ON,              /* B������ */
    ZONE_B_OFF,             /* B���ػ� */  
    HOTW_ON,                /* ��ˮ���� */
    HOTW_OFF,               /* ��ˮ�ػ� */

    TIMED_PUMPRET_ON,		/* �»�ˮ�ö�ʱ���� */
    TIMED_PUMPRET_OFF,		/* �»�ˮ�ö�ʱ�ر� */
    FORCE_HOTW_ON,          /* ǿ����ˮģʽ���� */
    FORCE_HOTW_OFF,         /* ǿ����ˮģʽ�ر� */
    TIMED_DISINFECT_ON,     /* ��ʱɱ�����ܿ��� */
    TIMED_DISINFECT_OFF,    /* ��ʱɱ�����ܹر� */
    PUMPRET_DISINFECT_ON,	/* �»�ˮ��ɱ������ */
    PUMPRET_DISINFECT_OFF,	/* �»�ˮ��ɱ���ر� */

    HOLIDAY_AWAY_ON,		/* �����뿪ģʽ���� */
    HOLIDAY_AWAY_OFF,		/* �����뿪ģʽ�ر� */
    HOLIDAY_HOME_ON,		/* �����ڼ�ģʽ���� */
    HOLIDAY_HOME_OFF,		/* �����ڼ�ģʽ�ر� */
    
    FORCE_TBH_ON,           /* ǿ��ˮ����ȿ��� */
    FORCE_TBH_OFF,          /* ǿ��ˮ����ȹر� */
    FORCE_IBH_ON,           /* ǿ���������ȿ��� */
    FORCE_IBH_OFF,          /* ǿ���������ȹر� */
    FORCE_AHS_ON,           /* ǿ���ⲿ��Դ���� */
    FORCE_AHS_OFF,          /* ǿ���ⲿ��Դ�ر� */

    FREE_ELEC_FORCE_HOTW_ON,/* ����õ�ǿ����ˮ���� */
    PEAK_ELEC_HOTW_OFF,     /* �߷��õ���ˮ�ػ� */
    PEAK_ELEC_AC_OFF,       /* �߷��õ�յ��ػ� */

    FLOOR_PREHEAT_ON,		/* ��ůԤ�ȹ��ܿ��� */
    FLOOR_PREHEAT_OFF,		/* ��ůԤ�ȹ��ܹر� */
    FLOOR_DRYUP_ON,			/* ��ů��ɹ��ܿ��� */
    FLOOR_DRYUP_OFF,		/* ��ů��ɹ��ܹر� */
    
    TEST_RUN_AIR_EMPTY_ON,  /* �����ſ������п��� */
    TEST_RUN_AIR_EMPTY_OFF, /* �����ſ������йر� */
    TEST_RUN_PUMP_ON,       /* ˮ�������п��� */
    TEST_RUN_PUMP_OFF,      /* ˮ�������йر� */
    TEST_RUN_COOL_MODE_ON,  /* ���������п��� */
    TEST_RUN_COOL_MODE_OFF, /* ���������йر� */
    TEST_RUN_HEAT_MODE_ON,  /* ���������п��� */
    TEST_RUN_HEAT_MODE_OFF, /* ���������йر� */
    TEST_RUN_HEAT_IBH_ON,	/* ���������и������ȿ��� */
    TEST_RUN_HEAT_IBH_OFF,	/* ���������и������ȹر� */
    TEST_RUN_HOTW_MODE_ON,  /* ����ˮ�����п��� */
    TEST_RUN_HOTW_MODE_OFF, /* ����ˮ�����йر� */
    TEST_RUN_HOTW_TBH_ON,	/* ����ˮ������ˮ����ȿ��� */
    TEST_RUN_HOTW_TBH_OFF,	/* ����ˮ������ˮ����ȹر� */

    ABILITY_TEST_ON,		/* �������Կ��� */
    ABILITY_TEST_OFF,		/* �������Թر� */

    REFRI_RECY_ON,		    /* �շ����� */
    REFRI_RECY_OFF,		    /* �շ��ر� */

    SILENT_MODE_ON,		    /* ����ģʽ���� */
    SILENT_MODE_OFF,		/* ����ģʽ�ر� */
    SILENT_MODE_L1,		    /* ��������һ */
    SILENT_MODE_L2,		    /* ��������� */

    HOLIDAY_AWAY_HEAT_ON,	/* �����������ģʽʹ�� */
    HOLIDAY_AWAY_HEAT_OFF,	/* �����������ģʽ���� */
    HOLIDAY_AWAY_HOTW_ON,	/* ���������ˮģʽʹ�� */
    HOLIDAY_AWAY_HOTW_OFF,	/* ���������ˮģʽ���� */

    TEST_DO_ON,		        /* �ֶ����Կ��� */
    TEST_DO_OFF,		    /* �ֶ����Թر� */

    VALLEY_ELEC_AC_OFF,     /* �͹��õ�յ��ػ� */
    VALLEY_ELEC_HOTW_OFF,   /* �͹��õ���ˮ�ػ� */

    WARM_CMD_CANCEL,		/* ȡ��Ԥ�� */
    WARM_CMD_QUICK,		    /* ����Ԥ�� */
    
    CMD_MAX,
};
LOG_SYS_EXT U16 cmd_mux[_DIVUCARRY(CMD_MAX, 16)];
#define GETCMD_N(cmd_id)              (log_sys_get_cmd_mux_bit(cmd_id))
#define SETCMD_N(cmd_id)              do{                                \
                                          if (cmd_id < CMD_MAX)          \
                                          {                              \
                                              SETBIT_N(cmd_mux, cmd_id); \
                                          }                              \
                                      }while(0)

LOG_SYS_EXT BOL fg_zone_A_on;				/* A��������־ */
LOG_SYS_EXT BOL fg_zone_A_off;				/* A���ػ���־ */
LOG_SYS_EXT BOL fg_zone_B_on;				/* B��������־ */
LOG_SYS_EXT BOL fg_zone_B_off;				/* B���ػ���־ */
LOG_SYS_EXT BOL fg_hotw_on;	                /* ��ˮ������־ */
LOG_SYS_EXT BOL fg_hotw_off;                /* ��ˮ�ػ���־ */

LOG_SYS_EXT BOL fg_zone_A_run;				/* A�����б�־ */
LOG_SYS_EXT BOL fg_zone_B_run;				/* B�����б�־ */
LOG_SYS_EXT BOL fg_hotw_run;				/* ��ˮ���б�־ */

LOG_SYS_EXT BOL fg_force_hotw_mode_run;		/* ǿ����ˮģʽ�������б�־ */
LOG_SYS_EXT BOL fg_timed_disinfect_run;		/* ��ʱɱ���������б�־ */
LOG_SYS_EXT BOL fg_floor_preheat_run;		/* ��ůԤ�ȹ������б�־ */
LOG_SYS_EXT BOL fg_floor_dryup_run;			/* ��ů��ɹ������б�־ */
LOG_SYS_EXT BOL fg_holiday_away_run;		/* �����뿪ģʽ���б�־ */
LOG_SYS_EXT BOL fg_holiday_home_run;		/* �����ڼ�ģʽ���б�־ */

LOG_SYS_EXT BOL fg_force_TBH_run;			/* ǿ��ˮ��������б�־ */
LOG_SYS_EXT BOL fg_force_IBH_run;			/* ǿ�������������б�־ */
LOG_SYS_EXT BOL fg_force_AHS_run;			/* ǿ���ⲿ��Դ���б�־ */

LOG_SYS_EXT BOL fg_TBH_run;                 /* ˮ��������б�־ */
LOG_SYS_EXT BOL fg_AHS_run;                 /* �ⲿ��Դ���б�־ */
LOG_SYS_EXT BOL fg_timed_PUMPret_run;       /* ��ʱ�»�ˮ�����б�־ */
LOG_SYS_EXT BOL fg_PUMPret_disinfect_run;	/* �»�ˮ��ɱ�����б�־ */

LOG_SYS_EXT BOL fg_ability_test_run;		/* �����������б�־ */

LOG_SYS_EXT BOL fg_test_run_heat_IBH_run;	/* ���������и����������б�־ */
LOG_SYS_EXT BOL fg_test_run_hotw_TBH_run;	/* ����ˮ������ˮ��������б�־ */

LOG_SYS_EXT BOL fg_holiday_away_heat_mode_used;				/* �����������ģʽʹ�� */
LOG_SYS_EXT BOL fg_holiday_away_hotw_mode_used;				/* ���������ˮģʽʹ�� */

/* ��ʱɱ���׶�2������Ӧ������Դ��������1 */
LOG_SYS_EXT BOL fg_timed_disinfect_TBH_need;/* ��ʱɱ��ˮ����������־ */
LOG_SYS_EXT BOL fg_timed_disinfect_IBH_need;/* ��ʱɱ���������������־ */
LOG_SYS_EXT BOL fg_timed_disinfect_AHS_need;/* ��ʱɱ���ⲿ��Դ�����־ */

LOG_SYS_EXT BOL fg_timed_disinfect_AHS_on;	/* ��ʱɱ���ⲿ��Դ������־ */

LOG_SYS_EXT U16 msg_sta_S3V1;               /* �����ͨ��1״̬�ַ��� */
LOG_SYS_EXT U16 msg_sta_S3V2;               /* �����ͨ��2״̬�ַ��� */
LOG_SYS_EXT U16 msg_sta_S3V3;               /* �����ͨ��3״̬�ַ��� */

LOG_SYS_EXT U16 fg_S3V1_is_hw;				/* �����ͨ��1Ϊ��ˮ�� */

LOG_SYS_EXT U16 cnt_S3V3_ext_circue_s;      /* �����ͨ��3��ѭ��ʱ�� */
LOG_SYS_EXT U16 cnt_S3V3_in_circue_s;       /* �����ͨ��3��ѭ��ʱ�� */
LOG_SYS_EXT U16 S3V3_in_circue_min_time;    /* ��ѭ����Сʱ�� */

enum
{
    WATER_TEMP_CTRL,
    ROOM_TEMP_CTRL,
};
LOG_SYS_EXT U08 temp_ctrl_type_zone_A_cool;
LOG_SYS_EXT U08 temp_ctrl_type_zone_B_cool;
LOG_SYS_EXT BOL temp_set_can_edit_zone_A_cool;
LOG_SYS_EXT BOL temp_set_can_edit_zone_B_cool;
LOG_SYS_EXT BOL fg_sv_temp_energy_overwrite_cool;			/* �����ܵ�Ŀ���¶ȱ���д��־ */

LOG_SYS_EXT U08 temp_ctrl_type_zone_A_heat;
LOG_SYS_EXT U08 temp_ctrl_type_zone_B_heat;
LOG_SYS_EXT BOL temp_set_can_edit_zone_A_heat;
LOG_SYS_EXT BOL temp_set_can_edit_zone_B_heat;
LOG_SYS_EXT BOL fg_sv_temp_energy_overwrite_heat;			/* �����ܵ�Ŀ���¶ȱ���д��־ */

LOG_SYS_EXT U08 temp_ctrl_type_zone_A;
LOG_SYS_EXT U08 temp_ctrl_type_zone_B;
LOG_SYS_EXT BOL temp_set_can_edit_zone_A;
LOG_SYS_EXT BOL temp_set_can_edit_zone_B;
LOG_SYS_EXT BOL fg_sv_temp_energy_overwrite_ac;             /* �յ��ܵ�Ŀ���¶ȱ���д��־ */
LOG_SYS_EXT BOL fg_sv_temp_energy_overwrite_hotw;			/* ��ˮ�ܵ�Ŀ���¶ȱ���д��־ */

LOG_SYS_EXT I16 temp_sv_disp_zone_A_cool;
LOG_SYS_EXT I16 temp_sv_disp_zone_B_cool;
LOG_SYS_EXT I16 temp_sv_disp_zone_A_heat;
LOG_SYS_EXT I16 temp_sv_disp_zone_B_heat;

LOG_SYS_EXT BOL fg_enter_test_run;			/* ���������б�־ */
LOG_SYS_EXT BOL fg_air_empty_test_run;		/* ��������ſ������� */
LOG_SYS_EXT BOL fg_pump_test_run;		    /* ����ˮ�����������б�־ */
LOG_SYS_EXT BOL fg_test_run_check_LDIw;		/* �����м��ˮ�����ر�־ */
LOG_SYS_EXT BOL fg_test_run_PUMPf_off;		/* �����б�Ƶˮ�ùرձ�־ */
LOG_SYS_EXT BOL fg_can_enter_test_run;
LOG_SYS_EXT U08 fg_test_run_cur;			/* ��ǰ������ */

enum
{
    AC_ENRG_CTRL_ZONE_A_ROOM,				/* ����ˮ���ܵ� */
    AC_ENRG_CTRL_ZONE_A_WATER,				/* ����ˮ���ܵ� */
    AC_ENRG_CTRL_DUAL_ZONE_WATER,			/* ˫��ˮ���ܵ� */
    AC_ENRG_CTRL_DUAL_ZONE_HYBRID,			/* ˫������ܵ� */
    AC_ENRG_CTRL_MAX,    
};
LOG_SYS_EXT U08 ac_energy_ctrl_type;

//#define ZONE_A        0
//#define ZONE_B        1
//#define ZONE_MAX      2
//
//typedef struct tag_ZONE_STATUS_FG
//{
//    U08 terminal_type        : 3; // ����ĩ������
//    U08 zone_on              : 1; // �����Ƿ�������״̬
//    U08 room_temp_ctrl       : 1; // �����Ƿ�Ϊ�����¶ȿ��� 
//
//    U08 weather_curve        : 1; // �Ƿ�ʹ�û���-Ŀ���¶�����
//    U08 comp_on              : 1; // ѹ���Ƿ��ڿ���״̬
//    U08 sta_antifreeze       : 1; // �Ƿ��ڷ���״̬
//    U08 sta_defrost          : 1; // �Ƿ��ڳ�˪״̬
//
//    U08 sta_holiday          : 1; // �Ƿ��ڼ���״̬
//    U08 sta_silent           : 1; // �Ƿ��ھ���״̬
//    U08 sta_eco              : 1; // �Ƿ��ڽ���״̬
//    U08 active_ahs           : 1; // �ⲿ��Դ�Ƿ�����
//
//    U08 active_ibh           : 1; // ���������Ƿ�����
//    U08 active_solar         : 1; // ̫�����Ƿ�����
//    U08 active_tbh           : 1; // ˮ������Ƿ�����
//    
//    U08 
//} ZONESTA_FG;
//
//typedef struct tag_DHW_STATUS_FG
//{
//    U08 dhw_on               : 1; // ��ˮ�Ƿ�������״̬
//    U08 sta_disinfect        : 1; // �Ƿ���ɱ��״̬
//    U08 comp_on              : 1; // ѹ���Ƿ��ڿ���״̬
//    
//} DHWSTA_FG;
//
//LOG_SYS_EXT ZONESTA_FG zone_fg[ZONE_MAX];

LOG_SYS_EXT BOOL commu_run;
LOG_SYS_EXT BOOL commu_stop;
LOG_SYS_EXT BOOL key_reset;
LOG_SYS_EXT BOOL key_warm_cancel;
LOG_SYS_EXT BOOL key_sound_cancel;
LOG_SYS_EXT BOOL key_user_para_init;
LOG_SYS_EXT BOOL key_fac_para_init;
LOG_SYS_EXT BOOL key_fac_limit_init;

LOG_SYS_EXT BOOL key_fac_invt_init;
LOG_SYS_EXT BOOL key_fac_enter_ability_test;
LOG_SYS_EXT BOOL key_runtime_init;
LOG_SYS_EXT BOOL key_systime_init;
//LOG_SYS_EXT BOOL key_sys_limit;
LOG_SYS_EXT BOOL key_alldata_init;
LOG_SYS_EXT BOOL key_password_init;


/**********************************************************************************/




/**********************************************************************************/
/* cell[].cell_status[]����ģ����д��ʱ������ʹ�ã���Ϊ��ͬ���ӷ��������ȡ�ӻ�״̬������ʱ�����
   д�����������ͺ�д�� */
#define M_CM_GetStatus(pcb_num, comp_num, attrib)    (cell[pcb_num].cell_status[comp_num] & (attrib))
#define M_CM_SetStatus(pcb_num, comp_num, attrib)    (cell[pcb_num].cell_status[comp_num] |= (attrib))
#define M_CM_ClrStatus(pcb_num, comp_num, attrib)    (cell[pcb_num].cell_status[comp_num] &= ~(attrib))

#define m_get_output(mod, cm, n)                     (GET_BIT(cell[mod].cell_relay[cm], (n)% RLY_BIT_NUM))
#define m_set_output(mod, cm, n)                     (SET_BIT(cell[mod].cell_relay[cm], (n)% RLY_BIT_NUM))
#define m_clr_output(mod, cm, n)                     (CLR_BIT(cell[mod].cell_relay[cm], (n)% RLY_BIT_NUM))

#define CELL_Is_used(x)     (*(&val_cell_enable_00+(x))==USED)          /* ��Ԫʹ�� */
/* ģ���ʹ����ˮ */
#define CELL_USE_HOTW(pcb_num)                      (M_CM_GetStatus(pcb_num, MAX_COMP, CELL_CAN_RUN_HOTW))
/* ģ��ǿ���������ȿɹر� */
#define CELL_FORCE_IBH_CAN_OFF(pcb_num)             (M_CM_GetStatus(pcb_num, MAX_COMP, CELL_FORCE_IBH_OFF))

/**********************************************************************************/
enum STATUS_RUN { _IDLE, _READY, _RUN, _DELAY, _ALARM };
LOG_SYS_EXT U08 status_run;         /* ����̬ */

LOG_SYS_EXT U16 sys_input;          /* ϵͳ���� */

//LOG_SYS_EXT U16 relay_output;

LOG_SYS_EXT U08 auto_run_bit;

LOG_SYS_EXT BOL fg_refri_recy_act;              /* �շ���־ */
LOG_SYS_EXT U16 cmd_refri_recy_enter;           /* �����շ� */
LOG_SYS_EXT BOL cmd_refri_recy_stop;            /* �˳��շ� */
LOG_SYS_EXT U16 cnt_refri_recy_lp[MAX_COMP];    /* ��ѹ���շ���ʱ */

LOG_SYS_EXT BOL fg_pump_on[SUB_SYS_TYPE_MAX];               /* ��ϵͳ��Ƶˮ�ÿ�����־ */
LOG_SYS_EXT BOL fg_pump_inter_on[SUB_SYS_TYPE_MAX];         /* ��ϵͳ��Ƶˮ�ü�Ъ����־ */

enum ANTIFREEZE_TYPE
{
    AF_IDLE_STA,
    AF_RUN_STA_AC,
    AF_RUN_STA_HOTW,
    AF_TYPE_MAX,
};

enum ANTIFREEZE_STAGE
{
    AF_STAGE_IDLE,          /* ���� */
    AF_STAGE_AC,            /* �յ�ˮ·�����׶� */
    AF_STAGE_HOTW,          /* ��ˮˮ·�����׶� */
};
LOG_SYS_EXT U08 idle_sta_antifreeze_stage;  /* ����̬�����׶� */
LOG_SYS_EXT U08 run_sta_antifreeze_stage;   /* ����̬�����׶� */

#define GET_AF_STAGE(stage)         ((idle_sta_antifreeze_stage==stage) || (run_sta_antifreeze_stage==stage))

LOG_SYS_EXT BOL fg_idle_sta_antifreeze;                     /* ����̬������־ */
LOG_SYS_EXT BOL fg_idle_sta_antifreeze_pump_need;           /* ����̬������ˮ������ */
LOG_SYS_EXT BOL fg_idle_sta_antifreeze_IBH_need;            /* ����̬������������������ */
LOG_SYS_EXT BOL fg_idle_sta_antifreeze_COMPf_need;			/* ����̬������ѹ������ */

LOG_SYS_EXT BOL fg_run_sta_ac_antifreeze;                   /* ����̬�յ�������־ */
LOG_SYS_EXT BOL fg_run_sta_ac_antifreeze_pump_need;         /* ����̬�յ�������ˮ������ */
LOG_SYS_EXT BOL fg_run_sta_ac_antifreeze_IBH_need;          /* ����̬�յ������������������� */
LOG_SYS_EXT BOL fg_run_sta_ac_antifreeze_COMPf_need;        /* ����̬�յ�������ѹ������ */

LOG_SYS_EXT BOL fg_run_sta_hotw_antifreeze;                 /* ����̬��ˮ������־ */
LOG_SYS_EXT BOL fg_run_sta_hotw_antifreeze_pump_need;       /* ����̬��ˮ������ˮ������ */
LOG_SYS_EXT BOL fg_run_sta_hotw_antifreeze_IBH_need;        /* ����̬��ˮ������������������ */
LOG_SYS_EXT BOL fg_run_sta_hotw_antifreeze_COMPf_need;      /* ����̬������ѹ������ */

/* ����̬���� �� ����̬�յ�ˮ·���� */
#define FG_IDLE_OR_AC_ANTIFREEZE                   (fg_idle_sta_antifreeze || fg_run_sta_ac_antifreeze)
/* ����̬���� �� ����̬��ˮˮ·���� */
#define FG_IDLE_OR_HOTW_ANTIFREEZE                 (fg_idle_sta_antifreeze || fg_run_sta_hotw_antifreeze)

/* ģ�鴦������̬�յ�ˮ·���� */
#define MOD_IS_RUN_STA_AC_AF(pcb_num)      ((GET_SUB_SYS(pcb_num) == SUB_SYS_AC_ONLY) && fg_run_sta_ac_antifreeze)
/* ģ�鴦������̬��ˮˮ·���� */
#define MOD_IS_RUN_STA_HOTW_AF(pcb_num)    ((GET_SUB_SYS(pcb_num) == SUB_SYS_AC_HOTW) && fg_run_sta_hotw_antifreeze)

/* A����ʱ���� */
LOG_SYS_EXT BOL fg_zone_A_timed_antifreeze;                 /* A����ʱ���� */
LOG_SYS_EXT BOL fg_zone_A_timed_antifreeze_PUMPo_need;      /* A����ʱ��������ѭ�������� */
LOG_SYS_EXT BOL fg_zone_A_timed_antifreeze_S3V1_ON_need;    /* A����ʱ���������ͨ��1����ON������� */
LOG_SYS_EXT BOL fg_zone_A_timed_antifreeze_COMPf_need;      /* A����ʱ������Ƶѹ������ */

/* B����ʱ���� */
LOG_SYS_EXT BOL fg_zone_B_timed_antifreeze;                 /* B����ʱ���� */
LOG_SYS_EXT BOL fg_zone_B_timed_antifreeze_PUMPm_need;      /* B����ʱ���������ˮ������ */
LOG_SYS_EXT BOL fg_zone_B_timed_antifreeze_S3V3_need;		/* B����ʱ���������ͨ��3��OFF������� */
LOG_SYS_EXT BOL fg_zone_B_timed_antifreeze_S3V1_ON_need;    /* B����ʱ���������ͨ��1����ON������� */
LOG_SYS_EXT BOL fg_zone_B_timed_antifreeze_COMPf_need;      /* B����ʱ������Ƶѹ������ */

/* ��ˮ��ʱ���� */
LOG_SYS_EXT BOL fg_hotw_timed_antifreeze;                   /* ��ˮ��ʱ���� */
LOG_SYS_EXT BOL fg_hotw_timed_antifreeze_S3V1_OFF_need;     /* ��ˮ��ʱ���������ͨ��1����OFF������� */
LOG_SYS_EXT BOL fg_hotw_timed_antifreeze_COMPf_need;        /* ��ˮ��ʱ������Ƶѹ������ */

/* A����ʱ���� */
#define ZONE_A_TAF            (fg_zone_A_timed_antifreeze_PUMPo_need || fg_zone_A_timed_antifreeze)
/* B����ʱ���� */
#define ZONE_B_TAF            (fg_zone_B_timed_antifreeze_PUMPm_need || fg_zone_B_timed_antifreeze)
/* ��ˮ��ʱ���� */
#define HOTW_TAF              (fg_hotw_timed_antifreeze_S3V1_OFF_need || fg_hotw_timed_antifreeze)

/* A������ */
#define ZONE_A_AF             (ZONE_A_TAF || GET_AF_STAGE(AF_STAGE_AC))
/* B������ */
#define ZONE_B_AF             (ZONE_B_TAF || GET_AF_STAGE(AF_STAGE_AC))
/* ��ˮ���� */
#define HOTW_AF               (HOTW_TAF || GET_AF_STAGE(AF_STAGE_HOTW))

/* ������ƵĿ���¶� */
#define AF_SV_TEMP_FREQ       (MAX(val_anti_comp_off+50, 300))
#define AF_HOTW_ENTER_TEMP    (val_anti_comp_off+50)

/* ������־���� */
#define FG_AF_ALL (fg_idle_sta_antifreeze           \
                   || fg_run_sta_ac_antifreeze      \
                   || fg_run_sta_hotw_antifreeze    \
                   || ZONE_A_TAF                    \
                   || ZONE_B_TAF                    \
                   || HOTW_TAF                      \
                   )

/* ˮ�õ��� */
LOG_SYS_EXT U08 debug_mode; /* ˮ�õ���ģʽ */

/* �����ſ������� */
enum
{
    TEST_RUN_AIR_EMPTY,     /* �����ſ������� */
    TEST_RUN_PUMP,          /* ˮ�������� */
    TEST_RUN_COOL_MODE,     /* ���������� */
    TEST_RUN_HEAT_MODE,     /* ���������� */
    TEST_RUN_HOTW_MODE,     /* ����ˮ������ */
    TEST_RUN_MAX,
};
LOG_SYS_EXT U16 fg_test_run[_DIVUCARRY(TEST_RUN_MAX, 16)];  /* ���������б�־ */
LOG_SYS_EXT U16 cnt_test_run[TEST_RUN_MAX];                 /* �����м�ʱ */
#define cnt_test_run_air_empty              (cnt_test_run[TEST_RUN_AIR_EMPTY])
#define cnt_test_run_pump                   (cnt_test_run[TEST_RUN_PUMP])

enum
{
    TEST_AIR_EMPTY_S3V1_ON, /* ��S3V1 */
    TEST_AIR_EMPTY_S3V2_ON, /* ��S3V2 */
    TEST_AIR_EMPTY_S3V3_ON,	/* ��S3V3 */
    TEST_AIR_EMPTY_PUMPF_ON,/* ��PUMPf */
    TEST_AIR_EMPTY_PUMPO_ON,/* ��PUMPo */
    TEST_AIR_EMPTY_PUMPM_ON,/* ��PUMPm */
    TEST_AIR_EMPTY_MAX,
};
LOG_SYS_EXT U16 fg_test_air_empty_on[_DIVUCARRY(TEST_AIR_EMPTY_MAX, 16)];   /* �����ſ������п�����־ */

enum
{
    TEST_PUMP_S3V1_ON,      /* ��S3V1 */
    TEST_PUMP_S3V2_ON,      /* ��S3V2 */
    TEST_PUMP_S3V3_ON,      /* ��S3V2 */
    TEST_PUMP_PUMPF_ON,     /* ��PUMPf */
    TEST_PUMP_PUMPO_ON,     /* ��PUMPo */
    TEST_PUMP_PUMPM_ON,     /* ��PUMPm */
    TEST_PUMP_CHECK_LDIW,   /* ���LDIw */
    TEST_PUMP_MAX,
};
LOG_SYS_EXT U16 fg_test_pump_on[_DIVUCARRY(TEST_PUMP_MAX, 16)];     /* ˮ�������п�����־ */

#define TEST_RUN_COOL_MODE_SV_COOL           (70)           /* �����������ܵ�Ŀ���¶� */
#define TEST_RUN_HEAT_MODE_SV_HEAT           (350)          /* �����������ܵ�Ŀ���¶� */
#define TEST_RUN_HOTW_MODE_SV_HOTW           (550)          /* ����ˮ�������ܵ�Ŀ���¶� */

LOG_SYS_EXT BOOL fg_machine_need_open;      /* ������Ҫ����(ͣ��ʱ�����ͷ���ʱ����ʹ��) */

/* ������log_mod��err_mod��ʹ�� */
LOG_SYS_EXT U08 ac_mode;                                    /* �յ�����ģʽ(�����A����B��) */
LOG_SYS_EXT U08 disp_mode[SUB_SYS_TYPE_MAX];                /* ��ǰ��ʾģʽ */
LOG_SYS_EXT U08 ctrl_mode[SUB_SYS_TYPE_MAX];                /* ��ǰ����ģʽ */
LOG_SYS_EXT U08 ctrl_mode_dly[SUB_SYS_TYPE_MAX];            /* ��ǰģʽȷ����ʱ */

/* ������ʾ�õ�ģʽ�ж� */
#define DISP_IS_COOL(type)                  (disp_mode[type] == MODE_COOL)	/* ��ǰ�Ƿ���ʾ���� */
#define DISP_IS_HEAT(type)                  (disp_mode[type] == MODE_HEAT)	/* ��ǰ�Ƿ���ʾ���� */
#define DISP_IS_HOTW(type)                  (disp_mode[type] == MODE_HOTW)	/* ��ǰ�Ƿ���ʾ��ˮ */

/* ��ʵ�ʿ����õ�ģʽ�ж� */
#define MODE_IS_COOL(type)                  (ctrl_mode[type] == MODE_COOL)  /* ��ǰ�����Ƿ�Ϊ���� */
#define MODE_IS_HEAT(type)                  (ctrl_mode[type] == MODE_HEAT)  /* ��ǰ�����Ƿ�Ϊ���� */
#define MODE_IS_HOTW(type)                  (ctrl_mode[type] == MODE_HOTW)  /* ��ǰ�����Ƿ�Ϊ��ˮ */
#define MODE_IS_AC(type)                    (MODE_IS_COOL(type) || MODE_IS_HEAT(type))

/* ģʽ���� */
#define RUN_COOL(type)                      (STEP_IS_RUN && MODE_IS_COOL(type))	                        /* �������� */
#define RUN_HEAT(type)                      (STEP_IS_RUN && MODE_IS_HEAT(type))		                    /* �������� */
#define RUN_AC(type)                        (STEP_IS_RUN && (MODE_IS_COOL(type)||MODE_IS_HEAT(type)))	/* �յ����� */
#define RUN_HOTW(type)                      (STEP_IS_RUN && MODE_IS_HOTW(type))	                        /* ����ˮ���� */

/* �ܵ������¶��������� */
LOG_SYS_EXT U08 cool_area;                  /* ���� */
LOG_SYS_EXT U08 heat_area;                  /* ���� */
LOG_SYS_EXT U08 hotw_area;                  /* ��ˮ */

/* ģʽ�����־ */
LOG_SYS_EXT BOL fg_cool_need;               /* �������� */
LOG_SYS_EXT BOL fg_heat_need;               /* �������� */
LOG_SYS_EXT BOL fg_hotw_need;               /* ����ˮ���� */

/* �յ����� */
#define FG_AC_NEED                          (fg_cool_need || fg_heat_need)

/* ģʽ����ʱ�� */
LOG_SYS_EXT RTIME cool_runtime[SUB_SYS_TYPE_MAX];				/* ��������ʱ�� */
LOG_SYS_EXT RTIME heat_runtime[SUB_SYS_TYPE_MAX];				/* ��������ʱ�� */
LOG_SYS_EXT RTIME hotw_runtime;				                    /* ����ˮ����ʱ�� */

/* ���������־ */
LOG_SYS_EXT BOL fg_energy_need[SUB_SYS_TYPE_MAX];           /* ��ϵͳ�������� */

LOG_SYS_EXT BOL fg_tuya_lang_switch;		//���Ը��¼��ʹ��	
LOG_SYS_EXT U08 cnt_wait_tuya_lang;		//�ȴ�Ϳѻ���������Ը���

LOG_SYS_EXT BOL fg_tuya_temp_unit_switch;		//�¶ȵ�λ���¼��ʹ��
LOG_SYS_EXT U08 cnt_wait_tuya_temp_unit;		//�ȴ�Ϳѻ�¶ȵ�λ����

LOG_SYS_EXT U16  HaveCompRuntime;           // ��ѹ�����е�ʱ��

LOG_SYS_EXT BOL fg_warm_complete;               // Ԥ����ɱ�ʶ
LOG_SYS_EXT BOL fg_warm_quick;                  // ����Ԥ�ȱ�ʶ
LOG_SYS_EXT U08 skip_preheat_cnt;              /* ����Ԥ�ȼ��� */
LOG_SYS_EXT U16 sys_warm_time;                  /* Ԥ�ȼ�ʱ */
LOG_SYS_EXT U16 sys_warm_time_need;             /* ��ҪԤ��ʱ�� */
//LOG_SYS_EXT U16 power_down_time;                /* ����ʱ������ */
LOG_SYS_EXT LOG_RTC power_down_time_last;       /* �ϴε���ʱ�䣺��ȷ�������eeprom��ʱ����������һ�� */


LOG_SYS_EXT U16 cnt_ster_cyc;       // ɱ�����ڼ�ʱ
LOG_SYS_EXT U16 cnt_ster_high_temp; // ɱ�����¶��ۼƼ�ʱ
LOG_SYS_EXT BOOL fg_enter_floor_dryup;  // �����ů��ɹ���
//LOG_SYS_EXT BOOL fg_force_ibh;    // �����ů��ɹ���
//LOG_SYS_EXT BOOL fg_force_tbh;    // �����ů��ɹ���

//LOG_SYS_EXT BOOL fg_enter_holiday;  // �����ݼ�ģʽ
//LOG_SYS_EXT I16 holiday_sv; // �ݼ�ģʽ�趨�¶�
//LOG_SYS_EXT U08 holiday_mode;   // �ݼ�ģʽ�趨ģʽ

#ifdef USE_AT_SV_FIX
#define EXIT_ABILITY_TEST_HEAT_SV           (620)
#define EXIT_ABILITY_TEST_COOL_SV           (50)
#else
#define EXIT_ABILITY_TEST_HEAT_SV           (sv_heat)
#define EXIT_ABILITY_TEST_COOL_SV           (sv_cool)
#endif

LOG_SYS_EXT I16 room_temp;          // �����¶�
/* ========================================================================================== */
#ifndef USE_QUICK_TIME
#define AC_PUMP_ON_DLY              30          //���յ�����ʱ
#define AC_PUMP_OFF_DLY             6           //�ؿյ�����ʱ:ϵͳ�ã�ģ�鲻�øú궨��
#define COMP_NEED_ON_TIME           120         //ѹ������������
#else
#define AC_PUMP_ON_DLY              (fg_dbg_slow?30:2)           //���յ�����ʱ
#define AC_PUMP_OFF_DLY             (fg_dbg_slow?6:6)           //�ؿյ�����ʱ
#define COMP_NEED_ON_TIME           (fg_dbg_slow?120:10)          //ѹ�����������У����ԣ�
#endif

#define COMP_ON_OFF_SPACE           3           //ѹ������ͣ���

#define COOL_LP_DLY                 5       //�����ѹ������ʱ
//#define LP_CHECK_DELAY                120     //��ѹ�����ʱ
//#define ERR_WT_EXIT               (10*5)  //���������²�
//#define ERR_DLY                       3       //һ�������ʱ
//#define ERR_FLUX_DLY              15      //ˮ�����������ʱ

#define INOUT_DIFF      5               // ����ˮ�²�(��)
#define INOUT_10DIFF    (INOUT_DIFF*10) // ����ˮ�²�(0.1�� �Ŵ�10��)    

#define CURRENT_DLY                 5       //�������󿪻������ʱ   
#define CURRENT_ERR_DLY             2       //�������������ʱ

#define  PUMP_MODE_ON     0   // һֱ����
#define  PUMP_MODE_INTER  1   // ��Ъ����    
#define  PUMP_MODE_MAX    2  

/* ����ģʽ */
#define POWER_MODE_STD              0       /* ��׼ */
#define POWER_MODE_STR              1       /* ǿ�� */
#define POWER_MODE_ECO              2       /* ���� */
#define POWER_MODE_AUTO             3       /* �Զ� */
#define POWER_MODE_MAX              4

LOG_SYS_EXT U08 fg_silent_mode;     // ����ģʽ
LOG_SYS_EXT U08 silent_mode_level;  // ����ģʽ����

#define ModeIsStd       (log_sys_get_power_mode()==POWER_MODE_STD)
#define ModeIsStr       (log_sys_get_power_mode()==POWER_MODE_STR)
#define ModeIsEco       (log_sys_get_power_mode()==POWER_MODE_ECO)

/* ========================================================================================== */
enum STATUS_READY {READY_IDLE, READY_PUMP};
enum STATUS_DELAY { DELAY_IDLE, DELAY_COMP, DELAY_COOL_PUMP,DELAY_PUMP, DELAY_VALVE };
LOG_SYS_EXT U08 status_ready;          /* ׼��̬ */
LOG_SYS_EXT U08 status_delay;          /* ͣ��̬ */
LOG_SYS_EXT U16 ready_time;            /* ׼��̬�����׶ε���ʱ */
LOG_SYS_EXT U08 delay_time;            /* ͣ��̬�����׶ε���ʱ */

#define STEP_IS_IDLE        (status_run == _IDLE)
#define STEP_IS_READY       (status_run == _READY)
#define STEP_IS_RUN         (status_run == _RUN)
#define STEP_IS_DELAY       (status_run == _DELAY)
#define STEP_IS_ALARM       (status_run == _ALARM)

#define StepIsIdle          (STEP_IS_IDLE)
#define StepIsReady         (STEP_IS_READY)
#define StepIsRun           (STEP_IS_RUN)
#define StepIsDelay         (STEP_IS_DELAY)
#define StepIsAlarm         (STEP_IS_ALARM)

#define MAC_IS_IDLE         (STEP_IS_IDLE || (STEP_IS_READY && status_ready==READY_IDLE))
#define MAC_IS_RUN          ((STEP_IS_READY && status_ready>READY_IDLE) \
                             || STEP_IS_RUN                             \
                             || STEP_IS_DELAY)

#define m_set_input_first(y)        SETBIT(cell[MAX_PCB].cell_input[(y)>>4], (y)&0x0f)
#define m_get_input_first(y)        GETBIT(cell[MAX_PCB].cell_input[(y)>>4], (y)&0x0f)
#define m_set_input(y)              SETBIT(sys_input, (y)&0x0f)
#define m_get_input(y)              GETBIT(sys_input, (y)&0x0f)



#define M_GET_CompMode(pcb_num, comp_num)    ((cell[pcb_num].cell_status[comp_num] >> COMP_MODE) & 0x0007)

LOG_SYS_EXT U08 random_time;            // ���ʱ�䣬�����ϵ��������ʱ������ȷ�ϣ�    
LOG_SYS_EXT U08 fg_ac_pump_firstopen;   // ������״ο�ˮ��

LOG_SYS_EXT RTIME sys_time;         /* ϵͳʱ�� */

#define FREE_ELEC_I_SV_HOTW       (700)
enum
{
    SG_CMD_NULL,
    SG_CMD_1,               /* ���ܵ���ָ��1 */
    SG_CMD_2,               /* ���ܵ���ָ��2 */
    SG_CMD_3,               /* ���ܵ���ָ��3 */
    SG_CMD_4,               /* ���ܵ���ָ��4 */
    SG_CMD_MAX,
};
enum
{
    SG_STA_NULL,
    SG_STA_1,               /* ���ܵ���״̬1 */
    SG_STA_2,               /* ���ܵ���״̬2 */
    SG_STA_3,               /* ���ܵ���״̬3 */
    SG_STA_4,               /* ���ܵ���״̬4 */
    SG_STA_MAX,
};
LOG_SYS_EXT U08 smart_grid_cmd;             /* ���ܵ���ָ�� */
LOG_SYS_EXT U16 cnt_sg_cmd_on[SG_CMD_MAX];  /* ���ܵ���ָ����Ч��ʱ */
LOG_SYS_EXT U16 cnt_sg_sta_off[SG_STA_MAX]; /* ���ܵ���״̬��ʱ */
LOG_SYS_EXT U08 smart_grid_sta;
LOG_SYS_EXT U08 sg_power_mode;
LOG_SYS_EXT BOOL fg_sg_hotw_priority;
LOG_SYS_EXT BOOL fg_sg_hotw_forbid;

/* �͹��õ�ǿ�ƹر� */
#define VALLEY_ELEC_FORCE_STOP    (smart_grid_sta==SG_STA_1)
/* �߷��õ��������� */
#define PEAK_ELEC_LIMIT_RUN       (smart_grid_sta==SG_STA_2)
/* ����õ����� */
#define FREE_ELEC_RUN             (smart_grid_sta==SG_STA_3)
/* ����õ�ǿ������ */
#define FREE_ELEC_FORCE_RUN       (smart_grid_sta==SG_STA_4)

enum
{
    SG_ACT_NULL,
    SG_ACT_ALL_FORCE_OFF,	/* ����ǿ�ƹر� */
    SG_ACT_HOTW_FORBID_ON,	/* ��ֹ��ˮ���� */
    SG_ACT_HOTW_PRIOR_RUN,	/* ��ˮ�ѿ��������������� */
    SG_ACT_HOTW_FORCE_ON,	/* ǿ����ˮ���� */
    SG_ACT_MAX,
};
LOG_SYS_EXT U08 smart_grid_act;

LOG_SYS_EXT U16 cnt_hotw_heat_switch_dly;		/* ���ȡ���ˮ�л���ʱ������ʱ */
#define SET_HOTW_HEAT_SWITCH_DLY (cnt_hotw_heat_switch_dly = 60)
#define CLR_HOTW_HEAT_SWITCH_DLY (cnt_hotw_heat_switch_dly =  0)
//LOG_SYS_EXT BOOL fg_mode_switching;

/**********************************************************************************/
LOG_SYS_EXT void cmd_comp_quik_stop(U08 pcb_num, U08 comp_num);
LOG_SYS_EXT void cmd_err_reset(void);
LOG_SYS_EXT void cmd_comp_run(U08 pcb_num, U08 comp_num, U08 to_mode);
LOG_SYS_EXT void cmd_systime_save(void);


LOG_SYS_EXT void work_initial(void);

LOG_SYS_EXT U08 OutIsChange(U08 no);
//LOG_SYS_EXT void runtime_initial(void);
//LOG_SYS_EXT void password_initial(void);
//LOG_SYS_EXT void systime_initial(void);
//LOG_SYS_EXT void systime_write_to_eeprom(void);
//LOG_SYS_EXT void mac_attrib_write_to_eeprom(void);
//LOG_SYS_EXT void limit_time_write_to_eeprom(void);
LOG_SYS_EXT BOOL is_thermo_dependece_all(U16 thermo_type, U16 comp_nun_set);

LOG_SYS_EXT void log_sys_init(void);
LOG_SYS_EXT void log_sys(U08 pcb_num);

LOG_SYS_EXT void log_sys_init_work(void);

LOG_SYS_EXT void log_sys_init_idle_sta_antifreeze(void);

LOG_SYS_EXT void menu_pv(void);
LOG_SYS_EXT void menu_work(void);
LOG_SYS_EXT BOL log_sys_have_fan_on(void);
LOG_SYS_EXT U08 log_sys_comp_run_num(U08 type);
LOG_SYS_EXT U08 log_sys_defrost_comp_num(U08 type);
//LOG_SYS_EXT BOL log_sys_is_silent(void);
LOG_SYS_EXT U16 log_sys_warm_time_get(void);
LOG_SYS_EXT U16 log_sys_get_ctrl_object(void);

#define GET_BY_CVS  0    /* ת��ֵ */
#define GET_BY_FACT 1    /* ʵ��ֵ */
LOG_SYS_EXT U08 log_sys_ctrl_mode_get(U08 type);

LOG_SYS_EXT void log_sys_UIRT(void);
LOG_SYS_EXT void log_sys_holiday_on_off(void);
LOG_SYS_EXT BOOL log_sys_have_comp_run(U08 mode, U08 type);
LOG_SYS_EXT BOL log_sys_have_non_defrosting_comp_run(U08 mode, U08 type);
LOG_SYS_EXT BOL log_sys_have_AHS_on(U08 zone);
LOG_SYS_EXT BOL log_sys_ac_have_comp_run(void);
LOG_SYS_EXT BOL log_sys_hotw_have_comp_run(void);
LOG_SYS_EXT BOL log_sys_ac_have_IBH_on(void);
LOG_SYS_EXT BOL log_sys_hotw_have_IBH_on(void);
LOG_SYS_EXT BOL log_sys_cell_pumpf_on(void);
LOG_SYS_EXT BOL log_sys_get_cmd_mux_bit(U08 id);
LOG_SYS_EXT BOL log_sys_have_cell_pump_run(U08 type);
LOG_SYS_EXT U08 log_sys_enegry_add(U08 mode, U08 type);
LOG_SYS_EXT U08 log_sys_get_power_mode(void);

LOG_SYS_EXT void log_sys_get_sv_disp_zone_A(U08 *grp, U08 *idx, I16 *value);
LOG_SYS_EXT void log_sys_get_sv_disp_zone_B(U08 *grp, U08 *idx, I16 *value);
LOG_SYS_EXT void log_sys_get_sv_disp_zone_hot(U08 *grp, U08 *idx, I16 *value);

LOG_SYS_EXT void log_sys_init_idle_sta_antifreeze(void);
LOG_SYS_EXT void log_sys_init_run_sta_antifreeze(void);
LOG_SYS_EXT void log_sys_init_timed_antifreeze(void);
LOG_SYS_EXT BOL log_sys_s3v1_is_hw(void);

/*--------------------------*/
//LOG_SYS_EXT U08 comp_status_debug_ac;   /*  ѹ��״̬���� */
//LOG_SYS_EXT U08 comp_status_debug_wt;   /*  ѹ��״̬���� */
//LOG_SYS_EXT U08 program_run_time;
//LOG_SYS_EXT U08 debug_1;
//LOG_SYS_EXT U16 debug_2;
//LOG_SYS_EXT U16 debug_3;
//LOG_SYS_EXT U16 debug_4;
//LOG_SYS_EXT U16 debug_5;
//LOG_SYS_EXT U16 debug_6[5];
//LOG_SYS_EXT U16 debug[20];    
//LOG_SYS_EXT U16 debug_8[10];
//LOG_SYS_EXT U16 debug_9[10];
//LOG_SYS_EXT U16 cmd_debug[4];
//LOG_SYS_EXT U16 cmd_debug2;
//LOG_SYS_EXT U16 cmd_debug_3;
//LOG_SYS_EXT U16 cmd_debug_4;
//LOG_SYS_EXT I16 anti_freeze_debug1;
//LOG_SYS_EXT I16 anti_freeze_debug2;
//LOG_SYS_EXT I16 anti_freeze_debug3;
/*--------------------------*/

#endif
