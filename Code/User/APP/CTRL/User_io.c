
#define IO_GLOBAL
#include "includes.h"
//#include "system.h"
//#include <system.h>
//#include <ws_lcd_st7920.h>

//static U08 input1_time[MAX_INPUT1];  		//输入滤波时间
//static U08 input2_time[MAX_INPUT2];		//拨码开关滤波时间

//#define SENSOR_TPYE_NTC1		0
//#define SENSOR_TPYE_NTC2		1
//#define SENSOR_TPYE_CUR			2
//#define TPYE_PRESS_R410A		3
//#define SENSOR_TPYE_PRESS		4


//static U16  ad_cur[MAX_CUR+2][MAX_AD_ARRAY];				/* 电流的AD队列 */
//static U16  ad_temp_nor[MAX_AD][MAX_AD_ARRAY];			/* 正常的AD队列 */
//static U16  ad_temp_low[MAX_AD][MAX_AD_ARRAY];			/* 低温的AD队列 */
//static U16  ad_temp_mid[MAX_AD][MAX_AD_ARRAY];			/* 精密的AD队列 */
//static U16  ad_temp_high[MAX_AD][MAX_AD_ARRAY];			/* 高温的AD队列 */
//static I16  mpv[MAX_TEMP];					/* pv的余数 */
//static U08  sensor_dly[MAX_TEMP]; 			/* 探头故障延时 */
//static U16  ad_end = 0;			/* AD采样结束标识 */


//三相电中间变量
//static U08 	phase_en = FALSE;				/* 第一次采样到有效数据 */
//static U08 	phase_last = PHASE_INVALID; 	/* 上一次数据 */
//static U08	phase_old = 0;					/* 上一次有效数据 */
//static U08	phase_now = 0;					/* 当前数据 */
//static I08	phase_count = 0;				/* 状态迁移量 */
//static U08	phase_time = 0; 				/* 采样周期计时 */ 
//static U08	phase_dly = 0;					/* 持续时间 */

//static U16  ad_cur[MAX_CUR][MAX_AD_ARRAY];				/* 电流的AD队列 */
//static U16  ad_temp_nor[MAX_AD][MAX_AD_ARRAY];			/* 正常的AD队列 */
//static U16  ad_temp_low[MAX_AD][MAX_AD_ARRAY];			/* 低温的AD队列 */
//static U16  ad_temp_mid[MAX_AD][MAX_AD_ARRAY];			/* 精密的AD队列 */
//static U16  ad_temp_high[MAX_AD][MAX_AD_ARRAY];			/* 高温的AD队列 */

//static U08  adcur_cnt;		/* 电流的AD队列 */
//static U08  adnor_cnt;		/* 正常的AD队列 */
//static U08  adlow_cnt;		/* 低温的AD队列 */
//static U08  admid_cnt;		/* 精密的AD队列 */
//static U08  adhig_cnt;		/* 高温的AD队列 */

//static I16  NOR_0 = 0;			/* 低温运放的零点值 */	
//static I16  NOR_A = 1023;		/* 低温运放的放大倍数，乘以了1023 */
//static I16  LOW_0 = 0;			/* 低温运放的零点值 */	
//static I16  LOW_A = 4096;		/* 低温运放的放大倍数，乘以了1023 */
//static I16  MID_0 = 0;			/* 中温运放的零点值 */	
//static I16  MID_A = 16368;		/* 中温运放的放大倍数，乘以了1023 */
//static I16  HIG_0 = 0;			/* 高温运放的零点值 */	
//static I32  HIG_A = 65944;		/* 高温运放的放大倍数，乘以了1023 */

/**********************************************************************************/
/*====================================================
		常温探头温度表 (温度值 - 电阻值(欧))
/====================================================*/
#if 0
const I32  NTCTbl_1[] =	
{
	// -50     -49     -48     -47 	   -46     -45     -44     -43     -42     -41
	706192, 656553, 610747, 568453, 529380, 493260, 459851, 428933, 400304, 349715,
	// -40     -39     -38     -37 	   -36     -35     -34     -33     -32     -31
	349192, 326387, 305244, 285574, 267321, 250355, 234578, 219900, 206236, 193511,
	// -30     -29     -28     -27 	   -26     -25     -24     -23     -22     -21
	181700, 170600, 160300, 150700, 141700, 133300, 125500, 118100, 111300, 104900,
	// -20     -19     -18     -17     -16     -15     -14     -13     -12     -11
	 98880,  93260,  88000,  83060,  78430,  74090,  70020,  66200,  62600,  59230,
	// -10      -9      -8      -7      -6      -5      -4      -3      -2      -1
	 56060,  53070,  50270,  47630,  45140,  42800,  40600,  38520,  36560,  34710,
	//   0       1       2       3       4       5       6       7       8       9
	 32970,  31310,  29750,  28280,  26890,  25570,  24330,  23150,  22040,  20990,
	//  10      11      12      13      14      15      16      17      18      19
	 20000,  19050,	 18160,	 17320,	 16520,	 15760,	 15040,	 14360,	 13710,	 13090,
	//  20      21      22      23      24      25      26      27      28      29
	 12510,	 11950,	 11430,	 10930,	 10450,	 10000,	  9570,	  9161,	  8771,	  8401,
	//  30      31      32      33      34      35      36      37      38      39
	  8048,	  7712,	  7391,	  7086,	  6795,	  6518,	  6253,	  6001,	  5760,	  5531,
	//  40      41      42      43      44      45      46      47      48      49
	  5311,	  5102,	  4902,	  4711,	  4528,	  4353,	  4186,	  4027,	  3874,	  3728,
	//  50      51      52      53      54      55      56      57      58      59
	  3588,	  3454,	  3326,	  3204,	  3086,	  2973,	  2865,	  2762,	  2663,	  2568,
	//  60      61      62      63      64      65      66      67      68      69
	  2477,	  2389,	  2305,	  2225,	  2147,	  2073,	  2002,	  1933,	  1867,	  1804,
	//  70      71      72      73      74      75      76      77      78      79
	  1743,	  1685,	  1629,	  1575,	  1523,	  1473,	  1425,	  1378,	  1334,	  1291,
	//  80      81      82      83      84      85      86      87      88      89
	  1250,	  1210,	  1172,	  1135,	  1099,	  1065,	  1032,	   1000,   969,	   940,
	//  90      91      92      93      94      95      96      97      98      99
	   911,	   883,	   857,	   831,	   806,	   782,	   759,	   737,	   715,	   695,
	// 100     101     102     103     104     105     106     107     108     109
	   674,	   655,	   636,	   618,	   601,	   584,	   567,	   551,	   536,	   521,
	// 110     111     112     113     114     115     116     117     118     119
	   507,	   493,	   479,	   466,	   454,	   441,	   429,	   418,	   407,	   396,
	// 120     121     122     123     124     125     126     127     128     129
	   386,	   376,	   366,	   356,	   347,	   338,	   329,	   321,	   313,	   305,
	// 130     131     132     133     134     135     136     137     138     139
	   297,	   290,	   283,	   276,	   269,	   262,	   256,	   250,	   243,	   238,
	// 140     141     142     143     144     145     146     147     148     149
	   232,	   226,	   221,	   216,	   211,	   206,	   201,	   196,	   192,	   187,
	// 150
	   183,
};

const I32  NTCTbl_2[]=
{
	//-30	  -29	  -28	  -27	  -26	  -25	  -24	  -23	  -22	  -21
	63730,	60320,	57120,	54100,	51270,	48600,	46090,	43720,	41490,	39380,
	//-20	  -19	  -18	  -17	  -16	  -15	  -14	  -13	  -12	  -11
	37400,	35530,	33760,	32090,	30520,	29030,	27620,	26290,	25030,	23840,
	//-10	   -9	   -8	   -7	   -6	   -5	   -4	   -3	   -2	   -1
	22720,	21650,	20640,	19680,	18770,	17910,	17100,	16320,	15590,	14890,
	//  0	    1	    2	    3	    4	    5	    6	    7	    8	    9
	14230,	13600,	13010,	12440,	11900,	11390,	10900,	10440,	10000,	 9580,
	// 10	   11	   12	   13	   14	   15	   16	   17	   18	   19
	 9181,	 8801,	 8439,	 8093,	 7764,	 7451,	 7151,	 6866,	 6593,	 6333,
	// 20	   21	   22	   23	   24	   25	   26	   27	   28	   29
	 6085,	 5848,	 5621,	 5405,	 5198,	 5000,	 4811,	 4630,	 4457,	 4291,
	// 30	   31	   32	   33	   34	   35	   36	   37	   38	   39
	 4132,	 3980,	 3835,	 3696,	 3562,	 3434,	 3311,	 3104,	 3081,	 2973,
	// 40	   41	   42	   43	   44	   45	   46	   47	   48	   49
	 2869,	 2769,	 2674,	 2582,	 2493,	 2409,	 2327,	 2249,	 2174,	 2102,
	// 50	   51	   52	   53	   54	   55	   56	   57	   58	   59
	 2032,	 1966,	 1902,	 1840,	 1780,	 1723,	 1668,	 1615,	 1564,	 1514,
	// 60	   61	   62	   63	   64	   65	   66	   67	   68	   69
	 1467,	 1421,	 1376,	 1334,	 1292,	 1252,	 1214,	 1177,	 1141,	 1107,
	// 70	   71	   72	   73	   74	   75	   76	   77	   78	   79
	 1073,	 1041,	 1010,	  980,	  951,	  923,	  896,	  870,	  844,	  820,
	// 80	   81	   82	   83	   84	   85	   86	   87	   88	   89
	  796,	  773,	  751,	  729,	  709,	  689,	  670,	  650,	  632,	  614,
	// 90	   91	   92	   93	   94	   95	   96	   97	   98	   99
	  597,	  581,	  565,	  549,	  534,	  520,	  506,	  492,	  479,	  466,
	//100
	  453,
};

/*====================================================
		电流AD表 (电流（A）- AD值) 
/====================================================*/
//const I32  CURTbl[] =
//{
//  // 80    76    72   68   64    60	  56   52    48    44 
//	1012, 1003, 994, 981, 954,  926, 893,  856, 814,  767,  
//  // 40    36    32    28   24   20   16	12	  8	  4      
//	714,  656,  592,  524, 451, 374,  293, 211, 129,  50, 
//  // 
//  	33,   18,   6,    1,
//};
//
//const I16 cur_value[] = 					/* 电流值 */
//{
//	MAX_CURR,  	760,   720,   680,   640,  600, 560,  520,  480, 440,
//	400,  		360,   320,   280,   240,  200, 160,  120,  80,  40,   
//	30,   		20,    10,    0,
//};

const I32  CURTbl[] =
{
  //80,    76,   72,  68,  64,   60,  56,   52,  48,   44,
	964, 951, 937, 923, 906,  889, 869,  846, 821,  792,  
  //42,    40,   38,  36,  34,  32,   30,  28,   26,  24,     
	776,  758,  740,  719, 697, 672,  645, 615, 582,  547, 
  //22,	   20,   18,  16,  14,  12,   10,  9.5, 9.0,  8.5,
	512,  470,  425,  378, 329, 280, 229,  217, 204,  191,  
  //8.0,  7.5,  7.0,  6.5, 6.0, 5.5,  5.0, 4.5, 4.0, 3.5,     
	179,  166,  153,  142, 129, 116,  104,  91,  80,  68,
  //3.0,   2.9, 2.8, 2.7, 2.6,  2.5, 2.4,  2.3, 2.2,  2.1, 
	 56,    54,  51,  49,  47,   44,  42,   40,  38,   35,  
  //2.0,  1.9,  1.8,  1.7, 1.6, 1.5,  1.4, 1.3, 1.2,  1.1,      
	 33,   31,   29,   27,  25,  22,   20,  18,  16,   14, 
  //1.0,  0.9,  0.8,  0.7, 0.6, 0.5,  0.4, 0.3, 0.2,  0.1,      
	 12,   11,    9,    7,   5,   4,    3,   2,   1,    0,
};

const I16 cur_value[] = 					/* 电流值 */
{
	MAX_CURR,  	760,   720,   680,   640,  600, 560,  520,  480, 440,
	420,  		400,   380,   360,   340,  320, 300,  280,  260,  240,
	220,		200,   180,   160,   140,  120, 100,  95,    90,  85,
	80,  		75,     70,   65,     60,  55,   50,  45,    40,  35,
	30,			29,   	28,   27,     26,  25,   24,  23,    22,  21,
	20,			19,   	18,   17,     16,  15,   14,  13,    12,  11,
	10,			9,   	8,    7,       6,   5,    4,  3,      2,   1,
};
#endif    
#if 0
//====================================
typedef struct tagRTC
{   
	 const I32 *v;		/* 表数据指针 */
	 const U08  num;	/* 表长度 */
	 const I16  t0;		/* 表的起始温度	*/
	 const I16  tn;		/* 表的结束温度	*/
	 const I16  td;		/* 表区间温度差	*/
} RTC_DATA;
    
RTC_DATA TabRTC[] =
{
	{  NTCTbl_1,  		201,  -500,  	1500,  10  },	/* 放大10倍 */
	{  NTCTbl_2,  		131,  -300,  	1000,  10  },	/* 放大10倍 */
	{  CURTbl,     		70,   MAX_CURR,    0, -40  },	/* 放大10倍 */
};

 /**********************************************************************************
		 三相电检测原理：记录一个周期内的波形变化
		 正常相序:	 11 - 10 - 00 - 01
		 错相相序:	 11 - 01 - 00 - 10
		 缺相相序:	 11 - 01、00、10
		 欠压过压：  不考虑这种情况。
		 
		 正常相序：101-100-110-010-011-001
		 错相相序：101-001-011-010-110-100
		 缺相相序：000-001-101-100（且是错相，则反方向变化）
				   000-100-110-010（且是错相，则反方向变化）
				   000-010-011-001（且是错相，则反方向变化）
				   000-100
				   000-010
				   000-001		 
 **********************************************************************************/
 
	 const I08 PhsTbl[8][8] =
	 { //  00	01	10	 11
		 { 0,  -1,	 1,   0 },	 //00
		 { 1,	0,	 1,  -1 },	 //01
		 {-1,  -1,	 0,   1 },	 //10
		 { 0,	1,	-1,   0 },	 //11
	 };

/*******************************************************************************
 函 数: output()
 功 能: 继电器输出驱动
 参 数: 空
 返 回: 空
 其 它: 空
*******************************************************************************/
void output(void)
{
   //继电器输出
	if(gIO.output.bit.b0)      SET_BIT(GPIOC->ODR, 7);//1#压机     //D_OUT1
	else                        CLR_BIT(GPIOC->ODR, 7);
  	  
 	if(gIO.output.bit.b1)      SET_BIT(GPIOC->ODR, 6);//1#四通阀   //D_OUT2
	else                        CLR_BIT(GPIOC->ODR, 6);

	if(gIO.output.bit.b2)      SET_BIT(GPIOC->ODR, 5);//1#喷液阀   //D_OUT3
	else                        CLR_BIT(GPIOC->ODR, 5);

	if(gIO.output.bit.b3)	    SET_BIT(GPIOC->ODR, 4);//1#风机     //D_OUT4
	else                        CLR_BIT(GPIOC->ODR, 4);

	if(gIO.output.bit.b4)	    SET_BIT(GPIOC->ODR, 3);//2#压机     //D_OUT5
	else                        CLR_BIT(GPIOC->ODR, 3);

	if(gIO.output.bit.b5)	    SET_BIT(GPIOC->ODR, 2);//2#四通阀   //D_OUT6
	else                        CLR_BIT(GPIOC->ODR, 2);

	if(gIO.output.bit.b6)      SET_BIT(GPIOC->ODR, 1);//2#喷液阀   //D_OUT7
	else                        CLR_BIT(GPIOC->ODR, 1);

	if(gIO.output.bit.b7)      SET_BIT(GPIOE->ODR, 5);//2#风机     //D_OUT8
	else                        CLR_BIT(GPIOE->ODR, 5); 

	if(gIO.output.bit.b8)      SET_BIT(GPIOB->ODR, 3);//曲轴加热   //D_OUT9
	else                        CLR_BIT(GPIOB->ODR, 3); 

	if(gIO.output.bit.b9)      SET_BIT(GPIOB->ODR, 4);//空调泵     //D_OUT10
	else                        CLR_BIT(GPIOB->ODR, 4); 

	if(gIO.output.bit.b10)     SET_BIT(GPIOB->ODR, 5);//电辅热     //D_OUT11
	else                        CLR_BIT(GPIOB->ODR, 5); 

	if(gIO.output.bit.b11)     SET_BIT(GPIOB->ODR, 6);//热水泵     //D_OUT12
	else                        CLR_BIT(GPIOB->ODR, 6); 
}

/*******************************************************************************
 函 数: input()
 功 能: 开关量输入采集，并进行50ms消抖来确定输入的有效性(D_IN1~D_IN5)
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
 void input(void)
 {
   	U08 i;
   	BIT16 tmp;
   	static BIT16 tmp_bak;
   
   	tmp.word=0;
	
   	if(GET_BIT(GPIOG->IDR, 7))      tmp.bit.b0 = 0;//1#压机低压     //DIN1  0x0001
   	else                            tmp.bit.b0 = 1;
   	
	if(GET_BIT(GPIOE->IDR, 4))      tmp.bit.b1 = 0;//1#压机高压     //DIN2  0x0002
   	else                            tmp.bit.b1 = 1;
   	
	if(GET_BIT(GPIOE->IDR, 3))      tmp.bit.b2 = 0;//2#压机低压     //DIN3  0x0004
   	else                            tmp.bit.b2 = 1;
   	
	if(GET_BIT(GPIOE->IDR, 2))      tmp.bit.b3 = 0;//2#压机高压     //DIN4  0x0008
   	else                            tmp.bit.b3 = 1;
   	
	if(GET_BIT(GPIOE->IDR, 1))      tmp.bit.b4 = 0;//连锁开关       //DIN5  0x0010
   	else                            tmp.bit.b4 = 1;

   	if(GET_BIT(GPIOE->IDR, 0))      tmp.bit.b5 = 0;//线控开关       //DIN6  0x0020  
   	else                            tmp.bit.b5 = 1;
   	
	if(GET_BIT(GPIOD->IDR, 0))      tmp.bit.b6 = 0;//热水水流开关   //DIN7  0x0040
   	else                            tmp.bit.b6 = 1;
   	
	if(GET_BIT(GPIOD->IDR, 2))      tmp.bit.b7 = 0;//空调水流开关   //DIN8  0x0080
   	else                            tmp.bit.b7 = 1;
   	

   	for(i = 0; i < MAX_INPUT1; i++)
	{
		if(GET_BIT(tmp_bak.word, i) != GET_BIT(tmp.word, i))
		{
		   	input1_time[i]	= 0;		   
		   	if(GET_BIT(tmp.word, i))
		   	{
				SET_BIT(tmp_bak.word, i);
			}
		   	else
			{
				CLR_BIT(tmp_bak.word, i);
			}   
		}
		if (input1_time[i] != 0x7fff)
		{
			input1_time[i]++;
		}	
		if (input1_time[i] > 0x32)  
		{
			if(GET_BIT(tmp.word, i))
			{
				SET_BIT(gIO.input.word, i);
			}
			else
			{
				CLR_BIT(gIO.input.word, i);
			}
		 }
	  }
}

/*******************************************************************************
 函 数: jmp()
 功 能: 拨码开关采集，并进行50ms消抖来确定输入的有效性(S1~S8)
 参 数: 空
 返 回: 
 其 它: 空
*******************************************************************************/
 void jmp(void)
 {
    U08 i;

    for(i=0;i<2;i++)
    {
        if(ad_cur[i+2][0] < 620)        //sw1.2=0;S1.1=0;或者sw1.4=0;S1.3=0
        {
              CLR_BIT_BYTE(gIO.jmp.byte, 0+i*2);
              CLR_BIT_BYTE(gIO.jmp.byte, 1+i*2);
        }
        else if(ad_cur[i+2][0] < 700)   //sw1.2=0;S1.1=1;或者sw1.4=0;S1.3=1
        {

              SET_BIT_BYTE(gIO.jmp.byte, 0+i*2);
              CLR_BIT_BYTE(gIO.jmp.byte, 1+i*2);
        }
        else if(ad_cur[i+2][0] < 880)   //sw1.2=1;S1.1=0;或者sw1.4=1;S1.3=0
        {

              CLR_BIT_BYTE(gIO.jmp.byte, 0+i*2);
              SET_BIT_BYTE(gIO.jmp.byte, 1+i*2);
        }
        else                            //sw1.2=1;S1.1=1;或者sw1.4=1;S1.3=1
        {
              SET_BIT_BYTE(gIO.jmp.byte, 0+i*2);
              SET_BIT_BYTE(gIO.jmp.byte, 1+i*2);
        }
    }
}
 
 /*====================================================
		 功能: 处理三相电检测,严格1ms中断调用
		 输入: 无
		 输出: 无
 /====================================================*/
 void phase(void)
 {
 	//PO1_MOD = 0;  PO2_MOD = 0;
	phase_now = 0;
	if( DIN_PH1 != 0 ) 
	{
		phase_now |= 0x01;
	}	
	if( DIN_PH2 != 0 )
	{
		phase_now |= 0x02;
	}	
	 
	phase_time++;
	 
	if (phase_now != phase_last)
 	{
		phase_last = phase_now;
	}	
	else if (phase_en == TRUE) 
	{
		phase_count += PhsTbl[phase_now][phase_old];
		phase_old	  = phase_now;
	}
	else 
	{
		phase_en	 = TRUE;
		phase_old	 = phase_now;
		phase_time  = 0;
	}

	if (phase_time >= PHASE_TIMES) 
	{  	/*  一个测试周期 */
		if (phase_dly < 0xff)
		{
			phase_dly++;
		}	
		 
		if (phase_count >= PHASE_OK) 
		{
			phase_dly = 0; 		 
			gIO.phase = 0;
		}
		else if (phase_count <= PHASE_REVERSE) 
		{
			if ( phase_dly >= 10 )
			{	 
			 	gIO.phase = 1;
			}  /*  错相、持续1秒后报警 */
		}
		else 
		{
			if ( phase_dly >= 30 ) 
			{	 
				gIO.phase = 2;
			}  /*  缺相、持续3秒钟才报警 */
		}
		
		phase_count = 0;
		phase_time  = 0;
	}	 
}
/*====================================================
		功能: 步进电机驱动，1ms中断中调用
				停止时，需要励磁0.5秒
				开始时，需要励磁0.5秒
				脉冲的占空比为50％
				励磁周期为32ms
				励磁速度可调，从低速到高速
机在较高速或大惯量负载时，一般不在工作速度起动，
而采用逐渐升频提速，一电机不失步，二可以减少噪音同时可以提高停止的定位精度。
步进电机最好不使用整步状态，整步状态时振动大。
		输入: 调节的步数
/====================================================*/
	#define IS_EIGHT_STEP      (val_motor_fre_type == 0)		/* 四相八拍 */
	#define IS_FOUR_STEP       (val_motor_fre_type == 1)		/* 四相四拍 */

	#define STEP_KEEP		600			/* 驱动时间 */
	#define STEP_SPEED		(moter_fre[val_motor_step_fre])		/* 步进周期(ms) */

//	const  U08  StepTbl[] = { 0x00, 0x01, 0x03, 0x02, 0x06, 0x04, 0x0c, 0x08, 0x09 };		/* 开阀顺序 */
	const  U08  StepTbl[] = { 0x00, 0x09, 0x08, 0x0c, 0x04, 0x06, 0x02, 0x03, 0x01,};		/* 开阀顺序(由于硬件排列与LY514相反，故采用相反的励磁顺序) */
	const  U08  moter_fre[] = { 32,  16,  12,   10,   8,   6,   4};	/* 每步多少时间(ms) */
							//{ 31, 62, 83, 100, 125, 166, 250 };	/* PPS每秒多少步 */
	static U08  step_relay = 0;
	static U08  motor_step = 0;				/* 动作步骤 */
	
	static I08  step_idx[MAX_COMP] = {0,0};
	static U16  step_time[MAX_COMP] = {0,0};

void IO_Step(void)
{
	U08  step_out = 0;
	static U08 no = 0;
	static U08 no_next = 0;
	U08 i;
	
	for (i=0; i<MAX_COMP; i++)
	{
		if (step_time[i] != 0xffff) step_time[i]++;
	}
	switch (motor_step)
	{
		case 0 :	/* 停止(掉电) */
			step_out = 0;	
			motor_step = 10;
			break;
			
		case 10:	/* 换下一个步进电机 */
			no = no_next;
			
			if (++no_next >= MAX_COMP)
			{
				no_next = 0;
			}

			/* 测试程序中步进电机特殊处理，全部为0#电机的输出 */
			if (test_fg) no_next = 0;
			
			if (gIO.io_step_sv[no] != gIO.io_step_pv[no]) 
			{
				motor_step = 1;		step_time[no] = 0;
			}
			break;		

		case 1 :	/* 保持过渡 */
			step_out = step_idx[no];
			if (step_time[no] >= STEP_KEEP) 
			{
				if (gIO.io_step_sv[no] > gIO.io_step_pv[no]) 	  	    motor_step = 2;	/* 需要正转 */
				else if (gIO.io_step_sv[no] < gIO.io_step_pv[no]) 	motor_step = 3;	/* 需要反转 */
				else 										  		    motor_step = 0;	/* 需要停止 */
			}
			break;
		case 2 :	/* 正转 */
			if (gIO.io_step_sv[no] > gIO.io_step_pv[no]) 
			{
				if (step_time[no] >= STEP_SPEED) 
				{
					step_time[no] = 0;
					gIO.io_step_pv[no]++;

					if (IS_FOUR_STEP) 
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
			else 
			{
				motor_step = 1;		step_time[no] = 0;
				step_out = step_idx[no];
			}
			break;
		case 3 :	/* 反转 */
			if (gIO.io_step_sv[no] < gIO.io_step_pv[no]) 
			{
				if (step_time[no] >= STEP_SPEED) 
				{
					step_time[no] = 0;
					gIO.io_step_pv[no]--;

					if (IS_FOUR_STEP) 
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
			else 
			{
				motor_step = 1;		step_time[no] = 0;
				step_out = step_idx[no];
			}
			break;
		default :	
			motor_step = 0;
			break;
	}

	if (IS_FOUR_STEP) 
	{				/* 四拍,取2,4,6,8(防错用) */
		if 	(step_out & 0x01) 	step_out += 1;
	}
	step_out = StepTbl[step_out];

	switch(no)
	{
	  case  0://#1
	    if(step_out & 0x08)     SET_BIT(GPIOB->ODR, 7);//STEP1
	    else                    CLR_BIT(GPIOB->ODR, 7);
	    if(step_out & 0x04)     SET_BIT(GPIOF->ODR, 0);//STEP2
	    else                    CLR_BIT(GPIOF->ODR, 0);
	    if(step_out & 0x02)     SET_BIT(GPIOF->ODR, 3);//STEP3
	    else                    CLR_BIT(GPIOF->ODR, 3);
	    if(step_out & 0x01)     SET_BIT(GPIOF->ODR, 4);//STEP4
	    else                    CLR_BIT(GPIOF->ODR, 4);  
	    if(test_fg == FALSE)//测试程序中两电机齐动
	      break;

	  case  1://#2
	    if(step_out & 0x08)     SET_BIT(GPIOF->ODR, 5);//STEP5
	    else                    CLR_BIT(GPIOF->ODR, 5);
	    if(step_out & 0x04)     SET_BIT(GPIOF->ODR, 6);//STEP6
	    else                    CLR_BIT(GPIOF->ODR, 6);
	    if(step_out & 0x02)     SET_BIT(GPIOF->ODR, 7);//STEP7
	    else                    CLR_BIT(GPIOF->ODR, 7);
	    if(step_out & 0x01)     SET_BIT(GPIOA->ODR, 3);//STEP8
	    else                    CLR_BIT(GPIOA->ODR, 3);  
	      break;
	}
}
/*====================================================
		功能: AD初始化
		输入: 无
		输出: 无
/====================================================*/
void ad_init(void)
{
	U08 i,j;
	
	for (i=0; i<MAX_AD; i++)
	{
		for (j=0; j<MAX_AD_ARRAY; j++)
		{
			ad_temp_nor[i][j] = 0xffff;
			ad_temp_low[i][j] = 0xffff;
			ad_temp_mid[i][j] = 0xffff;
			ad_temp_high[i][j] = 0xffff;
		}
	}
	
	for (i=0; i<MAX_CUR+2; i++)
	{
		for (j=0; j<MAX_AD_ARRAY; j++)
		{
			ad_cur[i][j] = 0xffff;
		}
	}
		
	for (i=0; i<MAX_TEMP; i++)
	{
		mpv[i] = 0;
		sensor_dly[i] = 0;
	}	
}

/*====================================================
		功能: 由 电阻 求 温度(摄氏度) 温度放大10倍
		输入: 电阻值
		输出: 温度值
/====================================================*/
static I16 io_ad_r_to_t(I32 v, U08 opt)
{
	I16  tmp;
	U16  k, head, trail;
	RTC_DATA *pRTC;	/* AD数据表格 */

	pRTC = &TabRTC[opt];

	head 	= 0;
	trail 	= pRTC->num - 1;

	if (v >= pRTC->v[head]) 		tmp = pRTC->t0;		/* 最低测量温度 */
	else if (v <= pRTC->v[trail]) 	tmp = pRTC->tn;		/* 最高测量温度 */
	else
	{
		k = (head + trail) >> 1;
		do
		{
			if (v > pRTC->v[k]) 			/* 前部 */
			{
				trail = k;		k = (head + trail) >> 1;
			}
			else if (v < pRTC->v[k+1]) 		/* 后部 */
			{
				head = k + 1;	k = (head + trail) >> 1;
			}
			else
			{
				break;
			}
		} while (1);
		
		if (opt == SENSOR_TPYE_CUR)
		{
			tmp  = cur_value[k];
			tmp += (cur_value[k+1] - cur_value[k]) * (pRTC->v[k] - v) / (pRTC->v[k] - pRTC->v[k+1]);
		}
		else 
		{
			tmp  = pRTC->t0 + pRTC->td * k;
			tmp += pRTC->td * (pRTC->v[k] - v) / (pRTC->v[k] - pRTC->v[k+1]);
		}
	}
	
	return tmp;
}
/*====================================================
		功能: AD值缓存区
		输入: AD值，缓存地址
/====================================================*/
static void io_ad_to_buffer(U16 *pAd, U16 v)
{
 	U16 temp;
 
	temp = v & 0x03ff;	/* 10位AD值 */
	if (*pAd == 0xffff) {	/* 第一次读取 */
		*(pAd + 5) = temp;		*(pAd + 4) = temp;		*(pAd + 3) = temp;
		*(pAd + 2) = temp;		*(pAd + 1) = temp;		*(pAd + 0) = temp;
	}
	else {
		*(pAd + 5) = *(pAd + 4);	*(pAd + 4) = *(pAd + 3);	*(pAd + 3) = *(pAd + 2);
		*(pAd + 2) = *(pAd + 1);	*(pAd + 1) = *(pAd + 0);	*(pAd + 0) = temp;
	}
}
/*====================================================
		功能: AD的采样,1ms中断调用(单次扫描模式)
		输入: 无
		输出: 无
/====================================================*/

#define MAX_AD_IDX	(MAX_AD << 4)
static U08 ad_idx = 0;			/* AD通道 */
static U08 ad_time = 250;		/* 通道采样周期 */

void io_ad(void)
{
	BIT8 x,n;		//温度序号
	U08 y;		//步骤
	U08 tmp_l,tmp_h;
	U16 tmp;
	static U08 m=0;
	U08 const channel_table[MAX_AD]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//10路温度
    U08 const channel_table2[MAX_CUR+2]={0x08,0x09,0x01,0x02};//2路电流+2路开关量
	if (--ad_time != 0) return;		// 选通当前的4051通道 

	ad_time = 1;
		
	if (ad_idx == MAX_AD_IDX)  
	{
		ad_time = 10;				// 使采样周期正好为250ms 
	}			
	else 
	{
		x.byte = ad_idx >> 4;		//获得温度序号
		y = ad_idx & 0x0f;
		
		switch (y)
		{
			case 0 :	//切换温度序号，并启动转换，从常温开始
				if (x.byte < MAX_AD-2)
				{
					if (x.bit.b0 == 1)      SET_BIT(GPIOG->ODR, 4);//ADR1
					else                    CLR_BIT(GPIOG->ODR, 4);
					if (x.bit.b1 == 1)      SET_BIT(GPIOG->ODR, 3);//ADR2
					else                    CLR_BIT(GPIOG->ODR, 3);
					if (x.bit.b2 == 1)      SET_BIT(GPIOG->ODR, 2);//ADR3
					else                    CLR_BIT(GPIOG->ODR, 2);
					                        CLR_BIT(GPIOI->ODR, 0);//ADR4  片选信号
				}
				else
				{
				    n.byte = x.byte-(MAX_AD-2);
				    if (n.bit.b0 == 1)      SET_BIT(GPIOG->ODR, 4);//ADR1
					else                    CLR_BIT(GPIOG->ODR, 4);
					if (n.bit.b1 == 1)      SET_BIT(GPIOG->ODR, 3);//ADR2
					else                    CLR_BIT(GPIOG->ODR, 3);
					if (n.bit.b2 == 1)      SET_BIT(GPIOG->ODR, 2);//ADR3
					else                    CLR_BIT(GPIOG->ODR, 2);
					                        SET_BIT(GPIOI->ODR, 0);//ADR4  片选信号
				}
				CLR_BIT(GPIOG->ODR, 6);	//PGA1//选择常温
				CLR_BIT(GPIOG->ODR, 5); //PGA2
				ADC2->CSR = channel_table[x.byte] ; //轮询选择通道,温度的都是8通道，顺带清零转换结束标志		
				break;
				
			case 1 :					
				SET_BIT(ADC2->CR1,0);							//启动AD转换 
				break;
				
			case 2 :
				if (GET_BIT(ADC2->CSR,7) == 0)					//未转换结束，就退出等待结束 
				{
					return;
				}			
				tmp_l = ADC2->DRL;
		   		tmp_h = ADC2->DRH;
	      		tmp = tmp_l + tmp_h * 0x100;						//获得AD值
				io_ad_to_buffer(&ad_temp_nor[x.byte][0], tmp);	//读AIN0的10位AD(常温) 
				SET_BIT(GPIOG->ODR, 6);//PGA1
				CLR_BIT(GPIOG->ODR, 5);//PGA2
				CLR_BIT(ADC2->CSR,7);							//转换结束标志硬件置位，软件清零
				break;
				
			case 3 :
				SET_BIT(ADC2->CR1,0);							//启动AD转换
				break;	
				
			case 4 :
				if (GET_BIT(ADC2->CSR,7) == 0)					//未转换结束，就退出等待结束 
				{
					return;
				}			
				tmp_l = ADC2->DRL;
		   		tmp_h = ADC2->DRH;
	      		tmp = tmp_l + tmp_h * 0x100;						//获得AD值
				io_ad_to_buffer(&ad_temp_low[x.byte][0], tmp);	//读AIN0的10位AD(低温) 
				CLR_BIT(GPIOG->ODR, 6);//PGA1
				SET_BIT(GPIOG->ODR, 5);//PGA2
				CLR_BIT(ADC2->CSR,7);							//转换结束标志硬件置位，软件清零
				break;
				
			case 5 :
				SET_BIT(ADC2->CR1,0);							//启动AD转换
				break;
				
			case 6 :
				if (GET_BIT(ADC2->CSR,7) == 0)					//未转换结束，就退出等待结束 
				{
					return;
				}			
				tmp_l = ADC2->DRL;
		   		tmp_h = ADC2->DRH;
	      		tmp = tmp_l + tmp_h * 0x100;						//获得AD值
				io_ad_to_buffer(&ad_temp_mid[x.byte][0], tmp);	//读AIN0的10位AD(中温) 
				SET_BIT(GPIOG->ODR, 6);//PGA1
				SET_BIT(GPIOG->ODR, 5);//PGA2		
				CLR_BIT(ADC2->CSR,7);							//转换结束标志硬件置位，软件清零
				break;
				
			case 7 :
				SET_BIT(ADC2->CR1,0);							//启动AD转换
				break;
				
			case 8 :
				if (GET_BIT(ADC2->CSR,7) == 0)					//未转换结束，就退出等待结束 
				{
					return;
				}			
				tmp_l = ADC2->DRL;
		   		tmp_h = ADC2->DRH;
	      		tmp = tmp_l + tmp_h * 0x100;						//获得AD值
				io_ad_to_buffer(&ad_temp_high[x.byte][0], tmp);	//读AIN0的10位AD(高温) 
				ADC2->CSR = channel_table2[m];
				break;
				
            case 9:
                SET_BIT(ADC2->CR1,0);							//启动AD转换
                break;
                
            case 10:
                if (GET_BIT(ADC2->CSR,7) == 0)					//未转换结束，就退出等待结束 
			    {
				    return;
			    }			
			    tmp_l = ADC2->DRL;
		    	tmp_h = ADC2->DRH;
	      	    tmp = tmp_l + tmp_h * 0x100;						//获得AD值
			    io_ad_to_buffer(&ad_cur[m][0], tmp);			//读AIN8的10位AD(电流1)
			    if(++m >= MAX_CUR+2) m=0;
			    
			    ad_end |= (0x0001 << x.byte);
				ad_idx += 5;								// 跳过case9-15	
			    break;
				
			default :
				break;
		}
	}
	if (++ad_idx > MAX_AD_IDX) ad_idx = 0;		//高四位切换温度选通，低四位切换读取步骤

}

/*====================================================
		功能: AD的平均滤波函数
		输入: 数据缓存
		输出: 滤波值
/====================================================*/
static U16 AD_FilterAverage(U16 *pAd)
{
	U08 i;
	U16 ad_min, ad_max, ad_sum;
	U16 ad_tmp[MAX_AD_ARRAY];
	
	DISABLE_INTERRUPT;
		ad_tmp[0] = pAd[0];
		ad_tmp[1] = pAd[1];
		ad_tmp[2] = pAd[2];
		ad_tmp[3] = pAd[3];
		ad_tmp[4] = pAd[4];
		ad_tmp[5] = pAd[5];
	ENABLE_INTERRUPT;
	
	ad_min = ad_tmp[0]; 	ad_max = ad_tmp[0]; 	ad_sum = ad_tmp[0];
	for (i=1; i<MAX_AD_ARRAY; i++)
	{
		ad_sum += ad_tmp[i];
		if (ad_min > ad_tmp[i]) ad_min = ad_tmp[i];
		if (ad_max < ad_tmp[i]) ad_max = ad_tmp[i];
	}
	
	ad_sum -= ad_min;	ad_sum -= ad_max;
	ad_sum /= (MAX_AD_ARRAY - 2);
	
	return ad_sum;
}

/*====================================================
		功能: AD的滤波函数
		输入: 无
		输出: 无
/====================================================*/
static void AD_Filter(U08 x, U08 opt)
{
	switch (opt)
	{
	case SENSOR_TPYE_CUR  :		/* 电流 */
		if (x >= MAX_CUR)
		{
			x = 0;
		}	
		gIO.cur_ad[x] = AD_FilterAverage(&ad_cur[x][0]);
		break;
	default :		/* 每路温度有4个AD值 */
		if (x >= MAX_TEMP)
		{
			x = 0;
		}	
		gIO.temp_ad[x][0] = AD_FilterAverage(&ad_temp_low[x][0]);	/* L */
		gIO.temp_ad[x][1] = AD_FilterAverage(&ad_temp_mid[x][0]);	/* M */
		gIO.temp_ad[x][2] = AD_FilterAverage(&ad_temp_high[x][0]);	/* H */
		gIO.temp_ad[x][3] = AD_FilterAverage(&ad_temp_nor[x][0]);	/* N */
		break;
	}
}

/*====================================================
		功能: 获取NTC的电阻
		输入: 第几路
		输出: 电阻
/====================================================*/
static I32 AD_GetR(U08 x)
{
	I32  r;		/* 电阻 */
	I16  ad_nor, ad_low, ad_mid, ad_hig;

	ad_nor = gIO.temp_ad[x][3];	/* 取常温AD */
	ad_low = gIO.temp_ad[x][0];	/* 取低温AD */
	ad_mid = gIO.temp_ad[x][1];	/* 取中温AD */
	ad_hig = gIO.temp_ad[x][2];	/* 取高温AD */
			
	if (ad_nor > 1000) 		return SENSOR_BREAKED;	/* 断路 */
	else if (ad_hig < 16) 	return SENSOR_SHORTED;	/* 短路 */
	else 
	{
		if (ad_nor > 200) 
		{
			r = (I32)(ad_nor - NOR_0) * 30000 / (NOR_A + NOR_0 - ad_nor);	/* 低温区-50~35度(ad: 981~182) */
		}
		else if (ad_low > 200) 
		{
			r = (I32)(ad_low - LOW_0) * 30000 / (LOW_A + LOW_0 - ad_low);	/* 低温区-50~35度(ad: 981~182) */
		}
		else if (ad_mid > 200) 
		{
			r = (I32)(ad_mid - MID_0) * 30000 / (MID_A + MID_0 - ad_mid);	/* 中温区35~85度(ad: 849~163) */
		}
		else 
		{
			r = (I32)(ad_hig - HIG_0) * 30000 / (HIG_A + HIG_0 - ad_hig);	/* 高温区85~150度(ad: 733~130) */
		}
		
		return r;
	}
}
/*====================================================
		功能: 获取NTC的参考地与参考电压
		输入: 无 
		输出: 无
/====================================================*/
static void AD_GetVef(void)
{
	I16  VEF_T0, VEF_T1, VEF_T2, VEF_T3;

	VEF_T0	= gIO.temp_ad[7][3];//TSET_H校准电阻AD值

	VEF_T1	= gIO.temp_ad[7][0];
	
	VEF_T2	= gIO.temp_ad[7][1];
	
	VEF_T3	= gIO.temp_ad[7][2];
	
	//debug_5[8] = VEF_G1;
	//debug_5[9] = VEF_T1;
	
	/* 第一个运放的零点漂移 */
	NOR_0 =  (VEF_T0 - 5);
	
	/* 第二个运放的零点漂移 */
	LOW_0 = (VEF_T1 - 20); 
	
	/* 第三个运放的零点漂移 */
	MID_0 = (VEF_T2 - 81); 

	/* 第四个运放的零点漂移 */
	HIG_0 = (VEF_T3 - 326); 
}
#endif

const I32 R22_PressTemp_Tab[]=
{
    //-40 -39  -38  -37  -36  -35  -34  -33  -32  -31
    5,  10,  15,  21,  26,  32,  38,  44,  50,  57,
    //-30
    64,  71,  78,  86,  93,  101, 110, 118, 127, 136,
    //-20
    145, 155,    165,    175,    185,    196,    207,    219,    230,    242,
    //-10
    255,    267,    281,    294,    308,    322,    336,    351,    366,    382,
    //0  1  2  3  4  5  6  7  8  9
    398, 415, 431, 449, 466, 484, 503, 522, 541, 561,
    //10
    581, 602, 623, 645, 667, 690, 713, 736, 760, 785,
    //20
    810, 836, 862, 889, 916, 944, 973, 1002, 1031, 1062,
    //30
    1092, 1124, 1156, 1188, 1221, 1255, 1290, 1325, 1361, 1397,
    //40
    1434, 1472, 1510, 1550, 1589, 1630, 1671, 1713, 1756, 1799,
    //50
    1843, 1888, 1934, 1980, 2028, 2076, 2125, 2174, 2225, 2276,
    //60
    2328,
};

//过冷度计算用泡点温度，过热度计算用露点温度计算
//此处用的泡点温度，原因是在泡点温度时才能保证所有的冷媒已完全变为液体
const I32 R407C_PressTemp_bubble_Tab[]=  /* 泡点温度 */
{
    //-30 -29  -28  -27  -26  -25  -24  -23  -22  -21
    87,  95,  103, 112, 120, 130, 139, 149, 159, 169,
    //-20
    179, 190, 202, 213, 225, 237, 250, 263, 276, 290,
    //-10
    304, 318, 333, 348, 364, 380, 396, 413, 431, 448,
    //0  1  2  3  4  5  6  7  8  9
    466, 485, 504, 524, 544, 564, 585, 607, 629, 651,
    //10
    674, 698, 722, 746, 772, 797, 824, 850, 878, 906,
    //20
    935, 964, 994, 1024, 1055, 1087, 1119, 1152, 1186, 1220,
    //30
    1255, 1291, 1327, 1364, 1402, 1440, 1479, 1519, 1560, 1602,
    //40
    1644, 1687, 1730, 1775, 1820, 1866, 1913, 1961, 2010, 2059,
    //50
    2109, 2161, 2213, 2266, 2319, 2374, 2430, 2487, 2544, 2603,
    //60
    2662,
};
const I32 R407C_PressTemp_dew_Tab[]=  /* 露点温度 */
{
    //-30 -29  -28  -27  -26  -25  -24  -23  -22  -21
    36,  43,  50,  56,  64,  71,  79,  88,  95,  103,
    //-20
    112, 121, 131, 140, 150, 161, 171, 182, 193, 205,
    //-10
    217, 229, 242, 255, 269, 282, 297, 311, 326, 342,
    //0  1  2  3  4  5  6  7  8  9
    358, 374, 391, 408, 426, 444, 462, 482, 501, 521,
    //10
    542, 563, 585, 607, 630, 653, 677, 701, 726, 752,
    //20
    778, 805, 832, 860, 889, 918, 948, 979, 1010, 1043,
    //30
    1075, 1109, 1143, 1178, 1214, 1250, 1287, 1235, 1364, 1403,
    //40
    1444, 1485, 1527, 1570, 1614, 1658, 1704, 1750, 1798, 1846,
    //50
    1895, 1945, 1996, 2048, 2102, 2156, 2211, 2267, 2324, 2383,
    //60
    2442,
};
const I32 R410a_PressTemp[]=
{
    //-50  -49  -48  -47 -46  -45  -44 -43  -42  -41 
       13,  19,  25,  31, 38,  44,  51, 57,  65,  72,
       
    //-40  -39  -38  -37 -36  -35  -34  -33 -32  -31 
       81,  88,  96, 106, 115, 124, 135, 143, 155, 164,
       
    //-30  -29  -28   -27 -26  -25  -24 -23  -22  -21 
      175, 186,  198, 211, 224, 234, 248, 263, 275, 291,
    
    //-20 -19  -18  -17  -16  -15  -14  -13  -12  -11
    301, 317, 332, 349, 365, 383, 400, 419, 437, 456,
    //-10
    476, 496, 517, 538, 560, 582, 605, 629, 653, 677,
    //0  1  2  3  4  5  6  7  8  9
    703, 729, 755, 782, 810, 839, 868, 898, 928, 959,
    //10
    991, 1024, 1057, 1091, 1126, 1162, 1198, 1235, 1273, 1312,
    //20
    1352, 1392, 1433, 1475, 1518, 1562, 1607, 1653, 1699, 1747,
    //30
    1795, 1845, 1895, 1946, 1999, 2052, 2106, 2162, 2218, 2279,
    //40
    2334, 2394, 2455, 2516, 2579, 2643, 2709, 2755, 2843, 2911,
    //50
    2981, 3053, 3125, 3199, 3273, 3350, 3426, 3505, 3584, 3666,
    //60
    3748, 3832, 3915, 4002, 4088, 4177, 4265, 4356, 4446, 4538,
    //70
    4630,
};

const I32 R32_PressTemp[]= 
{
 /* -50   -49   -48   -47   -46   -45   -44   -43   -42   -41 */
    9,    14,   20,   26,   33,   39,   46,   53,   61,   68, 
 /* -40   -39   -38   -37   -36   -35   -34   -33   -32   -31 */
    76,   84,   93,   102,  111,  120,  130,  140,  150,  161, 
 /* -30   -29   -28   -27   -26   -25   -24   -23   -22   -21 */
    172,  184,  195,  208,  220,  233,  247,  260,  275,  289,
 /* -20   -19   -18   -17   -16   -15   -14   -13   -12   -11 */
    304,  320,  336,  352,  369,  387,  405,  423,  442,  461,
 /* -10   -9    -8    -7    -6    -5    -4    -3    -2    -1  */
    481,  502,  523,  544,  567,  589,  613,  636,  661,  686,
 /* 0     1     2     3     4     5     6     7     8     9   */
    712,  738,  765,  793,  821,  850,  880,  910,  941,  973,
 /* 10    11    12    13    14    15    16    17    18    19  */
    1006, 1039, 1073, 1108, 1143, 1179, 1217, 1255, 1293, 1333,
 /* 20    21    22    23    24    25    26    27    28    29  */
    1372, 1414, 1457, 1500, 1544, 1588, 1634, 1681, 1728, 1777,
 /* 30    31    32    33    34    35    36    37    38    39  */
    1826, 1877, 1928, 1981, 2034, 2089, 2144, 2201, 2258, 2317,
 /* 40    41    42    43    44    45    46    47    48    49  */
    2377, 2438, 2500, 2563, 2628, 2693, 2760, 2828, 2898, 2968,
 /* 50    51    52    53    54    55    56    57    58    59  */
    3040, 3113, 3187, 3263, 3340, 3419, 3498, 3580, 3662, 3746,
 /* 60    61    62    63    64    65    66    67    68    69  */
    3832, 3919, 4008, 4098, 4190, 4283, 4378, 4475, 4573, 4673,
 /* 70    71    72    73                                      */
    4775, 4879, 4985, 5093,
};

/* (表压)R290-bar-℃ */
const I32 R290_PressTemp[] =
{
    /*   -50    -49    -48    -47    -46    -45    -44    -43    -42    -41  */
         -31,   -27,   -24,   -20,   -16,   -12,    -8,    -4,     1,     5, 

    /*   -40    -39    -38    -37    -36    -35    -34    -33    -32    -31  */
          10,    15,    20,    25,    30,    36,    42,    48,    54,    60, 

    /*   -30    -29    -28    -27    -26    -25    -24    -23    -22    -21  */
          67,    73,    80,    87,    95,   102,   110,   118,   126,   135, 

    /*   -20    -19    -18    -17    -16    -15    -14    -13    -12    -11  */
         143,   152,   161,   171,   180,   190,   200,   211,   222,   233, 

    /*   -10     -9     -8     -7     -6     -5     -4     -3     -2     -1  */
         244,   256,   267,   280,   292,   305,   318,   331,   345,   359, 

    /*     0      1      2      3      4      5      6      7      8      9  */
         373,   388,   403,   418,   434,   450,   466,   483,   500,   517, 

    /*    10     11     12     13     14     15     16     17     18     19  */
         535,   553,   572,   591,   610,   630,   650,   671,   692,   713, 

    /*    20     21     22     23     24     25     26     27     28     29  */
         735,   757,   780,   803,   827,   851,   875,   900,   926,   951, 

    /*    30     31     32     33     34     35     36     37     38     39  */
         978,  1004,  1032,  1060,  1088,  1117,  1146,  1176,  1206,  1237, 

    /*    40     41     42     43     44     45     46     47     48     49  */
        1268,  1300,  1332,  1365,  1399,  1433,  1468,  1503,  1539,  1575, 

    /*    50     51     52     53     54     55     56     57     58     59  */
        1612,  1650,  1688,  1726,  1766,  1806,  1846,  1888,  1930,  1972, 

    /*    60     61     62     63     64     65     66     67     68     69  */
        2015,  2059,  2104,  2149,  2195,  2242,  2289,  2337,  2386,  2435, 

    /*    70     71     72     73     74     75     76     77     78     79  */
        2485,  2536,  2588,  2641,  2694,  2748,  2803,  2859,  2915,  2972, 

    /*    80     81     82     83     84     85     86     87     88     89  */
        3031,  3090,  3150,  3210,  3272,  3335,  3398,  3463,  3529,  3595, 

    /*    90     91  */
        3663,  3731,
};

TAB_DATA const tab_dew_array[] = /* 露点温度，计算过热度，蒸发用 */
{
    {  R22_PressTemp_Tab,        101,  -4000,  6000,  100  },       /* 放大100倍 */
    {  R407C_PressTemp_dew_Tab,  91,   -3000,  6000,  100  },       /* 放大100倍 */
    {  R410a_PressTemp,          121,  -5000,  7000,  100  },       /* 放大100倍 */
    {  R32_PressTemp,            124,  -5000,  7300,  100  },       /* 放大100倍 */
    {  R290_PressTemp,           142,  -5000,  9100,  100  },       /* 放大100倍 */
};

TAB_DATA const tab_bub_array[] =  /* 泡点温度，计算过冷度，冷凝用 */
{
    {  R22_PressTemp_Tab,           101,  -4000,  6000,  100  },    /* 放大100倍 */
    {  R407C_PressTemp_bubble_Tab,  91,   -3000,  6000,  100  },    /* 放大100倍 */
    {  R410a_PressTemp,             121,  -5000,  7000,  100  },    /* 放大100倍 */
    {  R32_PressTemp,               124,  -5000,  7300,  100  },    /* 放大100倍 */
    {  R290_PressTemp,              142,  -5000,  9100,  100  },    /* 放大100倍 */
};


/*====================================================
函数：press_to_temp
功能：求取当前压力对应的饱和温度
参数：输入量当前值; 表选项(选取哪张表)
返回：饱和温度值(0.01)
/====================================================*/
I16 Press_to_Temp(I16 src_value, U08 tab_idx , U08 type)
{   
    I16 tmp;

    if (SensorCanUse(src_value))
    {
        if(type == PRESS_TEMP_DEW) /* 露点 */
        {
            tmp = tab_lookup(src_value, &tab_dew_array[tab_idx]);
        }
        else
        {
            tmp = tab_lookup(src_value, &tab_bub_array[tab_idx]);
        }
    }
    else
    {
        tmp = SENSOR_NOEXIST;
    }

    return tmp;
}



/*******************************************************************************
 函 数: AiSignalConvert()
 功 能: 模拟量信号转换为模拟量压力值等，并加上修正值
 参 数: signal对应模拟量信号值； pv_type传感器类型；CorrectValue模拟量值（如压力值）的修正值
 返 回: result 对应的模拟量值（如压力值），2位小数
 其 它: 参考X1.HL006A.K11.001-1\V100B23中ad.c的ad_read_comp_pv()
*******************************************************************************/
I16 AiSignalConvert(I16 signal, I16 pv_type, I16 CorrectValue)     
{
    I16 scale_max=1,scale_min=0;	// 设为1和0是防止传入下层参数后相减为0并参与除法
    I16 sig_max=1,sig_min=0;
    U08 type = AI_NONE;  
    
    I16 result = ~SENSOR_NOEXIST;

    if(signal == SENSOR_NOEXIST 
        || signal == SENSOR_SHORTED 
        || signal == SENSOR_BREAKED )
    {
        result = signal;
        return result;
    }

    //传感器类型和量程选择
#if 0
    pv_type = pv_type;
    if (val_J6_prs_sensor_type == USED)
    {
        type = (U08)val_J6_prs_sensor_type;  
        scale_max = val_J6_scale_max*10;
        scale_min = val_J6_scale_min*10;  
    }
    else
    {
        type = (U08)val_J5_prs_sensor_type; 
        scale_max = val_J5_scale_max*10;
        scale_min = val_J5_scale_min*10;
    }
#endif
    
    switch(pv_type)
    {
       case AI_PRS_HIGH:
            if (USE_REGULAR_PRS)
            {
                type = (U08)val_J6_prs_sensor_type; 
                scale_max = val_J6_scale_max*10;
                scale_min = val_J6_scale_min*10;
            }
            else
            {
                type = (U08)val_J5_prs_sensor_type;  
                scale_max = val_J5_scale_max*10;
                scale_min = val_J5_scale_min*10; 
            }
            break;
            
        case AI_PRS_LOW:
            type = (U08)val_J6_prs_sensor_type; 
            scale_max = val_J6_scale_max*10;
            scale_min = val_J6_scale_min*10;
            break;        
    }
    
    switch(type)
    {
        case AI_05_45V:
            sig_min = 500;
            sig_max = 4500; //三位小数
            break;
            
//        case AI_0_5V:
//        case AI_0_10V:
//        case AI_4_20MA:
        default:
            result =  SENSOR_NOEXIST;
            break;            
    }
    
    if(result != SENSOR_NOEXIST)
    {
        result = fun_line_value(signal, sig_max, sig_min, scale_max, scale_min);    
        result += CorrectValue;
    }

    return result;
    
}

/*====================================================
		功能: 读温度与电流值
		输入: 无
		输出: 无
/====================================================*/
void io_temp_read(void)
{
	/* 温度 */
	PunpGetTemp(&gIO.temp[0], MAX_TEMP);

	/* 相序 */	/* N0004 */
  	gIO.phase = (use_phase_lack) ? PunpGetPhase() : 0;

	/* 输入输出 */
	PunpGetDi(&gIO.input[0], MAX_DI_TOTAL_NUM);
	PunpSetDo(&gIO.output[0], MAX_DO_NUM);
	ARRCLR(gIO.output);
}
/* N0004 */
/*====================================================
		功能: 读pwm风机转速
		输入: 
		输出: 无
/====================================================*/
#define MAX_AVE  10      //  平均值
void io_speed_read(void)
{
    U08 i;
//	U16 tmp=0;
//	U08 i,j;
//	static U16 avbuf[FREQSPEEDNUM][MAX_AVE];
//	static U08 first_getin=0;
//	if(pwminput_fg)
//	{
//		pwminput_fg = 0;
//		if(first_getin)
//		{
//			first_getin = 0;
//			    for (i=0; i<FREQSPEEDNUM; i++)
//				{
//					tmp = PunpGetPwmInput(i);
//					for(j=0; j<MAX_AVE; j++)
//					{
//						avbuf[i][j]=tmp;
//					}
//				}
//		}
//		else
//		{
//		    for (i=0; i<FREQSPEEDNUM; i++)
//			{
//				tmp = PunpGetPwmInput(i);
//				for(j=0; j<(MAX_AVE-1); j++)
//				{
//					avbuf[i][j]=avbuf[i][j+1];
//				}
//				avbuf[i][j]=tmp;
//			}
//		}
//	}
    for (i=0; i<FREQSPEEDNUM; i++)
	{
//		tmp=0;
//        for(j=0; j<(MAX_AVE); j++)
//		{
//			tmp += avbuf[i][j];
//		}
		speedpv[i] =(GET_EXIO_FAN_PV(0, i)*60)/(val_fan_pwm_pulse*MAX_AVE);
	}

}

/* N0004 */
/*******************************************************************************
 函 数: adjustPwmRefPercent()
 功 能: EC风机PWM调速的硬件电路PWM脉宽占空比，与对应输出的PWM平均电压值并不是线性关系，需要转换
 参 数: PwmRefPercent--PWM输出电压给定百分比N%，它通常是上位机给的EC风机转速百分比N%（默认风机转速与其控制电压信号是线性正比关系），0--100%; 
 返 回: 经过软件校正硬件后的PWM输出电压给定百分比，使得实际输出电压值接近给定值N%，0--100.0%
 其 它: 空
*******************************************************************************/
#define PWM_COEF 10			/* 10：精度为0.1%；1：精度为1% */
U16 adjustPwmRefPercent(U16 PwmRefPercent)
{
    U16 PwmRefPercentAdjustVal;
         if(PwmRefPercent <    5*PWM_COEF)               PwmRefPercentAdjustVal = 0;
    else if(PwmRefPercent >= 100*PWM_COEF)               PwmRefPercentAdjustVal = 1000; 
    else
    {
        // 根据硬件PWM输出占空比与实际输出电压均值关系，分段线性计算：
             if(PwmRefPercent <= 50*PWM_COEF)   PwmRefPercentAdjustVal = ( 68L * PwmRefPercent -  170L*PWM_COEF)/(10*PWM_COEF);
        else if(PwmRefPercent <= 70*PWM_COEF)   PwmRefPercentAdjustVal = ( 91L * PwmRefPercent - 1308L*PWM_COEF)/(10*PWM_COEF);
        else if(PwmRefPercent <= 80*PWM_COEF)   PwmRefPercentAdjustVal = (103L * PwmRefPercent - 2140L*PWM_COEF)/(10*PWM_COEF);
        else if(PwmRefPercent <= 90*PWM_COEF)   PwmRefPercentAdjustVal = (165L * PwmRefPercent - 7100L*PWM_COEF)/(10*PWM_COEF);
        else                                    PwmRefPercentAdjustVal = (226L * PwmRefPercent -12600L*PWM_COEF)/(10*PWM_COEF);
    }
    return (PwmRefPercentAdjustVal);
}
/* N0004 */
/*====================================================
		功能:处理风机转速
		输入: 
		输出: 无
/====================================================*/
void io_speed_deal(void)
{
//	U16 tmp_pwm[FREQSPEEDNUM]={0,0};
//    U08 fan_pwm_channal[FREQSPEEDNUM] = {CHANNEL_FAN1, CHANNEL_FAN2};
	U16 i;

	io_speed_read();
	if((TestProgStaus==TPS_NO_TEST_CMD)||(TestProgStaus==TPS_DISENABLE_TEST))
	{
      	for (i=0; i<FREQSPEEDNUM; i++)
    	{
            /* 把变频风机转速与占空比当作线性处理 */
//            tmp_pwm[i] = adjustPwmRefPercent(speedsv[i]);
            speed_percent[i] = pwm_fan_speed_to_percent(i, speedsv[i], pv_cell_fan_now(i));
//            tmp_pwm[i] = adjustPwmRefPercent(speed_percent[i]);

            //PunpSetPwmFreqDuty(50, tmp_pwm[i], fan_pwm_channal[i]);
    	}
	}
}

