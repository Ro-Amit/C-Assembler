#ifndef HELPERS_H
#define HELPERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "globals.h"
#include "../data_structures/headers/memory.h"
#include "../data_structures/headers/symbol_table.h"
#include "../data_structures/headers/macro_list.h"


typedef enum{NONE, dot_DATA , dot_STRING} DataType;

/*List of reserved words: Instructions - add, sub...
 *                        Directives - .data, .string, .extern ...
 *                        Registers - r0,r1,...,r7
 */
static const char *RESERVED_WORDS[] = {
    "mov", "cmp", "add", "sub", "lea",
    "clr", "not", "inc", "dec", "jmp",
    "bne", "jsr", "red", "prn", "rts",
    "stop",
    ".data", ".string", ".extern", ".entry",
    "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",
    NULL
};


/**
 * @brief Checks if a line contains a label definition.
 * @param line The input line to check.
 * @param label_name Buffer to store the extracted label name.
 * @param State Current assembler state.
 * @return TRUE if a label is found, FALSE otherwise.
 */
int is_label_def(const char *line, char *label_name,ASSEMBLER_STATE State);

/**
 * @brief Validates a label name based on various criteria.
 * @param label_name The label name to validate.
 * @param table Symbol table for checking duplicates.
 * @param macro_table Macro table for checking conflicts.
 * @param allow_check_duplicates Flag to allow checking for duplicates.
 * @return TRUE if the label name is valid, FALSE otherwise.
 */
int validate_label_name(const char *label_name, SymbolTable *table, macro_node *macro_table,const int allow_check_duplicates);

int is_reserved_word(const char *label_name);

/**
 * @brief Checks if a label is a reserved word (instruction, directive, or register).
 * @param label_name The label name to check.
 * @return TRUE if the label is a reserved word, FALSE otherwise.
 */
int is_reserved_word(const char *label_name);

/**
 * @brief Removes leading spaces and tabs from a string.
 * @param str The string to process.
 * @return Pointer to the first non-whitespace character.
 */
char *remove_leading_spaces(char *str);

/**
 * @brief Checks if a line contains a data storage directive (.data or .string).
 * @param line The line to check.
 * @return `dot_DATA` for .data, `dot_STRING` for .string, `NONE` otherwise.
 */
int is_store_instruction(const char *line);

/**
 * @brief Processes a storage directive and stores data accordingly.
 * @param line The input line with the directive.
 * @param dtype The data type to store (dot_DATA or dot_STRING).
 * @param data_mem Pointer to data memory.
 * @return The number of words stored, or 0 if an error occurs.
 */
int store_data(const char *line, DataType dtype, DataMemory *data_mem);


/**
 * @brief Stores data values from a .data directive.
 * @param line The input line containing .data.
 * @param data_mem Pointer to data memory.
 * @return The number of words stored, or 0 if an error occurs.
 */
int store_data_values(const char *line,DataMemory *data_mem);


/**
 * @brief Stores string values from a .string directive.
 * @param line The input line containing .string.
 * @param data_mem Pointer to the data memory.
 * @return The number of words stored, or 0 if an error occurs.
 */
int store_string_values(const char *line,DataMemory *data_mem);

int is_extern(const char *line,char *label_name);

/**
 * @brief Checks if a line contains an extern directive.
 * @param line The input line.
 * @param label_name Buffer to store the extracted label name.
 * @return TRUE if the line contains an extern directive, FALSE otherwise.
 */
int is_extern(const char *line,char *label_name);

int is_entry(const char *line,char *label_name);

/**
 * @brief Checks if a line contains an entry directive.
 * @param line The input line.
 * @param label_name Buffer to store the extracted label name.
 * @return TRUE if the line contains an entry directive, FALSE otherwise.
 */
int is_entry(const char *line,char *label_name);

/**
 * @brief Validates an extern directive.
 * @param line The input line.
 * @param label_name Buffer for the extracted label name.
 * @param symbol_table Pointer to the symbol table.
 * @param macro_table Pointer to the macro table.
 * @param State Current assembler state.
 * @return TRUE if valid, FALSE otherwise.
 */
int validate_extern(const char *line,char *label_name,SymbolTable *symbol_table,macro_node *macro_table,ASSEMBLER_STATE State);

/**
 * @brief Validates an entry directive.
 * @param line The input line.
 * @param label_name Buffer for the extracted label name.
 * @param symbol_table Pointer to the symbol table.
 * @param macro_table Pointer to the macro table.
 * @param State Current assembler state.
 * @return TRUE if valid, FALSE otherwise.
 */
int validate_entry(const char *line,char *label_name,SymbolTable *symbol_table,macro_node *macro_table,ASSEMBLER_STATE State);

#endif /* HELPERS_H */
