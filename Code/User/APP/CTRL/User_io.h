#ifndef IO_H
#define IO_H   1
#ifdef IO_GLOBAL
#define IO_EXT
#else 
#define IO_EXT extern
#endif

//#define MAX_AD 				10	//最大AD数
//#define MAX_CUR 			MAX_CUR_NUM   //最大电流通道
#define MAX_TEMP 			MAX_TEMP_NUM  //最大温度通道           
//#define MAX_AD_ARRAY  		6	//滤波使用组数	
#define MAX_INPUT1 			MAX_DI_NUM	//开关量输入
//#define MAX_INPUT2			4	//拨码和拨盘开关
//#define MAX_SW1				4	//SW1拨码最多位数
//#define MAX_SW1_ADDRESS		3 	//SW1中地址位数
//#define AD_TYPE				4	//AD类型	0-低温 1-中温 2-高温 3-正常

#define MAX_CURR			MAX_CURR_50A			/* 电流测量最大值(单位：0.1A) */

//模拟量输入位置
//#define AI_J5               0
//#define AI_J6               1
//#define AI_J7               2
//#define AI_J8               3

//传感器类型
#define AI_PRS_HIGH 0       // 高压传感器
#define AI_PRS_LOW  1       // 低压传感器

#define PRESS_TEMP_DEW         0   /* 露点温度 吸气 */
#define PRESS_TEMP_BUBBLE      1   /* 泡点温度 排气 */


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
