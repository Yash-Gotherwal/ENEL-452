/*
Yash Gotherwal
200420786
ENEL_452 LAB1 

We will write a program to Transmit and Recieve commands which turns the board LED on and off.
This code is done in three different phases
First phase enables Usart2 and does the transmit-recieve
Second phase does the CLI_Transmit and CLI_Recieve which is an improvement to the Usart Code itself
Third phase connects everything together by adding different commands
Steps to follow

understanding the manual:

Phase 1:
- Check Schematics to see where inbuilt LED,Usart2 is connected. In this case LED connected to D13->PA5
- Make a serial open function in Usart.c which enables all registers and pins needed for lab
- Make a serial close function to close all ports and reset configuration.
- Make a Usart send(sendbyte) and recieve(getbyte) function and check if they work using main.


Phase 2:
- Make a Cli.c file and add a CLI_transmit and CLI_Recieve function. Implement the sendbyte and get byte 
function in them respectively.

Phase 3:
-Add functionality to CLI_Transmit and CLI_Recieve to send and recieve commands to turn led on and off.


*/

#include "stm32f10x.h" //include nucleoboard libraries
#include "usart.h"
#include "Cli.h"

int main(void) 
{

	serial_open();			//Enable required registers
	
	while(1){
		uint8_t sendData[] = "hello,world! \n";
		uint8_t recieveData[50];
		
		CLI_Transmit(sendData, sizeof(sendData));
		CLI_Receive(recieveData, sizeof(recieveData));
		
	}
	
	return 0;
	
}
