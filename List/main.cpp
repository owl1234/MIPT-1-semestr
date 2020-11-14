/**
 *  @file
 *  @author Kolesnikova Xenia <heiduk.k.k.s@yandex.ru>
 *  @par Last edition
 *                  November 14, 2020, 03:01:25
 *  @par What was changed?
 *                      1. Fixed insert
 *                      2. Add delete
 *                      3. Stast formed unit-tests
 *                      4. Friday 13 is over :)
 *  @par To-do list
 *                      1. Make insert for 0
 *                      2. Make other functions
 *                      3. Make a colorful output (started)
*/

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

//#define FIRST_TEST  1
#define SECOND_TEST 2

#define BEGIN_TEST(number_of_test)                                          \
    printf("Start testing on test %d..................\n", number_of_test); \
    BEGIN_WORK_WITH_LIST

#define BEGIN_WORK_WITH_LIST                                                \
    list_construct(my_list);

#define END_TEST(number_of_test)                                            \
    printf("End testing on test %d....................\n", number_of_test); \
    END_WORK_WITH_LIST

#define END_WORK_WITH_LIST                                                  \
    list_dump(my_list, create_struct(__FILE__, __LINE__, __FUNCTION__));    \
    draw_graph(my_list);                                                    \
    list_destruct(my_list);                                                 \

int main() {
    List* my_list = (List*)calloc(1, sizeof(List));

    #ifdef FIRST_TEST
    {
        BEGIN_TEST(FIRST_TEST)

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
        list_insert(my_list, 0, 1234);

        END_TEST(FIRST_TEST)
    }
    #endif // FIRST_TEST

    #ifdef SECOND_TEST
    {
        BEGIN_TEST(SECOND_TEST)
        printf("2!\n");

        list_insert(my_list, 0, 100);
        list_insert(my_list, 1, 10);
        list_insert(my_list, 2, 20);
        list_insert(my_list, 3, 30);
        list_insert(my_list, 4, 40);
        list_insert(my_list, 5, 50);

        int delete_value = -1;
        //list_delete_element(my_list, 3, &delete_value);
        printf("Delete value: %d\n", delete_value);

        END_TEST(SECOND_TEST)
    }
    #endif // SECOND_TEST




    return 0;
}
