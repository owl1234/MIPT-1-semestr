#include "work_with_tree.h"
#include "simplify_expression.h"
#include <math.h>
#include <stdio.h>

void simplify_expression(Tree* tree, FILE* latex) {
	if(!tree || !latex)
		return;

	bool is_changed = true;
	while(is_changed) {
		is_changed = false;
		tree->root = shrinking(tree->root, latex, is_changed);
		fprintf(latex, "\\centering\nLet's simplify the expression:\n");
		latex_tree(tree, latex);
		draw_tree(tree);
	}
}

Node* shrinking(Node* node, FILE* latex, bool& is_changed) {
	if(!node)
		return NULL;

	if(node->type != OPERATOR)
		return node;

	//printf("begin now: %d\n", node->value);

	if(node->left && node->right) {
		if(node->left->type == NUMBER && node->right->type == NUMBER)
			node = two_sons_numbers(node, latex, is_changed);
		
		else
		if(node->left->type == NUMBER && node->right->type == VARIABLE)
			node = left_num_right_var(node, latex, is_changed);

		else
		if(node->left->type == VARIABLE && node->right->type == NUMBER) {
			//printf("\n\nhhihih\n");
			node = left_var_right_num(node, latex, is_changed);
		}

		else
		if(node->left->type == OPERATOR && node->right->type == NUMBER) {
			node = left_oper_right_num(node, latex, is_changed);
		}

		node = simplify_mul(node, latex, is_changed);
		
	}

	if(node->left)
		node->left = shrinking(node->left, latex, is_changed);
	
	if(node->right)
		node->right = shrinking(node->right, latex, is_changed);

	//printf("end now: %d\n", node->value);

	return node;
}

static Node* two_sons_numbers(Node* node, FILE* latex, bool& is_changed) {
	if(!node || !node->left || !node->right || node->left->type != NUMBER || node->right->type != NUMBER)
		return node;

	switch(node->value) {
		case ADD:
			node->value = node->left->value + node->right->value;
			node->type  = NUMBER;
			free(node->left);  node->left  = NULL;
			free(node->right); node->right = NULL;
			printf("add!\n");
			is_changed = true;

		case SUB:
			if(node->type == OPERATOR) {
				node->value = node->left->value - node->right->value;
				node->type  = NUMBER;
				free(node->left);  node->left  = NULL;
				free(node->right); node->right = NULL;
				printf("sub!\n");
				is_changed = true;
			}

		case MUL:
			if(node->type == OPERATOR) {
				node->value = node->left->value * node->right->value;
				node->type  = NUMBER;
				free(node->left);  node->left  = NULL;
				free(node->right); node->right = NULL;
				printf("mul!\n");
				is_changed = true;
			}

		case DIV:
			if(node->type == OPERATOR) {
				node->value = node->left->value / node->right->value;
				printf("div! %d\n", node->value);

				node->type  = NUMBER;
				free(node->left);  node->left  = NULL;
				free(node->right); node->right = NULL;
				is_changed = true;
			}

		case POW:
			if(node->type == OPERATOR) {
				node->value = pow(node->left->value, node->right->value);
				node->type  = NUMBER;
				free(node->left);  node->left  = NULL;
				free(node->right); node->right = NULL;
				printf("pow! %d\n", node->value);
				is_changed = true;							
			}
	}

	return node;
}

static Node* left_num_right_var(Node* node, FILE* latex, bool& is_changed) {
	if(!node || !node->left || !node->right || node->left->type != NUMBER || node->right->type != VARIABLE)
		return node;

	if(node->left->value == 0 && node->type == OPERATOR && node->value == ADD) {
		node->value = node->right->value;
		node->type = node->right->type;
		free(node->left);  node->left  = NULL;
		free(node->right); node->right = NULL;
		printf("left num right var .... add! %d\n", node->value);
		is_changed = true;				
	} 

	else
	if(node->left->value == 0 && node->type == OPERATOR && node->value == SUB) {
		node->value = -node->right->value;
		node->type = node->right->type;
		free(node->left);  node->left  = NULL;
		free(node->right); node->right = NULL;
		printf("left num right var .... sub! %d\n", node->value);
		is_changed = true;				
	}

	else
	if(node->left->value == 0 && node->type == OPERATOR && node->value == MUL) {
		node->value = 0;
		node->type = node->right->type;
		free(node->left);  node->left  = NULL;
		free(node->right); node->right = NULL;
		printf("left num right var .... mul+! %d\n", node->value);
		is_changed = true;				
	} 	

	else
	if(node->left->value == 1 && node->type == OPERATOR && node->value == MUL) {
		node->value = node->right->value;
		node->type = node->right->type;
		free(node->left);  node->left  = NULL;
		free(node->right); node->right = NULL;
		printf("left num right var .... mul+! %d\n", node->value);
		is_changed = true;				
	} 

	else
	if(node->left->value == -1 && node->type == OPERATOR && node->value == MUL) {
		node->value = -node->right->value;
		node->type = node->right->type;
		free(node->left);  node->left  = NULL;
		free(node->right); node->right = NULL;
		printf("left num right var .... mul-! %d\n", node->value);
		is_changed = true;				
	} 

	else
	if(node->left->value == 0 && node->type == OPERATOR && node->value == DIV) {
		node->value = 0;
		node->type = node->right->type;
		free(node->left);  node->left  = NULL;
		free(node->right); node->right = NULL;
		printf("left num right var .... div! %d\n", node->value);
		is_changed = true;				
	}		

	return node;
}

static Node* left_var_right_num(Node* node, FILE* latex, bool& is_changed) {
	if(!node || !node->left || !node->right || node->left->type != VARIABLE || node->right->type != NUMBER)
		return node;

	if(node->right->value == 0 && node->type == OPERATOR && node->value == ADD) {
		node->value = node->left->value;
		node->type = node->left->type;
		free(node->left);  node->left  = NULL;
		free(node->right); node->right = NULL;
		printf("left var right num .... 0 add! %d\n", node->value);
		is_changed = true;				
	} 

	else
	if(node->right->value == 0 && node->type == OPERATOR && node->value == SUB) {
		node->value = -node->left->value;
		node->type = node->left->type;
		free(node->left);  node->left  = NULL;
		free(node->right); node->right = NULL;
		printf("left var right num .... 0 sub! %d\n", node->value);
		is_changed = true;				
	}

	else
	if(node->right->value == 0 && node->type == OPERATOR && node->value == MUL) {
		node->value = 0;
		node->type = node->right->type;
		free(node->left);  node->left  = NULL;
		free(node->right); node->right = NULL;
		printf("left var right num .... 0 mul! %d\n", node->value);
		is_changed = true;			
	} 

	else
	if(node->right->value == 1 && node->type == OPERATOR && node->value == MUL) {
		node->value = node->left->value;
		node->type = node->left->type;
		free(node->left);  node->left  = NULL;
		free(node->right); node->right = NULL;
		printf("left var right num .... 1 mul! %d\n", node->value);
		is_changed = true;				
	} 

	else
	if(node->right->value == -1 && node->type == OPERATOR && node->value == MUL) {
		node->value = -node->left->value;
		node->type = node->left->type;
		free(node->left);  node->left  = NULL;
		free(node->right); node->right = NULL;
		printf("left var right num .... -1 mul! %d\n", node->value);
		is_changed = true;				
	} 

	else
	if(node->right->value == 0 && node->type == OPERATOR && node->value == DIV) {
		node->value = 0;
		node->type = node->left->type;
		free(node->left);  node->left  = NULL;
		free(node->right); node->right = NULL;
		printf("left var right num .... 0 div! %d\n", node->value);
		is_changed = true;				
	}		

	return node;
}

static Node* left_oper_right_num(Node* node, FILE* latex, bool& is_changed) {
	if(!node || !node->left || !node->right || node->left->type != OPERATOR || node->right->type != NUMBER)
		return node;

	if(node->right->value == 0 && node->value == MUL) {
		node->value = 0;
		node->type = NUMBER;
		free(node->left);  node->left  = NULL;
		free(node->right); node->right = NULL;
		printf("left oper right num .... 0! %d\n", node->value);
		is_changed = true;				
	}

	else
	if(node->right->value == 1 && node->value == MUL) {
		Node* new_node = NULL;
		node_make_copy(node->left, new_node);
		printf("left oper right num .... 1! %d\n", node->value);
		is_changed = true;	
		return new_node;	
	}

	else
	if(node->right->value == 0 && node->value == ADD) {
		return node->left;	
	}

	return node;
}

static Node* simplify_mul(Node* node, FILE* latex, bool& is_changed) {
	if(!node || !node->left || !node->right || !(node->type == OPERATOR && node->value == MUL))
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