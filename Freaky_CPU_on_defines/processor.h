#include "common.h"

int initialization_proc(Processor* processor, const char* name_input_file);

void file_handler(Processor* processor);

int size_of_buffer(FILE* file);

int check_commands();

void print_ram(Elem_t* ram);

Elem_t get_value_to_compare(Processor* processor, int* now_byte);

int get_double_from_text(Processor* processor, int* now_byte);
