#include <stdio.h>
#include "make_latex.h"
#include "work_with_tree.h"


void simplify_expression(Tree* tree, FILE* latex);
Node* shrinking(Node* node, FILE* latex, bool& is_changed);
static Node* two_sons_numbers  (Node* node, FILE* latex, bool& is_changed);
static Node* left_num_right_var(Node* node, FILE* latex, bool& is_changed);
static Node* left_var_right_num(Node* node, FILE* latex, bool& is_changed);
static Node* left_oper_right_num(Node* node, FILE* latex, bool& is_changed);
static Node* simplify_mul		(Node* node, FILE* latex, bool& is_changed);