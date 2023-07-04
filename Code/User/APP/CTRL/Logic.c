/******************************************************************************

          版权所有 (C), 2001-2016, 广州市邦普电脑技术开发有限公司

 ******************************************************************************
  文 件 名   : Logic.c
  版 本 号   : 初稿
  作    者   : 周志鹏
  生成日期   : 2016年11月14日星期一
  最近修改   :
  功能描述   : 控制逻辑
  函数列表   :
  修改历史   :
  1.日    期   : 2016年11月14日星期一
    作    者   : 周志鹏
    修改内容   : 创建文件

******************************************************************************/
#define _LOGIC_GLOBAL_
#include  "Includes.h"


I16 Dbgdebug_xx;

//U08 hl009a_ver[MAX_VER] = APPFUNCODEVER;  //NOTE-CXW 是放在此处还是放cloud.c

/*-----------------包含头文件 -----------------------------*/



/*-----------------外部变量说明----------------------------*/



/*-----------------外部函数原型说明------------------------*/



/*-----------------内部函数原型说明------------------------*/



/*-----------------全局变量--------------------------------*/



/*-----------------模块级变量------------------------------*/



/*-----------------常量定义--------------------------------*/



/*-----------------宏定义 ---------------------------------*/
/*****************************************************************************
函  数 : USER_1ms_Deal()
功  能 : 用户1ms定时处理
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
void USER_1ms_Deal(void)
{
    U08 u;
    static U16 cnt_1ms = 0;

    static U08 cnt5ms = 0;
    static U08 cnt100ms = 0;
    static U08 cnt500ms = 0;
    static U08 cnt1s = 0;
    static U08 cnt1m = 0;

    if (++cnt_1ms >= 1000)  // 计1S
    {
        cnt_1ms = 0;

        if (F_TIME_SWITCH_1S == 0)  F_TIME_SWITCH_1S = 1;   // F_TIME_SWITCH_1S从0→1为持续1秒，0→1→0 计2秒
        else                        F_TIME_SWITCH_1S = 0;

        COM_ProtectTimer();
        
        if (++hmi_power_time >= 100) hmi_power_time = 0;
    }

    for (u = 0; u < MAX_UART; u++)
    {
//        if (fg_baudrate_confirm[u])
//        {
            switch (com_baudrate[u])
            {
                case B_2400:
                    if (cnt_1ms % 2 == 1)
                    {
                        COM_2MsTimer(u);       
                    }
                    if (cnt_1ms % 5 == 1)
                    {
                        COM_VariableTimer(u);
                    }
                    break;
                default:
                    if (cnt_1ms % 2 == 1)
                    {
                        COM_2MsTimer(u);
                        COM_VariableTimer(u);
                    }
                    break;
            }
//        }
//        else                /* 波特率不确定 */
//        {
//            if (cnt_1ms % 5 == 1)
//            {
//                COM_2MsTimer(u);
//                COM_VariableTimer(u);
//            }
//        }    
    }

    
//  if (cnt_1ms % 2 == 1)
//  {
//      COM_2MsTimer();
//  }

//  if (cnt_1ms % 5 == 1)
//  {
//        COM_5MsTimer();
//  }

    /* 以下参数 LY520A 的时钟处理写法 */
    if (++cnt5ms >= 5) /* 5ms */
    {       
        cnt5ms = 0;
        SysMsCount[C_5MS]++;
    }

    if (++cnt100ms >= 100) /* 100ms */
    {   
        cnt100ms = 0;
        F_TIME_BASIC_100MS = 1;
        SysMsCount[C_100MS]++;
        sys_status |= 0x01<<Is100Ms;

        if (++cnt500ms >= 5) /* 500ms */
        {   
            cnt500ms = 0;
            SysMsCount[C_500MS]++;
            F_TIME_BASIC_500MS = 1;

            flash_500ms = !flash_500ms;
        }

        if ( ++cnt1s >= 10 ) /* 1s */
        {       
            cnt1s = 0;
            SysMsCount[C_1S]++;
            sys_status |= 0x01<<IsSecond;
            F_TIME_BASIC_SEC = 1;

            flash_1s = !flash_1s;

            if (power_time != 0xffff) power_time++;

            if( ++cnt1m >= 60 ) /* 1min */
            {   
                cnt1m = 0;
                SysMsCount[C_1M]++;
                sys_status |= 0x01<<IsMinute;
                F_TIME_BASIC_MIN = 1;
            }
        }
    }
}

/*****************************************************************************
函  数 : USER_Init()
功  能 : 用户初始化
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
#define     HWIO_COLLECT_DLY        (500)
void USER_Init(void)
{
    /* 上电数据采集确认延时 */
    while (HDW_1ms_Tick() < HWIO_COLLECT_DLY)
    {
        ;
    }   

    /* 初始化 */
    PunpGetSwitch(&gIO.jmp[0], MAX_SW_BIT);
    PunpGetSwPl(&gIO.SwPlate, MAX_SWPL_BIT);

    /* SW1: 辅助电热档位数 */
    IBH_cfg_type = read_IBH_config();

    /* SW2: 机器型号 */
#ifdef USE_SW_MAC
    sw_mac_model_err = FALSE;
    sw_mac_model = read_mac_model();
    /* 判断读取到的机器型号是否超界，若是则用“未设置”机型参数进行覆盖 */
    if (sw_mac_model > MAC_MAX
    ||  sw_mac_model == MAC_MAX
    ||  sw_mac_model < MAC_NONE
    )
    {
        sw_mac_model = MAC_NONE;
        sw_mac_model_err = TRUE;
    }
#endif

    /* 级联地址 */
    FBUS_ADDR = read_address();
    FBUS_ADDR %= MAX_PCB;

    /* 压机数量 */
    val_comp_num=1; //read_comp_num();

    if (FBUS_ADDR != 0) 
    {
        pcb_type = TYPE_SLAVER;
    }
    else
    {
        pcb_type = TYPE_MASTER;
    }
    
    pcb_addr = FBUS_ADDR;

    pmac = &cell[MAX_PCB];

//    ARRCLR(fg_commu_connect);

    /* 应用逻辑 */
    User_main_init();

    COM_Init();

    /* 版本号 */
    STRCPY(&idu_mac_ver[FBUS_ADDR][0], APPFUNCODEVER);
    if (pcb_addr != 0)
    {
        /* 仅从模块需要将版本号赋值至主模块版本号的存放位置，以便从模块上的4G模块可以正常读取 */
        STRCPY(&idu_mac_ver[0][0], APPFUNCODEVER);
    }

    /* 来电自启动(未使用温控器时有效) */
    if (val_auto_run == USED && !USE_UIRT_CTRL)
    {
//        key_run = TRUE;
        if (USE_DUAL_ZONE_CTRL)
        {
            fg_zone_A_on = TRUE;
            fg_zone_B_on = TRUE;
        }
        else
        {
            fg_zone_A_on = TRUE;
        }

        if (val_hotw_used)
        {
            fg_hotw_on = TRUE;
        }
    }
    /* 掉电记忆(未使用温控器时有效) */
    if (val_remember_run == USED && !USE_UIRT_CTRL)
    {
//        if(auto_run_bit == ACT_RUN)
//        {
//            key_run = TRUE;
//        }
        /* A区 */
        if (GETBIT(auto_run_bit, ACT_ZONE_A))
        {
            fg_zone_A_on = TRUE;
        }

        /* B区 */
        if (GETBIT(auto_run_bit, ACT_ZONE_B))
        {
            fg_zone_B_on = TRUE;
        }

        /* 热水 */
        if (GETBIT(auto_run_bit, ACT_HOTW))
        {
            fg_hotw_on = TRUE;
        }
    }
    
    controller_is_punp = TRUE;

    SH_init();
    TYB_init();
    EK_mst_init();
}

/*****************************************************************************
函  数 : USER_GetTime()
功  能 : 时钟函数
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
U16 USER_GetTime(U08 num)
{
    U16 Count;

    DISABLE_IRQ();
        Count = SysMsCount[num];
    ENABLE_IRQ();

    return  Count;
}

/*****************************************************************************
函  数 : USER_time_basic_set()
功  能 : 开时间基准
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
void USER_time_basic_set(void)
{
    if (F_TIME_BASIC_MIN) 
    {
        F_TIME_BASIC_MIN = 0;
        F_TIME_BASIC_MIN_BACK = 1;
    }
    if (F_TIME_BASIC_SEC) 
    {
        F_TIME_BASIC_SEC = 0;
        F_TIME_BASIC_SEC_BACK = 1;
    }
    if (F_TIME_BASIC_500MS) 
    {
        F_TIME_BASIC_500MS = 0;
        F_TIME_BASIC_500MS_BACK = 1;
    }
    if (F_TIME_BASIC_100MS) 
    {
        F_TIME_BASIC_100MS = 0;
        F_TIME_BASIC_100MS_BACK = 1;
    }
}

/*****************************************************************************
函  数 : USER_time_basic_reset()
功  能 : 关时间基准
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
void USER_time_basic_reset(void)
{
    if (F_TIME_BASIC_MIN_BACK)   F_TIME_BASIC_MIN_BACK   = 0;
    if (F_TIME_BASIC_SEC_BACK)   F_TIME_BASIC_SEC_BACK   = 0;
    if (F_TIME_BASIC_500MS_BACK) F_TIME_BASIC_500MS_BACK = 0;
    if (F_TIME_BASIC_100MS_BACK) F_TIME_BASIC_100MS_BACK = 0;
        
//  F_TIME_SWITCH_1S_BAK=F_TIME_SWITCH_1S;          // 1秒切换标志，用完同步     
}

/****************************************************************
函数：work_mac_rd_sta
功能：读变频器
参数：设备
返回：无
其它：
*****************************************************************/
static U16 fg_100ms_invt_rd_sta;
void USER_invt_rd_sta(INVT_ID_ENUM id, U08 com)
{
    static U08 cnt1s = 0;
    static U16 cnt10s = 0;
    
    if (fg_100ms_invt_rd_sta
    &&  COM_QGetNodeCnt(com) < MAX_L_NODE_NUM
    )
    {
        fg_100ms_invt_rd_sta = 0;			/* 2022-9-29 该标志不一定100ms清零一次，导致卡位时间与实际通讯间隔有关 */
        
        if (++cnt1s >= 10)      cnt1s = 0;
        if (++cnt10s >= 100)    cnt10s = 0;

        /* 2022-9-29 临时补丁 商检时增加每次读取一次变频器状态 */
        if (TF_OPEN_COMP)
        {
            com_invt_rd(com, id, invt_states_get_addr(), invt_states_get_size(), Q_PRIO_LOW);
        }

        /* 一个卡位1秒 */
        switch (cnt1s)
        {
            case 0  :   
                com_invt_rd(com, id, INVT_ADDR_AD, INVT_AD_SIZE, Q_PRIO_LOW);
                break;
            case 1  :  
                com_invt_rd(com, id, invt_states_get_addr(), invt_states_get_size(), Q_PRIO_LOW);
                break;      
            case 2  : 
                com_invt_rd(com, id, INVT_ADDR_CUR_WAVE, INVT_CUR_WAVE_SIZE, Q_PRIO_LOW);
                break;
            case 3  : 
                if (info_invt_para[id].fg_rd)
                {
                    com_invt_rd(com, id, info_invt_para[id].addr, 1, Q_PRIO_LOW);
                }
                break;   

            case 4  : 
                com_invt_rd(com, id, invt_paras_get_addr(), invt_paras_get_size(), Q_PRIO_LOW);
                break;    

            /* 最多扩展到9，超过10个卡位需修改相关代码 */
            default :   
                break;
        }

        /* 一个卡位10秒 */
        switch (cnt10s)
        {
            case 0  :  
//                com_invt_rd(com, id, invt_paras_get_addr(), invt_paras_get_size(), Q_PRIO_LOW);
                break;
            case 1  :  
                com_invt_rd(com, id, invt_paras_fa_get_addr(), FA_NUM1, Q_PRIO_LOW);
                break; 
            case 2  :  
                com_invt_rd(com, id, FA_ADDR(FA_NUM1), FA_MAX-FA_NUM1, Q_PRIO_LOW);
                break; 
            case 3  : 
                com_invt_rd(com, id, invt_paras_fb_get_addr(), invt_paras_fb_get_size(), Q_PRIO_LOW);
                break;
            case 4  : 
                com_invt_rd(com, id, invt_paras_f10_get_addr(), invt_paras_f10_get_size(), Q_PRIO_LOW);
                break;
            case 5  :    
                com_invt_rd(com, id, INVT_ADDR_EC_FAN_FG, INVT_EC_FAN_FG_SIZE, Q_PRIO_LOW);
                break;
            case 6  :  
                com_invt_rd(com, id, invt_paras_f9_get_addr(), invt_paras_f9_get_size(), Q_PRIO_LOW);
                break;
            case 7  :
                com_invt_rd(com, id, invt_paras_f6_get_addr(), invt_paras_f6_get_size(), Q_PRIO_LOW);
                break;
            case 8  :  
                com_invt_rd(com, id, invt_paras_fp_get_addr(), invt_paras_fp_get_size(), Q_PRIO_LOW);
                break; 
            /* 最多扩展到99，超过100个卡位需修改相关代码 */
            default :   
                break;
        }
    }
}

/****************************************************************
函数：cmd_in_inverter 
功能：判断是否需发送命令到变频器，插入通讯结点（命令有变化或间隔大于5秒才发）。
参数：变频器序号
返回：无
其它：各个具体命令仅改变命令值，不插入通讯结点。
*****************************************************************/
void USER_invt_wr_cmd(INVT_ID_ENUM id, U08 com)
{
    U16 fg_invt_cmd_chg=FALSE, fg_invt_para_chg=FALSE;
    U16 fg_invt_para_fa_chg=FALSE, fg_invt_para_fb_chg=FALSE, fg_invt_para_f10_chg=FALSE;
    U16 fg_invt_para_f9_chg=FALSE, fg_invt_para_f6_chg=FALSE;
    U16 addr;

    // 处理变频器参数（写）
    {
        fg_invt_para_chg = invt_para_isChanged(id); //判断基本参数
        fg_invt_para_fa_chg = invt_para_fa_isChanged(id);//判断fa参数
        fg_invt_para_fb_chg = invt_para_fb_isChanged(id);//判断fb参数
        fg_invt_para_f10_chg = invt_para_f10_isChanged(id);//判断f10参数
        fg_invt_para_f9_chg = invt_para_f9_isChanged(id);//判断f9参数
        fg_invt_para_f6_chg = invt_para_f6_isChanged(id);//判断f6参数

        if (fg_invt_para_chg!=FALSE)
        {
            addr = invt_paras_get_addr()+fg_invt_para_chg-1;   

            if (com_invt_wr(com, id, addr , 1, Q_PRIO_HIGH))
            {
                invt_paras_backup(id, fg_invt_para_chg-1);
            }
        }
        else if (fg_invt_para_fa_chg!=FALSE)
        {
            addr = invt_paras_fa_get_addr()+fg_invt_para_fa_chg-1;   

            if (com_invt_wr(com, id, addr , 1, Q_PRIO_HIGH))
            {
                invt_paras_fa_backup(id, fg_invt_para_fa_chg-1);
            }
        }
        else if (fg_invt_para_fb_chg!=FALSE)
        {
            addr = invt_paras_fb_get_addr()+fg_invt_para_fb_chg-1;   

            if (com_invt_wr(com, id, addr , 1, Q_PRIO_HIGH))
            {
                invt_paras_fb_backup(id, fg_invt_para_fb_chg-1);
            }
        }
        else if (fg_invt_para_f10_chg!=FALSE)
        {
            addr = invt_paras_f10_get_addr()+fg_invt_para_f10_chg-1;   

            if (com_invt_wr(com, id, addr , 1, Q_PRIO_HIGH))
            {
                invt_paras_f10_backup(id, fg_invt_para_f10_chg-1);
            }
        }
        else if (fg_invt_para_f9_chg!=FALSE)
        {
            addr = invt_paras_f9_get_addr()+fg_invt_para_f9_chg-1;   

            if (com_invt_wr(com, id, addr , 1, Q_PRIO_HIGH))
            {
                invt_paras_f9_backup(id, fg_invt_para_f9_chg-1);
            }
        }
        else if (fg_invt_para_f6_chg!=FALSE)
        {
            addr = invt_paras_f6_get_addr()+fg_invt_para_f6_chg-1;
            
            if (com_invt_wr(com, id, addr , 1, Q_PRIO_HIGH))
            {
                invt_paras_f6_backup(id, fg_invt_para_f6_chg-1);
            }
        }
    }
    
    // 处理变频器命令(先处理完参数，再处理命令，因为处理参数后涉及到命令更新)
    if (!fg_invt_para_chg
    &&  !fg_invt_para_fa_chg
    &&  !fg_invt_para_fb_chg
    &&  !fg_invt_para_f10_chg
    &&  !fg_invt_para_f9_chg
    &&  !fg_invt_para_f6_chg
    )
    {
        fg_invt_cmd_chg = invt_cmd_isChanged(id);

        if (fg_invt_cmd_chg!=FALSE)
        {
            addr = invt_cmd_get_addr()+fg_invt_cmd_chg-1;   

            if (com_invt_wr(com, id, addr , 1, Q_PRIO_HIGH))
            {
                invt_cmd_backup(id, fg_invt_cmd_chg-1);

            }
        }
        else if (info_invt_para[id].fg_wr)
        {
            com_invt_wr(com, id, info_invt_para[id].addr , 1, Q_PRIO_HIGH);
        }
    }
    
    
}

/*******************************************************************************
 函 数: get_scan_period()
 功 能: 获取扫描周期
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
static void get_scan_period(void)
{
    static BOL fg_first = TRUE;
    static U32 s_tick;

    /* 更新系统扫描周期 */
    if (fg_first)
    {
        fg_first = FALSE;
        MinScanPeriod = 0xffff;
        MaxScanPeriod = 0;
        s_tick = SYS_GetTick();
    }
    else
    {
        CurScanPeriod = (U16)(SYS_GetTick() - s_tick);
        s_tick = SYS_GetTick();
        if (CurScanPeriod < MinScanPeriod)
        {
            MinScanPeriod = CurScanPeriod;
        }
        if (CurScanPeriod > MaxScanPeriod)
        {
            MaxScanPeriod = CurScanPeriod;
        }
    }

}

#if 0
/*****************************************************************************
函  数 : USER_COMMU_invt()
功  能 : 通讯命令处理
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
//上电后读取EX的版本信息时间
#define READ_VERSION_TIME_BGN 4  
#define READ_VERSION_TIME_END 6

BOOL USER_COMMU_invt(U08 com)
{
    BOOL fg_node_inserted = FALSE;

    if (F_TIME_BASIC_100MS_BACK)
    {
        _CNT(fg_100ms_invt_rd_sta);
    }
    
    if ( TestProgStaus != TPS_NO_TEST_CMD
//      || power_time < INIT_TIME 
      )
    {
        return TRUE;//开机倒计时或进入测试
    }


    /* 变频器、变频风机 */
    if (COM_QGetNodeCnt(com) <= 2)
    {
        enum 
        {
            FBUS_STEP_INVT, 
            FBUS_STEP_EX_EC
        };
        static U08 standy_step;

        switch (standy_step)
        {
            default:
            case FBUS_STEP_INVT:/* N0004 */
                {
                    static INVT_ID_ENUM id;
                    if ((id==INVT_ID_1 && val_inverter_used)
                    ||  (id==INVT_FAN1 && val_freq_fan_set==FREQ_FAN_SET_EC)
                    ||  (id==INVT_FAN2 && val_freq_fan_set==FREQ_FAN_SET_EC && val_freq_fan_num>1)
                    )
                    {
                        USER_invt_rd_sta(id, com);
                        USER_invt_wr_cmd(id, com);
                    }
                    if (++id >= INVT_ID_MAX) 
                    {
                        id = (INVT_ID_ENUM)0;
                        standy_step = FBUS_STEP_EX_EC;
                    }
                }
                break;
            case FBUS_STEP_EX_EC:
                if (USE_EX_EC_FAN)
                {
                    if (ec_fan_commu())
                    {
                        standy_step = FBUS_STEP_INVT;
                    }
                }
                else
                {
                    standy_step = FBUS_STEP_INVT;
                }
                break;
        } 
        fg_node_inserted = TRUE;
    }

    return fg_node_inserted;
}
#endif

extern U08 ec_fan_commu_interval_100ms[ECFAN_MAX];
//U08 cnt_intv_idu_odu_commu;

void USER_Commu_ODU_INVT(U08 com)
{
    static U08 commu_id = 0;
    
    if (F_TIME_BASIC_100MS_BACK)
    {
        _CNT(fg_100ms_invt_rd_sta);
    }

//    if (F_TIME_BASIC_SEC_BACK)
//    {
//        _CNT_WHEN(cnt_intv_idu_odu_commu, (commu_id==10));
//    }

    if ( TestProgStaus != TPS_NO_TEST_CMD
//      || power_time < INIT_TIME 
      )
    {
        return;//开机倒计时或进入测试
    }
   
    switch (commu_id)
    {
        case 0 :    
            if (power_time >= 5)
            {
                commu_id = 10;
            }
            break;
            
        case 10:            /* 通讯口短接方案 */
            /* 禁用变频器 */
            if (!USE_INVT)
            {
                commu_id = 11;
                break;
            }

//            /* 外机保持连接，仅断连变频器 */
//            if ((!GET_INVT_CONNECT && GET_INOUT_CONNECT)
//            && (cnt_intv_idu_odu_commu > 2)
//            )
//            {
//                commu_id = 11;
//                cnt_intv_idu_odu_commu = 0;
//                break;
//            }

            /* 变频器、变频风机 */
//            if (!GET_INVT_CONNECT || COM_QGetNodeCnt(com) <= 2)
            if (COM_QGetNodeCnt(com) <= 2)
            {
                enum 
                {
                    FBUS_STEP_INVT,
                    FBUS_STEP_INVT_FAN1,
                    FBUS_STEP_INVT_FAN2, 
                    FBUS_STEP_EX_EC1,
                    FBUS_STEP_EX_EC2,
                };
                
                static INVT_ID_ENUM invt_id;
                static ECFAN_ENUM ecfan_id;
                static U08 standy_step = FBUS_STEP_INVT;
                
                /* 选择当前驱动类型设备编号 */
                switch (standy_step)
                {
                    default:
                    case FBUS_STEP_INVT:       invt_id = INVT_ID_1;  break;
                    case FBUS_STEP_INVT_FAN1:  invt_id = INVT_FAN1;  break;
                    case FBUS_STEP_INVT_FAN2:  invt_id = INVT_FAN2;  break;
                    case FBUS_STEP_EX_EC1:     ecfan_id = ECFAN_1;   break;
                    case FBUS_STEP_EX_EC2:     ecfan_id = ECFAN_2;   break;
                }
    
                /* 节点插入队列 */
                switch (standy_step)
                {
                    case FBUS_STEP_INVT:
                    case FBUS_STEP_INVT_FAN1:
                    case FBUS_STEP_INVT_FAN2:
                        USER_invt_rd_sta(invt_id, com);
                        USER_invt_wr_cmd(invt_id, com);
                        break;
                    case FBUS_STEP_EX_EC1:
                    case FBUS_STEP_EX_EC2:
                        if (ec_fan_commu_interval_100ms[ecfan_id] >= EC_FAN_COMMU_INTERVAL)
                        {
                            ec_fan_commu_interval_100ms[ecfan_id] = 0;
                            
                            ec_fan_commu_rd_sta(ecfan_id);
                            ec_fan_commu_wr_ctrl(ecfan_id);
                        }
                        break;
                }
    
                /* 切换至下一驱动 */
                switch (standy_step)
                {
                    case FBUS_STEP_INVT:
                        standy_step = USE_INVT_FAN1 ? FBUS_STEP_INVT_FAN1 \
                                                    : (USE_ECFAN1 ? FBUS_STEP_EX_EC1 \
                                                                  : FBUS_STEP_INVT);
                        break;
                    case FBUS_STEP_INVT_FAN1:
                        standy_step = USE_INVT_FAN2 ? FBUS_STEP_INVT_FAN2 \
                                                    : FBUS_STEP_INVT;                   
                        break;
                    case FBUS_STEP_INVT_FAN2:
                        standy_step = FBUS_STEP_INVT;
                        break;
                    case FBUS_STEP_EX_EC1:
                        standy_step = USE_ECFAN2 ? FBUS_STEP_EX_EC2 \
                                                 : FBUS_STEP_INVT;
                        break;
                    case FBUS_STEP_EX_EC2:
                        standy_step = FBUS_STEP_INVT;
                        break;
                }

//                if (GET_INVT_CONNECT)/* 波特率自适应完成 */
//                {
                    /* 仍为邦普变频压机驱动 */
                    if (standy_step == FBUS_STEP_INVT)
                    {
                        commu_id = 11;      /* 变频发送结束，切换至内外机发送 */
                    }
//                }
//                else
//                {
//                    standy_step = FBUS_STEP_INVT;
//                    invt_id = INVT_ID_1;
//                }
            }
            break;
    
        case 11:
//            if ((!GET_INOUT_CONNECT && COM_QEmpty(com)) || COM_QEmpty(com))
            if (COM_QEmpty(com))
            {
                U08 i;

                for (i=0; i<EXIO_KLV_MAX; i++)
                {
                    EK_mst_create_request(com, i);
                }

//                if (INVT_INOUT_BOTH_CONNECT)
//                {
                    commu_id = USE_INVT ? 10 : 11;
//                }

//                if (!INVT_INOUT_BOTH_CONNECT)
//                {
//                    commu_id = 10;
//                }
            }
            break;
    }
}


/****************************************************************
函数：读取机组(R_LAN从机)状态
功能：主RLAN读取从RLAN的系统状态
参数：无
返回：无
其它：读取方式为循环读取，低优先级。
*****************************************************************/
#define     MAX_RD_DATA_NUM     120         /* 每次更新状态数据的最大值 字 */   
U08 work_mac_rd_sta(U08 com)
{
    U16 data_addr = 0;
    static U08 mac_now = 0; /* 当前通讯的机组 */
        
    if (COM_QGetNodeCnt(com) < MAX_L_NODE_NUM)
    {
        U08 max_cell_rd = 0;    /* CELL 结构体读取最大次数 */
        U08 fg_rd_overflow_max = 0;     /* 读取溢出。即最后一次非读取最大数据标志 */
        static U08 cell_rd_cnt = 0;     /* CELL 结构读取的次数 */

        /* CELL结构体数据读取次数 */
        max_cell_rd = CELL_SIZE / MAX_RD_DATA_NUM;
        if (CELL_SIZE % MAX_RD_DATA_NUM)
        {
            max_cell_rd = max_cell_rd + 1;
            fg_rd_overflow_max = 1;
        }

        /* 
         * 2017/12/18--1027 XMacro 
         * 模块禁用，则跳过禁用模块通讯节省不必要的通讯时间。
         */
        if (!CELL_Is_used(mac_now))
        {
            if (!M_CM_GetStatus(mac_now, MAX_COMP, CELL_RUN))//跳过读取禁用模块状态，必须待模块完全关闭才行
            {
                cell_rd_cnt = max_cell_rd;
            }
        }

        /* 分次处理结构体数据通讯 */
        if (cell_rd_cnt < max_cell_rd)
        {
            U08 read_num;
            
            /* 数据读取 */
            data_addr = (U16)FBUS_MAC_BGN + ((U16)CELL_SIZE*mac_now) + cell_rd_cnt*MAX_RD_DATA_NUM;
            read_num = MAX_RD_DATA_NUM;
            if (((cell_rd_cnt+1) >= max_cell_rd) && fg_rd_overflow_max == 1)
            {
                read_num = CELL_SIZE % MAX_RD_DATA_NUM;
            }
            /* 2023-1-9补丁：跳过cell_cmd，否则从从机读取的cell_cmd会覆盖主机写入的命令 */
            if (cell_rd_cnt == 0)
            {
                data_addr += MAX_CMD;
                read_num -= MAX_CMD;
            }
            com_FBUS_rd(com, mac_now, data_addr, read_num, Q_PRIO_LOW);

            cell_rd_cnt++;

            /* 
             * 2017/12/18--1027 XMacro 
             * 通讯故障后，只读取一次对应的判断。
             * 这样，可以节省通讯时间。
             */
            if (GETBIT(slave_commu_error, mac_now))
            {
                cell_rd_cnt = max_cell_rd;
            }
        }
        else
        {
            /* 单个模块的cell数据读取完成 */
            cell_rd_cnt = 0;
            if (++mac_now >= val_pcb_num)       
            {
                mac_now = 1;
                
                return 1;
            }
        }
    }

    return 0;
}

/****************************************************************
函数：work_mac_rd_ver()
功能：主RLAN读取从RLAN的版本号
参数：无
返回：无
其它：读取方式为循环读取，低优先级。
*****************************************************************/
static U16 fg_mac_ver_rd_cplt;              /* 版本号读取完成标志：每一位代表一个模块 */
U08 work_mac_rd_ver(U08 com)
{   
    U16 data_addr = 0;
    static U08 mac_now = 0; /* 当前通讯的机组 */

    if (COM_QGetNodeCnt(com) < MAX_L_NODE_NUM)
    {
        U08 max_data_rd_cnt = 0;            /* 数据读取最大次数 */
        U08 fg_rd_overflow_max = 0;     /* 读取溢出。即最后一次非读取最大数据标志 */
        static U08 data_rd_cnt = 0;     /* 数据读取的次数 */

        /* CELL结构体数据读取次数 */
        max_data_rd_cnt = ONE_VER_SIZE / MAX_RD_DATA_NUM;
        if (ONE_VER_SIZE % MAX_RD_DATA_NUM)
        {
            max_data_rd_cnt++;
            fg_rd_overflow_max = 1;
        }

        /* 分次处理结构体数据通讯 */
        if (data_rd_cnt < max_data_rd_cnt)
        {
            U08 read_num;
            
            /* 数据读取 */
            data_addr = (U16)FBUS_VER_BGN + ((U16)ONE_VER_SIZE*mac_now) + data_rd_cnt*MAX_RD_DATA_NUM;
            read_num = MAX_RD_DATA_NUM;
            if (((data_rd_cnt+1) >= max_data_rd_cnt) && fg_rd_overflow_max == 1)
            {
                read_num = ONE_VER_SIZE % MAX_RD_DATA_NUM;
            }
            com_FBUS_rd(com, mac_now, data_addr, read_num, Q_PRIO_LOW);

            /* 数据读取 */
            data_addr = (U16)FBUS_ODU_VER_BGN + ((U16)ONE_ODU_VER_SIZE*mac_now) + data_rd_cnt*MAX_RD_DATA_NUM;
            read_num = MAX_RD_DATA_NUM;
            if (((data_rd_cnt+1) >= max_data_rd_cnt) && fg_rd_overflow_max == 1)
            {
                read_num = ONE_ODU_VER_SIZE % MAX_RD_DATA_NUM;
            }
            com_FBUS_rd(com, mac_now, data_addr, read_num, Q_PRIO_LOW);

            data_rd_cnt++;

            /* 
             * 2017/12/18--1027 XMacro 
             * 通讯故障后，只读取一次对应的判断。
             * 这样，可以节省通讯时间。
             */
            if (GETBIT(slave_commu_error, mac_now))
            {
                data_rd_cnt = max_data_rd_cnt;
            }
        }
        else
        {
            /* 单个模块的数据读取完成 */
            data_rd_cnt = 0;
            SETBIT(fg_mac_ver_rd_cplt, mac_now);
            if (++mac_now >= val_pcb_num)       
            {
                mac_now = 1;
                return 1;
            }           
        }
    }

    return 0;
}

/*******************************************************************************
 函 数: work_mac_rd_mod_para()
 功 能: 主模块读取某个从模块的参数值
 参 数: com--; 
 返 回: 
 其 它: 空
*******************************************************************************/
U08 work_mac_rd_mod_para(U08 com)
{   
    U16 data_addr = 0;
    U08 mac_now = modParaRdIdDebug; /* 当前通讯的机组 */

    if (COM_QGetNodeCnt(com) < MAX_L_NODE_NUM)
    {
        U08 max_data_rd_cnt = 0;            /* 数据读取最大次数 */
        U08 fg_rd_overflow_max = 0;         /* 读取溢出。即最后一次非读取最大数据标志 */
        static U08 data_rd_cnt = 0;         /* 数据读取的次数 */

        /* para_C数据读取次数 */
        max_data_rd_cnt = PARA_SIZE / MAX_RD_DATA_NUM;
        if (PARA_SIZE % MAX_RD_DATA_NUM)
        {
            max_data_rd_cnt++;
            fg_rd_overflow_max = 1;
        }

        /* 分次处理结构体数据通讯 */
        if (data_rd_cnt < max_data_rd_cnt)
        {
            U08 read_num;
            
            /* 数据读取 */
            data_addr = (U16)ADDR_PARA_BGN + data_rd_cnt*MAX_RD_DATA_NUM;
            read_num = MAX_RD_DATA_NUM;
            if (((data_rd_cnt+1) >= max_data_rd_cnt) && fg_rd_overflow_max == 1)
            {
                read_num = PARA_SIZE % MAX_RD_DATA_NUM;
            }
            com_FBUS_rd(com, mac_now, data_addr, read_num, Q_PRIO_LOW);

            data_rd_cnt++;

            /* 
             * 2017/12/18--1027 XMacro 
             * 通讯故障后，只读取一次对应的判断。
             * 这样，可以节省通讯时间。
             */
            if (GETBIT(slave_commu_error, mac_now))
            {
                data_rd_cnt = max_data_rd_cnt;
            }
        }
        else
        {
            /* 当前模块的数据读取完成 */
            data_rd_cnt = 0;
            return 1;          
        }
    }

    return 0;
}

/****************************************************************
函数：发送系统状态给子机(cmd_mac_sta)
功能：子机在状态变化或定时发送状态给子机。(同时发送状态和系统温度)
参数：板序号
返回：无
其它：各个具体命令仅改变命令值，不插入通讯结点。
*****************************************************************/
static U08 cnt_commu_step_ot;               /* 读取状态的最长时间计时 */
U08 cmd_mac_sta(U08 com)
{
    /* 广播系统状态 或 超过3秒会强行退出 */
    if (COM_QGetNodeCnt(com) < MAX_L_NODE_NUM)
    {
        if (com_FBUS_bro(com, FBUS_SYS_STA_BGN, FBUS_SYS_STA_SIZE, Q_PRIO_LOW))
        {
            return 1;
        }
    }

    return 0;
}

/*******************************************************************************
 函 数: cmd_bro_special()
 功 能: 主机发送；广播特殊数据
 参 数: com--; 
 返 回: 
 其 它: 使用bro_special通道时，此处要使用相同变量进行传递，否则主模块无法通过级
        联通信的方式进行接收
*******************************************************************************/
U08 cmd_bro_special(U08 com)
{   
    static U08 cnt_bro_itv;
    /* 广播系统状态 或 超过3秒会强行退出 */
    if (COM_QGetNodeCnt(com) < MAX_L_NODE_NUM
    &&  cnt_bro_itv >= 5
        )
    {
        if (com_FBUS_bro(com, ADDR_SPECIAL_BGN, WR_SPECIAL_NUM, Q_PRIO_LOW))
        {
            cnt_bro_itv = 0;
                
            bro_special[0] = fg_idle_sta_antifreeze;
            bro_special[1] = cell[MAX_PCB].cell_runtime[SYS_RUNTIME];
            bro_special[2] = limit_value;
            bro_special[3] = limit_type;
            bro_special[4] = sv_temp_energy[SUB_SYS_AC_HOTW];
            bro_special[5] = ctrl_temp_energy[SUB_SYS_AC_HOTW];
            bro_special[6] = fg_warm_complete;
            bro_special[7] = fg_timed_disinfect_run;
            bro_special[8] = MacTypeMaster;
            bro_special[9] = fg_silent_mode;
            bro_special[10] = fg_ability_test_run;
            bro_special[11] = ctrl_mode[SUB_SYS_AC_HOTW];
            bro_special[12] = sv_temp_freq[SUB_SYS_AC_HOTW];
            bro_special[13] = fg_TBH_run;   /* TBH已开启 */
            bro_special[14] = ac_mode;
            bro_special[15] = fg_idle_sta_antifreeze_IBH_need;
            bro_special[16] = fg_force_IBH_run;             /* 强开辅助电热 */
            bro_special[17] = fg_AHS_run;
            bro_special[18] = fg_PUMPf_test_run;
            bro_special[19] = fg_refri_recy_act;
            bro_special[20] = heat_runtime[SUB_SYS_AC_HOTW].run;
            bro_special[21] = hotw_runtime.run;
            bro_special[22] = fg_timed_disinfect_IBH_need;
            bro_special[23] = fg_enter_test_run;
            bro_special[24] = fg_test_run_heat_IBH_run;
            bro_special[25] = fg_pump_test_run;
            bro_special[26] = fg_test_run_check_LDIw;
            bro_special[27] = fg_test_run_PUMPf_off;
            bro_special[28] = fg_floor_preheat_run;
            bro_special[29] = fg_floor_dryup_run;            
            bro_special[30] = fg_air_empty_test_run;
            bro_special[31] = sg_power_mode;
            bro_special[32] = fg_timed_disinfect_TBH_need;
            bro_special[33] = fg_timed_disinfect_AHS_need;
            bro_special[34] = fg_timed_disinfect_AHS_on;
            bro_special[35] = ctrl_mode[SUB_SYS_AC_ONLY];
            bro_special[36] = sv_temp_energy[SUB_SYS_AC_ONLY];
            bro_special[37] = ctrl_temp_energy[SUB_SYS_AC_ONLY];
            bro_special[38] = sv_temp_freq[SUB_SYS_AC_ONLY];
            bro_special[39] = status_run;
            bro_special[40] = fg_run_sta_ac_antifreeze_IBH_need;
            bro_special[41] = fg_run_sta_hotw_antifreeze_IBH_need;
            bro_special[42] = idle_sta_antifreeze_stage;
            bro_special[43] = fg_run_sta_ac_antifreeze;
            bro_special[44] = fg_run_sta_hotw_antifreeze;
            bro_special[45] = fg_zone_A_timed_antifreeze_COMPf_need;
            bro_special[46] = fg_zone_B_timed_antifreeze_COMPf_need;
            bro_special[47] = fg_hotw_timed_antifreeze_COMPf_need;
            bro_special[48] = fg_cool_need;
            bro_special[49] = fg_heat_need;
            bro_special[50] = fg_hotw_need;
            bro_special[51] = silent_mode_level;
            bro_special[52] = fg_sv_temp_energy_overwrite_ac;
            bro_special[53] = ctrl_temp_type_energy_ac;
            bro_special[54] = heat_runtime[SUB_SYS_AC_ONLY].run;
            bro_special[55] = fg_energy_need[SUB_SYS_AC_HOTW];
            bro_special[56] = fg_energy_need[SUB_SYS_AC_ONLY];
            bro_special[57] = fg_hotw_timed_antifreeze_S3V1_OFF_need;
            bro_special[58] = fg_zone_A_run;
            bro_special[59] = fg_zone_B_run;
            bro_special[60] = fg_hotw_run;

            return 1;
        }
    }

    if (F_TIME_BASIC_SEC_BACK)
    {
        _CNT(cnt_bro_itv);
    }

    return 0;
}

/*****************************************************************************
函  数 : USER_Commu_Fbus()
功  能 : 级联通讯处理
参  数 : 无
返  回 : 无
其  他 : 逻辑待优化-2021-7-14-kjh：不同外设共用通讯口，待模块化
*****************************************************************************/
#define     MAC_RW_INTVL        60          /* 读取超时 */
#define     MAC_BRO_INTVL       3           /* 广播超时 */
void USER_Commu_Fbus(U08 com)
{
    U08 i, j;
    U08 no = 0;
    U08 cmd_need = 0;
    static U08  commu_id = 0;
    static U08  commu_id_bak = 0;
    static U16 pcb_num_bak = 0;
    U16 *pfg_mac_ver_rd_cplt = &fg_mac_ver_rd_cplt;

    switch (commu_id)
    {
        case 0 :    /* 初始化 */
            commu_id = 1;           

            break;

        case 1 :    /* 广播特殊数据：要在级联数据之前 */
            if (cmd_bro_special(com) || cnt_commu_step_ot>MAC_BRO_INTVL)
            {
                commu_id = 10;
            }
            break;          

        case 10 :   
            if (m_password_broadcast(com)) break;   /* 密码广播 */
            if (para_broadcast(com)) break;         /* 参数广播 */

            /* 命令广播 和 状态读取 */
            {
                CLRBIT(cmd_modify, 0);  /* 0#模块命令不需要通讯 */

                if (cmd_broadcast != 0)
                {
                    /* 注：以下只能用于0#模块的广播，子模块的命令不能处理 */
                    if (com_FBUS_bro(com, FBUS_CMD_BGN, FBUS_CMD_SIZE, Q_PRIO_HIGH))
                    {
                        cmd_broadcast = 0;        /* 置广播标志 */

                        no = MAX_PCB;      /* 广播信息位于MAX_PCB中 */
                        for (i=0; i<MAX_CMD; i++)         /* 主模块直接使用, 不需要通讯 */
                        {
                            cell[0].cell_cmd[i] |= cell[no].cell_cmd[i];
                            bro_cell_cmd[i] = cell[no].cell_cmd[i]; /* 发送命令 */

                            cell[no].cell_cmd[i] = 0;   /* 使用后清零 */
                        }
                    }
                    break;
                }
                else if (cmd_modify != 0)
                {
                    for (j = 0; j < val_pcb_num; j++)
                    {
                        if (GETBIT(cmd_modify, j))
                        {
                            no = j;
                            cmd_need = 1;
                            break;
                        }
                    }
                    if (cmd_need == 1)
                    {
                        if (com_FBUS_wr(com, no, FBUS_CMD_BGN, FBUS_CMD_SIZE, Q_PRIO_HIGH))
                        {
                            CLRBIT(cmd_modify, no);

                            /* 主模块直接使用, 不需要通讯 */
                            for (i=0; i<MAX_CMD; i++)         
                            {
                                nor_cell_cmd[no][i] = cell[no].cell_cmd[i]; /* 发送命令 */
                                cell[no].cell_cmd[i] = 0;   /* 使用后清零 */
                            }
                            if (fg_trans_control_sta[no] == TRUE)
                            {
                                fg_trans_control_sta[no] = FALSE;
                            }
                        }
                    }
                    break;
                }
                else if (val_pcb_num > 1)   /* 主机通讯命令 状态读取 */
                {
                    enum {STEP_RD_STA, STEP_RD_VER, STEP_RD_MOD_PARA, STEP_RD_MAX};
                    static U08 step_rd = STEP_RD_STA;
                    
                    switch (step_rd)
                    {
                        case STEP_RD_STA:
                            if (work_mac_rd_sta(com) || cnt_commu_step_ot>MAC_RW_INTVL) 
                            {
                                step_rd++;
                            }
                            break;
                        case STEP_RD_VER:
                            /* 标志初始化：模块数改变 */
                            if (pcb_num_bak != val_pcb_num)
                            {
                                pcb_num_bak = val_pcb_num;
                                
                                *pfg_mac_ver_rd_cplt = 0;

                                for (i=val_pcb_num; i<16; i++)
                                {
                                    SETBIT(*pfg_mac_ver_rd_cplt, i);
                                }
                            }
                            /* 标志特殊处理：通讯故障要重新读取 */
                            for (i=0; i<val_pcb_num; i++)
                            {
                                // 不处理主机
                                if (i == 0) continue;
                                
                                if (GETBIT(slave_commu_error, i))
                                {
                                    CLRBIT(*pfg_mac_ver_rd_cplt, i);
                                }

                                // 仅读取一次的情况下，会导致读取不到子机外机板版本号
                                if (idu_mac_ver[i][0] == 0 || odu_mac_ver[i][0] == 0)
                                {
                                    CLRBIT(*pfg_mac_ver_rd_cplt, i);
                                }
                            }
                            
                            /* 只读取一遍版本号，读取完成后不再读取 */
                            if (*pfg_mac_ver_rd_cplt != 0xffff)
                            {
                                if (work_mac_rd_ver(com) || cnt_commu_step_ot>MAC_RW_INTVL) 
                                {
                                    step_rd++;
                                }
                            }
                            else    
                            {
                                step_rd++;
                            }
                            break;

                        /* 读某个从模块的参数值 */
                        case STEP_RD_MOD_PARA:
                            if (modParaRdDebug)
                            {
                                if (work_mac_rd_mod_para(com) || cnt_commu_step_ot>MAC_RW_INTVL)
                                {
                                    step_rd++;
                                    modParaRdDebug = FALSE;
                                }
                            }
                            else
                            {
                                step_rd++;
                            }
                            break;
                            
                        case STEP_RD_MAX:   
                            /* break; */
                        default:
                            step_rd = STEP_RD_STA;
                            commu_id = 20;
                            break;
                    }
                }
                else    
                {
                    commu_id = 30;          /* 跳到风盘通讯 */
                }
            }
            break;

        case 20 :   /* 系统状态广播 */
            if (cmd_mac_sta(com) || cnt_commu_step_ot>MAC_BRO_INTVL)
            {
                commu_id = 30;
            }
            break;

        case 30 :   /* 插入风盘通讯 */
#ifdef COM_FCU_FBUS            
            if (fcu_commu() || cnt_commu_step_ot>MAC_RW_INTVL)
#endif                   
            {
                commu_id = 1;               /* 最后一个通讯任务结束后返回 */
            }
            break;
    }

    if (commu_id_bak != commu_id)
    {
        commu_id_bak = commu_id;
        cnt_commu_step_ot = 0;
    }

    if (F_TIME_BASIC_100MS_BACK)
    {
        if (commu_m_password_broadcast_interval <= 0xfffe)
        {
            commu_m_password_broadcast_interval++;
        }
    }
    
    if (F_TIME_BASIC_SEC_BACK)
    {
        if (cnt_commu_step_ot <= 0xfe) cnt_commu_step_ot++;
    }
}

#ifdef USE_LOG_PRINT
    extern void DBG_LoggingPara(void);
#endif

/*******************************************************************************
 函 数: Timer_deal()
 功 能: 处理计时相关项
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void Timer_deal(void)
{
    disp_time();    /* DM603A界面相关时间 */
    ec_fan_time();
    fcu_time();
    invt_timer(F_TIME_BASIC_100MS_BACK);
    eev_timer(F_TIME_BASIC_100MS_BACK);   //NOTE-CXW 待测2017-5-12
    pid_timer(F_TIME_BASIC_100MS_BACK);
    SH_timer();
    TYB_timer();
    EK_mst_timer();
    modbus_time();
}

/*******************************************************************************
 函 数: COM_deal()
 功 能: 处理通讯相关项
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void COM_deal(void)
{
    /* 显示屏(文本/触摸/wifi屏)通讯 */
    SH_MB_commu_adjust();

    /* 内外机&变频通讯 */
    if (TF_IS_IN)           /* 内机商检时不处理COM_INVT */
    {
        VALCLR(fg_invt_commu_error);
        CLR_EXIO_COMMU_ERR(0);
        s_clr_error(ERR_INVT_COMMU);
        s_clr_error(S_ERR_COMMU_INOUT);
    }
    else
    {
        USER_Commu_ODU_INVT(COM_INVT);
        COM_MasterProg(COM_INVT);
    }

    /* 级联通讯 */
    if (TestProgStaus != TPS_NO_TEST_CMD
    ||  pcb_type == TYPE_SLAVER
        )
    {
        COM_SlaverProg(COM_FCU_FBUS);
    }
    else
    {
        USER_Commu_Fbus(COM_FCU_FBUS);
        COM_MasterProg(COM_FCU_FBUS);
    }

    /* 联网监控 */
    COM_SlaverProg(COM_BMS); 


#ifdef COM_4G
    COM_SlaverProg(COM_4G);
#endif
}

/*******************************************************************************
 函 数: DataStore_deal()
 功 能: 处理参数保存，密码保存等相关项
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void DataStore_deal(void)
{
    DataStore_Parameters();
    DataStore_EEPROM();
}

/*****************************************************************************
函  数 : USER_Main()
功  能 : 用户主函数
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
extern void TEST_Main(void);
void USER_Main(void)
{
    USER_time_basic_set();
    WDG_Refresh();
    if (F_TIME_SWITCH_1S == 0)  Dbgdebug_xx = 0xaaaa;
    else                        Dbgdebug_xx = 0;
    
    if (TestProgStaus != TPS_NO_TEST_CMD || power_time < POWER_ON_INTO_TEST)
    {
        TEST_Main();
    }

    /* 外机版本号已准备好 */
    if (GET_EXIO_VER_READY(0))
    {
        MEMCPY(&odu_mac_ver[FBUS_ADDR], GET_EXIO_VER(0), MAX_VER);
    }
    
    Timer_deal();
    COM_deal();
    DataStore_deal();

    if(power_time==INIT_INVT_RST_TIME)  //倒计时最后一秒复位变频器故障防止不同步上电引起的通讯故障
    {
        static U08 fg_reset = FALSE;
        if(!fg_reset)
        {
            fg_reset = TRUE;
            invt_err_reset();
        }
    }
    
    /* 应用逻辑 */
    if (TestProgStaus == TPS_NO_TEST_CMD && power_time > POWER_ON_INTO_TEST)
    {
        app_4g_ro.cmd = INTO_4G_APP;
        User_main_task();
        get_scan_period();
    }

    app_4g();

#ifdef USE_LOG_PRINT
    if (power_time > 10)
    {
	    LOG_Server();
        DBG_LoggingPara();
    }
#endif

#ifdef M_DEBUG
    /* DEBUG微码：sw2.4为off表示慢速，为on表示快速，支持热拨码；其余微码只要启用USE_QUICK_TIME即为快速 */
    {
        U16 jmp[MAX_SW];
        PunpGetSwitch(&jmp[0], MAX_SW_BIT);
        fg_dbg_slow = !GETBIT_N(jmp, 5);
    }
#endif

    USER_time_basic_reset();
}

