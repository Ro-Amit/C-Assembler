#ifndef MACRO_LIST_H
#define MACRO_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../header_files/globals.h"

#define MAX_MACRO_NAME_LENGTH 31
#define MAX_MACRO_CONTENT_LENGTH 200


typedef struct macro_node {
    char name[MAX_MACRO_NAME_LENGTH];         /* Macro name */
    char content[MAX_MACRO_CONTENT_LENGTH];      /* Macro content */
    struct macro_node *next;  /* Pointer to the next node in the linked list */
} macro_node;

/**
 * @brief Creates a new macro node.
 * @param name Macro name.
 * @param content Macro content.
 * @return Pointer to the new node.
 */
macro_node *create_node(const char *name, const char *content);
/**
 * @brief Searches for a macro by name in the list.
 * @param head Head of the macro list.
 * @param name Name of the macro to find.
 * @return Pointer to the found node or NULL.
 */
macro_node *search_list(macro_node *head, const char *name);
/**
 * @brief Inserts a new macro into the list.
 * @param head Pointer to the list head.
 * @param name Macro name.
 * @param content Macro content.
 */
void insert_node(macro_node **head, const char *name, const char *content);
/**
 * @brief Frees a single node.
 * @param node1 Pointer to the node to free.
 */
void free_node(macro_node *node1);
/**
 * @brief Frees the entire macro list.
 * @param head Pointer to the list head.
 */
void free_list(macro_node *head);
/**
 * @brief Prints the macro list (for debugging).
 * @param head Pointer to the list head.
 */
void print_list(const macro_node *head);
#endif
