/******************************************************************************

          ��Ȩ���� (C), 2001-2022, �����а��յ��Լ����������޹�˾

 ******************************************************************************
  �� �� ��   : Weather_Curve.h
  �� �� ��   : ����
  ��    ��   : J.H.
  ��������   : 2022��5��27��������
  ����޸�   :
  ��������   : Weather_Curve.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2022��5��27��������
    ��    ��   : J.H.
    �޸�����   : �����ļ�

******************************************************************************/

/*-----------------����ͷ�ļ� -----------------------------*/
#include "Includes.h"


/*-----------------�ⲿ����˵��----------------------------*/



/*-----------------�ⲿ����ԭ��˵��------------------------*/



/*-----------------�ڲ�����ԭ��˵��------------------------*/



/*-----------------ȫ�ֱ���--------------------------------*/



/*-----------------ģ�鼶����------------------------------*/


/* ���ȵ������� */
#define HEAT_SV_TEMP_NUM            (41)   /* ����Ŀ���¶ȵ��� */
#define HEAT_LOW_SV_TEMP_CURVE_NUM  (8)    /* ���ȵ��������� */
#define HEAT_HIGH_SV_TEMP_CURVE_NUM (8)		/* ���ȸ��������� */
#define HEAT_SV_TEMP_CURVE_NUM      (HEAT_LOW_SV_TEMP_CURVE_NUM+HEAT_HIGH_SV_TEMP_CURVE_NUM)

/* ���Ȼ��·ֶε� */
const I16 HEAT_ENV_TEMP_HEADER[HEAT_SV_TEMP_NUM] = 
{
    -20, -19, -18, -17, -16, -15, -14, -13, -12, -11,
    -10,  -9,  -8,  -7,  -6,  -5,  -4,  -3,  -2,  -1,
      0,   1,   2,   3,   4,   5,   6,   7,   8,   9,
     10,  11,  12,  13,  14,  15,  16,  17,  18,  19,
     20,
};

const I16 HEAT_LOW_SV_TEMP_CURVE_1[HEAT_SV_TEMP_NUM] = {38, 38, 38, 38, 38, 37, 37, 37, 37, 37, 37, 36, 36, 36, 36, 36, 36, 35, 35, 35, 35, 35, 35, 34, 34, 34, 34, 34, 34, 33, 33, 33, 33, 33, 33, 32, 32, 32, 32, 32, 32,};/* ���ȵ�������1 */
const I16 HEAT_LOW_SV_TEMP_CURVE_2[HEAT_SV_TEMP_NUM] = {37, 37, 37, 37, 37, 36, 36, 36, 36, 36, 36, 35, 35, 35, 35, 35, 35, 34, 34, 34, 34, 34, 34, 33, 33, 33, 33, 33, 33, 32, 32, 32, 32, 32, 32, 31, 31, 31, 31, 31, 31,};/* ���ȵ�������2 */
const I16 HEAT_LOW_SV_TEMP_CURVE_3[HEAT_SV_TEMP_NUM] = {36, 36, 36, 35, 35, 35, 35, 35, 35, 34, 34, 34, 34, 34, 34, 33, 33, 33, 33, 33, 33, 32, 32, 32, 32, 32, 32, 31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 30, 29, 29,};/* ���ȵ�������3 */
const I16 HEAT_LOW_SV_TEMP_CURVE_4[HEAT_SV_TEMP_NUM] = {35, 35, 35, 34, 34, 34, 34, 34, 34, 33, 33, 33, 33, 33, 33, 32, 32, 32, 32, 32, 32, 31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 30, 29, 29, 29, 29, 29, 29, 28, 28,};/* ���ȵ�������4 */
const I16 HEAT_LOW_SV_TEMP_CURVE_5[HEAT_SV_TEMP_NUM] = {34, 34, 34, 33, 33, 33, 33, 33, 33, 32, 32, 32, 32, 32, 32, 31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 30, 29, 29, 29, 29, 29, 29, 28, 28, 28, 28, 28, 28, 27, 27,};/* ���ȵ�������5 */
const I16 HEAT_LOW_SV_TEMP_CURVE_6[HEAT_SV_TEMP_NUM] = {32, 32, 32, 32, 31, 31, 31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 30, 30, 30, 29, 29, 29, 29, 29, 29, 29, 28, 28, 28, 28, 28, 28, 27, 27, 27, 27, 27, 27, 26, 26,};/* ���ȵ�������6 */
const I16 HEAT_LOW_SV_TEMP_CURVE_7[HEAT_SV_TEMP_NUM] = {31, 31, 31, 31, 30, 30, 30, 30, 30, 30, 30, 30, 29, 29, 29, 29, 29, 29, 29, 29, 28, 28, 28, 28, 28, 28, 28, 27, 27, 27, 27, 27, 27, 26, 26, 26, 26, 26, 26, 25, 25,};/* ���ȵ�������7 */
const I16 HEAT_LOW_SV_TEMP_CURVE_8[HEAT_SV_TEMP_NUM] = {29, 29, 29, 29, 28, 28, 28, 28, 28, 28, 28, 28, 27, 27, 27, 27, 27, 27, 27, 27, 26, 26, 26, 26, 26, 26, 26, 26, 25, 25, 25, 25, 25, 25, 25, 25, 24, 24, 24, 24, 24,};/* ���ȵ�������8 */

const I16 HEAT_HIGH_SV_TEMP_CURVE_1[HEAT_SV_TEMP_NUM] = {55, 55, 55, 55, 54, 54, 54, 54, 54, 54, 54, 54, 53, 53, 53, 53, 53, 53, 53, 53, 52, 52, 52, 52, 52, 52, 52, 52, 51, 51, 51, 51, 51, 51, 51, 51, 50, 50, 50, 50, 50,};/* ���ȸ�������1 */
const I16 HEAT_HIGH_SV_TEMP_CURVE_2[HEAT_SV_TEMP_NUM] = {53, 53, 53, 53, 52, 52, 52, 52, 52, 52, 52, 52, 51, 51, 51, 51, 51, 51, 51, 51, 50, 50, 50, 50, 50, 50, 50, 50, 49, 49, 49, 49, 49, 49, 49, 49, 48, 48, 48, 48, 48,};/* ���ȸ�������2 */
const I16 HEAT_HIGH_SV_TEMP_CURVE_3[HEAT_SV_TEMP_NUM] = {52, 52, 52, 52, 51, 51, 51, 51, 51, 51, 51, 51, 50, 50, 50, 50, 50, 50, 50, 50, 49, 49, 49, 49, 49, 49, 49, 49, 48, 48, 48, 48, 48, 48, 48, 48, 47, 47, 47, 47, 47,};/* ���ȸ�������3 */
const I16 HEAT_HIGH_SV_TEMP_CURVE_4[HEAT_SV_TEMP_NUM] = {50, 50, 50, 50, 49, 49, 49, 49, 49, 49, 49, 49, 48, 48, 48, 48, 48, 48, 48, 48, 47, 47, 47, 47, 47, 47, 47, 47, 46, 46, 46, 46, 46, 46, 46, 46, 45, 45, 45, 45, 45,};/* ���ȸ�������4 */
const I16 HEAT_HIGH_SV_TEMP_CURVE_5[HEAT_SV_TEMP_NUM] = {48, 48, 48, 48, 47, 47, 47, 47, 47, 47, 47, 47, 46, 46, 46, 46, 46, 46, 46, 46, 45, 45, 45, 45, 45, 45, 45, 45, 44, 44, 44, 44, 44, 44, 44, 44, 43, 43, 43, 43, 43,};/* ���ȸ�������5 */
const I16 HEAT_HIGH_SV_TEMP_CURVE_6[HEAT_SV_TEMP_NUM] = {45, 45, 45, 45, 44, 44, 44, 44, 44, 44, 44, 44, 43, 43, 43, 43, 43, 43, 43, 43, 42, 42, 42, 42, 42, 42, 42, 42, 41, 41, 41, 41, 41, 41, 41, 41, 40, 40, 40, 40, 40,};/* ���ȸ�������6 */
const I16 HEAT_HIGH_SV_TEMP_CURVE_7[HEAT_SV_TEMP_NUM] = {43, 43, 43, 43, 42, 42, 42, 42, 42, 42, 42, 42, 41, 41, 41, 41, 41, 41, 41, 41, 40, 40, 40, 40, 40, 40, 40, 40, 39, 39, 39, 39, 39, 39, 39, 39, 38, 38, 38, 38, 38,};/* ���ȸ�������7 */
const I16 HEAT_HIGH_SV_TEMP_CURVE_8[HEAT_SV_TEMP_NUM] = {40, 40, 40, 40, 39, 39, 39, 39, 39, 39, 39, 39, 38, 38, 38, 38, 38, 38, 38, 38, 37, 37, 37, 37, 37, 37, 37, 37, 36, 36, 36, 36, 36, 36, 36, 36, 35, 35, 35, 35, 35,};/* ���ȸ�������8 */

/* ���ȱ� */
const I16 *HEAT_SV_TEMP_TBL[HEAT_SV_TEMP_CURVE_NUM] = 
{
    HEAT_LOW_SV_TEMP_CURVE_1,
    HEAT_LOW_SV_TEMP_CURVE_2,
    HEAT_LOW_SV_TEMP_CURVE_3,
    HEAT_LOW_SV_TEMP_CURVE_4,
    HEAT_LOW_SV_TEMP_CURVE_5,
    HEAT_LOW_SV_TEMP_CURVE_6,
    HEAT_LOW_SV_TEMP_CURVE_7,
    HEAT_LOW_SV_TEMP_CURVE_8,

    HEAT_HIGH_SV_TEMP_CURVE_1,
    HEAT_HIGH_SV_TEMP_CURVE_2,
    HEAT_HIGH_SV_TEMP_CURVE_3,
    HEAT_HIGH_SV_TEMP_CURVE_4,
    HEAT_HIGH_SV_TEMP_CURVE_5,
    HEAT_HIGH_SV_TEMP_CURVE_6,
    HEAT_HIGH_SV_TEMP_CURVE_7,
    HEAT_HIGH_SV_TEMP_CURVE_8,
};

/* �������� */
#define COOL_SV_TEMP_NUM            (4)
#define COOL_LOW_SV_TEMP_CURVE_NUM  (8)
#define COOL_HIGH_SV_TEMP_CURVE_NUM (8)
#define COOL_SV_TEMP_CURVE_NUM      (COOL_LOW_SV_TEMP_CURVE_NUM+COOL_HIGH_SV_TEMP_CURVE_NUM)

/* ���价�·ֶε� */
const I16 COOL_ENV_TEMP_HEADER[COOL_SV_TEMP_NUM] = { 15, 22, 30, 0x7fff, };

const I16 COOL_LOW_SV_TEMP_CURVE_1[COOL_SV_TEMP_NUM] = { 16, 11,  8,  5, };/* �����������1 */
const I16 COOL_LOW_SV_TEMP_CURVE_2[COOL_SV_TEMP_NUM] = { 17, 12,  9,  6, };/* �����������2 */
const I16 COOL_LOW_SV_TEMP_CURVE_3[COOL_SV_TEMP_NUM] = { 18, 13, 10,  7, };/* �����������3 */
const I16 COOL_LOW_SV_TEMP_CURVE_4[COOL_SV_TEMP_NUM] = { 19, 14, 11,  8, };/* �����������4 */
const I16 COOL_LOW_SV_TEMP_CURVE_5[COOL_SV_TEMP_NUM] = { 20, 15, 12,  9, };/* �����������5 */
const I16 COOL_LOW_SV_TEMP_CURVE_6[COOL_SV_TEMP_NUM] = { 21, 16, 13, 10, };/* �����������6 */
const I16 COOL_LOW_SV_TEMP_CURVE_7[COOL_SV_TEMP_NUM] = { 22, 17, 14, 11, };/* �����������7 */
const I16 COOL_LOW_SV_TEMP_CURVE_8[COOL_SV_TEMP_NUM] = { 23, 18, 15, 12, };/* �����������8 */

const I16 COOL_HIGH_SV_TEMP_CURVE_1[COOL_SV_TEMP_NUM] = { 20, 18, 17, 16, };/* �����������1 */
const I16 COOL_HIGH_SV_TEMP_CURVE_2[COOL_SV_TEMP_NUM] = { 21, 19, 18, 17, };/* �����������2 */
const I16 COOL_HIGH_SV_TEMP_CURVE_3[COOL_SV_TEMP_NUM] = { 22, 20, 19, 17, };/* �����������3 */
const I16 COOL_HIGH_SV_TEMP_CURVE_4[COOL_SV_TEMP_NUM] = { 23, 21, 19, 18, };/* �����������4 */
const I16 COOL_HIGH_SV_TEMP_CURVE_5[COOL_SV_TEMP_NUM] = { 24, 21, 20, 18, };/* �����������5 */
const I16 COOL_HIGH_SV_TEMP_CURVE_6[COOL_SV_TEMP_NUM] = { 24, 22, 20, 19, };/* �����������6 */
const I16 COOL_HIGH_SV_TEMP_CURVE_7[COOL_SV_TEMP_NUM] = { 25, 22, 21, 19, };/* �����������7 */
const I16 COOL_HIGH_SV_TEMP_CURVE_8[COOL_SV_TEMP_NUM] = { 25, 23, 21, 20, };/* �����������8 */

/* ����� */
const I16 *COOL_SV_TEMP_TBL[COOL_SV_TEMP_CURVE_NUM] = 
{
    COOL_LOW_SV_TEMP_CURVE_1,
    COOL_LOW_SV_TEMP_CURVE_2,
    COOL_LOW_SV_TEMP_CURVE_3,
    COOL_LOW_SV_TEMP_CURVE_4,
    COOL_LOW_SV_TEMP_CURVE_5,
    COOL_LOW_SV_TEMP_CURVE_6,
    COOL_LOW_SV_TEMP_CURVE_7,
    COOL_LOW_SV_TEMP_CURVE_8,

    COOL_HIGH_SV_TEMP_CURVE_1,
    COOL_HIGH_SV_TEMP_CURVE_2,
    COOL_HIGH_SV_TEMP_CURVE_3,
    COOL_HIGH_SV_TEMP_CURVE_4,
    COOL_HIGH_SV_TEMP_CURVE_5,
    COOL_HIGH_SV_TEMP_CURVE_6,
    COOL_HIGH_SV_TEMP_CURVE_7,
    COOL_HIGH_SV_TEMP_CURVE_8,
};

I16 WC_get_weather_curve_sv_temp(I16 temp_env, U08 curve_id, U08 mode)
{
    U08 i = 0;
    U08 j = 0;
    U08 curve_idx = 0;
    I16 wc_sv_temp = SENSOR_NOEXIST;
    const I16 *p_header = PNULL;
    const I16 *p_curve = PNULL;

    if (curve_id == TEMP_CURVE_NULL)               return SENSOR_NOEXIST;
    if (curve_id > TEMP_CURVE_MAX)                 return SENSOR_NOEXIST;
    if (mode != MODE_COOL && mode != MODE_HEAT)    return SENSOR_NOEXIST;

    /* ����1~8 */
    if (curve_id < TEMP_CURVE_9)
    {
        curve_idx = curve_id - 1;
        if (SensorCanUse(temp_env))
        {
            p_header = (mode == MODE_COOL) ? COOL_ENV_TEMP_HEADER : HEAT_ENV_TEMP_HEADER;
            p_curve = (mode == MODE_COOL) ? COOL_SV_TEMP_TBL[curve_idx] : HEAT_SV_TEMP_TBL[curve_idx];
            j = (mode == MODE_COOL) ? COOL_SV_TEMP_NUM : HEAT_SV_TEMP_NUM;
            
            /* �ҵ���ǰ�����±� */
            for (i = 0; i < j; i++)
            {
                if (temp_env <= 10*(*p_header++)) break;
            }
        
            /* ���� */
            if (i >= j)
            {
                i = j-1;
            }
            
            wc_sv_temp = 10*p_curve[i];
        }
    }

    /* ����9 */
    if (curve_id == TEMP_CURVE_9)
    {
        if (SensorCanUse(temp_env))
        {
            if (mode == MODE_COOL)
            {
                wc_sv_temp = fun_line_value(temp_env, \
                                            MAX(val_cool_curve_9_TA_1,val_cool_curve_9_TA_2), \
                                            MIN(val_cool_curve_9_TA_1,val_cool_curve_9_TA_2), \
                                            MIN(val_cool_curve_9_TWout2_1,val_cool_curve_9_TWout2_2), \
                                            MAX(val_cool_curve_9_TWout2_1,val_cool_curve_9_TWout2_2));

            }
            else
            {
                wc_sv_temp = fun_line_value(temp_env, \
                                            MAX(val_heat_curve_9_TA_1,val_heat_curve_9_TA_2), \
                                            MIN(val_heat_curve_9_TA_1,val_heat_curve_9_TA_2), \
                                            MIN(val_heat_curve_9_TWout2_1,val_heat_curve_9_TWout2_2),\
                                            MAX(val_heat_curve_9_TWout2_1,val_heat_curve_9_TWout2_2));
            }
//            zzpdebug10 = wc_sv_temp;
        }
    }

    return wc_sv_temp;
}


