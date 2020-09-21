#include <stdio.h>
#include "Onegin.h"

void testing_strcmp_forward(const char* first_string, const char* second_string, const int len1, const int len2, const int right_answer, int* number_of_test) {
    int result = strcmp_forward((char*)first_string, (char*)second_string, len1, len2);

    if(result < 0 && right_answer < 0 || result > 0 && right_answer > 0 || right_answer == 0 && result == 0) {
        printf("Test #%d OK\n", *number_of_test);
    } else {
        printf("Test #%d BAD: strcmp_forward(%s, %s, %d, %d) == %d, should be %d\n", *number_of_test, first_string, second_string, len1, len2, result, right_answer);
    }

    (*number_of_test)++;
}

void testing_strcmp_reverse(const char* first_string, const char* second_string, const int len1, const int len2, const int right_answer, int* number_of_test) {
    int result = strcmp_reverse((char*)first_string, (char*)second_string, len1, len2);

    if(result < 0 && right_answer < 0 || result > 0 && right_answer > 0 || right_answer == 0 && result == 0) {
        printf("Test #%d OK\n", *number_of_test);
    } else {
        printf("Test #%d BAD: testing_strcmp_reverse(%s, %s, %d, %d) == %d, should be %d\n", *number_of_test, first_string, second_string, len1, len2, result, right_answer);
    }

    (*number_of_test)++;
}

void testing_number_of_lines(const char* file_name, const int correct_lines, const int correct_max_length, const char separator, int* number_of_test) {
    FILE* file = fopen(file_name, "r");
    if(file == nullptr) {
        printf("Test #%d BAD: file %s don't open\n", *number_of_test, file_name);
        return;
    }

    int max_length = 0;
    int lines = number_of_lines(file, &max_length, separator);

    if(max_length == correct_max_length && lines == correct_lines) {
        printf("Test #%d OK\n", *number_of_test);
    } else if(max_length == correct_max_length) {
        printf("Test #%d BAD: max_length found correctly, lines (program count): %d, correct_lines: %d\n", *number_of_test, lines, correct_lines);
    } else if(lines == correct_lines) {
        printf("Test #%d BAD: lines found correctly, max_length (program count): %d, correctmax_length: %d\n", *number_of_test, max_length, correct_max_length);
    } else {
        printf("Test #%d BAD: max_length (program count): %d, correct_max_length: %d\nlines (program count): %d, correct_lines: %d\n", *number_of_test, max_length, correct_max_length, lines, correct_lines);
    }

    int status = fclose(file);
    if(status == -1) {
        printf("Test #%d BAD: file %s don't close\n", *number_of_test, file_name);
        return;
    }

    (*number_of_test)++;
}

void testing_comparator_struct_pos(const struct pointer first, const struct pointer second, const int right_answer, int* number_of_test) {
    int result = comparator_struct_pos(&first, &second);

    if(result < 0 && right_answer < 0 || result > 0 && right_answer > 0 || right_answer == 0 && result == 0) {
        printf("Test #%d OK\n", *number_of_test);
    } else {
        printf("Test #%d BAD: int_comparator({%s, %d, %d}, {%s, %d, %d}) == %d, should be %d\n", *number_of_test, first.ptr, first.len, first.pos, second.ptr, second.len, second.pos, result, right_answer);
    }

    (*number_of_test)++;
}

void testing() {
    int number_of_test = 1;

    printf("Begin testing the function strcmp_forward\n");
    testing_strcmp_forward("abcde\n", "abcdf\n", 5, 5, -1, &number_of_test);
    testing_strcmp_forward("abcde\n", "abc\n", 5, 3, 1, &number_of_test);
    testing_strcmp_forward("qwe\n", "qwe\n", 3, 3, 0, &number_of_test);
    testing_strcmp_forward("a\n", "fa\n", 1, 2, -1, &number_of_test);

    printf("\nBegin testing the function strcmp_reverse\n");
    testing_strcmp_reverse("edcba", "fdcba", 5, 5, -1, &number_of_test);
    testing_strcmp_reverse("edcba", "cba", 5, 3, 1, &number_of_test);
    testing_strcmp_reverse("a", "af", 1, 2, -1, &number_of_test);
    testing_strcmp_reverse("www", "www", 3, 3, 0, &number_of_test);

    printf("\nBegin testing the function number_of_lines\n");
    testing_number_of_lines("testing/1.txt", 1, 1, '\n', &number_of_test);
    testing_number_of_lines("testing/2.txt", 0, 0, '\n', &number_of_test);
    testing_number_of_lines("testing/3.txt", 4, 6, '\n',  &number_of_test);
    testing_number_of_lines("testing/4.txt", 3802, 1000, '\n', &number_of_test);

    printf("\nBegin testing the function int_comparator\n");
    struct pointer test1 = {"a",    1,   4};
    struct pointer test2 = {"ertg", 4, 239};
    testing_comparator_struct_pos(test1, test2, -1, &number_of_test);

    test1 = {"adfg", 4, 66};
    test2 = {"dd",   2,  1};
    testing_comparator_struct_pos(test1, test2, 1, &number_of_test);

    test1 = {"efdds32", 7, 666};
    test2 = {"213r43r", 7, 666};
    testing_comparator_struct_pos(test1, test2, 0, &number_of_test);
}
