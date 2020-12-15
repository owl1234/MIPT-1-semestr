/**
 *  @file
 *  @author Kolesnikova Xenia <heiduk.k.k.s@yandex.ru>
 *  @par Last edition
 *                  December 15, 2020, 18:36:25
 *  @par What was changed?
 *                      1. Redoing from the beginning....
 *                      2. Create catalogs for easy access to vertex names and easy addition of new nodes
 *                      3. Create file with warnings (small library for beautiful and comfortable print warnings)
 *  @par To-do list
 *                      1. Kill recursion (change to stack)
 *                      2. Make definition
 *                      3. Make comparison
 *                      4. Launch the game again
 *                      5. Normal (!!!) parsing of command line arguments
 *                      6. Убрать костыль в на 25 строке (настолько он меня возмущает, что пишу по-русски)
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "main.h"
#include "work_with_file.h"
#include "binary_tree.h"
#include "work_with_catalog.h"
#include "warnings.cpp"

#define INFORMATION_ABOUT_CALL (call_of_dump){__FILE__, __LINE__, __FUNCTION__}

void help() {
    print_and_say(HELP_PHRASE, "This program load catalog tree from file", NULL);
    printf("[L]oad from file / [G]ame / [P]ut on the disk / [Q]uit     \n"
                    "For more information, go here: https://github.com/owl1234/MIPT-1-semestr/tree/master/Stack\n");
}

int main(int argc, char* argv[]) {
    help();

    Binary_tree akinator  = {};
    tree_construct(&akinator);

    Catalog_names catalog_name_nodes = {};
    catalog_names_construct(&catalog_name_nodes);

    char type_command[MAX_SIZE_KEY] = "";
    char garbage = '!';
    bool is_continue_game = false, is_continue_program = true;

    while(is_continue_program) {
        print_and_say(QUESTION_WITH_FULL_ANSWER, "Okey. Well, what do we do next?", NULL);

        scanf("%[^\r\n]", type_command);
        scanf("%c", &garbage);
        printf("command: %s\n", type_command);

        if(is_request_load_from_file(type_command)) {
            load_buffer_and_tree_from_file(&akinator, &catalog_name_nodes, "database.txt");
            printf("The download was successful.\n");
        }

        /*else if(is_request_game(type_command)) {
            is_continue_game = search_leaf(akinator.root);
            while(is_continue_game)
                is_continue_game = search_leaf(akinator.root);
        }

        else if(is_request_load_to_file(type_command)) {
            scanf("%[^\r\n]", type_command);
            scanf("%c", &garbage);

            FILE* file = fopen(type_command, "w");
            if(file)
                put_tree_to_disk(akinator.root, file, 0);
            else
                return USER_IS_STUPID;
        }*/

        else if(is_request_finish_the_program(type_command)) {
            print_and_say(PHRASE_WITHOUT_QUESTION, "Goodbue... :(", NULL);
            is_continue_program = false;
        }
    }

    printf("size: %lu\n", catalog_name_nodes.count_nodes);
    for(int i=0; i<catalog_name_nodes.count_nodes; ++i) {
        printf("%lu, %lu: ", catalog_name_nodes.nodes[i].count_symbols_from_begin, catalog_name_nodes.nodes[i].length_name);
        for(int j=0; j<catalog_name_nodes.nodes[i].length_name; ++j)
            printf("%c", catalog_name_nodes.buffer[catalog_name_nodes.nodes[i].count_symbols_from_begin + j]);
        printf("\n");
    }

    return 0;
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


bool is_request_finish_the_program(const char* request) {
    if(request[0] == 'Q' || request[0] == 'q')
        return true;
    return false;
}
