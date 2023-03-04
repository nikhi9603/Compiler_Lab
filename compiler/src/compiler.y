%{
#include<stdio.h>
#include<string.h>
#include<iostream>
using namespace std;
#include "syntax_tree.h"
#include "sym_table.h"

int yylex();
int main(int argc, char *argv[]);
void yyerror(char const *s);
void warning(char const *s, char const *t);
%}

%union {
    double numValue;
    char *var_name;
	struct expr_node* ptr;
    struct node* node_ptr;
    struct declstmt_tree* declstmt_tree_ptr ;
    struct statement* stmt ;
    struct funcStmt_node *func_node_ptr ;
}

%token <numValue> NUMBER 
%token <var_name> VAR
%token PLUS SUB MUL DIV RET END SEMI_COLON 
%token BEGINDECL ENDDECL ASSIGN INTEGERDECL PRINT_FN

%right ASSIGN
%left PLUS SUB  /* left associative*/
%left MUL DIV  	/* left associative */
%left '(' ')'	/* higher precedence compared to plus, mul, div, sub */
  
	/* adding types */
%type <ptr> expr
%type <node_ptr> varlist
%type <declstmt_tree_ptr> declList
%type <ptr> assignStmt
%type <func_node_ptr> printStmt
%type <declstmt_tree_ptr> declaration
%type <stmt> list
%type <func_node_ptr> exprList
%type <stmt> statement

%%
program: list  {evaluate_program($1);}
        ;

list: statement    { $$ = $1 ;}
    |list statement 
    {       
      struct statement* temp = $1 ;
      while( temp -> next !=  NULL)
      {
          temp = temp -> next ;   
      }
      temp -> next = $2;
      $$ = $1 ;
    }
    

statement:  declaration { $$ = createStmt(DECL_STMT , $1 , NULL , NULL , NULL ); }
        | printStmt     { $$ = createStmt(PRINTFN_STMT , NULL , NULL , $1 , NULL); }
        | assignStmt    { $$ = createStmt(ASSIGN_STMT , NULL , $1 , NULL , NULL); }
        | statement RET { $$ = $1 ;}
        ;
    

expr: expr PLUS expr  { $$ = createExpr_Node(OP , $1 , $3 , '+'); }
	|expr SUB expr    { $$ = createExpr_Node(OP , $1 , $3 , '-'); }
	|expr MUL expr    { $$ = createExpr_Node(OP , $1 , $3 , '*'); }
	|expr DIV expr    { $$ = createExpr_Node(OP , $1 , $3 , '/'); }
	|'(' expr ')'     { $$ = $2;}
	|NUMBER           { $$ = createExpr_Node(CONSTANT , NULL , NULL , '\0' , $1); }
    |VAR              { $$ = createExpr_Node(VARIABLE , NULL , NULL , '\0' , 0 , $1); }
    ;

declaration: BEGINDECL RET declList ENDDECL  {$$ = $3 ;} 	
		   ;

declList: INTEGERDECL varlist SEMI_COLON RET    { $$ = createDeclStmt_Node(INT , $2 , NULL); }
        | declList INTEGERDECL varlist SEMI_COLON RET
            {
                struct declstmt_tree *tree = createDeclStmt_Node(INT , $3 , NULL);
                struct declstmt_tree *temp = $1 ;

                while( temp -> next !=  NULL)
                {
                    temp = temp -> next ;   
                }
                temp -> next = tree ; 
                $$ = $1 ;
            }
        ;

            
varlist: VAR                   { $$ = createNode($1 , NULL) ; }
  	   | varlist ',' VAR      
           { 
                struct node* var_list = createNode($3,NULL) ;
                struct node *temp = $1 ;
                
                while( temp -> next !=  NULL)
                {
                    temp = temp -> next ;   
                }
                temp -> next = var_list;
                
                $$ = $1 ;
           }


assignStmt: VAR ASSIGN expr SEMI_COLON  { struct expr_node* var_node = createExpr_Node(VARIABLE , NULL , NULL , '\0' , 0 , $1) ;
                                          $$ = createExpr_Node(OP , var_node , $3 , '=') ; }


printStmt: PRINT_FN '(' exprList ')' SEMI_COLON {$$ = $3 ; }


exprList: expr       { $$ = createFunc_Node($1,NULL); }
        | exprList ',' expr     
        {
            struct funcStmt_node* temp = createFunc_Node($3,NULL);
            struct funcStmt_node *start = $1 ;
                
            while( start -> next !=  NULL)
            {
                start = start -> next ;   
            }
            start -> next = temp;
            $$ = $1 ;
        }

%%

#include <ctype.h>
int lineno = 1 ;
char *progname;	/* for error messages */
int main(int argc, char *argv[])
{  
    progname = argv[0];
    yyparse();
    return 0;
}

void yyerror(char const *s)
{
    warning(s, (char *)0);
}

void warning(char const *s, char const *t)	/* print warning message */
{
    fprintf(stderr, "%s: %s", progname, s);
    if (t)
        fprintf(stderr, " %s", t);
    fprintf(stderr, " near line %d\n", lineno);
}



