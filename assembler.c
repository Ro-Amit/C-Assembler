#include "assembler.h"
/*
 * File: assembler.c
 * Description: Main entry point of the assembler program. This file handles:
 *              preprocessing, first pass, second pass, and the creation of output files
 *              for one or more assembly source files provided via command-line arguments.
 */
int main(const int argc, char *argv[]) {
    int Size,Result, file_i;
    char source_filename[MAX_FILENAME_LENGTH];
    char test_filename[MAX_FILENAME_LENGTH];
    DATA_STRUCTURES data_structures;
    FILE *test_fp;

    /* Require at least one input file */
    if (argc < 2) {
        printf("Error: Please enter at least one file name as argument\n");
        return 1;
    }

    for (file_i = 1; file_i < argc; file_i++) {
        printf("\n-------- Assembling file: %s --------\n\n", argv[file_i]);

        strncpy(source_filename, argv[file_i], MAX_FILENAME_LENGTH);
        source_filename[MAX_FILENAME_LENGTH - 1] = '\0';
        sprintf(test_filename, "%s.as", source_filename);
        test_fp = fopen(test_filename, "r");
        if (test_fp == NULL) {
            printf("Error: Unable to open file: %s\n", test_filename);
            continue;
        }
        fclose(test_fp);

        Result=init_data_structures(&data_structures, PREPROCESS, 0);
        if (!Result)
        {
            printf("Error: initializing data_structures for: %s\n",source_filename);
            continue;
        }

        printf("Running Preprocessor...\n");
        Size = preprocess_file(source_filename, &data_structures);
        if (Size == 0) {
            printf("Error: Preprocessing failed for file: %s\n", source_filename);
            free_data_structures(&data_structures);
            continue;
        }
        Result = init_data_structures(&data_structures, FIRSTPASS, Size);
        if (!Result) {
            printf("Error: Initialization failed after preprocessing.\n");
            free_data_structures(&data_structures);
            continue;
        }

        printf("Running Firstpass...\n");
        Result = firstpass_main(source_filename, &data_structures);
        if (Result==FALSE) {
            printf("Error: First pass failed for file: %s\n", source_filename);
            free_data_structures(&data_structures);
            continue;
        }
        printf("Running Second Pass...\n");
        Result = secondpass_main(source_filename, &data_structures);
        if (Result == FALSE) {
            printf("Error: Second pass failed for file: %s\n", source_filename);
            free_data_structures(&data_structures);
            continue;
        }
        /*printSymbolTable(&(data_structures.symbol_table));*/

        printf("Creating output files...\n");
        Result=create_output_files(source_filename, &data_structures);
        if (Result == FALSE){
            printf("Error: Creating output failed for file: %s\n", source_filename);
            free_data_structures(&data_structures);
            continue;
        }

        printf("\n------ Successfully finished assembling %s ------\n", source_filename);
        free_data_structures(&data_structures);
    }

    return 0;
}
