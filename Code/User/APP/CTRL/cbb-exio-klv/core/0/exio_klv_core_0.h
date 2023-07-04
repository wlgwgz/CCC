/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: exio_klv_core_0.h
 文件描述: IO扩展功能核心0头文件
 创建人  : J.H.
 创建日期: 2022.01.14
*******************************************************************************/

#ifndef __EK_CORE_0_H__
#define __EK_CORE_0_H__

#ifdef EK_CORE_0_GLOBAL
    #define EK_CORE_0_EXT
#else
    #define EK_CORE_0_EXT extern
#endif


/* KLV数据 */
#define EXIO_KLV_DATA_BUF_BYTES               (242)
typedef struct tag_EXIO_KLV_DATA
{
    U08 stage;                                /* KLV通信阶段 */
    U08 buf[EXIO_KLV_DATA_BUF_BYTES];		  /* 数据缓存 */
} EXIO_KLV_DATA;
#define EXIO_KLV_DATA_BYTES                   (sizeof(EXIO_KLV_DATA))

/****************************************/
/* ------------主机调用接口------------ */
/****************************************/
EK_CORE_0_EXT void EK_mst_init(void);
EK_CORE_0_EXT void EK_mst_timer(void);
EK_CORE_0_EXT void EK_mst_parse_respond(const U08 rx_buf[], U08 device_addr);
EK_CORE_0_EXT void EK_mst_create_request(U08 com_id, U08 exio_id);
EK_CORE_0_EXT U08 EK_mst_send_request(U08 tx_buf[], U08 device_addr);


/****************************************/
/* ------------从机调用接口------------ */
/****************************************/
EK_CORE_0_EXT void EK_slv_init(void);
EK_CORE_0_EXT void EK_slv_timer(void);
EK_CORE_0_EXT void EK_slv_err_handler(void);
EK_CORE_0_EXT U08 EK_slv_send_respond(U08 *rx_buf, U08 *tx_buf, U16 *tx_byte);

#endif

