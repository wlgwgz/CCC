;*******************************************************************************
; Copyright 2016, Cypress Semiconductor Corporation. All rights reserved.
; You may use this file only in accordance with the license, terms, conditions,
; disclaimers, and limitations in the end user license agreement accompanying
; the software package with which this file was provided.
; CYPRESS PROVIDES THIS SOFTWARE "AS IS" AND MAKES NO WARRANTY
; OF ANY KIND, EXPRESS OR IMPLIED, WITH REGARD TO THIS SOFTWARE,
; INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
; MERCHANTABILITY, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
; PURPOSE.
;*******************************************************************************
; Stack Configuration
;  Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>

Stack_Size      EQU     0x00001000

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; Heap Configuration
;  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>

Heap_Size       EQU     0x00000400

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     MemManage_Handler         ; MPU Fault Handler
                DCD     BusFault_Handler          ; Bus Fault Handler
                DCD     UsageFault_Handler        ; Usage Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     DebugMon_Handler          ; Debug Monitor Handler
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

; Numbered IRQ handler vectors				
				
                DCD     CSV_IRQHandler		; IRQ #0
                DCD     SWDT_IRQHandler		; IRQ #1
                DCD     LVD_IRQHandler		; IRQ #2
                DCD     IRQ003SEL_IRQHandler		; IRQ #3
                DCD     IRQ004SEL_IRQHandler		; IRQ #4
                DCD     IRQ005SEL_IRQHandler		; IRQ #5
                DCD     IRQ006SEL_IRQHandler		; IRQ #6
                DCD     IRQ007SEL_IRQHandler		; IRQ #7
                DCD     IRQ008SEL_IRQHandler		; IRQ #8
                DCD     IRQ009SEL_IRQHandler		; IRQ #9
                DCD     IRQ010SEL_IRQHandler		; IRQ #10
                DCD     EXINT0_IRQHandler		; IRQ #11
                DCD     EXINT1_IRQHandler		; IRQ #12
                DCD     EXINT2_IRQHandler		; IRQ #13
                DCD     EXINT3_IRQHandler		; IRQ #14
                DCD     EXINT4_IRQHandler		; IRQ #15
                DCD     EXINT5_IRQHandler		; IRQ #16
                DCD     EXINT6_IRQHandler		; IRQ #17
                DCD     EXINT7_IRQHandler		; IRQ #18
                DCD     QPRC0_IRQHandler		; IRQ #19
                DCD     QPRC1_IRQHandler		; IRQ #20
                DCD     MFT0_WFG_DTIF_IRQHandler		; IRQ #21
                DCD     MFT1_WFG_DTIF_IRQHandler		; IRQ #22
                DCD     MFT2_WFG_DTIF_IRQHandler		; IRQ #23
                DCD     MFT0_FRT_PEAK_IRQHandler		; IRQ #24
                DCD     MFT0_FRT_ZERO_IRQHandler		; IRQ #25
                DCD     MFT0_ICU_IRQHandler		; IRQ #26
                DCD     MFT0_OCU_IRQHandler		; IRQ #27
                DCD     MFT1_FRT_PEAK_IRQHandler		; IRQ #28
                DCD     MFT1_FRT_ZERO_IRQHandler		; IRQ #29
                DCD     MFT1_ICU_IRQHandler		; IRQ #30
                DCD     MFT1_OCU_IRQHandler		; IRQ #31
                DCD     MFT2_FRT_PEAK_IRQHandler		; IRQ #32
                DCD     MFT2_FRT_ZERO_IRQHandler		; IRQ #33
                DCD     MFT2_ICU_IRQHandler		; IRQ #34
                DCD     MFT2_OCU_IRQHandler		; IRQ #35
                DCD     PPG00_02_04_IRQHandler		; IRQ #36
                DCD     PPG08_10_12_IRQHandler		; IRQ #37
                DCD     PPG16_18_20_IRQHandler		; IRQ #38
                DCD     BT0_IRQHandler		; IRQ #39
                DCD     BT1_IRQHandler		; IRQ #40
                DCD     BT2_IRQHandler		; IRQ #41
                DCD     BT3_IRQHandler		; IRQ #42
                DCD     BT4_IRQHandler		; IRQ #43
                DCD     BT5_IRQHandler		; IRQ #44
                DCD     BT6_IRQHandler		; IRQ #45
                DCD     BT7_IRQHandler		; IRQ #46
                DCD     DT_IRQHandler		; IRQ #47
                DCD     WC_IRQHandler		; IRQ #48
                DCD     EXTBUS_ERR_IRQHandler		; IRQ #49
                DCD     RTC_IRQHandler		; IRQ #50
                DCD     EXINT8_IRQHandler		; IRQ #51
                DCD     EXINT9_IRQHandler		; IRQ #52
                DCD     EXINT10_IRQHandler		; IRQ #53
                DCD     EXINT11_IRQHandler		; IRQ #54
                DCD     EXINT12_IRQHandler		; IRQ #55
                DCD     EXINT13_IRQHandler		; IRQ #56
                DCD     EXINT14_IRQHandler		; IRQ #57
                DCD     EXINT15_IRQHandler		; IRQ #58
                DCD     TIM_IRQHandler		; IRQ #59
                DCD     MFS0_RX_IRQHandler		; IRQ #60
                DCD     MFS0_TX_IRQHandler		; IRQ #61
                DCD     MFS1_RX_IRQHandler		; IRQ #62
                DCD     MFS1_TX_IRQHandler		; IRQ #63
                DCD     MFS2_RX_IRQHandler		; IRQ #64
                DCD     MFS2_TX_IRQHandler		; IRQ #65
                DCD     MFS3_RX_IRQHandler		; IRQ #66
                DCD     MFS3_TX_IRQHandler		; IRQ #67
                DCD     MFS4_RX_IRQHandler		; IRQ #68
                DCD     MFS4_TX_IRQHandler		; IRQ #69
                DCD     MFS5_RX_IRQHandler		; IRQ #70
                DCD     MFS5_TX_IRQHandler		; IRQ #71
                DCD     MFS6_RX_IRQHandler		; IRQ #72
                DCD     MFS6_TX_IRQHandler		; IRQ #73
                DCD     MFS7_RX_IRQHandler		; IRQ #74
                DCD     MFS7_TX_IRQHandler		; IRQ #75
                DCD     ADC0_IRQHandler		; IRQ #76
                DCD     ADC1_IRQHandler		; IRQ #77
                DCD     Dummy		; IRQ #78
                DCD     Dummy		; IRQ #79
                DCD     Dummy		; IRQ #80
                DCD     Dummy		; IRQ #81
                DCD     Dummy		; IRQ #82
                DCD     DMAC0_IRQHandler		; IRQ #83
                DCD     DMAC1_IRQHandler		; IRQ #84
                DCD     DMAC2_IRQHandler		; IRQ #85
                DCD     DMAC3_IRQHandler		; IRQ #86
                DCD     DMAC4_IRQHandler		; IRQ #87
                DCD     DMAC5_IRQHandler		; IRQ #88
                DCD     DMAC6_IRQHandler		; IRQ #89
                DCD     DMAC7_IRQHandler		; IRQ #90
                DCD     DSTC_IRQHandler		; IRQ #91
                DCD     EXINT16_19_IRQHandler		; IRQ #92
                DCD     EXINT20_23_IRQHandler		; IRQ #93
                DCD     EXINT24_27_IRQHandler		; IRQ #94
                DCD     EXINT28_31_IRQHandler		; IRQ #95
                DCD     QPRC2_IRQHandler		; IRQ #96
                DCD     Dummy		; IRQ #97
                DCD     Dummy		; IRQ #98
                DCD     Dummy		; IRQ #99
                DCD     Dummy		; IRQ #100
                DCD     Dummy		; IRQ #101
                DCD     Dummy		; IRQ #102
                DCD     Dummy		; IRQ #103
                DCD     Dummy		; IRQ #104
                DCD     Dummy		; IRQ #105
                DCD     Dummy		; IRQ #106
                DCD     Dummy		; IRQ #107
                DCD     Dummy		; IRQ #108
                DCD     Dummy		; IRQ #109
                DCD     Dummy		; IRQ #110
                DCD     ADC2_IRQHandler		; IRQ #111
                DCD     DSTC_HW_IRQHandler		; IRQ #112
                DCD     Dummy		; IRQ #113
                DCD     Dummy		; IRQ #114
                DCD     Dummy		; IRQ #115
                DCD     Dummy		; IRQ #116
                DCD     Dummy		; IRQ #117
                DCD     Dummy		; IRQ #118
                DCD     FLASHIF_IRQHandler		; IRQ #119


__Vectors_End

__Vectors_Size 	EQU 	__Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main

                LDR.W R0, =0xE000ED88            ; CPACR is located at address 0xE000ED88
                LDR R1, [R0]                     ; Read CPACR
                ORR R1, R1, #(0xF << 20)         ; Set bits 20-23 to enable CP10 and CP11 coprocessors
                STR R1, [R0]                     ; Write back the modified value to the CPACR

                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler         [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler          [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler        [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler          [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT  CSV_IRQHandler    [WEAK]
                EXPORT  SWDT_IRQHandler    [WEAK]
                EXPORT  LVD_IRQHandler    [WEAK]
                EXPORT  IRQ003SEL_IRQHandler    [WEAK]
                EXPORT  IRQ004SEL_IRQHandler    [WEAK]
                EXPORT  IRQ005SEL_IRQHandler    [WEAK]
                EXPORT  IRQ006SEL_IRQHandler    [WEAK]
                EXPORT  IRQ007SEL_IRQHandler    [WEAK]
                EXPORT  IRQ008SEL_IRQHandler    [WEAK]
                EXPORT  IRQ009SEL_IRQHandler    [WEAK]
                EXPORT  IRQ010SEL_IRQHandler    [WEAK]
                EXPORT  EXINT0_IRQHandler    [WEAK]
                EXPORT  EXINT1_IRQHandler    [WEAK]
                EXPORT  EXINT2_IRQHandler    [WEAK]
                EXPORT  EXINT3_IRQHandler    [WEAK]
                EXPORT  EXINT4_IRQHandler    [WEAK]
                EXPORT  EXINT5_IRQHandler    [WEAK]
                EXPORT  EXINT6_IRQHandler    [WEAK]
                EXPORT  EXINT7_IRQHandler    [WEAK]
                EXPORT  QPRC0_IRQHandler    [WEAK]
                EXPORT  QPRC1_IRQHandler    [WEAK]
                EXPORT  MFT0_WFG_DTIF_IRQHandler    [WEAK]
                EXPORT  MFT1_WFG_DTIF_IRQHandler    [WEAK]
                EXPORT  MFT2_WFG_DTIF_IRQHandler    [WEAK]
                EXPORT  MFT0_FRT_PEAK_IRQHandler    [WEAK]
                EXPORT  MFT0_FRT_ZERO_IRQHandler    [WEAK]
                EXPORT  MFT0_ICU_IRQHandler    [WEAK]
                EXPORT  MFT0_OCU_IRQHandler    [WEAK]
                EXPORT  MFT1_FRT_PEAK_IRQHandler    [WEAK]
                EXPORT  MFT1_FRT_ZERO_IRQHandler    [WEAK]
                EXPORT  MFT1_ICU_IRQHandler    [WEAK]
                EXPORT  MFT1_OCU_IRQHandler    [WEAK]
                EXPORT  MFT2_FRT_PEAK_IRQHandler    [WEAK]
                EXPORT  MFT2_FRT_ZERO_IRQHandler    [WEAK]
                EXPORT  MFT2_ICU_IRQHandler    [WEAK]
                EXPORT  MFT2_OCU_IRQHandler    [WEAK]
                EXPORT  PPG00_02_04_IRQHandler    [WEAK]
                EXPORT  PPG08_10_12_IRQHandler    [WEAK]
                EXPORT  PPG16_18_20_IRQHandler    [WEAK]
                EXPORT  BT0_IRQHandler    [WEAK]
                EXPORT  BT1_IRQHandler    [WEAK]
                EXPORT  BT2_IRQHandler    [WEAK]
                EXPORT  BT3_IRQHandler    [WEAK]
                EXPORT  BT4_IRQHandler    [WEAK]
                EXPORT  BT5_IRQHandler    [WEAK]
                EXPORT  BT6_IRQHandler    [WEAK]
                EXPORT  BT7_IRQHandler    [WEAK]
                EXPORT  DT_IRQHandler    [WEAK]
                EXPORT  WC_IRQHandler    [WEAK]
                EXPORT  EXTBUS_ERR_IRQHandler    [WEAK]
                EXPORT  RTC_IRQHandler    [WEAK]
                EXPORT  EXINT8_IRQHandler    [WEAK]
                EXPORT  EXINT9_IRQHandler    [WEAK]
                EXPORT  EXINT10_IRQHandler    [WEAK]
                EXPORT  EXINT11_IRQHandler    [WEAK]
                EXPORT  EXINT12_IRQHandler    [WEAK]
                EXPORT  EXINT13_IRQHandler    [WEAK]
                EXPORT  EXINT14_IRQHandler    [WEAK]
                EXPORT  EXINT15_IRQHandler    [WEAK]
                EXPORT  TIM_IRQHandler    [WEAK]
                EXPORT  MFS0_RX_IRQHandler    [WEAK]
                EXPORT  MFS0_TX_IRQHandler    [WEAK]
                EXPORT  MFS1_RX_IRQHandler    [WEAK]
                EXPORT  MFS1_TX_IRQHandler    [WEAK]
                EXPORT  MFS2_RX_IRQHandler    [WEAK]
                EXPORT  MFS2_TX_IRQHandler    [WEAK]
                EXPORT  MFS3_RX_IRQHandler    [WEAK]
                EXPORT  MFS3_TX_IRQHandler    [WEAK]
                EXPORT  MFS4_RX_IRQHandler    [WEAK]
                EXPORT  MFS4_TX_IRQHandler    [WEAK]
                EXPORT  MFS5_RX_IRQHandler    [WEAK]
                EXPORT  MFS5_TX_IRQHandler    [WEAK]
                EXPORT  MFS6_RX_IRQHandler    [WEAK]
                EXPORT  MFS6_TX_IRQHandler    [WEAK]
                EXPORT  MFS7_RX_IRQHandler    [WEAK]
                EXPORT  MFS7_TX_IRQHandler    [WEAK]
                EXPORT  ADC0_IRQHandler    [WEAK]
                EXPORT  ADC1_IRQHandler    [WEAK]
                EXPORT  DMAC0_IRQHandler    [WEAK]
                EXPORT  DMAC1_IRQHandler    [WEAK]
                EXPORT  DMAC2_IRQHandler    [WEAK]
                EXPORT  DMAC3_IRQHandler    [WEAK]
                EXPORT  DMAC4_IRQHandler    [WEAK]
                EXPORT  DMAC5_IRQHandler    [WEAK]
                EXPORT  DMAC6_IRQHandler    [WEAK]
                EXPORT  DMAC7_IRQHandler    [WEAK]
                EXPORT  DSTC_IRQHandler    [WEAK]
                EXPORT  EXINT16_19_IRQHandler    [WEAK]
                EXPORT  EXINT20_23_IRQHandler    [WEAK]
                EXPORT  EXINT24_27_IRQHandler    [WEAK]
                EXPORT  EXINT28_31_IRQHandler    [WEAK]
                EXPORT  QPRC2_IRQHandler    [WEAK]
                EXPORT  ADC2_IRQHandler    [WEAK]
                EXPORT  DSTC_HW_IRQHandler    [WEAK]
                EXPORT  FLASHIF_IRQHandler    [WEAK]

                
                EXPORT  Dummy	          [WEAK]

CSV_IRQHandler		; IRQ #0
SWDT_IRQHandler		; IRQ #1
LVD_IRQHandler		; IRQ #2
IRQ003SEL_IRQHandler		; IRQ #3
IRQ004SEL_IRQHandler		; IRQ #4
IRQ005SEL_IRQHandler		; IRQ #5
IRQ006SEL_IRQHandler		; IRQ #6
IRQ007SEL_IRQHandler		; IRQ #7
IRQ008SEL_IRQHandler		; IRQ #8
IRQ009SEL_IRQHandler		; IRQ #9
IRQ010SEL_IRQHandler		; IRQ #10
EXINT0_IRQHandler		; IRQ #11
EXINT1_IRQHandler		; IRQ #12
EXINT2_IRQHandler		; IRQ #13
EXINT3_IRQHandler		; IRQ #14
EXINT4_IRQHandler		; IRQ #15
EXINT5_IRQHandler		; IRQ #16
EXINT6_IRQHandler		; IRQ #17
EXINT7_IRQHandler		; IRQ #18
QPRC0_IRQHandler		; IRQ #19
QPRC1_IRQHandler		; IRQ #20
MFT0_WFG_DTIF_IRQHandler		; IRQ #21
MFT1_WFG_DTIF_IRQHandler		; IRQ #22
MFT2_WFG_DTIF_IRQHandler		; IRQ #23
MFT0_FRT_PEAK_IRQHandler		; IRQ #24
MFT0_FRT_ZERO_IRQHandler		; IRQ #25
MFT0_ICU_IRQHandler		; IRQ #26
MFT0_OCU_IRQHandler		; IRQ #27
MFT1_FRT_PEAK_IRQHandler		; IRQ #28
MFT1_FRT_ZERO_IRQHandler		; IRQ #29
MFT1_ICU_IRQHandler		; IRQ #30
MFT1_OCU_IRQHandler		; IRQ #31
MFT2_FRT_PEAK_IRQHandler		; IRQ #32
MFT2_FRT_ZERO_IRQHandler		; IRQ #33
MFT2_ICU_IRQHandler		; IRQ #34
MFT2_OCU_IRQHandler		; IRQ #35
PPG00_02_04_IRQHandler		; IRQ #36
PPG08_10_12_IRQHandler		; IRQ #37
PPG16_18_20_IRQHandler		; IRQ #38
BT0_IRQHandler		; IRQ #39
BT1_IRQHandler		; IRQ #40
BT2_IRQHandler		; IRQ #41
BT3_IRQHandler		; IRQ #42
BT4_IRQHandler		; IRQ #43
BT5_IRQHandler		; IRQ #44
BT6_IRQHandler		; IRQ #45
BT7_IRQHandler		; IRQ #46
DT_IRQHandler		; IRQ #47
WC_IRQHandler		; IRQ #48
EXTBUS_ERR_IRQHandler		; IRQ #49
RTC_IRQHandler		; IRQ #50
EXINT8_IRQHandler		; IRQ #51
EXINT9_IRQHandler		; IRQ #52
EXINT10_IRQHandler		; IRQ #53
EXINT11_IRQHandler		; IRQ #54
EXINT12_IRQHandler		; IRQ #55
EXINT13_IRQHandler		; IRQ #56
EXINT14_IRQHandler		; IRQ #57
EXINT15_IRQHandler		; IRQ #58
TIM_IRQHandler		; IRQ #59
MFS0_RX_IRQHandler		; IRQ #60
MFS0_TX_IRQHandler		; IRQ #61
MFS1_RX_IRQHandler		; IRQ #62
MFS1_TX_IRQHandler		; IRQ #63
MFS2_RX_IRQHandler		; IRQ #64
MFS2_TX_IRQHandler		; IRQ #65
MFS3_RX_IRQHandler		; IRQ #66
MFS3_TX_IRQHandler		; IRQ #67
MFS4_RX_IRQHandler		; IRQ #68
MFS4_TX_IRQHandler		; IRQ #69
MFS5_RX_IRQHandler		; IRQ #70
MFS5_TX_IRQHandler		; IRQ #71
MFS6_RX_IRQHandler		; IRQ #72
MFS6_TX_IRQHandler		; IRQ #73
MFS7_RX_IRQHandler		; IRQ #74
MFS7_TX_IRQHandler		; IRQ #75
ADC0_IRQHandler		; IRQ #76
ADC1_IRQHandler		; IRQ #77
DMAC0_IRQHandler		; IRQ #83
DMAC1_IRQHandler		; IRQ #84
DMAC2_IRQHandler		; IRQ #85
DMAC3_IRQHandler		; IRQ #86
DMAC4_IRQHandler		; IRQ #87
DMAC5_IRQHandler		; IRQ #88
DMAC6_IRQHandler		; IRQ #89
DMAC7_IRQHandler		; IRQ #90
DSTC_IRQHandler		; IRQ #91
EXINT16_19_IRQHandler		; IRQ #92
EXINT20_23_IRQHandler		; IRQ #93
EXINT24_27_IRQHandler		; IRQ #94
EXINT28_31_IRQHandler		; IRQ #95
QPRC2_IRQHandler		; IRQ #96
ADC2_IRQHandler		; IRQ #111
DSTC_HW_IRQHandler		; IRQ #112
FLASHIF_IRQHandler		; IRQ #119

Dummy


                B       .

                ENDP


                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB
                
                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit
                
                ELSE
                
                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, = Heap_Mem
                LDR     R1, = (Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem + Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF


                END
