/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: AppInfo.c
 �ļ�����: ����APP��Ϣ(��BOOT����Ӧ�ó������)��ͨ����������λ��0xc04000��
 	�»���ǰ�沿����Ӧ�ó����������»��ߺ��沿��һ�㲻����ģ�
 ������  : 
 ��������: 
*******************************************************************************/
#include "AppInfo.h"

/* N0004 */
unsigned char const AppInfo[128] __attribute__ ((used, at(APP_INFO_ADDR)))
= {"INIT"APPFUNCODEVER"_V1."BOOT_VER".0.00.FFFF_"};

#ifdef USE_DEBUG
    #warning "DEBUGģʽ!����ʱһ��Ҫ�ر�!!!"
#endif

#ifdef USE_QUICK_TIME
    #warning "ע��: �����ÿ���ʱ�����Ĭ��ʱ��!����ʱһ��Ҫ�ر�!!!"
#endif

#if defined(M_DEBUG)
    #warning "��ͨ�ð汾!"
    #warning "������� >>>>> �ڲ����԰汾!!!����ʱһ��Ҫת��!!!"
#elif defined(M_TCL)
    #warning "������� >>>>> ��ɽTCL�汾!!!"
#elif defined(M_CHANGHONG)
    #warning "������� >>>>> ��ɽ����汾!!!"
#elif defined(M_TONGYONG)
    #warning "������� >>>>> ͨ�ð汾!!!"
#elif defined(M_DISEN)
    #warning "������� >>>>> ���ݵ�ɭ�汾!!!"
#elif defined(M_PAIWO)
    #warning "������� >>>>> �㶫���ְ汾!!!"
#elif defined(M_NIUENTAI)
    #warning "������� >>>>> �㶫Ŧ��̩�汾!!!"
#elif defined(M_SHENGNENG)
    #warning "������� >>>>> �㽭���ܰ汾!!!"
#elif defined(M_HONGHE)
    #warning "������� >>>>> ��ɽ��̰汾!!!"
#elif defined(M_WANHE)
    #warning "������� >>>>> �㶫��Ͱ汾!!!"
#elif defined(M_CHANGFAN)
    #warning "������� >>>>> �㶫�����汾!!!"
#endif

