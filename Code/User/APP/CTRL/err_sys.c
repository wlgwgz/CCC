#define ERR_SYS_GLOBAL
#include "includes.h"


/* ========================================================================================== */
/* ���ϴ��� */
#define M_MAX_ERR_TIME 20
static U16 sys_err_time[M_MAX_ERR_TIME];                /* ʱ������(ϵͳ��) */
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
static U16 sys_err_time_100ms[M_MAX_ERR_TIME_100MS];    /* ʱ������(100MS��ʱ) */
#define cnt_err_tbh_ol              (sys_err_time_100ms[0])

/*******************************************************************************
 �� ��: get_UI35_ctrl_type()
 �� ��: ��ȡ�߿�����������
 �� ��: 
 �� ��: 
 �� ��: ��
*******************************************************************************/
I16 get_UI35_ctrl_type(void)
{
     I16 UI35_ctrl_type = val_UI35_ctrl_type;
    
    /*[�߿���ͨѶ���]Ϊ���ã���[�߿�������]���߿���ͨѶ���ϱ�־ȷ���ܵ����Ʒ�ʽ
      "��������"��⵽�߿���ͨѶ����ʱ����"����ˮ��"���С��߿���ͨѶ�ָ�ʱ���ָ�"��������"���С�
      "˫�����"��⵽�߿���ͨѶ����ʱ����"˫��ˮ��"���С��߿���ͨѶ�ָ�ʱ���ָ�"˫�����"���С�*/
      
    /* �������� �л��� ����ˮ�� */
    if ((val_UI35_ctrl_type==UI35_ZONE_A_TR) && UI35_commu_err_no_stop_flag)
    {
        UI35_ctrl_type = UI35_ZONE_A_TWout2;
    }

    /* ˫��ˮ��&���� �л��� ˫��ˮ�� */
    if ((val_UI35_ctrl_type==UI35_DUAL_ZONE_TWout2_TR) && UI35_commu_err_no_stop_flag)
    {
        UI35_ctrl_type = UI35_DUAL_ZONE_TWout2;
    }

    return UI35_ctrl_type;
}

void err_sys_init(void)
{
    U08 i;

    /* ���ϱ��� */
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
������ error_get_unit_all
���ܣ� �ж�����ģ���Ӧ�����Ƿ񶼷���
������ ��������
����ֵ�� TRUE ��ʾ����ģ���Ӧ�Ĺ����ѷ���
         FALSE ��ʾ��������ģ���Ӧ�Ĺ��϶�����
******************************************************/
//BOOL error_get_unit_all(U08 err_num) 
//{
//  U08 i;
//  U16 unit_error = 0;
//  BOOL fg_retval = TRUE;
//
//  /* ����ģ��ˮ�����㷵�سɹ� */
//  if (err_num == S_ERR_WATER_FLOW)
//  {
//      unit_error = S_ERR_WATER_FLOW;              /* ˮ������ */
//      for (i = 0; i < val_pcb_num; i++)
//      {
//          if (m_get_error(i, S_ERR_CASCADE_COMMU)) continue;
//          /* �Ƿ���ʱ��ϼ��ˮ�������ˮ�������쳣 */
//          if (!m_get_error(i, unit_error))                /* ˮ������ */
//          {
//              fg_retval = FALSE;
//              break;
//          }
//      }
//  }
//  return fg_retval;
//}

/*****************************************************
������ all_unit_error_judge
���ܣ� �ж�����ģ���Ƿ����ĳ����
������ ���Ϻţ��������жϽ���������ڣ�����һ����ڣ�

����ֵ�� TRUE   ��ʾ�������жϽ������
         FALSE  ��ʾ�������жϽ������
******************************************************/


BOOL all_unit_error_judge(U08 err_num, U08 attrib)
{
    U08 i;

    BOOL fg_retval;

    switch(attrib)
    {
        case ALL_EXIST: // ����ģ�鶼���ڴ˹���
            
            fg_retval = TRUE;
            for (i = 0; i < val_pcb_num; i++)
            {
                if (err_sys_get_s_commu_err(i)) continue;   // ͨѶ���ϵ�ģ�鲻����

                if (!m_get_error(i, err_num))   // ��һ��岻���ڴ˹��ϣ�����FALSE
                {
                    fg_retval = FALSE;
                    break;
                }
            }
            break;
            
        case ANYONE_EXIST:  // ����һ��ģ����ڸù���
            
            fg_retval = FALSE;
            for (i = 0; i < val_pcb_num; i++)
            {
                if (err_sys_get_s_commu_err(i)) continue;   // ͨѶ���ϵ�ģ�鲻����

                if (m_get_error(i, err_num))    // ��һ�����ڴ˹��ϣ�����TRUE
                {
                    fg_retval = TRUE;
                    break;
                }
            }
            break;
        
        default:    // ������Ԥ�����ԣ�����FALSE
            fg_retval = FALSE;  
            break;
    }
    return fg_retval;
}

/*==========================================
    ����: ϵͳ����
/==========================================*/
void err_sys_judge(U08 pcb_num)
{
//    /* ��Ƶˮ��δ���� */
//    if (!get_output(MAX_PCB, M_DO_PUMPf))   
//    {
//        cnt_sys_err_pump_on = 0;
//    }

//    /* ϵͳˮ������: ˮ�������� */
//    if (USE_SYS_WATER_FLOW)
//    {
//        if (GETBIT_N(fg_test_run, TEST_RUN_AIR_EMPTY)
//        || (GETBIT_N(fg_test_run, TEST_RUN_PUMP)&&!GETBIT_N(fg_test_pump_on, TEST_PUMP_CHECK_LDIW))
//        )                               
//        {
//            cnt_err_ac_flux_water = 0;      /* ϵͳ�ſյ��ԣ������ˮ�� */
//        }
//        
//        if (cnt_sys_err_pump_on <= AC_PUMP_ON_DLY)     /* ˮ�ÿ�����ʱδ�� */
//        {
//            cnt_err_ac_flux_water = 0;
//        }
//        else if (!m_get_input(M_IN_WATER_FLOW))             /* δ��⵽ˮ�������ź� */
//        {
//            cnt_err_ac_flux_water = 0;
//        }
//        else if (cnt_err_ac_flux_water >= val_err_ac_flux_delay)    /* ˮ�����ʱ�䵽��ˮ�����㱨�� */
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
//            /* �Զ���λ */
//            m_clr_error(MAX_PCB, M_ERR_WATER_FLOW);
//        }
//    }
    
//    /* ��Ƶˮ�ù��� */
//    if (USE_SYS_PUMPF)
//    {
//        if (cnt_sys_err_pump_on <= AC_PUMP_ON_DLY)      /* ˮ�ÿ�����ʱδ�� */
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

    /* ˮ����ȹ��� */
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
    // ���õ��ȹ���
    if (!m_get_input(S_IN_IBH_OL))              cnt_err_ibh_ol = 0;
    else if (cnt_err_ibh_ol >= ERR_OL_TIME)     m_set_error(MAX_PCB, M_ERR_IBH_OL);

    // ˮ����ȹ���
    if (!m_get_input(IN_TBH_BACK))              cnt_err_tbh_ol = 0;
    else if (cnt_err_tbh_ol >= ERR_OL_TIME)     m_set_error(MAX_PCB, M_ERR_TBH_OL);
#endif

    /* ����ͨѶ����*/
    //�߿���ͨѶ���ϵ���ҪӰ���޷��·������¶ȣ���������¶�һֱΪ�ж�ʱ�̵�ֵ��
    //[�߿���ͨѶ���]Ϊʹ��ʱ����"�߿���ͨѶ����"��ͣ���飬�ֶ���λ��
    //[�߿���ͨѶ���]Ϊ����ʱ������"�߿���ͨѶ����"����ͣ���飬UI35_commu_err_no_stop_flag��ֵTRUE��
    //                          "��������"��"˫��ˮ��&����"�ֱ��л���"����ˮ��"��"˫��ˮ��"���С�
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

    /* ����ͨѶ���� */
    {            
        /* ���ͨѶ�쳣���ָ���ż�⣬������ָ���Ҫô�Ƕϵ�Ҫô�ᱨͨѶ���� */
        {
            if (GETBIT(slave_commu_error, pcb_num))
            {
                /* ���ѱ�ͨѶ���ϣ����ٱ�ͨѶ�쳣 */
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

        /* ���ͨѶ���� */
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

    /* ����� ��ȱ�ౣ�� ϵͳ���� */
	if (val_00_board_alarm == MOD_00_POWER_ERR)
	{
        m_clr_error(MAX_PCB, M_ERR_PHASE_REVERSE);
		m_clr_error(MAX_PCB, M_ERR_PHASE_LACK);
	}		
	else
    {
        #define ALARM_PHASE_NULL    (0)     // �޹���(���ڼ�¼�������)
        #define ALARM_PHASE_ERR     (1)     // ����(���ڼ�¼�������)
        #define ALARM_PHASE_LACK    (2)     // ȱ��(���ڼ�¼�������)
        
        static U08 alarm_phase_bak = ALARM_PHASE_NULL;    // ��¼�������

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

        /* ���ϵ����ϱ�����������ʱ��������ֹ�ϵ�ʱ�� */
        if (power_time < POWER_ON_JUST || cnt_err_phase_sys > POWER_DOWN_DLY)
        {
            if (alarm_phase_bak == ALARM_PHASE_ERR)
            {
                m_set_error(MAX_PCB, M_ERR_PHASE_REVERSE);        //����
                alarm_phase_bak = ALARM_PHASE_NULL;
            }
		    if (alarm_phase_bak == ALARM_PHASE_LACK)
		    {
		        m_set_error(MAX_PCB, M_ERR_PHASE_LACK);          	//ȱ��
		        alarm_phase_bak = ALARM_PHASE_NULL;
		    }
        }
	}
    
    /* ���⻷���¶ȹ��� */
//    if (SensorIsError(pv_sys_TA))  
//    {
//        m_set_error(MAX_PCB, M_ERR_SYS_ENVIRON); 
//    }
//    else
//    {
//        m_clr_error(MAX_PCB ,M_ERR_SYS_ENVIRON); 
//    } 

//    /* ϵͳ���¹��� */
//  if (SensorIsError(pv_sys_temp_in))  
//  {
//      m_set_error(MAX_PCB, M_ERR_SYS_IN); 
//  }
//    else
//    {
//        m_clr_error(MAX_PCB ,M_ERR_SYS_IN); 
//    }
//
//    /* ϵͳ���¹��� */
//  if (SensorIsError(pv_sys_temp_out))  
//  {
//      m_set_error(MAX_PCB, M_ERR_SYS_OUT); 
//  }
//    else
//    {
//        m_clr_error(MAX_PCB ,M_ERR_SYS_OUT); 
//    }

    /* �����¶ȹ��� */
    if (SensorIsError(pv_sys_TR))
    {
        m_set_error(MAX_PCB, M_ERR_TR);
    }
    else
    {
        m_clr_error(MAX_PCB, M_ERR_TR);
    }

    /* ��ˮˮ���¶ȹ��� */
    if (SensorIsError(pv_sys_THWt))  
    {
        m_set_error(MAX_PCB, M_ERR_THWT); 
    }
    else
    {
        m_clr_error(MAX_PCB ,M_ERR_THWT); 
    }

    /* ̫�����¶ȹ��� */
    if (SensorIsError(pv_sys_TSL))
    {
        m_set_error(MAX_PCB, M_ERR_TSL); 
    }
    else
    {
        m_clr_error(MAX_PCB ,M_ERR_TSL); 
    }

    /* �յ��ܳ�ˮ�¶ȹ��� */
    if (SensorIsError(pv_sys_TWout2_ac) && val_TWout2_sensor_used)
    {
        m_set_error(MAX_PCB, M_ERR_TWOUT2_AC_ERR);
    }
    else
    {
        m_clr_error(MAX_PCB ,M_ERR_TWOUT2_AC_ERR); 
    }
    
    /* ϵͳ�ܳ�ˮ�¶ȹ��� */
    if (SensorIsError(pv_sys_TWout3))  
    {
        m_set_error(MAX_PCB, M_ERR_TWOUT3); 
    }
    else
    {
        m_clr_error(MAX_PCB ,M_ERR_TWOUT3); 
    }

    /* ��ů�ܽ��¶ȹ��� */
    if (SensorIsError(pv_sys_TFLin))  
    {
        m_set_error(MAX_PCB, M_ERR_TFLIN); 
    }
    else
    {
        m_clr_error(MAX_PCB ,M_ERR_TFLIN); 
    }

    /* ̫�����¶ȹ��� Ŀǰ������ */
    if (SensorIsError(pv_sys_TSL))  
    {
        m_set_error(MAX_PCB, M_ERR_TSL); 
    }
    else
    {
        m_clr_error(MAX_PCB ,M_ERR_TSL); 
    }

    /* ����ˮ���ϲ��¶ȹ��� */
    if (SensorIsError(pv_sys_TACt1))
    {
        m_set_error(MAX_PCB, M_ERR_TACT1);
    }
    else
    {
        m_clr_error(MAX_PCB, M_ERR_TACT1);
    }

    /* ����ˮ���²��¶ȹ��� */
    if (SensorIsError(pv_sys_TACt2))
    {
        m_set_error(MAX_PCB, M_ERR_TACT2);
    }
    else
    {
        m_clr_error(MAX_PCB, M_ERR_TACT2);
    }

//  
//    /* ɢ�����¶ȹ��� Ŀǰ������ */
//  if (SensorIsError(pv_sys_temp_TF))  
//  {
//      m_set_error(MAX_PCB, M_ERR_TF); 
//  }
//    else
//    {
//        m_clr_error(MAX_PCB ,M_ERR_TF); 
//    }

//    /* ��ýҺ���¶ȹ��� Ŀǰ������ */
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
 �� ��: err_sys_class()
 �� ��: ����ϵͳ���϶���
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void err_sys_class(U08 pcb_num)
{
    U08 i;
    
    if (err_sys_get_s_commu_err(pcb_num))			// ģ��ͨѶ���ϻ���� 
	{
		for (i=0; i<MAX_RELAY; i++)
		{
			cell[pcb_num].cell_relay[i] = 0;     	// ͨѶ���ϣ������ 
		}
		for (i=0; i<MAX_STATUS; i++)
		{
			cell[pcb_num].cell_status[i] = 0;    	// ͨѶ���ϣ���״̬ 
		}
	}
    
    /* ϵͳ���� */
    if (m_get_error(MAX_PCB, M_ERR_EEPROM)            /* EEPROM ���ݴ� */
    ||  m_get_error(MAX_PCB, M_ERR_PHASE_REVERSE)   // ���ౣ��
    ||  m_get_error(MAX_PCB, M_ERR_PHASE_LACK)      // ȱ�ౣ��
    || (gIO.phase == PHASE_STA_ERR && val_00_board_alarm == SYS_POWER_ERR)    /* ȱ���ౣ������ʱ������������Ȼ��ʱ���� */
	|| (gIO.phase == PHASE_STA_LACK && val_00_board_alarm == SYS_POWER_ERR)
//  ||  s_get_error(S_ERR_POWER)                    /* ��Դ���ϣ����ǵ���ϵͳ���ݰ�ģ����Ϸŵ������� */
        ) 
    {
        fg_pump_err = TRUE;
        fg_ac_sys_err = TRUE;
        fg_hotw_sys_err = TRUE;
        status_run = _ALARM;
    }
     
    /* �������ϴ��� */
    if (!fg_refri_recy_act)
    {
        /* �յ�ϵͳ���� */
        if ((USE_UI35_ROOM_TEMP_CTRL && m_get_error(MAX_PCB, M_ERR_TR))	    /* �߿������¿���ʱ�������¶ȹ��� */
        || (TACT1_AC_CTRL && m_get_error(MAX_PCB, M_ERR_TACT1))		        /* ʹ��TACT1�ܵ�ʱ��TACT1�¶ȹ��� */
        || (TACT2_AC_CTRL && m_get_error(MAX_PCB, M_ERR_TACT2))		        /* ʹ��TACT2�ܵ�ʱ��TACT2�¶ȹ��� */
        || (TACT1_TACT2_AC_CTRL \
           && m_get_error(MAX_PCB, M_ERR_TACT1) \
           && m_get_error(MAX_PCB, M_ERR_TACT2))			                /* ʹ��TACT1&TACT2�ܵ�ʱ��TACT1��TACT2�¶Ⱦ����� */
        || (AC_TWOUT2_AC_CTRL && m_get_error(MAX_PCB, M_ERR_TWOUT2_AC_ERR))	/* ʹ�ÿյ��ܳ�ˮ�¶��ܵ�ʱ���յ��ܳ�ˮ�¶��¶ȹ��� */
        )
        {
            fg_ac_sys_err = TRUE;
        }

        /* ��ˮϵͳ���� */
        if (m_get_error(MAX_PCB, M_ERR_THWT)                /* ��ˮˮ���¶ȹ��� */
        )
        {
            fg_hotw_sys_err = TRUE;
        }

        /* �յ�ϵͳ���ϣ�����ˮϵͳ���� */
        if (m_get_error(MAX_PCB, M_ERR_UI35_COMMU_ERR)      /* �߿���ͨѶ���� */
        )
        {
            fg_ac_sys_err = TRUE;
            fg_hotw_sys_err = TRUE;
        }
    }
}

/*******************************************************************************
 �� ��: err_sys_deal()
 �� ��: ����ģ����϶���
 �� ��: pcb_num--; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void err_sys_deal(U08 pcb_num)
{
    U08 j;
    
    if (status_run == _ALARM)  /* ͣ���� */
    {
        ;
    }
    else        /* ������������ */
    {
        if (!err_sys_get_s_commu_err(pcb_num))   /* ��ͨѶ����ʱ����ģ����ϴ��� */
        {
            /* ѹ�������� */
            for (j=0; j<m_val_comp_num(pcb_num); j++) 
            {
                if (GET_SUB_SYS(pcb_num) == SUB_SYS_AC_HOTW)/* �յ�+��ˮ��ϵͳ */
                {
                    if (!fg_zone_A_run && !fg_zone_B_run && !fg_hotw_run)       /* �յ�δ���У�����ˮδ���� */
                    {
                        if (fg_ac_sys_err
                        || fg_hotw_sys_err
                        )
                        {
                            set_comp_err(j, pcb_num);
                        }
                    }
                    else if ((fg_zone_A_run || fg_zone_B_run) && !fg_hotw_run)  /* �յ����У�����ˮδ���� */
                    {
                        if (fg_ac_sys_err)
                        {
                            set_comp_err(j, pcb_num);
                        }
                    }
                    else if (!fg_zone_A_run && !fg_zone_B_run && fg_hotw_run)   /* �յ�δ���У�����ˮ���� */
                    {
                        if (fg_hotw_sys_err)
                        {
                            set_comp_err(j, pcb_num);
                        }
                    }
                    else                                                        /* �յ����У�����ˮ���� */
                    {
                        if (fg_ac_sys_err
                        && fg_hotw_sys_err
                        )
                        {
                            set_comp_err(j, pcb_num);
                        }
                    }
                }
                else		                                /* ���յ���ϵͳ */
                {
                    if (fg_ac_sys_err)
                    {
                        set_comp_err(j, pcb_num);
                    }
                }

                /* ����ͣ������ */
                if (get_comp_err(j, pcb_num))               /* ѹ������ */
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
 �� ��: err_sys_flag_clr()
 �� ��: ��ʼ�����������ϱ�־
 �� ��: pcb_num--; 
 �� ��: ��
 �� ��: ��
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
 �� ��: err_sys_auto_reset()
 �� ��: ϵͳ�����Զ���λ
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void err_sys_auto_reset(void)   /* N0004 */
{

}

/*******************************************************************************
 �� ��: err_sys_limit_auto_reset()
 �� ��: ϵͳ�����������Զ���λ
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void err_sys_limit_auto_reset(void)  
{

    /* ������� */
    if (err_limit_auto_reset_deal(AUTOERR_M_PS_LACK, m_get_error(MAX_PCB, M_ERR_PHASE_LACK), (gIO.phase==PHASE_STA_LACK)))//ȱ�ౣ��
    {
        m_clr_error(MAX_PCB, M_ERR_PHASE_LACK);
    }

    if (err_limit_auto_reset_deal(AUTOERR_M_PS_REV, m_get_error(MAX_PCB, M_ERR_PHASE_REVERSE),(gIO.phase==PHASE_STA_ERR)))//���ౣ��
    {
        m_clr_error(MAX_PCB, M_ERR_PHASE_REVERSE);
    }

}

/******************************************************
������Error_reset
���ܣ�������ϸ�λ
��������
����ֵ����
******************************************************/
void err_sys_manual_reset(void)
{
    U08 i,j;
    
    if (gIO.phase != PHASE_STA_ERR)     m_clr_error(MAX_PCB,M_ERR_PHASE_REVERSE);   //����
    if (gIO.phase != PHASE_STA_LACK)    m_clr_error(MAX_PCB,M_ERR_PHASE_LACK);      //ȱ��

    
//    m_clr_error(MAX_PCB, M_ERR_LACK_FAN);   //�ͷ�ѹ��
//    m_clr_error(MAX_PCB, M_ERR_WATER_FLOW); //ˮ������
    m_clr_error(MAX_PCB, M_ERR_TBH_OL);     //ˮ����ȹ���

//    if (speed_percent_PUMPf_in < PUMP_CLR_ERR_FB_PERCENT)
//    {
//        m_clr_error(MAX_PCB, M_ERR_PUMPF);      //��Ƶˮ�ù���
//    }

    m_clr_error(MAX_PCB, M_ERR_UI35_COMMU_ERR);				/* �߿���ͨѶ���� */

    /* �������� */
    for (i=0; i<=MAX_PCB; i++)                      /* ͨѶ���� */
    {
        m_clr_error(i, S_ERR_CASCADE_COMMU);
    }
    
    /* δ���õ�ģ��������� */
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
    ����: �����ܽӿ�
/==========================================*/
void err_sys(U08 pcb_num)
{
//  U08 i,j;
//  static U08 m = 0;

    err_sys_time();                 //��ʱ

    err_sys_auto_reset();           //����ϵͳ�����Զ���λ

    err_sys_limit_auto_reset();     //����ϵͳ�����������Զ���λ

    if (key_reset)                  /* �ֶ���λ*/
    {
        cmd_err_reset();            /* ���ϸ�λ */
        
        err_sys_manual_reset();         //����ϵͳ�����ֶ���λ
#ifdef COM_FCU_FBUS         
        fcu_err_reset(); 
#endif
    }
    
    if (power_time < POWER_ON_INTO_WORK) return;        /* �ϵ�ǰ10�벻������ */  
    
    
    err_sys_judge(pcb_num);             /* ϵͳ���ϼ�� */
    
    err_sys_flag_clr(pcb_num);          // ��־����
    err_sys_class(pcb_num);                    /* ϵͳ���Ϲ��� */
    err_sys_deal(pcb_num);              /* ϵͳ���ϴ��� */
}

/*==========================================
    ����: ��ʱ
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

        /* ���ϼ��ʱ�� */
        for (i=0; i<M_MAX_ERR_TIME; i++) 
        {
            if (sys_err_time[i] != 0xffff) sys_err_time[i]++;
        }
    }
}

/*******************************************************************************
 �� ��: err_sys_get_s_commu_err()
 �� ��: ��ȡģ��ͨѶ����
 �� ��: pcb--ģ���; 
 �� ��: 
 �� ��: ��
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

