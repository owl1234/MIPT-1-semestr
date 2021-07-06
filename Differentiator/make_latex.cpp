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

	if(node->type == OPERATOR && COUNT_OF_ARGS[node->value] == 1) {
		fprintf(latex, "%s(", TEXT_OPERATIONS[node->value]);
		latex_node(node->left, latex);
		fprintf(latex, ")");

		return;
	} 	

	if(node->type == NUMBER)
		fprintf(latex, "%d", node->value);
	else if(node->type == VARIABLE)
		fprintf(latex, "%c", node->value);
	else {
		if(node->value == ADD || node->value == SUB)
			fprintf(latex, "(");

		latex_node(node->left,  latex);
		fprintf(latex, "%s", TEXT_OPERATIONS[node->value]);
		latex_node(node->right, latex);
		
		if(node->value == ADD || node->value == SUB)
			fprintf(latex, ")");
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