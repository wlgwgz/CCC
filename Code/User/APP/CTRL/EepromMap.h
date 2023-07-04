/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: EEPROM.H
 文件描述: 
 创建人  : 
 创建日期: 
*******************************************************************************/
#ifndef EEMAP_H
#define EEMAP_H

#ifdef  EEMAP_GLOBAL
    #define EEMAP_EXT
#else
    #define EEMAP_EXT extern
#endif


//#define EEPROMADDR_RESET_CNT        0x0000//复位计数存储地址3+2
#define EEPROMADDR_ADC_CALIBRATE    0x0002  // Adc 校准值4+2
#define EEPROMADDR_ADJUST_AI        0x000A//AI校准参数保存值，4*4*3+2 = 50，结束地址0x31


#endif

