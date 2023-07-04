/******************************************************************************

          ��Ȩ���� (C), 2001-2014, �����а��յ��Լ����������޹�˾

 ******************************************************************************
  �� �� ��   : Debug.c
  �� �� ��   : ����
  ��    ��   : // zzp v160a7
  ��������   : 2016��11��8�����ڶ�
  ����޸�   :
  ��������   : ����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��11��8�����ڶ�
    ��    ��   : // zzp v160a7
    �޸�����   : �����ļ�

******************************************************************************/
#define DEBUG_GLOBAL
/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include "Includes.h"

#ifdef USE_HDW_DEBUG
/* Scs_EnableHighPrecOsc ': unreferenced 'static' function */
/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/
extern void HDW_DelayMs(U16 msec);
/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/
U16 ms;
U16 s;
I16 Dbgdebug[20];
#define Dbgdebug0   Dbgdebug[0]
#define Dbgdebug1   Dbgdebug[1]
#define Dbgdebug2   Dbgdebug[2]
#define Dbgdebug3   Dbgdebug[3]
#define Dbgdebug4   Dbgdebug[4]
#define Dbgdebug5   Dbgdebug[5]
#define Dbgdebug6   Dbgdebug[6]
#define Dbgdebug7   Dbgdebug[7]
#define Dbgdebug8   Dbgdebug[8]
#define Dbgdebug9   Dbgdebug[9]
#define Dbgdebug10  Dbgdebug[10]
#define Dbgdebug11  Dbgdebug[11]
#define Dbgdebug12  Dbgdebug[12]
#define Dbgdebug13  Dbgdebug[13]
#define Dbgdebug14  Dbgdebug[14]
#define Dbgdebug15  Dbgdebug[15]
#define Dbgdebug16  Dbgdebug[16]
#define Dbgdebug17  Dbgdebug[17]
#define Dbgdebug18  Dbgdebug[18]
#define Dbgdebug19  Dbgdebug[19]
/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/

void DBG_1ms_Deal(void)
{
    if(++ms >= 1000)
    {
        ms = 0;
        s++;
    }
}

void DBG_Eeprom(void)
{
    if (Dbgdebug0 == 1)
    {
        if(eprom_test() == EEPROM_TEST_OK)
        {
            Dbgdebug0 = 0;
        }
    }
}

extern I16 Dbgdebug_xx;
void DBG_TestIO(void)
{
    U16 DO[1];        
    U32 curtime;
    static U32 baktime;
    static U16 cnt;
    static U16 flow = 1;

	Dbgdebug[1] = Dbgdebug_xx;
    if (Dbgdebug1 == -1)
    {
        curtime = HDW_1ms_Tick();

        if (curtime - baktime > 100)
        {
            if(++cnt >= ((1<<(MAX_DO_NUM+1))-1))
                cnt = 0;
            baktime = curtime;
        }
        DO[0] = cnt;
    }
    else if (Dbgdebug1 == -2)
    {
        curtime = HDW_1ms_Tick();

        if (curtime - baktime > 100)
        {
            flow <<= 1;
            if (flow == (1<<MAX_DO_NUM))
                flow = 1;
            baktime = curtime;
        }
        DO[0] = flow;        
    }
    else
    {
        DO[0] = Dbgdebug1;
    }
    
    //PunpSetDo(DO, MAX_DO_NUM);
}

void DBG_TestDelay(void)
{
    U32 systime;

    systime = HDW_1ms_Tick();
    
    HDW_DelayMs(Dbgdebug2);
    
    Dbgdebug3 = HDW_1ms_Tick() - systime;
}

void DBG_TestSw(void)
{
//    Dbgdebug4 = PunpGetSwitch();

//    Dbgdebug5 = PunpGetSwPl();
}

void DGB_TestIntoBoot(void)
{
    static struct {
        U08 opt;
    }dbgboot;
    if (dbgboot.opt)
    {
        USER_EnterBoot();
    }
}
void DBG_Main(void)
{
#ifdef USE_HDW_DEBUG
//  DBG_TimeFlag();
    DBG_Eeprom();
    DBG_TestIO();
//  DBG_TestLed();
    DBG_TestDelay();
    DBG_TestSw();
//  DBG_TestSpi();
//  DBG_Test4G();
//  DBG_TestEEV();
//  DGB_TestUart0Echo();
    DGB_TestIntoBoot();
#endif   
#ifdef USE_FUN_DEBUG
    DBG_TestFun();    
#endif
}
#endif

/*******************************************************************************
 �� ��: DBG_get_scan_period()
 �� ��: ��ȡ����ɨ����
 �� ��: num--; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void DBG_get_scan_period(U08 num)
{
	static BOL fg_first[10];
	static U32 s_tick[10];

    /* ˢ�� */
    if (dbg_ScanPeriod_flash == num)
    {
        _SETMAX(dbg_ScanPeriod_flash);
        
        fg_first[num] = FALSE;
    }
    else if (dbg_ScanPeriod_flash == 100)
    {
        _SETMAX(dbg_ScanPeriod_flash);
        
        ARRSET(fg_first, FALSE);
    }

	/* ����ϵͳɨ������ */
	if (!fg_first[num])
	{
		fg_first[num] = TRUE;
        
		dbg_ScanPeriod_Min[num] = 0xffff;
		dbg_ScanPeriod_Max[num] = 0;
		s_tick[num] = SYS_GetTick();
	}
	else
	{
		dbg_ScanPeriod_Cur[num] = (U16)(SYS_GetTick() - s_tick[num]);
		s_tick[num] = SYS_GetTick();
		if (dbg_ScanPeriod_Cur[num] < dbg_ScanPeriod_Min[num])
		{
			dbg_ScanPeriod_Min[num] = dbg_ScanPeriod_Cur[num];
		}
		if (dbg_ScanPeriod_Cur[num] > dbg_ScanPeriod_Max[num])
		{
			dbg_ScanPeriod_Max[num] = dbg_ScanPeriod_Cur[num];
		}
	}

    /* ���� */
    if (debug_kjh_flash == 100)
    {
        _SETMAX(debug_kjh_flash);
        
        ARRSET(debug_kjh, 0);
    }
    else if (debug_kjh_flash != _MAX_(debug_kjh_flash))
    {
        _SETMAX(debug_kjh_flash);
        
        debug_kjh[debug_kjh_flash] = 0;
    }
}

/*******************************************************************************
 �� ��: DBG_get_exec_time()
 �� ��: ��ȡ����ִ��ǰ������ʱ��
 �� ��: num--; 
		en--; 
 �� ��: ��
 �� ��: 
*******************************************************************************/
void DBG_get_exec_time(U08 num, U08 en)
{
    /* ˢ�� */
    if (dbg_ExecTime_flash == num)
    {
        _SETMAX(dbg_ExecTime_flash);
        
        dbg_ExecTime_init[num] = FALSE;
    }
    else if (dbg_ExecTime_flash == 100)
    {
        _SETMAX(dbg_ExecTime_flash);
        
        ARRSET(dbg_ExecTime_init, FALSE);
    }

	/* ����ϵͳɨ������ */
	if (!dbg_ExecTime_init[num])
	{
		dbg_ExecTime_init[num] = TRUE;
        
		dbg_ExecTime_Min[num] = 0xffff;
		dbg_ExecTime_Max[num] = 0;
		dbg_ExecTime_Tick[num] = SYS_GetTick();
	}
	else
	{
        if (en == EXEC_TIME_BGN)
        {
            dbg_ExecTime_Tick[num] = SYS_GetTick();
        }
        else if (en == EXEC_TIME_END)
        {
            dbg_ExecTime_Cur[num] = (U16)(SYS_GetTick() - dbg_ExecTime_Tick[num]);
		
    		if (dbg_ExecTime_Cur[num] < dbg_ExecTime_Min[num])
    		{
    			dbg_ExecTime_Min[num] = dbg_ExecTime_Cur[num];
    		}
    		if (dbg_ExecTime_Cur[num] > dbg_ExecTime_Max[num])
    		{
    			dbg_ExecTime_Max[num] = dbg_ExecTime_Cur[num];
    		}
        }
	}
}

