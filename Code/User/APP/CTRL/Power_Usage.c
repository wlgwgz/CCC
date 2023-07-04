/******************************************************************************

          ��Ȩ���� (C), 2001-2022, �����а��յ��Լ����������޹�˾

 ******************************************************************************
  �� �� ��   : Power_Usage.h
  �� �� ��   : ����
  ��    ��   : J.H.
  ��������   : 2022��6��6������һ
  ����޸�   :
  ��������   : Power_Usage.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2022��6��6������һ
    ��    ��   : J.H.
    �޸�����   : �����ļ�

  2.��    ��   : 2023-2-8
    ��    ��   : KJH
    �޸�����   : �����ܡ����õ��������

******************************************************************************/
#define POWER_USAGE_GLOBAL
/*-----------------����ͷ�ļ� -----------------------------*/
#include "Includes.h"


/*-----------------�ⲿ����˵��----------------------------*/



/*-----------------�ⲿ����ԭ��˵��------------------------*/



/*-----------------�ڲ�����ԭ��˵��------------------------*/



/*-----------------ȫ�ֱ���--------------------------------*/



/*-----------------ģ�鼶����------------------------------*/

/*******************************************************************************
 �� ��: PU_info_init()
 �� ��: ��ʼ���ṹ������PU_INFO
 �� ��: p_obj--ָ��; 
 �� ��: 
 �� ��: ��
*******************************************************************************/
static void PU_info_init(PU_INFO* p_obj)
{
    p_obj->timestamp    = fun_time_mktime(&clock[hmi_t_num]);
    p_obj->val_cool     = 0; 
    p_obj->val_heat     = 0; 
    p_obj->val_dhw      = 0; 
}

/*******************************************************************************
 �� ��: PU_info_inc()
 �� ��: ��p_src�ۼӵ�p_obj
 �� ��: p_obj--Ŀ��; 
		p_src--Դ; 
 �� ��: 
 �� ��: ��
*******************************************************************************/
static void PU_info_inc(PU_INFO* p_obj, PU_INFO* p_src)
{
    p_obj->timestamp     = p_src->timestamp;
    p_obj->val_cool     += p_src->val_cool;     
    p_obj->val_heat     += p_src->val_heat;     
    p_obj->val_dhw      += p_src->val_dhw;      
}

/*******************************************************************************
 �� ��: PU_week_cache_append()
 �� ��: �ӹ���һ���õ������е���β������һ���ڵ�
 �� ��: p_src--; 
 �� ��: 
 �� ��: ��
*******************************************************************************/
static BOOL PU_week_cache_append(PU_INFO* p_src)
{
    // ����
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
 �� ��: PU_week_cache_shift()
 �� ��: �ӹ���һ���õ������е�ͷ���Ƴ�һ���ڵ�
 �� ��: p_dst--; 
 �� ��: 
 �� ��: ��
*******************************************************************************/
static BOOL PU_week_cache_shift(void)
{
    // ���˲���Ҫ�ƣ�����ֱ�����
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
 �� ��: PU_week_cache_init()
 �� ��: ��ʼ��pwru_week_cache
 �� ��: ��
 �� ��: 
 �� ��: ��
*******************************************************************************/
static void PU_week_cache_init(void)
{
    PU_INFO pu_info;

    PU_info_init(&pu_info);

    /* ���cache */
    VALCLR(pwru_week_cache);			
    
    /* ����һ����ʼ���ڵ� */
    {
        pwru_week_cache.queue.head = 0;
        pwru_week_cache.queue.end = 0;
        pwru_week_cache.node[pwru_week_cache.queue.end] = pu_info;
        pwru_week_cache.queue.cnt = 1;
    }
}

/*******************************************************************************
 �� ��: PU_week_cache_check()
 �� ��: ����һ���õ������д洢���
 �� ��: ��
 �� ��: 
 �� ��: ��
*******************************************************************************/
static void PU_week_cache_check(void)
{
    U08 i, num, end;
    I32 diff;
    PU_INFO pu_info;
    LOG_RTC time;

    /* ��ʼ�� */
    if (pwru_week_cache.queue.cnt == 0
    ||  pwru_week_cache.queue.cnt > POWER_USAGE_WEEK_SIZE
    )
    {
        PU_week_cache_init();
        return;
    }

    end = pwru_week_cache.queue.end;

    /* ���½ڵ��뵱ǰ���ڱȽ� */
    time = fun_time_localtime(pwru_week_cache.node[end].timestamp);
    diff = fun_time_diff_day(&time, &clock[hmi_t_num]);
    
    if (diff == 0)                          /* ����ڵ㣬�򲻼�� */
    {
        return;
    }
    else if (diff < 0)		                /* δ���ڵ㣬��ʼ�� */
    {
        PU_week_cache_init();
        return;
    }

    num = MIN(diff, POWER_USAGE_WEEK_SIZE);
    for (i = 0; i < num; i++)
    {
        PU_week_cache_shift();

        /* ����ڵ� */
        PU_info_init(&pu_info);
        pu_info.timestamp -= 24L*3600*(num-1-i);	            /* ����x��ǰ��ʱ��� */
        PU_week_cache_append(&pu_info);
    }
}

/*******************************************************************************
 �� ��: PU_year_cache_append()
 �� ��: �ӹ���һ���õ������е���β������һ���ڵ�
 �� ��: p_src--; 
 �� ��: 
 �� ��: ��
*******************************************************************************/
static BOOL PU_year_cache_append(PU_INFO* p_src)
{
    // ����
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
 �� ��: PU_year_cache_shift()
 �� ��: �ӹ���һ���õ������е�ͷ���Ƴ�һ���ڵ�
 �� ��: p_dst--; 
 �� ��: 
 �� ��: ��
*******************************************************************************/
static BOOL PU_year_cache_shift(void)
{
    // ���˲���Ҫ�ƣ�����ֱ�����
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
 �� ��: PU_year_cache_init()
 �� ��: ��ʼ��pwru_year_cache
 �� ��: ��
 �� ��: 
 �� ��: ��
*******************************************************************************/
static void PU_year_cache_init(void)
{
    PU_INFO pu_info;

    PU_info_init(&pu_info);

    /* ���cache */
    VALCLR(pwru_year_cache);			
    
    /* ����һ����ʼ���ڵ� */
    {
        pwru_year_cache.queue.head = 0;
        pwru_year_cache.queue.end = 0;
        pwru_year_cache.node[pwru_year_cache.queue.end] = pu_info;
        pwru_year_cache.queue.cnt = 1;
    }
}

/*******************************************************************************
 �� ��: PU_year_cache_check()
 �� ��: ����һ���õ������д洢���
 �� ��: ��
 �� ��: 
 �� ��: ��
*******************************************************************************/
static void PU_year_cache_check(void)
{
    U08 i, num, end;
    I32 diff;
    PU_INFO pu_info;
    LOG_RTC time;

    /* ��ʼ�� */
    if (pwru_year_cache.queue.cnt == 0
    ||  pwru_year_cache.queue.cnt > POWER_USAGE_YEAR_SIZE
    )
    {
        PU_year_cache_init();
        return;
    }

    end = pwru_year_cache.queue.end;

    /* ���½ڵ��뵱ǰ���ڱȽ� */
    time = fun_time_localtime(pwru_year_cache.node[end].timestamp);
    diff = clock[hmi_t_num].month-time.month + POWER_USAGE_YEAR_SIZE*(clock[hmi_t_num].year-time.year);
    
    if (diff == 0)                          /* ���½ڵ㣬�򲻼�� */
    {
        return;
    }
    else if (diff < 0)		                /* δ���ڵ㣬��ʼ�� */
    {
        PU_year_cache_init();
        return;
    }

    num = MIN(diff, POWER_USAGE_YEAR_SIZE);
    for (i = 0; i < num; i++)
    {
        PU_year_cache_shift();

        /* ����ڵ� */
        PU_info_init(&pu_info);
        pu_info.timestamp -= 30L*24*3600*(num-1-i);	            /* ����x��ǰ��ʱ���:�����Ҫ����Ҳ���� */
        PU_year_cache_append(&pu_info);
    }
}

/*******************************************************************************
 �� ��: PU_cal_power_usage()
 �� ��: �õ��������
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void PU_cal_power_usage(void)
{
    PU_INFO *pu_week;
    PU_INFO *pu_year;
    static I16 remainder_cl = 0;
    static I16 remainder_ht = 0;
    static I16 remainder_hw = 0;
    
    /* ȷ���Ѷ�ȡ��ʱ�� */
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


    /* ÿ20���ۼ�һ�� */
    if (cnt_pwru_cal_sec >= 20)
    {
        PU_INFO pu_inc;
        
        cnt_pwru_cal_sec = 0;

        pu_inc.timestamp = fun_time_mktime(&clock[hmi_t_num]);
        /* pv_sys_cl_pwr����λ0.01kw */
        pu_inc.val_cool  = fun_div_remainder(&remainder_cl, (I32)pv_sys_cl_pwr, 18000);  /* ��λ��1ǧ��ʱ��1�� */
        pu_inc.val_heat  = fun_div_remainder(&remainder_ht, (I32)pv_sys_ht_pwr, 18000);
        pu_inc.val_dhw   = fun_div_remainder(&remainder_hw, (I32)pv_sys_hw_pwr, 18000);

        PU_info_inc(pu_week, &pu_inc);
        PU_info_inc(pu_year, &pu_inc);
    }

    /* ÿСʱ����һ�� */
    if (cnt_pwru_save_min >= QK_60)
    {
        cnt_pwru_save_min = 0;

        DataAccess_PowerUsageWeek(DATA_WRITE);
        DataAccess_PowerUsageYear(DATA_WRITE);
    }

    /* ������ */
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
            
            dbg_PU_10s  += fun_div_remainder(&remainder_10s, power_10s, DBG_PU_D(10));  /* ��λ��1w*h */
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


