/******************************************************************************

          版权所有 (C), 2001-2016, 广州市邦普电脑技术开发有限公司

 ******************************************************************************
  文 件 名   : defrost.h
  版 本 号   : 初稿
  作    者   : 周志鹏
  生成日期   : 2018年12月28日星期五
  最近修改   :
  功能描述   : defrost.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2018年12月28日星期五
    作    者   : 周志鹏
    修改内容   : 创建文件

******************************************************************************/
#ifndef __DEFROST_H__
#define __DEFROST_H__

#ifdef GBL_DEFROST
    #define DEFROST_EXT
#else
    #define DEFROST_EXT extern
#endif    

/*-----------------包含头文件 -----------------------------*/

//1 ——————以下内容无特殊情况,应所有程序一致!!!!!
// 如不一致,请说明本文件不一致.


/*-----------------外部变量说明----------------------------*/



/*-----------------外部函数原型说明------------------------*/



/*-----------------内部函数原型说明------------------------*/



/*-----------------全局变量--------------------------------*/



/*-----------------模块级变量------------------------------*/



/*-----------------常量定义--------------------------------*/

/* 除霜退出方式 */
#define DEFRO_EXIT_ALL			0			/* 共同退出 */
#define DEFRO_EXIT_SIN			1			/* 单独退出,并停机 */

/* 积霜方式 */
#define DEFRO_BY_FROSTTIME      0           /* 按积霜时间 */
#define DEFRO_BY_RUNTIME        1           /* 按压机运行时间 */

/* defrost_get_value取值序号 */
#define NUM_EXIT_DEFR_CM_OFF      8           /* 退除霜时压机停机标志：序号 */


/*-----------------宏定义 ---------------------------------*/


//4 积霜状态
enum {
    DEFROST_STA_NULL = 0,       // 条件不满足
    DEFROST_STA_NO_FROST,       // 无积霜
    DEFROST_STA_BY_HUMIDITY,    // 湿度除霜
    DEFROST_STA_BY_ENV_WING,    // 环翅差升除霜
    DEFROST_STA_BY_WING,        // 翅温除霜
    DEFROST_STA_BY_LOWPRESS,    // 低压除霜
    DEFROST_STA_BY_EMERGENCY,   // 紧急除霜
    DEFROST_STA_BY_MANUAL,      // 手动除霜
    DEFROST_STA_BY_LONGTIME,	// 定时除霜
    DEFROST_STA_BY_GAS_IN,      // 吸气温度除霜
    DEFROST_STA_BY_POWER,       // 能力衰减除霜
	DEFROST_STA_MAX
};
#define DEFROST_STA_IS_NEED(sta)    (((sta) == DEFROST_STA_BY_HUMIDITY)\
                                    ||((sta) == DEFROST_STA_BY_ENV_WING)\
                                    ||((sta) == DEFROST_STA_BY_WING)\
                                    ||((sta) == DEFROST_STA_BY_LOWPRESS)\
                                    ||((sta) == DEFROST_STA_BY_EMERGENCY)\
                                    ||((sta) == DEFROST_STA_BY_MANUAL)\
                                    ||((sta) == DEFROST_STA_BY_LONGTIME)\
                                    ||((sta) == DEFROST_STA_BY_GAS_IN)\
                                    ||((sta) == DEFROST_STA_BY_POWER)\
                                    )

//4 除霜步骤
enum {
    DEFROST_STEP_NULL = 0,      // 初始态
    DEFROST_STEP_FROSTING,      // 积霜中
    DEFROST_STEP_DEFROST,       // 除霜中
    DEFROST_STEP_OVER           // 除霜结束
};


//4 除霜停压机步骤
enum {
    DEFROST_CM_NULL        =0, // 无
    DEFROST_CM_STOP_BGN    =1, // 压机停机,四通开
    DEFROST_CM_OFF_FORE_OFF=2, // 提前关四通
    DEFROST_CM_ON_FORE_ON  =3, // 延后关四通
    DEFROST_CM_ON          =4, // 压机启动
    DEFROST_CM_STOP_END    =5, // 压机停机,四通关
    DEFROST_CM_OFF_FORE_ON =6, // 提前开四通
    DEFROST_CM_ON_FORE_OFF =7, // 延后开四通    
    DEFROST_CM_OVER        =8, // 结束除霜
    DEFROST_CM_STOP        =9  // 转入停机
};

typedef struct tagDEFROST_FG
{
    BOL emergency_defrost_frequent : 1; // 紧急除霜频繁
    BOL defrost_end                : 1; // 除过霜
    BOL first_defrost              : 1; // 已首次除霜
    BOL defrost_apply              : 1; // 申请除霜
    BOL bak4            		   : 1; // 保留
    BOL need_calib_EWD             : 1; // 需标定环翅差
    BOL need_calib_stop_EWD        : 1; // 需标定停机环翅差    
    BOL exit_defr_cm_off           : 1; // 退除霜时压机停机标志
}DEFROST_FG;

DEFROST_EXT U08 fg_defrost_apply[MAX_COMP];			// 申请除霜标志 
DEFROST_EXT DEFROST_FG defrost_fg[MAX_COMP];
DEFROST_EXT SIGNALSTA defrost_signal_into_lowpress[MAX_COMP];
DEFROST_EXT SIGNALSTA defrost_signal_into_namual[MAX_COMP];
DEFROST_EXT SIGNALSTA defrost_signal_exit_highpress[MAX_COMP];
DEFROST_EXT SIGNALSTA defrost_signal_exit_exh[MAX_COMP];
DEFROST_EXT SIGNALSTA defrost_signal_exit_cur[MAX_COMP];

DEFROST_EXT U08 defrost_cm_step[MAX_COMP];          // 除霜停压机阶段
DEFROST_EXT U16 defrost_cm_time[MAX_COMP];          // 除霜停压机计时
DEFROST_EXT U08 defrost_cm_intv_on;                 // 除霜停压机启动间隔
DEFROST_EXT U08 defrost_cm_intv_off;                // 除霜停压机停机间隔

DEFROST_EXT void defrost_init(void);
DEFROST_EXT void defrost(void);

DEFROST_EXT I16 defrost_EWD_except_diff(U08 cm);
DEFROST_EXT void defrost_EWD_except_reset(U08 cm);

DEFROST_EXT I16 defrost_get_value(U08 cm, U08 val_type);

DEFROST_EXT I32 defrost_cal_power(I32 water_diff);

#endif /* __DEFROST_H__ */

