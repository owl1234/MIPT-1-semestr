#include <stdio.h>
#include "compiler.h"

void compile(Tree* tree) {
	printf("begin compile\n");

	Compiler compiler = {};
	compiler.construct();
	compiler.initialize(tree);

	compiler.fills_name_table();
	compiler.compile();

	compiler.destruct();

	printf("end compile\n");
	return;
}