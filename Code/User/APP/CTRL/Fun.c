/*******************************************************************************
 Copyright (c) �����а��յ��Լ����������޹�˾ All rights reserved.
 �ļ�����: FUN.c
 �ļ�����: ͨ�õ��ӳ�������Ŀ�޹ء����˲������ݿɷŵ���׼���С�
 ������  : yx
 ��������: 2014.12.15
*******************************************************************************/
#define  FUN_GLOBAL
#include "includes.h"

/***********************************************
������ fun_erea_devide
���ܣ� ��������ȷ����������򻮷�(����ÿ������زͬ)
������ 
		s_area����ǰ�����, 1 ��Ӧ�������(��������¶ȵ������)
		Tpoint�������¶ȵ�����
		Diff��	�����л��²����飨���������������л�����Ӧ����ŴӸߵ��ͣ�
		PointMax���������ķ����¶ȵ㣨��󲻳���TEMP_POINT_MAX��
		
����ֵ: �����


************************************************/
#define TEMP_POINT_MAX 10  //�����¶ȵ������N���¶ȵ㣬����ΪN+1��������(Ҫ���������õ��˺�����Ӧ��)

U08 fun_erea_devide(I16 p_value, U08 s_area, I16 Tpoint[], I16 Diff[], U08 PointMax) //NOTE-CXW ��������������ĳɵ�ַ����
{
    U08 d_erea,num;
    U08 i,j;

	U08 effect_var = 0;				// ��Ч�����¶ȵ����
    I16 t_set[TEMP_POINT_MAX]={0}; 
    I16 d_set[TEMP_POINT_MAX]={0}; 	
    
    
    d_erea = s_area;
    if (d_erea < 1 || d_erea > (PointMax+1))// ��������Ч��Χ������
	{
		d_erea = 1;		// 1Ϊ�¶��������
	}
	
	if(PointMax>TEMP_POINT_MAX || SensorCannotUse(p_value))	// �������ķ����¶ȵ���󲻳���TEMP_POINT_MAX	 
	{
		return d_erea;
	}
	

	for(i=0; i<PointMax-1; i++)
	{
		if(Tpoint[i]<Tpoint[i+1])	// ��������¶ȵ㲻���ɴ�С���У����˳��ж�����
		{
			return d_erea;	
		}
		t_set[i] = Tpoint[i];
		d_set[i] = Diff[i];
	}
	t_set[i] = Tpoint[i];	// ��ֵ���һ���¶ȵ�
	d_set[i] = Diff[i];

	    
    //����������������غϵķ���������Ϊ0x8000
    for(num=0,i=num+1; i<PointMax; i++)
    {
        if(t_set[num] == (I16)0x8000)//����������ݲ��Ƚ�,�����㷨ִ���ٶ�(�з�����������0xffff(-1),0xff(255))
        {
            i = ++num;//�Ƚ���һ����
            continue;
        }
        else if(t_set[num] == t_set[i])//һ�����ݺͺ�����������Ƚ�
        {
            t_set[i] = 0x8000;
            d_set[i] = 0;	
        }

        if(i == PointMax-1) //�Ƚ϶������һ����Ϊֹ,
        {
            i = ++num;//�Ƚ���һ����
        }

        if(num == PointMax-1) break;//�Ƚϵ����һ�����˳�
    }
	

	    
    //������Ч�����¶ȵ����,�ų�0x8000��Ч����(�Ƶ��������)
    effect_var = PointMax;  

    for(i=0; i<effect_var; i++)//��Ч�����жϣ�������Ч���ݱ��ų�����ⷶΧ��С
    {
        if(t_set[i] == (I16)0x8000)
        {
            if(effect_var>0)
            {
                effect_var--;//��Ч���ݼ���һ�� 
            }
            for(j=i; j<effect_var; j++)
            { 	
            	t_set[j]=t_set[j+1];//������������ǰ�ƶ�һ�Σ�����ƶ������ݺ���effect_var��
            	d_set[j]=d_set[j+1];

            }
            t_set[effect_var] = 0x8000;
            d_set[effect_var] = 0;
			if(i>0)
            {
                i--;    //�����ж�iλ������
			}
        }

    }


	// ���ݵ�ǰֵȷ������
	for(i=0; i<effect_var; i++)	// ֻ������Ч����
	{
		if(d_erea == 1)	// ��ǰ���¶��������
    	{
    		if (p_value <= t_set[0])
    		{
    			d_erea ++;	
    		}
    		else 
    		{	
    			break;	// ����û�仯���˳�for
    		}
    	}
    	else if(d_erea == (PointMax+1))	// ��ǰ���¶��������
    	{
    		if (p_value >= t_set[PointMax-1]+d_set[PointMax-1])
    		{
    			d_erea --;
    		}
    		else 
    		{
    			break;
    		}
    	}
    	else 
    	{
    		if (p_value >= t_set[d_erea-2]+d_set[d_erea-2])
    		{
    			d_erea --;	
    		}
    		else if (p_value <= t_set[d_erea-1])
    		{
    			d_erea ++;	
    		}
    		else	// ����û�仯���˳�for
    		{	
    			break;	
    		}	
    	}
    	
	}
    	
    //��������������
    if (d_erea > effect_var+1)
    {
    	d_erea = effect_var+1;
    }
    
    return d_erea;
}
/*****************************************************************************
��  �� : Fun_LookupArea()
��  �� : �ز��������,֧��0����(�����ڽ�����ͬ)
��  �� : I16 val  : ��ǰֵ
         U08 area : ��ǰ����
         I16 high : �߷�������
         I16 mid  : 
         I16 norm : 
         I16 low  : 
         I16 diff : 
��  �� : ��������
��  �� : by:zzp ˼·:���ж��Ƿ��л�����,�����·���
*****************************************************************************/
U08 Fun_LookupArea(I16 val, U08 area, I16 high, I16 mid, I16 norm, I16 low,I16 diff)
{
    BOL area_ch = FALSE;
    U08 new_area = area;

    //3 �����Ƿ����,��ʱ��֧�ִ������
    if (!(high >= mid && mid >= norm && norm >= low))
    {
        return area;
    }
    
    
    //3 �жϷ����Ƿ�ı�
    switch (area)
    {
        case AREA_1:
            area_ch = (val < high - diff);
            break;
        case AREA_2:
            area_ch = (val < mid - diff || val > high);
            break;
        case AREA_3:
            area_ch = (val < norm - diff || val > mid);
            break;
        case AREA_4:
            area_ch = (val < low - diff || val > norm);
            break;
        case AREA_5:
            area_ch = (val > low);
            break;
        default:
            area_ch = TRUE;
            break;
    }
    

    //3 �����ı�ʱ,���·���
    if (area_ch)
    {
        // ����"<="��Ϊ�˴�������������ʱ
        if (high <= val)                    new_area = AREA_1;
        else if (mid <= val && val <= high) new_area = AREA_2;
        else if (norm <= val && val <= mid) new_area = AREA_3;
        else if (low <= val && val <= norm) new_area = AREA_4;
        else                                new_area = AREA_5;
    }

    return new_area;
}
/*****************************************************************************
��  �� : fun_area()
��  �� : ���ݷ��������
��  �� : I16 val  : ��ǰֵ
         U08 area : ��ǰ����
         I16 diff : �����ز�
         ...      : ������(�ɱ�)
��  �� : �·���
��  �� : 1�����������һ����������ΪSENSOR_NOEXIST,����ʹ��AREA_PT��
         2���׸������ķ���ֵ��0��������һЩ������ķ�����ʼֵΪ1��ͬ
		 3��֧�ַ������ɸߵ�������


                    A��                             SENSOR_NOEXIST             

--------------------------------------------------  p1      --
..................................................  p1-d     | ��:
                                                             | ������ǰ������Χ
                    B��                                      | �����·���
--------------------------------------------------  p2       |
..................................................  p2-d    __

                    C��
--------------------------------------------------  p3
..................................................  p3-d

                    D��                             SENSOR_NOEXIST

                                                    
*****************************************************************************/
U08 fun_area(I16 val, U08 area, I16 diff, ...)
{
//    #define MAX_AREA_POINT  20				/* ���֧�ֵķ������� */
    va_list ap;
    I16 point[MAX_AREA_POINT];
    I16 arg;
    I16 arg_bak = SENSOR_NOEXIST;
    U08 max_area=0;
    U08 i;
    BOL fg_area_changed = FALSE;
    BOL fg_area_err = FALSE;
    U08 new_area = area;

    va_start(ap, diff);

    //3 ����У��
    for (arg=va_arg(ap, int); SensorCanUse(arg) && max_area<MAX_AREA_POINT; arg=va_arg(ap, int))
    {
        if (SensorCanUse(arg_bak) && arg>arg_bak)
        {
            fg_area_err = TRUE;
            break;
        }
        
        point[max_area++] = arg;
        arg_bak = arg;
    }

    //3 �жϷ����Ƿ�ı�
    if (!fg_area_err)
    {
        I16 area_up = 0;
        I16 area_lo = 0;

        //4 ��ǰ������������
        if (area == 0)              area_up = SENSOR_NOEXIST,   area_lo = point[area] - diff;
        else if (area >= max_area)  area_up = point[area-1],    area_lo = SENSOR_NOEXIST;
        else                        area_up = point[area-1],    area_lo = point[area] - diff;
        //4 �ж��Ƿ񳬳�������
        if (   (val < area_lo && SensorCanUse(area_lo))
            || (val > area_up && SensorCanUse(area_up)))
        {
            fg_area_changed = TRUE;
        }
    }

    //3 �����ı�ʱ,���·���
    if (fg_area_changed)
    {
        I16 area_up = 0;
        I16 area_lo = 0;
        
        for (i=0; i<=max_area; i++)
        {
            //4 ÿ��������������
            if (i == 0)             area_up = SENSOR_NOEXIST,   area_lo = point[i];
            else if (i >= max_area) area_up = point[i-1],       area_lo = SENSOR_NOEXIST;
            else                    area_up = point[i-1],       area_lo = point[i];
            //4 �Ƿ����ڸķ�������������
            if (   (val >= area_lo || SensorCannotUse(area_lo))
                && (val <= area_up || SensorCannotUse(area_up)))
            {
                new_area = i;
                break;
            }
        }
    }
    

    va_end(ap);

    return new_area;
}

/****************************************************************
������16-2 ���ز����Է���(fun_line_diff)
���ܣ����������˵㼰�ز�õ����ز�����Է���(y���)��
������x��ǰֵ�����x���ꣻ���y���ꣻ�յ�x���ꣻ�յ�y���ꣻ�ز
���أ�yĿ��ֵ
������/��
*****************************************************************/
#define DIR_POS     0       /* ������ */
#define DIR_NEG     1       /* ������ */

I16 fun_line_diff(I16 value, I16 x_bgn, I16 y_bgn, I16 x_end, I16 y_end, I16 diff)
{
    I16 dest_value = y_bgn;
    I16 temp_value = value;
    static I16 valu_bak = 0;
    static BOOL fg_dir = DIR_POS;           /* ��ʼ��Ϊ������ */
    
    if (x_end > x_bgn && diff >= 0)
    {
        if (value > x_end)
        {
            dest_value = y_end;
            valu_bak = value;
            fg_dir = DIR_POS;
        }
        else if (value < x_bgn - diff)
        {
            dest_value = y_bgn;
            valu_bak = value;
            fg_dir = DIR_NEG;
        }
        else
        {
            if (fg_dir == DIR_POS)
            {
                if (value < valu_bak - diff)
                {
                    valu_bak = value;
                    fg_dir = DIR_NEG;
                }
                else if (value > valu_bak)
                {
                    valu_bak = value;
                }
            }
            else                    /* ������ */
            {
                if (value > valu_bak + diff)
                {
                    valu_bak = value;
                    fg_dir = DIR_POS;
                }
                else if (value < valu_bak)
                {
                    valu_bak = value;
                }
            }
            
            if (fg_dir == DIR_POS)      temp_value = valu_bak;
            else                        temp_value = valu_bak + diff;
            
            dest_value = (y_end - y_bgn) * (temp_value - x_bgn) / (x_end - x_bgn) + y_end;
        }
    }
    
    return dest_value;
}

/****************************************************************
������16-3 ��Ȳ������Ա�(fun_tab_lookup)
���ܣ���Ȳ������Ա�
��������ǰֵ����ṹ��ָ�룻�Ƿ��������롣
���أ�yĿ��ֵ
������/��
*****************************************************************/
I16 fun_tab_lookup_equal(I32 value, TAB_DATA *tab, U08 obj)
{
    I16 dest_value = 0;
    U16 head = 0;
    U16 end = tab->num - 1;
    U16 k = 0;
    
    if (tab->num > 0)
    {
        if (value <= tab->v[head])          
        {
            dest_value = tab->t0;       /* y���ֵ */
        }
        else if (value >= tab->v[end])      
        {
            dest_value = tab->tn;       /* y�յ�ֵ */
        }
        else
        {
            U16 i = 0;
            k = (head + end) >> 1;
            
            while(i++ < tab->num)       /* ��ֹ�����������⵼����ѭ�� */
            {
                if (value < tab->v[k])              head = k;
                else if (value > tab->v[k+1])       end = k;
                else                                break;
                
                k = (head + end) >> 1; 
            }
            
            dest_value = tab->t0 + tab->td * k;                 /* �������� */
            dest_value += fun_div_round((I32)tab->td * (value - tab->v[k]), \
                                        tab->v[k+1] - tab->v[k], obj);
        }
    }
    
    return dest_value;
}

/****************************************************************
������16-4 ��ǵȲ������Ա�(fun_tab_lookup)
���ܣ���ǵȲ������Ա�
��������ǰֵ��Դ��ָ�룻Ŀ�ı�ָ�룻Ԫ�ظ������Ƿ��������롣
���أ�yĿ��ֵ
������/��
*****************************************************************/
I16 fun_tab_lookup_unequal(I32 value, I32 *tab_src, I16 *tab_dest, U16 num, U08 obj)
{
    U16 head = 0;
    U16 end = num - 1;
    U16 k = 0;
    I16 dest_value = 0;
    
    if (num > 0)
    {
        if (value <= tab_src[0])        
        {
            dest_value = tab_dest[0];
        }
        else if (value <= tab_src[end])     
        {
            dest_value = tab_dest[end];
        }
        else
        {
            U16 i = 0;
            k = (head + end) >> 1;
            
            while(i++ < num)
            {
                if (value > tab_src[k])         head = k;
                else if (value > tab_src[k+1])  end = k;
                else                            break;
                
                k = (head + end) >> 1;
            }
            
            dest_value = fun_div_round((I32)(tab_dest[k+1] - tab_dest[k]) * (value - tab_src[k]), \
                                        tab_src[k+1] - tab_src[k], obj);
            dest_value += tab_dest[k];
        }
    }
    
    return dest_value;
}

/****************************************************************
������16-5 ��������(fun_div_round)
���ܣ�����������ĳ���������
    ��Ϊ��ֵʱ��+0.5��; Ϊ��ֵʱ��-0.5������ʽΪ���� = (������*2+����) /(����*2)
������������(I32)������(I32)��
���أ��������������(I16)��
������/��
*****************************************************************/
I16 fun_div_round(I32 dividend, I32 divisor, U08 obj)
{
    I16 result = dividend;      /* �̳�ʼ��Ϊ������ */
    I16 assist_data = divisor;  /* �������ݳ�ʼ��Ϊ����(+0.5) */
    
    if (divisor != 0)
    {
        if (obj == DIV_NORMAL)  /* ������������ */
        {
            assist_data = 0;
        }
        else                    /* ���������� */
        {
            if (dividend >= 0 && divisor < 0
                || dividend < 0 && divisor > 0)
            {
                assist_data = -divisor;     /* ��Ϊ��ֵʱ����Ҫ-0.5 */
            }
        }
        
        result = (dividend * 2 + assist_data) / (divisor * 2);
    }
    
    return result;
}

/****************************************************************
������16-6 �������ĳ���(fun_div_remainder)
���ܣ��������ĳ���������
����������ָ��(I16*)��������(I32)������(I16)��
���أ���(I16)��
������һ���������ۼ�ʱ�á�
*****************************************************************/
I16 fun_div_remainder(I16 *remainder, I32 dividend, I32 divisor)
{
    I32 assist_data = 0;
    
    assist_data = (*remainder) + dividend;  /* ����+��������Ϊ���εı����� */
    *remainder = assist_data % divisor;     /* ��ȡ�������� */
    
    return (assist_data / divisor);         /* �� */
}


/*********************************************************
����: ���������Сֵ��ȡ���Խ��
�������Ա�����ǰֵ; �Ա������ֵ; �Ա�����Сֵ; ������ֵ; �����Сֵ;
���أ���ʼ����
***********************************************************/
I16 fun_line_value(I16 x, I16 x1, I16 x2, I16 y1, I16 y2)
{
    I16 result = 0;
    I16 x_min, x_max, x_min_y, x_max_y;

    if (x1 < x2)
    {
        x_min = x1;
        x_min_y = y1;
        x_max = x2;
        x_max_y = y2;
    }
    else
    {
        x_min = x2;
        x_min_y = y2;
        x_max = x1;
        x_max_y = y1;   
    }
    
    if(SensorCanUse(x))
    {

        if (x1 == x2)
        {
            result = (y1 + y2) / 2;
        }
        else
        {
            if (x <= x_min)
            {
                result = x_min_y;
            }
            else if (x >= x_max)
            {
                result = x_max_y;
            }
            else
            {
                result = ((I32)x-x2) * (y1-y2) / (x1-x2) + y2;
            }
        }
    }
    else
    {
        result = x;
    }
    
    return result;
}

U16 fun_line_value_u16(U16 x, U16 x1, U16 x2, U16 y1, U16 y2)
{
    U16 result = 0;
    U16 x_min, x_max, x_min_y, x_max_y;

    if (x1 < x2)
    {
        x_min = x1;
        x_min_y = y1;
        x_max = x2;
        x_max_y = y2;
    }
    else
    {
        x_min = x2;
        x_min_y = y2;
        x_max = x1;
        x_max_y = y1;   
    }

    if (x1 == x2)
    {
        result = (y1 + y2) / 2;
    }
    else
    {
        if (x <= x_min)
        {
            result = x_min_y;
        }
        else if (x >= x_max)
        {
            result = x_max_y;
        }
        else
        {
            result = ((U32)x-x2) * (y1-y2) / (x1-x2) + y2;
        }
    }
    
    return result;
}


/*====================================================
������tab_lookup
���ܣ������Ա�(�����)
��������������ǰֵ; ��ָ��(��ṹ��ŵ�ַ)
���أ������(�Ŵ�����tab_array����)
/====================================================*/
I16 tab_lookup(I16 src_value, TAB_DATA const *tab_temp)
{
    I16  tmp;
    U16  k, head, trail;

    head = 0;
    trail = tab_temp->num - 1;

    if (src_value <= tab_temp->v[head])
        tmp = tab_temp->t0;  /* ��Ͳ����¶� */
    else if (src_value >= tab_temp->v[trail])
        tmp = tab_temp->tn;  /* ��߲����¶� */
    else
    {
        k = (head + trail) >> 1;
        do
        {
            if (src_value < tab_temp->v[k])    /* ǰ�� */
            {
                trail = k;
                k = (head + trail) >> 1;
            }
            else if (src_value > tab_temp->v[k+1])   /* �� */
            {
                head = k + 1;
                k = (head + trail) >> 1;
            }
            else
            {
                break;
            }
        }
        while (1);

        tmp = tab_temp->t0 + tab_temp->td * k;     /* �������� */
        tmp += tab_temp->td * (src_value - tab_temp->v[k])
               / (tab_temp->v[k+1] - tab_temp->v[k]);   /* С������ */
    }

    return tmp;
}


/**********************************************************
���ܣ�����a1 = (a1+a2) % b  
   �������� = (a1+a2) / b
    �����ϴ��������г�������
������������ŵ�ַ; ������; ����
���أ���
***********************************************************/
I16 _Math_divide(I16 *a1, I32 a2, I16 b)
{
    I32 a;

    a = (*a1) + a2;
    *a1 = a % b; //����

    return (a / b); //����
}
/*==================================================================================
//int fun_absolute_value(int v)
  ���������ľ���ֵ��
==================================================================================*/
I16 fun_absolute_value(I16 v)
{
    return (v > 0) ? v : -v;
}

/*******************************************************************************
 �� ��: fun_time_check()
 �� ��: ʱ���Ƿ���ȷ���
 �� ��: ktime ��ʱ��
 �� ��: ���� TRUE;
        ���� FALSE;
 �� ��: ��
*******************************************************************************/
BOOL fun_time_check(TIME ktime)
{
    BOOL result = TRUE;
    
    if(ktime.year < 2000
       || ktime.month==0 || ktime.month>12  
       || ktime.day == 0 || ktime.day > 31
       || ktime.hour >= 24 || ktime.min >=60 || ktime.sec >=60)//ʱ�䲻��������Χ
    {
        result = FALSE;
    }

    return result;
}

/*******************************************************************************
 �� ��: fun_time_compare()
 �� ��: ʱ���С�Ƚ�
  �� ��: front ǰһ��ʱ��
         after ��һ��ʱ��
 �� ��: after >= front TRUE;
        after < front  FALSE;
 �� ��: ��
*******************************************************************************/
BOOL fun_time_compare(TIME front, TIME after)
{
    BOOL result = FALSE;

    if(fun_time_check(front) == FALSE || fun_time_check(after) == FALSE)
    {
        return result;//ʱ�䲻��ȷ
    }

    result = TRUE;
    if(after.year < front.year)
    {
        result = FALSE; 
    }
    else if(after.year == front.year)
    {
        if(after.month < front.month)
        {
            result = FALSE; 
        }
        else if(after.month == front.month)
        {
            if(after.day < front.day)
            {
                result = FALSE; 
            }
            else if(after.day == front.day)
            {
                if(after.hour < front.hour)
                {
                    result = FALSE; 
                }
                else if(after.hour == front.hour)
                {
                    if(after.min < front.min)
                    {
                        result = FALSE; 
                    }
                    else if(after.min == front.min)
                    {
                        if(after.sec < front.sec)
                        {
                            result = FALSE; 
                        }
                    }
                }   
            }
        }
    }
    
    return result;
}

/*******************************************************************************
 �� ��: fun_twotime_sub()
 �� ��: ��������ʱ��Ĳ�ֵ
 �� ��: front ǰһ��ʱ��
        after ��һ��ʱ��
 �� ��: ����ʱ������ķ���      
 �� ��: ���Ϊ65535���� = 1092Сʱ = 45�죬����ʱ����0xffff
*******************************************************************************/
U16 fun_twotime_sub(TIME front, TIME after)
{
    I16 years,months,days,hours,minutes;
    U16 i,min,max;  //��ʱ����
    const I08 MonthMax[]={0,1,-2,1,0,1,0,1,1,0,1,0,1};//ÿ������30��Ĳ�����
    U16 result;

    if(fun_time_compare(front, after) == TRUE)
    {
        minutes =  after.min - front.min;
        hours =  after.hour - front.hour;
        days =  after.day - front.day;
        months =  after.month - front.month;
        years = after.year - front.year;

        result = 365*years + 30*months+days;

        if (after.month >= front.month) 
        {
            min = front.month;
            max = after.month;
        }
        else 
        {
            max = front.month;
            min = after.month;
        }
        
        for (i=min; i<max; i++) result += MonthMax[i];
        //���������һ��
        min = front.year;
        max = after.year;
        for (i=min; i<=max; i++) 
        {
            if (front.month > 2)    continue;
            if (after.month <= 2)   continue;
            if ((i%4==0&&i%100!=0)||(i%400==0))
            {   //  ����
                result++;   //��������һ��
            }
        }
        
        if(result >= 45)//�����������
        {
            result = 0xffff;
            return result;
        }
        result *= 24;   
        result += hours;
        result *= 60;   
        result += minutes;
    }
    else
    {
        result = FALSE;
    }
    return result;
} 



/*******************************************************************************
 �� ��: fun_power()
 �� ��: �����ݼ���
 �� ��: num��cnt�η�
 �� ��: �� wlg2015.06.12
*******************************************************************************/
U32 fun_power(U16 num, U08 cnt)
{
    U32 result = 1;
    U08 i;

    for(i=0; i<cnt; i++)
    {
        result *= num;
    }

    return result;
}



I16 fun_pv_mul_10(I16 pv)
{
    I32 mul=0;
    
    if (SensorCanUse(pv))
    {
        mul = (I32)pv*10;

        if(mul > 0x7FFF)            mul = 0x7FFF;
        else if(mul < SENSOR_MIN)   mul = SENSOR_MIN;
        
        return ((I16)mul);
    }
    else
    {
        return pv;
    }
}

I16 fun_pv_div_10(I16 pv)
{
    if (SensorCanUse(pv))
    {
        if (pv<0)
            return ((pv-5)/10);
        else
            return ((pv+5)/10);
    }
    else
    {
        return pv;
    }
}



I16 fun_pv_min(I16 a, I16 b)
{
    if (!SensorCanUse(a))
    {
        return b;
    }
    else if(!SensorCanUse(b))
    {
        return a;
    }
    else
    {
        return MIN(a, b);
    }
}

I16 fun_pv_max(I16 a, I16 b)
{
    if (!SensorCanUse(a))
    {
        return b;
    }
    else if(!SensorCanUse(b))
    {
        return a;
    }
    else
    {
        return MAX(a, b);
    }
}


/*****************************************************************************
��  �� : fun_pv_cmpst()
��  �� : ģ��������
��  �� : I16 pv   : ģ����ֵ
         I16 cmps : ����ֵ
��  �� : ��
��  �� : ��
*****************************************************************************/
I16 fun_pv_cmpst(I16 pv, I16 cmps)
{
    if(SensorCanUse(pv))
    {
        return (pv + cmps);
    }
    else
    {
        return pv;
    }
}
I16 fun_pv_sub(I16 a, I16 b)
{
    if (SensorCanUse(a) && SensorCanUse(b))
    {
        return a - b;
    }

    return SENSOR_NOEXIST;
}

/*******************************************************************************
 �� ��: fun_pv_filt_t()
 �� ��: ģ��������/�˲�(һ��ʱ����)
 �� ��: p_result--ָ������;
		pv--��������;
		filt--����ֵ;
		time--ʱ�������
		dly--ʱ��
 �� ��: true:�������� false:û��������
 �� ��: 
*******************************************************************************/
BOL fun_pv_filt_t(I16 *p_result, I16 pv, I16 filt, U16* time, U16 dly)
{
    BOL result = FALSE;
	if (fun_pv_filt(p_result, pv, filt))
	{
	    if (*time < dly)       result = TRUE;
        *time = 0;
	}

    return result;
}

I16 fun_pv_add(I16 a, I16 b)
{
    if (SensorCanUse(a) && SensorCanUse(b))
    {
        return a + b;
    }

    return SENSOR_NOEXIST;
}

I16 fun_pv_avg(I16 a, I16 b)
{
    if (!SensorCanUse(a))
    {
        return b;
    }
    else if(!SensorCanUse(b))
    {
        return a;
    }
    else
    {
        return (a + b) / 2;
    }    
}

/*******************************************************************************
 �� ��: fun_pv_in()
 �� ��: �ж��Ƿ��ڱ�������
 �� ��: v--; 
		low--; 
		high--; 
 �� ��: 
 �� ��: ��
*******************************************************************************/
BOL fun_pv_in(I16 v, I16 low, I16 high)
{
    BOL is_in = FALSE;
    
    if (SensorCanUse(v))
    {
        if (v >= low && v <= high)
        {
            is_in = TRUE;
        }
    }

    return is_in;
}

/*******************************************************************************
 �� ��: fun_pv_filt()
 �� ��: ģ��������/�˲�
 �� ��: p_result--ָ������;
		pv--��������;
		filt--����ֵ;
 �� ��: true:�������� false:û��������
 �� ��: û�����߽籣����ע��ʹ��
*******************************************************************************/
BOL fun_pv_filt(I16 *p_result, I16 pv, I16 filt)
{
    BOL result = FALSE;
	I16 filt_abs = fun_absolute_value(filt);
	if (!fun_pv_in(*p_result, pv-filt_abs, pv+filt_abs))
	{
	    *p_result = pv;
        result = TRUE;
	}

    return result;
}

/*******************************************************************************
 �� ��: fun_pv_in_open()
 �� ��: �ж��Ƿ��ڿ�������
 �� ��: v--; 
		low--; 
		high--; 
 �� ��: 
 �� ��: ��
*******************************************************************************/
BOL fun_pv_in_open(I16 v, I16 low, I16 high)
{
    BOL is_in = FALSE;
    
    if (SensorCanUse(v))
    {
        if (v > low && v < high)
        {
            is_in = TRUE;
        }
    }

    return is_in;
}

/*******************************************************************************
 �� ��: fun_pv_limit()
 �� ��: pv��Χ����
 �� ��: pv--ģ����; 
		min--��Сֵ; 
		max--���ֵ; 
 �� ��: 
 �� ��: ��
*******************************************************************************/
I16 fun_pv_limit(I16 pv, I16 min, I16 max)
{
    if (SensorCanUse(pv))
    {
        if(pv < min)        pv = min;
        else if(pv > max)   pv = max;
    }

    return pv;
}
/*******************************************************************************
 �� ��: fun_pv_get()
 �� ��: ��ȡģ����,a������ʱ��Ĭ��ֵdft
 �� ��: a--ģ����; 
		dft--Ĭ��ֵ; 
 �� ��: 
 �� ��: ��
*******************************************************************************/
I16 fun_pv_get(I16 a, I16 dft)
{
    if (SensorCanUse(a))
    {
        return a;
    }
    else
    {
        return dft;
    }
}

I16 fun_pv_abs(I16 pv)
{
    if (SensorCanUse(pv))
    {
        if (pv < 0)  return -pv;
        else         return pv;
    }
    else
    {
        return pv;
    }
}

I32 Fun_AbsI32(I32 a)
{
    if (a < 0)  return -a;
    else        return a;
}

/*****************************************************************************
��  �� : Fun_SimFloatDiv()
��  �� : ģ�⸡��������
��  �� : I32 a   : ������
         I32 b   : ����
         U08 dot : С��λ
��  �� : ��
��  �� : ��
*****************************************************************************/
I32 Fun_SimFloatDiv(I32 a, I32 b, U08 dot)
{
    I32 abs_a = 0;
    I32 times = 0;
    I32 c_a = 1;    /* a�ķŴ�ϵ�� */
    I32 c_b = 1;    /* b����Сϵ�� */
    U08 d_a = 0;    /* a�ķŴ�λ�� */
#define MAX_I32     ((I32)0x7FFFFFFF)

    abs_a = a;
    if (abs_a < 0)  abs_a = -a;
    
    while(abs_a < MAX_I32/(10*c_a))/* Ϊ�˷�ֹ���磬a���Ŵ�MAX_I32 */
    {
        c_a *= 10;
        d_a++;
        if (d_a >= dot)
            break;
    }

    while(d_a < dot)
    {
        c_b *= 10;
        d_a++;
    }

    times = ((a*c_a) / (b/c_b));  /* a��ʽc_a����b��Сc_b�� */

    return times;
}

/*****************************************************************************
��  �� : Fun_Range()
��  �� : a��o��(100��r)%��Χ�ڣ�����TRUE
��  �� : I16 a : 
         I16 o : 
         U08 r : 
��  �� : ��
��  �� : ��
*****************************************************************************/
U08 Fun_Range(I16 a, I16 o, U08 r)
{
    U08 res = FALSE;
    I32 min,max;

    if(r>100)   res = FALSE;
    else
    {
        min = (I32)o*(100-r)/100;
        max = (I32)o*(100+r)/100;
        if(min <= a && a <= max)
            res = TRUE;
        else
            res = FALSE;
    }

    return res;
}

// ******************************************************  
// * Line fitting with least square error method  
// * Input:  
// *    x, input x values  
// *    y, input y values  
// *    num, num of x and y  
// *    A, coefficient in y = x/A + B  
// *    B, coefficient in y = x/A + B  
// *  ��С���˷����ֱ�ߣ�Ϊģ����У׼����ƣ�������������һ������
// *   
// ******************************************************  
void Fun_LineFitting(U16 x[], U16 y[], U16 num, U08 dot, I32* K, I32* A, I32* B)  
{  
    I32 xmean = 0;  
    I32 ymean = 0;  
    I32 sumx2 = 0;  
    I32 sumxy = 0;  
    U16 i;
    U32 coe=1;
    
    for(i = 0; i < num; i++)  
    {  
        xmean += x[i];  
        ymean += y[i];  
    }  
    xmean /= num;  
    ymean /= num;  
  

    for(i = 0; i < num; i++)  
    {  
        sumx2 += (x[i] - xmean) * (x[i] - xmean);  
        sumxy += (y[i] - ymean) * (x[i] - xmean);  
    }  

    for (i=0; i<dot; i++)
    {
        coe *= 10;
    }

    *K = Fun_SimFloatDiv(sumxy, sumx2, dot); /* k = ��XY / ��X^2 */
    *A = Fun_SimFloatDiv(sumx2, sumxy, dot); /* a = ��X^2 / ��XY */// _     _
//  *B = ymean*coe - Fun_SimFloatDiv(xmean*sumxy, sumx2, dot); /* b = Y - k*X */ /* xmean*sumxy���ܳ��� */
    if (Fun_AbsI32(*A) > Fun_AbsI32(*K))        /* ʹ�ô���������پ��ȶ�ʧ */
    {
        *B = ymean*coe - Fun_SimFloatDiv(xmean, *A, dot+dot); /* A�Ŵ���dot�������ٷŴ�dot */
    }
    else
    {
        *B = ymean*coe - *K*xmean;
    }
    
}  

 
/*****************************************************************************
��  �� : Fun_NumFormat()
��  �� : ��ʽ��16λ��ֵ
��  �� : U16 num    : ��ֵ
         U08* buf   : Ŀ�껺���ַ
         U16 attrib : ��ʽ����
��  �� : �ֽ���
��  �� : ��
*****************************************************************************/
U08 Fun_NumFormat(U16 num, U08* buf, U16 attrib) 
{
    #define MAX_LENGHT  8
    U08 length   = 0;     /* �������ַ��� */
    U08 dot_n    = 0;
    U08 dot_fg   = FALSE;
    U08 nega_fg  = FALSE;
    U08 disp_char[MAX_LENGHT]=" ";
    U08 count = 0;  /* ʵ�ʸ�ʽ���ַ��� */
    U08 n = 0;

    num &= 0xFFFF;
    /* ���� */
    if(attrib&FNS_HEX)
    {
        length = FNS_DIGIT6;
    }
    else
    {
        length =(U08)(attrib & FNS_DIGIT_MASK);
    }
    /* ���� */
    if (!(attrib&FNS_UNSIGNED))
    {
        if ((I16)num < 0)
        {
            nega_fg = TRUE;
            num = 0-num;
        }
    }
    /* С��λ */
    if (attrib&FNS_FLOAT_MASK)
    {
        dot_n = ((U08)(attrib&FNS_FLOAT_MASK))>>4;
        dot_fg = TRUE;
    }

    if (num == 0)
    {
        disp_char[count++] = '0';
        if (dot_n > 0)
        {
            dot_n--;
        }
    }
    else
    {
        while (num != 0)
        {
            if(attrib&FNS_HEX)
            {
                U08 ch;
                ch = num%16;
                if(ch < 10) ch += '0';
                else
                {
                    ch = ch%10 + 'A';
                }
                disp_char[count++] = ch;
                num = num/16;//ȥ���Ѿ���ʾ��λ
                dot_fg = FALSE;
            }
            else
            {
                disp_char[count++] = num%10+'0';
                num = num/10;//ȥ���Ѿ���ʾ��λ
            }
            if (dot_fg)
            {
                dot_n--;
                if ((dot_n == 0)&&(num != 0))//С����ǰ�治Ϊ0������ʾС����
                {
                    disp_char[count++] = '.';
                    dot_fg = FALSE;
                }
            }
        }
    }
    if (dot_fg)
    {
        while (dot_n > 0)//��ȫС��������0
        {
            disp_char[count++] = '0';
            dot_n--;
        }
        disp_char[count++] = '.';
        disp_char[count++] = '0';
    }
    if (attrib&FNS_HEX)
    {
        for (; count<4; count++)
        {
            disp_char[count] = '0';
        }
        disp_char[count++] = 'x';
        disp_char[count++] = '0';
    }
    else if(nega_fg)
    {
        disp_char[count++] = '-';
    }

    // δ���峤��ʱ,��Ϊʹ��ʵ�ʳ���
    if(length == 0) length = count;

    if(attrib&FNS_RIGHT)//�Ҷ���
    {
        U08 c=' ';
        if(attrib&FNS_ZERO)
            c = '0';

        for(n=0; n<count&&n<length; n++)
        {
            buf[length-n-1]= disp_char[n];
        }

        while (count < length)//δ��ʾ������ո�
        {
            buf[length-count-1]= c;
            count++;
        }
    }
    else//�����
    {
        U08 i=0;
        U08 c=' ';
        if(attrib&FNS_ZERO)
            c = '0';

        n = count;
        for( ; n>0; )
        {
             buf[i++]=disp_char[--n];
        }
        while (count < length)//δ��ʾ������ո�
        {
            buf[count++] = c;
        }
    }

    buf[count] = '\0';

    return count;
}  

#ifdef CRC_SPEED_PRI
/*******************************************************************************
 �� ��: fun_crc16()
 �� ��: CRC16����
 �� ��: puchMsg--����ָ�� 
		usDataLen--���鳤�� 
 �� ��: CRC������
 �� ��: ��
*******************************************************************************/
unsigned short fun_crc16(const unsigned char *puchMsg, unsigned short usDataLen)
{
	unsigned char uchCRCHi = 0xFF;			/* ��CRC�ֽڳ�ʼ�� */
	unsigned char uchCRCLo = 0xFF;			/* ��CRC�ֽڳ�ʼ�� */
	unsigned char uIndex;					/* CRCѭ���е����� */
	
	while ((usDataLen--) != 0)				/* ������Ϣ������ */
	{
		uIndex = uchCRCHi ^ *(puchMsg++);	/* ����CRC */
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
		uchCRCLo = auchCRCLo[uIndex];
	}
	
	return ((unsigned short)uchCRCLo<<8 | (unsigned short)uchCRCHi);
}
#else
/*******************************************************************************
 �� ��: fun_crc16()
 �� ��: CRC16����
 �� ��: puchMsg--����ָ�� 
		usDataLen--���鳤�� 
 �� ��: CRC������
 �� ��: ��
*******************************************************************************/
unsigned short fun_crc16(const U08 *pData, U16 len)
{
    U08 j; 
    U16 crcReg;				/* CRC�Ĵ��� */
	#undef CRC_POLY
    #define CRC_POLY 0xa001	/* CRC����ʽ */
    
    crcReg = 0xffff;		/* ��ʼ���Ĵ��� */
    while ((len--) != 0)
    {
    	crcReg = crcReg ^ (*pData);
    	pData++;
    	for (j=0; j<8; j++)
    	{
    		if (GETBIT(crcReg, 0))
    		{
    			crcReg = (crcReg>>1) ^ CRC_POLY;
    		}
    		else
    		{
    			crcReg = crcReg>>1;
    		}
    	}
    }
    return crcReg;
}
#endif	/* CRC_SPEED_PRI */

/*******************************************************************************
 �� ��: fun_crc16_ok()
 �� ��: CRC16У��OK?(����У����)
 �� ��: pData--����; 
		lenIncCrc--����(����У����); 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL fun_crc16_ok(const U08 *pData, U16 lenIncCrc)
{
	return (fun_crc16(pData, lenIncCrc) == 0x0000);		
}

/*****************************************************************************
��  �� : Fun_CalAverageFilt()
��  �� : ���˳������Сֵ��ƽ��ֵ
��  �� : I16 src[] : 
         U08 n     : 
��  �� : ��
��  �� : ��
*****************************************************************************/
I16 Fun_CalAverageFilt(I16 src[], U08 n)
{
    U08 idx;
    I16 min,max,s;
    I32 sum;
    I16 avg;

    switch(n)
    {
        case 0:
        case 1:
            return src[0];
        case 2:
            return ((src[0]+src[1])>>1);
        default:
            min = max = src[0];
            for (sum=0,idx=0; idx<n; idx++)
            {
                s = src[idx];
                sum += s;
                if(s < min) min = s;
                if(s > max) max = s;
            }
            break;        
    }

    sum -= (min + max);

    if (sum >= 0)    avg = (sum + ((n-2)>>1)) / (n-2);
    else             avg = (sum - ((n-2)>>1)) / (n-2);
    
    return avg; 

}


/*****************************************************************************
��  �� : fun_filt_avg()
��  �� : ���˳������Сֵ��ƽ��ֵ,������Чֵ
��  �� : I16 src[] : 
         U08 n     : 
��  �� : ��
��  �� : ��
*****************************************************************************/
I16 fun_filt_avg(I16 src[], U08 n)
{
    U08 idx;
    I16 min=0,max=0,s;
    I32 sum;
    I16 avg = SENSOR_NOEXIST;
    BOL first = TRUE;
    U08 can_use_n = 0;

    switch(n)
    {
        case 0:
            avg = SENSOR_NOEXIST;
            break;
        case 1:
            avg = src[0];
            break;
        case 2:
            avg = fun_pv_avg(src[0], src[1]);
            break;
        default:
            // ͳ�ƿ��õ�ֵ
            for (sum=0,idx=0; idx<n; idx++)
            {
                s = src[idx];
                if (SensorCanUse(s))
                {
                    if (first)
                    {
                        first = FALSE;
                        min = max = s;
                    }
                    sum += s;
                    if(s < min) min = s;
                    if(s > max) max = s;
                    can_use_n++;
                }
            }

            // �ж���Ч�ĸ���
            switch (can_use_n)
            {
                case 0:
                    avg = SENSOR_NOEXIST;
                    break;
                case 1:
                    avg = min;
                    break;
                case 2:
                    avg = fun_pv_avg(min, max);
                    break;
                default:
                    sum -= (min + max);
                    if (sum >= 0)    avg = (sum + ((can_use_n-2)>>1)) / (can_use_n-2);
                    else             avg = (sum - ((can_use_n-2)>>1)) / (can_use_n-2);
                    break;
            }
            break;        
    }


    
    return avg; 

}



/*****************************************************************************
��  �� : fun_all_avg()
��  �� : ��ȫ����ƽ��ֵ,������Чֵ
��  �� : I16 src[] : 
         U08 n     : 
��  �� : ��
��  �� : ��
*****************************************************************************/
I16 fun_all_avg(I16 src[], U08 n)
{
    U08 idx;
    I16 min=0,max=0,s;
    I32 sum;
    I16 avg = SENSOR_NOEXIST;
    BOL first = TRUE;
    U08 can_use_n = 0;

    switch(n)
    {
        case 0:
            avg = SENSOR_NOEXIST;
            break;
        case 1:
            avg = src[0];
            break;
        case 2:
            avg = fun_pv_avg(src[0], src[1]);
            break;
        default:
            // ͳ�ƿ��õ�ֵ
            for (sum=0,idx=0; idx<n; idx++)
            {
                s = src[idx];
                if (SensorCanUse(s))
                {
                    if (first)
                    {
                        first = FALSE;
                        min = max = s;
                    }
                    sum += s;
                    if(s < min) min = s;
                    if(s > max) max = s;
                    can_use_n++;
                }
            }

            // �ж���Ч�ĸ���
            switch (can_use_n)
            {
                case 0:
                    avg = SENSOR_NOEXIST;
                    break;
                case 1:
                    avg = min;
                    break;
                case 2:
                    avg = fun_pv_avg(min, max);
                    break;
                default:
                    /* ��ȥ�������Сֵ */
                    // sum -= (min + max); 
                    if (sum >= 0)    avg = (sum + ((can_use_n)>>1)) / (can_use_n);
                    else             avg = (sum - ((can_use_n)>>1)) / (can_use_n);

                    /* ȥ�������Сֵ */
                    // sum -= (min + max); 
                    // if (sum >= 0)    avg = (sum + ((can_use_n-2)>>1)) / (can_use_n-2);
                    // else             avg = (sum - ((can_use_n-2)>>1)) / (can_use_n-2);
                    break;
            }
            break;        
    }


    
    return avg; 

}

/*****************************************************************************
��  �� : fun_rec_avg()
��  �� : ���ݼ�¼,������ƽ��ֵ,ȥ�������С 
��  �� : I16 val   : ��ǰֵ
         I16* data : ����ָ��
         U08 num   : �ܸ���
��  �� : ��¼��ƽ��ֵ
��  �� : ��
*****************************************************************************/
I16 fun_rec_avg(I16 val, I16* data, U08 num)
{
    U08 i;
    I16 avg;
    
    if (data[0] == SENSOR_NOEXIST)
    {
        for (i=0; i<num; i++)
        {
            data[i] = val;
        }
    }
    else
    {
        for (i=0; i<num-1; i++)
        {
            data[i] = data[i+1];
        }
        data[num-1] = val;
    }
    
    avg = fun_filt_avg(data, num);

    return avg;
}


/*****************************************************************************
��  �� : fun_stab_avg()
��  �� : �ж��Ƿ���ƽ��ֵ[d_lower, d_upper]���ȶ�,ȥ�������С
��  �� : I16* data   : ����
         U08 num     : ���ݸ���
         I16 d_lower : ƫ������
         I16 d_upper : ƫ������
��  �� : TRUE-�ȶ�,FALSE-���ȶ�
��  �� : by:zzp ˼·: ��ȥ�������С,�жϴδ��С�Ƿ���Ҫ��Χ��
*****************************************************************************/
BOL fun_stab_avg(I16* data, U08 num, I16 d_lower, I16 d_upper)
{
    U08 idx;
    U08 cnt;
    BOL first = TRUE;
    U08 min_i, max_i;
    I16 min, max, s;
    I16 min_2=0, max_2=0;
    I32 sum;

    switch(num)
    {
        case 0:
        case 1:
            return TRUE;
        case 2:
            if (data[0] > data[1])  max = data[0], min = data[1];
            else                    max = data[1], min = data[0];

            /* max - (max+min)/2 <= d_upper >>>> max - min <= 2*d_upper */
            /* min - (max+min)/2 >= d_lower >>>> min - max >= 2*d_lower */
            return (max - min <= 2*d_upper && min - max >= 2*d_lower);
            
        default:
            /* ���������С */
            min = max = data[0];
            min_i = max_i = 0;
            for (sum=0,idx=0,cnt=0; idx<num; idx++)
            {
                s = data[idx];
				sum += s, cnt++;    // ���,��ͳ�Ƹ���
                if(s < min) min = s, min_i = idx;
                if(s > max) max = s, max_i = idx;
            }

            if (num >= 4) // ����4������ȥ�������Сֵ
            {
                /* ȥ�������С,�Ҵδ��С */
                for (sum=0,idx=0,cnt=0; idx<num; idx++)
                {
                    if (idx == min_i || idx == max_i)   continue;
                    s = data[idx];
                    if (first)  
                    {
                        first = FALSE;
                        min_2 = max_2 = s;
                    }
                    sum += s, cnt++;    // ���,��ͳ�Ƹ���
                    if(s < min_2) min_2 = s;
                    if(s > max_2) max_2 = s;
                }
            }
            break;        
    }

    /* max_2 - sum/cnt <= d_upper >>>>  max_2*cnt - sum <= d_upper*cnt */
    /* min_2 - sum/cnt >= d_lower >>>>  min_2*cnt - sum >= d_lower*cnt */
    return (max_2*cnt - sum <= d_upper*cnt && min_2*cnt - sum >= d_lower*cnt); 

}

/*******************************************************************************
 �� ��: fun_rtc_check()
 �� ��: LOG_RTCʱ���Ƿ���ȷ���
 �� ��: ktime ��ʱ��
 �� ��: ���� TRUE;
        ���� FALSE;
 �� ��: ��
*******************************************************************************/
BOOL fun_rtc_check(LOG_RTC ktime)
{
    BOOL result = TRUE;

    if(!((ktime.year>=2000 && ktime.year<=2099) || (ktime.year>=0 && ktime.year<=99))	/* ����00~99��2000~2099 */
       || ktime.month==0 || ktime.month>12  
       || ktime.day==0 || ktime.day>31
       || ktime.hour>=24 || ktime.min>=60 || ktime.sec>=60)//ʱ�䲻��������Χ
    {
        result = FALSE;
    }

    return result;
}
/******************************************************************************
�㷨������

���㷨����˼���Ǽ���������ڵ� 0��3��1�յ�������Ȼ���������ȡ�����ļ����

m1 = (month_start + 9) % 12; �����ж������Ƿ����3�£�2�����ж�����ı�ʶ���������ڼ�¼��3�µļ��������

y1 = year_start - m1/10; �����1�º�2�£��򲻰�����ǰ�꣨��Ϊ�Ǽ��㵽0��3��1�յ���������

d1 = 365*y1 + y1/4 - y1/100 + y1/400 + (m1*306 + 5)/10 + (day_start - 1);

    ���� 365*y1 �ǲ�����������һ���������

    y1/4 - y1/100 + y1/400  �Ǽ���������������һ�죬

(m2*306 + 5)/10 ���ڼ��㵽��ǰ�µ�3��1�ռ��������306=365-31-28��1�º�2�£���5��ȫ���в���31���·ݵĸ���

(day_start - 1) ���ڼ��㵱ǰ�յ�1�յļ������

******************************************************************************/


/*****************************************************************************
��  �� : fun_time_days()
��  �� : �������ڵ�0000��3��1�յ�����
��  �� : TIME time : ����
��  �� : ��
��  �� : ��
*****************************************************************************/
I32 fun_time_days(LOG_RTC* time)
{
	I32 y, m, d;
	U16 year_temp = time->year>=2000 ? time->year : time->year+2000;	/* ����00~99��2000~2099 */

	m = (time->month + 9) % 12;
	y = year_temp - m/10;
	d = 365*y +                 // ������
        (y/4 - y/100 + y/400) + // ����ƫ������
        (m*306 + 5)/10 +        // ������
        (time->day - 1);         // ����

    return  d;
}

/*****************************************************************************
��  �� : fun_time_secs()
��  �� : �������ڵ��������
��  �� : TIME time : ����
��  �� : ��
��  �� : ��
*****************************************************************************/
I32 fun_time_secs(LOG_RTC* time)
{
    I32 secs;

    secs = time->hour*3600L + time->min*60 + time->sec;

    return secs;
}

/*****************************************************************************
��  �� : fun_time_diff()
��  �� : �����������ڲ�
��  �� : TIME front    : 
         TIME after    : 
         U08 diff_type : 
��  �� : ��
��  �� : �ú������ڼ�������ʱ��֮��Ĳ�ֵ����������ʶ���֡���ʱ������
*****************************************************************************/
I32 fun_time_diff(LOG_RTC* front, LOG_RTC* after, U08 diff_type)
{
    I32 diff_sec = 0;
    I32 div = 1;

    diff_sec = (fun_time_days(after) - fun_time_days(front))*(24*60*60L)
             + (fun_time_secs(after) - fun_time_secs(front));

    switch (diff_type)
    {
        case TIME_DIFF_DAYS:
            div = 24*60*60L;
            break;
        case TIME_DIFF_HOURS:
            div = 60*60L;
            break;
        case TIME_DIFF_MINS:
            div = 60L;
            break;
        case TIME_DIFF_SECS:
            div = 1L;
            break;
        default:
            return 0;
    }

    return diff_sec/div;
}

/*******************************************************************************
 �� ��: fun_time_diff_day()
 �� ��: �����������ڼ������
 �� ��: front--; 
		after--; 
 �� ��: 
 �� ��: ��
*******************************************************************************/
I32 fun_time_diff_day(LOG_RTC* front, LOG_RTC* after)
{
    I32 diff_day = 0;

    diff_day = fun_time_days(after) - fun_time_days(front);

    return diff_day;
}

#define BASE_YEAR 2000		/* Ϊ������00~99��2000~2099��������С��2000 */
/*****************************************************************************
��  �� : fun_time_mktime()
��  �� : ��2000��1��1��0ʱ0��0���ʱ���(����)[����ʱ��]
��  �� : TIME* now : ��ǰʱ��.
��  �� : ʱ���(����)
��  �� : ��
*****************************************************************************/
U32 fun_time_mktime(LOG_RTC* now)
{
    U32 diff_sec = 0;
    LOG_RTC from;
    from.day = 1;
    from.month= 1;
    from.year = BASE_YEAR;
    from.hour= 0;
    from.sec = 0;
    from.min = 0;

    if (now->year < BASE_YEAR)
    {
        return 0;
    }

    diff_sec = (fun_time_days(now) - fun_time_days(&from))*(24UL*60*60)
             + (fun_time_secs(now) - fun_time_secs(&from));

    
    return diff_sec;
}

/*****************************************************************************
��  �� : fun_time_localtime()
��  �� : ���ݾ�2000��1��1��0ʱ0��0���ʱ���(����)��������[����ʱ��]
��  �� : U32 secs : ʱ���
��  �� : ����
��  �� : ��
*****************************************************************************/
LOG_RTC fun_time_localtime(U32 secs)
{
    LOG_RTC t;
    U32 remain_days;
    I16 year;
    I16 m; // [0~11]
//    I16 d; // [0~30]
    static const U08 days[]={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    t.sec = secs % 60;
    t.min = (secs / 60) % 60;
    t.hour= (secs / 3600) % 24;
    #define is_leap(year)   ((((year) % 4 == 0) && ((year)%100 != 0)) || ((year)%400 == 0))
    // ����
    remain_days = secs / (24L*60*60);
    // ����
    t.week = (remain_days + 5)%7 + 1; // 2000.1.1��������(5)��������BASE_YEAR���������
    // ��
    year = BASE_YEAR;
    while (TRUE)
    {
        U16 days_per_year = 365;

        if (is_leap(year))
        {
            days_per_year += 1;
        }
        
        if (remain_days < days_per_year)
        {
            break;
        }

        year++;
        remain_days -= days_per_year;
    }
    t.year = year;
    
    // ��
    for (m=0; m<12; m++)
    {
        U08 m_days = days[m];
        
        if (m == 1) // 2��
        {
            if (is_leap(t.year))
            {
                m_days = 29;
            }
        }
        
        if (remain_days < m_days)
        {
//            d = remain_days;
            t.month = m + 1;
            break;
        }

        remain_days -= m_days;
    }

    // ��
    t.day = remain_days + 1;

    
    return t;
}


U16 Fun_NumDisp(U16 num, STR* buf, U16 attrib) 
{
    U08 length;
    U08 dot_n    = 0;
    U08 dot_fg   = FALSE;
    U08 nega_fg  = FALSE;
    U08 disp_char[MAX_LENGHT]=" ";    
    U08 count = 0;
    U08 n = 0;
    #define MAX_LENGHT  8

    if(attrib&FDNS_HEX)
    {
        length = FDNS_DIGIT_6;
    }
    else
    {
        length =(U08)(attrib & FDNS_DIGIT_MASK);
        if (length == 0)
        {
            length = 3;   // ��ʾ����Ĭ��Ϊ3
        }
    }
    if (attrib&FDNS_SIGNED)
    {
        if ((I16)num < 0)
        {
            nega_fg = TRUE;
            num = 0-num;
        }
    }
    if (attrib&FDNS_FLOAT_MASK)
    {
        dot_n = ((U08)(attrib&FDNS_FLOAT_MASK))>>4;
        dot_fg = TRUE;
    }

    if (num == 0)
    {
        disp_char[count++] = '0';
        if (dot_n > 0)
        {
            dot_n--;
        }
    }
    else
    {
        while (num != 0)
        {
            if(attrib&FDNS_HEX)
            {
                U08 ch;
                ch = num%16;
                if(ch < 10) ch += '0';
                else
                {
                    ch = ch%10 + 'A';
                }
                disp_char[count++] = ch;
                num = num/16;//ȥ���Ѿ���ʾ��λ
            }
            else
            {
                disp_char[count++] = num%10+'0';
                num = num/10;//ȥ���Ѿ���ʾ��λ
            }
            if (dot_fg)
            {
                dot_n--;
                if ((dot_n == 0)&&(num != 0))//С����ǰ�治Ϊ0������ʾС����
                {
                    disp_char[count++] = '.';
                    dot_fg = FALSE;
                }
            }
        }
    }
    if (dot_fg)
    {
        while (dot_n > 0)//��ȫС��������0
        {
            disp_char[count++] = '0';
            dot_n--;
        }
        disp_char[count++] = '.';
        disp_char[count++] = '0';
    }
    if (attrib&FDNS_HEX)
    {
        for (; count<4; count++)
        {
            disp_char[count] = '0';
        }
        disp_char[count++] = 'x';
        disp_char[count++] = '0';
    }
    else if(nega_fg)
    {
        disp_char[count++] = '-';
    }

    if(attrib&FDNS_SNAP_LEFT)//�����
    {
        U08 i=0;
        U08 c=' ';
        if(attrib&FDNS_ZERO)
            c = '0';
        
        n = count;
        for( ; n>0; )
        {
             buf[i++]=disp_char[--n];
        }
        while (count < length)//δ��ʾ������ո�
        {
            buf[count++] = c;
        }
    }
    else//�Ҷ���
    {
        U08 c=' ';
        if(attrib&FDNS_ZERO)
            c = '0';
        
        for(n=0; n<count&&n<length; n++)
        {
            buf[length-n-1]= disp_char[n];
        }

        while (count < length)//δ��ʾ������ո�
        {
            buf[length-count-1]= c;
            count++;
        }
    }

    return count;
}

/*****************************************************************************
��  �� : fun_bit_first_1()
��  �� : ���ҵ�һ��1������bitλ
��  �� : U16 word[] : 
         U16 words  : 
��  �� : λ��
��  �� : ��δ�ҵ�,����0xFFFF
*****************************************************************************/
U16 fun_bit_first_1(U16 word[], U16 words)
{
    U16 idx;
    U16 no = 0xFFFF;
    
    for (idx=0; idx<16*words; idx++) // ��һ�����Ϻ�
    {
        if (GETBIT_N(word, idx))
        {
            no = idx;
            break;
        }
    }

    return no;
}

/*******************************************************************************
 �� ��: fun_find_node_from_arr()
 �� ��: �����������л�ȡ�����λ��
 �� ��: pv--��ǰֵ; 
		arr[]--��������; 
		arr_size--���鳤��; 
		dir--���е��������; 
 �� ��: 
 �� ��: ��
*******************************************************************************/
U08 fun_find_node_from_arr(I16 pv, I16 arr[], U08 arr_size, U08 dir)
{
    U08 i, result = 0;

	switch (dir)
	{
	    case DIR_ACD:
	        for (i=0; i<arr_size; i++)
	        {
	            if (pv <= arr[i])
	            {
					break;	/* ����for */
	            }
	        }
			result = i;
	        break;
	    case DIR_DCD:
	        for (i=0; i<arr_size; i++)
	        {
	            if (pv >= arr[i])
	            {
					break;	/* ����for */
	            }
	        }
			result = i;
	        break;
	    default:
	        break;
	}

	return result;
}

/*****************************************************************************
��  �� : get_relative_pos()
��  �� : ��ȡָ������λ���ڲ�ͬ��С�Ŀ��е����λ��
��  �� : U16 blocks_size[] : ���С
         U16 num           : �����
         U16 idx           : ����λ��
��  �� : ���λ��
��  �� : ��
*****************************************************************************/
XY fun_get_relative_pos(U16 blocks_size[], U16 num, U16 idx)
{
    XY xy={0,0};
    U16 n;

    for (n=0; n<num; n++)
    {
        if (idx >= blocks_size[n])
        {
            idx -= blocks_size[n];
        }
        else
        {
            xy.x = n;
            xy.y = idx;
            break;
        }
    }

    return xy;
}

/*******************************************************************************
 �� ��: fun_value_trend()
 �� ��: ��ǰֵ�仯�����ж�
 �� ��: val--��ǰֵ; 
		avg--ƽ��ֵ; 
		diff--������; 
 �� ��: ��ƽ/����/�½�
 �� ��: ���Ŀ�ģ����fun_line_trend���ڸ���ʱ�����жϳ���ǰ�ı仯���ƣ�
        ���޷��ﵽ���Ŀ�ģ���ɾ���˺�����ʹ��fun_line_trend���
*******************************************************************************/
U08 fun_value_trend(I16 val, I16 avg, I16 diff)
{
    U08 trend = LINE_TREND_KEEP;

    if (val-avg > diff)
    {
        trend = LINE_TREND_RISE;
    }
    else if (avg-val > diff)
    {
        trend = LINE_TREND_FALL;
    }
    
    return trend;
}

/*******************************************************************************
 �� ��: fun_sort_bubble()
 �� ��: ð������
 �� ��: arr[]--��������; 
		n--���г���; 
		dir--DIR_ACD��DIR_DCD; 
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void fun_sort_bubble(I16 arr[], U08 n, U08 dir)
{
    U08 i=0, j=0;

    if (n < 2)  return;

    for (i=0; i<n-1; i++)
    {
        BOL fg_chg = FALSE;	/* ������ǰ�ж�������� */
        
        for (j=0; j<n-1-i; j++)
        {
            switch (dir)
            {
                default:
                case DIR_ACD:
                    if (arr[j] > arr[j+1])
                    {
                        I16 tmp = arr[j];
                        arr[j] = arr[j+1];
                        arr[j+1] = tmp;

                        fg_chg = TRUE;
                    }
                    break;
                    
                case DIR_DCD:
                    if (arr[j] < arr[j+1])
                    {
                        I16 tmp = arr[j];
                        arr[j] = arr[j+1];
                        arr[j+1] = tmp;

                        fg_chg = TRUE;
                    }
                    break;
            }
        }

        if (!fg_chg)    break;
    }
}

/*******************************************************************************
 �� ��: fun_check_fg_arr()
 �� ��: ����־λ�����Ƿ������λ���������λ���򷵻�TRUE
 �� ��: fg_arr--��־λ����; 
		bit_num--��־λ����; 
		is_once--�Ƿ�Ϊһ���Ա�־��TRUE:�������Զ����, FALSE:ֻ��������� 
 �� ��: TRUE/FALSE--�ɹ�/ʧ��
 �� ��: ��
*******************************************************************************/
BOOL fun_check_fg_arr(U16 *fg_arr, U16 bit_num, BOOL is_once)
{
    U16 i;
    BOOL result = FALSE;
    
    for (i = 0; i < bit_num; i++)
    {
        if (GETBIT_N(fg_arr, i))
        {
            result = TRUE;
        }
    }

    if (is_once)
    {
        MEMCLR(fg_arr, _DIVUCARRY(bit_num, 16));			/* ע��: �˴���������ARRCLR�꣬��Ϊ�˴��޷�ʹ��sizeof����fg_arr��ʵ���� */
    }

    return result;
}


/*******************************************************************************
 �� ��: fun_copy_str_to_U16()
 �� ��: ���ַ����浽U16����
 �� ��: ��
 �� ��: ��
 �� ��: ��
*******************************************************************************/
void fun_copy_str_to_U16(char* str, U16 arr[])
{
    U08 i = 0;
    U08 len = STRLEN(str);

    for (i=0; i<len; i++)
    {
        arr[i] = str[i];
    }
    arr[i] = 0;
}

void fun_U16_to_str(U16 word, U08 str[])
{
    str[0]=GET_DIGIT(word, 4)+'0';
    str[1]=GET_DIGIT(word, 3)+'0';
    str[2]=GET_DIGIT(word, 2)+'0';
    str[3]=GET_DIGIT(word, 1)+'0';
    str[4]=GET_DIGIT(word, 0)+'0';
    str[5]=0;
}

U16 Fun_StrCat(U08* str1, const U08* str2)
{
    STRCAT(str1, str2);

    return STRLEN(str2);
}

