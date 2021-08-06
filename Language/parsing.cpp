#include <stdio.h>
#include <stdlib.h>
#include "parsing.h"
#include "work_with_file.h"

void tree_fill(Tree* tree, const char* name_file) {
	if(!tree || !name_file)
		return;

	printf("begin fill the tree\n");

	File file = file_construct(name_file);
	FILE_STATUS status_file = read_buffer(&file);
	if(status_file != FILE_OK)
		return;

	Parser parser = {};
	parser.construct(tree->used_vars);

	printf("begin parse\n");

	tree->root = parser.get_G(file.buffer);

	for(int i = 0; i < ALPHABET; ++i)
		tree->used_vars[i] = parser.used_vars[i];

	printf("end parse\n");

	file_destruct(&file);
	
	printf("end fill the tree\n");
}

void get_variables(Tree* tree) {
	if(tree == NULL)
		return;

	get_variables(tree->root, tree->used_vars);
}

void get_variables(Node* node, bool* used_vars) {
	if(!node)
		return;

	get_variables(node->left, used_vars);

	if(node->type == VARIABLE) {
		used_vars[(int)node->value - 'a'] = true;
	}

	get_variables(node->right, used_vars);
}