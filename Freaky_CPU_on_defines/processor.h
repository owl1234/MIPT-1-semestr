#include "common.h"

#ifndef PROCESSOR_H
#define PROCESSOR_H

const struct call_of_dump proc_base_arguments_of_call = {__FILE__, -1, " "};

typedef enum {
    PROC_OKEY                = 0,
    PROC_BAD_PTR_TO_PROC     = 1,
    PROC_BAD_SIZE_PROC_STACK = 2,
    PROC_BAD_SIZE_CALL_STACK = 3,
    PROC_BAD_RAM             = 4,
    PROC_BAD_READ_FROM_FILE  = 5,
    PROC_UNKNOWN_COMMAND     = 6,
    PROC_BAD_VERSION         = 7,
    PROC_BAD_FILE            = 8,
} PROCESSOR_ERRORS;

const char TEXT_PROCESSOR_ERRORS[][30] = {
    "Processor is okey",
    "Bad ptr of processor",
    "Bad size of processor stack",
    "Bad size of call stack",
    "Bad ram",
    "Don't read from file",
    "Unknown command",
    "Bad version"
};

struct Processor {
    char* text;
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

PROCESSOR_ERRORS processing(Processor* processor);

int size_of_buffer(FILE* file);

int check_commands();

void print_ram(Elem_t* ram);

Elem_t get_value_to_compare(Processor* processor, int* now_byte);

Elem_t get_byte_from_text(Processor* processor, int* now_byte);

Elem_t get_double_from_text(Processor* processor, int* now_byte);

void destruct_processor(Processor* processor);

PROCESSOR_ERRORS check_signature(Processor* processor, int* now_byte);

inline long long get_long_long_number_from_binary(Processor* processor, int* now_byte);

#endif // PROCESSOR_H
