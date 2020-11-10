typedef enum {
    DISASM_OKEY               = 0,
    DISASM_BAD_FILE           = 1,
    DISASM_BAD_MEMORY         = 2,
    DISASM_BAD_READ_FROM_FILE = 3,
    DISASM_BAD_COMMAND        = 4,
    DISASM_BAD_PUSH_VALUE     = 5,
    DISASM_BAD_VERSION        = 6,

} DISASM_ERRORS;

const char* TEXT_DISASM_ERRORS[] = {
    "Okey",
    "File din't open",
    "Memory wasn't allocated",
    "The text wasn't read from the file",
    "Bad command",
    "Bad value to push",
    "Bad version of commands"
};


struct File {
    FILE* input_file;
    const char* name;
    double* text_for_disassembling;
    int lines;
    struct stat information;
};

DISASM_ERRORS file_construct(File* file, const char* name_file, const char* reading_mode);

DISASM_ERRORS destruct_file(File* file);

DISASM_ERRORS disassembling_file(File* input_file, const char* name_output_file);

DISASM_ERRORS check_signature(File* file, int* now_byte);

void find_labels_into_text(File* input_file, Label* labels, int* index_in_labels, int real_size);

bool is_code_connected_with_labels(int command, int* number_of_condition);

void put_char_into_disassembled_text(const char* command, char* disassembled_text, int* index_in_disassembled_text, int flag_of_the_end_line);

void put_int_into_disassembled_text(Elem_t value, char* disassembled_text, int* index_in_disassembled_text, int flag_of_the_end_line);

DISASM_ERRORS create_disassembling_file(const char* disassembled_text, const int index_in_disassembled_text, const char* name_output_file);

int get_double_from_text(File* file, int* now_byte);

