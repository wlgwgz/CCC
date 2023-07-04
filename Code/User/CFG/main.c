/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "mcu.h"
#include "includes.h"
extern void MCU_Config(void);
int main(void)
{
    /* Place your initialization/startup code here (e.g. Drv_Init()) */
    MCU_Config();
    HDW_Init();
    USER_Init();

    
    for(;;)
    {
    	WDG_Refresh();
        /* Place your application code here. */
        HDW_Main(); 
        USER_Main();	
    }
}


/* [] END OF FILE */
