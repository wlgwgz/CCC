/******************************************************************************

          版权所有 (C), 2001-2016, 广州市邦普电脑技术开发有限公司

 ******************************************************************************
  文 件 名   : HmiRoutine.h
  版 本 号   : 初稿
  作    者   : zzp
  生成日期   : 2021年4月20日
  最近修改   :
  功能描述   : HmiRoutine.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2021年4月20日
    作    者   : zzp
    修改内容   : 创建文件

******************************************************************************/
#ifndef __HMIROUTINE_H__
#define __HMIROUTINE_H__

#ifdef	HMIROUTINE_GBL
#define HMIROUTINE_EXT
#else
#define HMIROUTINE_EXT extern
#endif
/*-----------------包含头文件 -----------------------------*/



/*-----------------外部变量说明----------------------------*/



/*-----------------外部函数原型说明------------------------*/



/*-----------------内部函数原型说明------------------------*/

/* 原cloud.h中的宏定义 */
#define NORMAL_DATA 		     0x01
#define SELECTED_DATA 		     0x02
#define	NEW		                 0			/* 新：带银河风云WiFi文本/段码屏 */
#define	OLD			             1			/* 旧：不带银河风云WiFi文本/段码屏 */


/* 显示屏类型 */
#define HMI_TYPE_NONE            0  // 无
#define HMI_TYPE_TEXT_NORMAL     1  // 常规文本屏
#define HMI_TYPE_TEXT_WIFI_YHFY  2  // 云河风云WiFi文本屏
#define HMI_TYPE_TEXT_WIFI_TY    3  // 涂鸦WiFi文本屏
#define HMI_TYPE_SEG_NORMAL      4  // 常规段码屏
#define HMI_TYPE_SEG_WIFI_YHFY   5  // 云河风云WiFi段码屏
#define HMI_TYPE_SEG_WIFI_TY     6  // 涂鸦WiFi段码屏


/*-----------------全局变量--------------------------------*/

/* HMI相关定义 */
/* HMI地址从0x30开始(区间为0x30~0x3f) */
#define	HMI_ADDR_NULL		0				/* 显示终端空闲地址 */
#define	HMI_ADDR			0x31			/* 显示终端1通讯地址 */
#define	HMI_2_ADDR			0x32			/* 显示终端2通讯地址 */
#define HMI_ADDR_MASK		0x3f			/* 显示终端地址掩码 */

#define HMI_NEW_ADDR        0x21            /* 显示终端新协议地址 */


#define	HMI_1				0				/* 显示终端1序号 */
#define	HMI_2				1				/* 显示终端2序号 */


/* HMI传输数据类型(显示数据或机组数据)*/
#define	HMI_DISPLAY			0x01			/* 传输数据为显示数据 */
#define HMI_DATA			0x02			/* 传输数据为机组数据 */

#define HMI_ERR_COM_DLY		6				/* 显示终端尝试通讯周期(秒) */
HMIROUTINE_EXT U08 cnt_hmi_com_wait[MAX_HMI];		/* 显示终端尝试通讯计时 */
HMIROUTINE_EXT U08 hmi_r_num;						/* hmi接收序号 */
HMIROUTINE_EXT U08 hmi_t_num;						/* hmi发送序号 */
HMIROUTINE_EXT U08 hmi_next;						/* 下一个hmi的序号 */
           
HMIROUTINE_EXT U08 hmi_trans_type ;					/* 传输数据类型*/		
HMIROUTINE_EXT U08 hmi_trans_type_bak ;				/* 传输数据类型备份*/		
HMIROUTINE_EXT U08 hmi_board_type[MAX_HMI];			/* 前板协议类型标志*/
HMIROUTINE_EXT U08 hmi_type[MAX_HMI];			    /* 显示屏类型*/


#define HMI_BK_FG_SYNC         1                    // 控制板同步标志
#define HMI_BK_SUP_0XFX        1                    // 控制板支持0xFx地址
HMIROUTINE_EXT U08 hmi_bk_lose_conn[MAX_HMI];       // 控制板与屏断开连接			   


/* 显示屏扩展信息 */
HMIROUTINE_EXT U08 hmi_info_sync_flag[MAX_HMI];		// 已同步后板 	   
HMIROUTINE_EXT U08 hmi_info_sup_0xFx[MAX_HMI];		// 支持0xFx地址 
HMIROUTINE_EXT U08 hmi_info_dim_dcache[MAX_HMI];	// 支持动态点阵缓存 		   
HMIROUTINE_EXT U08 hmi_info_dim_scache[MAX_HMI];	// 支持静态点阵缓存 	

/* 显示屏涂鸦信息 */
HMIROUTINE_EXT U08 hmi_info_conn_bk_sig[MAX_HMI];	// 连接到后板信号(回送或清零)		   
HMIROUTINE_EXT U08 hmi_info_need_sync_all[MAX_HMI];	// 需要同步所有功能点(回送或清零)		   
HMIROUTINE_EXT U08 hmi_info_pid_valid[MAX_HMI];		// 产品ID有效	   

HMIROUTINE_EXT U08 hmi_ty_status;			   
HMIROUTINE_EXT U08 hmi_ty_signal;	
HMIROUTINE_EXT U08 hmi_ty_mac[6];	
//MODBUS_EXT U08 hmi_commu_error;				/* 显示终端通讯故障 */

#define TY_STATUS_SMARTCONFIG       (0)
#define TY_STATUS_APCONFIG          (1)
#define TY_STATUS_CONFIGED          (2)
#define TY_STATUS_LINK_ROTER        (3)
#define TY_STATUS_LINK_CLOUD        (4) // 连上云端
#define TY_STATUS_LOW_POWER         (5)


/*-----------------模块级变量------------------------------*/



/*-----------------常量定义--------------------------------*/



/*-----------------宏定义 ---------------------------------*/


HMIROUTINE_EXT void hmi_routine_init(void);
HMIROUTINE_EXT U08 hmi_deal_request(UART *pUart);
HMIROUTINE_EXT BOOL hmi_rece(UART *pUart);
HMIROUTINE_EXT void hmi_rece_tuya(U08* Buf);
HMIROUTINE_EXT BOOL hmi_trans(UART *pUart);
HMIROUTINE_EXT BOOL hmi_trans_tuya(U08* Buf);


#endif /* __HMIROUTINE_H__ */

