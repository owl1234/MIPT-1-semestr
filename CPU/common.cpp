#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "common.h"

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

int file_construct(File* file, const char* name_file) {
    assert(file);
    assert(name_file);

    file->name = name_file;
    stat(file->name, &(file->information));

    file->text         = (char*)calloc(file->information.st_size + 2, sizeof(char));
    file->copy_of_text = (char*)calloc(file->information.st_size + 2, sizeof(char));

    file->ptr_to_file = fopen(file->name, "r");
    int status = fread(file->text, sizeof(char), file->information.st_size, file->ptr_to_file);
    if(status != file->information.st_size) {
        printf("File don't read normal!\n");
        return ERROR_NUMBER;
    }

    strcpy(file->copy_of_text, file->text);
    fclose(file->ptr_to_file);

    file->lines = number_of_symbols(file->text, '\n');
    return OK_FILE;
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
