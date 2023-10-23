#include "stm32f10x.h"
#include "usart.h"
#include "IOdef.h"

volatile int Flag = 0;
volatile char RecievedChar = 0;

void delay(uint32_t delay_time) 
{
	for (volatile uint32_t i = 0; i < delay_time * 8000; i++)
		{ 
        // Delay loop
    }
}



void serial_open(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN | RCC_APB1ENR_TIM2EN;  //Usart2 clock enabled and timer2 enabled
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;		// port A clock enabled and alternate function input output
	
	GPIOA->CRL |= GPIO_CRL_MODE2_0 | GPIO_CRL_MODE2_1; // PA2 as alternate function (TX)
  GPIOA->CRL &= ~GPIO_CRL_CNF2_0; 									 // Clear CNF2 bits to configure as output
  GPIOA->CRL |= GPIO_CRL_CNF2_1;  									 // Set CNF2 bits for alternate function push-pull
	
  GPIOA->CRL &= ~GPIO_CRL_MODE3_0 & ~GPIO_CRL_MODE3_1; // PA3 as input (RX)
  GPIOA->CRL &= ~GPIO_CRL_CNF3_0; 										 // Clear CNF3 bits to configure as input
  GPIOA->CRL |= GPIO_CRL_CNF3_1;  										 // Set CNF3 bits for pull-up/pull-down input
	
	GPIOA->CRL |= GPIO_CRL_MODE5_0 | GPIO_CRL_MODE5_1;	//Configure PA5 for led
  GPIOA->CRL &= ~GPIO_CRL_CNF5_0 & ~GPIO_CRL_CNF5_1;

	
	USART2->CR2 &= ~USART_CR2_STOP;		//1 stop bit
	USART2->CR1 &= ~USART_CR1_M;			// 8 data bits
	USART2->CR1 &= ~USART_CR1_PCE;		// no parity
	USART2->BRR = 0x0138;							// baud rate
	USART2->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE; //Enable USART
	USART2->CR1 |= USART_CR1_RXNEIE;	//enable recieve but not empty interrupt
	
	//Timer2 enables for lab3
	TIM2->PSC = 35999;								//set presecaler to 35999 assuming 36khz clock
	TIM2->ARR = 100;								//set reload register to ten thousand
	TIM2->CR1 |= TIM_CR1_CEN;					//ENABLE timer 2
	
	NVIC_EnableIRQ(USART2_IRQn);			//Given in class
	NVIC_SetPriority(USART2_IRQn, 0);	//Assign high priority to interrupt

	__enable_irq();

}

void serial_close(void) 
{
  USART2->CR1 &= ~(USART_CR1_UE | USART_CR1_RE | USART_CR1_TE);   										// Disable USART2    
	GPIOA->CRL &= ~(GPIO_CRL_MODE2 | GPIO_CRL_CNF2 | GPIO_CRL_MODE3 | GPIO_CRL_CNF3);   // Reset GPIOA pins
	RCC->APB1ENR &= ~RCC_APB1ENR_USART2EN;																						  //Disable usart2 clock
}


void toggle_Led_ON(void)	//Function to turn LED on
{
	GPIOA->BSRR = GPIO_BSRR_BS5;
}

void toggle_Led_OFF(void)	//FunctionalState to turn LED off
{
	GPIOA->BSRR = GPIO_BSRR_BR5;
}


int sendbyte(uint8_t b,uint32_t Timeout) 	//Transmit function
{
	uint32_t counter = TIM2->CNT;		//start counter
	
  while ((USART2->SR & USART_SR_TC) == 0)				// Wait for the TXE flag to be set
	{
		if((TIM2->CNT - counter) >= Timeout)
		{
			return -1;						
		}
	}
	
  USART2->DR = b;		    // Write the character to the data register
	// Wait for the transmission to complete by checking the TC (transmission complete) flag
	// while (!(USART2->SR & USART_SR_TC));	
  return 0;  // Return 0 to indicate success
}

 char getbyte(uint32_t Timeout) 		//Recieve function
{	
	uint32_t counter = TIM2->CNT;		//start counter
	while(!Flag)
	{
		if((TIM2->CNT - counter) >= Timeout)
		{
			return 2;						//ERROR code 
		}
	}
	Flag = 0;
	return RecievedChar;
}

void USART2_IRQHandler(void)
{
	if((USART2->SR & USART_SR_RXNE))
	{
		RecievedChar = (char)(USART2->DR);
		Flag = 1;
	}	
}


