/******************************************************************************

          版权所有 (C), 2001-2016, 广州市邦普电脑技术开发有限公司

 ******************************************************************************
  文 件 名   : Work_frequency.c
  版 本 号   : 初稿
  作    者   : 邝杰洪
  生成日期   : 2020-5-17
  最近修改   :
  功能描述   : 工况频率表
  函数列表   :
  修改历史   :
  1.日    期   : 2020-5-17
    作    者   : 邝杰洪
    修改内容   : 创建文件

******************************************************************************/

/*-----------------包含头文件 -----------------------------*/
#include "Includes.h"


/*-----------------外部变量说明----------------------------*/

#define WF_FREQ_MAX val_invt_rps_max
#define WF_FREQ_MIN val_invt_rps_min

#define WF_FREQ_TAB_UP_X_CL_N	TAB_UP_X_CL
#define WF_FREQ_TAB_UP_Y_CL_N	TAB_UP_Y_CL
#define WF_FREQ_TAB_UP_X_HT_N	TAB_UP_X_HT
#define WF_FREQ_TAB_UP_Y_HT_N	TAB_UP_Y_HT
#define WF_FREQ_TAB_UP_CL(y, x) val_cl_up_env_in(y, x)
#define WF_FREQ_TAB_UP_HT(y, x) val_ht_up_env_in(y, x)

#define WF_FREQ_TAB_DN_X_CL_N	TAB_DN_X_CL
#define WF_FREQ_TAB_DN_Y_CL_N	TAB_DN_Y_CL
#define WF_FREQ_TAB_DN_X_HT_N	TAB_DN_X_HT
#define WF_FREQ_TAB_DN_Y_HT_N	TAB_DN_Y_HT
#define WF_FREQ_TAB_DN_CL(y, x) val_cl_dn_env_in(y, x)
#define WF_FREQ_TAB_DN_HT(y, x) val_ht_dn_env_in(y, x)


/*-----------------外部函数原型说明------------------------*/



/*-----------------内部函数原型说明------------------------*/



/*-----------------全局变量--------------------------------*/



/*-----------------模块级变量------------------------------*/
/* 制冷 */
/* 上限 */
#if defined(USE_WF_SHENGNENG)
I16 wf_tab_up_x_cl_env[WF_FREQ_TAB_UP_X_CL_N] = {480, 430, 350, -100};		
#else
I16 wf_tab_up_x_cl_env[WF_FREQ_TAB_UP_X_CL_N] = {500, 430, 350, -50};	
#endif

#if defined(USE_WF_OLD)
I16 wf_tab_up_y_cl_in[WF_FREQ_TAB_UP_Y_CL_N] = {50, 200, 350};	
#elif defined(USE_WF_SHENGNENG)
I16 wf_tab_up_y_cl_in[WF_FREQ_TAB_UP_Y_CL_N] = {50, 120, 200, 350};	
#else
I16 wf_tab_up_y_cl_in[WF_FREQ_TAB_UP_Y_CL_N] = {50, 150, 230, 350};	
#endif

I16 wf_tab_cl_freq_max[WF_FREQ_TAB_UP_Y_CL_N][WF_FREQ_TAB_UP_X_CL_N];
I16* wf_list_cl_freq_max[WF_FREQ_TAB_UP_Y_CL_N] = {
    wf_tab_cl_freq_max[ 0],
    wf_tab_cl_freq_max[ 1],
    wf_tab_cl_freq_max[ 2],  
#ifndef USE_WF_OLD
    wf_tab_cl_freq_max[ 3], 
#endif    
};

const TAB_STRU wf_tab_up_cool =  
{
    wf_tab_up_x_cl_env, 
    WF_FREQ_TAB_UP_X_CL_N, 
    wf_tab_up_y_cl_in, 
    WF_FREQ_TAB_UP_Y_CL_N,  
    wf_list_cl_freq_max
};
/* 下限 */
#if defined(USE_WF_SHENGNENG)
I16 wf_tab_dn_x_cl_env[WF_FREQ_TAB_UP_X_CL_N] = {480, 430, 350, -100};		
#else
I16 wf_tab_dn_x_cl_env[WF_FREQ_TAB_UP_X_CL_N] = {500, 430, 350, -50};	
#endif
	
#if defined(USE_WF_OLD)
I16 wf_tab_dn_y_cl_in[WF_FREQ_TAB_DN_Y_CL_N] = {50, 200, 350};
#elif defined(USE_WF_SHENGNENG)
I16 wf_tab_dn_y_cl_in[WF_FREQ_TAB_DN_Y_CL_N] = {50, 120, 200, 350};	
#else
I16 wf_tab_dn_y_cl_in[WF_FREQ_TAB_DN_Y_CL_N] = {50, 150, 230, 350};	
#endif

I16 wf_tab_cl_freq_min[WF_FREQ_TAB_DN_Y_CL_N][WF_FREQ_TAB_DN_X_CL_N];
I16* wf_list_cl_freq_min[WF_FREQ_TAB_DN_Y_CL_N] = {
    wf_tab_cl_freq_min[ 0],
    wf_tab_cl_freq_min[ 1],
    wf_tab_cl_freq_min[ 2],  
#ifndef USE_WF_OLD
    wf_tab_cl_freq_min[ 3], 
#endif      
};
const TAB_STRU wf_tab_dn_cool =  
{
    wf_tab_dn_x_cl_env, 
    WF_FREQ_TAB_DN_X_CL_N, 
    wf_tab_dn_y_cl_in, 
    WF_FREQ_TAB_DN_Y_CL_N,  
    wf_list_cl_freq_min
};

/* 制热 */
/* 上限 */
#if defined(USE_WF_SHENGNENG)
I16 wf_tab_up_x_ht_env[WF_FREQ_TAB_UP_X_HT_N] = {430, 70, 20, -70, -120, -250};		
#else
I16 wf_tab_up_x_ht_env[WF_FREQ_TAB_UP_X_HT_N] = {430, 70, 0, -70, -120, -250};
#endif

#if defined(USE_WF_OLD)
I16 wf_tab_up_y_ht_in[WF_FREQ_TAB_UP_Y_HT_N] = {150, 400, 550};	
#elif defined(USE_WF_SHENGNENG)
I16 wf_tab_up_y_ht_in[WF_FREQ_TAB_UP_Y_HT_N] = {150, 300, 400, 550};
#else
I16 wf_tab_up_y_ht_in[WF_FREQ_TAB_UP_Y_HT_N] = {150, 300, 400, 550};
#endif

I16 wf_tab_ht_freq_max[WF_FREQ_TAB_UP_Y_HT_N][WF_FREQ_TAB_UP_X_HT_N];
I16* wf_list_ht_freq_max[WF_FREQ_TAB_UP_Y_HT_N] = {
    wf_tab_ht_freq_max[ 0],
    wf_tab_ht_freq_max[ 1],
    wf_tab_ht_freq_max[ 2],  
#ifndef USE_WF_OLD
    wf_tab_ht_freq_max[ 3], 
#endif        
};
const TAB_STRU wf_tab_up_heat =  
{
    wf_tab_up_x_ht_env, 
    WF_FREQ_TAB_UP_X_HT_N, 
    wf_tab_up_y_ht_in, 
    WF_FREQ_TAB_UP_Y_HT_N,  
    wf_list_ht_freq_max
};

#if defined(USE_WF_R290)
I16 wf_tab_up_y_ht_in_R290[WF_FREQ_TAB_UP_Y_HT_N] = {150, 500, 620, 750};
const TAB_STRU wf_tab_up_heat_R290 =  
{
    wf_tab_up_x_ht_env, 
    WF_FREQ_TAB_UP_X_HT_N, 
    wf_tab_up_y_ht_in_R290, 
    WF_FREQ_TAB_UP_Y_HT_N,  
    wf_list_ht_freq_max
};
#endif

/* 下限 */
#if defined(USE_WF_SHENGNENG)
I16 wf_tab_dn_x_ht_env[WF_FREQ_TAB_DN_X_HT_N] = {430, 20, -120, -250};	
#else
I16 wf_tab_dn_x_ht_env[WF_FREQ_TAB_DN_X_HT_N] = {430, 0, -120, -250};
#endif

#if defined(USE_WF_OLD)  /* 四段时下限与水温无关，故这里合并处理 */
I16 wf_tab_dn_y_ht_in[WF_FREQ_TAB_DN_Y_HT_N] = {150, 400, 550};
#elif defined(USE_WF_SHENGNENG)
I16 wf_tab_dn_y_ht_in[WF_FREQ_TAB_DN_Y_HT_N] = {150, 300, 400, 550};		
#else
I16 wf_tab_dn_y_ht_in[WF_FREQ_TAB_DN_Y_HT_N] = {150, 300, 400, 550};
#endif

I16 wf_tab_ht_freq_min[WF_FREQ_TAB_DN_Y_HT_N][WF_FREQ_TAB_DN_X_HT_N];
I16* wf_list_ht_freq_min[WF_FREQ_TAB_DN_Y_HT_N] = {
    wf_tab_ht_freq_min[ 0],
    wf_tab_ht_freq_min[ 1],
    wf_tab_ht_freq_min[ 2],  
#ifndef USE_WF_OLD
    wf_tab_ht_freq_min[ 3], 
#endif       
};
const TAB_STRU wf_tab_dn_heat =  
{
    wf_tab_dn_x_ht_env, 
    WF_FREQ_TAB_DN_X_HT_N, 
    wf_tab_dn_y_ht_in, 
    WF_FREQ_TAB_DN_Y_HT_N,  
    wf_list_ht_freq_min
};

/*-----------------常量定义--------------------------------*/



/*-----------------宏定义 ---------------------------------*/





/*******************************************************************************
 函 数: wf_init()
 功 能: 初始化
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void wf_init(void)
{
    U08 x, y;
    for (x=0; x<WF_FREQ_TAB_UP_X_CL_N; x++)
    for (y=0; y<WF_FREQ_TAB_UP_Y_CL_N; y++)
    {
        wf_tab_cl_freq_max[y][x] = WF_FREQ_TAB_UP_CL(y, x);
    }

    for (x=0; x<WF_FREQ_TAB_UP_X_HT_N; x++)
    for (y=0; y<WF_FREQ_TAB_UP_Y_HT_N; y++)
    {
        wf_tab_ht_freq_max[y][x] = WF_FREQ_TAB_UP_HT(y, x);
    }

    for (x=0; x<WF_FREQ_TAB_DN_X_CL_N; x++)
    for (y=0; y<WF_FREQ_TAB_DN_Y_CL_N; y++)
    {
        wf_tab_cl_freq_min[y][x] = WF_FREQ_TAB_DN_CL(y, x);
    }

    for (x=0; x<WF_FREQ_TAB_DN_X_HT_N; x++)
    for (y=0; y<WF_FREQ_TAB_DN_Y_HT_N; y++)
    {
        wf_tab_ht_freq_min[y][x] = WF_FREQ_TAB_DN_HT(y, x);
    }
}

/*******************************************************************************
 函 数: wf_LookupFace()
 功 能: 面查找
 参 数: x--x值; 
		y--y值; 
		tab--二维表信息; 
 返 回: 
 其 它: 由Fun_LookupFace更改而来
*******************************************************************************/
static I16 wf_LookupFace(I16 x, I16 y, const TAB_STRU *tab)
{
    U08 a;
    // 表轴方向
    U08 dir_x = DIR_ACD, dir_y = DIR_ACD;
    // 段
    U16 i, j;				/* 分别对应x、y的段位 */
    U16 i1, i2, j1, j2;		/* 分别对应x、y的左段位、右段位 */
    // 坐标
    I16 x1, x2, y1, y2;		/* 分别对应x的左坐标、右坐标；y的上坐标、下坐标 */
    // 值
    I16 v11, v12, v21, v22;	
    I16 vx1, vx2, v1y, v2y;	
    I16 vxy, vyx, val;		/* 先x后y的结果、先y后x的结果、最终结果 */

    /* 确定表轴的方向 */
	for (a=1; a<tab->x_node_len; a++)
	{
	    if (tab->x_node[a-1] > tab->x_node[a])
	    {
	        dir_x = DIR_DCD;
			break;
	    }
	}
	for (a=1; a<tab->y_node_len; a++)
	{
	    if (tab->y_node[a-1] > tab->y_node[a])
	    {
	        dir_y = DIR_DCD;
			break;
	    }
	}

    i = fun_find_node_from_arr(x, tab->x_node, tab->x_node_len, dir_x);
    j = fun_find_node_from_arr(y, tab->y_node, tab->y_node_len, dir_y);

    /* 边界坐标处理 */
    if (i <=0 )                         i1 = i2 = 0;
    else if (i >= tab->x_node_len)      i1 = i2 = tab->x_node_len-1;
    else                                i1 = i-1, i2 = i;

    if (j <=0 )                         j1 = j2 = 0;
    else if (j >= tab->y_node_len)      j1 = j2 = tab->y_node_len-1;
    else                                j1 = j-1, j2 = j;

    {
        // 计算坐标
        x1 = tab->x_node[i1];
        x2 = tab->x_node[i2];
        y1 = tab->y_node[j1];
        y2 = tab->y_node[j2];

        /* 计算四个点的值 */
        v11 = tab->v[j1][i1];
        v21 = tab->v[j1][i2];
        v12 = tab->v[j2][i1];
        v22 = tab->v[j2][i2];
        
        /* 先x 后 y */
        vx1 = fun_line_value(x, x1, x2, v11, v21);
        vx2 = fun_line_value(x, x1, x2, v12, v22);
        vxy = fun_line_value(y, y1, y2, vx1, vx2);

        /* 先y 后 x */
        v1y = fun_line_value(y, y1, y2, v11, v12);
        v2y = fun_line_value(y, y1, y2, v21, v22);
        vyx = fun_line_value(x, x1, x2, v1y, v2y);

        // 取平均值
        val = (vxy + vyx) / 2;
    }
    
    return val;
}

/*****************************************************************************
函  数 : wf_get_freq_max_by_env_in()
功  能 : 环温回温获取频率上限
参  数 : I16 temp_env : 环温
         I16 temp_in : 回温
返  回 : 压机频率上限
其  他 : 无
*****************************************************************************/
I16 wf_get_freq_max_by_env_in(U08 mode, I16 temp_env, I16 temp_in)
{
    const TAB_STRU *tab = mode==MODE_COOL ? &wf_tab_up_cool : &wf_tab_up_heat;

#if defined(USE_WF_R290)
    if (val_refrigerant_type == RF_R290)
    {
        tab = mode==MODE_COOL ? &wf_tab_up_cool : &wf_tab_up_heat_R290;
    }
#endif
    
    if (SensorCannotUse(temp_env) || SensorCannotUse(temp_in))
    {
        return WF_FREQ_MAX;
    }
    
    return wf_LookupFace(temp_env, temp_in, tab);
}

/*****************************************************************************
函  数 : wf_get_freq_min_by_env_in()
功  能 : 环温回温获取频率下限
参  数 : I16 temp_env : 环温
         I16 temp_in : 回温
返  回 : 压机频率下限
其  他 : 无
*****************************************************************************/
I16 wf_get_freq_min_by_env_in(U08 mode, I16 temp_env, I16 temp_in)
{
    const TAB_STRU *tab = mode==MODE_COOL ? &wf_tab_dn_cool : &wf_tab_dn_heat;
    
    if (SensorCannotUse(temp_env) || SensorCannotUse(temp_in))
    {
        return WF_FREQ_MIN;
    }
    
    return wf_LookupFace(temp_env, temp_in, tab);
}

#ifdef USE_WF_TEST
static U16 freq_cl[2][7][7];
static I16 env_cl[] = {460, 400, 380, 350, 300, 250, -50};
static I16 in_cl[]  = {80, 120, 180, 200, 250, 300, 350};
static U16 freq_ht[2][7][16];
static I16 env_ht[] = {430, 200, 120, 70, 50, 20, 0, -20, -40, -70, -80, -100, -120, -150, -180, -250};
static I16 in_ht[]  = {200, 250, 300, 380, 400, 450, 500};
void wf_test(void)
{
    U08 a, b;
    for (a=0; a<ARRELE(env_cl); a++)
    for (b=0; b<ARRELE(in_cl); b++)
    {
        freq_cl[0][b][a] = wf_get_freq_max_by_env_in(MODE_COOL, env_cl[a], in_cl[b]);
        freq_cl[1][b][a] = wf_get_freq_min_by_env_in(MODE_COOL, env_cl[a], in_cl[b]);
    }

    for (a=0; a<ARRELE(env_ht); a++)
    for (b=0; b<ARRELE(in_ht); b++)
    {
        freq_ht[0][b][a] = wf_get_freq_max_by_env_in(MODE_HEAT, env_ht[a], in_ht[b]);
        freq_ht[1][b][a] = wf_get_freq_min_by_env_in(MODE_HEAT, env_ht[a], in_ht[b]);
    }
}    
#endif

