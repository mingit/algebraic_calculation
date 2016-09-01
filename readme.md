	- Author: Ming Li, ming.li@aalto.fi
	- Date: July 2016
	- Programming environment: Ubuntu 16.04, g++(-std=c++11)

*Exercise requirement
Task: calculate maximum value of a string algebraic expression 

Input: String containing an algebraic expression consisting of lowercase letters a, b, …, z, parentheses (, ), and operations +, –, *, /, ^. (The operation ^ is exponentiation, not xor.) The length of input string is limited by 1000.

Example input: ((a+b)*(b^c+(d-e)/a))^(c+d/e)

Output: The maximum value of the expression which can be achieved by substituting numbers 0 or 1 for variables (only substitutions which lead to a valid expression are allowed, i.e. you should not consider e=0 for the example above).

Example output for example input: 4.0.

You are allowed to produce error messages instead of an answer if by some reason task is unsolvable for the given input.

//Solution:
	- Step 1) Change string algebraic to infix. 
	- Step 2) Convert infix to postfix.
	- Step 3) Traverse all substitutions of variables in postfix and find the maximum output.

Key functions for each step:
	- Step 1) vector <token> to_infix (string expression);
	- Step 2) vector <token> to_postfix (vector <token> infix);
	- Step 3) a) bool getMax (vector <token> postfix, long double &maximum);
        	- b) bool calculate_postfix (vector <token> postfix, map <string, int> mapValue, long double &output); 

Complexity: N denotes the length of algebraic string, M denotes the number of variables in the algebraic expression. 
	- Step 1) O(N), where 0<N<=1000.
	- Step 2) O(N).
	- Step 3) O(2^M*N), where 0<M<=26.

key structures: stack (in the form of vector), map
key float type: long double (128-bit on author's computer) is used for algebraic computation. 

Input: 
Take input from the file "test.dat", where each line denotes a string algebraic expression.

A valid input should contain at least one variable.

*INVALID input examples include:
	- "(aa+b)"
	- "((a-b)"
	- "a+-b"
	- "()"
	- ")("
	- "A"
	- "^"
	- "(+(a-b))"
	- ""
	- " "
	- a(a*k)

Output:
	- std::cout: output important information, e.g., algebraic expression and maximum value of it.
	- std::clog: disabled as default. output algebraic calculation warning, e.g., division by zero. 
	- std::cerr: output error information, e.g., invalid algebraic expression.

Compile:
	- g++ -std=c++11 src/cal_algebraic.cpp -o bin/cal_algebraic
	- or make
Run:
	- ./bin/cal_algebraic
 
Test:
	- all expressions in file ./test/pretest.dat and ./test/test.dat were tested.
	- default input file is ./test/test.dat.

Known problem: 
	- Output may reach the boundaries of the range that a long double value can accommodate. In this case, the maximum output is inf. 
