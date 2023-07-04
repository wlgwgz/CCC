/**********************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司
 All rights reserved. 

 文件名称 : dm603.h
 文件描述 : 头文件定义
 
 创建人   : 李相宏
 创建日期 : 2012.11.20

 ============================ 修改记录 =============================== 
 修改人  : --
 程序版本: --
 完成日期: --
 修改内容: --
  
**********************************************************************/

#ifndef	_DM603_H_
#define	_DM603_H_

#ifdef	DM603_GLOBAL
#define DM603_EXT
#else
#define DM603_EXT extern
#endif

//#define	KEY_RET_ENT_CHG		/* 确定和返回键对换 宏定义 */
//#define	TEST_CHG_UART		/* 级联口更改为集控口用于测试 */
//#define	ERROR_IS_RIGHT		/* 故障靠右显示 */

/* 变量定义 */
#define KEY_NULL    	0x00
#define DM603_KEY_NULL	KEY_NULL	/* 空键 */

#define KEY_STOP    	0x10
#define KEY_DOWN    	0x08
#define KEY_UP    		0x04
#define KEY_SET   		0x02
#define KEY_RUN    		0x01

#define DM603_KEY_ONOFF		KEY_RUN
#define DM603_KEY_OK		KEY_STOP
#define DM603_KEY_UP		KEY_UP
#define DM603_KEY_DOWN		KEY_DOWN
#define DM603_KEY_FUNC		KEY_SET

#define	MAX_TIME					6				/* 定时组数 */
#define	MENU_TIME_SELECE_MIN		0
#define	MENU_TIME_SELECE_MAX		(MAX_TIME+2)
//#define DM603_KEY_FUNC	DM603_KEY_ONOFF	/* 开关=功能 键 */
DM603_EXT U08 dm603_cnt_key_updwn[MAX_HMI];		/* 上下键按键间隔(100ms) */
DM603_EXT U08 dm603_cnt_key_space[MAX_HMI];		/* 解锁时按键间隔(100ms) */
//DM603_EXT U16 dm603_cnt_key_null[MAX_HMI];		/* 无按键时间间隔(1秒) */
//DM603_EXT U08 dm603_fg_lock_clue[MAX_HMI];		/* 锁定提示标志 */

DM603_EXT U16 dm603_pv_test;		/* 无按键时间间隔(1秒) */

DM603_EXT U16 key_continuous_time[MAX_HMI];		/* 连续按键计时 */

DM603_EXT U32 dm603_alarm_sound_cnt[MAX_HMI];	/* 消音计时 */

#define ALARM_SOUND_SET_OFF				(0)	/* 不响 */
#define ALARM_SOUND_SET_ON				(1)	/* 长响 */
#define ALARM_SOUND_SET_10S_ONCE		(2)	/* 只响十秒 */
#define ALARM_SOUND_SET_10S_CYCLE		(3)	/* 周期十秒 */
#define ALARM_SOUND_SET_MAX				(4)	// 报警音设置上限，在此之前追加

#define ALARM_SOUND_CYCLE (QK_60*30)		/* 周期十秒的周期 */


DM603_EXT U08 key_delay_time_modbus[MAX_HMI];	// 进入BMS设置界面按键延时(100ms)（防止进入时响应到上一界面的按键）	 
DM603_EXT U08 key_delay_time_pass[MAX_HMI];		// 进入   密码界面按键延时(100ms)（防止进入时响应到上一界面的按键）

// 界面按键延时
// 使用注意:只对按键操作有效，if(key_delay_time[hmi_t_num]>=5){范围只能包括按键操作}，
//          非按键的判断不应放在此判断内，以免引起不必要的延时。
DM603_EXT U08 key_delay_time[MAX_HMI];			// 进入界面按键延时(100ms)（防止进入时响应到上一界面的按键）
#define KEY_DELAY QK_5

DM603_EXT U08 dm603_sound[MAX_HMI];			/* 消音 */
/* 组合键定义 */
/* 需要强制16进制转换，否则与Main_Key比较会出错，请注意 */
#define	DM603_KEY_OK_FUNC		((U16)(DM603_KEY_OK | DM603_KEY_FUNC))		/* 确定+功能 键 */
#define	DM603_KEY_UP_DOWN		((U16)(DM603_KEY_UP | DM603_KEY_DOWN))		/* 向上+向下 键 */
#define	DM603_KEY_ONOFF_FUNC	((U16)(DM603_KEY_ONOFF | DM603_KEY_FUNC))	/* 开关+功能 键 */

/* 测试程序组合键 */
#define	DM603_KEY_ONOFF_UP		((U16)(DM603_KEY_ONOFF | DM603_KEY_UP))		/* 开关+向上 键 */
#define	DM603_KEY_FUNC_UP		((U16)(DM603_KEY_FUNC | DM603_KEY_UP))		/* 功能+向上 键雪天模式组合功能按键 */

/* 3秒键定义 */
#define	DM603_KEY_ONOFF_3S			(0x00f1)	/* 开关键3秒 */
#define	DM603_KEY_OK_3S		    	(0x00f2)	/* 确定键3秒 */	 
#define	DM603_KEY_FUNC_3S			(0x00f3)	/* 功能键3秒 */
/* 用户命令-->常用命令 */
#define  KEY_CMD_RUN             	0x0001	/* 开机命令 */
#define  KEY_CMD_STOP            	0x0002	/* 关机命令 */
#define  KEY_CMD_RESET           	0x0100	/* 故障复位命令 */
#define  KEY_CMD_SOUND_CANCEL     	0x0200	/* 消音命令(此命令为上位机发送) */
#define  KEY_CMD_WARM_CANCEL     	0x0400	/* 取消预热 */
//#define  KEY_CMD_ENTER_FLOOR_DRYUP 	0x1000	/* 进入地暖烘干功能 */
//#define  KEY_CMD_FORCE_IBH			0x2000	/* 强制开启内置电加热 */
//#define  KEY_CMD_FORCE_TBH 			0x4000	/* 强制开启水箱电加热 */
//#define  KEY_CMD_FAST_HOTW	        0x8000	/* 快速制热水 */

/* 厂家命令-->常用命令 */
#define  KEY_CMD_USER_PARA_INIT		0x0001   /* 用户参数初始化命令 */
#define  KEY_CMD_FAC_PARA_INIT		0x0002   /* 厂家参数初始化命令 */
#define  KEY_CMD_FAC_INVT_INIT		0x0004   /* 变频器初始化命令 */
#define  KEY_CMD_LIMIT_INIT		    0x0008   /* 系统维护初始化命令 */

//#define  KEY_CMD_FAC_ABILITY_TEST	0x0010   /* 能力测试 */


#define  KEY_CMD_RUNTIME_INIT		0x0100   /* 累计运行时间初始化命令 */
#define  KEY_CMD_SYSTIME_INIT		0x0200   /* 系统运行时间初始化命令 */

#define  KEY_CMD_PASSWORD_INIT		0x4000   // 密码初始化
#define  KEY_CMD_ALLDATA_INIT		0x8000   // 全部数据初始化

/* 厂家命令-->特殊命令 */
//#define  KEY_CMD_SYS_LIMIT			0x0001	/* 使用期限到命令 */

DM603_EXT BOOL cmd_run;
DM603_EXT BOOL cmd_stop;

DM603_EXT BOOL key_run;
DM603_EXT BOOL key_stop;

/* 行定义 */
#define	ROW1		0
#define	ROW2		16
#define	ROW3		32
#define	ROW4		48

#define	RNUL		0xff

/* 列定义 */
#define	COL_END		128
#define	COL_BGN		0
#define	COL_ASC		8
#define	COL_CHR		16


/* 行定义 */
#define LINE1           0 
#define LINE2           1 
#define LINE3           2 
#define LINE4           3 
#define	LNULL		    0xff

/* 列定义(一个ASCII字符) */
#define POS1            0 
#define POS2            1 
#define POS3            2 
#define POS4            3 
#define POS5            4 
#define POS6            5 
#define POS7            6 
#define POS8            7 
#define POS9            8 
#define POS10           9 
#define POS11           10 
#define POS12           11 
#define POS13           12 
#define POS14           13 
#define POS15           14 
#define POS16           15 
#define POSEND          16 


/* 界面定义 */
enum DM603_FACE_ID
{
	FACE_OPEN	,				/* 开机界面 */
	FACE_MAIN	,				/* 主界面   */
	FACE_MENU	,				/* 状态查询 */
	FACE_MENU_TIME   ,			/* 定时界面 */
	FACE_MENU_TIME_SET,			/* 定时时间设置 */
	FACE_MENU_STATUS ,			/* 状态界面 */
	FACE_MENU_CLOCK  ,			/* 时钟界面 */
	FACE_MENU_VERSION,			/* 版本界面 */
	FACE_MENU_FUNC   ,			/* 高级功能 */
	FACE_MENU_FUNC_USER, 		/* 用户设置 */
	FACE_MENU_FUNC_DEFROST, 	/* 手动除霜 */
	FACE_MENU_CLOUD  ,			/* 无线功能界面 */
	FACE_MENU_CLOUD_CMD  ,		/* 无线命令界面 */
	FACE_MENU_FCU  ,			/* 风盘信息界面 */
	FACE_CLOUD_CMD_RESET,		/* 无线复位*/
	FACE_CLOUD_CMD_CLR,			/* 清空无线数据*/
	FACE_CLOUD_CMD_TEST,		/* 无线厂测 */
	FACE_CLOUD_CMD_RELEASE,		/* 无线解除绑定*/
	FACE_MENU_CLOUD_STATUS,		/* 无线状态界面 */
	FACE_MENU_CLOUD_INFO  ,		/* 无线状态界面 */
	FACE_MENU_FUNC_UINT, 		/* 模块使用设置 */
	FACE_MENU_FUNC_RUNTIME, 	/* 运行时间查询 */
	FACE_MENU_FUNC_HIS_ERR,		/* 历史故障查询 */
    FACE_MENU_INVT_PARA  ,	    /* 变频参数界面 */
	FACE_MENU_FUNC_SELECT, 		/* 高级功能选择 */
	FACE_PASS_M_FAC, 			/* 修改厂家密码 */ 
	FACE_PASS_M_USER, 			/* 修改用户密码 */ 
	FACE_PASS_M_LMT, 			/* 修改系统维护密码 */ 
	FACE_SET	,				/* 常规设置 */
	FACE_PARA	,				/* 厂家设置 */
	FACE_PARA_SET	,			/* 参数设置 */
	FACE_PARA_SET_VALUE	,		/* 参数项设置 */
	FACE_PARA_PASS_M,  			/* 密码修改 */
	FACE_PARA_INIT_P,			/* 初始化参数 */
	FACE_PARA_INIT_C,   		/* 初始化压机 */
	FACE_INVT_INIT,             /* 初始化变频器 */
	FACE_SUB_MENU_SELECT, 		/* 选择界面 */
	FECE_SUB_ACT,				/* 子节面具体处理 */
	FACE_MACHINE_CODE	,		/* 机组编码 */
	FACE_ERROR,					/* 故障查询 */
	FACE_ALARM,					/* 报警查询 */
	FACE_PASS	,				/* 密码界面 */
	FACE_LIMIT,					/* 使用期限 */
	FACE_LIMIT_SET,				/* 系统维护设置 */
	FACE_LIMIT_RUNTIME,			/* 运行时间查询 */
	FACE_LIMIT_RUNTIME_INIT,    /* 累计运行时间初始化 */   
	FACE_LIMIT_INIT,            // 初始化系统维护
	FACE_OFF	,				/* 期限到期 */
	FACE_PRE	,				/* 期限提醒 */
	FACE_DEBUG	,				/* 调试界面 */
	FACE_MOD	,				/* 通讯设置 */
	FACE_LOCK ,					/* 锁键界面 */
	FACE_LOCK1,					/* 锁键界面 */
	FACE_MENU_MODE,				/* 模式设置 */
	FACE_MENU_USER_SHORTCUT,	/* 快捷设置界面 */
	DM603_FACE_FORMT,			/* 联网通讯设置 */
    FACE_STATUS_CHECK,          /*  */
	FACE_ONOFF	,				/* 开关机确认界面 */
	FACE_INVT_EXCEPT_INFO,		/* 变频异常信息查询 */
	FACE_ENTER_FLOOR_DRYUP,		/* 地暖烘干功能 */
	FACE_FORCE_IBH,				/* 强制开启内置电加热 */
	FACE_FORCE_TBH,				/* 强制开启水箱电加热 */
	FACE_FORCE_HOT_WATER,		/* 强制制热水(快速制热水) */
	FACE_HOLIDAY_MODE_SET,		/* 休假模式设置 */
	FACE_ABILITY_TEST,			/* 能力测试 */

    FACE_PUNP_INIT,             // 邦普初始化
};

/* 界面操作定义 */
enum DM603_ACTION_ID
{
	SET_SV_ECONOMY_MODE,	// 设置节能模式
	PUNP_INIT_ALL,          // 邦普初始化-全部
	PUNP_INIT_PARA,         // 邦普初始化-参数
	PUNP_INIT_PASSWD,       // 邦普初始化-密码
    

};





enum DM603_MENU_ID {  DM603_MENU_CREATE, DM603_MENU_PROC, DM603_MENU_PAINT, DM603_MENU_OTHER, DM603_MENU_EXIT	  };

DM603_EXT U08 dm603_face[MAX_HMI];	/* 界面ID */

/* 菜单界面定义 */
#define		MENU_UP			0
#define		MENU_DOWN		1

//#define		MENU_SELECE_MIN		0
//#define		MENU_SELECE_MAX		(hmi_board_type[hmi_t_num]?5:6)




/* 定时操作 */
//#define		MENU_WR		0
//#define		MENU_RD		1
//#define		MENU_IN		2



#define		MENU_FUNC_SELECE_MIN		0
#define		MENU_FUNC_SELECE_MAX		4

/* 模块板最大个数,用于查看故障板 */
#define	MAX_PCB_ALL		(MAX_PCB+1)//

/* 密码输入子界面 */
#define		MIN_PASS_BIT		0
#define		MAX_PASS_BIT		8

#define		PASS_CHG_UP			0
#define		PASS_CHG_DOWN		1



//#define		PARA_SELECE_MIN		0
//#define		PARA_SELECE_MAX		(hmi_board_type[hmi_t_num]?3:4)	 

/* 参数设置子界面 */
#define		PARA_SELECE_LARGE	0	/* 参数组 */
#define		PARA_SELECE_SMALL	1	/* 参数项 */
#define		PARA_SELECE_VALUE	2	/* 参数值 */

/* 参数显示属性 */
#define		PARA_OPT_M		0	/* 项 */
#define		PARA_OPT_V		1	/* 值 */



/* 历史故障 */
#define		MAX_HIS_ERROR	10

/* 使用期限设置界面 */
//#define	FACE_LIMIT_SET 		0	/* 期限查询 */
//#define	FACE_LIMIT_PASS   	1	/* 期限改密 */
//#define	FACE_LIMIT_RUN_SET		2	/* 期限初始化 */
//#define	FACE_LIMIT_SELECT 	4	/* 选择界面 */

#define		LIMIT_SELECE_MIN		0
#define		LIMIT_SELECE_MAX		3

#define MAX_VER_LINE        15  /* 一行最多字符数 */
#define MAX_VER_LINE1       14  /* 第一行字符数 */
#define MAX_VER_LINE2(p)    MIN(STRLEN(p)-MAX_VER_LINE1, MAX_VER_LINE)

/* 操作命令 */
#define DM603_CMD_RUN				(key_run = TRUE)						    /* 开机 */
#define DM603_CMD_STOP				(key_stop = TRUE)						    /* 关机 */
#define DM603_CMD_RESET				cmd_normal[0] |= KEY_CMD_RESET			    /* 复位 */
#define DM603_CMD_WARM_CANCEL		cmd_normal[0] |= KEY_CMD_WARM_CANCEL	    /* 取消预热 */
//#define DM603_CMD_RNTER_FLOOR_DRYUP	cmd_normal[0] |= KEY_CMD_ENTER_FLOOR_DRYUP  /* 地暖烘干功能 */
//#define DM603_CMD_FORCE_IBH			cmd_normal[0] |= KEY_CMD_FORCE_IBH		    /* 强制开启内置电加热 */
//#define DM603_CMD_FORCE_TBH			cmd_normal[0] |= KEY_CMD_FORCE_TBH  		/* 强制开启水箱电加热 */
//#define DM603_CMD=_FAST_HOTW	        cmd_normal[0] |= KEY_CMD_FAST_HOTW	        /* 快速制热水 */

#define DM603_CMD_DEFROST(x)		SET_BIT(cmd_normal[1], x)				    /* 强制除霜 */
#define DM603_CMD_PARA_INIT			cmd_system[0] |= KEY_CMD_FAC_PARA_INIT	    /* 厂家参数初始化 */
#define DM603_CMD_INVT_INIT			cmd_system[0] |= KEY_CMD_FAC_INVT_INIT	    /* 变频器初始化 */
#define DM603_CMD_LIMIT_INIT		cmd_system[0] |= KEY_CMD_LIMIT_INIT	        /* 系统维护初始化 */

//#define DM603_CMD_ABILITY_TEST		cmd_system[0] |= KEY_CMD_FAC_ABILITY_TEST   /* 能力测试 */

#define DM603_CMD_RUNTIME_INIT		cmd_system[0] |= KEY_CMD_RUNTIME_INIT	    /* 压机累计运行时间初始化 */
#define DM603_CMD_SYSTIME_INIT		cmd_system[0] |= KEY_CMD_SYSTIME_INIT	    /* 系统累计运行时间初始化 */

#define DM603_CMD_PASSWORD_INIT		cmd_system[0] |= KEY_CMD_PASSWORD_INIT	    // 密码初始化
#define DM603_CMD_ALLDATA_INIT		cmd_system[0] |= KEY_CMD_ALLDATA_INIT	    // 所有数据初始化
#define DM603_CMD_PASS_INIT(x)		password_reset(x)						    /* 密码初始化 */


/* 参数信息结构体 */
typedef struct tagSMALL {
	const U16 msg;			/* 小项名 */
	const U16 * const pmsg;	/* 字符串 */
} SMALL;

/* 字典信息 */
typedef struct tagPARADICT {
    const U16 *pmsg;    /* 字典指针 */
    U08 items;          /* 字典项个数 */
}PARADICT;

/* 外部函数声明 */
DM603_EXT void dm603_tasking(U08 *disp_buf);
DM603_EXT void dm603_init(void);
DM603_EXT void dm603_online_routine(U08 *disp_buf);
DM603_EXT U08 menu_error_find_next(U08 opt, U08 x, U08 err_id);
//DM603_EXT void dm603_face_menu_eeprom(U08 opt, U08 *tbl);
DM603_EXT void dm603_routine(U08 *disp_buf);

DM603_EXT U08 dm603_language;	    // DM603A语言选项  
DM603_EXT U16 ValidCode;            // 验证码，计算动态密码使用

//DM603_EXT PARAMITEM  ParamPri[15];


//#define val_pcb_num				1
#define m_val_comp_num(pcb_num)		((M_CM_GetStatus((pcb_num), MAX_COMP, CELL_COMP))>>4)	//获得单元压机数
//#define MAX_COMP				2
//#define val_comp_num			2
///* 参数属性的16位含义：*/
///* 第0~1位: 小数点 */
//#define D_DOT1	 	0x0001	/* 1位小数点 */
//#define D_DOT2	 	0x0002	/* 2位小数点 */
//#define D_DOT3	 	0x0003	/* 3位小数点 */
//#define D_DOTMASK	0x0003	/* 屏蔽字 */
///* 第2~3位: */
///* 第4~7位: 单位 */
//#define UINT_0		(0<<4)		/* 无 */
//#define UINT_C		(1<<4)		/* 温度() */
//#define UINT_A		(2<<4)		/* 电流(A) */
//#define UINT_PR		(3<<4)		/* 压力(bar) */
//#define UINT_PE		(4<<4)		/* 百分比(%) */
//#define UINT_F		(5<<4)		/* 频率(HZ) */
//#define UINT_RS		(6<<4)		/* 转速(rps) */
//#define UINT_RM		(7<<4)		/* 转速(rpm) */
//#define UINT_S		(8<<4)		/* 秒 */
//#define UINT_M		(9<<4)		/* 分 */
//#define UINT_H		(10<<4)		/* 时 */
//#define UINT_D		(11<<4)		/* 天 */
//#define UINT_ST		(12<<4)		/* 步 */
//#define UINT_MASK	(0x000f<<4)	/* 屏蔽字 */
///* 第8位: */
//#define D_BIT		0x0100		/* 位操作 */
///* 第9位: */
//#define D_DE		0x0200		/* 温差 */
///* 第10位: */
//#define D_HIDE	 	0x0400		/* 隐藏属性 */
///* 第11位: */
//#define D_NEXCH	 	0x0800		/* 不能修改该项 */
///* 第12~14位: 权限级别(由低到高) */
//#define D_USER		(1<<12)		/* 用户设置 */
//#define D_PRO		(2<<12)		/* 工程设置 */
//#define D_SER		(3<<12)		/* 维修者设置 */
//#define D_FAC		(4<<12)		/* 厂家设置 */
//#define D_PUNP		(5<<12)		/* 邦普设置 */
//#define D_SAFEMASK	(0x000f<<12)	/* 屏蔽字 */
//
//#define FIND_ERR_PCB		0
#define FIND_ERR_MIN		1
#define FIND_ERR_NEXT		2
#define FIND_ERR_PREV		3
#define FIND_ERR_MAX		4
#define FIND_ERR_PCB_MIN	5
#define FIND_ERR_PCB_NEXT	6
#define FIND_ERR_PCB_PREV	7
#define FIND_ERR_PCB_MAX	8
#define FIND_ERR_TEST		9
//

//#define CELL_SIZE	(sizeof(CELL_DATA) / sizeof(U16))

#define SINGLE_HEAT  0
#define SINGLE_COOL	 1
#define HEAT_PUMP	 2
#define ELE_HEAT	 3

#define MAC_TYPE21 			21				/* 多功能热水机 */
#define MAC_TYPE1			1				/* 风冷双机头模块机 */
#define MAC_TYPE11			11				/* 热泵热水器 */
#define MAC_TYPE2			2				/* 水冷双机头模块机 */
#define MAC_TYPE3			3				/* 风冷四机头模块机 */
#define MAC_TYPE12			12				/* 高温热水机 */
#define MAC_TYPE_SELF		MAC_TYPE3		/* 本程序 */

/**************************************************/
/* 以下为新增定义，移植时视情况定义到其它文件中 */
#define	HIS_ERR_SIZE		(8)		                        /* 每条历史故障占用字节数(1字节(模块号)+1字节(故障号)+1字节(附加号)+5字节(时间) */
#define	MAX_HIS_ERR_TBL		(HIS_ERR_SIZE*MAX_HIS_ERROR)	/* 历史故障占用总字节数 */
DM603_EXT U08 his_err_tbl[MAX_HIS_ERR_TBL];	                /* 历史故障 */

#define MAX_TIME_TBL	(3+3*MAX_TIME)
DM603_EXT U08 dm603_time_tbl[MAX_TIME_TBL];	    /* 定时数据 */

/* 定时设置功能 */
#define		GET_TIME_EN()		GET_BIT(dm603_time_tbl[0], 0)
#define		SET_TIME_EN()		SET_BIT(dm603_time_tbl[0], 0)
#define		CLR_TIME_EN()		CLR_BIT(dm603_time_tbl[0], 0)

enum
{
	HOLIDAY_USED,
	HOLIDAY_HEAT_SET,
	HOLIDAY_HW_SET,
	HOLIDAY_START_YEAR,
	HOLIDAY_START_MONTH,
	HOLIDAY_START_DAY,
	HOLIDAY_END_YEAR,
	HOLIDAY_END_MONTH,
	HOLIDAY_END_DAY,
	HOLIDAY_MAX,
};
DM603_EXT U08 dm603_holiday_tbl[HOLIDAY_MAX];	    /* 休假模式数据 */

//DM603_EXT U08 dm603_time_tbl[MAX_TIME_TBL];	/* 定时数据 */

//DM603_EXT U16 debug_7[10];
DM603_EXT BOOL	f_blink_1S;			//1S闪烁标志
DM603_EXT BOOL  f_blink_1S_MODE;	// 模式1秒切换标志
DM603_EXT void disp_time(void);

/*----------新程序-------------------*/

typedef struct tagMENUITEM		// 菜单 
{
	U08  MenuType;			// 菜单类型
//	U08  MenuPassDis;		// 显示密码级别（用于判断是否显示该菜单）
	U16  MenuPassEnt;		// 进入密码权限（用于判断是否允许进入该菜单）
	U16  FuncId;			// 功能ID	根据菜单类型不同，功能不同(如果是参数，有无必要再强制转换为I16？)
	const U16 MenuMsg;			// 菜单名
	
	U16  MenuSign;			// 菜单属性
} MENUITEM;

// MenuType	菜单类型
#define MENU_FACE		0	// 进入界面的菜单，FuncId对应子界面的ID
#define MENU_ONOFF		1	// 单项ONOFF参数的菜单，FuncId对应要设置的参数（执行此菜单，则切换该参数的参数值ON/OFF）
#define MENU_ACTION		2	// ？？单纯执行动作的菜单，FuncId对应动作的ID（执行此菜单，则切换该动作，不切换界面）



// MenuPass 密码级别
#define PASS_NULL		0	// 不用密码
#define PASS_FUNC		((1<<PASS_USER)|(1<<PASS_SER)|(1<<PASS_FAC))		// 进入权限——高级功能界面
#define PASS_PARA		((1<<PASS_SER)|(1<<PASS_FAC)|(1<<PASS_SUPER))		// 进入权限——参数设置界面
#define PASS_LMT		((1<<PASS_LIMIT ))					// 进入权限——使用期限设置界面
// MenuSign	菜单属性

//#define M_HIDE	 	0x0400		// 隐藏属性

#define M_PASS	 	0x0001		// 进入需密码


#define ITEM_PER_PAGE	3								// 每页显示的菜单数（如果4条，则不显示提示行）
#define MENUITEM_SIZE	((GROUP_MAX>10)?GROUP_MAX:10)	// 菜单项数
DM603_EXT U08 Item_tbl[MAX_HMI][MENUITEM_SIZE];	//NOTE-CXW 是否每个屏独立？

//#define ALLOW_MENU_ONOFF	1	// 空间不够时可禁止此功能
//#ifdef ALLOW_MENU_ONOFF
//static U16 Item_value_tbl[MAX_HMI][ITEM_PER_PAGE];	// 用于单项参数值显示
//#endif	// end ALLOW_MENU_ONOFF

DM603_EXT U08 dm603_nextface[MAX_HMI];	// 预备跳转的下一界面，一般用于弹出密码界面时
DM603_EXT U16 dm603_PassNeed[MAX_HMI];	// 当前密码界面需要的权限


DM603_EXT U08 ParaGroup_Now[MAX_HMI];	// 当前参数组


/* 显示时钟信息 */
#define SEC			0X0001	// 秒
#define MINU		0X0002	// 分
#define HR			0X0004	// 时
#define COLON_BINK	0X0080	// 冒号闪烁
#define HMS_MASK	0X00FF	// 时间屏蔽字

#define HHMM		(HR|MINU)// 时分
#define HHMMSS		(HR|MINU|SEC)// 时分秒


#define DAY			0X0100	// 日
#define MON			0X0200	// 月	
#define YY			0X0400	// 年（2位）
#define YYYY		0X0800	// 年（4位）		// 英文时强制显示为17/12/31 （年月日）
#define CHN			0X8000	// 中文显示		
#define YMD_MASK	0XFF00	// 日期屏蔽字

#define YYMMDD		(YY|MON|DAY)// 年月日
#define YYYYMMDD	(YYYY|MON|DAY)// 年月日			


#define		CHAR_NUM			0X01	// 输入数字
#define		CHAR_ASCII			0X02	// 输入字母

typedef struct tagKEY                               
{                                                   
    U16 value;  //键值                              
    U16 time;   //按键时间                          
}KEY;  
DM603_EXT KEY Key[MAX_HMI];

typedef struct tag_MSG_ITEM_INFO
{
    U16 attr;
    U16 msg;
} MSG_ITEM_INFO;

DM603_EXT void Face_menu(U08 ItemPage, const MENUITEM *MenuItem, U08 ItemAll, U08 PassGrade,U08 *disp_buf);
DM603_EXT void Face_ParaGroup(U08 ItemPage, const MENUITEM *MenuItem,  const PARAMITEM *ParamGroup, U08 PassGrade, U08 *disp_buf);
DM603_EXT U08  Face_menu_hide(U08 ItemAll, U08 *Item_tbl, U08 Tbl_size);
DM603_EXT I16  Face_menu_action(U16 FuncID, U08 Act_opt);

DM603_EXT void DispInfo_Clock(U16 row, U16 col, RTC ClockDisp, U16 clock_type, U08 attrib, U08 *disp_buf);
DM603_EXT void DispInfo_Wifi(U16 row, U16 col, U08 fgServer, U08 *disp_buf);
DM603_EXT void PunpDispAscii(U08* string_ptr, U16 row, U16 col, U16 attrib);
DM603_EXT void PunpDispBmp(U16 bmp_code, U16 row, U16 col, U16 attrib);
DM603_EXT void PunpDispMenu(U16 licon, U16 lstr, U16 ricon, U16 rstr);
DM603_EXT void PunpDispNum(I16 num, U16 line, U16 pos, U16 format, U16 attrib);
DM603_EXT void PunpDispPv(I16 pv, U16 line, U16 pos, U16 format, U16 attrib);
DM603_EXT void PunpDispClearLine(U08 row1, U08 row2, U08 row3, U08 row4);
DM603_EXT void PunpDispClearScreen(void);
DM603_EXT void PunpDispText(U16 string_index, U16 row, U16 col, U16 attrib);
DM603_EXT void PunpDispFlashAll(void);
DM603_EXT void PunpDispSetLcdAutoDarkTime(U16 time_out);
DM603_EXT void PunpDispSetLcdLight(U08 sta);
DM603_EXT void PunpDispSetSound(U08 sta);
DM603_EXT void PunpDispSetLanguage(U08 language);
DM603_EXT U08 PunpDispGetLanguage(void);
DM603_EXT KEY PunpDispGetKey(void);
DM603_EXT RTC PunpDispGetClock(void);
DM603_EXT void PunpDispSetClock(RTC* pClock);

DM603_EXT void dm603_get_vfd_ver(U08 *ver_str);

/* 适配smarthmi */
extern const PARADICT para_dict[];
DM603_EXT U16 get_dict_num(void);

DM603_EXT U16 get_sys_p_num(void);
DM603_EXT U16 get_unit_p_num(void);
DM603_EXT U16 get_comp_p_num(void);

extern const U16 Msg_LimitType[];
extern const U16 msg_uint_o_cell[];
extern const U16 msg_uint_o_comp[];
extern const U16 msg_sys_o[];
extern const SMALL *para_small[];
extern const U16 para_large[];
extern const MENUITEM	MenuItem_ParaGroup[];
extern const U16 msg_m_error[];
extern const U16 msg_s_error[];
extern const U16 msg_PUMPf_error[];
extern const MSG_ITEM_INFO msg_uint_p_comp[];
extern const MSG_ITEM_INFO msg_uint_p_cell[];
extern const MSG_ITEM_INFO msg_sys_p[];
extern const U16 msg_sys_i[];
extern const U16 msg_uint_i_cell[];
extern const U16 msg_uint_i_comp[];
extern const MSG_ITEM_INFO msg_invt_para[];

#endif
