#include "FreeRTOS.h"
#include "stm32f10x.h"
#include "usart.h"
#include "IOdef.h"
#include "queue.h"

//int charflag;
QueueHandle_t CLIQueue;

void delay(uint32_t delay_time) 
{
	for (volatile uint32_t i = 0; i < delay_time * 8000; i++)
		{ 
        // Delay loop
    }
}



void serial_open(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN ;//| RCC_APB1RSTR_USART2RST; //Usart2 clock enabled
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN;		// port A clock enabled
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;													//ENABLE timer2
	
	GPIOA->CRL |= GPIO_CRL_MODE2_0 | GPIO_CRL_MODE2_1; // PA2 as alternate function (TX)
  GPIOA->CRL &= ~GPIO_CRL_CNF2_0; 									 // Clear CNF2 bits to configure as output
  GPIOA->CRL |= GPIO_CRL_CNF2_1;  									 // Set CNF2 bits for alternate function push-pull
	
  GPIOA->CRL &= ~GPIO_CRL_MODE3_0 & ~GPIO_CRL_MODE3_1; // PA3 as input (RX)
  GPIOA->CRL &= ~GPIO_CRL_CNF3_0; 										 // Clear CNF3 bits to configure as input
  GPIOA->CRL |= GPIO_CRL_CNF3_1;  										 // Set CNF3 bits for pull-up/pull-down input
	
	GPIOA->CRL |= GPIO_CRL_MODE5_0 | GPIO_CRL_MODE5_1;	//Configure PA5 for led
  GPIOA->CRL &= ~GPIO_CRL_CNF5_0 & ~GPIO_CRL_CNF5_1;

	TIM2->PSC = 35999;									//Assuming 36mhz clock
	TIM2->ARR = 50000;
	TIM2->DIER |= TIM_DIER_UIE;					//ENABLE update interrupt
	TIM2->CR1 |= TIM_CR1_CEN;						//ENABLE timer
	
	USART2->CR2 &= ~USART_CR2_STOP;		//1 stop bit
	USART2->CR1 &= ~USART_CR1_M;			// 8 data bits
	USART2->CR1 &= ~USART_CR1_PCE;		// no parity
	USART2->BRR = 0x0138;							// baud rate
	
	NVIC_EnableIRQ(USART2_IRQn);
	NVIC_EnableIRQ(TIM2_IRQn);
	
	USART2->CR1 |=  USART_CR1_RXNEIE | USART_CR1_TE | USART_CR1_RE | USART_CR1_UE; //Enable USART
	
	
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


int sendbyte(uint8_t b) 	//Transmit function
{
   TIM2->SR &= ~TIM_SR_UIF;  // Clear the update flag
	
    // Start the Timer
    TIM2->CR1 |= TIM_CR1_CEN;
		TIM2->CNT = 0; // Reset the timer counter

    while (!(USART2->SR & USART_SR_TXE)) 
			{
        // Check for timeout
        if (TIM2->SR & TIM_SR_UIF) 
					{
            TIM2->SR &= ~TIM_SR_UIF;  // Clear the update flag
            TIM2->CR1 &= ~TIM_CR1_CEN;  // Stop the timer
            return 1;  // Timeout error
					}
			}
		USART2->DR = b & 0xFF; // Load the character to be transmitted into the data register

    // Wait for the transmission to complete
    while (!(USART2->SR & USART_SR_TC)) {
        // Check for timeout
        if (TIM2->SR & TIM_SR_UIF) {
            TIM2->SR &= ~TIM_SR_UIF;  // Clear the update flag
            TIM2->CR1 &= ~TIM_CR1_CEN;  // Stop the timer
            return 1;  // Timeout error
        }
    }
    // Stop the Timer
    TIM2->CR1 &= ~TIM_CR1_CEN;
		TIM2->CNT = 0; // Reset the timer counter

    return 0; // Successful transmission
}

 char getbyte(void) 		//Recieve function
{	
	while(!(USART2->SR & USART_SR_RXNE));
	return (char)(USART2->DR);
}


void USART2_IRQHandler(void)
{
	if (USART2->SR & USART_SR_RXNE) 
		{
			
        // Read the received data from USART2_DR register
        uint8_t characterReceived = USART2->DR;
				xQueueSendToFrontFromISR(CLIQueue, &characterReceived, NULL);

        // Clear the RXNE flag (optional, but recommended)
        USART2->SR &= ~USART_SR_RXNE;
		}
}

