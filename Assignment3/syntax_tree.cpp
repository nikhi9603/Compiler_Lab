#include <iostream>
#include "syntax_tree.h"
#include "sym_table.h"
using namespace std;
#include <cstring>

extern int error_check_var ;

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
				newNode->params = params;
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
				cerr << "Error: Invalid expression" << endl ;
				exit(0);
	}
	return newNode;
}

/* DECLARATION TREE NODE */
struct decl_node* createDecl_Node(decl_node_type type , char* name , int size , struct stmt_list *args)
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
	else if (type == FUNC_NODE)
	{
		newNode->args = args;
	}	

	newNode->next = NULL;
	return newNode;
}


/* CREATING STATEMENT TREE OF TYPE USING expr_node as tree in structure stmt_list */
struct stmt_list* create_Stmt(stmt_type type , int line_num , struct expr_node* expr1 , struct expr_node* expr2)
{
	struct stmt_list* new_stmt = (struct stmt_list*)malloc(sizeof(struct stmt_list)) ;

	if(new_stmt == NULL)
	{
		cerr << "Memory not available to allocate new stmt node\n" << endl ;
		exit(0);
	}

	new_stmt->type = type;
	new_stmt->line_num = line_num;
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
			cerr << "Invalid statement" << line_num<< endl;
			exit(0);
	}
	return new_stmt;
}


/* creating CONDITIONAL STATEMENT tree */
struct stmt_list* create_Condt_Stmt(condt_type type , int line_num , struct expr_node* condition , struct stmt_list* stmt1 , struct stmt_list* stmt2)
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
	new_stmt->line_num = line_num;
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
			exit(0);
	}
	return new_stmt;
}


/* creating an extra node to specify that declaration block is ended */
struct stmt_list* create_Enddecl_Stmt(int n)
{

	struct stmt_list* new_stmt = (struct stmt_list*)malloc(sizeof(struct stmt_list)) ;

	if(new_stmt == NULL)
	{
		cerr << "Memory not available to allocate new stmt node\n" << endl ;
		exit(0);
	}
	new_stmt->type = END_DECL ;
	new_stmt->line_num = n;		// 0 for global enddecl , 1 for local enddecl
	new_stmt->next = NULL;
	return new_stmt;
}

/* creating an extra node to specify that declaration block is start */
struct stmt_list* create_decl_start_stmt()
{

	struct stmt_list* new_stmt = (struct stmt_list*)malloc(sizeof(struct stmt_list)) ;

	if(new_stmt == NULL)
	{
		cerr << "Memory not available to allocate new stmt node\n" << endl ;
		exit(0);
	}
	new_stmt->type = DECL_START ;
	new_stmt->next = NULL;
	return new_stmt;
}

/* DECLARATION  STATEMENT */
struct stmt_list* create_Decl_Stmt(int ret_type , struct decl_node* node , int line_num)
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
	// new_stmt->tree.decl_stmt_tree->scope = scope ;
	new_stmt->tree.decl_stmt_tree->node = node;
	new_stmt->line_num = line_num;

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
struct stmt_list* create_function(int ret_type , char* fun_name , struct stmt_list* argList , struct stmt_list *decl_block , struct stmt_list* stmt_block , struct stmt_list* return_stmt , int line_num)
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
	new_stmt->tree.func_def_tree->argList = argList;
	new_stmt->tree.func_def_tree->func_name = fun_name ;
	new_stmt->tree.func_def_tree->decl_block = decl_block ;
	new_stmt->tree.func_def_tree->stmt_block = stmt_block ;
	new_stmt->tree.func_def_tree->return_stmt = return_stmt ;
	new_stmt->line_num = line_num;
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
struct stmt_list* create_return_stmt(struct expr_node *expr , int line_num)
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
	new_stmt->line_num = line_num;

	return new_stmt;
}


// PRINTING ABSTRACT SYNTAX TREE
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
					case LOGICAL_NOT_OP : 
						cout << "LOGICAL_NOT " ;
						break;
					case LOGICAL_AND_OP : 
						cout << "LOGICAL_AND " ;
						break;
					case LOGICAL_OR_OP : 
						cout << "LOGICAL_OR " ;
						break;
					default:
						break;
				}
				break;
			}
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
				print_expressions(root->params);
				break;
			case STRING_VAR:
				cout << "STRING ";
				break;
			case FUNCTION_CALL:
			{
				cout << "FUNC_CALL " ;
				struct expr_node* temp = root->params;
				while(temp != NULL)
				{
					print_expressions(temp);
					temp = temp->params;
				}
				break;
			}
			default:
				break;
		}
		// LEFT TREE
		print_expressions(root->left);
		// RIGHT TREE
		print_expressions(root->right);
	}
}

void ast_printing(struct stmt_list* root)
{
	if( root != NULL )
	{
		switch(root->type)
		{
			case DECL_STMT:
			{
				// if(mark == 0)
				// {
				// 	cout << "DECL" << endl;
				// 	mark = 1 ;
				// }

				if(root->tree.decl_stmt_tree->ret_type == 0)
				{
					cout << "\tINTEGER ";
				}
				else
				{
					cout << "\tBOOL " ;
				}
				
				if(root->tree.decl_stmt_tree->node->decl_type == VAR_NODE)
				{
					cout << "VAR" ;
				}
				else if (root->tree.decl_stmt_tree->node->decl_type == VAR_ARR_NODE)
				{
					cout << "VAR_ARRAY " << root->tree.decl_stmt_tree->node->array_size ;
				}
				else if( root->tree.decl_stmt_tree->node->decl_type == FUNC_NODE)
				{
					cout << "FUNC_NAME " << endl;
					if(root->tree.decl_stmt_tree->node->args != NULL)
					{
						cout << "\tFUNC_ARGS_START" << endl;
						ast_printing(root->tree.decl_stmt_tree->node->args) ;
						cout << "\tFUNC_ARGS_END" << endl;
					}
					break;
				}

				struct decl_node* temp = root->tree.decl_stmt_tree->node->next ;
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
				cout << "\tASSIGN " ;
				print_expressions(root->tree.root->left);
				print_expressions(root->tree.root->right);
				cout << endl;
				break;
			case READ_STMT:
				cout << "\tREAD " ;
				if(root->tree.root->type == VARIABLE)
				{
					cout << "VAR " ;
				}
				else
				{
					cout << "VAR_ARRAY_ELEMENT VAR " ;
					print_expressions(root->tree.root->params) ;
				}	
				cout << endl;
				break;
			case WRITE_STMT:
				cout << "\tWRITE " ;
				print_expressions(root->tree.root) ;
				cout << endl;
				break;
			case CONDT:
			{
				switch (root->tree.condt_stmt_tree->type)
				{
				case IF_CONDT:
					cout << "\tIF " ;
					print_expressions(root->tree.condt_stmt_tree->condition) ;
					cout << "\n" << "\tTHEN" << endl ;
					ast_printing(root->tree.condt_stmt_tree->stmts1);
					cout  << "\tENDIF"  << endl;
					break;
				case IF_ELSE:
					cout << "\tIF " ;
					print_expressions(root->tree.condt_stmt_tree->condition) ;
					cout << "\n" << "\tTHEN" << endl ; 
					ast_printing(root->tree.condt_stmt_tree->stmts1);
					cout << "\tELSE" << endl;
					ast_printing(root->tree.condt_stmt_tree->stmts2);
					cout << "\tENDIF"  << endl;
					break;
				case WHILE_CONDT:
					cout << "\tWHILE " ;
					print_expressions(root->tree.condt_stmt_tree->condition) ;
					cout << "\n" << "\tDO" << endl ;
					ast_printing(root->tree.condt_stmt_tree->stmts1);
					cout << "\tENDWHILE"  << endl;
					break;
				default:
					break;
				}
				break;
			}
			case RETURN_STMT:
				cout << "\tRETURN " ;
				print_expressions(root->tree.root);
				cout << endl;
				break;
			case END_DECL:
				if(root->line_num == 0)
				{
					cout << "ENDDECL" << endl;
					cout << endl;
				}
				else
				{
					cout << "ENDDECL" << endl;
				}
				break;
			case FUNC_DEF:
				cout << endl;
				if(root->tree.func_def_tree->ret_type == 0)
				{
					cout << "INTEGER ";
				}
				else
				{
					cout << "BOOL " ;
				}

				if(strcmp(root->tree.func_def_tree->func_name , "main") != 0)
				{
					cout << "FUNC_NAME" << endl;
				}
				else
				{
					cout << "MAIN" << endl;
				}

				if(root->tree.func_def_tree->argList != NULL)
				{
					cout << "FUNC_ARGS_START" << endl;
					ast_printing(root->tree.func_def_tree->argList);
					cout << "FUNC_ARGS_END" << endl;
				}

				ast_printing(root->tree.func_def_tree->decl_block);

				cout << "BEGIN" << endl;
				ast_printing(root->tree.func_def_tree->stmt_block) ;
				ast_printing(root->tree.func_def_tree->return_stmt) ;
				cout << "END" << endl;

				cout << endl;
				break;
			case FUNC_CALL:
				cout << "\t" ;
				print_expressions(root->tree.root);
				cout << endl;
				break;
			case UNUSED_STMT:
				return;
			case DECL_START:
				cout << "DECL" << endl;
				break;
			default:
				break;
		}
		// cout << root->type << endl;
		ast_printing(root->next);
	}
	else
	{
		return ;
	}
}





// ERROR CHECKING (SEMANTIC ERRORS)
void expression_type_checking(struct expr_node* root , int line_num , char* func_name)
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
						expression_type_checking(root->left , line_num , func_name);
						expression_type_checking(root->right , line_num , func_name);

						if( (root->right->type == INTEGER && root->left->type == BOOL) || (root->right->type == BOOL && root->left->type == INTEGER) )
						{
							cout << "WARNING: Integer value and boolean value are invloved in arithmetic/relational operations: near line " << line_num << endl;
						}
						break;
					case DIV_OP:
					case REMAINDER_OP:
					{
						expression_type_checking(root->left , line_num , func_name);
						expression_type_checking(root->right , line_num , func_name);

						if( (root->right->type == INTEGER && root->left->type == BOOL) || (root->right->type == BOOL && root->left->type == INTEGER))
						{
							cout << "WARNING: Integer value and boolean value are invloved in arithmetic/relational operations: near line " << line_num << endl;
						}

						if( (root->right->type == INTEGER && root->right->const_val == 0) || (root->right->type == BOOL && root->right->bool_val == false) )
						{
							cerr << "ERROR:: Division by zero error near line " << line_num << endl;
							error_check_var = 1;
						}
						break;
					}
					case LOGICAL_AND_OP:
					case LOGICAL_NOT_OP:
					case LOGICAL_OR_OP:
						expression_type_checking(root->left , line_num , func_name);
						break;
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
				tuple<define_check , symbol_type , scope> temp;
				temp = symbol_lookup(func_name , root->name) ;

				if(get<0>(temp) == UNDECL)
				{
					cerr << "ERROR:: Undeclared variable " << root->name << " near line " << line_num << endl;
					error_check_var = 1;
				}

				if(get<1>(temp) == VAR_ARRAY_SYM)
				{
					error_check_var = 1;
					cerr << "ERROR:: Index value not given for variable '" << root->name << "' near line " << line_num<< endl;
				}
				else if(get<1>(temp) == FUNC_SYM)
				{
					error_check_var = 1;
					cerr << "ERROR:: Invalid access of function '" << root->name << "' near line " << line_num<< endl;
				}
				break;
			}
			case ARRAY_ELEMENT:
			{
				tuple<define_check , symbol_type , scope> temp;
				temp = symbol_lookup(func_name , root->name) ;
				
				if(get<0>(temp) == UNDECL)
				{
					cerr << "ERROR:: Undeclared variable found near line " << line_num << endl;
					error_check_var = 1;
				}

				if(get<1>(temp) == VAR_SYM)
				{
					error_check_var = 1;
					cerr << "ERROR:: Variable '" << root->name << "' is not an array type: near line " << line_num << endl;
				}
				else if(get<1>(temp) == VAR_ARRAY_SYM)
				{
					if(root->params->type == INTEGER|| root->params->type == BOOL)
					{
						if(get<2>(temp) == GLOBAL)
						{
							if(root->params->const_val >= Symbol_table["global"][root->name].size || root->params->const_val < 0)
							{
								error_check_var = 1;
								cerr << "ERROR:: Index out of bounds near line " <<  line_num << endl;
							}
						}
						else if(root->params->const_val >= Symbol_table[func_name][root->name].size || root->params->const_val < 0 )
						{	
							error_check_var = 1;
							cerr << "ERROR:: Index out of bounds near line " <<  line_num << endl;
						}
					}
					else 
					{
						expression_type_checking(root->params , line_num , func_name);
					}
				}
				else 
				{
					error_check_var = 1;
					cerr << "ERROR:: Invalid access of function near line " <<  line_num << endl;
				}
				break;
			}
			case FUNCTION_CALL:
			{
				tuple<define_check , symbol_type , scope> sym_check = symbol_lookup(func_name , root->name);

				if(get<0>(sym_check) == UNDECL)
				{
					error_check_var = 1;
					cerr << "ERROR:: Undeclared function '" << root->name << "' near line " << line_num << endl;
				}

				// checking number of arguments are same or not
				struct expr_node* temp = root->params;
				int i = 0;

				while(temp != NULL)
				{
					expression_type_checking(temp , line_num , func_name);
					temp = temp->params;
					i++;
				}

				if(i != Symbol_table["global"][root->name].size)
				{
					error_check_var = 1;
					cerr << "ERROR:: Mismatch of number of arguments passed to the function '" << root->name << "' near line" << line_num << endl;
				}
				break;
			}
			default:
				break;
		}
	}
}


// Checking for errors in semantic Analysis - type checking , re-declaration , index out of bounds...
void semantic_error_checking(struct stmt_list* root , char* func_name)
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
								temp_check = insert_symbol(func_name , temp->name , VAR_SYM , INT_SYM , 0 , NULL);
							}
							else
							{
								temp_check = insert_symbol(func_name, temp->name , VAR_SYM , BOOL_SYM , 0 , NULL);
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
								temp_check = insert_symbol(func_name, temp->name , VAR_ARRAY_SYM , INT_SYM , temp->array_size , NULL);
							}
							else
							{
								temp_check = insert_symbol(func_name, temp->name , VAR_ARRAY_SYM , BOOL_SYM , temp->array_size , NULL);
							}

							if(temp_check == UNDEF || temp_check == DEF)
							{
								error_check_var = 1;
								cerr << "ERROR:: Cannot declare variable '" << temp->name << "' again: line " << root->line_num << endl;
							}

							if(!(temp->array_size > 0))
							{
								error_check_var = 1;
								cerr << "ERROR:: Invalid size when declaring array '" << temp->name << "' near line " << root->line_num << endl;
							}

							break;
						case FUNC_NODE:
						{
							if(root->tree.decl_stmt_tree->ret_type == DECL_INT)
							{
								temp_check = insert_symbol(func_name, temp->name , FUNC_SYM , INT_SYM , 0 , NULL);
							}
							else
							{
								temp_check = insert_symbol(func_name, temp->name , FUNC_SYM , BOOL_SYM , 0 , NULL);
							}
							// cout << (temp->name) << endl;

							if(temp_check == UNDEF || temp_check == DEF)
							{
								error_check_var = 1;
								cerr << "ERROR:: Cannot declare function '" << temp->name << "' again: line " << root->line_num << endl;
							}
							else
							{
								struct stmt_list* temp_args = temp->args ;
								int i = 0;
								struct parameters_list* temp_params ;
								struct parameters_list* cur = NULL;

								while (temp_args != NULL)
								{
									struct decl_node* temp_node = temp_args->tree.decl_stmt_tree->node;
									while(temp_node != NULL)
									{
										if(temp_args->tree.decl_stmt_tree->ret_type == DECL_INT)
										{
											temp_params = (struct parameters_list*)malloc(sizeof(struct parameters_list)) ;
											temp_params->sym_val_type = INT_SYM ;
											temp_params -> next = NULL;	
										}
										else
										{
											temp_params = (struct parameters_list*)malloc(sizeof(struct parameters_list)) ;
											temp_params->sym_val_type = BOOL_SYM ;
											temp_params -> next = NULL;	
										}
										
										if(i == 0)
										{
											Symbol_table[func_name][temp->name].parameters = temp_params;
											// cout << temp_params->sym_val_type<<endl;
										}

										temp_node = temp_node -> next;
										
										if(cur == NULL)
										{
											cur = temp_params;
										}
										else
										{
											cur -> next = temp_params ;
											cur = cur -> next ;
										}


										i++;
									}
									temp_args = temp_args->next;
								}	
								Symbol_table[func_name][temp->name].size = i;

								Symbol_table[temp->name].clear();
								// cout << Symbol_table[func_name][temp->name].parameters->sym_val_type << endl;
							}
							break;
						}
						default:
							break;
					}
					temp = temp->next;
				}
				break;
			}
			case ASSIGN:
			{
				expression_type_checking(root->tree.root->left ,  root->line_num , func_name);
				expression_type_checking(root->tree.root->right , root->line_num , func_name);
				break;
			}
			case READ_STMT:
			case WRITE_STMT:
				expression_type_checking(root->tree.root , root->line_num , func_name);
				break;
			case CONDT:
			{
				switch (root->tree.condt_stmt_tree->type)
				{
					case IF_CONDT:
						expression_type_checking(root->tree.condt_stmt_tree->condition , root->line_num , func_name) ;
						semantic_error_checking(root->tree.condt_stmt_tree->stmts1 , func_name);
						break;
					case IF_ELSE:
						expression_type_checking(root->tree.condt_stmt_tree->condition , root->line_num , func_name) ;
						semantic_error_checking(root->tree.condt_stmt_tree->stmts1 , func_name);
						semantic_error_checking(root->tree.condt_stmt_tree->stmts2 , func_name);
						break;
					case WHILE_CONDT:
						expression_type_checking(root->tree.condt_stmt_tree->condition , root->line_num , func_name) ;
						semantic_error_checking(root->tree.condt_stmt_tree->stmts1 , func_name);
						break;
					default:
						break;
				}
				break;
			}
			case END_DECL:
			case DECL_START:
			case RETURN_STMT:
				// CHECK LATER
			case UNUSED_STMT:
				break;
			case FUNC_DEF:
			{
				tuple<define_check , symbol_type , scope> sym_check = symbol_lookup(func_name , root->tree.func_def_tree->func_name);
				define_check temp_check;
				// cout << func_name << " " << root->tree.func_def_tree->func_name << endl;

				if(get<0>(sym_check) == UNDECL && strcmp(root->tree.func_def_tree->func_name , "main") != 0)
				{
					error_check_var= 1;
					cerr << "ERROR:: Undeclared function '" << root->tree.func_def_tree->func_name << "' near line " << root->line_num << endl;
				}
				else if(strcmp(root->tree.func_def_tree->func_name , "main") == 0 &&  (get<0>(sym_check) != UNDECL))
				{
					error_check_var = 1;
					cerr << "ERROR:: 'main' is a keyword. It cannot be used as any variable name" << endl;
					// cerr << get<0>(sym_check) << " " << get<1>(sym_check) << " " << get<2>(sym_check) << endl;
				}
				else if(strcmp(root->tree.func_def_tree->func_name , "main") == 0 &&  (get<0>(sym_check) == UNDECL))
				{
					
					if(root->tree.decl_stmt_tree->ret_type == DECL_INT)
					{
						temp_check = insert_symbol("global", "main" , FUNC_SYM , INT_SYM , 0 , NULL);
					}
					else
					{
						temp_check = insert_symbol("global", "main" , FUNC_SYM , BOOL_SYM , 0 , NULL);
					}
				}
			
				semantic_error_checking(root->tree.func_def_tree->argList , root->tree.func_def_tree->func_name);

				// type checking of arguments with its declaration
				struct stmt_list* temp_args = root->tree.func_def_tree->argList;
				struct parameters_list* param_list = Symbol_table["global"][root->tree.func_def_tree->func_name].parameters ;
				int i = 0;

				while (temp_args != NULL && param_list != NULL)
				{
					struct decl_node* temp_node = temp_args->tree.decl_stmt_tree->node;

					while(temp_node != NULL && param_list != NULL)
					{
						if( !((temp_args->tree.decl_stmt_tree->ret_type == DECL_INT && param_list->sym_val_type == INT_SYM) || (temp_args->tree.decl_stmt_tree->ret_type == DECL_BOOL && param_list->sym_val_type == BOOL_SYM)))
						{
							error_check_var = 1;
							cerr << "ERROR: Arguments type is mismatched with its declaration for function '" <<  root->tree.func_def_tree->func_name << "' near line " << root->line_num << endl;
						}
						temp_node = temp_node -> next;
						param_list = param_list->next;
						i++;
					}
					temp_args = temp_args->next;
				}

				// checking number of arguments
				if(i != Symbol_table["global"][root->tree.func_def_tree->func_name].size)
				{
					cerr << "ERROR : Number of arguments is mismatched with its declaration for function '" << root->tree.func_def_tree->func_name << "' near line " << root->line_num << endl;
					error_check_var = 1;
					// cout << i << Symbol_table["global"][root->tree.func_def_tree->func_name].size;
				}

				// type checking of return type of function with its declaration
				if( ! ((root->tree.func_def_tree->ret_type == DECL_INT && Symbol_table[func_name][root->tree.func_def_tree->func_name].sym_val_type == INT_SYM) || (root->tree.func_def_tree->ret_type == DECL_BOOL && Symbol_table[func_name][root->tree.func_def_tree->func_name].sym_val_type == BOOL_SYM)))
				{
					cerr << "ERROR : Return type for function '" << root->tree.func_def_tree->func_name << "' is misatched with its declaration near line" << root->line_num << endl;
					error_check_var = 1;
				}

				semantic_error_checking(root->tree.func_def_tree->decl_block , root->tree.func_def_tree->func_name) ;
				semantic_error_checking(root->tree.func_def_tree->stmt_block , root->tree.func_def_tree->func_name) ;
				expression_type_checking(root->tree.func_def_tree->return_stmt->tree.root , root->tree.func_def_tree->return_stmt->line_num , root->tree.func_def_tree->func_name) ;		

				// if ((root->tree.main_func_def_tree->return_stmt->tree.root->type == INTEGER && root->tree.main_func_def_tree->ret_type == DECL_BOOL) || 
				// 	(root->tree.main_func_def_tree->return_stmt->tree.root->type == BOOL    && root->tree.main_func_def_tree->ret_type == DECL_INT)) 
				// {
				// 	error_check_var = 1;
				// 	cerr << "Unmatched return type for function main: line" << root->line_num << endl;
				// }
				break;
			}
			case FUNC_CALL:
			{
				expression_type_checking(root->tree.root , root->line_num , func_name);
				break;
			}
			default:
				break;
		}
		semantic_error_checking(root->next , func_name);
	}
	else
	{
		return ;
	}
}