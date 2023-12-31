#include <vector>
#include <string>
using namespace std;

// EXPRESSION NODE
enum expr_type { OP = 0 , INTEGER = 1 , BOOL = 2 , VARIABLE = 3 , ARRAY_ELEMENT = 4 , STRING_VAR = 5 , UNUSED = 6};
enum op_type { PLUS_OP = 0 , SUB_OP = 1 , MUL_OP = 2 , DIV_OP = 3 , REMAINDER_OP = 4 , 
               GREATERTHAN_OP = 5 , LESSTHAN_OP = 6 , GREATERTHAN_EQUAL_OP = 7 , LESSTHAN_EQUAL_OP = 8 , NOTEQUAL_OP = 9 , EQUALEQUAL_OP = 10 , ASSIGN_OP = 11};

struct expr_node
{
    expr_type type;
    op_type op;
    int const_val;
    bool bool_val;
    char* name;
    struct expr_node* index;  
    struct expr_node* left;
    struct expr_node* right;
}; 

// declaration statement
enum decl_node_type {VAR_NODE = 0 , VAR_ARR_NODE = 1 };
enum return_type {DECL_INT = 0 , DECL_BOOL = 1};

struct decl_node
{
    decl_node_type decl_type;
    char* name ;
    int array_size;
    struct decl_node *next;
};

struct declstmt_tree
{
    return_type ret_type ;
    struct decl_node* node;
};


// WRITE statement - requires expr or strings to print : these can be captured by expr_node pointer

// CONDITION statement
struct stmt_list;

enum condt_type {IF_CONDT = 0 , IF_ELSE = 1 , WHILE_CONDT = 2 };

struct condtStmt_tree
{
    condt_type type ;
    struct expr_node *condition ;
    struct stmt_list *stmts1 ;      // stmts if condition is true
    struct stmt_list *stmts2 ;      // stmts if condition is false
};

// FUNCTION DEFINTION BLOCK
struct func_definition_tree
{
    return_type ret_type;
    struct stmt_list *stmt_block;
    struct stmt_list *return_stmt;
};

// Statements
enum stmt_type {DECL_STMT = 0 , ASSIGN = 1 , READ_STMT = 2, WRITE_STMT  = 3, CONDT = 4 , RETURN_STMT = 5 , FUNC_DEF = 6 , END_DECL = 7 , UNUSED_STMT = 8};

union stmt_tree
{
    struct declstmt_tree *decl_stmt_tree;
    struct expr_node *root;         // READ , WRITE , ASSIGN_STMT , RETURN_STMT 
    struct condtStmt_tree* condt_stmt_tree ;
    struct func_definition_tree* main_func_def_tree;
};

struct stmt_list
{
    stmt_type type;
    int line_num ;
    union stmt_tree tree ;
    struct stmt_list *next ;
};

/* NODES CREATION */
struct expr_node* createExpr_Node(expr_type type ,  struct expr_node* left=NULL , struct expr_node* right=NULL , op_type op = PLUS_OP , int const_val = 0 , bool val = true , char *var_name = NULL, struct expr_node* index=NULL) ;
struct decl_node* createDecl_Node(decl_node_type type , char* name , int size);

/* STATEMENTS CREATION */
struct stmt_list* create_Stmt(stmt_type type , struct expr_node* expr1 , struct expr_node* expr2);
struct stmt_list* create_Condt_Stmt(condt_type type , struct expr_node* condition , struct stmt_list* stmt1 , struct stmt_list* stmt2);
struct stmt_list* create_Enddecl_Stmt();
struct stmt_list* create_Decl_Stmt(int ret_type , struct decl_node* node);
struct stmt_list* create_unused_stmt();
struct stmt_list* create_return_stmt(struct expr_node *expr);

/* MAIN FUNCTION */ 
struct stmt_list* create_Main(int ret_type ,struct stmt_list* stmt_block , struct stmt_list* return_stmt);

/* ABSTRACT SYNTAX TREE */
void ast_printing(struct stmt_list* root , int mark);
void print_expressions(struct expr_node* root);

/* SEMANTIC ERROR CHECKING */
void expression_type_checking(struct expr_node* root , int line_num);
void semantic_error_checking(struct stmt_list* root);

/* EVALUATION OF SYNTAX TREE */
int evaluate_expression_tree(struct expr_node* root , int line_num);
void evaluate_assign_stmt(struct stmt_list* root);
void evaluate_write_stmt(struct stmt_list* root);
void evaluate_condt_stmt(struct stmt_list* root);

void evaluate_program(struct stmt_list *root);

/* PRINTING ALL SYMBOL VALUES */
void print_symbol_values();