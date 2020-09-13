/**
 * @file
 * @author Kolesnikova Xenia <heiduk.k.k.s@yandex.ru>
 * @version 1.0 *
 */

// #include "TXLib.h"
#include <stdio.h>
#include "Onegin.h"
#include "testing.h"
#include <stdlib.h>
#include <string.h>
#include <typeinfo>
#include <assert.h>



int main (int argc, char* argv[]) {
    // Begin to count the length of the string
    if(argv[1][0] == 't') {
        testing();
    } else if(argv[1][0] == 's') {
        return sorting(argc, argv);
    } else {
        printf("Bad agruments. t - if you want to testing, s - if you want to sorting\n");
        return 3802;
    }

    return 0;
}

int strcmp_forward(char* str1, char* str2, int len1, int len2) { // unsigned
    while(*(str1 + 1) != '\n' && !('a' <= *str1 && *str1 <= 'z' || 'A' <= *str1 && *str1 <= 'Z')) {
        str1++;
    }
    while(*(str2 + 1) != '\n' && !('a' <= *str2 && *str2 <= 'z' || 'A' <= *str2 && *str2 <= 'Z')) {
        str2++;
    }
    while(*(str1 + 1) != '\n' && *(str2 + 1) != '\n' && *str1 == *str2) {
        while(*(str1 + 1) != '\n' && !('a' <= *str1 && *str1 <= 'z' || 'A' <= *str1 && *str1 <= 'Z')) {
            str1++;
        }
        while(*(str2 + 1) != '\n' && !('a' <= *str2 && *str2 <= 'z' || 'A' <= *str2 && *str2 <= 'Z')) {
            str2++;
        }

        if(*str1 != *str2) {
            break;
        }

        str1++;
        str2++;
    }

    if(*str1 == *str2) {
        if(*(str1 + 1) == '\n' && *(str2 + 1) == '\n') {
            return 0;
        } else if(*(str1 + 1) == '\n') {
            return -1;
        } else {
            return 1;
        }
    }

    return *str1 - *str2;
}

int strcmp_reverse(char* str1, char* str2, int len1, int len2) { // unsigned
    str1 += len1 - 1;
    str2 += len2 - 1;
    int pos1 = len1 - 1, pos2 = len2 - 1;
    while(pos1 - 1 >= 0 && !('a' <= *str1 && *str1 <= 'z' || 'A' <= *str1 && *str1 <= 'Z')) {
        str1--;
        pos1--;
    }
    while(pos2 - 1 >= 0 && !('a' <= *str2 && *str2 <= 'z' || 'A' <= *str2 && *str2 <= 'Z')) {
        str2--;
        pos2--;
    }
    while(pos1 - 1 >= 0 && pos2 - 1 >= 0 && *str1 == *str2) {
        while(pos1 - 1 >= 0 && !('a' <= *str1 && *str1 <= 'z' || 'A' <= *str1 && *str1 <= 'Z')) {
            str1--;
            pos1--;
        }
        while(pos2 - 1 >= 0 && !('a' <= *str2 && *str2 <= 'z' || 'A' <= *str2 && *str2 <= 'Z')) {
            str2--;
            pos2--;
        }

        if(*str1 != *str2) {
            break;
        }

        str1--;
        str2--;
        pos1--;
        pos2--;
    }

    if(*str1 == *str2) {
        if(pos1 - 1 >= 0 && pos2 - 1 >= 0 || pos1 == pos2 && pos1 == 0) {
            return 0;
        } else if(pos1 - 1 >= 0) {
            return 1;
        } else {
            return -1;
        }
    }

    return *str1 - *str2;
}

/*int comparator(const void* first_string, const void* second_string) {
    char* first  =  (char*)first_string;
    char* second = (char*)second_string;

    return strcmp_forward(first, second);
}*/

int int_comparator(const void* first, const void* second) {
    struct pointer* first_number  =  (struct pointer*)first;
    struct pointer* second_number = (struct pointer*)second;

    return (*first_number).pos - (*second_number).pos;
}


int number_of_lines(FILE* file, int* max_length) {
    int lines = 0, now_length = 0;
    char symbol = '!';

    while(!feof(file)) {
        fscanf(file,"%c", &symbol);
        if(symbol == '\n') {
            ++lines;
            *max_length = ((*max_length) < now_length ? now_length : (*max_length));
            now_length = 0;
        } else {
            ++now_length;
        }
    }

    if(lines > 0) {
        --lines;
    }

    return lines;
}

/*void qsorting(char** array, int left, int right, struct pointer* index) {
    if(left < right) {
        int now_left = left, now_right = right;
        int middle = (left + right) / 2;
        do {
            while(strcmp_forward(index[now_left].ptr, index[middle].ptr) < 0) {
                ++now_left;
            }
            while(strcmp_forward(index[now_right].ptr, index[middle].ptr) > 0) {
                --now_right;
            }
            if(now_left <= now_right) {
                struct pointer temp = index[now_left];
                index[now_left] = index[now_right];
                index[now_right] = temp;
                ++now_left;
                --now_right;
            }

        } while(now_left <= now_right);
        qsorting(array, left, now_right, index);
        qsorting(array, now_left, right, index);
    }
}*/

void my_qsort(char** array, int len, struct pointer* index, int (*compare)(char*, char*, int, int)) {
    for(int i=0; i<len; ++i) {
        int min_ind = i;
        for(int j=i+1; j<len; ++j) {
            if((*compare)(index[min_ind].ptr, index[j].ptr, index[min_ind].len, index[j].len) > 0) {
                min_ind = j;
            }
        }
        struct pointer temp = index[i];
        index[i] = index[min_ind];
        index[min_ind] = temp;
    }
}

int sorting(int argc, char* argv[]) {
    if(argc > 4) {
        printf("Bad count of agruments.\n");
        return 3802;
    }

    FILE* poem = fopen(argv[2], "r");
    if(poem == nullptr) {
        printf("File %s wasn't opened\n", argv[2]);
        return 3802;
    }

    int max_length = 0;
    int lines = number_of_lines(poem, &max_length);
    ++max_length;

    char** text = (char**)calloc(lines + 1, sizeof(char*));
    for(int i=0; i<lines; ++i) {
        text[i] = (char*)calloc(max_length + 1, sizeof(char));
    }

    int close_file_result = fclose(poem);
    if(close_file_result == -1) {
        printf("There were problems with the file %s. Run the program again.\n", argv[1]);
        return 3802;
    }

    // End to count the length of the string


    // Begin to read the string

    poem = fopen(argv[2], "r");
    if(poem == nullptr) {
        return 3802;
    }

    struct pointer* index = (struct pointer*)calloc(lines, sizeof(struct pointer));

    for(int line=0; line<lines; ++line) {
        int i = 0;
        char symbol = '!';
        while(symbol != '\n') {
            fscanf(poem, "%c", &symbol);
            text[line][i] = symbol;
            ++i;
        }
        index[line].ptr = text[line];
        index[line].len = i;
        index[line].pos = line;
    }

    fclose(poem);

    // End to read the string

    //qsorting(text, 0, lines - 1, index);

    //qsort(text, lines, sizeof(text[0]), comparator);

    FILE* file_result = fopen(argv[3], "w");

    fprintf(file_result, "-------------------------------------- BEGIN DO PART I --------------------------------------\n");

    my_qsort(text, lines, index, strcmp_forward);

    for (int i=0; i<lines; ++i) {
        fprintf(file_result, "%s", index[i].ptr);
    }
    fprintf(file_result, "-------------------------------------- END DO PART I --------------------------------------\n");

    qsort(index, lines, sizeof(index[0]), int_comparator);

    fprintf(file_result, "-------------------------------------- BEGIN DO PART II --------------------------------------\n");

    my_qsort(text, lines, index, strcmp_reverse);
    for (int i=0; i<lines; ++i) {
        fprintf(file_result, "%s", index[i].ptr);
    }
    fprintf(file_result, "-------------------------------------- END DO PART II --------------------------------------\n");

    fprintf(file_result, "-------------------------------------- BEGIN DO PART III --------------------------------------\n");
    for (int i=0; i<lines; ++i) {
        fprintf(file_result, "%s", text[i]);
    }
    fprintf(file_result, "-------------------------------------- END DO PART III --------------------------------------\n");


    fclose(file_result);

    // ...............................PART II..............................

    return 0;
}
