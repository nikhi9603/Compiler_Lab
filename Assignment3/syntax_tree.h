#include <vector>
using namespace std;

// EXPRESSION NODE
enum expr_type { OP = 0 , INTEGER = 1 , BOOL = 2 , VARIABLE = 3 , ARRAY_ELEMENT = 4 , FUNCTION_CALL = 5 , STRING_VAR = 6 , ARGS = 7 , UNUSED = 8};

struct expr_node
{
    expr_type type;
    char op;
    int const_val;
    bool bool_val;
    char* name;
    vector<expr_node> index_list; 
    struct expr_node* params ;  
    struct expr_node* left;
    struct expr_node* right;
}; 

// declaration statement
enum decl_scope {GLOBAL_SCOPE = 0 , FUNCTION_SCOPE = 1};
enum decl_node_type {VAR_NODE = 0 , VAR_ARR_NODE = 1 , FUNC_NODE = 2};
enum return_type {DECL_INT = 0 , DECL_BOOL = 1};

// struct arguments 
// {
//     char* var_name;
//     return_type var_type;
//     struct arguments *next;
// };
// type of arguments should not be struct args else should be included in union...

struct decl_node
{
    decl_node_type decl_type;
    char* name ;
    vector<int> index_list; 
    struct expr_node* args;
    struct decl_node *next;
};


struct declstmt_tree
{
    decl_scope scope ;
    return_type ret_type ;
    struct decl_node* node;
};

// ASSIGN statement - struct expr_node* root
// root contains variable which is left side of equation
// right contains expression which will be equated to it

// READ statement - it just needs a var_node to read into.

// WRITE statement - requires expr or strings to print : these can be captured by expr_node pointer

// CONDITION statement
struct stmt_list;

enum condt_type {IF = 0 , IF_ELSE = 1 , WHILE = 2 , FOR = 3};

struct condtStmt_tree
{
    condt_type type ;
    struct expr_node *condition ;
    struct stmt_list *stmts1 ;      // stmts if condition is true
    struct stmt_list *stmts2 ;      // stmts if condition is false
};

// FUNCTION DEFINTION BLOCK
struct func_defintion_tree
{
    return_type ret_type;
    char *func_name ;
    struct arguments *argList ;
    struct declstmt_tree *decl_block;
    struct stmt_list *stmt_block;
    struct expr_node *return_stmt;
};

// Statements
enum stmt_type {DECL = 0 , ASSIGN = 1 , READ = 2, WRITE  = 3, CONDT = 4 , FUNC_CALL = 5 , RETURN_STMT = 6 , FUNC_DEF = 7};

union stmt_tree
{
    struct declstmt_tree *decl_stmt_tree;
    struct expr_node *root;         // READ , WRITE , ASSIGN_STMT , RETURN_STMT 
    struct condtStmt_tree* condt_stmt_tree ;
    struct func_defintion_tree* func_def_tree;
};

struct stmt_list
{
    stmt_type type;
    union stmt_tree tree ;
    struct stmt_list *next ;
};

/* NODES CREATION */
struct expr_node* createExpr_Node(expr_type type ,  struct expr_node* left=NULL , struct expr_node* right=NULL , char op = '\0' , int const_val = 0 , bool val = true , char *var_name = NULL, struct expr_node* params=NULL) ;
struct decl_node* createDecl_Node(decl_node_type type , char* name);

/* STATEMENTS CREATION */
struct stmt_list* create_Stmt(stmt_type type , struct expr_node* expr1 , struct expr_node* expr2);
struct stmt_list* create_Condt_Stmt(condt_type type , struct expr_node* condition , struct stmt_list* stmt1 , struct stmt_list* stmt2 = NULL);
struct stmt_list* create_Enddecl_Stmt(decl_scope scope);
struct stmt_list* create_Decl_Stmt(decl_scope scope , int ret_type , struct decl_node* node);
