#include <stdlib.h>
#include "stack.h"
#include "warnings.h"
#include "work_with_tree.h"
#include "operation_codes_for_CPU.h"

#ifndef COMPILER_H
#define COMPILER_H

const char NAME_ASM_FILE[] = "asm_result.txt";

const size_t SIZE_MEMORY 		= 128;
const size_t BEGIN_VARS_ADDRESS = 64;
const size_t SIZE_FUNC_BUFFER   = 4096;

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
	FILE* buffer_with_functions;
	size_t count_of_functions;

  private:

  	enum OPER_INTO_COND {
  		INTO_FALSE_OPTION = 0,
  		INTO_TRUE_OPTION  = 1,
  		NOT_INTO_COND     = 2,
  	};

  	void compilation_error(struct call_of_dump info, const char message[] = "syntax error") {
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

  		printf("compiling... node %p, left %p, right %p. Type %d, value %d\n", node, node->left, node->right, node->type, (int)node->value);
  		printf("\t\t\tleft type %d\n", (int)node->left->type);

  		switch((int)node->left->type) {
  			case OPERATOR:
  				compiling_operator(node->left);
  				break;
  			case COND_TYPE:
  				printf("cond\n");  			
  				compiling_condition(node->left);
  				break;
  			//case PRINT:
  			//	compiling_print(node->left);
  			//	break;
  		};

  		fprintf(asm_code, "\n");
  		compiling(node->right);
  	}

  	void compiling_operator(Node* node) {
  		printf("begin compile oper\n");
  		switch((int)node->value) {
  			case EQUAL:
  				printf("equal\n");
  				compiling_assignment(node, asm_code);
  				break;

  		};
  		printf("end compile oper\n");
  	}

  	void compiling_print(Node* node) {
  		/*switch((int)node->type) {
  			case NUMBER:

  		}*/
  	}

  	void compiling_condition(Node* node) {
  		enum TYPE_CONDS {
  			NOTHING_TYPE 	   = 0,
  			LEFT_VAR_RIGHT_VAR = 1,
  			LEFT_NUM_RIGHT_VAR = 2,
  			LEFT_VAR_RIGHT_NUM = 3,
  			LEFT_NUM_RIGHT_NUM = 4,
  		};

  		TYPE_CONDS now_type = NOTHING_TYPE;
  		size_t left_var_addrss = 0, right_var_addrss = 0;

  		if((int)node->left->left->type == VARIABLE && (int)node->left->right->type == VARIABLE) {	// x < y
  			now_type = LEFT_VAR_RIGHT_VAR; 
  			left_var_addrss  = find_variable_address(node->left->left->value);
  			right_var_addrss = find_variable_address(node->left->right->value);
  			fprintf(asm_code, "cmp %d %d\n", memory[left_var_addrss], memory[right_var_addrss]);
  		} 
  		else
  		if((int)node->left->left->type == NUMBER && (int)node->left->right->type == VARIABLE) {		// 5 < x
  		  	now_type = LEFT_NUM_RIGHT_VAR;
  		  	right_var_addrss = find_variable_address(node->left->right->value);
  		  	fprintf(asm_code, "cmp %d %d\n", (int)node->left->left->value, memory[right_var_addrss]);
  		}
  		else
  		if((int)node->left->left->type == VARIABLE && (int)node->left->right->type == NUMBER) {		// x < 5
  			now_type = LEFT_VAR_RIGHT_NUM;
  			left_var_addrss = find_variable_address(node->left->left->value);
  			fprintf(asm_code, "cmp %d %d\n", memory[left_var_addrss], (int)node->left->right->value);
  		}
  		else
  		if((int)node->left->left->type == NUMBER && (int)node->left->right->type == NUMBER)	{		// 3 < 5
  			now_type = LEFT_NUM_RIGHT_NUM;
  			fprintf(asm_code, "cmp %d %d\n", (int)node->left->left->value, (int)node->left->right->value);
  		}
  		
  		size_t else_label = count_of_functions;
  		fprintf(asm_code, "%s L%lu\n\n", CPU_TEXT_OPERATION[(int)get_index_of_CPU_operation((COMPARISON_SIGNS)node->left->value)], 
  									   else_label);
  		++count_of_functions;

  		compiling_assignment(node->right->left->left, asm_code, INTO_FALSE_OPTION);
  		fprintf(asm_code, "jmp L%lu\n"
  						  "\nL%lu:\n", count_of_functions, else_label);
  		++count_of_functions;
  		compiling_assignment(node->right->right->left, asm_code, INTO_TRUE_OPTION);

  		fprintf(asm_code, "\nL%lu:\n", count_of_functions - 1);

  		printf("count_of_functions %lu\n", count_of_functions);
  	}

  	void compiling_assignment(Node* node, FILE* need_buffer, const OPER_INTO_COND remember_or_not = NOT_INTO_COND) {
  		size_t var_addrss = find_variable_address(node->left->value);
  		printf("\tvar address %lu, var %c\n", var_addrss, (char)node->left->value);
  		fprintf(need_buffer, "push rdx\n"
  						     "push %ld\n"
  						     "pop rdx\n", var_addrss);
  		

  		if(node->right->type == NUMBER) {
  			printf("\tleft is number, %d\n", (int)node->right->value);
  			fprintf(need_buffer, "push %d\n"
  							     "pop [rdx]\n", (int)node->right->value);

  			if(remember_or_not != INTO_FALSE_OPTION)
  				memory[var_addrss] = (int)node->right->value;
  		}  			
  	  	else
  	  	if(node->right->type == VARIABLE) {
  	  		size_t right_var_addrss = find_variable_address(node->right->value);
  			fprintf(need_buffer, "push %d\n"
  			                     "pop [rdx]\n", memory[right_var_addrss]);

  			if(remember_or_not != INTO_FALSE_OPTION)
  				memory[var_addrss] = memory[right_var_addrss];
  	  	}
  	  	else
  			compilation_error(INFORMATION_ABOUT_CALL, "assignment error!");

  		fprintf(need_buffer, "pop rdx\n");
  	}

  	CPU_OPERATIONS_CODES get_index_of_CPU_operation(const COMPARISON_SIGNS operation) {
  		printf("!!!!!\n!!!! %d operation !!!\n", operation);
  		switch(operation) {
  			case SIGN_EQUAL:
  				return OPERATION_CODE_JE;
  			case SIGN_NOT_EQUAL:
  				return OPERATION_CODE_JNE;
  			case SIGN_LESS:
  				return OPERATION_CODE_JB;
  			case SIGN_LESS_OR_EQUAL:
  				return OPERATION_CODE_JBE;
			case SIGN_MORE:
				return OPERATION_CODE_JA;
			case SIGN_MORE_OR_EQUAL:
				return OPERATION_CODE_JAE;
			default:
				compilation_error(INFORMATION_ABOUT_CALL, "Bad comparison operation");
				return OPERATION_CODE_MEOW;
  		}
  	}

  	CPU_OPERATIONS_CODES get_index_of_opposite_CPU_operation(const CPU_OPERATIONS_CODES operation) {
   		switch(operation) {
  			case OPERATION_CODE_JE:
  				return OPERATION_CODE_JNE;
  			case OPERATION_CODE_JNE:
  				return OPERATION_CODE_JE;
  			case OPERATION_CODE_JBE:
  				return OPERATION_CODE_JA;
  			case OPERATION_CODE_JA:
  				return OPERATION_CODE_JBE;
			case OPERATION_CODE_JAE:
				return OPERATION_CODE_JB;
			case OPERATION_CODE_JB:
				return OPERATION_CODE_JAE;
			default:
				compilation_error(INFORMATION_ABOUT_CALL, "Bad comparison operation");
				return OPERATION_CODE_MEOW;
  		} 		
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

  		/*buffer_with_functions = (char*)calloc(SIZE_FUNC_BUFFER, sizeof(char));
  		if(!buffer_with_functions) {
  			warning("bad buffer for functions", INFORMATION_ABOUT_CALL);
  			return;
  		}*/

  		buffer_with_functions = fopen("func_file.txt", "w");
  		if(!buffer_with_functions) {
  			warning("bad func file", INFORMATION_ABOUT_CALL);
  			return;
  		}

  	}

  	void initialize(Tree* tree) {
  		tree_make_copy(tree, prog_tree);

  		for(int i = 0; i < 4; ++i)
  			registers[i] = 0;

  		count_of_functions = 0;
  	}

  	void destruct() {
  		stack_destruct(&stack_frame);
  		fclose(asm_code);
  		tree_destruct(prog_tree);
  		free(memory);
  		fclose(buffer_with_functions);
  	}

  	void compile() {
  		fprintf(asm_code, "jmp main\n\n"
  						  "main:\n");

  		compiling(prog_tree->root);

  		fprintf(asm_code, "hlt\n");
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