#ifndef STRUCT_MANAGER_H
#define STRUCT_MANAGER_H

#include "macro_list.h"
#include "symbol_table.h"
#include "memory.h"
#include "instructions_details.h"
#include "../../header_files/globals.h"

/*
 * File: struct_manager.h
 * Description: Defines the DATA_STRUCTURES container that groups all major
 *              components used throughout the assembler, and declares
 *              initialization and cleanup functions.
 */
typedef struct {
    macro_node *macro_table;
    SymbolTable symbol_table;

    InstructionMemory inst_mem;
    DataMemory data_mem;
    TempMemory temp_mem;

    INSTRUCTIONS_DETAILS *instruction_details;
    EXTERNAL_USAGES *external_usages;

} DATA_STRUCTURES;

/**
 * @brief Initializes all assembler data structures.
 * @param ds Pointer to DATA_STRUCTURES.
 * @param state Current assembler phase.
 * @param Size Number of instruction lines.
 * @return 1 on success, 0 on failure.
 */
int init_data_structures(DATA_STRUCTURES *ds, const ASSEMBLER_STATE state, const int Size);
/**
 * @brief Frees all allocated assembler data structures.
 * @param ds Pointer to DATA_STRUCTURES.
 */
void free_data_structures(DATA_STRUCTURES *ds);
#endif
