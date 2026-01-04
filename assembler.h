#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include <string.h>
#include "./data_structures/headers/instructions_details.h"
#include "./header_files/preprocess.h"
#include "./header_files/firstpass.h"
#include "data_structures/headers/memory.h"
#include "./data_structures/headers/symbol_table.h"
#include "./data_structures/headers/macro_list.h"
#include "./header_files/secondpass.h"
#include "./data_structures/headers/struct_manager.h"
#include "header_files/output_maker.h"
#include "./header_files/globals.h"
/**
 * @brief Entry point for the assembler program.
 *
 * Handles command-line arguments for input file names, and coordinates the preprocessing,
 * first pass, second pass, and output generation steps for each provided source file.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of C-string arguments (file names to process).
 * @return int 0 if all files are processed successfully, 1 if an error occurs.
 */
int main(const int argc, char *argv[]);
#endif
