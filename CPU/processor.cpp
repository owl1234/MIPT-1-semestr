#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "stack.h"
#include "processor.h"
#include <unistd.h>

// #define connect_strings(first, second) first ## second

void print_regs(Elem_t* registers_variables, int number_of_register_vars) {
    for(int i=0; i<number_of_register_vars; ++i) {
        printf(identity, registers_variables[i]);
        printf(" ");
    }
    printf("\n");
}

void file_handler(File file) {
    printf("Input file: %s\n", file.name);
    int status = check_commands();
    if(status == ERROR_NUMBER) {
        return;
    }

    Stack_t proc_stack = {};
    stack_construct(&proc_stack);

    Elem_t* registers_variables = (Elem_t*)calloc(number_of_register_vars, sizeof(Elem_t));

    int now_command = -1;
    double now_value = 0.0;
    char push_value[MAX_SIZE] = "";

    Elem_t back_element = 0.0, last = 0.0, penultimate = 0.0, input_value = 0.0;
    int flag_of_registers = -1, number_of_register = -1, now_byte = 0;

    while(now_command != 0) {
        sscanf(file.text, "%d", &now_command);
        sleep(1);
        printf("now_command: %d, now_byte:  %d\n", now_command, now_byte);
        //index_in_text += length_of_number(now_command) + 1;
        file.text += length_of_number(now_command) + 1;

        if(now_command == OPERATION_CODE_HLT) {
            printf("End of work (hlt!)\n");
            break;
        } else if(now_command == OPERATION_CODE_PUSH) {
            proc_push(&file, push_value, &now_byte, &proc_stack);
        } else if(now_command == OPERATION_CODE_PUSH_IN_REGISTR) {
            push_in_registers(&file, OPERATION_CODE_PUSH_IN_REGISTR, registers_variables, &now_byte, &proc_stack);
        } else if(now_command == OPERATION_CODE_POP) {
            proc_pop(&file, &proc_stack);
        } else if(now_command / 10 == OPERATION_CODE_POP && now_command % 10 < number_of_register) {
            pop_in_registers(now_command % 10, registers_variables, &proc_stack);
        } else if(now_command == OPERATION_CODE_ADD) {
            printf("Add\n");
            proc_arifmetics(&proc_stack, OPERATION_CODE_ADD);
        } else if(now_command == OPERATION_CODE_SUB) {
            proc_arifmetics(&proc_stack, OPERATION_CODE_SUB);
        } else if(now_command == OPERATION_CODE_MUL) {
            proc_arifmetics(&proc_stack, OPERATION_CODE_MUL);
        } else if(now_command == OPERATION_CODE_DIV) {
            proc_arifmetics(&proc_stack, OPERATION_CODE_DIV);
        } else if(now_command == OPERATION_CODE_OUT) {
            if(proc_stack.size_stack == 0) {
                printf("Stack is empty\n");
            } else {
                printf("Answer: ");
                printf(identity, stack_back(&proc_stack));
                printf("\n");
            }
        } else if(now_command == OPERATION_CODE_IN) {
            scanf(identity, &input_value);
            stack_push(&proc_stack, input_value);
        } else if(now_command == OPERATION_CODE_JMP) {
            proc_jmp(&file, &now_byte);
        } else {
            printf("popados (%d) .....  (╯ ° □ °) ╯ (┻━┻) \n", now_command);
            abort();
        }

        ++now_byte;
    }
}

int check_commands() {
    if(sizeof(NUMBER_ARGUMENTS_FOR_OPERATION) == sizeof(LENGTH_OF_TEXT_OPERATION) &&
       sizeof(NUMBER_ARGUMENTS_FOR_OPERATION) / sizeof(NUMBER_ARGUMENTS_FOR_OPERATION[0]) == number_of_commands) {
        return OK;
    }

    printf("The command system is outdated!\n");
    return ERROR_NUMBER;
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

void push_in_registers(File* file, int code_operation, Elem_t* registers_variables, int* now_byte, Stack_t* proc_stack) {
    char number[MAX_SIZE] = "";
    sscanf(file->text, "%s", number);

    double registr = string_to_double(number);
    ++(*now_byte);
    file->text += strlen(number);
    registers_variables[code_operation % 10] = registr;
    //stack_push(proc_stack, registers_variables[registr]);
}

void pop_in_registers(int registr, Elem_t* registers_variables, Stack_t* proc_stack) {
    registers_variables[registr] = stack_back(proc_stack);
    stack_pop(proc_stack);
}

void proc_push(File* file, char* push_value, int* now_byte, Stack_t* proc_stack) {
    int flag_of_registers = -1, number_of_register = -1;
    double now_value = 0.0;

    sscanf(file->text, "%s", push_value);
    ++(*now_byte);
    file->text += strlen(push_value) + 1;

    now_value = string_to_double(push_value);
    stack_push(proc_stack, now_value);
    printf("push_value: %lg\n", now_value);
}

void proc_pop(File* file, Stack_t* proc_stack) {
    double back_element = 0.0;

    back_element = stack_back(proc_stack);
    stack_pop(proc_stack);
}

void proc_arifmetics(Stack_t* proc_stack, int operation_code) {
    double last = stack_back(proc_stack);
    stack_pop(proc_stack);

    double penultimate = stack_back(proc_stack);
    stack_pop(proc_stack);

    if(operation_code == OPERATION_CODE_ADD) {
        stack_push(proc_stack, last + penultimate);
    } else if(operation_code == OPERATION_CODE_SUB) {
        stack_push(proc_stack, penultimate - last);
    } else if(operation_code == OPERATION_CODE_MUL) {
        stack_push(proc_stack, last * penultimate);
    } else if(operation_code == OPERATION_CODE_DIV) {
        stack_push(proc_stack, penultimate / last);
    }
}

void proc_jmp(File* file, int* now_byte) {
    int address_label = 0;
    sscanf(file->text, "%d", &address_label);

    ++(*now_byte);

    //printf("labeL: %d, now_byte: %d\n", address_label, *now_byte);
    *now_byte = 0;

    file->text = file->copy_of_text;

    while(*now_byte < address_label) {
        while(*(file->text) != ' ' && *(file->text) != '\n')
            ++(file->text);
        ++(file->text);
        ++(*now_byte);
    }

    /*while(*now_byte > address_label) {
        printf("index_text: %d, text: %s, now_symbol: %c\n", *now_byte, file->text, *(file->text));
        while(*(file->text) != ' ' && *(file->text) != '\n')
            --(file->text);
        --(file->text);
        --(*now_byte);
    }
    --(file->text);*/
    //printf("text: %s\n\n", file->text);
}

/*
int main() {
    File input_file = {};

    int status = file_construct(&input_file, name_output_file_ass);
    printf("ame: %s\n", name_output_file_ass);
    if(status != OK_FILE) {
        return ERROR_NUMBER;
    }

    printf("%s\n", input_file.copy_of_text);

    file_handler(input_file);
    return 0;
}
*/
