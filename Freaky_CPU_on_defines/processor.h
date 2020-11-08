#include "common.h"

#ifndef PROCESSOR_H
#define PROCESSOR_H

const struct call_of_dump proc_base_arguments_of_call = {__FILE__, -1, " "};

typedef enum MY_PROCESSOR_ERRORS {
    PROC_OKEY     = 0,
    PROC_BAD_PTR_TO_PROC = 1,
    PROC_BAD_SIZE_PROC_STACK = 2,
    PROC_BAD_SIZE_CALL_STACK = 3,
    PROC_BAD_RAM  = 4,
    PROC_BAD_READ_FROM_FILE = 5,
} PROCESSOR_ERRORS;

const char TEXT_PROCESSOR_ERRORS[][30] = {
    "Processor is okey",
    "Bad size of processor stack",
    "Bad size of call stack",
    "Bad ram"
};

struct Processor {
    double* text;
    int symbols;
    Stack_t proc_stack;
    Stack_t call_stack;
    Elem_t* registers_variables;
    Elem_t* ram;
    PROCESSOR_ERRORS status;
    char* name_log_file;
};

void help();

void processor_dump(Processor* processor, struct call_of_dump arguments_of_call);

void error_print_data(Elem_t* array_elem_t, const int length, FILE* file);

void processor_verificator(Processor* processor, struct call_of_dump arguments_of_call);

int initialization_proc(Processor* processor, const char* name_input_file, const char* name_log_file);

void init_ram(Processor* processor);

void file_handler(Processor* processor);

int size_of_buffer(FILE* file);

int check_commands();

void print_ram(Elem_t* ram);

Elem_t get_value_to_compare(Processor* processor, int* now_byte);

int get_double_from_text(Processor* processor, int* now_byte);

void destruct_processor(Processor* processor);

#endif // PROCESSOR_H
