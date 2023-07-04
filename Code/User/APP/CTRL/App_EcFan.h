/******************************************************************************

          版权所有 (C), 2001-2016, 广州市邦普电脑技术开发有限公司

 ******************************************************************************
  文 件 名   : App_EcFan.h
  版 本 号   : 初稿
  作    者   : zzp
  生成日期   : 2019年8月21日
  最近修改   :
  功能描述   : App_EcFan.c 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2019年8月21日
    作    者   : zzp
    修改内容   : 创建文件

******************************************************************************/

#ifndef __APP_ECFAN_H__
#define __APP_ECFAN_H__


#ifdef GBL_ECFAN
    #define ECFAN_EXT
#else
    #define ECFAN_EXT extern
#endif  

/*-----------------接口------------------------*/
/* 使用风机个数 */
#define NUM_ECFAN_USE       val_freq_fan_num

#define COM_EC_FAN          COM_INVT

#define pv_ec_fan_now(ec)   (pv_cell_fan_now(ec))
#define pv_ec_fan_set(ec)   (pv_cell_fan_set(ec))
#define pv_ec_warn(ec)	    (pv_cell_EC_warn(ec))

#define ec_set_error(err_num) (s_set_error(err_num))
#define ec_clr_error(err_num) (s_clr_error(err_num))
#define ec_get_error(err_num) (s_get_error(err_num))

// 风机型号
#define val_ec_fan_drive        val_freq_fan_set
#define EC_FAN_DRIVE_PUNP       FREQ_FAN_SET_EC         // 邦普
#define EC_FAN_DRIVE_WK         FREQ_FAN_SET_EC_KeMao   // 维克:用的科贸EC风机
#define EC_FAN_DRIVE_ZSFN       FREQ_FAN_SET_EC_ZSFN    // 顺德四格机电风机	/* N0004 */

/*-----------------内部------------------------*/

typedef enum
{
    ECFAN_1,         
    ECFAN_2,		
    ECFAN_MAX     
} ECFAN_ENUM;

#define MAX_EC_ADDR			(247)	 	/* EC通讯地址最大值 */

//4 EC风机通信数组(要保证长度够放通信数据)
ECFAN_EXT I16 EcFan_rd_array[ECFAN_MAX][15];/* N0004 */
ECFAN_EXT I16 EcFan_wr_array[ECFAN_MAX][15];

/* 以下适配不同风机协议 */
//4 维克EC风机通信地址定义(地址是+1地址)
#define ECFAN_WK_ADDR_RD_STA       2001
#define ECFAN_WK_ADDR_WR_CTRL      1002
#define ECFAN_WK_ADDR_WR_RST       1019
#define EcFan_wk_rd_sta(ec, addr)   EcFan_rd_array[ec][addr-ECFAN_WK_ADDR_RD_STA]
#define EcFan_wk_wr_ctrl(ec, addr)  EcFan_wr_array[ec][addr-ECFAN_WK_ADDR_WR_CTRL]
/* N0004 */
//4 顺德四格机电EC风机通信地址定义(地址是+0地址)
#define ECFAN_ZSFN_ADDR_RD_STA       1011
#define ECFAN_ZSFN_ADDR_WR_CTRL      1011
//#define ECFAN_ZSFN_ADDR_WR_RST       1019
#define EcFan_zsfn_rd_sta(ec, addr)   EcFan_rd_array[ec][addr-ECFAN_ZSFN_ADDR_RD_STA]
#define EcFan_zsfn_wr_ctrl(ec, addr)  EcFan_wr_array[ec][addr-ECFAN_ZSFN_ADDR_WR_CTRL]

typedef struct tagECFAN
{
    I16 speed;      // RPM
    U16 err_word[2];
    U16 warn_word[2];
}ECFAN;
ECFAN_EXT ECFAN ECFan[ECFAN_MAX];

#define EC_FAN_COMMU_INTERVAL 10			    /* 通讯间隔 */


/*-----------------外部------------------------*/

ECFAN_EXT U16 EcFan_commu_err;

ECFAN_EXT void ec_fan(void);
ECFAN_EXT void ec_fan_init(void);
ECFAN_EXT void ec_fan_rd_update(void);
ECFAN_EXT BOL ec_fan_commu(void);
ECFAN_EXT void ec_fan_err_reset(void);
ECFAN_EXT void ec_fan_time(void);
ECFAN_EXT BOL ec_fan_commu_rd_sta(U08 ec);
ECFAN_EXT BOL ec_fan_commu_wr_ctrl(U08 ec);

#endif /* __APP_ECFAN_H__ */

