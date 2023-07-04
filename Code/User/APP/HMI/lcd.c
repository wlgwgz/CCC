/***********************************************************************************
         design by punp co.ltd copyright limited
         desinger
         date :
         name : lcd.c
***********************************************************************************/

#define LCD_GLOBAL

#include "includes.h"

/* S����Ʒ��ʾ�������� */
#define ADDR_ASC_END	0x0080		/* ASCII������ַ */
#define ADDR_BMP_BGN	0xf500		/* �Զ���ͼƬ��ʼ��ַ */
#define ADDR_BMP_END	0xf8ff		/* �Զ���ͼƬ������ַ */

#define AC_LONG					1		/* �ַ���� */
#define AC_HIGH					2		/* �ַ��߶� */
#define HZ_LONG					2		/* ���ֿ�� */
#define HZ_HIGH					2		/* ���ָ߶� */

#define LCM_COLUMN_NUM		16			/* Һ��������(�ֽڵ�λ) */

#define SCREEN_POWER_TIME		5		/* ��ʾ������ʱʱ�� */
/*----------------------------------------------------*/
	/* ���±������궨�岻��������� */

#define MAX_ROW			64		/* �����ֵ */
#define MAX_COL			128		/* �����ֵ */
#define MAX_DISP		255		/* ��ʾ�����ֽ��� */
//#define MAX_DISP		100		/* ���� */

#define MIN_CODE_BYTE		7		/* �ͱ���ʱ����Ԫ�����ֽ���(5+2) */
#define MIN_CODE_WORD		4		/* �������ʱ����Ԫ�������� */
#define MIN_PIXEL_BYTE		41		/* �͵���ʱ����Ԫ�����ֽ���(9+32),����һ������,������ʾ������� */

//U08 disp_buf[MAX_DISP];			/* ��ʾ���� */
//U08 buf_index;					/* ��ʾ�������� */
static U08 spe_index;			/* ������仺�������� */
#define MAX_CMD_SUB		2		/* ��������� */
static U08 cmd_sub[MAX_CMD_SUB];/* ������(�����κ����ݣ��磺���������е�) */
static BOOL fg_snd_100ms_need; /* 100ms�������� */

#define MAX_UNIT_NUM		(64+10)		/* �����ʾ��Ԫ���� */ // zzp ��һ������,��ֹ����ʱ������Χ
#define MAX_PIX_CODE		32		/* �����¼����������С(��Ϊ��λ��ÿ����ʾ��Ԫռ4����) */
static U16 total_num;		/* ���ַ����� */
static U16 cmplt_num;		/* ������ַ����� */
static U16 pixel_code[MAX_PIX_CODE];	/* ���֧�֡�MAX_PIX_CODE/4��������Ԫ */
static U08 pixel_index = 0;	/* ���͵���ʱ�������� */

static BOOL fg_pixel_finish;    /* ��������ɱ�־ */

static BOOL fg_disp_overflow;	/* ��������� */
static BOOL fg_first_commu;		/* ��ǰ���һ��ͨѶ */
static BOOL fg_cur_code_ov;		/* ��ǰ���볬�� */
static BOOL fg_f_need_update;	/* ǰ����Ҫ����(û�е���̫��) */
static BOOL fg_f_need_update_has_remind;	/* ǰ����Ҫ���������ѱ�־ */

static U16 pixel_addr_ofst;		/* �����ƫ�Ƶ�ַ(һ����ʾ��Ԫ�е�ƫ��) */

#define NUM_NORMAL				0		/* �������� */
#define NUM_SPE					1		/* ��������(�Զ���ͼƬ��ʾ) */

#define NUM_DEC					10		/* ʮ���� */
#define NUM_HEX					16		/* ʮ������ */

#define MAX_VALUE_NUM			8		/* ��ֵ���λ��(5λ����+С����+����+������) */

/* �߽������ */
#define CD_NORMAL				1		/* ���� */
#define CD_ABN_BACK				2		/* ��������쳣 */
#define CD_ABN_MUTUAL			3		/* ����ǰ��ƥ���쳣 */
#define CD_ABN_CUR				4		/* ��ǰ�����쳣(����) */

/* �ָ���(����������ģ���ͨѶЭ���й�) */
#define UINT_END		0xfffd	/* ��Ԫ�ָ��� */
#define RECORD_END		0xfffe	/* ��¼�ָ��� */
/* ���� */
#define TRANS_CLR_ALL			0x40	/* ���� */
#define TRANS_CLR_ROW			0x41	/* ���� */
#define TRANS_CODE				0x42	/* ���ͱ��� */
#define TRANS_VER_F				0x43	/* ��ʾǰ��汾�� */
#define TRANS_TIME_F			0x44	/* ��ʾǰ��ʱ�� */
#define TRANS_PIXEL				0x45	/* ���͵���(�������е�����) */
#define TRANS_PIXEL_ONLY		0x46	/* ���͵���(���е�������) */
#define TRANS_BAK4				0x47	/* ���� */
#define TRANS_SYNC              0x4F    /* ״̬ͬ�� */
#define TRANS_TY_INFO           0x51    /* ͿѻWiFi��_�ϱ���ƷID */
#define TRANS_TY_DATA           0x52    /* ͿѻWiFi��_�ϱ����� */
#define TRANS_TY_CMD            0x53    /* ͿѻWiFi��_���� */
#define TRANS_CLOCK				0x5f	/* дǰ��ʱ�� */
#define TRANS_SIMPLE			0x60	/* ������(����ʾ���ݵ�����) */
#define TRANS_NONE				0x7f	/* ����ʾ���� */
/* ������ */
	#define CMD_SUB_NONE		0x00	/* ������ */
//	#define CMD_BAK1			0x01	/* ���� */
	#define CMD_SUB_SND_ON		0x02	/* �������� */
	#define CMD_SUB_SND_OFF		0x04	/* �ط����� */
	#define CMD_SUB_SND_100MS	0x08	/* ��������100ms */
	#define CMD_SUB_LCD_OFF		0x10	/* �ر��� */
	#define CMD_SUB_LCD_ON		0x20	/* ������ */
	#define CMD_SUB_TEST_F		0x40	/* ����ǰ����Գ��� */
	#define CMD_SUB_FORBID_AUTO_LCD_ON		0x80	/* ��ֹ�Զ������� */
	//#define CMD_SUB_SET_CLK		0x80	/* ����ʱ�� */
	/* �ڶ����ֽ� */
	#define CMD_LANGUE_BGN		0		/* ����ѡ����ʼBIT */
	#define CMD_LANGUE_BITMASK	0x07	/* ����ѡ���Ӧ��BITMASK */



#define FONT_STD				0		/* ��׼�� */
#define FONT_SPE				1		/* �Զ����� */

	/* �������棬�����棬 �û����ã� �������ã� ״̬��ѯ�����ϲ�ѯ����ʷ���ϲ�ѯ�����룬  ʹ�����ޣ� �������� */
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
������disp_switch
���ܣ�����ת����
	���ݰ�����Ϣ�����������֮����л�
��������
����ֵ����
*****************************************************/
void disp_switch(U08 *disp_buf)
{
#ifdef USE_HMI_DISP
	dm603_tasking(disp_buf);
#endif	
}

/******************************************************
������disp_string_ptr8
���ܣ���ʾ�ַ���(ֱ����ʾ)��
�������ַ���������ʼ��ַ(8λ���ݵ�ָ��); ��(��); ��(��); ����; ��������ַ(д)
����ֵ����
*****************************************************/
void disp_string_ptr8(PTR_FAR_C08 string_ptr, U16 row, U16 col, U16 attrib, U08 *disp_buf)
{
	disp_record_init();		/* ��¼������ʼ�� */
	
	if (disp_is_text_need_flash(attrib))
	{
    	disp_code_16(string_ptr, row, col, attrib, disp_buf);
    }
}

/******************************************************
������disp_test_string
���ܣ���ʾ�ַ�����
�������ַ���������ʼ��ַ(16λ����ָ��); ��(��); ��(��); ����; ��������ַ(д)
����ֵ����
*****************************************************/
void disp_test_string(PTR_FAR_C08 string_ptr, U16 row, U16 col, U16 attrib, U08 *disp_buf)
{
	disp_record_init();		/* ��¼������ʼ�� */
	
	if (disp_is_text_need_flash(attrib))
	{
		disp_code_16(string_ptr, row, col, attrib, disp_buf);
    }
}

/******************************************************
������disp_string
���ܣ���ʾ�ַ�����
�������ַ����������; ��(��); ��(��); ����; ��������ַ(д)
����ֵ����
*****************************************************/
void disp_string(U16 string_index, U16 row, U16 col, U16 attrib, U08 *disp_buf)
{
	PTR_FAR_C08 string_ptr=NULL;
	disp_record_init();		/* ��¼������ʼ�� */

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
������disp_bmp
���ܣ���ʾͼƬ��
	����ʹ�ñ��뻹�ǵ�����ʾ��
	���һ��ͨѶ��ʾ��ȫ����Ҫ�����ϴ�״̬��������ʾ��ֱ����ʾ����Ϊֹ��
������ͼƬ����; ��; ��; ����; ��������ַ(д)
����ֵ����
Ҫ��һ��ֻ������һ����Ԫ��
*****************************************************/
void disp_bmp(U16 bmp_code, U16 row, U16 col, U16 attrib, U08 *disp_buf)
{
//	U08 font_long, font_high;
	U16 temp_code;
	U16 temp_row, temp_col;
	U08 buf_index_bak = buf_index;		/* �������� */
	
	disp_cur_code_check(bmp_code);
	
	if (buf_index > MAX_DISP - MIN_CODE_BYTE)			/* ��ʾ���泬�� */
//      ���󲻹�����������������	    
//      || spe_index > MAX_PIX_CODE - MIN_CODE_WORD)    /* ����Ԫ���������� */
	{
		fg_disp_overflow = TRUE;
		return;
	}
	
	temp_code = bmp_code;
	temp_row = row;
	temp_col = col;
	
	disp_record_init();		/* ��¼������ʼ�� */
	
	if (disp_is_text_need_flash(attrib))
	{
		buf_index_bak = buf_index;
		buf_index += 2;			/* Ԥ����������ݸ�����λ�� */
		
    	/* ����д�뻺���� */
		if (disp_is_str_in_font(temp_code))		/* �������ֿ��� */
		{
			cmplt_num++;
		}
        else if (spe_index > MAX_PIX_CODE - MIN_CODE_WORD)/* ���󲻹�,���������� */  
        {
            /* û�еĺ��ֹ���,ǰ����Ҫ���� */
            fg_f_need_update = TRUE;
            cmplt_num++;
        }
		else 	/* �����ֿ��У��������� */
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
    	
    	disp_wr_cmd(buf_index_bak, TRANS_CODE, disp_buf);		/* ���ͱ������� */
    }
}

/******************************************************
������disp_value_I16
���ܣ���ʾ16Ϊ�з�������
	����ֵת��Ϊ�������֣���ŵ�ָ���洢���У����հ��ַ����ķ�ʽ��ʾ��
	������С����ʾ��
��������ֵ; ; ; 
����ֵ����
*****************************************************/
void disp_value_I16(I16 value, U16 row, U16 col, U16 attrib, U08 *disp_buf)
{
//	U08 i;
	U08 disp_num;		/* ��ʾλ��(����λ��������С����) */
	U08 buf_src[MAX_VALUE_NUM];
	U08 buf_dest[MAX_VALUE_NUM];
	U08 max_num;		/* �������λ�� */
	U08 vld_num;		/* ��Ч����λ�� */
	I16 temp_value;
	BOOL fg_neg_need = FALSE;	/* ��Ҫ��ʾ���ű�־ */
	
	disp_record_init();		/* ��¼������ʼ�� */
	
	if (disp_is_value_need_flash(attrib))		/* ��ֵˢ�� */
	{
		temp_value = value;
		if (value < 0)	/* �з�����ʾ��ֵС��0 */
		{
			temp_value = ~value;		
			temp_value++;
			fg_neg_need = TRUE;
		}
		
    	disp_num = attrib & DNS_DIGIT_MASK;		/* ��ʾλ�� */
    	if (disp_num > DNS_DIGIT5)		disp_num = DNS_DIGIT5;
    	max_num = disp_num;	
    	vld_num = disp_num;
    		
		disp_get_num(temp_value, disp_num, buf_src);	/* ȡ���� */
		vld_num = disp_value_dot(buf_src, disp_num, &max_num, attrib);	/* ����С�����ǰ��0 */
		disp_value_neg(buf_src, &vld_num, &max_num, fg_neg_need);		/* ��ʾ���� */
        
        disp_value_cpmt_blank(buf_src, vld_num, max_num, attrib);	/* ���ո� */
        
    	disp_num_code(buf_src, max_num, buf_dest);					/* �����ֱ��� */
    	disp_code_16(buf_dest, row, col, attrib, disp_buf);			/* ���ַ����ķ�ʽ��ʾ */
    }
}

/******************************************************
������disp_value
���ܣ���ʾ16λ�޷�����
	����ֵת��Ϊ�������֣���ŵ�ָ���洢���У����հ��ַ����ķ�ʽ��ʾ��
	֧��С����ʾ��
��������ֵ; ; ; 
����ֵ����
*****************************************************/
void disp_value_U16(U16 value, U16 row, U16 col, U16 attrib, U08 *disp_buf)
{
//	U08 i;
	U08 disp_num;		/* ��ʾλ��(����λ��������С����) */
	U08 buf_src[MAX_VALUE_NUM];
	U08 buf_dest[MAX_VALUE_NUM];
	U08 max_num;		/* �������λ�� */
	U08 vld_num;		/* ��Ч����λ�� */
	
	disp_record_init();		/* ��¼������ʼ�� */
	
	if (disp_is_value_need_flash(attrib))		/* ��ֵˢ�� */
	{		
    	disp_num = attrib & DNS_DIGIT_MASK;		/* ��ʾλ�� */
    	if (disp_num > DNS_DIGIT5)		disp_num = DNS_DIGIT5;
    	max_num = disp_num;
    	vld_num = disp_num;
    		
		disp_get_num(value, disp_num, buf_src);	/* ȡ���� */
		vld_num = disp_value_dot(buf_src, disp_num, &max_num, attrib);	/* ����С�����ǰ��0 */
        
        disp_value_cpmt_blank(buf_src, vld_num, max_num, attrib);		/* ���ո� */
        
    	disp_num_code(buf_src, max_num, buf_dest);						/* �����ֱ��� */
    	disp_code_16(buf_dest, row, col, attrib, disp_buf);				/* ���ַ����ķ�ʽ��ʾ */
    }
}

/******************************************************
������disp_value_I08
���ܣ���ʾ8λ�з�������
	����ֵת��Ϊ�������֣���ŵ�ָ���洢���У����հ��ַ����ķ�ʽ��ʾ��
	������С����ʾ��
��������ֵ; ; ; 
����ֵ����
*****************************************************/
void disp_value_I08(I08 value, U16 row, U16 col, U16 attrib, U08 *disp_buf)
{
//	U08 i;
	U08 disp_num;		/* ��ʾλ��(����λ��������С����) */
	U08 buf_src[MAX_VALUE_NUM];
	U08 buf_dest[MAX_VALUE_NUM];
	U08 max_num;		/* �������λ�� */
	U08 vld_num;		/* ��Ч����λ�� */
	I08 temp_value;
	BOOL fg_neg_need = FALSE;	/* ��Ҫ��ʾ���ű�־ */
	
	disp_record_init();		/* ��¼������ʼ�� */
	
	if (disp_is_value_need_flash(attrib))		/* ��ֵˢ�� */
	{
		temp_value = value;
		if (value < 0)	/* �з�����ʾ��ֵС��0 */
		{
			temp_value = ~value;		
			temp_value++;
			fg_neg_need = TRUE;
		}
		
    	disp_num = attrib & DNS_DIGIT_MASK;		/* ��ʾλ�� */
    	if (disp_num > DNS_DIGIT3)		disp_num = DNS_DIGIT3;
    	max_num = disp_num;	
    	vld_num = disp_num;
    		
		disp_get_num(temp_value, disp_num, buf_src);					/* ȡ���� */
		vld_num = disp_value_dot(buf_src, disp_num, &max_num, attrib);	/* ����С�����ǰ��0 */
		disp_value_neg(buf_src, &vld_num, &max_num, fg_neg_need);		/* ��ʾ���� */
        
        disp_value_cpmt_blank(buf_src, vld_num, max_num, attrib);	/* ���ո� */
        
    	disp_num_code(buf_src, max_num, buf_dest);					/* �����ֱ��� */
    	disp_code_16(buf_dest, row, col, attrib, disp_buf);			/* ���ַ����ķ�ʽ��ʾ */
    }
}

/******************************************************
������disp_value_U08
���ܣ���ʾ8λ�޷�����
	����ֵת��Ϊ�������֣���ŵ�ָ���洢���У����հ��ַ����ķ�ʽ��ʾ��
	֧��С����ʾ��
��������ֵ; ; ; 
����ֵ����
*****************************************************/
void disp_value_U08(U08 value, U16 row, U16 col, U16 attrib, U08 *disp_buf)
{
//	U08 i;
	U08 disp_num;		/* ��ʾλ��(����λ��������С����) */
	U08 buf_src[MAX_VALUE_NUM];
	U08 buf_dest[MAX_VALUE_NUM];
	U08 max_num;		/* �������λ�� */
	U08 vld_num;		/* ��Ч����λ�� */
		
	disp_record_init();		/* ��¼������ʼ�� */
	
	if (disp_is_value_need_flash(attrib))		/* ��ֵˢ�� */
	{		
    	disp_num = attrib & DNS_DIGIT_MASK;		/* ��ʾλ�� */
    	if (disp_num > DNS_DIGIT3)		disp_num = DNS_DIGIT3;
    	max_num = disp_num;
    	vld_num = disp_num;
    		
		disp_get_num(value, disp_num, buf_src);	/* ȡ���� */
		vld_num = disp_value_dot(buf_src, disp_num, &max_num, attrib);	/* ����С�����ǰ��0 */
        
        disp_value_cpmt_blank(buf_src, vld_num, max_num, attrib);	/* ���ո� */
        
	   	disp_num_code(buf_src, max_num, buf_dest);			/* �����ֱ��� */
    	disp_code_16(buf_dest, row, col, attrib, disp_buf);	/* ���ַ����ķ�ʽ��ʾ */
    }
}

/*****************************************************************************
��  �� : disp_value_16()
��  �� : ��ʾ16λ�з���/�޷�����
��  �� : I16 value     : 
         U16 row       : 
         U16 col       : 
         U16 format    : 
         U16 attrib    : 
         U08 *disp_buf : 
��  �� : ��
��  �� : ��
*****************************************************************************/
void disp_value_16(I16 value, U16 row, U16 col, U16 format, U16 attrib, U08 *disp_buf)
{
	U08 num_buf[8];
		
	disp_record_init();		/* ��¼������ʼ�� */
	
	if (disp_is_value_need_flash(attrib))		/* ��ֵˢ�� */
	{		
    	num_buf[Fun_NumFormat((U16)value, num_buf, format)] = '\0';
    	disp_code_16(num_buf, row, col, attrib, disp_buf);	/* ���ַ����ķ�ʽ��ʾ */
    }
}

/**********************************************
������disp_clr_screen
���ܣ�����
��������
����ֵ����
***********************************************/
void disp_clr_screen(U08 *disp_buf)
{
	U08 buf_index_bak = buf_index;		/* �������� */
	
	if (buf_index > MAX_DISP - MIN_CODE_BYTE)			/* ��ʾ���泬�� */	
	{
		fg_disp_overflow = TRUE;
		return;
	}
	
	buf_index_bak = buf_index;
	buf_index += 2;			/* Ԥ����������ݸ�����λ�� */
		
    disp_buf[buf_index++] = 0;
	
	disp_wr_cmd(buf_index_bak, TRANS_CLR_ALL, disp_buf);	/* �������� */
}
/**********************************************
������disp_clr_line
���ܣ�����
		�����Դ�����ѭ������ĸ�ʽ�����ޱ������ݡ�
��������
����ֵ����
***********************************************/
void disp_clr_line(U16 row, U08 *disp_buf)
{
	U08 buf_index_bak = buf_index;		/* �������� */
	
	if (buf_index > MAX_DISP - MIN_CODE_BYTE)			/* ��ʾ���泬�� */	
	{
		fg_disp_overflow = TRUE;
		return;
	}
	
	buf_index_bak = buf_index;
	buf_index += 2;			/* Ԥ����������ݸ�����λ�� */
		
    disp_buf[buf_index++] = HIGH_BYTE(row);
	disp_buf[buf_index++] = LOW_BYTE(row);			/* �е�ַ */
	
	disp_wr_cmd(buf_index_bak, TRANS_CLR_ROW, disp_buf);	/* �������� */
}

/******************************************************************** 
 �� �� ��:	 void lcd_clcontime(void) 
 ��  ��:��ʱ��������û�а������������趨ʱ������������������
 ��ڲ���:
 �� �� ֵ:��
***********************************************************************/
//void lcd_clcontime(U08 *disp_buf)
//{
//	//û�а������³����趨ֵ����������������
//	if(g_screen_clc_time == 0)   
//	{ 
//		g_screen_clc_time = CLC_TIME; //���¼�ʱ
//		fg_flash_all = TRUE;
//		//fg_flash_once = TRUE;
//		disp_clr_screen(disp_buf);
//	}
//}

/**********************************************
������disp_version_f
���ܣ���ʾǰ��汾
		�����Դ�����ѭ������ĸ�ʽ�����ޱ������ݡ�
��������
����ֵ����
***********************************************/
void disp_version_f(U16 row, U16 col, U16 attrib, U08 *disp_buf)
{
	U08 buf_index_bak = buf_index;		/* �������� */
	
	if (buf_index > MAX_DISP - MIN_CODE_BYTE)			/* ��ʾ���泬�� */	
	{
		fg_disp_overflow = TRUE;
		return;
	}
	
	buf_index_bak = buf_index;
	buf_index += 2;			/* Ԥ����������ݸ�����λ�� */
    
    disp_buf[buf_index++] = HIGH_BYTE(row);
	disp_buf[buf_index++] = LOW_BYTE(row);			/* �е�ַ */
	disp_buf[buf_index++] = HIGH_BYTE(col);	
	disp_buf[buf_index++] = LOW_BYTE(col);			/* �е�ַ */
	disp_buf[buf_index++] = HIGH_BYTE(attrib);			/* ���� */
	
	disp_wr_cmd(buf_index_bak, TRANS_VER_F, disp_buf);		/* ��ʾ�汾������ */
}
/**********************************************
������disp_set_snd()
���ܣ���������
��������
����ֵ����
***********************************************/
void disp_set_snd(void)
{
#ifndef USE_NO_SPEAKER
	fg_sound = TRUE;
#endif 
}
/**********************************************
������disp_clr_snd()
���ܣ��ط�����
��������
����ֵ����
***********************************************/
void disp_clr_snd(void)
{
	fg_sound = FALSE;
}
/**********************************************
������disp_snd_100ms
���ܣ���������100MS
��������
����ֵ����
***********************************************/
//void disp_snd_100ms(void)
//{
//    cmd_sub[0] |= CMD_SUB_SND_100MS;
//}

/**********************************************
������disp_arg_init
���ܣ���ʾ������ʼ��
		�ϵ��ʼ��һ��
��������
����ֵ����
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

	dm603_init();	/* DM603A���������ݳ�ʼ�� */
}

/**********************************************
������disp_test_f
���ܣ�ִ��ǰ����Գ���(����Ȩ����ǰ��)
��������
����ֵ����
***********************************************/
void disp_test_f(void)
{
	cmd_sub[0] |= CMD_SUB_TEST_F;
	fg_disp_back[hmi_t_num] = FALSE;
}

/**********************************************
������disp_set_langue
���ܣ�ѡ������
����������
����ֵ����
***********************************************/
void disp_set_langue(U08 langue)
{
	cmd_sub[1] |= (langue << CMD_LANGUE_BGN) & CMD_LANGUE_BITMASK;
}

/**********************************************
������disp_set_clk
���ܣ�����ʱ��(����Ȩ����ǰ��)
��������
����ֵ����
***********************************************/
void disp_set_clk(U08 *disp_buf)
{
	U08 buf_index_bak = buf_index;		/* �������� */
	
	if (buf_index > MAX_DISP - MIN_CODE_BYTE)			/* ��ʾ���泬�� */	
	{
		fg_disp_overflow = TRUE;
		return;
	}
	
	fg_disp_back[hmi_t_num] = FALSE;
	fg_set_clk[hmi_t_num] = TRUE;
	
	buf_index_bak = buf_index;
	buf_index += 2;			/* Ԥ����������ݸ�����λ�� */
    
	/* ��λ ȷ�� �� �� */
    disp_buf[buf_index++] = DM603_KEY_FUNC;
	disp_buf[buf_index++] = DM603_KEY_OK;			/* �е�ַ */
	disp_buf[buf_index++] = DM603_KEY_UP;	
	disp_buf[buf_index++] = DM603_KEY_DOWN;			/* �е�ַ */
	
	disp_wr_cmd(buf_index_bak, TRANS_TIME_F, disp_buf);		/* ��ʾ�汾������ */
}

/**********************************************
������disp_wr_hmi_clk
���ܣ�ֱ��дǰ��ʱ��
		�͡�disp_set_clk����ͬ���ú�����ֱ��дǰ���ڴ�
��������ʾ����
����ֵ����
***********************************************/
BOOL disp_wr_hmi_clk(U08 *disp_buf, RTC clock_src)
{
	U08 buf_index_bak = buf_index;		/* �������� */
	
	if (buf_index > MAX_DISP - MIN_CODE_BYTE)			/* ��ʾ���泬�� */	
	{
		fg_disp_overflow = TRUE;
		return FALSE;
	}
	
	buf_index_bak = buf_index;
	buf_index += 2;			/* Ԥ����������ݸ�����λ�� */
    
    disp_buf[buf_index++] = clock_src.sec;
	disp_buf[buf_index++] = clock_src.min;			/*  */
	disp_buf[buf_index++] = clock_src.hour;	
	disp_buf[buf_index++] = clock_src.day;			/*  */
	disp_buf[buf_index++] = clock_src.month;		/*  */
	disp_buf[buf_index++] = clock_src.week;			/*  */
//	disp_buf[buf_index++] = clock_src.year;			/*  */
	/* �ı���RTC������U08���ͣ�ֻ�е���λ��������ΪU16����������λ */
	disp_buf[buf_index++] = (clock_src.year>=2000 ? (clock_src.year-2000) : clock_src.year);
	
	disp_wr_cmd(buf_index_bak, TRANS_CLOCK, disp_buf);		/* дʱ������ */
	
	return TRUE;
}

/******************************************************
������disp_routine
���ܣ���ʾ�ܵ��ȡ�
	�������֡��ַ���ͼƬ�ںδ���ʾ��������Ҫ������ʾ�Ƿ�ˢ�¡�
��������
����ֵ����
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
		cmd_sub[i] = CMD_SUB_NONE;			/* �������ʼ�� */
	}
	
	bnd_chk_result = disp_bound_code_check();

	buf_index = 0;						/* ��ʾ����������ʼ�� */


	//if (m_get_error(MAX_PCB, ERR_ID_CODE))		
	//{
		//fg_flash_all = TRUE;
		
		//dm603_face[hmi_t_num] = DM603_FACE_MAC;
		//dm603_face_mac(disp_buf);
	//}
	//else
	//{
	
	disp_screen_power();		/* �Ƿ���뵹��ʱ */
	
	if (fg_flash_all)						
	{
		cnt_disp_steady[hmi_t_num] = 0;
	}
	else if (cnt_disp_steady[hmi_t_num] > MAX_FLASH_INTERVAL)	
	{
		fg_flash_all = TRUE;    /* �Զ�ˢ�±�־ */
        fg_is_auto_flash_all = TRUE;
		cnt_disp_steady[hmi_t_num] = 0;
        disp_set_langue(dm603_language);
	}
//	if (cmplt_num >= total_num)	fg_flash_all = TRUE;		/* ���� */
	
	
	/* �������Ȩת�� */
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
	dm603_online_routine(disp_buf);	/* �ϵ�ѭ������ */
	dm603_routine(disp_buf);	/* ѭ������ */
#endif

#ifndef DEBUG	// ����ʱ���ر���	 
	if (disp_screen_dark(disp_buf))
	{
		disp_lcd_off();
	}
	else 			/* ������ʾ */
#endif
	{
#if 1	/* XM0015 */
    	if (!fg_first_commu && bnd_chk_result != CD_NORMAL	/* �߽���� */ 
            || (!fg_f_need_update_has_remind && fg_f_need_update)/* ǰ����Ҫ����,����һ�� */
    		|| fg_cur_code_ov || fg_disp_overflow)			/* ������� */
    	{
    		disp_mutual_abnormal(bnd_chk_result, disp_buf);
    	}
    	else if(fg_pixel_finish)    /* ��������ż���������� */
#endif
    	{
    		disp_switch(disp_buf);
    	}
    	/* ��������� */
    	disp_pixel(disp_buf);
    }

    /* ��������(A���в���ʱ��B��)���в�����������Ȩ��ǰ�� */
	if ((Main_Key[hmi_t_num] != KEY_NULL || !fg_disp_back[hmi_t_num])
		&& cnt_menu_lock[hmi_t_num] == 0)
	{
		if (hmi_next != hmi_t_num)	/* �ų�ֻ��һ��������� */
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
	if (GET_BIT(hmi_commu_error, hmi_t_num))	cnt_menu_lock[hmi_next] = 0;	/* A��ͨѶ���ϣ������B�������� */
	
	if (Main_Key[hmi_t_num] != KEY_NULL)	fg_active[hmi_t_num]  = TRUE;	/* ���� */
    
    disp_deal_snd();			/* ���������� */
    disp_cmd_simple(disp_buf);	/* ������� */
    disp_clk_syn(disp_buf);		/* ʱ��ͬ�� */
	//}

	fg_disp_all = FALSE;			/* ָʾ�ý�������������ʾ����� */
	fg_disp_value = FALSE;			/* ָʾ�ý�����ֵ������ʾ����� */
	fg_disp_blink = FALSE;			/* ָʾ�ý�����˸���ݱ�����ʾ����� */
	Main_Key[hmi_t_num] = KEY_NULL;	/* ������λ(��ֹ��������) */
	Key[hmi_t_num].value = KEY_NULL;
	
	
	return buf_index;
}



/******************************************************
������disp_read_key
���ܣ���ȡ����	
��������
����ֵ����
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

	//������
	if (((key_now[hmi_r_num] & (~ATTRIB_ATTRIBUTE_KEY)) == KEY_UP) || \
		((key_now[hmi_r_num] & (~ATTRIB_ATTRIBUTE_KEY)) == KEY_DOWN)) 
	{
		if (cnt_key_static[hmi_r_num] > KEYT_TIME_CNTNUS_CNFRM)	/* 1Sʱ�䵽������������ */
		{//	1����
			if (cnt_cntnus_add[hmi_r_num] >= KEYT_TIME_CNTNUS_ADD)	/* ������ֵ��ȡ */ 
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

	/* 3��� */
	flag = 0;
	if (key_now[hmi_r_num] & ATTRIB_LONG_KEY) //�ж��Ƿ񳤰���
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

	//��ǰ����
	Main_Key[hmi_r_num] = KEY_NULL;		/* ��ʼ�� */
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

	//�������ҷ���Ч����������ֻ��Ӧ�ϴ���Ч�ĳ�����
	if(fg_long_key && (Main_Key[hmi_r_num] != KEY_UP) && (Main_Key[hmi_r_num] != KEY_DOWN)) 
	{
		Main_Key[hmi_r_num] = Key_Tmp[hmi_r_num]; 
	}
	
	//���������ҷ���Ч�����������������
	if((key_now[hmi_r_num] & ATTRIB_CNTNUS_KEY) && ((key_now[hmi_r_num] & ATTRIB_LONG_KEY) != ATTRIB_LONG_KEY) && \
	   (Main_Key[hmi_r_num] != KEY_UP) && (Main_Key[hmi_r_num] != KEY_DOWN))
	{
		Main_Key[hmi_r_num] = KEY_NULL;	
	}

	/* ��������ʱ���� */
	if ((key_now[hmi_r_num] & ATTRIB_ATTRIBUTE_KEY) == 0)
	{
		key_continuous_time[hmi_r_num] = 0;
	}

    Key[hmi_r_num].value = Main_Key[hmi_r_num];
    Key[hmi_r_num].time = key_continuous_time[hmi_r_num];     
}


/*------------------------------------------------------------*/
/* ����Ϊ���ݲ㺯����Ӧ�ò㲻�ɵ��� */

/******************************************************
������disp_code_16
���ܣ��ַ�������д���档
	����ʹ�ñ��뻹�ǵ�����ʾ��
	���һ��ͨѶ��ʾ��ȫ����Ҫ�����ϴ�״̬��������ʾ��ֱ����ʾ����Ϊֹ��
�������ַ�����ʼ��ַ(�ֽ�); ��; ��; ����; ��������ַ(д)
����ֵ����
Ҫ��ֻ�������׼��(8*16-ASCII�ַ���16*16����)���Զ���ͼƬ�á�disp_bmp����ʾ
	  ����Ժ�Ҫ֧�������ߴ�ĵ�Ԫ������Ҫ���ġ�
*****************************************************/
static void disp_code_16(PTR_FAR_C08 string_ptr, U16 row, U16 col, U16 attrib, U08 *disp_buf)
{
	U08 font_long, font_high;
	U16 max_high = 0;			/* ��ߵĵ�Ԫ�߶� */	
	PTR_FAR_C08 temp_ptr;
	U16 temp_code;
	U16 temp_row, temp_col;
	U08 buf_index_bak = buf_index;		/* �������� */
	U08 col_cnt;					/* �ַ���ռ������(�ֽڵ�λ) */
	U08 col_end;					/* �ַ���������ֵ(�ֽڵ�λ) */
	
	temp_ptr = string_ptr;
	temp_row = row;
	temp_col = col;
	
	/* �����ַ��������е�Ԫ���֮��(8�����ص㵥λ) */
    col_cnt = 0; 	
    while(*temp_ptr != '\0')		
	{
		temp_code = *temp_ptr++;
		if (temp_code > ADDR_ASC_END)			/* �������ַ���ռ�����ֽ� */
		{
			temp_code = MAKEWORD(temp_code, *temp_ptr++);
		}
	
		disp_cal_dim(temp_code, &font_long, &font_high);
		
		col_cnt += font_long;
	}
    
    /* ���м�����(���еĿ�������8����Ϊ��λ) */
    if ((attrib & DTS_CENTER) != 0)		/* ���� */
    {
    	if (col_cnt < LCM_COLUMN_NUM)	temp_col = (LCM_COLUMN_NUM - col_cnt)/2;
    	else 							temp_col = 0;
    		
    	temp_col *= 8;		/* ת���ɵ�Ϊ��λ */
    }
    else if ((attrib & DTS_RIGHT) != 0)	/* ���� */
    {
    	col_end = col/8;
    	if (col_cnt < col_end)			temp_col = (col_end - col_cnt);
    	else 							temp_col = 0;
    		
    	temp_col *= 8;		/* ת���ɵ�Ϊ��λ */
	}
	
	if (buf_index < MAX_DISP - MIN_CODE_BYTE)
	{
    	buf_index += 2;			/* Ԥ����������ݸ�����λ�� */
    	
    	/* д��ַ������(ÿ���ַ���ֻ��һ����ַ������) */
    	disp_buf[buf_index++] = HIGH_BYTE(temp_row);
    	disp_buf[buf_index++] = LOW_BYTE(temp_row);	/* �е�ַ */
    	disp_buf[buf_index++] = HIGH_BYTE(temp_col);	
    	disp_buf[buf_index++] = LOW_BYTE(temp_col);	/* �е�ַ */
    	disp_buf[buf_index++] = HIGH_BYTE(attrib);		/* ���� */
    }
		
	/* ����д�뻺���� */
	temp_ptr = string_ptr;
	while(*temp_ptr != '\0')
	{
		if (total_num++ > MAX_UNIT_NUM)			    	break;		/* ��Ԫ����������쳣 */
		if (buf_index > MAX_DISP - MIN_CODE_BYTE)		break;		/* ��ʾ���泬�� */
//      ���󲻹�����������������	
//      if (spe_index > MAX_PIX_CODE - MIN_CODE_WORD)   break;      /* ����Ԫ���������� */
		
		temp_code = *temp_ptr++;
		if (temp_code > ADDR_ASC_END)			/* �������ַ���ռ�����ֽ� */
		{
			temp_code = MAKEWORD(temp_code, *temp_ptr++);
		}
		disp_cur_code_check(temp_code);
		if (disp_is_bmp(temp_code))				continue;	/* �Զ���ͼƬ�������� */
					
		if (disp_is_str_in_font(temp_code))		/* �������ֿ��� */
		{
			cmplt_num++;
		}
        else if (spe_index > MAX_PIX_CODE - MIN_CODE_WORD)/* ���󲻹�,���������� */
        {
            /* û�еĺ��ֹ���,ǰ����Ҫ���� */
            fg_f_need_update = TRUE;
            cmplt_num++;
        }
		else 	/* �����ֿ��У��������� */
		{
			pixel_code[spe_index++] = temp_row;
			pixel_code[spe_index++] = temp_col;
			pixel_code[spe_index++] = attrib;
			pixel_code[spe_index++] = temp_code;	
            /* �Զ�ˢ��ʱ,��0xFFFF,�����֡������ʱ����˸������ */
            if(fg_is_auto_flash_all)    temp_code = 0xFFFF;
            else            temp_code = MAKEWORD(' ', ' '); /* ������ʱ,�ȴ��հ� */
		}		
		
		if (temp_code > ADDR_ASC_END)	
		{
			disp_buf[buf_index++] = HIGH_BYTE(temp_code);
		}
		disp_buf[buf_index++] = LOW_BYTE(temp_code);
		
		/* ͨ����Ԫ�߶ȺͿ�ȼ����Ƿ��У���Ϊ����Ԫ��¼��ַ */
		disp_cal_dim(temp_code, &font_long, &font_high);
    	max_high = max_high > font_high ? max_high : font_high;
    	
    	temp_col += (U16)font_long*8;
    	if (temp_col >= MAX_COL)		/* ����һ����ʾ��Χ������ */
    	{
    		temp_row += max_high*8;
    		temp_col = 0;
    	}
	}
	
	disp_wr_cmd(buf_index_bak, TRANS_CODE, disp_buf);		/* ���ͱ������� */
	
	if (*temp_ptr != '\0') 			/* ���ַ���δ���� */
	{
		fg_disp_overflow = TRUE;
	}
}

/******************************************************
������disp_pixel
���ܣ���ʾ����
���������������ַ
����ֵ����ǰ��Ԫȡ�����/δ���
*****************************************************/
static void disp_pixel(U08 *disp_buf)
{
	U16 temp_row, temp_col, temp_attrib;
	U08 buf_index_bak = buf_index;		/* �������� */
//	U08 data_num = 0;			/* ��ǰ��ʾ��Ԫ���ݸ��� */
	U08 temp_cmd = TRANS_NONE;

	if (
//		disp_is_code_cmplt()				/* �����Ѵ��� */
//		&& 
		cmplt_num < total_num)			/* �贫�͵��� */
	{
        fg_pixel_finish = FALSE;        /* ����δ������ */
        
		while(cmplt_num < total_num)
		{
			if (buf_index > MAX_DISP - MIN_PIXEL_BYTE)		break;		/* ��ʾ���泬�� */

            /* ����������Χ����Ϊ������� */
			if (pixel_index >= MAX_PIX_CODE)                
			{
                cmplt_num = total_num;
                break;
			}
			
			buf_index_bak = buf_index;
			buf_index += 2;			/* Ԥ����������ݸ�����λ�� */
			
			if (pixel_addr_ofst == 0)		/* ǰһ����Ԫ����ɲ���д�С��� */
			{
				temp_row = pixel_code[pixel_index++];
				temp_col = pixel_code[pixel_index++];		/* ȡ����ַ */
				temp_attrib = pixel_code[pixel_index++];	/* ȡ������ */
				
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
			
			disp_wr_cmd(buf_index_bak, temp_cmd, disp_buf);	/* ���͵������� */
			
			if (pixel_addr_ofst == 0)
			{
				pixel_index++;
				cmplt_num++;
			} 
			else 
			{
				break;		/* �������������˳������µ��´δ��� */
			}
		}
	}

    /* ��������� */
    if(cmplt_num >= total_num)
    {
        fg_pixel_finish = TRUE;
        spe_index = 0;
    }
}

/******************************************************
������disp_get_pixel
���ܣ�ȡ����
	��������ȣ��߶ȣ����ݣ��ָ���
	�ݲ�����еı仯
�������������������жϻ������Ƿ�д��; �ַ�����; 
	  ��ǰ��Ԫ��ƫ�Ƶ�ַ(����Ϊ�������)
����ֵ����ǰ��Ԫȡ�����/δ���
*****************************************************/
static void disp_get_pixel(U16 font_code, U16 *addr_ofst, U08 *disp_buf)
{
//	U16 font_addr;
	PTR_FAR_C08 buf;
	U08 i;
	U08 k = 0;
	U08 font_long, font_high;		/* ��Ԫ�Ŀ�Ⱥ͸߶�(�ֽ�Ϊ��λ) */
	static U16 font_byte_num;		/* ��Ԫ�ֽ��� */
//	BOOL fg_cmplt = TRUE;			/* ��ǰ��Ԫ��ɱ�־ */
	
	if (fg_cur_code_ov)			/* ��ǰ���볬��,����ʾ */
	{
		*addr_ofst = 0;
		return;
	}
	if (disp_is_bmp(font_code))		/* ͼƬ */
	{
		buf = bmp_list[font_code - BMP_BGN_B] + *addr_ofst;
		buf += 2;		/* ��Ⱥ͸߶���ռλ�� */
	}
	else							/* ���� */
	{
		buf = hz16 + (font_code - CH_BGN_B)*32 + *addr_ofst;
	}
	
	if (*addr_ofst == 0)		/* �ϸ���Ԫ����� */
	{
		disp_cal_dim(font_code, &font_long, &font_high);
		font_byte_num = (U16)font_long * font_high * 8;
		
		disp_buf[buf_index++] = font_long;	/* ��д��Ⱥ͸߶� */
    	disp_buf[buf_index++] = font_high;
	}
	
	while(*addr_ofst + k < font_byte_num)		/* �õ�Ԫδ���� */
	{
    	if (buf_index < MAX_DISP - 8)		/* ÿ������д8���ֽ� */
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
    	*addr_ofst = 0;				/* �õ�Ԫ��ɣ�ƫ�Ƽ�¼���� */
    }
    else
    {
    	*addr_ofst += k;
    }
}

/******************************************************
������disp_get_num
���ܣ������λ���֡�
	���ΪASCII���ͼƬ��š�
����������; ��ʾ����(λ��); ����; �����ַ
����ֵ����Ч���ݸ���
*****************************************************/
static void disp_get_num(U16 v, U08 num, U08 *buf)
{
	U08  i;
	U16 temp_value;		/* ��ʱ���� */
//	U08 vld_num = 0;
	
	temp_value = v;
	for (i=0; i<num; i++) 
	{
		buf[num-i-1] = temp_value % 10;
		temp_value /= 10;
	}
}

/******************************************************
������disp_value
���ܣ���ʾ����С���㴦��
��������ֵ; ��Ч����λ��; ���� 
����ֵ����ʾ����λ��
*****************************************************/
static U08 disp_value_dot(U08 *buf_src, U08 v_num, U08 *max_num, U16 attrib)
{
	U08 i;
	U08 dot_pos;		/* С����λ�� */
	U08 dest_num;
	U08 temp_buf[MAX_VALUE_NUM];
	U08 zero_num = 0;	/* ��Ч0�ĸ��� */
		
	dot_pos = ((attrib & DNS_FLOAT_MASK) >> DNS_FLOAT_BIN_NUM);	/* С����λ�� */
	dest_num = v_num;		/* ��ʾλ�� */
	for (i=0; i<dest_num; i++)
	{
		temp_buf[i] = buf_src[i];	/* ��ʼ�� */
	}
	
	if (dot_pos >= v_num)	dot_pos = v_num-1;	/* �����ж� */

	if (dot_pos > 0)	/* С����ʾ */
	{
		for (i=0; i<dot_pos; i++)
		{
			temp_buf[v_num-i] = temp_buf[v_num-i-1];
		}
		temp_buf[v_num - dot_pos] = '.';		/* ����С���� */
		
		for (i=0; i<v_num-dot_pos-1; i++)	/* ȥ�������Чλǰ�����0 */
		{
			if (temp_buf[i] == 0)	zero_num++;
			else					break;
		}
				
		dest_num++;
		(*max_num)++;
	}
	else if ((attrib & DNS_ZERO) == 0)	/* ������ʾ���Ҳ���ʾǰ��0 */
	{
		for (i=0; i<v_num-1; i++)	/* ���ǰ��0(���һ��0Ҫ����) */
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
������disp_value_neg
���ܣ���ʾ����
��������ֵ; ��Ч����λ��; ���λ��; ���� 
����ֵ����
*****************************************************/
static void disp_value_neg(U08 *buf_src, U08 *vld_num, U08 *max_num, BOOL fg_neg)
{
	U08 i;
		
	if (fg_neg)		/* ��Ҫ��ʾ���� */
	{
		if (*max_num > MAX_VALUE_NUM-1)		*max_num = MAX_VALUE_NUM-1;
			
		for (i=0; i<*vld_num; i++)
		{
			buf_src[*max_num-i] = buf_src[*max_num-i-1];
		}
		buf_src[*max_num - *vld_num] = '-';		/* ��Ӹ��� */
		
		(*vld_num)++;
		(*max_num)++;
	}
}

/******************************************************
������disp_cal_dim
���ܣ����㵥Ԫ��Ⱥ͸߶ȡ�
����������; ���ָ��; �߶�ָ��; �ֿ�ָ��
����ֵ����
*****************************************************/
static void disp_cal_dim(U16 font_code, U08 *ch_long, U08 *high)
{
	PTR_FAR_C08 buf;
	
	if (font_code < ADDR_ASC_END)
	{
		*ch_long = AC_LONG;
		*high = AC_HIGH;
	}
	else if (disp_is_bmp(font_code))	/* ͼƬ */
	{
		if (font_code >= bmp_end_f[hmi_t_num])
		{
    		buf = bmp_list[font_code - BMP_BGN_B];
    		*ch_long = buf[0] >> 3;
    		*high = buf[1] >> 3;
    	}
    	else 		/* ���ֿ��ڣ��ú��ֳߴ����(��ʱʵ���ϲ�ʹ�ã�ֻΪ��ֹȡ����Ч����) */
    	{
    		*ch_long = HZ_LONG;
			*high = HZ_HIGH;
 		}
	}
	else 								/* ���� */
	{
		*ch_long = HZ_LONG;
		*high = HZ_HIGH;
	}
}

/******************************************************
������disp_num_code
���ܣ�����ת��Ϊ���롣
��������������(����)��ַ; ��ʾ����(λ��); �������(����)��ַ
����ֵ����
*****************************************************/
static void disp_num_code(U08 *buf_src, U08 num, U08 *buf_dest)
{
	U08  i;
	U08  TblHex[] = "0123456789abcdef";
	U08 temp_num;				/* ��ʱ���� */
	
	temp_num = num;
	if (temp_num > MAX_VALUE_NUM-1)		temp_num = MAX_VALUE_NUM-1;
	
	for (i=0; i<temp_num; i++)
	{
		if (buf_src[i] < 16)	buf_dest[i] = TblHex[buf_src[i]];
		else 					buf_dest[i] = buf_src[i];	/* �����֣�����ԭֵ */
	}
	
	buf_dest[temp_num] = '\0';
}

/******************************************************
������disp_mutual_abnormal
���ܣ�ƥ���쳣����
��������
����ֵ����
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
    else if(fg_f_need_update)   /* ����ǰ����Ҫ���� */
    {
        disp_string(update_need_f, 0, 0, DTS_CLR_HEAD|DTS_CLR_TRAIL, disp_buf);
    }
	
	if (fg_cur_code_ov)
	{
		disp_string(code_ov, 16, 0, DTS_CLR_HEAD|DTS_CLR_TRAIL, disp_buf);
	}
	if (fg_disp_overflow)
	{
		//fg_flash_all = TRUE;		/* ���� */
		disp_string(buffer_ov, 32, 0, DTS_CLR_HEAD|DTS_CLR_TRAIL, disp_buf);
	}
#endif	
	if (Main_Key[hmi_t_num] == KEY_STOP)		/* �˳� */
	{
		fg_first_commu = TRUE;
		fg_cur_code_ov = FALSE;
		fg_disp_overflow = FALSE;
		fg_flash_all = TRUE;
        if (fg_f_need_update)
        {
            fg_f_need_update_has_remind = TRUE;/* ������һ�� */
        }
	}
}
/**********************************************
������disp_forbid_auto_lcd_on
���ܣ���ֹ�Զ�������
��������
����ֵ����
***********************************************/
static void disp_forbid_auto_lcd_on(void)
{
	cmd_sub[0] |= CMD_SUB_FORBID_AUTO_LCD_ON;
}
/******************************************************
������disp_led_off
���ܣ���Ļ������һ��ʱ���ް�������ʱ�ر���
��������
����ֵ���Ƿ��������
*****************************************************/
static BOOL disp_screen_dark(U08 *disp_buf)
{
	BOOL fg_screen_dark = FALSE;

	if (fg_disp_ver_back[hmi_t_num])	disp_forbid_auto_lcd_on();
	
	if (disp_is_code_cmplt()				/* �����Ѵ��� */
		&& cmplt_num >= total_num)			/* �����Ѵ��� */
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
		disp_lcd_on();	/* ���κΰ����Ϳ����� */
	}
#ifdef USE_NOT_LOCK
    cnt_no_operate_sec[hmi_t_num] = 0;
	disp_lcd_on();
#endif  
	
	return fg_screen_dark;
}

/******************************************************
������disp_is_value_need_flash
���ܣ��ж���ֵ�Ƿ���Ҫˢ��
��������
����ֵ����/��
*****************************************************/
static BOOL disp_is_value_need_flash(U16 attrib)
{
	BOOL fg_need = FALSE;
	
	if (fg_disp_all 							/* ȫ��ˢ�� */
		|| fg_disp_blink && (attrib & DCS_BLINK)/* ��˸ */
		|| fg_disp_value)						/* ��ֵˢ�� */
	{
		fg_need = TRUE;
	}
	
	return fg_need;
}
/******************************************************
������disp_is_str_in_font
���ܣ��жϱ����Ƿ���ǰ����ģ��
�������ַ�����
����ֵ����/��
*****************************************************/
static BOOL disp_is_str_in_font(U16 ch_code)
{
	BOOL fg_in_font = FALSE;
	
    if (disp_is_bmp(ch_code))			/* ͼƬ */
    {
    	if (ch_code < bmp_end_f[hmi_t_num])	fg_in_font = TRUE;
    }
    else								/* ���� */
    {
    	/* ��ǰ��,������ */
    	if (ch_code < ch_end_f[hmi_t_num])		fg_in_font = TRUE;
        /* ����,�ں��,������ */
        else if(ch_code >= CH_BGN_B && ch_code < CH_END_B)  fg_in_font = FALSE;
        /* ����,���ֿⷶΧ��,������,ǰ�彫��ʾ�����ַ� */
        else if(ch_code < CH_END)               fg_in_font = TRUE;
    }
	
	return fg_in_font;
}

/******************************************************************** 
  �� �� ��: void disp_blank16(U08 row, U08 column, U08 num, U08 attrib)
  
  ��  ��: ��ʾ�հ��ַ�	
  ��ڲ���: row:����Ϣ  column:����Ϣ  num:�հ��ָ��� attrib:�ַ�����
  �� �� ֵ:��
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
�� �� ��: void disp_index_list(U08 index_now, U08 index_max, U16 const *string_ptr, U08 attrib)
��	��:��ʾ���Ϸ�ҳ���·�ҳͼ��
��ڲ���: index_now:��ǰӦ������ʾĿ¼  index_max:�����ʾ����
index_min:��С��ʾ����attrib:���� 0:ÿ��������ʾ���� 1:ÿ��������ʾһ��
�� �� ֵ:��
***********************************************************************/
void disp_next_flg(U16 index_now, U16 index_max,U16 index_min,U16 attrib,U08 *disp_buf)
{
 U16 per_screen_msg;
 U16 fgup=0;  //��ʾ����ͼ��
 U16 fgdown=0; //��ʾ����ͼ��
 if(attrib>SCREEN_4)
 	per_screen_msg=SCREEN_4;
 else
   per_screen_msg=attrib;
 //ͼ����ʾ�ж�
 if(per_screen_msg==SCREEN_1)  //һ��Ļֻ��ʾһ������
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
 else           //һ��Ļ��ʾ�������
  {
   if((index_now<per_screen_msg)&&(index_max>per_screen_msg))
   {
     fgup=FALSE;
     fgdown=TRUE;
   }
  else if(index_now>=per_screen_msg)	 
	{
	 fgup=TRUE;
	   if((index_now/per_screen_msg)!=((index_max-1)/per_screen_msg))  //�������һ��
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
 //ͼ����ʾ
 if(fgup)
   disp_bmp(idx_bmp_up_8_16,ROW1,15*ENLARGE_LOW, DCS_NORMAL,disp_buf);	//��ʾ����
 else
   disp_blank16(ROW1,15,1,DCS_NORMAL,disp_buf);	 //������ϱ�ʾ��
 if(fgdown)
   disp_bmp( idx_bmp_down_8_16,ROW3+(per_screen_msg/SCREEN_4)*ENLARGE_ROW, 15*ENLARGE_LOW, DCS_NORMAL,disp_buf)  ;   //��ʾ����
 else	
   disp_string_ptr8(" ",ROW3+per_screen_msg/SCREEN_4,15*ENLARGE_LOW,DCS_NORMAL,disp_buf);  //������±�ʾ��
}

/******************************************************************** 
�� �� ��: void disp_index_list(U08 index_now, U08 index_max, U16 const *string_ptr, U08 attrib)
��	��:��ʾĿ¼����
��ڲ���: index_now:��ǰ��ʾĿ¼��ʼλ�� max:�����ʾ���� *string_ptr:ָ��Ŀ¼������ָ��  attrib:����ÿ�������ʾ������
�� �� ֵ:��
***********************************************************************/
//void disp_index_list(U16 index_now, U16 index_max, const U08 *string_ptr[], U16 attrib, U08 *disp_buf)
//{
//	U16 i,attribute;
//	static U08 dis_index_re=0;
//	static U08 dis_index=0;
//	dis_index=index_now%attrib;  //���������ʾ��ʼλ��
//	for(i=0;i<attrib;i++)	//��ʾ��Ϣ
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
// disp_next_flg(index_now-dis_index, index_max,0,attrib,disp_buf);//��ʾ��ҳ��ʶ��
//}

/******************************************************
������disp_record_init
���ܣ���ʾ��¼��ʼ��
����������(��˸ʱ��Ҫһֱ����)
����ֵ����
*****************************************************/
static void disp_record_init(void)
{
	BOOL fg_flash_need_first = FALSE;
	
//	if (fg_disp_other)		return;		/* ���������ʱ������ʾ���� */
	
	if (fg_flash_all)		/* ȫ��ˢ�� */
	{
		if (!fg_disp_all)			fg_flash_need_first = TRUE;
		
		fg_disp_all = TRUE;
		fg_flash_all = FALSE;
	}
	else if (!fg_disp_all && cmplt_num >= total_num)	/* ȫ��ˢ������� */
	{
		if (!fg_disp_value)				/* ��ֵ����(һֱˢ��) */
		{	
    		fg_disp_value = TRUE;
    		if (!fg_disp_blink)		fg_flash_need_first = TRUE;	
    	}
    	
		if (!fg_disp_blink)				/* ��˸ */
    	{
    		fg_disp_blink = TRUE;
    		if (!fg_disp_value)		fg_flash_need_first = TRUE;
    	}
	}
	
	if (fg_flash_need_first)		/* ��Ҫˢ�£���Ϊ��һ�ε��ã�������ʼ����¼ */
	{
		spe_index = 0;
		total_num = 0;
		cmplt_num = 0;
		pixel_index = 0;
		pixel_addr_ofst = 0;
	}
}

/******************************************************
������disp_is_code_cmplt
���ܣ��жϱ����Ƿ���ʾ���
��������
����ֵ����/��
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
������disp_bound_code_check
���ܣ��߽������
��������
����ֵ���������쳣ֵ
*****************************************************/
static U08 disp_bound_code_check(void)
{
	U08 result = CD_NORMAL;
	
	/* ע�⣬�����ȼ����������������ʱ���ж�ǰ�� */
	if (CH_END_B < CH_BGN_B || BMP_END_B < BMP_BGN_B)		/* �������Ƚ� */
	{
		
		result = CD_ABN_BACK;
	}
	/* ��屣���ֿ�,û�е�����������Ŵ����,����Ҫ��� */
//  else if (ch_end_f[hmi_t_num] < CH_BGN_B || bmp_end_f[hmi_t_num] < BMP_BGN_B)    /* ǰ���������ͺ����ʼ����Ƚ� */
//  {
//      result = CD_ABN_MUTUAL;
//  }	
	return result;
}

/******************************************************
������disp_cur_code_check
���ܣ���ǰ������Ч�Լ��
��������
����ֵ���������쳣ֵ
*****************************************************/
static void disp_cur_code_check(U16 font_code)
{
	if (disp_is_bmp(font_code)) 
    {
    	if (font_code >= BMP_END_B && font_code >= bmp_end_f[hmi_t_num])	/* ��ǰͼƬ���볬���ж� */
    	{
    		fg_cur_code_ov = TRUE;
    	}
    }	
    else
	{
	    /* �������ֿⷶΧ�ڣ�����Ϊ��Ч��������ʾ���볬�� */
        if (font_code >= CH_END)
        {
            fg_cur_code_ov = TRUE;
        }
//      if ((font_code >= ch_end_f[hmi_t_num] && font_code < CH_BGN_B)      /* ���ֽ���ǰ���ֿ�ͺ���ֿ�֮��,�󳬳���Χ */
//          || font_code >= MAX(CH_END_B, ch_end_f[hmi_t_num]))
            
//    	if (font_code >= CH_END_B && font_code >= ch_end_f[hmi_t_num])		/* ��ǰ���ֱ��볬���ж� */
//  	{
//  		fg_cur_code_ov = TRUE;
//  	}
    }
}

/******************************************************
������disp_wr_cmd
���ܣ�д���	
���������ݸ���; ����; ��ʾ����
����ֵ����
*****************************************************/
static void disp_wr_cmd(U08 index_bak, U08 trans_cmd, U08 *disp_buf)
{
	if (buf_index - index_bak > 2)	/* ������Ч���� */
	{ 
		disp_buf[index_bak] = trans_cmd;
		disp_buf[index_bak+1] = buf_index - index_bak - 2;	/* ��Ч���ݸ��� */
	}
	else 
	{
		buf_index = index_bak;		/* ����Ч����ʱ�ָ�buf_index��ֵ */
	}
}

/**********************************************
������disp_clr_snd
���ܣ�����������
��������
����ֵ����
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
������disp_lcd_off
���ܣ��ر���
��������
����ֵ����
***********************************************/
static void disp_lcd_off(void)
{
	cmd_sub[0] |= CMD_SUB_LCD_OFF;
}

/**********************************************
������disp_lcd_on
���ܣ�������
��������
����ֵ����
***********************************************/
static void disp_lcd_on(void)
{
	cmd_sub[0] |= CMD_SUB_LCD_ON;
}

/******************************************************
������disp_screen_power
���ܣ���ʾ������ʱ�жϡ�
		����5����ͨѶʱ����ʾ�����µ���ʱ��	
��������
����ֵ����
*****************************************************/
static void disp_screen_power(void)
{
	if (cnt_non_commu[hmi_t_num] > 2)			cnt_screen_power[hmi_t_num] = SCREEN_POWER_TIME;
	if (!GET_BIT(hmi_commu_error, hmi_t_num))	cnt_non_commu[hmi_t_num] = 0;	/* ��ͨѶ���ϲ����� */
}

/**********************************************
������disp_cmd_simple
���ܣ�������(ָ������ʾ���ݣ������е���Ϣ������)
��������
����ֵ����
***********************************************/
static void disp_cmd_simple(U08 *disp_buf)
{
	U08 i;
	U08 data_num = 0;
	U08 buf_index_bak = buf_index + 2;	/* Ԥ����������ݸ�����λ�� */
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
������disp_is_bmp
���ܣ��жϵ�ǰ�����Ƿ����Զ���ͼƬ
����������
����ֵ��TRUE:�� / FALSE:��
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
������disp_value_cpmt_blank
���ܣ�������ʾʱ����ո�
��������������ַ; ��ʼ��ţ��������
����ֵ����
***********************************************/
static void disp_value_cpmt_blank(U08 *buf_src, U08 vld_num, U08 max_num, U16 attrib)
{
	U08 i;
	U08 blank_bgn, blank_end;	/* ���ո����ʼ������λ�� */
	U08 blank_num = 0;			/* �ո���� */
	
	if (vld_num > max_num)		return;
	blank_num = max_num - vld_num;
	
	if (attrib & DTS_RIGHT)		/* ������ʾ����߲��ո� */
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
    		buf_src[i] = buf_src[blank_num + i];	/* ���ְ��˵������ */
    	}
	}
	
	for (i=blank_bgn; i<blank_end; i++)
	{
		buf_src[i] = ' ';		/* ���ո� */
	}
}

/******************************************************
������disp_is_text_need_flash
���ܣ��ж��ı��Ƿ���Ҫˢ��
��������
����ֵ����/��
*****************************************************/
static BOOL disp_is_text_need_flash(U16 attrib)
{
	BOOL fg_need = FALSE;
	
	if (fg_disp_all 											/* ȫ��ˢ�� */
		|| fg_disp_blink && (attrib & (DCS_BLINK|DCS_FLASH)))	/* ��˸��һֱˢ�� */
	{
		fg_need = TRUE;
	}
	
	return fg_need;
}

/******************************************************
������disp_clk_syn
���ܣ�ʱ��ͬ��
��������
����ֵ����
*****************************************************/
static void disp_clk_syn(U08 *disp_buf)
{
	BOOL fg_need = FALSE;
	U08 *buf_0, *buf_1;
	U08 i;
	
	buf_0 = (U08 *)&clock[HMI_1];
	buf_1 = (U08 *)&clock[HMI_2];
	
	if (hmi_commu_error == 0 && MAX_HMI > 1)		/* ��ͨѶ���ϲ���ʱ��ͬ�� */
	{
		/* �ж��Ƿ�ƫ���3���� */
    	for (i=MAX_RTC_ITEM-1; i>=RTC_HOUR; i--)
    	{
    		if (i == RTC_WEEK)		continue;	/* ���ڲ���Ϊ�ж����� */
    			
    		if (buf_0[i] != buf_1[i])
    		{
    			fg_need = TRUE;
    			if (buf_0[i] < buf_1[i])	clk_base = HMI_2;	/* �����Ϊ��׼ʱ�� */
    			break;
    		}
    	}
    	if (!fg_need)		/* Сʱ���ϵ���ƫ����жϷ��� */
    	{
        	if (buf_0[RTC_MIN] > buf_1[RTC_MIN] && buf_0[RTC_MIN] - buf_1[RTC_MIN] > 3)
        	{
        		fg_need = TRUE;
        		clk_base = HMI_1;	/* �����Ϊ��׼ʱ�� */
        	}
        	else if (buf_1[RTC_MIN] > buf_0[RTC_MIN] && buf_1[RTC_MIN] - buf_0[RTC_MIN] > 3)
        	{
        		fg_need = TRUE;
        		clk_base = HMI_2;	/* �����Ϊ��׼ʱ�� */
        	}
        }
    
        /* �ж��Ƿ�ֻ��һ����������(�в���) */
        if (fg_need)
        {
            if (fg_active[HMI_1] && !fg_active[HMI_2]		/* HMI_1���� */
    			|| !fg_active[HMI_1] && fg_active[HMI_2])	/* HMI_2���� */
        	{
//        		debug_7[0]++;
        		if (fg_active[HMI_2])	clk_base = HMI_2;	/* �������Ϊ��׼ʱ�� */
        	}
        }
    	
    	if (fg_disp_back[hmi_t_num] && fg_disp_back[hmi_next])	/* �������Ȩ���ں�� */
    	{
        	if (fg_set_clk[hmi_next]				/* ���ڽ���������ʱ�� */
        		|| power_time  > 60 && fg_need)	/* ƫ���3���������ϵ�1���� *///cx
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
��  �� : disp_trans_sync()
��  �� : ����ͬ����Ϣ
��  �� : U08 *disp_buf : 
��  �� : ��
��  �� : ��
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
��  �� : disp_trans_ty_reset()
��  �� : ����Ϳѻ��λ����
��  �� : U08 *disp_buf : 
��  �� : ��
��  �� : ��
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
��  �� : disp_trans_ty_info()
��  �� : ����ͿѻWiFi����Ʒ��Ϣ
��  �� : U08 *disp_buf : 
��  �� : ��
��  �� : ��
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
��  �� : disp_trans_ty_data()
��  �� : ����ͿѻWiFi��������Ϣ
��  �� : U08 *disp_buf : 
         U08 dpid_idx  : DP����
��  �� : ��
��  �� : ��
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
��  �� : disp_trans_init()
��  �� : �����ʼ��
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void disp_trans_init(void)
{
    buf_index = 0;
}

