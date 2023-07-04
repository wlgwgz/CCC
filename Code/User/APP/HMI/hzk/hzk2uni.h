
/******************************************************************************
    版权所有 (C), 2001-2019, 广州市邦普电脑技术开发有限公司

    hzk2uni.c 的头文件,Hzk2Unicode.exe生成
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

