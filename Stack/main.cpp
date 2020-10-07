#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

const int error_number = 3802;

void help() {
    printf("This program create stack and do some actions with it\n"
            "The required parameters look like this:\n"
                    "[-h] -if you want to read this help again\n"
                    "[-s] - if you want to work with stack\n"
                    "[-t] - if you want to test this program.\n"
                    "For more information, go here: https://github.com/owl1234/String-sorter\n");
}

int main(const int argc, const char* argv[]) {
    //const int args_for_testing = 2;
    const int args_for_stack = 2;

    if(argc == args_for_stack) {
        const char* name_log_file = argv[2];
        if(argv[1][0] == '-' && argv[1][1] == 's' && argc == args_for_stack) {
            return initialization_stack();
        }
        else {
            help();
            return error_number;
        }
    } else {
        help();
        return error_number;
    }

    return 0;
}
