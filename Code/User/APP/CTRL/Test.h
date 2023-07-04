/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: Test.h
 �ļ�����: 
 ������  : 
 ��������: 
*******************************************************************************/
#ifndef TEST_H
#define TEST_H

  
#ifdef  TEST_GLOBAL
 #define TEST_EXT
#else
 #define TEST_EXT extern
#endif


#define TestTbl				((BIT16 *)&CoreTbl[28].word)


// ��������,(Ԥ��λ�ã�0~15 )
#define TestTbl_CMD			((BIT16 *)&TestTbl[0].word)

#define TestBackboardCmd        (TestTbl[0].byte.high)   /* ���Ժ������ */
#define TBC_EXIT                0                           /* �˳����Գ��� */
#define TBC_ENTER               1                           /* ������Գ��� */
#define TestBackboardType       (TestTbl[0].byte.low)    /* ���Ժ���ͺ� */
#define HL006A                  1
#define HL011A                  2
#define HL012A                  3
#define HL012A_EX               4

#define TestSwOutMode           (TestTbl[1].byte.high)      /* ���Լ̵������ģʽ */

#define TCmdTestEEPROM          (TestTbl[2].bits.b0)        /* ���ԡ���ʼ��EEPROM */
#define TCmdTestSpiflash        (TestTbl[2].bits.b1)        /* ���ԡ���ʼ��Spiflash */
#define TCmdInitEEPROM          (TestTbl[2].bits.b2)        /* ��ʼ��eeprom */
#define TCmdClrUartRcd          (TestTbl[2].bits.b3)        /* ����ͨѶ��¼���� */
#define TCmdClrCurRcd			(TestTbl[2].bits.b4)        /* ������¼��λ */
#define TCmdClrSwInRcd          (TestTbl[2].bits.b5)        /* ������\���뿪�������������*/
#define TCmdClrTmpRcd           (TestTbl[2].bits.b6)        /* �¶ȼ�¼��λ */
#define TCmdClrAIRcd            (TestTbl[2].bits.b7)        /* ģ������¼��λ */
#define TCmdadjustAO            (TestTbl[2].bits.b8)        /* ģ�������У׼ */
#define TCmdClrReset            (TestTbl[2].bits.b9)        /* ���㸴λ����*/
#define TCmdadjustpt1000        (TestTbl[2].bits.b10)       /* PT1000��ʼУ׼*/
#define TCmdClrFrepRcd          (TestTbl[2].bits.b11)       /* Ƶ��ֵ��λ*/
#define TCmdClrSwoutcount       (TestTbl[2].bits.b12)       /* �̵�������*/

#define TCmdClrPwmInRcd         (TestTbl[2].bits.b14)       /* ����PWM�����¼ */
//#define TCmdHavePvdTest         (TestTbl[2].bits.b14)       /* �Ƿ��е������ */
//#define TCmdDontAdjAi           (TestTbl[2].bits.b15)       /* ��У׼AI */


// ���������������,Ԥ��16���������������ת����,(Ԥ��λ�ã�3~4)

#define TestStepCmd				(TestTbl[3].word)   /* ����������� */


// �ڶ�������(λ�ã�8)
#define TCmd2ClrAdcAdjust        (TestTbl[8].bits.b0)       /* ����У׼ADC */



// ����״̬,(Ԥ��λ�ã�16~39 )
#define TestTbl_STA			((BIT16 *)&TestTbl[16].word)

#define TestProgStaus           (TestTbl[16].word)      /* ���Գ���״̬ 69*/
#define TPS_NO_TEST_CMD         0           /* ���δ�յ������������ */
#define TPS_ENTER_TEST_INIT     1           /* ���׼������������� */
#define TPS_ENTER_TEST_OK       2           /* ����ѽ������״̬ */
#define TPS_TYPE_ERR            3           /* ����ͺ�ѡ����� */
#define TPS_DISENABLE_TEST      4           /* ������������״̬�����ѽ��빤������ */

#define TestEepromCurPage       (TestTbl[18].word)      /* Eeprom��ǰ����ҳ */
#define TestEepromTotalPage     (TestTbl[19].word)      /* Eeprom����Ե���ҳ�� */
#define TestSpiflashCurPage     (TestTbl[20].word)      /* Flash��ǰ����ҳ */
#define TestSpiflashTotalPage   (TestTbl[21].word)      /* Flash����Ե���ҳ�� */

//״̬��0Ϊδ�⣬1Ϊ�����У�2Ϊ���Գɹ���3Ϊ����ʧ��
#define TestEepromStauts        (TestTbl[22].word)      /* Eeprom����״̬ */
#define InitEepromStauts        (TestTbl[23].word)      /* Eeprom��ʼ��״̬ */
#define TestSpiflashStauts      (TestTbl[24].word)      /* Flash����״̬ */
#define InitSpiflashStauts      (TestTbl[25].word)      /* Flash��ʼ��״̬*/

// ��������-MAX_DI_NUM=8·����������,(Ԥ��λ�ã�40~47 (=40+8-1))
// 8����=1���ֵ����ֿ�����+1���ְ���+1���������+1���ְ���+Ԥ��4����

#define TEST_DI_BEGIN      40                                                                    // ���������Դ洢��ʼλ��
#define TEST_DI_WORD       ((MAX_DI_TOTAL_NUM-1)/16+1)                                                 // ���ֿ�����ռ����

#define TestTbl_DI			((BIT16 *)&TestTbl[TEST_DI_BEGIN].word)                             // ��������������
#define DI_dinput(n)                  (TestTbl[TEST_DI_BEGIN+n].word)                           // ���ֿ�����
#define SW_SWITCH    		          (TestTbl[TEST_DI_BEGIN+TEST_DI_WORD+0].word)              // ϵͳ����
#define THR_PHASE			          (TestTbl[TEST_DI_BEGIN+TEST_DI_WORD+1].word)		        // �����
#define SR_SWITCH		              (TestTbl[TEST_DI_BEGIN+TEST_DI_WORD+2].word)		        // ����


// ��������-MAX_NTC_NUM=11·�¶�(Ԥ��12·),(Ԥ��λ�ã�48~119 (=48+6��*12·-1))
#define TEST_TEMP_BEGIN     48                                                                  // �¶Ȳ��Դ洢��ʼλ��
#define TEST_TEMP_ITEM       6                                                                  // �¶Ȳ�����Ŀ
#define TestTbl_NTC			((BIT16 *)&TestTbl[TEST_TEMP_BEGIN].word)                           // �¶Ȳ�������

#define TestCurTmp(n)         (*(I16*)&TestTbl[TEST_TEMP_BEGIN+0+(n)*TEST_TEMP_ITEM].word)      /* ��ǰ�¶�ֵ */
#define TestCurTmpMax(n)      (*(I16*)&TestTbl[TEST_TEMP_BEGIN+1+(n)*TEST_TEMP_ITEM].word)      /* ����¶�ֵ */
#define TestCurTmpMin(n)      (*(I16*)&TestTbl[TEST_TEMP_BEGIN+2+(n)*TEST_TEMP_ITEM].word)      /* ��С�¶�ֵ */
#define TestCurTmpAd(n)               (TestTbl[TEST_TEMP_BEGIN+3+(n)*TEST_TEMP_ITEM].word)      /* ��ǰ�¶�adֵ���Ѽ�ȥGND */
#define TestCurTmpAdMax(n)            (TestTbl[TEST_TEMP_BEGIN+4+(n)*TEST_TEMP_ITEM].word)      /* ����¶�adֵ */
#define TestCurTmpAdMin(n)            (TestTbl[TEST_TEMP_BEGIN+5+(n)*TEST_TEMP_ITEM].word)      /* ��С�¶�adֵ */

// �������ݣ�Ԥ��72�ֽ�(120~191)


// �������ݣ�MAX_CUR_CH_NUM=2·����(Ԥ��5·)��(Ԥ��λ�ã�192~221 (=192+6��*5·-1))
#define TEST_CURR_BEGIN     (TEST_TEMP_BEGIN+MAX_NTC_NUM*TEST_TEMP_ITEM)                        // �������Դ洢��ʼλ��
#define TEST_CURR_ITEM       6                                                                  // ����������Ŀ
#define TestTbl_CUR			((BIT16 *)&TestTbl[TEST_CURR_BEGIN].word)

#define TestCurCur(n)                 (TestTbl[TEST_CURR_BEGIN+0+(n)*TEST_CURR_ITEM].word)     /* ��ǰ����ֵ */
#define TestCurCurMax(n)              (TestTbl[TEST_CURR_BEGIN+1+(n)*TEST_CURR_ITEM].word)     /* ���ֵ */
#define TestCurCurMin(n)              (TestTbl[TEST_CURR_BEGIN+2+(n)*TEST_CURR_ITEM].word)     /* ��Сֵ */
#define TestCurCurAd(n)               (TestTbl[TEST_CURR_BEGIN+3+(n)*TEST_CURR_ITEM].word)     /* ��ǰ����adֵ*/
#define TestCurCurAdMax(n)            (TestTbl[TEST_CURR_BEGIN+4+(n)*TEST_CURR_ITEM].word)     /* ���adֵ */
#define TestCurCurAdMin(n)            (TestTbl[TEST_CURR_BEGIN+5+(n)*TEST_CURR_ITEM].word)     /* ��Сadֵ */

// �������ݣ�Ԥ��30�ֽ�(222~251)


// �������ݣ�MAX_AI_CH_NUM=4·ģ����(Ԥ��8·)��(ģ�������� Ԥ��λ�ã�252~259 (=252+1��*8·-1))
#define TEST_AI_TYPE_BEGIN  (252)                                                               // ģ�����������ʹ洢��ʼλ��
#define TestTbl_AI_TYPE		((BIT16 *)&TestTbl[TEST_AI_TYPE_BEGIN].word)
#define TestAIType(n)       (TestTbl_AI_TYPE[n].word)         

// �������ݣ�MAX_AI_CH_NUM=4·ģ����(Ԥ��8·)��(ģ�������� Ԥ��λ�ã�260~307 (=260+6��*8·-1))
#define TEST_AI_DATA_BEGIN  (260)                                                               // ģ�������ݴ洢��ʼλ��
#define TEST_AI_DATA_ITEM    6                                                                  // ģ�������Ը���                                                         // ģ�����������ݴ洢��ʼλ��
#define TestTbl_AI_DATA		((BIT16 *)&TestTbl[TEST_AI_DATA_BEGIN].word)

#define TestCurAI(n)                  (TestTbl[TEST_AI_DATA_BEGIN+0+(n)*TEST_AI_DATA_ITEM].word)   /* ��ǰֵ */
#define TestCurAIMax(n)               (TestTbl[TEST_AI_DATA_BEGIN+1+(n)*TEST_AI_DATA_ITEM].word)   /* ���ֵ */
#define TestCurAIMin(n)               (TestTbl[TEST_AI_DATA_BEGIN+2+(n)*TEST_AI_DATA_ITEM].word)   /* ��Сֵ */
#define TestCurAIAd(n)                (TestTbl[TEST_AI_DATA_BEGIN+3+(n)*TEST_AI_DATA_ITEM].word)   /* ��ǰadֵ*/
#define TestCurAIAdMax(n)             (TestTbl[TEST_AI_DATA_BEGIN+4+(n)*TEST_AI_DATA_ITEM].word)   /* ���adֵ */
#define TestCurAIAdMin(n)             (TestTbl[TEST_AI_DATA_BEGIN+5+(n)*TEST_AI_DATA_ITEM].word)   /* ��Сadֵ */

// �������ݣ�Ԥ��48�ֽ�(308~355)


// �������ݣ�MAX_MOTOR=4·�������(Ԥ��8·)��(Ԥ��λ�ã�356~363 (=356+1��*8·-1))
#define TEST_STEP_NOW_BEGIN     (356)                                                               // ���������ǰֵ�洢��ʼλ��
#define TestTbl_STEP_NOW   	((BIT16 *)&TestTbl[TEST_STEP_NOW_BEGIN].word)

#define TestStepNow(n)                (TestTbl[TEST_STEP_NOW_BEGIN+(n)].word)                       // ��ǰ����

// �������ݣ�MAX_MOTOR=4·�������(Ԥ��8·)��(Ԥ��λ�ã�364~371 (=364+1��*8·-1))
#define TEST_STEP_SET_BEGIN     (364)                                                               // ��������趨ֵ�洢��ʼλ��
#define TestTbl_STEP_SET   	((BIT16 *)&TestTbl[TEST_STEP_SET_BEGIN].word)

#define TestStepSet(n)                (TestTbl[TEST_STEP_SET_BEGIN+(n)].word)                       // �趨����


// �������ݣ�3·����(Ԥ��5·)��(Ԥ��λ�ã�372~416 (=372+9��*5·-1))
/*******************************************************************************
1�� ��ο�ͨѶ��¼�ṹCOMRECORD���崦��ע�� 
2�� �����Ǻ��Ը����ڵļ�¼��Ϣ���������Ǵӻ�ʱ(�㲥����ʱ��ʧ�ܡ���������Ϊ0)��
    ͨѶ����ʱ��ʾ����Ϣһ��Ϊ�����м�¼����Ϣ;��ȻҲ������ʾ�ӻ��м�¼����Ϣ��
3�� ����MODBUS�������˳������COMRECORD�м����������˳��һ��
*******************************************************************************/

#define TEST_COMMU_BEGIN    (372)
#define TEST_COMMU_ITEM     9 
#define TestTbl_COMMU		((BIT16 *)&TestTbl[TEST_COMMU_BEGIN].word)

#define BKBD_COMMU_RCD_PTR  ((COMRECORD *)&TestTbl[TEST_COMMU_BEGIN])     
#define BKBD_INVT_RCD_PTR       (BKBD_COMMU_RCD_PTR+0)


#define BKBD_INVT_UART0         (BKBD_COMMU_RCD_PTR+0)  //BMS��ؼ�GD005A
#define BKBD_INVT_UART1         (BKBD_COMMU_RCD_PTR+1)  //����
#define BKBD_INVT_UART2         (BKBD_COMMU_RCD_PTR+2)  //��Ƶ��
#define BKBD_INVT_UART3         (BKBD_COMMU_RCD_PTR+3)  //��ʾ��
#ifdef COM_4G
#define BKBD_INVT_UART4         (BKBD_COMMU_RCD_PTR+4)  //4G
#endif


// �������ݣ�MAX_DI_NUM=8·������(Ԥ��16·)���������仯������(Ԥ��λ�ã�417~432 (=417+1��*16·-1))
#define TEST_DI_DATA_BEGIN  (417)
#define TestTbl_DI_DATA		((BIT16 *)&TestTbl[TEST_DI_DATA_BEGIN].word)    // �������仯����

#define TestSwInChange(n)       (TestTbl[417+n].word)        /* �������仯���� 24*/



// �������ݣ�������¼(Ԥ��λ�ã�433~)
#define TestTimeDay             (TestTbl[433].word)     /* �� */
#define TestTimeHour            (TestTbl[434].word)     /* Сʱ */
#define TestTimeMin             (TestTbl[435].word)     /* �� */
#define TestTimeSec             (TestTbl[436].word)     /* �� */

#define TestResetPowerCnt       (TestTbl[437].byte.low)/*�ϵ縴λ����*/
#define TestResetDMPCnt         (TestTbl[438].byte.high)/*��Դ��λ����*/
#define TestResetUserCnt        (TestTbl[439].byte.low)/*�ڲ�Ӧ��/Ӧ�ø�λ����*/



//#define Testfbusnum             (TestTbl[?].word)    /* FBUSͨѶģ����� */
// �������ݣ�������440~449��



// �������ݣ�PWMԤ��2·         (Ԥ��λ�ã�450~459 (=450+5��*2·-1))
#define TEST_PWM_DATA_BEGIN     (450)
#define TEST_PWM_DATA_ITEM      (5  )                // PWM��������
#define TestTbl_PWM			    ((BIT16 *)&TestTbl[TEST_PWM_DATA_BEGIN].word)                           // �¶Ȳ�������
#define TestPwmOutFreq(n)       (TestTbl[TEST_PWM_DATA_BEGIN+0+(n)*TEST_PWM_DATA_ITEM].word)//PWM���Ƶ��
#define TestPwmOutDuty(n)       (TestTbl[TEST_PWM_DATA_BEGIN+1+(n)*TEST_PWM_DATA_ITEM].word)//PWM���ռ�ձ�
#define TestPwmInCur(n)         (TestTbl[TEST_PWM_DATA_BEGIN+2+(n)*TEST_PWM_DATA_ITEM].word)//PWM���뵱ǰռ�ձ�
#define TestPwmInMax(n)         (TestTbl[TEST_PWM_DATA_BEGIN+3+(n)*TEST_PWM_DATA_ITEM].word)//PWM�������ռ�ձ�
#define TestPwmInMin(n)         (TestTbl[TEST_PWM_DATA_BEGIN+4+(n)*TEST_PWM_DATA_ITEM].word)//PWM������Сռ�ձ�

// �������ݣ�ʣ�ࣨ460~483��/* N0004 */
#define TEST_PWM2_DATA_BEGIN     (460)
#define TEST_PWM2_DATA_ITEM      (5  )                // PWM��������
#define TestTb2_PWM			    ((BIT16 *)&TestTbl[TEST_PWM2_DATA_BEGIN].word)                           // �¶Ȳ�������
#define TestPwm2OutFreq(n)       (TestTbl[TEST_PWM2_DATA_BEGIN+0+(n)*TEST_PWM_DATA_ITEM].word)//PWM���Ƶ��
#define TestPwm2OutDuty(n)       (TestTbl[TEST_PWM2_DATA_BEGIN+1+(n)*TEST_PWM_DATA_ITEM].word)//PWM���ռ�ձ�
#define TestPwm2InCur(n)         (TestTbl[TEST_PWM2_DATA_BEGIN+2+(n)*TEST_PWM_DATA_ITEM].word)//PWM���뵱ǰƵ��
#define TestPwm2InMax(n)         (TestTbl[TEST_PWM2_DATA_BEGIN+3+(n)*TEST_PWM_DATA_ITEM].word)//PWM�������Ƶ��
#define TestPwm2InMin(n)         (TestTbl[TEST_PWM2_DATA_BEGIN+4+(n)*TEST_PWM_DATA_ITEM].word)//PWM������СƵ��

/* �������ݣ���Ƶ��Ϣ */
typedef struct TEST_INVT
{
    U16 func;				/* ������ */
    U16 ver;				/* �汾�� */
    I16 fg_init;			/* ��ʼ�� */
    I16 fg_open;			/* ���� */
    I16 fg_close;			/* �ػ� */
    I16 fg_reset;			/* ��λ */
    I16 power;				/* ���ʵȼ� */
    I16 motor;				/* ����ͺ� */
    I16 freq_sv;			/* Ŀ��Ƶ�� */
    U16 freq_run;			/* ����Ƶ�� */
    I16 volt_out;			/* �����ѹ */
    I16 curr_out;			/* ������� */
    U16 bord_rate;			/* ������ */
    I16 volt_gene;			/* ĸ�ߵ�ѹ */
    I16 temp_module;		/* ģ���¶� */
    I16 error;				/* ������Ϣ */
    I16 temp_pfc;			/* PFC�¶� */
    U16 fan_sv;				/* Ŀ����� */
    U16 fan_pv1;			/* ��ǰ����1 */
    U16 fan_pv2;			/* ��ǰ����2 */
}TEST_INVT_STRU;

TEST_EXT TEST_INVT_STRU test_invt_info;

TEST_EXT  U16 test_time_current;

TEST_EXT U08 ad_adjust_cnt;

TEST_EXT  U08 test_allow;
TEST_EXT  U08 punp_fg;      //������ճ�ʼ�������־�� 1:������ճ�ʼ���˵�                       0:û�н�����ճ�ʼ���˵�
   
                
#define	SYS_GetTick		HDW_1ms_Tick


#define MAX_4G_VER_WORD		16		// 4Gģ�鹦����汾����Ϣ�������
#define MAX_4G_SN_WORD		16		// 4Gģ��SN����Ϣ�������

// ���ڴ洢��������Ҫ��ȡ����4Gģ�������Ϣ
typedef struct
{
    U16 test_sta;                   /* 4Gģ�����״̬ */ 
    U16 csq;                        /* 4Gģ���ź�ǿ�� */
    U16 total;                      /* 4G����ư���ͨѶ���� */
    U16 succ;                       /* 4G����ư�ͨѶ�ܳɹ����� */
    U16 fail;                       /* 4G����ư�ͨѶ��ʧ�ܴ��� */
	U16 ver[MAX_4G_VER_WORD];		// 4Gģ�鹦����汾��
	U16 sn[MAX_4G_SN_WORD];			// 4Gģ��SN��
} TEST4G;

TEST_EXT TEST4G test_4g;


TEST_EXT void TEST_Main(void);
#endif
