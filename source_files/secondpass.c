#include "../header_files/secondpass.h"
/*
 * File: instructions_details.c
 * Description: Implements functions for managing instruction details,
 *              including initialization, validation, and memory handling.
 */
int need_complition(const int arr[])
{
    int i;
    for(i=0;i<MAX_WORDS_PER_INSTRUCTION;i++)
    {
        if(arr[i]==TRUE)
            return TRUE;
    }
    return FALSE;
}


/* Executes the second pass of the assembler, handling label resolution and encoding. */
int secondpass_main(char file_name[],DATA_STRUCTURES *data_structures)
{
    FILE *fp;
    char line[MAX_LINE_LENGTH];/*buffer to store line*/
    char label_name[MAX_LABEL_NAME];/*buffer to store label name*/
    char *current_line_pos;
    int label_def_flag=FALSE;
    Symbol* current_symbol = 0;
    int Error_flag=FALSE;
    INST_DET *inst_det;

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
        label_def_flag = is_label_def(line, label_name,SECONDPASS);
        if (label_def_flag == TRUE)
        {
            current_line_pos+=strlen(label_name)+1;/* +1 to be at next char after ':'*/
        }

        current_line_pos=remove_leading_spaces(current_line_pos);

        /*check for data storing instructions or .extern*/
        if (is_extern(current_line_pos,label_name) || is_store_instruction(current_line_pos))
            continue;

        if (is_entry(current_line_pos,label_name))
        {
            current_line_pos+=strlen(".entry");
            validate_entry(current_line_pos,label_name,&(data_structures->symbol_table),data_structures->macro_table,SECONDPASS);
            current_symbol=findSymbol(&(data_structures->symbol_table), label_name);
            if (current_symbol == NULL)
            {
                printf("%d |-->Error: .entry label %s not defined in the file\n",GLOBAL_LINE_NUMBER,label_name);
                Error_flag=TRUE;
                continue;
            }
            add_type_to_symbol(&(data_structures->symbol_table),label_name,ENTRY);
        }

        inst_det=&(data_structures->instruction_details->memory[GLOBAL_LINE_NUMBER]);
        if (inst_det->generated_error==TRUE)
        {
            printf("%d |-->Error: instruction %s generated error in the firstpass",GLOBAL_LINE_NUMBER,inst_det->instruction);
            Error_flag=TRUE;
            continue;
        }
        if (need_complition(inst_det->need_complition)==FALSE)
        {
            continue;
        }
        if (!complete_encoding(inst_det,&(data_structures->inst_mem),&(data_structures->symbol_table),data_structures->external_usages))
        {
            Error_flag=TRUE;
            inst_det->generated_error=TRUE;
        }
    }

    fclose(fp);
    if (Error_flag == TRUE)
        return 0;

    if (!dump_to_inst_mem(data_structures->instruction_details,&(data_structures->inst_mem)))
        return 0;


    return 1;
}

/* Completes the encoding process for an instruction during the second pass. */
int complete_encoding(INST_DET *inst_det,InstructionMemory *instruction_mem,const SymbolTable* symbol_table,EXTERNAL_USAGES *external_usages)
{
    int i;
    for(i=0;i<MAX_WORDS_PER_INSTRUCTION;i++)
    {
        if(inst_det->need_complition[i]==FALSE)
            continue;

        if (!detrermine_complition_type(inst_det,i,instruction_mem,symbol_table,external_usages))
            return 0;

    }
    return 1;
}

/* Determines the type of completion required for a given instruction operand in the second pass. */
int detrermine_complition_type(INST_DET *inst_det, const int index,InstructionMemory *instruction_mem, const SymbolTable* symbol_table, EXTERNAL_USAGES *external_usages)
{
    int word;
    /*printf("inst: %s, line: %d, op1: %s, op2: %s\n",inst_det->instruction,inst_det->line_number,inst_det->operand1,inst_det->operand2);*/
    if (index==1)
    {
        /*check if the first addressing mode is adding a storage word*/
        if (address_mode_to_storage_words(inst_det->mode1))
        {
            if (inst_det->mode1 == DIRECT || inst_det->mode1 == RELATIVE)
            {
                word=encode_word_secondpass(inst_det->operand1,inst_det->mode1,inst_det,index,symbol_table,external_usages);
                if (word==0) return 0;
                inst_det->memory_words[index]=word;
            }
        }
        /*else the second operand must be adding a storage word*/
        else
        {
            if (inst_det->mode2 == DIRECT || inst_det->mode2 == RELATIVE)
            {
                word=encode_word_secondpass(inst_det->operand2,inst_det->mode2,inst_det,index,symbol_table,external_usages);
                if (word==0) return 0;
                inst_det->memory_words[index]=word;
            }
        }

    }
    if (index==2)
    {
        if (inst_det->mode2 == DIRECT || inst_det->mode2 == RELATIVE)
        {
            word=encode_word_secondpass(inst_det->operand2,inst_det->mode2,inst_det,index,symbol_table,external_usages);
            if (word==0) return 0;
            inst_det->memory_words[index]=word;
        }

    }
    return 1;
}


/* Encodes a word in the second pass of the assembler, resolving labels and addresses. */
int encode_word_secondpass(char *label_name, const AddressingMode addressing_mode, const INST_DET *inst_det,const int Index, const SymbolTable* symbol_table, EXTERNAL_USAGES *external_usages)
{
    Symbol *symbol;
    int word=0;
    int jumping_distance=0;
    symbol = findSymbol(symbol_table,label_name);

    if (symbol == NULL)
    {
        printf("%d |-->Error: label %s not defined\n",GLOBAL_LINE_NUMBER,label_name);
        return 0;
    }
    if ((symbol->types & EXTERNAL) == EXTERNAL)
    {
        if (addressing_mode == RELATIVE)
        {
            printf("%d |-->Error: can't use Relative operand with external label %s\n",GLOBAL_LINE_NUMBER,label_name);
            return 0;
        }
        if (addressing_mode == DIRECT)
        {
            /*save external appearance to the list*/
            strcpy(external_usages->List[external_usages->Index].Label_name,label_name);
            external_usages->List[external_usages->Index].Address=inst_det->IC+Index;
            external_usages->Index++;
            /*store address as unsinged int*/
            word=set_bits(word,0,2,E);
            word=set_bits(word,3,23,(unsigned int)symbol->address);
            return word;
        }

    }
    else
    {
        if (addressing_mode == RELATIVE)
        {
            jumping_distance=symbol->address-inst_det->IC;
            word = set_bits(word,0,2,A);
            word = set_bits(word,3,23,jumping_distance);
            return word;
        }
        if (addressing_mode == DIRECT)
        {
            word=set_bits(word,0,2,R);
            /*store address as unsigned int*/
            word=set_bits(word,3,23,(unsigned int)symbol->address);
            return word;
        }

    }
    return word;

}

/* Dumps instruction details into the instruction memory. */
int dump_to_inst_mem(const INSTRUCTIONS_DETAILS *instructions_details,InstructionMemory *instruction_mem)
{
    int index;
    int j;
    INST_DET *inst_det;
    instruction_mem->IC=STARTING_IC;
    for (index=0;index<=instructions_details->Size;index++)
    {
        inst_det = &(instructions_details->memory[index]);
        if (strcmp(inst_det->instruction,"EMPTY")==0 || inst_det->generated_error == TRUE )
            continue;
        for (j=0;j<inst_det->num_memory_words;j++)
        {
            instruction_mem->memory[instruction_mem->IC + j] = inst_det->memory_words[j];
        }
        instruction_mem->IC+=inst_det->num_memory_words;
    }
    return instruction_mem->IC == instruction_mem->ICF;


}
