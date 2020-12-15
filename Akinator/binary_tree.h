#include <stdio.h>
#include "work_with_catalog.h"

const int MAX_SIZE_KEY = 100;
const int SIZE_OF_WARNINGS = 100;

struct Node_binary_tree {
    Node_binary_tree* left;
    Node_binary_tree* right;
    Node_binary_tree* parent;

    char* position_in_buffer;
    //char key[MAX_SIZE_KEY];
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
    PHRASE_WITHOUT_QUESTION = 0,
    YES_OR_NO_QUESTION = 1,
    HELP_PHRASE = 2,
    QUESTION_WITH_FULL_ANSWER = 3,
};

TREE_STATUS tree_construct(Binary_tree* tree);

TREE_STATUS load_buffer_and_tree_from_file(Binary_tree* tree, Catalog_names* catalog_name_nodes, char* file_name);

Node_binary_tree* load_tree_from_buffer(char* buffer, Catalog_names* catalog_name_nodes);

TREE_STATUS node_construct(Node_binary_tree* new_node, char* name_node, const int length_name);

char* search_next_position_after_symbol(char* pointer_in_tree, const char symbol);

char* find_right_node(char* pointer_in_tree);

void warning(struct call_of_dump arguments_of_call);

void dump_tree(Binary_tree* tree);

void do_dump_tree(Node_binary_tree* node, FILE* file);

void print_node_name_into_file(char* buffer, int size_name_node, FILE* file);

void print_node_name_into_concole(char* buffer, int size_name_node);

bool search_leaf(Node_binary_tree* node);

USER_ANSWERS get_user_answer();

bool check_akinator_answer(Node_binary_tree* node);

void put_tree_to_disk(Node_binary_tree* node, FILE* file, const int height);

void print_and_say(TYPE_UTTERANCE type, const char* word, ...);
