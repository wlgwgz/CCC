/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: pumpf.h
 文件描述: 变频水泵功能头文件
 创建人  : J.H.
 创建日期: 2022.07.10
*******************************************************************************/

#ifndef __PUMPF_H__
#define __PUMPF_H__

#ifdef PUMPF_CTRL_GLOBAL
    #define PUMPF_EXT
#else
    #define PUMPF_EXT extern
#endif

/* 控制 */
#define PUMPF_CTRL_OFF                           (0)		/* 禁用 */
#define PUMPF_CTRL_RPM                           (1)		/* 控转速 */
#define PUMPF_CTRL_FLOW                          (2)		/* 控流量 */
#define PUMPF_CTRL_PWR_ON_ONLY                   (3)		/* 仅得电 */
#define PUMPF_CTRL_POWER                         (4)		/* 控功率 */
#define PUMPF_CTRL_MAX                           (5)

/* 水泵信息缺失定义 */
#define PUMPF_RPM_NOEXIST                        (8888)		/* 转速信息缺失 */
#define PUMPF_PWM_IN_NOEXIST                     (9999)		/* PWM反馈信号信息缺失 */

/* 型号 */
#define PUMPF_00_PARA_9_IPWM1                    (0)        /* 威乐 Para 15-130/9-87/iPWM1-12 */
#define PUMPF_01_APM25_9_130_PWM                 (1)        /* 新界 APM25-9-130PWM */
#define PUMPF_02_GPA25_9HW_130                   (2)        /* 新沪 GPA25-9HW/130 */
#define PUMPF_03_APF25_12_130FPWM1               (3)		/* 新界 APF25-12-130FPWM1 */
#define PUMPF_04_APF25_12_130PWM1                (4)		/* 新界 APF25-12-130PWM1 */
#define PUMPF_05_GPA25_11H                       (5)		/* 新沪 GPA25_11H1 */
#define PUMPF_06_HBG_25_125_130P                 (6)		/* 奥姆特 HBG-25-125-130P */
#define PUMPF_07_UPM4XLK_25_90_130               (7)		/* 格兰富 UPM4XLK 25-90 130 */
#define PUMPF_MODEL_MAX                          (8)

/* 00: 威乐 Para 15-130/9-87/iPWM1-12 */
/* 转速范围 */
#define PUMPF_00_RPM_MIN                         (500)      /* rpm */
#define PUMPF_00_RPM_MAX                         (4540)     /* rpm */
/* 流量范围 */
#define PUMPF_00_FLOW_MIN                        (0)        /* L/H */
#define PUMPF_00_FLOW_MAX                        (4500)     /* L/H */
/* PWM控制 */
#define PUMPF_00_PWM_OUT_RPM_MAX_BGN             (0)        /* >0%: 最高转速运行起始点 */
#define PUMPF_00_PWM_OUT_RPM_MAX_END             (50)       /* <=5%: 最高转速运行结束点 */
#define PUMPF_00_PWM_OUT_RPM_MIN_BGN             (850)      /* >85%: 最低转速运行起始点 */
#define PUMPF_00_PWM_OUT_RPM_MIN_END             (880)      /* <=88%: 最低转速运行结束点 */
#define PUMPF_00_PWM_OUT_IDLE                    (980)      /* 93～100%: 待机(停机)，停止运行 */
/* PWM反馈 */
#define PUMPF_00_PWM_IN_SC                       (0)        /* 0%: iPWM信号接口短路 */
#define PUMPF_00_PWM_IN_IDLE                     (20)       /* 2%: 待机（停机），准备运行 */
#define PUMPF_00_PWM_IN_FLOW_INFO_MIN            (50)       /* 5%: 流量信息可用最低百分比 */
#define PUMPF_00_PWM_IN_FLOW_INFO_MAX            (750)      /* 75%: 流量信息可用最高百分比 */
#define PUMPF_00_PWM_IN_WARN                     (800)      /* 80%: 警告，未运行在最佳状态 */
#define PUMPF_00_PWM_IN_ERR_1                    (850)      /* 85%: 报警（停机），欠压/过压/异常外部水流 */
#define PUMPF_00_PWM_IN_ERR_2                    (900)      /* 90%: 报警（停机），其它部件失效/存在安装碎屑/温度不佳 */
#define PUMPF_00_PWM_IN_ERR_3                    (950)      /* 95%: 报警（停机），永久损坏故障 */
#define PUMPF_00_PWM_IN_OC                       (1000)     /* 100%: iPWM信号接口断路 */

/* 01: 新界 APM25-9-130 */
/* 转速范围 */
#define PUMPF_01_RPM_MIN                         (1000)     /* rpm */
#define PUMPF_01_RPM_MAX                         (4500)     /* rpm */
/* 流量范围 */
#define PUMPF_01_FLOW_MIN                        (300)      /* L/H */
#define PUMPF_01_FLOW_MAX                        (4500)     /* L/H */
/* PWM控制 */
#define PUMPF_01_PWM_OUT_RPM_MAX_BGN             (0)        /* >0%: 最高转速运行起始点 */
#define PUMPF_01_PWM_OUT_RPM_MAX_END             (50)       /* <=5%: 最高转速运行结束点 */
#define PUMPF_01_PWM_OUT_RPM_MIN_BGN             (850)      /* >85%: 最低转速运行起始点 */
#define PUMPF_01_PWM_OUT_RPM_MIN_END             (880)      /* <=88%: 最低转速运行结束点 */
#define PUMPF_01_PWM_OUT_IDLE                    (980)      /* 93～100%: 待机(停机)，停止运行 */
/* PWM反馈 */
#define PUMPF_01_PWM_IN_SC                       (0)        /* 0%: PWM反馈信号缺失 */
#define PUMPF_01_PWM_IN_IDLE                     (20)       /* 2%: 待机(停机)，准备运行 */
#define PUMPF_01_PWM_IN_NO_WATER_RUN             (30)       /* 3%: 空转或故障重启 */
#define PUMPF_01_PWM_IN_FLOW_INFO_MIN            (50)       /* 5%: 流量信息可用最低百分比 */
#define PUMPF_01_PWM_IN_FLOW_INFO_MAX            (700)      /* 70%: 流量信息可用最高百分比 */
#define PUMPF_01_PWM_IN_WARN                     (800)      /* 80%: 警告，低电压/高电压 */
#define PUMPF_01_PWM_IN_ERR_1                    (850)      /* 85%: 报警(停机)，欠压/过压故障 */
#define PUMPF_01_PWM_IN_ERR_2                    (900)      /* 90%: 报警(停机)，堵转/干转故障 */
#define PUMPF_01_PWM_IN_ERR_3                    (950)      /* 95%: 报警(停机)，缺相/过流故障 */
#define PUMPF_01_PWM_IN_OC                       (1000)     /* 100%: 无PWM信号输入 */

/* 02: 新沪 GPA25-9HW/130 */
/* 转速范围 */
#define PUMPF_02_RPM_MIN                         (1200)     /* rpm */
#define PUMPF_02_RPM_MAX                         (6900)     /* rpm */
/* 流量范围 */
#define PUMPF_02_FLOW_MIN                        (0)        /* L/H */
#define PUMPF_02_FLOW_MAX                        (3200)     /* L/H */
/* PWM控制 */
#define PUMPF_02_PWM_OUT_RPM_MAX_BGN             (0)        /* >0%: 最高转速运行起始点 */
#define PUMPF_02_PWM_OUT_RPM_MAX_END             (50)       /* <=5%: 最高转速运行结束点 */
#define PUMPF_02_PWM_OUT_RPM_MIN_BGN             (850)      /* >85%: 最低转速运行起始点 */
#define PUMPF_02_PWM_OUT_RPM_MIN_END             (880)      /* <=88%: 最低转速运行结束点 */
#define PUMPF_02_PWM_OUT_IDLE                    (980)      /* 93～99%: 待机(停机)，停止运行 */
/* PWM反馈 */
#define PUMPF_02_PWM_IN_SC                       (0)        /* 0%: iPWM信号接口短路 */
#define PUMPF_02_PWM_IN_IDLE                     (20)       /* 2%: 待机（停机），准备运行 */
#define PUMPF_02_PWM_IN_FLOW_INFO_MIN            (60)       /* 6%: 流量信息可用最低百分比 */
#define PUMPF_02_PWM_IN_FLOW_INFO_MAX            (500)      /* 50%: 流量信息可用最高百分比 */
#define PUMPF_02_PWM_IN_WARN                     (800)      /* 80%: 警告，未运行在最佳状态 */
#define PUMPF_02_PWM_IN_ERR_1                    (850)      /* 85%: 报警（停机），欠压/过压/异常外部水流 */
#define PUMPF_02_PWM_IN_ERR_2                    (900)      /* 90%: 报警（停机），水泵处于轻载，缺相/过流/过温保护 */
#define PUMPF_02_PWM_IN_ERR_3                    (950)      /* 95%: 报警（停机），永久停机(水泵堵转) */
#define PUMPF_02_PWM_IN_OC                       (1000)     /* 100%: 无PWM信号输入 */

/* 03: 新界 APM25-12-130FPWM1 */
/* 转速范围 */
#define PUMPF_03_RPM_MIN                         (1750)                 /* rpm */
#define PUMPF_03_RPM_MAX                         (5500)                 /* rpm */
/* 流量范围 */
#define PUMPF_03_FLOW_MIN                        (0)                    /* L/H */
#define PUMPF_03_FLOW_MAX                        (4000)                 /* L/H */
/* PWM控制 */
#define PUMPF_03_PWM_OUT_RPM_MAX_BGN             (0)                    /* >0%: 最高转速运行起始点 */
#define PUMPF_03_PWM_OUT_RPM_MAX_END             (100)                  /* <=10%: 最高转速运行结束点 */
#define PUMPF_03_PWM_OUT_RPM_MIN_BGN             (840)                  /* >84%: 最低转速运行起始点 */
#define PUMPF_03_PWM_OUT_RPM_MIN_END             (910)                  /* <=91%: 最低转速运行结束点 */
#define PUMPF_03_PWM_OUT_IDLE                    (980)                  /* 95～100%: 待机(停机)，停止运行 */
/* PWM反馈 */
#define PUMPF_03_PWM_IN_SC                       (PUMPF_PWM_IN_NOEXIST)	/* PWM反馈信号缺失[无] */
#define PUMPF_03_PWM_IN_IDLE                     (950)                  /* 95%: 待机(停机)，准备运行 */
#define PUMPF_03_PWM_IN_FLOW_INFO_MIN            (0)                    /* 0%: 流量信息可用最低百分比 */
#define PUMPF_03_PWM_IN_FLOW_INFO_MAX            (700)                  /* 70%: 流量信息可用最高百分比 */
#define PUMPF_03_PWM_IN_WARN                     (750)                  /* 75%: 警告，低电压/高电压 */
#define PUMPF_03_PWM_IN_ERR_1                    (800)                  /* 80%: 报警(停机)，欠压/过压故障 */
#define PUMPF_03_PWM_IN_ERR_2                    (900)                  /* 90%: 报警(停机)，堵转/干转故障 */
#define PUMPF_03_PWM_IN_ERR_3                    (850)                  /* 85%: 报警(停机)，缺相/过流故障 */
#define PUMPF_03_PWM_IN_OC                       (1000)	                /* 无PWM信号输入[无] */

/* 04: 新界 APF25-12-130PWM1 */
/* 转速范围 */
#define PUMPF_04_RPM_MIN                         (1000)                 /* rpm */
#define PUMPF_04_RPM_MAX                         (5300)                 /* rpm */
/* 功率范围 */
#define PUMPF_04_POW_MIN                         (0)                    /* W */
#define PUMPF_04_POW_MAX                         (180)                  /* W */
/* PWM控制 */
#define PUMPF_04_PWM_OUT_RPM_MAX_BGN             (0)                    /* >0%: 最高转速运行起始点 */
#define PUMPF_04_PWM_OUT_RPM_MAX_END             (100)                  /* <=10%: 最高转速运行结束点 */
#define PUMPF_04_PWM_OUT_RPM_MIN_BGN             (840)                  /* >84%: 最低转速运行起始点 */
#define PUMPF_04_PWM_OUT_RPM_MIN_END             (910)                  /* <=91%: 最低转速运行结束点 */
#define PUMPF_04_PWM_OUT_IDLE                    (980)                  /* 95～100%: 待机(停机)，停止运行 */
/* PWM反馈 */
#define PUMPF_04_PWM_IN_SC                       (PUMPF_PWM_IN_NOEXIST)	/* PWM反馈信号缺失[无] */
#define PUMPF_04_PWM_IN_IDLE                     (950)                  /* 95%: 待机(停机)，准备运行 */
#define PUMPF_04_PWM_IN_POWER_INFO_MIN           (0)                    /* 0%:  功率信息可用最低百分比 */
#define PUMPF_04_PWM_IN_POWER_INFO_MAX           (700)                  /* 70%: 功率信息可用最高百分比 */
#define PUMPF_04_PWM_IN_WARN                     (750)                  /* 75%: 警告，低电压/高电压 */
#define PUMPF_04_PWM_IN_ERR_1                    (PUMPF_PWM_IN_NOEXIST) /* 80%: 报警(停机)，欠压/过压故障 */
#define PUMPF_04_PWM_IN_ERR_2                    (900)                  /* 90%: 报警(停机)，堵转/干转故障 */
#define PUMPF_04_PWM_IN_ERR_3                    (850)                  /* 85%: 报警(停机)，缺相/过流故障 */
#define PUMPF_04_PWM_IN_OC                       (1000)	                /* 无PWM信号输入[无] */

/* 05: 新沪 GPA25_11H */
/* 转速范围：该款水泵规格书未注明转速，先假定，不影响控制 */
#define PUMPF_05_RPM_MIN                         (1000)                 /* rpm */
#define PUMPF_05_RPM_MAX                         (4500)                 /* rpm */
/* 流量范围 */
#define PUMPF_05_FLOW_MIN                        (0)                    /* L/H */
#define PUMPF_05_FLOW_MAX                        (5500)                 /* L/H */
/* PWM控制 */
#define PUMPF_05_PWM_OUT_RPM_MAX_BGN             (0)                    /* >0%: 最高转速运行起始点 */
#define PUMPF_05_PWM_OUT_RPM_MAX_END             (100)                  /* <=10%: 最高转速运行结束点 */
#define PUMPF_05_PWM_OUT_RPM_MIN_BGN             (840)                  /* >84%: 最低转速运行起始点 */
#define PUMPF_05_PWM_OUT_RPM_MIN_END             (900)                  /* <=90%: 最低转速运行结束点 */
#define PUMPF_05_PWM_OUT_IDLE                    (980)                  /* 95～100%: 待机(停机)，停止运行 */
/* PWM反馈 */
#define PUMPF_05_PWM_IN_SC                       (PUMPF_PWM_IN_NOEXIST)	/* PWM反馈信号缺失[无] */
#define PUMPF_05_PWM_IN_IDLE                     (950)                  /* 95%: 待机(停机)，准备运行 */
#define PUMPF_05_PWM_IN_FLOW_INFO_MIN            (0)                    /* 0%: 流量信息可用最低百分比 */
#define PUMPF_05_PWM_IN_FLOW_INFO_MAX            (700)                  /* 70%: 流量信息可用最高百分比 */
#define PUMPF_05_PWM_IN_WARN                     (750)                  /* 75%: 警告，低电压/高电压 */
#define PUMPF_05_PWM_IN_ERR_1                    (PUMPF_PWM_IN_NOEXIST) /* 无PWM信号输入[无] */
#define PUMPF_05_PWM_IN_ERR_2                    (900)                  /* 90%: 报警(停机)，堵转/干转故障 */
#define PUMPF_05_PWM_IN_ERR_3                    (850)                  /* 85%: 报警(停机)，电气故障 */
#define PUMPF_05_PWM_IN_OC                       (1000)	                /* 无PWM信号输入[无] */

/* 06: 奥姆特 HBG-25-125-130P */
/* 转速范围 */
#define PUMPF_06_RPM_MIN                         (1650)                 /* rpm */
#define PUMPF_06_RPM_MAX                         (4650)                 /* rpm */
/* 流量范围 */
#define PUMPF_06_FLOW_MIN                        (0)                    /* L/H */
#define PUMPF_06_FLOW_MAX                        (5998)                 /* L/H */
/* PWM控制 */
#define PUMPF_06_PWM_OUT_RPM_MAX_BGN             (0)                    /* >0%: 最高转速运行起始点 */
#define PUMPF_06_PWM_OUT_RPM_MAX_END             (50)                   /* <=5%: 最高转速运行结束点 */
#define PUMPF_06_PWM_OUT_RPM_MIN_BGN             (850)                  /* >85%: 最低转速运行起始点 */
#define PUMPF_06_PWM_OUT_RPM_MIN_END             (910)                  /* <=91%: 最低转速运行结束点 */
#define PUMPF_06_PWM_OUT_IDLE                    (980)                  /* 95～100%: 待机(停机)，停止运行 */
/* PWM反馈 */
#define PUMPF_06_PWM_IN_SC                       (PUMPF_PWM_IN_NOEXIST)	/* PWM反馈信号缺失[无] */
#define PUMPF_06_PWM_IN_IDLE                     (950)                  /* 95%: 待机(停机)，准备运行 */
#define PUMPF_06_PWM_IN_FLOW_INFO_MIN            (0)                    /* 0%: 流量信息可用最低百分比 */
#define PUMPF_06_PWM_IN_FLOW_INFO_MAX            (700)                  /* 70%: 流量信息可用最高百分比 */
#define PUMPF_06_PWM_IN_WARN                     (750)                  /* 75%: 警告，低电压/高电压 */
#define PUMPF_06_PWM_IN_ERR_1                    (800)                  /* 80%: 报警(停机)，堵转故障 */
#define PUMPF_06_PWM_IN_ERR_2                    (850)                  /* 85%: 报警(停机)，过流故障 */
#define PUMPF_06_PWM_IN_ERR_3                    (900)                  /* 90%: 报警(停机)，启动/缺相/过温故障 */
#define PUMPF_06_PWM_IN_OC                       (1000)	                /* 无PWM信号输入[无] */

/* 07: 格兰富 UPM4XLK 25-90 130 */
/* 转速范围 */
#define PUMPF_07_RPM_MIN                         (564)                  /* rpm */
#define PUMPF_07_RPM_MAX                         (6300)                 /* rpm */
/* 流量范围 */
#define PUMPF_07_FLOW_MIN                        (0)                    /* L/H */
#define PUMPF_07_FLOW_MAX                        (4000)                 /* L/H */
/* PWM控制 */
#define PUMPF_07_PWM_OUT_RPM_MAX_BGN             (0)                    /* >0%: 最高转速运行起始点 */
#define PUMPF_07_PWM_OUT_RPM_MAX_END             (100)                  /* <=10%: 最高转速运行结束点 */
#define PUMPF_07_PWM_OUT_RPM_MIN_BGN             (840)                  /* >84%: 最低转速运行起始点 */
#define PUMPF_07_PWM_OUT_RPM_MIN_END             (910)                  /* <=91%: 最低转速运行结束点 */
#define PUMPF_07_PWM_OUT_IDLE                    (980)                  /* 95～100%: 待机(停机)，停止运行 */
/* PWM反馈 */
#define PUMPF_07_PWM_IN_SC                       (PUMPF_PWM_IN_NOEXIST)	/* PWM反馈信号缺失[无] */
#define PUMPF_07_PWM_IN_IDLE                     (950)                  /* 95%: 待机(停机)，准备运行 */
#define PUMPF_07_PWM_IN_FLOW_INFO_MIN            (0)                    /* 0%: 流量信息可用最低百分比 */
#define PUMPF_07_PWM_IN_FLOW_INFO_MAX            (700)                  /* 70%: 流量信息可用最高百分比 */
#define PUMPF_07_PWM_IN_WARN                     (800)                  /* 80%: 警告，水泵干转，继续运行 */
#define PUMPF_07_PWM_IN_ERR_1                    (PUMPF_PWM_IN_NOEXIST) /* 无PWM信号输入[无] */
#define PUMPF_07_PWM_IN_ERR_2                    (850)                  /* 85%: 报警(停机)，电气故障(<150V) */
#define PUMPF_07_PWM_IN_ERR_3                    (900)                  /* 90%: 报警(停机)，水泵卡死 */
#define PUMPF_07_PWM_IN_OC                       (1000)	                /* 无PWM信号输入[无] */
#define PUMPF_07_PWM_IN_DIFF                     (10)		            /* PWM百分比回差值设置: 1% */

/* 水泵状态 */
#define PUMPF_STA_SC                             (0)		/* 短路 */
#define PUMPF_STA_IDLE                           (1)		/* 待机 */
#define PUMPF_STA_RUN                            (2)		/* 运行 */
#define PUMPF_STA_WARN                           (3)		/* 警告 */
#define PUMPF_STA_ERR                            (4)		/* 故障 */
#define PUMPF_STA_OC                             (5)		/* 断路 */

PUMPF_EXT BOL fg_PUMPf_need;                /* 变频水泵需求标志(保持) */
PUMPF_EXT BOL fg_PUMPf_long_time_no_run;	/* 变频水泵长时间未运行标志(保持) */
PUMPF_EXT BOL fg_PUMPf_test_run;			/* 变频水泵试运行标志(保持) */
PUMPF_EXT BOL fg_PUMPf_debug_run;		    /* 变频水泵调试运行标志(保持) */

PUMPF_EXT BOL fg_PUMPf_run;	                /* 变频水泵运行标志(保持) */
PUMPF_EXT U08 PUMPf_sta;	                /* 变频水泵状态 */
PUMPF_EXT RTIME PUMPf_time;	                /* 变频水泵计时 */
PUMPF_EXT U08 cnt_PUMPf_off_dly;			/* 变频水泵关闭延时计时 */
PUMPF_EXT U08 cnt_PUMPf_ctrl;				/* 变频水泵调速周期计时 */

enum {PUMF_NO_ERR, PUMPF_ERR_01, PUMPF_ERR_02, PUMPF_ERR_03, PUMPF_ERR_04, PUMPF_ERR_05, PUMPF_ERR_MAX};
PUMPF_EXT U08 PUMPf_err_id;                /* 变频水泵故障索引 */

PUMPF_EXT I16 PUMPf_pwm_out;/* PWM输出值(PWM占空比输出值:百分比) */
PUMPF_EXT I16 PUMPf_pwm_in;	/* PWM输入值(PWM占空比反馈值:百分比) */

PUMPF_EXT I16 PUMPf_rpm_sv;	/* 水泵目标转速:rpm */
PUMPF_EXT I16 PUMPf_flow_pv;/* 水泵当前流量:L/H */
PUMPF_EXT I16 PUMPf_power_pv;/* 水泵当前功率:W */

PUMPF_EXT I16 PUMPf_rpm_max;	/* 水泵最大转速:rpm */

PUMPF_EXT void PUMPF_pwm_out_deal(void);
PUMPF_EXT void PUMPF_speed_ctrl(BOL fg_on);
PUMPF_EXT void PUMPF_status(void);
PUMPF_EXT BOL PUMPF_is_warn(void);
PUMPF_EXT BOL PUMPF_is_err(void);
PUMPF_EXT BOL PUMPF_can_reset_err(void);
PUMPF_EXT void PUMPF_init(void);

#endif

