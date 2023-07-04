/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: Tuya_B_Config.h
 文件描述: 涂鸦无线功能配置(后板部分)
 创建人  : 林浩彬
 创建日期: 2022.03.11
*******************************************************************************/

#ifdef TUYA_B_CONFIG_GLOBAL
    #define TUYA_B_CFG_EXT
#else
    #define TUYA_B_CFG_EXT extern
#endif

/* ========================================================================================== */
/* 注意事项:                                                                                  */
/* 1、本文件所有的宏定义名禁止更改；                                                          */
/* 2、本文件中与"TUYA_USE_XXX"相关的条件编译(#if/#ifdef/#ifndef)不可随意改动。                */
/* ========================================================================================== */

/* 手机面板调试窗("TUYA-DEBUG"模式!发布时一定要屏蔽!!!) */
//#define TUYA_USE_DBG_CONSOLE

/* 使用计时配置 */
//#define TUYA_USE_CONFIG_TIMER
/* 使用初始化配置 */
#define TUYA_USE_CONFIG_INIT
 /* 使用"缓冲区超界保护"(注: 非到万不得已时请勿屏蔽此行!) */
#define TUYA_USE_BUF_SAFETY

/* 业务逻辑类型 */
#define TUYA_B_SLP_TYPE                            (TUYA_B_SLP_NT_HLXXX_MULTI)

/* 功能选择 */
#define TUYA_USE_HMI_MASTER                        /* 支持主机屏(新彩屏协议) */
#define TUYA_USE_HMI_SLAVE	                       /* 支持从机屏 */
//#define TUYA_USE_HMI_SELF	                       /* 支持一体式屏(风盘) */
#define TUYA_USE_SYS_STAT                          /* 使用"系统状态"功能 */
#define TUYA_USE_MOD_STAT                          /* 使用"模块状态"功能 */
#define TUYA_USE_PARAM_SET                         /* 使用"参数设置"功能 */
#define TUYA_USE_ERR_CUR                           /* 使用"当前故障"功能 */
#define TUYA_USE_ERR_HIS                           /* 使用"历史故障"功能 */
#define TUYA_USE_FAC_PWD                           /* 使用"厂家密码"功能 */
#define TUYA_USE_LANG_SET                          /* 使用"语言"功能 */
#define TUYA_USE_SPLASH                            /* 使用"启动页"功能 */
//#define TUYA_USE_CHILDLOCK                         /* 使用"童锁"功能 */
//#define TUYA_USE_WIND                              /* 使用"风速"功能 */
//#define TUYA_USE_HUMI                              /* 使用"湿度"功能 */
//#define TUYA_USE_HOT_WATER                         /* 使用"热水"功能 */
#define TUYA_USE_ELE_STAT                          /* 使用"用电查询"功能 */
//#define TUYA_USE_CLO_TIME                          /* 使用"云定时"功能 */
//#define TUYA_USE_TEMP_UNIT_SET				/* 使用温度单位设置 */

/* ========================================================================================== */
/* 常规定义                                                                                   */
/* ========================================================================================== */
#define TYBC_PCB_MAX                               (MAX_PCB)                   /* 预留最大模块数 */
#define TYBC_COMP_MAX                              (USE_COMP)                  /* 最大压机数 */
#define TYBC_PV_MAX                                (MAX_PV_WORD)               /* cell_pv[]个数 */
#define TYBC_SYS_PV_MAX                            (SYS_PV_NUM)                /* 系统模拟量个数(注意SYS_PV_NUM以后的空间可能另有用途) */
#define TYBC_UNIT_PV_BEGIN                         (CELL_PV_BEGIN)             /* 单元模拟量起始位置 */
#define TYBC_COMP_PV_MAX                           (COMP_PV_NUM)               /* 压机模拟量最大个数 */
#define TYBC_MOD_PV_MSG_MAX                        (CELL_PV_NUM+COMP_PV_NUM)   /* 单元&压机模拟量名称总数 */
#define TYBC_PWD_MAX                               (MAX_PASS_BIT)              /* 密码最大位数 */
#define TYBC_NO_ERR                                (F_NO_ERR)                  /* 无故障号 */
#define TYBC_MSG_NULL                              (msg_tyb_null)               /* 空字符串 */
#define TYBC_SEC_FLAG                              (F_TIME_BASIC_SEC_BACK)     /* 1秒标志 */
#define TYBC_STR_BYTE_MAX                          (120)		                   /* 单条字符串最大字节数 */

/* ========================================================================================== */
/* 产品配置                                                                                   */
/* ========================================================================================== */
/* 模组类型 */
#define TYBC_MODULE_TYPE                           (MODULE_WIFI)     /* WIFI模组 */

/* 产品ID */
//#define TYBC_PID                                   "cawxbpbi0l9vzyzy"/* 产品名称: 温控器品类_变频空气能 */
//#define TYBC_PID							       "6o2it5fidbsi18zc"	/* 空调+热水 同时设定 */
//#define TYBC_PID							       "p7yl8alakseirps3"	/* 变频多联供 */
//#define TYBC_PID                                   "9dn51spevrkyaipl"   /* 变频多联供debug */

/* 微码号主干&版本号 */
#if defined(M_DEBUG)
	#define TYBC_PID		"figijxoiqt2wlj8b"				/* DEBUG_使用通用pid */
#elif defined(M_TCL)
    #define TYBC_PID        "gvedawoegg8idjha"              /* 中山TCL */
#elif defined(M_CHANGHONG)
    #define TYBC_PID        "sa8ar4ibpyxm1k7u"              /* 中山长虹 */
#elif defined(M_TONGYONG)
    #define TYBC_PID        "figijxoiqt2wlj8b"              /* 通用 */
#elif defined(M_PAIWO)
    #define TYBC_PID        "ey0j6ijachritgw7"              /* 广东派沃 */
#elif defined(M_DISEN)
    #define TYBC_PID        "2nvnrlxwzkaodxq4"              /* 广州迪森 */
#elif defined(M_NIUENTAI)
    #define TYBC_PID        "jdvpsawdciqkkuhr"              /* 广东纽恩泰 */
#elif defined(M_SHENGNENG)
    #define TYBC_PID        "1ceeidrbexlkqhcb"              /* 浙江生能 */
#elif defined(M_HONGHE)
    #define TYBC_PID        "yxbw8sprrra5djvi"              /* 佛山鸿禾 */
#elif defined(M_WANHE)
    #define TYBC_PID        "fhsjdw1dca6vb1pl"              /* 万和集团 */
#elif defined(M_CHANGFAN)
    #define TYBC_PID        "kjkkmazngxxefqrd"              /* 广东长帆 */
#else
    #error "未定义厂家!"
#endif


/* ========================================================================================== */
/* 中英字符串字典配置                                                                         */
/* ========================================================================================== */
#ifdef USE_GBK
#define TYBC_CODE_STR_DICT_CN(idx)                 (IDX_TO_C_PTR(idx))   /* 内码中文字符串索引字典 */
#define TYBC_CODE_STR_DICT_EN(idx)                 (IDX_TO_E_PTR(idx))   /* 内码英文字符串索引字典 */
#elif defined (USE_UNICODE)
#define TYBC_CODE_STR_DICT_CN(idx)                 (chinese_uncompress_text(idx))   /* 内码中文字符串索引字典 */
#define TYBC_CODE_STR_DICT_EN(idx)                 (english_uncompress_text(idx))   /* 内码英文字符串索引字典 */
#define TYBC_CODE_STR_DICT_SP(idx)                 (spanish_uncompress_text(idx))	 /* 内码西班牙语字符串索引字典 */
#define TYBC_CODE_STR_DICT_GR(idx)				   (greek_uncompress_text(idx))	/* 内码希腊语字符串索引字典 */
#define TYBC_CODE_STR_DICT_FR(idx)				   (french_uncompress_text(idx))	/* 内码法语字符串索引字典 */
#define TYBC_CODE_STR_DICT_ITL(idx)				   (italian_uncompress_text(idx))	/* 内码意大利语字符串索引字典 */
#define TYBC_CODE_STR_DICT_POL(idx)				   (polish_uncompress_text(idx))	/* 内码波兰语字符串索引字典 */
#define TYBC_CODE_STR_DICT_HG(idx)				   (hungarian_uncompress_text(idx))	/* 内码匈牙利语字符串索引字典 */
#define TYBC_CODE_STR_DICT_GE(idx)				   (german_uncompress_text(idx))	/* 内码德语字符串索引字典 */
#endif

/* ========================================================================================== */
/* 版本信息配置                                                                               */
/* ========================================================================================== */
/* 字段0: 是否显示显示屏版本号 */
#define TYBC_DISP_HMI_VER_EN                       (TRUE)

/* 字段1: 是否显示控制器版本号 */
#define TYBC_DISP_BKBD_VER_EN                      (TRUE)
/* 显示控制器版本 */
#if (TYBC_DISP_BKBD_VER_EN == TRUE)
#define TYBC_BKBD_VER                              (APPFUNCODEVER)
#endif

/* ========================================================================================== */
/* 系统信息配置                                                                               */
/* ========================================================================================== */
/* 字段0: 字符编码类型信息 */
#ifdef USE_UNICODE
#define TYBC_STR_CODE_TYPE                         (UNI_CODE)
#else
#define TYBC_STR_CODE_TYPE                         (P_CODE)
#endif

/* 字段1: 系统维护标志信息 */
#define TYBC_SYS_MAINTENANCE_VAL                   ((fg_sys_limit) ? (TRUE) : (FALSE))

/* 字段2: 面板显示语言信息 */
#define TYBC_LANGUAGE_VAL                          (TYBC_language_to_rn_enum(sh_language))

/* 字段3: 故障告警标志信息 */
#define TYBC_ERROR_ALERT_VAL                       ((fg_error) ? (TRUE) : (FALSE))

/* 字段4: 模块数信息(单机请填0) */
#define TYBC_MOD_NUM                               (val_pcb_num)

/* 字段5: 厂家密码信息 */
/* 有"厂家密码" */
#ifdef TUYA_USE_FAC_PWD
#define TYBC_PASSWORD_POINTER                      (TYBC_get_password_pointer())  /* 厂家密码指针(指定U08*) */
/* 无"厂家密码" */
#else
#define TYBC_PASSWORD_POINTER                      (PNULL)
#endif

/* 字段6: 运行切换模式允许信息 */
#define TYBC_RUN_SWITCH_MODE_EN                    (TRUE)

/* 字段7: 启动页LOGO ID信息(无启动页则填 SPLASH_LOGO_NULL) */
#define TYBC_SPLASH_LOGO_VAL                       (val_TCL_splash_offset ? (SPLASH_LOGO_TCL+val_TCL_splash_offset-1) : 0)

/* 字段8: 智能语音允许信息 */
#define TYBC_SMART_SPEAKER_EN                      (FALSE)

/* 字段9: 参数组总数(预留最大数) */
#define TYBC_PARAM_GRP_MAX                         (GROUP_MAX)

/* 字段10: 参数项总数/每组参数项个数(预留最大数) */
#define TYBC_PARAM_GRP_SIZE                        (GROUP_SIZE)

/* 字段11: 可选功能(此处可绑定参数，且只选择"禁用"或"使用") */
#define TYBC_FUNC_EN_MORE_GEN_SET                  (TRUE)	/* "更多"-"通用设置"入口 */
#define TYBC_FUNC_EN_ADVANCE_ERR_HIS               (TRUE)	/* "高级"-"历史故障"入口 */
#define TYBC_FUNC_EN_ADVANCE_PARAM_SET             (TRUE)	/* "高级"-"参数设置"入口 */
#define TYBC_FUNC_EN_MAIN_STAT                     (FALSE)	/* "主界面"-"状态查询"入口 */
#define TYBC_FUNC_EN_ADVANCE_STAT                  (TRUE)	/* "高级"-"状态查询"入口 */
#define TYBC_FUNC_EN_MAIN_ELE_STAT                 (USE_ELEC_METERING)   /* "主界面"-"用电查询"入口 */
#define TYBC_FUNC_EN_MORE_CLO_TIME                 (FALSE)   /* "更多"-"云定时"入口 */

/* ========================================================================================== */
/* 通用设置信息配置                                                                           */
/* ========================================================================================== */
/* 字段0: 语言设置信息 */
/* 有"语言功能" */
#ifdef TUYA_USE_LANG_SET
#define TYBC_LANGUAGE_TYPE                         (FIELD_ENUM)    /* 使用：FIELD_ENUM 不用：FIELD_DISABLED */
#define TYBC_LANGUAGE_TITLE                        (TY_MSGID_TIT_LANG)
#define TYBC_LANGUAGE_RANGE                        (TYBC_get_language_range())
#define TYBC_LANGUAGE_MAX_NUM                      (RN_LANGUAGE_MAX)
#endif

/* 字段1: 启动页设置信息 */
/* 有"启动页功能" */
#ifdef TUYA_USE_SPLASH
#define TYBC_SPLASH_TYPE                           (FIELD_BOOL)    /* 使用：FIELD_BOOL 不用：FIELD_DISABLED */
#define TYBC_SPLASH_TITLE                          (TY_MSGID_TIT_SPLASH)
#define TYBC_SPLASH_VAL                            ((val_TCL_splash_offset&&para_splash) ? TRUE : FALSE)
#endif

/* 字段2: 童锁设置信息 */
/* 有"童锁功能" */
#ifdef TUYA_USE_CHILDLOCK
#define TYBC_CHILDLOCK_TYPE                        (FIELD_BOOL)    /* 使用：FIELD_BOOL 不用：FIELD_DISABLED */
#define TYBC_CHILD_LOCK_TITLE                      (TY_MSGID_TIT_CHILDLOCK)
#define TYBC_CHILDLOCK_VAL                         ((val_lock_screen==TRUE) ? (TRUE) : (FALSE))
#endif

/* 字段3: 区域一使能 */
/* 有"区域一" */
#define TYBC_ZoneA_EN_TYPE                        (FIELD_BOOL)    /* 使用：FIELD_BOOL 不用：FIELD_DISABLED */
#define TYBC_ZoneA_EN_TITLE                       (TY_MSGID_TIT_ZONE1)
#define TYBC_ZoneA_EN_VAL                         (TRUE)

/* 字段4: 区域二使能 */
/* 有"区域二" */
#define TYBC_ZoneB_EN_TYPE                        (FIELD_BOOL)    /* 使用：FIELD_BOOL 不用：FIELD_DISABLED */
#define TYBC_ZoneB_EN_TITLE                       (TY_MSGID_TIT_ZONE2)
#define TYBC_ZoneB_EN_VAL                         ((USE_DUAL_ZONE_CTRL) ? (TRUE) : (FALSE))

/* 字段5: 热水使能 */
/* 有"热水功能" */
#define TYBC_DHW_EN_TYPE                         (FIELD_BOOL)    /* 使用：FIELD_BOOL 不用：FIELD_DISABLED */
#define TYBC_DHW_EN_TITLE                        (TY_MSGID_TIT_DHW)
#define TYBC_DHW_EN_VAL                          ((val_hotw_used==1) ? (TRUE) : (FALSE))

/* 有"温度单位设置功能" */
#ifdef TUYA_USE_TEMP_UNIT_SET
#define TYBC_TEMP_UNIT_TYPE                         (FIELD_ENUM)    /* 使用：FIELD_ENUM 不用：FIELD_DISABLED */
#define TYBC_TEMP_UNIT_TITLE                        (TY_MSGID_TIT_TEMP_UNIT)
#define TYBC_TEMP_UNIT_RANGE                        (TYBC_get_temp_unit_range())
#define TYBC_TEMP_UNIT_MAX_NUM                      (RN_TEMP_UNIT_MAX)
#define TYBC_TEMP_UNIT_VAL                          (TEMP_IS_FARA)
#endif

/* ========================================================================================== */
/* 通用设置命令处理                                                                         */
/* ========================================================================================== */
/* 字段0: 语言命令处理 */
/* 有"语言功能" */
#ifdef TUYA_USE_LANG_SET
#define TYBC_CMD_LANGUAGE_CN                       (TYBC_language_set(LANGUE_CHINESE))    /* 设置中文 */
#define TYBC_CMD_LANGUAGE_EN                       (TYBC_language_set(LANGUE_ENGLISH))    /* 设置英文 */
#endif

/* 字段1: 启动页命令处理 */
/* 有"启动页功能" */
#ifdef TUYA_USE_SPLASH
#define TYBC_CMD_SPLASH_ON                         (para_splash = 1)/* 打开启动页 */
#define TYBC_CMD_SPLASH_OFF                        (para_splash = 0)/* 关闭启动页 */
#endif

/* 字段2: 童锁命令处理 */
/* 有"童锁功能" */
#ifdef TUYA_USE_CHILDLOCK
#define TYBC_CMD_CHILDLOCK_ON                      (val_lock_screen = TRUE)  /* 打开童锁 */
#define TYBC_CMD_CHILDLOCK_OFF                     (val_lock_screen = FALSE)  /* 关闭童锁 */
#endif

TUYA_B_CFG_EXT SH_MAIN_INFO sh_main_info;
/* ========================================================================================== */
/* 区域一功能信息配置                                                                         */
/* ========================================================================================== */
/* 字段0: 开关机 */
#define TYBC_SWITCH_TYPE                           (FIELD_BOOL)      			/* 使用：FIELD_BOOL 字段不用：FIELD_DISABLED */
#define TYBC_SWITCH_TITLE                          (TY_MSGID_TIT_SWITCH)        /* 字符串ID */
#define TYBC_SWITCH_VAL                            (sh_main_info.fg_zone_1_is_run)				/* 当前值 */

/* 字段1: 设定温度 */
#define TYBC_TEMP_SET_TYPE                         (FIELD_NUMBER)    /* 使用：FIELD_NUMBER 不用：FIELD_DISABLED */
#define TYBC_TEMP_SET_TITLE                        (TY_MSGID_TIT_SET_TEMP)	/* 字符串ID */
#define TYBC_TEMP_SET_ADDR                         ((ac_mode==MODE_COOL)?P_idx(sv_cool):P_idx(sv_heat))	/* 参数索引 */
#define TYBC_TEMP_SET_VAL                          (SHC_get_set_tmp_A())/* 设定值 */
#define TYBC_TEMP_SET_MAX                          (SHC_get_set_tmp_max_A())  /* 上限 */
#define TYBC_TEMP_SET_MIN                          (SHC_get_set_tmp_min_A())  /* 下限 */
#define TYBC_TEMP_SET_ATTR                         ((TYBC_cal_para_attrib(GROUP_USER,TYBC_TEMP_SET_ADDR) \
                                                   &(TYBC_UNIT|TYBC_DECIMAL))|STEP_1)  /* 属性(注意：设定温度参数值、最大值、最小值的小数点位数需一致) */

/* 字段2: 当前温度 */
#define TYBC_TEMP_CUR_TYPE                         (FIELD_RO_NUM)    /* 使用：FIELD_RO_NUM 不用：FIELD_DISABLED */
#define TYBC_TEMP_CUR_TITLE                        (TY_MSGID_TIT_CUR_TEMP)                  /* 字符串ID */
#define TYBC_TEMP_CUR_VAL                          (ctrl_temp_energy[SUB_SYS_AC_ONLY])	/* 总出水温度当前值 */
#define TYBC_TEMP_CUR_ATTR                         ((TYBC_sign_local2sh(UINT_C|D_DOT1) \
                                                   &(TYBC_UNIT|TYBC_DECIMAL))|STEP_1)     /* 属性 */

/* 字段3: 设定模式 */
#define TYBC_MODE_SET_TYPE                         (FIELD_ENUM)      /* 使用：FIELD_ENUM 不用：FIELD_DISABLED */
#define TYBC_MODE_SET_TITLE                        (TY_MSGID_TIT_SET_MODE)               /* 字符串ID */
#define TYBC_MODE_SET_VAL                          (TYBC_mode_set_to_rn_enum(sv_mode))/* 当前值 */
#define TYBC_MODE_SET_RANGE                        (TYBC_get_mode_set_range())        /* 范围 */
#define TYBC_MODE_SET_MAX_NUM                      (SET_MODE_MAX-1)                     /* 总枚举数 */

/* 字段4: 当前模式 */
#define TYBC_MODE_CUR_TYPE                         (FIELD_ENUM)      /* 使用：FIELD_ENUM 不用：FIELD_DISABLED */
#define TYBC_MODE_CUR_TITLE                        (TY_MSGID_NULL)                         /* 字符串ID */
#define TYBC_MODE_CUR_VAL                          (TYBC_mode_set_to_rn_enum(ac_mode))  /* 当前值 */
#define TYBC_MODE_CUR_RANGE                        (TYBC_get_mode_set_range())            /* 范围 */
#define TYBC_MODE_CUR_MAX_NUM                      (SET_MODE_MAX-1)                         /* 总枚举数 */

/* 字段5: 控制末端 */
#define TYBC_CTRL_END_VALUE                        (ZONE_A_TERMINAL_DEVICE) //区域一末端类型

/* 字段6: 控制对象 */
#define TYBC_CTRL_OBJ                              (sh_main_info.zone_1_temp_type)

/* 字段7: 压机状态 */
#define TYBC_COMP_STA                              (sh_main_info.fg_zone_1_comp_on)

/* 字段8: 除霜状态 */
#define TYBC_DEFRO_STA                         	   (sh_main_info.zone_1_status == ZONE_STA_DEFRO)

/* 字段9: 防冻状态 */
#define TYBC_ANFREZ_STA                            (sh_main_info.zone_1_status == ZONE_STA_ANTIFREZ)

/* 字段10: 外部热源输出状态 */
#define TYBC_OUTHEAT_STA                           (sh_main_info.zone_1_addition== ZONE_ADDITION_HEAT)

/* 字段11: 杀菌状态 */
#define TYBC_STERILIZE_STA                         (0)

/* 字段12: 辅助电热状态 */
#define TYBC_AUXHEAT_STA                           (sh_main_info.zone_1_addition == ZONE_ADDITION_ELEC)

/* 字段13: 水箱电热状态 */
#define TYBC_TANKHEAT_STA                          (0)

/* 字段14: 静音模式状态 */
#define TYBC_SILENT_STA                            (sh_main_info.zone_1_status == ZONE_STA_SILENT)

/* 字段15: 节能模式状态 */
#define TYBC_ENESAVING_STA                         (sh_main_info.zone_1_status == ZONE_STA_ECO)

/* 字段16: 假日状态 */
#define TYBC_HOLIDAY_STA                           (sh_main_info.zone_1_status == ZONE_STA_HOLIDAY)

/* 字段17: 太阳能信号状态 */
#define TYBC_SOLAR_STA                             (0)

/* ========================================================================================== */
/* 区域一功能命令处理                                                                           */
/* ========================================================================================== */
/* 开关机命令 */
#define TYBC_CMD_RUN                               (SHC_command_ON_zone_1_deal(TRUE))
#define TYBC_CMD_STOP                              (SHC_command_OFF_zone_1_deal(TRUE))

/* ========================================================================================== */
/* 区域二功能信息配置                                                                         */
/* ========================================================================================== */
/* 字段0: 开关机 */
#define TYBC_ZoneB_SWITCH_TYPE                           (FIELD_BOOL)      /* 使用：FIELD_BOOL 字段不用：FIELD_DISABLED */
#define TYBC_ZoneB_SWITCH_TITLE                          (TY_MSGID_TIT_SWITCH)        /* 字符串ID */
#define TYBC_ZoneB_SWITCH_VAL                            (sh_main_info.fg_zone_2_is_run)   /* 当前值 */

/* 字段1: 设定温度 */
#define TYBC_ZoneB_TEMP_SET_TYPE                         (FIELD_NUMBER)    /* 使用：FIELD_NUMBER 不用：FIELD_DISABLED */
#define TYBC_ZoneB_TEMP_SET_TITLE                        (TY_MSGID_TIT_SET_TEMP)	/* 字符串ID */
#define TYBC_ZoneB_TEMP_SET_ADDR                         ((ac_mode==MODE_COOL)?P_idx(sv_cool):P_idx(sv_heat))	/* 参数索引 */
#define TYBC_ZoneB_TEMP_SET_VAL                          (SHC_get_set_tmp_B())/* 设定值 */
#define TYBC_ZoneB_TEMP_SET_MAX                          (SHC_get_set_tmp_max_B())  /* 上限 */
#define TYBC_ZoneB_TEMP_SET_MIN                          (SHC_get_set_tmp_min_B())  /* 下限 */
#define TYBC_ZoneB_TEMP_SET_ATTR                         ((TYBC_cal_para_attrib(GROUP_USER, TYBC_ZoneB_TEMP_SET_ADDR) \
                                                   		 &(TYBC_UNIT|TYBC_DECIMAL))|STEP_1)  /* 属性(注意：设定温度参数值、最大值、最小值的小数点位数需一致) */

/* 字段2: 当前温度 */
#define TYBC_ZoneB_TEMP_CUR_TYPE                         (FIELD_RO_NUM)    /* 使用：FIELD_RO_NUM 不用：FIELD_DISABLED */
#define TYBC_ZoneB_TEMP_CUR_TITLE                        (TY_MSGID_TIT_CUR_TEMP)                  /* 字符串ID */
#define TYBC_ZoneB_TEMP_CUR_VAL                          (SHC_get_ctrl_tmp_B())	/* 第二个温度当前值 */
#define TYBC_ZoneB_TEMP_CUR_ATTR                         ((TYBC_sign_local2sh(UINT_C|D_DOT1) \
                                                   		 &(TYBC_UNIT|TYBC_DECIMAL))|STEP_1)     /* 属性 */

/* 字段3: 设定模式 */
#define TYBC_ZoneB_MODE_SET_TYPE                         (FIELD_ENUM)      /* 使用：FIELD_ENUM 不用：FIELD_DISABLED */
#define TYBC_ZoneB_MODE_SET_TITLE                        (TY_MSGID_TIT_SET_MODE)               /* 字符串ID */
#define TYBC_ZoneB_MODE_SET_VAL                          (TYBC_mode_set_to_rn_enum(sv_mode))/* 当前值 */
#define TYBC_ZoneB_MODE_SET_RANGE                        (TYBC_get_mode_set_range())        /* 范围 */
#define TYBC_ZoneB_MODE_SET_MAX_NUM                      (SET_MODE_MAX-1)                     /* 总枚举数 */

/* 字段4: 当前模式 */
#define TYBC_ZoneB_MODE_CUR_TYPE                         (FIELD_ENUM)      /* 使用：FIELD_ENUM 不用：FIELD_DISABLED */
#define TYBC_ZoneB_MODE_CUR_TITLE                        (TY_MSGID_NULL)                         /* 字符串ID */
#define TYBC_ZoneB_MODE_CUR_VAL                          (TYBC_mode_set_to_rn_enum(ac_mode))  /* 当前值 */
#define TYBC_ZoneB_MODE_CUR_RANGE                        (TYBC_get_mode_set_range())            /* 范围 */
#define TYBC_ZoneB_MODE_CUR_MAX_NUM                      (SET_MODE_MAX-1)                         /* 总枚举数 */

/* 字段5: 控制末端 */
#define TYBC_ZoneB_CTRL_END_VALUE                        (ZONE_B_TERMINAL_DEVICE) //区域二末端类型

/* 字段6: 控制对象 */
#define TYBC_ZoneB_CTRL_OBJ                              (sh_main_info.zone_2_temp_type)

/* 字段7: 压机状态 */
#define TYBC_ZoneB_COMP_STA                              (sh_main_info.fg_zone_2_comp_on)

/* 字段8: 除霜状态 */
#define TYBC_ZoneB_DEFRO_STA                         	   (sh_main_info.zone_2_status == ZONE_STA_DEFRO)

/* 字段9: 防冻状态 */
#define TYBC_ZoneB_ANFREZ_STA                            (sh_main_info.zone_2_status == ZONE_STA_ANTIFREZ)

/* 字段10: 外部热源输出状态 */
#define TYBC_ZoneB_OUTHEAT_STA                           (sh_main_info.zone_2_addition== ZONE_ADDITION_HEAT)

/* 字段11: 杀菌状态 */
#define TYBC_ZoneB_STERILIZE_STA                         (0)

/* 字段12: 辅助电热状态 */
#define TYBC_ZoneB_AUXHEAT_STA                           (sh_main_info.zone_2_addition == ZONE_ADDITION_ELEC)

/* 字段13: 水箱电热状态 */
#define TYBC_ZoneB_TANKHEAT_STA                          (0)

/* 字段14: 静音模式状态 */
#define TYBC_ZoneB_SILENT_STA                            (sh_main_info.zone_2_status == ZONE_STA_SILENT)

/* 字段15: 节能模式状态 */
#define TYBC_ZoneB_ENESAVING_STA                         (sh_main_info.zone_2_status == ZONE_STA_ECO)

/* 字段16: 假日状态 */
#define TYBC_ZoneB_HOLIDAY_STA                           (sh_main_info.zone_2_status == ZONE_STA_HOLIDAY)

/* 字段17: 太阳能信号状态 */
#define TYBC_ZoneB_SOLAR_STA                             (0)


/* ========================================================================================== */
/* 区域二功能命令处理                                                                           */
/* ========================================================================================== */
/* 开关机命令 */
#define TYBC_ZoneB_CMD_RUN                               (SHC_command_ON_zone_2_deal(TRUE))
#define TYBC_ZoneB_CMD_STOP                              (SHC_command_OFF_zone_2_deal(TRUE))

/* ========================================================================================== */
/* 热水功能信息配置                                                                         */
/* ========================================================================================== */
/* 字段0: 开关机 */
#define TYBC_DHW_SWITCH_TYPE                           	 (FIELD_BOOL)      /* 使用：FIELD_BOOL 字段不用：FIELD_DISABLED */
#define TYBC_DHW_SWITCH_TITLE                          	 (TY_MSGID_TIT_SWITCH)        /* 字符串ID */
#define TYBC_DHW_SWITCH_VAL                              (sh_main_info.fg_zone_hotw_is_run)/* 当前值 */

/* 字段1: 设定温度 */
#define TYBC_DHW_TEMP_SET_TYPE                           (FIELD_NUMBER)    /* 使用：FIELD_NUMBER 不用：FIELD_DISABLED */
#define TYBC_DHW_TEMP_SET_TITLE                          (TY_MSGID_TIT_SET_DHW_TEMP)	/* 字符串ID */
#define TYBC_DHW_TEMP_SET_ADDR                           (P_idx(sv_hotw))	/* 参数索引 */
#define TYBC_DHW_TEMP_SET_VAL                            (sv_hotw/10)/* 设定值 */
#define TYBC_DHW_TEMP_SET_MAX                            (para_get_max(P_GaI(sv_hotw)))  /* 上限 */
#define TYBC_DHW_TEMP_SET_MIN                            (para_get_min(P_GaI(sv_hotw)))  /* 下限 */
#define TYBC_DHW_TEMP_SET_ATTR                           ((TYBC_cal_para_attrib(GROUP_USER, TYBC_DHW_TEMP_SET_ADDR) \
                                                   		 &(TYBC_UNIT|TYBC_DECIMAL))|STEP_1)  /* 属性(注意：设定温度参数值、最大值、最小值的小数点位数需一致) */
/* 字段2: 当前温度 */
#define TYBC_DHW_TEMP_CUR_TYPE                        	 (FIELD_RO_NUM)    /* 使用：FIELD_RO_NUM 不用：FIELD_DISABLED */
#define TYBC_DHW_TEMP_CUR_TITLE                        	 (TY_MSGID_TIT_CUR_DHW_TEMP)                  /* 字符串ID */
#define TYBC_DHW_TEMP_CUR_VAL                       	 (pv_sys_THWt)	/* 总出水温度当前值 */
#define TYBC_DHW_TEMP_CUR_ATTR                    		 ((TYBC_sign_local2sh(UINT_C|D_DOT1) \
                                                  		  &(TYBC_UNIT|TYBC_DECIMAL))|STEP_1)

/* 字段3: 设定模式 */
#define TYBC_DHW_MODE_SET_TYPE                     		 (FIELD_ENUM)      /* 使用：FIELD_ENUM 不用：FIELD_DISABLED */
#define TYBC_DHW_MODE_SET_TITLE                        	 (TY_MSGID_TIT_SET_MODE)               /* 字符串ID */
#define TYBC_DHW_MODE_SET_VAL                            (TYBC_mode_set_to_rn_enum(sv_mode))/* 当前值 */
#define TYBC_DHW_MODE_SET_RANGE                       	 (TYBC_get_mode_set_range())        /* 范围 */
#define TYBC_DHW_MODE_SET_MAX_NUM                     	 (SET_MODE_MAX-1)                     /* 总枚举数 */

/* 字段4: 当前模式 */
#define TYBC_DHW_MODE_CUR_TYPE                         	 (FIELD_ENUM)      /* 使用：FIELD_ENUM 不用：FIELD_DISABLED */
#define TYBC_DHW_MODE_CUR_TITLE                        	 (TY_MSGID_NULL)                         /* 字符串ID */
#define TYBC_DHW_MODE_CUR_VAL                          	 (TYBC_mode_set_to_rn_enum(ac_mode))  /* 当前值 */
#define TYBC_DHW_MODE_CUR_RANGE                       	 (TYBC_get_mode_set_range())            /* 范围 */
#define TYBC_DHW_MODE_CUR_MAX_NUM                      	 (SET_MODE_MAX-1)                         /* 总枚举数 */

/* 字段5: 控制末端 */
#define TYBC_DHW_CTRL_END_VALUE                          (ZONE_MODE_HOTW) //热水末端类型

/* 字段6: 控制对象 */
#define TYBC_DHW_CTRL_OBJ                                (0)

/* 字段7: 压机状态 */
#define TYBC_DHW_COMP_STA                                (sh_main_info.fg_zone_hotw_comp_on)

/* 字段8: 除霜状态 */
#define TYBC_DHW_DEFRO_STA                         	     (sh_main_info.zone_hotw_status == ZONE_STA_DEFRO)

/* 字段9: 防冻状态 */
#define TYBC_DHW_ANFREZ_STA                           	 (sh_main_info.zone_hotw_status == ZONE_STA_ANTIFREZ)

/* 字段10: 外部热源输出状态 */
#define TYBC_DHW_OUTHEAT_STA                           	 (sh_main_info.zone_hotw_addition== ZONE_ADDITION_HEAT)

/* 字段11: 杀菌状态 */
#define TYBC_DHW_STERILIZE_STA                         	 (sh_main_info.fg_zone_hotw_disinfect)

/* 字段12: 辅助电热状态 */
#define TYBC_DHW_AUXHEAT_STA                          	 (sh_main_info.zone_hotw_addition== ZONE_ADDITION_ELEC)

/* 字段13: 水箱电热状态 */
#define TYBC_DHW_TANKHEAT_STA                         	 (sh_main_info.zone_hotw_addition== ZONE_ADDITION_TANK_HEATER)

/* 字段14: 静音模式状态 */
#define TYBC_DHW_SILENT_STA                           	 (sh_main_info.zone_hotw_status == ZONE_STA_SILENT)

/* 字段15: 节能模式状态 */
#define TYBC_DHW_ENESAVING_STA                        	 (sh_main_info.zone_hotw_status == ZONE_STA_ECO)

/* 字段16: 假日状态 */
#define TYBC_DHW_HOLIDAY_STA                          	 (sh_main_info.zone_hotw_status == ZONE_STA_HOLIDAY)

/* 字段17: 太阳能信号状态 */
#define TYBC_DHW_SOLAR_STA                             	 (sh_main_info.zone_hotw_addition== ZONE_ADDITION_SOLAR)


/* ========================================================================================== */
/* 热水功能命令处理                                                                           */
/* ========================================================================================== */
/* 开关机命令 */
#define TYBC_DHW_CMD_RUN                              		(SHC_command_ON_hotwater_deal(TRUE))
#define TYBC_DHW_CMD_STOP                              		(SHC_command_OFF_hotwater_deal(TRUE))


/* ========================================================================================== */
/* 扩展功能信息配置                                                                           */
/* ========================================================================================== */
/* 有"风速功能" */
#ifdef TUYA_USE_WIND
/* 字段0: 设定风速 */
#define TYBC_WIND_SET_TYPE                         (FIELD_ENUM)      /* 使用：FIELD_ENUM 不用：FIELD_DISABLED */
#define TYBC_WIND_SET_TITLE                        (msg_tyb_title_wind_set)
#define TYBC_WIND_SET_RANGE                        (TYBC_get_wind_set_range())
#define TYBC_WIND_SET_MAX_NUM                      (RN_WIND_MAX)
#define TYBC_WIND_SET_VAL                          (TYBC_wind_set_to_rn_enum(0))

/* 字段1: 当前风速 */
#define TYBC_WIND_CUR_TYPE                         (FIELD_ENUM)      /* 使用：FIELD_ENUM 不用：FIELD_DISABLED */
#define TYBC_WIND_CUR_TITLE                        (msg_tyb_title_wind_cur)
#define TYBC_WIND_CUR_RANGE                        (TYBC_get_wind_cur_range())
#define TYBC_WIND_CUR_MAX_NUM                      (RN_WIND_MAX)
#define TYBC_WIND_CUR_VAL                          (TYBC_wind_cur_to_rn_enum(0))
#endif

/* 有"湿度功能" */
#ifdef TUYA_USE_HUMI
/* 字段2: 设定湿度(预留,未启用) */
#define TYBC_HUMI_SET_TYPE                         (FIELD_NUMBER)    /* 使用：FIELD_NUMBER 不用：FIELD_DISABLED */
#define TYBC_HUMI_SET_TITLE                        (msg_tyb_null)
#define TYBC_HUMI_SET_VAL                          (20)
#define TYBC_HUMI_SET_MAX                          (100)
#define TYBC_HUMI_SET_MIN                          (0)
#define TYBC_HUMI_SET_DECIMAL                      (0)
/* 注意：设定湿度参数值、最大值、最小值的小数点位数需一致 */
#define TYBC_HUMI_SET_ATTR                         (((6)&(TYBC_UNIT|TYBC_DECIMAL))|STEP_1)

/* 字段3: 当前湿度(预留,未启用) */
#define TYBC_HUMI_CUR_TYPE                         (FIELD_RO_NUM)    /* 使用：FIELD_RO_NUM 不用：FIELD_DISABLED */
#define TYBC_HUMI_CUR_TITLE                        (msg_tyb_null)
#define TYBC_HUMI_CUR_VAL                          (10)
#define TYBC_HUMI_CUR_ATTR                         (((6)&(TYBC_UNIT|TYBC_DECIMAL))|STEP_1)
#endif

/* 有"热水功能" */
#ifdef TUYA_USE_HOT_WATER
/* 字段4: 热水设定温度 */
#define TYBC_HOT_WATER_SET_TYPE                    (FIELD_NUMBER)    /* 使用：FIELD_NUMBER 不用：FIELD_DISABLED */
#define TYBC_HOT_WATER_SET_TITLE                   (msg_tyb_title_hot_water_set)
#define TYBC_HOT_WATER_SET_VAL                     (sv_hotw/10)	/* 热水设定温度 */
#define TYBC_HOT_WATER_SET_MAX                     (HIEST_HOTW)
#define TYBC_HOT_WATER_SET_MIN                     (LOEST_HOTW)
#define TYBC_HOT_WATER_SET_ATTR                    ((TYBC_cal_para_attrib(P_grp(sv_hotw),P_idx(sv_hotw)) \
                                                   &(TYBC_UNIT|TYBC_DECIMAL))|STEP_1)

/* 字段5: 当前热水温度 */
#define TYBC_HOT_WATER_CUR_TYPE                    (FIELD_RO_NUM)    /* 使用：FIELD_RO_NUM 不用：FIELD_DISABLED */
#define TYBC_HOT_WATER_CUR_TITLE                   (msg_tyb_title_hot_water_cur)
#define TYBC_HOT_WATER_CUR_VAL                     (pv_sys_THWt)	/* 热水水箱温度 */
#define TYBC_HOT_WATER_CUR_ATTR                    ((TYBC_sign_local2sh(UINT_C|D_DOT1) \
                                                   &(TYBC_UNIT|TYBC_DECIMAL))|STEP_1)
#endif


/* ========================================================================================== */
/* 状态查询功能配置                                                                           */
/* ========================================================================================== */
/* 有"状态查询" */
#if (defined(TUYA_USE_SYS_STAT) || defined(TUYA_USE_MOD_STAT))
#define TYBC_PV_VAL(pcb_idx,pv_idx)                (TYBC_get_pv_val(pcb_idx,pv_idx))                   /* 模拟量数值 */
#endif

/* 有"系统状态" */
#ifdef TUYA_USE_SYS_STAT
/* 系统模拟量 */
#define TYBC_SYS_PV_TITLE(idx)                     (msg_sys_p[idx].msg)                              /* 名称 */
#define TYBC_SYS_PV_ATTR(idx)                      (TYBC_sign_local2sh(msg_sys_p[(idx)].attr))        /* 属性 */
#define TYBC_SYS_PV_IN_USED                        (get_sys_p_num())                                 /* 实际使用个数 */
#endif

/* 有"模块状态" */
#ifdef TUYA_USE_MOD_STAT
/* 模块单元模拟量 */
#define TYBC_UNIT_PV_TITLE(idx)                    (msg_uint_p_cell[idx].msg)                        /* 名称 */
#define TYBC_UNIT_PV_ATTR(idx)                     (TYBC_sign_local2sh(msg_uint_p_cell[(idx)].attr))  /* 属性 */
#define TYBC_UNIT_PV_IN_USED                       (get_unit_p_num())                                /* 实际使用个数 */
/* 模块压机模拟量 */
#define TYBC_COMP_PV_TITLE(idx)                    (msg_uint_p_comp[idx].msg)                        /* 名称 */
#define TYBC_COMP_PV_ATTR(idx)                     (TYBC_sign_local2sh(msg_uint_p_comp[idx].attr))    /* 属性 */
#define TYBC_COMP_PV_IN_USED                       (get_comp_p_num())                                /* 实际使用个数 */
#endif


/* 有"当前故障" */
#ifdef TUYA_USE_ERR_CUR
/* ========================================================================================== */
/* 查询当前故障功能配置                                                                       */
/* ========================================================================================== */
#define TYBC_GET_ERR_CUR(err,s_e_id,m_id,m_e_id)   (TYBC_get_err_cur(err, s_e_id, m_id, m_e_id))  /* 获取当前故障 */
#define TYBC_CUR_ERR_NUM                           (TYBC_get_err_cur_num())                       /* 当前故障数 */
#endif


/* 有"历史故障" */
#ifdef TUYA_USE_ERR_HIS
/* ========================================================================================== */
/* 查询历史故障功能配置                                                                       */
/* ========================================================================================== */
#define TYBC_GET_ERR_HIS(err,idx)                  (TYBC_get_err_his_info(err, idx))  /* 获取一条历史故障信息 */
#define TYBC_HIS_ERR_TBL_SIZE                      (HIS_ERR_SIZE)
#define TYBC_HIS_ERR_MAX                           (MAX_HIS_ERROR)
#endif


/* ========================================================================================== */
/* 参数设置功能配置                                                                           */
/* ========================================================================================== */
/* 有"参数设置" */
#ifdef TUYA_USE_PARAM_SET
/* 参数组 */
#define TYBC_PARAM_GRP_TITLE(grp_id)               (para_large[grp_id])     /* 名称 */
#define TYBC_PARAM_GRP_ATTR(grp_id)                (TYBC_cal_grp_attrib((U08)grp_id))       /* 属性 */
#define TYBC_PARAM_GRP_NUM                         (PARA_GROUP_NUM)                         /* 实际使用个数 */
#define TYBC_PARAM_ITEM_NUM(grp_id)                (ParamPri[grp_id].items)                 /* 参数组中的参数实际使用的个数，留意ParamPri[x].items */
/* 参数项 */
#define TYBC_PARAM_ITEM_TITLE(grp_id,item_id)      (para_small[(grp_id)][(item_id)].msg)                 /* 名称 */
#define TYBC_PARAM_ITEM_VAL(grp_id,item_id)        (para_get_val(grp_id, item_id))                    /* 当前值 */
#define TYBC_PARAM_MIN(grp_id,item_id)             (para_get_min(grp_id,item_id))   /* 最小值 */
#define TYBC_PARAM_MAX(grp_id,item_id)             (para_get_max(grp_id,item_id))   /* 最大值 */
#define TYBC_PARAM_ITEM_ATTR(grp_id,item_id)       (TYBC_cal_para_attrib(grp_id,item_id))                /* 属性 */
#define TYBC_PARAM_DICT_NUM                        (get_dict_num())                                      /* 参数选项字符串组数 */
#define TYBC_PARAM_DICT_ITEM_NUM(dict_idx)         (para_dict[dict_idx].items)                           /* 一个参数的选项字符串总数 */
#define TYBC_PARAM_DICT_ITEM(dict_idx, idx)        (para_dict[dict_idx].pmsg[idx])                       /* 参数选项名称字符串 */
#define TYBC_SET_PARAM(grp_id,item_id,value)       (para_set(grp_id,item_id,value)) /* 设置参数值 */
#endif


/* 只支持从机屏 */
#if defined(TUYA_USE_HMI_SLAVE) && !defined(TUYA_USE_HMI_MASTER)
/* ========================================================================================== */
/* 注:此部分内容参考自SmartHmi，此处内容以SmartHmi最新版本为准，一般无需修改                  */
/* ========================================================================================== */
#define TYBC_GET_M_ERR(err)                        (msg_m_error[err])/* 系统故障名称 */
#define TYBC_GET_S_ERR(err)                        (msg_s_error[err])/* 模块故障名称 */

/* 故障时间 */
typedef struct tag_TYBC_ERR_TIME
{
    U16 month;                                     /* 月 (月日同为0则不显示月日) */
    U16 day;                                       /* 日 */
    U16 hour;                                      /* 时 (时分同为0则不显示时分) */
    U16 mint;                                      /* 分 */
} TYBC_ERR_TIME;

/* 历史故障信息 */
typedef struct tag_TYBC_ERR_HIS
{
    U16 name;                                      /* 名称字符串号 */
    U16 id;                                        /* 序号(高8位为板号，低8位为单元号，为0xff时不显示序号) */
    U16 err_id;                                    /* 故障号 */
    TYBC_ERR_TIME time;                            /* 故障时间 */
} TYBC_ERR_HIS;
#define TYBC_ERR_WEI_ZHI_GZ                        (0)

/* 当前故障信息 */
typedef struct tag_TYBC_ERR_CUR
{
    U16 name;                                     /* 名称字符串号 */
    U16 id;                                       /* 序号(高8位为板号，低8位为单元号，为0xff时不显示序号) */
    U16 err_id;                                   /* 故障号 */
} TYBC_ERR_CUR;

/* 单位 */
#define TYBC_UNIT_NULL                             (0)       /* 0 无 */
#define TYBC_UNIT_TMP_C                            (1)       /* 1 温度(℃) */
#define TYBC_UNIT_TMP_F                            (2)       /* 2 温度(℉) */
#define TYBC_UNIT_A                                (3)       /* 3 电流(A) */
#define TYBC_UNIT_mA                               (4)       /* 4 电流(mA) */
#define TYBC_UNIT_PRS_B                            (5)       /* 5 压力(bar) */
#define TYBC_UNIT_PE                               (6)       /* 6 百分比(%) */
#define TYBC_UNIT_HZ                               (7)       /* 7 频率(HZ) */
#define TYBC_UNIT_RPS                              (8)       /* 8 转速(rps) */
#define TYBC_UNIT_RPM                              (9)       /* 9 转速(rpm) */
#define TYBC_UNIT_YEAR                             (10)      /* 10 年 */
#define TYBC_UNIT_MONTH                            (11)      /* 11 月 */
#define TYBC_UNIT_DAY                              (12)      /* 12 天 */
#define TYBC_UNIT_HOUR                             (13)      /* 13 时 */
#define TYBC_UNIT_MINT                             (14)      /* 14 分 */
#define TYBC_UNIT_SECOND                           (15)      /* 15 秒 */
#define TYBC_UNIT_ST                               (16)      /* 16 步 */
#define TYBC_UNIT_V                                (17)      /* 17 电压(V) */
#define TYBC_UNIT_mV                               (18)      /* 18 电压(mV) */
#define TYBC_UNIT_kO                               (19)      /* 19 电阻(kΩ) */
#define TYBC_UNIT_kW                               (20)      /* 20 功率(kW) */
#define TYBC_UNIT_kWH                              (21)      /* 20 能耗(kWH) */
#define TYBC_UNIT_MAX                              (22)      /* 最大单位数 */

/* 密码权限 */
#define TYBC_PWR_NONE                              (0)       /* 未登录 */
#define TYBC_PWR_SALE                              (1)       /* 经销商(使用期限) */
#define TYBC_PWR_USER                              (2)       /* 用户 */
#define TYBC_PWR_PRO                               (4)       /* 工程调试员 */
#define TYBC_PWR_SER                               (6)       /* 厂家维修员 */
#define TYBC_PWR_FAC                               (9)       /* 厂家管理员 */
#define TYBC_PWR_SPR                               (10)      /* 超级密码 */

/* 参数权限 */
#define TYBC_PARA_USR                              (0)       /* 用户 */
#define TYBC_PARA_PRO                              (1)       /* 工程 */
#define TYBC_PARA_SER                              (2)       /* 维修 */
#define TYBC_PARA_FAC                              (3)       /* 厂家 */

#define TYBC_UNIT_OFFSET                           (32)      /* 单位偏移量 */
#define TYBC_DECIMAL_BGN_BIT                       (8)       /* 小数位数 起始位 */
#define TYBC_HIDE_BGN_BIT                          (10)      /* 隐藏属性 起始位 */
#define TYBC_CONST_BGN_BIT                         (11)      /* 不可修改 起始位 */
#define TYBC_SAFE_BGN_BIT                          (12)      /* 参数权限 起始位 */
#define TYBC_BIT_BGN_BIT                           (15)      /* 位变量   起始位 */

#define TYBC_UNIT                                  (0x00ff)  /* bit0~7:   0~255，若值小于32,表示为数值型，值为单位号，最多支持32个单位；若值大于32，表示为字符串型参数，值减去偏差值32之后就是字符组号 */
#define TYBC_DECIMAL                               (0x0300)  /* bit8~9:   小数位数(0~3) */

#define TYBC_NO_ID                                 (0xff)    /* 0xff不显示序号 */

/* 同时支持从机屏+主机屏时，以下内容均从SmartHmi中引用 */
#elif (defined(TUYA_USE_HMI_SLAVE) && defined(TUYA_USE_HMI_MASTER))
typedef SH_ERR_HIS TYBC_ERR_HIS;
typedef SH_ERR_CURT TYBC_ERR_CUR;
#define TYBC_UNIT                                  (SH_UNIT)
#define TYBC_DECIMAL                               (SH_DECIMAL)
#define TYBC_sign_local2sh(sign_local)             ((((U16)sign_local) == 0xFFFF) ? (0xFFFE)\
                                                                           : SH_sign_local2sh((U16)sign_local))
#define TYBC_cal_grp_attrib(grp_id)                (SH_cal_grp_attrib((U08)grp_id))
#define TYBC_cal_para_attrib(grp_id,item_id)       (SH_cal_para_attrib((U08)grp_id,(U08)item_id))
#define TYBC_get_err_cur(e, s_e_id, m_id, m_e_id)  (SHC_get_curt_err((TYBC_ERR_CUR*)e,(U16*)s_e_id,(U16*)m_id,(U16*)m_e_id))
#define TYBC_get_err_cur_num()                     (SHC_get_err_num())
#define TYBC_get_err_his_info(err,idx)             (SHC_get_his_err_info((TYBC_ERR_HIS*)err,(U08)idx))
#endif


/* 以下内容请勿修改 */
TUYA_B_CFG_EXT U08 TYBC_language_to_rn_enum(U08 local_language);
TUYA_B_CFG_EXT U08 TYBC_language_to_local_enum(U08 rn_language);
TUYA_B_CFG_EXT U08 *TYBC_get_password_pointer(void);
TUYA_B_CFG_EXT BOOL TYBC_check_password_modified(void);
TUYA_B_CFG_EXT U16 *TYBC_get_language_range(void);
TUYA_B_CFG_EXT void TYBC_language_set(U08 val);
TUYA_B_CFG_EXT BOOL TYBC_get_switch_status(void);
TUYA_B_CFG_EXT U08 TYBC_mode_set_to_rn_enum(U08 local_mode);
TUYA_B_CFG_EXT U16 *TYBC_get_mode_set_range(void);
TUYA_B_CFG_EXT U08 TYBC_run_status_to_rn_enum(U08 local_enum);
TUYA_B_CFG_EXT void TYBC_temp_set(I16 cloud_val, U08 dpid);
TUYA_B_CFG_EXT void TYBC_mode_set(I16 cloud_val);
TUYA_B_CFG_EXT U16 *TYBC_get_wind_set_range(void);
TUYA_B_CFG_EXT U08 TYBC_wind_set_to_rn_enum(U08 local_wind_speed);
TUYA_B_CFG_EXT U16 *TYBC_get_wind_cur_range(void);
TUYA_B_CFG_EXT U08 TYBC_wind_cur_to_rn_enum(U08 local_wind_speed);
TUYA_B_CFG_EXT void TYBC_wind_set(I16 cloud_val);
TUYA_B_CFG_EXT void TYBC_humi_set(I16 cloud_val);
TUYA_B_CFG_EXT void TYBC_hot_water_set(I16 cloud_val);
TUYA_B_CFG_EXT void TYBC_config_timer(void);
TUYA_B_CFG_EXT void TYBC_config_init(void);
#ifndef TUYA_USE_HMI_MASTER
TUYA_B_CFG_EXT U16 TYBC_sign_local2sh(U16 sign_local);
/* 有"当前故障" */
#ifdef TUYA_USE_ERR_CUR
TUYA_B_CFG_EXT BOOL TYBC_get_err_cur(TYBC_ERR_CUR *err, U16 *sys_err_id, U16 *mod_id, U16 *mod_err_id);
TUYA_B_CFG_EXT U16 TYBC_get_err_cur_num(void);
#endif
/* 有"历史故障" */
#ifdef TUYA_USE_ERR_HIS
TUYA_B_CFG_EXT void TYBC_get_err_his_info(TYBC_ERR_HIS *err, U08 idx);
#endif
/* 有"参数设置" */
#ifdef TUYA_USE_PARAM_SET
TUYA_B_CFG_EXT U16 TYBC_cal_grp_attrib(U08 grp);
TUYA_B_CFG_EXT U16 TYBC_cal_para_attrib(U08 grp, U08 item);
#endif
#endif

TUYA_B_CFG_EXT I16 TYBC_get_pv_val(U16 pcb_idx, U16 pv_idx);
TUYA_B_CFG_EXT U16 *TYBC_get_temp_unit_range(void);

