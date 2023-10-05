#include "stm32f10x.h"
#include <stdio.h>
#include "IOdef.h"
#include "Cli.h"
#include "usart.h"


void CLI_Transmit(uint8_t *pData, uint16_t Size)	//Phase 2
{
	for (uint16_t i=0; i < Size; i++)
	{
		sendbyte(pData[i]);
	}
}

void CLI_Receive(uint8_t *pData, uint16_t Size)		//Phase 2
{
	for (uint16_t i=0; i < Size;i++)
	{
		pData[i] = getbyte();
	}
	
}

