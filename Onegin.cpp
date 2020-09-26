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
#include "main.h"

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
    int size_buffer = size_of_buffer(poem);

    char* buffer = (char*)calloc(size_buffer + 5, sizeof(char));
    int lines = 0;
    struct pointer* index = read_buffer(poem, buffer, size_buffer, &lines);

    int status = write_and_sort(name_in, name_out, lines, index, buffer);
    if(status == error_number) {
        printf("File %s don't open\n", name_in);
    }

    free(index);
    free(buffer);

    return status;
}

int size_of_buffer(FILE* file) {
    int length = 0;
    if (!fseek(file, 0, SEEK_END)) {
        length = ftell(file);
    }
    fseek(file, 0, SEEK_SET);

    return length;
}

struct pointer* read_buffer(FILE* file, char* buffer, int size_buffer, int* lines) {
    int status = fread(buffer, sizeof(char), size_buffer, file);

    if(status != size_buffer) {
        printf("Error in read file\n");
        return nullptr;
    }

    *lines = number_of_symbols(buffer, '\n');

    struct pointer* index = (struct pointer*)calloc(*lines, sizeof(struct pointer));
    fill_index_array(buffer, index, size_buffer, '\n');

    return index;
}

void fill_index_array(char* buffer, struct pointer* index, int size_buffer, char separator) {
    int now_line = 0, now_length = 0;
    (*index).ptr = buffer;
    char now_symbol = *buffer;

    while(now_symbol != '\0') {
        now_symbol = *buffer;
        if(now_symbol == separator) {
            (*index).len = now_length + 1;
            ++index;
            ++buffer;
            (*index).ptr = buffer;
            now_length = 0;
        } else {
            ++buffer;
            ++now_length;
        }
    }
}

int number_of_symbols(char* buffer, char separator) {
    char now_symbol = *buffer;
    int number = 0;

    while(now_symbol != '\0') {
        now_symbol = *buffer;
        if(now_symbol == separator) {
            ++number;
        }

        ++buffer;
    }

    return number;
}

int write_and_sort(const char* name_in, const char* name_out, int lines, struct pointer* index, char* buffer) {
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

    print_line(file_result, buffer, "buffer");

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
        print_line(file, index[i].ptr, "struct_pointer");
    }

    fprintf(file, "--------------------------------------- END DO PART %d ----------------------------------------\n\n", *number_of_out);

    (*number_of_out)++;
}

void print_line(FILE* file, char* line, char* flag_print) {
    assert(file != NULL);
    assert(line != NULL);

    char separator = '\n';

    if(flag_print == "buffer") {
        fprintf(file, "--------------------------------------- BEGIN DO END PART  ----------------------------------------\n");
        separator = '\0';
    }

    char* pt = line;
    while(*pt != separator) {
        fputc(*pt, file);
        ++pt;
    }

    if(separator != '\0') {
        fputc(*pt, file);
    }

    if(flag_print == "buffer") {
        fprintf(file, "--------------------------------------- END DO END PART  ----------------------------------------\n");
    }
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
