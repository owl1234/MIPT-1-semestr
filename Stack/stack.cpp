#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "stack.h"

const int dimension = 8;
const long long hash_number = 1000000009;
const char* name_log_file = "log.txt";
const int tabulation_in_probels = 7;
const char* name_file = __FILE__;
const struct call_of_dump base_arguments_of_call = {name_file, -1, " "};

const char* text_stack_t_status[] = {
    "Stack is okey",
    "Stack is error",
    "Stack is empty",
    "Stack is overflow",
    "Capacity is very big",
    "Size is bigger than capacity",
    "Stack created just now",
    HASH_PROTECTION("Bad hash of stack"),
    CANARY_PROTECTION("Canary is killed")
};

HASH_PROTECTION(
int GetHash(Stack_t* node) {
    long long hash_st = 0;
    int size_stack = node->size_stack;
    if(node->stack_status != STACK_EMPTY) {
        for(int i=0; i<=size_stack; ++i) {
            hash_st = (hash_st + node->data[i] % hash_number)  % hash_number;
        }
    } else {
        hash_st = (hash_st + POISON % hash_number)  % hash_number;
    }

    CANARY_PROTECTION(hash_st = (hash_st + node->canary_left[0] % hash_number) % hash_number;)
    CANARY_PROTECTION(hash_st = (hash_st + node->canary_right[0] % hash_number) % hash_number;)

    hash_st = (hash_st + node->capacity     % hash_number) % hash_number;
    hash_st = (hash_st + node->size_stack   % hash_number) % hash_number;
    hash_st = (hash_st + node->stack_status % hash_number) % hash_number;

    return hash_st;
}
)

void PrintElem_T(int value, FILE* file) {
    fprintf(file, "%d", value);
}

void PrintElem_T(double value, FILE* file) {
    fprintf(file, "%g", value);
}

void PrintElem_T(char value, FILE* file) {
    fprintf(file, "%c", value);
}

bool IsCanary(Elem_t value) {
    return (value == CANARY);
}

void ErrorPrintData(Stack_t* node, FILE* file) {
    int stack_size = node->capacity;
    int len_indent = get_len_indent(node->capacity);
    const int base_indent = 2 * tabulation_in_probels;

    if(stack_size == 0) {
        fprintf(file, "\n");
        write_indent(file, base_indent);
        fprintf(file, "Stack is empty\n");
    } else {
        for(int i=0; i<stack_size; ++i) {
            fprintf(file, "\n");
            printf("%d: %d and %d and %d\n", i, len_indent, get_len_indent(i), base_indent);
            write_indent(file, base_indent - (get_len_indent(i) - len_indent));
            if(node->data[i] == NULL) {
                fprintf(file, " [%d] - [%p] - !!!  NULL  !!! \t ( !!! ATTENTION !!! DATA IS NULL !!! )", i, &(node->data[i]));
            } else if(node->data[i] == POISON) {
                fprintf(file, " [%d] - [%p] - !!! POISON !!!", i, &(node->data[i]));
            } else {
                fprintf(file, "*[%d] - [%p] - ", i, &(node->data[i]));
                PrintElem_T(node->data[i], file);
            }
        }
    }
}

int get_len_indent(size_t number) {
    int len = 0;

    if(number == 0) {
        ++len;
    }

    while(number > 0) {
        ++len;
        number /= 10;
    }

    return len;
}

void write_indent(FILE* file, int count_indent) {
    int i = 1;
    for(; tabulation_in_probels * i < count_indent; i *= tabulation_in_probels) {
        printf("%d %d\n", i, count_indent);
        fprintf(file, "\t");
    }
    for(; i <= count_indent; ++i) {
        fprintf(file, " ");
    }
}

void StackDump(Stack_t* node, struct call_of_dump arguments_of_call = base_arguments_of_call) {
    FILE* log_errors = fopen(name_log_file, "a");

    fprintf(log_errors, "Stack_t [%p]\n", node);
    fprintf(log_errors, "File from which the dump is called %s from line %d (called the function %s)\n", arguments_of_call.name_file, arguments_of_call.number_of_line, arguments_of_call.name_function);
    fprintf(log_errors, "{\n");
    fprintf(log_errors, "\tsize_stack = %ld\n", node->size_stack);
    fprintf(log_errors, "\tcapacity   = %ld\n", node->capacity);
    fprintf(log_errors, "\tStatus of stack: %s\n", text_stack_t_status[node->stack_status]);

    CANARY_PROTECTION(
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
    )

    HASH_PROTECTION(fprintf(log_errors, "\tHash: %d\n", node->stack_hash);)
    fprintf(log_errors, "\tdata [%p]\n", node->data);
    fprintf(log_errors, "\t{");
    ErrorPrintData(node, log_errors);
    fprintf(log_errors, "\n\t}");
    fprintf(log_errors, "\n}\n");
    fprintf(log_errors, "\n\n");

    fclose(log_errors);
}
struct call_of_dump create_struct(const char* file_name, int number, const char* function_name) {
    struct call_of_dump tmp = {};
    tmp.name_file = file_name;
    tmp.number_of_line = number;
    tmp.name_function = function_name;
    return tmp;
}

void StackErr(Stack_t* node, struct call_of_dump arguments_of_call = base_arguments_of_call) {
    if(node == nullptr) {
        node->stack_status = STACK_ERROR;
        StackDump(node, create_struct(name_file, __LINE__, __FUNCTION__));
    }
    if(node->data == nullptr && node->stack_status != STACK_EMPTY) {
        node->stack_status = STACK_EMPTY;
        StackDump(node, create_struct(name_file, __LINE__, __FUNCTION__));
    }
    if(node->size_stack > node->capacity) {
        node->stack_status = STACK_OVERFLOW;
        StackDump(node, create_struct(name_file, __LINE__, __FUNCTION__));
    }
    if(node->size_stack > node->capacity) {
        node->stack_status = STACK_SMALL_CAPACITY;
        StackDump(node, create_struct(name_file, __LINE__, __FUNCTION__));
    }

    HASH_PROTECTION(
    if(node != nullptr && node->data != nullptr && GetHash(node) != node->stack_hash) {
        node->stack_status = STACK_BAD_HASH;
        StackDump(node, create_struct(name_file, __LINE__, __FUNCTION__));
    }
    )

    CANARY_PROTECTION(
    if(node->canary_left[0] != CANARY || node->canary_right[0] != CANARY) {
        node->stack_status = STACK_BAD_CANARY;
        StackDump(node, create_struct(name_file, __LINE__, __FUNCTION__));
    }
    )
}

int StackSize(Stack_t* node) {
    return node->size_stack;
}

int StackCapacity(Stack_t* node) {
    StackErr(node, create_struct(name_file, __LINE__, __FUNCTION__));

    return node->capacity;
}

void StackConstruct(Stack_t* node) {
    assert(dimension > 0);
    assert(node != nullptr);

    node->data = (Elem_t*)calloc(dimension, sizeof(Elem_t));
    node->size_stack = 0;
    node->capacity = dimension;
    fill_stack_stuff(node);

    node->stack_status = STACK_IS_CREATED;
    HASH_PROTECTION(node->stack_hash = GetHash(node);)

    StackErr(node, create_struct(name_file, __LINE__, __FUNCTION__));
}

void StackDestruct(Stack_t* node) {
    StackErr(node, create_struct(name_file, __LINE__, __FUNCTION__));

    free(node->data);
    node->size_stack = POISON;
    node->capacity   = POISON;

    free(node);
}

bool StackIsEmpty(Stack_t* node){
    StackErr(node, create_struct(name_file, __LINE__, __FUNCTION__));

    int size_of_stack = StackSize(node);
    if(size_of_stack == 0) {
        node->stack_status = STACK_EMPTY;
    }

    HASH_PROTECTION(node->stack_hash = GetHash(node);)

    return (size_of_stack == 0);
}

void StackResize(Stack_t* node) {
    StackErr(node, create_struct(name_file, __LINE__, __FUNCTION__));
    assert(node->capacity >= StackSize(node));

    Elem_t* new_data = (Elem_t*)realloc(node->data, StackCapacity(node) * 2 * sizeof(Elem_t) + 1);
    node->capacity *= 2;
    fill_stack_stuff(node);

    assert(new_data != nullptr);
    node->data = new_data;

    HASH_PROTECTION(node->stack_hash = GetHash(node);)

    StackErr(node, create_struct(name_file, __LINE__, __FUNCTION__));
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

void StackPush(Stack_t* node, Elem_t value) { //////////////////////////// STACK ELEM T VALUE
    StackErr(node, create_struct(name_file, __LINE__, __FUNCTION__));

    if(StackSize(node) >= StackCapacity(node)) {
        StackResize(node);
    }

    node->data[node->size_stack] = value;
    ++(node->size_stack);

    if(node->stack_status == STACK_IS_CREATED) {
        node->stack_status = STACK_OK;
    }
    HASH_PROTECTION(node->stack_hash = GetHash(node);)

    StackErr(node, create_struct(name_file, __LINE__, __FUNCTION__));
}

void StackPop(Stack_t* node) {
    StackErr(node, create_struct(name_file, __LINE__, __FUNCTION__)); //, {name_file, 280, "StackPop"});

    if(node->stack_status == STACK_EMPTY && StackIsEmpty(node)) {
        printf("Don't delete end element from stack\n");
        return;
    }

    node->data[node->size_stack] = POISON;
    if(node->size_stack == 0) {
        node->stack_status = STACK_EMPTY;
    } else {
        --(node->size_stack);
    }

    HASH_PROTECTION(node->stack_hash = GetHash(node);)

    StackErr(node, create_struct(name_file, __LINE__, __FUNCTION__));
}

Elem_t StackBack(Stack_t* node) {
    StackErr(node, create_struct(name_file, __LINE__, __FUNCTION__));

    if(StackIsEmpty(node)) {
        printf("Don't delete end element from stack\n");
        return POISON;
    }

    return node->data[StackSize(node)];
}

void StackClear(Stack_t* node) {
    StackErr(node, create_struct(name_file, __LINE__, __FUNCTION__));

    free(node->data);
    node->data = nullptr;
    node->capacity = 0;
    node->size_stack = 0;
    node->stack_status = STACK_EMPTY;

    HASH_PROTECTION(node->stack_hash =  0;)
}

void ClearFile(const char* name_log_file) {
    FILE* file = fopen(name_log_file, "w");
    fclose(file);
}

int initialization_stack() {
    ClearFile(name_log_file);

    Stack_t stk = {};
    StackConstruct(&stk);

    //(&stk)->data = nullptr;
    printf("size: %d\n", StackSize(&stk));

    for(int i=0; i<110; ++i) {
        int x = rand() % 69 + 1;
        StackPush(&stk, x);
    }
    StackPush(&stk, NULL);

    StackDump(&stk, create_struct(name_file, __LINE__, __FUNCTION__));
    StackClear(&stk);

    //printf("size: %d\n", StackSize(&stk));
    for(int i=0; i<3; ++i) {
        //printf("Size: %d, back: %d\n", StackSize(&stk), StackBack(&stk));
        //printf("%d\n", (&stk)->size_stack);
        StackPop(&stk);
    }

    StackDump(&stk, create_struct(name_file, __LINE__, __FUNCTION__));
    return 0;
}
