/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: EEPROM.H
 �ļ�����: 
 ������  : 
 ��������: 
*******************************************************************************/
#ifndef EEMAP_H
#define EEMAP_H

#ifdef  EEMAP_GLOBAL
    #define EEMAP_EXT
#else
    #define EEMAP_EXT extern
#endif


//#define EEPROMADDR_RESET_CNT        0x0000//��λ�����洢��ַ3+2
#define EEPROMADDR_ADC_CALIBRATE    0x0002  // Adc У׼ֵ4+2
#define EEPROMADDR_ADJUST_AI        0x000A//AIУ׼��������ֵ��4*4*3+2 = 50��������ַ0x31


#endif

