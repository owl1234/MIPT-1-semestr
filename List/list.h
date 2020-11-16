#include <stdio.h>

#define Elem_type int

const Elem_type POISON = -3802;
const int BEGIN_INIT_SIZE = 8;
const int MAX_VALUE_SIZE_T = (size_t)-1;

const char name_input_html_file[] = "list_dump.html";

struct call_of_dump {
    const char* name_file;
    int number_of_line;
    const char* name_function;
};

typedef enum {
    LIST_OK                = 0,
    LIST_ERROR             = 1,
    LIST_EMPTY             = 2,
    LIST_OVERFLOW          = 3,
    LIST_CAPACITY_OVERFLOW = 4,
    LIST_BAD_CAPACITY      = 5,
    LIST_IS_CREATED        = 6,
    LIST_BAD_MEMORY        = 7,
    LIST_BAD_SIZE          = 8,
    LIST_IS_DECTRUCT       = 9,
    LIST_NO_CONSTRUCT      = 10,
    LIST_DATA_NULL         = 11,
    LIST_NO_SUCH_ELEMENT   = 12,
    LIST_BAD_DATA          = 13,
} LIST_STATUSES;

const char TEXT_LIST_STATUSES[][100] {
    "List is okey",
    "Error!",
    "List is empty",
    "Size of list is very big",
    "Capacity of list is very big",
    "Capacity is lower then size",
    "List is created now",
    "Data is null",
    "Size is bad",
    "List is destructed",
    "List don't construct",
    "Data is null",
    "No such element in list",
    "Data is bad (maybe, there was a confusion with pointers)"
};

struct Node {
    long long prev;
    long long next;
    bool is_used;
    Elem_type value;
};

struct List {
    Node* data;
    size_t capacity;
    size_t size_list;
    int head;
    int tail;
    LIST_STATUSES list_status;
    bool flag_of_sorted;
    long long nearest_free;
};

const char LABEL_COLOR_FOR_VALID_ELEMENTS[]   = "blue";
const char LABEL_COLOR_FOR_UNVALID_ELEMENTS[] = "red";

const char ARROW_COLOR_FOR_VALID_ELEMENTS[]   = "purple";
const char ARROW_COLOR_FOR_UNVALID_ELEMENTS[] = "green";
const char ARROW_COLOR_FOR_FREE_ELEMENTS[] = "black";

void delete_old_information_from_file();

struct call_of_dump create_struct(const char* file_name, int number, const char* function_name);

void list_dump(List* my_list, struct call_of_dump arguments_of_call);

void list_verifier(List* my_list);

void draw_graph(List* my_list);

void file_print_list(FILE* log_file, List* my_list);

void list_print(List* my_list);

LIST_STATUSES list_construct(List* my_list);

void list_initializate(List* my_list, const long long begin_position = 0);

void list_destruct(List* my_list);

LIST_STATUSES list_insert(List* my_list, const long long physical_position, Elem_type value);

LIST_STATUSES push_first_element(List* my_list, long long temporary_free, Elem_type value);

LIST_STATUSES list_resize(List* my_list, const double quantity);

void list_push_first_element(List* my_list, Elem_type value);

LIST_STATUSES list_push_back(List* my_list, Elem_type value);

LIST_STATUSES list_push_front(List* my_list, Elem_type value);

void list_pop_back(List* my_list, Elem_type* element);

LIST_STATUSES list_delete_element(List* my_list, long long position, Elem_type* delete_value);

Elem_type list_get_logical_element(List* my_list, const long long position);

void put_free_position(List* my_list, long long position);

long long get_min_free_position(List* my_list);

long long get_max_free_position(List* my_list);

Elem_type list_get_physical_element(List* my_list, const long long position);

LIST_STATUSES list_slow_sort(List* my_list);
