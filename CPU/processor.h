#include "common.h"

void file_handler(File file);

int check_commands();

int length_of_number(int value);

void move_ptr(char* text, int value);

void move_ptr(char* text, char* value);

double string_to_double(char* text);

void push_in_registers(int registr, Elem_t* registers_variables, Stack_t* proc_stack);

void pop_in_registers(int registr, Elem_t* registers_variables, Stack_t* proc_stack);
