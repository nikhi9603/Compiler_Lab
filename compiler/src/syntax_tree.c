#include<stdio.h>
#include "syntax_tree.h"
#include<stdlib.h>

// Creating new nodes 
struct node* createNode(int type , double val , char op , struct node* left , struct node* right) 
{
	struct node* newNode ;
	
	// allocating memory for newNodes
	newNode = (struct node*)malloc(sizeof(struct node)) ;
	
	if(newNode == NULL)
	{
		printf("Memory not available to allocate new nodes\n");
		exit(-1);
	}
	
	// Irrespective of types, we need to add all inputs to newNode
	// While evaluating syntax trees, types are useful
	newNode->val = val ;
	newNode->op = op ;
	newNode->type = type ;
 	newNode->left = left ;
	newNode->right = right ;
	
	return newNode;
}


// Evaluating syntax tree
double evaluateSyntaxTree(struct node *root) 
{	
	double result = 0;
	
	// Type - 0 : operator
	if(root->type == 0)
	{
		// Evaluate left and right subtrees
		double left_val = evaluateSyntaxTree(root->left) ;
		double right_val = evaluateSyntaxTree(root->right) ;
	
		switch(root->op)
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
					printf("Division by Zero Error\n");
					exit(0);
				}
			default:		
				break;
		}
	}
	else if(root->type == 1)	// type - 1: constants
	{
		result = root->val;
	}
	return result ;
}


