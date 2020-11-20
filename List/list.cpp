#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <math.h>
#include <unistd.h>
#include "list.h"

#define IF_DEBUG(code) code

#define ASSERTION                                                                                                                            \
    system("echo \e[31m-----------------!WARNING!----------------\e[0m");                                                                    \
    char warning_info[150] = "";                                                                                                             \
    sprintf(warning_info, "echo \"\\e[31mIN FILE %s (FUNCTION %s, LINE %d)\\e[0m\"", __FILE__, __FUNCTION__, __LINE__);                      \
    system(warning_info);                                                                                                                    \
    sprintf(warning_info, "echo \"\\e[31mList status: %s\\e[0m\"", TEXT_LIST_STATUSES[my_list->list_status]);                                \
    system(warning_info);

#define REPORT_ABOUT_ERROR(code_of_error)                                                            \
    my_list->list_status = code_of_error;                                                            \
    ASSERTION                                                                                        \
    list_dump(my_list, create_struct(__FILE__, __LINE__, __FUNCTION__));

#define VERIFY_LIST_STATUS                                                                           \
    if(status != LIST_OK) {                                                                          \
        REPORT_ABOUT_ERROR(status);                                                                  \
        return status;                                                                               \
    }

const struct call_of_dump base_arguments_of_call = {__FILE__, -1, " "};

void delete_old_information_from_file() {
    FILE* file = fopen("new.dot", "wb");
    fclose(file);
    file = fopen(name_input_html_file, "wb");
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
    FILE* log_file = fopen(name_input_html_file, "a");

    fprintf(log_file, "<pre><tt>\n");
    fprintf(log_file, "   size_list: %lu\n",      my_list->size_list);
    fprintf(log_file, "    capacity: %lu\n",      my_list->capacity);
    fprintf(log_file, "      status: %d (%s)\n", my_list->list_status, TEXT_LIST_STATUSES[my_list->list_status]);
    fprintf(log_file, "        head: %lu\n",      my_list->head);
    fprintf(log_file, "        tail: %lu\n",      my_list->tail);
    fprintf(log_file, "nearest_free: %lu\n",      my_list->nearest_free);
    fprintf(log_file, "number_of_call: %d\n", my_list->number_of_draw_graph_call);
    fprintf(log_file, "Dump call has occured in %s, in function %s, line %d\n", arguments_of_call.name_file, arguments_of_call.name_function, arguments_of_call.number_of_line);

    file_print_list(log_file, my_list);

    draw_graph(my_list);
    fprintf(log_file, "<img src=picture%d.png/>\n\n", my_list->number_of_draw_graph_call - 1);

    fclose(log_file);
}

void list_verifier(List* my_list, struct call_of_dump arguments_of_call = base_arguments_of_call) {
    if(nullptr == my_list) {
        my_list->list_status = LIST_DATA_NULL;
        list_dump(my_list, arguments_of_call);
    } else if((my_list->size_list) > (my_list->capacity)) {
        my_list->list_status = LIST_OVERFLOW;
        list_dump(my_list, arguments_of_call);
    } else if(my_list->size_list < 0) {
        my_list->list_status = LIST_BAD_SIZE;
        list_dump(my_list, arguments_of_call);
    } else if(my_list->capacity < 0) {
        my_list->list_status = LIST_BAD_CAPACITY;
        list_dump(my_list, arguments_of_call);
    }

    size_t prev_position = 0, next_position = 0;

    for(size_t now_position=my_list->head; now_position!=my_list->head; now_position = my_list->data[now_position].next) {
        prev_position = (my_list->data[now_position].prev + my_list->size_list) % my_list->size_list;
        next_position = (my_list->data[now_position].next + my_list->size_list) % my_list->size_list;
        if((my_list->data[prev_position].next + my_list->size_list) % my_list->size_list != now_position  ||
           (my_list->data[next_position].prev + my_list->size_list) % my_list->size_list != now_position) {
            my_list->list_status = LIST_BAD_DATA;
            list_dump(my_list, arguments_of_call);
           }
    }
}

void draw_graph(List* my_list) {
    FILE* file = fopen(concatenate_strings_for_draw_graph(my_list), "wb");
    fprintf(file, "digraph graphname {\nrankdir = LR;\n");

    draw_all_elements(file, my_list);
    draw_list_elements(file, my_list);
    draw_free_elements(file, my_list);

    char result_string[] = "dot picture0.dot -T png -o picture0.png"; // | eog picture0.png"; //, my_list->number_of_draw_graph_call);

    ++my_list->number_of_draw_graph_call;

    fclose(file);

    system(result_string); // "dot new.dot -Tpng -O" //get_string_to_draw_graph(my_list)

    printf("get_string_to_draw_graph: %s\n", result_string);
    //system("echo a");
}

void draw_all_elements(FILE* file, List* my_list) {
    size_t now_position = 0;

    for(size_t i=0; i<my_list->capacity; ++i) {
        fprintf(file, "node%lu [shape=\"record\", ", i);

        if(!my_list->data[i].is_used) {
            fprintf(file, "color=\"%s\", ", LABEL_COLOR_FOR_UNVALID_ELEMENTS);
        } else {
            fprintf(file, "color=\"%s\", ", LABEL_COLOR_FOR_VALID_ELEMENTS);
        }

        fprintf(file, "label=\"%lu|{%lu|%lu}|%d\"]\n", i, my_list->data[i].prev, my_list->data[i].next, my_list->data[i].value);

        if(i + 1 < my_list->capacity) {
            fprintf(file, "node%lu->node%lu [color=%s]\n", i, i+1, ARROW_COLOR_FOR_UNVALID_ELEMENTS);
        }
    }

    fprintf(file, "node%lu->node0 [color=%s]\n", my_list->capacity - 1, ARROW_COLOR_FOR_UNVALID_ELEMENTS);
}

void draw_list_elements(FILE* file, List* my_list) {
    size_t now_position = my_list->head;

    fprintf(file, "node%lu\n", now_position);
    if(my_list->size_list > 1) {
        now_position = my_list->data[now_position].next;

        while(now_position != my_list->head) {
            fprintf(file, "->node%lu", now_position);

            if(-1 == now_position) {
                now_position += my_list->size_list;
            }
            now_position = my_list->data[now_position].next;

        }
    }

    fprintf(file, "->node%lu [color=%s]\n", now_position, ARROW_COLOR_FOR_VALID_ELEMENTS);
}

void draw_free_elements(FILE* file, List* my_list) {
    size_t now_position = my_list->nearest_free;
    fprintf(file, "node%lu", now_position);

    while(my_list->data[now_position].next < my_list->capacity) {
        if(-1 == now_position) {
            now_position += my_list->size_list;
        }
        now_position = my_list->data[now_position].next;
        fprintf(file, "->node%lu", now_position);
    }

    fprintf(file, "->node%lu [color=%s]\n}\n", my_list->nearest_free, ARROW_COLOR_FOR_FREE_ELEMENTS);
}

/*char* get_string_to_draw_graph(List* my_list) {
    char* result_string = (char*)calloc(SIZE_OF_NAME_FILES, sizeof(char));


    return result_string;
}*/

char* concatenate_strings_for_draw_graph(List* my_list) {
    char* result_string = (char*)calloc(SIZE_OF_NAME_FILES, sizeof(char));

    sprintf(result_string, "picture%d.dot", my_list->number_of_draw_graph_call);
    printf("concatenate_strings_for_draw_graph: %s\n", result_string);

    return result_string;
}

void file_print_list(FILE* log_file, List* my_list) {
    fprintf(log_file, "data\n ");
    fprintf(log_file, "\tvalue: ");
    for(size_t i=0; i<my_list->capacity; ++i) {
        fprintf(log_file, "%4d ", my_list->data[i].value);
    }
    fprintf(log_file, "\n\tnext: ");
    for(size_t i=0; i<my_list->capacity; ++i) {
        fprintf(log_file, "%4lu ", my_list->data[i].next);
    }
    fprintf(log_file, "\n\tprev: ");
    for(size_t i=0; i<my_list->capacity; ++i) {
        fprintf(log_file, "%4lu ", my_list->data[i].prev);
    }
    fprintf(log_file, "\n\tis_used: ");
    for(size_t i=0; i<my_list->capacity; ++i) {
        fprintf(log_file, "%4d ", my_list->data[i].is_used);
    }
    printf("\n\n");
}

void print_list(List* my_list) {
    printf( "data\n ");
    printf( "\tvalue: ");
    for(size_t i=0; i<my_list->capacity; ++i) {
        printf( "%4d ", my_list->data[i].value);
    }
    printf( "\n\tnext: ");
    for(size_t i=0; i<my_list->capacity; ++i) {
        printf( "%4lu ", my_list->data[i].next);
    }
    printf("\n\tprev: ");
    for(size_t i=0; i<my_list->capacity; ++i) {
        printf( "%4lu ", my_list->data[i].prev);
    }
    printf( "\n\tis_used: ");
    for(size_t i=0; i<my_list->capacity; ++i) {
        printf( "%4d ", my_list->data[i].is_used);
    }
    printf("\n\n");
}

LIST_STATUSES list_construct(List* my_list) {
    if(!my_list) {
        REPORT_ABOUT_ERROR(LIST_NO_CONSTRUCT)
        return LIST_NO_CONSTRUCT;
    }

    my_list->data = (Node*)calloc(BEGIN_INIT_SIZE + 1, sizeof(Node));
    if(!my_list->data) {
        REPORT_ABOUT_ERROR(LIST_BAD_MEMORY)
        return LIST_BAD_MEMORY;
    }

    my_list->capacity = BEGIN_INIT_SIZE;
    my_list->size_list = 0;

    list_initializate(my_list, 0);

    my_list->head                      = 0;
    my_list->tail                      = 0;
    my_list->flag_of_sorted            = true;
    my_list->nearest_free              = 0;
    my_list->number_of_draw_graph_call = 0;

    my_list->data[my_list->tail].next = 0;
    my_list->data[my_list->head].prev = my_list->size_list;

    my_list->list_status = LIST_OK;

    IF_DEBUG(list_verifier(my_list, create_struct(__FILE__, __LINE__, __FUNCTION__));)

    return LIST_OK;
}

void list_initializate(List* my_list, const size_t begin_position) {
    for(size_t now_position = begin_position; now_position <= my_list->capacity; ++now_position) {
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

LIST_STATUSES list_insert_before(List* my_list, const size_t physical_position, Elem_type value) {
    IF_DEBUG(list_verifier(my_list, create_struct(__FILE__, __LINE__, __FUNCTION__));)

    if(my_list->size_list + 1 >= my_list->capacity) {
        LIST_STATUSES status = list_resize(my_list, 2);
        VERIFY_LIST_STATUS
    }

    if(physical_position != my_list->size_list) {
        my_list->flag_of_sorted = false;
    }

    size_t temporary_free = my_list->nearest_free;
    if(-1 == temporary_free) {
        temporary_free = my_list->nearest_free + my_list->size_list;
        my_list->nearest_free += my_list->size_list;
    }

    my_list->nearest_free = my_list->data[my_list->nearest_free].next;

    if(0 == my_list->size_list) {
        return list_insert_first_element(my_list, temporary_free, value);
    }

    my_list->data[temporary_free].value = value;
    my_list->data[my_list->data[physical_position].prev].next = temporary_free;
    my_list->data[temporary_free].next = physical_position;
    my_list->data[temporary_free].prev = my_list->data[physical_position].prev;

    if(physical_position != temporary_free) {
        my_list->data[physical_position].prev = temporary_free;
    }

    if(0 == physical_position) {
        my_list->head = temporary_free;
        my_list->data[temporary_free].prev = my_list->tail;
        my_list->data[my_list->tail].next = temporary_free;
    }

    if(physical_position == my_list->size_list) {
        if(1 == temporary_free - my_list->size_list)
            temporary_free = -1;
        my_list->tail = temporary_free;
        my_list->data[temporary_free].next = my_list->head;
        my_list->data[my_list->head].prev = my_list->tail;
    }

    my_list->data[temporary_free].is_used = true;
    my_list->nearest_free = get_min_free_position(my_list);

    ++(my_list->size_list);

    IF_DEBUG(list_verifier(my_list, create_struct(__FILE__, __LINE__, __FUNCTION__));)

    return LIST_OK;
}

LIST_STATUSES list_insert_after(List* my_list, const size_t physical_position, Elem_type value) {
    IF_DEBUG(list_verifier(my_list, create_struct(__FILE__, __LINE__, __FUNCTION__));)

    return list_insert_before(my_list, physical_position + 1, value);
}

LIST_STATUSES list_insert_first_element(List* my_list, size_t temporary_free, Elem_type value) {
    my_list->head = temporary_free;
    my_list->data[my_list->head].value   = value;
    my_list->data[my_list->head].next    = 1;
    my_list->data[my_list->head].prev    = my_list->tail;
    my_list->data[my_list->head].is_used = true;
    my_list->nearest_free = 1;

    ++(my_list->size_list);

    IF_DEBUG(list_verifier(my_list, create_struct(__FILE__, __LINE__, __FUNCTION__));)

    return LIST_OK;
}

LIST_STATUSES list_insert_back(List* my_list, Elem_type value) {
    LIST_STATUSES status = list_insert_before(my_list, my_list->size_list, value);

    VERIFY_LIST_STATUS

    IF_DEBUG(list_verifier(my_list, create_struct(__FILE__, __LINE__, __FUNCTION__));)

    return LIST_OK;
}

LIST_STATUSES list_insert_front(List* my_list, Elem_type value) {
    LIST_STATUSES status = list_insert_before(my_list, 0, value);

    VERIFY_LIST_STATUS

    IF_DEBUG(list_verifier(my_list, create_struct(__FILE__, __LINE__, __FUNCTION__));)

    return LIST_OK;
}

LIST_STATUSES list_resize(List* my_list, const double quantity) {
    IF_DEBUG(list_verifier(my_list, create_struct(__FILE__, __LINE__, __FUNCTION__));)

    if(quantity * my_list->capacity > MAX_VALUE_SIZE_T || quantity < 0) {
        REPORT_ABOUT_ERROR(LIST_OVERFLOW)
    }

    int new_capacity = my_list->capacity * quantity;
    int old_capacity = my_list->capacity;

    my_list->data = (Node*)realloc(my_list->data, (new_capacity + 1) * sizeof(Node));
    if(!my_list->data) {
        REPORT_ABOUT_ERROR(LIST_BAD_MEMORY)
        return LIST_BAD_MEMORY;
    }

    my_list->capacity = new_capacity;

    if(quantity > 1) {
        list_initializate(my_list, old_capacity + 1);
    }

    IF_DEBUG(list_verifier(my_list, create_struct(__FILE__, __LINE__, __FUNCTION__));)

    return LIST_OK;
}

LIST_STATUSES list_delete_element(List* my_list, size_t position, Elem_type* delete_value) {
    IF_DEBUG(list_verifier(my_list, create_struct(__FILE__, __LINE__, __FUNCTION__));)

    if(position < 0) {
        REPORT_ABOUT_ERROR(LIST_NO_SUCH_ELEMENT)
        return LIST_NO_SUCH_ELEMENT;
    }

    if(my_list->size_list * 4 <= my_list->capacity) {
        list_resize(my_list, 0.25);
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

    IF_DEBUG(list_verifier(my_list, create_struct(__FILE__, __LINE__, __FUNCTION__));)

    return LIST_OK;
}

LIST_STATUSES list_pop_front(List* my_list, Elem_type* value) {
    LIST_STATUSES status = list_delete_element(my_list, 0, value);

    VERIFY_LIST_STATUS

    IF_DEBUG(list_verifier(my_list, create_struct(__FILE__, __LINE__, __FUNCTION__));)

    return LIST_OK;
}

LIST_STATUSES list_pop_back(List* my_list, Elem_type* value) {
    LIST_STATUSES status = list_delete_element(my_list, my_list->size_list, value);

    VERIFY_LIST_STATUS

    IF_DEBUG(list_verifier(my_list, create_struct(__FILE__, __LINE__, __FUNCTION__));)

    return LIST_OK;
}


void put_free_position(List* my_list, size_t position) {
    IF_DEBUG(list_verifier(my_list, create_struct(__FILE__, __LINE__, __FUNCTION__));)

    if(position >= my_list->capacity) {
        REPORT_ABOUT_ERROR(LIST_NO_SUCH_ELEMENT)
        return;
    }

    size_t now_position = my_list->nearest_free;

    while(now_position < position) {
        now_position = my_list->data[now_position].next;
    }

    size_t prev_position = my_list->data[now_position].prev;
    size_t next_position = now_position;

    size_t prev_of_prev_position = my_list->data[prev_position].prev;
    size_t next_of_prev_position = my_list->data[prev_position].next;

    size_t prev_of_next_position = my_list->data[next_position].prev;
    size_t next_of_next_position = my_list->data[next_position].next;

    my_list->data[next_position].prev = position;
    my_list->data[position].next = next_position;

    if(!my_list->data[prev_of_next_position].is_used) {
        my_list->data[prev_position].next = position;
        my_list->data[position].prev = prev_position;
    }

    my_list->nearest_free = get_min_free_position(my_list);
}

size_t get_min_free_position(List* my_list) {
    IF_DEBUG(list_verifier(my_list, create_struct(__FILE__, __LINE__, __FUNCTION__));)

    size_t now_position = my_list->nearest_free;

    while(!my_list->data[my_list->data[now_position].prev].is_used && now_position > my_list->data[now_position].prev) {
        now_position = my_list->data[now_position].prev;
    }

    return now_position;
}

size_t get_max_free_position(List* my_list) {
    IF_DEBUG(list_verifier(my_list, create_struct(__FILE__, __LINE__, __FUNCTION__));)

    size_t now_position = my_list->nearest_free;

    while(!my_list->data[now_position].is_used && my_list->data[now_position].next < my_list->capacity) {
        now_position = my_list->data[now_position].next;
    }

    return now_position;
}

Elem_type list_get_logical_element(List* my_list, const size_t position) {
    IF_DEBUG(list_verifier(my_list, create_struct(__FILE__, __LINE__, __FUNCTION__));)

    if(position > my_list->capacity) {
        REPORT_ABOUT_ERROR(LIST_NO_SUCH_ELEMENT)
        return LIST_NO_SUCH_ELEMENT;
    }

    if(!my_list->data[position].is_used) {
        my_list->list_status = LIST_NO_SUCH_ELEMENT;
        ASSERTION
        return POISON;
    }

    if(my_list->flag_of_sorted) {
        return my_list->data[position].value;
    }

    size_t begin_position = my_list->head;
    for(size_t number_of_element = 0; number_of_element < position; ++number_of_element) {
        if(my_list->data[begin_position].next >= my_list->capacity) {
            my_list->list_status = LIST_NO_SUCH_ELEMENT;
            ASSERTION
            return POISON;
        }

        begin_position = my_list->data[begin_position].next;
    }

    return my_list->data[begin_position].value;
}

Elem_type list_get_physical_element(List* my_list, const size_t position) {
    IF_DEBUG(list_verifier(my_list, create_struct(__FILE__, __LINE__, __FUNCTION__));)

    if(position > my_list->capacity) {
        REPORT_ABOUT_ERROR(LIST_NO_SUCH_ELEMENT)
        return LIST_NO_SUCH_ELEMENT;
    }

    return my_list->data[position].value;
}

LIST_STATUSES list_slow_sort(List* my_list) {
    IF_DEBUG(list_verifier(my_list, create_struct(__FILE__, __LINE__, __FUNCTION__));)

    if(!my_list->flag_of_sorted) {
        List* sorted_list_by_next = (List*)calloc(1, sizeof(List));
        list_construct(sorted_list_by_next);

        size_t now_position = 0;

        for(size_t i=0; i<my_list->size_list; ++i) {
            sorted_list_by_next->data[i].value   = my_list->data[now_position].value;
            now_position = my_list->data[now_position].next;
        }

        for(size_t i=0; i<my_list->size_list; ++i) {
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

void assertion(List* my_list) {
    ASSERTION
}
