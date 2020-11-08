#define GET_ADDRESS_LABEL_GO_TO                                         \
    int address_label = get_double_from_text(processor, &now_byte);     \
                                                                        \
    if(address_label == OPERATION_CODE_RECURSIVE_RET) {                 \
        COMMAND_RET;                                                    \
    } else {                                                            \
        stack_push(&(processor->call_stack), now_byte);                 \
        now_byte = address_label - 2;                                   \
    }

#define RECURSIVE_RETURN                                                \
    stack_pop(&(processor->call_stack));                                \
    int go_to_address = stack_back(&(processor->call_stack));           \
    IF_DEBUG(print_stack(&(processor->call_stack));)                    \
    now_byte = go_to_address;                                           \


DEFINE_COMMANDS ( HLT, 0, 0,
{
    printf("End of work (hlt!)\n");
    break;
},
{
    put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_HLT], disassembled_text, &index_in_disassembled_text, END_LINE);
}
)


DEFINE_COMMANDS ( PUSH, 1, 2,
{
    Elem_t flag_of_registers = get_double_from_text(processor, &now_byte);

    Elem_t now_value = get_double_from_text(processor, &now_byte);

    if(flag_of_registers == IS_REGISTER) {
        stack_push(&(processor->proc_stack), processor->registers_variables[(int)now_value]);
    } else if(flag_of_registers == IS_ELEM_T) {
        stack_push(&(processor->proc_stack), now_value);
    } else if(flag_of_registers == (IS_RAM | IS_REGISTER)) {
        stack_push(&(processor->proc_stack), processor->ram[(int)(processor->registers_variables[(int)now_value])]);
    } else if(flag_of_registers == (IS_RAM | IS_ELEM_T)) {
        printf("%d\n", (int)now_value);
        stack_push(&(processor->proc_stack), processor->ram[(int)now_value]);
    } else {
        printf("Unknown value to add to the stack"); // POPADOS();
        return PROC_UNKNOWN_COMMAND;
    }
},
{
    put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_PUSH], disassembled_text, &index_in_disassembled_text, SPACE);
    Elem_t flag_of_the_command = get_double_from_text(input_file, &now_byte);

    Elem_t now_value = get_double_from_text(input_file, &now_byte);
    if(flag_of_the_command == IS_ELEM_T) {
        put_int_into_disassembled_text(now_value, disassembled_text, &index_in_disassembled_text, END_LINE);
    } else if(flag_of_the_command == IS_REGISTER) {
        put_char_into_disassembled_text(TEXT_REGISTERS[(int)now_value], disassembled_text, &index_in_disassembled_text, END_LINE);
    }
}
)

DEFINE_COMMANDS ( POP, 2, 2,
{
    double back_element = 0.0;

    int flag_of_registers = get_double_from_text(processor, &now_byte);

    if(flag_of_registers == NOT_ARGS) {
        back_element = stack_back(&(processor->proc_stack));
        stack_pop(&(processor->proc_stack));
    } else {
        int number_of_register = get_double_from_text(processor, &now_byte);
        processor->registers_variables[number_of_register] = stack_back(&(processor->proc_stack));
        stack_pop(&(processor->proc_stack));
    }
},
{
    Elem_t flag_of_the_command = get_double_from_text(input_file, &now_byte);

    if(flag_of_the_command == IS_ELEM_T) {
        put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_POP], disassembled_text, &index_in_disassembled_text, END_LINE);
    } else if(flag_of_the_command == IS_REGISTER) {
        put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_POP], disassembled_text, &index_in_disassembled_text, SPACE);
        Elem_t now_value = get_double_from_text(input_file, &now_byte);
        put_char_into_disassembled_text(TEXT_REGISTERS[(int)now_value], disassembled_text, &index_in_disassembled_text, END_LINE);
    }
})

DEFINE_COMMANDS ( ADD, 3, 0,
{
    double last = stack_back(&(processor->proc_stack));
    stack_pop(&(processor->proc_stack));

    double penultimate = stack_back(&(processor->proc_stack));
    stack_pop(&(processor->proc_stack));

    stack_push(&(processor->proc_stack), last + penultimate);
},
{
    put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_ADD], disassembled_text, &index_in_disassembled_text, END_LINE);
}
)

DEFINE_COMMANDS ( SUB, 4, 0,
{
    double last = stack_back(&(processor->proc_stack));
    stack_pop(&(processor->proc_stack));

    double penultimate = stack_back(&(processor->proc_stack));
    stack_pop(&(processor->proc_stack));

    stack_push(&(processor->proc_stack), penultimate - last);
},
{
    put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_SUB], disassembled_text, &index_in_disassembled_text, END_LINE);
}
)

DEFINE_COMMANDS ( MUL, 5, 0,
{
    double last = stack_back(&(processor->proc_stack));
    stack_pop(&(processor->proc_stack));

    double penultimate = stack_back(&(processor->proc_stack));
    stack_pop(&(processor->proc_stack));

    stack_push(&(processor->proc_stack), last * penultimate);
},
{
    put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_MUL], disassembled_text, &index_in_disassembled_text, END_LINE);
}
)

DEFINE_COMMANDS ( DIV, 6, 0,
{
    double last = stack_back(&(processor->proc_stack));
    stack_pop(&(processor->proc_stack));

    double penultimate = stack_back(&(processor->proc_stack));
    stack_pop(&(processor->proc_stack));

    stack_push(&(processor->proc_stack), penultimate / last);
},
{
    put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_DIV], disassembled_text, &index_in_disassembled_text, END_LINE);
}
)

DEFINE_COMMANDS ( OUT, 7, 0,
{
    if(processor->proc_stack.size_stack == 0) {
        printf("Stack is empty\n");
    } else {
        //print_stack(&proc_stack);
        printf("Answer: (size_stack = %lu) ", processor->proc_stack.size_stack);
        printf(identity, stack_back(&(processor->proc_stack)));
        printf("\n\n");
    }
},
{
    put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_OUT], disassembled_text, &index_in_disassembled_text, END_LINE);
}
)

DEFINE_COMMANDS ( IN, 8, 0,
{
    printf("Enter your number: ");
    scanf(identity, &input_value);
    stack_push(&(processor->proc_stack), input_value);
},
{
    put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_IN], disassembled_text, &index_in_disassembled_text, END_LINE);
}
)

DEFINE_COMMANDS ( SQRT, 9, 0,
{
    double last = stack_back(&(processor->proc_stack));
    stack_pop(&(processor->proc_stack));

    stack_push(&(processor->proc_stack), sqrt(last));
},
{
    put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_SQRT], disassembled_text, &index_in_disassembled_text, END_LINE);
}
)

DEFINE_COMMANDS ( JMP, 10, 1,
{
    int address_label = get_double_from_text(processor, &now_byte);
    now_byte = address_label - 2;
},
{
    put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_JMP], disassembled_text, &index_in_disassembled_text, SPACE);
    now_command = input_file->text_for_disassembling[++now_byte];
    put_char_into_disassembled_text("label", disassembled_text, &index_in_disassembled_text, END_LINE);
}
)

/*DEFINE_COMMANDS ( CMP, 30, 4,
{
    first_comparison  = get_value_to_compare(processor, &now_byte);

    second_comparison = get_value_to_compare(processor, &now_byte);
})

DEFINE_COMMANDS ( JE, 31, 1,
{
    if(fabs(first_comparison - second_comparison) < EPSILON) {
        GET_ADDRESS_LABEL_GO_TO
    } else {
        int address_label = get_double_from_text(processor, &now_byte);
        if(fabs(address_label - OPERATION_CODE_RECURSIVE_RET) < EPSILON) {
            RECURSIVE_RETURN;
        }
    }
})

DEFINE_COMMANDS ( JNE, 32, 1,
{
    if(fabs(first_comparison - second_comparison) >= EPSILON) {
        GET_ADDRESS_LABEL_GO_TO
    } else {
        int address_label = get_double_from_text(processor, &now_byte);
        if(fabs(address_label - OPERATION_CODE_RECURSIVE_RET) < EPSILON) {
            RECURSIVE_RETURN;
        }
    }
})

DEFINE_COMMANDS ( JA, 33, 1,
{
    if(first_comparison > second_comparison) {
        GET_ADDRESS_LABEL_GO_TO
    } else {
        int address_label = get_double_from_text(processor, &now_byte);
        if(fabs(address_label - OPERATION_CODE_RECURSIVE_RET) < EPSILON) {
            RECURSIVE_RETURN;
        }
    }
})

DEFINE_COMMANDS ( JAE, 34, 1,
{
    if(first_comparison >= second_comparison) {
        GET_ADDRESS_LABEL_GO_TO
    } else {
        int address_label = get_double_from_text(processor, &now_byte);
        if(fabs(address_label - OPERATION_CODE_RECURSIVE_RET) < EPSILON) {
            RECURSIVE_RETURN;
        }
    }
})

DEFINE_COMMANDS ( JB, 35, 1,
{
    if(first_comparison < second_comparison) {
        GET_ADDRESS_LABEL_GO_TO
    } else {
        int address_label = get_double_from_text(processor, &now_byte);
        if(fabs(address_label - OPERATION_CODE_RECURSIVE_RET) < EPSILON) {
            RECURSIVE_RETURN;
        }
    }
})

DEFINE_COMMANDS ( JBE, 36, 1,
{
    if(first_comparison <= second_comparison) {
        GET_ADDRESS_LABEL_GO_TO
    } else {
        int address_label = get_double_from_text(processor, &now_byte);
        if(fabs(address_label - OPERATION_CODE_RECURSIVE_RET) < EPSILON) {
            RECURSIVE_RETURN;
        }
    }
})

DEFINE_COMMANDS ( CALL, 40, 1,
{
    int address_label = get_double_from_text(processor, &now_byte);

    stack_push(&(processor->call_stack), now_byte);
    now_byte = address_label - 2;
})*/

DEFINE_COMMANDS ( RET, 41, 0,
{
    int go_to_address = stack_back(&(processor->call_stack));
    stack_pop(&(processor->call_stack));
    now_byte = go_to_address;
},
{
    put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_RET], disassembled_text, &index_in_disassembled_text, END_LINE);
}
)

DEFINE_COMMANDS ( MEOW, 99, 0,
{
    printf("meow\n");
},
{
    put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_MEOW], disassembled_text, &index_in_disassembled_text, END_LINE);
}
)
