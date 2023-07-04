/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: Test.c
 文件描述:
 创建人  :
 创建日期:
*******************************************************************************/

#define  TEST_GLOBAL
#include "includes.h"


//static BOOL XDATA SpeakerAct = FALSE;
static BOOL XDATA FgFirstEnterTest = TRUE;

static BOOL XDATA FgReliability = FALSE;	/* 可靠性试验 */

//extern BOOL PT1000_Adjust();
/*******************************************************************************
 函 数: TEST_Enter()
 功 能: 处理进入测试程序
 参 数: 空
 返 回: TRUE/FALSE--成功进入测试/失败
 其 它: 空
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
 函 数: TEST_Exit()
 功 能: 处理退出测试程序
 参 数: 空
 返 回: TRUE/FALSE--成功退出测试/失败
 其 它: 可靠性处理，继电器、喇叭等复位
*******************************************************************************/
//BOOL TEST_Exit(void)
//{
//  if (TestBackboardCmd == TBC_EXIT)
//  {
//      TestProgStaus = TPS_NO_TEST_CMD;
//
//      /* 退出测试时复位处理 */
//
//      /* 复位开关量 */
//      TCmdClrSwInRcd = 1;
//
//
//      /* 复位继电器 */
//      TestSwOutMode = 0;
//      SysSwOut0 = 0;
//      SysSwOut1 = 0;
//
//      /* 复位喇叭 */
//      TCmdChgSpeaker = 0;
//      SpeakerAct = FALSE;
//
//      /* 复位测试时间，以便未重新上电再进入测试程序 */
//      TestTimeSec = TestTimeMin = TestTimeHour = TestTimeDay = 0;
//
//      /* 复位通讯记录 */
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
函  数 : TEST_HdwInit()
功  能 : 测试初始化硬件
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
void TEST_HdwInit(void)
{
    U08 i;

    /* 初始化PWM */
    for (i=0; i<MAX_PWM; i++)
    {
        TestPwmOutFreq(i) = 100;
        TestPwmOutDuty(i) = 100;
    }

    /* 初始化2PWM */
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
 函 数: TEST_EEPROMAndInit()
 功 能: 测试、初始化
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TEST_EEPROMAndInit(void)
{
    enum STATUS //测试及初始化状态
    {
        STATUS_NO_TEST,
        STATUS_TESTING,
        STATUS_SUCCESS,
        STATUS_FAIL,
        STATUS_MAX,
    };

    U08 testStatus;
//    static I16 da_calibra_bak[MAX_AO_CHANNEL][2]={{0,0},{0,0},{0,0}};/* DA校准值(占空比)存储值，存储值只有增量 */
//    static I16 adjustTemp[2] = {0, 0};
    static BOOL fg_test_start = FALSE;  //开始测试标志

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

            /* 测试OK 默认参数初始化 */
            case EEPROM_TEST_OK:
                TestEepromStauts = STATUS_SUCCESS;
                TCmdInitEEPROM = 1;
                //reset_all_data();   //默认初始化一次
                fg_test_start = FALSE;
                break;
            /* 测试失败 */
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


    //初始化命令
    if(TCmdInitEEPROM == 1)
    {
        TCmdInitEEPROM = 0;
        if(!fg_test_start)//wlg20150706
        {
            DataAccess_LimitPara(DATA_INIT);        // 初始化系统维护参数
//            DataAccess_ModbusPara(DATA_INIT);       // 初始化联网通讯参数
            DataAccess_FacPara(DATA_INIT);          // 初始化功能参数

            Password_InitAll();                     // 初始化密码
            DataAccess_CompRuntime(DATA_INIT);      // 初始化压机运行时间
            DataAccess_SysRuntime(DATA_INIT);       // 初始化系统运行时间
//            DataAccess_Timer(DATA_INIT);			// 初始化定时时间
	        DataAccess_HistoryErr(DATA_INIT);		// 初始化历史故障信息
	        DataAccess_Language(DATA_INIT);	        // 初始化语言信息
	        DataAccess_Temp_unit(DATA_INIT);	        // 初始化语言信息
	        DataAccess_InvtExceptInfo(DATA_INIT, _MAX_(U08));	// 初始化变频异常信息
//	        DataAccess_SkipPreheatCnt(DATA_INIT);	        /* 初始化跳过预热计数 */
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
//        case ACTION_START:      // 动作执行开始
//        case ACTION_ING:        // 动作执行中
//            InitEepromStauts = STATUS_TESTING;
//            break;
//
//        case ACTION_SUCC:   // 动作执行成功
//            InitEepromStauts = STATUS_SUCCESS;
//            break;
//        case ACTION_FAIL:   // 动作执行失败
//            InitEepromStauts = STATUS_FAIL;
//            break;
//    }
    if(GET_INIT_PARA()>=ACTION_ING) InitEepromStauts = GET_INIT_PARA()-1;
    else                            InitEepromStauts = STATUS_NO_TEST;
}



/*******************************************************************************
 函 数: TEST_FlashAndInit()
 功 能: FLASH测试、初始化
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TEST_FlashAndInit(void)
{
#if 0
    enum STATUS //测试及初始化状态
    {
        STATUS_NO_TEST,
        STATUS_TESTING,
        STATUS_SUCCESS,
        STATUS_FAIL,
        STATUS_MAX,
    };

    U16 testStatus;
    static BOOL fg_test_start = FALSE;  //开始测试标志

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

            /* 测试OK */
            case FLASH_TEST_OK:
                TestSpiflashStauts = STATUS_SUCCESS;
                fg_test_start = FALSE;
                break;

            /* 测试失败 */
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
 函 数: TEST_SwIn()
 功 能: 处理开关量输入测试
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/

static void TEST_SwIn(void)
{
    #define DI_WORD    _DIVUCARRY(MAX_DI_TOTAL_NUM, 16)
    static U16 s_swInBak[DI_WORD];
    U08 i,j;
    U16 di_input[_DIVUCARRY(MAX_DI_TOTAL_NUM, 16)];
	U16 sw_switch_temp;
	U16 sr_switch_temp;

	/* 开关量 */
    PunpGetDi(di_input, MAX_DI_TOTAL_NUM);

    /* 处理初始化 */
    if (FgFirstEnterTest)
    {
        for(i=0; i<DI_WORD; i++)
        {
            DI_dinput(i) = di_input[i];
            s_swInBak[i] = DI_dinput(i);
        }
    }

    /* 处理跳变次数 */
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
	/* 拨码 */
    PunpGetSwitch(&sw_switch_temp, MAX_SW_BIT);
	SW_SWITCH = sw_switch_temp;

	/* 拨盘 */
	PunpGetSwPl(&sr_switch_temp, MAX_SWPL_BIT);
	SR_SWITCH = sr_switch_temp;
//    THR_PHASE = PunpGetPhase();	/* N0004 */

    /* 清跳变次数 */
    if (TCmdClrSwInRcd == 1)
    {
        TCmdClrSwInRcd = 0;
        MEMCLR(&TestSwInChange(0), MAX_DI_NUM*sizeof(TestSwInChange(0)));
    }

}

/*******************************************************************************
 函 数: TEST_SwOut()
 功 能: 处理继电器输出测试
 参 数: 空
 返 回: 空
 其 它: 测试继电器输出模式
        00H：全闭/退出继电器测试
        01H：全开
        02H：700ms8个一组交替输出
        03H：1000ms16个一组交替输出
        04H：700ms半字节间隔输出
        05H：700ms逐个输出(打机时使用)
        06H：1000ms全开全闭切换
        07H: 500ms逐个输出(生产测试使用，要求速度快点)
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
            case 0://全闭
                SysSwOut[0] = 0;
                SysSwOut[1] = 0;
                break;
            case 1://全开
                SysSwOut[0] = 0xffff;
                SysSwOut[1] = 0xffff;
                break;
            case 2://8个一组
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
            case 3://16个一组
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
            case 4://半字节交换
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
            case 7://逐个
                ARRCLR(SysSwOut);
                if (s_cntSw < MAX_DO_NUM)
                {
                    SETBIT_N(SysSwOut, DO_SEQUENCE[s_cntSw]);
                }
                if (s_cntSw == MAX_DO_NUM-1)  s_cntSw = 0;
                else                          s_cntSw++;

                break;
            case 6://全开全闭交替
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
 函 数: TEST_Tmp()
 功 能: 处理温度测试
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
#define MAX_TEMP_CH_NUM		MAX_NTC_NUM
static void TEST_Tmp(void)
{
    I16 temp[MAX_TEMP_CH_NUM];
    U08 i;
    // 宏定义的最终结果是枚举,导致此预处理永远不成立
#if MAX_TEMP_CH_NUM != MAX_TEMP_NUM
   #error "温度探头定义不一致!"
#endif
    // 改为常量条件,编译器自动优化掉,若成立则无法通过测试.
    if (MAX_TEMP_CH_NUM != MAX_TEMP_NUM)
    {
        return;
    }

    PunpGetTemp(temp, MAX_TEMP_CH_NUM);

    for (i=0; i<MAX_TEMP_CH_NUM; i++)
    {
        /* 更新当前值(AD值直接在AD模块中更新) */
        TestCurTmp(i) = fun_pv_div_10(temp[i]);
 //       TestCurTmp(i) = temp[i];


        /* 初始化或复位记录 */
        if (FgFirstEnterTest || (TCmdClrTmpRcd==1)|| (TCmdClrFrepRcd==1))
        {
            TestCurTmpMax(i) = TestCurTmpMin(i) = TestCurTmp(i);
            TestCurTmpAdMax(i) = TestCurTmpAdMin(i) = TestCurTmpAd(i);
        }

        /* 处理最大值、最小值 */
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
函  数 : TEST_AdcAdjust()
功  能 : ADC校准功能
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
void TEST_AdcAdjust(void)
{
    static U16 ad_bak=0;
    static U08 ad_adjust=FALSE;


    if (MAX_TEMP_CH_NUM < 2)
    {   // 若成立,编译器将警告
        return;
    }

    if (TCmd2ClrAdcAdjust)
    {
        TCmd2ClrAdcAdjust = 0;
        AdCalibrate.sta = ADCAL_STA_NULL;
        AdCalibrate.val = 0;
        ad_adjust = FALSE;
//      ad_bak = 0;
//      ad_adjust_cnt = 0;  // 时间不清零,避免等待
    }


    if ((AD_R10000 - AD_CAL_50 <= TestCurTmpAd(1) && TestCurTmpAd(1) <= AD_R10000 + AD_CAL_50)
        && (ad_bak - AD_CAL_3 <= TestCurTmpAd(1) && TestCurTmpAd(1) <= ad_bak + AD_CAL_3))
    {
        if (!ad_adjust && AdCalibrate.sta != ADCAL_STA_SUCC)
        {
            if (ad_adjust_cnt >= 10)  // 10秒
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
函  数 : AI_Adjust()
功  能 :
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
BOOL AI_Adjust(void)
{
#if 0
#define R_4_20  ((U32)15*16*1023)
#define R_0_5   ((U32)60*16*1023)
#define R_0_10  ((U32)30*16*1023)
#define R_ADJ   ((U32)AD_ADJ*100)
#define MAX_ADJ_PNT     3
    static const U16 adj[][MAX_ADJ_PNT]= //校准三个点
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
    U08 pnt = 0;//校准点
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
        for (i=0; i<MAX_AI_NUM; i++)//检测当前校准那一路
        {
            if(AI_adfilter[i] > (976 - 200))//校准最小的ad为0-5v输入0.3v时，为976
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
        if(!need_init) //需初始化变量
        {
            for (pnt=0; pnt<MAX_ADJ_PNT; pnt++)
            {
                U16 snd_ad;//测试标准ad值
                snd_ad = (U32)adj[type][pnt]*r[type]/R_ADJ;
                if(Fun_Range(AI_adfilter[num],  snd_ad, 5))//在标准AD范围的5%内
                {
                    if(pnt_bak != pnt)
                    {
                        pnt_bak = pnt;
                        dly = 0;
                    }

                    if(dly > 10)//稳定1.0s，防止按太慢出错
                    {
                        U08 stab = TRUE;//稳定标志

                        if(ad_bak != AI_adfilter[num])//判断稳定
                        {
                            if(!Fun_Range(AI_adfilter[num],  ad_bak, 1))//1%以内
                            {
                                dly = 0;
                                stab = FALSE;
                            }
                            ad_bak = AI_adfilter[num];
                        }

                        if(stab)//稳定
                        {
                            if(!GETBIT(TestAIAdjStab, pnt))
                            {
                                SETBIT(TestAIAdjStab, pnt);//置位稳定
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
                if(adj_succ)//三个点校准完成
                {
                    if(!GETBIT(TestAIAdjusted, num*4+type))
                    {
                        I32 a = 0, b = 0, k = 0;

                        Fun_LineFitting(x, adj[type], MAX_ADJ_PNT, 3, &k, &a, &b);//3位小数
                        if(HDW_SaveAiAdj(num, type-1, (U16)a, (I16)b))//写入EEPROM
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
    // 初始化变量
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
 函 数: TEST_Cur()
 功 能: 处理电流测试
 参 数: 空
 返 回: 空
 其 它: 增加电流测试
*******************************************************************************/
static void TEST_Cur(void)
{
    U08 i;
    I16 ai_buf[MAX_CUR_NUM];
    I16 cur_type[MAX_CUR_NUM];

//	PunpGetCur(ai_buf, MAX_CUR_NUM);
    for (i=0; i<MAX_CUR_NUM; i++)
	{
		cur_type[i] = TEST_CURR_TYPE;		// 测试默认50A
	}

    PunpGetCur(&ai_buf[0], &cur_type[0], MAX_CUR_NUM);

    for (i=0; i<MAX_CUR_NUM; i++)
    {
        /* 更新当前值(AD值直接在AD模块中更新) */
        TestCurCur(i) = ai_buf[i];

        /* 初始化或复位记录 */
        if (FgFirstEnterTest || (TCmdClrCurRcd==1))
        {
            TestCurCurMax(i) = TestCurCurMin(i) = TestCurCur(i);
            TestCurCurAdMax(i) = TestCurCurAdMin(i) = TestCurCurAd(i);
        }

        /* 处理最大值、最小值 */
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
 函 数: TEST_AI()
 功 能: 处理模拟量输入测试
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TEST_AI(void)
{
    U08 i;
	I16 ai_buf[MAX_AI_NUM];

    for(i=0; i<MAX_AI_NUM; i++)  // 对应前板测试程序的模拟量类型做调整（前板测试程序0：0~5V、1：0~10V、2：4~20mA）
    {
        if(TestAIType(i)==0)    gIO.ai_type[i]=AI_05_45V;   // 前板的0对应0~5V(HL034硬件可测0~5V)，应用上压力传感器为0.5~4.5V
        else                    gIO.ai_type[i]=AI_NONE;
    }

	//PunpGetAi(ai_buf, MAX_AI_NUM);
	PunpGetAi(ai_buf,gIO.ai_type, MAX_AI_NUM);


    for (i=0; i<MAX_AI_NUM; i++)
    {

        TestCurAI(i) = ai_buf[i];

        /* 初始化或复位记录 */
        if (FgFirstEnterTest || (TCmdClrAIRcd==1))
        {
            TestCurAIMax(i) = TestCurAIMin(i) = TestCurAI(i);
            TestCurAIAdMax(i) = TestCurAIAdMin(i) = TestCurAIAd(i);
        }

        /* 处理最大值、最小值 */
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
 函 数: TEST_Motor()
 功 能: 处理模拟量输出测试
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
static void TEST_Motor(void)
{
    U08 i;
    /* 步进电机 */
    for (i=0; i<MAX_MOTOR; i++)
    {
//      EEV_STEP_SV gIO.io_step_sv[i] = TestStepSet(i);
        TestStepNow(i) = EEV_STEP_SV;

        /* 正转 */
        if (GETBIT(TestStepCmd, i*2+1))
        {
            CLRBIT(TestStepCmd, i*2+1);
//            EEV_STEP_SV = EEV_STEP_PV+50;
//            if (EEV_STEP_SV > 500) EEV_STEP_SV = 500;
            TestStepSet(i) = EEV_STEP_SV;
        }
        /* 反转 */
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
函  数 : TEST_PWM()
功  能 : PWM功能测试
参  数 : 无
返  回 : 无
其  他 : 无
*****************************************************************************/
static void TEST_PWM(void)
{
    U08 i;


    /* PWM */
    for (i=0; i<MAX_PWM; i++)
    {
        #if MAX_PWM > 1
        #error "PWM 路数过多"
        #endif

        PunpSetPwmFreqDuty(TestPwmOutFreq(i), TestPwmOutDuty(i), i);

        TestPwmInCur(i) = PunpGetPwmInDuty();

        /* 初始化或复位记录 */
        if (FgFirstEnterTest || (TCmdClrPwmInRcd==1))
        {
            TestPwmInMax(i) = TestPwmInMin(i) = TestPwmInCur(i);
        }

        /* 处理最大值、最小值 */
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

            /* 初始化或复位记录 */
            if (FgFirstEnterTest || (TCmdClrPwmInRcd==1))
            {
                TestPwm2InMax(i) = TestPwm2InMin(i) = TestPwm2InCur(i);
            }

            /* 处理最大值、最小值 */
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
 函 数: TEST_Time()
 功 能: 处理时间测试
 参 数: 空
 返 回: 空
 其 它: 空
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
 函 数: TEST_FpgaPwmOut()
 功 能: 向测试架发送输出高速脉冲的命令
 参 数: 空
 返 回: 空
 其 它: 空
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
 函 数: TEST_Reset_Record()
 功 能: 复位次数记录
 参 数: 空
 返 回: 空
 其 它: 空
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
 函 数: TEST_Reset_ClearRecord()
 功 能: 清零复位次数
 参 数: 空
 返 回: 空
 其 它: 空
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
 函 数: TEST_CanNet()
 功 能: 测试程序
 参 数: 空
 返 回: 空
 其 它: 不能超过16块板，不能同时发两条命令，每条命令的字节不能超过128.
*******************************************************************************/
#define RLAN_NUM  16//可用参数替换
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

    /* 设置模块地址 */
    if(first_in)
    {
        first_in = 0;
        CN_SetModleTlt(RLAN_NUM );
    }

    if(Testrlannum > MAX_CANNET_TLT) Testrlannum = MAX_CANNET_TLT;
    if(Testrlannum < 2) Testrlannum = 2;

    /* 主模块地址为1，从模块地址为2~16 */
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
                         /* 主模块读取从模块的地址，存入MODBUS区*/
                         COM_ReadRWwords(COM_CAN, commu_num, 0x011B, 2, &module_data[0],Q_PRIO_HIGH);
                         break;
                    case 1:
                         /* 主模读取从模块MODBUS区的开关量，并存入MODBUS区*/
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
            else if(commu_num >= Testrlannum )//解决板数设大后，改小引起的CAN死机
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


    /* 清除can通讯记录信息 */
    if (TCmdClrCanRcd == 1)
    {
        TCmdClrCanRcd = 0;
        TestCANnetOkCntTbl(0) = 0;
        TestCANnetErrCntTbl(0) = 0;
    }
}
#endif

/*******************************************************************************
函 数: Test_ex_ram()
功 能: 测试H14扩的ram是否可正常读写
参 数: 空
返 回:
其 它: 空
*******************************************************************************/
static BOOL Test_ex_ram(void)
{
     BOOL fg_ram_is_ok = TRUE;

     /* 抽取3处各4个字节,SCT上需要定义为不用，留给此处用来验证ram是否有效 */
     volatile U32 * ptr_1 = (U32 *)0x1FFF8000;
     volatile U32 * ptr_2 = (U32 *)0x2003C000;
     volatile U32 * ptr_3 = (U32 *)0x20043FFC;

     U32 ptr_1_data = 0;
     U32 ptr_2_data = 0;
     U32 ptr_3_data = 0;

     /* 先读出来备份起来，测试完再还原回去 */
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
 函 数: TEST_Commu_get_4g()
 功 能: 获取测试4G模组需要的信息
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void TEST_Commu_get_4g(void)
{
	U08 no = 0;

	test_4g.test_sta = (app_4g_rw.test_sta&0x7fff);		// 4G模组测试状态的最高位不可用
	if (sizeof(test_4g)>10)				// 旧方案结构体都是5个字即10个字节，采用新方案要置位标志
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
 函 数: TEST_Commu()
 功 能: 处理通讯测试
 参 数: 空
 返 回: 空
 其 它: 空
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
        /* 清除串口通讯记录信息 */
        COM_ClrRecord();
    }

	TEST_Commu_get_4g();

    if (COM_QEmpty(COM_INVT) && COM_Idle(COM_INVT)) // 变频口，对应测试屏上的串口3
    {
    	buf = (U16 *)&test_ver[0];
        if (!FgReliability)
        {
            COM_ReadROwords(COM_INVT, 1, 256+16, 16, buf, Q_PRIO_HIGH);
        }
        else
        {
            /* 可靠性试验与变频器通讯 */
            com_invt_rd(COM_INVT, INVT_ID_1, FA_ADDR(FA_FREQ_POLE), 1, Q_PRIO_LOW);
        }
    }
    /* 清除串口通讯记录信息 */
    if (TCmdClrUartRcd == 1)
    {
        TCmdClrUartRcd = 0;
        /* 清除串口通讯记录信息 */
        COM_ClrRecord();
    }
}

/*******************************************************************************
 函 数: TEST_SwOut_count()
 功 能: 处理继电器输出计数
 参 数: 空
 返 回: 空
 其 它:
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
//  /* 处理初始化 */
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
//  /* 处理跳变次数 */
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
//  /* 处理跳变次数 */
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
//  /* 清跳变次数 */
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
		功能: 读温度与电流值
		输入: 无
		输出: 无
/====================================================*/
static void TEST_ReadSource(void)
{
//    U08 i;
//    I16 cur_type[MAX_CUR_NUM];
//
//    for (i=0; i<MAX_CUR_NUM; i++)
//	{
//		cur_type[i] = TEST_CURR_TYPE;		// 测试默认50A
//	}


//	/* 膨胀阀步数 */
//  for (i=0; i<MAX_MOTOR; i++)
//  {
//      PunpConfigAi(i, gIO.ai_type[0], 500, 4500);
//  }
//  /* 模拟量 */
//  for (i=0; i<MAX_AI_NUM; i++)
//  {
//      PunpConfigAi(i, gIO.ai_type[0], 500, 4500);
//  }

//    PunpGetAi(&gIO.ai[0],&gIO.ai_type[0], MAX_AI_NUM);

//    ARRCLR(SysSwOut);
//    SETBIT_N(SysSwOut, DO_13);
//    PunpSetDo(SysSwOut, MAX_DO_NUM);

	/* 温度 */
	PunpGetTemp(&gIO.temp[0], MAX_TEMP_NUM);
	/* 电流 */
//	PunpGetCur(&gIO.cur[0], &cur_type[0], MAX_CUR_NUM);

	/* 输入输出 */
	//gIO.jmp.byte = PunpGetSwitch();
	PunpGetDi(&gIO.input[0], MAX_DI_NUM);
	PunpSetDo(&gIO.output[0], MAX_DO_NUM);
}

/*******************************************************************************
 函 数: TEST_Main()
 功 能: 测试主程序
 参 数: 空
 返 回: 空
 其 它: 将TEST_Main()链接到后256K，若后256K不可用，则进不了测试程序
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

    //if(HL_OR_EX == EX_BOARD) byte_board =  HL012A_EX;//只有EX扩展板的引脚是拉高，其余都是接地；

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

//				// 可靠性试验
//				PunpGetSwitch(&FgReliability_sw, MAX_SW_BIT);
//                if (FgReliability_sw == 0x0003)
//                {
//                    FgReliability = TRUE;
//                }

                /* 通讯测试初始化 */
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
	                COM_InitSlaver(COM_FCU_FBUS, BKBD_INVT_UART1, 1);  //默认从机/* N0004 */
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


