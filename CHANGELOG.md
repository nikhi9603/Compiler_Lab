##### WEEK1 : CALCULATOR
_Directory_ : calculator 
<br />
A simple calculator is designed allowing basic arithmetic operations + , - , * , / over integers and floating point numbers.
<br />
<br />
_Instructions to run_
<br />
`make` : To compile the calculator files and create an executable `calculator.out` to run
<br />
`make tests` : To run any testcase.
<br />Sample test cases are provided. (test2.txt: division by zero expr is present)
<br />When asked for a filename to test, provide testcase file which is present in current directory only(calculator)
<br />
`make clean` : Removes intermediate and executable files.
<br />
<br />

##### ASSIGNMENT1 : SYNTAX TREE CREATION AND EVALUATION
_Directory_ : compiler      (_src : source files_ , _include : header files_ , _tag : assignment-1_)
<br /> 
Syntax tree with operators as non-terminal nodes and operands(numbers) as terminal nodes is created.
<br />By post-order traversal of expression tree, expression can be easily evaluated.
<br />
<br />
_Instructions to run_
<br />
`make` : To compile the compiler source files present in _'src'_ and create an executable `compiler` to run in _'bin'_ directory.
<br />
`make tests` : To run any testcase.
<br />Sample test cases are provided. (test2.txt: division by zero expr is present)
<br />When asked for a filename to test, provide testcase file which is present in _'testcases'_ directory only.
<br />
`make clean` : Removes intermediate and executable file.
<br />
<br />

##### ASSIGNMENT2 : SUPPORT FOR DECLARATION , SYMBOL TABLE , FUNCTION CALLS 
_Directory_ : compiler      (_src : source files_ , _include : header files_ , _tag : assignment-2_)

<br />* Symbol table which stores the information related to variables like : (type , value , undefined or declared) is created.
<br />* Declaration for integer variables is allowed.
<br />* Function call (print) is supported with inputs as variables or expressions containing variables and numbers. (_var : only integers allowed , numbers : int / double_)
<br />
<br />
_Instructions to run_
<br />
`make` : To compile the compiler source files present in _'src'_ and create an executable `compiler` to run in _'bin'_ directory.
<br />
`make tests` : To run any testcase.
<br />Sample test cases are provided. 
<br />When asked for a filename to test, provide testcase file which is present in _'testcases'_ directory only.
<br /> * _`test1.txt`_ : 
> All statements syntax is correct. 
<br />Printing undefined variable as UNDEF
<br />Division by zero error.
>
<br /> * _`test2.txt`_ : 
> Multiple declaration of integer statements is taken. 
<br /> Var is assigned an double which it truncates to integer but expression can contain number which is type of double.
<br /> Print statement results difference of integer containing double and expression containing double.
> 
<br /> * _`test3.txt`_ : Re-declaration of variable.
<br /> * _`test4.txt`_ : Undeclared variable in assignment statements.
<br /> * _`test5.txt`_ : Undeclared variable in function call print statements.

<br /> `make clean` : Removes intermediate and executable file.

