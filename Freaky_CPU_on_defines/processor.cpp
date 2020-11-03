#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "stack.h"
#include "processor.h"
#include <unistd.h>
#include <math.h>
#include "enums.h"

#define DEFINE_COMMANDS(name, number, args, code)   \
    case COMMAND_##name:                            \
        code;                                       \
        break;

void POPADOS() {
    printf("popados  .....  (╯ ° □ °) ╯ (┻━┻) \n");
    abort();
}

void print_regs(Elem_t* registers_variables, int number_of_register_vars) {
    for(int i=0; i<number_of_register_vars; ++i) {
        printf(identity, registers_variables[i]);
        printf(" ");
    }
    printf("\n");
}

void print_proc(Processor* processor) {
    for(int i=0; i<processor->symbols; ++i)
        printf("%lg ", processor->text[i]);
    printf("\n");
}

void print_stack(Stack_t* stack_) {
    for(int i=0; i<stack_->size_stack; ++i)
        printf("%lg ", stack_->data[i]);
    printf("\n\n");
}

int initialization_proc(Processor* processor) {
    FILE* input_file = fopen("out_asm.lst", "rb");
    int bytes_in_buffer = size_of_buffer(input_file); // bytes !!!

    print_proc(processor);

    processor->symbols = bytes_in_buffer / sizeof(double);
    printf("size_of_text: %d\n", bytes_in_buffer);

    processor->text = (Elem_t*)calloc(bytes_in_buffer, sizeof(Elem_t));
    int status = fread(processor->text, sizeof(double), bytes_in_buffer, input_file);
    for(int i=0; i<bytes_in_buffer/sizeof(double); ++i)
        printf("%lg ", processor->text[i]);
    printf("\n");

    stack_construct(&(processor->proc_stack));
    stack_construct(&(processor->call_stack));
    processor->registers_variables = (Elem_t*)calloc(number_of_register_vars, sizeof(Elem_t));
    processor->ram = (Elem_t*)calloc(MAX_SIZE_RAM, sizeof(Elem_t));
    //init_ram(ram);

    if(status != bytes_in_buffer / sizeof(double)) {
        return status;
    }

    return OK;
}

int size_of_buffer(FILE* file) {
    int length = 0;
    if (!fseek(file, 0, SEEK_END)) {
        length = ftell(file);
    }
    fseek(file, 0, SEEK_SET);

    return length;
}

void file_handler(Processor* processor) {
    int status = check_commands();
    if(status == ERROR_NUMBER) {
        return;
    }

    //int now_command = -1;
    int now_command = OPERATION_CODE_MEOW;
    double now_value = 0.0;

    Elem_t back_element = 0.0, last = 0.0, penultimate = 0.0, input_value = 0.0;
    int flag_of_registers = -1, number_of_register = -1, now_byte = 0;

    Elem_t first_comparison = 0.0, second_comparison = 0.0;
    int number_of_condition = -1;

    //int size_buffer = processor->symbols
    while(now_byte < processor->symbols && now_command != OPERATION_CODE_HLT) {
        now_command = processor->text[now_byte];
        //printf("> now_command: %d (byte: %d) \n", now_command, now_byte);

       switch (now_command) {
            #include "COMMANDS.H"

            default:
                printf("popados (%d) .....  (╯ ° □ °) ╯ (┻━┻) \n", now_command);
                abort();
                break;
        }

        //print_stack(&(processor->proc_stack));
        //print_stack(&(processor->call_stack));
        //sleep(2);
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

/*int length_of_number(int value) {
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
}*/

Elem_t get_value_to_compare(Processor* processor, int* now_byte) {
    int flag_of_register = get_double_from_text(processor, now_byte);
    //printf("get_value_to_compare: %d ", flag_of_register);

    if(flag_of_register == IS_REGISTER) {
        int number_of_register = get_double_from_text(processor, now_byte);
        //printf("(%d)\n", number_of_register);
        return processor->registers_variables[number_of_register];
    } else {
        double x = get_double_from_text(processor, now_byte);
        //printf("(%lg)\n", x);
        return x;
    }
}

int get_double_from_text(Processor* processor, int* now_byte) {
    ++(*now_byte);
    return processor->text[*now_byte];
}

int main() {
    Processor processor = {};
    int status = initialization_proc(&processor);

    if(status != OK) {
        printf("Bad file\n");
        return ERROR_NUMBER;
    }

    file_handler(&processor);
    return 0;
}

#undef DEFINE_COMMANDS
