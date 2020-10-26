#include "common.h"

int is_right_command(const char* line, const char* command);

void assembling_file();

void find_and_write_command(char* text, char* assembled_text, int* index_in_assembled_text, Label* labels, int* index_in_labels, int* number_of_byte,
                                                                                            Label* jump_byte_position, int* index_in_jump_byte_position,
                                                                                            Label* ret_byte_position,  int* index_in_ret_byte_position);

void assembler_push(char* text, char* assembled_text, int* index_in_assembled_text, int* number_of_byte);

void assembler_pop(char* text, char* assembled_text, int* index_in_assembled_text, Label* labels, int* index_in_labels, int* number_of_byte,
                                                                                   Label* jump_byte_position, int* index_in_jump_byte_position,
                                                                                   Label* ret_byte_position,  int* index_in_ret_byte_position);

void assembler_cmp(char* text, char* assembled_text, int* index_in_assembled_text, int* number_of_byte);

void assembler_labels(char* text, char* assembled_text, int* index_in_assembled_text, int* number_of_byte, Label* jump_byte_position, int* index_in_jump_byte_position, int number_of_condition,
                                                                                                           Label* ret_byte_position,  int* index_in_ret_byte_position);

void put_cmp_value(char* text, char* assembled_text, int* index_in_assembled_text, int* number_of_byte, int end_of_line);

void put_int_into_assembled_text(int code_of_operation, char* assembled_text, int* index_in_assembled_text, int* number_of_byte, int flag_of_the_end_line);

void put_char_into_assembled_text(const char* temp_string, char* assembled_text, int* index_in_assembled_text, int* number_of_byte, int flag_of_the_end_line);

void create_label(char* text, Label* labels, int* index_in_labels, int index_in_assembled_text, int number_of_byte);

int create_assembling_file(const char* assembled_text, const int index_in_assembled_text);

int reversed_number(int value, int* length);

void work_with_registers(int last_operation_code, int operation_code, char* assembled_text, int* index_in_assembled_text);

void fill_array_by_machine_code(Label* labels, int count_of_labels, Label* jump_byte_position, int count_of_jump_byte_position, char* assembled_text, int length_of_assembled_text,
                                                                    Label* ret_byte_position,  int count_of_ret_byte_position);

bool is_text_connected_with_labels(char* text, int* number_of_condition);

