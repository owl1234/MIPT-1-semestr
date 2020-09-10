#include <stdio.h>
#include "Onegin.h"

void testing_comparator(const void* first_string, const void* second_string, int right_answer, int* number_of_test) {
    int result = comparator(first_string, second_string);

    if(result < 0 && right_answer < 0 || result > 0 && right_answer > 0 || right_answer == 0 && result == 0) {
        printf("Test #%d OK\n", *number_of_test);
    } else {
        printf("Test #%d BAD: comparator(%s, %s) == %d, should be %d\n", *number_of_test, (const char*)first_string, (const char*)second_string, result, right_answer);
    }

    (*number_of_test)++;
}

void testing_strcmp(const char* first_string, const char* second_string, int right_answer, int* number_of_test) {
    int result = strcmp((unsigned char*)first_string, (unsigned char*)second_string);

    if(result < 0 && right_answer < 0 || result > 0 && right_answer > 0 || right_answer == 0 && result == 0) {
        printf("Test #%d OK\n", *number_of_test);
    } else {
        printf("Test #%d BAD: strcmp(%s, %s) == %d, should be %d\n", *number_of_test, first_string, second_string, result, right_answer);
    }

    (*number_of_test)++;
}

void testing_open_file(FILE** file, char* file_name, bool status, int* number_of_test) {
    *file = open_file(file, file_name);
    if(file == nullptr) {
        printf("PANICCC\n");
    }
    if(file != nullptr && status == true || file == nullptr && status == false) {
        printf("Test #%d OK\n", *number_of_test);
    } else {
        if(file == nullptr) {
            printf("Test #%d BAD: file %s should open, but program can't do it\n", *number_of_test, file_name);
        } else {
            printf("Test #%d BAD: file %s shouldn't open, but program can do it\n", *number_of_test, file_name);
        }
    }

    (*number_of_test)++;
}

void testing_close_file(FILE** file, char* file_name, bool status, int* number_of_test) {
    int result = close_file(file);

    if(result == 0 && status == true || result == -1 && status == false) {
        printf("Test #%d OK\n", *number_of_test);
    } else {
        if(status == true) {
            printf("Test #%d BAD: file %s should close, but program can't do it\n", *number_of_test, file_name);
        } else {
            printf("Test #%d BAD: file %s shouldn't close, but program can do it\n", *number_of_test, file_name);
        }
    }

    (*number_of_test)++;
}

void testing_number_of_lines(char* file_name, int correct_lines, int correct_max_length, int* number_of_test) {
    FILE* file;
    file = open_file(&file, file_name);

    int max_length = 0;
    int lines = number_of_lines(file, &max_length);

    if(max_length == correct_max_length && lines == correct_lines) {
        printf("est #%d OK\n", *number_of_test);
    } else if(max_length == correct_max_length) {
        printf("Test #%d BAD: max_length found correctly, lines (program count): %d, correct_lines: %d\n", *number_of_test, lines, correct_lines);
    } else if(lines == correct_lines) {
        printf("Test #%d BAD: lines found correctly, max_length (program count): %d, correctmax_length: %d\n", *number_of_test, max_length, correct_max_length);
    } else {
        printf("Test #%d BAD: max_length (program count): %d, correct_max_length: %d\nlines (program count): %d, correct_lines: %d\n", *number_of_test, max_length, correct_max_length, lines, correct_lines);
    }

    close_file(&file);

    (*number_of_test)++;
}

void testing() {
    int number_of_test = 1;

    printf("Begin testing the function strcmp\n");
    testing_strcmp("abcde", "abcdf", -1, &number_of_test);
    testing_strcmp("abcde", "abc", 1, &number_of_test);
    testing_strcmp("a", "fa", -1, &number_of_test);
    testing_strcmp("www", "www", 0, &number_of_test);

    printf("\nBegin testing the function comparator\n");
    testing_comparator("abcde", "abcdf", -1, &number_of_test);
    testing_comparator("abcde", "abc", 1, &number_of_test);
    testing_comparator("a", "fa", -1, &number_of_test);
    testing_comparator("www", "www", 0, &number_of_test);

    printf("\nBegin testing the functions open_file and close_file\n");
    FILE* file;
    testing_open_file(&file, "Onegin.txt", true, &number_of_test);
    testing_close_file(&file, "Onegin.txt", true, &number_of_test);
    testing_open_file(&file, "wtgwrgwergf.txt", false, &number_of_test);
    testing_close_file(&file, "wtgwrgwergf.txt", false, &number_of_test);

    printf("\nBegin testing the function number_of_lines\n");
    testing_number_of_lines("Onegin.txt", 5244, 68,  &number_of_test);
}
