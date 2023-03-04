// Nodes of syntax tree
struct node 
{
	/* Type can be 0,1 */
	/* Type 0 : operators + , * , / , -  */
	/* Type 1 : constants */
	int type; 
	double val;
	char op;
	struct node *left;
	struct node *right;
};

// Creating new nodes 
struct node* createNode(int type , double val , char op , struct node* left , struct node* right) ;


// Evaluating syntax tree
double evaluateSyntaxTree(struct node *root) ;


