#include "../header_files/firstpass.h"
/*
 * File: firstpass.c
 * Description: Implements the first pass of the assembler.
 *              Handles labels, directives (.data, .entry, .extern),
 *              and memory updates.
 */

/* Updates the symbol table for data symbols after the first pass. */
void update_symbolTable_firstpass(const SymbolTable* symbol_table,const int ICF)
{
    Symbol* current = symbol_table->head;
    while (current != NULL) {
        if ((current->types & DATA)==DATA) {
            current->address += ICF;
        }
        current = current->next;
    }
}
/* Executes the first pass of the assembler, processing labels, directives, and instructions. */
int firstpass_main(char file_name[], DATA_STRUCTURES *data_structures)
{
    FILE *fp;
    char line[MAX_LINE_LENGTH];/*buffer to store line*/
    char label_name[MAX_LABEL_NAME];/*buffer to store label name*/
    char label_name_copy[MAX_LABEL_NAME];
    char *current_line_pos;
    int label_flag = FALSE,label_def_flag=FALSE;
    int L;
    int Datatype=NONE;
    int num_storage_words=0;
    int ERROR_flag=FALSE,Result;

    /*store file names with .am endings*/
    char source_filename[MAX_FILENAME_LENGTH];
    GLOBAL_LINE_NUMBER=0;
    sprintf(source_filename, "%s.am", file_name);

    /*open after macro file for reading*/
    fp = fopen(source_filename, "r");
    if (fp == NULL)
    {
        printf("|-->Error: in opening file\n");
        return 0;
    }
    while (fgets(line, MAX_LINE_LENGTH, fp))
    {
        GLOBAL_LINE_NUMBER++;
        /*check for labels in the beginning of the line*/
        label_name[0] = '\0';
        current_line_pos = line;
        /*printf("\n%d)  %s\n",index,current_line_pos);*/
        /*index++;*/

        label_def_flag = is_label_def(line, label_name,FIRSTPASS);
        if (label_def_flag==-1)
        {
            ERROR_flag=TRUE;
            continue;
        }
        strcpy(label_name_copy, label_name);
        if (label_def_flag == TRUE)
        {

            label_flag=validate_label_name(label_name,&(data_structures->symbol_table),data_structures->macro_table,TRUE);
            if (label_flag == FALSE)
            {
                ERROR_flag=TRUE;
                continue;
            }
            current_line_pos+=strlen(label_name)+1;/* +1 to be at next char after ':'*/
        }
        current_line_pos=remove_leading_spaces(current_line_pos);
        /*check for data storing instructions*/
        Datatype=is_store_instruction(current_line_pos);
        if (Datatype != NONE)
        {
            num_storage_words=store_data(current_line_pos,Datatype,&(data_structures->data_mem));
            if (num_storage_words && label_def_flag)
            {
                addSymbol(&(data_structures->symbol_table),label_name,data_structures->data_mem.DC-num_storage_words,DATA);
            }
            if (num_storage_words==0) ERROR_flag=TRUE;
            continue;
        }


        Result=is_extern(current_line_pos,label_name);
        if (Result==TRUE)
        {
            current_line_pos+=strlen(".extern");
            if (!validate_extern(current_line_pos,label_name,&(data_structures->symbol_table),data_structures->macro_table,FIRSTPASS))
            {
                ERROR_flag=TRUE;
                continue;
            }

            addSymbol(&(data_structures->symbol_table),label_name,FIRSTPASS_EXTERN_ADDRESS,EXTERNAL);
            continue;
        }


        Result=is_entry(current_line_pos,label_name);
        if (Result==TRUE)
        {
            current_line_pos+=strlen(".entry");
            if (!validate_entry(current_line_pos,label_name,&(data_structures->symbol_table),data_structures->macro_table,FIRSTPASS))
            {
                ERROR_flag=TRUE;
            }
            continue;
        }


        L=store_instruction(current_line_pos,(data_structures->instruction_details),GLOBAL_LINE_NUMBER);
        if (L && label_def_flag)
        {
            addSymbol(&(data_structures->symbol_table),label_name_copy,data_structures->inst_mem.IC,CODE);
        }
        if (L==0) ERROR_flag=TRUE;

        data_structures->instruction_details->memory[GLOBAL_LINE_NUMBER].IC=data_structures->inst_mem.IC;
        data_structures->instruction_details->memory[GLOBAL_LINE_NUMBER].line_number=GLOBAL_LINE_NUMBER;

        data_structures->inst_mem.IC+=L;
    }

    fclose(fp);

    data_structures->inst_mem.ICF=data_structures->inst_mem.IC;
    data_structures->data_mem.DCF=data_structures->data_mem.DC;
    update_symbolTable_firstpass(&(data_structures->symbol_table),data_structures->inst_mem.IC);

    if (data_structures->inst_mem.ICF > MAX_MEMORY_SIZE)
    {
        printf("|-->Error: Maximum memory size exceeded\n");
        return 0;
    }
    if (ERROR_flag==TRUE) return 0;
    return 1;
}
