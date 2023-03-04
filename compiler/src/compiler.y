%{
#include<stdio.h>
#include "syntax_tree.h"
int yylex();
void main();
void yyerror();
void warning();
%}

	/* type of YYSTYPE */
%union {
	double numValue;
	struct node* ptr;
}

%token NUMBER PLUS SUB MUL DIV RET 
%left PLUS SUB  /* left associative */
%left MUL DIV  	/* left associative */
%left '(' ')'	/* higher precedence compared to plus,mul,div,mul */
  
%%
list: 	  /* Parser: Productions */
	| list RET
	| list expr RET    { printf("\tResult = %.8g\n", evaluateSyntaxTree($<ptr>2)); }
	;
expr: 	expr PLUS expr    { $<ptr>$ = createNode(0 , 0 , '+' , $<ptr>1 , $<ptr>3 ); }
	|expr SUB expr    { $<ptr>$ = createNode(0 , 0 , '-' , $<ptr>1 , $<ptr>3 ); }
	|expr MUL expr    { $<ptr>$ = createNode(0 , 0 , '*' , $<ptr>1 , $<ptr>3 ); }
	|expr DIV expr    { $<ptr>$ = createNode(0 , 0 , '/' , $<ptr>1 , $<ptr>3 ); }
	|'(' expr ')'     { $<ptr>$ = $<ptr>2;}
	|NUMBER           { $<ptr>$ = createNode(1 , $<numValue>1 , 'o' , NULL , NULL);}
 	;
  
%%

	/* end of grammar */
#include <ctype.h>
int lineno = 1 ;
char	*progname;	/* for error messages */

void main(int argc, char *argv[])
{  
	progname = argv[0];
	yyparse();
}
  
void yyerror(char *s)
{
   warning(s, (char *) 0);
}

void warning(char *s, char *t)	/* print warning message */
{
	fprintf(stderr, "%s: %s", progname, s);
	if (t)
		fprintf(stderr, " %s", t);
	fprintf(stderr, " near line %d\n", lineno);
}


