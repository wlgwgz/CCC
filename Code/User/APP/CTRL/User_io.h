#ifndef IO_H
#define IO_H   1
#ifdef IO_GLOBAL
#define IO_EXT
#else 
#define IO_EXT extern
#endif

//#define MAX_AD 				10	//���AD��
//#define MAX_CUR 			MAX_CUR_NUM   //������ͨ��
#define MAX_TEMP 			MAX_TEMP_NUM  //����¶�ͨ��           
//#define MAX_AD_ARRAY  		6	//�˲�ʹ������	
#define MAX_INPUT1 			MAX_DI_NUM	//����������
//#define MAX_INPUT2			4	//����Ͳ��̿���
//#define MAX_SW1				4	//SW1�������λ��
//#define MAX_SW1_ADDRESS		3 	//SW1�е�ַλ��
//#define AD_TYPE				4	//AD����	0-���� 1-���� 2-���� 3-����

#define MAX_CURR			MAX_CURR_50A			/* �����������ֵ(��λ��0.1A) */

//ģ��������λ��
//#define AI_J5               0
//#define AI_J6               1
//#define AI_J7               2
//#define AI_J8               3

//����������
#define AI_PRS_HIGH 0       // ��ѹ������
#define AI_PRS_LOW  1       // ��ѹ������

#define PRESS_TEMP_DEW         0   /* ¶���¶� ���� */
#define PRESS_TEMP_BUBBLE      1   /* �ݵ��¶� ���� */


IO_EXT void io_temp_read(void);
IO_EXT void io_speed_deal(void);/* N0004 */

//IO_EXT void ad_init(void);
//IO_EXT void phase(void);
//IO_EXT void input(void);
//IO_EXT void jmp(void);
//IO_EXT void output(void);
//IO_EXT void io_ad(void);
IO_EXT void IO_Step(void);
IO_EXT I16 AiSignalConvert(I16 signal, I16 pv_type, I16 CorrectValue) ;
IO_EXT I16 Press_to_Temp(I16 src_value, U08 tab_idx , U08 type);
IO_EXT U08 pwminput_fg;


#endif
