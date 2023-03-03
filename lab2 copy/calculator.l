%{
#include <stdio.h>
#include "syntax_tree.h"
#include "y.tab.h"
extern YYSTYPE yylval ;
extern int lineno ;
%}
%%
[0-9]+	{ yylval.numValue = atoi(yytext) ; return NUMBER ; }
[0-9]+"."[0-9]+ {yylval.numValue = atof(yytext); return NUMBER;}
[ \t] 	;
<<EOF>>         {yywrap(); return END;}
[\n] 	{ lineno++ ; return RET ; }
[+]	{ return PLUS ;}
[-]	{  return SUB ;}
[*]	{  return MUL ;}
[/] 	{ return DIV ;}
[=] { return ASSIGN; }
[;] { return SEMI_COLON; }
"begindecl"    { return BEGINDECL; } 
"enddecl"      { return ENDDECL; } 
"integer"      { return INTEGERDECL; } 
"print"        { return PRINT_FN ; }
[a-zA-Z]+[A-Za-z0-9]*      { yylval.var_name = yytext ;  return VAR;} 
.						  { return yytext[0]; }
%%

int yywrap()
{
	return 1;
}
