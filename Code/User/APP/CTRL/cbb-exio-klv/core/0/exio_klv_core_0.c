/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: exio_klv_core_0.c
 文件描述: IO扩展功能核心0源文件
 创建人  : J.H.
 创建日期: 2022.01.14
*******************************************************************************/
#define EK_CORE_0_GLOBAL
#include "includes.h"

#ifdef EXIO_KLV_MASTER
static EXIO_KLV_DATA m_exio_klv_data[EXIO_KLV_MAX];           /* KLV帧数据(主机) */

/*******************************************************************************
 函 数: EK_mst_init()
 功 能: IO扩展板初始化操作(主机)
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void EK_mst_init(void)
{
    EK_SLP_mst_init();
}

/*******************************************************************************
 函 数: EK_mst_timer()
 功 能: IO扩展板计时器(主机)
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void EK_mst_timer(void)
{
    EK_SLP_mst_timer();
}

/*******************************************************************************
 函 数: EK_mst_fill_request_data()
 功 能: IO扩展板填充请求数据(主机)
 参 数: exio_id--IO扩展板号; 
 返 回: 空
 其 它: 空
*******************************************************************************/
static void EK_mst_fill_request_data(U08 exio_id)
{
    /* 初始化 */
    ARRCLR(m_exio_klv_data[exio_id].buf);

    /* 填充请求数据 */
    EK_SLP_mst_fill_request_handler(exio_id, &m_exio_klv_data[exio_id]);
}

/*******************************************************************************
 函 数: EK_mst_create_request()
 功 能: IO扩展板创建请求(主机)
 参 数: com_id--串口号; 
        exio_id--IO扩展板号; 
 返 回: 
 其 它: 空
*******************************************************************************/
void EK_mst_create_request(U08 com_id, U08 exio_id)
{
    /* 填充数据 */
    EK_mst_fill_request_data(exio_id);   

    /* 插入队列 */
    EKC_mst_insert_request_node(com_id, exio_id);
}

/*******************************************************************************
 函 数: EK_mst_send_request()
 功 能: IO扩展板发送请求数据(主机)
 参 数: tx_buf[]--发送缓存; 
		device_addr--设备地址; 
 返 回: 
 其 它: 空
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
 函 数: EK_mst_parse_respond()
 功 能: IO扩展板解析响应数据(主机)
 参 数: rx_buf[]--接收缓存; 
		device_addr--设备地址; 
 返 回: 空
 其 它: 空
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

    /* 读取接收缓冲区 */
    dest_buf = (U08*)&m_exio_klv_data[exio_id];
    for (i = 0; i < EXIO_KLV_DATA_BYTES; i++)
    {
        dest_buf[i] = rx_buf[i+3];
    }

    /* 解析应答数据 */
    EK_SLP_mst_parse_respond_handler(exio_id, (EXIO_KLV_DATA*)&dest_buf[0]);
}


#else
static EXIO_KLV_DATA s_exio_klv_data;				                /* KLV帧数据(从机) */

/*******************************************************************************
 函 数: EK_slv_init()
 功 能: IO扩展板初始化操作(从机)
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void EK_slv_init(void)
{
    EK_SLP_slv_init();
}

/*******************************************************************************
 函 数: EK_slv_timer()
 功 能: IO扩展板计时器(从机)
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void EK_slv_timer(void)
{
    EK_SLP_slv_timer();
}

/*******************************************************************************
 函 数: EK_slv_err_handler()
 功 能: IO扩展板故障处理(从机)
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void EK_slv_err_handler(void)
{
    EK_SLP_slv_commu_err_handler();
}

/*******************************************************************************
 函 数: EK_slv_parse_request_data()
 功 能: IO扩展板解析请求数据(从机)
 参 数: rx_buf[]--接收缓存; 
 返 回: 空
 其 它: 空
*******************************************************************************/
static void EK_slv_parse_request_data(U08 rx_buf[])
{
    U08 i;
    U08 *dest_buf = NULL;

    /* 读取接收缓冲区 */
    dest_buf = (U08 *)&s_exio_klv_data;
    for (i = 0; i < EXIO_KLV_DATA_BYTES; i++)
    {
        dest_buf[i] = rx_buf[i+3];
    }

    /* 解析请求数据 */
    EK_SLP_slv_parse_request_handler((EXIO_KLV_DATA*)&dest_buf[0]);
}

/*******************************************************************************
 函 数: EK_slv_fill_respond_data()
 功 能: IO扩展板填充响应数据(从机)
 参 数: tx_buf[]--发送缓存; 
 返 回: 填充数据字节数
 其 它: 空
*******************************************************************************/
static U16 EK_slv_fill_respond_data(U08 tx_buf[])
{
    U08 i;
    U16 len;
    U08 *src_buf = NULL;

    /* 清空数据内容缓存 */
    ARRCLR(s_exio_klv_data.buf);

    /* 更新数据内容 */
    src_buf = (U08 *)&s_exio_klv_data;
    EK_SLP_slv_fill_respond_handler((EXIO_KLV_DATA*)&src_buf[0]);

    /* 填充发送缓存 */
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
 函 数: EK_slv_send_respond()
 功 能: 从机(IO扩展板)写入数据并回复响应数据
 参 数: rx_buf--接收缓冲; 
		tx_buf--发送缓冲; 
		tx_byte--发送字节数; 
 返 回: 通讯状态
 其 它: 空
*******************************************************************************/
U08 EK_slv_send_respond(U08 *rx_buf, U08 *tx_buf, U16 *tx_byte)
{
    U08 com_err = MB_NO_ERR;    /* 故障状态 */
    U16 len = 0;
    
    /* 写入 */
    EK_slv_parse_request_data(&rx_buf[1]);

    /* 读取 */
    len = EK_slv_fill_respond_data(&tx_buf[1]);

    *tx_byte = 1+1+len; // 地址,功能码,数据

    return com_err;
}

#endif

