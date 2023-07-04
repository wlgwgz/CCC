/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: eev.c
 �ļ�����: �������ͷ�״̬����
 ������  : hzy
 ��������: 2017.02.13
*******************************************************************************/

#define EEV_GLOBALS
#include "includes.h"

#define EEV_NORMAL_CTRL         0       // ��������
#define EEV_FORCED_INCREASE     1       // ǿ�����󷧿���
#define EEV_FORCED_DECREASE     2       // ǿ�Ƽ�С������
#define EEV_FORCED_KEEP         3       // ���ַ����ȣ�ǿ�Ƶ��ڹ����У�



#define EEV_RAPID_TIME_1    150     /* ���ٵ���ǰ��ʱ1 */
#define EEV_RAPID_TIME_2    240     /* ���ٵ���ǰ��ʱ2 */
#define EEV_RAPID_AREA      25      /* ���ٵ������䵥λ�¶� */




typedef struct
{
    //�Ƕ�д��
    EEV_STATE_ENUM state;   //����״̬
    U16 timer_preopen;      //Ԥ����ʱ(100ms)
    U16 timer_calculate;    //Ԥ����ʱ(100ms)
    U16 timer_action;       //������ʱ(100ms)
    U16 timer_add;       //ǿ�ƿ�������ʱ(100ms)
    U16 timer_sub;       //ǿ�ƿ�С������ʱ(100ms)
    U16 timer_hold;       //ά�ֵ�ǰ���ȶ���ʱ���ʱ(1��)
    U16 timer_fastadd;       //����ǿ�ƿ�������ʱ(100ms)
    U16 timer_initchange;       //��ʼ������תǰ��ʱ(100ms)

    U16 cnt_positive;       //�������
    U16 cnt_negative;       //�������
    U16 step_sv;            //Ŀ�경��
    U16 step_pv;            //��ǰ����
    I16 pid_sv;             //���Ƶ�Ŀ��ֵ
    I16 pid_pv;             //���Ƶ㵱ǰֵ���ô˱���ʱ�Ѿ���PID��С���е�λ
    //ֻд��
    U16 steps_idle;         //��������
    U16 steps_preopen;      //Ԥ������
    U16 steps_defrost;      //��˪����
    U16 steps_max;          //�ܲ���
    U16 steps_run_max;      //������в���
    U16 steps_run_min;      //��С����
    U16 steps_add;          //
    U16 steps_sub;          //
    U16 period_preopen;     //Ԥ��ʱ��(100ms)
    U16 period_calculate;   //��������(100ms)
    U16 period_action;      //��������(100ms)  
    U16 period_add;      //ǿ�ƿ���������
    U16 period_sub;      //ǿ�ƿ�С��������

    
    U16 period_hold;      //ά�ֵ�ǰ���ȶ���ʱ��(��λ��)
    U16 period_cyc;      //ά�ֵ�ǰ���ȶ�������(��λ��)

//    //ֻ����
    U16 motor_fre;          //����Ƶ��(���)
    U16 motor_type;         //���ŷ�ʽ
}EEV_STEPS_STRUCT;

EEV_STEPS_STRUCT eev[EEV_ID_MAX];

typedef struct
{
    U16 adj_min;        // ��������
    U16 adj_max;        // ��������
    U16 adj_step;       // ���ڲ���
}ADJUST_STRUCT;

//ADJUST_STRUCT eev_adj_gas[EEV_ID_MAX];  // ��������ǿ�Ƶ��� 


//ģ���ʱ
//100ms��ʱ
enum
{
    EEV_TIMER_100MS_BAK,   //����
    EEV_TIMER_100MS_MAX
};
static U16 eev_timer_100ms[EEV_TIMER_100MS_MAX];
//1s��ʱ
enum
{
    EEV_TIMER_1S_BAK,   //����
    EEV_TIMER_1S_MAX
};
static U16 eev_timer_1s[EEV_TIMER_1S_MAX];


/*==========================================
    ����: ����a1 = (a1+a2) % b  
          �������� = (a1+a2) / b
    �����ϴ��������г�������
/==========================================*/
I32 eev_math_divide(I32 *a1, I32 a2, I32 b)
{
    I32 a;

    if (b == 0) b = 1;
    
    a = (*a1) + a2;
    *a1 = a % b;    //����
    return (a / b); //����
}

///*==========================================
//  ����: ���ٴ���
///==========================================*/
void eev_speedup(EEV_ID_ENUM id, I16 diff)
{
    if (diff > EEV_RAPID_AREA)
    {
        eev[id].cnt_positive++;
        eev[id].cnt_negative = 0;
    }
    else if (diff < -EEV_RAPID_AREA)
    {
        eev[id].cnt_negative++;
        eev[id].cnt_positive = 0;
    }
    else 
    {
        eev[id].cnt_positive = 0;
        eev[id].cnt_negative = 0;
    }

    if(EEV_RAPID_AREA==0) //Ϊ0�������ʱ
    {
        eev[id].cnt_positive = 0;
        eev[id].cnt_negative = 0;
    }
    
}

/*******************************************************************************
 �� ��: eev_increment_adjust() ��ԭ��pid_valve_out��
 �� ��: �仯������
 �� ��: id--�������; 
        increment--����; 
 �� ��: �����������
 �� ��: ���ݵ�ǰ��������������ֵ���統ǰ������С����ÿ�α仯���ʵ���С����ǰ��
        ���ϴ���ÿ�α仯���ʵ�����
*******************************************************************************/
I16 eev_increment_adjust(PID_ID_ENUM p_id, EEV_ID_ENUM id, I16 increment)
{
    static I32 remainder[EEV_ID_MAX] = {0,0};
    I16 dest_val = 0;
    U16 calcu_pv = 1;           //�������ĵ�ǰֵ
    I32 denominator = 1;        //��ĸ
    U16 steps_ref_min = 0;      //��С�ο����������ڷ�ֹ��ǰ���ȹ���ʱ���������� 
    I16 pid_kc;
        
    steps_ref_min = eev[id].steps_max / 4;
    if (steps_ref_min > eev[id].steps_run_max)     
    {
        steps_ref_min = eev[id].steps_run_max;
    }
        
    calcu_pv = GET_EXIO_ST_PV(0, id);
    if (calcu_pv < (eev[id].steps_max / 25))    // ʲôԭ��zzl 2018.04.08 
    {
        calcu_pv = eev[id].steps_run_max;    
    }
    else if (calcu_pv < steps_ref_min)          // ʲôԭ��zzl 2018.04.08
    {
        calcu_pv = steps_ref_min;
    }    
            
    pid_kc = pid_get_Kc(p_id);
    denominator = pid_kc * 2;      
    
    if (remainder[id] > denominator || remainder[id] < -denominator) //zzl 2018.04.18ΪʲôҪ�����?��ʵ���ݴ������ϴ˴��������
    {
        remainder[id] = 0;
    }
    dest_val = eev_math_divide(&remainder[id], (I32)increment * calcu_pv * 3, denominator);    /* ����Ĳ��� */

    return dest_val;
}

/*******************************************************************************
 �� ��: eev_io_step_read()
 �� ��: ��ȡʵ��IO����
 �� ��: no--�������; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void eev_io_step_read(EEV_ID_ENUM id)
{
    if (GET_EXIO_HDW_STATUS_READY(0))
    {
        eev[id].step_sv = GET_EXIO_ST_SV(0, id);//��ȡĿ�경��
        DISABLE_IRQ();
        eev[id].step_pv = GET_EXIO_ST_PV(0, id);//��ȡ��ǰ����
        ENABLE_IRQ();
    }
}

/*******************************************************************************
 �� ��: eev_io_step_write_sv()
 �� ��: ����IOĿ�경��
 �� ��: no--�������; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void eev_io_step_write_sv(EEV_ID_ENUM id, EEV_CMD_ENUM cmd)
{               
	PID_ID_ENUM p_id = (PID_ID_ENUM)id;
    cmd = cmd;				/* N0001 */
    if (eev[id].state == EEV_STATE_RUNNING)
    {
        if (eev[id].step_sv < eev[id].steps_run_min) eev[id].step_sv = eev[id].steps_run_min;
        if (eev[id].step_sv > eev[id].steps_run_max) eev[id].step_sv = eev[id].steps_run_max;
    }
    else
    {
        if (eev[id].step_sv > eev[id].steps_max)    eev[id].step_sv = eev[id].steps_max;
    }

    if(id==PID_ID_EEV_1) //��·
    {
        if ((val_motor_manual_mode == USED) && (cmd>=EEV_CMD_RUN || !log_mod_cell_is_run(MODE_NULL)))
        {
            SET_EXIO_ST_SV(0, id, val_motor_manual_steps);
            pid_set_AveAcrossCnt(p_id,0);//û�п���ʱ���¼���
        }
        else if (TF_OPEN_COMP && eev_isReady(id))
        {
            SET_EXIO_ST_SV(0, id, TF_eev_step);
            pid_set_AveAcrossCnt(p_id,0);//û�п���ʱ���¼���
            eev[id].step_sv = TF_eev_step;
		}
#if 0
		else if (USE_ABILITY_EEV_STEP && (cmd>=EEV_CMD_RUN || !log_mod_cell_is_run(MODE_NULL)))
		{
            SET_EXIO_ST_SV(0, id, val_ability_test_motor_steps);
            pid_set_AveAcrossCnt(p_id,0);//û�п���ʱ���¼���
            eev[id].step_sv = val_ability_test_motor_steps;
		}
#endif
        else
        {
            SET_EXIO_ST_SV(0, id, eev[id].step_sv);//����Ŀ�경��
        }
    }
    if(id==PID_ID_EEV_2) //��·
    {
        if((val_s_motor_manual_mode == USED) && (cmd>=EEV_CMD_RUN || !log_mod_cell_is_run(MODE_NULL)))
        {
            SET_EXIO_ST_SV(0, id, val_s_motor_manual_steps);
            pid_set_AveAcrossCnt(p_id,0);//û�п���ʱ���¼���
        }
		else if (fg_ability_test_run && (val_ability_test_evi_steps > 0) && (cmd>=EEV_CMD_RUN || !log_mod_cell_is_run(MODE_NULL)))
		{
            SET_EXIO_ST_SV(0, id, val_ability_test_evi_steps);
            pid_set_AveAcrossCnt(p_id,0);//û�п���ʱ���¼���
		}
        else
        {
            SET_EXIO_ST_SV(0, id, eev[id].step_sv);//����Ŀ�경��
        }
    }
    
}

/*******************************************************************************
 �� ��: eev_io_step_write_pv()
 �� ��: ��дIO��ǰ����
 �� ��: no--�������; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void eev_io_step_write_pv(EEV_ID_ENUM id)
{
//   EEV_STEP_PV pIO->io_step_pv[id] = eev[id].step_pv;//���µ�ǰ����
    /* �������ɴ���Ӳ����ʼ������ֹ�����󴥷� */
    if ((id == EEV_ID_1) && GET_EXIO_HDW_STATUS_READY(0))
    {
        SET_EXIO_INIT_HDW(0);	/* ����ִ�г�ʼ�� */
    }
}

/*******************************************************************************
 �� ��: eev_calibrate()
 �� ��: ����У׼
 �� ��: no--�������; 
 �� ��: ��
 �� ��: �������120%�ر�
*******************************************************************************/
void eev_calibrate(EEV_ID_ENUM id)
{
    eev[id].step_sv = 0;
    /* ���δ˴�*1.2��ԭ��: ��ǰ�����ᱻ�������ʵ���ݸ�д */
    eev[id].step_pv = eev_get_init_step_pv(id);

    eev_io_step_write_sv(id,EEV_CMD_RESET);//����Ŀ�경��
    eev_io_step_write_pv(id);//���µ�ǰ����
}

/*******************************************************************************
 �� ��: eev_timer()
 �� ��: ģ���ʱ����
 �� ��: BOOL TimeFlag ʱ���־���ò������ݣ�������ֲʱ100ms��־���Ʋ�ͬ��
 �� ��: ��
 �� ��: ��100ms��ʱ��������
*******************************************************************************/
void eev_timer(BOOL TimeFlag)
{
    U08 i;
    static U08 cnt_100ms_to_1s = 0; //100���뻻���1��

    if(TimeFlag)
    {
        for (i=0; i<EEV_TIMER_100MS_MAX; i++)
        {
            if (eev_timer_100ms[i] != 0xffff) eev_timer_100ms[i]++;
        }
        for (i=0; i<EEV_ID_MAX; i++)
        {
            if (eev[i].timer_preopen != 0xffff)     eev[i].timer_preopen++;//Ԥ����ʱ(100ms)
            if (eev[i].timer_calculate != 0xffff)   eev[i].timer_calculate++;//Ԥ����ʱ(100ms)
            if (eev[i].timer_action != 0xffff)      eev[i].timer_action++;//������ʱ(100ms)
            if (eev[i].timer_add != 0xffff)         eev[i].timer_add++;//������ʱ(100ms)
            if (eev[i].timer_fastadd != 0xffff)     eev[i].timer_fastadd++;//������ʱ(100ms)
		    if (eev[i].timer_sub!= 0xffff)          eev[i].timer_sub++;//������ʱ(100ms)
            if (eev[i].timer_initchange!= 0xffff)   eev[i].timer_initchange++;//������ʱ(100ms)

        }
        
        if (++cnt_100ms_to_1s >= 10)
        {
            cnt_100ms_to_1s = 0;
            for (i=0; i<EEV_TIMER_1S_MAX; i++)
            {
                if (eev_timer_1s[i] != 0xffff) eev_timer_1s[i]++;
            }
            for (i=0; i<EEV_ID_MAX; i++)
            {
                if (eev[i].timer_hold != 0xffff)     eev[i].timer_hold++;//ά�ֵ�ǰ���ȼ�ʱ(1��)
            }
        }
    }
}

/*******************************************************************************
 �� ��: eev_init()
 �� ��: ����״̬��ʼ��
 �� ��: ��
 �� ��: ��
 �� ��: ��Ӧ�ó����ʼ���е���
*******************************************************************************/
void eev_init(void)
{
    U08 i;

    
    for (i=0; i<EEV_ID_MAX; i++)
    {
        eev[i].state = EEV_STATE_RETURN_ZERO;

        if(i==EEV_ID_1)
        {
            eev_set_motor_type(EEV_ID_1, val_motor_fre_type);
            eev_set_step_speed(EEV_ID_1, val_motor_step_fre);
        }
        else if(i==EEV_ID_2)
        {
            eev_set_motor_type(EEV_ID_2, val_s_motor_fre_type);
            eev_set_step_speed(EEV_ID_2, val_s_motor_step_fre);
        }
    }
}

/*******************************************************************************
 �� ��: eev_set_pid_para()
 �� ��: ���õ����PID����
 �� ��: PID_ID_ENUM p_id: PID���, EEV_ID_ENUM e_id: �������
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void eev_set_pid_para(PID_ID_ENUM p_id, EEV_ID_ENUM e_id)
{
//    pid_set_ctrl(p_id, PID_CTRL_INCREMENT);//����ʽ
    pid_set_sv(p_id, &eev[e_id].pid_sv);//Ŀ��ֵ
    pid_set_pv(p_id, &eev[e_id].pid_pv);//��ǰֵ
    if(e_id==PID_ID_EEV_1)
    {
        DebugPID[DEBUG_PID_SV] = eev[e_id].pid_sv;
        DebugPID[DEBUG_PID_PV] = eev[e_id].pid_pv;
    }
    else
    {
        DebugEEV_ID_2PID[DEBUG_PID_SV] = eev[e_id].pid_sv;
        DebugEEV_ID_2PID[DEBUG_PID_PV] = eev[e_id].pid_pv;
    }

//    pid_set_Ts(p_id, val_m_pid_calculate_period);//��������
//    pid_set_Kp(p_id, 2);//����ϵ��
////    pid_set_Ti(p_id, 10*val_m_pid_i);//����ʱ��wlg v100a01
//    pid_set_Ti(p_id, val_m_pid_i);//����ʱ��
//    pid_set_Td(p_id, val_m_pid_d);//΢��ʱ��
//    pid_set_Kc(p_id, val_m_pid_p);//�����ñ�����

    
//    pid_set_ctrl(PID_ID_EEV_ECNM, PID_CTRL_INCREMENT);//����ʽ
//    pid_set_sv(PID_ID_EEV_ECNM, &eev[EEV_ID_ECNMC].pid_sv);//Ŀ��ֵ
//    pid_set_pv(PID_ID_EEV_ECNM, &eev[EEV_ID_ECNMC].pid_pv);//��ǰֵ
//    pid_set_Ts(PID_ID_EEV_ECNM, val_pid_calculate_period_1);//��������
//    pid_set_Kp(PID_ID_EEV_ECNM, 2);//����ϵ��
////    pid_set_Ti(PID_ID_EEV_ECNM, 10*val_pid_i_1);//����ʱ��
//    pid_set_Ti(PID_ID_EEV_MASTER,val_pid_i);//����ʱ��
//    pid_set_Td(PID_ID_EEV_ECNM, val_pid_d_1);//΢��ʱ��
//    pid_set_Kc(PID_ID_EEV_ECNM, val_pid_p_1);//�����ñ�����
}

/*******************************************************************************
 �� ��: eev_task_pid()
 �� ��: PID����
 �� ��: p_id--PID���; 
        id--�������; 
        cmd--ָ��; 
        pv--��ǰ����; 
        sv--Ŀ�경��;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
//#define val_control_limit			    (40)				// ���ٽӽ��²�
//#define val_limit_step_num			    (5 )				// ���ٽӽ�Ƶ��
//#define val_control_limit			    (val_pid_divisiontemp)				// ���ٽӽ��²�
//#define val_limit_step_num			    (val_pid_limitstepnum)				// ���ٽӽ�Ƶ��

void eev_task_pid(PID_ID_ENUM p_id, EEV_ID_ENUM id, EEV_CMD_ENUM cmd, I16 pv, I16 sv)
{
    I16 val = 0;
    static I16 step_quick[EEV_ID_MAX];
    static I16 step_pid[EEV_ID_MAX];
    U16 time_pos = 0;
    U16 time_neg = 0;
//    I16 val_a,val_b,val_c;

    eev[id].pid_pv = pv;
    eev[id].pid_sv = sv;
	
    //��ȡʵ�ʲ���
    eev_io_step_read(id);
    switch (eev[id].state)
    {
        /* ��������� */
        case EEV_STATE_RETURN_ZERO: 
            eev_calibrate(id);
            eev[id].state = EEV_STATE_WAIT_ZERO;
            break;
            
        /* �ȴ����������� */  
        case EEV_STATE_WAIT_ZERO:  /* �ȴ����������� */
            if (eev[id].step_sv == eev[id].step_pv) 
            {
                eev[id].state = EEV_STATE_WAIT_CMD;
            }
            break;

        case EEV_STATE_WAIT_CMD:  /* ׼���ã��ȴ����� */
            if (cmd == EEV_CMD_RESET)/* �������������ת�� �������� �ȴ� */
            {
                eev[id].state = EEV_STATE_IDLE;     
            }
            else if (cmd != EEV_CMD_STOP)/* �����������ת����ʼ���� */
            {
                eev[id].state = EEV_STATE_PRE_OPEN;  
            }
            else if (cmd == EEV_CMD_STOP)
            {
                /* �������������ֶ������ٹر�ʱ��������Ȼͣ�����ֶ����������� */
                eev[id].step_sv = 0;
            }
            break;
                    
        case EEV_STATE_IDLE:    // ��������
            eev[id].step_sv = eev[id].steps_idle;
            if (eev[id].step_sv == eev[id].step_pv) /* �ȴ����Ķ���ִ�� */
            {  
                if (cmd >= EEV_CMD_DEFRO)      eev[id].state = EEV_STATE_PRE_OPEN;    
                else if (cmd == EEV_CMD_STOP)  eev[id].state = EEV_STATE_RETURN_ZERO;/* ���� */
            }
            break;
        
        case EEV_STATE_DEFRO:       // ��˪����
        
        case EEV_STATE_PRE_OPEN:    // ��ʼ���ȣ�Ԥ���ȣ�
            if (cmd == EEV_CMD_DEFRO) 
            {
                eev[id].step_sv = eev[id].steps_defrost;/* ��˪ʱΪ��˪���� */
            }
            else
            {
                eev[id].step_sv = eev[id].steps_preopen;
            } 
            if (eev[id].step_sv == eev[id].step_pv)/* �ȴ����Ķ���ִ�� */
            {
                if (cmd >= EEV_CMD_RUN) 
                {
                    eev[id].timer_preopen = 0;
                    eev[id].state = EEV_STATE_PRE_KEEP; 
                }
                else if (cmd == EEV_CMD_RESET) eev[id].state = EEV_STATE_IDLE;
                else if (cmd == EEV_CMD_STOP)  eev[id].state = EEV_STATE_RETURN_ZERO;        /* ���� */
            }
            break;
            
        /* Ԥ��ά�� */  
        case EEV_STATE_PRE_KEEP:          
            if (eev[id].timer_preopen >= eev[id].period_preopen)
            {   
                eev[id].state = EEV_STATE_RUNNING;
                eev[id].cnt_positive = (U32)(EEV_RAPID_TIME_1 - 10) * 10 / eev[id].period_calculate;
                eev[id].cnt_negative = (U32)(EEV_RAPID_TIME_1 - 10) * 10 / eev[id].period_calculate;
                eev[id].timer_calculate = 0;
                eev[id].timer_action = 0;
            }
            else if (cmd == EEV_CMD_RESET)
            {
                eev[id].state = EEV_STATE_IDLE;
            }
            else if (cmd == EEV_CMD_STOP)
            {
                eev[id].state = EEV_STATE_RETURN_ZERO;          /* ���� */
            }
            else
            {
                /* �������³�ʼ���� */
                eev[id].step_sv = eev[id].steps_preopen;
            }
            break;
            
        /* PID���� */   
        case EEV_STATE_RUNNING: 
            
            if (cmd == EEV_CMD_STOP)       eev[id].state = EEV_STATE_RETURN_ZERO;          /* ���� */
            else if (cmd == EEV_CMD_PREV)  eev[id].state = EEV_STATE_PRE_OPEN;
            else if (cmd == EEV_CMD_RESET) eev[id].state = EEV_STATE_IDLE;
            else if (cmd == EEV_CMD_DEFRO) eev[id].state = EEV_STATE_DEFRO;

            eev_set_pid_para(p_id, id);
            if(eev[id].state != EEV_STATE_RUNNING)
            {
                pid_turn_off(p_id);
                break;   
            }
            
            /* pid�������� */
            if (eev[id].timer_calculate >= eev[id].period_calculate)
            {
                eev[id].timer_calculate = 0;  
                pid_turn_on(p_id);
                eev_speedup(id, pv-sv);    /* ���ٴ��� */
            }

            /* ���������� */
            if (eev[id].timer_action >= eev[id].period_action)      
            {
                eev[id].timer_action = 0;
                /* ���ٵ����� */  
                val += (I32)step_quick[id] * eev[id].period_calculate / 100; //ZZL 2018.04.10 ���п��ٵ�����val=8��16 ��24
                step_pid[id] += pid_read_Uk(p_id);//ZZL 2018.04.10 ��������PID������step_pid�п��ܴ��ڿ��ٵ�����

                val += step_pid[id];           

                /* ��ʱ����ٵ��ڴ��� */
                time_pos = eev[id].cnt_positive * eev[id].period_calculate / 10;
                time_neg = eev[id].cnt_negative * eev[id].period_calculate / 10;

                if (time_pos > EEV_RAPID_TIME_2 || time_neg > EEV_RAPID_TIME_2)
                {
                    val = val * 3;
                    if (time_pos > EEV_RAPID_TIME_2 + 30)   /* �ڶ��׶ο���30��󣬻ص���һ�׶ε��� */
                    {
                        eev[id].cnt_positive = (U32)(EEV_RAPID_TIME_2 - 60) * 10 / eev[id].period_calculate;
                    }
                    if (time_neg > EEV_RAPID_TIME_2 + 30)
                    {
                        eev[id].cnt_negative = (U32)(EEV_RAPID_TIME_2 - 60) * 10 / eev[id].period_calculate;
                    }
                }
                else if (time_pos > EEV_RAPID_TIME_1 || time_neg > EEV_RAPID_TIME_1)
                {
                    val = val * 2;
                }
                if(p_id==PID_ID_EEV_1)    DebugPID[DEBUG_PID_MIDDL]=val;   //NOTE-CXW PID����
                if(p_id==PID_ID_EEV_2)    DebugEEV_ID_2PID[DEBUG_PID_MIDDL]=val;   //NOTE-CXW PID����
           
                val = eev_increment_adjust(p_id, id, val);  /* ����Ĳ��� */
                if (p_id == PID_ID_EEV_1)                     /* �����޷� */
                {
                    I16 step_now = eev[id].step_pv;
                    I16 step_limit = MAX(1, (I32)step_now*val_m_limit_range/100);
                    val = fun_pv_limit(val, -step_limit, step_limit);	
                }

                if (id==PID_ID_EEV_1)                       // ��Ƶ���������޷�
                {
                    val += log_mod_eev_freq(cmd, eev[id].step_pv);
                }
                
                if(p_id==PID_ID_EEV_1)DebugPID[DEBUG_STEP]=val;   //NOTE-CXW PID����
                if(p_id==PID_ID_EEV_2)DebugEEV_ID_2PID[DEBUG_STEP]=val;   //NOTE-CXW PID����

                //ԭ����ע���ǣ�����ֵ��Чʱ�Ž�����ֵ����, ��ֹ��ʧ����
                //hzy20170220����ʵ���ǲ��Եģ�val֮����Ϊ0��
                //����Ϊ�������˱仯�������������У����Լ���ֵÿһ�ζ�Ӧ�����㡣
                //Ϊ�˺�ԭ����������һ�£�������������Ч��������߼���
               // if (val != 0)           
                {
                    step_quick[id] = 0;
                    step_pid[id] = 0;   
                }
                
                if (cmd == EEV_CMD_CONST)         /* ���ͷ����ֵ�ǰ״̬�������� */
                {
                    val = 0;
                }
                //ZZL 2018.04.10��if else �����⣬�п��ܵ���Val���ָ�������ѭ�����
                //�յ����Ƶ�ǰ����+ val��С��0�򴥷�
               // if(val<0 && eev[id].step_pv==0) val = 0;  
               // else                            val += eev[id].step_pv;         /* ��ȡ���ͷ���ǰ���� */
                 val += eev[id].step_pv;
                 if(val<0)  val = 0;
               
                if (cmd == EEV_CMD_LIMIT_DOWN)/* ��С���� */ 
                {    
                    if (val < eev[id].step_sv)
                    {
                        val = eev[id].step_sv;
                    }
                } 
                if (cmd == EEV_CMD_LIMIT_UP)/* �������� */ 
                {      
                    if (val > eev[id].step_sv)
                    {
                        val = eev[id].step_sv;
                    }
                }
                if (cmd == EEV_CMD_FORCE_UP     /* ǿ�ƿ��� */ 
                ||  cmd == EEV_CMD_FORCE_DOWN   /* ǿ�ƹ�С */ 
                ||  cmd == EEV_CMD_FORCE_FASTUP /* ����ǿ�ƿ��� */ 
                    )
                {      
                    val = eev[id].step_sv;
                }
                
                eev[id].step_sv = val;
            }
		    //����ǿ�ƿ���
            if (cmd != EEV_CMD_FORCE_FASTUP)
            {
                eev[id].timer_fastadd = 0;
            }
            else if ((eev[id].timer_fastadd >=(eev[id].period_add))&&(eev[id].period_add!=0))    
            {
                eev[id].timer_fastadd = 0;
                eev[id].step_sv = eev[id].step_sv + 2*eev[id].steps_add;
                pid_set_AveAcrossCnt(p_id,0);
            }
            //ǿ�ƿ�����������
            if (cmd != EEV_CMD_FORCE_UP)
            {
                eev[id].timer_add = 0;
            }
            else if ((eev[id].timer_add >= eev[id].period_add)&&(eev[id].period_add!=0))    
            {
                eev[id].timer_add = 0;
                eev[id].step_sv = eev[id].step_sv + eev[id].steps_add;
                pid_set_AveAcrossCnt(p_id,0);
            }
            //ǿ�ƹط�����
            if(cmd != EEV_CMD_FORCE_DOWN)
            {
                eev[id].timer_sub= 0;
            }
            else if ((eev[id].timer_sub>= eev[id].period_sub)&&(eev[id].period_sub!=0))    
            {
                eev[id].timer_sub= 0;
                if(eev[id].step_sv>=eev[id].steps_sub)
                {
                    eev[id].step_sv = eev[id].step_sv - eev[id].steps_sub;
                    pid_set_AveAcrossCnt(p_id,0);
                }
            }

            if (USE_ABILITY_EEV_STEP && id==EEV_ID_1)
    		{
                eev[id].step_sv = val_ability_test_motor_steps;
    		}

#ifdef USE_AT_EEVS
            /* �����������Բ��� */
    		if (USE_ABILITY_EVI_STEP && id==EEV_ID_2)
    		{
                eev[id].step_sv = val_ability_test_s_motor_steps;
    		}
#endif

            break;
            
        default:    
            eev[id].state = EEV_STATE_RETURN_ZERO;
            break;
    }
//    debug[4]=eev[0].state;
//    debug[5]=eev[1].state;
    //����Ŀ�경��    
    eev_io_step_write_sv(id,cmd);
}

/*******************************************************************************
 �� ��: eev_task_area()
 �� ��: �ֶο���
 �� ��: id--�������; 
        cmd--ָ��; 
        setting_steps--ָ������; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void eev_task_area(EEV_ID_ENUM id, EEV_CMD_ENUM cmd, U16 setting_steps)
{
    //��ȡʵ�ʲ���
    eev_io_step_read(id);
    
    switch (eev[id].state)
    {
        case EEV_STATE_RETURN_ZERO: /* ��������� */
            eev_calibrate(id);
            eev[id].state = EEV_STATE_WAIT_ZERO;
            break;
            
        case EEV_STATE_WAIT_ZERO:  /* �ȴ����������� */
            if (eev[id].step_sv == eev[id].step_pv) 
            {
                eev[id].state = EEV_STATE_WAIT_CMD;
            }
            break;

        case EEV_STATE_WAIT_CMD:  /* ׼���ã��ȴ����� */
            if (cmd == EEV_CMD_RESET)/* �������������ת�� �������� �ȴ� */
            {
                eev[id].state = EEV_STATE_IDLE;     
            }
            else if (cmd != EEV_CMD_STOP)/* �����������ת����ʼ���� */
            {
                eev[id].state = EEV_STATE_RUNNING;  
            }
            else if (cmd == EEV_CMD_STOP)
            {
                /* �������������ֶ������ٹر�ʱ��������Ȼͣ�����ֶ����������� */
                eev[id].step_sv = 0;
            }
            break;
            
        case EEV_STATE_IDLE:    /* �������� */
            eev[id].step_sv = eev[id].steps_idle;
            if (eev[id].step_sv == eev[id].step_pv)/* �ȴ����Ķ���ִ�� */
            {
                if (cmd >= EEV_CMD_PREV)   // �������� Ҫô��ѹ��������ת�����п���
                {
                    eev[id].state = EEV_STATE_RUNNING;  
                }
                else if (cmd == EEV_CMD_STOP)// Ҫô��ͣ��һ��ʱ���ת������
                {
                    eev[id].state = EEV_STATE_RETURN_ZERO;  
                }
            }
            break;
            
        case EEV_STATE_RUNNING:     // ���п��ȣ������а�ָ���Ŀ���ִ��
            
            if (eev[id].step_sv == eev[id].step_pv)// �ȴ����Ķ���ִ��
            {   
                if (cmd == EEV_CMD_RESET)
                {
                    eev[id].state = EEV_STATE_IDLE;             // ��������
                }
                else if (cmd == EEV_CMD_STOP)
                {
                    eev[id].state = EEV_STATE_RETURN_ZERO;        // ����
                }
            }
            if (eev[id].timer_action >= eev[id].period_action)    // �Ѹ�ֵ���ں��棬��Ϊ�˼�ʱ�����л����� 
            {
                eev[id].timer_action = 0;
                if(cmd == EEV_CMD_CONST)    //ά��
                {
                    eev[id].step_sv = eev[id].step_sv;
                }
                else
                {
                    eev[id].step_sv = setting_steps;
                }
            }
            break;
        
        default:
            eev[id].state = EEV_STATE_RETURN_ZERO;
            break;
    }
    //����Ŀ�경��
    eev_io_step_write_sv(id,cmd);
}


/*******************************************************************************
 �� ��: eev_set_steps_idle()
 �� ��: ���ô�������
 �� ��: id--�������; 
        value--����ֵ; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL eev_set_steps_idle(EEV_ID_ENUM id, U16 value)
{
    BOOL result = TRUE;
    
    if (id >= EEV_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        eev[id].steps_idle = value;//��������
    }

    return result;
}

/*******************************************************************************
 �� ��: eev_set_steps_preopen()
 �� ��: ����Ԥ������
 �� ��: id--�������; 
        value--����ֵ; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL eev_set_steps_preopen(EEV_ID_ENUM id, U16 value)
{
    BOOL result = TRUE;
    
    if (id >= EEV_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        eev[id].steps_preopen = value;//Ԥ������
    }

    return result;
}

/*******************************************************************************
 �� ��: eev_set_steps_defrost()
 �� ��: ���ó�˪����
 �� ��: id--�������; 
        value--����ֵ; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL eev_set_steps_defrost(EEV_ID_ENUM id, U16 value)
{
    BOOL result = TRUE;
    
    if (id >= EEV_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        eev[id].steps_defrost = value;//��˪����
    }

    return result;
}

/*******************************************************************************
 �� ��: eev_set_steps_max()
 �� ��: �����ܲ���
 �� ��: id--�������; 
        value--����ֵ; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL eev_set_steps_max(EEV_ID_ENUM id, U16 value)
{
    BOOL result = TRUE;
    
    if (id >= EEV_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        eev[id].steps_max = value;//�ܲ���
    }

    return result;
}

/*******************************************************************************
 �� ��: eev_set_steps_run_max()
 �� ��: ����������в���
 �� ��: id--�������; 
        value--����ֵ; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL eev_set_steps_run_max(EEV_ID_ENUM id, U16 value)
{
    BOOL result = TRUE;
    
    if (id >= EEV_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        eev[id].steps_run_max = value;//������в���
    }

    return result;
}
/*******************************************************************************
 �� ��: eev_set_steps_run_max()
 �� ��: ����������в���
 �� ��: id--�������; 
        value--����ֵ; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
I16 eev_get_steps_run_max(EEV_ID_ENUM id)
{
    I16 result = 0;
    
    if (id >= EEV_ID_MAX)
    {
        result = 0;
    }
    else
    {
        result=eev[id].steps_run_max;//������в���
    }

    return result;
}

/*******************************************************************************
 �� ��: eev_set_steps_min()
 �� ��: ������С����
 �� ��: id--�������; 
        value--����ֵ; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL eev_set_steps_min(EEV_ID_ENUM id, U16 value)
{
    BOOL result = TRUE;
    
    if (id >= EEV_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        eev[id].steps_run_min = value;//��С����
    }

    return result;
}
/*******************************************************************************
 �� ��: eev_set_steps_run_max()
 �� ��: ����������в���
 �� ��: id--�������; 
        value--����ֵ; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
I16 eev_get_steps_min(EEV_ID_ENUM id)
{
    I16	result = 0;
    
    if (id >= EEV_ID_MAX)
    {
        result = 0;
    }
    else
    {
        result=eev[id].steps_run_min;//������в���
    }

    return result;
}


/*******************************************************************************
 �� ��: eev_set_step_speed()
 �� ��: ��������Ƶ�����
 �� ��: id--�������; 
        value--����ֵ; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL eev_set_step_speed(EEV_ID_ENUM id, U16 value)
{
    BOOL result = TRUE;
    
    if (id >= EEV_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        eev[id].motor_fre = value;  //����Ƶ�����
    }

    return result;
}

/*******************************************************************************
 �� ��: eev_set_motor_type()
 �� ��: �������ŷ�ʽ
 �� ��: id--�������; 
        value--����ֵ; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL eev_set_motor_type(EEV_ID_ENUM id, U16 value)
{
    BOOL result = TRUE;
    
    if (id >= EEV_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        eev[id].motor_type = value;  //����Ƶ�����
    }

    return result;
}


/*******************************************************************************
 �� ��: eev_degree_to_steps()
 �� ��: ����ת��Ϊ����
 �� ��: id--�������; 
        DegreeValue--����ֵ; 
 �� ��: ����
 �� ��: Ӧ��eev_set_steps_max֮��ʹ�á�
*******************************************************************************/
U16 eev_degree_to_steps(EEV_ID_ENUM id, U16 DegreeValue)
{
    U16 Steps = 0;
    
    if (id >= EEV_ID_MAX )
    {
        Steps = 0;
    }
    else
    {
        Steps = (U32)DegreeValue * eev[id].steps_max / 100;     // ���ȶ�Ӧ����
    }

    return Steps;
}

/*******************************************************************************
 �� ��: eev_get_step_speed()
 �� ��: ��ȡ��������Ƶ�����
 �� ��: id--�������; 

 �� ��: ����Ƶ�����
 �� ��: ��
*******************************************************************************/
U16 eev_get_step_speed(EEV_ID_ENUM id)
{
    U16 StepSpeed = 0;
    
    if (id >= EEV_ID_MAX )
    {
        StepSpeed = 0;
    }
    else
    {
        StepSpeed = eev[id].motor_fre;     
    }

    return StepSpeed;
}

/*******************************************************************************
 �� ��: eev_get_motor_type()
 �� ��: ��ȡ�������ŷ�ʽ
 �� ��: id--�������; 

 �� ��: ���ŷ�ʽ
 �� ��: ��
*******************************************************************************/
U16 eev_get_motor_type(EEV_ID_ENUM id)
{
    U16 MotorType = 0;
    
    if (id >= EEV_ID_MAX )
    {
        MotorType = 0;
    }
    else
    {
        MotorType = eev[id].motor_type;     
    }

    return MotorType;
}
/*******************************************************************************
 �� ��: eev_get_motor_type()
 �� ��: ��ȡ�������ŷ�ʽ
 �� ��: id--�������; 

 �� ��: ���ŷ�ʽ
 �� ��: ��
*******************************************************************************/
U16 eev_get_timer_initchange(EEV_ID_ENUM id)
{
    U16 MotorType = 0;
    
    if (id >= EEV_ID_MAX )
    {
        MotorType = 0;
    }
    else
    {
        MotorType = eev[id].timer_initchange;     
    }

    return MotorType;
}

/*******************************************************************************
 �� ��: eev_set_period_preopen()
 �� ��: ����Ԥ��ʱ��
 �� ��: id--�������; 
        value--����ֵ; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL eev_set_period_preopen(EEV_ID_ENUM id, U16 value)
{
    BOOL result = TRUE;
    
    if (id >= EEV_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        eev[id].period_preopen = value;//Ԥ��ʱ��(100ms)
    }

    return result;
}

/*******************************************************************************
 �� ��: eev_set_period_calculate()
 �� ��: ���ü�������
 �� ��: id--�������; 
        value--����ֵ; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL eev_set_period_calculate(EEV_ID_ENUM id, U16 value)
{
    BOOL result = TRUE;
    
    if (id >= EEV_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        eev[id].period_calculate = value;//��������(100ms)
    }

    return result;
}

/*******************************************************************************
 �� ��: eev_get_step_fre()
 �� ��: ��ȡ����Ƶ��
 �� ��: id--�������; 
 �� ��: result--����Ƶ�����
 �� ��: ��
*******************************************************************************/
//U16 eev_get_step_fre(EEV_ID_ENUM id)
//{
//    U16 result = 0;
//    
//    if (id >= EEV_ID_MAX)
//    {
//        result = 0;
//    }
//    else
//    {
//        result = eev[id].moter_fre;
//    }
//
//    return result;
//}
/*******************************************************************************
 �� ��: eev_set_step_fre()
 �� ��: ��������Ƶ��
 �� ��: id--�������; 
        value--����ֵ������Ƶ����ţ�
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
//BOOL eev_set_step_fre(EEV_ID_ENUM id, U16 value)
//{
//    BOOL result = TRUE;
//    
//    if (id >= EEV_ID_MAX)
//    {
//        result = FALSE;
//    }
//    else
//    {
//        eev[id].moter_fre = value;
//    }
//
//    return result;
//}

/*******************************************************************************
 �� ��: eev_set_period_action()
 �� ��: ���ö�������
 �� ��: id--�������; 
        value--����ֵ; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL eev_set_period_action(EEV_ID_ENUM id, U16 value)
{
    BOOL result = TRUE;
    
    if (id >= EEV_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        eev[id].period_action = value;//��������(100ms)
    }

    return result;
}
/*******************************************************************************
 �� ��: eev_set_period_add()
 �� ��: ����ǿ�ƿ���������
 �� ��: id--�������; 
        value--����ֵ; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL eev_set_period_add(EEV_ID_ENUM id, U16 value)
{
    BOOL result = TRUE;
    
    if (id >= EEV_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        eev[id].period_add = value;//��������(100ms)
    }

    return result;
}
/*******************************************************************************
 �� ��: eev_set_steps_add()
 �� ��: ����ǿ������������
 �� ��: id--�������; 
        value--����ֵ; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL eev_set_steps_add(EEV_ID_ENUM id, U16 value)
{
    BOOL result = TRUE;
    
    if (id >= EEV_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        eev[id].steps_add = value;//��������(100ms)
    }

    return result;
}
/*******************************************************************************
 �� ��: eev_set_period_sub()
 �� ��: ����ǿ�ƿ�С��������
 �� ��: id--�������; 
        value--����ֵ; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL eev_set_period_sub(EEV_ID_ENUM id, U16 value)
{
    BOOL result = TRUE;
    
    if (id >= EEV_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        eev[id].period_sub= value;//��������(100ms)
    }

    return result;
}
/*******************************************************************************
 �� ��: eev_set_steps_sub()
 �� ��: ����ǿ�ƿ�С������
 �� ��: id--�������; 
        value--����ֵ; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL eev_set_steps_sub(EEV_ID_ENUM id, U16 value)
{
    BOOL result = TRUE;
    
    if (id >= EEV_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        eev[id].steps_sub = value;//��������(100ms)
    }

    return result;
}
/*******************************************************************************
 �� ��: eev_set_period_hold()
 �� ��: ����ǿ������������
 �� ��: id--�������; 
        value--����ֵ; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL eev_set_period_hold(EEV_ID_ENUM id, U16 value)
{
    BOOL result = TRUE;
    
    if (id >= EEV_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        eev[id].period_hold = value;//����ά��ʱ��(��)
    }

    return result;
}
/*******************************************************************************
 �� ��: eev_set_period_cyc()
 �� ��: ����ǿ������������
 �� ��: id--�������; 
        value--����ֵ; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL eev_set_period_cyc(EEV_ID_ENUM id, U16 value)
{
    BOOL result = TRUE;
    
    if (id >= EEV_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        eev[id].period_cyc = value;//��������(��)
    }

    return result;
}

/*******************************************************************************
 �� ��: adjust_method_U16()
 �� ��: ���ں���_U16����
 �� ��:   adj_info--����Ҫ�󣨰�����Сֵ�����ֵ��������; 
        original_data--ԭʼֵ
        direction--��������; 
 �� ��: adj_data--�������ֵ
 �� ��: ��
*******************************************************************************/
#define DIREC_ADD   0   // ����
#define DIREC_SUB   1   // ����
U16 adjust_method_U16(ADJUST_STRUCT adj_info, U16 original_data, U16 direction)
{
    U16 adj_data;
    adj_data = original_data;
    
    if (DIREC_ADD == direction)
    {
        if (adj_data < (adj_info.adj_max-adj_info.adj_step))
        {
            adj_data += adj_info.adj_step;
        }
        else
        {
            adj_data = adj_info.adj_max;
        }
    }
    else if (DIREC_SUB == direction)
    {
        if (adj_data > (adj_info.adj_min+adj_info.adj_step))
        {
            adj_data -= adj_info.adj_step;
        }
        else
        {
            adj_data = adj_info.adj_min;
        }
    }

    if(adj_data > adj_info.adj_max)   adj_data=adj_info.adj_max;
    if(adj_data < adj_info.adj_min)   adj_data=adj_info.adj_min;
    
    return adj_data;
}


/*******************************************************************************
 �� ��: eev_isReady()
 �� ��: ��ȡ�����Ƿ���׼���õ�״̬
 �� ��: id--�������; 

 �� ��: result--����׼���õ�״̬(TRUE-׼���ã�FALSE-δ׼����)
 �� ��: �������Ϊδ׼���ã�Ӧ�ò�����ѹ��
*******************************************************************************/
BOOL eev_isReady(EEV_ID_ENUM id)
{
    BOOL result = TRUE;
    
    if (id >= EEV_ID_MAX || eev[id].state < EEV_STATE_WAIT_CMD)
    {
        result = FALSE;
    }

    return result;
}
/*******************************************************************************
 �� ��: BOOL eev_holdjudge(BOOL curt_state,EEV_ID_ENUM id)
 �� ��: �Ƿ�ά�ֵ�ǰ�����ж�
 �� ��: curt_state----��ǰ����״̬; 
		curt_temp--��ǰֵ; 
		borderline--�߽�ֵ; 
		Diff--�ز�ֵ; 
		direction--�жϷ���(DRCT_UNDER��С�ڱ߽�ֵΪ�棻DRCT_ABOVE�����ڱ߽�ֵΪ��); 
		
 �� ��: TRUE/FALSE--�µĿ���״̬Ϊ��ά��/�ٲ�ά��
 �� ��: 
*******************************************************************************/

BOOL eev_holdjudge(BOOL curt_state,EEV_ID_ENUM id)
{  
    BOOL new_state = curt_state;    //��״̬


    if(eev[id].period_hold==0)  //ά��ʱ��Ϊ0��û��ά�ֹ���
    {
        new_state = FALSE;
        eev[id].timer_hold = 0;
    }
    else
    {
        if(eev[id].timer_hold>=eev[id].period_hold+eev[id].period_cyc)
        {
            eev[id].timer_hold = 0;
            new_state = FALSE;
        }
        else if(eev[id].timer_hold>=eev[id].period_cyc)
        {
            new_state = TRUE;
        }
        else if(eev[id].timer_hold>=0)
        {
            new_state = FALSE;
        }
    }
    
    return new_state;
}

/*******************************************************************************
 �� ��: eev_get_init_step_pv()
 �� ��: ��ȡ��ʼ��ʱ�ĵ���ǰ����(��ǰ����*1.2)�����ڹ���
 �� ��: id--; 
 �� ��: 
 �� ��: ��
*******************************************************************************/
U16 eev_get_init_step_pv(EEV_ID_ENUM id)
{
    return MAX((U32)eev[id].step_pv * 12 / 10, 60);			/* ����50*1.2=60����Ϊ�˹��step_pv��step_sv��� */
}

