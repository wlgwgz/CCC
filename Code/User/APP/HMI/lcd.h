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



/* ���¶���ascii����disp_8_table[]��λ�� */
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

/* S����Ʒ��ʾ�������� */
/* =============================================================== */
/* ��ʾ���� */
#define DCS_NORMAL			0x0000	/* ������ʾ */
/* ��0~2λ: λ�� */
#define DNS_DIGIT1			0x0001	/* ��ʾ����Чλ��Ϊ1λ(����С����͸���) */
#define DNS_DIGIT2			0x0002	/* ��ʾ����Чλ��Ϊ2λ(����С����͸���) */
#define DNS_DIGIT3			0x0003	/* ��ʾ����Чλ��Ϊ3λ(����С����͸���) */
#define DNS_DIGIT4			0x0004	/* ��ʾ����Чλ��Ϊ4λ(����С����͸���) */
#define DNS_DIGIT5			0x0005	/* ��ʾ����Чλ��Ϊ5λ(����С����͸���) */
#define DNS_DIGIT_MASK		0x0007	/* ��Чλ��������λ(��3λ) */
/* ��3~4λ: С���� */
#define DNS_FLOAT1			0x0008	/* ��1λС�� */
#define DNS_FLOAT2			0x0010	/* ��2λС�� */
#define DNS_FLOAT_MASK		0x0018	/* С��λ������λ(��3,4λ) */
#define DNS_FLOAT_BIN_NUM	3		/* С�������ԵĶ�����λ�� */
/* ��5λ: ǰ��0 */
#define DNS_ZERO			0x0020	/* ǰ������ʾ */
/* ��6λ: �����ַ� */
#define DNS_SPE				0x0040	/* �������ַ�(ͼƬ)��ʾ */
/* ��7λ: ���� */
#define DTS_CENTER			0x0080	/* ���� */

/* ��8ΪΪֱ��Ӱ����ʾ������(�ᴫ��ǰ�崦��) */
/* ��8~10λ: λ�� */
#define DTS_CLR_HEAD		0x0100	/* ��ͷ */
#define DTS_CLR_TRAIL		0x0200	/* ��β */
///* ��11λ: ������;(������ʾʱ�����ø�����λ) */
//#define DTS_CLR_LINE		0x0800	/* ���� */
/* ��11λ: ���� */
#define DTS_RIGHT			0x0800	/* ���� */

/* ��12λ: ���� */
#define DCS_REVERSE			0x1000	/* ���� */
/* ��13λ: ��˸ */
#define DCS_BLINK			0x2000	/* ��˸ */
/* ��14��ˢ�� */
#define DCS_FLASH			0x4000	/* ˢ�� */

#define DTS_CLR_ALL			(DTS_CLR_HEAD | DTS_CLR_TRAIL)	//��ͷ��β

/* ��15λ: ���� */

#define ATTRIB_LONG_KEY			0x1000		/* ���� */ 
#define ATTRIB_CNTNUS_KEY		0x2000		/* ������ */ 
#define ATTRIB_ATTRIBUTE_KEY    0xf000		/* ���Լ� */

LCD_EXT U16 key_value[MAX_HMI];				/* ��ֵ(ǰ�崫����ԭʼֵ) */
LCD_EXT U16 Main_Key[MAX_HMI];				/* ��ֵ(�������İ���) */
LCD_EXT U16 ch_end_f[MAX_HMI];				/* ǰ�庺�ֽ������� */
LCD_EXT U16 bmp_end_f[MAX_HMI];				/* ǰ���Զ���ͼƬ�������� */
LCD_EXT U08 commu_sucess[MAX_HMI];			/* �ϴ�ͨѶ�ɹ� */
LCD_EXT U08 fg_disp_back[MAX_HMI];			/* �����ƽ��� */
LCD_EXT BOOL fg_fn_status; 
LCD_EXT BOOL fg_fn_mode;

LCD_EXT U08 cnt_key_static[MAX_HMI];			/* ��������ʱ���ʱ(100ms) */
LCD_EXT U08 cnt_cntnus_add[MAX_HMI];			/* ������������ʱ(5ms) */
LCD_EXT BOOL fg_long_key;			/* ������־ */
LCD_EXT BOOL fg_cntnus;				/* ��������־ */

#define KEYT_TIME_LONG				30		/* 3�� */
#define KEYT_TIME_CNTNUS_CNFRM		10		/* 1�� */
#define KEYT_TIME_CNTNUS_ADD		1		/* 100���� */

#define LOCK_T_IN_MAIN			3		/* �������������ʱ��(��) */
#define LOCK_T_IN_OTHER			45		/* ���������������ʱ��(��) */
#define LOCK_T_CLUE				3		/* ������ʾ����ʱ��(��) */

LCD_EXT BOOL fg_disp_lock;		        /* ������־��δ����������� */

/* ����(��ΧΪ1~7) */
#define LANGUE_NULL				0		/* ���У���Ч״̬ */
#define LANGUE_CHINESE			1		/* ���� */
#define LANGUE_ENGLISH			2		/* Ӣ�� */
#define LANGUE_JAPANESE			3		/* ���� */
#define LANGUE_MAX              4
typedef struct date        /* ʱ�� */
{
	U16  sec;				/* �� */
	U16  min;				/* �� */
	U16  hour;				/* ʱ */
	U16  day;				/* �� */
	U16  month;				/* �� */
	U16  week;				/* ���� */
	U16  year;				/* �� */
	
} RTC;
LCD_EXT RTC clock[MAX_HMI];		/* ʵʱʱ�� */ 
#define LOG_RTC RTC

LCD_EXT U08 clk_base;			/* ��׼ʱ��(���ĸ�HMI��ʱ����Ϊ��׼ʱ��) */

#define MAX_RTC_ITEM		7		/* ʱ������Ŀ�� */
#define RTC_SEC				0		/* �� */  
#define RTC_MIN				1		/* �� */  
#define RTC_HOUR			2		/* ʱ */  
#define RTC_DAY				3		/* �� */  
#define RTC_MONTH			4		/* �� */  
#define RTC_WEEK			5		/* ���� */
#define RTC_YEAR			6		/* �� */  

LCD_EXT BOOL fg_flash_all;				/* ����ˢ������ */
LCD_EXT BOOL fg_is_auto_flash_all;	    /* �Ƿ��Զ�����ˢ�� */
LCD_EXT U08 cnt_disp_steady[MAX_HMI];	/* ���治ˢ�³���ʱ��(��) */
LCD_EXT U08 cnt_no_operate[MAX_HMI];	/* �ް�������ʱ��(��) */
LCD_EXT U08 cnt_menu_lock[MAX_HMI];		/* ������������ʱ(��) */
LCD_EXT U08 cnt_lock_clue[MAX_HMI];		/* ������ʾ������ʱ(��) */
LCD_EXT U08 fg_set_clk[MAX_HMI];		/* ����ʱ�ӱ�־ */
LCD_EXT U08 fg_active[MAX_HMI];			/* ���Ƿ��ѱ�����(�Ƿ������) */
LCD_EXT U08 cnt_no_operate_sec[MAX_HMI];/* �ް�������ʱ��(��) */
LCD_EXT U08 fg_disp_ver_back[MAX_HMI];	/* ��ʾ���汾�� */
LCD_EXT BOOL fg_sound;					/* ���������־ */
//LCD_EXT U08 sound_delay[MAX_HMI];		// ��ʱ�ط�����		 
 
LCD_EXT U08 cnt_non_commu[MAX_HMI];		/* ��ͨѶ��ʱ */
LCD_EXT U08 cnt_screen_power[MAX_HMI];	/* ��ʾ������ʱ */

LCD_EXT BOOL fg_disp_all;		/* ����ˢ��ָʾ��־(��Ч����Ϊһ����ʾѭ��) */
LCD_EXT BOOL fg_disp_value;		/* ��ֵˢ��ָʾ��־(��Ч����Ϊһ����ʾѭ��) */
LCD_EXT BOOL fg_disp_blink;		/* ��˸ָʾ��־(��Ч����Ϊһ����ʾѭ��) */

#define MAX_FLASH_INTERVAL		5		/* �ˢ��ʱ�䣨�룩 */
#define MAX_OPER_WAIT			60		/* ������ȴ�ʱ�䣨�룩���޲���������� */

#define   ENLARGE_ROW   16   //������Э���е�ַ��������
#define   ENLARGE_LOW   8    //������Э���е�ַ��������

#define SCREEN_1		4
#define SCREEN_4		4  //ÿ����ʾ4��������Ϣ ����ղ��Խ���


LCD_EXT U08 cnt_com_forbid;
LCD_EXT BOOL fg_com_forbid;
LCD_EXT U08 buf_index;					/* ��ʾ�������� */

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
