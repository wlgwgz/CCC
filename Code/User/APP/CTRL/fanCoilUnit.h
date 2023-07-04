/******************************************************************************

                  版权所有 (C), 2001-2017, 广州市邦普电脑技术开发有限公司

 ******************************************************************************
  文 件 名   : fanCoilUnit.h
  版 本 号   : 初稿
  作    者   : kjh
  生成日期   : 2021年4月7日
  最近修改   :
  功能描述   : fanCoilUnit.h 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2021年4月7日
    作    者   : kjh
    修改内容   : 创建文件

******************************************************************************/

#ifndef __FAN_COIL_UNIT_H__
#define __FAN_COIL_UNIT_H__


#ifdef GBL_FCU
#define FCU_EXT
#else
#define FCU_EXT extern
#endif

/*-----------------接口------------------------*/
/* 使用风机盘管个数:最大为MAX_FCU */
#define FCU_NUM_USE         val_fcu_num

#define FCU_COM             COM_FCU_FBUS
#define FCU_INIT_TIME       POWER_ON_INTO_WORK

/* 开机命令 */
#define FCU_CMD_RUN             (key_run = TRUE)

/* 主机设定模式 */
#define FCU_MASTER_MODE         sv_mode
#define FCU_SET_MODE_COOL       para_set_cent(P_GaI(sv_mode), SET_COOL)
#define FCU_SET_MODE_HEAT       para_set_cent(P_GaI(sv_mode), SET_HEAT)


#define fcu_set_error(err_num)  (m_set_error(MAX_PCB, err_num))
#define fcu_clr_error(err_num)  (m_clr_error(MAX_PCB, err_num))
#define fcu_get_error(err_num)  (m_get_error(MAX_PCB, err_num))

// 风机盘管类型
#define FCU_PUNP                0         // 邦普
#define fcu_type                FCU_PUNP

/* 风机盘管信号 */
FCU_EXT U16 fcu_semaphore;
#define FCU_SEM_NULL        0
#define FCU_SEM_M_START_S   1
#define FCU_SEM_M_CLOSE_S   2
//#define FCU_SEM_M_SET_MODE  3

/*-----------------内部------------------------*/

#define FCU_NUM_MAX             32		/* 更改后注意检查相关标志位是否够 */
#define FCU_ARRAY_SIZE_MAX		_DIVUCARRY(FCU_NUM_MAX, 16) /* 数组最大个数 */
#define FCU_ADDR_MAX			(247)	 	            /* FCU通讯地址最大值 */
#define FCU_ADDR_BASE			(val_fcu_base_addr)	 	/* FCU通讯地址基址 */

FCU_EXT I16 fcu_pv[FCU_NUM_MAX][10];
#define fcu_temp_indoor(ec)     (fcu_pv[ec][0])				            /* 风盘室内温度 */
#define fcu_egy_need(ec)        (fcu_pv[ec][1])				            /* 风盘能量需求 */
#define fcu_err_code(ec)	    (fcu_pv[ec][2])				            /* 风盘故障代码 */
#define fcu_sta_onoff(ec)	    (fcu_ctrl_rd(ec, FCU_ADDR_WR_ONOFF))	/* 风盘开关状态 */
#define fcu_sta_mode(ec)	    (fcu_ctrl_rd(ec, FCU_ADDR_WR_SET_MODE))	/* 风盘设定模式 */


//4 通信数组(要保证长度够放通信数据)
FCU_EXT I16 fcu_rd_array[FCU_NUM_MAX][32];/* N0004 */
FCU_EXT I16 fcu_rw_array_wr[FCU_NUM_MAX][15];
FCU_EXT I16 fcu_rw_array_rd[FCU_NUM_MAX][15];
FCU_EXT I16 fcu_rw_array_w_fg[FCU_NUM_MAX][15];

#define fcu_rd_sta(ec, addr)        fcu_rd_array[ec][addr-FCU_ADDR_RD_BASE]
#define fcu_ctrl_wr(ec, addr)       fcu_rw_array_wr[ec][addr-FCU_ADDR_WR_BASE]
#define fcu_ctrl_rd(ec, addr)       fcu_rw_array_rd[ec][addr-FCU_ADDR_WR_BASE]
#define fcu_ctrl_fg_wr(ec, addr)    fcu_rw_array_w_fg[ec][addr-FCU_ADDR_WR_BASE]

/* 以下适配不同风盘协议 */
//4 邦普风机盘管
#define FCU_ADDR_RD_BASE        0			/* 只读数据起始地址 */
#define FCU_ADDR_RD_NUM         18			/* 只读数据数量:最后数据地址-起始数据地址+1 */
#define FCU_ADDR_RD_TEMP        0			/* 室内温度 */
#define FCU_ADDR_RD_EGY         16			/* 能量需求 */
#define FCU_ADDR_RD_ERROR       17			/* 故障码 */

#define FCU_ADDR_WR_BASE        2			/* 读写数据起始地址 */
#define FCU_ADDR_WR_NUM         2			/* 读写数据数量:最后数据地址-起始数据地址+1 */
#define FCU_ADDR_WR_ONOFF       2			/* 开关机 */
#define FCU_ADDR_WR_SET_MODE    3			/* 设定模式 */
//#define FCU_ADDR_WR_RST       1019

#define FCU_MODE_COOL       	1			/* 制冷 */
#define FCU_MODE_HEAT       	2			/* 制热 */
#define FCU_MODE_VENT       	3			/* 通风 */
#define FCU_MODE_FLOOR      	4			/* 地暖 */

typedef struct tagFCU
{
    I16 temp_indoor;
    U16 err_word[2];
    U16 warn_word[2];
} S_FCU;
FCU_EXT S_FCU fcu[FCU_NUM_MAX];

/* 主末模式设置 */
#define FCU_MODE_INDEPEND   0	/* 相互独立 */
#define FCU_MODE_M_MAIN     1	/* 主机为主 */
#define FCU_MODE_S_MAIN     2	/* 末端为主 */
#define FCU_MODE_LINK       3	/* 相互联动 */
#define FCU_MODE_MAX        4
#define MODE_IS_INDEPEND    (val_fcu_mode_set == FCU_MODE_INDEPEND)
#define MODE_IS_M_MAIN      (val_fcu_mode_set == FCU_MODE_M_MAIN)
#define MODE_IS_S_MAIN      (val_fcu_mode_set == FCU_MODE_S_MAIN)
#define MODE_IS_LINK        (val_fcu_mode_set == FCU_MODE_LINK)

/* 能调修正对象 */
#define CPST_PV         0			/* 修正当前温度 */
#define CPST_SV         1			/* 修正目标温度 */
#define CPST_SET_MAX    2

/* 末端开启主机 */
#define S_START_M_NONE          0			/* 不开启 */
#define S_START_M_ONLY          1			/* 仅开主机 */
#define S_START_M_ALL           2			/* 主末全开 */
#define S_START_M_MAX           3

/* fcu位操作 */
#define GET_FCU_BIT(name, ec)	GETBIT(name[ec/16], ec%16)
#define SET_FCU_BIT(name, ec)	SETBIT(name[ec/16], ec%16)
#define CLR_FCU_BIT(name, ec)	CLRBIT(name[ec/16], ec%16)
/*-----------------外部------------------------*/

FCU_EXT U16 fcu_commu_err[FCU_ARRAY_SIZE_MAX];

FCU_EXT void fcu_deal(void);
FCU_EXT void fcu_init(void);
FCU_EXT void fcu_rd_update(void);
FCU_EXT BOL  fcu_commu(void);
FCU_EXT void fcu_err_reset(void);
FCU_EXT void fcu_time(void);
FCU_EXT BOL  fcu_has_egy_need(void);
FCU_EXT I16  fcu_energy_cpst(void);
FCU_EXT U08 fcu_state_get(U08 i);

#endif /* __FAN_COIL_UNIT_H__ */

