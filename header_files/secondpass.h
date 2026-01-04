#ifndef SECONDPASS_H
#define SECONDPASS_H

#include "helpers.h"
#include "handle_instructions.h"
#include "../data_structures/headers/struct_manager.h"
#include "../data_structures/headers/memory.h"
#include "../data_structures/headers/symbol_table.h"
#include "../data_structures/headers/instructions_details.h"
#include <stdio.h>
#include "globals.h"

/**
 * @brief Executes the second pass of the assembler, handling label resolution and encoding.
 * @param file_name The name of the input file (without extension).
 * @param data_structures Pointer to global assembler data structures.
 * @return 1 if successful, 0 if an error occurs.
 */
int secondpass_main(char file_name[],DATA_STRUCTURES *data_structures);

/**
 * @brief Dumps instruction details into the instruction memory.
 * @param instructions_details The details of the instructions.
 * @param instruction_mem Pointer to the instruction memory.
 * @return 1 if successful, 0 otherwise.
 */
int dump_to_inst_mem(const INSTRUCTIONS_DETAILS *instructions_details,InstructionMemory *instruction_mem);

/**
 * @brief Completes the encoding process for an instruction during the second pass.
 * @param inst_det Pointer to instruction details.
 * @param instruction_mem Pointer to instruction memory.
 * @param symbol_table Pointer to the symbol table.
 * @param external_usages Pointer to external usages.
 * @return 1 if successful, 0 otherwise.
 */
int complete_encoding(INST_DET *inst_det,InstructionMemory *instruction_mem,const SymbolTable* symbol_table,EXTERNAL_USAGES *external_usages);

/**
 * @brief Encodes a word in the second pass of the assembler, resolving labels and addresses.
 * @param label_name The label name to resolve.
 * @param addressing_mode The addressing mode of the operand.
 * @param inst_det Pointer to instruction details.
 * @param Index The index of the operand.
 * @param symbol_table Pointer to the symbol table.
 * @param external_usages Pointer to external usages.
 * @return Encoded word or 0 if an error occurs.
 */
int encode_word_secondpass(char *label_name,  AddressingMode addressing_mode, const INST_DET *inst_det, int Index, const SymbolTable* symbol_table, EXTERNAL_USAGES *external_usages);

/**
 * @brief Determines the type of completion required for a given instruction operand in the second pass.
 * @param inst_det Pointer to instruction details.
 * @param index The operand index.
 * @param instruction_mem Pointer to instruction memory.
 * @param symbol_table Pointer to symbol table.
 * @param external_usages Pointer to external usages.
 * @return 1 if successful, 0 otherwise.
 */
int detrermine_complition_type(INST_DET *inst_det, int index,InstructionMemory *instruction_mem, const SymbolTable* symbol_table,EXTERNAL_USAGES *external_usages);

#endif
