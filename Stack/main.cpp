#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

const int error_number = 3802;

void help() {
    printf("This program create stack and do some actions with it\n"
            "The required parameters look like this:\n"
                    "[-h] - if you want to read this help again\n"
                    "[-s] - if you want to work with stack\n"
                    "For more information, go here: https://github.com/owl1234/MIPT-1-semestr/tree/master/Stack\n");
}

int initialization_stack();

int main(const int argc, const char* argv[]) {
    const int args_for_stack = 2;

    if(argc == args_for_stack) {
        const char* name_log_file = argv[2];
        if(argv[1][0] == '-' && argv[1][1] == 's' && argc == args_for_stack) {
            return initialization_stack();
        }
        else {
            help();
            return error_number;
        }
    } else {
        help();
        return error_number;
    }

    return 0;
}

int initialization_stack() {
    const char* name_log_file = "log.txt";
    const char* file_name = __FILE__;
    clear_file(name_log_file);

    Stack_t stk = {};
    stack_construct(&stk);
    //stk.size_stack = -1;
    //printf("%d\n", stk.size_stack);

    //(&stk)->data = nullptr;
    printf("size: %d\n", stack_size(&stk));

    for(int i=0; i<35; ++i) {
        int x = rand() % 69 + 1;
        stack_push(&stk, x);
    }
    stack_push(&stk, NULL);

    stack_dump(&stk, create_struct(file_name, __LINE__, __FUNCTION__));
    stack_clear(&stk);

    //printf("size: %d\n", StackSize(&stk));
    for(int i=0; i<3; ++i) {
        //printf("Size: %d, back: %d\n", StackSize(&stk), StackBack(&stk));
        //printf("%d\n", (&stk)->size_stack);
        stack_pop(&stk);
    }

    stack_dump(&stk, create_struct(file_name, __LINE__, __FUNCTION__));
    return 0;
}
