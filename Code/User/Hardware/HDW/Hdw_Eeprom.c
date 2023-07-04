/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: EEPROM.C
 �ļ�����: ��װEEPM-M2456����
 ������  : 
 ��������: 
            V101 ��־�� 2017.05.05:
                1.�޸�EEPROM���ʺ���,�����˵�ַ�жϺ���.
                2.��Ƭ����ʱ,�������ݶ���Ҫ����
                3.ͬƬ����ʱ,EEPROM��Ϊԭ������+������+����Ҫ������,������ݵ�ַ
                  �Զ��ж��Ƿ���Ҫ����.
                4.�޸��˽���crc����,��Ϊ����ָ��crc����crc�ķ��ʷ���.
                5.�޸��˶�ȡ���ݵ���֤����.
*******************************************************************************/
#define EEPROM_GLOBAL
#include "Mcu_def.h"
#include "Hdw_Main.h"
#include "Hdw_Eeprom.h"
#include "CRC16.h"
#include "debug.h"
#define SET_SCL()       MCU_SetIICSck(1)
#define CLR_SCL()       MCU_SetIICSck(0)

/* ��ʼ��������Ϊ©����·�����д1��Ҳ�ܶ�����(����Ҫ����Ϊ����) */
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

/* IICʱ���������(��λΪns)����ο�оƬ�ֲ� */
/* Vcc = 2.5V-6.0V, STD. MODE, Clock frequency FCLK ��100 kHz */
#define THIGH()         HDW_DelayUs(5)  /* Clock high time  4000 �� 600 */
#define TLOW()          HDW_DelayUs(6)  /* Clock low time TLOW 4700 �� 1300 ��*/
#define THD_STA()       HDW_DelayUs(5)  /* Start condition hold time  4000 �� 600 */
#define TSU_STA()       HDW_DelayUs(6)  /* Start condition setup time  4700 �� 600 */
#define THD_DAT()                       /* Data input hold time 0 �� 0 */
#define TSU_DAT()       HDW_DelayUs(1)  /* Data input setup time  250 �� 100 */
#define TSU_STO()       HDW_DelayUs(5)  /* Stop condition setup time  4000 �� 600 */
#define TAA()           HDW_DelayUs(4)  /* Output valid from clock 3500 �� 900 */
#define TBUF()          HDW_DelayUs(6)  /* Bus free time  4700 �� 1300 */


#define IIC_CMD_WR      0xa0/* ����д */
#define IIC_CMD_RD      0xa1/* ���ƶ� */


#ifndef EEPROM_CONFIG
    #error "����,������ͷ�ļ�����EEPROM�궨��!"
#endif

#ifdef EEPROM_BAK_ON_OTHER_CHIP
    #if EEPADDR_BAK_OFFSET != 0
        #error "����,��Ƭ����ƫ�Ƶ�ַ����Ϊ0!"
    #endif
#else
    #if EEP_CHIP_NOR != EEP_CHIP_BAK
        #error "����,ͬƬ����chip��ַ������ͬ!"
    #endif
#endif

GSTATIC U16 TestAddr;       /* ���Ե�ַ */
GSTATIC U32 cnt_rd_eep;     /* ��ȡ�ƴ� */
GSTATIC U32 cnt_wr_eep;     /* д��ƴ� */

/*******************************************************************************
 �� ��: IIC_Start()
 �� ��: IIC��ʼ�ź�
 �� ��: ��
 �� ��: ��
 �� ��: ��
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
 �� ��: IIC_Stop()
 �� ��: IIC�����ź�
 �� ��: ��
 �� ��: ��
 �� ��: ��
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
 �� ��: IIC_RxAck()
 �� ��: ���մӻ���Ӧ���ź�����?
 �� ��: ��
 �� ��: TRUE/FALSE--���յ�Ӧ����/��Ӧ��
 �� ��: ��
*******************************************************************************/
static BOOL IIC_RxAck(void)
{
    BOOL ack;
        
    CFG_SDA_IN();
    SET_SCL();THIGH();
    ack = !GET_SDA();       /* ע��ȡ�� */
    CLR_SCL();TLOW();
    
    return ack;
}

/*******************************************************************************
 �� ��: IIC_TxAck()
 �� ��: ���ӻ�����Ӧ���ź�
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void IIC_TxAck(void)
{
    CFG_SDA_OUT();
    CLR_SDA();TSU_DAT();
    SET_SCL();THIGH();
    CLR_SCL();TLOW();
}

/*******************************************************************************
 �� ��: IIC_TxNoneAck()
 �� ��: ���ӻ����ͷ�Ӧ���ź�
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void IIC_TxNoneAck(void)
{
    CFG_SDA_OUT();
    SET_SDA();TSU_DAT();
    SET_SCL();THIGH();      
    CLR_SCL();TLOW();
}

/*******************************************************************************
 �� ��: IIC_ReadByte()
 �� ��: �Ӵӻ���һ�ֽ�
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static U08 IIC_ReadByte(void)
{
    U08 i;
    U08 readByte;
    
    CFG_SDA_IN();           /* ���ó�����ģʽ */
    
    readByte = 0;
    for (i=0; i<8; i++)
    {
        SET_SCL();THIGH();  /* ʹ��������Ч */
        readByte <<= 1;     /* �����ƶ�һλ,׼�������µ����� */
        if (GET_SDA())      /* ������ʱ��TAA() */
        {
            readByte |= 0x01;
        }
        CLR_SCL();TLOW();
    }
    return readByte;
}

/*******************************************************************************
 �� ��: IIC_WriteByte()
 �� ��: дһ�ֽڵ��ӻ�
 �� ��: writeByte--д�ֽ�����; 
 �� ��: TRUE/FALSE--�ɹ�(��Ӧ����)/ʧ��
 �� ��: ��
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
 �� ��: IIC_ReadBytes()
 �� ��: ��EEPROW��һ��������
 �� ��: pData--�����׵�ַ; 
        addr--EEPROW�׵�ַ; 
        num--���ݸ���; 
        chip--chip��ַ;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
static BOOL IIC_ReadBytes(U08 *pData, U16 rdaddr, U16 num, U08 chip)
{
    U16 safetyCnt;          /* ��ȫ��������������ѭ�� */
    U16 i;
    U08 chp = chip;
    U16 addr = rdaddr;
    
    cnt_rd_eep++;
    if(num == 0)    return FALSE; /* 0�����أ����num �� �� 1�� */
    

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
        ack = IIC_WriteByte(IIC_CMD_WR|(chp<<1));    /* ���������ź� */
        if (!ack) 
        {
            continue;   
        }
        IIC_WriteByte(HIBYTE(addr));        /* ��ַ��λ */
        ack = IIC_WriteByte(LOBYTE(addr));  /* ��ַ��λ */
        if (!ack) 
        {
            continue;
        }
        
        IIC_Start();
        ack = IIC_WriteByte(IIC_CMD_RD|(chp<<1));    /* �ٴθ��������ź� ���ƶ� */
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
    pData[i] = IIC_ReadByte();              /* �������һ������ */
    IIC_TxNoneAck();
    
    IIC_Stop();
    
    return TRUE;
}
/*******************************************************************************
 �� ��: IIC_WriteBytes()
 �� ��: дһ�������ݵ�EEPROW
 �� ��: pData--�����׵�ַ; 
        addr--EEPROW�׵�ַ; 
        num--���ݸ���; 
        chip--chip��ַ;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
static BOOL IIC_WriteBytes(const U08 *pData, U16 wraddr, U16 num, U08 chip)
{
    const U08 *pDataBak;    /* ��дԴ��ַ */
    U16 addrBak;            /* ��дĿ���ַ */
    U16 numBak;             /* ��д��ʼ���� */
    U16 numRead;
    U08 bufRead[EEPROM_PAGE_SIZE];  
    BOOL ack;
    U32 startTime;
    U08 chp = chip;
    U16 addr = wraddr;
    #define EEPROM_OVET_TIME    500

    cnt_wr_eep++;
    if(num == 0)    return FALSE; /* 0�����أ����num �� �� 1�� */


    startTime = HDW_1ms_Tick();
    while (num != 0)
    {
        WDG_Refresh();
        
        /* ��ʱ�˳� */
        if ((HDW_1ms_Tick() - startTime) > EEPROM_OVET_TIME)
        {
            return FALSE;
        }
        
        /* ���� */
        pDataBak = pData;
        addrBak = addr;
        numBak = num;
        
        /* ��ʼ */
        IIC_Start();
    
        /* ������ */
        ack = IIC_WriteByte(IIC_CMD_WR|(chp<<1));
        if (!ack)
        {
            continue;   
        }
        
        /* ��ַ */
        IIC_WriteByte(HIBYTE(addr));
        ack = IIC_WriteByte(LOBYTE(addr));  
        if (!ack) 
        {
            continue;
        }
        
        /* ˳��д�����е�����(��ҳд) */
        do      
        {
            ack = IIC_WriteByte(*pData);
            if (!ack) 
            {
                /* �ӱ����лָ� */
                pData = pDataBak;
                addr = addrBak;
                num = numBak;       
                break;
            }
            
            num--;
            pData++;
            addr++;
        }while (num && (addr & (EEPROM_PAGE_SIZE-1)));
        
        /* ֹͣ */
        IIC_Stop(); 
        
        /* �ȴ��ڲ�д������� */
        while (1)
        {
            WDG_Refresh();
            
            /* ��ʱ�˳� */
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
        
        /* ��߿ɿ��ԡ���������:�������ݱȽϣ�������ȣ�����д */
        if (numBak != num)
        {
            numRead = numBak-num;
            IIC_ReadBytes(bufRead, addrBak, numRead, chp);
            if (MEMCMP(bufRead, pDataBak, numRead) != 0)
            {
                /* �ӱ����лָ� */
                pData = pDataBak;
                addr = addrBak;
                num = numBak;               
            }
        }
    }
    return TRUE;
}

/*******************************����ΪӦ�ò�***********************************/

/*****************************************************************************
��  �� : sEEP_AddrAssert()
��  �� : ���ʵ�ַ�ж�,��ֹ�������
��  �� : U16 addr    : ������ʼ��ַ
         U16 num     : �����ֽ���
         U08* fg_bak : �Ƿ���Ҫ����ָ��
��  �� : TRUE/FALSE:��ַ��ȷ,��ַ����,fg_bak:�Ƿ񱸷�
��  �� : ��Ƭ����ʱ,����������Ҫ����,ͬƬ����ʱ,��Ϊԭ������+������+����Ҫ������
*****************************************************************************/
static BOOL sEEP_AddrAssert(U16 addr, U16 num, U08* fg_bak)
{
    /* ��Ƭ���� */
    #ifdef EEPROM_BAK_ON_OTHER_CHIP
    {
        /* ��Ƭ��ַ��Ҫ���� */
        if (addr+num <= EEPROM_TOTAL_SIZE)
        {
            *fg_bak = TRUE;
            return TRUE;
        }
    }
    /* ͬƬ���� */
    #else
    {
        /* ԭ��ַ������Ҫ���� */
        if (addr+num <= EEPADDR_BAK_OFFSET)
        {
            *fg_bak = TRUE;
            return TRUE;
        }
        #if 0   /* ���Բ����ж� */
        /* �����������󣬲���ֱ�ӷ��ʱ����� */
        else if (addr >= EEPADDR_BAK_OFFSET && addr+num < EEPADDR_BAK_OFFSET*2)
        {
            *fg_bak = FALSE;
            return FALSE;
        }
        #endif
        /* ����Ҫ�����������ñ��� */
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
 �� ��: eprom_wr_no_crc()
 �� ��: д����ֽڲ���CRC  
 �� ��: pSrc--Ҫд�����ݵ��׵�ַ����ָ���ת��Ϊ(U08 *)
        addr--SPI EEPROM��ַ�ռ��׵�ַ
        num--�ֽڷ��ͳ���(������crc���2���ֽ�) 

 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL eprom_wr_no_crc(const void *pSrc, U16 addr, U16 num)
{
    U08 buf[2];
    U08 fg_bak = FALSE;
    
    if (num == 0)   return FALSE;
    if (!sEEP_AddrAssert(addr, num, &fg_bak)) return FALSE;

    /* д���� ��һ��*/
    if (!IIC_WriteBytes(pSrc, addr, num, EEP_CHIP_NOR)) 
    {
        return FALSE;
    }

    if (fg_bak)
    {
        /* д���� �ڶ���*/
        if (!IIC_WriteBytes(pSrc, addr+EEPADDR_BAK_OFFSET, num, EEP_CHIP_BAK)) 
        {
            return FALSE;
        }
    }

    /* ָ������� */
    IIC_ReadBytes(buf, EEPROMADDR_FREE, 2, EEP_CHIP_NOR);
    
    return  TRUE;
}

/*******************************************************************************
 �� ��: eprom_wr_apnt_crc()
 �� ��: д����ֽ�ָ��CRC��ַ  
 �� ��: pSrc--Ҫд�����ݵ��׵�ַ����ָ���ת��Ϊ(U08 *)
        addr--SPI EEPROM��ַ�ռ��׵�ַ
        num--�ֽڷ��ͳ���(������crc���2���ֽ�) 
        crcaddr--ָ��CRC��ַ
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
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

    /* д���� */
    if (!eprom_wr_no_crc(pSrc, addr, num))
    {
        return FALSE;
    }
    /* дCRC */
    if (!eprom_wr_no_crc(buf, crcaddr, 2))
    {
        return FALSE;
    }
    
    return  TRUE;
}

/*******************************************************************************
 �� ��: eprom_rd_apnt_crc()
 �� ��: ������ֽ�,ָ��CRC��ַ  
 �� ��: pSrc--Ҫд�����ݵ��׵�ַ����ָ���ת��Ϊ(U08 *)
        addr--SPI EEPROM��ַ�ռ��׵�ַ
        num--�ֽڷ��ͳ���(������crc���2���ֽ�) 
        crcaddr--ָ��CRC��ַ
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL eprom_rd_apnt_crc(void *pDst, U16 addr, U16 num,U16 crcaddr)
{
    U08 buf[2];
    W16 crc;
    BOOL ok = FALSE;
    U08 fg_bak = FALSE;

    if (num == 0)   return FALSE;
    if (!sEEP_AddrAssert(addr, num+2, &fg_bak)) return FALSE;

    /* ��ȡ���ݺ���֤���� */
    {
        U08 nor_err = FALSE;/* ԭ������ */
        U08 bak_err = FALSE;/* �������ݴ��� */
        W16 bak_crc;
        
        /* ����1�ݲ��� */
        IIC_ReadBytes(pDst, addr, num,EEP_CHIP_NOR);   
        IIC_ReadBytes(buf, crcaddr, 2,EEP_CHIP_NOR);
        crc.word = CRC16(pDst, num);

        /* �жϵ�1�������Ƿ���ȷ */
        if ((buf[0]!=crc.byte.low) || (buf[1]!=crc.byte.high))
            nor_err = TRUE;

        /* ��Ҫ���ݣ���ȡ���� */
        if (fg_bak)
        {
            /* ����2�ݲ��� */
            IIC_ReadBytes(pDst, addr+EEPADDR_BAK_OFFSET, num,EEP_CHIP_BAK);   
            IIC_ReadBytes(buf, crcaddr+EEPADDR_BAK_OFFSET, 2,EEP_CHIP_BAK);
            bak_crc.word = CRC16(pDst, num);

            /* �жϵ�2�������Ƿ���ȷ */
            if ((buf[0]!=bak_crc.byte.low) || (buf[1]!=bak_crc.byte.high))
                bak_err = TRUE;
            /* ԭ������ȷʱ�����ݺ�ԭ�����Ƿ���ͬ */
            if (!nor_err && bak_crc.word != crc.word)
                bak_err = TRUE;

            /* ԭ���ݴ��� */
            if (nor_err)
            {
                /* ��2�ݲ���CRCУ����ȷ,���޸���1�� */ 
                if (!bak_err)
                {
                    IIC_WriteBytes(pDst, addr, num,EEP_CHIP_NOR);
                    IIC_WriteBytes(buf, crcaddr, 2,EEP_CHIP_NOR);
                }
            }
            else if (bak_err)/* ԭ������ȷ�����ݴ��� */
            {
                /* ����1�ݲ��� */
                IIC_ReadBytes(pDst, addr, num,EEP_CHIP_NOR);   
                IIC_ReadBytes(buf, crcaddr, 2,EEP_CHIP_NOR);
                //crc.word = CRC16(pDst, num);//Ҫ��Ҫ����֤һ�Σ�
                /* �޸���2�� */
                IIC_WriteBytes(pDst, addr+EEPADDR_BAK_OFFSET, num,EEP_CHIP_BAK);
                IIC_WriteBytes(buf, crcaddr+EEPADDR_BAK_OFFSET, 2,EEP_CHIP_BAK);
            }
        }

        /* �����ݶ��������ȡ��ȷ */
        ok = !(nor_err && bak_err);
        
    }
    /* ָ������� */
    IIC_ReadBytes(buf, EEPROMADDR_FREE, 2,EEP_CHIP_NOR);

    return ok;
}

/*******************************************************************************
 �� ��: eprom_rd_near_crc()
 �� ��: ��EEPROM�ж�ȡ�洢�����ݣ�����crcУ��
 �� ��: pDst--���ջ������׵�ַ����ָ���ת��Ϊ(U08 *)
        addr--SPI EEPROM���׵�ַ 
        num--�ֽڽ��ճ���(������crc���2���ֽ�)
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL eprom_rd_near_crc(void *pDst, U16 addr, U16 num)//����Ƭ���ڶ�ƬΪ����
{
    return (eprom_rd_apnt_crc(pDst, addr, num, addr+num));
}

/*******************************************************************************
 �� ��: eprom_wr_near_crc()
 �� ��: ��EEPROM��д������,�������ݺ���д��crc��
 �� ��: pSrc--Ҫд�����ݵ��׵�ַ����ָ���ת��Ϊ(U08 *)
        addr--SPI EEPROM��ַ�ռ��׵�ַ
        num--�ֽڷ��ͳ���(������crc���2���ֽ�) 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL eprom_wr_near_crc(const void *pSrc, U16 addr, U16 num)
{
    U08 buf[EEPROM_PAGE_SIZE];
    W16 crc;
    
    /* Ϊ����д��������������Сʱ(�ж��������ϸ�)��һ��д��Դ������crc�����򣬷ֿ�д��Դ������crc */
    if (num <= (EEPROM_PAGE_SIZE-2))
    {
        MEMCPY(buf, pSrc, num);
        crc.word = CRC16(pSrc, num);
        buf[num] = crc.byte.low;
        buf[num+1] = crc.byte.high; 

        return (eprom_wr_no_crc(buf, addr, num+2));
    }
    else/* CRC�ֿ�д�� */
    {       
        return (eprom_wr_apnt_crc(pSrc, addr, num, addr+num));
    }
}

/*******************************************************************************
 �� ��: eprom_get_test_page()
 �� ��: ��ȡ��ǰ����ҳ
 �� ��: ��
 �� ��: ��ǰ����ҳ(ҳ�Ŵ�1��ʼ)
 �� ��: ��
*******************************************************************************/
U16 eprom_get_test_page(void)
{
    U16 page;
    
    if (TestAddr >= (EEPROMADDR_MAX+1))
    {/* ���Գɹ� */
        page = (U16)(TestAddr/EEPROM_PAGE_SIZE);
    }
    else
    {/* ������(ʵΪ�´β���ҳ)������ʧ��(��ǰʧ��ҳ) */
        page = (U16)(TestAddr/EEPROM_PAGE_SIZE + 1);
    }

    return page;
}

/*******************************************************************************
 �� ��: eprom_get_total_page()
 �� ��: ��ȡ��ҳ��
 �� ��: ��
 �� ��: ��ҳ��
 �� ��: ��
*******************************************************************************/
U16 eprom_get_total_page(void)
{
    return EEPROM_PAGE_TOTAL;
}

/*******************************************************************************
 �� ��: eprom_test()
 �� ��: EEPROM����
 �� ��: ��
 �� ��: ����EEPROM����״̬����
 �� ��: ��
*******************************************************************************/
U08 eprom_test(void)
{
    static U16 s_stauts = EEPROM_TEST_READY;
    U16 cnt, val;
    #define ONEOFF_TEST_SIZE EEPROM_PAGE_SIZE  /* һ���Բ��Դ�С */
    U08 testBuf[ONEOFF_TEST_SIZE];
    U08 fg_import_test = FALSE;                /* ������Ҫ���ݱ�־ */
    static U08 ImportData[ONEOFF_TEST_SIZE];   /* ��Ҫ�����ݱ��� */
    static U08 fg_import_succ = FALSE;         /* ��Ҫ���ݶ�ȡ�ɹ���־ */
    static U08 stauts = 0;
    static U08 chip = EEP_CHIP_NOR;

    if (s_stauts == EEPROM_TEST_READY)
    {
        s_stauts = EEPROM_TEST_PROCESS;
        stauts = 0;
        chip = EEP_CHIP_NOR;
        fg_import_succ = FALSE;
        TestAddr = EEPROMADDR_TEST_BEGIN;
        /* ����׼��״̬���Ա���ó������׼������ */
        return EEPROM_TEST_READY;
    }

    /* ������Ҫ�����ݣ��������Ҫ�ָ��� */
    #if EEPROMADDR_IMPORT_BEGIN == 0
    if (TestAddr <= EEPROMADDR_IMPORT_END)
    #else
    if (EEPROMADDR_IMPORT_BEGIN <= TestAddr && TestAddr <= EEPROMADDR_IMPORT_END)
    #endif
    {
        fg_import_test = TRUE;
    }
    /* ��������Ҫ����ҲҪ�ָ� */
    #if (EEPROMADDR_IMPORT_BEGIN+EEPADDR_BAK_OFFSET) == 0
    if (TestAddr <= EEPROMADDR_IMPORT_END+EEPADDR_BAK_OFFSET)
    #else
    if (EEPROMADDR_IMPORT_BEGIN+EEPADDR_BAK_OFFSET <= TestAddr 
        && TestAddr <= EEPROMADDR_IMPORT_END+EEPADDR_BAK_OFFSET)
    #endif
    {
        fg_import_test = TRUE;
    }
  
    /* д�������� */
    val = (U16)((TestAddr-EEPROMADDR_TEST_BEGIN)/ONEOFF_TEST_SIZE);

    switch(stauts)
    {
        /* ��ȡ��Ҫ���� */
        case 0:
            if (fg_import_test)
            {
                stauts = 1;
                fg_import_succ = (U08)IIC_ReadBytes(ImportData, TestAddr, ONEOFF_TEST_SIZE,chip);
                break;
            }// û��break
        /* д��������� */
        case 1:
            for (cnt=0; cnt<ONEOFF_TEST_SIZE; cnt++)
            {
                testBuf[cnt] = (U08)(cnt + val);
            }

            //���Ե�xƬ
            if (!IIC_WriteBytes(testBuf, TestAddr, ONEOFF_TEST_SIZE,chip))
            {
                /* Ϊ�´β�������׼�� */
                s_stauts = EEPROM_TEST_READY;
                return EEPROM_TEST_WR_ERR;
            }
    
            MEMCLR(testBuf, ONEOFF_TEST_SIZE);
            stauts = 2;
            break;
        /* ���������� */
        case 2:
            if (!IIC_ReadBytes(testBuf, TestAddr, ONEOFF_TEST_SIZE,chip))
            {
                /* Ϊ�´β�������׼�� */
                s_stauts = EEPROM_TEST_READY;
                return EEPROM_TEST_RD_ERR;
            }

            /* �Ƚϲ������� */
            for (cnt=0; cnt<ONEOFF_TEST_SIZE; cnt++)
            {
                if (testBuf[cnt] != (U08)(cnt + val))
                {
                    /* Ϊ�´β�������׼�� */
                    s_stauts = EEPROM_TEST_READY;
                    return EEPROM_TEST_CHK_ERR;
                }
            }
            stauts = 3;
            break;
        /* д����Ҫ���� */
        case 3:
            if (fg_import_test && fg_import_succ) /* �ɹ���д�� */
            {
                stauts = 4;
                IIC_WriteBytes(ImportData, TestAddr, ONEOFF_TEST_SIZE,chip);
            }// û��break
            #ifdef  EEPROM_BAK_ON_OTHER_CHIP
            {
                if (chip == EEP_CHIP_NOR)//ת�����Եڶ�Ƭ
                {
                    chip = EEP_CHIP_BAK;
                    stauts = 0;
                    break;
                }// û��break
            }
            #endif
        /* ת���´β��Ե�ַ */
        case 4:
            TestAddr += ONEOFF_TEST_SIZE;
            chip = EEP_CHIP_NOR;
            stauts = 0;
            
            if (TestAddr > EEPROMADDR_MAX)
            {
                /* Ϊ�´β�������׼�� */
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


/* ֱ�Ӷ�ȡEEPROM�������ڲ��� */
BOOL DBG_EEP_ReadChip(U08 *pData, U16 rdaddr, U16 num, U08 chip)
{
    return(IIC_ReadBytes(pData, rdaddr, num, chip));
}

