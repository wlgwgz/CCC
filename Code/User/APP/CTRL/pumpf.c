/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: pumpf.c
 文件描述: 变频水泵功能源文件
 创建人  : J.H.
 创建日期: 2022.07.10
*******************************************************************************/

#define PUMPF_CTRL_GLOBAL
#include "includes.h"

/*******************************************************************************
 函 数: PUMPF_pwm_in_read()
 功 能: 读取已经过滤波后的占空比
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void PUMPF_pwm_in_read(void)
{
    I16 pv_min = PUMPF_PWM_IN_NOEXIST;
    I16 pv_max = PUMPF_PWM_IN_NOEXIST;
    I16 fb_duty_stop = PUMPF_PWM_IN_NOEXIST;
    /* 流量 */
    I16 fb_duty_min = PUMPF_PWM_IN_NOEXIST;
    I16 fb_duty_max = PUMPF_PWM_IN_NOEXIST;
    /* 功率 */
    I16 fb_pow_min = PUMPF_PWM_IN_NOEXIST;
    I16 fb_pow_max = PUMPF_PWM_IN_NOEXIST;

    switch (val_PUMPf_model)
    {
        case PUMPF_00_PARA_9_IPWM1:         /* 威乐 */
            pv_min = PUMPF_00_FLOW_MIN;
            pv_max = PUMPF_00_FLOW_MAX;
            fb_duty_min = PUMPF_00_PWM_IN_FLOW_INFO_MIN;
            fb_duty_max = PUMPF_00_PWM_IN_FLOW_INFO_MAX;
            fb_duty_stop = PUMPF_00_PWM_IN_ERR_1;
            break;
            
        default:
        case PUMPF_01_APM25_9_130_PWM:      /* 新界 */
            pv_min = PUMPF_01_FLOW_MIN;
            pv_max = PUMPF_01_FLOW_MAX;
            fb_duty_min = PUMPF_01_PWM_IN_FLOW_INFO_MIN;
            fb_duty_max = PUMPF_01_PWM_IN_FLOW_INFO_MAX;
            fb_duty_stop = PUMPF_01_PWM_IN_ERR_1;
            break;

        case PUMPF_02_GPA25_9HW_130:            /* 新沪 */
            pv_min = PUMPF_02_FLOW_MIN;
            pv_max = PUMPF_02_FLOW_MAX;
            fb_duty_min = PUMPF_02_PWM_IN_FLOW_INFO_MIN;
            fb_duty_max = PUMPF_02_PWM_IN_FLOW_INFO_MAX;
            fb_duty_stop = PUMPF_02_PWM_IN_ERR_1;
            break;

        case PUMPF_03_APF25_12_130FPWM1:    /* 新界 APF25-12-130FPWM1 */
            pv_min = PUMPF_03_FLOW_MIN;
            pv_max = PUMPF_03_FLOW_MAX;
            fb_duty_min = PUMPF_03_PWM_IN_FLOW_INFO_MIN;
            fb_duty_max = PUMPF_03_PWM_IN_FLOW_INFO_MAX;
            fb_duty_stop = PUMPF_03_PWM_IN_ERR_1;
            break;

        case PUMPF_04_APF25_12_130PWM1:    /* 新界 APF25-12-130PWM1 */
            pv_min = PUMPF_04_POW_MIN;
            pv_max = PUMPF_04_POW_MAX;
            fb_pow_min = PUMPF_04_PWM_IN_POWER_INFO_MIN;
            fb_pow_max = PUMPF_04_PWM_IN_POWER_INFO_MAX;
            fb_duty_stop = PUMPF_04_PWM_IN_ERR_3;
            break;

        case PUMPF_05_GPA25_11H:    /* 新沪 GPA25_11H1 */
            pv_min = PUMPF_05_FLOW_MIN;
            pv_max = PUMPF_05_FLOW_MAX;
            fb_duty_min = PUMPF_05_PWM_IN_FLOW_INFO_MIN;
            fb_duty_max = PUMPF_05_PWM_IN_FLOW_INFO_MAX;
            fb_duty_stop = PUMPF_05_PWM_IN_ERR_3;
            break;

        case PUMPF_06_HBG_25_125_130P:    /* 奥姆特 HBG-25-125-130P */
            pv_min = PUMPF_06_FLOW_MIN;
            pv_max = PUMPF_06_FLOW_MAX;
            fb_duty_min = PUMPF_06_PWM_IN_FLOW_INFO_MIN;
            fb_duty_max = PUMPF_06_PWM_IN_FLOW_INFO_MAX;
            fb_duty_stop = PUMPF_06_PWM_IN_ERR_1;
            break;

        case PUMPF_07_UPM4XLK_25_90_130:   /* 格兰富 UPM4XLK 25-90 130 */    
            pv_min = PUMPF_07_FLOW_MIN;
            pv_max = PUMPF_07_FLOW_MAX;
            fb_duty_min = PUMPF_07_PWM_IN_FLOW_INFO_MIN;
            fb_duty_max = PUMPF_07_PWM_IN_FLOW_INFO_MAX;
            fb_duty_stop = PUMPF_07_PWM_IN_ERR_1;
            break;
    }

    /* 获取PWM输入 */
    PUMPf_pwm_in = PunpGetPwmInDuty();

    /* 流量 */
    {
        /* 在min~max间，流量信息才有效 */
        if (fun_pv_in(PUMPf_pwm_in, fb_duty_min, fb_duty_max))
        {
           PUMPf_flow_pv = fun_line_value(PUMPf_pwm_in, fb_duty_min, fb_duty_max, pv_min, pv_max);
        }

        /* 待机或水泵停机时 */
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

    /* 功率 */
    {
        /* 在min~max间，功率信息才有效 */
        if (fun_pv_in(PUMPf_pwm_in, fb_pow_min, fb_pow_max))
        {
            PUMPf_power_pv = fun_line_value(PUMPf_pwm_in, fb_pow_min, fb_pow_max, pv_min, pv_max);
        }

        /* 待机或水泵停机时 */
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
 函 数: PUMPF_pwm_out_deal()
 功 能: 处理变频水泵PWM输出
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void PUMPF_pwm_out_deal(void)
{
	/* 读取PWM输入 */
	PUMPF_pwm_in_read();

	if((TestProgStaus==TPS_NO_TEST_CMD)||(TestProgStaus==TPS_DISENABLE_TEST))
	{
		PunpSetPwmFreqDuty(1000, PUMPf_pwm_out, PWM_OUT_1);
	}
}

/*******************************************************************************
 函 数: PUMPF_speed_ctrl()
 功 能: 变频水泵调速
 参 数: fg_on--变频水泵开启标志; 
 返 回: 空
 其 它: 空
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

    /* 选择变频水泵型号 */
    switch (val_PUMPf_model)
    {
        case PUMPF_00_PARA_9_IPWM1:         /* 威乐 */
            rpm_min = PUMPF_00_RPM_MIN;
            rpm_max = PUMPF_00_RPM_MAX;
            rpm_min_pwm_out = PUMPF_00_PWM_OUT_RPM_MIN_BGN;
            rpm_max_pwm_out = PUMPF_00_PWM_OUT_RPM_MAX_END;
            idle_pwm_out = PUMPF_00_PWM_OUT_IDLE;
            break;

        default:
        case PUMPF_01_APM25_9_130_PWM:      /* 新界 */
            rpm_min = PUMPF_01_RPM_MIN;
            rpm_max = PUMPF_01_RPM_MAX;
            rpm_min_pwm_out = PUMPF_01_PWM_OUT_RPM_MIN_BGN;
            rpm_max_pwm_out = PUMPF_01_PWM_OUT_RPM_MAX_END;
            idle_pwm_out = PUMPF_01_PWM_OUT_IDLE;
            break;

        case PUMPF_02_GPA25_9HW_130:        /* 新沪 */
            rpm_min = PUMPF_02_RPM_MIN;
            rpm_max = PUMPF_02_RPM_MAX;
            rpm_min_pwm_out = PUMPF_02_PWM_OUT_RPM_MIN_BGN;
            rpm_max_pwm_out = PUMPF_02_PWM_OUT_RPM_MAX_END;
            idle_pwm_out = PUMPF_02_PWM_OUT_IDLE;
            break;

        case PUMPF_03_APF25_12_130FPWM1:    /* 新界 APF25-12-130FPWM1 */
            rpm_min = PUMPF_03_RPM_MIN;
            rpm_max = PUMPF_03_RPM_MAX;
            rpm_min_pwm_out = PUMPF_03_PWM_OUT_RPM_MIN_BGN;
            rpm_max_pwm_out = PUMPF_03_PWM_OUT_RPM_MAX_END;
            idle_pwm_out = PUMPF_03_PWM_OUT_IDLE;
            break;

        case PUMPF_04_APF25_12_130PWM1:    /* 新界 APF25-12-130PWM1 */
            rpm_min = PUMPF_04_RPM_MIN;
            rpm_max = PUMPF_04_RPM_MAX;
            rpm_min_pwm_out = PUMPF_04_PWM_OUT_RPM_MIN_BGN;
            rpm_max_pwm_out = PUMPF_04_PWM_OUT_RPM_MAX_END;
            idle_pwm_out = PUMPF_04_PWM_OUT_IDLE;
            break;

        case PUMPF_05_GPA25_11H:    /* 新沪 GPA25_11H1 */
            rpm_min = PUMPF_05_RPM_MIN;
            rpm_max = PUMPF_05_RPM_MAX;
            rpm_min_pwm_out = PUMPF_05_PWM_OUT_RPM_MIN_BGN;
            rpm_max_pwm_out = PUMPF_05_PWM_OUT_RPM_MAX_END;
            idle_pwm_out = PUMPF_05_PWM_OUT_IDLE;
            break;
            
        case PUMPF_06_HBG_25_125_130P:    /* 奥姆特 HBG-25-125-130P */
            rpm_min = PUMPF_06_RPM_MIN;
            rpm_max = PUMPF_06_RPM_MAX;
            rpm_min_pwm_out = PUMPF_06_PWM_OUT_RPM_MIN_BGN;
            rpm_max_pwm_out = PUMPF_06_PWM_OUT_RPM_MAX_END;
            idle_pwm_out = PUMPF_06_PWM_OUT_IDLE;
            break;

        case PUMPF_07_UPM4XLK_25_90_130:   /* 格兰富 UPM4XLK 25-90 130 */   
            rpm_min = PUMPF_07_RPM_MIN;
            rpm_max = PUMPF_07_RPM_MAX;
            rpm_min_pwm_out = PUMPF_07_PWM_OUT_RPM_MIN_BGN;
            rpm_max_pwm_out = PUMPF_07_PWM_OUT_RPM_MAX_END;
            idle_pwm_out = PUMPF_07_PWM_OUT_IDLE;
            break;
    }

    /* 保持待机 */
    if (!fg_on                                              /* 未得电 */
    || (USE_PUMPf_CTRL==PUMPF_CTRL_PWR_ON_ONLY)             /* 仅得电 */
    )                              
    {
        fg_rpm_sv_init_need = TRUE;
        PUMPf_pwm_out = idle_pwm_out;
        PUMPf_rpm_sv = 0;
        return;
    }
    
    /* 流量控制 */
    if (USE_PUMPf_CTRL == PUMPF_CTRL_FLOW)    /* 控流量 */
    {
        if (SensorCannotUse(PUMPf_flow_pv))
        {
            PUMPf_pwm_out = idle_pwm_out;
        }
        /* 初始化为最高转速对应的输出占空比 */
        else if (fg_rpm_sv_init_need)
        {
            PUMPf_pwm_out = rpm_max_pwm_out;
            /* 以最高转速维持一段时间 */
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
                speed_incr = Fun_Range(PUMPf_flow_pv, flow_sv, 10) ? 10 : 50;   /* 1%或5%变化量 */
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
                /* 范围限制 */
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

    /* 功率控制 */
    if (USE_PUMPf_CTRL == PUMPF_CTRL_POWER)
    {
        if (SensorCannotUse(PUMPf_power_pv))
        {
            PUMPf_pwm_out = idle_pwm_out;
        }
        /* 初始化为最高转速对应的输出占空比 */
        else if (fg_rpm_sv_init_need)
        {
            PUMPf_pwm_out = rpm_max_pwm_out;
            /* 以最高转速维持一段时间 */
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
                speed_incr = Fun_Range(PUMPf_power_pv, flow_sv, 10) ? 10 : 50;   /* 1%或5%变化量 */
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
                /* 范围限制 */
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

    /* 转速控制 */
    if (USE_PUMPf_CTRL == PUMPF_CTRL_RPM)   /* 控转速 */
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

        /* RPM转速换算为PWM百分比 */
        PUMPf_pwm_out = fun_line_value(rpm_sv, rpm_min, rpm_max, rpm_min_pwm_out, rpm_max_pwm_out); // 负相关
    }

    /* 变频水泵警告时，按最大转速运行 */
    if (PUMPF_is_warn())
    {
        PUMPf_pwm_out = rpm_max_pwm_out;
    }

    /* PWM百分比换算为RPM转速 */
    PUMPf_rpm_sv = fun_line_value(PUMPf_pwm_out, rpm_max_pwm_out, rpm_min_pwm_out, rpm_max, rpm_min);

    /* 获取水泵最大转速 */
    PUMPf_rpm_max = rpm_max;
}

/*******************************************************************************
 函 数: PUMPF_status()
 功 能: 
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void PUMPF_status(void)
{
    #define PWM_IN_DIFF            (5)      /* PWM百分比回差值: 0.5% */
    
    I16 pwm_sc; 
    I16 pwm_idle;
    I16 pwm_min, pwm_max;
    I16 pwm_warn;
    I16 pwm_err_1, pwm_err_2, pwm_err_3;
    I16 pwm_oc;
    I16 pwm_diff = PWM_IN_DIFF;

    /* 变频水泵未运行 */
    if (!fg_PUMPf_run)
    {
        PUMPf_err_id = PUMF_NO_ERR;
        PUMPf_sta = PUMPF_STA_IDLE;
    }

    switch (val_PUMPf_model)
    {   
        case PUMPF_00_PARA_9_IPWM1:         /* 威乐 */
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
        case PUMPF_01_APM25_9_130_PWM:      /* 新界 */
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
            
        case PUMPF_02_GPA25_9HW_130:        /* 新沪 */
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

        case PUMPF_03_APF25_12_130FPWM1:	/* 新界 APF25-12-130FPWM1 */
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

        case PUMPF_04_APF25_12_130PWM1:	/* 新界 APF25-12-130PWM1 */
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

         case PUMPF_05_GPA25_11H:	/* 新沪 GPA25_11H1 */
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
            
         case PUMPF_06_HBG_25_125_130P:    /* 奥姆特 HBG-25-125-130P */
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

         case PUMPF_07_UPM4XLK_25_90_130:   /* 格兰富 UPM4XLK 25-90 130 */
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

    /* 故障1: 短路 */
    if (pwm_sc != PUMPF_PWM_IN_NOEXIST
    && PUMPf_pwm_in >= pwm_sc-pwm_diff
    && PUMPf_pwm_in <= pwm_sc+pwm_diff
    )
    {
        PUMPf_err_id = PUMPF_ERR_01;
        PUMPf_sta = PUMPF_STA_SC;
    }

    /* 待机 */
    if (PUMPf_pwm_in >= pwm_idle-pwm_diff
    && PUMPf_pwm_in <= pwm_idle+pwm_diff
    )
    {
        PUMPf_err_id = PUMF_NO_ERR;
        PUMPf_sta = PUMPF_STA_IDLE;
    }

    /* 警告 */
    if (PUMPf_pwm_in >= pwm_warn-pwm_diff
    && PUMPf_pwm_in <= pwm_warn+pwm_diff
    )
    {
        PUMPf_err_id = PUMF_NO_ERR;
        PUMPf_sta = PUMPF_STA_WARN;
    }

    /* 运行 */
    if (PUMPf_pwm_in >= pwm_min-pwm_diff
    && PUMPf_pwm_in <= pwm_max+pwm_diff
    )
    {
        PUMPf_err_id = PUMF_NO_ERR;
        PUMPf_sta = PUMPF_STA_RUN;
    }

    /* 故障2 */
    if (PUMPf_pwm_in >= pwm_err_1-pwm_diff
    && PUMPf_pwm_in <= pwm_err_1+pwm_diff
    )
    {
        PUMPf_err_id = PUMPF_ERR_02;
        PUMPf_sta = PUMPF_STA_ERR;
    }

    /* 故障3 */
    if (PUMPf_pwm_in >= pwm_err_2-pwm_diff
    && PUMPf_pwm_in <= pwm_err_2+pwm_diff
    )
    {
        PUMPf_err_id = PUMPF_ERR_03;
        PUMPf_sta = PUMPF_STA_ERR;
    }

    /* 故障4 */
    if (PUMPf_pwm_in >= pwm_err_3-pwm_diff
    && PUMPf_pwm_in <= pwm_err_3+pwm_diff
    )
    {
        PUMPf_err_id = PUMPF_ERR_04;
        PUMPf_sta = PUMPF_STA_ERR;
    }

    /* 故障5: 断路 */
    if (pwm_oc != PUMPF_PWM_IN_NOEXIST
    && PUMPf_pwm_in >= pwm_oc-pwm_diff
    && PUMPf_pwm_in <= pwm_oc+pwm_diff
    )
    {
        PUMPf_err_id = PUMPF_ERR_05;
        PUMPf_sta = PUMPF_STA_OC;
    }

#if defined(USE_BMS_DEBUG_PUMPF_ERR)
    /* BMS调试触发变频水泵故障信息 */
    if (GET_BMS_DEBUG_VAL(BDV_PUMPF_ERR))
    {
        PUMPf_err_id = GET_BMS_DEBUG_VAL(BDV_PUMPF_ERR_CODE);
    }
#endif
}

/*******************************************************************************
 函 数: PUMPF_is_warn()
 功 能: 变频水泵警告
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
BOL PUMPF_is_warn(void)
{
#if defined(USE_BMS_DEBUG_PUMPF_ERR)
    /* BMS调试触发变频水泵警告 */
    if (GET_BMS_DEBUG_VAL(BDV_PUMPF_WARN))
    {
        return TRUE;
    }
#endif
    
    return (PUMPf_sta == PUMPF_STA_WARN) ? TRUE : FALSE;
}

/*******************************************************************************
 函 数: PUMPF_is_err()
 功 能: 变频水泵故障
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
BOL PUMPF_is_err(void)
{
#if defined(USE_BMS_DEBUG_PUMPF_ERR)
    /* BMS调试触发变频水泵故障 */
    if (GET_BMS_DEBUG_VAL(BDV_PUMPF_ERR))
    {
        return TRUE;
    }
#endif
    
    /* 仅得电时，不检测故障 */
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
 函 数: PUMPF_can_reset_err()
 功 能: 变频水泵故障可复位
 参 数: 空
 返 回: 
 其 它: 空
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
 函 数: PUMPF_init()
 功 能: 
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void PUMPF_init(void)
{
    fg_PUMPf_need = FALSE;
    fg_PUMPf_long_time_no_run = FALSE;
    fg_PUMPf_test_run = FALSE;
    fg_PUMPf_debug_run = FALSE;
    PUMPf_sta = PUMPF_STA_IDLE;
}

