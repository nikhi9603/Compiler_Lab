#include <vector>
using namespace std;

// EXPRESSION NODE
enum expr_type { OP = 0 , INTEGER = 1 , BOOL = 2 , VARIABLE = 3 , ARRAY_ELEMENT = 4 , FUNCTION_CALL = 5 , UNUSED = 6};

struct expr_node
{
    expr_type type;
    char op;
    int const_val;
    bool bool_val;
    char* name;
    vector<int> index_list; 
    struct expr_node* params ;  
    struct expr_node* left;
    struct expr_node* right;
}; 

// declaration statement
enum decl_scope {GLOBAL_SCOPE = 0 , FUNCTION_SCOPE = 1};
enum decl_node_type {VAR_NODE = 0 , VAR_ARR_NODE = 1 , FUNC_NODE = 2};
enum return_type {DECL_INT = 0 , DECL_BOOL = 1};

struct arguments 
{
    char* var_name;
    return_type var_type;
    struct arguments *next;
};

struct decl_node
{
    decl_scope scope ;
    decl_node_type decl_type;
    char* name ;
    vector<int> index_list; 
    int nargs;
    struct arguments* args;
    struct decl_node *next;
};


struct declstmt_tree
{
    return_type ret_type ;
    struct decl_node* node;
    struct declstmt_tree *next;
};


// assignment statement - struct expr_node* root 
// root contains variable which is left side of equation
// right contains expression which will be equated to it


// read statement - it just needs a var_node to read into.

// write statement - requires list of variables to print 
struct writestmt_tree
{
    char* var_name ;
    struct writestmt_tree* next;
};

// condition statement
struct stmt_list;

enum condt_type {IF = 0 , IF_ELSE = 1 , WHILE = 2 , FOR = 3};

struct condtStmt_tree
{
    condt_type type ;
    struct expr_node *expr ;
    struct stmt_list *stmts ;
};

// Statements
enum stmt_type {DECL = 0 , ASSIGN = 1 , READ = 2, WRITE  = 3, CONDT = 4 , FUNC_CALL = 5 , RETURN_STMT = 6};

union stmt_tree
{
    struct declstmt_tree *decl_stmt_tree;
    struct expr_node *root;                 //FUNC_CALL , ASSIGN_STMT , RETURN_STMT
    struct writestmt_tree* write_stmt_tree ;
    struct condtStmt_tree* condt_stmt_tree ;
};

struct stmt_list
{
    stmt_type type;
    union stmt_tree tree ;
    struct stmt_list *next ;
};


/* NODES CREATION */
struct expr_node* createExpr_Node(expr_type type ,  struct expr_node* left=NULL , struct expr_node* right=NULL , char op = '\0' , int const_val = 0 , bool val = true , char *var_name = nullptr , vector<int> index_ls  , struct expr_node* params=NULL) ;
