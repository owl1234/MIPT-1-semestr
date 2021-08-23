#include <stdio.h>
#include "compiler.h"

void compile(Tree* tree) {
	printf("begin compile\n");

	Compiler compiler = {};
	compiler.construct();
	compiler.initialize(tree);

	compiler.fills_name_table();
	//compiler.evaluate_expression(tree->root);
	draw_tree(tree);
	compiler.compile();
	draw_tree(compiler.prog_tree);

	compiler.destruct();

	printf("end compile\n\n");
	return;
}