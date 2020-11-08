/**
 *  @file
 *  @author Kolesnikova Xenia <heiduk.k.k.s@yandex.ru>
 *  @par Last edition
 *                  November 9, 2020, 01:38:25
 *  @par What was changed?
 *                      1. Made defines for functions
 *  @par To-do list
 *                      1. Fix write to the file
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "common.h"
#include "disassembler.h"

void help() {
    printf("That is my realisarion of disassembler.\n"
           "The required parameters look like this:\n"
                    "[-h]                                                                     - if you want to read this help again\n"
                    "[-d name_input_file_to_disassembler name_output_file_to_disassembler]    - if you want to disassembler file\n"
                    "For more information, go here: https://github.com/owl1234/MIPT_projects_1_sem/tree/master/Freaky_CPU_on_defines\n");
}

#define DEFINE_COMMANDS(name, number, arg, code_processor, code_disassembler, code_assembler)       \
    case number:                                                                    \
        code_disassembler;                                                          \
        break;

int file_construct(File* file, const char* name_file, const char* reading_mode) {
    assert(file);
    assert(name_file);

    file->name = name_file;
    stat(file->name, &(file->information));

    file->input_file = fopen(file->name, reading_mode);
    if(file->input_file == NULL) {
        return DISASM_BAD_FILE;
    }

    file->text_for_disassembling = (double*)calloc(file->information.st_size + 2, sizeof(double));

    if(file->text_for_disassembling == NULL) {
        return DISASM_BAD_MEMORY;
    }

    int status = fread(file->text_for_disassembling, sizeof(char), file->information.st_size, file->input_file);

    if(status != file->information.st_size) {
        return DISASM_BAD_READ_FROM_FILE;
    }

    fclose(file->input_file);

    return OK;
}


DISASM_ERRORS disassembling_file(File* input_file, const char* name_output_file) {
    printf("Start disassembling file.........................................\n");
    printf("name_file: %s\n", input_file->name);

    int now_byte = 0;
    DISASM_ERRORS status = check_signature(input_file, &now_byte);
    if(status != DISASM_OKEY) {
        return status;
    }

    char* disassembled_text = (char*)calloc(3 * MAX_SIZE * input_file->lines, sizeof(char));
    int index_in_disassembled_text = 0;
    double now_command = 0;

    Label* labels = (Label*)calloc(MAX_COUNT_LABELS, sizeof(labels));
    int index_in_labels = 0;

    int real_size = input_file->information.st_size / sizeof(double);

    find_labels_into_text(input_file, labels, &index_in_labels, real_size);

    int now_position_in_labels = 0;

    while(now_byte < real_size) {
        now_command = input_file->text_for_disassembling[now_byte];
        printf("now_pos_in_balels: %d\n", now_position_in_labels);
        printf("%d, label: %d\n", now_byte, labels[now_position_in_labels].byte_address);
        IF_DEBUG(printf("> now_command: %d (byte: %d) \n", (int)now_command, now_byte);)

        switch((int)now_command) {

            #include "COMMANDS.H"

            default:
                printf("popados (bad command) .....  (╯ ° □ °) ╯ (┻━┻) \n");
        }

        printf("now_pos_in_balels: %d\n", now_position_in_labels);
        printf("%d, label: %d\n", now_byte, labels[now_position_in_labels].byte_address);
        if(now_position_in_labels < index_in_labels && now_byte == labels[now_position_in_labels].byte_address) {
            put_char_into_disassembled_text(labels[now_position_in_labels].name, disassembled_text, &index_in_disassembled_text, NOTHING);
            put_char_into_disassembled_text(":", disassembled_text, &index_in_disassembled_text, END_LINE);
            ++now_position_in_labels;
        }


        ++now_byte;
        printf("now_pos_in_balels: %d\n", now_position_in_labels);
        printf("%d, label: %d\n", now_byte, labels[now_position_in_labels].byte_address);
    }

    status = create_disassembling_file(disassembled_text, index_in_disassembled_text, name_output_file);
    if(status != DISASM_OKEY) {
        printf("There are problems with the file %s\n", name_output_file);
        return status;
    }

    printf("End to disassembling file...........................................\n");

    return DISASM_OKEY;
}

DISASM_ERRORS check_signature(File* file, int* now_byte) {

    if(file->text_for_disassembling[(*now_byte)++] > VERSION) {
        return DISASM_BAD_VERSION;
    }

    if(file->text_for_disassembling[(*now_byte)++] != SIGNATURE_NAME_HASH) {
        return DISASM_BAD_VERSION;
    }

    return DISASM_OKEY;
}

void find_labels_into_text(File* input_file, Label* labels, int* index_in_labels, int real_size) {
    int number_of_byte = 0;

    int now_command = input_file->text_for_disassembling[number_of_byte], number_of_condition = 0;

    while(number_of_byte < real_size) {
        now_command = input_file->text_for_disassembling[number_of_byte];
        if(is_code_connected_with_labels(now_command, &number_of_condition)) {
            labels[*index_in_labels].name = (char*)calloc(MAX_SIZE, sizeof(char));
            strcpy(labels[*index_in_labels].name, "label");

            char* number_of_label_byte = (char*)calloc(length_of_number(*index_in_labels) + 1, sizeof(char));
            sprintf(number_of_label_byte, "%d", *index_in_labels);

            strcat(labels[*index_in_labels].name, number_of_label_byte);

            now_command = input_file->text_for_disassembling[++number_of_byte];
            labels[(*index_in_labels)++].byte_address = now_command;

            free(number_of_label_byte);
        }
        ++number_of_byte;
    }

    for(int i=0; i<*index_in_labels; ++i)
        printf("%s %d\n", labels[i].name, labels[i].byte_address);

}

bool is_code_connected_with_labels(int command, int* number_of_condition) {
    int command_is_condition = true;

    switch(command) {
        case OPERATION_CODE_JMP:
            *number_of_condition = OPERATION_CODE_JMP;
            break;
        case OPERATION_CODE_JE:
            *number_of_condition = OPERATION_CODE_JE;
            break;
        case OPERATION_CODE_JNE:
            *number_of_condition = OPERATION_CODE_JNE;
            break;
        case OPERATION_CODE_JB:
            *number_of_condition = OPERATION_CODE_JB;
            break;
        case OPERATION_CODE_JBE:
            *number_of_condition = OPERATION_CODE_JBE;
            break;
        case OPERATION_CODE_JA:
            *number_of_condition = OPERATION_CODE_JA;
            break;
        case OPERATION_CODE_JAE:
            *number_of_condition = OPERATION_CODE_JAE;
            break;
        case OPERATION_CODE_CALL:
            *number_of_condition = OPERATION_CODE_CALL;
            break;
        default:
            command_is_condition = false;
    }

    return command_is_condition;
}

void put_char_into_disassembled_text(const char* command, char* disassembled_text, int* index_in_disassembled_text, int flag_of_the_end_line) {
    printf("\t\t%s\n", command);
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

    if(flag_of_the_end_line != NOTHING) {
        ++(*index_in_disassembled_text);
    }printf("\t\t%s\n", command);
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

DISASM_ERRORS create_disassembling_file(const char* disassembled_text, const int index_in_disassembled_text, const char* name_output_file) {
    printf("out: %s\n", name_output_file);
    FILE* output_file = fopen(name_output_file, "w");
    if(output_file == NULL) {
        return DISASM_BAD_FILE;
    }

    for(int i=0; i<index_in_disassembled_text; ++i)
        printf("%c", disassembled_text[i]);

    //printf("%d\n", index_in_disassembled_text);
    //fwrite(disassembled_text, sizeof(char), index_in_disassembled_text, output_file);

    int status = fclose(output_file);
    if(status == EOF) {
        return DISASM_BAD_FILE;
    }

    return DISASM_OKEY;
}

int main(const int argc, const char* argv[]) {
    assert(argc > 0);

    int status = OK;

    if(argc == 3) {
        File file = {};

        status = file_construct(&file, argv[1], "rb");
        if(status == OK) {
            disassembling_file(&file, argv[2]);
        } else {
            printf("The program was stopped.\n");
            return status;
        }
    } else {
        help();
    }

    return OK;
}

