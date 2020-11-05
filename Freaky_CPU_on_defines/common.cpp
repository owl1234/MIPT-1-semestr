#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "common.h"

void ERROR(const char* status) {
    printf("%s\n", status);
}

int is_right_command(const char* line, const char* command) {
    int length_command = strlen(command);

    for(int i=0; i<length_command; ++i) {
        if(command[i] != line[i]) {
            return 1;
        }
    }

    return 0;
}

int number_of_symbols(char* buffer, char separator) {
    char now_symbol = *buffer;
    int number = 0;

    while(now_symbol != '\0') {
        now_symbol = *buffer;
        if(now_symbol == separator) {
            ++number;
        }

        ++buffer;
    }

    return number;
}

int file_construct(File* file, const char* name_file, const char* reading_mode) {
    assert(file);
    assert(name_file);

    file->name = name_file;
    stat(file->name, &(file->information));

    file->ptr_to_file = fopen(file->name, reading_mode);
    int status = 0;

    if(!strcmp(reading_mode, "r")) {
        file->text_for_assembling    = (char*)calloc(file->information.st_size + 2, sizeof(char));
        file->text_for_disassembling = nullptr;
        status = fread(file->text_for_assembling, sizeof(char), file->information.st_size, file->ptr_to_file);
        file->lines = number_of_symbols(file->text_for_assembling, '\n');
    } else if(!strcmp(reading_mode, "rb")) {
        file->text_for_assembling    = nullptr;
        file->text_for_disassembling = (double*)calloc(file->information.st_size + 2, sizeof(double));
        status = fread(file->text_for_disassembling, sizeof(double), file->information.st_size, file->ptr_to_file);
        //file->lines = number_of_symbols(file->text_for_disassembling, '\n');
    }

    if(status * sizeof(char)   != file->information.st_size && !strcmp(reading_mode, "r") ||
       status * sizeof(double) != file->information.st_size && !strcmp(reading_mode, "rb")) {
        printf("File don't read normal! (%d %d)\n", status * 8, file->information.st_size);
        return ERROR_NUMBER;
    }

    fclose(file->ptr_to_file);

    return OK;
}


int type_of_value(const char* operation) {
    int length = strlen(operation);
    int flag_of_value = 0;

    for(int registr=0; registr<number_of_register_vars; ++registr) {
        if(!strcmp(TEXT_REGISTERS[registr], operation)) {
            return IS_REGISTER;
        }
    }

    if(!(operation[0] == '[' && operation[length-1] == ']')) {
        return IS_ELEM_T;
    }

    char* ram_address = (char*)calloc(length - 1, sizeof(char));

    for(int i=1; i<length-1; ++i) {
        ram_address[i-1] = operation[i];
    }

    for(int registr=0; registr<number_of_register_vars; ++registr) {
        if(!strcmp(TEXT_REGISTERS[registr], ram_address)) {
            return (IS_RAM | IS_REGISTER);
        }
    }

    return (IS_ELEM_T | IS_RAM);
}

int get_number_of_register(const char* text) {
    for(int registr=0; registr<number_of_register_vars; ++registr) {
        if(!strcmp(TEXT_REGISTERS[registr], text) || !strcmp(RAM_TEXT_REGISTERS[registr], text)) {
            return registr;
        }
    }

    return ERROR_NUMBER;
}
