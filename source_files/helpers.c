#include "../header_files/helpers.h"

/*
 * File: helpers.c
 * Description: Implements helper functions for label validation, memory
 *              handling, and checking assembly directives such as .data,
 *              .string, .extern, and .entry.
 */

#include "../header_files/globals.h"

/* Removes leading spaces and tabs from a string. */
char *remove_leading_spaces(char *str)
{
    while (*str && isspace((unsigned char)*str)) {
        str++; /* Move forward until the first non-space */
    }
    return str;
}

/* Validates a label name based on various criteria (e.g., starts with a letter, no special characters). */
int validate_label_name(const char *label_name, SymbolTable *table, macro_node *macro_table,const int allow_check_duplicates)
{
    int i;

    if (strlen(label_name) == 0)
    {
        printf("%d |-->Error: Empty label.\n",GLOBAL_LINE_NUMBER);
        return FALSE;
    }
    /* cehck first character is a letter */
    if (!isalpha(label_name[0])) {
        printf("%d |-->Error:  Label name '%s' must start with a letter.\n",GLOBAL_LINE_NUMBER, label_name);
        return FALSE;
    }

    /* check only letters and digits */
    for (i = 0; label_name[i] != '\0'; i++) {
        if (!isalnum(label_name[i])) {
            printf("%d |-->Error:  Label name '%s' contains invalid characters '%c'. Only letters and digits are allowed.\n",GLOBAL_LINE_NUMBER, label_name,label_name[i]);
            return FALSE;
        }
    }

    /*check if label name is a reserved word */
    if (is_reserved_word(label_name)) {
        printf("%d |-->Error:  Label name '%s' is a reserved word.\n",GLOBAL_LINE_NUMBER, label_name);
        return FALSE;
    }
    if (allow_check_duplicates==FALSE) return TRUE;

    /* Check if label exists in symbol table */
    if (findSymbol(table, label_name) != NULL) {
        printf("%d |-->Error:  Label name '%s' already exists in the symbol table.\n",GLOBAL_LINE_NUMBER, label_name);
        return FALSE;
    }
    /* Check if label exists in macro table */
    if (search_list(macro_table, label_name) != NULL) {
        printf("%d |-->Error:  Label name '%s' is already defined as a macro.\n",GLOBAL_LINE_NUMBER, label_name);
        return FALSE;
    }

    return TRUE; /* Label is valid */
}

/* Checks if a label is a reserved word. */
int is_reserved_word(const char *label_name)
{
    int i;
    for (i=0; RESERVED_WORDS[i]!=NULL;i++)
    {
        if (strcmp(label_name, RESERVED_WORDS[i]) == 0)
            return TRUE;
    }
    return FALSE;
}

/* Checks if a line contains a label definition (ends with ':'). */
int is_label_def(const char *line, char *label_name,ASSEMBLER_STATE State)
{
    int i = 0;

    /*copy until ':' or end of the line*/
    while (line[i] != ':' && line[i] != '\0') {
        if (i>=MAX_LABEL_NAME){
            {
                if (State!=PREPROCESS)
                    printf("%d |-->Error: Label name is too long in Line: %s",GLOBAL_LINE_NUMBER, line);
                return -1;
            }}
        label_name[i] = line[i];
        i++;
    }
    label_name[i] = '\0';  /* null terminate the labelname*/

    /* if valid name and ends with : then label is found*/
    if ( line[i] == ':')
        return TRUE;

    return FALSE;
}

/* Checks if a line contains a .data or .string directive. */
int is_store_instruction(const char *line)
{
    if (strncmp(line, ".data", 5) == 0 && isspace(line[5])) {
        return dot_DATA;
    }
    if (strncmp(line, ".string", 7) == 0 && isspace(line[7])) {
        return dot_STRING;
    }
    return NONE;
}

/* Processes and stores data based on the directive type (.data or .string). */
int store_data(const char *line, const DataType dtype,DataMemory *data_mem)
{
    if (dtype == dot_DATA) {
        return store_data_values(line+strlen(".data "),data_mem);
    }
    if (dtype == dot_STRING) {
        return store_string_values(line+strlen(".string "), data_mem);
    }
    return 0;
}

/* Stores numerical data from a .data directive. */
int store_data_values(const char *line,DataMemory *data_mem)
{
    int words_stored = 0, i = 0, num, sign = 1, has_digit = FALSE;
    int digit_count;
    TempMemory temp_mem;
    initialize_temp_memory(&temp_mem);

    /*read line*/
    while (line[i] != '\0')
    {
        /*skip spaces*/
        if (isspace(line[i])) {
            i++;
            continue;
        }
        /*check for sing*/
        if (line[i] == '+' || line[i] == '-') {
            sign = (line[i] == '-') ? -1 : 1;
            i++;
        }
        /*read and validate number*/
        if (isdigit(line[i])) {
            num = 0;
            has_digit = TRUE;
            digit_count = 0;
            /*read number*/
            while (isdigit(line[i]))
            {
                /* dont allow excessively long numbers :=more then 10 digits */
                if (digit_count++ > 10) {
                    printf("%d |-->Error:  Number too long in .data\n",GLOBAL_LINE_NUMBER);
                    return 0;
                }
                /*convert char to int*/
                num = num * 10 + (line[i] - '0');
                i++;
            }
            num = num * sign;

            if (!isspace(line[i]) && line[i] != ',' && line[i] != '\0') {
                printf("%d |-->Error:  Invalid number format in .data\n",GLOBAL_LINE_NUMBER);
                return 0;

            }

            if (data_mem->DC+temp_mem.TC >= MAX_MEMORY_SIZE) {
                printf("%d |-->Error:  Data memory overflow\n",GLOBAL_LINE_NUMBER);
                return 0;
            }
            temp_mem.memory[temp_mem.TC] = num;
            temp_mem.TC++;
            words_stored++;
            sign = 1;
        }

        else if (line[i] == ',')
        {
            if (!has_digit)
            {
                printf("%d |-->Error:  Missing number before comma in .data\n",GLOBAL_LINE_NUMBER);
                return 0;
            }
            has_digit = FALSE;
            i++;
        }
        else
        {
            printf("%d |-->Error:  Invalid character '%c' in .data\n",GLOBAL_LINE_NUMBER, line[i]);
            return 0;
        }
    }
    if (!has_digit) {
        printf("%d |-->Error:  .data Empty or ending with a comma\n",GLOBAL_LINE_NUMBER);
        return 0;
    }

    /*if here no error detected. storing in data mem and incrementing DC*/
    for (i=0;i<temp_mem.TC;i++)
    {
        data_mem->memory[data_mem->DC+i] = temp_mem.memory[i];
    }
    data_mem->DC+=temp_mem.TC;
    return words_stored;
}

/* Stores string values from a .string directive. */
int store_string_values(const char *line,DataMemory *data_mem)
{
    int words_stored = 0, i=0;
    TempMemory temp_mem;
    initialize_temp_memory(&temp_mem);

    /*find opening quote*/
    while (line[i] != '"' && line[i] != '\0') {
        i++;
    }
    /*check for .string without opening quote*/
    if (line[i] == '\0') {
        printf("%d |-->Error:  Missing opening quote in .string\n",GLOBAL_LINE_NUMBER);
        return 0;
    }
    i++;

    /*store char inside quotes*/
    while (line[i] != '"' && line[i] != '\0') {
        if (data_mem->DC+temp_mem.TC >= MAX_MEMORY_SIZE) {
            printf("%d |-->Error:  Data memory overflow\n",GLOBAL_LINE_NUMBER);
            return 0;
        }
        if ((unsigned char)line[i] > 127) { /* ASCII validation */
            printf("%d |-->Error:  Invalid non-ASCII character in .string\n",GLOBAL_LINE_NUMBER);
            return 0;
        }
        temp_mem.memory[temp_mem.TC] = (int)line[i];
        temp_mem.TC++;
        words_stored++;
        i++;
    }
    /*check that quotes are being closed*/
    if (line[i] != '"') {
        printf("%d |-->Error: Missing closing quote in .string\n",GLOBAL_LINE_NUMBER);
        return 0;
    }
    i++;/*move past closing " */
    /* check for no extra characters after closing quote */
    while (line[i] != '\0') {
        if (!isspace(line[i])) {
            printf("%d |-->Error:  Invalid character '%c' after closing quote in .string\n",GLOBAL_LINE_NUMBER, line[i]);
            return 0;
        }
        i++;
    }


    if (data_mem->DC+temp_mem.TC  >= MAX_MEMORY_SIZE) {
        printf("%d |-->Error:  Data memory overflow\n",GLOBAL_LINE_NUMBER);
        return 0;
    }
    temp_mem.memory[temp_mem.TC] = 0;  /*null terminate string*/
    temp_mem.TC++;
    words_stored++;
    /*if here no error detected, write to data mem and increment DC*/
    for (i=0;i<temp_mem.TC;i++)
    {
        data_mem->memory[data_mem->DC+i] = temp_mem.memory[i];
    }
    data_mem->DC+=temp_mem.TC;

    return words_stored;
}

/* Checks if a line contains an .extern directive. */
int is_extern(const char *line,char *label_name)
{
    if ( strncmp(line,".extern",7) != 0)
    {
        label_name[0] = '\0';
        return FALSE;
    }
    return TRUE;

}

/* Checks if a line contains an .entry directive. */
int is_entry(const char *line,char *label_name)
{
    if ( strncmp(line,".entry",6) != 0)
    {
        label_name[0] = '\0';
        return FALSE;
    }
    return TRUE;
}

/* Validates the .extern directive and its label. */
int validate_extern(const char *line,char *label_name,SymbolTable *symbol_table,macro_node *macro_table,ASSEMBLER_STATE State)
{
    int i=0,j=0;
    if (!isspace(line[i]))
    {
        printf("%d |-->Error:  .extern without space between label and .extern\n",GLOBAL_LINE_NUMBER);
        return FALSE;
    }
    /*skip spaces and check if reached end of line*/
    while (isspace(line[i]) )
    {
        i++;
    }
    if (line[i] == '\0')
    {
        printf("%d |-->Error:  .extern without label\n",GLOBAL_LINE_NUMBER);
        return FALSE;
    }

    /*read label*/
    while (line[i]!='\0' && !isspace(line[i]) && j<MAX_LABEL_NAME-1)
    {
        label_name[j] = line[i];
        i++;
        j++;
    }
    label_name[j] = '\0';/*null terminate string*/
    if (State==SECONDPASS) return TRUE;
    /*check overflow and no other characters in the line*/
    while (isspace(line[i]))
    {
        i++;
    }
    if (line[i] != '\0')
    {
        printf("%d |-->Error:  .extern has unexpected extra characters\n",GLOBAL_LINE_NUMBER);
        return FALSE;
    }

    return validate_label_name(label_name,symbol_table,macro_table,TRUE);
}

/* Validates the .entry directive and its label. */
int validate_entry(const char *line,char *label_name,SymbolTable *symbol_table,macro_node *macro_table,ASSEMBLER_STATE State)
{
    int i=0,j=0;
    Symbol *current_symbol;
    if (!isspace(line[i]))
    {
        printf("%d |-->Error:  .entry without space between label and .entry\n",GLOBAL_LINE_NUMBER);
        return FALSE;
    }
    while (isspace(line[i]) )
    {
        i++;
    }
    if (line[i] == '\0')
    {
        printf("%d |-->Error:  .entry without label\n",GLOBAL_LINE_NUMBER);
        return FALSE;
    }
    /*read label*/
    while (line[i]!='\0' && !isspace(line[i]))
    {
        label_name[j] = line[i];
        i++;
        j++;
    }
    label_name[j] = '\0';/*null terminate*/
    current_symbol=findSymbol(symbol_table,label_name);
    if (current_symbol != NULL && State==SECONDPASS)
    {
        if ((current_symbol->types & EXTERNAL)==EXTERNAL)
        {
            printf("%d |-->Error:  .entry '%s' already defined external\n",GLOBAL_LINE_NUMBER,current_symbol->name);
            return FALSE;
        }
        if ((current_symbol->types & ENTRY)==ENTRY)
        {
            printf("%d |-->Error:  .entry '%s' already defined as entry before\n",GLOBAL_LINE_NUMBER,current_symbol->name);
            return FALSE;
        }

    }
    if (search_list(macro_table,label_name) != NULL && State==SECONDPASS)
    {
        printf("%d |-->Error:  .extern '%s' already defined as macro before\n",GLOBAL_LINE_NUMBER,label_name);
        return FALSE;
    }
    if (State==SECONDPASS) return TRUE;

    /* check overflow and no other characters in the line */
    while (isspace(line[i]))
    {
        i++;
    }
    if (line[i] != '\0')
    {
        printf("%d |-->Error:  .entry has unexpected extra characters\n",GLOBAL_LINE_NUMBER);
        return FALSE;
    }


    return validate_label_name(label_name,symbol_table,macro_table,FALSE);
}
