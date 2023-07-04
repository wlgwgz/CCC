/******************************************************************************

          版权所有 (C), 2001-2016, 广州市邦普电脑技术开发有限公司

 ******************************************************************************
  文 件 名   : Logic.h
  版 本 号   : 初稿
  作    者   : 周志鹏
  生成日期   : 2016年11月14日星期一
  最近修改   :
  功能描述   : Logic.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2016年11月14日星期一
    作    者   : 周志鹏
    修改内容   : 创建文件

******************************************************************************/

#ifndef __LOGIC_H__
#define __LOGIC_H__


#ifdef  _LOGIC_GLOBAL_
    #define LG_EXT
#else
    #define LG_EXT extern
#endif
/*-----------------包含头文件 -----------------------------*/



/*-----------------外部变量说明----------------------------*/



/*-----------------全局变量--------------------------------*/
/* 本地输入 2个字，24路， 00~03*/
//(0~1)
//#define SysSwInLoc0			((BIT8  *)&ITbl[0])	/* 开关量输入0(本地) */
//#define sys_dinput(n)       (*(U08 *)&SysSwInLoc0[0+n].byte)        /* 系统开关量输入 */

//#define SW_SWITCH    		(*(U08 *)&SysSwInLoc0[8].byte)          /* 系统拔码 范围 0~0X0F */


#define FBUS_ADDR			(*(U16*)&ITbl[0].word)		// 级联地址


/* 本地温度输入 11个字，11路 08~18*/
//#define SysTmpIn(n)			(*(I16*)&ITbl[8+(n)].word)		/* 6路温度，0.1摄氏度 */
//
//// 本地电流输入	2个字	2路 20~21
//#define SysCurIn(n)			(*(U16*)&ITbl[20+(n)].word)		/*2路 */
//
//#define SysAIIn(n)			(*(U16*)&ITbl[30+(n)].word)		/*2路 */

typedef struct tagIO        /* 本机输入输出 */
{
//	U16  io_do[2];				/* 开关量DO */
//	U16  io_di[2];				/* 开关量DI */
//	U16  io_jmp;				/* 跳线地址 */
//	U16  io_phase;				/* 相序模块 */
	U16  io_init;				/* 初始化 */
//	U16  io_uart_cnt[9];		/* 通讯次数 */

//	I16  io_step_sv[EEV_ID_MAX];			/* 步进电机 */
//	I16  io_step_pv[EEV_ID_MAX];			/* 步进电机 */

	//I16  io_ai_cur[MAX_A];		/* 电流AI */
	//I16  io_ai_temp[MAX_T];		/* 温度AI */
	//U16  io_ad_cur[MAX_A];		/* 电流AD */
	//U16  io_ad_temp[MAX_AD][3];	/* 温度AD */

	U16   input[MAX_DI];			// 12位开关量输入强电开关量在高位
	U16   jmp[MAX_SW];				// 拨码开关
	U16   SwPlate;					// 拨盘
	U16   output[MAX_DO];			// 开关量DO
	U08     phase;						// 相序模块

//	I16     cur[MAX_CUR];				// 电流AI
	I16     temp[MAX_TEMP];				// 温度AI           0.01度
//	I16     ai[MAX_AI_NUM];             // 模拟量输入AI        // ai[0~3]对应板上J5~J8
//	I16     ai_type[MAX_AI_NUM];        // 模拟量输入AI类型
//	U16     cur_ad[MAX_CUR];			// 电流AD

//	U16     temp_ad[MAX_AD][AD_TYPE];	// 温度AD
} IO_DATA;

LG_EXT IO_DATA  gIO;
//#define		g_io		gIO

#ifdef USE_BMS_SOURCE
typedef struct{
//    I16 para[20];            /* 参数 */
    struct {
        I16 temp[MAX_TEMP_NUM];
        I16 ai[MAX_AI_NUM];
        I16 cur[MAX_CUR_NUM];
        U16 DI[MAX_DI_W];
        U16 humi[2];/* N0004 */
    }src[1];                 /* 资源,支持2个模块 */
}BMSSOURCE;
LG_EXT BMSSOURCE bms_source;
#endif

#ifdef USE_BMS_DEBUG
enum BMS_DEBUG_VAL_ID
{
    BDV_INVT_EXCEPT_INFO,   /* 变频异常信息 */
    BDV_INVT_CMD_INVT_ID_1, /* 变频控制字-1#变频器 */
    BDV_INVT_CMD_INVT_FAN1, /* 变频控制字-1#ec风机 */
    BDV_INVT_CMD_INVT_FAN2, /* 变频控制字-2#ec风机 */
    BDV_PUMPF_WARN,         /* 变频水泵警告 */
    BDV_PUMPF_ERR,          /* 变频水泵故障 */
    BDV_PUMPF_ERR_CODE,     /* 变频水泵故障信息 */
    BDV_MAX,
};
typedef struct{
    I16 debug_val[BDV_MAX];              /* 调试变量 */
}BMSDEBUG;
LG_EXT BMSDEBUG bms_debug;
#define SET_BMS_DEBUG_VAL(n, v)       (bms_debug.debug_val[(n)] = (v))
#define GET_BMS_DEBUG_VAL(n)          (bms_debug.debug_val[(n)])
#else
#define SET_BMS_DEBUG_VAL(n, v)    
#endif

/*-----------------模块级变量------------------------------*/
//LG_EXT U08  pcb_type;		/* 模块标识(0: 主板; 1: 从板; 2: 扩展板) */

//#define TYPE_MASTER		0	/* 主板 */
//#define TYPE_SLAVER		1	/* 从板 */
//LG_EXT U08 val_IBH_bit;
LG_EXT U08 IBH_cfg_type;
LG_EXT U16 val_comp_num;
LG_EXT U08 sw_mac_model;
LG_EXT U08 sw_mac_model_EXIO;
LG_EXT U08 sw_mac_model_err;        /* 机器型号超界标志位 */
#define SW_MAC_NUM 6		/* 使用6位拨码表示机型 */

typedef struct tagDIO       /* 本机输入输出 */
{
    U16  debugstart;
	I16  io_step_sv[2];				/* 步进电机 */
	I16  io_step_pv[2];				/* 步进电机 */
	/* 1+2+2=5 */

	BIT16   in_input;				// 内机开关量
	BIT16   out_input;				// 外机开关量
	BIT16   in_output[2];			// 内机输出
	BIT16   out_output;				// 外机输出
    /* 1+1+2+1=5 */

	I16     in_temp[12];		    // 内机温度AI
	I16     out_temp[10];           // 外部温度AI
	I16     press[3];       		// 模拟量输入对应压力值
	I16     ptot[3];        		// 模拟量输入压力对应温度
	I16     freq_sv;        		// 频率设定值
	I16     freq_pv;        		// 频率当前值
	I16     freq_temp;      		// 变频器当前温度
	I16     freq_volt;      		// 变频器电压
	I16     freq_cur;       		// 变频器电流
	I16     freq_max;       		// 变频器最大频率限制
	I16     freq_area;      		// 变频器调频区域
	I16     in_superheatsv; 		// 吸气过热度目标
	I16     in_superheatpv; 		// 吸气过热度当前
	I16     out_superheatsv;		// 排气过热度目标
	I16     out_superheatpv;		// 排气过热度当前
	I16     fan_speed_sv[2];        // 风机转速设定值
	I16     fan_speed_pv[2];        // 风机转速当前值
	I16     pump_speed_sv;        	// 水泵转速设定值
	I16     pump_speed_pv;        	// 水泵转速当前值
    /* 12+10+3+3+11+2+2+2=45 */

	I16     debugbuf[200];  // 模拟量输入AI类型
} IO_DEBUGDATA;

LG_EXT IO_DEBUGDATA  gdebugIO;
/*-----------------常量定义--------------------------------*/
LG_EXT U08  pcb_type;		/* 模块标识(0: 主板; 1: 从板; 2: 扩展板) */
LG_EXT U08  pcb_addr;		/* PCB板地址(ly514通讯) */

LG_EXT U16 hmi_power_time;
//LG_EXT U08 power_time;					/* 上电时间 */
#if 0
#define MAX_SYS_MS_COUNT	5
#define C_5MS		0
#define C_100MS		1
#define C_500MS		2
#define C_1S		3
#define C_1M		4

LG_EXT U16  SysMsCount[MAX_SYS_MS_COUNT];
#endif
#define	IsSecond		0		/* 1秒标志位，用完立即清零，所以每个闭合循环只能用一次 */
#define	IsMinute		1
#define Is100Ms			2
LG_EXT U08  sys_status;
#if 0
LG_EXT BOOL  F_TIME_BASIC_MIN;  		/* 一分标志 */
LG_EXT BOOL  F_TIME_BASIC_MIN_BACK;
LG_EXT BOOL  F_TIME_BASIC_SEC;  		/* 一秒标志 */
LG_EXT BOOL  F_TIME_BASIC_SEC_BACK;
LG_EXT BOOL  F_TIME_BASIC_500MS;  	/* 500毫秒标志 */
LG_EXT BOOL  F_TIME_BASIC_500MS_BACK;
LG_EXT BOOL  F_TIME_BASIC_100MS;  	/* 100毫秒标志 */
LG_EXT BOOL  F_TIME_BASIC_100MS_BACK;

LG_EXT U08 flash_500ms;	/* 500ms闪烁 */
#endif

LG_EXT U08 flash_500ms;	/* 500ms闪烁 */
LG_EXT U08 flash_1s;	    /* 1s闪烁 */

LG_EXT TIME user_time;
LG_EXT TIME system_time;

LG_EXT	U16 MinScanPeriod;
LG_EXT	U16 CurScanPeriod;
LG_EXT	U16 MaxScanPeriod;

//enum
//{
//    BAUD_INVT,
//    BAUD_INOUT,
//    BAUD_MAX,
//};
//LG_EXT U16 fg_commu_connect[_DIVUCARRY(BAUD_MAX, 16)];

//#define GET_INVT_CONNECT            (GETBIT_N(fg_commu_connect, BAUD_INVT))
//#define SET_INVT_CONNECT            (SETBIT_N(fg_commu_connect, BAUD_INVT))
//#define CLR_INVT_CONNECT            (CLRBIT_N(fg_commu_connect, BAUD_INVT))
//#define GET_INOUT_CONNECT           (GETBIT_N(fg_commu_connect, BAUD_INOUT))
//#define SET_INOUT_CONNECT           (SETBIT_N(fg_commu_connect, BAUD_INOUT))
//#define CLR_INOUT_CONNECT           (CLRBIT_N(fg_commu_connect, BAUD_INOUT))

//#define INVT_INOUT_BOTH_CONNECT     (GET_INVT_CONNECT && GET_INOUT_CONNECT)

/* 版本信息 */
//#define		val_pcb_num		2
#define MAX_PCB							8		// 最大模块数
#define MAX_COMP						2		// 单模块最大压机数，后续更改为1
#define USE_COMP						1		// 单模块使用压机数：移植彩屏时注意特殊更改
#define USE_EXIO                        1	/* 使用外机数 */
LG_EXT U08 idu_mac_ver[MAX_PCB][MAX_VER];
LG_EXT U08 odu_mac_ver[MAX_PCB][MAX_VER];

#if     MAX_COMP == 1
#define COMP_INIT(v)    _DATA_1(v)
#elif   MAX_COMP == 2
#define COMP_INIT(v)    _DATA_2(v)
#elif   MAX_COMP == 3
#define COMP_INIT(v)    _DATA_3(v)
#elif   MAX_COMP == 4
#define COMP_INIT(v)    _DATA_4(v)
#else
#error "未定义压机初始化"
#endif

LG_EXT BOOL fg_request_remote_upgrade;
LG_EXT U08 fg_accept_remote_upgrade;

/*-----------------宏定义 ---------------------------------*/
#define		wdg_clear()			/* 兼容HL017A,本芯片不需要看门狗 */


/*-----------------调试信息--------------------------------*/
#define	MAX_DEBUG		30
LG_EXT I16  debug_xx[MAX_DEBUG];


/*-----------------外部函数原型说明------------------------*/


/*-----------------内部函数原型说明------------------------*/
LG_EXT U16 USER_GetTime(U08 num);
LG_EXT void USER_time_basic_set(void);
LG_EXT void USER_time_basic_reset(void);

LG_EXT void USER_Init(void);
LG_EXT void USER_Main(void);
void USER_1ms_Deal(void);
#endif /* __LOGIC_H__ */
