
/******************************************************************************
    ��Ȩ���� (C), 2001-2019, �����а��յ��Լ����������޹�˾

    hzk2uni.c ��ͷ�ļ�,Hzk2Unicode.exe����
    by zzp

******************************************************************************/
#ifndef __HZK2UNI_H__
#define __HZK2UNI_H__

#ifdef USE_GBK
extern const U16 hzk2gbk_tab[];
#elif defined (USE_UNICODE)
extern const U16 hzk2unicode_tab[];
#endif

#ifndef HZK_CH_BGN
    #define HZK_CH_BGN  0xA1A1
#endif

#ifndef HZK_CH_END
    #define HZK_CH_END  (HZK_CH_BGN + 945)
#endif

#endif /* __HZK2UNI_H__ */

