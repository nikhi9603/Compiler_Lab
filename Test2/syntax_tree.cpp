#include <iostream>
#include "syntax_tree.h"
// #include "sym_table.h"
using namespace std;

/* EXPRESSION TREE NODE */
struct expr_node* createExpr_Node(expr_type type , struct expr_node* left , struct expr_node* right , op_type op  , int const_val  , bool val , char *var_name , struct expr_node* index) 
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
		case STRING_VAR:
				newNode->name = var_name;
				break;
        case UNUSED:
                break;
		default:
				/* raise error */
				cerr << "Error: Invalid expression" << endl ;
				// exit(0);
	}
	return newNode;
}

/* DECLARATION TREE NODE */
struct decl_node* createDecl_Node(decl_node_type type , char* name , int size)
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

    if(type == VAR_ARR_NODE)
    {
        newNode->array_size = size;   
    }
	
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
		default:
			cerr << "Error: Invalid statement" << endl;
			// exit(0);
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
			// exit(0);
	}
	return new_stmt;
}


/* creating an extra node to specify that declaration block is ended */
struct stmt_list* create_Enddecl_Stmt()
{
	struct stmt_list* new_stmt = (struct stmt_list*)malloc(sizeof(struct stmt_list)) ;

	if(new_stmt == NULL)
	{
		cerr << "Memory not available to allocate new stmt node\n" << endl ;
		exit(0);
	}
	new_stmt->type = END_DECL ;
	new_stmt->next = NULL;
	return new_stmt;
}

/* DECLARATION  STATEMENT */
struct stmt_list* create_Decl_Stmt(int ret_type , struct decl_node* node)
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
struct stmt_list* create_Main(int ret_type , struct stmt_list* stmt_block , struct stmt_list* return_stmt)
{
	struct stmt_list* new_stmt = (struct stmt_list*)malloc(sizeof(struct stmt_list)) ;

	if(new_stmt == NULL)
	{
		cerr << "Memory not available to allocate new stmt node\n" << endl ;
		exit(0);
	}

	new_stmt->type = FUNC_DEF ;
	new_stmt->tree.main_func_def_tree.stmt_block = stmt_block ;
	new_stmt->tree.main_func_def_tree.return_stmt = return_stmt ;
	new_stmt->next = NULL;

	if(ret_type == 1)
	{
		new_stmt->tree.main_func_def_tree.ret_type  = DECL_BOOL;
	}
	else
	{
		new_stmt->tree.main_func_def_tree.ret_type  = DECL_INT;
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


// Abstract syntax tree printing
void print_expressions(struct expr_node* root)
{
	if(root != NULL)
	{
		// LEFT TREE
		cout << "(" ;
		print_expressions(root->left);
		
		switch (root->type)
		{
		case OP:
		{
			switch (root->op)
			{
				case PLUS_OP :
					cout << "PLUS " ;
					break;
				case SUB_OP : 
					cout << "SUB " ;
					break;
				case MUL_OP : 
					cout << "MUL " ;
					break;
				case DIV_OP : 
					cout << "DIV " ;
					break;
				case REMAINDER_OP : 
					cout << "MODULO " ;
					break;
				case GREATERTHAN_OP : 
					cout << "GREATERTHAN " ;
					break;
				case LESSTHAN_OP : 
					cout << "LESSTHAN " ;
					break;
				case GREATERTHAN_EQUAL_OP : 
					cout << "GREATERTHANOREQUAL " ;
					break;
				case LESSTHAN_EQUAL_OP : 
					cout << "LESSTHANOREQUAL " ;
					break;
				case NOTEQUAL_OP : 
					cout << "NOTEQUAL " ;
					break;
				case EQUALEQUAL_OP : 
					cout << "EQUALEQUAL " ;
					break;
				default:
					break;
		}
			break;
		case INTEGER:
			cout << "INTEGER " ;
		case BOOL:
			cout << "BOOL " ;
		case VARIABLE:
			cout << "VAR " ;
		case ARRAY_ELEMENT:
			cout << "VAR_ARRAY_ELEMENT " ;
		case STRING_VAR:
			cout << "STRING ";
		default:
			break;
		
		// RIGHT TREE
		print_expressions(root->right);
		cout << ")" ;
		}
		}
	}
}



void ast_printing(struct stmt_list* root)
{
	if(root->type != END_DECL || root != NULL )
	{
		switch(root->type)
		{
			case DECL_STMT:
				cout << "DECL " << root->tree.decl_stmt_tree->ret_type << " ";
				cout << root->tree.decl_stmt_tree->node->name ;

				while(root->tree.decl_stmt_tree->node != NULL)
				{
					if(root->tree.decl_stmt_tree->node->decl_type == VAR_NODE)
					{
						cout << " , " << "VAR" ;
					}
					else
					{
						cout << " , " << "VAR_ARRAY" ;
					}
				}
				cout << endl;
				break;
			case ASSIGN:
				cout << "ASSIGN " ;
				if(root->tree.root->left->type == VARIABLE)
				{
					cout << "VAR " ;
				}
				else
				{
					cout << "VAR_ARRAY_ELEMENT " ;
				}

				print_expressions(root->tree.root->left);
				cout << endl;
				break;
			case READ_STMT:
				cout << "READ " ;
				if(root->tree.root->type == VARIABLE)
				{
					cout << "VAR " ;
				}
				else
				{
					cout << "VAR_ARRAY_ELEMENT " ;
				}	
				cout << endl;
				break;
			case WRITE_STMT:
				cout << "WRITE " ;
				print_expressions(root->tree.root) ;
				cout << endl;
				break;
			case CONDT:
			{
				switch (root->tree.condt_stmt_tree->type)
				{
				case IF_CONDT:
					cout << "IF " ;
					print_expressions(root->tree.condt_stmt_tree->condition) ;
					cout << "THEN" ;
					ast_printing(root->tree.condt_stmt_tree->stmts1);
					cout << "ENDIF"  << endl;
					break;
				case IF_ELSE:
					cout << "IF " ;
					print_expressions(root->tree.condt_stmt_tree->condition) ;
					cout << "THEN" ;
					ast_printing(root->tree.condt_stmt_tree->stmts1);
					cout << "ELSE" ;
					ast_printing(root->tree.condt_stmt_tree->stmts2);
					cout << "ENDIF"  << endl;
					break;
				case WHILE_CONDT:
					cout << "WHILE " ;
					print_expressions(root->tree.condt_stmt_tree->condition) ;
					cout << "DO" ;
					ast_printing(root->tree.condt_stmt_tree->stmts1);
					cout << "ENDWHILE"  << endl;
					break;
				default:
					break;
				}
			}
					
		}
	}
	
}

