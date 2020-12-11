#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "work_with_file.h"

#define ASSERTION(status)                                                                                                                    \
    system("echo \e[31m-----------------!WARNING!----------------\e[0m");                                                                    \
    char warning_info[150] = "";                                                                                                             \
    sprintf(warning_info, "echo \"\\e[31mIN FILE %s (FUNCTION %s, LINE %d)\\e[0m\"", __FILE__, __FUNCTION__, __LINE__);                      \
    system(warning_info);                                                                                                                    \
    sprintf(warning_info, "echo \"\\e[31mFile status: %s\\e[0m\"", TEXT_FILE_STATUS[status]);                                                               \
    system(warning_info);

File* file_construct(char* file_name) {
    File* input_file = {};

    input_file->name_file   = file_name;
    input_file->buffer      = NULL;
    input_file->size_buffer = 0;

    return input_file;
}

FILE_STATUS read_buffer(struct File* input_file) {

    //stat(input_file->name_file, &(input_file->information));

    FILE* file = fopen(input_file->name_file, "r");
    if(!file) {
        ASSERTION(FILE_BAD_PTR)
        return FILE_BAD_PTR;
    }

    input_file->size_buffer = get_size_of_buffer(file);
    input_file->buffer      = (char*)calloc(input_file->size_buffer + 5, sizeof(char));

    int status = fread(input_file->buffer, sizeof(char), input_file->size_buffer, file);
    if(status != input_file->size_buffer) {
        ASSERTION(FILE_BAD_BUFFER)
        return FILE_BAD_BUFFER;
    }

    fclose(file);
    return FILE_OK;
}

int get_size_of_buffer(FILE* file) { // stat
    int length = 0;
    if (!fseek(file, 0, SEEK_END)) {
        length = ftell(file);
    }
    fseek(file, 0, SEEK_SET);

    return length;
}

FILE_STATUS file_destruct(struct File* input_file) {
    free(input_file->name_file);
    free(input_file->buffer);
    free(input_file);

    return FILE_OK;
}


