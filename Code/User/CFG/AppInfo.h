/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: AppInfo.h
 文件描述: 测试功能码定义
 创建人  :
 创建日期:
*******************************************************************************/
//----------------------TY--BGN------------------------------------//

//----------------------TY---END-----------------------------------//
#ifndef __APPINFO_H__
#define __APPINFO_H__

//#define USE_GBK				/* 传给彩屏GBK编码 */
#define USE_UNICODE			/* 传给彩屏UNICODE编码 */

//#define USE_HMI_DISP				/* 使用文本屏显示 */

/* 调试，发布时必须屏蔽 */
//#define USE_DEBUG
//#define USE_LAB			    /* 实验室出差调试用 */
//#define USE_HIT               /* 打机测试,用于可靠性实验 */

/* 使用WorkFlash 32K的方法:
    1.当程序空间超过MainFlash大小时,启动WorkFlash空间.
    2.开放USE_WORK_FLASH_32KB宏定义
    3.更改Keil工程Linker->ScatterFile选择s6e2h14_256KB_32KB.sct
    4.若MainFlash开放到512KB,则对应选择512KB的sct.
*/
//#define USE_WORK_FLASH_32KB         /* 需要使用WorkFlash的32KB时定义 */

#ifdef USE_DEBUG
// 开发常用调试
//    #define USE_BMS_SOURCE      // 使用BMS仿真硬件资源
    #define USE_QUICK_TIME	    /* 使用快速时间替代默认时间，减少测试等待 */
    #define USE_NO_SPEAKER      // 屏蔽蜂鸣器

// 功能无影响调试
//    #define USE_NOT_LOCK        // 使用不自动锁屏
    #define USE_GLOBAL_VALUE    /* 将static改为全局变量，便于仿真 */
   	#define USE_DIS_NEXCH	    /* 调试模式时，关闭参数禁止修改功能 */
    #define USE_DEBUG_VER       // 调试版本
    #define USE_DISP_DBGVER     // 使用显示调试版本信息,在开机画面

// 特殊功能调试
    #define USE_NO_PARA_RESET /* eeprom错误不重置参数值 */
//  #define USE_HDW_DEBUG       // 硬件调试
//	#define USE_LOG_PRINT       //使用打印参数字符串信息
//    #define USE_WF_TEST
    #define USE_PUMPI_DEBUG     // 变频水泵调试
//    #define USE_OUT_UP_DBG        // 制热出温上限调试
#endif

#define USE_SPECIAL_READ	/* BMS读FLASH、EEPROM */
#define USE_BMS_DEBUG       /* BMS调试 */
#define USE_COMMU_TEST_READ	/* BMS读通讯测试信息 */

//#define USE_KIT_PARA_FAC	/* 启用多机型参数套件功能 */
//#define USE_QUICK_TIME
//#define USE_MAC_POWER		/* 启用能力计量功能 */

#if defined(USE_DEBUG_VER) || defined(USE_KIT_PARA_FAC)
#define DEBUG_VER   "dbg"
#else
#define DEBUG_VER   ""
#endif

/* 放到KEIL定义 */
//#define M_DEBUG             /* 内部调试 */
//#define M_TCL               /* 中山TCL(暂作为HL080A通用版本) */
//#define M_CHANGHONG         /* 中山长虹 */
//#define M_TONGYONG          /* 通用(HL087A通用版本) */
//#define M_DISEN             /* 广州迪森 */
//#define M_PAIWO             /* 广东派沃 */
//#define M_NIUENTAI          /* 广东纽恩泰 */
//#define M_SHENGNENG         /* 浙江生能 */
//#define M_HONGHE            /* 佛山鸿禾 */
//#define M_WANHE             /* 万和集团 */
//#define M_CHANGFAN          /* 广东长帆 */

/* 内部版本号 */
#define BUILD_VER           "v0.0.37-a"
 
/* 相关版本信息 */
#define MAX_VER             32
#define BOOT_VER            "HL055C"        /* BOOT型号 */

/* 微码号主干&版本号 */
#if defined(M_DEBUG)        
    #define SFW_STEM        "HL080A.K05.001-0"              /* 内部调试 */
    #define SFW_SUFFIX      "V100B10A"                      /* 内部调试版本 */
    #define USE_QUICK_TIME                                  /* 使用快速时间替代默认时间，减少测试等待 */
    #define USE_BMS_DEBUG_PUMPF_ERR                         /* 使用BMS手动触发变频水泵故障 */
#elif defined(M_TCL)        
    #define SFW_STEM        "HL080A.K05.001-1"              /* 中山TCL */
    #define SFW_SUFFIX      "V100B03A"                      /* 发布/生产版本 */
#elif defined(M_CHANGHONG)  
    #define SFW_STEM        "HL080A.K05.501-1"              /* 中山长虹 */
    #define SFW_SUFFIX      "V100B02B"                      /* 发布/生产版本 */
#elif defined(M_TONGYONG)   
    #define SFW_STEM        "HL087A.K05.001-1"              /* 通用 */
    #define SFW_SUFFIX      "V100B02A"                      /* 发布/生产版本 */
#elif defined(M_PAIWO)      
    #define SFW_STEM        "HL087A.K05.501-1"              /* 广东派沃 */
    #define SFW_SUFFIX      "V100B02A"                      /* 发布/生产版本 */
#elif defined(M_DISEN)      
    #define SFW_STEM        "HL087A.K05.502-1"              /* 广州迪森 */
    #define SFW_SUFFIX      "V100B04D"                      /* 发布/生产版本 */
#elif defined(M_NIUENTAI)
    #define SFW_STEM        "HL087A.K05.503-1"              /* 广东纽恩泰 */
    #define SFW_SUFFIX      "V100B04A"                      /* 发布/生产版本 */
#elif defined(M_SHENGNENG)
    #define SFW_STEM        "HL087A.K05.504-1"              /* 浙江生能 */
    #define SFW_SUFFIX      "V100B01E"                      /* 发布/生产版本 */
#elif defined(M_HONGHE)
    #define SFW_STEM        "HL087A.K05.505-1"              /* 佛山鸿禾 */
    #define SFW_SUFFIX      "V100B00D"                      /* 发布/生产版本 */
#elif defined(M_WANHE)   
    #define SFW_STEM        "HL087A.K05.506-1"              /* 万和集团 */
    #define SFW_SUFFIX      "V100B01E"                      /* 发布/生产版本 */
#elif defined(M_CHANGFAN)   
    #define SFW_STEM        "HL087A.K05.507-1"              /* 广东长帆 */
    #define SFW_SUFFIX      "V100B02B"                      /* 发布/生产版本 */
#else
    #error "未定义厂家!"
#endif

/* 条件编译功能 */
#if defined(M_DEBUG)                /* 内部调试 */
    #define USE_SW_MAC              /* 使用拨码设置机型 */
    #define USE_ODU_HL081B          /* 使用HL081B外机板 */
    #define USE_AT_EEVS             /* 能力测试使用辅阀手动控制 */
    #define USE_NEW_FORCE_IBH       /* 使用新强开辅助电热逻辑 */
#elif defined(M_TCL)                /* 中山TCL */
    #define USE_SW_MAC              /* 使用拨码设置机型 */
    #define USE_AREA_B_FIX          /* 使用B区固定范围 */
    #define USE_WF_OLD              /* 使用旧工况频率表 */
    #define USE_AT_SV_FIX           /* 使用能力测试固定目标温度 */
#elif defined(M_TONGYONG)           /* 通用 */
    #define USE_ODU_HL081B          /* 使用HL081B外机板 */
    #define USE_AT_EEVS             /* 能力测试使用辅阀手动控制 */
    #define USE_FREQ_LIMIT_TC_TE    /* 使用冷凝温度和蒸发温度完整逻辑 */
    #define USE_NEW_FORCE_IBH       /* 使用新强开辅助电热逻辑 */
#elif defined(M_CHANGHONG)          /* 中山长虹 */
    #define USE_WF_OLD              /* 使用旧工况频率表 */
    #define USE_AT_SV_FIX           /* 使用能力测试固定目标温度 */
    #define USE_NEW_FORCE_IBH       /* 使用新强开辅助电热逻辑 */
#elif defined(M_PAIWO)              /* 广东派沃 */
    #define USE_SW_MAC              /* 使用拨码设置机型 */
    #define USE_ODU_HL081B          /* 使用HL081B外机板 */
    #define USE_WF_OLD              /* 使用旧工况频率表 */    
    #define USE_AT_SV_FIX           /* 使用能力测试固定目标温度 */
#elif defined(M_DISEN)              /* 广州迪森 */
    #define USE_SW_MAC              /* 使用拨码设置机型 */
    #define USE_ODU_HL081B          /* 使用HL081B外机板 */
    #define USE_AT_SV_FIX           /* 使用能力测试固定目标温度 */
#elif defined(M_NIUENTAI)           /* 广东纽恩泰 */
    #define USE_SW_MAC              /* 使用拨码设置机型 */
    #define USE_ODU_HL081B          /* 使用HL081B外机板 */
    #define USE_AT_SV_FIX           /* 使用能力测试固定目标温度 */
#elif defined(M_SHENGNENG)          /* 浙江生能 */
    #define USE_WF_SHENGNENG        /* 使用生能工况频率表 */
    #define USE_SW_MAC              /* 使用拨码设置机型 */
    #define USE_ODU_HL081B          /* 使用HL081B外机板 */
    #define USE_HT_ENV_NEW          /* 使用新制热分段环温 */
    #define USE_AT_SV_FIX           /* 使用能力测试固定目标温度 */
    #define USE_FREQ_LIMIT_TC_TE    /* 使用冷凝温度和蒸发温度完整逻辑 */
#elif defined(M_HONGHE)             /* 佛山鸿禾 */
    #define USE_SW_MAC              /* 使用拨码设置机型 */
    #define USE_ODU_HL081B          /* 使用HL081B外机板 */
    #define USE_AT_SV_FIX           /* 使用能力测试固定目标温度 */
    #define USE_FREQ_LIMIT_TC_TE    /* 使用冷凝温度和蒸发温度完整逻辑 */
#elif defined(M_WANHE)              /* 广东万和 */
    #define USE_ODU_HL081B          /* 使用HL081B外机板 */
    #define USE_AT_EEVS             /* 能力测试使用辅阀手动控制 */
    #define USE_FREQ_LIMIT_TC_TE    /* 使用冷凝温度和蒸发温度完整逻辑 */
    #define USE_NEW_FORCE_IBH       /* 使用新强开辅助电热逻辑 */
#elif defined(M_CHANGFAN)           /* 广东长帆 */
    #define USE_ODU_HL081B          /* 使用HL081B外机板 */
    #define USE_AT_EEVS             /* 能力测试使用辅阀手动控制 */
    #define USE_FREQ_LIMIT_TC_TE    /* 使用冷凝温度和蒸发温度完整逻辑 */
    #define USE_NEW_FORCE_IBH       /* 使用新强开辅助电热逻辑 */
#endif

/* 使用新四段水温工况频率表，默认启用以下功能宏定义 */
#ifndef USE_WF_OLD
     #define USE_WF_R290     /* 使用R290工况频率表 */
#endif

#define APPFUNCODEVER       "X1."SFW_STEM"."SFW_SUFFIX DEBUG_VER /* 微码版本 */
#define STR_CODE            "STR."SFW_STEM                          /* 字符串库版本 */

/* 产品型号（识别码） */
#define PCB_ID_CODE		    "LK05XXXXA"
#define MAX_ID  		    (sizeof(PCB_ID_CODE) / sizeof(U08))

#define FALSH_ADDR_BASE     (0x00000000)
#define IPA_BASE_ADDR       (FALSH_ADDR_BASE)
#define IAP_SIZE            (0x4000)
#define BOOT_VER_ADDR       (FALSH_ADDR_BASE + 0x3F80)    /* BOOT的版本地址 */
#define APP_INFO_ADDR       (IPA_BASE_ADDR + IAP_SIZE)    /* INFO信息地址 */
#define APP_VECT_TAB_OFFSET (FALSH_ADDR_BASE + 0x4200)    /* APP中断向量地址 */


#define FLASH_WORK_BASE_ADDR (0x200C0000)
#define SEC_INFO_W32KB       (FLASH_WORK_BASE_ADDR)       /* 段信息:WorkFlash 32KB */

typedef struct {
    unsigned char info[8];  // 段信息头,固定为字符串"SECTADDR"
    unsigned long addr;     // 段首地址
    unsigned long naddr;    // 段首地址补码
}SECTINFO;


//#define CHG_VER			"2018/03/29"

#ifndef USE_QUICK_TIME
#define QK_5        5
#define QK_60       60
#define QK_3600     3600
#else
#define QK_5        (fg_dbg_slow?5:1)
#define QK_60       (fg_dbg_slow?60:1)
#define QK_3600     (fg_dbg_slow?3600:60)
#endif

/*******************************************************************************
特殊处理统一备注

 --> N0001:去除编译警告
 --> N0002:伽蒂芙兼容HL051B
 --> N0003:生能调试特殊处理，调试完成恢复原状
 --> N0004:与通用X1.HL051A.K05.002-1有区别的地方
 --> N0005:！！！变频水泵开启需求逻辑待优化

*******************************************************************************/


#endif
