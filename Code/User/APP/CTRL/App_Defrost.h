/******************************************************************************

          ��Ȩ���� (C), 2001-2016, �����а��յ��Լ����������޹�˾

 ******************************************************************************
  �� �� ��   : defrost.h
  �� �� ��   : ����
  ��    ��   : ��־��
  ��������   : 2018��12��28��������
  ����޸�   :
  ��������   : defrost.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��12��28��������
    ��    ��   : ��־��
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __DEFROST_H__
#define __DEFROST_H__

#ifdef GBL_DEFROST
    #define DEFROST_EXT
#else
    #define DEFROST_EXT extern
#endif    

/*-----------------����ͷ�ļ� -----------------------------*/

//1 �����������������������������,Ӧ���г���һ��!!!!!
// �粻һ��,��˵�����ļ���һ��.


/*-----------------�ⲿ����˵��----------------------------*/



/*-----------------�ⲿ����ԭ��˵��------------------------*/



/*-----------------�ڲ�����ԭ��˵��------------------------*/



/*-----------------ȫ�ֱ���--------------------------------*/



/*-----------------ģ�鼶����------------------------------*/



/*-----------------��������--------------------------------*/

/* ��˪�˳���ʽ */
#define DEFRO_EXIT_ALL			0			/* ��ͬ�˳� */
#define DEFRO_EXIT_SIN			1			/* �����˳�,��ͣ�� */

/* ��˪��ʽ */
#define DEFRO_BY_FROSTTIME      0           /* ����˪ʱ�� */
#define DEFRO_BY_RUNTIME        1           /* ��ѹ������ʱ�� */

/* defrost_get_valueȡֵ��� */
#define NUM_EXIT_DEFR_CM_OFF      8           /* �˳�˪ʱѹ��ͣ����־����� */


/*-----------------�궨�� ---------------------------------*/


//4 ��˪״̬
enum {
    DEFROST_STA_NULL = 0,       // ����������
    DEFROST_STA_NO_FROST,       // �޻�˪
    DEFROST_STA_BY_HUMIDITY,    // ʪ�ȳ�˪
    DEFROST_STA_BY_ENV_WING,    // ���������˪
    DEFROST_STA_BY_WING,        // ���³�˪
    DEFROST_STA_BY_LOWPRESS,    // ��ѹ��˪
    DEFROST_STA_BY_EMERGENCY,   // ������˪
    DEFROST_STA_BY_MANUAL,      // �ֶ���˪
    DEFROST_STA_BY_LONGTIME,	// ��ʱ��˪
    DEFROST_STA_BY_GAS_IN,      // �����¶ȳ�˪
    DEFROST_STA_BY_POWER,       // ����˥����˪
	DEFROST_STA_MAX
};
#define DEFROST_STA_IS_NEED(sta)    (((sta) == DEFROST_STA_BY_HUMIDITY)\
                                    ||((sta) == DEFROST_STA_BY_ENV_WING)\
                                    ||((sta) == DEFROST_STA_BY_WING)\
                                    ||((sta) == DEFROST_STA_BY_LOWPRESS)\
                                    ||((sta) == DEFROST_STA_BY_EMERGENCY)\
                                    ||((sta) == DEFROST_STA_BY_MANUAL)\
                                    ||((sta) == DEFROST_STA_BY_LONGTIME)\
                                    ||((sta) == DEFROST_STA_BY_GAS_IN)\
                                    ||((sta) == DEFROST_STA_BY_POWER)\
                                    )

//4 ��˪����
enum {
    DEFROST_STEP_NULL = 0,      // ��ʼ̬
    DEFROST_STEP_FROSTING,      // ��˪��
    DEFROST_STEP_DEFROST,       // ��˪��
    DEFROST_STEP_OVER           // ��˪����
};


//4 ��˪ͣѹ������
enum {
    DEFROST_CM_NULL        =0, // ��
    DEFROST_CM_STOP_BGN    =1, // ѹ��ͣ��,��ͨ��
    DEFROST_CM_OFF_FORE_OFF=2, // ��ǰ����ͨ
    DEFROST_CM_ON_FORE_ON  =3, // �Ӻ����ͨ
    DEFROST_CM_ON          =4, // ѹ������
    DEFROST_CM_STOP_END    =5, // ѹ��ͣ��,��ͨ��
    DEFROST_CM_OFF_FORE_ON =6, // ��ǰ����ͨ
    DEFROST_CM_ON_FORE_OFF =7, // �Ӻ���ͨ    
    DEFROST_CM_OVER        =8, // ������˪
    DEFROST_CM_STOP        =9  // ת��ͣ��
};

typedef struct tagDEFROST_FG
{
    BOL emergency_defrost_frequent : 1; // ������˪Ƶ��
    BOL defrost_end                : 1; // ����˪
    BOL first_defrost              : 1; // ���״γ�˪
    BOL defrost_apply              : 1; // �����˪
    BOL bak4            		   : 1; // ����
    BOL need_calib_EWD             : 1; // ��궨�����
    BOL need_calib_stop_EWD        : 1; // ��궨ͣ�������    
    BOL exit_defr_cm_off           : 1; // �˳�˪ʱѹ��ͣ����־
}DEFROST_FG;

DEFROST_EXT U08 fg_defrost_apply[MAX_COMP];			// �����˪��־ 
DEFROST_EXT DEFROST_FG defrost_fg[MAX_COMP];
DEFROST_EXT SIGNALSTA defrost_signal_into_lowpress[MAX_COMP];
DEFROST_EXT SIGNALSTA defrost_signal_into_namual[MAX_COMP];
DEFROST_EXT SIGNALSTA defrost_signal_exit_highpress[MAX_COMP];
DEFROST_EXT SIGNALSTA defrost_signal_exit_exh[MAX_COMP];
DEFROST_EXT SIGNALSTA defrost_signal_exit_cur[MAX_COMP];

DEFROST_EXT U08 defrost_cm_step[MAX_COMP];          // ��˪ͣѹ���׶�
DEFROST_EXT U16 defrost_cm_time[MAX_COMP];          // ��˪ͣѹ����ʱ
DEFROST_EXT U08 defrost_cm_intv_on;                 // ��˪ͣѹ���������
DEFROST_EXT U08 defrost_cm_intv_off;                // ��˪ͣѹ��ͣ�����

DEFROST_EXT void defrost_init(void);
DEFROST_EXT void defrost(void);

DEFROST_EXT I16 defrost_EWD_except_diff(U08 cm);
DEFROST_EXT void defrost_EWD_except_reset(U08 cm);

DEFROST_EXT I16 defrost_get_value(U08 cm, U08 val_type);

DEFROST_EXT I32 defrost_cal_power(I32 water_diff);

#endif /* __DEFROST_H__ */

