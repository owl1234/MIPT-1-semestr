#include <stdio.h>
#include "Onegin.h"
#include "testing.h"

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
