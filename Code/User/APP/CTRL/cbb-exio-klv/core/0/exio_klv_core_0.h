/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: exio_klv_core_0.h
 �ļ�����: IO��չ���ܺ���0ͷ�ļ�
 ������  : J.H.
 ��������: 2022.01.14
*******************************************************************************/

#ifndef __EK_CORE_0_H__
#define __EK_CORE_0_H__

#ifdef EK_CORE_0_GLOBAL
    #define EK_CORE_0_EXT
#else
    #define EK_CORE_0_EXT extern
#endif


/* KLV���� */
#define EXIO_KLV_DATA_BUF_BYTES               (242)
typedef struct tag_EXIO_KLV_DATA
{
    U08 stage;                                /* KLVͨ�Ž׶� */
    U08 buf[EXIO_KLV_DATA_BUF_BYTES];		  /* ���ݻ��� */
} EXIO_KLV_DATA;
#define EXIO_KLV_DATA_BYTES                   (sizeof(EXIO_KLV_DATA))

/****************************************/
/* ------------�������ýӿ�------------ */
/****************************************/
EK_CORE_0_EXT void EK_mst_init(void);
EK_CORE_0_EXT void EK_mst_timer(void);
EK_CORE_0_EXT void EK_mst_parse_respond(const U08 rx_buf[], U08 device_addr);
EK_CORE_0_EXT void EK_mst_create_request(U08 com_id, U08 exio_id);
EK_CORE_0_EXT U08 EK_mst_send_request(U08 tx_buf[], U08 device_addr);


/****************************************/
/* ------------�ӻ����ýӿ�------------ */
/****************************************/
EK_CORE_0_EXT void EK_slv_init(void);
EK_CORE_0_EXT void EK_slv_timer(void);
EK_CORE_0_EXT void EK_slv_err_handler(void);
EK_CORE_0_EXT U08 EK_slv_send_respond(U08 *rx_buf, U08 *tx_buf, U16 *tx_byte);

#endif

