#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "stack.h"
#include "warnings.h"

const int DIMENSION = 8;
const long long HASH_NUMBER = 1000000009;
const int TABULATION_IN_PROBELS = 7;
const char* name_log_file = "log.txt";

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
    "Stack is destructed"
};

#ifdef IF_DEBUG
void stack_err(Stack_t* node, struct call_of_dump arguments_of_call) {
    //printf("stack_err: status: %d, line: %d\n", node->stack_status, arguments_of_call.number_of_line);
    if(node == nullptr) {
        node->stack_status = STACK_ERROR;
    }
    if(node->data == nullptr && node->stack_status != STACK_EMPTY) {
        node->stack_status = STACK_EMPTY;
    }
    if((node->size_stack) > (node->capacity)) {
        node->stack_status = STACK_OVERFLOW;
    }
    if(node->size_stack < 0) {
        node->stack_status = STACK_BAD_SIZE;
    }
    if(node->capacity < 0) {
        node->stack_status = STACK_BAD_CAPACITY;
    }

    if(node->stack_status != STACK_OK && node->stack_status != STACK_EMPTY && node->stack_status != STACK_IS_CREATED) {
        fell(node);
    }
}
#endif

void fell(Stack_t* node) {
    FILE* log_errors = fopen(name_log_file, "a");

    fprintf(log_errors, "I was tired and fell with status %d (%s)...\n", node->stack_status, text_stack_t_status[node->stack_status]);
    printf("I was tired and fell with status %d (%s)...\n", node->stack_status, text_stack_t_status[node->stack_status]);
    fclose(log_errors);

    abort();
}

int stack_size(Stack_t* node) {
    return node->size_stack;
}

int stack_capacity(Stack_t* node) {
    IF_DEBUG(stack_err(node, INFORMATION_ABOUT_CALL);)

    return node->capacity;
}

void stack_construct(Stack_t* node) {
    assert(node != nullptr);

    node->data = (Elem_t*)calloc(DIMENSION, sizeof(Elem_t));
    node->size_stack = 0;
    node->capacity = DIMENSION;
    fill_stack_stuff(node);

    node->stack_status = STACK_IS_CREATED;

    IF_DEBUG(stack_err(node, INFORMATION_ABOUT_CALL);)
}

void stack_destruct(Stack_t* node) {
    IF_DEBUG(stack_err(node, INFORMATION_ABOUT_CALL);)

    free(node->data);
    node->stack_status = STACK_IS_DECTRUCT;
}

bool stack_is_empty(Stack_t* node){
    IF_DEBUG(stack_err(node, INFORMATION_ABOUT_CALL);)

    int size_of_stack = stack_size(node);
    if(size_of_stack == 0) {
        node->stack_status = STACK_EMPTY;
    }

    return (size_of_stack == 0);
}

void stack_resize(Stack_t* node) {
    IF_DEBUG(stack_err(node, INFORMATION_ABOUT_CALL);)
    if(!(node->capacity >= stack_size(node))) {
        warning("Bad capacity into stack", INFORMATION_ABOUT_CALL);
        node->stack_status = STACK_BAD_CAPACITY;
        return;
    }

    Elem_t* new_data = (Elem_t*)realloc(node->data, (node->capacity * 2 + 1) * sizeof(Elem_t));
    if(!new_data) {
        warning(text_stack_t_status[STACK_DATA_NULL], INFORMATION_ABOUT_CALL);
        node->stack_status = STACK_DATA_NULL;
        return;
    }

    node->data = new_data;
    node->capacity *= 2;

    fill_stack_stuff(node);

    IF_DEBUG(stack_err(node, INFORMATION_ABOUT_CALL);)
}

void fill_stack_stuff(Stack_t* node) {
    int begin_pos = node->size_stack, new_capacity = node->capacity;
    if(node->stack_status == STACK_IS_CREATED) {
        begin_pos = 0;
    }

    for(int i=begin_pos; i<new_capacity; ++i) {
        node->data[i].index_into_catalog     = POISON;
        node->data[i].is_it_property_of_node = NOT_IS_PROPERTY;
    }
}

void stack_push(Stack_t* node, size_t value, TYPE_PROPERTY_NODE type_property) {
    IF_DEBUG(stack_err(node, INFORMATION_ABOUT_CALL);)

    if(node->size_stack + 1 >= node->capacity) {
        stack_resize(node);
    }

    node->data[node->size_stack].index_into_catalog = value;
    node->data[node->size_stack].is_it_property_of_node = type_property;
    ++(node->size_stack);

    if(node->stack_status == STACK_IS_CREATED) {
        node->stack_status = STACK_OK;
    }

    IF_DEBUG(stack_err(node, INFORMATION_ABOUT_CALL);)
}

void stack_pop(Stack_t* node) {
    IF_DEBUG(stack_err(node, INFORMATION_ABOUT_CALL);)

    if(stack_is_empty(node) && node->stack_status == STACK_EMPTY) {
        IF_DEBUG(printf("Don't delete end element from stack\n");)
        return;
    }

    node->data[node->size_stack].index_into_catalog = POISON;
    node->data[node->size_stack].is_it_property_of_node = NOT_IS_PROPERTY;

    if(node->size_stack == 0) {
        node->stack_status = STACK_EMPTY;
    } else {
        --(node->size_stack);
    }

    IF_DEBUG(stack_err(node, INFORMATION_ABOUT_CALL);)
}

information_about_node stack_back(Stack_t* node) {
    IF_DEBUG(stack_err(node, INFORMATION_ABOUT_CALL);)

    if(stack_is_empty(node)) {
        IF_DEBUG(printf("Don't delete end element from stack\n");)
        return POISON_STRUCT;
    }

    return node->data[stack_size(node)-1];
}

void stack_clear(Stack_t* node) {
    IF_DEBUG(stack_err(node, INFORMATION_ABOUT_CALL);)

    free(node->data);
    node->data = nullptr;
    node->capacity = 0;
    node->size_stack = 0;
    node->stack_status = STACK_EMPTY;
}

void clear_file(const char* file_log_name) {
    FILE* file = fopen(file_log_name, "w");
    fclose(file);
}

