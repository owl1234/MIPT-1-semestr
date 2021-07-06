#include <stdio.h>
#include <string.h>
#include "work_with_file.h"
#include "work_with_tree.h"
#include "operation_codes.h"

int counter = 1;

ERRORS_TYPE tree_construct(Tree* tree) {
	if(!tree)
		return BAD_TREE_PTR;
	tree->root = NULL;

	for(int index = 0; index < ALPHABET; ++index) 
		tree->used_vars[index] = false;

	return TREE_OKEY;
}

ERRORS_TYPE tree_destruct(Tree* tree) {
	if(!tree)
		return TREE_OKEY;

	node_destruct(tree->root);
	free(tree->root);

	for(int index = 0; index < ALPHABET; ++index)
		tree->used_vars[index] = false;

	return TREE_OKEY;
}

Node* node_construct(const NODE_TYPE type, const Type_value value, Node* left_son, Node* right_son) {

	Node* new_node = (Node*)calloc(1, sizeof(Node));

	if(!new_node)
		return NULL;

	new_node->type  = type;
	new_node->value = value;

	new_node->left = left_son;
	new_node->right = right_son;

	new_node->number_node = counter++;

	return new_node;
}

ERRORS_TYPE node_destruct(Node* node) {
	if(!node)
		return NODE_OKEY;

	node_destruct(node->left);
	node_destruct(node->right);

	return NODE_OKEY;
}

int count_draw = 0;

ERRORS_TYPE draw_tree(Tree* tree) {
	if(!tree)
		return BAD_TREE_PTR;

	FILE* picture = {};
	char buffer_for_name_file[100]; // = (char*)calloc(100, sizeof(char));
	sprintf(buffer_for_name_file, "%s%d.dot", name_picture_with_tree, count_draw);

	//if(count_draw == 0)
		picture = fopen(buffer_for_name_file, "w");
	//else
	//	picture = fopen("second.dot", "w");

    fprintf(picture, "digraph graphname {\n");	

    draw_node(tree->root, picture);

    fprintf(picture, "}");
    fclose(picture);


	char buffer_for_system[100]; // = (char*)calloc(100, sizeof(char));
	sprintf(buffer_for_system, "dot %s -Tpng -O", buffer_for_name_file);

    /*if(count_draw == 0)
    	system("dot tree.dot -Tpng -O");   
   	else
   		system("dot second.dot -Tpng -O");*/

	system(buffer_for_system);

   	++count_draw;

    return TREE_OKEY;
}

void draw_node(Node* node, FILE* picture) {
	if(!node)
		return;

	draw_node(node->left, picture);

	draw_type_node(node, picture);

	if(node->left)
		fprintf(picture, "\tnode%d -> node%d\n", node->number_node, node->left->number_node);

	if(node->right)
		fprintf(picture, "\tnode%d -> node%d\n", node->number_node, node->right->number_node);

	draw_node(node->right, picture);
}

static void draw_type_node(Node* node, FILE* picture) {
	if(node->type == OPERATOR) {
		fprintf(picture, "\tnode%d [shape=\"record\", color=\"red\", label=\"%s\"]\n", node->number_node, TEXT_OPERATIONS[node->value]);
	} else if(node->type == VARIABLE) {
		fprintf(picture, "\tnode%d [shape=\"record\", color=\"red\", label=\"%c\"]\n", node->number_node, node->value);
	} else
		fprintf(picture, "\tnode%d [shape=\"record\", color=\"red\", label=\"%d\"]\n", node->number_node, node->value);
}



void draw_draw_node(Node* node) {
	if(!node)
		return;

	FILE* picture = {};
	char buffer_for_name_file[100];
	sprintf(buffer_for_name_file, "%s%d.dot", name_picture_with_tree, count_draw);

	picture = fopen(buffer_for_name_file, "w");

    fprintf(picture, "digraph graphname {\n");	

    draw_node(node, picture);

    fprintf(picture, "}");
    fclose(picture);

	char buffer_for_system[100];
	sprintf(buffer_for_system, "dot %s -Tpng -O", buffer_for_name_file);

	system(buffer_for_system);

   	++count_draw;

}


void tree_make_copy(Tree* from, Tree* to) {
	if(!from || !to)
		return;

	to->root = node_construct(from->root->type, from->root->value, NULL, NULL);
	node_make_copy(from->root, to->root);

	for(int i = 0; i < ALPHABET; ++i)
		to->used_vars[i] = from->used_vars[i];
}

void node_make_copy(Node* from, Node*& to) {
	if(!from)
		return;

	if(!to) {
		to = (Node*)calloc(1, sizeof(Node));
		to->value = from->value;
		to->type = from->type;
		to->number_node = from->number_node;
	}

	/*if(!from->left && !from->right) {
		to = node_construct(to->type, to->value, to->left, to->right);
	}*/

	if(from->left) {
		to->left = node_construct(from->left->type, from->left->value, NULL, NULL);
		node_make_copy(from->left, to->left);
	}

	if(from->right) {
		to->right = node_construct(from->right->type, from->right->value, NULL, NULL);
		node_make_copy(from->right, to->right);
	}
}