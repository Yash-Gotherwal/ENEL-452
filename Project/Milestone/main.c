/*
											ENEL 452 - Final Project - Elevator
															Yash Gotherwal
																200420786

This code is for the milestone demonstration i plan to make the elevator circuit
with CLI and user interrupt. For my milestone i want to get the cli status window 
working which will show the tasks currently running.

V1:
implements a working CLI Status window where user can enter commands such as g or 1.
For my milestone i needed a working cli status window i was able to add tasks for 
movement. i plan to add 4 more floors and change background or font colour for each floor
for elevator move task



*/

#include "stm32f10x.h"
#include "Init.h"
#include "Cli.h"
#include <stdint.h>

#define Move_ELEVATOR_TASK_PRIORITY	(tskIDLE_PRIORITY + 2)
#define	Cli_TASK_PRIORITY						(tskIDLE_PRIORITY + 1)

static void vMoveElevator(void * parameters);
static void vCLITask(void * parameters);

extern char command[];
extern QueueHandle_t xMoveElevator;
extern QueueHandle_t xCliQueue;

int main(void)
{
	serial_open();
	set_screen();

	xCliQueue = xQueueCreate(1, sizeof(char));
	xMoveElevator = xQueueCreate(1, sizeof(int)); 
	xTaskCreate(vMoveElevator, "MOVE", configMINIMAL_STACK_SIZE, NULL, Move_ELEVATOR_TASK_PRIORITY, NULL);
	xTaskCreate(vCLITask, "CLI", configMINIMAL_STACK_SIZE, NULL, Cli_TASK_PRIORITY, NULL);
	vTaskStartScheduler(); // start the scheduler
	return 0;
}



static void vMoveElevator(void * parameters) 
{
	int currentFloor = 1;
  int targetFloor = 1;
	for (;;) 
	{
		BaseType_t status = xQueueReceive(xMoveElevator, &targetFloor, 100); 
		if (status == pdTRUE) 
			{
       // Check the direction to move (up or down)
       int direction = (targetFloor > currentFloor) ? 1 : -1;
       // Move the elevator to the target floor
       while (currentFloor != targetFloor) 
				 {
					// Simulate the elevator moving to the next floor
					vTaskDelay(pdMS_TO_TICKS(2500)); // Adjust the delay as needed
					// Update the current floor based on the direction
					currentFloor += direction;
					CLI_Transmit(SCROLL_REGION, sizeof(SCROLL_REGION));	//set the scroll window
					CLI_Transmit(CLEAR_SCREEN, sizeof(CLEAR_SCREEN));	//clear the screen 
					CLI_Transmit(MOVE_CURSOR_TOP, sizeof(MOVE_CURSOR_TOP));	//move cursor to the status window
					const char* statusWin = "\r\nFloor: ";
					// Calculate the length of the concatenated string
					size_t totalLength = strlen(statusWin) + 1; // +1 for the null terminator
					char buffer[50]; // Adjust the size based on your needs
					// Convert the current floor to a string and concatenate
					snprintf(buffer, sizeof(buffer), "%s%d\r\n", statusWin, currentFloor);
					uint16_t statusWinLength = strlen(buffer);
					uint8_t* dataSt = (uint8_t*)buffer; //send the new frequency
					CLI_Transmit(dataSt, statusWinLength);
					CLI_Transmit(SCROLL_REGION, sizeof(SCROLL_REGION)); //scroll
					CLI_Transmit(RESTORE_CURSOR, sizeof(RESTORE_CURSOR)); //move cursor back to messgae 
					CLI_Transmit(MOVE_CURSOR_MIDDLE, sizeof(MOVE_CURSOR_MIDDLE));	
				 }
		  }
	}
}

static void vCLITask(void * parameters)
{
	CLI_Transmit(SCROLL_REGION, sizeof(SCROLL_REGION));	//set the scroll window
  CLI_Transmit(CLEAR_SCREEN, sizeof(CLEAR_SCREEN));	//clear the screen 
	CLI_Transmit(MOVE_CURSOR_TOP, sizeof(MOVE_CURSOR_TOP));	//move cursor to the status window
	const char* statusWin = "\r\nFloor: 1\r\n";		
	uint16_t statusWinLength = strlen(statusWin);
	uint8_t* dataSt = (uint8_t*)statusWin; //send the new frequency
	CLI_Transmit(dataSt, statusWinLength);
	CLI_Transmit(SCROLL_REGION, sizeof(SCROLL_REGION)); //scroll
	CLI_Transmit(RESTORE_CURSOR, sizeof(RESTORE_CURSOR)); //move cursor back to messgae 
	CLI_Transmit(MOVE_CURSOR_MIDDLE, sizeof(MOVE_CURSOR_MIDDLE));	//move the cursor to the message window	
	
	uint8_t charReceived;
	
	for (;;) 
	{
		BaseType_t status = xQueueReceive(xCliQueue, &charReceived, 100); //block until there is something in the queue
		if (status == pdTRUE) 
			{ 
			 if(charReceived == 0x08 || charReceived == 0x7f) { //backspace or delete has been pressed
			 charReceived = 0x7f; //ensure delete is sent
			}	
				sendbyte(charReceived);	//send received character
				CLI_Receive(&charReceived);	//append to the overall message/check message value
			}
	}
}



