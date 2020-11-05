/**
 *  @file
 *  @author Kolesnikova Xenia <heiduk.k.k.s@yandex.ru>
 *  @version 1.0
 *  @par Last edition
 *                  November 6, 2020, 00:00:00
 *  @par What was changed?
 *                      1. Fixed disassembler
 *                      2. Fixed command meow
 *  @par To-do list
 *                      1. Add functions related with labels to disassembler
 *                      2. Fix the output to the file
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "assembler.h"
#include "disassembler.h"
#include "processor.h"
#include "common.h"

#define CREATE_FILE(file_name, reading_mode)                      \
    File file = {};                                               \
                                                                  \
    status = file_construct(&file, file_name, reading_mode);      \
    if(status != OK) {                                            \
        return ERROR_NUMBER;                                      \
    }

#define CREATE_PROCESSOR(name_input_file)                         \
    Processor processor = {};                                     \
                                                                  \
    status = initialization_proc(&processor, name_input_file);    \
    if(status != OK) {                                            \
        return ERROR_NUMBER;                                      \
    }

void help() {
    printf("That is my realisarion of softprocessor.\n"
           "The required parameters look like this:\n"
                    "[-h]                                                               - if you want to read this help again\n"
                    "[-a name_input_file_to_assembler name_output_file_to_assembler]    - if you want to assembler file\n"
                    "[-p name_input_file_to_processor]                                  - if you want to follow instructions\n"
                    "[-a -p name_input_file_to_assembler name_output_file_to_assembler] - if you want to assembler file and follow instructions"
                    "For more information, go here: https://github.com/owl1234/MIPT_projects_1_sem/tree/master/Freaky_CPU_on_defines\n");
}

int main(const int argc, const char* argv[]) {
    assert(argc > 0);

    int status = OK;

    if(argc == 5 && !strcmp(argv[1], "-a") && !strcmp(argv[2], "-p")) {
        CREATE_FILE(argv[3], "r");
        assembling_file(&file, argv[4]);

        CREATE_PROCESSOR(argv[4]);
        file_handler(&processor);
    } else if(argc == 4 && !strcmp(argv[1], "-a")) {
        CREATE_FILE(argv[2], "r");
        assembling_file(&file, argv[3]);
    } else if(argc == 3 && !strcmp(argv[1], "-p")) {
        CREATE_PROCESSOR(argv[2]);
        file_handler(&processor);
    } else if(argc == 4 && !strcmp(argv[1], "-d")){
        CREATE_FILE(argv[2], "rb");
        disassembling_file(&file, argv[3]);
    } else {
        help();
    }

    return OK;
}

