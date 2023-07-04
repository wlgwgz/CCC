/********************************************************************************************************
*
* File                : main.c
* Hardware Environment: 
* Build Environment   : ST Visual Develop 4.1.6
* Version             : V1.0
*                              
*
*********************************************************************************************************/
#define MS_GLOBAL
#include "includes.h"

/************************************************
    函数：获取故障号
    返回值：故障号
*************************************************/
U08 menu_error_find_next(U08 opt, U08 x, U08 err_id)
{
    U08 i, j;
//  U08 x_new, y_new;
    CELL_DATA  *p;

    if (x >= MAX_PCB+1)
    {
        x = 0;
    }
    
    p = &cell[x];
    switch (opt)
    {
    case FIND_ERR_MIN:
        for (i=0; i<F_NO_ERR; i++)
        {               
            if (GET_BIT(p->cell_error[i / ERR_BIT_NUM], i % ERR_BIT_NUM)) return i;
        }
        break;
    case FIND_ERR_MAX :
        for (i = F_NO_ERR; i > 0; i--)
        {
            if (GET_BIT(p->cell_error[(i-1) / ERR_BIT_NUM], (i-1) % ERR_BIT_NUM)) return i-1;
        }
        break;
    case FIND_ERR_NEXT:
        for (i=err_id+1; i<F_NO_ERR; i++)
        {
            if (GET_BIT(p->cell_error[i / ERR_BIT_NUM], i % ERR_BIT_NUM)) return i;
        }            
        for (i=0; i<=err_id; i++)
        {
            if (GET_BIT(p->cell_error[i / ERR_BIT_NUM], i % ERR_BIT_NUM)) return i;
        }   
       break;
    case FIND_ERR_PREV:
        for (i=err_id; i>0; i--)
        {
            if (GET_BIT(p->cell_error[(i-1) / ERR_BIT_NUM], (i-1) % ERR_BIT_NUM)) return i-1;
        }
        for (i=F_NO_ERR; i>err_id; i--)
        {
            if (GET_BIT(p->cell_error[(i-1) / ERR_BIT_NUM], (i-1) % ERR_BIT_NUM)) return i-1;
        }
        break;

    case FIND_ERR_PCB_MIN :           /* 寻找最小故障板号 */
        for (i = 0; i < MAX_PCB_ALL; i++) 
        {
            for (j = 0; j < MAX_ERR; j++) 
            {
                if (cell[i].cell_error[j])  return i;
            }
        }
        break;
    case FIND_ERR_PCB_NEXT :    /* 寻找下一块故障板号 */
        for (i = x+1; i < MAX_PCB_ALL; i++) 
        {
            for (j = 0; j < MAX_ERR; j++) 
            {
                if (cell[i].cell_error[j])  return i;
            }
        }
        for (i = 0; i <= x; i++) 
        {
            for (j = 0; j < MAX_ERR; j++) 
            {
                if (cell[i].cell_error[j])  return i;
            }
        }
        break;
    case FIND_ERR_PCB_PREV:    /* 寻找上一块故障板号 */
        for (i = x; i > 0; i--)
        {
            for (j = 0; j < MAX_ERR; j++) 
            {
                if (cell[i-1].cell_error[j])  return i-1;
            }
        }
        for (i = MAX_PCB_ALL; i > x; i--)
        {
            for (j = 0; j < MAX_ERR; j++) 
            {
                if (cell[i-1].cell_error[j])  return i-1;
            }
        }
        break;
    
    case FIND_ERR_PCB_MAX:    /* 寻找最大故障板号 */
        for (i = MAX_PCB_ALL; i > 0; i--)
        {
            for (j = 0; j < MAX_ERR; j++) 
            {
                if (cell[i-1].cell_error[j])  return i-1;
            }
        }
        break;
    }
    
    return F_NO_ERR;
}

/*******************************************************************************
 函 数: main_init()
 功 能: main文件内全局变量初始化
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void main_init(void)
{
    U08 i, j;

    for (i=0; i<8; i++)
    {
        cmd_normal[i] = 0;
        cmd_system[i] = 0;
    }

//  remeber_on = FALSE;
    fg_error = FALSE;
    debug_mode = FALSE;      
    Link_remeber_on =FALSE;

    /* 预热标志初始化：从机初始化为已完成，防止曲轴闪开 */
    if (pcb_type == TYPE_MASTER)
    {
        fg_warm_complete = FALSE;
    }
    else
    {
        fg_warm_complete = TRUE;
    }
        
    /* 初始化命令、状态等 */
    for (i=0; i<MAX_PCB + 1; i++)
    {
        for (j=0; j<MAX_CMD; j++)        /* 命令 */
        {
            cell[i].cell_cmd[j] = 0;
        }
        for (j=0; j<MAX_RELAY; j++)     /* 输出 */
        {
            cell[i].cell_relay[j] = 0;
        }
        for (j=0; j<MAX_INPUT; j++)     /* 输入 */
        {
            cell[i].cell_input[j] = 0;
        }
        for (j=0; j<MAX_STATUS; j++)    /* 状态 */
        {
            cell[i].cell_status[j] = 0;
        }
//      for (j=0; j<MAX_RUNTIME; j++)    /* 累计运行时间 */
//      {
//          cell[i].cell_runtime[j] = 0;
//      }
        for (j=0; j<MAX_ERR; j++)        /* 故障 */
        {
            cell[i].cell_error[j] = 0;
        }
        for (j=0; j<MAX_PV; j++)        /* 模拟量 */
        {
            cell[i].cell_pv[j] = SENSOR_NOEXIST;
        }
    }
     
    pv_sys_environment_tmp = pv_sys_environment_update = SENSOR_NOEXIST;
}

/* N0004 */
/*******************************************************************************
 函 数: get_phase_use()
 功 能: 三相电使用否
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
BOOL get_phase_use(void)
{
    #define SW_PHASE_LACK    (2)

    BOOL use_phase = FALSE;

    if (1)  
    {
//        if (gIO.jmp[0] & 0x0001)        //SW1-1为三相电使用否
        if (GETBIT_N(gIO.jmp, SW_PHASE_LACK))             //SW2-1为三相电使用否
        {
            use_phase = TRUE;
        }
        else
        {
            use_phase = FALSE;
        }
    }

    return use_phase;
}



/*====================================================
        初始化变量
====================================================*/
void initial(void)
{
//    U08 i;
    
    main_init();        // 主调用初始化
#if 0 /* lxh */
    uart_init();        // 通信初始化
#endif
    defrost_init();
    lcd_init();         // 显示初始化
    on_off_init();      // 开关机初始化
    log_sys_init();     // 系统模块变量初始化 
    log_mod_init();     // 模块变量初始化 
    err_sys_init();     // 系统故障变量初始化   
    err_mod_init();     // 模块故障变量初始化
    hmi_routine_init();
//    ARRSET(gIO.ai_type, AI_05_45V);
//    gIO.ai_type[0]=gIO.ai_type[1]=gIO.ai_type[2]=gIO.ai_type[3]=AI_05_45V;    // 模拟量类型 //NOTE-CXW 是否需要？

    invt_init();        // 变频器通讯初始化
    pid_init();         // PID设置初始化
    
    ec_fan_init();      /* EC风机初始化 */
#ifdef COM_FCU_FBUS
    fcu_init();         /* 风机盘管初始化 */
#endif    
    
//    eev_isReady(0);

    para_check();       // 参数检查 

//    hardware        = MCU_GetHardware();    /* 获取硬件型号 *//* N0004 */
    use_phase_lack  = get_phase_use();        // 三相电检测使用否 /* N0004 */
    
    eev_init();         // 电膨状态初始化(在读取参数后、电机归零前调用)
    wf_init();

    PU_init();

    MacTypeMaster = val_mac_model;          /* 放“参数检查”之后：主机机型初始化为从机机型，即默认型号匹配 */
    
    /* 步进电机归零(要先读取参数才能计算电机最大步数，电机才能正确归零) */
//  for(i=0; i<EEV_ID_MAX; i++)
//  {
//        pIO = &gIO;
//   EEV_STEP_SV   pIO->io_step_sv[i] = 0;
//      if(i==EEV_ID_1) 
//        {
//       EEV_STEP_PV     pIO->io_step_pv[i] = val_motor_step_max;
//      }
//        else if(i==EEV_ID_2) 
//        {
//       EEV_STEP_PV     pIO->io_step_pv[i] = val_s_motor_step_max;
//        }
//  }
#ifdef USE_ODU_HL081B		/* HL081B */
   if (USE_PUNP_NTC)
   {
       ARRSET(Hdw_Temp_NTC_Type, NTC_PUNP_NORMAL_10K);
   }
   else if (USE_TCL_NTC)
   {
       ARRSET(Hdw_Temp_NTC_Type, NTC_TCL_NORMAL_10K);
   }
   else if (USE_CHANGHONG_NTC)
   {
       ARRSET(Hdw_Temp_NTC_Type, NTC_PUNP_NORMAL_10K);
   }
   else if (USE_PAIWO_NTC)
   {
       ARRSET(Hdw_Temp_NTC_Type, NTC_PAIWO_NORMAL_10K);
   }
   else if (USE_NIUENTAI_NTC)
   {
       ARRSET(Hdw_Temp_NTC_Type, NTC_NIUENTAI_NORMAL_5K);
   }
#else						/* HL081A */
   if (USE_PUNP_NTC)
   {
       ARRSET(Hdw_Temp_NTC_Type, NTC_PUNP_NORMAL_10K);
   }
   else if (USE_TCL_NTC)
   {
       ARRSET(Hdw_Temp_NTC_Type, NTC_TCL_NORMAL_10K);
   }
   else if (USE_CHANGHONG_NTC)
   {
       ARRSET(Hdw_Temp_NTC_Type, NTC_PUNP_NORMAL_10K);
   }
#endif
    
    if(pcb_addr==0) // 主板才初始化BMS口     
    {
#if 0 /* lxh */
        Task_ModbusRS485(); // BMS口初始化  
#endif 
    }   
}

/***********************************************
函数：his_err_update
功能：历史故障更新与保存
参数：外机序号
返回值：无
***********************************************/
static void his_err_update(void)
{
    U08 i, j, k, m;
    U08 err_num;
    U16 err_now = 0, err_bak = 0;
    BOOL fg_new_err = FALSE;        /* 有新的故障 */
    BOOL fg_upd_err = FALSE;        /* 更新故障 */
    U08 no_err_hmi=0;
    static U08 pcb_num;
    
    if (pcb_type == TYPE_SLAVER || power_time < POWER_ON_INTO_WORK) /* 上电10秒后才记录历史故障，防止时钟还未读取 */
    {
        return;
    }
    if (++pcb_num > MAX_PCB)        pcb_num = 0;

    for (i=0; i<MAX_HMI; i++)
    {
        if (GET_BIT(hmi_commu_error, i) != 0)
        {
            continue;
        }
        no_err_hmi = i;
    }
    
    for (i=0; i<MAX_ERR; i++)
    {
        for(j=0; j<ERR_BIT_NUM; j++)
        {
            err_now = cell[pcb_num].cell_error[i] & (0x0001 << j);
            err_bak = bak_err_out[pcb_num][i] & (0x0001 << j);
            
            if (err_now != err_bak && err_bak == 0)     /* 出现新的故障 */
            {
                INVT_ID_ENUM id;
                BOL fg_ready = TRUE;
                U08 sign = 0;

                err_num = i*ERR_BIT_NUM + j;

                /* 非系统故障时 */
                if (pcb_num != MAX_PCB)
                {
                    /* 防止附加号未准备好导致记录出错 */
                    for (id=INVT_ID_1; id<INVT_ID_MAX; id++)
                    {
                        if(err_num == err_invt_id_to_err_id(id, INVT_ERR_ERROR))
                        {
                            if (SensorCanUse(CELL_INVT_ERR(pcb_num, id)))   
                            {
                                sign = CELL_INVT_ERR(pcb_num, id);  // 附加号为变频器故障具体故障号
                            }
                            else
                            {
                                fg_ready = FALSE;
                            }
                        }
                    }
    
                    /* 变频水泵子故障 */
                    if (err_num == S_ERR_PUMPF)
                    {
                        if (SensorCanUse(pv_cell_PUMPf_err_code(pcb_num)))
                        {
                            sign = pv_cell_PUMPf_err_code(pcb_num);
                        }
                        else
                        {
                            fg_ready = FALSE;
                        }
                    }
                }
                
                if (fg_ready)
                {
                    for (k=MAX_HIS_ERROR-1; k>0; k--)
                    {
                        for (m=0; m<HIS_ERR_SIZE; m++)
                        {
                            his_err_tbl[k*HIS_ERR_SIZE+m] = his_err_tbl[(k-1)*HIS_ERR_SIZE+m];
                        }
                    }
                    
                    his_err_tbl[0] = err_num;
                    his_err_tbl[1] = pcb_num;
                    his_err_tbl[2] = sign;
                    his_err_tbl[3] = clock[no_err_hmi].month;
                    his_err_tbl[4] = clock[no_err_hmi].day;
                    his_err_tbl[5] = clock[no_err_hmi].hour;
                    his_err_tbl[6] = clock[no_err_hmi].min;
                    his_err_tbl[7] = clock[no_err_hmi].sec;
                    
                    fg_new_err = TRUE;
                    fg_upd_err = TRUE;
                }
            }
            else
            {
                fg_upd_err = TRUE;
            }

            if (fg_upd_err)
            {
                 GETBIT(cell[pcb_num].cell_error[i], j) ? SETBIT(bak_err_out[pcb_num][i], j)
                                                        : CLRBIT(bak_err_out[pcb_num][i], j);
            }
        }
    }
    
    if (fg_new_err) 
    { 
        DataAccess_HistoryErr(DATA_WRITE);      // 写入历史故障
    }
}

/*******************************************************************************
 函 数: read_address()
 功 能: 读地址
 参 数: 空
 返 回: 后板地址    
 其 它: 空
*******************************************************************************/
U16 read_address(void)
{
    return (gIO.SwPlate);
}
#if 0

/*******************************************************************************
 函 数: read_comp_num()
 功 能: 读压机数量
 参 数: 空
 返 回: 压机数量    
 其 它: 空
*******************************************************************************/
U16 read_comp_num(void)
{
//    U08 i;
    U16 comp_num = 0;

    
    if(GETBIT(gIO.jmp[0], 4))  
    {
        comp_num=1;
    }
    else
    {
        comp_num=2;
    }   
    return comp_num;
}
#endif

/*******************************************************************************
 函 数: read_IBH_config()
 功 能: 读IBH配置
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
U16 read_IBH_config(void)
{
    #define SW_IBH_BASE    (0)
    
    U16 IBH_gear = 0;

    if (GETBIT_N(gIO.jmp, SW_IBH_BASE+0))
    {
        SETBIT(IBH_gear, 1);/* 有IBH2 */
    }

    if (GETBIT_N(gIO.jmp, SW_IBH_BASE+1))
    {
        SETBIT(IBH_gear, 0);/* 有IBH1 */
    }
    
    return IBH_gear;
}

/*******************************************************************************
 函 数: read_mac_model()
 功 能: 读机型
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
U16 read_mac_model(void)
{
    #define SW_MAC_BASE    (MAX_SW_BIT-SW_MAC_NUM)  

    U16 mac_model = 0;
    I08 i = 0;

    for (i=0; i<SW_MAC_NUM; i++)
    {
        if (GETBIT_N(gIO.jmp, SW_MAC_BASE+i))
        {
            SETBIT(mac_model, SW_MAC_NUM-1-i);
        }
    }

    return mac_model;
}

/*********************************************************************************** 
* 函 数 名: read_io_info
* 功    能: 读取io口信息
* 入口参数: 无
* 返 回 值: 无
* 备    注: 每个循环体内都必须包含该函数
***********************************************************************************/
void Read_IO_Info(void)
{
    wdg_clear();
    self_comp_num = val_comp_num;       // 获得压机数
    io_temp_read();                     // 读温度与电流等
    if (val_freq_fan_set == FREQ_FAN_SET_PWM) io_speed_deal();/* N0004 */
    PUMPF_pwm_out_deal();
    his_err_update();           //   

#ifdef USE_WF_TEST
    wf_test();
#endif
}

/*******************************************************************************
 函 数: log_invt_except_info_rec()
 功 能: 变频异常信息记录
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void log_invt_except_info_rec(void)
{
    U16 lmt_now = invt_info_freq_limit_id(INVT_ID_1);
    U16 lmt_x00 = lmt_now/100;
    U16 lmt_00x = lmt_now%10;
    static U16 lmt_bak_x00;
    static U16 lmt_bak_00x;
    
    U16 err_now = invt_info_error_id(INVT_ID_1);
    static U16 err_bak;
    
    U16 info_occur = 0;
    InvtEXCEPT *pInfo = info_invt_except;

    /* 限频信息百位 */
    if (lmt_bak_x00 != lmt_x00)
    {
        lmt_bak_x00 = lmt_x00;
        if (lmt_x00 == 2)       SETBIT(info_occur, INFO_200);
    }
    /* 限频信息个位 */
    if (lmt_bak_00x != lmt_00x)
    {
        lmt_bak_00x = lmt_00x;
             if (lmt_00x == 4)  SETBIT(info_occur, INFO_004);
        else if (lmt_00x == 7)  SETBIT(info_occur, INFO_007);
        else if (lmt_00x == 8)  SETBIT(info_occur, INFO_008);
    }
    /* 变频故障 */
    if (err_bak != err_now)
    {
        err_bak = err_now;
             if (err_now ==  2)  SETBIT(info_occur, INFO_E02);
        else if (err_now ==  3)  SETBIT(info_occur, INFO_E03);
        else if (err_now ==  4)  SETBIT(info_occur, INFO_E04);
        else if (err_now == 12)  SETBIT(info_occur, INFO_E12);
        else if (err_now == 19)  SETBIT(info_occur, INFO_E19);
        else if (err_now == 20)  SETBIT(info_occur, INFO_E20);
        else if (err_now == 31)  SETBIT(info_occur, INFO_E31);
        else if (err_now !=  0)             
        {
            /* 其余故障使用随机覆盖更新 */
            U08 idx = 0;
            for (idx=INFO_EX1; idx<=INFO_EX5; idx++)
            {
                if (pInfo[idx].info == 0) break;
            }
            if (idx > INFO_EX5) idx = rand()%(INFO_EX5-INFO_EX1+1)+INFO_EX1;
            SETBIT(info_occur, idx);
        }
    }
    
    /* 断点信息记录 */
    if (info_occur > 0)
    {
        U08 x = 0;
        INVT_ID_ENUM id = INVT_ID_1;
        for (x=0; x<INFO_MAX; x++)
        {
            if (GETBIT(info_occur, x))
            {
                if (fun_pv_in(x, INFO_EX1, INFO_EX5))   pInfo[x].info = err_now;
                else                                    _CNT(pInfo[x].info);
                MEMCPY(&(pInfo[x].time), &clock[hmi_t_num], sizeof(RTC));
                pInfo[x].state[0] = invt_info_freq_run(id)/10;      /* 运行转速 */
                pInfo[x].state[1] = invt_info_freq_now(id)/10;      /* 瞬时转速 */
                pInfo[x].state[2] = invt_info_voltage_out(id);      /* 输出电压 */
                pInfo[x].state[3] = invt_info_current_out(id);      /* 输出电流 */
                pInfo[x].state[4] = invt_info_volt_generatrix(id);  /* 母线电压 */
                pInfo[x].state[5] = pv_cell_gas_out(id);            /* 排气温度 */
                pInfo[x].state[6] = pv_cell_TA(pcb_addr);           /* 环境温度 */
                pInfo[x].state[7] = pv_sys_TWout2_ac;               /* 空调出水温度 */

                /* 写入E方 */
                DataAccess_InvtExceptInfo(DATA_WRITE, x);
            }
        }
        
    }
}

/*******************************************************************************
 函 数: mod_task()
 功 能: 模块功能执行
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void mod_task(void)
{       
    err_mod_time();     

    err_mod();  
    
    log_mod();

    log_invt_except_info_rec();
    
    eeprom_limit_time_write();

    if (F_TIME_BASIC_500MS_BACK)
    {
        para_check_invt_value();
    }
}
/*******************************************************************************
 函 数: sys_task()
 功 能: 系统任务执行
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void sys_task(void)
{
    static U08 pcb_order = 0xff;
//  U08 i;
    if (++pcb_order >= val_pcb_num)      /* 循环处理各个模块 */
    {
        pcb_order = 0;
    }
    err_sys(pcb_order);

                
    log_sys(pcb_order);         
            
    
}
/*******************************************************************************
 函 数: task()
 功 能: 任务
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void mod_debug(void)
{
    U16 i;
    gdebugIO.debugstart = 0x5a;
    gdebugIO.io_step_sv[EEV_ID_1] = GET_EXIO_ST_SV(0, EEV_ID_1);
    gdebugIO.io_step_sv[EEV_ID_2] = GET_EXIO_ST_SV(0, EEV_ID_2);
    gdebugIO.io_step_pv[EEV_ID_1] = GET_EXIO_ST_PV(0, EEV_ID_1);
    gdebugIO.io_step_pv[EEV_ID_2] = GET_EXIO_ST_PV(0, EEV_ID_2);
    gdebugIO.in_input.word = gIO.input[0];              // 内机开关量
    for (i=0; i < EXIO_DI_MAX; i++)                     // 外机开关量
    {
        if (GET_EXIO_DI(0, i))
        {
            SETBIT(gdebugIO.out_input.word, i);
        }
        else
        {
            CLRBIT(gdebugIO.out_input.word, i);
        }
    }
    gdebugIO.in_output[0].word = gIO.output[0];     // 内机输出
    gdebugIO.in_output[1].word = gIO.output[1];

    for (i=0; i<EXIO_DO_MAX; i++)                   // 外机输出
    {
        if (GET_EXIO_DO(0, i))
        {
            SETBIT(gdebugIO.out_output.word, i);
        }
        else
        {
            CLRBIT(gdebugIO.out_output.word, i);
        }
    }

    for(i=0;i<MAX_TEMP;i++)                      // 内机温度
    {
        gdebugIO.in_temp[i] = gIO.temp[i];
    }
    for(i=0;i<EXIO_TEMP_MAX;i++)                // 外机温度
    {
        gdebugIO.out_temp[i] = GET_EXIO_TMP(0, i);
    }
    
    gdebugIO.press[0] = pv_cell_lp(0); //低压压力
    gdebugIO.press[1] = pv_cell_hp(0);//高压压力
//    gdebugIO.press[2] = pv_cell_lp(0);//备用压力
  
    gdebugIO.ptot[0] = pv_cell_evap_temp(0); //低压压力对应蒸发温度
    gdebugIO.ptot[1] = pv_cell_cond_temp(0);//高压压力对应冷凝温度
//    gdebugIO.ptot[2] = pv_cell_cond_temp(0);//备用压力

    gdebugIO.freq_sv = pv_cell_invt_set(0);        // 频率设定值
    gdebugIO.freq_pv = pv_cell_invt_cur(0);        // 频率当前值
    gdebugIO.freq_temp = pv_cell_invt_tmp(0);        // 变频器当前温度
    gdebugIO.freq_volt = pv_cell_invt_volt(0);        // 变频器电压
    gdebugIO.freq_cur = pv_cell_invt_curr(0);        //变频器电流

    gdebugIO.fan_speed_sv[0] = pv_cell_fan_set(0);          // 风机转速设定值
    gdebugIO.fan_speed_pv[0] = pv_cell_fan_now(0);          // 风机转速当前值
    gdebugIO.fan_speed_sv[1] = pv_cell_fan_set(1);          // 风机转速设定值
    gdebugIO.fan_speed_pv[1] = pv_cell_fan_now(1);          // 风机转速当前值
    gdebugIO.pump_speed_sv = pv_cell_PUMPf_set(pcb_addr);           // 水泵转速设定值
    gdebugIO.pump_speed_pv = pv_cell_PUMPf_cur(pcb_addr);           // 水泵转速当前值流量

    gdebugIO.debugbuf[ 0] = 257;
    gdebugIO.debugbuf[ 1] = DebugPID[DEBUG_UK];
    gdebugIO.debugbuf[ 2] = DebugPID[DEBUG_UP];
    gdebugIO.debugbuf[ 3] = DebugPID[DEBUG_UI];
    gdebugIO.debugbuf[ 4] = DebugPID[DEBUG_UD];
    gdebugIO.debugbuf[ 5] = DebugPID[DEBUG_PID_E1];
    gdebugIO.debugbuf[ 6] = DebugPID[DEBUG_PID_EC1];
    gdebugIO.debugbuf[ 7] = DebugPID[DEBUG_PID_EC2];
    gdebugIO.debugbuf[ 8] = DebugPID[DEBUG_PID_1UI];
    gdebugIO.debugbuf[ 9] = DebugPID[DEBUG_PID_2UI];
    gdebugIO.debugbuf[10] = DebugPID[DEBUG_STEP];
    gdebugIO.debugbuf[11] = DebugPID[DEBUG_PID_SV];
    gdebugIO.debugbuf[12] = DebugPID[DEBUG_PID_PV];
    gdebugIO.debugbuf[13] = DebugPID[DEBUG_PID_USE];
    gdebugIO.debugbuf[14] = DebugPID[DEBUG_PID_MIDDL];
    gdebugIO.debugbuf[15] = val_m_pid_p;// 反作用比例带
    gdebugIO.debugbuf[16] = val_m_pid_i;// 积分时间
    gdebugIO.debugbuf[17] = val_m_pid_d;// 微分时间
    gdebugIO.debugbuf[18] = val_m_pid_period;// pid 动作周期 
    gdebugIO.debugbuf[19] = val_m_pid_calculate_period;// pid计算周期 
    gdebugIO.debugbuf[20] = val_m_pid_Kp;// 比例系数
    gdebugIO.debugbuf[21] = val_m_pid_flexi; // 积分系数
    gdebugIO.debugbuf[22] = pid_get_AveAcrossCnt(PID_ID_EEV_1); /* 穿过次数 */
    gdebugIO.debugbuf[23] = pid_get_avemod(PID_ID_EEV_1);   /* 加速计数值 */
    gdebugIO.debugbuf[24] = 555;
    gdebugIO.debugbuf[25] = DebugEEV_ID_2PID[DEBUG_UK];
    gdebugIO.debugbuf[26] = DebugEEV_ID_2PID[DEBUG_UP];
    gdebugIO.debugbuf[27] = DebugEEV_ID_2PID[DEBUG_UI];
    gdebugIO.debugbuf[28] = DebugEEV_ID_2PID[DEBUG_UD];
    gdebugIO.debugbuf[29] = DebugEEV_ID_2PID[DEBUG_PID_E1];
    gdebugIO.debugbuf[30] = DebugEEV_ID_2PID[DEBUG_PID_EC1];
    gdebugIO.debugbuf[31] = DebugEEV_ID_2PID[DEBUG_PID_EC2];
    gdebugIO.debugbuf[32] = DebugEEV_ID_2PID[DEBUG_PID_1UI];
    gdebugIO.debugbuf[33] = DebugEEV_ID_2PID[DEBUG_PID_2UI];
    gdebugIO.debugbuf[34] = DebugEEV_ID_2PID[DEBUG_STEP];
    gdebugIO.debugbuf[35] = DebugEEV_ID_2PID[DEBUG_PID_SV];
    gdebugIO.debugbuf[36] = DebugEEV_ID_2PID[DEBUG_PID_PV];
    gdebugIO.debugbuf[37] = DebugEEV_ID_2PID[DEBUG_PID_USE];
    gdebugIO.debugbuf[38] = DebugEEV_ID_2PID[DEBUG_PID_MIDDL];
    gdebugIO.debugbuf[39] = val_s_pid_p;// 比例带 
    gdebugIO.debugbuf[40] = val_s_pid_i;// 积分时间 
    gdebugIO.debugbuf[41] = val_s_pid_d;    // 微分时间 
    gdebugIO.debugbuf[42] = val_s_pid_period;   // pid动作周期 
    gdebugIO.debugbuf[43] = val_s_pid_calculate_period; // pid计算周期 
    gdebugIO.debugbuf[44] = val_s_pid_Kp;// 比例系数
    gdebugIO.debugbuf[45] = val_s_pid_flexi;// 积分系数
    gdebugIO.debugbuf[46] = GET_EXIO_ST_SV(0, EEV_ID_2);
    gdebugIO.debugbuf[47] = GET_EXIO_ST_PV(0, EEV_ID_2);
    gdebugIO.debugbuf[48] = 666;
    gdebugIO.debugbuf[49] = DebugINVT_ID_1PID[DEBUG_UK];
    gdebugIO.debugbuf[50] = DebugINVT_ID_1PID[DEBUG_UP];
    gdebugIO.debugbuf[51] = DebugINVT_ID_1PID[DEBUG_UI];
    gdebugIO.debugbuf[52] = DebugINVT_ID_1PID[DEBUG_UD];
    gdebugIO.debugbuf[53] = DebugINVT_ID_1PID[DEBUG_PID_E1];
    gdebugIO.debugbuf[54] = DebugINVT_ID_1PID[DEBUG_PID_EC1];
    gdebugIO.debugbuf[55] = DebugINVT_ID_1PID[DEBUG_PID_EC2];
    gdebugIO.debugbuf[56] = DebugINVT_ID_1PID[DEBUG_PID_1UI];
    gdebugIO.debugbuf[57] = DebugINVT_ID_1PID[DEBUG_PID_2UI];
    gdebugIO.debugbuf[58] = DebugINVT_ID_1PID[DEBUG_STEP];
    gdebugIO.debugbuf[59] = DebugINVT_ID_1PID[DEBUG_PID_SV];
    gdebugIO.debugbuf[60] = DebugINVT_ID_1PID[DEBUG_PID_PV];
    gdebugIO.debugbuf[61] = DebugINVT_ID_1PID[DEBUG_PID_USE];
    gdebugIO.debugbuf[62] = DebugINVT_ID_1PID[DEBUG_PID_MIDDL];
    gdebugIO.debugbuf[63] = val_f_pid_p;// 比例带 
    gdebugIO.debugbuf[64] = val_f_pid_i;// 积分时间 
    gdebugIO.debugbuf[65] = val_f_pid_d;    // 微分时间 
    gdebugIO.debugbuf[66] = val_f_pid_period;   // pid动作周期 
    gdebugIO.debugbuf[67] = val_f_pid_calculate_period; // pid计算周期 
    gdebugIO.debugbuf[68] = val_f_pid_Kp;// 比例系数
    gdebugIO.debugbuf[69] = val_f_pid_flexi;/* 积分系数 */
    gdebugIO.debugbuf[70] = pv_cell_invt_set(0);
    gdebugIO.debugbuf[71] = pv_cell_invt_cur(0);
    gdebugIO.debugbuf[72] = 777;
    gdebugIO.debugbuf[73] = 73;
    gdebugIO.debugbuf[74] = invt_ad[INVT_ID_1].ad_u;
    gdebugIO.debugbuf[75] = invt_ad[INVT_ID_1].ad_v;
    gdebugIO.debugbuf[76] = invt_curr_wave[INVT_ID_1];
    gdebugIO.debugbuf[77] = speed_percent[FREQ_FAN1];
    gdebugIO.debugbuf[78] = speed_percent[FREQ_FAN2];
    gdebugIO.debugbuf[79] = fg_have_fan_on; /* 134 */
//    gdebugIO.debugbuf[80] = power_comp;	/* 135 */
//    gdebugIO.debugbuf[81] = power_fan;    /* 136 */
//    gdebugIO.debugbuf[82] = power_pump;   /* 137 */
//    gdebugIO.debugbuf[83] = power_IBH1;   /* 138 */
//    gdebugIO.debugbuf[84] = power_IBH2;   /* 139 */
    gdebugIO.debugbuf[85] = fan_adjust_fg[0];		/* 140 */
    gdebugIO.debugbuf[86] = fan_adjust_tmp[0][FAN_ADJ_TMP_BASE];	/* 141 */
    gdebugIO.debugbuf[87] = fan_adjust_tmp[0][FAN_ADJ_TMP_COMP];	/* 142 */
    gdebugIO.debugbuf[88] = fan_adjust_tmp[0][FAN_ADJ_TMP_POWER];	/* 143 */
    gdebugIO.debugbuf[89] = fan_adjust_tmp[0][FAN_ADJ_TMP_EXH];	/* 144 */
    gdebugIO.debugbuf[90] = fan_adjust_tmp[0][FAN_ADJ_TMP_SILENT];	/* 145 */
    gdebugIO.debugbuf[91] = PUMPf_pwm_out;	/* 146 */
    gdebugIO.debugbuf[92] = PUMPf_pwm_in;	/* 147 */
    gdebugIO.debugbuf[93] = 0;
    for (i=0; i<FORCE_MOTOR_MAX; i++)
    {
        if (fg_force_motor[EEV_ID_1][i])
        {
            SETBIT(gdebugIO.debugbuf[93], i);				/* 148 */
        }
    }
    gdebugIO.debugbuf[94] = 0;
    for (i=0; i<FORBID_SUB_MOTOR_MAX; i++)
    {
        if (fg_forbid_motor_sub[EEV_ID_1][i])
        {
            SETBIT(gdebugIO.debugbuf[94], i);				/* 149 */
        }
    }
    gdebugIO.debugbuf[95] = 0;
    for (i=0; i<FORBID_ADD_MOTOR_MAX; i++)
    {
        if (fg_forbid_motor_add[EEV_ID_1][i])
        {
            SETBIT(gdebugIO.debugbuf[95], i);				/* 150 */
        }
    }
    gdebugIO.debugbuf[ 96] = eev_adjust_tmp[0][EEV_ADJ_TMP_BASE];		/* 151 */
    gdebugIO.debugbuf[ 97] = eev_adjust_tmp[0][EEV_ADJ_TMP_COMP];		/* 152 */
    gdebugIO.debugbuf[ 98] = eev_adjust_tmp[0][EEV_ADJ_TMP_EXH];		/* 153 */
    gdebugIO.debugbuf[ 99] = eev_adjust_tmp[0][EEV_ADJ_TMP_GB];        /* 154 */
    gdebugIO.debugbuf[100] = eev_adjust_tmp[0][EEV_ADJ_TMP_RANGE];		/* 155 */
    gdebugIO.debugbuf[101] = eev_adjust_tmp[0][EEV_ADJ_TMP_USAGE];		/* 156 */
    gdebugIO.debugbuf[102] = 0;
    for (i=0; i<FORCE_MOTOR_MAX; i++)
    {
        if (fg_force_motor[EEV_ID_2][i])
        {
            SETBIT(gdebugIO.debugbuf[102], i);				/* 157 */
        }
    }
    gdebugIO.debugbuf[103] = 0;
    for (i=0; i<FORBID_EVI_MAX; i++)
    {
        if (fg_evi_forbid[EEV_ID_2][i])
        {
            SETBIT(gdebugIO.debugbuf[103], i);				/* 158 */
        }
    }
    gdebugIO.debugbuf[104] = 0;
    for (i=0; i<EVI_OPEN_MAX; i++)
    {
        if (fg_evi_allow[EEV_ID_2][i])
        {
            SETBIT(gdebugIO.debugbuf[104], i);				/* 159 */
        }
    }
    gdebugIO.debugbuf[105] = evi_adjust_tmp[0][EVI_ADJ_TMP_BASE];		/* 160 */
    gdebugIO.debugbuf[106] = evi_adjust_tmp[0][EVI_ADJ_TMP_RANGE];		/* 161 */
    gdebugIO.debugbuf[107] = evi_adjust_tmp[0][EVI_ADJ_TMP_USAGE];		/* 162 */

    gdebugIO.debugbuf[108] = dbg_PU_10s;		/* 163 */
    gdebugIO.debugbuf[109] = dbg_PU_20s;		/* 164 */
    gdebugIO.debugbuf[110] = dbg_PU_60s;		/* 165 */
//    gdebugIO.debugbuf[111] = power_idle;	    /* 166 */
//    gdebugIO.debugbuf[112] = power_other_run;	/* 167 */
}


/*******************************************************************************
 函 数: task()
 功 能: 任务
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void task(void)
{
    if (pcb_type == TYPE_MASTER)    //主板 
    {
        sys_task();
    }
    else                            //从板  
    {
    }
    
    mod_task();         /* 主板和从板都运行从机任务 */
    mod_debug();

    TF_task();
}

/*******************************************************************************
 函 数: punp_init()
 功 能: 邦普初始化
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/

//void punp_init(void)
//{
//
//        DataStore_Parameters();
//        DataStore_EEPROM();
//}


/*******************************************************************************
 函 数: main()
 功 能: 程序主调度
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
#define time_basic_set()    USER_time_basic_set()
#define time_basic_reset()  USER_time_basic_reset()

/*******************************************************************************
 函 数: User_main_init()
 功 能: 应用程序初始化操作
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void User_main_init(void)
{   
    initial();                      //软件初始化
    Task_ID_Read();     /* 识别码读取 */
    
//  remeber_start();
}

/*******************************************************************************
 函 数: User_main_task()
 功 能: 应用程序
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void User_main_task(void)
{   
    Read_IO_Info();
    pid_prog();
    task();
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}
#endif

