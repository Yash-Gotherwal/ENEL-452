#include "stm32f10x.h"
#include "usart.h"
#include "IOdef.h"





void delay(uint32_t delay_time) 
{
	for (volatile uint32_t i = 0; i < delay_time * 8000; i++)
		{ 
        // Delay loop
    }
}


void clockInit(void)
{
  
//* enable HSI and wait for it to be ready
    RCC->CR |= 0x00000001;
    while (((RCC->CR) & 0x00000003) == 0);
			
//* enable HSE with Bypass and wait for it to be ready
    RCC->CR |= 0x00050000;
    while (((RCC->CR) & 0x0007000) == 0);
				
//* enable flash prefetch buffer */
//FLASH->ACR = 0x00000012;
    
////SET HSI as SYSCLK and wait for it to be recognized
    RCC->CFGR = 0x04000000;
    while (((RCC->CFGR) & 0x0000000C) != 0);
			
//SET HSE as SYSCLK and wait for it to be recognized
    //RCC->CFGR = 0x04000001;
    //while (((RCC->CFGR) & 0x00000005) == 0);
			
// Disable PLL. 
		
		RCC->CR &= ~RCC_CR_PLLON;
			
//Change PLL source and set the PLL multiplier
			
//These are the SYSCLK values when using the PLL with HSI/2 as the input. The max value is 64 MHz
		//RCC->CFGR = 0x00000000;// 8MHz
		//RCC->CFGR = 0x00040000;// 12 MHz
		//RCC->CFGR = 0x00080000;// 16 MHz
		//RCC->CFGR = 0x000c0000;// 20 MHz			
		RCC->CFGR = 0x00100000;// 24 MHz **** MAX SPEED for VLDISCOVERY *****
		//RCC->CFGR = 0x00140000;//	28 MHz
		//RCC->CFGR = 0x00180000;// 32 MHz
		//RCC->CFGR = 0x001C0000;// 36 MHz			
		//RCC->CFGR = 0x00200000;// 40 MHz
		//RCC->CFGR = 0x00240000;//	44 MHz
		//RCC->CFGR = 0x00280000;// 48 MHz
		//RCC->CFGR = 0x002C0000;// 52 MHz
		//RCC->CFGR = 0x003C0000;// 64 MHz			

//These are the SYSCLK values when using the PLL with HSE/Bypass as the input. The max value is 72 MHz
		//RCC->CFGR = 0x00010000;// 16 MHz
		//RCC->CFGR = 0x00050000;// 24 MHz **** MAX SPEED for VLDISCOVERY *****
		//RCC->CFGR = 0x00090000;// 32 MHz
		//RCC->CFGR = 0x000d0000;// 40 MHz			
		//RCC->CFGR = 0x00110000;// 48 MHz
		//RCC->CFGR = 0x00150000;//	56 MHz
		//RCC->CFGR = 0x00190000;// 64 MHz
		//RCC->CFGR = 0x001d0000;// 72 MHz			

//ENABLE PLL and wait for it to be ready

    RCC->CR |= RCC_CR_PLLON;
    while (((RCC->CR) & 0x03000000) == 0);
   
// Set PLL as SYSCLK and wait for it to be ready
			
    RCC->CFGR |= 0x00000002;
    while (((RCC->CFGR) & 0x00000008) == 0);
		
}

void serial_open(void)
{

	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPAEN;		// port A clock 
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN | RCC_APB1RSTR_USART2RST; //usart2 enabled
  GPIOA->CRL &= ~(GPIO_CRL_MODE2 | GPIO_CRL_CNF2); //pin2
  // Set PA2 as alternate function output push-pull at 50 MHz
  GPIOA->CRL |= GPIO_CRL_CNF2_1 | GPIO_CRL_MODE2_1; //Pin2
	GPIOA->CRL &= ~(GPIO_CRL_MODE3 | GPIO_CRL_CNF3);	//Pin3
  // Set PA3 as input with pull-up/pull-down
  GPIOA->CRL |= GPIO_CRL_CNF3_0;	//pin3

	USART2->CR1 |= USART_CR1_UE |USART_CR1_TE | USART_CR1_RE;
	USART2->CR2 &= ~USART_CR2_STOP;
	USART2->CR1 |= USART_CR1_M;
	USART2->BRR = 0x138;
	
}

void serial_close(void) {
    // Disable USART2
    USART2->CR1 &= ~(USART_CR1_UE);
    
    // Reset GPIOA pins PA2 and PA3 to their default state
    GPIOA->CRL &= ~(GPIO_CRL_MODE2 | GPIO_CRL_CNF2 | GPIO_CRL_MODE3 | GPIO_CRL_CNF3);

    // Reset RCC settings (optional, depending on your overall application)
    RCC->APB2ENR &= ~(RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPAEN);
    RCC->APB1ENR &= ~(RCC_APB1ENR_USART2EN | RCC_APB1RSTR_USART2RST);
}

void blink(void)
{	
	
	GPIOA->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_MODE5); // led
  GPIOA->CRL |= GPIO_CRL_MODE5_0;  //led
//while(1) 
	//	{
			// Turn the LED on (set PA5 high)
      GPIOA->BSRR = GPIO_BSRR_BS5;

      // Delay for 1 second
      delay(1000);

      // Turn the LED off (set PA5 low)
      GPIOA->BSRR = GPIO_BSRR_BR5;

      // Delay for 1 second
      delay(1000);
    //}
	
}

void sendbyte(uint8_t b) 
{
    // Wait for the TXE (transmit data register empty) flag to be set
    while (!(USART2->SR & USART_SR_TXE));

    // Write the character to the data register
    USART2->DR = (b & 0xFF);

//    // Wait for the transmission to complete by checking the TC (transmission complete) flag
//    while (!(USART2->SR & USART_SR_TC));
//		
//	  // Return 0 to indicate success
//    return 0;
  
}

 uint8_t getbyte(void) 
{
    uint8_t read;
	
	if((USART2->SR & USART_SR_RXNE))
	{
		read = USART2->DR;
	}
	return read;
	
}




//	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPAEN;
//	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
//	USART2->CR1 |= USART_CR1_TE;
//	USART2->CR1 |= USART_CR1_RE;
//	USART2->CR2 = 0x0;
//	USART2->BRR = 0x138;
//	GPIOA->CRL &= ~GPIO_CRL_CM2;
//	GPIOA->CRL &= ~GPIO_CRL_CM3;
//	GPIOA->CRL |= GPIO_CRL_CM2_AP | GPIO_CRL_CM3_DF;
//	USART2->CR1 |= USART_CR1_UE;


