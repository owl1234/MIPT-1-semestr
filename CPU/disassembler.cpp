#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "common.h"
#include "disassembler.h"

const char* name_input_file_disass  = "input_disass.txt";
const char* name_output_file_disass = "output_disass.txt";

void disassembling_file() {
    File input_file = {};
    int status = file_construct(&input_file, name_input_file_disass);
    if(status != OK_FILE) {
        printf("PANIC.....\n");
        return;
    }

    printf("Start to disassembling file.........................................\n");

    char symbol = '!';
    char temp_string[MAX_SIZE] = {};
    int now_pos_symbol = 0;

    char* disassembled_text = (char*)calloc(MAX_SIZE * input_file.lines, sizeof(char));
    int index_in_disassembled_text = 0;
    int flag_of_the_command = 0;

    for(int i=0; i<input_file.information.st_size; ++i) {
        temp_string[now_pos_symbol] = input_file.text[i];

        // printf("i: %d, index: %d, flag_of_the_command: %d\n", i, now_pos_symbol, flag_of_the_command);
        if(isspace(temp_string[now_pos_symbol++])) {
            if(!is_right_command(temp_string, "1") && !flag_of_the_command) {
                // printf("yes of push........\n");
                put_char_into_disassembled_text(OPERATION_CODE_PUSH, disassembled_text, &index_in_disassembled_text);
                flag_of_the_command = 1;
                now_pos_symbol = 0;
            } else if(!is_right_command(temp_string, "2") && !flag_of_the_command) {
                // printf("yes of pop........\n");
                put_char_into_disassembled_text(OPERATION_CODE_POP, disassembled_text, &index_in_disassembled_text);
                flag_of_the_command = 0;
                now_pos_symbol = 0;
            } else if(!is_right_command(temp_string, "3") && !flag_of_the_command) {
                // printf("yes of add........\n");
                put_char_into_disassembled_text(OPERATION_CODE_ADD, disassembled_text, &index_in_disassembled_text);
                flag_of_the_command = 0;
                now_pos_symbol = 0;
            } else if(!is_right_command(temp_string, "4") && !flag_of_the_command) {
                // printf("yes of sub........\n");
                put_char_into_disassembled_text(OPERATION_CODE_SUB, disassembled_text, &index_in_disassembled_text);
                flag_of_the_command = 0;
                now_pos_symbol = 0;
            } else if(!is_right_command(temp_string, "5") && !flag_of_the_command) {
                // printf("yes of mul........\n");
                put_char_into_disassembled_text(OPERATION_CODE_MUL, disassembled_text, &index_in_disassembled_text);
                flag_of_the_command = 0;
                now_pos_symbol = 0;
            } else if(!is_right_command(temp_string, "6") && !flag_of_the_command) {
                // printf("yes of div........\n");
                put_char_into_disassembled_text(OPERATION_CODE_DIV, disassembled_text, &index_in_disassembled_text);
                flag_of_the_command = 0;
                now_pos_symbol = 0;
            } else if(!is_right_command(temp_string, "7") && !flag_of_the_command) {
                // printf("yes of out........\n");
                put_char_into_disassembled_text(OPERATION_CODE_OUT, disassembled_text, &index_in_disassembled_text);
                flag_of_the_command = 0;
                now_pos_symbol = 0;
            } else if(!is_right_command(temp_string, "0") && !flag_of_the_command) {
                // printf("yes of hlt........\n");
                put_char_into_disassembled_text(OPERATION_CODE_HLT, disassembled_text, &index_in_disassembled_text);
                flag_of_the_command = 0;
                now_pos_symbol = 0;
            } else {
                put_int_into_disassembled_text(temp_string, now_pos_symbol, disassembled_text, &index_in_disassembled_text);
                if(input_file.text[i] == '\n') {
                    flag_of_the_command = 0;
                    now_pos_symbol = 0;
                }
            }
        }
    }

    status = create_disassembling_file(disassembled_text, index_in_disassembled_text);
    if(status != OK_FILE) {
        printf("There are problems with the file %s\n", name_output_file_disass);
        return;
    }

    printf("End to disassembling file...........................................\n");
}

void put_char_into_disassembled_text(int code_of_operation, char* disassembled_text, int* index) {
    const char* command = TEXT_OPERATION[code_of_operation];
    int length_command = strlen(command);

    for(int i=0; i<length_command; ++i) {
        disassembled_text[*index] = command[i];
        ++(*index);
    }

    if(NUMBER_ARGUMENTS_FOR_OPERATION[code_of_operation] == 0) {
        disassembled_text[*index] = '\n';
        ++(*index);
    } else {
        disassembled_text[*index] = ' ';
        ++(*index);
    }
}

void put_int_into_disassembled_text(const char* temp_string, const int length, char* disassembled_text, int* index_in_disassembled_text) {
    for(int i=0; i<length; ++i) {
        disassembled_text[*index_in_disassembled_text] = temp_string[i];
        ++(*index_in_disassembled_text);
    }
}

int create_disassembling_file(const char* disassembled_text, const int index_in_disassembled_text) {
    FILE* output_file = fopen(name_output_file_disass, "w");
    if(output_file == NULL) {
        return ERROR_NUMBER;
    }

    fwrite(disassembled_text, sizeof(disassembled_text[0]), index_in_disassembled_text, output_file);

    int status = fclose(output_file);
    if(status == EOF) {
        return ERROR_NUMBER;
    }

    return OK_FILE;
}

int main() {
    disassembling_file();
}
