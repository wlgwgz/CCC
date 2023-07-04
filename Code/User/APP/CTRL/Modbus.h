/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: Modbus.h
 �ļ�����: 
 ������  : 
 ��������: 
*******************************************************************************/
#ifndef MODBUS_H
#define MODBUS_H

#ifdef MODBUS_GLOBAL
#define MODBUS_EXT
#else 
#define MODBUS_EXT extern
#endif

/* ϵͳ�ں�--Modbus�ռ䶨��(�ֵ�λ�������ֽ�) */
#define I_SIZE	64
#define Q_SIZE	8

/* ϸ��M��MTbl */
#define M_CORE_SIZE		512	/* ϵͳ�ں�����С */
//#define M_IO_SIZE		64	/* IO����С */
//#define M_WORK_SIZE		64	/* ��������С */	
//#define M_PARM_SIZE		64	/* ��������С */
//#define M_PROG_SIZE		64	/* ��������С */

//#define M_SIZE	(M_CORE_SIZE+M_IO_SIZE+M_WORK_SIZE+M_PARM_SIZE+M_PROG_SIZE)
#define M_SIZE	(M_CORE_SIZE)
//MODBUS_EXT BIT16 XDATA ITbl[I_SIZE];
//MODBUS_EXT BIT16 XDATA QTbl[Q_SIZE];
//MODBUS_EXT BIT16 XDATA MTbl[M_SIZE];
MODBUS_EXT BIT16 XDATA ModbusMemory[I_SIZE+Q_SIZE+M_SIZE];//xdata���ܳ���16K
#define ITbl (ModbusMemory)				
#define QTbl (ModbusMemory+I_SIZE)		
#define MTbl (ModbusMemory+I_SIZE+Q_SIZE)

/* �ж���ʱ��ͨ��Q������M���������˻�һ�����Q��M�����޶���ʱ��ֻ�ֿܷ����ʡ�
Q��M���ڵ�ַ���������ģ���M���������̫�󣬵���ͨ��Q�����ʲ���M����
��M������Ϊ64K�ֽڣ���Q�����岻��ͬһ��64K��Χ�� */
#define ACCESS_M_THROUGH_Q

#define M_CORE_OFFSET	0
#define M_IO_OFFSET		(M_CORE_OFFSET + M_CORE_SIZE)
//#define M_WORK_OFFSET	(M_IO_OFFSET + M_IO_SIZE)
//#define M_PARM_OFFSET	(M_WORK_OFFSET + M_WORK_SIZE)
//#define M_PROG_OFFSET	(M_PARM_OFFSET + M_PARM_SIZE)

#define CoreTbl			((BIT16 *)&MTbl[M_CORE_OFFSET])
//#define IOTbl			((BIT16 *)&MTbl[M_IO_OFFSET])
//#define WorkTbl			((BIT16 *)&MTbl[M_WORK_OFFSET])
//#define ParmTbl			((BIT16 *)&MTbl[M_PARM_OFFSET])
//#define ProgTbl			((BIT16 *)&MTbl[M_PROG_OFFSET])

/*******************************************************************************
MODBUS����ģ��
�������	��������	��������	��Ӧ������  ����
��ɢ������	��������	ֻ��		02			I/Oϵͳ�ṩ������������
��Ȧ	  	��������	��д		01 05 15	ͨ��Ӧ�ó���ı�������������
����Ĵ���	16-������	ֻ��		04			I/Oϵͳ�ṩ������������
���ּĴ���	16-������	��д		03 06 16	ͨ��Ӧ�ó���ı�������������
*******************************************************************************/
/* MODBUS������ */
#define MB_RD_COILS					0x01	/* ����Ȧ */
#define MB_RD_DISCRETE_INPUTS		0x02	/* ����ɢ������ */
#define MB_RD_HOLDING_REGISTERS		0x03	/* �����ּĴ��� */
#define MB_RD_INPUT_REGISTERS		0x04	/* ������Ĵ��� */
#define MB_WR_SINGLE_COIL			0x05	/* д������Ȧ */
#define MB_WR_SINGLE_REGISTER		0x06	/* д�������ּĴ��� */
#define MB_DIAGNOSTICS				0x08	/* ��·��ϲ��� */
#define MB_WR_MULTIPLE_COILS		0x0f	/* д�����Ȧ */
#define MB_WR_MULTIPLE_REGISTERS	0x10	/* д������ּĴ��� */
#define MB_RDWR_MULTIPLE_REGISTERS	0x17	/* �Զ��幦���룺��д */
#define MB_USER_DEFINED_DOWNLOAD	0x41	/* �û��Զ��幦����:�������� */
#define MB_USER_DEFINED_RD_BIGFILE 	0x44	/* �û��Զ��幦����:��ȡ������ */
#define MB_USER_DEFINED_KLV         0x47    /* �û��Զ��幦����:KLVЭ���д *//* N0004 */
#define MB_USER_DEFINED_EXIO_KLV    0x48	/* �û��Զ��幦����:EXIO_KLVЭ���д */

/* MODBUS�ӹ����� */
#define MB_0x41_SUB_FUNC_ALLOW_BOOT         0x0000      // ѯ���Ƿ������
#define MB_0x41_SUB_FUNC_ACCEPT_BOOT        0x0001      // ѯ���Ƿ��������
#define MB_0x41_SUB_FUNC_ENTER_BOOT         0x0010      // ִ�н�������
#define MB_0x41_SUB_FUNC_REQUEST_BOOT       0x0011      // ִ����������
#define MB_0x41_SUB_FUNC_ENTER_USB          0x0020      // ����USBģʽ
#define MB_0x41_SUB_FUNC_EXIT_USB           0x0021      // �˳�USBģʽ


#define ADDR_BROADCAST		0				/* �㲥��ַ */
#define ADDR_BRO_FBUS		0xf8			/* �����㲥��ַ */

/* MODBUS�쳣�� */
#define MB_NO_ERR       			0		/* MODBUS���쳣 */
#define MB_ILLEGAL_FUNCTION       	1		/* �Ƿ����� */
#define MB_ILLEGAL_DATA_ADDR       	2		/* �Ƿ����ݵ�ַ */
#define MB_ILLEGAL_DATA_VALUE       3		/* �Ƿ�����ֵ */
#define MB_SLAVE_DEVICE_FALUE       4		/* �ӻ��豸���ϣ��ӻ��޷����� */

#define PASSWORD_T_NUM  16//wlg20150429
#define PASSWORD_W_NUM  8
//MODBUS_EXT U08	password[PASSWORD_T_NUM][PASSWORD_W_NUM];	/* ���� */
//MODBUS_EXT U16	passwd_modify;		/* �����޸ı�ʶ */
//MODBUS_EXT U08	passwd_broadcast;	/* ����㲥 */
#define PWR_NONE		0		/* δ��¼ */
#define PWR_USER		1		/* �û� */
#define PWR_SALE		2		/* ������ */
#define PWR_PRO			3		/* ���̵���Ա */
#define PWR_SER			4		/* ����ά��Ա */
#define PWR_FAC			5		/* ���ҹ���Ա */
#define PWR_SPR			9		/* �������� */
#define PASS_LMT_1		10		/* ʹ�����޵�һ������ */

/* MODBUS��ַ���� */
MODBUS_EXT U08 controller_is_punp;		/* �Ƿ��ǰ��տ�����(���ʶ��) */
/* ��λ��(HMI��BMS)����: ��0x05��0x15 λ�ķ�ʽ���� */
#define ADDR_USER_CMD_BGN			0
#define USER_CMD_SIZE				((sizeof(cmd_normal) << 4) / sizeof(U16))
#define ADDR_USER_CMD_END			(ADDR_USER_CMD_BGN + USER_CMD_SIZE)
#define ADDR_FAC_CMD_BGN			128
#define FAC_CMD_SIZE				((sizeof(cmd_system) << 4) / sizeof(U16))
#define ADDR_FAC_CMD_END			(ADDR_FAC_CMD_BGN + FAC_CMD_SIZE)
/* cmd_mux */
#define ADDR_MUX_CMD_BGN			1024
#define MUX_CMD_SIZE				((sizeof(cmd_mux) << 4) / sizeof(U16))
#define ADDR_MUX_CMD_END			(ADDR_MUX_CMD_BGN + MUX_CMD_SIZE)
/* �ֶ����� */
#define ADDR_TEST_DO_BGN		    (2048)
#define TEST_DO_SIZE			    ((sizeof(test_data.wr.DO_sta) << 4) / sizeof(U16))
#define ADDR_TEST_DO_END		    (ADDR_TEST_DO_BGN+TEST_DO_SIZE)

/* DEBUG��Ϣ */
#define ADDR_DEBUG_BGN				0							    //������Ϣ��ַ��ʼ
#define DEBUG_SIZE					255								//������Ϣ��С
#define ADDR_DEBUG_END				(ADDR_DEBUG_BGN+DEBUG_SIZE)		//������Ϣ����

/* �汾�� */
#define ADDR_VER_BS_BGN				256		/* ��ʼ��ַ 0x100 */
#define VER_BS_SIZE					(sizeof(idu_mac_ver) / sizeof(U16))		/* ���ݸ��� */ 
#define ADDR_VER_BS_END				(ADDR_VER_BS_BGN + VER_BS_SIZE)		/* ������ַ */

/* ���ģʽ�汾,�����ֽ�˳�� */
#define ADDR_VER2_BGN               512
#define VER2_SIZE					(sizeof(idu_mac_ver) / sizeof(U16))		/* ���ݸ��� */ 
#define ADDR_VER2_END				(ADDR_VER2_BGN + VER2_SIZE)		/* ������ַ */

/* ������Ϣ */
#define ADDR_CTRL_BGN				768		/* ��ʼ��ַ 0x300 */
#define CTRL_SIZE					(sizeof(ctrl_info) / sizeof(U16))		/* ���ݸ��� */ 
#define ADDR_CTRL_END				(ADDR_CTRL_BGN + CTRL_SIZE)		/* ������ַ */
/* ������Ϣ */
#define ADDR_ASSI_BGN				896		/* ��ʼ��ַ 0x380 */
#define ASSI_SIZE					(sizeof(assist_info) / sizeof(U16))		/* ���ݸ��� */ 
#define ADDR_ASSI_END				(ADDR_ASSI_BGN + ASSI_SIZE)		/* ������ַ */
/* ������fg_test_run */
#define ADDR_TEST_RUN_BGN			910		/* ��ʼ��ַ 0x38E */
#define TEST_RUN_SIZE				(sizeof(fg_test_run) / sizeof(U16))		/* ���ݸ��� */ 
#define ADDR_TEST_RUN_END			(ADDR_TEST_RUN_BGN + TEST_RUN_SIZE)		/* ������ַ */
/* BMSHMI */
#define ADDR_BMSHMI_BGN				912		/* ��ʼ��ַ 0x390 */
#define BMSHMI_SIZE					(sizeof(bmsHMI.info_bit) / sizeof(U16))		/* ���ݸ��� */ 
#define ADDR_BMSHMI_END				(ADDR_BMSHMI_BGN + BMSHMI_SIZE)		/* ������ַ */

/* ѹ������ */
#define ADDR_COMPNUM_BGN            1000                            
#define COMPNUM_SIZE                MAX_PCB
#define ADDR_COMPNUM_END            (ADDR_COMPNUM_BGN + COMPNUM_SIZE)
/* ϵͳģ����Ϣ��Ϊ����ԭ����Э�飬��һ���ַ����  */
#define UNIT_MODE_SINGLE_NUM		0x0100			/* ����������ݸ��� */
#define UNIT_BIN_BIT_NUM			8				/* ���������ֶ�����λ�� */
#define ADDR_UNIT_MODE_BGN			0X400	        /* ��ʼ��ַ 0x400 */
#define ADDR_UNIT_MODE_END			(ADDR_UNIT_MODE_BGN + UNIT_MODE_SINGLE_NUM)	/* 1280 */

/* ģ����Ϣ  */
#define UNIT_SIZE					(UNIT_MODE_SINGLE_NUM * MAX_PCB)
#define ADDR_UNIT_BGN				(ADDR_UNIT_MODE_END)	        /* 1280 */
#define ADDR_UNIT_END				(ADDR_UNIT_BGN + UNIT_SIZE)		/* 1280 + 2048 = 3328 */

/* ģ����Ϣ,λ����(0x0300~0x1B00) */
#define UNIT_STEP_NUM           	0x0300          /* �������� */
#define UNIT_BITS              	 	CELL_BITS       /* ��ģ��ʵ�ʿɷ��ʳ��� */
#define ADDR_UNIT_BIT_BGN       	0x0300          /* ��ʼ���ʳ��� */
#define ADDR_UNIT_BIT_END       	(ADDR_UNIT_BIT_BGN + (UNIT_STEP_NUM*MAX_PCB))

/* ϵͳ��Ϣ  */   	
#define SYS_SIZE					UNIT_MODE_SINGLE_NUM
#define ADDR_SYS_BGN				(ADDR_UNIT_END)			        /* 3328 */
#define ADDR_SYS_END				(ADDR_SYS_BGN + SYS_SIZE)		/* 3328 + 256 = 3584 */

/* ϵͳ��Ϣ,λ����(0x0000~0x0300) */
#define SYS_BITS                	CELL_BITS
#define ADDR_SYS_BIT_BGN       	 	0x0000
#define ADDR_SYS_BIT_END        	(ADDR_SYS_BIT_BGN + SYS_BITS)

/* ����汾�����ģʽ�������ֽ�˳�� */
#define ADDR_VER_ODU_BGN            4000
#define VER_ODU_SIZE				(sizeof(odu_mac_ver) / sizeof(U16))		/* ���ݸ��� */ 
#define ADDR_VER_ODU_END			(ADDR_VER_ODU_BGN + VER_ODU_SIZE)		/* ������ַ */

/* ��Ƶ�����汾�����ģʽ�������ֽ�˳�� */
#define ADDR_VER_INVT_BGN            4256
#define VER_INVT_SIZE				(MAX_VER_BYTE/2)		                /* ���ݸ��� */ 
#define ADDR_VER_INVT_END			(ADDR_VER_INVT_BGN + VER_INVT_SIZE)		/* ������ַ */

//==========================================================================================================


/* FBUS ͨѶ��Э���ַ����
 * ʹ�� FBUS_XX ��ַǰ׺
 */

/* ������(FBUS)���� */
#define FBUS_CMD_BGN				1							/* ��ʼ��ַ */
#define FBUS_CMD_SIZE				MAX_CMD						/* ���ݸ��� */
#define FBUS_CMD_END				(FBUS_CMD_BGN + FBUS_CMD_SIZE)	/* ������ַ */

/* ϵͳ״̬(�����¶�) */
/* 2017/10/21--1044 
 * �㲥��ϵͳ״̬�������� ����MAX_PV_WORD ģ����  
 * ���⣬������10������״̬�ִ���
 */
#define FBUS_SYS_STA_BGN			30		/* ��ʼ��ַ */
#define FBUS_SYS_STA_SIZE			(MAX_PV)
#define FBUS_SYS_STA_END			(FBUS_SYS_STA_BGN + FBUS_SYS_STA_SIZE)		/* ������ַ */

/* �汾�� */
//#define FBUS_VER_BGN				6400		/* ��ʼ��ַ */
//#define ONE_VER_SIZE					(sizeof(VER) / sizeof(U16))			/* ���ݸ��� */ 
#define FBUS_VER_BGN				140		/* ��ʼ��ַ */
#define FBUS_VER_SIZE				(sizeof(idu_mac_ver) / sizeof(U16))		/* MAX_PCB���汾�� */ 
#define ONE_VER_SIZE				(MAX_VER / 2)						/* �����汾�� */ 
#define FBUS_VER_END				(FBUS_VER_BGN + FBUS_VER_SIZE)		/* ������ַ */

/* ����汾�� */
#define FBUS_ODU_VER_BGN            270
#define FBUS_ODU_VER_SIZE           (sizeof(odu_mac_ver) / sizeof(U16))
#define ONE_ODU_VER_SIZE            (MAX_VER / 2)						/* �����汾�� */ 
#define FBUS_ODU_VER_END            (FBUS_ODU_VER_BGN + FBUS_ODU_VER_SIZE)

/* ģ��״̬ */
#define FBUS_MAC_BGN				400		/* ��ʼ��ַ */
#define FBUS_MAC_SIZE				(sizeof(cell) / sizeof(U16))	/* ���ݸ��� */
#define FBUS_MAC_END				(FBUS_MAC_BGN + FBUS_MAC_SIZE)/* ������ַ */

//ʱ��
#define BMS_CLOCK_BGN               5000          // 5000
#define BMS_CLOCK_NUM               SH_CLOCK_SIZE              // 7
#define BMS_CLOCK_END               (BMS_CLOCK_BGN + BMS_CLOCK_NUM)          // 5007

//�����¶�
#define BMS_ROOM_TEMP_BGN           5010          
#define BMS_ROOM_TEMP_NUM           SH_ROOM_TEMP_SIZE  //1
#define BMS_ROOM_TEMP_END           (BMS_ROOM_TEMP_BGN + BMS_ROOM_TEMP_NUM)          // 5011

//��ʷ����(5020~5720 ����������ʷ������ƫС�������߿���һ�£�Ԥ��90����ʷ���� SH_MAX_ERR_HIS_NUM)
#define BMS_ERR_HIS_BGN             5020
#define BMS_ERR_HIS_SIZE            SH_ERR_HIS_SIZE  //7
#define BMS_ERR_HIS_END             (BMS_ERR_HIS_BGN + BMS_ERR_HIS_SIZE*MAX_HIS_ERROR)  // 5090


/* ���� */
/* ����ֵ(0x1900~0x1dff) */            	
#define PARA_SIGLE_NUM				0x0040			/* ����������ݸ���U16���ݸ��� */
#define PARA_BIN_BIT_NUM			6				/* ���������ֶ�����λ�� */
#define PARA_SIZE					(PARA_GROUP_NUM * PARA_SIGLE_NUM)
#define ADDR_PARA_BGN				6400	/* ��ʼ��ַ 0x1900 */
#define ADDR_PARA_END				(ADDR_PARA_BGN + PARA_SIZE)

/* ��������Ϣ(0x1E00~0x1E7f) */
#define PARA_ITEM_SIZE				(PARA_GROUP_NUM + 1)
#define ADDR_PARA_ITEM_BGN			7680	/* ��ʼ��ַ 0x1E00 */
#define ADDR_PARA_ITEM_END			(ADDR_PARA_ITEM_BGN + PARA_ITEM_SIZE)

/* ��������Ϣ(0x1E80~0x327f) */				/* һ���������ĸ����� */
#define PARA_INFO_SINGLE_NUM		(PARA_SIGLE_NUM*4)		/* ����������ݸ��� */
#define PARA_INFO_BIN_BIT_NUM		(PARA_BIN_BIT_NUM+2)	/* ���������ֶ�����λ�� */
//#define PARA_INFO_SINGLE_NUM		0x0080			/* ����������ݸ��� */
//#define PARA_INFO_BIN_BIT_NUM		7				/* ���������ֶ�����λ�� */
#define PARA_INFO_SIZE				(PARA_GROUP_NUM * PARA_INFO_SINGLE_NUM)
#define ADDR_PARA_INFO_BGN			7808	/* ��ʼ��ַ 0x1E80 */
#define ADDR_PARA_INFO_END			(ADDR_PARA_INFO_BGN + PARA_INFO_SIZE)

/* DEBUG������Ϣ */
#define ADDR_BMS_DEBUG_BGN			(20000)		/* �õ���״̬ */
#define BMS_DEBUG_SIZE				(sizeof(bms_debug)/sizeof(U16))
#define ADDR_BMS_DEBUG_END			(ADDR_BMS_DEBUG_BGN+BMS_DEBUG_SIZE)		//������Ϣ����

/* ����(0x1D4C~0x1D67) */
#define WR_PASS_NUM				    (sizeof(password)/sizeof(U16))
#define ADDR_PASS_BGN			    7500
#define ADDR_PASS_END			    (ADDR_PASS_BGN + WR_PASS_NUM)

/* �㲥��������(0x1F40~0x1FA3) */
MODBUS_EXT U16 bro_special[100];
#define WR_SPECIAL_NUM				(sizeof(bro_special)/sizeof(U16))
#define ADDR_SPECIAL_BGN			8000
#define ADDR_SPECIAL_END			(ADDR_SPECIAL_BGN + WR_SPECIAL_NUM)

#ifdef USE_BMS_SOURCE
/* BMS��Դ���� */
#define ADDR_BMSSRC_BGN			    (21000)		/* �õ���״̬ */
#define BMSSRC_SIZE				    (sizeof(bms_source)/sizeof(U16))
#define ADDR_BMSSRC_END			    (ADDR_BMSSRC_BGN+BMSSRC_SIZE)		//������Ϣ���� 
#endif  

/* ���Գ���ĵ�ַ */
#define ADDR_TEST_BGN				50000		/* ��ʼ��ַ */        
#define ADDR_TEST_END				59999		/* ������ַ */

#define ADDR_TEST_CMD_BGN           50000       // ����������ʼ��ַ
#define ADDR_TEST_CMD_END           50009       // �������������ַ��������ʵ��ʹ�á����50049��
#define ADDR_TEST_STA_BGN           50050       // ����״̬��ʼ��ַ
#define ADDR_TEST_STA_END           50060       // ����״̬������ַ��������ʵ��ʹ�á����50199��
#define ADDR_TEST_DI_BGN            50200       // ���Կ�����������ʼ��ַ
#define ADDR_TEST_DI_END            50208       // ���Կ��������������ַ��������ʵ��ʹ�á����50299��
#define ADDR_TEST_NTC_BGN           50300       // �����¶���ʼ��ַ
#define ADDR_TEST_NTC_END           50372       // �����¶Ƚ�����ַ��������ʵ��ʹ�á����?????��
#define ADDR_TEST_CUR_BGN           51200       // ���Ե�����ʼ��ַ
#define ADDR_TEST_CUR_END           51230       // ���Ե���������ַ��������ʵ��ʹ�á����51799��
#define ADDR_TEST_AITYPE_BGN        51800       // ����AI������ʼ��ַ
#define ADDR_TEST_AITYPE_END        51808       // ����AI���ͽ�����ַ��������ʵ��ʹ�á����51999��
#define ADDR_TEST_AI_BGN            52000       // ����AI������ʼ��ַ
#define ADDR_TEST_AI_END            52048       // ����AI���ݽ�����ַ��������ʵ��ʹ�á����52599��
#define ADDR_TEST_STEPNOW_BGN       52600       // ���Բ��������ǰֵ��ʼ��ַ
#define ADDR_TEST_STEPNOW_END       52608       // ���Բ��������ǰֵ������ַ��������ʵ��ʹ�á����53199��
#define ADDR_TEST_STEPSET_BGN       53200       // ���Բ�������趨ֵ��ʼ��ַ
#define ADDR_TEST_STEPSET_END       53208       // ���Բ�������趨ֵ������ַ��������ʵ��ʹ�á����53799��
#define ADDR_TEST_VERSION_BGN       53800       // ���԰汾����ʼ��ַ
#define ADDR_TEST_VERSION_END       53816       // ���԰汾�Ž�����ַ��������ʵ��ʹ�á����53928��
#define ADDR_TEST_COMMU_BGN         56800       // ���Դ���ͨѶ��ʼ��ַ
#define ADDR_TEST_COMMU_END         56845       // ���Դ���ͨѶ������ַ��������ʵ��ʹ�á����57699��
#ifdef COM_4G
#define ADDR_4G_TEST_BGN		57200										// ͨ��������ʼ��ַ
#define _4G_TEST_SIZE			(sizeof(TEST4G)/sizeof(U16))				// ͨ������ռ�ô�С																	//��Ӧ�ó���
#define ADDR_4G_TEST_END		(ADDR_4G_TEST_BGN+_4G_TEST_SIZE)			// ͨ�����ݽ�����ַ
#endif
#define ADDR_TEST_PWM_BGN           57700       // ����PWM��ʼ��ַ
#define ADDR_TEST_PWM_END           57720       // ����PWM������ַ��������ʵ��ʹ�á����57750��
#define ADDR_TEST_INVT_BGN          57730       // ����PWM��ʼ��ַ
#define ADDR_TEST_INVT_END          57750       // ����PWM������ַ��������ʵ��ʹ�á����57750��

/* MODBUSͨѶ���ݸ������� */
#define MB_MAX_QUANTITY_RD_WORDS 	((MODBUS_BUF_SIZE-5)/2)
#define MB_MAX_QUANTITY_RD_BITS 	(MB_MAX_QUANTITY_RD_WORDS*16)
#define MB_MAX_QUANTITY_WR_WORDS 	((MODBUS_BUF_SIZE-9)/2)
#define MB_MAX_QUANTITY_WR_BITS 	(MB_MAX_QUANTITY_WR_WORDS*16)
#define MB_MAX_QUANTITY_DIAG		((MODBUS_BUF_SIZE-9)/2)

#define BMS_TEST_FAC_BGN 		60000
#define	BMS_TEST_FAC_SIZE	   	(sizeof(test_fac)/sizeof(U16))
#define BMS_TEST_FAC_END      	(BMS_TEST_FAC_BGN+BMS_TEST_FAC_SIZE)


#define BMS_4G_RW_BGN 		65200
#define	BMS_4G_RW_SIZE	   	(sizeof(app_4g_rw)/sizeof(U16))
#define BMS_4G_RW_END      	(BMS_4G_RW_BGN+BMS_4G_RW_SIZE)

#define BMS_4G_RO_BGN		65250
#define BMS_4G_RO_SIZE		(sizeof(app_4g_ro)/sizeof(U16))
#define BMS_4G_RO_END      	(BMS_4G_RO_BGN+BMS_4G_RO_SIZE)


#define MAX_HMI				1			// HMI����(ע�⣺HMI�������ɴ���7��) 	 




void SYS_CopyMemForModbusType(U08 *pDst, const U08 *pSrc, U16 size);

BOOL MB_SlaverRxTx(U08 u, UART *pCOM);
BOOL MB_MasterTx(const COMMSG *pComMsg, UART *pCOM);
BOOL MB_MasterRx(U08 u, UART *pCOM, COMMSG *pComMsg);
BOOL MB_MasterTxReqPDU(const COMMSG *pComMsg, U08 pduTxBuf[], U16 *pPduTxTotal, U16 *pPduRxTotal);
void MB_MasterRxRspPDU(const COMMSG *pComMsg, const U08 pduRxBuf[]);
U08  MB_SlaveRxReqTxRspPDU(const U08 *pduRxBuf, U08 *pduTxBuf, U16 *pPduTxTotal);

BOOL USER_AllowEnterBoot(void);
void USER_EnterBoot(void);
U08 PDU_SlaveDealRdDInputs(const U08 *pduRxBuf, U08 *pduTxBuf, U16 *pPduTxTotal);
MODBUS_EXT void modbus_time(void);



#endif


