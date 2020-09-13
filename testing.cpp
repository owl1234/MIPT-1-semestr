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
        printf("Test #%d BAD: strcmp_forward(%s, %s, %d, %d) == %d, should be %d\n", *number_of_test, first_string, second_string, len1, len2, result, right_answer);
    }

    (*number_of_test)++;
}

void testing_strcmp_reverse(const char* first_string, const char* second_string, int len1, int len2, int right_answer, int* number_of_test) {
    int result = strcmp_reverse((char*)first_string, (char*)second_string, len1, len2); // unsigned

    if(result < 0 && right_answer < 0 || result > 0 && right_answer > 0 || right_answer == 0 && result == 0) {
        printf("Test #%d OK\n", *number_of_test);
    } else {
        printf("Test #%d BAD: testing_strcmp_reverse(%s, %s, %d, %d) == %d, should be %d\n", *number_of_test, first_string, second_string, len1, len2, result, right_answer);
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

void testing_int_comparator(struct pointer first, struct pointer second, int right_answer, int* number_of_test) {
    int result = int_comparator(&first, &second); // unsigned

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
    testing_strcmp_forward("abcde", "abcdf", 5, 5, -1, &number_of_test);
    testing_strcmp_forward("abcde", "abc", 5, 3, 1, &number_of_test);
    testing_strcmp_forward("a", "fa", 1, 2, -1, &number_of_test);
    testing_strcmp_forward("www", "www", 3, 3, 0, &number_of_test);

    printf("\nBegin testing the function strcmp_reverse\n");
    testing_strcmp_reverse("edcba", "fdcba", 5, 5, -1, &number_of_test);
    testing_strcmp_reverse("edcba", "cba", 5, 3, 1, &number_of_test);
    testing_strcmp_reverse("a", "af", 1, 2, -1, &number_of_test);
    testing_strcmp_reverse("www", "www", 3, 3, 0, &number_of_test);

    printf("\nBegin testing the function number_of_lines\n");
    testing_number_of_lines("testing/1.txt", 1, 1, &number_of_test);
    testing_number_of_lines("testing/2.txt", 0, 0, &number_of_test);
    testing_number_of_lines("testing/3.txt", 4, 6, &number_of_test);
    testing_number_of_lines("testing/4.txt", 3802, 1000, &number_of_test);

    printf("\nBegin testing the function int_comparator\n");
    struct pointer test1, test2;
    test1.ptr = "a";
    test1.len = 1;
    test1.pos = 4;

    test2.ptr = "ertg";
    test2.len = 4;
    test2.pos = 239;
    testing_int_comparator(test1, test2, -1, &number_of_test);

    test1.ptr = "adfg";
    test1.len = 4;
    test1.pos = 66;

    test2.ptr = "dd";
    test2.len = 2;
    test2.pos = 1;
    testing_int_comparator(test1, test2, 1, &number_of_test);

    test1.ptr = "efdds32";
    test1.len = 7;
    test1.pos = 666;

    test2.ptr = "213r43r";
    test2.len = 7;
    test2.pos = 666;
    testing_int_comparator(test1, test2, 0, &number_of_test);


    //testing_number_of_lines("testing/5.txt", 1, 1, &number_of_test);
    /*printf("\nBegin testing the function comparator\n");
    testing_comparator("abcde", "abcdf", -1, &number_of_test);
    testing_comparator("abcde", "abc", 1, &number_of_test);
    testing_comparator("a", "fa", -1, &number_of_test);
    testing_comparator("www", "www", 0, &number_of_test);*/
}
