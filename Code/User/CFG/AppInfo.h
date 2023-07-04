/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: AppInfo.h
 �ļ�����: ���Թ����붨��
 ������  :
 ��������:
*******************************************************************************/
//----------------------TY--BGN------------------------------------//

//----------------------TY---END-----------------------------------//
#ifndef __APPINFO_H__
#define __APPINFO_H__

//#define USE_GBK				/* ��������GBK���� */
#define USE_UNICODE			/* ��������UNICODE���� */

//#define USE_HMI_DISP				/* ʹ���ı�����ʾ */

/* ���ԣ�����ʱ�������� */
//#define USE_DEBUG
//#define USE_LAB			    /* ʵ���ҳ�������� */
//#define USE_HIT               /* �������,���ڿɿ���ʵ�� */

/* ʹ��WorkFlash 32K�ķ���:
    1.������ռ䳬��MainFlash��Сʱ,����WorkFlash�ռ�.
    2.����USE_WORK_FLASH_32KB�궨��
    3.����Keil����Linker->ScatterFileѡ��s6e2h14_256KB_32KB.sct
    4.��MainFlash���ŵ�512KB,���Ӧѡ��512KB��sct.
*/
//#define USE_WORK_FLASH_32KB         /* ��Ҫʹ��WorkFlash��32KBʱ���� */

#ifdef USE_DEBUG
// �������õ���
//    #define USE_BMS_SOURCE      // ʹ��BMS����Ӳ����Դ
    #define USE_QUICK_TIME	    /* ʹ�ÿ���ʱ�����Ĭ��ʱ�䣬���ٲ��Եȴ� */
    #define USE_NO_SPEAKER      // ���η�����

// ������Ӱ�����
//    #define USE_NOT_LOCK        // ʹ�ò��Զ�����
    #define USE_GLOBAL_VALUE    /* ��static��Ϊȫ�ֱ��������ڷ��� */
   	#define USE_DIS_NEXCH	    /* ����ģʽʱ���رղ�����ֹ�޸Ĺ��� */
    #define USE_DEBUG_VER       // ���԰汾
    #define USE_DISP_DBGVER     // ʹ����ʾ���԰汾��Ϣ,�ڿ�������

// ���⹦�ܵ���
    #define USE_NO_PARA_RESET /* eeprom�������ò���ֵ */
//  #define USE_HDW_DEBUG       // Ӳ������
//	#define USE_LOG_PRINT       //ʹ�ô�ӡ�����ַ�����Ϣ
//    #define USE_WF_TEST
    #define USE_PUMPI_DEBUG     // ��Ƶˮ�õ���
//    #define USE_OUT_UP_DBG        // ���ȳ������޵���
#endif

#define USE_SPECIAL_READ	/* BMS��FLASH��EEPROM */
#define USE_BMS_DEBUG       /* BMS���� */
#define USE_COMMU_TEST_READ	/* BMS��ͨѶ������Ϣ */

//#define USE_KIT_PARA_FAC	/* ���ö���Ͳ����׼����� */
//#define USE_QUICK_TIME
//#define USE_MAC_POWER		/* ���������������� */

#if defined(USE_DEBUG_VER) || defined(USE_KIT_PARA_FAC)
#define DEBUG_VER   "dbg"
#else
#define DEBUG_VER   ""
#endif

/* �ŵ�KEIL���� */
//#define M_DEBUG             /* �ڲ����� */
//#define M_TCL               /* ��ɽTCL(����ΪHL080Aͨ�ð汾) */
//#define M_CHANGHONG         /* ��ɽ���� */
//#define M_TONGYONG          /* ͨ��(HL087Aͨ�ð汾) */
//#define M_DISEN             /* ���ݵ�ɭ */
//#define M_PAIWO             /* �㶫���� */
//#define M_NIUENTAI          /* �㶫Ŧ��̩ */
//#define M_SHENGNENG         /* �㽭���� */
//#define M_HONGHE            /* ��ɽ��� */
//#define M_WANHE             /* ��ͼ��� */
//#define M_CHANGFAN          /* �㶫���� */

/* �ڲ��汾�� */
#define BUILD_VER           "v0.0.37-a"
 
/* ��ذ汾��Ϣ */
#define MAX_VER             32
#define BOOT_VER            "HL055C"        /* BOOT�ͺ� */

/* ΢�������&�汾�� */
#if defined(M_DEBUG)        
    #define SFW_STEM        "HL080A.K05.001-0"              /* �ڲ����� */
    #define SFW_SUFFIX      "V100B10A"                      /* �ڲ����԰汾 */
    #define USE_QUICK_TIME                                  /* ʹ�ÿ���ʱ�����Ĭ��ʱ�䣬���ٲ��Եȴ� */
    #define USE_BMS_DEBUG_PUMPF_ERR                         /* ʹ��BMS�ֶ�������Ƶˮ�ù��� */
#elif defined(M_TCL)        
    #define SFW_STEM        "HL080A.K05.001-1"              /* ��ɽTCL */
    #define SFW_SUFFIX      "V100B03A"                      /* ����/�����汾 */
#elif defined(M_CHANGHONG)  
    #define SFW_STEM        "HL080A.K05.501-1"              /* ��ɽ���� */
    #define SFW_SUFFIX      "V100B02B"                      /* ����/�����汾 */
#elif defined(M_TONGYONG)   
    #define SFW_STEM        "HL087A.K05.001-1"              /* ͨ�� */
    #define SFW_SUFFIX      "V100B02A"                      /* ����/�����汾 */
#elif defined(M_PAIWO)      
    #define SFW_STEM        "HL087A.K05.501-1"              /* �㶫���� */
    #define SFW_SUFFIX      "V100B02A"                      /* ����/�����汾 */
#elif defined(M_DISEN)      
    #define SFW_STEM        "HL087A.K05.502-1"              /* ���ݵ�ɭ */
    #define SFW_SUFFIX      "V100B04D"                      /* ����/�����汾 */
#elif defined(M_NIUENTAI)
    #define SFW_STEM        "HL087A.K05.503-1"              /* �㶫Ŧ��̩ */
    #define SFW_SUFFIX      "V100B04A"                      /* ����/�����汾 */
#elif defined(M_SHENGNENG)
    #define SFW_STEM        "HL087A.K05.504-1"              /* �㽭���� */
    #define SFW_SUFFIX      "V100B01E"                      /* ����/�����汾 */
#elif defined(M_HONGHE)
    #define SFW_STEM        "HL087A.K05.505-1"              /* ��ɽ��� */
    #define SFW_SUFFIX      "V100B00D"                      /* ����/�����汾 */
#elif defined(M_WANHE)   
    #define SFW_STEM        "HL087A.K05.506-1"              /* ��ͼ��� */
    #define SFW_SUFFIX      "V100B01E"                      /* ����/�����汾 */
#elif defined(M_CHANGFAN)   
    #define SFW_STEM        "HL087A.K05.507-1"              /* �㶫���� */
    #define SFW_SUFFIX      "V100B02B"                      /* ����/�����汾 */
#else
    #error "δ���峧��!"
#endif

/* �������빦�� */
#if defined(M_DEBUG)                /* �ڲ����� */
    #define USE_SW_MAC              /* ʹ�ò������û��� */
    #define USE_ODU_HL081B          /* ʹ��HL081B����� */
    #define USE_AT_EEVS             /* ��������ʹ�ø����ֶ����� */
    #define USE_NEW_FORCE_IBH       /* ʹ����ǿ�����������߼� */
#elif defined(M_TCL)                /* ��ɽTCL */
    #define USE_SW_MAC              /* ʹ�ò������û��� */
    #define USE_AREA_B_FIX          /* ʹ��B���̶���Χ */
    #define USE_WF_OLD              /* ʹ�þɹ���Ƶ�ʱ� */
    #define USE_AT_SV_FIX           /* ʹ���������Թ̶�Ŀ���¶� */
#elif defined(M_TONGYONG)           /* ͨ�� */
    #define USE_ODU_HL081B          /* ʹ��HL081B����� */
    #define USE_AT_EEVS             /* ��������ʹ�ø����ֶ����� */
    #define USE_FREQ_LIMIT_TC_TE    /* ʹ�������¶Ⱥ������¶������߼� */
    #define USE_NEW_FORCE_IBH       /* ʹ����ǿ�����������߼� */
#elif defined(M_CHANGHONG)          /* ��ɽ���� */
    #define USE_WF_OLD              /* ʹ�þɹ���Ƶ�ʱ� */
    #define USE_AT_SV_FIX           /* ʹ���������Թ̶�Ŀ���¶� */
    #define USE_NEW_FORCE_IBH       /* ʹ����ǿ�����������߼� */
#elif defined(M_PAIWO)              /* �㶫���� */
    #define USE_SW_MAC              /* ʹ�ò������û��� */
    #define USE_ODU_HL081B          /* ʹ��HL081B����� */
    #define USE_WF_OLD              /* ʹ�þɹ���Ƶ�ʱ� */    
    #define USE_AT_SV_FIX           /* ʹ���������Թ̶�Ŀ���¶� */
#elif defined(M_DISEN)              /* ���ݵ�ɭ */
    #define USE_SW_MAC              /* ʹ�ò������û��� */
    #define USE_ODU_HL081B          /* ʹ��HL081B����� */
    #define USE_AT_SV_FIX           /* ʹ���������Թ̶�Ŀ���¶� */
#elif defined(M_NIUENTAI)           /* �㶫Ŧ��̩ */
    #define USE_SW_MAC              /* ʹ�ò������û��� */
    #define USE_ODU_HL081B          /* ʹ��HL081B����� */
    #define USE_AT_SV_FIX           /* ʹ���������Թ̶�Ŀ���¶� */
#elif defined(M_SHENGNENG)          /* �㽭���� */
    #define USE_WF_SHENGNENG        /* ʹ�����ܹ���Ƶ�ʱ� */
    #define USE_SW_MAC              /* ʹ�ò������û��� */
    #define USE_ODU_HL081B          /* ʹ��HL081B����� */
    #define USE_HT_ENV_NEW          /* ʹ�������ȷֶλ��� */
    #define USE_AT_SV_FIX           /* ʹ���������Թ̶�Ŀ���¶� */
    #define USE_FREQ_LIMIT_TC_TE    /* ʹ�������¶Ⱥ������¶������߼� */
#elif defined(M_HONGHE)             /* ��ɽ��� */
    #define USE_SW_MAC              /* ʹ�ò������û��� */
    #define USE_ODU_HL081B          /* ʹ��HL081B����� */
    #define USE_AT_SV_FIX           /* ʹ���������Թ̶�Ŀ���¶� */
    #define USE_FREQ_LIMIT_TC_TE    /* ʹ�������¶Ⱥ������¶������߼� */
#elif defined(M_WANHE)              /* �㶫��� */
    #define USE_ODU_HL081B          /* ʹ��HL081B����� */
    #define USE_AT_EEVS             /* ��������ʹ�ø����ֶ����� */
    #define USE_FREQ_LIMIT_TC_TE    /* ʹ�������¶Ⱥ������¶������߼� */
    #define USE_NEW_FORCE_IBH       /* ʹ����ǿ�����������߼� */
#elif defined(M_CHANGFAN)           /* �㶫���� */
    #define USE_ODU_HL081B          /* ʹ��HL081B����� */
    #define USE_AT_EEVS             /* ��������ʹ�ø����ֶ����� */
    #define USE_FREQ_LIMIT_TC_TE    /* ʹ�������¶Ⱥ������¶������߼� */
    #define USE_NEW_FORCE_IBH       /* ʹ����ǿ�����������߼� */
#endif

/* ʹ�����Ķ�ˮ�¹���Ƶ�ʱ�Ĭ���������¹��ܺ궨�� */
#ifndef USE_WF_OLD
     #define USE_WF_R290     /* ʹ��R290����Ƶ�ʱ� */
#endif

#define APPFUNCODEVER       "X1."SFW_STEM"."SFW_SUFFIX DEBUG_VER /* ΢��汾 */
#define STR_CODE            "STR."SFW_STEM                          /* �ַ�����汾 */

/* ��Ʒ�ͺţ�ʶ���룩 */
#define PCB_ID_CODE		    "LK05XXXXA"
#define MAX_ID  		    (sizeof(PCB_ID_CODE) / sizeof(U08))

#define FALSH_ADDR_BASE     (0x00000000)
#define IPA_BASE_ADDR       (FALSH_ADDR_BASE)
#define IAP_SIZE            (0x4000)
#define BOOT_VER_ADDR       (FALSH_ADDR_BASE + 0x3F80)    /* BOOT�İ汾��ַ */
#define APP_INFO_ADDR       (IPA_BASE_ADDR + IAP_SIZE)    /* INFO��Ϣ��ַ */
#define APP_VECT_TAB_OFFSET (FALSH_ADDR_BASE + 0x4200)    /* APP�ж�������ַ */


#define FLASH_WORK_BASE_ADDR (0x200C0000)
#define SEC_INFO_W32KB       (FLASH_WORK_BASE_ADDR)       /* ����Ϣ:WorkFlash 32KB */

typedef struct {
    unsigned char info[8];  // ����Ϣͷ,�̶�Ϊ�ַ���"SECTADDR"
    unsigned long addr;     // ���׵�ַ
    unsigned long naddr;    // ���׵�ַ����
}SECTINFO;


//#define CHG_VER			"2018/03/29"

#ifndef USE_QUICK_TIME
#define QK_5        5
#define QK_60       60
#define QK_3600     3600
#else
#define QK_5        (fg_dbg_slow?5:1)
#define QK_60       (fg_dbg_slow?60:1)
#define QK_3600     (fg_dbg_slow?3600:60)
#endif

/*******************************************************************************
���⴦��ͳһ��ע

 --> N0001:ȥ�����뾯��
 --> N0002:٤��ܽ����HL051B
 --> N0003:���ܵ������⴦��������ɻָ�ԭ״
 --> N0004:��ͨ��X1.HL051A.K05.002-1������ĵط�
 --> N0005:��������Ƶˮ�ÿ��������߼����Ż�

*******************************************************************************/


#endif
