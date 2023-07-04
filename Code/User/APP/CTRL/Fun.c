/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: FUN.c
 文件描述: 通用的子程序，与项目无关。今后此部分内容可放到标准库中。
 创建人  : yx
 创建日期: 2014.12.15
*******************************************************************************/
#define  FUN_GLOBAL
#include "includes.h"

/***********************************************
函数： fun_erea_devide
功能： 控制区域确定，完成区域划分(允许每个区域回差不同)
参数： 
		s_area：当前区域号, 1 对应最高区域(大于最大温度点的区域)
		Tpoint：分区温度点数组
		Diff：	分区切换温差数组（低温区往高温区切换，对应区域号从高到低）
		PointMax：参与计算的分区温度点（最大不超过TEMP_POINT_MAX）
		
返回值: 区域号


************************************************/
#define TEMP_POINT_MAX 10  //分区温度点个数（N个温度点，划分为N+1个分区）(要满足所有用到此函数的应用)

U08 fun_erea_devide(I16 p_value, U08 s_area, I16 Tpoint[], I16 Diff[], U08 PointMax) //NOTE-CXW 待处理：数组参数改成地址传递
{
    U08 d_erea,num;
    U08 i,j;

	U08 effect_var = 0;				// 有效分区温度点个数
    I16 t_set[TEMP_POINT_MAX]={0}; 
    I16 d_set[TEMP_POINT_MAX]={0}; 	
    
    
    d_erea = s_area;
    if (d_erea < 1 || d_erea > (PointMax+1))// 处理超出有效范围的区域
	{
		d_erea = 1;		// 1为温度最高区域
	}
	
	if(PointMax>TEMP_POINT_MAX || SensorCannotUse(p_value))	// 参与计算的分区温度点最大不超过TEMP_POINT_MAX	 
	{
		return d_erea;
	}
	

	for(i=0; i<PointMax-1; i++)
	{
		if(Tpoint[i]<Tpoint[i+1])	// 如果分区温度点不是由大到小排列，则退出判断区域
		{
			return d_erea;	
		}
		t_set[i] = Tpoint[i];
		d_set[i] = Diff[i];
	}
	t_set[i] = Tpoint[i];	// 赋值最后一个温度点
	d_set[i] = Diff[i];

	    
    //整理分区变量，将重合的分区变量置为0x8000
    for(num=0,i=num+1; i<PointMax; i++)
    {
        if(t_set[num] == (I16)0x8000)//无意义的数据不比较,增加算法执行速度(有符号数不能用0xffff(-1),0xff(255))
        {
            i = ++num;//比较下一个数
            continue;
        }
        else if(t_set[num] == t_set[i])//一个数据和后面数据逐个比较
        {
            t_set[i] = 0x8000;
            d_set[i] = 0;	
        }

        if(i == PointMax-1) //比较对象到最后一个数为止,
        {
            i = ++num;//比较下一个数
        }

        if(num == PointMax-1) break;//比较到最后一个数退出
    }
	

	    
    //计算有效分区温度点个数,排除0x8000无效数据(移到数组最后)
    effect_var = PointMax;  

    for(i=0; i<effect_var; i++)//有效数据判断，随着无效数据被排除，检测范围变小
    {
        if(t_set[i] == (I16)0x8000)
        {
            if(effect_var>0)
            {
                effect_var--;//有效数据减少一个 
            }
            for(j=i; j<effect_var; j++)
            { 	
            	t_set[j]=t_set[j+1];//将所有数据向前移动一次，最后被移动的数据号是effect_var。
            	d_set[j]=d_set[j+1];

            }
            t_set[effect_var] = 0x8000;
            d_set[effect_var] = 0;
			if(i>0)
            {
                i--;    //重新判断i位置数据
			}
        }

    }


	// 根据当前值确定分区
	for(i=0; i<effect_var; i++)	// 只计算有效个数
	{
		if(d_erea == 1)	// 当前在温度最高区域
    	{
    		if (p_value <= t_set[0])
    		{
    			d_erea ++;	
    		}
    		else 
    		{	
    			break;	// 区域没变化，退出for
    		}
    	}
    	else if(d_erea == (PointMax+1))	// 当前在温度最低区域
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
    		else	// 区域没变化，退出for
    		{	
    			break;	
    		}	
    	}
    	
	}
    	
    //屏蔽无意义区域
    if (d_erea > effect_var+1)
    {
    	d_erea = effect_var+1;
    }
    
    return d_erea;
}
/*****************************************************************************
函  数 : Fun_LookupArea()
功  能 : 回差分区控制,支持0分区(即相邻界限相同)
参  数 : I16 val  : 当前值
         U08 area : 当前分区
         I16 high : 高分区界限
         I16 mid  : 
         I16 norm : 
         I16 low  : 
         I16 diff : 
返  回 : 计算后分区
其  他 : by:zzp 思路:先判断是否切换分区,在重新分区
*****************************************************************************/
U08 Fun_LookupArea(I16 val, U08 area, I16 high, I16 mid, I16 norm, I16 low,I16 diff)
{
    BOL area_ch = FALSE;
    U08 new_area = area;

    //3 分区是否合理,暂时不支持错误分区
    if (!(high >= mid && mid >= norm && norm >= low))
    {
        return area;
    }
    
    
    //3 判断分区是否改变
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
    

    //3 分区改变时,重新分区
    if (area_ch)
    {
        // 以下"<="是为了处理分区界限相等时
        if (high <= val)                    new_area = AREA_1;
        else if (mid <= val && val <= high) new_area = AREA_2;
        else if (norm <= val && val <= mid) new_area = AREA_3;
        else if (low <= val && val <= norm) new_area = AREA_4;
        else                                new_area = AREA_5;
    }

    return new_area;
}
/*****************************************************************************
函  数 : fun_area()
功  能 : 根据分区点分区
参  数 : I16 val  : 当前值
         U08 area : 当前分区
         I16 diff : 分区回差
         ...      : 分区点(可变)
返  回 : 新分区
其  他 : 1、分区点最后一个参数必须为SENSOR_NOEXIST,建议使用AREA_PT宏
         2、首个分区的分区值是0，与其他一些程序定义的分区起始值为1不同
		 3、支持分区点由高到低排列


                    A区                             SENSOR_NOEXIST             

--------------------------------------------------  p1      --
..................................................  p1-d     | 例:
                                                             | 超出当前分区范围
                    B区                                      | 则重新分区
--------------------------------------------------  p2       |
..................................................  p2-d    __

                    C区
--------------------------------------------------  p3
..................................................  p3-d

                    D区                             SENSOR_NOEXIST

                                                    
*****************************************************************************/
U08 fun_area(I16 val, U08 area, I16 diff, ...)
{
//    #define MAX_AREA_POINT  20				/* 最多支持的分区点数 */
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

    //3 分区校验
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

    //3 判断分区是否改变
    if (!fg_area_err)
    {
        I16 area_up = 0;
        I16 area_lo = 0;

        //4 当前分区的上下限
        if (area == 0)              area_up = SENSOR_NOEXIST,   area_lo = point[area] - diff;
        else if (area >= max_area)  area_up = point[area-1],    area_lo = SENSOR_NOEXIST;
        else                        area_up = point[area-1],    area_lo = point[area] - diff;
        //4 判断是否超出上下限
        if (   (val < area_lo && SensorCanUse(area_lo))
            || (val > area_up && SensorCanUse(area_up)))
        {
            fg_area_changed = TRUE;
        }
    }

    //3 分区改变时,重新分区
    if (fg_area_changed)
    {
        I16 area_up = 0;
        I16 area_lo = 0;
        
        for (i=0; i<=max_area; i++)
        {
            //4 每个分区的上下限
            if (i == 0)             area_up = SENSOR_NOEXIST,   area_lo = point[i];
            else if (i >= max_area) area_up = point[i-1],       area_lo = SENSOR_NOEXIST;
            else                    area_up = point[i-1],       area_lo = point[i];
            //4 是否落于改分区的上下限内
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
函数：16-2 带回差线性方程(fun_line_diff)
功能：给定两个端点及回差，得到带回差的线性方程(y输出)。
参数：x当前值；起点x坐标；起点y坐标；终点x坐标；终点y坐标；回差。
返回：y目标值
其它：/。
*****************************************************************/
#define DIR_POS     0       /* 正方向 */
#define DIR_NEG     1       /* 负方向 */

I16 fun_line_diff(I16 value, I16 x_bgn, I16 y_bgn, I16 x_end, I16 y_end, I16 diff)
{
    I16 dest_value = y_bgn;
    I16 temp_value = value;
    static I16 valu_bak = 0;
    static BOOL fg_dir = DIR_POS;           /* 初始化为正方向 */
    
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
            else                    /* 负方向 */
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
函数：16-3 查等步进线性表(fun_tab_lookup)
功能：查等步进线性表。
参数：当前值；表结构体指针；是否四舍五入。
返回：y目标值
其它：/。
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
            dest_value = tab->t0;       /* y起点值 */
        }
        else if (value >= tab->v[end])      
        {
            dest_value = tab->tn;       /* y终点值 */
        }
        else
        {
            U16 i = 0;
            k = (head + end) >> 1;
            
            while(i++ < tab->num)       /* 防止表排序有问题导致死循环 */
            {
                if (value < tab->v[k])              head = k;
                else if (value > tab->v[k+1])       end = k;
                else                                break;
                
                k = (head + end) >> 1; 
            }
            
            dest_value = tab->t0 + tab->td * k;                 /* 整数部分 */
            dest_value += fun_div_round((I32)tab->td * (value - tab->v[k]), \
                                        tab->v[k+1] - tab->v[k], obj);
        }
    }
    
    return dest_value;
}

/****************************************************************
函数：16-4 查非等步进线性表(fun_tab_lookup)
功能：查非等步进线性表。
参数：当前值；源表指针；目的表指针；元素个数；是否四舍五入。
返回：y目标值
其它：/。
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
函数：16-5 四舍五入(fun_div_round)
功能：带四舍五入的除法操作。
    商为正值时“+0.5”; 为负值时“-0.5”，公式为：商 = (被除数*2+除数) /(除数*2)
参数：被除数(I32)；除数(I32)。
返回：带四舍五入的商(I16)。
其它：/。
*****************************************************************/
I16 fun_div_round(I32 dividend, I32 divisor, U08 obj)
{
    I16 result = dividend;      /* 商初始化为被除数 */
    I16 assist_data = divisor;  /* 辅助数据初始化为除数(+0.5) */
    
    if (divisor != 0)
    {
        if (obj == DIV_NORMAL)  /* 不用四舍五入 */
        {
            assist_data = 0;
        }
        else                    /* 用四舍五入 */
        {
            if (dividend >= 0 && divisor < 0
                || dividend < 0 && divisor > 0)
            {
                assist_data = -divisor;     /* 商为负值时，需要-0.5 */
            }
        }
        
        result = (dividend * 2 + assist_data) / (divisor * 2);
    }
    
    return result;
}

/****************************************************************
函数：16-6 带余数的除法(fun_div_remainder)
功能：带余数的除法操作。
参数：余数指针(I16*)；被除数(I32)；除数(I16)。
返回：商(I16)。
其它：一般在连续累加时用。
*****************************************************************/
I16 fun_div_remainder(I16 *remainder, I32 dividend, I32 divisor)
{
    I32 assist_data = 0;
    
    assist_data = (*remainder) + dividend;  /* 余数+被除数作为本次的被除数 */
    *remainder = assist_data % divisor;     /* 求取本次余数 */
    
    return (assist_data / divisor);         /* 商 */
}


/*********************************************************
功能: 根据最大最小值求取线性结果
参数：自变量当前值; 自变量最大值; 自变量最小值; 结果最大值; 结果最小值;
返回：初始开度
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
函数：tab_lookup
功能：查线性表(升序表)
参数：输入量当前值; 表指针(表结构存放地址)
返回：查表结果(放大倍数由tab_array决定)
/====================================================*/
I16 tab_lookup(I16 src_value, TAB_DATA const *tab_temp)
{
    I16  tmp;
    U16  k, head, trail;

    head = 0;
    trail = tab_temp->num - 1;

    if (src_value <= tab_temp->v[head])
        tmp = tab_temp->t0;  /* 最低测量温度 */
    else if (src_value >= tab_temp->v[trail])
        tmp = tab_temp->tn;  /* 最高测量温度 */
    else
    {
        k = (head + trail) >> 1;
        do
        {
            if (src_value < tab_temp->v[k])    /* 前部 */
            {
                trail = k;
                k = (head + trail) >> 1;
            }
            else if (src_value > tab_temp->v[k+1])   /* 后部 */
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

        tmp = tab_temp->t0 + tab_temp->td * k;     /* 整数部分 */
        tmp += tab_temp->td * (src_value - tab_temp->v[k])
               / (tab_temp->v[k+1] - tab_temp->v[k]);   /* 小数部分 */
    }

    return tmp;
}


/**********************************************************
功能：余数a1 = (a1+a2) % b  
   返回商数 = (a1+a2) / b
    利用上次余数进行除法运算
参数：余数存放地址; 被除数; 除数
返回：商
***********************************************************/
I16 _Math_divide(I16 *a1, I32 a2, I16 b)
{
    I32 a;

    a = (*a1) + a2;
    *a1 = a % b; //余数

    return (a / b); //商数
}
/*==================================================================================
//int fun_absolute_value(int v)
  返回整数的绝对值。
==================================================================================*/
I16 fun_absolute_value(I16 v)
{
    return (v > 0) ? v : -v;
}

/*******************************************************************************
 函 数: fun_time_check()
 功 能: 时间是否正确检查
 参 数: ktime 个时间
 返 回: 正常 TRUE;
        错误 FALSE;
 其 它: 空
*******************************************************************************/
BOOL fun_time_check(TIME ktime)
{
    BOOL result = TRUE;
    
    if(ktime.year < 2000
       || ktime.month==0 || ktime.month>12  
       || ktime.day == 0 || ktime.day > 31
       || ktime.hour >= 24 || ktime.min >=60 || ktime.sec >=60)//时间不在正常范围
    {
        result = FALSE;
    }

    return result;
}

/*******************************************************************************
 函 数: fun_time_compare()
 功 能: 时间大小比较
  参 数: front 前一个时间
         after 后一个时间
 返 回: after >= front TRUE;
        after < front  FALSE;
 其 它: 空
*******************************************************************************/
BOOL fun_time_compare(TIME front, TIME after)
{
    BOOL result = FALSE;

    if(fun_time_check(front) == FALSE || fun_time_check(after) == FALSE)
    {
        return result;//时间不正确
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
 函 数: fun_twotime_sub()
 功 能: 处理两个时间的差值
 参 数: front 前一个时间
        after 后一个时间
 返 回: 两个时间的相差的分钟      
 其 它: 最大为65535分钟 = 1092小时 = 45天，超界时返回0xffff
*******************************************************************************/
U16 fun_twotime_sub(TIME front, TIME after)
{
    I16 years,months,days,hours,minutes;
    U16 i,min,max;  //临时变量
    const I08 MonthMax[]={0,1,-2,1,0,1,0,1,1,0,1,0,1};//每个月与30天的差数。
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
        //补偿闰年的一天
        min = front.year;
        max = after.year;
        for (i=min; i<=max; i++) 
        {
            if (front.month > 2)    continue;
            if (after.month <= 2)   continue;
            if ((i%4==0&&i%100!=0)||(i%400==0))
            {   //  闰年
                result++;   //闰年增加一天
            }
        }
        
        if(result >= 45)//最大天数限制
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
 函 数: fun_power()
 功 能: 几次幂计算
 返 回: num的cnt次方
 其 它: 空 wlg2015.06.12
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
函  数 : fun_pv_cmpst()
功  能 : 模拟量补偿
参  数 : I16 pv   : 模拟量值
         I16 cmps : 补偿值
返  回 : 无
其  他 : 无
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
 函 数: fun_pv_filt_t()
 功 能: 模拟量消抖/滤波(一段时间内)
 参 数: p_result--指向处理结果;
		pv--消抖对象;
		filt--消抖值;
		time--时间计数；
		dly--时间
 返 回: true:触发消抖 false:没触发消抖
 其 它: 
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
 函 数: fun_pv_in()
 功 能: 判断是否在闭区间内
 参 数: v--; 
		low--; 
		high--; 
 返 回: 
 其 它: 空
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
 函 数: fun_pv_filt()
 功 能: 模拟量消抖/滤波
 参 数: p_result--指向处理结果;
		pv--消抖对象;
		filt--消抖值;
 返 回: true:触发消抖 false:没触发消抖
 其 它: 没有做边界保护，注意使用
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
 函 数: fun_pv_in_open()
 功 能: 判断是否在开区间内
 参 数: v--; 
		low--; 
		high--; 
 返 回: 
 其 它: 空
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
 函 数: fun_pv_limit()
 功 能: pv范围限制
 参 数: pv--模拟量; 
		min--最小值; 
		max--最大值; 
 返 回: 
 其 它: 空
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
 函 数: fun_pv_get()
 功 能: 获取模拟量,a不可用时用默认值dft
 参 数: a--模拟量; 
		dft--默认值; 
 返 回: 
 其 它: 空
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
函  数 : Fun_SimFloatDiv()
功  能 : 模拟浮点数除法
参  数 : I32 a   : 被除数
         I32 b   : 除数
         U08 dot : 小数位
返  回 : 商
其  他 : 无
*****************************************************************************/
I32 Fun_SimFloatDiv(I32 a, I32 b, U08 dot)
{
    I32 abs_a = 0;
    I32 times = 0;
    I32 c_a = 1;    /* a的放大系数 */
    I32 c_b = 1;    /* b的缩小系数 */
    U08 d_a = 0;    /* a的放大位数 */
#define MAX_I32     ((I32)0x7FFFFFFF)

    abs_a = a;
    if (abs_a < 0)  abs_a = -a;
    
    while(abs_a < MAX_I32/(10*c_a))/* 为了防止超界，a最大放大到MAX_I32 */
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

    times = ((a*c_a) / (b/c_b));  /* a方式c_a倍，b缩小c_b倍 */

    return times;
}

/*****************************************************************************
函  数 : Fun_Range()
功  能 : a在o的(100±r)%范围内，返回TRUE
参  数 : I16 a : 
         I16 o : 
         U08 r : 
返  回 : 无
其  他 : 无
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
// *  最小二乘法拟合直线，为模拟量校准而设计，故数据类型有一定限制
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

    *K = Fun_SimFloatDiv(sumxy, sumx2, dot); /* k = ∑XY / ∑X^2 */
    *A = Fun_SimFloatDiv(sumx2, sumxy, dot); /* a = ∑X^2 / ∑XY */// _     _
//  *B = ymean*coe - Fun_SimFloatDiv(xmean*sumxy, sumx2, dot); /* b = Y - k*X */ /* xmean*sumxy可能超界 */
    if (Fun_AbsI32(*A) > Fun_AbsI32(*K))        /* 使用大的数，减少精度丢失 */
    {
        *B = ymean*coe - Fun_SimFloatDiv(xmean, *A, dot+dot); /* A放大了dot，故需再放大dot */
    }
    else
    {
        *B = ymean*coe - *K*xmean;
    }
    
}  

 
/*****************************************************************************
函  数 : Fun_NumFormat()
功  能 : 格式化16位数值
参  数 : U16 num    : 数值
         U08* buf   : 目标缓冲地址
         U16 attrib : 格式属性
返  回 : 字节数
其  他 : 无
*****************************************************************************/
U08 Fun_NumFormat(U16 num, U08* buf, U16 attrib) 
{
    #define MAX_LENGHT  8
    U08 length   = 0;     /* 属性中字符数 */
    U08 dot_n    = 0;
    U08 dot_fg   = FALSE;
    U08 nega_fg  = FALSE;
    U08 disp_char[MAX_LENGHT]=" ";
    U08 count = 0;  /* 实际格式化字符数 */
    U08 n = 0;

    num &= 0xFFFF;
    /* 长度 */
    if(attrib&FNS_HEX)
    {
        length = FNS_DIGIT6;
    }
    else
    {
        length =(U08)(attrib & FNS_DIGIT_MASK);
    }
    /* 符号 */
    if (!(attrib&FNS_UNSIGNED))
    {
        if ((I16)num < 0)
        {
            nega_fg = TRUE;
            num = 0-num;
        }
    }
    /* 小数位 */
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
                num = num/16;//去掉已经显示的位
                dot_fg = FALSE;
            }
            else
            {
                disp_char[count++] = num%10+'0';
                num = num/10;//去掉已经显示的位
            }
            if (dot_fg)
            {
                dot_n--;
                if ((dot_n == 0)&&(num != 0))//小数点前面不为0，则显示小数点
                {
                    disp_char[count++] = '.';
                    dot_fg = FALSE;
                }
            }
        }
    }
    if (dot_fg)
    {
        while (dot_n > 0)//补全小数点后面的0
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

    // 未定义长度时,改为使用实际长度
    if(length == 0) length = count;

    if(attrib&FNS_RIGHT)//右对齐
    {
        U08 c=' ';
        if(attrib&FNS_ZERO)
            c = '0';

        for(n=0; n<count&&n<length; n++)
        {
            buf[length-n-1]= disp_char[n];
        }

        while (count < length)//未显示满，填补空格
        {
            buf[length-count-1]= c;
            count++;
        }
    }
    else//左对齐
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
        while (count < length)//未显示满，填补空格
        {
            buf[count++] = c;
        }
    }

    buf[count] = '\0';

    return count;
}  

#ifdef CRC_SPEED_PRI
/*******************************************************************************
 函 数: fun_crc16()
 功 能: CRC16计算
 参 数: puchMsg--数组指针 
		usDataLen--数组长度 
 返 回: CRC计算结果
 其 它: 空
*******************************************************************************/
unsigned short fun_crc16(const unsigned char *puchMsg, unsigned short usDataLen)
{
	unsigned char uchCRCHi = 0xFF;			/* 高CRC字节初始化 */
	unsigned char uchCRCLo = 0xFF;			/* 低CRC字节初始化 */
	unsigned char uIndex;					/* CRC循环中的索引 */
	
	while ((usDataLen--) != 0)				/* 传输消息缓冲区 */
	{
		uIndex = uchCRCHi ^ *(puchMsg++);	/* 计算CRC */
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
		uchCRCLo = auchCRCLo[uIndex];
	}
	
	return ((unsigned short)uchCRCLo<<8 | (unsigned short)uchCRCHi);
}
#else
/*******************************************************************************
 函 数: fun_crc16()
 功 能: CRC16计算
 参 数: puchMsg--数组指针 
		usDataLen--数组长度 
 返 回: CRC计算结果
 其 它: 空
*******************************************************************************/
unsigned short fun_crc16(const U08 *pData, U16 len)
{
    U08 j; 
    U16 crcReg;				/* CRC寄存器 */
	#undef CRC_POLY
    #define CRC_POLY 0xa001	/* CRC多项式 */
    
    crcReg = 0xffff;		/* 初始化寄存器 */
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
 函 数: fun_crc16_ok()
 功 能: CRC16校验OK?(包括校验码)
 参 数: pData--数据; 
		lenIncCrc--个数(包括校验码); 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
*******************************************************************************/
BOOL fun_crc16_ok(const U08 *pData, U16 lenIncCrc)
{
	return (fun_crc16(pData, lenIncCrc) == 0x0000);		
}

/*****************************************************************************
函  数 : Fun_CalAverageFilt()
功  能 : 求滤除最大最小值的平均值
参  数 : I16 src[] : 
         U08 n     : 
返  回 : 无
其  他 : 无
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
函  数 : fun_filt_avg()
功  能 : 求滤除最大最小值的平均值,考虑无效值
参  数 : I16 src[] : 
         U08 n     : 
返  回 : 无
其  他 : 无
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
            // 统计可用的值
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

            // 判断有效的个数
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
函  数 : fun_all_avg()
功  能 : 求全部的平均值,考虑无效值
参  数 : I16 src[] : 
         U08 n     : 
返  回 : 无
其  他 : 无
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
            // 统计可用的值
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

            // 判断有效的个数
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
                    /* 不去除最大最小值 */
                    // sum -= (min + max); 
                    if (sum >= 0)    avg = (sum + ((can_use_n)>>1)) / (can_use_n);
                    else             avg = (sum - ((can_use_n)>>1)) / (can_use_n);

                    /* 去除最大最小值 */
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
函  数 : fun_rec_avg()
功  能 : 数据记录,并返回平均值,去除最大最小 
参  数 : I16 val   : 当前值
         I16* data : 数据指针
         U08 num   : 总个数
返  回 : 记录的平均值
其  他 : 无
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
函  数 : fun_stab_avg()
功  能 : 判断是否在平均值[d_lower, d_upper]内稳定,去除最大最小
参  数 : I16* data   : 数据
         U08 num     : 数据个数
         I16 d_lower : 偏差下限
         I16 d_upper : 偏差上限
返  回 : TRUE-稳定,FALSE-不稳定
其  他 : by:zzp 思路: 先去除最大最小,判断次大次小是否都在要求范围内
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
            /* 查找最大最小 */
            min = max = data[0];
            min_i = max_i = 0;
            for (sum=0,idx=0,cnt=0; idx<num; idx++)
            {
                s = data[idx];
				sum += s, cnt++;    // 求和,并统计个数
                if(s < min) min = s, min_i = idx;
                if(s > max) max = s, max_i = idx;
            }

            if (num >= 4) // 至少4个个才去除最大最小值
            {
                /* 去除最大最小,找次大次小 */
                for (sum=0,idx=0,cnt=0; idx<num; idx++)
                {
                    if (idx == min_i || idx == max_i)   continue;
                    s = data[idx];
                    if (first)  
                    {
                        first = FALSE;
                        min_2 = max_2 = s;
                    }
                    sum += s, cnt++;    // 求和,并统计个数
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
 函 数: fun_rtc_check()
 功 能: LOG_RTC时间是否正确检查
 参 数: ktime 个时间
 返 回: 正常 TRUE;
        错误 FALSE;
 其 它: 空
*******************************************************************************/
BOOL fun_rtc_check(LOG_RTC ktime)
{
    BOOL result = TRUE;

    if(!((ktime.year>=2000 && ktime.year<=2099) || (ktime.year>=0 && ktime.year<=99))	/* 兼容00~99与2000~2099 */
       || ktime.month==0 || ktime.month>12  
       || ktime.day==0 || ktime.day>31
       || ktime.hour>=24 || ktime.min>=60 || ktime.sec>=60)//时间不在正常范围
    {
        result = FALSE;
    }

    return result;
}
/******************************************************************************
算法解析：

该算法总体思想是计算给定日期到 0年3月1日的天数，然后相减，获取天数的间隔。

m1 = (month_start + 9) % 12; 用于判断日期是否大于3月（2月是判断闰年的标识），还用于纪录到3月的间隔月数。

y1 = year_start - m1/10; 如果是1月和2月，则不包括当前年（因为是计算到0年3月1日的天数）。

d1 = 365*y1 + y1/4 - y1/100 + y1/400 + (m1*306 + 5)/10 + (day_start - 1);

    其中 365*y1 是不算闰年多出那一天的天数，

    y1/4 - y1/100 + y1/400  是加所有闰年多出的那一天，

(m2*306 + 5)/10 用于计算到当前月到3月1日间的天数，306=365-31-28（1月和2月），5是全年中不是31天月份的个数

(day_start - 1) 用于计算当前日到1日的间隔天数

******************************************************************************/


/*****************************************************************************
函  数 : fun_time_days()
功  能 : 计算日期到0000年3月1日的天数
参  数 : TIME time : 日期
返  回 : 无
其  他 : 无
*****************************************************************************/
I32 fun_time_days(LOG_RTC* time)
{
	I32 y, m, d;
	U16 year_temp = time->year>=2000 ? time->year : time->year+2000;	/* 兼容00~99与2000~2099 */

	m = (time->month + 9) % 12;
	y = year_temp - m/10;
	d = 365*y +                 // 年天数
        (y/4 - y/100 + y/400) + // 闰年偏差天数
        (m*306 + 5)/10 +        // 月天数
        (time->day - 1);         // 天数

    return  d;
}

/*****************************************************************************
函  数 : fun_time_secs()
功  能 : 计算日期当天的秒数
参  数 : TIME time : 日期
返  回 : 无
其  他 : 无
*****************************************************************************/
I32 fun_time_secs(LOG_RTC* time)
{
    I32 secs;

    secs = time->hour*3600L + time->min*60 + time->sec;

    return secs;
}

/*****************************************************************************
函  数 : fun_time_diff()
功  能 : 计算两个日期差
参  数 : TIME front    : 
         TIME after    : 
         U08 diff_type : 
返  回 : 无
其  他 : 该函数用于计算两个时间之间的差值，不能用于识别跨分、跨时、跨天
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
 函 数: fun_time_diff_day()
 功 能: 计算两个日期间隔天数
 参 数: front--; 
		after--; 
 返 回: 
 其 它: 空
*******************************************************************************/
I32 fun_time_diff_day(LOG_RTC* front, LOG_RTC* after)
{
    I32 diff_day = 0;

    diff_day = fun_time_days(after) - fun_time_days(front);

    return diff_day;
}

#define BASE_YEAR 2000		/* 为“兼容00~99与2000~2099”，不能小于2000 */
/*****************************************************************************
函  数 : fun_time_mktime()
功  能 : 距2000年1月1日0时0分0秒的时间戳(秒数)[北京时间]
参  数 : TIME* now : 当前时间.
返  回 : 时间戳(秒数)
其  他 : 无
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
函  数 : fun_time_localtime()
功  能 : 根据距2000年1月1日0时0分0秒的时间戳(秒数)计算日期[北京时间]
参  数 : U32 secs : 时间戳
返  回 : 日期
其  他 : 无
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
    // 天数
    remain_days = secs / (24L*60*60);
    // 星期
    t.week = (remain_days + 5)%7 + 1; // 2000.1.1是星期六(5)，若更改BASE_YEAR需更新这里
    // 年
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
    
    // 月
    for (m=0; m<12; m++)
    {
        U08 m_days = days[m];
        
        if (m == 1) // 2月
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

    // 日
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
            length = 3;   // 显示长度默认为3
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
                num = num/16;//去掉已经显示的位
            }
            else
            {
                disp_char[count++] = num%10+'0';
                num = num/10;//去掉已经显示的位
            }
            if (dot_fg)
            {
                dot_n--;
                if ((dot_n == 0)&&(num != 0))//小数点前面不为0，则显示小数点
                {
                    disp_char[count++] = '.';
                    dot_fg = FALSE;
                }
            }
        }
    }
    if (dot_fg)
    {
        while (dot_n > 0)//补全小数点后面的0
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

    if(attrib&FDNS_SNAP_LEFT)//左对齐
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
        while (count < length)//未显示满，填补空格
        {
            buf[count++] = c;
        }
    }
    else//右对齐
    {
        U08 c=' ';
        if(attrib&FDNS_ZERO)
            c = '0';
        
        for(n=0; n<count&&n<length; n++)
        {
            buf[length-n-1]= disp_char[n];
        }

        while (count < length)//未显示满，填补空格
        {
            buf[length-count-1]= c;
            count++;
        }
    }

    return count;
}

/*****************************************************************************
函  数 : fun_bit_first_1()
功  能 : 查找第一个1所在是bit位
参  数 : U16 word[] : 
         U16 words  : 
返  回 : 位置
其  他 : 如未找到,返回0xFFFF
*****************************************************************************/
U16 fun_bit_first_1(U16 word[], U16 words)
{
    U16 idx;
    U16 no = 0xFFFF;
    
    for (idx=0; idx<16*words; idx++) // 第一个故障号
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
 函 数: fun_find_node_from_arr()
 功 能: 从有序数组中获取坐标点位置
 参 数: pv--当前值; 
		arr[]--有序数组; 
		arr_size--数组长度; 
		dir--序列的升序或降序; 
 返 回: 
 其 它: 空
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
					break;	/* 跳出for */
	            }
	        }
			result = i;
	        break;
	    case DIR_DCD:
	        for (i=0; i<arr_size; i++)
	        {
	            if (pv >= arr[i])
	            {
					break;	/* 跳出for */
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
函  数 : get_relative_pos()
功  能 : 获取指定绝对位置在不同大小的块中的相对位置
参  数 : U16 blocks_size[] : 块大小
         U16 num           : 块个数
         U16 idx           : 绝对位置
返  回 : 相对位置
其  他 : 无
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
 函 数: fun_value_trend()
 功 能: 当前值变化趋势判断
 参 数: val--当前值; 
		avg--平均值; 
		diff--分区点; 
 返 回: 持平/上升/下降
 其 它: 设计目的：相比fun_line_trend能在更短时间内判断出当前的变化趋势，
        若无法达到设计目的，则删除此函数，使用fun_line_trend替代
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
 函 数: fun_sort_bubble()
 功 能: 冒泡排序
 参 数: arr[]--待排序列; 
		n--序列长度; 
		dir--DIR_ACD、DIR_DCD; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void fun_sort_bubble(I16 arr[], U08 n, U08 dir)
{
    U08 i=0, j=0;

    if (n < 2)  return;

    for (i=0; i<n-1; i++)
    {
        BOL fg_chg = FALSE;	/* 用于提前判断排序完成 */
        
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
 函 数: fun_check_fg_arr()
 功 能: 检查标志位数组是否存在置位，如存在置位，则返回TRUE
 参 数: fg_arr--标志位数组; 
		bit_num--标志位数量; 
		is_once--是否为一次性标志，TRUE:遍历后自动清空, FALSE:只遍历不清空 
 返 回: TRUE/FALSE--成功/失败
 其 它: 空
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
        MEMCLR(fg_arr, _DIVUCARRY(bit_num, 16));			/* 注意: 此处不可以用ARRCLR宏，因为此处无法使用sizeof计算fg_arr真实长度 */
    }

    return result;
}


/*******************************************************************************
 函 数: fun_copy_str_to_U16()
 功 能: 把字符串存到U16数组
 参 数: 空
 返 回: 空
 其 它: 空
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

