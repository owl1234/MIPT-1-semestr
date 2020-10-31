#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "stack.h"
#include "processor.h"
#include <unistd.h>
#include <math.h>

// #define connect_strings(first, second) first ## second

void print_regs(Elem_t* registers_variables, int number_of_register_vars) {
    for(int i=0; i<number_of_register_vars; ++i) {
        printf(identity, registers_variables[i]);
        printf(" ");
    }
    printf("\n");
}

void print_stack(Stack_t* stack_) {
    for(int i=0; i<=stack_->size_stack; ++i)
        printf("%lg ", stack_->data[i]);
    printf("\n\n");
}

void file_handler(File file) {
    printf("Input file: %s\n", file.name);
    int status = check_commands();
    if(status == ERROR_NUMBER) {
        return;
    }

    Stack_t proc_stack = {};
    stack_construct(&proc_stack);

    Stack_t call_stack = {};
    stack_construct(&call_stack);

    Elem_t* registers_variables = (Elem_t*)calloc(number_of_register_vars, sizeof(Elem_t));

    int now_command = -1;
    double now_value = 0.0;

    Elem_t back_element = 0.0, last = 0.0, penultimate = 0.0, input_value = 0.0;
    int flag_of_registers = -1, number_of_register = -1, now_byte = 0;
    Elem_t first_comparison = 0.0, second_comparison = 0.0;
    int number_of_condition = -1;

    while(now_command != 0) {
        sscanf(file.text, "%d", &now_command);
        file.text += 9; //length_of_number(now_command) + 1;

        //printf("> now_command: %d (byte: %d) \n", now_command, now_byte);

        if(now_command == OPERATION_CODE_HLT) {
            printf("End of work (hlt!)\n");
            break;
        } else if(now_command == OPERATION_CODE_PUSH) {
            proc_push(&file, registers_variables, &now_byte, &proc_stack);
        } else if(now_command == OPERATION_CODE_POP) {
            proc_pop(&file, &proc_stack, &now_byte, registers_variables);
        } else if(now_command == OPERATION_CODE_ADD) {
            proc_arifmetics(&proc_stack, OPERATION_CODE_ADD);
        } else if(now_command == OPERATION_CODE_SUB) {
            proc_arifmetics(&proc_stack, OPERATION_CODE_SUB);
        } else if(now_command == OPERATION_CODE_MUL) {
            proc_arifmetics(&proc_stack, OPERATION_CODE_MUL);
        } else if(now_command == OPERATION_CODE_SQRT) {
            proc_arifmetics(&proc_stack, OPERATION_CODE_SQRT);
        } else if(now_command == OPERATION_CODE_DIV) {
            proc_arifmetics(&proc_stack, OPERATION_CODE_DIV);
        } else if(now_command == OPERATION_CODE_OUT) {
            if(proc_stack.size_stack == 0) {
                printf("Stack is empty\n");
            } else {
                //print_stack(&proc_stack);
                printf("Answer: (size = %d) ", proc_stack.size_stack);
                printf(identity, stack_back(&proc_stack));
                printf("\n\n");
            }
        } else if(now_command == OPERATION_CODE_IN) {
            scanf(identity, &input_value);
            stack_push(&proc_stack, input_value);
            //print_stack(&proc_stack);
        } else if(now_command == OPERATION_CODE_CMP) {
            proc_cmp(&file, &now_byte, &first_comparison, &second_comparison, registers_variables);
        } else if(now_command == OPERATION_CODE_CALL || now_command == OPERATION_CODE_JMP) {
            proc_jmp_and_call(&file, &now_byte, &call_stack, now_command);
        } else if(now_command == OPERATION_CODE_RET) {
            proc_ret(&file, &now_byte, &call_stack);
        } else if(is_condition(now_command, &number_of_condition)) {                                      // JE JNE JA JAE JB JBE
            //printf("cmp: first: %lg, second: %lg (now_b: %d, command: %d)\n", first_comparison, second_comparison, now_byte, now_command);
            proc_conditions(&file, number_of_condition, first_comparison, second_comparison, &now_byte, &call_stack);
        } else if(now_command == OPERATION_CODE_MEOW) {
            proc_meow(&file, &now_byte);
        } else {
            printf("popados (%d) .....  (╯ ° □ °) ╯ (┻━┻) \n", now_command);
            abort();
        }

        ++now_byte;
        //print_regs(registers_variables, number_of_register_vars);
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

    int length = 8; //strlen(text);
    int is_negative = 1;
    int position = 0;

    while(text[position] == '0') {
        ++position;
    }

    if(text[position] == '-') {
        ++position;
        is_negative = -1;
    }

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

    whole_part *= is_negative;

    return whole_part;
}

void push_in_registers(File* file, Elem_t* registers_variables, int* now_byte, Stack_t* proc_stack) {
    //char number[MAX_SIZE] = "";
    int number = get_int_from_text(file, now_byte);
    //registers_variables[number] = stack_back(proc_stack);
    //printf("(push) number of reg: %d, value: %d\n", number, registers_variables[number]);
    stack_push(proc_stack, registers_variables[number]);
}

void pop_in_registers(int registr, Elem_t* registers_variables, Stack_t* proc_stack) {
    registers_variables[registr] = stack_back(proc_stack);
    //printf("(pop) number of reg: %d, value: %lg\n", registr, registers_variables[registr]);
    stack_pop(proc_stack);
}

void proc_push(File* file, Elem_t* registers_variables, int* now_byte, Stack_t* proc_stack) {
    Elem_t now_value = 0.0;

    int flag_of_registers = get_int_from_text(file, now_byte);

    if(flag_of_registers == IS_REGISTER) {
        push_in_registers(file, registers_variables, now_byte, proc_stack);
        //print_stack(proc_stack);
    } else {
        //char* push_value = (char*)calloc(MAX_SIZE, sizeof(char));
        char* push_value = (char*)calloc(MAX_SIZE, sizeof(char));
        push_value = get_char_from_text(file, now_byte);
        //printf("push_val: %s ", push_value);
        now_value = string_to_double(push_value);
        //printf("(%lg)\n", now_value);
        stack_push(proc_stack, now_value);
    }
    //printf("push: now size %d\n", proc_stack->size_stack);
}

void proc_pop(File* file, Stack_t* proc_stack, int* now_byte, Elem_t* registers_variables) {
    double back_element = 0.0;

    int flag_of_registers = get_int_from_text(file, now_byte);
    //printf("flag_of_reg: %d\n", flag_of_registers);

    if(flag_of_registers == NOT_ARGS) {
        back_element = stack_back(proc_stack);
        stack_pop(proc_stack);
    } else {
        int number_of_register = get_int_from_text(file, now_byte);
        pop_in_registers(number_of_register, registers_variables, proc_stack);
        //abort();
    }
    //printf("pop: now size %d\n", proc_stack->size_stack);
}

void proc_arifmetics(Stack_t* proc_stack, int operation_code) {
    double last = stack_back(proc_stack);
    stack_pop(proc_stack);

    if(operation_code == OPERATION_CODE_SQRT) {
        stack_push(proc_stack, sqrt(last));
    } else {
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
}

void proc_jmp_and_call(File* file, int* now_byte, Stack_t* call_stack, int code_of_command) {
    int address_label = get_int_from_text(file, now_byte), number_of_condition = 0;

    //printf("!!! JUMP AND CALL !!! %d\n", address_label);
    if(address_label == OPERATION_CODE_RECURSIVE_RET) {
        proc_ret(file, now_byte, call_stack);
        //printf("recret !!!!!!!!!!!!!!!!!!!!1\n");
        return;
    }
    //printf("add_label: %d\n", address_label);
    //printf("code_of_command: %d\n", code_of_command);

    if(code_of_command == OPERATION_CODE_JE || code_of_command == OPERATION_CODE_CALL) { // is_condition(code_of_command, &number_of_condition)
        stack_push(call_stack, *now_byte);
        //printf("call: %d (size: %d) \n", *now_byte, call_stack->size_stack);
    }
    //printf("labeL: %d, now_byte: %d\n", address_label, *now_byte);
    *now_byte = 0;

    file->text = file->copy_of_text;

    while(*now_byte < address_label) {
        while(*(file->text) != ' ' && *(file->text) != '\n')
            ++(file->text);
        ++(file->text);
        ++(*now_byte);
    }


    //printf("now_byte: %d\n", *now_byte);


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

void proc_ret(File* file, int* now_byte, Stack_t* call_stack) {
    int go_to_address = stack_back(call_stack);
    //printf("BACK CALL STACK: %d\n", go_to_address);


    //printf("go_to_add: %d (size: %d)\n", go_to_address, call_stack->size_stack);
    if(call_stack->size_stack != 0) {
        *now_byte = 0;

        file->text = file->copy_of_text;

        while(*now_byte < go_to_address) {
            while(*(file->text) != ' ' && *(file->text) != '\n')
                ++(file->text);
            ++(file->text);
            ++(*now_byte);
        }

        stack_pop(call_stack);

        //printf("size of call_stack: %d\n", call_stack->size_stack);
        //printf("Ret to %d address\n", go_to_address);
    } else {
        //printf("I DON'T REEEEEEEEEEEEEEEEEEET\n");
    }
}

void proc_cmp(File* file, int* now_byte, Elem_t* first_comparison, Elem_t* second_comparison, Elem_t* register_variables) {

    //printf("proc_cmp:\n\t");
    *first_comparison  = get_value_to_compare(file, now_byte, register_variables);

    *second_comparison = get_value_to_compare(file, now_byte, register_variables);
    //printf("%lg %lg\n", *first_comparison, *second_comparison);
}

Elem_t get_value_to_compare(File* file, int* now_byte, Elem_t* register_variables) {
    int flag_of_register = get_int_from_text(file, now_byte);

    char* temp = (char*)calloc(MAX_SIZE, sizeof(char));

    if(flag_of_register == IS_REGISTER) {
        int number_of_register = get_int_from_text(file, now_byte);
        return register_variables[number_of_register];
    } else {
        temp = get_char_from_text(file, now_byte);
        return string_to_double(temp);
    }
}

bool is_condition(int now_command, int* number_of_condition) {
    int command_is_condition = true;
    switch(now_command) {
        case OPERATION_CODE_JE:
            *number_of_condition = OPERATION_CODE_JE;
            break;
        case OPERATION_CODE_JNE:
            *number_of_condition = OPERATION_CODE_JNE;
            break;
        case OPERATION_CODE_JA:
            *number_of_condition = OPERATION_CODE_JA;
            break;
        case OPERATION_CODE_JAE:
            *number_of_condition = OPERATION_CODE_JAE;
            break;
        case OPERATION_CODE_JB:
            *number_of_condition = OPERATION_CODE_JB;
            break;
        case OPERATION_CODE_JBE:
            *number_of_condition = OPERATION_CODE_JBE;
            break;
        default:
            *number_of_condition = -1;
            command_is_condition = false;
    }

    //printf("is_condition: %d\n", *number_of_condition);

    return command_is_condition;
}

int get_int_from_text(File* file, int* now_byte) {
    int value = 0;
    sscanf(file->text, "%d", &value);
    //printf("vslue: %d\n", value);

    file->text += 9; //length_of_number(value) + 1;
    ++(*now_byte);

    return value;
}

char* get_char_from_text(File* file, int* now_byte) {
    char* temp = (char*)calloc(MAX_SIZE, sizeof(char));
    sscanf(file->text, "%s", temp);

    file->text += strlen(temp) + 1;
    ++(*now_byte);

    return temp;
}

void proc_conditions(File* file, int number_of_condition, Elem_t first_comparison, Elem_t second_comparison, int* now_byte, Stack_t* call_stack) {
    if(number_of_condition == OPERATION_CODE_JE  && first_comparison == second_comparison ||
       number_of_condition == OPERATION_CODE_JNE && first_comparison != second_comparison ||
       number_of_condition == OPERATION_CODE_JA  && first_comparison >  second_comparison ||
       number_of_condition == OPERATION_CODE_JAE && first_comparison >= second_comparison ||
       number_of_condition == OPERATION_CODE_JB  && first_comparison <  second_comparison ||
       number_of_condition == OPERATION_CODE_JBE && first_comparison <= second_comparison) {
        //printf("HERE\n");
        proc_jmp_and_call(file, now_byte, call_stack, OPERATION_CODE_CALL);
    } else {
        get_int_from_text(file, now_byte);
    }
}

void proc_meow(File* file, int* now_byte) {
    printf("meow\n");
}

int main() {
    File input_file = {};

    int status = file_construct(&input_file, name_output_file_ass);
    printf("name: %s\n", name_output_file_ass);
    if(status != OK_FILE) {
        return ERROR_NUMBER;
    }

    printf("%s\n", input_file.copy_of_text);

    file_handler(input_file);
    return 0;
}

