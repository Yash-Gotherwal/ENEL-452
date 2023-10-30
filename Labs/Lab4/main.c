/*
Yash Gotherwal
200420786
ENEL_452 LAB4 

We will write a program to create a CLI that has a status window that displays information on the top
and has a scrollable window of 10 lines starting at line 10.

I built this lab on top of Lab2 code as i had problems making my interrupts lab run.

Just for fun i have changed my foreground (font) colour to cyan in the terminal by using escape sequence.

Steps to follow

Define escape sequences for: A scrollable region, Top of terminal, Saving and restoring cursor positions
moving cursor to middle of terminal and background.

Add initial scroll regions and backgrounds to main with a welcome message.

Add all defines in the present code from lab2. All i did in the lab was follow the psuedocode as follows.

After each working command:
1.Save cursor location.
2.Move cursor to top of screen.
3.Clear the line at top of screen.
4.Transmit the new message.
5.move cursor back to scroll region 
6. Restore cursor position.




*/

#include "stm32f10x.h" //include nucleoboard libraries
#include "usart.h"
#include "Cli.h"

int main(void) 
{

	serial_open();			//Enable required registers
	
		CLI_Transmit(SCROLL_REGION, sizeof(SCROLL_REGION));	//set scroll window
    CLI_Transmit(CLEAR_SCREEN, sizeof(CLEAR_SCREEN));		//clear the screen 
    CLI_Transmit(MOVE_CURSOR_MIDDLE, sizeof(MOVE_CURSOR_MIDDLE));		//move the cursor to the start of scroll window
	  CLI_Transmit(BACKGROUND,sizeof(BACKGROUND));	
	
		const char welcomeMessage[] = "\r\nEnter command(help for more info):";		
	
	
	while(1){
		uint8_t recieveData[50];
		CLI_Transmit(welcomeMessage, sizeof(welcomeMessage));
		CLI_Receive(recieveData, sizeof(recieveData));
	}
	
	return 0;
	
}
