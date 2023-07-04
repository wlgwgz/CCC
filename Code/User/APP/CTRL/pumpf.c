/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: pumpf.c
 �ļ�����: ��Ƶˮ�ù���Դ�ļ�
 ������  : J.H.
 ��������: 2022.07.10
*******************************************************************************/

#define PUMPF_CTRL_GLOBAL
#include "includes.h"

/*******************************************************************************
 �� ��: PUMPF_pwm_in_read()
 �� ��: ��ȡ�Ѿ����˲����ռ�ձ�
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void PUMPF_pwm_in_read(void)
{
    I16 pv_min = PUMPF_PWM_IN_NOEXIST;
    I16 pv_max = PUMPF_PWM_IN_NOEXIST;
    I16 fb_duty_stop = PUMPF_PWM_IN_NOEXIST;
    /* ���� */
    I16 fb_duty_min = PUMPF_PWM_IN_NOEXIST;
    I16 fb_duty_max = PUMPF_PWM_IN_NOEXIST;
    /* ���� */
    I16 fb_pow_min = PUMPF_PWM_IN_NOEXIST;
    I16 fb_pow_max = PUMPF_PWM_IN_NOEXIST;

    switch (val_PUMPf_model)
    {
        case PUMPF_00_PARA_9_IPWM1:         /* ���� */
            pv_min = PUMPF_00_FLOW_MIN;
            pv_max = PUMPF_00_FLOW_MAX;
            fb_duty_min = PUMPF_00_PWM_IN_FLOW_INFO_MIN;
            fb_duty_max = PUMPF_00_PWM_IN_FLOW_INFO_MAX;
            fb_duty_stop = PUMPF_00_PWM_IN_ERR_1;
            break;
            
        default:
        case PUMPF_01_APM25_9_130_PWM:      /* �½� */
            pv_min = PUMPF_01_FLOW_MIN;
            pv_max = PUMPF_01_FLOW_MAX;
            fb_duty_min = PUMPF_01_PWM_IN_FLOW_INFO_MIN;
            fb_duty_max = PUMPF_01_PWM_IN_FLOW_INFO_MAX;
            fb_duty_stop = PUMPF_01_PWM_IN_ERR_1;
            break;

        case PUMPF_02_GPA25_9HW_130:            /* �»� */
            pv_min = PUMPF_02_FLOW_MIN;
            pv_max = PUMPF_02_FLOW_MAX;
            fb_duty_min = PUMPF_02_PWM_IN_FLOW_INFO_MIN;
            fb_duty_max = PUMPF_02_PWM_IN_FLOW_INFO_MAX;
            fb_duty_stop = PUMPF_02_PWM_IN_ERR_1;
            break;

        case PUMPF_03_APF25_12_130FPWM1:    /* �½� APF25-12-130FPWM1 */
            pv_min = PUMPF_03_FLOW_MIN;
            pv_max = PUMPF_03_FLOW_MAX;
            fb_duty_min = PUMPF_03_PWM_IN_FLOW_INFO_MIN;
            fb_duty_max = PUMPF_03_PWM_IN_FLOW_INFO_MAX;
            fb_duty_stop = PUMPF_03_PWM_IN_ERR_1;
            break;

        case PUMPF_04_APF25_12_130PWM1:    /* �½� APF25-12-130PWM1 */
            pv_min = PUMPF_04_POW_MIN;
            pv_max = PUMPF_04_POW_MAX;
            fb_pow_min = PUMPF_04_PWM_IN_POWER_INFO_MIN;
            fb_pow_max = PUMPF_04_PWM_IN_POWER_INFO_MAX;
            fb_duty_stop = PUMPF_04_PWM_IN_ERR_3;
            break;

        case PUMPF_05_GPA25_11H:    /* �»� GPA25_11H1 */
            pv_min = PUMPF_05_FLOW_MIN;
            pv_max = PUMPF_05_FLOW_MAX;
            fb_duty_min = PUMPF_05_PWM_IN_FLOW_INFO_MIN;
            fb_duty_max = PUMPF_05_PWM_IN_FLOW_INFO_MAX;
            fb_duty_stop = PUMPF_05_PWM_IN_ERR_3;
            break;

        case PUMPF_06_HBG_25_125_130P:    /* ��ķ�� HBG-25-125-130P */
            pv_min = PUMPF_06_FLOW_MIN;
            pv_max = PUMPF_06_FLOW_MAX;
            fb_duty_min = PUMPF_06_PWM_IN_FLOW_INFO_MIN;
            fb_duty_max = PUMPF_06_PWM_IN_FLOW_INFO_MAX;
            fb_duty_stop = PUMPF_06_PWM_IN_ERR_1;
            break;

        case PUMPF_07_UPM4XLK_25_90_130:   /* ������ UPM4XLK 25-90 130 */    
            pv_min = PUMPF_07_FLOW_MIN;
            pv_max = PUMPF_07_FLOW_MAX;
            fb_duty_min = PUMPF_07_PWM_IN_FLOW_INFO_MIN;
            fb_duty_max = PUMPF_07_PWM_IN_FLOW_INFO_MAX;
            fb_duty_stop = PUMPF_07_PWM_IN_ERR_1;
            break;
    }

    /* ��ȡPWM���� */
    PUMPf_pwm_in = PunpGetPwmInDuty();

    /* ���� */
    {
        /* ��min~max�䣬������Ϣ����Ч */
        if (fun_pv_in(PUMPf_pwm_in, fb_duty_min, fb_duty_max))
        {
           PUMPf_flow_pv = fun_line_value(PUMPf_pwm_in, fb_duty_min, fb_duty_max, pv_min, pv_max);
        }

        /* ������ˮ��ͣ��ʱ */
        if ((PUMPf_pwm_in>fb_duty_stop && fb_duty_stop!=PUMPF_PWM_IN_NOEXIST)
        || (PUMPf_pwm_in < fb_duty_min && fb_duty_min!=PUMPF_PWM_IN_NOEXIST)
        )
        {
            PUMPf_flow_pv = 0;
        }

        if (fb_duty_min == PUMPF_PWM_IN_NOEXIST
        ||  fb_duty_max == PUMPF_PWM_IN_NOEXIST
            )
        {
            PUMPf_flow_pv = SENSOR_NOEXIST;
        }
    }

    /* ���� */
    {
        /* ��min~max�䣬������Ϣ����Ч */
        if (fun_pv_in(PUMPf_pwm_in, fb_pow_min, fb_pow_max))
        {
            PUMPf_power_pv = fun_line_value(PUMPf_pwm_in, fb_pow_min, fb_pow_max, pv_min, pv_max);
        }

        /* ������ˮ��ͣ��ʱ */
        if ((PUMPf_pwm_in>fb_duty_stop && fb_duty_stop!=PUMPF_PWM_IN_NOEXIST)
        || (PUMPf_pwm_in < fb_pow_min && fb_pow_min!=PUMPF_PWM_IN_NOEXIST)
        )
        {
            PUMPf_power_pv = 0;
        }

        if (fb_pow_min == PUMPF_PWM_IN_NOEXIST
        ||  fb_pow_max == PUMPF_PWM_IN_NOEXIST
            )
        {
            PUMPf_power_pv = SENSOR_NOEXIST;
        }
    }
}

/*******************************************************************************
 �� ��: PUMPF_pwm_out_deal()
 �� ��: �����Ƶˮ��PWM���
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void PUMPF_pwm_out_deal(void)
{
	/* ��ȡPWM���� */
	PUMPF_pwm_in_read();

	if((TestProgStaus==TPS_NO_TEST_CMD)||(TestProgStaus==TPS_DISENABLE_TEST))
	{
		PunpSetPwmFreqDuty(1000, PUMPf_pwm_out, PWM_OUT_1);
	}
}

/*******************************************************************************
 �� ��: PUMPF_speed_ctrl()
 �� ��: ��Ƶˮ�õ���
 �� ��: fg_on--��Ƶˮ�ÿ�����־; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void PUMPF_speed_ctrl(BOL fg_on)
{
    I16 speed_incr = 0;
    I16 rpm_min, rpm_max;
    I16 rpm_max_pwm_out, rpm_min_pwm_out;
    I16 idle_pwm_out;
    I16 rpm_sv = 0;
    I16 flow_sv = 0;
    static I16 flow_sv_bak = 0;
    static BOL fg_rpm_sv_init_need = TRUE;

    /* ѡ���Ƶˮ���ͺ� */
    switch (val_PUMPf_model)
    {
        case PUMPF_00_PARA_9_IPWM1:         /* ���� */
            rpm_min = PUMPF_00_RPM_MIN;
            rpm_max = PUMPF_00_RPM_MAX;
            rpm_min_pwm_out = PUMPF_00_PWM_OUT_RPM_MIN_BGN;
            rpm_max_pwm_out = PUMPF_00_PWM_OUT_RPM_MAX_END;
            idle_pwm_out = PUMPF_00_PWM_OUT_IDLE;
            break;

        default:
        case PUMPF_01_APM25_9_130_PWM:      /* �½� */
            rpm_min = PUMPF_01_RPM_MIN;
            rpm_max = PUMPF_01_RPM_MAX;
            rpm_min_pwm_out = PUMPF_01_PWM_OUT_RPM_MIN_BGN;
            rpm_max_pwm_out = PUMPF_01_PWM_OUT_RPM_MAX_END;
            idle_pwm_out = PUMPF_01_PWM_OUT_IDLE;
            break;

        case PUMPF_02_GPA25_9HW_130:        /* �»� */
            rpm_min = PUMPF_02_RPM_MIN;
            rpm_max = PUMPF_02_RPM_MAX;
            rpm_min_pwm_out = PUMPF_02_PWM_OUT_RPM_MIN_BGN;
            rpm_max_pwm_out = PUMPF_02_PWM_OUT_RPM_MAX_END;
            idle_pwm_out = PUMPF_02_PWM_OUT_IDLE;
            break;

        case PUMPF_03_APF25_12_130FPWM1:    /* �½� APF25-12-130FPWM1 */
            rpm_min = PUMPF_03_RPM_MIN;
            rpm_max = PUMPF_03_RPM_MAX;
            rpm_min_pwm_out = PUMPF_03_PWM_OUT_RPM_MIN_BGN;
            rpm_max_pwm_out = PUMPF_03_PWM_OUT_RPM_MAX_END;
            idle_pwm_out = PUMPF_03_PWM_OUT_IDLE;
            break;

        case PUMPF_04_APF25_12_130PWM1:    /* �½� APF25-12-130PWM1 */
            rpm_min = PUMPF_04_RPM_MIN;
            rpm_max = PUMPF_04_RPM_MAX;
            rpm_min_pwm_out = PUMPF_04_PWM_OUT_RPM_MIN_BGN;
            rpm_max_pwm_out = PUMPF_04_PWM_OUT_RPM_MAX_END;
            idle_pwm_out = PUMPF_04_PWM_OUT_IDLE;
            break;

        case PUMPF_05_GPA25_11H:    /* �»� GPA25_11H1 */
            rpm_min = PUMPF_05_RPM_MIN;
            rpm_max = PUMPF_05_RPM_MAX;
            rpm_min_pwm_out = PUMPF_05_PWM_OUT_RPM_MIN_BGN;
            rpm_max_pwm_out = PUMPF_05_PWM_OUT_RPM_MAX_END;
            idle_pwm_out = PUMPF_05_PWM_OUT_IDLE;
            break;
            
        case PUMPF_06_HBG_25_125_130P:    /* ��ķ�� HBG-25-125-130P */
            rpm_min = PUMPF_06_RPM_MIN;
            rpm_max = PUMPF_06_RPM_MAX;
            rpm_min_pwm_out = PUMPF_06_PWM_OUT_RPM_MIN_BGN;
            rpm_max_pwm_out = PUMPF_06_PWM_OUT_RPM_MAX_END;
            idle_pwm_out = PUMPF_06_PWM_OUT_IDLE;
            break;

        case PUMPF_07_UPM4XLK_25_90_130:   /* ������ UPM4XLK 25-90 130 */   
            rpm_min = PUMPF_07_RPM_MIN;
            rpm_max = PUMPF_07_RPM_MAX;
            rpm_min_pwm_out = PUMPF_07_PWM_OUT_RPM_MIN_BGN;
            rpm_max_pwm_out = PUMPF_07_PWM_OUT_RPM_MAX_END;
            idle_pwm_out = PUMPF_07_PWM_OUT_IDLE;
            break;
    }

    /* ���ִ��� */
    if (!fg_on                                              /* δ�õ� */
    || (USE_PUMPf_CTRL==PUMPF_CTRL_PWR_ON_ONLY)             /* ���õ� */
    )                              
    {
        fg_rpm_sv_init_need = TRUE;
        PUMPf_pwm_out = idle_pwm_out;
        PUMPf_rpm_sv = 0;
        return;
    }
    
    /* �������� */
    if (USE_PUMPf_CTRL == PUMPF_CTRL_FLOW)    /* ������ */
    {
        if (SensorCannotUse(PUMPf_flow_pv))
        {
            PUMPf_pwm_out = idle_pwm_out;
        }
        /* ��ʼ��Ϊ���ת�ٶ�Ӧ�����ռ�ձ� */
        else if (fg_rpm_sv_init_need)
        {
            PUMPf_pwm_out = rpm_max_pwm_out;
            /* �����ת��ά��һ��ʱ�� */
            if (cnt_PUMPf_ctrl > val_PUMPf_init_stable)
            {
                fg_rpm_sv_init_need = FALSE;
            }
        }
        else
        {
            flow_sv = USE_DEBUG_PUMP ? val_debug_test_pump : val_pump_sv_flow;
        
            if (cnt_PUMPf_ctrl > val_PUMPf_ctrl_period)
            {
                cnt_PUMPf_ctrl = 0;
                speed_incr = Fun_Range(PUMPf_flow_pv, flow_sv, 10) ? 10 : 50;   /* 1%��5%�仯�� */
                if (PUMPf_flow_pv < flow_sv - val_PUMPf_flow_diff)
                {
                    PUMPf_pwm_out -= speed_incr;
                }
                else if (PUMPf_flow_pv > flow_sv + val_PUMPf_flow_diff)
                {
                    PUMPf_pwm_out += speed_incr;
                }
            }

            if (flow_sv == 0)
            {
                PUMPf_pwm_out = idle_pwm_out;
                PUMPf_rpm_sv = 0;
            }
            else
            {
                /* ��Χ���� */
                PUMPf_pwm_out = fun_pv_limit(PUMPf_pwm_out, rpm_max_pwm_out, rpm_min_pwm_out);
            }

            if ((flow_sv!=0)
            && (flow_sv!=flow_sv_bak)
            )
            {
                flow_sv_bak = flow_sv;
                fg_rpm_sv_init_need = TRUE;
                cnt_PUMPf_ctrl = 0;
            }
        }
    }

    /* ���ʿ��� */
    if (USE_PUMPf_CTRL == PUMPF_CTRL_POWER)
    {
        if (SensorCannotUse(PUMPf_power_pv))
        {
            PUMPf_pwm_out = idle_pwm_out;
        }
        /* ��ʼ��Ϊ���ת�ٶ�Ӧ�����ռ�ձ� */
        else if (fg_rpm_sv_init_need)
        {
            PUMPf_pwm_out = rpm_max_pwm_out;
            /* �����ת��ά��һ��ʱ�� */
            if (cnt_PUMPf_ctrl > val_PUMPf_init_stable)
            {
                fg_rpm_sv_init_need = FALSE;
            }
        }
        else
        {
            flow_sv = USE_DEBUG_PUMP ? val_debug_test_pump : val_pump_sv_flow;
        
            if (cnt_PUMPf_ctrl > val_PUMPf_ctrl_period)
            {
                cnt_PUMPf_ctrl = 0;
                speed_incr = Fun_Range(PUMPf_power_pv, flow_sv, 10) ? 10 : 50;   /* 1%��5%�仯�� */
                if (PUMPf_power_pv < flow_sv - val_PUMPf_power_diff)
                {
                    PUMPf_pwm_out -= speed_incr;
                }
                else if (PUMPf_power_pv > flow_sv + val_PUMPf_power_diff)
                {
                    PUMPf_pwm_out += speed_incr;
                }
            }

            if (flow_sv == 0)
            {
                PUMPf_pwm_out = idle_pwm_out;
                PUMPf_rpm_sv = 0;
            }
            else
            {
                /* ��Χ���� */
                PUMPf_pwm_out = fun_pv_limit(PUMPf_pwm_out, rpm_max_pwm_out, rpm_min_pwm_out);
            }

            if ((flow_sv!=0)
            && (flow_sv!=flow_sv_bak)
            )
            {
                flow_sv_bak = flow_sv;
                fg_rpm_sv_init_need = TRUE;
                cnt_PUMPf_ctrl = 0;
            }
        }
    }

    /* ת�ٿ��� */
    if (USE_PUMPf_CTRL == PUMPF_CTRL_RPM)   /* ��ת�� */
    {
        rpm_sv = USE_DEBUG_PUMP ? val_debug_test_pump : TF_VAL(val_pump_sv, 4500);
        rpm_sv = fun_pv_limit(rpm_sv, rpm_min, rpm_max);
    
//        if (F_TIME_BASIC_SEC_BACK)
//        {
//            speed_incr = _DIVUCARRY(rpm_sv, TF_VAL(AC_PUMP_ON_DLY, TF_PUMP_ON_DLY));
//            PUMPf_rpm_sv += speed_incr;
//            if (PUMPf_rpm_sv > rpm_sv)    
//            {
//                PUMPf_rpm_sv = rpm_sv;
//            }
//        }    

        /* RPMת�ٻ���ΪPWM�ٷֱ� */
        PUMPf_pwm_out = fun_line_value(rpm_sv, rpm_min, rpm_max, rpm_min_pwm_out, rpm_max_pwm_out); // �����
    }

    /* ��Ƶˮ�þ���ʱ�������ת������ */
    if (PUMPF_is_warn())
    {
        PUMPf_pwm_out = rpm_max_pwm_out;
    }

    /* PWM�ٷֱȻ���ΪRPMת�� */
    PUMPf_rpm_sv = fun_line_value(PUMPf_pwm_out, rpm_max_pwm_out, rpm_min_pwm_out, rpm_max, rpm_min);

    /* ��ȡˮ�����ת�� */
    PUMPf_rpm_max = rpm_max;
}

/*******************************************************************************
 �� ��: PUMPF_status()
 �� ��: 
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void PUMPF_status(void)
{
    #define PWM_IN_DIFF            (5)      /* PWM�ٷֱȻز�ֵ: 0.5% */
    
    I16 pwm_sc; 
    I16 pwm_idle;
    I16 pwm_min, pwm_max;
    I16 pwm_warn;
    I16 pwm_err_1, pwm_err_2, pwm_err_3;
    I16 pwm_oc;
    I16 pwm_diff = PWM_IN_DIFF;

    /* ��Ƶˮ��δ���� */
    if (!fg_PUMPf_run)
    {
        PUMPf_err_id = PUMF_NO_ERR;
        PUMPf_sta = PUMPF_STA_IDLE;
    }

    switch (val_PUMPf_model)
    {   
        case PUMPF_00_PARA_9_IPWM1:         /* ���� */
            pwm_sc = PUMPF_00_PWM_IN_SC;
            pwm_idle = PUMPF_00_PWM_IN_IDLE;
            pwm_min = PUMPF_00_PWM_IN_FLOW_INFO_MIN;
            pwm_max = PUMPF_00_PWM_IN_FLOW_INFO_MAX;
            pwm_warn = PUMPF_00_PWM_IN_WARN;
            pwm_err_1 = PUMPF_00_PWM_IN_ERR_1;
            pwm_err_2 = PUMPF_00_PWM_IN_ERR_2;
            pwm_err_3 = PUMPF_00_PWM_IN_ERR_3;
            pwm_oc = PUMPF_00_PWM_IN_OC;
            break;

        default:
        case PUMPF_01_APM25_9_130_PWM:      /* �½� */
            pwm_sc = PUMPF_01_PWM_IN_SC;
            pwm_idle = PUMPF_01_PWM_IN_IDLE;
            pwm_min = PUMPF_01_PWM_IN_FLOW_INFO_MIN;
            pwm_max = PUMPF_01_PWM_IN_FLOW_INFO_MAX;
            pwm_warn = PUMPF_01_PWM_IN_WARN;
            pwm_err_1 = PUMPF_01_PWM_IN_ERR_1;
            pwm_err_2 = PUMPF_01_PWM_IN_ERR_2;
            pwm_err_3 = PUMPF_01_PWM_IN_ERR_3;
            pwm_oc = PUMPF_01_PWM_IN_OC;
            break;
            
        case PUMPF_02_GPA25_9HW_130:        /* �»� */
            pwm_sc = PUMPF_02_PWM_IN_SC;
            pwm_idle = PUMPF_02_PWM_IN_IDLE;
            pwm_min = PUMPF_02_PWM_IN_FLOW_INFO_MIN;
            pwm_max = PUMPF_02_PWM_IN_FLOW_INFO_MAX;
            pwm_warn = PUMPF_02_PWM_IN_WARN;
            pwm_err_1 = PUMPF_02_PWM_IN_ERR_1;
            pwm_err_2 = PUMPF_02_PWM_IN_ERR_2;
            pwm_err_3 = PUMPF_02_PWM_IN_ERR_3;
            pwm_oc = PUMPF_02_PWM_IN_OC;
            break;

        case PUMPF_03_APF25_12_130FPWM1:	/* �½� APF25-12-130FPWM1 */
            pwm_sc = PUMPF_03_PWM_IN_SC;
            pwm_idle = PUMPF_03_PWM_IN_IDLE;
            pwm_min = PUMPF_03_PWM_IN_FLOW_INFO_MIN;
            pwm_max = PUMPF_03_PWM_IN_FLOW_INFO_MAX;
            pwm_warn = PUMPF_03_PWM_IN_WARN;
            pwm_err_1 = PUMPF_03_PWM_IN_ERR_1;
            pwm_err_2 = PUMPF_03_PWM_IN_ERR_2;
            pwm_err_3 = PUMPF_03_PWM_IN_ERR_3;
            pwm_oc = PUMPF_03_PWM_IN_OC;
            break;    

        case PUMPF_04_APF25_12_130PWM1:	/* �½� APF25-12-130PWM1 */
            pwm_sc = PUMPF_04_PWM_IN_SC;
            pwm_idle = PUMPF_04_PWM_IN_IDLE;
            pwm_min = PUMPF_04_PWM_IN_POWER_INFO_MIN;
            pwm_max = PUMPF_04_PWM_IN_POWER_INFO_MAX;
            pwm_warn = PUMPF_04_PWM_IN_WARN;
            pwm_err_1 = PUMPF_04_PWM_IN_ERR_1;
            pwm_err_2 = PUMPF_04_PWM_IN_ERR_2;
            pwm_err_3 = PUMPF_04_PWM_IN_ERR_3;
            pwm_oc = PUMPF_04_PWM_IN_OC;
            break;   

         case PUMPF_05_GPA25_11H:	/* �»� GPA25_11H1 */
            pwm_sc = PUMPF_05_PWM_IN_SC;
            pwm_idle = PUMPF_05_PWM_IN_IDLE;
            pwm_min = PUMPF_05_PWM_IN_FLOW_INFO_MIN;
            pwm_max = PUMPF_05_PWM_IN_FLOW_INFO_MAX;
            pwm_warn = PUMPF_05_PWM_IN_WARN;
            pwm_err_1 = PUMPF_05_PWM_IN_ERR_1;
            pwm_err_2 = PUMPF_05_PWM_IN_ERR_2;
            pwm_err_3 = PUMPF_05_PWM_IN_ERR_3;
            pwm_oc = PUMPF_05_PWM_IN_OC;
            break;
            
         case PUMPF_06_HBG_25_125_130P:    /* ��ķ�� HBG-25-125-130P */
            pwm_sc = PUMPF_06_PWM_IN_SC;
            pwm_idle = PUMPF_06_PWM_IN_IDLE;
            pwm_min = PUMPF_06_PWM_IN_FLOW_INFO_MIN;
            pwm_max = PUMPF_06_PWM_IN_FLOW_INFO_MAX;
            pwm_warn = PUMPF_06_PWM_IN_WARN;
            pwm_err_1 = PUMPF_06_PWM_IN_ERR_1;
            pwm_err_2 = PUMPF_06_PWM_IN_ERR_2;
            pwm_err_3 = PUMPF_06_PWM_IN_ERR_3;
            pwm_oc = PUMPF_06_PWM_IN_OC;
            break;

         case PUMPF_07_UPM4XLK_25_90_130:   /* ������ UPM4XLK 25-90 130 */
            pwm_sc = PUMPF_07_PWM_IN_SC;
            pwm_idle = PUMPF_07_PWM_IN_IDLE;
            pwm_min = PUMPF_07_PWM_IN_FLOW_INFO_MIN;
            pwm_max = PUMPF_07_PWM_IN_FLOW_INFO_MAX;
            pwm_warn = PUMPF_07_PWM_IN_WARN;
            pwm_err_1 = PUMPF_07_PWM_IN_ERR_1;
            pwm_err_2 = PUMPF_07_PWM_IN_ERR_2;
            pwm_err_3 = PUMPF_07_PWM_IN_ERR_3;
            pwm_oc = PUMPF_07_PWM_IN_OC;
            pwm_diff = PUMPF_07_PWM_IN_DIFF;
            break;
    }

    /* ����1: ��· */
    if (pwm_sc != PUMPF_PWM_IN_NOEXIST
    && PUMPf_pwm_in >= pwm_sc-pwm_diff
    && PUMPf_pwm_in <= pwm_sc+pwm_diff
    )
    {
        PUMPf_err_id = PUMPF_ERR_01;
        PUMPf_sta = PUMPF_STA_SC;
    }

    /* ���� */
    if (PUMPf_pwm_in >= pwm_idle-pwm_diff
    && PUMPf_pwm_in <= pwm_idle+pwm_diff
    )
    {
        PUMPf_err_id = PUMF_NO_ERR;
        PUMPf_sta = PUMPF_STA_IDLE;
    }

    /* ���� */
    if (PUMPf_pwm_in >= pwm_warn-pwm_diff
    && PUMPf_pwm_in <= pwm_warn+pwm_diff
    )
    {
        PUMPf_err_id = PUMF_NO_ERR;
        PUMPf_sta = PUMPF_STA_WARN;
    }

    /* ���� */
    if (PUMPf_pwm_in >= pwm_min-pwm_diff
    && PUMPf_pwm_in <= pwm_max+pwm_diff
    )
    {
        PUMPf_err_id = PUMF_NO_ERR;
        PUMPf_sta = PUMPF_STA_RUN;
    }

    /* ����2 */
    if (PUMPf_pwm_in >= pwm_err_1-pwm_diff
    && PUMPf_pwm_in <= pwm_err_1+pwm_diff
    )
    {
        PUMPf_err_id = PUMPF_ERR_02;
        PUMPf_sta = PUMPF_STA_ERR;
    }

    /* ����3 */
    if (PUMPf_pwm_in >= pwm_err_2-pwm_diff
    && PUMPf_pwm_in <= pwm_err_2+pwm_diff
    )
    {
        PUMPf_err_id = PUMPF_ERR_03;
        PUMPf_sta = PUMPF_STA_ERR;
    }

    /* ����4 */
    if (PUMPf_pwm_in >= pwm_err_3-pwm_diff
    && PUMPf_pwm_in <= pwm_err_3+pwm_diff
    )
    {
        PUMPf_err_id = PUMPF_ERR_04;
        PUMPf_sta = PUMPF_STA_ERR;
    }

    /* ����5: ��· */
    if (pwm_oc != PUMPF_PWM_IN_NOEXIST
    && PUMPf_pwm_in >= pwm_oc-pwm_diff
    && PUMPf_pwm_in <= pwm_oc+pwm_diff
    )
    {
        PUMPf_err_id = PUMPF_ERR_05;
        PUMPf_sta = PUMPF_STA_OC;
    }

#if defined(USE_BMS_DEBUG_PUMPF_ERR)
    /* BMS���Դ�����Ƶˮ�ù�����Ϣ */
    if (GET_BMS_DEBUG_VAL(BDV_PUMPF_ERR))
    {
        PUMPf_err_id = GET_BMS_DEBUG_VAL(BDV_PUMPF_ERR_CODE);
    }
#endif
}

/*******************************************************************************
 �� ��: PUMPF_is_warn()
 �� ��: ��Ƶˮ�þ���
 �� ��: ��
 �� ��: 
 �� ��: ��
*******************************************************************************/
BOL PUMPF_is_warn(void)
{
#if defined(USE_BMS_DEBUG_PUMPF_ERR)
    /* BMS���Դ�����Ƶˮ�þ��� */
    if (GET_BMS_DEBUG_VAL(BDV_PUMPF_WARN))
    {
        return TRUE;
    }
#endif
    
    return (PUMPf_sta == PUMPF_STA_WARN) ? TRUE : FALSE;
}

/*******************************************************************************
 �� ��: PUMPF_is_err()
 �� ��: ��Ƶˮ�ù���
 �� ��: ��
 �� ��: 
 �� ��: ��
*******************************************************************************/
BOL PUMPF_is_err(void)
{
#if defined(USE_BMS_DEBUG_PUMPF_ERR)
    /* BMS���Դ�����Ƶˮ�ù��� */
    if (GET_BMS_DEBUG_VAL(BDV_PUMPF_ERR))
    {
        return TRUE;
    }
#endif
    
    /* ���õ�ʱ���������� */
    if (USE_PUMPf_CTRL == PUMPF_CTRL_PWR_ON_ONLY)
    {
        return FALSE;
    }
    
    if (PUMPf_sta == PUMPF_STA_ERR
    || PUMPf_sta == PUMPF_STA_OC
    || PUMPf_sta == PUMPF_STA_SC
    )
    {
        return TRUE;
    }

    return FALSE;
}

/*******************************************************************************
 �� ��: PUMPF_can_reset_err()
 �� ��: ��Ƶˮ�ù��Ͽɸ�λ
 �� ��: ��
 �� ��: 
 �� ��: ��
*******************************************************************************/
BOL PUMPF_can_reset_err(void)
{
    if (USE_PUMPf_CTRL == PUMPF_CTRL_PWR_ON_ONLY
    || PUMPf_sta == PUMPF_STA_IDLE
    || PUMPf_sta == PUMPF_STA_RUN
    )
    {
        return TRUE;
    }

    return FALSE;
}

/*******************************************************************************
 �� ��: PUMPF_init()
 �� ��: 
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void PUMPF_init(void)
{
    fg_PUMPf_need = FALSE;
    fg_PUMPf_long_time_no_run = FALSE;
    fg_PUMPf_test_run = FALSE;
    fg_PUMPf_debug_run = FALSE;
    PUMPf_sta = PUMPF_STA_IDLE;
}

