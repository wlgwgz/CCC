/*******************************************************************************
* Copyright (C) 2013-2015, Cypress Semiconductor Corporation. All rights reserved.
*
* This software, including source code, documentation and related
* materials "Software", is owned by Cypress Semiconductor
* Corporation "Cypress" and is protected by and subject to worldwide
* patent protection (United States and foreign), United States copyright
* laws and international treaty provisions. Therefore, you may use this
* Software only as provided in the license agreement accompanying the
* software package from which you obtained this Software "EULA".
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
* significant property damage, injury or death "High Risk Product". By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*/
/******************************************************************************/
/** \file iomux.h
 **
 ** Headerfile for IOMUX functions
 **  
 ** History:
 **   - 2021-06-11  1.0  Pin And Code Wizard  Generated code.
 **
 ******************************************************************************/

#ifndef __IOMUX_H__
#define __IOMUX_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "mcu.h"
/*
#include "pdl.h"
*/

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
 ******************************************************************************
 ** \defgroup IomuxGroup IoMux Functions (IOMUX)
 **
 ** Provided functions of IOMUX module:
 ** 
 ** - Iomux_Init()
 ** - Iomux_DeInit()
 **
 ** Iomux_Init() initializes the GPIO ports and peripheral pin functions
 ** according to the user settings of the Pin Wizard.
 ** Iomux_DeInit() de-initializes the GPIO ports and peripheral pin functions
 ** according to the user settings of the Pin Wizard to the default values.
 **
 ******************************************************************************/
//@{

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/

/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/

/******************************************************************************/
/* Global variable definitions ('extern')                                     */
/******************************************************************************/

/******************************************************************************/
/* Global function prototypes (definition in C source)                        */
/******************************************************************************/
extern void Iomux_Init(void);
extern void Iomux_DeInit(void);

//@} // IomuxGroup

#ifdef __cplusplus
}
#endif

#endif /* __IOMUX_H__ */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
