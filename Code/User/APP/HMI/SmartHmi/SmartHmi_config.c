/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: SmartHmi_config.c
 �ļ�����: �������ʱ��Ҫ�����޸ĵ�����
 ������  : lhb
 ��������: 2021.05.08
*******************************************************************************/
#include "includes.h"
#include "SmartHmi_commu_info.h"

#define SHC_USE_COMP USE_COMP
extern const U16 msg_invt_error[];
extern const U16 msg_invt_error_code[];

void SHC_test_do_timer(void);
U08 cnt_test_do_IBH1;
U08 cnt_test_do_IBH2;
U08 cnt_test_do_TBH;
U08 cnt_test_do_HT;

/***********************************************************************
 �� ��: �յ����ػ�������Ч(�޷����߿�����������ִ��A/B������)
 �� ��: ��
************************************************************************/
BOL SHC_ac_on_off_disabled(void)
{
    if (fg_floor_preheat_run                /* ��ůԤ�� */
    || fg_floor_dryup_run	                /* ��ů��� */
    || USE_UIRT_ZONE_A_MODE_SWITCH			/* �¿���-����ģʽ�л� */
    || USE_UIRT_ZONE_A_ON_OFF				/* �¿���-�������� */
    || USE_UIRT_DUAL_ZONE_ON_OFF			/* �¿���-˫������ */
    )
    {
        return TRUE;
    }

    return FALSE;
}

/***********************************************************************
 �� ��: ������ҿ��ػ�������Ч(�޷����߿�����������ִ��A/B������)
 �� ��: ��
************************************************************************/
BOL SHC_holiday_away_on_off_disabled(void)
{
    if (fg_floor_preheat_run                /* ��ůԤ�� */
    || fg_floor_dryup_run	                /* ��ů��� */
    )
    {
        return TRUE;
    }

    return FALSE;
}

/***********************************************************************
 �� ��: ��ˮ���ػ�������Ч(�޷����߿�����������ִ����ˮ����)
 �� ��: ��
************************************************************************/
BOL SHC_hotw_on_off_disabled(void)
{
    if (fg_floor_preheat_run                /* ��ůԤ�� */
    || fg_floor_dryup_run	                /* ��ů��� */
    )
    {
        return TRUE;
    }

    return FALSE;
}

/***********************************************************************
 �� ��: ģʽ�л�������Ч(�޷����߿��������������ÿյ�ģʽ)
 �� ��: ��
************************************************************************/
BOL SHC_ac_mode_switch_disabled(void)
{
    if (fg_floor_preheat_run                                /* ��ůԤ�� */
    || fg_floor_dryup_run	                                /* ��ů��� */
    || USE_UIRT_ZONE_A_MODE_SWITCH			                /* �¿���-����ģʽ�л� */
    || GETBIT_N(fg_test_run, TEST_RUN_COOL_MODE)			/* ����������ʱ */
    || GETBIT_N(fg_test_run, TEST_RUN_HEAT_MODE)			/* ����������ʱ */
    )
    {
        return TRUE;
    }

    return FALSE;
}

/***********************************************************************
 �� ��: SmartHmi_config.c�ڵı�����ʼ��
 �� ��: ��
************************************************************************/
void SHC_init(void)
{

}

/***********************************************************************
 �� ��: SmartHmi_config.c�ڵļ�ʱ
 �� ��: ��
************************************************************************/
void SHC_timer(void)
{
    U08 i;

    if (F_TIME_BASIC_SEC_BACK)
    {
        for (i=0; i<SH_TIME_1S_MAX; i++)
        {
            _CNT(SH_timer_1s[i]);
        }

        SHC_test_do_timer();
    }

}

/***********************************************************************
 �� ��: ģ����
 �� ��: ��
************************************************************************/
#define SHC_DISP_PCB_NUM            (val_pcb_num)

/***********************************************************************
 �� ��: ѹ����
 �� ��: ��
************************************************************************/
#define SHC_DISP_COMP_NUM           (val_comp_num)

/***********************************************************************
 �� ��: ����״̬
 �� ��: ����status_run֮���״̬ʱ����ÿ���л�һ��
************************************************************************/
#define SHC_DISP_GET_MAC_STATUS     (SHC_get_disp_status())
U08 SHC_get_disp_status(void)
{
    #define MAX_SH_MAC_STATUS_WORD  (1+SH_MAC_STATUS_MAX/16)

    U16 sta_word[MAX_SH_MAC_STATUS_WORD] = {0};
    #define SET_STA_BIT(sta)        (SETBIT(sta_word[sta/16], sta%16))
    #define GET_STA_BIT(sta)        (GETBIT(sta_word[sta/16], sta%16))

    U08 i = 0;
    U08 sta_num = 0;
    U08 sta_list[SH_MAC_STATUS_MAX];
    static U08 sta_idx;

    SET_STA_BIT(status_run);
    if (log_sys_defrost_comp_num(SUB_SYS_TYPE_MAX))
    {
        SET_STA_BIT(SH_DEFROST);
    }
    if (fg_idle_sta_antifreeze
    || fg_run_sta_ac_antifreeze
    || fg_run_sta_hotw_antifreeze
    )
    {
        SET_STA_BIT(SH_DEFREZZ);
    }
    if (!fg_warm_complete)
    {
        SET_STA_BIT(SH_WARM);
    }
//    if (m_get_input(IN_LINK))
//    {
//        SET_STA_BIT(SH_LINK);
//    }
    if (fg_silent_mode)
    {
        SET_STA_BIT(SH_SILENT);
    }
    if (ModeIsStr)
    {
        SET_STA_BIT(SH_STRONG);
    }
    if (ModeIsEco)
    {
        SET_STA_BIT(SH_ECONOMY);
    }
    if (fg_timed_disinfect_run)
    {
        SET_STA_BIT(SH_STER);
    }
    if (fg_floor_dryup_run)
    {
        SET_STA_BIT(SH_DRYUP);
    }
    if (fg_holiday_away_run
    || fg_holiday_home_run
    )
    {
        SET_STA_BIT(SH_HOLIDAY);
    }
    if (fg_ability_test_run)
    {
        SET_STA_BIT(SH_TEST);
    }

    /* ��ȡ��Ч״̬ */
    for (i=0; i<SH_MAC_STATUS_MAX; i++)
    {
        if (GET_STA_BIT(i))
        {
            sta_list[sta_num++] = i;
        }
    }

    /* �л���ʾ */
    if (SH_timer_1s[SH_TIME_1S_STATUS] >= 1)
    {
        SH_timer_1s[SH_TIME_1S_STATUS] = 0;

        sta_idx++;
    }
    if (sta_idx >= sta_num)            sta_idx = 0;

    return sta_list[sta_idx];
}

/***********************************************************************
 �� ��: ��ǰ�¶�
 �� ��: �̶�Ϊ1λС��
************************************************************************/
#define SHC_DISP_GET_CTRL_TMP       (SHC_get_ctrl_tmp())
I16 SHC_get_ctrl_tmp(void)
{
	I16 temp = ctrl_temp_energy[SUB_SYS_AC_ONLY];

	if (TEMP_IS_FARA)
	{
		temp = C_To_F(temp, UINT_C|D_DOT1);
	}

	return temp;
}

/***********************************************************************
 �� ��: �趨�¶�
 �� ��: С����λ����SH_DISP_GET_SET_TMP_DOTһ��
************************************************************************/
I16 SHC_get_set_tmp_A(void)
{
	I16 temp = 0xFFFF;
	U08 grp = 0xFF;
	U08 idx = 0xFF;

	log_sys_get_sv_disp_zone_A(&grp, &idx, &temp);

	if (grp != 0xFF && idx != 0xFF)		/* ����ֵ �Ѱ������϶�ת�� ������temp==0xFFFF��Ϊtemp��I16 */
	{
		temp = para_get_val(grp, idx);
	}
	else					/* �̶�ֵ��Ҫ����10 */
	{
		temp = fun_pv_div_10(temp);
		if (TEMP_IS_FARA)
		{
			temp = C_To_F(temp, UINT_C);	/* û��С���� */
		}
	}

    return temp;
}

/***********************************************************************
 �� ��: �������¶ȵ�λ
 �� ��:
************************************************************************/
#define SHC_DISP_SET_UNIT   (SHC_get_set_unit())
U16 SHC_get_set_unit(void)
{
    static I16 bak_temp_unit;

    if (fg_tuya_temp_unit_switch)
    {
        return (cnt_wait_tuya_temp_unit> 5) ? sh_temp_unit: bak_temp_unit;
    }

    bak_temp_unit = sh_temp_unit;
    return sh_temp_unit;
}

/***********************************************************************
 �� ��: �յ�����ģʽ(����/����/�Զ�)
 �� ��:
************************************************************************/
U16 SHC_get_valid_mode(void)
{
    U16 temp = 0;

    if (assist_info[0]&SYS_COOL_CAN_USE)
    {
        SETBIT(temp, SH_MODE_COOL);
    }
    if (assist_info[0]&SYS_HEAT_CAN_USE)
    {
        SETBIT(temp, SH_MODE_HEAT);
    }
    if (assist_info[0]&SYS_AUTO_CAN_USE)
    {
        SETBIT(temp, SH_MODE_AUTO);
    }

    return temp;
}

/***********************************************************************
 �� ��: �趨�յ�ģʽ
 �� ��: ������ĳЩ�ض�����ʱ����ǿ������Ϊ����/����ģʽ
************************************************************************/
U16 SHC_get_set_mode(void)
{
    U16 temp = 0;

    if (GETBIT_N(fg_test_run, TEST_RUN_COOL_MODE)			/* ����������ʱ */
    ||  fg_refri_recy_act	                                /* �շ� */
    )
    {
        SETBIT(temp, SH_MODE_COOL);
    }
    else if (fg_floor_preheat_run			                /* ��ůԤ������ʱ */
    || fg_floor_dryup_run	                                /* ��ů�������ʱ */
    || GETBIT_N(fg_test_run, TEST_RUN_HEAT_MODE)			/* ����������ʱ */
    )
    {
        SETBIT(temp, SH_MODE_HEAT);
    }
    else
    {
        switch (sv_mode)
        {
            case SET_COOL:
                SETBIT(temp, SH_MODE_COOL);
                break;
            case SET_HEAT:
                SETBIT(temp, SH_MODE_HEAT);
                break;
            case SET_AUTO:
                SETBIT(temp, SH_MODE_AUTO);
                break;
            default:
                break;
        }
    }

    return temp;
}

/***********************************************************************
 �� ��: ����ģʽ
 �� ��:
************************************************************************/
U16 SHC_get_run_mode(void)
{
    U16 temp = 0;

    switch (log_sys_ctrl_mode_get(GET_BY_FACT))
    {
        case MODE_COOL:
            SETBIT(temp, SH_MODE_COOL);
            break;
        case MODE_HEAT:
            SETBIT(temp, SH_MODE_HEAT);
            break;
        case MODE_AUTO:
            SETBIT(temp, SH_MODE_AUTO);
            break;
        case MODE_HOTW:
            SETBIT(temp, SH_MODE_HOTWATER);
        default:
            break;
    }

    return temp;
}

/***********************************************************************
 �� ��: ��ʼ��״̬
 �� ��: ������״̬������SH_�Ķ����Ƿ�һ��
************************************************************************/
U16 SHC_get_init_sta(void)
{
    U16 temp = 0;

    switch (gIO.io_init)
    {
        case ACTION_IDLE:
            temp = SH_STATE_IDLE;
            break;
        case ACTION_ING:
            temp = SH_STATE_ING;
            break;
        case ACTION_SUCC:
            temp = SH_STATE_SUCC;
            break;
        case ACTION_FAIL:
            temp = SH_STATE_FAIL;
            break;
    }

    return temp;
}

/***********************************************************************
 �� ��: ʹ�������Ƿ���
 �� ��:
************************************************************************/
#define SHC_DISP_GET_LMT_ON         (fg_sys_limit)

/***********************************************************************
 �� ��: ʹ��������������
 �� ��:
************************************************************************/
#define SHC_DISP_GET_LMT_SET_DAY    (limit_value)

/***********************************************************************
 �� ��: ������ʱ��
 �� ��: ����ϵͳ����ʱ��Ķ����Ƿ���ȷ
************************************************************************/
#define SHC_DISP_GET_LMT_RUN_HOUR   (cell[MAX_PCB].cell_runtime[SYS_RUNTIME])

/***********************************************************************
 �� ��: ʹ������ʣ��ʱ��
 �� ��:
************************************************************************/
#define SHC_DISP_GET_LMT_LEFT_HOUR  (pre_limit_time)

/***********************************************************************
 �� ��: ��ǰ���ϸ���
 �� ��:
************************************************************************/
U16 SHC_get_err_num(void)
{
    U16 i = 0;
    U16 j = 0;
    U16 k = 0;
    U16 temp = 0;

    for (i=0; i<val_pcb_num; i++)           /* ͳ��ģ����� */
    {
        for (j=0; j<MAX_ERR_WORD; j++)
        {
            for (k=0; k<16; k++)
            {
                if (GETBIT(cell[i].cell_error[j], k))
                {
                    temp++;
                }
            }
        }
    }
    for (j=0; j<MAX_ERR_WORD; j++)          /* ͳ��ϵͳ���� */
    {
        for (k=0; k<16; k++)
        {
            if (GETBIT(cell[MAX_PCB].cell_error[j], k))
            {
                temp++;
            }
        }
    }

    return temp;
}

/***********************************************************************
 �� ��: ��ʷ���ϸ���
 �� ��:
************************************************************************/
#define SHC_DISP_GET_HIS_NUM        (SHC_get_his_num())
U16 SHC_get_his_num(void)
{
    U16 temp = 0;
    U08 i;

    for (i=0; i<MAX_HIS_ERROR; i++)         /* ͳ����ʷ���� */
    {
        if (his_err_tbl[i*HIS_ERR_SIZE] != F_NO_ERR)
        {
            temp++;
        }
    }

    return temp;
}

/***********************************************************************
 �� ��: ��������
 �� ��:
************************************************************************/
#define SHC_DISP_GET_PARA_GRP_NUM   (PARA_GROUP_NUM)

/***********************************************************************
 �� ��: ��������
 �� ��:
************************************************************************/
#define SHC_DISP_GET_PASS_GRP_NUM   (ARRELE(password))

/***********************************************************************
 �� ��: WIFI�����Ƿ�ʹ��
 �� ��:
************************************************************************/
#define SHC_DISP_GET_WIFI_USE       (TRUE)

/***********************************************************************
 �� ��: Ԥ��ʣ�������
 �� ��: ע����Щ΢�뻹������Ԥ�Ȼ����¶ȵ�����
************************************************************************/
#define SHC_DISP_GET_PREHEAT_MINT   log_sys_warm_time_get()

/***********************************************************************
 �� ��: �趨�¶���Сֵ
 �� ��: С����λ����SHC_DISP_GET_SET_TMP_DOTһ��
************************************************************************/
I16 SHC_get_set_tmp_min_A(void)
{
    I16 temp = SENSOR_NOEXIST;

    /* �յ��ܵ�Ŀ���¶ȱ���дʱ�����Ʒ�Χ��ʾ */
    if (temp_set_can_edit_zone_A == FALSE)
    {
        return SHC_get_set_tmp_A();
    }

    switch (ac_mode)
    {
        case MODE_COOL:
            temp = (temp_ctrl_type_zone_A == WATER_TEMP_CTRL) ? \
                   para_get_min(P_GaI(sv_cool)) \
                   : para_get_min(P_GaI(sv_room_cool));
            break;
        case MODE_HEAT:
            temp = (temp_ctrl_type_zone_A == WATER_TEMP_CTRL) ? \
                   para_get_min(P_GaI(sv_heat)) \
                   : para_get_min(P_GaI(sv_room_heat));
            break;
    }

    return temp;
}

/***********************************************************************
 �� ��: �趨�¶����ֵ
 �� ��: С����λ����SHC_DISP_GET_SET_TMP_DOTһ��
************************************************************************/
I16 SHC_get_set_tmp_max_A(void)
{
    I16 temp = SENSOR_NOEXIST;

    /* �յ��ܵ�Ŀ���¶ȱ���дʱ�����Ʒ�Χ��ʾ */
    if (temp_set_can_edit_zone_A == FALSE)
    {
        return SHC_get_set_tmp_A();
    }

    switch (ac_mode)
    {
        case MODE_COOL:
            temp = (temp_ctrl_type_zone_A == WATER_TEMP_CTRL) ? \
                   para_get_max(P_GaI(sv_cool))
                   : para_get_max(P_GaI(sv_room_cool));
            break;
        case MODE_HEAT:
            temp = (temp_ctrl_type_zone_A == WATER_TEMP_CTRL) ? \
                   para_get_max(P_GaI(sv_heat))
                   : para_get_max(P_GaI(sv_room_heat));
            break;
    }

    return temp;
}

/***********************************************************************
 �� ��: ϵͳ��ˮ�¶�
 �� ��: ˮԴ�ȱ�ʱ�迼���Ƿ��л�ˮ·
************************************************************************/
#define SHC_DISP_GET_TMP_OUT        (SENSOR_NOEXIST)

/***********************************************************************
 �� ��: ϵͳ��ˮ�¶�
 �� ��: ˮԴ�ȱ�ʱ�迼���Ƿ��л�ˮ·
************************************************************************/
#define SHC_DISP_GET_TMP_IN         (SENSOR_NOEXIST)

/***********************************************************************
 �� ��: ���ƶ���
 �� ��:
************************************************************************/
#define SHC_DISP_GET_CTRL_OBJ       (log_sys_get_ctrl_object())

/***********************************************************************
 �� ��: ϵͳģ��������
 �� ��:
************************************************************************/
extern U08 dm603_common_count_pv(U08 *dest, I16 *src, U08 pcb_num);
U16 SHC_get_sys_av_num(void)
{
    U16 temp = 0;
    U08 useless[MAX_PV_WORD];

    temp = dm603_common_count_pv(useless, &cell[MAX_PCB].cell_pv[0], MAX_PV);

    return temp;
}

/***********************************************************************
 �� ��: ϵͳ��������
 �� ��:
************************************************************************/
U16 SHC_get_sys_do_num(void)
{
    U16 temp = 0;
    U08 i = 0;

    for (i=0; i<BIT_NUM(assist_info[3]); i++)
    {
         if (GETBIT(assist_info[3],i))
         {
            temp++;
         }
         if (GETBIT(assist_info[4],i))
         {
            temp++;
         }
    }

    return temp;
}

/***********************************************************************
 �� ��: ������ѹ����
 �� ��:
************************************************************************/
#define SHC_DISP_GET_CM_RUN_NUM     (log_sys_comp_run_num(SUB_SYS_TYPE_MAX))

/***********************************************************************
 �� ��: ����ģʽʹ�÷�
 �� ��: �޴˹���Ĭ��ֵΪ0
************************************************************************/
#define SHC_DISP_GET_TEST_EN        (USED)

/***********************************************************************
 �� ��: �ֶ���˪��Ϣ1
 �� ��: ��HD015A��HL051A/HL055A�ã�����΢��Ĭ��Ϊ0
************************************************************************/
#define SHC_DISP_GET_MNL_DEFRO_1    (SHC_redo_force_defrost_status(0))

/***********************************************************************
 �� ��: �ֶ���˪��Ϣ2
 �� ��: ��HD015A��HL051A/HL055A�ã�����΢��Ĭ��Ϊ0
************************************************************************/
#define SHC_DISP_GET_MNL_DEFRO_2    (SHC_redo_force_defrost_status(1))
U16 SHC_redo_force_defrost_status(U16 sta_idx)
{
    enum
    {
        SH_CELL_NO_RUN=0,
        SH_CELL_RUN,
        SH_CELL_FORCE_DEFORST,
        SH_CELL_DEFROST
    };

    U08 pcb = 0;
    U08 cmp = 0;
    U08 status = SH_CELL_NO_RUN;
    U16 temp;
    U16 return_val[2] = {0, 0};

    for (pcb=0; pcb<MAX_PCB; pcb++)
    {
        status = SH_CELL_NO_RUN;            /* ÿ���л�PCB��Ҫ��һ�Σ������ϴβ�����ϢӰ�� */
        if (pcb < val_pcb_num)
        {
            for (cmp=0; cmp<SHC_USE_COMP; cmp++)
            {
                if (M_CM_GetStatus(pcb, cmp, COMP_DEFROST))
                {
                    status = SH_CELL_DEFROST;
                    break;
                }
                else if (M_CM_GetStatus(pcb, cmp, COMP_FORCE_DEFROST))
                {
                    if (status < SH_CELL_FORCE_DEFORST)
                    {
                        status = SH_CELL_FORCE_DEFORST;
                    }
                }
                else if (M_CM_GetStatus(pcb, cmp, COMP_RUN))
                {
                    if (status < SH_CELL_RUN)
                    {
                        status = SH_CELL_RUN;
                    }
                }
            }
        }
        else
        {
            status = SH_CELL_NO_RUN;
        }
        temp = (status<<((pcb*2)%16));
        return_val[(pcb*2)/16] |= temp;     /* һ��ģ��ռ��2λ */
    }

    return return_val[sta_idx];
}

/***********************************************************************
 �� ��: �趨�¶�С����
 �� ��:
************************************************************************/
U16 SHC_get_set_tmp_dot(void)
{
    U16 temp;

    switch (sv_mode)
    {
        case MODE_COOL:
            temp = para_get_dot(P_GaI(sv_cool));
            break;
        case MODE_HEAT:
            temp = para_get_dot(P_GaI(sv_heat));
            break;
        case MODE_HOTW:
            temp = para_get_dot(P_GaI(sv_hotw));
            break;
        default:
            temp = 0;
            break;
    }

    return temp;
}

/***********************************************************************
 �� ��: �û���ѯ�̵���
 �� ��: ������HD015A��΢����Ҫ���ã�����΢��Ĭ��Ϊ0
************************************************************************/
#define SHC_DISP_GET_CHK_USER_DO    (SHC_get_chk_user_do())
U16 SHC_get_chk_user_do(void)
{
    U16 temp = 0;

    if (fg_pump_on[SUB_SYS_AC_HOTW] || fg_pump_on[SUB_SYS_AC_ONLY] || debug_mode)
    {
        SETBIT(temp, SH_CHK_USER_DO_PUMP_BIT);
    }
    if (log_sys_have_fan_on())
    {
        SETBIT(temp, SH_CHK_USER_DO_FAN_BIT);
    }
    if (log_sys_comp_run_num(SUB_SYS_TYPE_MAX) > 0)
    {
        SETBIT(temp, SH_CHK_USER_DO_COMP_BIT);
    }

    return temp;
}

/***********************************************************************
 �� ��: �������С
 �� ��:
************************************************************************/
#define SHC_DISP_GET_PARA_GRP_SIZE  (GROUP_SIZE)

/***********************************************************************
 �� ��: ʹ�����������������ֵ
 �� ��:
************************************************************************/
#define SHC_DISP_GET_LMT_SET_MAX    (1365)

/***********************************************************************
 �� ��: �Ƿ����ֶ���˪
 �� ��:
************************************************************************/
#define SHC_DISP_HAVE_MNL_DEFRO     (TRUE)

/***********************************************************************
 �� ��: ���й������Ƿ������޸�ģʽ
 �� ��:
************************************************************************/
#define SHC_DISP_RUN_ALLOW_SET_MODE (TRUE)

/***********************************************************************
 �� ��: �Ƿ��п������빦�ܣ�������ר��HL051A/HL055Aʹ��
 �� ��:
************************************************************************/
#define SHC_DISP_HAVE_START_CODE_MEIYI  (FALSE)

/***********************************************************************
 �� ��: �������룬������ר��HL051A/HL055Aʹ��
 �� ��:
************************************************************************/
#define SHC_DISP_START_CODE_MEIYI       (FALSE)

/***********************************************************************
 �� ��: �Ƿ���ѹ����˪
 �� ��:
************************************************************************/
#define SHC_DISP_HAVE_CM_MNL_DEFRO      (FALSE)

/***********************************************************************
 �� ��: �Ƿ���ʪ�ȿ���
 �� ��:
************************************************************************/
#define SHC_DISP_HAVE_HUMI              (FALSE)

/***********************************************************************
 �� ��: �Ƿ��п�������
 �� ��: ���У�ʹ�õ�����Ȩ����SH_PWR_START_MAC
************************************************************************/
#define SHC_DISP_HAVE_START_CODE        (FALSE)

/***********************************************************************
 �� ��: ��ʼ�������Ƿ���Ҫ��֤����
 �� ��: ���У�ʹ�õ�����Ȩ����SH_PWR_INIT_PARA
************************************************************************/
#define SHC_DISP_HAVE_RESET_PASS        (FALSE)

/***********************************************************************
 �� ��: �Ƿ������Ԥ��
 �� ��: ע����SH_CMD_OFF_OILHEAT����Ĵ���
************************************************************************/
#define SHC_DISP_HAVE_OFF_OILHEAT       (FALSE)

/***********************************************************************
 �� ��: �Ƿ��ѡ����Ԥ��
 �� ��: ע����SH_CMD_QUICK_OILHEAT����Ĵ���
************************************************************************/
#define SHC_DISP_HAVE_QUICK_OILHEAT     (TRUE)

/***********************************************************************
 �� ��: �Ƿ���BMS���ý���
 �� ��: ע����SH_CMD_BMS_PARA_SET����Ĵ���
************************************************************************/
#define SHC_DISP_HAVE_BMS_SET           (FALSE)

/***********************************************************************
 �� ��: �Ƿ�ΪӢ����ʾ
 �� ��:
************************************************************************/
#define SHC_DISP_IS_ENGLISH_DISP        (sh_language == RN_LANGUAGE_EN)

/***********************************************************************
 �� ��: �Ƿ��ֹ��Ӣ���л�
 �� ��: Ĭ��ΪFALSE������������Ӣ���л�
************************************************************************/
#define SHC_DISP_IS_FORBID_SET_LANGUAGE    (FALSE)

/***********************************************************************
 �� ��: �Ƿ�Ϊ���ձ�Ƶ��
 �� ��: Ĭ��ΪFALSE,��û�б�Ƶ����Ϣ����
************************************************************************/
#define SHC_DISP_IS_PUNP_INVT       (FALSE)

/***********************************************************************
 �� ��: �Ƿ��������ѯ����
 �� ��: Ĭ��ΪFALSE
************************************************************************/
#define SHC_DISP_HAVE_PASS_CHK      (FALSE)

/***********************************************************************
 �� ��: �Ƿ��е�ů��ɹ���
 �� ��: ע����SHC_DISP_HAVE_FLOOR_DRYUP����Ĵ���
************************************************************************/
#define SHC_DISP_HAVE_FLOOR_DRYUP        (USE_HEAT && HEAT_USE_FLH)

/***********************************************************************
 �� ��: �Ƿ���ǿ�ƿ���������
 �� ��: ע����SHC_DISP_HAVE_FORCE_IBH����Ĵ���
************************************************************************/
#define SHC_DISP_HAVE_FORCE_IBH          (IBH_cfg_type \
                                         && ((USE_HEAT && HEAT_USE_IBH && (ac_mode == MODE_HEAT)) \
                                            || (val_hotw_used && HOTW_USE_IBH)))

/***********************************************************************
 �� ��: �Ƿ���ǿ�ƿ�ˮ�����
 �� ��: ע����SHC_DISP_HAVE_FORCE_TBH����Ĵ���
************************************************************************/
#define SHC_DISP_HAVE_FORCE_TBH          (val_hotw_used)

/***********************************************************************
 �� ��: �Ƿ���ǿ����ˮ
 �� ��: ע����SHC_DISP_HAVE_FORCE_HW_MODE����Ĵ���
************************************************************************/
#define SHC_DISP_HAVE_FORCE_HW_MODE      (val_hotw_used)

/***********************************************************************
 �� ��: �Ƿ��м���ģʽ���ý���
 �� ��: ע����SHC_DISP_HAVE_HOLIDAY_MODE����Ĵ���
************************************************************************/
#define SHC_DISP_HAVE_HOLIDAY_MODE       (!USE_COOL_ONLY)

/***********************************************************************
 �� ��: ǰ�漸��������Ϣ������
 �� ��:
************************************************************************/
U16 SHC_get_disp_fg(void)
{
    U16 temp = 0;

    if (SHC_DISP_HAVE_CM_MNL_DEFRO)
    {
        SETBIT(temp, 0);
    }
    if (SHC_DISP_HAVE_HUMI)
    {
        SETBIT(temp, 1);
    }
    if (SHC_DISP_HAVE_START_CODE)
    {
        SETBIT(temp, 2);
    }
    if (SHC_DISP_HAVE_RESET_PASS)
    {
        SETBIT(temp, 3);
    }
    if (SHC_DISP_HAVE_OFF_OILHEAT)
    {
        SETBIT(temp, 4);
    }
    if (SHC_DISP_HAVE_QUICK_OILHEAT)
    {
        SETBIT(temp, 5);
    }
    if (SHC_DISP_HAVE_BMS_SET)
    {
        SETBIT(temp, 6);
    }
    if (SHC_DISP_IS_ENGLISH_DISP)
    {
        SETBIT(temp, 7);
    }
    if (SHC_DISP_IS_FORBID_SET_LANGUAGE)
    {
        SETBIT(temp, 8);
    }
    if (SHC_DISP_IS_PUNP_INVT)
    {
        SETBIT(temp, 9);
    }
    if (SHC_DISP_HAVE_PASS_CHK)
    {
        SETBIT(temp, 10);
    }
    if (SHC_DISP_HAVE_FLOOR_DRYUP)
    {
        SETBIT(temp, 11);
    }
    if (SHC_DISP_HAVE_FORCE_IBH)
    {
        SETBIT(temp, 12);
    }
    if (SHC_DISP_HAVE_FORCE_TBH)
    {
        SETBIT(temp, 13);
    }
    if (SHC_DISP_HAVE_FORCE_HW_MODE)
    {
        SETBIT(temp, 14);
    }
    if (SHC_DISP_HAVE_HOLIDAY_MODE)
    {
        SETBIT(temp, 15);
    }

    return temp;
}

/***********************************************************************
 �� ��: �Ƿ��з�����Ϣ��ʾ����
 �� ��: ע����SHC_DISP_HAVE_FCUINFO����Ĵ���
************************************************************************/
#define SHC_DISP_HAVE_FCUINFO      (val_fcu_num > 0)

/***********************************************************************
 �� ��: �Ƿ��������Խ���
 �� ��: ע����SHC_DISP_HAVE_ABILITY_TEST����Ĵ���
************************************************************************/
#define SHC_DISP_HAVE_ABILITY_TEST      (TRUE)

/***********************************************************************
 �� ��: �Ƿ��еڶ����趨�¶�
 �� ��: �������������еڶ����趨�¶�
************************************************************************/
#define SHC_DISP_HAVE_SET_TMP_EX        (USE_DUAL_ZONE_CTRL)

/***********************************************************************
 �� ��: �Ƿ�����ˮ�趨�¶�
 �� ��: ��ʹ����ˮ������ˮ�趨�¶�
************************************************************************/
#define SHC_DISP_HAVE_SET_HOT           (val_hotw_used)

/***********************************************************************
 �� ��: �Ƿ��б�Ƶ��������
 �� ��: ��ʹ�ð��ձ�Ƶ��ʱ���д˹���
************************************************************************/
#define SHC_DISP_HAVE_INVT_PARA         (val_inverter_used)

/***********************************************************************
 �� ��: �Ƿ��б�Ƶ���ϲ�ѯ
 �� ��: ��ʹ�ð��ձ�Ƶ��ʱ���д˹���
************************************************************************/
#define SHC_DISP_HAVE_INVT_ERR          (val_inverter_used)

/***********************************************************************
 �� ��: �Ƿ����շ�����
 �� ��:
************************************************************************/
#define SHC_DISP_HAVE_RECYCLE_REFRI     (TRUE)

/***********************************************************************
 �� ��: �Ƿ���ð�����
 �� ��:
************************************************************************/
#define SHC_DISP_HAVE_FORBID_KEY_BUZZER (FALSE)

/***********************************************************************
 �� ��: �Ƿ���ǿ���ⲿ��Դ
 �� ��:
************************************************************************/
#define SHC_DISP_HAVE_FORCE_AHS         ((USE_HEAT && HEAT_USE_AHS && (ac_mode == MODE_HEAT)) \
                                        || (val_hotw_used && HOTW_USE_AHS))

/***********************************************************************
 �� ��: �Ƿ��ھ���ģʽ
 �� ��:
************************************************************************/
#define SHC_DISP_IS_SILENT_MODE_ON      (fg_silent_mode)
/***********************************************************************
 �� ��: �Ƿ���ˮ��������
 �� ��:
************************************************************************/
#define SHC_DISP_IS_PUMP_TEST_ON        (GETBIT_N(fg_test_run, TEST_RUN_PUMP))

/***********************************************************************
 �� ��: �Ƿ�������������
 �� ��:
************************************************************************/
#define SHC_DISP_IS_COOL_TEST_ON        (GETBIT_N(fg_test_run, TEST_RUN_COOL_MODE))

/***********************************************************************
 �� ��: �Ƿ�������������
 �� ��:
************************************************************************/
#define SHC_DISP_IS_HEAT_TEST_ON        (GETBIT_N(fg_test_run, TEST_RUN_HEAT_MODE))

/***********************************************************************
 �� ��: �Ƿ�������ˮ������
 �� ��:
************************************************************************/
#define SHC_DISP_IS_HOTWATER_TEST_ON    (GETBIT_N(fg_test_run, TEST_RUN_HOTW_MODE))

/***********************************************************************
 �� ��: �Ƿ���ϵͳ�ſյ���
 �� ��:
************************************************************************/
#define SHC_DISP_IS_SYSTEM_EMPTY_ON     (GETBIT_N(fg_test_run, TEST_RUN_AIR_EMPTY))

/***********************************************************************
 �� ��: �Ƿ�ʹ��ͯ������
 �� ��:
************************************************************************/
#define SHC_DISP_HAVE_CHILD_LOCK        (val_lock_screen == TRUE)

/***********************************************************************
 �� ��: ǰ�漸��������Ϣ������
 �� ��:
************************************************************************/
U16 SHC_get_disp_fg_ex(void)
{
    U16 temp = 0;

    if (SHC_DISP_HAVE_FCUINFO)
    {
        SETBIT(temp, 0);
    }
    if (SHC_DISP_HAVE_ABILITY_TEST)
    {
        SETBIT(temp, 1);
    }
    if (SHC_DISP_HAVE_SET_TMP_EX)
    {
        SETBIT(temp, 2);
    }
    if (SHC_DISP_HAVE_SET_HOT)
    {
        SETBIT(temp, 3);
    }
    if (SHC_DISP_HAVE_INVT_PARA)
    {
        SETBIT(temp, 4);
    }
    if (SHC_DISP_HAVE_INVT_ERR)
    {
        SETBIT(temp, 5);
    }
    if (SHC_DISP_HAVE_RECYCLE_REFRI)
    {
        SETBIT(temp, 6);
    }
    if (SHC_DISP_HAVE_FORBID_KEY_BUZZER)
    {
        SETBIT(temp, 7);
    }
    if (SHC_DISP_HAVE_FORCE_AHS)
    {
        SETBIT(temp, 8);
    }
    if (SHC_DISP_IS_SILENT_MODE_ON)
    {
        SETBIT(temp, 9);
    }
    if (SHC_DISP_IS_PUMP_TEST_ON)
    {
        SETBIT(temp,10);
    }
    if (SHC_DISP_IS_COOL_TEST_ON)
    {
        SETBIT(temp,11);
    }
    if (SHC_DISP_IS_HEAT_TEST_ON)
    {
        SETBIT(temp,12);
    }
    if (SHC_DISP_IS_HOTWATER_TEST_ON)
    {
        SETBIT(temp,13);
    }
    if (SHC_DISP_IS_SYSTEM_EMPTY_ON)
    {
        SETBIT(temp,14);
    }
    if (SHC_DISP_HAVE_CHILD_LOCK)
    {
        SETBIT(temp,15);
    }

    return temp;
}

/***********************************************************************
 �� ��: �Ƿ����ǿ�ƿ���������
 �� ��: ע����SHC_DISP_CAN_FORCE_IBH����Ĵ���
************************************************************************/
#define SHC_DISP_CAN_FORCE_IBH       ((STEP_IS_RUN)\
                                      && ((fg_hotw_need && HOTW_USE_IBH && !val_TBH_used)\
                                          || (fg_heat_need && HEAT_USE_IBH)))// ����״̬�� ��ˮǿ�ƿ� ����ǿ�ƿ�

/***********************************************************************
 �� ��: �Ƿ����ǿ�ƿ�ˮ�����
 �� ��: ע����SHC_DISP_CAN_FORCE_TBH����Ĵ���
************************************************************************/
#define SHC_DISP_CAN_FORCE_TBH       ((STEP_IS_RUN)\
                                       && (fg_hotw_need && val_TBH_used))// ����״̬�� ��ˮǿ�ƿ�

/***********************************************************************
 �� ��: ǰ�漸��������Ϣ������
 �� ��:
************************************************************************/
U16 SHC_get_disp_can_do_fg(void)
{
    U16 temp = 0;

    if (SHC_DISP_CAN_FORCE_IBH)
    {
        SETBIT(temp, 0);
    }
    if (SHC_DISP_CAN_FORCE_TBH)
    {
        SETBIT(temp, 1);
    }

    return temp;
}

/***********************************************************************
 �� ��: �ڶ����趨�¶�
 �� ��: B���趨�¶�
************************************************************************/
I16 SHC_get_set_tmp_B(void)
{
	I16 temp = 0xFFFF;
	U08 grp = 0xFF;
	U08 idx = 0xFF;

	log_sys_get_sv_disp_zone_B(&grp, &idx, &temp);

	if (grp != 0xFF && idx != 0xFF)		/* ����ֵ �Ѱ������϶�ת�� ������temp==0xFFFF��Ϊtemp��I16 */
	{
		temp = para_get_val(grp, idx);
	}
	else					/* �̶�ֵ��Ҫ����10 */
	{
		temp = fun_pv_div_10(temp);
		if (TEMP_IS_FARA)
		{
			temp = C_To_F(temp, UINT_C);	/* û��С���� */
		}
	}

    return temp;
}

/***********************************************************************
 �� ��: �ڶ�����ǰ�¶�
 �� ��:
************************************************************************/
I16 SHC_get_ctrl_tmp_B(void)
{
    I16 temp = pv_sys_TWout2_ac;            /* �յ��ܳ�ˮ�¶� */

	if (TEMP_IS_FARA)
	{
		temp = C_To_F(temp, UINT_C|D_DOT1);
	}

    return temp;
}

/***********************************************************************
 �� ��: �ڶ����趨�¶����ֵ
 �� ��: С����λ����SHC_DISP_GET_SET_TMP_DOTһ��
************************************************************************/
I16 SHC_get_set_tmp_max_B(void)
{
    I16 temp = SENSOR_NOEXIST;

    /* �յ��ܵ�Ŀ���¶ȱ���дʱ�����Ʒ�Χ��ʾ */
    if (temp_set_can_edit_zone_B == FALSE)
    {
        return SHC_get_set_tmp_B();
    }

    switch (ac_mode)
    {
        case MODE_COOL:
            temp = (temp_ctrl_type_zone_B == WATER_TEMP_CTRL) ? \
                   para_get_max(P_GaI(sv_cool_zone_B)) \
                   : para_get_max(P_GaI(sv_room_cool_zone_B));
            break;
        case MODE_HEAT:
            temp = (temp_ctrl_type_zone_B == WATER_TEMP_CTRL) ? \
                   para_get_max(P_GaI(sv_heat_zone_B)) \
                   : para_get_max(P_GaI(sv_room_heat_zone_B));
            break;
    }

    return temp;
}

/***********************************************************************
 �� ��: �ڶ����趨�¶���Сֵ
 �� ��: С����λ����SHC_DISP_GET_SET_TMP_DOTһ��
************************************************************************/
I16 SHC_get_set_tmp_min_B(void)
{
    I16 temp = SENSOR_NOEXIST;

    /* �յ��ܵ�Ŀ���¶ȱ���дʱ�����Ʒ�Χ��ʾ */
    if (temp_set_can_edit_zone_B == FALSE)
    {
        return SHC_get_set_tmp_B();
    }

    switch (ac_mode)
    {
        case MODE_COOL:
            temp = (temp_ctrl_type_zone_B == WATER_TEMP_CTRL) ? \
                   para_get_min(P_GaI(sv_cool_zone_B))
                   : para_get_min(P_GaI(sv_room_cool_zone_B));
            break;
        case MODE_HEAT:
            temp = (temp_ctrl_type_zone_B == WATER_TEMP_CTRL) ? \
                   para_get_min(P_GaI(sv_heat_zone_B)) \
                   : para_get_min(P_GaI(sv_room_heat_zone_B));
            break;
    }

    return temp;
}

/***********************************************************************
 �� ��: ��ˮ�趨�¶�
 �� ��:
************************************************************************/
I16 SHC_get_hot_tmp_set(void)
{
	I16 temp = 0xFFFF;
	U08 grp = 0xFF;
	U08 idx = 0xFF;

	log_sys_get_sv_disp_zone_hot(&grp, &idx, &temp);

	if (grp != 0xFF && idx != 0xFF)		/* ����ֵ �Ѱ������϶�ת�� ������temp==0xFFFF��Ϊtemp��I16 */
	{
		temp = para_get_val(grp, idx);
	}
	else					/* �̶�ֵ��Ҫ����10 */
	{
		temp = fun_pv_div_10(temp);
		if (TEMP_IS_FARA)
		{
			temp = C_To_F(temp, UINT_C);	/* û��С���� */
		}
	}

    return temp;
}

/***********************************************************************
 �� ��: ��ˮ�趨�¶���Сֵ
 �� ��:
************************************************************************/
I16 SHC_get_hot_tmp_set_min(void)
{
    /* ��ˮ�ܵ�Ŀ���¶ȱ���дʱ������ȡ��Сֵ */
    if (fg_sv_temp_energy_overwrite_hotw)
    {
        return SHC_get_hot_tmp_set();
    }

    return para_get_min(P_GaI(sv_hotw));
}

/***********************************************************************
 �� ��: ��ˮ�趨�¶����ֵ
 �� ��:
************************************************************************/
I16 SHC_get_hot_tmp_set_max(void)
{
    /* ��ˮ�ܵ�Ŀ���¶ȱ���дʱ������ȡ���ֵ */
    if (fg_sv_temp_energy_overwrite_hotw)
    {
        return SHC_get_hot_tmp_set();
    }

    return para_get_max(P_GaI(sv_hotw));
}

/***********************************************************************
 �� ��: ϵͳά��������Ϣ
 �� ��: ���޴������ã�����0!!!
************************************************************************/
I16 SHC_get_lmt_type_info(void)
{
    I16 temp = 0;
    U16 dict_idx = 0;
    U08 min = 0;
    U08 max = 0;
    U08 i;

    /* ��8λΪ�ֵ���Ϣ */
    for (i=0; i<get_dict_num(); i++)
    {
        if(para_dict[i].pmsg == Msg_LimitType)
        {
            dict_idx = i + UNIT_OFFSET;
            break;
        }
    }
    /* ��8λ�ĸ�4λΪ���� */
    max = 1;
    /* ��8λ�ĵ�4λΪ���� */
    min = 0;

    temp = ((dict_idx & 0xFF) << 8)
         | ((max << 4) & 0xF0)
         | ((min << 0) & 0x0F);


    return temp;
}

/***********************************************************************
 �� ��: ϵͳά������ֵ
 �� ��: ���޴������ã�����0!!!
************************************************************************/
I16 SHC_get_lmt_type_value(void)
{
    I16 temp;

    temp = limit_type;

    return temp;
}

/***********************************************************************
 �� ��: ��Ƶ������Ϣ����
 �� ��: ���޴���ɷ���0
************************************************************************/
I16 SHC_get_invt_err_num(void)
{
    return INV_ERR_PARA_INFO_NUM;
}

/***********************************************************************
 �� ��: ��ǰ����ģʽ����
 �� ��: ���޴���ɷ���0
************************************************************************/
I16 SHC_get_silent_level(void)
{
    return silent_mode_level;
}

/***********************************************************************
 �� ��: ����ģʽ���������� ��8λΪ���� ��8λΪ����
 �� ��: ���޴���ɷ���0
************************************************************************/
I16 SHC_get_silent_level_attr(void)
{
    return MAKEWORD(SILENT_LEVEL_2, SILENT_LEVEL_1);
}

/***********************************************************************
 �� ��: �·������¶ȵ�ʱ���� 0��ʾʹ��ǰ���Ĭ��ֵ30��
 �� ��: ���޴���ɷ���0
************************************************************************/
I16 SHC_get_room_temp_wr_intvl(void)
{
    return 0;
}

/***********************************************************************
 �� ��: ��ȡ��嵱ǰ����
 �� ��:
************************************************************************/
I16 SHC_get_bkbd_language(void)
{
    static I16 bak_language;

    if (fg_tuya_lang_switch)
    {
        /* APP���������л���5����������Ļˢ�£���ʱ�ڼ䲻ˢ��3.5��������ֵ */
        return (cnt_wait_tuya_lang > 5) ? sh_language : bak_language;
    }

    bak_language = sh_language;
    return sh_language;
}

#define SHC_DISP_IS_FLOOR_PREHEAT_RUN	    (fg_floor_preheat_run)
#define SHC_DISP_IS_FLOOR_TIMER_RUN		    (fg_floor_dryup_run)
#define SHC_DISP_IS_HOLIDAY_AWAY		    (fg_holiday_away_run)
#define SHC_DISP_IS_HOLIDAY_HOME		    (fg_holiday_home_run)
#define SHC_DISP_HAVE_4G_FUNCTION		    (app_4g_have())
#define SHC_DISP_HAVE_REMOTE_UPGRADE_REQ	(fg_request_remote_upgrade)
#define SHC_DISP_IS_FLOOR_DRYUP_ON			(fg_floor_dryup_run)
#define SHC_DISP_IS_FORCE_IBH_ON			(fg_force_IBH_run)
#define SHC_DISP_IS_FORCE_TBH_ON			(fg_force_TBH_run)
#define SHC_DISP_IS_FORCE_HOT_WATER_ON		(fg_force_hotw_mode_run)
#define SHC_DISP_IS_FORCE_AHS_ON			(fg_force_AHS_run)
#if defined(M_TCL)
#define SHC_DISP_IS_USER_PWR_HAVE_CLR       (TRUE)
#else
#define SHC_DISP_IS_USER_PWR_HAVE_CLR       (FALSE)
#endif

/***********************************************************************
 �� ��: ǰ�漸��������Ϣ������
 �� ��:
************************************************************************/
U16 SHC_get_disp_fg_ex2(void)
{
	U16 temp = 0;

	if (SHC_DISP_IS_FLOOR_PREHEAT_RUN)
	{
		SETBIT(temp, 0);
	}
	if (SHC_DISP_IS_FLOOR_TIMER_RUN)
	{
		SETBIT(temp, 1);
	}
	if (SHC_DISP_IS_HOLIDAY_AWAY)
	{
		SETBIT(temp, 2);
	}
	if (SHC_DISP_IS_HOLIDAY_HOME)
	{
		SETBIT(temp, 3);
	}
	if (SHC_DISP_HAVE_4G_FUNCTION)
	{
		SETBIT(temp, 4);
	}
	if (SHC_DISP_HAVE_REMOTE_UPGRADE_REQ)
	{
		SETBIT(temp, 5);
	}
	if (SHC_DISP_IS_FLOOR_DRYUP_ON)
	{
		SETBIT(temp, 6);
	}
	if (SHC_DISP_IS_FORCE_IBH_ON)
	{
		SETBIT(temp, 7);
	}
	if (SHC_DISP_IS_FORCE_TBH_ON)
	{
		SETBIT(temp, 8);
	}
	if (SHC_DISP_IS_FORCE_HOT_WATER_ON)
	{
		SETBIT(temp, 9);
	}
	if (SHC_DISP_IS_FORCE_AHS_ON)
	{
		SETBIT(temp, 10);
	}
	if (SHC_DISP_IS_USER_PWR_HAVE_CLR)
	{
        SETBIT(temp, 13);
	}

	return temp;
}

/***********************************************************************
 �� ��: ��ʾ��������ǰ16λ
 �� ��:
************************************************************************/
U16 SHC_get_disp_language1(void)
{
	U16 temp = 0;

	SETBIT(temp, SH_LANGUAGE_CN);
	SETBIT(temp, SH_LANGUAGE_EN);
	SETBIT(temp, SH_LANGUAGE_ES);
	SETBIT(temp, SH_LANGUAGE_GR);
	SETBIT(temp, SH_LANGUAGE_FR);
	SETBIT(temp, SH_LANGUAGE_IT);
	SETBIT(temp, SH_LANGUAGE_PL);
	SETBIT(temp, SH_LANGUAGE_HG);
	SETBIT(temp, SH_LANGUAGE_GE);

	return temp;
}

/***********************************************************************
 �� ��: ��ʾ�������ú�16λ
 �� ��:
************************************************************************/
U16 SHC_get_disp_language2(void)
{
	U16 temp = 0;

//	SETBIT(temp, SH_LANGUAGE_BG-16);

	return temp;
}

/***********************************************************************
 �� ��: ������ģʽ
 �� ��: û�д˹��ܵ�Ĭ�Ϸ���0
************************************************************************/
U16 SHC_get_sound_set(void)
{
    U16 temp = 0;

    switch (val_alarm_sound_set)
    {
        case ALARM_SOUND_SET_OFF:
            temp = SH_ALARM_SOUND_SET_OFF;
            break;
        case ALARM_SOUND_SET_ON:
            temp = SH_ALARM_SOUND_SET_ON;
            break;
        case ALARM_SOUND_SET_10S_ONCE:
            temp = SH_ALARM_SOUND_SET_10S_ONCE;
            break;
        case ALARM_SOUND_SET_10S_CYCLE:
            temp = SH_ALARM_SOUND_SET_10S_CYCLE;
            break;
    }

    return temp;
}

/***********************************************************************
 �� ��: ����ģʽ
 �� ��: Ŀǰֻ��7���������ж��Ƿ�����ˮ
************************************************************************/
U16 SHC_get_mode_type(void)
{
    U16 temp = 0;

    if (USE_HEAT_ONLY)
    {
        temp = SH_MODE_HEAT_ONLY;
    }
    else
    {
        temp = SH_MODE_HEAT_PUMP;
    }
    return temp;
}

/***********************************************************************
 �� ��: ��ˮ�¶�
 �� ��:
************************************************************************/
#define SHC_DISP_GET_TMP_HOT            (SHC_get_tmp_hot())
I16 SHC_get_tmp_hot(void)
{
	I16 temp = pv_sys_THWt;

	if (TEMP_IS_FARA)
	{
		temp = C_To_F(temp, UINT_C|D_DOT1);
	}

	return temp;
}

/***********************************************************************
 �� ��: �����ʪ���趨ģʽ
 �� ��:
************************************************************************/
#define SHC_DISP_GET_HUMI_MODE_VALID    (0)

/***********************************************************************
 �� ��: ʪ���趨ģʽ
 �� ��:
************************************************************************/
#define SHC_DISP_GET_HUMI_MODE_SET      (0)

/***********************************************************************
 �� ��: ʪ������ģʽ
 �� ��:
************************************************************************/
#define SHC_DISP_GET_HUMI_MODE_RUN      (0)

/***********************************************************************
 �� ��: ��ǰʪ��
 �� ��:
************************************************************************/
#define SHC_DISP_GET_HUMI_CURT          (0)

/***********************************************************************
 �� ��: �趨ʪ��
 �� ��:
************************************************************************/
#define SHC_DISP_GET_HUMI_SET           (0)

/***********************************************************************
 �� ��: �趨ʪ����Сֵ
 �� ��:
************************************************************************/
#define SHC_DISP_GET_HUMI_SET_MIN       (0)

/***********************************************************************
 �� ��: �趨ʪ�����ֵ
 �� ��:
************************************************************************/
#define SHC_DISP_GET_HUMI_SET_MAX       (0)

/***********************************************************************
 �� ��: ѹ��ģ������Ԫ����
 �� ��:
************************************************************************/
#define SHC_COMP_PV_STEP                (128)

/***********************************************************************
 �� ��: �����������Ԫ����
 �� ��:
************************************************************************/
#define SHC_DO_STEP                     (128)

/***********************************************************************
 �� ��: ���������뵥Ԫ����
 �� ��:
************************************************************************/
#define SHC_DI_STEP                     (128)

/***********************************************************************
 �� ��: �������汾��
 �� ��:
************************************************************************/
#define SHC_VER_PCB_NAME                (msg_idu_version)
#define SHC_VER_CODE(pcb_id)            (&idu_mac_ver[pcb_id][0])
#define SHC_VER_DATA_SIZE               (sizeof(APPFUNCODEVER) / sizeof(U08))

/***********************************************************************
 �� ��: ��Ƶ���汾��
 �� ��: ��û�б�Ƶ���汾�ţ���ver.func_code[]��Ϊȫ0
************************************************************************/
/*******************************************************************************
 �� ��: SHC_transform_vfd_ver()
 �� ��:
 �� ��: ver_str--;
        func_code--;
        ver_code--;
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void SHC_transform_vfd_ver(U08 *ver_str, U16 func_code, U16 ver_code)
{
    #define SH_VER_VFD_CODE_NULL        (INVT_PARA_VALUE_NULL)

    U08 ver[32] = "X1.VF$$$$.$##.V100###";  /* ����X1.VF250A.F01.V100A00 */
    char symbol[] = {'X', 'K', 'F', 'T', 'Q'};              /* ϴ�ӡ��յ���������ͨ�á����� */

    if (func_code == (U16)SH_VER_VFD_CODE_NULL
        && ver_code == (U16)SH_VER_VFD_CODE_NULL
        )
    {
        ver_str[0] = '0';
        ver_str[1] = '.';
        ver_str[2] = '0';
    }
    else
    {
        MEMCPY(&ver_str[0], &ver[0], 5);

        /* �汾��ת�� */
        ver_str[5] = GET_DIGIT(func_code, 4)+'0';
        ver_str[6] = GET_DIGIT(func_code, 3)+'0';
        ver_str[7] = GET_DIGIT(func_code, 2)+'0';
        ver_str[8] = GET_DIGIT(func_code, 1)-1+'A';
        ver_str[9] = '.';
        ver_str[10] = symbol[GET_DIGIT(func_code, 0)-1];

        ver_str[11] = GET_DIGIT(ver_code, 4)+'0';
        ver_str[12] = GET_DIGIT(ver_code, 3)+'0';

        MEMCPY(&ver_str[13], &ver[13], 5);

        if (GET_DIGIT(ver_code, 2))
        {
            ver_str[18] = GET_DIGIT(ver_code, 2)-1+'A';
            ver_str[19] = GET_DIGIT(ver_code, 1)+'0';
            ver_str[20] = GET_DIGIT(ver_code, 0)+'0';
        }
        else
        {
            ver_str[18] = GET_DIGIT(ver_code, 1)-1+'A';
            ver_str[19] = GET_DIGIT(ver_code, 0)+'0';
            ver_str[20] = 0;
        }
    }
}
void SHC_get_vfd_ver(SH_CHK_VER *ver, U16 pcb_id)
{
    #define SHC_VER_VFD_DATA_SIZE        (sizeof(APPFUNCODEVER) / sizeof(U08))

    U08 i;
    U08 VerCode[MAX_VER_BYTE] = {0};        /* ���ڽ���Ƶ���汾��ת��ΪU08���� */

    ver->pcb_name = msg_invt_version;
    ver->pcb_id = MAKEWORD(pcb_id, NO_ID);

    /* ����Ƶ���汾��ת��ΪU08���� */
    SHC_transform_vfd_ver(VerCode, invt_info_function(INVT_ID_1), invt_info_version(INVT_ID_1));

    /* ********************************************************** */

    /* ��ȡ�汾���ַ��� */
    for (i=0; i<MAX_VER_BYTE; i++)
    {
        if (i < SHC_VER_VFD_DATA_SIZE)
        {
            ver->fun_code[i] = VerCode[i];
        }
        else
        {
            ver->fun_code[i] = 0;
        }
    }
}

/***********************************************************************
 �� ��: �����汾��
 �� ��:
************************************************************************/
#define SHC_EXIO_VER_PCB_NAME               (msg_odu_version)
#define SHC_EXIO_VER_CODE(pcb_id, exio_id)  (&odu_mac_ver[pcb_id][0])
#define SHC_EXIO_VER_DATA_SIZE              (sizeof(APPFUNCODEVER) / sizeof(U08))

/***********************************************************************
 �� ��: 4G��Ϣ
 �� ��:
************************************************************************/
void SHC_get_4G_info(SH_4G_INFO *info)
{
	const U16 run_sta_string[] =
	{
		msg_4g_status_none,
		msg_4g_status_no_module,
		msg_4g_status_connect_module,
		msg_4g_status_connect_net,
		msg_4g_status_connect_cloud,
		msg_4g_status_module_err,
	};
	U08 i;
	for (i=0; i<SH_MAX_4G_VER_WORD; i++)
	{
		info->ver.info[i] = 0;
	}
	for (i=0; i<SH_MAX_4G_SN_WORD; i++)
	{
		info->sn.info[i] = 0;
	}
	info->csq.name = msg_4g_signal;
	info->sta.name = msg_4g_run_status;
	info->ver.name = msg_4g_version;
	info->sn.name = msg_4g_sn;

	info->csq.value = app_4g_rw.csq;
	info->sta.value = app_4g_rw.run_sta;
	info->sta.value_string = run_sta_string[app_4g_rw.run_sta];
	for (i=0; i<SH_MAX_4G_VER_WORD; i++)
	{
		info->ver.info[i] = app_4g_rw.ver[i];
	}
	for (i=0; i<SH_MAX_4G_SN_WORD; i++)
	{
		info->sn.info[i] = app_4g_rw.sn[i];
	}
}

/***********************************************************************
 �� ��: ��������
 �� ��: ��û�г������룬�븳ֵΪȫ��0xFF
************************************************************************/
void SHC_get_pass_super(U16 *dest_pass)
{
    MEMCPY((U08 *)dest_pass, (U08 *)&password[PASS_SUPER][0], MAX_PASS_BIT);
}

/***********************************************************************
 �� ��: ��������
 �� ��: ��û�п������룬�븳ֵΪȫ��0xFF
************************************************************************/
void SHC_get_pass_pop(U16 *dest_pass)
{
    MEMCPY((U08 *)dest_pass, (U08 *)&password[PASS_POP][0], MAX_PASS_BIT);
}

/***********************************************************************
 �� ��: ����ģʽ
 �� ��: ��û�м���ģʽ���븳ֵΪȫ��0xFF
************************************************************************/
void SHC_get_holiday_info(SH_HOLIDAYINFO *info)
{
    ;
}

/***********************************************************************
 �� ��: ���ϲ�ѯʹ�õ��Ĺ��������ַ���
 �� ��:
************************************************************************/
#define SHC_NAME_M_ERR(err)         (msg_m_error[err])  /* ϵͳ�������� */
#define SHC_NAME_S_ERR(err)         (msg_s_error[err])  /* ģ��������� */

BOOL SHC_get_curt_err(SH_ERR_CURT *err, U16 *sys_err_id, U16 *mod_id, U16 *mod_err_id)
{
    BOOL fg_err = FALSE;

    /* ϵͳ���� */
    for (; *sys_err_id<F_NO_ERR; (*sys_err_id)++)
    {
        if (fg_err)  break;

        if (m_get_error(MAX_PCB, *sys_err_id))
        {
            fg_err = TRUE;
            err->name = SHC_NAME_M_ERR(*sys_err_id);
            err->id = MAKEWORD(NO_ID, NO_ID);
            err->err_id = *sys_err_id;
        }
    }

    if (!fg_err)
    {
        /* ��Ԫ���� */
        for (; *mod_id<MAX_PCB; *mod_err_id=0,(*mod_id)++)
        {
            for (; *mod_err_id<F_NO_ERR; (*mod_err_id)++)
            {
                /* ����forѭ����ͷ��ʹk�ܼ�1���´β����ظ���ȡk */
                if (fg_err)  break;

                if (m_get_error(*mod_id, *mod_err_id))
                {
                    fg_err = TRUE;

                    /* ע�⣡����������Ƶ���ϸ�Ϊϵͳ������ϣ���Ҫ���Ĵ˴� */
                    if (*mod_err_id == err_invt_id_to_err_id(INVT_ID_1, INVT_ERR_ERROR))
                    {
                        if (fun_pv_in(CELL_INVT_ERR(*mod_id, INVT_ID_1), INVT_ERR_ID_MIN, INVT_ERR_ID_MAX))
                        {
                            err->name = msg_invt_error[CELL_INVT_ERR(*mod_id, INVT_ID_1)-1];
                        }
                        else
                        {
                            err->name = msg_invt_error_unknown;
                        }
                    }
                    else if(*mod_err_id == S_ERR_PUMPF)
                    {
                        if (fun_pv_in_open(pv_cell_PUMPf_err_code(*mod_id), PUMF_NO_ERR, PUMPF_ERR_MAX))
                        {
                            err->name = msg_PUMPf_error[pv_cell_PUMPf_err_code(*mod_id)-1];
                        }
                        else
                        {
                            err->name = msg_PUMPf_error_unknown;
                        }
                    }
                    else
                    {
                        err->name = SHC_NAME_S_ERR(*mod_err_id);
                    }
                    err->id = MAKEWORD(*mod_id, NO_ID);
                    err->err_id = *mod_err_id + (*mod_id+1)*M_ERR_MAX + 1;
                }
            }
            /* ����forѭ���Ľ�β����ֹc��1,������©��ģ����������� */
            if (fg_err)  break;
        }
    }

    return fg_err;
}

/***********************************************************************
 �� ��: ��ʷ������Ϣ
 �� ��:
************************************************************************/
#define SHC_MAX_HIS_NUM                     (MAX_HIS_ERROR)
#define SHC_HIS_ERR_TBL_SIZE                (HIS_ERR_SIZE)
#define SHC_NO_ERR                          (F_NO_ERR)
void SHC_get_his_err_info(SH_ERR_HIS *err, U08 idx)
{
    U08 i;
    U08 err_id = 0;
    U08 err_pcb = 0;
    U08 sign = 0;
    U08 his_err_one[SHC_HIS_ERR_TBL_SIZE];

    /* ��ȡ��ʷ���� */
    for (i=0; i<SHC_HIS_ERR_TBL_SIZE; i++)
    {
        his_err_one[i] = his_err_tbl[idx*SHC_HIS_ERR_TBL_SIZE+i];
    }

    /* ����������š����Ϻš�����ʱ�� */
    err_id = SHC_NO_ERR;
    if (idx < SHC_MAX_HIS_NUM)
    {
        err_id  = his_err_one[0];
        err_pcb = his_err_one[1];
        sign    = his_err_one[2];
    }
    if (err_id < F_NO_ERR)
    {
        if (err_pcb == MAX_PCB)         /* ϵͳ���� */
        {
            err->name = SHC_NAME_M_ERR(err_id);
            err->id = MAKEWORD(NO_ID, NO_ID);
            err->err_id = err_id;
        }
        else            /* ��Ԫ���� */
        {
            if (err_id == err_invt_id_to_err_id(INVT_ID_1, INVT_ERR_ERROR))
            {
                if (fun_pv_in(sign, INVT_ERR_ID_MIN, INVT_ERR_ID_MAX))
                {
                    err->name = msg_invt_error[sign-1];
                }
                else
                {
                    err->name = msg_invt_error_unknown;
                }
            }
            else if(err_id == S_ERR_PUMPF)
            {
                if (fun_pv_in_open(sign, PUMF_NO_ERR, PUMPF_ERR_MAX))
                {
                    err->name = msg_PUMPf_error[sign-1];
                }
                else
                {
                    err->name = msg_PUMPf_error_unknown;
                }
            }
            else
            {
                err->name = SHC_NAME_S_ERR(err_id);
            }

            err->id = MAKEWORD(err_pcb, NO_ID);
            err->err_id = err_id + (err_pcb+1)*M_ERR_MAX + 1;
        }
        err->time.month  = his_err_one[3];
        err->time.day    = his_err_one[4];
        err->time.hour   = his_err_one[5];
        err->time.mint   = his_err_one[6];
    }
    else
    {
        err->name = SH_ERR_WEI_ZHI_GZ;  /* ��Ϊ"δ֪����"����ʾ */
        err->id = 0;
        err->err_id = 0;
        err->time.month = 0;
        err->time.day = 0;
        err->time.hour = 0;
        err->time.mint= 0;
    }
}

/***********************************************************************
 �� ��: ������Ϣ
 �� ��: ��û�з�����Ϣ�������д���
************************************************************************/
void SHC_get_fcu_info(SH_FCU *info)
{
#if 0
    U08 i;

    static const U16 status[] =
    {
        dm603_face_menu_fcu_sta_00, // δ����
        dm603_face_menu_fcu_sta_01, // ����̬
        dm603_face_menu_fcu_sta_02, // ����̬
        dm603_face_menu_fcu_sta_03, // ������
        dm603_face_menu_fcu_sta_04, // ������
        dm603_face_menu_fcu_sta_05, // δ����
    };
    static const U16 mode[] =
    {
        dm603_nothing,
        dm603_face_menu_fcu_mode_01,    // ����
        dm603_face_menu_fcu_mode_02,    // ����
        dm603_face_menu_fcu_mode_03,    // ͨ��
        dm603_face_menu_fcu_mode_04,    // ��ů
        dm603_face_menu_fcu_mode_05,    // δ֪
    };

    info->use_num = val_fcu_num;
    for (i=0; i<SH_FCU_MAX; i++)
    {
        if (i<val_fcu_num)
        {
            info->fcuinfo[i].str_mode = mode[MIN(fcu_sta_mode(i), 5)];
            info->fcuinfo[i].str_status = status[MIN(fcu_state_get(i), 5)];
            info->fcuinfo[i].temp_indoor = fcu_temp_indoor(i);
        }
        else
        {
            info->fcuinfo[i].str_mode = dm603_nothing;
            info->fcuinfo[i].str_status = dm603_nothing;
            info->fcuinfo[i].temp_indoor = 0;
        }
    }
#endif
}

/***********************************************************************
 �� ��: ģʽ��Ϣ
 �� ��: ������ʾ���Ķ�ʱ����
************************************************************************/
void SHC_get_mode_set_info(SH_MODE_SET_INFO *info)
{
    U08 i;
    U08 num = 0;

    for (i=0; i<SH_MODE_INFO_MAX; i++)
    {
        info->mode_info[i].mode_str = 0;
        info->mode_info[i].mode_key = 0;
        info->mode_info[i].temp_set_min = 0;
        info->mode_info[i].temp_set_max = 0;
        info->mode_info[i].temp_set_cmd = 0;
    }

    if (USE_COOL)
    {
        info->mode_info[num].mode_str = msg_face_main_mode_01;
        info->mode_info[num].mode_key = SH_MODE_COOL;
        info->mode_info[num].mode_cmd_on  = SH_CMD_ON_ZONE_1;
        info->mode_info[num].mode_cmd_off = SH_CMD_OFF_ZONE_1;
        info->mode_info[num].temp_set_min = para_get_min(P_GaI(sv_cool));
        info->mode_info[num].temp_set_max = para_get_max(P_GaI(sv_cool));
        info->mode_info[num].temp_set_cmd = SET_TMP_SET_ZONE_1;
        num++;
    }

    if (USE_HEAT)
    {
        info->mode_info[num].mode_str = msg_face_main_mode_02;
        info->mode_info[num].mode_key = SH_MODE_HEAT;
        info->mode_info[num].mode_cmd_on  = SH_CMD_ON_ZONE_1;
        info->mode_info[num].mode_cmd_off = SH_CMD_OFF_ZONE_1;
        info->mode_info[num].temp_set_min = para_get_min(P_GaI(sv_heat));
        info->mode_info[num].temp_set_max = para_get_max(P_GaI(sv_heat));
        info->mode_info[num].temp_set_cmd = SET_TMP_SET_ZONE_1;
        num++;
    }

    if (val_hotw_used == USED)
    {
        info->mode_info[num].mode_str = msg_face_main_mode_03;
        info->mode_info[num].mode_key = SH_MODE_HOTWATER;
        info->mode_info[num].mode_cmd_on  = SH_CMD_ON_HOTW;
        info->mode_info[num].mode_cmd_off = SH_CMD_OFF_HOTW;
        info->mode_info[num].temp_set_min = para_get_min(P_GaI(sv_hotw));
        info->mode_info[num].temp_set_max = para_get_max(P_GaI(sv_hotw));
        info->mode_info[num].temp_set_cmd = SET_TMP_SET_ZONE_HOTW;
        num++;
    }

    info->mode_num = num;

}

/***********************************************************************
 �� ��: ��������Ϣ
 �� ��: ���ڶ�����������
************************************************************************/
void SHC_get_main_info(SH_MAIN_INFO *info)
{
    /* A��ĩ������ */
    info->zone_1_mode            = ZONE_MODE_FCU;           /* Ĭ��: ���� */
    if (ZONE_A_TERMINAL_DEVICE == TD_RAD)
    {
        info->zone_1_mode        = ZONE_MODE_RAD;           /* ɢ��Ƭ */
    }
    else if (ZONE_A_TERMINAL_DEVICE == TD_FCU)
    {
        info->zone_1_mode        = ZONE_MODE_FCU;           /* ���� */
    }
    else if (ZONE_A_TERMINAL_DEVICE == TD_FLH)
    {
        info->zone_1_mode        = ZONE_MODE_FLOOR;         /* ��ů */
    }
    /* A���Ƿ�������״̬ */
    info->fg_zone_1_is_run       = fg_zone_A_run;
    /* A���¶ȿ������� */
    info->zone_1_temp_type       = temp_ctrl_type_zone_A;
    /* A���趨�¶��Ƿ���޸� */
    info->fg_zone_1_can_set_temp = temp_set_can_edit_zone_A;
    /* A��ѹ������ */
    info->fg_zone_1_comp_on      = fg_zone_A_run
                                   && MAC_IS_RUN
                                   && log_sys_ac_have_comp_run();
    /* A��״̬ */
    info->zone_1_status          = ZONE_STA_NULL;
    if (fg_ability_test_run)
    {
        info->zone_1_status      = ZONE_STA_TEST;           /* �������� */
    }
    else if (fg_refri_recy_act)
    {
        info->zone_1_status      = ZONE_STA_RECYCLE;		/* �շ� */
    }
    else if (ZONE_A_AF)
    {
        info->zone_1_status      = ZONE_STA_ANTIFREZ;       /* ���� */
    }
    else if ((!DISP_IS_HOTW(SUB_SYS_AC_HOTW) && log_sys_defrost_comp_num(SUB_SYS_AC_HOTW))
    || log_sys_defrost_comp_num(SUB_SYS_AC_ONLY)
    )
    {
        info->zone_1_status      = ZONE_STA_DEFRO;          /* ��˪ */
    }
    else if (fg_holiday_home_run || fg_holiday_away_run)
    {
        info->zone_1_status      = ZONE_STA_HOLIDAY;        /* ���� */
    }
    else if (fg_silent_mode)
    {
        info->zone_1_status      = ZONE_STA_SILENT;         /* ���� */
    }
    else if (ModeIsEco)
    {
        info->zone_1_status      = ZONE_STA_ECO;            /* ���� */
    }
    info->zone_1_addition        = ZONE_ADDITION_NULL;
    if (fg_zone_A_run && MAC_IS_RUN && HEAT_USE_AHS && log_sys_have_AHS_on(ZONE_AC))
    {
        info->zone_1_addition    = ZONE_ADDITION_HEAT;      /* �ⲿ��ԴAHS */
    }
    else if (fg_zone_A_run && MAC_IS_RUN && HEAT_USE_IBH && log_sys_ac_have_IBH_on())
    {
        info->zone_1_addition    = ZONE_ADDITION_ELEC;      /* ��������IBH */
    }
    /* A��δ���� */
    if (!fg_zone_A_run)
    {
        info->zone_1_addition    = ZONE_ADDITION_NULL;
    }

    /* B��ĩ������ */
    info->zone_2_mode            = ZONE_MODE_FLOOR;         /* Ĭ��: ���� */
    if (ZONE_B_TERMINAL_DEVICE == TD_RAD)
    {
        info->zone_2_mode        = ZONE_MODE_RAD;           /* ɢ��Ƭ */
    }
    else if (ZONE_B_TERMINAL_DEVICE == TD_FCU)
    {
        info->zone_2_mode        = ZONE_MODE_FCU;           /* ���� */
    }
    else if (ZONE_B_TERMINAL_DEVICE == TD_FLH)
    {
        info->zone_2_mode        = ZONE_MODE_FLOOR;         /* ��ů */
    }
    /* B���Ƿ�������״̬ */
    info->fg_zone_2_is_run       = fg_zone_B_run;
    /* B���¶ȿ������� */
    info->zone_2_temp_type       = temp_ctrl_type_zone_B;
    /* B���趨�¶��Ƿ���޸� */
    info->fg_zone_2_can_set_temp = temp_set_can_edit_zone_B;
    /* B��ѹ������ */
    info->fg_zone_2_comp_on      = fg_zone_B_run 
                                   && MAC_IS_RUN 
                                   && log_sys_ac_have_comp_run();
    /* B��״̬ */
    info->zone_2_status          = ZONE_STA_NULL;
//    if (fg_ability_test_run)
//    {
//        info->zone_2_status      = ZONE_STA_TEST;           /* �������� */
//    }
//    else
    if (ZONE_B_AF)
    {
        info->zone_2_status      = ZONE_STA_ANTIFREZ;       /* ���� */
    }
    else if ((!DISP_IS_HOTW(SUB_SYS_AC_HOTW) && log_sys_defrost_comp_num(SUB_SYS_AC_HOTW))
    || log_sys_defrost_comp_num(SUB_SYS_AC_ONLY)
    )
    {
        info->zone_2_status      = ZONE_STA_DEFRO;          /* ��˪ */
    }
    else if (fg_holiday_home_run || fg_holiday_away_run)
    {
        info->zone_2_status      = ZONE_STA_HOLIDAY;        /* ע: B���޼��� */
    }
    else if (fg_silent_mode)
    {
        info->zone_2_status      = ZONE_STA_SILENT;         /* ���� */
    }
    else if (ModeIsEco)
    {
        info->zone_2_status      = ZONE_STA_ECO;            /* ���� */
    }
    info->zone_2_addition        = ZONE_ADDITION_NULL;
    if (fg_zone_B_run && MAC_IS_RUN && HEAT_USE_AHS && log_sys_have_AHS_on(ZONE_AC))
    {
        info->zone_2_addition    = ZONE_ADDITION_HEAT;      /* �ⲿ��ԴAHS */
    }
    else if (fg_zone_B_run && MAC_IS_RUN && HEAT_USE_IBH && log_sys_ac_have_IBH_on())
    {
        info->zone_2_addition    = ZONE_ADDITION_ELEC;      /* ��������IBH */
    }
    /* B��δ���� */
    if (!fg_zone_B_run)
    {
        info->zone_2_addition    = ZONE_ADDITION_NULL;
    }

    /* ����ˮ�Ƿ����� */
    info->fg_zone_hotw_is_run    = fg_hotw_run;
    info->fg_zone_hotw_disinfect = fg_timed_disinfect_run;
    if (!MODE_IS_COOL(SUB_SYS_AC_HOTW) )		/* ������ʱ���Ŵ���ѹ��ͼ����ʾ */
    {
        info->fg_zone_hotw_comp_on   = fg_hotw_run 
                                       && MAC_IS_RUN 
                                       && log_sys_hotw_have_comp_run();
    }    
//    if (fg_ability_test_run)
//    {
//        info->zone_hotw_status   = ZONE_STA_TEST;           /* �������� */
//    }
//    else
    if (HOTW_AF)
    {
        info->zone_hotw_status   = ZONE_STA_ANTIFREZ;       /* ���� */
    }
    else if (DISP_IS_HOTW(SUB_SYS_AC_HOTW) && log_sys_defrost_comp_num(SUB_SYS_AC_HOTW))
    {
        info->zone_hotw_status   = ZONE_STA_DEFRO;          /* ��˪ */
    }
    else if (fg_holiday_away_run)
    {
        info->zone_hotw_status   = ZONE_STA_HOLIDAY;        /* ���� */
    }
    else if (fg_silent_mode)
    {
        info->zone_hotw_status   = ZONE_STA_SILENT;         /* ���� */
    }
    else if (ModeIsEco)
    {
        info->zone_hotw_status   = ZONE_STA_ECO;            /* ���� */
    }
    info->zone_hotw_addition     = ZONE_ADDITION_NULL;
    if (fg_hotw_run && MAC_IS_RUN && HOTW_USE_AHS && log_sys_have_AHS_on(ZONE_HOTW))
    {
        info->zone_hotw_addition   = ZONE_ADDITION_HEAT;    /* �ⲿ��ԴAHS */
    }
    else if (val_solar_used && get_output(MAX_PCB, M_DO_PUMPsl))
    {
        info->zone_hotw_addition   = ZONE_ADDITION_SOLAR;   /* ̫����ˮ��PUMPsl */
    }
    else if (val_TBH_used && get_output(MAX_PCB, M_DO_TBH))
    {
        info->zone_hotw_addition   = ZONE_ADDITION_TANK_HEATER;     /* ˮ�����TBH */
    }
    else if (fg_hotw_run && MAC_IS_RUN && HOTW_USE_IBH && log_sys_hotw_have_IBH_on())
    {
        info->zone_hotw_addition   = ZONE_ADDITION_ELEC;    /* ��������IBH */
    }

    /* ��ˮδ���� */
    if (!fg_hotw_run)
    {
        info->zone_hotw_addition   = ZONE_ADDITION_NULL;
    }

    /* ��Ƶˮ��PUMPf */
    if (log_sys_cell_pumpf_on())
    {
        info->fg_pumpi_is_on       = TRUE;
    }
    else
    {
        info->fg_pumpi_is_on       = FALSE;
    }

    /* ���ܵ��� */
    switch (smart_grid_cmd)
    {
        case SG_CMD_1:
            info->smart_grid_sta = SMART_GRID_VALLEY;       /* �͹��õ� */
            break;
        case SG_CMD_2:
            info->smart_grid_sta = SMART_GRID_PEAK;         /* �߷��õ� */
            break;
        case SG_CMD_3:
        case SG_CMD_4:
            info->smart_grid_sta = SMART_GRID_FREE;         /* ����õ� */
            break;
    }

    /* ���䴦��: ���� */
    if (FG_AF_ALL)
    {
        /* A�� */
        if (idle_sta_antifreeze_stage==AF_STAGE_AC              /* ����̬�յ�ˮ·���� */
        ||  fg_run_sta_ac_antifreeze			                /* ����̬�յ�ˮ·���� */
        ||  ZONE_A_TAF				                            /* A����ʱ���� */
        )
        {
            info->fg_zone_1_comp_on = log_sys_ac_have_comp_run();

            if (log_sys_have_AHS_on(ZONE_AC))
            {
                info->zone_1_addition = ZONE_ADDITION_HEAT;      /* �ⲿ��ԴAHS */
            }
            else if (log_sys_ac_have_IBH_on())
            {
                info->zone_1_addition = ZONE_ADDITION_ELEC;      /* ��������IBH */
            }
        }

        /* B�� */
        if (idle_sta_antifreeze_stage==AF_STAGE_AC              /* ����̬�յ�ˮ·���� */
        ||  fg_run_sta_ac_antifreeze			                /* ����̬�յ�ˮ·���� */
        ||  ZONE_B_TAF				                            /* B����ʱ���� */
        )
        {
            info->fg_zone_2_comp_on = log_sys_ac_have_comp_run();

            if (log_sys_have_AHS_on(ZONE_AC))
            {
                info->zone_2_addition = ZONE_ADDITION_HEAT;      /* �ⲿ��ԴAHS */
            }
            else if (log_sys_ac_have_IBH_on())
            {
                info->zone_2_addition = ZONE_ADDITION_ELEC;      /* ��������IBH */
            }
        }
        
        /* ��ˮ�� */
        if (idle_sta_antifreeze_stage==AF_STAGE_HOTW            /* ����̬��ˮˮ·���� */
        ||  fg_run_sta_hotw_antifreeze			                /* ����̬��ˮˮ·���� */
        ||  HOTW_TAF				                            /* ��ˮ��ʱ���� */
        )
        {
            info->fg_zone_hotw_comp_on = log_sys_hotw_have_comp_run();

            if (log_sys_have_AHS_on(ZONE_HOTW))
            {
                info->zone_hotw_addition = ZONE_ADDITION_HEAT;    /* �ⲿ��ԴAHS */
            }
            else if (log_sys_hotw_have_IBH_on())
            {
                info->zone_hotw_addition = ZONE_ADDITION_ELEC;    /* ��������IBH */
            }
        }
    }
}

/***********************************************************************
 �� ��: ��ȡ��Ƶ������Ϣ
 �� ��:
************************************************************************/
void SHC_get_invt_err_info(SH_CHK_INFO *info, U16 idx)
{
    U16 attr;

    attr = msg_invt_para[idx].attr;

    info->name = msg_invt_para[idx].msg;
    info->id = 0xff;
    if (attr == 0xFFFF)
    {
        if (inv_err_para_info[idx] <= 0)
        {
            info->value = msg_nothing;
        }
        else
        {
            info->value = msg_invt_error_code[inv_err_para_info[idx]-1];
        }
        info->attrib = UNIT_OFFSET;
    }
    else
    {
        info->value = inv_err_para_info[idx];
        info->attrib = SH_sign_local2sh(attr);
    }
}

/***********************************************************************
 �� ��: ����������
 �� ��: ����:Ȩ�ޡ�����;
************************************************************************/
U16 SHC_get_para_grp_sign(U08 grp)
{
    extern PARAMITEM  ParamPri[];
    U16 sign = 0;
    U08 i;
    U16 min_pwr = D_FAC;

	/* �Ȼ�ȡ����Ȩ����������� */
    sign = ParamPri[grp].sign & (~D_SAFEMASK);

	/* ��ѯ��������Ҫ��ʾ�Ĳ��������Ҫ��Ȩ�� */
	for (i=0; i<ParamPri[grp].items; i++)
	{
		U16 para_sign = para_get_sign(grp, i);

		if (D_HIDE != (para_sign & D_HIDE))
		{
			para_sign &= D_SAFEMASK;

			min_pwr = MIN(min_pwr, para_sign);
		}
	}

	/* ��Ȩ�����Ժϲ��������� */
	sign |= min_pwr;

    return sign;
}

/***********************************************************************
 �� ��: ������Ĳ�������
 �� ��:
************************************************************************/
#define SHC_get_para_grp_item(grp)      (ParamPri[grp].items)

/***********************************************************************
 �� ��: �������ַ���
 �� ��:
************************************************************************/
#define SHC_NAME_PARA_GRP(grp)          (para_large[grp])   /* ���������� */

/***********************************************************************
 �� ��: ��ȡ�ֵ�����
 �� ��:
************************************************************************/
#define SHC_GET_DICT_NUM                (get_dict_num())

/***********************************************************************
 �� ��: ��ȡ�ֵ����
 �� ��:
************************************************************************/
U16 SHC_get_dict_idx(const U16 *pmsg)
{
    U16 i;

    if (pmsg != NULL)
    {
        for (i=0; i<SHC_GET_DICT_NUM; i++)
        {
            if(para_dict[i].pmsg == pmsg)
            {
                return i;
            }
        }
    }

    return 0;
}

/***********************************************************************
 �� ��: ��ȡ�ֵ������
 �� ��:
************************************************************************/
#define SHC_get_dict_items(dict_idx)        (para_dict[dict_idx].items)

/***********************************************************************
 �� ��: ��ȡ�ֵ�ָ��
 �� ��:
************************************************************************/
#define SHC_get_dict_pmsg(x, y)         (para_dict[x].pmsg[y])

/***********************************************************************
 �� ��: ��ȡ��������
 �� ��:
************************************************************************/
#define SHC_get_para_get_sign(grp, item)    (para_get_sign(grp, item))

/***********************************************************************
 �� ��: ��ȡ�����ַ���ѡ��
 �� ��:
************************************************************************/
#define SHC_get_para_small_pmsg(grp, item)  (para_small[grp][item].pmsg)

/***********************************************************************
 �� ��: ��ȡ���������ַ���
 �� ��:
************************************************************************/
#define SHC_NAME_PARA(grp, item)            (para_small[(grp)][(item)].msg) /* �������� */

/***********************************************************************
 �� ��: ��ȡ����ֵ
 �� ��:
************************************************************************/
I16 SHC_para_get_val(U16 grp, U16 item)
{
    return para_get_val(grp, item);
}

/***********************************************************************
 �� ��: ��ȡ������������
 �� ��:
************************************************************************/
#define SHC_para_get_min(grp, item)         (para_get_min(grp, item))

/***********************************************************************
 �� ��: ��ȡ������������
 �� ��:
************************************************************************/
#define SHC_para_get_max(grp, item)         (para_get_max(grp, item))

/***********************************************************************
 �� ��: BMS����
 �� ��: ��û�д˹��ܣ���㸳ֵʹ���벻����������SH_DISP_HAVE_BMS_SET��ΪFALSE
************************************************************************/
#define SHC_BMS_PARA_NUM            2       /* ���ʵ��BMSͨѶ�������������ݺ�������������� */

#define SHC_BMS_PARA_NAME(item)     (0)     /* (para_small_bms[item].msg)      // BMSͨѶ���ò������� */
#define SHC_BMS_PARA_STR(item)      (NULL)  /* (para_small_bms[item].pmsg)     // BMSͨѶ���ò����ַ��� */
#define SHC_BMS_PARA_VAL(item)      (0)     /* (modbus_para[item])             // BMSͨѶ���ò���ֵ */
#define SHC_BMS_PARA_MIN(item)      (0)     /* (modbus_para_data[item].min)    // BMSͨѶ���ò�����Сֵ */
#define SHC_BMS_PARA_MAX(item)      (0)     /* (modbus_para_data[item].max)    // BMSͨѶ���ò�����Сֵ */
#define SHC_BMS_PARA_SIGN(item)     (0)     /* (modbus_para_data[item].sign)   // BMSͨѶ���ò������� */

/***********************************************************************
 �� ��: ��ģ��ѹ����,��8λΪ��չ�����
 �� ��: ��ֻ�е�ģ�飬ֱ�ӷ���ѹ����
************************************************************************/
#define SHC_pcb_comp_num(pcb_id)    (MAKEWORD(1, val_comp_num))

/***********************************************************************
 �� ��: �ֶ�����DO���
 �� ��: �����ô˹��ܣ���㸳ֵ���ַ���������SHC_DISP_GET_TEST_EN��ΪFALSE
************************************************************************/
typedef struct
{
    U16 string;
    U16 pcb_id;
    U16 comp_id;
    U16 do_id;
}SHC_TEST_DO;
const SHC_TEST_DO SHC_test_do_info[] =
{
    /* �ڻ����� */
    { msg_SHC_test_out_DFR,        MAX_PCB,        0xff,       M_DO_DFR,     }, /* ��˪ */
    { msg_SHC_test_out_ERR,        MAX_PCB,        0xff,       M_DO_ERR,     }, /* ���� */
    { msg_SHC_test_out_AHS,        MAX_PCB,        0xff,       M_DO_AHS,     }, /* �ⲿ��Դ */
    { msg_SHC_test_out_S3V1,       MAX_PCB,        0xff,       M_DO_S3V1,    }, /* �����ͨ��1 */
    { msg_SHC_test_out_S3V3,       MAX_PCB,        0xff,       M_DO_S3V3,    }, /* �����ͨ��3 */
    { msg_SHC_test_out_S3V2,       MAX_PCB,        0xff,       M_DO_S3V2,    }, /* �����ͨ��2 */
    { msg_SHC_test_out_HT,               0,    MAX_COMP,       S_DO_HT,      }, /* �������ȴ� */
    { msg_SHC_test_out_PUMPret,    MAX_PCB,        0xff,       M_DO_PUMPret, }, /* �»�ˮ�� */
    { msg_SHC_test_out_PUMPsl,     MAX_PCB,        0xff,       M_DO_PUMPsl,  }, /* ̫����ˮ�� */
    { msg_SHC_test_out_PUMPm,      MAX_PCB,        0xff,       M_DO_PUMPm,   }, /* ���ˮ�� */
    { msg_SHC_test_out_TBH,        MAX_PCB,        0xff,       M_DO_TBH,     }, /* ˮ����� */
    { msg_SHC_test_out_IBH1,             0,    MAX_COMP,       S_DO_IBH1,    }, /* ��������1 */
    { msg_SHC_test_out_IBH2,             0,    MAX_COMP,       S_DO_IBH2,    }, /* ��������2 */
    { msg_SHC_test_out_PUMPo,      MAX_PCB,        0xff,       M_DO_PUMPo,   }, /* ��ѭ���� */
    { msg_SHC_test_out_PUMPf,            0,    MAX_COMP,       S_DO_PUMPf,   }, /* ��Ƶˮ�� */

    /* ������� */
    { msg_SHC_test_out_QZH1,             0,           0,       SW_crank1,    },	/* ��������1 */
    { SHC_EXDO7_MSG,            SHC_EXDO7_PCB, SHC_EXDO7_CM,   SHC_EXDO7_DO, },	/* ����2\����2 */
    { msg_SHC_test_out_DPH,              0,    MAX_COMP,       S_DO_DPH,     },	/* ������ */
    { msg_SHC_test_out_RV,               0,           0,       SW_S1,        },	/* ��ͨ�� */
    { msg_SHC_test_out_EXVli,            0,           0,       SW_byway,     },	/* ��Һ�� */
};
#define SHC_TEST_DO_NUM             (MIN(ARRELE(SHC_test_do_info), MAX_CHK_TEST_DO_NUM))      /* ���ܳ������ */


/*******************************************************************************
 �� ��: SHC_test_do_timer()
 �� ��: DO��������ʱ���ʱ
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void SHC_test_do_timer(void)
{
    U08 i;
    if (test_data.wo.DO_mnl_mode)
    {
        for (i=0; i<SHC_TEST_DO_NUM; i++)
        {
            /* ϵͳ */
            if (SHC_test_do_info[i].pcb_id == MAX_PCB)
            {
                if (SHC_test_do_info[i].do_id == M_DO_TBH)
                {
                    _CNT_WHEN(cnt_test_do_TBH, GETBIT_N(test_data.wr.DO_sta, i));
                }
            }
            /* ģ�� */
            else if (SHC_test_do_info[i].comp_id == MAX_COMP)
            {
                if (SHC_test_do_info[i].do_id == S_DO_IBH1)
                {
                    _CNT_WHEN(cnt_test_do_IBH1, GETBIT_N(test_data.wr.DO_sta, i));
                }
                if (SHC_test_do_info[i].do_id == S_DO_IBH2)
                {
                    _CNT_WHEN(cnt_test_do_IBH2, GETBIT_N(test_data.wr.DO_sta, i));
                }
                if (SHC_test_do_info[i].do_id == S_DO_HT)
                {
                    _CNT_WHEN(cnt_test_do_HT, GETBIT_N(test_data.wr.DO_sta, i));
                }
            }
            /* ѹ�� */
            else
            {
                ;
            }
        }
    }
    else
    {
        cnt_test_do_IBH1 = 0;
        cnt_test_do_IBH2 = 0;
        cnt_test_do_TBH = 0;
        cnt_test_do_HT = 0;
    }
}

/*******************************************************************************
 �� ��: SHC_set_sys_test_do()
 �� ��: �ֶ�����ϵͳDO�������
 �� ��: ��
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SHC_set_sys_test_do(void)
{
    U08 i;
    U08 fg_test_out_mnl;

    fg_test_out_mnl = test_data.wo.DO_mnl_mode;

    if (STEP_IS_IDLE)
    {
        test_data.ro.fg_DO_can_mnl_mode = TRUE;
    }
    else
    {
        test_data.ro.fg_DO_can_mnl_mode = FALSE;
        fg_test_out_mnl = FALSE;
    }

    if (fg_test_out_mnl)
    {
        for (i=0; i<SHC_TEST_DO_NUM; i++)
        {
            if (SHC_test_do_info[i].pcb_id == MAX_PCB)  // ϵͳ������ڴ˴���
            {
                clr_output(MAX_PCB, SHC_test_do_info[i].do_id);
                if (GETBIT_N(test_data.wr.DO_sta, i))
                {
                    set_output(MAX_PCB,SHC_test_do_info[i].do_id);
                }
            }
        }

        /* ˮ����ȿ�����5s���Զ��رգ���ֹ���� */
        if (cnt_test_do_TBH > 5)
        {
            clr_output(MAX_PCB, M_DO_TBH);
        }

        return TRUE;
    }
    else
    {
        ARRCLR(test_data.wr.DO_sta);
    }

    return FALSE;
}

/*******************************************************************************
 �� ��: SHC_set_mod_test_do()
 �� ��: �ֶ�����ģ��DO�������
 �� ��: ��
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL SHC_set_mod_test_do(void)
{
    U08 i;
    U08 fg_test_out_mnl;

    fg_test_out_mnl = test_data.wo.DO_mnl_mode;

    if (STEP_IS_IDLE)
    {
        test_data.ro.fg_DO_can_mnl_mode = TRUE;
    }
    else
    {
        test_data.ro.fg_DO_can_mnl_mode = FALSE;
        fg_test_out_mnl = FALSE;
    }

    if (fg_test_out_mnl)
    {
        for (i=0; i<SHC_TEST_DO_NUM; i++)
        {
            if (SHC_test_do_info[i].pcb_id != MAX_PCB)  // ģ��������ڴ˴���
            {
                s_clr_output(SHC_test_do_info[i].comp_id, SHC_test_do_info[i].do_id);
                if (GETBIT_N(test_data.wr.DO_sta, i))
                {
                    s_set_output(SHC_test_do_info[i].comp_id, SHC_test_do_info[i].do_id);
                }
            }
        }

        if (get_output(MAX_PCB,M_DO_TBH)	/* TBH����ʱ��IBH1���ֹر� */
        || !IBH1_IS_USE	                    /* ��IBH1ʱ��������IBH1 */
        || cnt_test_do_IBH1 > 5				/* IBH1������5s���Զ��رգ���ֹ���� */
        )
        {
            s_clr_output(MAX_COMP, S_DO_IBH1);
        }
        if (get_output(MAX_PCB,M_DO_TBH)	/* TBH����ʱ��IBH2���ֹر� */
        || !IBH2_IS_USE	                    /* ��IBH2ʱ��������IBH2 */
        || cnt_test_do_IBH2 > 5				/* IBH2������5s���Զ��رգ���ֹ���� */
        )
        {
            s_clr_output(MAX_COMP, S_DO_IBH2);
        }
        /* ���������ȴ�������5s���Զ��رգ���ֹ���� */
        if (cnt_test_do_HT > 5)
        {
            s_clr_output(MAX_COMP, S_DO_HT);
        }

        return TRUE;
    }
    else
    {
        ARRCLR(test_data.wr.DO_sta);
    }

    return FALSE;
}

U16 SHC_get_test_DO_string(U08 i)
{
    return SHC_test_do_info[i].string;
}
/***********************************************************************
 �� ��: ��ǰ�Ƿ������ֶ�DO
 �� ��: �����ô˹��ܣ�return FALSE
************************************************************************/
BOOL SHC_get_DO_can_mnl(void)
{
    if (status_run == _IDLE)
    {
        return TRUE;
    }

    return FALSE;
}
/***********************************************************************
 �� ��: �ֶ�DO���״̬
 �� ��: �����ô˹��ܣ�return FALSE
************************************************************************/
BOOL SHC_get_test_do_sta(U16 id)
{
    if (SHC_test_do_info[id].pcb_id == MAX_PCB)
    {
        if (get_output(MAX_PCB, SHC_test_do_info[id].do_id))
        {
            return TRUE;
        }
    }
    else
    {
        if (s_get_output(SHC_test_do_info[id].comp_id, SHC_test_do_info[id].do_id))
        {
            return TRUE;
        }
    }

    return FALSE;
}

/***********************************************************************
 �� ��: �ֶ�����EEV
 �� ��: �����ô˹��ܣ���㸳ֵ���ַ���������SHC_DISP_GET_TEST_EN��ΪFALSE
************************************************************************/
//typedef struct
//{
//    U16 string;
//    U16 comp_id;
//    U16 eev_type;
//}SHC_TEST_EEV;
//const SHC_TEST_EEV SHC_test_eev_info[] =
//{
//    {msg_SHC_test_eev_01,     0,      0,},    /* {��·����1  } */
//};
//#define SHC_TEST_EEV_NUM            ((ARRELE(SHC_test_eev_info) > MAX_CHK_TEST_EEV_NUM) ? \
//                                    (MAX_CHK_TEST_EEV_NUM) : \
//                                    (ARRELE(SHC_test_eev_info)))     /* ���ܳ������ */
//U16 SHC_get_test_eev_string(U08 i)
//{
//    return SHC_test_eev_info[i].string;
//}

#define SHC_TEST_EEV_NUM              (0)
U16 SHC_get_test_eev_string(U08 i)
{
    return 0;
}

/***********************************************************************
 �� ��: �ֶ�����EEV���ֶ������
 �� ��: �����ô˹��ܣ�return 0
************************************************************************/
I16 SHC_get_motor_max(U16 id)
{
    if (id)
    {
        return 0;
    }
    return 0;
}
/***********************************************************************
 �� ��: �ֶ�����EEV�ĵ�ǰ����
 �� ��: �����ô˹��ܣ�return 0
************************************************************************/
I16 SHC_get_motor_pv(U16 id)
{
    if (id)
    {
        return 0;
    }
    return 0;
}

/***********************************************************************
 �� ��: ѹ��������Ϣ:����ʱ�䣬�ŵ�״̬��ѯ������ʾ
 �� ��:
************************************************************************/
#define SHC_COMP_EX_PER         1
#define SHC_COMP_EX_NUM         (SHC_USE_COMP*SHC_COMP_EX_PER)   /* ѹ��������Ϣ����  ��:ѹ������ʱ�� */

/***********************************************************************
 �� ��: ����ģ��ģ��������
 �� ��:
************************************************************************/
U08 SHC_common_count_mod_pv(U08 *dest, I16 *src)
{
    U16 i;
    U08 cursor = 0;

    /* ��Чģ���� */
    for (i = CELL_PV_BEGIN; i < MAX_PV_WORD; i++)
    {
        if (*src++ != SENSOR_NOEXIST)       /* ��Чģ�����ж� */
        {
            if(dest != PNULL)
            {
                dest[cursor] = i;
            }
            cursor++;
        }
    }

    return cursor;
}
/***********************************************************************
 �� ��: ����ѹ��ģ��������
 �� ��:
************************************************************************/
U08 SHC_common_count_comp_pv(U08 *dest, U08 pcb_num, U08 comp_num)
{
    U08 i, j;
    U08 cursor = 0;
    U08 exist = FALSE;

    for (i=0; i<comp_num*COMP_PV_NUM; i++)
    {
        exist = FALSE;
        for  (j=0; j<comp_num; j++)
        {
            /* ����һ����Ч����Ч */
            if(cell[pcb_num].cell_pv[i%COMP_PV_NUM+j*COMP_PV_NUM] != SENSOR_NOEXIST)
            {
                exist = TRUE;
                break;
            }
        }

        if (exist)
        {
            if(dest != PNULL)
            {
                dest[cursor] = i;
            }
            cursor++;
        }
    }
    cursor = cursor / comp_num;             /* �����㷨�ᵼ�²�ͬѹ����ͬ��ģ�������ظ����������Ҫ����comp_num */
    return cursor;
}
/***********************************************************************
 �� ��: ����DO��������
 �� ��:
************************************************************************/
DI_DO_INFO SHC_get_pcb_effect_do_logic(U08 pcb, U08 cm_max)
{
    DI_DO_INFO do_logic;
    U08 cm;

    /* ���� */
    VALCLR(do_logic);

    /* ϵͳ��� */
    if (pcb == 0)
    {
        do_logic.sys[0] = assist_info[3];
        do_logic.sys[1] = assist_info[4];
    }

    /* ģ����� */
    do_logic.cell = assist_info[2];

    /* ѹ����� */
    for (cm=0; cm<cm_max; cm++)
    {
        do_logic.comp[cm] = assist_info[1];
    }

    /* ���⴦�� */


    return do_logic;
}
/***********************************************************************
 �� ��: ������Чλ����
 �� ��:
************************************************************************/
U16 SHC_count_words_bits(U16 src[], U16 words)
{
    U16 b, n;

    for (b=0,n=0; b<words*16; b++)
    {
        if (GETBIT(src[b/16], b%16))
        {
            n++;
        }
    }

    return n;
}
/***********************************************************************
 �� ��: ����DI��������
 �� ��:
************************************************************************/
DI_DO_INFO SHC_get_pcb_effect_di_logic(U08 pcb, U08 cm_max)
{
    DI_DO_INFO di_logic;
    U08 cm;

    /* ���� */
    VALCLR(di_logic);

    /* ϵͳ��� */
    if (pcb == 0)
    {
        di_logic.sys[0] = assist_info[7];
        di_logic.sys[1] = assist_info[8];
    }

    /* ģ����� */
    di_logic.cell = assist_info[6];

    /* ѹ����� */
    for (cm=0; cm<cm_max; cm++)
    {
        di_logic.comp[cm] = assist_info[5];
    }

    /* ���⴦�� */

    return di_logic;
}
/***********************************************************************
 �� ��: ���㵥���Ĳ�ѯ��Ϣ����
 �� ��:
************************************************************************/
void SHC_get_info_num(SH_CHK_INFO_NUM *info_num, U16 pcb)
{
    DI_DO_INFO do_effect;
    DI_DO_INFO di_effect;

    info_num->cell_pv_num = SHC_common_count_mod_pv(PNULL, &cell[pcb].cell_pv[CELL_PV_BEGIN]);

    info_num->comp_pv_num = SHC_common_count_comp_pv(PNULL, pcb, SHC_USE_COMP)+SHC_COMP_EX_PER;   /* ��һ������ʱ�� */

    do_effect = SHC_get_pcb_effect_do_logic(pcb, m_val_comp_num(pcb));
    info_num->do_num = SHC_count_words_bits((U16*)&do_effect, sizeof(do_effect)/sizeof(U16));

    di_effect = SHC_get_pcb_effect_di_logic(pcb, m_val_comp_num(pcb));
    info_num->di_num = SHC_count_words_bits((U16*)&di_effect, sizeof(di_effect)/sizeof(U16));
}

/***********************************************************************
 �� ��: ģ����Ϣ�������ַ���
 �� ��:
************************************************************************/
#define SHC_NAME_PV_CELL(idx)       (msg_uint_p_cell[idx].msg)      /* ��Ԫģ�������� */
#define SHC_ATTR_PV_CELL(idx)       (msg_uint_p_cell[idx].attr)     /* ��Ԫģ�������� */

/***********************************************************************
 �� ��: ģ����Ϣ�ĸ�ֵ
 �� ��:
************************************************************************/
void SHC_get_mod_info(SH_CHK_INFO *info, U16 pcb_id, U16 idx)
{
    U08 cell_p_tbl[MAX_PV_WORD];
    U16 id_adj = 0;
    U08 cell_pv_id_adj = 0;
    I16 value = 0;

    /* ����ģ��ģ������Ϣ */
    SHC_common_count_mod_pv(cell_p_tbl, &cell[pcb_id].cell_pv[CELL_PV_BEGIN]);
    /* ģ���������� */
    id_adj = cell_p_tbl[idx];
    /* ��ԪԪ����Ϣ������ */
    cell_pv_id_adj = (id_adj - CELL_PV_BEGIN) % (CELL_PV_NUM);
    info->name = SHC_NAME_PV_CELL(cell_pv_id_adj);
    info->id = MAKEWORD(pcb_id, NO_ID);
    info->attrib = SH_sign_local2sh(SHC_ATTR_PV_CELL(cell_pv_id_adj));

    value = cell[pcb_id].cell_pv[id_adj];
    if (TEMP_IS_FARA && SHC_ATTR_PV_CELL(cell_pv_id_adj) != 0xFFFF)
    {
    	value = C_To_F(value, SHC_ATTR_PV_CELL(cell_pv_id_adj));
    }
    info->value = value;
}

/***********************************************************************
 �� ��: ϵͳ��Ϣ�������ַ���
 �� ��:
************************************************************************/
#define SHC_NAME_PV_SYS(idx)        (msg_sys_p[idx].msg)    /* ϵͳģ�������� */
#define SHC_ATTR_PV_SYS(idx)        (msg_sys_p[idx].attr)   /* ϵͳģ�������� */

/***********************************************************************
 �� ��: ��ȡϵͳ��Ϣ
 �� ��:
************************************************************************/
void SHC_get_sys_info(SH_CHK_INFO *info, U16 idx)
{
    U08 sys_p_tbl[MAX_PV_WORD];
    U08 id_adj = 0;         /* ��ӳ������� */
    I16 value = 0;

    dm603_common_count_pv(sys_p_tbl, &cell[MAX_PCB].cell_pv[0], MAX_PV);
    id_adj = sys_p_tbl[idx];
    info->name = SHC_NAME_PV_SYS(id_adj);
    info->id = MAKEWORD(NO_ID, NO_ID);

    value = cell[MAX_PCB].cell_pv[id_adj];
    if (TEMP_IS_FARA && SHC_ATTR_PV_SYS(id_adj) != 0xFFFF)
    {
    	value = C_To_F(value, SHC_ATTR_PV_SYS(id_adj));
    }
    info->value = value;

    info->attrib = (SHC_ATTR_PV_SYS(id_adj) == 0xFFFF) ? UNIT_OFFSET/* ��ʾ�ַ��� */
                                                       : SH_sign_local2sh(SHC_ATTR_PV_SYS(id_adj));/* 1λС��,�� */
}

/***********************************************************************
 �� ��: ѹ��ģ��������
 �� ��:
************************************************************************/
#define SHC_NAME_PV_COMP(idx)       (msg_uint_p_comp[idx].msg)      /* ѹ��ģ�������� */
#define SHC_ATTR_PV_COMP(idx)       (msg_uint_p_comp[idx].attr)     /* ѹ��ģ�������� */

/***********************************************************************
 �� ��: ��ȡѹ����Ϣ
 �� ��:
************************************************************************/
void SHC_get_comp_info(SH_CHK_INFO *info, U16 pcb_id, U16 idx)
{
    U08 comp_pv_num;
    U16 cmi;
    U16 exi;                /* ��չ��Ϣ��������� */
    U08 id_adj = 0;         /* ��ӳ������� */
    I16 value = 0;

    comp_pv_num = SHC_common_count_comp_pv(PNULL, pcb_id, SHC_USE_COMP)+SHC_COMP_EX_PER;

    cmi = idx / comp_pv_num;
    exi = idx % comp_pv_num;

    if (cmi < m_val_comp_num(pcb_id))
    {
        if (exi == 0)       /* ��չ��Ϣ ѹ������ʱ�� */
        {
            info->name = msg_comp_runtime;
            info->value = cell[pcb_id].cell_runtime[cmi];
            info->id = MAKEWORD(pcb_id, cmi+1);
            info->attrib = (SH_UNIT_HOUR);
        }
        else                /* ģ���� */
        {
            U08 pv_num;
            U08 avi = 0;
            U08 comp_p_tbl_buf[MAX_PV_WORD];
            pv_num = SHC_common_count_comp_pv(comp_p_tbl_buf, pcb_id, SHC_USE_COMP);
            avi = exi - SHC_COMP_EX_PER;     /* ��ȥ��չ��Ϣ���� */
            id_adj = comp_p_tbl_buf[avi];
            if (avi < pv_num)
            {
                /* �ϳ���Ϣ��Ԫ */
                info->name = SHC_NAME_PV_COMP(id_adj);
                info->id = MAKEWORD(pcb_id, cmi+1);
                info->attrib = SH_sign_local2sh(SHC_ATTR_PV_COMP(id_adj));

                value = cell[pcb_id].cell_pv[id_adj+cmi*COMP_PV_NUM];
                if (TEMP_IS_FARA && SHC_ATTR_PV_COMP(id_adj) != 0xFFFF)
                {
                	value = C_To_F(value, SHC_ATTR_PV_COMP(id_adj));
                }
                info->value = value;
            }
        }
    }
    else
    {
        /* �����Ĳ��ִ���Ԫģ���� */
        U08 cell_p_tbl[MAX_PV_WORD];
        U16 id_adj = 0;
        U08 cell_pv_id_adj = 0;

        /* ����ģ��ģ������Ϣ */
        if (SHC_common_count_mod_pv(cell_p_tbl, &cell[pcb_id].cell_pv[CELL_PV_BEGIN]) != 0)
        {
            /* ģ���������� */
            id_adj = cell_p_tbl[idx - m_val_comp_num(pcb_id) * comp_pv_num];
            /* ��ԪԪ����Ϣ������ */
            cell_pv_id_adj = (id_adj - CELL_PV_BEGIN) % (CELL_PV_NUM);
            info->name = SHC_NAME_PV_CELL(cell_pv_id_adj);
            info->id = MAKEWORD(pcb_id, NO_ID);
            info->attrib = SH_sign_local2sh(SHC_ATTR_PV_CELL(cell_pv_id_adj));

            value = cell[pcb_id].cell_pv[id_adj];
            if (TEMP_IS_FARA)
            {
            	value = C_To_F(value, SHC_ATTR_PV_CELL(cell_pv_id_adj));
            }
            info->value = value;
        }
        else
        {
            info->name = 0xFFFF;
            info->id = 0xFFFF;
            info->attrib = 0;
            info->value = SENSOR_NOEXIST;
        }
    }
}

/***********************************************************************
 �� ��: �̵�������
 �� ��:
************************************************************************/
#define SHC_NAME_DO_SYS(idx)        (msg_sys_o[idx])        /* ϵͳ������� */
#define SHC_NAME_DO_CELL(idx)       (msg_uint_o_cell[idx])  /* ��Ԫ������� */
#define SHC_NAME_DO_COMP(idx)       (msg_uint_o_comp[idx])  /* ѹ��������� */
/***********************************************************************
 �� ��: �������ڲ��Ҹ��������֣����ش������������ڵ����
 �� ��:
************************************************************************/
U16 SHC_get_effect_idx_bit(U16 src[], U16 words, U16 idx)
{
    U16 b, n;

    for (b=0,n=0; b<words*16; b++)
    {
        if (GETBIT(src[b/16], b%16))
        {
            if(n == idx)
            {
                return b;
            }
            n++;
        }
    }

    return 0xFFFF;
}
/***********************************************************************
 �� ��: ��ȡDO��Ϣ
 �� ��:
************************************************************************/
void SHC_get_do_info(SH_CHK_INFO *info, U16 pcb, U16 doi)
{
    DI_DO_INFO do_value;
    DI_DO_INFO do_effect;
    U16 n = 0;
    U16 j;

    /* ƴ�Ͽ�������Ϣ */
    {
        n = 0;              /* ��λ����λ�� */
        VALCLR(do_value);
        do_value.sys[0] = cell[MAX_PCB].cell_relay[0];
        do_value.sys[1] = cell[MAX_PCB].cell_relay[1];
        do_value.cell   = cell[pcb].cell_relay[MAX_COMP];
        for (j=0; j<m_val_comp_num(pcb); j++)
        {
            do_value.comp[j] = cell[pcb].cell_relay[j];
        }
        /* ƴ����Ч������ */
        do_effect = SHC_get_pcb_effect_do_logic(pcb, m_val_comp_num(pcb));

    }

    /* �ϳ���Ϣ��Ԫ */
    if (doi < sizeof(DI_DO_INFO)*8)
    {
        /* ������Чλ */
        n = SHC_get_effect_idx_bit((U16*)&do_effect, sizeof(do_effect)/sizeof(U16), doi);

        if(n != 0xFFFF)
        {
            U16 name = 0;
            U16 id = 0;

            if (n < 32)                     /* ǰ32λΪϵͳDO */
            {
                name = SHC_NAME_DO_SYS(n);
                id = MAKEWORD(NO_ID, NO_ID);
            }
            else if (n < 32 + 16)           /* �м�16λΪģ�� */
            {
                name = SHC_NAME_DO_CELL(n-32);
                id = MAKEWORD(pcb, NO_ID);
            }
            else                            /* ʣ�µ�Ϊѹ�� */
            {
                name = SHC_NAME_DO_COMP((n-32-16)%16);
                id = MAKEWORD(pcb, NO_ID);
            }
            info->name = name;
            info->id = id;
            info->value = GETBIT(((U16*)&do_value)[n/16], n%16);
            info->attrib = 0;               /* ������������ */
        }
    }
}

/***********************************************************************
 �� ��: DI���������
 �� ��:
************************************************************************/
#define SHC_NAME_DI_SYS(idx)        (msg_sys_i[idx])        /* ϵͳ�������� */
#define SHC_NAME_DI_CELL(idx)       (msg_uint_i_cell[idx])  /* ��Ԫ�������� */
#define SHC_NAME_DI_COMP(idx)       (msg_uint_i_comp[idx])  /* ѹ���������� */
/***********************************************************************
 �� ��: ��ȡDO��Ϣ
 �� ��:
************************************************************************/
void SHC_get_di_info(SH_CHK_INFO *info, U16 pcb, U16 dii)
{
    DI_DO_INFO di_value;
    DI_DO_INFO di_effect;
    U16 n = 0;
    U16 j;

    /* ƴ�Ͽ�������Ϣ */
    {
        n = 0;              /* ��λ����λ�� */
        VALCLR(di_value);
        di_value.sys[0] = cell[MAX_PCB].cell_input[0];
        di_value.sys[1] = cell[MAX_PCB].cell_input[1];
        di_value.cell   = cell[pcb].cell_input[MAX_COMP];
        for (j=0; j<m_val_comp_num(pcb); j++)
        {
            di_value.comp[j] = cell[pcb].cell_input[j];
        }
        /* ƴ����Ч������ */
        di_effect = SHC_get_pcb_effect_di_logic(pcb, m_val_comp_num(pcb));
    }


    /* �ϳ���Ϣ��Ԫ */
    if (dii < sizeof(DI_DO_INFO)*8)
    {
        /* ������Чλ */
        n = SHC_get_effect_idx_bit((U16*)&di_effect, sizeof(di_effect)/sizeof(U16), dii);

        if(n != 0xFFFF)
        {
            U16 name = 0;
            U08 id_cmp=NO_ID;
            if (n < 32)
            {
                name = SHC_NAME_DI_SYS(n);
            }
            else if (n < 32 + 16)
            {
                name = SHC_NAME_DI_CELL(n-32);
            }
            else
            {
                name = SHC_NAME_DI_COMP((n-32-16)%16);
                id_cmp = ((n - 32 - 16) / 16) + 1;
            }
            info->name = name;
            info->id = MAKEWORD(pcb, id_cmp);
            info->value = GETBIT(((U16*)&di_value)[n/16], n%16);
            info->attrib = 0;               /* ������������ */
        }
    }
}

/***********************************************************************
 �� ��: ��ȡ�ֶ���˪��Ϣ
 �� ��:
************************************************************************/
void SHC_get_defro_info(SH_DEFRO_INFO *info, U16 pcb)
{
    U16 cmp;
    BOL is_cm_defr=FALSE;
    BOL is_cm_force_defr=FALSE;
    BOL is_cm_run=FALSE;
    U16 pcb_status = SH_DEFR_NO_RUN;

    /* 4 ѹ����Ϣ */
    for (cmp=0; cmp<SHC_USE_COMP; cmp++)
    {
        U16 cm_status = SH_DEFR_NO_RUN;

        if (M_CM_GetStatus(pcb, cmp, COMP_DEFROST))
        {
            cm_status = SH_DEFR_DEFROST;
            is_cm_defr = TRUE;
        }
        else if (M_CM_GetStatus(pcb, cmp, COMP_FORCE_DEFROST))
        {
            cm_status = SH_DEFR_FORCE_DEFORST;
            is_cm_force_defr = TRUE;
        }
        else if (M_CM_GetStatus(pcb, cmp, COMP_RUN))
        {
            cm_status = SH_DEFR_RUN;
            is_cm_run = TRUE;
        }

        info->cms |= (cm_status<<(cmp*2));
    }

    /* 4 ģ����Ϣ */
    if (is_cm_defr)
    {
        pcb_status = SH_DEFR_DEFROST;
    }
    else if (is_cm_force_defr)
    {
        pcb_status = SH_DEFR_FORCE_DEFORST;
    }
    else if (is_cm_run)
    {
        pcb_status = SH_DEFR_RUN;
    }

    /* 4 ѹ������ */
    info->cell =   (pcb_status)
                | (m_val_comp_num(pcb) << 4);

}

/***********************************************************************
 �� ��: ��ȡ����������ʾ��Ϣ�ַ���
 �� ��: ���޴˹��ܣ�ֱ�������ַ����󷵻�
************************************************************************/
void SHC_get_start_code_tips_string(U08 *dest_buf, U16 offset)
{
    if (offset)
    {
        dest_buf[0] = '\0';
    }
    dest_buf[0] = '\0';
}
#if 0
/* ����HL055A������ */
{
    const U08* const* lang_idx;
    const U08* str_ptr;
    U08 buf[STRING_START_CODE_SIZE];
    U16 total_len = 0;
    U16 unic_len = 0;

    if (offset == 0)
    {
        lang_idx = language_c;
    }
    else
    {
        lang_idx = language_e;
    }

    str_ptr = lang_idx[dm603_face_main_input_pop_01];
    unic_len = EXSTR_ChcodeStr(buf, STRING_START_CODE_SIZE, str_ptr);
    MEMCPY(&dest_buf[total_len], buf, unic_len);
    total_len += unic_len;

    dest_buf[total_len++] = 13;
    dest_buf[total_len++] = 10;

    str_ptr = lang_idx[dm603_face_main_input_pop_02];
    unic_len = EXSTR_ChcodeStr(buf, STRING_START_CODE_SIZE, str_ptr);
    MEMCPY(&dest_buf[total_len], buf, unic_len);
    total_len += unic_len;

    unic_len = Fun_NumFormat(val_mac_type_num, buf, FNS_DIGIT4);
    MEMCPY(&dest_buf[total_len], buf, unic_len);
    total_len += unic_len;

    dest_buf[total_len++] = 13;
    dest_buf[total_len++] = 10;

    str_ptr = lang_idx[dm603_face_main_input_pop_03];
    unic_len = EXSTR_ChcodeStr(buf, STRING_START_CODE_SIZE, str_ptr);
    MEMCPY(&dest_buf[total_len], buf, unic_len);
    total_len += unic_len;

    unic_len = Fun_NumFormat(system_time.year, buf, FNS_DIGIT4);
    MEMCPY(&dest_buf[total_len], buf, unic_len);
    total_len += unic_len;

    dest_buf[total_len++] = '.';

    unic_len = Fun_NumFormat(system_time.month, buf, FNS_ZERO|FNS_DIGIT2);
    MEMCPY(&dest_buf[total_len], buf, unic_len);
    total_len += unic_len;

    dest_buf[total_len++] = '.';

    unic_len = Fun_NumFormat(system_time.day, buf, FNS_ZERO|FNS_DIGIT2);
    MEMCPY(&dest_buf[total_len], buf, unic_len);
    total_len += unic_len;

    dest_buf[total_len++] = '\0';

    return TRUE;
}
#endif

/***********************************************************************
 �� ��: ��ȡʹ�ý�����Ϣ
 �� ��:
************************************************************************/
void SHC_get_enable(SH_ENABLE_INFO *info)
{
    U16 pcb = 0;
    U16 cm = 0;

    for (pcb=0; pcb<MIN(MAX_PCB, SH_MAX_PCB_NUM); pcb++)
    {
        if (CELL_Is_used(pcb))
        {
            SETBIT(info->pcb_enable, pcb);
        }

        if (err_sys_get_s_commu_err(pcb))
        {
            SETBIT(info->pcb_commu_err, pcb);
        }

        for (cm=0; cm<MIN(MAX_COMP, SH_MAX_COMP_NUM); cm++)
        {
//            if (PARA_GetCompUse(pcb, cm))
            {
                SETBIT(info->comp_enable[cm], pcb);
            }
        }
    }
}

/***********************************************************************
 �� ��: ��ȡ�û���ѯ��������ѯ��
 �� ��: ���޴˹��ܣ�value���ΪSENSOR_NOEXIST
************************************************************************/
void SHC_get_user_info(SH_CHK_INFO *info, U16 i)
{
    U16 str_chk_user[SH_MAX_CHK_USER_INFO] =
    {
        msg_sys_pv_08,    /* ���� */
        msg_sys_pv_09,    /* ���� */
        msg_sys_pv_03,    /* ���� */
        msg_comp_pv_11,   /* ��ѹ */
        msg_comp_pv_12,   /* ��ѹ */
        msg_comp_pv_12,   /* ��ѹ */
    };

    I16 info_value[SH_MAX_CHK_USER_INFO];
    U16 info_attr[SH_MAX_CHK_USER_INFO];

    I16 value = 0;

    info_value[0] = SENSOR_NOEXIST;
    info_value[1] = SENSOR_NOEXIST;
    info_value[2] = pv_sys_TA;
    info_value[3] = pv_cell_lp(0);
    info_value[4] = pv_cell_hp(0);
    info_value[5] = SENSOR_NOEXIST;

    info_attr[0] = UINT_C|D_DOT1;
    info_attr[1] = UINT_C|D_DOT1;
    info_attr[2] = UINT_C|D_DOT1;
    info_attr[3] = UINT_PR|D_DOT1;
    info_attr[4] = UINT_PR|D_DOT1;
    info_attr[5] = UINT_PR|D_DOT1;

    info->name = str_chk_user[i];
    info->id = 0xffff;

    value = info_value[i];
    if (TEMP_IS_FARA && info_attr[i] != 0xFFFF)
    {
    	value = C_To_F(value, info_attr[i]);
    }
    info->value = value;

    info->attrib = SH_sign_local2sh(info_attr[i]);
}

/***********************************************************************
 �� ��: ��ȡָ���ı�Ƶ���Ĵ�����ַ��ֵ
 �� ��:
************************************************************************/
void SHC_get_invt_para(SH_INVT_PARA *info)
{
    info->addr     = info_invt_para[INVT_ID_1].addr;
    info->val      = info_invt_para[INVT_ID_1].val;
    info->str_tips = msg_nothing;
    /* ִ��һ�ζ���Ƶ���Ĳ��� */
}

#define MSG_FLOOR_PREHEAT_TEMP            (msg_para_small_14_09)
#define MSG_FLOOR_PREHEAT_INTERVAL        (msg_para_small_14_10)
#define MSG_FLOOR_PREHEAT_MAX             (msg_para_small_14_11)

/***********************************************************************
 �� ��: ��ȡָ���ĵ�ůԤ�Ȳ���
 �� ��:
************************************************************************/
void SHC_get_floor_preheat_info(SH_PARA_ATTRIB *info, U08 idx)
{
	switch (idx)
	{
	    default:
        case 0:
            info->name =   MSG_FLOOR_PREHEAT_TEMP;
        	info->id =     0xFFFF;
        	info->value =  para_get_val(P_GaI(val_floor_preheat_temp));
        	info->min =    para_get_min(P_GaI(val_floor_preheat_temp));
        	info->max =    para_get_max(P_GaI(val_floor_preheat_temp));
        	info->attrib = SH_sign_local2sh(para_get_sign(P_GaI(val_floor_preheat_temp)));
            break;

        case 1:
            info->name =   MSG_FLOOR_PREHEAT_INTERVAL;
        	info->id =     0xFFFF;
        	info->value =  para_get_val(P_GaI(val_floor_preheat_interval));
        	info->min =    para_get_min(P_GaI(val_floor_preheat_interval));
        	info->max =    para_get_max(P_GaI(val_floor_preheat_interval));
        	info->attrib = SH_sign_local2sh(para_get_sign(P_GaI(val_floor_preheat_interval)));
            break;

        case 2:
            info->name =   MSG_FLOOR_PREHEAT_MAX;
        	info->id =     0xFFFF;
        	info->value =  para_get_val(P_GaI(val_floor_preheat_max));
        	info->min =    para_get_min(P_GaI(val_floor_preheat_max));
        	info->max =    para_get_max(P_GaI(val_floor_preheat_max));
        	info->attrib = SH_sign_local2sh(para_get_sign(P_GaI(val_floor_preheat_max)));
            break;

	}
}

#define MSG_FLOOR_DRYUP_HEAD              (msg_para_small_14_14)
#define MSG_FLOOR_DRYUP_MID               (msg_para_small_14_15)
#define MSG_FLOOR_DRYUP_TAIL              (msg_para_small_14_16)
#define MSG_FLOOR_DRYUP_TEMP              (msg_para_small_14_17)

/***********************************************************************
 �� ��: ��ȡָ���Ķ�ʱ��ů����
 �� ��:
************************************************************************/
void SHC_get_floor_timer_info(SH_PARA_ATTRIB *info, U08 idx)
{
    switch (idx)
    {
        /* ����ů���ǰ�� */
        default:
        case 0:
        	info->name =   MSG_FLOOR_DRYUP_HEAD;
        	info->id =     0xFFFF;
        	info->value =  para_get_val(P_GaI(val_floor_dry_head));
        	info->min =    para_get_min(P_GaI(val_floor_dry_head));
        	info->max =    para_get_max(P_GaI(val_floor_dry_head));
        	info->attrib = SH_sign_local2sh(para_get_sign(P_GaI(val_floor_dry_head)));
            break;

        /* ����ů����С� */
        case 1:
        	info->name =   MSG_FLOOR_DRYUP_MID;
        	info->id =     0xFFFF;
        	info->value =  para_get_val(P_GaI(val_floor_dry_mid));
        	info->min =    para_get_min(P_GaI(val_floor_dry_mid));
        	info->max =    para_get_max(P_GaI(val_floor_dry_mid));
        	info->attrib = SH_sign_local2sh(para_get_sign(P_GaI(val_floor_dry_mid)));
            break;

        /* ����ů���β�� */
        case 2:
        	info->name =   MSG_FLOOR_DRYUP_TAIL;
        	info->id =     0xFFFF;
        	info->value =  para_get_val(P_GaI(val_floor_dry_tail));
        	info->min =    para_get_min(P_GaI(val_floor_dry_tail));
        	info->max =    para_get_max(P_GaI(val_floor_dry_tail));
        	info->attrib = SH_sign_local2sh(para_get_sign(P_GaI(val_floor_dry_tail)));
            break;

        /* ����ů����¶ȡ� */
        case 3:
        	info->name =   MSG_FLOOR_DRYUP_TEMP;
        	info->id =     0xFFFF;
        	info->value =  para_get_val(P_GaI(val_floor_dry_temp));
        	info->min =    para_get_min(P_GaI(val_floor_dry_temp));
        	info->max =    para_get_max(P_GaI(val_floor_dry_temp));
        	info->attrib = SH_sign_local2sh(para_get_sign(P_GaI(val_floor_dry_temp)));
            break;
    }
}

/***********************************************************************
 �� ��: ��ȡ��ʾ���̼���Ϣ
 �� ��:
************************************************************************/
void SHC_get_test_fac_hmi_info(SH_TEST_FAC_HMI_STRU *info)
{
#ifdef M_TCL
	static U08 wifi_ssid[] = "moduletest";
	static U08 wifi_pass[] = "12345678";
#else
    static U08 wifi_ssid[] = "TP-LINK-2.4G";
	static U08 wifi_pass[] = "cloud000002";
//    static U08 wifi_ssid[] = "wifi_kjh";
//	static U08 wifi_pass[] = "1234567890";
#endif
	U08 i;

	info->rd.fg_TF_is_on = TF_HAS_INNER&&TF_EN_WIFI;

	ARRCLR(info->rd.wifi_ssid);
	for (i=0; i<32; i++)
	{
		if (wifi_ssid[i] == 0)
		{
			break;
		}
		info->rd.wifi_ssid[i] = wifi_ssid[i];
	}

	ARRCLR(info->rd.wifi_password);
	for (i=0; i<32; i++)
	{
		if (wifi_pass[i] == 0)
		{
			break;
		}
		info->rd.wifi_password[i] = wifi_pass[i];
	}
}

/***********************************************************************
 �� ��: ����������
 �� ��:
************************************************************************/
void SHC_command_ON_deal(U16 command)
{
    if (command)
    {
        DM603_CMD_RUN;
    }
}

/***********************************************************************
 �� ��: ����ػ�����
 �� ��:
************************************************************************/
void SHC_command_OFF_deal(U16 command)
{
    if (command)
    {
        DM603_CMD_STOP;
    }
}

/***********************************************************************
 �� ��: ����λ����
 �� ��:
************************************************************************/
void SHC_command_RST_deal(U16 command)
{
    if (command)
    {
        DM603_CMD_RESET;
    }
}

/***********************************************************************
 �� ��: ����������ϸ�λ����
 �� ��:
************************************************************************/
void SHC_command_PASS_RST_deal(U16 command)
{
    if (command)
    {
        ;
    }
}

/***********************************************************************
 �� ��: ����ȡ��Ԥ������
 �� ��:
************************************************************************/
void SHC_command_OFF_OILHEAT_deal(U16 command)
{
    if (command)
    {
        ;
    }
}

/***********************************************************************
 �� ��: �����ֶ���˪����
 �� ��:
************************************************************************/
void SHC_command_DEFRO_deal(U16 command)
{
    DM603_CMD_DEFROST(command);
}

/***********************************************************************
 �� ��: ���������ʼ������
 �� ��:
************************************************************************/
void SHC_command_PARA_INIT_deal(U16 command)
{
    if (command)
    {
        DM603_CMD_PARA_INIT;
    }
}

/***********************************************************************
 �� ��: ����ѹ������ʱ���ʼ������
 �� ��:
************************************************************************/
void SHC_command_CM_RUNTM_INIT_deal(U16 command)
{
    if (command)
    {
        DM603_CMD_RUNTIME_INIT;
    }
}

/***********************************************************************
 �� ��: ����ϵͳ����ʱ���ʼ������
 �� ��:
************************************************************************/
void SHC_command_SYS_RUNTM_INIT_deal(U16 command)
{
    if (command)
    {
        cell[MAX_PCB].cell_runtime[SYS_RUNTIME] = 0;
        cmd_systime_save();
    }
}

/***********************************************************************
 �� ��: ������ʷ���ϳ�ʼ������
 �� ��:
************************************************************************/
void SHC_command_HIS_ERR_INIT_deal(U16 command)
{
    if (command)
    {
        DataAccess_HistoryErr(DATA_INIT);   /* ��ʼ����ʷ������Ϣ */
    }
}

/***********************************************************************
 �� ��: �����Ƶ����ʼ������
 �� ��:
************************************************************************/
void SHC_command_INVERTER_INIT_deal(U16 command)
{
    if (command)
    {
        DM603_CMD_INVT_INIT;
    }
}

/***********************************************************************
 �� ��: �����ֶ�ˮ������
 �� ��:
************************************************************************/
void SHC_command_PUMP_deal(U16 command)
{
    if (command)
    {
        if (debug_mode == FALSE
        && (status_run == _IDLE || status_run == _ALARM)
        && (!fg_idle_sta_antifreeze))
        {
            debug_mode = TRUE;
        }
        else
        {
            debug_mode = FALSE;
        }
    }
}

/***********************************************************************
 �� ��: ����BMS��������
 �� ��:
************************************************************************/
void SHC_command_BMS_PARA_deal(U16 command)
{
    //U08 item = HIBYTE(command);
    I16 value = LOBYTE(command);
    if(value >= SHC_BMS_PARA_MIN(item) && value <= SHC_BMS_PARA_MAX(item))
    {
/*        SH_BMS_PARA_VAL(item) = value; */
        /* ͨѶ���ݸ�ʽд��EEPROM */
/*      ModbusRS485EEprom(&SH_BMS_PARA_VAL(0), 1);  */
/*        Task_ModbusRS485(); */
    }
}

/***********************************************************************
 �� ��: �������Ԥ������
 �� ��:
************************************************************************/
void SHC_command_QUICK_OILHEAT_deal(U16 command)
{
    if (command)
    {
        fg_warm_quick = TRUE;
    }
}

/***********************************************************************
 �� ��: ����Ϳѻ�������ӱ�־
 �� ��:
************************************************************************/
void SHC_command_TUYA_NEW_LINK_deal(U16 command)
{
    if (command)
    {
        TYB_SET_FG_DPID_UPDATE_ALL;
    }
}

/***********************************************************************
 �� ��: ��������ů��ɹ���
 �� ��:
************************************************************************/
void SHC_command_enter_floor_dryup_deal(U16 command)
{
    if (command)
    {
        SETCMD_N(FLOOR_DRYUP_ON);
    }
    else
    {
    	SETCMD_N(FLOOR_DRYUP_OFF);
    }
}

/***********************************************************************
 �� ��: ����ǿ����������
 �� ��:
************************************************************************/
void SHC_command_force_ibh_deal(U16 command)
{
    if (command)
    {
        SETCMD_N(FORCE_IBH_ON);
    }
    else
    {
    	SETCMD_N(FORCE_IBH_OFF);
    }
}

/***********************************************************************
 �� ��: ����ǿ��ˮ�����
 �� ��:
************************************************************************/
void SHC_command_force_tbh_deal(U16 command)
{
    if (command)
    {
        SETCMD_N(FORCE_TBH_ON);
    }
    else
    {
    	SETCMD_N(FORCE_TBH_OFF);
    }
}

/***********************************************************************
 �� ��: ����ǿ����ˮģʽ
 �� ��:
************************************************************************/
void SHC_command_force_hw_mode_deal(U16 command)
{
    if (command)
    {
        SETCMD_N(FORCE_HOTW_ON);
    }
    else
    {
    	SETCMD_N(FORCE_HOTW_OFF);
    }
}

/***********************************************************************
 �� ��: ������������
 �� ��:
************************************************************************/
void SHC_command_ability_test_deal(U16 command)
{
    if (command)
    {
        SETCMD_N(ABILITY_TEST_ON);
    }
}

/***********************************************************************
 �� ��: A������
 �� ��:
************************************************************************/
void SHC_command_ON_zone_1_deal(U16 command)
{
    /* ���ɿ��� */
    if (SHC_ac_on_off_disabled())
    {
        return;
    }

    if (command)
    {
        SETCMD_N(ZONE_A_ON);
    }
}

/***********************************************************************
 �� ��: A���ػ�
 �� ��:
************************************************************************/
void SHC_command_OFF_zone_1_deal(U16 command)
{
    /* ���ɹػ� */
    if (SHC_ac_on_off_disabled())
    {
        return;
    }

    if (command)
    {
        SETCMD_N(ZONE_A_OFF);

        /* A���ػ�ʱ���˳��������� */
        SETCMD_N(ABILITY_TEST_OFF);

        /* �˳�ˮ�������� */
        if (GETBIT_N(fg_test_run, TEST_RUN_PUMP)
        && !USE_DUAL_ZONE_CTRL
        && !val_hotw_used
        )
        {
            SETCMD_N(TEST_RUN_PUMP_OFF);
        }
    }
}

/***********************************************************************
 �� ��: B������
 �� ��:
************************************************************************/
void SHC_command_ON_zone_2_deal(U16 command)
{
    /* ���ɿ��� */
    if (SHC_ac_on_off_disabled())
    {
        return;
    }

    if (command)
    {
        /* �յ�����ģʽΪ����ʱ��B�����ɿ��� */
        if (ac_mode != MODE_COOL)
        {
            SETCMD_N(ZONE_B_ON);
        }

        /* B������ʱ���˳��������� */
        SETCMD_N(ABILITY_TEST_OFF);
    }
}

/***********************************************************************
 �� ��: B���ػ�
 �� ��:
************************************************************************/
void SHC_command_OFF_zone_2_deal(U16 command)
{
    /* ���ɹػ� */
    if (SHC_ac_on_off_disabled())
    {
        return;
    }

    if (command)
    {
        SETCMD_N(ZONE_B_OFF);
    }
}

/***********************************************************************
 �� ��: ��ˮ����
 �� ��:
************************************************************************/
void SHC_command_ON_hotwater_deal(U16 command)
{
    /* ���ɿ��� */
    if (SHC_hotw_on_off_disabled())
    {
        return;
    }

    if (command)
    {
        SETCMD_N(HOTW_ON);

        /* ��ˮ����ʱ���˳��������� */
        SETCMD_N(ABILITY_TEST_OFF);
    }
}

/***********************************************************************
 �� ��: ��ˮ�ػ�
 �� ��:
************************************************************************/
void SHC_command_OFF_hotwater_deal(U16 command)
{
    /* ���ɹػ� */
    if (SHC_hotw_on_off_disabled())
    {
        return;
    }

    if (command)
    {
        SETCMD_N(HOTW_OFF);

        /* ��ˮ�ػ�ʱ���˳���ʱɱ�� */
        SETCMD_N(TIMED_DISINFECT_OFF);
    }
}

/***********************************************************************
 �� ��: ϵͳά����ʼ��
 �� ��:
************************************************************************/
void SHC_command_limit_init_deal(U16 command)
{
    if (command)
    {
        DM603_CMD_LIMIT_INIT;
    }
}

/***********************************************************************
 �� ��: �շ�����
 �� ��:
************************************************************************/
void SHC_command_recyle_refri_deal(U16 command)
{
    if (command)
    {
        SETCMD_N(REFRI_RECY_ON);
    }
}

/***********************************************************************
 �� ��: ��ʱ����ˮ��
 �� ��:
************************************************************************/
void SHC_command_TIMER_DHW_ON_deal(U16 command)
{
    if (command)
    {
        SETCMD_N(TIMED_PUMPRET_ON);
    }
}

/***********************************************************************
 �� ��: ǿ���ⲿ��Դ
 �� ��:
************************************************************************/
void SHC_command_force_AHS_deal(U16 command)
{
    if (command)
    {
        SETCMD_N(FORCE_AHS_ON);
    }
    else
    {
    	SETCMD_N(FORCE_AHS_OFF);
    }
}

/***********************************************************************
 �� ��: ɱ��
 �� ��:
************************************************************************/
void SHC_command_disinfect_deal(U16 command)
{
    if (command)
    {
        SETCMD_N(TIMED_DISINFECT_ON);
    }
    else
    {
        SETCMD_N(TIMED_DISINFECT_OFF);           /* �ֶ��˳���ʱɱ������ */
    }
}

/***********************************************************************
 �� ��: ����
 �� ��:
************************************************************************/
void SHC_command_silent_deal(U16 command)
{
    if (command)
    {
        SETCMD_N(SILENT_MODE_ON);
        fg_silent_mode = TRUE;
    }
    else
    {
        SETCMD_N(SILENT_MODE_OFF);
        fg_silent_mode = FALSE;
    }
}

/***********************************************************************
 �� ��: ˮ��������
 �� ��:
************************************************************************/
void SHC_command_pump_test_deal(U16 command)
{
    if (command)
    {
        SETCMD_N(TEST_RUN_PUMP_ON);
    }
    else
    {
        SETCMD_N(TEST_RUN_PUMP_OFF);
    }
}

/***********************************************************************
 �� ��: ����������
 �� ��:
************************************************************************/
void SHC_command_cool_test_deal(U16 command)
{
    if (command)
    {
        SETCMD_N(TEST_RUN_COOL_MODE_ON);
    }
    else
    {
        SETCMD_N(TEST_RUN_COOL_MODE_OFF);
    }
}

/***********************************************************************
 �� ��: ����������
 �� ��:
************************************************************************/
void SHC_command_heat_test_deal(U16 command)
{
    if (command)
    {
        SETCMD_N(TEST_RUN_HEAT_MODE_ON);
    }
    else
    {
        SETCMD_N(TEST_RUN_HEAT_MODE_OFF);
    }
}

/***********************************************************************
 �� ��: ����ˮ������
 �� ��:
************************************************************************/
void SHC_command_hotwater_test_deal(U16 command)
{
    if (command)
    {
        SETCMD_N(TEST_RUN_HOTW_MODE_ON);
    }
    else
    {
        SETCMD_N(TEST_RUN_HOTW_MODE_OFF);
    }
}

/***********************************************************************
 �� ��: ϵͳ�ſյ���
 �� ��:
************************************************************************/
void SHC_command_system_empty_deal(U16 command)
{
    if (command)
    {
        SETCMD_N(TEST_RUN_AIR_EMPTY_ON);
    }
    else
    {
        SETCMD_N(TEST_RUN_AIR_EMPTY_OFF);
    }
}

/***********************************************************************
 �� ��: ��ůԤ��
 �� ��:
************************************************************************/
void SHC_command_floor_preheat_deal(U16 command)
{
    if (command)
    {
    	SETCMD_N(FLOOR_PREHEAT_ON);
    }
    else
    {
    	SETCMD_N(FLOOR_PREHEAT_OFF);
    }
}

/***********************************************************************
 �� ��: ��ʱ��ů
 �� ��:
************************************************************************/
void SHC_command_floor_timer_deal(U16 command)
{
    if (command)
    {
    	SETCMD_N(FLOOR_DRYUP_ON);
    }
    else
    {
    	SETCMD_N(FLOOR_DRYUP_OFF);
    }
}

/***********************************************************************
 �� ��: �����뿪�����
 �� ��:
************************************************************************/
void SHC_command_holiday_away_deal(U16 command)
{
    if (SHC_holiday_away_on_off_disabled())
    {
        return;
    }

    if (command)
    {
    	SETCMD_N(HOLIDAY_AWAY_ON);
    }
    else
    {
    	SETCMD_N(HOLIDAY_AWAY_OFF);
    }
}

/***********************************************************************
 �� ��: �����ڼ������(����ʹ���Ҵ��ڼ����ڼ�ʱ����ڣ�ÿ1�����·�һ������)
 �� ��:
************************************************************************/
void SHC_command_holiday_home_deal(U16 command)
{
    if (SHC_ac_on_off_disabled())
    {
        return;
    }

    if (command)
    {
    	SETCMD_N(HOLIDAY_HOME_ON);
    }
    else
    {
    	SETCMD_N(HOLIDAY_HOME_OFF);
    }
}

/***********************************************************************
 �� ��: ����Զ���������������
 �� ��:
************************************************************************/
void SHC_command_accept_remote_upgrade_req_deal(U16 command)
{
	fg_accept_remote_upgrade = command;
}

/***********************************************************************
 �� ��: �����¶�����
 �� ��: ���ڴ��ڶ�������ƣ��˺���ʵ��δʹ��
************************************************************************/
BOOL SHC_set_tmp_deal(I16 set_data)
{
    BOOL result = TRUE;
    I16 para_min = 0;       /* ��Сֵ */
    I16 para_max = 0;       /* ���ֵ */
    U08 grp_cursor = 0;
    U08 item_cursor = 0;         /* ����λ�� */

    switch (sv_mode)
    {
        case SET_COOL:
            grp_cursor = P_grp(sv_cool);
            item_cursor = P_idx(sv_cool);
            break;
        case SET_HEAT:
            grp_cursor = P_grp(sv_heat);
            item_cursor = P_idx(sv_heat);
            break;
        case SET_AUTO:
            grp_cursor = (ac_mode == MODE_COOL) ? P_grp(sv_cool) : P_grp(sv_heat);
            item_cursor = (ac_mode == MODE_COOL) ? P_idx(sv_cool) : P_idx(sv_heat);
            break;
        default:
            result = FALSE;
            break;
    }
    if (result)
    {
        para_min = para_get_min(grp_cursor, item_cursor);
        para_max = para_get_max(grp_cursor, item_cursor);
        if (set_data >= para_min && set_data <= para_max)
        {
            para_set(grp_cursor, item_cursor, set_data);
        }
        else
        {
            result = FALSE;
        }
    }

    return result;
}

/***********************************************************************
 �� ��: ����ģʽ����
 �� ��:
************************************************************************/
BOOL SHC_set_mode_deal(I16 set_data)
{
    BOOL result = TRUE;
    I16 para_min = 0;       /* ��Сֵ */
    I16 para_max = 0;       /* ���ֵ */
    I16 para_new = 0;       /* ��ֵ */

    /* �����л�ģʽ */
    if (SHC_ac_mode_switch_disabled())
    {
        return result;
    }

    para_min = para_get_min(P_GaI(sv_mode));
    para_max = para_get_max(P_GaI(sv_mode));
    switch (set_data)
    {
        case SH_MODE_AUTO:
            para_new = SET_AUTO;
            break;
        case SH_MODE_COOL:
            para_new = SET_COOL;
            break;
        case SH_MODE_HEAT:
            para_new = SET_HEAT;
            break;
//        case SH_MODE_HOTWATER:
//            para_new = SET_HOT_WATER;
//            break;
//        case SH_MODE_COOL_HW:
//            para_new = SET_COOL_HOT_WATER;
//            break;
//        case SH_MODE_HEAT_HW:
//            para_new = SET_HEAT_HOT_WATER;
//            break;
        default:
            para_new = 0;
            break;
    }
    if (para_new >= para_min && para_new <= para_max)
    {
        para_set(P_GaI(sv_mode), para_new);
    }
    else
    {
        result = FALSE;
    }

    return result;
}

/***********************************************************************
 �� ��: ����ʹ����������
 �� ��:
************************************************************************/
BOOL SHC_set_limit_time_deal(I16 set_data)
{
    limit_value = set_data;

    return TRUE;
}

/***********************************************************************
 �� ��: ����ʱʹ������
 �� ��:
************************************************************************/
BOOL SHC_set_timing_use_deal(I16 set_data)
{
    if (set_data)
    {
        SET_TIME_EN();
    }
    else
    {
        CLR_TIME_EN();
    }

    DataAccess_Timer(DATA_WRITE);

    return TRUE;
}

///***********************************************************************
// �� ��: �������������������
// �� ��: Ŀǰ������ר�ó���ʹ��
//************************************************************************/
BOOL SHC_set_start_code_deal(I16 set_data)
{
    if (set_data)
    {
        ;
    }

    return TRUE;
}

/***********************************************************************
 �� ��: ����ʪ��ģʽ����
 �� ��:
************************************************************************/
BOOL SHC_set_humi_mode_deal(I16 set_data)
{
    if (set_data)
    {
        ;
    }

    return TRUE;
}

/***********************************************************************
 �� ��: ������������
 �� ��:
************************************************************************/
BOOL SHC_set_language_deal(I16 set_data)
{
    if (set_data >= 0
    && set_data <= SH_LANGUAGE_MAX
    )
    {
        sh_language = set_data;
        DataAccess_Language(DATA_WRITE);
    }
    return TRUE;
}

/***********************************************************************
 �� ��: ��������һ�¶�����
 �� ��:
************************************************************************/
BOOL SHC_set_tmp_zone_1_deal(I16 set_data)
{
    BOOL result = TRUE;
    I16 para_min = 0;       /* ��Сֵ */
    I16 para_max = 0;       /* ���ֵ */
    U08 grp_cursor = 0;     /* ������� */
    U08 item_cursor = 0;    /* ������� */

    if (!temp_set_can_edit_zone_A)
    {
        return TRUE;
    }

    if (temp_ctrl_type_zone_A == WATER_TEMP_CTRL)
    {
        switch (ac_mode)
        {
            case MODE_COOL:
                grp_cursor = P_grp(sv_cool);
                item_cursor = P_idx(sv_cool);
                break;
            case MODE_HEAT:
                grp_cursor = P_grp(sv_heat);
                item_cursor = P_idx(sv_heat);
                break;
            default:
                result = FALSE;
                break;
        }
    }
    else
    {
        switch (ac_mode)
        {
            case MODE_COOL:
                grp_cursor = P_grp(sv_room_cool);
                item_cursor = P_idx(sv_room_cool);
                break;
            case MODE_HEAT:
                grp_cursor = P_grp(sv_room_heat);
                item_cursor = P_idx(sv_room_heat);
                break;
            default:
                result = FALSE;
                break;
        }
    }

    if (result)
    {
        para_min = para_get_min(grp_cursor, item_cursor);
        para_max = para_get_max(grp_cursor, item_cursor);
        if (set_data >= para_min && set_data <= para_max)
        {
            para_set(grp_cursor, item_cursor, set_data);
        }
        else
        {
            result = FALSE;
        }
    }

    return result;
}

/***********************************************************************
 �� ��: ����������¶�����
 �� ��:
************************************************************************/
BOOL SHC_set_tmp_zone_2_deal(I16 set_data)
{
    BOOL result = TRUE;
    I16 para_min = 0;       /* ��Сֵ */
    I16 para_max = 0;       /* ���ֵ */
    U08 grp_cursor = 0;     /* ������� */
    U08 item_cursor = 0;    /* ������� */

    if (!temp_set_can_edit_zone_B)
    {
        return TRUE;
    }

    if (temp_ctrl_type_zone_B == WATER_TEMP_CTRL)
    {
        switch (ac_mode)
        {
            case MODE_COOL:
                grp_cursor = P_grp(sv_cool_zone_B);
                item_cursor = P_idx(sv_cool_zone_B);
                break;
            case MODE_HEAT:
                grp_cursor = P_grp(sv_heat_zone_B);
                item_cursor = P_idx(sv_heat_zone_B);
                break;
            default:
                result = FALSE;
                break;
        }
    }
    else
    {
        switch (ac_mode)
        {
            case MODE_COOL:
                grp_cursor = P_grp(sv_room_cool_zone_B);
                item_cursor = P_idx(sv_room_cool_zone_B);
                break;
            case MODE_HEAT:
                grp_cursor = P_grp(sv_room_heat_zone_B);
                item_cursor = P_idx(sv_room_heat_zone_B);
                break;
            default:
                result = FALSE;
                break;
        }
    }

    if (result)
    {
        para_min = para_get_min(grp_cursor, item_cursor);
        para_max = para_get_max(grp_cursor, item_cursor);
        if (set_data >= para_min && set_data <= para_max)
        {
            para_set(grp_cursor, item_cursor, set_data);
        }
        else
        {
            result = FALSE;
        }
    }

    return result;
}

/***********************************************************************
 �� ��: ������ˮ�¶�����
 �� ��:
************************************************************************/
BOOL SHC_set_tmp_zone_hotw_deal(I16 set_data)
{
    BOOL result = TRUE;
    I16 para_min = 0;       /* ��Сֵ */
    I16 para_max = 0;       /* ���ֵ */

    /* ��ֹ�޸ġ���ˮ�趨�¶ȡ������ */
    if (FREE_ELEC_FORCE_RUN     /* ����õ�ǿ������ */
    || GETBIT_N(fg_test_run, TEST_RUN_HOTW_MODE)
    )
    {
        return result;
    }

    if (result)
    {
        para_min = para_get_min(P_GaI(sv_hotw));
        para_max = para_get_max(P_GaI(sv_hotw));
        if (set_data >= para_min && set_data <= para_max)
        {
            para_set(P_GaI(sv_hotw), set_data);
        }
        else
        {
            result = FALSE;
        }
    }

    return result;
}

/***********************************************************************
 �� ��: ����ά����������
 �� ��:
************************************************************************/
BOOL SHC_set_lmt_type_deal(I16 set_data)
{
    BOOL result = TRUE;

    limit_type = set_data;

    return result;
}

/***********************************************************************
 �� ��: ��������������
 �� ��:
************************************************************************/
BOOL SHC_set_silent_level_deal(I16 set_data)
{
    BOOL result = TRUE;

    silent_mode_level = (set_data < SILENT_MAX) ? set_data : SILENT_LEVEL_1;

    return result;
}

/***********************************************************************
 �� ��: �����¶ȵ�λ����
 �� ��:
************************************************************************/
BOOL SHC_set_temp_unit_deal(I16 set_data)
{
    BOOL result = TRUE;

	if (set_data <= SH_SET_UNIT_F)
	{
		sh_temp_unit = set_data;
        DataAccess_Temp_unit(DATA_WRITE);
	}

    return result;
}

/***********************************************************************
 �� ��: ����ʱ���·�
 �� ��:
************************************************************************/
void SHC_set_clk_deal(SH_CLOCK *time_buff)
{
    U08 i;

    for (i=0; i<MAX_HMI; i++)
    {
        clock[i].year  = time_buff->year;
        clock[i].month = time_buff->month;
        clock[i].day   = time_buff->day;
        clock[i].week  = time_buff->week;
        clock[i].hour  = time_buff->hour;
        clock[i].min   = time_buff->min;
        clock[i].sec   = time_buff->sec;
    }
    system_time.year  = time_buff->year;
    system_time.month = time_buff->month;
    system_time.day   = time_buff->day;
    system_time.week  = time_buff->week;
    system_time.hour  = time_buff->hour;
    system_time.min   = time_buff->min;
    system_time.sec   = time_buff->sec;
}

/***********************************************************************
 �� ��: �������ģʽ���ݱ���
 �� ��:
************************************************************************/
void SHC_set_holidayinfo_deal(SH_HOLIDAYINFO info_buff)
{
    fg_holiday_away_heat_mode_used = info_buff.mode_heat_use;
    fg_holiday_away_hotw_mode_used = info_buff.mode_hotw_use;

    /* ����ģʽʹ�ã�������ģʽ���趨Ϊ"����ģʽ" */
    if (fg_holiday_away_heat_mode_used)
    {
        para_set(P_GaI(sv_mode), SET_HEAT);
    }
}

/***********************************************************************
 �� ��: ���������¶����ݱ���
 �� ��:
************************************************************************/
void SHC_set_room_temp_deal(I16 info)
{
    room_temp = info;
}

/***********************************************************************
 �� ��: ������������·�
 �� ��:
************************************************************************/
void SHC_set_para_deal(U16 grp_idx, U16 item_idx, I16 value)
{
    if (grp_idx < SHC_DISP_GET_PARA_GRP_NUM && item_idx < SHC_DISP_GET_PARA_GRP_SIZE)
    {
        I16 min = para_get_min(grp_idx, item_idx);
        I16 max = para_get_max(grp_idx, item_idx);

        if (value >= min && value <= max || para_is_bit(grp_idx, item_idx))
        {
            para_set(grp_idx, item_idx, value);
        }
    }
}

/***********************************************************************
 �� ��: �����Ƶ�����·�
 �� ��:
************************************************************************/
void SHC_set_invt_para_deal(SH_INVT_PARA info_buff)
{
    info_invt_para[INVT_ID_1].addr = info_buff.addr;
    info_invt_para[INVT_ID_1].val  = info_buff.val;

    /* ִ��һ���·���Ƶ�������Ĳ��� */
    if (info_buff.val == -1)
    {
        info_invt_para[INVT_ID_1].fg_rd = TRUE;
    }
    else
    {
        info_invt_para[INVT_ID_1].fg_wr = TRUE;
    }
}

/***********************************************************************
 �� ��: �����ůԤ�Ȳ����·�
 �� ��:
************************************************************************/
void SHC_set_floor_preheat_val_deal(U08 idx, I16 val)
{
	switch (idx)
	{
        case 0:     para_set(P_GaI(val_floor_preheat_temp),val);        break;	/* ����ůԤ���¶ȡ� */
        case 1:     para_set(P_GaI(val_floor_preheat_interval),val);    break;	/* ����ůԤ��ʱ���� */
        case 2:     para_set(P_GaI(val_floor_preheat_max),val);         break;	/* ����ůԤ��ʱ���� */
	}
}

/***********************************************************************
 �� ��: �����ů��ʱ�����·�
 �� ��:
************************************************************************/
void SHC_set_floor_timer_val_deal(U08 idx, I16 val)
{
    switch (idx)
    {
        case 0:     para_set(P_GaI(val_floor_dry_head),val);   break;		/* ����ů���ǰ�� */
        case 1:     para_set(P_GaI(val_floor_dry_mid),val);    break;		/* ����ů����С� */
        case 2:     para_set(P_GaI(val_floor_dry_tail),val);   break;		/* ����ů���β�� */
        case 3:		para_set(P_GaI(val_floor_dry_temp),val);   break;		/* ����ů����¶ȡ� */
    }
}

/***********************************************************************
 �� ��: ������ʾ���̼�״̬�·�
 �� ��:
************************************************************************/
void SHC_set_test_fac_hmi_info_deal(SH_TEST_FAC_HMI_STRU info)
{
	TF_wifi_sta = info.wr.wifi_sta;
}

/***********************************************************************
 �� ��: ��ȡͿѻ��Ϣ
 �� ��: û�д˹����򷵻�TRUE
************************************************************************/
#define SHC_get_tuya_info(dest_buf)            (TYB_upload_product_info((U08*)dest_buf))

/***********************************************************************
 �� ��: ��ȡͿѻ����
 �� ��: û�д˹����򷵻�TRUE
************************************************************************/
#define SHC_get_tuya_data(dest_buf,data_addr)  (TYB_SLP_SH_upload_tuya_data((U08*)dest_buf, (U16)data_addr))

/***********************************************************************
 �� ��: ����Ϳѻ�����·�
 �� ��: û�д˹����򷵻�TRUE
************************************************************************/
#define SHC_set_tuya_data(src_buf)             (TYB_parse_tuya_data((U08*)src_buf))


