/******************************************************************************

          ��Ȩ���� (C), 2001-2016, �����а��յ��Լ����������޹�˾

 ******************************************************************************
  �� �� ��   : Logic.h
  �� �� ��   : ����
  ��    ��   : ��־��
  ��������   : 2016��11��14������һ
  ����޸�   :
  ��������   : Logic.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��11��14������һ
    ��    ��   : ��־��
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef __LOGIC_H__
#define __LOGIC_H__


#ifdef  _LOGIC_GLOBAL_
    #define LG_EXT
#else
    #define LG_EXT extern
#endif
/*-----------------����ͷ�ļ� -----------------------------*/



/*-----------------�ⲿ����˵��----------------------------*/



/*-----------------ȫ�ֱ���--------------------------------*/
/* �������� 2���֣�24·�� 00~03*/
//(0~1)
//#define SysSwInLoc0			((BIT8  *)&ITbl[0])	/* ����������0(����) */
//#define sys_dinput(n)       (*(U08 *)&SysSwInLoc0[0+n].byte)        /* ϵͳ���������� */

//#define SW_SWITCH    		(*(U08 *)&SysSwInLoc0[8].byte)          /* ϵͳ���� ��Χ 0~0X0F */


#define FBUS_ADDR			(*(U16*)&ITbl[0].word)		// ������ַ


/* �����¶����� 11���֣�11· 08~18*/
//#define SysTmpIn(n)			(*(I16*)&ITbl[8+(n)].word)		/* 6·�¶ȣ�0.1���϶� */
//
//// ���ص�������	2����	2· 20~21
//#define SysCurIn(n)			(*(U16*)&ITbl[20+(n)].word)		/*2· */
//
//#define SysAIIn(n)			(*(U16*)&ITbl[30+(n)].word)		/*2· */

typedef struct tagIO        /* ����������� */
{
//	U16  io_do[2];				/* ������DO */
//	U16  io_di[2];				/* ������DI */
//	U16  io_jmp;				/* ���ߵ�ַ */
//	U16  io_phase;				/* ����ģ�� */
	U16  io_init;				/* ��ʼ�� */
//	U16  io_uart_cnt[9];		/* ͨѶ���� */

//	I16  io_step_sv[EEV_ID_MAX];			/* ������� */
//	I16  io_step_pv[EEV_ID_MAX];			/* ������� */

	//I16  io_ai_cur[MAX_A];		/* ����AI */
	//I16  io_ai_temp[MAX_T];		/* �¶�AI */
	//U16  io_ad_cur[MAX_A];		/* ����AD */
	//U16  io_ad_temp[MAX_AD][3];	/* �¶�AD */

	U16   input[MAX_DI];			// 12λ����������ǿ�翪�����ڸ�λ
	U16   jmp[MAX_SW];				// ���뿪��
	U16   SwPlate;					// ����
	U16   output[MAX_DO];			// ������DO
	U08     phase;						// ����ģ��

//	I16     cur[MAX_CUR];				// ����AI
	I16     temp[MAX_TEMP];				// �¶�AI           0.01��
//	I16     ai[MAX_AI_NUM];             // ģ��������AI        // ai[0~3]��Ӧ����J5~J8
//	I16     ai_type[MAX_AI_NUM];        // ģ��������AI����
//	U16     cur_ad[MAX_CUR];			// ����AD

//	U16     temp_ad[MAX_AD][AD_TYPE];	// �¶�AD
} IO_DATA;

LG_EXT IO_DATA  gIO;
//#define		g_io		gIO

#ifdef USE_BMS_SOURCE
typedef struct{
//    I16 para[20];            /* ���� */
    struct {
        I16 temp[MAX_TEMP_NUM];
        I16 ai[MAX_AI_NUM];
        I16 cur[MAX_CUR_NUM];
        U16 DI[MAX_DI_W];
        U16 humi[2];/* N0004 */
    }src[1];                 /* ��Դ,֧��2��ģ�� */
}BMSSOURCE;
LG_EXT BMSSOURCE bms_source;
#endif

#ifdef USE_BMS_DEBUG
enum BMS_DEBUG_VAL_ID
{
    BDV_INVT_EXCEPT_INFO,   /* ��Ƶ�쳣��Ϣ */
    BDV_INVT_CMD_INVT_ID_1, /* ��Ƶ������-1#��Ƶ�� */
    BDV_INVT_CMD_INVT_FAN1, /* ��Ƶ������-1#ec��� */
    BDV_INVT_CMD_INVT_FAN2, /* ��Ƶ������-2#ec��� */
    BDV_PUMPF_WARN,         /* ��Ƶˮ�þ��� */
    BDV_PUMPF_ERR,          /* ��Ƶˮ�ù��� */
    BDV_PUMPF_ERR_CODE,     /* ��Ƶˮ�ù�����Ϣ */
    BDV_MAX,
};
typedef struct{
    I16 debug_val[BDV_MAX];              /* ���Ա��� */
}BMSDEBUG;
LG_EXT BMSDEBUG bms_debug;
#define SET_BMS_DEBUG_VAL(n, v)       (bms_debug.debug_val[(n)] = (v))
#define GET_BMS_DEBUG_VAL(n)          (bms_debug.debug_val[(n)])
#else
#define SET_BMS_DEBUG_VAL(n, v)    
#endif

/*-----------------ģ�鼶����------------------------------*/
//LG_EXT U08  pcb_type;		/* ģ���ʶ(0: ����; 1: �Ӱ�; 2: ��չ��) */

//#define TYPE_MASTER		0	/* ���� */
//#define TYPE_SLAVER		1	/* �Ӱ� */
//LG_EXT U08 val_IBH_bit;
LG_EXT U08 IBH_cfg_type;
LG_EXT U16 val_comp_num;
LG_EXT U08 sw_mac_model;
LG_EXT U08 sw_mac_model_EXIO;
LG_EXT U08 sw_mac_model_err;        /* �����ͺų����־λ */
#define SW_MAC_NUM 6		/* ʹ��6λ�����ʾ���� */

typedef struct tagDIO       /* ����������� */
{
    U16  debugstart;
	I16  io_step_sv[2];				/* ������� */
	I16  io_step_pv[2];				/* ������� */
	/* 1+2+2=5 */

	BIT16   in_input;				// �ڻ�������
	BIT16   out_input;				// ���������
	BIT16   in_output[2];			// �ڻ����
	BIT16   out_output;				// ������
    /* 1+1+2+1=5 */

	I16     in_temp[12];		    // �ڻ��¶�AI
	I16     out_temp[10];           // �ⲿ�¶�AI
	I16     press[3];       		// ģ���������Ӧѹ��ֵ
	I16     ptot[3];        		// ģ��������ѹ����Ӧ�¶�
	I16     freq_sv;        		// Ƶ���趨ֵ
	I16     freq_pv;        		// Ƶ�ʵ�ǰֵ
	I16     freq_temp;      		// ��Ƶ����ǰ�¶�
	I16     freq_volt;      		// ��Ƶ����ѹ
	I16     freq_cur;       		// ��Ƶ������
	I16     freq_max;       		// ��Ƶ�����Ƶ������
	I16     freq_area;      		// ��Ƶ����Ƶ����
	I16     in_superheatsv; 		// �������ȶ�Ŀ��
	I16     in_superheatpv; 		// �������ȶȵ�ǰ
	I16     out_superheatsv;		// �������ȶ�Ŀ��
	I16     out_superheatpv;		// �������ȶȵ�ǰ
	I16     fan_speed_sv[2];        // ���ת���趨ֵ
	I16     fan_speed_pv[2];        // ���ת�ٵ�ǰֵ
	I16     pump_speed_sv;        	// ˮ��ת���趨ֵ
	I16     pump_speed_pv;        	// ˮ��ת�ٵ�ǰֵ
    /* 12+10+3+3+11+2+2+2=45 */

	I16     debugbuf[200];  // ģ��������AI����
} IO_DEBUGDATA;

LG_EXT IO_DEBUGDATA  gdebugIO;
/*-----------------��������--------------------------------*/
LG_EXT U08  pcb_type;		/* ģ���ʶ(0: ����; 1: �Ӱ�; 2: ��չ��) */
LG_EXT U08  pcb_addr;		/* PCB���ַ(ly514ͨѶ) */

LG_EXT U16 hmi_power_time;
//LG_EXT U08 power_time;					/* �ϵ�ʱ�� */
#if 0
#define MAX_SYS_MS_COUNT	5
#define C_5MS		0
#define C_100MS		1
#define C_500MS		2
#define C_1S		3
#define C_1M		4

LG_EXT U16  SysMsCount[MAX_SYS_MS_COUNT];
#endif
#define	IsSecond		0		/* 1���־λ�������������㣬����ÿ���պ�ѭ��ֻ����һ�� */
#define	IsMinute		1
#define Is100Ms			2
LG_EXT U08  sys_status;
#if 0
LG_EXT BOOL  F_TIME_BASIC_MIN;  		/* һ�ֱ�־ */
LG_EXT BOOL  F_TIME_BASIC_MIN_BACK;
LG_EXT BOOL  F_TIME_BASIC_SEC;  		/* һ���־ */
LG_EXT BOOL  F_TIME_BASIC_SEC_BACK;
LG_EXT BOOL  F_TIME_BASIC_500MS;  	/* 500�����־ */
LG_EXT BOOL  F_TIME_BASIC_500MS_BACK;
LG_EXT BOOL  F_TIME_BASIC_100MS;  	/* 100�����־ */
LG_EXT BOOL  F_TIME_BASIC_100MS_BACK;

LG_EXT U08 flash_500ms;	/* 500ms��˸ */
#endif

LG_EXT U08 flash_500ms;	/* 500ms��˸ */
LG_EXT U08 flash_1s;	    /* 1s��˸ */

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

/* �汾��Ϣ */
//#define		val_pcb_num		2
#define MAX_PCB							8		// ���ģ����
#define MAX_COMP						2		// ��ģ�����ѹ��������������Ϊ1
#define USE_COMP						1		// ��ģ��ʹ��ѹ��������ֲ����ʱע���������
#define USE_EXIO                        1	/* ʹ������� */
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
#error "δ����ѹ����ʼ��"
#endif

LG_EXT BOOL fg_request_remote_upgrade;
LG_EXT U08 fg_accept_remote_upgrade;

/*-----------------�궨�� ---------------------------------*/
#define		wdg_clear()			/* ����HL017A,��оƬ����Ҫ���Ź� */


/*-----------------������Ϣ--------------------------------*/
#define	MAX_DEBUG		30
LG_EXT I16  debug_xx[MAX_DEBUG];


/*-----------------�ⲿ����ԭ��˵��------------------------*/


/*-----------------�ڲ�����ԭ��˵��------------------------*/
LG_EXT U16 USER_GetTime(U08 num);
LG_EXT void USER_time_basic_set(void);
LG_EXT void USER_time_basic_reset(void);

LG_EXT void USER_Init(void);
LG_EXT void USER_Main(void);
void USER_1ms_Deal(void);
#endif /* __LOGIC_H__ */
