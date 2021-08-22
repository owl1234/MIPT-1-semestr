#include <stdio.h>
#include "operation_codes.h"

#define Type_value double

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

#ifndef TREE_CONSTS
#define TREE_CONSTS

enum NODE_TYPE {
	COND_TYPE 		= 0,
	NUMBER 	 		= 1,
	VARIABLE 		= 2,
	OPERATOR 		= 3,
	COMPARE_SIGN 	= 4,
	PRINT 			= 5,
	LOOP_TYPE       = 6,
	ASSIGN_TYPE 	= 7,
};

const char name_picture_with_tree[] = "tree";
const size_t MAX_SIZE_BUFFER_FOR_FILE = 128;

struct Node {
	NODE_TYPE type;
	Type_value value;
	Node* left;
	Node* right;
	int number_node;
};

struct Tree {
	Node* root;
	bool used_vars[ALPHABET];
};

enum ERRORS_TYPE {
	TREE_OKEY	  = 0,
	NODE_OKEY     = 0,
	BAD_TREE_PTR  = 1,
	BAD_ROOT_PTR  = 2,
	BAD_LEFT_PTR  = 3,
	BAD_RIGHT_PTR = 4,

};

#endif

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

ERRORS_TYPE tree_construct(Tree* tree);
ERRORS_TYPE tree_destruct(Tree* tree);
Node* node_construct(const NODE_TYPE type, const Type_value value, Node* left_son, Node* right_son);
ERRORS_TYPE node_destruct(Node* node);
ERRORS_TYPE draw_tree(Tree* tree);
void draw_node(Node* node, FILE* picture);
static void draw_type_node(Node* node, FILE* picture);
void tree_make_copy(Tree* from, Tree* to);
void node_make_copy(Node* from, Node*& to);



void draw_draw_node(Node* node);