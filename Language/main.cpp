/*
	Author:
						Kolesnikova Xenia <heiduk.k.k.s@yandex.ru>

    Last edition:
        				August 12, 2021, 13:30:17
    
    What was changed?
    					0. Happy birthday to me!))
						1. Fixed memory leaks on stack resizing  
						2. Attempts to generate assembly code begin. 
						   It can be seen in the assignment examples.   

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