#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0
#define ERROR "|-->Error: "

#define MAX_LINE_LENGTH 81
#define MAX_FILENAME_LENGTH 31
#define MAX_LABEL_NAME 31
#define BITS_IN_WORD 24

#define MAX_INSTRUCTION_NAME 6
#define MAX_OPERANDS 2
#define MAX_OPERAND_LENGTH 31
#define MIN_WORDS_PER_INSTRUCTION 1
#define SIZE_OF_INSTRUCTION_TABLE 16
#define DIRECT_REL_FIRSTPASS 0 /*defulat value for direct and relative operand in the firstpass*/
#define MAX_WORDS_PER_INSTRUCTION 3

extern int GLOBAL_LINE_NUMBER;
/*differetn addresing modes for operands*/
typedef enum {
    EOL = -5,       /*signal end of list*/
    NO_TYPE = -2,      /*no operand allowed*/
    IMMEDIATE = 0,  /* numbers #-15 */
    DIRECT = 1,     /*labels*/
    RELATIVE = 2,   /*relative &next*/
    REGISTER = 3    /*registers r2 */
} AddressingMode;
typedef enum
{
    SOURCE =1,
    DESTINATION=2
} SOURCE_OR_DESTINATION;

typedef enum
{
    A=4, /*100*/
    R=2, /*010*/
    E=1 /*001*/
} A_R_E;

typedef enum
{
    PREPROCESS,
    FIRSTPASS,
    SECONDPASS
} ASSEMBLER_STATE;

#endif
