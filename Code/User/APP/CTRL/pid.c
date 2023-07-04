/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: pid.c
 �ļ�����: �򻯰�PID�㷨��
           ΪӦ�ò��ṩPID����ӿڣ���װ"PID�㷨"��
 ������  : hzy
 ��������: 2017.02.08
*******************************************************************************/

#define PIDT_GLOBALS
#include "includes.h"


#define AVG_E1_MAX  60   //��¼e1ƫ������
#define AVG_E1_TIME 20   //��¼e1ƫ�����������Чֵ�˶���һ��ҪС��AVG_E1_MAX
#define AVG_E1_NUM  5   //��¼e1ƫ�������ٽ����ݵ�ƽ��

//PID��Ϣ�ṹ��
typedef struct
{
    //ֻд��
    PID_CTRL_ENUM control; //�����㷨
    I16* pv;    //���Ƶ�
    I16* sv;    //Ŀ��ֵ   
    I16 Ts;     //��������(100ms)
    I16 Kp;     //����ϵ��(0.1)
    I16 Ti;     //����ʱ��(100ms)
    I16 Td;     //΢��ʱ��(100ms)   
    I16 Kc;     //�����ñ�����
    I16 Ai;     //���ٻ��ֳ���(��λ0.1)(��Χ10~100)
//    I16 oldset;    //�����趨�¶ȱ�����趨�¶ȼ�¼
    //ֻ����
    I16 Uk;     //PID����ֵ  
    //���ɶ�д
    I16 e1;     //kʱ�̵�ƫ��
    I16 e2;     //k-1ʱ�̵�ƫ��
    I16 e3;     //k-2ʱ�̵�ƫ��
    I16 e4;     //k-3ʱ�̵�ƫ��
    I16 e5;     //k-4ʱ�̵�ƫ��
    I16 ec1;    //kʱ�̵�ƫ��仯��
    I16 ec2;    //k-1ʱ�̵�ƫ��仯��  
    I16 ec3;    //k-2ʱ�̵�ƫ��仯��  
    I16 ec4;    //k-3ʱ�̵�ƫ��仯��  
    I16 ec5;    //k-4ʱ�̵�ƫ��仯��  

    I32 avebuf[AVG_E1_MAX];     //ƫ������0:���һ��ƫ��
    I32 avemin;     //ƫ��������5�����ݵ���Сƽ��ֵ
    I32 avemax;     //ƫ��������5�����ݵ���Сƽ��ֵ
    I16 avemod;     //�¶��ȶ��������ټ���ֵ
    U16 ave_across_cnt; //����Ŀ��ֵ����
    I16 Ui1;     //�¶��ȶ��������ټ���ֵ
    I16 Ui2;     //�¶��ȶ��������ټ���ֵ

    I16 Up;     //������
    I16 Ui;     //������
    I16 Ud;     //΢����
    I16 Mp;     //�����������
    I16 Mi;     //�����������
    I16 Md;     //΢���������
    U08 cnt_pos;//��ƫ�����
    U08 cnt_neg;//��ƫ�����

    I16 FgMaxUi;     //���ٻ���������ֵ��־

    U08 cnt_highold;//��ƫ���ȶ�ʱ�����
    U08 cnt_lowhold;//��ƫ���ȶ�ʱ�����
    
}PID_STRUCT;

static PID_STRUCT pid[PID_ID_MAX];
static U16 timer_sampling_100ms[PID_ID_MAX];

//PID��Ϣ�ṹ��
typedef enum
{
    PID_OFF, //PID�ر�
    PID_ON   //PID����
}PID_ONOFF_ENUM;
static PID_ONOFF_ENUM pid_onoff[PID_ID_MAX];

//ģ���ʱ
//100ms��ʱ
enum
{
    PID_TIMER_100MS_BAK,   //����
    PID_TIMER_100MS_MAX
};
static U16 pid_timer_100ms[PID_TIMER_100MS_MAX];
//1s��ʱ
enum
{
    PID_TIMER_1S_BAK,   //����
    PID_TIMER_1S_MAX
};
static U16 pid_timer_1s[PID_TIMER_1S_MAX];





/*******************************************************************************
 �� ��: pid_math_bound()
 �� ��: �޶�ֵ�ķ�Χ
 �� ��: v--ֵ; 
        lower--�±߽�; 
        upper--�ϱ߽�; 
 �� ��: �Ƿ񳬽�
 �� ��: ��
*******************************************************************************/
static BOOL pid_math_bound(I16 *v, I16 lower, I16 upper)
{
    BOOL fg_over_bounds = TRUE; //�����־
    
    if (*v > upper)         *v = upper;
    else if (*v < lower)    *v = lower;
    else                    fg_over_bounds = FALSE; //δ����
    
    return  fg_over_bounds;
}

/*******************************************************************************
 �� ��: pid_math_divide()
 �� ��: �����ϴ��������г�������
 �� ��: a1--�ϴε�����(ָ��); 
        a2--������; 
        b--����; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
static I16 pid_math_divide(I16 *a1, I32 a2, I32 b)
{
    I32 a;
    
    //��������Ϊ0
    if (b == 0) b = 1;

    a = (*a1) + a2;
    *a1 = (I16)a % b;   //����
    return (a / b);     //����
}
/*******************************************************************************
 �� ��: pid_ui_aveflexible()
 �� ��: ���ٻ����ж����ٻ�����ƫ��2�����������
 �� ��: id--PID���;updatafg 0:��ʼ�� 1:��������
 �� ��: ��
 �� ��: ʵ���¶��ź���ë����������жϲź���
*******************************************************************************/
void pid_ui_aveflexible(PID_ID_ENUM id,U08 updatafg)
{
    I16 temp[AVG_E1_MAX];
//    static I16 getin_cnt[5]={0,0,0,0,0};  //�����������
    U08 i=0,j=0;

    //����ƫ���
    for((i=AVG_E1_MAX-1);i>0;i--)
    {
        pid[id].avebuf[i] = pid[id].avebuf[i-1];
    }
    pid[id].avebuf[0]= pid[id].e1;
    //����ƽ��ֵ
    for(i=0;i<AVG_E1_MAX-AVG_E1_NUM;i++)
    {
        temp[i] = 0;
        for(j=0;j<AVG_E1_NUM;j++)
        {
           temp[i] += pid[id].avebuf[i+j];
        }
    }
    //if(++getin_cnt[id]>=AVG_E1_TIME) //ÿTS����һ��
    {
        pid[id].avemin = temp[0];
        pid[id].avemax = temp[0];
        //����С���ƽ��ֵ
        for(i=0;i<AVG_E1_TIME;i++)
        {
            if(pid[id].avemin>temp[i]) pid[id].avemin = temp[i];
            if(pid[id].avemax<temp[i]) pid[id].avemax = temp[i];
        }
        pid[id].avemin = pid[id].avemin/AVG_E1_NUM;
        pid[id].avemax = pid[id].avemax/AVG_E1_NUM;
        if(((pid[id].e1>0)&&(pid[id].e2<=0))||((pid[id].e1<0)&&(pid[id].e2>=0))) //�����趨Ŀ��ֵ
        {
            pid[id].avemod = 0;   //��������
            if(pid[id].ave_across_cnt!=0x7fff) pid[id].ave_across_cnt++;   //������������
        }
        else if((pid[id].avemax-pid[id].avemin)<val_ave_stand)
        {
            if(pid[id].avemod!=0x7ff) pid[id].avemod = pid[id].avemod + 1;
        }
        else
        {
            pid[id].avemod = 0;
        }
    }

    // 2018.08.13 zzl���ٻ��ֵ����ò���ƫ��ֵ���޶�
	if(updatafg==0)
	{
        for(i=0;i<AVG_E1_MAX;i++)
        {
            pid[id].avebuf[i] = pid[id].e1;
        }
        pid[id].avemod = 0;
	}
	test_buf[0]=pid[PID_ID_EEV_1].ave_across_cnt;
	test_buf[1]=pid[PID_ID_EEV_2].ave_across_cnt;
	test_buf[2]=pid[PID_ID_EEV_1].avemod;
	test_buf[3]=pid[PID_ID_EEV_2].avemod;
	
}

/*******************************************************************************
 �� ��: pid_ui_flexible()
 �� ��: ���ٻ����ж����ٻ�����ƫ��2�����������
 �� ��: id--PID���;
 �� ��: ��
 �� ��: ʵ���¶��ź���ë����������жϲź���
*******************************************************************************/
void pid_ui_flexible(PID_ID_ENUM id)
{
	if (pid[id].e1 > 20)		
	{
		if (pid[id].ec1 > 0)			/* ƫ��Ϊ���Ҵ����������� */
		{
			if (pid[id].cnt_pos != 0xff)	pid[id].cnt_pos++;
		}
		else if ((pid[id].ec1 <= 0)&&(pid[id].ec2 <= 0)&&(pid[id].ec3 <= 0))		/* �½�����ƫ��������� */
		{
			pid[id].cnt_pos = 0;
		}
		pid[id].cnt_neg = 0;
	}
	else if (pid[id].e1 <-20)
	{
		if (pid[id].ec1 < 0)			/* ƫ��Ϊ���Ҵ����½����� */
		{
			if (pid[id].cnt_neg != 0xff)	pid[id].cnt_neg++;
		}
		else if ((pid[id].ec1 >= 0)&&(pid[id].ec2 >= 0)&&(pid[id].ec3 >= 0))		/* ��������ƫ��������� */
		{
			pid[id].cnt_neg = 0;
		}
		pid[id].cnt_pos = 0;
	}
	else 
	{
		pid[id].cnt_neg = 0;
		pid[id].cnt_pos = 0;
	}
}

/*******************************************************************************
 �� ��: pid_firstsampling()
 �� ��: PID���²���ֵ�״ν���
 �� ��: id--PID���; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
#define AVEGE  5 
void pid_firstsampling(PID_ID_ENUM id)
{
    pid[id].e1  = *pid[id].pv - *pid[id].sv;    //ƫ��
   // pid[id].ec1 = pid[id].e1 - pid[id].e2;      //ƫ��仯��
    pid[id].ec1 = 0;      //ƫ��仯��
    pid[id].ec2 = pid[id].ec1;
    pid[id].ec3 = pid[id].ec2;
    pid[id].ec4 = pid[id].ec3;
    pid[id].ec5 = pid[id].ec4;

    pid[id].e2  = pid[id].e1;  //
    pid[id].e3  = pid[id].e2;  //
    pid[id].e4  = pid[id].e3;  //
    pid[id].e5  = pid[id].e4;  //
}

/*******************************************************************************
 �� ��: pid_sampling()
 �� ��: PID���²���ֵ
 �� ��: id--PID���; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
#define AVEGE  5 
void pid_sampling(PID_ID_ENUM id)
{
    pid[id].ec5 = pid[id].ec4;
    pid[id].ec4 = pid[id].ec3;
    pid[id].ec3 = pid[id].ec2;
    pid[id].ec2 = pid[id].ec1;

    pid[id].e5  = pid[id].e4;  //
    pid[id].e4  = pid[id].e3;  //
    pid[id].e3  = pid[id].e2;  //
    pid[id].e2  = pid[id].e1;  //


    pid[id].e1  = *pid[id].pv - *pid[id].sv;    //ƫ��
    pid[id].ec1 = pid[id].e1 - pid[id].e2;      //ƫ��仯��

  //  pid[id].e5  = pid[id].e4;  //
  //  pid[id].e4  = pid[id].e3;  //
  //  pid[id].e3  = pid[id].e2;  //
  //  pid[id].e2  = pid[id].e1;  //

    
    pid_ui_aveflexible(id,1);
    pid_ui_flexible(id);
}

/*******************************************************************************
 �� ��: pid_control_fuzzy()
 �� ��: ģ��ʽ�㷨
 �� ��: id--PID���; 
 �� ��: ��
 �� ��: ԭʼ�б���ÿ1�ȵ�ƫ���ƫ��仯�ʶ���Ӧ1����λ�ı仯;
*******************************************************************************/
void pid_control_fuzzy(PID_ID_ENUM id)
{
    #define PID_FUZZY_RANGE     21  //ģ���ɵ���Χ
    //��Ҫʹ��ģ��ʽ������3���궨����ø�Ϊ���������뵽PID_STRUCT��
    #define FUZZY_A			10		//E �ı������� 
    #define FUZZY_B			10		//EC�ı������� 
    #define FUZZY_U			10		//Uk�ı�������
    
    const I08 UkMap[PID_FUZZY_RANGE][PID_FUZZY_RANGE] = 
    {
        {   -20, -19, -18, -17, -16, -15, -14, -13, -12, -11, -10,  -9,  -8,  -7,  -6,  -5,  -4,  -3,  -2,  -1,   0   },
        {   -19, -18, -17, -16, -15, -14, -13, -12, -11, -10,  -9,  -8,  -7,  -6,  -5,  -4,  -3,  -2,  -1,   0,   1   },
        {   -18, -17, -16, -15, -14, -13, -12, -11, -10,  -9,  -8,  -7,  -6,  -5,  -4,  -3,  -2,  -1,   0,   1,   2   },
        {   -17, -16, -15, -14, -13, -12, -11, -10,  -9,  -8,  -7,  -6,  -5,  -4,  -3,  -2,  -1,   0,   1,   2,   3   },
        {   -16, -15, -14, -13, -12, -11, -10,  -9,  -8,  -7,  -6,  -5,  -4,  -3,  -2,  -1,   0,   1,   2,   3,   4   },
        {   -15, -14, -13, -12, -11, -10,  -9,  -8,  -7,  -6,  -5,  -4,  -3,  -2,  -1,   0,   1,   2,   3,   4,   5   },
        {   -14, -13, -12, -11, -10,  -9,  -8,  -7,  -6,  -5,  -4,  -3,  -2,  -1,   0,   1,   2,   3,   4,   5,   6   },
        {   -13, -12, -11, -10,  -9,  -8,  -7,  -6,  -5,  -4,  -3,  -2,  -1,   0,   1,   2,   3,   4,   5,   6,   7   },
        {   -12, -11, -10,  -9,  -8,  -7,  -6,  -5,  -4,  -3,  -2,  -1,   0,   1,   2,   3,   4,   5,   6,   7,   8   },
        {   -11, -10,  -9,  -8,  -7,  -6,  -5,  -4,  -3,  -2,  -1,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9   },
        {   -10,  -9,  -8,  -7,  -6,  -5,  -4,  -3,  -2,  -1,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10   },
        {    -9,  -8,  -7,  -6,  -5,  -4,  -3,  -2,  -1,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11   },
        {    -8,  -7,  -6,  -5,  -4,  -3,  -2,  -1,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12   },
        {    -7,  -6,  -5,  -4,  -3,  -2,  -1,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13   },
        {    -6,  -5,  -4,  -3,  -2,  -1,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14   },
        {    -5,  -4,  -3,  -2,  -1,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15   },
        {    -4,  -3,  -2,  -1,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16   },
        {    -3,  -2,  -1,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17   },
        {    -2,  -1,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18   },
        {    -1,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19   },
        {     0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20   },
    };
    
    I16 x, y;
    I16 tmp;
    
    tmp = pid[id].e1 * FUZZY_A / 10;
    x = (tmp + 105) / 10;
    if (x < 0)
    {
        x = 0;
    }
    if (x >= PID_FUZZY_RANGE)
    {
        x = PID_FUZZY_RANGE - 1;
    }
    
    tmp = pid[id].ec1 * FUZZY_B / 10;
    y = (tmp + 105) / 10;
    if (y < 0)
    {
        y = 0;
    }
    if (y >= PID_FUZZY_RANGE)
    {
        y = PID_FUZZY_RANGE - 1;
    }
    
    pid[id].Uk += (I16)UkMap[x][y] * FUZZY_U / 10;
    pid_math_bound(&pid[id].Uk, -pid[id].Kc, pid[id].Kc);
}

/*******************************************************************************
 �� ��: pid_control_position()
 �� ��: λ��ʽ�㷨
 �� ��: pid--PID���; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void pid_control_position(PID_ID_ENUM id)
{
    I32 tmp;
    I16 max, min;
    
    max = pid[id].Kc;   
    min = -pid[id].Kc;
    
    
    if (pid_math_bound(&pid[id].e1, min, max))
    {
        pid[id].Up = pid[id].e1;
        pid[id].Ui = 0;
        pid[id].Mi = 0;
        pid[id].Ud = 0;
        pid[id].Md = 0;   
    }
    else 
    {
        /* ������ */
        pid[id].Up = pid_math_divide(&pid[id].Mp, (I32)pid[id].Kp * pid[id].e1, 10);
        
        /* ������ */
        if (pid[id].Ti == 0)
        {
            pid[id].Ui = 0;
            pid[id].Mi = 0;
        }
        else 
        {
            tmp = (I32)pid[id].e1 * pid[id].Ts;
		    if (pid->cnt_pos > 10 || pid->cnt_neg > 10)	tmp = tmp * pid->Ai / 10;
            pid[id].Ui += pid_math_divide(&pid[id].Mi, tmp, 10*pid[id].Ti);
            max -= pid[id].e1;
            min -= pid[id].e1;
            if (pid[id].Ui > max) pid[id].Ui = max;
            if (pid[id].Ui < min) pid[id].Ui = min;
        }
        
        /* ΢���� */
        if (pid[id].Td == 0)
        {
            pid[id].Ud = 0;
            pid[id].Md = 0;
        }
        else
        {
            tmp = pid[id].ec1 * pid[id].Td;
            pid[id].Ud = pid_math_divide(&pid[id].Md, tmp, pid[id].Ts);
            max -= pid[id].Ui;
            min -= pid[id].Ui;
            if (pid[id].Ud > max) pid[id].Ud = max;
            if (pid[id].Ud < min) pid[id].Ud = min;
        }
    }
    
    pid[id].Uk = pid[id].Up + pid[id].Ui + pid[id].Ud;
    pid_math_bound(&pid[id].Uk, 0, pid[id].Kc);
}

/*******************************************************************************
 �� ��: pid_control_increment()
 �� ��: ����ʽ�㷨
 �� ��: pid--PID���; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
#define DISABLE_D_V  5

void pid_control_increment(PID_ID_ENUM id)
{
    I32 tmp,ec1,ec2,tmp2,tmp3;
//    static U16 temp_getin_cnt[5]={0,0,0,0,0};

    tmp2 = 0;
    tmp3 = 0;
    /* ������ */
     pid[id].Up = pid_math_divide(&pid[id].Mp, (I32)pid[id].Kp * pid[id].ec1, 10);
     pid[id].FgMaxUi = 0;
    /* ������ */
    if (pid[id].Ti == 0)
    {
        pid[id].Ui = 0;
        pid[id].Mi = 0;
    }
    else 
    {
        tmp = (I32)pid[id].e1 * pid[id].Ts;
		if (pid[id].cnt_pos > 10 || pid[id].cnt_neg > 10)	tmp2 = (I32)pid[id].Ai*tmp / 10; //�������
		if (pid[id].avemod >50 )     //50����TS
		{
            
		    pid[id].avemod = MIN(pid[id].avemod, 450);
            tmp3  = (I32)pid[id].avemod*tmp / 30;    // ����

    		tmp3  = (I32)pid[id].Ai*tmp3 / 10;    // �ȶ��Ա���
		}

        pid[id].Ui1 = tmp2/100; //��С100��������20180813
        pid[id].Ui2 = tmp3/100; //��С100��������20180813
        tmp = tmp+tmp2+tmp3;
        pid[id].Ui = pid_math_divide(&(pid[id].Mi), tmp, 10*pid[id].Ti);
    }
    
    /* ΢���� */
    if (pid[id].Td == 0)
    {
        pid[id].Ud = 0;
        pid[id].Md = 0;
    }
    else
    {
        if((pid[id].e1>(-DISABLE_D_V)) && (pid[id].e1<DISABLE_D_V))//WLG V100A01 ƫ����0.5�ڲ�΢��
        {
            ec1 = 0;
            ec2 = 0;
        }
        else
        {
            ec1 = pid[id].ec1;
            ec2 = pid[id].ec2;
        }
        if(val_pid_d_type==0) //�޵�ͨ�˲�
        {
            tmp = (I32)pid[id].Td * (ec1-ec2);
            pid[id].Ud = pid_math_divide(&pid[id].Md, tmp, pid[id].Ts); 
    	}
        else
        {
            tmp = (I32)(4*(ec1-ec2) + pid[id].Ud) * pid[id].Td * 2;
    		pid[id].Ud = pid_math_divide(&pid[id].Md, tmp, (4*pid[id].Ts + 10*pid[id].Td)); //�˴�����Ҫ����zzl 2018.04.10
        }
    }
    
    pid[id].Uk += pid[id].Up + pid[id].Ui + pid[id].Ud;
    pid_math_bound(&pid[id].Uk, -pid[id].Kc, pid[id].Kc);

    //NOTE-CXW PID����
    DebugPID[DEBUG_UK]=pid[0].Uk;
    DebugPID[DEBUG_UP]=pid[0].Up;
    DebugPID[DEBUG_UI]=pid[0].Ui;
    DebugPID[DEBUG_UD]=pid[0].Ud;
    DebugPID[DEBUG_PID_E1]=pid[0].e1;
    DebugPID[DEBUG_PID_EC1]=pid[0].ec1;
    DebugPID[DEBUG_PID_EC2]=pid[0].ec2;
    DebugPID[DEBUG_PID_1UI]=pid[0].Ui1;
    DebugPID[DEBUG_PID_2UI]=pid[0].Ui2;


    DebugEEV_ID_2PID[DEBUG_UK]=pid[1].Uk;
    DebugEEV_ID_2PID[DEBUG_UP]=pid[1].Up;
    DebugEEV_ID_2PID[DEBUG_UI]=pid[1].Ui;
    DebugEEV_ID_2PID[DEBUG_UD]=pid[1].Ud;
    DebugEEV_ID_2PID[DEBUG_PID_E1]=pid[1].e1;
    DebugEEV_ID_2PID[DEBUG_PID_EC1]=pid[1].ec1;
    DebugEEV_ID_2PID[DEBUG_PID_EC2]=pid[1].ec2;
    DebugEEV_ID_2PID[DEBUG_PID_1UI]=pid[1].Ui1;
    DebugEEV_ID_2PID[DEBUG_PID_2UI]=pid[1].Ui2;



    DebugINVT_ID_1PID[DEBUG_UK]=pid[PID_ID_INVT_1].Uk;
    DebugINVT_ID_1PID[DEBUG_UP]=pid[PID_ID_INVT_1].Up;
    DebugINVT_ID_1PID[DEBUG_UI]=pid[PID_ID_INVT_1].Ui;
    DebugINVT_ID_1PID[DEBUG_UD]=pid[PID_ID_INVT_1].Ud;
    DebugINVT_ID_1PID[DEBUG_PID_E1]=pid[PID_ID_INVT_1].e1;
    DebugINVT_ID_1PID[DEBUG_PID_EC1]=pid[PID_ID_INVT_1].ec1;
    DebugINVT_ID_1PID[DEBUG_PID_EC2]=pid[PID_ID_INVT_1].ec2;
    DebugINVT_ID_1PID[DEBUG_PID_1UI]=pid[PID_ID_INVT_1].Ui1;
    DebugINVT_ID_1PID[DEBUG_PID_2UI]=pid[PID_ID_INVT_1].Ui2;



    
}

/*******************************************************************************
 �� ��: pid_timer()
 �� ��: ģ���ʱ����
 �� ��: BOOL TimeFlag ʱ���־���ò������ݣ�������ֲʱ100ms��־���Ʋ�ͬ��
 �� ��: ��
 �� ��: ��100ms��ʱ��������
*******************************************************************************/
void pid_timer(BOOL TimeFlag)
{
    U08 i;
    static U08 cnt_100ms_to_1s = 0; //100���뻻���1��

    if(TimeFlag)
    {
        for (i=0; i<PID_TIMER_100MS_MAX; i++)
        {
            if (pid_timer_100ms[i] != 0xffff) pid_timer_100ms[i]++;
        }
        
        for (i=0; i<PID_ID_MAX; i++)
        {
            if (timer_sampling_100ms[i] != 0xffff) timer_sampling_100ms[i]++;
        }
        
        if (++cnt_100ms_to_1s >= 10)
        {
            cnt_100ms_to_1s = 0;
            for (i=0; i<PID_TIMER_1S_MAX; i++)
            {
                if (pid_timer_1s[i] != 0xffff) pid_timer_1s[i]++;
            }
        }
    }
}

/*******************************************************************************
 �� ��: pid_init()
 �� ��: pid������ʼ��
 �� ��: ��
 �� ��: ��
 �� ��: Ӧ�ó�����Ҫ�ڴ˳�ʼ��֮���������pid����������ᱻ��ʼ������
*******************************************************************************/
void pid_init(void)
{
    U08 i;
    static I16 value = 0;
    
    for (i=0; i<PID_ID_MAX; i++)
    {
        pid_onoff[i] = PID_OFF;//�ر�
        pid[i].control = PID_CTRL_MAX;//��Ч�㷨
        pid[i].pv = &value;//����ֵ
        pid[i].sv = &value;//Ŀ��ֵ
        pid[i].Kc = 1000;//�����ñ�����
        pid[i].Ts = 10;//��������(100ms)
        pid[i].Kp = 1;//����ϵ��
        pid[i].Ti = 100;//����ʱ��
        pid[i].Td = 1;//΢��ʱ��
        pid[i].e1 = 0;//kʱ�̵�ƫ��
        pid[i].e2 = 0;//k-1ʱ�̵�ƫ��
        pid[i].ec1 = 0;//kʱ�̵�ƫ��仯��
        pid[i].ec2 = 0;//k-1ʱ�̵�ƫ��仯��
        pid[i].Up = 0;//������
        pid[i].Ui = 0;//������
        pid[i].Ud = 0;//΢����
        pid[i].Mp = 0;//�����������
        pid[i].Mi = 0;//�����������
        pid[i].Md = 0;//΢���������
        pid[i].Uk = 0;//PID����ֵ
        pid[i].Ai = 10;//���ٻ��ֳ���
        pid[i].cnt_pos = 0;//��ƫ�����
        pid[i].cnt_neg = 0;//��ƫ�����
        pid_ui_aveflexible((PID_ID_ENUM)i,0);
        pid_set_AveAcrossCnt((PID_ID_ENUM)i,0);
    }
}

/*******************************************************************************
 �� ��: pid_prog()
 �� ��: pid�����ȳ���
 �� ��: ��
 �� ��: ��
 �� ��: ����ѭ���е���
*******************************************************************************/
void pid_prog(void)
{
    U08 i;
    U08 tmp=0;
    PID_ID_ENUM p_id;
    for (i=0; i<PID_ID_MAX; i++)
    {
        p_id = (PID_ID_ENUM)i;
        
        if (pid_onoff[i] == PID_ON)
        {
            tmp++;
        }

        /* ֻ�������ҹط������ʹ�ü����߼� */
        if (i != PID_ID_EEV_1
        ||  pid[i].ave_across_cnt>AVG_ACROSS_NUM
        ||  pid[i].e1>0
        ||  CM_IS_UP(INVT_ID_1)				/* Ҫȷ��ÿ��ѭ������ִ�� */
            )
        {
            pid[i].avemod = 0;
        }

        if (timer_sampling_100ms[i] >= pid[i].Ts)
        {
            timer_sampling_100ms[i] = 0;

            if (pid_onoff[i] == PID_ON)
            {
                pid_sampling(p_id);
                
                switch (pid[i].control)
                {
                    case PID_CTRL_POSITION:
                        pid_control_position(p_id);
                        break;
                        
                    case PID_CTRL_INCREMENT:
                        pid_control_increment(p_id);
                        break;
                        
                    case PID_CTRL_FUZZY:
                        pid_control_fuzzy(p_id);
                        break;
                        
                    default:
                        break;
                }   
            }
            DebugPID[DEBUG_PID_USE] = pid_onoff[0];
            DebugEEV_ID_2PID[DEBUG_PID_USE] = pid_onoff[1];
            DebugINVT_ID_1PID[DEBUG_PID_USE] = pid_onoff[PID_ID_INVT_1];
        }
    }
    if(tmp != 0)
    {
        pid_environ_aveflexible(pv_sys_TA,1);
    }
    else
    {
        pid_environ_aveflexible(pv_sys_TA,0);
    }

}

/*******************************************************************************
 �� ��: pid_read_Uk()
 �� ��: ��ȡPID����ֵ
 �� ��: id--PID���; 
 �� ��: PID����ֵ
 �� ��: ����ʽ��ģ��ʽ������ֵ��ÿ�ζ�ȡ�������
*******************************************************************************/
I16 pid_read_Uk(PID_ID_ENUM id)
{
    I16 Uk = 0;

    if (id < PID_ID_MAX)
    {
        Uk = pid[id].Uk;
        
        if (pid[id].control == PID_CTRL_INCREMENT
         || pid[id].control == PID_CTRL_FUZZY)
        {
            pid[id].Uk = 0;
        }
    }
    return Uk;
}
/*******************************************************************************
 �� ��: pid_read_FgMaxUi()
 �� ��: ��ȡPID����ֵ���ٻ��ֻ��۵����ֵ��־
 �� ��: id--PID���; 
 �� ��: 1:���ٻ��ֵ������ֵ  0:���ٻ���û�е������ֵ
 �� ��: ����ʽ��ģ��ʽ������ֵ��ÿ�ζ�ȡ�������
*******************************************************************************/
I16 pid_read_FgMaxUi(PID_ID_ENUM id)
{
    I16 tmp = 0;

    if (id < PID_ID_MAX)
    {
       tmp=pid[id].FgMaxUi;        
    }
    return tmp;
}


/*******************************************************************************
 �� ��: pid_turn_on()
 �� ��: ����PID
 �� ��: id--PID���;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL pid_turn_on(PID_ID_ENUM id)
{
    BOOL result = TRUE;

    if (id >= PID_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        if(pid_onoff[id] == PID_OFF)//�ӹص���Ԥ��һ����������ƫ��仯�����
        {
            pid_firstsampling(id);
            pid[id].Uk = 0;
            pid_ui_aveflexible(id,0);
            pid_set_AveAcrossCnt(id,0);
        }
        pid_onoff[id] = PID_ON;
    }

    return result;
}

/*******************************************************************************
 �� ��: pid_turn_off()
 �� ��: �ر�PID
 �� ��: id--PID���;
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL pid_turn_off(PID_ID_ENUM id)
{
    BOOL result = TRUE;

    if (id >= PID_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        pid_onoff[id] = PID_OFF;
        pid_ui_aveflexible(id,0);
        pid_set_AveAcrossCnt(id,0);
    }

    return result;
}

/*******************************************************************************
 �� ��: pid_set_ctrl()
 �� ��: ���ÿ����㷨
 �� ��: id--PID���; 
        value--�㷨; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL pid_set_ctrl(PID_ID_ENUM id, PID_CTRL_ENUM value)
{
    BOOL result = TRUE;

    if (id >= PID_ID_MAX || value >= PID_CTRL_MAX)
    {
        result = FALSE;
    }
    else
    {
        pid[id].control = value;//�����㷨
    }

    return result;
}

/*******************************************************************************
 �� ��: pid_set_pv()
 �� ��: ���ÿ���Դ
 �� ��: id--PID���; 
        value--���Ƶ�ָ��; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL pid_set_pv(PID_ID_ENUM id, I16* p_pv)
{
    BOOL result = TRUE;
    
    if (id >= PID_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        pid[id].pv = p_pv;//���Ƶ�
    }
    return result;
}

/*******************************************************************************
 �� ��: pid_set_sv()
 �� ��: ����Ŀ��ֵ
 �� ��: id--PID���; 
        value--Ŀ��ֵָ��; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL pid_set_sv(PID_ID_ENUM id, I16* p_sv)
{
    BOOL result = TRUE;
    
    if (id >= PID_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        pid[id].sv = p_sv;//Ŀ��ֵ

#if 0
        if(pid[id].oldset != *p_sv)
        {
            if(id!=PID_ID_EEV_2)
            {
                pid_set_AveAcrossCnt(id,0);//û�п���ʱ���¼���
            }
        }
        pid[id].sv = p_sv;//Ŀ��ֵ
        pid[id].oldset = *p_sv;
#endif
        
    }
    
    return result;
} 

/*******************************************************************************
 �� ��: pid_set_Ts()
 �� ��: ���ò�������
 �� ��: id--PID���; 
        value--��������(100ms); 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL pid_set_Ts(PID_ID_ENUM id, I16 value)
{
    BOOL result = TRUE;
    
    if (id >= PID_ID_MAX || value == 0)
    {
        result = FALSE;
    }
    else
    {
        pid[id].Ts = value;//��������
    }

    return result;
}

/*******************************************************************************
 �� ��: pid_set_Kp()
 �� ��: ���ñ���ϵ��
 �� ��: id--PID���; 
        value--����ϵ��; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL pid_set_Kp(PID_ID_ENUM id, I16 value)
{
    BOOL result = TRUE;
    
    if (id >= PID_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        pid[id].Kp = value;//����ϵ��
    }

    return result;
}

/*******************************************************************************
 �� ��: pid_set_Ti()
 �� ��: ���û���ʱ��
 �� ��: id--PID���; 
        value--����ʱ��(100ms); 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL pid_set_Ti(PID_ID_ENUM id, I16 value)
{
    BOOL result = TRUE;
    
    if (id >= PID_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        pid[id].Ti = value;//����ʱ��
    }

    return result;
}

/*******************************************************************************
 �� ��: pid_set_Td����΢��ʱ��()
 �� ��: ����΢��ʱ��
 �� ��: id--PID���; 
        value--΢��ʱ��(100ms); 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ΢��ʱ����Ϊ0��ʾ��ʹ��΢��
*******************************************************************************/
BOOL pid_set_Td(PID_ID_ENUM id, I16 value)
{
    BOOL result = TRUE;
    
    if (id >= PID_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        pid[id].Td = value;//΢��ʱ��
    }

    return result;
}

/*******************************************************************************
 �� ��: pid_set_Kc()
 �� ��: ���÷����ñ�����
 �� ��: id--PID���; 
        value--��������С(����); 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��"�����ñ�����"�����޶�����ֵUk��С(λ��ʽ)��UK�仯��(����ʽģ��ʽ)
*******************************************************************************/
BOOL pid_set_Kc(PID_ID_ENUM id, I16 value)
{
    BOOL result = TRUE;
    
    if (id >= PID_ID_MAX)
    {
        result = FALSE;
    }
    else
    {
        pid[id].Kc = value;//�����ñ�����
    }

    return result;
}

/*******************************************************************************
 �� ��: pid_get_Kc()
 �� ��: ��ȡ�����ñ�����
 �� ��: id--PID���; 
 �� ��: ������
 �� ��: 
*******************************************************************************/
I16 pid_get_Kc(PID_ID_ENUM id)
{
    I16 result = 0;
    
    if (id >= PID_ID_MAX)
    {
        result = 0;
    }
    else
    {
        result = pid[id].Kc;//�����ñ�����
    }

    return result;
}


/*******************************************************************************
 �� ��: pid_set_Ai()
 �� ��: ���ñ��ٻ��ֳ���
 �� ��: id--PID���; 
        value--���ٻ��ֳ���; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��λ0.1����Χ10~100
*******************************************************************************/
BOOL pid_set_Ai(PID_ID_ENUM id, I16 value)
{
    #define MIN_AI  0   //Ai��Сֵ
    #define MAX_AI  100  //Ai���ֵ
    
    BOOL result = TRUE;
    
    if (id >= PID_ID_MAX || value < MIN_AI || value > MAX_AI)
    {
        result = FALSE;
    }
    else
    {
        pid[id].Ai = value;//���ٻ��ֳ���
    }

    return result;
}
/*******************************************************************************
 �� ��: pid_set_AveAcrossCnt()
 �� ��: ���ô����趨�¶ȴ���
 �� ��: id--PID���; 
        value--���ٻ��ֳ���; 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��:
*******************************************************************************/
BOOL pid_set_AveAcrossCnt(PID_ID_ENUM id, I16 value)
{
    
    BOOL result = TRUE;
    
    if (id >= PID_ID_MAX )
    {
        result = FALSE;
    }
    else
    {
        pid[id].ave_across_cnt = value;//
    }

    return result;
}

U16 pid_get_AveAcrossCnt(PID_ID_ENUM id)
{
    return pid[id].ave_across_cnt;
}

U16 pid_get_avemod(PID_ID_ENUM id)
{
    return pid[id].avemod;
}
/*******************************************************************************
 �� ��: pid_environ_aveflexible()
 �� ��: ���ݻ����¶��Ƿ���Ҳ����ж��Ƿ�Ҫ���¼��ٿ���
               �ж��¶Ⱦ��Ҳ������ݻ����¶���10�����ڲ�����5�漰����
               ��Ҫ����Ӧ�ͻ���ʵ���Ҳ���ʱ���²��������ı仯
 �� ��: judgetemp  ��ǰ�����¶ȵ�λ0.1��; 
              updatafg:0��ʼ��  1:��������
 �� ��:�����¶��Ƿ��������¼�������
 �� ��:  
*******************************************************************************/
#define AVG_ENVIRON_DIF 50     //�����¶�ƫ��ֵ��λ0.1��
#define AVG_ENVIRON_TIME 120   //�����¶ȼ�¼ʱ����������λAVG_ENVIRON_SPACE
#define AVG_ENVIRON_SPACE 5    //�����¶ȼ�¼ʱ������λ1��

void pid_environ_aveflexible(I16 judgetemp,U08 updatafg)
{
//    I16 temp[AVG_E1_MAX];
//    U16 tmp_time;
//    static I16 getin_cnt[5]={0,0,0,0,0};  //�����������
    static I16 environ_buf[AVG_ENVIRON_TIME];
           I16 environ_max,environ_min;

    U08 i=0;
    static U16 tmp_upfg=0;  //�����
    static U16 tmp_upfgbak=0xff;//���±���

    tmp_upfg = ((USER_GetTime(C_1S)%AVG_ENVIRON_SPACE)==0)?1:0;
    
    if((tmp_upfgbak!=tmp_upfg)&&(updatafg==1)) //����洢
    {
        tmp_upfgbak=tmp_upfg;
        if(tmp_upfg)
        {
            //����ƫ���
            for((i=AVG_ENVIRON_TIME-1);i>0;i--)
            {
                environ_buf[i] = environ_buf[i-1];
            }
            environ_buf[0]= judgetemp;
            environ_max = environ_buf[0];
            environ_min = environ_buf[0];
            //����С���ֵ
            for(i=0;i<AVG_ENVIRON_TIME;i++)
            {
                if(environ_min>environ_buf[i]) environ_min = environ_buf[i];
                if(environ_max<environ_buf[i]) environ_max = environ_buf[i];
            }
            //�ж�
            if((environ_max-environ_min)>AVG_ENVIRON_DIF)
            {
                pid_set_AveAcrossCnt(PID_ID_EEV_1,0);
                pid_set_AveAcrossCnt(PID_ID_EEV_2,0);
                pid_set_AveAcrossCnt(PID_ID_INVT_1,0);
            }
        }
    }
    // 
	if(updatafg==0)
	{
	    tmp_upfg = 0;
	    tmp_upfgbak=0xff;
        for(i=0;i<AVG_ENVIRON_TIME;i++)
        {
            environ_buf[i] = judgetemp;
        }
	}
}

