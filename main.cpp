#include <stdio.h>
#include "main.h"
#include "Onegin.h"
#include "testing.h"

void help() {
    printf("This program sorts strings from file in alphabetical order (without taking into account the signs of pre).\n"
            "The required parameters look like this:\n"
                    "[-h]                                  - if you want to read this help again\n"
                    "[-s name_input_file name_output_file] - if you want to sorts strings\n"
                    "[-t]                                  - if you want to test this program.\n"
                    "For more information, go here: https://github.com/owl1234/String-sorter\n");
}

int main (const int argc, const char* argv[]) {
    const int args_for_testing = 2;
    const int args_for_sorting = 4;

    if(argc == args_for_testing || argc == args_for_sorting) {
        if     (argv[1][0] == '-' && argv[1][1] == 't' && argc == args_for_testing) { testing(); }
        else if(argv[1][0] == '-' && argv[1][1] == 's' && argc == args_for_sorting) { return sorting(argc, argv); }
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
