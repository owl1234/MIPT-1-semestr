#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <math.h>
#include <unistd.h>
#include "list.h"

#define IF_DEBUG(code) code

#define ASSERTION                                                                                    \
    /*system("[\033[01;32m\]");*/                                                                    \
    fprintf(stderr, "\n-----------------!WARNING!----------------\n");                               \
    fprintf(stderr, "IN FILE %s\nIN LINE %d (FUNCTION %s)\n", __FILE__, __LINE__, __FUNCTION__);     \
    printf("%s\n\n", TEXT_LIST_STATUSES[my_list->list_status]);                                      \
    /*system("\033[32\n");*/

#define REPORT_ABOUT_ERROR(code_of_error)                                                            \
    my_list->list_status = code_of_error;                                                            \
    ASSERTION                                                                                        \
    list_dump(my_list);

const struct call_of_dump base_arguments_of_call = {__FILE__, -1, " "};

void delete_old_information_from_file() {
    FILE* file = fopen("new.dot", "wb");
    fclose(file);
}

struct call_of_dump create_struct(const char* file_name, int number, const char* function_name) {
    struct call_of_dump tmp = {};
    tmp.name_file      = file_name;
    tmp.number_of_line = number;
    tmp.name_function  = function_name;
    return tmp;
}

void list_dump(List* my_list, struct call_of_dump arguments_of_call = base_arguments_of_call) {
    FILE* log_file = fopen(name_input_html_file, "wb");

    fprintf(log_file, "<pre><tt>\n");
    fprintf(log_file, "   size_list: %d\n",      my_list->size_list);
    fprintf(log_file, "    capacity: %d\n",      my_list->capacity);
    fprintf(log_file, "      status: %d (%s)\n", my_list->list_status, TEXT_LIST_STATUSES[my_list->list_status]);
    fprintf(log_file, "        head: %d\n",      my_list->head);
    fprintf(log_file, "        tail: %d\n",      my_list->tail);
    fprintf(log_file, "nearest_free: %d\n",      my_list->nearest_free);

    file_print_list(log_file, my_list);

    fprintf(log_file, "<img src=new.dot.png/>");
    fclose(log_file);
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

    int prev_position = 0, next_position = 0;

    for(int now_position=0; now_position<my_list->size_list; ++now_position) {
        if(my_list->data[(my_list->data[now_position].prev + my_list->size_list) % my_list->size_list].next != now_position ||
           my_list->data[(my_list->data[now_position].next + my_list->size_list) % my_list->size_list].prev != now_position) {
            my_list->list_status = LIST_BAD_DATA;
            list_dump(my_list, create_struct(__FILE__, __LINE__, __FUNCTION__));
           }
    }
}

void draw_graph(List* my_list) {
    FILE* file = fopen("new.dot", "a");
    fprintf(file, "digraph graphname {\n rankdir = LR;\n");

    long long now_position = my_list->head, old_position = my_list->head;

    for(int i=0; i<my_list->capacity; ++i) {
        fprintf(file, "node%ld [shape=\"record\", ", i);

        if(!my_list->data[i].is_used) {
            fprintf(file, "color=\"%s\", ", LABEL_COLOR_FOR_UNVALID_ELEMENTS);
        } else {
            fprintf(file, "color=\"%s\", ", LABEL_COLOR_FOR_VALID_ELEMENTS);
        }

        fprintf(file, "label=\"%d|{%d|%d}|%d\"]\n", i, my_list->data[i].prev, my_list->data[i].next, my_list->data[i].value);
        if(i < my_list->capacity - 1) {
            fprintf(file, "node%d->node%d [color=%s]\n", i, i+1, ARROW_COLOR_FOR_UNVALID_ELEMENTS);
        }
    }

    now_position = my_list->head;
    int now_length = 0;
    do {
        fprintf(file, "node%d->", now_position);

        if(now_position == -1) {
            now_position += my_list->size_list;
        }
        now_position = my_list->data[now_position].next;
        ++now_length;

    } while(now_position != my_list->head);

    fprintf(file, "node%d [color=%s]\n", now_position, ARROW_COLOR_FOR_VALID_ELEMENTS);

    now_position = my_list->nearest_free;
    fprintf(file, "node%d", now_position);

    while(my_list->data[now_position].next < my_list->capacity) {
        if(now_position == -1) {
            now_position += my_list->size_list;
        }
        now_position = my_list->data[now_position].next;
        fprintf(file, "->node%d", now_position);
        printf("%d\n", now_position);
    }

    fprintf(file, "[color=%s]}\n", ARROW_COLOR_FOR_FREE_ELEMENTS);

    system("dot new.dot -Tpng -O");
}

void file_print_list(FILE* log_file, List* my_list) {
    fprintf(log_file, "data\n ");
    fprintf(log_file, "\tvalue: ");
    for(int i=0; i<my_list->capacity; ++i) {
        fprintf(log_file, "%4d ", my_list->data[i].value);
    }
    fprintf(log_file, "\n\tnext: ");
    for(int i=0; i<my_list->capacity; ++i) {
        fprintf(log_file, "%4d ", my_list->data[i].next);
    }
    fprintf(log_file, "\n\tprev: ");
    for(int i=0; i<my_list->capacity; ++i) {
        fprintf(log_file, "%4d ", my_list->data[i].prev);
    }
    fprintf(log_file, "\n\tis_used: ");
    for(int i=0; i<my_list->capacity; ++i) {
        fprintf(log_file, "%4d ", my_list->data[i].is_used);
    }
    printf("\n\n");
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

void list_initializate(List* my_list, const long long begin_position) {
    for(int now_position = begin_position; now_position <= my_list->capacity; ++now_position) {
        my_list->data[now_position].value   = POISON;
        my_list->data[now_position].next    = now_position + 1;
        my_list->data[now_position].prev    = now_position - 1;
        my_list->data[now_position].is_used = false;
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

LIST_STATUSES list_insert(List* my_list, const long long physical_position, Elem_type value) {
    IF_DEBUG(list_verifier(my_list);)

    if(my_list->size_list + 1 >= my_list->capacity) {
        list_resize(my_list, 2);
    }

    if(physical_position != my_list->size_list) {
        my_list->flag_of_sorted = false;
    }

    long long temporary_free = my_list->nearest_free;
    if(temporary_free == -1) {
        temporary_free = my_list->nearest_free + my_list->size_list;
        my_list->nearest_free += my_list->size_list;
    }

    my_list->nearest_free = my_list->data[my_list->nearest_free].next;

    if(my_list->size_list == 0) {
        return push_first_element(my_list, temporary_free, value);
    }

    my_list->data[temporary_free].value = value;
    my_list->data[my_list->data[physical_position].prev].next = temporary_free;
    my_list->data[temporary_free].next = physical_position;
    my_list->data[temporary_free].prev = my_list->data[physical_position].prev;

    if(physical_position != temporary_free) {
        my_list->data[physical_position].prev = temporary_free;
    }

    if(physical_position == 0) {
        my_list->head = temporary_free;
        my_list->data[temporary_free].prev = my_list->tail;
        my_list->data[my_list->tail].next = temporary_free;
    }

    if(physical_position == my_list->size_list) {
        if(temporary_free - my_list->size_list == 1)
            temporary_free = -1;
        my_list->tail = temporary_free;
        my_list->data[temporary_free].next = my_list->head; //temporary_free;
        my_list->data[my_list->head].prev = my_list->tail;
    }

    my_list->data[temporary_free].is_used = true;
    my_list->nearest_free = get_min_free_position(my_list);

    ++(my_list->size_list);

    return LIST_OK;
}

LIST_STATUSES push_first_element(List* my_list, long long temporary_free, Elem_type value) {
    my_list->head = temporary_free;
    my_list->data[my_list->head].value   = value;
    my_list->data[my_list->head].next    = 1;
    my_list->data[my_list->head].prev    = -1;
    my_list->data[my_list->head].is_used = true;
    my_list->nearest_free = 1;

    ++(my_list->size_list);

    return LIST_OK;
}

LIST_STATUSES list_push_back(List* my_list, Elem_type value) {
    LIST_STATUSES status = list_insert(my_list, my_list->size_list, value);

    if(status != LIST_OK) {
        REPORT_ABOUT_ERROR(status);
        return status;
    }

    return LIST_OK;
}

LIST_STATUSES list_push_front(List* my_list, Elem_type value) {
    LIST_STATUSES status = list_insert(my_list, 0, value);

    if(status != LIST_OK) {
        REPORT_ABOUT_ERROR(status);
        return status;
    }

    return LIST_OK;
}

LIST_STATUSES list_resize(List* my_list, const double quantity) {
    if(quantity * my_list->capacity > MAX_VALUE_SIZE_T || quantity < 0) {
        REPORT_ABOUT_ERROR(LIST_OVERFLOW)
    }

    long long new_capacity = my_list->capacity * quantity;
    long long old_capacity = my_list->capacity;

    my_list->data = (Node*)realloc(my_list->data, (new_capacity + 1) * sizeof(Node));
    if(!my_list->data) {
        REPORT_ABOUT_ERROR(LIST_BAD_MEMORY)
        return my_list->list_status;
    }

    my_list->capacity = new_capacity;

    if(quantity > 1) {
        list_initializate(my_list, old_capacity + 1);
    }

    return LIST_OK;
}

LIST_STATUSES list_delete_element(List* my_list, long long position, Elem_type* delete_value) {

    if(position < 0) {
        REPORT_ABOUT_ERROR(LIST_NO_SUCH_ELEMENT)
        return my_list->list_status;
    }

    if(my_list->size_list * 2 <= my_list->capacity) {
        list_resize(my_list, 0.5);
    }

    if(my_list->size_list != position) {
        my_list->flag_of_sorted = false;
    }

    *delete_value = my_list->data[position].value;

    if(my_list->head == position) {
        my_list->head = my_list->data[my_list->head].next;
    }

    if(my_list->tail == position) {
        my_list->tail = my_list->data[my_list->tail].prev;
    }

    my_list->data[my_list->data[position].prev].next = my_list->data[position].next;
    my_list->data[my_list->data[position].next].prev = my_list->data[position].prev;

    my_list->data[position].is_used = false;

    put_free_position(my_list, position);

    --my_list->size_list;

    return LIST_OK;
}

void put_free_position(List* my_list, long long position) {
    if(position >= my_list->capacity) {
        REPORT_ABOUT_ERROR(LIST_NO_SUCH_ELEMENT)
        return;
    }

    int now_position = my_list->nearest_free;

    while(now_position < position) {
        now_position = my_list->data[now_position].next;
    }

    int prev_position = my_list->data[now_position].prev;
    int next_position = now_position;

    int prev_of_prev_position = my_list->data[prev_position].prev;
    int next_of_prev_position = my_list->data[prev_position].next;

    int prev_of_next_position = my_list->data[next_position].prev;
    int next_of_next_position = my_list->data[next_position].next;

    my_list->data[next_position].prev = position;
    my_list->data[position].next = next_position;

    if(my_list->data[prev_of_next_position].is_used == false) {
        my_list->data[prev_position].next = position;
        my_list->data[position].prev = prev_position;
    }

    my_list->nearest_free = get_min_free_position(my_list);
}

long long get_min_free_position(List* my_list) {
    long long now_position = my_list->nearest_free;

    while(my_list->data[my_list->data[now_position].prev].is_used == false && now_position > my_list->data[now_position].prev) {
        now_position = my_list->data[now_position].prev;
    }

    return now_position;
}

long long get_max_free_position(List* my_list) {
    long long now_position = my_list->nearest_free;

    while(my_list->data[now_position].is_used == false && my_list->data[now_position].next < my_list->capacity) {
        now_position = my_list->data[now_position].next;
    }

    return now_position;
}

Elem_type list_get_logical_element(List* my_list, const long long position) {
    if(position > my_list->capacity) {
        REPORT_ABOUT_ERROR(LIST_NO_SUCH_ELEMENT)
        return my_list->list_status;
    }

    if(my_list->data[position].is_used == false) {
        my_list->list_status = LIST_NO_SUCH_ELEMENT;
        ASSERTION
        return POISON;
    }

    if(my_list->flag_of_sorted == true) {
        return my_list->data[position].value;
    }

    int begin_position = my_list->head;
    for(int number_of_element = 0; number_of_element < position; ++number_of_element) {
        if(my_list->data[begin_position].next >= my_list->capacity) {
            my_list->list_status = LIST_NO_SUCH_ELEMENT;
            ASSERTION
            return POISON;
        }

        begin_position = my_list->data[begin_position].next;
    }

    return my_list->data[begin_position].value;
}

Elem_type list_get_physical_element(List* my_list, const long long position) {
    if(position > my_list->capacity ) {
        REPORT_ABOUT_ERROR(LIST_NO_SUCH_ELEMENT)
        return my_list->list_status;
    }

    return my_list->data[position].value;
}

LIST_STATUSES list_slow_sort(List* my_list) {
    IF_DEBUG(list_verifier(my_list);)

    if(my_list->list_status != LIST_OK) {
        REPORT_ABOUT_ERROR(my_list->list_status)
        return my_list->list_status;
    }

    if(!my_list->flag_of_sorted) {
        List* sorted_list_by_next = (List*)calloc(1, sizeof(List));
        list_construct(sorted_list_by_next);

        int now_position = 0;

        for(int i=0; i<my_list->size_list; ++i) {
            sorted_list_by_next->data[i].value   = my_list->data[now_position].value;
            now_position = my_list->data[now_position].next;
        }

        for(int i=0; i<my_list->size_list; ++i) {
            my_list->data[i].value   = sorted_list_by_next->data[i].value;
            my_list->data[i].is_used = true;
            my_list->data[i].next    = (i + 1) % (my_list->size_list + 1);
            my_list->data[i].prev    = (i + my_list->size_list) % (my_list->size_list + 1);
        }

        list_initializate(my_list, my_list->size_list);

        my_list->flag_of_sorted = true;
        my_list->head           = 0;
        my_list->tail           = my_list->size_list - 1;

        my_list->data[my_list->head].prev = my_list->tail;
        my_list->data[my_list->tail].next = my_list->head;

        free(sorted_list_by_next);
    }

    return LIST_OK;
}
