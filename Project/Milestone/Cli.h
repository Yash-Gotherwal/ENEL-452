#ifndef Cli
#define Cli

#include "stm32f10x.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "queue.h"

#define SCROLL_REGION "\x1b[15;20r"						//defines the region in which scrollable window works
#define POSITION_CURSOR "\x1b[10;0H"
#define MOVE_CURSOR_TOP "\x1b[1;1H"  					//move cursor to top of window
#define MOVE_CURSOR_MIDDLE "\x1b[15;1H" 			//Creates scrollable window. This is the number of lines with scrollable properties
#define CLEAR_SCREEN "\x1b[2J"
#define CLEAR_LINE "\x1b[K"										//clear line at cursor position
//#define BACKGROUND "\x1b[92m"									//Change background in our case font colours
#define SAVE_CURSOR "\x1b[s"
#define RESTORE_CURSOR "\x1b[u"

typedef struct{
	char command[10];
}CommandData;


void CLI_Transmit(uint8_t *pData, uint16_t Size);
int sendbyte(uint8_t b);
//void set_screen(void);
void CLI_Receive(CommandData *cmdData,uint8_t *pData);
char getbyte(void);
void UpdateStatus(int currentFloor);
void InitializeCLI(void);
void ProcessReceivedChar(CommandData *cmdData,uint8_t charReceived);



#endif 
