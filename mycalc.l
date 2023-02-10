%{
#include <stdio.h>
#include "y.tab.h"
extern YYSTYPE yylval ;
extern int lineno ;
%}
%%
[0-9]+	{ yylval.numValue = atoi(yytext) ; return NUMBER ; }
[0-9]+"."[0-9]+ {yylval.numValue = atof(yytext); return NUMBER;}
[ \t] 	;
[\n] 	{ lineno++ ; return RET ; }
[+]	{ return PLUS ;}
[-]	{  return SUB ;}
[*]	{  return MUL ;}
[/] 	{ return DIV ;}
	/* [=] { return ASSIGN; }*/
<<EOF>>	{ return 0;}
	/* "begindecl"    { return BEGIN_DECL; } */
	/* "enddecl"      { return END_DECL; } */
	/* "integer"      { return INTEGER_DECL; } */
	/* [A-Za-z]+      { return VAR; }*/
.	return yytext[0];
%%

int yywrap()
{
	return 1;
}
