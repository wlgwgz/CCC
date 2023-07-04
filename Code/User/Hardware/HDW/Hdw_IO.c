/******************************************************************************

          ��Ȩ���� (C), 2001-2014, �����а��յ��Լ����������޹�˾

 ******************************************************************************
  �� �� ��   : Hdw_IO.c
  �� �� ��   : ����
  ��    ��   : // zzp v160a7
  ��������   : 2016��11��8�����ڶ�
  ����޸�   :
  ��������   : IO�ڴ���
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��11��8�����ڶ�
    ��    ��   : // zzp v160a7
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include "Includes.h"
/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/
const I08 PhsTbl[8][8] =
     { /*  00   01  10   11 */
         { 0,  -1,   1,   0 },   //00
         { 1,   0,   1,  -1 },   //01
         {-1,  -1,   0,   1 },   //10
         { 0,   1,  -1,   0 },   //11
     };
/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/
U16 Hdw_Input[INPUT_W];
U16 Hdw_HDI[MAX_HDI];
U16 Hdw_DO[MAX_DO_W];
U08 Hdw_Phase;
U16 Hdw_Switch;
U16 Hdw_SwPl;
static U08 IN_time[INPUT_NUM]; /* ����ʱ�� */

void HDW_GetInput(void)
{
    U16 i;
    U16 IN_tmp[INPUT_W];            /* ��ʱ��ȡ���� */
    static U16 IN_bak[INPUT_W];/* ���� */

    ARRCLR(IN_tmp);

    #if IN_74HC165_BITS > 0
    {
        U16 In_74HC165[IN_74HC165_W];
        //4 ��74HC165
        MCU_GetInput74HC165(In_74HC165);
        for (i=0; i<IN_74HC165_BITS; i++)
        {
            if (GETBIT_N(In_74HC165, i))
            {
                SETBIT_N(IN_tmp, i);
            }
        }
    }
    #endif

    #if IN_PARALLEL_BITS > 0
    {
        U16 In_Parallel[IN_PARALLEL_W];
        //4 ������Input
        MCU_GetInputParallel(In_Parallel);
        for (i=0; i<IN_PARALLEL_BITS; i++)
        {
            if (GETBIT_N(In_Parallel, i))
            {
                SETBIT_N(IN_tmp, i+IN_74HC165_BITS);
            }
        }
    }
    #endif

    //4 �˲�
    for (i=0; i<INPUT_NUM; i++)
    {
        BOL cur_di = GETBIT(IN_tmp[i>>4], i&0x0f);
        BOL bak_di = GETBIT(IN_bak[i>>4], i&0x0f);
        if(cur_di != bak_di)    
        {
            if(cur_di)  SETBIT(IN_bak[i>>4], i&0x0f);
            else        CLRBIT(IN_bak[i>>4], i&0x0f);
            IN_time[i] = 0;
        }
        else
        {
            if(IN_time[i] != 0xff)  IN_time[i]++;

            if(IN_time[i] >= 10) /* 100ms���� */
            {
                if(cur_di)      
                    SETBIT(Hdw_Input[i>>4], i&0x0f);    
                else            
                    CLRBIT(Hdw_Input[i>>4], i&0x0f);            
            }
        }
    }
    
}

/*****************************************************************************
��  �� : HDW_GetHDI()
��  �� : ��ȡǿ�翪��������,100ms����
��  �� : ��
��  �� : ��
��  �� : ��ֹ����ʧЧ��1ms����һ�Σ��˴������ߵ�ƽ��һ��ȡ����PunpGetDi�д���
*****************************************************************************/
void HDW_GetHDI(void)
{
    U16 i;
    U16 HDI_tmp[MAX_HDI];       /* ��ʱ��ȡ���� */
    static U16 HDI_bak[MAX_HDI];/* ���� */
    static U08 HDI_time[MAX_HDI_NUM]; /* ����ʱ�� */
	static U08 HDI_time1[MAX_HDI_NUM]; /* ����ʧЧ����ʱ�� */
//	static U08 HDI_pulse[MAX_HDI_NUM]; /* ����ʧЧ����������� */
	static U08 HDI_low[MAX_HDI_NUM]; /* ����ʧЧ�͵�ƽ�ɼ����� */
    
    MCU_GetHDi(HDI_tmp);
	
    for (i=0; i<MAX_HDI_NUM; i++)
    {
        BOOL cur_di = GETBIT(HDI_tmp[i>>4], i&0x0f);
        BOOL bak_di = GETBIT(HDI_bak[i>>4], i&0x0f);
        if(cur_di != bak_di)    
        {
            if(cur_di)  SETBIT(HDI_bak[i>>4], i&0x0f);
            else        CLRBIT(HDI_bak[i>>4], i&0x0f);
            HDI_time[i] = 0;
        }
        else
        {
            if(HDI_time[i] != 0xff)  HDI_time[i]++;

            if(HDI_time[i] >= 100) /* 100ms���� */
            {
				HDI_low[i] = 0;
				HDI_time1[i] = 0;
                if(cur_di)
                    SETBIT(Hdw_HDI[i>>4], i&0x0f);
                else
                    CLRBIT(Hdw_HDI[i>>4], i&0x0f);                
            }
        }
		
		if (!cur_di)
		{
			if(HDI_low[i] != 0xff)  HDI_low[i]++;
		}

		if(HDI_time1[i] != 0xff)  HDI_time1[i]++;
		
		if (HDI_time1[i] >= 100) /* 100ms���� */
		{
			HDI_time1[i] = 0;

			if (HDI_low[i] >= 35) // ������ʱ���ڲɼ���35�����ϵ͵�ƽ����Ϊ�պ�
			{
				CLRBIT(Hdw_HDI[i>>4], i&0x0f);
			}
			HDI_low[i] = 0;
		}
    }

#if 0
    for (i=0; i<MAX_HDI_NUM; i++)
    {
        BOOL cur_di = GETBIT(HDI_tmp[i>>4], i&0x0f);
        BOOL bak_di = GETBIT(HDI_bak[i>>4], i&0x0f);
        if(cur_di != bak_di)    
        {
        	if(HDI_pulse[i] != 0xff)  HDI_pulse[i]++;
        	 
            if(cur_di)  SETBIT(HDI_bak[i>>4], i&0x0f);
            else        CLRBIT(HDI_bak[i>>4], i&0x0f);
            HDI_time[i] = 0;
        }

		if (!cur_di)
		{
			if(HDI_low[i] != 0xff)  HDI_low[i]++;
		}

		if(HDI_time1[i] != 0xff)  HDI_time1[i]++;
		
		if (HDI_pulse[i] >= 5)
		{
			if (HDI_time1[i] >= 100) /* 100ms���� */
			{
				HDI_time1[i] = 0;
				if (HDI_low[i] >= 50) // ������ʱ���ڲɼ���50�����ϵ͵�ƽ����Ϊ�պ�
				{
					HDI_pulse[i] = 0;
					HDI_time[i] = 0;
					HDI_low[i] = 0;
					CLRBIT(Hdw_HDI[i>>4], i&0x0f);
				}
			}
		}
        else
        {
            if(HDI_time[i] != 0xff)  HDI_time[i]++;

            if(HDI_time[i] >= 100) /* 100ms���� */
            {
            	HDI_pulse[i] = 0;
				HDI_low[i] = 0;
				HDI_time1[i] = 0;
				
                if(cur_di)
                    SETBIT(Hdw_HDI[i>>4], i&0x0f);
                else
                    CLRBIT(Hdw_HDI[i>>4], i&0x0f);                
            }
        }
    }
#endif
    
}

/*****************************************************************************
��  �� : HDW_SetDo()
��  �� : ����̵���
��  �� : ��
��  �� : ��
��  �� : ����������������,��ÿ����ѭ��Ӧˢ�����
*****************************************************************************/
void HDW_SetDo(void)
{
    U08 i;

    /* �����Ӳ�� */
    MCU_SetDo(Hdw_DO);

    /* ������� */
    for (i=0; i<MAX_DO_W; i++)
    {
        Hdw_DO[i] = 0;
    }
    
}

#if 0
/*****************************************************************************
��  �� : HDW_GetPhase()
��  �� : �������
��  �� : ��
��  �� : ��
��  �� : �ϸ�1ms�жϵ���
*****************************************************************************/
void HDW_GetPhase(void)
{
#define USE_PHASE_FILTER    /* ʹ�������˲� */
    U08 i;
    U08 PhaseNow = 0;
    static I08 PhaseCount;
    static U08 PhaseTimes;
    static U08 PhaseDly;
    static U08 PhaseBak;
    static U08 PhaseOld;
    static BOL fg_First = TRUE;
#ifdef USE_PHASE_FILTER    
    U08 PhaseCnt1 = 0;
    U08 PhaseCnt2 = 0;
    U08 PhaseBuf;

    /* ������ȡ5�� */
    for (i=0; i<MAX_PHASE_SHAKE; i++)
    {
        PhaseBuf = MCU_GetPhase();
        if (PhaseBuf & 0x01) 
        {
            PhaseCnt1++;     
        }
        if (PhaseBuf & 0x02)
        {
            PhaseCnt2++;
        }
    }

    /* ����һ�룬����1 */
    if (PhaseCnt1 > MAX_PHASE_SHAKE/2)
        PhaseNow |= 0x01;
    if (PhaseCnt2 > MAX_PHASE_SHAKE/2)
        PhaseNow |= 0x02;
#else
    PhaseNow = MCU_GetPhase() & 0x03;
#endif

    if (PhaseBak != PhaseNow)
    {
        PhaseBak = PhaseNow;
    }
    else
    {
        if(fg_First)
        {
            fg_First = FALSE;
            PhaseOld = PhaseNow;
            PhaseTimes = 0;
            return;
        }
        else
        {
            if (PhaseOld != PhaseNow)
            {
                PhaseCount += PhsTbl[PhaseNow][PhaseOld];
                PhaseOld = PhaseNow;
            }
            
        }
    }

    /* һ��������� */
    if (++PhaseTimes >= PHASE_TIMES)
    {
        PhaseTimes = 0;
        
        if (PhaseDly != 0xff)   PhaseDly++;

        if (PhaseCount >= PHASE_OK_CNT)
        {
            PhaseDly = 0;
            Hdw_Phase = PHASE_STA_OK;
        }
        else if (PhaseCount <= PHASE_ERR_CNT)
        {
            if (PhaseDly >= 10)/*  ���ࡢ����1��󱨾� */
            {
                Hdw_Phase = PHASE_STA_ERR;
            }
        }
        else
        {
            if (PhaseDly >= 30)/*  ȱ�ࡢ����3���Ӳű��� */
            {
                Hdw_Phase = PHASE_STA_LACK;
            }
        }

        PhaseCount = 0;
    }

}

/*****************************************************************************
��  �� : PunpGetPhase()
��  �� : �����״̬�ӿں���
��  �� : ��
��  �� : �����״̬
��  �� : ��
*****************************************************************************/
U08 PunpGetPhase(void)
{
    return Hdw_Phase;
}
#endif

/*****************************************************************************
��  �� : PunpGetPhase()
��  �� : �����״̬�ӿں���
��  �� : ��
��  �� : �����״̬
��  �� : ��
*****************************************************************************/
U08 PunpGetPhase(void)
{
    return GET_EXIO_PH(0);
}

/*****************************************************************************
��  �� : PunpGetDi()
��  �� : ��ȡ�������ӿں���
��  �� : U16 InputBuf[] : ���������� 
         U16 InputNum   : ����������
��  �� : TRUE�ɹ�/FALSEʧ��
��  �� : ��
*****************************************************************************/
BOL PunpGetDi(U16 InputBuf[], U16 InputNum)
{
    U16 i;
    U16 input_idx;
    // DI��Hdw_Input�е�����
    static const U08 di_idx[MAX_DI_NUM] = {
        7,6,5,4,3,2,1
    };

    if (InputNum > MAX_DI_TOTAL_NUM)
        return FALSE;
    
    for (i=0; i<InputNum; i++)
    {
    	if (i < MAX_DI_NUM)	// ��ͨ����
    	{
	        input_idx = di_idx[i];
	        // �պ�Ϊ0
	        if(GETBIT_N(Hdw_Input, input_idx))
	        {
	            CLRBIT(InputBuf[i>>4], i&0x0f);
	        }
	        else
	        {
	            SETBIT(InputBuf[i>>4], i&0x0f);
	        }
    	}
		else	// ǿ�翪��
		{
			input_idx = i - MAX_DI_NUM;
			// �պ�Ϊ0
			if (GETBIT_N(Hdw_HDI, input_idx))
			{
				CLRBIT(InputBuf[i>>4], i&0x0f);
			}
	        else
	        {
	            SETBIT(InputBuf[i>>4], i&0x0f);
	        }
		}
    }  

    return TRUE;
}

/*****************************************************************************
��  �� : PunpGet4GExist()
��  �� : ��ȡ�Ƿ����4Gģ��
��  �� : ��
��  �� : 0Ϊ��,1Ϊû��
��  �� : ��
*****************************************************************************/
BOL PunpGet4GExist(void)
{
    // 0Ϊ��,1Ϊû��
    return !GETBIT_N(Hdw_Input, 0);
}

/*****************************************************************************
��  �� : PunpCloseAllDo()
��  �� : �ر�����DO���
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void PunpCloseAllDo(void)
{
    ARRCLR(Hdw_DO);
    HDW_SetDo();
}

/*****************************************************************************
��  �� : PunpSetDo()
��  �� : ����̵����ӿں���
��  �� : U16 OutputBuf[] : ������� 
         U16 OutputNum   : �������
��  �� : TRUE�ɹ�/FALSEʧ��
��  �� : ÿ����ѭ��Ӧ����
*****************************************************************************/
BOL PunpSetDo(U16 OutputBuf[], U16 OutputNum)
{
    U16 i;

    if (OutputNum > MAX_DO_NUM)
        return FALSE;
    
    for (i=0; i<OutputNum; i++)
    {
        if(GETBIT(OutputBuf[i>>4], i&0x0f))
            SETBIT(Hdw_DO[i>>4], i&0x0f);
        else
            CLRBIT(Hdw_DO[i>>4], i&0x0f);
    }    

    return TRUE;
}

/*****************************************************************************
��  �� : PunpSetLed()
��  �� : LED����
��  �� : U16 Led : LED���
         U08 Sta : LED״̬
��  �� : ��
��  �� : ��
*****************************************************************************/
BOOL PunpSetLed(U16 Led, U08 Sta)
{
    if (Led >= MAX_LED_NUM)
        return FALSE;
    
    MCU_SetLed(Led, Sta);

    return TRUE;
}

/*****************************************************************************
��  �� : PunpSet4GPower()
��  �� : 4Gģ�鿪�ػ�����
��  �� : BOL onoff : ��ƽ
��  �� : ��
��  �� : ��
*****************************************************************************/
void PunpSet4GPower(BOL onoff)
{
    MCU_Set4GPower(onoff);
}

/*****************************************************************************
��  �� : PunpSet4GReset()
��  �� : 4Gģ�������λ����
��  �� : BOL reset : ��λ��ƽ
��  �� : ��
��  �� : ��
*****************************************************************************/
void PunpSet4GReset(BOL reset)
{
    MCU_Set4GReset(reset);
}

/*******************************************************************************
 �� ��: PunpGetSwitch()
 �� ��: 
 �� ��: SwBuf[]--; 
		SwNum--; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void PunpGetSwitch(U16 SwBuf[], U16 SwNum)
{
    U16 switch_w[MAX_SW]={0};
    U16 i;
    U16 input_idx;
    static const U08 sw_idx[PINS_NUM_SW] = {
        24,25, // SW1
		20,21,22,23, // SW2
		12,13,14,15, // SW4
		8,9,10,11,	 // SW5
    };

	if (SwNum > PINS_NUM_SW)
	{
		return;	
	}
    for (i=0; i<SwNum; i++)
    {
        input_idx = sw_idx[i];
        // onΪ0
        if(GETBIT_N(Hdw_Input, input_idx))
        {
            CLRBIT(switch_w[i>>4], i&0x0f);
        }
        else
        {
            SETBIT(switch_w[i>>4], i&0x0f);
        }
    }    
	for (i=0; i<MAX_SW; i++)
	{
		SwBuf[i] = switch_w[i];
	}
}


/*******************************************************************************
 �� ��: PunpGetSwPl()
 �� ��: 
 �� ��: ��
 �� ��: 
 �� ��: ��
*******************************************************************************/
void PunpGetSwPl(U16 SwPlBuf[], U16 SwPlNum)
{
    U16 switch_w[MAX_SWPL] = {0};
    U16 i;
    U16 input_idx;
    static const U08 swpl_idx[MAX_SWPL_BIT] = {
		16,17,18,19, // SW3 ����
    };

    for (i=0; i<MAX_SWPL_BIT; i++)
    {
        input_idx = swpl_idx[i];
        // onΪ0
        if(GETBIT_N(Hdw_Input, input_idx))
        {
            CLRBIT(switch_w[i>>4], i&0x0f);
        }
        else
        {
            SETBIT(switch_w[i>>4], i&0x0f);
        }
    }    
	for (i=0; i<MAX_SWPL; i++)
	{
		SwPlBuf[i] = switch_w[i];
	}
}

#if MAX_MOTOR_NUM != 0
/*====================================================
		����: �������������1ms�ж��е���
				ֹͣʱ����Ҫ����0.5��
				��ʼʱ����Ҫ����0.5��
				�����ռ�ձ�Ϊ50��
				��������Ϊ32ms
				�����ٶȿɵ����ӵ��ٵ�����
���ڽϸ��ٻ���������ʱ��һ�㲻�ڹ����ٶ��𶯣�
����������Ƶ���٣�һ�����ʧ���������Լ�������ͬʱ�������ֹͣ�Ķ�λ���ȡ�
���������ò�ʹ������״̬������״̬ʱ�񶯴�
		����: ���ڵĲ���
/====================================================*/
//	#define val_motor_fre_type	0
	#define IS_EIGHT_STEP       0 //(val_motor_fre_type == 0)		/* ������� */
	#define IS_FOUR_STEP        1 //(val_motor_fre_type == 1)		/* �������� */

	#define STEP_KEEP		600			/* ����ʱ�� */
//	#define val_motor_step_fre	0
//	#define STEP_SPEED		(moter_fre[val_motor_step_fre])		/* ��������(ms) */

	const  U08  StepTbl[] = { 0x00, 0x09, 0x08, 0x0c, 0x04, 0x06, 0x02, 0x03, 0x01,};		/* ����˳�� */	
	const  U08  moter_fre[] = { 32,  16,  12,   10,   8,   6,   4};	/* ÿ������ʱ��(ms) */
							//{ 31, 62, 83, 100, 125, 166, 250 };	/* PPSÿ����ٲ� */
//	static U08  step_relay = 0;
	static U08  motor_step = 0;					/* �������� */
	static I08  step_idx[MAX_MOTOR] = {0, 0};
	static U16  step_time[MAX_MOTOR] = {0, 0};

void IO_Step(void)
{
	U08  step_out = 0;
	static U08 no = 0;
	static U08 no_next = 0;
	U08 i;
    U08 step_speed_index,step_speed, motor_type;

    step_speed_index = eev_get_step_speed((EEV_ID_ENUM)no);
    step_speed = moter_fre[step_speed_index];
    motor_type   = eev_get_motor_type((EEV_ID_ENUM)no);
	
	for (i=0; i<MAX_MOTOR; i++)  
	{
		if (step_time[i] != 0xffff) step_time[i]++;
	}
	switch (motor_step)
	{
		case 0 :	/* ֹͣ(����) */
			step_out = 0;	
			motor_step = 10;
			break;
			
		case 10:	/* ����һ��������� */
			no = no_next;
			
			if (++no_next >= MAX_MOTOR)
			{
				no_next = 0;
			}

			/* ���Գ����в���������⴦��ȫ��Ϊ0#�������� */
			//if (is_testing) no_next = 0;
			
			if (gIO.io_step_sv[no] != gIO.io_step_pv[no]) {
				motor_step = 1;		step_time[no] = 0;
			}
			break;		

		case 1 :	/* ���ֹ��� */
			step_out = step_idx[no];
			if (step_time[no] >= STEP_KEEP) {
				if (gIO.io_step_sv[no] > gIO.io_step_pv[no]) 	  	motor_step = 2;	/* ��Ҫ��ת */
				else if (gIO.io_step_sv[no] < gIO.io_step_pv[no]) 	motor_step = 3;	/* ��Ҫ��ת */
				else 										  		motor_step = 0;	/* ��Ҫֹͣ */
			}
			break;
		case 2 :	/* ��ת */
			if (gIO.io_step_sv[no] > gIO.io_step_pv[no]) {
				if (step_time[no] >= step_speed /*STEP_SPEED*/) 
                {
					step_time[no] = 0;
					gIO.io_step_pv[no]++;

					if (IS_FOUR_STEP == motor_type) 
                    {
						step_idx[no] += 2;
						if (step_idx[no] > 8) step_idx[no] = 2;
					}
					else 
                    {
						step_idx[no]++;
						if (step_idx[no] > 8) step_idx[no] = 1;
					}
				}
				step_out = step_idx[no];
			}
			else {
				motor_step = 1;		step_time[no] = 0;
				step_out = step_idx[no];
			}
			break;
		case 3 :	/* ��ת */
			if (gIO.io_step_sv[no] < gIO.io_step_pv[no]) {
				if (step_time[no] >= step_speed /*STEP_SPEED*/) 
                {
					step_time[no] = 0;
					gIO.io_step_pv[no]--;

					if (IS_FOUR_STEP == motor_type)
                    {
						step_idx[no] -= 2;
						if (step_idx[no] < 2) step_idx[no] = 8;
					}
					else 
                    {
						step_idx[no]--;
						if (step_idx[no] < 1) step_idx[no] = 8;
					}
				}
				step_out = step_idx[no];
			}
			else {
				motor_step = 1;		step_time[no] = 0;
				step_out = step_idx[no];
			}
			break;
		default :	
			motor_step = 0;
			break;
	}

	if (IS_FOUR_STEP == motor_type) /* ����,ȡ2,4,6,8(������) */
    {               
		if 	(step_out & 0x01) 	step_out += 1;
	}

	step_out = StepTbl[step_out];

    MCU_SetEevStep(no, step_out);
	
	/* ���Գ����в���������⴦��ȫ��Ϊ0#�������� */
#if 0
	if (is_testing) {
		if (no != 0) no = 0;
		DIN_BJ2 = (DIN_BJ1 & 0xf0) | step_out;				/* p4��4λ */
		DIN_BJ3 = (DIN_BJ4 & 0x0f) | (step_out << 4);		/* p2��4λ */
		DIN_BJ4 = (DIN_BJ3 & 0xf0) | step_out;				/* p2��4λ */
		for (i = 1; i < MAX_COMP; i++) gIO.io_step_pv[i] = gIO.io_step_pv[0];
	}
#endif
}

/*****************************************************************************
��  �� : HDW_Step()
��  �� : �������
��  �� : ��
��  �� : ��
��  �� : ��
*****************************************************************************/
void HDW_Step(void)
{
	IO_Step();
}
#endif
