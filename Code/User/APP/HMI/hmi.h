/**********************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾
 All rights reserved. 

 �ļ����� : dm603.h
 �ļ����� : ͷ�ļ�����
 
 ������   : �����
 �������� : 2012.11.20

 ============================ �޸ļ�¼ =============================== 
 �޸���  : --
 ����汾: --
 �������: --
 �޸�����: --
  
**********************************************************************/

#ifndef	_DM603_H_
#define	_DM603_H_

#ifdef	DM603_GLOBAL
#define DM603_EXT
#else
#define DM603_EXT extern
#endif

//#define	KEY_RET_ENT_CHG		/* ȷ���ͷ��ؼ��Ի� �궨�� */
//#define	TEST_CHG_UART		/* �����ڸ���Ϊ���ؿ����ڲ��� */
//#define	ERROR_IS_RIGHT		/* ���Ͽ�����ʾ */

/* �������� */
#define KEY_NULL    	0x00
#define DM603_KEY_NULL	KEY_NULL	/* �ռ� */

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

#define	MAX_TIME					6				/* ��ʱ���� */
#define	MENU_TIME_SELECE_MIN		0
#define	MENU_TIME_SELECE_MAX		(MAX_TIME+2)
//#define DM603_KEY_FUNC	DM603_KEY_ONOFF	/* ����=���� �� */
DM603_EXT U08 dm603_cnt_key_updwn[MAX_HMI];		/* ���¼��������(100ms) */
DM603_EXT U08 dm603_cnt_key_space[MAX_HMI];		/* ����ʱ�������(100ms) */
//DM603_EXT U16 dm603_cnt_key_null[MAX_HMI];		/* �ް���ʱ����(1��) */
//DM603_EXT U08 dm603_fg_lock_clue[MAX_HMI];		/* ������ʾ��־ */

DM603_EXT U16 dm603_pv_test;		/* �ް���ʱ����(1��) */

DM603_EXT U16 key_continuous_time[MAX_HMI];		/* ����������ʱ */

DM603_EXT U32 dm603_alarm_sound_cnt[MAX_HMI];	/* ������ʱ */

#define ALARM_SOUND_SET_OFF				(0)	/* ���� */
#define ALARM_SOUND_SET_ON				(1)	/* ���� */
#define ALARM_SOUND_SET_10S_ONCE		(2)	/* ֻ��ʮ�� */
#define ALARM_SOUND_SET_10S_CYCLE		(3)	/* ����ʮ�� */
#define ALARM_SOUND_SET_MAX				(4)	// �������������ޣ��ڴ�֮ǰ׷��

#define ALARM_SOUND_CYCLE (QK_60*30)		/* ����ʮ������� */


DM603_EXT U08 key_delay_time_modbus[MAX_HMI];	// ����BMS���ý��水����ʱ(100ms)����ֹ����ʱ��Ӧ����һ����İ�����	 
DM603_EXT U08 key_delay_time_pass[MAX_HMI];		// ����   ������水����ʱ(100ms)����ֹ����ʱ��Ӧ����һ����İ�����

// ���水����ʱ
// ʹ��ע��:ֻ�԰���������Ч��if(key_delay_time[hmi_t_num]>=5){��Χֻ�ܰ�����������}��
//          �ǰ������жϲ�Ӧ���ڴ��ж��ڣ��������𲻱�Ҫ����ʱ��
DM603_EXT U08 key_delay_time[MAX_HMI];			// ������水����ʱ(100ms)����ֹ����ʱ��Ӧ����һ����İ�����
#define KEY_DELAY QK_5

DM603_EXT U08 dm603_sound[MAX_HMI];			/* ���� */
/* ��ϼ����� */
/* ��Ҫǿ��16����ת����������Main_Key�Ƚϻ������ע�� */
#define	DM603_KEY_OK_FUNC		((U16)(DM603_KEY_OK | DM603_KEY_FUNC))		/* ȷ��+���� �� */
#define	DM603_KEY_UP_DOWN		((U16)(DM603_KEY_UP | DM603_KEY_DOWN))		/* ����+���� �� */
#define	DM603_KEY_ONOFF_FUNC	((U16)(DM603_KEY_ONOFF | DM603_KEY_FUNC))	/* ����+���� �� */

/* ���Գ�����ϼ� */
#define	DM603_KEY_ONOFF_UP		((U16)(DM603_KEY_ONOFF | DM603_KEY_UP))		/* ����+���� �� */
#define	DM603_KEY_FUNC_UP		((U16)(DM603_KEY_FUNC | DM603_KEY_UP))		/* ����+���� ��ѩ��ģʽ��Ϲ��ܰ��� */

/* 3������� */
#define	DM603_KEY_ONOFF_3S			(0x00f1)	/* ���ؼ�3�� */
#define	DM603_KEY_OK_3S		    	(0x00f2)	/* ȷ����3�� */	 
#define	DM603_KEY_FUNC_3S			(0x00f3)	/* ���ܼ�3�� */
/* �û�����-->�������� */
#define  KEY_CMD_RUN             	0x0001	/* �������� */
#define  KEY_CMD_STOP            	0x0002	/* �ػ����� */
#define  KEY_CMD_RESET           	0x0100	/* ���ϸ�λ���� */
#define  KEY_CMD_SOUND_CANCEL     	0x0200	/* ��������(������Ϊ��λ������) */
#define  KEY_CMD_WARM_CANCEL     	0x0400	/* ȡ��Ԥ�� */
//#define  KEY_CMD_ENTER_FLOOR_DRYUP 	0x1000	/* �����ů��ɹ��� */
//#define  KEY_CMD_FORCE_IBH			0x2000	/* ǿ�ƿ������õ���� */
//#define  KEY_CMD_FORCE_TBH 			0x4000	/* ǿ�ƿ���ˮ������ */
//#define  KEY_CMD_FAST_HOTW	        0x8000	/* ��������ˮ */

/* ��������-->�������� */
#define  KEY_CMD_USER_PARA_INIT		0x0001   /* �û�������ʼ������ */
#define  KEY_CMD_FAC_PARA_INIT		0x0002   /* ���Ҳ�����ʼ������ */
#define  KEY_CMD_FAC_INVT_INIT		0x0004   /* ��Ƶ����ʼ������ */
#define  KEY_CMD_LIMIT_INIT		    0x0008   /* ϵͳά����ʼ������ */

//#define  KEY_CMD_FAC_ABILITY_TEST	0x0010   /* �������� */


#define  KEY_CMD_RUNTIME_INIT		0x0100   /* �ۼ�����ʱ���ʼ������ */
#define  KEY_CMD_SYSTIME_INIT		0x0200   /* ϵͳ����ʱ���ʼ������ */

#define  KEY_CMD_PASSWORD_INIT		0x4000   // �����ʼ��
#define  KEY_CMD_ALLDATA_INIT		0x8000   // ȫ�����ݳ�ʼ��

/* ��������-->�������� */
//#define  KEY_CMD_SYS_LIMIT			0x0001	/* ʹ�����޵����� */

DM603_EXT BOOL cmd_run;
DM603_EXT BOOL cmd_stop;

DM603_EXT BOOL key_run;
DM603_EXT BOOL key_stop;

/* �ж��� */
#define	ROW1		0
#define	ROW2		16
#define	ROW3		32
#define	ROW4		48

#define	RNUL		0xff

/* �ж��� */
#define	COL_END		128
#define	COL_BGN		0
#define	COL_ASC		8
#define	COL_CHR		16


/* �ж��� */
#define LINE1           0 
#define LINE2           1 
#define LINE3           2 
#define LINE4           3 
#define	LNULL		    0xff

/* �ж���(һ��ASCII�ַ�) */
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


/* ���涨�� */
enum DM603_FACE_ID
{
	FACE_OPEN	,				/* �������� */
	FACE_MAIN	,				/* ������   */
	FACE_MENU	,				/* ״̬��ѯ */
	FACE_MENU_TIME   ,			/* ��ʱ���� */
	FACE_MENU_TIME_SET,			/* ��ʱʱ������ */
	FACE_MENU_STATUS ,			/* ״̬���� */
	FACE_MENU_CLOCK  ,			/* ʱ�ӽ��� */
	FACE_MENU_VERSION,			/* �汾���� */
	FACE_MENU_FUNC   ,			/* �߼����� */
	FACE_MENU_FUNC_USER, 		/* �û����� */
	FACE_MENU_FUNC_DEFROST, 	/* �ֶ���˪ */
	FACE_MENU_CLOUD  ,			/* ���߹��ܽ��� */
	FACE_MENU_CLOUD_CMD  ,		/* ����������� */
	FACE_MENU_FCU  ,			/* ������Ϣ���� */
	FACE_CLOUD_CMD_RESET,		/* ���߸�λ*/
	FACE_CLOUD_CMD_CLR,			/* �����������*/
	FACE_CLOUD_CMD_TEST,		/* ���߳��� */
	FACE_CLOUD_CMD_RELEASE,		/* ���߽����*/
	FACE_MENU_CLOUD_STATUS,		/* ����״̬���� */
	FACE_MENU_CLOUD_INFO  ,		/* ����״̬���� */
	FACE_MENU_FUNC_UINT, 		/* ģ��ʹ������ */
	FACE_MENU_FUNC_RUNTIME, 	/* ����ʱ���ѯ */
	FACE_MENU_FUNC_HIS_ERR,		/* ��ʷ���ϲ�ѯ */
    FACE_MENU_INVT_PARA  ,	    /* ��Ƶ�������� */
	FACE_MENU_FUNC_SELECT, 		/* �߼�����ѡ�� */
	FACE_PASS_M_FAC, 			/* �޸ĳ������� */ 
	FACE_PASS_M_USER, 			/* �޸��û����� */ 
	FACE_PASS_M_LMT, 			/* �޸�ϵͳά������ */ 
	FACE_SET	,				/* �������� */
	FACE_PARA	,				/* �������� */
	FACE_PARA_SET	,			/* �������� */
	FACE_PARA_SET_VALUE	,		/* ���������� */
	FACE_PARA_PASS_M,  			/* �����޸� */
	FACE_PARA_INIT_P,			/* ��ʼ������ */
	FACE_PARA_INIT_C,   		/* ��ʼ��ѹ�� */
	FACE_INVT_INIT,             /* ��ʼ����Ƶ�� */
	FACE_SUB_MENU_SELECT, 		/* ѡ����� */
	FECE_SUB_ACT,				/* �ӽ�����崦�� */
	FACE_MACHINE_CODE	,		/* ������� */
	FACE_ERROR,					/* ���ϲ�ѯ */
	FACE_ALARM,					/* ������ѯ */
	FACE_PASS	,				/* ������� */
	FACE_LIMIT,					/* ʹ������ */
	FACE_LIMIT_SET,				/* ϵͳά������ */
	FACE_LIMIT_RUNTIME,			/* ����ʱ���ѯ */
	FACE_LIMIT_RUNTIME_INIT,    /* �ۼ�����ʱ���ʼ�� */   
	FACE_LIMIT_INIT,            // ��ʼ��ϵͳά��
	FACE_OFF	,				/* ���޵��� */
	FACE_PRE	,				/* �������� */
	FACE_DEBUG	,				/* ���Խ��� */
	FACE_MOD	,				/* ͨѶ���� */
	FACE_LOCK ,					/* �������� */
	FACE_LOCK1,					/* �������� */
	FACE_MENU_MODE,				/* ģʽ���� */
	FACE_MENU_USER_SHORTCUT,	/* ������ý��� */
	DM603_FACE_FORMT,			/* ����ͨѶ���� */
    FACE_STATUS_CHECK,          /*  */
	FACE_ONOFF	,				/* ���ػ�ȷ�Ͻ��� */
	FACE_INVT_EXCEPT_INFO,		/* ��Ƶ�쳣��Ϣ��ѯ */
	FACE_ENTER_FLOOR_DRYUP,		/* ��ů��ɹ��� */
	FACE_FORCE_IBH,				/* ǿ�ƿ������õ���� */
	FACE_FORCE_TBH,				/* ǿ�ƿ���ˮ������ */
	FACE_FORCE_HOT_WATER,		/* ǿ������ˮ(��������ˮ) */
	FACE_HOLIDAY_MODE_SET,		/* �ݼ�ģʽ���� */
	FACE_ABILITY_TEST,			/* �������� */

    FACE_PUNP_INIT,             // ���ճ�ʼ��
};

/* ����������� */
enum DM603_ACTION_ID
{
	SET_SV_ECONOMY_MODE,	// ���ý���ģʽ
	PUNP_INIT_ALL,          // ���ճ�ʼ��-ȫ��
	PUNP_INIT_PARA,         // ���ճ�ʼ��-����
	PUNP_INIT_PASSWD,       // ���ճ�ʼ��-����
    

};





enum DM603_MENU_ID {  DM603_MENU_CREATE, DM603_MENU_PROC, DM603_MENU_PAINT, DM603_MENU_OTHER, DM603_MENU_EXIT	  };

DM603_EXT U08 dm603_face[MAX_HMI];	/* ����ID */

/* �˵����涨�� */
#define		MENU_UP			0
#define		MENU_DOWN		1

//#define		MENU_SELECE_MIN		0
//#define		MENU_SELECE_MAX		(hmi_board_type[hmi_t_num]?5:6)




/* ��ʱ���� */
//#define		MENU_WR		0
//#define		MENU_RD		1
//#define		MENU_IN		2



#define		MENU_FUNC_SELECE_MIN		0
#define		MENU_FUNC_SELECE_MAX		4

/* ģ���������,���ڲ鿴���ϰ� */
#define	MAX_PCB_ALL		(MAX_PCB+1)//

/* ���������ӽ��� */
#define		MIN_PASS_BIT		0
#define		MAX_PASS_BIT		8

#define		PASS_CHG_UP			0
#define		PASS_CHG_DOWN		1



//#define		PARA_SELECE_MIN		0
//#define		PARA_SELECE_MAX		(hmi_board_type[hmi_t_num]?3:4)	 

/* ���������ӽ��� */
#define		PARA_SELECE_LARGE	0	/* ������ */
#define		PARA_SELECE_SMALL	1	/* ������ */
#define		PARA_SELECE_VALUE	2	/* ����ֵ */

/* ������ʾ���� */
#define		PARA_OPT_M		0	/* �� */
#define		PARA_OPT_V		1	/* ֵ */



/* ��ʷ���� */
#define		MAX_HIS_ERROR	10

/* ʹ���������ý��� */
//#define	FACE_LIMIT_SET 		0	/* ���޲�ѯ */
//#define	FACE_LIMIT_PASS   	1	/* ���޸��� */
//#define	FACE_LIMIT_RUN_SET		2	/* ���޳�ʼ�� */
//#define	FACE_LIMIT_SELECT 	4	/* ѡ����� */

#define		LIMIT_SELECE_MIN		0
#define		LIMIT_SELECE_MAX		3

#define MAX_VER_LINE        15  /* һ������ַ��� */
#define MAX_VER_LINE1       14  /* ��һ���ַ��� */
#define MAX_VER_LINE2(p)    MIN(STRLEN(p)-MAX_VER_LINE1, MAX_VER_LINE)

/* �������� */
#define DM603_CMD_RUN				(key_run = TRUE)						    /* ���� */
#define DM603_CMD_STOP				(key_stop = TRUE)						    /* �ػ� */
#define DM603_CMD_RESET				cmd_normal[0] |= KEY_CMD_RESET			    /* ��λ */
#define DM603_CMD_WARM_CANCEL		cmd_normal[0] |= KEY_CMD_WARM_CANCEL	    /* ȡ��Ԥ�� */
//#define DM603_CMD_RNTER_FLOOR_DRYUP	cmd_normal[0] |= KEY_CMD_ENTER_FLOOR_DRYUP  /* ��ů��ɹ��� */
//#define DM603_CMD_FORCE_IBH			cmd_normal[0] |= KEY_CMD_FORCE_IBH		    /* ǿ�ƿ������õ���� */
//#define DM603_CMD_FORCE_TBH			cmd_normal[0] |= KEY_CMD_FORCE_TBH  		/* ǿ�ƿ���ˮ������ */
//#define DM603_CMD=_FAST_HOTW	        cmd_normal[0] |= KEY_CMD_FAST_HOTW	        /* ��������ˮ */

#define DM603_CMD_DEFROST(x)		SET_BIT(cmd_normal[1], x)				    /* ǿ�Ƴ�˪ */
#define DM603_CMD_PARA_INIT			cmd_system[0] |= KEY_CMD_FAC_PARA_INIT	    /* ���Ҳ�����ʼ�� */
#define DM603_CMD_INVT_INIT			cmd_system[0] |= KEY_CMD_FAC_INVT_INIT	    /* ��Ƶ����ʼ�� */
#define DM603_CMD_LIMIT_INIT		cmd_system[0] |= KEY_CMD_LIMIT_INIT	        /* ϵͳά����ʼ�� */

//#define DM603_CMD_ABILITY_TEST		cmd_system[0] |= KEY_CMD_FAC_ABILITY_TEST   /* �������� */

#define DM603_CMD_RUNTIME_INIT		cmd_system[0] |= KEY_CMD_RUNTIME_INIT	    /* ѹ���ۼ�����ʱ���ʼ�� */
#define DM603_CMD_SYSTIME_INIT		cmd_system[0] |= KEY_CMD_SYSTIME_INIT	    /* ϵͳ�ۼ�����ʱ���ʼ�� */

#define DM603_CMD_PASSWORD_INIT		cmd_system[0] |= KEY_CMD_PASSWORD_INIT	    // �����ʼ��
#define DM603_CMD_ALLDATA_INIT		cmd_system[0] |= KEY_CMD_ALLDATA_INIT	    // �������ݳ�ʼ��
#define DM603_CMD_PASS_INIT(x)		password_reset(x)						    /* �����ʼ�� */


/* ������Ϣ�ṹ�� */
typedef struct tagSMALL {
	const U16 msg;			/* С���� */
	const U16 * const pmsg;	/* �ַ��� */
} SMALL;

/* �ֵ���Ϣ */
typedef struct tagPARADICT {
    const U16 *pmsg;    /* �ֵ�ָ�� */
    U08 items;          /* �ֵ������ */
}PARADICT;

/* �ⲿ�������� */
DM603_EXT void dm603_tasking(U08 *disp_buf);
DM603_EXT void dm603_init(void);
DM603_EXT void dm603_online_routine(U08 *disp_buf);
DM603_EXT U08 menu_error_find_next(U08 opt, U08 x, U08 err_id);
//DM603_EXT void dm603_face_menu_eeprom(U08 opt, U08 *tbl);
DM603_EXT void dm603_routine(U08 *disp_buf);

DM603_EXT U08 dm603_language;	    // DM603A����ѡ��  
DM603_EXT U16 ValidCode;            // ��֤�룬���㶯̬����ʹ��

//DM603_EXT PARAMITEM  ParamPri[15];


//#define val_pcb_num				1
#define m_val_comp_num(pcb_num)		((M_CM_GetStatus((pcb_num), MAX_COMP, CELL_COMP))>>4)	//��õ�Ԫѹ����
//#define MAX_COMP				2
//#define val_comp_num			2
///* �������Ե�16λ���壺*/
///* ��0~1λ: С���� */
//#define D_DOT1	 	0x0001	/* 1λС���� */
//#define D_DOT2	 	0x0002	/* 2λС���� */
//#define D_DOT3	 	0x0003	/* 3λС���� */
//#define D_DOTMASK	0x0003	/* ������ */
///* ��2~3λ: */
///* ��4~7λ: ��λ */
//#define UINT_0		(0<<4)		/* �� */
//#define UINT_C		(1<<4)		/* �¶�() */
//#define UINT_A		(2<<4)		/* ����(A) */
//#define UINT_PR		(3<<4)		/* ѹ��(bar) */
//#define UINT_PE		(4<<4)		/* �ٷֱ�(%) */
//#define UINT_F		(5<<4)		/* Ƶ��(HZ) */
//#define UINT_RS		(6<<4)		/* ת��(rps) */
//#define UINT_RM		(7<<4)		/* ת��(rpm) */
//#define UINT_S		(8<<4)		/* �� */
//#define UINT_M		(9<<4)		/* �� */
//#define UINT_H		(10<<4)		/* ʱ */
//#define UINT_D		(11<<4)		/* �� */
//#define UINT_ST		(12<<4)		/* �� */
//#define UINT_MASK	(0x000f<<4)	/* ������ */
///* ��8λ: */
//#define D_BIT		0x0100		/* λ���� */
///* ��9λ: */
//#define D_DE		0x0200		/* �²� */
///* ��10λ: */
//#define D_HIDE	 	0x0400		/* �������� */
///* ��11λ: */
//#define D_NEXCH	 	0x0800		/* �����޸ĸ��� */
///* ��12~14λ: Ȩ�޼���(�ɵ͵���) */
//#define D_USER		(1<<12)		/* �û����� */
//#define D_PRO		(2<<12)		/* �������� */
//#define D_SER		(3<<12)		/* ά�������� */
//#define D_FAC		(4<<12)		/* �������� */
//#define D_PUNP		(5<<12)		/* �������� */
//#define D_SAFEMASK	(0x000f<<12)	/* ������ */
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

#define MAC_TYPE21 			21				/* �๦����ˮ�� */
#define MAC_TYPE1			1				/* ����˫��ͷģ��� */
#define MAC_TYPE11			11				/* �ȱ���ˮ�� */
#define MAC_TYPE2			2				/* ˮ��˫��ͷģ��� */
#define MAC_TYPE3			3				/* �����Ļ�ͷģ��� */
#define MAC_TYPE12			12				/* ������ˮ�� */
#define MAC_TYPE_SELF		MAC_TYPE3		/* ������ */

/**************************************************/
/* ����Ϊ�������壬��ֲʱ��������嵽�����ļ��� */
#define	HIS_ERR_SIZE		(8)		                        /* ÿ����ʷ����ռ���ֽ���(1�ֽ�(ģ���)+1�ֽ�(���Ϻ�)+1�ֽ�(���Ӻ�)+5�ֽ�(ʱ��) */
#define	MAX_HIS_ERR_TBL		(HIS_ERR_SIZE*MAX_HIS_ERROR)	/* ��ʷ����ռ�����ֽ��� */
DM603_EXT U08 his_err_tbl[MAX_HIS_ERR_TBL];	                /* ��ʷ���� */

#define MAX_TIME_TBL	(3+3*MAX_TIME)
DM603_EXT U08 dm603_time_tbl[MAX_TIME_TBL];	    /* ��ʱ���� */

/* ��ʱ���ù��� */
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
DM603_EXT U08 dm603_holiday_tbl[HOLIDAY_MAX];	    /* �ݼ�ģʽ���� */

//DM603_EXT U08 dm603_time_tbl[MAX_TIME_TBL];	/* ��ʱ���� */

//DM603_EXT U16 debug_7[10];
DM603_EXT BOOL	f_blink_1S;			//1S��˸��־
DM603_EXT BOOL  f_blink_1S_MODE;	// ģʽ1���л���־
DM603_EXT void disp_time(void);

/*----------�³���-------------------*/

typedef struct tagMENUITEM		// �˵� 
{
	U08  MenuType;			// �˵�����
//	U08  MenuPassDis;		// ��ʾ���뼶�������ж��Ƿ���ʾ�ò˵���
	U16  MenuPassEnt;		// ��������Ȩ�ޣ������ж��Ƿ��������ò˵���
	U16  FuncId;			// ����ID	���ݲ˵����Ͳ�ͬ�����ܲ�ͬ(����ǲ��������ޱ�Ҫ��ǿ��ת��ΪI16��)
	const U16 MenuMsg;			// �˵���
	
	U16  MenuSign;			// �˵�����
} MENUITEM;

// MenuType	�˵�����
#define MENU_FACE		0	// �������Ĳ˵���FuncId��Ӧ�ӽ����ID
#define MENU_ONOFF		1	// ����ONOFF�����Ĳ˵���FuncId��ӦҪ���õĲ�����ִ�д˲˵������л��ò����Ĳ���ֵON/OFF��
#define MENU_ACTION		2	// ��������ִ�ж����Ĳ˵���FuncId��Ӧ������ID��ִ�д˲˵������л��ö��������л����棩



// MenuPass ���뼶��
#define PASS_NULL		0	// ��������
#define PASS_FUNC		((1<<PASS_USER)|(1<<PASS_SER)|(1<<PASS_FAC))		// ����Ȩ�ޡ����߼����ܽ���
#define PASS_PARA		((1<<PASS_SER)|(1<<PASS_FAC)|(1<<PASS_SUPER))		// ����Ȩ�ޡ����������ý���
#define PASS_LMT		((1<<PASS_LIMIT ))					// ����Ȩ�ޡ���ʹ���������ý���
// MenuSign	�˵�����

//#define M_HIDE	 	0x0400		// ��������

#define M_PASS	 	0x0001		// ����������


#define ITEM_PER_PAGE	3								// ÿҳ��ʾ�Ĳ˵��������4��������ʾ��ʾ�У�
#define MENUITEM_SIZE	((GROUP_MAX>10)?GROUP_MAX:10)	// �˵�����
DM603_EXT U08 Item_tbl[MAX_HMI][MENUITEM_SIZE];	//NOTE-CXW �Ƿ�ÿ����������

//#define ALLOW_MENU_ONOFF	1	// �ռ䲻��ʱ�ɽ�ֹ�˹���
//#ifdef ALLOW_MENU_ONOFF
//static U16 Item_value_tbl[MAX_HMI][ITEM_PER_PAGE];	// ���ڵ������ֵ��ʾ
//#endif	// end ALLOW_MENU_ONOFF

DM603_EXT U08 dm603_nextface[MAX_HMI];	// Ԥ����ת����һ���棬һ�����ڵ����������ʱ
DM603_EXT U16 dm603_PassNeed[MAX_HMI];	// ��ǰ���������Ҫ��Ȩ��


DM603_EXT U08 ParaGroup_Now[MAX_HMI];	// ��ǰ������


/* ��ʾʱ����Ϣ */
#define SEC			0X0001	// ��
#define MINU		0X0002	// ��
#define HR			0X0004	// ʱ
#define COLON_BINK	0X0080	// ð����˸
#define HMS_MASK	0X00FF	// ʱ��������

#define HHMM		(HR|MINU)// ʱ��
#define HHMMSS		(HR|MINU|SEC)// ʱ����


#define DAY			0X0100	// ��
#define MON			0X0200	// ��	
#define YY			0X0400	// �꣨2λ��
#define YYYY		0X0800	// �꣨4λ��		// Ӣ��ʱǿ����ʾΪ17/12/31 �������գ�
#define CHN			0X8000	// ������ʾ		
#define YMD_MASK	0XFF00	// ����������

#define YYMMDD		(YY|MON|DAY)// ������
#define YYYYMMDD	(YYYY|MON|DAY)// ������			


#define		CHAR_NUM			0X01	// ��������
#define		CHAR_ASCII			0X02	// ������ĸ

typedef struct tagKEY                               
{                                                   
    U16 value;  //��ֵ                              
    U16 time;   //����ʱ��                          
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

/* ����smarthmi */
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
