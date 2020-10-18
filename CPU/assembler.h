int is_right_command(const char* line, const char* command);

void assembling_file();

void put_int_into_assembled_text(int code_of_operation, char* assembled_text, int* index);

void put_char_into_assembled_text(const char* temp_string, const int length, char* assembled_text, int* index_in_assembled_text);

int create_assembling_file(const char* assembled_text, const int index_in_assembled_text);

int reversed_number(int value, int* length);

