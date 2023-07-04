/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: Test_fac.h
 �ļ�����: 
 ������  : 
 ��������: 
*******************************************************************************/
#ifndef TEST_FAC_H
#define TEST_FAC_H

  
#ifdef  TEST_FAC_GLOBAL
 #define TF_EXT
#else
 #define TF_EXT extern
#endif


/* �̼�����:���Ĵ˽ṹ�������̼�������ͬ������ */
#define TF_SRC_NUM 15
typedef struct TEST_FAC
{
    /* ����д */
    U16 type;				/* ���ͣ�0���̼죻1�ڻ��̼죻2����̼죻3�����̼� */
    U16 cmd;				/* ���bit��0�̼졢1��ѹ���ء�2�̵�����3WiFi��4NTC */
    U16 sw;					/* ���뿪�� */
    U16 addr;				/* ���̵�ַ */

    /* ��ֻ�� */
    U16 err;				/* ���ϣ�0�޹��ϣ�1�й��ϣ��̼����账������� */
    U16 str_ver_inner[32];  /* �ڻ��汾�� */
    U16 str_ver_outer[32];  /* ����汾�� */
    U16 str_mac[32];		/* ��ʾ���� */
    U16 str_sta[32];		/* ��ʾ�ı����̼�״̬ */
    U16 str_err[32];		/* ��ʾ�ı����̼���� */

    struct
    {
        U16 str[8];			/* ����:7�ַ�+1������ */
        I16 val;			/* ֵ */
        U16 attrib;			/* ����,����SH_sign_local2sh() */
    }src[TF_SRC_NUM];

}TEST_FAC_STRU;
TF_EXT TEST_FAC_STRU test_fac;

/* �̼����� */
#define TF_TYPE_IDLE 0			/* ���̼� */
#define TF_TYPE_IN   1			/* �ڻ��̼� */
#define TF_TYPE_OUT  2			/* ����̼� */
#define TF_TYPE_ALL  3			/* �����̼� */
#define TF_IS_IDLE              (test_fac.type == TF_TYPE_IDLE)
#define TF_IS_IN                (test_fac.type == TF_TYPE_IN)	
#define TF_IS_OUT               (test_fac.type == TF_TYPE_OUT)	
#define TF_IS_ALL               (test_fac.type == TF_TYPE_ALL)	

#define TF_HAS_INNER     (TF_IS_IN  || TF_IS_ALL) /* �ڻ��������̼� */
#define TF_HAS_OUTER     (TF_IS_OUT || TF_IS_ALL) /* ����������̼� */

/* ���� */
#define TF_EN                   (GETBIT(test_fac.cmd, 0))	        /* �����̼� */
#define TF_EN_HDI               (GETBIT(test_fac.cmd, 1))	        /* ��ѹ���� */    
#define TF_EN_DO                (GETBIT(test_fac.cmd, 2))	        /* �̵��� */   
#define TF_EN_WIFI              (GETBIT(test_fac.cmd, 3))	        /* WiFi */     
#define TF_EN_NTC               (GETBIT(test_fac.cmd, 4))	        /* ǿ��NTC */ 

#define TF_VAL(v1, v2)     (TF_EN ? (v2) : (v1))   /* v1��������v2���̼�ʹ��ֵ */
#define TF_START_INNER     (TF_EN && TF_HAS_INNER) /* �����ڻ��̼� */
#define TF_START_OUTER     (TF_EN && TF_HAS_OUTER) /* ��������̼� */

#define TF_OPEN_IBH     (TF_START_INNER) /* �̼쿪���� */
#define TF_OPEN_PUMP    (TF_START_INNER) /* �̼쿪ˮ�� */
#define TF_OPEN_COMP    (TF_START_OUTER) /* �̼쿪ѹ�� */

#define TF_PUMP_ON_DLY  5	/* ˮ�����ؼ����ʱ */
#define TF_ERR_FLOW_DLY 10	/* ˮ�����ر�����ʱ */

#define TF_FAN_ON_DLY   0	/* �������ʱ */
#define TF_4V_ON_DLY    0	/* ����ͨ����ʱ */

#define TF_LP_CHK_DLY           0	/* ��ѹ�����ʱ */
#define TF_LP_ERR_DLY           30	/* ��ѹ������ʱ */

TF_EXT I16 TF_comp_freq;
TF_EXT I16 TF_fan_freq;
TF_EXT I16 TF_eev_step;
TF_EXT U16 TF_wifi_sta;

void TF_task(void);

#endif
