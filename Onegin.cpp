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
#include <math.h>

void help() {
    printf("This program sorts strings from file in alphabetical order (without taking into account the signs of pre).\n"
            "The required parameters look like this:\n"
                    "[-h]                                  - if you want to read this help again\n"
                    "[-s name_input_file name_output_file] - if you want to sorts strings\n"
                    "[-t]                                  - if you want to test this program.\n"
                    "For more information, go here: https://github.com/owl1234/String-sorter\n");
}

int sorting(const int argc, const char* argv[]) {
    assert(isfinite(argc));
    assert(argv != NULL);

    const char* name_in  = argv[2];
    const char* name_out = argv[3];

    FILE* poem = fopen(name_in, "r");
    if(poem == nullptr) {
        printf("File %s don't open\n", name_in);
        return error_number;
    }

    int max_length = 0;
    int lines = number_of_lines(poem, &max_length, '\n');

    int close_file_result = fclose(poem);
    if(close_file_result == -1) {
        printf("There were problems with the file %s. Run the program again.\n", name_in);
        return error_number;
    }

    poem = fopen(name_in, "r");
    if(poem == nullptr) {
        printf("There were problems with the file %s. Run the program again.\n", name_in);
        return error_number;
    }

    struct pointer* index = (struct pointer*)calloc(lines, sizeof(struct pointer));
    initialization(index, lines, max_length);
    fill_index_array(poem, index, lines, max_length, '\n');

    close_file_result = fclose(poem);
    if(close_file_result == -1) {
        printf("There were problems with the file %s. Run the program again.\n", name_in);
        return error_number;
    }

    int status = write_and_sort(name_in, name_out, lines, index);
    if(status == error_number) {
        printf("File %s don't open\n", name_in);
    }

    return status;
}

int number_of_lines(FILE* file, int* max_length, char separator) {
    assert(file       != NULL);
    assert(max_length != NULL);

    int lines = 0, now_length = 0;
    char symbol = '!';
    if(separator == '!') {
        symbol = '@';
    }

    while(!feof(file)) {
        fscanf(file,"%c", &symbol);
        if(symbol == separator) {
            ++lines;
            if((*max_length) < now_length) {
                *max_length = now_length;
            }
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

void initialization(struct pointer* index, const int lines,  const int max_length) {
    assert(isfinite(lines));
    assert(isfinite(max_length));
    assert(index != NULL);

    for(int i=0; i<lines; ++i) {
        index[i].len = 0;
        index[i].pos = 0;
        index[i].ptr = (char*)calloc(max_length + 3, sizeof(char));
    }
}

void fill_index_array(FILE* file, struct pointer* index, int lines, int max_length, char separator) {
    assert(isfinite(lines));
    assert(file  != NULL);
    assert(index != NULL);

    char** text = (char**)calloc(lines + 1, sizeof(char*));
    for(int i=0; i<lines; ++i) {
        text[i] = (char*)calloc(max_length + 3, sizeof(char));
    }

    for(int line=0; line<lines; ++line) {
        int i = 0;
        char symbol = '!';
        if(separator == '!') {
            symbol = '@';
        }

        while(symbol != separator) {
            fscanf(file, "%c", &symbol);
            text[line][i] = symbol;
            ++i;
        }

        strcpy(index[line].ptr, text[line]);
        index[line].len = i;
        index[line].pos = line;
    }

    for(int i=0; i<lines; ++i) {
        free(text[i]);
    }

    free(text);
}

int write_and_sort(const char* name_in, const char* name_out, int lines, struct pointer* index) {
    assert(isfinite(lines));
    assert(name_in  != NULL);
    assert(name_out != NULL);
    assert(index    != NULL);

    FILE* file_result = fopen(name_out, "w");
    if(file_result == nullptr) {
        return error_number;
    }

    int number_of_out = 1;

    my_qsort(lines, index, strcmp_forward);
    print_array(file_result, index, lines, &number_of_out);

    qsort(index, lines, sizeof(index[0]), comparator_struct_ptr);
    print_array(file_result, index, lines, &number_of_out);

    qsort(index, lines, sizeof(index[0]), comparator_struct_pos);
    print_array(file_result, index, lines, &number_of_out);

    fclose(file_result);

    return 0;
}

void my_qsort(int len, struct pointer* index, int (*compare)(char*, char*, int, int)) {
    assert(isfinite(len));
    assert(index != NULL);

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

int strcmp_forward(char* str1, char* str2, int len1, int len2) {
    assert(isfinite(len1));
    assert(isfinite(len2));
    assert(str1 != NULL);
    assert(str2 != NULL);

    while(*(str1 + 1) != '\n' && !is_alpha_EN(*str1)) {
        str1++;
    }
    while(*(str2 + 1) != '\n' && !is_alpha_EN(*str2)) {
        str2++;
    }

    while(*(str1 + 1) != '\n' && *(str2 + 1) != '\n' && *str1 == *str2) {
        while(*(str1 + 1) != '\n' && !is_alpha_EN(*str1)) {
            str1++;
        }
        while(*(str2 + 1) != '\n' && !is_alpha_EN(*str2)) {
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

bool is_alpha_EN(char symbol) {
    return ('a' <= symbol && symbol <= 'z' || 'A' <= symbol && symbol <= 'Z');
}

void print_array(FILE* file, const struct pointer* index, const int lines, int* number_of_out) {
    assert(isfinite(lines));
    assert(file          != NULL);
    assert(index         != NULL);
    assert(number_of_out != NULL);

    fprintf(file, "-------------------------------------- BEGIN DO PART %d ---------------------------------------\n", *number_of_out);
    for (int i=0; i<lines; ++i) {
        fprintf(file, "%s", index[i].ptr);
    }
    fprintf(file, "--------------------------------------- END DO PART %d ----------------------------------------\n\n", *number_of_out);

    (*number_of_out)++;
}


int comparator_struct_ptr(const void* first, const void* second) {
    struct pointer first_struct  = *(struct pointer*)first;
    struct pointer second_struct = *(struct pointer*)second;

    return strcmp_reverse(first_struct.ptr, second_struct.ptr, first_struct.len, second_struct.len);
}

int strcmp_reverse(char* str1, char* str2, int len1, int len2) {
    assert(isfinite(len1));
    assert(isfinite(len2));
    assert(str1 != NULL);
    assert(str2 != NULL);

    str1 += len1 - 1;
    str2 += len2 - 1;
    int pos1 = len1 - 1;
    int pos2 = len2 - 1;
    while(pos1 - 1 >= 0 && !is_alpha_EN(*str1)) { str1--, pos1--; }
    while(pos2 - 1 >= 0 && !is_alpha_EN(*str2)) { str2--; pos2--; }

    while(pos1 - 1 >= 0 && pos2 - 1 >= 0 && *str1 == *str2) {
        while(pos1 - 1 >= 0 && !is_alpha_EN(*str1)) { str1--; pos1--; }
        while(pos2 - 1 >= 0 && !is_alpha_EN(*str2)) { str2--; pos2--; }

        if(*str1 != *str2) {
            break;
        }

        str1--, pos1--;
        str2--, pos2--;
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

int comparator_struct_pos(const void* first, const void* second) {
    assert(first  != NULL);
    assert(second != NULL);

    struct pointer* first_number  = (struct pointer*)first;
    struct pointer* second_number = (struct pointer*)second;

    return (*first_number).pos - (*second_number).pos;
}
