/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: Modbus.h
 文件描述: 
 创建人  : 
 创建日期: 
*******************************************************************************/
#ifndef MODBUS_H
#define MODBUS_H

#ifdef MODBUS_GLOBAL
#define MODBUS_EXT
#else 
#define MODBUS_EXT extern
#endif

/* 系统内核--Modbus空间定义(字单位，不是字节) */
#define I_SIZE	64
#define Q_SIZE	8

/* 细分M区MTbl */
#define M_CORE_SIZE		512	/* 系统内核区大小 */
//#define M_IO_SIZE		64	/* IO区大小 */
//#define M_WORK_SIZE		64	/* 工作区大小 */	
//#define M_PARM_SIZE		64	/* 参数区大小 */
//#define M_PROG_SIZE		64	/* 程序区大小 */

//#define M_SIZE	(M_CORE_SIZE+M_IO_SIZE+M_WORK_SIZE+M_PARM_SIZE+M_PROG_SIZE)
#define M_SIZE	(M_CORE_SIZE)
//MODBUS_EXT BIT16 XDATA ITbl[I_SIZE];
//MODBUS_EXT BIT16 XDATA QTbl[Q_SIZE];
//MODBUS_EXT BIT16 XDATA MTbl[M_SIZE];
MODBUS_EXT BIT16 XDATA ModbusMemory[I_SIZE+Q_SIZE+M_SIZE];//xdata不能超过16K
#define ITbl (ModbusMemory)				
#define QTbl (ModbusMemory+I_SIZE)		
#define MTbl (ModbusMemory+I_SIZE+Q_SIZE)

/* 有定义时，通过Q区访问M区，容许人机一起访问Q、M区；无定义时，只能分开访问。
Q、M区在地址上是连续的，但M区定义可能太大，导致通过Q区访问不到M区，
如M区定义为64K字节，与Q区定义不在同一个64K范围内 */
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
MODBUS数据模型
基本表格	对象类型	访问类型	对应功能码  内容
离散量输入	单个比特	只读		02			I/O系统提供这种类型数据
线圈	  	单个比特	读写		01 05 15	通过应用程序改变这种类型数据
输入寄存器	16-比特字	只读		04			I/O系统提供这种类型数据
保持寄存器	16-比特字	读写		03 06 16	通过应用程序改变这种类型数据
*******************************************************************************/
/* MODBUS功能码 */
#define MB_RD_COILS					0x01	/* 读线圈 */
#define MB_RD_DISCRETE_INPUTS		0x02	/* 读离散量输入 */
#define MB_RD_HOLDING_REGISTERS		0x03	/* 读保持寄存器 */
#define MB_RD_INPUT_REGISTERS		0x04	/* 读输入寄存器 */
#define MB_WR_SINGLE_COIL			0x05	/* 写单个线圈 */
#define MB_WR_SINGLE_REGISTER		0x06	/* 写单个保持寄存器 */
#define MB_DIAGNOSTICS				0x08	/* 回路诊断测试 */
#define MB_WR_MULTIPLE_COILS		0x0f	/* 写多个线圈 */
#define MB_WR_MULTIPLE_REGISTERS	0x10	/* 写多个保持寄存器 */
#define MB_RDWR_MULTIPLE_REGISTERS	0x17	/* 自定义功能码：读写 */
#define MB_USER_DEFINED_DOWNLOAD	0x41	/* 用户自定义功能码:程序下载 */
#define MB_USER_DEFINED_RD_BIGFILE 	0x44	/* 用户自定义功能码:读取大数据 */
#define MB_USER_DEFINED_KLV         0x47    /* 用户自定义功能码:KLV协议读写 *//* N0004 */
#define MB_USER_DEFINED_EXIO_KLV    0x48	/* 用户自定义功能码:EXIO_KLV协议读写 */

/* MODBUS子功能码 */
#define MB_0x41_SUB_FUNC_ALLOW_BOOT         0x0000      // 询问是否可升级
#define MB_0x41_SUB_FUNC_ACCEPT_BOOT        0x0001      // 询问是否接受升级
#define MB_0x41_SUB_FUNC_ENTER_BOOT         0x0010      // 执行进入升级
#define MB_0x41_SUB_FUNC_REQUEST_BOOT       0x0011      // 执行请求升级
#define MB_0x41_SUB_FUNC_ENTER_USB          0x0020      // 进入USB模式
#define MB_0x41_SUB_FUNC_EXIT_USB           0x0021      // 退出USB模式


#define ADDR_BROADCAST		0				/* 广播地址 */
#define ADDR_BRO_FBUS		0xf8			/* 级联广播地址 */

/* MODBUS异常码 */
#define MB_NO_ERR       			0		/* MODBUS无异常 */
#define MB_ILLEGAL_FUNCTION       	1		/* 非法功能 */
#define MB_ILLEGAL_DATA_ADDR       	2		/* 非法数据地址 */
#define MB_ILLEGAL_DATA_VALUE       3		/* 非法数据值 */
#define MB_SLAVE_DEVICE_FALUE       4		/* 从机设备故障，从机无法处理 */

#define PASSWORD_T_NUM  16//wlg20150429
#define PASSWORD_W_NUM  8
//MODBUS_EXT U08	password[PASSWORD_T_NUM][PASSWORD_W_NUM];	/* 密码 */
//MODBUS_EXT U16	passwd_modify;		/* 密码修改标识 */
//MODBUS_EXT U08	passwd_broadcast;	/* 密码广播 */
#define PWR_NONE		0		/* 未登录 */
#define PWR_USER		1		/* 用户 */
#define PWR_SALE		2		/* 经销商 */
#define PWR_PRO			3		/* 工程调试员 */
#define PWR_SER			4		/* 厂家维修员 */
#define PWR_FAC			5		/* 厂家管理员 */
#define PWR_SPR			9		/* 超级密码 */
#define PASS_LMT_1		10		/* 使用期限第一组密码 */

/* MODBUS地址定义 */
MODBUS_EXT U08 controller_is_punp;		/* 是否是邦普控制器(身份识别) */
/* 上位机(HMI、BMS)命令: 用0x05、0x15 位的方式访问 */
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
/* 手动调试 */
#define ADDR_TEST_DO_BGN		    (2048)
#define TEST_DO_SIZE			    ((sizeof(test_data.wr.DO_sta) << 4) / sizeof(U16))
#define ADDR_TEST_DO_END		    (ADDR_TEST_DO_BGN+TEST_DO_SIZE)

/* DEBUG信息 */
#define ADDR_DEBUG_BGN				0							    //调试信息地址起始
#define DEBUG_SIZE					255								//调试信息大小
#define ADDR_DEBUG_END				(ADDR_DEBUG_BGN+DEBUG_SIZE)		//调试信息结束

/* 版本号 */
#define ADDR_VER_BS_BGN				256		/* 起始地址 0x100 */
#define VER_BS_SIZE					(sizeof(idu_mac_ver) / sizeof(U16))		/* 数据个数 */ 
#define ADDR_VER_BS_END				(ADDR_VER_BS_BGN + VER_BS_SIZE)		/* 结束地址 */

/* 大端模式版本,即按字节顺序 */
#define ADDR_VER2_BGN               512
#define VER2_SIZE					(sizeof(idu_mac_ver) / sizeof(U16))		/* 数据个数 */ 
#define ADDR_VER2_END				(ADDR_VER2_BGN + VER2_SIZE)		/* 结束地址 */

/* 公共信息 */
#define ADDR_CTRL_BGN				768		/* 起始地址 0x300 */
#define CTRL_SIZE					(sizeof(ctrl_info) / sizeof(U16))		/* 数据个数 */ 
#define ADDR_CTRL_END				(ADDR_CTRL_BGN + CTRL_SIZE)		/* 结束地址 */
/* 辅助信息 */
#define ADDR_ASSI_BGN				896		/* 起始地址 0x380 */
#define ASSI_SIZE					(sizeof(assist_info) / sizeof(U16))		/* 数据个数 */ 
#define ADDR_ASSI_END				(ADDR_ASSI_BGN + ASSI_SIZE)		/* 结束地址 */
/* 试运行fg_test_run */
#define ADDR_TEST_RUN_BGN			910		/* 起始地址 0x38E */
#define TEST_RUN_SIZE				(sizeof(fg_test_run) / sizeof(U16))		/* 数据个数 */ 
#define ADDR_TEST_RUN_END			(ADDR_TEST_RUN_BGN + TEST_RUN_SIZE)		/* 结束地址 */
/* BMSHMI */
#define ADDR_BMSHMI_BGN				912		/* 起始地址 0x390 */
#define BMSHMI_SIZE					(sizeof(bmsHMI.info_bit) / sizeof(U16))		/* 数据个数 */ 
#define ADDR_BMSHMI_END				(ADDR_BMSHMI_BGN + BMSHMI_SIZE)		/* 结束地址 */

/* 压机个数 */
#define ADDR_COMPNUM_BGN            1000                            
#define COMPNUM_SIZE                MAX_PCB
#define ADDR_COMPNUM_END            (ADDR_COMPNUM_BGN + COMPNUM_SIZE)
/* 系统模块信息：为兼容原来的协议，这一块地址保留  */
#define UNIT_MODE_SINGLE_NUM		0x0100			/* 单组最大数据个数 */
#define UNIT_BIN_BIT_NUM			8				/* 单组屏蔽字二进制位数 */
#define ADDR_UNIT_MODE_BGN			0X400	        /* 起始地址 0x400 */
#define ADDR_UNIT_MODE_END			(ADDR_UNIT_MODE_BGN + UNIT_MODE_SINGLE_NUM)	/* 1280 */

/* 模块信息  */
#define UNIT_SIZE					(UNIT_MODE_SINGLE_NUM * MAX_PCB)
#define ADDR_UNIT_BGN				(ADDR_UNIT_MODE_END)	        /* 1280 */
#define ADDR_UNIT_END				(ADDR_UNIT_BGN + UNIT_SIZE)		/* 1280 + 2048 = 3328 */

/* 模块信息,位访问(0x0300~0x1B00) */
#define UNIT_STEP_NUM           	0x0300          /* 步进长度 */
#define UNIT_BITS              	 	CELL_BITS       /* 单模块实际可访问长度 */
#define ADDR_UNIT_BIT_BGN       	0x0300          /* 起始访问长度 */
#define ADDR_UNIT_BIT_END       	(ADDR_UNIT_BIT_BGN + (UNIT_STEP_NUM*MAX_PCB))

/* 系统信息  */   	
#define SYS_SIZE					UNIT_MODE_SINGLE_NUM
#define ADDR_SYS_BGN				(ADDR_UNIT_END)			        /* 3328 */
#define ADDR_SYS_END				(ADDR_SYS_BGN + SYS_SIZE)		/* 3328 + 256 = 3584 */

/* 系统信息,位访问(0x0000~0x0300) */
#define SYS_BITS                	CELL_BITS
#define ADDR_SYS_BIT_BGN       	 	0x0000
#define ADDR_SYS_BIT_END        	(ADDR_SYS_BIT_BGN + SYS_BITS)

/* 外机版本：大端模式，即按字节顺序 */
#define ADDR_VER_ODU_BGN            4000
#define VER_ODU_SIZE				(sizeof(odu_mac_ver) / sizeof(U16))		/* 数据个数 */ 
#define ADDR_VER_ODU_END			(ADDR_VER_ODU_BGN + VER_ODU_SIZE)		/* 结束地址 */

/* 变频驱动版本：大端模式，即按字节顺序 */
#define ADDR_VER_INVT_BGN            4256
#define VER_INVT_SIZE				(MAX_VER_BYTE/2)		                /* 数据个数 */ 
#define ADDR_VER_INVT_END			(ADDR_VER_INVT_BGN + VER_INVT_SIZE)		/* 结束地址 */

//==========================================================================================================


/* FBUS 通讯口协议地址定义
 * 使用 FBUS_XX 地址前缀
 */

/* 机组内(FBUS)命令 */
#define FBUS_CMD_BGN				1							/* 起始地址 */
#define FBUS_CMD_SIZE				MAX_CMD						/* 数据个数 */
#define FBUS_CMD_END				(FBUS_CMD_BGN + FBUS_CMD_SIZE)	/* 结束地址 */

/* 系统状态(包括温度) */
/* 2017/10/21--1044 
 * 广播的系统状态数据内容 包含MAX_PV_WORD 模拟量  
 * 另外，还包含10个其它状态字传输
 */
#define FBUS_SYS_STA_BGN			30		/* 起始地址 */
#define FBUS_SYS_STA_SIZE			(MAX_PV)
#define FBUS_SYS_STA_END			(FBUS_SYS_STA_BGN + FBUS_SYS_STA_SIZE)		/* 结束地址 */

/* 版本号 */
//#define FBUS_VER_BGN				6400		/* 起始地址 */
//#define ONE_VER_SIZE					(sizeof(VER) / sizeof(U16))			/* 数据个数 */ 
#define FBUS_VER_BGN				140		/* 起始地址 */
#define FBUS_VER_SIZE				(sizeof(idu_mac_ver) / sizeof(U16))		/* MAX_PCB个版本号 */ 
#define ONE_VER_SIZE				(MAX_VER / 2)						/* 单个版本号 */ 
#define FBUS_VER_END				(FBUS_VER_BGN + FBUS_VER_SIZE)		/* 结束地址 */

/* 外机版本号 */
#define FBUS_ODU_VER_BGN            270
#define FBUS_ODU_VER_SIZE           (sizeof(odu_mac_ver) / sizeof(U16))
#define ONE_ODU_VER_SIZE            (MAX_VER / 2)						/* 单个版本号 */ 
#define FBUS_ODU_VER_END            (FBUS_ODU_VER_BGN + FBUS_ODU_VER_SIZE)

/* 模块状态 */
#define FBUS_MAC_BGN				400		/* 起始地址 */
#define FBUS_MAC_SIZE				(sizeof(cell) / sizeof(U16))	/* 数据个数 */
#define FBUS_MAC_END				(FBUS_MAC_BGN + FBUS_MAC_SIZE)/* 结束地址 */

//时钟
#define BMS_CLOCK_BGN               5000          // 5000
#define BMS_CLOCK_NUM               SH_CLOCK_SIZE              // 7
#define BMS_CLOCK_END               (BMS_CLOCK_BGN + BMS_CLOCK_NUM)          // 5007

//室内温度
#define BMS_ROOM_TEMP_BGN           5010          
#define BMS_ROOM_TEMP_NUM           SH_ROOM_TEMP_SIZE  //1
#define BMS_ROOM_TEMP_END           (BMS_ROOM_TEMP_BGN + BMS_ROOM_TEMP_NUM)          // 5011

//历史故障(5020~5720 由于现有历史故障数偏小，故与线控器一致，预留90个历史故障 SH_MAX_ERR_HIS_NUM)
#define BMS_ERR_HIS_BGN             5020
#define BMS_ERR_HIS_SIZE            SH_ERR_HIS_SIZE  //7
#define BMS_ERR_HIS_END             (BMS_ERR_HIS_BGN + BMS_ERR_HIS_SIZE*MAX_HIS_ERROR)  // 5090


/* 参数 */
/* 参数值(0x1900~0x1dff) */            	
#define PARA_SIGLE_NUM				0x0040			/* 单组最大数据个数U16数据个数 */
#define PARA_BIN_BIT_NUM			6				/* 单组屏蔽字二进制位数 */
#define PARA_SIZE					(PARA_GROUP_NUM * PARA_SIGLE_NUM)
#define ADDR_PARA_BGN				6400	/* 起始地址 0x1900 */
#define ADDR_PARA_END				(ADDR_PARA_BGN + PARA_SIZE)

/* 参数组信息(0x1E00~0x1E7f) */
#define PARA_ITEM_SIZE				(PARA_GROUP_NUM + 1)
#define ADDR_PARA_ITEM_BGN			7680	/* 起始地址 0x1E00 */
#define ADDR_PARA_ITEM_END			(ADDR_PARA_ITEM_BGN + PARA_ITEM_SIZE)

/* 参数项信息(0x1E80~0x327f) */				/* 一个参数有四个属性 */
#define PARA_INFO_SINGLE_NUM		(PARA_SIGLE_NUM*4)		/* 单组最大数据个数 */
#define PARA_INFO_BIN_BIT_NUM		(PARA_BIN_BIT_NUM+2)	/* 单组屏蔽字二进制位数 */
//#define PARA_INFO_SINGLE_NUM		0x0080			/* 单组最大数据个数 */
//#define PARA_INFO_BIN_BIT_NUM		7				/* 单组屏蔽字二进制位数 */
#define PARA_INFO_SIZE				(PARA_GROUP_NUM * PARA_INFO_SINGLE_NUM)
#define ADDR_PARA_INFO_BGN			7808	/* 起始地址 0x1E80 */
#define ADDR_PARA_INFO_END			(ADDR_PARA_INFO_BGN + PARA_INFO_SIZE)

/* DEBUG调试信息 */
#define ADDR_BMS_DEBUG_BGN			(20000)		/* 用调试状态 */
#define BMS_DEBUG_SIZE				(sizeof(bms_debug)/sizeof(U16))
#define ADDR_BMS_DEBUG_END			(ADDR_BMS_DEBUG_BGN+BMS_DEBUG_SIZE)		//调试信息结束

/* 密码(0x1D4C~0x1D67) */
#define WR_PASS_NUM				    (sizeof(password)/sizeof(U16))
#define ADDR_PASS_BGN			    7500
#define ADDR_PASS_END			    (ADDR_PASS_BGN + WR_PASS_NUM)

/* 广播特殊数据(0x1F40~0x1FA3) */
MODBUS_EXT U16 bro_special[100];
#define WR_SPECIAL_NUM				(sizeof(bro_special)/sizeof(U16))
#define ADDR_SPECIAL_BGN			8000
#define ADDR_SPECIAL_END			(ADDR_SPECIAL_BGN + WR_SPECIAL_NUM)

#ifdef USE_BMS_SOURCE
/* BMS资源仿真 */
#define ADDR_BMSSRC_BGN			    (21000)		/* 用调试状态 */
#define BMSSRC_SIZE				    (sizeof(bms_source)/sizeof(U16))
#define ADDR_BMSSRC_END			    (ADDR_BMSSRC_BGN+BMSSRC_SIZE)		//调试信息结束 
#endif  

/* 测试程序的地址 */
#define ADDR_TEST_BGN				50000		/* 起始地址 */        
#define ADDR_TEST_END				59999		/* 结束地址 */

#define ADDR_TEST_CMD_BGN           50000       // 测试命令起始地址
#define ADDR_TEST_CMD_END           50009       // 测试命令结束地址（本程序实际使用。最大50049）
#define ADDR_TEST_STA_BGN           50050       // 测试状态起始地址
#define ADDR_TEST_STA_END           50060       // 测试状态结束地址（本程序实际使用。最大50199）
#define ADDR_TEST_DI_BGN            50200       // 测试开关量输入起始地址
#define ADDR_TEST_DI_END            50208       // 测试开关量输入结束地址（本程序实际使用。最大50299）
#define ADDR_TEST_NTC_BGN           50300       // 测试温度起始地址
#define ADDR_TEST_NTC_END           50372       // 测试温度结束地址（本程序实际使用。最大?????）
#define ADDR_TEST_CUR_BGN           51200       // 测试电流起始地址
#define ADDR_TEST_CUR_END           51230       // 测试电流结束地址（本程序实际使用。最大51799）
#define ADDR_TEST_AITYPE_BGN        51800       // 测试AI类型起始地址
#define ADDR_TEST_AITYPE_END        51808       // 测试AI类型结束地址（本程序实际使用。最大51999）
#define ADDR_TEST_AI_BGN            52000       // 测试AI数据起始地址
#define ADDR_TEST_AI_END            52048       // 测试AI数据结束地址（本程序实际使用。最大52599）
#define ADDR_TEST_STEPNOW_BGN       52600       // 测试步进电机当前值起始地址
#define ADDR_TEST_STEPNOW_END       52608       // 测试步进电机当前值结束地址（本程序实际使用。最大53199）
#define ADDR_TEST_STEPSET_BGN       53200       // 测试步进电机设定值起始地址
#define ADDR_TEST_STEPSET_END       53208       // 测试步进电机设定值结束地址（本程序实际使用。最大53799）
#define ADDR_TEST_VERSION_BGN       53800       // 测试版本号起始地址
#define ADDR_TEST_VERSION_END       53816       // 测试版本号结束地址（本程序实际使用。最大53928）
#define ADDR_TEST_COMMU_BGN         56800       // 测试串口通讯起始地址
#define ADDR_TEST_COMMU_END         56845       // 测试串口通讯结束地址（本程序实际使用。最大57699）
#ifdef COM_4G
#define ADDR_4G_TEST_BGN		57200										// 通信数据起始地址
#define _4G_TEST_SIZE			(sizeof(TEST4G)/sizeof(U16))				// 通信数据占用大小																	//和应用程序
#define ADDR_4G_TEST_END		(ADDR_4G_TEST_BGN+_4G_TEST_SIZE)			// 通信数据结束地址
#endif
#define ADDR_TEST_PWM_BGN           57700       // 测试PWM起始地址
#define ADDR_TEST_PWM_END           57720       // 测试PWM结束地址（本程序实际使用。最大57750）
#define ADDR_TEST_INVT_BGN          57730       // 测试PWM起始地址
#define ADDR_TEST_INVT_END          57750       // 测试PWM结束地址（本程序实际使用。最大57750）

/* MODBUS通讯数据个数限制 */
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


#define MAX_HMI				1			// HMI数量(注意：HMI个数不可大于7个) 	 




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


