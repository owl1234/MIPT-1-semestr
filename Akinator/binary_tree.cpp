#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdarg.h>
#include "binary_tree.h"
#include "work_with_file.h"

#define INFORMATION_ABOUT_CALL (call_of_dump){__FILE__, __LINE__, __FUNCTION__}

#define ASSERTION(status)                                                                                                                    \
    system("echo \e[31m-----------------!WARNING!----------------\e[0m");                                                                    \
    char warning_info[150] = "";                                                                                                             \
    sprintf(warning_info, "echo \"\\e[31mIN FILE %s (FUNCTION %s, LINE %d)\\e[0m\"", __FILE__, __FUNCTION__, __LINE__);                      \
    system(warning_info);                                                                                                                    \
    sprintf(warning_info, "echo \"\\e[31mFile status: %d\\e[0m\"", status);                                                \
    system(warning_info);

#define REPORT_ABOUT_ERROR(code_of_error)                                                            \
    warning(INFORMATION_ABOUT_CALL);                                                        \

char say_string[MAX_SIZE_KEY] = "";


#define PRINT_AND_SAY(string)                   \
    printf("%s", string);                     \
    strcat(say_string, "echo \"");               \
    strcat(say_string, string);                 \
    strcat(say_string, "\" | festival --tts");  \
    system(say_string);                         \
    memset(say_string, '\0', MAX_SIZE_KEY);

TREE_STATUS tree_construct(Binary_tree* tree) {
    if(!tree) {
        REPORT_ABOUT_ERROR(TREE_BAD_POINTER);
        return TREE_BAD_POINTER;
    }

    tree->root = NULL;
    tree->size_tree = 0;

    return TREE_OKEY;
}

TREE_STATUS load_buffer_and_tree_from_file(Binary_tree* tree, char* file_name) {
    File input_file = {};
    input_file.name_file = file_name;

    FILE_STATUS status = read_buffer(&input_file);
    if(status != FILE_OK) {
        REPORT_ABOUT_ERROR(TREE_BAD_READ_FROM_FILE);
        return TREE_BAD_READ_FROM_FILE;
    }

    int count_of_quotes = 0, is_found_node = 0;

    tree->root = load_tree_from_buffer(input_file.buffer);

    return TREE_OKEY;
}

Node_binary_tree* load_tree_from_buffer(char* buffer) {
    if(!buffer)
        return NULL;

    char* now_position = buffer;
    now_position = search_next_position_after_symbol(now_position, '"');                                  // find begin root

    char* root_position = now_position;
    int length_name_root = 0;

    while(*now_position != '"') {                              // find end root
        ++now_position;
        ++length_name_root;
    }
    ++now_position;

    Node_binary_tree* result_tree = (Node_binary_tree*)calloc(1, sizeof(Node_binary_tree));
    node_construct(result_tree, root_position, length_name_root);

    while(*now_position != '[' && *now_position != ']' && *now_position != '"')        // find []
        ++now_position;

    char* left_son = {};
    int length_name_left = 0;
    bool is_leaf_left = false, is_leaf_right = false;

    if(*now_position  != ']') {
        now_position = search_next_position_after_symbol(now_position, '"');

        left_son = now_position;
        length_name_left = 0;

        while(*now_position != '"') {                          // find end the left son
            ++now_position;
            ++length_name_left;
        }
        ++now_position;

        while(*now_position != '"' && *now_position != '[')     // find
            ++now_position;

        if(*now_position == '[') {
            now_position = find_right_node(now_position);
            is_leaf_left = false;
        } else
            is_leaf_left = true;


        int length_name_right = 0;

        now_position = search_next_position_after_symbol(now_position, '"');                              // find begin right name

        char* right_son = now_position;
        while(*now_position != '"') {                             // find end right name
            ++now_position;
            ++length_name_right;
        }
        ++now_position;

        while(*now_position != ']' && *now_position != '[')
            ++now_position;

        if(*now_position == '[')
            is_leaf_right = false;
        else
            is_leaf_right = true;

        if(is_leaf_left) {
            Node_binary_tree* left_son_of_leaf = (Node_binary_tree*)calloc(1, sizeof(Node_binary_tree));
            node_construct(left_son_of_leaf, left_son, length_name_left);
            result_tree->left = left_son_of_leaf;
        }
        else
            result_tree->left = load_tree_from_buffer(left_son - 1);
        result_tree->left->length_name = length_name_left;
        result_tree->left->position_in_buffer = left_son;
        result_tree->left->parent = result_tree;
        result_tree->left->is_leaf = is_leaf_left;

        if(is_leaf_right) {
            Node_binary_tree* right_son_of_leaf = (Node_binary_tree*)calloc(1, sizeof(Node_binary_tree));
            node_construct(right_son_of_leaf, left_son, length_name_right);
            result_tree->right = right_son_of_leaf;
        } else
            result_tree->right = load_tree_from_buffer(right_son - 1);
        result_tree->right->length_name = length_name_right;
        result_tree->right->position_in_buffer = right_son;
        result_tree->right->parent = result_tree;
        result_tree->right->is_leaf = is_leaf_right;
    }

    return result_tree;
}

TREE_STATUS node_construct(Node_binary_tree* new_node, char* position_in_buffer, const int length_name) {
    if(!new_node) {
        REPORT_ABOUT_ERROR(TREE_BAD_POINTER);
        return TREE_BAD_POINTER;
    }

    new_node->height = 1;
    new_node->length_name = length_name;

    if(length_name > MAX_SIZE_KEY)
        new_node->length_name = MAX_SIZE_KEY;

    new_node->position_in_buffer = position_in_buffer;

    return TREE_OKEY;
}

char* search_next_position_after_symbol(char* pointer_in_tree, const char symbol) {
    char* copy_pointer = pointer_in_tree;

    while(*copy_pointer != symbol)
        ++copy_pointer;
    ++copy_pointer;

    return copy_pointer;
}

char* find_right_node(char* pointer_in_tree) {
    char* copy_pointer = pointer_in_tree + 1;

    int is_it_node = 0, count_of_brackets = 1;

    while(count_of_brackets > 0) {
        if(*copy_pointer == '"')
            is_it_node = (is_it_node + 1) % 2;
        else {
            if(*copy_pointer == '[')
                ++count_of_brackets;
            else if(*copy_pointer == ']')
                --count_of_brackets;
        }

        ++copy_pointer;
    }

    while(*copy_pointer != '"')
        ++copy_pointer;

    return copy_pointer;        // возвращает указатель на имя правого сына

}

void warning(struct call_of_dump arguments_of_call) {
    system("echo \e[31m-----------------!WARNING!----------------\e[0m");
    char warning_info[SIZE_OF_WARNINGS] = "";
    sprintf(warning_info, "echo \"\\e[31mIN FILE %s (FUNCTION %s, LINE %d)\\e[0m\"", arguments_of_call.name_file, arguments_of_call.name_function, arguments_of_call.number_of_line);
    system(warning_info);
    system(warning_info);
}

void dump_tree(Binary_tree* tree) {
    if(!tree)
        return;
    FILE* picture = fopen("picture.dot", "w");
    fprintf(picture, "digraph graphname {\n");

    if(tree->root)
        do_dump_tree(tree->root, picture);

    fprintf(picture, "}\n");
    fclose(picture);

    char result_string[SIZE_OF_NAME_FILES] = "";
    sprintf(result_string, "dot picture.dot -T png -o picture.png"); // | eog picture0.png"; //, my_list->number_of_draw_graph_call);
    system(result_string);
}

void do_dump_tree(Node_binary_tree* node, FILE* file) {
    fprintf(file, "%ld[shape=record label = \"", (size_t)node); //\"height = %ld|{left = %p|right = %p| parent = %p|node = %p}|size_name = %d|", (size_t)node, node->height, node->left, node->right, node->parent, node, node->length_name);
    print_node_name_into_file(node->position_in_buffer, node->length_name, file);
    fprintf(file, "\"]\n");
    if(node->left) {
        fprintf(file, "%ld->%ld\n", (size_t)node, (size_t)node->left);
        do_dump_tree(node->left, file);
    }
    if(node->right) {
        fprintf(file, "%ld->%ld\n", (size_t)node, (size_t)node->right);
        do_dump_tree(node->right, file);
    }

}

void print_node_name_into_file(char* buffer, int size_name_node, FILE* file) {
    char* now_position = buffer;

    for(int i=0; i<size_name_node; ++i) {
        fprintf(file, "%c", *now_position);
        ++now_position;
    }
}

void print_node_name_into_concole(char* buffer, int size_name_node) {
    char* now_position = buffer;

    for(int i=0; i<size_name_node; ++i) {
        printf("%c", *now_position);
        ++now_position;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////// PLAY ///////////////////////////////////////////////////////////////////////////////

bool search_leaf(Node_binary_tree* node) {
    char* name_node = (char*)calloc(node->length_name, sizeof(char));
    memcpy(name_node, node->position_in_buffer, node->length_name);

    if(node->is_leaf) {
        print_and_say(true, "Everything is clear. You made a ", name_node, " wish. Am I right?", NULL);
        free(name_node);
        return check_akinator_answer(node);
    } else {
        print_and_say(true, name_node, "?", NULL);

        USER_ANSWERS user_answer = get_user_answer();
        if(user_answer == YES_ANSWER_USER) {
            search_leaf(node->left);
        } else
            search_leaf(node->right);
    }

    free(name_node);
    return false;
}

USER_ANSWERS get_user_answer() {
    char user_answer = '@', garbage = '!';

    scanf("%c", &user_answer);
    while(scanf("%c", &garbage) && garbage != '\n') {}

    if(user_answer == 'y' || user_answer == 'Y')
        return YES_ANSWER_USER;
    if(user_answer == 'n' || user_answer == 'N')
        return NO_ANSWER_USER;

    print_and_say(true, "I don't understand you, my small friend.");
    get_user_answer();
}

bool check_akinator_answer(Node_binary_tree* node) {
    USER_ANSWERS user_answer = get_user_answer();

    if(user_answer == YES_ANSWER_USER) {
        print_and_say(true, "Of course, who would doubt it? Ha-ha. Do you want to play again?");
    } else {
        print_and_say(false, "Hmmm.. It is very strange. All right, what did you wish for?");

        char* name_new_node = (char*)calloc(MAX_SIZE_KEY, sizeof(char));
        char now_symbol = '!';
        int length_name_new_node = 0;

        scanf("%c", &now_symbol);
        while(now_symbol != '\n') {
            name_new_node[length_name_new_node++] = now_symbol;
            scanf("%c", &now_symbol);
        }

        strcat(name_new_node, " and ");
        memcpy(name_new_node + length_name_new_node + strlen(" and "), node->position_in_buffer, node->length_name);

        print_and_say(false, "What is the differences between ", name_new_node, "?", NULL);

        Node_binary_tree* new_node = (Node_binary_tree*)calloc(1, sizeof(Node_binary_tree));
        node_construct(new_node, name_new_node, length_name_new_node);

        char* difference = (char*)calloc(MAX_SIZE_KEY, sizeof(char));
        int length_difference = 0;

        scanf("%c", &now_symbol);
        while(now_symbol != '\n') {
            difference[length_difference++] = now_symbol;
            scanf("%c", &now_symbol);
        }

        Node_binary_tree* new_indicator = (Node_binary_tree*)calloc(1, sizeof(Node_binary_tree));
        node_construct(new_indicator, difference, length_difference);

        new_indicator->left  = new_node;
        new_indicator->right = node;
        new_indicator->parent = node->parent;
        new_indicator->is_leaf = false;

        if(node->parent->left == node)
            node->parent->left = new_indicator;
        else
            node->parent->right = new_indicator;

        node->parent  = new_node->parent  = new_indicator;
        node->is_leaf = new_node->is_leaf = true;

        print_and_say(true, "Well, I remember that word. But next time you won't catch me, I'll be smarter! Do you want to play again?", NULL);

        free(name_new_node);
        free(new_node);
        free(difference);
        free(new_indicator);
    }

    if(get_user_answer() == YES_ANSWER_USER) {
        printf("\t\tokey yes\n");
        return true;
    }
    else
        return false;
}

void put_tree_to_disk(Node_binary_tree* node, FILE* file, const int height) {
    for(int i=0; i<height; ++i)
        fprintf(file, " ");
    fprintf(file, "\"");
    print_node_name_into_file(node->position_in_buffer, node->length_name, file);
    fprintf(file, "\"\n");

    if(!node->is_leaf) {
        for(int i=0; i<height; ++i)
            fprintf(file, " ");
        fprintf(file, "[\n");
        put_tree_to_disk(node->left, file,  height + 1);
        put_tree_to_disk(node->right, file, height + 1);
        for(int i=0; i<height; ++i)
            fprintf(file, " ");
        fprintf(file, "]\n");
    }
}

void make_definition(Node_binary_tree* node) {

}

void print_and_say(bool is_yes_or_no_question, const char* word, ...) {
    char* buffer_arguments = (char*)calloc(300, sizeof(char)); //[300] = {0};
    char* now_word = (char*)calloc(100, sizeof(char));
    char* say_string = (char*)calloc(300, sizeof(char));

    va_list say_and_print_strings;
    va_start(say_and_print_strings, word);

    strcat(buffer_arguments, word);
    now_word = va_arg(say_and_print_strings, char*);

    while(now_word != NULL) {
        strcat(buffer_arguments, now_word);
        now_word = va_arg(say_and_print_strings, char*);
    }

    va_end(say_and_print_strings);

    printf("%s", buffer_arguments);

    if(is_yes_or_no_question)
        printf(" ([Y]es of [N]o)");
    printf("\n> ");

    strcat(say_string, "echo \"");
    strcat(say_string, buffer_arguments);
    strcat(say_string, "\" | festival --tts\n");
    system(say_string);

    free(now_word);
    free(buffer_arguments);
    free(say_string);
}
