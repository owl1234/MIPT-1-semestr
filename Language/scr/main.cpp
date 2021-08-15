/*
	Author:
						Kolesnikova Xenia <heiduk.k.k.s@yandex.ru>

    Last edition:
        				August 15, 2021, 23:06:08
    
    What was changed?
    					1. Added assembly and processing. Hooray!
						2. Fixed stack_back. Even in such a simple function, I made a mistake. Ha-ha!

	To-do list
						1. Make loops (for/while)
						2. Make functions

*/

#include <stdio.h>
#include "parsing.h"
#include "compiler.h"

#define CHECK_STATUS(status)								\
	if(!(status == TREE_OKEY || status == NODE_OKEY))		\
		printf("awful!!!\n");

int main() {
	Tree tree = {};
	ERRORS_TYPE status = tree_construct(&tree);
	CHECK_STATUS(status)

	tree_fill(&tree, "file.txt");

	draw_tree(&tree);

	printf("\t\t\tUSED VARS:\n");
	for(int i = 0; i < ALPHABET; ++i)
		printf("%d ", tree.used_vars[i]);
	printf("\n\n");

	compile(&tree);

    tree_destruct(&tree);
	
	return TREE_OKEY;
}