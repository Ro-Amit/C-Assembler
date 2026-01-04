#ifndef FIRSTPASS_H
#define FIRSTPASS_H

#include "globals.h"
#include "helpers.h"
#include "../data_structures/headers/symbol_table.h"
#include "../data_structures/headers/memory.h"
#include "../data_structures/headers/struct_manager.h"
#include "handle_instructions.h"
/**
 * @brief Executes the first pass of the assembler.
 *
 * Processes labels, directives, and instructions.
 *
 * @param file_name Name of the file to process.
 * @param data_structures Pointer to the data structure.
 * @return int Returns 1 on success, 0 on error.
 */
int firstpass_main(char file_name[], DATA_STRUCTURES *data_structures);

#define FIRSTPASS_EXTERN_ADDRESS 0

#endif
