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
int lineno = 1 ;
int error_check_var = 0;
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
%token <var_name> MAIN 
%token IF THEN ELSE ENDIF
%token LOGICAL_AND LOGICAL_NOT LOGICAL_OR
%token EQUALEQUAL LESSTHANOREQUAL GREATERTHANOREQUAL NOTEQUAL
%token WHILE DO ENDWHILE FOR 
%token T F 
%token RETURN


%left '<' '>'
%left EQUALEQUAL LESSTHANOREQUAL GREATERTHANOREQUAL NOTEQUAL
%left '+' '-'
%left '*' '/'
%left '%'
%left LOGICAL_AND LOGICAL_OR
%left LOGICAL_NOT

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

%type <decl_node_ptr> func
%type <numValue> arg_type
%type <stmt_list_ptr> arg_list 
%type <stmt_list_ptr> arg_list1
%type <stmt_list_ptr> arg
%type <decl_node_ptr> var_list

%type <stmt_list_ptr> Fdef_sec
%type <stmt_list_ptr> Fdef
%type <var_name> func_name
%type <stmt_list_ptr> Ldecl_sec
%type <stmt_list_ptr> Ldecl_list
%type <stmt_list_ptr> Ldecl
%type <decl_node_ptr> Lid_list
%type <decl_node_ptr> Lid
%type <numValue> type

%type <stmt_list_ptr> func_stmt
%type <expr_ptr> func_call
%type <expr_ptr> param_list
%type <expr_ptr> param_list1
%type <expr_ptr> para

%type <var_name> main

%type <numValue> func_ret_type
%type <stmt_list_ptr> ret_stmt

%type <stmt_list_ptr> FargList 

%%

Prog	:	Gdecl_sec Fdef_sec MainBlock
		{ 
			if ( $1 != NULL && $2 != NULL )
			{
				struct stmt_list* temp = $1 ;

				while(temp->type != END_DECL)
				{
					temp = temp->next;
				}
				temp -> next = $2;
				$$ = $1 ;

				temp = $2;
				while(temp->next != NULL)
				{
					temp = temp->next;
				}
				temp -> next = $3;
			}
			else if( $1 == 	NULL && $2 == NULL )
			{
				$$ = $3 ;
			}
			else if ( $1 == NULL )
			{
				cerr << "ERROR: Undeclared functions in global declaration block" << endl;
				exit(0);
			}
			else if( $2 == 	NULL)
			{
				$$ = $1 ;

				struct stmt_list* temp = $1 ;

				while(temp->type != END_DECL)
				{
					temp = temp->next;
				}
				
				temp -> next = $3;
			}

			// AST
			ast_printing($$);

			// SEMANTIC ERROR CHECKING
			cout << endl;
			cout << endl;
			semantic_error_checking($$ , "global");

			if(error_check_var == 1)
			{
				exit(0);
			}
			
			// // EVALUATION PART
			// evaluate_program($$)

			// // PRINTING SYMBOL TABLE VALUES
			// cout << "\n\nSymbol Table Values" << endl;
			// cout << "-------------------" << endl;
			// print_symbol_values();
		}
		;
	
Gdecl_sec:	DECL Gdecl_list ENDDECL 
		 { 
			if( $2->type == END_DECL )
			{
				$$ = NULL;
			}
			else
			{
				$$ = create_decl_start_stmt();
				$$ -> next = $2 ; 
			}
		 }
		 ;
	
Gdecl_list: /*	NULL */			{ $$ = create_Enddecl_Stmt(0); }
		| 	Gdecl Gdecl_list 	{ $1->next = $2; $$ = $1; }
		;
	
Gdecl 	:	ret_type Glist ';'	{ $$ = create_Decl_Stmt($1 , $2 , lineno); }
		;
	
ret_type:	T_INT	{ $$ = 0 ;}
		|	T_BOOL 	{ $$ = 1 ;}
		;
	
Glist 	:	Gid					{ $$ = $1; }
		| 	func 				{ $$ = $1; }
		|	Gid ',' Glist 		{ $1->next = $3; $$ = $1 ; }
		|	func ',' Glist		{ $1->next = $3; $$ = $1 ; }
		;

Gid	:	VAR					 { $$ = createDecl_Node(VAR_NODE , $1); }
		|	VAR '[' NUM ']'	 { $$ = createDecl_Node(VAR_ARR_NODE , $1 , $3); }		
		;


	// doubt in arg_list structure
func 	:	VAR '(' arg_list ')' 	
		{ if ($3 == NULL)
		  {
			 $$ = createDecl_Node(FUNC_NODE , $1 , 0 , NULL);
		  }
		  else
		  {
			$$ = createDecl_Node(FUNC_NODE , $1 , 0 , $3);
		  } 					
		}
	;
		
arg_list: /* NULL */ 	{ $$ = NULL ; }	
	|	arg_list1		{ $$ = $1 ; }
	;
	
arg_list1:	arg_list1 ';' arg		
			{ 
				struct stmt_list* temp = $1;

				while(temp -> next != NULL)
				{
					temp = temp -> next;
				}
				temp -> next = $3;
				$$ = $1 ;
			}	
			|	arg		{ $$ = $1 ; }
			;
	
arg :	arg_type var_list		{ $$ = create_Decl_Stmt($1 , $2 , lineno); }
	;
	
arg_type:	T_INT		{ $$ = 0; }
		|	T_BOOL 		{ $$ = 1; }
		;

var_list:	VAR 		 			{ $$ = createDecl_Node(VAR_NODE , $1);  }
	|	VAR ',' var_list 			{ struct decl_node* temp = createDecl_Node(VAR_NODE , $1);	temp->next = $3 ; $$ = temp;}
	;

Fdef_sec: /* NULL */ 			{ $$ = NULL ; }
		|	Fdef_sec Fdef		
		{ 
			struct stmt_list* temp = $1;
			if(temp == NULL)
			{
				$$ = $2; $$->next = NULL;
			}
			else
			{
				while(temp -> next != NULL)
				{
					temp = temp -> next;
				}
				temp -> next = $2;
				$$ = $1 ; 
			}
			
		}
		;
	
Fdef:	func_ret_type func_name '(' FargList ')' '{' Ldecl_sec BEG stmt_list ret_stmt END '}'	
		{	$$ = create_function( $1 , $2 , $4 , $7 , $9 , $10 , $4->line_num); }
	;
	
func_ret_type:	T_INT		{ $$ = 0 ; }
			 |	T_BOOL		{ $$ = 1 ; }
			 ;
		
func_name:	VAR		{ $$ = $1 ; }
		 ;
	
FargList:	arg_list	{ $$ = $1 ; $$->line_num = lineno; }
		;

ret_stmt:	RETURN expr ';'		{ $$ = create_return_stmt($2 , lineno); }
		;
		

MainBlock: 	func_ret_type main '('')''{' Ldecl_sec BEG stmt_list ret_stmt END  '}'		
			{ $$ = create_function($1 , $2 , NULL , $6 , $8 , $9 , lineno); }
		 ;
	
main	:	MAIN	{ $$ = $1; }
		;
		
Ldecl_sec:	DECL Ldecl_list ENDDECL		
		 { 
			if( $2->type == END_DECL )
			{
				$$ = NULL;
			}
			else
			{
				$$ = create_decl_start_stmt();
				$$ -> next = $2 ; 
			}
		 }
		 ;

Ldecl_list:	/* NULL */				{ $$ = create_Enddecl_Stmt(1); }	
		  |	Ldecl Ldecl_list		{ $$ = $1; $$->next = $2; }
		  ;

Ldecl	:	type Lid_list ';'		{ $$ = create_Decl_Stmt($1 , $2 , lineno); }
		;

type	:	T_INT		{ $$ = 0 ; }
		|	T_BOOL		{ $$ = 1 ; }
	 	;

Lid_list:	Lid						{ $$ = $1; }
		|	Lid ',' Lid_list		{ $1->next = $3; $$ = $1 ; }
		;
	
Lid	:	VAR		{ $$ = createDecl_Node(VAR_NODE , $1); }
	;

stmt_list:	/* NULL {  } */				{ $$ = create_unused_stmt(); }	
		 |	statement stmt_list			{ $1 -> next = $2 ; $$ = $1 ; }
		 // |	error ';' 		{  }
		 ;

statement:	assign_stmt  ';'	{ $$ = $1; }
		|	read_stmt ';'		{ $$ = $1; }
		|	write_stmt ';'		{ $$ = $1; }
		|	cond_stmt  ';'		{ $$ = $1; }
		|	func_stmt ';'		{ $$ = $1; }
		;
 
read_stmt:	READ '(' var_expr ')' 		{ $$ = create_Stmt(READ_STMT , lineno , $3 , NULL) ; }
		 ;

write_stmt:	WRITE '(' expr ')'		 		 { $$ = create_Stmt(WRITE_STMT , lineno , $3 , NULL); }
	 	  | WRITE '(''"' str_expr '"'')'     { $$ = create_Stmt(WRITE_STMT , lineno , $4 , NULL); }
		  ;

assign_stmt:	var_expr '=' expr 		{ $$ = create_Stmt(ASSIGN ,lineno ,  $1 , $3); }
	 	   ;

cond_stmt:	IF expr THEN stmt_list ENDIF 				{ $$ = create_Condt_Stmt(IF_CONDT , $4->line_num , $2 , $4 , NULL); }
	|	IF expr THEN stmt_list ELSE stmt_list ENDIF 	{ $$ = create_Condt_Stmt(IF_ELSE , $4->line_num , $2 , $4 , $6); }
	|	WHILE expr DO stmt_list ENDWHILE 			{ $$ = create_Condt_Stmt(WHILE_CONDT , $4->line_num , $2 , $4 , NULL); }
	//  |    FOR '(' assign_stmt  ';'  expr ';'  assign_stmt ')' '{' stmt_list '}'     {     }
	;

func_stmt:	func_call 					{ $$ = create_Stmt(FUNC_CALL , lineno , $1 , NULL); }
		 ;
	
func_call:	VAR '(' param_list ')'		
			{ 
				if ($3 == NULL)
				{
					$$ = createExpr_Node(FUNCTION_CALL , NULL , NULL , PLUS_OP , 0 , true , $1 , NULL); 
				}
				else
				{
					$$ = createExpr_Node(FUNCTION_CALL , NULL , NULL , PLUS_OP , 0 , true , $1 , $3); 
				}
			}
		 ;
	
param_list:	/* NULL */ 					{ $$ = NULL ; }			
	|	param_list1						{ $$ = $1 ; }
	;

param_list1:	para					{ $$ = $1 ; $1 -> params = NULL; }	
	|	para ',' param_list1			{ $1 -> params = $3 ; $$ = $1  ; }
	;

para	:	expr						{ $$ = $1 ; }
	;

expr	:	NUM 						{ $$ = createExpr_Node(INTEGER , NULL , NULL , PLUS_OP , $1); }
	|	'-' NUM							{ $$ = createExpr_Node(INTEGER , NULL , NULL , PLUS_OP , -1*$2); }
	|	var_expr						{ $$ = $1; }
	|	T								{ $$ = createExpr_Node(BOOL , NULL , NULL , PLUS_OP , 1 , true); }
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
	|	LOGICAL_NOT expr				{ $$ = createExpr_Node(OP , NULL , $2 , LOGICAL_NOT_OP); }	
	|	expr LOGICAL_AND expr			{ $$ = createExpr_Node(OP , $1 , $3 , LOGICAL_AND_OP); }	
	|	expr LOGICAL_OR expr			{ $$ = createExpr_Node(OP , $1 , $3 , LOGICAL_OR_OP); }	
	|	func_call						{ $$ = $1; } 
	;

str_expr : VAR           { $$ = createExpr_Node(STRING_VAR , NULL , NULL , PLUS_OP , 0 , true , $1); }
         | VAR str_expr  { $$ = createExpr_Node(STRING_VAR , NULL , $2 , PLUS_OP , 0 , true , $1); }
		//  | '\n' 			{}	// \n is ignored in compiler.l file itself
		 | '='			 {}
         ;

var_expr:	VAR					{ $$ = createExpr_Node(VARIABLE , NULL , NULL , PLUS_OP , 0 , true , $1); }
	|	VAR '[' expr ']'		{ $$ = createExpr_Node(ARRAY_ELEMENT , NULL , NULL , PLUS_OP , 0 , true , $1 , $3); }
	;
%%


// #include <ctype.h>
// char *progname;	/* for error messages */
// int main(int argc, char *argv[])
// {  
//     progname = argv[0];
// 	yyin = fopen(argv[1] , "r");
//     yyparse();
//     return 0;
// }

// void yyerror(char const *s)
// {
//     warning(s, (char *)0);
// }

// void warning(char const *s, char const *t)	/* print warning message */
// {
//     fprintf(stderr, "%s: %s", progname, s);
//     if (t)
//         fprintf(stderr, " %s", t);
//     fprintf(stderr, " near line %d\n", lineno);
// }

#include <stdio.h>
#include <ctype.h>
char *progname;	/* for error messages */
#include <signal.h>
#include <setjmp.h>
jmp_buf	begin;
extern FILE* yyin;

int main(int argc, char *argv[])
{  
    progname = argv[0];
	yyin = fopen(argv[1] , "r");
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