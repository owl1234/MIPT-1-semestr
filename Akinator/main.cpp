/**
 *  @file
 *  @author Kolesnikova Xenia <heiduk.k.k.s@yandex.ru>
 *  @par Last edition
 *                  December 11, 2020, 12:17:25
 *  @par What was changed?
 *                      1. All :)
 *  @par To-do list
 *                      1. Kill recursion (change to stack)
 *                      2. Make definition
 *                      3. Make comparison
 *                      4. Launch the game again
 *                      5. Normal (!!!) parsing of command line arguments
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "main.h"
#include "work_with_file.h"
#include "binary_tree.h"

#define INFORMATION_ABOUT_CALL (call_of_dump){__FILE__, __LINE__, __FUNCTION__}

void help() {
    printf("This program load catalog tree from file\n"
            "[L]oad from file / [G]ame / [P]ut on the disk        \n"
                    "For more information, go here: https://github.com/owl1234/MIPT-1-semestr/tree/master/Stack\n");
}

int main(int argc, char* argv[]) {
    if(argc >= 3) {
        Binary_tree akinator = {};
        tree_construct(&akinator);

        for(int i=1; i<argc; ) {
            printf("%i %s\n", i, argv[i]);
            if(is_request_load_from_file(argv[i])) {
                if(i + 1 <= argc) {
                    load_buffer_and_tree_from_file(&akinator, argv[i+1]);
                    dump_tree(&akinator);
                } else {
                    warning(INFORMATION_ABOUT_CALL);
                    return USER_IS_STUPID;
                }
                ++i;
            }

            else if(is_request_game(argv[i])) {
                while(search_leaf(akinator.root)) {printf("main!!!!!!!!!!!!!!!\n");}
            }

            else if(is_request_load_to_file(argv[i])) {
                if(i + 1 <= argc) {
                    FILE* file = fopen(argv[i+1], "w");
                    if(file)
                        put_tree_to_disk(akinator.root, file, 0);
                    else
                        return USER_IS_STUPID;
                } else {
                    warning(INFORMATION_ABOUT_CALL);
                    return USER_IS_STUPID;
                }
                ++i;
            }

            ++i;
        }

    } else {
        help();
        return USER_IS_STUPID;
    }
}

bool is_request_load_from_file(const char* request) {
    if(request[0] == 'L' || request[0] == 'l')
        return true;
    return false;
}

bool is_request_play(const char* request) {
    if(request[0] == 'P' || request[0] == 'p')
        return true;
    return false;
}

bool is_request_game(const char* request) {
    if(request[0] == 'g' || request[0] == 'G')
        return true;
    return false;
}

bool is_request_load_to_file(const char* request) {
    if(request[0] == 'P' || request[0] == 'p')
        return true;
    return false;
}
