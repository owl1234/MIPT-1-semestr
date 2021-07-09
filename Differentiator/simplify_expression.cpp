#include "work_with_tree.h"
#include "simplify_expression.h"
#include <math.h>
#include <stdio.h>

#define IF_DEBUG_SIMPLIFY(code) //code

#define CHECK_LATEX_FILE(node)							\
	if(!latex)											\
		return node;

#define CHECK_NODE(node)								\
	if(!node)											\
		return node;

#define CHECK_TREE(tree)								\
	if(!tree)											\
		return;

#define CHECK_LEFT_SON(node)							\
	if(!node->left)										\
		return node;

#define CHECK_CHILDREN(node)							\
	if(!node->left || !node->right)						\
		return node;

#define DELETE_CHILDREN(node)							\
	free(node->left);  node->left  = NULL;				\
	free(node->right); node->right = NULL;


void simplify_expression(Tree* tree, FILE* latex) {
	CHECK_TREE(tree)
	if(!latex)
		return;

	bool is_changed = false;
	tree->root = shrinking(tree->root, latex, is_changed);
	if(!is_changed)
		return;
	
	fprintf(latex, "\\centering\n%s\n", SIMPLIFIED_PHRASES[rand() % COUNT_SIMPLIFIED_PHRASES]);
	latex_tree(tree, latex);
	draw_tree(tree);

	while(is_changed) {
		is_changed = false;
		tree->root = shrinking(tree->root, latex, is_changed);
		fprintf(latex, "\\centering\n%s\n", SIMPLIFIED_PHRASES[rand() % COUNT_SIMPLIFIED_PHRASES]);
		latex_tree(tree, latex);
		draw_tree(tree);
	}
}

Node* shrinking(Node* node, FILE* latex, bool& is_changed) {
	CHECK_NODE(node);

	if(node->type != OPERATOR)
		return node;

	//printf("begin now: %lg, type %d\n", node->value, node->type);

	if(node->left && node->right) {
		//printf("\t left %d, right %d\n", node->left->type, node->right->type);
		if(node->left->type == NUMBER && node->right->type == NUMBER)
			node = two_sons_numbers(node, latex, is_changed);
		
		else
		if(node->left->type == NUMBER && node->right->type == VARIABLE)
			node = left_num_right_var(node, latex, is_changed);

		else
		if(node->left->type == VARIABLE && node->right->type == NUMBER) 
			node = left_var_right_num(node, latex, is_changed);

		else
		if(node->left->type == OPERATOR && node->right->type == NUMBER) 
			node = left_oper_right_num(node, latex, is_changed);
		
		else
		if(node->left->type == NUMBER && node->right->type == OPERATOR)
			node = left_num_right_oper(node, latex, is_changed);

		node = simplify_mul(node, latex, is_changed);
	} else if(node->left) {
		if(node->type == OPERATOR && node->value == LN)
			node = simplify_ln_e(node, latex, is_changed);
	}

	if(node->left)
		node->left = shrinking(node->left, latex, is_changed);
	
	if(node->right)
		node->right = shrinking(node->right, latex, is_changed);

	return node;
}

static Node* two_sons_numbers(Node* node, FILE* latex, bool& is_changed) {
	CHECK_NODE		(node);
	CHECK_CHILDREN	(node);
	CHECK_LATEX_FILE(node);

	if(node->left->type != NUMBER || node->right->type != NUMBER)
		return node;

	switch((int)node->value) {
		case ADD:
			node->value = node->left->value + node->right->value;
			node->type  = NUMBER;
			DELETE_CHILDREN(node)
			IF_DEBUG_SIMPLIFY(printf("add!\n");)
			is_changed = true;

		case SUB:
			if(node->type == OPERATOR) {
				node->value = node->left->value - node->right->value;
				node->type  = NUMBER;
				DELETE_CHILDREN(node)
				IF_DEBUG_SIMPLIFY(printf("sub!\n");)
				is_changed = true;
			}

		case MUL:
			if(node->type == OPERATOR) {
				node->value = node->left->value * node->right->value;
				node->type  = NUMBER;
				DELETE_CHILDREN(node)DELETE_CHILDREN(node)
				IF_DEBUG_SIMPLIFY(printf("mul!\n");)
				is_changed = true;
			}

		case DIV:
			if(node->type == OPERATOR) {
				node->value = node->left->value / node->right->value;
				IF_DEBUG_SIMPLIFY(printf("div! %lg\n", node->value);)

				node->type  = NUMBER;
				DELETE_CHILDREN(node)DELETE_CHILDREN(node)
				is_changed = true;
			}

		case POW:
			if(node->type == OPERATOR) {
				node->value = pow(node->left->value, node->right->value);
				node->type  = NUMBER;
				DELETE_CHILDREN(node)DELETE_CHILDREN(node)
				IF_DEBUG_SIMPLIFY(printf("pow! %lg\n", node->value);)
				is_changed = true;							
			}
	}

	return node;
}

static Node* left_num_right_var(Node* node, FILE* latex, bool& is_changed) {
	CHECK_NODE		(node);
	CHECK_CHILDREN	(node);
	CHECK_LATEX_FILE(node);

	if(node->left->type != NUMBER || node->right->type != VARIABLE)
		return node;

	if(node->left->value == 0. && node->type == OPERATOR && node->value == ADD) {
		node->value = node->right->value;
		node->type = node->right->type;
		DELETE_CHILDREN(node)
		IF_DEBUG_SIMPLIFY(printf("left num right var .... add! %lg\n", node->value);)
		is_changed = true;				
	} 

	else
	if(node->left->value == 0. && node->type == OPERATOR && node->value == SUB) {
		node->value = -node->right->value;
		node->type = node->right->type;
		DELETE_CHILDREN(node)
		IF_DEBUG_SIMPLIFY(printf("left num right var .... sub! %lg\n", node->value);)
		is_changed = true;				
	}

	else
	if(node->left->value == 0. && node->type == OPERATOR && node->value == MUL) {
		node->value = 0.;
		node->type = node->right->type;
		DELETE_CHILDREN(node)
		IF_DEBUG_SIMPLIFY(printf("left num right var .... mul+! %lg\n", node->value);)
		is_changed = true;				
	} 	

	else
	if(node->left->value == 1. && node->type == OPERATOR && node->value == MUL) {
		node->value = node->right->value;
		node->type = node->right->type;
		DELETE_CHILDREN(node)
		IF_DEBUG_SIMPLIFY(printf("left num right var .... mul+! %lg\n", node->value);)
		is_changed = true;				
	} 

	else
	if(node->left->value == -1. && node->type == OPERATOR && node->value == MUL) {
		node->value = -node->right->value;
		node->type = node->right->type;
		DELETE_CHILDREN(node)
		IF_DEBUG_SIMPLIFY(printf("left num right var .... mul-! %lg\n", node->value);)
		is_changed = true;				
	} 

	else
	if(node->left->value == 0. && node->type == OPERATOR && node->value == DIV) {
		node->value = 0.;
		node->type = node->right->type;
		DELETE_CHILDREN(node)
		IF_DEBUG_SIMPLIFY(printf("left num right var .... div! %lg\n", node->value);)
		is_changed = true;				
	}		

	return node;
}

static Node* left_var_right_num(Node* node, FILE* latex, bool& is_changed) {
	CHECK_NODE		(node);
	CHECK_CHILDREN	(node);
	CHECK_LATEX_FILE(node);

	if(node->left->type != VARIABLE || node->right->type != NUMBER)
		return node;

	if(node->right->value == 0. && node->type == OPERATOR && node->value == ADD) {
		node->value = node->left->value;
		node->type = node->left->type;
		DELETE_CHILDREN(node)
		IF_DEBUG_SIMPLIFY(printf("left var right num .... 0 add! %lg\n", node->value);)
		is_changed = true;				
	} 

	else
	if(node->right->value == 0. && node->type == OPERATOR && node->value == SUB) {
		node->value = -node->left->value;
		node->type = node->left->type;
		DELETE_CHILDREN(node)
		IF_DEBUG_SIMPLIFY(printf("left var right num .... 0 sub! %lg\n", node->value);)
		is_changed = true;				
	}

	else
	if(node->right->value == 0. && node->type == OPERATOR && node->value == MUL) {
		node->value = 0.;
		node->type = node->right->type;
		DELETE_CHILDREN(node)
		IF_DEBUG_SIMPLIFY(printf("left var right num .... 0 mul! %lg\n", node->value);)
		is_changed = true;			
	} 

	else
	if(node->right->value == 1. && node->type == OPERATOR && node->value == MUL) {
		node->value = node->left->value;
		node->type = node->left->type;
		DELETE_CHILDREN(node)
		IF_DEBUG_SIMPLIFY(printf("left var right num .... 1 mul! %lg\n", node->value);)
		is_changed = true;				
	} 

	else
	if(node->right->value == -1. && node->type == OPERATOR && node->value == MUL) {
		node->value = -node->left->value;
		node->type = node->left->type;
		DELETE_CHILDREN(node)
		IF_DEBUG_SIMPLIFY(printf("left var right num .... -1 mul! %lg\n", node->value);)
		is_changed = true;				
	} 

	else
	if(node->right->value == 0. && node->type == OPERATOR && node->value == DIV) {
		node->value = 0.;
		node->type = node->left->type;
		DELETE_CHILDREN(node)
		IF_DEBUG_SIMPLIFY(printf("left var right num .... 0 div! %lg\n", node->value);)
		is_changed = true;				
	}		

	return node;
}

static Node* left_oper_right_num(Node* node, FILE* latex, bool& is_changed) {
	CHECK_NODE		(node);
	CHECK_CHILDREN	(node);
	CHECK_LATEX_FILE(node);

	if(node->left->type != OPERATOR || node->right->type != NUMBER)
		return node;

	if(node->right->value == 0. && node->value == MUL) {
		node->value = 0.;
		node->type = NUMBER;
		DELETE_CHILDREN(node)
		IF_DEBUG_SIMPLIFY(printf("left oper right num .... 0! %lg\n", node->value);)
		is_changed = true;				
	}

	else
	if(node->right->value == 1. && node->value == MUL) {
		Node* new_node = NULL;
		node_make_copy(node->left, new_node);
		IF_DEBUG_SIMPLIFY(printf("left oper right num .... 1! %lg\n", node->value);)
		is_changed = true;	
		return new_node;	
	}

	else
	if(node->right->value == 0. && node->value == ADD) {
		is_changed = true;				
		return node->left;	
	}

	return node;
}

static Node* left_num_right_oper(Node* node, FILE* latex, bool& is_changed) {
	CHECK_NODE		(node);
	CHECK_CHILDREN	(node);
	CHECK_LATEX_FILE(node);

	if(node->left->type != NUMBER || node->right->type != OPERATOR)
		return node;

	if(node->left->value == 0. && node->value == MUL) {
		node->value = 0.;
		node->type = NUMBER;
		DELETE_CHILDREN(node)
		IF_DEBUG_SIMPLIFY(printf("left oper right num .... 0! %lg\n", node->value);)
		is_changed = true;				
	}

	else
	if(node->left->value == 1. && node->value == MUL) {
		Node* new_node = NULL;
		node_make_copy(node->right, new_node);
		IF_DEBUG_SIMPLIFY(printf("left oper right num .... 1! %lg\n", node->value);)
		is_changed = true;	
		return new_node;	
	}

	else
	if(node->left->value == 0. && node->value == ADD) {
		is_changed = true;
		return node->right;	
	}

	return node;
}

static Node* simplify_mul(Node* node, FILE* latex, bool& is_changed) {
	CHECK_NODE		(node);
	CHECK_CHILDREN	(node);
	CHECK_LATEX_FILE(node);

	if(!(node->type == OPERATOR && node->value == MUL))
		return node;

	if(node->left->type == NUMBER && node->right->type == OPERATOR && node->right->value == MUL) {
		if(node->right->left->type == NUMBER) {
			node->right->left->value *= node->left->value;
			is_changed = true;
			return node->right;
		}

		else
		if(node->right->right->type == NUMBER) {
			node->right->right->value *= node->left->value;
			is_changed = true;
			return node->right;
		}
	}

	return node;
}

static Node* simplify_ln_e(Node* node, FILE* latex, bool& is_changed) {
	CHECK_NODE		(node);
	CHECK_LEFT_SON	(node);
	CHECK_LATEX_FILE(node);

	if(!(node->type == OPERATOR && node->value == LN))
		return node;

	if(node->left->type == VARIABLE && node->left->value == 'e') {
		Node* answer = node_construct(NUMBER, 1, NULL, NULL);
		is_changed = true;
		return answer;
	}

	return node;
}