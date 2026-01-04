#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../header_files/globals.h"

typedef enum{
    CODE=1,
    DATA=2,
    ENTRY=4,
    EXTERNAL=8
  } SymbolType;

typedef struct Symbol{
    char name[MAX_LABEL_NAME+1];
    int address;
    int types;
    struct Symbol *next;
} Symbol;

/*
 * File: symbol_table.h
 * Description: Declares the symbol table structure and functions used to
 *              create, update, search, print, and free symbol
 */
typedef struct SymbolTable{
    Symbol *head;
    Symbol *last;
} SymbolTable;


/**
 * @brief Initializes an empty symbol table.
 * @param table Pointer to the SymbolTable to initialize.
 */
void create_SymbolTable(SymbolTable* table);
/**
 * @brief Creates a new symbol.
 * @param name Label name.
 * @param address Memory address.
 * @param attributes Bitwise combination of SymbolType values.
 * @return Pointer to the created Symbol.
 */
Symbol* createSymbol(const char *name, int address, int attributes);

/**
 * @brief Adds a symbol to the symbol table.
 * @param table Pointer to the symbol table.
 * @param name Symbol name.
 * @param address Symbol address.
 * @param attributes Symbol attributes (bitwise SymbolType).
 */
void addSymbol(SymbolTable *table, const char *name, int address, int attributes);

/**
 * @brief Searches for a symbol by name.
 * @param table Pointer to the symbol table.
 * @param name Symbol name to find.
 * @return Pointer to the symbol or NULL if not found.
 */
Symbol* findSymbol(const SymbolTable *table, const char *name);

/**
 * @brief Prints all symbols in the symbol table (for debugging).
 * @param table Pointer to the symbol table.
 */
void printSymbolTable(SymbolTable *table);

/**
 * @brief Frees memory allocated for the symbol table.
 * @param table Pointer to the symbol table.
 */
void freeSymbolTable(SymbolTable *table);
/**
 * @brief Adds a new type to an existing symbol.
 * @param table Pointer to the symbol table.
 * @param name Symbol name.
 * @param type Type to add (SymbolType).
 */
void add_type_to_symbol(SymbolTable* table, const char* name, const int type);

#endif
