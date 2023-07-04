/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: eev.h
 文件描述: 电子膨胀阀状态控制
           使用时只需关注本h文件，无需关注c文件。
 创建人  : hzy
 创建日期: 2017.02.08
*******************************************************************************/

#ifndef EEV_H
    #define EEV_H

#ifdef  EEV_GLOBALS
    #define EEV_EXT
#else
    #define EEV_EXT extern
#endif





/************************************** 使用说明 *****************************************

1.前置工作
    1.1 在主模块初始化时，调用本模块初始化函数 eev_init()(在读取参数后、电机归零前调用)。 
    1.2 在主模块处理100ms计时时，调用本模块计时函数 eev_timer()。
    1.3 若使用PID调节，先在eev_set_pid_para()中设置适当的PID参数。
    1.4 关于励磁频率、励磁方式参数
        1）在eev_init()处理eev的励磁频率、励磁方式初始化（相关变量定义有变化时、不同eev分开不同变量时需修改初始化）
        2）在para.c模块的para_end()，使用eev_set_motor_type()、eev_set_step_speed()处理励磁频率、励磁方式的修改
    
2.函数调用
    2.1 通过eev_set_xxx()设置膨胀阀控制相关参数。
    2.2 采用PID算法时，调用eev_task_pid()；采用分段控制时，调用eev_task_area()。


3.本模块由X1.HL017A.K05.005-1.V100A07中的PID.c更改而来，拆分成eev.c和ipd.c。因为
  需要与原控制结果保持一致，所以代码并非完全规范。若用于替换其它微码的PID.c，还需
  做差异对比并适当修改。
******************************************************************************************/






//************************************** 函数 *****************************************

//电膨序号
typedef enum
{
    EEV_ID_1,       //1#膨胀阀主路
    EEV_ID_2,       //2#膨胀阀辅路
    EEV_ID_MAX      //膨胀阀个数
}EEV_ID_ENUM;

//电膨状态切换命令
typedef enum
{
    EEV_CMD_STOP,           //关闭
    EEV_CMD_RESET,          //复位
    EEV_CMD_DEFRO,          //除霜
    EEV_CMD_PREV,           //初始开度
    EEV_CMD_RUN,            //正常运行
    EEV_CMD_LIMIT_DOWN,     //限制关小
    EEV_CMD_LIMIT_UP,       //限制开大
    EEV_CMD_FORCE_DOWN,     //强制关小
    EEV_CMD_FORCE_UP,       //强制开大
    EEV_CMD_FORCE_FASTUP,   //快速强制开大
    EEV_CMD_CONST,          //不动作
    EEV_CMD_MAX
}EEV_CMD_ENUM;


typedef enum //电子膨胀阀调节状态
{
    EEV_STATE_RETURN_ZERO,  //归零
    EEV_STATE_WAIT_ZERO,    //等待归零
    EEV_STATE_WAIT_CMD,     //等待命令
    EEV_STATE_IDLE,         //待机
    EEV_STATE_DEFRO,        //除霜
    EEV_STATE_PRE_OPEN,     //预开
    EEV_STATE_PRE_KEEP,     //预开维持
    EEV_STATE_RUNNING,      //运行
    EEV_STATE_MAX           //最大状态数
}EEV_STATE_ENUM;

/*******************************************************************************
 函 数: eev_timer()
 功 能: 模块计时函数
 参 数: BOOL TimeFlag 时间标志（用参数传递，以免移植时100ms标志名称不同）
 返 回: 空
 其 它: 在100ms计时处理处调用
*******************************************************************************/
void eev_timer(BOOL TimeFlag);

/*******************************************************************************
 函 数: eev_init()
 功 能: 电膨状态初始化
 参 数: 空
 返 回: 空
 其 它: 在应用程序初始化中调用
*******************************************************************************/
void eev_init(void);

/*******************************************************************************
 函 数: eev_task_pid()
 功 能: 过热度控制
 参 数: id--电膨序号; 
        cmd--指令; 
        pv--当前步数; 
        sv--目标步数; 
 返 回: 空
 其 它: 空
*******************************************************************************/
//void eev_task_pid(EEV_ID_ENUM id, EEV_CMD_ENUM cmd, I16 pv, I16 sv);
void eev_task_pid(PID_ID_ENUM p_id, EEV_ID_ENUM id, EEV_CMD_ENUM cmd, I16 pv, I16 sv);


/*******************************************************************************
 函 数: eev_task_area()
 功 能: 分段控制
 参 数: id--电膨序号; 
        cmd--指令; 
        setting_steps--指定开度; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void eev_task_area(EEV_ID_ENUM id, EEV_CMD_ENUM cmd, U16 setting_steps);

/*******************************************************************************
 函 数: eev_set_steps_idle()
 功 能: 设置待机步数
 参 数: id--电膨序号; 
        value--设置值; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL eev_set_steps_idle(EEV_ID_ENUM id, U16 value);

/*******************************************************************************
 函 数: eev_set_steps_preopen()
 功 能: 设置预开步数
 参 数: id--电膨序号; 
        value--设置值; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL eev_set_steps_preopen(EEV_ID_ENUM id, U16 value);

/*******************************************************************************
 函 数: eev_set_steps_defrost()
 功 能: 设置除霜步数
 参 数: id--电膨序号; 
        value--设置值; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL eev_set_steps_defrost(EEV_ID_ENUM id, U16 value);

/*******************************************************************************
 函 数: eev_set_steps_max()
 功 能: 设置总步数
 参 数: id--电膨序号; 
        value--设置值; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL eev_set_steps_max(EEV_ID_ENUM id, U16 value);

/*******************************************************************************
 函 数: eev_set_steps_run_max()
 功 能: 设置最大运行步数
 参 数: id--电膨序号; 
        value--设置值; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL eev_set_steps_run_max(EEV_ID_ENUM id, U16 value);

/*******************************************************************************
 函 数: eev_set_steps_min()
 功 能: 设置最小步数
 参 数: id--电膨序号; 
        value--设置值; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL eev_set_steps_min(EEV_ID_ENUM id, U16 value);

/*******************************************************************************
 函 数: eev_set_step_speed()
 功 能: 设置励磁频率序号
 参 数: id--电膨序号; 
        value--设置值; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL eev_set_step_speed(EEV_ID_ENUM id, U16 value);


/*******************************************************************************
 函 数: eev_set_motor_type()
 功 能: 设置励磁方式
 参 数: id--电膨序号; 
        value--设置值; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL eev_set_motor_type(EEV_ID_ENUM id, U16 value);


/*******************************************************************************
 函 数: eev_degree_to_steps()
 功 能: 开度转换为步数
 参 数: id--电膨序号; 
        DegreeValue--开度值; 
 返 回: 步数
 其 它: 空
*******************************************************************************/
U16 eev_degree_to_steps(EEV_ID_ENUM id, U16 DegreeValue);

/*******************************************************************************
 函 数: eev_get_step_speed()
 功 能: 获取电膨励磁频率序号
 参 数: id--电膨序号; 

 返 回: 励磁频率序号
 其 它: 无
*******************************************************************************/
U16 eev_get_step_speed(EEV_ID_ENUM id);

/*******************************************************************************
 函 数: eev_get_motor_type()
 功 能: 获取电膨励磁方式
 参 数: id--电膨序号; 

 返 回: 励磁方式
 其 它: 无
*******************************************************************************/
U16 eev_get_motor_type(EEV_ID_ENUM id);


/*******************************************************************************
 函 数: eev_set_period_preopen()
 功 能: 设置预开时间
 参 数: id--电膨序号; 
        value--设置值; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL eev_set_period_preopen(EEV_ID_ENUM id, U16 value);

/*******************************************************************************
 函 数: eev_set_period_calculate()
 功 能: 设置计算周期
 参 数: id--电膨序号; 
        value--设置值; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL eev_set_period_calculate(EEV_ID_ENUM id, U16 value);

/*******************************************************************************
 函 数: eev_set_period_action()
 功 能: 设置动作周期
 参 数: id--电膨序号; 
        value--设置值; 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL eev_set_period_action(EEV_ID_ENUM id, U16 value);
BOOL eev_set_period_add(EEV_ID_ENUM id, U16 value);
BOOL eev_set_steps_add(EEV_ID_ENUM id, U16 value);
BOOL eev_set_period_hold(EEV_ID_ENUM id, U16 value);
BOOL eev_set_period_cyc(EEV_ID_ENUM id, U16 value);
BOOL eev_set_period_sub(EEV_ID_ENUM id, U16 value);
BOOL eev_set_steps_sub(EEV_ID_ENUM id, U16 value);


U16 eev_adj_byExhaustTemp(EEV_ID_ENUM id, U08 x, U16 start_value);
BOOL eev_isReady(EEV_ID_ENUM id);
U16 eev_get_step_fre(EEV_ID_ENUM id);
BOOL eev_holdjudge(BOOL curt_state,EEV_ID_ENUM id);

I16 eev_get_steps_run_max(EEV_ID_ENUM id);
I16 eev_get_steps_min(EEV_ID_ENUM id);
void eev_io_step_read(EEV_ID_ENUM id);
EEV_EXT U16 eev_get_init_step_pv(EEV_ID_ENUM id);

#endif 
