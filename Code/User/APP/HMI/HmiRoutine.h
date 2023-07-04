/******************************************************************************

          ��Ȩ���� (C), 2001-2016, �����а��յ��Լ����������޹�˾

 ******************************************************************************
  �� �� ��   : HmiRoutine.h
  �� �� ��   : ����
  ��    ��   : zzp
  ��������   : 2021��4��20��
  ����޸�   :
  ��������   : HmiRoutine.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2021��4��20��
    ��    ��   : zzp
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __HMIROUTINE_H__
#define __HMIROUTINE_H__

#ifdef	HMIROUTINE_GBL
#define HMIROUTINE_EXT
#else
#define HMIROUTINE_EXT extern
#endif
/*-----------------����ͷ�ļ� -----------------------------*/



/*-----------------�ⲿ����˵��----------------------------*/



/*-----------------�ⲿ����ԭ��˵��------------------------*/



/*-----------------�ڲ�����ԭ��˵��------------------------*/

/* ԭcloud.h�еĺ궨�� */
#define NORMAL_DATA 		     0x01
#define SELECTED_DATA 		     0x02
#define	NEW		                 0			/* �£������ӷ���WiFi�ı�/������ */
#define	OLD			             1			/* �ɣ��������ӷ���WiFi�ı�/������ */


/* ��ʾ������ */
#define HMI_TYPE_NONE            0  // ��
#define HMI_TYPE_TEXT_NORMAL     1  // �����ı���
#define HMI_TYPE_TEXT_WIFI_YHFY  2  // �ƺӷ���WiFi�ı���
#define HMI_TYPE_TEXT_WIFI_TY    3  // ͿѻWiFi�ı���
#define HMI_TYPE_SEG_NORMAL      4  // ���������
#define HMI_TYPE_SEG_WIFI_YHFY   5  // �ƺӷ���WiFi������
#define HMI_TYPE_SEG_WIFI_TY     6  // ͿѻWiFi������


/*-----------------ȫ�ֱ���--------------------------------*/

/* HMI��ض��� */
/* HMI��ַ��0x30��ʼ(����Ϊ0x30~0x3f) */
#define	HMI_ADDR_NULL		0				/* ��ʾ�ն˿��е�ַ */
#define	HMI_ADDR			0x31			/* ��ʾ�ն�1ͨѶ��ַ */
#define	HMI_2_ADDR			0x32			/* ��ʾ�ն�2ͨѶ��ַ */
#define HMI_ADDR_MASK		0x3f			/* ��ʾ�ն˵�ַ���� */

#define HMI_NEW_ADDR        0x21            /* ��ʾ�ն���Э���ַ */


#define	HMI_1				0				/* ��ʾ�ն�1��� */
#define	HMI_2				1				/* ��ʾ�ն�2��� */


/* HMI������������(��ʾ���ݻ��������)*/
#define	HMI_DISPLAY			0x01			/* ��������Ϊ��ʾ���� */
#define HMI_DATA			0x02			/* ��������Ϊ�������� */

#define HMI_ERR_COM_DLY		6				/* ��ʾ�ն˳���ͨѶ����(��) */
HMIROUTINE_EXT U08 cnt_hmi_com_wait[MAX_HMI];		/* ��ʾ�ն˳���ͨѶ��ʱ */
HMIROUTINE_EXT U08 hmi_r_num;						/* hmi������� */
HMIROUTINE_EXT U08 hmi_t_num;						/* hmi������� */
HMIROUTINE_EXT U08 hmi_next;						/* ��һ��hmi����� */
           
HMIROUTINE_EXT U08 hmi_trans_type ;					/* ������������*/		
HMIROUTINE_EXT U08 hmi_trans_type_bak ;				/* �����������ͱ���*/		
HMIROUTINE_EXT U08 hmi_board_type[MAX_HMI];			/* ǰ��Э�����ͱ�־*/
HMIROUTINE_EXT U08 hmi_type[MAX_HMI];			    /* ��ʾ������*/


#define HMI_BK_FG_SYNC         1                    // ���ư�ͬ����־
#define HMI_BK_SUP_0XFX        1                    // ���ư�֧��0xFx��ַ
HMIROUTINE_EXT U08 hmi_bk_lose_conn[MAX_HMI];       // ���ư������Ͽ�����			   


/* ��ʾ����չ��Ϣ */
HMIROUTINE_EXT U08 hmi_info_sync_flag[MAX_HMI];		// ��ͬ����� 	   
HMIROUTINE_EXT U08 hmi_info_sup_0xFx[MAX_HMI];		// ֧��0xFx��ַ 
HMIROUTINE_EXT U08 hmi_info_dim_dcache[MAX_HMI];	// ֧�ֶ�̬���󻺴� 		   
HMIROUTINE_EXT U08 hmi_info_dim_scache[MAX_HMI];	// ֧�־�̬���󻺴� 	

/* ��ʾ��Ϳѻ��Ϣ */
HMIROUTINE_EXT U08 hmi_info_conn_bk_sig[MAX_HMI];	// ���ӵ�����ź�(���ͻ�����)		   
HMIROUTINE_EXT U08 hmi_info_need_sync_all[MAX_HMI];	// ��Ҫͬ�����й��ܵ�(���ͻ�����)		   
HMIROUTINE_EXT U08 hmi_info_pid_valid[MAX_HMI];		// ��ƷID��Ч	   

HMIROUTINE_EXT U08 hmi_ty_status;			   
HMIROUTINE_EXT U08 hmi_ty_signal;	
HMIROUTINE_EXT U08 hmi_ty_mac[6];	
//MODBUS_EXT U08 hmi_commu_error;				/* ��ʾ�ն�ͨѶ���� */

#define TY_STATUS_SMARTCONFIG       (0)
#define TY_STATUS_APCONFIG          (1)
#define TY_STATUS_CONFIGED          (2)
#define TY_STATUS_LINK_ROTER        (3)
#define TY_STATUS_LINK_CLOUD        (4) // �����ƶ�
#define TY_STATUS_LOW_POWER         (5)


/*-----------------ģ�鼶����------------------------------*/



/*-----------------��������--------------------------------*/



/*-----------------�궨�� ---------------------------------*/


HMIROUTINE_EXT void hmi_routine_init(void);
HMIROUTINE_EXT U08 hmi_deal_request(UART *pUart);
HMIROUTINE_EXT BOOL hmi_rece(UART *pUart);
HMIROUTINE_EXT void hmi_rece_tuya(U08* Buf);
HMIROUTINE_EXT BOOL hmi_trans(UART *pUart);
HMIROUTINE_EXT BOOL hmi_trans_tuya(U08* Buf);


#endif /* __HMIROUTINE_H__ */

