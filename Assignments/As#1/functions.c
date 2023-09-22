/*
file where functions are defined. i used toUpperCase() to convert all characters from lowercase
to uppercase this happens by subtracting value 32 from the charachters as the ASCII value for 
uppercase characters is 32 less than lowercase charachters

The second function is a calculate function. it does computations for complex numbers and returns 
result i used the switch case to do all calculations in a single program.

*/

#include "functions.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

char toUpperCase(char c) 
	{
    return (c >= 'a' && c <= 'z') ? c - 32 : c;
	}

struct Complex calculate(char op, const struct Complex z1, const struct Complex z2)
	{
		struct Complex result = {0.0, 0.0};
		switch (op)
			{
				case 'A': 
				result.real = z1.real + z2.real;
				result.imag = z1.imag + z2.imag;
				break;
				case 'S': 
				result.real = z1.real - z2.real;
				result.imag = z1.imag - z2.imag;
				break;
				case 'M': 
				result.real = z1.real * z2.real - z1.imag * z2.imag;
				result.imag = z1.real * z2.imag + z1.imag * z2.real;
				break;
				case 'D': 
					{
						double denominator = z2.real * z2.real + z2.imag * z2.imag;
						result.real = (z1.real * z2.real + z1.imag * z2.imag) / denominator;
						result.imag = (z1.imag * z2.real - z1.real * z2.imag) / denominator;
                
						if (denominator == 0.0)
							{
								fprintf(stderr, "Error code 4: divide by zero.\n");
								break;	
							}
					}
			}
	return result;
}
