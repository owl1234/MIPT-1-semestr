#define CANARY_PROTECTION(code) code
#define HASH_PROTECTION(code) code

typedef int Elem_t;
const int POISON = -3802;
CANARY_PROTECTION(const int CANARY = -57005;)

typedef enum {
    STACK_OK                = 0,
    STACK_ERROR             = 1,
    STACK_EMPTY             = 2,
    STACK_OVERFLOW          = 3,
    STACK_CAPACITY_OVERFLOW = 4,
    STACK_SMALL_CAPACITY    = 5,
    STACK_IS_CREATED        = 6,
    HASH_PROTECTION  (STACK_BAD_HASH   = 7),
    CANARY_PROTECTION(STACK_BAD_CANARY = 8)
} stack_t_status;

struct Stack_t {
    CANARY_PROTECTION(int canary_left[1] = { CANARY };)
    Elem_t* data;
    size_t size_stack;
    size_t capacity;
    stack_t_status stack_status;
    int stack_hash;
    CANARY_PROTECTION(int canary_right[1] = { CANARY };)
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

int GetHash(Stack_t* node);

/**
*   \brief This function prints int number to file
*
*   @param value Int number
*   @param file The file to which the value will be written
*
*   @return Nothing
*/

void PrintElem_T(int value, FILE* file);

/**
*   \brief This function prints double number to file
*
*   @param value Double number
*   @param file The file to which the value will be written
*
*   @return Nothing
*/

void PrintElem_T(double value, FILE* file);

/**
*   \brief This function prints char symbol to file
*
*   @param value Char symbol
*   @param file The file to which the value will be written
*
*   @return Nothing
*/

void PrintElem_T(char value, FILE* file);

/**
*   \brief Checks if the value is equal to canary
*
*   @param value Value to compare
*
*   @return true, if value is equal to canary, else false,
*/

bool IsCanary(Elem_t value);

/**
*   \brief This function prints data to file
*
*   @param node Stack
*   @param file The file to which the data will be written
*
*   @return Nothing
*/

void ErrorPrintData(Stack_t* node, FILE* file);

/**
*   \brief This function gets the number of digits of a number
*
*   @param number Number
*
*   @return Number of digits
*/

int get_len_indent(size_t number);

/**
*   \brief This function prints indetns (tabulations and space bars to file
*
*   @param count_indent Number of space bars
*   @param file The file to which the data will be written
*
*   @return Nothing
*/

void write_indent(FILE* file, int count_indent);

/**
*   \brief This function displays all information about the stack and the reason for calling it
*
*   @param node Stack
*   @param arguments_of_call This parameter contains name of file, name of function and number of line where the call came from
*
*   @return Nothing
*/

void StackDump(Stack_t* node, struct call_of_dump arguments_of_call);

/**
*   \brief This function checks the stack for simple errors and calls diagnostics in their case
*
*   @param node Stack
*   @param arguments_of_call This parameter contains name of file, name of function and number of line where the call came from
*
*   @return Nothing
*/

void StackErr(Stack_t* node, struct call_of_dump arguments_of_call);

/**
*   \brief This function returns size of the stack
*
*   @param node Stack
*
*   @return Size of stack
*/

int StackSize(Stack_t* node);

/**
*   \brief This function returns capacity of the stack
*
*   @param node Stack
*
*   @return Capacity of stack
*/

int StackCapacity(Stack_t* node);

/**
*   \brief This function constructs the stack
*
*   @param node Stack
*
*   @return Nothing
*/

void StackConstruct(Stack_t* node);

/**
*   \brief This function destructs the stack
*
*   @param node Stack
*
*   @return Nothing
*/

void StackDestruct(Stack_t* node);

/**
*   \brief This function create new stack
*
*   @param dimention Base size of stack
*
*   @return New stack
*/

Stack_t* NewStack(size_t dimension);

/**
*   \brief This function checks if the stack is empty
*
*   @param node Stack
*
*   @return true, is node is empty, else false
*/

bool StackIsEmpty(Stack_t* node);

/**
*   \brief This function resizes data of the stack
*
*   @param node Stack
*
*   @return Nothing
*/

void StackResize(Stack_t* node);

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

void StackPush(Stack_t* node, int value);

/**
*   \brief This function deletes last added element of the stack
*
*   @param node Stack
*
*   @return Nothing
*/

void StackPop(Stack_t* node);

/**
*   \brief This function returns last added element of the stack
*
*   @param node Stack
*
*   @return Last added element of the stack
*/

Elem_t StackBack(Stack_t* node);

/**
*   \brief This function deletes all elements from stack
*
*   @param node Stack
*
*   @return Nothing
*/

void StackClear(Stack_t* node);

/**
*   \brief This function deletes all elements from file
*
*   @param file File
*
*   @return Nothing
*/

void ClearFile(const char* name_log_file);

/**
*   \brief This function initializations the stack
*
*   @param Nothing
*
*   @return 0, if there are no errors
*/

int initialization_stack();
