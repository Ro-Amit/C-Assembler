/*
 * File: handle_instructions.c
 * Description: Implements functions for instruction handling, including
 *              instruction parsing, operand validation, and encoding.
 */
#include "../header_files/handle_instructions.h"
int GLOBAL_LINE_NUMBER;
/* Instruction table*/
const Instruction INSTRUCTION_TABLE[] =
    {
    {"mov",  0, 0, 2, {IMMEDIATE, DIRECT, REGISTER,EOL}, {DIRECT, REGISTER,EOL}},
    {"cmp",  1, 0, 2, {IMMEDIATE, DIRECT, REGISTER,EOL}, {IMMEDIATE, DIRECT, REGISTER,EOL}},
    {"add",  2,  1, 2, {IMMEDIATE, DIRECT, REGISTER,EOL}, {DIRECT, REGISTER,EOL}},
    {"sub",  2,  2, 2, {IMMEDIATE, DIRECT, REGISTER,EOL}, {DIRECT, REGISTER,EOL}},
    {"lea",  4, 0, 2, {DIRECT,EOL}, {DIRECT, REGISTER,EOL}},
    {"clr",  5,  1, 1, {NO_TYPE,EOL}, {DIRECT, REGISTER,EOL}},
    {"not",  5,  2, 1, {NO_TYPE,EOL}, {DIRECT, REGISTER,EOL}},
    {"inc",  5,  3, 1, {NO_TYPE,EOL}, {DIRECT, REGISTER,EOL}},
    {"dec",  5,  4, 1, {NO_TYPE,EOL}, {DIRECT, REGISTER,EOL}},
    {"jmp",  9,  1, 1, {NO_TYPE,EOL}, {DIRECT, RELATIVE,EOL}},
    {"bne",  9,  2, 1, {NO_TYPE,EOL}, {DIRECT, RELATIVE,EOL}},
    {"jsr",  9,  3, 1, {NO_TYPE,EOL}, {DIRECT, RELATIVE,EOL}},
    {"red", 12, 0, 1, {NO_TYPE,EOL}, {DIRECT, REGISTER,EOL}},
    {"prn", 13, 0, 1, {NO_TYPE,EOL}, {IMMEDIATE, DIRECT, REGISTER,EOL}},
    {"rts", 14, 0, 0, {NO_TYPE,EOL}, {NO_TYPE,EOL}},
    {"stop", 15, 0, 0, {NO_TYPE,EOL}, {NO_TYPE,EOL}}
    };

/* Parses and stores an instruction with its operands, validating them. */
int store_instruction(const char *line,INSTRUCTIONS_DETAILS *instructions_details, const int inst_details_index)
{
    char instruction[MAX_INSTRUCTION_NAME];
    int operand1_value;
    int operand2_value;
    AddressingMode addressing_mode1, addressing_mode2;
    int expected_num_operands;
    int num_of_storage_words =MIN_WORDS_PER_INSTRUCTION;
    int validation_flag =FALSE;
    INST_DET inst_det;
    char operand1[MAX_OPERAND_LENGTH];
    char operand2[MAX_OPERAND_LENGTH];
    /*read inst name*/
    int i=0;
    while(!isspace(line[i]) && i<MAX_INSTRUCTION_NAME-1 && line[i]!= '\0')
    {
        instruction[i] = line[i];
        i++;
    }
    if (!isspace(line[i]) && line[i] != '\0')
    {
        printf("%d |-->Error: Instruction name too long\n",GLOBAL_LINE_NUMBER);
        return 0;
    }
    instruction[i] = '\0';/*nul terminate*/

    /*validate inst name*/
    if (!is_valid_instruction_name(instruction))
    {
        printf("%d |-->Error: Instruction name is not valid\n",GLOBAL_LINE_NUMBER);
        return 0;
    }
    /*check if line is without operands when it should be with*/
    expected_num_operands=get_expected_num_operands(instruction);
    if ((line[i] == '\0') && expected_num_operands > 0)
    {
        printf("%d |-->Error: missing operands\n",GLOBAL_LINE_NUMBER);
        return 0;
    }
    while(!isspace(line[i])) i++;
    if ((line[i] == '\0'  || line[i]=='\n') && expected_num_operands > 0)
    {
        printf("%d |-->Error: missing operands\n",GLOBAL_LINE_NUMBER);
        return 0;
    }
    /*handle operands*/
    validation_flag=get_addressing_types(&operand1_value,&addressing_mode1,
                                            &operand2_value,&addressing_mode2,
                                            expected_num_operands,line+i,
                                            operand1,operand2);

    if (!validation_flag)
    {
        return 0;
    }

    if (expected_num_operands>0)
        num_of_storage_words+=address_mode_to_storage_words(addressing_mode1);
    if (expected_num_operands > 1)
        num_of_storage_words+=address_mode_to_storage_words(addressing_mode2);

    inst_det.line_number=inst_details_index;
    strcpy(inst_det.instruction,instruction);
    inst_det.num_operands=expected_num_operands;
    inst_det.num_memory_words=num_of_storage_words;
    strcpy(inst_det.operand1,operand1);
    strcpy(inst_det.operand2,operand2);
    inst_det.mode1=addressing_mode1;
    inst_det.mode2=addressing_mode2;
    inst_det.value1=operand1_value;
    inst_det.value2=operand2_value;
    inst_det.generated_error=TRUE;

    validation_flag=encode_and_store_instruction_firstpass(instruction,expected_num_operands,
                                                            operand1_value,addressing_mode1,
                                                            operand2_value, addressing_mode2,
                                                            num_of_storage_words,
                                                            &inst_det);


    if (!validation_flag) return 0;

    instructions_details->memory[inst_details_index]=inst_det;
    return num_of_storage_words;

}

/* Checks if an instruction name is valid by comparing it with the instruction table. */
int is_valid_instruction_name(const char *instruction_name)
{
    int i;
    for (i=0;i<SIZE_OF_INSTRUCTION_TABLE;i++)
    {
        if (strcmp(instruction_name,INSTRUCTION_TABLE[i].instruction_name)==0)
            return 1;
    }
    return 0;
}
/* Retrieves the expected number of operands for a given instruction. */
int get_expected_num_operands(const char *instruction_name)
{
    int i;
    for (i=0;i<SIZE_OF_INSTRUCTION_TABLE;i++)
    {
        if (strcmp(instruction_name,INSTRUCTION_TABLE[i].instruction_name)==0)
            return INSTRUCTION_TABLE[i].num_operands;
    }
    /*should not reach here*/
    return -1;
}

/* Determines the addressing modes and values of the operands in an instruction. */
int get_addressing_types(int *operand1_value, AddressingMode *addressing_mode1,
                         int *operand2_value, AddressingMode *addressing_mode2,
                         int const expected_num_operands, const char *line,
                         char *operand1, char *operand2)
{
    int i = 0;
    int len;
    if (expected_num_operands == 0)
    {
        *addressing_mode1=NO_TYPE;
        *addressing_mode2=NO_TYPE;
        *operand1_value=0;
        *operand2_value=0;
        operand1[0]='\0';
        operand2[0]='\0';
        while (isspace(line[i])) i++;
        if (line[i] != '\0')
        {
            printf("%d |-->Error: %s\n",GLOBAL_LINE_NUMBER,line);
            printf("%d |-->Error: Unexpected characters '%c'after instruction\n",GLOBAL_LINE_NUMBER,line[i]);
            return 0;
        }
    }
    /* Skip leading spaces */
    while (isspace(line[i])) i++;
    if (expected_num_operands > 0 && line[i] == '\0')
    {
        printf("%d |-->Error: missing operands\n",GLOBAL_LINE_NUMBER);
        return 0;
    }

    /* Handle first operand */
    if (expected_num_operands > 0)
    {
        /* Read first operand */
        len = determine_operand_type(line + i, operand1, addressing_mode1);
        if (*addressing_mode1 == NO_TYPE)
        {
            printf("%d |-->Error: invalid first operand type\n",GLOBAL_LINE_NUMBER);
            return 0;
        }
        if (*addressing_mode1 == IMMEDIATE)
        {
            if (!process_immediate(operand1, operand1_value, i)) return 0;
        }
        else if (*addressing_mode1 == REGISTER)
        {
            if (!process_register(operand1, operand1_value)) return 0;
        }
        else
        {
            if (!process_label_operand(line+i, operand1, addressing_mode1, i)) return 0;
            *operand1_value = DIRECT_REL_FIRSTPASS;
        }

        /* Update position after operand */
        i += len;

        /* Skip spaces after operand */
        while (isspace(line[i])) i++;

    }
    /* Ensure a comma follows for a second operand */
    if (expected_num_operands > 1)
    {
        if (line[i] != ',')
        {
            printf("%d |-->Error: Missing comma between operands\n",GLOBAL_LINE_NUMBER);
            return 0;
        }
        i++; /* Move past comma */
    }

    /* Handle second operand */
    if (expected_num_operands > 1)
    {
        /* Skip spaces after comma */
        while (isspace(line[i])) i++;
        if (line[i]==',')
        {
            printf("%d |-->Error: double ',' or Missing second operand\n",GLOBAL_LINE_NUMBER);
            return 0;
        }
        /* Read second operand */
        len = determine_operand_type(line + i, operand2, addressing_mode2);
        if (*addressing_mode2 == NO_TYPE)
        {
            printf("%d |-->Error: invalid second operand type\n",GLOBAL_LINE_NUMBER);
            return 0;
        }
        if (*addressing_mode2 == IMMEDIATE)
        {
            if (!process_immediate(operand2, operand2_value, i)) return 0;
        }
        else if (*addressing_mode2 == REGISTER)
        {
            if (!process_register(operand2, operand2_value)) return 0;
        }
        else
        {
            if (!process_label_operand(line+i, operand2, addressing_mode2, i)) return 0;

            *operand2_value = DIRECT_REL_FIRSTPASS;
        }

        /* Update position after operand */
        i += len;
    }

    /* Skip trailing spaces */
    while (isspace(line[i])) i++;

    /* Ensure nothing follows the second operand */
    if (line[i] != '\0')
    {
        printf("%d |-->Error: Unexpected characters after operand\n",GLOBAL_LINE_NUMBER);
        return 0;
    }

    return 1; /* Success */
}

/* Determines the type of an operand and validates it. */
int determine_operand_type(const char *line, char *operand, AddressingMode *addressing_mode)
{
    int i = 0;
    int j = 0;
    int k = 0;
    char label_name[MAX_LABEL_NAME];
    *addressing_mode = NO_TYPE;
    /* Skip leading spaces */
    while (line[i] != '\0' && isspace(line[i])) i++;

    /* Determine addressing mode */
    if (line[i] == '#') {
        *addressing_mode = IMMEDIATE;
        i++;
    }
    else if (line[i] == '&')
    {
        *addressing_mode = RELATIVE;
        i++; /* Skip '&' */
    }
    else if (line[i+1] != '\0' && line[i] == 'r' && line[i+1] >= '0' && line[i+1] <= '7')
    {
        *addressing_mode = REGISTER;
    }
    else if (isalpha(line[i]))
    {
        j = i; /* Save current index */
        while (line[i] != '\0' && !isspace(line[i]) && line[i] != ',' && k < MAX_LABEL_NAME - 1)
        {
            label_name[k] = line[i];
            k++;
            i++;
        }
        label_name[k] = '\0';

        /* Validate label */
        if (validate_label_operand(label_name))
        {
            *addressing_mode = DIRECT;
        }
        else
        {
            i = j; /* If not a valid label, restore position */
            *addressing_mode = NO_TYPE;
        }
    }
    else
    {
        *addressing_mode = NO_TYPE;
    }

    /* Read operand until space, comma, or end of line */
    j = 0;
    while (line[i] != '\0' && !isspace(line[i]) && line[i] != ',')
    {
        operand[j] = line[i];
        i++;
        j++;
    }
    operand[j] = '\0';

    /* Return how many characters were read */
    return i;
}


/* Processes an immediate operand and extracts its value. */
int process_immediate(const char *line, int *operand1_value, int pos)
{

    int i = 0, sign=1;
    int num=0,digit_count;

    /*check for sign*/
    if (line[i] == '+' || line[i] == '-') {
        sign = (line[i] == '-') ? -1 : 1;
        i++;
    }
    /*read and validate number*/
    num = 0;
    digit_count = 0;
    /*read number*/
    while (isdigit(line[i]))
    {
        /* dont allow excessively long numbers :=more then 10 digits */
        if (digit_count > 10)
        {
            printf("%d |-->Error: Number too long in instruction\n",GLOBAL_LINE_NUMBER);
            return 0;
        }
        /*convert char to int*/
        num = num * 10 + (line[i] - '0');
        i++;
        digit_count++;
    }
    while (isspace(line[i]) && line[i]!='\0' && line[i]!=',' ) i++;
    if (!(line[i]==',' || line[i]=='\0'))
    {
        printf("%d |-->Error: invalid characters = %c after operand\n",GLOBAL_LINE_NUMBER,line[i]);
        return 0;
    }
    if (digit_count==0)
    {
        printf("%d |-->Error: missing number in immediate operand",GLOBAL_LINE_NUMBER);
        return 0;
    }
    num = num * sign;
    *operand1_value = num;
    return 1;

}

/* Processes a register operand and extracts its value. */
int process_register(const char *line, int *operand1_value)
{
    int i=0;
    if (line[i]=='r' && line[i+1]>='0' && line[i+1]<='7')
    {
        *operand1_value = line[i+1] - '0';
    }
    i+=2;
    while (isspace(line[i])) i++;
    if (line[i]=='\0' || line[i]==',' ) return 1;

    return 0;
}

/* Processes a label operand and extracts its value. */
int process_label_operand(const char *line, char *operand_value, AddressingMode *addressing_mode, int pos)
{
    int i = 0;
    int label_len = 0;
    if (*addressing_mode == RELATIVE) i++;

    while (line[i] != '\0' && !isspace(line[i]) && line[i] != ',')
    {
        if (label_len >= MAX_LABEL_NAME - 1)
        {
            printf("%d |-->Error: Label name too long\n",GLOBAL_LINE_NUMBER);
            return 0;
        }
        operand_value[label_len++] = line[i++];
    }
    operand_value[label_len] = '\0';

    return 1;
}
/* Encodes and stores the instruction during the first pass of assembly. */
int encode_and_store_instruction_firstpass(const char *instruction, int expected_num_operands,
                                            int operand1_value,AddressingMode addressing_mode1,
                                            int operand2_value, AddressingMode addressing_mode2,
                                            int num_of_storage_words,
                                            INST_DET *inst_det)
{
    int i;
    int first_word=0;
    int second_word=0;
    int third_word=0;
    int words_index;
    int words_array[MAX_WORDS_PER_INSTRUCTION];

    const int opcode=get_opcode(instruction);
    const int func=get_func(instruction);
    int dest_reg,source_reg;
    int dest_addressing,source_addressing;

    for (i = 0; i < MAX_WORDS_PER_INSTRUCTION; i++)
        inst_det->need_complition[i] = FALSE;

    if (expected_num_operands ==1)
    {
        /*first operand is target/dest operand*/
        dest_reg = (addressing_mode1 == REGISTER) ? operand1_value : 0;
        dest_addressing =(addressing_mode1 == NO_TYPE) ? 0: addressing_mode1;
        source_reg = 0;
        source_addressing = 0;
    }
    else
    {
        source_reg = (addressing_mode1 == REGISTER) ? operand1_value : 0;
        dest_reg = (addressing_mode2 == REGISTER) ? operand2_value : 0;
        source_addressing =(addressing_mode1 == NO_TYPE) ? 0: addressing_mode1;
        dest_addressing =(addressing_mode2 == NO_TYPE) ? 0: addressing_mode2;

    }
    if (!check_operands_types(instruction,expected_num_operands, addressing_mode1, addressing_mode2)) return 0;

    first_word=set_bits(first_word,18,23,opcode);
    first_word=set_bits(first_word,16,17,source_addressing);
    first_word=set_bits(first_word,13,15,source_reg);
    first_word=set_bits(first_word,11,12,dest_addressing);
    first_word=set_bits(first_word,8,10,dest_reg);
    first_word=set_bits(first_word,3,7,func);
    first_word=set_bits(first_word,0,2,A);

    /*if register type then already encoded in first word
     * direct and relative types will be handled in second pass
     */
    if (num_of_storage_words==2)
    {
        /*check if the first addressing mode is adding a storage word*/
        if (address_mode_to_storage_words(addressing_mode1))
        {
            if (addressing_mode1 == IMMEDIATE)
            {
                second_word=set_bits(second_word,3,23,operand1_value);
                second_word=set_bits(second_word,0,2,A);
            }
        }
        /*else the second operand must be adding a storage word*/
        else
        {
            if (addressing_mode2 == IMMEDIATE)
            {
                second_word=set_bits(second_word,3,23,operand2_value);
                second_word=set_bits(second_word,0,2,A);
            }
        }

    }
    if (num_of_storage_words==3)
    {
        if (addressing_mode2 == IMMEDIATE)
        {
            third_word=set_bits(third_word,3,23,operand2_value);
            third_word=set_bits(third_word,0,2,A);
        }

    }

    words_array[0]=first_word;
    words_array[1]=second_word;
    words_array[2]=third_word;

    inst_det->generated_error=FALSE;

    for (words_index=0;words_index<num_of_storage_words;words_index++)
    {
        inst_det->memory_words[words_index]=words_array[words_index];
        inst_det->need_complition[words_index]=(words_array[words_index]==0) ? TRUE : FALSE;
    }
    return 1;
}

/* Returns the number of memory words required for a given addressing mode. */
int address_mode_to_storage_words(const AddressingMode addressing_mode)
{
    if (addressing_mode == IMMEDIATE) return 1;
    if (addressing_mode == DIRECT) return 1;
    if (addressing_mode == RELATIVE) return 1;
    if (addressing_mode == REGISTER) return 0;
    return 0;

}

/* Sets specific bits in a number. */
int set_bits(int num, const int start_bit, const int end_bit, const int value) {
    const int num_bits = end_bit - start_bit + 1;
    const int mask = (1 << num_bits) - 1;
    num &= ~(mask << start_bit);
    num |= (value & mask) << start_bit;
    return num;
}

/* Validates if a label operand is properly formatted and not a reserved word. */
int validate_label_operand(const char *label_name)
{
    int i;
    /* check length */
    if (strlen(label_name) > MAX_LABEL_NAME) {
        printf("%d |-->Error: Label name '%s' exceeds maximum length of %d characters.\n",GLOBAL_LINE_NUMBER, label_name, MAX_LABEL_NAME);
        return FALSE;
    }

    /* cehck first character is a letter */
    if (!isalpha(label_name[0])) {
        printf("%d |-->Error: Label name '%s' must start with a letter.\n",GLOBAL_LINE_NUMBER, label_name);
        return FALSE;
    }

    /* check only letters and digits */
    for (i = 0; label_name[i] != '\0'; i++) {
        if (!isalnum(label_name[i])) {
            printf("%d |-->Error: Label name '%s' contains invalid characters. Only letters and digits are allowed.\n",GLOBAL_LINE_NUMBER, label_name);
            return FALSE;
        }
    }

    /*check if label name is a reserved word */
    if (is_reserved_word(label_name)) {
        printf("%d |-->Error: Label name '%s' is a reserved word.\n",GLOBAL_LINE_NUMBER, label_name);
        return FALSE;
    }
    return TRUE;
}


/* Retrieves the opcode of an instruction from the instruction table. */
int get_opcode(const char *instruction)
{
    int i;
    for (i=0; i<SIZE_OF_INSTRUCTION_TABLE;i++)
    {
        if (strcmp(instruction,INSTRUCTION_TABLE[i].instruction_name)==0)
            return INSTRUCTION_TABLE[i].opcode;
    }
    /*should not reach here*/
    return -1;
}

/* Retrieves the function code (funct) for an instruction. */
int get_func(const char *instruction)
{
    int i;
    for (i=0; i<SIZE_OF_INSTRUCTION_TABLE;i++)
    {
        if (strcmp(instruction,INSTRUCTION_TABLE[i].instruction_name)==0)
            return INSTRUCTION_TABLE[i].funct;
    }
    return -1;    /*should not reach here*/
}

/* Checks if an addressing mode is allowed for a specific instruction type. */
int is_allowed_add_type(const char *instruction,AddressingMode addressing_mode, SOURCE_OR_DESTINATION source_or_dest)
{
    int i;
    int j;

    for (i=0; i<SIZE_OF_INSTRUCTION_TABLE;i++)
    {
        /*find instruction*/
        if (strcmp(instruction,INSTRUCTION_TABLE[i].instruction_name)==0) break;
    }

    /*if looking for source operand find it in allowed types*/
    if (source_or_dest == SOURCE)
    {
        for (j=0; INSTRUCTION_TABLE[i].source_operands[j]!=EOL;j++)
        {
            if (addressing_mode == INSTRUCTION_TABLE[i].source_operands[j])
                return 1;
        }
    }
    /*if looking for destination operand find it in allowed types*/
    else if (source_or_dest == DESTINATION)
    {
        for (j=0;INSTRUCTION_TABLE[i].dest_operands[j]!=EOL;j++)
        {
            if (addressing_mode == INSTRUCTION_TABLE[i].dest_operands[j])
                return 1;
        }
    }
    /*if not found addressing type not allowed return false*/
    return 0;

}

/* Validates the types of operands for a given instruction. */
int check_operands_types(const char *instruction,const int expected_num_operands,
                        const AddressingMode addressing_mode1, const AddressingMode addressing_mode2)
{
    int allowed_type1,allowed_type2;
    if (expected_num_operands>1)
    {
        allowed_type1=is_allowed_add_type(instruction,addressing_mode1,SOURCE);
        allowed_type2=is_allowed_add_type(instruction,addressing_mode2,DESTINATION);
        if (!allowed_type1 && !allowed_type2)
        {
            printf("%d |-->Error: both operand types are invalid\n",GLOBAL_LINE_NUMBER);
            return FALSE;
        }
        if (!allowed_type1)
        {
            printf("%d |-->Error: source operand is invalid\n",GLOBAL_LINE_NUMBER);
            return FALSE;
        }
        if (!allowed_type2)
        {
            printf("%d |-->Error: destination operand is invalid\n",GLOBAL_LINE_NUMBER);
            return FALSE;
        }
        return TRUE;
    }

    if (expected_num_operands>0)
    {
        allowed_type1=is_allowed_add_type(instruction,addressing_mode1,DESTINATION);
        if (!allowed_type1)
        {
            printf("%d |-->Error: destination operand is invalid\n",GLOBAL_LINE_NUMBER);
            return FALSE;
        }
        return TRUE;
    }

    return addressing_mode1==NO_TYPE && addressing_mode2==NO_TYPE;
}
