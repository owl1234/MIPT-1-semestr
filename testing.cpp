#include <stdio.h>
#include "Onegin.h"

/*void testing_comparator(const void* first_string, const void* second_string, int right_answer, int* number_of_test) {
    int result = comparator(first_string, second_string);

    if(result < 0 && right_answer < 0 || result > 0 && right_answer > 0 || right_answer == 0 && result == 0) {
        printf("Test #%d OK\n", *number_of_test);
    } else {
        printf("Test #%d BAD: comparator(%s, %s) == %d, should be %d\n", *number_of_test, (const char*)first_string, (const char*)second_string, result, right_answer);
    }

    (*number_of_test)++;
}*/

void testing_strcmp_forward(const char* first_string, const char* second_string, int len1, int len2, int right_answer, int* number_of_test) {
    int result = strcmp_forward((char*)first_string, (char*)second_string, len1, len2); // unsigned

    if(result < 0 && right_answer < 0 || result > 0 && right_answer > 0 || right_answer == 0 && result == 0) {
        printf("Test #%d OK\n", *number_of_test);
    } else {
        printf("Test #%d BAD: strcmp(%s, %s) == %d, should be %d\n", *number_of_test, first_string, second_string, result, right_answer);
    }

    (*number_of_test)++;
}

void testing_number_of_lines(char* file_name, int correct_lines, int correct_max_length, int* number_of_test) {
    FILE* file = fopen(file_name, "r");

    int max_length = 0;
    int lines = number_of_lines(file, &max_length);

    if(max_length == correct_max_length && lines == correct_lines) {
        printf("Test #%d OK\n", *number_of_test);
    } else if(max_length == correct_max_length) {
        printf("Test #%d BAD: max_length found correctly, lines (program count): %d, correct_lines: %d\n", *number_of_test, lines, correct_lines);
    } else if(lines == correct_lines) {
        printf("Test #%d BAD: lines found correctly, max_length (program count): %d, correctmax_length: %d\n", *number_of_test, max_length, correct_max_length);
    } else {
        printf("Test #%d BAD: max_length (program count): %d, correct_max_length: %d\nlines (program count): %d, correct_lines: %d\n", *number_of_test, max_length, correct_max_length, lines, correct_lines);
    }

    fclose(file);

    (*number_of_test)++;
}

void testing() {
    int number_of_test = 1;

    printf("Begin testing the function strcmp\n");
    testing_strcmp_forward("abcde", "abcdf", 5, 5, -1, &number_of_test);
    testing_strcmp_forward("abcde", "abc", 5, 3, 1, &number_of_test);
    testing_strcmp_forward("a", "fa", 1, 2, -1, &number_of_test);
    testing_strcmp_forward("www", "www", 3, 3, 0, &number_of_test);

    /*printf("\nBegin testing the function comparator\n");
    testing_comparator("abcde", "abcdf", -1, &number_of_test);
    testing_comparator("abcde", "abc", 1, &number_of_test);
    testing_comparator("a", "fa", -1, &number_of_test);
    testing_comparator("www", "www", 0, &number_of_test);*/
}
