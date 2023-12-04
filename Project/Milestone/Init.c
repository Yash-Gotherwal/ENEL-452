#include "Init.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "stm32f10x.h"
#include "IOdef.h"

QueueHandle_t xCliQueue;
QueueHandle_t xMaintainenceQueue;

void serial_open(void)
{
	/*Enable require ports*/
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN; //PortA and PortC
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;											 //ENABLE AFIO port for pc13
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN; 									 //Usart2 clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;											 //Timer 2 clock
	
	/*Enable Pins as required*/
	GPIOA->CRL |= GPIO_CRL_MODE2_0 | GPIO_CRL_MODE2_1; 			 // PA2 as Output (TX)
	GPIOA->CRL &= ~GPIO_CRL_CNF2_0; 									 			 // Clear CNF2 bits to configure as output
	GPIOA->CRL |= GPIO_CRL_CNF2_1;										 			 // Set CNF2 bits for alternate function push-pull
	
	GPIOA->CRL &= ~GPIO_CRL_MODE3_0 & ~GPIO_CRL_MODE3_1; 		 // PA3 as input (RX)
	GPIOA->CRL &= ~GPIO_CRL_CNF3_0; 										 		 // Clear CNF3 bits to configure as input
	GPIOA->CRL |= GPIO_CRL_CNF3_1;											 		 // Set CNF3 bits for pull-up/pull-down input
	
	/*Setting PC13 as alternate function input output and remaping to EXTI13*/
	AFIO->EXTICR[3]	|= AFIO_EXTICR4_EXTI13_PC;
	
	/*Enable interrupt on line 13 and set falling edge trigger to pc13*/
	EXTI->IMR |= EXTI_IMR_MR13;
	EXTI->FTSR |= EXTI_FTSR_TR13;
	
	NVIC_EnableIRQ(EXTI15_10_IRQn); // Enable EXTI 10 to 15 IRQ in NVIC
	NVIC_SetPriority(EXTI15_10_IRQn, 6);
	
	GPIOA->CRL |= GPIO_CRL_MODE5_0 | GPIO_CRL_MODE5_1;			 //Configure PA5 for led
	GPIOA->CRL &= ~GPIO_CRL_CNF5_0 & ~GPIO_CRL_CNF5_1;
	
	/*Set baud rate*/
	USART2->CR2 &= ~USART_CR2_STOP;													 //1 stop bit
	USART2->CR1 &= ~USART_CR1_M;														 // 8 data bits
	USART2->CR1 &= ~USART_CR1_PCE;									 				 // no parity
	USART2->BRR = 0x0138;																		 // baud rate
	
	/*Enable USART and USART Interrupts*/
	USART2->CR1 |=	USART_CR1_RXNEIE | USART_CR1_TE | USART_CR1_RE | USART_CR1_UE; 	//Enable USART

	/*Enable NVIC for usart and timer*/
	NVIC_EnableIRQ(USART2_IRQn);
	NVIC_EnableIRQ(TIM2_IRQn);
	
	/*Add timer delays and update interrupts*/
	TIM2->PSC = 36000;
	TIM2->ARR = 50000;
	TIM2->DIER |= TIM_DIER_UIE;
	TIM2->CR1 |= TIM_CR1_CEN;
	
}

void USART2_IRQHandler(void) 
{
	uint8_t characterReceived;
	if (USART2->SR & USART_SR_RXNE) 						// IF USART2 received interrupt
		{
			characterReceived = USART2->DR;					// Read the received data from USART2_DR register
			xQueueSendToFrontFromISR(xCliQueue, &characterReceived, NULL); //Send recieved character to front of queue
			USART2->SR &= ~USART_SR_RXNE;						// Clear the RXNE flag
		}
}
void EXTI15_10_IRQHandler(void)
{
	if(EXTI->PR & EXTI_PR_PR13)
		{ 
			// Check if the interrupt is from PC13
      EXTI->PR |= EXTI_PR_PR13; 
			int stopFlag = 1;
			xQueueSendToFrontFromISR(xMaintainenceQueue, &stopFlag, NULL);	
    }
}
