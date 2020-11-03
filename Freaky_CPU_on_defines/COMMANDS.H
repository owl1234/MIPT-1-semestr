#define GET_ADDRESS_LABEL_GO_TO                                         \
    int address_label = get_double_from_text(processor, &now_byte);     \
                                                                        \
    if(address_label == OPERATION_CODE_RECURSIVE_RET) {                 \
        COMMAND_RET;                                                    \
    } else {                                                            \
        stack_push(&(processor->call_stack), now_byte);                 \
        now_byte = address_label - 1;                                   \
    }

DEFINE_COMMANDS ( HLT, 0, 0,
{
    printf("End of work (hlt!)\n");
    //print_ram(ram);
    break;
})


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
        stack_push(&(processor->proc_stack), processor->ram[(int)now_value]);
    } else {
        POPADOS();
    }
    //printf("push: %lg\n\n", now_value);
})

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
})

DEFINE_COMMANDS ( ADD, 3, 0,
{
    double last = stack_back(&(processor->proc_stack));
    stack_pop(&(processor->proc_stack));

    double penultimate = stack_back(&(processor->proc_stack));
    stack_pop(&(processor->proc_stack));

    stack_push(&(processor->proc_stack), last + penultimate);
})

DEFINE_COMMANDS ( SUB, 4, 0,
{
    double last = stack_back(&(processor->proc_stack));
    stack_pop(&(processor->proc_stack));

    double penultimate = stack_back(&(processor->proc_stack));
    stack_pop(&(processor->proc_stack));

    stack_push(&(processor->proc_stack), penultimate - last);
})

DEFINE_COMMANDS ( MUL, 5, 0,
{
    double last = stack_back(&(processor->proc_stack));
    stack_pop(&(processor->proc_stack));

    double penultimate = stack_back(&(processor->proc_stack));
    stack_pop(&(processor->proc_stack));

    stack_push(&(processor->proc_stack), last * penultimate);
})

DEFINE_COMMANDS ( DIV, 6, 0,
{
    double last = stack_back(&(processor->proc_stack));
    stack_pop(&(processor->proc_stack));

    double penultimate = stack_back(&(processor->proc_stack));
    stack_pop(&(processor->proc_stack));

    stack_push(&(processor->proc_stack), penultimate / last);
})

DEFINE_COMMANDS ( OUT, 7, 0,
{
    if(processor->proc_stack.size_stack == 0) {
        printf("Stack is empty\n");
    } else {
        //print_stack(&proc_stack);
        printf("Answer: (size = %d) ", processor->proc_stack.size_stack);
        printf(identity, stack_back(&(processor->proc_stack)));
        printf("\n\n");
    }
})

DEFINE_COMMANDS ( IN, 8, 0,
{
    scanf(identity, &input_value);
    stack_push(&(processor->proc_stack), input_value);
})

DEFINE_COMMANDS ( SQRT, 9, 0,
{
    double last = stack_back(&(processor->proc_stack));
    stack_pop(&(processor->proc_stack));

    stack_push(&(processor->proc_stack), sqrt(last));
})

DEFINE_COMMANDS ( JMP, 10, 1,
{
    int address_label = get_double_from_text(processor, &now_byte);
    now_byte = address_label - 1;
})

DEFINE_COMMANDS ( CMP, 30, 4,
{
    first_comparison  = get_value_to_compare(processor, &now_byte);

    second_comparison = get_value_to_compare(processor, &now_byte);
})

DEFINE_COMMANDS ( JE, 31, 1,
{
    if(fabs(first_comparison - second_comparison) < EPSILON) {
        GET_ADDRESS_LABEL_GO_TO
    } else {
        get_double_from_text(processor, &now_byte);
    }
})

DEFINE_COMMANDS ( JNE, 32, 1,
{
    if(fabs(first_comparison - second_comparison) >= EPSILON) {
        GET_ADDRESS_LABEL_GO_TO
    } else {
        get_double_from_text(processor, &now_byte);
    }
})

DEFINE_COMMANDS ( JA, 33, 1,
{
    if(first_comparison > second_comparison) {
        GET_ADDRESS_LABEL_GO_TO
    } else {
        get_double_from_text(processor, &now_byte);
    }
})

DEFINE_COMMANDS ( JAE, 34, 1,
{
    if(first_comparison >= second_comparison) {
        GET_ADDRESS_LABEL_GO_TO
    } else {
        get_double_from_text(processor, &now_byte);
    }
})

DEFINE_COMMANDS ( JB, 35, 1,
{
    if(first_comparison < second_comparison) {
        GET_ADDRESS_LABEL_GO_TO
    } else {
        get_double_from_text(processor, &now_byte);
    }
})

DEFINE_COMMANDS ( JBE, 36, 1,
{
    if(first_comparison <= second_comparison) {
        GET_ADDRESS_LABEL_GO_TO
    } else {
        get_double_from_text(processor, &now_byte);
    }
})

DEFINE_COMMANDS ( CALL, 40, 1,
{
    int address_label = get_double_from_text(processor, &now_byte);

    stack_push(&(processor->call_stack), now_byte);
    now_byte = address_label - 1;
})

DEFINE_COMMANDS ( RET, 41, 0,
{
    int go_to_address = stack_back(&(processor->call_stack));
    stack_pop(&(processor->call_stack));
    now_byte = go_to_address;
})

DEFINE_COMMANDS ( MEOW, 99, 0,
{
    printf("meow\n");
})

