#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "assembler.h"
#include "common.h"

void assembling_file(File* input_file, const char* name_output_file) {
    /*File input_file = {};
    printf("!!! %d\n", number_of_commands);
    int status = file_construct(&input_file, name_input_file_ass);
    if(status != OK) {
        printf("PANIC (%d).....\n", status);
        return;
    }*/

    printf("Start to assembling file.........................................\n");

    char symbol = '!';
    char* temp_string = (char*)calloc(MAX_SIZE, sizeof(char));
    int now_pos_symbol = 0, code_of_last_command = -1;

    char* assembled_text = (char*)calloc(2 * MAX_SIZE * input_file->lines, sizeof(char));
    Label* labels = (Label*)calloc(MAX_COUNT_LABELS, sizeof(Label));
    int index_in_assembled_text = 0, index_in_labels = 0;
    int number_of_byte = 0;

    Label* go_to_labels = (Label*)calloc(MAX_SIZE * input_file->lines, sizeof(Label));

    int index_in_go_to_labels = 0;

    temp_string = strtok(input_file->text, SEPARATORS);
    while(temp_string != NULL) {
        //printf("now_command: %s (%d)\n", temp_string, number_of_byte);
        find_and_write_command(temp_string, assembled_text, &index_in_assembled_text, labels, &index_in_labels, &number_of_byte, go_to_labels, &index_in_go_to_labels);
        temp_string = strtok(NULL, SEPARATORS);
    }

    fill_array_by_machine_code(labels, index_in_labels, go_to_labels,  index_in_go_to_labels, assembled_text, index_in_assembled_text);

    int status = create_assembling_file(assembled_text, index_in_assembled_text);
    if(status != OK) {
        printf("There are problems with the file %s\n", name_output_file);
        return;
    }

    printf("End to assembling file...........................................\n");
}

void find_and_write_command(char* text, char* assembled_text, int* index_in_assembled_text, Label* labels, int* index_in_labels, int* number_of_byte,
                                                                                            Label* go_to_labels, int* index_in_go_to_labels) {
    int number_of_condition = 0;

    if(!strcmp(text, "push")) {
        assembler_push(text, assembled_text, index_in_assembled_text, number_of_byte);
    } else if(!strcmp(text, "pop")) {
        assembler_pop(text, assembled_text, index_in_assembled_text, labels, index_in_labels, number_of_byte, go_to_labels, index_in_go_to_labels);
    } else if(!strcmp(text, "hlt")) {
        put_int_into_assembled_text(OPERATION_CODE_HLT, assembled_text, index_in_assembled_text, number_of_byte);
        //print_ram(ram);
    } else if(is_text_connected_with_labels(text, &number_of_condition)) {
        assembler_labels(text, assembled_text, index_in_assembled_text, number_of_byte, go_to_labels, index_in_go_to_labels, number_of_condition);
    } else if(!strcmp(text, "cmp")) {
        assembler_cmp(text, assembled_text, index_in_assembled_text, number_of_byte);
    } else if(!strcmp(text, "meow")) {
        assembler_meow(assembled_text, index_in_assembled_text, number_of_byte);
    } else if(!strcmp(text, "sqrt")) {
        assembler_sqrt(assembled_text, index_in_assembled_text, number_of_byte);
    } else {
        for(int operation_code = OPERATION_CODE_ADD; operation_code<number_of_commands; ++operation_code) {
            if(!strcmp(text, TEXT_OPERATION[operation_code])) {
                put_int_into_assembled_text(operation_code, assembled_text, index_in_assembled_text, number_of_byte);
                return;
            }
        }

        int length_command = strlen(text);
        if(text[length_command-1] == ':') {
            create_label(text, labels, index_in_labels, *index_in_assembled_text, *number_of_byte);
        }
    }
}

void assembler_push(char* text, char* assembled_text, int* index_in_assembled_text, int* number_of_byte) {
    put_int_into_assembled_text(OPERATION_CODE_PUSH, assembled_text, index_in_assembled_text, number_of_byte);

    text = strtok(NULL, SEPARATORS);

    int type_push_value = type_of_value(text);
    put_int_into_assembled_text(type_push_value, assembled_text, index_in_assembled_text, number_of_byte);

    if(type_push_value == IS_REGISTER || type_push_value == (IS_RAM | IS_REGISTER)) {
        put_int_into_assembled_text(get_number_of_register(text), assembled_text, index_in_assembled_text, number_of_byte);
    } else if(type_push_value == IS_ELEM_T || type_push_value == (IS_RAM | IS_ELEM_T)) {
        double now_value = strtod(text, NULL);
        put_double_into_assembled_text(now_value, assembled_text, index_in_assembled_text, number_of_byte);
    } else {
        printf("goodbye... (%d)\n", type_push_value);
        abort();
    }
}

void assembler_pop(char* text, char* assembled_text, int* index_in_assembled_text, Label* labels, int* index_in_labels, int* number_of_byte,
                                                                                   Label* go_to_labels, int* index_in_go_to_labels) {

    put_int_into_assembled_text(OPERATION_CODE_POP, assembled_text, index_in_assembled_text, number_of_byte);

    text = strtok(NULL, SEPARATORS);

    if(type_of_value(text) == IS_REGISTER) {
        put_int_into_assembled_text(IS_REGISTER, assembled_text, index_in_assembled_text, number_of_byte);
        put_int_into_assembled_text(get_number_of_register(text), assembled_text, index_in_assembled_text, number_of_byte);
    } else {
        put_int_into_assembled_text(NOT_ARGS, assembled_text, index_in_assembled_text, number_of_byte);
        find_and_write_command(text, assembled_text, index_in_assembled_text, labels, index_in_labels, number_of_byte, go_to_labels, index_in_go_to_labels);
    }
}

void assembler_cmp(char* text, char* assembled_text, int* index_in_assembled_text, int* number_of_byte) {
    put_int_into_assembled_text(OPERATION_CODE_CMP, assembled_text, index_in_assembled_text, number_of_byte);

    put_cmp_value(text, assembled_text, index_in_assembled_text, number_of_byte);

    put_cmp_value(text, assembled_text, index_in_assembled_text, number_of_byte);
}

void assembler_meow(char* assembled_text, int* index_in_assembled_text, int* number_of_byte) {
    put_int_into_assembled_text(OPERATION_CODE_MEOW, assembled_text, index_in_assembled_text, number_of_byte);
}

void assembler_sqrt(char* assembled_text, int* index_in_assembled_text, int* number_of_byte) {
    put_int_into_assembled_text(OPERATION_CODE_SQRT, assembled_text, index_in_assembled_text, number_of_byte);
}

void assembler_labels(char* text, char* assembled_text, int* index_in_assembled_text, int* number_of_byte, Label* go_to_labels, int* index_in_go_to_labels, int number_of_condition) {

    int now_command = 0;

    if(strcmp(text, "ret")) {
        put_int_into_assembled_text(number_of_condition, assembled_text, index_in_assembled_text, number_of_byte);

        text = strtok(NULL, SEPARATORS);
        if(!strcmp(text, "recret")) {
            put_int_into_assembled_text(OPERATION_CODE_RECURSIVE_RET, assembled_text, index_in_assembled_text, number_of_byte);
        } else {
            go_to_labels[*index_in_go_to_labels].name = text;
            go_to_labels[*index_in_go_to_labels].byte_address = *number_of_byte;
            go_to_labels[*index_in_go_to_labels].type_of_command = number_of_condition;
            go_to_labels[(*index_in_go_to_labels)++].address = *index_in_assembled_text;

            put_int_into_assembled_text(NOTHING, assembled_text, index_in_assembled_text, number_of_byte);
        }
    } else {
        put_int_into_assembled_text(number_of_condition, assembled_text, index_in_assembled_text, number_of_byte);
    }
}

void put_cmp_value(char* text, char* assembled_text, int* index_in_assembled_text, int* number_of_byte) {
    text = strtok(NULL, SEPARATORS);

    int type_of_argument = type_of_value(text);
    put_int_into_assembled_text(type_of_argument, assembled_text, index_in_assembled_text, number_of_byte);

    if(type_of_argument == IS_REGISTER) {
        put_int_into_assembled_text(get_number_of_register(text), assembled_text, index_in_assembled_text, number_of_byte);
    } else {
        put_double_into_assembled_text(strtod(text, NULL), assembled_text, index_in_assembled_text, number_of_byte);
    }
}

void put_int_into_assembled_text(int code_of_operation, char* assembled_text, int* index_in_assembled_text, int* number_of_byte) {
    int length_code = 0;
    int reverse_code = reversed_number(code_of_operation, &length_code);

    double converted_code = (double)code_of_operation;

    memcpy(assembled_text + *index_in_assembled_text, &converted_code, sizeof(double)); //
    *index_in_assembled_text += sizeof(double);

    ++(*number_of_byte);
}

void put_double_into_assembled_text(double code_of_operation, char* assembled_text, int* index_in_assembled_text, int* number_of_byte) {
    memcpy(assembled_text + *index_in_assembled_text, &code_of_operation, sizeof(double)); //
    *index_in_assembled_text += sizeof(double);

    ++(*number_of_byte);
}

void create_label(char* text, Label* labels, int* index_in_labels, int index_in_assembled_text, int number_of_byte) {
    labels[*index_in_labels].name = (char*)calloc(MAX_SIZE, sizeof(char));
    strcpy(labels[*index_in_labels].name, text);
    labels[*index_in_labels].address = number_of_byte;
    labels[*index_in_labels].byte_address = number_of_byte;
    labels[*index_in_labels].type_of_command = -1;

    ++(*index_in_labels);
}

int create_assembling_file(const char* assembled_text, const int index_in_assembled_text) {
    FILE* output_file = fopen("out_asm.lst", "wb");
    if(output_file == NULL) {
        return ERROR_NUMBER;
    }

    fwrite(assembled_text, sizeof(assembled_text[0]), index_in_assembled_text, output_file);

    int status = fclose(output_file);
    if(status == EOF) {
        return ERROR_NUMBER;
    }

    return OK;
}

int reversed_number(int value, int* length) {
    int reverse_number = 0;

    if(value == 0) {
        *length = 1;
    }

    while(value > 0) {
        reverse_number *= 10;
        reverse_number += value % 10;
        value /= 10;
        ++(*length);
    }

    return reverse_number;
}

int length_of_number(int value) {
    if(value == 0) {
        return 1;
    }

    int length = 0;

    while(value > 0) {
        value /= 10;
        ++length;
    }

    return length;
}

void fill_array_by_machine_code(Label* labels, int count_of_labels, Label* go_to_labels, int count_of_go_to_labels, char* assembled_text, int length_of_assembled_text) {
    for(int i=0; i<count_of_go_to_labels; ++i) {
        for(int j=0; j<count_of_labels; ++j) {
            if(!strcmp(labels[j].name, go_to_labels[i].name)) {
                double copy_ = (double)labels[j].byte_address;
                memcpy(assembled_text + go_to_labels[i].address, &copy_, sizeof(double)); // (labels[j].byte_address)
            }
        }
    }
}

bool is_text_connected_with_labels(char* text, int* number_of_condition) {
    int command_is_condition = true;

    if(!strcmp(text, "jmp")) {
        *number_of_condition = OPERATION_CODE_JMP;
    } else if(!strcmp(text, "je")) {
        *number_of_condition = OPERATION_CODE_JE;
    } else if(!strcmp(text, "jne")) {
        *number_of_condition = OPERATION_CODE_JNE;
    } else if(!strcmp(text, "ja")) {
        *number_of_condition = OPERATION_CODE_JA;
    } else if(!strcmp(text, "jae")) {
        *number_of_condition = OPERATION_CODE_JAE;
    } else if(!strcmp(text, "jb")) {
        *number_of_condition = OPERATION_CODE_JB;
    } else if(!strcmp(text, "jbe")) {
        *number_of_condition = OPERATION_CODE_JBE;
    } else if(!strcmp(text, "call")) {
        *number_of_condition = OPERATION_CODE_CALL;
    } else if(!strcmp(text, "ret")) {
        *number_of_condition = OPERATION_CODE_RET;
    } else {
        *number_of_condition = -1;
        command_is_condition = false;
    }

    return command_is_condition;
}
