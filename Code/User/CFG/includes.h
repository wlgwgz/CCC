
#ifndef _INCLUDES_H_
#define _INCLUDES_H_

#include "AppInfo.h"

/* 底层封闭定义 */
/* DAvE */
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "pdl_header.h"


//#include "IO.h"
//#include "CC2.h"
//#include "ADC0.h"
//#include "ADC1.h"
//#include "USIC0.h"
//#include "USIC1.h"
//#include "USIC2.h"

#include "Types.h"
//#include "MAIN.h"

/* MCU */
#include "Mcu_ADC.h"
#include "Mcu_def.h"
#include "Mcu_GPIO.h"
#include "Mcu_UART.h"
#include "Mcu_Timer.h"
#include "Mcu_Interrupt.h"

/* Hardware */
//#include "para.h"
#include "Hdw_Main.h"
#include "Hdw_Adc.h"
#include "Hdw_Timer.h"
#include "Hdw_Ai.h"
#include "Hdw_Curr.h"
#include "Hdw_Eeprom.h"
#include "Hdw_IO.h"
#include "Hdw_Temp.h"

/* LIB */
#include "Signal.h"
#include "Kit_ParaFac.h"

/* 逻辑应用 */
#include "HL051A.h"
#include "EXIO_HL055C.h"

#include "Debug.h"
#include "com_master.h"
#include "User_io.h"
#include "EepromMap.h"
#include "App_EcFan.h"
#include "fanCoilUnit.h"
#include "inverter.h"
#include "pid.h"
#include "eev.h"
#include "exio_klv.h"
#include "Logic.h"

#include "uart.h"
#include "Modbus.h"
#include "lcd.h"

#include "Test.h"
#include "Test_fac.h"
#include "CRC16.h"
#include "App_4g.h"

/* User */
#include "common.h"
#include "err_mod.h"
#include "err_sys.h"
#include "init.h"
#include "log_mod.h"
#include "log_sys.h"
#include "on_off.h"
#include "para.h"

#include "User_main.h"
#include "App_Defrost.h"

#include "pumpf.h"

/* HMI */
#include "AACH16.h"
#include "APPBMP.h"
#include "hmi.h"
#include "HmiRoutine.h"
#include "index.h"
#include "appstr.h"
#include "aach_b.h"

/* SmartHMI */
#include "SmartHmi_commu_info.h"
#include "SmartHmi.h"
#include "ExStr.h" 
#include "hzk2gbk.h"
#include "hzk2uni.h"
#include "str_version.h"
#include "Tuya_B.h"

#include "Fun.h"
#include "LOGGING.h"
#include "Work_frequency.h"
#include "Weather_Curve.h"
#include "Power_Usage.h"

#include "config.h"
#endif
