void disassembling_file(File* input_file, const char* name_output_file);

void disassembler_push(File* file, char* disassembled_text, int* index_in_disassembled_text, int* now_byte);

void disassembler_pop(File* file, char* disassembled_text, int* index_in_disassembled_text, int* now_byte);

void put_char_into_disassembled_text(const char* command, char* disassembled_text, int* index_in_disassembled_text, int flag_of_the_end_line);

void put_int_into_disassembled_text(Elem_t value, char* disassembled_text, int* index_in_disassembled_text, int flag_of_the_end_line);

int create_disassembling_file(const char* disassembled_text, const int index_in_disassembled_text, const char* name_output_file);

int get_double_from_text(File* file, int* now_byte);

