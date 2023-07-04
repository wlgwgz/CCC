#define CONFIG_GLOBAL
#include "includes.h"
/********************************************************************************************************
*
* File                : GW532A.c
* Hardware Environment: 
* Build Environment   : ST Visual Develop 4.1.6
* Version             : V1.0
* By                  : ��־��
* data                : 2012-03-22
* ֻ����һ�ε�Ӳ���ײ������ļ���ȫ��Ӧ�ò���롣                             
*
*********************************************************************************************************/

//u32 LSIMeasurment(void);
#if 0
/*******************************************************************************
 �� ��: SYS_DelayUS()
 �� ��: ��ʱ΢��
 �� ��: usec--΢��; 
 �� ��: ��
 �� ��: �ú�����ʱ����ȷ�����ö�ʱ����ʵ�־�ȷ��ʱ��
 		�������ж�������ָ��ʱ�䣬����Щ���(Ҳû��ϵ)�����豣֤��ʱʱ���ǹ���
*******************************************************************************/
void SYS_DelayUS(U16 usec)	 
{ 
	while (usec-- != 0)
	{
		//CPU����Ϊ16M
		ASM_NOP;ASM_NOP;ASM_NOP;ASM_NOP;ASM_NOP;
		ASM_NOP;ASM_NOP;ASM_NOP;ASM_NOP;ASM_NOP;		
		ASM_NOP;ASM_NOP;ASM_NOP;ASM_NOP;ASM_NOP;	

//		ASM_NOP;ASM_NOP;ASM_NOP;ASM_NOP;ASM_NOP;
//		ASM_NOP;ASM_NOP;ASM_NOP;ASM_NOP;ASM_NOP;		
//		ASM_NOP;ASM_NOP;ASM_NOP;ASM_NOP;ASM_NOP;		
	}
}

/*******************************************************************************
 �� ��: SYS_DelayMS()
 �� ��: ��ʱ����
 �� ��: msec--����; 
 �� ��: ��
 �� ��: ��
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
Description:	ʱ�ӳ�ʼ������
Parameters: none
Return value: none
***********************************************************************************/
void config_clock(void)
{

	//CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV2);
	//�����ڲ����پ�����2��Ƶ16MHz/2=8MHz
	CLK->SWR = 0xe1;     				//��ʱ���л��Ĵ��� ѡ��HSIΪ��ʱ��
	//CLK->CKDIVR=0x08;   				//2��Ƶ
	CLK->CKDIVR = 0x00;   				//1��Ƶ
	CLK->SWCR |= 0x02;      			//�����л�
	while((CLK->SWCR & 0x01)==0x01);  	//�ȴ������ȶ�
	CLK->SWCR &=(~0x02);      			//�ر��л�

/*
	//�����ⲿ���񲻷�Ƶ
	CLK->ECKR |=0X1; //�����ⲿʱ�� 
	while(!(CLK->ECKR&0X2)); //�ȴ��ⲿʱ��rdy 
	CLK->CKDIVR &= 0XF8; //CPU�޷�Ƶ 
	//CLK->CKDIVR=0x01;   //�ⲿ2��Ƶ
	CLK->SWR = 0XB4; //ѡ���ⲿʱ�� 
	while(!(CLK->SWCR&0X8)); //����Ҫ�� 
	CLK->SWCR |=0X2; //ʹ���ⲿʱ�� 
*/	
}
/***********************************************************************************
Function Name: 	config_pin
Description:	IO���ų�ʼ������,NTC�͵�����hardwareinit_AD(void)�д���
Parameters: none
Return value: none
***********************************************************************************/
void config_pin(void)
{
    //A��
	GPIOA->DDR = 0x68;		//TXD��R-/Tͨ��״̬���ƣ����,RXD\OSCIN\OSCOUT����
  	GPIOA->CR1 = 0x68;		//TXD��R-/T,STEP8�������,������������
  	GPIOA->CR2 = 0x68;		//���Ƶ�����Ϊ10MHZ
  	
    //B��
  	GPIOB->DDR = 0xF8;    	//STEP1\OUT2\OUT1\OUT0\OUT9���,S2\S1\CUR2����
  	GPIOB->CR1 = 0xF8;    	//�������
  	GPIOB->CR2 = 0xF8;     	//���Ƶ�����Ϊ10MHZ

    //C��
	GPIOC->DDR = 0xFE;    	//OUT1~OUT7���
  	GPIOC->CR1 = 0xFE;    	//�������
  	GPIOC->CR2 = 0xFE;	   	//���Ƶ�����Ϊ10MHZ

    //D��
	GPIOD->DDR = 0x38;		//PRO_TXD\LED1\PRO_R/T���
	GPIOD->CR1 = 0x38;	  	//������� 
	GPIOD->CR2 = 0x38;   

	//E��
	GPIOE->DDR = 0x20;		//OUT8�����
	GPIOE->CR1 = 0x20;	  	//�������
	GPIOE->CR2 = 0x20;	  	//���Ƶ�����10MH 

    //F��
	GPIOF->DDR = 0xFF;		//���
	GPIOF->CR1 = 0xFF;	  	//�������
	GPIOF->CR2 = 0xFF;	  	//���Ƶ�����10MH 

    //G��
  	GPIOG->DDR = 0x7C;    	//
	GPIOG->CR1 = 0x7C;    	//�������
  	GPIOG->CR2 = 0x7C;    	//���Ƶ�����Ϊ10MHZ 

  	//I��
  	GPIOI->DDR = 0x01;    	//
	GPIOI->CR1 = 0x01;    	//�������
  	GPIOI->CR2 = 0x01;    	//���Ƶ�����Ϊ10MHZ 
 }
/***********************************************************************************
Function Name: 	config_ad
Description:	AD��ʼ������
Parameters: none
Return value: none
***********************************************************************************/
void config_ad(void)
{

	U08 i,j;

    ADC2->TDRL |= 0x07;  //��ֹAIN0~AIN2  ʩ���ش�����
	ADC2->TDRH |= 0x03;  	//��ֹAIN8~AIN9  ʩ���ش�����
   	ADC2->CSR = 0x00 ;   	//ѡ��AIN0
   	ADC2->CR2 = 0x08 ;   	//�Ҷ���
   	ADC2->CR1 = 0x71 ;    	//����ת��
   	
	for(i=0;i<250;i++);//�ȴ�ͨ���ȶ�
	ADC2->CR1 |= 0x01 ;  //����ADת��

}
/***********************************************************************************
Function Name: 	hardwareinit_interrupt
Description:	�жϳ�ʼ������  	TIM2����1ms�ж�
Parameters: none
Return value: none
***********************************************************************************/
void config_time_interrupt(void)
{
 	TIM2->IER=0x01;	  //��������ж�
	TIM2->EGR=0x01;	  //�������¼�,���³�ʼ��������
	
	TIM2->CNTRH=0x03;  //���ü������Ĵ���(ʮ����Ϊ1000)
	TIM2->CNTRL=0xe7;
		
	TIM2->ARRH=0x03;   //�����Զ�װ�ؼĴ���
	TIM2->ARRL=0xe7;
		
	TIM2->PSCR=0x04;   //����Ԥ��ƵΪ16M�����뾧���Ƶ��1M
	//TIM2->PSCR=0x03;   //����Ԥ��ƵΪ8M�����뾧���Ƶ��1M
  	TIM2->SR1=0;     //����жϱ�־
  	TIM2->CR1=0x01;	  //������ʱ��2
	ITC->ISPR4=0xf7; //���ȼ�Ϊ1�� ����Խ�����ȼ�Խ��
	ENABLE_INTERRUPT;	  //����ȫ���ж�

}

/*****************************************************
@Fn: InitialFlashReg() 
@Br: ��ʼ������Ĵ����� 
@Pa: ��
@Rt: ��
@Sp: ��
*****************************************************/ 
void config_flash( void )
{
  FLASH->CR1 = FLASH_CR1_RESET_VALUE;
  FLASH->CR2 = FLASH_CR2_RESET_VALUE;
  FLASH->NCR2 = FLASH_NCR2_RESET_VALUE;
  //FLASH->IAPSR &= ( U08 )(~( 1 << DUL ));    /* ���ֻ��DATA������  д����ʹ�� */
 // FLASH->IAPSR &= ( U08 )(~( 1 << PUL ));    /* ������������� ���洢����д����ʹ��*/
  FLASH->IAPSR=FLASH_IAPSR_RESET_VALUE;

}

/***********************************************************************************
Function Name: 	config_wdg
Description:	���Ź���ʼ������
Parameters: none
Return value: none
***********************************************************************************/
void config_wdg(void)
{
	IWDG->KR = 0xcc;	  /* �������Ź� */
	IWDG->KR = 0x55;	  /* д���޸�ֵʹ�ܼ�ֵ */ 
	IWDG->RLR = 0xFF;		
	IWDG->PR = 0x06;	  /* 510msʱ�� */ 
	IWDG->KR = 0xaa;   /* д���ָ�������״̬ */  
	//IWDG_KR = 0xcc;	  /* �������Ź� */ 

}
/***********************************************************************************
Function Name: 	config_beep
Description:	��������ʼ��2.37K��Ƶ�����
Parameters: none
Return value: none
***********************************************************************************/
void config_beep( void )
{ 
    //PD4�ڶ����ܽŲ���beep���ܣ���ǿ��д��beep  
    if((FLASH_ReadOptionByteIIC(0x4803) & 0x807F)!=0x807f)  
    {  
     UnlockFlash(UNLOCK_EEPROM_TYPE);
        FLASH_ProgramOptionByteIIC(0x4803,0x80);//��PD4�ĵڶ����ܽ�д��beep  
       // WWDG_CR = 0x80;//��λstm8  
    LockFlash(UNLOCK_EEPROM_TYPE);  
    }  
  //2.46K
  // BEEP_CSR=0x0b;   /* ��Ƶ����Ϊ13 */  
  // BEEP_CSR |= 0x40;   /* ��ƵƵ��ѡ��Ϊ4 */  
   //2.37K
	//BEEP_CSR=0x19;	 /* ��Ƶ����Ϊ27 */  
	//BEEP_CSR |= 0x80;	/* ��ƵƵ��ѡ��Ϊ2 */  
   
  BEEP->CSR=0x19;   /* ��Ƶ����Ϊ27 */  
  BEEP->CSR |= 0x80;   /* ��ƵƵ��ѡ��Ϊ2 */  
  BEEP->CSR &= 0xdf;   /*�رշ�����*/
} 

/*************************************************
������Init_UART1
���ܣ���ʼ��UART1�Ĵ�������Ӧ��I/O����
		���ؿ�
��������
����ֵ����
****************************************************/
void config_uart1(void)
{
	UART1->SR = 0x00;		/* ������ɣ������������ */
  	UART1->CR1 = 0x00;  		//uartʹ��  8������λ ��У��
//  	UART1->CR2 = 0x6c;		/* ��������ж�ʹ�ܣ������ж�ʹ�ܣ�����ʹ�� */	 
  	UART1->CR3 = 0x00;  		//2��ֹͣλ
	// ���ò����ʣ�����ע�����¼��㣺
	//    (1) ������дBRR2
	//    (2) BRR1��ŵ��Ƿ�Ƶϵ���ĵ�11λ����4λ��
	//    (3) BRR2��ŵ��Ƿ�Ƶϵ���ĵ�15λ����12λ���͵�3λ
	//        ����0λ
	// ������ڲ�����λ9600ʱ����Ƶϵ��=8000000/9600=833
	// ��Ӧ��ʮ��������Ϊ0x0341��BBR1=34,BBR2=01
  	UART1->BRR2 = 0x01;
  	UART1->BRR1 = 0x34;
  	//UART1->BRR2 = 0x03;
  	//UART1->BRR1 = 0x68;

	 
	// ������ʹ�ܺ���ʱ10ms�����������ɱ�־����������״�ʹ�ܵ��µķ����ж�
	UART1->CR2 = 0x0c;	   	// ����ʹ�ܣ�����ʹ��
	SYS_DelayMS(10);		// ��ʱ�ȴ�10ms(������֡�������)
	UART1->SR &= ~0x40; 	// �巢����ɱ�־λTC 	
	UART1->CR2 |= 0x60;	   	// ��������ж�ʹ��,�����ж�ʹ��

}
/****************************************************
������Init_UART3
���ܣ���ʼ��UART3�Ĵ�������Ӧ��I/O����
��������
����ֵ����
******************************************************/
void config_uart3(void)
{
	UART3->SR = 0x00;		/* ������ɣ������������ */
  	UART3->CR1 = 0x00;  //uartʹ��  8������λ ��У��
//  	UART3->CR2 = 0x6c;	/* ��������ж�ʹ�ܣ������ж�ʹ�ܣ�����ʹ�� */	 
  	UART3->CR3 = 0x00;  //һ��ֹͣλ
	// ���ò����ʣ�����ע�����¼��㣺
	//    (1) ������дBRR2
	//    (2) BRR1��ŵ��Ƿ�Ƶϵ���ĵ�11λ����4λ��
	//    (3) BRR2��ŵ��Ƿ�Ƶϵ���ĵ�15λ����12λ���͵�3λ
	//        ����0λ
	// ������ڲ�����λ31250ʱ����Ƶϵ��=8000000/31250=256
	// ��Ӧ��ʮ��������Ϊ0x0100��BBR1=10,BBR2=00
  	UART3->BRR2 = 0x01;
  	UART3->BRR1 = 0x34;
  	//UART3->BRR2 = 0x03;
  	//UART3->BRR1 = 0x68;

	 
	// ������ʹ�ܺ���ʱ10ms�����������ɱ�־����������״�ʹ�ܵ��µķ����ж�
	UART3->CR2 = 0x0c;	   	// ����ʹ�ܣ�����ʹ�� */ 
	SYS_DelayMS(10);		// ��ʱ10ms
	UART3->SR &= ~0x40; 	// �巢����ɱ�־λTC	
	UART3->CR2 |= 0x60;	   	// ��������ж�ʹ��,�����ж�ʹ��

}

/***********************************************************************************
Function Name: 	config
Description:	ϵͳ��ʼ������
Parameters: none
Return value: none
***********************************************************************************/
void config(void)
{
	DISABLE_INTERRUPT;	  		//��ֹ�ж�
	config_clock();   			//ʱ�ӳ�ʼ������
	config_pin();       		//IO���ų�ʼ������
	config_ad();        		//AD��ʼ������
	config_time_interrupt(); 	//1ms�жϳ�ʼ������
	config_flash();        		//�洢����ʼ������

	config_wdg(); 	  			//���Ź���ʼ������	 

	config_uart1();				//��ʼ������1ͨѶ
	config_uart3();     		//��ʼ������3ͨѶ
	ENABLE_INTERRUPT;			//���ж�
}
#endif	
