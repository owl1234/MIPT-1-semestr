#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "stack.h"
#include "processor.h"

void file_handler(File file) {
    Stack_t proc_stack = {};
    stack_construct(&proc_stack);

    printf("%s\n", file.name);
    int now_command = -1;
    double now_value = 0.0;
    char string_double_value[MAX_SIZE] = "";

    Elem_t back_element = 0.0, last = 0.0, penultimate = 0.0;

    while(now_command != 0) {
        sscanf(file.text, "%i", &now_command);
        file.text += length_of_number(now_command) + 1; // move_ptr(now_command)

        if(now_command == OPERATION_CODE_HLT) {
            printf("End of work (hlt!)\n");
            break;
        } else if(now_command == OPERATION_CODE_PUSH) {
            sscanf(file.text, "%s", string_double_value);
            file.text += strlen(string_double_value) + 1;

            now_value = string_to_double(string_double_value);
            stack_push(&proc_stack, now_value);
        } else if(now_command == OPERATION_CODE_POP) {
            back_element = stack_back(&proc_stack);
            stack_pop(&proc_stack);
        } else if(now_command == OPERATION_CODE_ADD) {
            last = stack_back(&proc_stack);
            stack_pop(&proc_stack);

            penultimate = stack_back(&proc_stack);
            stack_pop(&proc_stack);

            stack_push(&proc_stack, last + penultimate);
        } else if(now_command == OPERATION_CODE_SUB) {
            last = stack_back(&proc_stack);
            stack_pop(&proc_stack);

            penultimate = stack_back(&proc_stack);
            stack_pop(&proc_stack);

            stack_push(&proc_stack, penultimate - last);
        } else if(now_command == OPERATION_CODE_MUL) {
            last = stack_back(&proc_stack);
            stack_pop(&proc_stack);

            penultimate = stack_back(&proc_stack);
            stack_pop(&proc_stack);

            stack_push(&proc_stack, last * penultimate);
        } else if(now_command == OPERATION_CODE_DIV) {
            last = stack_back(&proc_stack);
            stack_pop(&proc_stack);

            penultimate = stack_back(&proc_stack);
            stack_pop(&proc_stack);

            stack_push(&proc_stack, penultimate / last);
        } else if(now_command == OPERATION_CODE_OUT) {
            printf("%lg\n", stack_back(&proc_stack));
        } else {
            printf("popados (%d) .....  (╯ ° □ °) ╯ (┻━┻) \n", now_command);
            abort();
        }
        //printf("    hellooooooooooooooooooooooooooo\n");
    }
}

int length_of_number(int value) {
    int length = 0;
    if(value == 0) {
        value = 1;
    }

    while(value > 0) {
        value /= 10;
        ++length;
    }

    return length;
}

void move_ptr(char* text, int value) {
    text += length_of_number(value);
}

void move_ptr(char* text, char* value) {
    text += strlen(value);
    //printf("strlen: %d\n", strlen(value));
    //printf("text: %s\n-------------------------------------------------------------\n", text);
}

double string_to_double(char* text) {
    double whole_part = 0;
    double fractional_part = 0;

    int length = strlen(text);
    int position = 0;

    for(; position<length; ++position) {
        if(text[position] == '.') {
            break;
        }

        whole_part += text[position] - 48;
        whole_part *= 10;
    }
    whole_part /= 10;

    if(text[position] == '.') {
        for(int i=length-1; i>position; --i) {
            fractional_part += text[i] - 48;
            fractional_part /= 10;
        }

        whole_part += fractional_part;
    }

    return whole_part;

}
