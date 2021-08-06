#include <stdlib.h>
#include "stack.h"
#include "warnings.h"

#ifndef COMPILER_H
#define COMPILER_H

const char NAME_ASM_FILE[] = "asm_result.txt";

#endif

struct Compiler {
	FILE* asm_code;
	Stack_t stack_frame;


  public:
  	void construct() {
  		stack_construct(&stack_frame);
  		asm_code = fopen(NAME_ASM_FILE, "w");
  		if(!asm_code) {
  			warning("bad asm file", INFORMATION_ABOUT_CALL);
  			return;
  		}
  	}

  	void destruct() {
  		stack_destruct(&stack_frame);
  		fclose(asm_code);
  	}
};



void compile();