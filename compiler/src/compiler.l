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
<<EOF>>	{ return 0;}
.	return yytext[0];
%%

int yywrap()
{
	return 1;
}

