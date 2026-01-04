#include "../headers/symbol_table.h"
/*
 * File: symbol_table.c
 * Description: Implements operations on the symbol table used by the assembler,
 *              including creation, insertion, lookup, printing, and cleanup.
 */
/* Initializes an empty symbol table. */
void create_SymbolTable(SymbolTable* table) {
    table->head = NULL;
    table->last = NULL;
}

/* Adds a new type to an existing symbol in the table. */
void add_type_to_symbol(SymbolTable* table, const char* name, const int type)
{
    Symbol* current;
    current=findSymbol(table,name);
    current->types|=type;

}


/* Creates and returns a new symbol with the given properties. */
Symbol* createSymbol(const char* name, int address, int types) {
    Symbol* newSymbol = (Symbol*)malloc(sizeof(Symbol));
    if (!newSymbol) {
        printf("|-->Error: Memory allocation failed for symbol\n");
        return NULL;
    }
    strncpy(newSymbol->name, name, MAX_LABEL_NAME);
    newSymbol->address = address;
    newSymbol->types = types;
    newSymbol->next = NULL;
    return newSymbol;
}

/* Adds a new symbol to the table if it's not already defined. */
void addSymbol(SymbolTable* table, const char* name, int address, int types) {
    Symbol* newSymbol;

    if (findSymbol(table, name)) {
        printf("Error: Duplicate symbol '%s'\n", name);
        return;
    }


    newSymbol = createSymbol(name, address, types);
    if (!newSymbol) return;

    if (table->head == NULL) {
        table->head = newSymbol;
        table->last = newSymbol;
    } else {
        table->last->next = newSymbol;
        table->last = newSymbol;
    }
}

/* Searches for a symbol in the table by name. */
Symbol* findSymbol(const SymbolTable* table, const char* name) {
    Symbol* current = table->head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current; /* found*/
        }
        current = current->next;
    }
    return NULL; /* not found*/
}

/* Frees all memory used by the symbol table. */
void freeSymbolTable(SymbolTable* table) {
    Symbol* current = table->head;
    Symbol* temp;
    while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp);
    }
    table->head = NULL;
    table->last = NULL;
}

/* Prints all symbols in the symbol table with their details. */
void printSymbolTable(SymbolTable* table) {
    Symbol* current = table->head;
    printf("\nSymbol Table:\n");
    printf("--------------------------------------\n");
    printf("| %-10s | %-6s | %-12s |\n", "Label", "Address", "Types");
    printf("--------------------------------------\n");

    while (current != NULL) {
        printf("| %-10s | %-6d | ", current->name, current->address);

        if (current->types & CODE) printf("CODE ");
        if (current->types & DATA) printf("DATA ");
        if (current->types & ENTRY) printf("ENTRY ");
        if (current->types & EXTERNAL) printf("EXTERNAL ");
        printf("|\n");

        current = current->next;
    }
    printf("--------------------------------------\n");
}

/* Returns the number of symbols in the table. */
int SymbolTable_size(SymbolTable* table)
{
    Symbol* current = table->head;
    int size = 0;
    while (current != NULL)
    {
        size++;
        current = current->next;
    }
    return size;
}




