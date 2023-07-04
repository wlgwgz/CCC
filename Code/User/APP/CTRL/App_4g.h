/******************************************************************************

          ��Ȩ���� (C), 2001-2099, �����а��յ��Լ����������޹�˾

 ******************************************************************************
  �� �� ��   : app_4g.c
  �� �� ��   : ����
  ��    ��   : ��С��
  ��������   : 2022��6��9��
  ����޸�   :
  ��������   : 4gӦ������߼�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2022��6��9��
    ��    ��   : ��С��
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef __APP_4G_H__
#define __APP_4G_H__


#ifdef  _APP_4G_GLOBAL_
    #define APP_4G_EXT
#else
    #define APP_4G_EXT extern
#endif
/*-----------------����ͷ�ļ� -----------------------------*/



/*-----------------�ⲿ����˵��----------------------------*/

// ���ڴ洢��4Gģ�������4Gģ����Ϣ
typedef struct{
	U16 type;					// ģ������
	I16 test_sta;				// ����״̬
	U16 csq;					// �ź�ǿ��
	I16 run_sta;				// ����״̬
	U16 bak;					// ����
	U16 ver[MAX_4G_VER_WORD];	// 4Gģ�鹦����汾��
	U16 sn[MAX_4G_SN_WORD];		// 4Gģ��SN��
}APP4GRW;
APP_4G_EXT APP4GRW app_4g_rw;

// ���ڽ�������run_sta
#define APP_4G_STA_NONE          0 // ��
#define APP_4G_STA_NO_MODULE     1 // ��ģ��
#define APP_4G_STA_CONCAT        2 // ������ģ��
#define APP_4G_STA_CONNECT_NET   3 // ����������
#define APP_4G_STA_CONNECT_CLOUD 4 // ��������
#define APP_4G_STA_ALARM         5 // ����

// ���ڴ洢4G�ӿ��ư��������Ϣ
typedef struct{
	U16 cmd;					// ����
	U16 bak;					// ����
}APP4GRO;
APP_4G_EXT APP4GRO app_4g_ro;

// ����4G����cmd��ֵ����
#define INTO_4G_NONE			0	// ��
#define INTO_4G_TEST			1	// ����4G����
#define INTO_4G_APP				2	// ����4GӦ��


/*-----------------�ⲿ����ԭ��˵��------------------------*/



/*-----------------�ڲ�����ԭ��˵��------------------------*/

APP_4G_EXT BOOL app_4g_have(void);
APP_4G_EXT U16  app_4g_commIntProt_time(BOOL fg_need_long_time);
APP_4G_EXT void app_4g_commIntProt_deal(void);
APP_4G_EXT void app_4g_init(void);
APP_4G_EXT void app_4g(void);

#endif /* __APP_4G_H__ */
