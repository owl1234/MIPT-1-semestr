#include <stdlib.h>
#include "stack.h"
#include "warnings.h"
#include "work_with_tree.h"

#ifndef COMPILER_H
#define COMPILER_H

const char NAME_ASM_FILE[] = "asm_result.txt";

const size_t SIZE_MEMORY 		= 128;
const size_t BEGIN_VARS_ADDRESS = 64;

#endif

struct Name_table {
	char name_var;
	double value_var;
	size_t address;
};

struct Compiler {
	FILE* asm_code;
	Stack_t stack_frame;
	Tree* prog_tree;
	int registers[4];
	Name_table* name_table;
	size_t size_name_table;
	int* memory;

  private:

  	void syntax_error(struct call_of_dump info, const char message[] = "syntax error") {
  		warning(message, info);
  	}

  	size_t find_variable_address(const char name_variable) {
  		for(size_t index = 0; index < size_name_table; ++index)
  			if(name_table[index].name_var == name_variable)
  				return name_table[index].address;
  		return 0;
  	}

  	void compiling(Node* node) {
  		if(!node || !node->left)
  			return;

  		//printf("compiling... node %p, left %p, right %p. Type %d, value %d\n", node, node->left, node->right, node->type, node->value);

  		switch((int)node->left->type) {
  			case OPERATOR:
  				compiling_operator(node->left);
  		};

  		fprintf(asm_code, "\n");
  		compiling(node->right);
  	}

  	void compiling_operator(Node* node) {
  		switch((int)node->value) {
  			case EQUAL:
  				compiling_assignment(node);
  		};
  	}

  	void compiling_assignment(Node* node) {
  		size_t var_addrss = find_variable_address(node->left->value);
  		fprintf(asm_code, "PUSH rdx\n"
  						  "PUSH %ld\n"
  						  "POP rdx\n", find_variable_address(node->left->value));
  		

  		if(node->right->type == NUMBER) {
  			fprintf(asm_code, "PUSH %d\n"
  							  "POP [rdx]\n", (int)node->right->value);
  			memory[var_addrss] = (int)node->right->value;
  		}  			
  	  	else
  	  	if(node->right->type == VARIABLE) {
  	  		size_t right_var_addrss = find_variable_address(node->right->value);
  			fprintf(asm_code, "PUSH %d\n"
  			                  "POP [rdx]\n", memory[right_var_addrss]);
  			memory[var_addrss] = memory[right_var_addrss];
  	  	}
  	  	else
  			syntax_error(INFORMATION_ABOUT_CALL, "assignment error!");

  		fprintf(asm_code, "POP rdx\n");
  	}

  public:
  	void construct() {
  		stack_construct(&stack_frame);
  		for(int i = 0; i < 8; ++i)			
  			stack_resize(&stack_frame); 	

  		asm_code = fopen(NAME_ASM_FILE, "w");
  		if(!asm_code) {
  			warning("bad asm file", INFORMATION_ABOUT_CALL);
  			return;
  		}

		prog_tree = (Tree*)calloc(1, sizeof(Tree));  		
    	if(!prog_tree) {
  			warning("bad prog tree", INFORMATION_ABOUT_CALL);
  			return;
  		}

  		tree_construct(prog_tree);

  		memory = (int*)calloc(SIZE_MEMORY, sizeof(int));
  	  	if(!memory) {
  			warning("bad memory", INFORMATION_ABOUT_CALL);
  			return;
  		}
  	}

  	void initialize(Tree* tree) {
  		tree_make_copy(tree, prog_tree);

  		for(int i = 0; i < 4; ++i)
  			registers[i] = 0;
  	}

  	void destruct() {
  		stack_destruct(&stack_frame);
  		fclose(asm_code);
  		tree_destruct(prog_tree);
  	}

  	void compile() {
  		compiling(prog_tree->root);
  	}

  	void fills_name_table() {
  		size_t counter_used_vars = 0;

  		for(size_t letter = 0; letter < ALPHABET; ++letter)
  			if(prog_tree->used_vars[letter])
  				++counter_used_vars;

  		name_table = (Name_table*)calloc(counter_used_vars, sizeof(Name_table));

  		size_t index_into_name_table = 0;

  		for(size_t letter = 0; letter < ALPHABET; ++letter)
  			if(prog_tree->used_vars[letter]) {
  				name_table[index_into_name_table].name_var  = letter + 'a';
  				name_table[index_into_name_table].value_var = 0.;
  				name_table[index_into_name_table].address   = BEGIN_VARS_ADDRESS + letter;
  				++index_into_name_table;
  			}

  		size_name_table = counter_used_vars;
  	}
};



void compile(Tree* tree);