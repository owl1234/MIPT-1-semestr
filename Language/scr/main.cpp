/*
	Author:
						Kolesnikova Xenia <heiduk.k.k.s@yandex.ru>

    Last edition:
        				August 23, 2021, 21:47:39
    
    What was changed?
    					1. Make expression evaluations    					

	To-do list
						1. Make functions

*/

#include <stdio.h>
#include "parsing.h"
#include "compiler.h"

#define CHECK_STATUS(status)								\
	if(!(status == TREE_OKEY || status == NODE_OKEY))		\
		printf("An error occured\n");

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

	//draw_tree(&tree);

    tree_destruct(&tree);
	
	return TREE_OKEY;
}