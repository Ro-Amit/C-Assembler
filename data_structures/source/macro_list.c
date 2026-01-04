#include "../headers/macro_list.h"
/*
* File: instructions_details.c
* Description: Implements initialization functions for instruction details
*              and external usage tracking structures used in the assembler.
*/

/*==========  Linked list functions =========*/

/* Creates a new macro node with the given name and content. */
macro_node *create_node(const char *name, const char *content) {
    macro_node *new_node = (macro_node *)malloc(sizeof(macro_node));
    if (!new_node) {
        printf("|-->Error: malloc failed for macro\n");
        return NULL;
    }

    /* Copy the macro name and content into the struct */
    strncpy(new_node->name, name, MAX_MACRO_NAME_LENGTH - 1);
    new_node->name[MAX_MACRO_NAME_LENGTH - 1] = '\0';  /* Ensure null-termination */

    strncpy(new_node->content, content, MAX_MACRO_CONTENT_LENGTH - 1);
    new_node->content[MAX_MACRO_CONTENT_LENGTH - 1] = '\0';  /* Ensure null-termination */

    new_node->next = NULL;

    return new_node;
}

/* Searches the macro list for a macro by name. */
macro_node *search_list(macro_node *head, const char *name) {
    while (head) {
        if (strcmp(head->name, name) == 0) {
            return head;
        }
        head = head->next;
    }
    return NULL; /* Not found */
}

/* Inserts a new macro into the list if it doesn't already exist. */
void insert_node(macro_node **head, const char *name, const char *content) {
    macro_node *new_node;

    /* check if the macro already exists */
    if (search_list(*head, name)) {
        printf("|-->Error: Macro '%s' already exists. Skipping insertion.\n", name);
        return;
    }

    /*create the new node */
    new_node = create_node(name, content);
    if (!new_node) {
        return; /* Handle memory allocation failure */
    }

    /*insert at the beginning */
    new_node->next = *head;
    *head = new_node;
}

/* Frees a single macro node. */
void free_node(macro_node *node1) {
    if (node1) {
        free(node1);  /* Only free the struct itself */
    }
}

/* Frees all nodes in the macro list. */
void free_list(macro_node *head) {
    while (head) {
        macro_node *temp = head;
        head = head->next;
        free_node(temp);
    }
}
/* Prints all macros in the list. */
void print_list(const macro_node *head) {
    printf("\nMacro List:\n");
    printf("----------------------------\n");
    while (head) {
        printf("Name: %s\n", head->name);
        printf("Content: %s\n", head->content);
        printf("----------------------------\n");
        head = head->next;
    }
}



