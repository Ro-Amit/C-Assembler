#ifndef PREPROCESS_H
#define PREPROCESS_H
/*
 * File: preprocess.h
 * Description: Declares functions for the preprocessing phase of the assembler.
 *              Handles macro definitions and macro expansion.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../data_structures/headers/macro_list.h"
#include "../data_structures/headers/struct_manager.h"
#include "globals.h"
#include "helpers.h"


/* Function declarations */
/**
 * @brief Runs preprocessing on the input file (macro handling).
 * @param file_name Input file name.
 * @param ds Pointer to assembler data structures.
 * @return 1 on success, 0 on failure.
 */
int preprocess_file(char file_name[], DATA_STRUCTURES *ds);
/**
 * @brief Saves a macro definition to the macro list.
 * @param macro_name Name of the macro.
 * @param head Pointer to macro list.
 * @param fp File pointer to source file.
 */
void save_macro(const char *macro_name, macro_node **head, FILE *fp);
/**
 * @brief Checks if a line is a macro call.
 * @param line Line to check.
 * @param head Pointer to macro list.
 * @return 1 if it's a macro call, 0 otherwise.
 */
int is_macro_call(const char *line, macro_node *head);
/**
 * @brief Expands and writes a macro to the output file.
 * @param fp File pointer to output file.
 * @param macro_name Name of the macro to expand.
 * @param head Pointer to macro list.
 * @return 1 on success, 0 on failure.
 */
int expand_macro(FILE *fp, const char *macro_name, macro_node *head);
/**
 * @brief Checks if a line contains "endmacro".
 * @param line Line to check.
 * @return 1 if it's "endmacro", 0 otherwise.
 */
int is_end_macro(const char *line);
/**
 * @brief Checks if a line starts a macro definition.
 * @param line Line to check.
 * @param macro_name Buffer to store the macro name.
 * @return 1 if it's a macro definition, 0 otherwise.
 */
int is_macro_def(const char *line, char *macro_name);
/**
 * @brief Extracts a macro name from a definition or call line.
 * @param line Line to parse.
 * @param allow_to_check_mcro_name Flag for extra validation.
 * @return Pointer to macro name or NULL on failure.
 */
char *extract_macro_name(const char *line, int allow_to_check_mcro_name);

#endif
