#include "work_with_tree.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "operation_codes.h"
#include "warnings.h"

/*
		G 		::= Cond | L {L}* '\0'																					//L {L}* '\0'
																													//L 		::= V_DECL | A ;															- line
		Cond 	::= PHRASE_COND Bdy_cond 
					PHRASE_BEGIN_ACTION_1 PHRASE_BEGIN_BLOCK Act_cond PHRASE_END_BLOCK 
					PHRASE_BEGIN_ACTION_2 PHRASE_BEGIN_BLOCK Act_cond PHRASE_END_BLOCK 		- condition
		Bdy_cond::= PHRASE_BEGIN_COND E COMPARISON_SIGNS E PHRASE_END_COND												
		Act_cond::= L 																							
		V_DECL  ::= PHRASE_DECL V {'=' E}*													- var decl
		L 		::= V_DECL | A | PRINT;														- line
		PRINT   ::= PHRASE_PRINT PHRASE_BEGIN_BLOCK [V N] PHRASE_END_BLOCK 					- print
		A 		::= V '=' E 																- assignment
		E 		::= T {[+ -], T}* 															- expression
		T 		::= D {[* /], D}* 															- term
		D 		::= U {^ U}*		 														- degree
		U 		::= [+ -]? P      															- unary
		P 		::= '('E')' | N | F | V 													- priority
		F 		::= 'sin('E')'    | 'cos('E')'    | 'tg('E')'    | 'ctg('E')'    | 
			  		'arcsin('E')' | 'arccos('E')' | 'arctg('E')' | 'arcctg('E')' |			- function
			  		'lg('E')'     | 'ln('E')'
		V 		::= ['a'-'z']+ 																- variable
		N 		::= ['0'-'9']																- number
*/

#define IF_DEBUG_PARSING(code) //code

#define SEARCH_BEGIN_BLOCK													\
   	if(!get_phrase(PHRASE_BEGIN_BLOCK, LENGTH_PHRASE_BEGIN_BLOCK))			\
  		return NULL;														\
  	pointer += LENGTH_PHRASE_BEGIN_BLOCK;	

#define SEARCH_END_BLOCK													\
   	if(!get_phrase(PHRASE_END_BLOCK, LENGTH_PHRASE_END_BLOCK))				\
  		return NULL;														\
  	pointer += LENGTH_PHRASE_END_BLOCK;			

const int BIG_NUMBER = 16;

enum SPECIAL_INFO {
	NOTHING 		   = 0,
	FIND_VAR_INTO_DECL = 1,
};

struct Parser {
	const char* pointer;
	bool used_vars[ALPHABET];
	SPECIAL_INFO special_info;

  private:
  	bool is_space(char symbol) {
  		if(symbol == ' ' || symbol == '	' || symbol == '\n')
  			return true;
  		return false;
  	}

  	void skip_spaces() {
  		while(is_space(*pointer))
  			++pointer;
  	}

  	void syntax_error(struct call_of_dump info, const char message[] = "syntax error") {
  		warning(message, info);
  	}

  	bool require(char symbol) {
  		if(symbol != *pointer)
  			return false;

  		if(*pointer != '\0')
  			++pointer;
  		return true;
  	}

  	bool get_phrase(const char* phrase, const int length) {
  		for(int index = 0; index < length; ++index)
  			if(*(pointer + index) != phrase[index]) {
  				return false;
  			}

  		return true;
  	}

  	COMPARISON_SIGNS get_sign_condition() {
  		for(int index = 1; index < COUNT_OF_COMPARISON_SIGNS; ++index) {
  			//printf("\tcheck index %d... now %s, len %lu\n", index, TEXT_COMPARISON_SIGNS[index], LENGTH_TEXT_COMPARISON_SIGNS[index]);
  			if(get_phrase(TEXT_COMPARISON_SIGNS[index], LENGTH_TEXT_COMPARISON_SIGNS[index]) == true)
  				return (COMPARISON_SIGNS)index;
  		}

  		return SIGN_NOTHING;
  	}

  	Node* get_number() {
  		IF_DEBUG_PARSING(printf("GET_N\n");)
  		char* ptr_after_number = NULL;
		double value = strtod(pointer, &ptr_after_number);

		if(pointer == ptr_after_number)
			return NULL;

		pointer = ptr_after_number;

		Node* tmp = node_construct(NUMBER, value, NULL, NULL);
		return tmp;
  	}

  	Node* get_variable() {
  		IF_DEBUG_PARSING(printf("GET_V\n");)
  		skip_spaces();

  		if(get_phrase(PHRASE_TO_VARIABLE_DECLARATION, LENGTH_PHRASE_TO_VARIABLE_DECLARATION) ||
  		   get_phrase(PHRASE_TO_CONDITION,            LENGTH_PHRASE_TO_CONDITION)			 ||
  		   get_phrase(PHRASE_BEGIN_ACTION_1, 		  LENGTH_PHRASE_BEGIN_ACTION_1)			 ||
  		   get_phrase(PHRASE_BEGIN_ACTION_2, 		  LENGTH_PHRASE_BEGIN_ACTION_2))
  			return NULL;

  		if('a' <= *pointer && *pointer <= 'z') {
  			if(used_vars[*pointer - 'a'] == false && !(special_info == FIND_VAR_INTO_DECL)) {
  				char msg[128] = "";
  				sprintf(msg, "variable %c not declared", *pointer);
  				syntax_error(INFORMATION_ABOUT_CALL, msg);
  				return NULL;
  			}

  			used_vars[*pointer - 'a'] = true;
  			Node* new_node = node_construct(VARIABLE, *pointer, NULL, NULL);
  			++pointer;
  			return new_node;
  		}

  		return NULL;
  	}

  	Node* get_function() {
  		IF_DEBUG_PARSING(printf("GET_F\n");)
  		skip_spaces();
  		Node* new_node = NULL;
  		const char* find_str = NULL;
  		
  		for(int function = SIN; function <= LG; ++function) {
  			char* buffer = (char*)calloc(BIG_NUMBER, sizeof(char));
  			strcat(buffer, (char*)TEXT_OPERATIONS[function]);  			
  			strcat(buffer, "(");

	  		find_str = strstr(pointer, buffer);

	  		if(find_str == pointer) {
	  			new_node = node_construct(OPERATOR, function, NULL, NULL);
	  			pointer += OPERATIONS_LENGTH[function] + 1;
	  			new_node->left = get_expression();
	  		} else {
	  			free(buffer);
	  			continue;
	  		}

	  		free(buffer);

	  		skip_spaces();
	  		if(!require(')')) {
	  			node_destruct(new_node);
	  			return NULL;
	  		} else
	  			break;
	  	}

  		return new_node;
  	}

  	Node* get_priority() {
  		IF_DEBUG_PARSING(printf("GET_P\n");)
  		skip_spaces();
  		Node* new_node = NULL;

  		if(*pointer == '(') {
  			++pointer;
  			new_node = get_expression();
  			if(new_node == NULL) {
  				node_destruct(new_node);
  				return NULL;
  			}

  			skip_spaces();
  			if(!require(')')) {
  				node_destruct(new_node);
  				return NULL;
  			}

  			return new_node;
  		}

  		new_node = get_number();
  		if(new_node != NULL) {
  			return new_node;
  		}

  		new_node = get_function();
  		if(new_node != NULL)
  			return new_node;

  		new_node = get_variable();
  		if(new_node != NULL)
  			return new_node;

  		node_destruct(new_node);

  		return NULL;
  	}

  	Node* get_unary() {
  		IF_DEBUG_PARSING(printf("GET_U\n");)
  		skip_spaces();

  		if(*pointer == '+')
  			++pointer;
  		else if(*pointer == '-') {
  			++pointer;
  			Node* new_node = node_construct(OPERATOR, MUL, NULL, NULL);
  			new_node->left  = node_construct(NUMBER, -1.0, NULL, NULL);
  			new_node->right = get_priority();

  			return new_node;
  		}

  		return get_priority();
  	}

  	Node* get_degree() {
  		IF_DEBUG_PARSING(printf("GET_D\n");)
	  	skip_spaces();
	  	Node* left_son = get_unary();
	  	skip_spaces();

	  	if(left_son == NULL) {
	  		node_destruct(left_son);
	  		return NULL;
	  	}

	  	if(*pointer != '^')
	  		return left_son;

	  	Node* new_node = node_construct(OPERATOR, POW, NULL, NULL);
	  	node_make_copy(left_son, new_node->left);
	  	free(left_son);
	  	++pointer;
	  	new_node->right = get_degree();
	  	return new_node;
  	}

  	Node* get_term() {
  		IF_DEBUG_PARSING(printf("GET_T\n");)
  		skip_spaces();
  		Node* left_son = get_degree();
  		skip_spaces();

  		if(left_son == NULL)
  			return NULL;

  		int code_operation = 0;

  		if(*pointer == '*')
  			code_operation = MUL;
  		else if(*pointer == '/')
  			code_operation = DIV;
  		else
  			return left_son;

  		Node* new_node = node_construct(OPERATOR, code_operation, NULL, NULL);
  	  	node_make_copy(left_son, new_node->left);
	  	free(left_son);
  		++pointer;
  		new_node->right = get_term();

  		return new_node;
  	}

  	Node* get_expression() {
  		IF_DEBUG_PARSING(printf("GET_E\n");)
  		skip_spaces();
  		Node* left_son = get_term();
  		skip_spaces();

  		if(left_son == NULL)
  			return NULL;

  		int code_operation = 0;
  		if(*pointer == '+')
  			code_operation = ADD;
  		else if(*pointer == '-')
  			code_operation = SUB;
  		else
  			return left_son;

  		Node* new_node = node_construct(OPERATOR, code_operation, NULL, NULL);
  	  	node_make_copy(left_son, new_node->left);
	  	free(left_son);
  		++pointer;
  		new_node->right = get_expression();

  		return new_node;
  	}

  	Node* get_assignment() {
  		IF_DEBUG_PARSING(printf("GET_ASS\n");)
  		skip_spaces();
  		Node* left_son = get_variable();
  		skip_spaces();

  		if(left_son == NULL)
  			return NULL;

  		if(*pointer != '=') {
  			syntax_error(INFORMATION_ABOUT_CALL);;
  			return NULL;
  		}

  		++pointer;

  		Node* right_son = get_expression();

  		Node* new_node = node_construct(OPERATOR, EQUAL, NULL, NULL);
  	  	node_make_copy(left_son,  new_node->left);
  	  	node_make_copy(right_son, new_node->right);

	  	free(left_son);
	  	free(right_son);

  		return new_node;
  	}

   	Node* get_variable_declaration() {
  		IF_DEBUG_PARSING(printf("GET_VAR_DECL\n");)
  		skip_spaces();
  		if(get_phrase(PHRASE_TO_VARIABLE_DECLARATION, LENGTH_PHRASE_TO_VARIABLE_DECLARATION) == false)
  			return NULL;

  		pointer += LENGTH_PHRASE_TO_VARIABLE_DECLARATION;
  		skip_spaces();

  		special_info = FIND_VAR_INTO_DECL;
  		Node* left_son = get_variable();
  		special_info = NOTHING;

  		if(left_son == NULL) {
  			syntax_error(INFORMATION_ABOUT_CALL);
  			return NULL;
  		}

  		/*if(used_vars[(int)left_son->value - 'a'] == true) {
  			char msg[128] = "";
  			sprintf(msg, "variable %c was previously declared", (int)left_son->value);
  			syntax_error(INFORMATION_ABOUT_CALL, msg);
  			node_destruct(left_son);
  			return NULL;
  		}*/

  		used_vars[(int)left_son->value - 'a'] = true;

  		skip_spaces();

  		if(*pointer != '=') 
  			return left_son;
  		
  		++pointer;

  		Node* right_son = get_expression();

  		Node* new_node = node_construct(OPERATOR, EQUAL, NULL, NULL);
  	  	node_make_copy(left_son,  new_node->left);
  	  	node_make_copy(right_son, new_node->right);

	  	free(left_son);
	  	free(right_son);

  		return new_node;
  	}

  	Node* get_print() {
   		IF_DEBUG_PARSING(printf("GET_PRINT\n");)
  		skip_spaces();

   		if(!get_phrase(PHRASE_PRINT, LENGTH_PHRASE_PRINT)) {
   			//printf("don't find print(((\n");
  			return NULL;
   		}

  		pointer += LENGTH_PHRASE_PRINT; 
  		skip_spaces();

   		if(!get_phrase(PHRASE_BEGIN_BLOCK, LENGTH_PHRASE_BEGIN_BLOCK))
  			return NULL;

  		pointer += LENGTH_PHRASE_BEGIN_BLOCK;

  		skip_spaces();	

  		Node* new_node = node_construct(PRINT, PRINT, NULL, NULL);
  		new_node->left = get_variable();

  		if(new_node->left == NULL)
  			new_node->left = get_number();

  		skip_spaces();

   		if(!get_phrase(PHRASE_END_BLOCK, LENGTH_PHRASE_END_BLOCK))
  			return NULL;

  		pointer += LENGTH_PHRASE_END_BLOCK;	

  		skip_spaces();

  		return new_node;	
  	}

  	Node* get_body_condition() {
  		IF_DEBUG_PARSING(printf("GET_BODY_CONDITION\n");)
  		skip_spaces();

   		if(!get_phrase(PHRASE_BEGIN_CONDITION, LENGTH_PHRASE_BEGIN_CONDITION))
  			return NULL;

  		pointer += LENGTH_PHRASE_BEGIN_CONDITION;

  		Node* left_son = get_expression(); 
  		if(left_son == NULL) {
  			syntax_error(INFORMATION_ABOUT_CALL, "Left part of expression not found"); return NULL;
  		}

  		for(int i=0; i < 7; ++i)
  			printf("%c", *(pointer + i));
  		printf("\n");


  		COMPARISON_SIGNS sign_cnd = get_sign_condition();
  		printf("Find sign comparison %d\n", sign_cnd);
  		if(sign_cnd == 0) {
  			syntax_error(INFORMATION_ABOUT_CALL, "Sign of condition not found"); return NULL;
  		}
  		pointer += LENGTH_TEXT_COMPARISON_SIGNS[sign_cnd];

  		skip_spaces();

  		Node* right_son = get_expression();
  		if(right_son == NULL) {
  			syntax_error(INFORMATION_ABOUT_CALL, "Right part of expression not found"); return NULL;
  		}

   		if(!get_phrase(PHRASE_END_CONDITION, LENGTH_PHRASE_END_CONDITION))
  			return NULL;

  		pointer += LENGTH_PHRASE_END_CONDITION;

  		Node* answer = node_construct(COMPARE_SIGN, sign_cnd, NULL, NULL);
  		node_make_copy(left_son,  answer->left);
  		node_make_copy(right_son, answer->right);

  		return answer;
  	}

  	Node* get_condition() {
  		IF_DEBUG_PARSING(printf("GET_ACTION_CONDITION\n");)
  		skip_spaces();
  		
  		if(!get_phrase(PHRASE_TO_CONDITION, LENGTH_PHRASE_TO_CONDITION)) {
  			return NULL;
  		}
		
		pointer += LENGTH_PHRASE_TO_CONDITION;
  		skip_spaces();

  		Node* left_son = get_body_condition();
  		if(left_son == NULL) {
  			syntax_error(INFORMATION_ABOUT_CALL, "Body condition not found"); return NULL;
  		} /*else
  			printf("find body cond\n");*/

  		//printf("%c%c%c%c\n", *pointer, *(pointer + 1), *(pointer + 2), *(pointer + 3));

  		skip_spaces();

  		if(!get_phrase(PHRASE_BEGIN_ACTION_1, LENGTH_PHRASE_BEGIN_ACTION_1)) {
  			syntax_error(INFORMATION_ABOUT_CALL, "don't find phrase to action 1\n"); return NULL;
  		}
		
		pointer += LENGTH_PHRASE_BEGIN_ACTION_1;
  		skip_spaces();

   		SEARCH_BEGIN_BLOCK

  		Node* right_left_son  = get_line();

  		skip_spaces();

   		SEARCH_END_BLOCK

  		skip_spaces();

  		if(!get_phrase(PHRASE_BEGIN_ACTION_2, LENGTH_PHRASE_BEGIN_ACTION_2)) {
  			syntax_error(INFORMATION_ABOUT_CALL, "don't find phrase to action 2\n"); return NULL;
  		}
		
		pointer += LENGTH_PHRASE_BEGIN_ACTION_2;
  		skip_spaces();

   		SEARCH_BEGIN_BLOCK

  		Node* right_right_son = get_line();
  		skip_spaces();

   		SEARCH_END_BLOCK

  		if(right_left_son == NULL || right_right_son == NULL) {
  			syntax_error(INFORMATION_ABOUT_CALL, " not found"); return NULL;
  		} 

  		Node* right_son = node_construct(COND_TYPE, ACTIONS, NULL, NULL);
  		node_make_copy(right_left_son,  right_son->left);
  		node_make_copy(right_right_son, right_son->right);

  		Node* answer = node_construct(COND_TYPE, CONDITION, NULL, NULL);
  		node_make_copy(left_son,  answer->left);
  		node_make_copy(right_son, answer->right);

  		return answer;
  	}

  	Node* get_action_condition() {
  		IF_DEBUG_PARSING(printf("GET_ACTION_CONDITION\n");)
  		skip_spaces();
  		
 		/*Node* new_node = get_condition();

 		if(new_node == NULL)
 			new_node = get_line();*/

  		Node* new_node = get_line();

 		if(new_node == NULL)
 			syntax_error(INFORMATION_ABOUT_CALL);
 		return new_node;
  	}

  	Node* get_line() {
		IF_DEBUG_PARSING(printf("GET_LINE\n");)
		skip_spaces();

		bool is_it_condition = true;

		//printf("\tline!!\n");

		Node* new_node = node_construct(OPERATOR, SEMICOLON, NULL, NULL);
		new_node->left = get_condition();

		if(new_node->left == NULL) {
			new_node->left = get_print();
			is_it_condition = false;
		} else printf("find cond\n");

		if(new_node->left == NULL) {
  			new_node->left = get_variable_declaration();
		} /*else printf("find print!!!\n");*/

  		if(new_node->left == NULL) {
  			new_node->left = get_assignment();
  		} /*else printf("find var declare\n");*/

  		if(new_node->left == NULL)
  			return NULL;
  		/*else printf("find assgnmt\n");*/
  		skip_spaces();

  		//printf("well, if? %c, %d\n", *pointer, is_it_condition);

  		if(*pointer == ';' || is_it_condition) {
  			if(!is_it_condition)
  				++pointer;
  			skip_spaces();

  			new_node->right = get_line();
  		} else if(is_it_condition == false) {
  			syntax_error(INFORMATION_ABOUT_CALL, "Semicolon not found\n");
  		}

  		return new_node;
  	}


  public:
  	void construct(bool* input_vars) {
  		pointer = NULL;
  		for(int i = 0; i < ALPHABET; ++i)
  			used_vars[i] = input_vars[i];
  	}

  	Node* get_G(const char* line_to_parsing) {
  		IF_DEBUG_PARSING(printf("GET_G\n");)
  		if(line_to_parsing == NULL) {
  			warning("Bad string to parsing", base_arguments_of_call);
  			return NULL;
  		}

  		pointer = line_to_parsing;
  		skip_spaces();
  		Node* root = get_condition();
  		if(root == NULL)
  			root = get_line();
  		skip_spaces();

  		if(!require('\0') || root == NULL) {
  			//printf("%d, %c, %d, %c, %p\n", *pointer, *pointer, *(pointer + 1), *(pointer + 1), root);
  			syntax_error(INFORMATION_ABOUT_CALL, "fatal error");
  			node_destruct(root);
  			return NULL;
  		}

  		return root;
  	}
};

void tree_fill(Tree* tree, const char* name_file);
void get_variables(Tree* tree);
void get_variables(Node* node, bool* used_vars);


