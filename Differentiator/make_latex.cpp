#include <stdio.h>
#include "make_latex.h"

void latex_tree(const Tree* tree, FILE* latex) {
	if(!tree || !latex)
		return;

	fprintf(latex, "$$ "); //"\\begin{equation} ");
	latex_node(tree->root, latex, true);
	fprintf(latex, " $$\n"); //\\end{equation}\n");

	return;
}

bool latex_node(const Node* node, FILE* latex, bool is_eval_into_brackets) {
	if(!node || !latex)
		return false;

	if(node->type == OPERATOR && COUNT_OF_ARGS[(int)node->value] == 1) {
		fprintf(latex, "%s(", TEXT_OPERATIONS[(int)node->value]);
		latex_node(node->left, latex, true);
		fprintf(latex, ")");

		return true;
	} 	

	if(node->type == NUMBER) {
		if(node->value < 0)
			fprintf(latex, "(%lg)", node->value);
		else
			fprintf(latex, "%lg", node->value);
	}

	else
	if(node->type == VARIABLE) {
		fprintf(latex, "%c", (int)node->value);
		return false;
	}
	else
	if(node->type == OPERATOR && node->value == DIV) {
		fprintf(latex, "\\frac{");
		latex_node(node->left,  latex, true);
		fprintf(latex, "}{");
		latex_node(node->right,  latex, true);
		fprintf(latex, "}");
		return false;
	}

	else
	if(node->type == OPERATOR && node->value == POW) {
		bool flag = false;
		if(node->left->type == NUMBER && !is_integer(node->left->value) || node->left->type != VARIABLE)
			fprintf(latex, "("), flag = true;

		latex_node(node->left,  latex, flag);

		if(node->left->type == NUMBER && !is_integer(node->left->value) || node->left->type != VARIABLE)
			fprintf(latex, ")");

		fprintf(latex, "^");

		if(node->right->type == NUMBER && !is_integer(node->right->value) || node->right->type != VARIABLE)
			fprintf(latex, "{");

		latex_node(node->right,  latex, flag);

		if(node->right->type == NUMBER && !is_integer(node->right->value) || node->right->type != VARIABLE)
			fprintf(latex, "}");
	}
	else {
		bool flag = false;

		if((node->value == ADD || node->value == SUB) && is_eval_into_brackets == false)
			fprintf(latex, "("), flag = true;

		latex_node(node->left,  latex, !is_eval_into_brackets);

		fprintf(latex, "%s", TEXT_OPERATIONS[(int)node->value]);

		latex_node(node->right, latex, !is_eval_into_brackets);
		
		if((node->value == ADD || node->value == SUB) && is_eval_into_brackets == false)
			fprintf(latex, ")");

		return flag;	
	}

	return false;
}

void test_latex(const Node* node, FILE* latex, const char* str) {
	if(!node || !latex)
		return;

	//printf("\t\t\t%s\n", str);
	fprintf(latex, "%s $$ ", str);
	latex_node(node, latex, false);
	fprintf(latex, "$$\n");

	return;
}

static inline bool is_integer(const double number) {
	double delta = number - (int)number;
	if(delta < 0.001)
		return true;
	return false;
}