#include <fstream>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include <math.h>
#include <cmath>
#include <string>
#include <assert.h>
#include <limits> 
#include <map>
#include <bitset>
#include <iomanip>

using namespace std;

typedef pair <int,string> token;

#define Type first
#define Value second
#define mp make_pair
#define MAX_STRING 1000 // maximum allowed length of algebraic expression
#define MIN_LDOUBLE numeric_limits<long double>::lowest() 

const int LLETTER = 0, OPERATION = 1, LBRACKET = 2, RBRACKET = 3, OTHER = -1;

//function declaration
int get_type (char);
bool greater_priority(string a, string b);
vector <token> to_infix (string);//step 1
vector <token> to_postfix (vector <token>);//step 2
bool calculate_postfix (vector <token>, map <string, int>, long double &);//step 3.a
bool getMax (vector <token>, long double &);//step 3.b

/*return char c's type grouped by const int value.*/
int get_type (char c)
{
    if  (c >= 'a' && c <= 'z') 
        return LLETTER;
    else if (c == '*' || c == '+' || c == '/' || c == '-' || c == '^') 
        return OPERATION;
    else if (c == '(' )
        return LBRACKET;
    else if (c == ')')
        return RBRACKET;
    else if (c == '~')
        return OPERATION;
    else
        return OTHER;
}

/*return true only if operator a has greater priority than b.*/
bool greater_priority(string a, string b)
{
    return ( (a == "^" && b != "^") || ((a == "*" || a == "/") && (b == "+" || b == "-")));
}

/* Tokenize the string algebraic expression. 
   Return an empty vector if the algebraic expression is invalid.
   Give potential correction tips if the expression invalid.*/
vector <token> to_infix (string expression)
{
    vector < token > tokens;
    int number_of_LLETTER = 0;
    int number_of_OPERATION = 0;
    int number_of_LBRACKET = 0;
    int number_of_RBRACKET = 0;
    bool validExpression = true;//its initial value must be true.
    int lastType = OTHER;//remember the type of last valid char

    for (unsigned int i = 0; i  < expression.size(); i++)
    {
        char c = expression.at(i);
        if (c == ' ') // remove all spaces
            continue;
        int type = get_type(c);

        switch(type)
        {
            case LLETTER:
                number_of_LLETTER++;
                lastType = type;
                break;
            case OPERATION:
                number_of_OPERATION++;
                if (lastType == OTHER)
                {
                    cerr << "Invalid expression: cannot start from an operation \'" << c << "\'." << endl;
                    validExpression = false;
                }
                if (lastType == LBRACKET)
                {
                    cerr << "Invalid expression: extra \'" << c << "\' may be after \""
                        << expression.substr (0, i) << "\" or an invalid character." << endl;
                    validExpression = false;
                }
                lastType = type;
                break;
            case LBRACKET:
                number_of_LBRACKET++;
                if (lastType == LLETTER)
                {
                    cerr << "Invalid expression:  operand \'" << expression.at(i-1) << "\' cannot be followed by \'(\'." << endl; 
                    validExpression = false;
                }
                lastType = type;
                break;
            case RBRACKET:
                number_of_RBRACKET++;
                if (lastType == OPERATION)
                {
                    cerr << "Invalid expression: operation \'" << expression.at(i-1) << "\' cannot be followed by \')\'." << endl;
                    validExpression = false;

                }
                if (lastType == OTHER)
                {
                    cerr << "Invalid expression: cannot start from an operation \'" << c << "\'." << endl;
                    validExpression = false;
                }
                lastType = type;
                break;
            default:
                cerr << "Invalid character: \'" << c << "\'." << endl;
                validExpression = false;
                break;
        }

        tokens.push_back(mp(type, expression.substr(i,1))); //push the character anyway in order to find out all potential erros.

        int ts = tokens.size();
        //two continuous operands/operations are not allowed
        if ( (ts > 1) && (tokens.at(ts-1).Value != "~") && (tokens.at(ts-1).Type == tokens.at(ts-2).Type) )
        { 
            if ( (tokens.at(ts-1).Type != LBRACKET) && (tokens.at(ts-1).Type != RBRACKET) )
            {
                cerr << "Invalid expression: extra \'" << tokens.at(ts-1).Value << "\'" 
                    << " at the end of \"" << expression.substr (0, ts) << "\"." << endl;
                validExpression = false;
            }
        }
    }

    if (number_of_LBRACKET != number_of_RBRACKET)
    {
        cerr << "Invalid expression: brackets do not match: " << number_of_LBRACKET << " \'(\' but " 
            << number_of_RBRACKET << " \')\'." << endl; 
        validExpression = false;
    }

    if ((number_of_LLETTER != 0) && (number_of_LLETTER != (number_of_OPERATION + 1)) )//general check
    {
        cerr << "Invalid expression: there may be extra operation(s) or operand(s)." 
            <<  endl; 
        validExpression = false;
    }

    if ( number_of_LLETTER == 0 )// at least one oprand should be given.
    {
        cerr << "Invalid expression: at least one valid variable should be given." 
            <<  endl; 
        validExpression = false;
    }

    if (!validExpression)
        tokens.clear ();

    return tokens;
}

/*Derive infix to postfix notation with priority handling. 
  Return an empty vector if the infix is not an valid algebraic expression.*/
vector <token> to_postfix (vector <token> infix)
{
    vector <token> postfix;
    vector <token> stackbuf; // stack buffer 
    token tk;

    for (unsigned int i = 0; i < infix.size (); i++)
    {
        tk = infix.at(i);
        if ( tk.Type == LLETTER)
        {
            postfix.push_back(tk);
        }
        else if ( tk.Type == OPERATION)
        {
            while ( (!stackbuf.empty ()) && (stackbuf.back().Type == OPERATION) )
            {
                if ( !greater_priority (tk.Value, stackbuf.back().Value) )
                {
                    postfix.push_back ( stackbuf.back() );
                    stackbuf.pop_back();
                }
                else
                    break;
            }
            stackbuf.push_back(tk);
        }
        else if (tk.Type == LBRACKET)
        {
            stackbuf.push_back(tk);
        }
        else if (tk.Type == RBRACKET)
        {
            while ( !stackbuf.empty () && (stackbuf.back().Type != LBRACKET) )
            {
                postfix.push_back ( stackbuf.back() );
                stackbuf.pop_back();
            }
            if (stackbuf.empty())
            {
                cerr << "Invalid expression: brackets does not match." 
                    <<  endl;
                postfix.clear();
                return postfix;
            }
            else
                stackbuf.pop_back (); //pop the left bracket
        }
    }

    while ( !stackbuf.empty ())
    {
        if (stackbuf.back().Type != OPERATION)
        {
            postfix.clear ();
            return postfix;
        }

        postfix.push_back ( stackbuf.back() );
        stackbuf.pop_back();
    }

    return postfix;
}

/*calculate the value of postfix with assigned value. Return true if a valid output is calculated. The valid output is stored in the third function parameter: long double &output.*/
bool calculate_postfix (vector <token> postfix, map <string, int> mapValue, long double &output)
{
    vector <token> stackbuf; // stack buffer 
    token tk;
    long double op1, op2, val;

    if (postfix.empty() )
    {
        cerr << "Error! expression is not complete."  << endl; 
        return false;
    }

    if (postfix.size () == 1)// if there is only one valid operand, to add "+0" to make the postfix complete while keeping the same result.
    {
        postfix.push_back ( mp(LLETTER, "0") );
        postfix.push_back ( mp(OPERATION, "+") );
    }

    for (unsigned int i = 0; i < postfix.size (); i++)
    {
        tk = postfix.at(i);
        if (tk.Type == OPERATION)
        {
            if (stackbuf.size () < 2)
            {
                cerr <<"Error! stackbuf should have at least two elements. stackbuf.size () = " << stackbuf.size () << "." << endl; 
                return false;
            }

            if ( mapValue.find (stackbuf.back().Value) != mapValue.end() ) // try to find the character in mapValue
                op2 = mapValue[stackbuf.back ().Value];
            else
                op2 = std::stold (stackbuf.back().Value); // use the value calculated by previous operations
            stackbuf.pop_back();

            if ( mapValue.find (stackbuf.back().Value) != mapValue.end() ) 
                op1 = mapValue[stackbuf.back ().Value];
            else
                op1 = std::stold (stackbuf.back ().Value); 
            stackbuf.pop_back();

            if (tk.Value == "*")
            {
                val = op1 * op2;
            }
            else if (tk.Value == "+")
            {
                val = op1 + op2;
            }
            else if (tk.Value == "-")
            {
                val = op1 - op2;
            }
            else if (tk.Value == "/")
            {
                if (op2 == 0)
                {
                    clog << __FILE__ << ": log at line " <<  __LINE__ << ": Division by zero." << endl; 
                    return false;
                }
                val = op1 / op2;
            }
            else if (tk.Value == "^")
            {
                if ( (op1 == 0) && (op2 <= 0) )// pow (0, -1) = inf
                {
                    clog << __FILE__ << ": log at line " <<  __LINE__ << ": if base is zero, exponent should be a positive number." << endl; 
                    return false;
                }
			
                val = pow (op1, op2);

				if (isnan(val))
				{
                    clog << __FILE__ << ": log at line " <<  __LINE__ << ": wrong parameters for pow function because it outputs a meaningless value nan." << endl; 
					return false;
				}

            }

            stackbuf.push_back( mp(LLETTER, std::to_string (val)) );//add convert long double to string function
        }
        else if (tk.Type == LLETTER)
        {
            stackbuf.push_back( mp(tk.Type, tk.Value) );
        }
        else
        {
            cerr << "Invalid type  =  " << tk.Type << "." << endl; 
            return false;
        }
    }

    if (stackbuf.size () != 1)// at last stackbuf should only have one element, which accommodates the result.
    {
        cerr << "Error stackbuf.size () = " << stackbuf.size () << "." << endl; 
        return false;
    }

    output = std::stold (stackbuf.back().Value);
    return true;
}

/*Get the maximum value of the postfix by substituting 0 or 1 for each variable. Return false if there is no valid result at all after doing all substitutions. 
  map structure is used in the process of substituting 0 and 1 for each variable.
 */
bool getMax (vector <token> postfix, long double &maximum)
{
    long double val = MIN_LDOUBLE;
    bool validMax = false;
    map <string, int> mapValue;
    map <string, int> mapValue_for_maximum;//one of the substitutions which can reach the maximum output
    long loops = 0;//loops could be up to pow(2,26) - 1 in this program.

    for (vector<token>::iterator it = postfix.begin() ; it != postfix.end(); it++)
    {
        if (get_type (it->Value.at(0)) == LLETTER)
            mapValue[it->Value] = 0; 
    }

    loops = pow (2, mapValue.size ());

    for (long i = 0; i < loops; i++)
    {
        string binary_value = bitset<26>(i).to_string();

        for (map<string, int>::iterator it = mapValue.begin() ; it != mapValue.end(); it++)
        {
            it->second = stoi(string (1, binary_value.back())); 
            binary_value.pop_back ();
        }

        if (calculate_postfix (postfix, mapValue, val))
        {
            if (val > maximum)
            {
                maximum = val;
                mapValue_for_maximum = mapValue;
            }

            validMax = true;
        }
    }

    if (validMax)
    {
        clog << "Substitution which can reach the maximum output of the algebraic expression." << endl;
        for (std::map<string, int>::iterator it=mapValue.begin(); it!=mapValue.end(); ++it)
            clog << it->first << " => " << it->second << '\n';
    }

    return validMax;
}

int main (void)
{
    string expression = "";
    string file = "./test/test.dat";//change it to any file which contains the expressions for test.
    vector <token> infix, postfix;
    int expression_counter = 0;

    clog.rdbuf(0);//disable clog output, comment it out if algebraic warning information is requred, e.g., division by zero.

    cout << "This program takes algebraic expressions in \"./test/test.dat\" as input, each line for one expression." 
        << " Feel free to change it." << endl
        << "\"./test/pretest.dat\" contains various input examples for test." << endl  
        << "Hit Enter key to continue..." << endl;

    getchar();

    fstream testFile (file);
    if (testFile)
    {
        while (getline (testFile, expression))
        {
            cout << endl << endl;
            cout << "Algebraic expression " << ++expression_counter << ": \"" << expression << "\"." << endl;
            if (expression.size () > MAX_STRING)//length check
            {
                cerr << "The length of this expression is " << expression.size () << " (including space)." << endl;
                cerr << "It exceeds the maximum allowed size " << MAX_STRING << "." << endl;
                continue;
            }

            infix = to_infix (expression);
            if (infix.empty ())
            {
                cerr << "Error! Invalid algebraic expression!" << endl;
                continue;
            }

            postfix = to_postfix (infix);
            if (postfix.empty())
            {
                cerr << "Error! Invalid postfix!" << endl;
                continue;    
            }

            clog << "postfix: ";
            for (unsigned int i = 0; i < postfix.size (); i++)
            {
                clog << postfix.at (i).Value;
            }
            clog << endl;

            long double maximum = MIN_LDOUBLE;
            if (getMax (postfix, maximum))
            {
                if(!isinf(maximum))
                    cout << "Maximum output: " << setiosflags(ios::fixed) << setprecision(1) << maximum << "." << endl;
                else
                    cout << "Maximum output: infinity." << endl;
            }
            else 
                cout << "No valid output for this expression after trying all possible substitutions." << endl;
        }
        testFile.close();

        if (expression_counter == 0)
            cout << "test.dat is empty. Please type in algegraic expression for test." << endl;
    }
    else
    {
        ofstream createfile (file);
        if (createfile)
            cerr << "Cannot find the test file: " << file
                << ". This program just created it for you. Please type in your algebraic expression and run this program again." << endl;
        else
            cerr << "Error! cannot find the test file and cannot create a new one! Please ask help from computer admin." << endl; 

        return -1;
    }

    cout << endl;

    return 0;
}

