
#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <stdio.h>
#include "work_with_catalog.h"
#include "stack.h"

const int MAX_SIZE_KEY = 100;
const int MAX_SIZE_DEFINITION = 1000;
const int MAX_SIZE_SAY_WORDS  = 1000;

struct Node_binary_tree {
    Node_binary_tree* left;
    Node_binary_tree* right;
    Node_binary_tree* parent;

    char* position_in_buffer;
    size_t index_into_names_catalog;
    int length_name;

    size_t height;
    bool is_leaf;
};

struct Binary_tree {
    Node_binary_tree* root;
    size_t size_tree;
};

enum TREE_STATUS {
    TREE_OKEY = 0,
    TREE_BAD_POINTER = 1,
    TREE_BAD_READ_FROM_FILE = 2,
};

const char TEXT_TREE_STATUS[][50] = {
    "Tree okey",
    "Bad ptr to tree",
    "Tree don't read from file"
};

enum USER_ANSWERS {
    YES_ANSWER_USER  = 'Y',
    NO_ANSWER_USER   = 'N',
    ANSWER_NO_EXISTS = '@',
};

enum TYPE_UTTERANCE {
    PHRASE_WITHOUT_QUESTION   = 0,
    YES_OR_NO_QUESTION        = 1,
    HELP_PHRASE               = 2,
    QUESTION_WITH_FULL_ANSWER = 3,
};

TREE_STATUS tree_construct(Binary_tree* tree);

TREE_STATUS load_buffer_and_tree_from_file(Binary_tree* tree, Catalog_names* catalog_name_nodes, char* file_name);

Node_binary_tree* load_tree_from_buffer(char* buffer, Catalog_names* catalog_name_nodes);

TREE_STATUS node_construct(Node_binary_tree* new_node, char* position_in_buffer, const int length_name, Catalog_names* catalog_name_nodes);

char* search_next_position_after_symbol(char* pointer_in_tree, const char symbol);

char* find_right_node(char* pointer_in_tree);

void warning(struct call_of_dump arguments_of_call);

void dump_tree(Binary_tree* tree);

void do_dump_tree(Node_binary_tree* node, FILE* file);

void print_node_name_into_file(Catalog_names* catalog_name_nodes, size_t index_into_catalog, FILE* file);

void print_node_name_into_concole(Catalog_names* catalog_name_nodes, size_t index_into_catalog);

bool search_leaf(Node_binary_tree* node, Catalog_names* catalog_name_nodes);

USER_ANSWERS get_user_answer();

bool check_akinator_answer(Node_binary_tree* node, Catalog_names* catalog_name_nodes);

void put_tree_to_disk(Node_binary_tree* node, FILE* file, const int height);

void find_node_in_tree(Binary_tree* node, Catalog_names* catalog_name_nodes, Stack_t* definition_stack, const char* need_word);

bool do_find_node_in_tree(Node_binary_tree* node, Catalog_names* catalog_name_nodes, Stack_t* definition_stack, const char* need_word);

bool is_equivalent_words(Catalog_names* catalog_name_nodes, size_t position_into_catalog, const char* need_word);

void print_definition(Catalog_names* catalog_name_nodes, Stack_t* definition_stack, const char* need_word);

void comparison_nodes(Binary_tree* tree, Catalog_names* catalog_name_nodes, const char* first_word, const char* second_word);

void compare_definitions(Catalog_names* catalog_name_nodes, Stack_t* definition_first_word, Stack_t* definition_second_word, const char* first_word, const char* second_word);

void find_random_node_in_tree(Binary_tree* tree, Catalog_names* catalog_name_nodes);

void do_find_random_node_in_tree(Node_binary_tree* node, Catalog_names* catalog_name_nodes, Stack_t* definition_stack, char* find_word);

void print_and_say(TYPE_UTTERANCE type, const char* word, ...);

#endif // BINARY_TREE_H
