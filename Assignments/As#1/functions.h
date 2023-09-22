/*
Header file for functions
*/
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

struct Complex 
	{
	double real;
  double imag;
	};

char toUpperCase(char c);
struct Complex calculate(char op, const struct Complex z1, const struct Complex z2);

#endif // FUNCTIONS_H
