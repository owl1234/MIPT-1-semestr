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
*   \brief This function tests function strcmp()
*
*   @param first_string,second_string Pointers to the strings being compared
*   @param number_of_test Test number
*
*   @return Nothing
*/

void testing_strcmpp(const unsigned char* first_string, const unsigned char* second_string, int* number_of_test);


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
*   \brief This function starts testing functions
*
*   @return Nothing
*/

void testing();
