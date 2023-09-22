Yash Gotherwal
200420786
ENEL 452- As#1
Complex Calculator

The above files are code files for a Complex Calculator. It provides a Read-Evaluate-Print Loop (REPL) interface where users can perform basic arithmetic operations on complex numbers.
The program supports addition (A), subtraction (S), multiplication (M), and division (D) of complex numbers.

The code has the following features .

- Accepts user input in the form of commands followed by complex numbers.
- Handles complex number arithmetic operations (addition, subtraction, multiplication, division).
- Converts lowercase command characters to uppercase for consistency.
- Checks for invalid commands. Anything other than A,S,M,D and Q should give illegal command error (Error 1)
- Validates user input and checks for missing or extra arguments. (Error 2 and 3) <-- i was unable to pass the tests for this
- Detects and reports division by zero errors. (Error 4)

The code is organized into three files:

1. `main.c`: This file contains the main program logic, including user input processing and error handling. It utilizes functions defined in `functions.h` and `functions.c` for complex number calculations.
2. `functions.h`: This header file contains function declarations for utility functions, including converting characters to uppercase and the complex number calculation function.
3. `functions.c`: This source file defines the actual implementations of the utility functions and the complex number calculation function.

You can compile the code in gcc using gcc main.c functions.c -o {"your executable name file here"}.

problems:
my file was not passing python tests with the printf("enter exp:") so i commented out the line and my program still works it just does not have any command for letting user know when to input.

i took the idea to use toUppercase functions from the stack overflow refrenced below.

refrences:
https://stackoverflow.com/questions/34703007/to-converting-upper-and-lower-case-why-we-add-32-and-32-pleas-explain
