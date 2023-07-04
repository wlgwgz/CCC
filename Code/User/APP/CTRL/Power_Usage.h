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

******************************************************************************/

#ifndef __POWER_USAGE_H__
#define __POWER_USAGE_H__

#ifdef POWER_USAGE_GLOBAL
    #define PWRU_EXT
#else
    #define PWRU_EXT extern
#endif

/*-----------------包含头文件 -----------------------------*/



/*-----------------外部变量说明----------------------------*/



/*-----------------外部函数原型说明------------------------*/



/*-----------------内部函数原型说明------------------------*/



/*-----------------全局变量--------------------------------*/



/*-----------------模块级变量------------------------------*/



/*-----------------常量定义--------------------------------*/




/*-----------------宏定义 ---------------------------------*/

#define POWER_USAGE_WEEK_SIZE                     (7)  /* 一周用电量记录条目数 */
#define POWER_USAGE_YEAR_SIZE                     (12) /* 一年用电量记录条目数 */

/* 队列信息 */
typedef struct {
    U08 head;    /* 头游标 */
    U08 end;     /* 尾游标 */
    U08 cnt;     /* 节点计数 */
} PWRU_QUEUE;

typedef struct tag_POWER_USAGE_INFO
{ 
    U32 timestamp;          /* 时间戳 */
    U16 val_cool;			/* 制冷当前值 */
    U16 val_heat;			/* 制热当前值 */
    U16 val_dhw;			/* 制热水当前值 */
} PU_INFO;

/* 滚动一周用电量队列 */
typedef struct {
    PWRU_QUEUE queue;                       /* 队列信息 */
    PU_INFO node[POWER_USAGE_WEEK_SIZE];	/* 节点信息 */
} PU_WEEK_CACHE;

/* 滚动一年用电量队列 */
typedef struct {
    PWRU_QUEUE queue;                       /* 队列信息 */
    PU_INFO node[POWER_USAGE_YEAR_SIZE];	/* 节点信息 */
} PU_YEAR_CACHE;

PWRU_EXT U16 cnt_pwru_save_min;
PWRU_EXT U16 cnt_pwru_cal_sec;
PWRU_EXT PU_WEEK_CACHE pwru_week_cache;     /* 滚动一周用电量 3*7 */
PWRU_EXT PU_YEAR_CACHE pwru_year_cache;     /* 滚动一年用电量 3*12 */

/* 调试用 */
PWRU_EXT U16 dbg_cnt_pwru_cal_10s;
PWRU_EXT U16 dbg_cnt_pwru_cal_20s;
PWRU_EXT U16 dbg_cnt_pwru_cal_60s;
PWRU_EXT U16 dbg_PU_10s;
PWRU_EXT U16 dbg_PU_20s;
PWRU_EXT U16 dbg_PU_60s;

#define POWER_USAGE_WEEK_BYTES              (sizeof(pwru_week_cache)/sizeof(U08))
#define POWER_USAGE_YEAR_BYTES              (sizeof(pwru_year_cache)/sizeof(U08))

PWRU_EXT void PU_cal_power_usage(void);
PWRU_EXT void PU_init(void);

#endif /* __POWER_USAGE_H__ */
