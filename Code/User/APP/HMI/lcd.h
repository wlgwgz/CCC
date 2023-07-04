#ifndef LCD_H
#define LCD_H

#ifdef LCD_GLOBAL
#define LCD_EXT
#else
#define LCD_EXT extern
#endif

/***********************************************************************************
         design by punp co.ltd copyright limited
         desinger   the driver of key broad
         date :
         name : key.h
***********************************************************************************/



/* 以下定义ascii码在disp_8_table[]的位置 */
#define  NUM_0        0
#define  NUM_1        1
#define  NUM_2        2
#define  NUM_3        3
#define  NUM_4        4
#define  NUM_5        5
#define  NUM_6        6
#define  NUM_7        7
#define  NUM_8        8
#define  NUM_9        9
#define  ASC_A        10
#define  ASC_B        11
#define  ASC_C        12
#define  ASC_D        13
#define  ASC_E        14
#define  ASC_F        15
#define  ASC_G        16
#define  ASC_H        17
#define  ASC_I        18
#define  ASC_J        19
#define  ASC_K        20
#define  ASC_L        21
#define  ASC_M        22
#define  ASC_N        23
#define  ASC_O        24
#define  ASC_P        25
#define  ASC_Q        26
#define  ASC_R        27
#define  ASC_S        28
#define  ASC_T        29
#define  ASC_U        30
#define  ASC_V        31
#define  ASC_W        32
#define  ASC_X        33
#define  ASC_Y        34
#define  ASC_Z        35
#define  A_8          	36
#define  B_8          	37
#define  C_8          	38
#define  D_8          	39
#define  E_8          	40
#define  F_8          	41
#define  G_8          	42
#define  DP_8         	43
#define  CHAR_OFF		44
#define  CHAR_ON		45
#define  ASC_EQU		46

#define  ASC_NEG		G_8
#define  ASC_LINE		D_8
#define  ASC_DOT		DP_8

/* S级产品显示屏后板程序 */
/* =============================================================== */
/* 显示属性 */
#define DCS_NORMAL			0x0000	/* 正常显示 */
/* 第0~2位: 位数 */
#define DNS_DIGIT1			0x0001	/* 显示的有效位数为1位(不含小数点和负号) */
#define DNS_DIGIT2			0x0002	/* 显示的有效位数为2位(不含小数点和负号) */
#define DNS_DIGIT3			0x0003	/* 显示的有效位数为3位(不含小数点和负号) */
#define DNS_DIGIT4			0x0004	/* 显示的有效位数为4位(不含小数点和负号) */
#define DNS_DIGIT5			0x0005	/* 显示的有效位数为5位(不含小数点和负号) */
#define DNS_DIGIT_MASK		0x0007	/* 有效位数的屏蔽位(低3位) */
/* 第3~4位: 小数点 */
#define DNS_FLOAT1			0x0008	/* 带1位小数 */
#define DNS_FLOAT2			0x0010	/* 带2位小数 */
#define DNS_FLOAT_MASK		0x0018	/* 小数位数屏蔽位(第3,4位) */
#define DNS_FLOAT_BIN_NUM	3		/* 小数点属性的二进制位置 */
/* 第5位: 前导0 */
#define DNS_ZERO			0x0020	/* 前导零显示 */
/* 第6位: 特殊字符 */
#define DNS_SPE				0x0040	/* 按特殊字符(图片)显示 */
/* 第7位: 居中 */
#define DTS_CENTER			0x0080	/* 居中 */

/* 高8为为直接影响显示的属性(会传到前板处理) */
/* 第8~10位: 位置 */
#define DTS_CLR_HEAD		0x0100	/* 清头 */
#define DTS_CLR_TRAIL		0x0200	/* 清尾 */
///* 第11位: 特殊用途(正常显示时不可用该属性位) */
//#define DTS_CLR_LINE		0x0800	/* 清行 */
/* 第11位: 靠右 */
#define DTS_RIGHT			0x0800	/* 靠右 */

/* 第12位: 反白 */
#define DCS_REVERSE			0x1000	/* 反白 */
/* 第13位: 闪烁 */
#define DCS_BLINK			0x2000	/* 闪烁 */
/* 第14：刷新 */
#define DCS_FLASH			0x4000	/* 刷新 */

#define DTS_CLR_ALL			(DTS_CLR_HEAD | DTS_CLR_TRAIL)	//清头清尾

/* 第15位: 备用 */

#define ATTRIB_LONG_KEY			0x1000		/* 长键 */ 
#define ATTRIB_CNTNUS_KEY		0x2000		/* 连续键 */ 
#define ATTRIB_ATTRIBUTE_KEY    0xf000		/* 属性键 */

LCD_EXT U16 key_value[MAX_HMI];				/* 键值(前板传来的原始值) */
LCD_EXT U16 Main_Key[MAX_HMI];				/* 键值(经处理后的按键) */
LCD_EXT U16 ch_end_f[MAX_HMI];				/* 前板汉字结束编码 */
LCD_EXT U16 bmp_end_f[MAX_HMI];				/* 前板自定义图片结束编码 */
LCD_EXT U08 commu_sucess[MAX_HMI];			/* 上次通讯成功 */
LCD_EXT U08 fg_disp_back[MAX_HMI];			/* 后板控制界面 */
LCD_EXT BOOL fg_fn_status; 
LCD_EXT BOOL fg_fn_mode;

LCD_EXT U08 cnt_key_static[MAX_HMI];			/* 按键持续时间计时(100ms) */
LCD_EXT U08 cnt_cntnus_add[MAX_HMI];			/* 连续键递增计时(5ms) */
LCD_EXT BOOL fg_long_key;			/* 长键标志 */
LCD_EXT BOOL fg_cntnus;				/* 连续键标志 */

#define KEYT_TIME_LONG				30		/* 3秒 */
#define KEYT_TIME_CNTNUS_CNFRM		10		/* 1秒 */
#define KEYT_TIME_CNTNUS_ADD		1		/* 100毫秒 */

#define LOCK_T_IN_MAIN			3		/* 主界面操作锁定时间(秒) */
#define LOCK_T_IN_OTHER			45		/* 其它界面操作锁定时间(秒) */
#define LOCK_T_CLUE				3		/* 锁定提示持续时间(秒) */

LCD_EXT BOOL fg_disp_lock;		        /* 锁屏标志：未处理多屏方案 */

/* 语言(范围为1~7) */
#define LANGUE_NULL				0		/* 空闲，无效状态 */
#define LANGUE_CHINESE			1		/* 中文 */
#define LANGUE_ENGLISH			2		/* 英文 */
#define LANGUE_JAPANESE			3		/* 日文 */
#define LANGUE_MAX              4
typedef struct date        /* 时钟 */
{
	U16  sec;				/* 秒 */
	U16  min;				/* 分 */
	U16  hour;				/* 时 */
	U16  day;				/* 日 */
	U16  month;				/* 月 */
	U16  week;				/* 星期 */
	U16  year;				/* 年 */
	
} RTC;
LCD_EXT RTC clock[MAX_HMI];		/* 实时时钟 */ 
#define LOG_RTC RTC

LCD_EXT U08 clk_base;			/* 基准时钟(以哪个HMI的时钟作为基准时钟) */

#define MAX_RTC_ITEM		7		/* 时钟总项目数 */
#define RTC_SEC				0		/* 秒 */  
#define RTC_MIN				1		/* 分 */  
#define RTC_HOUR			2		/* 时 */  
#define RTC_DAY				3		/* 日 */  
#define RTC_MONTH			4		/* 月 */  
#define RTC_WEEK			5		/* 星期 */
#define RTC_YEAR			6		/* 年 */  

LCD_EXT BOOL fg_flash_all;				/* 整屏刷新需求 */
LCD_EXT BOOL fg_is_auto_flash_all;	    /* 是否自动整屏刷新 */
LCD_EXT U08 cnt_disp_steady[MAX_HMI];	/* 界面不刷新持续时间(秒) */
LCD_EXT U08 cnt_no_operate[MAX_HMI];	/* 无按键持续时间(分) */
LCD_EXT U08 cnt_menu_lock[MAX_HMI];		/* 界面锁定倒计时(秒) */
LCD_EXT U08 cnt_lock_clue[MAX_HMI];		/* 锁定提示持续计时(秒) */
LCD_EXT U08 fg_set_clk[MAX_HMI];		/* 设置时钟标志 */
LCD_EXT U08 fg_active[MAX_HMI];			/* 屏是否已被激活(是否操作过) */
LCD_EXT U08 cnt_no_operate_sec[MAX_HMI];/* 无按键持续时间(秒) */
LCD_EXT U08 fg_disp_ver_back[MAX_HMI];	/* 显示屏版本号 */
LCD_EXT BOOL fg_sound;					/* 蜂鸣器响标志 */
//LCD_EXT U08 sound_delay[MAX_HMI];		// 延时关蜂鸣器		 
 
LCD_EXT U08 cnt_non_commu[MAX_HMI];		/* 无通讯计时 */
LCD_EXT U08 cnt_screen_power[MAX_HMI];	/* 显示屏倒计时 */

LCD_EXT BOOL fg_disp_all;		/* 整屏刷新指示标志(有效区间为一个显示循环) */
LCD_EXT BOOL fg_disp_value;		/* 数值刷新指示标志(有效区间为一个显示循环) */
LCD_EXT BOOL fg_disp_blink;		/* 闪烁指示标志(有效区间为一个显示循环) */

#define MAX_FLASH_INTERVAL		5		/* 最长刷新时间（秒） */
#define MAX_OPER_WAIT			60		/* 最长操作等待时间（秒），无操作则进屏保 */

#define   ENLARGE_ROW   16   //更加新协议行地址按点来算
#define   ENLARGE_LOW   8    //根据新协议列地址按点来算

#define SCREEN_1		4
#define SCREEN_4		4  //每屏显示4个参数信息 如邦普测试界面


LCD_EXT U08 cnt_com_forbid;
LCD_EXT BOOL fg_com_forbid;
LCD_EXT U08 buf_index;					/* 显示缓存索引 */

LCD_EXT U08 disp_routine(U08 *disp_buf);

LCD_EXT void lcd_init(void);
LCD_EXT void dm603_tasking(U08 *disp_buf);

LCD_EXT void disp_blank16(U16 row, U16 column, U16 num, U16 attrib,U08 *disp_buf);

LCD_EXT void disp_next_flg(U16 index_now, U16 index_max,U16 index_min,U16 attrib,U08 *disp_buf);
//LCD_EXT void disp_index_list(U16 index_now, U16 index_max, const U08 *string_ptr[], U16 attrib,U08 *disp_buf);


LCD_EXT void disp_switch(U08 *disp_buf);
LCD_EXT void disp_test_string(PTR_FAR_C08 string_ptr, U16 row, U16 col, U16 attrib, U08 *disp_buf);
LCD_EXT void disp_string(U16 string_index, U16 row, U16 col, U16 attrib, U08 *disp_buf);
LCD_EXT void disp_string_ptr8(PTR_FAR_C08 string_ptr, U16 row, U16 col, U16 attrib, U08 *disp_buf);
LCD_EXT void disp_bmp(U16 bmp_code, U16 row, U16 col, U16 attrib, U08 *disp_buf);
LCD_EXT void disp_value_I16(I16 value, U16 row, U16 col, U16 attrib, U08 *disp_buf);
LCD_EXT void disp_value_U16(U16 value, U16 row, U16 col, U16 attrib, U08 *disp_buf);
LCD_EXT void disp_value_I08(I08 value, U16 row, U16 col, U16 attrib, U08 *disp_buf);
LCD_EXT void disp_value_U08(U08 value, U16 row, U16 col, U16 attrib, U08 *disp_buf);
LCD_EXT void disp_clr_screen(U08 *disp_buf);
LCD_EXT void disp_clr_line(U16 row, U08 *disp_buf);
LCD_EXT void disp_version_f(U16 row, U16 col, U16 attrib, U08 *disp_buf);
LCD_EXT void disp_set_snd(void);
LCD_EXT void disp_clr_snd(void);
LCD_EXT void disp_snd_100ms(void);
LCD_EXT void disp_read_key(void);
LCD_EXT void disp_test_f(void);
LCD_EXT void disp_set_langue(U08 langue);
LCD_EXT void disp_set_clk(U08 *disp_buf);
LCD_EXT void disp_value_16(I16 value, U16 row, U16 col, U16 format, U16 attrib, U08 *disp_buf);
LCD_EXT BOOL disp_wr_hmi_clk(U08 *disp_buf, RTC clock_src);
LCD_EXT void disp_trans_sync(U08 *disp_buf);
LCD_EXT void disp_trans_ty_info(U08 *disp_buf);
LCD_EXT void disp_trans_ty_data(U08 *disp_buf, U08 dpid_idx);
LCD_EXT void disp_trans_ty_reset(U08 *disp_buf);
LCD_EXT void disp_trans_init(void);
#endif
