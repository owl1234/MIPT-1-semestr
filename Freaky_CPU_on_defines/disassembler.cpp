/**
 *  @file
 *  @author Kolesnikova Xenia <heiduk.k.k.s@yandex.ru>
 *  @par Last edition
 *                  November 10, 2020, 20:12:25
 *  @par What was changed?
 *                      1. Add new defines
 *  @par To-do list
 *                      1. Fix write to the file
 *                      2. Fix crash of disassembler
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <unistd.h>
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
    case number: {                                                                                  \
        code_disassembler;                                                                          \
        break;                                                                                      \
    }

DISASM_ERRORS file_construct(File* file, const char* name_file, const char* reading_mode) {
    assert(file);
    assert(name_file);

    file->name = name_file;
    stat(file->name, &(file->information));

    file->input_file = fopen(file->name, reading_mode);
    if(file->input_file == NULL) {
        return DISASM_BAD_FILE;
    }

    file->text_for_disassembling = (double*)calloc(file->information.st_size + 2, sizeof(double));

    if(!file->text_for_disassembling) {
        return DISASM_BAD_MEMORY;
    }

    int status = fread(file->text_for_disassembling, sizeof(char), file->information.st_size, file->input_file);

    if(status != file->information.st_size) {
        return DISASM_BAD_READ_FROM_FILE;
    }

    fclose(file->input_file);

    return DISASM_OKEY;
}

DISASM_ERRORS destruct_file(File* file) {
    if(!file->text_for_disassembling) {
        return DISASM_BAD_MEMORY;
    }

    free(file->text_for_disassembling);

    return DISASM_OKEY;
}

void print_labels(Label* labels, int length) {
    //printf("%p\n", labels);
    for(int i=0;i<length; ++i) {
        printf("\t%s %d\n", labels[i].name, labels[i].byte_address);
    }
    printf("\n");
}

DISASM_ERRORS create_disassembler_struct(File* input_file, struct disassembler_struct* disasm_struct) {
    disasm_struct->disassembled_text = (char*)calloc(3 * MAX_SIZE * input_file->lines, sizeof(char));
    if(!disasm_struct->disassembled_text) {
        return DISASM_BAD_MEMORY;
    }

    disasm_struct->index_in_disassembled_text = 0;
    disasm_struct->now_byte = 0;

    disasm_struct->labels = (Label*)calloc(MAX_COUNT_LABELS, sizeof(Label));
    if(!disasm_struct->labels) {
        return DISASM_BAD_MEMORY;
    }

    disasm_struct->index_in_labels = 0;
    disasm_struct->now_position_in_labels = 0;

    disasm_struct->real_size = input_file->information.st_size / sizeof(double);

    return DISASM_OKEY;
}

DISASM_ERRORS disassembling_file(File* input_file, const char* name_output_file) {
    printf("Start disassembling file.........................................\n");
    printf("name_file: %s\n", input_file->name);

    struct disassembler_struct disasm_struct = {};
    DISASM_ERRORS status = create_disassembler_struct(input_file, &disasm_struct);

    status = check_signature(input_file, &disasm_struct);
    if(status != DISASM_OKEY) {
        return status;
    }

    find_labels_into_text(input_file, &disasm_struct);

    double now_command = 0;

    print_labels(disasm_struct.labels, disasm_struct.index_in_labels);
    printf("\n%d\n", disasm_struct.now_position_in_labels);
    while(disasm_struct.now_byte < disasm_struct.real_size) {
        now_command = input_file->text_for_disassembling[disasm_struct.now_byte];
        IF_DEBUG(printf("> now_command: %d (byte: %d) (%p)\n", (int)now_command, disasm_struct.now_byte, disasm_struct.labels);)

        switch((int)now_command) {

            #include "COMMANDS.H"

            default:
                printf("popados (bad command) .....  (╯ ° □ °) ╯ (┻━┻) \n");
        }
        printf("%d\n", disasm_struct.now_position_in_labels);
        if(disasm_struct.now_position_in_labels < disasm_struct.index_in_labels && disasm_struct.now_byte == disasm_struct.labels[disasm_struct.now_position_in_labels].byte_address) {
            put_char_into_disassembled_text(disasm_struct.labels[disasm_struct.now_position_in_labels].name, &disasm_struct, NOTHING);
            put_char_into_disassembled_text(":", &disasm_struct, END_LINE);
            ++(disasm_struct.now_position_in_labels);
            printf("!!! %d\n", disasm_struct.now_position_in_labels);
        }

        ++(disasm_struct.now_byte);
        print_labels(disasm_struct.labels, disasm_struct.index_in_labels);
    }

    status = create_disassembling_file(&disasm_struct, name_output_file);
    if(status != DISASM_OKEY) {
        printf("There are problems with the file %s\n", name_output_file);
        return status;
    }

    printf("End to disassembling file...........................................\n");

    return DISASM_OKEY;
}

DISASM_ERRORS check_signature(File* file, struct disassembler_struct* disasm_struct) {
    if(file->text_for_disassembling[(disasm_struct->now_byte)++] > VERSION) {
        return DISASM_BAD_VERSION;
    }

    if(file->text_for_disassembling[(disasm_struct->now_byte)++] != SIGNATURE_NAME_HASH) {
        return DISASM_BAD_VERSION;
    }

    return DISASM_OKEY;
}

void find_labels_into_text(File* input_file, struct disassembler_struct* disasm_struct) {
    int number_of_byte = 0;

    int now_command = input_file->text_for_disassembling[number_of_byte], number_of_condition = 0;

    while(number_of_byte < disasm_struct->real_size) {
        now_command = input_file->text_for_disassembling[number_of_byte];
        //printf("now_byte: %d\n", number_of_byte);
        if(is_code_connected_with_labels(now_command, &number_of_condition)) {
            disasm_struct->labels[disasm_struct->index_in_labels].name = (char*)calloc(MAX_SIZE, sizeof(char));
            strcpy(disasm_struct->labels[disasm_struct->index_in_labels].name, "label");

            char* number_of_label_byte = (char*)calloc(length_of_number(disasm_struct->index_in_labels) + 1, sizeof(char));
            sprintf(number_of_label_byte, "%d", disasm_struct->index_in_labels);

            strcat(disasm_struct->labels[disasm_struct->index_in_labels].name, number_of_label_byte);
            //printf("I find new label! %s\n", disasm_struct->labels[disasm_struct->index_in_labels]);

            now_command = input_file->text_for_disassembling[++number_of_byte];
            disasm_struct->labels[disasm_struct->index_in_labels].byte_address = now_command;
            disasm_struct->index_in_labels++;
            free(number_of_label_byte);
        }
        ++number_of_byte;
        //sleep(2);
    }
        print_labels(disasm_struct->labels, disasm_struct->index_in_labels);

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

void put_char_into_disassembled_text(const char* command, struct disassembler_struct* disasm_struct, int flag_of_the_end_line) {
    int length_command = strlen(command);

    print_labels(disasm_struct->labels, disasm_struct->index_in_labels);
    for(int i=0; i<length_command; ++i) {
        disasm_struct->disassembled_text[disasm_struct->index_in_disassembled_text] = command[i];
        ++(disasm_struct->index_in_disassembled_text);
    }

    if(flag_of_the_end_line == END_LINE) {
        disasm_struct->disassembled_text[(disasm_struct->index_in_disassembled_text)++] = '\n';
    } else {
        disasm_struct->disassembled_text[(disasm_struct->index_in_disassembled_text)++] = ' ';
    }

    //printf("index_in_dasasm_text: %d\n", *index_in_disassembled_text);
    if(flag_of_the_end_line != NOTHING) {
        ++(disasm_struct->index_in_disassembled_text);
    }

}

void put_int_into_disassembled_text(Elem_t value, struct disassembler_struct* disasm_struct, int flag_of_the_end_line) {
    char* temp_string = (char*)calloc(MAX_SIZE, sizeof(char));
    sprintf(temp_string, "%lg", value);
    put_char_into_disassembled_text(temp_string, disasm_struct, flag_of_the_end_line);
    free(temp_string);
}

double get_double_from_text(File* file, struct disassembler_struct* disasm_struct) {
    ++(disasm_struct->now_byte);
    return file->text_for_disassembling[disasm_struct->now_byte];
}

DISASM_ERRORS create_disassembling_file(struct disassembler_struct* disasm_struct, const char* name_output_file) {
    printf("out: %s\n", name_output_file);
    FILE* output_file = fopen(name_output_file, "w");
    if(output_file == NULL) {
        return DISASM_BAD_FILE;
    }

    //printf("Be happy %d\n", disasm_struct->index_in_labels);
    for(int i=0; i<disasm_struct->index_in_disassembled_text; ++i)
        fprintf(output_file, "%c", disasm_struct->disassembled_text[i]);

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

        status = file_construct(&file, argv[1], "r");
        for(int i=0; i<file.information.st_size; ++i)
            printf("%lg ", file.text_for_disassembling[i]);
        if(status == OK) {
            disassembling_file(&file, argv[2]);
        } else {
            printf("The program was stopped (%s).\n", TEXT_DISASM_ERRORS[status]);
            return status;
        }

        status = destruct_file(&file);
        if(status != OK) {
            printf("The program was stopped (%s).\n", TEXT_DISASM_ERRORS[status]);
            return status;
        }
        printf("%s\n", TEXT_DISASM_ERRORS[status]);
    } else {
        help();
    }

    return status;
}

