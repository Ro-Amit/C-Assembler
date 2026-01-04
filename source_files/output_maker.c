#include "../header_files/output_maker.h"
#include <stdio.h>
#include "../header_files/output_maker.h"
#include <stdio.h>
#include <string.h>
/*
 * File: output_maker.c
 * Description: Implements functions to create object, entry, and external
 *              files from the assembler's output data structures.
 */

/* Adds leading characters to a string to match the desired length. */
void add_leading_char(char string[], const char c, const int length)
{
    const int original_len = strlen(string);
    int i;
    char temp[MAX_LINE_LENGTH];

    if (original_len < length)
    {
        for (i = 0; i < length - original_len; i++)
        {
            temp[i] = c;
        }
        temp[i] = '\0';
        strcat(temp, string);
        strcpy(string, temp);
    }
}

/* Converts a value to its hexadecimal representation and adds leading zeros. */
void get_bit_hex_string(const int value, char *hex_str)
{
    unsigned int masked_value;
    masked_value = value & HEX_MASK;
    sprintf(hex_str, "%X", masked_value);
    add_leading_char(hex_str, '0', RIGHT_COL_LEN);
}

/* Creates an object file (.ob) containing the assembler's instruction and data memory. */
int create_object_file(char file_name[], const DATA_STRUCTURES *data_structures)
{
    FILE *file;
    int i;
    char str1[MAX_LINE_LENGTH];
    char str2[MAX_LINE_LENGTH];
    char object_file_name[MAX_FILENAME_LENGTH];

    if (data_structures->data_mem.DCF==0 && data_structures->inst_mem.ICF==0)
    {
        printf("\t#Object File not created since empty");
        return -1;
    }

    sprintf(object_file_name, "%s.ob", file_name);
    file = fopen(object_file_name, "w");
    if (file == NULL)
    {
        printf("|-->Error: Object File could not be opened\n");
        return FALSE;
    }

    /* Write header line: ICF and DCF */
    sprintf(str1, "%d", data_structures->inst_mem.ICF - STARTING_IC);
    add_leading_char(str1, ' ', LEFT_COL_LEN);
    sprintf(str2, "%d", data_structures->data_mem.DCF);
    strcat(str1, " ");
    strcat(str1, str2);
    strcat(str1, "\n");
    fputs(str1, file);

    /* Write instruction memory */
    for (i = STARTING_IC; i < data_structures->inst_mem.ICF; i++)
    {
        sprintf(str1, "%d", i);
        add_leading_char(str1, '0', LEFT_COL_LEN);

        get_bit_hex_string(data_structures->inst_mem.memory[i], str2);

        strcat(str1, " ");
        strcat(str1, str2);
        strcat(str1, "\n");

        fputs(str1, file);
    }

    /* Write data memory */
    for (i = 0; i < data_structures->data_mem.DCF; i++)
    {
        sprintf(str1, "%d", i + data_structures->inst_mem.ICF);
        add_leading_char(str1, '0', LEFT_COL_LEN);

        get_bit_hex_string(data_structures->data_mem.memory[i], str2);

        strcat(str1, " ");
        strcat(str1, str2);
        strcat(str1, "\n");

        fputs(str1, file);
    }

    fclose(file);
    return TRUE;
}

/* Creates an entries file (.ent) listing all entry symbols and their addresses. */
int create_entries_file(char file_name[], const DATA_STRUCTURES *data_structures)
{
    FILE *file;
    char str1[MAX_LINE_LENGTH];
    char str2[MAX_LINE_LENGTH];
    Symbol* current;
    int found = FALSE;
    /*check that there is at least one entry symbol*/
    current= data_structures->symbol_table.head;
    while (current!=NULL)
    {
        if ((current->types & ENTRY) == ENTRY)
            found = TRUE;
        current = current->next;
    }

    if (!found)
    {
        printf("\t#Entries file not created since empty\n");
        return -1;
    }
    sprintf(str1, "%s.ent", file_name);
    file = fopen(str1, "w");
    if (file == NULL)
    {
        printf("|-->Error: Entries File could not be opened\n");
        return FALSE;
    }

    current= data_structures->symbol_table.head;
    while (current != NULL) {
        if ((current->types & ENTRY) == ENTRY)
        {
            strcpy(str1,current->name);
            strcat(str1, " ");
            sprintf(str2,"%d",current->address);
            add_leading_char(str2, '0', LEFT_COL_LEN);
            strcat(str1,str2);
            strcat(str1,"\n");
            fputs(str1, file);
        }
        current = current->next;
    }
    fclose(file);
    return TRUE;
}

/* Creates an externals file (.ext) listing all external symbol usages. */
int create_externals_file(char file_name[], const DATA_STRUCTURES *data_structures)
{
    FILE *file;
    char str1[MAX_LINE_LENGTH];
    char str2[MAX_LINE_LENGTH];
    int i;
    /*check that there is at least one entry symbol*/
    if (data_structures->external_usages->Index==0)
    {
        printf("\t#External File not created since empty\n");
        return -1;
    }

    sprintf(str1, "%s.ext", file_name);
    file = fopen(str1, "w");
    if (file == NULL)
    {
        printf("|-->Error: Externals File could not be opened\n");
        return FALSE;
    }

    for (i = 0; i < data_structures->external_usages->Index; i++)
    {
        sprintf(str1, "%s", data_structures->external_usages->List[i].Label_name);
        sprintf(str2, "%d", data_structures->external_usages->List[i].Address);
        add_leading_char(str2, '0', LEFT_COL_LEN);
        strcat(str1, " ");
        strcat(str1, str2);
        strcat(str1, "\n");
        fputs(str1, file);
    }
    fclose(file);
    return TRUE;

}
/* Creates all necessary output files (.ob, .ent, .ext) for the assembler. */
int create_output_files(char file_name[], const DATA_STRUCTURES *data_structures)
{
    int Result;
    Result=create_object_file(file_name, data_structures);
    if (Result==FALSE)
        return FALSE;
    if (Result==TRUE) printf("\tCreated Object file\n");

    Result=create_entries_file(file_name, data_structures);
    if (Result==FALSE)
        return FALSE;
    if (Result==TRUE) printf("\tCreated Entries file\n");

    Result=create_externals_file(file_name, data_structures);
    if (Result==FALSE)
        return FALSE;
    if (Result==TRUE) printf("\tCreated Externals file\n");

    return TRUE;
}
