#ifndef COMMON_H
#define COMMON_H    1
#ifdef  COMMON_GLOBAL
#define COMMON_EXT
#else 
#define COMMON_EXT extern
#endif

typedef void *PVOID;

//���Դ洢����
//COMMON_EXT U16 debug_cs[16];

/* ģ������ */
#define TYPE_MASTER             (0)         /* ���� */
#define TYPE_SLAVER             (1)         /* �Ӱ� */

/* ˮ·���� */
#define ZONE_AC                 0	/* �յ��� */
#define ZONE_HOTW               1	/* ��ˮ�� */

/* ӵ��˫ˮ·ϵͳ */
#define IS_DUAL_SYS             (val_pcb_num>1 && val_hotw_pcb_num<val_pcb_num)

/* ��ϵͳ���� */
#define SUB_SYS_AC_HOTW         (0)         /* �յ�+��ˮ��ϵͳ */
#define SUB_SYS_AC_ONLY         (1)         /* ���յ���ϵͳ */
#define SUB_SYS_TYPE_MAX        (2)

/* ��ȡĳ����ϵͳ����ʼ/����ģ��� */
#define PCB_BGN(type)           (((type) == SUB_SYS_AC_ONLY) ? val_hotw_pcb_num : 0)
#define PCB_END(type)           (((type) == SUB_SYS_AC_HOTW) ? val_hotw_pcb_num : val_pcb_num)

/* ��ȡ��ǰģ��������ϵͳ */
#define GET_SUB_SYS(pcb_num)    (((pcb_num) < PCB_END(SUB_SYS_AC_HOTW)) ? SUB_SYS_AC_HOTW\
                                                                        : SUB_SYS_AC_ONLY)

/* ��ȡ��ǰģ��������ϵͳ�Ƿ���Ҫ���� */
#define SUB_SYS_AC_HOTW_ON     ((fg_zone_A_run || fg_zone_B_run || fg_hotw_run) || fg_run_sta_hotw_antifreeze)
#define SUB_SYS_AC_ONLY_ON     ((fg_zone_A_run || fg_zone_B_run) || fg_run_sta_ac_antifreeze)
#define GET_SUB_SYS_ON(pcb_num)((energy_ctrl_type == JOINT_ENERGY_CTRL) ? TRUE \
                                                                        : (GET_SUB_SYS(pcb_num) == SUB_SYS_AC_HOTW) ? SUB_SYS_AC_HOTW_ON \
                                                                                                                    : SUB_SYS_AC_ONLY_ON)

/*----------------����--------------------------------*/
/* ѹ������ */  //ע��ѹ���������ַ���
#define CMD_OPEN_COMP           0x0001          /* ��ѹ�� */
#define CMD_CLOSE_COMP          0x0002          /* ��ѹ�� */
#define CMD_QUICK_STOP_COMP     0x0004          /* ��ͣѹ�� */
#define CMD_STOP_COMP_SYSTEM    0x0008          /* ͣѹ��ϵͳ */
#define CMD_CM_MODE                  4          //ѹ��ģʽ

#define CMD_ALLOW_DEFROST       0x0100          /* ѹ�������˪ */
#define CMD_FORCE_DEFROST       0x0200          /* ѹ��ǿ�Ƴ�˪ */

/* ģ������ */
#define CMD_STOP_ALL_COMP       0x0001          /* ͣ����ѹ�� */
#define CMD_STOP_ALL_MAC        0x0002          /* �ر�������� */
#define CMD_ERR_RESET           0x0004          /* ��λ */
#define CMD_FAC_PARA_INIT       0x0008          /* ���Ҳ�����ʼ�� */
#define CMD_RUNTIME_INIT        0x0010          /* ѹ���ۼ�����ʱ���ʼ�� */
#define CMD_MODE                5               /* ģʽ������ʼλ��5~7 */

#define CMD_OPEN_CELL           0x0100		    /* ��ģ�� */
#define CMD_CLOSE_CELL          0x0200		    /* ��ģ�� */
#define CMD_LIMIT_INIT          0x0400          /* ϵͳά��������ʼ�� */
#define CMD_SYSTIME_INIT        0x0800          /* ϵͳ�ۼ�����ʱ���ʼ�� */

#define CMD_SYSTIME_SAVE        0x1000          /* ϵͳ�ۼ�����ʱ�䱣�� */
#define CMD_PASSWORD_INIT       0x4000          // �����ʼ��
#define CMD_INVT_INIT           0x8000          // ��Ƶ����ʼ��

/*----------------״̬--------------------------------*/
/* ��һ��״̬ */
#define COMP_RUN                0x0001          /* ѹ������״̬ */
#define COMP_RUN_NEED           0x0002          /* ѹ������ */
#define COMP_CAN_OPEN           0x0004          /* �ɿ� */
#define COMP_CAN_CLOSE          0x0008          /* �ɹ� */
#define COMP_ADD_PRIO           0x0010          /* ѹ���������� */
#define COMP_SUB_PRIO           0x0020          /* ѹ��ж������ */
#define COMP_MODE               6               /* ��ǰ����ģʽ��ʼλ�ã�ռ��6��7��8λ�� */


/* ѹ����ʹ��ģʽ���� */
#define C_MODE_COOL             0x0200          /* ���� */  /* ��������ģʽ��ռ��9��10λ�� */
#define C_MODE_HEAT             0x0400          /* ���� */

#define COMP_CAN_ADD            0x0800          /* ѹ������Ƶ */
#define COMP_CAN_SUB            0x1000          /* ѹ���ɽ�Ƶ */ 

#define COMP_DEFROST            0x2000          /* ��˪״̬ */
#define COMP_DEFROST_APPLY      0x4000          /* ѹ����˪���� */
#define COMP_FORCE_DEFROST      0x8000          /* ǿ�Ƴ�˪״̬ */


/* ģ��(��Ԫ)״̬���� */
#define CELL_RUN                  0x0001	/* ��Ԫ���� */
#define CELL_ERR                  0x0002	/* ģ����� */
#define CELL_PUMP_OPEN            0x0004	/* ģ��ˮ�ÿ��� */
#define CELL_COMP                 0x0030	/* ��Ԫѹ����(��4 5 λ) */
#define CELL_CAN_RUN_HOTW         0x0040	/* ��Ԫ����������ˮ */
#define CELL_FORCE_IBH_OFF        0x0080	/* ��Ԫǿ���������ȹر� */
#define CELL_PUMP_RUN             0x0100	/* ģ��ˮ���ȶ����� */

/*----------------------------------------------------*/
/* ��ģ�鶨�� */
/* ϵͳ���� */
#define M_ERR_PHASE_REVERSE      1  // ���ౣ�� 
#define M_ERR_EEPROM             2  /* EEPROM���ݴ���� */  
//#define M_ERR_SYS_ENVIRON        3  /* ϵͳ���¹��� */  
//#define M_ERR_SYS_IN             4  /* ϵͳ���¹��� */  
#define M_ERR_TWOUT2             5  /* �ܳ�ˮ�¶ȹ��� */  
#define M_ERR_EEPROM_LIMIT       6  /* ϵͳά�����ݴ�*/
#define M_ERR_PHASE_LACK         7  // ȱ�ౣ��
//#define M_ERR_LACK_FAN           8  // �ͷ�ѹ��
//#define M_ERR_WATER_FLOW         9  // ˮ������
#define M_ERR_TBH_OL            10			/* ˮ����ȹ��ع��� */
//#define M_ERR_IBH_OL            11			/* ���ù��� */
#define M_ERR_THWT              12			/* ��ˮˮ���¶ȹ��� */
//#define M_ERR_OUT_T1            13			/* ���ù��� */
#define M_ERR_TWOUT3            14			/* ϵͳ�ܳ�ˮ�¶ȹ��� */
#define M_ERR_TFLIN             15			/* ��ů�ܽ��¹��� */
#define M_ERR_FCU_ERR_COMMU1    16  // ����01ͨѶ���� 
#define M_ERR_FCU_ERR_COMMU2    17  // ����02ͨѶ���� 
#define M_ERR_FCU_ERR_COMMU3    18  // ����03ͨѶ���� 
#define M_ERR_FCU_ERR_COMMU4    19  // ����04ͨѶ���� 
#define M_ERR_FCU_ERR_COMMU5    20  // ����05ͨѶ���� 
#define M_ERR_FCU_ERR_COMMU6    21  // ����06ͨѶ���� 
#define M_ERR_FCU_ERR_COMMU7    22  // ����07ͨѶ���� 
#define M_ERR_FCU_ERR_COMMU8    23  // ����08ͨѶ���� 
#define M_ERR_FCU_ERR_COMMU9    24  // ����09ͨѶ���� 
#define M_ERR_FCU_ERR_COMMU10   25  // ����10ͨѶ����  
#define M_ERR_FCU_ERR_COMMU11   26  // ����11ͨѶ����  
#define M_ERR_FCU_ERR_COMMU12   27  // ����12ͨѶ����  
#define M_ERR_FCU_ERR_COMMU13   28  // ����13ͨѶ����  
#define M_ERR_FCU_ERR_COMMU14   29  // ����14ͨѶ����  
#define M_ERR_FCU_ERR_COMMU15   30  // ����15ͨѶ����  
#define M_ERR_FCU_ERR_COMMU16   31  // ����16ͨѶ����  
#define M_ERR_FCU_ERR_ERR1      32  // ����01���� 
#define M_ERR_FCU_ERR_ERR2      33  // ����02���� 
#define M_ERR_FCU_ERR_ERR3      34  // ����03���� 
#define M_ERR_FCU_ERR_ERR4      35  // ����04���� 
#define M_ERR_FCU_ERR_ERR5      36  // ����05���� 
#define M_ERR_FCU_ERR_ERR6      37  // ����06���� 
#define M_ERR_FCU_ERR_ERR7      38  // ����07���� 
#define M_ERR_FCU_ERR_ERR8      39  // ����08���� 
#define M_ERR_FCU_ERR_ERR9      40  // ����09���� 
#define M_ERR_FCU_ERR_ERR10     41  // ����10����  
#define M_ERR_FCU_ERR_ERR11     42  // ����11����  
#define M_ERR_FCU_ERR_ERR12     43  // ����12����  
#define M_ERR_FCU_ERR_ERR13     44  // ����13����  
#define M_ERR_FCU_ERR_ERR14     45  // ����14����  
#define M_ERR_FCU_ERR_ERR15     46  // ����15����  
#define M_ERR_FCU_ERR_ERR16     47  // ����16���� 
#define M_ERR_FCU_ERR_COMMU17   48  // ����17ͨѶ���� 
#define M_ERR_FCU_ERR_COMMU18   49  // ����18ͨѶ���� 
#define M_ERR_FCU_ERR_COMMU19   50  // ����19ͨѶ����
#define M_ERR_FCU_ERR_COMMU20   51  // ����20ͨѶ����
#define M_ERR_FCU_ERR_COMMU21   52  // ����21ͨѶ����
#define M_ERR_FCU_ERR_COMMU22   53  // ����22ͨѶ����
#define M_ERR_FCU_ERR_COMMU23   54  // ����23ͨѶ����
#define M_ERR_FCU_ERR_COMMU24   55  // ����24ͨѶ����
#define M_ERR_FCU_ERR_COMMU25   56  // ����25ͨѶ����
#define M_ERR_FCU_ERR_COMMU26   57  // ����26ͨѶ����
#define M_ERR_FCU_ERR_COMMU27   58  // ����27ͨѶ����
#define M_ERR_FCU_ERR_COMMU28   59  // ����28ͨѶ����
#define M_ERR_FCU_ERR_COMMU29   60  // ����29ͨѶ����
#define M_ERR_FCU_ERR_COMMU30   61  // ����30ͨѶ����
#define M_ERR_FCU_ERR_COMMU31   62  // ����31ͨѶ����
#define M_ERR_FCU_ERR_COMMU32   63  // ����32ͨѶ����
#define M_ERR_FCU_ERR_ERR17     64  // ����17���� 
#define M_ERR_FCU_ERR_ERR18     65  // ����18���� 
#define M_ERR_FCU_ERR_ERR19     66  // ����19����
#define M_ERR_FCU_ERR_ERR20     67  // ����20����
#define M_ERR_FCU_ERR_ERR21     68  // ����21����
#define M_ERR_FCU_ERR_ERR22     69  // ����22����
#define M_ERR_FCU_ERR_ERR23     70  // ����23����
#define M_ERR_FCU_ERR_ERR24     71  // ����24����
#define M_ERR_FCU_ERR_ERR25     72  // ����25����
#define M_ERR_FCU_ERR_ERR26     73  // ����26����
#define M_ERR_FCU_ERR_ERR27     74  // ����27����
#define M_ERR_FCU_ERR_ERR28     75  // ����28����
#define M_ERR_FCU_ERR_ERR29     76  // ����29����
#define M_ERR_FCU_ERR_ERR30     77  // ����30����
#define M_ERR_FCU_ERR_ERR31     78  // ����31����
#define M_ERR_FCU_ERR_ERR32     79  // ����32����
#define M_ERR_TACT1             80			/* ����ˮ���ϲ��¶ȹ��� */
#define M_ERR_TACT2             81			/* ����ˮ���²��¶ȹ��� */
#define M_ERR_TSL               82			/* ̫�����¶ȹ��� */
#define M_ERR_UI35_COMMU_ERR    83			/* �߿���ͨѶ���� */
#define M_ERR_TWOUT2_AC_ERR     84			/* �յ��ܳ�ˮ�¶ȹ��� */
#define M_ERR_TR                85			/* �����¶ȹ��� */
#define M_ERR_PUMPF             86			/* ��Ƶˮ�ù��� */
#define M_ERR_MAX  (MAX_ERR*ERR_BIT_NUM) // 96
enum
{
    AUTOERR_M_PS_LACK = 0    ,// ȱ�ࣨϵͳ��
    AUTOERR_M_PS_REV         ,// ���ࣨϵͳ��
    AUTOERR_M_BAK2           ,
    AUTOERR_M_BAK3           ,
    AUTOERR_M_BAK4           ,
    AUTOERR_M_BAK5           ,
    AUTOERR_M_BAK6           ,
    AUTOERR_M_BAK7           ,
    AUTOERR_LP_DI_1          ,// ѹ��1��ѹ�����أ�
    AUTOERR_HP_DI_1          ,// ѹ��1��ѹ�����أ�
    AUTOERR_LP_AI_1          ,// ѹ��1��ѹ��ѹ����
    AUTOERR_HP_AI_1          ,// ѹ��1��ѹ��ѹ����
    AUTOERR_GAS_1            ,// ѹ��1��������
    AUTOERR_GASIN_LOW_1      ,// �����¶ȹ���
    AUTOERR_EVAP_LOW_1       ,// ������������
    AUTOERR_S_IN_HIGH        ,// ���¹���
    AUTOERR_S_IN_LOW         ,// ���¹���
    AUTOERR_S_OUT_IN_DIFF    ,// �����²����
    AUTOERR_S_OUT_IN_ERR     ,// �����²��쳣
//    AUTOERR_S_CUR_HIGH       ,// ѹ����������
    AUTOERR_S_AC_FLUX        ,// ˮ�����أ�ģ�飩
    AUTOERR_S_OUT_HIGH       ,// ���¹��ߣ�ģ�飩
    AUTOERR_S_OUT_LOW        ,// ���¹��ͣ�ģ�飩
    AUTOERR_S_INVT           ,// ��Ƶ����
    AUTOERR_S_PWM_FAN_EXCEPT ,// ���ת���쳣
    AUTOERR_EVAP_HIGH        ,// �����¶ȹ���
    MAX_AUTO_RESET_NUM
};

/* �Զ���λ���� */
#define MAX_ALLOW_TIMES  2          /* �Զ���λ������1Сʱ�ڵ�����������(�����ô��������ֶ���λ) */   

#define val_am_lock_time_min    (QK_60*30)  /* AM��������ʱ����Сֵ */
#define val_am_lock_time_add    (QK_60*30)  /* AM��������ʱ������ */
#define val_am_lock_time_max    (QK_60*240) /* AM��������ʱ�����ֵ */

typedef struct tagAUTORESETERROR 
{
    U16  dly;           /* �Զ���λ��ʱ */
    U08  en;            /* �Զ���λʹ�ܱ�־λ */
    U08  auto_en;       /* �����͹��Ͽ�����Զ���λ���ϱ�־λ*/
    U08  cnt;           /* �Զ���λ�������� */
    U08  have_reset;    /* �Ѿ���λ��һ�α�־λ */
    U16  reset_time[MAX_ALLOW_TIMES];       /* �Զ���λ���ϼ�ʱ */  
    U16  err_time;                          /* ��ǰ���ϼ�ʱ */
    U16  err_time_lock;                     /* ��������ʱ�� */
} AUTORESETERROR;
COMMON_EXT AUTORESETERROR  autoERR[MAX_AUTO_RESET_NUM]; // ע��ʹ��λ�ù̶�����Ҫ������

/* ע��ͬ������assist_info */
/* ���붨�� */
#define IN_LINK             0           /* ��ͨ������ */
#define IN_REMOTE           1           /* �߿ؿ��� */
//#define IN_ELE_HEAT       2           /* �縨�ȱ��� */    /* N0002 */
#define IN_SEND_FAN_PRESS   3           /* �ͷ�ѹ�� */  
//#define M_IN_WATER_FLOW    4           // ˮ������		/* ʵ����ϵͳˮ������ */
#define IN_MODE_COOL        5           // ���俪��
#define IN_MODE_HEAT        6           // ���ȿ���
//#define S_IN_IBH_OL           7           // �������ȷ���
#define M_DI_TBH_OL           8           // ˮ����ȷ���
#define IN_RT_COOL          9				/* �¿��������ź� */
#define IN_RT_HEAT          10				/* �¿��������ź� */
#define IN_SOLAR            11				/* ̫�����ź� */
#define IN_SG               12          // ���ܵ���SG�ź�
#define IN_EVU              13          // ���ܵ���EVU�ź�


/* ������� */
#define SW_AC_PM_WATER      1				/* �յ��� */
#define SW_AC_HT            2				/* �縨�� */
#define SW_END_PM           6				/* ĩ�˱� */
#define SW_AC_PM_AIR        7				/* �ͷ�� */
 
#define M_DO_DFR              (0)			/* ��˪ָʾ */
#define M_DO_ERR              (1)			/* ����ָʾ */
#define M_DO_AHS              (2)			/* �ⲿ��Դ */
#define M_DO_S3V1             (3)			/* �����ͨ��1 */
#define M_DO_S3V3             (4)			/* �����ͨ��3 */
#define M_DO_S3V2             (5)			/* �����ͨ��2 */
#define M_DO_TBH              (6)			/* ˮ����� */
#define M_DO_BAK_07           (7)			/* ���� */
#define M_DO_PUMPret          (8)			/* ����ˮ��ˮ�� */
#define M_DO_PUMPsl           (9)			/* ̫����ˮ�� */
#define M_DO_PUMPm            (10)			/* ���ˮ�� */
#define M_DO_PUMPo            (11)			/* ��ѭ���� */
#define M_DO_PUMP             (12)			/* ˮ�� */
#define M_DO_BAK_13           (13)			/* ���� */


#define SW_AC_PM                (AIR_IS_WATER?SW_AC_PM_WATER    : SW_AC_PM_AIR)     // �յ���\�ͷ�� 

#define get_output(x, y)        GET_BIT(cell[x].cell_relay[(y)/ RLY_BIT_NUM], (y)% RLY_BIT_NUM)
#define set_output(x, y)        SET_BIT(cell[x].cell_relay[(y)/ RLY_BIT_NUM], (y)% RLY_BIT_NUM)
#define clr_output(x, y)        CLR_BIT(cell[x].cell_relay[(y)/ RLY_BIT_NUM], (y)% RLY_BIT_NUM)


/*--------------------------------------------------------------------*/
/* ��ģ�鶨�� */
#define CELL_ERR_BEGIN          64 // ѹ��������:ģ��-��Ԫ����ƫ������
#define CELL_ERR_NUM            32 // ��Ԫ������

                                                            
/* ѹ�����϶��� */
#define ERR_LP_1                0   // {ѹ����ѹ}       
#define ERR_HP_1                1   // {ѹ����ѹ}       
//#define ERR_CURR_HIGH_1         2   // {ѹ����������}   
//#define ERR_CURR_LOW_1          3   // {ѹ��������С}   
#define ERR_WING_1              4   // {��Ƭ̽ͷ����}   
#define ERR_GAS_OUT_1           5   // {����̽ͷ����}   
#define ERR_EVAP_HIGH_1         6   // {�����¶ȹ���}   
#define ERR_J5_SENSOR_1         7   // {J5ѹ������}   
#define ERR_J6_SENSOR_1         8   // {J6ѹ������}   
#define ERR_LP_AI_1             9   // {ѹ����ѹ����}   
#define ERR_HP_AI_1             10  // {ѹ����ѹ����}   
#define ERR_GAS_IN_1            11  // {�����¶ȹ���}   
#define ERR_EVAP_IN_1           12  // {�����¶ȹ���}   
#define ERR_GAS_IN_LOW_1        13  // {�����¶ȹ���}   
#define ERR_EMERG_DEFROST_1     14  // {������˪Ƶ��}   
#define ERR_GAS_IN_OUT_ERR_1    15  // {�����²��쳣}   
#define ERR_EVAP_LOW_1          16  // {������������}   
//#define ERR_IN_SUB_FREQ_1       17  // {��Ƶ����}       
#define ERR_TA_LIMIT_COMP_ON    18  // {��������ѹ������}            
#define ERR_IN_LOW              19  // {�����¶ȹ���}   
#define ERR_IN_HIGH             20  // {�����¶ȹ���}   
#define ERR_PWM_FAN1_EXCEPT     21  // {���1ת���쳣}  
#define ERR_PWM_FAN2_EXCEPT     22  // {���2ת���쳣}  
#define ERR_EC1_COMMU           23  // {EC1ͨѶ����}    
#define ERR_EC1_ERR             24  // {EC1����}        
#define ERR_EC2_COMMU           25  // {EC2ͨѶ����}    
#define ERR_EC2_ERR             26  // {EC2����}        
#define ERR_INVT_COMMU          27  // {��ƵͨѶ����}   
#define ERR_INVT_ERR            28  // {��Ƶ����}       
#define ERR_INVT_MODEL          29  // {��Ƶ�ͺ�������} 
#define ERR_TEMP_EVI_IN         30  // {����������¶ȹ���}   
#define ERR_TEMP_EVI_OUT        31  // {�����������¶ȹ���}  
#define ERR_TRFgas_SENSOR       32	/* ��ý�����¶ȹ��� */
#define ERR_TRFliq_SENSOR       33	/* ��ýҺ���¶ȹ��� */
#define ERR_EVAP_HIGH           34  // {�����¶ȹ���}

// ��Ԫ����
#define S_ERR_TEMP_TA           CELL_ERR_BEGIN+0			/* �����¶ȹ��� */
#define S_ERR_CASCADE_COMMU     CELL_ERR_BEGIN+1			/* ͨѶ����  */
#define S_ERR_EEPROM            CELL_ERR_BEGIN+2			/* EEPROM���ݴ� */
#define S_ERR_IBH_OVERLOAD      CELL_ERR_BEGIN+3			/* �������ȹ��� */
//#define S_ERR_TBH_OVERLOAD      CELL_ERR_BEGIN+4			/* ˮ����ȹ��� */
#define S_ERR_OUT_LOW           CELL_ERR_BEGIN+5			/* �廻��ˮ�¶ȹ��� */
#define S_ERR_OUT_HIGH          CELL_ERR_BEGIN+6			/* �廻��ˮ�¶ȹ��� */
#define S_ERR_TEMP_TWOUT1       CELL_ERR_BEGIN+7			/* �廻��ˮ�¶ȹ��� */
#define S_ERR_WATER_FLOW        CELL_ERR_BEGIN+8			/* ˮ������ */
#define S_ERR_PHASE_LACK        CELL_ERR_BEGIN+9			/* ȱ�ౣ�� */
#define S_ERR_TEMP_TWIN1        CELL_ERR_BEGIN+10			/* �廻��ˮ�¶ȹ��� */
#define S_ERR_TEMP_TWOUT2       CELL_ERR_BEGIN+11			/* �ܳ�ˮ�¶ȹ��� */
#define S_ERR_PHASE_REVERSE     CELL_ERR_BEGIN+12			/* ���ౣ�� */
#define S_ERR_OUT_IN_DIFF       CELL_ERR_BEGIN+13			/* �廻�����²���� */
#define S_ERR_OUT_IN_ERR        CELL_ERR_BEGIN+14			/* �廻�����²��쳣 */
#define S_ERR_PUMPF_WARN        CELL_ERR_BEGIN+15			/* ��Ƶˮ�þ��� */
#define S_ERR_COMMU_EXPT        CELL_ERR_BEGIN+16			/* ͨѶ�쳣 */
#define S_ERR_COMMU_INOUT       CELL_ERR_BEGIN+17			/* �����ͨѶ�쳣 */
#define S_ERR_VER_INOUT         CELL_ERR_BEGIN+18			/* Э��汾���� */
#define S_ERR_PUMPF             CELL_ERR_BEGIN+19			/* ��Ƶˮ�ù��� */
#define S_ERR_MAC_ERR           CELL_ERR_BEGIN+20			/* ���������쳣 */

/* ����Ϊδ�õ��Ĺ��� */
//#define S_ERR_POWER			/* ��Դ���� */
//#define S_ERR_COOL_FLUX_OL	/* ������� */
//#define S_ERR_ANTIFREZZ		/* �����¶ȹ��� */
//#define S_ERR_TEMP_ANTIFREZZ  /* ����̽ͷ���� */

#define F_NO_ERR                (CELL_ERR_BEGIN+CELL_ERR_NUM)   /* �޹���, +1��Ϊ0,��ͷ��ʼ */
#define ERR_BAK                 F_NO_ERR

#define ERR_TOTAL               ((MAX_PCB+1)*F_NO_ERR) // ���ģ������Ӧ���ܹ�����

//ѹ�����
typedef enum
{
    PRS_ID_1,       //��ѹ
    PRS_ID_2,       //��ѹ
    PRS_ID_MAX      //ѹ������
}PRS_ID_ENUM;

/*--------------------------------------------------------------------*/
/* ע��ͬ������assist_info */
/* ѹ�����붨�� */
#define IN_LP           0               /* ѹ����ѹ */
#define IN_HP           1               /* ѹ����ѹ */

// ��Ԫ����������           /* ��Ԫ�߼���ַ��ѹ���߼���ַ���ã�����������صĵ�ַ�ǹ���һ�ֽ� */
#define S_IN_IBH_OL       0	/* �������ȷ��� */
#define S_IN_WATER_FLOW      1	/* ˮ������ */

//#define IN_FAN_OL       2               /* ��ȴ������� */
//#define IN_POWER_ERR    4               /* ��Դ���� */
//#define IN_SUB_FREQ     5               /* ��Ƶ���أ���ѹ���ص� */  /* N0004 */

/*--------------------------------------------------------------------*/
/* ������� */
// ѹ��������� ��Ӧcell_relay[0~MAX_COMP-1]
#define SW_CM           0           /* ѹ�� */
#define SW_S1           1           /* ��ͨ�� */
#define SW_byway        2           /* ��Һ�� */
#define SW_FAN_LOW      3           /* ���ٷ�������1 */
#define SW_FAN_HIG      4           /* ���ٷ�������2 */
#define SW_crank1       5           /* ��������1 */
#define SW_EVI          6           /* ���ʷ� */
#define SW_crank2       7           /* ��������2 */
/* ���8�������� */

// ģ��������� ��Ӧcell_relay[MAX_COMP]  
#define S_DO_PUMPf        0	/* ��Ƶˮ�� */
#define S_DO_IBH1         1	/* ��������1 */
#define S_DO_IBH2         2	/* ��������2 */
#define S_DO_DPH          3	/* ���̵���� */
#define S_DO_HT           4	/* ���������ȴ� */
#define S_DO_DPH2         5	/* ���̵����2 */

//#define SW_valve_defr   1           /* ��˪�� */

#define s_get_output(cm, n)     (GET_BIT(pCell->cell_relay[cm], (n)% RLY_BIT_NUM))
#define s_set_output(cm, n)     (SET_BIT(pCell->cell_relay[cm], (n)% RLY_BIT_NUM))
#define s_clr_output(cm, n)     (CLR_BIT(pCell->cell_relay[cm], (n)% RLY_BIT_NUM))


/* �ͷ��������ˮ�����滻��Ӧ�� */
#define AIR_IS_WATER        (val_machine_type == AIR_WATER)     // ������ˮ
#define AIR_IS_AIR          (val_machine_type == AIR_AIR)       // �����ͷ�

/*--------------------------------------------------------------------*/
// ģ�������� (�¶�Ϊ�Ŵ�10�����¶�ֵ)--------------------------------
#define SYS_PV_NUM      MAX_PV                          // ϵͳģ��������
#define COMP_PV_NUM     48                              // ��̨ѹ��ģ��������                                     
#define CELL_PV_NUM     (MAX_PV-COMP_PV_NUM*USE_COMP)   // ��Ԫģ��������
#define CELL_PV_BEGIN   (COMP_PV_NUM*USE_COMP)          // ��Ԫģ������ʼλ��


// ϵͳģ��������
#define pv_sys_TR                       (cell[MAX_PCB].cell_pv[ 0])	/* �����¶� 0.1�� */
#define pv_sys_bak_01                   (cell[MAX_PCB].cell_pv[ 1])	/* ���� 0.1�� */
#define pv_sys_TA                       (cell[MAX_PCB].cell_pv[ 2])	/* ϵͳ����(ģ��ƽ��ֵ) 0.1�� */
#define pv_sys_THWt                     (cell[MAX_PCB].cell_pv[ 3])	/* ��ˮˮ���¶� 0.1�� */
#define pv_sys_TWout2                   (cell[MAX_PCB].cell_pv[ 4])	/* ϵͳ����(ģ��ƽ��ֵ) 0.1�� */
#define pv_sys_TWout3                   (cell[MAX_PCB].cell_pv[ 5])	/* ϵͳ�ܳ�ˮ�¶� 0.1�� */
#define pv_sys_TSL                      (cell[MAX_PCB].cell_pv[ 6])	/* ̫�����¶� 0.1�� */
#define pv_sys_TACt1                    (cell[MAX_PCB].cell_pv[ 7])	/* ����ˮ���ϲ��¶� 0.1�� */
#define pv_sys_TACt2                    (cell[MAX_PCB].cell_pv[ 8])	/* ����ˮ���²��¶� 0.1�� */
#define pv_sys_TWout2_ac                (cell[MAX_PCB].cell_pv[ 9])	/* �յ��ܳ�ˮ�¶� 0.1�� */
#define pv_sys_bak_10                   (cell[MAX_PCB].cell_pv[10])	/* ���� 0.1�� */
#define pv_sys_bak_11                   (cell[MAX_PCB].cell_pv[11])	/* ���� 0.1�� */
#define pv_sys_bak_12                   (cell[MAX_PCB].cell_pv[12])	/* ���� 0.1�� */
#define pv_sys_bak_13                   (cell[MAX_PCB].cell_pv[13])	/* ���� 0.1�� */
#define pv_sys_TFLin                    (cell[MAX_PCB].cell_pv[14])	/* ��ů�ܽ�ˮ�¶� 0.1�� */
#define pv_sys_S3V1_state               (cell[MAX_PCB].cell_pv[15])	/* �����ͨ��1״̬ */
#define pv_sys_S3V2_state               (cell[MAX_PCB].cell_pv[16])	/* �����ͨ��2״̬ */
#define pv_sys_S3V3_state               (cell[MAX_PCB].cell_pv[17])	/* �����ͨ��3״̬ */
#define pv_sys_PUMPf_pwm_in             (cell[MAX_PCB].cell_pv[18])     // ����
#define pv_sys_cl_pwr                   (cell[MAX_PCB].cell_pv[19])     // ���书��
#define pv_sys_ht_pwr                   (cell[MAX_PCB].cell_pv[20])     // ���ȹ���
#define pv_sys_hw_pwr                   (cell[MAX_PCB].cell_pv[21])     // ����ˮ����

// ģ��_ѹ��ģ��������
#define pv_cell_curr(cm)            (cell[pcb_addr].cell_pv[ 0+(cm*COMP_PV_NUM)])   // {����}           
#define pv_cell_step_m(cm)          (cell[pcb_addr].cell_pv[ 1+(cm*COMP_PV_NUM)])   // {�����򿪶�}     
#define pv_cell_step_s(cm)          (cell[pcb_addr].cell_pv[ 2+(cm*COMP_PV_NUM)])   // {�����򿪶�}     
#define pv_cell_wing(cm)            (cell[pcb_addr].cell_pv[ 3+(cm*COMP_PV_NUM)])   // {��Ƭ�¶�}       
#define pv_cell_gas_out(cm)         (cell[pcb_addr].cell_pv[ 4+(cm*COMP_PV_NUM)])   // {�����¶�}       
#define pv_cell_gas_in(cm)          (cell[pcb_addr].cell_pv[ 5+(cm*COMP_PV_NUM)])   // {�����¶�}       
#define pv_cell_evap(cm)            (cell[pcb_addr].cell_pv[ 6+(cm*COMP_PV_NUM)])   // {�����¶�}       
#define pv_cell_comp_top(cm)        (cell[pcb_addr].cell_pv[ 7+(cm*COMP_PV_NUM)])   // {ѹ���¶�}       
#define pv_cell_evap_temp(cm)       (cell[pcb_addr].cell_pv[ 8+(cm*COMP_PV_NUM)])   // {���������¶�}   
#define pv_cell_cond_temp(cm)       (cell[pcb_addr].cell_pv[ 9+(cm*COMP_PV_NUM)])   // {���������¶�}   
#define pv_cell_lp(cm)              (cell[pcb_addr].cell_pv[10+(cm*COMP_PV_NUM)])   // {��ѹѹ��}       
#define pv_cell_hp(cm)              (cell[pcb_addr].cell_pv[11+(cm*COMP_PV_NUM)])   // {��ѹѹ��}       
#define pv_cell_eev_sv(cm)          (cell[pcb_addr].cell_pv[12+(cm*COMP_PV_NUM)])   // {����Ŀ�����}   
#define pv_cell_eev_pv(cm)          (cell[pcb_addr].cell_pv[13+(cm*COMP_PV_NUM)])   // {������ǰ����}   
#define pv_cell_evi_sv(cm)          (cell[pcb_addr].cell_pv[14+(cm*COMP_PV_NUM)])   // {����Ŀ�����}   
#define pv_cell_evi_pv(cm)          (cell[pcb_addr].cell_pv[15+(cm*COMP_PV_NUM)])   // {������ǰ����}   
#define pv_cell_evi_in(cm)          (cell[pcb_addr].cell_pv[16+(cm*COMP_PV_NUM)])   // {����������¶�} 
#define pv_cell_evi_out(cm)         (cell[pcb_addr].cell_pv[17+(cm*COMP_PV_NUM)])   // {�����������¶�} 
#define pv_cell_nofrost_EWD(cm)     (cell[pcb_addr].cell_pv[18+(cm*COMP_PV_NUM)])   // {��˪���л����} 
#define pv_cell_invt_set(cm)        (cell[pcb_addr].cell_pv[19+(cm*COMP_PV_NUM)])   // {��ƵĿ��ת��}   
#define pv_cell_invt_cur(cm)        (cell[pcb_addr].cell_pv[20+(cm*COMP_PV_NUM)])   // {��Ƶ��ǰת��}   
#define pv_cell_invt_volt(cm)       (cell[pcb_addr].cell_pv[21+(cm*COMP_PV_NUM)])   // {��Ƶ�����ѹ}   
#define pv_cell_invt_curr(cm)       (cell[pcb_addr].cell_pv[22+(cm*COMP_PV_NUM)])   // {��Ƶ�������}   
#define pv_cell_invt_tmp(cm)        (cell[pcb_addr].cell_pv[23+(cm*COMP_PV_NUM)])   // {��Ƶģ���¶�}   
#define pv_cell_invt_msg(cm)        (cell[pcb_addr].cell_pv[24+(cm*COMP_PV_NUM)])   // {��Ƶ��Ƶ��Ϣ}   
#define pv_cell_invt_curr_in(cm)    (cell[pcb_addr].cell_pv[25+(cm*COMP_PV_NUM)])   // {��Ƶ�������}   
#define pv_cell_invt_power_out(cm)  (cell[pcb_addr].cell_pv[26+(cm*COMP_PV_NUM)])   // {��Ƶ�������}   
#define pv_cell_fan1_set(cm)        (cell[pcb_addr].cell_pv[27+(cm*COMP_PV_NUM)])   // {���1Ŀ������}  
#define pv_cell_fan1_now(cm)        (cell[pcb_addr].cell_pv[28+(cm*COMP_PV_NUM)])   // {���1��ǰת��}  
#define pv_cell_fan2_set(cm)        (cell[pcb_addr].cell_pv[29+(cm*COMP_PV_NUM)])   // {���2Ŀ������}  
#define pv_cell_fan2_now(cm)        (cell[pcb_addr].cell_pv[30+(cm*COMP_PV_NUM)])   // {���2��ǰת��}  
#define pv_cell_EC1_warn(cm)        (cell[pcb_addr].cell_pv[31+(cm*COMP_PV_NUM)])   // {EC���1������Ϣ}
#define pv_cell_EC2_warn(cm)        (cell[pcb_addr].cell_pv[32+(cm*COMP_PV_NUM)])   // {EC���2������Ϣ}
#define pv_cell_TRFgas(cm)          (cell[pcb_addr].cell_pv[33+(cm*COMP_PV_NUM)])	/* ��ý�����¶� */
#define pv_cell_TRFliq(cm)          (cell[pcb_addr].cell_pv[34+(cm*COMP_PV_NUM)])	/* ��ýҺ���¶� */
#define pv_cell_power_0(cm)         (cell[pcb_addr].cell_pv[35+(cm*COMP_PV_NUM)])   /* ģ�鹦�� */

/* ģ�鹦�ʣ�ģ�鼶�ӿ� */
#define pv_cell_power(pcb_num)      (cell[pcb_num].cell_pv[35+(0*COMP_PV_NUM)])   /* ģ�鹦�� */

/* ��������CELL_PV_BEGIN */

#define pv_cell_fan_set(n)          (cell[pcb_addr].cell_pv[27+n*2+(0*COMP_PV_NUM)])// {���Ŀ������}     
#define pv_cell_fan_now(n)          (cell[pcb_addr].cell_pv[28+n*2+(0*COMP_PV_NUM)])// {�����ǰת��}     
#define pv_cell_EC_warn(n)          (cell[pcb_addr].cell_pv[31+n+(0*COMP_PV_NUM)])  // {EC���������Ϣ}

/* ������ѹ���¶�ȡ���ߣ����������ͨ�����������¹��߱��� */
#define pv_gas_out_high(cm)             fun_pv_max(pv_cell_gas_out(cm), pv_cell_comp_top(cm))

/* �����¶ȣ������¶Ȼ����������¶� */
#define pv_evap_temp(cm)                fun_pv_get(pv_cell_evap_temp(cm), pv_cell_evap(cm))

// ģ��_��Ԫģ��������
#define pv_cell_TWin1(pcb_num)          (cell[pcb_num].cell_pv[ 0+CELL_PV_BEGIN])	/* �廻��ˮ�¶� */
#define pv_cell_TWout1(pcb_num)         (cell[pcb_num].cell_pv[ 1+CELL_PV_BEGIN])	/* �廻��ˮ�¶� */
#define pv_cell_TWout2(pcb_num)         (cell[pcb_num].cell_pv[ 2+CELL_PV_BEGIN])	/* �ܳ�ˮ�¶� */
#define pv_cell_TA(pcb_num)             (cell[pcb_num].cell_pv[ 3+CELL_PV_BEGIN])	/* ģ�黷���¶� */
#define pv_cell_PUMPf_set(pcb_num)      (cell[pcb_num].cell_pv[ 4+CELL_PV_BEGIN])	/* ��Ƶˮ��Ŀ��ת�� */
#define pv_cell_PUMPf_cur(pcb_num)      (cell[pcb_num].cell_pv[ 5+CELL_PV_BEGIN])	/* ��Ƶˮ�õ�ǰ���� */
#define pv_cell_PUMPf_pwm_out(pcb_num)  (cell[pcb_num].cell_pv[ 6+CELL_PV_BEGIN])	/* ��Ƶˮ��PWM�����ź� */
#define pv_cell_PUMPf_pwm_in(pcb_num)   (cell[pcb_num].cell_pv[ 7+CELL_PV_BEGIN])	/* ��Ƶˮ��PWM�����ź� */
#define pv_cell_PUMPf_err_code(pcb_num) (cell[pcb_num].cell_pv[ 8+CELL_PV_BEGIN])	/* ��Ƶˮ�ù�����Ϣ */
#define pv_cell_mac_power(pcb_num)      (cell[pcb_num].cell_pv[ 9+CELL_PV_BEGIN])	/* ����궨���� */
#define pv_cell_mac_power_now(pcb_num)  (cell[pcb_num].cell_pv[10+CELL_PV_BEGIN])	/* ���鵱ǰ���� */
#define pv_cell_PUMPf_pow(pcb_num)      (cell[pcb_num].cell_pv[11+CELL_PV_BEGIN])	/* ��Ƶˮ�õ�ǰ���� */

/*-------------------------------------------------------------*/



/*---- ��������Ϣ����---- */
/* ϵͳ��ʹ��ģʽ */
#define SYS_COOL_CAN_USE        0x0001      /* ������� */
#define SYS_HEAT_CAN_USE        0x0002      /* ���ȿ��� */
#define SYS_HOT_CAN_USE         0x0004      /* ��ˮ���� */
#define SYS_CHOT_PART_CAN_USE   0x0008      /* ����+������ˮ���� */
#define SYS_CHOT_CAN_USE        0x0010      /* ����+��ˮ���� */
#define SYS_HHOT_CAN_USE        0x0020      /* ����+��ˮ���� */
#define SYS_AUTO_CAN_USE        0x0040      /* �Զ����� */

//enum STEP         { COMP_IDLE, START_FAN, START_VAVLE, START_COMP};



/**�ڲ�����**************************************************/
#define MAX_DEVELOP_EXC     3
COMMON_EXT U16 Unknown_Exception[MAX_DEVELOP_EXC]; // δ֪�쳣 

#define DATA_EXCEPTION  0           // �����쳣 Unknown_Exception[DATA_EXCEPTION] ------------------------------------
    #define DATA_BREAK_BOUNDS   0   // ���ݷ��ʳ���


//#define OTHER_EXCEPTION  1       // �����쳣------------------------------------
  



#endif
