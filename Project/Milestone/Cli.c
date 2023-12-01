#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "queue.h"
#include <stdio.h>
#include "IOdef.h"
#include "Cli.h"
#include "Init.h"
#include "string.h"

//char command[10] = ""; 			<--- removed from global added as a typedef in Cli.h
QueueHandle_t xMoveElevator;

int sendbyte(uint8_t b)
{
	TIM2->SR &= ~TIM_SR_UIF;	// Clear update flag first
	TIM2->CR1 |= TIM_CR1_CEN; //start timer
	TIM2->CNT = 0; 						// counter reset

	/*Check for timeout if timeout occurs return 1*/
	while (!(USART2->SR & USART_SR_TXE))
		{
			if (TIM2->SR & TIM_SR_UIF) 	 //if Status register and update flag true
				{
					TIM2->SR &= ~TIM_SR_UIF;	 	 // Clear the update flag
					TIM2->CR1 &= ~TIM_CR1_CEN;	 // Stop the timer
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
					TIM2->SR &= ~TIM_SR_UIF;		// Clear the update flag
					TIM2->CR1 &= ~TIM_CR1_CEN;	// Stop the timer
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

//void set_screen(void)			//Code for setting scrollable region and display window
//{
//	CLI_Transmit(SCROLL_REGION, sizeof(SCROLL_REGION));	//set scroll window
//	CLI_Transmit(CLEAR_SCREEN, sizeof(CLEAR_SCREEN));		//clear the screen
//	CLI_Transmit(MOVE_CURSOR_MIDDLE, sizeof(MOVE_CURSOR_MIDDLE));		//move the cursor to the start of scroll window
//}

void CLI_Transmit(uint8_t *pData, uint16_t Size)
{
	for (uint16_t i=0; i < Size; i++)
	{
		sendbyte(pData[i]);
	}
}

void CLI_Receive(CommandData *cmdData, uint8_t *pData)
{
	int commandLength = strlen(cmdData->command);

	switch (pData[0])
			{
				case 0x08:
				case 0x7f:
						if (commandLength > 0)
							{
								cmdData->command[--commandLength] = '\0';
							}
						break;

				case 0x0D:
						if (strcmp(cmdData->command, "p1") == 0)
							{
								int Floor = -1;
								const char* DisplayMessage = "\x1b[91mMoving to Parking...\r\n";
								uint16_t length = (uint16_t)strlen(DisplayMessage);
								uint8_t* message = (uint8_t*)DisplayMessage;
								CLI_Transmit(message, length);
								xQueueSendToFrontFromISR(xMoveElevator, &Floor, NULL);
							}
							else if (strcmp(cmdData->command, "0") == 0)
							{
								int Floor = 0;
								const char* DisplayMessage = "\x1b[92mMoving to Lobby...\r\n";
								uint16_t length = (uint16_t)strlen(DisplayMessage);
								uint8_t* message = (uint8_t*)DisplayMessage;
								CLI_Transmit(message, length);
								xQueueSendToFrontFromISR(xMoveElevator, &Floor, NULL);
							}
						else if (strcmp(cmdData->command, "1") == 0)
							{
								int Floor = 1;
								const char* DisplayMessage = "\x1b[93mMoving to floor 1...\r\n";
								uint16_t length = (uint16_t)strlen(DisplayMessage);
								uint8_t* message = (uint8_t*)DisplayMessage;
								CLI_Transmit(message, length);
								xQueueSendToFrontFromISR(xMoveElevator, &Floor, NULL);
							}
							else if (strcmp(cmdData->command, "2") == 0)
							{
								int Floor = 2;
								const char* DisplayMessage = "\x1b[94mMoving to floor 2...\r\n";
								uint16_t length = (uint16_t)strlen(DisplayMessage);
								uint8_t* message = (uint8_t*)DisplayMessage;
								CLI_Transmit(message, length);
								xQueueSendToFrontFromISR(xMoveElevator, &Floor, NULL);
							}
							else if (strcmp(cmdData->command, "3") == 0)
							{
								int Floor = 3;
								const char* DisplayMessage = "\x1b[95mMoving to floor 3...\r\n";
								uint16_t length = (uint16_t)strlen(DisplayMessage);
								uint8_t* message = (uint8_t*)DisplayMessage;
								CLI_Transmit(message, length);
								xQueueSendToFrontFromISR(xMoveElevator, &Floor, NULL);
							}
							else if (strcmp(cmdData->command, "4") == 0)
							{
								int Floor = 4;
								const char* DisplayMessage = "\x1b[96mMoving to floor 4...\r\n";
								uint16_t length = (uint16_t)strlen(DisplayMessage);
								uint8_t* message = (uint8_t*)DisplayMessage;
								CLI_Transmit(message, length);
								xQueueSendToFrontFromISR(xMoveElevator, &Floor, NULL);
							}
							else if (strcmp(cmdData->command, "5") == 0)
							{
								int Floor = 5;
								const char* DisplayMessage = "\x1b[97mMoving to Penthouse...\r\n";
								uint16_t length = (uint16_t)strlen(DisplayMessage);
								uint8_t* message = (uint8_t*)DisplayMessage;
								CLI_Transmit(message, length);
								xQueueSendToFrontFromISR(xMoveElevator, &Floor, NULL);
							}
							memset(cmdData->command, 0, sizeof(cmdData->command));
							break;

				default:
						if (commandLength < sizeof(cmdData->command) - 1)
							{
								cmdData->command[commandLength++] = pData[0];
								cmdData->command[commandLength] = '\0';
							}
							break;
			}
}

void UpdateStatus(int currentFloor)
	{
	 CLI_Transmit(SCROLL_REGION, sizeof(SCROLL_REGION));			// Set the scroll window
	 CLI_Transmit(CLEAR_SCREEN, sizeof(CLEAR_SCREEN));				// Clear the screen
	 CLI_Transmit(MOVE_CURSOR_TOP, sizeof(MOVE_CURSOR_TOP));	// Move cursor to the status window

	 // Construct and transmit the status message
	 const char* statusWin = "\r\nFloor: ";
	 char buffer[50];	// Adjust the size based on your needs
	 snprintf(buffer, sizeof(buffer), "%s%d\r\n", statusWin, currentFloor);
	 uint16_t statusWinLength = strlen(buffer);
	 uint8_t* dataSt = (uint8_t*)buffer;
	 CLI_Transmit(dataSt, statusWinLength);

	 // Perform scroll, restore cursor position, and move cursor to the message area
	 CLI_Transmit(SCROLL_REGION, sizeof(SCROLL_REGION));
	 CLI_Transmit(RESTORE_CURSOR, sizeof(RESTORE_CURSOR));
	 CLI_Transmit(MOVE_CURSOR_MIDDLE, sizeof(MOVE_CURSOR_MIDDLE));
}

void InitializeCLI(void)
	{
		CLI_Transmit(SCROLL_REGION, sizeof(SCROLL_REGION));			// Set the scroll window
		CLI_Transmit(CLEAR_SCREEN, sizeof(CLEAR_SCREEN));				// Clear the screen
		CLI_Transmit(MOVE_CURSOR_TOP, sizeof(MOVE_CURSOR_TOP));	// Move cursor to the status window

		const char *statusWin = "\r\nFloor: 1\r\n";
		uint16_t statusWinLength = strlen(statusWin);
		uint8_t *dataSt = (uint8_t *)statusWin;
		CLI_Transmit(dataSt, statusWinLength);

		CLI_Transmit(SCROLL_REGION, sizeof(SCROLL_REGION));	// Scroll
		CLI_Transmit(RESTORE_CURSOR, sizeof(RESTORE_CURSOR)); // Move cursor back to the message
		CLI_Transmit(MOVE_CURSOR_MIDDLE, sizeof(MOVE_CURSOR_MIDDLE));	// Move cursor to the message window
}

void ProcessReceivedChar(CommandData *cmdData, uint8_t charReceived)
	{
		if (charReceived == 0x08 || charReceived == 0x7f) {
				charReceived = 0x7f;	// Ensure delete is sent
		}

		sendbyte(charReceived);				// Send the received character
		CLI_Receive(cmdData,&charReceived);		// Append to the overall message/check message value
}



