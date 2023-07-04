/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: Tuya_B_Config.c
 文件描述: 涂鸦无线功能配置(后板部分)
 创建人  : 林浩彬
 创建日期: 2020.12.13
*******************************************************************************/
#define TUYA_B_CONFIG_GLOBAL
#include "includes.h"

const RN_ENUM_MAP TYBC_LANGUAGE_MAP[]=
{
    { SH_LANGUAGE_EN ,  RN_LANGUAGE_EN,  TY_MSGID_LANG_EN  },		/* 英语 */
    { SH_LANGUAGE_PL ,  RN_LANGUAGE_POL, TY_MSGID_LANG_POL },		/* 波兰语 */
    { SH_LANGUAGE_ES ,  RN_LANGUAGE_SP , TY_MSGID_LANG_ESP },		/* 西班牙语 */
    { SH_LANGUAGE_GR ,  RN_LANGUAGE_GR , TY_MSGID_LANG_GR  },		/* 希腊语 */
    { SH_LANGUAGE_FR ,  RN_LANGUAGE_FR , TY_MSGID_LANG_FR  },		/* 法语 */
    { SH_LANGUAGE_IT ,  RN_LANGUAGE_ITL, TY_MSGID_LANG_ITL },		/* 意大利语 */
    { SH_LANGUAGE_CN ,  RN_LANGUAGE_CN,  TY_MSGID_LANG_CHN },		/* 中文 */
    { SH_LANGUAGE_HG,   RN_LANGUAGE_HG,  TY_MSGID_LANG_HG  },		/* 匈牙利语 */
    { SH_LANGUAGE_GE ,  RN_LANGUAGE_GE,  TY_MSGID_LANG_GE  },		/* 德语 */
    { SH_LANGUAGE_CZ ,  RN_LANGUAGE_CZ,  TY_MSGID_NULL 	   },		/* 捷克 */
    { SH_LANGUAGE_PT ,  RN_LANGUAGE_PT,  TY_MSGID_NULL     },		/* 葡萄牙 */
    { SH_LANGUAGE_TR ,  RN_LANGUAGE_TR,  TY_MSGID_NULL     },		/* 土耳其 */
    { SH_LANGUAGE_NL ,  RN_LANGUAGE_NL,  TY_MSGID_NULL     },		/* 荷兰 */
    { SH_LANGUAGE_SE ,  RN_LANGUAGE_SE,  TY_MSGID_NULL     },		/* 瑞典 */
    { SH_LANGUAGE_DK ,  RN_LANGUAGE_DK,  TY_MSGID_NULL     },		/* 丹麦 */
    { SH_LANGUAGE_NO ,  RN_LANGUAGE_NO,  TY_MSGID_NULL     },		/* 挪威 */
};

const RN_ENUM_MAP TYBC_MODE_SET_MAP[] =
{
    { SET_AUTO,           RN_MODE_AUTO,      TY_MSGID_MODE_AUTO,      }, /* 0: RN_MODE_AUTO 自动 */
    { SET_COOL,           RN_MODE_COOL,      TY_MSGID_MODE_COOL,      }, /* 1: RN_MODE_COOL 制冷 */
    { SET_HEAT,           RN_MODE_HEAT,      TY_MSGID_MODE_HEAT,      }, /* 2: RN_MODE_HEAT 制热 */
};

const RN_ENUM_MAP TYBC_RUN_STATUS_MAP[] =
{
    { _IDLE,   RN_RUN_STATUS_IDLE,    TY_MSGID_STA_READY,   }, /* 0: RN_RUN_STATUS_IDLE 准备就绪 */
    { _READY,  RN_RUN_STATUS_LAUNCH,  TY_MSGID_STA_ON,      }, /* 1: RN_RUN_STATUS_LAUNCH 正在启动 */
    { _RUN,    RN_RUN_STATUS_RUN,     TY_MSGID_STA_RUNNING, }, /* 2: RN_RUN_STATUS_RUN 运行中 */
    { _DELAY,  RN_RUN_STATUS_STOP,    TY_MSGID_STA_OFF,     }, /* 3: RN_RUN_STATUS_STOP 正在停止 */
    { _ALARM,  RN_RUN_STATUS_ALERT,   TY_MSGID_STA_ALERT,   }, /* 4: RN_RUN_STATUS_ALERT 故障告警 */
};

/*******************************************************************************
 函 数: TYBC_language_to_rn_enum()
 功 能: 将本地语言枚举值进行转换为RN面板语言枚举值
 参 数: local_language--本地语言枚举值;
 返 回: 面板语言枚举值
 其 它: 空
*******************************************************************************/
U08 TYBC_language_to_rn_enum(U08 local_language)
{
    U08 i;

    for (i = 0; i < ARRELE(TYBC_LANGUAGE_MAP); i++)
    {
        if (local_language == TYBC_LANGUAGE_MAP[i].local_enum)
        {
            return TYBC_LANGUAGE_MAP[i].rn_enum;
        }
    }
    return RN_LANGUAGE_CN;
}

/*******************************************************************************
 函 数: TYBC_language_to_local_enum()
 功 能: 将本地语言枚举值进行转换为RN面板语言枚举值
 参 数: rn_language--本地语言枚举值;
 返 回: 面板语言枚举值
 其 它: 空
*******************************************************************************/
U08 TYBC_language_to_local_enum(U08 rn_language)
{
    U08 i;

    for (i = 0; i < ARRELE(TYBC_LANGUAGE_MAP); i++)
    {
        if (rn_language == TYBC_LANGUAGE_MAP[i].rn_enum)
        {
            return TYBC_LANGUAGE_MAP[i].local_enum;
        }
    }
    return SH_LANGUAGE_CN;
}

/* 有"语言设置" */
#ifdef TUYA_USE_LANG_SET
/*******************************************************************************
 函 数: TYBC_get_language_msg()
 功 能: 通过本地语言枚举值获取相应的显示字符串
 参 数: local_language--;
 返 回: 面板语言显示字符串号
 其 它: 空
*******************************************************************************/
static U16 TYBC_get_language_msg(U08 local_language)
{
    U08 i;

    for (i = 0; i < ARRELE(TYBC_LANGUAGE_MAP); i++)
    {
        if (local_language == TYBC_LANGUAGE_MAP[i].local_enum)
        {
            return TYBC_LANGUAGE_MAP[i].msg_id;
        }
    }
    return TY_MSGID_NULL;
}
#endif

/* 有"厂家密码" */
#ifdef TUYA_USE_FAC_PWD
/*******************************************************************************
 函 数: TYBC_get_password_pointer()
 功 能: 此处将厂家密码统一转换为U08*, 进而避免大小端问题。请根据实际情况对此函数
        进行修改!!!
 参 数: 空
 返 回: pwd--厂家密码指针(U08*)
 其 它: 空
*******************************************************************************/
U08 *TYBC_get_password_pointer(void)
{
    U08 i;
    static U08 pwd[MAX_PASS_BIT];

    ARRCLR(pwd);

    for (i = 0; i < MAX_PASS_WORD; i++)
    {
        pwd[i] = password[PASS_FAC][i];
    }

    return pwd;
}

/*******************************************************************************
 函 数: TYBC_check_password_modified()
 功 能: 检查厂家密码是否修改。请根据实际情况对此函数进行修改! 注意不同平台需要留
        意实际的password维度和大小，以免超界
 参 数: 空
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL TYBC_check_password_modified(void)
{
    U08 i;
    BOOL fg_pwd_updated = FALSE;
    static U08 pwd_prev[MAX_PASS_BIT] = {0};

    for (i = 0; i < MAX_PASS_BIT; i++)
    {
        if (pwd_prev[i] != TYBC_PASSWORD_POINTER[i])
        {
            fg_pwd_updated = TRUE;
        }
    }

    if (fg_pwd_updated)
    {
        MEMCPY(pwd_prev, TYBC_PASSWORD_POINTER, MAX_PASS_BIT);
    }

    return fg_pwd_updated;
}
#endif

/* 有"语言功能" */
#ifdef TUYA_USE_LANG_SET
/*******************************************************************************
 函 数: TYBC_get_language_range()
 功 能: 获取语言选项字符串号数组指针
 参 数: 空
 返 回: 字符串号数组指针
 其 它: 空
*******************************************************************************/
U16 *TYBC_get_language_range(void)
{
    U08 i, rn_enum;
    U16 rn_msg;
    static U16 MSG_RN_LANGAUGE_U[RN_LANGUAGE_MAX];

    /* 标志、可用模式初始化 */
    ARRSET(MSG_RN_LANGAUGE_U, TY_MSGID_NULL);

    /* 调整语言可选项 */
    for (i = 0; i < RN_LANGUAGE_MAX; i++)
    {
        rn_enum = TYBC_language_to_rn_enum(i);
        rn_msg = TYBC_get_language_msg(i);
        if (rn_msg != TY_MSGID_NULL)
        {
            MSG_RN_LANGAUGE_U[rn_enum] = rn_msg;
        }
    }

    return MSG_RN_LANGAUGE_U;
}

/*******************************************************************************
 函 数: TYBC_language_set()
 功 能: 设置控制器语言并写入EEPROM
 参 数: val--语言设定值;
 返 回: 空
 其 它: 空
*******************************************************************************/
void TYBC_language_set(U08 val)
{
    if (val > 0
    && val <= RN_LANGUAGE_MAX
    )
    {
        sh_language = TYBC_language_to_local_enum(val);
        DataAccess_Language(DATA_WRITE);
    }
}
#endif

#ifdef TUYA_USE_TEMP_UNIT_SET
/*******************************************************************************
 函 数: TYBC_get_temp_unit_range()
 功 能:
 参 数: 空
 返 回:
 其 它: 空
*******************************************************************************/
U16 *TYBC_get_temp_unit_range(void)
{
	static U16 MSG_RN_TEMP_UNIT_U[RN_TEMP_UNIT_MAX];

	MSG_RN_TEMP_UNIT_U[RN_TEMP_UNIT_C] = TY_MSGID_TEMP_UNIT_C;
	MSG_RN_TEMP_UNIT_U[RN_TEMP_UNIT_F] = TY_MSGID_TEMP_UNIT_F;

    return MSG_RN_TEMP_UNIT_U;
}
#endif

/*******************************************************************************
 函 数: TYBC_get_switch_status()
 功 能: 获取机组实际开启/关闭状态
 参 数: 空
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL TYBC_get_switch_status(void)
{
    BOOL fg_switch_on = FALSE;

    if (status_run == _READY
        || status_run == _RUN)
    {
        fg_switch_on = TRUE;
    }

    return fg_switch_on;
}

/*******************************************************************************
 函 数: TYBC_mode_set_to_rn_enum()
 功 能: 将本地设定模式枚举值进行转换为RN面板设定模式枚举值
 参 数: local_mode--本地模式枚举值;
 返 回: 面板模式枚举值
 其 它: 空
*******************************************************************************/
U08 TYBC_mode_set_to_rn_enum(U08 local_mode)
{
    U08 i;

    for (i = 0; i < ARRELE(TYBC_MODE_SET_MAP); i++)
    {
        if (local_mode == TYBC_MODE_SET_MAP[i].local_enum)
        {
            return TYBC_MODE_SET_MAP[i].rn_enum;
        }
    }
    return RN_MODE_AUTO;
}

/*******************************************************************************
 函 数: TYBC_get_mode_set_msg()
 功 能: 通过本地设定模式枚举值获取相应的显示字符串
 参 数: local_mode--本地模式枚举值;
 返 回: 面板模式显示字符串号
 其 它: 空
*******************************************************************************/
U16 TYBC_get_mode_set_msg(U08 local_mode)
{
    U08 i;

    for (i = 0; i < ARRELE(TYBC_MODE_SET_MAP); i++)
    {
        if (local_mode == TYBC_MODE_SET_MAP[i].local_enum)
        {
            return TYBC_MODE_SET_MAP[i].msg_id;
        }
    }
    return msg_tyb_null;
}

/*******************************************************************************
 函 数: TYBC_get_mode_set_range()
 功 能: 获取设定模式选项字符串号数组指针
 参 数: 空
 返 回: 字符串号数组指针
 其 它: 空
*******************************************************************************/
U16 *TYBC_get_mode_set_range(void)
{
    U08 i, rn_enum;
    U16 rn_msg;
    I16 max, min;
    static U16 MSG_RN_MODE_SET_U[RN_MODE_MAX];

    /* 标志、可用模式初始化 */
    ARRSET(MSG_RN_MODE_SET_U, msg_tyb_null);

    /* 获取最大值、最小值 */
    max = para_get_max(P_GaI(sv_mode));
    min = para_get_min(P_GaI(sv_mode));

    /* 调整可用模式选项 */
    for (i = 0; i < SET_MODE_MAX; i++)
    {
        if (i >= min && i <= max)
        {
            rn_enum = TYBC_mode_set_to_rn_enum(i);
            rn_msg = TYBC_get_mode_set_msg(i);
            if (rn_msg != msg_tyb_null)
            {
                MSG_RN_MODE_SET_U[rn_enum] = rn_msg;
            }
        }
    }

    return MSG_RN_MODE_SET_U;
}

/*******************************************************************************
 函 数: TYBC_run_status_to_rn_enum()
 功 能: 将本地运行状态枚举值进行转换为APP面板运行状态枚举值
 参 数: local_run_status--本地运行状态枚举值;
 返 回: 空
 其 它: 请参考log_sys_public_info()中的做法
*******************************************************************************/
U08 TYBC_run_status_to_rn_enum(U08 local_enum)
{
    U08 i;

    /* 机组除霜 */
    /* 处理防冻 */
    if (fg_idle_sta_antifreeze
    || fg_run_sta_ac_antifreeze
    || fg_run_sta_hotw_antifreeze
    )
    {
        return RN_RUN_STATUS_ANTIFREEZE;
    }
    if(log_sys_defrost_comp_num(SUB_SYS_TYPE_MAX))
    {
        return RN_RUN_STATUS_DEFROST;
    }

    /* 处理预热  */
    if (!fg_warm_complete)
    {
        return RN_RUN_STATUS_PREHEAT;
    }

    /* 处理run_status */
    for (i = 0; i < ARRELE(TYBC_RUN_STATUS_MAP); i++)
    {
        if (local_enum == TYBC_RUN_STATUS_MAP[i].local_enum)
        {
            return TYBC_RUN_STATUS_MAP[i].rn_enum;
        }
    }
    return RN_RUN_STATUS_IDLE;
}

/*******************************************************************************
 函 数: TYBC_temp_set()
 功 能: 设置温度
 参 数: cloud_val--云端下发的温度设定值;
 返 回: 空
 其 它: 空
*******************************************************************************/
void TYBC_temp_set(I16 cloud_val, U08 dpid)
{
	switch (dpid)
	{
		case 16:  			//DPID_TEMP_SET
		case 107:			//DPID_ZONEA_FUNC_CMD
			SHC_set_tmp_zone_1_deal(cloud_val);
			break;
		case 109:			//DPID_ZONEB_FUNC_CMD
			SHC_set_tmp_zone_2_deal(cloud_val);
			break;
		case 111:			//DPID_DHW_FUNC_CMD
			SHC_set_tmp_zone_hotw_deal(cloud_val);
			break;
	}
}

/*******************************************************************************
 函 数: TYBC_mode_set()
 功 能: 设置模式
 参 数: cloud_val--云端下发的模式设定值;
 返 回: 空
 其 它: 空
*******************************************************************************/
void TYBC_mode_set(I16 cloud_val)
{
    I16 max;
    I16 min;
    I16 val;


    /* 不可切换模式 */
    if (SHC_ac_mode_switch_disabled())
    {
        return;
    }

    max = para_get_max(P_GaI(sv_mode));
    min = para_get_min(P_GaI(sv_mode));

    switch (cloud_val)
    {
        case RN_MODE_AUTO:
            val = SET_AUTO;
            break;
        case RN_MODE_COOL:
            val = SET_COOL;
            break;
        case RN_MODE_HEAT:
            val = SET_HEAT;
            break;
        case RN_MODE_HOTW:
//            val = SET_HOT_WATER;
//            break;
        case RN_MODE_COOL_HOTW:
//            val = SET_COOL_HOT_WATER;
//            break;
        case RN_MODE_HEAT_HOTW:
//            val = SET_HEAT_HOT_WATER;
//            break;
        case RN_MODE_AUTO_HOTW:
//            val = SET_AUTO_HOT_WATER;
//            break;
        case RN_MODE_FLOOR:
        case RN_MODE_VENT:
        default:
            return;
    }

    if (val >= min && val <= max)
    {
        para_set(P_GaI(sv_mode), val);
    }
}

/* 有"风速功能" */
#ifdef TUYA_USE_WIND
/*******************************************************************************
 函 数: TYBC_get_wind_set_range()
 功 能: 获取设定风速选项字符串号数组指针
 参 数: 空
 返 回: 字符串号数组指针
 其 它: 空
*******************************************************************************/
U16 *TYBC_get_wind_set_range(void)
{
    /* 个数必须为RN_WIND_MAX，顺序按照RN_WIND_XXX排列，不使用或不存在的模式用空格表示 */
    static U16 MSG_RN_WIND_SET[RN_WIND_MAX] =
    {
        ty_enum_wind_low,   /* 0: RN_WIND_LOW, 低风速 */
        ty_enum_wind_mid,   /* 1: RN_WIND_MID, 中风速 */
        ty_enum_wind_high,  /* 2: RN_WIND_HIGH, 高风速 */
        ty_enum_wind_auto,  /* 3: RN_WIND_AUTO, 自动 */
        msg_tyb_null,        /* 4: RN_WIND_SET_MAX, 最大可设风速数(无) */
        msg_tyb_null,        /* 5: RN_WIND_NULL, 无风速 */
    };

    /* 若有根据厂家模式禁用某些模式，在此处将对应的模式禁用(改为空格) */

    return MSG_RN_WIND_SET;
}

/*******************************************************************************
 函 数: TYBC_wind_set_to_rn_enum()
 功 能: 将本地设定风速枚举值进行转换为RN面板设定风速枚举值
 参 数: local_wind_speed--本地设定风速枚举值;
 返 回: 面板设定风速枚举值
 其 它: 空
*******************************************************************************/
U08 TYBC_wind_set_to_rn_enum(U08 local_wind_speed)
{
    RN_ENUM_MAP local_to_rn[RN_WIND_MAX]=
    {
        { 0,      RN_WIND_LOW     },  /* 0: 低风速 */
        { 1,      RN_WIND_MID     },  /* 1: 中风速 */
        { 2,      RN_WIND_HIGH    },  /* 2: 高风速 */
        { 3,      RN_WIND_AUTO    },  /* 3: 自动 */
        { 4,      RN_WIND_SET_MAX },  /* 4: 最大可设风速数 */
        { 5,      RN_WIND_NULL    },  /* 5: 无风速 */
    };
    U08 i;

    for (i = 0; i < RN_WIND_MAX; i++)
    {
        if (local_wind_speed == local_to_rn[i].local_enum)
        {
            return local_to_rn[i].rn_enum;
        }
    }
    return RN_WIND_NULL;
}

/*******************************************************************************
 函 数: TYBC_get_wind_cur_range()
 功 能: 获取当前风速选项字符串号数组指针
 参 数: 空
 返 回: 字符串号数组指针
 其 它: 空
*******************************************************************************/
U16 *TYBC_get_wind_cur_range(void)
{
    /* 个数必须为RN_WIND_MAX，顺序按照TY_RN_WIND排列，不使用的风速用空格表示 */
    static U16 MSG_RN_WIND_CUR[RN_WIND_MAX] =
    {
        ty_enum_wind_low,   /* 0: RN_WIND_LOW, 低风速 */
        ty_enum_wind_mid,   /* 1: RN_WIND_MID, 中风速 */
        ty_enum_wind_high,  /* 2: RN_WIND_HIGH, 高风速 */
        msg_tyb_null,        /* 3: RN_WIND_AUTO, 自动 */
        msg_tyb_null,        /* 4: RN_WIND_SET_MAX, 最大可设风速数(无) */
        ty_enum_wind_null,  /* 5: RN_WIND_NULL, 无风速 */
    };

    return MSG_RN_WIND_CUR;
}

/*******************************************************************************
 函 数: TYBC_wind_cur_to_rn_enum()
 功 能: 将本地当前风速枚举值进行转换为RN面板当前风速枚举值
 参 数: local_wind_speed--本地当前风速枚举值 ;
 返 回: 面板当前风速枚举值
 其 它: 空
*******************************************************************************/
U08 TYBC_wind_cur_to_rn_enum(U08 local_wind_speed)
{
    RN_ENUM_MAP local_to_rn[RN_WIND_MAX]=
    {
        { 0,      RN_WIND_LOW     },  /* 0: 低风速 */
        { 1,      RN_WIND_MID     },  /* 1: 中风速 */
        { 2,      RN_WIND_HIGH    },  /* 2: 高风速 */
        { 3,      RN_WIND_AUTO    },  /* 3: 自动 */
        { 4,      RN_WIND_SET_MAX },  /* 4: 最大可设风速数 */
        { 5,      RN_WIND_NULL    },  /* 5: 无风速 */
    };
    U08 i;

    for (i = 0; i < RN_WIND_MAX; i++)
    {
        if (local_wind_speed == local_to_rn[i].local_enum)
        {
            return local_to_rn[i].rn_enum;
        }
    }
    return RN_WIND_NULL;
}
#endif

/* 有"风速功能" */
#ifdef TUYA_USE_WIND
/*******************************************************************************
 函 数: TYBC_wind_set()
 功 能: 设置风速
 参 数: cloud_val--云端下发的风速设定值;
 返 回: 空
 其 它: 空
*******************************************************************************/
void TYBC_wind_set(I16 cloud_val)
{
    ;
}
#endif

/* 有"湿度功能" */
#ifdef TUYA_USE_HUMI
/*******************************************************************************
 函 数: TYBC_humi_set()
 功 能: 设置湿度
 参 数: cloud_val--云端下发的湿度设定值;
 返 回: 空
 其 它: 空
*******************************************************************************/
void TYBC_humi_set(I16 cloud_val)
{
    ;
}
#endif

/* 有"热水功能" */
#ifdef TUYA_USE_HOT_WATER
/*******************************************************************************
 函 数: TYBC_hot_water_set()
 功 能: 设置热水温度
 参 数: cloud_val--云端下发的热水温度设定值;
 返 回: 空
 其 它: 空
*******************************************************************************/
void TYBC_hot_water_set(I16 cloud_val)
{
    U08 item_id = 0xFF;
    U08 grp_id = 0xFF;
    I16 max;
    I16 min;

    grp_id = P_grp(sv_hotw);
    item_id = P_idx(sv_hotw);

    if (item_id != 0xFF)
    {
        /* 获取对应温度的最大值、最小值 */
        max = para_get_max(grp_id, item_id);
        min = para_get_min(grp_id, item_id);

        /* 设定温度 */
        if (cloud_val >= min && cloud_val <= max)
        {
            para_set(grp_id, item_id, cloud_val);
        }
    }
}
#endif

/* 有"计时配置" */
#ifdef TUYA_USE_CONFIG_TIMER
void TYBC_config_timer(void)
{
    ;
}
#endif

/* 有"初始化配置" */
#ifdef TUYA_USE_CONFIG_INIT
void TYBC_config_init(void)
{
    TYBC_CMD_SPLASH_ON;
}
#endif

#ifndef TUYA_USE_HMI_SELF
/* 只支持从机屏 */
#ifndef TUYA_USE_HMI_MASTER
/*******************************************************************************
 函 数: TYBC_sign_local2sh()
 功 能: 本地sign属性转换为SH sign属性(SmartHmi中同名函数的副本)
 参 数: sign_local--本地属性;
 返 回: SH属性
 其 它: 空
*******************************************************************************/
U16 TYBC_sign_local2sh(U16 sign_local)
{
    U16 sign_sh = 0;        /* 转换后属性 */
    U16 unit_local = 0;     /* 原始单位 */
    U16 unit_sh = 0;        /* 转换后单位 */
    U16 decimal_local = 0;  /* 原始小数位 */
    U16 decimal_sh = 0;     /* 转换后小数位 */
    U16 fg_is_hide = 0;     /* 参数隐藏 */
    U16 fg_is_const = 0;    /* 参数不可更改 */
    U16 fg_is_bit = 0;      /* 参数为位变量 */
    U16 auth_local = 0;     /* 原始权限 */
    U16 auth_sh = 0;        /* 转换后权限 */

    /* 单位 */
    unit_local = sign_local & UINT_MASK;
    switch (unit_local)
    {
        case UINT_0:
            unit_sh = TYBC_UNIT_NULL;         /* 无 */
            break;
        case UINT_C:
            unit_sh = TYBC_UNIT_TMP_C;        /* 温度(℃) */
            break;
        case UINT_A:
            unit_sh = TYBC_UNIT_A;            /* 电流(A) */
            break;
        case UINT_PR:
            unit_sh = TYBC_UNIT_PRS_B;        /* 压力(bar) */
            break;
        case UINT_PE:
            unit_sh = TYBC_UNIT_PE;           /* 百分比(%) */
            break;
        case UINT_F:
            unit_sh = TYBC_UNIT_HZ;           /* 频率(HZ) */
            break;
        case UINT_RS:
            unit_sh = TYBC_UNIT_RPS;          /* 转速(rps) */
            break;
        case UINT_RM:
            unit_sh = TYBC_UNIT_RPM;          /* 转速(rpm) */
            break;
        case UINT_S:
            unit_sh = TYBC_UNIT_SECOND;       /* 秒 */
            break;
        case UINT_M:
            unit_sh = TYBC_UNIT_MINT;         /* 分 */
            break;
        case UINT_H:
            unit_sh = TYBC_UNIT_HOUR;         /* 时 */
            break;
        case UINT_D:
            unit_sh = TYBC_UNIT_DAY;          /* 日 */
            break;
        case UINT_ST:
            unit_sh = TYBC_UNIT_ST;           /* 步 */
            break;
        case UINT_V:
            unit_sh = TYBC_UNIT_V;            /* V */
            break;
        default:
            unit_sh = TYBC_UNIT_NULL;         /* 无 */
            break;
    }

    /* 小数位 */
    decimal_local= sign_local & D_DOTMASK;
    switch (decimal_local)
    {
        case D_DOT1:
            decimal_sh = 1;
            break;
        case D_DOT2:
            decimal_sh = 2;
            break;
        case D_DOT3:
            decimal_sh = 3;
            break;
        default:
            decimal_sh = 0;
            break;
    }

    /* 权限 */
    auth_local = sign_local & D_SAFEMASK;
    switch (auth_local)
    {
        case D_PRO:
            auth_sh = TYBC_PARA_PRO;
            break;
        case D_SER:
            auth_sh = TYBC_PARA_SER;
            break;
        case D_FAC:
            auth_sh = TYBC_PARA_FAC;
            break;
        case D_USER:
        default:
            auth_sh = TYBC_PARA_USR;
            break;
    }
    /* 是否隐藏 */
    if (sign_local & (D_HIDE))
    {
        fg_is_hide = TRUE;
    }
    else
    {
        fg_is_hide = FALSE;
    }

    /* 是否不可改 */
    if (sign_local & (D_NEXCH))
    {
        fg_is_const = TRUE;
    }
    else
    {
        fg_is_const = FALSE;
    }

    /* 是否为位变量 */
    if (sign_local & (D_BIT))
    {
        fg_is_bit = TRUE;
    }
    else
    {
        fg_is_bit = FALSE;
    }

    /* 合成完整属性 */
    sign_sh = (unit_sh << 0)
            | (decimal_sh << TYBC_DECIMAL_BGN_BIT)
            | (fg_is_hide << TYBC_HIDE_BGN_BIT)
            | (fg_is_const << TYBC_CONST_BGN_BIT)
            | (auth_sh << TYBC_SAFE_BGN_BIT)
            | (fg_is_bit << TYBC_BIT_BGN_BIT);

    return sign_sh;
}

/* 有"当前故障" */
#ifdef TUYA_USE_ERR_CUR
/*******************************************************************************
 函 数: TYBC_get_err_cur()
 功 能: 故障查询使用到的故障名称字符串(源自SmartHmi_config.c)
 参 数: err--故障信息;
        sys_err_id--系统故障号;
        mod_id--模块号;
        mod_err_id--模块故障号;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL TYBC_get_err_cur(TYBC_ERR_CUR *err, U16 *sys_err_id, U16 *mod_id, U16 *mod_err_id)
{
    BOOL fg_err = FALSE;

    /* 系统故障 */
    for (; *sys_err_id<TYBC_NO_ERR; (*sys_err_id)++)
    {
        if (fg_err)  break;

        if (m_get_error(MAX_PCB, *sys_err_id))
        {
            fg_err = TRUE;
            err->name = TYBC_GET_M_ERR(*sys_err_id);
            err->id = MAKEWORD(TYBC_NO_ID, TYBC_NO_ID);
            err->err_id = *sys_err_id;
        }
    }

    if (!fg_err)
    {
        /* 单元故障 */
        for (; *mod_id<MAX_PCB; *mod_err_id=0,(*mod_id)++)
        {
            for (; *mod_err_id<TYBC_NO_ERR; (*mod_err_id)++)
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
                    err->id = MAKEWORD(*mod_id, TYBC_NO_ID);
                    err->err_id = *mod_err_id;
                }
            }
            /* 放在for循环的结尾，防止c加1,导致遗漏本模块的其他故障 */
            if (fg_err)  break;
        }
    }

    return fg_err;
}

/*******************************************************************************
 函 数: TYBC_get_err_cur_num()
 功 能: 当前故障个数
 参 数: 空
 返 回: 当前故障个数
 其 它: 空
*******************************************************************************/
U16 TYBC_get_err_cur_num(void)
{
    U16 i = 0;
    U16 j = 0;
    U16 k = 0;
    U16 temp = 0;

    for (i = 0; i < val_pcb_num; i++)           /* 统计模块故障 */
    {
        for (j = 0; j < MAX_ERR_WORD; j++)
        {
            for (k = 0; k < 16; k++)
            {
                if (GETBIT(cell[i].cell_error[j], k))
                {
                    temp++;
                }
            }
        }
    }
    for (j = 0; j < MAX_ERR_WORD; j++)          /* 统计系统故障 */
    {
        for (k = 0; k < 16; k++)
        {
            if (GETBIT(cell[MAX_PCB].cell_error[j], k))
            {
                temp++;
            }
        }
    }

    return temp;
}
#endif

/* 有"历史故障" */
#ifdef TUYA_USE_ERR_HIS
/*******************************************************************************
 函 数: TYBC_get_err_his_info()
 功 能: 历史故障信息(源自SmartHmi_config.c)
 参 数: err--历史故障信息;
        idx--索引号;
 返 回: 空
 其 它: 空
*******************************************************************************/
void TYBC_get_err_his_info(TYBC_ERR_HIS *err, U08 idx)
{
    U08 i;
    U08 err_id = 0;
    U08 err_pcb = 0;
    U08 sign = 0;
    U08 his_err_one[TYBC_HIS_ERR_TBL_SIZE];

    /* 获取历史故障 */
    for (i=0; i<TYBC_HIS_ERR_TBL_SIZE; i++)
    {
        his_err_one[i] = his_err_tbl[idx*TYBC_HIS_ERR_TBL_SIZE+i];
    }

    /* 故障名、板号、故障号、故障时间 */
    err_id = TYBC_NO_ERR;
    if (idx < TYBC_HIS_ERR_MAX)
    {
        err_id  = his_err_one[0];
        err_pcb = his_err_one[1];
        sign    = his_err_one[2];
    }
    if (err_id < TYBC_NO_ERR)
    {
        if (err_pcb == TYBC_PCB_MAX)         /* 系统故障 */
        {
            err->name = TYBC_GET_M_ERR(err_id);
            err->id = MAKEWORD(TYBC_NO_ID, TYBC_NO_ID);
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
                else if(err_id == S_ERR_PUMPF)
                {
                    if (sign > PUMF_NO_ERR && sign < PUMPF_ERR_MAX)
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
                    err->name = msg_invt_error_unknown;
                }
            }
            else
            {
                err->name = SHC_NAME_S_ERR(err_id);
            }

            err->id = MAKEWORD(err_pcb, TYBC_NO_ID);
            err->err_id = err_id;
        }
        err->time.month  = his_err_one[3];
        err->time.day    = his_err_one[4];
        err->time.hour   = his_err_one[5];
        err->time.mint   = his_err_one[6];
    }
    else
    {
        err->name = TYBC_ERR_WEI_ZHI_GZ;  /* 名为"未知故障"则不显示 */
        err->id = 0;
        err->err_id = 0;
        err->time.month = 0;
        err->time.day = 0;
        err->time.hour = 0;
        err->time.mint= 0;
    }
}
#endif

/* 有"参数设置" */
#ifdef TUYA_USE_PARAM_SET
/*******************************************************************************
 函 数: TYBC_get_para_grp_sign()
 功 能: 获取参数组属性(源自SmartHmi_config.c)
 参 数: grp--参数组;
 返 回: 参数组属性
 其 它: 空
*******************************************************************************/
U16 TYBC_get_para_grp_sign(U08 grp)
{
    extern PARAMITEM  ParamPri[];
    U16 sign = 0;

    sign = ParamPri[grp].sign;

    return sign;
}


/*******************************************************************************
 函 数: TYBC_cal_grp_attrib()
 功 能: 将本地属性格式转换为SH属性格式(SmartHmi中同名函数的副本)
 参 数: grp--参数组;
 返 回: 参数组属性
 其 它: 参数组属性有:权限(使用单位的掩码)、是否隐藏
*******************************************************************************/
U16 TYBC_cal_grp_attrib(U08 grp)
{
    U16 sign_local = 0;     /* 原始属性 */
    U16 sign_sh = 0;        /* 转换后属性 */
    U16 authority_local = 0;/* 原始权限 */
    U16 authority_sh = 0;   /* 转换后权限 */
    U16 fg_is_hide = 0;     /* 参数隐藏 */

    /* 读取属性 */
    sign_local = TYBC_get_para_grp_sign(grp);

    /* 权限 */
    authority_local = sign_local & D_SAFEMASK;
    switch (authority_local)
    {
        case D_USER:
            authority_sh = TYBC_PWR_USER;
            break;
        case D_PRO:
            authority_sh = TYBC_PWR_PRO;
            break;
        case D_SER:
            authority_sh = TYBC_PWR_SER;
            break;
        case D_FAC:
            authority_sh = TYBC_PWR_FAC;
            break;
        default:
            authority_sh = TYBC_PWR_NONE;
            break;
    }

    /* 是否隐藏 */
    if (sign_local & (D_HIDE))
    {
        fg_is_hide = TRUE;
    }
    else
    {
        fg_is_hide = FALSE;
    }

    /* 合成完整属性 */
    sign_sh = (authority_sh << 1)
            | (fg_is_hide << 12);

    return sign_sh;
}
#endif

/*******************************************************************************
 函 数: TYBC_get_dict_idx()
 功 能: 获取字典序号(源自SmartHmi_config.c)
 参 数: pmsg--字典指针;
 返 回: 字典序号
 其 它: 空
*******************************************************************************/
U16 TYBC_get_dict_idx(const U16 *pmsg)
{
    U16 i;

    if (pmsg != NULL)
    {
        for (i = 0; i < TYBC_PARAM_DICT_NUM; i++)
        {
            if(para_dict[i].pmsg == pmsg)
            {
                return i;
            }
        }
    }

    return 0;
}

/*******************************************************************************
 函 数: TYBC_attr_local2sh()
 功 能: 本地属性转换为SH属性,增加字典信息(SmartHmi中同名函数的副本)
 参 数: sign_local--本地属性;
        dic_str--字典;
 返 回: SH属性
 其 它: 空
*******************************************************************************/
U16 TYBC_attr_local2sh(U16 sign_local, const U16 *dic_str)
{
    U16 sign_sh = 0;
    U16 dict_sh = 0;        /* 转换后单位 */
    U16 dict_idx = 0;       /* 参数值字典序号 */

    sign_sh |= TYBC_sign_local2sh(sign_local);

    if (dic_str != NULL)    /* 字符串型:加上字典 */
    {
        sign_sh &= ~(TYBC_UNIT|TYBC_DECIMAL);
        dict_idx = TYBC_get_dict_idx(dic_str);/* 字典序号 */
        dict_sh = dict_idx + TYBC_UNIT_OFFSET;   /* 加上偏移 */
        sign_sh |= dict_sh;
    }

    return sign_sh;
}

/*******************************************************************************
 函 数: TYBC_cal_para_attrib()
 功 能: 将本地属性格式转换为SH属性格式(源自SmartHmi.c)
 参 数: grp--参数组;
        item--参数项;
 返 回: 参数项属性
 其 它: 参数组属性有:是否为字符串型、单位、是否隐藏、是否可改
*******************************************************************************/
U16 TYBC_cal_para_attrib(U08 grp, U08 item)
{
    U16 sign_local = 0;     /* 原始属性 */
    U16 sign_sh = 0;        /* 转换后属性 */

    /* 读取原始属性 */
    sign_local = para_get_sign(grp, item);

    sign_sh |= TYBC_attr_local2sh(sign_local, para_small[grp][item].pmsg);

    return sign_sh;
}
#endif
#endif

#ifdef TUYA_HAS_STAT
/*******************************************************************************
 函 数: TYBC_get_pv_val()
 功 能:
 参 数: pcb_idx--;
		pv_idx--;
 返 回:
 其 它: 空
*******************************************************************************/
I16 TYBC_get_pv_val(U16 pcb_idx, U16 pv_idx)
{
	I16 value = cell[pcb_idx].cell_pv[pv_idx];
	U16 attrib = 0;

	if (TYBC_PCB_MAX == pcb_idx)
	{
		attrib = msg_sys_p[(pv_idx)].attr;
	}
	else
	{
		if (pv_idx < TYBC_UNIT_PV_BEGIN)
		{
			attrib = msg_uint_p_comp[pv_idx].attr;
		}
		else
		{
			attrib = msg_uint_p_cell[(pv_idx-TYBC_UNIT_PV_BEGIN)].attr;
		}
	}

	if (TEMP_IS_FARA && attrib != 0xFFFF)
	{
		value = C_To_F(value, attrib);
	}

	return value;
}
#endif

