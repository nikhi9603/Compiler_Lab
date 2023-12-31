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
<br />
<br />

##### ASSIGNMENT3
_Directory_ : Assignment3 

**24-04-2023:** 
<br/>* Designed structures of nodes which are part of syntax tree. (Declared expression tree nodes, declaration tree nodes and various types of statement nodes. And also written functions to create such nodes.) 
<br/>* Production rule actions in yacc file were written except functions part(including main). 
<br/>* Evaluation of syntax tree , symbol table , printing AST are left. Support for functions should be done.

**31-4-2023:**
<br/>* Main function is also handled in the syntax tree creation now and conditional statements actions to their production rules were written. With this syntax tree creation is completed fully.
<br/>* Evaluation of syntax tree , symbol table , printing AST are left. Support for functions should be done.


##### TEST2
<br/>* commit - 1: starting the exam by creating directory Test2
<br/>* commit - 2: Restored the grammar , lex file from previous labs and modified the production rules and tokens
<br/>* commit - 3: Restored syntax_tree.h , syntax_tree.cpp and modified according to the changes in the grammar
<br/>* commit - 4: Created Makefile and resolved errors
<br/>* commit - 5: started abstract syntax tree printing and print function for syntax tree of expression is done
<br/>* commit - 6: AST printing code is wriiten but errors should be debugged
<br/>* commit - 7: Symbol table has been restored and complete modifications as per this grammar is not completed
<br/>* commit - 8: Few errors of abstract syntax tree were removed. 
<br/>* commit - 9: AST is working
<br/>* commit - 10: Symbol table and its operations like lookup,insert,update completed without errors. And also all semantic error checking and printing is done.
<br/> Syntax errors: Stopping at first syntax error.
<br/> If no syntax error is present in file , then AST and all semantic errors are printed 
<br/>* commit - 10: Evaluation of statements and printing of symbol table values is done. (values are not getting updated)
<br/>* commit - 11: Evaluation is working except for read statements.
<br/>* commit - 12: Changed in format of testcase file passing through the command. Added make tests in makefile and also testcases were updated in 'testcases' folder.
Eg: (executable) (input_relativ_etestcase_path) i.e ./compiler tetscases/test3.txt


##### ASSIGNMENT3 CONTINUATION:
<br/>* All the changes and assignment details are mentioned in the report of "Assignment3" folder.




