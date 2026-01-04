#include "../headers/struct_manager.h"
#include <stdlib.h>
#include <stdio.h>

/*
 * File: struct_manager.c
 * Description: Implements initialization and cleanup functions for
 *              assembler-wide data structures used in different phases.
 */

/* Initializes assembler data structures based on the current phase. */
int init_data_structures(DATA_STRUCTURES *ds, const ASSEMBLER_STATE state, const int Size)
{
    switch (state)
    {
        case PREPROCESS:
            ds->macro_table =NULL;
            ds->instruction_details =NULL;
            (ds->symbol_table.head) = NULL;
            ds->external_usages = NULL;
            return 1;

        case FIRSTPASS:
            create_SymbolTable(&ds->symbol_table);

            initialize_memory(&ds->inst_mem, &ds->data_mem);
            initialize_temp_memory(&ds->temp_mem);


            ds->instruction_details = initialize_instructions_details(Size);
            if (!ds->instruction_details) return 0;

            ds->external_usages  = initialize_external_usages(Size);
            if (!ds->external_usages) return 0;

            return 1;

        case SECONDPASS:
            return 1;
    }
    return 1;
}

/* Frees all allocated assembler data structures. */
void free_data_structures(DATA_STRUCTURES *ds)
{
    if (ds->macro_table)
        free_list((ds->macro_table));/*free macro table*/
    if (ds->symbol_table.head)
        freeSymbolTable(&ds->symbol_table);
    if (ds->instruction_details)
        free(ds->instruction_details);
    if (ds->external_usages)
        free(ds->external_usages);
}
