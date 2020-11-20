/**
 *  @file
 *  @author Kolesnikova Xenia <heiduk.k.k.s@yandex.ru>
 *  @par Last edition
 *                  November 20, 2020, 07:50:25
 *  @par What was changed?
 *                      1. Add insert before/after
 *                      2. Fixed beautiful print of warnings
*/

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

//#define FIRST_TEST   1
//#define SECOND_TEST  2
//#define THIRD_TEST   3
//#define FOURTH_TEST  4
//#define FIFTH_TEST 5
//#define SIXTH_TEST 6
//#define SEVENTH_TEST 7
//#define EIGHTH_TEST 8

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

        list_insert_before(my_list, 0, 100);
        list_insert_before(my_list, 1, 10);
        list_insert_before(my_list, 2, 20);
        list_insert_before(my_list, 3, 30);
        list_insert_before(my_list, 4, 40);
        list_insert_before(my_list, 5, 50);
        list_insert_before(my_list, 6, 60);
        list_insert_before(my_list, 7, 70);
        list_insert_before(my_list, 8, 80);
        list_insert_before(my_list, 9, 90);

        int delete_value = -1;
        list_delete_element(my_list, 3, &delete_value);
        printf("Delete value: %d\n", delete_value);

        list_delete_element(my_list, 6, &delete_value);
        printf("Delete value: %d\n", delete_value);

        list_insert_before(my_list, 1, 2000);

        END_TEST(SECOND_TEST)
    }
    #endif // SECOND_TEST

    #ifdef THIRD_TEST
    {
        BEGIN_TEST(THIRD_TEST)

        list_insert_before(my_list, 0, 100);
        list_insert_before(my_list, 1, 10);
        list_insert_before(my_list, 2, 20);
        list_insert_before(my_list, 3, 30);
        list_insert_before(my_list, 4, 40);

        printf("There is an element %d in logical position %d, (%d in physical position)\n", list_get_logical_element(my_list, 1), 1, list_get_physical_element(my_list, 1));

        list_insert_before(my_list, 1, 7024);
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

        list_insert_before(my_list, 0, 100);
        list_insert_before(my_list, 1, 50);
        list_insert_before(my_list, 1, 60);
        list_insert_before(my_list, 1, 70);

        printf("There is %d on the %d logical position (before sort)\n", list_get_logical_element(my_list, 2));

        list_slow_sort(my_list);

        printf("There is %d on the %d logical position (after sort)\n", list_get_logical_element(my_list, 2));

        END_TEST(FOURTH_TEST)
    }
    #endif // FOURTH_TEST

    #ifdef FIFTH_TEST
    {
        BEGIN_TEST(FIFTH_TEST)

        list_insert_before(my_list, 0, 100);
        list_insert_before(my_list, 1, 50);
        list_insert_before(my_list, 2, 60);
        list_insert_before(my_list, 3, 70);

        list_insert_back(my_list, 80);
        list_insert_back(my_list, 90);

        list_insert_front(my_list, 200);

        int delete_value = -1;
        list_pop_front(my_list, &delete_value);
        printf("Delete value: %d\n", delete_value);

        list_pop_back(my_list, &delete_value);
        printf("Delete value: %d\n", delete_value);

        END_TEST(FIFTH_TEST)
    }
    #endif // FIFTH_TEST

    #ifdef SIXTH_TEST
    {
        BEGIN_TEST(SIXTH_TEST)

        list_insert_before(my_list, 0, 10);
        list_insert_before(my_list, 1, 339);
        list_insert_before(my_list, 2, 910);

        END_TEST(SIXTH_TEST)
    }
    #endif // SIXTH_TEST

    #ifdef SEVENTH_TEST
    {
        assertion(my_list);
    }
    #endif // SEVENTH_TEST

    #ifdef EIGHTH_TEST
    {
        BEGIN_TEST(EIGHTH_TEST)

        list_insert_before(my_list, 0, 615);
        list_insert_after(my_list, 0, 120);
        list_insert_after(my_list, 0, 291);
        list_insert_before(my_list, 0, 410);

        int delete_value = 0;
        list_delete_element(my_list, 0, &delete_value);
        printf("Delete value: %d\n", delete_value);

        END_TEST(EIGHTH_TEST)
    }
    #endif // EIGHTH_TEST


    return 0;
}
