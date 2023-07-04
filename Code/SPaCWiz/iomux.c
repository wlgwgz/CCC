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
/** \file iomux.c
 **
 ** A detailed description is available at 
 ** @link IomuxGroup IOMUX Module description @endlink
 **
 ** History:
 **   - 2021-06-11  1.0  Pin And Code Wizard  Generated code.
 **
 ******************************************************************************/

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "iomux.h"
#include "gpio.h"

/**
 ******************************************************************************
 ** \addtogroup IomuxGroup
 ******************************************************************************/
//@{

/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/

/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/

/******************************************************************************/
/* Local function prototypes ('static')                                       */
/******************************************************************************/
static void Iomux_GPIO_Init(void);
static void Iomux_GPIO_DeInit(void);

static void Iomux_CLOCK_Init(void);
static void Iomux_CLOCK_DeInit(void);

static void Iomux_DEBUG_Init(void);
static void Iomux_DEBUG_DeInit(void);

static void Iomux_MFS_Init(void);
static void Iomux_MFS_DeInit(void);

static void Iomux_BT_Init(void);
static void Iomux_BT_DeInit(void);

static void Iomux_ADC_Init(void);
static void Iomux_ADC_DeInit(void);


/******************************************************************************/
/* Local variable definitions ('static')                                      */
/******************************************************************************/

/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/
/**
 *****************************************************************************
 ** \brief This function initializes GPIO
 **
 *****************************************************************************/
static void Iomux_GPIO_Init(void)
{
	// Init GPIO pin function
	// I2C
	Gpio1pin_InitOut( GPIO1PIN_P32, Gpio1pin_InitVal( 0u ) );
	Gpio1pin_InitOut( GPIO1PIN_P33, Gpio1pin_InitVal( 0u ) );

	// DO
	Gpio1pin_InitOut( GPIO1PIN_P31, Gpio1pin_InitVal( 0u ) );
	Gpio1pin_InitOut( GPIO1PIN_P4B, Gpio1pin_InitVal( 0u ) );
	Gpio1pin_InitOut( GPIO1PIN_P49, Gpio1pin_InitVal( 0u ) );
	
	Gpio1pin_InitOut( GPIO1PIN_P48, Gpio1pin_InitVal( 0u ) );
	Gpio1pin_InitOut( GPIO1PIN_P47, Gpio1pin_InitVal( 0u ) );	
	Gpio1pin_InitOut( GPIO1PIN_P46, Gpio1pin_InitVal( 0u ) );
	Gpio1pin_InitOut( GPIO1PIN_P45, Gpio1pin_InitVal( 0u ) );
	Gpio1pin_InitOut( GPIO1PIN_P44, Gpio1pin_InitVal( 0u ) );
	Gpio1pin_InitOut( GPIO1PIN_P39, Gpio1pin_InitVal( 0u ) );
	Gpio1pin_InitOut( GPIO1PIN_P3A, Gpio1pin_InitVal( 0u ) );
	Gpio1pin_InitOut( GPIO1PIN_P3B, Gpio1pin_InitVal( 0u ) );
	Gpio1pin_InitOut( GPIO1PIN_P3C, Gpio1pin_InitVal( 0u ) );
	Gpio1pin_InitOut( GPIO1PIN_P3D, Gpio1pin_InitVal( 0u ) );
	Gpio1pin_InitOut( GPIO1PIN_P3E, Gpio1pin_InitVal( 0u ) );
	Gpio1pin_InitOut( GPIO1PIN_P3F, Gpio1pin_InitVal( 0u ) );
	Gpio1pin_InitOut( GPIO1PIN_P4C, Gpio1pin_InitVal( 0u ) );
	Gpio1pin_InitOut( GPIO1PIN_P4D, Gpio1pin_InitVal( 0u ) );
	Gpio1pin_InitOut( GPIO1PIN_P4E, Gpio1pin_InitVal( 0u ) );

	// HDI
	Gpio1pin_InitIn( GPIO1PIN_P30, Gpio1pin_InitPullup( 0u ) );		/* D_HIN1 */
	Gpio1pin_InitIn( GPIO1PIN_P51, Gpio1pin_InitPullup( 0u ) );		/* D_HIN3 */
	Gpio1pin_InitIn( GPIO1PIN_P52, Gpio1pin_InitPullup( 0u ) );		/* D_HIN2 */
//	Gpio1pin_InitIn( GPIO1PIN_P0D, Gpio1pin_InitPullup( 0u ) );
	Gpio1pin_InitIn( GPIO1PIN_P0C, Gpio1pin_InitPullup( 0u ) );		/* D_HIN4 */
	Gpio1pin_InitIn( GPIO1PIN_P23, Gpio1pin_InitPullup( 0u ) );		/* D_HIN5 */
	
	// UX-485
	Gpio1pin_InitOut( GPIO1PIN_P50, Gpio1pin_InitVal( 0u ) );
	Gpio1pin_InitOut( GPIO1PIN_P60, Gpio1pin_InitVal( 0u ) );
	Gpio1pin_InitOut( GPIO1PIN_P80, Gpio1pin_InitVal( 0u ) );
	Gpio1pin_InitOut( GPIO1PIN_P81, Gpio1pin_InitVal( 0u ) );

    Gpio1pin_InitIn( GPIO1PIN_P0B, Gpio1pin_InitPullup( 0u ) );
	Gpio1pin_InitIn( GPIO1PIN_P0E, Gpio1pin_InitPullup( 0u ) );

	// 74HC165
//	Gpio1pin_InitOut( GPIO1PIN_P0C, Gpio1pin_InitVal( 0u ) );
    Gpio1pin_InitOut( GPIO1PIN_P1A, Gpio1pin_InitVal( 0u ) );		/* SH/LD */
//	Gpio1pin_InitOut( GPIO1PIN_P0A, Gpio1pin_InitVal( 0u ) );
    Gpio1pin_InitOut( GPIO1PIN_P19, Gpio1pin_InitVal( 0u ) );		/* CLK */
//	Gpio1pin_InitIn( GPIO1PIN_P63, Gpio1pin_InitPullup( 0u ) );
    Gpio1pin_InitIn( GPIO1PIN_P1B, Gpio1pin_InitPullup( 0u ) );		/* INPUT */
	
	// LED yellow red
	Gpio1pin_InitIn( GPIO1PIN_P02, Gpio1pin_InitPullup( 0u ) );
//	Gpio1pin_InitIn( GPIO1PIN_P00, Gpio1pin_InitPullup( 0u ) );

	// 4G
	Gpio1pin_InitOut( GPIO1PIN_P00, Gpio1pin_InitVal( 0u ) );		/* 4G_RST */
	Gpio1pin_InitOut( GPIO1PIN_P63, Gpio1pin_InitVal( 0u ) );		/* 4G_POWER */

    // 4051
    Gpio1pin_InitOut( GPIO1PIN_P14, Gpio1pin_InitVal( 0u ) );
    Gpio1pin_InitOut( GPIO1PIN_P15, Gpio1pin_InitVal( 0u ) );
	Gpio1pin_InitOut( GPIO1PIN_P16, Gpio1pin_InitVal( 0u ) );
}

/**
 *****************************************************************************
 ** \brief This function DeInitializes GPIO
 **
 *****************************************************************************/
static void Iomux_GPIO_DeInit(void)
{
	// DeInit GPIO pin function
}



/**
 *****************************************************************************
 ** \brief This function initializes CLOCK
 **
 *****************************************************************************/
static void Iomux_CLOCK_Init(void)
{
	// Init CLOCK pin function
	SetPinFunc_X0();
	SetPinFunc_X1();
}

/**
 *****************************************************************************
 ** \brief This function DeInitializes CLOCK
 **
 *****************************************************************************/
static void Iomux_CLOCK_DeInit(void)
{
	// DeInit CLOCK pin function
}


/**
 *****************************************************************************
 ** \brief This function initializes DEBUG
 **
 *****************************************************************************/
static void Iomux_DEBUG_Init(void)
{
	// Init DEBUG pin function
	PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 16u, 1u, 0x1u );
	bFM_GPIO_PFR0_P4 = 1u;
	PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 16u, 1u, 0x1u );
	bFM_GPIO_PFR0_P3 = 1u;
	PINRELOC_SET_EPFR( FM_GPIO->EPFR00, 16u, 1u, 0x1u );
	bFM_GPIO_PFR0_P1 = 1u;
}

/**
 *****************************************************************************
 ** \brief This function DeInitializes DEBUG
 **
 *****************************************************************************/
static void Iomux_DEBUG_DeInit(void)
{
	// DeInit DEBUG pin function
}


/**
 *****************************************************************************
 ** \brief This function initializes MFS
 **
 *****************************************************************************/
static void Iomux_MFS_Init(void)
{
	// Init MFS pin function
	SetPinFunc_SOT0_0();
	SetPinFunc_SIN0_0();
	SetPinFunc_SOT4_0();
	SetPinFunc_SIN4_0();
	SetPinFunc_SIN6_0();
	SetPinFunc_SOT6_0();
	SetPinFunc_SIN1_0();
	SetPinFunc_SOT1_0();	/* ´®¿Ú */
	SetPinFunc_SIN5_0();
	SetPinFunc_SOT5_0();
}

/**
 *****************************************************************************
 ** \brief This function DeInitializes MFS
 **
 *****************************************************************************/
static void Iomux_MFS_DeInit(void)
{
	// DeInit MFS pin function
}


/**
 *****************************************************************************
 ** \brief This function initializes BT
 **
 *****************************************************************************/
static void Iomux_BT_Init(void)
{
	// Init BT pin function
	//SetPinFunc_TIOB1_1_IN();
	//SetPinFunc_TIOA7_1_OUT();
	//SetPinFunc_TIOB0_1_IN();
	//SetPinFunc_TIOA5_2_OUT();
//	SetPinFunc_TIOB6_1_IN();
	
//	SetPinFunc_TIOA3_2_OUT();
	SetPinFunc_TIOA5_2_OUT();				/* PWM_INPUT */
}

/**
 *****************************************************************************
 ** \brief This function DeInitializes BT
 **
 *****************************************************************************/
static void Iomux_BT_DeInit(void)
{
	// DeInit BT pin function
}


/**
 *****************************************************************************
 ** \brief This function initializes ADC
 **
 *****************************************************************************/
static void Iomux_ADC_Init(void)
{
	// Init ADC pin function
	SetPinFunc_AN00();		/* NTC_1 */
	SetPinFunc_AN01();		/* NTC_2 */
	SetPinFunc_AN02();		/* NTC_3 */
	SetPinFunc_AN03();		/* NTC_4 */
//	SetPinFunc_AN04();
//	SetPinFunc_AN05();
//	SetPinFunc_AN06();
	SetPinFunc_AN07();		/* NTC */
//	SetPinFunc_AN08();
//	SetPinFunc_AN09();
//	SetPinFunc_AN10();
//	SetPinFunc_AN11();
	
}

/**
 *****************************************************************************
 ** \brief This function DeInitializes ADC
 **
 *****************************************************************************/
static void Iomux_ADC_DeInit(void)
{
	// DeInit ADC pin function
}

/**
 *****************************************************************************
 ** \brief This function initializes the GPIO ports and peripheral pin
 **        functions according to the user settings of the Pin Wizard.
 **
 *****************************************************************************/
void Iomux_Init(void)
{
	Iomux_GPIO_Init();

	Iomux_CLOCK_Init();

	Iomux_DEBUG_Init();

	Iomux_MFS_Init();

	Iomux_BT_Init();

	Iomux_ADC_Init();
}

/**
 *****************************************************************************
 ** \brief This function de-initializes the GPIO ports and peripheral pin
 **        functions according to the user settings of the Pin Wizard to the
 **        default values.
 **
 *****************************************************************************/
void Iomux_DeInit(void)
{
	Iomux_GPIO_DeInit();

	Iomux_CLOCK_DeInit();

	Iomux_DEBUG_DeInit();

	Iomux_MFS_DeInit();

	Iomux_BT_DeInit();

	Iomux_ADC_DeInit();
}

//@} // IomuxGroup

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
