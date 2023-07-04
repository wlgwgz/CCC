/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: eev.h
 �ļ�����: �������ͷ�״̬����
           ʹ��ʱֻ���ע��h�ļ��������עc�ļ���
 ������  : hzy
 ��������: 2017.02.08
*******************************************************************************/

#ifndef EEV_H
    #define EEV_H

#ifdef  EEV_GLOBALS
    #define EEV_EXT
#else
    #define EEV_EXT extern
#endif





/************************************** ʹ��˵�� *****************************************

1.ǰ�ù���
    1.1 ����ģ���ʼ��ʱ�����ñ�ģ���ʼ������ eev_init()(�ڶ�ȡ�����󡢵������ǰ����)�� 
    1.2 ����ģ�鴦��100ms��ʱʱ�����ñ�ģ���ʱ���� eev_timer()��
    1.3 ��ʹ��PID���ڣ�����eev_set_pid_para()�������ʵ���PID������
    1.4 ��������Ƶ�ʡ����ŷ�ʽ����
        1����eev_init()����eev������Ƶ�ʡ����ŷ�ʽ��ʼ������ر��������б仯ʱ����ͬeev�ֿ���ͬ����ʱ���޸ĳ�ʼ����
        2����para.cģ���para_end()��ʹ��eev_set_motor_type()��eev_set_step_speed()��������Ƶ�ʡ����ŷ�ʽ���޸�
    
2.��������
    2.1 ͨ��eev_set_xxx()�������ͷ�������ز�����
    2.2 ����PID�㷨ʱ������eev_task_pid()�����÷ֶο���ʱ������eev_task_area()��


3.��ģ����X1.HL017A.K05.005-1.V100A07�е�PID.c���Ķ�������ֳ�eev.c��ipd.c����Ϊ
  ��Ҫ��ԭ���ƽ������һ�£����Դ��벢����ȫ�淶���������滻����΢���PID.c������
  ������ԱȲ��ʵ��޸ġ�
******************************************************************************************/






//************************************** ���� *****************************************

//�������
typedef enum
{
    EEV_ID_1,       //1#���ͷ���·
    EEV_ID_2,       //2#���ͷ���·
    EEV_ID_MAX      //���ͷ�����
}EEV_ID_ENUM;

//����״̬�л�����
typedef enum
{
    EEV_CMD_STOP,           //�ر�
    EEV_CMD_RESET,          //��λ
    EEV_CMD_DEFRO,          //��˪
    EEV_CMD_PREV,           //��ʼ����
    EEV_CMD_RUN,            //��������
    EEV_CMD_LIMIT_DOWN,     //���ƹ�С
    EEV_CMD_LIMIT_UP,       //���ƿ���
    EEV_CMD_FORCE_DOWN,     //ǿ�ƹ�С
    EEV_CMD_FORCE_UP,       //ǿ�ƿ���
    EEV_CMD_FORCE_FASTUP,   //����ǿ�ƿ���
    EEV_CMD_CONST,          //������
    EEV_CMD_MAX
}EEV_CMD_ENUM;


typedef enum //�������ͷ�����״̬
{
    EEV_STATE_RETURN_ZERO,  //����
    EEV_STATE_WAIT_ZERO,    //�ȴ�����
    EEV_STATE_WAIT_CMD,     //�ȴ�����
    EEV_STATE_IDLE,         //����
    EEV_STATE_DEFRO,        //��˪
    EEV_STATE_PRE_OPEN,     //Ԥ��
    EEV_STATE_PRE_KEEP,     //Ԥ��ά��
    EEV_STATE_RUNNING,      //����
    EEV_STATE_MAX           //���״̬��
}EEV_STATE_ENUM;

/*******************************************************************************
 �� ��: eev_timer()
 �� ��: ģ���ʱ����
 �� ��: BOOL TimeFlag ʱ���־���ò������ݣ�������ֲʱ100ms��־���Ʋ�ͬ��
 �� ��: ��
 �� ��: ��100ms��ʱ����������
*******************************************************************************/
void eev_timer(BOOL TimeFlag);

/*******************************************************************************
 �� ��: eev_init()
 �� ��: ����״̬��ʼ��
 �� ��: ��
 �� ��: ��
 �� ��: ��Ӧ�ó����ʼ���е���
*******************************************************************************/
void eev_init(void);

/*******************************************************************************
 �� ��: eev_task_pid()
 �� ��: ���ȶȿ���
 �� ��: id--�������; 
        cmd--ָ��; 
        pv--��ǰ����; 
        sv--Ŀ�경��; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
//void eev_task_pid(EEV_ID_ENUM id, EEV_CMD_ENUM cmd, I16 pv, I16 sv);
void eev_task_pid(PID_ID_ENUM p_id, EEV_ID_ENUM id, EEV_CMD_ENUM cmd, I16 pv, I16 sv);


/*******************************************************************************
 �� ��: eev_task_area()
 �� ��: �ֶο���
 �� ��: id--�������; 
        cmd--ָ��; 
        setting_steps--ָ������; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void eev_task_area(EEV_ID_ENUM id, EEV_CMD_ENUM cmd, U16 setting_steps);

/*******************************************************************************
 �� ��: eev_set_steps_idle()
 �� ��: ���ô�������
 �� ��: id--�������; 
        value--����ֵ; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL eev_set_steps_idle(EEV_ID_ENUM id, U16 value);

/*******************************************************************************
 �� ��: eev_set_steps_preopen()
 �� ��: ����Ԥ������
 �� ��: id--�������; 
        value--����ֵ; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL eev_set_steps_preopen(EEV_ID_ENUM id, U16 value);

/*******************************************************************************
 �� ��: eev_set_steps_defrost()
 �� ��: ���ó�˪����
 �� ��: id--�������; 
        value--����ֵ; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL eev_set_steps_defrost(EEV_ID_ENUM id, U16 value);

/*******************************************************************************
 �� ��: eev_set_steps_max()
 �� ��: �����ܲ���
 �� ��: id--�������; 
        value--����ֵ; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL eev_set_steps_max(EEV_ID_ENUM id, U16 value);

/*******************************************************************************
 �� ��: eev_set_steps_run_max()
 �� ��: ����������в���
 �� ��: id--�������; 
        value--����ֵ; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL eev_set_steps_run_max(EEV_ID_ENUM id, U16 value);

/*******************************************************************************
 �� ��: eev_set_steps_min()
 �� ��: ������С����
 �� ��: id--�������; 
        value--����ֵ; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL eev_set_steps_min(EEV_ID_ENUM id, U16 value);

/*******************************************************************************
 �� ��: eev_set_step_speed()
 �� ��: ��������Ƶ�����
 �� ��: id--�������; 
        value--����ֵ; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL eev_set_step_speed(EEV_ID_ENUM id, U16 value);


/*******************************************************************************
 �� ��: eev_set_motor_type()
 �� ��: �������ŷ�ʽ
 �� ��: id--�������; 
        value--����ֵ; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL eev_set_motor_type(EEV_ID_ENUM id, U16 value);


/*******************************************************************************
 �� ��: eev_degree_to_steps()
 �� ��: ����ת��Ϊ����
 �� ��: id--�������; 
        DegreeValue--����ֵ; 
 �� ��: ����
 �� ��: ��
*******************************************************************************/
U16 eev_degree_to_steps(EEV_ID_ENUM id, U16 DegreeValue);

/*******************************************************************************
 �� ��: eev_get_step_speed()
 �� ��: ��ȡ��������Ƶ�����
 �� ��: id--�������; 

 �� ��: ����Ƶ�����
 �� ��: ��
*******************************************************************************/
U16 eev_get_step_speed(EEV_ID_ENUM id);

/*******************************************************************************
 �� ��: eev_get_motor_type()
 �� ��: ��ȡ�������ŷ�ʽ
 �� ��: id--�������; 

 �� ��: ���ŷ�ʽ
 �� ��: ��
*******************************************************************************/
U16 eev_get_motor_type(EEV_ID_ENUM id);


/*******************************************************************************
 �� ��: eev_set_period_preopen()
 �� ��: ����Ԥ��ʱ��
 �� ��: id--�������; 
        value--����ֵ; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL eev_set_period_preopen(EEV_ID_ENUM id, U16 value);

/*******************************************************************************
 �� ��: eev_set_period_calculate()
 �� ��: ���ü�������
 �� ��: id--�������; 
        value--����ֵ; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL eev_set_period_calculate(EEV_ID_ENUM id, U16 value);

/*******************************************************************************
 �� ��: eev_set_period_action()
 �� ��: ���ö�������
 �� ��: id--�������; 
        value--����ֵ; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL eev_set_period_action(EEV_ID_ENUM id, U16 value);
BOOL eev_set_period_add(EEV_ID_ENUM id, U16 value);
BOOL eev_set_steps_add(EEV_ID_ENUM id, U16 value);
BOOL eev_set_period_hold(EEV_ID_ENUM id, U16 value);
BOOL eev_set_period_cyc(EEV_ID_ENUM id, U16 value);
BOOL eev_set_period_sub(EEV_ID_ENUM id, U16 value);
BOOL eev_set_steps_sub(EEV_ID_ENUM id, U16 value);


U16 eev_adj_byExhaustTemp(EEV_ID_ENUM id, U08 x, U16 start_value);
BOOL eev_isReady(EEV_ID_ENUM id);
U16 eev_get_step_fre(EEV_ID_ENUM id);
BOOL eev_holdjudge(BOOL curt_state,EEV_ID_ENUM id);

I16 eev_get_steps_run_max(EEV_ID_ENUM id);
I16 eev_get_steps_min(EEV_ID_ENUM id);
void eev_io_step_read(EEV_ID_ENUM id);
EEV_EXT U16 eev_get_init_step_pv(EEV_ID_ENUM id);

#endif 