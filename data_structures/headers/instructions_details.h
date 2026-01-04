#ifndef INSTRUCTIONS_DETAILS_H
#define INSTRUCTIONS_DETAILS_H
#include "../../header_files/globals.h"
typedef struct {
    int line_number;/*Line number in source file*/
    int IC;
    char instruction[MAX_INSTRUCTION_NAME];/* Instruction name*/
    int num_operands;/*Number of operands (0, 1, or 2)*/
    int num_memory_words;/* How many words the instruction takes in memory*/

    char operand1[MAX_OPERAND_LENGTH];/* First operand (if exists)*/
    AddressingMode mode1;/* Addressing mode for operand 1*/
    int value1;/*value of operand 1*/

    char operand2[MAX_OPERAND_LENGTH];/* First operand (if exists)*/
    AddressingMode mode2;/* Addressing mode for operand 1*/
    int value2;/*value of operand 1*/

    /*temp storage of the mem words generataed by the instruction*/
    int memory_words[MAX_WORDS_PER_INSTRUCTION];

    int generated_error;/*true if the instruction generated error*/
    int need_complition[MAX_WORDS_PER_INSTRUCTION];
} INST_DET;

typedef struct
{
    int Size;
    INST_DET *memory;
} INSTRUCTIONS_DETAILS;




typedef struct
{
    char Label_name[MAX_LABEL_NAME];
    int  Address;
} EXTERNAL_APPEARANCE;

typedef struct
{
    int Size;
    int Index;
    EXTERNAL_APPEARANCE *List;
}  EXTERNAL_USAGES;

/**
 * @brief Initializes and allocates memory for instruction details.
 * @param Size Number of instructions.
 * @return Pointer to allocated INSTRUCTIONS_DETAILS structure.
 */
INSTRUCTIONS_DETAILS *initialize_instructions_details(const int Size);
/**
 * @brief Initializes external usages list.
 * @param Size Number of expected external entries.
 * @return Pointer to allocated EXTERNAL_USAGES structure.
 */
void* initialize_external_usages(const int Size);


#endif
