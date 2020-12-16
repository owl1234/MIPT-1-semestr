/**
 *  @file
 *  @author Kolesnikova Xenia <heiduk.k.k.s@yandex.ru>
 *  @par Last edition
 *                  December 16, 2020, 23:14:25
 *  @par What was changed?
 *                      1. Make comparison
 *  @par To-do list
 *                      1. Kill recursion (change to stack)
 *                      2. Kill magic numbers!!
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "main.h"
#include "work_with_file.h"
#include "binary_tree.h"
//#include "work_with_catalog.h"
#include "warnings.cpp"
//#include "stack.h"

#define INFORMATION_ABOUT_CALL (call_of_dump){__FILE__, __LINE__, __FUNCTION__}

void help() {
    print_and_say(HELP_PHRASE, "This program load catalog tree from file", NULL);
    printf("[L]oad from file / [G]ame / [P]ut on the disk / [D]efinition of someone make / [C]ompare two words / [Q]uit     \n"
                    "For more information, go here: https://github.com/owl1234/MIPT-1-semestr/tree/master/Stack\n");
}

int main(int argc, char* argv[]) {
    help();

    Binary_tree akinator  = {};
    tree_construct(&akinator);

    Catalog_names catalog_name_nodes = {};
    catalog_names_construct(&catalog_name_nodes);

    Stack_t definition_stack = {};
    stack_construct(&definition_stack);

    char type_command[MAX_SIZE_KEY] = "", second_word_for_comparison[MAX_SIZE_KEY] = "";
    char garbage = '!';
    bool is_continue_game = false, is_continue_program = true, is_load_tree_from_file = false;

    while(is_continue_program) {
        print_and_say(QUESTION_WITH_FULL_ANSWER, "Okey. Well, what do we do next?", NULL);

        scanf("%[^\r\n]%c", type_command, &garbage);

        if(is_request_load_from_file(type_command)) {
            load_buffer_and_tree_from_file(&akinator, &catalog_name_nodes, "database_pro.txt");
            is_load_tree_from_file = true;
            printf("The download was successful.\n");
        }

        else if(is_request_game(type_command)) {
            if(!is_load_tree_from_file) {
                print_and_say(PHRASE_WITHOUT_QUESTION, "Please, download the tree before you play with me.", NULL);
            } else {
                is_continue_game = search_leaf(akinator.root, &catalog_name_nodes);
                while(is_continue_game)
                    is_continue_game = search_leaf(akinator.root, &catalog_name_nodes);
            }
        }

        else if(is_request_make_the_definition(type_command)) {
            print_and_say(QUESTION_WITH_FULL_ANSWER, "Write the word you want to hear defined.");
            scanf("%[^\r\n]%c", type_command, &garbage);
            find_node_in_tree(&akinator, &catalog_name_nodes, &definition_stack, type_command);
        }

        else if(is_request_make_the_comparison(type_command)) {
            print_and_say(QUESTION_WITH_FULL_ANSWER, "Write the first word you want to hear defined.");
            scanf("%[^\r\n]%c", type_command, &garbage);

            print_and_say(QUESTION_WITH_FULL_ANSWER, "Write the second word you want to hear defined.");
            scanf("%[^\r\n]%c", second_word_for_comparison, &garbage);

            comparison_nodes(&akinator, &catalog_name_nodes, type_command, second_word_for_comparison);
        }

        /*else if(is_request_load_to_file(type_command)) {
            scanf("%[^\r\n]%c", type_command, &garbage);

            FILE* file = fopen(type_command, "w");
            if(file) {
                put_tree_to_disk(akinator.root, file, 0);
                printf("The tree was successfully placed on disk.\n");
            } else
                return USER_IS_STUPID;
        }*/

        else if(is_request_finish_the_program(type_command)) {
            print_and_say(PHRASE_WITHOUT_QUESTION, "Bye, my little friend.", NULL);
            is_continue_program = false;
        }
    }

    /*printf("size: %lu\n", catalog_name_nodes.count_nodes);
    for(int i=0; i<catalog_name_nodes.count_nodes; ++i) {
        printf("%lu, %lu: ", catalog_name_nodes.nodes[i].count_symbols_from_begin, catalog_name_nodes.nodes[i].length_name);
        for(int j=0; j<catalog_name_nodes.nodes[i].length_name; ++j)
            printf("%c", catalog_name_nodes.buffer[catalog_name_nodes.nodes[i].count_symbols_from_begin + j]);
        printf("\n");
    }*/

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

bool is_request_make_the_definition(const char* request) {
    if(request[0] == 'D' || request[0] == 'd')
        return true;
    return false;
}

bool is_request_make_the_comparison(const char* request) {
    if(request[0] == 'C' || request[0] == 'c')
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
