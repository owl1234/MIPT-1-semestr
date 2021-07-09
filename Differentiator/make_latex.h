#include "work_with_tree.h"
#include "operation_codes.h"

void latex_tree(const Tree* tree, FILE* latex);
bool latex_node(const Node* node, FILE* latex, bool is_eval_into_brackets);
void test_latex(const Node* node, FILE* latex, const char* str);
static inline bool is_integer(const double number);

#ifndef MAKE_LATEX_H
#define MAKE_LATEX_H

const char SIMPLIFIED_PHRASES[][200] = {
	"Let's simplify the expression: ",
	"The following expression is very easy to simplify to the form: ",
	"Hmmm .. Let's try to simplify our expression a little by doing simple things: ",
	"Obviously, the original expression is simplified to this form",
	"It is easy to show that the formula above, after simplification, turns into: "
};

const int COUNT_SIMPLIFIED_PHRASES = 4;

#endif