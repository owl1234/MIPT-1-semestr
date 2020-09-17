/**
*   \brief This function tests function comparator()
*
*   @param first_string,second_string Pointers to the strings being compared
*   @param right_answer Correct result of comparing strings
*   @param number_of_test Test number
*
*   @return Nothing
*/

void testing_comparator(const void* first_string, const void* second_string, int right_answer, int* number_of_test);

/**
*   \brief This function tests function strcmp_forward()
*
*   @param first_string,second_string Pointers to the strings being compared
*   @param len1,len2 String length
*   @param right_answer Correct result of the function
*   @param number_of_test Test number
*
*   @return Nothing
*/

void testing_strcmp_forward(const char* first_string, const char* second_string, int len1, int len2, int right_answer, int* number_of_test);

/**
*   \brief This function tests function strcmp_reverse()
*
*   @param first_string,second_string Pointers to the strings being compared
*   @param len1,len2 String length
*   @param right_answer Correct result of the function
*   @param number_of_test Test number
*
*   @return Nothing
*/

void testing_strcmp_reverse(const char* first_string, const char* second_string, int len1, int len2, int right_answer, int* number_of_test);

/**
*   \brief This function tests function number_of_lines()
*
*   @param file_name Name of the file
*   @param correct_lines Correct count of lines
*   @param correct_max_length Correct maximum length of the lines
*   @param number_of_test Test number
*
*   @return Nothing
*/

void testing_number_of_lines(char* file_name, int correct_lines, int correct_max_length, int* number_of_test);

/**
*   \brief This function tests function int_comparator()
*
*   @param first,second Structures that need to sort
*   @param right_answer Correct result of the function
*   @param number_of_test Test number
*
*   @return Nothing
*/


void testing_int_comparator(struct pointer first, struct pointer second, int right_answer, int* number_of_test);

/**
*   \brief This function starts testing functions
*
*   @return Nothing
*/

void testing();
