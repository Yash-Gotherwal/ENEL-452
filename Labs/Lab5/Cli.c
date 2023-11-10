#include "FreeRTOS.h"
#include "stm32f10x.h"
#include <stdio.h>
#include "IOdef.h"
#include "Cli.h"
#include "usart.h"
#include "string.h"
#include "queue.h"

QueueHandle_t Frequency_queue;


void CLI_Transmit(uint8_t *pData, uint16_t Size)	
{
	for (uint16_t i=0; i < Size; i++)
	{
		sendbyte(pData[i]);
	}
}

void CLI_Receive(uint8_t *pData, uint16_t Size)		
{
	for (uint16_t i=0; i < Size;i++)
	{
		uint8_t receivedChar;													
    uint8_t receivedIndex = 0;
      char commandBuffer[10] ="";  	// Buffer to store received command
			int command = strlen(commandBuffer);
	
    while (receivedIndex < Size - 1) 
			{ 
       receivedChar = getbyte();									//check getbyte to see if there is any data and store it to recievedChar
       sendbyte(receivedChar); 										//Echo the received character

			 if (receivedChar == '\r' || receivedChar == '\n') 
				 {
          pData[receivedIndex] = '\0';   					// if command is new line or blank character set the buffer index value to 0
						{																			//once user enters command
            if (strncmp(commandBuffer, "on", strlen("on")) == 0) 	//if on call toggle_led_on function
							{
								toggle_Led_ON();
                CLI_Transmit((uint8_t*)"\nLED is now ON.\n\r", 16);	
								//Psuedo code explained in main.c
								CLI_Transmit(SAVE_CURSOR, sizeof(SAVE_CURSOR));						
								CLI_Transmit(MOVE_CURSOR_TOP, sizeof(MOVE_CURSOR_TOP));		
								CLI_Transmit(CLEAR_LINE, sizeof(CLEAR_LINE));							
								const char on_statement[] = "The LED is Turned ON";			
								CLI_Transmit(on_statement, sizeof(on_statement));							
								CLI_Transmit(SCROLL_REGION, sizeof(SCROLL_REGION));
								CLI_Transmit(RESTORE_CURSOR, sizeof(RESTORE_CURSOR));
							}
						else if (strncmp(commandBuffer, "off", strlen("off")) == 0) //if off call toggle_led_off function
							{
								toggle_Led_OFF();
                CLI_Transmit((uint8_t*)"\nLED is now OFF.\n\r", 17);
								//Psuedo code explained in main.c
								CLI_Transmit(SAVE_CURSOR, sizeof(SAVE_CURSOR));						 
								CLI_Transmit(MOVE_CURSOR_TOP, sizeof(MOVE_CURSOR_TOP));		
								CLI_Transmit(CLEAR_LINE, sizeof(CLEAR_LINE));							
								const char off_statement[] = "The LED is Turned OFF";			
								CLI_Transmit(off_statement, sizeof(off_statement));									
								CLI_Transmit(SCROLL_REGION, sizeof(SCROLL_REGION));				
								CLI_Transmit(RESTORE_CURSOR, sizeof(RESTORE_CURSOR));
							}
							if (strncmp(commandBuffer, "status", strlen("status")) == 0) //check status of led
							{
                if (GPIOA->ODR & GPIO_ODR_ODR5)																	//if led on
									{
                    CLI_Transmit((uint8_t*)"\nLED is ON.\r\n", 12);
										CLI_Transmit(SAVE_CURSOR, sizeof(SAVE_CURSOR));						
										CLI_Transmit(MOVE_CURSOR_TOP, sizeof(MOVE_CURSOR_TOP));		
										CLI_Transmit(CLEAR_LINE, sizeof(CLEAR_LINE));							
										const char on_statement[] = "The LED is currently ON";			
										CLI_Transmit(on_statement, sizeof(on_statement));													
										CLI_Transmit(SCROLL_REGION, sizeof(SCROLL_REGION));
										CLI_Transmit(RESTORE_CURSOR, sizeof(RESTORE_CURSOR));
									} 
								else 																														//if led off
									{
                    CLI_Transmit((uint8_t*)"LED is OFF.\r\n", 13);
										CLI_Transmit(SAVE_CURSOR, sizeof(SAVE_CURSOR));						
										CLI_Transmit(MOVE_CURSOR_TOP, sizeof(MOVE_CURSOR_TOP));		
										CLI_Transmit(CLEAR_LINE, sizeof(CLEAR_LINE));							
										const char off_statement[] = "The LED is currently OFF";			
										CLI_Transmit(off_statement, sizeof(off_statement));													
										CLI_Transmit(SCROLL_REGION, sizeof(SCROLL_REGION));				
										CLI_Transmit(RESTORE_CURSOR, sizeof(RESTORE_CURSOR));
									}
						  }
							else if (strncmp(commandBuffer, "help", strlen("help")) == 0) 		//help window 
								{
									CLI_Transmit(SAVE_CURSOR, sizeof(SAVE_CURSOR));						
									CLI_Transmit(MOVE_CURSOR_TOP, sizeof(MOVE_CURSOR_TOP));		
									CLI_Transmit(CLEAR_LINE, sizeof(CLEAR_LINE));							
									const char helpText[] = "\r\nAvailable commands:\r\n"
                                  "1. on - Turn on the LED.\r\n"
                                  "2. off - Turn off the LED.\r\n"
                                  "3. status - Query the state of the LED.\r\n"
                                  "4. help - Display this help message.\r\n";
									CLI_Transmit((uint8_t*)helpText, strlen(helpText));
									CLI_Transmit(SCROLL_REGION, sizeof(SCROLL_REGION));				
									CLI_Transmit(RESTORE_CURSOR, sizeof(RESTORE_CURSOR));
								}
							else if (strncmp(commandBuffer, "freq1", strlen("freq1")) == 0 )
							{
								int f = 200;
								xQueueSendToFront(Frequency_queue, &f, portMAX_DELAY);
							}
							else if (strncmp(commandBuffer, "freq2", strlen("freq2")) == 0 )
							{
								int f = 1000;
								xQueueSendToFront(Frequency_queue, &f, portMAX_DELAY);
							}
							else if (strncmp(commandBuffer, "freq3", strlen("freq3")) == 0 )
							{
								int f = 3000;
								xQueueSendToFront(Frequency_queue, &f, portMAX_DELAY);
							}
            memset(commandBuffer, 0, sizeof(command));							       // Reset the command buffer
            CLI_Transmit((uint8_t*)"Invalid command. Type 'help' for available commands.\r\n", 54);	       // Invalid command
				}
			}
		}
	}
}
	
