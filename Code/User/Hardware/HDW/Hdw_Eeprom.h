/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: EEPROM.H
 文件描述: 
 创建人  : 
 创建日期: 
*******************************************************************************/
#ifndef EEPROM_H
#define EEPROM_H

#ifdef  EEPROM_GLOBAL
    #define EEPROM_EXT
#else
    #define EEPROM_EXT extern
#endif



/* 配置好后面的宏后，开放这个宏 */
#define EEPROM_CONFIG

#ifdef EEPROM_CONFIG
    /* 定义时，异片备份，没定义时，同片备份 */
//    #define EEPROM_BAK_ON_OTHER_CHIP   // HL034A为同片备份

    /* 不同的EEPROM芯片时序略有不同，要根据实际情况处理 */
    #define EEPROM_24256             0
    //#define EEPROM_24LC16            1
#endif


#if defined(EEPROM_24256)
    #define EEPROM_PAGE_SIZE         64                                  /* 每一页的大小(字节) */
    #define EEPROM_PAGE_TOTAL        512                                 /* 总页数 */
    #define EEPROM_TOTAL_SIZE        (EEPROM_PAGE_SIZE*EEPROM_PAGE_TOTAL)/* 总大小 */  
    #define EEPROM_BAK_ADDR          (EEPROM_TOTAL_SIZE/2) /* 备份位置，默认取一半，可修改 */
#elif defined(EEPROM_24LC16)

#else    
    #error "错误,未定义EEPROM芯片型号!"
#endif

    
#ifdef EEPROM_BAK_ON_OTHER_CHIP /* 异片备份 */
    #define EEP_CHIP_NOR        0               /* 原数据chip地址 */
    #define EEP_CHIP_BAK        1               /* 备份数据chip地址 */
    #define EEPADDR_BAK_OFFSET  0               /* 异片备份地址不用偏移 */
#else                           /* 同片备份 */
    #define EEP_CHIP_NOR        0               /* 原数据chip地址 */
    #define EEP_CHIP_BAK        EEP_CHIP_NOR    /* 备份数据chip地址，在同一片 */
    #define EEPADDR_BAK_OFFSET  EEPROM_BAK_ADDR /* 同片备份地址 */
#endif

/*==============================================================================
0x0000~0x003F: 某些特殊的系统级参数、运行中不经常修改与保存，
EEPROM初始化测试时需要读出,测完后写回。 
==============================================================================*/
#define EEPROMADDR_IMPORT_BEGIN     0x0000

#define EEPROMADDR_IMPORT_END       0x003F



/* EEPROM初始化测试时的开始地址 */
#define EEPROMADDR_TEST_BEGIN       0x0000      /* 全部参与测试 */

#define EEPROMADDR_FREE         (EEPROM_TOTAL_SIZE-1)      /* 空闲区地址 */
#define EEPROMADDR_MAX          (EEPROM_TOTAL_SIZE-1)      /* 最大地址，共32K字节 */

/* EEPROM测试状态 */
enum 
{
    EEPROM_TEST_READY,      /* 准备 */
    EEPROM_TEST_PROCESS,        /* 过程中 */
    EEPROM_TEST_OK,         /* 成功 */
    EEPROM_TEST_WR_ERR,     /* 失败:写EEPROM错误 */
    EEPROM_TEST_RD_ERR,     /* 失败:读EEPROM错误 */
    EEPROM_TEST_CHK_ERR,        /* 失败:校验错误(写入值不等于读出值) */
    
    EEPROM_TEST_TYPE_ERR        /* 失败:类型错误 */
};



extern U08 eprom_test(void);
extern U16 eprom_get_test_page(void);
extern U16 eprom_get_total_page(void);
extern BOOL DBG_EEP_ReadChip(U08 *pData, U16 rdaddr, U16 num, U08 chip);
extern BOOL eprom_rd_apnt_crc(void *pDst, U16 addr, U16 num,U16 crcaddr);
extern BOOL eprom_rd_near_crc(void *pDst, U16 addr, U16 num);
extern BOOL eprom_wr_apnt_crc(const void *pSrc, U16 addr, U16 num,U16 crcaddr);
extern BOOL eprom_wr_near_crc(const void *pSrc, U16 addr, U16 num);
extern BOOL eprom_wr_no_crc(const void *pSrc, U16 addr, U16 num);

#endif

