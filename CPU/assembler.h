enum FLAGS_OF_THE_END_LINE {
    END_LINE = 0,
    NOT_END_LINE = 1
};

int is_right_command(const char* line, const char* command);

void assembling_file();

void put_int_into_assembled_text(int code_of_operation, char* assembled_text, int* index, int flag_of_the_end_line);

void put_char_into_assembled_text(const char* temp_string, const int length, char* assembled_text, int* index_in_assembled_text);

int create_assembling_file(const char* assembled_text, const int index_in_assembled_text);

int reversed_number(int value, int* length);

void work_with_registers(int last_operation_code, int operation_code, char* assembled_text, int* index_in_assembled_text);
