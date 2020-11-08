/**
 *  @file
 *  @author Kolesnikova Xenia <heiduk.k.k.s@yandex.ru>
 *  @par Last edition
 *                  November 8, 2020, 22:45:25
 *  @par What was changed?
 *                      1. Add signature
 *  @par To-do list
 *                      2. Make adequate listing !!!
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <unistd.h>
#include "assembler.h"
#include "common.h"

void help() {
    printf("That is my realisarion of assembler.\n"
           "The required parameters look like this:\n"
                    "[-h]                                                               - if you want to read this help again\n"
                    "[-a name_input_file_to_assembler name_output_file_to_assembler]    - if you want to assembler file\n"
                    "For more information, go here: https://github.com/owl1234/MIPT_projects_1_sem/tree/master/Freaky_CPU_on_defines\n");
}

int file_construct(File* file, char* name_file, const char* reading_mode) {
    assert(file);
    assert(name_file);

    file->name = name_file;
    stat(file->name, &(file->information));

    file->input_file = fopen(file->name, reading_mode);
    if(file->input_file == NULL) {
        return ASM_BAD_FILE;
    }

    file->text_for_assembling = (char*)calloc(file->information.st_size + 2, sizeof(char));
    file->text_for_listing    = (char*)calloc(file->information.st_size + 2, sizeof(char));

    if(file->text_for_assembling == NULL || file->text_for_listing == NULL) {
        return ASM_BAD_MEMORY;
    }

    int status = fread(file->text_for_assembling, sizeof(char), file->information.st_size, file->input_file);

    if(status != file->information.st_size) {
        return ASM_BAD_READ_FROM_FILE;
    }

    fclose(file->input_file);

    file->listing_file = fopen("listing.txt", "wb");
    if(file->listing_file == NULL) {
        return ASM_BAD_FILE;
    }

    return OK;
}

void listing(File* file, const char* symbols_to_output, const int flag_of_the_end) {
    fprintf(file->listing_file, "%s%s", symbols_to_output, TEXT_FLAGS_OF_THE_END_LINE[flag_of_the_end]);
}

void listing_alignment(File* file, const int number_to_output, const int flag_of_the_end) {
    fprintf(file->listing_file, "%#08d%s", number_to_output, TEXT_FLAGS_OF_THE_END_LINE[flag_of_the_end]);
}

void listing(File* file, const int flag_of_the_end, int count_of_args, ...) {
    va_list argptr;
    va_start(argptr, count_of_args);

    while(count_of_args > 0) {
        fprintf(file->listing_file, "%d ", va_arg(argptr, int));
        --count_of_args;
    }

    va_end(argptr);

    fprintf(file->listing_file, "%s", TEXT_FLAGS_OF_THE_END_LINE[flag_of_the_end]);
}

void listing(File* file, const int flag_of_the_end, double count_of_args, ...) {
    va_list argptr;
    va_start(argptr, count_of_args);

    int int_count_of_args = (int)count_of_args;
    while(int_count_of_args > 0) {
        fprintf(file->listing_file, "%lg ", va_arg(argptr, double));
        --int_count_of_args;
    }

    va_end(argptr);

    fprintf(file->listing_file, "%s", TEXT_FLAGS_OF_THE_END_LINE[flag_of_the_end]);
}

void listing(File* file, char symbol_to_output, int flag_of_the_end) {
    fprintf(file->listing_file, "%c%s", symbol_to_output, TEXT_FLAGS_OF_THE_END_LINE[flag_of_the_end]);
}

int assembling_file(File* input_file, const char* name_output_file) {
    printf("Start assembling file.........................................\n");

    char* assembled_text = (char*)calloc(3 * MAX_SIZE * input_file->information.st_size + SIGNATURE_SIZE, sizeof(char));       // TO-DO CORRECT SIZE
    int index_in_assembled_text = 0, number_of_byte = 0;

    write_signature(assembled_text, &index_in_assembled_text, &number_of_byte);

    Label* labels = (Label*)calloc(MAX_COUNT_LABELS, sizeof(Label));

    int status = 0, index_in_labels = 0;

    find_labels_into_text(input_file, labels, &index_in_labels, &number_of_byte);

    char* temp_string = (char*)calloc(MAX_SIZE, sizeof(char));
    temp_string = strtok(input_file->text_for_assembling, SEPARATORS);

    listing_alignment(input_file, number_of_byte, SPACE);
    listing(input_file, "|", SPACE);

    while(temp_string != NULL) {
        //printf("now_command: %s (%d)\n", temp_string, number_of_byte);
        status = find_and_write_command(temp_string, assembled_text, &index_in_assembled_text, labels, index_in_labels, &number_of_byte, input_file);

        if(!is_it_label(temp_string)) {
            temp_string = strtok(NULL, SEPARATORS);

            if(temp_string == NULL) {
                break;
            }

            listing_alignment(input_file, number_of_byte, SPACE);
            listing(input_file, "|", SPACE);
        } else {
            temp_string = strtok(NULL, SEPARATORS);
        }
    }

    fclose(input_file->listing_file);

    status = create_assembling_file(assembled_text, index_in_assembled_text, name_output_file);
    if(status != OK) {
        printf("There are problems with the file %s\n", name_output_file);
        return ASM_BAD_FILE;
    }

    printf("Assembling file finished...........................................\n");
    return OK;
}

void write_signature(char* assembled_text, int* index_in_assembled_text, int* number_of_byte) {
    put_int_into_assembled_text(VERSION, assembled_text, index_in_assembled_text, number_of_byte);
    put_int_into_assembled_text(SIGNATURE_NAME_HASH, assembled_text, index_in_assembled_text, number_of_byte);
}

void find_labels_into_text(File* input_file, Label* labels, int* index_in_labels, int* number_of_byte) {
    int copy_number_of_byte = *number_of_byte;

    char* copy_of_text_for_assembling = (char*)calloc(input_file->information.st_size + 2, sizeof(char));
    strcpy(copy_of_text_for_assembling, input_file->text_for_assembling);

    char* temp_string = (char*)calloc(MAX_SIZE, sizeof(char));
    char* prev_string = (char*)calloc(MAX_SIZE, sizeof(char));
    temp_string = strtok(input_file->text_for_assembling, SEPARATORS);

    while(temp_string != NULL) {
        if(is_it_label(temp_string)) {
            labels[*index_in_labels].name = (char*)calloc(strlen(temp_string), sizeof(char));
            strcpy(labels[*index_in_labels].name, temp_string);
            labels[*index_in_labels].byte_address = copy_number_of_byte;
            ++(*index_in_labels);

        } else if(!strcmp("push", temp_string)) {
            copy_number_of_byte += NUMBER_ARGUMENTS_FOR_OPERATION[OPERATION_CODE_PUSH];
        } else if(!strcmp("pop", temp_string)) {
            copy_number_of_byte += NUMBER_ARGUMENTS_FOR_OPERATION[OPERATION_CODE_POP] + 1;
        } else if(!strcmp("cmp", temp_string)) {
            copy_number_of_byte += NUMBER_ARGUMENTS_FOR_OPERATION[OPERATION_CODE_CMP] - 1;
        } else {
            ++copy_number_of_byte;
        }


        strcpy(prev_string, temp_string);
        temp_string = strtok(NULL, SEPARATORS);
    }

    strcpy(input_file->text_for_assembling, copy_of_text_for_assembling);

    free(temp_string);
    free(prev_string);
}

bool is_it_label(const char* word) {
    int length = strlen(word);
    if(word[length - 1] == ':') {
        return true;
    }

    return false;
}

int find_and_write_command(char* text, char* assembled_text, int* index_in_assembled_text, Label* labels, int index_in_labels, int* number_of_byte, File* listing_file) { // TO-DO DEFINES
    int number_of_condition = 0;

    if(!strcmp(text, "push")) {
        assembler_push(text, assembled_text, index_in_assembled_text, number_of_byte, listing_file);
    } else if(!strcmp(text, "pop")) {
        assembler_pop(text, assembled_text, index_in_assembled_text, labels, index_in_labels, number_of_byte, listing_file);
    } else if(!strcmp(text, "hlt")) {
        put_int_into_assembled_text(OPERATION_CODE_HLT, assembled_text, index_in_assembled_text, number_of_byte);

        listing(listing_file, NOTHING, ONE_ARGUMENT, OPERATION_CODE_HLT);
        listing(listing_file, "| hlt", END_LINE);
    } else if(is_text_connected_with_labels(text, &number_of_condition)) {
        assembler_labels(text, assembled_text, index_in_assembled_text, number_of_byte, labels, index_in_labels, number_of_condition, listing_file);
    } else if(!strcmp(text, "cmp")) {
        assembler_cmp(text, assembled_text, index_in_assembled_text, number_of_byte, listing_file);
    } else if(!strcmp(text, "meow")) {
        assembler_meow(assembled_text, index_in_assembled_text, number_of_byte, listing_file);
    } else if(!strcmp(text, "sqrt")) {
        assembler_sqrt(assembled_text, index_in_assembled_text, number_of_byte, listing_file);
    } else {
        for(int operation_code = OPERATION_CODE_ADD; operation_code<number_of_commands; ++operation_code) {
            if(!strcmp(text, TEXT_OPERATION[operation_code])) {
                put_int_into_assembled_text(operation_code, assembled_text, index_in_assembled_text, number_of_byte);

                listing(listing_file, NOTHING, ONE_ARGUMENT, operation_code);
                listing(listing_file, "|", SPACE);
                listing(listing_file, TEXT_OPERATION[operation_code], END_LINE);
                return OK;
            }
        }

        return ASM_BAD_COMMAND;
    }

    return OK;

}

void assembler_push(char* text, char* assembled_text, int* index_in_assembled_text, int* number_of_byte, File* listing_file) {
    put_int_into_assembled_text(OPERATION_CODE_PUSH, assembled_text, index_in_assembled_text, number_of_byte);

    text = strtok(NULL, SEPARATORS);

    int type_push_value = type_of_value(text);
    put_int_into_assembled_text(type_push_value, assembled_text, index_in_assembled_text, number_of_byte);


    if(type_push_value == IS_REGISTER || type_push_value == (IS_RAM | IS_REGISTER)) {

        put_int_into_assembled_text(get_number_of_register(text), assembled_text, index_in_assembled_text, number_of_byte);

        listing(listing_file, NOTHING, THREE_AGRUMENTS, OPERATION_CODE_PUSH, type_push_value, get_number_of_register(text));
        listing(listing_file, "| push", SPACE);
        listing(listing_file, text, END_LINE);

    } else if(type_push_value == IS_ELEM_T || type_push_value == (IS_RAM | IS_ELEM_T)) {
        if(type_push_value == (IS_RAM | IS_ELEM_T)) {
            text = strchr(text, '[') + 1;
            reverse_string(text);
            text = strchr(text, ']') + 1;
            reverse_string(text);
        }

        double now_value = strtod(text, NULL);
        put_double_into_assembled_text(now_value, assembled_text, index_in_assembled_text, number_of_byte);

        listing(listing_file, NOTHING, TWO_ARGUMENTS, OPERATION_CODE_PUSH, type_push_value);
        listing(listing_file, NOTHING, (double)ONE_ARGUMENT, now_value);
        listing(listing_file, "| push ", NOTHING);
        listing(listing_file, END_LINE, (double)ONE_ARGUMENT, now_value);

    } else {
        printf("Bad type of push value! (%d)\n", type_push_value);
        return;
    }
}

void assembler_pop(char* text, char* assembled_text, int* index_in_assembled_text, Label* labels, int index_in_labels, int* number_of_byte, File* listing_file) {

    put_int_into_assembled_text(OPERATION_CODE_POP, assembled_text, index_in_assembled_text, number_of_byte);

    text = strtok(NULL, SEPARATORS);

    if(type_of_value(text) == IS_REGISTER) {

        put_int_into_assembled_text(IS_REGISTER, assembled_text, index_in_assembled_text, number_of_byte);
        put_int_into_assembled_text(get_number_of_register(text), assembled_text, index_in_assembled_text, number_of_byte);

        listing(listing_file, NOTHING, THREE_AGRUMENTS, OPERATION_CODE_POP, IS_REGISTER, get_number_of_register(text));
        listing(listing_file, "| pop ", NOTHING);
        listing(listing_file, text, END_LINE);

    } else {

        put_int_into_assembled_text(NOT_ARGS, assembled_text, index_in_assembled_text, number_of_byte);

        listing(listing_file, NOTHING, TWO_ARGUMENTS, OPERATION_CODE_POP, NOT_ARGS);
        listing(listing_file, "| pop", END_LINE);

        listing_alignment(listing_file, *number_of_byte, SPACE);
        listing(listing_file, "| ", NOTHING);

        find_and_write_command(text, assembled_text, index_in_assembled_text, labels, index_in_labels, number_of_byte, listing_file);

    }
}

void assembler_cmp(char* text, char* assembled_text, int* index_in_assembled_text, int* number_of_byte, File* listing_file) {
    put_int_into_assembled_text(OPERATION_CODE_CMP, assembled_text, index_in_assembled_text, number_of_byte);

    int first_type_of_value = 0, second_type_of_value = 0;

    char* first_string = (char*)calloc(MAX_SIZE, sizeof(char));
    char* second_string = (char*)calloc(MAX_SIZE, sizeof(char));

    int first_value  = put_cmp_value(text, assembled_text, index_in_assembled_text, number_of_byte, first_string,  &first_type_of_value);
    int second_value = put_cmp_value(text, assembled_text, index_in_assembled_text, number_of_byte, second_string, &second_type_of_value);

    listing(listing_file, NOTHING, FIVE_ARGUMENTS, OPERATION_CODE_CMP, first_type_of_value, first_value, second_type_of_value, second_value);
    listing(listing_file, "|", SPACE);
    listing(listing_file, "cmp", SPACE);
    listing(listing_file, first_string, SPACE);
    listing(listing_file, second_string, END_LINE);
}

void assembler_meow(char* assembled_text, int* index_in_assembled_text, int* number_of_byte, File* listing_file) {
    put_int_into_assembled_text(OPERATION_CODE_MEOW, assembled_text, index_in_assembled_text, number_of_byte);

    listing(listing_file, NOTHING, ONE_ARGUMENT, OPERATION_CODE_MEOW);
    listing(listing_file, "| meow", END_LINE);
}

void assembler_sqrt(char* assembled_text, int* index_in_assembled_text, int* number_of_byte, File* listing_file) {
    put_int_into_assembled_text(OPERATION_CODE_SQRT, assembled_text, index_in_assembled_text, number_of_byte);

    listing(listing_file, NOTHING, ONE_ARGUMENT, OPERATION_CODE_SQRT);
    listing(listing_file, "| sqrt", END_LINE);
}

void assembler_labels(char* text, char* assembled_text, int* index_in_assembled_text, int* number_of_byte, Label* labels, int index_in_labels, int number_of_condition,
                                                                                                                                                            File* listing_file) {

    if(strcmp(text, "ret")) {
        put_int_into_assembled_text(number_of_condition, assembled_text, index_in_assembled_text, number_of_byte);

        text = strtok(NULL, SEPARATORS);
        if(!strcmp(text, "recret")) {
            put_int_into_assembled_text(OPERATION_CODE_RECURSIVE_RET, assembled_text, index_in_assembled_text, number_of_byte);

            listing(listing_file, NOTHING, TWO_ARGUMENTS, number_of_condition, OPERATION_CODE_RECURSIVE_RET);
            listing(listing_file, "|", SPACE);
            listing(listing_file, TEXT_OPERATION[number_of_condition], SPACE);
            listing(listing_file, text, END_LINE);
        } else {
            for(int label=0; label<index_in_labels; ++label) {
                if(is_equal_labels(text, labels[label].name)) {
                    put_int_into_assembled_text(labels[label].byte_address + 1, assembled_text, index_in_assembled_text, number_of_byte);

                    listing(listing_file, NOTHING, TWO_ARGUMENTS, number_of_condition, labels[label].byte_address);
                    listing(listing_file, "|", SPACE);
                    listing(listing_file, TEXT_OPERATION[number_of_condition], SPACE);
                    listing(listing_file, text, END_LINE);
                    break;
                }
            }
        }
    } else {
        put_int_into_assembled_text(number_of_condition, assembled_text, index_in_assembled_text, number_of_byte);

        listing(listing_file, NOTHING, ONE_ARGUMENT, OPERATION_CODE_RET);
        listing(listing_file, "|", SPACE);
        listing(listing_file, TEXT_OPERATION[OPERATION_CODE_RET], END_LINE);
    }
}

bool is_equal_labels(const char* first, const char* second) {
    //printf("\t%s\n", second);
    int length_first  = strlen(first);
    int length_second = strlen(second);

    if(abs(length_first - length_second) > 1) {
        return false;
    }

    int length = max(length_first, length_second) - 1;

    for(int i=0; i<length; ++i) {
        if(first[i] != second[i]) {
            return false;
        }
    }

    return true;
}

int max(int first, int second) {
    if(first > second) {
        return first;
    }

    return second;
}

int put_cmp_value(char* text, char* assembled_text, int* index_in_assembled_text, int* number_of_byte, char* argument, int* type_of_argument) {
    text = strtok(NULL, SEPARATORS);
    strcpy(argument, text);

    *type_of_argument = type_of_value(text);
    put_int_into_assembled_text(*type_of_argument, assembled_text, index_in_assembled_text, number_of_byte);

    if(*type_of_argument == IS_REGISTER) {
        put_int_into_assembled_text(get_number_of_register(text), assembled_text, index_in_assembled_text, number_of_byte);
        return get_number_of_register(text);
    } else {
        int now_value = strtod(text, NULL);
        put_double_into_assembled_text(now_value, assembled_text, index_in_assembled_text, number_of_byte);
        return now_value;
    }
}

void put_int_into_assembled_text(int code_of_operation, char* assembled_text, int* index_in_assembled_text, int* number_of_byte) {
    double converted_code = (double)code_of_operation;

    memcpy(assembled_text + *index_in_assembled_text, &converted_code, sizeof(double)); //
    *index_in_assembled_text += sizeof(double);

    ++(*number_of_byte);
}

void put_int_into_assembled_text(long long code_of_operation, char* assembled_text, int* index_in_assembled_text, int* number_of_byte) {
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
    labels[*index_in_labels].byte_address = number_of_byte;
    labels[*index_in_labels].type_of_command = -1;

    ++(*index_in_labels);
}

int create_assembling_file(const char* assembled_text, const int index_in_assembled_text, const char* name_output_file) {
    FILE* output_file = fopen(name_output_file, "wb");
    if(output_file == NULL) {
        return ASM_BAD_FILE;
    }

    fwrite(assembled_text, sizeof(assembled_text[0]), index_in_assembled_text, output_file);

    int status = fclose(output_file);
    if(status == EOF) {
        return ASM_BAD_FILE;
    }

    return OK;
}

void reverse_string(char* text) {
    int length = strlen(text);
    char* copy_of_text = (char*)calloc(length, sizeof(char));

    for(int i=0; i<length; ++i) {
        copy_of_text[i] = text[length - i - 1];
    }

    strcpy(text, copy_of_text);
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

ASM_ERRORS destruct_file(File* file) {
    if(!file->text_for_assembling) {
        return ASM_BAD_MEMORY;
    }

    free(file->text_for_assembling);

    if(!file->text_for_listing) {
        return ASM_BAD_MEMORY;
    }

    free(file->text_for_listing);

    return ASM_OKEY;
}

int main(const int argc, char* argv[]) {
    assert(argc > 0);

    int status = OK;

    if(argc == 3) {
        File file = {};

        status = file_construct(&file, argv[1], "r");
        if(status == OK) {
            assembling_file(&file, argv[2]);
        } else {
            printf("The program was stopped.\n");
            return status;
        }

        destruct_file(&file);
    } else {
        help();
    }

    return OK;
}
