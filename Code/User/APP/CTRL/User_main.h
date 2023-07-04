#ifndef MS_H
#define MS_H

#ifdef MS_GLOBAL
#define MS_EXT
#else 
#define MS_EXT extern
#endif

/**********************************************************/


/* ===============================================================
	MODBUS数据模型
	基本表格			对象类型	访问类型	对应功能码  	内容
	离散量输入(2000)	单个比特	只读		02				I/O系统提供这种类型数据
	线圈	(2000)		单个比特	读写		01 05 15		通过应用程序改变这种类型数据
	输入寄存器(500)	16-比特字	只读		04				I/O系统提供这种类型数据
	保持寄存器(500)	16-比特字	读写		03 06 16		通过应用程序改变这种类型数据
/ =============================================================== */
/***********************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
07.07.2005 RTE-MAB First Release
***********************************************************************************/


#define MODBUS_01       0x01		/* MODBUS功能码 */
#define MODBUS_02       0x02
#define MODBUS_03       0x03
#define MODBUS_04       0x04
#define MODBUS_05       0x05
#define MODBUS_06       0x06
#define MODBUS_08       0x08
#define MODBUS_0F       0x0f
#define MODBUS_10       0x10
#define MODBUS_17       0x17

#define MODBUS_NO_ERR       0    /* 无异常 */
#define MODBUS_ERR_01       1    /* 非法功能 */
#define MODBUS_ERR_02       2    /* 非法数据地址 */
#define MODBUS_ERR_03       3    /* 非法数据值 */
#define MODBUS_ERR_04       4    /* 从设备故障 */
#define MODBUS_ERR_05       5    /* 确认  客户机(或主站)可以继续发送轮询程序完成报文来确定是否完成处理 */
#define MODBUS_ERR_06       6    /* 从设备忙 发送轮询程序完成报文来确定是否完成处理，空闲后重发数据 */
#define MODBUS_ERR_99		99

/* 诊断子功能码  MODBUS_08 */
#define MODBUS_DIAG_00       0x00	// Return Query Data
#define MODBUS_DIAG_01       0x01	// Restart Communications Option
#define MODBUS_DIAG_02       0x02	// Return Diagnostic Register
#define MODBUS_DIAG_03       0x03	// Change ASCII Input Delimiter
#define MODBUS_DIAG_04       0x04	// Force Listen Only Mode
#define MODBUS_DIAG_0A       0x0A	// Clear Counters and Diagnostic Register
#define MODBUS_DIAG_0B       0x0B	// Return Bus Message Count
#define MODBUS_DIAG_0C       0x0C	// Return Bus Communication Error Count
#define MODBUS_DIAG_0D       0x0D	// Return Bus Exception Error Count
#define MODBUS_DIAG_0E       0x0E	// Return Slave Message Count
#define MODBUS_DIAG_0F       0x0F	// Return Slave No Response Count
#define MODBUS_DIAG_10       0x10	// Return Slave NAK Count
#define MODBUS_DIAG_11       0x11	// Return Slave Busy Count
#define MODBUS_DIAG_12       0x12	// Return Bus Character Overrun Count

/* =============================================================== */
/*typedef struct tagModBus
{	
	U08 opt;
	U08 (*cb)(U08 *buf, U16 addr, U16 num);
} MODBUS; */

//#define MAX_PCB							2		//最大模块数   



// 模块数据结构说明--------------------------------------------------------------------------------------------
// cell[模块号 ].cell_cmd[i]:		i=0：压机1命令；			i=1：压机2命令；	i=MAX_COMP：模块命令；
// cell[MAX_PCB].cell_cmd[i]:		i=MAX_COMP：系统命令；
// cell[模块号 ].cell_relay[i]:		i=0：压机1输出；			i=1：压机2输出；	i=MAX_COMP：模块单元输出；
// cell[MAX_PCB].cell_relay[i]:		i=0：系统输出；	
// cell[模块号 ].cell_input[i]:		i=0：压机1输入；			i=1：压机2输入；	i=MAX_COMP：模块单元输入；
// cell[MAX_PCB].cell_input[i]:		i=0：系统输入；
// cell[模块号 ].cell_status[i]:	i=0：压机1状态；			i=1：压机2状态；	i=MAX_COMP：模块单元状态；
// cell[MAX_PCB].cell_status[i]:	i=0：系统状态；
// cell[模块号 ].cell_runtime[i]:	    i=0：压机1运行时间；		i=1：压机2运行时间；
// cell[MAX_PCB].cell_runtime[i]:	i=0：机组累计运行时间（保存）；
// cell[模块号 ].cell_error[i]:		模块_压机和单元故障
// cell[MAX_PCB].cell_error[i]:		系统相关故障
// cell[模块号 ].cell_pv[i]:		模块_压机和单元模拟量；
// cell[MAX_PCB].cell_pv[i]:		系统相关模拟量；
// -------------------------------------------------------------------------------------------------------------

#define RLY_BIT_NUM			16		/* 输出变量的位数(区分字节变量和字变量) */
#define IN_BIT_NUM			16		/* 输入变量的位数(区分字节变量和字变量) */
#define ERR_BIT_NUM			16		/* 故障变量的位数(区分字节变量和字变量) */

#define MAX_CMD                  (MAX_COMP+1)
//#define MAX_CMD                  (2)
#define MAX_RELAY                (MAX_COMP+1)
#define MAX_INPUT                (MAX_COMP+1)
#define MAX_STATUS               (MAX_COMP+1)
//#define MAX_RUNTIME               MAX_COMP	    // （预留：2个时间计数）	//NOTE-CXW 待确认 
#define MAX_RUNTIME               2	    // （预留：2个时间计数）	//NOTE-CXW 待确认 
#define MAX_ERR                  ((USE_COMP*2+1)*2)	// 
#define MAX_PV                   60	// 需同步更改：联网监控说明书、上位机监控软件

#define MAX_PV_WORD MAX_PV
#define MAX_ERR_WORD MAX_ERR

#define MAX_FREQINFO             16
typedef struct tagINVT
{
	I16 info[MAX_FREQINFO];
    U08 error_now;		        // 当前变频故障号
    U08 bak;				    /* 2023-1-9：增加bak目的是凑齐16位对齐，更改此结构体时注意调整 */
} INVT_DATA;

//#define MAX_CMD                  3
//#define MAX_RELAY                3
//#define MAX_INPUT                3
//#define MAX_STATUS               3
//#define MAX_RUNTIME              3		// 此处数值如有变化，要注意MODBUS.c中函数sys_information()是否需修改
//#define MAX_ERR                  4
//#define MAX_PV                   10		// 此处数值如有变化，要注意MODBUS.c中函数sys_information()是否需修改
typedef struct tagCELL		/* 谨记: 该数组元素的大小不能随意修改! */	// 此处结构如有变化，要注意MODBUS.c中函数sys_information()是否需修改
{
	U16 cell_cmd[MAX_CMD];			/* 命令 */
	U16 cell_relay[MAX_RELAY];		/* 开关量输出 */
	U16 cell_input[MAX_INPUT];		/* 开关量输入 */
	U16 cell_status[MAX_STATUS];	/* 状态 */
	U16 cell_runtime[MAX_RUNTIME];	/* 累计运行时间 */
	U16 cell_error[MAX_ERR];		/* 故障 */
	I16 cell_pv[MAX_PV];			/* 模拟量 */
	INVT_DATA cell_invt[INVT_ID_MAX];	// 变频器
} CELL_DATA;
MS_EXT CELL_DATA  cell[MAX_PCB + 1];	/* 最后一个为系统的 */
MS_EXT CELL_DATA  *pCell;
MS_EXT CELL_DATA  *pmac;

#define MAX_CELL_BYTE	(sizeof(CELL_DATA)/sizeof(U08))
#define CELL_SIZE		(sizeof(CELL_DATA) / sizeof(U16))		 
#define CELL_BITS   (((sizeof(CELL_DATA)/sizeof(U16))-MAX_PV-MAX_CMD-(sizeof(INVT_DATA)/sizeof(U16)*INVT_ID_MAX))*16)

MS_EXT U16 bak_err_out[MAX_PCB+1][MAX_ERR];	/* 历史故障判断备份 */
MS_EXT BOOL	Link_remeber_on;	// 记忆机组在二通阀断开前的状态是待机（FALSE）还是运行（TRUE）
/* 外部变量声明 */
MS_EXT U08  fg_error;	/* 故障总的标识 */

#define CELL_INVT_ERR(pcb, id)      (cell[pcb].cell_invt[id].error_now)
#define CELL_INVT_INFO(pcb, id, x)  (cell[pcb].cell_invt[id].info[x])

#define MAX_SYS_MS_COUNT	5
#define C_5MS		0
#define C_100MS		1
#define C_500MS		2
#define C_1S		3
#define C_1M		4


/************************************************************************************/
//#define NULL     		0
#define PNULL    		((PVOID)0)

#undef TRUE
#define TRUE    		1
#undef FALSE
#define FALSE   		0

#define ON				1
#define OFF				0

#define BIT_HIGH		1
#define BIT_LOW			0
#define BYTE_HIGH		0xFF
#define BYTE_LOW		0x00

#define BIT_0 			0x1
#define BIT_1 			0x2
#define BIT_2 			0x4
#define BIT_3 			0x8
#define BIT_4 			0x10
#define BIT_5 			0x20
#define BIT_6 			0x40
#define BIT_7 			0x80
              			
#define BIT_8 			0x100
#define BIT_9 			0x200
#define BIT_10			0x400
#define BIT_11			0x800
#define BIT_12			0x1000
#define BIT_13			0x2000
#define BIT_14			0x4000
#define BIT_15			0x8000
              			
#define BIT_16			0x10000L
#define BIT_17			0x20000L
#define BIT_18			0x40000L
#define BIT_19			0x80000L
#define BIT_20			0x100000L
#define BIT_21			0x200000L
#define BIT_22			0x400000L
#define BIT_23			0x800000L
              			
#define BIT_24			0x1000000L
#define BIT_25			0x2000000L
#define BIT_26			0x4000000L
#define BIT_27			0x8000000L
#define BIT_28			0x10000000L
#define BIT_29			0x20000000L
#define BIT_30			0x40000000L
#define BIT_31			0x80000000L

#define LED_ON      	1
#define LED_OFF     	0

//#define LED_RED				DIN_LED1
//#define LED_GREEN				DIN_LED2
#define LED_SET(led, state) 	led  = state
#define LED_TOGGLE(led)			led ^= LED_ON

/************************************************************************************/


//#define SWAPWORD(x)         (U16)((((x) & 0xFF) << 8) | (((x) >> 8) & 0xFF))

#define LOW4_BIT(x)			((U08)(x) & 0x0f)
#define HIGH4_BIT(x)			((U08)(x) >> 4)

//#define LOW_BYTE(x)           (U08)(x)
//#define HIGH_BYTE(x)           (U08)((U16)(x) >> 8)

//#define MAKEWORD(b, a)      ((U16) (((U08)(a)) | ((U16)((U08)(b))) << 8)) // Types.h有
#define MAKELONGWORD(b, a)      ((U32) (((U16)(a)) | ((U32)((U16)(b))) << 16))

//#define ASM_NOP				_asm("nop")

//#define	ENABLE_INTERRUPT   _asm("rim")	  //开启全局中断
//#define	DISABLE_INTERRUPT  _asm("sim")	  //禁止中断

MS_EXT U16 self_comp_num;					// 模块压机数 
MS_EXT U08 use_phase_lack;					// 缺相使用否

MS_EXT U16  SysMsCount[MAX_SYS_MS_COUNT];

MS_EXT BOOL  F_TIME_BASIC_MIN;  		/* 一分标志 */
MS_EXT BOOL  F_TIME_BASIC_MIN_BACK;
MS_EXT BOOL  F_TIME_BASIC_SEC;  		/* 一秒标志 */
MS_EXT BOOL  F_TIME_BASIC_SEC_BACK;
MS_EXT BOOL  F_TIME_BASIC_500MS;  		/* 500毫秒标志 */
MS_EXT BOOL  F_TIME_BASIC_500MS_BACK;
MS_EXT BOOL  F_TIME_BASIC_100MS;  		/* 100毫秒标志 */
MS_EXT BOOL  F_TIME_BASIC_100MS_BACK;

MS_EXT BOOL  F_TIME_SWITCH_1S;			// 1秒切换标志（中断中处理）	 


//MS_EXT BOOL	remeber_on;

MS_EXT U16 power_time;					/* 上电时间 */
MS_EXT U08 g_screen_clc_time;   		//清屏时间单位秒


MS_EXT U08 fg_seep_crc_err;				/* 功能参数读取出错 */
MS_EXT U16 fg_seep_crc_iderr;		    /* 功能参数读取出错参数组 */

MS_EXT U08 fg_seep_overflow;			/* 功能参数超界 */
MS_EXT U16 fg_seep_overflow_iderr;		/* 功能参数超界参数组 */
MS_EXT U16 fg_seep_overflow_item;		/* 功能参数超界参数项 */

MS_EXT U08 fg_limit_crc_err;			/* 系统维护参数读取出错 */
MS_EXT U08 fg_limit_overflow;			/* 系统维护参数超界 */

//MS_EXT U08 debug_buf[50];			/* 功能参数超界 */

MS_EXT U16	cmd_modify;			/* 命令修改标识 */
MS_EXT BOOL	cmd_broadcast;		/* 命令广播 */
MS_EXT U16  cmd_normal[8];		/* 用户命令 */
MS_EXT U16  cmd_system[8];		/* 系统命令 */
MS_EXT U16 bro_cell_cmd[MAX_CMD];	/* 按新通讯协议的广播命令 */

MS_EXT U16 commu_m_password_broadcast_interval;
MS_EXT BOL fg_trans_control_sta[MAX_PCB];
MS_EXT U16 nor_cell_cmd[MAX_PCB][MAX_CMD];	/* 按新通讯协议的广播命令 */

//MS_EXT U08	version_info[MAX_PCB][32];	/* 版本信息 */

#define CTRL_INFO_NUM 80
MS_EXT U16	ctrl_info[CTRL_INFO_NUM];	/* 机组信息 */	 
MS_EXT U16	assist_info[10];	/* 辅助属性信息 */
MS_EXT U16	pcb_comp_num[MAX_PCB];	/* 压机个数 */

/* BMSHMI */
typedef struct
{
    /* 位 */
    U16	info_bit[6];
    
#if 0
    /* 字 */
	I16 info_word[50];
#endif
}STR_BMSHMI;
MS_EXT STR_BMSHMI bmsHMI;
#define SET_BMSHMI_N(num, info)   do{                                       \
                                      if (info)                             \
                                      {                                     \
                                          SETBIT_N(bmsHMI.info_bit, num);   \
                                      }                                     \
                                  }while(0)

#define INV_ERR_PARA_INFO_NUM (FP_MAX+1)    //输入电压电流共用一个变频参数地址，拆分后个数需加一
MS_EXT I16  inv_err_para_info[INV_ERR_PARA_INFO_NUM];  //变频故障参数信息
//MS_EXT U16	history_error[50];	/* 历史故障信息 */

MS_EXT I16	m_motor_sv_sheat;	/*主路设置过热度 */	 
MS_EXT I16	m_motor_pv_sheat;	/*主路当前过热度 */	 
MS_EXT I16	s_motor_sv_sheat;	/*辅路设置过热度 */	 
MS_EXT I16	s_motor_pv_sheat;	/*辅路当前过热度 */	 
MS_EXT I16	m_motor_judge_sheat;	/*判断主路过热度 1:常温制热过热度 2:极冷制热过热度*/	 
MS_EXT I16	s_motor_pvnow_sheat;	/*排气当前过热度 */	 


MS_EXT BOOL fg_sys_limit;		/* 使用期限到标志 */
MS_EXT U16 pre_limit_time;	/* 提醒时间 */

#define PASS_SUPER		0		/* 动态密码 */
#define PASS_FAC		1		/* 厂家密码 */
#define PASS_SER		2		/* 维修者密码 */
#define PASS_PRO		3		/* 工程密码 */
#define PASS_USER		4		/* 用户密码 */
#define PASS_LIMIT		5		/* 使用期限密码 */
#define PASS_POP        6	    /* 开机密码 */
#define MAX_PASS_GROUP	12 	    /* 最大密码组数 */

#define MAX_PASS_WORD		8 	/* 每组密码字节数 */
MS_EXT U08	password[MAX_PASS_GROUP][MAX_PASS_WORD];	/* 密码 */
//MS_EXT U08	superpassword[MAX_PASS_WORD];	/* 超级密码 */

MS_EXT U16	passwd_modify;		/* 密码修改标识 */
MS_EXT U16	passwd_broadcast;	/* 密码广播 */

// cell[MAX_PCB].cell_runtime[n]:   n的定义
#define SYS_RUNTIME					0		//系统累计运行计时
//#define XX_RUNTIME				1		//备用

#define _GET_	0
#define _SET_	1
#define _mGET_	2
#define _mSET_	3
#define _mCMD_	4


/* =============================================================== */
MS_EXT U16 Init_cmd;
#define CMD


/* =============================================================== */
//MS_EXT U16 CRC16(U08 *data_in, U08 length);
//MS_EXT U08 crc_check(U08 *data_in, U08 length);
MS_EXT U08  password_broadcast(U08 *buf);
//MS_EXT void password_read_from_eeprom(void);
//MS_EXT void password_write_to_eeprom(void);
//MS_EXT void password_initial(void);
MS_EXT void password_reset(U08 x);

MS_EXT void sys_task(void);
MS_EXT void mod_task(void);

//MS_EXT void wdg_clear(void);

MS_EXT void version_initial(void);
MS_EXT U08 version_information(U08 *buf, U16 addr, U16 num, U08 opt);

MS_EXT void Read_IO_Info(void);
//MS_EXT void User_main(void);
MS_EXT void User_main_init(void);
MS_EXT void User_main_task(void);
MS_EXT U16 read_address(void);
//MS_EXT U16 read_comp_num(void);
MS_EXT U16 read_IBH_config(void);
MS_EXT U16 read_mac_model(void);

MS_EXT void punp_init(void);


#endif
