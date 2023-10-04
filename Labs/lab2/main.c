/*
Yash Gotherwal
200420786
ENEL_452 LAB1 

We will write a program to make LED blink on and off for Lab 1 assignment

Steps to follow

understanding the manual:
-Check Schematics to see where inbuilt LED is connected. In this case LED connected to D13->PA5
-Enable Clock for Port A
-Using reference manual GPIO mapping we understand that in order to make PA5 a push-pull output we have to turn Mode5 to 01. PA5 is in GPIO_CRL

Coding to blink:
- Add a delay function to be used in main code which makes the LED blink every second (1 sec-on/1 sec-off)
- Enable Clock for port A
- Set mode5 to 01 as output
- run an infinite loop which sets and resets bit on PA5 as on or off.


*/

#include "stm32f10x.h" //include nucleoboard libraries
#include "usart.h"

int main(void) 
{
	clockInit();
	// Enable the GPIOA peripheral clock
	serial_open();
	while(1){
	blink();
	uint8_t x = 0x48;
	sendbyte(x);
	uint8_t val = getbyte();
	blink();		
	//printf("Received data: 0x%02X\n", val); 
	}

}

