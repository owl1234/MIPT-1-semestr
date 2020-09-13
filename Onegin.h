
struct pointer {
    char* ptr;
    int len;
    int pos;
};

/**
*   \brief This function returns result of the comparing strings
*
*   @param first_string,second_string Comparing strings
*
*   @return strcmp(str1, str2):
*           Negative number, if str1 is less than str2
*           0, if str1 is equal to str2
*           Positive number, if str1 is greater than str2
*/

int comparator(const void* first_string, const void* second_string);

/**
*   \brief This function contains string comparison rules
*
*   @param str1,str2 Comparing strings
*
*   @return Negative number, if str1 is less than str2
*           0, if str1 is equal to str2
*           Positive number, if str1 is greater than str2
*/

int strcmp_forward(char* str1, char* str2, int len1, int len2); // unsigned

/**
*   \brief This function calculates number of lines in the file
*
*   @param file File with strings
*   @param max_length Maximum length of strings in a file
*
*   @return Number of lines in the file
*/

int number_of_lines(FILE* file, int* max_length);

/**
*   \brief This function close the file
*
*   @param file The file we want to close
*
*   @return Nothing
*/

void my_qsort(char** array, int len, struct pointer* index, int (*compare)(char*, char*));

int sorting(int argc, char* argv[]);

void qsorting(char** array, int left, int right, struct pointer* index);

int strcmp_reverse(char* str1, char* str2, int len1, int len2);

int int_comparator(const void* first, const void* second);
