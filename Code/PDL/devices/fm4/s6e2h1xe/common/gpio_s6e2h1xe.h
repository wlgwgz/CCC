/*************************************************************************************
* Copyright (C) 2013-2015, Cypress Semiconductor Corporation. All rights reserved.    
*                                                                                     
* This software, including source code, documentation and related                     
* materials ( "Software" ), is owned by Cypress Semiconductor                         
* Corporation ( "Cypress" ) and is protected by and subject to worldwide              
* patent protection (United States and foreign), United States copyright              
* laws and international treaty provisions. Therefore, you may use this               
* Software only as provided in the license agreement accompanying the                 
* software package from which you obtained this Software ( "EULA" ).                  
* If no EULA applies, Cypress hereby grants you a personal, nonexclusive,             
* non-transferable license to copy, modify, and compile the                           
* Software source code solely for use in connection with Cypress's                    
* integrated circuit products. Any reproduction, modification, translation,           
* compilation, or representation of this Software except as specified                 
* above is prohibited without the express written permission of Cypress.              
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO                                
* WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING,                                
* BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED                                        
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A                                     
* PARTICULAR PURPOSE. Cypress reserves the right to make                              
* changes to the Software without notice. Cypress does not assume any                 
* liability arising out of the application or use of the Software or any              
* product or circuit described in the Software. Cypress does not                      
* authorize its products for use in any products where a malfunction or               
* failure of the Cypress product may reasonably be expected to result in              
* significant property damage, injury or death ( "High Risk Product" ). By            
* including Cypress's product in a High Risk Product, the manufacturer                
* of such system or application assumes all risk of such use and in doing             
* so agrees to indemnify Cypress against all liability.                               
*/
/************************************************************************************/
/** \file gpio_s6e2h1xe.h
 **
 ** Header file for S6E2H1XE GPIO functions, included in gpio.h
 **
 ** History:
 **   - 2015-12-04  2.0  NOSU	Clean ALL FM Series pin files
 **
 ** Timestamp:
 **   - 2015-12-16 18:30:40  Auto created by GpioHeaderGenerator Rev 1.0.0
 **
 ******************************************************************************/

#ifndef __GPIO_S6E2H1XE_H__
#define __GPIO_S6E2H1XE_H__

#include <stdint.h>

#define PINCONFIG_SET_REG(pinreg,pos,width,value)    \
          ((pinreg) = ((pinreg) & ~(((1u<<(width))-1u)<<(pos)) | \
          ((value) << (pos))))

/******************************************************************************
   GPIO
*******************************************************************************/

/*---- GPIO bit P00 ----*/
#define GPIO1PIN_P00_GET                ( bFM_GPIO_PDIR0_P0 )

#define GPIO1PIN_P00_PUT(v)             ( bFM_GPIO_PDOR0_P0=(v) )

#define GPIO1PIN_P00_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P00_INITIN(v) \
                                                           : GPIO1PIN_P00_INITOUT(v) )

#define GPIO1PIN_P00_INITIN(v)          do{ \
                                            bFM_GPIO_PCR0_P0=(v).bPullup; \
                                            bFM_GPIO_DDR0_P0=0u; \
                                            bFM_GPIO_PFR0_P0=0u; }while(0u)

#define GPIO1PIN_P00_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR0_P0=(v).bInitVal; \
                                            bFM_GPIO_DDR0_P0=1u; \
                                            bFM_GPIO_PFR0_P0=0u; }while(0u)

/*---- GPIO bit NP00 ----*/
#define GPIO1PIN_NP00_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR0_P0)) )

#define GPIO1PIN_NP00_PUT(v)            ( bFM_GPIO_PDOR0_P0=(uint32_t)(!(v)) )

#define GPIO1PIN_NP00_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP00_INITIN(v) \
                                                           : GPIO1PIN_NP00_INITOUT(v) )

#define GPIO1PIN_NP00_INITIN(v)         do{ \
                                            bFM_GPIO_PCR0_P0=(v).bPullup; \
                                            bFM_GPIO_DDR0_P0=0u; \
                                            bFM_GPIO_PFR0_P0=0u; }while(0u)

#define GPIO1PIN_NP00_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR0_P0=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR0_P0=1u; \
                                            bFM_GPIO_PFR0_P0=0u; }while(0u)

/*---- GPIO bit P01 ----*/
#define GPIO1PIN_P01_GET                ( bFM_GPIO_PDIR0_P1 )

#define GPIO1PIN_P01_PUT(v)             ( bFM_GPIO_PDOR0_P1=(v) )

#define GPIO1PIN_P01_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P01_INITIN(v) \
                                                           : GPIO1PIN_P01_INITOUT(v) )

#define GPIO1PIN_P01_INITIN(v)          do{ \
                                            bFM_GPIO_PCR0_P1=(v).bPullup; \
                                            bFM_GPIO_DDR0_P1=0u; \
                                            bFM_GPIO_PFR0_P1=0u; }while(0u)

#define GPIO1PIN_P01_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR0_P1=(v).bInitVal; \
                                            bFM_GPIO_DDR0_P1=1u; \
                                            bFM_GPIO_PFR0_P1=0u; }while(0u)

/*---- GPIO bit NP01 ----*/
#define GPIO1PIN_NP01_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR0_P1)) )

#define GPIO1PIN_NP01_PUT(v)            ( bFM_GPIO_PDOR0_P1=(uint32_t)(!(v)) )

#define GPIO1PIN_NP01_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP01_INITIN(v) \
                                                           : GPIO1PIN_NP01_INITOUT(v) )

#define GPIO1PIN_NP01_INITIN(v)         do{ \
                                            bFM_GPIO_PCR0_P1=(v).bPullup; \
                                            bFM_GPIO_DDR0_P1=0u; \
                                            bFM_GPIO_PFR0_P1=0u; }while(0u)

#define GPIO1PIN_NP01_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR0_P1=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR0_P1=1u; \
                                            bFM_GPIO_PFR0_P1=0u; }while(0u)

/*---- GPIO bit P02 ----*/
#define GPIO1PIN_P02_GET                ( bFM_GPIO_PDIR0_P2 )

#define GPIO1PIN_P02_PUT(v)             ( bFM_GPIO_PDOR0_P2=(v) )

#define GPIO1PIN_P02_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P02_INITIN(v) \
                                                           : GPIO1PIN_P02_INITOUT(v) )

#define GPIO1PIN_P02_INITIN(v)          do{ \
                                            bFM_GPIO_PCR0_P2=(v).bPullup; \
                                            bFM_GPIO_DDR0_P2=0u; \
                                            bFM_GPIO_PFR0_P2=0u; }while(0u)

#define GPIO1PIN_P02_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR0_P2=(v).bInitVal; \
                                            bFM_GPIO_DDR0_P2=1u; \
                                            bFM_GPIO_PFR0_P2=0u; }while(0u)

/*---- GPIO bit NP02 ----*/
#define GPIO1PIN_NP02_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR0_P2)) )

#define GPIO1PIN_NP02_PUT(v)            ( bFM_GPIO_PDOR0_P2=(uint32_t)(!(v)) )

#define GPIO1PIN_NP02_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP02_INITIN(v) \
                                                           : GPIO1PIN_NP02_INITOUT(v) )

#define GPIO1PIN_NP02_INITIN(v)         do{ \
                                            bFM_GPIO_PCR0_P2=(v).bPullup; \
                                            bFM_GPIO_DDR0_P2=0u; \
                                            bFM_GPIO_PFR0_P2=0u; }while(0u)

#define GPIO1PIN_NP02_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR0_P2=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR0_P2=1u; \
                                            bFM_GPIO_PFR0_P2=0u; }while(0u)

/*---- GPIO bit P03 ----*/
#define GPIO1PIN_P03_GET                ( bFM_GPIO_PDIR0_P3 )

#define GPIO1PIN_P03_PUT(v)             ( bFM_GPIO_PDOR0_P3=(v) )

#define GPIO1PIN_P03_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P03_INITIN(v) \
                                                           : GPIO1PIN_P03_INITOUT(v) )

#define GPIO1PIN_P03_INITIN(v)          do{ \
                                            bFM_GPIO_PCR0_P3=(v).bPullup; \
                                            bFM_GPIO_DDR0_P3=0u; \
                                            bFM_GPIO_PFR0_P3=0u; }while(0u)

#define GPIO1PIN_P03_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR0_P3=(v).bInitVal; \
                                            bFM_GPIO_DDR0_P3=1u; \
                                            bFM_GPIO_PFR0_P3=0u; }while(0u)

/*---- GPIO bit NP03 ----*/
#define GPIO1PIN_NP03_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR0_P3)) )

#define GPIO1PIN_NP03_PUT(v)            ( bFM_GPIO_PDOR0_P3=(uint32_t)(!(v)) )

#define GPIO1PIN_NP03_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP03_INITIN(v) \
                                                           : GPIO1PIN_NP03_INITOUT(v) )

#define GPIO1PIN_NP03_INITIN(v)         do{ \
                                            bFM_GPIO_PCR0_P3=(v).bPullup; \
                                            bFM_GPIO_DDR0_P3=0u; \
                                            bFM_GPIO_PFR0_P3=0u; }while(0u)

#define GPIO1PIN_NP03_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR0_P3=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR0_P3=1u; \
                                            bFM_GPIO_PFR0_P3=0u; }while(0u)

/*---- GPIO bit P04 ----*/
#define GPIO1PIN_P04_GET                ( bFM_GPIO_PDIR0_P4 )

#define GPIO1PIN_P04_PUT(v)             ( bFM_GPIO_PDOR0_P4=(v) )

#define GPIO1PIN_P04_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P04_INITIN(v) \
                                                           : GPIO1PIN_P04_INITOUT(v) )

#define GPIO1PIN_P04_INITIN(v)          do{ \
                                            bFM_GPIO_PCR0_P4=(v).bPullup; \
                                            bFM_GPIO_DDR0_P4=0u; \
                                            bFM_GPIO_PFR0_P4=0u; }while(0u)

#define GPIO1PIN_P04_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR0_P4=(v).bInitVal; \
                                            bFM_GPIO_DDR0_P4=1u; \
                                            bFM_GPIO_PFR0_P4=0u; }while(0u)

/*---- GPIO bit NP04 ----*/
#define GPIO1PIN_NP04_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR0_P4)) )

#define GPIO1PIN_NP04_PUT(v)            ( bFM_GPIO_PDOR0_P4=(uint32_t)(!(v)) )

#define GPIO1PIN_NP04_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP04_INITIN(v) \
                                                           : GPIO1PIN_NP04_INITOUT(v) )

#define GPIO1PIN_NP04_INITIN(v)         do{ \
                                            bFM_GPIO_PCR0_P4=(v).bPullup; \
                                            bFM_GPIO_DDR0_P4=0u; \
                                            bFM_GPIO_PFR0_P4=0u; }while(0u)

#define GPIO1PIN_NP04_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR0_P4=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR0_P4=1u; \
                                            bFM_GPIO_PFR0_P4=0u; }while(0u)

/*---- GPIO bit P09 ----*/
#define GPIO1PIN_P09_GET                ( bFM_GPIO_PDIR0_P9 )

#define GPIO1PIN_P09_PUT(v)             ( bFM_GPIO_PDOR0_P9=(v) )

#define GPIO1PIN_P09_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P09_INITIN(v) \
                                                           : GPIO1PIN_P09_INITOUT(v) )

#define GPIO1PIN_P09_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN19=0u; \
                                            bFM_GPIO_PCR0_P9=(v).bPullup; \
                                            bFM_GPIO_DDR0_P9=0u; \
                                            bFM_GPIO_PFR0_P9=0u; }while(0u)

#define GPIO1PIN_P09_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN19=0u; \
                                            bFM_GPIO_PDOR0_P9=(v).bInitVal; \
                                            bFM_GPIO_DDR0_P9=1u; \
                                            bFM_GPIO_PFR0_P9=0u; }while(0u)

/*---- GPIO bit NP09 ----*/
#define GPIO1PIN_NP09_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR0_P9)) )

#define GPIO1PIN_NP09_PUT(v)            ( bFM_GPIO_PDOR0_P9=(uint32_t)(!(v)) )

#define GPIO1PIN_NP09_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP09_INITIN(v) \
                                                           : GPIO1PIN_NP09_INITOUT(v) )

#define GPIO1PIN_NP09_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN19=0u; \
                                            bFM_GPIO_PCR0_P9=(v).bPullup; \
                                            bFM_GPIO_DDR0_P9=0u; \
                                            bFM_GPIO_PFR0_P9=0u; }while(0u)

#define GPIO1PIN_NP09_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN19=0u; \
                                            bFM_GPIO_PDOR0_P9=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR0_P9=1u; \
                                            bFM_GPIO_PFR0_P9=0u; }while(0u)

/*---- GPIO bit P0A ----*/
#define GPIO1PIN_P0A_GET                ( bFM_GPIO_PDIR0_PA )

#define GPIO1PIN_P0A_PUT(v)             ( bFM_GPIO_PDOR0_PA=(v) )

#define GPIO1PIN_P0A_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P0A_INITIN(v) \
                                                           : GPIO1PIN_P0A_INITOUT(v) )

#define GPIO1PIN_P0A_INITIN(v)          do{ \
                                            bFM_GPIO_PCR0_PA=(v).bPullup; \
                                            bFM_GPIO_DDR0_PA=0u; \
                                            bFM_GPIO_PFR0_PA=0u; }while(0u)

#define GPIO1PIN_P0A_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR0_PA=(v).bInitVal; \
                                            bFM_GPIO_DDR0_PA=1u; \
                                            bFM_GPIO_PFR0_PA=0u; }while(0u)

/*---- GPIO bit NP0A ----*/
#define GPIO1PIN_NP0A_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR0_PA)) )

#define GPIO1PIN_NP0A_PUT(v)            ( bFM_GPIO_PDOR0_PA=(uint32_t)(!(v)) )

#define GPIO1PIN_NP0A_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP0A_INITIN(v) \
                                                           : GPIO1PIN_NP0A_INITOUT(v) )

#define GPIO1PIN_NP0A_INITIN(v)         do{ \
                                            bFM_GPIO_PCR0_PA=(v).bPullup; \
                                            bFM_GPIO_DDR0_PA=0u; \
                                            bFM_GPIO_PFR0_PA=0u; }while(0u)

#define GPIO1PIN_NP0A_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR0_PA=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR0_PA=1u; \
                                            bFM_GPIO_PFR0_PA=0u; }while(0u)

/*---- GPIO bit P0B ----*/
#define GPIO1PIN_P0B_GET                ( bFM_GPIO_PDIR0_PB )

#define GPIO1PIN_P0B_PUT(v)             ( bFM_GPIO_PDOR0_PB=(v) )

#define GPIO1PIN_P0B_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P0B_INITIN(v) \
                                                           : GPIO1PIN_P0B_INITOUT(v) )

#define GPIO1PIN_P0B_INITIN(v)          do{ \
                                            bFM_GPIO_PCR0_PB=(v).bPullup; \
                                            bFM_GPIO_DDR0_PB=0u; \
                                            bFM_GPIO_PFR0_PB=0u; }while(0u)

#define GPIO1PIN_P0B_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR0_PB=(v).bInitVal; \
                                            bFM_GPIO_DDR0_PB=1u; \
                                            bFM_GPIO_PFR0_PB=0u; }while(0u)

/*---- GPIO bit NP0B ----*/
#define GPIO1PIN_NP0B_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR0_PB)) )

#define GPIO1PIN_NP0B_PUT(v)            ( bFM_GPIO_PDOR0_PB=(uint32_t)(!(v)) )

#define GPIO1PIN_NP0B_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP0B_INITIN(v) \
                                                           : GPIO1PIN_NP0B_INITOUT(v) )

#define GPIO1PIN_NP0B_INITIN(v)         do{ \
                                            bFM_GPIO_PCR0_PB=(v).bPullup; \
                                            bFM_GPIO_DDR0_PB=0u; \
                                            bFM_GPIO_PFR0_PB=0u; }while(0u)

#define GPIO1PIN_NP0B_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR0_PB=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR0_PB=1u; \
                                            bFM_GPIO_PFR0_PB=0u; }while(0u)

/*---- GPIO bit P0C ----*/
#define GPIO1PIN_P0C_GET                ( bFM_GPIO_PDIR0_PC )

#define GPIO1PIN_P0C_PUT(v)             ( bFM_GPIO_PDOR0_PC=(v) )

#define GPIO1PIN_P0C_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P0C_INITIN(v) \
                                                           : GPIO1PIN_P0C_INITOUT(v) )

#define GPIO1PIN_P0C_INITIN(v)          do{ \
                                            bFM_GPIO_PCR0_PC=(v).bPullup; \
                                            bFM_GPIO_DDR0_PC=0u; \
                                            bFM_GPIO_PFR0_PC=0u; }while(0u)

#define GPIO1PIN_P0C_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR0_PC=(v).bInitVal; \
                                            bFM_GPIO_DDR0_PC=1u; \
                                            bFM_GPIO_PFR0_PC=0u; }while(0u)

/*---- GPIO bit NP0C ----*/
#define GPIO1PIN_NP0C_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR0_PC)) )

#define GPIO1PIN_NP0C_PUT(v)            ( bFM_GPIO_PDOR0_PC=(uint32_t)(!(v)) )

#define GPIO1PIN_NP0C_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP0C_INITIN(v) \
                                                           : GPIO1PIN_NP0C_INITOUT(v) )

#define GPIO1PIN_NP0C_INITIN(v)         do{ \
                                            bFM_GPIO_PCR0_PC=(v).bPullup; \
                                            bFM_GPIO_DDR0_PC=0u; \
                                            bFM_GPIO_PFR0_PC=0u; }while(0u)

#define GPIO1PIN_NP0C_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR0_PC=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR0_PC=1u; \
                                            bFM_GPIO_PFR0_PC=0u; }while(0u)

/*---- GPIO bit P0D ----*/
#define GPIO1PIN_P0D_GET                ( bFM_GPIO_PDIR0_PD )

#define GPIO1PIN_P0D_PUT(v)             ( bFM_GPIO_PDOR0_PD=(v) )

#define GPIO1PIN_P0D_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P0D_INITIN(v) \
                                                           : GPIO1PIN_P0D_INITOUT(v) )

#define GPIO1PIN_P0D_INITIN(v)          do{ \
                                            bFM_GPIO_PCR0_PD=(v).bPullup; \
                                            bFM_GPIO_DDR0_PD=0u; \
                                            bFM_GPIO_PFR0_PD=0u; }while(0u)

#define GPIO1PIN_P0D_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR0_PD=(v).bInitVal; \
                                            bFM_GPIO_DDR0_PD=1u; \
                                            bFM_GPIO_PFR0_PD=0u; }while(0u)

/*---- GPIO bit NP0D ----*/
#define GPIO1PIN_NP0D_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR0_PD)) )

#define GPIO1PIN_NP0D_PUT(v)            ( bFM_GPIO_PDOR0_PD=(uint32_t)(!(v)) )

#define GPIO1PIN_NP0D_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP0D_INITIN(v) \
                                                           : GPIO1PIN_NP0D_INITOUT(v) )

#define GPIO1PIN_NP0D_INITIN(v)         do{ \
                                            bFM_GPIO_PCR0_PD=(v).bPullup; \
                                            bFM_GPIO_DDR0_PD=0u; \
                                            bFM_GPIO_PFR0_PD=0u; }while(0u)

#define GPIO1PIN_NP0D_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR0_PD=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR0_PD=1u; \
                                            bFM_GPIO_PFR0_PD=0u; }while(0u)

/*---- GPIO bit P0E ----*/
#define GPIO1PIN_P0E_GET                ( bFM_GPIO_PDIR0_PE )

#define GPIO1PIN_P0E_PUT(v)             ( bFM_GPIO_PDOR0_PE=(v) )

#define GPIO1PIN_P0E_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P0E_INITIN(v) \
                                                           : GPIO1PIN_P0E_INITOUT(v) )

#define GPIO1PIN_P0E_INITIN(v)          do{ \
                                            bFM_GPIO_PCR0_PE=(v).bPullup; \
                                            bFM_GPIO_DDR0_PE=0u; \
                                            bFM_GPIO_PFR0_PE=0u; }while(0u)

#define GPIO1PIN_P0E_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR0_PE=(v).bInitVal; \
                                            bFM_GPIO_DDR0_PE=1u; \
                                            bFM_GPIO_PFR0_PE=0u; }while(0u)

/*---- GPIO bit NP0E ----*/
#define GPIO1PIN_NP0E_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR0_PE)) )

#define GPIO1PIN_NP0E_PUT(v)            ( bFM_GPIO_PDOR0_PE=(uint32_t)(!(v)) )

#define GPIO1PIN_NP0E_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP0E_INITIN(v) \
                                                           : GPIO1PIN_NP0E_INITOUT(v) )

#define GPIO1PIN_NP0E_INITIN(v)         do{ \
                                            bFM_GPIO_PCR0_PE=(v).bPullup; \
                                            bFM_GPIO_DDR0_PE=0u; \
                                            bFM_GPIO_PFR0_PE=0u; }while(0u)

#define GPIO1PIN_NP0E_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR0_PE=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR0_PE=1u; \
                                            bFM_GPIO_PFR0_PE=0u; }while(0u)

/*---- GPIO bit P10 ----*/
#define GPIO1PIN_P10_GET                ( bFM_GPIO_PDIR1_P0 )

#define GPIO1PIN_P10_PUT(v)             ( bFM_GPIO_PDOR1_P0=(v) )

#define GPIO1PIN_P10_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P10_INITIN(v) \
                                                           : GPIO1PIN_P10_INITOUT(v) )

#define GPIO1PIN_P10_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN00=0u; \
                                            bFM_GPIO_PCR1_P0=(v).bPullup; \
                                            bFM_GPIO_DDR1_P0=0u; \
                                            bFM_GPIO_PFR1_P0=0u; }while(0u)

#define GPIO1PIN_P10_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN00=0u; \
                                            bFM_GPIO_PDOR1_P0=(v).bInitVal; \
                                            bFM_GPIO_DDR1_P0=1u; \
                                            bFM_GPIO_PFR1_P0=0u; }while(0u)

/*---- GPIO bit NP10 ----*/
#define GPIO1PIN_NP10_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR1_P0)) )

#define GPIO1PIN_NP10_PUT(v)            ( bFM_GPIO_PDOR1_P0=(uint32_t)(!(v)) )

#define GPIO1PIN_NP10_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP10_INITIN(v) \
                                                           : GPIO1PIN_NP10_INITOUT(v) )

#define GPIO1PIN_NP10_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN00=0u; \
                                            bFM_GPIO_PCR1_P0=(v).bPullup; \
                                            bFM_GPIO_DDR1_P0=0u; \
                                            bFM_GPIO_PFR1_P0=0u; }while(0u)

#define GPIO1PIN_NP10_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN00=0u; \
                                            bFM_GPIO_PDOR1_P0=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR1_P0=1u; \
                                            bFM_GPIO_PFR1_P0=0u; }while(0u)

/*---- GPIO bit P11 ----*/
#define GPIO1PIN_P11_GET                ( bFM_GPIO_PDIR1_P1 )

#define GPIO1PIN_P11_PUT(v)             ( bFM_GPIO_PDOR1_P1=(v) )

#define GPIO1PIN_P11_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P11_INITIN(v) \
                                                           : GPIO1PIN_P11_INITOUT(v) )

#define GPIO1PIN_P11_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN01=0u; \
                                            bFM_GPIO_PCR1_P1=(v).bPullup; \
                                            bFM_GPIO_DDR1_P1=0u; \
                                            bFM_GPIO_PFR1_P1=0u; }while(0u)

#define GPIO1PIN_P11_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN01=0u; \
                                            bFM_GPIO_PDOR1_P1=(v).bInitVal; \
                                            bFM_GPIO_DDR1_P1=1u; \
                                            bFM_GPIO_PFR1_P1=0u; }while(0u)

/*---- GPIO bit NP11 ----*/
#define GPIO1PIN_NP11_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR1_P1)) )

#define GPIO1PIN_NP11_PUT(v)            ( bFM_GPIO_PDOR1_P1=(uint32_t)(!(v)) )

#define GPIO1PIN_NP11_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP11_INITIN(v) \
                                                           : GPIO1PIN_NP11_INITOUT(v) )

#define GPIO1PIN_NP11_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN01=0u; \
                                            bFM_GPIO_PCR1_P1=(v).bPullup; \
                                            bFM_GPIO_DDR1_P1=0u; \
                                            bFM_GPIO_PFR1_P1=0u; }while(0u)

#define GPIO1PIN_NP11_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN01=0u; \
                                            bFM_GPIO_PDOR1_P1=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR1_P1=1u; \
                                            bFM_GPIO_PFR1_P1=0u; }while(0u)

/*---- GPIO bit P12 ----*/
#define GPIO1PIN_P12_GET                ( bFM_GPIO_PDIR1_P2 )

#define GPIO1PIN_P12_PUT(v)             ( bFM_GPIO_PDOR1_P2=(v) )

#define GPIO1PIN_P12_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P12_INITIN(v) \
                                                           : GPIO1PIN_P12_INITOUT(v) )

#define GPIO1PIN_P12_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN02=0u; \
                                            bFM_GPIO_PCR1_P2=(v).bPullup; \
                                            bFM_GPIO_DDR1_P2=0u; \
                                            bFM_GPIO_PFR1_P2=0u; }while(0u)

#define GPIO1PIN_P12_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN02=0u; \
                                            bFM_GPIO_PDOR1_P2=(v).bInitVal; \
                                            bFM_GPIO_DDR1_P2=1u; \
                                            bFM_GPIO_PFR1_P2=0u; }while(0u)

/*---- GPIO bit NP12 ----*/
#define GPIO1PIN_NP12_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR1_P2)) )

#define GPIO1PIN_NP12_PUT(v)            ( bFM_GPIO_PDOR1_P2=(uint32_t)(!(v)) )

#define GPIO1PIN_NP12_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP12_INITIN(v) \
                                                           : GPIO1PIN_NP12_INITOUT(v) )

#define GPIO1PIN_NP12_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN02=0u; \
                                            bFM_GPIO_PCR1_P2=(v).bPullup; \
                                            bFM_GPIO_DDR1_P2=0u; \
                                            bFM_GPIO_PFR1_P2=0u; }while(0u)

#define GPIO1PIN_NP12_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN02=0u; \
                                            bFM_GPIO_PDOR1_P2=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR1_P2=1u; \
                                            bFM_GPIO_PFR1_P2=0u; }while(0u)

/*---- GPIO bit P13 ----*/
#define GPIO1PIN_P13_GET                ( bFM_GPIO_PDIR1_P3 )

#define GPIO1PIN_P13_PUT(v)             ( bFM_GPIO_PDOR1_P3=(v) )

#define GPIO1PIN_P13_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P13_INITIN(v) \
                                                           : GPIO1PIN_P13_INITOUT(v) )

#define GPIO1PIN_P13_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN03=0u; \
                                            bFM_GPIO_PCR1_P3=(v).bPullup; \
                                            bFM_GPIO_DDR1_P3=0u; \
                                            bFM_GPIO_PFR1_P3=0u; }while(0u)

#define GPIO1PIN_P13_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN03=0u; \
                                            bFM_GPIO_PDOR1_P3=(v).bInitVal; \
                                            bFM_GPIO_DDR1_P3=1u; \
                                            bFM_GPIO_PFR1_P3=0u; }while(0u)

/*---- GPIO bit NP13 ----*/
#define GPIO1PIN_NP13_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR1_P3)) )

#define GPIO1PIN_NP13_PUT(v)            ( bFM_GPIO_PDOR1_P3=(uint32_t)(!(v)) )

#define GPIO1PIN_NP13_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP13_INITIN(v) \
                                                           : GPIO1PIN_NP13_INITOUT(v) )

#define GPIO1PIN_NP13_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN03=0u; \
                                            bFM_GPIO_PCR1_P3=(v).bPullup; \
                                            bFM_GPIO_DDR1_P3=0u; \
                                            bFM_GPIO_PFR1_P3=0u; }while(0u)

#define GPIO1PIN_NP13_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN03=0u; \
                                            bFM_GPIO_PDOR1_P3=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR1_P3=1u; \
                                            bFM_GPIO_PFR1_P3=0u; }while(0u)

/*---- GPIO bit P14 ----*/
#define GPIO1PIN_P14_GET                ( bFM_GPIO_PDIR1_P4 )

#define GPIO1PIN_P14_PUT(v)             ( bFM_GPIO_PDOR1_P4=(v) )

#define GPIO1PIN_P14_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P14_INITIN(v) \
                                                           : GPIO1PIN_P14_INITOUT(v) )

#define GPIO1PIN_P14_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN04=0u; \
                                            bFM_GPIO_PCR1_P4=(v).bPullup; \
                                            bFM_GPIO_DDR1_P4=0u; \
                                            bFM_GPIO_PFR1_P4=0u; }while(0u)

#define GPIO1PIN_P14_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN04=0u; \
                                            bFM_GPIO_PDOR1_P4=(v).bInitVal; \
                                            bFM_GPIO_DDR1_P4=1u; \
                                            bFM_GPIO_PFR1_P4=0u; }while(0u)

/*---- GPIO bit NP14 ----*/
#define GPIO1PIN_NP14_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR1_P4)) )

#define GPIO1PIN_NP14_PUT(v)            ( bFM_GPIO_PDOR1_P4=(uint32_t)(!(v)) )

#define GPIO1PIN_NP14_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP14_INITIN(v) \
                                                           : GPIO1PIN_NP14_INITOUT(v) )

#define GPIO1PIN_NP14_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN04=0u; \
                                            bFM_GPIO_PCR1_P4=(v).bPullup; \
                                            bFM_GPIO_DDR1_P4=0u; \
                                            bFM_GPIO_PFR1_P4=0u; }while(0u)

#define GPIO1PIN_NP14_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN04=0u; \
                                            bFM_GPIO_PDOR1_P4=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR1_P4=1u; \
                                            bFM_GPIO_PFR1_P4=0u; }while(0u)

/*---- GPIO bit P15 ----*/
#define GPIO1PIN_P15_GET                ( bFM_GPIO_PDIR1_P5 )

#define GPIO1PIN_P15_PUT(v)             ( bFM_GPIO_PDOR1_P5=(v) )

#define GPIO1PIN_P15_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P15_INITIN(v) \
                                                           : GPIO1PIN_P15_INITOUT(v) )

#define GPIO1PIN_P15_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN05=0u; \
                                            bFM_GPIO_PCR1_P5=(v).bPullup; \
                                            bFM_GPIO_DDR1_P5=0u; \
                                            bFM_GPIO_PFR1_P5=0u; }while(0u)

#define GPIO1PIN_P15_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN05=0u; \
                                            bFM_GPIO_PDOR1_P5=(v).bInitVal; \
                                            bFM_GPIO_DDR1_P5=1u; \
                                            bFM_GPIO_PFR1_P5=0u; }while(0u)

/*---- GPIO bit NP15 ----*/
#define GPIO1PIN_NP15_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR1_P5)) )

#define GPIO1PIN_NP15_PUT(v)            ( bFM_GPIO_PDOR1_P5=(uint32_t)(!(v)) )

#define GPIO1PIN_NP15_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP15_INITIN(v) \
                                                           : GPIO1PIN_NP15_INITOUT(v) )

#define GPIO1PIN_NP15_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN05=0u; \
                                            bFM_GPIO_PCR1_P5=(v).bPullup; \
                                            bFM_GPIO_DDR1_P5=0u; \
                                            bFM_GPIO_PFR1_P5=0u; }while(0u)

#define GPIO1PIN_NP15_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN05=0u; \
                                            bFM_GPIO_PDOR1_P5=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR1_P5=1u; \
                                            bFM_GPIO_PFR1_P5=0u; }while(0u)

/*---- GPIO bit P16 ----*/
#define GPIO1PIN_P16_GET                ( bFM_GPIO_PDIR1_P6 )

#define GPIO1PIN_P16_PUT(v)             ( bFM_GPIO_PDOR1_P6=(v) )

#define GPIO1PIN_P16_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P16_INITIN(v) \
                                                           : GPIO1PIN_P16_INITOUT(v) )

#define GPIO1PIN_P16_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN06=0u; \
                                            bFM_GPIO_PCR1_P6=(v).bPullup; \
                                            bFM_GPIO_DDR1_P6=0u; \
                                            bFM_GPIO_PFR1_P6=0u; }while(0u)

#define GPIO1PIN_P16_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN06=0u; \
                                            bFM_GPIO_PDOR1_P6=(v).bInitVal; \
                                            bFM_GPIO_DDR1_P6=1u; \
                                            bFM_GPIO_PFR1_P6=0u; }while(0u)

/*---- GPIO bit NP16 ----*/
#define GPIO1PIN_NP16_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR1_P6)) )

#define GPIO1PIN_NP16_PUT(v)            ( bFM_GPIO_PDOR1_P6=(uint32_t)(!(v)) )

#define GPIO1PIN_NP16_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP16_INITIN(v) \
                                                           : GPIO1PIN_NP16_INITOUT(v) )

#define GPIO1PIN_NP16_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN06=0u; \
                                            bFM_GPIO_PCR1_P6=(v).bPullup; \
                                            bFM_GPIO_DDR1_P6=0u; \
                                            bFM_GPIO_PFR1_P6=0u; }while(0u)

#define GPIO1PIN_NP16_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN06=0u; \
                                            bFM_GPIO_PDOR1_P6=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR1_P6=1u; \
                                            bFM_GPIO_PFR1_P6=0u; }while(0u)

/*---- GPIO bit P17 ----*/
#define GPIO1PIN_P17_GET                ( bFM_GPIO_PDIR1_P7 )

#define GPIO1PIN_P17_PUT(v)             ( bFM_GPIO_PDOR1_P7=(v) )

#define GPIO1PIN_P17_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P17_INITIN(v) \
                                                           : GPIO1PIN_P17_INITOUT(v) )

#define GPIO1PIN_P17_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN07=0u; \
                                            bFM_GPIO_PCR1_P7=(v).bPullup; \
                                            bFM_GPIO_DDR1_P7=0u; \
                                            bFM_GPIO_PFR1_P7=0u; }while(0u)

#define GPIO1PIN_P17_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN07=0u; \
                                            bFM_GPIO_PDOR1_P7=(v).bInitVal; \
                                            bFM_GPIO_DDR1_P7=1u; \
                                            bFM_GPIO_PFR1_P7=0u; }while(0u)

/*---- GPIO bit NP17 ----*/
#define GPIO1PIN_NP17_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR1_P7)) )

#define GPIO1PIN_NP17_PUT(v)            ( bFM_GPIO_PDOR1_P7=(uint32_t)(!(v)) )

#define GPIO1PIN_NP17_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP17_INITIN(v) \
                                                           : GPIO1PIN_NP17_INITOUT(v) )

#define GPIO1PIN_NP17_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN07=0u; \
                                            bFM_GPIO_PCR1_P7=(v).bPullup; \
                                            bFM_GPIO_DDR1_P7=0u; \
                                            bFM_GPIO_PFR1_P7=0u; }while(0u)

#define GPIO1PIN_NP17_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN07=0u; \
                                            bFM_GPIO_PDOR1_P7=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR1_P7=1u; \
                                            bFM_GPIO_PFR1_P7=0u; }while(0u)

/*---- GPIO bit P18 ----*/
#define GPIO1PIN_P18_GET                ( bFM_GPIO_PDIR1_P8 )

#define GPIO1PIN_P18_PUT(v)             ( bFM_GPIO_PDOR1_P8=(v) )

#define GPIO1PIN_P18_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P18_INITIN(v) \
                                                           : GPIO1PIN_P18_INITOUT(v) )

#define GPIO1PIN_P18_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN08=0u; \
                                            bFM_GPIO_PCR1_P8=(v).bPullup; \
                                            bFM_GPIO_DDR1_P8=0u; \
                                            bFM_GPIO_PFR1_P8=0u; }while(0u)

#define GPIO1PIN_P18_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN08=0u; \
                                            bFM_GPIO_PDOR1_P8=(v).bInitVal; \
                                            bFM_GPIO_DDR1_P8=1u; \
                                            bFM_GPIO_PFR1_P8=0u; }while(0u)

/*---- GPIO bit NP18 ----*/
#define GPIO1PIN_NP18_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR1_P8)) )

#define GPIO1PIN_NP18_PUT(v)            ( bFM_GPIO_PDOR1_P8=(uint32_t)(!(v)) )

#define GPIO1PIN_NP18_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP18_INITIN(v) \
                                                           : GPIO1PIN_NP18_INITOUT(v) )

#define GPIO1PIN_NP18_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN08=0u; \
                                            bFM_GPIO_PCR1_P8=(v).bPullup; \
                                            bFM_GPIO_DDR1_P8=0u; \
                                            bFM_GPIO_PFR1_P8=0u; }while(0u)

#define GPIO1PIN_NP18_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN08=0u; \
                                            bFM_GPIO_PDOR1_P8=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR1_P8=1u; \
                                            bFM_GPIO_PFR1_P8=0u; }while(0u)

/*---- GPIO bit P19 ----*/
#define GPIO1PIN_P19_GET                ( bFM_GPIO_PDIR1_P9 )

#define GPIO1PIN_P19_PUT(v)             ( bFM_GPIO_PDOR1_P9=(v) )

#define GPIO1PIN_P19_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P19_INITIN(v) \
                                                           : GPIO1PIN_P19_INITOUT(v) )

#define GPIO1PIN_P19_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN09=0u; \
                                            bFM_GPIO_PCR1_P9=(v).bPullup; \
                                            bFM_GPIO_DDR1_P9=0u; \
                                            bFM_GPIO_PFR1_P9=0u; }while(0u)

#define GPIO1PIN_P19_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN09=0u; \
                                            bFM_GPIO_PDOR1_P9=(v).bInitVal; \
                                            bFM_GPIO_DDR1_P9=1u; \
                                            bFM_GPIO_PFR1_P9=0u; }while(0u)

/*---- GPIO bit NP19 ----*/
#define GPIO1PIN_NP19_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR1_P9)) )

#define GPIO1PIN_NP19_PUT(v)            ( bFM_GPIO_PDOR1_P9=(uint32_t)(!(v)) )

#define GPIO1PIN_NP19_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP19_INITIN(v) \
                                                           : GPIO1PIN_NP19_INITOUT(v) )

#define GPIO1PIN_NP19_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN09=0u; \
                                            bFM_GPIO_PCR1_P9=(v).bPullup; \
                                            bFM_GPIO_DDR1_P9=0u; \
                                            bFM_GPIO_PFR1_P9=0u; }while(0u)

#define GPIO1PIN_NP19_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN09=0u; \
                                            bFM_GPIO_PDOR1_P9=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR1_P9=1u; \
                                            bFM_GPIO_PFR1_P9=0u; }while(0u)

/*---- GPIO bit P1A ----*/
#define GPIO1PIN_P1A_GET                ( bFM_GPIO_PDIR1_PA )

#define GPIO1PIN_P1A_PUT(v)             ( bFM_GPIO_PDOR1_PA=(v) )

#define GPIO1PIN_P1A_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P1A_INITIN(v) \
                                                           : GPIO1PIN_P1A_INITOUT(v) )

#define GPIO1PIN_P1A_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN10=0u; \
                                            bFM_GPIO_PCR1_PA=(v).bPullup; \
                                            bFM_GPIO_DDR1_PA=0u; \
                                            bFM_GPIO_PFR1_PA=0u; }while(0u)

#define GPIO1PIN_P1A_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN10=0u; \
                                            bFM_GPIO_PDOR1_PA=(v).bInitVal; \
                                            bFM_GPIO_DDR1_PA=1u; \
                                            bFM_GPIO_PFR1_PA=0u; }while(0u)

/*---- GPIO bit NP1A ----*/
#define GPIO1PIN_NP1A_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR1_PA)) )

#define GPIO1PIN_NP1A_PUT(v)            ( bFM_GPIO_PDOR1_PA=(uint32_t)(!(v)) )

#define GPIO1PIN_NP1A_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP1A_INITIN(v) \
                                                           : GPIO1PIN_NP1A_INITOUT(v) )

#define GPIO1PIN_NP1A_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN10=0u; \
                                            bFM_GPIO_PCR1_PA=(v).bPullup; \
                                            bFM_GPIO_DDR1_PA=0u; \
                                            bFM_GPIO_PFR1_PA=0u; }while(0u)

#define GPIO1PIN_NP1A_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN10=0u; \
                                            bFM_GPIO_PDOR1_PA=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR1_PA=1u; \
                                            bFM_GPIO_PFR1_PA=0u; }while(0u)

/*---- GPIO bit P1B ----*/
#define GPIO1PIN_P1B_GET                ( bFM_GPIO_PDIR1_PB )

#define GPIO1PIN_P1B_PUT(v)             ( bFM_GPIO_PDOR1_PB=(v) )

#define GPIO1PIN_P1B_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P1B_INITIN(v) \
                                                           : GPIO1PIN_P1B_INITOUT(v) )

#define GPIO1PIN_P1B_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN11=0u; \
                                            bFM_GPIO_PCR1_PB=(v).bPullup; \
                                            bFM_GPIO_DDR1_PB=0u; \
                                            bFM_GPIO_PFR1_PB=0u; }while(0u)

#define GPIO1PIN_P1B_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN11=0u; \
                                            bFM_GPIO_PDOR1_PB=(v).bInitVal; \
                                            bFM_GPIO_DDR1_PB=1u; \
                                            bFM_GPIO_PFR1_PB=0u; }while(0u)

/*---- GPIO bit NP1B ----*/
#define GPIO1PIN_NP1B_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR1_PB)) )

#define GPIO1PIN_NP1B_PUT(v)            ( bFM_GPIO_PDOR1_PB=(uint32_t)(!(v)) )

#define GPIO1PIN_NP1B_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP1B_INITIN(v) \
                                                           : GPIO1PIN_NP1B_INITOUT(v) )

#define GPIO1PIN_NP1B_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN11=0u; \
                                            bFM_GPIO_PCR1_PB=(v).bPullup; \
                                            bFM_GPIO_DDR1_PB=0u; \
                                            bFM_GPIO_PFR1_PB=0u; }while(0u)

#define GPIO1PIN_NP1B_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN11=0u; \
                                            bFM_GPIO_PDOR1_PB=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR1_PB=1u; \
                                            bFM_GPIO_PFR1_PB=0u; }while(0u)

/*---- GPIO bit P21 ----*/
#define GPIO1PIN_P21_GET                ( bFM_GPIO_PDIR2_P1 )

#define GPIO1PIN_P21_PUT(v)             ( bFM_GPIO_PDOR2_P1=(v) )

#define GPIO1PIN_P21_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P21_INITIN(v) \
                                                           : GPIO1PIN_P21_INITOUT(v) )

#define GPIO1PIN_P21_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN17=0u; \
                                            bFM_GPIO_PCR2_P1=(v).bPullup; \
                                            bFM_GPIO_DDR2_P1=0u; \
                                            bFM_GPIO_PFR2_P1=0u; }while(0u)

#define GPIO1PIN_P21_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN17=0u; \
                                            bFM_GPIO_PDOR2_P1=(v).bInitVal; \
                                            bFM_GPIO_DDR2_P1=1u; \
                                            bFM_GPIO_PFR2_P1=0u; }while(0u)

/*---- GPIO bit NP21 ----*/
#define GPIO1PIN_NP21_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR2_P1)) )

#define GPIO1PIN_NP21_PUT(v)            ( bFM_GPIO_PDOR2_P1=(uint32_t)(!(v)) )

#define GPIO1PIN_NP21_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP21_INITIN(v) \
                                                           : GPIO1PIN_NP21_INITOUT(v) )

#define GPIO1PIN_NP21_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN17=0u; \
                                            bFM_GPIO_PCR2_P1=(v).bPullup; \
                                            bFM_GPIO_DDR2_P1=0u; \
                                            bFM_GPIO_PFR2_P1=0u; }while(0u)

#define GPIO1PIN_NP21_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN17=0u; \
                                            bFM_GPIO_PDOR2_P1=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR2_P1=1u; \
                                            bFM_GPIO_PFR2_P1=0u; }while(0u)

/*---- GPIO bit P22 ----*/
#define GPIO1PIN_P22_GET                ( bFM_GPIO_PDIR2_P2 )

#define GPIO1PIN_P22_PUT(v)             ( bFM_GPIO_PDOR2_P2=(v) )

#define GPIO1PIN_P22_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P22_INITIN(v) \
                                                           : GPIO1PIN_P22_INITOUT(v) )

#define GPIO1PIN_P22_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN16=0u; \
                                            bFM_GPIO_PCR2_P2=(v).bPullup; \
                                            bFM_GPIO_DDR2_P2=0u; \
                                            bFM_GPIO_PFR2_P2=0u; }while(0u)

#define GPIO1PIN_P22_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN16=0u; \
                                            bFM_GPIO_PDOR2_P2=(v).bInitVal; \
                                            bFM_GPIO_DDR2_P2=1u; \
                                            bFM_GPIO_PFR2_P2=0u; }while(0u)

/*---- GPIO bit NP22 ----*/
#define GPIO1PIN_NP22_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR2_P2)) )

#define GPIO1PIN_NP22_PUT(v)            ( bFM_GPIO_PDOR2_P2=(uint32_t)(!(v)) )

#define GPIO1PIN_NP22_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP22_INITIN(v) \
                                                           : GPIO1PIN_NP22_INITOUT(v) )

#define GPIO1PIN_NP22_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN16=0u; \
                                            bFM_GPIO_PCR2_P2=(v).bPullup; \
                                            bFM_GPIO_DDR2_P2=0u; \
                                            bFM_GPIO_PFR2_P2=0u; }while(0u)

#define GPIO1PIN_NP22_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN16=0u; \
                                            bFM_GPIO_PDOR2_P2=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR2_P2=1u; \
                                            bFM_GPIO_PFR2_P2=0u; }while(0u)

/*---- GPIO bit P23 ----*/
#define GPIO1PIN_P23_GET                ( bFM_GPIO_PDIR2_P3 )

#define GPIO1PIN_P23_PUT(v)             ( bFM_GPIO_PDOR2_P3=(v) )

#define GPIO1PIN_P23_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P23_INITIN(v) \
                                                           : GPIO1PIN_P23_INITOUT(v) )

#define GPIO1PIN_P23_INITIN(v)          do{ \
                                            bFM_GPIO_ADE_AN15=0u; \
                                            bFM_GPIO_PCR2_P3=(v).bPullup; \
                                            bFM_GPIO_DDR2_P3=0u; \
                                            bFM_GPIO_PFR2_P3=0u; }while(0u)

#define GPIO1PIN_P23_INITOUT(v)         do{ \
                                            bFM_GPIO_ADE_AN15=0u; \
                                            bFM_GPIO_PDOR2_P3=(v).bInitVal; \
                                            bFM_GPIO_DDR2_P3=1u; \
                                            bFM_GPIO_PFR2_P3=0u; }while(0u)

/*---- GPIO bit NP23 ----*/
#define GPIO1PIN_NP23_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR2_P3)) )

#define GPIO1PIN_NP23_PUT(v)            ( bFM_GPIO_PDOR2_P3=(uint32_t)(!(v)) )

#define GPIO1PIN_NP23_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP23_INITIN(v) \
                                                           : GPIO1PIN_NP23_INITOUT(v) )

#define GPIO1PIN_NP23_INITIN(v)         do{ \
                                            bFM_GPIO_ADE_AN15=0u; \
                                            bFM_GPIO_PCR2_P3=(v).bPullup; \
                                            bFM_GPIO_DDR2_P3=0u; \
                                            bFM_GPIO_PFR2_P3=0u; }while(0u)

#define GPIO1PIN_NP23_INITOUT(v)        do{ \
                                            bFM_GPIO_ADE_AN15=0u; \
                                            bFM_GPIO_PDOR2_P3=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR2_P3=1u; \
                                            bFM_GPIO_PFR2_P3=0u; }while(0u)

/*---- GPIO bit P30 ----*/
#define GPIO1PIN_P30_GET                ( bFM_GPIO_PDIR3_P0 )

#define GPIO1PIN_P30_PUT(v)             ( bFM_GPIO_PDOR3_P0=(v) )

#define GPIO1PIN_P30_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P30_INITIN(v) \
                                                           : GPIO1PIN_P30_INITOUT(v) )

#define GPIO1PIN_P30_INITIN(v)          do{ \
                                            bFM_GPIO_PCR3_P0=(v).bPullup; \
                                            bFM_GPIO_DDR3_P0=0u; \
                                            bFM_GPIO_PFR3_P0=0u; }while(0u)

#define GPIO1PIN_P30_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR3_P0=(v).bInitVal; \
                                            bFM_GPIO_DDR3_P0=1u; \
                                            bFM_GPIO_PFR3_P0=0u; }while(0u)

/*---- GPIO bit NP30 ----*/
#define GPIO1PIN_NP30_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR3_P0)) )

#define GPIO1PIN_NP30_PUT(v)            ( bFM_GPIO_PDOR3_P0=(uint32_t)(!(v)) )

#define GPIO1PIN_NP30_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP30_INITIN(v) \
                                                           : GPIO1PIN_NP30_INITOUT(v) )

#define GPIO1PIN_NP30_INITIN(v)         do{ \
                                            bFM_GPIO_PCR3_P0=(v).bPullup; \
                                            bFM_GPIO_DDR3_P0=0u; \
                                            bFM_GPIO_PFR3_P0=0u; }while(0u)

#define GPIO1PIN_NP30_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR3_P0=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR3_P0=1u; \
                                            bFM_GPIO_PFR3_P0=0u; }while(0u)

/*---- GPIO bit P31 ----*/
#define GPIO1PIN_P31_GET                ( bFM_GPIO_PDIR3_P1 )

#define GPIO1PIN_P31_PUT(v)             ( bFM_GPIO_PDOR3_P1=(v) )

#define GPIO1PIN_P31_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P31_INITIN(v) \
                                                           : GPIO1PIN_P31_INITOUT(v) )

#define GPIO1PIN_P31_INITIN(v)          do{ \
                                            bFM_GPIO_PCR3_P1=(v).bPullup; \
                                            bFM_GPIO_DDR3_P1=0u; \
                                            bFM_GPIO_PFR3_P1=0u; }while(0u)

#define GPIO1PIN_P31_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR3_P1=(v).bInitVal; \
                                            bFM_GPIO_DDR3_P1=1u; \
                                            bFM_GPIO_PFR3_P1=0u; }while(0u)

/*---- GPIO bit NP31 ----*/
#define GPIO1PIN_NP31_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR3_P1)) )

#define GPIO1PIN_NP31_PUT(v)            ( bFM_GPIO_PDOR3_P1=(uint32_t)(!(v)) )

#define GPIO1PIN_NP31_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP31_INITIN(v) \
                                                           : GPIO1PIN_NP31_INITOUT(v) )

#define GPIO1PIN_NP31_INITIN(v)         do{ \
                                            bFM_GPIO_PCR3_P1=(v).bPullup; \
                                            bFM_GPIO_DDR3_P1=0u; \
                                            bFM_GPIO_PFR3_P1=0u; }while(0u)

#define GPIO1PIN_NP31_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR3_P1=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR3_P1=1u; \
                                            bFM_GPIO_PFR3_P1=0u; }while(0u)

/*---- GPIO bit P32 ----*/
#define GPIO1PIN_P32_GET                ( bFM_GPIO_PDIR3_P2 )

#define GPIO1PIN_P32_PUT(v)             ( bFM_GPIO_PDOR3_P2=(v) )

#define GPIO1PIN_P32_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P32_INITIN(v) \
                                                           : GPIO1PIN_P32_INITOUT(v) )

#define GPIO1PIN_P32_INITIN(v)          do{ \
                                            bFM_GPIO_PCR3_P2=(v).bPullup; \
                                            bFM_GPIO_DDR3_P2=0u; \
                                            bFM_GPIO_PFR3_P2=0u; }while(0u)

#define GPIO1PIN_P32_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR3_P2=(v).bInitVal; \
                                            bFM_GPIO_DDR3_P2=1u; \
                                            bFM_GPIO_PFR3_P2=0u; }while(0u)

/*---- GPIO bit NP32 ----*/
#define GPIO1PIN_NP32_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR3_P2)) )

#define GPIO1PIN_NP32_PUT(v)            ( bFM_GPIO_PDOR3_P2=(uint32_t)(!(v)) )

#define GPIO1PIN_NP32_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP32_INITIN(v) \
                                                           : GPIO1PIN_NP32_INITOUT(v) )

#define GPIO1PIN_NP32_INITIN(v)         do{ \
                                            bFM_GPIO_PCR3_P2=(v).bPullup; \
                                            bFM_GPIO_DDR3_P2=0u; \
                                            bFM_GPIO_PFR3_P2=0u; }while(0u)

#define GPIO1PIN_NP32_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR3_P2=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR3_P2=1u; \
                                            bFM_GPIO_PFR3_P2=0u; }while(0u)

/*---- GPIO bit P33 ----*/
#define GPIO1PIN_P33_GET                ( bFM_GPIO_PDIR3_P3 )

#define GPIO1PIN_P33_PUT(v)             ( bFM_GPIO_PDOR3_P3=(v) )

#define GPIO1PIN_P33_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P33_INITIN(v) \
                                                           : GPIO1PIN_P33_INITOUT(v) )

#define GPIO1PIN_P33_INITIN(v)          do{ \
                                            bFM_GPIO_PCR3_P3=(v).bPullup; \
                                            bFM_GPIO_DDR3_P3=0u; \
                                            bFM_GPIO_PFR3_P3=0u; }while(0u)

#define GPIO1PIN_P33_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR3_P3=(v).bInitVal; \
                                            bFM_GPIO_DDR3_P3=1u; \
                                            bFM_GPIO_PFR3_P3=0u; }while(0u)

/*---- GPIO bit NP33 ----*/
#define GPIO1PIN_NP33_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR3_P3)) )

#define GPIO1PIN_NP33_PUT(v)            ( bFM_GPIO_PDOR3_P3=(uint32_t)(!(v)) )

#define GPIO1PIN_NP33_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP33_INITIN(v) \
                                                           : GPIO1PIN_NP33_INITOUT(v) )

#define GPIO1PIN_NP33_INITIN(v)         do{ \
                                            bFM_GPIO_PCR3_P3=(v).bPullup; \
                                            bFM_GPIO_DDR3_P3=0u; \
                                            bFM_GPIO_PFR3_P3=0u; }while(0u)

#define GPIO1PIN_NP33_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR3_P3=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR3_P3=1u; \
                                            bFM_GPIO_PFR3_P3=0u; }while(0u)

/*---- GPIO bit P39 ----*/
#define GPIO1PIN_P39_GET                ( bFM_GPIO_PDIR3_P9 )

#define GPIO1PIN_P39_PUT(v)             ( bFM_GPIO_PDOR3_P9=(v) )

#define GPIO1PIN_P39_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P39_INITIN(v) \
                                                           : GPIO1PIN_P39_INITOUT(v) )

#define GPIO1PIN_P39_INITIN(v)          do{ \
                                            bFM_GPIO_PCR3_P9=(v).bPullup; \
                                            bFM_GPIO_DDR3_P9=0u; \
                                            bFM_GPIO_PFR3_P9=0u; }while(0u)

#define GPIO1PIN_P39_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR3_P9=(v).bInitVal; \
                                            bFM_GPIO_DDR3_P9=1u; \
                                            bFM_GPIO_PFR3_P9=0u; }while(0u)

/*---- GPIO bit NP39 ----*/
#define GPIO1PIN_NP39_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR3_P9)) )

#define GPIO1PIN_NP39_PUT(v)            ( bFM_GPIO_PDOR3_P9=(uint32_t)(!(v)) )

#define GPIO1PIN_NP39_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP39_INITIN(v) \
                                                           : GPIO1PIN_NP39_INITOUT(v) )

#define GPIO1PIN_NP39_INITIN(v)         do{ \
                                            bFM_GPIO_PCR3_P9=(v).bPullup; \
                                            bFM_GPIO_DDR3_P9=0u; \
                                            bFM_GPIO_PFR3_P9=0u; }while(0u)

#define GPIO1PIN_NP39_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR3_P9=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR3_P9=1u; \
                                            bFM_GPIO_PFR3_P9=0u; }while(0u)

/*---- GPIO bit P3A ----*/
#define GPIO1PIN_P3A_GET                ( bFM_GPIO_PDIR3_PA )

#define GPIO1PIN_P3A_PUT(v)             ( bFM_GPIO_PDOR3_PA=(v) )

#define GPIO1PIN_P3A_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P3A_INITIN(v) \
                                                           : GPIO1PIN_P3A_INITOUT(v) )

#define GPIO1PIN_P3A_INITIN(v)          do{ \
                                            bFM_GPIO_PCR3_PA=(v).bPullup; \
                                            bFM_GPIO_DDR3_PA=0u; \
                                            bFM_GPIO_PFR3_PA=0u; }while(0u)

#define GPIO1PIN_P3A_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR3_PA=(v).bInitVal; \
                                            bFM_GPIO_DDR3_PA=1u; \
                                            bFM_GPIO_PFR3_PA=0u; }while(0u)

/*---- GPIO bit NP3A ----*/
#define GPIO1PIN_NP3A_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR3_PA)) )

#define GPIO1PIN_NP3A_PUT(v)            ( bFM_GPIO_PDOR3_PA=(uint32_t)(!(v)) )

#define GPIO1PIN_NP3A_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP3A_INITIN(v) \
                                                           : GPIO1PIN_NP3A_INITOUT(v) )

#define GPIO1PIN_NP3A_INITIN(v)         do{ \
                                            bFM_GPIO_PCR3_PA=(v).bPullup; \
                                            bFM_GPIO_DDR3_PA=0u; \
                                            bFM_GPIO_PFR3_PA=0u; }while(0u)

#define GPIO1PIN_NP3A_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR3_PA=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR3_PA=1u; \
                                            bFM_GPIO_PFR3_PA=0u; }while(0u)

/*---- GPIO bit P3B ----*/
#define GPIO1PIN_P3B_GET                ( bFM_GPIO_PDIR3_PB )

#define GPIO1PIN_P3B_PUT(v)             ( bFM_GPIO_PDOR3_PB=(v) )

#define GPIO1PIN_P3B_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P3B_INITIN(v) \
                                                           : GPIO1PIN_P3B_INITOUT(v) )

#define GPIO1PIN_P3B_INITIN(v)          do{ \
                                            bFM_GPIO_PCR3_PB=(v).bPullup; \
                                            bFM_GPIO_DDR3_PB=0u; \
                                            bFM_GPIO_PFR3_PB=0u; }while(0u)

#define GPIO1PIN_P3B_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR3_PB=(v).bInitVal; \
                                            bFM_GPIO_DDR3_PB=1u; \
                                            bFM_GPIO_PFR3_PB=0u; }while(0u)

/*---- GPIO bit NP3B ----*/
#define GPIO1PIN_NP3B_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR3_PB)) )

#define GPIO1PIN_NP3B_PUT(v)            ( bFM_GPIO_PDOR3_PB=(uint32_t)(!(v)) )

#define GPIO1PIN_NP3B_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP3B_INITIN(v) \
                                                           : GPIO1PIN_NP3B_INITOUT(v) )

#define GPIO1PIN_NP3B_INITIN(v)         do{ \
                                            bFM_GPIO_PCR3_PB=(v).bPullup; \
                                            bFM_GPIO_DDR3_PB=0u; \
                                            bFM_GPIO_PFR3_PB=0u; }while(0u)

#define GPIO1PIN_NP3B_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR3_PB=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR3_PB=1u; \
                                            bFM_GPIO_PFR3_PB=0u; }while(0u)

/*---- GPIO bit P3C ----*/
#define GPIO1PIN_P3C_GET                ( bFM_GPIO_PDIR3_PC )

#define GPIO1PIN_P3C_PUT(v)             ( bFM_GPIO_PDOR3_PC=(v) )

#define GPIO1PIN_P3C_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P3C_INITIN(v) \
                                                           : GPIO1PIN_P3C_INITOUT(v) )

#define GPIO1PIN_P3C_INITIN(v)          do{ \
                                            bFM_GPIO_PCR3_PC=(v).bPullup; \
                                            bFM_GPIO_DDR3_PC=0u; \
                                            bFM_GPIO_PFR3_PC=0u; }while(0u)

#define GPIO1PIN_P3C_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR3_PC=(v).bInitVal; \
                                            bFM_GPIO_DDR3_PC=1u; \
                                            bFM_GPIO_PFR3_PC=0u; }while(0u)

/*---- GPIO bit NP3C ----*/
#define GPIO1PIN_NP3C_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR3_PC)) )

#define GPIO1PIN_NP3C_PUT(v)            ( bFM_GPIO_PDOR3_PC=(uint32_t)(!(v)) )

#define GPIO1PIN_NP3C_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP3C_INITIN(v) \
                                                           : GPIO1PIN_NP3C_INITOUT(v) )

#define GPIO1PIN_NP3C_INITIN(v)         do{ \
                                            bFM_GPIO_PCR3_PC=(v).bPullup; \
                                            bFM_GPIO_DDR3_PC=0u; \
                                            bFM_GPIO_PFR3_PC=0u; }while(0u)

#define GPIO1PIN_NP3C_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR3_PC=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR3_PC=1u; \
                                            bFM_GPIO_PFR3_PC=0u; }while(0u)

/*---- GPIO bit P3D ----*/
#define GPIO1PIN_P3D_GET                ( bFM_GPIO_PDIR3_PD )

#define GPIO1PIN_P3D_PUT(v)             ( bFM_GPIO_PDOR3_PD=(v) )

#define GPIO1PIN_P3D_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P3D_INITIN(v) \
                                                           : GPIO1PIN_P3D_INITOUT(v) )

#define GPIO1PIN_P3D_INITIN(v)          do{ \
                                            bFM_GPIO_PCR3_PD=(v).bPullup; \
                                            bFM_GPIO_DDR3_PD=0u; \
                                            bFM_GPIO_PFR3_PD=0u; }while(0u)

#define GPIO1PIN_P3D_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR3_PD=(v).bInitVal; \
                                            bFM_GPIO_DDR3_PD=1u; \
                                            bFM_GPIO_PFR3_PD=0u; }while(0u)

/*---- GPIO bit NP3D ----*/
#define GPIO1PIN_NP3D_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR3_PD)) )

#define GPIO1PIN_NP3D_PUT(v)            ( bFM_GPIO_PDOR3_PD=(uint32_t)(!(v)) )

#define GPIO1PIN_NP3D_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP3D_INITIN(v) \
                                                           : GPIO1PIN_NP3D_INITOUT(v) )

#define GPIO1PIN_NP3D_INITIN(v)         do{ \
                                            bFM_GPIO_PCR3_PD=(v).bPullup; \
                                            bFM_GPIO_DDR3_PD=0u; \
                                            bFM_GPIO_PFR3_PD=0u; }while(0u)

#define GPIO1PIN_NP3D_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR3_PD=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR3_PD=1u; \
                                            bFM_GPIO_PFR3_PD=0u; }while(0u)

/*---- GPIO bit P3E ----*/
#define GPIO1PIN_P3E_GET                ( bFM_GPIO_PDIR3_PE )

#define GPIO1PIN_P3E_PUT(v)             ( bFM_GPIO_PDOR3_PE=(v) )

#define GPIO1PIN_P3E_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P3E_INITIN(v) \
                                                           : GPIO1PIN_P3E_INITOUT(v) )

#define GPIO1PIN_P3E_INITIN(v)          do{ \
                                            bFM_GPIO_PCR3_PE=(v).bPullup; \
                                            bFM_GPIO_DDR3_PE=0u; \
                                            bFM_GPIO_PFR3_PE=0u; }while(0u)

#define GPIO1PIN_P3E_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR3_PE=(v).bInitVal; \
                                            bFM_GPIO_DDR3_PE=1u; \
                                            bFM_GPIO_PFR3_PE=0u; }while(0u)

/*---- GPIO bit NP3E ----*/
#define GPIO1PIN_NP3E_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR3_PE)) )

#define GPIO1PIN_NP3E_PUT(v)            ( bFM_GPIO_PDOR3_PE=(uint32_t)(!(v)) )

#define GPIO1PIN_NP3E_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP3E_INITIN(v) \
                                                           : GPIO1PIN_NP3E_INITOUT(v) )

#define GPIO1PIN_NP3E_INITIN(v)         do{ \
                                            bFM_GPIO_PCR3_PE=(v).bPullup; \
                                            bFM_GPIO_DDR3_PE=0u; \
                                            bFM_GPIO_PFR3_PE=0u; }while(0u)

#define GPIO1PIN_NP3E_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR3_PE=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR3_PE=1u; \
                                            bFM_GPIO_PFR3_PE=0u; }while(0u)

/*---- GPIO bit P3F ----*/
#define GPIO1PIN_P3F_GET                ( bFM_GPIO_PDIR3_PF )

#define GPIO1PIN_P3F_PUT(v)             ( bFM_GPIO_PDOR3_PF=(v) )

#define GPIO1PIN_P3F_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P3F_INITIN(v) \
                                                           : GPIO1PIN_P3F_INITOUT(v) )

#define GPIO1PIN_P3F_INITIN(v)          do{ \
                                            bFM_GPIO_PCR3_PF=(v).bPullup; \
                                            bFM_GPIO_DDR3_PF=0u; \
                                            bFM_GPIO_PFR3_PF=0u; }while(0u)

#define GPIO1PIN_P3F_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR3_PF=(v).bInitVal; \
                                            bFM_GPIO_DDR3_PF=1u; \
                                            bFM_GPIO_PFR3_PF=0u; }while(0u)

/*---- GPIO bit NP3F ----*/
#define GPIO1PIN_NP3F_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR3_PF)) )

#define GPIO1PIN_NP3F_PUT(v)            ( bFM_GPIO_PDOR3_PF=(uint32_t)(!(v)) )

#define GPIO1PIN_NP3F_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP3F_INITIN(v) \
                                                           : GPIO1PIN_NP3F_INITOUT(v) )

#define GPIO1PIN_NP3F_INITIN(v)         do{ \
                                            bFM_GPIO_PCR3_PF=(v).bPullup; \
                                            bFM_GPIO_DDR3_PF=0u; \
                                            bFM_GPIO_PFR3_PF=0u; }while(0u)

#define GPIO1PIN_NP3F_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR3_PF=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR3_PF=1u; \
                                            bFM_GPIO_PFR3_PF=0u; }while(0u)

/*---- GPIO bit P44 ----*/
#define GPIO1PIN_P44_GET                ( bFM_GPIO_PDIR4_P4 )

#define GPIO1PIN_P44_PUT(v)             ( bFM_GPIO_PDOR4_P4=(v) )

#define GPIO1PIN_P44_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P44_INITIN(v) \
                                                           : GPIO1PIN_P44_INITOUT(v) )

#define GPIO1PIN_P44_INITIN(v)          do{ \
                                            bFM_DAC0_DACR_DAE=0u; \
                                            bFM_GPIO_PCR4_P4=(v).bPullup; \
                                            bFM_GPIO_DDR4_P4=0u; \
                                            bFM_GPIO_PFR4_P4=0u; }while(0u)

#define GPIO1PIN_P44_INITOUT(v)         do{ \
                                            bFM_DAC0_DACR_DAE=0u; \
                                            bFM_GPIO_PDOR4_P4=(v).bInitVal; \
                                            bFM_GPIO_DDR4_P4=1u; \
                                            bFM_GPIO_PFR4_P4=0u; }while(0u)

/*---- GPIO bit NP44 ----*/
#define GPIO1PIN_NP44_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR4_P4)) )

#define GPIO1PIN_NP44_PUT(v)            ( bFM_GPIO_PDOR4_P4=(uint32_t)(!(v)) )

#define GPIO1PIN_NP44_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP44_INITIN(v) \
                                                           : GPIO1PIN_NP44_INITOUT(v) )

#define GPIO1PIN_NP44_INITIN(v)         do{ \
                                            bFM_DAC0_DACR_DAE=0u; \
                                            bFM_GPIO_PCR4_P4=(v).bPullup; \
                                            bFM_GPIO_DDR4_P4=0u; \
                                            bFM_GPIO_PFR4_P4=0u; }while(0u)

#define GPIO1PIN_NP44_INITOUT(v)        do{ \
                                            bFM_DAC0_DACR_DAE=0u; \
                                            bFM_GPIO_PDOR4_P4=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR4_P4=1u; \
                                            bFM_GPIO_PFR4_P4=0u; }while(0u)

/*---- GPIO bit P45 ----*/
#define GPIO1PIN_P45_GET                ( bFM_GPIO_PDIR4_P5 )

#define GPIO1PIN_P45_PUT(v)             ( bFM_GPIO_PDOR4_P5=(v) )

#define GPIO1PIN_P45_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P45_INITIN(v) \
                                                           : GPIO1PIN_P45_INITOUT(v) )

#define GPIO1PIN_P45_INITIN(v)          do{ \
                                            bFM_DAC1_DACR_DAE=0u; \
                                            bFM_GPIO_PCR4_P5=(v).bPullup; \
                                            bFM_GPIO_DDR4_P5=0u; \
                                            bFM_GPIO_PFR4_P5=0u; }while(0u)

#define GPIO1PIN_P45_INITOUT(v)         do{ \
                                            bFM_DAC1_DACR_DAE=0u; \
                                            bFM_GPIO_PDOR4_P5=(v).bInitVal; \
                                            bFM_GPIO_DDR4_P5=1u; \
                                            bFM_GPIO_PFR4_P5=0u; }while(0u)

/*---- GPIO bit NP45 ----*/
#define GPIO1PIN_NP45_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR4_P5)) )

#define GPIO1PIN_NP45_PUT(v)            ( bFM_GPIO_PDOR4_P5=(uint32_t)(!(v)) )

#define GPIO1PIN_NP45_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP45_INITIN(v) \
                                                           : GPIO1PIN_NP45_INITOUT(v) )

#define GPIO1PIN_NP45_INITIN(v)         do{ \
                                            bFM_DAC1_DACR_DAE=0u; \
                                            bFM_GPIO_PCR4_P5=(v).bPullup; \
                                            bFM_GPIO_DDR4_P5=0u; \
                                            bFM_GPIO_PFR4_P5=0u; }while(0u)

#define GPIO1PIN_NP45_INITOUT(v)        do{ \
                                            bFM_DAC1_DACR_DAE=0u; \
                                            bFM_GPIO_PDOR4_P5=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR4_P5=1u; \
                                            bFM_GPIO_PFR4_P5=0u; }while(0u)

/*---- GPIO bit P46 ----*/
#define GPIO1PIN_P46_GET                ( bFM_RTC_VBDIR_VDIR3 )

#define GPIO1PIN_P46_PUT(v)             do{ FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBDOR_VDOR3=(v); \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        } while(0u)

#define GPIO1PIN_P46_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P46_INITIN(v) \
                                                           : GPIO1PIN_P46_INITOUT(v) )

#define GPIO1PIN_P46_INITIN(v)          do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBPCR_VPCR3=(v).bPullup; \
                                            bFM_RTC_VBDDR_VDDR3=0u; \
                                            PINCONFIG_SET_REG(FM_RTC->VBPFR, 4u, 2u, 0x00u); \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while(0u)

#define GPIO1PIN_P46_INITOUT(v)      do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBDOR_VDOR3=(v).bInitVal; \
                                            bFM_RTC_VBDDR_VDDR3=1u; \
                                            PINCONFIG_SET_REG(FM_RTC->VBPFR, 4u, 2u, 0x00u); \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while(0u)

/*---- GPIO bit NP46 ----*/
#define GPIO1PIN_NP46_GET               ( (uint32_t)(!(uint32_t)(bFM_RTC_VBDIR_VDIR3)) )

#define GPIO1PIN_NP46_PUT(v)            do{ FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBDOR_VDOR3=(uint32_t)(!(v)); \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        } while(0u)

#define GPIO1PIN_NP46_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP46_INITIN(v) \
                                                           : GPIO1PIN_NP46_INITOUT(v) )

#define GPIO1PIN_NP46_INITIN(v)         do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBPCR_VPCR3=(v).bPullup; \
                                            bFM_RTC_VBDDR_VDDR3=0u; \
                                            PINCONFIG_SET_REG(FM_RTC->VBPFR, 4u, 2u, 0x00u); \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while(0u)

#define GPIO1PIN_NP46_INITOUT(v)        do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBDOR_VDOR3=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_RTC_VBDDR_VDDR3=1u; \
                                            PINCONFIG_SET_REG(FM_RTC->VBPFR, 4u, 2u, 0x00u); \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while(0u)

/*---- GPIO bit P47 ----*/
#define GPIO1PIN_P47_GET                ( bFM_RTC_VBDIR_VDIR2 )

#define GPIO1PIN_P47_PUT(v)             do{ FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBDOR_VDOR2=(v); \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        } while(0u)

#define GPIO1PIN_P47_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P47_INITIN(v) \
                                                           : GPIO1PIN_P47_INITOUT(v) )

#define GPIO1PIN_P47_INITIN(v)          do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBPCR_VPCR2=(v).bPullup; \
                                            bFM_RTC_VBDDR_VDDR2=0u; \
                                            PINCONFIG_SET_REG(FM_RTC->VBPFR, 4u, 2u, 0x00u); \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while(0u)

#define GPIO1PIN_P47_INITOUT(v)      do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBDOR_VDOR2=(v).bInitVal; \
                                            bFM_RTC_VBDDR_VDDR2=1u; \
                                            PINCONFIG_SET_REG(FM_RTC->VBPFR, 4u, 2u, 0x00u); \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while(0u)

/*---- GPIO bit NP47 ----*/
#define GPIO1PIN_NP47_GET               ( (uint32_t)(!(uint32_t)(bFM_RTC_VBDIR_VDIR2)) )

#define GPIO1PIN_NP47_PUT(v)            do{ FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBDOR_VDOR2=(uint32_t)(!(v)); \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        } while(0u)

#define GPIO1PIN_NP47_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP47_INITIN(v) \
                                                           : GPIO1PIN_NP47_INITOUT(v) )

#define GPIO1PIN_NP47_INITIN(v)         do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBPCR_VPCR2=(v).bPullup; \
                                            bFM_RTC_VBDDR_VDDR2=0u; \
                                            PINCONFIG_SET_REG(FM_RTC->VBPFR, 4u, 2u, 0x00u); \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while(0u)

#define GPIO1PIN_NP47_INITOUT(v)        do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBDOR_VDOR2=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_RTC_VBDDR_VDDR2=1u; \
                                            PINCONFIG_SET_REG(FM_RTC->VBPFR, 4u, 2u, 0x00u); \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while(0u)

/*---- GPIO bit P48 ----*/
#define GPIO1PIN_P48_GET                ( bFM_RTC_VBDIR_VDIR0 )

#define GPIO1PIN_P48_PUT(v)             do{ FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBDOR_VDOR0=(v); \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        } while(0u)

#define GPIO1PIN_P48_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P48_INITIN(v) \
                                                           : GPIO1PIN_P48_INITOUT(v) )

#define GPIO1PIN_P48_INITIN(v)          do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBPCR_VPCR0=(v).bPullup; \
                                            bFM_RTC_VBDDR_VDDR0=0u; \
                                            bFM_RTC_VBPFR_VPFR0=0u; \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while(0u)

#define GPIO1PIN_P48_INITOUT(v)      do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBDOR_VDOR0=(v).bInitVal; \
                                            bFM_RTC_VBDDR_VDDR0=1u; \
                                            bFM_RTC_VBPFR_VPFR0=0u; \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while(0u)

/*---- GPIO bit NP48 ----*/
#define GPIO1PIN_NP48_GET               ( (uint32_t)(!(uint32_t)(bFM_RTC_VBDIR_VDIR0)) )

#define GPIO1PIN_NP48_PUT(v)            do{ FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBDOR_VDOR0=(uint32_t)(!(v)); \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        } while(0u)

#define GPIO1PIN_NP48_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP48_INITIN(v) \
                                                           : GPIO1PIN_NP48_INITOUT(v) )

#define GPIO1PIN_NP48_INITIN(v)         do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBPCR_VPCR0=(v).bPullup; \
                                            bFM_RTC_VBDDR_VDDR0=0u; \
                                            bFM_RTC_VBPFR_VPFR0=0u; \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while(0u)

#define GPIO1PIN_NP48_INITOUT(v)        do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBDOR_VDOR0=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_RTC_VBDDR_VDDR0=1u; \
                                            bFM_RTC_VBPFR_VPFR0=0u; \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while(0u)

/*---- GPIO bit P49 ----*/
#define GPIO1PIN_P49_GET                ( bFM_RTC_VBDIR_VDIR1 )

#define GPIO1PIN_P49_PUT(v)             do{ FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBDOR_VDOR1=(v); \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        } while(0u)

#define GPIO1PIN_P49_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P49_INITIN(v) \
                                                           : GPIO1PIN_P49_INITOUT(v) )

#define GPIO1PIN_P49_INITIN(v)          do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBPCR_VPCR1=(v).bPullup; \
                                            bFM_RTC_VBDDR_VDDR1=0u; \
                                            bFM_RTC_VBPFR_VPFR1=0u; \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while(0u)

#define GPIO1PIN_P49_INITOUT(v)      do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBDOR_VDOR1=(v).bInitVal; \
                                            bFM_RTC_VBDDR_VDDR1=1u; \
                                            bFM_RTC_VBPFR_VPFR1=0u; \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while(0u)

/*---- GPIO bit NP49 ----*/
#define GPIO1PIN_NP49_GET               ( (uint32_t)(!(uint32_t)(bFM_RTC_VBDIR_VDIR1)) )

#define GPIO1PIN_NP49_PUT(v)            do{ FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBDOR_VDOR1=(uint32_t)(!(v)); \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        } while(0u)

#define GPIO1PIN_NP49_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP49_INITIN(v) \
                                                           : GPIO1PIN_NP49_INITOUT(v) )

#define GPIO1PIN_NP49_INITIN(v)         do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBPCR_VPCR1=(v).bPullup; \
                                            bFM_RTC_VBDDR_VDDR1=0u; \
                                            bFM_RTC_VBPFR_VPFR1=0u; \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while(0u)

#define GPIO1PIN_NP49_INITOUT(v)        do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBDOR_VDOR1=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_RTC_VBDDR_VDDR1=1u; \
                                            bFM_RTC_VBPFR_VPFR1=0u; \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while(0u)

/*---- GPIO bit P4B ----*/
#define GPIO1PIN_P4B_GET                ( bFM_GPIO_PDIR4_PB )

#define GPIO1PIN_P4B_PUT(v)             ( bFM_GPIO_PDOR4_PB=(v) )

#define GPIO1PIN_P4B_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P4B_INITIN(v) \
                                                           : GPIO1PIN_P4B_INITOUT(v) )

#define GPIO1PIN_P4B_INITIN(v)          do{ \
                                            bFM_GPIO_PCR4_PB=(v).bPullup; \
                                            bFM_GPIO_DDR4_PB=0u; \
                                            bFM_GPIO_PFR4_PB=0u; }while(0u)

#define GPIO1PIN_P4B_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR4_PB=(v).bInitVal; \
                                            bFM_GPIO_DDR4_PB=1u; \
                                            bFM_GPIO_PFR4_PB=0u; }while(0u)

/*---- GPIO bit NP4B ----*/
#define GPIO1PIN_NP4B_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR4_PB)) )

#define GPIO1PIN_NP4B_PUT(v)            ( bFM_GPIO_PDOR4_PB=(uint32_t)(!(v)) )

#define GPIO1PIN_NP4B_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP4B_INITIN(v) \
                                                           : GPIO1PIN_NP4B_INITOUT(v) )

#define GPIO1PIN_NP4B_INITIN(v)         do{ \
                                            bFM_GPIO_PCR4_PB=(v).bPullup; \
                                            bFM_GPIO_DDR4_PB=0u; \
                                            bFM_GPIO_PFR4_PB=0u; }while(0u)

#define GPIO1PIN_NP4B_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR4_PB=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR4_PB=1u; \
                                            bFM_GPIO_PFR4_PB=0u; }while(0u)

/*---- GPIO bit P4C ----*/
#define GPIO1PIN_P4C_GET                ( bFM_GPIO_PDIR4_PC )

#define GPIO1PIN_P4C_PUT(v)             ( bFM_GPIO_PDOR4_PC=(v) )

#define GPIO1PIN_P4C_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P4C_INITIN(v) \
                                                           : GPIO1PIN_P4C_INITOUT(v) )

#define GPIO1PIN_P4C_INITIN(v)          do{ \
                                            bFM_GPIO_PCR4_PC=(v).bPullup; \
                                            bFM_GPIO_DDR4_PC=0u; \
                                            bFM_GPIO_PFR4_PC=0u; }while(0u)

#define GPIO1PIN_P4C_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR4_PC=(v).bInitVal; \
                                            bFM_GPIO_DDR4_PC=1u; \
                                            bFM_GPIO_PFR4_PC=0u; }while(0u)

/*---- GPIO bit NP4C ----*/
#define GPIO1PIN_NP4C_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR4_PC)) )

#define GPIO1PIN_NP4C_PUT(v)            ( bFM_GPIO_PDOR4_PC=(uint32_t)(!(v)) )

#define GPIO1PIN_NP4C_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP4C_INITIN(v) \
                                                           : GPIO1PIN_NP4C_INITOUT(v) )

#define GPIO1PIN_NP4C_INITIN(v)         do{ \
                                            bFM_GPIO_PCR4_PC=(v).bPullup; \
                                            bFM_GPIO_DDR4_PC=0u; \
                                            bFM_GPIO_PFR4_PC=0u; }while(0u)

#define GPIO1PIN_NP4C_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR4_PC=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR4_PC=1u; \
                                            bFM_GPIO_PFR4_PC=0u; }while(0u)

/*---- GPIO bit P4D ----*/
#define GPIO1PIN_P4D_GET                ( bFM_GPIO_PDIR4_PD )

#define GPIO1PIN_P4D_PUT(v)             ( bFM_GPIO_PDOR4_PD=(v) )

#define GPIO1PIN_P4D_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P4D_INITIN(v) \
                                                           : GPIO1PIN_P4D_INITOUT(v) )

#define GPIO1PIN_P4D_INITIN(v)          do{ \
                                            bFM_GPIO_PCR4_PD=(v).bPullup; \
                                            bFM_GPIO_DDR4_PD=0u; \
                                            bFM_GPIO_PFR4_PD=0u; }while(0u)

#define GPIO1PIN_P4D_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR4_PD=(v).bInitVal; \
                                            bFM_GPIO_DDR4_PD=1u; \
                                            bFM_GPIO_PFR4_PD=0u; }while(0u)

/*---- GPIO bit NP4D ----*/
#define GPIO1PIN_NP4D_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR4_PD)) )

#define GPIO1PIN_NP4D_PUT(v)            ( bFM_GPIO_PDOR4_PD=(uint32_t)(!(v)) )

#define GPIO1PIN_NP4D_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP4D_INITIN(v) \
                                                           : GPIO1PIN_NP4D_INITOUT(v) )

#define GPIO1PIN_NP4D_INITIN(v)         do{ \
                                            bFM_GPIO_PCR4_PD=(v).bPullup; \
                                            bFM_GPIO_DDR4_PD=0u; \
                                            bFM_GPIO_PFR4_PD=0u; }while(0u)

#define GPIO1PIN_NP4D_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR4_PD=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR4_PD=1u; \
                                            bFM_GPIO_PFR4_PD=0u; }while(0u)

/*---- GPIO bit P4E ----*/
#define GPIO1PIN_P4E_GET                ( bFM_GPIO_PDIR4_PE )

#define GPIO1PIN_P4E_PUT(v)             ( bFM_GPIO_PDOR4_PE=(v) )

#define GPIO1PIN_P4E_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P4E_INITIN(v) \
                                                           : GPIO1PIN_P4E_INITOUT(v) )

#define GPIO1PIN_P4E_INITIN(v)          do{ \
                                            bFM_GPIO_PCR4_PE=(v).bPullup; \
                                            bFM_GPIO_DDR4_PE=0u; \
                                            bFM_GPIO_PFR4_PE=0u; }while(0u)

#define GPIO1PIN_P4E_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR4_PE=(v).bInitVal; \
                                            bFM_GPIO_DDR4_PE=1u; \
                                            bFM_GPIO_PFR4_PE=0u; }while(0u)

/*---- GPIO bit NP4E ----*/
#define GPIO1PIN_NP4E_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR4_PE)) )

#define GPIO1PIN_NP4E_PUT(v)            ( bFM_GPIO_PDOR4_PE=(uint32_t)(!(v)) )

#define GPIO1PIN_NP4E_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP4E_INITIN(v) \
                                                           : GPIO1PIN_NP4E_INITOUT(v) )

#define GPIO1PIN_NP4E_INITIN(v)         do{ \
                                            bFM_GPIO_PCR4_PE=(v).bPullup; \
                                            bFM_GPIO_DDR4_PE=0u; \
                                            bFM_GPIO_PFR4_PE=0u; }while(0u)

#define GPIO1PIN_NP4E_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR4_PE=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR4_PE=1u; \
                                            bFM_GPIO_PFR4_PE=0u; }while(0u)

/*---- GPIO bit P50 ----*/
#define GPIO1PIN_P50_GET                ( bFM_GPIO_PDIR5_P0 )

#define GPIO1PIN_P50_PUT(v)             ( bFM_GPIO_PDOR5_P0=(v) )

#define GPIO1PIN_P50_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P50_INITIN(v) \
                                                           : GPIO1PIN_P50_INITOUT(v) )

#define GPIO1PIN_P50_INITIN(v)          do{ \
                                            bFM_GPIO_PCR5_P0=(v).bPullup; \
                                            bFM_GPIO_DDR5_P0=0u; \
                                            bFM_GPIO_PFR5_P0=0u; }while(0u)

#define GPIO1PIN_P50_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR5_P0=(v).bInitVal; \
                                            bFM_GPIO_DDR5_P0=1u; \
                                            bFM_GPIO_PFR5_P0=0u; }while(0u)

/*---- GPIO bit NP50 ----*/
#define GPIO1PIN_NP50_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR5_P0)) )

#define GPIO1PIN_NP50_PUT(v)            ( bFM_GPIO_PDOR5_P0=(uint32_t)(!(v)) )

#define GPIO1PIN_NP50_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP50_INITIN(v) \
                                                           : GPIO1PIN_NP50_INITOUT(v) )

#define GPIO1PIN_NP50_INITIN(v)         do{ \
                                            bFM_GPIO_PCR5_P0=(v).bPullup; \
                                            bFM_GPIO_DDR5_P0=0u; \
                                            bFM_GPIO_PFR5_P0=0u; }while(0u)

#define GPIO1PIN_NP50_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR5_P0=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR5_P0=1u; \
                                            bFM_GPIO_PFR5_P0=0u; }while(0u)

/*---- GPIO bit P51 ----*/
#define GPIO1PIN_P51_GET                ( bFM_GPIO_PDIR5_P1 )

#define GPIO1PIN_P51_PUT(v)             ( bFM_GPIO_PDOR5_P1=(v) )

#define GPIO1PIN_P51_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P51_INITIN(v) \
                                                           : GPIO1PIN_P51_INITOUT(v) )

#define GPIO1PIN_P51_INITIN(v)          do{ \
                                            bFM_GPIO_PCR5_P1=(v).bPullup; \
                                            bFM_GPIO_DDR5_P1=0u; \
                                            bFM_GPIO_PFR5_P1=0u; }while(0u)

#define GPIO1PIN_P51_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR5_P1=(v).bInitVal; \
                                            bFM_GPIO_DDR5_P1=1u; \
                                            bFM_GPIO_PFR5_P1=0u; }while(0u)

/*---- GPIO bit NP51 ----*/
#define GPIO1PIN_NP51_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR5_P1)) )

#define GPIO1PIN_NP51_PUT(v)            ( bFM_GPIO_PDOR5_P1=(uint32_t)(!(v)) )

#define GPIO1PIN_NP51_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP51_INITIN(v) \
                                                           : GPIO1PIN_NP51_INITOUT(v) )

#define GPIO1PIN_NP51_INITIN(v)         do{ \
                                            bFM_GPIO_PCR5_P1=(v).bPullup; \
                                            bFM_GPIO_DDR5_P1=0u; \
                                            bFM_GPIO_PFR5_P1=0u; }while(0u)

#define GPIO1PIN_NP51_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR5_P1=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR5_P1=1u; \
                                            bFM_GPIO_PFR5_P1=0u; }while(0u)

/*---- GPIO bit P52 ----*/
#define GPIO1PIN_P52_GET                ( bFM_GPIO_PDIR5_P2 )

#define GPIO1PIN_P52_PUT(v)             ( bFM_GPIO_PDOR5_P2=(v) )

#define GPIO1PIN_P52_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P52_INITIN(v) \
                                                           : GPIO1PIN_P52_INITOUT(v) )

#define GPIO1PIN_P52_INITIN(v)          do{ \
                                            bFM_GPIO_PCR5_P2=(v).bPullup; \
                                            bFM_GPIO_DDR5_P2=0u; \
                                            bFM_GPIO_PFR5_P2=0u; }while(0u)

#define GPIO1PIN_P52_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR5_P2=(v).bInitVal; \
                                            bFM_GPIO_DDR5_P2=1u; \
                                            bFM_GPIO_PFR5_P2=0u; }while(0u)

/*---- GPIO bit NP52 ----*/
#define GPIO1PIN_NP52_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR5_P2)) )

#define GPIO1PIN_NP52_PUT(v)            ( bFM_GPIO_PDOR5_P2=(uint32_t)(!(v)) )

#define GPIO1PIN_NP52_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP52_INITIN(v) \
                                                           : GPIO1PIN_NP52_INITOUT(v) )

#define GPIO1PIN_NP52_INITIN(v)         do{ \
                                            bFM_GPIO_PCR5_P2=(v).bPullup; \
                                            bFM_GPIO_DDR5_P2=0u; \
                                            bFM_GPIO_PFR5_P2=0u; }while(0u)

#define GPIO1PIN_NP52_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR5_P2=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR5_P2=1u; \
                                            bFM_GPIO_PFR5_P2=0u; }while(0u)

/*---- GPIO bit P53 ----*/
#define GPIO1PIN_P53_GET                ( bFM_GPIO_PDIR5_P3 )

#define GPIO1PIN_P53_PUT(v)             ( bFM_GPIO_PDOR5_P3=(v) )

#define GPIO1PIN_P53_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P53_INITIN(v) \
                                                           : GPIO1PIN_P53_INITOUT(v) )

#define GPIO1PIN_P53_INITIN(v)          do{ \
                                            bFM_GPIO_PCR5_P3=(v).bPullup; \
                                            bFM_GPIO_DDR5_P3=0u; \
                                            bFM_GPIO_PFR5_P3=0u; }while(0u)

#define GPIO1PIN_P53_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR5_P3=(v).bInitVal; \
                                            bFM_GPIO_DDR5_P3=1u; \
                                            bFM_GPIO_PFR5_P3=0u; }while(0u)

/*---- GPIO bit NP53 ----*/
#define GPIO1PIN_NP53_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR5_P3)) )

#define GPIO1PIN_NP53_PUT(v)            ( bFM_GPIO_PDOR5_P3=(uint32_t)(!(v)) )

#define GPIO1PIN_NP53_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP53_INITIN(v) \
                                                           : GPIO1PIN_NP53_INITOUT(v) )

#define GPIO1PIN_NP53_INITIN(v)         do{ \
                                            bFM_GPIO_PCR5_P3=(v).bPullup; \
                                            bFM_GPIO_DDR5_P3=0u; \
                                            bFM_GPIO_PFR5_P3=0u; }while(0u)

#define GPIO1PIN_NP53_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR5_P3=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR5_P3=1u; \
                                            bFM_GPIO_PFR5_P3=0u; }while(0u)

/*---- GPIO bit P54 ----*/
#define GPIO1PIN_P54_GET                ( bFM_GPIO_PDIR5_P4 )

#define GPIO1PIN_P54_PUT(v)             ( bFM_GPIO_PDOR5_P4=(v) )

#define GPIO1PIN_P54_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P54_INITIN(v) \
                                                           : GPIO1PIN_P54_INITOUT(v) )

#define GPIO1PIN_P54_INITIN(v)          do{ \
                                            bFM_GPIO_PCR5_P4=(v).bPullup; \
                                            bFM_GPIO_DDR5_P4=0u; \
                                            bFM_GPIO_PFR5_P4=0u; }while(0u)

#define GPIO1PIN_P54_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR5_P4=(v).bInitVal; \
                                            bFM_GPIO_DDR5_P4=1u; \
                                            bFM_GPIO_PFR5_P4=0u; }while(0u)

/*---- GPIO bit NP54 ----*/
#define GPIO1PIN_NP54_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR5_P4)) )

#define GPIO1PIN_NP54_PUT(v)            ( bFM_GPIO_PDOR5_P4=(uint32_t)(!(v)) )

#define GPIO1PIN_NP54_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP54_INITIN(v) \
                                                           : GPIO1PIN_NP54_INITOUT(v) )

#define GPIO1PIN_NP54_INITIN(v)         do{ \
                                            bFM_GPIO_PCR5_P4=(v).bPullup; \
                                            bFM_GPIO_DDR5_P4=0u; \
                                            bFM_GPIO_PFR5_P4=0u; }while(0u)

#define GPIO1PIN_NP54_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR5_P4=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR5_P4=1u; \
                                            bFM_GPIO_PFR5_P4=0u; }while(0u)

/*---- GPIO bit P55 ----*/
#define GPIO1PIN_P55_GET                ( bFM_GPIO_PDIR5_P5 )

#define GPIO1PIN_P55_PUT(v)             ( bFM_GPIO_PDOR5_P5=(v) )

#define GPIO1PIN_P55_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P55_INITIN(v) \
                                                           : GPIO1PIN_P55_INITOUT(v) )

#define GPIO1PIN_P55_INITIN(v)          do{ \
                                            bFM_GPIO_PCR5_P5=(v).bPullup; \
                                            bFM_GPIO_DDR5_P5=0u; \
                                            bFM_GPIO_PFR5_P5=0u; }while(0u)

#define GPIO1PIN_P55_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR5_P5=(v).bInitVal; \
                                            bFM_GPIO_DDR5_P5=1u; \
                                            bFM_GPIO_PFR5_P5=0u; }while(0u)

/*---- GPIO bit NP55 ----*/
#define GPIO1PIN_NP55_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR5_P5)) )

#define GPIO1PIN_NP55_PUT(v)            ( bFM_GPIO_PDOR5_P5=(uint32_t)(!(v)) )

#define GPIO1PIN_NP55_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP55_INITIN(v) \
                                                           : GPIO1PIN_NP55_INITOUT(v) )

#define GPIO1PIN_NP55_INITIN(v)         do{ \
                                            bFM_GPIO_PCR5_P5=(v).bPullup; \
                                            bFM_GPIO_DDR5_P5=0u; \
                                            bFM_GPIO_PFR5_P5=0u; }while(0u)

#define GPIO1PIN_NP55_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR5_P5=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR5_P5=1u; \
                                            bFM_GPIO_PFR5_P5=0u; }while(0u)

/*---- GPIO bit P56 ----*/
#define GPIO1PIN_P56_GET                ( bFM_GPIO_PDIR5_P6 )

#define GPIO1PIN_P56_PUT(v)             ( bFM_GPIO_PDOR5_P6=(v) )

#define GPIO1PIN_P56_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P56_INITIN(v) \
                                                           : GPIO1PIN_P56_INITOUT(v) )

#define GPIO1PIN_P56_INITIN(v)          do{ \
                                            bFM_GPIO_PCR5_P6=(v).bPullup; \
                                            bFM_GPIO_DDR5_P6=0u; \
                                            bFM_GPIO_PFR5_P6=0u; }while(0u)

#define GPIO1PIN_P56_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR5_P6=(v).bInitVal; \
                                            bFM_GPIO_DDR5_P6=1u; \
                                            bFM_GPIO_PFR5_P6=0u; }while(0u)

/*---- GPIO bit NP56 ----*/
#define GPIO1PIN_NP56_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR5_P6)) )

#define GPIO1PIN_NP56_PUT(v)            ( bFM_GPIO_PDOR5_P6=(uint32_t)(!(v)) )

#define GPIO1PIN_NP56_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP56_INITIN(v) \
                                                           : GPIO1PIN_NP56_INITOUT(v) )

#define GPIO1PIN_NP56_INITIN(v)         do{ \
                                            bFM_GPIO_PCR5_P6=(v).bPullup; \
                                            bFM_GPIO_DDR5_P6=0u; \
                                            bFM_GPIO_PFR5_P6=0u; }while(0u)

#define GPIO1PIN_NP56_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR5_P6=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR5_P6=1u; \
                                            bFM_GPIO_PFR5_P6=0u; }while(0u)

/*---- GPIO bit P60 ----*/
#define GPIO1PIN_P60_GET                ( bFM_GPIO_PDIR6_P0 )

#define GPIO1PIN_P60_PUT(v)             ( bFM_GPIO_PDOR6_P0=(v) )

#define GPIO1PIN_P60_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P60_INITIN(v) \
                                                           : GPIO1PIN_P60_INITOUT(v) )

#define GPIO1PIN_P60_INITIN(v)          do{ \
                                            bFM_GPIO_PCR6_P0=(v).bPullup; \
                                            bFM_GPIO_DDR6_P0=0u; \
                                            bFM_GPIO_PFR6_P0=0u; }while(0u)

#define GPIO1PIN_P60_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR6_P0=(v).bInitVal; \
                                            bFM_GPIO_DDR6_P0=1u; \
                                            bFM_GPIO_PFR6_P0=0u; }while(0u)

/*---- GPIO bit NP60 ----*/
#define GPIO1PIN_NP60_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR6_P0)) )

#define GPIO1PIN_NP60_PUT(v)            ( bFM_GPIO_PDOR6_P0=(uint32_t)(!(v)) )

#define GPIO1PIN_NP60_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP60_INITIN(v) \
                                                           : GPIO1PIN_NP60_INITOUT(v) )

#define GPIO1PIN_NP60_INITIN(v)         do{ \
                                            bFM_GPIO_PCR6_P0=(v).bPullup; \
                                            bFM_GPIO_DDR6_P0=0u; \
                                            bFM_GPIO_PFR6_P0=0u; }while(0u)

#define GPIO1PIN_NP60_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR6_P0=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR6_P0=1u; \
                                            bFM_GPIO_PFR6_P0=0u; }while(0u)

/*---- GPIO bit P61 ----*/
#define GPIO1PIN_P61_GET                ( bFM_GPIO_PDIR6_P1 )

#define GPIO1PIN_P61_PUT(v)             ( bFM_GPIO_PDOR6_P1=(v) )

#define GPIO1PIN_P61_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P61_INITIN(v) \
                                                           : GPIO1PIN_P61_INITOUT(v) )

#define GPIO1PIN_P61_INITIN(v)          do{ \
                                            bFM_GPIO_PCR6_P1=(v).bPullup; \
                                            bFM_GPIO_DDR6_P1=0u; \
                                            bFM_GPIO_PFR6_P1=0u; }while(0u)

#define GPIO1PIN_P61_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR6_P1=(v).bInitVal; \
                                            bFM_GPIO_DDR6_P1=1u; \
                                            bFM_GPIO_PFR6_P1=0u; }while(0u)

/*---- GPIO bit NP61 ----*/
#define GPIO1PIN_NP61_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR6_P1)) )

#define GPIO1PIN_NP61_PUT(v)            ( bFM_GPIO_PDOR6_P1=(uint32_t)(!(v)) )

#define GPIO1PIN_NP61_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP61_INITIN(v) \
                                                           : GPIO1PIN_NP61_INITOUT(v) )

#define GPIO1PIN_NP61_INITIN(v)         do{ \
                                            bFM_GPIO_PCR6_P1=(v).bPullup; \
                                            bFM_GPIO_DDR6_P1=0u; \
                                            bFM_GPIO_PFR6_P1=0u; }while(0u)

#define GPIO1PIN_NP61_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR6_P1=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR6_P1=1u; \
                                            bFM_GPIO_PFR6_P1=0u; }while(0u)

/*---- GPIO bit P62 ----*/
#define GPIO1PIN_P62_GET                ( bFM_GPIO_PDIR6_P2 )

#define GPIO1PIN_P62_PUT(v)             ( bFM_GPIO_PDOR6_P2=(v) )

#define GPIO1PIN_P62_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P62_INITIN(v) \
                                                           : GPIO1PIN_P62_INITOUT(v) )

#define GPIO1PIN_P62_INITIN(v)          do{ \
                                            bFM_GPIO_PCR6_P2=(v).bPullup; \
                                            bFM_GPIO_DDR6_P2=0u; \
                                            bFM_GPIO_PFR6_P2=0u; }while(0u)

#define GPIO1PIN_P62_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR6_P2=(v).bInitVal; \
                                            bFM_GPIO_DDR6_P2=1u; \
                                            bFM_GPIO_PFR6_P2=0u; }while(0u)

/*---- GPIO bit NP62 ----*/
#define GPIO1PIN_NP62_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR6_P2)) )

#define GPIO1PIN_NP62_PUT(v)            ( bFM_GPIO_PDOR6_P2=(uint32_t)(!(v)) )

#define GPIO1PIN_NP62_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP62_INITIN(v) \
                                                           : GPIO1PIN_NP62_INITOUT(v) )

#define GPIO1PIN_NP62_INITIN(v)         do{ \
                                            bFM_GPIO_PCR6_P2=(v).bPullup; \
                                            bFM_GPIO_DDR6_P2=0u; \
                                            bFM_GPIO_PFR6_P2=0u; }while(0u)

#define GPIO1PIN_NP62_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR6_P2=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR6_P2=1u; \
                                            bFM_GPIO_PFR6_P2=0u; }while(0u)

/*---- GPIO bit P63 ----*/
#define GPIO1PIN_P63_GET                ( bFM_GPIO_PDIR6_P3 )

#define GPIO1PIN_P63_PUT(v)             ( bFM_GPIO_PDOR6_P3=(v) )

#define GPIO1PIN_P63_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P63_INITIN(v) \
                                                           : GPIO1PIN_P63_INITOUT(v) )

#define GPIO1PIN_P63_INITIN(v)          do{ \
                                            bFM_GPIO_PCR6_P3=(v).bPullup; \
                                            bFM_GPIO_DDR6_P3=0u; \
                                            bFM_GPIO_PFR6_P3=0u; }while(0u)

#define GPIO1PIN_P63_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR6_P3=(v).bInitVal; \
                                            bFM_GPIO_DDR6_P3=1u; \
                                            bFM_GPIO_PFR6_P3=0u; }while(0u)

/*---- GPIO bit NP63 ----*/
#define GPIO1PIN_NP63_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR6_P3)) )

#define GPIO1PIN_NP63_PUT(v)            ( bFM_GPIO_PDOR6_P3=(uint32_t)(!(v)) )

#define GPIO1PIN_NP63_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP63_INITIN(v) \
                                                           : GPIO1PIN_NP63_INITOUT(v) )

#define GPIO1PIN_NP63_INITIN(v)         do{ \
                                            bFM_GPIO_PCR6_P3=(v).bPullup; \
                                            bFM_GPIO_DDR6_P3=0u; \
                                            bFM_GPIO_PFR6_P3=0u; }while(0u)

#define GPIO1PIN_NP63_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR6_P3=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR6_P3=1u; \
                                            bFM_GPIO_PFR6_P3=0u; }while(0u)

/*---- GPIO bit P80 ----*/
#define GPIO1PIN_P80_GET                ( bFM_GPIO_PDIR8_P0 )

#define GPIO1PIN_P80_PUT(v)             ( bFM_GPIO_PDOR8_P0=(v) )

#define GPIO1PIN_P80_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P80_INITIN(v) \
                                                           : GPIO1PIN_P80_INITOUT(v) )

#define GPIO1PIN_P80_INITIN(v)          do{ \
                                            bFM_GPIO_DDR8_P0=0u; \
                                            bFM_GPIO_PFR8_P0=0u; }while(0u)

#define GPIO1PIN_P80_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR8_P0=(v).bInitVal; \
                                            bFM_GPIO_DDR8_P0=1u; \
                                            bFM_GPIO_PFR8_P0=0u; }while(0u)

/*---- GPIO bit NP80 ----*/
#define GPIO1PIN_NP80_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR8_P0)) )

#define GPIO1PIN_NP80_PUT(v)            ( bFM_GPIO_PDOR8_P0=(uint32_t)(!(v)) )

#define GPIO1PIN_NP80_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP80_INITIN(v) \
                                                           : GPIO1PIN_NP80_INITOUT(v) )

#define GPIO1PIN_NP80_INITIN(v)         do{ \
                                            bFM_GPIO_DDR8_P0=0u; \
                                            bFM_GPIO_PFR8_P0=0u; }while(0u)

#define GPIO1PIN_NP80_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR8_P0=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR8_P0=1u; \
                                            bFM_GPIO_PFR8_P0=0u; }while(0u)

/*---- GPIO bit P81 ----*/
#define GPIO1PIN_P81_GET                ( bFM_GPIO_PDIR8_P1 )

#define GPIO1PIN_P81_PUT(v)             ( bFM_GPIO_PDOR8_P1=(v) )

#define GPIO1PIN_P81_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_P81_INITIN(v) \
                                                           : GPIO1PIN_P81_INITOUT(v) )

#define GPIO1PIN_P81_INITIN(v)          do{ \
                                            bFM_GPIO_DDR8_P1=0u; \
                                            bFM_GPIO_PFR8_P1=0u; }while(0u)

#define GPIO1PIN_P81_INITOUT(v)         do{ \
                                            bFM_GPIO_PDOR8_P1=(v).bInitVal; \
                                            bFM_GPIO_DDR8_P1=1u; \
                                            bFM_GPIO_PFR8_P1=0u; }while(0u)

/*---- GPIO bit NP81 ----*/
#define GPIO1PIN_NP81_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIR8_P1)) )

#define GPIO1PIN_NP81_PUT(v)            ( bFM_GPIO_PDOR8_P1=(uint32_t)(!(v)) )

#define GPIO1PIN_NP81_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NP81_INITIN(v) \
                                                           : GPIO1PIN_NP81_INITOUT(v) )

#define GPIO1PIN_NP81_INITIN(v)         do{ \
                                            bFM_GPIO_DDR8_P1=0u; \
                                            bFM_GPIO_PFR8_P1=0u; }while(0u)

#define GPIO1PIN_NP81_INITOUT(v)        do{ \
                                            bFM_GPIO_PDOR8_P1=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDR8_P1=1u; \
                                            bFM_GPIO_PFR8_P1=0u; }while(0u)

/*---- GPIO bit PE0 ----*/
#define GPIO1PIN_PE0_GET                ( bFM_GPIO_PDIRE_P0 )

#define GPIO1PIN_PE0_PUT(v)             ( bFM_GPIO_PDORE_P0=(v) )

#define GPIO1PIN_PE0_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PE0_INITIN(v) \
                                                           : GPIO1PIN_PE0_INITOUT(v) )

#define GPIO1PIN_PE0_INITIN(v)          do{ \
                                            bFM_GPIO_PCRE_P0=(v).bPullup; \
                                            bFM_GPIO_DDRE_P0=0u; \
                                            bFM_GPIO_PFRE_P0=0u; }while(0u)

#define GPIO1PIN_PE0_INITOUT(v)         do{ \
                                            bFM_GPIO_PDORE_P0=(v).bInitVal; \
                                            bFM_GPIO_DDRE_P0=1u; \
                                            bFM_GPIO_PFRE_P0=0u; }while(0u)

/*---- GPIO bit NPE0 ----*/
#define GPIO1PIN_NPE0_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRE_P0)) )

#define GPIO1PIN_NPE0_PUT(v)            ( bFM_GPIO_PDORE_P0=(uint32_t)(!(v)) )

#define GPIO1PIN_NPE0_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPE0_INITIN(v) \
                                                           : GPIO1PIN_NPE0_INITOUT(v) )

#define GPIO1PIN_NPE0_INITIN(v)         do{ \
                                            bFM_GPIO_PCRE_P0=(v).bPullup; \
                                            bFM_GPIO_DDRE_P0=0u; \
                                            bFM_GPIO_PFRE_P0=0u; }while(0u)

#define GPIO1PIN_NPE0_INITOUT(v)        do{ \
                                            bFM_GPIO_PDORE_P0=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRE_P0=1u; \
                                            bFM_GPIO_PFRE_P0=0u; }while(0u)

/*---- GPIO bit PE2 ----*/
#define GPIO1PIN_PE2_GET                ( bFM_GPIO_PDIRE_P2 )

#define GPIO1PIN_PE2_PUT(v)             ( bFM_GPIO_PDORE_P2=(v) )

#define GPIO1PIN_PE2_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PE2_INITIN(v) \
                                                           : GPIO1PIN_PE2_INITOUT(v) )

#define GPIO1PIN_PE2_INITIN(v)          do{ \
                                            PINCONFIG_SET_REG(FM_GPIO->SPSR, 2u, 2u, 0u); \
                                            bFM_GPIO_PCRE_P2=(v).bPullup; \
                                            bFM_GPIO_DDRE_P2=0u; \
                                            bFM_GPIO_PFRE_P2=0u; }while(0u)

#define GPIO1PIN_PE2_INITOUT(v)         do{ \
                                            PINCONFIG_SET_REG(FM_GPIO->SPSR, 2u, 2u, 0u); \
                                            bFM_GPIO_PDORE_P2=(v).bInitVal; \
                                            bFM_GPIO_DDRE_P2=1u; \
                                            bFM_GPIO_PFRE_P2=0u; }while(0u)

/*---- GPIO bit NPE2 ----*/
#define GPIO1PIN_NPE2_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRE_P2)) )

#define GPIO1PIN_NPE2_PUT(v)            ( bFM_GPIO_PDORE_P2=(uint32_t)(!(v)) )

#define GPIO1PIN_NPE2_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPE2_INITIN(v) \
                                                           : GPIO1PIN_NPE2_INITOUT(v) )

#define GPIO1PIN_NPE2_INITIN(v)         do{ \
                                            PINCONFIG_SET_REG(FM_GPIO->SPSR, 2u, 2u, 0u); \
                                            bFM_GPIO_PCRE_P2=(v).bPullup; \
                                            bFM_GPIO_DDRE_P2=0u; \
                                            bFM_GPIO_PFRE_P2=0u; }while(0u)

#define GPIO1PIN_NPE2_INITOUT(v)        do{ \
                                            PINCONFIG_SET_REG(FM_GPIO->SPSR, 2u, 2u, 0u); \
                                            bFM_GPIO_PDORE_P2=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRE_P2=1u; \
                                            bFM_GPIO_PFRE_P2=0u; }while(0u)

/*---- GPIO bit PE3 ----*/
#define GPIO1PIN_PE3_GET                ( bFM_GPIO_PDIRE_P3 )

#define GPIO1PIN_PE3_PUT(v)             ( bFM_GPIO_PDORE_P3=(v) )

#define GPIO1PIN_PE3_INIT(v)            ( (0==(v).bOutput) ? GPIO1PIN_PE3_INITIN(v) \
                                                           : GPIO1PIN_PE3_INITOUT(v) )

#define GPIO1PIN_PE3_INITIN(v)          do{ \
                                            PINCONFIG_SET_REG(FM_GPIO->SPSR, 2u, 2u, 0u); \
                                            bFM_GPIO_PCRE_P3=(v).bPullup; \
                                            bFM_GPIO_DDRE_P3=0u; \
                                            bFM_GPIO_PFRE_P3=0u; }while(0u)

#define GPIO1PIN_PE3_INITOUT(v)         do{ \
                                            PINCONFIG_SET_REG(FM_GPIO->SPSR, 2u, 2u, 0u); \
                                            bFM_GPIO_PDORE_P3=(v).bInitVal; \
                                            bFM_GPIO_DDRE_P3=1u; \
                                            bFM_GPIO_PFRE_P3=0u; }while(0u)

/*---- GPIO bit NPE3 ----*/
#define GPIO1PIN_NPE3_GET               ( (uint32_t)(!(uint32_t)(bFM_GPIO_PDIRE_P3)) )

#define GPIO1PIN_NPE3_PUT(v)            ( bFM_GPIO_PDORE_P3=(uint32_t)(!(v)) )

#define GPIO1PIN_NPE3_INIT(v)           ( (0==(v).bOutput) ? GPIO1PIN_NPE3_INITIN(v) \
                                                           : GPIO1PIN_NPE3_INITOUT(v) )

#define GPIO1PIN_NPE3_INITIN(v)         do{ \
                                            PINCONFIG_SET_REG(FM_GPIO->SPSR, 2u, 2u, 0u); \
                                            bFM_GPIO_PCRE_P3=(v).bPullup; \
                                            bFM_GPIO_DDRE_P3=0u; \
                                            bFM_GPIO_PFRE_P3=0u; }while(0u)

#define GPIO1PIN_NPE3_INITOUT(v)        do{ \
                                            PINCONFIG_SET_REG(FM_GPIO->SPSR, 2u, 2u, 0u); \
                                            bFM_GPIO_PDORE_P3=(uint32_t)(!((uint32_t)(v).bInitVal)); \
                                            bFM_GPIO_DDRE_P3=1u; \
                                            bFM_GPIO_PFRE_P3=0u; }while(0u)

/******************************************************************************
   PIN RELOCATION
*******************************************************************************/

/*--- ADTG_1_ADC0 ---*/
#define SetPinFunc_ADTG_1_ADC0(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 12u, 4u,  2u ); \
                                            bFM_GPIO_PFR5_P5 = 1u; \
                                        }while (0u)

/*--- ADTG_1_ADC1 ---*/
#define SetPinFunc_ADTG_1_ADC1(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 16u, 4u,  2u ); \
                                            bFM_GPIO_PFR5_P5 = 1u; \
                                        }while (0u)

/*--- ADTG_1_ADC2 ---*/
#define SetPinFunc_ADTG_1_ADC2(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 20u, 4u,  2u ); \
                                            bFM_GPIO_PFR5_P5 = 1u; \
                                        }while (0u)

/*--- ADTG_2_ADC0 ---*/
#define SetPinFunc_ADTG_2_ADC0(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 12u, 4u,  3u ); \
                                            bFM_GPIO_PFR3_P9 = 1u; \
                                        }while (0u)

/*--- ADTG_2_ADC1 ---*/
#define SetPinFunc_ADTG_2_ADC1(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 16u, 4u,  3u ); \
                                            bFM_GPIO_PFR3_P9 = 1u; \
                                        }while (0u)

/*--- ADTG_2_ADC2 ---*/
#define SetPinFunc_ADTG_2_ADC2(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 20u, 4u,  3u ); \
                                            bFM_GPIO_PFR3_P9 = 1u; \
                                        }while (0u)

/*--- ADTG_3_ADC0 ---*/
#define SetPinFunc_ADTG_3_ADC0(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 12u, 4u,  4u ); \
                                            bFM_GPIO_PFR6_P2 = 1u; \
                                        }while (0u)

/*--- ADTG_3_ADC1 ---*/
#define SetPinFunc_ADTG_3_ADC1(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 16u, 4u,  4u ); \
                                            bFM_GPIO_PFR6_P2 = 1u; \
                                        }while (0u)

/*--- ADTG_3_ADC2 ---*/
#define SetPinFunc_ADTG_3_ADC2(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 20u, 4u,  4u ); \
                                            bFM_GPIO_PFR6_P2 = 1u; \
                                        }while (0u)

/*--- ADTG_6_ADC0 ---*/
#define SetPinFunc_ADTG_6_ADC0(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 12u, 4u,  7u ); \
                                            bFM_GPIO_PFR3_P3 = 1u; \
                                        }while (0u)

/*--- ADTG_6_ADC1 ---*/
#define SetPinFunc_ADTG_6_ADC1(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 16u, 4u,  7u ); \
                                            bFM_GPIO_PFR3_P3 = 1u; \
                                        }while (0u)

/*--- ADTG_6_ADC2 ---*/
#define SetPinFunc_ADTG_6_ADC2(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 20u, 4u,  7u ); \
                                            bFM_GPIO_PFR3_P3 = 1u; \
                                        }while (0u)

/*--- AIN0_0 ---*/
#define SetPinFunc_AIN0_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 0u, 2u,  1u ); \
                                            bFM_GPIO_PFR3_PA = 1u; \
                                        }while (0u)

/*--- AIN0_2 ---*/
#define SetPinFunc_AIN0_2(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 0u, 2u,  3u ); \
                                            bFM_GPIO_PFR5_P0 = 1u; \
                                        }while (0u)

/*--- AIN1_2 ---*/
#define SetPinFunc_AIN1_2(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 6u, 2u,  3u ); \
                                            bFM_GPIO_PFR4_PC = 1u; \
                                        }while (0u)

/*--- AIN2_1 ---*/
#define SetPinFunc_AIN2_1(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR14, 0u, 2u,  2u ); \
                                            bFM_GPIO_PFR8_P1 = 1u; \
                                        }while (0u)

/*--- AIN2_2 ---*/
#define SetPinFunc_AIN2_2(dummy)        do{ \
                                            bFM_GPIO_ADE_AN07=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR14, 0u, 2u,  3u ); \
                                            bFM_GPIO_PFR1_P7 = 1u; \
                                        }while (0u)

/*--- BIN0_0 ---*/
#define SetPinFunc_BIN0_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 2u, 2u,  1u ); \
                                            bFM_GPIO_PFR3_PB = 1u; \
                                        }while (0u)

/*--- BIN0_2 ---*/
#define SetPinFunc_BIN0_2(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 2u, 2u,  3u ); \
                                            bFM_GPIO_PFR5_P1 = 1u; \
                                        }while (0u)

/*--- BIN1_2 ---*/
#define SetPinFunc_BIN1_2(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 8u, 2u,  3u ); \
                                            bFM_GPIO_PFR4_PD = 1u; \
                                        }while (0u)

/*--- BIN2_1 ---*/
#define SetPinFunc_BIN2_1(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR14, 2u, 2u,  2u ); \
                                            bFM_GPIO_PFR8_P0 = 1u; \
                                        }while (0u)

/*--- BIN2_2 ---*/
#define SetPinFunc_BIN2_2(dummy)        do{ \
                                            bFM_GPIO_ADE_AN06=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR14, 2u, 2u,  3u ); \
                                            bFM_GPIO_PFR1_P6 = 1u; \
                                        }while (0u)

/*--- CROUT_0 ---*/
#define SetPinFunc_CROUT_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN16=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 1u, 2u,  1u ); \
                                            bFM_GPIO_PFR2_P2 = 1u; \
                                        }while (0u)

/*--- CROUT_1 ---*/
#define SetPinFunc_CROUT_1(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 1u, 2u,  2u ); \
                                            bFM_GPIO_PFR6_P3 = 1u; \
                                        }while (0u)

/*--- CTS4_0 ---*/
#define SetPinFunc_CTS4_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 2u, 2u,  1u ); \
                                            bFM_GPIO_PFR5_P0 = 1u; \
                                        }while (0u)

/*--- DA0 ---*/
#define SetPinFunc_DA0(dummy)           do{ \
                                            /* bFM_DAC0_DACR_DAE=1u; */ \
                                        }while (0u)

/*--- DA1 ---*/
#define SetPinFunc_DA1(dummy)           do{ \
                                            /* bFM_DAC1_DACR_DAE=1u; */ \
                                        }while (0u)

/*--- DTTI0X_0 ---*/
#define SetPinFunc_DTTI0X_0(dummy)      do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 16u, 2u,  1u ); \
                                            bFM_GPIO_PFR3_P9 = 1u; \
                                        }while (0u)

/*--- DTTI1X_0 ---*/
#define SetPinFunc_DTTI1X_0(dummy)      do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 16u, 2u,  1u ); \
                                            bFM_GPIO_PFR5_P6 = 1u; \
                                        }while (0u)

/*--- DTTI2X_0 ---*/
#define SetPinFunc_DTTI2X_0(dummy)      do{ \
                                            bFM_GPIO_ADE_AN08=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 16u, 2u,  1u ); \
                                            bFM_GPIO_PFR1_P8 = 1u; \
                                        }while (0u)

/*--- DTTI2X_1 ---*/
#define SetPinFunc_DTTI2X_1(dummy)      do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 16u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_P1 = 1u; \
                                        }while (0u)

/*--- FRCK0_2 ---*/
#define SetPinFunc_FRCK0_2(dummy)       do{ \
                                            bFM_GPIO_ADE_AN00=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 18u, 2u,  3u ); \
                                            bFM_GPIO_PFR1_P0 = 1u; \
                                        }while (0u)

/*--- FRCK1_0 ---*/
#define SetPinFunc_FRCK1_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 18u, 2u,  1u ); \
                                            bFM_GPIO_PFR0_PA = 1u; \
                                        }while (0u)

/*--- FRCK1_1 ---*/
#define SetPinFunc_FRCK1_1(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 18u, 2u,  2u ); \
                                            bFM_GPIO_PFR4_PE = 1u; \
                                        }while (0u)

/*--- FRCK2_0 ---*/
#define SetPinFunc_FRCK2_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 18u, 2u,  1u ); \
                                            bFM_GPIO_PFR6_P0 = 1u; \
                                        }while (0u)

/*--- IC00_1 ---*/
#define SetPinFunc_IC00_1(dummy)        do{ \
                                            bFM_GPIO_ADE_AN09=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 20u, 3u,  2u ); \
                                            bFM_GPIO_PFR1_P9 = 1u; \
                                        }while (0u)

/*--- IC00_2 ---*/
#define SetPinFunc_IC00_2(dummy)        do{ \
                                            bFM_GPIO_ADE_AN01=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 20u, 3u,  3u ); \
                                            bFM_GPIO_PFR1_P1 = 1u; \
                                        }while (0u)

/*--- IC00_LSYN0 ---*/
#define SetPinFunc_IC00_LSYN0(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 20u, 3u,  4u ); \
                                        }while (0u)

/*--- IC00_LSYN4 ---*/
#define SetPinFunc_IC00_LSYN4(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 20u, 3u,  5u ); \
                                        }while (0u)

/*--- IC01_1 ---*/
#define SetPinFunc_IC01_1(dummy)        do{ \
                                            bFM_GPIO_ADE_AN10=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 23u, 3u,  2u ); \
                                            bFM_GPIO_PFR1_PA = 1u; \
                                        }while (0u)

/*--- IC01_2 ---*/
#define SetPinFunc_IC01_2(dummy)        do{ \
                                            bFM_GPIO_ADE_AN02=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 23u, 3u,  3u ); \
                                            bFM_GPIO_PFR1_P2 = 1u; \
                                        }while (0u)

/*--- IC01_LSYN1 ---*/
#define SetPinFunc_IC01_LSYN1(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 23u, 3u,  4u ); \
                                        }while (0u)

/*--- IC01_LSYN5 ---*/
#define SetPinFunc_IC01_LSYN5(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 23u, 3u,  5u ); \
                                        }while (0u)

/*--- IC02_1 ---*/
#define SetPinFunc_IC02_1(dummy)        do{ \
                                            bFM_GPIO_ADE_AN11=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 26u, 3u,  2u ); \
                                            bFM_GPIO_PFR1_PB = 1u; \
                                        }while (0u)

/*--- IC02_2 ---*/
#define SetPinFunc_IC02_2(dummy)        do{ \
                                            bFM_GPIO_ADE_AN03=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 26u, 3u,  3u ); \
                                            bFM_GPIO_PFR1_P3 = 1u; \
                                        }while (0u)

/*--- IC02_LSYN2 ---*/
#define SetPinFunc_IC02_LSYN2(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 26u, 3u,  4u ); \
                                        }while (0u)

/*--- IC02_LSYN6 ---*/
#define SetPinFunc_IC02_LSYN6(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 26u, 3u,  5u ); \
                                        }while (0u)

/*--- IC03_2 ---*/
#define SetPinFunc_IC03_2(dummy)        do{ \
                                            bFM_GPIO_ADE_AN04=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 29u, 3u,  3u ); \
                                            bFM_GPIO_PFR1_P4 = 1u; \
                                        }while (0u)

/*--- IC03_LSYN3 ---*/
#define SetPinFunc_IC03_LSYN3(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 29u, 3u,  4u ); \
                                        }while (0u)

/*--- IC03_LSYN7 ---*/
#define SetPinFunc_IC03_LSYN7(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 29u, 3u,  5u ); \
                                        }while (0u)

/*--- IC10_0 ---*/
#define SetPinFunc_IC10_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 20u, 3u,  1u ); \
                                            bFM_GPIO_PFR0_PB = 1u; \
                                        }while (0u)

/*--- IC10_LSYN0 ---*/
#define SetPinFunc_IC10_LSYN0(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 20u, 3u,  4u ); \
                                        }while (0u)

/*--- IC10_LSYN4 ---*/
#define SetPinFunc_IC10_LSYN4(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 20u, 3u,  5u ); \
                                        }while (0u)

/*--- IC11_0 ---*/
#define SetPinFunc_IC11_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 23u, 3u,  1u ); \
                                            bFM_GPIO_PFR0_PC = 1u; \
                                        }while (0u)

/*--- IC11_LSYN1 ---*/
#define SetPinFunc_IC11_LSYN1(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 23u, 3u,  4u ); \
                                        }while (0u)

/*--- IC11_LSYN5 ---*/
#define SetPinFunc_IC11_LSYN5(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 23u, 3u,  5u ); \
                                        }while (0u)

/*--- IC12_0 ---*/
#define SetPinFunc_IC12_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 26u, 3u,  1u ); \
                                            bFM_GPIO_PFR0_PD = 1u; \
                                        }while (0u)

/*--- IC12_LSYN2 ---*/
#define SetPinFunc_IC12_LSYN2(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 26u, 3u,  4u ); \
                                        }while (0u)

/*--- IC12_LSYN6 ---*/
#define SetPinFunc_IC12_LSYN6(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 26u, 3u,  5u ); \
                                        }while (0u)

/*--- IC13_0 ---*/
#define SetPinFunc_IC13_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 29u, 3u,  1u ); \
                                            bFM_GPIO_PFR0_PE = 1u; \
                                        }while (0u)

/*--- IC13_LSYN3 ---*/
#define SetPinFunc_IC13_LSYN3(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 29u, 3u,  4u ); \
                                        }while (0u)

/*--- IC13_LSYN7 ---*/
#define SetPinFunc_IC13_LSYN7(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 29u, 3u,  5u ); \
                                        }while (0u)

/*--- IC20_0 ---*/
#define SetPinFunc_IC20_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 20u, 3u,  1u ); \
                                            bFM_GPIO_PFR8_P1 = 1u; \
                                        }while (0u)

/*--- IC20_LSYN0 ---*/
#define SetPinFunc_IC20_LSYN0(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 20u, 3u,  4u ); \
                                        }while (0u)

/*--- IC20_LSYN4 ---*/
#define SetPinFunc_IC20_LSYN4(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 20u, 3u,  5u ); \
                                        }while (0u)

/*--- IC21_0 ---*/
#define SetPinFunc_IC21_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 23u, 3u,  1u ); \
                                            bFM_GPIO_PFR8_P0 = 1u; \
                                        }while (0u)

/*--- IC21_LSYN1 ---*/
#define SetPinFunc_IC21_LSYN1(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 23u, 3u,  4u ); \
                                        }while (0u)

/*--- IC21_LSYN5 ---*/
#define SetPinFunc_IC21_LSYN5(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 23u, 3u,  5u ); \
                                        }while (0u)

/*--- IC22_0 ---*/
#define SetPinFunc_IC22_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 26u, 3u,  1u ); \
                                            bFM_GPIO_PFR6_P2 = 1u; \
                                        }while (0u)

/*--- IC22_LSYN2 ---*/
#define SetPinFunc_IC22_LSYN2(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 26u, 3u,  4u ); \
                                        }while (0u)

/*--- IC22_LSYN6 ---*/
#define SetPinFunc_IC22_LSYN6(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 26u, 3u,  5u ); \
                                        }while (0u)

/*--- IC23_0 ---*/
#define SetPinFunc_IC23_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 29u, 3u,  1u ); \
                                            bFM_GPIO_PFR6_P3 = 1u; \
                                        }while (0u)

/*--- IC23_1 ---*/
#define SetPinFunc_IC23_1(dummy)        do{ \
                                            bFM_GPIO_ADE_AN19=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 29u, 3u,  2u ); \
                                            bFM_GPIO_PFR0_P9 = 1u; \
                                        }while (0u)

/*--- IC23_LSYN3 ---*/
#define SetPinFunc_IC23_LSYN3(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 29u, 3u,  4u ); \
                                        }while (0u)

/*--- IC23_LSYN7 ---*/
#define SetPinFunc_IC23_LSYN7(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 29u, 3u,  5u ); \
                                        }while (0u)

/*--- INT00_0 ---*/
#define SetPinFunc_INT00_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 0u, 2u,  1u ); \
                                            bFM_GPIO_PFR5_P0 = 1u; \
                                        }while (0u)

/*--- INT00_1 ---*/
#define SetPinFunc_INT00_1(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 0u, 2u,  2u ); \
                                            bFM_GPIO_PFR0_PB = 1u; \
                                        }while (0u)

/*--- INT01_0 ---*/
#define SetPinFunc_INT01_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 2u, 2u,  1u ); \
                                            bFM_GPIO_PFR5_P1 = 1u; \
                                        }while (0u)

/*--- INT02_0 ---*/
#define SetPinFunc_INT02_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 4u, 2u,  1u ); \
                                            bFM_GPIO_PFR5_P4 = 1u; \
                                        }while (0u)

/*--- INT02_1 ---*/
#define SetPinFunc_INT02_1(dummy)       do{ \
                                            bFM_GPIO_ADE_AN00=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 4u, 2u,  2u ); \
                                            bFM_GPIO_PFR1_P0 = 1u; \
                                        }while (0u)

/*--- INT03_0 ---*/
#define SetPinFunc_INT03_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 6u, 2u,  1u ); \
                                            bFM_GPIO_PFR6_P3 = 1u; \
                                        }while (0u)

/*--- INT03_1 ---*/
#define SetPinFunc_INT03_1(dummy)       do{ \
                                            bFM_GPIO_ADE_AN03=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 6u, 2u,  2u ); \
                                            bFM_GPIO_PFR1_P3 = 1u; \
                                        }while (0u)

/*--- INT04_0 ---*/
#define SetPinFunc_INT04_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 8u, 2u,  1u ); \
                                            bFM_GPIO_PFR3_P3 = 1u; \
                                        }while (0u)

/*--- INT04_1 ---*/
#define SetPinFunc_INT04_1(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 8u, 2u,  2u ); \
                                            bFM_GPIO_PFR6_P2 = 1u; \
                                        }while (0u)

/*--- INT05_1 ---*/
#define SetPinFunc_INT05_1(dummy)       do{ \
                                            bFM_GPIO_ADE_AN09=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 10u, 2u,  2u ); \
                                            bFM_GPIO_PFR1_P9 = 1u; \
                                        }while (0u)

/*--- INT06_1 ---*/
#define SetPinFunc_INT06_1(dummy)       do{ \
                                            bFM_GPIO_ADE_AN17=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 12u, 2u,  2u ); \
                                            bFM_GPIO_PFR2_P1 = 1u; \
                                        }while (0u)

/*--- INT07_2 ---*/
#define SetPinFunc_INT07_2(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 14u, 2u,  3u ); \
                                            bFM_GPIO_PFR5_P5 = 1u; \
                                        }while (0u)

/*--- INT08_2 ---*/
#define SetPinFunc_INT08_2(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 16u, 2u,  3u ); \
                                            bFM_GPIO_PFR5_P6 = 1u; \
                                        }while (0u)

/*--- INT09_2 ---*/
#define SetPinFunc_INT09_2(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 18u, 2u,  3u ); \
                                            bFM_GPIO_PFR3_P1 = 1u; \
                                        }while (0u)

/*--- INT10_1 ---*/
#define SetPinFunc_INT10_1(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 20u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_P2 = 1u; \
                                        }while (0u)

/*--- INT11_1 ---*/
#define SetPinFunc_INT11_1(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 22u, 2u,  2u ); \
                                            bFM_GPIO_PFR4_PE = 1u; \
                                        }while (0u)

/*--- INT12_2 ---*/
#define SetPinFunc_INT12_2(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 24u, 2u,  3u ); \
                                            bFM_GPIO_PFR0_PA = 1u; \
                                        }while (0u)

/*--- INT13_2 ---*/
#define SetPinFunc_INT13_2(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 26u, 2u,  3u ); \
                                            bFM_GPIO_PFR4_PD = 1u; \
                                        }while (0u)

/*--- INT14_1 ---*/
#define SetPinFunc_INT14_1(dummy)       do{ \
                                            bFM_GPIO_ADE_AN06=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 28u, 2u,  2u ); \
                                            bFM_GPIO_PFR1_P6 = 1u; \
                                        }while (0u)

/*--- INT15_2 ---*/
#define SetPinFunc_INT15_2(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR06, 30u, 2u,  3u ); \
                                            bFM_GPIO_PFR3_P0 = 1u; \
                                        }while (0u)

/*--- MAD00_0 ---*/
#define SetPinFunc_MAD00_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 14u, 1u,  1u ); \
                                            bFM_GPIO_PFR3_PD = 1u; \
                                        }while (0u)

/*--- MAD01_0 ---*/
#define SetPinFunc_MAD01_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 2u, 1u,  1u ); \
                                            bFM_GPIO_PFR3_PE = 1u; \
                                        }while (0u)

/*--- MAD02_0 ---*/
#define SetPinFunc_MAD02_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 3u, 1u,  1u ); \
                                            bFM_GPIO_PFR3_PF = 1u; \
                                        }while (0u)

/*--- MAD03_0 ---*/
#define SetPinFunc_MAD03_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 4u, 1u,  1u ); \
                                            bFM_GPIO_PFR4_PB = 1u; \
                                        }while (0u)

/*--- MAD04_0 ---*/
#define SetPinFunc_MAD04_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 5u, 1u,  1u ); \
                                            bFM_GPIO_PFR4_PC = 1u; \
                                        }while (0u)

/*--- MAD05_0 ---*/
#define SetPinFunc_MAD05_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 6u, 1u,  1u ); \
                                            bFM_GPIO_PFR4_PD = 1u; \
                                        }while (0u)

/*--- MAD06_0 ---*/
#define SetPinFunc_MAD06_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 7u, 1u,  1u ); \
                                            bFM_GPIO_PFR4_PE = 1u; \
                                        }while (0u)

/*--- MAD07_0 ---*/
#define SetPinFunc_MAD07_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN00=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 8u, 1u,  1u ); \
                                            bFM_GPIO_PFR1_P0 = 1u; \
                                        }while (0u)

/*--- MAD08_0 ---*/
#define SetPinFunc_MAD08_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN01=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 15u, 1u,  1u ); \
                                            bFM_GPIO_PFR1_P1 = 1u; \
                                        }while (0u)

/*--- MAD09_0 ---*/
#define SetPinFunc_MAD09_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN02=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 16u, 1u,  1u ); \
                                            bFM_GPIO_PFR1_P2 = 1u; \
                                        }while (0u)

/*--- MAD10_0 ---*/
#define SetPinFunc_MAD10_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN03=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 17u, 1u,  1u ); \
                                            bFM_GPIO_PFR1_P3 = 1u; \
                                        }while (0u)

/*--- MAD11_0 ---*/
#define SetPinFunc_MAD11_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN04=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 18u, 1u,  1u ); \
                                            bFM_GPIO_PFR1_P4 = 1u; \
                                        }while (0u)

/*--- MAD12_0 ---*/
#define SetPinFunc_MAD12_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN05=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 19u, 1u,  1u ); \
                                            bFM_GPIO_PFR1_P5 = 1u; \
                                        }while (0u)

/*--- MAD13_0 ---*/
#define SetPinFunc_MAD13_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN06=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 20u, 1u,  1u ); \
                                            bFM_GPIO_PFR1_P6 = 1u; \
                                        }while (0u)

/*--- MAD14_0 ---*/
#define SetPinFunc_MAD14_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN07=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 21u, 1u,  1u ); \
                                            bFM_GPIO_PFR1_P7 = 1u; \
                                        }while (0u)

/*--- MAD15_0 ---*/
#define SetPinFunc_MAD15_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN08=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 22u, 1u,  1u ); \
                                            bFM_GPIO_PFR1_P8 = 1u; \
                                        }while (0u)

/*--- MAD16_0 ---*/
#define SetPinFunc_MAD16_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN09=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 23u, 1u,  1u ); \
                                            bFM_GPIO_PFR1_P9 = 1u; \
                                        }while (0u)

/*--- MAD17_0 ---*/
#define SetPinFunc_MAD17_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN10=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 24u, 1u,  1u ); \
                                            bFM_GPIO_PFR1_PA = 1u; \
                                        }while (0u)

/*--- MAD18_0 ---*/
#define SetPinFunc_MAD18_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN11=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 25u, 1u,  1u ); \
                                            bFM_GPIO_PFR1_PB = 1u; \
                                        }while (0u)

/*--- MADATA00_0 ---*/
#define SetPinFunc_MADATA00_0(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 9u, 1u,  1u ); \
                                            bFM_GPIO_PFR5_P0 = 1u; \
                                        }while (0u)

/*--- MADATA01_0 ---*/
#define SetPinFunc_MADATA01_0(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 10u, 1u,  1u ); \
                                            bFM_GPIO_PFR5_P1 = 1u; \
                                        }while (0u)

/*--- MADATA02_0 ---*/
#define SetPinFunc_MADATA02_0(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 11u, 1u,  1u ); \
                                            bFM_GPIO_PFR5_P2 = 1u; \
                                        }while (0u)

/*--- MADATA03_0 ---*/
#define SetPinFunc_MADATA03_0(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 12u, 1u,  1u ); \
                                            bFM_GPIO_PFR5_P3 = 1u; \
                                        }while (0u)

/*--- MADATA04_0 ---*/
#define SetPinFunc_MADATA04_0(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 13u, 1u,  1u ); \
                                            bFM_GPIO_PFR5_P4 = 1u; \
                                        }while (0u)

/*--- MADATA05_0 ---*/
#define SetPinFunc_MADATA05_0(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 14u, 1u,  1u ); \
                                            bFM_GPIO_PFR5_P5 = 1u; \
                                        }while (0u)

/*--- MADATA06_0 ---*/
#define SetPinFunc_MADATA06_0(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 15u, 1u,  1u ); \
                                            bFM_GPIO_PFR5_P6 = 1u; \
                                        }while (0u)

/*--- MADATA07_0 ---*/
#define SetPinFunc_MADATA07_0(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 16u, 1u,  1u ); \
                                            bFM_GPIO_PFR3_P0 = 1u; \
                                        }while (0u)

/*--- MADATA08_0 ---*/
#define SetPinFunc_MADATA08_0(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 17u, 1u,  1u ); \
                                            bFM_GPIO_PFR3_P1 = 1u; \
                                        }while (0u)

/*--- MADATA09_0 ---*/
#define SetPinFunc_MADATA09_0(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 18u, 1u,  1u ); \
                                            bFM_GPIO_PFR3_P2 = 1u; \
                                        }while (0u)

/*--- MADATA10_0 ---*/
#define SetPinFunc_MADATA10_0(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 19u, 1u,  1u ); \
                                            bFM_GPIO_PFR3_P3 = 1u; \
                                        }while (0u)

/*--- MALE_0 ---*/
#define SetPinFunc_MALE_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 0u, 1u,  1u ); \
                                            bFM_GPIO_PFR0_PC = 1u; \
                                        }while (0u)

/*--- MCSX0_0 ---*/
#define SetPinFunc_MCSX0_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR11, 1u, 1u,  1u ); \
                                            bFM_GPIO_PFR0_PB = 1u; \
                                        }while (0u)

/*--- MCSX1_0 ---*/
#define SetPinFunc_MCSX1_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 7u, 1u,  1u ); \
                                            bFM_GPIO_PFR0_PA = 1u; \
                                        }while (0u)

/*--- MCSX5_0 ---*/
#define SetPinFunc_MCSX5_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN19=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 11u, 1u,  1u ); \
                                            bFM_GPIO_PFR0_P9 = 1u; \
                                        }while (0u)

/*--- MCSX6_0 ---*/
#define SetPinFunc_MCSX6_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 12u, 1u,  1u ); \
                                            bFM_GPIO_PFR0_P2 = 1u; \
                                        }while (0u)

/*--- MCSX7_0 ---*/
#define SetPinFunc_MCSX7_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 13u, 1u,  1u ); \
                                            bFM_GPIO_PFR0_P0 = 1u; \
                                        }while (0u)

/*--- MDQM0_0 ---*/
#define SetPinFunc_MDQM0_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 4u, 1u,  1u ); \
                                            bFM_GPIO_PFR0_PD = 1u; \
                                        }while (0u)

/*--- MDQM1_0 ---*/
#define SetPinFunc_MDQM1_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 4u, 1u,  1u ); \
                                            bFM_GPIO_PFR0_PE = 1u; \
                                        }while (0u)

/*--- MOEX_0 ---*/
#define SetPinFunc_MOEX_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 5u, 1u,  1u ); \
                                            bFM_GPIO_PFR6_P2 = 1u; \
                                        }while (0u)

/*--- MRDY_0 ---*/
#define SetPinFunc_MRDY_0(dummy)        do{ \
                                            bFM_GPIO_PFR6_P0 = 1u; \
                                        }while (0u)

/*--- MWEX_0 ---*/
#define SetPinFunc_MWEX_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR10, 3u, 1u,  1u ); \
                                            bFM_GPIO_PFR6_P3 = 1u; \
                                        }while (0u)

/*--- NMIX ---*/
#define SetPinFunc_NMIX(dummy)          do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 0u, 1u,  1u ); \
                                            bFM_GPIO_PFR6_P0 = 1u; \
                                        }while (0u)

/*--- RTCCO_0 ---*/
#define SetPinFunc_RTCCO_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 4u, 2u,  1u ); \
                                            bFM_GPIO_PFR6_P1 = 1u; \
                                        }while (0u)

/*--- RTCCO_1 ---*/
#define SetPinFunc_RTCCO_1(dummy)       do{ \
                                            bFM_GPIO_ADE_AN02=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 4u, 2u,  2u ); \
                                            bFM_GPIO_PFR1_P2 = 1u; \
                                        }while (0u)

/*--- RTCCO_2 ---*/
#define SetPinFunc_RTCCO_2(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 4u, 2u,  3u ); \
                                            bFM_GPIO_PFR3_P9 = 1u; \
                                        }while (0u)

/*--- RTO00_0 ---*/
#define SetPinFunc_RTO00_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 0u, 2u,  1u ); \
                                            bFM_GPIO_PFR3_PA = 1u; \
                                        }while (0u)

/*--- RTO00_1 ---*/
#define SetPinFunc_RTO00_1(dummy)       do{ \
                                            bFM_GPIO_ADE_AN15=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 0u, 2u,  2u ); \
                                            bFM_GPIO_PFR2_P3 = 1u; \
                                        }while (0u)

/*--- RTO01_0 ---*/
#define SetPinFunc_RTO01_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 2u, 2u,  1u ); \
                                            bFM_GPIO_PFR3_PB = 1u; \
                                        }while (0u)

/*--- RTO02_0 ---*/
#define SetPinFunc_RTO02_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 4u, 2u,  1u ); \
                                            bFM_GPIO_PFR3_PC = 1u; \
                                        }while (0u)

/*--- RTO03_0 ---*/
#define SetPinFunc_RTO03_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 6u, 2u,  1u ); \
                                            bFM_GPIO_PFR3_PD = 1u; \
                                        }while (0u)

/*--- RTO04_0 ---*/
#define SetPinFunc_RTO04_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 8u, 2u,  1u ); \
                                            bFM_GPIO_PFR3_PE = 1u; \
                                        }while (0u)

/*--- RTO05_0 ---*/
#define SetPinFunc_RTO05_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR01, 10u, 2u,  1u ); \
                                            bFM_GPIO_PFR3_PF = 1u; \
                                        }while (0u)

/*--- RTO10_0 ---*/
#define SetPinFunc_RTO10_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 0u, 2u,  1u ); \
                                            bFM_GPIO_PFR5_P0 = 1u; \
                                        }while (0u)

/*--- RTO11_0 ---*/
#define SetPinFunc_RTO11_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 2u, 2u,  1u ); \
                                            bFM_GPIO_PFR5_P1 = 1u; \
                                        }while (0u)

/*--- RTO12_0 ---*/
#define SetPinFunc_RTO12_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 4u, 2u,  1u ); \
                                            bFM_GPIO_PFR5_P2 = 1u; \
                                        }while (0u)

/*--- RTO13_0 ---*/
#define SetPinFunc_RTO13_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 6u, 2u,  1u ); \
                                            bFM_GPIO_PFR5_P3 = 1u; \
                                        }while (0u)

/*--- RTO14_0 ---*/
#define SetPinFunc_RTO14_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 8u, 2u,  1u ); \
                                            bFM_GPIO_PFR5_P4 = 1u; \
                                        }while (0u)

/*--- RTO14_1 ---*/
#define SetPinFunc_RTO14_1(dummy)       do{ \
                                            bFM_DAC0_DACR_DAE=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 8u, 2u,  2u ); \
                                            bFM_GPIO_PFR4_P4 = 1u; \
                                        }while (0u)

/*--- RTO15_0 ---*/
#define SetPinFunc_RTO15_0(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 10u, 2u,  1u ); \
                                            bFM_GPIO_PFR5_P5 = 1u; \
                                        }while (0u)

/*--- RTO15_1 ---*/
#define SetPinFunc_RTO15_1(dummy)       do{ \
                                            bFM_DAC1_DACR_DAE=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR02, 10u, 2u,  2u ); \
                                            bFM_GPIO_PFR4_P5 = 1u; \
                                        }while (0u)

/*--- RTO20_0 ---*/
#define SetPinFunc_RTO20_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN07=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 0u, 2u,  1u ); \
                                            bFM_GPIO_PFR1_P7 = 1u; \
                                        }while (0u)

/*--- RTO21_0 ---*/
#define SetPinFunc_RTO21_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN06=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 2u, 2u,  1u ); \
                                            bFM_GPIO_PFR1_P6 = 1u; \
                                        }while (0u)

/*--- RTO22_0 ---*/
#define SetPinFunc_RTO22_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN05=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 4u, 2u,  1u ); \
                                            bFM_GPIO_PFR1_P5 = 1u; \
                                        }while (0u)

/*--- RTO23_0 ---*/
#define SetPinFunc_RTO23_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN16=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 6u, 2u,  1u ); \
                                            bFM_GPIO_PFR2_P2 = 1u; \
                                        }while (0u)

/*--- RTO24_0 ---*/
#define SetPinFunc_RTO24_0(dummy)       do{ \
                                            bFM_GPIO_ADE_AN17=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 8u, 2u,  1u ); \
                                            bFM_GPIO_PFR2_P1 = 1u; \
                                        }while (0u)

/*--- RTO25_1 ---*/
#define SetPinFunc_RTO25_1(dummy)       do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR03, 10u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_P0 = 1u; \
                                        }while (0u)

/*--- RTS4_0 ---*/
#define SetPinFunc_RTS4_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 0u, 2u,  1u ); \
                                            bFM_GPIO_PFR5_P1 = 1u; \
                                        }while (0u)

/*--- RTS4_2 ---*/
#define SetPinFunc_RTS4_2(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 0u, 2u,  3u ); \
                                            bFM_GPIO_PFR3_P0 = 1u; \
                                        }while (0u)

/*--- SCK0_0 ---*/
#define SetPinFunc_SCK0_0(dummy)        do{ \
                                            bFM_GPIO_ADE_AN15=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR07, 8u, 2u,  1u ); \
                                            bFM_GPIO_PFR2_P3 = 1u; \
                                        }while (0u)

/*--- SCK0_1 ---*/
#define SetPinFunc_SCK0_1(dummy)        do{ \
                                            bFM_GPIO_ADE_AN05=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR07, 8u, 2u,  2u ); \
                                            bFM_GPIO_PFR1_P5 = 1u; \
                                        }while (0u)

/*--- SCK1_1 ---*/
#define SetPinFunc_SCK1_1(dummy)        do{ \
                                            bFM_GPIO_ADE_AN02=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR07, 14u, 2u,  2u ); \
                                            bFM_GPIO_PFR1_P2 = 1u; \
                                        }while (0u)

/*--- SCK2_2 ---*/
#define SetPinFunc_SCK2_2(dummy)        do{ \
                                            bFM_GPIO_ADE_AN08=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR07, 20u, 2u,  3u ); \
                                            bFM_GPIO_PFR1_P8 = 1u; \
                                        }while (0u)

/*--- SCK3_1 ---*/
#define SetPinFunc_SCK3_1(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR07, 26u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_P3 = 1u; \
                                        }while (0u)

/*--- SCK4_0 ---*/
#define SetPinFunc_SCK4_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 8u, 2u,  1u ); \
                                            bFM_GPIO_PFR5_P2 = 1u; \
                                        }while (0u)

/*--- SCK4_1 ---*/
#define SetPinFunc_SCK4_1(dummy)        do{ \
                                            bFM_GPIO_ADE_AN11=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 8u, 2u,  2u ); \
                                            bFM_GPIO_PFR1_PB = 1u; \
                                        }while (0u)

/*--- SCK5_0 ---*/
#define SetPinFunc_SCK5_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 14u, 2u,  1u ); \
                                            bFM_GPIO_PFR6_P0 = 1u; \
                                        }while (0u)

/*--- SCK6_1 ---*/
#define SetPinFunc_SCK6_1(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 20u, 2u,  2u ); \
                                            bFM_GPIO_PFR0_PD = 1u; \
                                        }while (0u)

/*--- SCK7_1 ---*/
#define SetPinFunc_SCK7_1(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 26u, 2u,  2u ); \
                                            bFM_GPIO_PFR4_PC = 1u; \
                                        }while (0u)

/*--- SCS6_1 ---*/
#define SetPinFunc_SCS6_1(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR16, 0u, 2u,  2u ); \
                                            bFM_GPIO_PFR0_PE = 1u; \
                                        }while (0u)

/*--- SCS7_1 ---*/
#define SetPinFunc_SCS7_1(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR16, 2u, 2u,  2u ); \
                                            bFM_GPIO_PFR4_PB = 1u; \
                                        }while (0u)

/*--- SIN0_0 ---*/
#define SetPinFunc_SIN0_0(dummy)        do{ \
                                            bFM_GPIO_ADE_AN17=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR07, 4u, 2u,  1u ); \
                                            bFM_GPIO_PFR2_P1 = 1u; \
                                        }while (0u)

/*--- SIN0_1 ---*/
#define SetPinFunc_SIN0_1(dummy)        do{ \
                                            bFM_GPIO_ADE_AN03=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR07, 4u, 2u,  2u ); \
                                            bFM_GPIO_PFR1_P3 = 1u; \
                                        }while (0u)

/*--- SIN1_0 ---*/
#define SetPinFunc_SIN1_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR07, 10u, 2u,  1u ); \
                                            bFM_GPIO_PFR0_PA = 1u; \
                                        }while (0u)

/*--- SIN1_1 ---*/
#define SetPinFunc_SIN1_1(dummy)        do{ \
                                            bFM_GPIO_ADE_AN00=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR07, 10u, 2u,  2u ); \
                                            bFM_GPIO_PFR1_P0 = 1u; \
                                        }while (0u)

/*--- SIN2_2 ---*/
#define SetPinFunc_SIN2_2(dummy)        do{ \
                                            bFM_GPIO_ADE_AN06=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR07, 16u, 2u,  3u ); \
                                            bFM_GPIO_PFR1_P6 = 1u; \
                                        }while (0u)

/*--- SIN3_1 ---*/
#define SetPinFunc_SIN3_1(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR07, 22u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_P1 = 1u; \
                                        }while (0u)

/*--- SIN4_0 ---*/
#define SetPinFunc_SIN4_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 4u, 2u,  1u ); \
                                            bFM_GPIO_PFR5_P4 = 1u; \
                                        }while (0u)

/*--- SIN4_1 ---*/
#define SetPinFunc_SIN4_1(dummy)        do{ \
                                            bFM_GPIO_ADE_AN09=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 4u, 2u,  2u ); \
                                            bFM_GPIO_PFR1_P9 = 1u; \
                                        }while (0u)

/*--- SIN5_0 ---*/
#define SetPinFunc_SIN5_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 10u, 2u,  1u ); \
                                            bFM_GPIO_PFR6_P2 = 1u; \
                                        }while (0u)

/*--- SIN6_0 ---*/
#define SetPinFunc_SIN6_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 16u, 2u,  1u ); \
                                            bFM_GPIO_PFR5_P5 = 1u; \
                                        }while (0u)

/*--- SIN6_1 ---*/
#define SetPinFunc_SIN6_1(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 16u, 2u,  2u ); \
                                            bFM_GPIO_PFR0_PB = 1u; \
                                        }while (0u)

/*--- SIN7_1 ---*/
#define SetPinFunc_SIN7_1(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 22u, 2u,  2u ); \
                                            bFM_GPIO_PFR4_PE = 1u; \
                                        }while (0u)

/*--- SOT0_0 ---*/
#define SetPinFunc_SOT0_0(dummy)        do{ \
                                            bFM_GPIO_ADE_AN16=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR07, 6u, 2u,  1u ); \
                                            bFM_GPIO_PFR2_P2 = 1u; \
                                        }while (0u)

/*--- SOT0_1 ---*/
#define SetPinFunc_SOT0_1(dummy)        do{ \
                                            bFM_GPIO_ADE_AN04=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR07, 6u, 2u,  2u ); \
                                            bFM_GPIO_PFR1_P4 = 1u; \
                                        }while (0u)

/*--- SOT1_0 ---*/
#define SetPinFunc_SOT1_0(dummy)        do{ \
                                            bFM_GPIO_ADE_AN19=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR07, 12u, 2u,  1u ); \
                                            bFM_GPIO_PFR0_P9 = 1u; \
                                        }while (0u)

/*--- SOT1_1 ---*/
#define SetPinFunc_SOT1_1(dummy)        do{ \
                                            bFM_GPIO_ADE_AN01=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR07, 12u, 2u,  2u ); \
                                            bFM_GPIO_PFR1_P1 = 1u; \
                                        }while (0u)

/*--- SOT2_2 ---*/
#define SetPinFunc_SOT2_2(dummy)        do{ \
                                            bFM_GPIO_ADE_AN07=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR07, 18u, 2u,  3u ); \
                                            bFM_GPIO_PFR1_P7 = 1u; \
                                        }while (0u)

/*--- SOT3_1 ---*/
#define SetPinFunc_SOT3_1(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR07, 24u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_P2 = 1u; \
                                        }while (0u)

/*--- SOT4_0 ---*/
#define SetPinFunc_SOT4_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 6u, 2u,  1u ); \
                                            bFM_GPIO_PFR5_P3 = 1u; \
                                        }while (0u)

/*--- SOT4_1 ---*/
#define SetPinFunc_SOT4_1(dummy)        do{ \
                                            bFM_GPIO_ADE_AN10=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 6u, 2u,  2u ); \
                                            bFM_GPIO_PFR1_PA = 1u; \
                                        }while (0u)

/*--- SOT5_0 ---*/
#define SetPinFunc_SOT5_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 12u, 2u,  1u ); \
                                            bFM_GPIO_PFR6_P1 = 1u; \
                                        }while (0u)

/*--- SOT6_0 ---*/
#define SetPinFunc_SOT6_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 18u, 2u,  1u ); \
                                            bFM_GPIO_PFR5_P6 = 1u; \
                                        }while (0u)

/*--- SOT6_1 ---*/
#define SetPinFunc_SOT6_1(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 18u, 2u,  2u ); \
                                            bFM_GPIO_PFR0_PC = 1u; \
                                        }while (0u)

/*--- SOT7_1 ---*/
#define SetPinFunc_SOT7_1(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR08, 24u, 2u,  2u ); \
                                            bFM_GPIO_PFR4_PD = 1u; \
                                        }while (0u)

/*--- SUBOUT_0 ---*/
#define SetPinFunc_SUBOUT_0(dummy)      do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 6u, 2u,  1u ); \
                                            bFM_GPIO_PFR6_P1 = 1u; \
                                        }while (0u)

/*--- SUBOUT_1 ---*/
#define SetPinFunc_SUBOUT_1(dummy)      do{ \
                                            bFM_GPIO_ADE_AN02=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 6u, 2u,  2u ); \
                                            bFM_GPIO_PFR1_P2 = 1u; \
                                        }while (0u)

/*--- SUBOUT_2 ---*/
#define SetPinFunc_SUBOUT_2(dummy)      do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 6u, 2u,  3u ); \
                                            bFM_GPIO_PFR3_P9 = 1u; \
                                        }while (0u)

/*--- SWCLK ---*/
#define SetPinFunc_SWCLK(dummy)         do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 16u, 1u,  1u ); \
                                            bFM_GPIO_PFR0_P1 = 1u; \
                                        }while (0u)

/*--- SWDIO ---*/
#define SetPinFunc_SWDIO(dummy)         do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 16u, 1u,  1u ); \
                                            bFM_GPIO_PFR0_P3 = 1u; \
                                        }while (0u)

/*--- SWO ---*/
#define SetPinFunc_SWO(dummy)           do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 16u, 1u,  1u ); \
                                            bFM_GPIO_PFR0_P4 = 1u; \
                                        }while (0u)

/*--- TCK ---*/
#define SetPinFunc_TCK(dummy)           do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 16u, 1u,  1u ); \
                                            bFM_GPIO_PFR0_P1 = 1u; \
                                        }while (0u)

/*--- TDI ---*/
#define SetPinFunc_TDI(dummy)           do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 17u, 1u,  1u ); \
                                            bFM_GPIO_PFR0_P2 = 1u; \
                                        }while (0u)

/*--- TDO ---*/
#define SetPinFunc_TDO(dummy)           do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 16u, 1u,  1u ); \
                                            bFM_GPIO_PFR0_P4 = 1u; \
                                        }while (0u)

/*--- TIOA0_1_OUT ---*/
#define SetPinFunc_TIOA0_1_OUT(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 2u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_PA = 1u; \
                                        }while (0u)

/*--- TIOA1_1_IN ---*/
#define SetPinFunc_TIOA1_1_IN(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 8u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_PB = 1u; \
                                        }while (0u)

/*--- TIOA1_1_OUT ---*/
#define SetPinFunc_TIOA1_1_OUT(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 10u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_PB = 1u; \
                                        }while (0u)

/*--- TIOA1_2_IN ---*/
#define SetPinFunc_TIOA1_2_IN(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 8u, 2u,  3u ); \
                                            bFM_GPIO_PFR5_P3 = 1u; \
                                        }while (0u)

/*--- TIOA1_2_OUT ---*/
#define SetPinFunc_TIOA1_2_OUT(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 10u, 2u,  3u ); \
                                            bFM_GPIO_PFR5_P3 = 1u; \
                                        }while (0u)

/*--- TIOA2_1_OUT ---*/
#define SetPinFunc_TIOA2_1_OUT(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 18u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_PC = 1u; \
                                        }while (0u)

/*--- TIOA2_2_OUT ---*/
#define SetPinFunc_TIOA2_2_OUT(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 18u, 2u,  3u ); \
                                            bFM_GPIO_PFR6_P0 = 1u; \
                                        }while (0u)

/*--- TIOA3_1_IN ---*/
#define SetPinFunc_TIOA3_1_IN(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 24u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_PD = 1u; \
                                        }while (0u)

/*--- TIOA3_1_OUT ---*/
#define SetPinFunc_TIOA3_1_OUT(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 26u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_PD = 1u; \
                                        }while (0u)

/*--- TIOA3_2_IN ---*/
#define SetPinFunc_TIOA3_2_IN(dummy)    do{ \
                                            bFM_GPIO_ADE_AN19=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 24u, 2u,  3u ); \
                                            bFM_GPIO_PFR0_P9 = 1u; \
                                        }while (0u)

/*--- TIOA3_2_OUT ---*/
#define SetPinFunc_TIOA3_2_OUT(dummy)   do{ \
                                            bFM_GPIO_ADE_AN19=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 26u, 2u,  3u ); \
                                            bFM_GPIO_PFR0_P9 = 1u; \
                                        }while (0u)

/*--- TIOA4_0_OUT ---*/
#define SetPinFunc_TIOA4_0_OUT(dummy)   do{ \
                                            bFM_DAC0_DACR_DAE=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR05, 2u, 2u,  1u ); \
                                            bFM_GPIO_PFR4_P4 = 1u; \
                                        }while (0u)

/*--- TIOA4_1_OUT ---*/
#define SetPinFunc_TIOA4_1_OUT(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR05, 2u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_PE = 1u; \
                                        }while (0u)

/*--- TIOA5_1_IN ---*/
#define SetPinFunc_TIOA5_1_IN(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR05, 8u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_PF = 1u; \
                                        }while (0u)

/*--- TIOA5_1_OUT ---*/
#define SetPinFunc_TIOA5_1_OUT(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR05, 10u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_PF = 1u; \
                                        }while (0u)

/*--- TIOA5_2_IN ---*/
#define SetPinFunc_TIOA5_2_IN(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR05, 8u, 2u,  3u ); \
                                            bFM_GPIO_PFR0_PD = 1u; \
                                        }while (0u)

/*--- TIOA5_2_OUT ---*/
#define SetPinFunc_TIOA5_2_OUT(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR05, 10u, 2u,  3u ); \
                                            bFM_GPIO_PFR0_PD = 1u; \
                                        }while (0u)

/*--- TIOA6_1_OUT ---*/
#define SetPinFunc_TIOA6_1_OUT(dummy)   do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR05, 18u, 2u,  2u ); \
                                            bFM_GPIO_PFR0_PC = 1u; \
                                        }while (0u)

/*--- TIOA7_1_IN ---*/
#define SetPinFunc_TIOA7_1_IN(dummy)    do{ \
                                            bFM_GPIO_ADE_AN15=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR05, 24u, 2u,  2u ); \
                                            bFM_GPIO_PFR2_P3 = 1u; \
                                        }while (0u)

/*--- TIOA7_1_OUT ---*/
#define SetPinFunc_TIOA7_1_OUT(dummy)   do{ \
                                            bFM_GPIO_ADE_AN15=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR05, 26u, 2u,  2u ); \
                                            bFM_GPIO_PFR2_P3 = 1u; \
                                        }while (0u)

/*--- TIOB0_0_IN ---*/
#define SetPinFunc_TIOB0_0_IN(dummy)    do{ \
                                            bFM_DAC1_DACR_DAE=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 4u, 3u,  1u ); \
                                            bFM_GPIO_PFR4_P5 = 1u; \
                                        }while (0u)

/*--- TIOB0_1_IN ---*/
#define SetPinFunc_TIOB0_1_IN(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 4u, 3u,  2u ); \
                                            bFM_GPIO_PFR3_P0 = 1u; \
                                        }while (0u)

/*--- TIOB1_0_IN ---*/
#define SetPinFunc_TIOB1_0_IN(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 12u, 2u,  1u ); \
                                            bFM_GPIO_PFR4_PB = 1u; \
                                        }while (0u)

/*--- TIOB1_1_IN ---*/
#define SetPinFunc_TIOB1_1_IN(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 12u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_P1 = 1u; \
                                        }while (0u)

/*--- TIOB1_2_IN ---*/
#define SetPinFunc_TIOB1_2_IN(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 12u, 2u,  3u ); \
                                            bFM_GPIO_PFR5_P4 = 1u; \
                                        }while (0u)

/*--- TIOB2_0_IN ---*/
#define SetPinFunc_TIOB2_0_IN(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 20u, 2u,  1u ); \
                                            bFM_GPIO_PFR4_PC = 1u; \
                                        }while (0u)

/*--- TIOB2_1_IN ---*/
#define SetPinFunc_TIOB2_1_IN(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 20u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_P2 = 1u; \
                                        }while (0u)

/*--- TIOB2_2_IN ---*/
#define SetPinFunc_TIOB2_2_IN(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 20u, 2u,  3u ); \
                                            bFM_GPIO_PFR6_P1 = 1u; \
                                        }while (0u)

/*--- TIOB3_0_IN ---*/
#define SetPinFunc_TIOB3_0_IN(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 28u, 2u,  1u ); \
                                            bFM_GPIO_PFR4_PD = 1u; \
                                        }while (0u)

/*--- TIOB3_1_IN ---*/
#define SetPinFunc_TIOB3_1_IN(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR04, 28u, 2u,  2u ); \
                                            bFM_GPIO_PFR3_P3 = 1u; \
                                        }while (0u)

/*--- TIOB4_0_IN ---*/
#define SetPinFunc_TIOB4_0_IN(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR05, 4u, 2u,  1u ); \
                                            bFM_GPIO_PFR4_PE = 1u; \
                                        }while (0u)

/*--- TIOB5_2_IN ---*/
#define SetPinFunc_TIOB5_2_IN(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR05, 12u, 2u,  3u ); \
                                            bFM_GPIO_PFR0_PE = 1u; \
                                        }while (0u)

/*--- TIOB6_1_IN ---*/
#define SetPinFunc_TIOB6_1_IN(dummy)    do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR05, 20u, 2u,  2u ); \
                                            bFM_GPIO_PFR0_PB = 1u; \
                                        }while (0u)

/*--- TIOB7_1_IN ---*/
#define SetPinFunc_TIOB7_1_IN(dummy)    do{ \
                                            bFM_GPIO_ADE_AN16=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR05, 28u, 2u,  2u ); \
                                            bFM_GPIO_PFR2_P2 = 1u; \
                                        }while (0u)

/*--- TMS ---*/
#define SetPinFunc_TMS(dummy)           do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 16u, 1u,  1u ); \
                                            bFM_GPIO_PFR0_P3 = 1u; \
                                        }while (0u)

/*--- TRSTX ---*/
#define SetPinFunc_TRSTX(dummy)         do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 17u, 1u,  1u ); \
                                            bFM_GPIO_PFR0_P0 = 1u; \
                                        }while (0u)

/*--- VREGCTL ---*/
#define SetPinFunc_VREGCTL(dummy)       do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBPFR_VPFR0=0u; \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while (0u)

/*--- VWAKEUP ---*/
#define SetPinFunc_VWAKEUP(dummy)       do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            bFM_RTC_VBPFR_VPFR1=0u; \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while (0u)

/*--- WKUP0 ---*/
#define SetPinFunc_WKUP0(dummy)         do{ \
                                            bFM_GPIO_PFR6_P0 = 1u; \
                                        }while (0u)

/*--- WKUP1 ---*/
#define SetPinFunc_WKUP1(dummy)         do{ \
                                            bFM_GPIO_PFR3_P0 = 1u; \
                                        }while (0u)

/*--- WKUP2 ---*/
#define SetPinFunc_WKUP2(dummy)         do{ \
                                            bFM_GPIO_PFR4_PE = 1u; \
                                        }while (0u)

/*--- WKUP3 ---*/
#define SetPinFunc_WKUP3(dummy)         do{ \
                                            bFM_GPIO_ADE_AN07=0u; \
                                            bFM_GPIO_PFR1_P7 = 1u; \
                                        }while (0u)

/*--- X0 ---*/
#define SetPinFunc_X0(dummy)            do{ \
                                            PINCONFIG_SET_REG(FM_GPIO->SPSR, 2u, 2u, 1u); \
                                        }while (0u)

/*--- X0A ---*/
#define SetPinFunc_X0A(dummy)           do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            PINCONFIG_SET_REG(FM_RTC->VBPFR, 4u, 2u, 0x00u); \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while (0u)

/*--- X1 ---*/
#define SetPinFunc_X1(dummy)            do{ \
                                            PINCONFIG_SET_REG(FM_GPIO->SPSR, 2u, 2u, 1u); \
                                        }while (0u)

/*--- X1A ---*/
#define SetPinFunc_X1A(dummy)           do{ \
                                            bFM_RTC_VDET_PON = 0u; \
                                            FM_RTC->WTCR20_f.PREAD = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                            PINCONFIG_SET_REG(FM_RTC->VBPFR, 4u, 2u, 0x00u); \
                                            FM_RTC->WTCR20_f.PWRITE = 1u; \
                                            while(0u != FM_RTC->WTCR10_f.TRANS){} \
                                        }while (0u)

/*--- ZIN0_0 ---*/
#define SetPinFunc_ZIN0_0(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 4u, 2u,  1u ); \
                                            bFM_GPIO_PFR3_PC = 1u; \
                                        }while (0u)

/*--- ZIN0_2 ---*/
#define SetPinFunc_ZIN0_2(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 4u, 2u,  3u ); \
                                            bFM_GPIO_PFR5_P2 = 1u; \
                                        }while (0u)

/*--- ZIN1_2 ---*/
#define SetPinFunc_ZIN1_2(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR09, 10u, 2u,  3u ); \
                                            bFM_GPIO_PFR4_PE = 1u; \
                                        }while (0u)

/*--- ZIN2_1 ---*/
#define SetPinFunc_ZIN2_1(dummy)        do{ \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR14, 4u, 2u,  2u ); \
                                            bFM_GPIO_PFR6_P1 = 1u; \
                                        }while (0u)

/*--- ZIN2_2 ---*/
#define SetPinFunc_ZIN2_2(dummy)        do{ \
                                            bFM_GPIO_ADE_AN05=0u; \
                                            PINRELOC_SET_EPFR( FM_GPIO->EPFR14, 4u, 2u,  3u ); \
                                            bFM_GPIO_PFR1_P5 = 1u; \
                                        }while (0u)

/******************************************************************************
   ANALOG PINS
*******************************************************************************/

/*--- AN00 ---*/
#define SetPinFunc_AN00(dummy)          do{ \
                                            bFM_GPIO_ADE_AN00=1u; \
                                        }while (0u)

/*--- AN01 ---*/
#define SetPinFunc_AN01(dummy)          do{ \
                                            bFM_GPIO_ADE_AN01=1u; \
                                        }while (0u)

/*--- AN02 ---*/
#define SetPinFunc_AN02(dummy)          do{ \
                                            bFM_GPIO_ADE_AN02=1u; \
                                        }while (0u)

/*--- AN03 ---*/
#define SetPinFunc_AN03(dummy)          do{ \
                                            bFM_GPIO_ADE_AN03=1u; \
                                        }while (0u)

/*--- AN04 ---*/
#define SetPinFunc_AN04(dummy)          do{ \
                                            bFM_GPIO_ADE_AN04=1u; \
                                        }while (0u)

/*--- AN05 ---*/
#define SetPinFunc_AN05(dummy)          do{ \
                                            bFM_GPIO_ADE_AN05=1u; \
                                        }while (0u)

/*--- AN06 ---*/
#define SetPinFunc_AN06(dummy)          do{ \
                                            bFM_GPIO_ADE_AN06=1u; \
                                        }while (0u)

/*--- AN07 ---*/
#define SetPinFunc_AN07(dummy)          do{ \
                                            bFM_GPIO_ADE_AN07=1u; \
                                        }while (0u)

/*--- AN08 ---*/
#define SetPinFunc_AN08(dummy)          do{ \
                                            bFM_GPIO_ADE_AN08=1u; \
                                        }while (0u)

/*--- AN09 ---*/
#define SetPinFunc_AN09(dummy)          do{ \
                                            bFM_GPIO_ADE_AN09=1u; \
                                        }while (0u)

/*--- AN10 ---*/
#define SetPinFunc_AN10(dummy)          do{ \
                                            bFM_GPIO_ADE_AN10=1u; \
                                        }while (0u)

/*--- AN11 ---*/
#define SetPinFunc_AN11(dummy)          do{ \
                                            bFM_GPIO_ADE_AN11=1u; \
                                        }while (0u)

/*--- AN15 ---*/
#define SetPinFunc_AN15(dummy)          do{ \
                                            bFM_GPIO_ADE_AN15=1u; \
                                        }while (0u)

/*--- AN16 ---*/
#define SetPinFunc_AN16(dummy)          do{ \
                                            bFM_GPIO_ADE_AN16=1u; \
                                        }while (0u)

/*--- AN17 ---*/
#define SetPinFunc_AN17(dummy)          do{ \
                                            bFM_GPIO_ADE_AN17=1u; \
                                        }while (0u)

/*--- AN19 ---*/
#define SetPinFunc_AN19(dummy)          do{ \
                                            bFM_GPIO_ADE_AN19=1u; \
                                        }while (0u)

#endif // #ifndef __GPIO_S6E2H1XE_H__


/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/

