#include <stdlib.h>
#include <math.h>

//#include "stack.h"
#include "common.h"
#include "warnings.h"
#include "work_with_tree.h"
//#include "operation_codes_for_CPU.h"

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

struct Condition_parts {
	int left_value;
	int right_value;
};

struct Compiler {
	FILE* asm_code;
	Stack_t stack_frame;
	Tree* prog_tree;
	int registers[4];
	Name_table* name_table;
	size_t size_name_table;
	int* memory;
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

  	void compiling(Node* &node, const OPER_INTO_COND remember_or_not = NOT_INTO_COND) {
  		if(!node || !node->left)
  			return;

  		//printf("compiling... node %p, left %p, right %p. Type %d, value %d\n", node, node->left, node->right, node->type, (int)node->value);
  		//printf("\t\t\tleft type %d\n", (int)node->left->type);

  		switch((int)node->left->type) {
  			case OPERATOR:
  				compiling_operator(node->left, remember_or_not);
  				break;
  			case COND_TYPE:
  				compiling_condition(node->left, remember_or_not);
  				break;
  			case LOOP_TYPE:
  				compiling_loop(node->left, remember_or_not);
  				break;
  			case PRINT:
  				compiling_print(node->left, remember_or_not);
  				break;
  			case ASSIGN_TYPE:
  				compiling_operator(node->left, remember_or_not);
  				break;
  		};

  		fprintf(asm_code, "\n");
  		compiling(node->right);
  	}

  	void compiling_operator(Node* &node, const OPER_INTO_COND remember_or_not = NOT_INTO_COND) {
  		printf("begin compile oper\n");
  		if(node->type == OPERATOR) {
  			switch((int)node->value) {
	  			case EQUAL:
	  				printf("equal\n");
	  				compiling_assignment(node, asm_code, remember_or_not);
	  				break;

	  		};
	  	}

	  	else
	  	if(node->type == ASSIGN_TYPE)
	  		compiling_assignment(node, asm_code, remember_or_not);
  		printf("end compile oper\n");
  	}

  	void compiling_print(Node* node, const OPER_INTO_COND remember_or_not = NOT_INTO_COND) {
  		switch((int)node->left->type) {
  			case NUMBER:
  				fprintf(asm_code, "push %d\n", (int)node->left->value);
  				break;
  			case VARIABLE:
  				fprintf(asm_code, "push %d\n", memory[find_variable_address((char)node->left->value)]);
  				break;
  		}

  		fprintf(asm_code, "out\n");
  	}

  	void compiling_condition(Node* node, const OPER_INTO_COND remember_or_not = NOT_INTO_COND) {
  		Condition_parts values_in_cond = compiling_condition_expression(node);
  		
  		size_t else_label = count_of_functions;
  		fprintf(asm_code, "%s L%lu\n\n", CPU_TEXT_OPERATION[(int)get_index_of_CPU_operation((COMPARISON_SIGNS)node->left->value)], 
  									     else_label);
  		++count_of_functions;

  		bool is_condition_right = get_truth_of_cond(node->left, values_in_cond.left_value, values_in_cond.right_value);
  		compiling(node->right->right, (OPER_INTO_COND)!is_condition_right);

  		fprintf(asm_code, "jmp L%lu\n"
  						  "\nL%lu:\n", count_of_functions, else_label);
  		++count_of_functions;
  		compiling(node->right->left, (OPER_INTO_COND)is_condition_right);

  		fprintf(asm_code, "\nL%lu:\n", count_of_functions - 1);
  	}

  	void compiling_loop(Node* node, const OPER_INTO_COND remember_or_not = NOT_INTO_COND) {
		size_t loop_label = count_of_functions, exit_from_loop_label = 0;
  		++count_of_functions;

  		fprintf(asm_code, "\nL%lu:\n", loop_label);
		Condition_parts values_in_cond = compiling_condition_expression(node);

		exit_from_loop_label = count_of_functions;
		++count_of_functions;

		bool is_condition_right = get_truth_of_cond(node->left, values_in_cond.left_value, values_in_cond.right_value);

  		fprintf(asm_code, "%s L%lu\n\n", CPU_TEXT_OPERATION[(int)get_index_of_opposite_CPU_operation(get_index_of_CPU_operation((COMPARISON_SIGNS)node->left->value))], 
  									     exit_from_loop_label);

  		compiling(node->right->left, (OPER_INTO_COND)is_condition_right);
		fprintf(asm_code, "jmp L%lu\n\nL%lu:\n", loop_label, exit_from_loop_label);
  	}

  	Condition_parts compiling_condition_expression(Node* node) {
		enum TYPE_CONDS {
  			NOTHING_TYPE 	   = 0,
  			LEFT_VAR_RIGHT_VAR = 1,
  			LEFT_NUM_RIGHT_VAR = 2,
  			LEFT_VAR_RIGHT_NUM = 3,
  			LEFT_NUM_RIGHT_NUM = 4,
  		};

  		TYPE_CONDS now_type = NOTHING_TYPE;
  		size_t left_var_addrss = 0, right_var_addrss = 0, left_value = 0, right_value = 0;

  		if((int)node->left->left->type == VARIABLE && (int)node->left->right->type == VARIABLE) {	// x < y
  			now_type = LEFT_VAR_RIGHT_VAR; 
  			left_var_addrss  = find_variable_address(node->left->left->value);
  			right_var_addrss = find_variable_address(node->left->right->value);
  			fprintf(asm_code, "push [%lu]\n"
  							  "pop rax\n"
  							  "push [%lu]\n"
  							  "pop rbx\n"
  							  "cmp rax rbx\n", left_var_addrss, right_var_addrss);
  			left_value = memory[left_var_addrss];      right_value = memory[right_var_addrss];
  		} 
  		else
  		if((int)node->left->left->type == NUMBER && (int)node->left->right->type == VARIABLE) {		// 5 < x
  		  	now_type = LEFT_NUM_RIGHT_VAR;
  		  	right_var_addrss = find_variable_address(node->left->right->value);
  			fprintf(asm_code, "push [%lu]\n"
  							  "pop rax\n"
							  "cmp %d rax\n", right_var_addrss, (int)node->left->left->value);
  			left_value = (int)node->left->left->value; right_value = memory[right_var_addrss];
  		}
  		else
  		if((int)node->left->left->type == VARIABLE && (int)node->left->right->type == NUMBER) {		// x < 5
  			now_type = LEFT_VAR_RIGHT_NUM;
  			left_var_addrss = find_variable_address(node->left->left->value);
  			fprintf(asm_code, "push [%lu]\n"
  							  "pop rax\n"
							  "cmp rax %d\n", left_var_addrss, (int)node->left->right->value);
  			left_value = memory[left_var_addrss];      right_value = (int)node->left->right->value;
  		}
  		else
  		if((int)node->left->left->type == NUMBER && (int)node->left->right->type == NUMBER)	{		// 3 < 5
  			now_type = LEFT_NUM_RIGHT_NUM;
  			fprintf(asm_code, "cmp %d %d\n", (int)node->left->left->value, (int)node->left->right->value);
  			left_value = (int)node->left->left->value; right_value = (int)node->left->right->value;
  		}  		

  		Condition_parts values = {};
  		values.left_value  = left_value;
  		values.right_value = right_value;

  		return values;
  	}

  	void compiling_assignment(Node* &node, FILE* need_buffer, const OPER_INTO_COND remember_or_not = NOT_INTO_COND) {
  		size_t var_addrss = find_variable_address(node->left->value);
  		//printf("\tassignment! var address %lu, var %c\n", var_addrss, (char)node->left->value);
  		fprintf(need_buffer, "push rax\n"
  							 "push rdx\n"
  						     "push %ld\n"
  						     "pop rdx\n", var_addrss);

  		node = evaluate_expression(node);
  		//draw_draw_node(prog_tree->root);

  		if(node->type != ASSIGN_TYPE || node->type == ASSIGN_TYPE && node->value == SIGN_ASSIGNMENT) {
	  		if(node->right->type == NUMBER) {
	  			//printf("\tright is number, %d\n", (int)node->right->value);
	  			fprintf(need_buffer, "push %d\n"
	  							     "pop [rdx]\n", (int)node->right->value);

	  			if(remember_or_not != INTO_FALSE_OPTION)
	  				memory[var_addrss] = (int)node->right->value;
	  		}  			
	  	  	else
	  	  	if(node->right->type == VARIABLE) {
	  	  		size_t right_var_addrss = find_variable_address(node->right->value);
	  	  		//printf("\tright is var, %c, flag %d\n", (char)node->right->value, remember_or_not);
	  			fprintf(need_buffer, "push %d\n"
	  			                     "pop [rdx]\n", memory[right_var_addrss]);

	  			if(remember_or_not != INTO_FALSE_OPTION)
	  				memory[var_addrss] = memory[right_var_addrss];
	  	  	}
	  	  	else
	  			compilation_error(INFORMATION_ABOUT_CALL, "assignment error!");
  		}
  		else {
  			fprintf(asm_code, "push [rdx]\n"
  							  "push %d\n", (int)node->right->value);

  			switch((int)node->value) {
  				case SIGN_ASSIGN_NOTHING:
  				case SIGN_ASSIGNMENT:
  					break;
  				default:
  					fprintf(asm_code, "%s\n"
  									  "pop [rdx]\n", TEXT_ASM_ARITHMETIC_OPERATIONS[(int)node->value]);
  			}

  			if(remember_or_not != INTO_FALSE_OPTION) {
	  			switch((int)node->value) {
	  				case SIGN_ADDITION:
	  					memory[var_addrss] += node->right->value;
	  					break;
	  				case SIGN_SUBTRACTION:
	  					memory[var_addrss] -= node->right->value;
	  					break;
	  				case SIGN_MULTIPLICATION:
	  					memory[var_addrss] *= node->right->value;
	  					break;
	  				case SIGN_DIVISION:
	  					memory[var_addrss] /= node->right->value;
	  					break;
	  			};
	  		}
  		}

  		fprintf(need_buffer, "pop rdx\n"
  							 "pop rax\n");
  	}

  	CPU_OPERATIONS_CODES get_index_of_CPU_operation(const COMPARISON_SIGNS operation) {
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

  	bool get_truth_of_cond(Node* node, int left_value, int right_value) {
  		switch((int)node->value) {
  			case SIGN_EQUAL:
  				return left_value == right_value;
  			case SIGN_NOT_EQUAL:
  				return left_value != right_value;
  			case SIGN_LESS:
  				return left_value < right_value;
  			case SIGN_LESS_OR_EQUAL:
  				return left_value <= right_value;
  			case SIGN_MORE:
  				return left_value > right_value;
  			case SIGN_MORE_OR_EQUAL:
  				return left_value >= right_value;
  			default:
  				return false;
  		}
  	}

  	void substitute_variable_values(Node* &node) {
  		if(!node)
  			return;

  		//printf("substitute_variable_values, node: type %d, value %d, number: %lu\n", (int)node->type, (int)node->value, node->number_node);
  		substitute_variable_values(node->left);
  		substitute_variable_values(node->right);

  		if(node->type == OPERATOR && node->left && node->left->type == VARIABLE && node->value != EQUAL) {
  			node->left->type  = NUMBER;
  			node->left->value = memory[find_variable_address((char)node->left->value)];
  		}

  		if((node->type == OPERATOR || node->type == ASSIGN_TYPE) && node->right && node->right->type == VARIABLE) {
  			node->right->type  = NUMBER;
  			node->right->value = memory[find_variable_address((char)node->right->value)];
  		}

  		left_num_right_num(node);
  	}

  	void left_num_right_num(Node* &node) {
  		if(!node || !node->left || !node->right || node && node->type != OPERATOR)
  			return;

  		switch((int)node->value) {
  			case ADD:
  				node->value = node->left->value + node->right->value; 		break;
  			case SUB:
  				node->value = node->left->value - node->right->value; 		break;  			
  			case MUL:
  				node->value = node->left->value * node->right->value; 		break;  			
			case DIV:
				if(fabs(node->right->value) < EPSILON) {
					compilation_error(INFORMATION_ABOUT_CALL, "Division by zero");
					return;
				}
  				node->value = node->left->value / node->right->value; 		break;			
			case POW:
  				node->value = pow(node->left->value, node->right->value); 	break;			
			case SIN:
  				node->value = sin(node->left->value); 						break;			
			case COS:
  				node->value = cos(node->left->value); 						break;			
			case TG: 
  				node->value = tan(node->left->value); 						break;			
			case CTG:
  				node->value = 1.0 / tan(node->left->value); 				break;			
			case ARCSIN:
  				node->value = asin(node->left->value); 						break;			
			case ARCCOS:
  				node->value = acos(node->left->value); 						break;			
			case ARCTG: 
  				node->value = atan(node->left->value); 						break;			
			case ARCCTG:
  				node->value = 1.0 / atan(node->left->value); 				break;			
			case LN :   
  				node->value = log(node->left->value) / log(exp(1)); 		break;			
			case LG :	
  				node->value = log(node->left->value); 						break;	
  			default:
  				return;		
  		}

  		node->type = NUMBER;
  		node->left = node->right = NULL;
  		free(node->left);
  		free(node->right);
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

  		count_of_functions = 0;
  	}

  	void destruct() {
  		stack_destruct(&stack_frame);
  		fclose(asm_code);
  		tree_destruct(prog_tree);
  		free(memory);
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


  	Node* evaluate_expression(Node* node) {
  		//printf("eval! node: type %d, value %d, number: %lu\n", (int)node->type, (int)node->value, node->number_node);
  		if(!node)
  			return NULL;

  		if(node->left &&  node->left->type == OPERATOR)
  			node->left  = evaluate_expression(node->left);

  		if(node->right && node->right->type == OPERATOR)
  			node->right = evaluate_expression(node->right);

  		/*Node* copy_node = node_construct(node->type, node->value, NULL, NULL);
  		node_make_copy(node->left,  copy_node->left);
  		node_make_copy(node->right, copy_node->right);*/

  		substitute_variable_values(node);

  		//printf("\n\n");

  		return node;
  	}
};



void compile(Tree* tree);