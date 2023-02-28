%{
#include<stdio.h>
#include "syntax_tree.h"
#include "sym_table.h"
int yylex();
void main();
void yyerror();
void warning();

extern struct sym_table symbol_table[100] ;
%}

	/* type of YYSTYPE */
%union {
	double numValue;
	struct node* ptr;
}

%token NUMBER PLUS SUB MUL DIV RET 
%token BEGINDECL ENDDECL VAR ASSIGN INTEGERDECL 
%left PLUS SUB  /* left associative*/
%left MUL DIV  	/* left associative */
%left '(' ')'	/* higher precedence compared to plus, mul, div, sub */
  
	/* adding types */
%type <ptr> expr
%type <numValue> NUMBER

%%
list: 	  /* Parser: Productions */
	| list RET
	| list assignStmt RET 
	| list declaration RET
	| list funStmt RET
	;
	/* | list expr RET    { printf("\tResult = %.8g\n", evaluateSyntaxTree($<ptr>2)); }
	; */
expr: expr PLUS expr  { $$ = createNode(0 , 0 , '+' , $1 , $3 ); }
	|expr SUB expr    { $$ = createNode(0 , 0 , '-' , $1 , $3 ); }
	|expr MUL expr    { $$ = createNode(0 , 0 , '*' , $1 , $3 ); }
	|expr DIV expr    { $$ = createNode(0 , 0 , '/' , $1 , $3 ); }
	|'(' expr ')'     { $$ = $2;}
	|NUMBER           { $$ = createNode(1 , $1 , 'o' , NULL , NULL);}
	|VAR 			  { }
 	;

declaration: BEGINDECL RET INTEGERDECL varlist ';' RET ENDDECL  /* call for symtable lookup and insertion if not found in symtable */	
		   ;
  	
varlist: VAR
  	   | varlist ',' VAR 
	   ;

assignStmt: VAR ASSIGN expr ';'		/* lookup + updating corresponding value field of that variable */
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

