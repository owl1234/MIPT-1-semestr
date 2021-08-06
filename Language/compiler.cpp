#include <stdio.h>
#include "compiler.h"

void compile() {
	printf("begin compile\n");

	Compiler compiler = {};
	compiler.construct();
	compiler.destruct();

	printf("end compile\n");
	return;
}