/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: SmartHmi_commu_info.h
 文件描述: 通讯信息定义
 创建人  : hzy
 创建日期: 2016.07.15
*******************************************************************************/

#ifndef SMARTHMI_H
#define SMARTHMI_H

#ifdef  SMARTHMI_GLOBAL
    #define SH_EXT
#else
    #define SH_EXT extern
#endif

typedef struct tagDI_DO_INFO
{
    U16 sys[2];         //系统逻辑开关量
    U16 cell;           //单元逻辑开关量
    U16 comp[MAX_COMP]; //压机逻辑开关量,每个压机1个字
}DI_DO_INFO;

SH_EXT U16 SH_hmi_commu_succ;   //前板通讯成功计数(用于通讯自适应)

/* 1s计时 */
enum
{
    SH_TIME_1S_STATUS,   // 状态显示
    SH_TIME_1S_MAX
};

SH_EXT SH_CHK_TEST test_data;

SH_EXT U16 SH_timer_1s[SH_TIME_1S_MAX];
SH_EXT U16 SH_cal_grp_attrib(U08 grp);
SH_EXT U16 SH_sign_local2sh(U16 sign_local);
SH_EXT U16 SH_attr_local2sh(U16 sign_local, const U16 *dic_str);
SH_EXT U16 SH_cal_para_attrib(U08 grp, U08 item);
SH_EXT void SH_MB_commu_adjust(void);
SH_EXT void SH_MB_DealUartSlave(U08 u);

SH_EXT void SH_timer(void);
SH_EXT void SH_init(void);
SH_EXT BOOL SHC_get_curt_err(SH_ERR_CURT *err, U16 *sys_err_id, U16 *mod_id, U16 *mod_err_id);
SH_EXT void SHC_get_his_err_info(SH_ERR_HIS *err, U08 idx);
SH_EXT U16 SHC_get_err_num(void);
SH_EXT BOOL SHC_set_sys_test_do(void);
SH_EXT BOOL SHC_set_mod_test_do(void);

SH_EXT I16 SHC_get_set_tmp_A(void);
SH_EXT I16 SHC_get_set_tmp_B(void);
SH_EXT I16 SHC_get_ctrl_tmp_B(void);
SH_EXT I16 SHC_get_set_tmp_max_A(void);
SH_EXT I16 SHC_get_set_tmp_min_A(void);
SH_EXT I16 SHC_get_set_tmp_max_B(void);
SH_EXT I16 SHC_get_set_tmp_min_B(void);
SH_EXT I16 SHC_get_hot_tmp_set_max(void);
SH_EXT I16 SHC_get_hot_tmp_set_min(void);

SH_EXT void SHC_command_ON_zone_1_deal(U16 command);
SH_EXT void SHC_command_OFF_zone_1_deal(U16 command);
SH_EXT void SHC_command_ON_zone_2_deal(U16 command);
SH_EXT void SHC_command_OFF_zone_2_deal(U16 command);
SH_EXT void SHC_command_ON_hotwater_deal(U16 command);
SH_EXT void SHC_command_OFF_hotwater_deal(U16 command);
SH_EXT BOOL SHC_set_tmp_zone_1_deal(I16 set_data);
SH_EXT BOOL SHC_set_tmp_zone_2_deal(I16 set_data);
SH_EXT BOOL SHC_set_tmp_zone_hotw_deal(I16 set_data);
SH_EXT BOOL SH_set_clock(U08 *src_buf, U16 offset, U16 num);
SH_EXT BOOL SH_set_room_temp(U08 *src_buf, U16 data_num);
SH_EXT BOOL SH_get_his_error(U08 *dest_buf, U16 offset, U16 num);

SH_EXT BOL SHC_ac_mode_switch_disabled(void);

#endif

