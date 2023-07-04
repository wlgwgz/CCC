/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: EEPROM.H
 �ļ�����: 
 ������  : 
 ��������: 
*******************************************************************************/
#ifndef EEPROM_H
#define EEPROM_H

#ifdef  EEPROM_GLOBAL
    #define EEPROM_EXT
#else
    #define EEPROM_EXT extern
#endif



/* ���úú���ĺ�󣬿�������� */
#define EEPROM_CONFIG

#ifdef EEPROM_CONFIG
    /* ����ʱ����Ƭ���ݣ�û����ʱ��ͬƬ���� */
//    #define EEPROM_BAK_ON_OTHER_CHIP   // HL034AΪͬƬ����

    /* ��ͬ��EEPROMоƬʱ�����в�ͬ��Ҫ����ʵ��������� */
    #define EEPROM_24256             0
    //#define EEPROM_24LC16            1
#endif


#if defined(EEPROM_24256)
    #define EEPROM_PAGE_SIZE         64                                  /* ÿһҳ�Ĵ�С(�ֽ�) */
    #define EEPROM_PAGE_TOTAL        512                                 /* ��ҳ�� */
    #define EEPROM_TOTAL_SIZE        (EEPROM_PAGE_SIZE*EEPROM_PAGE_TOTAL)/* �ܴ�С */  
    #define EEPROM_BAK_ADDR          (EEPROM_TOTAL_SIZE/2) /* ����λ�ã�Ĭ��ȡһ�룬���޸� */
#elif defined(EEPROM_24LC16)

#else    
    #error "����,δ����EEPROMоƬ�ͺ�!"
#endif

    
#ifdef EEPROM_BAK_ON_OTHER_CHIP /* ��Ƭ���� */
    #define EEP_CHIP_NOR        0               /* ԭ����chip��ַ */
    #define EEP_CHIP_BAK        1               /* ��������chip��ַ */
    #define EEPADDR_BAK_OFFSET  0               /* ��Ƭ���ݵ�ַ����ƫ�� */
#else                           /* ͬƬ���� */
    #define EEP_CHIP_NOR        0               /* ԭ����chip��ַ */
    #define EEP_CHIP_BAK        EEP_CHIP_NOR    /* ��������chip��ַ����ͬһƬ */
    #define EEPADDR_BAK_OFFSET  EEPROM_BAK_ADDR /* ͬƬ���ݵ�ַ */
#endif

/*==============================================================================
0x0000~0x003F: ĳЩ�����ϵͳ�������������в������޸��뱣�棬
EEPROM��ʼ������ʱ��Ҫ����,�����д�ء� 
==============================================================================*/
#define EEPROMADDR_IMPORT_BEGIN     0x0000

#define EEPROMADDR_IMPORT_END       0x003F



/* EEPROM��ʼ������ʱ�Ŀ�ʼ��ַ */
#define EEPROMADDR_TEST_BEGIN       0x0000      /* ȫ��������� */

#define EEPROMADDR_FREE         (EEPROM_TOTAL_SIZE-1)      /* ��������ַ */
#define EEPROMADDR_MAX          (EEPROM_TOTAL_SIZE-1)      /* ����ַ����32K�ֽ� */

/* EEPROM����״̬ */
enum 
{
    EEPROM_TEST_READY,      /* ׼�� */
    EEPROM_TEST_PROCESS,        /* ������ */
    EEPROM_TEST_OK,         /* �ɹ� */
    EEPROM_TEST_WR_ERR,     /* ʧ��:дEEPROM���� */
    EEPROM_TEST_RD_ERR,     /* ʧ��:��EEPROM���� */
    EEPROM_TEST_CHK_ERR,        /* ʧ��:У�����(д��ֵ�����ڶ���ֵ) */
    
    EEPROM_TEST_TYPE_ERR        /* ʧ��:���ʹ��� */
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

