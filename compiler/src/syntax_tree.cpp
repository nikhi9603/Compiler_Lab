#include <iostream>
#include "syntax_tree.h"
#include "sym_table.h"
using namespace std;

/* EXPRESSION TREE NODE */
struct expr_node*  createExpr_Node(expr_type type , struct expr_node* left , struct expr_node* right , char op , double val , char *var_name) 
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
		case 0: 		/* Type 0 : operators + , * , / , -  */
				newNode->node_type_obj.op = op;
				break;
		case 1:			/* Type 1 : constants */
				newNode->node_type_obj.val = val;				
				break;
		case 2:
				newNode->node_type_obj.variable_name = var_name;
				break;		
		default:
				/* raise error */
				cerr << "Invalid expression" << endl ;
				exit(0);
	}

    return newNode;
}

/* NODE CREATION */

/* ASSIGNMENT STATEMENT NODE */
struct node* createNode(char* var_name , struct node* next_node)
{
	struct node* newNode;

	// allocating memory for newNodes
	newNode = (struct node*)malloc(sizeof(struct node)) ;
	
	if(newNode == NULL)
	{
		cerr << "Memory not available to allocate new nodes\n" << endl ;
		exit(0);
	}

	newNode -> var_name = var_name ;
	newNode -> next = next_node ;
	return newNode ;
}

/* FUNCTION TREE NODE */
struct funcStmt_node* createFunc_Node(struct expr_node* expr , struct funcStmt_node* next)
{
	struct funcStmt_node* newNode;

	// allocating memory for newNodes
	newNode = (struct funcStmt_node*)malloc(sizeof(struct funcStmt_node)) ;
	
	if(newNode == NULL)
	{
		cerr << "Memory not available to allocate new nodes\n" << endl ;
		exit(0);
	}
	newNode->expr_root = expr;
	newNode->next = next ;
	return newNode ;
}

/* DECLARATION STATEMENT NODE */
struct declstmt_tree* createDeclStmt_Node(var_type type, struct node *decl_var , struct declstmt_tree *next)
{
	struct declstmt_tree* newNode;

	// allocating memory for newNodes
	newNode = (struct declstmt_tree*)malloc(sizeof(struct declstmt_tree)) ;
	
	if(newNode == NULL)
	{
		cerr << "Memory not available to allocate new nodes\n" << endl ;
		exit(0);
	}
	
	newNode->type = type;
	newNode->decl_var = decl_var;
	newNode->next = next ;
	return newNode ;
}

double evaluate_Expr_Tree(struct expr_node *root , int i) 
{
	double result = 0;
	struct symbol_details *sym ;
	
	// Type - 0 : operator
	if(root->type == OP)
	{
		// Evaluate left and right subtrees
		double left_val = evaluate_Expr_Tree(root->left , 0) ;
		double right_val = evaluate_Expr_Tree(root->right , 0) ;
	
		switch(root->node_type_obj.op)
		{
			case '+' :
				result = left_val + right_val ;
				break;
			case '-' :
				result = left_val - right_val ;
				break;
			case '*' :
				result = left_val * right_val ;
				break;
			case '/' :
				if(right_val != 0)
				{
					result = left_val / right_val ;
					break;
				}
				else
				{
					cerr << "Division by Zero Error\n" << endl;
					exit(0);
				}
			default:		
				break;
		}
	}
	else if(root->type == CONSTANT)	// type - 1: constants
	{
		result = root->node_type_obj.val;
	}
	else if (root->type == VARIABLE)
	{
		sym = symbol_lookup(root->node_type_obj.variable_name);

		// undeclared variables or undefined variables both are wrong in expressions
		if( sym->check == UNDECL)
		{
			cerr << "Undeclared variable " << root->node_type_obj.variable_name << endl ;
			exit(0);
		}
		else if ( sym->check==UNDEF && i == 0 )
		{
			cout << "Warning: Variable" << root->node_type_obj.variable_name << "is being used without initializing" << endl;
			result = sym->val ;
		}
		else if (sym->check==UNDEF && i == 1)
		{
			cout<< "UNDEF" << endl ;
			result = -100;
		}
		result = sym->val ;
		
	}
	return result ;
}

void print_func_tree(struct funcStmt_node *root)
{
	struct funcStmt_node *temp = root ;
	double result = 0;
	struct symbol_details *sym ;

	while(temp != NULL)
	{
		if(temp->expr_root->type == VARIABLE)
		{
			sym = symbol_lookup(temp->expr_root->node_type_obj.variable_name);
			if(sym->check == UNDECL)
			{
				cerr << "Undeclared variable " << temp->expr_root->node_type_obj.variable_name << endl ;
				exit(0);
			}
			else if (sym->check == UNDEF)
			{
				cout << "UNDEF" << endl ;
			}
			else
			{
				result = sym->val;
				cout << result << endl ;
			}
		}
		else 
		{
			result = evaluate_Expr_Tree(temp->expr_root,0);
			cout <<  result << endl ;
		}
		temp = temp -> next ;
	}
}

void evaluate_assign_stmt(struct expr_node *root)
{
	double res = evaluate_Expr_Tree(root->right , 0);
	update_symbol_details(root->left->node_type_obj.variable_name , res);
}

void evaluate_declare_stmt(struct declstmt_tree *root)
{
	while( root != NULL)
	{
		struct node* temp = root -> decl_var ;
		struct symbol_details *sym ;

		while(temp != NULL)
		{
			sym = symbol_lookup(temp->var_name) ;

			if(sym->check != UNDECL)
			{
				cerr << "Re-declaring variable " << temp->var_name << endl ;
				exit(0);
			}
			else
			{
				insert_symbol(temp->var_name , INTEGER_SYM , UNDEF , "integer" , 0);
			}
			temp = temp->next ;
		}
		root = root -> next;
	}
}

struct statement* createStmt(stmt_type type , struct declstmt_tree *decl_tree , struct expr_node* assignStmt_tree , struct funcStmt_node *func_tree , struct statement* next)
{
	struct statement* newNode;

	// allocating memory for newNodes
	newNode = (struct statement*)malloc(sizeof(struct statement)) ;
	
	if(newNode == NULL)
	{
		cerr << "Memory not available to allocate new nodes\n" << endl ;
		exit(0);
	}

	newNode->type = type ;
	newNode->next = next ;

	switch (newNode->type)
	{
		case DECL_STMT:
			newNode->tree.decl_tree = decl_tree ;
			break;
		case ASSIGN_STMT:
			newNode->tree.assignStmt_tree = assignStmt_tree ;
			break;
		case PRINTFN_STMT:
			newNode->tree.func_tree = func_tree;
			break;
		default:
			cerr << "Unknown type of statement" << endl ;
			exit(0);
	}

	return newNode;
}

void evaluate_program(struct statement *root)
{
	struct statement *temp = root ;

	while(temp != NULL)
	{
		switch(temp->type)
		{
			case DECL_STMT:
				evaluate_declare_stmt(temp->tree.decl_tree);
				break;
			case ASSIGN_STMT:
				evaluate_assign_stmt(temp->tree.assignStmt_tree);
				break;
			case PRINTFN_STMT:
				print_func_tree(temp->tree.func_tree);
				break;
			default:
				break;
		}
		temp = temp->next ;
	}
}
