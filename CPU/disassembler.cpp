#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "common.h"
#include "disassembler.h"

void disassembling_file() {
    File input_file = {};
    int status = file_construct(&input_file, name_input_file_disass);
    printf("%s\n", input_file.name);
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
    int flag_of_the_command = 0, code_of_last_command = 0;
    int put_in_register = -1;

    for(int i=0; i<input_file.information.st_size; ++i) {
        temp_string[now_pos_symbol] = input_file.text[i];

        if(isspace(temp_string[now_pos_symbol++])) {
            if(!is_right_command(temp_string, "1") && !flag_of_the_command) {
                put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_PUSH], disassembled_text, &index_in_disassembled_text, NOT_END_LINE);
                flag_of_the_command = 1;
                code_of_last_command = OPERATION_CODE_PUSH;
                now_pos_symbol = 0;
            } else if(!is_right_command(temp_string, "2") && !flag_of_the_command) {
                put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_POP], disassembled_text, &index_in_disassembled_text, NOT_END_LINE);
                flag_of_the_command = 1;
                code_of_last_command = OPERATION_CODE_POP;
                now_pos_symbol = 0;
            } else if(!is_right_command(temp_string, "3") && !flag_of_the_command) {
                put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_ADD], disassembled_text, &index_in_disassembled_text, END_LINE);
                flag_of_the_command = 0;
                code_of_last_command = OPERATION_CODE_ADD;
                now_pos_symbol = 0;
            } else if(!is_right_command(temp_string, "4") && !flag_of_the_command) {
                put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_SUB], disassembled_text, &index_in_disassembled_text, END_LINE);
                flag_of_the_command = 0;
                code_of_last_command = OPERATION_CODE_SUB;
                now_pos_symbol = 0;
            } else if(!is_right_command(temp_string, "5") && !flag_of_the_command) {
                put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_MUL], disassembled_text, &index_in_disassembled_text, END_LINE);
                flag_of_the_command = 0;
                code_of_last_command = OPERATION_CODE_MUL;
                now_pos_symbol = 0;
            } else if(!is_right_command(temp_string, "6") && !flag_of_the_command) {
                put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_DIV], disassembled_text, &index_in_disassembled_text, END_LINE);
                flag_of_the_command = 0;
                code_of_last_command = OPERATION_CODE_DIV;
                now_pos_symbol = 0;
            } else if(!is_right_command(temp_string, "7") && !flag_of_the_command) {
                put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_OUT], disassembled_text, &index_in_disassembled_text, END_LINE);
                flag_of_the_command = 0;
                code_of_last_command = OPERATION_CODE_OUT;
                now_pos_symbol = 0;
            } else if(!is_right_command(temp_string, "8") && !flag_of_the_command) {
                put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_IN], disassembled_text, &index_in_disassembled_text, END_LINE);
                flag_of_the_command = 0;
                code_of_last_command = OPERATION_CODE_IN;
                now_pos_symbol = 0;
            } else if(!is_right_command(temp_string, "0") && !flag_of_the_command) {
                put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_HLT], disassembled_text, &index_in_disassembled_text, END_LINE);
                flag_of_the_command = 0;
                code_of_last_command = OPERATION_CODE_HLT;
                now_pos_symbol = 0;
            } else {
                if(code_of_last_command == OPERATION_CODE_PUSH || code_of_last_command == OPERATION_CODE_POP) {
                    if(put_in_register == -1 && !is_right_command(temp_string, "0")) {
                        if(code_of_last_command == OPERATION_CODE_PUSH)
                            put_in_register = 0;
                        else {
                            put_char_into_disassembled_text("", disassembled_text, &index_in_disassembled_text, END_LINE);
                        }
                    } else if(put_in_register == -1 && !is_right_command(temp_string, "1")) {
                        put_in_register = 1;
                    } else if(put_in_register == 0) {
                        put_int_into_disassembled_text(temp_string, now_pos_symbol, disassembled_text, &index_in_disassembled_text);
                        put_in_register = -1;
                    } else if(put_in_register == 1) {
                        char* ptr_to_strtod = "";
                        int possible_register = strtod(temp_string, &ptr_to_strtod);
                        if(possible_register >= number_of_register_vars) {
                            printf("ERROR REGISTER! %d\n", possible_register);
                            return;
                        } else {
                            put_char_into_disassembled_text(TEXT_REGISTERS[possible_register], disassembled_text, &index_in_disassembled_text, END_LINE);
                        }
                        put_in_register = -1;
                    }
                    now_pos_symbol = 0;
                }
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

void put_char_into_disassembled_text(const char* command, char* disassembled_text, int* index_in_disassembled_text, int flag_of_the_end_line) {
    int length_command = strlen(command);

    for(int i=0; i<length_command; ++i) {
        disassembled_text[*index_in_disassembled_text] = command[i];
        ++(*index_in_disassembled_text);
    }

    if(flag_of_the_end_line == END_LINE) {
        disassembled_text[*index_in_disassembled_text] = '\n';
    } else {
        disassembled_text[*index_in_disassembled_text] = ' ';
    }
    ++(*index_in_disassembled_text);
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

/*
int main() {
    disassembling_file();
}
*/
