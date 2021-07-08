#include <stdio.h>
#include <math.h>
#include "work_with_tree.h"

const double LG_10 = log10(10);


void 					derivate_tree				(Tree* tree, FILE* latex);
Node* 					derivate_node				(Node* node, FILE* latex);

static Node* 			derivate_add_and_sub		(Node* node, FILE* latex);
static Node* 			derivate_mul		 		(Node* node, FILE* latex);
static Node*			derivate_div		 		(Node* node, FILE* latex);
static Node* 			derivate_pow				(Node* node, FILE* latex);

static Node* 			derivate_sin		 		(Node* node, FILE* latex);
static Node* 			derivate_cos		 		(Node* node, FILE* latex);
static Node* 			derivate_tg					(Node* node, FILE* latex);
static Node* 			derivate_ctg				(Node* node, FILE* latex);

static Node* 			derivate_ln					(Node* node, FILE* latex);
static Node* 			derivate_lg					(Node* node, FILE* latex);


static inline Node* 	do_derivating_pow			(Node* left, Node* right, Node* der_left, Node* der_right);
static inline Node* 	do_derivating_div			(Node* left, Node* right, Node* der_left, Node* der_right, FILE* latex);