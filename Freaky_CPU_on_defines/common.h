#include <sys/stat.h>
#include <limits.h>

#ifndef COMMON_H
#define COMMON_H

#include "operation_codes.h"
#include "stack.h"

const int VERSION = 1;
const int SIGNATURE_SIZE = 10;
const long long SIGNATURE_NAME_HASH = 1343871799; // 0x64F3F67CF65BE1CBA2C649705203B97E % ULLONG_MAX;

const int MAX_SIZE = 11;
const int MAX_SIZE_RAM = 10;
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

const char TEXT_FLAGS_OF_THE_END_LINE[][2] = {
    "\n",
    " ",
    ""
};

typedef enum COUNT_OF_ARGS {
    ONE_ARGUMENT    = 1,
    TWO_ARGUMENTS   = 2,
    THREE_AGRUMENTS = 3,
    FOUR_ARGUMENTS  = 4,
    FIVE_ARGUMENTS  = 5,
} COUNT_OF_ARGUMENTS;

enum TYPE_OF_ARGUMENT {
    NOT_ARGS    = 0, // for pop
    IS_ELEM_T   = 1,
    IS_REGISTER = 2,
    IS_RAM      = 4,
};

struct Label {
    char* name;
    int byte_address;
    //int type_of_command;
};

int size_of_buffer(FILE* file);

int is_right_command(const char* line, const char* command);

int type_of_value(const char* operation);

int get_number_of_register(const char* text);

int length_of_number(int value);

bool is_it_register(const char* text);

double string_to_double(char* text);

#endif // COMMON_H
