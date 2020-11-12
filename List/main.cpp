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

    /*list_print(my_list);*/
    draw_graph(my_list);

    //int el = 0;
    //list_pop_back(my_list, &el);
    //printf("%d\n", el);

    /*list_print(my_list);
    list_delete_element(my_list, 4);
    list_delete_element(my_list, 1);

    list_print(my_list);
    print_list(my_list);

    sort_by_next(my_list);
    print_list(my_list);*/

    //printf("%d\n", list_get_element(my_list, 1));
    return 0;
}
