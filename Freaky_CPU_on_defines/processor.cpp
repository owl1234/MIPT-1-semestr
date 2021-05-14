/**
 *  @file
 *  @author Kolesnikova Xenia <heiduk.k.k.s@yandex.ru>
 *  @par Last edition
 *                  May 14, 2021, 14:25:25
 *  @par What was changed?
 *                      1. All... ha-ha-ha. To be honest, it not well code,
                           so I have to rewrite part of it
                        2. For example, print assembly code into binary was changed 
                        3. Double was killed, int is real power
 *  @par To-do list
 *                      
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include "common.h"
#include "stack.h"
#include "processor.h"
#include "enums.h"

#define DEFINE_COMMANDS(name, number, args, code_processor, code_disassembler, code_assembler)   \
    case COMMAND_##name: {                                                                       \
        code_processor;                                                                          \
        break;                                                                                   \
    }


/*void POPADOS() {
    printf("popados  .....  (╯ ° □ °) ╯ (┻━┻) \n");
    abort();
}*/


void help() {
    printf("That is my realisarion of softprocessor.\n"
           "The required parameters look like this:\n"
                    "[-h]                                                         - if you want to read this help again\n"
                    "[-p name_input_file_to_processor name_log_file_to_processor] - if you want to follow instructions\n"
                    "For more information, go here: https://github.com/owl1234/MIPT_projects_1_sem/tree/master/Freaky_CPU_on_defines\n");
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
        printf("%d ", processor->text[i]);
    printf("\n");
}

void print_stack(Stack_t* stack_) {
    for(size_t i=0; i<stack_->size_stack; ++i)
        printf("%d ", stack_->data[i]);
    printf("\n\n");
}

void processor_dump(Processor* processor, struct call_of_dump arguments_of_call = proc_base_arguments_of_call) {
    printf("%s\n", processor->name_log_file);
    //FILE* log_errors = fopen(processor->name_log_file, "a");

    /*fprintf(log_errors, "Processor [%p], status is %s\n", processor, TEXT_PROCESSOR_ERRORS[processor->status]);
    fprintf(log_errors, "File from which the dump is called %s from line %d (called the function %s)\n", arguments_of_call.name_file, arguments_of_call.number_of_line, arguments_of_call.name_function);
    fprintf(log_errors, "{\n");
    fprintf(log_errors, "\tsymbols = %d\n", processor->symbols);

    
    fprintf(log_errors, "\tproc_stack_size = %lu\n", processor->proc_stack.size_stack);
    fprintf(log_errors, "\tproc_capacity = %lu\n", processor->proc_stack.capacity);
    fprintf(log_errors, "\tproc_stack [%p]\n", &(processor->proc_stack));
    fprintf(log_errors, "\t{");
    error_print_data(&(processor->proc_stack), log_errors);
    fprintf(log_errors, "\n\t}\n");

    fprintf(log_errors, "\tcall_stack_size = %lu\n", processor->call_stack.size_stack);
    fprintf(log_errors, "\tcall_capacity = %lu\n", processor->call_stack.capacity);
    fprintf(log_errors, "\tcall_stack [%p]\n", &(processor->call_stack));
    fprintf(log_errors, "\t{");
    error_print_data(&(processor->call_stack), log_errors);
    fprintf(log_errors, "\n\t}\n"); 

    fprintf(log_errors, "\tregisters_variables [%p]\n", processor->registers_variables);
    fprintf(log_errors, "\t{");
    error_print_array_elem_t(processor->registers_variables, number_of_register_vars, log_errors);
    fprintf(log_errors, "\n\t}\n");

    fprintf(log_errors, "\tram [%p]\n", processor->ram);
    fprintf(log_errors, "\t{");
    error_print_array_elem_t(processor->ram, MAX_SIZE_RAM, log_errors);
    fprintf(log_errors, "\n\t}\n");

    fprintf(log_errors, "\n}\n");
    fprintf(log_errors, "\n\n");*

    fclose(log_errors);*/
}

void error_print_data(Elem_t* array_elem_t, const int length, FILE* file) {
    for(int i=0; i<length; ++i) {
        fprintf(file, identity, array_elem_t[i]);
        fprintf(file, " ");
    }
}

void processor_verificator(Processor* processor, struct call_of_dump arguments_of_call = proc_base_arguments_of_call) {
    if(processor == NULL) {
        processor->status = PROC_BAD_PTR_TO_PROC;
        processor_dump(processor, arguments_of_call);
    }

    stack_err(&(processor->proc_stack), arguments_of_call);
    stack_err(&(processor->call_stack), arguments_of_call);
}
)

void init_ram(Processor* processor) {
    srand(time(NULL));

    for(int i=0; i<MAX_SIZE; ++i) {
        processor->ram[i] = rand() % 20;
    }
}

int initialization_proc(Processor* processor, const char* name_input_file, const char* name_log_file) {
    FILE* input_file = fopen(name_input_file, "rb");
    if(input_file == NULL) {
        return PROC_BAD_FILE;
    }

    int bytes_in_buffer = size_of_buffer(input_file);

    processor->symbols = bytes_in_buffer / sizeof(char);

    processor->text = (char*)calloc(bytes_in_buffer, sizeof(char));
    int status = fread(processor->text, sizeof(char), bytes_in_buffer, input_file);

    stack_construct(&(processor->proc_stack), name_log_file);
    stack_construct(&(processor->call_stack), name_log_file);
    processor->registers_variables = (Elem_t*)calloc(number_of_register_vars, sizeof(Elem_t));

    processor->name_log_file = (char*)calloc(strlen(name_log_file), sizeof(char));
    strcpy(processor->name_log_file, name_log_file);
    processor->ram = (Elem_t*)calloc(MAX_SIZE_RAM, sizeof(Elem_t));
    init_ram(processor);

    if(status != (int)(bytes_in_buffer / sizeof(char))) {
        processor->status = PROC_BAD_READ_FROM_FILE;
        return PROC_BAD_READ_FROM_FILE;
    }

    processor->status = PROC_OKEY;

    IF_DEBUG(processor_verificator(processor, create_struct(__FILE__, __LINE__, __FUNCTION__));)

    return PROC_OKEY;
}

PROCESSOR_ERRORS processing(Processor* processor) {
    assert(processor != NULL);

    int now_command = OPERATION_CODE_MEOW;

    Elem_t back_element = 0.0, last = 0.0, penultimate = 0.0, input_value = 0.0, now_value = 0;
    int flag_of_registers = -1, number_of_register = -1, now_byte = 0, ram_index = 0;

    PROCESSOR_ERRORS status = check_signature(processor, &now_byte);
    if(status != PROC_OKEY) {
        return status;
    }

    Elem_t first_comparison = 0.0, second_comparison = 0.0;
    int number_of_condition = -1;

    while(now_byte < processor->symbols && now_command != OPERATION_CODE_HLT) {
        now_command = processor->text[now_byte];
        for(int i = 0; i < processor->symbols; ++i) {
            if(i == now_byte)
                printf("!!!");
            printf("%d ", processor->text[i]);
        }
        printf("\n");
        printf("\nnow byte: %d, symbols: %d\n", now_byte, processor->symbols);
        //IF_DEBUG(printf("> now_command: %d (byte: %d) \n", now_command, now_byte);)

       switch (now_command) {
            #include "COMMANDS.H"

            default:
                printf("Bad command %d, byte: %d .....  (╯ ° □ °) ╯ (┻━┻) \n", now_command, now_byte);
                return PROC_UNKNOWN_COMMAND;
        }
        ++now_byte;
        IF_DEBUG(processor_verificator(processor, create_struct(__FILE__, __LINE__, __FUNCTION__));)
    }

    printf("end now byte: %d, symbols: %d\n", now_byte, processor->symbols);

    return PROC_OKEY;
}

PROCESSOR_ERRORS check_signature(Processor* processor, int* now_byte) {

    if(processor->text[(*now_byte)++] > VERSION) {
        return PROC_BAD_VERSION;
    }

    int ans = get_long_long_number_from_binary(processor, now_byte);
    printf("\t\t%d\n", ans);
    if(ans != SIGNATURE_NAME_HASH)
        return PROC_BAD_VERSION;

    printf("Okey signature\n");

    return PROC_OKEY;
}

void destruct_processor(Processor* processor) {
    free(processor->ram);
    free(processor->text);
    stack_destruct(&(processor->call_stack));
    stack_destruct(&(processor->proc_stack));
}

Elem_t get_value_to_compare(Processor* processor, int* now_byte) {
    int flag_of_register = get_byte_from_text(processor, now_byte);

    if(flag_of_register == IS_REGISTER) {
        int number_of_register = get_byte_from_text(processor, now_byte);
        return processor->registers_variables[number_of_register];
    } else {
        return get_byte_from_text(processor, now_byte);
    }
}

Elem_t get_byte_from_text(Processor* processor, int* now_byte) {
    ++(*now_byte);
    return processor->text[*now_byte];
}

Elem_t get_double_from_text(Processor* processor, int* now_byte) {
    ++(*now_byte);
    return processor->text[*now_byte];
}

inline long long get_long_long_number_from_binary(Processor* processor, int* now_byte) {
    long long result_number = 0ll;

    char* ptr_to_number = processor->text + (*now_byte);

    for(int i = 0; i < processor->symbols; ++i) {
        if(i == *now_byte)
            printf("!!!");
        printf("%d ", processor->text[i]);
    }
    printf("\n");

    for(int i = SIZE_BYTE - 1; i >= 0; --i) {
        long long byte = ptr_to_number[i];
        if(byte < 0 && i != SIZE_BYTE - 1)
            byte += (1 << SIZE_BYTE);

        result_number += byte;

        if(i > 0) {
            result_number <<= SIZE_BYTE;
        }
    }
    (*now_byte) += BYTE_IN_ELEM_TYPE;

    return result_number;
}

#undef DEFINE_COMMANDS

int main(const int argc, const char* argv[]) {
    int status = ERROR_NUMBER;

    if(argc == 3) {
        Processor processor = {};
        printf("begin processing\n");

        status = initialization_proc(&processor, argv[1], argv[2]);
        for(int i=0; i<processor.symbols; ++i)
            printf("%x", processor.text[i]);
        printf("\n");

        processor_dump(&processor, create_struct(__FILE__, __LINE__, __FUNCTION__));
        if(status != OK) {
            printf("%s\n", TEXT_PROCESSOR_ERRORS[status]);
            return status;
        }

        status = processing(&processor);
        printf("%s\n", TEXT_PROCESSOR_ERRORS[status]);

        destruct_processor(&processor);
    } else {
        help();
    }

    return status;
}
