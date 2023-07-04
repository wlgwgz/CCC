/******************************************************************************

          版权所有 (C), 2001-2016, 广州市邦普电脑技术开发有限公司

 ******************************************************************************
  文 件 名   : app_defrost.c
  版 本 号   : 初稿
  作    者   : 周志鹏
  生成日期   : 2018年12月28日星期五
  最近修改   :
  功能描述   : 除霜逻辑
  函数列表   :
              defrost
              defrost_by_emergency
              defrost_by_env_wing
              defrost_by_humidity
              defrost_by_lowpress
              defrost_by_manual
              defrost_by_wing
              defrost_cm_stop
              defrost_fun_frost_time
              defrost_fun_get_interval
              defrost_fun_get_wing
              defrost_fun_rec_data
              defrost_grp_need_exit
              defrost_grp_need_into
  修改历史   :
  1.日    期   : 2018年12月28日星期五
    作    者   : 周志鹏
    修改内容   : 创建文件

******************************************************************************/
#define GBL_DEFROST     1
/*-----------------包含头文件 -----------------------------*/
#include "Includes.h"


//1 *-----------------接口----------------------------*/
// 为了使代码统一,以下为了兼容不同程序的不同实现,而设计的接口,
// 如果其他程序不兼容本文件的函数等,请用宏定义实现之
// 如果宏定义不能实现,请反馈

extern const U08 err_wing[];

#define interface_defrost_wing_err(cm)          (s_get_error(err_wing[cm]))

#define log_mod_fan_comp_group                  fan_comp_group_count
#define val_defrost_humidity_fun_en             FALSE
#define pv_sys_dew                              SENSOR_NOEXIST       
#define val_defrost_enter_out			        (val_defrost_end_out+60)  	/* 进除霜水温 */
#define val_defrost_end_diff                    20
#define val_cm_on_time                          COMP_NEED_ON_TIME
#define val_defrost_env_wing_dly                60
#define LOG_USED                                USED

//1 ——————以下内容无特殊情况,应所有程序一致!!!!!
// 如不一致,请说明本文件不一致.

/*-----------------外部变量说明----------------------------*/


/*-----------------外部函数原型说明------------------------*/



/*-----------------内部函数原型说明------------------------*/



/*-----------------全局变量--------------------------------*/




/*-----------------模块级变量------------------------------*/
GSTATIC U16 defrost_cnt_defrost[MAX_COMP];       // 除霜时间计时 
GSTATIC U16 defrost_cnt_frost[MAX_COMP];         // 积霜时间计时
GSTATIC U16 defrost_cnt_defrost_end[MAX_COMP];   // 除霜后时间计时
GSTATIC U08 defrost_cnt_wing_confirm[MAX_COMP];  // 积霜翅温确认计时
GSTATIC U08 defrost_cnt_out_temp_low[MAX_COMP];  // 除霜出水温度低计时

GSTATIC U16 defrost_cnt_wing_temp_high[MAX_COMP]; // 除霜翅片温度高计时

//1 环翅差记录
#define DEFROST_REC_ENV_WING_DIFF_INTV      30      /* 记录环翅差周期 */
#define DEFROST_REC_ENV_WING_DIFF_NUM       10      /* 记录环翅差个数 */
#define DEFROST_REC_ENV_WING_DIFF_STAB_TIME (DEFROST_REC_ENV_WING_DIFF_INTV*DEFROST_REC_ENV_WING_DIFF_NUM)
GSTATIC U08 defrost_cnt_rec_EWD;          /* 环翅差记录计时 */
GSTATIC I16 defrost_rec_EWD[MAX_COMP][DEFROST_REC_ENV_WING_DIFF_NUM];/* 环翅差记录数据 */
GSTATIC I16 defrost_rec_EWD_avg[MAX_COMP];     /* 平均环翅差 */
#define DEFROST_REC_EWD_AVG_INTV    30      /* 记录平均环翅差均值 */
#define DEFROST_REC_EWD_AVG_NUM     10      /* 记录平均环翅差个数 */
GSTATIC U08 defrost_cnt_rec_EWD_avg;          /* 平均环翅差记录计时 */
GSTATIC I16 defrost_rec_EWD_usedavg[MAX_COMP][DEFROST_REC_ENV_WING_DIFF_NUM];/* 环翅差有效均值记录数据 */
GSTATIC I16 defrost_rec_EWD_usedavg_avg[MAX_COMP]; /* 平均环翅差有效均值 */
GSTATIC I16 defrost_temp_EWD_nofrost[MAX_COMP];/* 无霜运行环翅差 */
GSTATIC I16 defrost_temp_EWD_nofrost_env[MAX_COMP];/* 无霜运行环翅差标定时环温 */
GSTATIC U16 defrost_cnt_env_wing_rise_diff_1[MAX_COMP];  /* 环翅差升差1时间 */
GSTATIC U16 defrost_cnt_env_wing_rise_diff_2[MAX_COMP];  /* 环翅差升差2时间 */
GSTATIC I16 defrost_temp_EWD_stop[MAX_COMP];   /* 停机环翅差 */

#define DEFROST_REC_EWD_START_TIME (7*60 + DEFROST_REC_ENV_WING_DIFF_STAB_TIME) /* 无霜运行环翅差标定起始时间 */
#define DEFROST_REC_EWD_END_TIME (15*60 + DEFROST_REC_ENV_WING_DIFF_STAB_TIME) /* 无霜运行环翅差标定结束时间 */

U16 defrost_power_dly[MAX_COMP];/* 能衰除霜消抖 */

//1 紧急除霜
#ifndef USE_QUICK_TIME
	#define DEFROST_EMERGENCY_TIME          120     // 120分钟内
	#define DEFROST_EMERGENCY_MAX_TIMES     2       // 发生第3次报警
#else
	#define DEFROST_EMERGENCY_TIME          5     	// n分钟内
	#define DEFROST_EMERGENCY_MAX_TIMES     1       // 发生第m+1次报警
#endif /* USE_QUICK_TIME */

U08 defrost_emergency_cnt[MAX_COMP];  /* 紧急除霜次数 */
U08 defrost_cnt_emergency_intv[MAX_COMP][DEFROST_EMERGENCY_MAX_TIMES];/* 紧急除霜间隔计时 */

U16 defrost_emergency_dly[MAX_COMP];    /* 紧急除霜消抖 */
U16 defrost_gas_in_dly[MAX_COMP];       /* 吸气温度除霜消抖 */

/*-----------------常量定义--------------------------------*/



/*-----------------宏定义 ---------------------------------*/

I16 defrost_EWD_except_diff(U08 cm)
{
    return fun_pv_sub(defrost_temp_EWD_nofrost[cm], 
                      defrost_temp_EWD_stop[cm]);
}

void defrost_EWD_except_reset(U08 cm)
{
    defrost_temp_EWD_nofrost[cm] = SENSOR_NOEXIST;
    defrost_temp_EWD_stop[cm] = SENSOR_NOEXIST;
}


I16 defrost_get_value(U08 cm, U08 val_type)
{
    switch (val_type)
    {
        case 0: return SENSOR_NOEXIST;                      /* 无 */
        case 1: return SENSOR_NOEXIST;                      /* 无 */
        case 2: return SENSOR_NOEXIST;                      /* 无 */
        case 3: return defrost_rec_EWD_avg[cm];             /* 平均环翅差 */
        case 4: return defrost_rec_EWD_usedavg_avg[cm];     /* 平均环翅差均值 */
        case 5: return defrost_temp_EWD_nofrost[cm];        /* 无霜运行环翅差 */
        case 6: return defrost_temp_EWD_nofrost_env[cm];    /* 无霜运行环翅差标定时环温 */
        case 7: return defrost_temp_EWD_stop[cm];           /* 停机环翅差 */
		case NUM_EXIT_DEFR_CM_OFF: return defrost_fg[cm].exit_defr_cm_off;		/* 退除霜时压机停机标志 */
    }

    return SENSOR_NOEXIST;
}

/*******************************************************************************
 函 数: defrost_is_allow_auto()
 功 能: 判断是否允许自动除霜
 参 数: 空
 返 回: 
 其 它: 手动调试时禁止自动除霜
*******************************************************************************/
BOL defrost_is_allow_auto(void)
{
    BOL result = TRUE;

    if (val_invt_manual_mode == USED
    ||  val_motor_manual_mode == USED
    ||  val_s_motor_manual_mode == USED
    ||  val_freq_fan_ctrl == FREQ_FAN_CTRL_MANUAL
    ||  ABILITY_TEST_NO_DEFR
    ||  !TF_IS_IDLE
        )
    {
        result = FALSE;
    }

    return result;
}

/*****************************************************************************
函  数 : defrost_fun_get_wing()
功  能 : 选择翅片温度
参  数 : U08 cm : 压机号
返  回 : 无
其  他 : 无
*****************************************************************************/
I16 defrost_fun_get_wing(U08 cm)
{
    return (pv_cell_wing(cm));
}

/*****************************************************************************
函  数 : defrost_fun_get_out_temp()
功  能 : 选择出水温度
参  数 : U08 cm : 
返  回 : 无
其  他 : 无
*****************************************************************************/
I16 defrost_fun_get_out_temp(U08 cm)
{
	I16 temp_low = SENSOR_NOEXIST;

    cm = cm;
	
	temp_low = fun_pv_min(pv_cell_TWin1(pcb_addr), pv_cell_TWout1(pcb_addr));
	
    return (temp_low);
}

/*****************************************************************************
函  数 : defrost_fun_get_env()
功  能 : 选择环境温度
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
I16 defrost_fun_get_env(void)
{
    return pv_cell_TA(pcb_addr);
}

/*******************************************************************************
 函 数: defrost_fun_get_gas_in()
 功 能: 选择吸气温度
 参 数: cm--; 
 返 回: 
 其 它: 空
*******************************************************************************/
I16 defrost_fun_get_gas_in(U08 cm)
{
    return pv_cell_gas_in(cm);
}

/*****************************************************************************
函  数 : defrost_fun_get_interval()
功  能 : 获取除霜周期
参  数 : 无
返  回 : 除霜周期,秒
其  他 : 无
*****************************************************************************/
I16 defrost_fun_get_interval(U08 cm)
{
    static U08 env_area = AREA_1;
    I16 interval;
    I16 temp_env = defrost_fun_get_env();

    env_area = Fun_LookupArea(temp_env, env_area, val_defrost_environ1,
                                                  val_defrost_environ2,
                                                  val_defrost_environ3,
                                                  val_defrost_environ4, 10);
    
    switch(env_area)
    {
        default:        
        case AREA_1: interval = val_defrost_period1; break;
        case AREA_2: interval = val_defrost_period2; break;
        case AREA_3: interval = val_defrost_period3; break;
        case AREA_4: interval = val_defrost_period4; break;
        case AREA_5: interval = val_defrost_period5; break;
    }

    /* 首次除霜 */
    if (defrost_fg[cm].first_defrost == FALSE)
    {
        interval = val_defrost_first_run_period;
    }
    
    return AT_VAL(interval, val_ability_test_interval);		/* 时间进制统一放到参数定义中处理 */
//    return interval*60;
}

/*****************************************************************************
函  数 : defrost_fun_rec_data()
功  能 : 数据记录,包括环翅差温度等
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
void defrost_fun_rec_data(void)
{
    U08 cm;
    I16 temp_env = defrost_fun_get_env(); 
    
    for (cm=0; cm<val_comp_num; cm++)
    {       
        I16 temp_wing = defrost_fun_get_wing(cm);
              
        
        //3 无霜环翅差记录
        if (defrost_fg[cm].need_calib_EWD)
        {   
            BOL calib_EWD_end = FALSE;  // 取值结束
            I16 temp_EWD_nofrost_new = SENSOR_NOEXIST;  // 新的无霜运行环翅差
            
            //4 除霜后7分钟+稳定时间开始标定,取标定8分钟内的最小平均环翅差
            if (defrost_cnt_defrost_end[cm] < DEFROST_REC_EWD_START_TIME)
            {
                ;// nothing
            }
            //4 记录最小环翅差
            else if (defrost_cnt_defrost_end[cm] <= DEFROST_REC_EWD_END_TIME)
            {
                /* 翅片温度<=可除霜翅温 */   /* 环境温度<=可除霜环温 */
                if ((temp_wing <= val_defrost_allow_wing+30)&&(temp_env <= val_defrost_allow_environ+30))
                if (fun_pv_in(defrost_rec_EWD_avg[cm], val_defrost_env_wing_lower, val_defrost_env_wing_upper))
                {
                    //环翅差±0.5内稳定,取值结束
                    if (fun_stab_avg(defrost_rec_EWD[cm], DEFROST_REC_ENV_WING_DIFF_NUM,  -5, 5))
                    {
                        temp_EWD_nofrost_new = defrost_rec_EWD_avg[cm];
                        calib_EWD_end = TRUE;
                    }
                }
            }
            //4 标定结束,统计结果
            else    
            {
                // 取均值的均值
                temp_EWD_nofrost_new = defrost_rec_EWD_usedavg_avg[cm];
                calib_EWD_end = TRUE;
            }
            
            if (calib_EWD_end)
            { 
                // 环温变化
                I16 env_change = fun_pv_sub(temp_env, defrost_temp_EWD_nofrost_env[cm]);
                // 成功
                if (SensorCanUse(temp_EWD_nofrost_new))
                {
                    I16 old_change = fun_pv_sub(temp_EWD_nofrost_new, defrost_temp_EWD_nofrost[cm]);
                    // 若环温变化±1℃内且和旧值相差5℃以上，取小值
                    if (fun_pv_in(env_change , -10, 10) && !fun_pv_in(old_change , -50, 50))
                    {
                        defrost_temp_EWD_nofrost[cm] = fun_pv_min(temp_EWD_nofrost_new, defrost_temp_EWD_nofrost[cm]);
                    }
                    else
                    {
                        defrost_temp_EWD_nofrost[cm] = temp_EWD_nofrost_new;
                        defrost_temp_EWD_nofrost_env[cm] = temp_env;
                    }                    
                }
                // 失败
                else
                {
                    // 标定失败,环温变化±2℃内，保留旧值，否则清掉。
                    if (!fun_pv_in(env_change , -20, 20))
                    {   // 置为故障态
                        defrost_temp_EWD_nofrost[cm] = SENSOR_ERROR;
                    }
                }
                defrost_fg[cm].need_calib_EWD = FALSE;
            }
        }

        //3 停机环翅差记录
        if (S_CM_GetStatus(cm, CM_ON))  // 启动过,就可以标定停机环翅差了
        {
            defrost_fg[cm].need_calib_stop_EWD = TRUE;
        }
        
        if (defrost_fg[cm].need_calib_stop_EWD)
        {
            BOL grp_off_time_enough = TRUE;
            BOL cm_in_grp = FALSE;
            U08 i;
            U08 begin = 0, end = 0;
            log_mod_fan_comp_group(log_mod_cond_type(val_comp_num), &begin, &end, cm);
            #define CALIB_STOP_EWD_TIME     (25*60 + DEFROST_REC_ENV_WING_DIFF_STAB_TIME)
            // 同组停机时间都满足
            for (i=begin; i<end; i++)
            {
                cm_in_grp = TRUE;
                if (cm_time[i].off < CALIB_STOP_EWD_TIME)
                {
                    grp_off_time_enough = FALSE;
                    break;
                }
            }
            
            if (cm_in_grp && grp_off_time_enough
                && power_time >= CALIB_STOP_EWD_TIME)    // 上电时off直接是65535,首次上电时间也有满足
            {
                if (fun_stab_avg(defrost_rec_EWD[cm], DEFROST_REC_ENV_WING_DIFF_NUM, -20, 20)
                    && fun_pv_in(defrost_rec_EWD_avg[cm], -100, 100))
                {
                    // 直接更新
                    defrost_temp_EWD_stop[cm] = defrost_rec_EWD_avg[cm];
                    defrost_fg[cm].need_calib_stop_EWD = FALSE;
                }
            }
        }


   		//3 纽恩泰要求显示“无霜运行环翅差”的标定结果
        if (Cm[cm].run_mode != MODE_COOL)
        	pv_cell_nofrost_EWD(cm) = fun_pv_get(defrost_temp_EWD_nofrost[cm], -999);
//			pv_cell_nofrost_EWD(pcb_addr, cm) = fun_pv_get(defrost_temp_EWD_nofrost[cm], -999);

		//3 压机启动时，清除退除霜时压机停机标志
		if (S_CM_GetStatus(cm, CM_NEED) && S_CM_GetStatus(cm, CM_ON))
		{
		    defrost_fg[cm].exit_defr_cm_off = FALSE;
		}

    }
}

/*****************************************************************************
函  数 : defrost_time()
功  能 : 计时功能
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
void defrost_time(void)
{
    U16 cm;
    U16 k;
    I16 temp_env = defrost_fun_get_env();
    
    if (F_TIME_BASIC_SEC_BACK)
    {
		_CNT(defrost_cm_intv_on);
		_CNT(defrost_cm_intv_off);
		
        for (cm=0; cm<val_comp_num; cm++)
        {
            I16 temp_wing = defrost_fun_get_wing(cm);
            I16 temp_EWD = fun_pv_sub(temp_env, temp_wing); /* 环翅差 */
            I16 temp_out = defrost_fun_get_out_temp(cm);
            
            _CNT_WHEN(defrost_cnt_defrost[cm], S_CM_GetStatus(cm, CM_DEFROST));
            _CNT_WHEN(defrost_cnt_out_temp_low[cm], temp_out <= val_defrost_end_out);
            _CNT_WHEN(defrost_cnt_defrost_end[cm], defrost_fg[cm].defrost_end   \
                                                && S_CM_GetStatus(cm, CM_ON));
			_CNT_WHEN(defrost_cnt_wing_temp_high[cm], temp_wing >= val_exit_dfr_wingT2 \
												&& S_CM_GetStatus(cm, CM_DEFROST));
			_CNT(defrost_cm_time[cm]);
			
            /* 环翅差升稳定计时 */
            {
                /* 环翅差升 */
                I16 temp_EWD_rise = fun_pv_sub(temp_EWD, defrost_temp_EWD_nofrost[cm]);
                _CNT_WHEN(defrost_cnt_env_wing_rise_diff_1[cm], SensorCanUse(temp_EWD_rise)   \
                                                && temp_EWD_rise >= val_defrost_EWD_rise_diff1);
                _CNT_WHEN(defrost_cnt_env_wing_rise_diff_2[cm], SensorCanUse(temp_EWD_rise)   \
                                                && temp_EWD_rise >= val_defrost_EWD_rise_diff2);
                
            }

            /* 积霜时间清零 */
            if(val_defrost_time_sel == DEFRO_BY_FROSTTIME)
            {
                _CNT_WHEN(defrost_cnt_wing_confirm[cm], (temp_wing > 20));
                /* 翅片温度持续60S大于2时除霜累计时间清零 */
                if (defrost_cnt_wing_confirm[cm] > 60)
                {
                    defrost_cnt_frost[cm] = 0;      
                }
            }
            
            /* 正在除霜,积霜时间清零 */
            if (S_CM_GetStatus(cm, CM_DEFROST)) 
            {
                defrost_cnt_frost[cm] = 0;  
            }
            else
            {
                BOL is_frost = FALSE;
                
                /* 按积霜时间 */
                if (val_defrost_time_sel == DEFRO_BY_FROSTTIME)
                {
                    /* 按湿度计时 */
                    if (val_defrost_humidity_fun_en && SensorCanUse(pv_sys_dew))
                    {
                        is_frost = (temp_wing <= pv_sys_dew);
                    }
                    else
                    {
                        is_frost = (temp_wing <= 0 && temp_wing < temp_env);
                    }
                }
                /* 按压机运行时间 */
                else if (val_defrost_time_sel == DEFRO_BY_RUNTIME)
                {
                    is_frost = (S_CM_GetStatus(cm, CM_ON) != 0);
                }
                                                            /* 运行120S后才开始检测翅片温度 */
                _CNT_WITH(defrost_cnt_frost[cm], is_frost && (cm_time[cm].on > 120));
            }     

			//4 紧急除霜消抖计时
			_CNT(defrost_emergency_dly[cm]);

			_CNT(defrost_power_dly[cm]);
        }

        
        /* 记录环翅差数据 */
        _CNT(defrost_cnt_rec_EWD);
        if (defrost_cnt_rec_EWD >= DEFROST_REC_ENV_WING_DIFF_INTV)
        {
            defrost_cnt_rec_EWD = 0;
            for (cm=0; cm<val_comp_num; cm++)
            {
                I16 temp_EWD = temp_env - defrost_fun_get_wing(cm);
                defrost_rec_EWD_avg[cm] = fun_rec_avg(temp_EWD, &defrost_rec_EWD[cm][0], 
                                                    DEFROST_REC_ENV_WING_DIFF_NUM);
            }
        }

        /* 记录有效平均环翅差 */
        _CNT(defrost_cnt_rec_EWD_avg);
        if (defrost_cnt_rec_EWD_avg >= DEFROST_REC_EWD_AVG_INTV)
        {
            defrost_cnt_rec_EWD_avg = 0;
            for (cm=0; cm<val_comp_num; cm++)
            {
                // 环翅差在±2℃温度,记录一次均值
                if ((fun_stab_avg(defrost_rec_EWD[cm], DEFROST_REC_ENV_WING_DIFF_NUM,  -20, 20)))
                {
                    defrost_rec_EWD_usedavg_avg[cm] = fun_rec_avg(defrost_rec_EWD_avg[cm], &defrost_rec_EWD_usedavg[cm][0], 
                                                    DEFROST_REC_EWD_AVG_NUM);
                }
            }
        }
    }

    if (F_TIME_BASIC_MIN_BACK)
    {
        for (cm=0; cm<val_comp_num; cm++)
        {
            //4 紧急除霜间隔计时
            for (k=0; k<DEFROST_EMERGENCY_MAX_TIMES; k++)
            {
                if (defrost_emergency_cnt[cm] > k)  // 发生过,才开始计时
                { 
                    _CNT(defrost_cnt_emergency_intv[cm][k]);
                }
            }

            _CNT(defrost_gas_in_dly[cm]);
        }
    }
} 

/*****************************************************************************
函  数 : defrost_frost_by_env_wing_rise()
功  能 : 环翅差升积霜
参  数 : U08 cm : 
返  回 : 无
其  他 : 无
*****************************************************************************/
U08 defrost_frost_by_env_wing_rise(U08 cm)
{
    U08 by_env_wing_rise = DEFROST_STA_NULL;    // 初始化为无效
    
    // 判断是否满足积霜检测条件
    if (   val_defrost_env_wing_rise_en                         /* 环翅差升除霜使用 */
        && SensorCanUse(defrost_temp_EWD_nofrost[cm]) /* 稳定环翅差温度 */
        // 没有除霜才判断，加这个条件是为了防止满足重度除霜时，除霜间隔是大于DEFROST_REC_EWD_END_TIME，但无霜环翅差还是会被意外清掉
		&& !S_CM_GetStatus(cm, CM_DEFROST)
	) 
    {
        by_env_wing_rise = DEFROST_STA_NO_FROST;
        
        // 判断是否需要除霜

        //4 当前运行环翅差（环境温度-翅片温度）-"无霜运行环翅差"≥+4℃【环翅升差1】持续【温度确认时间】且累计积霜运行时间≥除霜间隔
        if (defrost_cnt_env_wing_rise_diff_1[cm] > val_defrost_temp_sure_time
            && defrost_cnt_frost[cm] > defrost_fun_get_interval(cm)
            &&(defrost_cnt_frost[cm] > val_defrost_min_time))  /* 积霜时间＞【最小除霜间隔】  */
        {
            by_env_wing_rise = DEFROST_STA_BY_ENV_WING;
        }
        
        //4 当前运行环翅差（环境温度-翅片温度）-"无霜运行环翅差"≥+6℃【环翅升差2】持续【温度确认时间】
        if (defrost_cnt_env_wing_rise_diff_2[cm] > val_defrost_temp_sure_time
		    &&(defrost_cnt_frost[cm] > val_defrost_min_time)
		)
        {
            by_env_wing_rise = DEFROST_STA_BY_ENV_WING;
        }
    }

	/* 实际除霜间隔小于标定"无霜环翅差"所需时间则认为标定偏小,清空标定值,置为故障态 */
	if (by_env_wing_rise == DEFROST_STA_BY_ENV_WING && defrost_cnt_frost[cm]<=DEFROST_REC_EWD_END_TIME)
	{
	    defrost_temp_EWD_nofrost[cm] = SENSOR_ERROR;
	}
    
    // 返回积霜状态
    return by_env_wing_rise;
}

/*****************************************************************************
函  数 : defrost_frost_by_env_wing_diff()
功  能 : 环翅差积霜
参  数 : U08 cm : 
返  回 : 无
其  他 : 无
*****************************************************************************/
U08 defrost_frost_by_env_wing_diff(U08 cm)
{
    U08 by_EWD = DEFROST_STA_NULL;
    I16 temp_wing = defrost_fun_get_wing(cm);
    I16 temp_env = defrost_fun_get_env();
    I16 temp_EWD = temp_env - temp_wing;
    I16 defrost_EWD = val_defrost_allow_diff1;
    
    // 判断是否满足积霜检测条件
    if (TRUE)
    {
        static U08 env_area = AREA_1;
        env_area = Fun_LookupArea(temp_env, env_area, val_defrost_environ1,
                                                      val_defrost_environ2,
                                                      val_defrost_environ3,
                                                      val_defrost_environ4, 10);
        // 计算除霜环翅差
        switch(env_area)
        {
            default:        
            case AREA_1: defrost_EWD = val_defrost_allow_diff1; break;
            case AREA_2: defrost_EWD = val_defrost_allow_diff2; break;
            case AREA_3: defrost_EWD = val_defrost_allow_diff3; break;
            case AREA_4: defrost_EWD = val_defrost_allow_diff4; break;
            case AREA_5: defrost_EWD = val_defrost_allow_diff5; break;
        }
        
        // 判断是否需要除霜

        if (   temp_EWD >= AT_VAL(defrost_EWD, val_ability_test_EWD)        // 环境温度－翅片温度≥除霜环翅差
            && defrost_cnt_frost[cm] > defrost_fun_get_interval(cm) // 积霜周期到
            && (defrost_cnt_frost[cm] > val_defrost_min_time)  /* 积霜时间＞【最小除霜间隔】  */
           )
        {
            by_EWD = DEFROST_STA_BY_WING;
        }
    }
    
    // 返回积霜状态
    return by_EWD;
}

/*****************************************************************************
函  数 : defrost_by_frost(
功  能 : 正常积霜除霜
参  数 : U08 cm : 
返  回 : 无
其  他 : 无
*****************************************************************************/
U08 defrost_by_frost(U08 cm)
{
    U08 by_frost = DEFROST_STA_NO_FROST;
    I16 temp_wing = defrost_fun_get_wing(cm);
    I16 temp_out = defrost_fun_get_out_temp(cm);
    I16 temp_env = defrost_fun_get_env();

    // 判断是否需要积霜
    if (  (cm_time[cm].on > val_defrost_cm_on_time)  /* 压机运行时间>除霜开机时间 */
        &&(temp_out > val_defrost_enter_out)              
        &&(temp_wing <= val_defrost_allow_wing)         /* 翅片温度<=可除霜翅温 */
        &&(temp_env <= val_defrost_allow_environ)       /* 环境温度<=可除霜环温 */
        )
    {
        U08 by_humidity = DEFROST_STA_NULL;
        U08 by_env_wing_rise = DEFROST_STA_NULL;
        U08 by_env_wing_diff = DEFROST_STA_NULL;

        //4 每种积霜都执行,根据结果选择
//      // 湿度积霜
//      by_humidity = defrost_frost_by_humidity(cm);
        
        // 环翅差升积霜
        by_env_wing_rise = defrost_frost_by_env_wing_rise(cm);
        
        // 环翅差积霜
        by_env_wing_diff = defrost_frost_by_env_wing_diff(cm);

        // 默认湿度积霜
        by_frost = by_humidity;
        // 湿度积霜不可用时,环翅差升积霜
        if (by_frost == DEFROST_STA_NULL)
        {
            by_frost = by_env_wing_rise;
        }
        // 环翅差升积霜不可用时,使用环翅差积霜
        if (by_frost == DEFROST_STA_NULL)
        {
            by_frost = by_env_wing_diff;
        }

        if (ABILITY_TEST_HAS_DEFR)
        {
            by_frost = by_env_wing_diff;
        }
    }


    // 返回除霜结果
    return by_frost;
}

/*****************************************************************************
函  数 : defrost_by_lowpress()
功  能 : 低压除霜
参  数 : U08 cm : 
返  回 : 返回除霜状态
其  他 : 无
*****************************************************************************/
U08 defrost_by_lowpress(U08 cm)
{
    U08 by_lowpress = DEFROST_STA_NO_FROST;
    I16 temp_wing = defrost_fun_get_wing(cm);
    I16 temp_out = defrost_fun_get_out_temp(cm);
    I16 temp_env = defrost_fun_get_env();
    BOL allow_lp_into = FALSE;

	#define LOWPRESS_DEFROST_ALLOW_ENVIR		(-5*10)
    
    // 判断是否满足低压除霜条件
    if (  (val_defrost_LP == LOG_USED)              /*  */
		&&(defrost_cnt_frost[cm] > val_defrost_min_time)/* 积霜时间＞【最小除霜间隔】  */
        &&(cm_time[cm].on >= val_defrost_lp_dly) /*  */
        &&(defrost_cnt_frost[cm] > val_defrost_lp_time)    /*  */
        &&(temp_wing < val_defrost_lp_wing)         /*  */
        &&(temp_out > val_defrost_enter_out)
        &&(temp_env <= LOWPRESS_DEFROST_ALLOW_ENVIR)
        )
    {
        allow_lp_into = TRUE;
    }
    
    // 处理低压信号量
    if (SIGNAL_Accept(&defrost_signal_into_lowpress[cm], allow_lp_into))
    {
        by_lowpress = DEFROST_STA_BY_LOWPRESS;
    }


    // 返回除霜结果
    return by_lowpress;
}

/*****************************************************************************
函  数 : defrost_by_emergency()
功  能 : 紧急除霜
参  数 : U08 cm : 
返  回 : 返回除霜状态
其  他 : 无
*****************************************************************************/
U08 defrost_by_emergency(U08 cm)
{
    U08 by_emergency = DEFROST_STA_NO_FROST;
    I16 temp_wing = defrost_fun_get_wing(cm);
    I16 temp_out = defrost_fun_get_out_temp(cm);
    I16 temp_env = defrost_fun_get_env();
    I16 temp_EWD = fun_pv_sub(temp_env, temp_wing);

    if (temp_EWD <= val_defrost_emergency_diff)
	{
	    defrost_emergency_dly[cm] = 0;
	}
    
    // 判断是否满足紧急除霜条件
    if (  (cm_time[cm].on > val_defrost_cm_on_time)  /* 压机运行时间>除霜开机时间 */
        &&(defrost_cnt_frost[cm] > val_defrost_min_time)/* 积霜时间＞【最小除霜间隔】  */
        &&(defrost_cnt_frost[cm] > val_defrost_emergency_time)/* 积霜时间＞【紧急除霜间隔】  */
        &&(temp_out > val_defrost_enter_out)   
        &&(temp_wing <= val_defrost_allow_wing)         /* 翅片温度<=可除霜翅温 */
        )
    {
        /* 环翅差（环境温度－翅片温度）＞18℃【强除霜环翅差】 */
		
        if (defrost_emergency_dly[cm] > val_defrost_env_wing_dly)
        {
            BOL allow = TRUE;
            if (defrost_emergency_cnt[cm] >= DEFROST_EMERGENCY_MAX_TIMES // 第3次发生
                && !S_CM_GetStatus(cm, CM_DEFROST)) // 每除霜时才判断,应对带入除霜情况
            {
                // 第k轮发生间隔
                U16 k = defrost_emergency_cnt[cm]%DEFROST_EMERGENCY_MAX_TIMES;
                if (defrost_cnt_emergency_intv[cm][k] <= DEFROST_EMERGENCY_TIME)
                {
                    allow = FALSE;
                }
            }

            if (allow)
            {
                by_emergency = DEFROST_STA_BY_EMERGENCY;
            }
            else
            {   /* 紧急除霜频繁 */
                defrost_fg[cm].emergency_defrost_frequent = TRUE; /* 报相关故障 */
				defrost_emergency_cnt[cm] = 0;/* 每三次报一次警：紧急除霜计数清零 */
			
            }
        }
    }


    // 返回除霜结果
    return by_emergency;
}

/*******************************************************************************
 函 数: defrost_cal_power()
 功 能: 根据水温差计算机组能力
 参 数: water_diff--水温差; 
 返 回: 机组能力
 其 它: 空
*******************************************************************************/
I32 defrost_cal_power(I32 water_diff)
{
    I32 result = 0;
    I16 flow = val_ability_test_flow;

    if (SensorCanUse(PUMPf_flow_pv)
    &&  USE_PUMPf_CTRL != PUMPF_CTRL_PWR_ON_ONLY
    )
    {
        flow = _DIVUROUND(PUMPf_flow_pv, 100);			/* 1.0kL/h */
    }

    result = _DIVUROUND(water_diff*flow*4186, 3600);    /* 0.01kw */
    
    return result;
}

/*****************************************************************************
函  数 : defrost_by_power()
功  能 : 能力衰减除霜
参  数 : U08 cm : 
返  回 : 返回除霜状态
其  他 : 无
*****************************************************************************/
U08 defrost_by_power(U08 cm)
{
    U08 by_power = DEFROST_STA_NO_FROST;
    I16 temp_wing = defrost_fun_get_wing(cm);
    I16 temp_out = defrost_fun_get_out_temp(cm);
    I16 power_diff = fun_pv_sub(pv_cell_mac_power(pcb_addr), pv_cell_mac_power_now(pcb_addr));

    if (val_ability_test_power == 0
    ||  power_diff <= val_ability_test_power)
	{
	    defrost_power_dly[cm] = 0;
	}
    
    // 判断是否满足紧急除霜条件
    if (  (cm_time[cm].on > val_defrost_cm_on_time)  /* 压机运行时间>除霜开机时间 */
//        &&(defrost_cnt_frost[cm] > val_defrost_cm_on_time)/* 积霜时间＞【除霜开机时间】  */
        &&(temp_out > val_defrost_enter_out)   
        &&(temp_wing <= val_defrost_allow_wing)         /* 翅片温度<=可除霜翅温 */
        && ABILITY_TEST_HAS_DEFR
        )
    {
        if (defrost_power_dly[cm] > 60)
        {
            by_power = DEFROST_STA_BY_POWER;
        }
    }


    // 返回除霜结果
    return by_power;
}

/*******************************************************************************
 函 数: defrost_by_longtime()
 功 能: 定时除霜，当环温低时，且长时间未除霜，直接执行除霜
 参 数: cm--压机序号; 
 返 回: 是否进入除霜
 其 它: 空
*******************************************************************************/
U08 defrost_by_longtime(U08 cm)
{ 
    I16 temp_wing = defrost_fun_get_wing(cm);
    I16 temp_gas_in = defrost_fun_get_gas_in(cm);
    I16 temp_allow = fun_pv_min(temp_wing, temp_gas_in);
    I16 temp_out = defrost_fun_get_out_temp(cm);
    U08 allow_longtime_into = DEFROST_STA_NO_FROST; 
    U16 defrost_longtime;	/* 定时除霜间隔 */

#if defined(M_DISEN)
	if (pv_cell_TWin1(pcb_addr) < val_timingdfr_temp_in1)
	{
	    defrost_longtime = val_defrost_longtime1;
    }
	else if (pv_cell_TWin1(pcb_addr) < val_timingdfr_temp_in2)
	{
	    defrost_longtime = val_defrost_longtime2;
    }
	else
	{
	    defrost_longtime = val_defrost_longtime3;
    }
#else
    defrost_longtime = val_defrost_longtime;
#endif

	if (defrost_longtime == 0   // 间隔为0表示不用
    ||  fg_ability_test_run				/* 性能测试不进入 */
    )
	{
		allow_longtime_into = DEFROST_STA_NO_FROST;
	}
	else
	{
		// 低环温且长时间未除霜
	    if ((defrost_cnt_frost[cm] >= defrost_longtime)   
	    &&  (defrost_cnt_frost[cm] > val_defrost_min_time) /* 积霜时间＞【最小除霜间隔】  */
	    &&	(temp_allow < val_defrost_allow_wing)
	    &&  (temp_out > val_defrost_enter_out)
	    )
	    {
	        allow_longtime_into = DEFROST_STA_BY_LONGTIME;
	    }
	} 
	
    // 返回除霜结果
    return allow_longtime_into;
}

#if 0
/*******************************************************************************
 函 数: defrost_by_gas_in()
 功 能: 吸气温度除霜
 参 数: cm--; 
 返 回: 
 其 它: 防止翅片温度异常的情况
*******************************************************************************/
U08 defrost_by_gas_in(U08 cm)
{ 
    U08 by_gas_in = DEFROST_STA_NO_FROST;
    I16 temp_out = defrost_fun_get_out_temp(cm);
    I16 temp_env = defrost_fun_get_env();
    I16 temp_gas_in = defrost_fun_get_gas_in(cm);
    I16 temp_EGD = fun_pv_sub(temp_env, temp_gas_in);

#ifndef USE_QUICK_TIME
    #define val_defrost_allow_gas_in        (-10*10)		/* 允许除霜吸气温度 */
    #define val_defrost_allow_EGD		    (25*10)		    /* 允许除霜环吸差 */
    #define val_defrost_by_gas_in_dly	    (30)		    /* 确认延时：min */
#else
    #define val_defrost_allow_gas_in        (-10*10)		/* 允许除霜吸气温度 */
    #define val_defrost_allow_EGD		    (25*10)		    /* 允许除霜环吸差 */
    #define val_defrost_by_gas_in_dly	    (2)		        /* 确认延时：min */
#endif

	if (temp_EGD < val_defrost_allow_EGD
    ||  temp_gas_in >= val_defrost_allow_gas_in
        )
	{
	    defrost_gas_in_dly[cm] = 0;
	}

	// 判断是否满足吸气温度除霜条件
    if (  (cm_time[cm].on > val_defrost_cm_on_time)  /* 压机运行时间>除霜开机时间 */
        &&(defrost_cnt_frost[cm] > val_defrost_cm_on_time)/* 积霜时间＞【除霜开机时间】  */
        &&(temp_out > val_defrost_enter_out)              
        )
    {
        
        if (defrost_gas_in_dly[cm] > val_defrost_by_gas_in_dly)
        {
            by_gas_in = DEFROST_STA_BY_GAS_IN;
        }
    }
	
    // 返回除霜结果
    return by_gas_in;
}
#endif

/*****************************************************************************
函  数 : defrost_by_manual()
功  能 : 手动除霜
参  数 : U08 cm : 
返  回 : 返回除霜状态
其  他 : 无
*****************************************************************************/
U08 defrost_by_manual(U08 cm)
{
    U08 by_manual = DEFROST_STA_NO_FROST;
    I16 temp_wing = defrost_fun_get_wing(cm);
    I16 temp_out = defrost_fun_get_out_temp(cm);
    BOL allow_manual = FALSE;
    
    // 判断是否允许手动除霜条件
    if (temp_wing < AT_VAL(val_defrost_end_wing, val_ability_test_wing))
    {
        // 判断手动除霜命令
        allow_manual = TRUE;
    }

    // 接受手动除霜命令,并记忆
    if (SIGNAL_Accept(&defrost_signal_into_namual[cm], allow_manual))
    {
        S_CM_SetStatus(cm, CM_FORCE_DEFROST);
    }

    // 达到条件才进入手动除霜
    if (S_CM_GetStatus(cm, CM_FORCE_DEFROST)
        &&(temp_out > val_defrost_enter_out)
        )
    {
        by_manual = DEFROST_STA_BY_MANUAL;
    }

    // 返回除霜结果
    return by_manual;
}

/*****************************************************************************
函  数 : defrost_grp_into_apply()
功  能 : 申请除霜,处理带入压机
参  数 : U08 cm_bgn    : 组起始压机号
         U08 cm_end    : 组结束压机号
         U08 cm        : 当前压机号
         BOL into_need : 是否需要进入除霜
返  回 : 无
其  他 : 无
*****************************************************************************/
void defrost_grp_into_apply(U08 cm_bgn, U08 cm_end, U08 cm, BOL into_need)
{
    U08 i;
    BOL fg_wait = FALSE;
    
    if ((!S_CM_GetStatus(cm, CM_NEED_DEFROST)) 
        && (!S_CM_GetStatus(cm, CM_DEFROST))
        && into_need
        )
    {
        //3 判断是否需要等待其他压机
        for(i=cm_bgn; i<cm_end; i++)
        {
            if (i == cm)        continue;
                
            if (S_CM_GetStatus(i, CM_NEED_DEFROST)                  /* 同组已有压机有除霜需求 */
                 || S_CM_GetStatus(i, CM_ON) && cm_time[i].on < val_cm_on_time)     /* 开启但运行未满设定时间 */
            {
                fg_wait = TRUE;
                break;
            }
        }
    

        //3 不用等待其他压机时,申请除霜
        if (!fg_wait)
        {
            S_CM_SetStatus(cm, CM_NEED_DEFROST);
            S_CM_ClrStatus(cm, CM_FORCE_DEFROST);
            defrost_fg[cm].first_defrost = TRUE;
            
            defrost_fg[cm].defrost_apply = TRUE;
                
            for (i=cm_bgn; i<cm_end; i++)
            {
                I16 temp_wing = defrost_fun_get_wing(i);
                I16 temp_out = defrost_fun_get_out_temp(i);
                I16 temp_gas_in = defrost_fun_get_gas_in(i);
                I16 temp_allow = fun_pv_min(temp_wing, temp_gas_in);
                
                if (i == cm)        continue;

                //4 其他被带入除霜的压机
                if(S_CM_GetStatus(i, CM_ON))  /*  */
                {
                   if ((temp_allow < val_defrost_allow_wing   			/* 翅片或吸气温度允许除霜或([首次除霜]且[翅温＜退除霜翅温-2℃]) */  
                        ||( val_defrost_env_wing_rise_en
                            && defrost_fg[i].first_defrost == FALSE 
                            && temp_wing < AT_VAL(val_defrost_end_wing, val_ability_test_wing) - 20))
                      && SensorCanUse(temp_wing)            			/* 翅片温度探头无故障 */
                      && temp_out > val_defrost_enter_out)    			
                   {
                        S_CM_SetStatus(i, CM_NEED_DEFROST);
                        S_CM_ClrStatus(i, CM_FORCE_DEFROST);
                        defrost_fg[i].first_defrost = TRUE;
                        defrost_fg[i].defrost_apply = TRUE;
                   }
                }
                else         /* 防止压机被选上但是还未开启的情况 */
                {
                    S_CM_ClrStatus(i, CM_NEED);
                }
            }
        }
    }
}
/*****************************************************************************
函  数 : defrost_grp_into_act()
功  能 : 一台压机进入除霜时的动作,包括带入除霜
参  数 : U08 cm_bgn    : 组起始压机号
         U08 cm_end    : 组结束压机号
         U08 cm        : 当前压机号
         BOL into_must : 是否必须进入
返  回 : 无
其  他 : S_CM_SetStatus(cm, CM_DEFROST);
*****************************************************************************/
void defrost_grp_into_act(U08 cm_bgn, U08 cm_end, U08 cm, BOL into_must)
{
    U08 i;

    if (into_must)
    {
        S_CM_SetStatus(cm, CM_NEED_DEFROST);
        S_CM_SetStatus(cm, CM_DEFROST_ALLOW);
    }
    
    //3 有除霜需求,且允许除霜时,进入除霜
    if (S_CM_GetStatus(cm, CM_NEED_DEFROST) && S_CM_GetStatus(cm, CM_DEFROST_ALLOW))
    {
        S_CM_SetStatus(cm, CM_DEFROST);


        //4 其他无除霜需求的,关机
        for (i=cm_bgn; i<cm_end; i++)
        {
            if (i == cm)        continue;
                
            if (S_CM_GetStatus(i, CM_NEED_DEFROST))  /* 相邻压机的处理 */
            {
                S_CM_SetStatus(i, CM_DEFROST);
                S_CM_SetStatus(i, CM_DEFROST_ALLOW);
            }
            else 
            {
                S_CM_ClrStatus(i, CM_NEED);
            }
        }
    }
}

/*****************************************************************************
函  数 : defrost_grp_into()
功  能 : 一组共用风机的压机压缩进入除霜条件判断
参  数 : U08 cm_bgn    : 组起始压机号
         U08 cm_end    : 组结束压机号
         U08 cm        : 当前压机号
返  回 : 无
其  他 : 无
*****************************************************************************/
void defrost_grp_into(U08 cm_bgn, U08 cm_end, U08 cm)
{
    U08 into_by_frost = DEFROST_STA_NO_FROST;
    U08 into_by_lowpress = DEFROST_STA_NO_FROST;
    U08 into_by_emergency = DEFROST_STA_NO_FROST;
    U08 into_by_manual = DEFROST_STA_NO_FROST;
	U08 into_by_longtime = DEFROST_STA_NO_FROST;
    U08 into_by_gas_in = DEFROST_STA_NO_FROST;
    U08 into_by_power = DEFROST_STA_NO_FROST;
    BOL into_need = FALSE;
    BOL into_must = FALSE;
//  static BOL is_defrost[MAX_COMP] = {COMP_INIT(FALSE)};
    BOL is_defrost_bgn = FALSE;
    BOL is_defrost_end = FALSE;

    is_defrost_bgn = (S_CM_GetStatus(cm, CM_DEFROST) != 0);
    
    // 处理每个除霜条件,一定要处理
    into_by_frost = defrost_by_frost(cm);
    into_by_lowpress = defrost_by_lowpress(cm);
    into_by_emergency = defrost_by_emergency(cm);
    into_by_manual = defrost_by_manual(cm);
	into_by_longtime = defrost_by_longtime(cm);
	into_by_power = defrost_by_power(cm);
//    into_by_gas_in = defrost_by_gas_in(cm);

    
    // 仲裁除霜结果
    if (defrost_is_allow_auto())			/* 因“处理每个除霜条件,一定要处理”，故放到结果处判断 */
    {
        if (  DEFROST_STA_IS_NEED(into_by_frost)
            ||DEFROST_STA_IS_NEED(into_by_lowpress)
            ||DEFROST_STA_IS_NEED(into_by_emergency)
            ||DEFROST_STA_IS_NEED(into_by_manual)
            ||DEFROST_STA_IS_NEED(into_by_longtime)
            ||DEFROST_STA_IS_NEED(into_by_gas_in)
            ||DEFROST_STA_IS_NEED(into_by_power)
            )
        {
            into_need = TRUE;
        }
    }
	else
	{
        if (DEFROST_STA_IS_NEED(into_by_manual)
        ||  DEFROST_STA_IS_NEED(into_by_power)
           )
        {
            into_need = TRUE;
        }		
	}

    if (DEFROST_STA_IS_NEED(into_by_lowpress))
    {
        into_must = TRUE;
    }

    // 申请除霜,处理带入除霜
    defrost_grp_into_apply(cm_bgn, cm_end, cm, into_need);
    
    // 进入除霜,处理带入除霜
    defrost_grp_into_act(cm_bgn, cm_end, cm, into_must);

    // 发生除霜时刻,可能是异步的,进除霜条件一定要处理
    is_defrost_end = (S_CM_GetStatus(cm, CM_DEFROST) != 0);
    if (!is_defrost_bgn && is_defrost_end)
    {
        if (DEFROST_STA_IS_NEED(into_by_emergency))
        {
            // 第k轮发生间隔从新计时
            U16 k = defrost_emergency_cnt[cm]%DEFROST_EMERGENCY_MAX_TIMES;
            defrost_cnt_emergency_intv[cm][k] = 0;
            // 前一个循环是首次发生,后面是再次发生
            if (++defrost_emergency_cnt[cm] >= 2*DEFROST_EMERGENCY_MAX_TIMES) 
                defrost_emergency_cnt[cm] = DEFROST_EMERGENCY_MAX_TIMES;
        }
    }
}

/*****************************************************************************
函  数 : defrost_grp_exit_act()
功  能 : 一台压机退出除霜时的动作,包括独立退出
参  数 : U08 cm_bgn    : 组起始压机号
         U08 cm_end    : 组结束压机号
         U08 cm        : 当前压机号
         U08 attrib    : 退出类型
返  回 : 无
其  他 : 无
*****************************************************************************/
void defrost_grp_exit_act(U08 cm_bgn, U08 cm_end, U08 cm, U08 attrib)
{
    U08 i;

    // 根据不同的退出类型退出除霜    
    if (attrib == DEFRO_EXIT_SIN)
    {   
        S_CM_ClrStatus(cm, CM_NEED_DEFROST);
        S_CM_ClrStatus(cm, CM_FORCE_DEFROST);
        defrost_fg[cm].defrost_apply = FALSE;
        // 单独退出除霜代表"单独"和"停机" 
        S_CM_ClrStatus(cm, CM_NEED);
		defrost_fg[cm].defrost_end = TRUE;      /* 已除过霜 */
        defrost_fg[cm].need_calib_EWD = TRUE;
		defrost_fg[cm].exit_defr_cm_off = TRUE;
    }
    else 
    {
        for (i=cm_bgn; i<cm_end; i++)
        {
            S_CM_ClrStatus(i, CM_NEED_DEFROST);
            S_CM_ClrStatus(i, CM_FORCE_DEFROST);
            defrost_fg[i].defrost_apply = FALSE;
			defrost_fg[i].defrost_end = TRUE;      /* 已除过霜 */
            defrost_fg[i].need_calib_EWD = TRUE;
        }
    }
    
}

#if defined(M_DISEN)
#define val_defrost_end_wing_acin               (pv_cell_TWin1(pcb_addr)<= 250 ? 180 \
                                                                               : (pv_cell_TWin1(pcb_addr) <= 350 ? 150 \
                                                                                                                 : 120))
/*****************************************************************************
函  数 : defrost_grp_exit()
功  能 : 一组共用风机的压机压缩退出除霜条件判断
参  数 : U08 cm_bgn    : 组起始压机号
         U08 cm_end    : 组结束压机号
         U08 cm        : 当前压机号
返  回 : 无
其  他 : S_CM_ClrStatus(cm, CM_NEED_DEFROST);
*****************************************************************************/
void defrost_grp_exit(U08 cm_bgn, U08 cm_end, U08 cm)
{
    U08 i;
    U08 attr = 0xFF;
    BOL grp_exit_frost_time = FALSE;    // 除霜时间到,退组
    BOL grp_exit_out_temp = FALSE;      // 出水温度低,退组
    BOL grp_exit_wing_temp = FALSE;     // 所有除霜需求的翅片温度高,退组
    BOL grp_exit_ext_temp = FALSE;      // 所有除霜需求的排气温度高,退组
    BOL grp_exit_wing_temp_cnt = FALSE;     // 所有除霜需求的翅片温度大于3℃超过150s,退组
    BOL has_defrost_cm = FALSE;            // 有除霜需求的机组
    BOL evp_independ = (val_evap_type < log_mod_cond_type(val_comp_num)); // 蒸发器独立??
    I16 temp_out = defrost_fun_get_out_temp(cm);
    
    // 判断退出除霜条件
    if (S_CM_GetStatus(cm, CM_NEED_DEFROST))
    {
        //3 判断是否同组退出除霜
        // 除霜时间到
        if (defrost_cnt_defrost[cm] >= AT_VAL(val_defrost_time, val_ability_test_defr_time))  
        {
            grp_exit_frost_time = TRUE;
        }
        // 任何出水温度低
        if (defrost_cnt_out_temp_low[cm] > 10 
        &&  temp_out<=val_defrost_end_out	/* 防止出水温度拉升过程出现误判 */
        &&  !evp_independ)
        {
            grp_exit_out_temp = TRUE;
        }
        // 所有除霜需求的的翅片温度高/排气都高/电流都高
        for (i=cm_bgn, grp_exit_wing_temp=TRUE, grp_exit_ext_temp=TRUE, grp_exit_wing_temp_cnt=TRUE; i<cm_end; i++) // 初始化为退出
        {
            if (S_CM_GetStatus(i, CM_NEED_DEFROST))
            {
                has_defrost_cm = TRUE;
                // 翅片高
                if (defrost_fun_get_wing(i) < AT_VAL(val_defrost_end_wing_acin, val_ability_test_wing))
                {
                    grp_exit_wing_temp = FALSE;
                }
                // 排气高
                if (pv_cell_gas_out(i) < 950 || defrost_cnt_defrost[i] < 120)
                {
                    grp_exit_ext_temp = FALSE;
                }	
				// 翅温到达计时
		        if (defrost_cnt_wing_temp_high[i] <= val_exit_dfr_wingT2_time 
		        ||  defrost_fun_get_wing(i) < val_exit_dfr_wingT2	
				)
		        {
		            grp_exit_wing_temp_cnt = FALSE;
		        }
            }
        }
        grp_exit_wing_temp = (grp_exit_wing_temp && has_defrost_cm);
		grp_exit_ext_temp = (grp_exit_ext_temp && has_defrost_cm);
//		grp_exit_wing_temp_cnt = (grp_exit_wing_temp_cnt && has_defrost_cm);
        grp_exit_wing_temp_cnt = 0;			/* 2022-11-26迪森要求删除此条件，暂清零处理，防止客户反悔又要添加 */
		
        //4 仲裁组退出
        if (grp_exit_frost_time || grp_exit_out_temp || grp_exit_wing_temp || grp_exit_ext_temp || grp_exit_wing_temp_cnt)
        {
            attr = DEFRO_EXIT_ALL;
        }

        //3 判断是否单个退出
        if (attr == 0xFF)
        {
            BOL signal_exit = FALSE;
            BOL other_defrost = FALSE;
            // 出水温度低 或 翅片温度高
            if (  (defrost_cnt_out_temp_low[cm] > 10 && evp_independ)
                ||(defrost_fun_get_wing(cm) >= AT_VAL(val_defrost_end_wing_acin, val_ability_test_wing))
                )
            {
                signal_exit = TRUE;
            }

			// 翅温到达计时
		    if (defrost_cnt_wing_temp_high[i] > val_exit_dfr_wingT2_time
		    &&  defrost_fun_get_wing(i) > val_exit_dfr_wingT2	
			)
		    {
		        signal_exit = TRUE;
		    }	

            // 排气温度高
            if (pv_cell_gas_out(cm) >= 950 && defrost_cnt_defrost[cm] >= 120)
            {
                signal_exit = TRUE;
            }

            // 有其他压机在除霜,则停机
            if (signal_exit)
            {
                for (i=cm_bgn; i<cm_end; i++)
                {
                    if (i == cm)    continue;
                    
                    if (S_CM_GetStatus(i, CM_DEFROST) || S_CM_GetStatus(i, CM_NEED_DEFROST))
                    {
                        other_defrost = TRUE;
                        break;
                    }
                }

                if (other_defrost)  attr = DEFRO_EXIT_SIN;
                else                attr = DEFRO_EXIT_ALL;
            }
        }
    }
    
    // 处理退出除霜信号量
    {
        BOL allow_exit = FALSE;
		BOL fg_accept = FALSE;				/* 信号接收标志 */
        if (S_CM_GetStatus(cm, CM_DEFROST))
        {
            allow_exit = TRUE;
        }

		/* 不用"||"是因为信号都要被处理 */
        if (SIGNAL_Accept(&defrost_signal_exit_highpress[cm], allow_exit))
        {
            fg_accept = TRUE;
            /* 补丁 */
            {
                /* 2020-10-27会议沟通：严重故障时，直接停掉压缩机 */
                S_CM_ClrStatus(cm, CM_NEED);
                S_CM_ClrStatus(cm, CM_ON);	// 关压机
                s_clr_output(cm, SW_CM);
                mod_err_comp_risk[cm] = 1;
            }
        }
		if (SIGNAL_Accept(&defrost_signal_exit_exh[cm], allow_exit))
        {
            fg_accept = TRUE;
        }
		if (SIGNAL_Accept(&defrost_signal_exit_cur[cm], allow_exit))
        {
            fg_accept = TRUE;
        }

		if (fg_accept)
		{
		    attr = DEFRO_EXIT_SIN;
		}
    }

    
    // 退出除霜动作
    if (attr != 0xFF)
    {
        defrost_grp_exit_act(cm_bgn, cm_end, cm, attr);
    }
    
}
#else
/*****************************************************************************
函  数 : defrost_grp_exit()
功  能 : 一组共用风机的压机压缩退出除霜条件判断
参  数 : U08 cm_bgn    : 组起始压机号
         U08 cm_end    : 组结束压机号
         U08 cm        : 当前压机号
返  回 : 无
其  他 : S_CM_ClrStatus(cm, CM_NEED_DEFROST);
*****************************************************************************/
void defrost_grp_exit(U08 cm_bgn, U08 cm_end, U08 cm)
{
    U08 i;
    U08 attr = 0xFF;
    BOL grp_exit_frost_time = FALSE;    // 除霜时间到,退组
    BOL grp_exit_out_temp = FALSE;      // 出水温度低,退组
    BOL grp_exit_wing_temp = FALSE;     // 所有除霜需求的翅片温度高,退组
    BOL has_defrost_cm = FALSE;            // 有除霜需求的机组
    BOL evp_independ = (val_evap_type < log_mod_cond_type(val_comp_num)); // 蒸发器独立??
    I16 temp_out = defrost_fun_get_out_temp(cm);
    
    // 判断退出除霜条件
    if (S_CM_GetStatus(cm, CM_NEED_DEFROST))
    {
        //3 判断是否同组退出除霜
        // 除霜时间到
        if (defrost_cnt_defrost[cm] >= AT_VAL(val_defrost_time, val_ability_test_defr_time))  
        {
            grp_exit_frost_time = TRUE;
        }
        // 任何出水温度低
        if (defrost_cnt_out_temp_low[cm] > 10 
        &&  temp_out<=val_defrost_end_out	/* 防止出水温度拉升过程出现误判 */
        &&  !evp_independ)
        {
            grp_exit_out_temp = TRUE;
        }
        // 所有除霜需求的的翅片温度高
        for (i=cm_bgn, grp_exit_wing_temp=TRUE; i<cm_end; i++) // 初始化为退出
        {
            if (S_CM_GetStatus(i, CM_NEED_DEFROST))
            {
                has_defrost_cm = TRUE;
                if (defrost_fun_get_wing(i) < AT_VAL(val_defrost_end_wing, val_ability_test_wing))
                {
                    grp_exit_wing_temp = FALSE;
                }
            }
        }
        grp_exit_wing_temp = (grp_exit_wing_temp && has_defrost_cm);

        //4 仲裁组退出
        if (grp_exit_frost_time || grp_exit_out_temp || grp_exit_wing_temp)
        {
            attr = DEFRO_EXIT_ALL;
        }

        //3 判断是否单个退出
        if (attr == 0xFF)
        {
            BOL signal_exit = FALSE;
            BOL other_defrost = FALSE;
            // 出水温度低
            if (  (defrost_cnt_out_temp_low[cm] > 10 && evp_independ)
                ||(defrost_fun_get_wing(cm) > AT_VAL(val_defrost_end_wing, val_ability_test_wing) + val_defrost_end_diff)
                )
            {
                signal_exit = TRUE;
            }

            // 有其他压机在除霜,则停机
            if (signal_exit)
            {
                for (i=cm_bgn; i<cm_end; i++)
                {
                    if (i == cm)    continue;
                    
                    if (S_CM_GetStatus(i, CM_DEFROST) || S_CM_GetStatus(i, CM_NEED_DEFROST))
                    {
                        other_defrost = TRUE;
                        break;
                    }
                }

                if (other_defrost)  attr = DEFRO_EXIT_SIN;
                else                attr = DEFRO_EXIT_ALL;
            }
        }
    }
    
    // 处理退出除霜信号量
    {
        BOL allow_exit = FALSE;
		BOL fg_accept = FALSE;				/* 信号接收标志 */
        if (S_CM_GetStatus(cm, CM_DEFROST))
        {
            allow_exit = TRUE;
        }

		/* 不用"||"是因为信号都要被处理 */
        if (SIGNAL_Accept(&defrost_signal_exit_highpress[cm], allow_exit))
        {
            fg_accept = TRUE;
            /* 补丁 */
            {
                /* 2020-10-27会议沟通：严重故障时，直接停掉压缩机 */
                S_CM_ClrStatus(cm, CM_NEED);
                S_CM_ClrStatus(cm, CM_ON);	// 关压机
                s_clr_output(cm, SW_CM);
                mod_err_comp_risk[cm] = 1;
            }
        }
		if (SIGNAL_Accept(&defrost_signal_exit_exh[cm], allow_exit))
        {
            fg_accept = TRUE;
        }
		if (SIGNAL_Accept(&defrost_signal_exit_cur[cm], allow_exit))
        {
            fg_accept = TRUE;
        }

		if (fg_accept)
		{
		    attr = DEFRO_EXIT_SIN;
		}
    }

    
    // 退出除霜动作
    if (attr != 0xFF)
    {
        defrost_grp_exit_act(cm_bgn, cm_end, cm, attr);
    }
    
}
#endif

/*****************************************************************************
函  数 : defrost_over()
功  能 : 除霜结束后处理
参  数 : U08 cm : 当前压机号
返  回 : 无
其  他 : 无
*****************************************************************************/
void defrost_over(U08 cm)
{
    if (!S_CM_GetStatus(cm, CM_NEED_DEFROST)
       && S_CM_GetStatus(cm, CM_DEFROST))   /* 退除霜前开风机 */
    {
//        if(cnt_time_fan_delay[cm] >= val_defrost_fan_dly)
        if(cm_time[cm].cnt_time_fan_delay >= val_defrost_fan_dly)
        {               
            S_CM_ClrStatus(cm, CM_DEFROST_ALLOW);   /* 退出除霜后清零 */ 
			if (defrost_cm_step[cm] == DEFROST_CM_NULL)
            {
                S_CM_ClrStatus(cm, CM_DEFROST);        
            }
        }
    }

    /* 正在除霜,清状态 */
    if (S_CM_GetStatus(cm, CM_NEED_DEFROST)
        && S_CM_GetStatus(cm, CM_DEFROST))
    {
        defrost_cnt_frost[cm] = 0;
//        cnt_time_fan_delay[cm] = 0;
        cm_time[cm].cnt_time_fan_delay = 0;
        defrost_fg[cm].defrost_apply = FALSE;
        defrost_fg[cm].defrost_end = FALSE;
    }
}

/*****************************************************************************
函  数 : defrost_comp_stop()
功  能 : 除霜停压机的状态机处理
________________                         __________________________________ 风机
                ￣↓                     ↑        
                  ↓______________...____↑        

_______            _____________...________             ___________________ 压机
     ￣↓         ↑      ↑               ↓          ↑      ↑
       ↓_________↑      ↑               ↓__________↑      ↑
                                          
__________________________ 延后关                提前开         ___________ 四通阀
          ￣      ↓      ↓                           ↑      ↑
           提前关 ↓      ↓____...____________________↑______↑延后开
             

参  数 : U08 cm : 
返  回 : 无
其  他 : 无
*****************************************************************************/
void defrost_comp_stop(U08 cm)
{
    cm = cm;
}
#if 0
{
    if (!USE_FIX_COMP) return;				/* 定频压机才有停机切阀 */
    
    for (cm=0; cm<val_comp_num; cm++)
    {
        switch(defrost_cm_step[cm])
        {
            // 无
            case DEFROST_CM_NULL:
                if (val_defrost_stop_cm_en
                    && S_CM_GetStatus(cm, CM_DEFROST)
                    && defrost_cm_intv_off >= 4
                    )
                {
					defrost_cm_intv_off = 0;
                    defrost_cm_time[cm] = 0;
                    defrost_cm_step[cm] = DEFROST_CM_STOP_BGN;
                }
                break;
            // 压机停机,四通开,处理阀
            case DEFROST_CM_STOP_BGN:
                if (defrost_cm_time[cm] >= val_defrost_stop_cm_tm_entry
                    && defrost_cm_intv_on >= 4)
                {
                    defrost_cm_intv_on = 0;
                    defrost_cm_time[cm] = 0;
                    if (val_defrost_four_tm_entry >= 0) // 提前
                    {
                        defrost_cm_step[cm] = DEFROST_CM_OFF_FORE_OFF;
                    }
                    else
                    {
                        defrost_cm_step[cm] = DEFROST_CM_ON_FORE_ON;
                    }
                }
                break;
            // 提前关四通
            case DEFROST_CM_OFF_FORE_OFF:
            // 延后关四通
            case DEFROST_CM_ON_FORE_ON:
                if (defrost_cm_time[cm] >= fun_absolute_value(val_defrost_four_tm_entry))
                {
                    defrost_cnt_defrost[cm] = 0; // 重新计时除霜时间
                    defrost_cm_time[cm] = 0;
                    defrost_cm_step[cm] = DEFROST_CM_ON;
                }
                break;
            // 压机启动
            case DEFROST_CM_ON:
                if (!S_CM_GetStatus(cm, CM_NEED_DEFROST)
				 && defrost_cm_intv_off >= 4
					) // 退除霜
                {
					defrost_cm_intv_off = 0;
                    defrost_cm_time[cm] = 0;
                    defrost_cm_step[cm] = DEFROST_CM_STOP_END;
                }
                break;
            // 压机停机,四通关,处理阀
            case DEFROST_CM_STOP_END:
                if (defrost_cm_time[cm] >= val_defrost_stop_cm_tm_entry
                    && defrost_cm_intv_on >= 4)
                {
                    defrost_cm_intv_on = 0;
                    defrost_cm_time[cm] = 0;
                    if (val_defrost_four_tm_entry >= 0) // 提前
                    {
                        defrost_cm_step[cm] = DEFROST_CM_OFF_FORE_ON;
                    }
                    else
                    {
                        defrost_cm_step[cm] = DEFROST_CM_ON_FORE_OFF;
                    }
                }
                break;
            // 提前开四通
            case DEFROST_CM_OFF_FORE_ON:
            // 延后开四通    
            case DEFROST_CM_ON_FORE_OFF:                
                if (defrost_cm_time[cm] >= fun_absolute_value(val_defrost_four_tm_entry))
                {
                    defrost_cm_time[cm] = 0;
                    defrost_cnt_defrost[cm] = 0;
                    defrost_cm_step[cm] = DEFROST_CM_OVER;
                }
                break;
            // 结束除霜
            case DEFROST_CM_OVER:
                S_CM_ClrStatus(cm, CM_DEFROST); 
                defrost_cm_step[cm] = DEFROST_CM_STOP;
                break;
            case DEFROST_CM_STOP: /* 实际转不到这个状态 */
            default:
                defrost_cm_step[cm] = DEFROST_CM_NULL;
                break;
        }

        /* 除霜停机 */
        switch (defrost_cm_step[cm])
        {
            /* 除霜特殊情况需要停压缩机,但不清CM_ON标志 */
            case DEFROST_CM_STOP_BGN:
            case DEFROST_CM_OFF_FORE_OFF:
            case DEFROST_CM_STOP_END:
            case DEFROST_CM_OFF_FORE_ON:
//          case DEFROST_CM_STOP:   // 除霜后重新开启,则此处不能开放
                S_CM_SetStatus(cm, CM_DEFROST_OFF);
                break;
            default:
                S_CM_ClrStatus(cm, CM_DEFROST_OFF);
                break;
        }
    }
}
#endif


/*****************************************************************************
函  数 : defrost_signal_end()
功  能 : 除霜信号量后处理,未处理的信号请求直接拒绝
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
void defrost_signal_end(void)
{
    U08 cm;
    
    for (cm=0; cm<val_comp_num; cm++)
    {
        SIGNAL_AcceptEnd(&defrost_signal_into_lowpress[cm]);
        SIGNAL_AcceptEnd(&defrost_signal_into_namual[cm]);
        SIGNAL_AcceptEnd(&defrost_signal_exit_highpress[cm]);
		SIGNAL_AcceptEnd(&defrost_signal_exit_exh[cm]);
		SIGNAL_AcceptEnd(&defrost_signal_exit_cur[cm]);
    }
}

/*****************************************************************************
函  数 : defrost_init()
功  能 : 数据初始化
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
void defrost_init(void)
{
    U16 i;

    ARRCLR(defrost_fg);
    ARRSET(defrost_temp_EWD_nofrost, SENSOR_NOEXIST);
    ARRSET(defrost_temp_EWD_nofrost_env, SENSOR_NOEXIST);
    ARRSET(defrost_temp_EWD_stop, SENSOR_NOEXIST);
    ARRSET(defrost_rec_EWD_avg, SENSOR_NOEXIST);
	ARRSET(defrost_rec_EWD_usedavg_avg, SENSOR_NOEXIST);

	
    for (i=0; i<MAX_COMP; i++)
    {
        ARRSET(defrost_rec_EWD[i], SENSOR_NOEXIST);
        ARRSET(defrost_rec_EWD_usedavg[i], SENSOR_NOEXIST);
    }
}

/*****************************************************************************
函  数 : defrost()
功  能 : 除霜过程总调度
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
void defrost(void)
{
    U08 comp_num;
    U08 begin, end;

    defrost_time();

    defrost_fun_rec_data();
    
    for (comp_num=0; comp_num<val_comp_num; comp_num++) 
    {
		// 停机超过4小时,清除霜结束和标定标志
        if (cm_time[comp_num].off > (4*60*60))
        {
            defrost_fg[comp_num].defrost_end = FALSE;
            defrost_fg[comp_num].need_calib_EWD = FALSE;
        }
        
        // 是否需要判断除霜
        if (Cm[comp_num].run_mode == MODE_COOL
        || (interface_defrost_wing_err(comp_num))                  /* 翅片温度探头故障 */
        || SensorIsError(defrost_fun_get_env())             /* 环境温度探头故障 */
        || !S_CM_GetStatus(comp_num, CM_ON))                /*  压机未运行 */
        { // 请除除霜相关标志
            S_CM_ClrStatus(comp_num, CM_NEED_DEFROST);        /* 除霜需求 */
            S_CM_ClrStatus(comp_num, CM_DEFROST);             /* 除霜标志 */ 
            S_CM_ClrStatus(comp_num, CM_DEFROST_ALLOW);
            S_CM_ClrStatus(comp_num, CM_FORCE_DEFROST);       /* 强制除霜标志 */
            S_CM_ClrStatus(comp_num, CM_ALLOW_FORCE_DEFROST); /* 压机强制除霜满足标志 */
            S_CM_ClrStatus(comp_num, CM_DEFROST_OFF);         
            defrost_fg[comp_num].defrost_apply = FALSE;
            defrost_cm_step[comp_num] = DEFROST_CM_NULL;
            continue;   
        }

        begin = 0, end = 0;
        log_mod_fan_comp_group(log_mod_cond_type(val_comp_num), &begin, &end, comp_num);
        if (end == 0)       break;              /* 无意义数据，直接跳出 */
        
        // 处理是否需要进入除霜
        defrost_grp_into(begin, end, comp_num);

        // 处理是否需要退出除霜
        defrost_grp_exit(begin, end, comp_num);

        // 退出除霜后处理
        defrost_over(comp_num);

        // 除霜停机过程
        defrost_comp_stop(comp_num);
    }

    //3 信号后处理
    defrost_signal_end();
}


