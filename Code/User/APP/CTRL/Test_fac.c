/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: Test_fac.c
 文件描述: 
 创建人  : 
 创建日期: 
*******************************************************************************/

#define  TEST_FAC_GLOBAL
#include "includes.h"

#define TF_STRCPY fun_copy_str_to_U16

U16 TF_err_id;
enum 
{  
    TF_ERR_IBH,             /*  0 IBH故障 */
    TF_ERR_E3,              /*  1 翅片温度故障 */
    TF_ERR_E8,              /*  2 排气温度故障 */
    TF_ERR_HMI,             /*  3 显示屏通讯故障 */
    TF_ERR_SW,              /*  4 拨码开关故障 */
    TF_ERR_ADDR,            /*  5 拨盘地址故障 */
    TF_ERR_OTHER,           /*  6 商检中未定义的其他故障 */
    TF_ERR_WIFI_NO_MODULE,  /*  7 无WiFi模组 */
    TF_ERR_WIFI_FAIL,       /*  8 WiFi模组故障 */
    TF_ERR_WIFI_OVER,       /*  9 WiFi测试超时 */
    TF_ERR_MAX              /* 10 超过16时需改程序 */
};

U16 TF_step;
enum {TF_STEP_IDLE,     TF_STEP_READY,     TF_STEP_TESTING,    TF_STEP_PASS,   TF_STEP_MAX};

U16 TF_cnt_in_step;         /* 内机商检步骤计时 */
U16 TF_cnt_out_step;        /* 外机商检步骤计时 */
U16 TF_cnt_open_cm;         /* 开启压机间隔 */
U16 TF_cnt_hmi;             /* hmi故障计时 */
U16 TF_cnt_do_dly;          /* 继电器输出间隔 */
U16 TF_cnt_wifi_dly;        /* wifi测试超时 */


/*******************************************************************************
 函 数: TF_Time()
 功 能: 处理时间
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TF_Time(void)
{
    if (F_TIME_BASIC_100MS_BACK)
    {
        _CNT(TF_cnt_do_dly); 
    }
    
    if (F_TIME_BASIC_SEC_BACK)
    {
        _CNT(TF_cnt_in_step); 
        _CNT(TF_cnt_out_step);
        _CNT(TF_cnt_open_cm);
        _CNT(TF_cnt_hmi);
        _CNT(TF_cnt_wifi_dly);
    }
}

/*******************************************************************************
 函 数: TF_logic()
 功 能: 处理商检逻辑
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TF_logic(void)
{
    static U08 TF_in_step = 0;
    static U08 TF_out_step = 0;
    static BOL fg_TF_IN_pass = FALSE;
    static BOL fg_TF_OUT_pass = FALSE;
    INVT_ID_ENUM id = INVT_ID_1;
    U08 cm = 0;
    static U16 type_bak = TF_TYPE_IDLE;
    static I16 temp_TWout2_bak;
    static BOL fg_init = FALSE;
    BOL fg_TF_WIFI_pass = TRUE;

    U16 TF_IBH_time_dly     = val_TF_IBH_time%100;              /* 商检电热延时 */
    U16 TF_IBH_time_over    = val_TF_IBH_time/100;              /* 商检电热超时 */
    U16 TF_heat_time_dly    = val_TF_heat_time%100;             /* 商检制热延时 */
    U16 TF_heat_time_over   = val_TF_heat_time/100;             /* 商检制热超时 */
    U16 TF_cool_time_dly    = val_TF_cool_time%100;             /* 商检制冷延时 */
    U16 TF_cool_time_over   = val_TF_cool_time/100;             /* 商检制冷超时 */

    /* 未启动商检时的处理 */
    if (!TF_EN
    ||  !fg_init
    )
    {
        fg_init = TRUE;
        
        TF_in_step = 0;
        TF_out_step = 0;
        fg_TF_IN_pass = FALSE;
        fg_TF_OUT_pass = FALSE;
        TF_err_id = 0;
        TF_step = TF_STEP_IDLE;
        temp_TWout2_bak = pv_cell_TWout2(pcb_addr);
        TF_cnt_wifi_dly = 0;
    }

    /* 切换商检类型时，执行一次手动复位 */
    if (type_bak != test_fac.type)
    {
        type_bak = test_fac.type;

        key_reset = TRUE;
    }
    
    /* 其他故障 */
    if (get_output(MAX_PCB, M_DO_ERR))
    {
        SETBIT(TF_err_id, TF_ERR_OTHER);
    }
    else
    {
        CLRBIT(TF_err_id, TF_ERR_OTHER);
    }

    /* 内机静态检测 */
    if (TF_HAS_INNER)
    {
        /* 显示器通讯故障 */
        if (SH_hmi_commu_succ > 0)
        {
            TF_cnt_hmi = 0;
            if (SH_hmi_commu_succ > 10) CLRBIT(TF_err_id, TF_ERR_HMI);
        }
        else if (TF_cnt_hmi > 10)
        {
            SETBIT(TF_err_id, TF_ERR_HMI);
        }

        /* 拨码开关 */
        if (gIO.jmp[0] != test_fac.sw)
        {
            SETBIT(TF_err_id, TF_ERR_SW);
        }
        else
        {
            CLRBIT(TF_err_id, TF_ERR_SW);
        }

        /* 拨盘地址 */
        if (gIO.SwPlate != test_fac.addr)
        {
            SETBIT(TF_err_id, TF_ERR_ADDR);
        }
        else
        {
            CLRBIT(TF_err_id, TF_ERR_ADDR);
        }

        /* 线控器WIFI */
        if (TF_EN_WIFI)
        {
            fg_TF_WIFI_pass = FALSE;
            switch (TF_wifi_sta)
            {
                case TEST_FAC_WIFI_STA_TESTING:
                    break;
                case TEST_FAC_WIFI_STA_NO_MODULE:
                    SETBIT(TF_err_id, TF_ERR_WIFI_NO_MODULE);
                    TF_cnt_wifi_dly = 0;
                    break;
                case TEST_FAC_WIFI_STA_SUCC:
                    fg_TF_WIFI_pass = TRUE;
                    TF_cnt_wifi_dly = 0;
                    break;
                case TEST_FAC_WIFI_STA_FAIL:
                    SETBIT(TF_err_id, TF_ERR_WIFI_FAIL);
                    TF_cnt_wifi_dly = 0;
                    break;
            }

            if (TF_cnt_wifi_dly > 80)		/* 测试超时：如线控器无WiFi商检功能等情况 */
            {
                SETBIT(TF_err_id, TF_ERR_WIFI_OVER);
            }
        }
        else
        {
            CLRBIT(TF_err_id, TF_ERR_WIFI_NO_MODULE);
            CLRBIT(TF_err_id, TF_ERR_WIFI_FAIL);
            CLRBIT(TF_err_id, TF_ERR_WIFI_OVER);
        }
    }

    /* 内机商检 */
    if (TF_START_INNER)
    {
        switch (TF_in_step)
        {
            case 0:
                if (log_sys_have_cell_pump_run(SUB_SYS_TYPE_MAX))
                {
                    TF_in_step++;
                    TF_cnt_in_step = 0;
                }
                break;
                
            case 1:
                if (IBH1_IS_USE || IBH2_IS_USE)
                {
                    /* 辅助电热 */
                    if (pv_cell_TWout2(pcb_addr)>pv_cell_TWout1(pcb_addr)
                    &&  pv_cell_TWout2(pcb_addr)-temp_TWout2_bak >= 10
                    &&  TF_cnt_in_step>=TF_IBH_time_dly
                    )
                    {
                        TF_in_step = 100;
                    }
                    else if (TF_cnt_in_step >= TF_IBH_time_dly+TF_IBH_time_over)
                    {
                        if (pv_cell_TWout2(pcb_addr)<=pv_cell_TWout1(pcb_addr)
                        ||  pv_cell_TWout2(pcb_addr)-temp_TWout2_bak < 10
                        )
                        {
                            SETBIT(TF_err_id, TF_ERR_IBH);
                            TF_in_step = 101;
                        }
                        else
                        {
                            TF_in_step = 100;
                        }
                    }
                }
                else
                {
                    TF_in_step = 100;
                }
                break;
                
            case 100:       /* 成功 */
                /* 要等待其他并行条件 */
                if (fg_TF_WIFI_pass)
                {
                    fg_TF_IN_pass = TRUE;
                }
                break;
            case 101:       /* 失败 */
                fg_TF_IN_pass = FALSE;
                break;
                
            default:
                break;
        }

        /* 继电器输出 */
        if (TF_EN_DO)
        {
            /* 2022-9-23 赖工与TCL沟通确认不做 */
#if 0
            extern const U08 DO_SEQUENCE[MAX_DO_NUM];
            static U16 DO_out[MAX_DO_W];
            static U08 s_cntSw = 0;

            /* 轮询控制 */
            if (TF_cnt_do_dly >= 5)
            {
                TF_cnt_do_dly = 0;

                ARRCLR(DO_out);
                
                if (s_cntSw < MAX_DO_NUM)
                {
                    SETBIT_N(DO_out, DO_SEQUENCE[s_cntSw]);                       
                }
                if (s_cntSw == MAX_DO_NUM-1)  s_cntSw = 0;
                else                          s_cntSw++;
            }

            /* 独立控制 */
            if (s_get_output(MAX_COMP, S_DO_PUMPf)) SETBIT_N(DO_out, DO_01);
            else                                    CLRBIT_N(DO_out, DO_01);                   
            if (s_get_output(MAX_COMP, S_DO_IBH1))  SETBIT_N(DO_out, DO_05);
            else                                    CLRBIT_N(DO_out, DO_05);
            if (s_get_output(MAX_COMP, S_DO_IBH2))  SETBIT_N(DO_out, DO_06);
            else                                    CLRBIT_N(DO_out, DO_06);

            MEMCPY(gIO.output, DO_out, sizeof(DO_out));
#endif
        }

        /* 开关量检测 */
        if (TF_EN_HDI)
        {
            /* 2022-9-23 赖工与TCL沟通确认不做 */
        }
    }

    /* 外机商检 */
    if (TF_START_OUTER)
    {
        switch (TF_out_step)
        {
            /* 制热 */
            case 0:
                if (TF_cnt_open_cm >= 1)
                {
                    TF_cnt_open_cm = 0;
                    log_sys_enegry_add(MODE_HEAT, SUB_SYS_TYPE_MAX);
                }

                TF_comp_freq = val_TF_heat_freq;
                TF_fan_freq = val_TF_heat_fan;
                TF_eev_step = val_TF_heat_step;

//                if (fun_pv_in(invt_info_freq_now(id), TF_comp_freq-50, TF_comp_freq+50))
                if (invt_info_freq_now(id) >= val_invt_start_freq-50)
                {
                    TF_out_step++;
                    TF_cnt_out_step = 0;
                }
                break;

            case 1:
                if (pv_cell_gas_out(cm) > pv_cell_gas_in(cm)
                &&  pv_cell_gas_out(cm) > pv_cell_wing(cm)
                &&  TF_cnt_out_step >= TF_heat_time_dly
                )
                {
                    TF_out_step = 10;
                    TF_cnt_out_step = 0;
                }
                else if (TF_cnt_out_step >= TF_heat_time_dly+TF_heat_time_over)
                {
                    if (pv_cell_gas_out(cm) <= pv_cell_gas_in(cm))
                    {
                        SETBIT(TF_err_id, TF_ERR_E8);
                        TF_out_step = 101;
                    }
                    else if (pv_cell_gas_out(cm) <= pv_cell_wing(cm))
                    {
                        SETBIT(TF_err_id, TF_ERR_E3);
                        TF_out_step = 101;
                    }
                    else
                    {
                        TF_out_step = 10;
                        TF_cnt_out_step = 0;
                    }
                }
                break;

            /* 转换 */
            case 10:
                TF_comp_freq = 40*100;
                
                if (fun_pv_in(invt_info_freq_now(id), TF_comp_freq-50, TF_comp_freq+50))
                {
                    cmd_comp_run(pcb_addr, cm, MODE_COOL);
                    TF_fan_freq = val_TF_cool_fan;
                    TF_eev_step = val_TF_cool_step;

                    TF_out_step++;
                    TF_cnt_out_step = 0;
                }
                break;

            /* 制冷 */
            case 11:
                if (TF_cnt_out_step >= 1)   /* 低频等1秒钟 */
                {
                    TF_comp_freq = val_TF_cool_freq;

                    if (fun_pv_in(invt_info_freq_now(id), TF_comp_freq-50, TF_comp_freq+50))
                    {
                        TF_out_step++;
                        TF_cnt_out_step = 0;
                    }
                }
                break;

            case 12:
                if (pv_cell_gas_in(cm) < pv_cell_wing(cm)
                &&  TF_cnt_out_step >= TF_cool_time_dly
                )
                {
                    TF_out_step = 100;
                    TF_cnt_out_step = 0;
                }
                else if (TF_cnt_out_step >= TF_cool_time_dly+TF_cool_time_over)
                {
                    SETBIT(TF_err_id, TF_ERR_E3);
                    TF_out_step = 101;
                }
                break;

             case 100:      /* 成功 */
                fg_TF_OUT_pass = TRUE;
                break;

             case 101:      /* 失败 */
                fg_TF_OUT_pass = FALSE;
                break;

            default:
                break;
        }
    }


    /* 商检状态 */
    switch (test_fac.type)
    {
        default:
        case TF_TYPE_IDLE:
            TF_step = TF_STEP_IDLE;
            break;
        case TF_TYPE_IN:
            if (fg_TF_IN_pass
            &&  TF_err_id==0
            )
            {
                TF_step = TF_STEP_PASS;
            }
            else if (TF_EN)
            {
                TF_step = TF_STEP_TESTING;
            }
            else
            {
                TF_step = TF_STEP_READY;
            }
            
            break;
        case TF_TYPE_OUT:
            if (fg_TF_OUT_pass
            &&  TF_err_id==0
            )
            {
                TF_step = TF_STEP_PASS;
            }
            else if (TF_EN)
            {
                TF_step = TF_STEP_TESTING;
            }
            else
            {
                TF_step = TF_STEP_READY;
            }

            break;
        case TF_TYPE_ALL:
            if (fg_TF_IN_pass
            &&  fg_TF_OUT_pass
            &&  TF_err_id==0
            )
            {
                TF_step = TF_STEP_PASS;
            }
            else if (TF_EN)
            {
                TF_step = TF_STEP_TESTING;
            }
            else
            {
                TF_step = TF_STEP_READY;
            }
            break;
    }
}

/*******************************************************************************
 函 数: TF_hmi()
 功 能: 处理商检屏
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TF_hmi(void)
{
    TEST_FAC_STRU * TF = &test_fac;
    extern const U16 Msg_mac_model[];
    U08 cm = 0;

    /* 故障 */
    if (TF_err_id != 0)
    {
        TF->err = TRUE;
    }
    else
    {
        TF->err = FALSE;
    }

    /* 版本号 */
    {
        /* 内机 */
        TF_STRCPY(APPFUNCODEVER, TF->str_ver_inner);

        /* 外机 */
        TF_STRCPY((char*)odu_mac_ver[pcb_addr], TF->str_ver_outer);
    }

    /* 机型 */
    MEMCPY(TF->str_mac, english_uncompress_text(Msg_mac_model[sw_mac_model]), sizeof(TF->str_mac));

    /* 商检状态 */
    switch (TF_step)
    {
        default:
        case TF_STEP_IDLE:
            TF_STRCPY("Idle!", TF->str_sta);
            break;
        case TF_STEP_READY:
            TF_STRCPY("Ready!", TF->str_sta);
            break;
        case TF_STEP_TESTING:
            TF_STRCPY("Testing!", TF->str_sta);
            break;
        case TF_STEP_PASS:
            TF_STRCPY("Pass!", TF->str_sta);/* “Pass”字段不能随意更改，商检屏用于识别成功 */
            break;
    }

    /* 商检故障 */
    {
        if (GETBIT(TF_err_id, TF_ERR_WIFI_NO_MODULE))
        {
            TF_STRCPY("ERR WIFI NO MODULE", TF->str_err);
        }
        else if (GETBIT(TF_err_id, TF_ERR_WIFI_FAIL))
        {
            TF_STRCPY("ERR WIFI ERROR", TF->str_err);
        }
        else if (GETBIT(TF_err_id, TF_ERR_WIFI_OVER))
        {
            TF_STRCPY("ERR WIFI OVER", TF->str_err);
        }
        else if (GETBIT(TF_err_id, TF_ERR_IBH))
        {
            TF_STRCPY("ERR IBH", TF->str_err);
        }
        else if (GETBIT(TF_err_id, TF_ERR_E3))
        {
            TF_STRCPY("ERR E3", TF->str_err);
        }
        else if (GETBIT(TF_err_id, TF_ERR_E8))
        {
            TF_STRCPY("ERR E8", TF->str_err);
        }
        else if (GETBIT(TF_err_id, TF_ERR_HMI))
        {
            TF_STRCPY("ERR HMI", TF->str_err);
        }
        else if (GETBIT(TF_err_id, TF_ERR_SW))
        {
            TF_STRCPY("ERR SW", TF->str_err);
        }
        else if (GETBIT(TF_err_id, TF_ERR_ADDR))
        {
            TF_STRCPY("ERR ADDR", TF->str_err);
        }
        else if (GETBIT(TF_err_id, TF_ERR_OTHER))   /* 注：按最低级别故障处理 */
        {
            U16 mod_id, sys_err_id, mod_err_id;
            TYBC_ERR_CUR err;                                 /* 故障信息 */
            U08 str1[32] = "ERR OTHER ";
            U08 str2[32];

            mod_id = sys_err_id = mod_err_id = 0;
            MEMCLR(&err, sizeof(err));
            MEMCLR(&str2, sizeof(str2));

            TYBC_GET_ERR_CUR(&err, &sys_err_id, &mod_id, &mod_err_id);
            fun_U16_to_str(err.err_id, str2);

            Fun_StrCat(str1, str2);
        
            TF_STRCPY((char*)str1, TF->str_err);
        }
        else
        {
            TF_STRCPY("...", TF->str_err);
        }
    }
    

    /* 资源点 */
    {
        U08 idx = 0;

        ARRCLR(TF->src);

        /* 内机资源 */
        if (TF_HAS_INNER)
        {
            /* 水泵 */
            if (idx < TF_SRC_NUM)
            {
                TF_STRCPY("PUMPf", TF->src[idx].str);
                TF->src[idx].val = pv_cell_PUMPf_set(pcb_addr);
                TF->src[idx].attrib = SH_sign_local2sh(msg_uint_p_cell[4].attr);
                idx++;
            }

            /* IBH1 */
            if (idx < TF_SRC_NUM)
            {
                if (IBH1_IS_USE)
                {
                    TF_STRCPY("IBH1", TF->src[idx].str);
                    TF->src[idx].val = s_get_output(MAX_COMP, S_DO_IBH1);
                    TF->src[idx].attrib = SH_sign_local2sh(D_BIT);
                    idx++;
                }
            }

            /* IBH2 */
            if (idx < TF_SRC_NUM)
            {
                if (IBH2_IS_USE)
                {
                    TF_STRCPY("IBH2", TF->src[idx].str);
                    TF->src[idx].val = s_get_output(MAX_COMP, S_DO_IBH2);
                    TF->src[idx].attrib = SH_sign_local2sh(D_BIT);
                    idx++;
                }
            }

            /* TWout1 */
            if (idx < TF_SRC_NUM)
            {
                TF_STRCPY("TWout1", TF->src[idx].str);
                TF->src[idx].val = pv_cell_TWout1(pcb_addr);
                TF->src[idx].attrib = SH_sign_local2sh(msg_uint_p_cell[1].attr);
                idx++;
            }

            /* TWout2 */
            if (idx < TF_SRC_NUM)
            {
                TF_STRCPY("TWout2", TF->src[idx].str);
                TF->src[idx].val = pv_cell_TWout2(pcb_addr);
                TF->src[idx].attrib = SH_sign_local2sh(msg_uint_p_cell[2].attr);
                idx++;
            }
        }

        /* 外机资源 */
        if (TF_HAS_OUTER)
        {
            /* 压机-目标 */
            if (idx < TF_SRC_NUM)
            {
                TF_STRCPY("COMPf_S", TF->src[idx].str);
                TF->src[idx].val = pv_cell_invt_set(cm);
                TF->src[idx].attrib = SH_sign_local2sh(msg_uint_p_comp[19].attr);
                idx++;
            }
            
            /* 压机-当前 */
            if (idx < TF_SRC_NUM)
            {
                TF_STRCPY("COMPf", TF->src[idx].str);
                TF->src[idx].val = pv_cell_invt_cur(cm);
                TF->src[idx].attrib = SH_sign_local2sh(msg_uint_p_comp[20].attr);
                idx++;
            }

            /* 变频风机1-目标 */
            if (idx < TF_SRC_NUM)
            {
                if (SensorCanUse(pv_cell_fan1_set(cm)))
                {
                    TF_STRCPY("FANf1_S", TF->src[idx].str);
                    TF->src[idx].val = pv_cell_fan1_set(cm);
                    TF->src[idx].attrib = SH_sign_local2sh(msg_uint_p_comp[27].attr);
                    idx++;
                }
            }

            /* 变频风机1 */
            if (idx < TF_SRC_NUM)
            {
                if (SensorCanUse(pv_cell_fan1_now(cm)))
                {
                    TF_STRCPY("FANf1", TF->src[idx].str);
                    TF->src[idx].val = pv_cell_fan1_now(cm);
                    TF->src[idx].attrib = SH_sign_local2sh(msg_uint_p_comp[28].attr);
                    idx++;
                }
            }

            /* 变频风机2 */
            if (idx < TF_SRC_NUM)
            {
                if (SensorCanUse(pv_cell_fan2_now(cm)))
                {
                    TF_STRCPY("FANf2", TF->src[idx].str);
                    TF->src[idx].val = pv_cell_fan2_now(cm);
                    TF->src[idx].attrib = SH_sign_local2sh(msg_uint_p_comp[30].attr);
                    idx++;
                }
            }

            /* 主阀 */
            if (idx < TF_SRC_NUM)
            {
                TF_STRCPY("EEVm", TF->src[idx].str);
                TF->src[idx].val = pv_cell_step_m(cm);
                TF->src[idx].attrib = SH_sign_local2sh(msg_uint_p_comp[1].attr);
                idx++;
            }

            /* 辅阀 */
            if (idx < TF_SRC_NUM)
            {
                if (SensorCanUse(pv_cell_step_s(cm)))
                {
                    TF_STRCPY("EEVs", TF->src[idx].str);
                    TF->src[idx].val = pv_cell_step_s(cm);
                    TF->src[idx].attrib = SH_sign_local2sh(msg_uint_p_comp[2].attr);
                    idx++;
                }
            }

            /* TS：吸气 */
            if (idx < TF_SRC_NUM)
            {
                TF_STRCPY("TS", TF->src[idx].str);
                TF->src[idx].val = pv_cell_gas_in(cm);
                TF->src[idx].attrib = SH_sign_local2sh(msg_uint_p_comp[5].attr);
                idx++;
            }

            /* TD：排气 */
            if (idx < TF_SRC_NUM)
            {
                TF_STRCPY("TD", TF->src[idx].str);
                TF->src[idx].val = pv_cell_gas_out(cm);
                TF->src[idx].attrib = SH_sign_local2sh(msg_uint_p_comp[4].attr);
                idx++;
            }

            /* TF：翅片 */
            if (idx < TF_SRC_NUM)
            {
                TF_STRCPY("TF", TF->src[idx].str);
                TF->src[idx].val = pv_cell_wing(cm);
                TF->src[idx].attrib = SH_sign_local2sh(msg_uint_p_comp[3].attr);
                idx++;
            }

            /* RV */
            if (idx < TF_SRC_NUM)
            {
                TF_STRCPY("RV", TF->src[idx].str);
                TF->src[idx].val = s_get_output(0, SW_S1);
                TF->src[idx].attrib = SH_sign_local2sh(D_BIT);
                idx++;
            }
        }
    }
}


/*******************************************************************************
 函 数: TF_task()
 功 能: 处理商检
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void TF_task(void)
{
    if (!TF_IS_IDLE)
    {
        TF_Time();
        TF_logic();
        TF_hmi();
    }
}


