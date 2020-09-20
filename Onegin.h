
struct pointer {
    char* ptr;
    int len;
    int pos;
};

/**
*   \brief This function opens files and starts functions number_of_lines, initialization, make_index_array and write_and_sort
*
*   @param argc,argv Command line parameters

*   @return error_number, if an error occurred, else 0
*/

int sorting(const int argc, const char* argv[]);

/**
*   \brief This function calculates number of lines in the file
*
*   @param file File with strings
*   @param max_length Maximum length of strings in a file
*   @param separator A symbol that is considered the end of a string
*
*   @return Number of lines in the file
*/

int number_of_lines(FILE* file, int* max_length, char separator);

/**
*   \brief This function initializes an array index with the zeros (fields len and pos) and empty arrays (field ptr)
*
*   @param index Array of type pointer*
*   @param lines Number of lines in array
*   @param max_length Maximum length of strings in a file
*
*   @return Nothing
*/

void initialization(struct pointer* index, const int lines,  const int max_length);

/**
*   \brief This function fills the index array with data from the file
*
*   @param file File with strings
*   @param index Array of type pointer*
*   @param lines Number of lines in array
*   @param max_length Maximum length of strings in a file
*   @param separator A symbol that is considered the end of a string
*
*   @return Nothing
*/

void fill_index_array(FILE* file, struct pointer* index, int lines, int max_length, char separator);

/**
*   \brief This function starts sorting
*
*   @param name_in,name_out Input and output files
*   @param lines Number of lines in array
*   @param index Array of type pointer*
*
*   @return Nothing
*/

int write_and_sort(const char* name_in, const char* name_out, int lines, struct pointer* index);

/**
*   \brief This function sorts text
*
*   @param len Number of lines in array
*   @param index Array of type pointer*
*   @param compare Comparator
*
*   @return Nothing
*/

void my_qsort(int len, struct pointer* index, int (*compare)(char*, char*, int, int));

/**
*   \brief This function contains string comparison rules (sort by first character)
*
*   @param str1,str2 Comparing strings
*
*   @return Negative number, if str1 is less than str2
*           0, if str1 is equal to str2
*           Positive number, if str1 is greater than str2
*/

int strcmp_forward(char* str1, char* str2, int len1, int len2);

/**
*   \brief This function fills the index array with data from the file
*
*   @param file File with strings
*   @param index Array of type pointer*
*   @param lines Number of lines in array
*   @param number_of_out Number of print
*
*   @return Nothing
*/
void print_array(FILE* file, const struct pointer* index, const int lines, int* number_of_out);

/**
*   \brief This function compares two strings which are structure fields
*
*   @param first,second Comparing structs
*
*   @return Negative number, if first.ptr is less than second.ptr
*           0, if first.ptr is equal to second.ptr
*           Positive number, if first.ptr is greater than second.ptr
*/

int comparator_struct_ptr(const void* first, const void* second);

/**
*   \brief This function contains string comparison rules (sort by last character)
*
*   @param str1,str2 Comparing strings
*
*   @return Negative number, if str1 is less than str2
*           0, if str1 is equal to str2
*           Positive number, if str1 is greater than str2
*/

int strcmp_reverse(char* str1, char* str2, int len1, int len2);

/**
*   \brief This function compares two indices of strings which are structure fields
*
*   @param first,second Comparing structs
*
*   @return Negative number, if first.pos is less than second.pos
*           0, if first.pos is equal to second.pos
*           Positive number, if first.pos is greater than second.pos
*/

int comparator_struct_pos(const void* first, const void* second);




