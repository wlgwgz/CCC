/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: uart.h
 �ļ�����: 
 ������  : 
 ��������: 
*******************************************************************************/
#ifndef UART_H
#define UART_H	

#include "AppInfo.h"

#ifdef	UART_GLOBAL
	#define	UART_EXT
#else	
	#define	UART_EXT 	extern
#endif

#define COM_485_1       MCU_UART4_NUM
#define COM_485_2       MCU_UART5_NUM
#define COM_485_3       MCU_UART6_NUM
#define COM_485_4       MCU_UART0_NUM
#define COM_TTL         MCU_UART1_NUM
#define MAX_UART		MCU_UART_COUNT   /* ��󴮿�ͨ���� */

#define COM_FCU_FBUS			COM_485_1	// ����������
#define COM_INVT				COM_485_3	// ��Ƶ��
#define COM_BMS					COM_485_2	// BMS ��λ����������ͨѶ������
#define COM_TEXT_HMI			COM_485_4	// �ı���
#define COM_4G                  COM_TTL

UART_EXT U08 hmi_commu_error;				/* ��ʾ�ն�ͨѶ���� */
UART_EXT U08 slave_commu_error;				/* ģ���ͨѶ���� */
UART_EXT BOOL self_commu_err;              	/* ��������ͨѶ����(��ģ�����չ���Լ�ʹ��, �����������) */
UART_EXT U08 fg_invt_commu_error;          /* ��Ƶ��ͨѶ���� */

//UART_EXT U08 invt_baudrate;	/* ��Ƶ�������ʣ����ڲ���������Ӧ */
UART_EXT U08 com_baudrate[MAX_UART];
//UART_EXT BOOL fg_baudrate_confirm[MAX_UART];	/* ͨ�ſڲ���ȷ�� */


/* ͨѶ���ݸ�ʽ */
typedef enum tagCOMMUFORMAT
{
	COM_8_N_1,				/* 8������λ��1��ֹͣλ */
	COM_8_N_2				/* 8������λ��2��ֹͣλ */
}COMMUFORMAT;

/* ͨѶ������Ϣ����Ӧ�ò�������ϵ����(����Ϊ���нڵ��е�������) */
typedef struct tagCOMMSG
{
    U08 driverNo;			/* ͨѶ�豸�ĵ�ַ */
    U08 cmd;				/* ���� */
    U16 startAddr;			/* ͨѶЭ���ж�д��ʼ��ַ */
//    U16 length;				/* ��д���ݳ��� */
    U16 length_wr;			/* д���ݳ��� */
    U16 length_rd;			/* �����ݳ��� */
//    U16 *pData;				/* ���ͻ�������ݵ�ַ(λbitʱ��bit0Ϊ��ʼ��ַ������) */
    const U16 *pData_wr;		/* �������ݵ�ַ(λbitʱ��bit0Ϊ��ʼ��ַ������) */
    U16 *pData_rd;				/* �������ݵ�ַ(λbitʱ��bit0Ϊ��ʼ��ַ������) */
} COMMSG;
#define oneWrVal length_wr		/* Ϊ��ʡ�ռ䣬дһ������ʱ��ʹ��length����ʹ��pData */

/* ���нڵ� */
typedef struct tagQNODE
{
    U08 next;				/* �ýڵ�����һ���ڵ� */
    U08 driverType;			/* ͨѶ�豸������ */
    COMMSG data;			/* �ڵ������� */
	U08 sta;
} QNODE;
#define NODE_STA_NONE       0   /* �ڵ�� */
#define NODE_STA_CREATE     1   /* �ڵ㴴�� */
#define NODE_STA_SEND       2   /* �ڵ㷢�� */
#define NODE_STA_FINESH     3   /* �ڵ���� */

#define DRIVER_MB_MASTER	1	/* ��׼MODBUS���� */
//#define DRIVER_CAN_MASTER	2	/* CAN����(CANû��ʹ�ñ�׼MODBUS�������豸��ַ��CRC) */


#define Q_PRIO_HIGH		0	/* �����ȼ� */
#define Q_PRIO_LOW		1	/* �����ȼ� */
//#define MAX_NODE_NUM 	10	/* ���������Ľڵ���Ŀ */
#define MAX_NODE_NUM 	20	/* ���������Ľڵ���Ŀ */
#define MAX_L_NODE_NUM 	10	/* ���������ĵ����ȼ��ڵ���Ŀ */


/* ͨ�Ŷ��� */
typedef struct tagSENDQUEUE
{
	U08 head;				/* ������ */
	U08 end;				/* ����β */
	U08 cnt;				/* ���нڵ���� */
    U08 insertPos;			/* �������Ų���λ�� */
    QNODE node[MAX_NODE_NUM];/* ���нڵ����� */
} SENDQUEUE;

/* �������յ�����֡���ͣ������ж�����ͨѶ�������� */
enum 
{
	RX_FRAME_NO,			/* û�н��յ�����֡ */
	RX_FRAME_LEN_SHORTAGE,	/* ����֡���Ȳ���(����<4��С�����۸���) */
	RX_FRAME_DATA_ERR,		/* ����֡��������󣬵�����У����� */
	RX_FRAME_NORMAL_RSP,	/* ����֡��ȷ���Ҵӻ���������Ӧ */
	RX_FRAME_EXCEP_RSP		/* ����֡��ȷ�����ӻ����쳣��Ӧ */
};

/* ͨѶ�������� */
enum
{
	COMMU_NO_ERR,			/* ��ͨѶ���� */
	COMMU_OVERTIME,			/* [�����ж�]ͨѶ��ʱ����:δ�յ�����֡ */
	COMMU_INTERFERE,		/* [�����ж�]ͨѶ���Ź���:����֡���ȶ�(����<4��С�����۸���) */
	COMMU_DATA_ERR,			/* [�����ж�]ͨѶ���ݹ���:��������֡�������ǵ�ַ�����ݴ�У��ʹ��� */
	COMMU_EXCEP_RSP,		/* [�����ж�]�ӻ��ظ��쳣��Ӧ */
	COMMU_INT_PROT			/* [�ӻ��ж�]ͨѶ�жϱ���:�����жϱ���ʱ����δ�յ���Ч����֡ */
};

/* ͨ��״̬�� */
enum 	
{	
    COMMU_IDLE,				/* ����	*/
    COMMU_DELAY,			/* ��ʱ��ͨѶ�����ʱ�뷢����ʱ */
    COMMU_START,			/* �������� */
    COMMU_TX,				/* ���� */
    COMMU_RX,				/* ���� */
    COMMU_BROADCAST,		/* �㲥 */
    COMMU_FAIL				/* ʧ�� */
};

#define MODBUS_BUF_SIZE 			256		/* MODBUS��������С����Ҫ����256 */
#define COMMU_RX_SIZE (MODBUS_BUF_SIZE+10)	/* ���ջ�������С(����ǰ������и���) */
#define COMMU_TX_SIZE (MODBUS_BUF_SIZE)		/* ���ͻ�������С */

/* ע��:�������ʱ�䶨�峬��255ʱ���轫��Ӧ�ı�������ΪU16 */
//#define COMMU_TIME_INTERVAL			50		/* *2 ͨѶ�����ʱʱ��50*2ms=100ms */  
#define COMMU_TIME_INTERVAL			25		/* *2 ͨѶ�����ʱʱ��25*2ms=50ms������ͨѶ�����ʱ(��Ҫ����Ƕȴ�����ͨѶ) */
#define COMMU_TIME_RX_OVER			250		/* *2 ���ճ�ʱʱ�� */
#define COMMU_TIME_TX_OVER       	255     /* *2 ���ͳ�ʱ */
#define COMMU_TIME_RX_FRAME			10		/* *2 һ֡�������ʱ�䣬�����5*2msδ�յ���һ�ֽ���Ϊ������� */
#define COMMU_TIME_TX_DLY			10		/* *2 ������ʱʱ�䣬����485����оƬΪ����״̬����ʱ���ٷ������� */
#define COMMU_TIME_TX_TIMEOUT       255     /* *2 ���ͳ�ʱ */
#define COMMU_TIME_BROADCAST_DLY	100		/* *2 �㲥������ʱ */
#define COMMU_TIME_LED				20		/* *2 LED����˸ʱ�� */
#define COMMU_TIME_PROTECT			30		/* ͨѶ�жϱ���ʱ��(30��) */

#define COMMU_FAIL_NUM              2		/* �������&��Ƶ��ͨѶ����: ͨѶʧ�����Դ���Ϊ2 */
#define COMMU_FAIL_NUM_INVT         5		/* �����&��Ƶ��ͨѶ��: ͨѶʧ�����Դ���Ϊ5 */

/* ͨѶʱ��ṹ�� */
typedef struct tagCOMTIME
{
	U08 interval;			/* ͨѶ�����ʱ(����) */
	U16 rxOvertime;			/* ���ճ�ʱ��ʱ(����) */
	U08 txDly;				/* ������ʱ��ʱ(�������ӻ�) */
	U08 rxFrame;			/* һ֡������ɼ�ʱ(�������ӻ�) */
	U08 broadcastDly;		/* �㲥������ʱ��ʱ(����) */
	U08 led;				/* ͨѶ����˸��ʱ(�������ӻ�) */
	U16 commIntProt;		/* ͨѶ�жϱ���ʱ��(�ӻ����뵥λ��ʱ) */
} COMTIME;

/*******************************************************************************
	ͨѶ��¼�ṹ �����Ϣ
1��	ͨѶ�ܴ����������������ͨѶ����(COMMU_START������ʧ�ܺ����Ե���������)
	ͨѶ�ɹ��������յ�����ȷ֡����    
	ͨѶʧ�ܣ������ڳ�ʱʱ����δ�յ���ȷ֡	    	
	ͨѶ�ܴ��� = ��ȷ֡����+ʧ�ܴ���+�㲥����(�������)
2��	������֡�� = ����֡����+��ȷ֡����
	����֡���� = ����֡���Ȳ���+����֡У�����
	��ȷ֡���� = ��ǰ+��ȷ+�ں�
3��	����������ĵ�ʽ�����㡢��ʾ�������	
	��������󣬻��0���¼��� 
4�� �������Ǵӻ�ʱ���㲥����ʱ��ʧ�ܡ���������Ϊ0��
	ͨѶ����ʱ��ʾ����Ϣһ��Ϊ�����м�¼����Ϣ;��ȻҲ������ʾ�ӻ��м�¼����Ϣ��
5��	�Ժ��������Ӽ����������ں������ӣ���MODBUS��������ͬ������
*******************************************************************************/
typedef struct tagCOMRECORD
{
	U16 broadcast;			/* �㲥���� */
	U16 overtime;			/* ͨѶ��ʱ����(�����ڳ�ʱʱ����δ�յ��κ�һ֡����) */
	U16 fail;				/* ͨѶʧ�ܴ���(�����ڳ�ʱʱ����δ�յ���ȷ֡) */
	U16 alarm;				/* ͨѶ��������(���Լ�����ʧ�ܺ���㱨��) */
	U16 fLenShortage;		/* ����֡���Ȳ���(����<4��С�����۸���) */
	U16 frameDataErr;		/* ����֡��������󣬵�����У����� */
	U16 frameOkAhead;		/* ��ȷ������ǰ�и��ţ�Ҳ����ǰ���� */
	U16 frameOk;			/* ������ȫ��ȷ */
	U16 frameOkBack;		/* ��ȷ���ݺ��и��� */
} COMRECORD;

/* ���ڽṹ�嶨�� */
typedef struct tagUART
{
	U08 ownAddr;			/* ���ڱ���ĵ�ַ������ʱΪ0���ӻ�ʱ�����趨(һ��Ϊ1) */

    U16 rxCnt;				/* ʵ���ѽ��յ��ֽ��� */
	U16 rxTotal;			/* ��������յ��ֽ��� */
    U16 txCnt;				/* ʵ���ѷ��͵��ֽ��� */
    U16 txTotal;			/* �����跢�͵��ֽ��� */
	U08 rxBuf[COMMU_RX_SIZE];			/* ���ջ����� */
	U08 txBuf[COMMU_TX_SIZE];			/* ���ͻ����� */  
	U08 exceptCode;			/* modbus�쳣�� */
	U08 rxFrameType;		/* ���յ�������֡���ͣ������ж�ͨѶ�������� */

    U08 step;				/* ͨѶ״̬ */
	U08 failCnt;			/* ͨѶʧ���ظ��ۼ� */
    U08 commuErr;			/* ͨѶ�������ͣ���ֻ֧��һ�����ڽ�һ���豸 */

	COMTIME time;
	
	COMRECORD *pRecord;		/* ͨѶ��¼�ṹ��ָ�룬��ͨѶ��ʼ�������ʼ�� */
	
} UART;

extern const U32 baudRateList[];

void COM_DisableRece(U08 u);
void COM_SetFormat(U08 u, COMMUFORMAT format);
void COM_ReadyRece(U08 u);
void COM_Uart4RxInt(void);
void COM_Uart4TxInt(void);
void COM_Uart3RxInt(void);
void COM_Uart3TxInt(void);
void COM_Uart2RxInt(void);
void COM_Uart2TxInt(void);
void COM_Uart1RxInt(void);
void COM_Uart1TxInt(void);
void COM_Uart0RxInt(void);
void COM_Uart0TxInt(void);
void COM_TrasDly(U08 u);
void COM_Set485Tras(U08 u);
void COM_WrTrasBufReg(U08 u, U08 value);
U08  COM_GetErr(U08 u);
U08  COM_GetMbExceptCode(U08 u);
void COM_ResetAlarm(U08 u);
BOOL COM_Idle(U08 u);
U08  COM_GetLedTime(U08 u);
void COM_2MsTimer(U08 u);
void COM_VariableTimer(U08 u);
void COM_ProtectTimer(void);
void COM_SlaverProg(U08 u);
void COM_MasterProg(U08 u);
void COM4_MasterProg(U08 u);
void COM_HMIMasterProg(U08 u);
void COM_InitSlaver(U08 u, COMRECORD *pRcd, U08 addr);
void COM_Init(void);
BOOL COM_QEmpty(U08 u);
BOOL COM_QFull(U08 u);
U08  COM_QGetNodeCnt(U08 u);
BOOL COM_QInsertNode(U08 u, U08 prio, const QNODE *pNode);
BOOL COM_WriteWords(U08 u, U08 pcb, U16 startAddr, U16  length, const U16 *pData, U08 prio);
BOOL COM_WriteOneWord(U08 u, U08 pcb,  U16 startAddr, U16 val, U08 prio);
BOOL COM_WriteOneBit(U08 u, U08 pcb,  U16 startAddr, U16 val, U08 prio);
BOOL COM_ReadRWwords(U08 u, U08 pcb,  U16 startAddr, U16  length, U16  *pData, U08 prio);
BOOL COM_ReadROwords(U08 u, U08 pcb,  U16 startAddr, U16  length, U16  *pData, U08 prio);
void COM_Config(U08 u,U08 baud, U08 dataType);
UART_EXT void COM_ClrRecord(void);//wlg20150706
UART_EXT BOOL COM_QSendNodeData(U08 u);
UART_EXT BOOL COM_QAcceptNodeData(U08 u);
UART_EXT void COM_QNextNode(U08 u);
UART_EXT void COM_QInit(U08 u);
UART_EXT U08 COM_GetNodeSta(U08 u, U08 node_idx);
UART_EXT U08 COM_GetNodeIdx(U08 u, U08 prio);
UART_EXT void COM_InitMaster(U08 u, COMRECORD *pRcd);
//UART_EXT U08 test_combuf[20];
UART_EXT UART Uart[MAX_UART];	/* ���ڽṹ������ */
UART_EXT void  LOG_COM_WrTrasBufReg(U08 u, U08 value);
#define COM_ERR_CLR				0			/* ���ͨѶ���� */
#define COM_ERR_REC				1			/* ��¼ͨѶ���� */
#define COM_ERR_GET			    2			/* ��ȡͨѶ���� */
UART_EXT BOOL COM_err_deal(U08 u, U08 addr, U08 op);
UART_EXT void COM_config_and_init(U08 u);
void COM_UartRxInt(U08 u);
void COM_UartTxInt(U08 u);

#endif
