#include <stdio.h>

enum OPERATIONS_CODES {
    OPERATION_CODE_HLT             = 0,
    OPERATION_CODE_PUSH            = 1,
    OPERATION_CODE_POP             = 2,
    OPERATION_CODE_ADD             = 3,
    OPERATION_CODE_SUB             = 4,
    OPERATION_CODE_MUL             = 5,
    OPERATION_CODE_DIV             = 6,
    OPERATION_CODE_OUT             = 7,
    OPERATION_CODE_IN              = 8,
    OPERATION_CODE_JMP             = 9,
    OPERATION_CODE_CMP             = 30,
    OPERATION_CODE_JE              = 31,
    OPERATION_CODE_JNE             = 32,
    OPERATION_CODE_JA              = 33,
    OPERATION_CODE_JAE             = 34,
    OPERATION_CODE_JB              = 35,
    OPERATION_CODE_JBE             = 36
};

enum REGISTER_VARIABLES {
    RAX = 0,
    RBX = 1,
    RCX = 2,
    RDX = 3
};

const char TEXT_OPERATION[][5] = {
    "hlt",
    "push",
    "pop",
    "add",
    "sub",
    "mul",
    "div",
    "out",
    "in",
    "jmp",
    "push",
    "cmp",
    "je",
    "ja"
};

const char TEXT_REGISTERS[][4] = {
    "rax",
    "rbx",
    "rcx",
    "rdx"
};

const int NUMBER_ARGUMENTS_FOR_OPERATION[] = {
    0,
    2,
    1,
    0,
    0,
    0,
    0,
    0,
    0
};

const int LENGTH_OF_TEXT_OPERATION[] = {
    3,
    4,
    3,
    3,
    3,
    3,
    3,
    3,
    2
};

