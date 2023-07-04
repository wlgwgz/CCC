/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: Test_fac.h
 文件描述: 
 创建人  : 
 创建日期: 
*******************************************************************************/
#ifndef TEST_FAC_H
#define TEST_FAC_H

  
#ifdef  TEST_FAC_GLOBAL
 #define TF_EXT
#else
 #define TF_EXT extern
#endif


/* 商检内容:更改此结构体需与商检屏程序同步更新 */
#define TF_SRC_NUM 15
typedef struct TEST_FAC
{
    /* 屏可写 */
    U16 type;				/* 类型：0无商检；1内机商检；2外机商检；3整机商检 */
    U16 cmd;				/* 命令：bit：0商检、1高压开关、2继电器、3WiFi、4NTC */
    U16 sw;					/* 拨码开关 */
    U16 addr;				/* 拨盘地址 */

    /* 屏只读 */
    U16 err;				/* 故障：0无故障；1有故障，商检屏需处理故障音 */
    U16 str_ver_inner[32];  /* 内机版本号 */
    U16 str_ver_outer[32];  /* 外机版本号 */
    U16 str_mac[32];		/* 显示机型 */
    U16 str_sta[32];		/* 显示文本：商检状态 */
    U16 str_err[32];		/* 显示文本：商检故障 */

    struct
    {
        U16 str[8];			/* 名称:7字符+1结束符 */
        I16 val;			/* 值 */
        U16 attrib;			/* 属性,参照SH_sign_local2sh() */
    }src[TF_SRC_NUM];

}TEST_FAC_STRU;
TF_EXT TEST_FAC_STRU test_fac;

/* 商检类型 */
#define TF_TYPE_IDLE 0			/* 无商检 */
#define TF_TYPE_IN   1			/* 内机商检 */
#define TF_TYPE_OUT  2			/* 外机商检 */
#define TF_TYPE_ALL  3			/* 整机商检 */
#define TF_IS_IDLE              (test_fac.type == TF_TYPE_IDLE)
#define TF_IS_IN                (test_fac.type == TF_TYPE_IN)	
#define TF_IS_OUT               (test_fac.type == TF_TYPE_OUT)	
#define TF_IS_ALL               (test_fac.type == TF_TYPE_ALL)	

#define TF_HAS_INNER     (TF_IS_IN  || TF_IS_ALL) /* 内机或整机商检 */
#define TF_HAS_OUTER     (TF_IS_OUT || TF_IS_ALL) /* 外机或整机商检 */

/* 命令 */
#define TF_EN                   (GETBIT(test_fac.cmd, 0))	        /* 进入商检 */
#define TF_EN_HDI               (GETBIT(test_fac.cmd, 1))	        /* 高压开关 */    
#define TF_EN_DO                (GETBIT(test_fac.cmd, 2))	        /* 继电器 */   
#define TF_EN_WIFI              (GETBIT(test_fac.cmd, 3))	        /* WiFi */     
#define TF_EN_NTC               (GETBIT(test_fac.cmd, 4))	        /* 强检NTC */ 

#define TF_VAL(v1, v2)     (TF_EN ? (v2) : (v1))   /* v1：正常；v2：商检使用值 */
#define TF_START_INNER     (TF_EN && TF_HAS_INNER) /* 启动内机商检 */
#define TF_START_OUTER     (TF_EN && TF_HAS_OUTER) /* 启动外机商检 */

#define TF_OPEN_IBH     (TF_START_INNER) /* 商检开电热 */
#define TF_OPEN_PUMP    (TF_START_INNER) /* 商检开水泵 */
#define TF_OPEN_COMP    (TF_START_OUTER) /* 商检开压机 */

#define TF_PUMP_ON_DLY  5	/* 水流开关检测延时 */
#define TF_ERR_FLOW_DLY 10	/* 水流开关报警延时 */

#define TF_FAN_ON_DLY   0	/* 开风机延时 */
#define TF_4V_ON_DLY    0	/* 开四通阀延时 */

#define TF_LP_CHK_DLY           0	/* 低压检测延时 */
#define TF_LP_ERR_DLY           30	/* 低压消抖延时 */

TF_EXT I16 TF_comp_freq;
TF_EXT I16 TF_fan_freq;
TF_EXT I16 TF_eev_step;
TF_EXT U16 TF_wifi_sta;

void TF_task(void);

#endif
