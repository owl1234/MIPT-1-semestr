#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <ctype.h>
#include "common.h"
#include "disassembler.h"

//#define IF_DEBUG(code) code

void disassembling_file(File* input_file, const char* name_output_file) {
    printf("Start disassembling file.........................................\n");
    printf("name_file: %s\n", input_file->name);

    char symbol = '!';
    char temp_string[MAX_SIZE] = {};
    int now_pos_symbol = 0;

    char* disassembled_text = (char*)calloc(2 * MAX_SIZE * input_file->lines, sizeof(char));
    int index_in_disassembled_text = 0;
    double flag_of_the_command = 0, now_command = 0, now_value = 0;
    int put_in_register = -1;

    int now_byte = 0;
    while(now_byte < input_file->information.st_size / sizeof(double)) {
        now_command = input_file->text_for_disassembling[now_byte];
        IF_DEBUG(printf("> now_command: %g (byte: %d) \n", now_command, now_byte);)

       switch((int)now_command) {
            case OPERATION_CODE_HLT:
                put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_HLT], disassembled_text, &index_in_disassembled_text, END_LINE);
                break;
            case OPERATION_CODE_PUSH:
                disassembler_push(input_file, disassembled_text, &index_in_disassembled_text, &now_byte);
                break;
            case OPERATION_CODE_POP:
                disassembler_pop(input_file, disassembled_text, &index_in_disassembled_text, &now_byte);
                break;
            case OPERATION_CODE_ADD:
                put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_ADD], disassembled_text, &index_in_disassembled_text, END_LINE);
                break;
            case OPERATION_CODE_SUB:
                put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_SUB], disassembled_text, &index_in_disassembled_text, END_LINE);
                break;
            case OPERATION_CODE_MUL:
                put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_MUL], disassembled_text, &index_in_disassembled_text, END_LINE);
                break;
            case OPERATION_CODE_DIV:
                put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_DIV], disassembled_text, &index_in_disassembled_text, END_LINE);
                break;
            case OPERATION_CODE_OUT:
                put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_OUT], disassembled_text, &index_in_disassembled_text, END_LINE);
                break;
            case OPERATION_CODE_IN:
                put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_IN], disassembled_text, &index_in_disassembled_text, END_LINE);
                break;
            case OPERATION_CODE_SQRT:
                put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_SQRT], disassembled_text, &index_in_disassembled_text, END_LINE);
                break;
            /*case OPERATION_CODE_JMP:
                put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_JMP], disassembled_text, &index_in_disassembled_text, SPACE);
                put_char_into_disassembled_text("labelll", disassembled_text, &index_in_disassembled_text, END_LINE);
                break;
            case OPERATION_CODE_CMP:
                put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_CMP], disassembled_text, &index_in_disassembled_text, SPACE);
                put_char_into_disassembled_text("labelll", disassembled_text, &index_in_disassembled_text, END_LINE);
                break;
            case OPERATION_CODE_JE:
                put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_JE], disassembled_text, &index_in_disassembled_text, SPACE);
                put_char_into_disassembled_text("labelll", disassembled_text, &index_in_disassembled_text, END_LINE);
                break;*/
            case OPERATION_CODE_MEOW:
                put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_MEOW], disassembled_text, &index_in_disassembled_text, END_LINE);
                break;


            default:
                ERROR("popados (bad command) .....  (╯ ° □ °) ╯ (┻━┻) \n");
        }
        ++now_byte;
    }

    int status = create_disassembling_file(disassembled_text, index_in_disassembled_text, name_output_file);
    if(status != OK) {
        printf("There are problems with the file %s\n", name_output_file);
        return;
    }

    printf("End to disassembling file...........................................\n");
}

void disassembler_push(File* file, char* disassembled_text, int* index_in_disassembled_text, int* now_byte) {
    put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_PUSH], disassembled_text, index_in_disassembled_text, SPACE);
    Elem_t flag_of_the_command = get_double_from_text(file, now_byte);

    Elem_t now_value = get_double_from_text(file, now_byte);
    if(flag_of_the_command == IS_ELEM_T) {
        put_int_into_disassembled_text(now_value, disassembled_text, index_in_disassembled_text, END_LINE);
    } else if(flag_of_the_command == IS_REGISTER) {
        put_char_into_disassembled_text(TEXT_REGISTERS[(int)now_value], disassembled_text, index_in_disassembled_text, END_LINE);
    }
}

void disassembler_pop(File* file, char* disassembled_text, int* index_in_disassembled_text, int* now_byte) {
    Elem_t flag_of_the_command = get_double_from_text(file, now_byte);

    if(flag_of_the_command == IS_ELEM_T) {
        put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_POP], disassembled_text, index_in_disassembled_text, END_LINE);
    } else if(flag_of_the_command == IS_REGISTER) {
        put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_POP], disassembled_text, index_in_disassembled_text, SPACE);
        Elem_t now_value = get_double_from_text(file, now_byte);
        put_char_into_disassembled_text(TEXT_REGISTERS[(int)now_value], disassembled_text, index_in_disassembled_text, END_LINE);
    }
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

void put_int_into_disassembled_text(Elem_t value, char* disassembled_text, int* index_in_disassembled_text, int flag_of_the_end_line) {
    char* temp_string = (char*)calloc(MAX_SIZE, sizeof(char));
    sprintf(temp_string, "%lg", value);
    put_char_into_disassembled_text(temp_string, disassembled_text, index_in_disassembled_text, flag_of_the_end_line);
    free(temp_string);
}

int get_double_from_text(File* file, int* now_byte) {
    ++(*now_byte);
    return file->text_for_disassembling[*now_byte];
}

int create_disassembling_file(const char* disassembled_text, const int index_in_disassembled_text, const char* name_output_file) {
    FILE* output_file = fopen(name_output_file, "w");
    if(output_file == NULL) {
        return ERROR_NUMBER;
    }

    for(int i=0; i<index_in_disassembled_text; ++i)
        printf("%c", disassembled_text[i]);

    //fprintf(output_file, "meow\n");
    //char buffer[] = { 'w' , 't' , 'f' };

    //fwrite(buffer , 1 , sizeof(buffer) , output_file );
    //fwrite(disassembled_text, sizeof(disassembled_text[0]), index_in_disassembled_text, output_file);

    int status = fclose(output_file);
    if(status == EOF) {
        return ERROR_NUMBER;
    }

    return OK;
}

/*
int main() {
    disassembling_file();
}
*/
