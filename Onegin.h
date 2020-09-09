int comparator(const void* first_string, const void* second_string);

int strcmp (const unsigned char* str1, const unsigned char* str2);

int number_of_lines(FILE* file, int* max_length);

FILE* open_file(FILE** file, char* file_name);

void close_file(FILE** file);
