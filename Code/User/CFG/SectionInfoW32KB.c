
/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: SectionInfoW32KB.c
 �ļ�����: �ζ�����Ϣ,������Ϣ��λ��Work Flash 32KB��ʼλ��,���ڸ�֪BOOT���ָö�����
           ��256KB����Work Flash��32KB��
           ����: S6E2H1xϵ��Flash��ΪMain Flash(256KB/512KB)��Work Flash(32KB)
                 ��������plcx�ļ��Ὣ������ƴ��,���BOOT�޷����ָ�256KB��ǰ32KB
                 ������512KB��256KB��ǰ32KB����Work Flash��32KB.
 ������  : by zzp 
 ��������: 2021.06.19
*******************************************************************************/
#include "AppInfo.h"

#ifdef USE_WORK_FLASH_32KB
const SECTINFO SectionInfoW32KB  __attribute__ ((used, at(FLASH_WORK_BASE_ADDR)))
= {
    {"SECTADDR"},
    FLASH_WORK_BASE_ADDR,
    ~FLASH_WORK_BASE_ADDR,
};
#error "��ʹ����32K Work�ռ�,ֻ�пռ䲻��ʱ�ſ�����,���޸�sct�ļ�,���ú�رմ˴���,ͬʱ�򿪢ڴ���!"
#else
//#error "�ڹر���32K Work�ռ�,���޸�sct�ļ�,Ȼ��رմ˴���ͬʱ�򿪢ٴ���!"    
#endif

