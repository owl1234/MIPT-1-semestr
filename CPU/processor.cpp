#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "stack.h"
#include "processor.h"

// #define connect_strings(first, second) first ## second

void print_regs(Elem_t* registers_variables, int number_of_register_vars) {
    for(int i=0; i<number_of_register_vars; ++i) {
        printf(identity, registers_variables[i]);
        printf(" ");
    }
    printf("\n");
}

void file_handler(File file) {
    printf("file: %s\n", file.name);
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
    int flag_of_registers = -1, number_of_register = -1;

    while(now_command != 0) {
        sscanf(file.text, "%d", &now_command);
        file.text += length_of_number(now_command) + 1;

        if(now_command == OPERATION_CODE_HLT) {
            printf("End of work (hlt!)\n");
            printf(identity, registers_variables[0]);
            printf("\n");

            print_regs(registers_variables, number_of_register_vars);
            break;
        } else if(now_command == OPERATION_CODE_PUSH) {
            proc_push(&file, push_value, &proc_stack, registers_variables);
        } else if(now_command == OPERATION_CODE_POP) {
            proc_pop(&file, &proc_stack, registers_variables);
        } else if(now_command == OPERATION_CODE_ADD) {
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
                printf(identity, stack_back(&proc_stack));
                printf("\n");
            }
        } else if(now_command == OPERATION_CODE_IN) {
            scanf(identity, input_value);
            stack_push(&proc_stack, input_value);
        } else {
            printf("popados (%d) .....  (╯ ° □ °) ╯ (┻━┻) \n", now_command);
            abort();
        }
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

void push_in_registers(int registr, Elem_t* registers_variables, Stack_t* proc_stack) {
    registers_variables[registr] = stack_back(proc_stack);
}

void pop_in_registers(int registr, Elem_t* registers_variables, Stack_t* proc_stack) {
    registers_variables[registr] = stack_back(proc_stack);
    stack_pop(proc_stack);
}

void proc_push(File* file, char* push_value, Stack_t* proc_stack, Elem_t* registers_variables) {
    int flag_of_registers = -1, number_of_register = -1;
    double now_value = 0.0;

    sscanf(file->text, "%d", &flag_of_registers);
    file->text += length_of_number(flag_of_registers) + 1;

    if(flag_of_registers == IS_REGISTER) {
        sscanf(file->text, "%d", &number_of_register);
        file->text += length_of_number(number_of_register) + 1;

        push_in_registers(number_of_register, registers_variables, proc_stack);
    } else {
        sscanf(file->text, "%s", push_value);
        file->text += strlen(push_value) + 1;

        now_value = string_to_double(push_value);
        stack_push(proc_stack, now_value);
    }
}

void proc_pop(File* file, Stack_t* proc_stack, Elem_t* registers_variables) {
    int flag_of_registers = 0, number_of_register = 0;
    double back_element = 0.0;

    sscanf(file->text, "%d", &flag_of_registers);
    file->text += length_of_number(flag_of_registers) + 1;

    if(flag_of_registers == 1) {
        sscanf(file->text, "%d", &number_of_register);
        file->text += length_of_number(number_of_register) + 1;
        pop_in_registers(number_of_register, registers_variables, proc_stack);
    } else {
        back_element = stack_back(proc_stack);
        stack_pop(proc_stack);
    }
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

/*
int main() {
    File input_file = {};

        int status = file_construct(&input_file, name_output_file_ass);
        if(status != OK_FILE) {
            return ERROR_NUMBER;
        }

        file_handler(input_file);
}
*/
