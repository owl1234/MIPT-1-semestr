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
    //printf("%s\n", argv[1]);

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

int strcmpp(char* str1, char* str2) { // unsigned
    printf("%s %s\n", str1, str2);
    while(*str1 == *str2) {
        if(*str1 == '\0') {
            return 0;
        }
        str1++;
        str2++;
    }

    while(*str1 != '\0' && *str2 != '\0') {
        while(!('а' <= *str1 && *str1 <= 'я' || 'А' <= *str1 && *str1 <= 'Я') && *str1 != '\0') {
            str1++;
        }
        while(!('а' <= *str2 && *str2 <= 'я' || 'А' <= *str2 && *str2 <= 'Я') && *str2 != '\0') {
            str2++;
        }
    }

    //printf("%d\n\n", *str1-*str2);

    return *str1 - *str2;
}

int comparator(const void* first_string, const void* second_string) {
    char* first  =  (char*)first_string;
    char* second = (char*)second_string;

    return strcmpp(first, second);
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

    --lines;

    return lines;
}

FILE* open_file(FILE** file, char* file_name) {
    *file = fopen(file_name, "r");

    //if(file == nullptr) {
    //    return nullptr;
    //}

    return *file;
}

int close_file(FILE** file) {
    if(*file == nullptr) {
        return -1;
    }
    int result = fclose(*file);

    return result;
}

int sorting(int argc, char* argv[]) {
    if(argc > 4) {
        printf("Bad count of agruments.\n");
        return 3802;
    }
    /*printf("%d arguments:", argc);
    for(int i=0; i<argc; ++i)
        printf(" %s", argv[i]);
    printf("\n");*/
    FILE* poem;
    poem = open_file(&poem, argv[2]);
    if(poem == nullptr) {
        printf("File %s wasn't opened\n", argv[2]);
        return 3802;
    }

    int max_length = 0;
    int lines = number_of_lines(poem, &max_length);

    char** text = (char**)calloc(lines + 1, sizeof(char*));
    for(int i=0; i<lines; ++i) {
        text[i] = (char*)calloc(max_length + 1, sizeof(char));
    }

    //printf("lines: %d, max_length: %d\n", lines, max_length);

    int close_file_result = close_file(&poem);
    if(close_file_result == -1) {
        printf("There were problems with the file %s. Run the program again.\n", argv[1]);
        return 3802;
    }

    // End to count the length of the string


    // Begin to read the string

    poem = open_file(&poem, argv[2]);
    if(poem == nullptr) {
        return 3802;
    }

    for(int line=0; line<lines; ++line) {
        int i = 0;
        char symbol = '!';

        while(symbol != '\n') {
            fscanf(poem, "%c", &symbol);
            // printf("%d ", symbol);
            text[line][i] = symbol;
            ++i;
        }
        // printf("\n");
    }

    close_file(&poem);

    printf("----------------- read file ---------------- \n");
    for(int i=0; i<lines; ++i) {
        for(int j=0; j<max_length; ++j) {
            printf("%c", text[i][j]);
        }
        printf("\n");
    }
    printf("----------------- end read file ---------------- \n");



    // End to read the string

    qsort(text, lines, sizeof(text[0]), comparator);

    FILE *fdout = fopen(argv[3], "w");

    for (int i = 0; i < lines; ++i)
        fputs(text[i], fdout);

    fclose(fdout);
    return 0;
}
