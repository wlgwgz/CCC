/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: Test.h
 文件描述: 
 创建人  : 
 创建日期: 
*******************************************************************************/
#ifndef TEST_H
#define TEST_H

  
#ifdef  TEST_GLOBAL
 #define TEST_EXT
#else
 #define TEST_EXT extern
#endif


#define TestTbl				((BIT16 *)&CoreTbl[28].word)


// 测试命令,(预留位置：0~15 )
#define TestTbl_CMD			((BIT16 *)&TestTbl[0].word)

#define TestBackboardCmd        (TestTbl[0].byte.high)   /* 测试后板命令 */
#define TBC_EXIT                0                           /* 退出测试程序 */
#define TBC_ENTER               1                           /* 进入测试程序 */
#define TestBackboardType       (TestTbl[0].byte.low)    /* 测试后板型号 */
#define HL006A                  1
#define HL011A                  2
#define HL012A                  3
#define HL012A_EX               4

#define TestSwOutMode           (TestTbl[1].byte.high)      /* 测试继电器输出模式 */

#define TCmdTestEEPROM          (TestTbl[2].bits.b0)        /* 测试、初始化EEPROM */
#define TCmdTestSpiflash        (TestTbl[2].bits.b1)        /* 测试、初始化Spiflash */
#define TCmdInitEEPROM          (TestTbl[2].bits.b2)        /* 初始化eeprom */
#define TCmdClrUartRcd          (TestTbl[2].bits.b3)        /* 串口通讯记录清零 */
#define TCmdClrCurRcd			(TestTbl[2].bits.b4)        /* 电流记录复位 */
#define TCmdClrSwInRcd          (TestTbl[2].bits.b5)        /* 开关量\拨码开关跳变次数清零*/
#define TCmdClrTmpRcd           (TestTbl[2].bits.b6)        /* 温度记录复位 */
#define TCmdClrAIRcd            (TestTbl[2].bits.b7)        /* 模拟量记录复位 */
#define TCmdadjustAO            (TestTbl[2].bits.b8)        /* 模拟量输出校准 */
#define TCmdClrReset            (TestTbl[2].bits.b9)        /* 清零复位次数*/
#define TCmdadjustpt1000        (TestTbl[2].bits.b10)       /* PT1000开始校准*/
#define TCmdClrFrepRcd          (TestTbl[2].bits.b11)       /* 频率值复位*/
#define TCmdClrSwoutcount       (TestTbl[2].bits.b12)       /* 继电器计数*/

#define TCmdClrPwmInRcd         (TestTbl[2].bits.b14)       /* 清零PWM输入记录 */
//#define TCmdHavePvdTest         (TestTbl[2].bits.b14)       /* 是否有电膨测试 */
//#define TCmdDontAdjAi           (TestTbl[2].bits.b15)       /* 不校准AI */


// 步进电机测试命令,预留16个步进电机的正反转命令,(预留位置：3~4)

#define TestStepCmd				(TestTbl[3].word)   /* 步进电机命令 */


// 第二套命令(位置：8)
#define TCmd2ClrAdcAdjust        (TestTbl[8].bits.b0)       /* 重新校准ADC */



// 测试状态,(预留位置：16~39 )
#define TestTbl_STA			((BIT16 *)&TestTbl[16].word)

#define TestProgStaus           (TestTbl[16].word)      /* 测试程序状态 69*/
#define TPS_NO_TEST_CMD         0           /* 后板未收到进入测试命令 */
#define TPS_ENTER_TEST_INIT     1           /* 后板准备进入测试命令 */
#define TPS_ENTER_TEST_OK       2           /* 后板已进入测试状态 */
#define TPS_TYPE_ERR            3           /* 后板型号选择错误 */
#define TPS_DISENABLE_TEST      4           /* 不容许进入测试状态，如已进入工作程序 */

#define TestEepromCurPage       (TestTbl[18].word)      /* Eeprom当前测试页 */
#define TestEepromTotalPage     (TestTbl[19].word)      /* Eeprom需测试的总页数 */
#define TestSpiflashCurPage     (TestTbl[20].word)      /* Flash当前测试页 */
#define TestSpiflashTotalPage   (TestTbl[21].word)      /* Flash需测试的总页数 */

//状态：0为未测，1为测试中，2为测试成功，3为测试失败
#define TestEepromStauts        (TestTbl[22].word)      /* Eeprom测试状态 */
#define InitEepromStauts        (TestTbl[23].word)      /* Eeprom初始化状态 */
#define TestSpiflashStauts      (TestTbl[24].word)      /* Flash测试状态 */
#define InitSpiflashStauts      (TestTbl[25].word)      /* Flash初始化状态*/

// 测试数据-MAX_DI_NUM=8路开关量输入,(预留位置：40~47 (=40+8-1))
// 8个字=1个字的数字开关量+1个字拔码+1个字三相电+1个字拔盘+预留4个字

#define TEST_DI_BEGIN      40                                                                    // 开关量测试存储起始位置
#define TEST_DI_WORD       ((MAX_DI_TOTAL_NUM-1)/16+1)                                                 // 数字开关量占的字

#define TestTbl_DI			((BIT16 *)&TestTbl[TEST_DI_BEGIN].word)                             // 开关量测试数据
#define DI_dinput(n)                  (TestTbl[TEST_DI_BEGIN+n].word)                           // 数字开关量
#define SW_SWITCH    		          (TestTbl[TEST_DI_BEGIN+TEST_DI_WORD+0].word)              // 系统拔码
#define THR_PHASE			          (TestTbl[TEST_DI_BEGIN+TEST_DI_WORD+1].word)		        // 三相电
#define SR_SWITCH		              (TestTbl[TEST_DI_BEGIN+TEST_DI_WORD+2].word)		        // 拔盘


// 测试数据-MAX_NTC_NUM=11路温度(预留12路),(预留位置：48~119 (=48+6项*12路-1))
#define TEST_TEMP_BEGIN     48                                                                  // 温度测试存储起始位置
#define TEST_TEMP_ITEM       6                                                                  // 温度测试项目
#define TestTbl_NTC			((BIT16 *)&TestTbl[TEST_TEMP_BEGIN].word)                           // 温度测试数据

#define TestCurTmp(n)         (*(I16*)&TestTbl[TEST_TEMP_BEGIN+0+(n)*TEST_TEMP_ITEM].word)      /* 当前温度值 */
#define TestCurTmpMax(n)      (*(I16*)&TestTbl[TEST_TEMP_BEGIN+1+(n)*TEST_TEMP_ITEM].word)      /* 最大温度值 */
#define TestCurTmpMin(n)      (*(I16*)&TestTbl[TEST_TEMP_BEGIN+2+(n)*TEST_TEMP_ITEM].word)      /* 最小温度值 */
#define TestCurTmpAd(n)               (TestTbl[TEST_TEMP_BEGIN+3+(n)*TEST_TEMP_ITEM].word)      /* 当前温度ad值，已减去GND */
#define TestCurTmpAdMax(n)            (TestTbl[TEST_TEMP_BEGIN+4+(n)*TEST_TEMP_ITEM].word)      /* 最大温度ad值 */
#define TestCurTmpAdMin(n)            (TestTbl[TEST_TEMP_BEGIN+5+(n)*TEST_TEMP_ITEM].word)      /* 最小温度ad值 */

// 测试数据：预留72字节(120~191)


// 测试数据：MAX_CUR_CH_NUM=2路电流(预留5路)，(预留位置：192~221 (=192+6项*5路-1))
#define TEST_CURR_BEGIN     (TEST_TEMP_BEGIN+MAX_NTC_NUM*TEST_TEMP_ITEM)                        // 电流测试存储起始位置
#define TEST_CURR_ITEM       6                                                                  // 电流测试项目
#define TestTbl_CUR			((BIT16 *)&TestTbl[TEST_CURR_BEGIN].word)

#define TestCurCur(n)                 (TestTbl[TEST_CURR_BEGIN+0+(n)*TEST_CURR_ITEM].word)     /* 当前电流值 */
#define TestCurCurMax(n)              (TestTbl[TEST_CURR_BEGIN+1+(n)*TEST_CURR_ITEM].word)     /* 最大值 */
#define TestCurCurMin(n)              (TestTbl[TEST_CURR_BEGIN+2+(n)*TEST_CURR_ITEM].word)     /* 最小值 */
#define TestCurCurAd(n)               (TestTbl[TEST_CURR_BEGIN+3+(n)*TEST_CURR_ITEM].word)     /* 当前电流ad值*/
#define TestCurCurAdMax(n)            (TestTbl[TEST_CURR_BEGIN+4+(n)*TEST_CURR_ITEM].word)     /* 最大ad值 */
#define TestCurCurAdMin(n)            (TestTbl[TEST_CURR_BEGIN+5+(n)*TEST_CURR_ITEM].word)     /* 最小ad值 */

// 测试数据：预留30字节(222~251)


// 测试数据：MAX_AI_CH_NUM=4路模拟量(预留8路)，(模拟量类型 预留位置：252~259 (=252+1项*8路-1))
#define TEST_AI_TYPE_BEGIN  (252)                                                               // 模拟量输入类型存储起始位置
#define TestTbl_AI_TYPE		((BIT16 *)&TestTbl[TEST_AI_TYPE_BEGIN].word)
#define TestAIType(n)       (TestTbl_AI_TYPE[n].word)         

// 测试数据：MAX_AI_CH_NUM=4路模拟量(预留8路)，(模拟量数据 预留位置：260~307 (=260+6项*8路-1))
#define TEST_AI_DATA_BEGIN  (260)                                                               // 模拟量数据存储起始位置
#define TEST_AI_DATA_ITEM    6                                                                  // 模拟量测试个数                                                         // 模拟量输入数据存储起始位置
#define TestTbl_AI_DATA		((BIT16 *)&TestTbl[TEST_AI_DATA_BEGIN].word)

#define TestCurAI(n)                  (TestTbl[TEST_AI_DATA_BEGIN+0+(n)*TEST_AI_DATA_ITEM].word)   /* 当前值 */
#define TestCurAIMax(n)               (TestTbl[TEST_AI_DATA_BEGIN+1+(n)*TEST_AI_DATA_ITEM].word)   /* 最大值 */
#define TestCurAIMin(n)               (TestTbl[TEST_AI_DATA_BEGIN+2+(n)*TEST_AI_DATA_ITEM].word)   /* 最小值 */
#define TestCurAIAd(n)                (TestTbl[TEST_AI_DATA_BEGIN+3+(n)*TEST_AI_DATA_ITEM].word)   /* 当前ad值*/
#define TestCurAIAdMax(n)             (TestTbl[TEST_AI_DATA_BEGIN+4+(n)*TEST_AI_DATA_ITEM].word)   /* 最大ad值 */
#define TestCurAIAdMin(n)             (TestTbl[TEST_AI_DATA_BEGIN+5+(n)*TEST_AI_DATA_ITEM].word)   /* 最小ad值 */

// 测试数据：预留48字节(308~355)


// 测试数据：MAX_MOTOR=4路步进电机(预留8路)，(预留位置：356~363 (=356+1项*8路-1))
#define TEST_STEP_NOW_BEGIN     (356)                                                               // 步进电机当前值存储起始位置
#define TestTbl_STEP_NOW   	((BIT16 *)&TestTbl[TEST_STEP_NOW_BEGIN].word)

#define TestStepNow(n)                (TestTbl[TEST_STEP_NOW_BEGIN+(n)].word)                       // 当前步数

// 测试数据：MAX_MOTOR=4路步进电机(预留8路)，(预留位置：364~371 (=364+1项*8路-1))
#define TEST_STEP_SET_BEGIN     (364)                                                               // 步进电机设定值存储起始位置
#define TestTbl_STEP_SET   	((BIT16 *)&TestTbl[TEST_STEP_SET_BEGIN].word)

#define TestStepSet(n)                (TestTbl[TEST_STEP_SET_BEGIN+(n)].word)                       // 设定步数


// 测试数据：3路串口(预留5路)，(预留位置：372~416 (=372+9项*5路-1))
/*******************************************************************************
1、 请参考通讯记录结构COMRECORD定义处的注释 
2、 以下是后板对各串口的记录信息，当串口是从机时(广播、超时、失败、报警次数为0)，
    通讯测试时显示的信息一般为主机中记录的信息;当然也可以显示从机中记录的信息。
3、 以下MODBUS区域定义的顺序需与COMRECORD中计数器定义的顺序一致
*******************************************************************************/

#define TEST_COMMU_BEGIN    (372)
#define TEST_COMMU_ITEM     9 
#define TestTbl_COMMU		((BIT16 *)&TestTbl[TEST_COMMU_BEGIN].word)

#define BKBD_COMMU_RCD_PTR  ((COMRECORD *)&TestTbl[TEST_COMMU_BEGIN])     
#define BKBD_INVT_RCD_PTR       (BKBD_COMMU_RCD_PTR+0)


#define BKBD_INVT_UART0         (BKBD_COMMU_RCD_PTR+0)  //BMS监控即GD005A
#define BKBD_INVT_UART1         (BKBD_COMMU_RCD_PTR+1)  //级联
#define BKBD_INVT_UART2         (BKBD_COMMU_RCD_PTR+2)  //变频器
#define BKBD_INVT_UART3         (BKBD_COMMU_RCD_PTR+3)  //显示屏
#ifdef COM_4G
#define BKBD_INVT_UART4         (BKBD_COMMU_RCD_PTR+4)  //4G
#endif


// 测试数据：MAX_DI_NUM=8路开关量(预留16路)，开关量变化次数，(预留位置：417~432 (=417+1项*16路-1))
#define TEST_DI_DATA_BEGIN  (417)
#define TestTbl_DI_DATA		((BIT16 *)&TestTbl[TEST_DI_DATA_BEGIN].word)    // 开关量变化次数

#define TestSwInChange(n)       (TestTbl[417+n].word)        /* 开关量变化次数 24*/



// 测试数据：其它记录(预留位置：433~)
#define TestTimeDay             (TestTbl[433].word)     /* 天 */
#define TestTimeHour            (TestTbl[434].word)     /* 小时 */
#define TestTimeMin             (TestTbl[435].word)     /* 分 */
#define TestTimeSec             (TestTbl[436].word)     /* 秒 */

#define TestResetPowerCnt       (TestTbl[437].byte.low)/*上电复位计数*/
#define TestResetDMPCnt         (TestTbl[438].byte.high)/*电源域复位计数*/
#define TestResetUserCnt        (TestTbl[439].byte.low)/*内部应用/应用复位计数*/



//#define Testfbusnum             (TestTbl[?].word)    /* FBUS通讯模块个数 */
// 测试数据：保留（440~449）



// 测试数据：PWM预留2路         (预留位置：450~459 (=450+5项*2路-1))
#define TEST_PWM_DATA_BEGIN     (450)
#define TEST_PWM_DATA_ITEM      (5  )                // PWM测试字数
#define TestTbl_PWM			    ((BIT16 *)&TestTbl[TEST_PWM_DATA_BEGIN].word)                           // 温度测试数据
#define TestPwmOutFreq(n)       (TestTbl[TEST_PWM_DATA_BEGIN+0+(n)*TEST_PWM_DATA_ITEM].word)//PWM输出频率
#define TestPwmOutDuty(n)       (TestTbl[TEST_PWM_DATA_BEGIN+1+(n)*TEST_PWM_DATA_ITEM].word)//PWM输出占空比
#define TestPwmInCur(n)         (TestTbl[TEST_PWM_DATA_BEGIN+2+(n)*TEST_PWM_DATA_ITEM].word)//PWM输入当前占空比
#define TestPwmInMax(n)         (TestTbl[TEST_PWM_DATA_BEGIN+3+(n)*TEST_PWM_DATA_ITEM].word)//PWM输入最大占空比
#define TestPwmInMin(n)         (TestTbl[TEST_PWM_DATA_BEGIN+4+(n)*TEST_PWM_DATA_ITEM].word)//PWM输入最小占空比

// 测试数据：剩余（460~483）/* N0004 */
#define TEST_PWM2_DATA_BEGIN     (460)
#define TEST_PWM2_DATA_ITEM      (5  )                // PWM测试字数
#define TestTb2_PWM			    ((BIT16 *)&TestTbl[TEST_PWM2_DATA_BEGIN].word)                           // 温度测试数据
#define TestPwm2OutFreq(n)       (TestTbl[TEST_PWM2_DATA_BEGIN+0+(n)*TEST_PWM_DATA_ITEM].word)//PWM输出频率
#define TestPwm2OutDuty(n)       (TestTbl[TEST_PWM2_DATA_BEGIN+1+(n)*TEST_PWM_DATA_ITEM].word)//PWM输出占空比
#define TestPwm2InCur(n)         (TestTbl[TEST_PWM2_DATA_BEGIN+2+(n)*TEST_PWM_DATA_ITEM].word)//PWM输入当前频率
#define TestPwm2InMax(n)         (TestTbl[TEST_PWM2_DATA_BEGIN+3+(n)*TEST_PWM_DATA_ITEM].word)//PWM输入最大频率
#define TestPwm2InMin(n)         (TestTbl[TEST_PWM2_DATA_BEGIN+4+(n)*TEST_PWM_DATA_ITEM].word)//PWM输入最小频率

/* 测试数据：变频信息 */
typedef struct TEST_INVT
{
    U16 func;				/* 功能码 */
    U16 ver;				/* 版本号 */
    I16 fg_init;			/* 初始化 */
    I16 fg_open;			/* 开机 */
    I16 fg_close;			/* 关机 */
    I16 fg_reset;			/* 复位 */
    I16 power;				/* 功率等级 */
    I16 motor;				/* 电机型号 */
    I16 freq_sv;			/* 目标频率 */
    U16 freq_run;			/* 运行频率 */
    I16 volt_out;			/* 输出电压 */
    I16 curr_out;			/* 输出电流 */
    U16 bord_rate;			/* 波特率 */
    I16 volt_gene;			/* 母线电压 */
    I16 temp_module;		/* 模块温度 */
    I16 error;				/* 故障信息 */
    I16 temp_pfc;			/* PFC温度 */
    U16 fan_sv;				/* 目标风速 */
    U16 fan_pv1;			/* 当前风速1 */
    U16 fan_pv2;			/* 当前风速2 */
}TEST_INVT_STRU;

TEST_EXT TEST_INVT_STRU test_invt_info;

TEST_EXT  U16 test_time_current;

TEST_EXT U08 ad_adjust_cnt;

TEST_EXT  U08 test_allow;
TEST_EXT  U08 punp_fg;      //进入邦普初始化程序标志， 1:进入邦普初始化菜单                       0:没有进入邦普初始化菜单
   
                
#define	SYS_GetTick		HDW_1ms_Tick


#define MAX_4G_VER_WORD		16		// 4G模组功能码版本号信息最大字数
#define MAX_4G_SN_WORD		16		// 4G模组SN号信息最大字数

// 用于存储测试屏需要读取的与4G模组相关信息
typedef struct
{
    U16 test_sta;                   /* 4G模组测试状态 */ 
    U16 csq;                        /* 4G模组信号强度 */
    U16 total;                      /* 4G与控制板总通讯次数 */
    U16 succ;                       /* 4G与控制板通讯总成功次数 */
    U16 fail;                       /* 4G与控制板通讯总失败次数 */
	U16 ver[MAX_4G_VER_WORD];		// 4G模组功能码版本号
	U16 sn[MAX_4G_SN_WORD];			// 4G模组SN号
} TEST4G;

TEST_EXT TEST4G test_4g;


TEST_EXT void TEST_Main(void);
#endif
