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
    STACK_BAD_HASH          = 7,
    STACK_BAD_CANARY        = 8,
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

int GetHash(Stack_t* node);

void PrintElem_T(int value, FILE* file);

void PrintElem_T(double value, FILE* file);

void PrintElem_T(char value, FILE* file);

bool IsCanary(Elem_t value);

void ErrorPrintData(Stack_t* node, FILE* file);

int get_len_indent(size_t number);

void write_indent(FILE* file, int count_indent);

void StackDump(Stack_t* node, struct call_of_dump arguments_of_call);

void StackErr(Stack_t* node, struct call_of_dump arguments_of_call);

int StackSize(Stack_t* node);

int StackCapacity(Stack_t* node);

void StackConstruct(Stack_t* node);

void StackDestruct(Stack_t* node);

Stack_t* NewStack(size_t dimension);

int StackIsEmpty(Stack_t* node);

void StackResize(Stack_t* node);

void fill_stack_stuff(Stack_t* node);

void StackPush(Stack_t* node, int value);

void StackPop(Stack_t* node);

Elem_t StackBack(Stack_t* node);

void StackClear(Stack_t* node);

void ClearFile(const char* name_log_file);

int initialization_stack();
