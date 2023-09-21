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

#include "stm32f10x.h" 					 //include nucleoboard libraries

void delay(uint32_t delay_time); // Added prototype as keil gave warning error.

void delay(uint32_t delay_time) 
{
	for (volatile uint32_t i = 0; i < delay_time * 8000; i++)
		{ 
        // Delay loop
    }
}

int main(void) 
{
	// Enable the GPIOA peripheral clock
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;  

  // Configure PA5 as a general-purpose output push-pull (for the onboard green LED)
  GPIOA->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_MODE5); // Clear the bits first
  GPIOA->CRL |= GPIO_CRL_MODE5_0; 								 // Set MODE5_0 bit to configure as 10 MHz output

	while(1) 
		{
			// Turn the LED on (set PA5 high)
      GPIOA->BSRR = GPIO_BSRR_BS5;

      // Delay for 1 second
      delay(1000);

      // Turn the LED off (set PA5 low)
      GPIOA->BSRR = GPIO_BSRR_BR5;

      // Delay for 1 second
      delay(1000);
    }
}
