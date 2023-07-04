/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: SmartHmi_config.c
 文件描述: 适配彩屏时需要检查和修改的内容
 创建人  : lhb
 创建日期: 2021.05.08
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
 功 能: 空调开关机命令无效(无法在线控器主界面下执行A/B区开机)
 其 它: 空
************************************************************************/
BOL SHC_ac_on_off_disabled(void)
{
    if (fg_floor_preheat_run                /* 地暖预热 */
    || fg_floor_dryup_run	                /* 地暖烘干 */
    || USE_UIRT_ZONE_A_MODE_SWITCH			/* 温控器-单区模式切换 */
    || USE_UIRT_ZONE_A_ON_OFF				/* 温控器-单区开关 */
    || USE_UIRT_DUAL_ZONE_ON_OFF			/* 温控器-双区开关 */
    )
    {
        return TRUE;
    }

    return FALSE;
}

/***********************************************************************
 功 能: 假日离家开关机命令无效(无法在线控器主界面下执行A/B区开机)
 其 它: 空
************************************************************************/
BOL SHC_holiday_away_on_off_disabled(void)
{
    if (fg_floor_preheat_run                /* 地暖预热 */
    || fg_floor_dryup_run	                /* 地暖烘干 */
    )
    {
        return TRUE;
    }

    return FALSE;
}

/***********************************************************************
 功 能: 热水开关机命令无效(无法在线控器主界面下执行热水开机)
 其 它: 空
************************************************************************/
BOL SHC_hotw_on_off_disabled(void)
{
    if (fg_floor_preheat_run                /* 地暖预热 */
    || fg_floor_dryup_run	                /* 地暖烘干 */
    )
    {
        return TRUE;
    }

    return FALSE;
}

/***********************************************************************
 功 能: 模式切换命令无效(无法在线控器主界面下设置空调模式)
 其 它: 空
************************************************************************/
BOL SHC_ac_mode_switch_disabled(void)
{
    if (fg_floor_preheat_run                                /* 地暖预热 */
    || fg_floor_dryup_run	                                /* 地暖烘干 */
    || USE_UIRT_ZONE_A_MODE_SWITCH			                /* 温控器-单区模式切换 */
    || GETBIT_N(fg_test_run, TEST_RUN_COOL_MODE)			/* 制冷试运行时 */
    || GETBIT_N(fg_test_run, TEST_RUN_HEAT_MODE)			/* 制热试运行时 */
    )
    {
        return TRUE;
    }

    return FALSE;
}

/***********************************************************************
 功 能: SmartHmi_config.c内的变量初始化
 其 它: 空
************************************************************************/
void SHC_init(void)
{

}

/***********************************************************************
 功 能: SmartHmi_config.c内的计时
 其 它: 空
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
 功 能: 模块数
 其 它: 空
************************************************************************/
#define SHC_DISP_PCB_NUM            (val_pcb_num)

/***********************************************************************
 功 能: 压机数
 其 它: 空
************************************************************************/
#define SHC_DISP_COMP_NUM           (val_comp_num)

/***********************************************************************
 功 能: 机组状态
 其 它: 存在status_run之外的状态时，需每秒切换一次
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

    /* 提取有效状态 */
    for (i=0; i<SH_MAC_STATUS_MAX; i++)
    {
        if (GET_STA_BIT(i))
        {
            sta_list[sta_num++] = i;
        }
    }

    /* 切换显示 */
    if (SH_timer_1s[SH_TIME_1S_STATUS] >= 1)
    {
        SH_timer_1s[SH_TIME_1S_STATUS] = 0;

        sta_idx++;
    }
    if (sta_idx >= sta_num)            sta_idx = 0;

    return sta_list[sta_idx];
}

/***********************************************************************
 功 能: 当前温度
 其 它: 固定为1位小数
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
 功 能: 设定温度
 其 它: 小数点位数与SH_DISP_GET_SET_TMP_DOT一致
************************************************************************/
I16 SHC_get_set_tmp_A(void)
{
	I16 temp = 0xFFFF;
	U08 grp = 0xFF;
	U08 idx = 0xFF;

	log_sys_get_sv_disp_zone_A(&grp, &idx, &temp);

	if (grp != 0xFF && idx != 0xFF)		/* 参数值 已包含华氏度转换 不能用temp==0xFFFF因为temp是I16 */
	{
		temp = para_get_val(grp, idx);
	}
	else					/* 固定值需要除以10 */
	{
		temp = fun_pv_div_10(temp);
		if (TEMP_IS_FARA)
		{
			temp = C_To_F(temp, UINT_C);	/* 没有小数点 */
		}
	}

    return temp;
}

/***********************************************************************
 功 能: 主界面温度单位
 其 它:
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
 功 能: 空调可设模式(制冷/制热/自动)
 其 它:
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
 功 能: 设定空调模式
 其 它: 在运行某些特定功能时，可强制设置为制冷/制热模式
************************************************************************/
U16 SHC_get_set_mode(void)
{
    U16 temp = 0;

    if (GETBIT_N(fg_test_run, TEST_RUN_COOL_MODE)			/* 制冷试运行时 */
    ||  fg_refri_recy_act	                                /* 收氟 */
    )
    {
        SETBIT(temp, SH_MODE_COOL);
    }
    else if (fg_floor_preheat_run			                /* 地暖预热运行时 */
    || fg_floor_dryup_run	                                /* 地暖烘干运行时 */
    || GETBIT_N(fg_test_run, TEST_RUN_HEAT_MODE)			/* 制热试运行时 */
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
 功 能: 运行模式
 其 它:
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
 功 能: 初始化状态
 其 它: 需检查其状态定义与SH_的定义是否一致
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
 功 能: 使用期限是否到期
 其 它:
************************************************************************/
#define SHC_DISP_GET_LMT_ON         (fg_sys_limit)

/***********************************************************************
 功 能: 使用期限设置天数
 其 它:
************************************************************************/
#define SHC_DISP_GET_LMT_SET_DAY    (limit_value)

/***********************************************************************
 功 能: 已运行时间
 其 它: 需检查系统运行时间的定义是否正确
************************************************************************/
#define SHC_DISP_GET_LMT_RUN_HOUR   (cell[MAX_PCB].cell_runtime[SYS_RUNTIME])

/***********************************************************************
 功 能: 使用期限剩余时间
 其 它:
************************************************************************/
#define SHC_DISP_GET_LMT_LEFT_HOUR  (pre_limit_time)

/***********************************************************************
 功 能: 当前故障个数
 其 它:
************************************************************************/
U16 SHC_get_err_num(void)
{
    U16 i = 0;
    U16 j = 0;
    U16 k = 0;
    U16 temp = 0;

    for (i=0; i<val_pcb_num; i++)           /* 统计模块故障 */
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
    for (j=0; j<MAX_ERR_WORD; j++)          /* 统计系统故障 */
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
 功 能: 历史故障个数
 其 它:
************************************************************************/
#define SHC_DISP_GET_HIS_NUM        (SHC_get_his_num())
U16 SHC_get_his_num(void)
{
    U16 temp = 0;
    U08 i;

    for (i=0; i<MAX_HIS_ERROR; i++)         /* 统计历史故障 */
    {
        if (his_err_tbl[i*HIS_ERR_SIZE] != F_NO_ERR)
        {
            temp++;
        }
    }

    return temp;
}

/***********************************************************************
 功 能: 参数组数
 其 它:
************************************************************************/
#define SHC_DISP_GET_PARA_GRP_NUM   (PARA_GROUP_NUM)

/***********************************************************************
 功 能: 密码组数
 其 它:
************************************************************************/
#define SHC_DISP_GET_PASS_GRP_NUM   (ARRELE(password))

/***********************************************************************
 功 能: WIFI功能是否使用
 其 它:
************************************************************************/
#define SHC_DISP_GET_WIFI_USE       (TRUE)

/***********************************************************************
 功 能: 预热剩余分钟数
 其 它: 注意有些微码还有允许预热环境温度的限制
************************************************************************/
#define SHC_DISP_GET_PREHEAT_MINT   log_sys_warm_time_get()

/***********************************************************************
 功 能: 设定温度最小值
 其 它: 小数点位数与SHC_DISP_GET_SET_TMP_DOT一致
************************************************************************/
I16 SHC_get_set_tmp_min_A(void)
{
    I16 temp = SENSOR_NOEXIST;

    /* 空调能调目标温度被改写时，限制范围显示 */
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
 功 能: 设定温度最大值
 其 它: 小数点位数与SHC_DISP_GET_SET_TMP_DOT一致
************************************************************************/
I16 SHC_get_set_tmp_max_A(void)
{
    I16 temp = SENSOR_NOEXIST;

    /* 空调能调目标温度被改写时，限制范围显示 */
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
 功 能: 系统出水温度
 其 它: 水源热泵时需考虑是否切换水路
************************************************************************/
#define SHC_DISP_GET_TMP_OUT        (SENSOR_NOEXIST)

/***********************************************************************
 功 能: 系统回水温度
 其 它: 水源热泵时需考虑是否切换水路
************************************************************************/
#define SHC_DISP_GET_TMP_IN         (SENSOR_NOEXIST)

/***********************************************************************
 功 能: 控制对象
 其 它:
************************************************************************/
#define SHC_DISP_GET_CTRL_OBJ       (log_sys_get_ctrl_object())

/***********************************************************************
 功 能: 系统模拟量个数
 其 它:
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
 功 能: 系统输出点个数
 其 它:
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
 功 能: 已运行压机数
 其 它:
************************************************************************/
#define SHC_DISP_GET_CM_RUN_NUM     (log_sys_comp_run_num(SUB_SYS_TYPE_MAX))

/***********************************************************************
 功 能: 调试模式使用否
 其 它: 无此功能默认值为0
************************************************************************/
#define SHC_DISP_GET_TEST_EN        (USED)

/***********************************************************************
 功 能: 手动除霜信息1
 其 它: 仅HD015A配HL051A/HL055A用，其他微码默认为0
************************************************************************/
#define SHC_DISP_GET_MNL_DEFRO_1    (SHC_redo_force_defrost_status(0))

/***********************************************************************
 功 能: 手动除霜信息2
 其 它: 仅HD015A配HL051A/HL055A用，其他微码默认为0
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
        status = SH_CELL_NO_RUN;            /* 每次切换PCB都要清一次，避免上次残留信息影响 */
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
        return_val[(pcb*2)/16] |= temp;     /* 一个模块占用2位 */
    }

    return return_val[sta_idx];
}

/***********************************************************************
 功 能: 设定温度小数点
 其 它:
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
 功 能: 用户查询继电器
 其 它: 仅适配HD015A的微码需要配置，其他微码默认为0
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
 功 能: 参数组大小
 其 它:
************************************************************************/
#define SHC_DISP_GET_PARA_GRP_SIZE  (GROUP_SIZE)

/***********************************************************************
 功 能: 使用期限设置天数最大值
 其 它:
************************************************************************/
#define SHC_DISP_GET_LMT_SET_MAX    (1365)

/***********************************************************************
 功 能: 是否有手动除霜
 其 它:
************************************************************************/
#define SHC_DISP_HAVE_MNL_DEFRO     (TRUE)

/***********************************************************************
 功 能: 运行过程中是否允许修改模式
 其 它:
************************************************************************/
#define SHC_DISP_RUN_ALLOW_SET_MODE (TRUE)

/***********************************************************************
 功 能: 是否有开机密码功能，仅美意专用HL051A/HL055A使用
 其 它:
************************************************************************/
#define SHC_DISP_HAVE_START_CODE_MEIYI  (FALSE)

/***********************************************************************
 功 能: 开机密码，仅美意专用HL051A/HL055A使用
 其 它:
************************************************************************/
#define SHC_DISP_START_CODE_MEIYI       (FALSE)

/***********************************************************************
 功 能: 是否有压机除霜
 其 它:
************************************************************************/
#define SHC_DISP_HAVE_CM_MNL_DEFRO      (FALSE)

/***********************************************************************
 功 能: 是否有湿度控制
 其 它:
************************************************************************/
#define SHC_DISP_HAVE_HUMI              (FALSE)

/***********************************************************************
 功 能: 是否有开机密码
 其 它: 若有，使用的密码权限是SH_PWR_START_MAC
************************************************************************/
#define SHC_DISP_HAVE_START_CODE        (FALSE)

/***********************************************************************
 功 能: 初始化参数是否需要验证密码
 其 它: 若有，使用的密码权限是SH_PWR_INIT_PARA
************************************************************************/
#define SHC_DISP_HAVE_RESET_PASS        (FALSE)

/***********************************************************************
 功 能: 是否可跳过预热
 其 它: 注意检查SH_CMD_OFF_OILHEAT命令的处理
************************************************************************/
#define SHC_DISP_HAVE_OFF_OILHEAT       (FALSE)

/***********************************************************************
 功 能: 是否可选快速预热
 其 它: 注意检查SH_CMD_QUICK_OILHEAT命令的处理
************************************************************************/
#define SHC_DISP_HAVE_QUICK_OILHEAT     (TRUE)

/***********************************************************************
 功 能: 是否有BMS设置界面
 其 它: 注意检查SH_CMD_BMS_PARA_SET命令的处理
************************************************************************/
#define SHC_DISP_HAVE_BMS_SET           (FALSE)

/***********************************************************************
 功 能: 是否为英文显示
 其 它:
************************************************************************/
#define SHC_DISP_IS_ENGLISH_DISP        (sh_language == RN_LANGUAGE_EN)

/***********************************************************************
 功 能: 是否禁止中英文切换
 其 它: 默认为FALSE，即：允许中英文切换
************************************************************************/
#define SHC_DISP_IS_FORBID_SET_LANGUAGE    (FALSE)

/***********************************************************************
 功 能: 是否为邦普变频器
 其 它: 默认为FALSE,则没有变频器信息界面
************************************************************************/
#define SHC_DISP_IS_PUNP_INVT       (FALSE)

/***********************************************************************
 功 能: 是否有密码查询界面
 其 它: 默认为FALSE
************************************************************************/
#define SHC_DISP_HAVE_PASS_CHK      (FALSE)

/***********************************************************************
 功 能: 是否有地暖烘干功能
 其 它: 注意检查SHC_DISP_HAVE_FLOOR_DRYUP命令的处理
************************************************************************/
#define SHC_DISP_HAVE_FLOOR_DRYUP        (USE_HEAT && HEAT_USE_FLH)

/***********************************************************************
 功 能: 是否有强制开辅助电热
 其 它: 注意检查SHC_DISP_HAVE_FORCE_IBH命令的处理
************************************************************************/
#define SHC_DISP_HAVE_FORCE_IBH          (IBH_cfg_type \
                                         && ((USE_HEAT && HEAT_USE_IBH && (ac_mode == MODE_HEAT)) \
                                            || (val_hotw_used && HOTW_USE_IBH)))

/***********************************************************************
 功 能: 是否有强制开水箱电热
 其 它: 注意检查SHC_DISP_HAVE_FORCE_TBH命令的处理
************************************************************************/
#define SHC_DISP_HAVE_FORCE_TBH          (val_hotw_used)

/***********************************************************************
 功 能: 是否有强制热水
 其 它: 注意检查SHC_DISP_HAVE_FORCE_HW_MODE命令的处理
************************************************************************/
#define SHC_DISP_HAVE_FORCE_HW_MODE      (val_hotw_used)

/***********************************************************************
 功 能: 是否有假日模式设置界面
 其 它: 注意检查SHC_DISP_HAVE_HOLIDAY_MODE命令的处理
************************************************************************/
#define SHC_DISP_HAVE_HOLIDAY_MODE       (!USE_COOL_ONLY)

/***********************************************************************
 功 能: 前面几个辅助信息的整合
 其 它:
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
 功 能: 是否有风盘信息显示界面
 其 它: 注意检查SHC_DISP_HAVE_FCUINFO命令的处理
************************************************************************/
#define SHC_DISP_HAVE_FCUINFO      (val_fcu_num > 0)

/***********************************************************************
 功 能: 是否能力测试界面
 其 它: 注意检查SHC_DISP_HAVE_ABILITY_TEST命令的处理
************************************************************************/
#define SHC_DISP_HAVE_ABILITY_TEST      (TRUE)

/***********************************************************************
 功 能: 是否有第二个设定温度
 其 它: 有两个区域则有第二个设定温度
************************************************************************/
#define SHC_DISP_HAVE_SET_TMP_EX        (USE_DUAL_ZONE_CTRL)

/***********************************************************************
 功 能: 是否有热水设定温度
 其 它: 有使用热水则有热水设定温度
************************************************************************/
#define SHC_DISP_HAVE_SET_HOT           (val_hotw_used)

/***********************************************************************
 功 能: 是否有变频参数界面
 其 它: 仅使用邦普变频器时才有此功能
************************************************************************/
#define SHC_DISP_HAVE_INVT_PARA         (val_inverter_used)

/***********************************************************************
 功 能: 是否有变频故障查询
 其 它: 仅使用邦普变频器时才有此功能
************************************************************************/
#define SHC_DISP_HAVE_INVT_ERR          (val_inverter_used)

/***********************************************************************
 功 能: 是否有收氟功能
 其 它:
************************************************************************/
#define SHC_DISP_HAVE_RECYCLE_REFRI     (TRUE)

/***********************************************************************
 功 能: 是否禁用按键音
 其 它:
************************************************************************/
#define SHC_DISP_HAVE_FORBID_KEY_BUZZER (FALSE)

/***********************************************************************
 功 能: 是否有强开外部热源
 其 它:
************************************************************************/
#define SHC_DISP_HAVE_FORCE_AHS         ((USE_HEAT && HEAT_USE_AHS && (ac_mode == MODE_HEAT)) \
                                        || (val_hotw_used && HOTW_USE_AHS))

/***********************************************************************
 功 能: 是否处于静音模式
 其 它:
************************************************************************/
#define SHC_DISP_IS_SILENT_MODE_ON      (fg_silent_mode)
/***********************************************************************
 功 能: 是否处于水泵试运行
 其 它:
************************************************************************/
#define SHC_DISP_IS_PUMP_TEST_ON        (GETBIT_N(fg_test_run, TEST_RUN_PUMP))

/***********************************************************************
 功 能: 是否处于制冷试运行
 其 它:
************************************************************************/
#define SHC_DISP_IS_COOL_TEST_ON        (GETBIT_N(fg_test_run, TEST_RUN_COOL_MODE))

/***********************************************************************
 功 能: 是否处于制热试运行
 其 它:
************************************************************************/
#define SHC_DISP_IS_HEAT_TEST_ON        (GETBIT_N(fg_test_run, TEST_RUN_HEAT_MODE))

/***********************************************************************
 功 能: 是否处于制热水试运行
 其 它:
************************************************************************/
#define SHC_DISP_IS_HOTWATER_TEST_ON    (GETBIT_N(fg_test_run, TEST_RUN_HOTW_MODE))

/***********************************************************************
 功 能: 是否处于系统排空调试
 其 它:
************************************************************************/
#define SHC_DISP_IS_SYSTEM_EMPTY_ON     (GETBIT_N(fg_test_run, TEST_RUN_AIR_EMPTY))

/***********************************************************************
 功 能: 是否使用童锁功能
 其 它:
************************************************************************/
#define SHC_DISP_HAVE_CHILD_LOCK        (val_lock_screen == TRUE)

/***********************************************************************
 功 能: 前面几个辅助信息的整合
 其 它:
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
 功 能: 是否可以强制开辅助电热
 其 它: 注意检查SHC_DISP_CAN_FORCE_IBH命令的处理
************************************************************************/
#define SHC_DISP_CAN_FORCE_IBH       ((STEP_IS_RUN)\
                                      && ((fg_hotw_need && HOTW_USE_IBH && !val_TBH_used)\
                                          || (fg_heat_need && HEAT_USE_IBH)))// 运行状态下 热水强制开 制热强制开

/***********************************************************************
 功 能: 是否可以强制开水箱电热
 其 它: 注意检查SHC_DISP_CAN_FORCE_TBH命令的处理
************************************************************************/
#define SHC_DISP_CAN_FORCE_TBH       ((STEP_IS_RUN)\
                                       && (fg_hotw_need && val_TBH_used))// 运行状态下 热水强制开

/***********************************************************************
 功 能: 前面几个辅助信息的整合
 其 它:
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
 功 能: 第二个设定温度
 其 它: B区设定温度
************************************************************************/
I16 SHC_get_set_tmp_B(void)
{
	I16 temp = 0xFFFF;
	U08 grp = 0xFF;
	U08 idx = 0xFF;

	log_sys_get_sv_disp_zone_B(&grp, &idx, &temp);

	if (grp != 0xFF && idx != 0xFF)		/* 参数值 已包含华氏度转换 不能用temp==0xFFFF因为temp是I16 */
	{
		temp = para_get_val(grp, idx);
	}
	else					/* 固定值需要除以10 */
	{
		temp = fun_pv_div_10(temp);
		if (TEMP_IS_FARA)
		{
			temp = C_To_F(temp, UINT_C);	/* 没有小数点 */
		}
	}

    return temp;
}

/***********************************************************************
 功 能: 第二个当前温度
 其 它:
************************************************************************/
I16 SHC_get_ctrl_tmp_B(void)
{
    I16 temp = pv_sys_TWout2_ac;            /* 空调总出水温度 */

	if (TEMP_IS_FARA)
	{
		temp = C_To_F(temp, UINT_C|D_DOT1);
	}

    return temp;
}

/***********************************************************************
 功 能: 第二个设定温度最大值
 其 它: 小数点位数与SHC_DISP_GET_SET_TMP_DOT一致
************************************************************************/
I16 SHC_get_set_tmp_max_B(void)
{
    I16 temp = SENSOR_NOEXIST;

    /* 空调能调目标温度被改写时，限制范围显示 */
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
 功 能: 第二个设定温度最小值
 其 它: 小数点位数与SHC_DISP_GET_SET_TMP_DOT一致
************************************************************************/
I16 SHC_get_set_tmp_min_B(void)
{
    I16 temp = SENSOR_NOEXIST;

    /* 空调能调目标温度被改写时，限制范围显示 */
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
 功 能: 热水设定温度
 其 它:
************************************************************************/
I16 SHC_get_hot_tmp_set(void)
{
	I16 temp = 0xFFFF;
	U08 grp = 0xFF;
	U08 idx = 0xFF;

	log_sys_get_sv_disp_zone_hot(&grp, &idx, &temp);

	if (grp != 0xFF && idx != 0xFF)		/* 参数值 已包含华氏度转换 不能用temp==0xFFFF因为temp是I16 */
	{
		temp = para_get_val(grp, idx);
	}
	else					/* 固定值需要除以10 */
	{
		temp = fun_pv_div_10(temp);
		if (TEMP_IS_FARA)
		{
			temp = C_To_F(temp, UINT_C);	/* 没有小数点 */
		}
	}

    return temp;
}

/***********************************************************************
 功 能: 热水设定温度最小值
 其 它:
************************************************************************/
I16 SHC_get_hot_tmp_set_min(void)
{
    /* 热水能调目标温度被改写时，不获取最小值 */
    if (fg_sv_temp_energy_overwrite_hotw)
    {
        return SHC_get_hot_tmp_set();
    }

    return para_get_min(P_GaI(sv_hotw));
}

/***********************************************************************
 功 能: 热水设定温度最大值
 其 它:
************************************************************************/
I16 SHC_get_hot_tmp_set_max(void)
{
    /* 热水能调目标温度被改写时，不获取最大值 */
    if (fg_sv_temp_energy_overwrite_hotw)
    {
        return SHC_get_hot_tmp_set();
    }

    return para_get_max(P_GaI(sv_hotw));
}

/***********************************************************************
 功 能: 系统维护类型信息
 其 它: 若无此项设置，返回0!!!
************************************************************************/
I16 SHC_get_lmt_type_info(void)
{
    I16 temp = 0;
    U16 dict_idx = 0;
    U08 min = 0;
    U08 max = 0;
    U08 i;

    /* 高8位为字典信息 */
    for (i=0; i<get_dict_num(); i++)
    {
        if(para_dict[i].pmsg == Msg_LimitType)
        {
            dict_idx = i + UNIT_OFFSET;
            break;
        }
    }
    /* 低8位的高4位为上限 */
    max = 1;
    /* 低8位的低4位为下限 */
    min = 0;

    temp = ((dict_idx & 0xFF) << 8)
         | ((max << 4) & 0xF0)
         | ((min << 0) & 0x0F);


    return temp;
}

/***********************************************************************
 功 能: 系统维护类型值
 其 它: 若无此项设置，返回0!!!
************************************************************************/
I16 SHC_get_lmt_type_value(void)
{
    I16 temp;

    temp = limit_type;

    return temp;
}

/***********************************************************************
 功 能: 变频故障信息个数
 其 它: 若无此项可返回0
************************************************************************/
I16 SHC_get_invt_err_num(void)
{
    return INV_ERR_PARA_INFO_NUM;
}

/***********************************************************************
 功 能: 当前静音模式级别
 其 它: 若无此项可返回0
************************************************************************/
I16 SHC_get_silent_level(void)
{
    return silent_mode_level;
}

/***********************************************************************
 功 能: 静音模式级别上下限 低8位为下限 高8位为上限
 其 它: 若无此项可返回0
************************************************************************/
I16 SHC_get_silent_level_attr(void)
{
    return MAKEWORD(SILENT_LEVEL_2, SILENT_LEVEL_1);
}

/***********************************************************************
 功 能: 下发室内温度的时间间隔 0表示使用前板的默认值30秒
 其 它: 若无此项可返回0
************************************************************************/
I16 SHC_get_room_temp_wr_intvl(void)
{
    return 0;
}

/***********************************************************************
 功 能: 获取后板当前语言
 其 它:
************************************************************************/
I16 SHC_get_bkbd_language(void)
{
    static I16 bak_language;

    if (fg_tuya_lang_switch)
    {
        /* APP发生语言切换，5秒后才允许屏幕刷新，计时期间不刷新3.5寸屏语言值 */
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
 功 能: 前面几个辅助信息的整合
 其 它:
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
 功 能: 显示语言配置前16位
 其 它:
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
 功 能: 显示语言配置后16位
 其 它:
************************************************************************/
U16 SHC_get_disp_language2(void)
{
	U16 temp = 0;

//	SETBIT(temp, SH_LANGUAGE_BG-16);

	return temp;
}

/***********************************************************************
 功 能: 报警音模式
 其 它: 没有此功能的默认返回0
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
 功 能: 厂家模式
 其 它: 目前只有7寸屏拿来判断是否有热水
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
 功 能: 热水温度
 其 它:
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
 功 能: 可设的湿度设定模式
 其 它:
************************************************************************/
#define SHC_DISP_GET_HUMI_MODE_VALID    (0)

/***********************************************************************
 功 能: 湿度设定模式
 其 它:
************************************************************************/
#define SHC_DISP_GET_HUMI_MODE_SET      (0)

/***********************************************************************
 功 能: 湿度运行模式
 其 它:
************************************************************************/
#define SHC_DISP_GET_HUMI_MODE_RUN      (0)

/***********************************************************************
 功 能: 当前湿度
 其 它:
************************************************************************/
#define SHC_DISP_GET_HUMI_CURT          (0)

/***********************************************************************
 功 能: 设定湿度
 其 它:
************************************************************************/
#define SHC_DISP_GET_HUMI_SET           (0)

/***********************************************************************
 功 能: 设定湿度最小值
 其 它:
************************************************************************/
#define SHC_DISP_GET_HUMI_SET_MIN       (0)

/***********************************************************************
 功 能: 设定湿度最大值
 其 它:
************************************************************************/
#define SHC_DISP_GET_HUMI_SET_MAX       (0)

/***********************************************************************
 功 能: 压机模拟量单元步长
 其 它:
************************************************************************/
#define SHC_COMP_PV_STEP                (128)

/***********************************************************************
 功 能: 开关量输出单元步长
 其 它:
************************************************************************/
#define SHC_DO_STEP                     (128)

/***********************************************************************
 功 能: 开关量输入单元步长
 其 它:
************************************************************************/
#define SHC_DI_STEP                     (128)

/***********************************************************************
 功 能: 控制器版本号
 其 它:
************************************************************************/
#define SHC_VER_PCB_NAME                (msg_idu_version)
#define SHC_VER_CODE(pcb_id)            (&idu_mac_ver[pcb_id][0])
#define SHC_VER_DATA_SIZE               (sizeof(APPFUNCODEVER) / sizeof(U08))

/***********************************************************************
 功 能: 变频器版本号
 其 它: 若没有变频器版本号，则将ver.func_code[]置为全0
************************************************************************/
/*******************************************************************************
 函 数: SHC_transform_vfd_ver()
 功 能:
 参 数: ver_str--;
        func_code--;
        ver_code--;
 返 回: 空
 其 它: 空
*******************************************************************************/
void SHC_transform_vfd_ver(U08 *ver_str, U16 func_code, U16 ver_code)
{
    #define SH_VER_VFD_CODE_NULL        (INVT_PARA_VALUE_NULL)

    U08 ver[32] = "X1.VF$$$$.$##.V100###";  /* 例：X1.VF250A.F01.V100A00 */
    char symbol[] = {'X', 'K', 'F', 'T', 'Q'};              /* 洗涤、空调、辅机、通用、其他 */

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

        /* 版本号转换 */
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
    U08 VerCode[MAX_VER_BYTE] = {0};        /* 用于将变频器版本号转换为U08数组 */

    ver->pcb_name = msg_invt_version;
    ver->pcb_id = MAKEWORD(pcb_id, NO_ID);

    /* 将变频器版本号转换为U08数组 */
    SHC_transform_vfd_ver(VerCode, invt_info_function(INVT_ID_1), invt_info_version(INVT_ID_1));

    /* ********************************************************** */

    /* 获取版本号字符串 */
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
 功 能: 外机板版本号
 其 它:
************************************************************************/
#define SHC_EXIO_VER_PCB_NAME               (msg_odu_version)
#define SHC_EXIO_VER_CODE(pcb_id, exio_id)  (&odu_mac_ver[pcb_id][0])
#define SHC_EXIO_VER_DATA_SIZE              (sizeof(APPFUNCODEVER) / sizeof(U08))

/***********************************************************************
 功 能: 4G信息
 其 它:
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
 功 能: 超级密码
 其 它: 若没有超级密码，请赋值为全是0xFF
************************************************************************/
void SHC_get_pass_super(U16 *dest_pass)
{
    MEMCPY((U08 *)dest_pass, (U08 *)&password[PASS_SUPER][0], MAX_PASS_BIT);
}

/***********************************************************************
 功 能: 开机密码
 其 它: 若没有开机密码，请赋值为全是0xFF
************************************************************************/
void SHC_get_pass_pop(U16 *dest_pass)
{
    MEMCPY((U08 *)dest_pass, (U08 *)&password[PASS_POP][0], MAX_PASS_BIT);
}

/***********************************************************************
 功 能: 假日模式
 其 它: 若没有假日模式，请赋值为全是0xFF
************************************************************************/
void SHC_get_holiday_info(SH_HOLIDAYINFO *info)
{
    ;
}

/***********************************************************************
 功 能: 故障查询使用到的故障名称字符串
 其 它:
************************************************************************/
#define SHC_NAME_M_ERR(err)         (msg_m_error[err])  /* 系统故障名称 */
#define SHC_NAME_S_ERR(err)         (msg_s_error[err])  /* 模块故障名称 */

BOOL SHC_get_curt_err(SH_ERR_CURT *err, U16 *sys_err_id, U16 *mod_id, U16 *mod_err_id)
{
    BOOL fg_err = FALSE;

    /* 系统故障 */
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
        /* 单元故障 */
        for (; *mod_id<MAX_PCB; *mod_err_id=0,(*mod_id)++)
        {
            for (; *mod_err_id<F_NO_ERR; (*mod_err_id)++)
            {
                /* 放在for循环开头，使k能加1，下次不会重复获取k */
                if (fg_err)  break;

                if (m_get_error(*mod_id, *mod_err_id))
                {
                    fg_err = TRUE;

                    /* 注意！！！：若变频故障改为系统级别故障，需要更改此处 */
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
            /* 放在for循环的结尾，防止c加1,导致遗漏本模块的其他故障 */
            if (fg_err)  break;
        }
    }

    return fg_err;
}

/***********************************************************************
 功 能: 历史故障信息
 其 它:
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

    /* 获取历史故障 */
    for (i=0; i<SHC_HIS_ERR_TBL_SIZE; i++)
    {
        his_err_one[i] = his_err_tbl[idx*SHC_HIS_ERR_TBL_SIZE+i];
    }

    /* 故障名、板号、故障号、故障时间 */
    err_id = SHC_NO_ERR;
    if (idx < SHC_MAX_HIS_NUM)
    {
        err_id  = his_err_one[0];
        err_pcb = his_err_one[1];
        sign    = his_err_one[2];
    }
    if (err_id < F_NO_ERR)
    {
        if (err_pcb == MAX_PCB)         /* 系统故障 */
        {
            err->name = SHC_NAME_M_ERR(err_id);
            err->id = MAKEWORD(NO_ID, NO_ID);
            err->err_id = err_id;
        }
        else            /* 单元故障 */
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
        err->name = SH_ERR_WEI_ZHI_GZ;  /* 名为"未知故障"则不显示 */
        err->id = 0;
        err->err_id = 0;
        err->time.month = 0;
        err->time.day = 0;
        err->time.hour = 0;
        err->time.mint= 0;
    }
}

/***********************************************************************
 功 能: 风盘信息
 其 它: 若没有风盘信息，不进行处理
************************************************************************/
void SHC_get_fcu_info(SH_FCU *info)
{
#if 0
    U08 i;

    static const U16 status[] =
    {
        dm603_face_menu_fcu_sta_00, // 未连接
        dm603_face_menu_fcu_sta_01, // 故障态
        dm603_face_menu_fcu_sta_02, // 待机态
        dm603_face_menu_fcu_sta_03, // 有需求
        dm603_face_menu_fcu_sta_04, // 无需求
        dm603_face_menu_fcu_sta_05, // 未定义
    };
    static const U16 mode[] =
    {
        dm603_nothing,
        dm603_face_menu_fcu_mode_01,    // 制冷
        dm603_face_menu_fcu_mode_02,    // 制热
        dm603_face_menu_fcu_mode_03,    // 通风
        dm603_face_menu_fcu_mode_04,    // 地暖
        dm603_face_menu_fcu_mode_05,    // 未知
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
 功 能: 模式信息
 其 它: 用于显示屏的定时功能
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
 功 能: 主界面信息
 其 它: 用于多联供主界面
************************************************************************/
void SHC_get_main_info(SH_MAIN_INFO *info)
{
    /* A区末端类型 */
    info->zone_1_mode            = ZONE_MODE_FCU;           /* 默认: 风盘 */
    if (ZONE_A_TERMINAL_DEVICE == TD_RAD)
    {
        info->zone_1_mode        = ZONE_MODE_RAD;           /* 散热片 */
    }
    else if (ZONE_A_TERMINAL_DEVICE == TD_FCU)
    {
        info->zone_1_mode        = ZONE_MODE_FCU;           /* 风盘 */
    }
    else if (ZONE_A_TERMINAL_DEVICE == TD_FLH)
    {
        info->zone_1_mode        = ZONE_MODE_FLOOR;         /* 地暖 */
    }
    /* A区是否处于运行状态 */
    info->fg_zone_1_is_run       = fg_zone_A_run;
    /* A区温度控制类型 */
    info->zone_1_temp_type       = temp_ctrl_type_zone_A;
    /* A区设定温度是否可修改 */
    info->fg_zone_1_can_set_temp = temp_set_can_edit_zone_A;
    /* A区压机启动 */
    info->fg_zone_1_comp_on      = fg_zone_A_run
                                   && MAC_IS_RUN
                                   && log_sys_ac_have_comp_run();
    /* A区状态 */
    info->zone_1_status          = ZONE_STA_NULL;
    if (fg_ability_test_run)
    {
        info->zone_1_status      = ZONE_STA_TEST;           /* 能力测试 */
    }
    else if (fg_refri_recy_act)
    {
        info->zone_1_status      = ZONE_STA_RECYCLE;		/* 收氟 */
    }
    else if (ZONE_A_AF)
    {
        info->zone_1_status      = ZONE_STA_ANTIFREZ;       /* 防冻 */
    }
    else if ((!DISP_IS_HOTW(SUB_SYS_AC_HOTW) && log_sys_defrost_comp_num(SUB_SYS_AC_HOTW))
    || log_sys_defrost_comp_num(SUB_SYS_AC_ONLY)
    )
    {
        info->zone_1_status      = ZONE_STA_DEFRO;          /* 除霜 */
    }
    else if (fg_holiday_home_run || fg_holiday_away_run)
    {
        info->zone_1_status      = ZONE_STA_HOLIDAY;        /* 假日 */
    }
    else if (fg_silent_mode)
    {
        info->zone_1_status      = ZONE_STA_SILENT;         /* 静音 */
    }
    else if (ModeIsEco)
    {
        info->zone_1_status      = ZONE_STA_ECO;            /* 节能 */
    }
    info->zone_1_addition        = ZONE_ADDITION_NULL;
    if (fg_zone_A_run && MAC_IS_RUN && HEAT_USE_AHS && log_sys_have_AHS_on(ZONE_AC))
    {
        info->zone_1_addition    = ZONE_ADDITION_HEAT;      /* 外部热源AHS */
    }
    else if (fg_zone_A_run && MAC_IS_RUN && HEAT_USE_IBH && log_sys_ac_have_IBH_on())
    {
        info->zone_1_addition    = ZONE_ADDITION_ELEC;      /* 辅助电热IBH */
    }
    /* A区未运行 */
    if (!fg_zone_A_run)
    {
        info->zone_1_addition    = ZONE_ADDITION_NULL;
    }

    /* B区末端类型 */
    info->zone_2_mode            = ZONE_MODE_FLOOR;         /* 默认: 风盘 */
    if (ZONE_B_TERMINAL_DEVICE == TD_RAD)
    {
        info->zone_2_mode        = ZONE_MODE_RAD;           /* 散热片 */
    }
    else if (ZONE_B_TERMINAL_DEVICE == TD_FCU)
    {
        info->zone_2_mode        = ZONE_MODE_FCU;           /* 风盘 */
    }
    else if (ZONE_B_TERMINAL_DEVICE == TD_FLH)
    {
        info->zone_2_mode        = ZONE_MODE_FLOOR;         /* 地暖 */
    }
    /* B区是否处于运行状态 */
    info->fg_zone_2_is_run       = fg_zone_B_run;
    /* B区温度控制类型 */
    info->zone_2_temp_type       = temp_ctrl_type_zone_B;
    /* B区设定温度是否可修改 */
    info->fg_zone_2_can_set_temp = temp_set_can_edit_zone_B;
    /* B区压机启动 */
    info->fg_zone_2_comp_on      = fg_zone_B_run 
                                   && MAC_IS_RUN 
                                   && log_sys_ac_have_comp_run();
    /* B区状态 */
    info->zone_2_status          = ZONE_STA_NULL;
//    if (fg_ability_test_run)
//    {
//        info->zone_2_status      = ZONE_STA_TEST;           /* 能力测试 */
//    }
//    else
    if (ZONE_B_AF)
    {
        info->zone_2_status      = ZONE_STA_ANTIFREZ;       /* 防冻 */
    }
    else if ((!DISP_IS_HOTW(SUB_SYS_AC_HOTW) && log_sys_defrost_comp_num(SUB_SYS_AC_HOTW))
    || log_sys_defrost_comp_num(SUB_SYS_AC_ONLY)
    )
    {
        info->zone_2_status      = ZONE_STA_DEFRO;          /* 除霜 */
    }
    else if (fg_holiday_home_run || fg_holiday_away_run)
    {
        info->zone_2_status      = ZONE_STA_HOLIDAY;        /* 注: B区无假日 */
    }
    else if (fg_silent_mode)
    {
        info->zone_2_status      = ZONE_STA_SILENT;         /* 静音 */
    }
    else if (ModeIsEco)
    {
        info->zone_2_status      = ZONE_STA_ECO;            /* 节能 */
    }
    info->zone_2_addition        = ZONE_ADDITION_NULL;
    if (fg_zone_B_run && MAC_IS_RUN && HEAT_USE_AHS && log_sys_have_AHS_on(ZONE_AC))
    {
        info->zone_2_addition    = ZONE_ADDITION_HEAT;      /* 外部热源AHS */
    }
    else if (fg_zone_B_run && MAC_IS_RUN && HEAT_USE_IBH && log_sys_ac_have_IBH_on())
    {
        info->zone_2_addition    = ZONE_ADDITION_ELEC;      /* 辅助电热IBH */
    }
    /* B区未运行 */
    if (!fg_zone_B_run)
    {
        info->zone_2_addition    = ZONE_ADDITION_NULL;
    }

    /* 制热水是否运行 */
    info->fg_zone_hotw_is_run    = fg_hotw_run;
    info->fg_zone_hotw_disinfect = fg_timed_disinfect_run;
    if (!MODE_IS_COOL(SUB_SYS_AC_HOTW) )		/* 非制冷时，才处理压机图标显示 */
    {
        info->fg_zone_hotw_comp_on   = fg_hotw_run 
                                       && MAC_IS_RUN 
                                       && log_sys_hotw_have_comp_run();
    }    
//    if (fg_ability_test_run)
//    {
//        info->zone_hotw_status   = ZONE_STA_TEST;           /* 能力测试 */
//    }
//    else
    if (HOTW_AF)
    {
        info->zone_hotw_status   = ZONE_STA_ANTIFREZ;       /* 防冻 */
    }
    else if (DISP_IS_HOTW(SUB_SYS_AC_HOTW) && log_sys_defrost_comp_num(SUB_SYS_AC_HOTW))
    {
        info->zone_hotw_status   = ZONE_STA_DEFRO;          /* 除霜 */
    }
    else if (fg_holiday_away_run)
    {
        info->zone_hotw_status   = ZONE_STA_HOLIDAY;        /* 假日 */
    }
    else if (fg_silent_mode)
    {
        info->zone_hotw_status   = ZONE_STA_SILENT;         /* 静音 */
    }
    else if (ModeIsEco)
    {
        info->zone_hotw_status   = ZONE_STA_ECO;            /* 节能 */
    }
    info->zone_hotw_addition     = ZONE_ADDITION_NULL;
    if (fg_hotw_run && MAC_IS_RUN && HOTW_USE_AHS && log_sys_have_AHS_on(ZONE_HOTW))
    {
        info->zone_hotw_addition   = ZONE_ADDITION_HEAT;    /* 外部热源AHS */
    }
    else if (val_solar_used && get_output(MAX_PCB, M_DO_PUMPsl))
    {
        info->zone_hotw_addition   = ZONE_ADDITION_SOLAR;   /* 太阳能水泵PUMPsl */
    }
    else if (val_TBH_used && get_output(MAX_PCB, M_DO_TBH))
    {
        info->zone_hotw_addition   = ZONE_ADDITION_TANK_HEATER;     /* 水箱电热TBH */
    }
    else if (fg_hotw_run && MAC_IS_RUN && HOTW_USE_IBH && log_sys_hotw_have_IBH_on())
    {
        info->zone_hotw_addition   = ZONE_ADDITION_ELEC;    /* 辅助电热IBH */
    }

    /* 热水未运行 */
    if (!fg_hotw_run)
    {
        info->zone_hotw_addition   = ZONE_ADDITION_NULL;
    }

    /* 变频水泵PUMPf */
    if (log_sys_cell_pumpf_on())
    {
        info->fg_pumpi_is_on       = TRUE;
    }
    else
    {
        info->fg_pumpi_is_on       = FALSE;
    }

    /* 智能电网 */
    switch (smart_grid_cmd)
    {
        case SG_CMD_1:
            info->smart_grid_sta = SMART_GRID_VALLEY;       /* 低谷用电 */
            break;
        case SG_CMD_2:
            info->smart_grid_sta = SMART_GRID_PEAK;         /* 高峰用电 */
            break;
        case SG_CMD_3:
        case SG_CMD_4:
            info->smart_grid_sta = SMART_GRID_FREE;         /* 免费用电 */
            break;
    }

    /* 补充处理: 防冻 */
    if (FG_AF_ALL)
    {
        /* A区 */
        if (idle_sta_antifreeze_stage==AF_STAGE_AC              /* 待机态空调水路防冻 */
        ||  fg_run_sta_ac_antifreeze			                /* 运行态空调水路防冻 */
        ||  ZONE_A_TAF				                            /* A区定时防冻 */
        )
        {
            info->fg_zone_1_comp_on = log_sys_ac_have_comp_run();

            if (log_sys_have_AHS_on(ZONE_AC))
            {
                info->zone_1_addition = ZONE_ADDITION_HEAT;      /* 外部热源AHS */
            }
            else if (log_sys_ac_have_IBH_on())
            {
                info->zone_1_addition = ZONE_ADDITION_ELEC;      /* 辅助电热IBH */
            }
        }

        /* B区 */
        if (idle_sta_antifreeze_stage==AF_STAGE_AC              /* 待机态空调水路防冻 */
        ||  fg_run_sta_ac_antifreeze			                /* 运行态空调水路防冻 */
        ||  ZONE_B_TAF				                            /* B区定时防冻 */
        )
        {
            info->fg_zone_2_comp_on = log_sys_ac_have_comp_run();

            if (log_sys_have_AHS_on(ZONE_AC))
            {
                info->zone_2_addition = ZONE_ADDITION_HEAT;      /* 外部热源AHS */
            }
            else if (log_sys_ac_have_IBH_on())
            {
                info->zone_2_addition = ZONE_ADDITION_ELEC;      /* 辅助电热IBH */
            }
        }
        
        /* 热水区 */
        if (idle_sta_antifreeze_stage==AF_STAGE_HOTW            /* 待机态热水水路防冻 */
        ||  fg_run_sta_hotw_antifreeze			                /* 运行态热水水路防冻 */
        ||  HOTW_TAF				                            /* 热水定时防冻 */
        )
        {
            info->fg_zone_hotw_comp_on = log_sys_hotw_have_comp_run();

            if (log_sys_have_AHS_on(ZONE_HOTW))
            {
                info->zone_hotw_addition = ZONE_ADDITION_HEAT;    /* 外部热源AHS */
            }
            else if (log_sys_hotw_have_IBH_on())
            {
                info->zone_hotw_addition = ZONE_ADDITION_ELEC;    /* 辅助电热IBH */
            }
        }
    }
}

/***********************************************************************
 功 能: 获取变频故障信息
 其 它:
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
 功 能: 参数组属性
 其 它: 包括:权限、隐藏;
************************************************************************/
U16 SHC_get_para_grp_sign(U08 grp)
{
    extern PARAMITEM  ParamPri[];
    U16 sign = 0;
    U08 i;
    U16 min_pwr = D_FAC;

	/* 先获取除了权限以外的属性 */
    sign = ParamPri[grp].sign & (~D_SAFEMASK);

	/* 查询本组内需要显示的参数的最低要求权限 */
	for (i=0; i<ParamPri[grp].items; i++)
	{
		U16 para_sign = para_get_sign(grp, i);

		if (D_HIDE != (para_sign & D_HIDE))
		{
			para_sign &= D_SAFEMASK;

			min_pwr = MIN(min_pwr, para_sign);
		}
	}

	/* 将权限属性合并到组属性 */
	sign |= min_pwr;

    return sign;
}

/***********************************************************************
 功 能: 参数组的参数个数
 其 它:
************************************************************************/
#define SHC_get_para_grp_item(grp)      (ParamPri[grp].items)

/***********************************************************************
 功 能: 参数组字符串
 其 它:
************************************************************************/
#define SHC_NAME_PARA_GRP(grp)          (para_large[grp])   /* 参数组名称 */

/***********************************************************************
 功 能: 获取字典总数
 其 它:
************************************************************************/
#define SHC_GET_DICT_NUM                (get_dict_num())

/***********************************************************************
 功 能: 获取字典序号
 其 它:
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
 功 能: 获取字典项个数
 其 它:
************************************************************************/
#define SHC_get_dict_items(dict_idx)        (para_dict[dict_idx].items)

/***********************************************************************
 功 能: 获取字典指针
 其 它:
************************************************************************/
#define SHC_get_dict_pmsg(x, y)         (para_dict[x].pmsg[y])

/***********************************************************************
 功 能: 获取参数属性
 其 它:
************************************************************************/
#define SHC_get_para_get_sign(grp, item)    (para_get_sign(grp, item))

/***********************************************************************
 功 能: 获取参数字符串选项
 其 它:
************************************************************************/
#define SHC_get_para_small_pmsg(grp, item)  (para_small[grp][item].pmsg)

/***********************************************************************
 功 能: 获取参数名称字符串
 其 它:
************************************************************************/
#define SHC_NAME_PARA(grp, item)            (para_small[(grp)][(item)].msg) /* 参数名称 */

/***********************************************************************
 功 能: 获取参数值
 其 它:
************************************************************************/
I16 SHC_para_get_val(U16 grp, U16 item)
{
    return para_get_val(grp, item);
}

/***********************************************************************
 功 能: 获取参数设置下限
 其 它:
************************************************************************/
#define SHC_para_get_min(grp, item)         (para_get_min(grp, item))

/***********************************************************************
 功 能: 获取参数设置上限
 其 它:
************************************************************************/
#define SHC_para_get_max(grp, item)         (para_get_max(grp, item))

/***********************************************************************
 功 能: BMS设置
 其 它: 若没有此功能，随便赋值使编译不报警，并将SH_DISP_HAVE_BMS_SET置为FALSE
************************************************************************/
#define SHC_BMS_PARA_NUM            2       /* 后板实际BMS通讯参数项数（根据后板具体个数决定） */

#define SHC_BMS_PARA_NAME(item)     (0)     /* (para_small_bms[item].msg)      // BMS通讯设置参数名称 */
#define SHC_BMS_PARA_STR(item)      (NULL)  /* (para_small_bms[item].pmsg)     // BMS通讯设置参数字符串 */
#define SHC_BMS_PARA_VAL(item)      (0)     /* (modbus_para[item])             // BMS通讯设置参数值 */
#define SHC_BMS_PARA_MIN(item)      (0)     /* (modbus_para_data[item].min)    // BMS通讯设置参数最小值 */
#define SHC_BMS_PARA_MAX(item)      (0)     /* (modbus_para_data[item].max)    // BMS通讯设置参数最小值 */
#define SHC_BMS_PARA_SIGN(item)     (0)     /* (modbus_para_data[item].sign)   // BMS通讯设置参数属性 */

/***********************************************************************
 功 能: 各模块压机数,高8位为扩展板个数
 其 它: 若只有单模块，直接返回压机数
************************************************************************/
#define SHC_pcb_comp_num(pcb_id)    (MAKEWORD(1, val_comp_num))

/***********************************************************************
 功 能: 手动调试DO输出
 其 它: 若不用此功能，随便赋值给字符串，并将SHC_DISP_GET_TEST_EN置为FALSE
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
    /* 内机部分 */
    { msg_SHC_test_out_DFR,        MAX_PCB,        0xff,       M_DO_DFR,     }, /* 除霜 */
    { msg_SHC_test_out_ERR,        MAX_PCB,        0xff,       M_DO_ERR,     }, /* 故障 */
    { msg_SHC_test_out_AHS,        MAX_PCB,        0xff,       M_DO_AHS,     }, /* 外部热源 */
    { msg_SHC_test_out_S3V1,       MAX_PCB,        0xff,       M_DO_S3V1,    }, /* 电磁三通阀1 */
    { msg_SHC_test_out_S3V3,       MAX_PCB,        0xff,       M_DO_S3V3,    }, /* 电磁三通阀3 */
    { msg_SHC_test_out_S3V2,       MAX_PCB,        0xff,       M_DO_S3V2,    }, /* 电磁三通阀2 */
    { msg_SHC_test_out_HT,               0,    MAX_COMP,       S_DO_HT,      }, /* 防冻加热带 */
    { msg_SHC_test_out_PUMPret,    MAX_PCB,        0xff,       M_DO_PUMPret, }, /* 下回水泵 */
    { msg_SHC_test_out_PUMPsl,     MAX_PCB,        0xff,       M_DO_PUMPsl,  }, /* 太阳能水泵 */
    { msg_SHC_test_out_PUMPm,      MAX_PCB,        0xff,       M_DO_PUMPm,   }, /* 混合水泵 */
    { msg_SHC_test_out_TBH,        MAX_PCB,        0xff,       M_DO_TBH,     }, /* 水箱电热 */
    { msg_SHC_test_out_IBH1,             0,    MAX_COMP,       S_DO_IBH1,    }, /* 辅助电热1 */
    { msg_SHC_test_out_IBH2,             0,    MAX_COMP,       S_DO_IBH2,    }, /* 辅助电热2 */
    { msg_SHC_test_out_PUMPo,      MAX_PCB,        0xff,       M_DO_PUMPo,   }, /* 外循环泵 */
    { msg_SHC_test_out_PUMPf,            0,    MAX_COMP,       S_DO_PUMPf,   }, /* 变频水泵 */

    /* 外机部分 */
    { msg_SHC_test_out_QZH1,             0,           0,       SW_crank1,    },	/* 曲轴电加热1 */
    { SHC_EXDO7_MSG,            SHC_EXDO7_PCB, SHC_EXDO7_CM,   SHC_EXDO7_DO, },	/* 曲轴2\底盘2 */
    { msg_SHC_test_out_DPH,              0,    MAX_COMP,       S_DO_DPH,     },	/* 底盘热 */
    { msg_SHC_test_out_RV,               0,           0,       SW_S1,        },	/* 四通阀 */
    { msg_SHC_test_out_EXVli,            0,           0,       SW_byway,     },	/* 喷液阀 */
};
#define SHC_TEST_DO_NUM             (MIN(ARRELE(SHC_test_do_info), MAX_CHK_TEST_DO_NUM))      /* 不能超出最大 */


/*******************************************************************************
 函 数: SHC_test_do_timer()
 功 能: DO开启持续时间计时
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void SHC_test_do_timer(void)
{
    U08 i;
    if (test_data.wo.DO_mnl_mode)
    {
        for (i=0; i<SHC_TEST_DO_NUM; i++)
        {
            /* 系统 */
            if (SHC_test_do_info[i].pcb_id == MAX_PCB)
            {
                if (SHC_test_do_info[i].do_id == M_DO_TBH)
                {
                    _CNT_WHEN(cnt_test_do_TBH, GETBIT_N(test_data.wr.DO_sta, i));
                }
            }
            /* 模块 */
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
            /* 压机 */
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
 函 数: SHC_set_sys_test_do()
 功 能: 手动调试系统DO输出动作
 参 数: 空
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
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
            if (SHC_test_do_info[i].pcb_id == MAX_PCB)  // 系统输出才在此处理
            {
                clr_output(MAX_PCB, SHC_test_do_info[i].do_id);
                if (GETBIT_N(test_data.wr.DO_sta, i))
                {
                    set_output(MAX_PCB,SHC_test_do_info[i].do_id);
                }
            }
        }

        /* 水箱电热开启超5s后，自动关闭，防止遗忘 */
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
 函 数: SHC_set_mod_test_do()
 功 能: 手动调试模块DO输出动作
 参 数: 空
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
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
            if (SHC_test_do_info[i].pcb_id != MAX_PCB)  // 模块输出才在此处理
            {
                s_clr_output(SHC_test_do_info[i].comp_id, SHC_test_do_info[i].do_id);
                if (GETBIT_N(test_data.wr.DO_sta, i))
                {
                    s_set_output(SHC_test_do_info[i].comp_id, SHC_test_do_info[i].do_id);
                }
            }
        }

        if (get_output(MAX_PCB,M_DO_TBH)	/* TBH开启时，IBH1保持关闭 */
        || !IBH1_IS_USE	                    /* 无IBH1时，不开启IBH1 */
        || cnt_test_do_IBH1 > 5				/* IBH1开启超5s后，自动关闭，防止遗忘 */
        )
        {
            s_clr_output(MAX_COMP, S_DO_IBH1);
        }
        if (get_output(MAX_PCB,M_DO_TBH)	/* TBH开启时，IBH2保持关闭 */
        || !IBH2_IS_USE	                    /* 无IBH2时，不开启IBH2 */
        || cnt_test_do_IBH2 > 5				/* IBH2开启超5s后，自动关闭，防止遗忘 */
        )
        {
            s_clr_output(MAX_COMP, S_DO_IBH2);
        }
        /* 防冻结电加热带开启超5s后，自动关闭，防止遗忘 */
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
 功 能: 当前是否允许手动DO
 其 它: 若不用此功能，return FALSE
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
 功 能: 手动DO输出状态
 其 它: 若不用此功能，return FALSE
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
 功 能: 手动调试EEV
 其 它: 若不用此功能，随便赋值给字符串，并将SHC_DISP_GET_TEST_EN置为FALSE
************************************************************************/
//typedef struct
//{
//    U16 string;
//    U16 comp_id;
//    U16 eev_type;
//}SHC_TEST_EEV;
//const SHC_TEST_EEV SHC_test_eev_info[] =
//{
//    {msg_SHC_test_eev_01,     0,      0,},    /* {主路电膨1  } */
//};
//#define SHC_TEST_EEV_NUM            ((ARRELE(SHC_test_eev_info) > MAX_CHK_TEST_EEV_NUM) ? \
//                                    (MAX_CHK_TEST_EEV_NUM) : \
//                                    (ARRELE(SHC_test_eev_info)))     /* 不能超出最大 */
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
 功 能: 手动调试EEV的手动最大步数
 其 它: 若不用此功能，return 0
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
 功 能: 手动调试EEV的当前步数
 其 它: 若不用此功能，return 0
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
 功 能: 压机额外信息:运行时间，放到状态查询界面显示
 其 它:
************************************************************************/
#define SHC_COMP_EX_PER         1
#define SHC_COMP_EX_NUM         (SHC_USE_COMP*SHC_COMP_EX_PER)   /* 压机额外信息个数  如:压机运行时间 */

/***********************************************************************
 功 能: 计算模块模拟量个数
 其 它:
************************************************************************/
U08 SHC_common_count_mod_pv(U08 *dest, I16 *src)
{
    U16 i;
    U08 cursor = 0;

    /* 有效模拟量 */
    for (i = CELL_PV_BEGIN; i < MAX_PV_WORD; i++)
    {
        if (*src++ != SENSOR_NOEXIST)       /* 无效模拟量判断 */
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
 功 能: 计算压机模拟量个数
 其 它:
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
            /* 任意一个有效就有效 */
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
    cursor = cursor / comp_num;             /* 以上算法会导致不同压机的同个模拟量被重复计数，因此要除以comp_num */
    return cursor;
}
/***********************************************************************
 功 能: 计算DO输出点个数
 其 它:
************************************************************************/
DI_DO_INFO SHC_get_pcb_effect_do_logic(U08 pcb, U08 cm_max)
{
    DI_DO_INFO do_logic;
    U08 cm;

    /* 清零 */
    VALCLR(do_logic);

    /* 系统输出 */
    if (pcb == 0)
    {
        do_logic.sys[0] = assist_info[3];
        do_logic.sys[1] = assist_info[4];
    }

    /* 模块输出 */
    do_logic.cell = assist_info[2];

    /* 压机输出 */
    for (cm=0; cm<cm_max; cm++)
    {
        do_logic.comp[cm] = assist_info[1];
    }

    /* 特殊处理 */


    return do_logic;
}
/***********************************************************************
 功 能: 计算有效位个数
 其 它:
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
 功 能: 计算DI输入点个数
 其 它:
************************************************************************/
DI_DO_INFO SHC_get_pcb_effect_di_logic(U08 pcb, U08 cm_max)
{
    DI_DO_INFO di_logic;
    U08 cm;

    /* 清零 */
    VALCLR(di_logic);

    /* 系统输出 */
    if (pcb == 0)
    {
        di_logic.sys[0] = assist_info[7];
        di_logic.sys[1] = assist_info[8];
    }

    /* 模块输出 */
    di_logic.cell = assist_info[6];

    /* 压机输出 */
    for (cm=0; cm<cm_max; cm++)
    {
        di_logic.comp[cm] = assist_info[5];
    }

    /* 特殊处理 */

    return di_logic;
}
/***********************************************************************
 功 能: 计算单块板的查询信息个数
 其 它:
************************************************************************/
void SHC_get_info_num(SH_CHK_INFO_NUM *info_num, U16 pcb)
{
    DI_DO_INFO do_effect;
    DI_DO_INFO di_effect;

    info_num->cell_pv_num = SHC_common_count_mod_pv(PNULL, &cell[pcb].cell_pv[CELL_PV_BEGIN]);

    info_num->comp_pv_num = SHC_common_count_comp_pv(PNULL, pcb, SHC_USE_COMP)+SHC_COMP_EX_PER;   /* 加一个运行时间 */

    do_effect = SHC_get_pcb_effect_do_logic(pcb, m_val_comp_num(pcb));
    info_num->do_num = SHC_count_words_bits((U16*)&do_effect, sizeof(do_effect)/sizeof(U16));

    di_effect = SHC_get_pcb_effect_di_logic(pcb, m_val_comp_num(pcb));
    info_num->di_num = SHC_count_words_bits((U16*)&di_effect, sizeof(di_effect)/sizeof(U16));
}

/***********************************************************************
 功 能: 模块信息的名称字符串
 其 它:
************************************************************************/
#define SHC_NAME_PV_CELL(idx)       (msg_uint_p_cell[idx].msg)      /* 单元模拟量名称 */
#define SHC_ATTR_PV_CELL(idx)       (msg_uint_p_cell[idx].attr)     /* 单元模拟量属性 */

/***********************************************************************
 功 能: 模块信息的赋值
 其 它:
************************************************************************/
void SHC_get_mod_info(SH_CHK_INFO *info, U16 pcb_id, U16 idx)
{
    U08 cell_p_tbl[MAX_PV_WORD];
    U16 id_adj = 0;
    U08 cell_pv_id_adj = 0;
    I16 value = 0;

    /* 更新模块模拟量信息 */
    SHC_common_count_mod_pv(cell_p_tbl, &cell[pcb_id].cell_pv[CELL_PV_BEGIN]);
    /* 模拟量索引号 */
    id_adj = cell_p_tbl[idx];
    /* 单元元素信息索引号 */
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
 功 能: 系统信息的名称字符串
 其 它:
************************************************************************/
#define SHC_NAME_PV_SYS(idx)        (msg_sys_p[idx].msg)    /* 系统模拟量名称 */
#define SHC_ATTR_PV_SYS(idx)        (msg_sys_p[idx].attr)   /* 系统模拟量属性 */

/***********************************************************************
 功 能: 获取系统信息
 其 它:
************************************************************************/
void SHC_get_sys_info(SH_CHK_INFO *info, U16 idx)
{
    U08 sys_p_tbl[MAX_PV_WORD];
    U08 id_adj = 0;         /* 重映射后的序号 */
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

    info->attrib = (SHC_ATTR_PV_SYS(id_adj) == 0xFFFF) ? UNIT_OFFSET/* 显示字符串 */
                                                       : SH_sign_local2sh(SHC_ATTR_PV_SYS(id_adj));/* 1位小数,℃ */
}

/***********************************************************************
 功 能: 压机模拟量名称
 其 它:
************************************************************************/
#define SHC_NAME_PV_COMP(idx)       (msg_uint_p_comp[idx].msg)      /* 压机模拟量名称 */
#define SHC_ATTR_PV_COMP(idx)       (msg_uint_p_comp[idx].attr)     /* 压机模拟量属性 */

/***********************************************************************
 功 能: 获取压机信息
 其 它:
************************************************************************/
void SHC_get_comp_info(SH_CHK_INFO *info, U16 pcb_id, U16 idx)
{
    U08 comp_pv_num;
    U16 cmi;
    U16 exi;                /* 扩展信息相对索引号 */
    U08 id_adj = 0;         /* 重映射后的序号 */
    I16 value = 0;

    comp_pv_num = SHC_common_count_comp_pv(PNULL, pcb_id, SHC_USE_COMP)+SHC_COMP_EX_PER;

    cmi = idx / comp_pv_num;
    exi = idx % comp_pv_num;

    if (cmi < m_val_comp_num(pcb_id))
    {
        if (exi == 0)       /* 扩展信息 压机运行时间 */
        {
            info->name = msg_comp_runtime;
            info->value = cell[pcb_id].cell_runtime[cmi];
            info->id = MAKEWORD(pcb_id, cmi+1);
            info->attrib = (SH_UNIT_HOUR);
        }
        else                /* 模拟量 */
        {
            U08 pv_num;
            U08 avi = 0;
            U08 comp_p_tbl_buf[MAX_PV_WORD];
            pv_num = SHC_common_count_comp_pv(comp_p_tbl_buf, pcb_id, SHC_USE_COMP);
            avi = exi - SHC_COMP_EX_PER;     /* 减去扩展信息个数 */
            id_adj = comp_p_tbl_buf[avi];
            if (avi < pv_num)
            {
                /* 合成信息单元 */
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
        /* 超出的部分传单元模拟量 */
        U08 cell_p_tbl[MAX_PV_WORD];
        U16 id_adj = 0;
        U08 cell_pv_id_adj = 0;

        /* 更新模块模拟量信息 */
        if (SHC_common_count_mod_pv(cell_p_tbl, &cell[pcb_id].cell_pv[CELL_PV_BEGIN]) != 0)
        {
            /* 模拟量索引号 */
            id_adj = cell_p_tbl[idx - m_val_comp_num(pcb_id) * comp_pv_num];
            /* 单元元素信息索引号 */
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
 功 能: 继电器名称
 其 它:
************************************************************************/
#define SHC_NAME_DO_SYS(idx)        (msg_sys_o[idx])        /* 系统输出名称 */
#define SHC_NAME_DO_CELL(idx)       (msg_uint_o_cell[idx])  /* 单元输出名称 */
#define SHC_NAME_DO_COMP(idx)       (msg_uint_o_comp[idx])  /* 压机输出名称 */
/***********************************************************************
 功 能: 在数组内查找给定的数字，返回此数字在数组内的序号
 其 它:
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
 功 能: 获取DO信息
 其 它:
************************************************************************/
void SHC_get_do_info(SH_CHK_INFO *info, U16 pcb, U16 doi)
{
    DI_DO_INFO do_value;
    DI_DO_INFO do_effect;
    U16 n = 0;
    U16 j;

    /* 拼合开关量信息 */
    {
        n = 0;              /* 复位查找位置 */
        VALCLR(do_value);
        do_value.sys[0] = cell[MAX_PCB].cell_relay[0];
        do_value.sys[1] = cell[MAX_PCB].cell_relay[1];
        do_value.cell   = cell[pcb].cell_relay[MAX_COMP];
        for (j=0; j<m_val_comp_num(pcb); j++)
        {
            do_value.comp[j] = cell[pcb].cell_relay[j];
        }
        /* 拼合有效开关量 */
        do_effect = SHC_get_pcb_effect_do_logic(pcb, m_val_comp_num(pcb));

    }

    /* 合成信息单元 */
    if (doi < sizeof(DI_DO_INFO)*8)
    {
        /* 查找有效位 */
        n = SHC_get_effect_idx_bit((U16*)&do_effect, sizeof(do_effect)/sizeof(U16), doi);

        if(n != 0xFFFF)
        {
            U16 name = 0;
            U16 id = 0;

            if (n < 32)                     /* 前32位为系统DO */
            {
                name = SHC_NAME_DO_SYS(n);
                id = MAKEWORD(NO_ID, NO_ID);
            }
            else if (n < 32 + 16)           /* 中间16位为模块 */
            {
                name = SHC_NAME_DO_CELL(n-32);
                id = MAKEWORD(pcb, NO_ID);
            }
            else                            /* 剩下的为压机 */
            {
                name = SHC_NAME_DO_COMP((n-32-16)%16);
                id = MAKEWORD(pcb, NO_ID);
            }
            info->name = name;
            info->id = id;
            info->value = GETBIT(((U16*)&do_value)[n/16], n%16);
            info->attrib = 0;               /* 开关量无属性 */
        }
    }
}

/***********************************************************************
 功 能: DI输入点名称
 其 它:
************************************************************************/
#define SHC_NAME_DI_SYS(idx)        (msg_sys_i[idx])        /* 系统输入名称 */
#define SHC_NAME_DI_CELL(idx)       (msg_uint_i_cell[idx])  /* 单元输入名称 */
#define SHC_NAME_DI_COMP(idx)       (msg_uint_i_comp[idx])  /* 压机输入名称 */
/***********************************************************************
 功 能: 获取DO信息
 其 它:
************************************************************************/
void SHC_get_di_info(SH_CHK_INFO *info, U16 pcb, U16 dii)
{
    DI_DO_INFO di_value;
    DI_DO_INFO di_effect;
    U16 n = 0;
    U16 j;

    /* 拼合开关量信息 */
    {
        n = 0;              /* 复位查找位置 */
        VALCLR(di_value);
        di_value.sys[0] = cell[MAX_PCB].cell_input[0];
        di_value.sys[1] = cell[MAX_PCB].cell_input[1];
        di_value.cell   = cell[pcb].cell_input[MAX_COMP];
        for (j=0; j<m_val_comp_num(pcb); j++)
        {
            di_value.comp[j] = cell[pcb].cell_input[j];
        }
        /* 拼合有效开关量 */
        di_effect = SHC_get_pcb_effect_di_logic(pcb, m_val_comp_num(pcb));
    }


    /* 合成信息单元 */
    if (dii < sizeof(DI_DO_INFO)*8)
    {
        /* 查找有效位 */
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
            info->attrib = 0;               /* 开关量无属性 */
        }
    }
}

/***********************************************************************
 功 能: 获取手动除霜信息
 其 它:
************************************************************************/
void SHC_get_defro_info(SH_DEFRO_INFO *info, U16 pcb)
{
    U16 cmp;
    BOL is_cm_defr=FALSE;
    BOL is_cm_force_defr=FALSE;
    BOL is_cm_run=FALSE;
    U16 pcb_status = SH_DEFR_NO_RUN;

    /* 4 压机信息 */
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

    /* 4 模块信息 */
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

    /* 4 压机个数 */
    info->cell =   (pcb_status)
                | (m_val_comp_num(pcb) << 4);

}

/***********************************************************************
 功 能: 获取开机密码提示信息字符串
 其 它: 若无此功能，直接填充空字符串后返回
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
/* 生能HL055A的例子 */
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
 功 能: 获取使用禁用信息
 其 它:
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
 功 能: 获取用户查询的六个查询量
 其 它: 若无此功能，value填充为SENSOR_NOEXIST
************************************************************************/
void SHC_get_user_info(SH_CHK_INFO *info, U16 i)
{
    U16 str_chk_user[SH_MAX_CHK_USER_INFO] =
    {
        msg_sys_pv_08,    /* 出温 */
        msg_sys_pv_09,    /* 回温 */
        msg_sys_pv_03,    /* 环温 */
        msg_comp_pv_11,   /* 低压 */
        msg_comp_pv_12,   /* 高压 */
        msg_comp_pv_12,   /* 高压 */
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
 功 能: 获取指定的变频器寄存器地址的值
 其 它:
************************************************************************/
void SHC_get_invt_para(SH_INVT_PARA *info)
{
    info->addr     = info_invt_para[INVT_ID_1].addr;
    info->val      = info_invt_para[INVT_ID_1].val;
    info->str_tips = msg_nothing;
    /* 执行一次读变频器的操作 */
}

#define MSG_FLOOR_PREHEAT_TEMP            (msg_para_small_14_09)
#define MSG_FLOOR_PREHEAT_INTERVAL        (msg_para_small_14_10)
#define MSG_FLOOR_PREHEAT_MAX             (msg_para_small_14_11)

/***********************************************************************
 功 能: 获取指定的地暖预热参数
 其 它:
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
 功 能: 获取指定的定时地暖参数
 其 它:
************************************************************************/
void SHC_get_floor_timer_info(SH_PARA_ATTRIB *info, U08 idx)
{
    switch (idx)
    {
        /* 【地暖烘干前】 */
        default:
        case 0:
        	info->name =   MSG_FLOOR_DRYUP_HEAD;
        	info->id =     0xFFFF;
        	info->value =  para_get_val(P_GaI(val_floor_dry_head));
        	info->min =    para_get_min(P_GaI(val_floor_dry_head));
        	info->max =    para_get_max(P_GaI(val_floor_dry_head));
        	info->attrib = SH_sign_local2sh(para_get_sign(P_GaI(val_floor_dry_head)));
            break;

        /* 【地暖烘干中】 */
        case 1:
        	info->name =   MSG_FLOOR_DRYUP_MID;
        	info->id =     0xFFFF;
        	info->value =  para_get_val(P_GaI(val_floor_dry_mid));
        	info->min =    para_get_min(P_GaI(val_floor_dry_mid));
        	info->max =    para_get_max(P_GaI(val_floor_dry_mid));
        	info->attrib = SH_sign_local2sh(para_get_sign(P_GaI(val_floor_dry_mid)));
            break;

        /* 【地暖烘干尾】 */
        case 2:
        	info->name =   MSG_FLOOR_DRYUP_TAIL;
        	info->id =     0xFFFF;
        	info->value =  para_get_val(P_GaI(val_floor_dry_tail));
        	info->min =    para_get_min(P_GaI(val_floor_dry_tail));
        	info->max =    para_get_max(P_GaI(val_floor_dry_tail));
        	info->attrib = SH_sign_local2sh(para_get_sign(P_GaI(val_floor_dry_tail)));
            break;

        /* 【地暖烘干温度】 */
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
 功 能: 获取显示屏商检信息
 其 它:
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
 功 能: 处理开机命令
 其 它:
************************************************************************/
void SHC_command_ON_deal(U16 command)
{
    if (command)
    {
        DM603_CMD_RUN;
    }
}

/***********************************************************************
 功 能: 处理关机命令
 其 它:
************************************************************************/
void SHC_command_OFF_deal(U16 command)
{
    if (command)
    {
        DM603_CMD_STOP;
    }
}

/***********************************************************************
 功 能: 处理复位命令
 其 它:
************************************************************************/
void SHC_command_RST_deal(U16 command)
{
    if (command)
    {
        DM603_CMD_RESET;
    }
}

/***********************************************************************
 功 能: 处理密码故障复位命令
 其 它:
************************************************************************/
void SHC_command_PASS_RST_deal(U16 command)
{
    if (command)
    {
        ;
    }
}

/***********************************************************************
 功 能: 处理取消预热命令
 其 它:
************************************************************************/
void SHC_command_OFF_OILHEAT_deal(U16 command)
{
    if (command)
    {
        ;
    }
}

/***********************************************************************
 功 能: 处理手动除霜命令
 其 它:
************************************************************************/
void SHC_command_DEFRO_deal(U16 command)
{
    DM603_CMD_DEFROST(command);
}

/***********************************************************************
 功 能: 处理参数初始化命令
 其 它:
************************************************************************/
void SHC_command_PARA_INIT_deal(U16 command)
{
    if (command)
    {
        DM603_CMD_PARA_INIT;
    }
}

/***********************************************************************
 功 能: 处理压机运行时间初始化命令
 其 它:
************************************************************************/
void SHC_command_CM_RUNTM_INIT_deal(U16 command)
{
    if (command)
    {
        DM603_CMD_RUNTIME_INIT;
    }
}

/***********************************************************************
 功 能: 处理系统运行时间初始化命令
 其 它:
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
 功 能: 处理历史故障初始化命令
 其 它:
************************************************************************/
void SHC_command_HIS_ERR_INIT_deal(U16 command)
{
    if (command)
    {
        DataAccess_HistoryErr(DATA_INIT);   /* 初始化历史故障信息 */
    }
}

/***********************************************************************
 功 能: 处理变频器初始化命令
 其 它:
************************************************************************/
void SHC_command_INVERTER_INIT_deal(U16 command)
{
    if (command)
    {
        DM603_CMD_INVT_INIT;
    }
}

/***********************************************************************
 功 能: 处理手动水泵命令
 其 它:
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
 功 能: 处理BMS参数命令
 其 它:
************************************************************************/
void SHC_command_BMS_PARA_deal(U16 command)
{
    //U08 item = HIBYTE(command);
    I16 value = LOBYTE(command);
    if(value >= SHC_BMS_PARA_MIN(item) && value <= SHC_BMS_PARA_MAX(item))
    {
/*        SH_BMS_PARA_VAL(item) = value; */
        /* 通讯数据格式写入EEPROM */
/*      ModbusRS485EEprom(&SH_BMS_PARA_VAL(0), 1);  */
/*        Task_ModbusRS485(); */
    }
}

/***********************************************************************
 功 能: 处理快速预热命令
 其 它:
************************************************************************/
void SHC_command_QUICK_OILHEAT_deal(U16 command)
{
    if (command)
    {
        fg_warm_quick = TRUE;
    }
}

/***********************************************************************
 功 能: 处理涂鸦重新连接标志
 其 它:
************************************************************************/
void SHC_command_TUYA_NEW_LINK_deal(U16 command)
{
    if (command)
    {
        TYB_SET_FG_DPID_UPDATE_ALL;
    }
}

/***********************************************************************
 功 能: 处理进入地暖烘干功能
 其 它:
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
 功 能: 处理强开辅助电热
 其 它:
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
 功 能: 处理强开水箱电热
 其 它:
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
 功 能: 处理强开热水模式
 其 它:
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
 功 能: 处理能力测试
 其 它:
************************************************************************/
void SHC_command_ability_test_deal(U16 command)
{
    if (command)
    {
        SETCMD_N(ABILITY_TEST_ON);
    }
}

/***********************************************************************
 功 能: A区开机
 其 它:
************************************************************************/
void SHC_command_ON_zone_1_deal(U16 command)
{
    /* 不可开机 */
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
 功 能: A区关机
 其 它:
************************************************************************/
void SHC_command_OFF_zone_1_deal(U16 command)
{
    /* 不可关机 */
    if (SHC_ac_on_off_disabled())
    {
        return;
    }

    if (command)
    {
        SETCMD_N(ZONE_A_OFF);

        /* A区关机时，退出能力测试 */
        SETCMD_N(ABILITY_TEST_OFF);

        /* 退出水泵试运行 */
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
 功 能: B区开机
 其 它:
************************************************************************/
void SHC_command_ON_zone_2_deal(U16 command)
{
    /* 不可开机 */
    if (SHC_ac_on_off_disabled())
    {
        return;
    }

    if (command)
    {
        /* 空调区域模式为制冷时，B区不可开启 */
        if (ac_mode != MODE_COOL)
        {
            SETCMD_N(ZONE_B_ON);
        }

        /* B区开机时，退出能力测试 */
        SETCMD_N(ABILITY_TEST_OFF);
    }
}

/***********************************************************************
 功 能: B区关机
 其 它:
************************************************************************/
void SHC_command_OFF_zone_2_deal(U16 command)
{
    /* 不可关机 */
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
 功 能: 热水开机
 其 它:
************************************************************************/
void SHC_command_ON_hotwater_deal(U16 command)
{
    /* 不可开机 */
    if (SHC_hotw_on_off_disabled())
    {
        return;
    }

    if (command)
    {
        SETCMD_N(HOTW_ON);

        /* 热水开机时，退出能力测试 */
        SETCMD_N(ABILITY_TEST_OFF);
    }
}

/***********************************************************************
 功 能: 热水关机
 其 它:
************************************************************************/
void SHC_command_OFF_hotwater_deal(U16 command)
{
    /* 不可关机 */
    if (SHC_hotw_on_off_disabled())
    {
        return;
    }

    if (command)
    {
        SETCMD_N(HOTW_OFF);

        /* 热水关机时，退出定时杀菌 */
        SETCMD_N(TIMED_DISINFECT_OFF);
    }
}

/***********************************************************************
 功 能: 系统维护初始化
 其 它:
************************************************************************/
void SHC_command_limit_init_deal(U16 command)
{
    if (command)
    {
        DM603_CMD_LIMIT_INIT;
    }
}

/***********************************************************************
 功 能: 收氟功能
 其 它:
************************************************************************/
void SHC_command_recyle_refri_deal(U16 command)
{
    if (command)
    {
        SETCMD_N(REFRI_RECY_ON);
    }
}

/***********************************************************************
 功 能: 定时开热水泵
 其 它:
************************************************************************/
void SHC_command_TIMER_DHW_ON_deal(U16 command)
{
    if (command)
    {
        SETCMD_N(TIMED_PUMPRET_ON);
    }
}

/***********************************************************************
 功 能: 强开外部热源
 其 它:
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
 功 能: 杀菌
 其 它:
************************************************************************/
void SHC_command_disinfect_deal(U16 command)
{
    if (command)
    {
        SETCMD_N(TIMED_DISINFECT_ON);
    }
    else
    {
        SETCMD_N(TIMED_DISINFECT_OFF);           /* 手动退出定时杀菌功能 */
    }
}

/***********************************************************************
 功 能: 静音
 其 它:
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
 功 能: 水泵试运行
 其 它:
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
 功 能: 制冷试运行
 其 它:
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
 功 能: 制热试运行
 其 它:
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
 功 能: 制热水试运行
 其 它:
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
 功 能: 系统排空调试
 其 它:
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
 功 能: 地暖预热
 其 它:
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
 功 能: 定时地暖
 其 它:
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
 功 能: 假日离开命令处理
 其 它:
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
 功 能: 假日在家命令处理(功能使用且处于假日在家时间段内，每1分钟下发一次命令)
 其 它:
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
 功 能: 接受远程升级请求命令处理
 其 它:
************************************************************************/
void SHC_command_accept_remote_upgrade_req_deal(U16 command)
{
	fg_accept_remote_upgrade = command;
}

/***********************************************************************
 功 能: 处理温度设置
 其 它: 由于存在多区域控制，此函数实际未使用
************************************************************************/
BOOL SHC_set_tmp_deal(I16 set_data)
{
    BOOL result = TRUE;
    I16 para_min = 0;       /* 最小值 */
    I16 para_max = 0;       /* 最大值 */
    U08 grp_cursor = 0;
    U08 item_cursor = 0;         /* 参数位置 */

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
 功 能: 处理模式设置
 其 它:
************************************************************************/
BOOL SHC_set_mode_deal(I16 set_data)
{
    BOOL result = TRUE;
    I16 para_min = 0;       /* 最小值 */
    I16 para_max = 0;       /* 最大值 */
    I16 para_new = 0;       /* 新值 */

    /* 不可切换模式 */
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
 功 能: 处理使用期限设置
 其 它:
************************************************************************/
BOOL SHC_set_limit_time_deal(I16 set_data)
{
    limit_value = set_data;

    return TRUE;
}

/***********************************************************************
 功 能: 处理定时使用设置
 其 它:
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
// 功 能: 处理清除开机密码设置
// 其 它: 目前仅美意专用程序使用
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
 功 能: 处理湿度模式设置
 其 它:
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
 功 能: 处理语言设置
 其 它:
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
 功 能: 处理区域一温度设置
 其 它:
************************************************************************/
BOOL SHC_set_tmp_zone_1_deal(I16 set_data)
{
    BOOL result = TRUE;
    I16 para_min = 0;       /* 最小值 */
    I16 para_max = 0;       /* 最大值 */
    U08 grp_cursor = 0;     /* 参数组号 */
    U08 item_cursor = 0;    /* 参数项号 */

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
 功 能: 处理区域二温度设置
 其 它:
************************************************************************/
BOOL SHC_set_tmp_zone_2_deal(I16 set_data)
{
    BOOL result = TRUE;
    I16 para_min = 0;       /* 最小值 */
    I16 para_max = 0;       /* 最大值 */
    U08 grp_cursor = 0;     /* 参数组号 */
    U08 item_cursor = 0;    /* 参数项号 */

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
 功 能: 处理热水温度设置
 其 它:
************************************************************************/
BOOL SHC_set_tmp_zone_hotw_deal(I16 set_data)
{
    BOOL result = TRUE;
    I16 para_min = 0;       /* 最小值 */
    I16 para_max = 0;       /* 最大值 */

    /* 禁止修改【热水设定温度】的情况 */
    if (FREE_ELEC_FORCE_RUN     /* 免费用电强制运行 */
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
 功 能: 处理维护类型设置
 其 它:
************************************************************************/
BOOL SHC_set_lmt_type_deal(I16 set_data)
{
    BOOL result = TRUE;

    limit_type = set_data;

    return result;
}

/***********************************************************************
 功 能: 处理静音级别设置
 其 它:
************************************************************************/
BOOL SHC_set_silent_level_deal(I16 set_data)
{
    BOOL result = TRUE;

    silent_mode_level = (set_data < SILENT_MAX) ? set_data : SILENT_LEVEL_1;

    return result;
}

/***********************************************************************
 功 能: 处理温度单位设置
 其 它:
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
 功 能: 处理时钟下发
 其 它:
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
 功 能: 处理假日模式数据保存
 其 它:
************************************************************************/
void SHC_set_holidayinfo_deal(SH_HOLIDAYINFO info_buff)
{
    fg_holiday_away_heat_mode_used = info_buff.mode_heat_use;
    fg_holiday_away_hotw_mode_used = info_buff.mode_hotw_use;

    /* 制热模式使用，【控制模式】设定为"制热模式" */
    if (fg_holiday_away_heat_mode_used)
    {
        para_set(P_GaI(sv_mode), SET_HEAT);
    }
}

/***********************************************************************
 功 能: 处理室内温度数据保存
 其 它:
************************************************************************/
void SHC_set_room_temp_deal(I16 info)
{
    room_temp = info;
}

/***********************************************************************
 功 能: 处理参数设置下发
 其 它:
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
 功 能: 处理变频参数下发
 其 它:
************************************************************************/
void SHC_set_invt_para_deal(SH_INVT_PARA info_buff)
{
    info_invt_para[INVT_ID_1].addr = info_buff.addr;
    info_invt_para[INVT_ID_1].val  = info_buff.val;

    /* 执行一次下发变频器参数的操作 */
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
 功 能: 处理地暖预热参数下发
 其 它:
************************************************************************/
void SHC_set_floor_preheat_val_deal(U08 idx, I16 val)
{
	switch (idx)
	{
        case 0:     para_set(P_GaI(val_floor_preheat_temp),val);        break;	/* 【地暖预热温度】 */
        case 1:     para_set(P_GaI(val_floor_preheat_interval),val);    break;	/* 【地暖预热时长】 */
        case 2:     para_set(P_GaI(val_floor_preheat_max),val);         break;	/* 【地暖预热时长】 */
	}
}

/***********************************************************************
 功 能: 处理地暖定时参数下发
 其 它:
************************************************************************/
void SHC_set_floor_timer_val_deal(U08 idx, I16 val)
{
    switch (idx)
    {
        case 0:     para_set(P_GaI(val_floor_dry_head),val);   break;		/* 【地暖烘干前】 */
        case 1:     para_set(P_GaI(val_floor_dry_mid),val);    break;		/* 【地暖烘干中】 */
        case 2:     para_set(P_GaI(val_floor_dry_tail),val);   break;		/* 【地暖烘干尾】 */
        case 3:		para_set(P_GaI(val_floor_dry_temp),val);   break;		/* 【地暖烘干温度】 */
    }
}

/***********************************************************************
 功 能: 处理显示屏商检状态下发
 其 它:
************************************************************************/
void SHC_set_test_fac_hmi_info_deal(SH_TEST_FAC_HMI_STRU info)
{
	TF_wifi_sta = info.wr.wifi_sta;
}

/***********************************************************************
 功 能: 获取涂鸦信息
 其 它: 没有此功能则返回TRUE
************************************************************************/
#define SHC_get_tuya_info(dest_buf)            (TYB_upload_product_info((U08*)dest_buf))

/***********************************************************************
 功 能: 获取涂鸦数据
 其 它: 没有此功能则返回TRUE
************************************************************************/
#define SHC_get_tuya_data(dest_buf,data_addr)  (TYB_SLP_SH_upload_tuya_data((U08*)dest_buf, (U16)data_addr))

/***********************************************************************
 功 能: 处理涂鸦数据下发
 其 它: 没有此功能则返回TRUE
************************************************************************/
#define SHC_set_tuya_data(src_buf)             (TYB_parse_tuya_data((U08*)src_buf))


