#include "../headers/instructions_details.h"
/*
* File: instructions_details.c
* Description: Implements initialization functions for instruction details
*              and external usage tracking structures used in the assembler.
*/

/* Allocates and initializes instruction details memory. */
INSTRUCTIONS_DETAILS *initialize_instructions_details(const int Size)
{
    int i;
    INSTRUCTIONS_DETAILS *instructions_details = malloc(sizeof(INSTRUCTIONS_DETAILS));
    if (instructions_details == NULL) {
        printf("|-->Error: allocating memory for instructions_details struct\n");
        return NULL;
    }

    instructions_details->Size = Size;
    instructions_details->memory = malloc(sizeof(INST_DET) * Size);
    if (instructions_details->memory == NULL) {
        printf("|-->Error: allocating memory for instructions array\n");
        free(instructions_details);
        return NULL;
    }

    for (i = 0; i < Size; i++) {
        strcpy(instructions_details->memory[i].instruction, "EMPTY");
        instructions_details->memory[i].num_memory_words = 0;
    }

    return instructions_details;
}

/* Allocates and initializes external usage tracking memory. */
void* initialize_external_usages(const int Size)
{
    EXTERNAL_USAGES *external_usages = malloc(sizeof(EXTERNAL_USAGES));
    if (external_usages == NULL)
    {
        printf("|-->Error: allocating memory for external_usages\n");
        return NULL;
    }
    external_usages->Size = Size;
    external_usages->List = malloc(sizeof(EXTERNAL_APPEARANCE) * Size);
    if (external_usages->List == NULL)
    {
        printf("|-->Error: allocating memory for external_usages array\n");
        free(external_usages);
        return NULL;
    }

    external_usages->Index=0;
    return external_usages;
}
