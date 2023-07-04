/*******************************************************************************
 Copyright (c) 广州市邦普电脑技术开发有限公司 All rights reserved.
 文件名称: fun.h
 文件描述: 
 创建人  : YX
 创建日期: 2014.12.13
*******************************************************************************/
#ifndef FUN_H
#define FUN_H

#ifdef  FUN_GLOBAL
    #define FUN_EXT
#else
    #define FUN_EXT extern
#endif

/* 分区结果定义 */
#define AREA_INVALID    0       /* 无效区域(输入参数错误) */
#define AREA_L          1       /* 低区域 */
#define AREA_M          2       /* 中区域 */
#define AREA_H          3       /* 高区域 */
#define AREA_HH         4       /* 特高区域 */

/* 除法选择定义 */
#define DIV_ROUND       0       /* 带四舍五入的除法 */
#define DIV_NORMAL      1       /* 不带四舍五入的除法 */


typedef struct tagRTC
{
    const I32 *v;       /* 表数据指针 */
    const U16  num;     /* 元素个数 */
    const I16  t0;      /* y起点值  */
    const I16  tn;      /* y终点值  */
    const I16  td;      /* y步进    */
} TAB_DATA;


#define DIFF_TYPE_NEG   0   //向下
#define DIFF_TYPE_POS   1   //向上




/* 数值格式属性 */
#define FNS_NULL       0x0000   /* 正常,默认占3位 */
#define FNS_DIGIT1     0x0001   /* 最少占1位(包括小数点和负号,下同) */
#define FNS_DIGIT2     0x0002   /* 最少占2位 */
#define FNS_DIGIT3     0x0003   /* 最少占3位 */
#define FNS_DIGIT4     0x0004   /* 最少占4位 */
#define FNS_DIGIT5     0x0005   /* 最少占5位 */
#define FNS_DIGIT6     0x0006   /* 最少占6位 */
#define FNS_DIGIT7     0x0007   /* 最少占7位 */
#define FNS_FLOAT1     0x0010   /* 1位小数 */
#define FNS_FLOAT2     0x0020   /* 2位小数 */
#define FNS_FLOAT3     0x0030   /* 3位小数 */
#define FNS_UNSIGN     0x0100   /* 按无符号显示 */
#define FNS_RIGHT      0x0200   /* 数值靠右 */
#define FNS_HEX        0x0400   /* 按16进制显示,占6位 */
#define FNS_ZERO       0x0800   /* 显示前导0 */
#define FNS_DIGIT_MASK 0x0007
#define FNS_FLOAT_MASK 0x0030
#define FNS_FLOAT_SET(f)  (((f)<<4)&FNS_FLOAT_MASK)

#define FNS_FLOAT_1     FNS_FLOAT1
#define FNS_UNSIGNED    FNS_UNSIGN
#define FNS_DIGIT(n)   (((n)<<0)&FNS_DIGIT_MASK)
#define FNS_FLOAT(n)   (((n)<<4)&FNS_FLOAT_MASK)



#define FDNS_NULL       0x0000
#define FDNS_DIGIT_1    0x0001
#define FDNS_DIGIT_2    0x0002
#define FDNS_DIGIT_3    0x0003
#define FDNS_DIGIT_4    0x0004
#define FDNS_DIGIT_5    0x0005
#define FDNS_DIGIT_6    0x0006
#define FDNS_DIGIT_MASK 0x0007
#define FDNS_FLOAT_MASK 0x0030
#define FDNS_SIGNED     0x0100
#define FDNS_SNAP_LEFT  0x0200
#define FDNS_HEX        0x0400
#define FDNS_ZERO       0x0800

/* fun_area */
//#define AREA_PT(...)    __VA_ARGS__, SENSOR_NOEXIST		/* 英飞凌平台暂不支持 */
#define MAX_AREA_POINT  20				/* 最多支持的分区点数 */
// 分区数 = 分区点数+1
enum {
    AREA_00 = 0,
    AREA_01,
    AREA_02,
    AREA_03,
    AREA_04,
    AREA_05,
    AREA_06,
    AREA_07,
    AREA_08,
    AREA_09,
    AREA_10,
    AREA_11,
    AREA_12,
    AREA_13,
    AREA_14,
    AREA_15,
    AREA_16,
    AREA_17,
    AREA_18,
    AREA_19,
    AREA_20,
    AREA_MAX
};

typedef struct tagXY
{
    I16 x;
    I16 y;
}XY;

// 趋势线
#define LINE_TREND_KEEP  0   // 持平
#define LINE_TREND_RISE  1   // 上升趋势
#define LINE_TREND_FALL  2   // 下降趋势
#define TREND_IS_KEEP(val) ((val) == LINE_TREND_KEEP)
#define TREND_IS_RISE(val) ((val) == LINE_TREND_RISE)
#define TREND_IS_FALL(val) ((val) == LINE_TREND_FALL)


FUN_EXT U08 Fun_LookupArea(I16 val, U08 area, I16 high, I16 mid, I16 norm, I16 low,I16 diff);
FUN_EXT U08 fun_area(I16 val, U08 area, I16 diff, ...);
FUN_EXT U08 fun_erea_devide(I16 p_value, U08 s_area, I16 Tpoint[], I16 Diff[], U08 PointMax);
FUN_EXT I16 fun_line_diff(I16 value, I16 x_bgn, I16 y_bgn, I16 x_end, I16 y_end, I16 diff);
FUN_EXT I16 fun_tab_lookup_equal(I32 value, TAB_DATA *tab, U08 obj);
FUN_EXT I16 fun_tab_lookup_unequal(I32 value, I32 *tab_src, I16 *tab_dest, U16 num, U08 obj);
FUN_EXT I16 fun_div_round(I32 dividend, I32 divisor, U08 obj);
FUN_EXT I16 fun_div_remainder(I16 *remainder, I32 dividend, I32 divisor);
FUN_EXT I16 fun_line_value(I16 src_value, I16 src_max, I16 src_min, I16 dest_max, I16 dest_min);
FUN_EXT U16 fun_line_value_u16(U16 x, U16 x1, U16 x2, U16 y1, U16 y2);
FUN_EXT I16 tab_lookup(I16 src_value, TAB_DATA const *tab_temp);
FUN_EXT I16 _Math_divide(I16 *a1, I32 a2, I16 b);
FUN_EXT I16 fun_absolute_value(I16 v);
FUN_EXT BOOL fun_time_compare(TIME front, TIME after);
FUN_EXT BOOL fun_time_check(TIME ktime);
FUN_EXT U16 fun_twotime_sub(TIME front, TIME after);
FUN_EXT U32 fun_power(U16 num, U08 cnt);
FUN_EXT I16 fun_pv_mul_10(I16 pv);
FUN_EXT I16 fun_pv_div_10(I16 pv);
FUN_EXT I16 fun_pv_min(I16 a, I16 b);
FUN_EXT I16 fun_pv_max(I16 a, I16 b);
FUN_EXT I16 fun_pv_cmpst(I16 pv, I16 cmps);
FUN_EXT I16 fun_pv_sub(I16 a, I16 b);
FUN_EXT I16 fun_pv_add(I16 a, I16 b);
FUN_EXT I16 fun_pv_avg(I16 a, I16 b);
FUN_EXT BOL fun_pv_in(I16 v, I16 low, I16 high);
FUN_EXT BOL fun_pv_filt(I16 *p_result, I16 pv, I16 filt);
FUN_EXT BOL fun_pv_filt_t(I16 *p_result, I16 pv, I16 filt, U16* time, U16 dly);
FUN_EXT BOL fun_pv_in_open(I16 v, I16 low, I16 high);
FUN_EXT I16 fun_pv_limit(I16 pv, I16 min, I16 max);
FUN_EXT I16 fun_pv_get(I16 a, I16 dft);
FUN_EXT I16 fun_pv_abs(I16 pv);
FUN_EXT I32 Fun_SimFloatDiv(I32 a, I32 b, U08 dot);
FUN_EXT U08 Fun_Range(I16 a, I16 o, U08 r);
FUN_EXT I32 Fun_AbsI32(I32 a);
FUN_EXT void Fun_LineFitting(U16 x[], U16 y[], U16 num, U08 dot, I32* K, I32* A, I32* B) ;
FUN_EXT U08 Fun_NumFormat(U16 num, U08* buf, U16 attrib) ;
FUN_EXT U16 fun_crc16(const U08 *pData, U16 len);
FUN_EXT BOOL fun_crc16_ok(const U08 *pData, U16 lenIncCrc);
FUN_EXT I16 fun_filt_avg(I16 src[], U08 n);
FUN_EXT I16 fun_all_avg(I16 src[], U08 n);
FUN_EXT I16 fun_rec_avg(I16 val, I16* data, U08 num);
FUN_EXT BOL fun_stab_avg(I16* data, U08 num, I16 d_lower, I16 d_upper);
/* 时间单位 */
#define TIME_DIFF_SECS          1
#define TIME_DIFF_MINS          2
#define TIME_DIFF_HOURS         3
#define TIME_DIFF_DAYS          4
FUN_EXT BOOL fun_rtc_check(LOG_RTC ktime);
FUN_EXT I32 fun_time_diff(LOG_RTC* front, LOG_RTC* after, U08 diff_type);
FUN_EXT I32 fun_time_diff_day(LOG_RTC* front, LOG_RTC* after);
FUN_EXT U32 fun_time_mktime(LOG_RTC* now);
FUN_EXT LOG_RTC fun_time_localtime(U32 secs);
U16 Fun_NumDisp(U16 num, STR* buf, U16 attrib);
FUN_EXT U16 fun_bit_first_1(U16 word[], U16 words);
U08 fun_find_node_from_arr(I16 pv, I16 arr[], U08 arr_size, U08 dir);
FUN_EXT XY fun_get_relative_pos(U16 blocks_size[], U16 num, U16 idx);

FUN_EXT U08 fun_value_trend(I16 val, I16 avg, I16 diff);
FUN_EXT void fun_sort_bubble(I16 arr[], U08 n, U08 dir);

FUN_EXT BOOL fun_check_fg_arr(U16 *fg_arr, U16 bit_num, BOOL is_once);
FUN_EXT void fun_copy_str_to_U16(char* str, U16 arr[]);
FUN_EXT void fun_U16_to_str(U16 word, U08 str[]);
FUN_EXT U16 Fun_StrCat(U08* str1, const U08* str2);
#endif


