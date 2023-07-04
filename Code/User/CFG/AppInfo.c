/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: AppInfo.c
 文件描述: 定义APP信息(与BOOT下载应用程序相关)，通过链接器定位到0xc04000，
 	下划线前面部分随应用程序升级，下划线后面部分一般不需更改；
 创建人  : 
 创建日期: 
*******************************************************************************/
#include "AppInfo.h"

/* N0004 */
unsigned char const AppInfo[128] __attribute__ ((used, at(APP_INFO_ADDR)))
= {"INIT"APPFUNCODEVER"_V1."BOOT_VER".0.00.FFFF_"};

#ifdef USE_DEBUG
    #warning "DEBUG模式!发布时一定要关闭!!!"
#endif

#ifdef USE_QUICK_TIME
    #warning "注意: 已启用快速时间替代默认时间!发布时一定要关闭!!!"
#endif

#if defined(M_DEBUG)
    #warning "非通用版本!"
    #warning "编译输出 >>>>> 内部调试版本!!!发布时一定要转正!!!"
#elif defined(M_TCL)
    #warning "编译输出 >>>>> 中山TCL版本!!!"
#elif defined(M_CHANGHONG)
    #warning "编译输出 >>>>> 中山长虹版本!!!"
#elif defined(M_TONGYONG)
    #warning "编译输出 >>>>> 通用版本!!!"
#elif defined(M_DISEN)
    #warning "编译输出 >>>>> 广州迪森版本!!!"
#elif defined(M_PAIWO)
    #warning "编译输出 >>>>> 广东派沃版本!!!"
#elif defined(M_NIUENTAI)
    #warning "编译输出 >>>>> 广东纽恩泰版本!!!"
#elif defined(M_SHENGNENG)
    #warning "编译输出 >>>>> 浙江生能版本!!!"
#elif defined(M_HONGHE)
    #warning "编译输出 >>>>> 佛山鸿禾版本!!!"
#elif defined(M_WANHE)
    #warning "编译输出 >>>>> 广东万和版本!!!"
#elif defined(M_CHANGFAN)
    #warning "编译输出 >>>>> 广东长帆版本!!!"
#endif

