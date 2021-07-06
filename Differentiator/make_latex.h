#include "work_with_tree.h"
#include "operation_codes.h"

void latex_tree(const Tree* tree, FILE* latex);
void latex_node(const Node* node, FILE* latex);
void test_latex(const Node* node, FILE* latex, const char* str);
