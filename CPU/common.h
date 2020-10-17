#include <sys/stat.h>
#include "operation_codes.h"

const int MAX_SIZE = 10;
const int ERROR_NUMBER = -3802;
const int OK_FILE = 0;

struct File {
    FILE* ptr_to_file;
    const char* name;
    char* text;
    int lines;
    struct stat information;
};

int is_right_command(const char* line, const char* command);

int number_of_symbols(char* buffer, char separator);

int file_construct(File* file, const char* name_file);


