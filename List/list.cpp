#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <math.h>
#include <unistd.h>
#include "list.h"

#define IF_DEBUG(code) code

#define ASSERTION                                                                                    \
    /*system("\033[31m");*/                                                                              \
    fprintf(stderr, "-----------------!WARNING!----------------\n");                                 \
    fprintf(stderr, "IN FILE %s\nIN LINE %d (FUNCTION %s)\n", __FILE__, __LINE__, __FUNCTION__);     \
    printf("%s\n", TEXT_LIST_STATUSES[my_list->list_status]);                                        \
    /*system("\033[32\n");*/

#define REPORT_ABOUT_ERROR(code_of_error)                                                            \
    my_list->list_status = code_of_error;                                                            \
    ASSERTION                                                                                        \
    list_dump(my_list);

const struct call_of_dump base_arguments_of_call = {__FILE__, -1, " "};

int comparator_node(const void* first, const void* second) {
    Node first_node  = *(Node*)first;
    Node second_node = *(Node*)second;

    return abs(first_node.next) > abs(second_node.next) || abs(first_node.next) == abs(second_node.next) && abs(first_node.prev) > abs(second_node.prev);
}

void sort_by_next(List* my_list) {
    qsort(my_list->data, my_list->capacity, sizeof(my_list->data[0]), comparator_node);
    my_list->head = 1;

    my_list->data[1].prev = 0;
    my_list->data[1].next = 2;
    int now_position = 2;
    while(my_list->data[now_position].prev != POISON) {
        my_list->data[now_position].next = now_position + 1;
        my_list->data[now_position].prev = now_position - 1;
        ++now_position;
    }

    my_list->tail = now_position - 1;
}

struct call_of_dump create_struct(const char* file_name, int number, const char* function_name) {
    struct call_of_dump tmp = {};
    tmp.name_file      = file_name;
    tmp.number_of_line = number;
    tmp.name_function  = function_name;
    return tmp;
}

void list_dump(List* my_list, struct call_of_dump arguments_of_call = base_arguments_of_call) {
    printf("popados .....  (╯ ° □ °) ╯ (┻━┻) \n");
    //abort();
}

void list_verifier(List* my_list) {
    if(my_list == nullptr) {
        my_list->list_status = LIST_DATA_NULL;
        list_dump(my_list, create_struct(__FILE__, __LINE__, __FUNCTION__));
    } else if((my_list->size_list) > (my_list->capacity)) {
        my_list->list_status = LIST_OVERFLOW;
        list_dump(my_list, create_struct(__FILE__, __LINE__, __FUNCTION__));
    } else if(my_list->size_list < 0) {
        my_list->list_status = LIST_BAD_SIZE;
        list_dump(my_list, create_struct(__FILE__, __LINE__, __FUNCTION__));
    } else if(my_list->capacity < 0) {
        my_list->list_status = LIST_BAD_CAPACITY;
        list_dump(my_list, create_struct(__FILE__, __LINE__, __FUNCTION__));
    }
}
//#endif // IF_DEBUG

void draw_graph(List* my_list) {
    FILE* file = fopen("new.dot", "w");
    fprintf(file, "digraph graphname {");

    int now_position = my_list->head, old_position = my_list->head;
    now_position = my_list->data[now_position].next;

    do {
        fprintf(file, "node%d [shape=\"record\",color=\"#FF0EDD\",label=\"%d|{%d|%d}|%d\"]\n", old_position, my_list->data[old_position].prev, my_list->data[old_position].next, my_list->data[old_position].value, old_position);
        fprintf(file, "node%d [shape=\"record\",color=\"#FF0EDD\",label=\"%d|{%d|%d}|%d\"]\n", now_position, my_list->data[now_position].prev, my_list->data[now_position].next, my_list->data[now_position].value, now_position);
        printf("%d\n", now_position);

        fprintf(file, "node%d->node%d\n", old_position, now_position);


        old_position = now_position;
        now_position = my_list->data[now_position].next;

    } while(now_position != my_list->tail);

    fprintf(file, "node%d [shape=\"record\",color=\"#FF0EDD\",label=\"%d|{%d|%d}|%d\"]\n", old_position, my_list->data[old_position].prev, my_list->data[old_position].next, my_list->data[old_position].value, old_position);
    fprintf(file, "node%d [shape=\"record\",color=\"#FF0EDD\",label=\"%d|{%d|%d}|%d\"]\n", now_position, my_list->data[now_position].prev, my_list->data[now_position].next, my_list->data[now_position].value, now_position);
    printf("%d\n", now_position);

    fprintf(file, "node%d->node%d\n", old_position, now_position);


    fprintf(file, "}");
    system("dot new.dot -Tpng -O");
}

void print_list(List* my_list) {
    for(int i=0; i<my_list->capacity; ++i) {
        printf("%d ", my_list->data[i].value);
    }
    printf("\n");
    for(int i=0; i<my_list->capacity; ++i) {
        printf("%d ", my_list->data[i].next);
    }
    printf("\n");
    for(int i=0; i<my_list->capacity; ++i) {
        printf("%d ", my_list->data[i].prev);
    }
    printf("\nhead: %d, tail: %d\. nearest_free: %d\nsize_list: %d, capacity: %d\n\n", my_list->head, my_list->tail, my_list->nearest_free, my_list->size_list, my_list->capacity);
}

void list_print(List* my_list) {
    int now_index = my_list->head;

    printf("list: ");
    while(now_index != my_list->tail) {
        printf("%d ", my_list->data[now_index].value);
        now_index = my_list->data[now_index].next;
    }
    printf("%d\n", my_list->data[now_index].value);
}

LIST_STATUSES list_construct(List* my_list) {
    //my_list = (List*)calloc(1, sizeof(List));
    if(!my_list) {
        REPORT_ABOUT_ERROR(LIST_NO_CONSTRUCT)
        return my_list->list_status;
    }

    my_list->data = (Node*)calloc(BEGIN_INIT_SIZE + 1, sizeof(Node));
    if(!my_list->data) {
        REPORT_ABOUT_ERROR(LIST_BAD_MEMORY)
        return my_list->list_status;
    }

    my_list->capacity = BEGIN_INIT_SIZE;
    my_list->size_list = 0;

    list_initializate(my_list, 0);

    my_list->data[0].prev = my_list->capacity;
    my_list->data[my_list->capacity].next = -1;

    //print_list(my_list);
    my_list->head = 0;
    my_list->tail = 0;
    my_list->flag_of_sorted = true;
    my_list->nearest_free = 0;

    my_list->data[my_list->tail].next = -1;
    my_list->data[my_list->head].prev = my_list->size_list;

    my_list->list_status = LIST_OK;

    IF_DEBUG(list_verifier(my_list);)

    return LIST_OK;
}

void list_initializate(List* my_list, int begin_position) {
    for(int now_position = begin_position; now_position <= my_list->capacity; ++now_position) {
        my_list->data[now_position].value = POISON;
        my_list->data[now_position].next  = now_position + 1;
        my_list->data[now_position].prev  = now_position - 1;
    }
}

void list_destruct(List* my_list) {
    free(my_list->data);

    my_list->capacity  = POISON;
    my_list->size_list = POISON;
    my_list->head      = POISON;
    my_list->tail      = POISON;

    free(my_list);
}

LIST_STATUSES list_insert(List* my_list, int position, Elem_type value) {
    IF_DEBUG(list_verifier(my_list);)

    if(my_list->size_list + 1 >= my_list->capacity) {
        list_resize(my_list);
    }

    if(position != my_list->size_list) {
        my_list->flag_of_sorted = false;
    }

    long long temporary_free = my_list->nearest_free;
    my_list->nearest_free = my_list->data[my_list->nearest_free].next;

    if(my_list->size_list == 0) {
        my_list->head = temporary_free;
        my_list->data[my_list->head].value = value;
        my_list->data[my_list->head].next  = 1;
        my_list->data[my_list->head].prev  = my_list->nearest_free;
        my_list->nearest_free = 1;

        ++(my_list->size_list);
        print_list(my_list);

        return LIST_OK;
    }

    my_list->data[temporary_free].value = value;

    my_list->data[my_list->data[position].prev].next = temporary_free;

    my_list->data[temporary_free].next = position;
    my_list->data[temporary_free].prev = my_list->data[my_list->data[position].next].prev;

    //my_list->data[my_list->data[position].next].prev = temporary_free;
    //my_list->data[temporary_free].prev = position;
    //my_list->data[my_list->data[position].next].prev = my_list->data[position].next;


    if(position == 0) {
        my_list->head = temporary_free;
        my_list->data[temporary_free].prev = my_list->tail;
        my_list->data[my_list->tail].next = my_list->head;
    }

    if(position == my_list->size_list) {
        my_list->tail = temporary_free;
        my_list->data[my_list->data[temporary_free].prev].next = temporary_free;
        my_list->data[temporary_free].next = my_list->head;
        my_list->data[my_list->head].prev = my_list->tail;
    } else {
        my_list->data[my_list->data[position].next].prev = temporary_free;
    }

    ++(my_list->size_list);
    print_list(my_list);

    return LIST_OK;
}

LIST_STATUSES list_resize(List* my_list) {
    if(my_list->capacity > MAX_VALUE_SIZE_T / 2) {
        REPORT_ABOUT_ERROR(LIST_OVERFLOW)
    }

    my_list->data = (Node*)realloc(my_list->data, (my_list->capacity + 1) * 2 * sizeof(Node));
    if(!my_list->data) {
        REPORT_ABOUT_ERROR(LIST_BAD_MEMORY)
        return my_list->list_status;
    }

    my_list->capacity *= 2;
    list_initializate(my_list, my_list->capacity / 2);

     my_list->data[my_list->capacity].next = -1;

    return LIST_OK;
}

int get_next_free_position(List* my_list) {
    if(my_list->size_list + 1 < my_list->capacity) {
        list_resize(my_list);
    }
    ++(my_list->size_list);
    return my_list->size_list;
}

/*void list_pop_back(List* my_list, Elem_type* element) {
    *element = my_list->data[my_list->tail].value;

    int new_tail = my_list->data[my_list->tail].prev;
    my_list->data[my_list->tail].value = POISON;
    my_list->data[my_list->tail].next  = get_next_free_position(my_list);
    my_list->data[my_list->tail].prev  = POISON;
    my_list->tail = new_tail;
}

void list_delete_element(List* my_list, int position) {
    int begin_position = my_list->head;
    //printf("begin: %d\n", begin_position);
    //printf("%d %d %d\n", my_list->data[1].next, my_list->data[1].prev, my_list->data[1].value);
    while(my_list->data[begin_position].next - 1 != position) {
        begin_position = my_list->data[begin_position].next;
    }

    //printf("%d\n", my_list->data[begin_position].value);
    int prev_index = my_list->data[begin_position].prev, next_index = my_list->data[begin_position].next;
    //printf("prev_index: %d\t %d %d\n", prev_index, my_list->data[prev_index].prev, my_list->data[prev_index].next);
    //printf("next_index: %d\t %d %d\n", next_index, my_list->data[next_index].prev, my_list->data[next_index].next);
    my_list->data[begin_position].next = get_next_free_position(my_list);
    my_list->data[begin_position].prev = POISON;

    if(position != 1) {
        my_list->data[prev_index].next = next_index;
    }
    my_list->data[next_index].prev = prev_index;

    if(my_list->head == position) {
        my_list->head = next_index;
    }
}

Elem_type list_get_element(List* my_list, int position) {
    if(position > my_list->size_list) {
        printf("No such element exists\n");
        return POISON;
    }
    int begin_position = my_list->head;
    printf("pos: %d ", begin_position);
    for(int number_of_element = 0; number_of_element < position; ++number_of_element) {
        begin_position = my_list->data[begin_position].next;
        printf("%d ", begin_position);
    }

    return my_list->data[begin_position].value;
}*/
