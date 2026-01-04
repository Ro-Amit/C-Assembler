#ifndef OUTPUT_MAKER_H
#define OUTPUT_MAKER_H
#include <stdlib.h>
#include "globals.h"
#include "../data_structures/headers/struct_manager.h"

#define LEFT_COL_LEN 7
#define RIGHT_COL_LEN 6
#define HEX_MASK 0xFFFFFF

/**
 * @brief Creates output files for the assembled source code.
 * @param file_name The name of the original input file (without extension).
 * @param data_structures Pointer to the assembler data structures.
 * @return 1 on success, 0 on failure.
 */
int create_output_files(char file_name[], const DATA_STRUCTURES *data_structures);
#endif
