/******************************************************************************

          版权所有 (C), 2001-2014, 广州市邦普电脑技术开发有限公司

 ******************************************************************************
  文 件 名   : Mcu_GPIO.c
  版 本 号   : 初稿
  作    者   : // zzp v160a7
  生成日期   : 2016年11月8日星期二
  最近修改   :
  功能描述   : 处理GPIO口
  函数列表   :
  修改历史   :
  1.日    期   : 2016年11月8日星期二
    作    者   : // zzp v160a7
    修改内容   : 创建文件

******************************************************************************/

#include "Mcu_def.h"
#include "Hdw_Main.h"


/*****************************************************************************
函  数 : MCU_ClrAllDo()
功  能 : 关闭所有输出
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
void MCU_ClrAllDo(void)
{
    WriteGpioPin(IO_OUT_00, 0);
    WriteGpioPin(IO_OUT_01, 0);
    WriteGpioPin(IO_OUT_02, 0);
    WriteGpioPin(IO_OUT_03, 0);
    WriteGpioPin(IO_OUT_05, 0);
    WriteGpioPin(IO_OUT_06, 0);
    WriteGpioPin(IO_OUT_07, 0);
    WriteGpioPin(IO_OUT_08, 0);
    WriteGpioPin(IO_OUT_09, 0);
    WriteGpioPin(IO_OUT_10, 0);
    WriteGpioPin(IO_OUT_11, 0);
    WriteGpioPin(IO_OUT_12, 0);
    WriteGpioPin(IO_OUT_13, 0);
    WriteGpioPin(IO_OUT_14, 0);
    WriteGpioPin(IO_OUT_15, 0);
    WriteGpioPin(IO_OUT_16, 0);
    WriteGpioPin(IO_OUT_17, 0);
    WriteGpioPin(IO_OUT_18, 0);
}

void MCU_SetDo(U16 Do_buf[])
{
    U16 buf = 0;    /* 先存到buf中，节省指令数 */
    
#define GetDo(n)   (GETBIT(buf, (n)&0x0f))  

    buf = Do_buf[0];

    WriteGpioPin(IO_OUT_00, GetDo(0));
    WriteGpioPin(IO_OUT_01, GetDo(1));
    WriteGpioPin(IO_OUT_02, GetDo(2));
    WriteGpioPin(IO_OUT_03, GetDo(3));
    WriteGpioPin(IO_OUT_05, GetDo(4));
    WriteGpioPin(IO_OUT_06, GetDo(5));
    WriteGpioPin(IO_OUT_07, GetDo(6));
    WriteGpioPin(IO_OUT_08, GetDo(7));
    WriteGpioPin(IO_OUT_09, GetDo(8));
    WriteGpioPin(IO_OUT_10, GetDo(9));
    WriteGpioPin(IO_OUT_11, GetDo(10));
    WriteGpioPin(IO_OUT_12, GetDo(11));
    WriteGpioPin(IO_OUT_13, GetDo(12));
    WriteGpioPin(IO_OUT_14, GetDo(13));
    WriteGpioPin(IO_OUT_15, GetDo(14));
    WriteGpioPin(IO_OUT_16, GetDo(15));
	
    buf = Do_buf[1];
	
    WriteGpioPin(IO_OUT_17, GetDo(0));
    WriteGpioPin(IO_OUT_18, GetDo(1));
	
    // 每满16位读一个字到buf中
}

#if MAX_MOTOR_NUM != 0
/*****************************************************************************
函  数 : MCU_SetEev1Step()
功  能 : 
参  数 : U08 num  : 
         U08 addr : 
返  回 : 无
其  他 : 无
*****************************************************************************/
void MCU_SetEevStep(U08 num, U08 step)
{
    switch (num)
	{
		case 0:
            WriteGpioPin(IO_STEP_01 , GETBIT(step, 3));
            WriteGpioPin(IO_STEP_02 , GETBIT(step, 2));
            WriteGpioPin(IO_STEP_03 , GETBIT(step, 1));
            WriteGpioPin(IO_STEP_04 , GETBIT(step, 0));
			break;
		case 1:
            WriteGpioPin(IO_STEP_05 , GETBIT(step, 3));
            WriteGpioPin(IO_STEP_06 , GETBIT(step, 2));
            WriteGpioPin(IO_STEP_07 , GETBIT(step, 1));
            WriteGpioPin(IO_STEP_08 , GETBIT(step, 0));
			break;
	} 
}
#endif

/*****************************************************************************
函  数 : MCU_SetLed()
功  能 : LED亮灭
参  数 : U16 Leds : led位标志
返  回 : 无
其  他 : 无
*****************************************************************************/
void MCU_SetLed(U16 Led, U08 Sta)
{
    switch (Led)
    {
        case LED_RED_IDX:
            WriteGpioPin(LED_OUT_RED, Sta==LED_STA_ON?0:1); //输出0为亮灯
            break;
        case LED_ORG_IDX:
            WriteGpioPin(LED_OUT_ORG, Sta==LED_STA_ON?0:1); //输出0为亮灯
            break;
    }
}

/*****************************************************************************
函  数 : MCU_SetNtcChannel()
功  能 : 
参  数 : U08 addr : 
返  回 : 无
其  他 : 无
*****************************************************************************/
void MCU_SetNtcChannel(U08 addr)
{
//  WriteGpioPin(IO_NTC_CH_EN, GETBIT(addr, 3));
    WriteGpioPin(IO_NTC_CH_C , GETBIT(addr, 2));
    WriteGpioPin(IO_NTC_CH_B , GETBIT(addr, 1));
    WriteGpioPin(IO_NTC_CH_A , GETBIT(addr, 0));
}

#if 0
/*****************************************************************************
函  数 : MCU_SetNtcPga()
功  能 : 
参  数 : U08 addr : 
返  回 : 无
其  他 : 无
*****************************************************************************/
void MCU_SetNtcPga(U08 addr)
{

    GETBIT(addr,1) \
      ?  GPIO_SetOutputBit(NTC_PGA_B) \
       : GPIO_ClrOutputBit(NTC_PGA_B);

    GETBIT(addr,0) \
      ?  GPIO_SetOutputBit(NTC_PGA_A) \
       : GPIO_ClrOutputBit(NTC_PGA_A);

}

void MCU_SetLed(U16 LedSta)
{

    GETBIT(LedSta,0) \
      ?  GPIO_ClrOutputBit(LED_OUT_RED) \   // 输出0为亮灯
       : GPIO_SetOutputBit(LED_OUT_RED);    // 输出1为灭灯

    GETBIT(LedSta,1) \
      ?  GPIO_ClrOutputBit(LED_OUT_ORG) \   // 输出0为亮灯
       : GPIO_SetOutputBit(LED_OUT_ORG);    // 输出1为灭灯       

}

/*****************************************************************************
函  数 : MCU_GetDi()
功  能 : 读取开关量引脚信号
参  数 : U16 Di_buf[] : 
返  回 : 无
其  他 : 无
*****************************************************************************/
void MCU_GetDi(U16 Di_buf[])
{
    U16 buf = 0;    /* 先存到buf中，节省指令数 */

#define SetDi(n)   (SETBIT(buf, (n)&0x0f))  
#define ClrDi(n)   (CLRBIT(buf, (n)&0x0f))  

    buf = 0;
    
    GPIO_ReadInputBit(IO_IN_01) \
      ?  SetDi(0) : ClrDi(0);

    GPIO_ReadInputBit(IO_IN_02) \
      ?  SetDi(1) : ClrDi(1);

    GPIO_ReadInputBit(IO_IN_03) \
      ?  SetDi(2) : ClrDi(2);

    GPIO_ReadInputBit(IO_IN_04) \
      ?  SetDi(3) : ClrDi(3);    
    
    GPIO_ReadInputBit(IO_IN_05) \
      ?  SetDi(4) : ClrDi(4);

    GPIO_ReadInputBit(IO_IN_06) \
      ?  SetDi(5) : ClrDi(5);

	/* N0004 */
    GPIO_ReadInputBit(IO_IN_07) \
      ?  SetDi(6) : ClrDi(6);

    Di_buf[0] = buf;    // 每满16位保存到Di_buf[]
}



/*****************************************************************************
函  数 : MCU_GetSwitch()
功  能 : 读取拨码引脚信号
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
U16 MCU_GetSwitch(void)
{
    U16 Sw = 0;  

    GPIO_ReadInputBit(SW_IN_01) \
      ?  SETBIT(Sw, 0) : CLRBIT(Sw, 0);

    GPIO_ReadInputBit(SW_IN_02) \
      ?  SETBIT(Sw, 1) : CLRBIT(Sw, 1);

    GPIO_ReadInputBit(SW_IN_03) \
      ?  SETBIT(Sw, 2) : CLRBIT(Sw, 2);

    GPIO_ReadInputBit(SW_IN_04) \
      ?  SETBIT(Sw, 3) : CLRBIT(Sw, 3);
    
    GPIO_ReadInputBit(SW_IN_05) \
      ?  SETBIT(Sw, 4) : CLRBIT(Sw, 4);
	/* N0004 */
    GPIO_ReadInputBit(SW_IN_06) \
      ?  SETBIT(Sw, 5) : CLRBIT(Sw, 5);

   return Sw;
}
#endif

/*****************************************************************************
函  数 : MCU_GetInput74HC165()
功  能 : 读取开关量引脚信号
参  数 : U16 Di_buf[] : 输入数据地址
返  回 : 无
其  他 : 由74H165并转串
*****************************************************************************/
void MCU_GetInput74HC165(U16 In_buf[])
{
#if IN_74HC165_BITS > 0
	U08 i;
	U16 inHC165[IN_74HC165_W];

    ARRCLR(inHC165);
    
	/* 读取74HC165转化输入:开关量、跳线、拨盘
	具体延时时间请参考74HC165手册与NOP延时时间 */
	IN_HC165_SH_L();
    ASM_NOP();ASM_NOP();ASM_NOP();ASM_NOP();ASM_NOP();ASM_NOP();
    ASM_NOP();ASM_NOP();ASM_NOP();ASM_NOP();ASM_NOP();ASM_NOP();
    ASM_NOP();ASM_NOP();ASM_NOP();ASM_NOP();ASM_NOP();ASM_NOP();
	for (i=0; i<IN_74HC165_BITS; i++)
	{
		if (IN_HC165_QH_ISH())
		{
			SETBIT_N(inHC165, i);
		}
        else
        {
            CLRBIT_N(inHC165, i);
        }
		IN_HC165_SH_H();
        ASM_NOP();ASM_NOP();ASM_NOP();ASM_NOP();ASM_NOP();ASM_NOP();
        ASM_NOP();ASM_NOP();ASM_NOP();ASM_NOP();ASM_NOP();ASM_NOP();
        ASM_NOP();ASM_NOP();ASM_NOP();ASM_NOP();ASM_NOP();ASM_NOP();
		IN_HC165_CLK_L();
        ASM_NOP();ASM_NOP();ASM_NOP();ASM_NOP();ASM_NOP();ASM_NOP();
        ASM_NOP();ASM_NOP();ASM_NOP();ASM_NOP();ASM_NOP();ASM_NOP();
        ASM_NOP();ASM_NOP();ASM_NOP();ASM_NOP();ASM_NOP();ASM_NOP();
		IN_HC165_CLK_H();
        ASM_NOP();ASM_NOP();ASM_NOP();ASM_NOP();ASM_NOP();ASM_NOP();
        ASM_NOP();ASM_NOP();ASM_NOP();ASM_NOP();ASM_NOP();ASM_NOP();
        ASM_NOP();ASM_NOP();ASM_NOP();ASM_NOP();ASM_NOP();ASM_NOP();
	}	

    for (i=0; i<IN_74HC165_W; i++)
    {
        In_buf[i] = inHC165[i];
    }
#endif
}

/*****************************************************************************
函  数 : MCU_GetHDi()
功  能 : 读取强电开关量引脚信号
参  数 : U16 HDi_buf[] : 
返  回 : 无
其  他 : 无
*****************************************************************************/
void MCU_GetHDi(U16 HDi_buf[])
{
	U08 i = 0;
	U16 HdiCPU[MAX_HDI] = {0};  

	/* 读取直连CPU开关量输入 */ 
	if (ReadGpioPin(IO_HIN_01))
	{
		SETBIT(HdiCPU[0>>4], (0&0x0f));
	} 
	if (ReadGpioPin(IO_HIN_02))
	{
		SETBIT(HdiCPU[1>>4], (1&0x0f));
	}
	if (ReadGpioPin(IO_HIN_03))
	{
		SETBIT(HdiCPU[2>>4], (2&0x0f));
	}
	if (ReadGpioPin(IO_HIN_04))
	{
		SETBIT(HdiCPU[3>>4], (3&0x0f));
	}
	if (ReadGpioPin(IO_HIN_05))
	{
		SETBIT(HdiCPU[4>>4], (4&0x0f));
	}

	for (i=0; i<MAX_HDI; i++)
	{
		HDi_buf[i] = HdiCPU[i]; 
	}
    
}

#if 0
/*****************************************************************************
函  数 : MCU_GetPhase()
功  能 : 读取相序当前输入信号 
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
U08 MCU_GetPhase(void)
{
    U08 Ph = 0;

    WRITEBIT(Ph ,0, ReadGpioPin(PH_IN_01));
    WRITEBIT(Ph ,1, ReadGpioPin(PH_IN_02));

    return Ph;
}
#endif

void MCU_SetIICSck(U08 sta)
{
    WriteGpioPin(IIC_SCL_OUT, sta);
}

void MCU_SetIICSda(U08 sta)
{
    WriteGpioPin(IIC_SDA_OUT, sta);
}

BOL MCU_GetIICSda(void)
{
    return ReadGpioPin(IIC_SDA_IN);
}

void MCU_CfgIICSda(U08 sta)
{
    if(sta == 0) 
    {
        IIC_CFG_SDA_IN();      // 0输入
    }
    else
    {
        IIC_CFG_SDA_OUT();     // 1输出
    }
}

BOL MCU_GetPwcIn(void)
{
    return ReadGpioPin(IO_PWC_IN);    
}

void MCU_Set4GPower(U08 sta)
{
    WriteGpioPin(IO_4G_POWER, sta);
}


void MCU_Set4GReset(U08 sta)
{
    WriteGpioPin(IO_4G_RESET, sta);
}

void MCU_GPIO_Init(void)
{
//    U16 i;
//    U16 j;

    // 其余都在sPaCWiz代码生成工具生成,见iomux.c
    // Nothing();


	// 必须通过函数初始化,通过宏不行
    Vbat_InitGpioOutput(VbatGpioP46, 0u, FALSE);
    Vbat_InitGpioOutput(VbatGpioP47, 0u, FALSE);
    Vbat_InitGpioOutput(VbatGpioP48, 0u, FALSE);
    Vbat_InitGpioOutput(VbatGpioP49, 0u, FALSE);
}


