#include <vector>
#include <string>
#include <fstream>
using namespace std;

// EXPRESSION NODE
enum expr_type { OP = 0 , INTEGER = 1 , BOOL = 2 , VARIABLE = 3 , ARRAY_ELEMENT = 4 , FUNCTION_CALL = 5 , STRING_VAR = 6 , UNUSED = 7};
enum op_type { PLUS_OP = 0 , SUB_OP = 1 , MUL_OP = 2 , DIV_OP = 3 , REMAINDER_OP = 4 , 
               GREATERTHAN_OP = 5 , LESSTHAN_OP = 6 , GREATERTHAN_EQUAL_OP = 7 , LESSTHAN_EQUAL_OP = 8 , NOTEQUAL_OP = 9 , EQUALEQUAL_OP = 10 ,
               LOGICAL_NOT_OP = 11 , LOGICAL_AND_OP = 12 , LOGICAL_OR_OP = 13 , ASSIGN_OP = 14};

struct expr_node
{
    expr_type type;
    op_type op;
    int const_val;
    bool bool_val;
    char* name;
    struct expr_node* params ;  
    struct expr_node* left;
    struct expr_node* right;
}; 

// declaration statement
// enum decl_scope {GLOBAL_SCOPE = 0 , FUNCTION_SCOPE = 1};
enum decl_node_type {VAR_NODE = 0 , VAR_ARR_NODE = 1 , FUNC_NODE = 2};
enum return_type {DECL_INT = 0 , DECL_BOOL = 1};

// struct arguments 
// {
//     char* var_name;
//     return_type var_type;
//     struct arguments *next;
// };
// type of arguments should not be struct args else should be included in union...
struct decl_node;
struct declstmt_tree;
struct stmt_list;

struct decl_node
{
    decl_node_type decl_type;
    char* name ;
    int array_size;
    struct stmt_list* args;
    struct decl_node *next;
};


struct declstmt_tree
{
    // decl_scope scope ;
    return_type ret_type ;
    struct decl_node* node;
};

// ASSIGN statement - struct expr_node* root
// root contains variable which is left side of equation
// right contains expression which will be equated to it

// READ statement - it just needs a var_node to read into.

// WRITE statement - requires expr or strings to print : these can be captured by expr_node pointer

// CONDITION statement


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
    char* func_name ;
    struct stmt_list *argList ;     // declstmt
    struct stmt_list *decl_block;
    struct stmt_list *stmt_block;
    struct stmt_list *return_stmt;
};

// Statements
enum stmt_type {DECL_STMT = 0 , ASSIGN = 1 , READ_STMT = 2, WRITE_STMT  = 3, CONDT = 4 , RETURN_STMT = 5 , FUNC_DEF = 6 , FUNC_CALL = 7 , END_DECL = 8 , UNUSED_STMT = 9 , DECL_START = 10};

union stmt_tree
{
    struct declstmt_tree *decl_stmt_tree;
    struct expr_node *root;         // READ , WRITE , ASSIGN_STMT , RETURN_STMT , FUNC_CALL
    struct condtStmt_tree* condt_stmt_tree ;
    struct func_definition_tree* func_def_tree;
};

struct stmt_list
{
    stmt_type type;
    int line_num ;
    union stmt_tree tree ;
    struct stmt_list *next ;
};

/* NODES CREATION */
struct expr_node* createExpr_Node(expr_type type ,  struct expr_node* left=NULL , struct expr_node* right=NULL , op_type op = PLUS_OP , int const_val = 0 , bool val = true , char *var_name = NULL, struct expr_node* params=NULL) ;
struct decl_node* createDecl_Node(decl_node_type type , char* name , int size=0 , struct stmt_list* args=NULL);

/* STATEMENTS CREATION */
struct stmt_list* create_Stmt(stmt_type type , int line_num , struct expr_node* expr1 , struct expr_node* expr2);
struct stmt_list* create_Condt_Stmt(condt_type type , int line_num , struct expr_node* condition , struct stmt_list* stmt1 , struct stmt_list* stmt2);
struct stmt_list* create_Enddecl_Stmt(int n);
struct stmt_list* create_Decl_Stmt(int ret_type , struct decl_node* node  , int line_num);
struct stmt_list* create_unused_stmt();
struct stmt_list* create_return_stmt(struct expr_node *expr , int line_num);
struct stmt_list* create_decl_start_stmt();

/* FUNCTION CREATION */ 
struct stmt_list* create_function(int ret_type , char* fun_name , struct stmt_list* argList ,  struct stmt_list *decl_block , struct stmt_list* stmt_block , struct stmt_list* return_stmt , int line_num);

/* ABSTRACT SYNTAX TREE */
void ast_printing(struct stmt_list* root);
void print_expressions(struct expr_node* root);

/* SEMANTIC ERROR CHECKING */
void expression_type_checking(struct expr_node* root , int line_num , char* func_name);
void semantic_error_checking(struct stmt_list* root , char* func_name);

/* PRINTING EQUIVALENT C CODE */
void generate_equivalent_c_expressions(struct expr_node* root , ofstream& out_stream);
void generate_equivalent_c_statements(struct stmt_list* root , ofstream& out_stream , int isFuncArgs);
void generate_equivalent_c_code(struct stmt_list* start , char* output_file_name);

// /* EVALUATION OF SYNTAX TREE */
// int evaluate_expression_tree(struct expr_node* root , int line_num);
// void evaluate_assign_stmt(struct stmt_list* root);
// void evaluate_write_stmt(struct stmt_list* root);
// void evaluate_condt_stmt(struct stmt_list* root);

// void evaluate_program(struct stmt_list *root);

// /* PRINTING ALL SYMBOL VALUES */
// void print_symbol_values();