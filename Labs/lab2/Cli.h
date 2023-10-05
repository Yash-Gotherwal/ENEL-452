#ifndef Cli
#define Cli

#include "stm32f10x.h"
#include <stdio.h>

void CLI_Transmit(uint8_t *pData, uint16_t Size);
void CLI_Receive(uint8_t *pData, uint16_t Size);

#endif

