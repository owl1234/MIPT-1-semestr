#include "processor.h"

#define DEFINE_COMMANDS(name, number, arg, code_processor, code_disassembler, code_assembler) \
        COMMAND_##name = number,

typedef enum my_commands_for_processor {
    #include "COMMANDS.H"
} commands_for_processor;

#undef DEFINE_COMMANDS
