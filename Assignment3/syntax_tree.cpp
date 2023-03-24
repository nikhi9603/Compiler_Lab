#include <iostream>
#include "syntax_tree.h"
// #include "sym_table.h"
using namespace std;

/* EXPRESSION TREE NODE */
struct expr_node* createExpr_Node(expr_type type , struct expr_node* left , struct expr_node* right=NULL , char op  , int const_val  , bool val , char *name , struct expr_node* params) 
{
     struct expr_node* newNode;

	// allocating memory for newNodes
	newNode = (struct expr_node*)malloc(sizeof(struct expr_node)) ;
	
	if(newNode == NULL)
	{
		cerr << "Memory not available to allocate new nodes\n" << endl ;
		exit(0);
	}

    newNode->type = type ;
 	newNode->left = left ;
	newNode->right = right ;

    switch (type)
	{
		case OP: 		/* Type 0 : operators + , * , / , -  */
				newNode->op = op;
				break;
		case INTEGER:			/* Type 1 : constants */
				newNode->const_val = const_val ;				
				break;
		case BOOL:
                newNode->bool_val = val;
				break;	
        case VARIABLE:
                newNode->name = name;
				break;
        case ARRAY_ELEMENT:
                newNode->name = name;
				break;	
        case FUNCTION_CALL:
                newNode->name = name;
                newNode->params = params;
                break;
        case UNUSED:
                break;
		default:
				/* raise error */
				cerr << "Invalid expression" << endl ;
				exit(0);
	}
}


struct stmt_list* create_Assign_Stmt(struct expr_node* var_node , struct expr_node* expr)
{
	struct expr_node* node = createExpr_Node(OP , var_node , expr , '=') ;

	struct stmt_list* new_stmt = (struct stmt_list*)malloc(sizeof(struct stmt_list)) ;

	if(new_stmt == NULL)
	{
		cerr << "Memory not available to allocate new stmt node\n" << endl ;
		exit(0);
	}

	new_stmt->type = ASSIGN ;
	new_stmt->tree.root = node ;
	new_stmt->next = NULL;

	return new_stmt;
}
