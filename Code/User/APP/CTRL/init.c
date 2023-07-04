#define CONFIG_GLOBAL
#include "includes.h"
/********************************************************************************************************
*
* File                : GW532A.c
* Hardware Environment: 
* Build Environment   : ST Visual Develop 4.1.6
* Version             : V1.0
* By                  : 赵志亮
* data                : 2012-03-22
* 只调用一次的硬件底层配置文件完全和应用层隔离。                             
*
*********************************************************************************************************/

//u32 LSIMeasurment(void);
#if 0
/*******************************************************************************
 函 数: SYS_DelayUS()
 功 能: 延时微秒
 参 数: usec--微秒; 
 返 回: 空
 其 它: 该函数延时不精确，可用定时器来实现精确延时。
 		忽略了判断条件等指令时间，会有些误差(也没关系)，但需保证延时时间是够的
*******************************************************************************/
void SYS_DelayUS(U16 usec)	 
{ 
	while (usec-- != 0)
	{
		//CPU晶振为16M
		ASM_NOP;ASM_NOP;ASM_NOP;ASM_NOP;ASM_NOP;
		ASM_NOP;ASM_NOP;ASM_NOP;ASM_NOP;ASM_NOP;		
		ASM_NOP;ASM_NOP;ASM_NOP;ASM_NOP;ASM_NOP;	

//		ASM_NOP;ASM_NOP;ASM_NOP;ASM_NOP;ASM_NOP;
//		ASM_NOP;ASM_NOP;ASM_NOP;ASM_NOP;ASM_NOP;		
//		ASM_NOP;ASM_NOP;ASM_NOP;ASM_NOP;ASM_NOP;		
	}
}

/*******************************************************************************
 函 数: SYS_DelayMS()
 功 能: 延时毫秒
 参 数: msec--毫秒; 
 返 回: 空
 其 它: 空
*******************************************************************************/
void SYS_DelayMS(U16 msec)	 
{
	while (msec-- != 0)
	{
		wdg_clear();
		SYS_DelayUS(1000);
	}
}


/***********************************************************************************
Function Name: 	init_system_crystal
Description:	时钟初始化设置
Parameters: none
Return value: none
***********************************************************************************/
void config_clock(void)
{

	//CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV2);
	//启用内部高速晶振且2分频16MHz/2=8MHz
	CLK->SWR = 0xe1;     				//主时钟切换寄存器 选择HSI为主时钟
	//CLK->CKDIVR=0x08;   				//2分频
	CLK->CKDIVR = 0x00;   				//1分频
	CLK->SWCR |= 0x02;      			//开启切换
	while((CLK->SWCR & 0x01)==0x01);  	//等待晶振稳定
	CLK->SWCR &=(~0x02);      			//关闭切换

/*
	//启用外部晶振不分频
	CLK->ECKR |=0X1; //开启外部时钟 
	while(!(CLK->ECKR&0X2)); //等待外部时钟rdy 
	CLK->CKDIVR &= 0XF8; //CPU无分频 
	//CLK->CKDIVR=0x01;   //外部2分频
	CLK->SWR = 0XB4; //选择外部时钟 
	while(!(CLK->SWCR&0X8)); //这里要等 
	CLK->SWCR |=0X2; //使能外部时钟 
*/	
}
/***********************************************************************************
Function Name: 	config_pin
Description:	IO引脚初始化设置,NTC和电流在hardwareinit_AD(void)中处理
Parameters: none
Return value: none
***********************************************************************************/
void config_pin(void)
{
    //A口
	GPIOA->DDR = 0x68;		//TXD、R-/T通信状态控制，输出,RXD\OSCIN\OSCOUT输入
  	GPIOA->CR1 = 0x68;		//TXD、R-/T,STEP8推挽输出,其它浮空输入
  	GPIOA->CR2 = 0x68;		//输出频率最大为10MHZ
  	
    //B口
  	GPIOB->DDR = 0xF8;    	//STEP1\OUT2\OUT1\OUT0\OUT9输出,S2\S1\CUR2输入
  	GPIOB->CR1 = 0xF8;    	//推挽输出
  	GPIOB->CR2 = 0xF8;     	//输出频率最大为10MHZ

    //C口
	GPIOC->DDR = 0xFE;    	//OUT1~OUT7输出
  	GPIOC->CR1 = 0xFE;    	//推挽输出
  	GPIOC->CR2 = 0xFE;	   	//输出频率最大为10MHZ

    //D口
	GPIOD->DDR = 0x38;		//PRO_TXD\LED1\PRO_R/T输出
	GPIOD->CR1 = 0x38;	  	//推免输出 
	GPIOD->CR2 = 0x38;   

	//E口
	GPIOE->DDR = 0x20;		//OUT8输出口
	GPIOE->CR1 = 0x20;	  	//推免输出
	GPIOE->CR2 = 0x20;	  	//输出频率最大10MH 

    //F口
	GPIOF->DDR = 0xFF;		//输出
	GPIOF->CR1 = 0xFF;	  	//推免输出
	GPIOF->CR2 = 0xFF;	  	//输出频率最大10MH 

    //G口
  	GPIOG->DDR = 0x7C;    	//
	GPIOG->CR1 = 0x7C;    	//推免输出
  	GPIOG->CR2 = 0x7C;    	//输出频率最大为10MHZ 

  	//I口
  	GPIOI->DDR = 0x01;    	//
	GPIOI->CR1 = 0x01;    	//推免输出
  	GPIOI->CR2 = 0x01;    	//输出频率最大为10MHZ 
 }
/***********************************************************************************
Function Name: 	config_ad
Description:	AD初始化设置
Parameters: none
Return value: none
***********************************************************************************/
void config_ad(void)
{

	U08 i,j;

    ADC2->TDRL |= 0x07;  //禁止AIN0~AIN2  施密特触发器
	ADC2->TDRH |= 0x03;  	//禁止AIN8~AIN9  施密特触发器
   	ADC2->CSR = 0x00 ;   	//选择AIN0
   	ADC2->CR2 = 0x08 ;   	//右对齐
   	ADC2->CR1 = 0x71 ;    	//单次转换
   	
	for(i=0;i<250;i++);//等待通道稳定
	ADC2->CR1 |= 0x01 ;  //启动AD转换

}
/***********************************************************************************
Function Name: 	hardwareinit_interrupt
Description:	中断初始化设置  	TIM2产生1ms中断
Parameters: none
Return value: none
***********************************************************************************/
void config_time_interrupt(void)
{
 	TIM2->IER=0x01;	  //允许更新中断
	TIM2->EGR=0x01;	  //产生新事件,重新初始化计数器
	
	TIM2->CNTRH=0x03;  //设置计数器寄存器(十进制为1000)
	TIM2->CNTRL=0xe7;
		
	TIM2->ARRH=0x03;   //设置自动装载寄存器
	TIM2->ARRL=0xe7;
		
	TIM2->PSCR=0x04;   //设置预分频为16M的输入晶振分频成1M
	//TIM2->PSCR=0x03;   //设置预分频为8M的输入晶振分频成1M
  	TIM2->SR1=0;     //清除中断标志
  	TIM2->CR1=0x01;	  //开启定时器2
	ITC->ISPR4=0xf7; //优先级为1级 数字越大优先级越高
	ENABLE_INTERRUPT;	  //开启全局中断

}

/*****************************************************
@Fn: InitialFlashReg() 
@Br: 初始化闪存寄存器组 
@Pa: 无
@Rt: 无
@Sp: 无
*****************************************************/ 
void config_flash( void )
{
  FLASH->CR1 = FLASH_CR1_RESET_VALUE;
  FLASH->CR2 = FLASH_CR2_RESET_VALUE;
  FLASH->NCR2 = FLASH_NCR2_RESET_VALUE;
  //FLASH->IAPSR &= ( U08 )(~( 1 << DUL ));    /* 清除只读DATA区解锁  写保护使能 */
 // FLASH->IAPSR &= ( U08 )(~( 1 << PUL ));    /* 清除程序区解锁 主存储区域写保护使能*/
  FLASH->IAPSR=FLASH_IAPSR_RESET_VALUE;

}

/***********************************************************************************
Function Name: 	config_wdg
Description:	看门狗初始化设置
Parameters: none
Return value: none
***********************************************************************************/
void config_wdg(void)
{
	IWDG->KR = 0xcc;	  /* 启动看门狗 */
	IWDG->KR = 0x55;	  /* 写入修改值使能键值 */ 
	IWDG->RLR = 0xFF;		
	IWDG->PR = 0x06;	  /* 510ms时长 */ 
	IWDG->KR = 0xaa;   /* 写入后恢复到保护状态 */  
	//IWDG_KR = 0xcc;	  /* 启动看门狗 */ 

}
/***********************************************************************************
Function Name: 	config_beep
Description:	蜂鸣器初始化2.37K的频率输出
Parameters: none
Return value: none
***********************************************************************************/
void config_beep( void )
{ 
    //PD4第二功能脚不是beep功能，就强制写成beep  
    if((FLASH_ReadOptionByteIIC(0x4803) & 0x807F)!=0x807f)  
    {  
     UnlockFlash(UNLOCK_EEPROM_TYPE);
        FLASH_ProgramOptionByteIIC(0x4803,0x80);//将PD4的第二功能脚写成beep  
       // WWDG_CR = 0x80;//复位stm8  
    LockFlash(UNLOCK_EEPROM_TYPE);  
    }  
  //2.46K
  // BEEP_CSR=0x0b;   /* 分频因数为13 */  
  // BEEP_CSR |= 0x40;   /* 分频频率选择为4 */  
   //2.37K
	//BEEP_CSR=0x19;	 /* 分频因数为27 */  
	//BEEP_CSR |= 0x80;	/* 分频频率选择为2 */  
   
  BEEP->CSR=0x19;   /* 分频因数为27 */  
  BEEP->CSR |= 0x80;   /* 分频频率选择为2 */  
  BEEP->CSR &= 0xdf;   /*关闭蜂鸣器*/
} 

/*************************************************
函数：Init_UART1
功能：初始化UART1寄存器及对应的I/O引脚
		下载口
参数：无
返回值：无
****************************************************/
void config_uart1(void)
{
	UART1->SR = 0x00;		/* 发送完成，接收完成清零 */
  	UART1->CR1 = 0x00;  		//uart使能  8个数据位 无校验
//  	UART1->CR2 = 0x6c;		/* 发送完成中断使能，接收中断使能，接收使能 */	 
  	UART1->CR3 = 0x00;  		//2个停止位
	// 设置波特率，必须注意以下几点：
	//    (1) 必须先写BRR2
	//    (2) BRR1存放的是分频系数的第11位到第4位，
	//    (3) BRR2存放的是分频系数的第15位到第12位，和第3位
	//        到第0位
	// 例如对于波特率位9600时，分频系数=8000000/9600=833
	// 对应的十六进制数为0x0341，BBR1=34,BBR2=01
  	UART1->BRR2 = 0x01;
  	UART1->BRR1 = 0x34;
  	//UART1->BRR2 = 0x03;
  	//UART1->BRR1 = 0x68;

	 
	// 开发送使能后，延时10ms，清除发送完成标志，避免产生首次使能导致的发送中断
	UART1->CR2 = 0x0c;	   	// 发送使能，接收使能
	SYS_DelayMS(10);		// 延时等待10ms(待空闲帧发送完成)
	UART1->SR &= ~0x40; 	// 清发送完成标志位TC 	
	UART1->CR2 |= 0x60;	   	// 发送完成中断使能,接收中断使能

}
/****************************************************
函数：Init_UART3
功能：初始化UART3寄存器及对应的I/O引脚
参数：无
返回值：无
******************************************************/
void config_uart3(void)
{
	UART3->SR = 0x00;		/* 发送完成，接收完成清零 */
  	UART3->CR1 = 0x00;  //uart使能  8个数据位 无校验
//  	UART3->CR2 = 0x6c;	/* 发送完成中断使能，接收中断使能，接收使能 */	 
  	UART3->CR3 = 0x00;  //一个停止位
	// 设置波特率，必须注意以下几点：
	//    (1) 必须先写BRR2
	//    (2) BRR1存放的是分频系数的第11位到第4位，
	//    (3) BRR2存放的是分频系数的第15位到第12位，和第3位
	//        到第0位
	// 例如对于波特率位31250时，分频系数=8000000/31250=256
	// 对应的十六进制数为0x0100，BBR1=10,BBR2=00
  	UART3->BRR2 = 0x01;
  	UART3->BRR1 = 0x34;
  	//UART3->BRR2 = 0x03;
  	//UART3->BRR1 = 0x68;

	 
	// 开发送使能后，延时10ms，清除发送完成标志，避免产生首次使能导致的发送中断
	UART3->CR2 = 0x0c;	   	// 发送使能，接收使能 */ 
	SYS_DelayMS(10);		// 延时10ms
	UART3->SR &= ~0x40; 	// 清发送完成标志位TC	
	UART3->CR2 |= 0x60;	   	// 发送完成中断使能,接收中断使能

}

/***********************************************************************************
Function Name: 	config
Description:	系统初始化程序
Parameters: none
Return value: none
***********************************************************************************/
void config(void)
{
	DISABLE_INTERRUPT;	  		//禁止中断
	config_clock();   			//时钟初始化设置
	config_pin();       		//IO引脚初始化设置
	config_ad();        		//AD初始化设置
	config_time_interrupt(); 	//1ms中断初始化设置
	config_flash();        		//存储器初始化设置

	config_wdg(); 	  			//看门狗初始化设置	 

	config_uart1();				//初始化串口1通讯
	config_uart3();     		//初始化串口3通讯
	ENABLE_INTERRUPT;			//开中断
}
#endif	
