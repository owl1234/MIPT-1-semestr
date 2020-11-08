#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "stack.h"

const int DIMENSION = 8;
const long long HASH_NUMBER = 1000000009;
const int TABULATION_IN_PROBELS = 7;
//const char* name_log_file = "log.txt";
const struct call_of_dump stack_base_arguments_of_call = {__FILE__, -1, " "};

const char* text_stack_t_status[] = {
    "Stack is okey",
    "Stack is error",
    "Stack is empty",
    "Stack is overflow",
    "Capacity is bad",
    "Size is bigger than capacity",
    "Stack created just now",
    "Data is null",
    "Size is bad",
    "Stack is destructed",
    IF_HASH_PROTECTION("Bad hash of stack"),
    IF_CANARY_PROTECTION("Canary is killed")
};

#ifdef IF_CANARY_PROTECTION
int get_hash(Stack_t* node) {
    long long hash_st = 0;
    int size_stack = node->size_stack;
    if(node->stack_status != STACK_EMPTY) {
        for(int i=0; i<=size_stack; ++i) {
            hash_st = (hash_st + node->data[i] - HASH_NUMBER)  - HASH_NUMBER;
        }
    } else {
        hash_st = (hash_st + POISON % HASH_NUMBER)  - HASH_NUMBER;
    }

    IF_CANARY_PROTECTION(hash_st = (hash_st + node->canary_left[0]  % HASH_NUMBER) % HASH_NUMBER;)
    IF_CANARY_PROTECTION(hash_st = (hash_st + node->canary_right[0] % HASH_NUMBER) % HASH_NUMBER;)

    hash_st = (hash_st + node->capacity     % HASH_NUMBER) % HASH_NUMBER;
    hash_st = (hash_st + node->size_stack   % HASH_NUMBER) % HASH_NUMBER;
    hash_st = (hash_st + node->stack_status % HASH_NUMBER) % HASH_NUMBER;

    return hash_st;
}
#endif // IF_CANARY_PROTECTION

void print_Elem_T(int value, FILE* file) {
    fprintf(file, "%d", value);
}

void print_Elem_T(double value, FILE* file) {
    fprintf(file, "%g", value);
}

void print_Elem_T(char value, FILE* file) {
    fprintf(file, "%c", value);
}

IF_CANARY_PROTECTION(
bool is_canary(Elem_t value) {
    return (value == CANARY);
}
)

void error_print_data(Stack_t* node, FILE* file) {
    int stack_all_size  = node->capacity;
    const int base_indent = 2 * TABULATION_IN_PROBELS;

    if(node->stack_status == STACK_BAD_CAPACITY) {
        fprintf(file, "\n");
        fprintf(file, "%*s", base_indent, "!!! ERROR !!! BAD CAPACITY !!!\n");
    } else if(node->stack_status == STACK_BAD_SIZE) {
        fprintf(file, "\n");
        fprintf(file, "%*s", base_indent, "!!! ERROR !!! BAD SIZE !!!\n");
    } else {
        fprintf(file, "\n");
        if(node->data == nullptr) {
            fprintf(file, "%*s", base_indent, "!!! ERROR !!! DATA IS NULL !!!\n");
        } else {
            for(int i=0; i<stack_all_size; ++i) {
                if(node->data[i] == POISON) {
                    fprintf(file, "%*s", base_indent, "[");
                    fprintf(file, "%d] - [%p] - !!! POISON !!!\n", i, &(node->data[i]));
                } else {
                    fprintf(file, "%*s", base_indent, "*[");
                    fprintf(file, "%d] - [%p] - ", i, &(node->data[i]));
                    print_Elem_T(node->data[i], file);
                    fprintf(file, "\n");
                }
            }
        }
    }
}

void error_print_array_elem_t(Elem_t* array_elem_t, const int length, FILE* file) {
    const int base_indent = 2 * TABULATION_IN_PROBELS;

    fprintf(file, "\n");
    if(array_elem_t == nullptr) {
        fprintf(file, "%*s", base_indent, "!!! ERROR !!! DATA IS NULL !!!\n");
    } else {
        for(int i=0; i<length; ++i) {
            if(array_elem_t[i] == POISON) {
                fprintf(file, "%*s", base_indent, "[");
                fprintf(file, "%d] - [%p] - !!! POISON !!!\n", i, &(array_elem_t[i]));
            } else {
                fprintf(file, "%*s", base_indent, "*[");
                fprintf(file, "%d] - [%p] = ", i, &(array_elem_t[i]));
                print_Elem_T(array_elem_t[i], file);
                fprintf(file, "\n");
            }
        }
    }
}

void stack_dump(Stack_t* node, struct call_of_dump arguments_of_call = stack_base_arguments_of_call) {
    FILE* log_errors = fopen(node->name_log_file, "a");

    fprintf(log_errors, "Stack_t [%p]\n", node);
    fprintf(log_errors, "File from which the dump is called %s from line %d (called the function %s)\n", arguments_of_call.name_file, arguments_of_call.number_of_line, arguments_of_call.name_function);
    fprintf(log_errors, "{\n");
    fprintf(log_errors, "\tsize_stack = %ld\n", node->size_stack);
    fprintf(log_errors, "\tcapacity   = %ld\n", node->capacity);
    fprintf(log_errors, "\tStatus of stack: %s\n", text_stack_t_status[node->stack_status]);

    #ifdef IF_CANARY_PROTECTION
    fprintf(log_errors, "\tLeft  canary of stack: %d ", node->canary_left[0]);
    if(node->canary_left[0] == CANARY) {
        fprintf(log_errors, "(okey canary)\n");
    } else {
        fprintf(log_errors, "(BAD CANARY !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)\n");
    }
    fprintf(log_errors, "\tRight canary of stack: %d ", node->canary_right[0]);
    if(node->canary_right[0] == CANARY) {
        fprintf(log_errors, "(okey canary)\n");
    } else {
        fprintf(log_errors, "(BAD CANARY !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)\n");
    }
    #endif

    IF_HASH_PROTECTION(fprintf(log_errors, "\tHash: %d\n", node->stack_hash);)
    fprintf(log_errors, "\tdata [%p]\n", node->data);
    fprintf(log_errors, "\t{");
    error_print_data(node, log_errors);
    fprintf(log_errors, "\n\t}");
    fprintf(log_errors, "\n}\n");
    fprintf(log_errors, "\n\n");

    fclose(log_errors);
}

struct call_of_dump create_struct(const char* file_name, int number, const char* function_name) {
    struct call_of_dump tmp = {};
    tmp.name_file      = file_name;
    tmp.number_of_line = number;
    tmp.name_function  = function_name;
    return tmp;
}

#ifdef IF_DEBUG
void stack_err(Stack_t* node, struct call_of_dump arguments_of_call = stack_base_arguments_of_call) {
    //printf("stack_err: status: %d, line: %d\n", node->stack_status, arguments_of_call.number_of_line);
    if(node == nullptr) {
        node->stack_status = STACK_ERROR;
        stack_dump(node, arguments_of_call);
    }
    if(node->data == nullptr && node->stack_status != STACK_EMPTY && node->stack_status != STACK_IS_CREATED) {
        node->stack_status = STACK_EMPTY;
        stack_dump(node, arguments_of_call);
    }
    if((node->size_stack) > (node->capacity)) {
        node->stack_status = STACK_OVERFLOW;
        stack_dump(node, arguments_of_call);
    }
    if(node->size_stack < 0) {
        node->stack_status = STACK_BAD_SIZE;
        stack_dump(node, arguments_of_call);
    }
    if(node->capacity < 0) {
        node->stack_status = STACK_BAD_CAPACITY;
        stack_dump(node, arguments_of_call);
    }

    IF_HASH_PROTECTION(
    if(node != nullptr && node->data != nullptr && get_hash(node) != node->stack_hash) {
        node->stack_status = STACK_BAD_HASH;
        stack_dump(node, arguments_of_call);
    }
    )

    IF_CANARY_PROTECTION(
    if(node->canary_left[0] != CANARY || node->canary_right[0] != CANARY) {
        node->stack_status = STACK_BAD_CANARY;
        stack_dump(node, arguments_of_call);
    }
    )


    if(node->stack_status != STACK_OK && node->stack_status != STACK_EMPTY && node->stack_status != STACK_IS_CREATED) {
        fell(node);
    }
}
#endif

void fell(Stack_t* node) {
    FILE* log_errors = fopen(node->name_log_file, "a");

    fprintf(log_errors, "I was tired and fell with status %d (%s)...\n", node->stack_status, text_stack_t_status[node->stack_status]);
    printf("I was tired and fell with status %d (%s)...\n", node->stack_status, text_stack_t_status[node->stack_status]);
    fclose(log_errors);

    abort();
}

size_t stack_size(Stack_t* node) {
    return node->size_stack;
}

int stack_capacity(Stack_t* node) {
    IF_DEBUG(stack_err(node, create_struct(__FILE__, __LINE__, __FUNCTION__));)

    return node->capacity;
}

void stack_construct(Stack_t* node, const char* name_log_file) {
    assert(node != nullptr);

    node->data = (Elem_t*)calloc(DIMENSION, sizeof(Elem_t));
    node->size_stack = 0;
    node->capacity = DIMENSION;
    fill_stack_stuff(node);

    node->name_log_file = (char*)calloc(strlen(name_log_file), sizeof(char));
    strcpy(node->name_log_file, name_log_file);

    node->stack_status = STACK_IS_CREATED;
    IF_HASH_PROTECTION(node->stack_hash = get_hash(node);)

    IF_DEBUG(stack_err(node, create_struct(__FILE__, __LINE__, __FUNCTION__));)
}

void stack_destruct(Stack_t* node) {
    IF_DEBUG(stack_err(node, create_struct(__FILE__, __LINE__, __FUNCTION__));)

    free(node->data);
    node->size_stack = POISON;
    node->capacity   = POISON;
    node->data = nullptr;
    node->stack_status = STACK_IS_DECTRUCT;
}

bool stack_is_empty(Stack_t* node) {
    IF_DEBUG(stack_err(node, create_struct(__FILE__, __LINE__, __FUNCTION__));)

    int size_of_stack = stack_size(node);
    if(size_of_stack == 0) {
        node->stack_status = STACK_EMPTY;
    }

    IF_HASH_PROTECTION(node->stack_hash = get_hash(node);)

    return (size_of_stack == 0);
}

void stack_resize(Stack_t* node) {
    IF_DEBUG(stack_err(node, create_struct(__FILE__, __LINE__, __FUNCTION__));)
    assert(node->capacity >= stack_size(node));

    Elem_t* new_data = (Elem_t*)realloc(node->data, stack_capacity(node) * 2 * sizeof(Elem_t) + 1);
    node->capacity *= 2;
    fill_stack_stuff(node);

    assert(new_data != nullptr);
    node->data = new_data;

    IF_HASH_PROTECTION(node->stack_hash = get_hash(node);)

    IF_DEBUG(stack_err(node, create_struct(__FILE__, __LINE__, __FUNCTION__));)
}

void fill_stack_stuff(Stack_t* node) {
    int begin_pos = node->size_stack + 1, new_capacity = node->capacity;
    if(node->stack_status == STACK_IS_CREATED) {
        begin_pos = 0;
    }

    for(int i=begin_pos; i<new_capacity; ++i) {
        node->data[i] = POISON;
    }
}

void stack_push(Stack_t* node, Elem_t value) {
    // IF_DEBUG(stack_err(node, create_struct(__FILE__, __LINE__, __FUNCTION__));)

    if(node->size_stack >= node->capacity) {
        stack_resize(node);
    }

    node->data[node->size_stack] = value;
    ++(node->size_stack);

    if(node->stack_status == STACK_IS_CREATED) {
        node->stack_status = STACK_OK;
    }

    IF_HASH_PROTECTION(node->stack_hash = get_hash(node);)

    IF_DEBUG(stack_err(node, create_struct(__FILE__, __LINE__, __FUNCTION__));)
}

void stack_pop(Stack_t* node) {
    //IF_DEBUG(stack_err(node, create_struct(__FILE__, __LINE__, __FUNCTION__));)

    if(node->stack_status == STACK_EMPTY && stack_is_empty(node)) {
        printf("Don't delete end element from stack\n");
        return;
    }

    node->data[node->size_stack] = POISON;

    if(node->size_stack == 0) {
        node->stack_status = STACK_EMPTY;
    } else {
        --(node->size_stack);
    }

    IF_HASH_PROTECTION(node->stack_hash = get_hash(node);)

    IF_DEBUG(stack_err(node, create_struct(__FILE__, __LINE__, __FUNCTION__));)
}

Elem_t stack_back(Stack_t* node) {
    IF_DEBUG(stack_err(node, create_struct(__FILE__, __LINE__, __FUNCTION__));)

    if(stack_is_empty(node)) {
        printf("Don't take end element from stack\n");
        return POISON;
    }

    return node->data[stack_size(node)-1];
}

void stack_clear(Stack_t* node) {
    IF_DEBUG(stack_err(node, create_struct(__FILE__, __LINE__, __FUNCTION__));)

    free(node->data);
    node->data = nullptr;
    node->capacity = 0;
    node->size_stack = 0;
    node->stack_status = STACK_EMPTY;

    IF_HASH_PROTECTION(node->stack_hash =  0;)
}

void clear_file(const char* file_log_name) {
    FILE* file = fopen(file_log_name, "w");
    fclose(file);
}
