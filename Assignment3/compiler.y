/*
 *   This file is part of SIL Compiler.
 *
 *  SIL Compiler is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  SIL Compiler is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with SIL Compiler.  If not, see <http://www.gnu.org/licenses/>.
 */

%{	
#include <stdio.h>
#include<string.h>
#include<iostream>
using namespace std;
#include "syntax_tree.h"
#include "sym_table.h"
	// #include "AbsSynTree.h"
	// #define YYSTYPE double
int yylex();
	// void yyerror( char* );
int main(int argc, char *argv[]);
void yyerror(char const *s);
void warning(char const *s, char const *t);
	// int i;	
%}

%union {
	double numValue;
	char* var_name;
	struct expr_node* expr_ptr ;
	struct stmt_list* stmt_list_ptr ;
}

%token BEG END
%token T_INT T_BOOL
%token READ WRITE
%token DECL ENDDECL
%token <var_name> VAR 
%token <numValue> NUM
%token IF THEN ELSE ENDIF
%token LOGICAL_AND LOGICAL_NOT LOGICAL_OR
%token EQUALEQUAL LESSTHANOREQUAL GREATERTHANOREQUAL NOTEQUAL
%token WHILE DO ENDWHILE FOR 
%token T F 
%token MAIN RETURN


%left '<' '>'
%left EQUALEQUAL LESSTHANOREQUAL GREATERTHANOREQUAL NOTEQUAL
%left '+' '-'
%left '*' '/'
%left '%'
%left LOGICAL_AND LOGICAL_OR
%left LOGICAL_NOT

	/* adding types */
%type <expr_ptr> expr
%type <expr_ptr> func_call 
%type <expr_ptr> var_expr
%type <expr_ptr> param_list
%type <expr_ptr> param_list1
%type <expr_ptr> para

%%

Prog	:	Gdecl_sec Fdef_sec MainBlock
	;
	
Gdecl_sec:	DECL Gdecl_list ENDDECL{}
	;
	
Gdecl_list: 
	| 	Gdecl Gdecl_list
	;
	
Gdecl 	:	ret_type Glist ';'
	;
	
ret_type:	T_INT		{ }
	|	T_BOOL 		{ }
	;
	
Glist 	:	Gid
	| 	func 
	|	Gid ',' Glist 
	|	func ',' Glist
	;

Gid	:	VAR		{ 				}
	|	Gid '[' NUM ']'	{                                                   }		;
	
	// doubt in arg_list structure
func 	:	VAR '(' arg_list ')' 					{ 					}
	;
		
arg_list:	
	|	arg_list1
	;
	
arg_list1:	arg_list1 ';' arg		
	|	arg
	;
	
arg 	:	arg_type var_list	
	;
	
arg_type:	T_INT		 {  }
	|	T_BOOL 		 { }
	;
var_list:	VAR 		 { }
	|	VAR ',' var_list { 	}
	;
	
Fdef_sec:	
	|	Fdef_sec Fdef
	;
	
Fdef	:	func_ret_type func_name '(' FargList ')' '{' Ldecl_sec BEG stmt_list ret_stmt END '}'	{	 				}
	;
	
func_ret_type:	T_INT		{ }
	|	T_BOOL		{ }
	;
		
func_name:	VAR		{ 					}
	;
	
FargList:	arg_list	{ 					}
	;
ret_stmt:	RETURN expr ';'	{ 					}
	;
		
MainBlock: 	func_ret_type main '('')''{' Ldecl_sec BEG stmt_list ret_stmt END  '}'		{ 				  	  }
				  
	;
	
main	:	MAIN		{ 					}
	;
		
Ldecl_sec:	DECL Ldecl_list ENDDECL	{}
	;
Ldecl_list:		
	|	Ldecl Ldecl_list
	;
Ldecl	:	type Lid_list ';'		
	;
type	:	T_INT			{ }
	|	T_BOOL			{ }
	;
Lid_list:	Lid
	|	Lid ',' Lid_list
	;
	
Lid	:	VAR			{ 						}
	;
stmt_list:	/* NULL */		{  }
	|	statement stmt_list	{						}
	|	error ';' 		{  }
	;
statement:	assign_stmt  ';'		{ 							 }
	|	read_stmt ';'		{ }
	|	write_stmt ';'		{ }
	|	cond_stmt 		{ }
	|	func_stmt ';'		{ }
	;
read_stmt:	READ '(' var_expr ')' {						 }
	;
write_stmt:	WRITE '(' expr ')' 	{  }
	 | WRITE '(''"' str_expr '"'')'      { }
	;

assign_stmt:	var_expr '=' expr 	{ 						}
	;
cond_stmt:	IF expr THEN stmt_list ENDIF 	{ 						}
	|	IF expr THEN stmt_list ELSE stmt_list ENDIF 	{ 						}
	|	WHILE expr DO stmt_list ENDWHILE ';'{ 						}
	//  |    FOR '(' assign_stmt  ';'  expr ';'  assign_stmt ')' '{' stmt_list '}'                                             {                                                 }
	;

func_stmt:	func_call 		{ 						}
	;
	
func_call:	VAR '(' param_list ')'		{ $$ = createExpr_Node(FUNCTION_CALL , NULL , NULL , '\0' , 0 , $1 , NULL); $$->params = $3 -> params; }
	;
	
param_list:				
	|	param_list1						{ $$ = $1 ; }
	;
	
param_list1:	para					{ $$ = $1 ; $1 -> params = NULL; }	
	|	para ',' param_list1			{ $1 -> params = $3 ; $$ = $1  ; }
	;

para	:	expr						{ $$ = $1 ; }
	;

expr	:	NUM 						{ $$ = createExpr_Node(INTEGER , NULL , NULL , '\0' , $1); }
	|	'-' NUM							{ $$ = createExpr_Node(INTEGER , NULL , NULL , '\0' , -1*$1); }
	|	var_expr						{ $$ = $1; }
	|	T								{ $$ = createExpr_Node(BOOL , NULL , NULL , '\0' , 0 , true); }
	|	F								{ $$ = createExpr_Node(BOOL , NULL , NULL , '\0' , 0 , false); }
	|	'(' expr ')'					{ $$ = $2; }
	|	expr '+' expr 					{ $$ = createExpr_Node(OP , $1 , $3 , '+'); }
	|	expr '-' expr	 				{ $$ = createExpr_Node(OP , $1 , $3 , '-'); }
	|	expr '*' expr 					{ $$ = createExpr_Node(OP , $1 , $3 , '*'); }
	|	expr '/' expr 					{ $$ = createExpr_Node(OP , $1 , $3 , '/'); }
	|	expr '%' expr 					{ $$ = createExpr_Node(OP , $1 , $3 , '%'); }		
	|	expr '<' expr					{ $$ = createExpr_Node(OP , $1 , $3 , '<'); }	
	|	expr '>' expr					{ $$ = createExpr_Node(OP , $1 , $3 , '>'); }	
	|	expr GREATERTHANOREQUAL expr	{ $$ = createExpr_Node(OP , $1 , $3 , '>='); }	
	|	expr LESSTHANOREQUAL expr		{ $$ = createExpr_Node(OP , $1 , $3 , '<='); }	
	|	expr NOTEQUAL expr				{ $$ = createExpr_Node(OP , $1 , $3 , '!='); }	
	|	expr EQUALEQUAL expr			{ $$ = createExpr_Node(OP , $1 , $3 , '=='); }	
	|	LOGICAL_NOT expr				{ $$ = createExpr_Node(OP , NULL , $3 , '!');}	
	|	expr LOGICAL_AND expr			{ $$ = createExpr_Node(OP , $1 , $3 , '&&'); }	
	|	expr LOGICAL_OR expr			{ $$ = createExpr_Node(OP , $1 , $3 , '||'); }	
	|	func_call						{ $$ = $1; } 
	;
str_expr :  VAR                       {}
              | str_expr VAR   { }
            ;

var_expr:	VAR	{ 				  }
	|	var_expr '[' expr ']'	{                                                 }
	;
%%
