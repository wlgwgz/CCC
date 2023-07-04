/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: uart.h
 文件描述: 
 创建人  : 
 创建日期: 
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
#define MAX_UART		MCU_UART_COUNT   /* 最大串口通道数 */

#define COM_FCU_FBUS			COM_485_1	// 级联、风盘
#define COM_INVT				COM_485_3	// 变频器
#define COM_BMS					COM_485_2	// BMS 上位机即第三方通讯测试屏
#define COM_TEXT_HMI			COM_485_4	// 文本屏
#define COM_4G                  COM_TTL

UART_EXT U08 hmi_commu_error;				/* 显示终端通讯故障 */
UART_EXT U08 slave_commu_error;				/* 模块板通讯故障 */
UART_EXT BOOL self_commu_err;              	/* 本机级联通讯故障(从模块或扩展板自己使用, 不作故障输出) */
UART_EXT U08 fg_invt_commu_error;          /* 变频器通讯故障 */

//UART_EXT U08 invt_baudrate;	/* 变频器波特率，用于波特率自适应 */
UART_EXT U08 com_baudrate[MAX_UART];
//UART_EXT BOOL fg_baudrate_confirm[MAX_UART];	/* 通信口波特确认 */


/* 通讯数据格式 */
typedef enum tagCOMMUFORMAT
{
	COM_8_N_1,				/* 8个数据位、1个停止位 */
	COM_8_N_2				/* 8个数据位、2个停止位 */
}COMMUFORMAT;

/* 通讯数据信息，与应用层数据联系紧密(可做为队列节点中的数据域) */
typedef struct tagCOMMSG
{
    U08 driverNo;			/* 通讯设备的地址 */
    U08 cmd;				/* 命令 */
    U16 startAddr;			/* 通讯协议中读写起始地址 */
//    U16 length;				/* 读写数据长度 */
    U16 length_wr;			/* 写数据长度 */
    U16 length_rd;			/* 读数据长度 */
//    U16 *pData;				/* 发送或接收数据地址(位bit时，bit0为起始地址的数据) */
    const U16 *pData_wr;		/* 发送数据地址(位bit时，bit0为起始地址的数据) */
    U16 *pData_rd;				/* 接收数据地址(位bit时，bit0为起始地址的数据) */
} COMMSG;
#define oneWrVal length_wr		/* 为节省空间，写一个数据时，使用length，不使用pData */

/* 队列节点 */
typedef struct tagQNODE
{
    U08 next;				/* 该节点后的下一个节点 */
    U08 driverType;			/* 通讯设备的类型 */
    COMMSG data;			/* 节点数据域 */
	U08 sta;
} QNODE;
#define NODE_STA_NONE       0   /* 节点空 */
#define NODE_STA_CREATE     1   /* 节点创建 */
#define NODE_STA_SEND       2   /* 节点发送 */
#define NODE_STA_FINESH     3   /* 节点完成 */

#define DRIVER_MB_MASTER	1	/* 标准MODBUS主机 */
//#define DRIVER_CAN_MASTER	2	/* CAN主机(CAN没有使用标准MODBUS，少了设备地址和CRC) */


#define Q_PRIO_HIGH		0	/* 高优先级 */
#define Q_PRIO_LOW		1	/* 低优先级 */
//#define MAX_NODE_NUM 	10	/* 队列中最大的节点数目 */
#define MAX_NODE_NUM 	20	/* 队列中最大的节点数目 */
#define MAX_L_NODE_NUM 	10	/* 队列中最大的低优先级节点数目 */


/* 通信队列 */
typedef struct tagSENDQUEUE
{
	U08 head;				/* 队列首 */
	U08 end;				/* 队列尾 */
	U08 cnt;				/* 队列节点个数 */
    U08 insertPos;			/* 高优先优插入位置 */
    QNODE node[MAX_NODE_NUM];/* 队列节点数据 */
} SENDQUEUE;

/* 主机接收的数据帧类型，用于判断主机通讯故障类型 */
enum 
{
	RX_FRAME_NO,			/* 没有接收到数据帧 */
	RX_FRAME_LEN_SHORTAGE,	/* 数据帧长度不足(干扰<4或小于理论个数) */
	RX_FRAME_DATA_ERR,		/* 数据帧长度满足后，但数据校验错误 */
	RX_FRAME_NORMAL_RSP,	/* 数据帧正确，且从机是正常响应 */
	RX_FRAME_EXCEP_RSP		/* 数据帧正确，但从机是异常响应 */
};

/* 通讯故障类型 */
enum
{
	COMMU_NO_ERR,			/* 无通讯故障 */
	COMMU_OVERTIME,			/* [主机判断]通讯超时故障:未收到数据帧 */
	COMMU_INTERFERE,		/* [主机判断]通讯干扰故障:数据帧长度短(干扰<4或小于理论个数) */
	COMMU_DATA_ERR,			/* [主机判断]通讯数据故障:错误数据帧，可能是地址错、数据错、校验和错误 */
	COMMU_EXCEP_RSP,		/* [主机判断]从机回复异常响应 */
	COMMU_INT_PROT			/* [从机判断]通讯中断保护:超过中断保护时间仍未收到有效数据帧 */
};

/* 通信状态字 */
enum 	
{	
    COMMU_IDLE,				/* 空闲	*/
    COMMU_DELAY,			/* 延时，通讯间隔延时与发送延时 */
    COMMU_START,			/* 启动发送 */
    COMMU_TX,				/* 发送 */
    COMMU_RX,				/* 接收 */
    COMMU_BROADCAST,		/* 广播 */
    COMMU_FAIL				/* 失败 */
};

#define MODBUS_BUF_SIZE 			256		/* MODBUS缓冲区大小，不要超过256 */
#define COMMU_RX_SIZE (MODBUS_BUF_SIZE+10)	/* 接收缓冲区大小(数据前后可能有干扰) */
#define COMMU_TX_SIZE (MODBUS_BUF_SIZE)		/* 发送缓冲区大小 */

/* 注意:当下面的时间定义超过255时，需将相应的变量更新为U16 */
//#define COMMU_TIME_INTERVAL			50		/* *2 通讯间隔延时时间50*2ms=100ms */  
#define COMMU_TIME_INTERVAL			25		/* *2 通讯间隔延时时间25*2ms=50ms，减少通讯间隔延时(主要是与角度传感器通讯) */
#define COMMU_TIME_RX_OVER			250		/* *2 接收超时时间 */
#define COMMU_TIME_TX_OVER       	255     /* *2 发送超时 */
#define COMMU_TIME_RX_FRAME			10		/* *2 一帧接收完成时间，即如果5*2ms未收到下一字节认为接收完成 */
#define COMMU_TIME_TX_DLY			10		/* *2 发送延时时间，先置485控制芯片为发送状态，延时后再发送数据 */
#define COMMU_TIME_TX_TIMEOUT       255     /* *2 发送超时 */
#define COMMU_TIME_BROADCAST_DLY	100		/* *2 广播结束延时 */
#define COMMU_TIME_LED				20		/* *2 LED灯闪烁时间 */
#define COMMU_TIME_PROTECT			30		/* 通讯中断保护时间(30秒) */

#define COMMU_FAIL_NUM              2		/* 除内外机&变频器通讯口外: 通讯失败重试次数为2 */
#define COMMU_FAIL_NUM_INVT         5		/* 内外机&变频器通讯口: 通讯失败重试次数为5 */

/* 通讯时间结构体 */
typedef struct tagCOMTIME
{
	U08 interval;			/* 通讯间隔计时(主机) */
	U16 rxOvertime;			/* 接收超时计时(主机) */
	U08 txDly;				/* 发送延时计时(主机、从机) */
	U08 rxFrame;			/* 一帧接收完成计时(主机、从机) */
	U08 broadcastDly;		/* 广播结束延时计时(主机) */
	U08 led;				/* 通讯灯闪烁计时(主机、从机) */
	U16 commIntProt;		/* 通讯中断保护时间(从机，秒单位计时) */
} COMTIME;

/*******************************************************************************
	通讯记录结构 诊断信息
1、	通讯总次数：主机发起的总通讯次数(COMMU_START，包括失败后重试的启动发送)
	通讯成功：主机收到的正确帧总数    
	通讯失败：主机在超时时间内未收到正确帧	    	
	通讯总次数 = 正确帧总数+失败次数+广播次数(针对主机)
2、	接收总帧数 = 错误帧总数+正确帧总数
	错误帧总数 = 数据帧长度不足+数据帧校验错误
	正确帧总数 = 在前+正确+在后
3、	请利用上面的等式来计算、显示相关数据	
	计数溢出后，会从0重新计数 
4、 当串口是从机时，广播、超时、失败、报警次数为0，
	通讯测试时显示的信息一般为主机中记录的信息;当然也可以显示从机中记录的信息。
5、	以后若需增加计数器，请在后面增加，且MODBUS区域定义需同步增加
*******************************************************************************/
typedef struct tagCOMRECORD
{
	U16 broadcast;			/* 广播次数 */
	U16 overtime;			/* 通讯超时次数(主机在超时时间内未收到任何一帧数据) */
	U16 fail;				/* 通讯失败次数(主机在超时时间内未收到正确帧) */
	U16 alarm;				/* 通讯报警次数(重试几次仍失败后才算报警) */
	U16 fLenShortage;		/* 数据帧长度不足(干扰<4或小于理论个数) */
	U16 frameDataErr;		/* 数据帧长度满足后，但数据校验错误 */
	U16 frameOkAhead;		/* 正确的数据前有干扰，也可能前后都有 */
	U16 frameOk;			/* 数据完全正确 */
	U16 frameOkBack;		/* 正确数据后有干扰 */
} COMRECORD;

/* 串口结构体定义 */
typedef struct tagUART
{
	U08 ownAddr;			/* 串口本身的地址，主机时为0，从机时可自设定(一般为1) */

    U16 rxCnt;				/* 实际已接收的字节数 */
	U16 rxTotal;			/* 理论需接收的字节数 */
    U16 txCnt;				/* 实际已发送的字节数 */
    U16 txTotal;			/* 理论需发送的字节数 */
	U08 rxBuf[COMMU_RX_SIZE];			/* 接收缓冲区 */
	U08 txBuf[COMMU_TX_SIZE];			/* 发送缓冲区 */  
	U08 exceptCode;			/* modbus异常码 */
	U08 rxFrameType;		/* 接收到的数据帧类型，用于判断通讯故障类型 */

    U08 step;				/* 通讯状态 */
	U08 failCnt;			/* 通讯失败重复累计 */
    U08 commuErr;			/* 通讯故障类型，暂只支持一个串口接一个设备 */

	COMTIME time;
	
	COMRECORD *pRecord;		/* 通讯记录结构体指针，在通讯初始化中需初始化 */
	
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
UART_EXT UART Uart[MAX_UART];	/* 串口结构体数组 */
UART_EXT void  LOG_COM_WrTrasBufReg(U08 u, U08 value);
#define COM_ERR_CLR				0			/* 清除通讯故障 */
#define COM_ERR_REC				1			/* 记录通讯故障 */
#define COM_ERR_GET			    2			/* 获取通讯故障 */
UART_EXT BOOL COM_err_deal(U08 u, U08 addr, U08 op);
UART_EXT void COM_config_and_init(U08 u);
void COM_UartRxInt(U08 u);
void COM_UartTxInt(U08 u);

#endif
