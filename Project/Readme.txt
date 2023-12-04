						ENEL 452 - Final Project - Elevator Control
							Yash Gotherwal
   							  200420786

This code is for the Final submission for my Elevator control system for the ENEL 452 project.

The features include:
-> Elevator movement between floors 0-5 and parking p1.
	--> commands: 0-5 and p1 can be executed.
	--> Each floor has a different colour associated with it.
-> EMERGENCY STOP:
	--> emergency stop command 'e' can be executed anytime and 
	    elevator will stop working displaying a message along with an alert.
-> Maintainence mode has been added as a user button interrupt (pc13).
	--> Press the user button on any floor and it will stop the elevator
	    operations while displaying a message.
	--> Commands can be entered but will not be executed unless the user
	    button is pressed again.
	--> Once the user button is pressed a second time the elevator will
	    execute the last command issued.


------------------Versions and releases----------------------------------------------

V1:
implements a working CLI Status window where user can enter commands such as p1,g or 1.
For my milestone i needed a working cli status window i was able to add tasks for 
movement. i plan to add 4 more floors and change background or font colour for each floor
for elevator move task

V2:
-Added commands for floors 1-5.
-Added escape sequences so that font changes for each floor.
-Cleaned up main.c by creating InitializeCLI,ProcessReceivedChar,UpdateStatus functions in Cli.c
 which are called in vCLITask and vMoveElevator.
-Added a bit more comments for tasks and explaination
-removed unused floor message char. array from cli.c as i was not able to figure it out
-Added a typedef in Cli.h and added pointers in CLI_Recieve and updated CLI_Task to
remove char command[] from global variable. By doing so encountering an error where
first input is not being considered. i.e. first command does not work.
-Commented out set_screen function and function call in main.c as i included that in
InitializeCLI
-Tabified code and tried to make commenting clearer

V3:
- Enabled AFIO clock and mapped pin pc13 to exti13
- Enabled external line interrupt for 10-15 (same address in NVIC)
- Added EXTI IRQ HANDLER which sends stuff to maintainence queue.
- Added Door opening and door closing status in update status.
- Added emergency stop command such that when command e is entered the code stops working and displays a message with an
  alert sound.
- Added maintainence mode functionality on user button.
	--> When user button is pressed elevator goes into maintainence mode and does not 
			accept any commands until user button is pressed again.
	--> Once user button is pressed again the elevator will execute the last command issued while in
			maintainence mode.
	--> For eg. - Elevator on floor 2
		    - user button pressed (enter maintainence)
		    - command to go on floor 3	(will not work)
		    - command to go on floor 1	(will not work)
		    - press user button again (exit maintainence mode)
		    - command to go to floor 1 will be executed.


--------------------------------Known Code issues-------------------------------------

- First input does not work we need to add the command twice. All commands after the first one works fine.
- When changing 2 floors at the same time the colour changes only till next floor.
- Need to reset the board if emergency stop command is executed.
