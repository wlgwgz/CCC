/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: inverter.c
 文件描述: 变频器通讯协议处理
 创建人  : hzy
 创建日期: 2016.12.14
*******************************************************************************/

#define INVERTER_GLOBAL
#include "includes.h"



// 变频器设备地址
//#define ADD_INVERTER_COMMU        1 
#define ADD_INVERTER_COMMU        val_invt_addr 
#define ADD_INVT_FAN_COMMU        110 /* N0004 */


// 变频器数据地址
#define INVT_ADDR_RW_PARA_F00       0x0000  //基本参数

#define INVT_ADDR_RW_CMD            0x3200  //通讯指令
//#define INVT_ADDR_RW_CMD_SIZE       10      //通讯指令大小
//#define INVT_ADDR_RW_CMD_CONTROL        (INVT_ADDR_RW_CMD+0) //控制字 
//#define INVT_ADDR_RW_CMD_SET_FREQ       (INVT_ADDR_RW_CMD+1) //给定频率
//#define INVT_ADDR_RW_CMD_EEP_SAVE       (INVT_ADDR_RW_CMD+9) //RAM中参数存入EEPROM

#define INVT_ADDR_R_STATES          0x3210 //变频器状态


//变频器通讯指令
#define CMD_CONTROL         0   // 控制字
#define CMD_SET_FREQ        1   // 给定频率
#define CMD_SET_PFC         4   // 有源PFC
#define CMD_SET_FREQ_EC1    7   // EC1给定转速
#define CMD_SET_FREQ_EC2    8   // EC2给定转速
//#define CMD_EEP_SAVE        9   // RAM中参数存入EEPROM
#define CMD_INVT_MAX       10   // 通讯指令大小
static I16 invt_cmd[INVT_ID_MAX][CMD_INVT_MAX];
//static I16 invt_cmd_bak[INVT_ID_MAX][CMD_INVT_MAX];

//参数值被更改
static U08 fg_cmd_changed[INVT_ID_MAX][CMD_INVT_MAX];



//状态信息（只读）
typedef struct tagINVT_INFO
{
    U16 state;          // 主状态字
    U16 freq_run;       // 运行转速(0.01rps)
    I16 temperature;    // 模块温度(0.1℃)
    U16 power_out;      // 输出功率(0.01kW)
    U16 freq_set;       // 给定转速(0.01rps)
    U16 current_out;    // 输出电流(0.1A)
    U16 torque_out;     // 输出转矩(0.1%)
    U16 volt_out;       // 输出电压(0.1V)
    U16 volt_generatrix;// 母线电压(0.1V)
    U16 errid;          // 故障代码
    U16 errcode1;       // 报警字1：变频输入电流
    U16 errcode2;       // 报警字2：PFC温度
    U16 freq_now;       // 当前转速(0.01rps)
    U16 freq_limit;     // 限制频率信息个位-过流限频；十位-过温限频；百位-欠压限频
    U16 bak1;           // 备用
    U16 bak2;           // 备用
    U16 bak3;           // 备用
    /* 以下为协议新增的数据，注意处理新旧协议兼容性问题 */
    U16 pulse_ec1;       // ec1脉冲数
    U16 pulse_ec2;       // ec2脉冲数
}INVT_INFO;
GSTATIC  INVT_INFO invt_info[INVT_ID_MAX];               // 变频器当前信息
#define INVT_INFO_SIZE  (sizeof(INVT_INFO)/sizeof(U16)) // 变频器状态信息大小
#define INVT_INFO_SIZE_OLD  (17) // 变频器状态信息大小-旧协议

//状态字各位含义
#define INVT_STATE_RUN      2   //运行中
#define INVT_STATE_ERR      3   //故障



//变频器命令
enum
{
    INVTCMD_ON,             //开启
    INVTCMD_OFF,            //停止
    INVTCMD_OFF_FREE,       //自由停机（立即停）
    INVTCMD_OFF_QUICK,      //急停(不推荐使用)
    INVTCMD_FREQ_LOCK,      //频率锁死 
    INVTCMD_FREQ_UNLOCK,    //频率解锁
    INVTCMD_ACCLRT_LOCK,    //变速锁死  
    INVTCMD_ACCLRT_UNLOCK,  //变速解锁  
    INVTCMD_ERR_RESET,      //故障复位
    INVTCMD_PARA_INIT,      //参数初始化
    INVTCMD_PARA_READ,      //参数读取
    INVTCMD_PFC_OFF,        //禁用有源PFC
    INVTCMD_PFC_ON,         //使用有源PFC
//    INVTCMD_PARA_SAVE       //参数保存
};
static U16 invt_cmd_ctrl[INVT_ID_MAX]; //变频器命令字



//变频器转速设置
static I16 invt_freq_set[INVT_ID_MAX];
static I16 invt_freq_set_ec[INVT_ID_MAX][FREQSPEEDNUM];



//模块计时
//100ms计时
enum
{
    INVT_TIMER_100MS_CMD_UPDATE,     // 更新命令
    INVT_TIMER_100MS_PFC_CMD_UPDATE,     // 更新PFC命令
    INVT_TIMER_100MS_MAX
};
static U16 invt_timer_100ms[INVT_ID_MAX][INVT_TIMER_100MS_MAX];
//1s计时
enum
{
    INVT_TIMER_1S_WR_FREQ,       //设定转速确认间隔
    INVT_TIMER_1S_WR_FREQ_FAN,   //设定风速确认间隔
    INVT_TIMER_1S_RESET_CHG,     //变频复位命令间隔
    INVT_TIMER_1S_MAX
};
static U16 invt_timer_1s[INVT_ID_MAX][INVT_TIMER_1S_MAX];

/*******************************************************************************
 函 数: invt_cmd_on()
 功 能: 发送开启命令
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void invt_cmd_on(INVT_ID_ENUM id)
{
    SET_BIT(invt_cmd_ctrl[id], INVTCMD_ON);
}

/*******************************************************************************
 函 数: invt_cmd_off()
 功 能: 发送停止命令
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void invt_cmd_off(INVT_ID_ENUM id)
{
    SET_BIT(invt_cmd_ctrl[id], INVTCMD_OFF);
}

/*******************************************************************************
 函 数: invt_cmd_off_free()
 功 能: 发送自由停机命令
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void invt_cmd_off_free(INVT_ID_ENUM id)
{
    SET_BIT(invt_cmd_ctrl[id], INVTCMD_OFF_FREE);
}


/*******************************************************************************
 函 数: invt_cmd_off_quick()
 功 能: 发送急停命令
 参 数: 空
 返 回: 空
 其 它: "急停"为断电停机，不推荐使用
*******************************************************************************/
void invt_cmd_off_quick(INVT_ID_ENUM id)        
{
    SET_BIT(invt_cmd_ctrl[id], INVTCMD_OFF_QUICK);
}

/*******************************************************************************
 函 数: invt_cmd_freq_lock()
 功 能: 发送频率锁死命令
 参 数: 空
 返 回: 空
 其 它: 不推荐使用
*******************************************************************************/
void invt_cmd_freq_lock(INVT_ID_ENUM id)
{
    SET_BIT(invt_cmd_ctrl[id], INVTCMD_FREQ_LOCK);
}

/*******************************************************************************
 函 数: invt_cmd_freq_unlock()
 功 能: 发送频率解锁命令
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void invt_cmd_freq_unlock(INVT_ID_ENUM id)      
{
    SET_BIT(invt_cmd_ctrl[id], INVTCMD_FREQ_UNLOCK);
}

/*******************************************************************************
 函 数: invt_cmd_acclrt_lock()
 功 能: 发送变速锁死命令
 参 数: 空
 返 回: 空
 其 它: 不推荐使用
*******************************************************************************/
void invt_cmd_acclrt_lock(INVT_ID_ENUM id)
{
    SET_BIT(invt_cmd_ctrl[id], INVTCMD_ACCLRT_LOCK);
}

/*******************************************************************************
 函 数: invt_cmd_acclrt_unlock()
 功 能: 发送变速解锁命令
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void invt_cmd_acclrt_unlock(INVT_ID_ENUM id)
{
    SET_BIT(invt_cmd_ctrl[id], INVTCMD_ACCLRT_UNLOCK);
}

/*******************************************************************************
 函 数: invt_cmd_err_reset()
 功 能: 发送故障复位命令
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void invt_cmd_err_reset(INVT_ID_ENUM id)
{
    SET_BIT(invt_cmd_ctrl[id], INVTCMD_ERR_RESET);
}

/*******************************************************************************
 函 数: invt_cmd_para_init()
 功 能: 发送参数初始化命令
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void invt_cmd_para_init(INVT_ID_ENUM id)
{
    SET_BIT(invt_cmd_ctrl[id], INVTCMD_PARA_INIT);

    invt_para[id][F00_PARA_INIT] = 11; //参数初始化指令（11:全初始化；22:不初始化通讯参数）
    fg_para_changed[id][F00_PARA_INIT] = TRUE;
}

/*******************************************************************************
 函 数: invt_cmd_para_read()
 功 能: 发送参数读取命令
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void invt_cmd_para_read(INVT_ID_ENUM id)
{
    SET_BIT(invt_cmd_ctrl[id], INVTCMD_PARA_READ);
}

/*******************************************************************************
 函 数: invt_cmd_pfc_off()
 功 能: 发送禁用有源PFC命令
 参 数: id--; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void invt_cmd_pfc_off(INVT_ID_ENUM id)
{
    SET_BIT(invt_cmd_ctrl[id], INVTCMD_PFC_OFF);
}

/*******************************************************************************
 函 数: invt_cmd_pfc_on()
 功 能: 发送使用有源PFC命令
 参 数: id--; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void invt_cmd_pfc_on(INVT_ID_ENUM id)
{
    SET_BIT(invt_cmd_ctrl[id], INVTCMD_PFC_ON);
}

#if 0
/*******************************************************************************
 函 数: invt_cmd_para_save()
 功 能: 发送参数保存命令
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void invt_cmd_para_save(INVT_ID_ENUM id)
{
    SET_BIT(invt_cmd_ctrl[id], INVTCMD_PARA_SAVE);
}
#endif


/*******************************************************************************
 函 数: invt_get_device_addr()
 功 能: 读取变频器设备地址
 参 数: 空 
 返 回: 变频器设备地址
 其 它: 空
*******************************************************************************/
U08 invt_get_device_addr(INVT_ID_ENUM id)/* N0004 */
{
    U08 addr;
    if (id >= INVT_ID_MAX)
    {
        addr = ADD_INVERTER_COMMU;
    }
    else if (id >= INVT_FAN1)
    {
        addr = id-INVT_FAN1+ADD_INVT_FAN_COMMU;
    }
    else
    {
        addr = id-INVT_ID_1+ADD_INVERTER_COMMU;
    }
    
    return addr;
}

/*******************************************************************************
 函 数: invt_cmd_get_addr()
 功 能: 访问变频器通讯指令的地址
 参 数: 空 
 返 回: 变频器通讯指令的地址
 其 它: 空
*******************************************************************************/
U16 invt_cmd_get_addr(void)
{
    U16 addr;
    addr = INVT_ADDR_RW_CMD;
    return addr;
}

/*******************************************************************************
 函 数: invt_cmd_get_size()
 功 能: 访问变频器通讯指令的大小（字）
 参 数: 空 
 返 回: 变频器通讯指令的大小
 其 它: 空
*******************************************************************************/
U08 invt_cmd_get_size(void)
{
    U08 size;
    size = CMD_INVT_MAX;
    return size;
}

/*******************************************************************************
 函 数: invt_paras_get_buf()
 功 能: 读取变频器基本参数的存储位置
 参 数: 空 
 返 回: 空
 其 它: 空
*******************************************************************************/
U16* invt_cmd_get_buf(INVT_ID_ENUM id)
{
    U16 *buf;
    buf = (U16*)&invt_cmd[id][0];
    return buf;
}

/*******************************************************************************
 函 数: invt_paras_get_addr()
 功 能: 访问变频器基本参数的地址
 参 数: 空 
 返 回: 变频器基本参数的地址
 其 它: 空
*******************************************************************************/
U16 invt_paras_get_addr()
{
    U16 addr;
    addr = INVT_ADDR_RW_PARA_F00;
    return addr;
}

/*******************************************************************************
 函 数: invt_paras_get_size()
 功 能: 访问变频器基本参数的大小（字）
 参 数: 空 
 返 回: 变频器基本参数的大小
 其 它: 空
*******************************************************************************/
U08 invt_paras_get_size()
{
    U08 size;
    size = F00_MAX;
    return size;
}

/*******************************************************************************
 函 数: invt_paras_fa_get_addr()
 功 能: 访问变频器Fa参数的地址
 参 数: 空 
 返 回: 变频器Fa参数的地址
 其 它: 空
*******************************************************************************/
U16 invt_paras_fa_get_addr()
{
    U16 addr;
    addr = INVT_ADDR_RW_PARA_FA;
    return addr;
}
U16 invt_paras_fb_get_addr()
{
    U16 addr;
    addr = INVT_ADDR_RW_PARA_FB;
    return addr;
}
U16 invt_paras_fp_get_addr(void)
{
    U16 addr;
    addr = INVT_ADDR_RW_PARA_FP;
    return addr;
}
U16 invt_paras_f10_get_addr(void)
{
    U16 addr;
    addr = INVT_ADDR_RW_PARA_F10;
    return addr;
}
U16 invt_paras_f9_get_addr(void)
{
    U16 addr;
    addr = INVT_ADDR_RW_PARA_F9;
    return addr;
}
U16 invt_paras_f6_get_addr(void)
{
    U16 addr;
    addr = INVT_ADDR_RW_PARA_F6;
    return addr;
}

/*******************************************************************************
 函 数: invt_paras_fa_get_size()
 功 能: 访问变频器基本参数的大小（字）
 参 数: 空 
 返 回: 变频器基本参数的大小
 其 它: 空
*******************************************************************************/
U08 invt_paras_fa_get_size()
{
    U08 size;
    size = FA_MAX;
    return size;
}
U08 invt_paras_fb_get_size()
{
    U08 size;
    size = FB_MAX;
    return size;
}
U08 invt_paras_fp_get_size(void)
{
    U08 size;
    size = FP_MAX;
    return size;
}
U08 invt_paras_f10_get_size(void)
{
    U08 size;
    size = F10_MAX;
    return size;
}
U08 invt_paras_f9_get_size(void)
{
    U08 size;
    size = F9_MAX;
    return size;
}
U08 invt_paras_f6_get_size(void)
{
    U08 size;
    size = F6_MAX;
    return size;
}
/*******************************************************************************
 函 数: invt_paras_fa_get_buf()
 功 能: 读取变频器基本参数的存储位置
 参 数: 空 
 返 回: 空
 其 它: 空
*******************************************************************************/
U16* invt_paras_fa_get_buf(INVT_ID_ENUM id)
{
    U16 *buf;
    buf = (U16*)&invt_para_fa[id][0];
    return buf;
}
U16* invt_paras_fb_get_buf(INVT_ID_ENUM id)
{
    U16 *buf;
    buf = (U16*)&invt_para_fb[id][0];
    return buf;
}
U16* invt_paras_fp_get_buf(INVT_ID_ENUM id)
{
    U16 *buf;
    buf = (U16*)&invt_para_fp[id][0];
    return buf;
}
U16* invt_paras_f10_get_buf(INVT_ID_ENUM id)
{
    U16 *buf;
    buf = (U16*)&invt_para_f10[id][0];
    return buf;
}
U16* invt_paras_f9_get_buf(INVT_ID_ENUM id)
{
    U16 *buf;
    buf = (U16*)&invt_para_f9[id][0];
    return buf;
}
U16* invt_paras_f6_get_buf(INVT_ID_ENUM id)
{
    U16 *buf;
    buf = (U16*)&invt_para_f6[id][0];
    return buf;
}

/*******************************************************************************
 函 数: invt_para_fa_isChanged()
 功 能: 判断变频器参数是否有修改
 参 数: INVT_ID_ENUM id--变频器序号; 
 返 回: FALSE:无修改；其它:修改的参数序号+1
 其 它: 空
*******************************************************************************/
U16 invt_para_fa_isChanged(INVT_ID_ENUM id)
{
    U16 ChangeIndex=FALSE;
//    U08 i;
    U08 para_id = 0;
    BOOL fg_send_para = FALSE;          //发送新参数
    
//    invt_deal_para(id);  // 更新命令
    
    //检查是否有参数更改
    if (fg_para_fa_changed[id][FA_FREQ_START])
	{
	    para_id = FA_FREQ_START;
	    fg_send_para = TRUE;
	}
    else if (fg_para_fa_changed[id][FA_MAX_CUR_LMT])
	{
	    para_id = FA_MAX_CUR_LMT;
	    fg_send_para = TRUE;
	}
    else if (fg_para_fa_changed[id][FA_FREQ_START2])
	{
	    para_id = FA_FREQ_START2;
	    fg_send_para = TRUE;
	} 

    //更改参数
    if(fg_send_para)
    {
        ChangeIndex = para_id+1;
//        invt_cmd_para_save(id);   //更改完后保存
    }
    
    return ChangeIndex;
}
U16 invt_para_fb_isChanged(INVT_ID_ENUM id)
{
    U16 ChangeIndex=FALSE;
    U08 para_id = 0;
    BOOL fg_send_para = FALSE;          //发送新参数
    
    //检查是否有参数更改
    if (fg_para_fb_changed[id][FB_MAX_LOAD_TIME])
	{
	    para_id = FB_MAX_LOAD_TIME;
	    fg_send_para = TRUE;
	}
    else if (fg_para_fb_changed[id][FB_BLOCKAGE_TIME])
	{
	    para_id = FB_BLOCKAGE_TIME;
	    fg_send_para = TRUE;
	}
    
    //更改参数
    if(fg_send_para)
    {
        ChangeIndex = para_id+1;
//        invt_cmd_para_save(id);   //更改完后保存
    }
    
    return ChangeIndex;
}
U16 invt_para_f10_isChanged(INVT_ID_ENUM id)
{
    U16 ChangeIndex=FALSE;
    U08 para_id = 0;
    BOOL fg_send_para = FALSE;          //发送新参数
    
    //检查是否有参数更改
    if (fg_para_f10_changed[id][F10_POWER])
	{
	    para_id = F10_POWER;
	    fg_send_para = TRUE;
	}
    
    //更改参数
    if(fg_send_para)
    {
        ChangeIndex = para_id+1;
//        invt_cmd_para_save(id);   //更改完后保存
    }
    
    return ChangeIndex;
}
U16 invt_para_f9_isChanged(INVT_ID_ENUM id)
{
    U16 ChangeIndex=FALSE;
    U08 para_id = 0;
    BOOL fg_send_para = FALSE;          //发送新参数
    
    //检查是否有参数更改
    if (fg_para_f9_changed[id][F9_EC_FAN_NUM])
	{
	    para_id = F9_EC_FAN_NUM;
	    fg_send_para = TRUE;
	}
    
    //更改参数
    if(fg_send_para)
    {
        ChangeIndex = para_id+1;
//        invt_cmd_para_save(id);   //更改完后保存
    }
    
    return ChangeIndex;
}
U16 invt_para_f6_isChanged(INVT_ID_ENUM id)
{
    U16 ChangeIndex=FALSE;
    U08 para_id = 0;
    BOOL fg_send_para = FALSE;          //发送新参数
    
    //检查是否有参数更改
    if (fg_para_f6_changed[id][F6_00_EC_PWM_SW_FREQ])
	{
	    para_id = F6_00_EC_PWM_SW_FREQ;
	    fg_send_para = TRUE;
	}
    
    //更改参数
    if(fg_send_para)
    {
        ChangeIndex = para_id+1;
//        invt_cmd_para_save(id);   //更改完后保存
    }
    
    return ChangeIndex;
}

/*******************************************************************************
 函 数: invt_paras_fa_backup()
 功 能: 变频器内部参数修改后取消修改标志
 参 数: INVT_ID_ENUM id--变频器序号; U08 index--参数序号
 返 回: 0xFF:无修改；其它:修改的命令序号
 其 它: 空
*******************************************************************************/
void invt_paras_fa_backup(INVT_ID_ENUM id, U08 index)
{
    if(index<FA_MAX)
    {
        fg_para_fa_changed[id][index]=FALSE;

    }
}
void invt_paras_fb_backup(INVT_ID_ENUM id, U08 index)
{
    if(index<FB_MAX)
    {
        fg_para_fb_changed[id][index]=FALSE;
    }
}
void invt_paras_f10_backup(INVT_ID_ENUM id, U08 index)
{
    if(index<F10_MAX)
    {
        fg_para_f10_changed[id][index]=FALSE;
    }
}
void invt_paras_f9_backup(INVT_ID_ENUM id, U08 index)
{
    if(index<F9_MAX)
    {
        fg_para_f9_changed[id][index]=FALSE;
    }
}
void invt_paras_f6_backup(INVT_ID_ENUM id, U08 index)
{
    if(index<F6_MAX)
    {
        fg_para_f6_changed[id][index]=FALSE;
    }
}
/*******************************************************************************
 函 数: invt_paras_get_buf()
 功 能: 读取变频器基本参数的存储位置
 参 数: 空 
 返 回: 空
 其 它: 空
*******************************************************************************/
U16* invt_paras_get_buf(INVT_ID_ENUM id)
{
    U16 *buf;
    buf = (U16*)&invt_para[id][0];
    return buf;
}

/*******************************************************************************
 函 数: invt_states_get_addr()
 功 能: 读取变频器状态的地址
 参 数: 空 
 返 回: 变频器状态的地址
 其 它: 空
*******************************************************************************/
U16 invt_states_get_addr()
{
    U16 addr;
    addr = INVT_ADDR_R_STATES;
    return addr;
}

/*******************************************************************************
 函 数: invt_states_get_size()
 功 能: 读取变频器状态的大小（字）
 参 数: 空 
 返 回: 变频器状态的大小
 其 它: 空
*******************************************************************************/
U08 invt_states_get_size()
{
    U08 size;
    
    if (USE_PWM_FAN_EX
    ||  invt_para_read(INVT_ID_1, INVT_PARA_EC_NUM)>0
    ||  TestProgStaus != TPS_NO_TEST_CMD
        )
    {
        size = INVT_INFO_SIZE;
    }
    else
    {
        size = INVT_INFO_SIZE_OLD;
    }
    
    return size;
}

/*******************************************************************************
 函 数: invt_states_get_buf()
 功 能: 读取变频器状态的存储位置
 参 数: 空 
 返 回: 空
 其 它: 空
*******************************************************************************/
U16* invt_states_get_buf(INVT_ID_ENUM id)
{
    U16 *buf;
    buf = (U16*)&invt_info[id];
    return buf;
}



/*******************************************************************************
 函 数: invt_cmd_deal()
 功 能: 响应应用程序发来的命令
 参 数: INVT_ID_ENUM id 变频器序号
 返 回: 空
 其 它: 空
*******************************************************************************/
static void invt_deal_cmd(INVT_ID_ENUM id)
{
    //通讯协议中定义的控制字各位
    #define INVT_CNTRL_ONOFF1        0   //ON/OFF1（上升沿运行，为 0 则停机）
    #define INVT_CNTR_OFF2           1   //OFF2（为 0 则断电停机）此函数未采用该种形式
    #define INVT_CNTR_OFF3           2   //OFF3（为 0 则紧急停机）
    #define INVT_CNTR_LOCK_FREQ      3   //驱动封锁（为 0 则驱动封锁）
    #define INVT_CNTR_LOCK_ACCLRT    4   //斜坡使能（为 0 则停止加减速）
    #define INVT_CNTR_ERR_RESET      7   //故障复位（上升沿进行故障复位）
    #define INVT_CNTR_CLOCKWISE      8   //正向点动

    #define CMD_UPDATE_INTRVL        5  // 更新命令间隔

    static I16 invt_control_bak[INVT_ID_MAX] = {0};    //变频器命令字备份
    
    if(invt_timer_100ms[id][INVT_TIMER_100MS_CMD_UPDATE]<CMD_UPDATE_INTRVL)   return;     // 时间未到，不更新 //NOTE-CXW 是否需要时间间隔？
    
    //开启
    if(GET_BIT(invt_cmd_ctrl[id], INVTCMD_ON))
    {
        CLR_BIT(invt_cmd_ctrl[id], INVTCMD_ON);
        SET_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTRL_ONOFF1);
        SET_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTR_OFF2);
        SET_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTR_OFF3);
        SET_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTR_LOCK_FREQ);
    }
    
    
    
    
    // 停机
    if(GET_BIT(invt_cmd_ctrl[id], INVTCMD_OFF_FREE))//自由停机
    {
        CLR_BIT(invt_cmd_ctrl[id], INVTCMD_OFF_FREE);
        CLR_BIT(invt_cmd_ctrl[id], INVTCMD_OFF_QUICK);
        CLR_BIT(invt_cmd_ctrl[id], INVTCMD_OFF);
        
        CLR_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTRL_ONOFF1);
        CLR_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTR_OFF2);
        SET_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTR_OFF3);
        SET_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTR_LOCK_FREQ);						//按此修改	
        SET_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTR_LOCK_ACCLRT);						//按此修改	

    }
    else if(GET_BIT(invt_cmd_ctrl[id], INVTCMD_OFF_QUICK))//急停
    {
        CLR_BIT(invt_cmd_ctrl[id], INVTCMD_OFF_QUICK);
        CLR_BIT(invt_cmd_ctrl[id], INVTCMD_OFF);

        
        SET_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTR_OFF2);
        CLR_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTR_OFF3);
    }
    else if(GET_BIT(invt_cmd_ctrl[id], INVTCMD_OFF))    //减速停止
    {
        CLR_BIT(invt_cmd_ctrl[id], INVTCMD_OFF);
        CLR_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTRL_ONOFF1);
        SET_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTR_OFF2);
        SET_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTR_OFF3);
    }

    
    //频率锁死
    if(GET_BIT(invt_cmd_ctrl[id], INVTCMD_FREQ_LOCK))
    {
        CLR_BIT(invt_cmd_ctrl[id], INVTCMD_FREQ_LOCK);
        CLR_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTR_LOCK_FREQ);
    }
    //频率解锁
    if(GET_BIT(invt_cmd_ctrl[id], INVTCMD_FREQ_UNLOCK))
    {
        CLR_BIT(invt_cmd_ctrl[id], INVTCMD_FREQ_UNLOCK);
        SET_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTR_LOCK_FREQ);
    }
    //变速锁死
    if(GET_BIT(invt_cmd_ctrl[id], INVTCMD_ACCLRT_LOCK))
    {
        CLR_BIT(invt_cmd_ctrl[id], INVTCMD_ACCLRT_LOCK);
        CLR_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTR_LOCK_ACCLRT);
    }
    //变速解锁
    if(GET_BIT(invt_cmd_ctrl[id], INVTCMD_ACCLRT_UNLOCK))
    {
        CLR_BIT(invt_cmd_ctrl[id], INVTCMD_ACCLRT_UNLOCK);
        SET_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTR_LOCK_ACCLRT);
    } 
    //故障复位
    if(GET_BIT(invt_cmd_ctrl[id], INVTCMD_ERR_RESET))
    {
        CLR_BIT(invt_cmd_ctrl[id], INVTCMD_ERR_RESET);
        SET_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTR_ERR_RESET);
        invt_timer_1s[id][INVT_TIMER_1S_RESET_CHG] = 0;
    } 
    else //故障复位上升沿有效，遂无命令时拉低
    {
        CLR_BIT(invt_cmd[id][CMD_CONTROL], INVT_CNTR_ERR_RESET);
    }

    // 更新变频控制字到BMS协议地址
    {
        I16 temp = invt_cmd[id][CMD_CONTROL];
        if(invt_timer_1s[id][INVT_TIMER_1S_RESET_CHG] < 3)  // 使复位命令保持，以便监控数据变化
            SET_BIT(temp, INVT_CNTR_ERR_RESET);
        if (id == INVT_ID_1)
        {
            SET_BMS_DEBUG_VAL(BDV_INVT_CMD_INVT_ID_1, temp);
        }
        else if (id == INVT_FAN1)
        {
            SET_BMS_DEBUG_VAL(BDV_INVT_CMD_INVT_FAN1, temp);
        }
        else if (id == INVT_FAN2)
        {
            SET_BMS_DEBUG_VAL(BDV_INVT_CMD_INVT_FAN2, temp);
        }
    }

    
    //命令改变，发送一次命令
    if(invt_control_bak[id] != invt_cmd[id][CMD_CONTROL])
    {
        invt_control_bak[id] = invt_cmd[id][CMD_CONTROL];
        fg_cmd_changed[id][CMD_CONTROL]=TRUE;
    }

    
//    // 参数保存
//    if(GET_BIT(invt_cmd_ctrl[id], INVTCMD_PARA_SAVE))
//    {
//        CLR_BIT(invt_cmd_ctrl[id], INVTCMD_PARA_SAVE);
//        invt_cmd[id][CMD_EEP_SAVE]=1;
//        fg_cmd_changed[id][CMD_EEP_SAVE]=TRUE;
//    }
    
    invt_timer_100ms[id][INVT_TIMER_100MS_CMD_UPDATE]=0;  
}

/*******************************************************************************
 函 数: invt_deal_pfc_cmd()
 功 能: 处理使用/禁用有源PFC请求，填入相应的通信命令值: 禁用(=55)，使用(其它值)
 参 数: id--; 
 返 回: 
 其 它: 空
*******************************************************************************/
static void invt_deal_pfc_cmd(INVT_ID_ENUM id)
{
    #define CMD_UPDATE_INTRVL        5  // 更新命令间隔

    if(invt_timer_100ms[id][INVT_TIMER_100MS_PFC_CMD_UPDATE]<CMD_UPDATE_INTRVL)   return;     // 时间未到，不更新

    if(GET_BIT(invt_cmd_ctrl[id], INVTCMD_PFC_OFF))
    {
        CLR_BIT(invt_cmd_ctrl[id], INVTCMD_PFC_OFF);
        invt_cmd[id][CMD_SET_PFC] = 55;
        fg_cmd_changed[id][CMD_SET_PFC]=TRUE;
    }

    if(GET_BIT(invt_cmd_ctrl[id], INVTCMD_PFC_ON))
    {
        CLR_BIT(invt_cmd_ctrl[id], INVTCMD_PFC_ON);
        invt_cmd[id][CMD_SET_PFC] = 0;
        fg_cmd_changed[id][CMD_SET_PFC]=TRUE;
    }

    invt_timer_100ms[id][INVT_TIMER_100MS_PFC_CMD_UPDATE]=0;  
}

/*******************************************************************************
 函 数: invt_deal_freq()
 功 能: 响应应用程序发来的新频率
 参 数: pUart--通讯缓冲; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
static void invt_deal_freq(INVT_ID_ENUM id)//(UARTBUF *pUart)
{    
    #define FREQ_CONFRM_TIME      5   //转速确认时间(秒)
    
//    static U16 timer_interval = 0;    //发送间隔备份
//    static U16 invt_freq_set_bak = 0; //设定转速备份
//    BOOL fg_send_new_freq = FALSE;    //发送新转速
    BOOL fg_freq_not_right = FALSE;   //运行转速不是设定转速
    BOOL fg_freq_changed = FALSE;     //设定转速改变

    //运行转速不是设定转速 且 持续了一段时间
    if(invt_info_freq_arm(id) != invt_freq_set[id]
    && invt_timer_1s[id][INVT_TIMER_1S_WR_FREQ] >= FREQ_CONFRM_TIME)
    {
        fg_freq_not_right = TRUE;
    }
    //设定转速改变
    if(invt_cmd[id][CMD_SET_FREQ] != invt_freq_set[id])
    {
        fg_freq_changed = TRUE;
    }
    //发送一次转速
    if(invt_info_is_running(id) && (fg_freq_not_right || fg_freq_changed))
    {
        invt_timer_1s[id][INVT_TIMER_1S_WR_FREQ] = 0;
//        invt_freq_set_bak = invt_freq_set;
        
        invt_cmd[id][CMD_SET_FREQ]=invt_freq_set[id];
        fg_cmd_changed[id][CMD_SET_FREQ]=TRUE;
        
//        invt_uart_send_frequency(pUart, invt_freq_set);
//        fg_send_new_freq = TRUE;
    }

//    return fg_send_new_freq;
}

/*******************************************************************************
 函 数: invt_deal_freq_ec()
 功 能: 响应应用程序发来的ec风机转速
 参 数: id--变频器号; ec--ec风机号；
 返 回: 无
 其 它: 空
*******************************************************************************/
static void invt_deal_freq_ec(INVT_ID_ENUM id, U08 ec)
{    
    I16 value_now = invt_freq_set_ec[id][ec];
    U08 cmd_idx = CMD_SET_FREQ_EC1+ec;		/* 注意：若序号不连续应做相应修改 */
    
    // 发送一次转速
    if (invt_cmd[id][cmd_idx] != value_now
    || (value_now==0 
        && (invt_info_pulse_ec1(id)!=0 && ec==FREQ_FAN1
            || invt_info_pulse_ec2(id)!=0 && ec==FREQ_FAN2)	/* 闭环关风机，此处理须有命令发送间隔 */
        && invt_timer_1s[id][INVT_TIMER_1S_WR_FREQ_FAN] >= 1)
        )
    {
        invt_cmd[id][cmd_idx]=value_now;
        fg_cmd_changed[id][cmd_idx]=TRUE;
        invt_timer_1s[id][INVT_TIMER_1S_WR_FREQ_FAN] = 0;
    }
}

/*******************************************************************************
 函 数: invt_cmd_isChanged()
 功 能: 判断变频器命令是否有修改
 参 数: INVT_ID_ENUM id--变频器序号; 
 返 回: FALSE:无修改；其它:修改的命令序号+1
 其 它: 空
*******************************************************************************/
U08 invt_cmd_isChanged(INVT_ID_ENUM id)
{
    U08 ChangeIndex=FALSE;
    U08 i;

    invt_deal_cmd(id);              // 更新命令
    invt_deal_pfc_cmd(id);          // 更新PFC命令
    invt_deal_freq(id);             // 更新频率
    invt_deal_freq_ec(id, FREQ_FAN1); // 更新ec1
    invt_deal_freq_ec(id, FREQ_FAN2); // 更新ec2
    
    for(i=0; (i<CMD_INVT_MAX && i<0xFF) ; i++)
    {
        if (fg_cmd_changed[id][i])
        {
            ChangeIndex = i+1;
            
            break;
        }
    }
    return ChangeIndex;
}
/*******************************************************************************
 函 数: invt_cmd_backup()
 功 能: 变频器命令使用后备份（失效）
 参 数: INVT_ID_ENUM id--变频器序号; U08 index--参数序号

 返 回: 0xFF:无修改；其它:修改的命令序号
 其 它: 空
*******************************************************************************/
void invt_cmd_backup(INVT_ID_ENUM id, U08 index)
{
    if(index<CMD_INVT_MAX)
    {
//        invt_cmd_bak[id][index]=invt_cmd[id][index];
        fg_cmd_changed[id][index]=FALSE;
//        if(index==CMD_EEP_SAVE)
//        {
//            invt_cmd[id][index]=0;  // 避免为1时错误发出命令
//        }
    }
}

/*******************************************************************************
 函 数: invt_para_isChanged()
 功 能: 判断变频器参数是否有修改
 参 数: INVT_ID_ENUM id--变频器序号; 
 返 回: FALSE:无修改；其它:修改的参数序号+1
 其 它: 空
*******************************************************************************/
U16 invt_para_isChanged(INVT_ID_ENUM id)
{
    U16 ChangeIndex=FALSE;
//    U08 i;
    U08 para_id = 0;
    BOOL fg_send_para = FALSE;          //发送新参数
    
//    invt_deal_para(id);  // 更新命令
    
    //检查是否有参数更改
    if(fg_para_changed[id][F00_MOTOR_TYPE])	/* 2023-4-28 先发电机型号再发上下限，防止驱动型号更改后又恢复上下限 */
	{
//	    fg_para_changed[id][F00_MOTOR_TYPE] = FALSE;
	    para_id = F00_MOTOR_TYPE;
	    fg_send_para = TRUE;
	}
    else if(fg_para_changed[id][F00_FREQ_MAX])
	{
//	    fg_para_changed[id][F00_FREQ_MAX] = FALSE;
	    para_id = F00_FREQ_MAX;
	    fg_send_para = TRUE;
	}
    else if(fg_para_changed[id][F00_FREQ_MIN])
	{
//	    fg_para_changed[id][F00_FREQ_MIN] = FALSE;
	    para_id = F00_FREQ_MIN;
	    fg_send_para = TRUE;
	}
    

    //更改参数
    if(fg_send_para)
    {
        ChangeIndex = para_id+1;
//        invt_cmd_para_save(id);   //更改完后保存
    }
    else if(fg_para_changed[id][F00_PARA_INIT])
	{
//	    fg_para_changed[id][F00_PARA_INIT] = FALSE;
	    para_id = F00_PARA_INIT;

        ChangeIndex= para_id+1;
        invt_cmd_para_read(id);//初始化后读取          //NOTE-CXW 可能有时间顺序？
	}
    
    return ChangeIndex;
}




/*******************************************************************************
 函 数: invt_paras_backup()
 功 能: 变频器参数修改后取消修改标志
 参 数: INVT_ID_ENUM id--变频器序号; U08 index--参数序号
 返 回: 0xFF:无修改；其它:修改的命令序号
 其 它: 空
*******************************************************************************/
void invt_paras_backup(INVT_ID_ENUM id, U08 index)
{
    if(index<F00_MAX)
    {
        fg_para_changed[id][index]=FALSE;

    }
}

/*******************************************************************************
 函 数: invt_timer()
 功 能: 模块计时函数
 参 数: BOOL TimeFlag 时间标志（用参数传递，以免移植时100ms标志名称不同）
 返 回: 空
 其 它: 在100ms计时处理处调用
*******************************************************************************/
void invt_timer(BOOL TimeFlag)
{
    U08 i,j;
    static U08 cnt_100ms_to_1s = 0; //100毫秒换算成1秒

    if(TimeFlag)
    {
        for(i=0; i<INVT_ID_MAX; i++)
        {
            for(j=0; j<INVT_TIMER_100MS_MAX; j++)
            {
            	if(invt_timer_100ms[i][j] != 0xffff) invt_timer_100ms[i][j]++;
            }
        }     
        if(++cnt_100ms_to_1s >= 10)
        {
            cnt_100ms_to_1s = 0;
            for(i=0; i<INVT_ID_MAX; i++)
            {
                for(j=0; j<INVT_TIMER_1S_MAX; j++)
                {
                	if(invt_timer_1s[i][j] != 0xffff) invt_timer_1s[i][j]++;
                }
            }
        }
    }
}

/*******************************************************************************
 函 数: invt_init()
 功 能: 初始化变频器通讯模块
        初始化内容：停止变频器输出，解锁频率及变速，读取一次参数。
 参 数: INVT_ID_ENUM id 变频器序号
 返 回: 空
 其 它: 在应用程序初始化中调用
*******************************************************************************/
void invt_init()
{
    INVT_ID_ENUM i;
    U08 j;

    //参数缓冲区初始为无效值
    for(i=(INVT_ID_ENUM)0; i<INVT_ID_MAX; i++)
    {
        //关机，解锁，读取参数
        //invt_cmd_off(i);
        invt_cmd_off_free(i);
        invt_cmd_freq_unlock(i);
        invt_cmd_acclrt_unlock(i);
        invt_cmd_para_read(i);
        
        for(j=0; j<F00_MAX; j++)
        {
        	fg_para_changed[i][j] = FALSE;
        	invt_para[i][j] = INVT_PARA_VALUE_NULL;
        }
        for(j=0; j<CMD_INVT_MAX; j++)
        {
        	fg_cmd_changed[i][j] = FALSE;
        	invt_cmd[i][j] = INVT_PARA_VALUE_NULL;
//            invt_cmd_bak[i][j] = INVT_PARA_VALUE_NULL;
        }

        for(j=0; j<INVT_TIMER_100MS_MAX; j++)
        {
        	invt_timer_100ms[i][j] = 0;
        }
        for(j=0; j<INVT_TIMER_1S_MAX; j++)
        {
        	invt_timer_1s[i][j] = 0;
        }
    }
}



/*******************************************************************************
 函 数: invt_info_is_running()
 功 能: 变频器运行状态
 参 数: 空
 返 回: TRUE/FALSE--运行中/非运行
 其 它: 空
*******************************************************************************/
BOOL invt_info_is_running(INVT_ID_ENUM id)
{
    return (GET_BIT(invt_info[id].state, INVT_STATE_RUN) == 1);
}

/*******************************************************************************
 函 数: invt_info_is_alarm()
 功 能: 变频器故障状态
 参 数: INVT_ID_ENUM id 变频器序号
 返 回: TRUE/FALSE--有故障/无故障
 其 它: 空
*******************************************************************************/
BOOL invt_info_is_alarm(INVT_ID_ENUM id)
{
    return (GET_BIT(invt_info[id].state, INVT_STATE_ERR) == 1);
}

/*******************************************************************************
 函 数: invt_info_version()
 功 能: 读取软件版本号
 参 数: INVT_ID_ENUM id 变频器序号
 返 回: 变频器软件版本号
 其 它: 空
*******************************************************************************/
U16 invt_info_version(INVT_ID_ENUM id)
{
    return invt_para[id][F00_VERSION];
}

/*******************************************************************************
 函 数: invt_info_function()
 功 能: 读取软件功能码
 参 数: INVT_ID_ENUM id 变频器序号
 返 回: 变频器软件功能码
 其 它: 空
*******************************************************************************/
U16 invt_info_function(INVT_ID_ENUM id)
{
    return invt_para[id][F00_FUNCTION];
}
/*******************************************************************************
 函 数: invt_info_freq_run()
 功 能: 查看变频器运行状态
 参 数: 空
 返 回: 变频器运行转速(0.01rps)
 其 它: 运行转速为变频器此刻的运行转速
*******************************************************************************/
U16 invt_info_freq_state(INVT_ID_ENUM id)
{
    return invt_info[id].state;
}

/*******************************************************************************
 函 数: invt_info_freq_run()
 功 能: 查看运行转速
 参 数: 空
 返 回: 变频器运行转速(0.01rps)
 其 它: 运行转速为变频器此刻的运行转速
*******************************************************************************/
U16 invt_info_freq_run(INVT_ID_ENUM id)
{
    return invt_info[id].freq_run;
}

/*******************************************************************************
 函 数: invt_info_freq_arm()
 功 能: 查看目标转速
 参 数: 空
 返 回: 变频器设定转速(0.01rps)
 其 它: 目标转速为变频器接收到的设定转速
*******************************************************************************/
U16 invt_info_freq_arm(INVT_ID_ENUM id)
{
    return invt_info[id].freq_set;
}

/*******************************************************************************
 函 数: invt_info_freq_now()
 功 能: 查看当前转速
 参 数: 空
 返 回: 变频器当前转速(0.01rps)
 其 它: 当前转速为变频器此刻的真实转速
*******************************************************************************/
U16 invt_info_freq_now(INVT_ID_ENUM id)
{
    return invt_info[id].freq_now;
}

/*******************************************************************************
 函 数: invt_info_temperature()
 功 能: 查看变频模块温度
 参 数: 空
 返 回: 当前温度(0.1℃)
 其 它: 空
*******************************************************************************/
I16 invt_info_temperature(INVT_ID_ENUM id)
{
    return invt_info[id].temperature;
}

/*******************************************************************************
 函 数: invt_info_powerout()
 功 能: 查看变频模块输出功率
 参 数: 空
 返 回: 当前输出功率(0.01KW)
 其 它: 空
*******************************************************************************/
U16 invt_info_powerout(INVT_ID_ENUM id)
{
    return invt_info[id].power_out;
}


/*******************************************************************************
 函 数: invt_info_error_id()
 功 能: 查看故障号
 参 数: 空
 返 回: 变频器当前故障的故障号
 其 它: 只能查看到最早发生的故障，当同时存在多个故障时，只能等该故障复位后
        才能才看其它的故障。
*******************************************************************************/
U16 invt_info_error_id(INVT_ID_ENUM id)
{
    return invt_info[id].errid;
}

/*******************************************************************************
 函 数: invt_clr_error_id()
 功 能: 清空故障号
 参 数: 空
 返 回: 
 其 它: 
*******************************************************************************/
void invt_clr_error_id(INVT_ID_ENUM id)
{
     invt_info[id].errid = 0;
}

/*******************************************************************************
 函 数: invt_info_errorcode1_id()
 功 能: 查看故障号1
 参 数: 空
 返 回: 变频器当前故障的故障号
 其 它: 只能查看到最早发生的故障，当同时存在多个故障时，只能等该故障复位后
        才能才看其它的故障。
*******************************************************************************/
U16 invt_info_errorcode1_id(INVT_ID_ENUM id)
{
    return invt_info[id].errcode1;
}
/*******************************************************************************
 函 数: invt_info_errorcode1_id()
 功 能: 查看故障号1
 参 数: 空
 返 回: 变频器当前故障的故障号
 其 它: 只能查看到最早发生的故障，当同时存在多个故障时，只能等该故障复位后
        才能才看其它的故障。
*******************************************************************************/
U16 invt_info_temp_pfc(INVT_ID_ENUM id)
{
    return invt_info[id].errcode2;
}
/*******************************************************************************
 函 数: invt_info_freq_limit_id()
 功 能: 查看限频信息
 参 数: 空
 返 回: 变频器当前故障的故障号
 其 它: 只能查看到最早发生的故障，当同时存在多个故障时，只能等该故障复位后
        才能才看其它的故障。
*******************************************************************************/
U16 invt_info_freq_limit_id(INVT_ID_ENUM id)
{
    return invt_info[id].freq_limit;
}


/*******************************************************************************
 函 数: invt_info_current_out()
 功 能: 查看输出电流(0.1A)
 参 数: 空
 返 回: 变频器当前输出电流
 其 它: 空
*******************************************************************************/
U16 invt_info_current_out(INVT_ID_ENUM id)
{
    return invt_info[id].current_out;
}
/*******************************************************************************
 函 数: invt_info_torque_out()
 功 能: 查看输出转矩
 参 数: 空
 返 回: 变频器当前输出转矩
 其 它: 空
*******************************************************************************/
U16 invt_info_torque_out(INVT_ID_ENUM id)
{
    return invt_info[id].torque_out;
}


/*******************************************************************************
 函 数: invt_info_voltage_out()
 功 能: 查看输出电压
 参 数: 空
 返 回: 变频器当前输出电压
 其 它: 空
*******************************************************************************/
U16 invt_info_voltage_out(INVT_ID_ENUM id)
{
    return invt_info[id].volt_out;
}

/*******************************************************************************
 函 数: invt_info_volt_generatrix()
 功 能: 查看母线电压
 参 数: 空
 返 回: 变频器当前母线电压
 其 它: 空
*******************************************************************************/
U16 invt_info_volt_generatrix(INVT_ID_ENUM id)
{
    return invt_info[id].volt_generatrix;
}

/*******************************************************************************
 函 数: invt_info_pulse_ec1()
 功 能: 查看ec1脉冲数
 参 数: id--; 
 返 回: 
 其 它: 空
*******************************************************************************/
U16 invt_info_pulse_ec1(INVT_ID_ENUM id)
{
    return invt_info[id].pulse_ec1;
}

/*******************************************************************************
 函 数: invt_info_pulse_ec2()
 功 能: 查看ec2脉冲数
 参 数: id--; 
 返 回: 
 其 它: 空
*******************************************************************************/
U16 invt_info_pulse_ec2(INVT_ID_ENUM id)
{
    return invt_info[id].pulse_ec2;
}


#define PULSE_SAMPLE_MAX  (20)
/*******************************************************************************
 函 数: invt_info_pulse_ec_filt()
 功 能: 脉冲数滑动窗口滤波
 参 数: invt_id--; 
		fan_id--; 
 返 回: 
 其 它: 空
*******************************************************************************/
U16 invt_info_pulse_ec_filt(INVT_ID_ENUM invt_id, U08 fan_id)
{
    U08 i;
    U16 pulse_sum, pulse_filt, pulse_now;
    
    static U16 pulse_prev[FREQSPEEDNUM] = { 0xffff, 0xffff };
    static U16 pulse_buf[FREQSPEEDNUM][PULSE_SAMPLE_MAX];

    if (fan_id > FREQSPEEDNUM)
    {
        return 0;
    }
    
    /* 选择脉冲数 */
    switch (fan_id)
    {
        default:
        case FREQ_FAN1:  pulse_now = invt_info_pulse_ec1(invt_id);  break;
        case FREQ_FAN2:  pulse_now = invt_info_pulse_ec2(invt_id);  break;
    }

    /* 滑动窗口滤波 */
    if (pulse_prev[fan_id] != pulse_now)
    {
        /* 初始化 */
        if (pulse_prev[fan_id] == 0xffff)
        {
            pulse_prev[fan_id] = pulse_now;
            for (i = 0; i < PULSE_SAMPLE_MAX; i++)
            {
                pulse_buf[fan_id][i] = pulse_now;
            }
        }
        else
        {
            for (i = 0; i < (PULSE_SAMPLE_MAX-1); i++)
            {
                pulse_buf[fan_id][i] = pulse_buf[fan_id][i+1];
            }
            pulse_buf[fan_id][i] = pulse_now;
        }
    }

    /* 窗口内采样值求和&取平均 */
    pulse_sum = 0;
    for (i = 0; i < PULSE_SAMPLE_MAX; i++)
    {
        pulse_sum += pulse_buf[fan_id][i];
    }
    pulse_filt = (U32)(pulse_sum*60)/(val_fan_pwm_pulse*PULSE_SAMPLE_MAX);
    
    return pulse_filt;
}

/*******************************************************************************
 函 数: invt_set_rps_out()
 功 能: 设置目标输出转速(经过转速上限、下限限制)
 参 数: freq_new--输出转速(0.01rps); 
 返 回: 空
 其 它: 空
*******************************************************************************/
void invt_set_rps_out(INVT_ID_ENUM id, U16 freq_new)
{
    I16 max = invt_para[id][F00_FREQ_MAX];
    I16 min = invt_para[id][F00_FREQ_MIN];

    //设定转速范围检查
    if(max != INVT_PARA_VALUE_NULL && min != INVT_PARA_VALUE_NULL)
    {
        if(freq_new > max)
        {   
            freq_new = max;
        }
        if(freq_new < min)
        {
            freq_new = min;
        }
    }
        
        invt_freq_set[id] = freq_new;
}


/*******************************************************************************
 函 数: invt_get_rps_out()
 功 能: 获取目标输出转速(经过转速上限、下限限制)
 参 数: id--变频器ID; 
 返 回: 空
 其 它: 空
*******************************************************************************/
I16 invt_get_rps_out(INVT_ID_ENUM id)
{
    return invt_freq_set[id] ;
}

/*******************************************************************************
 函 数: invt_set_rps_ec()
 功 能: 设置目标ec转速
 参 数: freq_new--目标ec转速(0～100%); 
 返 回: 空
 其 它: 空
*******************************************************************************/
void invt_set_rps_ec(INVT_ID_ENUM id, U08 ec, U16 freq_new)
{
    invt_freq_set_ec[id][ec] = freq_new;
}

/*******************************************************************************
 函 数: invt_get_rps_ec()
 功 能: 获取目标ec转速
 参 数: id--变频器ID; 
 返 回: 空
 其 它: 空
*******************************************************************************/
I16 invt_get_rps_ec(INVT_ID_ENUM id, U08 ec)
{
    return invt_freq_set_ec[id][ec];
}


/*******************************************************************************
 函 数: invt_para_read()
 功 能: 读取一个参数的值
 参 数: INVT_ID_ENUM id--变频器序号 para_id--参数号; 
 返 回: 参数值
 其 它: 空
*******************************************************************************/
I16 invt_para_read(INVT_ID_ENUM id,INVT_PARA_ID para_id)
{     
    I16 para_value = INVT_PARA_VALUE_NULL;
    
    switch (para_id)
    {
    	case INVT_PARA_MOTOR_TYPE://电机型号(0:松下6TD075X；1:松下5SS072X；2:三菱)
    	    para_value = invt_para[id][F00_MOTOR_TYPE];
    		break;
    		
    	case INVT_PARA_FREQ_MAX://转速上限(0.01rps)
    	    para_value = invt_para[id][F00_FREQ_MAX];
    		break;
    		
    	case INVT_PARA_FREQ_MIN://转速下限(0.01rps)
    	    para_value = invt_para[id][F00_FREQ_MIN];
    		break;

        case INVT_PARA_FREQ_START:// 驱动开机转速
    	    para_value = invt_para_fa[id][FA_FREQ_START];
    		break;

        case INVT_PARA_POLE:// 电机极数
    	    para_value = invt_para_fa[id][FA_FREQ_POLE];
    		break;
    		
        case INVT_PARA_POWER:// 功率等级
    	    para_value = invt_para_f10[id][F10_POWER];
    		break;
        case INVT_PARA_EC_NUM:// EC风机个数
    	    para_value = invt_para_f9[id][F9_EC_FAN_NUM];
    		break;
        case INVT_PARA_EC_PWM_SW_FREQ:// EC风机PWM开关频率
            para_value = invt_para_f6[id][F6_00_EC_PWM_SW_FREQ];
            break;
    }
    
    return para_value;
}

/*******************************************************************************
 函 数: invt_para_set()
 功 能: 改写一个参数的值
 参 数: INVT_ID_ENUM id--变频器序号 
        para_id--参数号; 
		para_value--参数值; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void invt_para_set(INVT_ID_ENUM id, INVT_PARA_ID para_id, I16 para_value)
{
    //输入数据检查
    if(para_value == INVT_PARA_VALUE_NULL)
    {
        return;
    }
    
    switch (para_id)
    {
    	case INVT_PARA_MOTOR_TYPE://电机型号(0:松下6TD075X；1:松下5SS072X；2:三菱)
    	    if(invt_para[id][F00_MOTOR_TYPE] != para_value)
    	    {
        	    fg_para_changed[id][F00_MOTOR_TYPE] = TRUE;
        	    invt_para[id][F00_MOTOR_TYPE] = para_value;
    	    }
    		break;
    		
    	case INVT_PARA_FREQ_MAX://转速上限(0.01rps)
    	    if(invt_para[id][F00_FREQ_MAX] != para_value)
    	    {
        	    fg_para_changed[id][F00_FREQ_MAX] = TRUE;
        	    invt_para[id][F00_FREQ_MAX] = para_value;
    	    }
    		break;
    		
    	case INVT_PARA_FREQ_MIN://转速下限(0.01rps)
    	    if(invt_para[id][F00_FREQ_MIN] != para_value)
    	    {
        	    fg_para_changed[id][F00_FREQ_MIN] = TRUE;
        	    invt_para[id][F00_FREQ_MIN] = para_value;
    	    }
    		break;
            
     	case INVT_PARA_FREQ_START:// 驱动开机转速
    	    {
                U16 freq_start = para_value*invt_para_read(id, INVT_PARA_POLE)/2;
    	        if (invt_para_fa[id][FA_FREQ_START] != freq_start
                &&  fun_pv_in(freq_start, 100, 30000)		/* 范围保护 */
                    )
        	    {
            	    fg_para_fa_changed[id][FA_FREQ_START] = TRUE;
            	    invt_para_fa[id][FA_FREQ_START] = freq_start;
            	    fg_para_fa_changed[id][FA_FREQ_START2] = TRUE;
            	    invt_para_fa[id][FA_FREQ_START2] = freq_start;	
        	    }
    	    }
    		break;
        case INVT_PARA_POWER:// 功率等级
    	    if(invt_para_f10[id][F10_POWER] != para_value)
    	    {
        	    fg_para_f10_changed[id][F10_POWER] = TRUE;
        	    invt_para_f10[id][F10_POWER] = para_value;
    	    }
    		break;
        case INVT_PARA_EC_NUM:// EC风机个数
    	    if(invt_para_f9[id][F9_EC_FAN_NUM] != para_value)
    	    {
        	    fg_para_f9_changed[id][F9_EC_FAN_NUM] = TRUE;
        	    invt_para_f9[id][F9_EC_FAN_NUM] = para_value;
    	    }
    		break;
   		case INVT_PARA_EC_PWM_SW_FREQ:// EC风机PWM开关频率
   		    if (invt_para_f6[id][F6_00_EC_PWM_SW_FREQ] != para_value)
   		    {
                fg_para_f6_changed[id][F6_00_EC_PWM_SW_FREQ] = TRUE;
        	    invt_para_f6[id][F6_00_EC_PWM_SW_FREQ] = para_value;
            }
            break;
    	default:
    		break;
    }
}

/*******************************************************************************
 函 数: invt_para_init()
 功 能: 初始化变频器参数
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void invt_para_init(void)      //NOTE-CXW 初始化处要加上
{
    INVT_ID_ENUM i;
    for(i=(INVT_ID_ENUM)0; i<INVT_ID_MAX; i++)
    {
        invt_cmd_para_init(i);
    }
}

/*******************************************************************************
 函 数: invt_err_reset()
 功 能: 变频器故障复位
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void invt_err_reset(void)
{
    INVT_ID_ENUM id;
    for (id=INVT_FAN1; id<INVT_ID_MAX; id++)			/* 变频风机 */
    {
        invt_cmd_err_reset(id);
    }
    
    err_mod_invt_err_reset(INVT_ID_1);			/* 变频压机 */
}

