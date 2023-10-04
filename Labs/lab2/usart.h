#ifndef usart
#define usart


#include "stm32f10x.h"
#include <stdio.h>

void delay(uint32_t delay_time); // Added prototype as keil gave warning error.
void clockInit(void);
void serial_open (void);
void blink(void);
void sendbyte(uint8_t b);
uint8_t getbyte(void);
void serial_close(void);


 
 
 
 
#endif

