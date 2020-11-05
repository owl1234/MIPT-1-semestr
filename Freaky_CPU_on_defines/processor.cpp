#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>
#include "common.h"
#include "stack.h"
#include "processor.h"
#include "enums.h"

#define DEFINE_COMMANDS(name, number, args, code)   \
    case COMMAND_##name:                            \
        code;                                       \
        break;

#define IF_DEBUG(code)

void POPADOS() {
    printf("popados  .....  (╯ ° □ °) ╯ (┻━┻) \n");
    abort();
}

IF_DEBUG(
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
)

int initialization_proc(Processor* processor, const char* name_input_file) {
    FILE* input_file = fopen(name_input_file, "rb");
    int bytes_in_buffer = size_of_buffer(input_file); // bytes !!!

    processor->symbols = bytes_in_buffer / sizeof(double);

    processor->text = (Elem_t*)calloc(bytes_in_buffer, sizeof(Elem_t));
    int status = fread(processor->text, sizeof(double), bytes_in_buffer, input_file);

    stack_construct(&(processor->proc_stack));
    stack_construct(&(processor->call_stack));
    processor->registers_variables = (Elem_t*)calloc(number_of_register_vars, sizeof(Elem_t));
    processor->ram = (Elem_t*)calloc(MAX_SIZE_RAM, sizeof(Elem_t));
    //init_ram(ram);

    if(status != bytes_in_buffer / sizeof(double)) {
        return ERROR_NUMBER;
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
    assert(processor != nullptr);

    int now_command = OPERATION_CODE_MEOW;
    double now_value = 0.0;

    Elem_t back_element = 0.0, last = 0.0, penultimate = 0.0, input_value = 0.0;
    int flag_of_registers = -1, number_of_register = -1, now_byte = 0;

    Elem_t first_comparison = 0.0, second_comparison = 0.0;
    int number_of_condition = -1;

    while(now_byte < processor->symbols && now_command != OPERATION_CODE_HLT) {
        now_command = processor->text[now_byte];
        IF_DEBUG(printf("> now_command: %d (byte: %d) \n", now_command, now_byte);)

       switch (now_command) {
            #include "COMMANDS.H"

            default:
                ERROR("popados (bad command) .....  (╯ ° □ °) ╯ (┻━┻) \n");
        }
        ++now_byte;
    }
}

/*int check_commands() {
    if(sizeof(NUMBER_ARGUMENTS_FOR_OPERATION) == sizeof(LENGTH_OF_TEXT_OPERATION) &&
       sizeof(NUMBER_ARGUMENTS_FOR_OPERATION) == number_of_commands) {
        return OK;
    }

    printf("The command system is outdated! (%d %d %d)\n", sizeof(NUMBER_ARGUMENTS_FOR_OPERATION), sizeof(LENGTH_OF_TEXT_OPERATION), number_of_commands);
    return ERROR_NUMBER;
}*/

Elem_t get_value_to_compare(Processor* processor, int* now_byte) {
    int flag_of_register = get_double_from_text(processor, now_byte);

    if(flag_of_register == IS_REGISTER) {
        int number_of_register = get_double_from_text(processor, now_byte);
        return processor->registers_variables[number_of_register];
    } else {
        return get_double_from_text(processor, now_byte);
    }
}

int get_double_from_text(Processor* processor, int* now_byte) {
    ++(*now_byte);
    return processor->text[*now_byte];
}

#undef DEFINE_COMMANDS
