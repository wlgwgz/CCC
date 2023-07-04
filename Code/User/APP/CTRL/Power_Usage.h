/******************************************************************************

          ��Ȩ���� (C), 2001-2022, �����а��յ��Լ����������޹�˾

 ******************************************************************************
  �� �� ��   : Power_Usage.h
  �� �� ��   : ����
  ��    ��   : J.H.
  ��������   : 2022��6��6������һ
  ����޸�   :
  ��������   : Power_Usage.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2022��6��6������һ
    ��    ��   : J.H.
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef __POWER_USAGE_H__
#define __POWER_USAGE_H__

#ifdef POWER_USAGE_GLOBAL
    #define PWRU_EXT
#else
    #define PWRU_EXT extern
#endif

/*-----------------����ͷ�ļ� -----------------------------*/



/*-----------------�ⲿ����˵��----------------------------*/



/*-----------------�ⲿ����ԭ��˵��------------------------*/



/*-----------------�ڲ�����ԭ��˵��------------------------*/



/*-----------------ȫ�ֱ���--------------------------------*/



/*-----------------ģ�鼶����------------------------------*/



/*-----------------��������--------------------------------*/




/*-----------------�궨�� ---------------------------------*/

#define POWER_USAGE_WEEK_SIZE                     (7)  /* һ���õ�����¼��Ŀ�� */
#define POWER_USAGE_YEAR_SIZE                     (12) /* һ���õ�����¼��Ŀ�� */

/* ������Ϣ */
typedef struct {
    U08 head;    /* ͷ�α� */
    U08 end;     /* β�α� */
    U08 cnt;     /* �ڵ���� */
} PWRU_QUEUE;

typedef struct tag_POWER_USAGE_INFO
{ 
    U32 timestamp;          /* ʱ��� */
    U16 val_cool;			/* ���䵱ǰֵ */
    U16 val_heat;			/* ���ȵ�ǰֵ */
    U16 val_dhw;			/* ����ˮ��ǰֵ */
} PU_INFO;

/* ����һ���õ������� */
typedef struct {
    PWRU_QUEUE queue;                       /* ������Ϣ */
    PU_INFO node[POWER_USAGE_WEEK_SIZE];	/* �ڵ���Ϣ */
} PU_WEEK_CACHE;

/* ����һ���õ������� */
typedef struct {
    PWRU_QUEUE queue;                       /* ������Ϣ */
    PU_INFO node[POWER_USAGE_YEAR_SIZE];	/* �ڵ���Ϣ */
} PU_YEAR_CACHE;

PWRU_EXT U16 cnt_pwru_save_min;
PWRU_EXT U16 cnt_pwru_cal_sec;
PWRU_EXT PU_WEEK_CACHE pwru_week_cache;     /* ����һ���õ��� 3*7 */
PWRU_EXT PU_YEAR_CACHE pwru_year_cache;     /* ����һ���õ��� 3*12 */

/* ������ */
PWRU_EXT U16 dbg_cnt_pwru_cal_10s;
PWRU_EXT U16 dbg_cnt_pwru_cal_20s;
PWRU_EXT U16 dbg_cnt_pwru_cal_60s;
PWRU_EXT U16 dbg_PU_10s;
PWRU_EXT U16 dbg_PU_20s;
PWRU_EXT U16 dbg_PU_60s;

#define POWER_USAGE_WEEK_BYTES              (sizeof(pwru_week_cache)/sizeof(U08))
#define POWER_USAGE_YEAR_BYTES              (sizeof(pwru_year_cache)/sizeof(U08))

PWRU_EXT void PU_cal_power_usage(void);
PWRU_EXT void PU_init(void);

#endif /* __POWER_USAGE_H__ */
