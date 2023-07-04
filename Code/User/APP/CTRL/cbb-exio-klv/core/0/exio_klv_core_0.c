/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: exio_klv_core_0.c
 �ļ�����: IO��չ���ܺ���0Դ�ļ�
 ������  : J.H.
 ��������: 2022.01.14
*******************************************************************************/
#define EK_CORE_0_GLOBAL
#include "includes.h"

#ifdef EXIO_KLV_MASTER
static EXIO_KLV_DATA m_exio_klv_data[EXIO_KLV_MAX];           /* KLV֡����(����) */

/*******************************************************************************
 �� ��: EK_mst_init()
 �� ��: IO��չ���ʼ������(����)
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void EK_mst_init(void)
{
    EK_SLP_mst_init();
}

/*******************************************************************************
 �� ��: EK_mst_timer()
 �� ��: IO��չ���ʱ��(����)
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void EK_mst_timer(void)
{
    EK_SLP_mst_timer();
}

/*******************************************************************************
 �� ��: EK_mst_fill_request_data()
 �� ��: IO��չ�������������(����)
 �� ��: exio_id--IO��չ���; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void EK_mst_fill_request_data(U08 exio_id)
{
    /* ��ʼ�� */
    ARRCLR(m_exio_klv_data[exio_id].buf);

    /* ����������� */
    EK_SLP_mst_fill_request_handler(exio_id, &m_exio_klv_data[exio_id]);
}

/*******************************************************************************
 �� ��: EK_mst_create_request()
 �� ��: IO��չ�崴������(����)
 �� ��: com_id--���ں�; 
        exio_id--IO��չ���; 
 �� ��: 
 �� ��: ��
*******************************************************************************/
void EK_mst_create_request(U08 com_id, U08 exio_id)
{
    /* ������� */
    EK_mst_fill_request_data(exio_id);   

    /* ������� */
    EKC_mst_insert_request_node(com_id, exio_id);
}

/*******************************************************************************
 �� ��: EK_mst_send_request()
 �� ��: IO��չ�巢����������(����)
 �� ��: tx_buf[]--���ͻ���; 
		device_addr--�豸��ַ; 
 �� ��: 
 �� ��: ��
*******************************************************************************/
U08 EK_mst_send_request(U08 tx_buf[], U08 device_addr)
{
    U08 i;
    U08 len = EXIO_KLV_DATA_BYTES;
    U08 *src_buf = NULL;

    if (EXIO_KLV_ADDR <= device_addr < EXIO_KLV_ADDR+EXIO_KLV_MAX)
    {
        U08 exio_id = device_addr - EXIO_KLV_ADDR;        
        src_buf = (U08*)&m_exio_klv_data[exio_id];
    }
    
    if (src_buf != NULL)
    {
        for (i = 0; i < EXIO_KLV_DATA_BYTES; i++)
        {
            tx_buf[i+3] = src_buf[i];
        }
    }
    else
    {
        tx_buf[3] = 0;
        tx_buf[4] = len = 0;
    }

    tx_buf[1] = HIBYTE(len);
    tx_buf[2] = LOBYTE(len);

    return len+2;
}

/*******************************************************************************
 �� ��: EK_mst_parse_respond()
 �� ��: IO��չ�������Ӧ����(����)
 �� ��: rx_buf[]--���ջ���; 
		device_addr--�豸��ַ; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void EK_mst_parse_respond(const U08 rx_buf[], U08 device_addr)
{
    U08 i;
    U08 exio_id;
    U08 *dest_buf = NULL;
    
    if (device_addr >= EXIO_KLV_ADDR && device_addr < EXIO_KLV_ADDR+EXIO_KLV_MAX)
    {
        exio_id = device_addr - EXIO_KLV_ADDR;
    }

    /* ��ȡ���ջ����� */
    dest_buf = (U08*)&m_exio_klv_data[exio_id];
    for (i = 0; i < EXIO_KLV_DATA_BYTES; i++)
    {
        dest_buf[i] = rx_buf[i+3];
    }

    /* ����Ӧ������ */
    EK_SLP_mst_parse_respond_handler(exio_id, (EXIO_KLV_DATA*)&dest_buf[0]);
}


#else
static EXIO_KLV_DATA s_exio_klv_data;				                /* KLV֡����(�ӻ�) */

/*******************************************************************************
 �� ��: EK_slv_init()
 �� ��: IO��չ���ʼ������(�ӻ�)
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void EK_slv_init(void)
{
    EK_SLP_slv_init();
}

/*******************************************************************************
 �� ��: EK_slv_timer()
 �� ��: IO��չ���ʱ��(�ӻ�)
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void EK_slv_timer(void)
{
    EK_SLP_slv_timer();
}

/*******************************************************************************
 �� ��: EK_slv_err_handler()
 �� ��: IO��չ����ϴ���(�ӻ�)
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void EK_slv_err_handler(void)
{
    EK_SLP_slv_commu_err_handler();
}

/*******************************************************************************
 �� ��: EK_slv_parse_request_data()
 �� ��: IO��չ�������������(�ӻ�)
 �� ��: rx_buf[]--���ջ���; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void EK_slv_parse_request_data(U08 rx_buf[])
{
    U08 i;
    U08 *dest_buf = NULL;

    /* ��ȡ���ջ����� */
    dest_buf = (U08 *)&s_exio_klv_data;
    for (i = 0; i < EXIO_KLV_DATA_BYTES; i++)
    {
        dest_buf[i] = rx_buf[i+3];
    }

    /* ������������ */
    EK_SLP_slv_parse_request_handler((EXIO_KLV_DATA*)&dest_buf[0]);
}

/*******************************************************************************
 �� ��: EK_slv_fill_respond_data()
 �� ��: IO��չ�������Ӧ����(�ӻ�)
 �� ��: tx_buf[]--���ͻ���; 
 �� ��: ��������ֽ���
 �� ��: ��
*******************************************************************************/
static U16 EK_slv_fill_respond_data(U08 tx_buf[])
{
    U08 i;
    U16 len;
    U08 *src_buf = NULL;

    /* ����������ݻ��� */
    ARRCLR(s_exio_klv_data.buf);

    /* ������������ */
    src_buf = (U08 *)&s_exio_klv_data;
    EK_SLP_slv_fill_respond_handler((EXIO_KLV_DATA*)&src_buf[0]);

    /* ��䷢�ͻ��� */
    if (src_buf != NULL)
    {
        for (i = 0; i < EXIO_KLV_DATA_BYTES; i++)
        {
            tx_buf[i+3] = src_buf[i];
        }
    }
    len = EXIO_KLV_DATA_BYTES;

    tx_buf[1] = HIBYTE(len);
    tx_buf[2] = LOBYTE(len);

    return len + 2;
}

/*******************************************************************************
 �� ��: EK_slv_send_respond()
 �� ��: �ӻ�(IO��չ��)д�����ݲ��ظ���Ӧ����
 �� ��: rx_buf--���ջ���; 
		tx_buf--���ͻ���; 
		tx_byte--�����ֽ���; 
 �� ��: ͨѶ״̬
 �� ��: ��
*******************************************************************************/
U08 EK_slv_send_respond(U08 *rx_buf, U08 *tx_buf, U16 *tx_byte)
{
    U08 com_err = MB_NO_ERR;    /* ����״̬ */
    U16 len = 0;
    
    /* д�� */
    EK_slv_parse_request_data(&rx_buf[1]);

    /* ��ȡ */
    len = EK_slv_fill_respond_data(&tx_buf[1]);

    *tx_byte = 1+1+len; // ��ַ,������,����

    return com_err;
}

#endif

