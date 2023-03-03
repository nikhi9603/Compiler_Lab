#include <stdlib.h>

enum expr_type { OP = 0 , CONSTANT = 1 , VARIABLE = 2};

union node_type
{
    char op;
    double val;
    char* variable_name;    
};

// struct nodes of syntax tree
struct expr_node
{
    /* Type can be 0, 1, 2 */
	/* Type 0 : operators + , * , / , -  */
	/* Type 1 : constants */
	/* Type 2 : variables */
    expr_type type;
    union node_type node_type_obj ;
    struct expr_node* left;
    struct expr_node* right;
};

/* TREES FOR EACH STATEMENT */
struct node
{
    char *var_name ;
    struct node *next;
};

enum var_type { INT = 0 , FLOAT = 1 , CHAR = 2} ; 

struct declstmt_tree 
{
    /* For lab : only integer is considered */
    var_type type;
    struct node *decl_var ;          /* only (struct node* right) is consdidered for whole list */
    struct declstmt_tree *next;
};

// enum arg_type {EXPR = 0 , VARIABLE_ARG = 1};

// union arguments
// {
//     struct node var ;
//     struct expr_node expr ;
// };

// struct funcStmt_node
// {
//     arg_type type;
//     union arguments args;
//     struct funcStmt_node *next ;
// };

union stmt_tree
{
    struct declstmt_tree *decl_tree;
    struct expr_node* assignStmt_tree ;
    struct node *func_tree ;
};

// type of each statement
enum stmt_type {DECL_STMT = 0 , ASSIGN_STMT = 1 , PRINTFN_STMT = 2} ;

struct statement
{
    stmt_type type ;
    union stmt_tree *tree ;
    struct statement *next;
};

/* NODES CREATION */
struct expr_node* createExpr_Node(expr_type type ,  struct expr_node* left=NULL , struct expr_node* right=NULL , char op = '\0' , double val = 0 , char *var_name = nullptr, char *fun_name = nullptr) ;

struct node* createNode(char* var_name , struct node* next_node);

//struct funcStmt_node* createFunc_Node(struct expr_node expr , struct funcStmt_node* next);

double evaluate_Expr_Tree(struct expr_node* root , int i) ;

void print_func_tree(struct node *root);

void evaluate_assign_stmt(struct expr_node *root);

void evaluate_declare_stmt(struct declstmt_tree *root);

struct statement* createStmt(stmt_type type , struct declstmt_tree *decl_tree , struct expr_node* assignStmt_tree , struct node *func_tree , struct statement* next);

void evaluate_program(struct statement *root);

