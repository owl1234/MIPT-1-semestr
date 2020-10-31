#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "assembler.h"
#include "common.h"

void print_ram(Elem_t* ram) {
    for(int i=0; i<10; ++i) {
        printf("%d ", ram[i]);
    }
    printf("\n");
}

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

    char* assembled_text = (char*)calloc(100 * input_file.lines, sizeof(char));
    Label* labels = (Label*)calloc(MAX_COUNT_LABELS, sizeof(Label));
    int index_in_assembled_text = 0, index_in_labels = 0;
    int number_of_byte = 0;

    Label* go_to_labels = (Label*)calloc(MAX_SIZE * input_file.lines, sizeof(Label));

    int index_in_go_to_labels = 0; //, index_in_ret_byte_position = 0, index_in_call_byte_position = 0, index_in_cond_byte_position = 0;

    temp_string = strtok(input_file.text, SEPARATORS);
    while(temp_string != NULL) {
        printf("now_command: %s\n", temp_string);
        find_and_write_command(temp_string, assembled_text, &index_in_assembled_text, labels, &index_in_labels, &number_of_byte, go_to_labels, &index_in_go_to_labels);
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

    fill_array_by_machine_code(labels, index_in_labels, go_to_labels,  index_in_go_to_labels, assembled_text, index_in_assembled_text);

    status = create_assembling_file(assembled_text, index_in_assembled_text);
    if(status != OK_FILE) {
        printf("There are problems with the file %s\n", name_output_file_ass);
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
        put_int_into_assembled_text(OPERATION_CODE_HLT, assembled_text, index_in_assembled_text, number_of_byte, END_LINE);
        print_ram(ram);
    } else if(is_text_connected_with_labels(text, &number_of_condition)) {
        //printf("jshba %s\n", text);
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
                //printf("=-------------------------------------operat_code: %d (%s)\n", operation_code, text);
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

void assembler_push(char* text, char* assembled_text, int* index_in_assembled_text, int* number_of_byte) {
    put_int_into_assembled_text(OPERATION_CODE_PUSH, assembled_text, index_in_assembled_text, number_of_byte, SPACE);

    text = strtok(NULL, SEPARATORS);

    int type_push_value = type_of_value(text);
    printf("ass_push: %d\n", type_push_value);
    put_int_into_assembled_text(type_push_value, assembled_text, index_in_assembled_text, number_of_byte, SPACE);

    if(type_push_value == IS_REGISTER || type_push_value == (IS_RAM | IS_REGISTER)) {
        put_int_into_assembled_text(get_number_of_register(text), assembled_text, index_in_assembled_text, number_of_byte, END_LINE);
    } else if(type_push_value == IS_ELEM_T || type_push_value == (IS_RAM | IS_ELEM_T)) {
        put_char_into_assembled_text(text, assembled_text, index_in_assembled_text, number_of_byte, END_LINE);
    } else {
        printf("goodbue... (%d)\n", type_push_value);
        abort();
    }
}

void assembler_pop(char* text, char* assembled_text, int* index_in_assembled_text, Label* labels, int* index_in_labels, int* number_of_byte,
                                                                                   Label* go_to_labels, int* index_in_go_to_labels) {

    put_int_into_assembled_text(OPERATION_CODE_POP, assembled_text, index_in_assembled_text, number_of_byte, SPACE);

    text = strtok(NULL, SEPARATORS);

    if(type_of_value(text) == IS_REGISTER) {
        put_int_into_assembled_text(IS_REGISTER, assembled_text, index_in_assembled_text, number_of_byte, SPACE);
        put_int_into_assembled_text(get_number_of_register(text), assembled_text, index_in_assembled_text, number_of_byte, END_LINE);
    } else {
        put_int_into_assembled_text(NOT_ARGS, assembled_text, index_in_assembled_text, number_of_byte, END_LINE);
        find_and_write_command(text, assembled_text, index_in_assembled_text, labels, index_in_labels, number_of_byte, go_to_labels, index_in_go_to_labels);
    }
}

void assembler_cmp(char* text, char* assembled_text, int* index_in_assembled_text, int* number_of_byte) {
    put_int_into_assembled_text(OPERATION_CODE_CMP, assembled_text, index_in_assembled_text, number_of_byte, SPACE);

    put_cmp_value(text, assembled_text, index_in_assembled_text, number_of_byte, SPACE);

    put_cmp_value(text, assembled_text, index_in_assembled_text, number_of_byte, END_LINE);
}

void assembler_meow(char* assembled_text, int* index_in_assembled_text, int* number_of_byte) {
    put_int_into_assembled_text(OPERATION_CODE_MEOW, assembled_text, index_in_assembled_text, number_of_byte, END_LINE);
    printf("meow\n");
}

void assembler_sqrt(char* assembled_text, int* index_in_assembled_text, int* number_of_byte) {
    put_int_into_assembled_text(OPERATION_CODE_SQRT, assembled_text, index_in_assembled_text, number_of_byte, END_LINE);
}

void assembler_labels(char* text, char* assembled_text, int* index_in_assembled_text, int* number_of_byte, Label* go_to_labels, int* index_in_go_to_labels, int number_of_condition) {

    int now_command = 0;

    if(strcmp(text, "ret")) {
        put_int_into_assembled_text(number_of_condition, assembled_text, index_in_assembled_text, number_of_byte, SPACE);

        text = strtok(NULL, SEPARATORS);
        if(!strcmp(text, "recret")) {
            //printf("!!!!!!!!!!!!!!!!!!!! RECRET !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!   \n");
            put_int_into_assembled_text(OPERATION_CODE_RECURSIVE_RET, assembled_text, index_in_assembled_text, number_of_byte, END_LINE);
        } else {
            go_to_labels[*index_in_go_to_labels].name = text;
            go_to_labels[*index_in_go_to_labels].byte_address = *number_of_byte;
            go_to_labels[*index_in_go_to_labels].type_of_command = number_of_condition;
            //printf("num: %d (%s)\n", number_of_condition, text);
            go_to_labels[(*index_in_go_to_labels)++].address = *index_in_assembled_text;

            put_int_into_assembled_text(98, assembled_text, index_in_assembled_text, number_of_byte, END_LINE);
        }
    } else {
        put_int_into_assembled_text(number_of_condition, assembled_text, index_in_assembled_text, number_of_byte, END_LINE);
    }


    //printf("I'm here %s\n", text);
}

void put_cmp_value(char* text, char* assembled_text, int* index_in_assembled_text, int* number_of_byte, int end_of_line) {
    text = strtok(NULL, SEPARATORS);

    int type_of_argument = type_of_value(text);
    put_int_into_assembled_text(type_of_argument, assembled_text, index_in_assembled_text, number_of_byte, SPACE);

    if(type_of_argument == IS_REGISTER) {
        put_int_into_assembled_text(get_number_of_register(text), assembled_text, index_in_assembled_text, number_of_byte, end_of_line);
    } else {
        put_char_into_assembled_text(text, assembled_text, index_in_assembled_text, number_of_byte, end_of_line);
    }
}

void put_int_into_assembled_text(int code_of_operation, char* assembled_text, int* index_in_assembled_text, int* number_of_byte, int flag_of_the_end_line) {
    int length_code = 0;
    int reverse_code = reversed_number(code_of_operation, &length_code);

    for(int i=0; i<8-length_code; ++i) {
        assembled_text[*index_in_assembled_text] = '0';
        ++(*index_in_assembled_text);
    }
    for(int i=0; i<length_code; ++i) {
        assembled_text[*index_in_assembled_text] = (reverse_code % 10 + 48);
        reverse_code /= 10;
        ++(*index_in_assembled_text);
    }

    if(flag_of_the_end_line == END_LINE) {
        put_char_into_assembled_text("", assembled_text, index_in_assembled_text, number_of_byte, END_LINE);
    } else if(flag_of_the_end_line == SPACE) {
        put_char_into_assembled_text("", assembled_text, index_in_assembled_text, number_of_byte, SPACE);
    }

    ++(*number_of_byte);
    /*char* text = (char*)calloc(8, sizeof(char));
    for(int i=0; i<8; ++i) {
        text[i] = reverse_code % 10 + 48;
        reverse_code /= 10;
    }

    put_char_into_assembled_text(text, assembled_text, index_in_assembled_text, number_of_byte, flag_of_the_end_line);*/
}

void create_label(char* text, Label* labels, int* index_in_labels, int index_in_assembled_text, int number_of_byte) {
    printf("new label: %d - byte, %s - text\n", number_of_byte, text);
    labels[*index_in_labels].name = (char*)calloc(MAX_SIZE, sizeof(char));
    strcpy(labels[*index_in_labels].name, text);
    labels[*index_in_labels].address = number_of_byte;
    labels[*index_in_labels].byte_address = number_of_byte;
    labels[*index_in_labels].type_of_command = -1;

    ++(*index_in_labels);
}

void put_char_into_assembled_text(const char* temp_string, char* assembled_text, int* index_in_assembled_text, int* number_of_byte, int flag_of_the_end_line) {
    int length = strlen(temp_string);
    int deduction = 0;

    if(temp_string[0] == '[')
        deduction += 2;

    if(!(length == 0)) {
        for(int i=0; i<8-length+deduction; ++i) {
            assembled_text[*index_in_assembled_text] = '0';
            ++(*index_in_assembled_text);
        }
        for(int i=0; i<length; ++i) {
            if(temp_string[i] != '[' && temp_string[i] != ']') {
                assembled_text[*index_in_assembled_text] = temp_string[i];
                ++(*index_in_assembled_text);
            }
        }

        if(temp_string != " " && temp_string != "\n") {
            ++(*number_of_byte);
        }
    }

    if(flag_of_the_end_line == END_LINE)  {
        assembled_text[(*index_in_assembled_text)++] = '\n';
    } else if(flag_of_the_end_line == SPACE) {
        assembled_text[(*index_in_assembled_text)++] = ' ';
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

void fill_array_by_machine_code(Label* labels, int count_of_labels, Label* go_to_labels, int count_of_go_to_labels, char* assembled_text, int length_of_assembled_text) {
    /*printf("size: %d\n", count_of_go_to_labels);
    for(int i=0; i<count_of_go_to_labels; ++i) {
        printf("%s %d %d %d\n", go_to_labels[i].name, go_to_labels[i].address, go_to_labels[i].byte_address, go_to_labels[i].type_of_command);
    }*/

    for(int i=0; i<count_of_go_to_labels; ++i) {
        int position = go_to_labels[i].address;


        for(int j=0; j<count_of_labels; ++j) {
            if(!strcmp(labels[j].name, go_to_labels[i].name)) {
                int now_address = labels[j].address;
                //printf("position: %d\n", position);
                for(int now_pow=7; now_pow>=0; --now_pow) {
                    assembled_text[position + now_pow] = now_address % 10 + 48;
                    now_address /= 10;
                }
            }
        }
    }
    /*printf("jump (%d) : ", count_of_jump_byte_position);
    for(int i=0; i<count_of_jump_byte_position; ++i)
        printf("%d ", jump_byte_position[i].address);
    printf("\nret (%d) : ", count_of_ret_byte_position);
    for(int i=0; i<count_of_ret_byte_position; ++i)
        printf("%d (%d),\t", ret_byte_position[i].address, ret_byte_position[i].byte_address);
    printf("\nlabels: (%d) \n", count_of_labels);
    for(int i=0; i<count_of_labels; ++i)
        printf("%d %s\n", labels[i].address, labels[i].name);
    //printf("\nconds: (%d) \n", count_of_cond_byte_position);
    //for(int i=0; i<count_of_cond_byte_position; ++i)
    //    printf("%d %s\n", cond_byte_position[i].address, cond_byte_position[i].name);
    printf("\n\n");

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

    for(int i=0; i<count_of_call_byte_position; ++i) {
        int position = call_byte_position[i].address;

        for(int j=0; j<count_of_labels; ++j) {
            if(!strcmp(call_byte_position[i].name, labels[j].name)) {
                assembled_text[position]     = labels[j].address / 10 + 48;
                assembled_text[position + 1] = labels[j].address % 10 + 48;
            }
        }
    }

    for(int i=0; i<count_of_call_byte_position; ++i) {
        int position = ret_byte_position[i].address;

        printf("ret_b_p[i]: %d, call_b_pos.byte_add = %d\n", ret_byte_position[i].address, call_byte_position[i].byte_address);
        assembled_text[position]     = (call_byte_position[i].byte_address + 1) / 10 + 48;
        assembled_text[position + 1] = (call_byte_position[i].byte_address + 1) % 10 + 48;
    }

    /*printf("jump (%d) : ", count_of_jump_byte_position);
    for(int i=0; i<count_of_jump_byte_position; ++i)
        printf("%d ", jump_byte_position[i].address);
    printf("\nret (%d) : ", count_of_ret_byte_position);
    for(int i=0; i<count_of_ret_byte_position; ++i)
        printf("%d ", ret_byte_position[i].address);
    printf("\n");*/
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
        //printf("find call!\n");
    } else if(!strcmp(text, "ret")) {
        *number_of_condition = OPERATION_CODE_RET;
        //printf("find ret!\n");
    } else {
        *number_of_condition = -1;
        command_is_condition = false;
    }
    printf("num_of_cond: %d\n", *number_of_condition);

    return command_is_condition;
}

int main() {
    assembling_file();
}
