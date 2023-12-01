/*
											ENEL 452 - Final Project - Elevator
															Yash Gotherwal
																200420786

This code is for the milestone demonstration i plan to make the elevator circuit
with CLI and user interrupt. For my milestone i want to get the cli status window 
working which will show the tasks currently running.

V1:
implements a working CLI Status window where user can enter commands such as p1,g or 1.
For my milestone i needed a working cli status window i was able to add tasks for 
movement. i plan to add 4 more floors and change background or font colour for each floor
for elevator move task

V2:
-Added commands for floors 1-5.
-Added escape sequences so that font changes for each floor.
-Cleaned up main.c by creating InitializeCLI,ProcessReceivedChar,UpdateStatus functions in Cli.c
 which are called in vCLITask and vMoveElevator.
-Added a bit more comments for tasks and explaination
-removed unused floor message char. array from cli.c as i was not able to figure it out
-Added a typedef in Cli.h and added pointers in CLI_Recieve and updated CLI_Task to
remove char command[] from global variable. By doing so encountering an error where
first input is not being considered. i.e. first command does not work.
-Commented out set_screen function and function call in main.c as i included that in
InitializeCLI
-Tabified code and tried to make commenting clearer

----------****To-DO****------------
-Add maintainence mode and Userbutton handler
	-Enable EXTI and check if remap to PC13 required
-Check requirements from project and add stuff accordingly


*/

#include "stm32f10x.h"
#include "Init.h"
#include "Cli.h"
#include <stdint.h>

#define Move_ELEVATOR_TASK_PRIORITY	(tskIDLE_PRIORITY + 2)
#define Cli_TASK_PRIORITY					(tskIDLE_PRIORITY + 1)

static void vMoveElevator(void *parameters);
static void vCLITask(void *parameters);

extern QueueHandle_t xMoveElevator;
extern QueueHandle_t xCliQueue;

int main(void)
{
		serial_open();
		//set_screen();

		// Create queues and tasks
		xCliQueue = xQueueCreate(1, sizeof(char));
		xMoveElevator = xQueueCreate(1, sizeof(int)); 
		xTaskCreate(vMoveElevator, "MOVE", configMINIMAL_STACK_SIZE, NULL, Move_ELEVATOR_TASK_PRIORITY, NULL);
		xTaskCreate(vCLITask, "CLI", configMINIMAL_STACK_SIZE, NULL, Cli_TASK_PRIORITY, NULL);

		vTaskStartScheduler(); // Start the scheduler

		return 0;
}

// Task to move the elevator
static void vMoveElevator(void *parameters) 
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

				// Update the status on the CLI
				UpdateStatus(currentFloor);
			 }
		 }
	 }
}

// Task for CLI processing
static void vCLITask(void *parameters) 
{
	InitializeCLI();	// Set up the initial CLI display
	CommandData cmd;
	uint8_t charReceived;

	// Prompt for user input
	const char* Message = "\r\nWhat floor?: ";
	uint16_t length = (uint16_t)strlen(Message);
	uint8_t* message = (uint8_t*)Message;	
	CLI_Transmit(message, length); 

	for (;;) 
	 {
		BaseType_t status = xQueueReceive(xCliQueue, &charReceived, 100);
		
		if (status == pdTRUE)
			{
			 ProcessReceivedChar(&cmd,charReceived);
			}
	 }
}


