#define GET_ADDRESS_LABEL_GO_TO                                         \
    int address_label = get_byte_from_text(processor, &now_byte);       \
                                                                        \
    if(address_label == OPERATION_CODE_RECURSIVE_RET) {                 \
        COMMAND_ret;                                                    \
    } else {                                                            \
        stack_push(&(processor->call_stack), now_byte);                 \
        now_byte = address_label - 2;                                   \
    }

#define PUT_ADDRESS_LABEL_GO_TO(code_of_operation)                                                                                      \
    print_labels(disasm_struct.labels, disasm_struct.index_in_labels);                                                                  \
    put_char_into_disassembled_text(TEXT_OPERATION[(int)now_command], &disasm_struct, SPACE);                                           \
    printf("put_address: command %lg\n", now_command);                                                                                  \
    now_command = (int)get_byte_from_text(input_file, &disasm_struct);                                                                  \
    printf("put_address: command %lg\n", now_command);                                                                                  \
                                                                                                                                        \
    for(int label=0; label<disasm_struct.index_in_labels; ++label) {                                                                    \
        printf("\tput_address: %d %lg\n", disasm_struct.labels[label].byte_address, now_command);                                       \
        if(abs(disasm_struct.labels[label].byte_address - now_command) < EPSILON) {                                                     \
            put_char_into_disassembled_text("meow", &disasm_struct, END_LINE);                                                          \
        }                                                                                                                               \
    }                                                                                                                                   \
    printf("\t# %d, label: %d\n", disasm_struct.now_position_in_labels, disasm_struct.labels[disasm_struct.now_position_in_labels].byte_address);                                                                                                                                 \


#define RECURSIVE_RETURN                                                \
    STACK_POP(processor->call_stack);                                   \
    int go_to_address = STACK_BACK(processor->call_stack);              \
    IF_DEBUG(print_stack(&(processor->call_stack));)                    \
    now_byte = go_to_address;


#define PUT_COMMAND_WITHOUT_ARGUMENTS(code_of_command)                                                                                  \
    put_opcode_into_assembled_text(code_of_command, assembled_text, &index_in_assembled_text, &number_of_byte);                         \
                                                                                                                                        \
    listing(input_file->listing_file, NOTHING, ONE_ARGUMENT, code_of_command);                                                          \
    listing(input_file->listing_file, "|", SPACE);                                                                                      \
    listing(input_file->listing_file, TEXT_OPERATION[code_of_command], END_LINE);                                                       \


#define CONDITION_COMMANDS(number_of_condition)                                                                                         \
    is_find_label = false;                                                                                                              \
    put_opcode_into_assembled_text(number_of_condition, assembled_text, &index_in_assembled_text, &number_of_byte);                     \
                                                                                                                                        \
    temp_string = strtok(NULL, SEPARATORS);                                                                                             \
    for(int label=0; label<index_in_labels; ++label) {                                                                                  \
        if(is_equal_labels(temp_string, labels[label].name)) {                                                                          \
            put_int_into_assembled_text(labels[label].byte_address + 1, assembled_text, &index_in_assembled_text, &number_of_byte);     \
                                                                                                                                        \
            listing(input_file->listing_file, NOTHING, TWO_ARGUMENTS, number_of_condition, labels[label].byte_address);                 \
            listing(input_file->listing_file, "|", SPACE);                                                                              \
            listing(input_file->listing_file, TEXT_OPERATION[number_of_condition], SPACE);                                              \
            listing(input_file->listing_file, temp_string, END_LINE);                                                                   \
                                                                                                                                        \
            is_find_label = true;                                                                                                       \
            break;                                                                                                                      \
        }                                                                                                                               \
    }                                                                                                                                   \
                                                                                                                                        \
    if(!is_find_label && !strcmp(temp_string, "recret")) {                                                                              \
        put_opcode_into_assembled_text(OPERATION_CODE_RECURSIVE_RET, assembled_text, &index_in_assembled_text, &number_of_byte);        \
                                                                                                                                        \
        listing(input_file->listing_file, NOTHING, TWO_ARGUMENTS, number_of_condition, OPERATION_CODE_RECURSIVE_RET);                   \
        listing(input_file->listing_file, "|", SPACE);                                                                                  \
        listing(input_file->listing_file, TEXT_OPERATION[number_of_condition], SPACE);                                                  \
        listing(input_file->listing_file, temp_string, END_LINE);                                                                       \
    }

#define FROM_RAM_TO_ELEM_T                                                  \
    for(size_t i=1; i<MAX_SIZE-1; ++i) {                                    \
        handle_ram_variable[i-1] = temp_string[i];                          \
    }

#define TAKE_ELEM_T_FROM_RAM_REG_ELEM_T                                     \
    size_t index_for_take_elem_t = 0;                                       \
    for(; index_for_take_elem_t < MAX_SIZE - 1 && temp_string[index_for_take_elem_t] != '\0'; ++index_for_take_elem_t)      \
        if(temp_string[index_for_take_elem_t] >= '0' && temp_string[index_for_take_elem_t] <= '9')                          \
            break;                                                                                                          \
                                                                                                                            \
    for(size_t i = 0; i < MAX_SIZE; ++i)                                                                                    \
        handle_ram_variable[i] = temp_string[index_for_take_elem_t + i];                                                    \

#define TAKE_ELEM_T_FROM_RAM_ELEM_T                                                                                         \
    for(size_t i = 1; i < MAX_SIZE - 1; ++i)                                                                                    \
        handle_ram_variable[i - 1] = temp_string[i];                                                                            \


#define ASSEMBLER(code) code
#define PROCESSOR(code) code
#define DISASSEMBLER(code) code;

#define STACK_BACK(stack) stack_back(&stack)
#define STACK_POP(stack)  stack_pop(&stack)

DEFINE_COMMANDS ( recursive_return, -1, 0,
PROCESSOR (

),
DISASSEMBLER (

),
ASSEMBLER (
    put_opcode_into_assembled_text(number_of_condition, assembled_text, &index_in_assembled_text, &number_of_byte);

    temp_string = strtok(NULL, SEPARATORS);

    put_opcode_into_assembled_text(OPERATION_CODE_RECURSIVE_RET, assembled_text, &index_in_assembled_text, &number_of_byte);

    listing(input_file->listing_file, NOTHING, TWO_ARGUMENTS, number_of_condition, OPERATION_CODE_RECURSIVE_RET);
    listing(input_file->listing_file, "|", SPACE);
    listing(input_file->listing_file, TEXT_OPERATION[number_of_condition], SPACE);
    listing(input_file->listing_file, temp_string, END_LINE);
)
)


DEFINE_COMMANDS ( hlt, OPERATION_CODE_HLT, 0,
PROCESSOR (
    printf("End of work (hlt!)\n");
    break;
),
DISASSEMBLER (
    put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_HLT], &disasm_struct, END_LINE);
),
ASSEMBLER (
    put_opcode_into_assembled_text(OPERATION_CODE_HLT, assembled_text, &index_in_assembled_text, &number_of_byte);

    listing(input_file->listing_file, NOTHING, ONE_ARGUMENT, OPERATION_CODE_HLT);
    listing(input_file->listing_file, "| hlt", END_LINE);
)
)


DEFINE_COMMANDS ( push, OPERATION_CODE_PUSH, 2,
PROCESSOR (
    Flag_t flag_of_registers = get_byte_from_text(processor, &now_byte);

    Elem_t now_value = 0; 

    if(flag_of_registers == IS_REGISTER) {
        now_value = get_byte_from_text(processor, &now_byte);
        stack_push(&(processor->proc_stack), processor->registers_variables[now_value]);
    } else if(flag_of_registers == IS_ELEM_T) {
        ++now_byte;
        now_value = get_long_long_number_from_binary(processor, &now_byte);
        IF_DEBUG_PROCESSOR(printf("\t\t push %d\n", now_value);)
        stack_push(&(processor->proc_stack), now_value);
        --now_byte;
    } else if(flag_of_registers == (IS_RAM | IS_REGISTER)) {
        now_value = get_byte_from_text(processor, &now_byte);
        ram_index = (int)(processor->registers_variables[(int)now_value]);
        if(ram_index < MAX_SIZE_RAM) {
            stack_push(&(processor->proc_stack), processor->ram[ram_index]);
        } else {
            printf("Bad index (%d) in RAM\n", ram_index);
        }
    } else if(flag_of_registers == (IS_RAM | IS_ELEM_T)) {
        ++now_byte;
        now_value = get_long_long_number_from_binary(processor, &now_byte);
        --now_byte;
        ram_index = (int)now_value;
        if(ram_index < MAX_SIZE_RAM) {
            stack_push(&(processor->proc_stack), processor->ram[ram_index]);
        } else {
            printf("Bad index in RAM\n");
        }
    } else if(flag_of_registers == (IS_RAM | IS_REGISTER | IS_ELEM_T)) {
        now_value = get_byte_from_text(processor, &now_byte);
        ++now_byte;
        int now_elem_t = get_long_long_number_from_binary(processor, &now_byte);
        --now_byte;
        IF_DEBUG_PROCESSOR(printf("\t\tnow_value: %d - it's reg, %d - delta\n", now_value, now_elem_t);)
        ram_index = (int)(processor->registers_variables[(int)now_value]) + now_elem_t;

        if(ram_index < MAX_SIZE_RAM) {
            stack_push(&(processor->proc_stack), processor->ram[ram_index]);
        } else {
            printf("Bad index (%d) in RAM\n", ram_index);
        }


    } else {
        printf("Unknown value to add to the stack\n");
        return PROC_UNKNOWN_COMMAND;
    }

    IF_DEBUG_PROCESSOR(printf("\tmade push %d, flag_of_registers %d\n", now_value, flag_of_registers);)
),
DISASSEMBLER (
    put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_PUSH], &disasm_struct, SPACE);
    Elem_t flag_of_the_command = get_byte_from_text(input_file, &disasm_struct);

    Elem_t now_value = get_byte_from_text(input_file, &disasm_struct);
    if(flag_of_the_command == IS_ELEM_T) {
        put_int_into_disassembled_text(now_value, &disasm_struct, END_LINE);
    } else if(flag_of_the_command == IS_REGISTER) {
        put_char_into_disassembled_text(TEXT_REGISTERS[(int)now_value], &disasm_struct, END_LINE);
    }
),
ASSEMBLER (
    put_opcode_into_assembled_text(OPERATION_CODE_PUSH, assembled_text, &index_in_assembled_text, &number_of_byte);

    temp_string = strtok(NULL, SEPARATORS);

    int type_push_value = type_of_value(temp_string);

    put_char_into_assembled_text(type_push_value, assembled_text, &index_in_assembled_text, &number_of_byte);

    if(type_push_value == IS_REGISTER || type_push_value == (IS_RAM | IS_REGISTER)) {

        put_char_into_assembled_text(get_number_of_register(temp_string), assembled_text, &index_in_assembled_text, &number_of_byte);

        listing(input_file->listing_file, NOTHING, THREE_AGRUMENTS, OPERATION_CODE_PUSH, type_push_value, get_number_of_register(temp_string));
        listing(input_file->listing_file, "| push", SPACE);
        listing(input_file->listing_file, temp_string, END_LINE);

    } else if(type_push_value == IS_ELEM_T || type_push_value == (IS_RAM | IS_ELEM_T)) {
        FROM_RAM_TO_ELEM_T

        if(type_push_value == (IS_RAM | IS_ELEM_T)) {
            now_value = strtod(handle_ram_variable, NULL);
        } else {
            now_value = strtod(temp_string, NULL);
        }

        put_int_into_assembled_text(now_value, assembled_text, &index_in_assembled_text, &number_of_byte);

        listing(input_file->listing_file, NOTHING, TWO_ARGUMENTS, OPERATION_CODE_PUSH, type_push_value);
        listing(input_file->listing_file, NOTHING, (double)ONE_ARGUMENT, now_value);
        listing(input_file->listing_file, "| push ", NOTHING);
        listing(input_file->listing_file, END_LINE, (double)ONE_ARGUMENT, now_value);

    } else if(type_push_value == (IS_RAM | IS_REGISTER | IS_ELEM_T)) {
        put_char_into_assembled_text(get_number_of_register_from_ram_reg_elem_t(temp_string), assembled_text, &index_in_assembled_text, &number_of_byte);

        TAKE_ELEM_T_FROM_RAM_REG_ELEM_T

        now_value = strtod(handle_ram_variable, NULL);

        put_int_into_assembled_text(now_value, assembled_text, &index_in_assembled_text, &number_of_byte);

        IF_DEBUG_ASSEMBLER(printf("now value push: %d, %s\n", now_value, temp_string);)
    } else {
        printf("Bad type of push value! (%d)\n", type_push_value);
        return ASM_BAD_COMMAND;
    }
)
)

DEFINE_COMMANDS ( pop, OPERATION_CODE_POP, 2,
PROCESSOR (
    int back_element = 0;

    int flag_of_registers = get_byte_from_text(processor, &now_byte);

    if(flag_of_registers == NOT_ARGS) {
        back_element = STACK_BACK(processor->proc_stack);
        STACK_POP(processor->proc_stack);
    } else {
        int number_of_register = get_byte_from_text(processor, &now_byte);
        IF_DEBUG_PROCESSOR(printf("\t\t!!! number of reg: %d\n", number_of_register);)
        IF_DEBUG_PROCESSOR(printf("\t\tstack back: %d, register #%d = %d\n", stack_back(&processor->proc_stack), number_of_register, processor->registers_variables[number_of_register]);)
        
        if(flag_of_registers == (IS_RAM | IS_REGISTER)) {
            if(processor->registers_variables[number_of_register] < MAX_SIZE_RAM)
                processor->ram[processor->registers_variables[number_of_register]] = STACK_BACK(processor->proc_stack);
            else
                processor->status = PROC_BAD_SIZE_PROC_STACK;
        } else if(flag_of_registers == IS_REGISTER) {
            processor->registers_variables[number_of_register] = STACK_BACK(processor->proc_stack);
        } else if(flag_of_registers == (IS_RAM | IS_REGISTER | IS_ELEM_T)) {
            ++now_byte;
            now_value = get_long_long_number_from_binary(processor, &now_byte);
            processor->ram[processor->registers_variables[number_of_register] + now_value] = STACK_BACK(processor->proc_stack);
            --now_byte;
        } else if(flag_of_registers == (IS_RAM | IS_ELEM_T)) {
            now_value = get_long_long_number_from_binary(processor, &now_byte);
            --now_byte;
            if(now_value < MAX_SIZE_RAM)
                processor->ram[now_value] = STACK_BACK(processor->proc_stack);
            else
                processor->status = PROC_BAD_SIZE_PROC_STACK;
        }
        else
            abort();

        IF_DEBUG_PROCESSOR(printf("\t\tstack back: %d, register #%d = %d (%d)\n", stack_back(&processor->proc_stack), number_of_register, processor->registers_variables[number_of_register], now_value);)
        IF_DEBUG_PROCESSOR(printf("\t\tWell............. answer: [%d] = %d\n", processor->registers_variables[number_of_register] + now_value, processor->ram[processor->registers_variables[number_of_register] + now_value]);)
        STACK_POP(processor->proc_stack);
    }
),
DISASSEMBLER (
    int flag_of_the_command = (int)get_byte_from_text(input_file, &disasm_struct);

    if(flag_of_the_command == NOT_ARGS) {
        put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_POP], &disasm_struct, END_LINE);
    } if(flag_of_the_command == IS_ELEM_T) {
        put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_POP], &disasm_struct, END_LINE);
    } else if(flag_of_the_command == IS_REGISTER) {
        put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_POP], &disasm_struct, SPACE);
        Elem_t now_value = get_byte_from_text(input_file, &disasm_struct);
        put_char_into_disassembled_text(TEXT_REGISTERS[(int)now_value], &disasm_struct, END_LINE);
    }
),
ASSEMBLER (
    put_opcode_into_assembled_text(OPERATION_CODE_POP, assembled_text, &index_in_assembled_text, &number_of_byte);

    temp_string = strtok(NULL, SEPARATORS);
    int type_pop_value = type_of_value(temp_string);
    IF_DEBUG_ASSEMBLER(printf("\t\ttype value: %d\n", type_pop_value);)

    if(type_pop_value == IS_REGISTER || type_pop_value == (IS_REGISTER | IS_RAM)) {

        put_char_into_assembled_text(type_pop_value, assembled_text, &index_in_assembled_text, &number_of_byte);
        put_char_into_assembled_text(get_number_of_register(temp_string), assembled_text, &index_in_assembled_text, &number_of_byte);

        listing(input_file->listing_file, NOTHING, THREE_AGRUMENTS, OPERATION_CODE_POP, type_pop_value, get_number_of_register(temp_string));
        listing(input_file->listing_file, "| pop ", NOTHING);
        listing(input_file->listing_file, temp_string, END_LINE);

    } else if(type_pop_value == (IS_REGISTER | IS_RAM | IS_ELEM_T)) {
        IF_DEBUG_ASSEMBLER(printf("\tPOP IS_REGISTER | IS_RAM | IS_ELEM_T \n");)
        put_char_into_assembled_text(type_pop_value, assembled_text, &index_in_assembled_text, &number_of_byte);
        IF_DEBUG_ASSEMBLER(printf("get num %d\n", get_number_of_register(temp_string));)
        put_char_into_assembled_text(get_number_of_register_from_ram_reg_elem_t(temp_string), assembled_text, &index_in_assembled_text, &number_of_byte);

        TAKE_ELEM_T_FROM_RAM_REG_ELEM_T

        now_value = strtod(handle_ram_variable, NULL);

        put_int_into_assembled_text(now_value, assembled_text, &index_in_assembled_text, &number_of_byte);

        IF_DEBUG_ASSEMBLER(printf("now value pop: %d, %s\n", now_value, temp_string);)

        listing(input_file->listing_file, NOTHING, TWO_ARGUMENTS, OPERATION_CODE_POP, type_pop_value);
        listing(input_file->listing_file, END_LINE, (double)ONE_ARGUMENT, now_value);
        //listing(input_file->listing_file, "| pop [", NOTHING);
        //listing(input_file->listing_file, NOTHING, (double)ONE_ARGUMENT, now_value); 
    }  else if(type_pop_value == (IS_RAM | IS_ELEM_T)) {
        IF_DEBUG_ASSEMBLER(printf("\tPOP IS_RAM | IS_ELEM_T \n");)
        put_char_into_assembled_text(type_pop_value, assembled_text, &index_in_assembled_text, &number_of_byte);

        TAKE_ELEM_T_FROM_RAM_ELEM_T

        now_value = strtod(handle_ram_variable, NULL);

        put_int_into_assembled_text(now_value, assembled_text, &index_in_assembled_text, &number_of_byte);

        IF_DEBUG_ASSEMBLER(printf("now value pop: %d, %s\n", now_value, temp_string);)

        listing(input_file->listing_file, NOTHING, TWO_ARGUMENTS, OPERATION_CODE_POP, type_pop_value);
        listing(input_file->listing_file, END_LINE, (double)ONE_ARGUMENT, now_value);
        //listing(input_file->listing_file, "| pop [", NOTHING);
        //listing(input_file->listing_file, NOTHING, (double)ONE_ARGUMENT, now_value); 
    }

    else {

        put_char_into_assembled_text(NOT_ARGS, assembled_text, &index_in_assembled_text, &number_of_byte);

        listing(input_file->listing_file, NOTHING, TWO_ARGUMENTS, OPERATION_CODE_POP, NOT_ARGS);
        listing(input_file->listing_file, "| pop", END_LINE);

        listing_alignment(input_file->listing_file, number_of_byte, SPACE);
        listing(input_file->listing_file, "| ", NOTHING);

        flag_of_pop_without_args = true;
    }
)
)

DEFINE_COMMANDS ( add, OPERATION_CODE_ADD, 0,
PROCESSOR (
    Elem_t last = STACK_BACK(processor->proc_stack);
    STACK_POP(processor->proc_stack);

    Elem_t penultimate = STACK_BACK(processor->proc_stack);
    STACK_POP(processor->proc_stack);

    printf("\tadd %d + %d = %d\n", last, penultimate, last + penultimate);
    stack_push(&(processor->proc_stack), last + penultimate);
),
DISASSEMBLER (
    put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_ADD], &disasm_struct, END_LINE);
),
ASSEMBLER (
    PUT_COMMAND_WITHOUT_ARGUMENTS(OPERATION_CODE_ADD)
)
)

DEFINE_COMMANDS ( sub, OPERATION_CODE_SUB, 0,
PROCESSOR (
    Elem_t last = STACK_BACK(processor->proc_stack);
    STACK_POP(processor->proc_stack);

    Elem_t penultimate = STACK_BACK(processor->proc_stack);
    STACK_POP(processor->proc_stack);

    printf("\tsub %d - %d = %d\n", penultimate, last, penultimate - last);
    stack_push(&(processor->proc_stack), penultimate - last);
),
DISASSEMBLER (
    put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_SUB], &disasm_struct, END_LINE);
),
ASSEMBLER (
    PUT_COMMAND_WITHOUT_ARGUMENTS(OPERATION_CODE_SUB)
)
)

DEFINE_COMMANDS ( mul, OPERATION_CODE_MUL, 0,
PROCESSOR (
    last = STACK_BACK(processor->proc_stack);
    STACK_POP(processor->proc_stack);

    penultimate = STACK_BACK(processor->proc_stack);
    STACK_POP(processor->proc_stack);

    printf("\tmul %d * %d = %d\n", penultimate, last, penultimate * last);
    stack_push(&(processor->proc_stack), last * penultimate);
),
DISASSEMBLER (
    put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_MUL], &disasm_struct, END_LINE);
),
ASSEMBLER (
    PUT_COMMAND_WITHOUT_ARGUMENTS(OPERATION_CODE_MUL)
)
)

DEFINE_COMMANDS ( div, OPERATION_CODE_DIV, 0,
PROCESSOR (
    Elem_t last = STACK_BACK(processor->proc_stack);
    STACK_POP(processor->proc_stack);

    Elem_t penultimate = STACK_BACK(processor->proc_stack);
    STACK_POP(processor->proc_stack);

    printf("\tdiv %d / %d = %d\n", penultimate, last, penultimate / last);
    stack_push(&(processor->proc_stack), penultimate / last);
),
DISASSEMBLER (
    put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_DIV], &disasm_struct, END_LINE);
),
ASSEMBLER (
    PUT_COMMAND_WITHOUT_ARGUMENTS(OPERATION_CODE_DIV)
)
)

DEFINE_COMMANDS ( out, OPERATION_CODE_OUT, 0,
PROCESSOR (
    if(processor->proc_stack.size_stack == 0) {
        printf("Stack is empty\n");
    } else {
        //print_stack(&proc_stack);
        printf("Answer: (size_stack = %lu) ", processor->proc_stack.size_stack);
        printf(identity, stack_back(&processor->proc_stack)); //STACK_BACK(processor->proc_stack));
        printf("\n\n");
    }
),
DISASSEMBLER (
    put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_OUT], &disasm_struct, END_LINE);
),
ASSEMBLER (
    PUT_COMMAND_WITHOUT_ARGUMENTS(OPERATION_CODE_OUT)
)
)

DEFINE_COMMANDS ( in, OPERATION_CODE_IN, 0,
PROCESSOR (
    printf("Enter your number: ");
    scanf(identity, &input_value);
    stack_push(&(processor->proc_stack), input_value);
),
DISASSEMBLER (
    put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_IN], &disasm_struct, END_LINE);
),
ASSEMBLER (
    PUT_COMMAND_WITHOUT_ARGUMENTS(OPERATION_CODE_IN)
)
)

DEFINE_COMMANDS ( sqrt, OPERATION_CODE_SQRT, 0,
PROCESSOR (
    double last = STACK_BACK(processor->proc_stack);
    STACK_POP(processor->proc_stack);

    stack_push(&(processor->proc_stack), sqrt(last));
),
DISASSEMBLER (
    put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_SQRT], &disasm_struct, END_LINE);
),
ASSEMBLER (
    PUT_COMMAND_WITHOUT_ARGUMENTS(OPERATION_CODE_SQRT)
)
)

DEFINE_COMMANDS ( jmp, OPERATION_CODE_JMP, 1,
PROCESSOR (
    int address_label = get_byte_from_text(processor, &now_byte);
    now_byte = address_label - 2;
),
DISASSEMBLER (
    PUT_ADDRESS_LABEL_GO_TO(OPERATION_CODE_JMP)
),
ASSEMBLER (
    CONDITION_COMMANDS(OPERATION_CODE_JMP)
)
)

DEFINE_COMMANDS ( cmp, OPERATION_CODE_CMP, 4,
PROCESSOR (
    first_comparison  = get_value_to_compare(processor, &now_byte);

    second_comparison = get_value_to_compare(processor, &now_byte);
),
DISASSEMBLER (
    put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_CMP], &disasm_struct, SPACE);
),
ASSEMBLER (
    put_opcode_into_assembled_text(OPERATION_CODE_CMP, assembled_text, &index_in_assembled_text, &number_of_byte);

    char* first_string  = (char*)calloc(MAX_SIZE, sizeof(char));
    char* second_string = (char*)calloc(MAX_SIZE, sizeof(char));

    int first_value  = put_cmp_value(temp_string, assembled_text, &index_in_assembled_text, &number_of_byte, first_string,  &first_type_of_value);
    int second_value = put_cmp_value(temp_string, assembled_text, &index_in_assembled_text, &number_of_byte, second_string, &second_type_of_value);

    listing(input_file->listing_file, NOTHING, FIVE_ARGUMENTS, OPERATION_CODE_CMP, first_type_of_value, first_value, second_type_of_value, second_value);
    listing(input_file->listing_file, "|", SPACE);
    listing(input_file->listing_file, "cmp", SPACE);
    listing(input_file->listing_file, first_string, SPACE);
    listing(input_file->listing_file, second_string, END_LINE);
)
)

DEFINE_COMMANDS ( je, OPERATION_CODE_JE, 1,
PROCESSOR (
    if(fabs(first_comparison - second_comparison) < EPSILON) {
        GET_ADDRESS_LABEL_GO_TO
    } else {
        int address_label = get_byte_from_text(processor, &now_byte);
        if(fabs(address_label - OPERATION_CODE_RECURSIVE_RET) < EPSILON) {
            RECURSIVE_RETURN;
        }
    }
),
DISASSEMBLER (
    PUT_ADDRESS_LABEL_GO_TO(OPERATION_CODE_JE)
),
ASSEMBLER (
    CONDITION_COMMANDS(OPERATION_CODE_JE)
)
)

DEFINE_COMMANDS ( jne, OPERATION_CODE_JNE, 1,
PROCESSOR (
    if(fabs(first_comparison - second_comparison) >= EPSILON) {
        GET_ADDRESS_LABEL_GO_TO
    } else {
        int address_label = get_byte_from_text(processor, &now_byte);
        if(fabs(address_label - OPERATION_CODE_RECURSIVE_RET) < EPSILON) {
            RECURSIVE_RETURN;
        }
    }
),
DISASSEMBLER (
    PUT_ADDRESS_LABEL_GO_TO(OPERATION_CODE_JNE)
),
ASSEMBLER (
    CONDITION_COMMANDS(OPERATION_CODE_JNE)
)
)

DEFINE_COMMANDS ( ja, OPERATION_CODE_JA, 1,
PROCESSOR (
    if(first_comparison > second_comparison) {
        GET_ADDRESS_LABEL_GO_TO
    } else {
        int address_label = get_byte_from_text(processor, &now_byte);
        if(fabs(address_label - OPERATION_CODE_RECURSIVE_RET) < EPSILON) {
            RECURSIVE_RETURN;
        }
    }
),
DISASSEMBLER (
    PUT_ADDRESS_LABEL_GO_TO(OPERATION_CODE_JA)
),
ASSEMBLER (
    CONDITION_COMMANDS(OPERATION_CODE_JA)
)
)

DEFINE_COMMANDS ( jae, OPERATION_CODE_JAE, 1,
PROCESSOR (
    if(first_comparison >= second_comparison) {
        GET_ADDRESS_LABEL_GO_TO
    } else {
        int address_label = get_byte_from_text(processor, &now_byte);
        if(fabs(address_label - OPERATION_CODE_RECURSIVE_RET) < EPSILON) {
            RECURSIVE_RETURN;
        }
    }
),
DISASSEMBLER (
    PUT_ADDRESS_LABEL_GO_TO(OPERATION_CODE_JAE)
),
ASSEMBLER (
    CONDITION_COMMANDS(OPERATION_CODE_JAE)
)
)

DEFINE_COMMANDS ( jb, OPERATION_CODE_JB, 1,
PROCESSOR (
    if(first_comparison < second_comparison) {
        GET_ADDRESS_LABEL_GO_TO
    } else {
        int address_label = get_byte_from_text(processor, &now_byte);
        if(fabs(address_label - OPERATION_CODE_RECURSIVE_RET) < EPSILON) {
            RECURSIVE_RETURN;
        }
    }
),
DISASSEMBLER (
    PUT_ADDRESS_LABEL_GO_TO(OPERATION_CODE_JB)
),
ASSEMBLER (
    CONDITION_COMMANDS(OPERATION_CODE_JB)
)
)

DEFINE_COMMANDS ( jbe, OPERATION_CODE_JBE, 1,
PROCESSOR (
    if(first_comparison <= second_comparison) {
        GET_ADDRESS_LABEL_GO_TO
    } else {
        int address_label = get_byte_from_text(processor, &now_byte);
        if(fabs(address_label - OPERATION_CODE_RECURSIVE_RET) < EPSILON) {
            RECURSIVE_RETURN;
        }
    }
),
DISASSEMBLER (
    PUT_ADDRESS_LABEL_GO_TO(OPERATION_CODE_JBE)
),
ASSEMBLER (
    CONDITION_COMMANDS(OPERATION_CODE_JBE)
)
)

DEFINE_COMMANDS ( call, OPERATION_CODE_CALL, 1,
PROCESSOR (
    int address_label = get_byte_from_text(processor, &now_byte);

    stack_push(&(processor->call_stack), now_byte);
    now_byte = address_label - 2;
),
DISASSEMBLER (
    PUT_ADDRESS_LABEL_GO_TO(OPERATION_CODE_CALL)
),
ASSEMBLER (
    CONDITION_COMMANDS(OPERATION_CODE_CALL)
)
)

DEFINE_COMMANDS ( ret, OPERATION_CODE_RET, 0,
PROCESSOR (
    int go_to_address = STACK_BACK(processor->call_stack);
    STACK_POP(processor->call_stack);
    now_byte = go_to_address;
),
DISASSEMBLER (
    put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_RET], &disasm_struct, END_LINE);
),
ASSEMBLER (
    put_opcode_into_assembled_text(OPERATION_CODE_RET, assembled_text, &index_in_assembled_text, &number_of_byte);

    listing(input_file->listing_file, NOTHING, ONE_ARGUMENT, OPERATION_CODE_RET);
    listing(input_file->listing_file, "|", SPACE);
    listing(input_file->listing_file, TEXT_OPERATION[OPERATION_CODE_RET], END_LINE);
)
)

DEFINE_COMMANDS ( meow, OPERATION_CODE_MEOW, 0,
PROCESSOR (
    printf("meow\n");
),
DISASSEMBLER (
    put_char_into_disassembled_text(TEXT_OPERATION[OPERATION_CODE_MEOW], &disasm_struct, END_LINE);
),
ASSEMBLER (
    printf("--%c\n", char(OPERATION_CODE_MEOW));
    put_opcode_into_assembled_text(OPERATION_CODE_MEOW, assembled_text, &index_in_assembled_text, &number_of_byte);

    listing(input_file->listing_file, NOTHING, ONE_ARGUMENT, OPERATION_CODE_MEOW);
    listing(input_file->listing_file, "| meow", END_LINE);
)
)
