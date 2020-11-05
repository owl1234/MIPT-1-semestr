#include <sys/stat.h>

#ifndef COMMON_H
#define COMMON_H

#include "operation_codes.h"
#include "stack.h"

#define DEFINE_COMMANDS(name, number, arg, code)                                                   \
    if (!strcmp(command, #name))                                                                   \
    {                                                                                              \
        code = COM_##name;                                                                         \
        printf("gav-gav\n");                                                                       \
    }


const int MAX_SIZE = 10;
const int MAX_SIZE_RAM = 100000;
const int ERROR_NUMBER = -3802;
const double EPSILON = 1e-6;
const int OK = 0;
const int number_of_register_vars = 4;
const int number_of_commands = sizeof(TEXT_OPERATION) / sizeof(TEXT_OPERATION[0]);
const int MAX_COUNT_LABELS = 20;

const char SEPARATORS[] = "\n ";

enum FLAGS_OF_THE_END_LINE {
    END_LINE = 0,
    SPACE    = 1,
    NOTHING  = 2
};

enum TYPE_OF_ARGUMENT {
    NOT_ARGS    = 0, // for pop
    IS_ELEM_T   = 1,
    IS_REGISTER = 2,
    IS_RAM      = 4,
};

struct File {
    FILE* ptr_to_file;
    const char* name;
    char* text_for_assembling;
    double* text_for_disassembling;
    int lines;
    struct stat information;
};

struct Processor {
    double* text;
    double* copy_of_text;
    int symbols;
    Stack_t proc_stack;
    Stack_t call_stack;
    Elem_t* registers_variables;
    Elem_t* ram;
};

struct Label {
    char* name;
    int address; //symbol_address;
    int byte_address;
    int type_of_command;
};

void ERROR(const char* status);

int is_right_command(const char* line, const char* command);

int number_of_symbols(char* buffer, char separator);

int file_construct(File* file, const char* name_file, const char* reading_mode);

int type_of_value(const char* operation);

int get_number_of_register(const char* text);

#endif // COMMON_H
