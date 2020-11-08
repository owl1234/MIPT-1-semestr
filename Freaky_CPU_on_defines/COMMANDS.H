#define PUT_ADDRESS_LABEL_GO_TO(code_of_operation)                                                                                      \
    put_char_into_disassembled_text(TEXT_OPERATION[code_of_operation], disassembled_text, &index_in_disassembled_text, SPACE);          \
    printf("\t# %d, label: %d\n", now_byte, labels[now_position_in_labels].byte_address); \
    now_command = (int)get_double_from_text(input_file, &now_byte);                                                                     \
    printf("\t# %d, label: %d\n", now_byte, labels[now_position_in_labels].byte_address);       \
                                                                                                                                        \
    for(int label=0; label<index_in_labels; ++label) {                                                                                  \
        if(labels[label].byte_address == now_command) {                                                                                 \
            printf("\t# %d, label: %d\n", now_byte, labels[now_position_in_labels].byte_address); \
            /*)printf("!!! label !!! %d %lg\n", labels[label].byte_address, now_command);                                */                \
            put_char_into_disassembled_text("AAA", disassembled_text, &index_in_disassembled_text, END_LINE);              \
        }                                                                                                                               \
    }   \
    printf("\t# %d, label: %d\n", now_byte, labels[now_position_in_labels].byte_address);                                                                                                                                 \


#define RECURSIVE_RETURN                                                \
    stack_pop(&(processor->call_stack));                                \
    int go_to_address = stack_back(&(processor->call_stack));           \
    IF_DEBUG(print_stack(&(processor->call_stack));)                    \
    now_byte = go_to_address;


#define PUT_COMMAND_WITHOUT_ARGUMENTS(code_of_command)                  \
    put_int_into_assembled_text(code_of_command, assembled_text, &index_in_assembled_text, &number_of_byte);   \
                                                                            \
    listing(input_file->listing_file, NOTHING, ONE_ARGUMENT, code_of_command);           \
    listing(input_file->listing_file, "|", SPACE);                                      \
    listing(input_file->listing_file, TEXT_OPERATION[code_of_command], END_LINE);        \


#define CONDITION_COMMANDS(number_of_condition)                                             \
    put_int_into_assembled_text(number_of_condition, assembled_text, &index_in_assembled_text, &number_of_byte);  \
                                                                                                        \
    temp_string = strtok(NULL, SEPARATORS);                                                                    \
    for(int label=0; label<index_in_labels; ++label) {                                                  \
        if(is_equal_labels(temp_string, labels[label].name)) {                                                 \
            put_int_into_assembled_text(labels[label].byte_address + 1, assembled_text, &index_in_assembled_text, &number_of_byte);   \
                                                                                                        \
            listing(input_file->listing_file, NOTHING, TWO_ARGUMENTS, number_of_condition, labels[label].byte_address);     \
            listing(input_file->listing_file, "|", SPACE);                                                                  \
            listing(input_file->listing_file, TEXT_OPERATION[number_of_condition], SPACE);                                       \
            listing(input_file->listing_file, temp_string, END_LINE);                                                                  \
            break;                                                                                              \
        }                                                                                                       \
    }

DEFINE_COMMANDS ( RECURSIVE_RETURN, -1, 0,
{

},
{

},
{
    put_int_into_assembled_text(number_of_condition, assembled_text, &index_in_assembled_text, &number_of_byte);

    temp_string = strtok(NULL, SEPARATORS);

    put_int_into_assembled_text(OPERATION_CODE_RECURSIVE_RET, assembled_text, &index_in_assembled_text, &number_of_byte);

    listing(input_file->listing_file, NOTHING, TWO_ARGUMENTS, number_of_condition, OPERATION_CODE_RECURSIVE_RET);
    listing(input_file->listing_file, "|", SPACE);
    listing(input_file->listing_file, TEXT_OPERATION[number_of_condition], SPACE);
    listing(input_file->listing_file, temp_string, END_LINE);
}
)


DEFINE_COMMANDS ( HLT, 0, 0,
{
    printf("End of work (hlt!)\n");
    break;
},
{
    put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_HLT], disassembled_text, &index_in_disassembled_text, END_LINE);
},
{
    put_int_into_assembled_text(OPERATION_CODE_HLT, assembled_text, &index_in_assembled_text, &number_of_byte);

    listing(input_file->listing_file, NOTHING, ONE_ARGUMENT, OPERATION_CODE_HLT);
    listing(input_file->listing_file, "| hlt", END_LINE);
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
},
{
    put_int_into_assembled_text(OPERATION_CODE_PUSH, assembled_text, &index_in_assembled_text, &number_of_byte);

    temp_string = strtok(NULL, SEPARATORS);

    int type_push_value = type_of_value(temp_string);
    put_int_into_assembled_text(type_push_value, assembled_text, &index_in_assembled_text, &number_of_byte);


    if(type_push_value == IS_REGISTER || type_push_value == (IS_RAM | IS_REGISTER)) {

        put_int_into_assembled_text(get_number_of_register(temp_string), assembled_text, &index_in_assembled_text, &number_of_byte);

        listing(input_file->listing_file, NOTHING, THREE_AGRUMENTS, OPERATION_CODE_PUSH, type_push_value, get_number_of_register(temp_string));
        listing(input_file->listing_file, "| push", SPACE);
        listing(input_file->listing_file, temp_string, END_LINE);

    } else if(type_push_value == IS_ELEM_T || type_push_value == (IS_RAM | IS_ELEM_T)) {
        if(type_push_value == (IS_RAM | IS_ELEM_T)) {
            temp_string = strchr(temp_string, '[') + 1;
            reverse_string(temp_string);
            temp_string = strchr(temp_string, ']') + 1;
            reverse_string(temp_string);
        }

        double now_value = strtod(temp_string, NULL);
        put_double_into_assembled_text(now_value, assembled_text, &index_in_assembled_text, &number_of_byte);

        listing(input_file->listing_file, NOTHING, TWO_ARGUMENTS, OPERATION_CODE_PUSH, type_push_value);
        listing(input_file->listing_file, NOTHING, (double)ONE_ARGUMENT, now_value);
        listing(input_file->listing_file, "| push ", NOTHING);
        listing(input_file->listing_file, END_LINE, (double)ONE_ARGUMENT, now_value);

    } else {
        printf("Bad type of push value! (%d)\n", type_push_value);
        return ASM_BAD_COMMAND;
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
},
{
    put_int_into_assembled_text(OPERATION_CODE_POP, assembled_text, &index_in_assembled_text, &number_of_byte);

    temp_string = strtok(NULL, SEPARATORS);

    if(type_of_value(temp_string) == IS_REGISTER) {

        put_int_into_assembled_text(IS_REGISTER, assembled_text, &index_in_assembled_text, &number_of_byte);
        put_int_into_assembled_text(get_number_of_register(temp_string), assembled_text, &index_in_assembled_text, &number_of_byte);

        listing(input_file->listing_file, NOTHING, THREE_AGRUMENTS, OPERATION_CODE_POP, IS_REGISTER, get_number_of_register(temp_string));
        listing(input_file->listing_file, "| pop ", NOTHING);
        listing(input_file->listing_file, temp_string, END_LINE);

    } else {

        put_int_into_assembled_text(NOT_ARGS, assembled_text, &index_in_assembled_text, &number_of_byte);

        listing(input_file->listing_file, NOTHING, TWO_ARGUMENTS, OPERATION_CODE_POP, NOT_ARGS);
        listing(input_file->listing_file, "| pop", END_LINE);

        listing_alignment(input_file->listing_file, number_of_byte, SPACE);
        listing(input_file->listing_file, "| ", NOTHING);

        //find_and_write_command(text, assembled_text, index_in_assembled_text, labels, index_in_labels, number_of_byte, listing_file);

    }
}
)

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
},
{
    PUT_COMMAND_WITHOUT_ARGUMENTS(OPERATION_CODE_ADD)
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
},
{
    PUT_COMMAND_WITHOUT_ARGUMENTS(OPERATION_CODE_SUB)
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
},
{
    PUT_COMMAND_WITHOUT_ARGUMENTS(OPERATION_CODE_MUL)
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
},
{
    PUT_COMMAND_WITHOUT_ARGUMENTS(OPERATION_CODE_DIV)
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
},
{
    PUT_COMMAND_WITHOUT_ARGUMENTS(OPERATION_CODE_OUT)
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
},
{
    PUT_COMMAND_WITHOUT_ARGUMENTS(OPERATION_CODE_IN)
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
},
{
    PUT_COMMAND_WITHOUT_ARGUMENTS(OPERATION_CODE_SQRT)
}
)

DEFINE_COMMANDS ( JMP, 10, 1,
{
    int address_label = get_double_from_text(processor, &now_byte);
    now_byte = address_label - 2;
},
{
    printf("\n-----------------------------------------------------\n\t# %d, label: %d\n", now_byte, labels[now_position_in_labels].byte_address);
    PUT_ADDRESS_LABEL_GO_TO(OPERATION_CODE_JMP)
    printf("\t# %d, label: %d\n\n-----------------------------------------------------\n", now_byte, labels[now_position_in_labels].byte_address);
},
{
    CONDITION_COMMANDS(OPERATION_CODE_JMP)
}
)

DEFINE_COMMANDS ( CMP, 30, 4,
{
    first_comparison  = get_value_to_compare(processor, &now_byte);

    second_comparison = get_value_to_compare(processor, &now_byte);
},
{
    put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_CMP], disassembled_text, &index_in_disassembled_text, SPACE);

},
{
    put_int_into_assembled_text(OPERATION_CODE_CMP, assembled_text, &index_in_assembled_text, &number_of_byte);

    /*char* first_string = (char*)calloc(MAX_SIZE, sizeof(char));
    char* second_string = (char*)calloc(MAX_SIZE, sizeof(char));

    int first_value  = put_cmp_value(text, assembled_text, index_in_assembled_text, number_of_byte, first_string,  &first_type_of_value);
    int second_value = put_cmp_value(text, assembled_text, index_in_assembled_text, number_of_byte, second_string, &second_type_of_value);

    listing(input_file->listing_file, NOTHING, FIVE_ARGUMENTS, OPERATION_CODE_CMP, first_type_of_value, first_value, second_type_of_value, second_value);
    listing(input_file->listing_file, "|", SPACE);
    listing(input_file->listing_file, "cmp", SPACE);
    listing(input_file->listing_file, first_string, SPACE);
    listing(input_file->listing_file, second_string, END_LINE);*/
}
)

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
},
{
    PUT_ADDRESS_LABEL_GO_TO(OPERATION_CODE_JE)
},
{
    CONDITION_COMMANDS(OPERATION_CODE_JE)
}
)

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
},
{
    PUT_ADDRESS_LABEL_GO_TO(OPERATION_CODE_JNE)
},
{
    CONDITION_COMMANDS(OPERATION_CODE_JNE)
}
)

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
},
{
    PUT_ADDRESS_LABEL_GO_TO(OPERATION_CODE_JA)
},
{
    CONDITION_COMMANDS(OPERATION_CODE_JA)
}
)

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
},
{
    PUT_ADDRESS_LABEL_GO_TO(OPERATION_CODE_JAE)
},
{
    CONDITION_COMMANDS(OPERATION_CODE_JAE)
}
)

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
},
{
    PUT_ADDRESS_LABEL_GO_TO(OPERATION_CODE_JB)
},
{
    CONDITION_COMMANDS(OPERATION_CODE_JB)
}
)

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
},
{
    PUT_ADDRESS_LABEL_GO_TO(OPERATION_CODE_JBE)
},
{
    CONDITION_COMMANDS(OPERATION_CODE_JBE)
}
)

DEFINE_COMMANDS ( CALL, 40, 1,
{
    int address_label = get_double_from_text(processor, &now_byte);

    stack_push(&(processor->call_stack), now_byte);
    now_byte = address_label - 2;
},
{
    PUT_ADDRESS_LABEL_GO_TO(OPERATION_CODE_CALL)
},
{
    CONDITION_COMMANDS(OPERATION_CODE_CALL)
}
)

DEFINE_COMMANDS ( RET, 41, 0,
{
    int go_to_address = stack_back(&(processor->call_stack));
    stack_pop(&(processor->call_stack));
    now_byte = go_to_address;
},
{
    put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_RET], disassembled_text, &index_in_disassembled_text, END_LINE);
},
{
    put_int_into_assembled_text(number_of_condition, assembled_text, &index_in_assembled_text, &number_of_byte);

    listing(input_file->listing_file, NOTHING, ONE_ARGUMENT, OPERATION_CODE_RET);
    listing(input_file->listing_file, "|", SPACE);
    listing(input_file->listing_file, TEXT_OPERATION[OPERATION_CODE_RET], END_LINE);
}
)

DEFINE_COMMANDS ( MEOW, 99, 0,
{
    printf("meow\n");
},
{
    put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_MEOW], disassembled_text, &index_in_disassembled_text, END_LINE);
},
{
    put_int_into_assembled_text(OPERATION_CODE_MEOW, assembled_text, &index_in_assembled_text, &number_of_byte);

    listing(input_file->listing_file, NOTHING, ONE_ARGUMENT, OPERATION_CODE_MEOW);
    listing(input_file->listing_file, "| meow", END_LINE);
}
)
