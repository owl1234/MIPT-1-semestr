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
    char temp_string[MAX_SIZE] = {};
    int now_pos_symbol = 0, code_of_last_command = -1;

    char* assembled_text = (char*)calloc(MAX_SIZE * input_file.lines, sizeof(char));
    int index_in_assembled_text = 0;

    for(int i=0; i<input_file.information.st_size; ++i) {
        temp_string[now_pos_symbol] = input_file.text[i];

        if(isspace(temp_string[now_pos_symbol++])) {
            if(!is_right_command(temp_string, "push")) {
                put_int_into_assembled_text(OPERATION_CODE_PUSH, assembled_text, &index_in_assembled_text, NOT_END_LINE);
                code_of_last_command = OPERATION_CODE_PUSH;
                now_pos_symbol = 0;
            } else if(!is_right_command(temp_string, "pop")) {

                if(input_file.text[i] == '\n') {
                    put_int_into_assembled_text(OPERATION_CODE_POP, assembled_text, &index_in_assembled_text, NOT_END_LINE);
                    put_int_into_assembled_text(0, assembled_text, &index_in_assembled_text, END_LINE);
                } else {
                    put_int_into_assembled_text(OPERATION_CODE_POP, assembled_text, &index_in_assembled_text, NOT_END_LINE);
                }

                code_of_last_command = OPERATION_CODE_POP;
                now_pos_symbol = 0;
            } else if(!is_right_command(temp_string, "add")) {
                put_int_into_assembled_text(OPERATION_CODE_ADD, assembled_text, &index_in_assembled_text, END_LINE);
                code_of_last_command = OPERATION_CODE_ADD;
                now_pos_symbol = 0;
            } else if(!is_right_command(temp_string, "sub")) {
                put_int_into_assembled_text(OPERATION_CODE_SUB, assembled_text, &index_in_assembled_text, END_LINE);
                code_of_last_command = OPERATION_CODE_SUB;
                now_pos_symbol = 0;
            } else if(!is_right_command(temp_string, "mul")) {
                put_int_into_assembled_text(OPERATION_CODE_MUL, assembled_text, &index_in_assembled_text, END_LINE);
                code_of_last_command = OPERATION_CODE_MUL;
                now_pos_symbol = 0;
            } else if(!is_right_command(temp_string, "div")) {
                put_int_into_assembled_text(OPERATION_CODE_DIV, assembled_text, &index_in_assembled_text, END_LINE);
                code_of_last_command = OPERATION_CODE_DIV;
                now_pos_symbol = 0;
            } else if(!is_right_command(temp_string, "out")) {
                put_int_into_assembled_text(OPERATION_CODE_OUT, assembled_text, &index_in_assembled_text, END_LINE);
                code_of_last_command = OPERATION_CODE_OUT;
                now_pos_symbol = 0;
            } else if(!is_right_command(temp_string, "in")) {
                put_int_into_assembled_text(OPERATION_CODE_IN, assembled_text, &index_in_assembled_text, END_LINE);
                code_of_last_command = OPERATION_CODE_IN;
                now_pos_symbol = 0;
            } else if(!is_right_command(temp_string, "hlt")) {
                put_int_into_assembled_text(OPERATION_CODE_HLT, assembled_text, &index_in_assembled_text, END_LINE);
                code_of_last_command = OPERATION_CODE_HLT;
                now_pos_symbol = 0;
                break;
            } else if(!is_right_command(temp_string, "rax")) {
                work_with_registers(code_of_last_command, RAX, assembled_text, &index_in_assembled_text);
                now_pos_symbol = 0;
            } else if(!is_right_command(temp_string, "rbx")) {
                work_with_registers(code_of_last_command, RBX, assembled_text, &index_in_assembled_text);
                now_pos_symbol = 0;
            } else if(!is_right_command(temp_string, "rcx")) {
                work_with_registers(code_of_last_command, RCX, assembled_text, &index_in_assembled_text);
                now_pos_symbol = 0;
            } else if(!is_right_command(temp_string, "rdx")) {
                work_with_registers(code_of_last_command, RDX, assembled_text, &index_in_assembled_text);
                now_pos_symbol = 0;
            }
            else {
                put_int_into_assembled_text(0, assembled_text, &index_in_assembled_text, NOT_END_LINE);
                put_char_into_assembled_text(temp_string, now_pos_symbol, assembled_text, &index_in_assembled_text);
                if(input_file.text[i] == '\n') {
                    now_pos_symbol = 0;
                }
            }
        }
    }

    status = create_assembling_file(assembled_text, index_in_assembled_text);
    if(status != OK_FILE) {
        printf("There are problems with the file %s\n", name_output_file_ass);
        return;
    }

    printf("End to assembling file...........................................\n");
}

void put_int_into_assembled_text(int code_of_operation, char* assembled_text, int* index_in_assembled_text, int flag_of_the_end_line) {
    int length_code = 0;
    int reverse_code = reversed_number(code_of_operation, &length_code);

    for(int i=0; i<length_code; ++i) {
        assembled_text[*index_in_assembled_text] = (reverse_code % 10 + 48);
        reverse_code /= 10;
        ++(*index_in_assembled_text);
    }

    if(flag_of_the_end_line == END_LINE) {
        put_char_into_assembled_text("\n", 1, assembled_text, index_in_assembled_text);
    } else {
        put_char_into_assembled_text(" ", 1, assembled_text, index_in_assembled_text);
    }
}

void put_char_into_assembled_text(const char* temp_string, const int length, char* assembled_text, int* index_in_assembled_text) {
    for(int i=0; i<length; ++i) {
        assembled_text[*index_in_assembled_text] = temp_string[i];
        ++(*index_in_assembled_text);
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

void work_with_registers(int last_operation_code, int registr, char* assembled_text, int* index_in_assembled_text) {
    put_int_into_assembled_text(1, assembled_text, index_in_assembled_text, NOT_END_LINE);
    put_int_into_assembled_text(registr, assembled_text, index_in_assembled_text, END_LINE);
}

/*
int main() {
    assembling_file();
}
*/
