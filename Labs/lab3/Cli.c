#include "stm32f10x.h"
#include <stdio.h>
#include "IOdef.h"
#include "Cli.h"
#include "usart.h"
#include "string.h"

uint32_t Timeout = 1000;

void CLI_Transmit(uint8_t *pData, uint16_t Size)	//Phase 2
{
	for (uint16_t i=0; i < Size; i++)
	{
		sendbyte(pData[i], Timeout);
	}
}

void CLI_Receive(uint8_t *pData, uint16_t Size)		//Phase 2 & phase 3
{
	int checkBuffer = 0;
	for (uint16_t i=0; i < Size && !checkBuffer;i++)
	{
		//pData[i] = getbyte();												<--used for phase 2 commented out for phase 3
		
		uint8_t recChar;													
    uint8_t receivedIndex = 0;
    char commandBuffer[10]; 											// Buffer to store received command

    while (receivedIndex < Size - 1) 
			{ 
				if (Flag)
					{
           recChar = getbyte(Timeout);
           Flag = 0;
           sendbyte(recChar, Timeout);
					}
       recChar = getbyte(Timeout);									//check getbyte to see if there is any data and store it to recievedChar
			 sendbyte(recChar, Timeout); 										//Echo the received character

			 if (recChar == '\r' || recChar == '\n') 
				 {
					checkBuffer = 1;
          pData[receivedIndex] = '\0';   					// if command is new line or blank character set the buffer index value to 0
						{																			//once user enters command
            if (strncmp(commandBuffer, "on", 2)) 	//if on call toggle_led_on function
							{
								toggle_Led_ON();
                CLI_Transmit((uint8_t*)"\nLED is now ON.\n\r", 16);	
							}
						else if (strncmp(commandBuffer, "off", strlen("off")) == 0) //if off call toggle_led_off function
							{
								toggle_Led_OFF();
                CLI_Transmit((uint8_t*)"\nLED is now OFF.\n\r", 17);
							}
							else if (strncmp(commandBuffer, "status", strlen("status")) == 0) //check status of led
							{
                if (GPIOA->ODR & GPIO_ODR_ODR5)																	//if led on
									{
                    CLI_Transmit((uint8_t*)"\nLED is ON.\r\n", 12);
									} 
								else 																														//if led off
									{
                    CLI_Transmit((uint8_t*)"LED is OFF.\r\n", 13);
									}
						  }
							else if (strncmp(commandBuffer, "help", strlen("help")) == 0) 		//help window 
								{
									char helpText[] = "\r\nAvailable commands:\r\n"
                                  "1. on - Turn on the LED.\r\n"
                                  "2. off - Turn off the LED.\r\n"
                                  "3. status - Query the state of the LED.\r\n"
                                  "4. help - Display this help message.\r\n";
									CLI_Transmit((uint8_t*)helpText, strlen(helpText));
								} 
							else
								{
                CLI_Transmit((uint8_t*)"Invalid command. Type 'help' for available commands.\r\n", 54);	       // Invalid command
								}
						}
            memset(commandBuffer, 0, sizeof(commandBuffer));							       // Reset the command buffer
						checkBuffer = 0;
            break;
				 }
        commandBuffer[receivedIndex] = recChar;        										// Store the received character in the command buffer
        receivedIndex++;
			}
	
	}
}
	
