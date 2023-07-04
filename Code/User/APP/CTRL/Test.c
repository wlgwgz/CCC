/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: Test.c
 �ļ�����:
 ������  :
 ��������:
*******************************************************************************/

#define  TEST_GLOBAL
#include "includes.h"


//static BOOL XDATA SpeakerAct = FALSE;
static BOOL XDATA FgFirstEnterTest = TRUE;

static BOOL XDATA FgReliability = FALSE;	/* �ɿ������� */

//extern BOOL PT1000_Adjust();
/*******************************************************************************
 �� ��: TEST_Enter()
 �� ��: ���������Գ���
 �� ��: ��
 �� ��: TRUE/FALSE--�ɹ��������/ʧ��
 �� ��: ��
*******************************************************************************/
//BOOL TEST_Enter(void)
//{
//  if (TestBackboardCmd == TBC_ENTER)
//  {
//      if (TestBackboardType == TBT_LY308)
//      {
//          TestProgStaus = TPS_ENTER_TEST_OK;
//
//
//          FgFirstEnterTest = TRUE;
//
//          return TRUE;
//      }
//      else
//      {
//          TestProgStaus = TPS_TYPE_ERR;
//      }
//  }
//  return FALSE;
//}

/*******************************************************************************
 �� ��: TEST_Exit()
 �� ��: �����˳����Գ���
 �� ��: ��
 �� ��: TRUE/FALSE--�ɹ��˳�����/ʧ��
 �� ��: �ɿ��Դ����̵��������ȵȸ�λ
*******************************************************************************/
//BOOL TEST_Exit(void)
//{
//  if (TestBackboardCmd == TBC_EXIT)
//  {
//      TestProgStaus = TPS_NO_TEST_CMD;
//
//      /* �˳�����ʱ��λ���� */
//
//      /* ��λ������ */
//      TCmdClrSwInRcd = 1;
//
//
//      /* ��λ�̵��� */
//      TestSwOutMode = 0;
//      SysSwOut0 = 0;
//      SysSwOut1 = 0;
//
//      /* ��λ���� */
//      TCmdChgSpeaker = 0;
//      SpeakerAct = FALSE;
//
//      /* ��λ����ʱ�䣬�Ա�δ�����ϵ��ٽ�����Գ��� */
//      TestTimeSec = TestTimeMin = TestTimeHour = TestTimeDay = 0;
//
//      /* ��λͨѶ��¼ */
//      TCmdClrUartRcd = 1;
//      TCmdClrCanRcd = 1;
//
//      return TRUE;
//  }
//  else
//  {
//      return FALSE;
//  }
//}

/*****************************************************************************
��  �� : TEST_HdwInit()
��  �� : ���Գ�ʼ��Ӳ��
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void TEST_HdwInit(void)
{
    U08 i;

    /* ��ʼ��PWM */
    for (i=0; i<MAX_PWM; i++)
    {
        TestPwmOutFreq(i) = 100;
        TestPwmOutDuty(i) = 100;
    }

    /* ��ʼ��2PWM */
    for (i=0; i<MAX_2PWM; i++)
    {
        TestPwm2OutFreq(i) = 100;
        TestPwm2OutDuty(i) = 100;
    }

    VALCLR(test_4g);
	app_4g_ro.cmd = INTO_4G_TEST;

}

extern U32 HdwTickMs;
/*******************************************************************************
 �� ��: TEST_EEPROMAndInit()
 �� ��: ���ԡ���ʼ��
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TEST_EEPROMAndInit(void)
{
    enum STATUS //���Լ���ʼ��״̬
    {
        STATUS_NO_TEST,
        STATUS_TESTING,
        STATUS_SUCCESS,
        STATUS_FAIL,
        STATUS_MAX,
    };

    U08 testStatus;
//    static I16 da_calibra_bak[MAX_AO_CHANNEL][2]={{0,0},{0,0},{0,0}};/* DAУ׼ֵ(ռ�ձ�)�洢ֵ���洢ֵֻ������ */
//    static I16 adjustTemp[2] = {0, 0};
    static BOOL fg_test_start = FALSE;  //��ʼ���Ա�־

    if (TCmdTestEEPROM == 1)
    {
        TCmdTestEEPROM = 0;
        fg_test_start = TRUE;
    }
    if(fg_test_start)
    {
        testStatus = eprom_test();
        TestEepromCurPage = eprom_get_test_page();
        switch (testStatus)
        {
            case EEPROM_TEST_READY:
                TestEepromStauts = STATUS_NO_TEST;
                break;

            case EEPROM_TEST_PROCESS:
                TestEepromStauts = STATUS_TESTING;
                break;

            /* ����OK Ĭ�ϲ�����ʼ�� */
            case EEPROM_TEST_OK:
                TestEepromStauts = STATUS_SUCCESS;
                TCmdInitEEPROM = 1;
                //reset_all_data();   //Ĭ�ϳ�ʼ��һ��
                fg_test_start = FALSE;
                break;
            /* ����ʧ�� */
            case EEPROM_TEST_WR_ERR:
            case EEPROM_TEST_RD_ERR:
            case EEPROM_TEST_CHK_ERR:
            case EEPROM_TEST_TYPE_ERR:
                TestEepromStauts = STATUS_FAIL;
                fg_test_start = FALSE;
                break;

            default:
                break;
        }
    }


    //��ʼ������
    if(TCmdInitEEPROM == 1)
    {
        TCmdInitEEPROM = 0;
        if(!fg_test_start)//wlg20150706
        {
            DataAccess_LimitPara(DATA_INIT);        // ��ʼ��ϵͳά������
//            DataAccess_ModbusPara(DATA_INIT);       // ��ʼ������ͨѶ����
            DataAccess_FacPara(DATA_INIT);          // ��ʼ�����ܲ���

            Password_InitAll();                     // ��ʼ������
            DataAccess_CompRuntime(DATA_INIT);      // ��ʼ��ѹ������ʱ��
            DataAccess_SysRuntime(DATA_INIT);       // ��ʼ��ϵͳ����ʱ��
//            DataAccess_Timer(DATA_INIT);			// ��ʼ����ʱʱ��
	        DataAccess_HistoryErr(DATA_INIT);		// ��ʼ����ʷ������Ϣ
	        DataAccess_Language(DATA_INIT);	        // ��ʼ��������Ϣ
	        DataAccess_Temp_unit(DATA_INIT);	        // ��ʼ��������Ϣ
	        DataAccess_InvtExceptInfo(DATA_INIT, _MAX_(U08));	// ��ʼ����Ƶ�쳣��Ϣ
//	        DataAccess_SkipPreheatCnt(DATA_INIT);	        /* ��ʼ������Ԥ�ȼ��� */
//			DataAccess_holiday(DATA_INIT);
			DataAccess_PowerUsageWeek(DATA_INIT);
            DataAccess_PowerUsageYear(DATA_INIT);
        }
    }
//    switch(GET_INIT_PARA())
//    {
//        case ACTION_IDLE:
//            InitEepromStauts = STATUS_NO_TEST;
//            break;
//
//        case ACTION_START:      // ����ִ�п�ʼ
//        case ACTION_ING:        // ����ִ����
//            InitEepromStauts = STATUS_TESTING;
//            break;
//
//        case ACTION_SUCC:   // ����ִ�гɹ�
//            InitEepromStauts = STATUS_SUCCESS;
//            break;
//        case ACTION_FAIL:   // ����ִ��ʧ��
//            InitEepromStauts = STATUS_FAIL;
//            break;
//    }
    if(GET_INIT_PARA()>=ACTION_ING) InitEepromStauts = GET_INIT_PARA()-1;
    else                            InitEepromStauts = STATUS_NO_TEST;
}



/*******************************************************************************
 �� ��: TEST_FlashAndInit()
 �� ��: FLASH���ԡ���ʼ��
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TEST_FlashAndInit(void)
{
#if 0
    enum STATUS //���Լ���ʼ��״̬
    {
        STATUS_NO_TEST,
        STATUS_TESTING,
        STATUS_SUCCESS,
        STATUS_FAIL,
        STATUS_MAX,
    };

    U16 testStatus;
    static BOOL fg_test_start = FALSE;  //��ʼ���Ա�־

    if (TCmdTestSpiflash == 1)
    {
        TCmdTestSpiflash = 0;
        fg_test_start = TRUE;
    }

    if (fg_test_start)
    {
        testStatus = flash_test();
        TestSpiflashCurPage = flash_get_test_page();
        switch (testStatus)
        {
            case FLASH_TEST_READY:
                TestSpiflashStauts = STATUS_NO_TEST;
                break;

            case FLASH_TEST_PROCESS:
                TestSpiflashStauts = STATUS_TESTING;
                break;

            /* ����OK */
            case FLASH_TEST_OK:
                TestSpiflashStauts = STATUS_SUCCESS;
                fg_test_start = FALSE;
                break;

            /* ����ʧ�� */
            case FLASH_TEST_WR_ERR:
            case FLASH_TEST_RD_ERR:
            case FLASH_TEST_CHK_ERR:
            case FLASH_TEST_TYPE_ERR:
                TestSpiflashStauts = STATUS_FAIL;
                fg_test_start = FALSE;
                break;

            default:
                break;
        }
    }
#endif
}




/*******************************************************************************
 �� ��: TEST_SwIn()
 �� ��: ���������������
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/

static void TEST_SwIn(void)
{
    #define DI_WORD    _DIVUCARRY(MAX_DI_TOTAL_NUM, 16)
    static U16 s_swInBak[DI_WORD];
    U08 i,j;
    U16 di_input[_DIVUCARRY(MAX_DI_TOTAL_NUM, 16)];
	U16 sw_switch_temp;
	U16 sr_switch_temp;

	/* ������ */
    PunpGetDi(di_input, MAX_DI_TOTAL_NUM);

    /* �����ʼ�� */
    if (FgFirstEnterTest)
    {
        for(i=0; i<DI_WORD; i++)
        {
            DI_dinput(i) = di_input[i];
            s_swInBak[i] = DI_dinput(i);
        }
    }

    /* ����������� */
    for(i=0; i<DI_WORD; i++)
    {
        DI_dinput(i) = di_input[i];
        if(DI_dinput(i) != s_swInBak[i])
        {
            for(j=0; j<8; j++)
            {
                if (GETBIT(DI_dinput(i), j) != GETBIT(s_swInBak[i], j))
                {
                    TestSwInChange(i*16+j)++;
                }
            }
            s_swInBak[i] = DI_dinput(i);
        }
    }
	/* ���� */
    PunpGetSwitch(&sw_switch_temp, MAX_SW_BIT);
	SW_SWITCH = sw_switch_temp;

	/* ���� */
	PunpGetSwPl(&sr_switch_temp, MAX_SWPL_BIT);
	SR_SWITCH = sr_switch_temp;
//    THR_PHASE = PunpGetPhase();	/* N0004 */

    /* ��������� */
    if (TCmdClrSwInRcd == 1)
    {
        TCmdClrSwInRcd = 0;
        MEMCLR(&TestSwInChange(0), MAX_DI_NUM*sizeof(TestSwInChange(0)));
    }

}

/*******************************************************************************
 �� ��: TEST_SwOut()
 �� ��: ����̵����������
 �� ��: ��
 �� ��: ��
 �� ��: ���Լ̵������ģʽ
        00H��ȫ��/�˳��̵�������
        01H��ȫ��
        02H��700ms8��һ�齻�����
        03H��1000ms16��һ�齻�����
        04H��700ms���ֽڼ�����
        05H��700ms������(���ʱʹ��)
        06H��1000msȫ��ȫ���л�
        07H: 500ms������(��������ʹ�ã�Ҫ���ٶȿ��)
*******************************************************************************/
const U08 DO_SEQUENCE[MAX_DO_NUM] =
{
    DO_01,
    DO_15, DO_14, DO_13,
    DO_12, DO_05, DO_06, DO_07, DO_11, DO_10, DO_09, DO_08, DO_04, DO_03, DO_02, DO_18, DO_17, DO_16,
};
static U16 SysSwOut[MAX_DO_W];
//#define	SysSwOut1	SysSwOut0
static void TEST_SwOut(void)
{
    static U32 s_tick = 0;
    U32 dly;
    U08 i;
    static U08 byte_idx = 0;
    static U08 half_idx = 0;
    static U08 s_cntSw = 0;

    switch (TestSwOutMode)
    {
        case 3:
        case 6:
            dly = 1000;
            break;
        case 7:
            dly = 500;
            break;
        default:
            dly = 700;
            break;
    }

    if (s_tick == 0)
    {
        s_tick = SYS_GetTick();
    }
    else if ((SYS_GetTick() - s_tick) >= dly)
    {
        s_tick = SYS_GetTick();
        switch (TestSwOutMode)
        {
            case 0://ȫ��
                SysSwOut[0] = 0;
                SysSwOut[1] = 0;
                break;
            case 1://ȫ��
                SysSwOut[0] = 0xffff;
                SysSwOut[1] = 0xffff;
                break;
            case 2://8��һ��
                ARRCLR(SysSwOut);
                for (i = (byte_idx*8); i < ((byte_idx+1)*8); i++)
                {
                    if (i >= MAX_DO_NUM)  break;
                    SETBIT_N(SysSwOut, DO_SEQUENCE[i]);
                }
                if (byte_idx == (MAX_DO_W*2-1))  byte_idx = 0;
                else                             byte_idx++;
//                if ((SysSwOut[0]==0x00ff) && (SysSwOut[1]==0x0000))
//                {
//                    SysSwOut[0] = 0xff00;
//					SysSwOut[1] = 0x0000;
//                }
//                else if ((SysSwOut[0]==0xff00) && (SysSwOut[1]==0x0000))
//                {
//                    SysSwOut[0] = 0x0000;
//					SysSwOut[1] = 0x00ff;
//                }
//				else if ((SysSwOut[0]==0x0000) && (SysSwOut[1]==0x00ff))
//				{
//                    SysSwOut[0] = 0x0000;
//					SysSwOut[1] = 0xff00;
//				}
//                else
//                {
//                    SysSwOut[0] = 0x00ff;
//					SysSwOut[1] = 0x0000;
//                }
                break;
            case 3://16��һ��
                ARRCLR(SysSwOut);
                for (i = (half_idx?0:16); i < (half_idx?16:MAX_DO_NUM); i++)
                {
                    SETBIT_N(SysSwOut, DO_SEQUENCE[i]);
                }
                half_idx = !half_idx;
//                if (SysSwOut[0]==0xffff)
//                {
//                    SysSwOut[0] = 0x0000;
//					SysSwOut[1] = 0xffff;
//                }
//                else
//                {
//                    SysSwOut[0] = 0xffff;
//					SysSwOut[1] = 0x0000;
//                }
                break;
            case 4://���ֽڽ���
                ARRCLR(SysSwOut);
                for (i = (half_idx?0:1); i < MAX_DO_NUM; i+=2)
                {
                    SETBIT_N(SysSwOut, DO_SEQUENCE[i]);
                }
                half_idx = !half_idx;
//                if ((SysSwOut[0]==0xaaaa) && (SysSwOut[1]==0xaaaa))
//                {
//                    SysSwOut[0] = 0x5555;
//                    SysSwOut[1] = 0x5555;
//                }
//                else
//                {
//                    SysSwOut[0] = 0xaaaa;
//                    SysSwOut[1] = 0xaaaa;
//                }
                break;
            case 5:
            case 7://���
                ARRCLR(SysSwOut);
                if (s_cntSw < MAX_DO_NUM)
                {
                    SETBIT_N(SysSwOut, DO_SEQUENCE[s_cntSw]);
                }
                if (s_cntSw == MAX_DO_NUM-1)  s_cntSw = 0;
                else                          s_cntSw++;

                break;
            case 6://ȫ��ȫ�ս���
                if ((SysSwOut[0]==0xffff) && (SysSwOut[1]==0xffff))
                {
                    SysSwOut[0] = 0;
                    SysSwOut[1] = 0;
                }
                else
                {
                    SysSwOut[0] = 0xffff;
                    SysSwOut[1] = 0xffff;
                }
                break;
        }
     }
	if (TestSwOutMode!=7)
	{
		s_cntSw = 0;
	}
	PunpSetDo(SysSwOut, MAX_DO_NUM);
}

/*******************************************************************************
 �� ��: TEST_Tmp()
 �� ��: �����¶Ȳ���
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
#define MAX_TEMP_CH_NUM		MAX_NTC_NUM
static void TEST_Tmp(void)
{
    I16 temp[MAX_TEMP_CH_NUM];
    U08 i;
    // �궨������ս����ö��,���´�Ԥ������Զ������
#if MAX_TEMP_CH_NUM != MAX_TEMP_NUM
   #error "�¶�̽ͷ���岻һ��!"
#endif
    // ��Ϊ��������,�������Զ��Ż���,���������޷�ͨ������.
    if (MAX_TEMP_CH_NUM != MAX_TEMP_NUM)
    {
        return;
    }

    PunpGetTemp(temp, MAX_TEMP_CH_NUM);

    for (i=0; i<MAX_TEMP_CH_NUM; i++)
    {
        /* ���µ�ǰֵ(ADֱֵ����ADģ���и���) */
        TestCurTmp(i) = fun_pv_div_10(temp[i]);
 //       TestCurTmp(i) = temp[i];


        /* ��ʼ����λ��¼ */
        if (FgFirstEnterTest || (TCmdClrTmpRcd==1)|| (TCmdClrFrepRcd==1))
        {
            TestCurTmpMax(i) = TestCurTmpMin(i) = TestCurTmp(i);
            TestCurTmpAdMax(i) = TestCurTmpAdMin(i) = TestCurTmpAd(i);
        }

        /* �������ֵ����Сֵ */
        if (TestCurTmp(i) > TestCurTmpMax(i))
        {
            TestCurTmpMax(i) = TestCurTmp(i);
        }
        if (TestCurTmp(i) < TestCurTmpMin(i))
        {
            TestCurTmpMin(i) = TestCurTmp(i);
        }

        if (TestCurTmpAd(i) > TestCurTmpAdMax(i))
        {
            TestCurTmpAdMax(i) = TestCurTmpAd(i);
        }
        if (TestCurTmpAd(i) < TestCurTmpAdMin(i))
        {
            TestCurTmpAdMin(i) = TestCurTmpAd(i);
        }
    }
    TCmdClrTmpRcd = 0;
}



/*****************************************************************************
��  �� : TEST_AdcAdjust()
��  �� : ADCУ׼����
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void TEST_AdcAdjust(void)
{
    static U16 ad_bak=0;
    static U08 ad_adjust=FALSE;


    if (MAX_TEMP_CH_NUM < 2)
    {   // ������,������������
        return;
    }

    if (TCmd2ClrAdcAdjust)
    {
        TCmd2ClrAdcAdjust = 0;
        AdCalibrate.sta = ADCAL_STA_NULL;
        AdCalibrate.val = 0;
        ad_adjust = FALSE;
//      ad_bak = 0;
//      ad_adjust_cnt = 0;  // ʱ�䲻����,����ȴ�
    }


    if ((AD_R10000 - AD_CAL_50 <= TestCurTmpAd(1) && TestCurTmpAd(1) <= AD_R10000 + AD_CAL_50)
        && (ad_bak - AD_CAL_3 <= TestCurTmpAd(1) && TestCurTmpAd(1) <= ad_bak + AD_CAL_3))
    {
        if (!ad_adjust && AdCalibrate.sta != ADCAL_STA_SUCC)
        {
            if (ad_adjust_cnt >= 10)  // 10��
            {
                I16 cal_val = (I32)AD_R10000 - (I32)TestCurTmpAd(1);

                if (-AD_CAL_18 <= cal_val && cal_val <= AD_CAL_18)
                {
                    if (cal_val <= -AD_CAL_15)     cal_val = -AD_CAL_15;
                    if (cal_val >= AD_CAL_15)      cal_val = AD_CAL_15;
                    AdCalibrate.sta = ADCAL_STA_SUCC;
                    AdCalibrate.val = cal_val;
                    if (!eprom_wr_near_crc(&AdCalibrate, EEPROMADDR_ADC_CALIBRATE, sizeof(AdCalibrate)))
                    {
                        AdCalibrate.sta = ADCAL_STA_NULL;
                        AdCalibrate.val = 0;
                    }
                }
                else
                {
                    AdCalibrate.sta = ADCAL_STA_ERROR;
                    AdCalibrate.val = 0;
                }

                ad_adjust = TRUE;
            }
        }
    }
    else
    {
        ad_bak = TestCurTmpAd(1);
        ad_adjust_cnt = 0;
    }
}

/*****************************************************************************
��  �� : AI_Adjust()
��  �� :
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
BOOL AI_Adjust(void)
{
#if 0
#define R_4_20  ((U32)15*16*1023)
#define R_0_5   ((U32)60*16*1023)
#define R_0_10  ((U32)30*16*1023)
#define R_ADJ   ((U32)AD_ADJ*100)
#define MAX_ADJ_PNT     3
    static const U16 adj[][MAX_ADJ_PNT]= //У׼������
    {
        {0,   0,    0   },   //
        {600, 1200, 1800},   // 4-20
        {30,  250,  480 },   // 0-5
        {150, 500,  850 },   // 0-10
    };
    static const U32 r[]=
    {
        0,  R_4_20, R_0_5,  R_0_10
    };
    U08 pnt = 0;//У׼��
    static U08 pnt_bak=0xff;
    U08 num = 0xff;
    static U08 num_bak;
    U08 type = 0;
    static U08 type_bak;
    static U08 dly;
    U08 need_init = FALSE;
    static U16 ad_bak;
    U08 i;

    if(F_TIME_BASIC_100MS_BACK)
    {
        if(dly != 0xff) dly++;
    }

    if(TCmdDontAdjAi)
    {
        TestAIAdjNum = 0xff;
        TestAIAdjType = AI_NONE;
    }
    else
    {
        num = 0xff;
        for (i=0; i<MAX_AI_NUM; i++)//��⵱ǰУ׼��һ·
        {
            if(AI_adfilter[i] > (976 - 200))//У׼��С��adΪ0-5v����0.3vʱ��Ϊ976
            {
                num = i;
                break;
            }
        }
        TestAIAdjNum = num;
    }

    if(num != 0xff)
    {
        type = TestAIType(num);
        TestAIAdjType = type;
        if(type == AI_NONE)
            return TRUE;

        if(num != num_bak || type != type_bak)
        {
            num_bak = num;
            type_bak = type;
            need_init = TRUE;
        }
        if(!need_init) //���ʼ������
        {
            for (pnt=0; pnt<MAX_ADJ_PNT; pnt++)
            {
                U16 snd_ad;//���Ա�׼adֵ
                snd_ad = (U32)adj[type][pnt]*r[type]/R_ADJ;
                if(Fun_Range(AI_adfilter[num],  snd_ad, 5))//�ڱ�׼AD��Χ��5%��
                {
                    if(pnt_bak != pnt)
                    {
                        pnt_bak = pnt;
                        dly = 0;
                    }

                    if(dly > 10)//�ȶ�1.0s����ֹ��̫������
                    {
                        U08 stab = TRUE;//�ȶ���־

                        if(ad_bak != AI_adfilter[num])//�ж��ȶ�
                        {
                            if(!Fun_Range(AI_adfilter[num],  ad_bak, 1))//1%����
                            {
                                dly = 0;
                                stab = FALSE;
                            }
                            ad_bak = AI_adfilter[num];
                        }

                        if(stab)//�ȶ�
                        {
                            if(!GETBIT(TestAIAdjStab, pnt))
                            {
                                SETBIT(TestAIAdjStab, pnt);//��λ�ȶ�
                                TestCurAIAdjAd(pnt) = AI_adfilter[num];
                                ad_bak = AI_adfilter[num];
                                break;
                            }
                        }
                    }
                }
            }
            {
                U16 x[MAX_ADJ_PNT];
                U08 adj_succ = TRUE;
                for (pnt=0; pnt<MAX_ADJ_PNT; pnt++)
                {
                    if(!GETBIT(TestAIAdjStab, pnt))
                    {
                        adj_succ = FALSE;
                        break;
                    }
                    x[pnt] = TestCurAIAdjAd(pnt);
                }
                if(adj_succ)//������У׼���
                {
                    if(!GETBIT(TestAIAdjusted, num*4+type))
                    {
                        I32 a = 0, b = 0, k = 0;

                        Fun_LineFitting(x, adj[type], MAX_ADJ_PNT, 3, &k, &a, &b);//3λС��
                        if(HDW_SaveAiAdj(num, type-1, (U16)a, (I16)b))//д��EEPROM
                        {
                            SETBIT(TestAIAdjusted, num*4+type);
                            for (pnt=0; pnt<MAX_ADJ_PNT; pnt++)
                            {
                                TestCurAIAdjVle(pnt) = (AI_read_signal(type,num,TestCurAIAdjAd(pnt))+5)/10;
                            }
                        }
                    }
                }
                else
                {
                    CLRBIT(TestAIAdjusted, num*4+type);
                }
            }
        }
    }
    // ��ʼ������
    if(num == 0xff || need_init || TCmdDontAdjAi)
    {
        dly = 0;
        pnt_bak = 0xff;
        for (pnt=0; pnt<MAX_ADJ_PNT; pnt++)
        {
            CLRBIT(TestAIAdjStab, pnt);
            CLRBIT(TestAIAdjFail, pnt);
            TestCurAIAdjAd(pnt) = 0;
            TestCurAIAdjVle(pnt) = 0;
        }
    }

    return TRUE;
#endif
    return TRUE;
}

#if 0
/*******************************************************************************
 �� ��: TEST_Cur()
 �� ��: �����������
 �� ��: ��
 �� ��: ��
 �� ��: ���ӵ�������
*******************************************************************************/
static void TEST_Cur(void)
{
    U08 i;
    I16 ai_buf[MAX_CUR_NUM];
    I16 cur_type[MAX_CUR_NUM];

//	PunpGetCur(ai_buf, MAX_CUR_NUM);
    for (i=0; i<MAX_CUR_NUM; i++)
	{
		cur_type[i] = TEST_CURR_TYPE;		// ����Ĭ��50A
	}

    PunpGetCur(&ai_buf[0], &cur_type[0], MAX_CUR_NUM);

    for (i=0; i<MAX_CUR_NUM; i++)
    {
        /* ���µ�ǰֵ(ADֱֵ����ADģ���и���) */
        TestCurCur(i) = ai_buf[i];

        /* ��ʼ����λ��¼ */
        if (FgFirstEnterTest || (TCmdClrCurRcd==1))
        {
            TestCurCurMax(i) = TestCurCurMin(i) = TestCurCur(i);
            TestCurCurAdMax(i) = TestCurCurAdMin(i) = TestCurCurAd(i);
        }

        /* �������ֵ����Сֵ */
        if (TestCurCur(i) > TestCurCurMax(i))
        {
            TestCurCurMax(i) = TestCurCur(i);
        }
        if (TestCurCur(i) < TestCurCurMin(i))
        {
            TestCurCurMin(i) = TestCurCur(i);
        }

        if (TestCurCurAd(i) > TestCurCurAdMax(i))
        {
            TestCurCurAdMax(i) = TestCurCurAd(i);
        }
        if (TestCurCurAd(i) < TestCurCurAdMin(i))
        {
            TestCurCurAdMin(i) = TestCurCurAd(i);
        }
    }
    TCmdClrCurRcd = 0;
}
#endif


#if 0
/*******************************************************************************
 �� ��: TEST_AI()
 �� ��: ����ģ�����������
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TEST_AI(void)
{
    U08 i;
	I16 ai_buf[MAX_AI_NUM];

    for(i=0; i<MAX_AI_NUM; i++)  // ��Ӧǰ����Գ����ģ����������������ǰ����Գ���0��0~5V��1��0~10V��2��4~20mA��
    {
        if(TestAIType(i)==0)    gIO.ai_type[i]=AI_05_45V;   // ǰ���0��Ӧ0~5V(HL034Ӳ���ɲ�0~5V)��Ӧ����ѹ��������Ϊ0.5~4.5V
        else                    gIO.ai_type[i]=AI_NONE;
    }

	//PunpGetAi(ai_buf, MAX_AI_NUM);
	PunpGetAi(ai_buf,gIO.ai_type, MAX_AI_NUM);


    for (i=0; i<MAX_AI_NUM; i++)
    {

        TestCurAI(i) = ai_buf[i];

        /* ��ʼ����λ��¼ */
        if (FgFirstEnterTest || (TCmdClrAIRcd==1))
        {
            TestCurAIMax(i) = TestCurAIMin(i) = TestCurAI(i);
            TestCurAIAdMax(i) = TestCurAIAdMin(i) = TestCurAIAd(i);
        }

        /* �������ֵ����Сֵ */
        if (TestCurAI(i) > TestCurAIMax(i))
        {
            TestCurAIMax(i) = TestCurAI(i);
        }
        if (TestCurAI(i) < TestCurAIMin(i))
        {
            TestCurAIMin(i) = TestCurAI(i);
        }

        if (TestCurAIAd(i) > TestCurAIAdMax(i))
        {
            TestCurAIAdMax(i) = TestCurAIAd(i);
        }
        if (TestCurAIAd(i) < TestCurAIAdMin(i))
        {
            TestCurAIAdMin(i) = TestCurAIAd(i);
        }
    }
    TCmdClrAIRcd = 0;
    AI_Adjust();


}
#endif

#if 0
/*******************************************************************************
 �� ��: TEST_Motor()
 �� ��: ����ģ�����������
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TEST_Motor(void)
{
    U08 i;
    /* ������� */
    for (i=0; i<MAX_MOTOR; i++)
    {
//      EEV_STEP_SV gIO.io_step_sv[i] = TestStepSet(i);
        TestStepNow(i) = EEV_STEP_SV;

        /* ��ת */
        if (GETBIT(TestStepCmd, i*2+1))
        {
            CLRBIT(TestStepCmd, i*2+1);
//            EEV_STEP_SV = EEV_STEP_PV+50;
//            if (EEV_STEP_SV > 500) EEV_STEP_SV = 500;
            TestStepSet(i) = EEV_STEP_SV;
        }
        /* ��ת */
        if (GETBIT(TestStepCmd, i*2))
        {
            CLRBIT(TestStepCmd, i*2);
//            EEV_STEP_SV = EEV_STEP_PV-50;
//            if (EEV_STEP_SV < 0) EEV_STEP_SV = 0;
            TestStepSet(i) = EEV_STEP_SV;
        }
    }

}
#endif

/*****************************************************************************
��  �� : TEST_PWM()
��  �� : PWM���ܲ���
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
static void TEST_PWM(void)
{
    U08 i;


    /* PWM */
    for (i=0; i<MAX_PWM; i++)
    {
        #if MAX_PWM > 1
        #error "PWM ·������"
        #endif

        PunpSetPwmFreqDuty(TestPwmOutFreq(i), TestPwmOutDuty(i), i);

        TestPwmInCur(i) = PunpGetPwmInDuty();

        /* ��ʼ����λ��¼ */
        if (FgFirstEnterTest || (TCmdClrPwmInRcd==1))
        {
            TestPwmInMax(i) = TestPwmInMin(i) = TestPwmInCur(i);
        }

        /* �������ֵ����Сֵ */
        if (TestPwmInCur(i) > TestPwmInMax(i))
        {
            TestPwmInMax(i) = TestPwmInCur(i);
        }
        if (TestPwmInCur(i) < TestPwmInMin(i))
        {
            TestPwmInMin(i) = TestPwmInCur(i);
        }

    }
#if 0
    /* pwm2 *//* N0004 */
    {
        U08 fan_pwm_channal[FREQSPEEDNUM] = {CHANNEL_FAN1, CHANNEL_FAN2};
        for (i=0; i<MAX_2PWM; i++)
        {
#ifndef USE_HIT
            PunpSetPwmFreqDuty(TestPwm2OutFreq(i), TestPwm2OutDuty(i), fan_pwm_channal[i]);
#endif
            TestPwm2InCur(i) = PunpGetPwmInput(i);

            /* ��ʼ����λ��¼ */
            if (FgFirstEnterTest || (TCmdClrPwmInRcd==1))
            {
                TestPwm2InMax(i) = TestPwm2InMin(i) = TestPwm2InCur(i);
            }

            /* �������ֵ����Сֵ */
            if (TestPwm2InCur(i) > TestPwm2InMax(i))
            {
                TestPwm2InMax(i) = TestPwm2InCur(i);
            }
            if (TestPwm2InCur(i) < TestPwm2InMin(i))
            {
                TestPwm2InMin(i) = TestPwm2InCur(i);
            }
        }
    }

#ifdef USE_HIT
{
    #define HIT_ADD         0
    #define HIT_SUB         1
    #define HIT_ADJ10       2
    #define HIT_ADJ_DUTY    3
    static U16 swbak;
    static I16 duty[3]={500, 500, 500};
    static I16 freq[3]={100,100,100};
    U16 sw = PunpGetSwitch();
    I16 add=0;
    if (sw != swbak)
    {
        if (GETBIT(swbak, HIT_ADD) ^ GETBIT(sw, HIT_ADD))
        {
            if (GETBIT(sw, HIT_ADJ10))  add = 100;
            else                        add = 10;
        }
        if (GETBIT(swbak, HIT_SUB) ^ GETBIT(sw, HIT_SUB))
        {
            if (GETBIT(sw, HIT_ADJ10))  add = -100;
            else                        add = -10;
        }
        swbak = sw;
    }

    if (add != 0)
    {
        if (GETBIT(sw, HIT_ADJ_DUTY))
        {
            for (i=1; i<3; i++)
            {
                duty[i] += add;
            }
        }
        else
        {
            for (i=1; i<3; i++)
            {
                freq[i] += add;
            }
        }

        PunpSetPwmFreqDuty(freq[1], duty[1], CHANNEL_FAN1);
        PunpSetPwmFreqDuty(freq[2], duty[2], CHANNEL_FAN2);
    }
}




#endif

   // PunpSetPwmFreq(TestPwm2OutFreq(0),PWM_OUT_CHANNEL2);
   // PunpSetPwmDuty(TestPwm2OutDuty(0),PWM_OUT_CHANNEL2);
#endif

    TCmdClrPwmInRcd = 0;

}

/*******************************************************************************
 �� ��: TEST_Time()
 �� ��: ����ʱ�����
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TEST_Time(void)
{
    static U32 s_tick = 0;

    if ((SYS_GetTick() - s_tick) >= 1000)
    {
        s_tick = SYS_GetTick();

        _CNT(test_time_current);
        _CNT(ad_adjust_cnt);

        if (++TestTimeSec >= 60)
        {
            TestTimeSec = 0;
            if (++TestTimeMin >= 60)
            {
                TestTimeMin = 0;
                if (++TestTimeHour >= 24)
                {
                    TestTimeHour = 0;
                    ++TestTimeDay;
                }
            }
        }
    }
}

/*******************************************************************************
 �� ��: TEST_FpgaPwmOut()
 �� ��: ����Լܷ�������������������
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
//static void TEST_FpgaPwmOut(void)
//{
//  if (TCmdFpgaPwmOut == 1)
//  {
//      TCmdFpgaPwmOut = 0;
//      COM_WriteOneBit(COM_INVT, ADDR_PWM_OUT, 1);
//  }
//}
/*******************************************************************************
 �� ��: TEST_Reset_Record()
 �� ��: ��λ������¼
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void  TEST_Reset_Record(void)
{
#if 0
    static U08 temp[3];

    eprom_rd_near_crc(&TestResetPowerCnt, EEPROMADDR_RESET_CNT, 3);

    if(RESET_POWER && RESET_DMP_M && RESET_DMP_1)
    {
        if(TestResetPowerCnt != 0xff)   TestResetPowerCnt++;
    }
    else if(RESET_DMP_M && RESET_DMP_1)
    {
        if(TestResetDMPCnt != 0xff)   TestResetDMPCnt++;
    }
    else if(!RESET_DMP_1
            && (RESET_CPU || RESET_SW || RESET_ESR0
                || RESET_ESR1 || RESET_ESR2 || RESET_WDT
                || RESET_MP ||RESET_DB || RESET_OJCONF1
                || RESET_OJCONF2 || RESET_OJCONF3))
    {
        if(TestResetUserCnt != 0xff)   TestResetUserCnt++;
    }

    eprom_wr_near_crc(&TestResetPowerCnt, EEPROMADDR_RESET_CNT, 3);
#endif
}
/*******************************************************************************
 �� ��: TEST_Reset_ClearRecord()
 �� ��: ���㸴λ����
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static void TEST_Reset_ClearRecord(void)
{
#if 0
    if(TCmdClrReset)
    {
        TestResetPowerCnt = 0;
        TestResetDMPCnt = 0;
        TestResetUserCnt = 0;
        if(eprom_wr_near_crc(&TestResetPowerCnt, EEPROMADDR_RESET_CNT, 3))
        {
            TCmdClrReset = 0;
        }
    }
#endif
}
#if 0
/*******************************************************************************
 �� ��: TEST_CanNet()
 �� ��: ���Գ���
 �� ��: ��
 �� ��: ��
 �� ��: ���ܳ���16��壬����ͬʱ���������ÿ��������ֽڲ��ܳ���128.
*******************************************************************************/
#define RLAN_NUM  16//���ò����滻
static void TEST_CanNet(void)
{
    static U32  cnt_time = 0;
    static U08  send_cyc = 0;
    static U08  commu_num = 1;
    static I16  first_in = 1;
    static U16  module_data[3];
    static U16  module_data1[3];
    static U08  fg_init = TRUE;

    if(SYS_GetTick() < 1000)    return;

    /* ����ģ���ַ */
    if(first_in)
    {
        first_in = 0;
        CN_SetModleTlt(RLAN_NUM );
    }

    if(Testrlannum > MAX_CANNET_TLT) Testrlannum = MAX_CANNET_TLT;
    if(Testrlannum < 2) Testrlannum = 2;

    /* ��ģ���ַΪ1����ģ���ַΪ2~16 */
    if(rlan_addr == 0)
    {
        if(CN_GetMasterCommStatus() == CN_IDLE && SYS_GetTick() - cnt_time >= 200)//wlg20150706
        {
            cnt_time = SYS_GetTick();
            if(commu_num < Testrlannum)
            {
                switch(send_cyc)
                {
                    case 0:
                         /* ��ģ���ȡ��ģ��ĵ�ַ������MODBUS��*/
                         COM_ReadRWwords(COM_CAN, commu_num, 0x011B, 2, &module_data[0],Q_PRIO_HIGH);
                         break;
                    case 1:
                         /* ��ģ��ȡ��ģ��MODBUS���Ŀ�������������MODBUS��*/
                         COM_ReadROwords(COM_CAN, commu_num, 2, 2, &module_data1[0],Q_PRIO_HIGH);
                         break;
                }
                if(++send_cyc > 1)
                {
                    send_cyc = 0;
                    if(++commu_num >= Testrlannum)
                    {
                        commu_num = 1;
                    }
                }
            }
            else if(commu_num >= Testrlannum )//����������󣬸�С�����CAN����
            {
                commu_num = 1;
            }
        }
    }
    else
    {
        if(fg_init)
        {
            fg_init = FALSE;
            CN_SetSlaveCommuOverTime(10);
        }

        if(CN_GetSlaveRecCommErr())
        {
            ;
        }
    }


    /* ���canͨѶ��¼��Ϣ */
    if (TCmdClrCanRcd == 1)
    {
        TCmdClrCanRcd = 0;
        TestCANnetOkCntTbl(0) = 0;
        TestCANnetErrCntTbl(0) = 0;
    }
}
#endif

/*******************************************************************************
�� ��: Test_ex_ram()
�� ��: ����H14����ram�Ƿ��������д
�� ��: ��
�� ��:
�� ��: ��
*******************************************************************************/
static BOOL Test_ex_ram(void)
{
     BOOL fg_ram_is_ok = TRUE;

     /* ��ȡ3����4���ֽ�,SCT����Ҫ����Ϊ���ã������˴�������֤ram�Ƿ���Ч */
     volatile U32 * ptr_1 = (U32 *)0x1FFF8000;
     volatile U32 * ptr_2 = (U32 *)0x2003C000;
     volatile U32 * ptr_3 = (U32 *)0x20043FFC;

     U32 ptr_1_data = 0;
     U32 ptr_2_data = 0;
     U32 ptr_3_data = 0;

     /* �ȶ����������������������ٻ�ԭ��ȥ */
     ptr_1_data = *ptr_1;
     ptr_2_data = *ptr_2;
     ptr_3_data = *ptr_3;

     *ptr_1 = 0x11111111;
     *ptr_2 = 0x22222222;
     *ptr_3 = 0x33333333;

     if (*ptr_1 != 0x11111111
         || *ptr_2 != 0x22222222
         || *ptr_3 != 0x33333333)
     {
         fg_ram_is_ok = FALSE;
     }

     *ptr_1 = ptr_1_data;
     *ptr_2 = ptr_2_data;
     *ptr_3 = ptr_3_data;

     return fg_ram_is_ok;
}

/*******************************************************************************
 �� ��: TEST_Commu_get_4g()
 �� ��: ��ȡ����4Gģ����Ҫ����Ϣ
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void TEST_Commu_get_4g(void)
{
	U08 no = 0;

	test_4g.test_sta = (app_4g_rw.test_sta&0x7fff);		// 4Gģ�����״̬�����λ������
	if (sizeof(test_4g)>10)				// �ɷ����ṹ�嶼��5���ּ�10���ֽڣ������·���Ҫ��λ��־
	{
		SETBIT(test_4g.test_sta, 15);
	}
	test_4g.csq = app_4g_rw.csq;
	test_4g.succ = Uart[COM_4G].pRecord->broadcast + Uart[COM_4G].pRecord->frameOk
						+ Uart[COM_4G].pRecord->frameOkAhead + Uart[COM_4G].pRecord->frameOkBack;
	test_4g.fail = Uart[COM_4G].pRecord->fail;
	test_4g.total = test_4g.succ + test_4g.fail;

	for (no=0; no<MAX_4G_VER_WORD; no++)
	{
		test_4g.ver[no]= app_4g_rw.ver[no];
	}
	for (no=0; no<MAX_4G_SN_WORD; no++)
	{
		test_4g.sn[no]= app_4g_rw.sn[no];
	}
}

/*******************************************************************************
 �� ��: TEST_Commu()
 �� ��: ����ͨѶ����
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
#define UART_TEST_ADDR  2
static U08 test_ver[MAX_VER];
//static U08 test_comver[MAX_VER];

#define UART_TEST_VER_BGN  14
#define UART_TEST_VER_SIZE  2

static void TEST_Commu(void)
{
    U16 *buf = NULL;

    if (power_time < 10)
    {
        /* �������ͨѶ��¼��Ϣ */
        COM_ClrRecord();
    }

	TEST_Commu_get_4g();

    if (COM_QEmpty(COM_INVT) && COM_Idle(COM_INVT)) // ��Ƶ�ڣ���Ӧ�������ϵĴ���3
    {
    	buf = (U16 *)&test_ver[0];
        if (!FgReliability)
        {
            COM_ReadROwords(COM_INVT, 1, 256+16, 16, buf, Q_PRIO_HIGH);
        }
        else
        {
            /* �ɿ����������Ƶ��ͨѶ */
            com_invt_rd(COM_INVT, INVT_ID_1, FA_ADDR(FA_FREQ_POLE), 1, Q_PRIO_LOW);
        }
    }
    /* �������ͨѶ��¼��Ϣ */
    if (TCmdClrUartRcd == 1)
    {
        TCmdClrUartRcd = 0;
        /* �������ͨѶ��¼��Ϣ */
        COM_ClrRecord();
    }
}

/*******************************************************************************
 �� ��: TEST_SwOut_count()
 �� ��: ����̵����������
 �� ��: ��
 �� ��: ��
 �� ��:
*******************************************************************************/
//static void TEST_SwOut_count(void)
//{
//  static U08 s_swInBak;
//  U08 swin;
//  static BIT8 s_swOutBak;
//  BIT8 swout;
//  static U08 flg_break = FALSE;
//  static U32 s_tick[8] = {0};
//  static U32 swIncent[8] ={0};
//  static U32 swoutcent[8]={0};
//  U08 i,j;
//
//  swin = sys_dinput(0);
//
//    swout.bits.b0 = SysCOUT12;
//    swout.bits.b1 = SysCOUT13;
//    swout.bits.b2 = SysCOUT14;
//    swout.bits.b3 = SysCOUT15;
//    swout.bits.b4 = SysCOUT16;
//    swout.bits.b5 = SysCOUT17;
//    swout.bits.b6 = SysCOUT18;
//    swout.bits.b7 = SysCOUT19;
//
//
//  /* �����ʼ�� */
//  if (FgFirstEnterTest)
//  {
//      s_swInBak = swin;
//      s_swOutBak.byte = swout.byte;
//      for(j=0; j<8; j++)
//      {
//          s_tick[j] = SYS_GetTick();
//      }
//  }
//
//  /* ����������� */
//    if(swin != s_swInBak)
//    {
//        for(j=0; j<8; j++)
//        {
//            if ((GETBIT(swin, j) != GETBIT(s_swInBak, j)) && !GETBIT(flg_break,j))
//            {
//                swIncent[j]++;
//            }
//            Testswincount(j) = ((swIncent[j]<<16)|(swIncent[j]>>16));
//        }
//        s_swInBak = swin;
//    }
//
//  /* ����������� */
//    if(swout.byte != s_swOutBak.byte)
//    {
//        for(j=0; j<8; j++)
//        {
//            if ((GETBIT(swout.byte, j) != GETBIT(s_swOutBak.byte, j)) && !GETBIT(flg_break,j))
//            {
//                swoutcent[j]++;
//            }
//            Testswoutcount(j) = ((swoutcent[j]<<16)|(swoutcent[j]>>16));
//
//        }
//        s_swOutBak.byte = swout.byte;
//    }
//
//    for(i=0; i<8; i++)
//    {
//        if(Testswincount(i) == Testswoutcount(i))
//        {
//            s_tick[i] = SYS_GetTick();
//        }
//
//        if((SYS_GetTick() - s_tick[i]) > 500) SETBIT(flg_break,i);
//    }
//
//
//  /* ��������� */
//  if (TCmdClrSwoutcount == 1)
//  {
//      TCmdClrSwoutcount = 0;
//      MEMCLR(&Testswincount(0), 8*sizeof(Testswincount(0)));
//      MEMCLR(&Testswoutcount(0), 8*sizeof(Testswoutcount(0)));
//  }
//
//
//
//}

/*====================================================
		����: ���¶������ֵ
		����: ��
		���: ��
/====================================================*/
static void TEST_ReadSource(void)
{
//    U08 i;
//    I16 cur_type[MAX_CUR_NUM];
//
//    for (i=0; i<MAX_CUR_NUM; i++)
//	{
//		cur_type[i] = TEST_CURR_TYPE;		// ����Ĭ��50A
//	}


//	/* ���ͷ����� */
//  for (i=0; i<MAX_MOTOR; i++)
//  {
//      PunpConfigAi(i, gIO.ai_type[0], 500, 4500);
//  }
//  /* ģ���� */
//  for (i=0; i<MAX_AI_NUM; i++)
//  {
//      PunpConfigAi(i, gIO.ai_type[0], 500, 4500);
//  }

//    PunpGetAi(&gIO.ai[0],&gIO.ai_type[0], MAX_AI_NUM);

//    ARRCLR(SysSwOut);
//    SETBIT_N(SysSwOut, DO_13);
//    PunpSetDo(SysSwOut, MAX_DO_NUM);

	/* �¶� */
	PunpGetTemp(&gIO.temp[0], MAX_TEMP_NUM);
	/* ���� */
//	PunpGetCur(&gIO.cur[0], &cur_type[0], MAX_CUR_NUM);

	/* ������� */
	//gIO.jmp.byte = PunpGetSwitch();
	PunpGetDi(&gIO.input[0], MAX_DI_NUM);
	PunpSetDo(&gIO.output[0], MAX_DO_NUM);
}

/*******************************************************************************
 �� ��: TEST_Main()
 �� ��: ����������
 �� ��: ��
 �� ��: ��
 �� ��: ��TEST_Main()���ӵ���256K������256K�����ã�������˲��Գ���
*******************************************************************************/
void TEST_Main __attribute__((used, section(".ARM.__at_0x00040000")))(void)
{
    U08 byte_board;
//	U16 FgReliability_sw = 0;

    byte_board = HL006A;

    if (Test_ex_ram() == FALSE)
    {
        return;
    }

    //if(HL_OR_EX == EX_BOARD) byte_board =  HL012A_EX;//ֻ��EX��չ������������ߣ����඼�ǽӵأ�

    switch(TestProgStaus)
    {
        case TPS_NO_TEST_CMD:
            if(SYS_GetTick() < 500)
            {
                break;
            }

            if(TestBackboardCmd == TBC_ENTER && power_time < 7)
            {
                TEST_HdwInit();
                TEST_Reset_Record();
                TestProgStaus = TPS_ENTER_TEST_INIT;
            }
            break;
        case TPS_ENTER_TEST_INIT:
            //if(TestBackboardType == byte_board)
            if(1)
            {
                U08 i;
                //TestSpiflashTotalPage = flash_get_total_page();
                TestEepromTotalPage = eprom_get_total_page();
                TestProgStaus = TPS_ENTER_TEST_OK;

                test_time_current = 0;

                for (i=0; i<EEV_ID_MAX; i++)
				{
//				EEV_STEP_SV	pIO->io_step_sv[i] = 0;
//				EEV_STEP_PV	pIO->io_step_pv[i] = 0;
				}

//				// �ɿ�������
//				PunpGetSwitch(&FgReliability_sw, MAX_SW_BIT);
//                if (FgReliability_sw == 0x0003)
//                {
//                    FgReliability = TRUE;
//                }

                /* ͨѶ���Գ�ʼ�� */
                {
                    U08 u;
                    TCmdClrUartRcd = 1;
                    for (u=0; u<MAX_UART; u++)
                    {
                        if (u != COM_BMS) COM_ResetAlarm(u);
                    }

                    COM_Config(COM_INVT, B_9600, T_8N1);
                    COM_InitMaster(COM_INVT, BKBD_INVT_UART2);

                    COM_Config(COM_FCU_FBUS, B_9600, T_8N1);           /* N0004 */
	                COM_InitSlaver(COM_FCU_FBUS, BKBD_INVT_UART1, 1);  //Ĭ�ϴӻ�/* N0004 */
#ifdef COM_4G
                    COM_Config(COM_4G, B_9600, T_8N1);
                    COM_InitSlaver(COM_4G, BKBD_INVT_UART4, 1);
#endif
                }
            }
            else
            {
                TestProgStaus = TPS_TYPE_ERR;
            }
            break;
        case TPS_ENTER_TEST_OK:
            WDG_Refresh();
            TEST_ReadSource();
            TEST_EEPROMAndInit();
            TEST_FlashAndInit();
            TEST_SwIn();
            TEST_SwOut();
//          TEST_SwOut_count();
            TEST_Tmp();
            TEST_AdcAdjust();
//            TEST_Cur();
//            TEST_AI();
//            TEST_Motor();
            TEST_PWM();
            TEST_Time();
            TEST_Reset_ClearRecord();   //NOTE-CXW
            TEST_Commu();
//            TEST_CanNet();
            FgFirstEnterTest = FALSE;
#if 0
            if(TestBackboardType != byte_board)
            {
                TestProgStaus = TPS_TYPE_ERR;
            }
#endif

            //if(power_time < INITIAL_TIME + 10)
            //{
            //    COM_ClrRecord();//wlg20150706
            //}
            break;
        case TPS_TYPE_ERR:
            if(TestBackboardCmd == TBC_ENTER)
            {
                if(TestBackboardType == byte_board)
                {
                    TestProgStaus = TPS_ENTER_TEST_INIT;
                }
            }
            break;

         default:
            break;
    }
}


