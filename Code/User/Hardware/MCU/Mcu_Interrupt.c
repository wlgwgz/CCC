

#include "includes.h"





extern void MCU_SysTickCallback(void);


/*******************************************************************************
 函 数: HW_IT_Init()
 功 能: 中断初始化
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void MCU_IT_Init(void)
{
	extern unsigned char const AppInfo[];

	SCB->VTOR = APP_VECT_TAB_OFFSET; /* 设置中断向量表 */
    
	/* 该语句只是引用下app_msg，若未使用app_msg，编译器可能删掉app_msg(与编译器有关) */
	if (AppInfo[0] != 0)
	{
		ASM_NOP();
	}	

    #ifdef USE_WORK_FLASH_32KB
    {
        extern const SECTINFO SectionInfoW32KB;
        if (SectionInfoW32KB.info[0] != 0)   ASM_NOP();
    }
    #endif
}

/*****************************************************************************
函  数 : SysTick_Handler()
功  能 : 系统滴答时钟,1ms定时
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
void SysTick_Handler(void)
{
    HDW_1ms_Deal();
	USER_1ms_Deal();/* wjs 增加暖通该函数，但通讯相关函数应该放在硬件中吗? */
#ifdef USE_HDW_DEBUG
	DBG_1ms_Deal();
#endif
}

void Uart0RxIntCallback(void)
{
	MCU_UART_IRQHandlerRx(MCU_UART0_NUM);
}
void Uart0TxIntCallback(void)
{
	MCU_UART_IRQHandlerTx(MCU_UART0_NUM);
}
void Uart1RxIntCallback(void)
{
	MCU_UART_IRQHandlerRx(MCU_UART1_NUM);
}
void Uart1TxIntCallback(void)
{
	MCU_UART_IRQHandlerTx(MCU_UART1_NUM);
}
void Uart4RxIntCallback(void)
{
	MCU_UART_IRQHandlerRx(MCU_UART4_NUM);
}
void Uart4TxIntCallback(void)
{      
	MCU_UART_IRQHandlerTx(MCU_UART4_NUM);
}
void Uart5RxIntCallback(void)
{
	MCU_UART_IRQHandlerRx(MCU_UART5_NUM);
}
void Uart5TxIntCallback(void)
{   
#if defined(USE_LOG_PRINT)
    if(LOG_IsEnable())
            LOG_TrasInt();
    else    
#endif   
	MCU_UART_IRQHandlerTx(MCU_UART5_NUM);
}
void Uart6RxIntCallback(void)
{
	MCU_UART_IRQHandlerRx(MCU_UART6_NUM);
}
void Uart6TxIntCallback(void)
{
	MCU_UART_IRQHandlerTx(MCU_UART6_NUM);
}

void LvdCallback(void)
{
    MCU_VbatDeinit();
    MCU_ClrAllDo();
    NVIC_SystemReset();
}

#if 0
void SVD_IRQHandler(void)
{
    MCU_SVD_IRQHandler();
}
#endif



#ifdef USE_ALL_IRQ

//#define dbg_irq  ((char*)(0x20040000))
int dbg_irq[100];


void                CSV_IRQHandler (void){dbg_irq[ 0]++;while(1);} //   ; IRQ #0 
void                SWDT_IRQHandler(void){dbg_irq[ 1]++;while(1);} //   ; IRQ #1 
void                LVD_IRQHandler (void){dbg_irq[ 2]++;while(1);} //   ; IRQ #2 
void        IRQ003SEL_IRQHandler   (void){dbg_irq[ 3]++;while(1);} //   ; IRQ #3 
void        IRQ004SEL_IRQHandler   (void){dbg_irq[ 4]++;while(1);} //   ; IRQ #4 
void        IRQ005SEL_IRQHandler   (void){dbg_irq[ 5]++;while(1);} //   ; IRQ #5 
void        IRQ006SEL_IRQHandler   (void){dbg_irq[ 6]++;while(1);} //   ; IRQ #6 
void        IRQ007SEL_IRQHandler   (void){dbg_irq[ 7]++;while(1);} //   ; IRQ #7 
void        IRQ008SEL_IRQHandler   (void){dbg_irq[ 8]++;while(1);} //   ; IRQ #8 
void        IRQ009SEL_IRQHandler   (void){dbg_irq[ 9]++;while(1);} //   ; IRQ #9 
void       IRQ010SEL_IRQHandler    (void){dbg_irq[10]++;while(1);} //  ; IRQ #10 
//void           EXINT0_IRQHandler   (void){dbg_irq[11]++;while(1);} //  ; IRQ #11 
void           EXINT1_IRQHandler   (void){dbg_irq[12]++;while(1);} //  ; IRQ #12 
void           EXINT2_IRQHandler   (void){dbg_irq[13]++;while(1);} //  ; IRQ #13 
void           EXINT3_IRQHandler   (void){dbg_irq[14]++;while(1);} //  ; IRQ #14 
void           EXINT4_IRQHandler   (void){dbg_irq[15]++;while(1);} //  ; IRQ #15 
void           EXINT5_IRQHandler   (void){dbg_irq[16]++;while(1);} //  ; IRQ #16 
void           EXINT6_IRQHandler   (void){dbg_irq[17]++;while(1);} //  ; IRQ #17 
void           EXINT7_IRQHandler   (void){dbg_irq[18]++;while(1);} //  ; IRQ #18 
void           QPRC0_IRQHandler    (void){dbg_irq[19]++;while(1);} //  ; IRQ #19 
void           QPRC1_IRQHandler    (void){dbg_irq[20]++;while(1);} //  ; IRQ #20 
void   MFT0_WFG_DTIF_IRQHandler    (void){dbg_irq[21]++;while(1);} //  ; IRQ #21 
void   MFT1_WFG_DTIF_IRQHandler    (void){dbg_irq[22]++;while(1);} //  ; IRQ #22 
void   MFT2_WFG_DTIF_IRQHandler    (void){dbg_irq[23]++;while(1);} //  ; IRQ #23 
void   MFT0_FRT_PEAK_IRQHandler    (void){dbg_irq[24]++;while(1);} //  ; IRQ #24 
void   MFT0_FRT_ZERO_IRQHandler    (void){dbg_irq[25]++;while(1);} //  ; IRQ #25 
void           MFT0_ICU_IRQHandler (void){dbg_irq[26]++;while(1);} //  ; IRQ #26 
void           MFT0_OCU_IRQHandler (void){dbg_irq[27]++;while(1);} //  ; IRQ #27 
void   MFT1_FRT_PEAK_IRQHandler    (void){dbg_irq[28]++;while(1);} //  ; IRQ #28 
void   MFT1_FRT_ZERO_IRQHandler    (void){dbg_irq[29]++;while(1);} //  ; IRQ #29 
void           MFT1_ICU_IRQHandler (void){dbg_irq[30]++;while(1);} //  ; IRQ #30 
void           MFT1_OCU_IRQHandler (void){dbg_irq[31]++;while(1);} //  ; IRQ #31 
void   MFT2_FRT_PEAK_IRQHandler    (void){dbg_irq[32]++;while(1);} //  ; IRQ #32 
void   MFT2_FRT_ZERO_IRQHandler    (void){dbg_irq[33]++;while(1);} //  ; IRQ #33 
void           MFT2_ICU_IRQHandler (void){dbg_irq[34]++;while(1);} //  ; IRQ #34 
void           MFT2_OCU_IRQHandler (void){dbg_irq[35]++;while(1);} //  ; IRQ #35 
void       PPG00_02_04_IRQHandler  (void){dbg_irq[36]++;while(1);} //  ; IRQ #36 
void       PPG08_10_12_IRQHandler  (void){dbg_irq[37]++;while(1);} //  ; IRQ #37 
void       PPG16_18_20_IRQHandler  (void){dbg_irq[38]++;while(1);} //  ; IRQ #38 
//void               BT0_IRQHandler  (void){dbg_irq[39]++;while(1);} //  ; IRQ #39 
//void               BT1_IRQHandler  (void){dbg_irq[40]++;while(1);} //  ; IRQ #40 
//void               BT2_IRQHandler  (void){dbg_irq[41]++;while(1);} //  ; IRQ #41 
void               BT3_IRQHandler  (void){dbg_irq[42]++;while(1);} //  ; IRQ #42 
void               BT4_IRQHandler  (void){dbg_irq[43]++;while(1);} //  ; IRQ #43 
//void               BT5_IRQHandler  (void){dbg_irq[44]++;while(1);} //  ; IRQ #44 
//void               BT6_IRQHandler  (void){dbg_irq[45]++;while(1);} //  ; IRQ #45 
//void               BT7_IRQHandler  (void){dbg_irq[46]++;while(1);} //  ; IRQ #46 
void               DT_IRQHandler   (void){dbg_irq[47]++;while(1);} //  ; IRQ #47 
void               WC_IRQHandler   (void){dbg_irq[48]++;while(1);} //  ; IRQ #48 
void       EXTBUS_ERR_IRQHandler   (void){dbg_irq[49]++;while(1);} //  ; IRQ #49 
void               RTC_IRQHandler  (void){dbg_irq[50]++;while(1);} //  ; IRQ #50 
void           EXINT8_IRQHandler   (void){dbg_irq[51]++;while(1);} //  ; IRQ #51 
void           EXINT9_IRQHandler   (void){dbg_irq[52]++;while(1);} //  ; IRQ #52 
void           EXINT10_IRQHandler  (void){dbg_irq[53]++;while(1);} //  ; IRQ #53 
void           EXINT11_IRQHandler  (void){dbg_irq[54]++;while(1);} //  ; IRQ #54 
void           EXINT12_IRQHandler  (void){dbg_irq[55]++;while(1);} //  ; IRQ #55 
void           EXINT13_IRQHandler  (void){dbg_irq[56]++;while(1);} //  ; IRQ #56 
void           EXINT14_IRQHandler  (void){dbg_irq[57]++;while(1);} //  ; IRQ #57 
void           EXINT15_IRQHandler  (void){dbg_irq[58]++;while(1);} //  ; IRQ #58 
//void               TIM_IRQHandler  (void){dbg_irq[59]++;while(1);} //  ; IRQ #59 
//void           MFS0_RX_IRQHandler  (void){dbg_irq[60]++;while(1);} //  ; IRQ #60 
//void           MFS0_TX_IRQHandler  (void){dbg_irq[61]++;while(1);} //  ; IRQ #61 
void           MFS1_RX_IRQHandler  (void){dbg_irq[62]++;while(1);} //  ; IRQ #62 
void           MFS1_TX_IRQHandler  (void){dbg_irq[63]++;while(1);} //  ; IRQ #63 
void           MFS2_RX_IRQHandler  (void){dbg_irq[64]++;while(1);} //  ; IRQ #64 
void           MFS2_TX_IRQHandler  (void){dbg_irq[65]++;while(1);} //  ; IRQ #65 
void           MFS3_RX_IRQHandler  (void){dbg_irq[66]++;while(1);} //  ; IRQ #66 
void           MFS3_TX_IRQHandler  (void){dbg_irq[67]++;while(1);} //  ; IRQ #67 
//void           MFS4_RX_IRQHandler  (void){dbg_irq[68]++;while(1);} //  ; IRQ #68 
//void           MFS4_TX_IRQHandler  (void){dbg_irq[69]++;while(1);} //  ; IRQ #69 
//void           MFS5_RX_IRQHandler  (void){dbg_irq[70]++;while(1);} //  ; IRQ #70 
//void           MFS5_TX_IRQHandler  (void){dbg_irq[71]++;while(1);} //  ; IRQ #71 
//void           MFS6_RX_IRQHandler  (void){dbg_irq[72]++;while(1);} //  ; IRQ #72 
//void           MFS6_TX_IRQHandler  (void){dbg_irq[73]++;while(1);} //  ; IRQ #73 
void           MFS7_RX_IRQHandler  (void){dbg_irq[74]++;while(1);} //  ; IRQ #74 
void           MFS7_TX_IRQHandler  (void){dbg_irq[75]++;while(1);} //  ; IRQ #75 
//void               ADC0_IRQHandler (void){dbg_irq[76]++;while(1);} //  ; IRQ #76 
void               ADC1_IRQHandler (void){dbg_irq[77]++;while(1);} //  ; IRQ #77 
void           DMAC0_IRQHandler    (void){dbg_irq[78]++;while(1);} //  ; IRQ #83 
void           DMAC1_IRQHandler    (void){dbg_irq[79]++;while(1);} //  ; IRQ #84 
void           DMAC2_IRQHandler    (void){dbg_irq[80]++;while(1);} //  ; IRQ #85 
void           DMAC3_IRQHandler    (void){dbg_irq[81]++;while(1);} //  ; IRQ #86 
void           DMAC4_IRQHandler    (void){dbg_irq[82]++;while(1);} //  ; IRQ #87 
void           DMAC5_IRQHandler    (void){dbg_irq[83]++;while(1);} //  ; IRQ #88 
void           DMAC6_IRQHandler    (void){dbg_irq[84]++;while(1);} //  ; IRQ #89 
void           DMAC7_IRQHandler    (void){dbg_irq[85]++;while(1);} //  ; IRQ #90 
void               DSTC_IRQHandler (void){dbg_irq[86]++;while(1);} //  ; IRQ #91 
void       EXINT16_19_IRQHandler   (void){dbg_irq[87]++;while(1);} //  ; IRQ #92 
void       EXINT20_23_IRQHandler   (void){dbg_irq[88]++;while(1);} //  ; IRQ #93 
void       EXINT24_27_IRQHandler   (void){dbg_irq[89]++;while(1);} //  ; IRQ #94 
void       EXINT28_31_IRQHandler   (void){dbg_irq[90]++;while(1);} //  ; IRQ #95 
void           QPRC2_IRQHandler    (void){dbg_irq[91]++;while(1);} //  ; IRQ #96 
void              ADC2_IRQHandler  (void){dbg_irq[92]++;while(1);} // ; IRQ #111 
void          DSTC_HW_IRQHandler   (void){dbg_irq[93]++;while(1);} // ; IRQ #112 
void          FLASHIF_IRQHandler   (void){dbg_irq[94]++;while(1);} // ; IRQ #119 
void Dummy                         (void){dbg_irq[95]++;while(1);}



#endif

