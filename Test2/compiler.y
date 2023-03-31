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
#include<stdlib.h>
#include<string.h>
#include<iostream>
#include<vector>
using namespace std;
#include "syntax_tree.h"
	// #include "sym_table.h"

	// void yyerror( char* );
int main(int argc, char *argv[]);
void yyerror(char const  *s);
// void execerror(char *s , char*t);
void warning(char const *s, char const *t);
int yylex();
int lineno = 1 ;
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
%token MAIN 
%token IF THEN ELSE ENDIF
%token EQUALEQUAL LESSTHANOREQUAL GREATERTHANOREQUAL NOTEQUAL
%token WHILE DO ENDWHILE FOR 
%token T F 
%token RETURN


%left '<' '>'
%left EQUALEQUAL LESSTHANOREQUAL GREATERTHANOREQUAL NOTEQUAL
%left '+' '-'
%left '*' '/'
%left '%'

	/* adding types */
%type <expr_ptr> expr
%type <expr_ptr> var_expr
%type <expr_ptr> str_expr

%type <stmt_list_ptr> Prog
%type <stmt_list_ptr> MainBlock

%type <stmt_list_ptr> statement
%type <stmt_list_ptr> stmt_list
%type <stmt_list_ptr> assign_stmt
%type <stmt_list_ptr> read_stmt
%type <stmt_list_ptr> write_stmt
%type <stmt_list_ptr> cond_stmt

%type <stmt_list_ptr> Gdecl_sec
%type <stmt_list_ptr> Gdecl_list 
%type <stmt_list_ptr> Gdecl
%type <numValue> ret_type
%type <decl_node_ptr> Glist
%type <decl_node_ptr> Gid

%type <numValue> func_ret_type
%type <stmt_list_ptr> ret_stmt


%%

Prog	:	Gdecl_sec MainBlock			
			{ 
			  struct stmt_list* temp = $1 ;
			  while(temp->type != ENDDECL)
			  {
				temp = temp->next;
			  }
			  temp -> next = $2;
			  $$ = $1 ;
			   
			//    ast_printing($$ , 0);
			}
	;
	
Gdecl_sec:	DECL Gdecl_list ENDDECL { $$ = $2 ; }
	;
	
Gdecl_list: /*	NULL */			{ $$ = create_Enddecl_Stmt(); }
		| 	Gdecl Gdecl_list 	{ $1->next = $2; $$ = $1; }
		;
	
Gdecl 	:	ret_type Glist ';'	{ $$ = create_Decl_Stmt($1 , $2); $$->line_num = lineno; }
		;
	
ret_type:	T_INT	{ $$ = 0 ;}
		|	T_BOOL 	{ $$ = 1 ;}
		;
	
Glist 	:	Gid					{ $$ = $1; }
		|	Gid ',' Glist 		{ $1->next = $3; $$ = $1 ; }
		;

Gid	:	VAR					 { $$ = createDecl_Node(VAR_NODE , $1 , 0); }
		|	VAR '[' NUM ']'	 { $$ = createDecl_Node(VAR_ARR_NODE , $1 , $3); }
		;
	
func_ret_type:	T_INT		{ $$ = 0 ; }
			 |	T_BOOL		{ $$ = 1 ; }
			 ;

ret_stmt:	RETURN expr ';'		{ $$ = create_return_stmt($2); $$->line_num = lineno; }
		;
		
MainBlock: 	func_ret_type MAIN '('')''{' BEG stmt_list ret_stmt END  '}'		
			{ $$ = create_Main($1 , $7 , $8);
			  $$->line_num = lineno ; }     //change required
		 ;
		
stmt_list:	/* NULL {  } */				{ $$ = create_unused_stmt(); }	
		 |	statement stmt_list			{ $1 -> next = $2 ; $$ = $1 ; }
        //  | error ';'
		 ;

statement:	assign_stmt  ';'	{ $$ = $1; }
		|	read_stmt ';'		{ $$ = $1; }
		|	write_stmt ';'		{ $$ = $1; }
		|	cond_stmt 			{ $$ = $1; }
	;

read_stmt:	READ '(' var_expr ')' 		{ $$ = create_Stmt(READ_STMT , $3 , NULL);    $$->line_num = lineno ; }
	;

write_stmt:	WRITE '(' expr ')'		 	{ $$ = create_Stmt(WRITE_STMT , $3 , NULL);   $$->line_num = lineno ; }
	 | WRITE '(''"' str_expr '"'')'     { $$ = create_Stmt(WRITE_STMT , $4 , NULL);   $$->line_num = lineno ; }
	;

assign_stmt:	var_expr '=' expr 		{ $$ = create_Stmt(ASSIGN , $1 , $3);    $$->line_num = lineno ; }
	;

cond_stmt:	IF expr THEN stmt_list ENDIF 				{ $$ = create_Condt_Stmt(IF_CONDT , $2 , $4 , NULL);    	$$->line_num = lineno ; }
	|	IF expr THEN stmt_list ELSE stmt_list ENDIF 	{ $$ = create_Condt_Stmt(IF_ELSE , $2 , $4 , $6);   $$->line_num = lineno ; }
	|	WHILE expr DO stmt_list ENDWHILE ';'			{ $$ = create_Condt_Stmt(WHILE_CONDT , $2 , $4 , NULL);   $$->line_num = lineno ; }
	;

expr	:	NUM 						{ $$ = createExpr_Node(INTEGER , NULL , NULL , PLUS_OP , $1); }
	|	'-' NUM							{ $$ = createExpr_Node(INTEGER , NULL , NULL , PLUS_OP , -1*$2); }
	|	var_expr						{ $$ = $1; }
	|	T								{ $$ = createExpr_Node(BOOL , NULL , NULL , PLUS_OP , 0 , true); }
	|	F								{ $$ = createExpr_Node(BOOL , NULL , NULL , PLUS_OP , 0 , false); }
	|	'(' expr ')'					{ $$ = $2; }
	|	expr '+' expr 					{ $$ = createExpr_Node(OP , $1 , $3 , PLUS_OP); }
	|	expr '-' expr	 				{ $$ = createExpr_Node(OP , $1 , $3 , SUB_OP); }
	|	expr '*' expr 					{ $$ = createExpr_Node(OP , $1 , $3 , MUL_OP); }
	|	expr '/' expr 					{ $$ = createExpr_Node(OP , $1 , $3 , DIV_OP); }
	|	expr '%' expr 					{ $$ = createExpr_Node(OP , $1 , $3 , REMAINDER_OP); }		
	|	expr '<' expr					{ $$ = createExpr_Node(OP , $1 , $3 , LESSTHAN_OP); }	
	|	expr '>' expr					{ $$ = createExpr_Node(OP , $1 , $3 , GREATERTHAN_OP); }	
	|	expr GREATERTHANOREQUAL expr	{ $$ = createExpr_Node(OP , $1 , $3 , GREATERTHAN_EQUAL_OP); }	
	|	expr LESSTHANOREQUAL expr		{ $$ = createExpr_Node(OP , $1 , $3 , LESSTHAN_EQUAL_OP); }	
	|	expr NOTEQUAL expr				{ $$ = createExpr_Node(OP , $1 , $3 , NOTEQUAL_OP); }	
	|	expr EQUALEQUAL expr			{ $$ = createExpr_Node(OP , $1 , $3 , EQUALEQUAL_OP); }	
	;

str_expr : VAR           { $$ = createExpr_Node(STRING_VAR , NULL , NULL , PLUS_OP , 0 , true , $1); }
         | VAR str_expr  { $$ = createExpr_Node(STRING_VAR , NULL , $2 , PLUS_OP , 0 , true , $1); }
         ;

var_expr:	VAR					{ $$ = createExpr_Node(VARIABLE , NULL , NULL , PLUS_OP , 0 , true , $1); }
	|	VAR '[' expr ']'	    { $$ = createExpr_Node(ARRAY_ELEMENT , NULL , NULL , PLUS_OP , 0 , true , $1 , $3); }
	;
%%

#include <stdio.h>
#include <ctype.h>
char *progname;	/* for error messages */
#include <signal.h>
#include <setjmp.h>
jmp_buf	begin;

int main(int argc, char *argv[])
{  
    progname = argv[0];
    // setjmp(begin);
    yyparse();
    return 0;
}

void yyerror(char const *s)
{
    warning(s, (char *)0);
}

// void execerror(char *s, char *t)	
// {
// 	warning(s, t);
//     // longjmp(begin, 0);
// }

void warning(char const *s, char const *t)	/* print warning message */
{
    fprintf(stderr, "%s: %s", progname, s);
    if (t && *t)
        fprintf(stderr, " %s", t);
    fprintf(stderr, " near line %d\n", lineno);
}