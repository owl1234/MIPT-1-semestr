#include "common.h"

void file_handler(File file);

int check_commands();

int length_of_number(int value);

double string_to_double(char* text);

void push_in_registers(File* file, Elem_t* registers_variables, int* now_byte, Stack_t* proc_stack);

void proc_push(File* file, Elem_t* registers_variables, int* now_byte, Stack_t* proc_stack);

void proc_pop(File* file, Stack_t* proc_stack, int* now_byte, Elem_t* registers_variables);

void pop_in_registers(int registr, Elem_t* registers_variables, Stack_t* proc_stack);

void proc_arifmetics(Stack_t* proc_stack, int operation_code);

void proc_jmp(File* file, int* now_byte);

void proc_cmp(File* file, int* now_byte, Elem_t* first_comparison, Elem_t* second_comparison, Elem_t* register_variables);

Elem_t get_value_to_compare(File* file, int* now_byte, Elem_t* register_variables);

bool is_condition(int now_command, int* number_of_condition);

int get_int_from_text(File* file, int* now_byte);

char* get_char_from_text(File* file, int* now_byte);

void proc_conditions(File* file, int number_of_condition, Elem_t first_comparison, Elem_t second_comparison, int* now_byte);
