/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: pumpf.h
 �ļ�����: ��Ƶˮ�ù���ͷ�ļ�
 ������  : J.H.
 ��������: 2022.07.10
*******************************************************************************/

#ifndef __PUMPF_H__
#define __PUMPF_H__

#ifdef PUMPF_CTRL_GLOBAL
    #define PUMPF_EXT
#else
    #define PUMPF_EXT extern
#endif

/* ���� */
#define PUMPF_CTRL_OFF                           (0)		/* ���� */
#define PUMPF_CTRL_RPM                           (1)		/* ��ת�� */
#define PUMPF_CTRL_FLOW                          (2)		/* ������ */
#define PUMPF_CTRL_PWR_ON_ONLY                   (3)		/* ���õ� */
#define PUMPF_CTRL_POWER                         (4)		/* �ع��� */
#define PUMPF_CTRL_MAX                           (5)

/* ˮ����Ϣȱʧ���� */
#define PUMPF_RPM_NOEXIST                        (8888)		/* ת����Ϣȱʧ */
#define PUMPF_PWM_IN_NOEXIST                     (9999)		/* PWM�����ź���Ϣȱʧ */

/* �ͺ� */
#define PUMPF_00_PARA_9_IPWM1                    (0)        /* ���� Para 15-130/9-87/iPWM1-12 */
#define PUMPF_01_APM25_9_130_PWM                 (1)        /* �½� APM25-9-130PWM */
#define PUMPF_02_GPA25_9HW_130                   (2)        /* �»� GPA25-9HW/130 */
#define PUMPF_03_APF25_12_130FPWM1               (3)		/* �½� APF25-12-130FPWM1 */
#define PUMPF_04_APF25_12_130PWM1                (4)		/* �½� APF25-12-130PWM1 */
#define PUMPF_05_GPA25_11H                       (5)		/* �»� GPA25_11H1 */
#define PUMPF_06_HBG_25_125_130P                 (6)		/* ��ķ�� HBG-25-125-130P */
#define PUMPF_07_UPM4XLK_25_90_130               (7)		/* ������ UPM4XLK 25-90 130 */
#define PUMPF_MODEL_MAX                          (8)

/* 00: ���� Para 15-130/9-87/iPWM1-12 */
/* ת�ٷ�Χ */
#define PUMPF_00_RPM_MIN                         (500)      /* rpm */
#define PUMPF_00_RPM_MAX                         (4540)     /* rpm */
/* ������Χ */
#define PUMPF_00_FLOW_MIN                        (0)        /* L/H */
#define PUMPF_00_FLOW_MAX                        (4500)     /* L/H */
/* PWM���� */
#define PUMPF_00_PWM_OUT_RPM_MAX_BGN             (0)        /* >0%: ���ת��������ʼ�� */
#define PUMPF_00_PWM_OUT_RPM_MAX_END             (50)       /* <=5%: ���ת�����н����� */
#define PUMPF_00_PWM_OUT_RPM_MIN_BGN             (850)      /* >85%: ���ת��������ʼ�� */
#define PUMPF_00_PWM_OUT_RPM_MIN_END             (880)      /* <=88%: ���ת�����н����� */
#define PUMPF_00_PWM_OUT_IDLE                    (980)      /* 93��100%: ����(ͣ��)��ֹͣ���� */
/* PWM���� */
#define PUMPF_00_PWM_IN_SC                       (0)        /* 0%: iPWM�źŽӿڶ�· */
#define PUMPF_00_PWM_IN_IDLE                     (20)       /* 2%: ������ͣ������׼������ */
#define PUMPF_00_PWM_IN_FLOW_INFO_MIN            (50)       /* 5%: ������Ϣ������Ͱٷֱ� */
#define PUMPF_00_PWM_IN_FLOW_INFO_MAX            (750)      /* 75%: ������Ϣ������߰ٷֱ� */
#define PUMPF_00_PWM_IN_WARN                     (800)      /* 80%: ���棬δ���������״̬ */
#define PUMPF_00_PWM_IN_ERR_1                    (850)      /* 85%: ������ͣ������Ƿѹ/��ѹ/�쳣�ⲿˮ�� */
#define PUMPF_00_PWM_IN_ERR_2                    (900)      /* 90%: ������ͣ��������������ʧЧ/���ڰ�װ��м/�¶Ȳ��� */
#define PUMPF_00_PWM_IN_ERR_3                    (950)      /* 95%: ������ͣ�����������𻵹��� */
#define PUMPF_00_PWM_IN_OC                       (1000)     /* 100%: iPWM�źŽӿڶ�· */

/* 01: �½� APM25-9-130 */
/* ת�ٷ�Χ */
#define PUMPF_01_RPM_MIN                         (1000)     /* rpm */
#define PUMPF_01_RPM_MAX                         (4500)     /* rpm */
/* ������Χ */
#define PUMPF_01_FLOW_MIN                        (300)      /* L/H */
#define PUMPF_01_FLOW_MAX                        (4500)     /* L/H */
/* PWM���� */
#define PUMPF_01_PWM_OUT_RPM_MAX_BGN             (0)        /* >0%: ���ת��������ʼ�� */
#define PUMPF_01_PWM_OUT_RPM_MAX_END             (50)       /* <=5%: ���ת�����н����� */
#define PUMPF_01_PWM_OUT_RPM_MIN_BGN             (850)      /* >85%: ���ת��������ʼ�� */
#define PUMPF_01_PWM_OUT_RPM_MIN_END             (880)      /* <=88%: ���ת�����н����� */
#define PUMPF_01_PWM_OUT_IDLE                    (980)      /* 93��100%: ����(ͣ��)��ֹͣ���� */
/* PWM���� */
#define PUMPF_01_PWM_IN_SC                       (0)        /* 0%: PWM�����ź�ȱʧ */
#define PUMPF_01_PWM_IN_IDLE                     (20)       /* 2%: ����(ͣ��)��׼������ */
#define PUMPF_01_PWM_IN_NO_WATER_RUN             (30)       /* 3%: ��ת��������� */
#define PUMPF_01_PWM_IN_FLOW_INFO_MIN            (50)       /* 5%: ������Ϣ������Ͱٷֱ� */
#define PUMPF_01_PWM_IN_FLOW_INFO_MAX            (700)      /* 70%: ������Ϣ������߰ٷֱ� */
#define PUMPF_01_PWM_IN_WARN                     (800)      /* 80%: ���棬�͵�ѹ/�ߵ�ѹ */
#define PUMPF_01_PWM_IN_ERR_1                    (850)      /* 85%: ����(ͣ��)��Ƿѹ/��ѹ���� */
#define PUMPF_01_PWM_IN_ERR_2                    (900)      /* 90%: ����(ͣ��)����ת/��ת���� */
#define PUMPF_01_PWM_IN_ERR_3                    (950)      /* 95%: ����(ͣ��)��ȱ��/�������� */
#define PUMPF_01_PWM_IN_OC                       (1000)     /* 100%: ��PWM�ź����� */

/* 02: �»� GPA25-9HW/130 */
/* ת�ٷ�Χ */
#define PUMPF_02_RPM_MIN                         (1200)     /* rpm */
#define PUMPF_02_RPM_MAX                         (6900)     /* rpm */
/* ������Χ */
#define PUMPF_02_FLOW_MIN                        (0)        /* L/H */
#define PUMPF_02_FLOW_MAX                        (3200)     /* L/H */
/* PWM���� */
#define PUMPF_02_PWM_OUT_RPM_MAX_BGN             (0)        /* >0%: ���ת��������ʼ�� */
#define PUMPF_02_PWM_OUT_RPM_MAX_END             (50)       /* <=5%: ���ת�����н����� */
#define PUMPF_02_PWM_OUT_RPM_MIN_BGN             (850)      /* >85%: ���ת��������ʼ�� */
#define PUMPF_02_PWM_OUT_RPM_MIN_END             (880)      /* <=88%: ���ת�����н����� */
#define PUMPF_02_PWM_OUT_IDLE                    (980)      /* 93��99%: ����(ͣ��)��ֹͣ���� */
/* PWM���� */
#define PUMPF_02_PWM_IN_SC                       (0)        /* 0%: iPWM�źŽӿڶ�· */
#define PUMPF_02_PWM_IN_IDLE                     (20)       /* 2%: ������ͣ������׼������ */
#define PUMPF_02_PWM_IN_FLOW_INFO_MIN            (60)       /* 6%: ������Ϣ������Ͱٷֱ� */
#define PUMPF_02_PWM_IN_FLOW_INFO_MAX            (500)      /* 50%: ������Ϣ������߰ٷֱ� */
#define PUMPF_02_PWM_IN_WARN                     (800)      /* 80%: ���棬δ���������״̬ */
#define PUMPF_02_PWM_IN_ERR_1                    (850)      /* 85%: ������ͣ������Ƿѹ/��ѹ/�쳣�ⲿˮ�� */
#define PUMPF_02_PWM_IN_ERR_2                    (900)      /* 90%: ������ͣ������ˮ�ô������أ�ȱ��/����/���±��� */
#define PUMPF_02_PWM_IN_ERR_3                    (950)      /* 95%: ������ͣ����������ͣ��(ˮ�ö�ת) */
#define PUMPF_02_PWM_IN_OC                       (1000)     /* 100%: ��PWM�ź����� */

/* 03: �½� APM25-12-130FPWM1 */
/* ת�ٷ�Χ */
#define PUMPF_03_RPM_MIN                         (1750)                 /* rpm */
#define PUMPF_03_RPM_MAX                         (5500)                 /* rpm */
/* ������Χ */
#define PUMPF_03_FLOW_MIN                        (0)                    /* L/H */
#define PUMPF_03_FLOW_MAX                        (4000)                 /* L/H */
/* PWM���� */
#define PUMPF_03_PWM_OUT_RPM_MAX_BGN             (0)                    /* >0%: ���ת��������ʼ�� */
#define PUMPF_03_PWM_OUT_RPM_MAX_END             (100)                  /* <=10%: ���ת�����н����� */
#define PUMPF_03_PWM_OUT_RPM_MIN_BGN             (840)                  /* >84%: ���ת��������ʼ�� */
#define PUMPF_03_PWM_OUT_RPM_MIN_END             (910)                  /* <=91%: ���ת�����н����� */
#define PUMPF_03_PWM_OUT_IDLE                    (980)                  /* 95��100%: ����(ͣ��)��ֹͣ���� */
/* PWM���� */
#define PUMPF_03_PWM_IN_SC                       (PUMPF_PWM_IN_NOEXIST)	/* PWM�����ź�ȱʧ[��] */
#define PUMPF_03_PWM_IN_IDLE                     (950)                  /* 95%: ����(ͣ��)��׼������ */
#define PUMPF_03_PWM_IN_FLOW_INFO_MIN            (0)                    /* 0%: ������Ϣ������Ͱٷֱ� */
#define PUMPF_03_PWM_IN_FLOW_INFO_MAX            (700)                  /* 70%: ������Ϣ������߰ٷֱ� */
#define PUMPF_03_PWM_IN_WARN                     (750)                  /* 75%: ���棬�͵�ѹ/�ߵ�ѹ */
#define PUMPF_03_PWM_IN_ERR_1                    (800)                  /* 80%: ����(ͣ��)��Ƿѹ/��ѹ���� */
#define PUMPF_03_PWM_IN_ERR_2                    (900)                  /* 90%: ����(ͣ��)����ת/��ת���� */
#define PUMPF_03_PWM_IN_ERR_3                    (850)                  /* 85%: ����(ͣ��)��ȱ��/�������� */
#define PUMPF_03_PWM_IN_OC                       (1000)	                /* ��PWM�ź�����[��] */

/* 04: �½� APF25-12-130PWM1 */
/* ת�ٷ�Χ */
#define PUMPF_04_RPM_MIN                         (1000)                 /* rpm */
#define PUMPF_04_RPM_MAX                         (5300)                 /* rpm */
/* ���ʷ�Χ */
#define PUMPF_04_POW_MIN                         (0)                    /* W */
#define PUMPF_04_POW_MAX                         (180)                  /* W */
/* PWM���� */
#define PUMPF_04_PWM_OUT_RPM_MAX_BGN             (0)                    /* >0%: ���ת��������ʼ�� */
#define PUMPF_04_PWM_OUT_RPM_MAX_END             (100)                  /* <=10%: ���ת�����н����� */
#define PUMPF_04_PWM_OUT_RPM_MIN_BGN             (840)                  /* >84%: ���ת��������ʼ�� */
#define PUMPF_04_PWM_OUT_RPM_MIN_END             (910)                  /* <=91%: ���ת�����н����� */
#define PUMPF_04_PWM_OUT_IDLE                    (980)                  /* 95��100%: ����(ͣ��)��ֹͣ���� */
/* PWM���� */
#define PUMPF_04_PWM_IN_SC                       (PUMPF_PWM_IN_NOEXIST)	/* PWM�����ź�ȱʧ[��] */
#define PUMPF_04_PWM_IN_IDLE                     (950)                  /* 95%: ����(ͣ��)��׼������ */
#define PUMPF_04_PWM_IN_POWER_INFO_MIN           (0)                    /* 0%:  ������Ϣ������Ͱٷֱ� */
#define PUMPF_04_PWM_IN_POWER_INFO_MAX           (700)                  /* 70%: ������Ϣ������߰ٷֱ� */
#define PUMPF_04_PWM_IN_WARN                     (750)                  /* 75%: ���棬�͵�ѹ/�ߵ�ѹ */
#define PUMPF_04_PWM_IN_ERR_1                    (PUMPF_PWM_IN_NOEXIST) /* 80%: ����(ͣ��)��Ƿѹ/��ѹ���� */
#define PUMPF_04_PWM_IN_ERR_2                    (900)                  /* 90%: ����(ͣ��)����ת/��ת���� */
#define PUMPF_04_PWM_IN_ERR_3                    (850)                  /* 85%: ����(ͣ��)��ȱ��/�������� */
#define PUMPF_04_PWM_IN_OC                       (1000)	                /* ��PWM�ź�����[��] */

/* 05: �»� GPA25_11H */
/* ת�ٷ�Χ���ÿ�ˮ�ù����δע��ת�٣��ȼٶ�����Ӱ����� */
#define PUMPF_05_RPM_MIN                         (1000)                 /* rpm */
#define PUMPF_05_RPM_MAX                         (4500)                 /* rpm */
/* ������Χ */
#define PUMPF_05_FLOW_MIN                        (0)                    /* L/H */
#define PUMPF_05_FLOW_MAX                        (5500)                 /* L/H */
/* PWM���� */
#define PUMPF_05_PWM_OUT_RPM_MAX_BGN             (0)                    /* >0%: ���ת��������ʼ�� */
#define PUMPF_05_PWM_OUT_RPM_MAX_END             (100)                  /* <=10%: ���ת�����н����� */
#define PUMPF_05_PWM_OUT_RPM_MIN_BGN             (840)                  /* >84%: ���ת��������ʼ�� */
#define PUMPF_05_PWM_OUT_RPM_MIN_END             (900)                  /* <=90%: ���ת�����н����� */
#define PUMPF_05_PWM_OUT_IDLE                    (980)                  /* 95��100%: ����(ͣ��)��ֹͣ���� */
/* PWM���� */
#define PUMPF_05_PWM_IN_SC                       (PUMPF_PWM_IN_NOEXIST)	/* PWM�����ź�ȱʧ[��] */
#define PUMPF_05_PWM_IN_IDLE                     (950)                  /* 95%: ����(ͣ��)��׼������ */
#define PUMPF_05_PWM_IN_FLOW_INFO_MIN            (0)                    /* 0%: ������Ϣ������Ͱٷֱ� */
#define PUMPF_05_PWM_IN_FLOW_INFO_MAX            (700)                  /* 70%: ������Ϣ������߰ٷֱ� */
#define PUMPF_05_PWM_IN_WARN                     (750)                  /* 75%: ���棬�͵�ѹ/�ߵ�ѹ */
#define PUMPF_05_PWM_IN_ERR_1                    (PUMPF_PWM_IN_NOEXIST) /* ��PWM�ź�����[��] */
#define PUMPF_05_PWM_IN_ERR_2                    (900)                  /* 90%: ����(ͣ��)����ת/��ת���� */
#define PUMPF_05_PWM_IN_ERR_3                    (850)                  /* 85%: ����(ͣ��)���������� */
#define PUMPF_05_PWM_IN_OC                       (1000)	                /* ��PWM�ź�����[��] */

/* 06: ��ķ�� HBG-25-125-130P */
/* ת�ٷ�Χ */
#define PUMPF_06_RPM_MIN                         (1650)                 /* rpm */
#define PUMPF_06_RPM_MAX                         (4650)                 /* rpm */
/* ������Χ */
#define PUMPF_06_FLOW_MIN                        (0)                    /* L/H */
#define PUMPF_06_FLOW_MAX                        (5998)                 /* L/H */
/* PWM���� */
#define PUMPF_06_PWM_OUT_RPM_MAX_BGN             (0)                    /* >0%: ���ת��������ʼ�� */
#define PUMPF_06_PWM_OUT_RPM_MAX_END             (50)                   /* <=5%: ���ת�����н����� */
#define PUMPF_06_PWM_OUT_RPM_MIN_BGN             (850)                  /* >85%: ���ת��������ʼ�� */
#define PUMPF_06_PWM_OUT_RPM_MIN_END             (910)                  /* <=91%: ���ת�����н����� */
#define PUMPF_06_PWM_OUT_IDLE                    (980)                  /* 95��100%: ����(ͣ��)��ֹͣ���� */
/* PWM���� */
#define PUMPF_06_PWM_IN_SC                       (PUMPF_PWM_IN_NOEXIST)	/* PWM�����ź�ȱʧ[��] */
#define PUMPF_06_PWM_IN_IDLE                     (950)                  /* 95%: ����(ͣ��)��׼������ */
#define PUMPF_06_PWM_IN_FLOW_INFO_MIN            (0)                    /* 0%: ������Ϣ������Ͱٷֱ� */
#define PUMPF_06_PWM_IN_FLOW_INFO_MAX            (700)                  /* 70%: ������Ϣ������߰ٷֱ� */
#define PUMPF_06_PWM_IN_WARN                     (750)                  /* 75%: ���棬�͵�ѹ/�ߵ�ѹ */
#define PUMPF_06_PWM_IN_ERR_1                    (800)                  /* 80%: ����(ͣ��)����ת���� */
#define PUMPF_06_PWM_IN_ERR_2                    (850)                  /* 85%: ����(ͣ��)���������� */
#define PUMPF_06_PWM_IN_ERR_3                    (900)                  /* 90%: ����(ͣ��)������/ȱ��/���¹��� */
#define PUMPF_06_PWM_IN_OC                       (1000)	                /* ��PWM�ź�����[��] */

/* 07: ������ UPM4XLK 25-90 130 */
/* ת�ٷ�Χ */
#define PUMPF_07_RPM_MIN                         (564)                  /* rpm */
#define PUMPF_07_RPM_MAX                         (6300)                 /* rpm */
/* ������Χ */
#define PUMPF_07_FLOW_MIN                        (0)                    /* L/H */
#define PUMPF_07_FLOW_MAX                        (4000)                 /* L/H */
/* PWM���� */
#define PUMPF_07_PWM_OUT_RPM_MAX_BGN             (0)                    /* >0%: ���ת��������ʼ�� */
#define PUMPF_07_PWM_OUT_RPM_MAX_END             (100)                  /* <=10%: ���ת�����н����� */
#define PUMPF_07_PWM_OUT_RPM_MIN_BGN             (840)                  /* >84%: ���ת��������ʼ�� */
#define PUMPF_07_PWM_OUT_RPM_MIN_END             (910)                  /* <=91%: ���ת�����н����� */
#define PUMPF_07_PWM_OUT_IDLE                    (980)                  /* 95��100%: ����(ͣ��)��ֹͣ���� */
/* PWM���� */
#define PUMPF_07_PWM_IN_SC                       (PUMPF_PWM_IN_NOEXIST)	/* PWM�����ź�ȱʧ[��] */
#define PUMPF_07_PWM_IN_IDLE                     (950)                  /* 95%: ����(ͣ��)��׼������ */
#define PUMPF_07_PWM_IN_FLOW_INFO_MIN            (0)                    /* 0%: ������Ϣ������Ͱٷֱ� */
#define PUMPF_07_PWM_IN_FLOW_INFO_MAX            (700)                  /* 70%: ������Ϣ������߰ٷֱ� */
#define PUMPF_07_PWM_IN_WARN                     (800)                  /* 80%: ���棬ˮ�ø�ת���������� */
#define PUMPF_07_PWM_IN_ERR_1                    (PUMPF_PWM_IN_NOEXIST) /* ��PWM�ź�����[��] */
#define PUMPF_07_PWM_IN_ERR_2                    (850)                  /* 85%: ����(ͣ��)����������(<150V) */
#define PUMPF_07_PWM_IN_ERR_3                    (900)                  /* 90%: ����(ͣ��)��ˮ�ÿ��� */
#define PUMPF_07_PWM_IN_OC                       (1000)	                /* ��PWM�ź�����[��] */
#define PUMPF_07_PWM_IN_DIFF                     (10)		            /* PWM�ٷֱȻز�ֵ����: 1% */

/* ˮ��״̬ */
#define PUMPF_STA_SC                             (0)		/* ��· */
#define PUMPF_STA_IDLE                           (1)		/* ���� */
#define PUMPF_STA_RUN                            (2)		/* ���� */
#define PUMPF_STA_WARN                           (3)		/* ���� */
#define PUMPF_STA_ERR                            (4)		/* ���� */
#define PUMPF_STA_OC                             (5)		/* ��· */

PUMPF_EXT BOL fg_PUMPf_need;                /* ��Ƶˮ�������־(����) */
PUMPF_EXT BOL fg_PUMPf_long_time_no_run;	/* ��Ƶˮ�ó�ʱ��δ���б�־(����) */
PUMPF_EXT BOL fg_PUMPf_test_run;			/* ��Ƶˮ�������б�־(����) */
PUMPF_EXT BOL fg_PUMPf_debug_run;		    /* ��Ƶˮ�õ������б�־(����) */

PUMPF_EXT BOL fg_PUMPf_run;	                /* ��Ƶˮ�����б�־(����) */
PUMPF_EXT U08 PUMPf_sta;	                /* ��Ƶˮ��״̬ */
PUMPF_EXT RTIME PUMPf_time;	                /* ��Ƶˮ�ü�ʱ */
PUMPF_EXT U08 cnt_PUMPf_off_dly;			/* ��Ƶˮ�ùر���ʱ��ʱ */
PUMPF_EXT U08 cnt_PUMPf_ctrl;				/* ��Ƶˮ�õ������ڼ�ʱ */

enum {PUMF_NO_ERR, PUMPF_ERR_01, PUMPF_ERR_02, PUMPF_ERR_03, PUMPF_ERR_04, PUMPF_ERR_05, PUMPF_ERR_MAX};
PUMPF_EXT U08 PUMPf_err_id;                /* ��Ƶˮ�ù������� */

PUMPF_EXT I16 PUMPf_pwm_out;/* PWM���ֵ(PWMռ�ձ����ֵ:�ٷֱ�) */
PUMPF_EXT I16 PUMPf_pwm_in;	/* PWM����ֵ(PWMռ�ձȷ���ֵ:�ٷֱ�) */

PUMPF_EXT I16 PUMPf_rpm_sv;	/* ˮ��Ŀ��ת��:rpm */
PUMPF_EXT I16 PUMPf_flow_pv;/* ˮ�õ�ǰ����:L/H */
PUMPF_EXT I16 PUMPf_power_pv;/* ˮ�õ�ǰ����:W */

PUMPF_EXT I16 PUMPf_rpm_max;	/* ˮ�����ת��:rpm */

PUMPF_EXT void PUMPF_pwm_out_deal(void);
PUMPF_EXT void PUMPF_speed_ctrl(BOL fg_on);
PUMPF_EXT void PUMPF_status(void);
PUMPF_EXT BOL PUMPF_is_warn(void);
PUMPF_EXT BOL PUMPF_is_err(void);
PUMPF_EXT BOL PUMPF_can_reset_err(void);
PUMPF_EXT void PUMPF_init(void);

#endif

