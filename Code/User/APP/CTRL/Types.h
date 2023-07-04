/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: Types.h
 �ļ�����: �����������Ͷ���
 ������  : 
 ��������: 
*******************************************************************************/
#ifndef TYPES_H
#define TYPES_H

//#include <Intrins.h>
#include  <STRING.H>
#include  <math.h>

typedef unsigned short  BOOL;
//typedef unsigned char BOOL;
typedef unsigned char   BOL;
typedef unsigned char   STR;
/* �������Ͷ���,KEIL��֧��64λ,���ø���������64λ������ */

#define STDINT int

/* renesas��� */
typedef   signed char   I08;
typedef   signed short  I16;
typedef   signed long   I32;
typedef          double I64;
typedef unsigned char   U08;
typedef unsigned short  U16;
typedef unsigned long   U32;
typedef          double U64;        

/* ucos��� */
typedef   signed char   INT8S;
typedef   signed short  INT16S;
typedef   signed long   INT32S;
typedef          double INT64S;
typedef unsigned char   INT8U;
typedef unsigned short  INT16U;
typedef unsigned long   INT32U;
typedef          double INT64U;     

typedef  const  U08*    PTR_FAR_C08;
typedef  const  U16*    PTR_FAR_C16;
typedef   U08*      PTR_FAR_08;
typedef   U16*      PTR_FAR_16;
/* infenion���,���Ƽ�,ֻ��Ϊ�˼��� */
//typedef   signed char sbyte;
//typedef   signed int  sword;
//typedef   signed long slong;
//typedef unsigned char ubyte; 
//typedef unsigned int  uword;
//typedef unsigned long ulong;      

//****************************************************************************
// @Typedefs
//****************************************************************************

//typedef unsigned char  ubyte;    // 1 byte unsigned; prefix: ub 
//typedef signed char    sbyte;    // 1 byte signed;   prefix: sb 
//typedef unsigned int   uword;    // 2 byte unsigned; prefix: uw 
//typedef signed int     sword;    // 2 byte signed;   prefix: sw 
//typedef unsigned long  ulong;    // 4 byte unsigned; prefix: ul 
//typedef signed long    slong;    // 4 byte signed;   prefix: sl 
//typedef unsigned short ushort;   // 2 byte unsigned; prefix: us 
//typedef signed short   sshort;   // 2 byte signed;   prefix: ss 

//typedef volatile struct
//{
//unsigned int    bit0      : 1;
//unsigned int    bit1      : 1;
//unsigned int    bit2      : 1;
//unsigned int    bit3      : 1;
//unsigned int    bit4      : 1;
//unsigned int    bit5      : 1;
//unsigned int    bit6      : 1;
//unsigned int    bit7      : 1;
//unsigned int    bit8      : 1;
//unsigned int    bit9      : 1;
//unsigned int    bit10     : 1;
//unsigned int    bit11     : 1;
//unsigned int    bit12     : 1;
//unsigned int    bit13     : 1;
//unsigned int    bit14     : 1;
//unsigned int    bit15     : 1;
//}  T_Reg16;


#ifdef USE_GLOBAL_VALUE
#define GSTATIC
#else
#define GSTATIC  static
#endif




/* ���������Ͷ��� */
#ifndef CONST
#define CONST const
#endif

#ifndef FAR
#define FAR far
#endif

/* ����AFARΪ��,����@far */
#define AFAR  //��
#define uchar   unsigned char   /* ����hzk.c���÷�,Ӧ�ó���һ�㲻�� */

/* �������ڲ�sram�������ⲿsram֮ǰʹ�ã��ؼ���sdata��idata��bdata�����ԣ��ɲ���keil��help�� */
#define INDATATYPE  sdata

#define DATA  
#define XDATA 

/* �������Ͷ��� */
/* ��ָ�� */
#ifndef NULL
#define NULL (void *)0
#endif

/* ���ַ� */
#define NUL '\0'

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

typedef void (*PVOIDFUNC)(void);

typedef struct _TIME
{
    U16 sec;
    U16 min;
    U16 hour;
    U16 week;
    U16 day;
    U16 month;
    U16 year;
    U16 time_sta;       /* ��ʱ����ص�״̬(bit0:ʹ�����޵�) */
}TIME, *PTIME;
#define TIME_STA_LMT        0       /* ʹ�����޵�(bit0) */
#define TIME_STA_ERR        1       /* ʱ�Ӵ����־(bit1)�����·�ʱ��󣬻����㣬zzp */

typedef union _W16
{
    U16 word;
    
    /* �뱣֤��CPU�ߵ��ֽڵ�˳��һ�� */
    struct
    {
        U08 low;
        U08 high; 
    }byte;
}W16;

/* λ�� */
typedef union _BIT8
{
    U08 byte;
    struct
    {
        U08 b0  : 1;
        U08 b1  : 1;
        U08 b2  : 1;
        U08 b3  : 1;
        U08 b4  : 1;
        U08 b5  : 1;
        U08 b6  : 1;
        U08 b7  : 1;
    }bits;
}BIT8;

typedef union _BIT16
{
    U16 word;

    /* �뱣֤��CPU�ߵ��ֽڵ�˳��һ�� */
    struct 
    {
        U08 low;
        U08 high; 
    } byte;
    
    struct
    {
        U08 b0  : 1;
        U08 b1  : 1;
        U08 b2  : 1;
        U08 b3  : 1;
        U08 b4  : 1;
        U08 b5  : 1;
        U08 b6  : 1;
        U08 b7  : 1;
        U08 b8  : 1;
        U08 b9  : 1;
        U08 b10 : 1;
        U08 b11 : 1;
        U08 b12 : 1;
        U08 b13 : 1;
        U08 b14 : 1;
        U08 b15 : 1;
    }bits;
}BIT16;

typedef union _BIT32
{
    U32 dword;
    U16 word[2];
    U08 byte[4];
    struct
    {
        U08 b0  : 1;
        U08 b1  : 1;
        U08 b2  : 1;
        U08 b3  : 1;
        U08 b4  : 1;
        U08 b5  : 1;
        U08 b6  : 1;
        U08 b7  : 1;
        U08 b8  : 1;
        U08 b9  : 1;
        U08 b10 : 1;
        U08 b11 : 1;
        U08 b12 : 1;
        U08 b13 : 1;
        U08 b14 : 1;
        U08 b15 : 1;
        U08 b16 : 1;
        U08 b17 : 1;
        U08 b18 : 1;
        U08 b19 : 1;
        U08 b20 : 1;
        U08 b21 : 1;
        U08 b22 : 1;
        U08 b23 : 1;
        U08 b24 : 1;
        U08 b25 : 1;
        U08 b26 : 1;
        U08 b27 : 1;
        U08 b28 : 1;
        U08 b29 : 1;
        U08 b30 : 1;
        U08 b31 : 1;
    }bits;
}BIT32;

/* ����� */
#define BUILD_BUG_ON(c)      ((void)sizeof(char[1-2*!!(c)]))   // ������cΪ��ʱ,�����������
#define BUILD_BUG_ON_ZERO(e) (sizeof(char[1-2*!!(e)]) ? 0:1)   // ������cΪ��ʱ,�����������,��ʱ���Ϊ0


/* ԭ�Ӳ���,ִ���жϱ����Ĵ��� */
#define ATOMIC(code)          do {                      \
                                    __disable_irq();    \
                                    code;               \
                                    __enable_irq();     \
                                    } while(0)
                                    
/* ���ݸ�ʽ */
enum
{
    T_8N1,      /* 8λ���ݣ�1λֹͣ���޼��� */
    T_8N2,      /* 8λ���ݣ�2λֹͣ���޼��� */
    T_8O1,      /* 8λ���ݣ�1λֹͣ������� */
    T_8E1,      /* 8λ���ݣ�1λֹͣ��ż���� */
};

/* ������ */
enum
{
    B_2400,     /* 2400 */
    B_4800,     /* 4800 */
    B_9600,     /* 9600 */
    B_19200,    /* 19200 */
    B_38400,    /* 38400 */
    B_57600,    /* 38400 */
    B_115200,   /* 115200 */
};


/* ���� */
#define _CNT08(cnt)         {if((cnt) != 0xff) (cnt)++;}
#define _CNT16(cnt)         {if((cnt) != 0xffff) (cnt)++;}
#define _CNT32(cnt)         {if((cnt) != 0xffffffff) (cnt)++;}
#define _CNTDOWN(cnt)       {if((cnt) != 0) (cnt)--;}

/* ��ѧ���� */
#define _DIVUINT(a, b)      ((a)/(b))                       /* �޷���ȡ������ */
#define _DIVUCARRY(a, b)    ((a)/(b) + !!((a)%(b)))			/* �޷��Ž�λ���� */
#define _DIVUROUND(a, b)    (((a)+(b)/2)/(b))               /* �޷���������� */


/* ��־���� */
#define _SET(val)           ((val) = 1)
#define _CLR(val)           ((val) = 0)
#define _GET(val)           ((val) != 0)


/* λ���� */
#define GETBIT(var, bit)    ((((var) >> (bit)) & 0x01) != 0)    /* ȡλ */
#define SETBIT(var, bit)    ((var) |= (1 << (bit)))              /* ��λ */
#define CLRBIT(var, bit)    ((var) &= ~(U16)(1 << (bit)))        /* ��λ */
#define TOGGLEBIT(var, bit) ((var) ^= (1 << (bit)))             /* ȡ�� */
#define WRITEBIT(var, bits, newBit) \
        ((newBit)? SETBIT(var,bits) : CLRBIT(var,bits))         /* ��ֵ */
#define GET_BIT(var, bit)		GETBIT(var, bit)	            /* ���ݾ�д�� */
#define SET_BIT(var, bit)		SETBIT(var, bit)
#define CLR_BIT(var, bit)		CLRBIT(var, bit)

#define NUMTOBIT(NUM)       (1 << (NUM))

/* ����λ���� */
#define NBITMASK(n)         ((1<<(n))-1)    /* Nλ������λ */
#define _VALBITS(val)       (sizeof(val)*8)
#define _VALMASK(val)       (sizeof(val)*8 - 1)
#define GETBIT_N(p, n)	    GETBIT((p)[(n)/_VALBITS((p)[0])], (n)&_VALMASK((p)[0]))  /* ȡλ */
#define SETBIT_N(p, n)	    SETBIT((p)[(n)/_VALBITS((p)[0])], (n)&_VALMASK((p)[0]))	/* ��λ */
#define CLRBIT_N(p, n)	    CLRBIT((p)[(n)/_VALBITS((p)[0])], (n)&_VALMASK((p)[0]))	/* ��λ */ 

#define SHIFT_BIT(v, n)     (((v)?1:0)<<(n))

/* �õ�val�ĵ�bitsֵ����bitsΪ4--val&1111b--��val�ĵ�4λֵ */
#define GETLOWBITS(val, bits)   ((val) & ((1<<(bits))-1))   

#define SWAPWORD(x)     (U16)((((x) & 0xFF) << 8) | (((x) >> 8) & 0xFF))

/* ��8λ���8λ���16λ���� */
#define MAKEWORD(high, low) ((U16)((low) | (((U16)(high)) << 8)))   
#define LOBYTE(x)       (U08)(x)            /* ��8λ */
#define HIBYTE(x)       (U08)((U16)(x) >> 8)/* ��8λ */
#define LOW_BYTE(x)		LOBYTE(x)	        /* ���ݾ�д�� */
#define HIGH_BYTE(x)	HIBYTE(x)

/* 4���ֽ����32λ��˫�� */
#define MAKEDWORD(high, mh, ml, low)    \
((U32)((low) | (((U32)(ml)) << 8) | (((U32)(mh)) << 16) | (((U32)(high)) << 24)))
/* 32λ�ֵ�4���ֽ� */
#define DWORDLO(x)      (U08)(x)            /* ���8λ */
#define DWORDML(x)      (U08)((U32)(x)>>8)  /* �м��8λ */
#define DWORDMH(x)      (U08)((U32)(x)>>16) /* �м��8λ */
#define DWORDHI(x)      (U08)((U32)(x)>>24) /* ���8λ */

#undef MAX
#define MAX(a, b) ((a)>=(b)?(a):(b))        /* a\b�����ֵ */

#undef MIN
#define MIN(a, b) ((a)<=(b)?(a):(b))        /* a\b����Сֵ */

/* ����10(��������) */
#define DIVIDE10(val)   ((val)>=0 ? ((val)+5)/10 : ((val)-5)/10)
/* ����100(��������) */
#define DIVIDE100(val)  ((val)>=0 ? ((val)+50)/100 : ((val)-50)/100)
/* ����1000(��������) */
#define DIVIDE1000(val) ((val)>=0 ? ((val)+500)/1000 : ((val)-500)/1000)

/* ���϶�cת���ɻ��϶�f(��������) */
#define C2F(c)  (((c)>=0) ? ((c)*18+5)/10+32 : ((c)*18-5)/10+32)

/* ���϶Ȳ�ֵcDiff��Ϊ��ֵ��ת���ɻ��϶Ȳ�ֵfDiff(��������) */
#define DIFF_C2F(cDiff) (((cDiff)*18+5)/10) 

/* 0.1����(�Ŵ�10��)�����϶�c ת���� 1���ȵĻ��϶�f(��������) */
#define C10_2_F(c)  (((c)>=0) ? ((c)*9+25)/50+32 : ((c)*9-25)/50+32)

/* ���϶�fת�������϶�c(��������) */
#define F2C(f)  (((f)>=32) ? (((f)-32)*10+9)/18 : (((f)-32)*10-9)/18)   

/* ���϶Ȳ�ֵfDiff��Ϊ��ֵ��ת�������϶Ȳ�ֵcDiff(��������) */
#define DIFF_F2C(fDiff) (((fDiff)*10+9)/18) 

/* ���϶�f ת���� ���¶ȵ�λһ�µ��¶�t(��������) */
#define F2T(f)  (PARM_UseMetricUnit() ? (F2C(f)) : (f)) 

/* ���¶ȵ�λһ�µ��¶�t ת���� ���϶�f(��������) */
#define T2F(t)  (PARM_UseMetricUnit() ? (C2F(t)) : (t)) 

#define DO_NOTHING()        /* ��ȷָʾ����� */

/* �洢������(Ŀ���ַ, Դ��ַ, �ֽ���) */
#define MEMCPY(destAddr, srcAddr, bytes) memcpy(destAddr, srcAddr, bytes)               /* δ����crcУ���� */
/* �洢����0(��ַ, �ֽ���) */
#define MEMCLR(addr, bytes) memset(addr, 0, bytes)
/* �洢���Ƚ�(��ַ1, ��ַ2, �ֽ���) */
#define MEMCMP(addr1, addr2, bytes) memcmp(addr1, addr2, bytes)
/* �ַ�������(�ַ���) */
#define STRLEN(str)                         strlen((char*)str)
/* �ַ���ƴ��(�ַ���1���ַ���2) */
#define STRCAT(str1, str2)                  strcat((char*)str1, (char*)str2)
/* �ַ�������(Ŀ���ַ�����Դ���ַ���) */
#define STRCPY(destStr, srcStr)             strcpy((char*)destStr, (char*)srcStr)
/* �ַ�������(Ŀ���ַ�����Դ���ַ���),��ิ��N���ֽ�,����С��Nʱʣ�ಿ��ΪNULL */
#define STRNCPY(destStr, srcStr, maxlen)    strncpy((char*)destStr, (char*)srcStr, maxlen)
/* �ַ����Ƚ�(�ַ���1���ַ���2),���Ƚ�ǰN���ֽ� */
#define STRNCMP(str1, str2, len)            strncmp((char*)str1, (char*)str2, len)
/* �ַ����Ƚ�(�ַ���1���ַ���2) */
#define STRCMP(str1, str2)                  strcmp((char*)str1, (char*)str2)
/* �ַ����ָ�(�ַ���, �ָ���) */
#define STRTOK(str, delim)                  strtok((char*)str, (char*)delim)
/* �ַ�������(�ַ���, ���ַ���) */
#define STRSTR(str, sub)                    strstr((char*)str, (char*)sub)

#define SSCANF                              sscanf

/* ����Ԫ�ظ��� */
#define ARRAY_ELEMENTS(array)   (sizeof(array)/sizeof(array[0]))
#define ELEMENTS(array)                     ARRAY_ELEMENTS(array)
#define ARRELE(array)                       ARRAY_ELEMENTS(array)
/* �����ʼ�� */
#define ARRSET(arr, init)                                                       \
                    do{                                                         \
                        U16 __arr_idx;                                          \
                        for (__arr_idx=0; __arr_idx<ARRELE(arr); __arr_idx++){  \
                            arr[__arr_idx] = init;                              \
                        }                                                       \
                    }while(0)
                    
/* 2ά�����ʼ�� */
#define ARR2SET(arr, init)                                                      \
                    do{                                                         \
                        U16 __arr_idx2;                                          \
                        for (__arr_idx2=0; __arr_idx2<ARRELE(arr); __arr_idx2++){  \
                            ARRSET(arr[__arr_idx2], init);                       \
                        }                                                       \
                    }while(0)       
                    
/* �������� */
#define ARRCLR(array)                       MEMCLR(array, sizeof(array))
/* �������� */
#define VALCLR(value)                       MEMCLR(&value, sizeof(value))
/* ָ������ */
#define PTRCLR(ptr)                         MEMCLR(ptr, sizeof(*(ptr)))

/* �������ֹ���˫�� */
#define MAKEDWORD2(high, low)	((U32)((low) | (((U32)(high)) << 16)))
#define LOWORD(x)		(U16)((U32)(x))			/* ��16λ */
#define HIWORD(x)		(U16)((U32)(x) >> 16)	/* ��16λ */

/* �ַ�������(�ַ���) */
#define STRLEN(str)                         strlen((char*)str)
/* �ַ���ƴ��(�ַ���1���ַ���2) */
#define STRCAT(str1, str2)                  strcat((char*)str1, (char*)str2)
/* �ַ�������(Ŀ���ַ�����Դ���ַ���) */
#define STRCPY(destStr, srcStr)             strcpy((char*)destStr, (char*)srcStr)

/* ��ʱ�� */
#define _MAX_(i)            (sizeof(i)==4?0xFFFFFFFF:(sizeof(i)==2?0xFFFF:0xFF))
#define _SETMAX(val)        ((val) = _MAX_(val))
#define _CNT(i)             {if((i) != _MAX_(i))  (i)++;}	    // ���ϼ���
#define _CNT_WHEN(i, c)     {if(c) {_CNT(i);} else {(i)=0;}}    // ��c����ʱ����,��������
#define _CNT_WITH(i, c)     {if(c) {_CNT(i);}}                  // ��c����ʱ����,������ͣ

/* ȡ��λ */
#define GET_DIGIT(num, bit) ((U16)((num)/pow(10,(bit))) % 10)	/* 0����λ�� */

/* �ظ� */
#define _DATA_1(d)          (d),
#define _DATA_2(d)          (d),(d),
#define _DATA_3(d)          (d),(d),(d),
#define _DATA_4(d)          (d),(d),(d),(d), 

/* ��ȡλ�� */
#define BIT_NUM(i)			(sizeof(i) * 8)

#endif

