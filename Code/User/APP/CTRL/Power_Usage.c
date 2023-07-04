/******************************************************************************

          版权所有 (C), 2001-2022, 广州市邦普电脑技术开发有限公司

 ******************************************************************************
  文 件 名   : Power_Usage.h
  版 本 号   : 初稿
  作    者   : J.H.
  生成日期   : 2022年6月6日星期一
  最近修改   :
  功能描述   : Power_Usage.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2022年6月6日星期一
    作    者   : J.H.
    修改内容   : 创建文件

  2.日    期   : 2023-2-8
    作    者   : KJH
    修改内容   : 完善周、年用电计量功能

******************************************************************************/
#define POWER_USAGE_GLOBAL
/*-----------------包含头文件 -----------------------------*/
#include "Includes.h"


/*-----------------外部变量说明----------------------------*/



/*-----------------外部函数原型说明------------------------*/



/*-----------------内部函数原型说明------------------------*/



/*-----------------全局变量--------------------------------*/



/*-----------------模块级变量------------------------------*/

/*******************************************************************************
 函 数: PU_info_init()
 功 能: 初始化结构体类型PU_INFO
 参 数: p_obj--指针; 
 返 回: 
 其 它: 空
*******************************************************************************/
static void PU_info_init(PU_INFO* p_obj)
{
    p_obj->timestamp    = fun_time_mktime(&clock[hmi_t_num]);
    p_obj->val_cool     = 0; 
    p_obj->val_heat     = 0; 
    p_obj->val_dhw      = 0; 
}

/*******************************************************************************
 函 数: PU_info_inc()
 功 能: 由p_src累加到p_obj
 参 数: p_obj--目标; 
		p_src--源; 
 返 回: 
 其 它: 空
*******************************************************************************/
static void PU_info_inc(PU_INFO* p_obj, PU_INFO* p_src)
{
    p_obj->timestamp     = p_src->timestamp;
    p_obj->val_cool     += p_src->val_cool;     
    p_obj->val_heat     += p_src->val_heat;     
    p_obj->val_dhw      += p_src->val_dhw;      
}

/*******************************************************************************
 函 数: PU_week_cache_append()
 功 能: 从滚动一周用电量队列的列尾部推入一个节点
 参 数: p_src--; 
 返 回: 
 其 它: 空
*******************************************************************************/
static BOOL PU_week_cache_append(PU_INFO* p_src)
{
    // 非满
    if (pwru_week_cache.queue.cnt < POWER_USAGE_WEEK_SIZE)
    {
        if (++pwru_week_cache.queue.end >= POWER_USAGE_WEEK_SIZE)
        {
            pwru_week_cache.queue.end = 0;
        }

        pwru_week_cache.node[pwru_week_cache.queue.end] = *p_src;

        pwru_week_cache.queue.cnt++;

        return TRUE;
    }

    return FALSE;
}

/*******************************************************************************
 函 数: PU_week_cache_shift()
 功 能: 从滚动一周用电量队列的头部移出一个节点
 参 数: p_dst--; 
 返 回: 
 其 它: 空
*******************************************************************************/
static BOOL PU_week_cache_shift(void)
{
    // 满了才需要移，否则直接添加
    if (pwru_week_cache.queue.cnt == POWER_USAGE_WEEK_SIZE)
    {
        if (++pwru_week_cache.queue.head >= POWER_USAGE_WEEK_SIZE)
        {
            pwru_week_cache.queue.head = 0;
        }

        pwru_week_cache.queue.cnt--;

        return TRUE;
    }

    return FALSE;
}

/*******************************************************************************
 函 数: PU_week_cache_init()
 功 能: 初始化pwru_week_cache
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
static void PU_week_cache_init(void)
{
    PU_INFO pu_info;

    PU_info_init(&pu_info);

    /* 清空cache */
    VALCLR(pwru_week_cache);			
    
    /* 插入一个初始化节点 */
    {
        pwru_week_cache.queue.head = 0;
        pwru_week_cache.queue.end = 0;
        pwru_week_cache.node[pwru_week_cache.queue.end] = pu_info;
        pwru_week_cache.queue.cnt = 1;
    }
}

/*******************************************************************************
 函 数: PU_week_cache_check()
 功 能: 滚动一周用电量队列存储检查
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
static void PU_week_cache_check(void)
{
    U08 i, num, end;
    I32 diff;
    PU_INFO pu_info;
    LOG_RTC time;

    /* 初始化 */
    if (pwru_week_cache.queue.cnt == 0
    ||  pwru_week_cache.queue.cnt > POWER_USAGE_WEEK_SIZE
    )
    {
        PU_week_cache_init();
        return;
    }

    end = pwru_week_cache.queue.end;

    /* 最新节点与当前日期比较 */
    time = fun_time_localtime(pwru_week_cache.node[end].timestamp);
    diff = fun_time_diff_day(&time, &clock[hmi_t_num]);
    
    if (diff == 0)                          /* 当天节点，则不检查 */
    {
        return;
    }
    else if (diff < 0)		                /* 未来节点，初始化 */
    {
        PU_week_cache_init();
        return;
    }

    num = MIN(diff, POWER_USAGE_WEEK_SIZE);
    for (i = 0; i < num; i++)
    {
        PU_week_cache_shift();

        /* 补充节点 */
        PU_info_init(&pu_info);
        pu_info.timestamp -= 24L*3600*(num-1-i);	            /* 计算x天前的时间戳 */
        PU_week_cache_append(&pu_info);
    }
}

/*******************************************************************************
 函 数: PU_year_cache_append()
 功 能: 从滚动一年用电量队列的列尾部推入一个节点
 参 数: p_src--; 
 返 回: 
 其 它: 空
*******************************************************************************/
static BOOL PU_year_cache_append(PU_INFO* p_src)
{
    // 非满
    if (pwru_year_cache.queue.cnt < POWER_USAGE_YEAR_SIZE)
    {
        if (++pwru_year_cache.queue.end >= POWER_USAGE_YEAR_SIZE)
        {
            pwru_year_cache.queue.end = 0;
        }

        pwru_year_cache.node[pwru_year_cache.queue.end] = *p_src;

        pwru_year_cache.queue.cnt++;

        return TRUE;
    }

    return FALSE;
}

/*******************************************************************************
 函 数: PU_year_cache_shift()
 功 能: 从滚动一年用电量队列的头部移出一个节点
 参 数: p_dst--; 
 返 回: 
 其 它: 空
*******************************************************************************/
static BOOL PU_year_cache_shift(void)
{
    // 满了才需要移，否则直接添加
    if (pwru_year_cache.queue.cnt == POWER_USAGE_YEAR_SIZE)
    {
        if (++pwru_year_cache.queue.head >= POWER_USAGE_YEAR_SIZE)
        {
            pwru_year_cache.queue.head = 0;
        }

        pwru_year_cache.queue.cnt--;

        return TRUE;
    }

    return FALSE;
}

/*******************************************************************************
 函 数: PU_year_cache_init()
 功 能: 初始化pwru_year_cache
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
static void PU_year_cache_init(void)
{
    PU_INFO pu_info;

    PU_info_init(&pu_info);

    /* 清空cache */
    VALCLR(pwru_year_cache);			
    
    /* 插入一个初始化节点 */
    {
        pwru_year_cache.queue.head = 0;
        pwru_year_cache.queue.end = 0;
        pwru_year_cache.node[pwru_year_cache.queue.end] = pu_info;
        pwru_year_cache.queue.cnt = 1;
    }
}

/*******************************************************************************
 函 数: PU_year_cache_check()
 功 能: 滚动一年用电量队列存储检查
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
static void PU_year_cache_check(void)
{
    U08 i, num, end;
    I32 diff;
    PU_INFO pu_info;
    LOG_RTC time;

    /* 初始化 */
    if (pwru_year_cache.queue.cnt == 0
    ||  pwru_year_cache.queue.cnt > POWER_USAGE_YEAR_SIZE
    )
    {
        PU_year_cache_init();
        return;
    }

    end = pwru_year_cache.queue.end;

    /* 最新节点与当前日期比较 */
    time = fun_time_localtime(pwru_year_cache.node[end].timestamp);
    diff = clock[hmi_t_num].month-time.month + POWER_USAGE_YEAR_SIZE*(clock[hmi_t_num].year-time.year);
    
    if (diff == 0)                          /* 当月节点，则不检查 */
    {
        return;
    }
    else if (diff < 0)		                /* 未来节点，初始化 */
    {
        PU_year_cache_init();
        return;
    }

    num = MIN(diff, POWER_USAGE_YEAR_SIZE);
    for (i = 0; i < num; i++)
    {
        PU_year_cache_shift();

        /* 补充节点 */
        PU_info_init(&pu_info);
        pu_info.timestamp -= 30L*24*3600*(num-1-i);	            /* 计算x月前的时间戳:这个不要好像也可以 */
        PU_year_cache_append(&pu_info);
    }
}

/*******************************************************************************
 函 数: PU_cal_power_usage()
 功 能: 用电计量处理
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void PU_cal_power_usage(void)
{
    PU_INFO *pu_week;
    PU_INFO *pu_year;
    static I16 remainder_cl = 0;
    static I16 remainder_ht = 0;
    static I16 remainder_hw = 0;
    
    /* 确保已读取到时间 */
    if (!fun_rtc_check(clock[hmi_t_num])
    ||  !USE_ELEC_METERING
    )                       
    {
        _SETMAX(dbg_cnt_pwru_cal_10s);
        _SETMAX(dbg_cnt_pwru_cal_20s);
        _SETMAX(dbg_cnt_pwru_cal_60s);
            
        return;
    }
    
    PU_week_cache_check();
    PU_year_cache_check();

    pu_week = &pwru_week_cache.node[pwru_week_cache.queue.end];
    pu_year = &pwru_year_cache.node[pwru_year_cache.queue.end];


    /* 每20秒累计一次 */
    if (cnt_pwru_cal_sec >= 20)
    {
        PU_INFO pu_inc;
        
        cnt_pwru_cal_sec = 0;

        pu_inc.timestamp = fun_time_mktime(&clock[hmi_t_num]);
        /* pv_sys_cl_pwr：单位0.01kw */
        pu_inc.val_cool  = fun_div_remainder(&remainder_cl, (I32)pv_sys_cl_pwr, 18000);  /* 单位：1千瓦时，1度 */
        pu_inc.val_heat  = fun_div_remainder(&remainder_ht, (I32)pv_sys_ht_pwr, 18000);
        pu_inc.val_dhw   = fun_div_remainder(&remainder_hw, (I32)pv_sys_hw_pwr, 18000);

        PU_info_inc(pu_week, &pu_inc);
        PU_info_inc(pu_year, &pu_inc);
    }

    /* 每小时保存一次 */
    if (cnt_pwru_save_min >= QK_60)
    {
        cnt_pwru_save_min = 0;

        DataAccess_PowerUsageWeek(DATA_WRITE);
        DataAccess_PowerUsageYear(DATA_WRITE);
    }

    /* 调试用 */
    {
        #define DBG_PU_D(cnt) (3600/(cnt))
        static I16 remainder_10s = 0;
        static I16 remainder_20s = 0;
        static I16 remainder_60s = 0;
        static I32 power_10s = 0;
        static I32 power_20s = 0;
        static I32 power_60s = 0;
        
        if (dbg_cnt_pwru_cal_60s >= 60)
        {
            dbg_cnt_pwru_cal_60s = 0;
            power_60s = pv_cell_power(pcb_addr);
        }
        if (dbg_cnt_pwru_cal_20s >= 20)
        {
            dbg_cnt_pwru_cal_20s = 0;
            power_20s = pv_cell_power(pcb_addr);
        }
        if (dbg_cnt_pwru_cal_10s >= 10)
        {
            dbg_cnt_pwru_cal_10s = 0;
            power_10s = pv_cell_power(pcb_addr);
            
            dbg_PU_10s  += fun_div_remainder(&remainder_10s, power_10s, DBG_PU_D(10));  /* 单位：1w*h */
            dbg_PU_20s  += fun_div_remainder(&remainder_20s, power_20s, DBG_PU_D(10));
            dbg_PU_60s  += fun_div_remainder(&remainder_60s, power_60s, DBG_PU_D(10));
        }
    }
}

void PU_init(void)
{
    DataAccess_PowerUsageWeek(DATA_READ);
    DataAccess_PowerUsageYear(DATA_READ);
}


