#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "queue.h"
#include <stdio.h>
#include "IOdef.h"
#include "Cli.h"
#include "Init.h"
#include "string.h"

char command[10] = ""; 											// Buffer to store received command
QueueHandle_t xMoveElevator;

int sendbyte(uint8_t b)
{
	TIM2->SR &= ~TIM_SR_UIF;  // Clear update flag first
  TIM2->CR1 |= TIM_CR1_CEN; //start timer
	TIM2->CNT = 0; 						// counter reset

	/*Check for timeout if timeout occurs return 1*/
  while (!(USART2->SR & USART_SR_TXE)) 
		{
			if (TIM2->SR & TIM_SR_UIF) 	 //if Status register and update flag true
				{
					TIM2->SR &= ~TIM_SR_UIF;   	 // Clear the update flag
					TIM2->CR1 &= ~TIM_CR1_CEN;   // Stop the timer
					return 1;  									
        }
    }

  USART2->DR = b & 0xFF; // Load character

  while (!(USART2->SR & USART_SR_TC)) 
		{ 
			//Check Usart interrupt
      // if timeout
      if (TIM2->SR & TIM_SR_UIF) 
				{
          TIM2->SR &= ~TIM_SR_UIF;  // Clear the update flag
          TIM2->CR1 &= ~TIM_CR1_CEN;  // Stop the timer
          return 1;  
         }
      }
    /*Stop timer*/
    TIM2->CR1 &= ~TIM_CR1_CEN;
		TIM2->CNT = 0; // Reset the timer counter

    return 0; // Success
}

 char getbyte(void) 		//Recieve function
{	
	while(!(USART2->SR & USART_SR_RXNE));
	return (char)(USART2->DR);
}

void set_screen(void)			//Code for setting scrollable region and display window
{
	CLI_Transmit(SCROLL_REGION, sizeof(SCROLL_REGION));	//set scroll window
  CLI_Transmit(CLEAR_SCREEN, sizeof(CLEAR_SCREEN));		//clear the screen 
  CLI_Transmit(MOVE_CURSOR_MIDDLE, sizeof(MOVE_CURSOR_MIDDLE));		//move the cursor to the start of scroll window
	CLI_Transmit(BACKGROUND,sizeof(BACKGROUND));	
}

void CLI_Transmit(uint8_t *pData, uint16_t Size)	
{
	for (uint16_t i=0; i < Size; i++)
	{
		sendbyte(pData[i]);
	}
}

void CLI_Receive(uint8_t *pData)		
{
  int commandLength = strlen(command);
//	char* floorMessages[] = {
//		"Moving to ground lobby...\r\n",
//		"Moving to floor 1...\r\n",
//		"Moving to floor 2...\r\n",
//		"Moving to floor 3...\r\n",
//		"Moving to floor 4...\r\n",
//		"Moving to floor 5...\r\n",
//	};
//	uint16_t length = strlen(floorMessages);

	switch (pData[0]) 
			{
        case 0x08:
        case 0x7f:
            if (commandLength > 0)
							{
                command[--commandLength] = '\0';
              }
            break;

        case 0x0D:
            if (strcmp(command, "p1") == 0) 
							{
								const char* DisplayMessage = "Moving to Parking...\r\n";
								uint16_t length = (uint16_t)strlen(DisplayMessage);
								uint8_t* message = (uint8_t*)DisplayMessage;	
								CLI_Transmit(message, length);
								int Floor = -1;
								xQueueSendToFrontFromISR(xMoveElevator, &Floor, NULL);
							}
							else if (strcmp(command, "g") == 0) 
							{
								const char* DisplayMessage = "Moving to Lobby...\r\n";
								uint16_t length = (uint16_t)strlen(DisplayMessage);
								uint8_t* message = (uint8_t*)DisplayMessage;	
								CLI_Transmit(message, length);
                int Floor = 0;
                xQueueSendToFrontFromISR(xMoveElevator, &Floor, NULL);
							}
            else if (strcmp(command, "1") == 0) 
							{
								const char* DisplayMessage = "Moving to floor 1...\r\n";
								uint16_t length = (uint16_t)strlen(DisplayMessage);
								uint8_t* message = (uint8_t*)DisplayMessage;	
								CLI_Transmit(message, length);
                int Floor = 1;
                xQueueSendToFrontFromISR(xMoveElevator, &Floor, NULL);
							}
							memset(command, 0, sizeof(command));
							break;

        default:
            if (commandLength < sizeof(command) - 1) 
							{
                command[commandLength++] = pData[0];
                command[commandLength] = '\0';
							}
							break;
			}
}

