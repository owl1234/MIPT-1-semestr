#include "processor.h"

#define DEFINE_COMMANDS(name, number, arg, code) \
        COMMAND_##name = number,

typedef enum commands_for_processor {
    #include "COMMANDS.H"
} commands;

#undef DEFINE_COMMANDS
