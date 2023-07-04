/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: Types.h
 文件描述: 常用数据类型定义
 创建人  : 
 创建日期: 
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
/* 变量类型定义,KEIL不支持64位,故用浮点数代替64位整型数 */

#define STDINT int

/* renesas风格 */
typedef   signed char   I08;
typedef   signed short  I16;
typedef   signed long   I32;
typedef          double I64;
typedef unsigned char   U08;
typedef unsigned short  U16;
typedef unsigned long   U32;
typedef          double U64;        

/* ucos风格 */
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
/* infenion风格,不推荐,只是为了兼容 */
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




/* 编译器类型定义 */
#ifndef CONST
#define CONST const
#endif

#ifndef FAR
#define FAR far
#endif

/* 定义AFAR为空,兼容@far */
#define AFAR  //空
#define uchar   unsigned char   /* 兼容hzk.c的用法,应用程序一般不用 */

/* 定义在内部sram，测试外部sram之前使用，关键字sdata、idata、bdata都可以，可参照keil中help； */
#define INDATATYPE  sdata

#define DATA  
#define XDATA 

/* 其他类型定义 */
/* 空指针 */
#ifndef NULL
#define NULL (void *)0
#endif

/* 空字符 */
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
    U16 time_sta;       /* 与时间相关的状态(bit0:使用期限到) */
}TIME, *PTIME;
#define TIME_STA_LMT        0       /* 使用期限到(bit0) */
#define TIME_STA_ERR        1       /* 时钟错误标志(bit1)，屏下发时间后，会清零，zzp */

typedef union _W16
{
    U16 word;
    
    /* 请保证与CPU高低字节的顺序一致 */
    struct
    {
        U08 low;
        U08 high; 
    }byte;
}W16;

/* 位域 */
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

    /* 请保证与CPU高低字节的顺序一致 */
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

/* 编译宏 */
#define BUILD_BUG_ON(c)      ((void)sizeof(char[1-2*!!(c)]))   // 当条件c为真时,产生编译错误
#define BUILD_BUG_ON_ZERO(e) (sizeof(char[1-2*!!(e)]) ? 0:1)   // 当条件c为真时,产生编译错误,假时结果为0


/* 原子操作,执行中断保护的代码 */
#define ATOMIC(code)          do {                      \
                                    __disable_irq();    \
                                    code;               \
                                    __enable_irq();     \
                                    } while(0)
                                    
/* 数据格式 */
enum
{
    T_8N1,      /* 8位数据，1位停止，无检验 */
    T_8N2,      /* 8位数据，2位停止，无检验 */
    T_8O1,      /* 8位数据，1位停止，奇检验 */
    T_8E1,      /* 8位数据，1位停止，偶检验 */
};

/* 波特率 */
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


/* 计数 */
#define _CNT08(cnt)         {if((cnt) != 0xff) (cnt)++;}
#define _CNT16(cnt)         {if((cnt) != 0xffff) (cnt)++;}
#define _CNT32(cnt)         {if((cnt) != 0xffffffff) (cnt)++;}
#define _CNTDOWN(cnt)       {if((cnt) != 0) (cnt)--;}

/* 数学计算 */
#define _DIVUINT(a, b)      ((a)/(b))                       /* 无符号取整除法 */
#define _DIVUCARRY(a, b)    ((a)/(b) + !!((a)%(b)))			/* 无符号进位除法 */
#define _DIVUROUND(a, b)    (((a)+(b)/2)/(b))               /* 无符号舍入除法 */


/* 标志操作 */
#define _SET(val)           ((val) = 1)
#define _CLR(val)           ((val) = 0)
#define _GET(val)           ((val) != 0)


/* 位操作 */
#define GETBIT(var, bit)    ((((var) >> (bit)) & 0x01) != 0)    /* 取位 */
#define SETBIT(var, bit)    ((var) |= (1 << (bit)))              /* 置位 */
#define CLRBIT(var, bit)    ((var) &= ~(U16)(1 << (bit)))        /* 清位 */
#define TOGGLEBIT(var, bit) ((var) ^= (1 << (bit)))             /* 取反 */
#define WRITEBIT(var, bits, newBit) \
        ((newBit)? SETBIT(var,bits) : CLRBIT(var,bits))         /* 赋值 */
#define GET_BIT(var, bit)		GETBIT(var, bit)	            /* 兼容旧写法 */
#define SET_BIT(var, bit)		SETBIT(var, bit)
#define CLR_BIT(var, bit)		CLRBIT(var, bit)

#define NUMTOBIT(NUM)       (1 << (NUM))

/* 任意位操作 */
#define NBITMASK(n)         ((1<<(n))-1)    /* N位的屏蔽位 */
#define _VALBITS(val)       (sizeof(val)*8)
#define _VALMASK(val)       (sizeof(val)*8 - 1)
#define GETBIT_N(p, n)	    GETBIT((p)[(n)/_VALBITS((p)[0])], (n)&_VALMASK((p)[0]))  /* 取位 */
#define SETBIT_N(p, n)	    SETBIT((p)[(n)/_VALBITS((p)[0])], (n)&_VALMASK((p)[0]))	/* 置位 */
#define CLRBIT_N(p, n)	    CLRBIT((p)[(n)/_VALBITS((p)[0])], (n)&_VALMASK((p)[0]))	/* 清位 */ 

#define SHIFT_BIT(v, n)     (((v)?1:0)<<(n))

/* 得到val的低bits值，如bits为4--val&1111b--即val的低4位值 */
#define GETLOWBITS(val, bits)   ((val) & ((1<<(bits))-1))   

#define SWAPWORD(x)     (U16)((((x) & 0xFF) << 8) | (((x) >> 8) & 0xFF))

/* 高8位与低8位组成16位的字 */
#define MAKEWORD(high, low) ((U16)((low) | (((U16)(high)) << 8)))   
#define LOBYTE(x)       (U08)(x)            /* 低8位 */
#define HIBYTE(x)       (U08)((U16)(x) >> 8)/* 高8位 */
#define LOW_BYTE(x)		LOBYTE(x)	        /* 兼容旧写法 */
#define HIGH_BYTE(x)	HIBYTE(x)

/* 4个字节组成32位的双字 */
#define MAKEDWORD(high, mh, ml, low)    \
((U32)((low) | (((U32)(ml)) << 8) | (((U32)(mh)) << 16) | (((U32)(high)) << 24)))
/* 32位字的4个字节 */
#define DWORDLO(x)      (U08)(x)            /* 最低8位 */
#define DWORDML(x)      (U08)((U32)(x)>>8)  /* 中间低8位 */
#define DWORDMH(x)      (U08)((U32)(x)>>16) /* 中间高8位 */
#define DWORDHI(x)      (U08)((U32)(x)>>24) /* 最高8位 */

#undef MAX
#define MAX(a, b) ((a)>=(b)?(a):(b))        /* a\b的最大值 */

#undef MIN
#define MIN(a, b) ((a)<=(b)?(a):(b))        /* a\b的最小值 */

/* 除以10(四舍五入) */
#define DIVIDE10(val)   ((val)>=0 ? ((val)+5)/10 : ((val)-5)/10)
/* 除以100(四舍五入) */
#define DIVIDE100(val)  ((val)>=0 ? ((val)+50)/100 : ((val)-50)/100)
/* 除以1000(四舍五入) */
#define DIVIDE1000(val) ((val)>=0 ? ((val)+500)/1000 : ((val)-500)/1000)

/* 摄氏度c转换成华氏度f(四舍五入) */
#define C2F(c)  (((c)>=0) ? ((c)*18+5)/10+32 : ((c)*18-5)/10+32)

/* 摄氏度差值cDiff（为正值）转换成华氏度差值fDiff(四舍五入) */
#define DIFF_C2F(cDiff) (((cDiff)*18+5)/10) 

/* 0.1精度(放大10倍)的摄氏度c 转换成 1精度的华氏度f(四舍五入) */
#define C10_2_F(c)  (((c)>=0) ? ((c)*9+25)/50+32 : ((c)*9-25)/50+32)

/* 华氏度f转换成摄氏度c(四舍五入) */
#define F2C(f)  (((f)>=32) ? (((f)-32)*10+9)/18 : (((f)-32)*10-9)/18)   

/* 华氏度差值fDiff（为正值）转换成摄氏度差值cDiff(四舍五入) */
#define DIFF_F2C(fDiff) (((fDiff)*10+9)/18) 

/* 华氏度f 转换成 与温度单位一致的温度t(四舍五入) */
#define F2T(f)  (PARM_UseMetricUnit() ? (F2C(f)) : (f)) 

/* 与温度单位一致的温度t 转换成 华氏度f(四舍五入) */
#define T2F(t)  (PARM_UseMetricUnit() ? (C2F(t)) : (t)) 

#define DO_NOTHING()        /* 明确指示空语句 */

/* 存储器复制(目标地址, 源地址, 字节数) */
#define MEMCPY(destAddr, srcAddr, bytes) memcpy(destAddr, srcAddr, bytes)               /* 未包括crc校验码 */
/* 存储器清0(地址, 字节数) */
#define MEMCLR(addr, bytes) memset(addr, 0, bytes)
/* 存储器比较(地址1, 地址2, 字节数) */
#define MEMCMP(addr1, addr2, bytes) memcmp(addr1, addr2, bytes)
/* 字符串长度(字符串) */
#define STRLEN(str)                         strlen((char*)str)
/* 字符串拼接(字符串1，字符串2) */
#define STRCAT(str1, str2)                  strcat((char*)str1, (char*)str2)
/* 字符串复制(目标字符串，源地字符串) */
#define STRCPY(destStr, srcStr)             strcpy((char*)destStr, (char*)srcStr)
/* 字符串复制(目标字符串，源地字符串),最多复制N个字节,长度小于N时剩余部分为NULL */
#define STRNCPY(destStr, srcStr, maxlen)    strncpy((char*)destStr, (char*)srcStr, maxlen)
/* 字符串比较(字符串1，字符串2),最多比较前N个字节 */
#define STRNCMP(str1, str2, len)            strncmp((char*)str1, (char*)str2, len)
/* 字符串比较(字符串1，字符串2) */
#define STRCMP(str1, str2)                  strcmp((char*)str1, (char*)str2)
/* 字符串分割(字符串, 分隔符) */
#define STRTOK(str, delim)                  strtok((char*)str, (char*)delim)
/* 字符串查找(字符串, 字字符串) */
#define STRSTR(str, sub)                    strstr((char*)str, (char*)sub)

#define SSCANF                              sscanf

/* 数组元素个数 */
#define ARRAY_ELEMENTS(array)   (sizeof(array)/sizeof(array[0]))
#define ELEMENTS(array)                     ARRAY_ELEMENTS(array)
#define ARRELE(array)                       ARRAY_ELEMENTS(array)
/* 数组初始化 */
#define ARRSET(arr, init)                                                       \
                    do{                                                         \
                        U16 __arr_idx;                                          \
                        for (__arr_idx=0; __arr_idx<ARRELE(arr); __arr_idx++){  \
                            arr[__arr_idx] = init;                              \
                        }                                                       \
                    }while(0)
                    
/* 2维数组初始化 */
#define ARR2SET(arr, init)                                                      \
                    do{                                                         \
                        U16 __arr_idx2;                                          \
                        for (__arr_idx2=0; __arr_idx2<ARRELE(arr); __arr_idx2++){  \
                            ARRSET(arr[__arr_idx2], init);                       \
                        }                                                       \
                    }while(0)       
                    
/* 数组清零 */
#define ARRCLR(array)                       MEMCLR(array, sizeof(array))
/* 变量清零 */
#define VALCLR(value)                       MEMCLR(&value, sizeof(value))
/* 指针清零 */
#define PTRCLR(ptr)                         MEMCLR(ptr, sizeof(*(ptr)))

/* 由两个字构成双字 */
#define MAKEDWORD2(high, low)	((U32)((low) | (((U32)(high)) << 16)))
#define LOWORD(x)		(U16)((U32)(x))			/* 低16位 */
#define HIWORD(x)		(U16)((U32)(x) >> 16)	/* 高16位 */

/* 字符串长度(字符串) */
#define STRLEN(str)                         strlen((char*)str)
/* 字符串拼接(字符串1，字符串2) */
#define STRCAT(str1, str2)                  strcat((char*)str1, (char*)str2)
/* 字符串复制(目标字符串，源地字符串) */
#define STRCPY(destStr, srcStr)             strcpy((char*)destStr, (char*)srcStr)

/* 计时宏 */
#define _MAX_(i)            (sizeof(i)==4?0xFFFFFFFF:(sizeof(i)==2?0xFFFF:0xFF))
#define _SETMAX(val)        ((val) = _MAX_(val))
#define _CNT(i)             {if((i) != _MAX_(i))  (i)++;}	    // 向上计数
#define _CNT_WHEN(i, c)     {if(c) {_CNT(i);} else {(i)=0;}}    // 当c满足时计数,否则清零
#define _CNT_WITH(i, c)     {if(c) {_CNT(i);}}                  // 当c满足时计数,否则暂停

/* 取数位 */
#define GET_DIGIT(num, bit) ((U16)((num)/pow(10,(bit))) % 10)	/* 0：个位数 */

/* 重复 */
#define _DATA_1(d)          (d),
#define _DATA_2(d)          (d),(d),
#define _DATA_3(d)          (d),(d),(d),
#define _DATA_4(d)          (d),(d),(d),(d), 

/* 获取位数 */
#define BIT_NUM(i)			(sizeof(i) * 8)

#endif

