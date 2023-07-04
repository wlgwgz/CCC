/***********************************************************************************
         design by punp co.ltd copyright limited
         desinger
         date :
         name : lcd.c
***********************************************************************************/

#define LCD_GLOBAL

#include "includes.h"

/* S级产品显示屏后板程序 */
#define ADDR_ASC_END	0x0080		/* ASCII结束地址 */
#define ADDR_BMP_BGN	0xf500		/* 自定义图片起始地址 */
#define ADDR_BMP_END	0xf8ff		/* 自定义图片结束地址 */

#define AC_LONG					1		/* 字符宽度 */
#define AC_HIGH					2		/* 字符高度 */
#define HZ_LONG					2		/* 汉字宽度 */
#define HZ_HIGH					2		/* 汉字高度 */

#define LCM_COLUMN_NUM		16			/* 液晶的列数(字节单位) */

#define SCREEN_POWER_TIME		5		/* 显示屏倒计时时间 */
/*----------------------------------------------------*/
	/* 以下变量及宏定义不可随意更改 */

#define MAX_ROW			64		/* 最大行值 */
#define MAX_COL			128		/* 最大列值 */
#define MAX_DISP		255		/* 显示缓存字节数 */
//#define MAX_DISP		100		/* 调试 */

#define MIN_CODE_BYTE		7		/* 送编码时，单元最少字节数(5+2) */
#define MIN_CODE_WORD		4		/* 记忆编码时，单元最少字数 */
#define MIN_PIXEL_BYTE		41		/* 送点阵时，单元最少字节数(9+32),至少一个汉字,避免显示半个汉字 */

//U08 disp_buf[MAX_DISP];			/* 显示缓存 */
//U08 buf_index;					/* 显示缓存索引 */
static U08 spe_index;			/* 编码记忆缓冲区索引 */
#define MAX_CMD_SUB		2		/* 子命令个数 */
static U08 cmd_sub[MAX_CMD_SUB];/* 子命令(不带任何数据，如：清屏、清行等) */
static BOOL fg_snd_100ms_need; /* 100ms鸣叫需求 */

#define MAX_UNIT_NUM		(64+10)		/* 最大显示单元个数 */ // zzp 留一点冗余,防止覆盖时超出范围
#define MAX_PIX_CODE		32		/* 编码记录区缓冲区大小(字为单位，每个显示单元占4个字) */
static U16 total_num;		/* 总字符个数 */
static U16 cmplt_num;		/* 已完成字符个数 */
static U16 pixel_code[MAX_PIX_CODE];	/* 最大支持“MAX_PIX_CODE/4”个点阵单元 */
static U08 pixel_index = 0;	/* 传送点阵时编码索引 */

static BOOL fg_pixel_finish;    /* 点阵传输完成标志 */

static BOOL fg_disp_overflow;	/* 缓冲区溢出 */
static BOOL fg_first_commu;		/* 与前板第一次通讯 */
static BOOL fg_cur_code_ov;		/* 当前编码超界 */
static BOOL fg_f_need_update;	/* 前板需要升级(没有的字太多) */
static BOOL fg_f_need_update_has_remind;	/* 前板需要升级已提醒标志 */

static U16 pixel_addr_ofst;		/* 点阵的偏移地址(一个显示单元中的偏移) */

#define NUM_NORMAL				0		/* 正常数字 */
#define NUM_SPE					1		/* 特殊数字(自定义图片显示) */

#define NUM_DEC					10		/* 十进制 */
#define NUM_HEX					16		/* 十六进制 */

#define MAX_VALUE_NUM			8		/* 数值最大位数(5位数据+小数点+负号+结束符) */

/* 边界编码检查 */
#define CD_NORMAL				1		/* 正常 */
#define CD_ABN_BACK				2		/* 后板自身异常 */
#define CD_ABN_MUTUAL			3		/* 后板和前板匹配异常 */
#define CD_ABN_CUR				4		/* 后当前编码异常(超界) */

/* 分隔符(不可随意更改，和通讯协议有关) */
#define UINT_END		0xfffd	/* 单元分隔符 */
#define RECORD_END		0xfffe	/* 记录分隔符 */
/* 命令 */
#define TRANS_CLR_ALL			0x40	/* 清屏 */
#define TRANS_CLR_ROW			0x41	/* 清行 */
#define TRANS_CODE				0x42	/* 传送编码 */
#define TRANS_VER_F				0x43	/* 显示前板版本号 */
#define TRANS_TIME_F			0x44	/* 显示前板时钟 */
#define TRANS_PIXEL				0x45	/* 传送点阵(包含行列等数据) */
#define TRANS_PIXEL_ONLY		0x46	/* 传送点阵(仅有点阵数据) */
#define TRANS_BAK4				0x47	/* 备用 */
#define TRANS_SYNC              0x4F    /* 状态同步 */
#define TRANS_TY_INFO           0x51    /* 涂鸦WiFi屏_上报产品ID */
#define TRANS_TY_DATA           0x52    /* 涂鸦WiFi屏_上报数据 */
#define TRANS_TY_CMD            0x53    /* 涂鸦WiFi屏_命令 */
#define TRANS_CLOCK				0x5f	/* 写前板时钟 */
#define TRANS_SIMPLE			0x60	/* 简单命令(无显示数据的命令) */
#define TRANS_NONE				0x7f	/* 无显示数据 */
/* 子命令 */
	#define CMD_SUB_NONE		0x00	/* 空命令 */
//	#define CMD_BAK1			0x01	/* 备用 */
	#define CMD_SUB_SND_ON		0x02	/* 开蜂鸣器 */
	#define CMD_SUB_SND_OFF		0x04	/* 关蜂鸣器 */
	#define CMD_SUB_SND_100MS	0x08	/* 蜂鸣器响100ms */
	#define CMD_SUB_LCD_OFF		0x10	/* 关背光 */
	#define CMD_SUB_LCD_ON		0x20	/* 开背光 */
	#define CMD_SUB_TEST_F		0x40	/* 进入前板测试程序 */
	#define CMD_SUB_FORBID_AUTO_LCD_ON		0x80	/* 禁止自动开背光 */
	//#define CMD_SUB_SET_CLK		0x80	/* 设置时钟 */
	/* 第二个字节 */
	#define CMD_LANGUE_BGN		0		/* 语言选择起始BIT */
	#define CMD_LANGUE_BITMASK	0x07	/* 语言选择对应的BITMASK */



#define FONT_STD				0		/* 标准码 */
#define FONT_SPE				1		/* 自定义码 */

	/* 开机界面，主界面， 用户设置， 厂家设置， 状态查询，故障查询，历史故障查询，密码，  使用期限， 期限提醒 */
enum {DISP_OPEN, DISP_MAIN, DISP_USER, DISP_SET, DISP_CHK, DISP_ERR, DISP_HIS, DISP_PASS, DISP_LIMIT, DISP_PRE};

static void disp_get_pixel(U16 font_code, U16 *addr_ofst, U08 *disp_buf);
static void disp_get_num(U16 v, U08 num, U08 *buf);
static U08 disp_value_dot(U08 *buf_src, U08 v_num, U08 *max_num, U16 attrib);
static void disp_num_code(U08 *buf_src, U08 num, U08 *buf_dest);
static void disp_cal_dim(U16 font_code, U08 *ch_long, U08 *high);
static void disp_pixel(U08 *disp_buf);
static void disp_code_16(PTR_FAR_C08 string_ptr, U16 row, U16 col, U16 attrib, U08 *disp_buf);
static BOOL disp_screen_dark(U08 *disp_buf);
static void disp_mutual_abnormal(U08 bnd_chk, U08 *disp_buf);
//static void disp_value_neg(U08 *buf_src, U08 *vld_num, BOOL fg_neg);
static void disp_value_neg(U08 *buf_src, U08 *vld_num, U08 *max_num, BOOL fg_neg);
static BOOL disp_is_value_need_flash(U16 attrib);
static BOOL disp_is_str_in_font(U16 ch_code);
static void disp_record_init(void);
static BOOL disp_is_code_cmplt(void);
static U08 disp_bound_code_check(void);
static void disp_cur_code_check(U16 font_code);
static void disp_wr_cmd(U08 index_bak, U08 trans_cmd, U08 *disp_buf);
static void disp_deal_snd(void);
static void disp_screen_power(void);
static void disp_cmd_simple(U08 *disp_buf);
static void disp_lcd_off(void);
static void disp_lcd_on(void);
static BOOL disp_is_bmp(U16 font_code);
static void disp_value_cpmt_blank(U08 *buf_src, U08 vld_num, U08 max_num, U16 attrib);
static BOOL disp_is_text_need_flash(U16 attrib);
static void disp_clk_syn(U08 *disp_buf);
//static void disp_clr_buf_data(void);

/******************************************************
函数：disp_switch
功能：界面转换。
	根据按键信息处理各个界面之间的切换
参数：无
返回值：无
*****************************************************/
void disp_switch(U08 *disp_buf)
{
#ifdef USE_HMI_DISP
	dm603_tasking(disp_buf);
#endif	
}

/******************************************************
函数：disp_string_ptr8
功能：显示字符串(直接显示)。
参数：字符串编码起始地址(8位数据的指针); 行(点); 列(点); 属性; 缓冲区地址(写)
返回值：无
*****************************************************/
void disp_string_ptr8(PTR_FAR_C08 string_ptr, U16 row, U16 col, U16 attrib, U08 *disp_buf)
{
	disp_record_init();		/* 记录变量初始化 */
	
	if (disp_is_text_need_flash(attrib))
	{
    	disp_code_16(string_ptr, row, col, attrib, disp_buf);
    }
}

/******************************************************
函数：disp_test_string
功能：显示字符串。
参数：字符串编码起始地址(16位数据指针); 行(点); 列(点); 属性; 缓冲区地址(写)
返回值：无
*****************************************************/
void disp_test_string(PTR_FAR_C08 string_ptr, U16 row, U16 col, U16 attrib, U08 *disp_buf)
{
	disp_record_init();		/* 记录变量初始化 */
	
	if (disp_is_text_need_flash(attrib))
	{
		disp_code_16(string_ptr, row, col, attrib, disp_buf);
    }
}

/******************************************************
函数：disp_string
功能：显示字符串。
参数：字符串编码序号; 行(点); 列(点); 属性; 缓冲区地址(写)
返回值：无
*****************************************************/
void disp_string(U16 string_index, U16 row, U16 col, U16 attrib, U08 *disp_buf)
{
	PTR_FAR_C08 string_ptr=NULL;
	disp_record_init();		/* 记录变量初始化 */

	if (disp_is_text_need_flash(attrib))
	{
		if (dm603_language == LANGUE_CHINESE)
		{
#ifdef USE_GBK
			string_ptr = IDX_TO_C_PTR(string_index);
#elif defined (USE_UNICODE)
            string_ptr = chinese_uncompress_text(string_index);
#endif
		}
		else
		{
#ifdef USE_GBK
			string_ptr = IDX_TO_E_PTR(string_index);
#elif defined (USE_UNICODE)
            string_ptr = english_uncompress_text(string_index);
#endif
		}
		disp_code_16(string_ptr, row, col, attrib, disp_buf);
    }
}
/******************************************************
函数：disp_bmp
功能：显示图片。
	决定使用编码还是点阵显示。
	如果一次通讯显示不全，需要记忆上次状态，继续显示，直到显示完整为止。
参数：图片编码; 行; 列; 属性; 缓冲区地址(写)
返回值：无
要求：一次只允许传入一个单元。
*****************************************************/
void disp_bmp(U16 bmp_code, U16 row, U16 col, U16 attrib, U08 *disp_buf)
{
//	U08 font_long, font_high;
	U16 temp_code;
	U16 temp_row, temp_col;
	U08 buf_index_bak = buf_index;		/* 备份索引 */
	
	disp_cur_code_check(bmp_code);
	
	if (buf_index > MAX_DISP - MIN_CODE_BYTE)			/* 显示缓存超界 */
//      点阵不够，后续继续传编码	    
//      || spe_index > MAX_PIX_CODE - MIN_CODE_WORD)    /* 点阵单元记忆区超界 */
	{
		fg_disp_overflow = TRUE;
		return;
	}
	
	temp_code = bmp_code;
	temp_row = row;
	temp_col = col;
	
	disp_record_init();		/* 记录变量初始化 */
	
	if (disp_is_text_need_flash(attrib))
	{
		buf_index_bak = buf_index;
		buf_index += 2;			/* 预留命令和数据个数的位置 */
		
    	/* 编码写入缓冲区 */
		if (disp_is_str_in_font(temp_code))		/* 编码在字库中 */
		{
			cmplt_num++;
		}
        else if (spe_index > MAX_PIX_CODE - MIN_CODE_WORD)/* 点阵不够,继续传编码 */  
        {
            /* 没有的汉字过多,前板需要升级 */
            fg_f_need_update = TRUE;
            cmplt_num++;
        }
		else 	/* 不在字库中，则记忆编码 */
		{
			pixel_code[spe_index++] = temp_row;
			pixel_code[spe_index++] = temp_col;
			pixel_code[spe_index++] = attrib;
			pixel_code[spe_index++] = temp_code;			
		}
		
		disp_buf[buf_index++] = HIGH_BYTE(temp_row);
		disp_buf[buf_index++] = LOW_BYTE(temp_row);
		disp_buf[buf_index++] = HIGH_BYTE(temp_col);
		disp_buf[buf_index++] = LOW_BYTE(temp_col);
		
		disp_buf[buf_index++] = HIGH_BYTE(attrib);
		disp_buf[buf_index++] = HIGH_BYTE(temp_code);
		disp_buf[buf_index++] = LOW_BYTE(temp_code);
			
		total_num++;
    	
    	disp_wr_cmd(buf_index_bak, TRANS_CODE, disp_buf);		/* 传送编码命令 */
    }
}

/******************************************************
函数：disp_value_I16
功能：显示16为有符号数。
	将数值转换为单个数字，存放到指定存储区中，最终按字符串的方式显示。
	考虑了小数显示。
参数：数值; ; ; 
返回值：无
*****************************************************/
void disp_value_I16(I16 value, U16 row, U16 col, U16 attrib, U08 *disp_buf)
{
//	U08 i;
	U08 disp_num;		/* 显示位数(数据位数，不含小数点) */
	U08 buf_src[MAX_VALUE_NUM];
	U08 buf_dest[MAX_VALUE_NUM];
	U08 max_num;		/* 最大数据位数 */
	U08 vld_num;		/* 有效数据位数 */
	I16 temp_value;
	BOOL fg_neg_need = FALSE;	/* 需要显示负号标志 */
	
	disp_record_init();		/* 记录变量初始化 */
	
	if (disp_is_value_need_flash(attrib))		/* 数值刷新 */
	{
		temp_value = value;
		if (value < 0)	/* 有符号显示且值小于0 */
		{
			temp_value = ~value;		
			temp_value++;
			fg_neg_need = TRUE;
		}
		
    	disp_num = attrib & DNS_DIGIT_MASK;		/* 显示位数 */
    	if (disp_num > DNS_DIGIT5)		disp_num = DNS_DIGIT5;
    	max_num = disp_num;	
    	vld_num = disp_num;
    		
		disp_get_num(temp_value, disp_num, buf_src);	/* 取数字 */
		vld_num = disp_value_dot(buf_src, disp_num, &max_num, attrib);	/* 处理小数点和前导0 */
		disp_value_neg(buf_src, &vld_num, &max_num, fg_neg_need);		/* 显示负号 */
        
        disp_value_cpmt_blank(buf_src, vld_num, max_num, attrib);	/* 补空格 */
        
    	disp_num_code(buf_src, max_num, buf_dest);					/* 给数字编码 */
    	disp_code_16(buf_dest, row, col, attrib, disp_buf);			/* 按字符串的方式显示 */
    }
}

/******************************************************
函数：disp_value
功能：显示16位无符号数
	将数值转换为单个数字，存放到指定存储区中，最终按字符串的方式显示。
	支持小数显示。
参数：数值; ; ; 
返回值：无
*****************************************************/
void disp_value_U16(U16 value, U16 row, U16 col, U16 attrib, U08 *disp_buf)
{
//	U08 i;
	U08 disp_num;		/* 显示位数(数据位数，不含小数点) */
	U08 buf_src[MAX_VALUE_NUM];
	U08 buf_dest[MAX_VALUE_NUM];
	U08 max_num;		/* 最大数据位数 */
	U08 vld_num;		/* 有效数据位数 */
	
	disp_record_init();		/* 记录变量初始化 */
	
	if (disp_is_value_need_flash(attrib))		/* 数值刷新 */
	{		
    	disp_num = attrib & DNS_DIGIT_MASK;		/* 显示位数 */
    	if (disp_num > DNS_DIGIT5)		disp_num = DNS_DIGIT5;
    	max_num = disp_num;
    	vld_num = disp_num;
    		
		disp_get_num(value, disp_num, buf_src);	/* 取数字 */
		vld_num = disp_value_dot(buf_src, disp_num, &max_num, attrib);	/* 处理小数点和前导0 */
        
        disp_value_cpmt_blank(buf_src, vld_num, max_num, attrib);		/* 补空格 */
        
    	disp_num_code(buf_src, max_num, buf_dest);						/* 给数字编码 */
    	disp_code_16(buf_dest, row, col, attrib, disp_buf);				/* 按字符串的方式显示 */
    }
}

/******************************************************
函数：disp_value_I08
功能：显示8位有符号数。
	将数值转换为单个数字，存放到指定存储区中，最终按字符串的方式显示。
	考虑了小数显示。
参数：数值; ; ; 
返回值：无
*****************************************************/
void disp_value_I08(I08 value, U16 row, U16 col, U16 attrib, U08 *disp_buf)
{
//	U08 i;
	U08 disp_num;		/* 显示位数(数据位数，不含小数点) */
	U08 buf_src[MAX_VALUE_NUM];
	U08 buf_dest[MAX_VALUE_NUM];
	U08 max_num;		/* 最大数据位数 */
	U08 vld_num;		/* 有效数据位数 */
	I08 temp_value;
	BOOL fg_neg_need = FALSE;	/* 需要显示负号标志 */
	
	disp_record_init();		/* 记录变量初始化 */
	
	if (disp_is_value_need_flash(attrib))		/* 数值刷新 */
	{
		temp_value = value;
		if (value < 0)	/* 有符号显示且值小于0 */
		{
			temp_value = ~value;		
			temp_value++;
			fg_neg_need = TRUE;
		}
		
    	disp_num = attrib & DNS_DIGIT_MASK;		/* 显示位数 */
    	if (disp_num > DNS_DIGIT3)		disp_num = DNS_DIGIT3;
    	max_num = disp_num;	
    	vld_num = disp_num;
    		
		disp_get_num(temp_value, disp_num, buf_src);					/* 取数字 */
		vld_num = disp_value_dot(buf_src, disp_num, &max_num, attrib);	/* 处理小数点和前导0 */
		disp_value_neg(buf_src, &vld_num, &max_num, fg_neg_need);		/* 显示负号 */
        
        disp_value_cpmt_blank(buf_src, vld_num, max_num, attrib);	/* 补空格 */
        
    	disp_num_code(buf_src, max_num, buf_dest);					/* 给数字编码 */
    	disp_code_16(buf_dest, row, col, attrib, disp_buf);			/* 按字符串的方式显示 */
    }
}

/******************************************************
函数：disp_value_U08
功能：显示8位无符号数
	将数值转换为单个数字，存放到指定存储区中，最终按字符串的方式显示。
	支持小数显示。
参数：数值; ; ; 
返回值：无
*****************************************************/
void disp_value_U08(U08 value, U16 row, U16 col, U16 attrib, U08 *disp_buf)
{
//	U08 i;
	U08 disp_num;		/* 显示位数(数据位数，不含小数点) */
	U08 buf_src[MAX_VALUE_NUM];
	U08 buf_dest[MAX_VALUE_NUM];
	U08 max_num;		/* 最大数据位数 */
	U08 vld_num;		/* 有效数据位数 */
		
	disp_record_init();		/* 记录变量初始化 */
	
	if (disp_is_value_need_flash(attrib))		/* 数值刷新 */
	{		
    	disp_num = attrib & DNS_DIGIT_MASK;		/* 显示位数 */
    	if (disp_num > DNS_DIGIT3)		disp_num = DNS_DIGIT3;
    	max_num = disp_num;
    	vld_num = disp_num;
    		
		disp_get_num(value, disp_num, buf_src);	/* 取数字 */
		vld_num = disp_value_dot(buf_src, disp_num, &max_num, attrib);	/* 处理小数点和前导0 */
        
        disp_value_cpmt_blank(buf_src, vld_num, max_num, attrib);	/* 补空格 */
        
	   	disp_num_code(buf_src, max_num, buf_dest);			/* 给数字编码 */
    	disp_code_16(buf_dest, row, col, attrib, disp_buf);	/* 按字符串的方式显示 */
    }
}

/*****************************************************************************
函  数 : disp_value_16()
功  能 : 显示16位有符号/无符号数
参  数 : I16 value     : 
         U16 row       : 
         U16 col       : 
         U16 format    : 
         U16 attrib    : 
         U08 *disp_buf : 
返  回 : 无
其  他 : 无
*****************************************************************************/
void disp_value_16(I16 value, U16 row, U16 col, U16 format, U16 attrib, U08 *disp_buf)
{
	U08 num_buf[8];
		
	disp_record_init();		/* 记录变量初始化 */
	
	if (disp_is_value_need_flash(attrib))		/* 数值刷新 */
	{		
    	num_buf[Fun_NumFormat((U16)value, num_buf, format)] = '\0';
    	disp_code_16(num_buf, row, col, attrib, disp_buf);	/* 按字符串的方式显示 */
    }
}

/**********************************************
函数：disp_clr_screen
功能：清屏
参数：无
返回值：无
***********************************************/
void disp_clr_screen(U08 *disp_buf)
{
	U08 buf_index_bak = buf_index;		/* 备份索引 */
	
	if (buf_index > MAX_DISP - MIN_CODE_BYTE)			/* 显示缓存超界 */	
	{
		fg_disp_overflow = TRUE;
		return;
	}
	
	buf_index_bak = buf_index;
	buf_index += 2;			/* 预留命令和数据个数的位置 */
		
    disp_buf[buf_index++] = 0;
	
	disp_wr_cmd(buf_index_bak, TRANS_CLR_ALL, disp_buf);	/* 清行命令 */
}
/**********************************************
函数：disp_clr_line
功能：清行
		用属性处理，遵循传编码的格式，但无编码数据。
参数：无
返回值：无
***********************************************/
void disp_clr_line(U16 row, U08 *disp_buf)
{
	U08 buf_index_bak = buf_index;		/* 备份索引 */
	
	if (buf_index > MAX_DISP - MIN_CODE_BYTE)			/* 显示缓存超界 */	
	{
		fg_disp_overflow = TRUE;
		return;
	}
	
	buf_index_bak = buf_index;
	buf_index += 2;			/* 预留命令和数据个数的位置 */
		
    disp_buf[buf_index++] = HIGH_BYTE(row);
	disp_buf[buf_index++] = LOW_BYTE(row);			/* 行地址 */
	
	disp_wr_cmd(buf_index_bak, TRANS_CLR_ROW, disp_buf);	/* 清行命令 */
}

/******************************************************************** 
 函 数 名:	 void lcd_clcontime(void) 
 功  能:定时清屏函数没有按键操作超过设定时间且允许清屏则清屏
 入口参数:
 返 回 值:无
***********************************************************************/
//void lcd_clcontime(U08 *disp_buf)
//{
//	//没有按键按下超过设定值且允许清屏则清屏
//	if(g_screen_clc_time == 0)   
//	{ 
//		g_screen_clc_time = CLC_TIME; //重新计时
//		fg_flash_all = TRUE;
//		//fg_flash_once = TRUE;
//		disp_clr_screen(disp_buf);
//	}
//}

/**********************************************
函数：disp_version_f
功能：显示前板版本
		用属性处理，遵循传编码的格式，但无编码数据。
参数：无
返回值：无
***********************************************/
void disp_version_f(U16 row, U16 col, U16 attrib, U08 *disp_buf)
{
	U08 buf_index_bak = buf_index;		/* 备份索引 */
	
	if (buf_index > MAX_DISP - MIN_CODE_BYTE)			/* 显示缓存超界 */	
	{
		fg_disp_overflow = TRUE;
		return;
	}
	
	buf_index_bak = buf_index;
	buf_index += 2;			/* 预留命令和数据个数的位置 */
    
    disp_buf[buf_index++] = HIGH_BYTE(row);
	disp_buf[buf_index++] = LOW_BYTE(row);			/* 行地址 */
	disp_buf[buf_index++] = HIGH_BYTE(col);	
	disp_buf[buf_index++] = LOW_BYTE(col);			/* 列地址 */
	disp_buf[buf_index++] = HIGH_BYTE(attrib);			/* 属性 */
	
	disp_wr_cmd(buf_index_bak, TRANS_VER_F, disp_buf);		/* 显示版本号命令 */
}
/**********************************************
函数：disp_set_snd()
功能：开蜂鸣器
参数：无
返回值：无
***********************************************/
void disp_set_snd(void)
{
#ifndef USE_NO_SPEAKER
	fg_sound = TRUE;
#endif 
}
/**********************************************
函数：disp_clr_snd()
功能：关蜂鸣器
参数：无
返回值：无
***********************************************/
void disp_clr_snd(void)
{
	fg_sound = FALSE;
}
/**********************************************
函数：disp_snd_100ms
功能：蜂鸣器响100MS
参数：无
返回值：无
***********************************************/
//void disp_snd_100ms(void)
//{
//    cmd_sub[0] |= CMD_SUB_SND_100MS;
//}

/**********************************************
函数：disp_arg_init
功能：显示变量初始化
		上电初始化一次
参数：无
返回值：无
***********************************************/
void lcd_init(void)
{
	U08 i;
	
	fg_first_commu = FALSE;
	fg_flash_all = TRUE;
	fg_sound = FALSE;
    fg_pixel_finish = TRUE;
	
	hmi_r_num = 0;
	hmi_t_num = 0;
	hmi_next = 0;
	clk_base = HMI_1;
	hmi_commu_error = 0xFF;
	for (i=0; i<MAX_HMI; i++)
	{
    	fg_disp_back[i] = TRUE;
    	ch_end_f[i] = 0xffff;
    	bmp_end_f[i] = 0xffff;
    	
    	cnt_non_commu[i] = 0xff;
    	cnt_menu_lock[i] = 0;
    	cnt_lock_clue[i] = 0;
    	
    	fg_set_clk[i] = FALSE;
    	fg_active[i] = FALSE;
        hmi_bk_lose_conn[i] = TRUE;
		hmi_board_type[i] = OLD;
    }

	dm603_init();	/* DM603A变量及内容初始化 */
}

/**********************************************
函数：disp_test_f
功能：执行前板测试程序(控制权交给前板)
参数：无
返回值：无
***********************************************/
void disp_test_f(void)
{
	cmd_sub[0] |= CMD_SUB_TEST_F;
	fg_disp_back[hmi_t_num] = FALSE;
}

/**********************************************
函数：disp_set_langue
功能：选择语言
参数：语言
返回值：无
***********************************************/
void disp_set_langue(U08 langue)
{
	cmd_sub[1] |= (langue << CMD_LANGUE_BGN) & CMD_LANGUE_BITMASK;
}

/**********************************************
函数：disp_set_clk
功能：设置时钟(控制权交给前板)
参数：无
返回值：无
***********************************************/
void disp_set_clk(U08 *disp_buf)
{
	U08 buf_index_bak = buf_index;		/* 备份索引 */
	
	if (buf_index > MAX_DISP - MIN_CODE_BYTE)			/* 显示缓存超界 */	
	{
		fg_disp_overflow = TRUE;
		return;
	}
	
	fg_disp_back[hmi_t_num] = FALSE;
	fg_set_clk[hmi_t_num] = TRUE;
	
	buf_index_bak = buf_index;
	buf_index += 2;			/* 预留命令和数据个数的位置 */
    
	/* 移位 确定 加 减 */
    disp_buf[buf_index++] = DM603_KEY_FUNC;
	disp_buf[buf_index++] = DM603_KEY_OK;			/* 行地址 */
	disp_buf[buf_index++] = DM603_KEY_UP;	
	disp_buf[buf_index++] = DM603_KEY_DOWN;			/* 列地址 */
	
	disp_wr_cmd(buf_index_bak, TRANS_TIME_F, disp_buf);		/* 显示版本号命令 */
}

/**********************************************
函数：disp_wr_hmi_clk
功能：直接写前板时钟
		和“disp_set_clk”不同，该函数是直接写前板内存
参数：显示缓存
返回值：无
***********************************************/
BOOL disp_wr_hmi_clk(U08 *disp_buf, RTC clock_src)
{
	U08 buf_index_bak = buf_index;		/* 备份索引 */
	
	if (buf_index > MAX_DISP - MIN_CODE_BYTE)			/* 显示缓存超界 */	
	{
		fg_disp_overflow = TRUE;
		return FALSE;
	}
	
	buf_index_bak = buf_index;
	buf_index += 2;			/* 预留命令和数据个数的位置 */
    
    disp_buf[buf_index++] = clock_src.sec;
	disp_buf[buf_index++] = clock_src.min;			/*  */
	disp_buf[buf_index++] = clock_src.hour;	
	disp_buf[buf_index++] = clock_src.day;			/*  */
	disp_buf[buf_index++] = clock_src.month;		/*  */
	disp_buf[buf_index++] = clock_src.week;			/*  */
//	disp_buf[buf_index++] = clock_src.year;			/*  */
	/* 文本屏RTC的年是U08类型，只有低两位，后板的年为U16，完整的四位 */
	disp_buf[buf_index++] = (clock_src.year>=2000 ? (clock_src.year-2000) : clock_src.year);
	
	disp_wr_cmd(buf_index_bak, TRANS_CLOCK, disp_buf);		/* 写时钟命令 */
	
	return TRUE;
}

/******************************************************
函数：disp_routine
功能：显示总调度。
	决定汉字、字符、图片在何处显示，根据需要决定显示是否刷新。
参数：无
返回值：无
*****************************************************/
U08 disp_routine(U08 *disp_buf)
{
	U08 bnd_chk_result = CD_NORMAL;
	U08 i;
	static U08 fg_disp_back_bak[MAX_HMI];
	hmi_next = hmi_t_num;
	if (++hmi_next >= MAX_HMI)		hmi_next = 0;
		
	for (i=0; i<MAX_CMD_SUB;i++)
	{
		cmd_sub[i] = CMD_SUB_NONE;			/* 子命令初始化 */
	}
	
	bnd_chk_result = disp_bound_code_check();

	buf_index = 0;						/* 显示缓存索引初始化 */


	//if (m_get_error(MAX_PCB, ERR_ID_CODE))		
	//{
		//fg_flash_all = TRUE;
		
		//dm603_face[hmi_t_num] = DM603_FACE_MAC;
		//dm603_face_mac(disp_buf);
	//}
	//else
	//{
	
	disp_screen_power();		/* 是否进入倒计时 */
	
	if (fg_flash_all)						
	{
		cnt_disp_steady[hmi_t_num] = 0;
	}
	else if (cnt_disp_steady[hmi_t_num] > MAX_FLASH_INTERVAL)	
	{
		fg_flash_all = TRUE;    /* 自动刷新标志 */
        fg_is_auto_flash_all = TRUE;
		cnt_disp_steady[hmi_t_num] = 0;
        disp_set_langue(dm603_language);
	}
//	if (cmplt_num >= total_num)	fg_flash_all = TRUE;		/* 调试 */
	
	
	/* 界面控制权转换 */
	if (fg_disp_back_bak[hmi_t_num] != fg_disp_back[hmi_t_num])
	{
		if (fg_disp_back[hmi_t_num])
		{
			fg_flash_all = TRUE;
			disp_clr_screen(disp_buf);
		}
		fg_disp_back_bak[hmi_t_num] = fg_disp_back[hmi_t_num];
	}

#ifdef USE_HMI_DISP
	dm603_online_routine(disp_buf);	/* 上电循环处理 */
	dm603_routine(disp_buf);	/* 循环调用 */
#endif

#ifndef DEBUG	// 调试时不关背光	 
	if (disp_screen_dark(disp_buf))
	{
		disp_lcd_off();
	}
	else 			/* 正常显示 */
#endif
	{
#if 1	/* XM0015 */
    	if (!fg_first_commu && bnd_chk_result != CD_NORMAL	/* 边界错误 */ 
            || (!fg_f_need_update_has_remind && fg_f_need_update)/* 前板需要升级,提醒一次 */
    		|| fg_cur_code_ov || fg_disp_overflow)			/* 编码错误 */
    	{
    		disp_mutual_abnormal(bnd_chk_result, disp_buf);
    	}
    	else if(fg_pixel_finish)    /* 点阵传送完才继续处理界面 */
#endif
    	{
    		disp_switch(disp_buf);
    	}
    	/* 处理点阵传送 */
    	disp_pixel(disp_buf);
    }

    /* 锁定处理(A屏有操作时锁B屏)：有操作或界面控制权在前板 */
	if ((Main_Key[hmi_t_num] != KEY_NULL || !fg_disp_back[hmi_t_num])
		&& cnt_menu_lock[hmi_t_num] == 0)
	{
		if (hmi_next != hmi_t_num)	/* 排除只有一个屏的情况 */
		{
			if (dm603_face[hmi_t_num] == FACE_MAIN || dm603_face[hmi_t_num] == FACE_LOCK)
			{
				cnt_menu_lock[hmi_next] = LOCK_T_IN_MAIN;
			}
			else
			{
				cnt_menu_lock[hmi_next] = LOCK_T_IN_OTHER;
			}
		}
	}
	if (GET_BIT(hmi_commu_error, hmi_t_num))	cnt_menu_lock[hmi_next] = 0;	/* A屏通讯故障，解除对B屏的锁定 */
	
	if (Main_Key[hmi_t_num] != KEY_NULL)	fg_active[hmi_t_num]  = TRUE;	/* 激活 */
    
    disp_deal_snd();			/* 蜂鸣器控制 */
    disp_cmd_simple(disp_buf);	/* 简单命令传送 */
    disp_clk_syn(disp_buf);		/* 时钟同步 */
	//}

	fg_disp_all = FALSE;			/* 指示该界面整屏编码显示已完成 */
	fg_disp_value = FALSE;			/* 指示该界面数值编码显示已完成 */
	fg_disp_blink = FALSE;			/* 指示该界面闪烁内容编码显示已完成 */
	Main_Key[hmi_t_num] = KEY_NULL;	/* 按键复位(防止调用两次) */
	Key[hmi_t_num].value = KEY_NULL;
	
	
	return buf_index;
}



/******************************************************
函数：disp_read_key
功能：读取按键	
参数：无
返回值：无
*****************************************************/
void disp_read_key(void)
{
	U08 flag;		
	static U16 old_key[MAX_HMI];			
	static U16 Key_Tmp[MAX_HMI];		
	static U16 key_now[MAX_HMI];	
	   
	key_now[hmi_r_num] = key_value[hmi_r_num];	
	
	if (old_key[hmi_r_num] != key_now[hmi_r_num] || key_now[hmi_r_num] == KEY_NULL)
	{
		cnt_key_static[hmi_r_num] = 0;
		old_key[hmi_r_num] = key_now[hmi_r_num];
	}

	//连续键
	if (((key_now[hmi_r_num] & (~ATTRIB_ATTRIBUTE_KEY)) == KEY_UP) || \
		((key_now[hmi_r_num] & (~ATTRIB_ATTRIBUTE_KEY)) == KEY_DOWN)) 
	{
		if (cnt_key_static[hmi_r_num] > KEYT_TIME_CNTNUS_CNFRM)	/* 1S时间到，进入连续键 */
		{//	1秒后键
			if (cnt_cntnus_add[hmi_r_num] >= KEYT_TIME_CNTNUS_ADD)	/* 连续键值获取 */ 
			{
				fg_cntnus = TRUE;
				Key_Tmp[hmi_r_num] = KEY_NULL;
				cnt_cntnus_add[hmi_r_num] = 0;

				if (key_continuous_time[hmi_r_num] < 0xfffe) key_continuous_time[hmi_r_num]++;
			}
		}
	}
	else 
	{
		fg_cntnus = FALSE;
		cnt_cntnus_add[hmi_r_num] = 0; 
		key_continuous_time[hmi_r_num] = 0;
	}

	/* 3秒键 */
	flag = 0;
	if (key_now[hmi_r_num] & ATTRIB_LONG_KEY) //判断是否长按键
	{
		if (!fg_long_key)
		{
			flag = 1;
			fg_long_key = TRUE;
		}
	}
	else
	{
		fg_long_key = FALSE;
	}

	//当前按键
	Main_Key[hmi_r_num] = KEY_NULL;		/* 初始化 */
	Main_Key[hmi_r_num] = key_now[hmi_r_num] & (~ATTRIB_ATTRIBUTE_KEY);
	if (flag && (key_now[hmi_r_num] != KEY_NULL))
	{
		if ((key_now[hmi_r_num] & ATTRIB_LONG_KEY) != 0)	
		{
			if (Main_Key[hmi_r_num] == DM603_KEY_ONOFF) 
			{
				Main_Key[hmi_r_num] = DM603_KEY_ONOFF_3S;
				fg_snd_100ms_need = TRUE;
			}
			
			if (Main_Key[hmi_r_num] == DM603_KEY_OK)  
			{
				Main_Key[hmi_r_num] = DM603_KEY_OK_3S;
				fg_snd_100ms_need = TRUE;
			}
			
			if (Main_Key[hmi_r_num] == DM603_KEY_FUNC) 
			{
				Main_Key[hmi_r_num] = DM603_KEY_FUNC_3S;
				fg_snd_100ms_need = TRUE;
			}
			if(Key_Tmp[hmi_r_num] != Main_Key[hmi_r_num]) Key_Tmp[hmi_r_num] = Main_Key[hmi_r_num];
		}
		else
		{
			Key_Tmp[hmi_r_num]	= KEY_NULL;
		}
	}

	//长按键且非有效连续按键则只响应上次有效的长按键
	if(fg_long_key && (Main_Key[hmi_r_num] != KEY_UP) && (Main_Key[hmi_r_num] != KEY_DOWN)) 
	{
		Main_Key[hmi_r_num] = Key_Tmp[hmi_r_num]; 
	}
	
	//连续按键且非有效连续按键则清除按键
	if((key_now[hmi_r_num] & ATTRIB_CNTNUS_KEY) && ((key_now[hmi_r_num] & ATTRIB_LONG_KEY) != ATTRIB_LONG_KEY) && \
	   (Main_Key[hmi_r_num] != KEY_UP) && (Main_Key[hmi_r_num] != KEY_DOWN))
	{
		Main_Key[hmi_r_num] = KEY_NULL;	
	}

	/* 连续键计时处理 */
	if ((key_now[hmi_r_num] & ATTRIB_ATTRIBUTE_KEY) == 0)
	{
		key_continuous_time[hmi_r_num] = 0;
	}

    Key[hmi_r_num].value = Main_Key[hmi_r_num];
    Key[hmi_r_num].time = key_continuous_time[hmi_r_num];     
}


/*------------------------------------------------------------*/
/* 以下为数据层函数，应用层不可调用 */

/******************************************************
函数：disp_code_16
功能：字符串编码写缓存。
	决定使用编码还是点阵显示。
	如果一次通讯显示不全，需要记忆上次状态，继续显示，直到显示完整为止。
参数：字符串起始地址(字节); 行; 列; 属性; 缓冲区地址(写)
返回值：无
要求：只允许传入标准码(8*16-ASCII字符和16*16汉字)，自定义图片用“disp_bmp”显示
	  如果以后要支持其他尺寸的单元，则需要更改。
*****************************************************/
static void disp_code_16(PTR_FAR_C08 string_ptr, U16 row, U16 col, U16 attrib, U08 *disp_buf)
{
	U08 font_long, font_high;
	U16 max_high = 0;			/* 最高的单元高度 */	
	PTR_FAR_C08 temp_ptr;
	U16 temp_code;
	U16 temp_row, temp_col;
	U08 buf_index_bak = buf_index;		/* 备份索引 */
	U08 col_cnt;					/* 字符串占的列数(字节单位) */
	U08 col_end;					/* 字符串结束列值(字节单位) */
	
	temp_ptr = string_ptr;
	temp_row = row;
	temp_col = col;
	
	/* 计算字符串中所有单元宽度之和(8个像素点单位) */
    col_cnt = 0; 	
    while(*temp_ptr != '\0')		
	{
		temp_code = *temp_ptr++;
		if (temp_code > ADDR_ASC_END)			/* 非西文字符，占两个字节 */
		{
			temp_code = MAKEWORD(temp_code, *temp_ptr++);
		}
	
		disp_cal_dim(temp_code, &font_long, &font_high);
		
		col_cnt += font_long;
	}
    
    /* 居中及靠右(居中的颗粒度以8个点为单位) */
    if ((attrib & DTS_CENTER) != 0)		/* 居中 */
    {
    	if (col_cnt < LCM_COLUMN_NUM)	temp_col = (LCM_COLUMN_NUM - col_cnt)/2;
    	else 							temp_col = 0;
    		
    	temp_col *= 8;		/* 转换成点为单位 */
    }
    else if ((attrib & DTS_RIGHT) != 0)	/* 靠右 */
    {
    	col_end = col/8;
    	if (col_cnt < col_end)			temp_col = (col_end - col_cnt);
    	else 							temp_col = 0;
    		
    	temp_col *= 8;		/* 转换成点为单位 */
	}
	
	if (buf_index < MAX_DISP - MIN_CODE_BYTE)
	{
    	buf_index += 2;			/* 预留命令和数据个数的位置 */
    	
    	/* 写地址和属性(每个字符串只给一个地址和属性) */
    	disp_buf[buf_index++] = HIGH_BYTE(temp_row);
    	disp_buf[buf_index++] = LOW_BYTE(temp_row);	/* 行地址 */
    	disp_buf[buf_index++] = HIGH_BYTE(temp_col);	
    	disp_buf[buf_index++] = LOW_BYTE(temp_col);	/* 列地址 */
    	disp_buf[buf_index++] = HIGH_BYTE(attrib);		/* 属性 */
    }
		
	/* 编码写入缓冲区 */
	temp_ptr = string_ptr;
	while(*temp_ptr != '\0')
	{
		if (total_num++ > MAX_UNIT_NUM)			    	break;		/* 单元个数溢出，异常 */
		if (buf_index > MAX_DISP - MIN_CODE_BYTE)		break;		/* 显示缓存超界 */
//      点阵不够，后续继续传编码	
//      if (spe_index > MAX_PIX_CODE - MIN_CODE_WORD)   break;      /* 点阵单元记忆区超界 */
		
		temp_code = *temp_ptr++;
		if (temp_code > ADDR_ASC_END)			/* 非西文字符，占两个字节 */
		{
			temp_code = MAKEWORD(temp_code, *temp_ptr++);
		}
		disp_cur_code_check(temp_code);
		if (disp_is_bmp(temp_code))				continue;	/* 自定义图片，不处理 */
					
		if (disp_is_str_in_font(temp_code))		/* 编码在字库中 */
		{
			cmplt_num++;
		}
        else if (spe_index > MAX_PIX_CODE - MIN_CODE_WORD)/* 点阵不够,继续传编码 */
        {
            /* 没有的汉字过多,前板需要升级 */
            fg_f_need_update = TRUE;
            cmplt_num++;
        }
		else 	/* 不在字库中，则记忆编码 */
		{
			pixel_code[spe_index++] = temp_row;
			pixel_code[spe_index++] = temp_col;
			pixel_code[spe_index++] = attrib;
			pixel_code[spe_index++] = temp_code;	
            /* 自动刷新时,传0xFFFF,解决分帧传点阵时会闪烁的问题 */
            if(fg_is_auto_flash_all)    temp_code = 0xFFFF;
            else            temp_code = MAKEWORD(' ', ' '); /* 传点阵时,先传空白 */
		}		
		
		if (temp_code > ADDR_ASC_END)	
		{
			disp_buf[buf_index++] = HIGH_BYTE(temp_code);
		}
		disp_buf[buf_index++] = LOW_BYTE(temp_code);
		
		/* 通过单元高度和宽度计算是否换行，并为点阵单元记录地址 */
		disp_cal_dim(temp_code, &font_long, &font_high);
    	max_high = max_high > font_high ? max_high : font_high;
    	
    	temp_col += (U16)font_long*8;
    	if (temp_col >= MAX_COL)		/* 超过一列显示范围，换行 */
    	{
    		temp_row += max_high*8;
    		temp_col = 0;
    	}
	}
	
	disp_wr_cmd(buf_index_bak, TRANS_CODE, disp_buf);		/* 传送编码命令 */
	
	if (*temp_ptr != '\0') 			/* 该字符串未结束 */
	{
		fg_disp_overflow = TRUE;
	}
}

/******************************************************
函数：disp_pixel
功能：显示点阵。
参数：数据输出地址
返回值：当前单元取出完成/未完成
*****************************************************/
static void disp_pixel(U08 *disp_buf)
{
	U16 temp_row, temp_col, temp_attrib;
	U08 buf_index_bak = buf_index;		/* 备份索引 */
//	U08 data_num = 0;			/* 当前显示单元数据个数 */
	U08 temp_cmd = TRANS_NONE;

	if (
//		disp_is_code_cmplt()				/* 编码已传完 */
//		&& 
		cmplt_num < total_num)			/* 需传送点阵 */
	{
        fg_pixel_finish = FALSE;        /* 点阵未传送完 */
        
		while(cmplt_num < total_num)
		{
			if (buf_index > MAX_DISP - MIN_PIXEL_BYTE)		break;		/* 显示缓存超界 */

            /* 超出最大点阵范围，认为传送完成 */
			if (pixel_index >= MAX_PIX_CODE)                
			{
                cmplt_num = total_num;
                break;
			}
			
			buf_index_bak = buf_index;
			buf_index += 2;			/* 预留命令和数据个数的位置 */
			
			if (pixel_addr_ofst == 0)		/* 前一个单元已完成才填写行、列 */
			{
				temp_row = pixel_code[pixel_index++];
				temp_col = pixel_code[pixel_index++];		/* 取出地址 */
				temp_attrib = pixel_code[pixel_index++];	/* 取出属性 */
				
				disp_buf[buf_index++] = HIGH_BYTE(temp_row);
				disp_buf[buf_index++] = LOW_BYTE(temp_row);
				disp_buf[buf_index++] = HIGH_BYTE(temp_col);
				disp_buf[buf_index++] = LOW_BYTE(temp_col);
				disp_buf[buf_index++] = HIGH_BYTE(temp_attrib);
				
				temp_cmd = TRANS_PIXEL;
			}
			else
			{
				temp_cmd = TRANS_PIXEL_ONLY;
			}
			disp_get_pixel(pixel_code[pixel_index], &pixel_addr_ofst, disp_buf);
			
			disp_wr_cmd(buf_index_bak, temp_cmd, disp_buf);	/* 传送点阵命令 */
			
			if (pixel_addr_ofst == 0)
			{
				pixel_index++;
				cmplt_num++;
			} 
			else 
			{
				break;		/* 缓冲区已满，退出，余下的下次传送 */
			}
		}
	}

    /* 点阵传送完成 */
    if(cmplt_num >= total_num)
    {
        fg_pixel_finish = TRUE;
        spe_index = 0;
    }
}

/******************************************************
函数：disp_get_pixel
功能：取点阵。
	包含：宽度；高度；数据；分隔符
	暂不理会行的变化
参数：缓冲区索引：判断缓冲区是否写满; 字符编码; 
	  当前单元的偏移地址(该项为输出参数)
返回值：当前单元取出完成/未完成
*****************************************************/
static void disp_get_pixel(U16 font_code, U16 *addr_ofst, U08 *disp_buf)
{
//	U16 font_addr;
	PTR_FAR_C08 buf;
	U08 i;
	U08 k = 0;
	U08 font_long, font_high;		/* 单元的宽度和高度(字节为单位) */
	static U16 font_byte_num;		/* 单元字节数 */
//	BOOL fg_cmplt = TRUE;			/* 当前单元完成标志 */
	
	if (fg_cur_code_ov)			/* 当前编码超界,不显示 */
	{
		*addr_ofst = 0;
		return;
	}
	if (disp_is_bmp(font_code))		/* 图片 */
	{
		buf = bmp_list[font_code - BMP_BGN_B] + *addr_ofst;
		buf += 2;		/* 宽度和高度所占位置 */
	}
	else							/* 汉字 */
	{
		buf = hz16 + (font_code - CH_BGN_B)*32 + *addr_ofst;
	}
	
	if (*addr_ofst == 0)		/* 上个单元已完成 */
	{
		disp_cal_dim(font_code, &font_long, &font_high);
		font_byte_num = (U16)font_long * font_high * 8;
		
		disp_buf[buf_index++] = font_long;	/* 填写宽度和高度 */
    	disp_buf[buf_index++] = font_high;
	}
	
	while(*addr_ofst + k < font_byte_num)		/* 该单元未结束 */
	{
    	if (buf_index < MAX_DISP - 8)		/* 每次至少写8个字节 */
    	{   
    		for (i=0; i<8; i++) 		
    		{
    			disp_buf[buf_index++] = buf[k++];
    		}
    	}
    	else
    	{
    		break;
    	}
    }
    if (*addr_ofst + k >= font_byte_num)
    {
    	*addr_ofst = 0;				/* 该单元完成，偏移记录清零 */
    }
    else
    {
    	*addr_ofst += k;
    }
}

/******************************************************
函数：disp_get_num
功能：计算各位数字。
	结果为ASCII码或图片编号。
参数：数据; 显示个数(位数); 进制; 缓存地址
返回值：有效数据个数
*****************************************************/
static void disp_get_num(U16 v, U08 num, U08 *buf)
{
	U08  i;
	U16 temp_value;		/* 临时数据 */
//	U08 vld_num = 0;
	
	temp_value = v;
	for (i=0; i<num; i++) 
	{
		buf[num-i-1] = temp_value % 10;
		temp_value /= 10;
	}
}

/******************************************************
函数：disp_value
功能：显示数据小数点处理
参数：数值; 有效数据位数; 属性 
返回值：显示数据位数
*****************************************************/
static U08 disp_value_dot(U08 *buf_src, U08 v_num, U08 *max_num, U16 attrib)
{
	U08 i;
	U08 dot_pos;		/* 小数点位置 */
	U08 dest_num;
	U08 temp_buf[MAX_VALUE_NUM];
	U08 zero_num = 0;	/* 无效0的个数 */
		
	dot_pos = ((attrib & DNS_FLOAT_MASK) >> DNS_FLOAT_BIN_NUM);	/* 小数点位置 */
	dest_num = v_num;		/* 显示位数 */
	for (i=0; i<dest_num; i++)
	{
		temp_buf[i] = buf_src[i];	/* 初始化 */
	}
	
	if (dot_pos >= v_num)	dot_pos = v_num-1;	/* 界限判断 */

	if (dot_pos > 0)	/* 小数显示 */
	{
		for (i=0; i<dot_pos; i++)
		{
			temp_buf[v_num-i] = temp_buf[v_num-i-1];
		}
		temp_buf[v_num - dot_pos] = '.';		/* 增加小数点 */
		
		for (i=0; i<v_num-dot_pos-1; i++)	/* 去掉最高有效位前多余的0 */
		{
			if (temp_buf[i] == 0)	zero_num++;
			else					break;
		}
				
		dest_num++;
		(*max_num)++;
	}
	else if ((attrib & DNS_ZERO) == 0)	/* 整数显示，且不显示前导0 */
	{
		for (i=0; i<v_num-1; i++)	/* 清空前导0(最后一个0要保留) */
		{
			if (temp_buf[i] == 0)	zero_num++;
			else					break;
		}
	}
	
	dest_num -= zero_num;
	for (i=0; i<dest_num; i++)
	{
		buf_src[zero_num+i] = temp_buf[zero_num+i];
	}
	
	return dest_num;
}

/******************************************************
函数：disp_value_neg
功能：显示负号
参数：数值; 有效数据位数; 最大位数; 属性 
返回值：无
*****************************************************/
static void disp_value_neg(U08 *buf_src, U08 *vld_num, U08 *max_num, BOOL fg_neg)
{
	U08 i;
		
	if (fg_neg)		/* 需要显示负号 */
	{
		if (*max_num > MAX_VALUE_NUM-1)		*max_num = MAX_VALUE_NUM-1;
			
		for (i=0; i<*vld_num; i++)
		{
			buf_src[*max_num-i] = buf_src[*max_num-i-1];
		}
		buf_src[*max_num - *vld_num] = '-';		/* 添加负号 */
		
		(*vld_num)++;
		(*max_num)++;
	}
}

/******************************************************
函数：disp_cal_dim
功能：计算单元宽度和高度。
参数：编码; 宽度指针; 高度指针; 字库指针
返回值：无
*****************************************************/
static void disp_cal_dim(U16 font_code, U08 *ch_long, U08 *high)
{
	PTR_FAR_C08 buf;
	
	if (font_code < ADDR_ASC_END)
	{
		*ch_long = AC_LONG;
		*high = AC_HIGH;
	}
	else if (disp_is_bmp(font_code))	/* 图片 */
	{
		if (font_code >= bmp_end_f[hmi_t_num])
		{
    		buf = bmp_list[font_code - BMP_BGN_B];
    		*ch_long = buf[0] >> 3;
    		*high = buf[1] >> 3;
    	}
    	else 		/* 在字库内，用汉字尺寸代替(此时实际上不使用，只为防止取到无效数据) */
    	{
    		*ch_long = HZ_LONG;
			*high = HZ_HIGH;
 		}
	}
	else 								/* 汉字 */
	{
		*ch_long = HZ_LONG;
		*high = HZ_HIGH;
	}
}

/******************************************************
函数：disp_num_code
功能：数字转换为编码。
参数：输入数据(数字)地址; 显示个数(位数); 输出数据(编码)地址
返回值：无
*****************************************************/
static void disp_num_code(U08 *buf_src, U08 num, U08 *buf_dest)
{
	U08  i;
	U08  TblHex[] = "0123456789abcdef";
	U08 temp_num;				/* 临时个数 */
	
	temp_num = num;
	if (temp_num > MAX_VALUE_NUM-1)		temp_num = MAX_VALUE_NUM-1;
	
	for (i=0; i<temp_num; i++)
	{
		if (buf_src[i] < 16)	buf_dest[i] = TblHex[buf_src[i]];
		else 					buf_dest[i] = buf_src[i];	/* 非数字，保留原值 */
	}
	
	buf_dest[temp_num] = '\0';
}

/******************************************************
函数：disp_mutual_abnormal
功能：匹配异常处理。
参数：无
返回值：无
*****************************************************/
static void disp_mutual_abnormal(U08 bnd_chk, U08 *disp_buf)
{
#ifdef USE_HMI_DISP
	if (bnd_chk == CD_ABN_BACK)
	{
		disp_string(bound_err_b, 0, 0, DTS_CLR_HEAD|DTS_CLR_TRAIL, disp_buf);
	}
	else if (bnd_chk == CD_ABN_MUTUAL)
	{
		disp_string(update_need_f, 0, 0, DTS_CLR_HEAD|DTS_CLR_TRAIL, disp_buf);
	}
    else if(fg_f_need_update)   /* 提醒前板需要升级 */
    {
        disp_string(update_need_f, 0, 0, DTS_CLR_HEAD|DTS_CLR_TRAIL, disp_buf);
    }
	
	if (fg_cur_code_ov)
	{
		disp_string(code_ov, 16, 0, DTS_CLR_HEAD|DTS_CLR_TRAIL, disp_buf);
	}
	if (fg_disp_overflow)
	{
		//fg_flash_all = TRUE;		/* 调试 */
		disp_string(buffer_ov, 32, 0, DTS_CLR_HEAD|DTS_CLR_TRAIL, disp_buf);
	}
#endif	
	if (Main_Key[hmi_t_num] == KEY_STOP)		/* 退出 */
	{
		fg_first_commu = TRUE;
		fg_cur_code_ov = FALSE;
		fg_disp_overflow = FALSE;
		fg_flash_all = TRUE;
        if (fg_f_need_update)
        {
            fg_f_need_update_has_remind = TRUE;/* 仅提醒一次 */
        }
	}
}
/**********************************************
函数：disp_forbid_auto_lcd_on
功能：禁止自动开背光
参数：无
返回值：无
***********************************************/
static void disp_forbid_auto_lcd_on(void)
{
	cmd_sub[0] |= CMD_SUB_FORBID_AUTO_LCD_ON;
}
/******************************************************
函数：disp_led_off
功能：屏幕保护。一段时间无按键操作时关背光
参数：无
返回值：是否进入屏保
*****************************************************/
static BOOL disp_screen_dark(U08 *disp_buf)
{
	BOOL fg_screen_dark = FALSE;

	if (fg_disp_ver_back[hmi_t_num])	disp_forbid_auto_lcd_on();
	
	if (disp_is_code_cmplt()				/* 编码已传完 */
		&& cmplt_num >= total_num)			/* 点阵已传完 */
	{
		if (fg_disp_ver_back[hmi_t_num])
		{
			if (cnt_no_operate_sec[hmi_t_num] >= MAX_OPER_WAIT)		disp_lcd_off();
			else 												fg_screen_dark = FALSE;
		}
		else
		{
			if (cnt_no_operate_sec[hmi_t_num] >= MAX_OPER_WAIT)		fg_screen_dark = TRUE;
			else 												fg_screen_dark = FALSE;
		}											fg_screen_dark = FALSE;
	}
	
	if (Main_Key[hmi_t_num] != KEY_NULL)	
	{
		if (cnt_no_operate_sec[hmi_t_num] >= MAX_OPER_WAIT)		
		{
			fg_flash_all = TRUE;
			disp_clr_screen(disp_buf);
			Main_Key[hmi_t_num] = KEY_NULL;
            Key[hmi_t_num].value = KEY_NULL;
		}
		cnt_no_operate_sec[hmi_t_num] = 0;
		disp_lcd_on();	/* 有任何按键就开背光 */
	}
#ifdef USE_NOT_LOCK
    cnt_no_operate_sec[hmi_t_num] = 0;
	disp_lcd_on();
#endif  
	
	return fg_screen_dark;
}

/******************************************************
函数：disp_is_value_need_flash
功能：判断数值是否需要刷新
参数：无
返回值：是/否
*****************************************************/
static BOOL disp_is_value_need_flash(U16 attrib)
{
	BOOL fg_need = FALSE;
	
	if (fg_disp_all 							/* 全局刷新 */
		|| fg_disp_blink && (attrib & DCS_BLINK)/* 闪烁 */
		|| fg_disp_value)						/* 数值刷新 */
	{
		fg_need = TRUE;
	}
	
	return fg_need;
}
/******************************************************
函数：disp_is_str_in_font
功能：判断编码是否在前板字模中
参数：字符编码
返回值：是/否
*****************************************************/
static BOOL disp_is_str_in_font(U16 ch_code)
{
	BOOL fg_in_font = FALSE;
	
    if (disp_is_bmp(ch_code))			/* 图片 */
    {
    	if (ch_code < bmp_end_f[hmi_t_num])	fg_in_font = TRUE;
    }
    else								/* 汉字 */
    {
    	/* 在前板,传编码 */
    	if (ch_code < ch_end_f[hmi_t_num])		fg_in_font = TRUE;
        /* 否则,在后板,传点阵 */
        else if(ch_code >= CH_BGN_B && ch_code < CH_END_B)  fg_in_font = FALSE;
        /* 否则,在字库范围内,传编码,前板将显示特殊字符 */
        else if(ch_code < CH_END)               fg_in_font = TRUE;
    }
	
	return fg_in_font;
}

/******************************************************************** 
  函 数 名: void disp_blank16(U08 row, U08 column, U08 num, U08 attrib)
  
  功  能: 显示空白字符	
  入口参数: row:行信息  column:列信息  num:空白字个数 attrib:字符属性
  返 回 值:无
 ***********************************************************************/
void disp_blank16(U16 row, U16 column, U16 num, U16 attrib,U08 *disp_buf)
 {
	 U16 i;
 
	 for(i=0;i<num;i++)
	  {
	 // disp_string(msg_oneblank,row,ENLARGE_LOW*(column++),attrib,disp_buf);
	 // disp_string_ptr8(" ",row,ENLARGE_LOW*(column++),attrib,disp_buf);
	  disp_string_ptr8(" ",row,ENLARGE_LOW*column,attrib,disp_buf);
	  column++;
	  }
	//	 disp_string(msg_oneblank,row,ENLARGE_LOW*(column++),attrib,disp_buf);
	 return;
 }

/******************************************************************** 
函 数 名: void disp_index_list(U08 index_now, U08 index_max, U16 const *string_ptr, U08 attrib)
功	能:显示可上翻页、下翻页图标
入口参数: index_now:当前应反白显示目录  index_max:最大显示索引
index_min:最小显示索引attrib:属性 0:每个参数显示两行 1:每个参数显示一行
返 回 值:无
***********************************************************************/
void disp_next_flg(U16 index_now, U16 index_max,U16 index_min,U16 attrib,U08 *disp_buf)
{
 U16 per_screen_msg;
 U16 fgup=0;  //显示向上图标
 U16 fgdown=0; //显示向下图标
 if(attrib>SCREEN_4)
 	per_screen_msg=SCREEN_4;
 else
   per_screen_msg=attrib;
 //图标显示判断
 if(per_screen_msg==SCREEN_1)  //一屏幕只显示一个参数
 	{
 	if(index_max==index_min)
		{
		  fgup=FALSE;
		  fgdown=FALSE;
	  }
 	 else if(index_now==index_min)
 	  	{
 	  	 fgup=FALSE;
		  fgdown=TRUE;
	   }
	  else if(index_now==index_max)
	  	{
		  fgup=TRUE;
		  fgdown=FALSE;
	   }
	  else
	  	{
		  fgup=TRUE;
		  fgdown=TRUE;
	  }
  }
 else           //一屏幕显示多个参数
  {
   if((index_now<per_screen_msg)&&(index_max>per_screen_msg))
   {
     fgup=FALSE;
     fgdown=TRUE;
   }
  else if(index_now>=per_screen_msg)	 
	{
	 fgup=TRUE;
	   if((index_now/per_screen_msg)!=((index_max-1)/per_screen_msg))  //不是最后一屏
	     fgdown=TRUE;
	   else
	    fgdown=FALSE;
    }
  else
	 {
	 fgup=FALSE;
	 fgdown=FALSE;
   }
 }
 //图标显示
 if(fgup)
   disp_bmp(idx_bmp_up_8_16,ROW1,15*ENLARGE_LOW, DCS_NORMAL,disp_buf);	//显示向上
 else
   disp_blank16(ROW1,15,1,DCS_NORMAL,disp_buf);	 //清除向上标示符
 if(fgdown)
   disp_bmp( idx_bmp_down_8_16,ROW3+(per_screen_msg/SCREEN_4)*ENLARGE_ROW, 15*ENLARGE_LOW, DCS_NORMAL,disp_buf)  ;   //显示向下
 else	
   disp_string_ptr8(" ",ROW3+per_screen_msg/SCREEN_4,15*ENLARGE_LOW,DCS_NORMAL,disp_buf);  //清除向下表示符
}

/******************************************************************** 
函 数 名: void disp_index_list(U08 index_now, U08 index_max, U16 const *string_ptr, U08 attrib)
功	能:显示目录索引
入口参数: index_now:当前显示目录起始位置 max:最大显示索引 *string_ptr:指向目录索引的指针  attrib:属性每屏最多显示多少行
返 回 值:无
***********************************************************************/
//void disp_index_list(U16 index_now, U16 index_max, const U08 *string_ptr[], U16 attrib, U08 *disp_buf)
//{
//	U16 i,attribute;
//	static U08 dis_index_re=0;
//	static U08 dis_index=0;
//	dis_index=index_now%attrib;  //方便计算显示起始位置
//	for(i=0;i<attrib;i++)	//显示信息
//	 { 
//	   	if(i==dis_index)
//		 	attribute=DCS_REVERSE;
//	   	else
//		 	attribute=DCS_NORMAL;
//	   	dis_index_re=index_now-dis_index+i;
		
//	   	if(dis_index_re<index_max)
//	   	{
//		 	disp_test_string(string_ptr[dis_index_re],i*ENLARGE_ROW,0,attribute|DTS_CLR_TRAIL,disp_buf);  
//	   	}
//	   	else
//	   	{
//			disp_test_string(c_dm603_nothing,i*ENLARGE_ROW,0,attribute|DTS_CLR_TRAIL,disp_buf);  
//	   	}
//	}
// disp_next_flg(index_now-dis_index, index_max,0,attrib,disp_buf);//显示翻页标识符
//}

/******************************************************
函数：disp_record_init
功能：显示记录初始化
参数：属性(闪烁时需要一直传送)
返回值：无
*****************************************************/
static void disp_record_init(void)
{
	BOOL fg_flash_need_first = FALSE;
	
//	if (fg_disp_other)		return;		/* 特殊命令传送时不送显示数据 */
	
	if (fg_flash_all)		/* 全部刷新 */
	{
		if (!fg_disp_all)			fg_flash_need_first = TRUE;
		
		fg_disp_all = TRUE;
		fg_flash_all = FALSE;
	}
	else if (!fg_disp_all && cmplt_num >= total_num)	/* 全部刷新已完成 */
	{
		if (!fg_disp_value)				/* 数值更新(一直刷新) */
		{	
    		fg_disp_value = TRUE;
    		if (!fg_disp_blink)		fg_flash_need_first = TRUE;	
    	}
    	
		if (!fg_disp_blink)				/* 闪烁 */
    	{
    		fg_disp_blink = TRUE;
    		if (!fg_disp_value)		fg_flash_need_first = TRUE;
    	}
	}
	
	if (fg_flash_need_first)		/* 需要刷新，且为第一次调用，用来初始化记录 */
	{
		spe_index = 0;
		total_num = 0;
		cmplt_num = 0;
		pixel_index = 0;
		pixel_addr_ofst = 0;
	}
}

/******************************************************
函数：disp_is_code_cmplt
功能：判断编码是否显示完成
参数：无
返回值：是/否
*****************************************************/
static BOOL disp_is_code_cmplt(void)
{
	BOOL fg_cmplt = TRUE;
	
	if (fg_disp_all || fg_disp_value || fg_disp_blink)
	{
		fg_cmplt = FALSE;
	}
	
	return fg_cmplt;
}

/******************************************************
函数：disp_bound_code_check
功能：边界编码检查
参数：无
返回值：正常或异常值
*****************************************************/
static U08 disp_bound_code_check(void)
{
	U08 result = CD_NORMAL;
	
	/* 注意，有优先级：后板自身无问题时才判断前板 */
	if (CH_END_B < CH_BGN_B || BMP_END_B < BMP_BGN_B)		/* 后板自身比较 */
	{
		
		result = CD_ABN_BACK;
	}
	/* 后板保存字库,没有的字用特殊符号代替后,不需要检查 */
//  else if (ch_end_f[hmi_t_num] < CH_BGN_B || bmp_end_f[hmi_t_num] < BMP_BGN_B)    /* 前板结束编码和后板起始编码比较 */
//  {
//      result = CD_ABN_MUTUAL;
//  }	
	return result;
}

/******************************************************
函数：disp_cur_code_check
功能：当前编码有效性检查
参数：无
返回值：正常或异常值
*****************************************************/
static void disp_cur_code_check(U16 font_code)
{
	if (disp_is_bmp(font_code)) 
    {
    	if (font_code >= BMP_END_B && font_code >= bmp_end_f[hmi_t_num])	/* 当前图片编码超界判断 */
    	{
    		fg_cur_code_ov = TRUE;
    	}
    }	
    else
	{
	    /* 编码在字库范围内，就认为有效，否则提示编码超界 */
        if (font_code >= CH_END)
        {
            fg_cur_code_ov = TRUE;
        }
//      if ((font_code >= ch_end_f[hmi_t_num] && font_code < CH_BGN_B)      /* 汉字介于前板字库和后板字库之间,后超出范围 */
//          || font_code >= MAX(CH_END_B, ch_end_f[hmi_t_num]))
            
//    	if (font_code >= CH_END_B && font_code >= ch_end_f[hmi_t_num])		/* 当前汉字编码超界判断 */
//  	{
//  		fg_cur_code_ov = TRUE;
//  	}
    }
}

/******************************************************
函数：disp_wr_cmd
功能：写命令。	
参数：数据个数; 命令; 显示缓存
返回值：无
*****************************************************/
static void disp_wr_cmd(U08 index_bak, U08 trans_cmd, U08 *disp_buf)
{
	if (buf_index - index_bak > 2)	/* 存在有效数据 */
	{ 
		disp_buf[index_bak] = trans_cmd;
		disp_buf[index_bak+1] = buf_index - index_bak - 2;	/* 有效数据个数 */
	}
	else 
	{
		buf_index = index_bak;		/* 无有效数据时恢复buf_index的值 */
	}
}

/**********************************************
函数：disp_clr_snd
功能：蜂鸣器处理
参数：无
返回值：无
***********************************************/
static void disp_deal_snd(void)
{
    if (fg_sound)	cmd_sub[0] |= CMD_SUB_SND_ON;
    else			cmd_sub[0] |= CMD_SUB_SND_OFF;

	if (fg_snd_100ms_need) 
	{
		 //debug_7[1]++;
		 cmd_sub[0] |= CMD_SUB_SND_100MS;
		 fg_snd_100ms_need = FALSE;
	}
}

/**********************************************
函数：disp_lcd_off
功能：关背光
参数：无
返回值：无
***********************************************/
static void disp_lcd_off(void)
{
	cmd_sub[0] |= CMD_SUB_LCD_OFF;
}

/**********************************************
函数：disp_lcd_on
功能：开背光
参数：无
返回值：无
***********************************************/
static void disp_lcd_on(void)
{
	cmd_sub[0] |= CMD_SUB_LCD_ON;
}

/******************************************************
函数：disp_screen_power
功能：显示屏倒计时判断。
		持续5秒无通讯时，显示屏重新倒计时。	
参数：无
返回值：无
*****************************************************/
static void disp_screen_power(void)
{
	if (cnt_non_commu[hmi_t_num] > 2)			cnt_screen_power[hmi_t_num] = SCREEN_POWER_TIME;
	if (!GET_BIT(hmi_commu_error, hmi_t_num))	cnt_non_commu[hmi_t_num] = 0;	/* 无通讯故障才清零 */
}

/**********************************************
函数：disp_cmd_simple
功能：简单命令(指：无显示数据；无行列等信息的命令)
参数：无
返回值：无
***********************************************/
static void disp_cmd_simple(U08 *disp_buf)
{
	U08 i;
	U08 data_num = 0;
	U08 buf_index_bak = buf_index + 2;	/* 预留命令和数据个数的位置 */
	BOOL fg_have_sub_cmd = FALSE;
	
	for (i=0; i<MAX_CMD_SUB; i++)
	{
        if (cmd_sub[i] != CMD_SUB_NONE)	
        {
    		fg_have_sub_cmd = TRUE;
    		break;
        }
    }
    
    if (fg_have_sub_cmd)
    {
    	for (i=0; i<MAX_CMD_SUB; i++)
    	{
    		disp_buf[buf_index_bak++] = cmd_sub[i];
    		data_num++;
    	}
    	disp_buf[buf_index++] = TRANS_SIMPLE;
    	disp_buf[buf_index++] = data_num;
    	buf_index += data_num;
    }
}

/**********************************************
函数：disp_is_bmp
功能：判断当前编码是否是自定义图片
参数：编码
返回值：TRUE:是 / FALSE:否
***********************************************/
static BOOL disp_is_bmp(U16 font_code)
{
	BOOL fg_bmp = FALSE;
	
	if (font_code >= ADDR_BMP_BGN && font_code < ADDR_BMP_END)
	{
		fg_bmp = TRUE;
	}
	
	return fg_bmp;
}

/**********************************************
函数：disp_value_cpmt_blank
功能：数字显示时补充空格
参数：缓冲区地址; 起始序号；结束序号
返回值：无
***********************************************/
static void disp_value_cpmt_blank(U08 *buf_src, U08 vld_num, U08 max_num, U16 attrib)
{
	U08 i;
	U08 blank_bgn, blank_end;	/* 补空格的起始、结束位置 */
	U08 blank_num = 0;			/* 空格个数 */
	
	if (vld_num > max_num)		return;
	blank_num = max_num - vld_num;
	
	if (attrib & DTS_RIGHT)		/* 靠右显示，左边补空格 */
    {
    	blank_bgn = 0;
    	blank_end = blank_num;
	}
	else 
	{
		blank_bgn = vld_num;
    	blank_end = max_num;
    	
    	for (i=0; i<vld_num; i++)
    	{
    		buf_src[i] = buf_src[blank_num + i];	/* 数字搬运到最左边 */
    	}
	}
	
	for (i=blank_bgn; i<blank_end; i++)
	{
		buf_src[i] = ' ';		/* 补空格 */
	}
}

/******************************************************
函数：disp_is_text_need_flash
功能：判断文本是否需要刷新
参数：无
返回值：是/否
*****************************************************/
static BOOL disp_is_text_need_flash(U16 attrib)
{
	BOOL fg_need = FALSE;
	
	if (fg_disp_all 											/* 全局刷新 */
		|| fg_disp_blink && (attrib & (DCS_BLINK|DCS_FLASH)))	/* 闪烁、一直刷新 */
	{
		fg_need = TRUE;
	}
	
	return fg_need;
}

/******************************************************
函数：disp_clk_syn
功能：时钟同步
参数：无
返回值：无
*****************************************************/
static void disp_clk_syn(U08 *disp_buf)
{
	BOOL fg_need = FALSE;
	U08 *buf_0, *buf_1;
	U08 i;
	
	buf_0 = (U08 *)&clock[HMI_1];
	buf_1 = (U08 *)&clock[HMI_2];
	
	if (hmi_commu_error == 0 && MAX_HMI > 1)		/* 无通讯故障才做时钟同步 */
	{
		/* 判断是否偏差超过3分钟 */
    	for (i=MAX_RTC_ITEM-1; i>=RTC_HOUR; i--)
    	{
    		if (i == RTC_WEEK)		continue;	/* 星期不作为判断依据 */
    			
    		if (buf_0[i] != buf_1[i])
    		{
    			fg_need = TRUE;
    			if (buf_0[i] < buf_1[i])	clk_base = HMI_2;	/* 快的作为基准时钟 */
    			break;
    		}
    	}
    	if (!fg_need)		/* 小时以上的无偏差，才判断分钟 */
    	{
        	if (buf_0[RTC_MIN] > buf_1[RTC_MIN] && buf_0[RTC_MIN] - buf_1[RTC_MIN] > 3)
        	{
        		fg_need = TRUE;
        		clk_base = HMI_1;	/* 快的作为基准时钟 */
        	}
        	else if (buf_1[RTC_MIN] > buf_0[RTC_MIN] && buf_1[RTC_MIN] - buf_0[RTC_MIN] > 3)
        	{
        		fg_need = TRUE;
        		clk_base = HMI_2;	/* 快的作为基准时钟 */
        	}
        }
    
        /* 判断是否只有一个屏被激活(有操作) */
        if (fg_need)
        {
            if (fg_active[HMI_1] && !fg_active[HMI_2]		/* HMI_1激活 */
    			|| !fg_active[HMI_1] && fg_active[HMI_2])	/* HMI_2激活 */
        	{
//        		debug_7[0]++;
        		if (fg_active[HMI_2])	clk_base = HMI_2;	/* 激活的作为基准时钟 */
        	}
        }
    	
    	if (fg_disp_back[hmi_t_num] && fg_disp_back[hmi_next])	/* 界面控制权都在后板 */
    	{
        	if (fg_set_clk[hmi_next]				/* 相邻界面有设置时钟 */
        		|| power_time  > 60 && fg_need)	/* 偏差超过3分钟且已上电1分钟 *///cx
        	{
//        		debug_7[1]++;
        		if (fg_set_clk[hmi_next])	clk_base = hmi_next;
        		if (disp_wr_hmi_clk(disp_buf, clock[clk_base]))
        		{
        			fg_set_clk[hmi_next] = FALSE;
        		}
        	}
        }
    }
}

/*****************************************************************************
函  数 : disp_trans_sync()
功  能 : 传送同步信息
参  数 : U08 *disp_buf : 
返  回 : 无
其  他 : 无
*****************************************************************************/
void disp_trans_sync(U08 *disp_buf)
{
    U08 remain_length = MAX_DISP - buf_index;
    U08* buf = &disp_buf[buf_index];

    if (remain_length >= 4)
    {
        buf[0] = TRANS_SYNC;
        buf[1] = 2;
        buf[2] = SHIFT_BIT(HMI_BK_FG_SYNC,              0)
                |SHIFT_BIT(HMI_BK_SUP_0XFX,             1)
                |SHIFT_BIT(hmi_bk_lose_conn[hmi_t_num], 2);
        buf[3] = 0;

        if (hmi_info_sync_flag[hmi_t_num])
        {
            hmi_bk_lose_conn[hmi_t_num] = FALSE;
        }
        
        buf_index += 4;
    }
}

/*****************************************************************************
函  数 : disp_trans_ty_reset()
功  能 : 传输涂鸦复位命令
参  数 : U08 *disp_buf : 
返  回 : 无
其  他 : 无
*****************************************************************************/
void disp_trans_ty_reset(U08 *disp_buf)
{
    U08 remain_length = MAX_DISP - buf_index;
    U08* buf = &disp_buf[buf_index];

    if (remain_length >= 2+1)
    {
        buf[0] = TRANS_TY_CMD;
        buf[1] = 1;
        buf[2] = 1;

        buf_index += 2+1;
    }
}
/*****************************************************************************
函  数 : disp_trans_ty_info()
功  能 : 传送涂鸦WiFi屏产品信息
参  数 : U08 *disp_buf : 
返  回 : 无
其  他 : 无
*****************************************************************************/
void disp_trans_ty_info(U08 *disp_buf)
{
    U08 remain_length = MAX_DISP - buf_index;
    U08* buf = &disp_buf[buf_index];

    if (remain_length >= 2+1+16+1)
    {
        buf[0] = TRANS_TY_INFO;
        buf[1] = 1+16+1;
        TYB_upload_product_info(&buf[2]);

        buf_index += 2+1+16+1;
    }
}

/*****************************************************************************
函  数 : disp_trans_ty_data()
功  能 : 传送涂鸦WiFi屏数据信息
参  数 : U08 *disp_buf : 
         U08 dpid_idx  : DP索引
返  回 : 无
其  他 : 无
*****************************************************************************/
void disp_trans_ty_data(U08 *disp_buf, U08 dpid_idx)
{
    U08 remain_length = MAX_DISP - buf_index;
    U08* buf = &disp_buf[buf_index];
    U08 len = sizeof(TUYA_DATA);

        
    if (remain_length >= 2+len)
    {
        buf[0] = TRANS_TY_DATA;
        buf[1] = len;
        TYB_fill_dp((TUYA_DATA*)&buf[2], dpid_idx);
        buf_index += 2+len;
    }
}

/*****************************************************************************
函  数 : disp_trans_init()
功  能 : 传输初始化
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
void disp_trans_init(void)
{
    buf_index = 0;
}

