/*
	Author:
						Kolesnikova Xenia <heiduk.k.k.s@yandex.ru>

    Last edition:
        				July 10, 2021, 16:24:10
    
    What was changed?
						1. Add assignment, variables declaration.
						2. Now the program can consist of several lines        

	To-do list
						1. Make conditions (if-else)
						2. Make loops (for/while)
						3. Make functions

*/

#include <stdio.h>
#include "parsing.h"

#define CHECK_STATUS(status)								\
	if(!(status == TREE_OKEY || status == NODE_OKEY))		\
		printf("awful!!!\n");

int main() {
	Tree tree = {};
	ERRORS_TYPE status = tree_construct(&tree);
	CHECK_STATUS(status)

	tree_fill(&tree, "file.txt");

	draw_tree(&tree);

    tree_destruct(&tree);
	
	return TREE_OKEY;
}