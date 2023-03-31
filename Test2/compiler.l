
%{
#include <stdio.h>
#include <iostream>
using namespace std;
#include "syntax_tree.h"
#include "y.tab.h"
extern int 	lineno;
int     wflag;
extern YYSTYPE yylval;
%}

%option noyywrap
digit	[0-9]
letter	[a-zA-Z]

%%
"main"    	                    {yylval.var_name =strdup(yytext) ; return MAIN;}                            			
"return"	                    {return RETURN;}                            		
"begin"		                    {return BEG;}                                        
"end"		                    {return END;}                                    
"read"		                    {return READ;}                                	
"write"		                    {return WRITE;}                            		
"integer"	                    {return T_INT;}                            
"boolean"	                    {return T_BOOL;}                            	
"decl"	                        {return DECL;}                        
"enddecl"	                    {return ENDDECL;}                            
"if"		                    {return IF;}                            
"then"		                    {return THEN;}                            
"else"		                    {return ELSE;}                
"endif"		                    {return ENDIF;}                
"do"		                    {return DO;}                
"while"		                    {return WHILE;}                
"endwhile"	                    {return ENDWHILE;}                	
"=="		                    {return EQUALEQUAL;}                
"<="		                    {return LESSTHANOREQUAL;}                		
">="		                    {return GREATERTHANOREQUAL;}                		
"!="		                    {return NOTEQUAL;}                			             		
"true"		                    {return T;}                		
"false"		                    {return F;}                		
{digit}+	                    {yylval.numValue = atoi(yytext) ; return NUM;}
{letter}({letter}|{digit})*	 	{yylval.var_name =strdup(yytext) ; return VAR;}
[-+*/=()><;,\{\}\[\]\%]         {return yytext[0];}
["]                             {return yytext[0];}                          
[\n]                            {lineno++;}
[ \t]				            ;
    // <<EOF>>                         {yywrap(); return 0;}
.						        {return yytext[0];}

%%

// int yywrap()
// {
// 	return 1;
// }