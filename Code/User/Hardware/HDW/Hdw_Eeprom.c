/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: EEPROM.C
 文件描述: 封装EEPM-M2456操作
 创建人  : 
 创建日期: 
            V101 周志鹏 2017.05.05:
                1.修改EEPROM访问函数,增加了地址判断函数.
                2.分片备份时,所有数据都需要备份
                3.同片备份时,EEPROM分为原数据区+备份区+不需要备份区,程序根据地址
                  自动判断是否需要备份.
                4.修改了紧随crc函数,改为调用指定crc和无crc的访问方法.
                5.修改了读取备份的验证方法.
*******************************************************************************/
#define EEPROM_GLOBAL
#include "Mcu_def.h"
#include "Hdw_Main.h"
#include "Hdw_Eeprom.h"
#include "CRC16.h"
#include "debug.h"
#define SET_SCL()       MCU_SetIICSck(1)
#define CLR_SCL()       MCU_SetIICSck(0)

/* 初始化中配置为漏极开路输出，写1后也能读引脚(不需要设置为输入) */
#ifdef  SDA_OD
#define CFG_SDA_OUT()
#define CFG_SDA_IN()    MCU_SetIICSda(1)
#else
#define CFG_SDA_OUT()   MCU_CfgIICSda(1)
#define CFG_SDA_IN()    MCU_CfgIICSda(0)
#endif
#define SET_SDA()       MCU_SetIICSda(1)
#define CLR_SDA()       MCU_SetIICSda(0)

#define GET_SDA()       MCU_GetIICSda()

/* IIC时间参数定义(单位为ns)，请参考芯片手册 */
/* Vcc = 2.5V-6.0V, STD. MODE, Clock frequency FCLK —100 kHz */
#define THIGH()         HDW_DelayUs(5)  /* Clock high time  4000 — 600 */
#define TLOW()          HDW_DelayUs(6)  /* Clock low time TLOW 4700 — 1300 —*/
#define THD_STA()       HDW_DelayUs(5)  /* Start condition hold time  4000 — 600 */
#define TSU_STA()       HDW_DelayUs(6)  /* Start condition setup time  4700 — 600 */
#define THD_DAT()                       /* Data input hold time 0 — 0 */
#define TSU_DAT()       HDW_DelayUs(1)  /* Data input setup time  250 — 100 */
#define TSU_STO()       HDW_DelayUs(5)  /* Stop condition setup time  4000 — 600 */
#define TAA()           HDW_DelayUs(4)  /* Output valid from clock 3500 — 900 */
#define TBUF()          HDW_DelayUs(6)  /* Bus free time  4700 — 1300 */


#define IIC_CMD_WR      0xa0/* 控制写 */
#define IIC_CMD_RD      0xa1/* 控制读 */


#ifndef EEPROM_CONFIG
    #error "错误,请先在头文件配置EEPROM宏定义!"
#endif

#ifdef EEPROM_BAK_ON_OTHER_CHIP
    #if EEPADDR_BAK_OFFSET != 0
        #error "错误,异片备份偏移地址必须为0!"
    #endif
#else
    #if EEP_CHIP_NOR != EEP_CHIP_BAK
        #error "错误,同片备份chip地址必须相同!"
    #endif
#endif

GSTATIC U16 TestAddr;       /* 测试地址 */
GSTATIC U32 cnt_rd_eep;     /* 读取计次 */
GSTATIC U32 cnt_wr_eep;     /* 写入计次 */

/*******************************************************************************
 函 数: IIC_Start()
 功 能: IIC开始信号
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
static void IIC_Start(void)
{
    CFG_SDA_OUT();
    CLR_SCL();TLOW();
    SET_SDA();TSU_STA();
    SET_SCL();THIGH();
    CLR_SDA();THD_STA();
    CLR_SCL();TLOW();
}

/*******************************************************************************
 函 数: IIC_Stop()
 功 能: IIC结束信号
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
static void IIC_Stop(void)
{
    CFG_SDA_OUT();
    CLR_SCL();TLOW();   
    CLR_SDA();TSU_STO();
    SET_SCL();THIGH();  
    SET_SDA();TBUF();
    CLR_SCL();TLOW();
}

/*******************************************************************************
 函 数: IIC_RxAck()
 功 能: 接收从机的应答信号了吗?
 参 数: 空
 返 回: TRUE/FALSE--接收到应答了/无应答
 其 它: 空
*******************************************************************************/
static BOOL IIC_RxAck(void)
{
    BOOL ack;
        
    CFG_SDA_IN();
    SET_SCL();THIGH();
    ack = !GET_SDA();       /* 注意取反 */
    CLR_SCL();TLOW();
    
    return ack;
}

/*******************************************************************************
 函 数: IIC_TxAck()
 功 能: 给从机发送应答信号
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
static void IIC_TxAck(void)
{
    CFG_SDA_OUT();
    CLR_SDA();TSU_DAT();
    SET_SCL();THIGH();
    CLR_SCL();TLOW();
}

/*******************************************************************************
 函 数: IIC_TxNoneAck()
 功 能: 给从机发送非应答信号
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
static void IIC_TxNoneAck(void)
{
    CFG_SDA_OUT();
    SET_SDA();TSU_DAT();
    SET_SCL();THIGH();      
    CLR_SCL();TLOW();
}

/*******************************************************************************
 函 数: IIC_ReadByte()
 功 能: 从从机读一字节
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
static U08 IIC_ReadByte(void)
{
    U08 i;
    U08 readByte;
    
    CFG_SDA_IN();           /* 配置成输入模式 */
    
    readByte = 0;
    for (i=0; i<8; i++)
    {
        SET_SCL();THIGH();  /* 使数据线有效 */
        readByte <<= 1;     /* 向左移动一位,准备放入新的数据 */
        if (GET_SDA())      /* 已满足时间TAA() */
        {
            readByte |= 0x01;
        }
        CLR_SCL();TLOW();
    }
    return readByte;
}

/*******************************************************************************
 函 数: IIC_WriteByte()
 功 能: 写一字节到从机
 参 数: writeByte--写字节数据; 
 返 回: TRUE/FALSE--成功(即应答了)/失败
 其 它: 空
*******************************************************************************/
static BOOL IIC_WriteByte(U08 writeByte)
{
    U08  i;
    
    CFG_SDA_OUT();
    for (i=0; i<8; i++)
    {
        if ((writeByte&0x80) != 0)
        {
            SET_SDA();TSU_DAT();
        }
        else
        {
            CLR_SDA();TSU_DAT();
        }
        writeByte <<= 1;
        SET_SCL();THIGH();
        CLR_SCL();TLOW();
    }

    return IIC_RxAck();
}

/*******************************************************************************
 函 数: IIC_ReadBytes()
 功 能: 从EEPROW读一连串数据
 参 数: pData--数据首地址; 
        addr--EEPROW首地址; 
        num--数据个数; 
        chip--chip地址;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
static BOOL IIC_ReadBytes(U08 *pData, U16 rdaddr, U16 num, U08 chip)
{
    U16 safetyCnt;          /* 安全计数器，避免死循环 */
    U16 i;
    U08 chp = chip;
    U16 addr = rdaddr;
    
    cnt_rd_eep++;
    if(num == 0)    return FALSE; /* 0个返回，因此num 是 ≥ 1的 */
    

    safetyCnt = 0;
    while (1)
    {                   
        BOOL ack;

        WDG_Refresh();

        if (safetyCnt++ >= 100)
        {
            return FALSE;
        }
        IIC_Start();
        ack = IIC_WriteByte(IIC_CMD_WR|(chp<<1));    /* 给出控制信号 */
        if (!ack) 
        {
            continue;   
        }
        IIC_WriteByte(HIBYTE(addr));        /* 地址高位 */
        ack = IIC_WriteByte(LOBYTE(addr));  /* 地址低位 */
        if (!ack) 
        {
            continue;
        }
        
        IIC_Start();
        ack = IIC_WriteByte(IIC_CMD_RD|(chp<<1));    /* 再次给出控制信号 控制读 */
        if (ack)
        {
            break;
        }
    }

    for (i=0; i<num-1; i++) 
    {
        pData[i]= IIC_ReadByte();
        IIC_TxAck();
    }
    pData[i] = IIC_ReadByte();              /* 读出最后一个数据 */
    IIC_TxNoneAck();
    
    IIC_Stop();
    
    return TRUE;
}
/*******************************************************************************
 函 数: IIC_WriteBytes()
 功 能: 写一连串数据到EEPROW
 参 数: pData--数据首地址; 
        addr--EEPROW首地址; 
        num--数据个数; 
        chip--chip地址;
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
static BOOL IIC_WriteBytes(const U08 *pData, U16 wraddr, U16 num, U08 chip)
{
    const U08 *pDataBak;    /* 重写源地址 */
    U16 addrBak;            /* 重写目标地址 */
    U16 numBak;             /* 重写起始个数 */
    U16 numRead;
    U08 bufRead[EEPROM_PAGE_SIZE];  
    BOOL ack;
    U32 startTime;
    U08 chp = chip;
    U16 addr = wraddr;
    #define EEPROM_OVET_TIME    500

    cnt_wr_eep++;
    if(num == 0)    return FALSE; /* 0个返回，因此num 是 ≥ 1的 */


    startTime = HDW_1ms_Tick();
    while (num != 0)
    {
        WDG_Refresh();
        
        /* 超时退出 */
        if ((HDW_1ms_Tick() - startTime) > EEPROM_OVET_TIME)
        {
            return FALSE;
        }
        
        /* 备份 */
        pDataBak = pData;
        addrBak = addr;
        numBak = num;
        
        /* 开始 */
        IIC_Start();
    
        /* 控制字 */
        ack = IIC_WriteByte(IIC_CMD_WR|(chp<<1));
        if (!ack)
        {
            continue;   
        }
        
        /* 地址 */
        IIC_WriteByte(HIBYTE(addr));
        ack = IIC_WriteByte(LOBYTE(addr));  
        if (!ack) 
        {
            continue;
        }
        
        /* 顺序写入所有的数据(分页写) */
        do      
        {
            ack = IIC_WriteByte(*pData);
            if (!ack) 
            {
                /* 从备份中恢复 */
                pData = pDataBak;
                addr = addrBak;
                num = numBak;       
                break;
            }
            
            num--;
            pData++;
            addr++;
        }while (num && (addr & (EEPROM_PAGE_SIZE-1)));
        
        /* 停止 */
        IIC_Stop(); 
        
        /* 等待内部写周期完成 */
        while (1)
        {
            WDG_Refresh();
            
            /* 超时退出 */
            if ((HDW_1ms_Tick() - startTime) > EEPROM_OVET_TIME)
            {
                return FALSE;
            }       
            IIC_Start();
            ack = IIC_WriteByte(IIC_CMD_WR|(chp<<1));
            if (ack)
            {
                break;  
            }           
        }
        IIC_Stop(); 
        
        /* 提高可靠性、抗干扰性:读出数据比较，若不相等，则重写 */
        if (numBak != num)
        {
            numRead = numBak-num;
            IIC_ReadBytes(bufRead, addrBak, numRead, chp);
            if (MEMCMP(bufRead, pDataBak, numRead) != 0)
            {
                /* 从备份中恢复 */
                pData = pDataBak;
                addr = addrBak;
                num = numBak;               
            }
        }
    }
    return TRUE;
}

/*******************************以下为应用层***********************************/

/*****************************************************************************
函  数 : sEEP_AddrAssert()
功  能 : 访问地址判断,防止超界访问
参  数 : U16 addr    : 访问起始地址
         U16 num     : 访问字节数
         U08* fg_bak : 是否需要备份指针
返  回 : TRUE/FALSE:地址正确,地址错误,fg_bak:是否备份
其  他 : 分片备份时,所有数据需要备份,同片备份时,分为原数据区+备份区+不需要备份区
*****************************************************************************/
static BOOL sEEP_AddrAssert(U16 addr, U16 num, U08* fg_bak)
{
    /* 分片备份 */
    #ifdef EEPROM_BAK_ON_OTHER_CHIP
    {
        /* 整片地址需要备份 */
        if (addr+num <= EEPROM_TOTAL_SIZE)
        {
            *fg_bak = TRUE;
            return TRUE;
        }
    }
    /* 同片备份 */
    #else
    {
        /* 原地址区，需要备份 */
        if (addr+num <= EEPADDR_BAK_OFFSET)
        {
            *fg_bak = TRUE;
            return TRUE;
        }
        #if 0   /* 可以不用判断 */
        /* 备份区，错误，不能直接访问备份区 */
        else if (addr >= EEPADDR_BAK_OFFSET && addr+num < EEPADDR_BAK_OFFSET*2)
        {
            *fg_bak = FALSE;
            return FALSE;
        }
        #endif
        /* 不需要备份区，不用备份 */
        else if (addr >= EEPADDR_BAK_OFFSET*2 && addr+num <= EEPROM_TOTAL_SIZE)
        {
            *fg_bak = FALSE;
            return TRUE;
        }
    }
    #endif
    
    *fg_bak = FALSE;
    return FALSE;
}

/*******************************************************************************
 函 数: eprom_wr_no_crc()
 功 能: 写多个字节不带CRC  
 参 数: pSrc--要写入数据的首地址，空指针会转换为(U08 *)
        addr--SPI EEPROM地址空间首地址
        num--字节发送长度(不包括crc码的2个字节) 

 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL eprom_wr_no_crc(const void *pSrc, U16 addr, U16 num)
{
    U08 buf[2];
    U08 fg_bak = FALSE;
    
    if (num == 0)   return FALSE;
    if (!sEEP_AddrAssert(addr, num, &fg_bak)) return FALSE;

    /* 写数据 第一份*/
    if (!IIC_WriteBytes(pSrc, addr, num, EEP_CHIP_NOR)) 
    {
        return FALSE;
    }

    if (fg_bak)
    {
        /* 写数据 第二份*/
        if (!IIC_WriteBytes(pSrc, addr+EEPADDR_BAK_OFFSET, num, EEP_CHIP_BAK)) 
        {
            return FALSE;
        }
    }

    /* 指向空闲区 */
    IIC_ReadBytes(buf, EEPROMADDR_FREE, 2, EEP_CHIP_NOR);
    
    return  TRUE;
}

/*******************************************************************************
 函 数: eprom_wr_apnt_crc()
 功 能: 写多个字节指定CRC地址  
 参 数: pSrc--要写入数据的首地址，空指针会转换为(U08 *)
        addr--SPI EEPROM地址空间首地址
        num--字节发送长度(不包括crc码的2个字节) 
        crcaddr--指定CRC地址
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL eprom_wr_apnt_crc(const void *pSrc, U16 addr, U16 num,U16 crcaddr)
{
    U08 buf[2];
    W16 crc;
    U08 fg_bak = FALSE;
    U08 fg_crc_bak = FALSE;
    
    if (num == 0)   return FALSE;
    if (!sEEP_AddrAssert(addr, num, &fg_bak))       return FALSE;
    if (!sEEP_AddrAssert(addr, num, &fg_crc_bak))   return FALSE;
    if (fg_bak != fg_crc_bak)                       return FALSE;
        
    crc.word = CRC16(pSrc, num);
    buf[0] = crc.byte.low;
    buf[1] = crc.byte.high;

    /* 写数据 */
    if (!eprom_wr_no_crc(pSrc, addr, num))
    {
        return FALSE;
    }
    /* 写CRC */
    if (!eprom_wr_no_crc(buf, crcaddr, 2))
    {
        return FALSE;
    }
    
    return  TRUE;
}

/*******************************************************************************
 函 数: eprom_rd_apnt_crc()
 功 能: 读多个字节,指定CRC地址  
 参 数: pSrc--要写入数据的首地址，空指针会转换为(U08 *)
        addr--SPI EEPROM地址空间首地址
        num--字节发送长度(不包括crc码的2个字节) 
        crcaddr--指定CRC地址
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL eprom_rd_apnt_crc(void *pDst, U16 addr, U16 num,U16 crcaddr)
{
    U08 buf[2];
    W16 crc;
    BOOL ok = FALSE;
    U08 fg_bak = FALSE;

    if (num == 0)   return FALSE;
    if (!sEEP_AddrAssert(addr, num+2, &fg_bak)) return FALSE;

    /* 读取数据和验证数据 */
    {
        U08 nor_err = FALSE;/* 原数错误 */
        U08 bak_err = FALSE;/* 备份数据错误 */
        W16 bak_crc;
        
        /* 读第1份参数 */
        IIC_ReadBytes(pDst, addr, num,EEP_CHIP_NOR);   
        IIC_ReadBytes(buf, crcaddr, 2,EEP_CHIP_NOR);
        crc.word = CRC16(pDst, num);

        /* 判断第1份数据是否正确 */
        if ((buf[0]!=crc.byte.low) || (buf[1]!=crc.byte.high))
            nor_err = TRUE;

        /* 需要备份，读取备份 */
        if (fg_bak)
        {
            /* 读第2份参数 */
            IIC_ReadBytes(pDst, addr+EEPADDR_BAK_OFFSET, num,EEP_CHIP_BAK);   
            IIC_ReadBytes(buf, crcaddr+EEPADDR_BAK_OFFSET, 2,EEP_CHIP_BAK);
            bak_crc.word = CRC16(pDst, num);

            /* 判断第2份数据是否正确 */
            if ((buf[0]!=bak_crc.byte.low) || (buf[1]!=bak_crc.byte.high))
                bak_err = TRUE;
            /* 原数据正确时，备份和原数据是否相同 */
            if (!nor_err && bak_crc.word != crc.word)
                bak_err = TRUE;

            /* 原数据错误 */
            if (nor_err)
            {
                /* 第2份参数CRC校验正确,则修复第1份 */ 
                if (!bak_err)
                {
                    IIC_WriteBytes(pDst, addr, num,EEP_CHIP_NOR);
                    IIC_WriteBytes(buf, crcaddr, 2,EEP_CHIP_NOR);
                }
            }
            else if (bak_err)/* 原数据正确，备份错误 */
            {
                /* 读第1份参数 */
                IIC_ReadBytes(pDst, addr, num,EEP_CHIP_NOR);   
                IIC_ReadBytes(buf, crcaddr, 2,EEP_CHIP_NOR);
                //crc.word = CRC16(pDst, num);//要不要再验证一次？
                /* 修复第2份 */
                IIC_WriteBytes(pDst, addr+EEPADDR_BAK_OFFSET, num,EEP_CHIP_BAK);
                IIC_WriteBytes(buf, crcaddr+EEPADDR_BAK_OFFSET, 2,EEP_CHIP_BAK);
            }
        }

        /* 非两份都错误，则读取正确 */
        ok = !(nor_err && bak_err);
        
    }
    /* 指向空闲区 */
    IIC_ReadBytes(buf, EEPROMADDR_FREE, 2,EEP_CHIP_NOR);

    return ok;
}

/*******************************************************************************
 函 数: eprom_rd_near_crc()
 功 能: 从EEPROM中读取存储的内容，且有crc校验
 参 数: pDst--接收缓冲区首地址，空指针会转换为(U08 *)
        addr--SPI EEPROM的首地址 
        num--字节接收长度(不包括crc码的2个字节)
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL eprom_rd_near_crc(void *pDst, U16 addr, U16 num)//读两片，第二片为备份
{
    return (eprom_rd_apnt_crc(pDst, addr, num, addr+num));
}

/*******************************************************************************
 函 数: eprom_wr_near_crc()
 功 能: 往EEPROM中写入数据,且在数据后面写入crc码
 参 数: pSrc--要写入数据的首地址，空指针会转换为(U08 *)
        addr--SPI EEPROM地址空间首地址
        num--字节发送长度(不包括crc码的2个字节) 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL eprom_wr_near_crc(const void *pSrc, U16 addr, U16 num)
{
    U08 buf[EEPROM_PAGE_SIZE];
    W16 crc;
    
    /* 为减少写次数，当个数较小时(判断条件不严格)，一起写入源数据与crc；否则，分开写入源数据与crc */
    if (num <= (EEPROM_PAGE_SIZE-2))
    {
        MEMCPY(buf, pSrc, num);
        crc.word = CRC16(pSrc, num);
        buf[num] = crc.byte.low;
        buf[num+1] = crc.byte.high; 

        return (eprom_wr_no_crc(buf, addr, num+2));
    }
    else/* CRC分开写入 */
    {       
        return (eprom_wr_apnt_crc(pSrc, addr, num, addr+num));
    }
}

/*******************************************************************************
 函 数: eprom_get_test_page()
 功 能: 获取当前测试页
 参 数: 空
 返 回: 当前测试页(页号从1开始)
 其 它: 空
*******************************************************************************/
U16 eprom_get_test_page(void)
{
    U16 page;
    
    if (TestAddr >= (EEPROMADDR_MAX+1))
    {/* 测试成功 */
        page = (U16)(TestAddr/EEPROM_PAGE_SIZE);
    }
    else
    {/* 测试中(实为下次测试页)、测试失败(当前失败页) */
        page = (U16)(TestAddr/EEPROM_PAGE_SIZE + 1);
    }

    return page;
}

/*******************************************************************************
 函 数: eprom_get_total_page()
 功 能: 获取总页数
 参 数: 空
 返 回: 总页数
 其 它: 空
*******************************************************************************/
U16 eprom_get_total_page(void)
{
    return EEPROM_PAGE_TOTAL;
}

/*******************************************************************************
 函 数: eprom_test()
 功 能: EEPROM测试
 参 数: 空
 返 回: 参照EEPROM测试状态定义
 其 它: 空
*******************************************************************************/
U08 eprom_test(void)
{
    static U16 s_stauts = EEPROM_TEST_READY;
    U16 cnt, val;
    #define ONEOFF_TEST_SIZE EEPROM_PAGE_SIZE  /* 一次性测试大小 */
    U08 testBuf[ONEOFF_TEST_SIZE];
    U08 fg_import_test = FALSE;                /* 测试重要数据标志 */
    static U08 ImportData[ONEOFF_TEST_SIZE];   /* 重要的数据保存 */
    static U08 fg_import_succ = FALSE;         /* 重要数据读取成功标志 */
    static U08 stauts = 0;
    static U08 chip = EEP_CHIP_NOR;

    if (s_stauts == EEPROM_TEST_READY)
    {
        s_stauts = EEPROM_TEST_PROCESS;
        stauts = 0;
        chip = EEP_CHIP_NOR;
        fg_import_succ = FALSE;
        TestAddr = EEPROMADDR_TEST_BEGIN;
        /* 返回准备状态，以便调用程序进行准备处理 */
        return EEPROM_TEST_READY;
    }

    /* 测试重要的数据，测完后需要恢复的 */
    #if EEPROMADDR_IMPORT_BEGIN == 0
    if (TestAddr <= EEPROMADDR_IMPORT_END)
    #else
    if (EEPROMADDR_IMPORT_BEGIN <= TestAddr && TestAddr <= EEPROMADDR_IMPORT_END)
    #endif
    {
        fg_import_test = TRUE;
    }
    /* 备份区重要数据也要恢复 */
    #if (EEPROMADDR_IMPORT_BEGIN+EEPADDR_BAK_OFFSET) == 0
    if (TestAddr <= EEPROMADDR_IMPORT_END+EEPADDR_BAK_OFFSET)
    #else
    if (EEPROMADDR_IMPORT_BEGIN+EEPADDR_BAK_OFFSET <= TestAddr 
        && TestAddr <= EEPROMADDR_IMPORT_END+EEPADDR_BAK_OFFSET)
    #endif
    {
        fg_import_test = TRUE;
    }
  
    /* 写测试数据 */
    val = (U16)((TestAddr-EEPROMADDR_TEST_BEGIN)/ONEOFF_TEST_SIZE);

    switch(stauts)
    {
        /* 读取重要数据 */
        case 0:
            if (fg_import_test)
            {
                stauts = 1;
                fg_import_succ = (U08)IIC_ReadBytes(ImportData, TestAddr, ONEOFF_TEST_SIZE,chip);
                break;
            }// 没有break
        /* 写入测试数据 */
        case 1:
            for (cnt=0; cnt<ONEOFF_TEST_SIZE; cnt++)
            {
                testBuf[cnt] = (U08)(cnt + val);
            }

            //测试第x片
            if (!IIC_WriteBytes(testBuf, TestAddr, ONEOFF_TEST_SIZE,chip))
            {
                /* 为下次测试做好准备 */
                s_stauts = EEPROM_TEST_READY;
                return EEPROM_TEST_WR_ERR;
            }
    
            MEMCLR(testBuf, ONEOFF_TEST_SIZE);
            stauts = 2;
            break;
        /* 读测试数据 */
        case 2:
            if (!IIC_ReadBytes(testBuf, TestAddr, ONEOFF_TEST_SIZE,chip))
            {
                /* 为下次测试做好准备 */
                s_stauts = EEPROM_TEST_READY;
                return EEPROM_TEST_RD_ERR;
            }

            /* 比较测试数据 */
            for (cnt=0; cnt<ONEOFF_TEST_SIZE; cnt++)
            {
                if (testBuf[cnt] != (U08)(cnt + val))
                {
                    /* 为下次测试做好准备 */
                    s_stauts = EEPROM_TEST_READY;
                    return EEPROM_TEST_CHK_ERR;
                }
            }
            stauts = 3;
            break;
        /* 写回重要数据 */
        case 3:
            if (fg_import_test && fg_import_succ) /* 成功才写回 */
            {
                stauts = 4;
                IIC_WriteBytes(ImportData, TestAddr, ONEOFF_TEST_SIZE,chip);
            }// 没有break
            #ifdef  EEPROM_BAK_ON_OTHER_CHIP
            {
                if (chip == EEP_CHIP_NOR)//转到测试第二片
                {
                    chip = EEP_CHIP_BAK;
                    stauts = 0;
                    break;
                }// 没有break
            }
            #endif
        /* 转到下次测试地址 */
        case 4:
            TestAddr += ONEOFF_TEST_SIZE;
            chip = EEP_CHIP_NOR;
            stauts = 0;
            
            if (TestAddr > EEPROMADDR_MAX)
            {
                /* 为下次测试做好准备 */
                s_stauts = EEPROM_TEST_READY;
                stauts = 0;
                return EEPROM_TEST_OK;
            }
            break;
            
        default:
            stauts = 0;
            break;

    }

    return EEPROM_TEST_PROCESS;
}


/* 直接读取EEPROM，仅用于测试 */
BOOL DBG_EEP_ReadChip(U08 *pData, U16 rdaddr, U16 num, U08 chip)
{
    return(IIC_ReadBytes(pData, rdaddr, num, chip));
}

