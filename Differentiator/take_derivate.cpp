#include <stdio.h>
#include "parsing.h"
#include "work_with_tree.h"
#include "take_derivate.h"
#include "make_latex.h"

#define IF_DEBUG_DERIVATE(code) code

void derivate_tree(Tree* tree, FILE* latex) {
	if(!tree || !latex)
		return;

	printf("begin derivate\n");
	tree->root = derivate_node(tree->root, latex);
	printf("end derivate\n\n\n");
}

Node* derivate_node(Node* node, FILE* latex) {
	if(!node || !latex)
		return NULL;

	if(node->type == NUMBER) {
		node->value = 0;
	} else if(node->type == OPERATOR) {
		if(node->value == ADD || node->value == SUB) 
			node = derivate_add_and_sub(node, latex);

		else
		if(node->value == MUL) 
			node = derivate_mul(node, latex);
		
		else
		if(node->value == DIV)
			node = derivate_div(node, latex);

		else
		if(node->value == POW)
			node = derivate_pow(node, latex);

		else
		if(node->value == SIN) 
			node = derivate_sin(node, latex);
	
		else
		if(node->value == COS) 
			node = derivate_cos(node, latex);


	} else {
		node->value = 1;
		node->type = NUMBER;
	}

	return node;
}

static Node* derivate_add_and_sub(Node* node, FILE* latex) {
	if(!node || !latex)
		return node;

	node->left  = derivate_node(node->left, latex);
	node->right = derivate_node(node->right, latex);

	return node;
}

static Node* derivate_mul(Node* node, FILE* latex) {
	if(!node || !latex || !node->left || !node->right)
		return node;

	Node* old_left  = node_construct(node->left->type,  node->left->value,  NULL, NULL);
	Node* old_right = node_construct(node->right->type, node->right->value, NULL, NULL);		

	node_make_copy(node->left, old_left);			
	node_make_copy(node->right, old_right);

	Node* der_old_left  = node_construct(old_left->type,  old_left->value,  NULL, NULL);
	Node* der_old_right = node_construct(old_right->type,  old_right->value,  NULL, NULL);

	node_make_copy(old_left, der_old_left);
	node_make_copy(old_right, der_old_right);

	der_old_left  = derivate_node(der_old_left,  latex);
	der_old_right = derivate_node(der_old_right, latex);

	Node* new_left  = node_construct(OPERATOR, MUL, NULL,  NULL);
	Node* new_right = node_construct(OPERATOR, MUL, NULL, NULL);

	node_make_copy(old_left,	  new_left->left);
	node_make_copy(old_right,    new_right->left);

	node_make_copy(der_old_right, new_left->right);
	node_make_copy(der_old_left, new_right->right);		

	Node* result_derivating = node_construct(OPERATOR, ADD, NULL, NULL);

	node_make_copy(new_left,  result_derivating->left);
	node_make_copy(new_right, result_derivating->right);

	return result_derivating;
}

static Node* derivate_div(Node* node, FILE* latex) {
	if(!node || !latex)
		return node;

	Node* old_left  = node_construct(node->left->type,  node->left->value,  NULL, NULL);
	Node* old_right = node_construct(node->right->type, node->right->value, NULL, NULL);

	node_make_copy(node->left,  old_left);
	node_make_copy(node->right, old_right);

	Node* der_old_left  = node_construct(old_left->type,  old_left->value,  NULL, NULL);
	Node* der_old_right = node_construct(old_right->type, old_right->value, NULL, NULL);

	node_make_copy(old_left, der_old_left);
	node_make_copy(old_right, der_old_right);

	der_old_left  = derivate_node(der_old_left,  latex);
	der_old_right = derivate_node(der_old_right, latex);

	return do_derivating_div(old_left, old_right, der_old_left, der_old_right);

	return node;
}

static inline Node* do_derivating_div(Node* left, Node* right, Node* der_left, Node* der_right) {
	//---------------------------------------- make left -------------------------------------------

	Node* answer_left = node_construct(OPERATOR, SUB, NULL, NULL);

	Node* answer_left_left = node_construct(OPERATOR, MUL, NULL, NULL);
	node_make_copy(der_left, answer_left_left->left);
	node_make_copy(right,    answer_left_left->right);

	Node* answer_left_right = node_construct(OPERATOR, MUL, NULL, NULL);
	node_make_copy(left,      answer_left_right->left);
	node_make_copy(der_right, answer_left_right->right);

	node_make_copy(answer_left_left,  answer_left->left);
	node_make_copy(answer_left_right, answer_left->right);

	//---------------------------------------- make right -------------------------------------------

	Node* answer_right = node_construct(OPERATOR, MUL, NULL, NULL);

	node_make_copy(right, answer_right->left);
	node_make_copy(right, answer_right->right);

	//---------------------------------------- make answer -------------------------------------------

	Node* answer = node_construct(OPERATOR, DIV, NULL, NULL);
	node_make_copy(answer_left,  answer->left);
	node_make_copy(answer_right, answer->right);

	return answer;
}

static Node* derivate_pow(Node* node, FILE* latex) {
	if(!node || !latex || !node->left || !node->right)
		return node;

	Node* old_left  = node_construct(node->left->type,  node->left->value,  NULL, NULL);
	Node* old_right = node_construct(node->right->type, node->right->value, NULL, NULL);

	node_make_copy(node->left,  old_left);
	node_make_copy(node->right, old_right);

	Node* der_old_left  = node_construct(old_left->type,  old_left->value,  NULL, NULL);
	Node* der_old_right = node_construct(old_right->type, old_right->value, NULL, NULL);

	node_make_copy(old_left, der_old_left);
	node_make_copy(old_right, der_old_right);

	der_old_left  = derivate_node(der_old_left,  latex);
	der_old_right = derivate_node(der_old_right, latex);

	return do_derivating_pow(old_left, old_right, der_old_left, der_old_right);
}

static inline Node* do_derivating_pow(Node* left, Node* right, Node* der_left, Node* der_right) {
	//---------------------------------------- make left -------------------------------------------

	Node* answer_left = node_construct(OPERATOR, MUL, NULL, NULL);
	node_make_copy(der_left, answer_left->right);

	Node* answer_left_left = node_construct(OPERATOR, MUL, NULL, NULL);
	node_make_copy(right, answer_left_left->left);

	Node* answer_left_left_right = node_construct(OPERATOR, POW, NULL, NULL);
	node_make_copy(left, answer_left_left_right->left);

	Node* answer_left_left_right_right = node_construct(OPERATOR, SUB, NULL, NULL);
	node_make_copy(right, answer_left_left_right_right->left);
	answer_left_left_right_right->right = node_construct(NUMBER, 1, NULL, NULL);

	node_make_copy(answer_left_left_right_right, answer_left_left_right->right);

	node_make_copy(answer_left_left_right, answer_left_left->right);

	node_make_copy(answer_left_left, answer_left->left);

	//---------------------------------------- make right -------------------------------------------

	Node* answer_right = node_construct(OPERATOR, MUL, NULL, NULL);
	node_make_copy(der_right, answer_right->right);

	Node* answer_right_left = node_construct(OPERATOR, MUL, NULL, NULL);

	Node* answer_right_left_left = node_construct(OPERATOR, LG, NULL, NULL);
	node_make_copy(left, answer_right_left_left->left);

	node_make_copy(answer_right_left_left, answer_right_left->left);

	Node* answer_right_left_right = node_construct(OPERATOR, POW, NULL, NULL);
	node_make_copy(left,  answer_right_left_right->left);
	node_make_copy(right, answer_right_left_right->right);

	node_make_copy(answer_right_left_right, answer_right_left->right);

	node_make_copy(answer_right_left, answer_right->left);

	//---------------------------------------- make answer -------------------------------------------

	Node* answer = node_construct(OPERATOR, ADD, NULL, NULL);
	node_make_copy(answer_left,  answer->left);
	node_make_copy(answer_right, answer->right);

	return answer;
}


static Node* derivate_sin(Node* node, FILE* latex) {
	if(!node || !latex)
		return node;

	Node* answer = node_construct(OPERATOR, MUL, NULL, NULL);

	node_make_copy(node, answer->left);
	answer->left->value = COS;

	Node* der_arg = node_construct(node->left->type, node->left->value, NULL, NULL);
	node_make_copy(node->left, der_arg);

	der_arg = derivate_node(der_arg, latex);
	node_make_copy(der_arg, answer->right);

	return answer;
}

static Node* derivate_cos(Node* node, FILE* latex) {
	if(!node || !latex)
		return node;

	Node* answer_without_minus = node_construct(OPERATOR, MUL, NULL, NULL);

	node_make_copy(node, answer_without_minus->left);
	answer_without_minus->left->value = SIN;

	Node* der_arg = node_construct(node->left->type, node->left->value, NULL, NULL);
	node_make_copy(node->left, der_arg);

	der_arg = derivate_node(der_arg, latex);
	node_make_copy(der_arg, answer_without_minus->right);

	test_latex(answer_without_minus, latex, "answer_without_minus from cos: ");

	Node* answer = node_construct(OPERATOR, MUL, NULL, NULL);
	answer->left = node_construct(NUMBER, -1, NULL, NULL);
	node_make_copy(answer_without_minus, answer->right);

	return answer;
}

bool is_leaf(Node* node) {
	if(!node || !node->left && !node->right)
		return true;
	return false;
}