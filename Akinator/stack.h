#ifndef H_STACK
#define H_STACK

#define IF_DEBUG(code) //code

enum TYPE_PROPERTY_NODE {
    NOT_IS_PROPERTY = 0,
    IS_PROPERTY     = 1
};

struct information_about_node {
    size_t index_into_catalog;
    TYPE_PROPERTY_NODE is_it_property_of_node;
};

typedef information_about_node Elem_t;
const int POISON = 100000;
const information_about_node POISON_STRUCT = {POISON, NOT_IS_PROPERTY};

typedef enum {
    STACK_OK                = 0,
    STACK_ERROR             = 1,
    STACK_EMPTY             = 2,
    STACK_OVERFLOW          = 3,
    STACK_CAPACITY_OVERFLOW = 4,
    STACK_BAD_CAPACITY      = 5,
    STACK_IS_CREATED        = 6,
    STACK_DATA_NULL         = 7,
    STACK_BAD_SIZE          = 8,
    STACK_IS_DECTRUCT       = 9
} stack_t_status;

struct Stack_t {
    Elem_t* data;
    size_t size_stack;
    size_t capacity;
    stack_t_status stack_status;
};

/**
*   \brief This function checks the stack for simple errors and calls diagnostics in their case
*
*   @param node Stack
*   @param arguments_of_call This parameter contains name of file, name of function and number of line where the call came from
*
*   @return Nothing
*/

void stack_err(Stack_t* node, struct call_of_dump arguments_of_call);

void fell(Stack_t* node);

/**
*   \brief This function returns size of the stack
*
*   @param node Stack
*
*   @return Size of stack
*/

int stack_size(Stack_t* node);

/**
*   \brief This function returns capacity of the stack
*
*   @param node Stack
*
*   @return Capacity of stack
*/

int stack_capacity(Stack_t* node);

/**
*   \brief This function constructs the stack
*
*   @param node Stack
*
*   @return Nothing
*/

void stack_construct(Stack_t* node);

/**
*   \brief This function destructs the stack
*
*   @param node Stack
*
*   @return Nothing
*/

void stack_destruct(Stack_t* node);

/**
*   \brief This function create new stack
*
*   @param dimention Base size of stack
*
*   @return New stack
*/

Stack_t* new_stack(size_t dimension);

/**
*   \brief This function checks if the stack is empty
*
*   @param node Stack
*
*   @return true, is node is empty, else false
*/

bool stack_is_empty(Stack_t* node);

/**
*   \brief This function resizes data of the stack
*
*   @param node Stack
*
*   @return Nothing
*/

void stack_resize(Stack_t* node);

/**
*   \brief This function fills unused array cells by poison
*
*   @param node Stack
*
*   @return Nothing
*/

void fill_stack_stuff(Stack_t* node);

/**
*   \brief This function pushes new elements in the stack
*
*   @param node Stack
*   @param value Number which we need to push
*
*   @return Nothing
*/

void stack_push(Stack_t* node, size_t value, TYPE_PROPERTY_NODE type_property);

/**
*   \brief This function deletes last added element of the stack
*
*   @param node Stack
*
*   @return Nothing
*/

void stack_pop(Stack_t* node);

/**
*   \brief This function returns last added element of the stack
*
*   @param node Stack
*
*   @return Last added element of the stack
*/

information_about_node stack_back(Stack_t* node);

/**
*   \brief This function deletes all elements from stack
*
*   @param node Stack
*
*   @return Nothing
*/

void stack_clear(Stack_t* node);

/**
*   \brief This function deletes all elements from file
*
*   @param file File
*
*   @return Nothing
*/

void clear_file(const char* file_log_name);

#endif

/**
*   \brief This function initializations the stack
*
*   @param Nothing
*
*   @return 0, if there are no errors
*/

