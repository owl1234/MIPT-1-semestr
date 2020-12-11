/**
 *  @file
 *  @author Kolesnikova Xenia <heiduk.k.k.s@yandex.ru>
 *  @par Last edition
 *                  December 11, 2020, 10:17:25
 *  @par What was changed?
 *                      1. Fixed tests
 *                      2. Fixed search physical position by logical
*/

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

//#define FIRST_TEST   1
//#define SECOND_TEST  2
//#define FOURTH_TEST  4
//#define FIFTH_TEST   5
//#define SIXTH_TEST   6

int main() {
    FILE* file = fopen(name_input_html_file, "wb");
    fprintf(file, "<pre><tt>\n");
    fclose(file);

    #ifdef FIRST_TEST
    {
        List my_list = {0};
        list_construct(&my_list);

        list_insert_before(&my_list, 1, 100);
        list_insert_before(&my_list, 2, 10);
        list_insert_before(&my_list, 3, 20);
        list_insert_before(&my_list, 4, 30);
        list_insert_before(&my_list, 5, 40);
        list_insert_before(&my_list, 6, 50);
        list_insert_before(&my_list, 7, 60);
        list_insert_before(&my_list, 8, 70);

        list_dump(&my_list, INFORMATION_ABOUT_CALL);
        list_insert_before(&my_list, 8, 80);

        list_dump(&my_list, INFORMATION_ABOUT_CALL);
        list_insert_before(&my_list, 2, 90);

        list_dump(&my_list, INFORMATION_ABOUT_CALL);
        list_insert_before(&my_list, 1, 1000);

        list_dump(&my_list, INFORMATION_ABOUT_CALL);
        list_insert_before(&my_list, 0, 1234);

        list_dump(&my_list, INFORMATION_ABOUT_CALL);
        list_insert_before(&my_list, 11, 1812);
        list_insert_before(&my_list, 11, 912387);

        list_dump(&my_list, INFORMATION_ABOUT_CALL);
        list_insert_after(&my_list, 0, -8152);

        list_dump(&my_list, INFORMATION_ABOUT_CALL);
        list_destruct(&my_list);
    }
    #endif // FIRST_TEST

    #ifdef SECOND_TEST
    {
        List my_list = {0};
        list_construct(&my_list);

        list_insert_before(&my_list, 1, 10);
        list_insert_before(&my_list, 2, 20);
        list_insert_before(&my_list, 3, 30);
        list_insert_before(&my_list, 4, 40);
        list_insert_before(&my_list, 5, 50);
        list_insert_before(&my_list, 6, 60);
        list_insert_before(&my_list, 7, 70);
        list_insert_before(&my_list, 8, 80);
        list_insert_before(&my_list, 9, 90);
        list_dump(&my_list, INFORMATION_ABOUT_CALL);

        int delete_value = -1;
        list_delete_element(&my_list, 6, &delete_value);
        printf("Delete value from pos %d: %d\n", 6, delete_value);

        list_delete_element(&my_list, 3, &delete_value);
        printf("Delete value from pos %d: %d\n", 3, delete_value);

        list_dump(&my_list, INFORMATION_ABOUT_CALL);
        list_insert_before(&my_list, 1, 2000);

        list_dump(&my_list, INFORMATION_ABOUT_CALL);
        list_destruct(&my_list);
    }
    #endif // SECOND_TEST

    #ifdef FOURTH_TEST
    {
        List my_list = {0};
        list_construct(&my_list);

        list_insert_before(&my_list, 1, 50);
        list_insert_before(&my_list, 1, 60);
        list_dump(&my_list, INFORMATION_ABOUT_CALL);

        list_insert_before(&my_list, 2, 80);
        list_dump(&my_list, INFORMATION_ABOUT_CALL);

        list_insert_before(&my_list, 1, 70);
        list_dump(&my_list, INFORMATION_ABOUT_CALL);

        Elem_type search_element = list_slow_find_physical_position_by_logical(&my_list, 5);

        if(search_element == my_list.fictive)
            printf("Element on pos 5 not found\n");
        else
            printf("(before sort) Element %d: logical pos: %d, physical pos: %d\n", my_list.data[search_element].value, 5, search_element);

        search_element = list_slow_find_physical_position_by_logical(&my_list, 3);
        printf("(before sort) Element %d: logical pos: %d, physical pos: %d\n", my_list.data[search_element].value, 3, search_element);

        list_very_very_slow_sort(&my_list);

        search_element = list_slow_find_physical_position_by_logical(&my_list, 3);
        printf("(after sort)  Element %d: logical pos: %d, physical pos: %d\n", my_list.data[search_element].value, 3, search_element);


        list_dump(&my_list, INFORMATION_ABOUT_CALL);
        list_destruct(&my_list);
    }
    #endif // FOURTH_TEST

    #ifdef FIFTH_TEST
    {
        List my_list = {0};
        list_construct(&my_list);

        list_insert_before(&my_list, 1, 50);
        list_insert_before(&my_list, 2, 60);
        list_insert_before(&my_list, 3, 70);
        list_dump(&my_list, INFORMATION_ABOUT_CALL);

        list_insert_back(&my_list, 80);
        list_insert_back(&my_list, 90);
        list_dump(&my_list, INFORMATION_ABOUT_CALL);

        list_insert_front(&my_list, 200);
        list_dump(&my_list, INFORMATION_ABOUT_CALL);

        int position_delete_value = my_list.fictive;

        list_pop_front(&my_list, &position_delete_value);
        printf("Delete value from front: %d\n", my_list.data[position_delete_value].value);

        list_pop_back(&my_list, &position_delete_value);
        printf("Delete value from back: %d\n", my_list.data[position_delete_value].value);

        list_dump(&my_list, INFORMATION_ABOUT_CALL);

        list_delete_element(&my_list, 2, &position_delete_value);
        printf("Delete value from pos %d: %d\n", 2, my_list.data[position_delete_value].value);

        list_dump(&my_list, INFORMATION_ABOUT_CALL);
        list_destruct(&my_list);
    }
    #endif // FIFTH_TEST

    #ifdef SIXTH_TEST
    {
        List my_list = {0};
        list_construct(&my_list);

        list_insert_before(&my_list, 1, 10);
        list_insert_before(&my_list, 2, 339);
        list_insert_before(&my_list, 3, 910);

        list_dump(&my_list, INFORMATION_ABOUT_CALL);

        list_insert_front(&my_list, 723);
        list_dump(&my_list, INFORMATION_ABOUT_CALL);

        warning(&my_list, INFORMATION_ABOUT_CALL);

        list_destruct(&my_list);
    }
    #endif // SIXTH_TEST

    return 0;
}
