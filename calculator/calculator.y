%{
#define	YYSTYPE	double
#include<stdio.h>
int yylex();
void main();
void yyerror();
void warning();
%}
%token NUMBER PLUS SUB MUL DIV RET 
%left PLUS SUB  /* left associative, same precedence */
%left MUL DIV  	/* left assoc., higher precedence */
%left '(' ')'
  
%%
list: 	  /* Parser: Productions */
	| list RET
	| list expr RET    { printf("\tResult = %.8g\n", $2); }
	;
expr: 	expr PLUS expr { $$ = $1 + $3;} 
	|expr SUB expr { $$ = $1 - $3;}
	|expr MUL expr { $$ = $1 * $3;}
	|expr DIV expr { $$ = $1 / $3;}
	|'(' expr ')' { $$ = $2;}
	|NUMBER {$$ = $1;}
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

