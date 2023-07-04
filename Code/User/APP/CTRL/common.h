#ifndef COMMON_H
#define COMMON_H    1
#ifdef  COMMON_GLOBAL
#define COMMON_EXT
#else 
#define COMMON_EXT extern
#endif

typedef void *PVOID;

//测试存储变量
//COMMON_EXT U16 debug_cs[16];

/* 模块类型 */
#define TYPE_MASTER             (0)         /* 主板 */
#define TYPE_SLAVER             (1)         /* 从板 */

/* 水路类型 */
#define ZONE_AC                 0	/* 空调区 */
#define ZONE_HOTW               1	/* 热水区 */

/* 拥有双水路系统 */
#define IS_DUAL_SYS             (val_pcb_num>1 && val_hotw_pcb_num<val_pcb_num)

/* 子系统类型 */
#define SUB_SYS_AC_HOTW         (0)         /* 空调+热水子系统 */
#define SUB_SYS_AC_ONLY         (1)         /* 单空调子系统 */
#define SUB_SYS_TYPE_MAX        (2)

/* 获取某个子系统中起始/结束模块号 */
#define PCB_BGN(type)           (((type) == SUB_SYS_AC_ONLY) ? val_hotw_pcb_num : 0)
#define PCB_END(type)           (((type) == SUB_SYS_AC_HOTW) ? val_hotw_pcb_num : val_pcb_num)

/* 获取当前模块所在子系统 */
#define GET_SUB_SYS(pcb_num)    (((pcb_num) < PCB_END(SUB_SYS_AC_HOTW)) ? SUB_SYS_AC_HOTW\
                                                                        : SUB_SYS_AC_ONLY)

/* 获取当前模块所在子系统是否需要运行 */
#define SUB_SYS_AC_HOTW_ON     ((fg_zone_A_run || fg_zone_B_run || fg_hotw_run) || fg_run_sta_hotw_antifreeze)
#define SUB_SYS_AC_ONLY_ON     ((fg_zone_A_run || fg_zone_B_run) || fg_run_sta_ac_antifreeze)
#define GET_SUB_SYS_ON(pcb_num)((energy_ctrl_type == JOINT_ENERGY_CTRL) ? TRUE \
                                                                        : (GET_SUB_SYS(pcb_num) == SUB_SYS_AC_HOTW) ? SUB_SYS_AC_HOTW_ON \
                                                                                                                    : SUB_SYS_AC_ONLY_ON)

/*----------------命令--------------------------------*/
/* 压机命令 */  //注意压机命令是字符型
#define CMD_OPEN_COMP           0x0001          /* 开压机 */
#define CMD_CLOSE_COMP          0x0002          /* 关压机 */
#define CMD_QUICK_STOP_COMP     0x0004          /* 急停压机 */
#define CMD_STOP_COMP_SYSTEM    0x0008          /* 停压机系统 */
#define CMD_CM_MODE                  4          //压机模式

#define CMD_ALLOW_DEFROST       0x0100          /* 压机允许除霜 */
#define CMD_FORCE_DEFROST       0x0200          /* 压机强制除霜 */

/* 模块命令 */
#define CMD_STOP_ALL_COMP       0x0001          /* 停所有压机 */
#define CMD_STOP_ALL_MAC        0x0002          /* 关闭所有输出 */
#define CMD_ERR_RESET           0x0004          /* 复位 */
#define CMD_FAC_PARA_INIT       0x0008          /* 厂家参数初始化 */
#define CMD_RUNTIME_INIT        0x0010          /* 压机累计运行时间初始化 */
#define CMD_MODE                5               /* 模式命令起始位置5~7 */

#define CMD_OPEN_CELL           0x0100		    /* 开模块 */
#define CMD_CLOSE_CELL          0x0200		    /* 关模块 */
#define CMD_LIMIT_INIT          0x0400          /* 系统维护参数初始化 */
#define CMD_SYSTIME_INIT        0x0800          /* 系统累计运行时间初始化 */

#define CMD_SYSTIME_SAVE        0x1000          /* 系统累计运行时间保存 */
#define CMD_PASSWORD_INIT       0x4000          // 密码初始化
#define CMD_INVT_INIT           0x8000          // 变频器初始化

/*----------------状态--------------------------------*/
/* 第一组状态 */
#define COMP_RUN                0x0001          /* 压机运行状态 */
#define COMP_RUN_NEED           0x0002          /* 压机需求 */
#define COMP_CAN_OPEN           0x0004          /* 可开 */
#define COMP_CAN_CLOSE          0x0008          /* 可关 */
#define COMP_ADD_PRIO           0x0010          /* 压机加载优先 */
#define COMP_SUB_PRIO           0x0020          /* 压机卸载优先 */
#define COMP_MODE               6               /* 当前运行模式起始位置（占用6、7、8位） */


/* 压机可使用模式定义 */
#define C_MODE_COOL             0x0200          /* 制冷 */  /* 可用运行模式（占用9、10位） */
#define C_MODE_HEAT             0x0400          /* 制热 */

#define COMP_CAN_ADD            0x0800          /* 压机可升频 */
#define COMP_CAN_SUB            0x1000          /* 压机可降频 */ 

#define COMP_DEFROST            0x2000          /* 除霜状态 */
#define COMP_DEFROST_APPLY      0x4000          /* 压机除霜申请 */
#define COMP_FORCE_DEFROST      0x8000          /* 强制除霜状态 */


/* 模块(单元)状态定义 */
#define CELL_RUN                  0x0001	/* 单元运行 */
#define CELL_ERR                  0x0002	/* 模块故障 */
#define CELL_PUMP_OPEN            0x0004	/* 模块水泵开启 */
#define CELL_COMP                 0x0030	/* 单元压机数(第4 5 位) */
#define CELL_CAN_RUN_HOTW         0x0040	/* 单元可运行制热水 */
#define CELL_FORCE_IBH_OFF        0x0080	/* 单元强开辅助电热关闭 */
#define CELL_PUMP_RUN             0x0100	/* 模块水泵稳定运行 */

/*----------------------------------------------------*/
/* 主模块定义 */
/* 系统故障 */
#define M_ERR_PHASE_REVERSE      1  // 错相保护 
#define M_ERR_EEPROM             2  /* EEPROM数据错故障 */  
//#define M_ERR_SYS_ENVIRON        3  /* 系统环温故障 */  
//#define M_ERR_SYS_IN             4  /* 系统回温故障 */  
#define M_ERR_TWOUT2             5  /* 总出水温度故障 */  
#define M_ERR_EEPROM_LIMIT       6  /* 系统维护数据错*/
#define M_ERR_PHASE_LACK         7  // 缺相保护
//#define M_ERR_LACK_FAN           8  // 送风压差
//#define M_ERR_WATER_FLOW         9  // 水流不足
#define M_ERR_TBH_OL            10			/* 水箱电热过载故障 */
//#define M_ERR_IBH_OL            11			/* 备用故障 */
#define M_ERR_THWT              12			/* 热水水箱温度故障 */
//#define M_ERR_OUT_T1            13			/* 备用故障 */
#define M_ERR_TWOUT3            14			/* 系统总出水温度故障 */
#define M_ERR_TFLIN             15			/* 地暖总进温故障 */
#define M_ERR_FCU_ERR_COMMU1    16  // 风盘01通讯故障 
#define M_ERR_FCU_ERR_COMMU2    17  // 风盘02通讯故障 
#define M_ERR_FCU_ERR_COMMU3    18  // 风盘03通讯故障 
#define M_ERR_FCU_ERR_COMMU4    19  // 风盘04通讯故障 
#define M_ERR_FCU_ERR_COMMU5    20  // 风盘05通讯故障 
#define M_ERR_FCU_ERR_COMMU6    21  // 风盘06通讯故障 
#define M_ERR_FCU_ERR_COMMU7    22  // 风盘07通讯故障 
#define M_ERR_FCU_ERR_COMMU8    23  // 风盘08通讯故障 
#define M_ERR_FCU_ERR_COMMU9    24  // 风盘09通讯故障 
#define M_ERR_FCU_ERR_COMMU10   25  // 风盘10通讯故障  
#define M_ERR_FCU_ERR_COMMU11   26  // 风盘11通讯故障  
#define M_ERR_FCU_ERR_COMMU12   27  // 风盘12通讯故障  
#define M_ERR_FCU_ERR_COMMU13   28  // 风盘13通讯故障  
#define M_ERR_FCU_ERR_COMMU14   29  // 风盘14通讯故障  
#define M_ERR_FCU_ERR_COMMU15   30  // 风盘15通讯故障  
#define M_ERR_FCU_ERR_COMMU16   31  // 风盘16通讯故障  
#define M_ERR_FCU_ERR_ERR1      32  // 风盘01故障 
#define M_ERR_FCU_ERR_ERR2      33  // 风盘02故障 
#define M_ERR_FCU_ERR_ERR3      34  // 风盘03故障 
#define M_ERR_FCU_ERR_ERR4      35  // 风盘04故障 
#define M_ERR_FCU_ERR_ERR5      36  // 风盘05故障 
#define M_ERR_FCU_ERR_ERR6      37  // 风盘06故障 
#define M_ERR_FCU_ERR_ERR7      38  // 风盘07故障 
#define M_ERR_FCU_ERR_ERR8      39  // 风盘08故障 
#define M_ERR_FCU_ERR_ERR9      40  // 风盘09故障 
#define M_ERR_FCU_ERR_ERR10     41  // 风盘10故障  
#define M_ERR_FCU_ERR_ERR11     42  // 风盘11故障  
#define M_ERR_FCU_ERR_ERR12     43  // 风盘12故障  
#define M_ERR_FCU_ERR_ERR13     44  // 风盘13故障  
#define M_ERR_FCU_ERR_ERR14     45  // 风盘14故障  
#define M_ERR_FCU_ERR_ERR15     46  // 风盘15故障  
#define M_ERR_FCU_ERR_ERR16     47  // 风盘16故障 
#define M_ERR_FCU_ERR_COMMU17   48  // 风盘17通讯故障 
#define M_ERR_FCU_ERR_COMMU18   49  // 风盘18通讯故障 
#define M_ERR_FCU_ERR_COMMU19   50  // 风盘19通讯故障
#define M_ERR_FCU_ERR_COMMU20   51  // 风盘20通讯故障
#define M_ERR_FCU_ERR_COMMU21   52  // 风盘21通讯故障
#define M_ERR_FCU_ERR_COMMU22   53  // 风盘22通讯故障
#define M_ERR_FCU_ERR_COMMU23   54  // 风盘23通讯故障
#define M_ERR_FCU_ERR_COMMU24   55  // 风盘24通讯故障
#define M_ERR_FCU_ERR_COMMU25   56  // 风盘25通讯故障
#define M_ERR_FCU_ERR_COMMU26   57  // 风盘26通讯故障
#define M_ERR_FCU_ERR_COMMU27   58  // 风盘27通讯故障
#define M_ERR_FCU_ERR_COMMU28   59  // 风盘28通讯故障
#define M_ERR_FCU_ERR_COMMU29   60  // 风盘29通讯故障
#define M_ERR_FCU_ERR_COMMU30   61  // 风盘30通讯故障
#define M_ERR_FCU_ERR_COMMU31   62  // 风盘31通讯故障
#define M_ERR_FCU_ERR_COMMU32   63  // 风盘32通讯故障
#define M_ERR_FCU_ERR_ERR17     64  // 风盘17故障 
#define M_ERR_FCU_ERR_ERR18     65  // 风盘18故障 
#define M_ERR_FCU_ERR_ERR19     66  // 风盘19故障
#define M_ERR_FCU_ERR_ERR20     67  // 风盘20故障
#define M_ERR_FCU_ERR_ERR21     68  // 风盘21故障
#define M_ERR_FCU_ERR_ERR22     69  // 风盘22故障
#define M_ERR_FCU_ERR_ERR23     70  // 风盘23故障
#define M_ERR_FCU_ERR_ERR24     71  // 风盘24故障
#define M_ERR_FCU_ERR_ERR25     72  // 风盘25故障
#define M_ERR_FCU_ERR_ERR26     73  // 风盘26故障
#define M_ERR_FCU_ERR_ERR27     74  // 风盘27故障
#define M_ERR_FCU_ERR_ERR28     75  // 风盘28故障
#define M_ERR_FCU_ERR_ERR29     76  // 风盘29故障
#define M_ERR_FCU_ERR_ERR30     77  // 风盘30故障
#define M_ERR_FCU_ERR_ERR31     78  // 风盘31故障
#define M_ERR_FCU_ERR_ERR32     79  // 风盘32故障
#define M_ERR_TACT1             80			/* 缓冲水箱上部温度故障 */
#define M_ERR_TACT2             81			/* 缓冲水箱下部温度故障 */
#define M_ERR_TSL               82			/* 太阳能温度故障 */
#define M_ERR_UI35_COMMU_ERR    83			/* 线控器通讯故障 */
#define M_ERR_TWOUT2_AC_ERR     84			/* 空调总出水温度故障 */
#define M_ERR_TR                85			/* 室内温度故障 */
#define M_ERR_PUMPF             86			/* 变频水泵故障 */
#define M_ERR_MAX  (MAX_ERR*ERR_BIT_NUM) // 96
enum
{
    AUTOERR_M_PS_LACK = 0    ,// 缺相（系统）
    AUTOERR_M_PS_REV         ,// 逆相（系统）
    AUTOERR_M_BAK2           ,
    AUTOERR_M_BAK3           ,
    AUTOERR_M_BAK4           ,
    AUTOERR_M_BAK5           ,
    AUTOERR_M_BAK6           ,
    AUTOERR_M_BAK7           ,
    AUTOERR_LP_DI_1          ,// 压机1低压（开关）
    AUTOERR_HP_DI_1          ,// 压机1高压（开关）
    AUTOERR_LP_AI_1          ,// 压机1低压（压传）
    AUTOERR_HP_AI_1          ,// 压机1高压（压传）
    AUTOERR_GAS_1            ,// 压机1排气过高
    AUTOERR_GASIN_LOW_1      ,// 吸气温度过低
    AUTOERR_EVAP_LOW_1       ,// 制冷蒸发过低
    AUTOERR_S_IN_HIGH        ,// 回温过高
    AUTOERR_S_IN_LOW         ,// 回温过低
    AUTOERR_S_OUT_IN_DIFF    ,// 出回温差过大
    AUTOERR_S_OUT_IN_ERR     ,// 出回温差异常
//    AUTOERR_S_CUR_HIGH       ,// 压机电流过高
    AUTOERR_S_AC_FLUX        ,// 水流开关（模块）
    AUTOERR_S_OUT_HIGH       ,// 出温过高（模块）
    AUTOERR_S_OUT_LOW        ,// 出温过低（模块）
    AUTOERR_S_INVT           ,// 变频故障
    AUTOERR_S_PWM_FAN_EXCEPT ,// 风机转速异常
    AUTOERR_EVAP_HIGH        ,// 蒸发温度过高
    MAX_AUTO_RESET_NUM
};

/* 自动复位变量 */
#define MAX_ALLOW_TIMES  2          /* 自动复位故障在1小时内的最多允许次数(超过该次数就需手动复位) */   

#define val_am_lock_time_min    (QK_60*30)  /* AM故障锁定时间最小值 */
#define val_am_lock_time_add    (QK_60*30)  /* AM故障锁定时间增量 */
#define val_am_lock_time_max    (QK_60*240) /* AM故障锁定时间最大值 */

typedef struct tagAUTORESETERROR 
{
    U16  dly;           /* 自动复位计时 */
    U08  en;            /* 自动复位使能标志位 */
    U08  auto_en;       /* 云推送故障可清除自动复位故障标志位*/
    U08  cnt;           /* 自动复位次数计数 */
    U08  have_reset;    /* 已经复位过一次标志位 */
    U16  reset_time[MAX_ALLOW_TIMES];       /* 自动复位故障计时 */  
    U16  err_time;                          /* 当前故障计时 */
    U16  err_time_lock;                     /* 故障锁定时间 */
} AUTORESETERROR;
COMMON_EXT AUTORESETERROR  autoERR[MAX_AUTO_RESET_NUM]; // 注意使用位置固定，不要随便调整

/* 注意同步更新assist_info */
/* 输入定义 */
#define IN_LINK             0           /* 二通阀连锁 */
#define IN_REMOTE           1           /* 线控开关 */
//#define IN_ELE_HEAT       2           /* 电辅热保护 */    /* N0002 */
#define IN_SEND_FAN_PRESS   3           /* 送风压差 */  
//#define M_IN_WATER_FLOW    4           // 水流不足		/* 实际无系统水流开关 */
#define IN_MODE_COOL        5           // 制冷开关
#define IN_MODE_HEAT        6           // 制热开关
//#define S_IN_IBH_OL           7           // 辅助电热反馈
#define M_DI_TBH_OL           8           // 水箱电热反馈
#define IN_RT_COOL          9				/* 温控器制冷信号 */
#define IN_RT_HEAT          10				/* 温控器制热信号 */
#define IN_SOLAR            11				/* 太阳能信号 */
#define IN_SG               12          // 智能电网SG信号
#define IN_EVU              13          // 智能电网EVU信号


/* 输出定义 */
#define SW_AC_PM_WATER      1				/* 空调泵 */
#define SW_AC_HT            2				/* 电辅热 */
#define SW_END_PM           6				/* 末端泵 */
#define SW_AC_PM_AIR        7				/* 送风机 */
 
#define M_DO_DFR              (0)			/* 除霜指示 */
#define M_DO_ERR              (1)			/* 故障指示 */
#define M_DO_AHS              (2)			/* 外部热源 */
#define M_DO_S3V1             (3)			/* 电磁三通阀1 */
#define M_DO_S3V3             (4)			/* 电磁三通阀3 */
#define M_DO_S3V2             (5)			/* 电磁三通阀2 */
#define M_DO_TBH              (6)			/* 水箱电热 */
#define M_DO_BAK_07           (7)			/* 备用 */
#define M_DO_PUMPret          (8)			/* 生活水箱水泵 */
#define M_DO_PUMPsl           (9)			/* 太阳能水泵 */
#define M_DO_PUMPm            (10)			/* 混合水泵 */
#define M_DO_PUMPo            (11)			/* 外循环泵 */
#define M_DO_PUMP             (12)			/* 水泵 */
#define M_DO_BAK_13           (13)			/* 备用 */


#define SW_AC_PM                (AIR_IS_WATER?SW_AC_PM_WATER    : SW_AC_PM_AIR)     // 空调泵\送风机 

#define get_output(x, y)        GET_BIT(cell[x].cell_relay[(y)/ RLY_BIT_NUM], (y)% RLY_BIT_NUM)
#define set_output(x, y)        SET_BIT(cell[x].cell_relay[(y)/ RLY_BIT_NUM], (y)% RLY_BIT_NUM)
#define clr_output(x, y)        CLR_BIT(cell[x].cell_relay[(y)/ RLY_BIT_NUM], (y)% RLY_BIT_NUM)


/*--------------------------------------------------------------------*/
/* 子模块定义 */
#define CELL_ERR_BEGIN          64 // 压机故障数:模块-单元故障偏移量：
#define CELL_ERR_NUM            32 // 单元故障数

                                                            
/* 压机故障定义 */
#define ERR_LP_1                0   // {压机低压}       
#define ERR_HP_1                1   // {压机高压}       
//#define ERR_CURR_HIGH_1         2   // {压机电流过大}   
//#define ERR_CURR_LOW_1          3   // {压机电流过小}   
#define ERR_WING_1              4   // {翅片探头故障}   
#define ERR_GAS_OUT_1           5   // {排气探头故障}   
#define ERR_EVAP_HIGH_1         6   // {排气温度过高}   
#define ERR_J5_SENSOR_1         7   // {J5压传故障}   
#define ERR_J6_SENSOR_1         8   // {J6压传故障}   
#define ERR_LP_AI_1             9   // {压传低压过低}   
#define ERR_HP_AI_1             10  // {压传高压过高}   
#define ERR_GAS_IN_1            11  // {吸气温度故障}   
#define ERR_EVAP_IN_1           12  // {阀后温度故障}   
#define ERR_GAS_IN_LOW_1        13  // {吸气温度过低}   
#define ERR_EMERG_DEFROST_1     14  // {紧急除霜频繁}   
#define ERR_GAS_IN_OUT_ERR_1    15  // {吸排温差异常}   
#define ERR_EVAP_LOW_1          16  // {制冷蒸发过低}   
//#define ERR_IN_SUB_FREQ_1       17  // {降频开关}       
#define ERR_TA_LIMIT_COMP_ON    18  // {环温限制压机开启}            
#define ERR_IN_LOW              19  // {回温温度过低}   
#define ERR_IN_HIGH             20  // {回温温度过高}   
#define ERR_PWM_FAN1_EXCEPT     21  // {风机1转速异常}  
#define ERR_PWM_FAN2_EXCEPT     22  // {风机2转速异常}  
#define ERR_EC1_COMMU           23  // {EC1通讯故障}    
#define ERR_EC1_ERR             24  // {EC1故障}        
#define ERR_EC2_COMMU           25  // {EC2通讯故障}    
#define ERR_EC2_ERR             26  // {EC2故障}        
#define ERR_INVT_COMMU          27  // {变频通讯故障}   
#define ERR_INVT_ERR            28  // {变频故障}       
#define ERR_INVT_MODEL          29  // {变频型号设置中} 
#define ERR_TEMP_EVI_IN         30  // {经济器入口温度故障}   
#define ERR_TEMP_EVI_OUT        31  // {经济器出口温度故障}  
#define ERR_TRFgas_SENSOR       32	/* 冷媒气侧温度故障 */
#define ERR_TRFliq_SENSOR       33	/* 冷媒液侧温度故障 */
#define ERR_EVAP_HIGH           34  // {蒸发温度过高}

// 单元故障
#define S_ERR_TEMP_TA           CELL_ERR_BEGIN+0			/* 环境温度故障 */
#define S_ERR_CASCADE_COMMU     CELL_ERR_BEGIN+1			/* 通讯故障  */
#define S_ERR_EEPROM            CELL_ERR_BEGIN+2			/* EEPROM数据错 */
#define S_ERR_IBH_OVERLOAD      CELL_ERR_BEGIN+3			/* 辅助电热过载 */
//#define S_ERR_TBH_OVERLOAD      CELL_ERR_BEGIN+4			/* 水箱电热过载 */
#define S_ERR_OUT_LOW           CELL_ERR_BEGIN+5			/* 板换出水温度过低 */
#define S_ERR_OUT_HIGH          CELL_ERR_BEGIN+6			/* 板换出水温度过高 */
#define S_ERR_TEMP_TWOUT1       CELL_ERR_BEGIN+7			/* 板换出水温度故障 */
#define S_ERR_WATER_FLOW        CELL_ERR_BEGIN+8			/* 水流不足 */
#define S_ERR_PHASE_LACK        CELL_ERR_BEGIN+9			/* 缺相保护 */
#define S_ERR_TEMP_TWIN1        CELL_ERR_BEGIN+10			/* 板换回水温度故障 */
#define S_ERR_TEMP_TWOUT2       CELL_ERR_BEGIN+11			/* 总出水温度故障 */
#define S_ERR_PHASE_REVERSE     CELL_ERR_BEGIN+12			/* 错相保护 */
#define S_ERR_OUT_IN_DIFF       CELL_ERR_BEGIN+13			/* 板换出回温差过大 */
#define S_ERR_OUT_IN_ERR        CELL_ERR_BEGIN+14			/* 板换出回温差异常 */
#define S_ERR_PUMPF_WARN        CELL_ERR_BEGIN+15			/* 变频水泵警告 */
#define S_ERR_COMMU_EXPT        CELL_ERR_BEGIN+16			/* 通讯异常 */
#define S_ERR_COMMU_INOUT       CELL_ERR_BEGIN+17			/* 内外机通讯异常 */
#define S_ERR_VER_INOUT         CELL_ERR_BEGIN+18			/* 协议版本过低 */
#define S_ERR_PUMPF             CELL_ERR_BEGIN+19			/* 变频水泵故障 */
#define S_ERR_MAC_ERR           CELL_ERR_BEGIN+20			/* 机型设置异常 */

/* 以下为未用到的故障 */
//#define S_ERR_POWER			/* 电源故障 */
//#define S_ERR_COOL_FLUX_OL	/* 风机过载 */
//#define S_ERR_ANTIFREZZ		/* 防冰温度过低 */
//#define S_ERR_TEMP_ANTIFREZZ  /* 防冰探头故障 */

#define F_NO_ERR                (CELL_ERR_BEGIN+CELL_ERR_NUM)   /* 无故障, +1后为0,从头开始 */
#define ERR_BAK                 F_NO_ERR

#define ERR_TOTAL               ((MAX_PCB+1)*F_NO_ERR) // 最大模块数对应的总故障数

//压传序号
typedef enum
{
    PRS_ID_1,       //低压
    PRS_ID_2,       //高压
    PRS_ID_MAX      //压传个数
}PRS_ID_ENUM;

/*--------------------------------------------------------------------*/
/* 注意同步更新assist_info */
/* 压机输入定义 */
#define IN_LP           0               /* 压机低压 */
#define IN_HP           1               /* 压机高压 */

// 单元开关量输入           /* 单元逻辑地址与压机逻辑地址共用，因在联网监控的地址是共用一字节 */
#define S_IN_IBH_OL       0	/* 辅助电热反馈 */
#define S_IN_WATER_FLOW      1	/* 水流开关 */

//#define IN_FAN_OL       2               /* 冷却风机保护 */
//#define IN_POWER_ERR    4               /* 电源故障 */
//#define IN_SUB_FREQ     5               /* 降频开关：中压开关等 */  /* N0004 */

/*--------------------------------------------------------------------*/
/* 输出定义 */
// 压机输出定义 对应cell_relay[0~MAX_COMP-1]
#define SW_CM           0           /* 压机 */
#define SW_S1           1           /* 四通阀 */
#define SW_byway        2           /* 喷液阀 */
#define SW_FAN_LOW      3           /* 低速风机：风机1 */
#define SW_FAN_HIG      4           /* 高速风机：风机2 */
#define SW_crank1       5           /* 曲轴电加热1 */
#define SW_EVI          6           /* 增焓阀 */
#define SW_crank2       7           /* 曲轴电加热2 */
/* 最多8个，已满 */

// 模块输出定义 对应cell_relay[MAX_COMP]  
#define S_DO_PUMPf        0	/* 变频水泵 */
#define S_DO_IBH1         1	/* 辅助电热1 */
#define S_DO_IBH2         2	/* 辅助电热2 */
#define S_DO_DPH          3	/* 底盘电加热 */
#define S_DO_HT           4	/* 防冻结电加热带 */
#define S_DO_DPH2         5	/* 底盘电加热2 */

//#define SW_valve_defr   1           /* 除霜阀 */

#define s_get_output(cm, n)     (GET_BIT(pCell->cell_relay[cm], (n)% RLY_BIT_NUM))
#define s_set_output(cm, n)     (SET_BIT(pCell->cell_relay[cm], (n)% RLY_BIT_NUM))
#define s_clr_output(cm, n)     (CLR_BIT(pCell->cell_relay[cm], (n)% RLY_BIT_NUM))


/* 送风机型与送水机型替换对应点 */
#define AIR_IS_WATER        (val_machine_type == AIR_WATER)     // 风冷送水
#define AIR_IS_AIR          (val_machine_type == AIR_AIR)       // 风冷送风

/*--------------------------------------------------------------------*/
// 模拟量定义 (温度为放大10倍的温度值)--------------------------------
#define SYS_PV_NUM      MAX_PV                          // 系统模拟量个数
#define COMP_PV_NUM     48                              // 单台压机模拟量个数                                     
#define CELL_PV_NUM     (MAX_PV-COMP_PV_NUM*USE_COMP)   // 单元模拟量个数
#define CELL_PV_BEGIN   (COMP_PV_NUM*USE_COMP)          // 单元模拟量起始位置


// 系统模拟量定义
#define pv_sys_TR                       (cell[MAX_PCB].cell_pv[ 0])	/* 室内温度 0.1度 */
#define pv_sys_bak_01                   (cell[MAX_PCB].cell_pv[ 1])	/* 备用 0.1℃ */
#define pv_sys_TA                       (cell[MAX_PCB].cell_pv[ 2])	/* 系统环温(模块平均值) 0.1℃ */
#define pv_sys_THWt                     (cell[MAX_PCB].cell_pv[ 3])	/* 热水水箱温度 0.1℃ */
#define pv_sys_TWout2                   (cell[MAX_PCB].cell_pv[ 4])	/* 系统出温(模块平均值) 0.1℃ */
#define pv_sys_TWout3                   (cell[MAX_PCB].cell_pv[ 5])	/* 系统总出水温度 0.1℃ */
#define pv_sys_TSL                      (cell[MAX_PCB].cell_pv[ 6])	/* 太阳能温度 0.1℃ */
#define pv_sys_TACt1                    (cell[MAX_PCB].cell_pv[ 7])	/* 缓冲水箱上部温度 0.1℃ */
#define pv_sys_TACt2                    (cell[MAX_PCB].cell_pv[ 8])	/* 缓冲水箱下部温度 0.1℃ */
#define pv_sys_TWout2_ac                (cell[MAX_PCB].cell_pv[ 9])	/* 空调总出水温度 0.1℃ */
#define pv_sys_bak_10                   (cell[MAX_PCB].cell_pv[10])	/* 备用 0.1℃ */
#define pv_sys_bak_11                   (cell[MAX_PCB].cell_pv[11])	/* 备用 0.1℃ */
#define pv_sys_bak_12                   (cell[MAX_PCB].cell_pv[12])	/* 备用 0.1℃ */
#define pv_sys_bak_13                   (cell[MAX_PCB].cell_pv[13])	/* 备用 0.1℃ */
#define pv_sys_TFLin                    (cell[MAX_PCB].cell_pv[14])	/* 地暖总进水温度 0.1℃ */
#define pv_sys_S3V1_state               (cell[MAX_PCB].cell_pv[15])	/* 电磁三通阀1状态 */
#define pv_sys_S3V2_state               (cell[MAX_PCB].cell_pv[16])	/* 电磁三通阀2状态 */
#define pv_sys_S3V3_state               (cell[MAX_PCB].cell_pv[17])	/* 电磁三通阀3状态 */
#define pv_sys_PUMPf_pwm_in             (cell[MAX_PCB].cell_pv[18])     // 备用
#define pv_sys_cl_pwr                   (cell[MAX_PCB].cell_pv[19])     // 制冷功率
#define pv_sys_ht_pwr                   (cell[MAX_PCB].cell_pv[20])     // 制热功率
#define pv_sys_hw_pwr                   (cell[MAX_PCB].cell_pv[21])     // 制热水功率

// 模块_压机模拟量定义
#define pv_cell_curr(cm)            (cell[pcb_addr].cell_pv[ 0+(cm*COMP_PV_NUM)])   // {电流}           
#define pv_cell_step_m(cm)          (cell[pcb_addr].cell_pv[ 1+(cm*COMP_PV_NUM)])   // {主电膨开度}     
#define pv_cell_step_s(cm)          (cell[pcb_addr].cell_pv[ 2+(cm*COMP_PV_NUM)])   // {辅电膨开度}     
#define pv_cell_wing(cm)            (cell[pcb_addr].cell_pv[ 3+(cm*COMP_PV_NUM)])   // {翅片温度}       
#define pv_cell_gas_out(cm)         (cell[pcb_addr].cell_pv[ 4+(cm*COMP_PV_NUM)])   // {排气温度}       
#define pv_cell_gas_in(cm)          (cell[pcb_addr].cell_pv[ 5+(cm*COMP_PV_NUM)])   // {吸气温度}       
#define pv_cell_evap(cm)            (cell[pcb_addr].cell_pv[ 6+(cm*COMP_PV_NUM)])   // {阀后温度}       
#define pv_cell_comp_top(cm)        (cell[pcb_addr].cell_pv[ 7+(cm*COMP_PV_NUM)])   // {压顶温度}       
#define pv_cell_evap_temp(cm)       (cell[pcb_addr].cell_pv[ 8+(cm*COMP_PV_NUM)])   // {蒸发饱和温度}   
#define pv_cell_cond_temp(cm)       (cell[pcb_addr].cell_pv[ 9+(cm*COMP_PV_NUM)])   // {冷凝饱和温度}   
#define pv_cell_lp(cm)              (cell[pcb_addr].cell_pv[10+(cm*COMP_PV_NUM)])   // {低压压力}       
#define pv_cell_hp(cm)              (cell[pcb_addr].cell_pv[11+(cm*COMP_PV_NUM)])   // {高压压力}       
#define pv_cell_eev_sv(cm)          (cell[pcb_addr].cell_pv[12+(cm*COMP_PV_NUM)])   // {主阀目标过热}   
#define pv_cell_eev_pv(cm)          (cell[pcb_addr].cell_pv[13+(cm*COMP_PV_NUM)])   // {主阀当前过热}   
#define pv_cell_evi_sv(cm)          (cell[pcb_addr].cell_pv[14+(cm*COMP_PV_NUM)])   // {辅阀目标过热}   
#define pv_cell_evi_pv(cm)          (cell[pcb_addr].cell_pv[15+(cm*COMP_PV_NUM)])   // {辅阀当前过热}   
#define pv_cell_evi_in(cm)          (cell[pcb_addr].cell_pv[16+(cm*COMP_PV_NUM)])   // {经济器入口温度} 
#define pv_cell_evi_out(cm)         (cell[pcb_addr].cell_pv[17+(cm*COMP_PV_NUM)])   // {经济器出口温度} 
#define pv_cell_nofrost_EWD(cm)     (cell[pcb_addr].cell_pv[18+(cm*COMP_PV_NUM)])   // {无霜运行环翅差} 
#define pv_cell_invt_set(cm)        (cell[pcb_addr].cell_pv[19+(cm*COMP_PV_NUM)])   // {变频目标转速}   
#define pv_cell_invt_cur(cm)        (cell[pcb_addr].cell_pv[20+(cm*COMP_PV_NUM)])   // {变频当前转速}   
#define pv_cell_invt_volt(cm)       (cell[pcb_addr].cell_pv[21+(cm*COMP_PV_NUM)])   // {变频输出电压}   
#define pv_cell_invt_curr(cm)       (cell[pcb_addr].cell_pv[22+(cm*COMP_PV_NUM)])   // {变频输出电流}   
#define pv_cell_invt_tmp(cm)        (cell[pcb_addr].cell_pv[23+(cm*COMP_PV_NUM)])   // {变频模块温度}   
#define pv_cell_invt_msg(cm)        (cell[pcb_addr].cell_pv[24+(cm*COMP_PV_NUM)])   // {变频限频信息}   
#define pv_cell_invt_curr_in(cm)    (cell[pcb_addr].cell_pv[25+(cm*COMP_PV_NUM)])   // {变频输入电流}   
#define pv_cell_invt_power_out(cm)  (cell[pcb_addr].cell_pv[26+(cm*COMP_PV_NUM)])   // {变频输出功率}   
#define pv_cell_fan1_set(cm)        (cell[pcb_addr].cell_pv[27+(cm*COMP_PV_NUM)])   // {风机1目标运行}  
#define pv_cell_fan1_now(cm)        (cell[pcb_addr].cell_pv[28+(cm*COMP_PV_NUM)])   // {风机1当前转速}  
#define pv_cell_fan2_set(cm)        (cell[pcb_addr].cell_pv[29+(cm*COMP_PV_NUM)])   // {风机2目标运行}  
#define pv_cell_fan2_now(cm)        (cell[pcb_addr].cell_pv[30+(cm*COMP_PV_NUM)])   // {风机2当前转速}  
#define pv_cell_EC1_warn(cm)        (cell[pcb_addr].cell_pv[31+(cm*COMP_PV_NUM)])   // {EC风机1警告信息}
#define pv_cell_EC2_warn(cm)        (cell[pcb_addr].cell_pv[32+(cm*COMP_PV_NUM)])   // {EC风机2警告信息}
#define pv_cell_TRFgas(cm)          (cell[pcb_addr].cell_pv[33+(cm*COMP_PV_NUM)])	/* 冷媒气侧温度 */
#define pv_cell_TRFliq(cm)          (cell[pcb_addr].cell_pv[34+(cm*COMP_PV_NUM)])	/* 冷媒液侧温度 */
#define pv_cell_power_0(cm)         (cell[pcb_addr].cell_pv[35+(cm*COMP_PV_NUM)])   /* 模块功率 */

/* 模块功率：模块级接口 */
#define pv_cell_power(pcb_num)      (cell[pcb_num].cell_pv[35+(0*COMP_PV_NUM)])   /* 模块功率 */

/* 最大个数：CELL_PV_BEGIN */

#define pv_cell_fan_set(n)          (cell[pcb_addr].cell_pv[27+n*2+(0*COMP_PV_NUM)])// {风机目标运行}     
#define pv_cell_fan_now(n)          (cell[pcb_addr].cell_pv[28+n*2+(0*COMP_PV_NUM)])// {风机当前转速}     
#define pv_cell_EC_warn(n)          (cell[pcb_addr].cell_pv[31+n+(0*COMP_PV_NUM)])  // {EC风机警告信息}

/* 排温与压顶温度取高者，用于替代普通的排温做排温过高保护 */
#define pv_gas_out_high(cm)             fun_pv_max(pv_cell_gas_out(cm), pv_cell_comp_top(cm))

/* 阀后温度：阀后温度或蒸发饱和温度 */
#define pv_evap_temp(cm)                fun_pv_get(pv_cell_evap_temp(cm), pv_cell_evap(cm))

// 模块_单元模拟量定义
#define pv_cell_TWin1(pcb_num)          (cell[pcb_num].cell_pv[ 0+CELL_PV_BEGIN])	/* 板换回水温度 */
#define pv_cell_TWout1(pcb_num)         (cell[pcb_num].cell_pv[ 1+CELL_PV_BEGIN])	/* 板换出水温度 */
#define pv_cell_TWout2(pcb_num)         (cell[pcb_num].cell_pv[ 2+CELL_PV_BEGIN])	/* 总出水温度 */
#define pv_cell_TA(pcb_num)             (cell[pcb_num].cell_pv[ 3+CELL_PV_BEGIN])	/* 模块环境温度 */
#define pv_cell_PUMPf_set(pcb_num)      (cell[pcb_num].cell_pv[ 4+CELL_PV_BEGIN])	/* 变频水泵目标转速 */
#define pv_cell_PUMPf_cur(pcb_num)      (cell[pcb_num].cell_pv[ 5+CELL_PV_BEGIN])	/* 变频水泵当前流量 */
#define pv_cell_PUMPf_pwm_out(pcb_num)  (cell[pcb_num].cell_pv[ 6+CELL_PV_BEGIN])	/* 变频水泵PWM控制信号 */
#define pv_cell_PUMPf_pwm_in(pcb_num)   (cell[pcb_num].cell_pv[ 7+CELL_PV_BEGIN])	/* 变频水泵PWM反馈信号 */
#define pv_cell_PUMPf_err_code(pcb_num) (cell[pcb_num].cell_pv[ 8+CELL_PV_BEGIN])	/* 变频水泵故障信息 */
#define pv_cell_mac_power(pcb_num)      (cell[pcb_num].cell_pv[ 9+CELL_PV_BEGIN])	/* 机组标定能力 */
#define pv_cell_mac_power_now(pcb_num)  (cell[pcb_num].cell_pv[10+CELL_PV_BEGIN])	/* 机组当前能力 */
#define pv_cell_PUMPf_pow(pcb_num)      (cell[pcb_num].cell_pv[11+CELL_PV_BEGIN])	/* 变频水泵当前功率 */

/*-------------------------------------------------------------*/



/*---- 公共区信息定义---- */
/* 系统可使用模式 */
#define SYS_COOL_CAN_USE        0x0001      /* 制冷可设 */
#define SYS_HEAT_CAN_USE        0x0002      /* 制热可设 */
#define SYS_HOT_CAN_USE         0x0004      /* 热水可设 */
#define SYS_CHOT_PART_CAN_USE   0x0008      /* 制冷+部分热水可设 */
#define SYS_CHOT_CAN_USE        0x0010      /* 制冷+热水可设 */
#define SYS_HHOT_CAN_USE        0x0020      /* 制热+热水可设 */
#define SYS_AUTO_CAN_USE        0x0040      /* 自动可设 */

//enum STEP         { COMP_IDLE, START_FAN, START_VAVLE, START_COMP};



/**内部故障**************************************************/
#define MAX_DEVELOP_EXC     3
COMMON_EXT U16 Unknown_Exception[MAX_DEVELOP_EXC]; // 未知异常 

#define DATA_EXCEPTION  0           // 数据异常 Unknown_Exception[DATA_EXCEPTION] ------------------------------------
    #define DATA_BREAK_BOUNDS   0   // 数据访问超界


//#define OTHER_EXCEPTION  1       // 其它异常------------------------------------
  



#endif
