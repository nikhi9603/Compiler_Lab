#include <iostream>
#include "syntax_tree.h"
#include "sym_table.h"
using namespace std;

extern int error_check_var ;

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
				newNode->index = index;
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
			new_stmt->tree.condt_stmt_tree->stmts2 = stmt2;
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
	struct func_definition_tree *func_tree = (struct func_definition_tree*)malloc(sizeof(struct func_definition_tree));

	if(new_stmt == NULL || func_tree == NULL)
	{
		cerr << "Memory not available to allocate new stmt node\n" << endl ;
		exit(0);
	}

	new_stmt->type = FUNC_DEF ;
	new_stmt->tree.main_func_def_tree = func_tree;
	new_stmt->tree.main_func_def_tree->stmt_block = stmt_block ;
	new_stmt->tree.main_func_def_tree->return_stmt = return_stmt ;
	new_stmt->next = NULL;

	if(ret_type == 1)
	{
		new_stmt->tree.main_func_def_tree->ret_type  = DECL_BOOL;
	}
	else
	{
		new_stmt->tree.main_func_def_tree->ret_type  = DECL_INT;
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
	new_stmt->next = NULL;
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
				cout << "NUM " ;
				break;
			case BOOL:
				cout << "BOOL_VAL " ;
				break;
			case VARIABLE:
				cout << "VAR " ;
				break;
			case ARRAY_ELEMENT:
				cout << "VAR_ARRAY_ELEMENT VAR " ;
				print_expressions(root->index);
				break;
			case STRING_VAR:
				cout << "STRING ";
				break;
			default:
				break;
			}
		}
		// LEFT TREE
		print_expressions(root->left);
		// RIGHT TREE
		print_expressions(root->right);
		
	}
}


void ast_printing(struct stmt_list* root , int mark)
{
	if( root != NULL )
	{
		switch(root->type)
		{
			case DECL_STMT:
			{
				if(mark == 0)
				{
					cout << "DECL" << endl;
					mark = 1 ;
				}

				if(root->tree.decl_stmt_tree->ret_type == 0)
				{
					cout << "INTEGER ";
				}
				else
				{
					cout << "BOOL " ;
				}
				
				if(root->tree.decl_stmt_tree->node->decl_type == VAR_NODE)
				{
					cout << "VAR" ;
				}
				else
				{
					cout << "VAR_ARRAY " << root->tree.decl_stmt_tree->node->array_size ;
				}

				struct decl_node* temp = root->tree.decl_stmt_tree->node ;
				while(temp != NULL)
				{
					if(temp->decl_type == VAR_NODE)
					{
						cout << " , " << "VAR" ;
					}
					else
					{
						cout << " , " << "VAR_ARRAY " << temp->array_size ;
					}

					temp = temp -> next;
				}
				cout << endl;
				break;
			}
			case ASSIGN:
				cout << "ASSIGN " ;
				print_expressions(root->tree.root->left);
				print_expressions(root->tree.root->right);
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
					cout << "VAR_ARRAY_ELEMENT VAR " ;
					print_expressions(root->tree.root->index) ;
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
					cout << "THEN" << endl ;
					ast_printing(root->tree.condt_stmt_tree->stmts1 , mark);
					cout  << "ENDIF"  << endl;
					break;
				case IF_ELSE:
					cout << "IF " ;
					print_expressions(root->tree.condt_stmt_tree->condition) ;
					cout << "THEN" << endl ; 
					ast_printing(root->tree.condt_stmt_tree->stmts1 , mark);
					cout << "ELSE" << endl;
					ast_printing(root->tree.condt_stmt_tree->stmts2 , mark);
					cout << "ENDIF"  << endl;
					break;
				case WHILE_CONDT:
					cout << "WHILE " ;
					print_expressions(root->tree.condt_stmt_tree->condition) ;
					cout << "\n" << "DO" << endl ;
					ast_printing(root->tree.condt_stmt_tree->stmts1 , mark);
					cout << "ENDWHILE"  << endl;
					break;
				default:
					break;
				}
				break;
			}
			case RETURN_STMT:
				cout << "RETURN " ;
				print_expressions(root->tree.root);
				cout << endl;
				break;
			case END_DECL:
				cout << "ENDDECL" << endl;
				break;
			case FUNC_DEF:
				if(root->tree.main_func_def_tree->ret_type == 0)
				{
					cout << "INTEGER ";
				}
				else
				{
					cout << "BOOL " ;
				}
				cout << "MAIN" << endl;
				cout << "BEGIN" << endl;
				ast_printing(root->tree.main_func_def_tree->stmt_block , mark) ;
				ast_printing(root->tree.main_func_def_tree->return_stmt , mark) ;
				cout << "END" << endl;
				break;
			case UNUSED_STMT:
				return;
			default:
				break;
		}
		// cout << root->type << endl;
		ast_printing(root->next , mark);
	}
	else
	{
		return ;
	}
}


/* expression error,type checking */
void expression_type_checking(struct expr_node* root , int line_num)
{
	if(root != NULL)
	{
		switch (root->type)
		{
			case OP:
			{
				switch (root->op)
				{
					case PLUS_OP:
					case SUB_OP:
					case MUL_OP:
					case GREATERTHAN_OP:
					case LESSTHAN_OP:
					case GREATERTHAN_EQUAL_OP:
					case LESSTHAN_EQUAL_OP:
					case NOTEQUAL_OP:
					case EQUALEQUAL_OP:
					case ASSIGN_OP:
						expression_type_checking(root->left , line_num);
						expression_type_checking(root->right , line_num);

						if( (root->right->type == INTEGER && root->left->type == BOOL) || (root->right->type == BOOL && root->left->type == INTEGER))
						{
							cout << "WARNING: Integer value and boolean value are invloved in arithmetic/logical operations: near line " << line_num << endl;
						}
						break;
					case DIV_OP:
					case REMAINDER_OP:
					{
						expression_type_checking(root->left , line_num);
						expression_type_checking(root->right , line_num);

						if( (root->right->type == INTEGER && root->left->type == BOOL) || (root->right->type == BOOL && root->left->type == INTEGER))
						{
							cout << "WARNING: Integer value and boolean value are invloved in arithmetic/logical operations: near line " << line_num << endl;
						}

						if( (root->right->type == INTEGER && root->right->const_val == 0) || (root->right->type == BOOL && root->right->bool_val == false) )
						{
							cerr << "ERROR:: Division by zero error near line " << line_num << endl;
							error_check_var = 1;
						}
						break;
					}
					default:
						break;
				}
				break;
			}
			case INTEGER:
			case BOOL:
			case STRING_VAR:
				break;
			case VARIABLE:
			{
				pair <define_check , symbol_type> temp;
				temp = symbol_lookup(root->name) ;

				if(temp.first == UNDECL)
				{
					cerr << "ERROR:: Undeclared variable " << root->name << " near line " << line_num << endl;
					error_check_var = 1;
				}

				if(temp.second == VAR_ARRAY_SYM)
				{
					error_check_var = 1;
					cerr << "ERROR:: Index value not given for variable '" << root->name << "' near line " << endl;
				}

				break;
			}
			case ARRAY_ELEMENT:
			{
				pair <define_check , symbol_type> temp;
				temp = symbol_lookup(root->name) ;
				
				if(temp.first == UNDECL)
				{
					cerr << "ERROR:: Undeclared variable found near line " << line_num << endl;
					error_check_var = 1;
				}

				if(temp.second == VAR_SYM)
				{
					error_check_var = 1;
					cerr << "ERROR:: Variable '" << root->name << "' is not an array type: near line " << endl;
				}
				else
				{
					if(root->index->type == INTEGER)
					{
						if(root->index->const_val > symbol_table[root->name].size)
						{
							error_check_var = 1;
							cerr << "ERROR:: Index out of bounds near line " <<  line_num << endl;
						}
					}
				}
				
				break;
			}
			default:
				break;
		}
	}
}


// Checking for errors in semantic Analysis - type checking , re-declaration , index out of bounds...
void semantic_error_checking(struct stmt_list* root)
{
	if( root != NULL )
	{
		switch(root->type)
		{
			case DECL_STMT:
			{
				struct decl_node* temp = root->tree.decl_stmt_tree->node;
				define_check temp_check;

				while(temp != NULL)
				{
					switch (temp->decl_type)
					{
						case VAR_NODE:
							if(root->tree.decl_stmt_tree->ret_type == DECL_INT)
							{
								temp_check = insert_symbol(temp->name , VAR_SYM , UNDEF , INT_SYM , 0);
							}
							else
							{
								temp_check = insert_symbol(temp->name , VAR_SYM , UNDEF , BOOL_SYM , 0);
							}

							if(temp_check == UNDEF || temp_check == DEF)
							{
								error_check_var = 1;
								cerr << "ERROR:: Cannot declare variable '" << temp->name << "' again: line " << root->line_num << endl;
							}
							break;
						case VAR_ARR_NODE:
							if(root->tree.decl_stmt_tree->ret_type == DECL_INT)
							{
								temp_check = insert_symbol(temp->name , VAR_ARRAY_SYM , DEF , INT_SYM , temp->array_size);
							}
							else
							{
								temp_check = insert_symbol(temp->name , VAR_ARRAY_SYM , DEF , BOOL_SYM , temp->array_size);
							}

							if(temp_check == UNDEF || temp_check == DEF)
							{
								error_check_var = 1;
								cerr << "ERROR:: Cannot declare variable '" << temp->name << "' again: line " << root->line_num << endl;
							}
							break;
						default:
							break;
					}
					temp = temp->next;
				}
				break;
			}
			case ASSIGN:
			{
				expression_type_checking(root->tree.root->left ,  root->line_num);
				expression_type_checking(root->tree.root->right , root->line_num);
				break;
			}
			case READ_STMT:
			case WRITE_STMT:
				expression_type_checking(root->tree.root , root->line_num);
				break;
			case CONDT:
			{
				switch (root->tree.condt_stmt_tree->type)
				{
					case IF_CONDT:
						expression_type_checking(root->tree.condt_stmt_tree->condition , root->line_num) ;
						semantic_error_checking(root->tree.condt_stmt_tree->stmts1);
						break;
					case IF_ELSE:
						expression_type_checking(root->tree.condt_stmt_tree->condition , root->line_num) ;
						semantic_error_checking(root->tree.condt_stmt_tree->stmts1);
						semantic_error_checking(root->tree.condt_stmt_tree->stmts2);
						break;
					case WHILE_CONDT:
						expression_type_checking(root->tree.condt_stmt_tree->condition , root->line_num) ;
						semantic_error_checking(root->tree.condt_stmt_tree->stmts1);
						break;
					default:
						break;
				}
				break;
			}
			case END_DECL:
			case RETURN_STMT:
			case UNUSED_STMT:
				break;
			case FUNC_DEF:
				semantic_error_checking(root->tree.main_func_def_tree->stmt_block) ;
				expression_type_checking(root->tree.main_func_def_tree->return_stmt->tree.root , root->tree.main_func_def_tree->return_stmt->line_num) ;

				if ((root->tree.main_func_def_tree->return_stmt->tree.root->type == INTEGER && root->tree.main_func_def_tree->ret_type == DECL_BOOL) || 
					(root->tree.main_func_def_tree->return_stmt->tree.root->type == BOOL    && root->tree.main_func_def_tree->ret_type == DECL_INT)) 
				{
					error_check_var = 1;
					cerr << "Unmatched return type for function main: line" << root->line_num << endl;
				}
				break;
			default:
				break;
		}
		semantic_error_checking(root->next);
	}
	else
	{
		return ;
	}
}

