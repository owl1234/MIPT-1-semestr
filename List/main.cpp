/**
 *  @file
 *  @author Kolesnikova Xenia <heiduk.k.k.s@yandex.ru>
 *  @par Last edition
 *                  November 13, 2020, 00:30:25
 *  @par What was changed?
 *                      1. Add dump
 *  @par To-do list
 *                      1. Make insert for 0
 *                      2. Make other functions
 *                      3. Make a colorful output
 *                      4. Strange output!
*/

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

int main() {
    List* my_list = (List*)calloc(1, sizeof(List));
    list_construct(my_list);

    print_list(my_list);
    list_insert(my_list, 0, 100);

    list_insert(my_list, 1, 10);
    list_insert(my_list, 2, 20);
    list_insert(my_list, 3, 30);
    list_insert(my_list, 4, 40);
    list_insert(my_list, 5, 50);
    list_insert(my_list, 6, 60);
    list_insert(my_list, 7, 70);
    list_insert(my_list, 7, 80);
    list_insert(my_list, 2, 90);
    list_insert(my_list, 1, 1000);

    printf("%d\n", list_get_element(my_list, 2));
    printf("%d\n", list_get_element(my_list, 7));

    list_dump(my_list, create_struct(__FILE__, __LINE__, __FUNCTION__));

    //list_insert(my_list, 0, 3802);

    draw_graph(my_list);

    return 0;
}
