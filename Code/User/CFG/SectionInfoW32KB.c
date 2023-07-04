
/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: SectionInfoW32KB.c
 文件描述: 段定义信息,将此信息定位到Work Flash 32KB起始位置,用于告知BOOT区分该段属于
           高256KB还是Work Flash的32KB；
           起因: S6E2H1x系列Flash分为Main Flash(256KB/512KB)和Work Flash(32KB)
                 因生产的plcx文件会将这两段拼接,因此BOOT无法区分高256KB的前32KB
                 是属于512KB后256KB的前32KB还是Work Flash的32KB.
 创建人  : by zzp 
 创建日期: 2021.06.19
*******************************************************************************/
#include "AppInfo.h"

#ifdef USE_WORK_FLASH_32KB
const SECTINFO SectionInfoW32KB  __attribute__ ((used, at(FLASH_WORK_BASE_ADDR)))
= {
    {"SECTADDR"},
    FLASH_WORK_BASE_ADDR,
    ~FLASH_WORK_BASE_ADDR,
};
#error "①使用了32K Work空间,只有空间不足时才可启用,并修改sct文件,启用后关闭此错误,同时打开②错误!"
#else
//#error "②关闭了32K Work空间,请修改sct文件,然后关闭此错误同时打开①错误!"    
#endif

