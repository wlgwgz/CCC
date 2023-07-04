/******************************************************************************

          版权所有 (C), 2001-2099, 广州市邦普电脑技术开发有限公司

 ******************************************************************************
  文 件 名   : app_4g.c
  版 本 号   : 初稿
  作    者   : 陈小诚
  生成日期   : 2022年6月9日
  最近修改   :
  功能描述   : 4g应用相关逻辑
  函数列表   :
  修改历史   :
  1.日    期   : 2022年6月9日
    作    者   : 陈小诚
    修改内容   : 创建文件

******************************************************************************/

#ifndef __APP_4G_H__
#define __APP_4G_H__


#ifdef  _APP_4G_GLOBAL_
    #define APP_4G_EXT
#else
    #define APP_4G_EXT extern
#endif
/*-----------------包含头文件 -----------------------------*/



/*-----------------外部变量说明----------------------------*/

// 用于存储从4G模组读到的4G模组信息
typedef struct{
	U16 type;					// 模块类型
	I16 test_sta;				// 测试状态
	U16 csq;					// 信号强度
	I16 run_sta;				// 运行状态
	U16 bak;					// 备用
	U16 ver[MAX_4G_VER_WORD];	// 4G模组功能码版本号
	U16 sn[MAX_4G_SN_WORD];		// 4G模组SN号
}APP4GRW;
APP_4G_EXT APP4GRW app_4g_rw;

// 用于解析以上run_sta
#define APP_4G_STA_NONE          0 // 无
#define APP_4G_STA_NO_MODULE     1 // 无模组
#define APP_4G_STA_CONCAT        2 // 已连接模组
#define APP_4G_STA_CONNECT_NET   3 // 已连接网络
#define APP_4G_STA_CONNECT_CLOUD 4 // 已连接云
#define APP_4G_STA_ALARM         5 // 故障

// 用于存储4G从控制板读到的信息
typedef struct{
	U16 cmd;					// 命令
	U16 bak;					// 备用
}APP4GRO;
APP_4G_EXT APP4GRO app_4g_ro;

// 上面4G命令cmd的值解析
#define INTO_4G_NONE			0	// 无
#define INTO_4G_TEST			1	// 进入4G测试
#define INTO_4G_APP				2	// 进入4G应用


/*-----------------外部函数原型说明------------------------*/



/*-----------------内部函数原型说明------------------------*/

APP_4G_EXT BOOL app_4g_have(void);
APP_4G_EXT U16  app_4g_commIntProt_time(BOOL fg_need_long_time);
APP_4G_EXT void app_4g_commIntProt_deal(void);
APP_4G_EXT void app_4g_init(void);
APP_4G_EXT void app_4g(void);

#endif /* __APP_4G_H__ */
