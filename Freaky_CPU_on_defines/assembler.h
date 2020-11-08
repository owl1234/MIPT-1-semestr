#include "common.h"

struct File {
    FILE* input_file;
    const char* name;
    char* text_for_assembling;
    FILE* listing_file;
    char* text_for_listing;
    int lines;
    struct stat information;
};

typedef enum ASSEMBLER_ERROROS {
    ASM_BAD_FILE           = 1,
    ASM_BAD_MEMORY         = 2,
    ASM_BAD_READ_FROM_FILE = 3,
    ASM_BAD_COMMAND        = 4,
    ASM_BAD_PUSH_VALUE     = 5,

} ASM_ERRORS;

const char* TEXT_ASM_ERRORS[] = {
    "Okey",
    "File din't open",
    "Memory wasn't allocated",
    "The text wasn't read from the file"
};

void help();

int is_right_command(const char* line, const char* command);

void listing(File* file, const char* symbols_to_output, const int flag_of_the_end);

void listing_alignment(File* file, const int number_to_output, const int flag_of_the_end);

void listing(File* file, const int flag_of_the_end, int count_of_args, ...);

void listing(File* file, const int flag_of_the_end, double count_of_args, ...);

void listing(File* file, const char symbol_to_output, int flag_of_the_end);

int assembling_file(File* input_file, const char* name_output_file);

void find_labels_into_text(File* input_file, Label* labels, int* index_in_labels);

bool is_it_label(const char* word);

int find_and_write_command(char* text, char* assembled_text, int* index_in_assembled_text, Label* labels, int* index_in_labels, int* number_of_byte,
                                                                                            Label* go_to_labels, int* index_in_go_to_labels, File* input_file);

void assembler_push(char* text, char* assembled_text, int* index_in_assembled_text, int* number_of_byte, File* input_file);

void assembler_pop(char* text, char* assembled_text, int* index_in_assembled_text, Label* labels, int* index_in_labels, int* number_of_byte,
                                                                                   Label* go_to_labels, int* index_in_go_to_labels, File* input_file);

void assembler_cmp(char* text, char* assembled_text, int* index_in_assembled_text, int* number_of_byte, File* input_file);

void assembler_meow(char* assembled_text, int* index_in_assembled_text, int* number_of_byte, File* input_file);

void assembler_sqrt(char* assembled_text, int* index_in_assembled_text, int* number_of_byte, File* input_file);

void assembler_labels(char* text, char* assembled_text, int* index_in_assembled_text, int* number_of_byte, Label* go_labels, int* index_in_labels, int number_of_condition,
                                                                                                                                                            File* input_file);

bool is_equal_labels(const char* first, const char* second);

int min(int first, int second);

int put_cmp_value(char* text, char* assembled_text, int* index_in_assembled_text, int* number_of_byte, char* argument, int* type_of_argument, File* input_file);

void put_int_into_assembled_text(int code_of_operation, char* assembled_text, int* index_in_assembled_text, int* number_of_byte, File* input_file);

void put_double_into_assembled_text(double code_of_operation, char* assembled_text, int* index_in_assembled_text, int* number_of_byte, File* input_file);

void create_label(char* text, Label* labels, int* index_in_labels, int index_in_assembled_text, int number_of_byte);

int create_assembling_file(const char* assembled_text, const int index_in_assembled_text, const char* name_output_file);

void reverse_string(char* text);

int reversed_number(int value, int* length);

void work_with_registers(int last_operation_code, int operation_code, char* assembled_text, int* index_in_assembled_text);

bool is_text_connected_with_labels(char* text, int* number_of_condition);
