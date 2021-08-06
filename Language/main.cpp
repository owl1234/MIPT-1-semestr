/*
	Author:
						Kolesnikova Xenia <heiduk.k.k.s@yandex.ru>

    Last edition:
        				August 7, 2021, 00:52:29
    
    What was changed?
						1. Add if-else  
						2. Yes, I'm alive, even doing something     

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

	for(int i = 0; i < 26; ++i)
		printf("%d ", tree.used_vars[i]);
	printf("\n");

	compile();

    tree_destruct(&tree);
	
	return TREE_OKEY;
}