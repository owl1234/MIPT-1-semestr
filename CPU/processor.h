#include "common.h"

void file_handler(File file);

int check_commands();

int length_of_number(int value);

/*void move_ptr(char* text, int value);

void move_ptr(char* text, char* value);*/

double string_to_double(char* text);

void push_in_registers(int registr, Elem_t* registers_variables, Stack_t* proc_stack);

void proc_push(File* file, char* push_value, Stack_t* proc_stack, Elem_t* registers_variables);

void proc_pop(File* file, Stack_t* proc_stack, Elem_t* registers_variables);

void pop_in_registers(int registr, Elem_t* registers_variables, Stack_t* proc_stack);

void proc_arifmetics(Stack_t* proc_stack, int operation_code);
