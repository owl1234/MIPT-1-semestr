/**
 *  @file
 *  @author Kolesnikova Xenia <heiduk.k.k.s@yandex.ru>
 *  @par Last edition
 *                  December 87, 2020, 21:59:25
 *  @par What was changed?
 *                      1. Fixed stack which caused the program to behave very randomly!!!
 *                      2. The tree become beautiful.
 *  @par To-do list
 *                      1. Pass Ded for a good grade.... (impossible ?!)
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "main.h"
#include "work_with_file.h"
#include "binary_tree.h"

#define INFORMATION_ABOUT_CALL (call_of_dump){__FILE__, __LINE__, __FUNCTION__}

void help() {
    print_and_say(HELP_PHRASE, "This program load catalog tree from file", NULL);
    printf("[L]oad from file / [G]ame / [P]ut on the disk / [D]efinition of someone make / [R]andom determination / [C]ompare two words / [Q]uit     \n"
                    "For more information, go here: https://github.com/owl1234/MIPT_projects_1_sem/tree/master/Akinator\n");
}

int main(int argc, char* argv[]) {
    help();
    srand(time(0));

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
        printf("\n");
        print_and_say(QUESTION_WITH_FULL_ANSWER, "Okey. Well, what do we do next?", NULL);

        scanf("%[^\r\n]%c", type_command, &garbage);

        if(is_request_load_from_file(type_command)) {
            //print_and_say(QUESTION_WITH_FULL_ANSWER, "Write the name of file to load the tree from.", NULL);
            //scanf("%[^\r\n]%c", type_command, &garbage);
            load_buffer_and_tree_from_file(&akinator, &catalog_name_nodes, "database_pro.txt"); // type_command
            is_load_tree_from_file = true;
            printf("The download was successful.\n");
        }

        else if(is_request_game(type_command) && is_load_tree_from_file) {
            is_continue_game = search_leaf(akinator.root, &catalog_name_nodes);
            while(is_continue_game)
                is_continue_game = search_leaf(akinator.root, &catalog_name_nodes);
        }

        else if(is_request_make_the_definition(type_command) && is_load_tree_from_file) {
            print_and_say(QUESTION_WITH_FULL_ANSWER, "Write the word you want to hear defined.", NULL);
            scanf("%[^\r\n]%c", type_command, &garbage);
            find_node_in_tree(&akinator, &catalog_name_nodes, type_command);
        }

        else if(is_request_make_the_comparison(type_command) && is_load_tree_from_file) {
            print_and_say(QUESTION_WITH_FULL_ANSWER, "Write the first word you want to hear defined.", NULL);
            scanf("%[^\r\n]%c", type_command, &garbage);

            print_and_say(QUESTION_WITH_FULL_ANSWER, "Write the second word you want to hear defined.", NULL);
            scanf("%[^\r\n]%c", second_word_for_comparison, &garbage);

            comparison_nodes(&akinator, &catalog_name_nodes, type_command, second_word_for_comparison);
        }

        else if(is_request_make_the_random_definition(type_command) && is_load_tree_from_file) {
            find_random_node_in_tree(&akinator, &catalog_name_nodes);
        }

        else if(is_request_load_to_file(type_command) && is_load_tree_from_file) {
            //print_and_say(QUESTION_WITH_FULL_ANSWER, "Write the name of file to load the tree to.", NULL);
            //scanf("%[^\r\n]%c", type_command, &garbage);

            FILE* file = fopen("load_tree.txt", "w"); // type_command
            if(file) {
                put_tree_to_disk(akinator.root, &catalog_name_nodes, file, 0);
                printf("The tree was successfully placed on disk.\n");
            } else
                return USER_IS_STUPID;

            dump_tree(&akinator, &catalog_name_nodes);
            fclose(file);
        }

        else if(is_request_help(type_command)) {
            help();
        }

        else if(is_request_finish_the_program(type_command)) {
            print_and_say(PHRASE_WITHOUT_QUESTION, "Bye, my little friend.", NULL);
            is_continue_program = false;
        }

        else if(!is_load_tree_from_file) {
                print_and_say(PHRASE_WITHOUT_QUESTION, "Please, download the tree before you play with me.", NULL);
            }
    }

    destruct_tree(&akinator);

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

bool is_request_make_the_random_definition(const char* request) {
    if(request[0] == 'R' || request[0] == 'r')
        return true;
    return false;
}

bool is_request_load_to_file(const char* request) {
    if(request[0] == 'P' || request[0] == 'p')
        return true;
    return false;
}

bool is_request_help(const char* request) {
    if(request[0] == 'H' || request[0] == 'h')
        return true;
    return false;
}


bool is_request_finish_the_program(const char* request) {
    if(request[0] == 'Q' || request[0] == 'q')
        return true;
    return false;
}

