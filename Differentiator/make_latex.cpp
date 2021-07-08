#include <stdio.h>
#include "make_latex.h"

void latex_tree(const Tree* tree, FILE* latex) {
	if(!tree || !latex)
		return;

	fprintf(latex, "$$ ");
	latex_node(tree->root, latex);
	fprintf(latex, "$$\n");

	return;
}

void latex_node(const Node* node, FILE* latex) {
	if(!node || !latex)
		return;

	if(node->type == OPERATOR && COUNT_OF_ARGS[(int)node->value] == 1) {
		fprintf(latex, "%s", TEXT_OPERATIONS[(int)node->value]);
		if((node->value == SIN || node->value == COS) && node->left->type == NUMBER ||
		  ((node->value == LG || node->value == LN) && node->left->value == POW))		// add other functions with one argument
			fprintf(latex, "(");

		latex_node(node->left, latex);

		if((node->value == SIN || node->value == COS) && node->left->type == NUMBER ||
		  ((node->value == LG || node->value == LN) && node->left->value == POW))		// add other functions with one argument
			fprintf(latex, ")");

		return;
	} 	

	if(node->type == NUMBER)
		fprintf(latex, "%lg", node->value);

	else
	if(node->type == VARIABLE)
		fprintf(latex, "%c", (int)node->value);
	else {
		if(node->value == ADD || node->value == SUB || node->value == POW && node->left->value != NUMBER)
			fprintf(latex, "(");

		latex_node(node->left,  latex);

		if(node->value == POW && node->left->value != NUMBER)
			fprintf(latex, ")");

		fprintf(latex, "%s", TEXT_OPERATIONS[(int)node->value]);

		if(node->value == POW)
			fprintf(latex, "{");

		else
		if(node->value == DIV && node->right->type == OPERATOR)
			fprintf(latex, "(");	

		latex_node(node->right, latex);
		
		if(node->value == ADD || node->value == SUB || node->value == DIV && node->right->type == OPERATOR)
			fprintf(latex, ")");

		else
		if(node->value == POW)
			fprintf(latex, "}");		
	}
}

void test_latex(const Node* node, FILE* latex, const char* str) {
	if(!node || !latex)
		return;

	//printf("\t\t\t%s\n", str);
	fprintf(latex, "%s $$ ", str);
	latex_node(node, latex);
	fprintf(latex, "$$\n");

	return;
}