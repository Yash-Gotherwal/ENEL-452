#ifndef usart
#define usart


#include "stm32f10x.h"
#include <stdio.h>

void delay(uint32_t delay_time); // Added prototype as keil gave warning error.
void serial_open (void);
void serial_close(void);
void toggle_Led_ON(void);
void toggle_Led_OFF(void);
int sendbyte(uint8_t b);
char getbyte(void);



 
 
#endif

