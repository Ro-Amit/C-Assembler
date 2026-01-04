#include "../header_files/preprocess.h"
#define DEBUG_MODE 1
/*
 * File: preprocess.c
 * Description: Implements the preprocessing phase for the assembler, including
 *              macro handling, validation, and expansion.
 */

/* Processes the input file, handling macro definitions, calls, and expansions. */
int preprocess_file(char file_name[], DATA_STRUCTURES *ds) {
    FILE *fp, *new_fp;
    char line[MAX_LINE_LENGTH];
    char macro_name[MAX_MACRO_NAME_LENGTH];
    char copy_macro_name[MAX_MACRO_NAME_LENGTH];
    char *temp_macro_name;
    char *trimmed_line;
    int macro_flag = FALSE,Error_flag=FALSE;
    int valid_macro_flag = FALSE;
    char macro_content[MAX_MACRO_CONTENT_LENGTH];
    int line_count = 0;
    int started_storing_macro =FALSE;
    char source_filename[MAX_FILENAME_LENGTH];
    char after_macro_filename[MAX_FILENAME_LENGTH];
    int temp_counter = 0;

    GLOBAL_LINE_NUMBER = 0;
    sprintf(source_filename, "%s.as", file_name);
    sprintf(after_macro_filename, "%s.am", file_name);

    fp = fopen(source_filename, "r");
    new_fp = fopen(after_macro_filename, "w");

    if (fp == NULL)
    {
        printf("Error opening file %s\n", source_filename);
        return 0;
    }
    if (new_fp == NULL)
    {
        printf("Error opening file %s\n", after_macro_filename);
        return 0;
    }

    while (fgets(line, MAX_LINE_LENGTH, fp)) {
        temp_counter++;
        GLOBAL_LINE_NUMBER = line_count+1;

        if (strlen(line)==MAX_LINE_LENGTH-1 && line[strlen(line)-1]!='\n')
        {
            printf(".as %d |-->Error: line too long\n",temp_counter);
            Error_flag = TRUE;
            continue;
        }
        if (*line == '\0' || *line == ';') {
            continue;
        }

        trimmed_line = remove_leading_spaces(line);
        if (*trimmed_line == '\0' || *trimmed_line == ';') {
            continue;
        }
        /*if label is defined skip line*/
        if (is_label_def(trimmed_line,macro_name,PREPROCESS))
        {
            fputs(trimmed_line, new_fp);
            line_count++;
            continue;
        }
        /*increment the global line number*/

        /*storing macro until mcro end*/
        if (macro_flag == TRUE) {
            if (is_end_macro(trimmed_line)) {
                trimmed_line+=strlen("mcroend");
                while (isspace(*trimmed_line)) trimmed_line++;
                if (!(*trimmed_line=='\n' || *trimmed_line=='\0'))
                {
                    printf(".as %d|-->Error: Characters after mcroend.\n",temp_counter);
                    Error_flag = TRUE;
                    valid_macro_flag = FALSE;
                }
                if (valid_macro_flag == TRUE && Error_flag == FALSE)
                {
                    insert_node(&(ds->macro_table), copy_macro_name, macro_content);
                    /*printf("inserting macro: %s",copy_macro_name);*/
                }

                macro_flag = FALSE;
                valid_macro_flag = FALSE;
                started_storing_macro = FALSE;
                macro_content[0] = '\0';
                continue;
            }

            if (started_storing_macro == FALSE) {
                macro_content[0] = '\0';
                started_storing_macro = TRUE;
            }

            if (strlen(macro_content) + strlen(trimmed_line) + 2 < MAX_MACRO_CONTENT_LENGTH) {
                strcat(macro_content, trimmed_line);
            } else {
                printf(".as %d |-->Error: Macro '%s' is too large.\n", temp_counter, macro_name);
                Error_flag = TRUE;
            }
            continue;
        }

        /*check if a macro is being called*/
        if (is_macro_call(trimmed_line, ds->macro_table)) {
            temp_macro_name = extract_macro_name(trimmed_line, FALSE);

            if (temp_macro_name) {
                line_count+=expand_macro(new_fp, temp_macro_name, ds->macro_table);
                free(temp_macro_name);
            }
            continue;
        }
        /**/
        if (is_macro_def(trimmed_line, macro_name))
        {

            temp_macro_name = extract_macro_name(line + strlen("mcro "), TRUE);
            if (temp_macro_name==NULL)
                continue;
            strcpy(macro_name, temp_macro_name);
            strcpy(copy_macro_name,macro_name);
            free(temp_macro_name);
            trimmed_line+=strlen("macro ")+strlen(macro_name);
            while (isspace(*trimmed_line))
                trimmed_line++;
            if (!(*trimmed_line=='\n' || *trimmed_line=='\0'))
            {
                printf(".as %d|-->Error: Characters after mcro definition.\n",temp_counter);
                Error_flag = TRUE;
                valid_macro_flag = FALSE;
                macro_flag = TRUE;
                continue;
            }
            valid_macro_flag = TRUE;
            macro_flag = TRUE;
        }
        else
        {
            fputs(trimmed_line, new_fp);
            line_count++;
        }
    }
    fclose(fp);
    fclose(new_fp);
    /*print_list(ds->macro_table);*/

    if (Error_flag == TRUE)
        return 0;

    return line_count;
}

/* Extracts a macro name from a line and validates its format. */
char *extract_macro_name(const char *line, const int allow_to_check_mcro_name) {
    char temp[MAX_MACRO_NAME_LENGTH];
    char *macro_name;
    int i;

    if (sscanf(line, "%s", temp) != 1) {
        return NULL;
    }


    if (allow_to_check_mcro_name == TRUE) {

        if (strlen(temp) >= MAX_MACRO_NAME_LENGTH) {
            printf("|-->Error: Macro name '%s' is too long.\n", temp);
            return NULL;
        }

        for (i = 0; temp[i] != '\0'; i++) {
            if (!isalnum(temp[i])) {
                printf("|-->Error: Macro name '%s' contains invalid characters.\n", temp);
                return NULL;
            }
        }
        if (is_reserved_word(temp))
        {
            printf("|-->Error: Macro name '%s' is reserved word.\n", temp);
            return NULL;
        }
    }

    macro_name = (char *)malloc(strlen(temp) + 1);
    if (!macro_name) {
        printf("|-->Error: Memory allocation failed.\n");
        return NULL;
    }

    strcpy(macro_name, temp);
    return macro_name;
}

/* Checks if a line defines a new macro. */
int is_macro_def(const char *line, char *macro_name) {
    return strncmp(line, "mcro ",strlen("mcro "))==0;
}

/* Checks if a line calls an existing macro. */
int is_macro_call(const char *line, macro_node *head) {
    char *macro_name = extract_macro_name(line, FALSE);
    int found = 0;
    if (macro_name) {
        found = search_list(head, macro_name) != NULL;
        free(macro_name);
    }
    return found;
}

/* Expands a macro and writes its content to the output file. */
int expand_macro(FILE *fp, const char *macro_name, macro_node *head) {
    int count = 0;
    const char *p;
    const macro_node *curr = search_list(head, macro_name);
    if (curr == NULL) {
        printf("%d |-->Error: macro '%s' to expand not found\n", GLOBAL_LINE_NUMBER, macro_name);
        return -1;
    }

    fputs(curr->content, fp);
    for (p = curr->content; *p != '\0'; p++) {
        if (*p == '\n') count++;
    }

    return count;
}

/* Checks if a line indicates the end of a macro definition. */
int is_end_macro(const char *line) {
    return strncmp(line, "mcroend", strlen("mcroend")) == 0;
}
