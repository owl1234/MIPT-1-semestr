#include "processor.h"

#define DEFINE_COMMANDS(name, number, arg, code) \
        COMMAND_##name = number,

enum commands_for_processor {
    #include "COMMANDS.H"
};

#undef DEFINE_COMMANDS
