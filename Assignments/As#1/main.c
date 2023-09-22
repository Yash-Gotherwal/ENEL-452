/**
Course: ENEL 452
Sept 20,2023
Yash Gotherwal
200420786

This is the main file for Complex calculator As#1. This code accomplishes
the task of creating a REPL style complex calculator. 

There is 2 ways to input commands. 

-The user can Manually type in A,S,M,D commands followed by 4 numbers seperated with a space 
(first two numbers will be 1st complex number and next 2 numbers will be the second complex number)
and Depending on the command chosen the program will add,subtract,multiply or divide the complex numbers.

-The user can create a textfile in the same directory as the code and the use gcc stdinput command to read commands directly
from the text file.

Certain parameters have been added to the code such as:
1) All lowercase characters will be converted to uppercase characters
2) White spaces may be ignored
3) If a complex number is divided by zero there will be an error generated
4) There might be an error if extra arguments are passed.

The difficulties encountered while using the code are in the Readme.txt file submitted seperately.
*/
#include <stdio.h>
#include <string.h>
#include "functions.h"

int main() 
	{
	
		char line[100];
    struct Complex z1 = {0.0, 0.0};
    struct Complex z2 = {0.0, 0.0};
    struct Complex result;
    
		/*printf("This is a complex calculator assignment you will be taken to a new line after every argument. \n");
		printf("Just start entering your command in order to use the calculator. \n");
		printf("use following functions A->Add S->subtract M->multiply D->Divide. \n");
		*/
		
    while (1) 
			{
				//printf("Enter exp: \n"); ---> line was creating error in passing python tests so commented out.
        fgets(line, sizeof(line), stdin);	//gets line input from user

        if (line[0] == '\n') 
				{
					continue;
        }

        char op = toUpperCase(line[0]);	//converts lowercase to uppercase charachters

        if (op == 'Q') //Checks for q command and exits program.
					{
            break;
					}

        double real1, imag1, real2, imag2;
        int numParsed = sscanf(line + 1, "%lf %lf %lf %lf", &real1, &imag1, &real2, &imag2); //parses line and searches for 4 long float numbers

        if (numParsed != 4)
					{
            if (numParsed < 4)	
							{
                fprintf(stdout, "error code: 2: missing arguments\n");
							} 
						else 
							{
                fprintf(stdout, "error code: 3: extra arguments \n");
							}
								continue;
					}

        if (op != 'A' && op != 'S' && op != 'M' && op != 'D')
					{
            fprintf(stdout, "error code: 1: illegal command\n");
            continue;
					}

        z1.real = real1;
        z1.imag = imag1;
        z2.real = real2;
        z2.imag = imag2;

        if (op == 'D' && z2.real == 0.0 && z2.imag == 0.0) //check if denominator is zero
					{
            fprintf(stdout, "error code: 4: divide by zero\n");
					} 
				else 
					{
            result = calculate(op, z1, z2);
            // Print the result
            printf("%lf + j %lf\n", result.real, result.imag);
					}
				
			}
    return 0;
	}
