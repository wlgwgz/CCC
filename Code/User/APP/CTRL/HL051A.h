/******************************************************************************

          ��Ȩ���� (C), 2001-2016, �����а��յ��Լ����������޹�˾

 ******************************************************************************
  �� �� ��   : HL051A.h
  �� �� ��   : ����
  ��    ��   : ��־��
  ��������   : 2018��3��29��������
  ����޸�   :
  ��������   : HL051A.h ��ͷ�ļ�,����Ӳ������Դ����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��3��29��������
    ��    ��   : ��־��
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __HL051A_H__
#define __HL051A_H__


/* ������������Ŷ��� */
#define DI_01	         0	
#define DI_02	         1	
#define DI_03	         2	
#define DI_04	         3	
#define DI_05	         4	
#define DI_06	         5	
#define DI_07	         6	
//������ǿ�翪����1-5
#define DI_08	         7	
#define DI_09	         8	
#define DI_10	         9	
#define DI_11	         10	
#define DI_12	         11
#define GET_DI(n)       (gIO.input[n>>4] & (1<<(n)))


/* �����������Ŷ��� */
#define DO_01	0	
#define DO_02	1	
#define DO_03	2	
#define DO_04	3	
#define DO_05	4	
#define DO_06	5	
#define DO_07	6	
#define DO_08	7	
#define DO_09	8	
#define DO_10	9	
#define DO_11	10	
#define DO_12	11	
#define DO_13	12	
#define DO_14	13	
#define DO_15	14	
#define DO_16	15	
#define DO_17	16	
#define DO_18	17
#define DO_MAX  18
#define SET_DO(n)       SETBIT(gIO.output[n>>4], (n&0x0f))
#define DO_BIT(n)       (1<<(n))

/* �¶������Ŷ��� */
#define TEMP_01	         0
#define TEMP_02	         1
#define TEMP_03          2
#define TEMP_04          3
#define TEMP_05          4
#define TEMP_06          5
#define TEMP_07          6
#define TEMP_08          7
#define TEMP_09          8
#define TEMP_10          9
#define TEMP_11          10
#define TEMP_12          11
/* ����̼�ʱ������ڻ�ģ���� */
#define GET_TEMP(n)     (TF_IS_OUT ? SENSOR_NOEXIST : fun_pv_div_10(gIO.temp[n])) //�¶�0.01תΪ0.1

/* ģ���������Ŷ��� */
#define AI_01            0
#define AI_02            1
#define AI_03            2
#define GET_AI(n)       0//(gIO.ai[n])


/* ���������Ŷ��� */
#define CUR_01           0
#define CUR_02           1
#define GET_CUR(n)      SENSOR_NOEXIST//(gIO.cur[n])

/* �����Ŷ��� */
#define JMP_01	         0	
#define JMP_02	         1	
#define JMP_03	         2	
#define JMP_04	         3	
#define JMP_05	         4	
#define JMP_06	         5	
#define JMP_07	         6	
#define JMP_08	         7	
#define JMP_09	         8	
#define JMP_10	         9	
#define JMP_11	         10	
#define JMP_12	         11
#define JMP_13	         12	
#define JMP_14	         13
#define GET_JMP(n)       (gIO.jmp[n>>4] & (1<<(n)))

/* ʹ��BMS����Ӳ����Դʱ���� */
#ifdef USE_BMS_SOURCE
//#undef  GET_DI
//#define GET_DI(n)       GETBIT(bms_source.src[0].DI[(n)>>4], (n)&0x0f)
//#undef  GET_DI_W
//#define GET_DI_W(w)     (bms_source.src[0].DI[w])
#undef  GET_TEMP
#define GET_TEMP(n)      (bms_source.src[0].temp[n])
#undef  GET_AI
#define GET_AI(n)       (bms_source.src[0].ai[n])
#undef  GET_CUR
#define GET_CUR(n)      (bms_source.src[0].cur[n])
#undef  GET_HUMI
#define GET_HUMI(n)     (bms_source.src[0].humi[n])
#endif

#endif /* __HL051A_H__ */

