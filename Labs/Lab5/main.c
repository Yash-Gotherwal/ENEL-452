/*

ENEL 452 Lab5
Yash Gotherwal
200420786

This lab implements sending and recieving functions using FreeRtos

The base code was provided by trevor douglas and it was a simple blink led program. 
Certain functions from lab4 and lab3 were added on top of the provided code.

I tried implementing a frequency and cliqueue using the documents and presentation during lab 
For some reason i only am able to implement 1 charachter at a time while the light is blinking

During debug i was able to turn the light on or off once by using "on/off"
command in putty but once enter is pressed the project does not respond to charachters anymore.

changes made to document are as followed:

->Added cli task to implement recieve functions and set its priority higher than blink task
->Added a frequency with function implementation in cli_Recieve i wanted to provide 3 different frequencies. 
Not sure if code is wrong or i am making errors
Pseudocode used for blink functions:
-> Add an int with desired frequency
->send desired frequency to queue when command is passed.
->Retrieve from queue using vclitask

Added a xTaskCreate for cli in main and made two queues similiar to document from presentation
*/

#include <stdio.h>
//#include "stm32F103RB.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "Cli.h"
#include "usart.h"

#define mainBLINKY_TASK_PRIORITY 		(tskIDLE_PRIORITY + 2 )
#define Cli_TASK_PRIORITY						(tskIDLE_PRIORITY + 1 )

static void vBlinkTask( void * parameters);
static void vCliTask (void * parameters);

static int frequency;
extern char commandBuffer[10]; // Declare and initialize a character array to store the input string
extern QueueHandle_t Frequency_queue;
extern QueueHandle_t CLIQueue;


int main(void)
{
	serial_open();
	
	Frequency_queue = xQueueCreate(10, sizeof(int));
	CLIQueue = xQueueCreate(10, sizeof(char));
	
	xTaskCreate(vBlinkTask, "Blinky", configMINIMAL_STACK_SIZE, NULL, mainBLINKY_TASK_PRIORITY, NULL);
	xTaskCreate(vCliTask, "CLI", configMINIMAL_STACK_SIZE, NULL, Cli_TASK_PRIORITY, NULL);
	
//	RCC->APB2ENR |= (1u<<2) | (1u<<4) ;
//	RCC->APB1ENR |= RCC_APB1ENR_USART2EN; //enable USART2 clock
//	GPIOA->CRL &= ~(1u<<22) &~ (1u<<23) &~(1u<<10) &~ (1u<<11);
//	GPIOA->CRL |=  (1u<<20) |  (1u<<21) | (3<<8) | (2<<10);
//	//GPIOA->CRL &= ~(1u<<22) &~ (1u<<23) &~(4<11);
//	
//	//AFIO->MAPR |= AFIO_MAPR_USART2_REMAP;
//	USART2->BRR = (8<<0) | (19<<4);  //hopefully baud 115200
//	USART2->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
//	USART2->CR2 |= USART_CR2_CLKEN;
	
	
	
	/* Start the scheduler. */
	vTaskStartScheduler();
	while(1){}
}


static void vBlinkTask( void * parameters)
{
//	for (;;)
//	{
//	
//		GPIOA->ODR |= (1u<<5);	
//		USART2->DR = 0x60;
//		vTaskDelay(1000);
//		
//		GPIOA->ODR &= ~(1u<<5);
//		
//		vTaskDelay(1500);
//		
//		
//	}
	frequency = 1000;
	for (;;)
	{
		if( uxQueueMessagesWaiting( Frequency_queue ) != 0 )		
		{			
			xQueueReceive(Frequency_queue, &frequency, 1);
		}		
		GPIOA->BSRR |= GPIO_BSRR_BS5;		
		vTaskDelay(pdMS_TO_TICKS(frequency));
		GPIOA->BSRR |= GPIO_BSRR_BR5;		
		vTaskDelay((pdMS_TO_TICKS(frequency)));
	}
}

static void vCliTask (void * parameters)
{
	CLI_Transmit((uint8_t *)SCROLL_REGION, sizeof(SCROLL_REGION));	//set scroll window
    CLI_Transmit((uint8_t *)CLEAR_SCREEN, sizeof(CLEAR_SCREEN));		//clear the screen in case previous messages were printed
    CLI_Transmit((uint8_t *)MOVE_CURSOR_TOP, sizeof(MOVE_CURSOR_TOP));		//move the cursor to the top to set default message
    CLI_Transmit((uint8_t *)MOVE_CURSOR_MIDDLE, sizeof(MOVE_CURSOR_MIDDLE));		//move the cursor to the scroll window in the middle
		
		CLI_Transmit((uint8_t*)"\nEnter command. \n\r", 16);

		
	uint8_t characterReceived;
	for (;;)
	{
		BaseType_t xStatus = xQueueReceive(CLIQueue, &characterReceived, portMAX_DELAY);
		if( xStatus == pdPASS )
		{
			sendbyte(characterReceived);				//send received character
			CLI_Receive(&characterReceived, 10);	//process character
		}		
	}
	
}


