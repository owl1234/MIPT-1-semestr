#include "common.h"

int is_right_command(const char* line, const char* command);

void assembling_file();

void find_and_write_command(char* text, char* assembled_text, int* index_in_assembled_text, Label* labels, int* index_in_labels, int* number_of_byte,
                                                                                            Label* go_to_labels, int* index_in_go_to_labels);

void assembler_push(char* text, char* assembled_text, int* index_in_assembled_text, int* number_of_byte);

void assembler_pop(char* text, char* assembled_text, int* index_in_assembled_text, Label* labels, int* index_in_labels, int* number_of_byte,
                                                                                   Label* go_to_labels, int* index_in_go_to_labels);

void assembler_cmp(char* text, char* assembled_text, int* index_in_assembled_text, int* number_of_byte);

void assembler_meow(char* assembled_text, int* index_in_assembled_text, int* number_of_byte);

void assembler_sqrt(char* assembled_text, int* index_in_assembled_text, int* number_of_byte);

void assembler_labels(char* text, char* assembled_text, int* index_in_assembled_text, int* number_of_byte, Label* go_to_labels, int* index_in_go_to_labels, int number_of_condition);

void put_cmp_value(char* text, char* assembled_text, int* index_in_assembled_text, int* number_of_byte);

void put_int_into_assembled_text(int code_of_operation, char* assembled_text, int* index_in_assembled_text, int* number_of_byte);

void put_double_into_assembled_text(double code_of_operation, char* assembled_text, int* index_in_assembled_text, int* number_of_byte);

void put_char_into_assembled_text(char* text, char* assembled_text, int* index_in_assembled_text, int* number_of_byte);

void create_label(char* text, Label* labels, int* index_in_labels, int index_in_assembled_text, int number_of_byte);

int create_assembling_file(const char* assembled_text, const int index_in_assembled_text);

int reversed_number(int value, int* length);

void work_with_registers(int last_operation_code, int operation_code, char* assembled_text, int* index_in_assembled_text);

void fill_array_by_machine_code(Label* labels, int count_of_labels, Label* go_to_labels, int count_of_go_to_labels, char* assembled_text, int length_of_assembled_text);

bool is_text_connected_with_labels(char* text, int* number_of_condition);
