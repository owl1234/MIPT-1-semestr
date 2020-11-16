/**
 *  @file
 *  @author Kolesnikova Xenia <heiduk.k.k.s@yandex.ru>
 *  @par Last edition
 *                  November 16, 2020, 13:03:25
 *  @par What was changed?
 *                      1. Add pop_back and pop_front
 *                      2. Add check for list validity everywhere
 *  @par To-do list
 *                      1. Fix the function call (that is, drawing the picture manually)
*/

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

//#define FIRST_TEST   1
//#define SECOND_TEST  2
//#define THIRD_TEST   3
//#define FOURTH_TEST  4
#define FIFTH_TEST 5

#define BEGIN_TEST(number_of_test)                                          \
    delete_old_information_from_file();                                     \
    printf("Start testing on test %d..................\n", number_of_test); \
    BEGIN_WORK_WITH_LIST

#define BEGIN_WORK_WITH_LIST                                                \
    list_construct(my_list);

#define END_TEST(number_of_test)                                            \
    printf("End testing on test %d....................\n", number_of_test); \
    END_WORK_WITH_LIST

#define END_WORK_WITH_LIST                                                  \
    list_dump(my_list, create_struct(__FILE__, __LINE__, __FUNCTION__));    \
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
        list_insert(my_list, 11, 1812);
        list_insert(my_list, 11, 912387);

        END_TEST(FIRST_TEST)
    }
    #endif // FIRST_TEST

    #ifdef SECOND_TEST
    {
        BEGIN_TEST(SECOND_TEST)

        list_insert(my_list, 0, 100);
        list_insert(my_list, 1, 10);
        list_insert(my_list, 2, 20);
        list_insert(my_list, 3, 30);
        list_insert(my_list, 4, 40);
        list_insert(my_list, 5, 50);
        list_insert(my_list, 6, 60);
        list_insert(my_list, 7, 70);
        list_insert(my_list, 8, 80);
        list_insert(my_list, 9, 90);

        int delete_value = -1;
        list_delete_element(my_list, 3, &delete_value);
        printf("Delete value: %d\n", delete_value);
        list_delete_element(my_list, 6, &delete_value);
        printf("Delete value: %d\n", delete_value);

        list_insert(my_list, 1, 2000);

        END_TEST(SECOND_TEST)
    }
    #endif // SECOND_TEST

    #ifdef THIRD_TEST
    {
        BEGIN_TEST(THIRD_TEST)

        list_insert(my_list, 0, 100);
        list_insert(my_list, 1, 10);
        list_insert(my_list, 2, 20);
        list_insert(my_list, 3, 30);
        list_insert(my_list, 4, 40);

        printf("There is an element %d in logical position %d, (%d in physical position)\n", list_get_logical_element(my_list, 1), 1, list_get_physical_element(my_list, 1));

        list_insert(my_list, 1, 7024);
        printf("There is an element %d in logical position %d, (%d in physical position)\n", list_get_logical_element(my_list, 1), 1, list_get_physical_element(my_list, 1));

        int delete_value = -1;
        list_delete_element(my_list, 1, &delete_value);
        printf("There is an element %d in logical position %d, (%d in physical position)\n", list_get_logical_element(my_list, 1), 1, list_get_physical_element(my_list, 1));


        END_TEST(THIRD_TEST)
    }
    #endif // THIRD_TEST

    #ifdef FOURTH_TEST
    {
        BEGIN_TEST(FOURTH_TEST)

        list_insert(my_list, 0, 100);
        list_insert(my_list, 1, 50);
        list_insert(my_list, 1, 60);
        list_insert(my_list, 1, 70);

        printf("There is %d on the %d logical position (before sort)\n", list_get_logical_element(my_list, 2));

        list_slow_sort(my_list);

        printf("There is %d on the %d logical position (after sort)\n", list_get_logical_element(my_list, 2));

        END_TEST(FOURTH_TEST)
    }
    #endif // FOURTH_TEST

    #ifdef FIFTH_TEST
    {
        BEGIN_TEST(FIFTH_TEST)

        list_insert(my_list, 0, 100);
        list_insert(my_list, 1, 50);
        list_insert(my_list, 2, 60);
        list_insert(my_list, 3, 70);

        list_push_back(my_list, 80);
        list_push_back(my_list, 90);

        list_push_front(my_list, 200);

        int delete_value = -1;
        list_pop_front(my_list, &delete_value);
        printf("Delete value: %d\n", delete_value);

        list_pop_back(my_list, &delete_value);
        printf("Delete value: %d\n", delete_value);

        END_TEST(FIFTH_TEST)
    }
    #endif // FIFTH_TEST


    return 0;
}
