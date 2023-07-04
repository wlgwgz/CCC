/******************************************************************************

          ��Ȩ���� (C), 2001-2016, �����а��յ��Լ����������޹�˾

 ******************************************************************************
  �� �� ��   : app_defrost.c
  �� �� ��   : ����
  ��    ��   : ��־��
  ��������   : 2018��12��28��������
  ����޸�   :
  ��������   : ��˪�߼�
  �����б�   :
              defrost
              defrost_by_emergency
              defrost_by_env_wing
              defrost_by_humidity
              defrost_by_lowpress
              defrost_by_manual
              defrost_by_wing
              defrost_cm_stop
              defrost_fun_frost_time
              defrost_fun_get_interval
              defrost_fun_get_wing
              defrost_fun_rec_data
              defrost_grp_need_exit
              defrost_grp_need_into
  �޸���ʷ   :
  1.��    ��   : 2018��12��28��������
    ��    ��   : ��־��
    �޸�����   : �����ļ�

******************************************************************************/
#define GBL_DEFROST     1
/*-----------------����ͷ�ļ� -----------------------------*/
#include "Includes.h"


//1 *-----------------�ӿ�----------------------------*/
// Ϊ��ʹ����ͳһ,����Ϊ�˼��ݲ�ͬ����Ĳ�ͬʵ��,����ƵĽӿ�,
// ����������򲻼��ݱ��ļ��ĺ�����,���ú궨��ʵ��֮
// ����궨�岻��ʵ��,�뷴��

extern const U08 err_wing[];

#define interface_defrost_wing_err(cm)          (s_get_error(err_wing[cm]))

#define log_mod_fan_comp_group                  fan_comp_group_count
#define val_defrost_humidity_fun_en             FALSE
#define pv_sys_dew                              SENSOR_NOEXIST       
#define val_defrost_enter_out			        (val_defrost_end_out+60)  	/* ����˪ˮ�� */
#define val_defrost_end_diff                    20
#define val_cm_on_time                          COMP_NEED_ON_TIME
#define val_defrost_env_wing_dly                60
#define LOG_USED                                USED

//1 �����������������������������,Ӧ���г���һ��!!!!!
// �粻һ��,��˵�����ļ���һ��.

/*-----------------�ⲿ����˵��----------------------------*/


/*-----------------�ⲿ����ԭ��˵��------------------------*/



/*-----------------�ڲ�����ԭ��˵��------------------------*/



/*-----------------ȫ�ֱ���--------------------------------*/




/*-----------------ģ�鼶����------------------------------*/
GSTATIC U16 defrost_cnt_defrost[MAX_COMP];       // ��˪ʱ���ʱ 
GSTATIC U16 defrost_cnt_frost[MAX_COMP];         // ��˪ʱ���ʱ
GSTATIC U16 defrost_cnt_defrost_end[MAX_COMP];   // ��˪��ʱ���ʱ
GSTATIC U08 defrost_cnt_wing_confirm[MAX_COMP];  // ��˪����ȷ�ϼ�ʱ
GSTATIC U08 defrost_cnt_out_temp_low[MAX_COMP];  // ��˪��ˮ�¶ȵͼ�ʱ

GSTATIC U16 defrost_cnt_wing_temp_high[MAX_COMP]; // ��˪��Ƭ�¶ȸ߼�ʱ

//1 ������¼
#define DEFROST_REC_ENV_WING_DIFF_INTV      30      /* ��¼��������� */
#define DEFROST_REC_ENV_WING_DIFF_NUM       10      /* ��¼�������� */
#define DEFROST_REC_ENV_WING_DIFF_STAB_TIME (DEFROST_REC_ENV_WING_DIFF_INTV*DEFROST_REC_ENV_WING_DIFF_NUM)
GSTATIC U08 defrost_cnt_rec_EWD;          /* ������¼��ʱ */
GSTATIC I16 defrost_rec_EWD[MAX_COMP][DEFROST_REC_ENV_WING_DIFF_NUM];/* ������¼���� */
GSTATIC I16 defrost_rec_EWD_avg[MAX_COMP];     /* ƽ������� */
#define DEFROST_REC_EWD_AVG_INTV    30      /* ��¼ƽ��������ֵ */
#define DEFROST_REC_EWD_AVG_NUM     10      /* ��¼ƽ���������� */
GSTATIC U08 defrost_cnt_rec_EWD_avg;          /* ƽ��������¼��ʱ */
GSTATIC I16 defrost_rec_EWD_usedavg[MAX_COMP][DEFROST_REC_ENV_WING_DIFF_NUM];/* �������Ч��ֵ��¼���� */
GSTATIC I16 defrost_rec_EWD_usedavg_avg[MAX_COMP]; /* ƽ���������Ч��ֵ */
GSTATIC I16 defrost_temp_EWD_nofrost[MAX_COMP];/* ��˪���л���� */
GSTATIC I16 defrost_temp_EWD_nofrost_env[MAX_COMP];/* ��˪���л����궨ʱ���� */
GSTATIC U16 defrost_cnt_env_wing_rise_diff_1[MAX_COMP];  /* ���������1ʱ�� */
GSTATIC U16 defrost_cnt_env_wing_rise_diff_2[MAX_COMP];  /* ���������2ʱ�� */
GSTATIC I16 defrost_temp_EWD_stop[MAX_COMP];   /* ͣ������� */

#define DEFROST_REC_EWD_START_TIME (7*60 + DEFROST_REC_ENV_WING_DIFF_STAB_TIME) /* ��˪���л����궨��ʼʱ�� */
#define DEFROST_REC_EWD_END_TIME (15*60 + DEFROST_REC_ENV_WING_DIFF_STAB_TIME) /* ��˪���л����궨����ʱ�� */

U16 defrost_power_dly[MAX_COMP];/* ��˥��˪���� */

//1 ������˪
#ifndef USE_QUICK_TIME
	#define DEFROST_EMERGENCY_TIME          120     // 120������
	#define DEFROST_EMERGENCY_MAX_TIMES     2       // ������3�α���
#else
	#define DEFROST_EMERGENCY_TIME          5     	// n������
	#define DEFROST_EMERGENCY_MAX_TIMES     1       // ������m+1�α���
#endif /* USE_QUICK_TIME */

U08 defrost_emergency_cnt[MAX_COMP];  /* ������˪���� */
U08 defrost_cnt_emergency_intv[MAX_COMP][DEFROST_EMERGENCY_MAX_TIMES];/* ������˪�����ʱ */

U16 defrost_emergency_dly[MAX_COMP];    /* ������˪���� */
U16 defrost_gas_in_dly[MAX_COMP];       /* �����¶ȳ�˪���� */

/*-----------------��������--------------------------------*/



/*-----------------�궨�� ---------------------------------*/

I16 defrost_EWD_except_diff(U08 cm)
{
    return fun_pv_sub(defrost_temp_EWD_nofrost[cm], 
                      defrost_temp_EWD_stop[cm]);
}

void defrost_EWD_except_reset(U08 cm)
{
    defrost_temp_EWD_nofrost[cm] = SENSOR_NOEXIST;
    defrost_temp_EWD_stop[cm] = SENSOR_NOEXIST;
}


I16 defrost_get_value(U08 cm, U08 val_type)
{
    switch (val_type)
    {
        case 0: return SENSOR_NOEXIST;                      /* �� */
        case 1: return SENSOR_NOEXIST;                      /* �� */
        case 2: return SENSOR_NOEXIST;                      /* �� */
        case 3: return defrost_rec_EWD_avg[cm];             /* ƽ������� */
        case 4: return defrost_rec_EWD_usedavg_avg[cm];     /* ƽ��������ֵ */
        case 5: return defrost_temp_EWD_nofrost[cm];        /* ��˪���л���� */
        case 6: return defrost_temp_EWD_nofrost_env[cm];    /* ��˪���л����궨ʱ���� */
        case 7: return defrost_temp_EWD_stop[cm];           /* ͣ������� */
		case NUM_EXIT_DEFR_CM_OFF: return defrost_fg[cm].exit_defr_cm_off;		/* �˳�˪ʱѹ��ͣ����־ */
    }

    return SENSOR_NOEXIST;
}

/*******************************************************************************
 �� ��: defrost_is_allow_auto()
 �� ��: �ж��Ƿ������Զ���˪
 �� ��: ��
 �� ��: 
 �� ��: �ֶ�����ʱ��ֹ�Զ���˪
*******************************************************************************/
BOL defrost_is_allow_auto(void)
{
    BOL result = TRUE;

    if (val_invt_manual_mode == USED
    ||  val_motor_manual_mode == USED
    ||  val_s_motor_manual_mode == USED
    ||  val_freq_fan_ctrl == FREQ_FAN_CTRL_MANUAL
    ||  ABILITY_TEST_NO_DEFR
    ||  !TF_IS_IDLE
        )
    {
        result = FALSE;
    }

    return result;
}

/*****************************************************************************
��  �� : defrost_fun_get_wing()
��  �� : ѡ���Ƭ�¶�
��  �� : U08 cm : ѹ����
��  �� : ��
��  �� : ��
*****************************************************************************/
I16 defrost_fun_get_wing(U08 cm)
{
    return (pv_cell_wing(cm));
}

/*****************************************************************************
��  �� : defrost_fun_get_out_temp()
��  �� : ѡ���ˮ�¶�
��  �� : U08 cm : 
��  �� : ��
��  �� : ��
*****************************************************************************/
I16 defrost_fun_get_out_temp(U08 cm)
{
	I16 temp_low = SENSOR_NOEXIST;

    cm = cm;
	
	temp_low = fun_pv_min(pv_cell_TWin1(pcb_addr), pv_cell_TWout1(pcb_addr));
	
    return (temp_low);
}

/*****************************************************************************
��  �� : defrost_fun_get_env()
��  �� : ѡ�񻷾��¶�
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
I16 defrost_fun_get_env(void)
{
    return pv_cell_TA(pcb_addr);
}

/*******************************************************************************
 �� ��: defrost_fun_get_gas_in()
 �� ��: ѡ�������¶�
 �� ��: cm--; 
 �� ��: 
 �� ��: ��
*******************************************************************************/
I16 defrost_fun_get_gas_in(U08 cm)
{
    return pv_cell_gas_in(cm);
}

/*****************************************************************************
��  �� : defrost_fun_get_interval()
��  �� : ��ȡ��˪����
��  �� : ��
��  �� : ��˪����,��
��  �� : ��
*****************************************************************************/
I16 defrost_fun_get_interval(U08 cm)
{
    static U08 env_area = AREA_1;
    I16 interval;
    I16 temp_env = defrost_fun_get_env();

    env_area = Fun_LookupArea(temp_env, env_area, val_defrost_environ1,
                                                  val_defrost_environ2,
                                                  val_defrost_environ3,
                                                  val_defrost_environ4, 10);
    
    switch(env_area)
    {
        default:        
        case AREA_1: interval = val_defrost_period1; break;
        case AREA_2: interval = val_defrost_period2; break;
        case AREA_3: interval = val_defrost_period3; break;
        case AREA_4: interval = val_defrost_period4; break;
        case AREA_5: interval = val_defrost_period5; break;
    }

    /* �״γ�˪ */
    if (defrost_fg[cm].first_defrost == FALSE)
    {
        interval = val_defrost_first_run_period;
    }
    
    return AT_VAL(interval, val_ability_test_interval);		/* ʱ�����ͳһ�ŵ����������д��� */
//    return interval*60;
}

/*****************************************************************************
��  �� : defrost_fun_rec_data()
��  �� : ���ݼ�¼,����������¶ȵ�
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void defrost_fun_rec_data(void)
{
    U08 cm;
    I16 temp_env = defrost_fun_get_env(); 
    
    for (cm=0; cm<val_comp_num; cm++)
    {       
        I16 temp_wing = defrost_fun_get_wing(cm);
              
        
        //3 ��˪������¼
        if (defrost_fg[cm].need_calib_EWD)
        {   
            BOL calib_EWD_end = FALSE;  // ȡֵ����
            I16 temp_EWD_nofrost_new = SENSOR_NOEXIST;  // �µ���˪���л����
            
            //4 ��˪��7����+�ȶ�ʱ�俪ʼ�궨,ȡ�궨8�����ڵ���Сƽ�������
            if (defrost_cnt_defrost_end[cm] < DEFROST_REC_EWD_START_TIME)
            {
                ;// nothing
            }
            //4 ��¼��С�����
            else if (defrost_cnt_defrost_end[cm] <= DEFROST_REC_EWD_END_TIME)
            {
                /* ��Ƭ�¶�<=�ɳ�˪���� */   /* �����¶�<=�ɳ�˪���� */
                if ((temp_wing <= val_defrost_allow_wing+30)&&(temp_env <= val_defrost_allow_environ+30))
                if (fun_pv_in(defrost_rec_EWD_avg[cm], val_defrost_env_wing_lower, val_defrost_env_wing_upper))
                {
                    //������0.5���ȶ�,ȡֵ����
                    if (fun_stab_avg(defrost_rec_EWD[cm], DEFROST_REC_ENV_WING_DIFF_NUM,  -5, 5))
                    {
                        temp_EWD_nofrost_new = defrost_rec_EWD_avg[cm];
                        calib_EWD_end = TRUE;
                    }
                }
            }
            //4 �궨����,ͳ�ƽ��
            else    
            {
                // ȡ��ֵ�ľ�ֵ
                temp_EWD_nofrost_new = defrost_rec_EWD_usedavg_avg[cm];
                calib_EWD_end = TRUE;
            }
            
            if (calib_EWD_end)
            { 
                // ���±仯
                I16 env_change = fun_pv_sub(temp_env, defrost_temp_EWD_nofrost_env[cm]);
                // �ɹ�
                if (SensorCanUse(temp_EWD_nofrost_new))
                {
                    I16 old_change = fun_pv_sub(temp_EWD_nofrost_new, defrost_temp_EWD_nofrost[cm]);
                    // �����±仯��1�����Һ;�ֵ���5�����ϣ�ȡСֵ
                    if (fun_pv_in(env_change , -10, 10) && !fun_pv_in(old_change , -50, 50))
                    {
                        defrost_temp_EWD_nofrost[cm] = fun_pv_min(temp_EWD_nofrost_new, defrost_temp_EWD_nofrost[cm]);
                    }
                    else
                    {
                        defrost_temp_EWD_nofrost[cm] = temp_EWD_nofrost_new;
                        defrost_temp_EWD_nofrost_env[cm] = temp_env;
                    }                    
                }
                // ʧ��
                else
                {
                    // �궨ʧ��,���±仯��2���ڣ�������ֵ�����������
                    if (!fun_pv_in(env_change , -20, 20))
                    {   // ��Ϊ����̬
                        defrost_temp_EWD_nofrost[cm] = SENSOR_ERROR;
                    }
                }
                defrost_fg[cm].need_calib_EWD = FALSE;
            }
        }

        //3 ͣ��������¼
        if (S_CM_GetStatus(cm, CM_ON))  // ������,�Ϳ��Ա궨ͣ���������
        {
            defrost_fg[cm].need_calib_stop_EWD = TRUE;
        }
        
        if (defrost_fg[cm].need_calib_stop_EWD)
        {
            BOL grp_off_time_enough = TRUE;
            BOL cm_in_grp = FALSE;
            U08 i;
            U08 begin = 0, end = 0;
            log_mod_fan_comp_group(log_mod_cond_type(val_comp_num), &begin, &end, cm);
            #define CALIB_STOP_EWD_TIME     (25*60 + DEFROST_REC_ENV_WING_DIFF_STAB_TIME)
            // ͬ��ͣ��ʱ�䶼����
            for (i=begin; i<end; i++)
            {
                cm_in_grp = TRUE;
                if (cm_time[i].off < CALIB_STOP_EWD_TIME)
                {
                    grp_off_time_enough = FALSE;
                    break;
                }
            }
            
            if (cm_in_grp && grp_off_time_enough
                && power_time >= CALIB_STOP_EWD_TIME)    // �ϵ�ʱoffֱ����65535,�״��ϵ�ʱ��Ҳ������
            {
                if (fun_stab_avg(defrost_rec_EWD[cm], DEFROST_REC_ENV_WING_DIFF_NUM, -20, 20)
                    && fun_pv_in(defrost_rec_EWD_avg[cm], -100, 100))
                {
                    // ֱ�Ӹ���
                    defrost_temp_EWD_stop[cm] = defrost_rec_EWD_avg[cm];
                    defrost_fg[cm].need_calib_stop_EWD = FALSE;
                }
            }
        }


   		//3 Ŧ��̩Ҫ����ʾ����˪���л����ı궨���
        if (Cm[cm].run_mode != MODE_COOL)
        	pv_cell_nofrost_EWD(cm) = fun_pv_get(defrost_temp_EWD_nofrost[cm], -999);
//			pv_cell_nofrost_EWD(pcb_addr, cm) = fun_pv_get(defrost_temp_EWD_nofrost[cm], -999);

		//3 ѹ������ʱ������˳�˪ʱѹ��ͣ����־
		if (S_CM_GetStatus(cm, CM_NEED) && S_CM_GetStatus(cm, CM_ON))
		{
		    defrost_fg[cm].exit_defr_cm_off = FALSE;
		}

    }
}

/*****************************************************************************
��  �� : defrost_time()
��  �� : ��ʱ����
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void defrost_time(void)
{
    U16 cm;
    U16 k;
    I16 temp_env = defrost_fun_get_env();
    
    if (F_TIME_BASIC_SEC_BACK)
    {
		_CNT(defrost_cm_intv_on);
		_CNT(defrost_cm_intv_off);
		
        for (cm=0; cm<val_comp_num; cm++)
        {
            I16 temp_wing = defrost_fun_get_wing(cm);
            I16 temp_EWD = fun_pv_sub(temp_env, temp_wing); /* ����� */
            I16 temp_out = defrost_fun_get_out_temp(cm);
            
            _CNT_WHEN(defrost_cnt_defrost[cm], S_CM_GetStatus(cm, CM_DEFROST));
            _CNT_WHEN(defrost_cnt_out_temp_low[cm], temp_out <= val_defrost_end_out);
            _CNT_WHEN(defrost_cnt_defrost_end[cm], defrost_fg[cm].defrost_end   \
                                                && S_CM_GetStatus(cm, CM_ON));
			_CNT_WHEN(defrost_cnt_wing_temp_high[cm], temp_wing >= val_exit_dfr_wingT2 \
												&& S_CM_GetStatus(cm, CM_DEFROST));
			_CNT(defrost_cm_time[cm]);
			
            /* ��������ȶ���ʱ */
            {
                /* ������� */
                I16 temp_EWD_rise = fun_pv_sub(temp_EWD, defrost_temp_EWD_nofrost[cm]);
                _CNT_WHEN(defrost_cnt_env_wing_rise_diff_1[cm], SensorCanUse(temp_EWD_rise)   \
                                                && temp_EWD_rise >= val_defrost_EWD_rise_diff1);
                _CNT_WHEN(defrost_cnt_env_wing_rise_diff_2[cm], SensorCanUse(temp_EWD_rise)   \
                                                && temp_EWD_rise >= val_defrost_EWD_rise_diff2);
                
            }

            /* ��˪ʱ������ */
            if(val_defrost_time_sel == DEFRO_BY_FROSTTIME)
            {
                _CNT_WHEN(defrost_cnt_wing_confirm[cm], (temp_wing > 20));
                /* ��Ƭ�¶ȳ���60S����2ʱ��˪�ۼ�ʱ������ */
                if (defrost_cnt_wing_confirm[cm] > 60)
                {
                    defrost_cnt_frost[cm] = 0;      
                }
            }
            
            /* ���ڳ�˪,��˪ʱ������ */
            if (S_CM_GetStatus(cm, CM_DEFROST)) 
            {
                defrost_cnt_frost[cm] = 0;  
            }
            else
            {
                BOL is_frost = FALSE;
                
                /* ����˪ʱ�� */
                if (val_defrost_time_sel == DEFRO_BY_FROSTTIME)
                {
                    /* ��ʪ�ȼ�ʱ */
                    if (val_defrost_humidity_fun_en && SensorCanUse(pv_sys_dew))
                    {
                        is_frost = (temp_wing <= pv_sys_dew);
                    }
                    else
                    {
                        is_frost = (temp_wing <= 0 && temp_wing < temp_env);
                    }
                }
                /* ��ѹ������ʱ�� */
                else if (val_defrost_time_sel == DEFRO_BY_RUNTIME)
                {
                    is_frost = (S_CM_GetStatus(cm, CM_ON) != 0);
                }
                                                            /* ����120S��ſ�ʼ����Ƭ�¶� */
                _CNT_WITH(defrost_cnt_frost[cm], is_frost && (cm_time[cm].on > 120));
            }     

			//4 ������˪������ʱ
			_CNT(defrost_emergency_dly[cm]);

			_CNT(defrost_power_dly[cm]);
        }

        
        /* ��¼��������� */
        _CNT(defrost_cnt_rec_EWD);
        if (defrost_cnt_rec_EWD >= DEFROST_REC_ENV_WING_DIFF_INTV)
        {
            defrost_cnt_rec_EWD = 0;
            for (cm=0; cm<val_comp_num; cm++)
            {
                I16 temp_EWD = temp_env - defrost_fun_get_wing(cm);
                defrost_rec_EWD_avg[cm] = fun_rec_avg(temp_EWD, &defrost_rec_EWD[cm][0], 
                                                    DEFROST_REC_ENV_WING_DIFF_NUM);
            }
        }

        /* ��¼��Чƽ������� */
        _CNT(defrost_cnt_rec_EWD_avg);
        if (defrost_cnt_rec_EWD_avg >= DEFROST_REC_EWD_AVG_INTV)
        {
            defrost_cnt_rec_EWD_avg = 0;
            for (cm=0; cm<val_comp_num; cm++)
            {
                // ������ڡ�2���¶�,��¼һ�ξ�ֵ
                if ((fun_stab_avg(defrost_rec_EWD[cm], DEFROST_REC_ENV_WING_DIFF_NUM,  -20, 20)))
                {
                    defrost_rec_EWD_usedavg_avg[cm] = fun_rec_avg(defrost_rec_EWD_avg[cm], &defrost_rec_EWD_usedavg[cm][0], 
                                                    DEFROST_REC_EWD_AVG_NUM);
                }
            }
        }
    }

    if (F_TIME_BASIC_MIN_BACK)
    {
        for (cm=0; cm<val_comp_num; cm++)
        {
            //4 ������˪�����ʱ
            for (k=0; k<DEFROST_EMERGENCY_MAX_TIMES; k++)
            {
                if (defrost_emergency_cnt[cm] > k)  // ������,�ſ�ʼ��ʱ
                { 
                    _CNT(defrost_cnt_emergency_intv[cm][k]);
                }
            }

            _CNT(defrost_gas_in_dly[cm]);
        }
    }
} 

/*****************************************************************************
��  �� : defrost_frost_by_env_wing_rise()
��  �� : ���������˪
��  �� : U08 cm : 
��  �� : ��
��  �� : ��
*****************************************************************************/
U08 defrost_frost_by_env_wing_rise(U08 cm)
{
    U08 by_env_wing_rise = DEFROST_STA_NULL;    // ��ʼ��Ϊ��Ч
    
    // �ж��Ƿ������˪�������
    if (   val_defrost_env_wing_rise_en                         /* ���������˪ʹ�� */
        && SensorCanUse(defrost_temp_EWD_nofrost[cm]) /* �ȶ�������¶� */
        // û�г�˪���жϣ������������Ϊ�˷�ֹ�����ضȳ�˪ʱ����˪����Ǵ���DEFROST_REC_EWD_END_TIME������˪�����ǻᱻ�������
		&& !S_CM_GetStatus(cm, CM_DEFROST)
	) 
    {
        by_env_wing_rise = DEFROST_STA_NO_FROST;
        
        // �ж��Ƿ���Ҫ��˪

        //4 ��ǰ���л��������¶�-��Ƭ�¶ȣ�-"��˪���л����"��+4�桾��������1���������¶�ȷ��ʱ�䡿���ۼƻ�˪����ʱ��ݳ�˪���
        if (defrost_cnt_env_wing_rise_diff_1[cm] > val_defrost_temp_sure_time
            && defrost_cnt_frost[cm] > defrost_fun_get_interval(cm)
            &&(defrost_cnt_frost[cm] > val_defrost_min_time))  /* ��˪ʱ�䣾����С��˪�����  */
        {
            by_env_wing_rise = DEFROST_STA_BY_ENV_WING;
        }
        
        //4 ��ǰ���л��������¶�-��Ƭ�¶ȣ�-"��˪���л����"��+6�桾��������2���������¶�ȷ��ʱ�䡿
        if (defrost_cnt_env_wing_rise_diff_2[cm] > val_defrost_temp_sure_time
		    &&(defrost_cnt_frost[cm] > val_defrost_min_time)
		)
        {
            by_env_wing_rise = DEFROST_STA_BY_ENV_WING;
        }
    }

	/* ʵ�ʳ�˪���С�ڱ궨"��˪�����"����ʱ������Ϊ�궨ƫС,��ձ궨ֵ,��Ϊ����̬ */
	if (by_env_wing_rise == DEFROST_STA_BY_ENV_WING && defrost_cnt_frost[cm]<=DEFROST_REC_EWD_END_TIME)
	{
	    defrost_temp_EWD_nofrost[cm] = SENSOR_ERROR;
	}
    
    // ���ػ�˪״̬
    return by_env_wing_rise;
}

/*****************************************************************************
��  �� : defrost_frost_by_env_wing_diff()
��  �� : ������˪
��  �� : U08 cm : 
��  �� : ��
��  �� : ��
*****************************************************************************/
U08 defrost_frost_by_env_wing_diff(U08 cm)
{
    U08 by_EWD = DEFROST_STA_NULL;
    I16 temp_wing = defrost_fun_get_wing(cm);
    I16 temp_env = defrost_fun_get_env();
    I16 temp_EWD = temp_env - temp_wing;
    I16 defrost_EWD = val_defrost_allow_diff1;
    
    // �ж��Ƿ������˪�������
    if (TRUE)
    {
        static U08 env_area = AREA_1;
        env_area = Fun_LookupArea(temp_env, env_area, val_defrost_environ1,
                                                      val_defrost_environ2,
                                                      val_defrost_environ3,
                                                      val_defrost_environ4, 10);
        // �����˪�����
        switch(env_area)
        {
            default:        
            case AREA_1: defrost_EWD = val_defrost_allow_diff1; break;
            case AREA_2: defrost_EWD = val_defrost_allow_diff2; break;
            case AREA_3: defrost_EWD = val_defrost_allow_diff3; break;
            case AREA_4: defrost_EWD = val_defrost_allow_diff4; break;
            case AREA_5: defrost_EWD = val_defrost_allow_diff5; break;
        }
        
        // �ж��Ƿ���Ҫ��˪

        if (   temp_EWD >= AT_VAL(defrost_EWD, val_ability_test_EWD)        // �����¶ȣ���Ƭ�¶ȡݳ�˪�����
            && defrost_cnt_frost[cm] > defrost_fun_get_interval(cm) // ��˪���ڵ�
            && (defrost_cnt_frost[cm] > val_defrost_min_time)  /* ��˪ʱ�䣾����С��˪�����  */
           )
        {
            by_EWD = DEFROST_STA_BY_WING;
        }
    }
    
    // ���ػ�˪״̬
    return by_EWD;
}

/*****************************************************************************
��  �� : defrost_by_frost(
��  �� : ������˪��˪
��  �� : U08 cm : 
��  �� : ��
��  �� : ��
*****************************************************************************/
U08 defrost_by_frost(U08 cm)
{
    U08 by_frost = DEFROST_STA_NO_FROST;
    I16 temp_wing = defrost_fun_get_wing(cm);
    I16 temp_out = defrost_fun_get_out_temp(cm);
    I16 temp_env = defrost_fun_get_env();

    // �ж��Ƿ���Ҫ��˪
    if (  (cm_time[cm].on > val_defrost_cm_on_time)  /* ѹ������ʱ��>��˪����ʱ�� */
        &&(temp_out > val_defrost_enter_out)              
        &&(temp_wing <= val_defrost_allow_wing)         /* ��Ƭ�¶�<=�ɳ�˪���� */
        &&(temp_env <= val_defrost_allow_environ)       /* �����¶�<=�ɳ�˪���� */
        )
    {
        U08 by_humidity = DEFROST_STA_NULL;
        U08 by_env_wing_rise = DEFROST_STA_NULL;
        U08 by_env_wing_diff = DEFROST_STA_NULL;

        //4 ÿ�ֻ�˪��ִ��,���ݽ��ѡ��
//      // ʪ�Ȼ�˪
//      by_humidity = defrost_frost_by_humidity(cm);
        
        // ���������˪
        by_env_wing_rise = defrost_frost_by_env_wing_rise(cm);
        
        // ������˪
        by_env_wing_diff = defrost_frost_by_env_wing_diff(cm);

        // Ĭ��ʪ�Ȼ�˪
        by_frost = by_humidity;
        // ʪ�Ȼ�˪������ʱ,���������˪
        if (by_frost == DEFROST_STA_NULL)
        {
            by_frost = by_env_wing_rise;
        }
        // ���������˪������ʱ,ʹ�û�����˪
        if (by_frost == DEFROST_STA_NULL)
        {
            by_frost = by_env_wing_diff;
        }

        if (ABILITY_TEST_HAS_DEFR)
        {
            by_frost = by_env_wing_diff;
        }
    }


    // ���س�˪���
    return by_frost;
}

/*****************************************************************************
��  �� : defrost_by_lowpress()
��  �� : ��ѹ��˪
��  �� : U08 cm : 
��  �� : ���س�˪״̬
��  �� : ��
*****************************************************************************/
U08 defrost_by_lowpress(U08 cm)
{
    U08 by_lowpress = DEFROST_STA_NO_FROST;
    I16 temp_wing = defrost_fun_get_wing(cm);
    I16 temp_out = defrost_fun_get_out_temp(cm);
    I16 temp_env = defrost_fun_get_env();
    BOL allow_lp_into = FALSE;

	#define LOWPRESS_DEFROST_ALLOW_ENVIR		(-5*10)
    
    // �ж��Ƿ������ѹ��˪����
    if (  (val_defrost_LP == LOG_USED)              /*  */
		&&(defrost_cnt_frost[cm] > val_defrost_min_time)/* ��˪ʱ�䣾����С��˪�����  */
        &&(cm_time[cm].on >= val_defrost_lp_dly) /*  */
        &&(defrost_cnt_frost[cm] > val_defrost_lp_time)    /*  */
        &&(temp_wing < val_defrost_lp_wing)         /*  */
        &&(temp_out > val_defrost_enter_out)
        &&(temp_env <= LOWPRESS_DEFROST_ALLOW_ENVIR)
        )
    {
        allow_lp_into = TRUE;
    }
    
    // �����ѹ�ź���
    if (SIGNAL_Accept(&defrost_signal_into_lowpress[cm], allow_lp_into))
    {
        by_lowpress = DEFROST_STA_BY_LOWPRESS;
    }


    // ���س�˪���
    return by_lowpress;
}

/*****************************************************************************
��  �� : defrost_by_emergency()
��  �� : ������˪
��  �� : U08 cm : 
��  �� : ���س�˪״̬
��  �� : ��
*****************************************************************************/
U08 defrost_by_emergency(U08 cm)
{
    U08 by_emergency = DEFROST_STA_NO_FROST;
    I16 temp_wing = defrost_fun_get_wing(cm);
    I16 temp_out = defrost_fun_get_out_temp(cm);
    I16 temp_env = defrost_fun_get_env();
    I16 temp_EWD = fun_pv_sub(temp_env, temp_wing);

    if (temp_EWD <= val_defrost_emergency_diff)
	{
	    defrost_emergency_dly[cm] = 0;
	}
    
    // �ж��Ƿ����������˪����
    if (  (cm_time[cm].on > val_defrost_cm_on_time)  /* ѹ������ʱ��>��˪����ʱ�� */
        &&(defrost_cnt_frost[cm] > val_defrost_min_time)/* ��˪ʱ�䣾����С��˪�����  */
        &&(defrost_cnt_frost[cm] > val_defrost_emergency_time)/* ��˪ʱ�䣾��������˪�����  */
        &&(temp_out > val_defrost_enter_out)   
        &&(temp_wing <= val_defrost_allow_wing)         /* ��Ƭ�¶�<=�ɳ�˪���� */
        )
    {
        /* ���������¶ȣ���Ƭ�¶ȣ���18�桾ǿ��˪���� */
		
        if (defrost_emergency_dly[cm] > val_defrost_env_wing_dly)
        {
            BOL allow = TRUE;
            if (defrost_emergency_cnt[cm] >= DEFROST_EMERGENCY_MAX_TIMES // ��3�η���
                && !S_CM_GetStatus(cm, CM_DEFROST)) // ÿ��˪ʱ���ж�,Ӧ�Դ����˪���
            {
                // ��k�ַ������
                U16 k = defrost_emergency_cnt[cm]%DEFROST_EMERGENCY_MAX_TIMES;
                if (defrost_cnt_emergency_intv[cm][k] <= DEFROST_EMERGENCY_TIME)
                {
                    allow = FALSE;
                }
            }

            if (allow)
            {
                by_emergency = DEFROST_STA_BY_EMERGENCY;
            }
            else
            {   /* ������˪Ƶ�� */
                defrost_fg[cm].emergency_defrost_frequent = TRUE; /* ����ع��� */
				defrost_emergency_cnt[cm] = 0;/* ÿ���α�һ�ξ���������˪�������� */
			
            }
        }
    }


    // ���س�˪���
    return by_emergency;
}

/*******************************************************************************
 �� ��: defrost_cal_power()
 �� ��: ����ˮ�²�����������
 �� ��: water_diff--ˮ�²�; 
 �� ��: ��������
 �� ��: ��
*******************************************************************************/
I32 defrost_cal_power(I32 water_diff)
{
    I32 result = 0;
    I16 flow = val_ability_test_flow;

    if (SensorCanUse(PUMPf_flow_pv)
    &&  USE_PUMPf_CTRL != PUMPF_CTRL_PWR_ON_ONLY
    )
    {
        flow = _DIVUROUND(PUMPf_flow_pv, 100);			/* 1.0kL/h */
    }

    result = _DIVUROUND(water_diff*flow*4186, 3600);    /* 0.01kw */
    
    return result;
}

/*****************************************************************************
��  �� : defrost_by_power()
��  �� : ����˥����˪
��  �� : U08 cm : 
��  �� : ���س�˪״̬
��  �� : ��
*****************************************************************************/
U08 defrost_by_power(U08 cm)
{
    U08 by_power = DEFROST_STA_NO_FROST;
    I16 temp_wing = defrost_fun_get_wing(cm);
    I16 temp_out = defrost_fun_get_out_temp(cm);
    I16 power_diff = fun_pv_sub(pv_cell_mac_power(pcb_addr), pv_cell_mac_power_now(pcb_addr));

    if (val_ability_test_power == 0
    ||  power_diff <= val_ability_test_power)
	{
	    defrost_power_dly[cm] = 0;
	}
    
    // �ж��Ƿ����������˪����
    if (  (cm_time[cm].on > val_defrost_cm_on_time)  /* ѹ������ʱ��>��˪����ʱ�� */
//        &&(defrost_cnt_frost[cm] > val_defrost_cm_on_time)/* ��˪ʱ�䣾����˪����ʱ�䡿  */
        &&(temp_out > val_defrost_enter_out)   
        &&(temp_wing <= val_defrost_allow_wing)         /* ��Ƭ�¶�<=�ɳ�˪���� */
        && ABILITY_TEST_HAS_DEFR
        )
    {
        if (defrost_power_dly[cm] > 60)
        {
            by_power = DEFROST_STA_BY_POWER;
        }
    }


    // ���س�˪���
    return by_power;
}

/*******************************************************************************
 �� ��: defrost_by_longtime()
 �� ��: ��ʱ��˪�������µ�ʱ���ҳ�ʱ��δ��˪��ֱ��ִ�г�˪
 �� ��: cm--ѹ�����; 
 �� ��: �Ƿ�����˪
 �� ��: ��
*******************************************************************************/
U08 defrost_by_longtime(U08 cm)
{ 
    I16 temp_wing = defrost_fun_get_wing(cm);
    I16 temp_gas_in = defrost_fun_get_gas_in(cm);
    I16 temp_allow = fun_pv_min(temp_wing, temp_gas_in);
    I16 temp_out = defrost_fun_get_out_temp(cm);
    U08 allow_longtime_into = DEFROST_STA_NO_FROST; 
    U16 defrost_longtime;	/* ��ʱ��˪��� */

#if defined(M_DISEN)
	if (pv_cell_TWin1(pcb_addr) < val_timingdfr_temp_in1)
	{
	    defrost_longtime = val_defrost_longtime1;
    }
	else if (pv_cell_TWin1(pcb_addr) < val_timingdfr_temp_in2)
	{
	    defrost_longtime = val_defrost_longtime2;
    }
	else
	{
	    defrost_longtime = val_defrost_longtime3;
    }
#else
    defrost_longtime = val_defrost_longtime;
#endif

	if (defrost_longtime == 0   // ���Ϊ0��ʾ����
    ||  fg_ability_test_run				/* ���ܲ��Բ����� */
    )
	{
		allow_longtime_into = DEFROST_STA_NO_FROST;
	}
	else
	{
		// �ͻ����ҳ�ʱ��δ��˪
	    if ((defrost_cnt_frost[cm] >= defrost_longtime)   
	    &&  (defrost_cnt_frost[cm] > val_defrost_min_time) /* ��˪ʱ�䣾����С��˪�����  */
	    &&	(temp_allow < val_defrost_allow_wing)
	    &&  (temp_out > val_defrost_enter_out)
	    )
	    {
	        allow_longtime_into = DEFROST_STA_BY_LONGTIME;
	    }
	} 
	
    // ���س�˪���
    return allow_longtime_into;
}

#if 0
/*******************************************************************************
 �� ��: defrost_by_gas_in()
 �� ��: �����¶ȳ�˪
 �� ��: cm--; 
 �� ��: 
 �� ��: ��ֹ��Ƭ�¶��쳣�����
*******************************************************************************/
U08 defrost_by_gas_in(U08 cm)
{ 
    U08 by_gas_in = DEFROST_STA_NO_FROST;
    I16 temp_out = defrost_fun_get_out_temp(cm);
    I16 temp_env = defrost_fun_get_env();
    I16 temp_gas_in = defrost_fun_get_gas_in(cm);
    I16 temp_EGD = fun_pv_sub(temp_env, temp_gas_in);

#ifndef USE_QUICK_TIME
    #define val_defrost_allow_gas_in        (-10*10)		/* �����˪�����¶� */
    #define val_defrost_allow_EGD		    (25*10)		    /* �����˪������ */
    #define val_defrost_by_gas_in_dly	    (30)		    /* ȷ����ʱ��min */
#else
    #define val_defrost_allow_gas_in        (-10*10)		/* �����˪�����¶� */
    #define val_defrost_allow_EGD		    (25*10)		    /* �����˪������ */
    #define val_defrost_by_gas_in_dly	    (2)		        /* ȷ����ʱ��min */
#endif

	if (temp_EGD < val_defrost_allow_EGD
    ||  temp_gas_in >= val_defrost_allow_gas_in
        )
	{
	    defrost_gas_in_dly[cm] = 0;
	}

	// �ж��Ƿ����������¶ȳ�˪����
    if (  (cm_time[cm].on > val_defrost_cm_on_time)  /* ѹ������ʱ��>��˪����ʱ�� */
        &&(defrost_cnt_frost[cm] > val_defrost_cm_on_time)/* ��˪ʱ�䣾����˪����ʱ�䡿  */
        &&(temp_out > val_defrost_enter_out)              
        )
    {
        
        if (defrost_gas_in_dly[cm] > val_defrost_by_gas_in_dly)
        {
            by_gas_in = DEFROST_STA_BY_GAS_IN;
        }
    }
	
    // ���س�˪���
    return by_gas_in;
}
#endif

/*****************************************************************************
��  �� : defrost_by_manual()
��  �� : �ֶ���˪
��  �� : U08 cm : 
��  �� : ���س�˪״̬
��  �� : ��
*****************************************************************************/
U08 defrost_by_manual(U08 cm)
{
    U08 by_manual = DEFROST_STA_NO_FROST;
    I16 temp_wing = defrost_fun_get_wing(cm);
    I16 temp_out = defrost_fun_get_out_temp(cm);
    BOL allow_manual = FALSE;
    
    // �ж��Ƿ������ֶ���˪����
    if (temp_wing < AT_VAL(val_defrost_end_wing, val_ability_test_wing))
    {
        // �ж��ֶ���˪����
        allow_manual = TRUE;
    }

    // �����ֶ���˪����,������
    if (SIGNAL_Accept(&defrost_signal_into_namual[cm], allow_manual))
    {
        S_CM_SetStatus(cm, CM_FORCE_DEFROST);
    }

    // �ﵽ�����Ž����ֶ���˪
    if (S_CM_GetStatus(cm, CM_FORCE_DEFROST)
        &&(temp_out > val_defrost_enter_out)
        )
    {
        by_manual = DEFROST_STA_BY_MANUAL;
    }

    // ���س�˪���
    return by_manual;
}

/*****************************************************************************
��  �� : defrost_grp_into_apply()
��  �� : �����˪,�������ѹ��
��  �� : U08 cm_bgn    : ����ʼѹ����
         U08 cm_end    : �����ѹ����
         U08 cm        : ��ǰѹ����
         BOL into_need : �Ƿ���Ҫ�����˪
��  �� : ��
��  �� : ��
*****************************************************************************/
void defrost_grp_into_apply(U08 cm_bgn, U08 cm_end, U08 cm, BOL into_need)
{
    U08 i;
    BOL fg_wait = FALSE;
    
    if ((!S_CM_GetStatus(cm, CM_NEED_DEFROST)) 
        && (!S_CM_GetStatus(cm, CM_DEFROST))
        && into_need
        )
    {
        //3 �ж��Ƿ���Ҫ�ȴ�����ѹ��
        for(i=cm_bgn; i<cm_end; i++)
        {
            if (i == cm)        continue;
                
            if (S_CM_GetStatus(i, CM_NEED_DEFROST)                  /* ͬ������ѹ���г�˪���� */
                 || S_CM_GetStatus(i, CM_ON) && cm_time[i].on < val_cm_on_time)     /* ����������δ���趨ʱ�� */
            {
                fg_wait = TRUE;
                break;
            }
        }
    

        //3 ���õȴ�����ѹ��ʱ,�����˪
        if (!fg_wait)
        {
            S_CM_SetStatus(cm, CM_NEED_DEFROST);
            S_CM_ClrStatus(cm, CM_FORCE_DEFROST);
            defrost_fg[cm].first_defrost = TRUE;
            
            defrost_fg[cm].defrost_apply = TRUE;
                
            for (i=cm_bgn; i<cm_end; i++)
            {
                I16 temp_wing = defrost_fun_get_wing(i);
                I16 temp_out = defrost_fun_get_out_temp(i);
                I16 temp_gas_in = defrost_fun_get_gas_in(i);
                I16 temp_allow = fun_pv_min(temp_wing, temp_gas_in);
                
                if (i == cm)        continue;

                //4 �����������˪��ѹ��
                if(S_CM_GetStatus(i, CM_ON))  /*  */
                {
                   if ((temp_allow < val_defrost_allow_wing   			/* ��Ƭ�������¶������˪��([�״γ�˪]��[���£��˳�˪����-2��]) */  
                        ||( val_defrost_env_wing_rise_en
                            && defrost_fg[i].first_defrost == FALSE 
                            && temp_wing < AT_VAL(val_defrost_end_wing, val_ability_test_wing) - 20))
                      && SensorCanUse(temp_wing)            			/* ��Ƭ�¶�̽ͷ�޹��� */
                      && temp_out > val_defrost_enter_out)    			
                   {
                        S_CM_SetStatus(i, CM_NEED_DEFROST);
                        S_CM_ClrStatus(i, CM_FORCE_DEFROST);
                        defrost_fg[i].first_defrost = TRUE;
                        defrost_fg[i].defrost_apply = TRUE;
                   }
                }
                else         /* ��ֹѹ����ѡ�ϵ��ǻ�δ��������� */
                {
                    S_CM_ClrStatus(i, CM_NEED);
                }
            }
        }
    }
}
/*****************************************************************************
��  �� : defrost_grp_into_act()
��  �� : һ̨ѹ�������˪ʱ�Ķ���,���������˪
��  �� : U08 cm_bgn    : ����ʼѹ����
         U08 cm_end    : �����ѹ����
         U08 cm        : ��ǰѹ����
         BOL into_must : �Ƿ�������
��  �� : ��
��  �� : S_CM_SetStatus(cm, CM_DEFROST);
*****************************************************************************/
void defrost_grp_into_act(U08 cm_bgn, U08 cm_end, U08 cm, BOL into_must)
{
    U08 i;

    if (into_must)
    {
        S_CM_SetStatus(cm, CM_NEED_DEFROST);
        S_CM_SetStatus(cm, CM_DEFROST_ALLOW);
    }
    
    //3 �г�˪����,�������˪ʱ,�����˪
    if (S_CM_GetStatus(cm, CM_NEED_DEFROST) && S_CM_GetStatus(cm, CM_DEFROST_ALLOW))
    {
        S_CM_SetStatus(cm, CM_DEFROST);


        //4 �����޳�˪�����,�ػ�
        for (i=cm_bgn; i<cm_end; i++)
        {
            if (i == cm)        continue;
                
            if (S_CM_GetStatus(i, CM_NEED_DEFROST))  /* ����ѹ���Ĵ��� */
            {
                S_CM_SetStatus(i, CM_DEFROST);
                S_CM_SetStatus(i, CM_DEFROST_ALLOW);
            }
            else 
            {
                S_CM_ClrStatus(i, CM_NEED);
            }
        }
    }
}

/*****************************************************************************
��  �� : defrost_grp_into()
��  �� : һ�鹲�÷����ѹ��ѹ�������˪�����ж�
��  �� : U08 cm_bgn    : ����ʼѹ����
         U08 cm_end    : �����ѹ����
         U08 cm        : ��ǰѹ����
��  �� : ��
��  �� : ��
*****************************************************************************/
void defrost_grp_into(U08 cm_bgn, U08 cm_end, U08 cm)
{
    U08 into_by_frost = DEFROST_STA_NO_FROST;
    U08 into_by_lowpress = DEFROST_STA_NO_FROST;
    U08 into_by_emergency = DEFROST_STA_NO_FROST;
    U08 into_by_manual = DEFROST_STA_NO_FROST;
	U08 into_by_longtime = DEFROST_STA_NO_FROST;
    U08 into_by_gas_in = DEFROST_STA_NO_FROST;
    U08 into_by_power = DEFROST_STA_NO_FROST;
    BOL into_need = FALSE;
    BOL into_must = FALSE;
//  static BOL is_defrost[MAX_COMP] = {COMP_INIT(FALSE)};
    BOL is_defrost_bgn = FALSE;
    BOL is_defrost_end = FALSE;

    is_defrost_bgn = (S_CM_GetStatus(cm, CM_DEFROST) != 0);
    
    // ����ÿ����˪����,һ��Ҫ����
    into_by_frost = defrost_by_frost(cm);
    into_by_lowpress = defrost_by_lowpress(cm);
    into_by_emergency = defrost_by_emergency(cm);
    into_by_manual = defrost_by_manual(cm);
	into_by_longtime = defrost_by_longtime(cm);
	into_by_power = defrost_by_power(cm);
//    into_by_gas_in = defrost_by_gas_in(cm);

    
    // �ٲó�˪���
    if (defrost_is_allow_auto())			/* �򡰴���ÿ����˪����,һ��Ҫ�������ʷŵ�������ж� */
    {
        if (  DEFROST_STA_IS_NEED(into_by_frost)
            ||DEFROST_STA_IS_NEED(into_by_lowpress)
            ||DEFROST_STA_IS_NEED(into_by_emergency)
            ||DEFROST_STA_IS_NEED(into_by_manual)
            ||DEFROST_STA_IS_NEED(into_by_longtime)
            ||DEFROST_STA_IS_NEED(into_by_gas_in)
            ||DEFROST_STA_IS_NEED(into_by_power)
            )
        {
            into_need = TRUE;
        }
    }
	else
	{
        if (DEFROST_STA_IS_NEED(into_by_manual)
        ||  DEFROST_STA_IS_NEED(into_by_power)
           )
        {
            into_need = TRUE;
        }		
	}

    if (DEFROST_STA_IS_NEED(into_by_lowpress))
    {
        into_must = TRUE;
    }

    // �����˪,��������˪
    defrost_grp_into_apply(cm_bgn, cm_end, cm, into_need);
    
    // �����˪,��������˪
    defrost_grp_into_act(cm_bgn, cm_end, cm, into_must);

    // ������˪ʱ��,�������첽��,����˪����һ��Ҫ����
    is_defrost_end = (S_CM_GetStatus(cm, CM_DEFROST) != 0);
    if (!is_defrost_bgn && is_defrost_end)
    {
        if (DEFROST_STA_IS_NEED(into_by_emergency))
        {
            // ��k�ַ���������¼�ʱ
            U16 k = defrost_emergency_cnt[cm]%DEFROST_EMERGENCY_MAX_TIMES;
            defrost_cnt_emergency_intv[cm][k] = 0;
            // ǰһ��ѭ�����״η���,�������ٴη���
            if (++defrost_emergency_cnt[cm] >= 2*DEFROST_EMERGENCY_MAX_TIMES) 
                defrost_emergency_cnt[cm] = DEFROST_EMERGENCY_MAX_TIMES;
        }
    }
}

/*****************************************************************************
��  �� : defrost_grp_exit_act()
��  �� : һ̨ѹ���˳���˪ʱ�Ķ���,���������˳�
��  �� : U08 cm_bgn    : ����ʼѹ����
         U08 cm_end    : �����ѹ����
         U08 cm        : ��ǰѹ����
         U08 attrib    : �˳�����
��  �� : ��
��  �� : ��
*****************************************************************************/
void defrost_grp_exit_act(U08 cm_bgn, U08 cm_end, U08 cm, U08 attrib)
{
    U08 i;

    // ���ݲ�ͬ���˳������˳���˪    
    if (attrib == DEFRO_EXIT_SIN)
    {   
        S_CM_ClrStatus(cm, CM_NEED_DEFROST);
        S_CM_ClrStatus(cm, CM_FORCE_DEFROST);
        defrost_fg[cm].defrost_apply = FALSE;
        // �����˳���˪����"����"��"ͣ��" 
        S_CM_ClrStatus(cm, CM_NEED);
		defrost_fg[cm].defrost_end = TRUE;      /* �ѳ���˪ */
        defrost_fg[cm].need_calib_EWD = TRUE;
		defrost_fg[cm].exit_defr_cm_off = TRUE;
    }
    else 
    {
        for (i=cm_bgn; i<cm_end; i++)
        {
            S_CM_ClrStatus(i, CM_NEED_DEFROST);
            S_CM_ClrStatus(i, CM_FORCE_DEFROST);
            defrost_fg[i].defrost_apply = FALSE;
			defrost_fg[i].defrost_end = TRUE;      /* �ѳ���˪ */
            defrost_fg[i].need_calib_EWD = TRUE;
        }
    }
    
}

#if defined(M_DISEN)
#define val_defrost_end_wing_acin               (pv_cell_TWin1(pcb_addr)<= 250 ? 180 \
                                                                               : (pv_cell_TWin1(pcb_addr) <= 350 ? 150 \
                                                                                                                 : 120))
/*****************************************************************************
��  �� : defrost_grp_exit()
��  �� : һ�鹲�÷����ѹ��ѹ���˳���˪�����ж�
��  �� : U08 cm_bgn    : ����ʼѹ����
         U08 cm_end    : �����ѹ����
         U08 cm        : ��ǰѹ����
��  �� : ��
��  �� : S_CM_ClrStatus(cm, CM_NEED_DEFROST);
*****************************************************************************/
void defrost_grp_exit(U08 cm_bgn, U08 cm_end, U08 cm)
{
    U08 i;
    U08 attr = 0xFF;
    BOL grp_exit_frost_time = FALSE;    // ��˪ʱ�䵽,����
    BOL grp_exit_out_temp = FALSE;      // ��ˮ�¶ȵ�,����
    BOL grp_exit_wing_temp = FALSE;     // ���г�˪����ĳ�Ƭ�¶ȸ�,����
    BOL grp_exit_ext_temp = FALSE;      // ���г�˪����������¶ȸ�,����
    BOL grp_exit_wing_temp_cnt = FALSE;     // ���г�˪����ĳ�Ƭ�¶ȴ���3�泬��150s,����
    BOL has_defrost_cm = FALSE;            // �г�˪����Ļ���
    BOL evp_independ = (val_evap_type < log_mod_cond_type(val_comp_num)); // ����������??
    I16 temp_out = defrost_fun_get_out_temp(cm);
    
    // �ж��˳���˪����
    if (S_CM_GetStatus(cm, CM_NEED_DEFROST))
    {
        //3 �ж��Ƿ�ͬ���˳���˪
        // ��˪ʱ�䵽
        if (defrost_cnt_defrost[cm] >= AT_VAL(val_defrost_time, val_ability_test_defr_time))  
        {
            grp_exit_frost_time = TRUE;
        }
        // �κγ�ˮ�¶ȵ�
        if (defrost_cnt_out_temp_low[cm] > 10 
        &&  temp_out<=val_defrost_end_out	/* ��ֹ��ˮ�¶��������̳������� */
        &&  !evp_independ)
        {
            grp_exit_out_temp = TRUE;
        }
        // ���г�˪����ĵĳ�Ƭ�¶ȸ�/��������/��������
        for (i=cm_bgn, grp_exit_wing_temp=TRUE, grp_exit_ext_temp=TRUE, grp_exit_wing_temp_cnt=TRUE; i<cm_end; i++) // ��ʼ��Ϊ�˳�
        {
            if (S_CM_GetStatus(i, CM_NEED_DEFROST))
            {
                has_defrost_cm = TRUE;
                // ��Ƭ��
                if (defrost_fun_get_wing(i) < AT_VAL(val_defrost_end_wing_acin, val_ability_test_wing))
                {
                    grp_exit_wing_temp = FALSE;
                }
                // ������
                if (pv_cell_gas_out(i) < 950 || defrost_cnt_defrost[i] < 120)
                {
                    grp_exit_ext_temp = FALSE;
                }	
				// ���µ����ʱ
		        if (defrost_cnt_wing_temp_high[i] <= val_exit_dfr_wingT2_time 
		        ||  defrost_fun_get_wing(i) < val_exit_dfr_wingT2	
				)
		        {
		            grp_exit_wing_temp_cnt = FALSE;
		        }
            }
        }
        grp_exit_wing_temp = (grp_exit_wing_temp && has_defrost_cm);
		grp_exit_ext_temp = (grp_exit_ext_temp && has_defrost_cm);
//		grp_exit_wing_temp_cnt = (grp_exit_wing_temp_cnt && has_defrost_cm);
        grp_exit_wing_temp_cnt = 0;			/* 2022-11-26��ɭҪ��ɾ���������������㴦����ֹ�ͻ�������Ҫ��� */
		
        //4 �ٲ����˳�
        if (grp_exit_frost_time || grp_exit_out_temp || grp_exit_wing_temp || grp_exit_ext_temp || grp_exit_wing_temp_cnt)
        {
            attr = DEFRO_EXIT_ALL;
        }

        //3 �ж��Ƿ񵥸��˳�
        if (attr == 0xFF)
        {
            BOL signal_exit = FALSE;
            BOL other_defrost = FALSE;
            // ��ˮ�¶ȵ� �� ��Ƭ�¶ȸ�
            if (  (defrost_cnt_out_temp_low[cm] > 10 && evp_independ)
                ||(defrost_fun_get_wing(cm) >= AT_VAL(val_defrost_end_wing_acin, val_ability_test_wing))
                )
            {
                signal_exit = TRUE;
            }

			// ���µ����ʱ
		    if (defrost_cnt_wing_temp_high[i] > val_exit_dfr_wingT2_time
		    &&  defrost_fun_get_wing(i) > val_exit_dfr_wingT2	
			)
		    {
		        signal_exit = TRUE;
		    }	

            // �����¶ȸ�
            if (pv_cell_gas_out(cm) >= 950 && defrost_cnt_defrost[cm] >= 120)
            {
                signal_exit = TRUE;
            }

            // ������ѹ���ڳ�˪,��ͣ��
            if (signal_exit)
            {
                for (i=cm_bgn; i<cm_end; i++)
                {
                    if (i == cm)    continue;
                    
                    if (S_CM_GetStatus(i, CM_DEFROST) || S_CM_GetStatus(i, CM_NEED_DEFROST))
                    {
                        other_defrost = TRUE;
                        break;
                    }
                }

                if (other_defrost)  attr = DEFRO_EXIT_SIN;
                else                attr = DEFRO_EXIT_ALL;
            }
        }
    }
    
    // �����˳���˪�ź���
    {
        BOL allow_exit = FALSE;
		BOL fg_accept = FALSE;				/* �źŽ��ձ�־ */
        if (S_CM_GetStatus(cm, CM_DEFROST))
        {
            allow_exit = TRUE;
        }

		/* ����"||"����Ϊ�źŶ�Ҫ������ */
        if (SIGNAL_Accept(&defrost_signal_exit_highpress[cm], allow_exit))
        {
            fg_accept = TRUE;
            /* ���� */
            {
                /* 2020-10-27���鹵ͨ�����ع���ʱ��ֱ��ͣ��ѹ���� */
                S_CM_ClrStatus(cm, CM_NEED);
                S_CM_ClrStatus(cm, CM_ON);	// ��ѹ��
                s_clr_output(cm, SW_CM);
                mod_err_comp_risk[cm] = 1;
            }
        }
		if (SIGNAL_Accept(&defrost_signal_exit_exh[cm], allow_exit))
        {
            fg_accept = TRUE;
        }
		if (SIGNAL_Accept(&defrost_signal_exit_cur[cm], allow_exit))
        {
            fg_accept = TRUE;
        }

		if (fg_accept)
		{
		    attr = DEFRO_EXIT_SIN;
		}
    }

    
    // �˳���˪����
    if (attr != 0xFF)
    {
        defrost_grp_exit_act(cm_bgn, cm_end, cm, attr);
    }
    
}
#else
/*****************************************************************************
��  �� : defrost_grp_exit()
��  �� : һ�鹲�÷����ѹ��ѹ���˳���˪�����ж�
��  �� : U08 cm_bgn    : ����ʼѹ����
         U08 cm_end    : �����ѹ����
         U08 cm        : ��ǰѹ����
��  �� : ��
��  �� : S_CM_ClrStatus(cm, CM_NEED_DEFROST);
*****************************************************************************/
void defrost_grp_exit(U08 cm_bgn, U08 cm_end, U08 cm)
{
    U08 i;
    U08 attr = 0xFF;
    BOL grp_exit_frost_time = FALSE;    // ��˪ʱ�䵽,����
    BOL grp_exit_out_temp = FALSE;      // ��ˮ�¶ȵ�,����
    BOL grp_exit_wing_temp = FALSE;     // ���г�˪����ĳ�Ƭ�¶ȸ�,����
    BOL has_defrost_cm = FALSE;            // �г�˪����Ļ���
    BOL evp_independ = (val_evap_type < log_mod_cond_type(val_comp_num)); // ����������??
    I16 temp_out = defrost_fun_get_out_temp(cm);
    
    // �ж��˳���˪����
    if (S_CM_GetStatus(cm, CM_NEED_DEFROST))
    {
        //3 �ж��Ƿ�ͬ���˳���˪
        // ��˪ʱ�䵽
        if (defrost_cnt_defrost[cm] >= AT_VAL(val_defrost_time, val_ability_test_defr_time))  
        {
            grp_exit_frost_time = TRUE;
        }
        // �κγ�ˮ�¶ȵ�
        if (defrost_cnt_out_temp_low[cm] > 10 
        &&  temp_out<=val_defrost_end_out	/* ��ֹ��ˮ�¶��������̳������� */
        &&  !evp_independ)
        {
            grp_exit_out_temp = TRUE;
        }
        // ���г�˪����ĵĳ�Ƭ�¶ȸ�
        for (i=cm_bgn, grp_exit_wing_temp=TRUE; i<cm_end; i++) // ��ʼ��Ϊ�˳�
        {
            if (S_CM_GetStatus(i, CM_NEED_DEFROST))
            {
                has_defrost_cm = TRUE;
                if (defrost_fun_get_wing(i) < AT_VAL(val_defrost_end_wing, val_ability_test_wing))
                {
                    grp_exit_wing_temp = FALSE;
                }
            }
        }
        grp_exit_wing_temp = (grp_exit_wing_temp && has_defrost_cm);

        //4 �ٲ����˳�
        if (grp_exit_frost_time || grp_exit_out_temp || grp_exit_wing_temp)
        {
            attr = DEFRO_EXIT_ALL;
        }

        //3 �ж��Ƿ񵥸��˳�
        if (attr == 0xFF)
        {
            BOL signal_exit = FALSE;
            BOL other_defrost = FALSE;
            // ��ˮ�¶ȵ�
            if (  (defrost_cnt_out_temp_low[cm] > 10 && evp_independ)
                ||(defrost_fun_get_wing(cm) > AT_VAL(val_defrost_end_wing, val_ability_test_wing) + val_defrost_end_diff)
                )
            {
                signal_exit = TRUE;
            }

            // ������ѹ���ڳ�˪,��ͣ��
            if (signal_exit)
            {
                for (i=cm_bgn; i<cm_end; i++)
                {
                    if (i == cm)    continue;
                    
                    if (S_CM_GetStatus(i, CM_DEFROST) || S_CM_GetStatus(i, CM_NEED_DEFROST))
                    {
                        other_defrost = TRUE;
                        break;
                    }
                }

                if (other_defrost)  attr = DEFRO_EXIT_SIN;
                else                attr = DEFRO_EXIT_ALL;
            }
        }
    }
    
    // �����˳���˪�ź���
    {
        BOL allow_exit = FALSE;
		BOL fg_accept = FALSE;				/* �źŽ��ձ�־ */
        if (S_CM_GetStatus(cm, CM_DEFROST))
        {
            allow_exit = TRUE;
        }

		/* ����"||"����Ϊ�źŶ�Ҫ������ */
        if (SIGNAL_Accept(&defrost_signal_exit_highpress[cm], allow_exit))
        {
            fg_accept = TRUE;
            /* ���� */
            {
                /* 2020-10-27���鹵ͨ�����ع���ʱ��ֱ��ͣ��ѹ���� */
                S_CM_ClrStatus(cm, CM_NEED);
                S_CM_ClrStatus(cm, CM_ON);	// ��ѹ��
                s_clr_output(cm, SW_CM);
                mod_err_comp_risk[cm] = 1;
            }
        }
		if (SIGNAL_Accept(&defrost_signal_exit_exh[cm], allow_exit))
        {
            fg_accept = TRUE;
        }
		if (SIGNAL_Accept(&defrost_signal_exit_cur[cm], allow_exit))
        {
            fg_accept = TRUE;
        }

		if (fg_accept)
		{
		    attr = DEFRO_EXIT_SIN;
		}
    }

    
    // �˳���˪����
    if (attr != 0xFF)
    {
        defrost_grp_exit_act(cm_bgn, cm_end, cm, attr);
    }
    
}
#endif

/*****************************************************************************
��  �� : defrost_over()
��  �� : ��˪��������
��  �� : U08 cm : ��ǰѹ����
��  �� : ��
��  �� : ��
*****************************************************************************/
void defrost_over(U08 cm)
{
    if (!S_CM_GetStatus(cm, CM_NEED_DEFROST)
       && S_CM_GetStatus(cm, CM_DEFROST))   /* �˳�˪ǰ����� */
    {
//        if(cnt_time_fan_delay[cm] >= val_defrost_fan_dly)
        if(cm_time[cm].cnt_time_fan_delay >= val_defrost_fan_dly)
        {               
            S_CM_ClrStatus(cm, CM_DEFROST_ALLOW);   /* �˳���˪������ */ 
			if (defrost_cm_step[cm] == DEFROST_CM_NULL)
            {
                S_CM_ClrStatus(cm, CM_DEFROST);        
            }
        }
    }

    /* ���ڳ�˪,��״̬ */
    if (S_CM_GetStatus(cm, CM_NEED_DEFROST)
        && S_CM_GetStatus(cm, CM_DEFROST))
    {
        defrost_cnt_frost[cm] = 0;
//        cnt_time_fan_delay[cm] = 0;
        cm_time[cm].cnt_time_fan_delay = 0;
        defrost_fg[cm].defrost_apply = FALSE;
        defrost_fg[cm].defrost_end = FALSE;
    }
}

/*****************************************************************************
��  �� : defrost_comp_stop()
��  �� : ��˪ͣѹ����״̬������
________________                         __________________________________ ���
                ����                     ��        
                  ��______________...____��        

_______            _____________...________             ___________________ ѹ��
     ����         ��      ��               ��          ��      ��
       ��_________��      ��               ��__________��      ��
                                          
__________________________ �Ӻ��                ��ǰ��         ___________ ��ͨ��
          ��      ��      ��                           ��      ��
           ��ǰ�� ��      ��____...____________________��______���Ӻ�
             

��  �� : U08 cm : 
��  �� : ��
��  �� : ��
*****************************************************************************/
void defrost_comp_stop(U08 cm)
{
    cm = cm;
}
#if 0
{
    if (!USE_FIX_COMP) return;				/* ��Ƶѹ������ͣ���з� */
    
    for (cm=0; cm<val_comp_num; cm++)
    {
        switch(defrost_cm_step[cm])
        {
            // ��
            case DEFROST_CM_NULL:
                if (val_defrost_stop_cm_en
                    && S_CM_GetStatus(cm, CM_DEFROST)
                    && defrost_cm_intv_off >= 4
                    )
                {
					defrost_cm_intv_off = 0;
                    defrost_cm_time[cm] = 0;
                    defrost_cm_step[cm] = DEFROST_CM_STOP_BGN;
                }
                break;
            // ѹ��ͣ��,��ͨ��,����
            case DEFROST_CM_STOP_BGN:
                if (defrost_cm_time[cm] >= val_defrost_stop_cm_tm_entry
                    && defrost_cm_intv_on >= 4)
                {
                    defrost_cm_intv_on = 0;
                    defrost_cm_time[cm] = 0;
                    if (val_defrost_four_tm_entry >= 0) // ��ǰ
                    {
                        defrost_cm_step[cm] = DEFROST_CM_OFF_FORE_OFF;
                    }
                    else
                    {
                        defrost_cm_step[cm] = DEFROST_CM_ON_FORE_ON;
                    }
                }
                break;
            // ��ǰ����ͨ
            case DEFROST_CM_OFF_FORE_OFF:
            // �Ӻ����ͨ
            case DEFROST_CM_ON_FORE_ON:
                if (defrost_cm_time[cm] >= fun_absolute_value(val_defrost_four_tm_entry))
                {
                    defrost_cnt_defrost[cm] = 0; // ���¼�ʱ��˪ʱ��
                    defrost_cm_time[cm] = 0;
                    defrost_cm_step[cm] = DEFROST_CM_ON;
                }
                break;
            // ѹ������
            case DEFROST_CM_ON:
                if (!S_CM_GetStatus(cm, CM_NEED_DEFROST)
				 && defrost_cm_intv_off >= 4
					) // �˳�˪
                {
					defrost_cm_intv_off = 0;
                    defrost_cm_time[cm] = 0;
                    defrost_cm_step[cm] = DEFROST_CM_STOP_END;
                }
                break;
            // ѹ��ͣ��,��ͨ��,����
            case DEFROST_CM_STOP_END:
                if (defrost_cm_time[cm] >= val_defrost_stop_cm_tm_entry
                    && defrost_cm_intv_on >= 4)
                {
                    defrost_cm_intv_on = 0;
                    defrost_cm_time[cm] = 0;
                    if (val_defrost_four_tm_entry >= 0) // ��ǰ
                    {
                        defrost_cm_step[cm] = DEFROST_CM_OFF_FORE_ON;
                    }
                    else
                    {
                        defrost_cm_step[cm] = DEFROST_CM_ON_FORE_OFF;
                    }
                }
                break;
            // ��ǰ����ͨ
            case DEFROST_CM_OFF_FORE_ON:
            // �Ӻ���ͨ    
            case DEFROST_CM_ON_FORE_OFF:                
                if (defrost_cm_time[cm] >= fun_absolute_value(val_defrost_four_tm_entry))
                {
                    defrost_cm_time[cm] = 0;
                    defrost_cnt_defrost[cm] = 0;
                    defrost_cm_step[cm] = DEFROST_CM_OVER;
                }
                break;
            // ������˪
            case DEFROST_CM_OVER:
                S_CM_ClrStatus(cm, CM_DEFROST); 
                defrost_cm_step[cm] = DEFROST_CM_STOP;
                break;
            case DEFROST_CM_STOP: /* ʵ��ת�������״̬ */
            default:
                defrost_cm_step[cm] = DEFROST_CM_NULL;
                break;
        }

        /* ��˪ͣ�� */
        switch (defrost_cm_step[cm])
        {
            /* ��˪���������Ҫͣѹ����,������CM_ON��־ */
            case DEFROST_CM_STOP_BGN:
            case DEFROST_CM_OFF_FORE_OFF:
            case DEFROST_CM_STOP_END:
            case DEFROST_CM_OFF_FORE_ON:
//          case DEFROST_CM_STOP:   // ��˪�����¿���,��˴����ܿ���
                S_CM_SetStatus(cm, CM_DEFROST_OFF);
                break;
            default:
                S_CM_ClrStatus(cm, CM_DEFROST_OFF);
                break;
        }
    }
}
#endif


/*****************************************************************************
��  �� : defrost_signal_end()
��  �� : ��˪�ź�������,δ������ź�����ֱ�Ӿܾ�
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void defrost_signal_end(void)
{
    U08 cm;
    
    for (cm=0; cm<val_comp_num; cm++)
    {
        SIGNAL_AcceptEnd(&defrost_signal_into_lowpress[cm]);
        SIGNAL_AcceptEnd(&defrost_signal_into_namual[cm]);
        SIGNAL_AcceptEnd(&defrost_signal_exit_highpress[cm]);
		SIGNAL_AcceptEnd(&defrost_signal_exit_exh[cm]);
		SIGNAL_AcceptEnd(&defrost_signal_exit_cur[cm]);
    }
}

/*****************************************************************************
��  �� : defrost_init()
��  �� : ���ݳ�ʼ��
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void defrost_init(void)
{
    U16 i;

    ARRCLR(defrost_fg);
    ARRSET(defrost_temp_EWD_nofrost, SENSOR_NOEXIST);
    ARRSET(defrost_temp_EWD_nofrost_env, SENSOR_NOEXIST);
    ARRSET(defrost_temp_EWD_stop, SENSOR_NOEXIST);
    ARRSET(defrost_rec_EWD_avg, SENSOR_NOEXIST);
	ARRSET(defrost_rec_EWD_usedavg_avg, SENSOR_NOEXIST);

	
    for (i=0; i<MAX_COMP; i++)
    {
        ARRSET(defrost_rec_EWD[i], SENSOR_NOEXIST);
        ARRSET(defrost_rec_EWD_usedavg[i], SENSOR_NOEXIST);
    }
}

/*****************************************************************************
��  �� : defrost()
��  �� : ��˪�����ܵ���
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void defrost(void)
{
    U08 comp_num;
    U08 begin, end;

    defrost_time();

    defrost_fun_rec_data();
    
    for (comp_num=0; comp_num<val_comp_num; comp_num++) 
    {
		// ͣ������4Сʱ,���˪�����ͱ궨��־
        if (cm_time[comp_num].off > (4*60*60))
        {
            defrost_fg[comp_num].defrost_end = FALSE;
            defrost_fg[comp_num].need_calib_EWD = FALSE;
        }
        
        // �Ƿ���Ҫ�жϳ�˪
        if (Cm[comp_num].run_mode == MODE_COOL
        || (interface_defrost_wing_err(comp_num))                  /* ��Ƭ�¶�̽ͷ���� */
        || SensorIsError(defrost_fun_get_env())             /* �����¶�̽ͷ���� */
        || !S_CM_GetStatus(comp_num, CM_ON))                /*  ѹ��δ���� */
        { // �����˪��ر�־
            S_CM_ClrStatus(comp_num, CM_NEED_DEFROST);        /* ��˪���� */
            S_CM_ClrStatus(comp_num, CM_DEFROST);             /* ��˪��־ */ 
            S_CM_ClrStatus(comp_num, CM_DEFROST_ALLOW);
            S_CM_ClrStatus(comp_num, CM_FORCE_DEFROST);       /* ǿ�Ƴ�˪��־ */
            S_CM_ClrStatus(comp_num, CM_ALLOW_FORCE_DEFROST); /* ѹ��ǿ�Ƴ�˪�����־ */
            S_CM_ClrStatus(comp_num, CM_DEFROST_OFF);         
            defrost_fg[comp_num].defrost_apply = FALSE;
            defrost_cm_step[comp_num] = DEFROST_CM_NULL;
            continue;   
        }

        begin = 0, end = 0;
        log_mod_fan_comp_group(log_mod_cond_type(val_comp_num), &begin, &end, comp_num);
        if (end == 0)       break;              /* ���������ݣ�ֱ������ */
        
        // �����Ƿ���Ҫ�����˪
        defrost_grp_into(begin, end, comp_num);

        // �����Ƿ���Ҫ�˳���˪
        defrost_grp_exit(begin, end, comp_num);

        // �˳���˪����
        defrost_over(comp_num);

        // ��˪ͣ������
        defrost_comp_stop(comp_num);
    }

    //3 �źź���
    defrost_signal_end();
}


