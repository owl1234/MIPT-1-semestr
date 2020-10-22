#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "assembler.h"
#include "common.h"

void assembling_file() {
    File input_file = {};
    int status = file_construct(&input_file, name_input_file_ass);
    if(status != OK_FILE) {
        printf("PANIC.....\n");
        return;
    }

    printf("Start to assembling file.........................................\n");

    char symbol = '!';
    char* temp_string = (char*)calloc(MAX_SIZE, sizeof(char));
    int now_pos_symbol = 0, code_of_last_command = -1;

    char* assembled_text = (char*)calloc(MAX_SIZE * input_file.lines, sizeof(char));
    Label* labels = (Label*)calloc(MAX_SIZE, sizeof(Label));
    int index_in_assembled_text = 0, index_in_labels = 0;
    int number_of_byte = 0;

    Label* jump_byte_position = (Label*)calloc(MAX_SIZE * input_file.lines, sizeof(Label));
    int index_in_jump_byte_position = 0;

    temp_string = strtok(input_file.text, SEPARATORS);
    while(temp_string != NULL) {
        find_and_write_command(temp_string, assembled_text, &index_in_assembled_text, labels, &index_in_labels, &number_of_byte, jump_byte_position, &index_in_jump_byte_position);
        temp_string = strtok(NULL, SEPARATORS);
    }

    /*printf("%s\n------------------------------------------------------\n", assembled_text);
    printf("Labels: ");
    for(int i=0; i<index_in_labels; ++i)
        printf("%s %d\n", labels[i].name, labels[i].address);
    printf("\njump labels: ");
    for(int i=0; i<index_in_jump_byte_position; ++i)
        printf("%s %d\n", jump_byte_position[i].name, jump_byte_position[i].address);
    printf("\n\n");*/

    fill_array_by_machine_code(labels, index_in_labels, jump_byte_position, index_in_jump_byte_position, assembled_text, index_in_assembled_text);

    status = create_assembling_file(assembled_text, index_in_assembled_text);
    if(status != OK_FILE) {
        printf("There are problems with the file %s\n", name_output_file_ass);
        return;
    }

    printf("End to assembling file...........................................\n");
}

void find_and_write_command(char* text, char* assembled_text, int* index_in_assembled_text, Label* labels, int* index_in_labels, int* number_of_byte, Label* jump_byte_position, int* index_in_jump_byte_position) {
    if(!is_right_command(text, "push")) {
        text = strtok(NULL, SEPARATORS);

        for(int registr=0; registr<number_of_register_vars; ++registr) {
            if(!strcmp(text, TEXT_REGISTERS[registr])) {
                put_int_into_assembled_text(OPERATION_CODE_PUSH_IN_REGISTR, assembled_text, index_in_assembled_text, number_of_byte, NOT_END_LINE);
                put_int_into_assembled_text(registr, assembled_text, index_in_assembled_text, number_of_byte, END_LINE);
                return;
            }
        }

        put_int_into_assembled_text(OPERATION_CODE_PUSH, assembled_text, index_in_assembled_text, number_of_byte, NOT_END_LINE);
        put_char_into_assembled_text(text, strlen(text), assembled_text, index_in_assembled_text, number_of_byte);
        put_char_into_assembled_text("\n", 1, assembled_text, index_in_assembled_text, number_of_byte);
    } else if(!is_right_command(text, "pop")) {
        text = strtok(NULL, SEPARATORS);

        for(int registr=0; registr<number_of_register_vars; ++registr) {
            if(!strcmp(text, TEXT_REGISTERS[registr])) {
                put_int_into_assembled_text(OPERATION_CODE_POP * 10 + registr, assembled_text, index_in_assembled_text, number_of_byte, END_LINE);
                //++(*number_of_byte);
                return;
            }
        }

        put_int_into_assembled_text(OPERATION_CODE_POP, assembled_text, index_in_assembled_text, number_of_byte, END_LINE);

        find_and_write_command(text, assembled_text, index_in_assembled_text, labels, index_in_labels, number_of_byte, jump_byte_position, index_in_jump_byte_position);
    } else if(!is_right_command(text, "hlt")) {
        put_int_into_assembled_text(OPERATION_CODE_HLT, assembled_text, index_in_assembled_text, number_of_byte, END_LINE);
        return;
    } else if(!is_right_command(text, "jmp")) {
        put_int_into_assembled_text(OPERATION_CODE_JMP, assembled_text, index_in_assembled_text, number_of_byte, NOT_END_LINE);
        text = strtok(NULL, SEPARATORS);
        jump_byte_position[*index_in_jump_byte_position].name = text;
        jump_byte_position[(*index_in_jump_byte_position)++].address = *index_in_assembled_text;
        put_int_into_assembled_text(MAX_SIZE, assembled_text, index_in_assembled_text, number_of_byte, END_LINE);
    } else {
        for(int operation_code = OPERATION_CODE_ADD; operation_code<number_of_commands; ++operation_code) {
            if(!strcmp(text, TEXT_OPERATION[operation_code])) {
                put_int_into_assembled_text(operation_code, assembled_text, index_in_assembled_text, number_of_byte, END_LINE);
                return;
            }
        }

        int length_command = strlen(text);
        if(text[length_command-1] == ':') {
            create_label(text, labels, index_in_labels, *index_in_assembled_text, *number_of_byte);
        }
    }
}

void put_int_into_assembled_text(int code_of_operation, char* assembled_text, int* index_in_assembled_text, int* number_of_byte, int flag_of_the_end_line) {
    int length_code = 0;
    int reverse_code = reversed_number(code_of_operation, &length_code);

    for(int i=0; i<length_code; ++i) {
        assembled_text[*index_in_assembled_text] = (reverse_code % 10 + 48);
        reverse_code /= 10;
        ++(*index_in_assembled_text);
    }

    if(flag_of_the_end_line == END_LINE) {
        put_char_into_assembled_text("\n", 1, assembled_text, index_in_assembled_text, number_of_byte);
    } else {
        put_char_into_assembled_text(" ", 1, assembled_text, index_in_assembled_text, number_of_byte);
    }

    ++(*number_of_byte);
}

void create_label(char* text, Label* labels, int* index_in_labels, int index_in_assembled_text, int number_of_byte) {
    printf("new label: %d - byte, %s - text\n", number_of_byte, text);
    labels[*index_in_labels].name = (char*)calloc(MAX_SIZE, sizeof(char));
    strcpy(labels[*index_in_labels].name, text);
    labels[*index_in_labels].address = number_of_byte;

    ++(*index_in_labels);
}

void put_char_into_assembled_text(const char* temp_string, const int length, char* assembled_text, int* index_in_assembled_text, int* number_of_byte) {
    for(int i=0; i<length; ++i) {
        assembled_text[*index_in_assembled_text] = temp_string[i];
        ++(*index_in_assembled_text);
    }

    if(temp_string != " " && temp_string != "\n") {
        ++(*number_of_byte);
    }
}

int create_assembling_file(const char* assembled_text, const int index_in_assembled_text) {
    FILE* output_file = fopen(name_output_file_ass, "w");
    if(output_file == NULL) {
        return ERROR_NUMBER;
    }

    fwrite(assembled_text, sizeof(assembled_text[0]), index_in_assembled_text, output_file);

    int status = fclose(output_file);
    if(status == EOF) {
        return ERROR_NUMBER;
    }

    return OK_FILE;
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

void fill_array_by_machine_code(Label* labels, int count_of_labels, Label* jump_byte_position, int count_of_jump_byte_position, char* assembled_text, int length_of_assembled_text) {
    char* temp_string = (char*)calloc(MAX_SIZE, sizeof(char));
    int label_index = 0, assembler_text_index = 0;

    for(int i=0; i<count_of_jump_byte_position; ++i) {
        int position = jump_byte_position[i].address;

        for(int j=0; j<count_of_labels; ++j) {
            if(!strcmp(jump_byte_position[i].name, labels[j].name)) {
                assembled_text[position]     = labels[j].address / 10 + 48;
                assembled_text[position + 1] = labels[j].address % 10 + 48;
            }
        }
    }
}
/*
int main() {
    assembling_file();
}*/
