void disassembling_file();

void put_char_into_disassembled_text(int code_of_operation, char* disassembled_text, int* index);

void put_int_into_disassembled_text(const char* temp_string, const int length, char* disassembled_text, int* index_in_disassembled_text);

int create_disassembling_file(const char* disassembled_text, const int index_in_disassembled_text);


