/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: SmartHmi.c
 文件描述: 处理与4.3寸触摸屏的通信
 创建人  : lhb
 创建日期: 2021.05.08
*******************************************************************************/
#define SMARTHMI_GLOBAL
#include "includes.h"
#include "SmartHmi_commu_info.h"
#include "SmartHmi_config.c"


/* 最大允许一次读取的字节数(根据实际通讯缓存更改) */
#define MAX_TRAS_BYTES      (MODBUS_BUF_SIZE-4)

/* MODBUS异常码(无需更改) */
#define SH_MB_NO_ERR                0       /* MODBUS无异常 */
#define SH_MB_ILLEGAL_FUNCTION      1       /* 非法功能 */
#define SH_MB_ILLEGAL_DATA_ADDR     2       /* 非法数据地址 */
#define SH_MB_ILLEGAL_DATA_VALUE    3       /* 非法数据值 */
#define SH_MB_SLAVE_DEVICE_FALUE    4       /* 从机设备故障，从机无法处理 */

//SH_CHK_TEST test_data;

static BOOL fg_pass_big_endian = TRUE;     /* HL051A/HL055A系列默认值为FALSE，HL040A默认值为TRUE */

/*****************************************************************************
函  数 : SH_sign_local2sh()
功  能 : 本地sign属性转换为SH sign属性
参  数 : U16 sign_local :
返  回 : 无
其  他 : 无
*****************************************************************************/
U16 SH_sign_local2sh(U16 sign_local)
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
            unit_sh = SH_UNIT_NULL;         /* 无 */
            break;
        case UINT_C:
            unit_sh = (TEMP_IS_FARA) ? (SH_UNIT_TMP_F) : (SH_UNIT_TMP_C);        /* 温度(℃) */
            break;
        case UINT_A:
            unit_sh = SH_UNIT_A;            /* 电流(A) */
            break;
        case UINT_PR:
            unit_sh = SH_UNIT_PRS_B;        /* 压力(bar) */
            break;
        case UINT_PE:
            unit_sh = SH_UNIT_PE;           /* 百分比(%) */
            break;
        case UINT_F:
            unit_sh = SH_UNIT_HZ;           /* 频率(HZ) */
            break;
        case UINT_RS:
            unit_sh = SH_UNIT_RPS;          /* 转速(rps) */
            break;
        case UINT_RM:
            unit_sh = SH_UNIT_RPM;          /* 转速(rpm) */
            break;
        case UINT_S:
            unit_sh = SH_UNIT_SECOND;       /* 秒 */
            break;
        case UINT_M:
            unit_sh = SH_UNIT_MINT;         /* 分 */
            break;
        case UINT_H:
            unit_sh = SH_UNIT_HOUR;         /* 时 */
            break;
        case UINT_D:
            unit_sh = SH_UNIT_DAY;          /* 日 */
            break;
        case UINT_ST:
            unit_sh = SH_UNIT_ST;           /* 步 */
            break;
        case UINT_V:
            unit_sh = SH_UNIT_V;            /* V */
            break;
        case UINT_LH:
            unit_sh = SH_UNIT_LH;            /* L/H */
            break;
        default:
            unit_sh = SH_UNIT_NULL;         /* 无 */
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
            auth_sh = SH_PARA_PRO;
            break;
        case D_SER:
            auth_sh = SH_PARA_SER;
            break;
        case D_FAC:
            auth_sh = SH_PARA_FAC;
            break;
        case D_USER:
        default:
            auth_sh = SH_PARA_USR;
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
            | (decimal_sh << DECIMAL_BGN_BIT)
            | (fg_is_hide << HIDE_BGN_BIT)
            | (fg_is_const << CONST_BGN_BIT)
            | (auth_sh << SAFE_BGN_BIT)
            | (fg_is_bit << BIT_BGN_BIT);

    return sign_sh;
}

/*******************************************************************************
 函 数: SH_get_disp_info()
 功 能: 获取显示辅助信息
 参 数: dest_buf--数据返回缓存;
        offset--起始位置;
        num--数据个数(字);
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_get_disp_info(U08 *dest_buf, U16 offset, U16 num)
{
    U08 i;
    I16 disp_info[DISP_INFO_MAX];           /* 显示信息 */
    U08 idx = 0;

    /* 输入数据检查 */
    if (num*sizeof(U16) > MAX_TRAS_BYTES || offset+num > SH_MAX_DISP_INFO)  /* 通讯缓存超界 */
    {
        return FALSE;
    }

    /* 初始化 */
    for (i=0; i<DISP_INFO_MAX; i++)
    {
        disp_info[i] = 0;
    }

    /* *************************更新数据************************* */

    /* 0 模块数 */
    disp_info[DISP_PCB_NUM] = SHC_DISP_PCB_NUM;

    /* 1 压机数 */
    disp_info[DISP_CMP_NUM] = SHC_DISP_COMP_NUM;

    /* 2 机组状态 */
    disp_info[DISP_MAC_STA] = SHC_DISP_GET_MAC_STATUS;

    /* 3 当前温度 */
    disp_info[DISP_TMP_CURT] = SHC_DISP_GET_CTRL_TMP;

    /* 4 设定温度 */
    disp_info[DISP_TMP_SET] = SHC_get_set_tmp_A();

    /* 5 温度单位 */
    disp_info[DISP_SET_UNIT] = SHC_DISP_SET_UNIT;

    /* 6 可设模式(位变量) */
    disp_info[DISP_MODE_VALID] = SHC_get_valid_mode();

    /* 7 设定模式 */
    disp_info[DISP_MODE_SET] = SHC_get_set_mode();

    /* 8 运行模式 */
    disp_info[DISP_MODE_RUN] = SHC_get_run_mode();

    /* 9 定时使用 */        /* 不用此功能 */
    disp_info[DISP_TIMING_USE] = 0;

    /* 10 定时组数(0表示使用前板的定时逻辑) */              /* 固定使用前板的定时 */
    disp_info[DISP_TIMING_GRP] = 0;

    /* 11 初始化状态 */
    disp_info[DISP_INIT_STATE] = SHC_get_init_sta();

    /* 12 使用期限到期(TRUE:到期,FALSE:未到) */
    disp_info[DISP_LMT_ON] = SHC_DISP_GET_LMT_ON;

    /* 13 使用期限设定天数 */
    disp_info[DISP_LMT_SET_DAY] = SHC_DISP_GET_LMT_SET_DAY;

    /* 14 使用期限已运行小时数 */
    disp_info[DISP_LMT_RUN_HOUR] = SHC_DISP_GET_LMT_RUN_HOUR;

    /* 15 使用期限剩余小时数 */
    disp_info[DISP_LMT_LEFT_HOUR] = SHC_DISP_GET_LMT_LEFT_HOUR;

    /* 16 当前故障数 */
    disp_info[DISP_ERR_NUM] = SHC_get_err_num();

    /* 17 历史故障数 */
    disp_info[DISP_HIS_NUM] = SHC_DISP_GET_HIS_NUM;

    /* 18 查询界面 模拟量个数 */
    disp_info[DISP_CHK_AV_NUM] = 0;         /* 不用 */

    /* 19 查询界面 开关量个数 */
    disp_info[DISP_CHK_DV_NUM] = 0;         /* 不用 */

    /* 20 查询界面 附加量个数 */
    disp_info[DISP_CHK_EX_NUM] = 0;         /* 不用 */

    /* 21 参数组数量 */
    disp_info[DISP_PARA_GRP_NUM] = SHC_DISP_GET_PARA_GRP_NUM;

    /* 22 密码组数 */
    disp_info[DISP_PASS_GRP_NUM] = SHC_DISP_GET_PASS_GRP_NUM;

    /* 23 允许手动除霜模块数 */             /* 不用 */
    disp_info[DISP_DEFRO_USE] = 0;

    /* 24 无线功能(TRUE:使用,FALSE:不用) */
    disp_info[DISP_WIFI_USE] = SHC_DISP_GET_WIFI_USE;

    /* 25 预热剩余小时数 */
    disp_info[DISP_PRE_HEAT_TIME_MINT] = SHC_DISP_GET_PREHEAT_MINT;

    /* 26 设定温度最小值 */
    /* 27 设定温度最大值 */
    disp_info[DISP_TMP_SET_MIN] = SHC_get_set_tmp_min_A();
    disp_info[DISP_TMP_SET_MAX] = SHC_get_set_tmp_max_A();

    /* 28 是否允许升级后板程序(TRUE:允许,FALSE:不许) */     /* 不用 */
    disp_info[DISP_BOOT_DOWNLOAD] = FALSE;

    /* 29 当前系统出水温度 */
    disp_info[DISP_TMP_OUT] = SHC_DISP_GET_TMP_OUT;

    /* 30 当前系统回水温度 */
    disp_info[DISP_TMP_IN] = SHC_DISP_GET_TMP_IN;

    /* 31 控制对象 */
    disp_info[DISP_CTRL_OBJ] = SHC_DISP_GET_CTRL_OBJ;

    /* 32 系统模块模拟量 */
    disp_info[DISP_CHK_SYS_AV_NUM] = SHC_get_sys_av_num();

    /* 33 压机模拟量步长 */
    disp_info[DISP_CHK_CMP_AV_STEP] = SHC_COMP_PV_STEP;

    /* 34 单板最大DO输出步长 */
    disp_info[DISP_CHK_DO_STEP] = SHC_DO_STEP;

    /* 35 单板最大DI输入步长 */
    disp_info[DISP_CHK_DI_STEP] = SHC_DI_STEP;

    /* 36 系统开关量输出个数 */
    disp_info[DISP_CHK_SYS_DO_NUM] = SHC_get_sys_do_num();

    /* 37 系统已运行压机数 */
    disp_info[DISP_SYS_COMP_RUN_NUM] = SHC_DISP_GET_CM_RUN_NUM;

    /* 38 设定模式索引(高位组号，低位项号) */               /* 不用 */
    disp_info[DISP_MODE_SET_INDEX] = 0;

    /* 39 调试模式是否使用 */
    disp_info[DISP_TEST_EN] = SHC_DISP_GET_TEST_EN;

    /* 40~41 运行除霜等状态 */
    disp_info[DISP_FORCE_DFRST_INFO_1] = SHC_DISP_GET_MNL_DEFRO_1;
    disp_info[DISP_FORCE_DFRST_INFO_2] = SHC_DISP_GET_MNL_DEFRO_2;

    /* 42 设定温度小数点 */
    disp_info[DISP_TMP_SET_DOT] = SHC_get_set_tmp_dot();

    /* 43 用户查询的DO输出 */
    disp_info[DISP_CHK_USER_DO] = SHC_DISP_GET_CHK_USER_DO;

    /* 44 参数组大小 */
    disp_info[DISP_PARA_GRP_SIZE] = SHC_DISP_GET_PARA_GRP_SIZE;

    /* 45 使用期限天数最大值 */
    disp_info[DISP_LMT_SET_MAX] = SHC_DISP_GET_LMT_SET_MAX;

    /* 46 是否有手动除霜 */
    disp_info[DISP_HAVE_MNL_DEFRO] = SHC_DISP_HAVE_MNL_DEFRO;

    /* 47 运行过程中是否允许修改模式 */
    disp_info[DISP_RUN_ALLOW_SET_MODE] = SHC_DISP_RUN_ALLOW_SET_MODE;

    /* 48 是否有启动机组密码 美意HL051A/HL055A使用 */
    disp_info[DISP_HAVE_START_CODE] = SHC_DISP_HAVE_START_CODE_MEIYI;

    /* 49 启动机组密码 美意HL051A/HL055A使用 */
    disp_info[DISP_START_CODE] = SHC_DISP_START_CODE_MEIYI;

    /* 50 辅助处理标志 */
    disp_info[DISP_DISP_FG] = SHC_get_disp_fg();

    /* 51 蜂鸣器报警模式 */
    disp_info[DISP_ALARM_SOUND_SET] = SHC_get_sound_set();

    /* 52 厂家模式 */
    disp_info[DISP_MODE_TYPE] = SHC_get_mode_type();

    /* 53 当前热水温度 */
    disp_info[DISP_TMP_HOT] = SHC_DISP_GET_TMP_HOT;

    /* 54 湿度可设模式 */
    disp_info[DISP_HUMI_MODE_VALID] = SHC_DISP_GET_HUMI_MODE_VALID;

    /* 55 湿度设定模式 */
    disp_info[DISP_HUMI_MODE_SET] = SHC_DISP_GET_HUMI_MODE_SET;

    /* 56 湿度运行模式 */
    disp_info[DISP_HUMI_MODE_RUN] = SHC_DISP_GET_HUMI_MODE_RUN;

    /* 57 当前湿度 */
    disp_info[DISP_HUMI_CURT] = SHC_DISP_GET_HUMI_CURT;

    /* 58 设定湿度 */
    disp_info[DISP_HUMI_SET] = SHC_DISP_GET_HUMI_SET;

    /* 59 湿度设定最小值 */
    disp_info[DISP_HUMI_SET_MIN] = SHC_DISP_GET_HUMI_SET_MIN;

    /* 60 湿度设定最大值 */
    disp_info[DISP_HUMI_SET_MAX] = SHC_DISP_GET_HUMI_SET_MAX;

    /* 61 辅助处理标志 */
    disp_info[DISP_DISP_FG_EX] = SHC_get_disp_fg_ex();

    /* 62 辅助处理标志，是否可以动作 */
    disp_info[DISP_CAN_DO_FG] = SHC_get_disp_can_do_fg();

    /* 63 第二个设定温度 */
    disp_info[DISP_TMP_SET_EX] = SHC_get_set_tmp_B();

    /* 64 第二个当前温度 */
    disp_info[DISP_TMP_CURT_EX] = SHC_get_ctrl_tmp_B();

    /* 65 第二个设定温度最小值 */
    disp_info[DISP_TMP_SET_EX_MIN] = SHC_get_set_tmp_min_B();

    /* 66 第二个设定温度最大值 */
    disp_info[DISP_TMP_SET_EX_MAX] = SHC_get_set_tmp_max_B();

    /* 67 热水设定温度 */
    disp_info[DISP_TMP_HOT_SET] = SHC_get_hot_tmp_set();

    /* 68 热水设定温度最小值 */
    disp_info[DISP_TMP_HOT_SET_MIN] = SHC_get_hot_tmp_set_min();

    /* 69 热水设定温度最大值 */
    disp_info[DISP_TMP_HOT_SET_MAX] = SHC_get_hot_tmp_set_max();

    /* 70 系统维护类型信息 */
    disp_info[DISP_LMT_TYPE_INFO] = SHC_get_lmt_type_info();

    /* 71 系统维护类型 */
    disp_info[DISP_LMT_TYPE_VALUE] = SHC_get_lmt_type_value();

    /* 72 变频故障个数 */
    disp_info[DISP_INVT_ERR_NUM] = SHC_get_invt_err_num();

    /* 73 静音模式级别 */
    disp_info[DISP_SILENT_LEVEL] = SHC_get_silent_level();

    /* 74 静音模式级别上下限 */
    disp_info[DISP_SILENT_LEVEL_ATTR] = SHC_get_silent_level_attr();

    /* 75 下发室内温度间隔 */
    disp_info[DISP_ROOM_TEMP_WR_INTVL] = SHC_get_room_temp_wr_intvl();

    /* 76 后板语言 */
    disp_info[DISP_BKBD_LANGUAGE] = SHC_get_bkbd_language();

    /* 77 辅助处理信息 */
    disp_info[DISP_DISP_FG_EX2] = SHC_get_disp_fg_ex2();

    /* 80 显示语言配置1 */
    disp_info[DISP_LANGUAGE_FG1] = SHC_get_disp_language1();

    /* 81 显示语言配置2 */
    disp_info[DISP_LANGUAGE_FG2] = SHC_get_disp_language2();

    /* ********************************************************** */
    /* 赋值 */
    for (i=0; i<num; i++)
    {
        U16 temp = 0;
        if (offset+i < DISP_INFO_MAX)
        {
            temp = disp_info[offset+i];
        }
        dest_buf[idx++] = HIBYTE(temp);
        dest_buf[idx++] = LOBYTE(temp);
    }
    return TRUE;
}

/*******************************************************************************
 函 数: SH_get_version()
 功 能: 获取版本号
 参 数: dest_buf--数据返回缓存;
        offset--起始位置;
        num--版本个数;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_get_version(U08 *dest_buf, U16 offset, U16 num)
{
    U08 i;
    U16 pcb_id = 0;         /* 板号 */
    SH_CHK_VER ver;         /* 显示信息 */
    U08 idx = 0;            /* 数据返回索引 */
    U08 *pVerCode = NULL;   /* 指向功能码字符串源 */
    U08 data_size = 0;      /* 数据大小 */
    U16 *src_buf = NULL;    /* 指向源数据的指针 */

    /* 输入数据检查 */
    if (num*sizeof(SH_CHK_VER) > MAX_TRAS_BYTES || offset+num > SH_MAX_PCB_NUM) /* 通讯缓存超界 */
    {
        return FALSE;
    }

    for (pcb_id=offset; pcb_id<offset+num; pcb_id++)
    {
        /* *************************更新数据************************* */

        /* 板名、板号、版本号源、版本号长度 */
        ver.pcb_name = SHC_VER_PCB_NAME;
        ver.pcb_id = MAKEWORD(pcb_id, NO_ID);
        pVerCode = SHC_VER_CODE(pcb_id);
        data_size = SHC_VER_DATA_SIZE;

        /* ********************************************************** */

        /* 获取版本号字符串 */
        for (i=0; i<MAX_VER_BYTE; i++)
        {
            if (i < data_size)
            {
                ver.fun_code[i] = pVerCode[i];
            }
            else
            {
                ver.fun_code[i] = 0;
            }
        }
        /* 赋返回值 */
        dest_buf[idx++] = HIBYTE(ver.pcb_name);
        dest_buf[idx++] = LOBYTE(ver.pcb_name);
        dest_buf[idx++] = HIBYTE(ver.pcb_id);
        dest_buf[idx++] = LOBYTE(ver.pcb_id);
        src_buf = (U16*)&ver.fun_code;
        data_size = MAX_VER_BYTE / 2;
        for (i=0; i<data_size; i++)
        {
            dest_buf[idx++] = HIBYTE(src_buf[i]);
            dest_buf[idx++] = LOBYTE(src_buf[i]);
        }
    }

    return TRUE;
}

/*******************************************************************************
 函 数: SH_get_vfd_version()
 功 能:
 参 数: dest_buf--;
        offset--;
        num--;
 返 回: 空
 其 它: 空
*******************************************************************************/
BOOL SH_get_vfd_version(U08 *dest_buf, U16 offset, U16 num)
{
    U08 i;
    U16 pcb_id = 0;         /* 板号 */
    SH_CHK_VER ver;         /* 显示信息 */
    U08 idx = 0;            /* 数据返回索引 */
    U08 data_size = 0;      /* 数据大小 */
    U16 *src_buf = NULL;    /* 指向源数据的指针 */

    /* 输入数据检查 */
    if (num*sizeof(SH_CHK_VER) > MAX_TRAS_BYTES || offset+num > SH_MAX_PCB_NUM) /* 通讯缓存超界 */
    {
        return FALSE;
    }

    for (pcb_id=offset; pcb_id<offset+num; pcb_id++)
    {
        /* *************************更新数据************************* */
        SHC_get_vfd_ver(&ver, pcb_id);
        /* ********************************************************** */

        /* 赋返回值 */
        dest_buf[idx++] = HIBYTE(ver.pcb_name);
        dest_buf[idx++] = LOBYTE(ver.pcb_name);
        dest_buf[idx++] = HIBYTE(ver.pcb_id);
        dest_buf[idx++] = LOBYTE(ver.pcb_id);
        src_buf = (U16*)&ver.fun_code;
        data_size = MAX_VER_BYTE / 2;
        for (i=0; i<data_size; i++)
        {
            dest_buf[idx++] = HIBYTE(src_buf[i]);
            dest_buf[idx++] = LOBYTE(src_buf[i]);
        }
    }

    return TRUE;
}

/*******************************************************************************
 函 数: SH_get_odu_version()
 功 能: 获取外机板版本号
 参 数: dest_buf--数据返回缓存;
        offset--起始位置;
        num--版本个数;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_get_exio_version(U08 *dest_buf, U16 offset, U16 num)
{
    U08 i;
    U16 cell_id = 0;         /* 板号 */
    SH_CHK_VER ver;         /* 显示信息 */
    U08 idx = 0;            /* 数据返回索引 */
    U08 *pVerCode = NULL;   /* 指向功能码字符串源 */
    U08 data_size = 0;      /* 数据大小 */
    U16 *src_buf = NULL;    /* 指向源数据的指针 */

    /* 输入数据检查 */
    if (num*sizeof(SH_CHK_VER) > MAX_TRAS_BYTES || offset+num > SH_MAX_PCB_NUM*SH_MAX_EXIO_NUM) /* 通讯缓存超界 */
    {
        return FALSE;
    }

    for (cell_id=offset; cell_id<offset+num; cell_id++)
    {
        U16 pcb_id = cell_id / SH_MAX_EXIO_NUM;
        U16 exio_id = cell_id % SH_MAX_EXIO_NUM;

        /* *************************更新数据************************* */

        /* 板名、板号、版本号源、版本号长度 */
        ver.pcb_name = SHC_EXIO_VER_PCB_NAME;
        ver.pcb_id = MAKEWORD(pcb_id, exio_id);
        pVerCode = SHC_EXIO_VER_CODE(pcb_id, exio_id);
        data_size = SHC_EXIO_VER_DATA_SIZE;

        /* ********************************************************** */

        /* 获取版本号字符串 */
        for (i=0; i<MAX_VER_BYTE; i++)
        {
            if (i < data_size)
            {
                ver.fun_code[i] = pVerCode[i];
            }
            else
            {
                ver.fun_code[i] = 0;
            }
        }
        /* 赋返回值 */
        dest_buf[idx++] = HIBYTE(ver.pcb_name);
        dest_buf[idx++] = LOBYTE(ver.pcb_name);
        dest_buf[idx++] = HIBYTE(ver.pcb_id);
        dest_buf[idx++] = LOBYTE(ver.pcb_id);
        src_buf = (U16*)&ver.fun_code;
        data_size = MAX_VER_BYTE / 2;
        for (i=0; i<data_size; i++)
        {
            dest_buf[idx++] = HIBYTE(src_buf[i]);
            dest_buf[idx++] = LOBYTE(src_buf[i]);
        }
    }

    return TRUE;
}

/*******************************************************************************
 函 数: SH_get_4G_info()
 功 能:
 参 数: dest_buf--;
		offset--;
		num--;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_get_4G_info(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i;
    SH_4G_INFO info;
    U16 idx = 0;            /* 数据返回索引 */
    U16 *src_buf = NULL;

    /* 输入数据检查 */
    if (offset+num > SH_4G_INFO_SIZE) /* 通讯缓存超界 */
    {
        return FALSE;
    }

    VALCLR(info);
    // 赋值
    SHC_get_4G_info(&info);

    /* 赋返回值 */
    src_buf = (U16 *)&info + offset;
    for(i=0; i<SH_4G_INFO_SIZE; i++)
    {
        dest_buf[idx++] = HIBYTE(src_buf[i]);
        dest_buf[idx++] = LOBYTE(src_buf[i]);
    }

    return TRUE;
}

/*******************************************************************************
 函 数: SH_get_passwrd()
 功 能: 获取密码
 参 数: dest_buf--数据返回缓存;
        offset--密码组号;
        num--密码组数;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_get_passwrd(U08 *dest_buf, U16 offset, U16 num)
{
    U08 i;
    SH_PASS passwrd;        /* 密码 */
    U16 pass_grp = 0;       /* 密码组 */
    U08 idx = 0;            /* 数据返回索引 */
    U16 *passwrd_src = PNULL;               /* 指向密码字符串源 */
    const U16 pass_null[MAX_PASS_BIT/2] = {0xffff, 0xffff, 0xffff, 0xffff};
    U16 pass_temp[MAX_PASS_BIT/2] = {0xffff, 0xffff, 0xffff, 0xffff};

    /* 输入数据检查 */
    if (num*sizeof(SH_PASS) > MAX_TRAS_BYTES || offset+num > SH_MAX_PASS_GRP)   /* 通讯缓存超界 */
    {
        return FALSE;
    }

    for (pass_grp=offset; pass_grp<offset+num; pass_grp++)
    {
        /* *************************更新数据************************* */
        /* 权限、密码 */
        switch (pass_grp)
        {
            case PASS_LIMIT:
                passwrd.authority = SH_PWR_SALE;
                passwrd_src = (U16*)&password[PASS_LIMIT][0];
                break;
            case PASS_USER:
                passwrd.authority = SH_PWR_USER;
                passwrd_src = (U16*)&password[PASS_USER][0];
                break;
            case PASS_PRO:
                passwrd.authority = SH_PWR_PRO;
                passwrd_src = (U16*)&password[PASS_PRO][0];
                break;
            case PASS_SER:
                passwrd.authority = SH_PWR_SER;
                passwrd_src = (U16*)&password[PASS_SER][0];
                break;
            case PASS_FAC:
                passwrd.authority = SH_PWR_FAC;
                passwrd_src = (U16*)&password[PASS_FAC][0];
                break;
            case PASS_SUPER:
                passwrd.authority = SH_PWR_SPR;
                SHC_get_pass_super(pass_temp);
                passwrd_src = (U16*)&pass_temp[0];
                break;
            case PASS_POP:
                passwrd.authority = SH_PWR_START_MAC;
                SHC_get_pass_pop(pass_temp);
                passwrd_src = (U16*)&pass_temp[0];
                break;
            default:
                passwrd.authority = SH_PWR_NONE;
                passwrd_src = (U16*)pass_null;
                break;
        }

        /* ********************************************************** */

        /* 赋返回值 */
        dest_buf[idx++] = HIBYTE(passwrd.authority);
        dest_buf[idx++] = LOBYTE(passwrd.authority);
        for (i=0; i<MAX_PASS_BIT/2; i++)
        {
            if (fg_pass_big_endian == TRUE)
            {
                dest_buf[idx++] = HIBYTE(passwrd_src[i]);
                dest_buf[idx++] = LOBYTE(passwrd_src[i]);
            }
            else
            {
                dest_buf[idx++] = LOBYTE(passwrd_src[i]);
                dest_buf[idx++] = HIBYTE(passwrd_src[i]);
            }
        }
    }

    return TRUE;
}

/*******************************************************************************
 函 数: SH_get_holiday_info()
 功 能: 获取
 参 数: dest_buf--数据返回缓存;
        offset--密码组号;
        num--密码组数;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_get_holidayinfo(U08 *dest_buf, U16 data_num)
{
    SH_HOLIDAYINFO info;
    U16 *src_buf = PNULL;
    U16 idx = 0;
    U16 n = 0;

    /* 输入数据检查 */
    if (data_num != SH_HOLIDAYINFO_SIZE)   /* 读取个数不一致 */
    {
        return FALSE;
    }

    VALCLR(info);

    SHC_get_holiday_info(&info);

    //赋返回值
    src_buf = (U16*)&info;
    for (idx=0,n=0; n<data_num; n++)
    {
        dest_buf[idx++] = HIBYTE(src_buf[n]);
        dest_buf[idx++] = LOBYTE(src_buf[n]);
    }

    return TRUE;
}

/*******************************************************************************
 函 数: SH_get_curt_error()
 功 能: 获取当前故障
 参 数: dest_buf--数据返回缓存;
        offset--起始位置;
        num--故障数;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_get_curt_error(U08 *dest_buf, U16 offset, U16 num)
{
    U16 j = 0;              /* 板号 */
    U16 sys_err_id;
    U16 mod_id;
    U16 mod_err_id;         /* 故障索引 */
    U08 fg_err = FALSE;     /* 找到故障 */
    SH_ERR_CURT err;        /* 故障信息 */
    U16 idx = 0;            /* 数据返回索引 */
    U16 *src_buf = NULL;
    U16 err_idx = 0;        /* 当前故障列表的故障号 从0开始 */
    U16 err_cnt = 0;        /* 读取的故障数 */

    /* 输入数据检查 */
    if (num*sizeof(SH_ERR_CURT) > MAX_TRAS_BYTES || offset+num > SH_MAX_ERR_CURT_NUM)   /* 通讯缓存超界 */
    {
        return FALSE;
    }

    sys_err_id = mod_id = mod_err_id = 0;
    while (TRUE)
    {
        fg_err = FALSE;

        /* 搜索结束 */
        if (err_cnt >= num)
        {
            break;
        }

        fg_err = SHC_get_curt_err(&err, &sys_err_id, &mod_id, &mod_err_id);

        if (!fg_err)
        {
            err.name = 0;
            err.id = 0;
            err.err_id = 0;
        }

        /* ********************************************************** */
        if (err_idx >= offset || !fg_err)   /* 没查到故障说明已经没有故障,也要填充通信缓冲 */
        {
            err_cnt++;
            /* 赋返回值 */
            src_buf = (U16*)&err;
            for (j=0; j<SH_ERR_CRT_SIZE; j++)
            {
                dest_buf[idx++] = HIBYTE(src_buf[j]);
                dest_buf[idx++] = LOBYTE(src_buf[j]);
            }
        }
        if (fg_err)
        {
            err_idx++;
        }
    }
    return TRUE;
}

/*******************************************************************************
 函 数: SH_get_his_error()
 功 能: 获取历史故障
 参 数: dest_buf--数据返回缓存;
        offset--起始位置;
        num--故障数;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_get_his_error(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i, j;
    SH_ERR_HIS err;         /* 故障信息 */
    U16 idx = 0;            /* 数据返回索引 */
    U16 *src_buf = NULL;

    /* 输入数据检查 */
    if (num*sizeof(SH_ERR_HIS) > MAX_TRAS_BYTES || offset+num > SH_MAX_ERR_HIS_NUM) /* 通讯缓存超界 */
    {
        return FALSE;
    }

    for(i=0; i<num; i++)
    {
        SHC_get_his_err_info(&err, offset+i);

        /* ********************************************************** */

        /* 赋返回值 */
        src_buf = (U16*)&err;
        for (j=0; j<SH_ERR_HIS_SIZE; j++)
        {
            dest_buf[idx++] = HIBYTE(src_buf[j]);
            dest_buf[idx++] = LOBYTE(src_buf[j]);
        }
    }

    return TRUE;
}

/*******************************************************************************
 函 数: SH_get_fcuinfo()
 功 能: 获取风盘信息
 参 数: dest_buf--数据返回缓存;
        offset--起始位置;
        num--读取数据个数;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_get_fcuinfo(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i;
    SH_FCU info;         /* 风盘信息 */
    U16 idx = 0;            /* 数据返回索引 */
    U16 *src_buf = NULL;

    /* 输入数据检查 */
    if (offset+num > SH_FCUINFO_SIZE) /* 通讯缓存超界 */
    {
        return FALSE;
    }

    VALCLR(info);
    // 赋值
    SHC_get_fcu_info(&info);

    /* 赋返回值 */
    src_buf = (U16 *)&info + offset;
    for(i=0; i<SH_FCUINFO_SIZE; i++)
    {
        dest_buf[idx++] = HIBYTE(src_buf[i]);
        dest_buf[idx++] = LOBYTE(src_buf[i]);
    }

    return TRUE;
}

/*******************************************************************************
 函 数: SH_get_mode_set_info()
 功 能:
 参 数: dest_buf--;
        offset--;
        num--;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_get_mode_set_info(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i;
    SH_MODE_SET_INFO info;
    U16 idx = 0;            /* 数据返回索引 */
    U16 *src_buf = NULL;

    /* 输入数据检查 */
    if (offset+num > SH_MODE_SET_INFO_SIZE) /* 通讯缓存超界 */
    {
        return FALSE;
    }

    VALCLR(info);
    // 赋值
    SHC_get_mode_set_info(&info);

    /* 赋返回值 */
    src_buf = (U16 *)&info + offset;
    for(i=0; i<SH_MODE_SET_INFO_SIZE; i++)
    {
        dest_buf[idx++] = HIBYTE(src_buf[i]);
        dest_buf[idx++] = LOBYTE(src_buf[i]);
    }

    return TRUE;
}

/*******************************************************************************
 函 数: SH_get_main_info()
 功 能:
 参 数: dest_buf--;
        offset--;
        num--;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_get_main_info(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i;
    U16 idx = 0;            /* 数据返回索引 */
    U16 *src_buf = NULL;

    /* 输入数据检查 */
    if (offset+num > SH_MAIN_INFO_SIZE) /* 通讯缓存超界 */
    {
        return FALSE;
    }

    VALCLR(sh_main_info);
    // 赋值
    SHC_get_main_info(&sh_main_info);

    /* 赋返回值 */
    src_buf = (U16 *)&sh_main_info + offset;
    for(i=0; i<SH_MAIN_INFO_SIZE; i++)
    {
        dest_buf[idx++] = HIBYTE(src_buf[i]);
        dest_buf[idx++] = LOBYTE(src_buf[i]);
    }

    return TRUE;
}

/*******************************************************************************
 函 数: SH_get_invt_err()
 功 能:
 参 数: dest_buf--;
        offset--;
        num--;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_get_invt_err(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i, j;
    SH_CHK_INFO info;       /* 模拟量信息 */
    U16 idx = 0;            /* 数据返回索引 */
    U16 *src_buf = NULL;    /* 指向源数据 */

    /* 输入数据检查 */
    if (num*sizeof(SH_CHK_INFO) > MAX_TRAS_BYTES/* || offset+num > SH_MAX_CHK_INFO*/)//通讯缓存超界
    {
        return FALSE;
    }

    for (i=offset; i<offset+num; i++)
    {
        /* *************************更新数据************************* */

        if (i < SHC_get_invt_err_num())
        {
            SHC_get_invt_err_info(&info, i);
        }

        /* ********************************************************** */

        /* 赋返回值 */
        src_buf = (U16*)&info;
        for (j=0; j<SH_CHK_SYS_AV_SIZE; j++)
        {
            dest_buf[idx++] = HIBYTE(src_buf[j]);
            dest_buf[idx++] = LOBYTE(src_buf[j]);
        }
    }

    return TRUE;
}

/*******************************************************************************
 函 数: SH_cal_grp_attrib()
 功 能: 将本地属性格式转换为SH属性格式
 参 数: grp--参数组;
 返 回: 参数组属性
 其 它: 参数组属性有:权限(使用单位的掩码)、是否隐藏
*******************************************************************************/
U16 SH_cal_grp_attrib(U08 grp)
{
    U16 sign_local = 0;     /* 原始属性 */
    U16 sign_sh = 0;        /* 转换后属性 */
    U16 authority_local = 0;/* 原始权限 */
    U16 authority_sh = 0;   /* 转换后权限 */
    U16 fg_is_hide = 0;     /* 参数隐藏 */

    /* 读取属性 */
    sign_local = SHC_get_para_grp_sign(grp);

    /* 权限 */
    authority_local = sign_local & D_SAFEMASK;
    switch (authority_local)
    {
        case D_USER:
            authority_sh = SH_PWR_USER;
            break;
        case D_PRO:
            authority_sh = SH_PWR_PRO;
            break;
        case D_SER:
            authority_sh = SH_PWR_SER;
            break;
        case D_FAC:
            authority_sh = SH_PWR_FAC;
            break;
        default:
            authority_sh = SH_PWR_NONE;
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

/*******************************************************************************
 函 数: SH_get_para_grp_info()
 功 能: 获取参数组信息
 参 数: dest_buf--数据返回缓存;
        offset--起始位置;
        num--数据个数(字);
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_get_para_grp_info(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i;
    SH_PARA_GRP para_grp_name[SH_MAX_PARA_GRP_NUM];         /* 显示信息 */
    U16 idx = 0;            /* 数据返回索引 */
    U16 data_words = 0;     /* 数据字大小 */
    U16 offset_words = 0;   /* 字偏移基地址 */
    U16 *src_buf = NULL;

    /* 输入数据检查 */
    if (num*sizeof(SH_PARA_GRP) > MAX_TRAS_BYTES || offset+num > SH_MAX_PARA_GRP_NUM)/* 通讯缓存超界 */
    {
        return FALSE;
    }

    /* *************************更新数据************************* */

    for (i=0; i<SHC_DISP_GET_PARA_GRP_NUM; i++)
    {
        para_grp_name[i].name = SHC_NAME_PARA_GRP(i);
        para_grp_name[i].item_num = (U16)SHC_get_para_grp_item(i);
        para_grp_name[i].attrib = SH_cal_grp_attrib((U08)i);
    }

    /* ********************************************************** */

    /* 赋值 */
    src_buf = (U16*)&para_grp_name[0].name;
    data_words = num * SH_PARA_GRP_SIZE;
    offset_words = offset * SH_PARA_GRP_SIZE;
    for (i=0; i<data_words; i++)
    {
        if (offset_words+i < SH_MAX_PARA_GRP_NUM*SH_PARA_GRP_SIZE)
        {
            dest_buf[idx++] = HIBYTE(src_buf[offset_words+i]);
            dest_buf[idx++] = LOBYTE(src_buf[offset_words+i]);
        }
        else
        {
            dest_buf[idx++] = 0;
            dest_buf[idx++] = 0;
        }
    }
    return TRUE;
}

/*****************************************************************************
函  数 : SH_attr_local2sh()
功  能 : 本地属性转换为SH属性,增加字典信息
参  数 : U16 sign_local     : 本地属性
         const U16 *dic_str : 字典
返  回 : 无
其  他 : 无
*****************************************************************************/
U16 SH_attr_local2sh(U16 sign_local, const U16 *dic_str)
{
    U16 sign_sh = 0;
    U16 dict_sh = 0;        /* 转换后单位 */
    U16 dict_idx = 0;       /* 参数值字典序号 */

    sign_sh |= SH_sign_local2sh(sign_local);

    if (dic_str != NULL)    /* 字符串型:加上字典 */
    {
        sign_sh &= ~(SH_UNIT|SH_DECIMAL);
        dict_idx = SHC_get_dict_idx(dic_str);/* 字典序号 */
        dict_sh = dict_idx + UNIT_OFFSET;   /* 加上偏移 */
        sign_sh |= dict_sh;
    }

    return sign_sh;
}

/*******************************************************************************
 函 数: SH_cal_para_attrib()
 功 能: 将本地属性格式转换为SH属性格式
 参 数: grp--参数组;
        item--参数项;
 返 回: 参数项属性
 其 它: 参数组属性有:是否为字符串型、单位、是否隐藏、是否可改
*******************************************************************************/
U16 SH_cal_para_attrib(U08 grp, U08 item)
{
    U16 sign_local = 0;     /* 原始属性 */
    U16 sign_sh = 0;        /* 转换后属性 */

    /* 读取原始属性 */
    sign_local = SHC_get_para_get_sign(grp, item);

    sign_sh |= SH_attr_local2sh(sign_local, SHC_get_para_small_pmsg(grp, item));

    return sign_sh;
}

/*******************************************************************************
 函 数: SH_get_dict_attrib()
 功 能:
 参 数: dest_buf--;
        offset--;
        num--;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_get_dict_attrib(U08 *dest_buf, U16 offset, U16 num)
{
    U08 i;
    U08 idx = 0;            /* 数据返回索引 */
    U08 array_num = 0;      /* 字符组数 */
    U16 data_words = 0;     /* 数据字数 */
    U16 offset_words = 0;   /* 字偏移基地址 */
    U16 src_buf[SH_MAX_PARA_STRING_GRP];
    U16 dic_idx;

    /* 输入数据检查 */
    if (num > MAX_TRAS_BYTES/2 || offset+num > SH_MAX_PARA_STRING_GRP)  /* 通讯缓存超界 */
    {
        return FALSE;
    }

    /* *************************更新数据************************* */
    array_num = SHC_GET_DICT_NUM;
    ARRCLR(src_buf);
    for (i=0, dic_idx=0; i<array_num; i++)
    {
        src_buf[i] = dic_idx;
        dic_idx += SHC_get_dict_items(i);
    }
    /* ********************************************************** */

    /* 赋值 */
    data_words = num;
    offset_words = offset;
    for (i=0; i<data_words; i++)
    {
        if (offset_words+i < array_num)     /* 有效部分赋值 */
        {
            dest_buf[idx++] = HIBYTE(src_buf[offset_words+i]);
            dest_buf[idx++] = LOBYTE(src_buf[offset_words+i]);
        }
        else                /* 无效部分赋0 */
        {
            dest_buf[idx++] = 0;
            dest_buf[idx++] = 0;
        }
    }

    return TRUE;
}

/*******************************************************************************
 函 数: SH_get_dict_string()
 功 能: 获取参数字典字符串
 参 数: dest_buf--数据返回缓存;
        offset--起始位置;
        num--数据个数(字);
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_get_dict_string(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i;
    U16 msg = 0;            /* 临时变量 */
    U08 idx = 0;            /* 数据返回索引 */
    U08 array_num = 0;      /* 字符组数 */
    U16 dic_max;
    U16 dic_items[SH_MAX_PARA_STRING_GRP];

    /* 输入数据检查 */
    if (num > MAX_TRAS_BYTES/2 || offset+num > SH_MAX_DICT_STR)     /* 通讯缓存超界 */
    {
        return FALSE;
    }

    /* *************************更新数据************************* */
    array_num =  SHC_GET_DICT_NUM;
    ARRCLR(dic_items);
    for (i=0, dic_max=0; i<array_num; i++)
    {
        U16 items = SHC_get_dict_items(i);
        dic_items[i] = items;
        dic_max += items;
    }
    /* ********************************************************** */

    dic_max = MIN(dic_max, SH_MAX_DICT_STR);
    for (i=offset; i<offset+num; i++)
    {
        msg = 0;
        if (i < dic_max)    /* dic_max 即为最大字典字符串项数 */
        {
            XY xy = fun_get_relative_pos(dic_items, array_num, i);
            msg = SHC_get_dict_pmsg(xy.x, xy.y);
        }
        /* 赋值 */
        dest_buf[idx++] = HIBYTE(msg);
        dest_buf[idx++] = LOBYTE(msg);

    }
    return TRUE;
}

/*******************************************************************************
 函 数: SH_get_para_item_info()
 功 能: 获取参数项信息
 参 数: dest_buf--数据返回缓存;
        offset--起始位置;
        num--数据个数(字);
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_get_para_item_info(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i, j;
    U08 grp_idx = 0;        /* 组号 */
    U08 item_idx = 0;       /* 项号 */
    SH_PARA_ATTRIB para_item;               /* 显示信息 */
    U08 idx = 0;            /* 数据返回索引 */
    U16 *src_buf = NULL;    /* 数据指针 */

    /* 输入数据检查 */
    if (num*sizeof(SH_PARA_ATTRIB) > MAX_TRAS_BYTES || num+offset > SH_MAX_PARA_ITEM_NUM*SH_MAX_PARA_GRP_NUM)/* 通讯缓存超界 */
    {
        return FALSE;
    }

    /* *************************更新数据************************* */

    for (i=offset; i<offset+num; i++)
    {
        /* 计算组号及项号 */
        grp_idx = (U08)(i / SHC_DISP_GET_PARA_GRP_SIZE);
        item_idx = (U08)(i % SHC_DISP_GET_PARA_GRP_SIZE);

        /* 默认为隐藏 */
        para_item.attrib = (1 << HIDE_BGN_BIT);

        if (grp_idx < SHC_DISP_GET_PARA_GRP_NUM
            && item_idx < SHC_get_para_grp_item(grp_idx))
        {
            para_item.name = SHC_NAME_PARA(grp_idx, item_idx);
            para_item.id = 0xFFFF;
            /* 数值 */
            para_item.value = SHC_para_get_val(grp_idx, item_idx);
            /* 最小值 */
            para_item.min = SHC_para_get_min(grp_idx, item_idx);
            /* 最大值 */
            para_item.max = SHC_para_get_max(grp_idx, item_idx);
            /* 属性 */
            para_item.attrib = SH_cal_para_attrib(grp_idx, item_idx);
        }
        else
        {
            return FALSE;
        }

        /* ********************************************************** */

        /* 赋值 */
        src_buf = (U16*)&para_item;
        for (j=0; j<SH_PARA_ATTRIB_SIZE; j++)
        {
            dest_buf[idx++] = HIBYTE(src_buf[j]);
            dest_buf[idx++] = LOBYTE(src_buf[j]);
        }
    }

    return TRUE;
}

/*******************************************************************************
 函 数: SH_get_bms_para_info()
 功 能: 获取BMS通讯设置参数项信息
 参 数: dest_buf--数据返回缓存;
        offset--起始位置;
        num--数据个数(字);
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_get_bms_para_info(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i, j;
    SH_PARA_ATTRIB para_item;               /* 显示信息 */
    U08 idx = 0;            /* 数据返回索引 */
    U16 *src_buf = NULL;    /* 数据指针 */

    /* 输入数据检查 */
    if (num*sizeof(SH_PARA_ATTRIB) > MAX_TRAS_BYTES || num+offset > SH_MAX_BMS_PARA_NUM)    /* 通讯缓存超界 */
    {
        return FALSE;
    }

    /* *************************更新数据************************* */

    for (i=offset; i<offset+num; i++)
    {
        /* 默认为隐藏 */
        para_item.attrib = (1 << HIDE_BGN_BIT);

        if (i < SHC_BMS_PARA_NUM)
        {
            para_item.name = SHC_BMS_PARA_NAME(i);
            para_item.id = 0xFFFF;
            /* 数值 */
            para_item.value= SHC_BMS_PARA_VAL(i);
            /* 最小值 */
            para_item.min = SHC_BMS_PARA_MIN(i);
            /* 最大值 */
            para_item.max = SHC_BMS_PARA_MAX(i);
            /* 属性 */
            para_item.attrib = SH_attr_local2sh(SHC_BMS_PARA_SIGN(i), SHC_BMS_PARA_STR(i));
        }
        else if (i < SH_MAX_BMS_PARA_NUM)
        {
            para_item.name = 0;
        }
        else
        {
            return FALSE;
        }

        /* ********************************************************** */

        /* 赋值 */
        src_buf = (U16*)&para_item;
        for (j=0; j<SH_BMS_PARA_SIZE; j++)
        {
            dest_buf[idx++] = HIBYTE(src_buf[j]);
            dest_buf[idx++] = LOBYTE(src_buf[j]);
        }
    }

    return TRUE;
}

/*******************************************************************************
 函 数: SH_get_pcb_comp_num()
 功 能: 获取压机数
 参 数: dest_buf--数据返回缓存;
        offset--起始位置;
        num--数据个数(字);
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_get_pcb_comp_num(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i;
    U08 idx = 0;            /* 数据返回索引 */

    /* 输入数据检查 */
    if (num+offset > SH_MAX_PCB_NUM)
    {
        return FALSE;
    }

    /* *************************更新数据************************* */

    for (i=offset; i<offset+num; i++)
    {
        dest_buf[idx++] = HIBYTE(SHC_pcb_comp_num(i));
        dest_buf[idx++] = LOBYTE(SHC_pcb_comp_num(i));
    }

    return TRUE;
}

/*******************************************************************************
 函 数: SH_get_test_ro()
 功 能: 获取调试模式的字符串和电膨开度等信息
 参 数: dest_buf--发送缓冲;
        offset--地址偏移;
        num--数据个数;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_get_test_ro(U08 *dest_buf, U16 offset, U16 num)
{
    U08 i;
    U08 idx = 0;

    /* 输入数据检查 */
    if (num*sizeof(U16) > MAX_TRAS_BYTES || offset+num > SH_CHK_TEST_RO_SIZE)/* 通讯缓存超界 */
    {
        return FALSE;
    }

    /* 调试输出字符串赋值 */
    for (i=0; i<SHC_TEST_DO_NUM; i++)
    {
        test_data.ro.str_DO[i] = SHC_get_test_DO_string(i);
    }

    /* 调试电膨字符串赋值 */
    for (i=0; i<SHC_TEST_EEV_NUM; i++)
    {
        test_data.ro.str_EEV[i] = SHC_get_test_eev_string(i);
    }

    /* 输出个数 */
    test_data.ro.DO_num = SHC_TEST_DO_NUM;

    /* 膨胀阀个数 */
    test_data.ro.EEV_num = SHC_TEST_EEV_NUM;

    /* 调试电膨最小步数 */
    for (i=0; i<SHC_TEST_EEV_NUM; i++)
    {
        test_data.ro.EEV_step_min[i] = 0;
    }

    /* 调试电膨最大步数 */
    for (i=0; i<SHC_TEST_EEV_NUM; i++)
    {
        test_data.ro.EEV_step_max[i] = SHC_get_motor_max(i);
    }

    /* 调试电膨当前步数 */
    for (i=0; i<SHC_TEST_EEV_NUM; i++)
    {
        test_data.ro.EEV_step_pv[i] = SHC_get_motor_pv(i);
    }

    /* 是否允许继电器手动模式 */
    test_data.ro.fg_DO_can_mnl_mode = SHC_get_DO_can_mnl();

    /* 是否允许电膨手动模式 */
    test_data.ro.fg_EEV_can_mnl_mode = TRUE;

    /* 赋值 */
    for (i=0; i<num; i++)
    {
        U16 temp = 0;
        if (i < SH_CHK_TEST_RO_SIZE)
        {
            temp = ((U16 *)(&test_data.ro))[i];
        }
        dest_buf[idx++] = HIBYTE(temp);
        dest_buf[idx++] = LOBYTE(temp);
    }
    return TRUE;
}

/*******************************************************************************
 函 数: SH_get_test_wr()
 功 能: 读取后板调试用的DO状态
 参 数: dest_buf--;
        offset--;
        num--;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_get_test_wr(U08 *dest_buf, U16 offset, U16 num)
{
    U08 i;
    U08 idx = 0;
    U16 temp[_DIVUCARRY(SHC_TEST_DO_NUM, 16)];

    /* 输入数据检查 */
    if (num > MAX_TRAS_BYTES/2 || offset+num > SH_CHK_TEST_WR_SIZE)
    {
        return FALSE;
    }

    ARRCLR(temp);
    for (i=0; i<SHC_TEST_DO_NUM; i++)
    {
        if (SHC_get_test_do_sta(i))
        {
            SETBIT_N(temp, i);
        }
    }

    /* 赋值 */
    for (i=0; i<num; i++)
    {
        dest_buf[idx++] = HIBYTE(temp[i]);
        dest_buf[idx++] = LOBYTE(temp[i]);
    }

    return TRUE;
}

/*****************************************************************************
函  数 : SH_get_chk_info_num()
功  能 : 查询信息个数信息
参  数 : U08 *dest_buf : 数据返回缓存;
         U16 offset    : 起始信息单元位置
         U16 num       : 信息单元个数;
返  回 : TRUE/FALSE--成功/失败
其  他 : 无
*****************************************************************************/
BOOL SH_get_chk_info_num(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i, j;
    SH_CHK_INFO_NUM info_num;
    U16 idx = 0;            /* 数据返回索引 */
    U16 *src_buf = NULL;    /* 指向源数据 */

    /* 输入数据检查 */
    if (num*sizeof(SH_CHK_INFO_NUM) > MAX_TRAS_BYTES/* || offset+num > SH_MAX_CHK_INFO*/)//通讯缓存超界
    {
        return FALSE;
    }


    for (i=offset; i<offset+num; i++)
    {
        U16 pcb = i;

        if (pcb < SH_MAX_PCB_NUM)
        {
            SHC_get_info_num(&info_num, pcb);
        }

        /* ********************************************************** */

        /* 赋返回值 */
        src_buf = (U16*)&info_num;
        for (j=0; j<SH_CHK_NUM_SIZE; j++)
        {
            dest_buf[idx++] = HIBYTE(src_buf[j]);
            dest_buf[idx++] = LOBYTE(src_buf[j]);
        }
    }

    return TRUE;
}

/*******************************************************************************
 函 数: SH_get_chk_info_av_mod()
 功 能: 获取当前系统和模块模拟量显示信息
 参 数: dest_buf--数据返回缓存;
        offset--起始位置;
        num--模拟量数;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_get_chk_info_av_mod(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i, j;
    SH_CHK_INFO info;       /* 模拟量信息 */
    U16 idx = 0;            /* 数据返回索引 */
    U16 *src_buf = NULL;    /* 指向源数据 */
    U08 model_id = 0;       /* 模块号 */

    /* 输入数据检查 */
    if (num*sizeof(SH_CHK_INFO) > MAX_TRAS_BYTES/* || offset+num > SH_MAX_CHK_INFO*/)//通讯缓存超界
    {
        return FALSE;
    }

    model_id = offset;

    for (i=0; i<num; i++)
    {
        /* *************************更新数据************************* */
        /* 模块 */
        SHC_get_mod_info(&info, model_id, i);

        /* ********************************************************** */

        /* 赋返回值 */
        src_buf = (U16*)&info;
        for (j=0; j<SH_CHK_MOD_AV_SIZE; j++)
        {
            dest_buf[idx++] = HIBYTE(src_buf[j]);
            dest_buf[idx++] = LOBYTE(src_buf[j]);
        }
    }

    return TRUE;
}

/*******************************************************************************
 函 数: SH_get_chk_info_av_sys()
 功 能: 获取当前系统模拟量显示信息
 参 数: dest_buf--数据返回缓存;
        offset--起始位置;
        num--模拟量数;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_get_chk_info_av_sys(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i, j;
    SH_CHK_INFO info;       /* 模拟量信息 */
    U16 idx = 0;            /* 数据返回索引 */
    U16 *src_buf = NULL;    /* 指向源数据 */

    /* 输入数据检查 */
    if (num*sizeof(SH_CHK_INFO) > MAX_TRAS_BYTES/* || offset+num > SH_MAX_CHK_INFO*/)//通讯缓存超界
    {
        return FALSE;
    }

    for (i=offset; i<offset+num; i++)
    {
        /* *************************更新数据************************* */

        /* 系统 */
        if (i < SHC_get_sys_av_num())
        {
            SHC_get_sys_info(&info, i);
        }

        /* ********************************************************** */

        /* 赋返回值 */
        src_buf = (U16*)&info;
        for (j=0; j<SH_CHK_SYS_AV_SIZE; j++)
        {
            dest_buf[idx++] = HIBYTE(src_buf[j]);
            dest_buf[idx++] = LOBYTE(src_buf[j]);
        }
    }

    return TRUE;
}

/*******************************************************************************
 函 数: SH_get_chk_info_av_comp()
 功 能: 获取当前压机模拟量显示信息
 参 数: dest_buf--数据返回缓存;
        offset--起始位置;
        num--模拟量数;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_get_chk_info_av_comp(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i, j;
    SH_CHK_INFO info;       /* 模拟量信息 */
    U16 idx = 0;            /* 数据返回索引 */
    U16 *src_buf = NULL;    /* 指向源数据 */

    /* 输入数据检查 */
    if (num*sizeof(SH_CHK_INFO) > MAX_TRAS_BYTES/* || offset+num > SH_MAX_CHK_INFO*/)//通讯缓存超界
    {
        return FALSE;
    }

    for (i=offset; i<offset+num; i++)
    {
        U16 pcb = i / SHC_COMP_PV_STEP;
        U16 pvi = i % SHC_COMP_PV_STEP;      /* 模块内的第几个模拟量 */
        info.value = SENSOR_NOEXIST;        /* 初始化为没有 */

        if (pcb < SH_MAX_PCB_NUM)
        {
            SHC_get_comp_info(&info, pcb, pvi);
        }
        /* ********************************************************** */

        /* 赋返回值 */
        src_buf = (U16*)&info;
        for (j=0; j<SH_CHK_COMP_AV_SIZE; j++)
        {
            dest_buf[idx++] = HIBYTE(src_buf[j]);
            dest_buf[idx++] = LOBYTE(src_buf[j]);
        }
    }

    return TRUE;
}


/*******************************************************************************
 函 数: SH_get_chk_info_do()
 功 能: 获取当前开关量输出显示信息
 参 数: dest_buf--数据返回缓存;
        offset--起始位置;
        num--开关量数;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_get_chk_info_do(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i, j;
    SH_CHK_INFO info;       /* 开关量信息 */
    U16 idx = 0;            /* 数据返回索引 */
    U16 *src_buf = NULL;    /* 指向源数据 */

    /* 输入数据检查 */
    if (num*sizeof(SH_CHK_INFO) > MAX_TRAS_BYTES/* || offset+num > SH_MAX_CHK_INFO*/)//通讯缓存超界
    {
        return FALSE;
    }


    for (i=offset; i<offset+num; i++)
    {
        U16 pcb = i / SHC_DO_STEP;
        U16 doi = i % SHC_DO_STEP;

        info.value = SENSOR_NOEXIST;        /* 初始化为没有 */

        if (pcb < SH_MAX_PCB_NUM)
        {
            SHC_get_do_info(&info, pcb, doi);
        }
        /* ********************************************************** */

        /* 赋返回值 */
        src_buf = (U16*)&info;
        for (j=0; j<SH_CHK_DO_SIZE; j++)
        {
            dest_buf[idx++] = HIBYTE(src_buf[j]);
            dest_buf[idx++] = LOBYTE(src_buf[j]);
        }
    }
    return TRUE;
}

/*******************************************************************************
 函 数: SH_get_chk_info_di()
 功 能: 获取当前开关量输出显示信息
 参 数: dest_buf--数据返回缓存;
        offset--起始位置;
        num--开关量数;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_get_chk_info_di(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i, j;
    SH_CHK_INFO info;       /* 开关量信息 */
    U16 idx = 0;            /* 数据返回索引 */
    U16 *src_buf = NULL;    /* 指向源数据 */

    /* 输入数据检查 */
    if (num*sizeof(SH_CHK_INFO) > MAX_TRAS_BYTES/* || offset+num > SH_MAX_CHK_INFO*/)//通讯缓存超界
    {
        return FALSE;
    }


    for (i=offset; i<offset+num; i++)
    {
        U16 pcb = i / SHC_DI_STEP;
        U16 dii = i % SHC_DI_STEP;

        info.value = SENSOR_NOEXIST;        /* 初始化为没有 */

        if (pcb < SH_MAX_PCB_NUM)
        {
            SHC_get_di_info(&info, pcb, dii);
        }
        /* ********************************************************** */

        /* 赋返回值 */
        src_buf = (U16*)&info;
        for (j=0; j<SH_CHK_DI_SIZE; j++)
        {
            dest_buf[idx++] = HIBYTE(src_buf[j]);
            dest_buf[idx++] = LOBYTE(src_buf[j]);
        }
    }
    return TRUE;
}

/*******************************************************************************
 函 数: SH_get_defrost_info()
 功 能:
 参 数: dest_buf--;
        offset--;
        num--;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_get_defrost_info(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i, j;
    SH_DEFRO_INFO info;     /* 开关量信息 */
    U16 idx = 0;            /* 数据返回索引 */
    U16 *src_buf = NULL;    /* 指向源数据 */

    /* 输入数据检查 */
    if (num*sizeof(SH_DEFRO_INFO) > MAX_TRAS_BYTES/* || offset+num > SH_MAX_CHK_INFO*/)//通讯缓存超界
    {
        return FALSE;
    }

    for (i=offset; i<offset+num; i++)
    {
        U16 pcb = i;

        /* 初始化 */
        info.cell = 0;
        info.cms = 0;

        if (pcb < MAX_PCB)
        {
            SHC_get_defro_info(&info, pcb);
        }
        /* ********************************************************** */

        /* 赋返回值 */
        src_buf = (U16*)&info;
        for (j=0; j<DEFR_INFO_SIZE; j++)
        {
            dest_buf[idx++] = HIBYTE(src_buf[j]);
            dest_buf[idx++] = LOBYTE(src_buf[j]);
        }
    }

    return TRUE;
}

/*******************************************************************************
 函 数: SH_get_start_code_tips()
 功 能:
 参 数: dest_buf--;
        offset--;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_get_start_code_tips(U08 *dest_buf, U16 offset)
{
    SHC_get_start_code_tips_string(dest_buf, offset);

    return TRUE;
}

/*******************************************************************************
 函 数: SH_get_enable_info()
 功 能: 获取使用/禁用信息
 参 数: dest_buf--数据返回缓存;
        offset--起始位置;
        num--信息结构体数;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
 移 植:
*******************************************************************************/
BOOL SH_get_enable_info(U08 *dest_buf, U16 offset, U16 num)
{
    SH_ENABLE_INFO info;               //开关量信息
    U16 *src_buf = NULL;            //指向源数据
    U16 idx = 0;
    U16 n = 0;

    //输入数据检查
    if (num*sizeof(SH_ENABLE_INFO) > MAX_TRAS_BYTES/* || offset+num > SH_MAX_CHK_INFO*/)//通讯缓存超界
    {
        return FALSE;
    }

    VALCLR(info);

    SHC_get_enable(&info);

    //赋返回值
    src_buf = &((U16*)&info)[offset];
    for (idx=0,n=0; n<num; n++)
    {
        dest_buf[idx++] = HIBYTE(src_buf[n]);
        dest_buf[idx++] = LOBYTE(src_buf[n]);
    }


    return TRUE;
}

/*******************************************************************************
 函 数: SH_get_chk_user_info()
 功 能: 用户查询信息
 参 数: dest_buf--;
        offset--;
        num--;
 返 回:
 其 它: 空
*******************************************************************************/
BOL SH_get_chk_user_info(U08 *dest_buf, U16 offset, U16 num)
{
    U08 i, j;
    U16 idx = 0;            /* 数据返回索引 */
    U16 *src_buf = NULL;    /* 指向源数据 */
    SH_CHK_INFO info;

    /* 输入数据检查 */
    if (num*sizeof(SH_CHK_INFO) > MAX_TRAS_BYTES)
    {
        return FALSE;
    }

    for (i=offset; i<offset+num; i++)
    {
        SHC_get_user_info(&info, i);

        /* 赋返回值 */
        src_buf = (U16*)&info;
        for (j=0; j<SH_CHK_USER_SIZE; j++)
        {
            dest_buf[idx++] = HIBYTE(src_buf[j]);
            dest_buf[idx++] = LOBYTE(src_buf[j]);
        }
    }

    return TRUE;
}

/*******************************************************************************
 函 数: SH_get_invt_para()
 功 能:
 参 数: dest_buf--;
        data_num--;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_get_invt_para(U08 *dest_buf, U16 data_num)
{
    SH_INVT_PARA info;
    U16 *src_buf = PNULL;
    U16 idx = 0;
    U16 n = 0;

    /* 输入数据检查 */
    if (data_num != SH_INVTPARA_SIZE)   /* 读取个数不一致 */
    {
        return FALSE;
    }

    VALCLR(info);

    SHC_get_invt_para(&info);

    //赋返回值
    src_buf = (U16*)&info;
    for (idx=0,n=0; n<data_num; n++)
    {
        dest_buf[idx++] = HIBYTE(src_buf[n]);
        dest_buf[idx++] = LOBYTE(src_buf[n]);
    }

    return TRUE;
}

/*******************************************************************************
 函 数: SH_get_floor_preheat_info()
 功 能:
 参 数: dest_buf--;
		offset--;
		num--;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_get_floor_preheat_info(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i, j;
    SH_PARA_ATTRIB info;       /* 模拟量信息 */
    U16 idx = 0;            /* 数据返回索引 */
    U16 *src_buf = NULL;    /* 指向源数据 */

    /* 输入数据检查 */
    if (num*sizeof(SH_PARA_ATTRIB) > MAX_TRAS_BYTES)//通讯缓存超界
    {
        return FALSE;
    }

    for (i=offset; i<offset+num; i++)
    {
        /* *************************更新数据************************* */

        SHC_get_floor_preheat_info(&info, i);

        /* ********************************************************** */

        /* 赋返回值 */
        src_buf = (U16*)&info;
        for (j=0; j<SH_FLOOR_PREHEAT_SIZE; j++)
        {
            dest_buf[idx++] = HIBYTE(src_buf[j]);
            dest_buf[idx++] = LOBYTE(src_buf[j]);
        }
    }

    return TRUE;
}

/*******************************************************************************
 函 数: SH_get_floor_timer_info()
 功 能:
 参 数: dest_buf--;
		offset--;
		num--;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_get_floor_timer_info(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i, j;
    SH_PARA_ATTRIB info;       /* 模拟量信息 */
    U16 idx = 0;            /* 数据返回索引 */
    U16 *src_buf = NULL;    /* 指向源数据 */

    /* 输入数据检查 */
    if (num*sizeof(SH_PARA_ATTRIB) > MAX_TRAS_BYTES)//通讯缓存超界
    {
        return FALSE;
    }

    for (i=offset; i<offset+num; i++)
    {
        /* *************************更新数据************************* */

        SHC_get_floor_timer_info(&info, i);

        /* ********************************************************** */

        /* 赋返回值 */
        src_buf = (U16*)&info;
        for (j=0; j<SH_FLOOR_PREHEAT_SIZE; j++)
        {
            dest_buf[idx++] = HIBYTE(src_buf[j]);
            dest_buf[idx++] = LOBYTE(src_buf[j]);
        }
    }

    return TRUE;
}

/*******************************************************************************
 函 数: SH_get_test_fac_hmi_info()
 功 能: 获取显示屏商检信息
 参 数: dest_buf--数据返回缓存;
        offset--起始位置;
        num--读取数据个数;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_get_test_fac_hmi_info(U08 *dest_buf, U16 offset, U16 num)
{
    U16 i;
    SH_TEST_FAC_HMI_STRU info;         /* 风盘信息 */
    U16 idx = 0;            /* 数据返回索引 */
    U16 *src_buf = NULL;

    /* 输入数据检查 */
    if (offset+num > SH_TEST_FAC_HMI_RD_SIZE) /* 通讯缓存超界 */
    {
        return FALSE;
    }

    VALCLR(info);
    // 赋值
    SHC_get_test_fac_hmi_info(&info);

    /* 赋返回值 */
    src_buf = (U16 *)&info + offset;
    for(i=0; i<SH_TEST_FAC_HMI_RD_SIZE; i++)
    {
        dest_buf[idx++] = HIBYTE(src_buf[i]);
        dest_buf[idx++] = LOBYTE(src_buf[i]);
    }

    return TRUE;
}

/*******************************************************************************
 函 数: SH_set_command()
 功 能: 响应收到的命令
 参 数: src_buf--数据源;
        offset--起始位置;
        num--数据个数(字);
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_set_command(U08 *src_buf, U16 offset, U16 num)
{
    I16 command;            /* 命令信息 */

    /* 输入数据检查 */
    if (num != 1 || offset+num > SH_MAX_CMD)
    {
        return FALSE;
    }

    /* 赋值 */
    command = MAKEWORD(src_buf[0], src_buf[1]);

    /* *************************更新数据************************* */
    switch (offset)
    {
        /* 0 开机 */
        case SH_CMD_ON:
            SHC_command_ON_deal(command);
            break;

        /* 1 关机 */
        case SH_CMD_OFF:
            SHC_command_OFF_deal(command);
            break;

        /* 2 故障复位 */
        case SH_CMD_RST:
            SHC_command_RST_deal(command);
            break;

        /* 3 密码故障复位 */
        case SH_CMD_PASS_RST:
            SHC_command_PASS_RST_deal(command);
            break;

        /* 4 取消预热 */
        case SH_CMD_OFF_OILHEAT:
            SHC_command_OFF_OILHEAT_deal(command);
            break;

        /* 5 模块手动除霜 */
        case SH_CMD_DEFRO:
            SHC_command_DEFRO_deal(command);
            break;

        /* 6 参数初始化 */
        case SH_CMD_PARA_INIT:
            SHC_command_PARA_INIT_deal(command);
            break;

        /* 7 压机累计运行时间初始化 */
        case SH_CMD_CM_RUNTM_INIT:
            SHC_command_CM_RUNTM_INIT_deal(command);
            break;

        /* 8 系统累计运行时间初始化 */
        case SH_CMD_SYS_RUNTM_INIT:
            SHC_command_SYS_RUNTM_INIT_deal(command);
            break;

        /* 9 清空历史故障 */
        case SH_CMD_HIS_ERR_INIT:
            SHC_command_HIS_ERR_INIT_deal(command);
            break;

        /* 10 变频器初始化 */
        case SH_CMD_INVERTER_INIT:
            SHC_command_INVERTER_INIT_deal(command);
            break;

        /* 11 手动开关水泵 */
        case SH_CMD_PUMP:
            SHC_command_PUMP_deal(command);
            break;

        /* 12 BMS参数设置 */
        case SH_CMD_BMS_PARA_SET:
            SHC_command_BMS_PARA_deal(command);
            break;

        /* 13 快速预热 */
        case SH_CMD_QUICK_OILHEAT:
            SHC_command_QUICK_OILHEAT_deal(command);
            break;

        /* 14 涂鸦重新连接标志 */
        case SH_CMD_TUYA_NEW_LINK:
            SHC_command_TUYA_NEW_LINK_deal(command);
            break;

        /* 16 进入地暖烘干功能 */
        case SH_CMD_ENTER_FLOOR_DRYUP:
            SHC_command_enter_floor_dryup_deal(command);
            break;

        /* 17 强开辅助电热 */
        case SH_CMD_FORCE_IBH:
            SHC_command_force_ibh_deal(command);
            break;

        /* 18 强开水箱电热 */
        case SH_CMD_FORCE_TBH:
            SHC_command_force_tbh_deal(command);
            break;

        /* 19 强开热水模式 */
        case SH_CMD_FORCE_HW_MODE:
            SHC_command_force_hw_mode_deal(command);
            break;

        /* 20 能力测试 */
        case SH_CMD_ABILITY_TEST:
            SHC_command_ability_test_deal(command);
            break;

        /* 21 区域一开机 */
        case SH_CMD_ON_ZONE_1:
            SHC_command_ON_zone_1_deal(command);
            break;

        /* 22 区域一关机 */
        case SH_CMD_OFF_ZONE_1:
            SHC_command_OFF_zone_1_deal(command);
            break;

        /* 23 区域二开机 */
        case SH_CMD_ON_ZONE_2:
            SHC_command_ON_zone_2_deal(command);
            break;

        /* 24 区域二关机 */
        case SH_CMD_OFF_ZONE_2:
            SHC_command_OFF_zone_2_deal(command);
            break;

        /* 25 热水开机 */
        case SH_CMD_ON_HOTW:
            SHC_command_ON_hotwater_deal(command);
            break;

        /* 26 热水关机 */
        case SH_CMD_OFF_HOTW:
            SHC_command_OFF_hotwater_deal(command);
            break;

        /* 27 系统维护初始化 */
        case SH_CMD_LIMIT_INIT:
            SHC_command_limit_init_deal(command);
        	break;

        /* 28 收氟功能 */
        case SH_CMD_RECYCLE_REFRI:
            SHC_command_recyle_refri_deal(command);
        	break;

        /* 29 定时开热水泵 */
        case SH_CMD_TIMER_DHW_ON:
            SHC_command_TIMER_DHW_ON_deal(command);
        	break;

        /* 30 强开外部热源 */
        case SH_CMD_FORCE_AHS:
            SHC_command_force_AHS_deal(command);
        	break;

        /* 31 杀菌 */
        case SH_CMD_DISINFECT:
            SHC_command_disinfect_deal(command);
        	break;

        /* 31 静音模式 */
        case SH_CMD_SILENT:
            SHC_command_silent_deal(command);
        	break;

        /* 33 水泵试运行 */
        case SH_CMD_PUMP_TEST:
            SHC_command_pump_test_deal(command);
        	break;

        /* 34 制冷试运行 */
        case SH_CMD_COOL_TEST:
            SHC_command_cool_test_deal(command);
        	break;

        /* 35 制热试运行 */
        case SH_CMD_HEAT_TEST:
            SHC_command_heat_test_deal(command);
        	break;

        /* 36 制热水试运行 */
        case SH_CMD_HOTWATER_TEST:
            SHC_command_hotwater_test_deal(command);
        	break;

        /* 37 系统排空调试 */
        case SH_CMD_SYSTEM_EMPTY:
            SHC_command_system_empty_deal(command);
        	break;

        /* 38 地暖预热 */
        case SH_CMD_FLOOR_PREHEAT:
            SHC_command_floor_preheat_deal(command);
        	break;

        /* 39 定时地暖 */
        case SH_CMD_FLOOR_TIMER:
            SHC_command_floor_timer_deal(command);
        	break;

        /* 40 假日离开 */
        case SH_CMD_HOLIDAY_AWAY:
            SHC_command_holiday_away_deal(command);
        	break;

        /* 41 假日在家 */
        case SH_CMD_HOLIDAY_HOME:
            SHC_command_holiday_home_deal(command);
        	break;

        /* 42 接受远程升级请求 */
        case SH_CMD_ACCEPT_REMOTE_UPGRADE_REQ:
            SHC_command_accept_remote_upgrade_req_deal(command);
        	break;

        default:
            break;
    }
    /* ********************************************************** */

    return TRUE;
}

/*******************************************************************************
 函 数: SH_set_setting()
 功 能: 响应新设定
 参 数: src_buf--数据源;
        offset--起始位置;
        num--数据个数(字);
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_set_setting(U08 *src_buf, U16 offset, U16 num)
{
    I16 set_data;           /* 设置信息 */
    BOOL result = TRUE;     /* 设置结果 */

    /* 输入数据检查 */
    if (num != 1 || offset+num > SH_MAX_SET_INFO)
    {
        return FALSE;
    }

    /* 赋值 */
    set_data = MAKEWORD(src_buf[0], src_buf[1]);

    /* *************************更新数据************************* */
    switch (offset)
    {
        /* 0 目标温度 */
        case SET_TMP_SET:
            result = SHC_set_tmp_deal(set_data);
            break;

        /* 1 设定模式 */
        case SET_MODE_SET:
            result = SHC_set_mode_deal(set_data);
            break;

        /* 2 使用期限 */
        case SET_LIMIT_TIME:
            result = SHC_set_limit_time_deal(set_data);
            break;

        /* 3 定时使用(TRUE:使用,FALSE:不用) */
        case SET_TIMING_USE:
            result = SHC_set_timing_use_deal(set_data);
            break;

        /* 4 清除开机密码功能 */
        case SET_START_CODE:
            result = SHC_set_start_code_deal(set_data);
            break;

        /* 5 设定湿度模式 */
        case SET_HUMI_MODE:
            result = SHC_set_humi_mode_deal(set_data);
            break;

        /* 6 设定语言 */
        case SET_LANGUAGE:
            result = SHC_set_language_deal(set_data);
            break;

        /* 7 区域一目标温度 */
        case SET_TMP_SET_ZONE_1:
            result = SHC_set_tmp_zone_1_deal(set_data);
            break;

        /* 8 区域二目标温度 */
        case SET_TMP_SET_ZONE_2:
            result = SHC_set_tmp_zone_2_deal(set_data);
            break;

        /* 9 热水目标温度 */
        case SET_TMP_SET_ZONE_HOTW:
            result = SHC_set_tmp_zone_hotw_deal(set_data);
            break;

        /* 10 设置维护类型 */
        case SET_LIMIT_TYPE:
            result = SHC_set_lmt_type_deal(set_data);
            break;

        /* 11 设置静音级别 */
        case SET_SILENT_LEVEL:
            result = SHC_set_silent_level_deal(set_data);
            break;

        /* 12 设置温度单位 */
        case SET_TEMP_UNIT:
            result = SHC_set_temp_unit_deal(set_data);
            break;

        default:
            result = FALSE;
            break;
    }
    /* ********************************************************** */

    return result;
}

/*******************************************************************************
 函 数: SH_set_passwrd()
 功 能: 响应收到的命令
 参 数: src_buf--数据源;
        offset--起始位置;
        num--数据个数(字);
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_set_passwrd(U08 *src_buf, U16 offset, U16 num)
{
    U08 i;
    U08 idx = 0;            /* 数据读取索引 */
    SH_PASS passwrd;        /* 密码 */
    U16 pass_grp = 0;       /* 密码组 */
    U16 *passwrd_dest = NULL;               /* 指向密码数组 */

    /* 输入数据检查 */
    if (num*sizeof(SH_PASS) > MAX_TRAS_BYTES || offset+num > SH_MAX_PASS_GRP)   /* 通讯缓存超界 */
    {
        return FALSE;
    }

    for (pass_grp=offset; pass_grp<offset+num; pass_grp++)
    {
        U16 data_offset = sizeof(SH_PASS) * (pass_grp - offset);    /* 数据偏移 */
        /* 赋值 */
        passwrd.authority = MAKEWORD(src_buf[data_offset], src_buf[data_offset+1]);
        for (i=0; i<MAX_PASS_BIT; i++)
        {
            U16 pwd_offset = data_offset+2;
            passwrd.pass_bit[i] = src_buf[pwd_offset+i];
        }

        /* *************************更新数据************************* */

        /* 根据权限判断密码组 */
        switch (passwrd.authority)
        {
            case SH_PWR_SALE:
                passwrd_dest = (U16 *)&password[PASS_LIMIT][0];
                SETBIT(passwd_modify, PASS_LIMIT);
                break;
            case SH_PWR_USER:
                passwrd_dest = (U16 *)&password[PASS_USER][0];
                SETBIT(passwd_modify, PASS_USER);
                break;
            case SH_PWR_PRO:
                passwrd_dest = (U16 *)&password[PASS_PRO][0];
                SETBIT(passwd_modify, PASS_PRO);
                break;
            case SH_PWR_SER:
                passwrd_dest = (U16 *)&password[PASS_SER][0];
                SETBIT(passwd_modify, PASS_SER);
                break;
            case SH_PWR_FAC:
                passwrd_dest = (U16 *)&password[PASS_FAC][0];
                SETBIT(passwd_modify, PASS_FAC);
                break;

            default:
                passwrd_dest = (U16 *)NULL;
                return FALSE;
        }

        /* 保存新密码 */
        if (passwrd_dest != (U16 *)NULL)
        {
            idx = 0;
            for (i=0; i<MAX_PASS_BIT/2; i++)
            {
                if (fg_pass_big_endian == TRUE)
                {
                    passwrd_dest[i] = MAKEWORD(passwrd.pass_bit[idx], passwrd.pass_bit[idx+1]);
                }
                else
                {
                    passwrd_dest[i] = MAKEWORD(passwrd.pass_bit[idx+1], passwrd.pass_bit[idx]);
                }
                idx += 2;
            }
        }
        /* ********************************************************** */

    }

    return TRUE;
}

/*******************************************************************************
 函 数: SH_set_clock()
 功 能: 更新时间
 参 数: src_buf--数据源;
        offset--起始位置;
        num--数据个数(字);
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
 移 植: 更新为前板下发的时间。
*******************************************************************************/
BOOL SH_set_clock(U08 *src_buf, U16 offset, U16 num)
{
    U08 i;
    SH_CLOCK time_buff;     /* 时钟接受缓存区 */
    U08 *dest_buf = NULL;

    /* 输入数据检查 */
    if (num > MAX_TRAS_BYTES/2 || offset+num > SH_CLOCK_SIZE)
    {
        return FALSE;
    }

    /* 赋值 */
    dest_buf = (U08*)&time_buff;
    for (i=0; i<sizeof(SH_CLOCK)/2; i++)    /* 八位字符调换高低字节顺序 */
    {
        dest_buf[2*i+0] = src_buf[2*i+1];
        dest_buf[2*i+1] = src_buf[2*i+0];
    }

    /* *************************更新数据************************* */
    SHC_set_clk_deal(&time_buff);

    /* ********************************************************** */

    return TRUE;
}

/*******************************************************************************
 函 数: SH_set_holidayinfo()
 功 能: 设置假日模式数据
 参 数: src_buf--数据源;
        data_num--数据个数(字);
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
 移 植: 更新为前板下发的时间。
*******************************************************************************/
BOOL SH_set_holidayinfo(U08 *src_buf, U16 data_num)
{
    U08 i;
    SH_HOLIDAYINFO info_buff;     /* 接受缓存区 */
    U08 *dest_buf = NULL;

    /* 输入数据检查 */
    if (data_num != SH_HOLIDAYINFO_SIZE)
    {
        return FALSE;
    }

    /* 赋值 */
    dest_buf = (U08*)&info_buff;
    for (i=0; i<SH_HOLIDAYINFO_SIZE; i++)    /* 八位字符调换高低字节顺序 */
    {
        dest_buf[2*i+0] = src_buf[2*i+1];
        dest_buf[2*i+1] = src_buf[2*i+0];
    }

    /* *************************更新数据************************* */
    SHC_set_holidayinfo_deal(info_buff);

    /* ********************************************************** */

    return TRUE;
}

/*******************************************************************************
 函 数: SH_set_room_temp()
 功 能: 设置假日模式数据
 参 数: src_buf--数据源;
        data_num--数据个数(字);
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
 移 植: 更新为前板下发的时间。
*******************************************************************************/
BOOL SH_set_room_temp(U08 *src_buf, U16 data_num)
{
    U08 i;
    U08 *dest_buf = NULL;
    I16 info;

    /* 输入数据检查 */
    if (data_num != SH_ROOM_TEMP_SIZE)
    {
        return FALSE;
    }

    /* 赋值 */

    /* *************************更新数据************************* */
    dest_buf = (U08*)&info;
    for (i=0; i<SH_ROOM_TEMP_SIZE; i++)    /* 八位字符调换高低字节顺序 */
    {
        dest_buf[2*i+0] = src_buf[2*i+1];
        dest_buf[2*i+1] = src_buf[2*i+0];
    }

    SHC_set_room_temp_deal(info);
    /* ********************************************************** */

    return TRUE;
}

/*******************************************************************************
 函 数: SH_set_para()
 功 能: 保存新参数
 参 数: src_buf--数据源;
        offset--起始位置;
        num--数据个数(字);
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_set_para(U08 *src_buf, U16 offset, U16 num)
{
    U08 i;
    U08 grp_idx = 0;        /* 组号 */
    U08 item_idx = 0;       /* 项号 */
    I16 value = 0;

    /* 输入数据检查 */
    if (num > MAX_TRAS_BYTES/2 || num+offset > SH_MAX_PARA_ITEM_NUM*SH_MAX_PARA_GRP_NUM)/* 通讯缓存超界 */
    {
        return FALSE;
    }

    for (i=0; i<num; i++)
    {
        /* 计算组号及项号 */
        grp_idx = (U08)((offset+i) / SHC_DISP_GET_PARA_GRP_SIZE);
        item_idx = (U08)((offset+i) % SHC_DISP_GET_PARA_GRP_SIZE);
        value = MAKEWORD(src_buf[2*i], src_buf[2*i+1]);

        /* *************************更新数据************************* */
        SHC_set_para_deal(grp_idx, item_idx, value);
        /* ********************************************************** */
    }

    return TRUE;
}

/*******************************************************************************
 函 数: SH_set_test_wo()
 功 能: 设置调试只写数据
 参 数: src_buf--;
        offset--;
        num--;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_set_test_wo(U08 *src_buf, U16 offset, U16 num)
{
    U08 i;

    /* 输入数据检查 */
    if (num > MAX_TRAS_BYTES/2 || offset+num > SH_CHK_TEST_WO_SIZE)
    {
        return FALSE;
    }

    for (i=offset; i<offset+num; i++)
    {
        ((U16*)(&test_data.wo))[i] = MAKEWORD(src_buf[2*(i-offset)], src_buf[2*(i-offset)+1]);
    }

    return TRUE;
}

/*******************************************************************************
 函 数: SH_set_test_wr()
 功 能: 设置调试读写数据
 参 数: src_buf--;
        offset--;
        num--;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_set_test_wr(U08 *src_buf, U16 offset, U16 num)
{
    U08 i;

    /* 输入数据检查 */
    if (num > MAX_TRAS_BYTES/2 || offset+num > SH_CHK_TEST_WR_SIZE)
    {
        return FALSE;
    }

    for (i=offset; i<offset+num; i++)
    {
        ((U16*)(&test_data.wr))[i] = MAKEWORD(src_buf[2*(i-offset)], src_buf[2*(i-offset)+1]);
    }

    return TRUE;
}

/*******************************************************************************
 函 数: SH_set_pass_big_endian()
 功 能:
 参 数: 空
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_set_pass_big_endian(void)
{
    fg_pass_big_endian = TRUE;

    return TRUE;
}

/*******************************************************************************
 函 数: SH_set_invt_para()
 功 能:
 参 数: src_buf--;
        data_num--;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_set_invt_para(U08 *src_buf, U16 data_num)
{
    U08 i;
    SH_INVT_PARA info_buff;     /* 接受缓存区 */
    U08 *dest_buf = NULL;

    /* 输入数据检查 */
    if (data_num != SH_INVTPARA_SIZE)
    {
        return FALSE;
    }

    /* 赋值 */
    dest_buf = (U08*)&info_buff;
    for (i=0; i<SH_INVTPARA_SIZE; i++)    /* 八位字符调换高低字节顺序 */
    {
        dest_buf[2*i+0] = src_buf[2*i+1];
        dest_buf[2*i+1] = src_buf[2*i+0];
    }

    /* *************************更新数据************************* */
    SHC_set_invt_para_deal(info_buff);

    /* ********************************************************** */

    return TRUE;
}

/*******************************************************************************
 函 数: SH_set_floor_preheat_val()
 功 能:
 参 数: src_buf--;
		data_num--;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_set_floor_preheat_val(U08 *src_buf, U16 offset, U16 num)
{
    U08 i;
    U08 idx = 0;        /* 组号 */
    I16 value = 0;

    /* 输入数据检查 */
    if (num > MAX_TRAS_BYTES/2 || num+offset > SH_MAX_FLOOR_PREHEAT_INFO)/* 通讯缓存超界 */
    {
        return FALSE;
    }

    for (i=0; i<num; i++)
    {
        idx = offset + i;
        value = MAKEWORD(src_buf[2*i], src_buf[2*i+1]);

        /* *************************更新数据************************* */
        SHC_set_floor_preheat_val_deal(idx, value);
        /* ********************************************************** */
    }

    return TRUE;
}

/*******************************************************************************
 函 数: SH_set_floor_timer_val()
 功 能:
 参 数: src_buf--;
		data_num--;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_set_floor_timer_val(U08 *src_buf, U16 offset, U16 num)
{
    U08 i;
    U08 idx = 0;        /* 组号 */
    I16 value = 0;

    /* 输入数据检查 */
    if (num > MAX_TRAS_BYTES/2 || num+offset > SH_MAX_FLOOR_TIMER_INFO)/* 通讯缓存超界 */
    {
        return FALSE;
    }

    for (i=0; i<num; i++)
    {
        idx = offset + i;
        value = MAKEWORD(src_buf[2*i], src_buf[2*i+1]);

        /* *************************更新数据************************* */
        SHC_set_floor_timer_val_deal(idx, value);
        /* ********************************************************** */
    }

    return TRUE;
}

/*******************************************************************************
 函 数: SH_set_test_fac_hmi_info()
 功 能:
 参 数: src_buf--;
        data_num--;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL SH_set_test_fac_hmi_info(U08 *src_buf, U16 data_num)
{
    U08 i;
    SH_TEST_FAC_HMI_STRU info_buff;     /* 接受缓存区 */
    U08 *dest_buf = NULL;

    /* 输入数据检查 */
    if (data_num != SH_TEST_FAC_HMI_WR_SIZE)
    {
        return FALSE;
    }

    /* 赋值 */
    dest_buf = (U08*)&info_buff.wr;
    for (i=0; i<SH_TEST_FAC_HMI_WR_SIZE; i++)    /* 八位字符调换高低字节顺序 */
    {
        dest_buf[2*i+0] = src_buf[2*i+1];
        dest_buf[2*i+1] = src_buf[2*i+0];
    }

    /* *************************更新数据************************* */
    SHC_set_test_fac_hmi_info_deal(info_buff);

    /* ********************************************************** */

    return TRUE;
}

/****************************************************************
函数：SH_MB_rd_reg
功能：接收HMI发过来的读数据字请求
参数：RxBuf--接收缓冲区;
      TxBuf--发送缓冲区;
      pTxTotal--需发送的字节总数;
返回：成功/失败
其它：空
*****************************************************************/
U08 SH_MB_rd_reg(const U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
    U16 addr_offst = 0;     /* 数据起始位置(由数据地址决定) */
    U16 data_num = 0;
    U16 data_addr = 0;
    U08 com_err = SH_MB_NO_ERR;             /* 故障状态 */
    BOOL fg_rd_succ = FALSE;/* 调用外部子程序写入状态标志 */
    U16 cell_offst = 0;     /* 单元起始位置(从第几个结构体单元开始读) */
    U16 cell_num = 0;       /* 单元个数(读取几个结构体单元) */

    data_addr = MAKEWORD(RxBuf[2], RxBuf[3]);               /* 数据地址 */
    data_num = MAKEWORD(RxBuf[4], RxBuf[5]);/* 数据个数 */

    if (RxBuf[1] == 0x03)   /* 可读写 */
    {
        /* 下句注释只为消除和无符号数大于等于0的编译警告 */
        if (/*data_addr >= ADDR_CMD_BGN && */data_addr + data_num <= ADDR_SH_CMD_END)//命令
        {
            /* 不支持读命令 */
        }
        else if (data_addr >= ADDR_SH_SET_BGN && data_addr + data_num <= ADDR_SH_SET_END)   /* 设置 */
        {
            /* 不支持读设置 */
        }
        else if (data_addr >= ADDR_SH_PASS_BGN && data_addr + data_num <= ADDR_SH_PASS_END) /* 密码 */
        {
            addr_offst = data_addr - ADDR_SH_PASS_BGN;
            cell_offst = addr_offst / SH_PASS_SIZE;
            cell_num = data_num / SH_PASS_SIZE;
            fg_rd_succ = SH_get_passwrd(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_CLOCK_BGN && data_addr + data_num <= ADDR_SH_CLOCK_END)   /* 时钟 */
        {
            /* 暂不支持读时钟 */
        }
        else if (data_addr >= ADDR_SH_HOLIDAYINFO_BGN && data_addr + data_num <= ADDR_SH_HOLIDAYINFO_END)   /* 假日模式 */
        {
            fg_rd_succ = SH_get_holidayinfo(&TxBuf[3], data_num);
        }
        else if (data_addr >= ADDR_SH_PARA_BGN && data_addr + data_num <= ADDR_SH_PARA_END) /* 参数值 */
        {
            /* 参数值暂从参数属性中读 */
        }
        else if (data_addr >= ADDR_SH_CHK_TEST_WR_BGN && data_addr + data_num <= ADDR_SH_CHK_TEST_WR_END)   /* 调试字符串等 */
        {
            addr_offst = data_addr - ADDR_SH_CHK_TEST_WR_BGN;
            cell_offst = addr_offst;
            cell_num = data_num;
            fg_rd_succ = SH_get_test_wr(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_TUYA_INFO_BGN && data_addr + data_num <= ADDR_SH_TUYA_INFO_END)
        {
            fg_rd_succ = SHC_get_tuya_info(&TxBuf[3]);
        }
        else if (data_addr >= ADDR_SH_TUYA_DATA_BGN && data_addr + data_num <= ADDR_SH_TUYA_DATA_END)
        {
            fg_rd_succ = SHC_get_tuya_data(&TxBuf[3], data_addr);
        }
        else if (data_addr >= ADDR_SH_INVTPARA_BGN && data_addr + data_num <= ADDR_SH_INVTPARA_END)
        {
            fg_rd_succ = SH_get_invt_para(&TxBuf[3], data_num);
        }
		else if (data_addr >= ADDR_SH_TEST_FAC_HMI_RD_BGN && data_addr + data_num <= ADDR_SH_TEST_FAC_HMI_RD_END)
        {
            addr_offst = data_addr - ADDR_SH_TEST_FAC_HMI_RD_BGN;
            fg_rd_succ = SH_get_test_fac_hmi_info(&TxBuf[3], addr_offst, data_num);
        }

    }
    else                    /* 只读 */
    {
        if (data_addr >= SH_ADDR_STRING_START_CODE_BGN && data_addr + data_num <= SH_ADDR_STRING_START_CODE_END)
        {
            addr_offst = data_addr - SH_ADDR_STRING_START_CODE_BGN;
            cell_offst = addr_offst / STRING_START_CODE_SIZE;
            fg_rd_succ = SH_get_start_code_tips(&TxBuf[3], cell_offst);
        }
        else if (data_addr >= SH_ADDR_ENABLE_INFO_BGN && data_addr + data_num <= SH_ADDR_ENABLE_INFO_END)
        {
            addr_offst = data_addr - SH_ADDR_ENABLE_INFO_BGN;
            fg_rd_succ = SH_get_enable_info(&TxBuf[3], addr_offst, data_num);
        }
        else if (data_addr >= SH_ADDR_DEFR_INFO_BGN && data_addr + data_num <= SH_ADDR_DEFR_INFO_END)   /* 除霜信息 */
        {
            addr_offst = data_addr - SH_ADDR_DEFR_INFO_BGN;
            cell_offst = addr_offst / DEFR_INFO_SIZE;
            cell_num = data_num / DEFR_INFO_SIZE;
            fg_rd_succ = SH_get_defrost_info(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_DISP_BGN && data_addr + data_num <= ADDR_SH_DISP_END) /* 显示辅助信息 */
        {
            addr_offst = data_addr - ADDR_SH_DISP_BGN;
            cell_offst = addr_offst;
            cell_num = data_num;
            fg_rd_succ = SH_get_disp_info(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_CHK_NUM_BGN && data_addr + data_num <= ADDR_SH_CHK_NUM_END)/* 查询信息个数 */
        {
            addr_offst = data_addr - ADDR_SH_CHK_NUM_BGN;
            cell_offst = addr_offst / SH_CHK_NUM_SIZE;
            cell_num = data_num / SH_CHK_NUM_SIZE;
            fg_rd_succ = SH_get_chk_info_num(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_CHK_TEST_RO_BGN && data_addr + data_num <= ADDR_SH_CHK_TEST_RO_END)   /* 调试字符串等 */
        {
            addr_offst = data_addr - ADDR_SH_CHK_TEST_RO_BGN;
            cell_offst = addr_offst;
            cell_num = data_num;
            fg_rd_succ = SH_get_test_ro(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_VER_BGN && data_addr + data_num <= ADDR_SH_VER_END)   /* 版本号 */
        {
            addr_offst = data_addr - ADDR_SH_VER_BGN;
            cell_offst = addr_offst / SH_VER_SIZE;
            cell_num = data_num / SH_VER_SIZE;
            fg_rd_succ = SH_get_version(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_ERR_CRT_BGN && data_addr + data_num <= ADDR_SH_ERR_CRT_END)/* 故障 当前 */
        {
            addr_offst = data_addr - ADDR_SH_ERR_CRT_BGN;
            cell_offst = addr_offst / SH_ERR_CRT_SIZE;
            cell_num = data_num / SH_ERR_CRT_SIZE;
            fg_rd_succ = SH_get_curt_error(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_ERR_HIS_BGN && data_addr + data_num <= ADDR_SH_ERR_HIS_END)/* 故障 历史 */
        {
            addr_offst = data_addr - ADDR_SH_ERR_HIS_BGN;
            cell_offst = addr_offst / SH_ERR_HIS_SIZE;
            cell_num = data_num / SH_ERR_HIS_SIZE;
            fg_rd_succ = SH_get_his_error(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_FCUINFO_BGN && data_addr + data_num <= ADDR_SH_FCUINFO_END)/* 风盘信息 */
        {
            addr_offst = data_addr - ADDR_SH_FCUINFO_BGN;
            fg_rd_succ = SH_get_fcuinfo(&TxBuf[3], addr_offst, data_num);
        }
        else if (data_addr >= ADDR_SH_PARA_ATTRIB_BGN && data_addr + data_num <= ADDR_SH_PARA_ATTRIB_END)   /* 参数项属性 */
        {
            addr_offst = data_addr - ADDR_SH_PARA_ATTRIB_BGN;
            cell_offst = addr_offst / SH_PARA_ATTRIB_SIZE;
            cell_num = data_num / SH_PARA_ATTRIB_SIZE;
            fg_rd_succ = SH_get_para_item_info(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_PCB_COMP_NUM_BGN && data_addr + data_num <= ADDR_SH_PCB_COMP_NUM_END) /* 压机个数 */
        {
            addr_offst = data_addr - ADDR_SH_PCB_COMP_NUM_BGN;
            cell_offst = addr_offst / SH_PCB_COMP_NUM_SIZE;
            cell_num = data_num / SH_PCB_COMP_NUM_SIZE;
            fg_rd_succ = SH_get_pcb_comp_num(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_BMS_PARA_BGN && data_addr + data_num <= ADDR_SH_BMS_PARA_END) /* BMS通讯设置参数 */
        {
            addr_offst = data_addr - ADDR_SH_BMS_PARA_BGN;
            cell_offst = addr_offst / SH_BMS_PARA_SIZE;
            cell_num = data_num / SH_BMS_PARA_SIZE;
            fg_rd_succ = SH_get_bms_para_info(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_PARA_GRP_BGN && data_addr + data_num <= ADDR_SH_PARA_GRP_END) /* 参数组信息 */
        {
            addr_offst = data_addr - ADDR_SH_PARA_GRP_BGN;
            cell_offst = addr_offst / SH_PARA_GRP_SIZE;
            cell_num = data_num / SH_PARA_GRP_SIZE;
            fg_rd_succ = SH_get_para_grp_info(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_PARA_STRING_BGN && data_addr + data_num <= ADDR_SH_PARA_STRING_END)/* 参数名称字符串 */
        {
            addr_offst = data_addr - ADDR_SH_PARA_STRING_BGN;
            cell_offst = addr_offst / SH_PARA_STRING_SIZE;
            cell_num = data_num / SH_PARA_STRING_SIZE;
            fg_rd_succ = SH_get_dict_string(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_STR_ATTR_BGN && data_addr + data_num <= ADDR_SH_STR_ATTR_END) /* 参数字符串属性 */
        {
            addr_offst = data_addr - ADDR_SH_STR_ATTR_BGN;
            cell_offst = addr_offst / SH_STR_ATTR_PER_SIZE;
            cell_num = data_num / SH_STR_ATTR_PER_SIZE;
            fg_rd_succ = SH_get_dict_attrib(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_CHK_SYS_AV_BGN && data_addr + data_num <= ADDR_SH_CHK_SYS_AV_END) /* 查询信息 系统单元模拟量 */
        {
            addr_offst = data_addr - ADDR_SH_CHK_SYS_AV_BGN;
            cell_offst = addr_offst / SH_CHK_SYS_AV_SIZE;
            cell_num = data_num / SH_CHK_SYS_AV_SIZE;
            fg_rd_succ = SH_get_chk_info_av_sys(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_CHK_MOD_AV_BGN && data_addr + data_num <= ADDR_SH_CHK_MOD_AV_END) /* 查询信息 系统单元模拟量 */
        {
            addr_offst = data_addr - ADDR_SH_CHK_MOD_AV_BGN;
            cell_offst = addr_offst / SH_CHK_MOD_AV_STEP;
            cell_num = data_num / SH_CHK_SYS_AV_SIZE;
            fg_rd_succ = SH_get_chk_info_av_mod(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_CHK_COMP_AV_BGN && data_addr + data_num <= ADDR_SH_CHK_COMP_AV_END)/* 查询信息 压机模拟量 */
        {
            addr_offst = data_addr - ADDR_SH_CHK_COMP_AV_BGN;
            cell_offst = addr_offst / SH_CHK_COMP_AV_SIZE;
            cell_num = data_num / SH_CHK_COMP_AV_SIZE;
            fg_rd_succ = SH_get_chk_info_av_comp(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_CHK_DO_BGN && data_addr + data_num <= ADDR_SH_CHK_DO_END) /* 查询信息 开关量 */
        {
            addr_offst = data_addr - ADDR_SH_CHK_DO_BGN;
            cell_offst = addr_offst / SH_CHK_DO_SIZE;
            cell_num = data_num / SH_CHK_DO_SIZE;
            fg_rd_succ = SH_get_chk_info_do(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_CHK_DI_BGN && data_addr + data_num <= ADDR_SH_CHK_DI_END) /* 查询信息 开关量输入 */
        {
            addr_offst = data_addr - ADDR_SH_CHK_DI_BGN;
            cell_offst = addr_offst / SH_CHK_DI_SIZE;
            cell_num = data_num / SH_CHK_DI_SIZE;
            fg_rd_succ = SH_get_chk_info_di(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_CHK_USER_BGN && data_addr + data_num <= ADDR_SH_CHK_USER_END)
        {
            addr_offst = data_addr - ADDR_SH_CHK_USER_BGN;
            cell_offst = addr_offst / SH_CHK_USER_SIZE;
            cell_num = data_num / SH_CHK_USER_SIZE;
            fg_rd_succ = SH_get_chk_user_info(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_VFD_VER_BGN && data_addr + data_num <= ADDR_SH_VFD_VER_END)   /* 变频版本号 */
        {
            addr_offst = data_addr - ADDR_SH_VFD_VER_BGN;
            cell_offst = addr_offst / SH_VFD_VER_SIZE;
            cell_num = data_num / SH_VFD_VER_SIZE;
            fg_rd_succ = SH_get_vfd_version(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_EXIO_VER_BGN && data_addr + data_num <= ADDR_SH_EXIO_VER_END)   /* 外机板版本号 */
        {
            addr_offst = data_addr - ADDR_SH_EXIO_VER_BGN;
            cell_offst = addr_offst / SH_EXIO_VER_SIZE;
            cell_num = data_num / SH_EXIO_VER_SIZE;
            fg_rd_succ = SH_get_exio_version(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_4G_INFO_BGN && data_addr + data_num <= ADDR_SH_4G_INFO_END)
        {
            addr_offst = data_addr - ADDR_SH_4G_INFO_BGN;
            cell_offst = addr_offst / SH_4G_INFO_SIZE;
            cell_num = data_num / SH_4G_INFO_SIZE;
            fg_rd_succ = SH_get_4G_info(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_MODE_SET_INFO_BGN && data_addr + data_num <= ADDR_SH_MODE_SET_INFO_END)   /* 模式信息 */
        {
            addr_offst = data_addr - ADDR_SH_MODE_SET_INFO_BGN;
            cell_offst = addr_offst / SH_MODE_SET_INFO_SIZE;
            cell_num = data_num / SH_MODE_SET_INFO_SIZE;
            fg_rd_succ = SH_get_mode_set_info(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_MAIN_INFO_BGN && data_addr + data_num <= ADDR_SH_MAIN_INFO_END)   /* 主界面信息 */
        {
            addr_offst = data_addr - ADDR_SH_MAIN_INFO_BGN;
            cell_offst = addr_offst / SH_MAIN_INFO_SIZE;
            cell_num = data_num / SH_MAIN_INFO_SIZE;
            fg_rd_succ = SH_get_main_info(&TxBuf[3], cell_offst, cell_num);
        }
        else if (data_addr >= ADDR_SH_INVTERR_BGN && data_addr + data_num <= ADDR_SH_INVTERR_END) /* 查询信息 系统单元模拟量 */
        {
            addr_offst = data_addr - ADDR_SH_INVTERR_BGN;
            cell_offst = addr_offst / SH_INVTERR_SIZE;
            cell_num = data_num / SH_INVTERR_SIZE;
            fg_rd_succ = SH_get_invt_err(&TxBuf[3], cell_offst, cell_num);
        }
		else if (data_addr >= ADDR_SH_FLOOR_PREHEAT_BGN && data_addr + data_num <= ADDR_SH_FLOOR_PREHEAT_END)
        {
            addr_offst = data_addr - ADDR_SH_FLOOR_PREHEAT_BGN;
            cell_offst = addr_offst / SH_FLOOR_PREHEAT_SIZE;
            cell_num = data_num / SH_FLOOR_PREHEAT_SIZE;
            fg_rd_succ = SH_get_floor_preheat_info(&TxBuf[3], cell_offst, cell_num);
        }
		else if (data_addr >= ADDR_SH_FLOOR_TIMER_BGN && data_addr + data_num <= ADDR_SH_FLOOR_TIMER_END)
        {
            addr_offst = data_addr - ADDR_SH_FLOOR_TIMER_BGN;
            cell_offst = addr_offst / SH_FLOOR_TIMER_SIZE;
            cell_num = data_num / SH_FLOOR_TIMER_SIZE;
            fg_rd_succ = SH_get_floor_timer_info(&TxBuf[3], cell_offst, cell_num);
        }
    }
    /* 读取错误 */
    if (!fg_rd_succ)
    {
        com_err = SH_MB_ILLEGAL_DATA_ADDR;
    }

    TxBuf[2] = (U08)(data_num << 1);        /* 数据个数为字数，所以字节数需乘以2 */
    *pTxTotal = (U08)(TxBuf[2] + 2);        /* 加上功能码字节及长度字节 */

    return com_err;
}


/*******************************************************************************
 函 数: SH_MB_rd_data()
 功 能:
 参 数: RxBuf--;
        TxBuf--;
        pTxTotal--;
 返 回:
 其 它: 空
*******************************************************************************/
U08 SH_MB_rd_data(const U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
    #define MB_FILE_EXSTR       1

    U08 com_err = SH_MB_NO_ERR;             /* 故障状态 */
    U16 file_num;
    U32 addr_ofs;
    U08 read_len;

    file_num = MAKEWORD(RxBuf[2], RxBuf[3]);/* 文件号 */
    addr_ofs = MAKEDWORD(RxBuf[4], RxBuf[5], RxBuf[6], RxBuf[7]);
    read_len = RxBuf[8];

    switch (file_num)
    {
        case MB_FILE_EXSTR:
            EXSTR_COM_SlvReadStr(addr_ofs, read_len, &TxBuf[4],pTxTotal);
            break;
        default:
            com_err = SH_MB_ILLEGAL_DATA_ADDR;
            break;
    }

    /* 4 加文件号 */
    TxBuf[2] = RxBuf[2];
    TxBuf[3] = RxBuf[3];
    *pTxTotal += 2;

    /* 4 功能码 */
    TxBuf[1] = RxBuf[1];
    *pTxTotal += 1;

    return com_err;
}

/****************************************************************
函数：SH_MB_wr_reg
功能：主机接收HMI发过来的数据字。
参数：RxBuf--接收缓冲区;
      TxBuf--发送缓冲区;
      pTxTotal--需发送的字节总数;
返回：成功/失败
其它：
*****************************************************************/
U08 SH_MB_wr_reg(const U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
    U16 addr_offst = 0;     /* 数据起始位置(由数据地址决定) */
    U16 data_num = 0;       /* 数据个数 */
    U16 data_addr = 0;
    U08 *src_buf = PNULL;
    U08 com_err = SH_MB_NO_ERR;
    BOOL fg_wr_succ = FALSE;/* 调用外部子程序写入状态标志 */
    U16 cell_offst = 0;     /* 单元起始位置(从第几个结构体单元开始读) */
    U16 cell_num = 0;       /* 单元个数(读取几个结构体单元) */

    data_addr = MAKEWORD(RxBuf[2], RxBuf[3]);               /* 数据地址 */

    if (RxBuf[1] == 0x06)   /* 功能码0x06: 写单个字 */
    {
        data_num = 1;
        src_buf = (U08*)&RxBuf[4];
    }
    else                    /* 功能码0x10: 写多个字 */
    {
        data_num = MAKEWORD(RxBuf[4], RxBuf[5]);
        src_buf = (U08*)&RxBuf[7];
    }

    /* 下句注释只为消除和无符号数大于等于0的编译警告 */
    if (/*data_addr >= ADDR_CMD_BGN && */data_addr + data_num <= ADDR_SH_CMD_END)//命令
    {
        addr_offst = data_addr - ADDR_SH_CMD_BGN;
        cell_offst = addr_offst;
        cell_num = data_num;
        fg_wr_succ = SH_set_command(src_buf, cell_offst, cell_num);
    }
    else if (data_addr >= ADDR_SH_SET_BGN && data_addr + data_num <= ADDR_SH_SET_END)   /* 设置 */
    {
        addr_offst = data_addr - ADDR_SH_SET_BGN;
        cell_offst = addr_offst;
        cell_num = data_num;
        fg_wr_succ = SH_set_setting(src_buf, cell_offst, cell_num);
    }
    else if (data_addr >= ADDR_SH_PASS_BGN && data_addr + data_num <= ADDR_SH_PASS_END) /* 密码 */
    {
        addr_offst = data_addr - ADDR_SH_PASS_BGN;
        cell_offst = addr_offst / SH_PASS_SIZE;
        cell_num = data_num / SH_PASS_SIZE;
        fg_wr_succ = SH_set_passwrd(src_buf, cell_offst, cell_num);
    }
    else if (data_addr >= ADDR_SH_CLOCK_BGN && data_addr + data_num <= ADDR_SH_CLOCK_END)   /* 时钟 */
    {
        addr_offst = data_addr - ADDR_SH_CLOCK_BGN;
        cell_offst = addr_offst / SH_CLOCK_SIZE;
        cell_num = data_num / SH_CLOCK_SIZE;
        fg_wr_succ = SH_set_clock(src_buf, cell_offst, cell_num);
    }
    else if (data_addr >= ADDR_SH_HOLIDAYINFO_BGN && data_addr + data_num <= ADDR_SH_HOLIDAYINFO_END)   /* 假日模式 */
    {
        fg_wr_succ = SH_set_holidayinfo(src_buf, data_num);
    }
    else if (data_addr >= ADDR_ROOM_TEMP_BGN && data_addr + data_num <= ADDR_SH_ROOM_TEMP_END)   /* 室内温度 */
    {
        fg_wr_succ = SH_set_room_temp(src_buf, data_num);
    }
    else if (data_addr >= ADDR_SH_PARA_BGN && data_addr + data_num <= ADDR_SH_PARA_END) /* 参数值 */
    {
        addr_offst = data_addr - ADDR_SH_PARA_BGN;
        cell_offst = addr_offst;
        cell_num = data_num;
        fg_wr_succ = SH_set_para(src_buf, cell_offst, cell_num);
    }
    else if (data_addr >= ADDR_SH_CHK_TEST_WO_BGN && data_addr + data_num <= ADDR_SH_CHK_TEST_WO_END)
    {
        addr_offst = data_addr - ADDR_SH_CHK_TEST_WO_BGN;
        cell_offst = addr_offst;
        cell_num = data_num;
        fg_wr_succ = SH_set_test_wo(src_buf, cell_offst, cell_num);
    }
    else if (data_addr >= ADDR_SH_CHK_TEST_WR_BGN && data_addr + data_num <= ADDR_SH_CHK_TEST_WR_END)
    {
        addr_offst = data_addr - ADDR_SH_CHK_TEST_WR_BGN;
        cell_offst = addr_offst;
        cell_num = data_num;
        fg_wr_succ = SH_set_test_wr(src_buf, cell_offst, cell_num);
    }
    else if (data_addr >= ADDR_SH_PASS_BIG_ENDIAN_BGN && data_addr + data_num <= ADDR_SH_PASS_BIG_ENDIAN_END)
    {
        fg_wr_succ = SH_set_pass_big_endian();
    }
    else if (data_addr >= ADDR_SH_TUYA_DATA_BGN && data_addr + data_num <= ADDR_SH_TUYA_DATA_END)
    {
        addr_offst = data_addr - ADDR_SH_TUYA_DATA_BGN;
        fg_wr_succ = SHC_set_tuya_data(src_buf);
    }
    else if (data_addr >= ADDR_SH_INVTPARA_BGN && data_addr + data_num <= ADDR_SH_INVTPARA_END)
    {
        fg_wr_succ = SH_set_invt_para(src_buf, data_num);
    }
    else if (data_addr >= ADDR_SH_FLOOR_PREHEAT_VAL_BGN && data_addr + data_num <= ADDR_SH_FLOOR_PREHEAT_VAL_END)
    {
        addr_offst = data_addr - ADDR_SH_FLOOR_PREHEAT_VAL_BGN;
        cell_offst = addr_offst;
        cell_num = data_num;
        fg_wr_succ = SH_set_floor_preheat_val(src_buf, cell_offst, cell_num);
    }
    else if (data_addr >= ADDR_SH_FLOOR_TIMER_VAL_BGN && data_addr + data_num <= ADDR_SH_FLOOR_TIMER_VAL_END)
    {
        addr_offst = data_addr - ADDR_SH_FLOOR_TIMER_VAL_BGN;
        cell_offst = addr_offst;
        cell_num = data_num;
        fg_wr_succ = SH_set_floor_timer_val(src_buf, cell_offst, cell_num);
    }
    else if (data_addr >= ADDR_SH_TEST_FAC_HMI_WR_BGN && data_addr + data_num <= ADDR_SH_TEST_FAC_HMI_WR_END)
    {
        fg_wr_succ = SH_set_test_fac_hmi_info(src_buf, data_num);
    }

    /* 写入错误 */
    if(!fg_wr_succ)
    {
        com_err = SH_MB_ILLEGAL_DATA_ADDR;
    }

    TxBuf[2] = RxBuf[2];
    TxBuf[3] = RxBuf[3];
    TxBuf[4] = RxBuf[4];
    TxBuf[5] = RxBuf[5];
    *pTxTotal = 5;

    return com_err;
}

/*******************************************************************************
 函 数: SH_MB_diag()
 功 能: 从机处理ModBus回路诊断测试命令
 参 数: RxBuf--接收缓冲区;
        TxBuf--发送缓冲区;
        pTxTotal--需发送的字节总数;
 返 回: 异常标志
 其 它:
*******************************************************************************/
static U08 SH_MB_diag(const U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
    U08 i;
    U16 subFunctionCode;    /* 子功能码 */
    U16 data_num;           /* 数据个数 */
    U08 bytes;              /* 数据字节数 */

    subFunctionCode = MAKEWORD(RxBuf[2], RxBuf[3]);

    if (subFunctionCode == 0)
    {
        /* 个数判断 */
        data_num = MAKEWORD(RxBuf[4], RxBuf[5]);
        bytes = RxBuf[6];
        if ((data_num<1) || (data_num>100)
            || (bytes != 2*data_num))
        {
            return SH_MB_ILLEGAL_DATA_VALUE;
        }

        /* 请求处理 */
        for (i=1; i<bytes+7; i++)
        {
            TxBuf[i] = RxBuf[i];
        }
        *pTxTotal = (U08)(bytes+6);

        return SH_MB_NO_ERR;
    }
    else
    {
        return SH_MB_ILLEGAL_FUNCTION;
    }
}

/*******************************************************************************
 函 数: SH_MB_match_rx()
 功 能: 从机匹配接收数据
 参 数: pCOM--串口结构体指针;
 返 回: TRUE/FALSE--匹配成功/匹配失败
 其 它:
*******************************************************************************/
static BOOL SH_MB_match_rx(UART *pCOM)
{
    #define LEAST_FRAME_BYTES   4           /* 最少的帧字节数 */
    #define SH_SLAVER_ADDR      0x01        /* 从机地址 */

    U16 idxBegin;           /* 查找数据匹配的开始索引号 */
    U16 idxEnd;             /* 查找数据匹配的结束索引号 */
    BOOL matchOk;           /* 匹配OK标识 */
    U16 matchOkIdx=0;       /* 匹配OK时的索引号(注，赋初值0防止编译警告) */
    U16 theoreticBytes=0;   /* 根据功能码，理论需接收的字节数 */

    /* 若收到的数据字节数小于最少的帧字节数，可以确认是干扰 */
    if (pCOM->rxCnt < LEAST_FRAME_BYTES)
    {
        /* 通讯记录:帧数据过短++ */
        return FALSE;
    }
    /* 从头依次开始寻找接收数据是否匹配，不需要查找完所有接收数据， */
    /* 当 剩余数据个数 小于 最少帧字节数 时，就不需要继续往查找了。 */
    matchOk = FALSE;
    idxEnd = pCOM->rxCnt - LEAST_FRAME_BYTES;
    for (idxBegin=0; idxBegin<=idxEnd; idxBegin++)
    {
        U08 rxAddr;         /* 接收的地址 */
        U08 rxFunCode;      /* 接收的功能码 */
        BOOL funCodeMatch;  /* 功能码匹配否 */

        /* 地址不匹配，就继续下一个查找 */
        rxAddr = pCOM->rxBuf[idxBegin];
        if ((rxAddr!=SH_SLAVER_ADDR) && (rxAddr!=0))
        {
            continue;
        }

        /* 查找功能码与理论接收个数 */
        rxFunCode = pCOM->rxBuf[idxBegin+1];
        switch (rxFunCode)
        {
            case 0x01 :     /* 0x01 读可读写位 */
            case 0x02 :     /* 0x02 读只读位 */
            case 0x03 :     /* 0x03 读可读写字 */
            case 0x04 :     /* 0x04 读只读字 */
            case 0x05 :     /* 0x05 写单个位 */
            case 0x06 :     /* 0x06 写单个字 */
                funCodeMatch = TRUE;
                theoreticBytes = 8;
                break;

            case 0x08 :     /* 0x08 回路诊断 */
            case 0x0f :     /* 0x0f 写多个位 */
            case 0x10 :     /* 0x10 写多个字 */
            case 0x41 :     /* 0x41 自定义：程序下载 */
                funCodeMatch = TRUE;
                theoreticBytes = 9+pCOM->rxBuf[idxBegin+6];
                break;
            case 0x44:
                funCodeMatch = TRUE;
                theoreticBytes = 11;
                break;
            default:
                funCodeMatch = FALSE;
                break;
        }
        /* 功能码与接收个数不匹配，就继续下一个查找 */
        if ((!funCodeMatch) || (pCOM->rxCnt < (idxBegin+theoreticBytes)))
        {
            continue;
        }
        /* crc校验通过后，匹配成功退出查找；否则继续下一个查找 */
        if (fun_crc16_ok(&pCOM->rxBuf[idxBegin], (U08)theoreticBytes))
        {
            matchOk = TRUE;
            matchOkIdx = idxBegin;
            break;
        }
    }

    if (matchOk)
    {
        /* 匹配数据前移至缓冲区首 */
        if (matchOkIdx > 0)
        {
            U16 i;
            for (i=0; i<theoreticBytes; i++)
            {
                pCOM->rxBuf[i] = pCOM->rxBuf[matchOkIdx+i];
            }
        }
        /* 记录正确数据次数 */
        if (pCOM->rxCnt == theoreticBytes)
        {
            /* 通讯记录:完全正确++ */
        }
        else
        {
            if (matchOkIdx == 0)
            {
                /* 通讯记录:后干扰++ */
            }
            else
            {
                /* 通讯记录:前干扰++ */
            }
        }

        return TRUE;
    }
    else
    {
        /* 未找到匹配数据 */
        /* 通讯记录:数据校验错++ */
        return FALSE;
    }
}

/*******************************************************************************
 函 数: SH_MB_deal_rece()
 功 能: 应答HMI的读/写请求
 参 数: RxBuf--接收缓冲区;
        TxBuf--发送缓冲区;
        pTxTotal--需发送的字节总数;
 返 回: 异常标志
 其 它: HD009A彩屏专用
 移 植: 根据当前程序平台进行修改，以适应当前的UART数据结构、通讯相关宏定义等。
*******************************************************************************/
U08 SH_MB_deal_rece(const U08 *RxBuf, U08 *TxBuf, U16 *pTxTotal)
{
    U08 exceptCode = SH_MB_NO_ERR;
    W16 crc;
    U08 TxTotal = 0;

    /* 功能码分类 */
    switch (RxBuf[1])
    {
        case MB_RD_HOLDING_REGISTERS :      /* 0x03 读可读写字 */
        case MB_RD_INPUT_REGISTERS :        /* 0x04 读只读字 */
            exceptCode = SH_MB_rd_reg(RxBuf, TxBuf, pTxTotal);
            break;

        case MB_WR_SINGLE_REGISTER :        /* 0x06 写单个字 */
        case MB_WR_MULTIPLE_REGISTERS :     /* 0x10 写多个字 */
            exceptCode = SH_MB_wr_reg(RxBuf, TxBuf, pTxTotal);
            break;

        case MB_DIAGNOSTICS :               /* 0x08 回路诊断 */
            exceptCode = SH_MB_diag(RxBuf, TxBuf, pTxTotal);
            break;
        case MB_USER_DEFINED_RD_BIGFILE :
            exceptCode = SH_MB_rd_data(RxBuf, TxBuf, pTxTotal);
            break;
        case MB_USER_DEFINED_DOWNLOAD :     /* 0x41 自定义：程序下载 */
            exceptCode = PDU_SlaveDealUserDef(RxBuf, TxBuf, pTxTotal);
            break;

        /* 以下彩屏未用到的功能码暂定为非法功能码 */
        case MB_RD_COILS:                   /* 0x01 读可读写位 */
        case MB_RD_DISCRETE_INPUTS:         /* 0x02 读只读位 */
        case MB_WR_SINGLE_COIL:             /* 0x05 写单个位 */
        case MB_WR_MULTIPLE_COILS:          /* 0x0f 写多个位 */
        default :                           /* 其它功能码由于暂不支持，也作为非法功能码 */
            exceptCode = SH_MB_ILLEGAL_FUNCTION;
            break;
    }

    /* 处理异常响应PDU */
    if (exceptCode != SH_MB_NO_ERR)
    {
        TxBuf[1] = (U08)(RxBuf[1] | 0x80);
        TxBuf[2] = exceptCode;
        *pTxTotal = 2;
    }
    else
    {
        TxBuf[1] = RxBuf[1];
    }

    /* 增加附加域:帧头的地址、帧尾的CRC校验 */
    TxTotal = *pTxTotal;
    TxBuf[0] = RxBuf[0];
    TxTotal += 1;
    crc.word = fun_crc16(&TxBuf[0], TxTotal);
    TxBuf[TxTotal++] = crc.byte.low;
    TxBuf[TxTotal++] = crc.byte.high;
    *pTxTotal = TxTotal;

    return exceptCode;
}

/*******************************************************************************
 函 数: SH_MB_DealUartSlave()
 功 能: 从机通讯处理
 参 数: 串口号
 返 回: 空
 其 它:
 移 植: 根据当前程序平台进行修改，以适应当前的UART数据结构、通讯模型、宏定义、通讯计时等。
*******************************************************************************/
void SH_MB_DealUartSlave(U08 u)
{
    UART *pCOM;

    pCOM = &Uart[u];

    switch (pCOM->step)
    {
        case COMMU_RX :     /* 收到正确帧且非广播时，转为发送延时，否则一直接收 */
            /* 一帧接收完成:接收帧时间内未收到下一数据或接收缓冲区满 */
            if ((pCOM->rxCnt>0 && pCOM->time.rxFrame==0)
                || (pCOM->rxCnt >= COMMU_RX_SIZE))
            {
                COM_DisableRece(u);         /* 处理接收帧时关闭接收使能 */
                if (SH_MB_match_rx(pCOM))
                {
                    SH_MB_deal_rece(&pCOM->rxBuf[0], &pCOM->txBuf[0], &pCOM->txTotal);

                    _CNT(SH_hmi_commu_succ);/* 注意:通讯成功后需要累加成功数，否则会被认为是通讯失败而切换成主口 */

                    pCOM->time.led = COMMU_TIME_LED;
                    pCOM->time.commIntProt = COMMU_TIME_PROTECT;
                    pCOM->commuErr = COMMU_NO_ERR;          /* 自动复位 */

                    if (pCOM->rxBuf[0] != ADDR_BROADCAST)   /* 不是广播, 才回送数据 */
                    {
                        pCOM->step = COMMU_DELAY;
                        pCOM->time.txDly = COMMU_TIME_TX_DLY;
                    }
                    else    /* 广播时，准备接收下一帧 */
                    {
                        COM_ReadyRece(u);
                    }
                }
                else        /* 错误帧时，准备接收下一帧 */
                {
                    COM_ReadyRece(u);
                }

                pCOM->rxCnt = 0;            /* 处理一帧数据后，清接收计数器 */
            }

            /* 通讯中断保护时间到，仍未收到有效数据帧，报通讯中断保护故障 */
            if (pCOM->time.commIntProt == 0)
            {
                pCOM->commuErr = COMMU_INT_PROT;
            }
            break;
        case COMMU_DELAY :
            COM_DisableRece(u);
            COM_Set485Tras(u);
            if (pCOM->time.txDly == 0)
            {
                pCOM->step = COMMU_START;
            }
            else
            {
                break;
            }
            break;
        case COMMU_START :
            pCOM->txCnt = 0;
            pCOM->rxCnt = 0;
            COM_WrTrasBufReg(u, pCOM->txBuf[0]);
            pCOM->step = COMMU_TX;
            pCOM->time.txDly = COMMU_TIME_TX_TIMEOUT;
            break;

        case COMMU_TX :     /* 转接收由中断改为这里 */
            if (pCOM->txCnt >= pCOM->txTotal)
            {
                pCOM->time.commIntProt = COMMU_TIME_PROTECT;/* 从机用 */
                pCOM->step = COMMU_RX;
                /* 在发送中断中已经置为接收，在高速通信时， */
                /* 可能执行此处前已经接收了数据，因此此次可以不处理 */
                COM_ReadyRece(u);           /* 继续处理，确保485收发引脚设置成功 */
            }
            else if (pCOM->time.txDly == 0) /* 超时 */
            {
                pCOM->commuErr = COMMU_OVERTIME;
                pCOM->step = COMMU_FAIL;
                COM_ReadyRece(u);
            }
            break;

        default :
            pCOM->step = COMMU_RX;
            COM_ReadyRece(u);
            pCOM->rxCnt = 0;
            break;
    }
}

/*******************************************************************************
 函 数: SH_MB_commu_adjust()
 功 能: 通讯自适应:兼容文本屏与4.3寸彩屏
 参 数: 空
 返 回: 空
 其 它: 通讯失败持续一段时间后:循环切换主从模式
 移 植: 使用本函数替换原本的单向通讯处理函数，并在本函数中调用该函数。
        根据需要在主从切换时适当添加串口初始化。
*******************************************************************************/
void SH_MB_commu_adjust(void)
{
    enum 
    {
        HMI_IS_M, 
        HMI_IS_S,  
#if defined(M_NIUENTAI) || defined(M_DEBUG)
        HMI_IS_NET,			/* 纽恩泰的屏：基于BMS协议开发 */
#endif
        HMI_NUM_MAX
    };
    #define SH_TYPE_CHG_INTVL       6       /* 主从切换间隔(500ms) */

    static U08 cnt_commu_fail = 0;          /* 通讯失败计时 */
    static U08 hmi_type = 0;                /* 前板类型 */
    static U08 flash_500_bak = 0;           /* 500ms状态备份 */

    /* 通讯处理 */
    switch (hmi_type)
    {
        default:
        case HMI_IS_S:
            /* 作为主机与屏(DM603)通信 */
            COM_HMIMasterProg(COM_TEXT_HMI);    /* 注意:通讯成功后需要 SH_hmi_commu_succ++; */
            break;
        case HMI_IS_M:
            /* 作为从机与屏通信 */
            SH_MB_DealUartSlave(COM_TEXT_HMI);
            break;  
#if defined(M_NIUENTAI) || defined(M_DEBUG)
        case HMI_IS_NET:
            /* 作为从机与纽恩泰的屏通信 */
            COM_SlaverProg(COM_TEXT_HMI); 
            break; 
#endif             
    }

    /* 失败计数(每500ms执行一次) */
    /* 由于本函数在一个循环内有可能被调用多次，所以不能用F_TIME_BASIC_500MS_BACK来计时 */
    if (flash_500_bak != flash_500ms)
    {
        flash_500_bak = flash_500ms;

        if (SH_hmi_commu_succ == 0)
        {
            _CNT(cnt_commu_fail);
        }
        else
        {
            cnt_commu_fail = 0;
            SH_hmi_commu_succ = 0;
        }
    }
    /* 通讯失败超时:切换屏 */
    if (cnt_commu_fail >= SH_TYPE_CHG_INTVL)
    {
        cnt_commu_fail = 0;
        if (++hmi_type >= HMI_NUM_MAX)
        {
            hmi_type = 0;
        }

        switch (hmi_type)
        {
            default:
            case HMI_IS_S:
                COM_Config(COM_TEXT_HMI, B_19200, T_8N1);  
                COM_InitMaster(COM_TEXT_HMI, BKBD_INVT_UART3);          /* 显示屏 */
                break;
            case HMI_IS_M:
                COM_Config(COM_TEXT_HMI, B_19200, T_8N1);  
                COM_InitSlaver(COM_TEXT_HMI, BKBD_INVT_UART3, 1);       /* 7寸屏 */
                break;       
#if defined(M_NIUENTAI) || defined(M_DEBUG)
            case HMI_IS_NET:
                COM_Config(COM_TEXT_HMI, B_9600, T_8N1);                  
                COM_InitSlaver(COM_TEXT_HMI, BKBD_INVT_UART3, 1);	    /* AD007 */ 
                break;     
#endif                
        }
    }
}

/*****************************************************************************
函  数 : SH_timer()
功  能 : 计时
参  数 : 无
返  回 : 无
其  他 : 需放到外部执行
*****************************************************************************/
void SH_timer(void)
{
    SHC_timer();
}

void SH_init(void)
{
    SHC_init();
}

