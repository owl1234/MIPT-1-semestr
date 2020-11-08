#ifndef H_STACK
#define H_STACK

#define IF_CANARY_PROTECTION(code) code
#define IF_HASH_PROTECTION(code) code
#define IF_DEBUG(code) code

typedef double Elem_t;
const char identity[] = "%lg";

const int POISON = -3802;
IF_CANARY_PROTECTION(const int CANARY = -57005;)

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
    STACK_IS_DECTRUCT       = 9,
    IF_HASH_PROTECTION  (STACK_BAD_HASH   = 10),
    IF_CANARY_PROTECTION(STACK_BAD_CANARY = 11)
} stack_t_status;

struct Stack_t {
    IF_CANARY_PROTECTION(int canary_left[1] = { CANARY };)
    Elem_t* data;
    size_t size_stack;
    size_t capacity;
    stack_t_status stack_status;
    int stack_hash;
    char* name_log_file;
    IF_CANARY_PROTECTION(int canary_right[1] = { CANARY };)
};

struct call_of_dump {
    const char* name_file;
    int number_of_line;
    const char* name_function;
};

/**
*   \brief This function gets the hash of the stack
*
*   @param node Stack
*
*   @return Hash of the stack
*/

int get_hash(Stack_t* node);

/**
*   \brief This function prints int number to file
*
*   @param value Int number
*   @param file The file to which the value will be written
*
*   @return Nothing
*/

void print_Elem_T(int value, FILE* file);

/**
*   \brief This function prints double number to file
*
*   @param value Double number
*   @param file The file to which the value will be written
*
*   @return Nothing
*/

void print_Elem_T(double value, FILE* file);

/**
*   \brief This function prints char symbol to file
*
*   @param value Char symbol
*   @param file The file to which the value will be written
*
*   @return Nothing
*/

void print_Elem_T(char value, FILE* file);

/**
*   \brief Checks if the value is equal to canary
*
*   @param value Value to compare
*
*   @return true, if value is equal to canary, else false,
*/

bool is_canary(Elem_t value);

/**
*   \brief This function prints data to file
*
*   @param node Stack
*   @param file The file to which the data will be written
*
*   @return Nothing
*/

void error_print_data(Stack_t* node, FILE* file);

void error_print_array_elem_t(Elem_t* array_elem_t, const int length, FILE* file);

/**
*   \brief This function displays all information about the stack and the reason for calling it
*
*   @param node Stack
*   @param arguments_of_call This parameter contains name of file, name of function and number of line where the call came from
*
*   @return Nothing
*/

void stack_dump(Stack_t* node, struct call_of_dump arguments_of_call);

/**
*   \brief This function create the struct with specified value
*
*   @param file_name Name of the file where the call corrupted
*   @param number Number of line where the call corrupted
*   @param function_name Name of function where the call corrupted
*
*   @return Nothing
*/


struct call_of_dump create_struct(const char* file_name, int number, const char* function_name);

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

void stack_construct(Stack_t* node, const char* name_log_file);

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

void stack_push(Stack_t* node, Elem_t value);

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

Elem_t stack_back(Stack_t* node);

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

//int initialization_stack();
