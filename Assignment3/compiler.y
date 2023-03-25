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
#include<vector>
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
	int numValue;
	char* var_name;
	struct expr_node* expr_ptr ;
	struct decl_node* decl_node_ptr ;
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
// %type <expr_ptr> func_call 
%type <expr_ptr> var_expr
%type <expr_ptr> str_expr
%type <expr_ptr> param_list
%type <expr_ptr> param_list1
%type <expr_ptr> para

%type <stmt_list_ptr> Prog
%type <stmt_list_ptr> MainBlock

%type <stmt_list_ptr> statement
%type <stmt_list_ptr> stmt_list
%type <stmt_list_ptr> assign_stmt
%type <stmt_list_ptr> read_stmt
%type <stmt_list_ptr> write_stmt

%type <stmt_list_ptr> Gdecl_sec
%type <stmt_list_ptr> Gdecl_list 
%type <stmt_list_ptr> Gdecl
%type <numValue> ret_type
%type <decl_node_ptr> Glist
%type <decl_node_ptr> Gid

%%

Prog	:	Gdecl_sec MainBlock
	// Gdecl_sec Fdef_sec MainBlock
	;
	
Gdecl_sec:	DECL Gdecl_list ENDDECL { $$ = $2 ; }
	;
	
Gdecl_list: /*	NULL */			{ $$ = create_Enddecl_Stmt(GLOBAL_SCOPE); }
		| 	Gdecl Gdecl_list 	{ $$ = $1; $$->next = $2; }
		;
	
Gdecl 	:	ret_type Glist ';'	{ $$ = create_Decl_Stmt(GLOBAL_SCOPE , $1 , $2); }
		;
	
ret_type:	T_INT	{ $$ = 0 ;}
		|	T_BOOL 	{ $$ = 1 ;}
		;
	
Glist 	:	Gid					{ $$ = $1; }
		// | 	func 
		|	Gid ',' Glist 		{ $1->next = $3; $$ = $1 ;}
		// |	func ',' Glist
		;

Gid	:	VAR					 { $$ = createDecl_Node(VAR_NODE , $1); }
		|	Gid '[' NUM ']'	 
		{
			if($1->decl_type == VAR_NODE)
			{
				$1->decl_type = VAR_ARR_NODE ;
			}
			($1->index_list).push_back($3) ;
			$$ =$1;
		}
		;
		
	// doubt in arg_list structure
// func 	:	VAR '(' arg_list ')' 					{ 					}
// 	;
		
// arg_list:	
// 	|	arg_list1
// 	;
	
// arg_list1:	arg_list1 ';' arg		
// 	|	arg
// 	;
	
// arg 	:	arg_type var_list	
// 	;
	
// arg_type:	T_INT		 {  }
// 	|	T_BOOL 		 { }
	// ;

// var_list:	VAR 		 { }
// 	|	VAR ',' var_list { 	}
// 	;
	
// Fdef_sec:	
// 	|	Fdef_sec Fdef
// 	;
	
// Fdef	:	func_ret_type func_name '(' FargList ')' '{' Ldecl_sec BEG stmt_list ret_stmt END '}'	{	 				}
// 	;
	
// func_ret_type:	T_INT		{ }
// 	|	T_BOOL		{ }
// 	;
		
// func_name:	VAR		{ 					}
// 	;
	
// FargList:	arg_list	{ 					}
// 	;

// ret_stmt:	RETURN expr ';'	{ }
// 	;
		
// MainBlock: 	func_ret_type main '('')''{' Ldecl_sec BEG stmt_list ret_stmt END  '}'		{  }
				  
// 	;
	
// main	:	MAIN		{ 					}
// 	;
		
// Ldecl_sec:	DECL Ldecl_list ENDDECL	{}
// 	;
// Ldecl_list:		
// 	|	Ldecl Ldecl_list
// 	;
// Ldecl	:	type Lid_list ';'		
// 	;
// type	:	T_INT			{ }
// 	|	T_BOOL			{ }
// 	;
// Lid_list:	Lid
// 	|	Lid ',' Lid_list
// 	;
	
// Lid	:	VAR			{ 						}
// 	;

stmt_list:	/* NULL {  } */		
	|	statement stmt_list	{ $1 -> next = $2 ; $$ = $1 ; }
	// |	error ';' 		{  }
	;
statement:	assign_stmt  ';'	{ $$ = $1; }
		|	read_stmt ';'		{ $$ = $1; }
		|	write_stmt ';'		{ $$ = $1; }
		|	cond_stmt 			{ $$ = $1; }
		// |	func_stmt ';'		{ $$ = $1; }
	;

read_stmt:	READ '(' var_expr ')' 		{ $$ = create_Stmt(READ , $3 , NULL); }
	;

write_stmt:	WRITE '(' expr ')'		 	{ $$ = create_Stmt(WRITE , $3 , NULL);}
	 | WRITE '(''"' str_expr '"'')'     { $$ = create_Stmt(WRITE , $4 , NULL);}
	;

assign_stmt:	var_expr '=' expr 		{ $$ = create_Stmt(ASSIGN , $1 , $2); }
	;

// cond_stmt:	IF expr THEN stmt_list ENDIF 	{ 						}
// 	|	IF expr THEN stmt_list ELSE stmt_list ENDIF 	{ 						}
// 	|	WHILE expr DO stmt_list ENDWHILE ';'{ 						}
// 	//  |    FOR '(' assign_stmt  ';'  expr ';'  assign_stmt ')' '{' stmt_list '}'                                             {                                                 }
// 	;

// func_stmt:	func_call 		{ 						}
	// ;
	
// func_call:	VAR '(' param_list ')'		{ $$ = createExpr_Node(FUNCTION_CALL , NULL , NULL , '\0' , 0 , $1 , NULL); $$->params = $3 -> params; }
// 	;
	
// param_list:				
// 	|	param_list1						{ $$ = $1 ; }
// 	;
	
// param_list1:	para					{ $$ = $1 ; $1 -> params = NULL; }	
// 	|	para ',' param_list1			{ $1 -> params = $3 ; $$ = $1  ; }
// 	;

// para	:	expr						{ $$ = $1 ; }
	// ;

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
	// |	func_call						{ $$ = $1; } 
	;

str_expr : VAR           { $$ = createExpr_Node(STRING_VAR , NULL , NULL , '\0' , 0 , true , $1); }
         | VAR str_expr  { $$ = createExpr_Node(STRING_VAR , NULL , $2 , '\0' , 0 , true , $1); }
         ;

var_expr:	VAR					{ $$ = createExpr_Node(VARIABLE , NULL , NULL , '\0' , 0 , true , $1); }
	|	var_expr '[' expr ']'	
	{ 	
		if($1 -> type == VARIABLE) 
		{
			$1 -> type = ARRAY_ELEMENT ;
		}
		($1->index_list).push_back($3) ;
		$$ =$1;
	}
	;
%%
