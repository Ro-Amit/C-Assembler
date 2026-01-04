/*
 * File: memory.c
 * Description: Implements memory initialization functions for instruction,
 *              data, and temporary memory structures used by the assembler.
 */

#include "../headers/memory.h"

/* Initializes instruction and data memory structures. */
void initialize_memory(InstructionMemory *inst_mem, DataMemory *data_mem)
{
    int i;

    /* Reset instruction memory */
    for (i = 0; i < MAX_MEMORY_SIZE; i++) {
        inst_mem->memory[i] = 0;
    }
    inst_mem->IC = STARTING_IC;
    inst_mem->ICF = STARTING_IC;
    /* Reset data memory */
    for (i = 0; i < MAX_MEMORY_SIZE; i++) {
        data_mem->memory[i] = 0;
    }
    data_mem->DC = STARTING_DC;
    data_mem->DCF = STARTING_DC;
}

/* Initializes the temporary memory structure. */
void initialize_temp_memory(TempMemory *temp_mem)
{
    int i;
    for (i = 0; i < MAX_TEMP_MEMORY; i++)
    {
        temp_mem->memory[i] = 0;
    }
    temp_mem->TC=STARTING_TC;
}
