#include <iostream>
#include "syntax_tree.h"
// #include "sym_table.h"
using namespace std;

/* EXPRESSION TREE NODE */
struct expr_node* createExpr_Node(expr_type type , struct expr_node* left , struct expr_node* right , op_type op  , int const_val  , bool val , char *var_name , struct expr_node* params) 
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
                newNode->name = var_name;
				break;
        case ARRAY_ELEMENT:
                newNode->name = var_name;
				break;	
        case FUNCTION_CALL:
                newNode->name = var_name;
                newNode->params = params;
                break;
		case STRING_VAR:
				newNode->name = var_name;
				break;
        case UNUSED:
                break;
		default:
				/* raise error */
				cerr << "Invalid expression" << endl ;
				exit(0);
	}
	return newNode;
}

/* DECLARATION TREE NODE */
struct decl_node* createDecl_Node(decl_node_type type , char* name)
{
    struct decl_node* newNode;

	// allocating memory for newNodes
	newNode = (struct decl_node*)malloc(sizeof(struct decl_node)) ;
	
	if(newNode == NULL)
	{
		cerr << "Memory not available to allocate new nodes\n" << endl ;
		exit(0);
	}	
	newNode->decl_type = type;
	newNode->name = name;
	newNode->next = NULL;
	return newNode;
}


/* CREATING STATEMENT TREE OF TYPE USING expr_node as tree in structure stmt_list */
struct stmt_list* create_Stmt(stmt_type type , struct expr_node* expr1 , struct expr_node* expr2)
{
	struct stmt_list* new_stmt = (struct stmt_list*)malloc(sizeof(struct stmt_list)) ;

	if(new_stmt == NULL)
	{
		cerr << "Memory not available to allocate new stmt node\n" << endl ;
		exit(0);
	}

	new_stmt->type = type;
	new_stmt->next = NULL;

	switch (type)
	{
		case ASSIGN:
		{
			struct expr_node* node = createExpr_Node(OP , expr1 , expr2 , ASSIGN_OP) ;
			new_stmt->tree.root = node ;
			break;
		}
		case READ_STMT:
			new_stmt->tree.root = expr1;
			break;
		case WRITE_STMT:
			new_stmt->tree.root = expr1;
			break;
		case RETURN_STMT:
			new_stmt->tree.root = expr1;
			break;
		case FUNC_CALL:
			new_stmt->tree.root = expr1;
			break;
		default:
			cerr << "Invalid statement" << endl;
			exit(0);
	}
	return new_stmt;
}


/* creating CONDITIONAL STATEMENT tree */
struct stmt_list* create_Condt_Stmt(condt_type type , struct expr_node* condition , struct stmt_list* stmt1 , struct stmt_list* stmt2)
{
	struct stmt_list* new_stmt = (struct stmt_list*)malloc(sizeof(struct stmt_list)) ;
	struct condtStmt_tree* condt_tree = (struct condtStmt_tree*)malloc(sizeof(struct condtStmt_tree)) ;

	if(new_stmt == NULL || condt_tree == NULL)
	{
		cerr << "Memory not available to allocate new stmt node\n" << endl ;
		exit(0);
	}

	new_stmt->type = CONDT ;
	new_stmt->tree.condt_stmt_tree = condt_tree ;
	new_stmt->tree.condt_stmt_tree->type = type ;
	new_stmt->next = NULL;

	switch (type)
	{
		case IF_CONDT:
			new_stmt->tree.condt_stmt_tree->condition = condition;
			new_stmt->tree.condt_stmt_tree->stmts1 = stmt1;
			break;
		case IF_ELSE:
			new_stmt->tree.condt_stmt_tree->condition = condition;
			new_stmt->tree.condt_stmt_tree->stmts1 = stmt1;
			new_stmt->tree.condt_stmt_tree->stmts1 = stmt2;
			break;
		case WHILE_CONDT:
			new_stmt->tree.condt_stmt_tree->condition = condition;
			new_stmt->tree.condt_stmt_tree->stmts1 = stmt1;
			break;
		default:
			cerr << "Invalid conditional statement" << endl;
			exit(0);
	}
	return new_stmt;
}


/* creating an extra node to specify that declaration block is ended */
struct stmt_list* create_Enddecl_Stmt(decl_scope scope)
{
	struct stmt_list* new_stmt = (struct stmt_list*)malloc(sizeof(struct stmt_list)) ;
	struct declstmt_tree* decl_tree = (struct declstmt_tree*)malloc(sizeof(struct declstmt_tree));

	if(new_stmt == NULL || decl_tree == NULL)
	{
		cerr << "Memory not available to allocate new stmt node\n" << endl ;
		exit(0);
	}
	new_stmt->type = DECL_STMT ;
	new_stmt->tree.decl_stmt_tree = decl_tree;
	new_stmt->tree.decl_stmt_tree->scope = GLOBAL_SCOPE ;
	new_stmt->tree.decl_stmt_tree->node = NULL;
	new_stmt->next = NULL;
	return new_stmt;
}


/* DECLARATION  STATEMENT */
struct stmt_list* create_Decl_Stmt(decl_scope scope , int ret_type , struct decl_node* node)
{
	struct stmt_list* new_stmt = (struct stmt_list*)malloc(sizeof(struct stmt_list)) ;
	struct declstmt_tree* decl_tree = (struct declstmt_tree*)malloc(sizeof(struct declstmt_tree));

	if(new_stmt == NULL || decl_tree == NULL)
	{
		cerr << "Memory not available to allocate new stmt node\n" << endl ;
		exit(0);
	}

	new_stmt->type = DECL_STMT;
	new_stmt->tree.decl_stmt_tree = decl_tree;
	new_stmt->tree.decl_stmt_tree->scope = scope ;
	new_stmt->tree.decl_stmt_tree->node = node;

	if(ret_type == 1)
	{
		new_stmt->tree.decl_stmt_tree->ret_type = DECL_BOOL;
	}
	else
	{
		new_stmt->tree.decl_stmt_tree->ret_type = DECL_INT;
	}
	new_stmt->next = NULL;
	return new_stmt;
}


/* creating MAIN function */
struct stmt_list* create_Main(int ret_type , char* fun_name , struct stmt_list *decl_block , struct stmt_list* stmt_block , struct stmt_list* return_stmt)
{
	struct stmt_list* new_stmt = (struct stmt_list*)malloc(sizeof(struct stmt_list)) ;
	struct func_definition_tree* func_tree = (struct func_definition_tree*)malloc(sizeof(struct func_definition_tree)) ;

	if(new_stmt == NULL || func_tree == NULL)
	{
		cerr << "Memory not available to allocate new stmt node\n" << endl ;
		exit(0);
	}

	new_stmt->type = FUNC_DEF ;
	new_stmt->tree.func_def_tree = func_tree ;
	new_stmt->tree.func_def_tree->argList = NULL;
	new_stmt->tree.func_def_tree->func_name = fun_name ;
	new_stmt->tree.func_def_tree->decl_block = decl_block ;
	new_stmt->tree.func_def_tree->stmt_block = stmt_block ;
	new_stmt->tree.func_def_tree->return_stmt = return_stmt ;
	new_stmt->next = NULL;

	if(ret_type == 1)
	{
		new_stmt->tree.func_def_tree->ret_type = DECL_BOOL;
	}
	else
	{
		new_stmt->tree.func_def_tree->ret_type = DECL_INT;
	}

	return new_stmt;
}


/* unused statement creation */
struct stmt_list* create_unused_stmt()
{
	struct stmt_list* new_stmt = (struct stmt_list*)malloc(sizeof(struct stmt_list)) ;

	if(new_stmt == NULL)
	{
		cerr << "Memory not available to allocate new stmt node\n" << endl ;
		exit(0);
	}

	new_stmt->type = UNUSED_STMT;
	return new_stmt;
}

/* creating RETURN statement */
struct stmt_list* create_return_stmt(struct expr_node *expr)
{
	struct stmt_list* new_stmt = (struct stmt_list*)malloc(sizeof(struct stmt_list)) ;	

	if(new_stmt == NULL)
	{
		cerr << "Memory not available to allocate new stmt node\n" << endl ;
		exit(0);
	}

	new_stmt->type = RETURN_STMT;
	new_stmt->next = NULL ;
	new_stmt->tree.root = expr;

	return new_stmt;
}
