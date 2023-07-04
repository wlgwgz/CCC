#ifndef MS_H
#define MS_H

#ifdef MS_GLOBAL
#define MS_EXT
#else 
#define MS_EXT extern
#endif

/**********************************************************/


/* ===============================================================
	MODBUS����ģ��
	�������			��������	��������	��Ӧ������  	����
	��ɢ������(2000)	��������	ֻ��		02				I/Oϵͳ�ṩ������������
	��Ȧ	(2000)		��������	��д		01 05 15		ͨ��Ӧ�ó���ı�������������
	����Ĵ���(500)	16-������	ֻ��		04				I/Oϵͳ�ṩ������������
	���ּĴ���(500)	16-������	��д		03 06 16		ͨ��Ӧ�ó���ı�������������
/ =============================================================== */
/***********************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
07.07.2005 RTE-MAB First Release
***********************************************************************************/


#define MODBUS_01       0x01		/* MODBUS������ */
#define MODBUS_02       0x02
#define MODBUS_03       0x03
#define MODBUS_04       0x04
#define MODBUS_05       0x05
#define MODBUS_06       0x06
#define MODBUS_08       0x08
#define MODBUS_0F       0x0f
#define MODBUS_10       0x10
#define MODBUS_17       0x17

#define MODBUS_NO_ERR       0    /* ���쳣 */
#define MODBUS_ERR_01       1    /* �Ƿ����� */
#define MODBUS_ERR_02       2    /* �Ƿ����ݵ�ַ */
#define MODBUS_ERR_03       3    /* �Ƿ�����ֵ */
#define MODBUS_ERR_04       4    /* ���豸���� */
#define MODBUS_ERR_05       5    /* ȷ��  �ͻ���(����վ)���Լ���������ѯ������ɱ�����ȷ���Ƿ���ɴ��� */
#define MODBUS_ERR_06       6    /* ���豸æ ������ѯ������ɱ�����ȷ���Ƿ���ɴ������к��ط����� */
#define MODBUS_ERR_99		99

/* ����ӹ�����  MODBUS_08 */
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

//#define MAX_PCB							2		//���ģ����   



// ģ�����ݽṹ˵��--------------------------------------------------------------------------------------------
// cell[ģ��� ].cell_cmd[i]:		i=0��ѹ��1���			i=1��ѹ��2���	i=MAX_COMP��ģ�����
// cell[MAX_PCB].cell_cmd[i]:		i=MAX_COMP��ϵͳ���
// cell[ģ��� ].cell_relay[i]:		i=0��ѹ��1�����			i=1��ѹ��2�����	i=MAX_COMP��ģ�鵥Ԫ�����
// cell[MAX_PCB].cell_relay[i]:		i=0��ϵͳ�����	
// cell[ģ��� ].cell_input[i]:		i=0��ѹ��1���룻			i=1��ѹ��2���룻	i=MAX_COMP��ģ�鵥Ԫ���룻
// cell[MAX_PCB].cell_input[i]:		i=0��ϵͳ���룻
// cell[ģ��� ].cell_status[i]:	i=0��ѹ��1״̬��			i=1��ѹ��2״̬��	i=MAX_COMP��ģ�鵥Ԫ״̬��
// cell[MAX_PCB].cell_status[i]:	i=0��ϵͳ״̬��
// cell[ģ��� ].cell_runtime[i]:	    i=0��ѹ��1����ʱ�䣻		i=1��ѹ��2����ʱ�䣻
// cell[MAX_PCB].cell_runtime[i]:	i=0�������ۼ�����ʱ�䣨���棩��
// cell[ģ��� ].cell_error[i]:		ģ��_ѹ���͵�Ԫ����
// cell[MAX_PCB].cell_error[i]:		ϵͳ��ع���
// cell[ģ��� ].cell_pv[i]:		ģ��_ѹ���͵�Ԫģ������
// cell[MAX_PCB].cell_pv[i]:		ϵͳ���ģ������
// -------------------------------------------------------------------------------------------------------------

#define RLY_BIT_NUM			16		/* ���������λ��(�����ֽڱ������ֱ���) */
#define IN_BIT_NUM			16		/* ���������λ��(�����ֽڱ������ֱ���) */
#define ERR_BIT_NUM			16		/* ���ϱ�����λ��(�����ֽڱ������ֱ���) */

#define MAX_CMD                  (MAX_COMP+1)
//#define MAX_CMD                  (2)
#define MAX_RELAY                (MAX_COMP+1)
#define MAX_INPUT                (MAX_COMP+1)
#define MAX_STATUS               (MAX_COMP+1)
//#define MAX_RUNTIME               MAX_COMP	    // ��Ԥ����2��ʱ�������	//NOTE-CXW ��ȷ�� 
#define MAX_RUNTIME               2	    // ��Ԥ����2��ʱ�������	//NOTE-CXW ��ȷ�� 
#define MAX_ERR                  ((USE_COMP*2+1)*2)	// 
#define MAX_PV                   60	// ��ͬ�����ģ��������˵���顢��λ��������

#define MAX_PV_WORD MAX_PV
#define MAX_ERR_WORD MAX_ERR

#define MAX_FREQINFO             16
typedef struct tagINVT
{
	I16 info[MAX_FREQINFO];
    U08 error_now;		        // ��ǰ��Ƶ���Ϻ�
    U08 bak;				    /* 2023-1-9������bakĿ���Ǵ���16λ���룬���Ĵ˽ṹ��ʱע����� */
} INVT_DATA;

//#define MAX_CMD                  3
//#define MAX_RELAY                3
//#define MAX_INPUT                3
//#define MAX_STATUS               3
//#define MAX_RUNTIME              3		// �˴���ֵ���б仯��Ҫע��MODBUS.c�к���sys_information()�Ƿ����޸�
//#define MAX_ERR                  4
//#define MAX_PV                   10		// �˴���ֵ���б仯��Ҫע��MODBUS.c�к���sys_information()�Ƿ����޸�
typedef struct tagCELL		/* ����: ������Ԫ�صĴ�С���������޸�! */	// �˴��ṹ���б仯��Ҫע��MODBUS.c�к���sys_information()�Ƿ����޸�
{
	U16 cell_cmd[MAX_CMD];			/* ���� */
	U16 cell_relay[MAX_RELAY];		/* ��������� */
	U16 cell_input[MAX_INPUT];		/* ���������� */
	U16 cell_status[MAX_STATUS];	/* ״̬ */
	U16 cell_runtime[MAX_RUNTIME];	/* �ۼ�����ʱ�� */
	U16 cell_error[MAX_ERR];		/* ���� */
	I16 cell_pv[MAX_PV];			/* ģ���� */
	INVT_DATA cell_invt[INVT_ID_MAX];	// ��Ƶ��
} CELL_DATA;
MS_EXT CELL_DATA  cell[MAX_PCB + 1];	/* ���һ��Ϊϵͳ�� */
MS_EXT CELL_DATA  *pCell;
MS_EXT CELL_DATA  *pmac;

#define MAX_CELL_BYTE	(sizeof(CELL_DATA)/sizeof(U08))
#define CELL_SIZE		(sizeof(CELL_DATA) / sizeof(U16))		 
#define CELL_BITS   (((sizeof(CELL_DATA)/sizeof(U16))-MAX_PV-MAX_CMD-(sizeof(INVT_DATA)/sizeof(U16)*INVT_ID_MAX))*16)

MS_EXT U16 bak_err_out[MAX_PCB+1][MAX_ERR];	/* ��ʷ�����жϱ��� */
MS_EXT BOOL	Link_remeber_on;	// ��������ڶ�ͨ���Ͽ�ǰ��״̬�Ǵ�����FALSE���������У�TRUE��
/* �ⲿ�������� */
MS_EXT U08  fg_error;	/* �����ܵı�ʶ */

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

//#define MAKEWORD(b, a)      ((U16) (((U08)(a)) | ((U16)((U08)(b))) << 8)) // Types.h��
#define MAKELONGWORD(b, a)      ((U32) (((U16)(a)) | ((U32)((U16)(b))) << 16))

//#define ASM_NOP				_asm("nop")

//#define	ENABLE_INTERRUPT   _asm("rim")	  //����ȫ���ж�
//#define	DISABLE_INTERRUPT  _asm("sim")	  //��ֹ�ж�

MS_EXT U16 self_comp_num;					// ģ��ѹ���� 
MS_EXT U08 use_phase_lack;					// ȱ��ʹ�÷�

MS_EXT U16  SysMsCount[MAX_SYS_MS_COUNT];

MS_EXT BOOL  F_TIME_BASIC_MIN;  		/* һ�ֱ�־ */
MS_EXT BOOL  F_TIME_BASIC_MIN_BACK;
MS_EXT BOOL  F_TIME_BASIC_SEC;  		/* һ���־ */
MS_EXT BOOL  F_TIME_BASIC_SEC_BACK;
MS_EXT BOOL  F_TIME_BASIC_500MS;  		/* 500�����־ */
MS_EXT BOOL  F_TIME_BASIC_500MS_BACK;
MS_EXT BOOL  F_TIME_BASIC_100MS;  		/* 100�����־ */
MS_EXT BOOL  F_TIME_BASIC_100MS_BACK;

MS_EXT BOOL  F_TIME_SWITCH_1S;			// 1���л���־���ж��д���	 


//MS_EXT BOOL	remeber_on;

MS_EXT U16 power_time;					/* �ϵ�ʱ�� */
MS_EXT U08 g_screen_clc_time;   		//����ʱ�䵥λ��


MS_EXT U08 fg_seep_crc_err;				/* ���ܲ�����ȡ���� */
MS_EXT U16 fg_seep_crc_iderr;		    /* ���ܲ�����ȡ��������� */

MS_EXT U08 fg_seep_overflow;			/* ���ܲ������� */
MS_EXT U16 fg_seep_overflow_iderr;		/* ���ܲ������������ */
MS_EXT U16 fg_seep_overflow_item;		/* ���ܲ������������ */

MS_EXT U08 fg_limit_crc_err;			/* ϵͳά��������ȡ���� */
MS_EXT U08 fg_limit_overflow;			/* ϵͳά���������� */

//MS_EXT U08 debug_buf[50];			/* ���ܲ������� */

MS_EXT U16	cmd_modify;			/* �����޸ı�ʶ */
MS_EXT BOOL	cmd_broadcast;		/* ����㲥 */
MS_EXT U16  cmd_normal[8];		/* �û����� */
MS_EXT U16  cmd_system[8];		/* ϵͳ���� */
MS_EXT U16 bro_cell_cmd[MAX_CMD];	/* ����ͨѶЭ��Ĺ㲥���� */

MS_EXT U16 commu_m_password_broadcast_interval;
MS_EXT BOL fg_trans_control_sta[MAX_PCB];
MS_EXT U16 nor_cell_cmd[MAX_PCB][MAX_CMD];	/* ����ͨѶЭ��Ĺ㲥���� */

//MS_EXT U08	version_info[MAX_PCB][32];	/* �汾��Ϣ */

#define CTRL_INFO_NUM 80
MS_EXT U16	ctrl_info[CTRL_INFO_NUM];	/* ������Ϣ */	 
MS_EXT U16	assist_info[10];	/* ����������Ϣ */
MS_EXT U16	pcb_comp_num[MAX_PCB];	/* ѹ������ */

/* BMSHMI */
typedef struct
{
    /* λ */
    U16	info_bit[6];
    
#if 0
    /* �� */
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

#define INV_ERR_PARA_INFO_NUM (FP_MAX+1)    //�����ѹ��������һ����Ƶ������ַ����ֺ�������һ
MS_EXT I16  inv_err_para_info[INV_ERR_PARA_INFO_NUM];  //��Ƶ���ϲ�����Ϣ
//MS_EXT U16	history_error[50];	/* ��ʷ������Ϣ */

MS_EXT I16	m_motor_sv_sheat;	/*��·���ù��ȶ� */	 
MS_EXT I16	m_motor_pv_sheat;	/*��·��ǰ���ȶ� */	 
MS_EXT I16	s_motor_sv_sheat;	/*��·���ù��ȶ� */	 
MS_EXT I16	s_motor_pv_sheat;	/*��·��ǰ���ȶ� */	 
MS_EXT I16	m_motor_judge_sheat;	/*�ж���·���ȶ� 1:�������ȹ��ȶ� 2:�������ȹ��ȶ�*/	 
MS_EXT I16	s_motor_pvnow_sheat;	/*������ǰ���ȶ� */	 


MS_EXT BOOL fg_sys_limit;		/* ʹ�����޵���־ */
MS_EXT U16 pre_limit_time;	/* ����ʱ�� */

#define PASS_SUPER		0		/* ��̬���� */
#define PASS_FAC		1		/* �������� */
#define PASS_SER		2		/* ά�������� */
#define PASS_PRO		3		/* �������� */
#define PASS_USER		4		/* �û����� */
#define PASS_LIMIT		5		/* ʹ���������� */
#define PASS_POP        6	    /* �������� */
#define MAX_PASS_GROUP	12 	    /* ����������� */

#define MAX_PASS_WORD		8 	/* ÿ�������ֽ��� */
MS_EXT U08	password[MAX_PASS_GROUP][MAX_PASS_WORD];	/* ���� */
//MS_EXT U08	superpassword[MAX_PASS_WORD];	/* �������� */

MS_EXT U16	passwd_modify;		/* �����޸ı�ʶ */
MS_EXT U16	passwd_broadcast;	/* ����㲥 */

// cell[MAX_PCB].cell_runtime[n]:   n�Ķ���
#define SYS_RUNTIME					0		//ϵͳ�ۼ����м�ʱ
//#define XX_RUNTIME				1		//����

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
