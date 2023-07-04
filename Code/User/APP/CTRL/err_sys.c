#define ERR_SYS_GLOBAL
#include "includes.h"


/* ========================================================================================== */
/* 故障处理 */
#define M_MAX_ERR_TIME 20
static U16 sys_err_time[M_MAX_ERR_TIME];                /* 时间消抖(系统板) */
#define cnt_err_commu_UI35          (sys_err_time[ 0])
#define cnt_err_phase_sys           (sys_err_time[ 1])
#define cnt_err_commu_pcb_err0      (sys_err_time[ 2])
#define cnt_err_commu_pcb_err1      (sys_err_time[ 3])
#define cnt_err_commu_pcb_err2      (sys_err_time[ 4])
#define cnt_err_commu_pcb_err3      (sys_err_time[ 5])
#define cnt_err_commu_pcb_err4      (sys_err_time[ 6])
#define cnt_err_commu_pcb_err5      (sys_err_time[ 7])
#define cnt_err_commu_pcb_err6      (sys_err_time[ 8])
#define cnt_err_commu_pcb_err7      (sys_err_time[ 9])
#define cnt_err_commu_pcb_expt0     (sys_err_time[10])
#define cnt_err_commu_pcb_expt1     (sys_err_time[11])
#define cnt_err_commu_pcb_expt2     (sys_err_time[12])
#define cnt_err_commu_pcb_expt3     (sys_err_time[13])
#define cnt_err_commu_pcb_expt4     (sys_err_time[14])
#define cnt_err_commu_pcb_expt5     (sys_err_time[15])
#define cnt_err_commu_pcb_expt6     (sys_err_time[16])
#define cnt_err_commu_pcb_expt7     (sys_err_time[17])
#define cnt_err_ac_flux_water       (sys_err_time[18])
#define cnt_sys_err_pump_on         (sys_err_time[19])

#define cnt_err_commu_pcb_err_n(n)      (sys_err_time[n+2])
#define cnt_err_commu_pcb_expt_n(n)     (sys_err_time[n+10])

#define M_MAX_ERR_TIME_100MS 5
static U16 sys_err_time_100ms[M_MAX_ERR_TIME_100MS];    /* 时间消抖(100MS计时) */
#define cnt_err_tbh_ol              (sys_err_time_100ms[0])

/*******************************************************************************
 函 数: get_UI35_ctrl_type()
 功 能: 获取线控器控制类型
 参 数: 
 返 回: 
 其 它: 空
*******************************************************************************/
I16 get_UI35_ctrl_type(void)
{
     I16 UI35_ctrl_type = val_UI35_ctrl_type;
    
    /*[线控器通讯检测]为禁用，按[线控器控制]和线控器通讯故障标志确定能调控制方式
      "单区室温"检测到线控器通讯故障时，按"单区水温"运行。线控器通讯恢复时，恢复"单区室温"运行。
      "双区混合"检测到线控器通讯故障时，按"双区水温"运行。线控器通讯恢复时，恢复"双区混合"运行。*/
      
    /* 单区室温 切换成 单区水温 */
    if ((val_UI35_ctrl_type==UI35_ZONE_A_TR) && UI35_commu_err_no_stop_flag)
    {
        UI35_ctrl_type = UI35_ZONE_A_TWout2;
    }

    /* 双区水温&室温 切换成 双区水温 */
    if ((val_UI35_ctrl_type==UI35_DUAL_ZONE_TWout2_TR) && UI35_commu_err_no_stop_flag)
    {
        UI35_ctrl_type = UI35_DUAL_ZONE_TWout2;
    }

    return UI35_ctrl_type;
}

void err_sys_init(void)
{
    U08 i;

    /* 故障变量 */
    for (i=0; i<M_MAX_ERR_TIME; i++)
    {
        sys_err_time[i] = 0;
    }   

    for (i=0; i<M_MAX_ERR_TIME_100MS ;i++ )
    {
        sys_err_time_100ms[i] = 0;
    }

//    for (i=1; i<MAX_PCB; i++)
//    {
//        cnt_err_commu_pcb_expt_n(i) = 0xffff;
//    }
}

/*****************************************************
函数： error_get_unit_all
功能： 判断所有模块对应故障是否都发生
参数： 故障名称
返回值： TRUE 表示所有模块对应的故障已发生
         FALSE 表示不是所有模块对应的故障都发生
******************************************************/
//BOOL error_get_unit_all(U08 err_num) 
//{
//  U08 i;
//  U16 unit_error = 0;
//  BOOL fg_retval = TRUE;
//
//  /* 所有模块水流不足返回成功 */
//  if (err_num == S_ERR_WATER_FLOW)
//  {
//      unit_error = S_ERR_WATER_FLOW;              /* 水流不足 */
//      for (i = 0; i < val_pcb_num; i++)
//      {
//          if (m_get_error(i, S_ERR_CASCADE_COMMU)) continue;
//          /* 非防冻时混合检测水流不足和水流开关异常 */
//          if (!m_get_error(i, unit_error))                /* 水流不足 */
//          {
//              fg_retval = FALSE;
//              break;
//          }
//      }
//  }
//  return fg_retval;
//}

/*****************************************************
函数： all_unit_error_judge
功能： 判断所有模块是否存在某故障
参数： 故障号，期望的判断结果（都存在，任意一块存在）

返回值： TRUE   表示期望的判断结果成立
         FALSE  表示期望的判断结果成立
******************************************************/


BOOL all_unit_error_judge(U08 err_num, U08 attrib)
{
    U08 i;

    BOOL fg_retval;

    switch(attrib)
    {
        case ALL_EXIST: // 所有模块都存在此故障
            
            fg_retval = TRUE;
            for (i = 0; i < val_pcb_num; i++)
            {
                if (err_sys_get_s_commu_err(i)) continue;   // 通讯故障的模块不考虑

                if (!m_get_error(i, err_num))   // 有一块板不存在此故障，返回FALSE
                {
                    fg_retval = FALSE;
                    break;
                }
            }
            break;
            
        case ANYONE_EXIST:  // 任意一个模块存在该故障
            
            fg_retval = FALSE;
            for (i = 0; i < val_pcb_num; i++)
            {
                if (err_sys_get_s_commu_err(i)) continue;   // 通讯故障的模块不考虑

                if (m_get_error(i, err_num))    // 有一块板存在此故障，返回TRUE
                {
                    fg_retval = TRUE;
                    break;
                }
            }
            break;
        
        default:    // 其它非预期属性，返回FALSE
            fg_retval = FALSE;  
            break;
    }
    return fg_retval;
}

/*==========================================
    功能: 系统故障
/==========================================*/
void err_sys_judge(U08 pcb_num)
{
//    /* 变频水泵未开启 */
//    if (!get_output(MAX_PCB, M_DO_PUMPf))   
//    {
//        cnt_sys_err_pump_on = 0;
//    }

//    /* 系统水流开关: 水流不足检测 */
//    if (USE_SYS_WATER_FLOW)
//    {
//        if (GETBIT_N(fg_test_run, TEST_RUN_AIR_EMPTY)
//        || (GETBIT_N(fg_test_run, TEST_RUN_PUMP)&&!GETBIT_N(fg_test_pump_on, TEST_PUMP_CHECK_LDIW))
//        )                               
//        {
//            cnt_err_ac_flux_water = 0;      /* 系统排空调试，不检测水流 */
//        }
//        
//        if (cnt_sys_err_pump_on <= AC_PUMP_ON_DLY)     /* 水泵开启延时未到 */
//        {
//            cnt_err_ac_flux_water = 0;
//        }
//        else if (!m_get_input(M_IN_WATER_FLOW))             /* 未检测到水流开关信号 */
//        {
//            cnt_err_ac_flux_water = 0;
//        }
//        else if (cnt_err_ac_flux_water >= val_err_ac_flux_delay)    /* 水流检测时间到，水流不足报警 */
//        {
//            m_set_error(MAX_PCB, M_ERR_WATER_FLOW);
//        }
//    }
//    else
//    {
//        U08 fg_water_flow_err = TRUE;
//        U08 pcb;
//
//        for (pcb = 0; pcb<val_pcb_num; pcb++)
//        {
//            if (CELL_Is_used(pcb) && !m_get_error(pcb, S_ERR_CASCADE_COMMU))
//            {
//                if (!m_get_error(pcb, S_ERR_WATER_FLOW))
//                {
//                    fg_water_flow_err = FALSE;
//                    break;
//                }
//            } 
//        }
//
//        if (fg_water_flow_err)
//        {
//            m_set_error(MAX_PCB, M_ERR_WATER_FLOW);
//        }
//        else
//        {
//            /* 自动复位 */
//            m_clr_error(MAX_PCB, M_ERR_WATER_FLOW);
//        }
//    }
    
//    /* 变频水泵故障 */
//    if (USE_SYS_PUMPF)
//    {
//        if (cnt_sys_err_pump_on <= AC_PUMP_ON_DLY)      /* 水泵开启延时未到 */
//        {
//            cnt_sys_err_PUMPf = 0;
//        }
//        else if (speed_percent_PUMPf_in < PUMP_SET_ERR_FB_PERCENT)
//        {
//            cnt_sys_err_PUMPf = 0;
//        }
//        else if (cnt_sys_err_PUMPf >= 2)
//        {
//            m_set_error(MAX_PCB, M_ERR_PUMPF);
//        }
//    }
//    else
//    {
//        m_clr_error(MAX_PCB, M_ERR_PUMPF);
//    }

    /* 水箱电热过载 */
    if (val_hotw_used && val_TBH_used)
    {
        if (!m_get_input(M_DI_TBH_OL))
        {
            cnt_err_tbh_ol = 0;
        }
        else if (cnt_err_tbh_ol >= ERR_OL_TIME)
        {
            m_set_error(MAX_PCB, M_ERR_TBH_OL);
        }
    }
    else
    {
        m_clr_error(MAX_PCB, M_ERR_TBH_OL);
    }
#if 0
    // 内置电热过载
    if (!m_get_input(S_IN_IBH_OL))              cnt_err_ibh_ol = 0;
    else if (cnt_err_ibh_ol >= ERR_OL_TIME)     m_set_error(MAX_PCB, M_ERR_IBH_OL);

    // 水箱电热过载
    if (!m_get_input(IN_TBH_BACK))              cnt_err_tbh_ol = 0;
    else if (cnt_err_tbh_ol >= ERR_OL_TIME)     m_set_error(MAX_PCB, M_ERR_TBH_OL);
#endif

    /* 彩屏通讯故障*/
    //线控器通讯故障的主要影响无法下发室内温度，后板室内温度一直为中断时刻的值。
    //[线控器通讯检测]为使用时，报"线控器通讯故障"，停机组，手动复位。
    //[线控器通讯检测]为禁用时，不报"线控器通讯故障"，不停机组，UI35_commu_err_no_stop_flag赋值TRUE，
    //                          "单区室温"或"双区水温&室温"分别切换成"单区水温"或"双区水温"运行。
    if (SH_hmi_commu_succ != 0)
    {
        cnt_err_commu_UI35 = 0;
        UI35_commu_err_no_stop_flag = FALSE; 
        if (!val_use_UI35_commu_detect)
        {
           m_clr_error(MAX_PCB, M_ERR_UI35_COMMU_ERR);
        }
        
    }
    else if (cnt_err_commu_UI35 >= ERR_UI35_COMMU_ERR_TIME)
    {
        if (val_use_UI35_commu_detect)
        {
            UI35_commu_err_no_stop_flag = FALSE;
            m_set_error(MAX_PCB, M_ERR_UI35_COMMU_ERR);
        }
        else if (!val_use_UI35_commu_detect)
        {
           UI35_commu_err_no_stop_flag = TRUE;
           m_clr_error(MAX_PCB, M_ERR_UI35_COMMU_ERR);
        }
    }   

    /* 级联通讯故障 */
    {            
        /* 检测通讯异常：恢复后才检测，如果不恢复则要么是断电要么会报通讯故障 */
        {
            if (GETBIT(slave_commu_error, pcb_num))
            {
                /* 若已报通讯故障，则不再报通讯异常 */
                if (!m_get_error(pcb_num, S_ERR_CASCADE_COMMU))
                {
                    SET_BIT(fg_commu_err, pcb_num);
                }
                else
                {
                    CLR_BIT(fg_commu_err, pcb_num);
                }
                cnt_err_commu_pcb_expt_n(pcb_num) = 0;
                m_clr_error(pcb_num, S_ERR_COMMU_EXPT);
            }
            else            
            {
                if (GET_BIT(fg_commu_err, pcb_num))
                {
                    m_set_error(pcb_num, S_ERR_COMMU_EXPT);
                }
                
                if (cnt_err_commu_pcb_expt_n(pcb_num) > COMMU_EXPT_AUTO_RESET_DLY)
                {
                    m_clr_error(pcb_num, S_ERR_COMMU_EXPT);
                    CLR_BIT(fg_commu_err, pcb_num);
                }
            }
        }

        /* 检测通讯故障 */
        {
            if (!GETBIT(slave_commu_error, pcb_num))
            {
                cnt_err_commu_pcb_err_n(pcb_num) = 0;
                m_clr_error(pcb_num, S_ERR_CASCADE_COMMU);
            }
            else if (cnt_err_commu_pcb_err_n(pcb_num) >= COMMU_ERR_DETECT_DLY)
            {
                m_set_error(pcb_num, S_ERR_CASCADE_COMMU);
            }
        }
    }

    /* 三相电 错缺相保护 系统故障 */
	if (val_00_board_alarm == MOD_00_POWER_ERR)
	{
        m_clr_error(MAX_PCB, M_ERR_PHASE_REVERSE);
		m_clr_error(MAX_PCB, M_ERR_PHASE_LACK);
	}		
	else
    {
        #define ALARM_PHASE_NULL    (0)     // 无故障(用于记录相序故障)
        #define ALARM_PHASE_ERR     (1)     // 逆相(用于记录相序故障)
        #define ALARM_PHASE_LACK    (2)     // 缺相(用于记录相序故障)
        
        static U08 alarm_phase_bak = ALARM_PHASE_NULL;    // 记录相序故障

        switch (gIO.phase)
        {
            default:
            case PHASE_STA_OK:
                if (alarm_phase_bak == ALARM_PHASE_NULL) cnt_err_phase_sys = 0;
                break;
            case PHASE_STA_ERR:
                alarm_phase_bak = ALARM_PHASE_ERR;
                break;
            case PHASE_STA_LACK:
                alarm_phase_bak = ALARM_PHASE_LACK;
                break;
        }

        /* 刚上电马上报警，否则延时报警，防止断电时误报 */
        if (power_time < POWER_ON_JUST || cnt_err_phase_sys > POWER_DOWN_DLY)
        {
            if (alarm_phase_bak == ALARM_PHASE_ERR)
            {
                m_set_error(MAX_PCB, M_ERR_PHASE_REVERSE);        //逆相
                alarm_phase_bak = ALARM_PHASE_NULL;
            }
		    if (alarm_phase_bak == ALARM_PHASE_LACK)
		    {
		        m_set_error(MAX_PCB, M_ERR_PHASE_LACK);          	//缺相
		        alarm_phase_bak = ALARM_PHASE_NULL;
		    }
        }
	}
    
    /* 室外环境温度故障 */
//    if (SensorIsError(pv_sys_TA))  
//    {
//        m_set_error(MAX_PCB, M_ERR_SYS_ENVIRON); 
//    }
//    else
//    {
//        m_clr_error(MAX_PCB ,M_ERR_SYS_ENVIRON); 
//    } 

//    /* 系统回温故障 */
//  if (SensorIsError(pv_sys_temp_in))  
//  {
//      m_set_error(MAX_PCB, M_ERR_SYS_IN); 
//  }
//    else
//    {
//        m_clr_error(MAX_PCB ,M_ERR_SYS_IN); 
//    }
//
//    /* 系统出温故障 */
//  if (SensorIsError(pv_sys_temp_out))  
//  {
//      m_set_error(MAX_PCB, M_ERR_SYS_OUT); 
//  }
//    else
//    {
//        m_clr_error(MAX_PCB ,M_ERR_SYS_OUT); 
//    }

    /* 室内温度故障 */
    if (SensorIsError(pv_sys_TR))
    {
        m_set_error(MAX_PCB, M_ERR_TR);
    }
    else
    {
        m_clr_error(MAX_PCB, M_ERR_TR);
    }

    /* 热水水箱温度故障 */
    if (SensorIsError(pv_sys_THWt))  
    {
        m_set_error(MAX_PCB, M_ERR_THWT); 
    }
    else
    {
        m_clr_error(MAX_PCB ,M_ERR_THWT); 
    }

    /* 太阳能温度故障 */
    if (SensorIsError(pv_sys_TSL))
    {
        m_set_error(MAX_PCB, M_ERR_TSL); 
    }
    else
    {
        m_clr_error(MAX_PCB ,M_ERR_TSL); 
    }

    /* 空调总出水温度故障 */
    if (SensorIsError(pv_sys_TWout2_ac) && val_TWout2_sensor_used)
    {
        m_set_error(MAX_PCB, M_ERR_TWOUT2_AC_ERR);
    }
    else
    {
        m_clr_error(MAX_PCB ,M_ERR_TWOUT2_AC_ERR); 
    }
    
    /* 系统总出水温度故障 */
    if (SensorIsError(pv_sys_TWout3))  
    {
        m_set_error(MAX_PCB, M_ERR_TWOUT3); 
    }
    else
    {
        m_clr_error(MAX_PCB ,M_ERR_TWOUT3); 
    }

    /* 地暖总进温度故障 */
    if (SensorIsError(pv_sys_TFLin))  
    {
        m_set_error(MAX_PCB, M_ERR_TFLIN); 
    }
    else
    {
        m_clr_error(MAX_PCB ,M_ERR_TFLIN); 
    }

    /* 太阳能温度故障 目前仅报警 */
    if (SensorIsError(pv_sys_TSL))  
    {
        m_set_error(MAX_PCB, M_ERR_TSL); 
    }
    else
    {
        m_clr_error(MAX_PCB ,M_ERR_TSL); 
    }

    /* 缓冲水箱上部温度故障 */
    if (SensorIsError(pv_sys_TACt1))
    {
        m_set_error(MAX_PCB, M_ERR_TACT1);
    }
    else
    {
        m_clr_error(MAX_PCB, M_ERR_TACT1);
    }

    /* 缓冲水箱下部温度故障 */
    if (SensorIsError(pv_sys_TACt2))
    {
        m_set_error(MAX_PCB, M_ERR_TACT2);
    }
    else
    {
        m_clr_error(MAX_PCB, M_ERR_TACT2);
    }

//  
//    /* 散热器温度故障 目前仅报警 */
//  if (SensorIsError(pv_sys_temp_TF))  
//  {
//      m_set_error(MAX_PCB, M_ERR_TF); 
//  }
//    else
//    {
//        m_clr_error(MAX_PCB ,M_ERR_TF); 
//    }

//    /* 冷媒液侧温度故障 目前仅报警 */
//    if (SensorIsError(pv_sys_TRFliq))
//    {
//        m_set_error(MAX_PCB, M_ERR_TRFLIQ); 
//    }
//    else
//    {
//        m_clr_error(MAX_PCB, M_ERR_TRFLIQ); 
//    }

}

/*******************************************************************************
 函 数: err_sys_class()
 功 能: 处理系统故障动作
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void err_sys_class(U08 pcb_num)
{
    U08 i;
    
    if (err_sys_get_s_commu_err(pcb_num))			// 模块通讯故障或禁用 
	{
		for (i=0; i<MAX_RELAY; i++)
		{
			cell[pcb_num].cell_relay[i] = 0;     	// 通讯故障，清输出 
		}
		for (i=0; i<MAX_STATUS; i++)
		{
			cell[pcb_num].cell_status[i] = 0;    	// 通讯故障，清状态 
		}
	}
    
    /* 系统故障 */
    if (m_get_error(MAX_PCB, M_ERR_EEPROM)            /* EEPROM 数据错 */
    ||  m_get_error(MAX_PCB, M_ERR_PHASE_REVERSE)   // 错相保护
    ||  m_get_error(MAX_PCB, M_ERR_PHASE_LACK)      // 缺相保护
    || (gIO.phase == PHASE_STA_ERR && val_00_board_alarm == SYS_POWER_ERR)    /* 缺错相保护做延时报警处理，但依然及时保护 */
	|| (gIO.phase == PHASE_STA_LACK && val_00_board_alarm == SYS_POWER_ERR)
//  ||  s_get_error(S_ERR_POWER)                    /* 电源故障：因是单机系统，暂把模块故障放到这里来 */
        ) 
    {
        fg_pump_err = TRUE;
        fg_ac_sys_err = TRUE;
        fg_hotw_sys_err = TRUE;
        status_run = _ALARM;
    }
     
    /* 正常故障处理 */
    if (!fg_refri_recy_act)
    {
        /* 空调系统故障 */
        if ((USE_UI35_ROOM_TEMP_CTRL && m_get_error(MAX_PCB, M_ERR_TR))	    /* 线控器室温控制时，室内温度故障 */
        || (TACT1_AC_CTRL && m_get_error(MAX_PCB, M_ERR_TACT1))		        /* 使用TACT1能调时，TACT1温度故障 */
        || (TACT2_AC_CTRL && m_get_error(MAX_PCB, M_ERR_TACT2))		        /* 使用TACT2能调时，TACT2温度故障 */
        || (TACT1_TACT2_AC_CTRL \
           && m_get_error(MAX_PCB, M_ERR_TACT1) \
           && m_get_error(MAX_PCB, M_ERR_TACT2))			                /* 使用TACT1&TACT2能调时，TACT1和TACT2温度均故障 */
        || (AC_TWOUT2_AC_CTRL && m_get_error(MAX_PCB, M_ERR_TWOUT2_AC_ERR))	/* 使用空调总出水温度能调时，空调总出水温度温度故障 */
        )
        {
            fg_ac_sys_err = TRUE;
        }

        /* 热水系统故障 */
        if (m_get_error(MAX_PCB, M_ERR_THWT)                /* 热水水箱温度故障 */
        )
        {
            fg_hotw_sys_err = TRUE;
        }

        /* 空调系统故障，且热水系统故障 */
        if (m_get_error(MAX_PCB, M_ERR_UI35_COMMU_ERR)      /* 线控器通讯故障 */
        )
        {
            fg_ac_sys_err = TRUE;
            fg_hotw_sys_err = TRUE;
        }
    }
}

/*******************************************************************************
 函 数: err_sys_deal()
 功 能: 处理模块故障动作
 参 数: pcb_num--; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void err_sys_deal(U08 pcb_num)
{
    U08 j;
    
    if (status_run == _ALARM)  /* 停机组 */
    {
        ;
    }
    else        /* 其他部件故障 */
    {
        if (!err_sys_get_s_commu_err(pcb_num))   /* 无通讯故障时才做模块故障处理 */
        {
            /* 压缩机故障 */
            for (j=0; j<m_val_comp_num(pcb_num); j++) 
            {
                if (GET_SUB_SYS(pcb_num) == SUB_SYS_AC_HOTW)/* 空调+热水子系统 */
                {
                    if (!fg_zone_A_run && !fg_zone_B_run && !fg_hotw_run)       /* 空调未运行，且热水未运行 */
                    {
                        if (fg_ac_sys_err
                        || fg_hotw_sys_err
                        )
                        {
                            set_comp_err(j, pcb_num);
                        }
                    }
                    else if ((fg_zone_A_run || fg_zone_B_run) && !fg_hotw_run)  /* 空调运行，且热水未运行 */
                    {
                        if (fg_ac_sys_err)
                        {
                            set_comp_err(j, pcb_num);
                        }
                    }
                    else if (!fg_zone_A_run && !fg_zone_B_run && fg_hotw_run)   /* 空调未运行，且热水运行 */
                    {
                        if (fg_hotw_sys_err)
                        {
                            set_comp_err(j, pcb_num);
                        }
                    }
                    else                                                        /* 空调运行，且热水运行 */
                    {
                        if (fg_ac_sys_err
                        && fg_hotw_sys_err
                        )
                        {
                            set_comp_err(j, pcb_num);
                        }
                    }
                }
                else		                                /* 单空调子系统 */
                {
                    if (fg_ac_sys_err)
                    {
                        set_comp_err(j, pcb_num);
                    }
                }

                /* 故障停机处理 */
                if (get_comp_err(j, pcb_num))               /* 压机故障 */
                {
                    if (M_GET_CompMode(pcb_num, j) == MODE_COOL 
                        || M_GET_CompMode(pcb_num, j) == MODE_HEAT)
                    {
                        cmd_comp_quik_stop(pcb_num, j);                     
                    }
                }
            }
        }       
    }
}

/*******************************************************************************
 函 数: err_sys_flag_clr()
 功 能: 初始化各部件故障标志
 参 数: pcb_num--; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void err_sys_flag_clr(U08 pcb_num)
{
    U08 i;
    
//  if (pcb_num > MAX_PCB)     return;
        
    if (status_run == _ALARM)
    {
        status_run = _IDLE;
    }
    
//  fg_stop_all_mac = FALSE;
    fg_pump_err = FALSE;
    fg_ac_sys_err = FALSE;
    fg_hotw_sys_err = FALSE;

    
    for (i=0; i<m_val_comp_num(pcb_num); i++)
    {
        clr_comp_err(i, pcb_num);
    }
}

/*******************************************************************************
 函 数: err_sys_auto_reset()
 功 能: 系统故障自动复位
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void err_sys_auto_reset(void)   /* N0004 */
{

}

/*******************************************************************************
 函 数: err_sys_limit_auto_reset()
 功 能: 系统故障限制型自动复位
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void err_sys_limit_auto_reset(void)  
{

    /* 三相电检测 */
    if (err_limit_auto_reset_deal(AUTOERR_M_PS_LACK, m_get_error(MAX_PCB, M_ERR_PHASE_LACK), (gIO.phase==PHASE_STA_LACK)))//缺相保护
    {
        m_clr_error(MAX_PCB, M_ERR_PHASE_LACK);
    }

    if (err_limit_auto_reset_deal(AUTOERR_M_PS_REV, m_get_error(MAX_PCB, M_ERR_PHASE_REVERSE),(gIO.phase==PHASE_STA_ERR)))//错相保护
    {
        m_clr_error(MAX_PCB, M_ERR_PHASE_REVERSE);
    }

}

/******************************************************
函数：Error_reset
功能；处理故障复位
参数：无
返回值：无
******************************************************/
void err_sys_manual_reset(void)
{
    U08 i,j;
    
    if (gIO.phase != PHASE_STA_ERR)     m_clr_error(MAX_PCB,M_ERR_PHASE_REVERSE);   //逆相
    if (gIO.phase != PHASE_STA_LACK)    m_clr_error(MAX_PCB,M_ERR_PHASE_LACK);      //缺相

    
//    m_clr_error(MAX_PCB, M_ERR_LACK_FAN);   //送风压差
//    m_clr_error(MAX_PCB, M_ERR_WATER_FLOW); //水流不足
    m_clr_error(MAX_PCB, M_ERR_TBH_OL);     //水箱电热过载

//    if (speed_percent_PUMPf_in < PUMP_CLR_ERR_FB_PERCENT)
//    {
//        m_clr_error(MAX_PCB, M_ERR_PUMPF);      //变频水泵故障
//    }

    m_clr_error(MAX_PCB, M_ERR_UI35_COMMU_ERR);				/* 线控器通讯故障 */

    /* 其他故障 */
    for (i=0; i<=MAX_PCB; i++)                      /* 通讯故障 */
    {
        m_clr_error(i, S_ERR_CASCADE_COMMU);
    }
    
    /* 未设置的模块故障清零 */
    for (i=val_pcb_num; i<MAX_PCB; i++)              /*  */
    {
        for (j=0; j<MAX_ERR; j++)
        {
            cell[i].cell_error[j] = 0;
        }
    }

    slave_commu_error = 0;
}
/*==========================================
    功能: 故障总接口
/==========================================*/
void err_sys(U08 pcb_num)
{
//  U08 i,j;
//  static U08 m = 0;

    err_sys_time();                 //计时

    err_sys_auto_reset();           //部分系统故障自动复位

    err_sys_limit_auto_reset();     //部分系统故障限制型自动复位

    if (key_reset)                  /* 手动复位*/
    {
        cmd_err_reset();            /* 故障复位 */
        
        err_sys_manual_reset();         //部分系统故障手动复位
#ifdef COM_FCU_FBUS         
        fcu_err_reset(); 
#endif
    }
    
    if (power_time < POWER_ON_INTO_WORK) return;        /* 上电前10秒不检测故障 */  
    
    
    err_sys_judge(pcb_num);             /* 系统故障检测 */
    
    err_sys_flag_clr(pcb_num);          // 标志清理
    err_sys_class(pcb_num);                    /* 系统故障归类 */
    err_sys_deal(pcb_num);              /* 系统故障处理 */
}

/*==========================================
    功能: 计时
/==========================================*/
void err_sys_time(void)
{
    U08 i;
        
    if (F_TIME_BASIC_100MS_BACK)
    {
        for (i=0; i<M_MAX_ERR_TIME_100MS; i++) 
        {
            if (sys_err_time_100ms[i] != 0xffff) sys_err_time_100ms[i]++;
        }
    }
    
    if (F_TIME_BASIC_SEC_BACK)
    {
//      if (screen_time < 0xfffe) screen_time++;

        /* 故障检测时间 */
        for (i=0; i<M_MAX_ERR_TIME; i++) 
        {
            if (sys_err_time[i] != 0xffff) sys_err_time[i]++;
        }
    }
}

/*******************************************************************************
 函 数: err_sys_get_s_commu_err()
 功 能: 获取模块通讯故障
 参 数: pcb--模块号; 
 返 回: 
 其 它: 空
*******************************************************************************/
BOL err_sys_get_s_commu_err(U08 pcb)
{
    BOL fg_err = FALSE;

    if (CELL_Is_used(pcb))
    {
        if (m_get_error(pcb, S_ERR_CASCADE_COMMU)
        ||  GETBIT(slave_commu_error, pcb)
            )
        {
            fg_err = TRUE;
        }
    }

    return fg_err;
}

