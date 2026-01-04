#ifndef MEMORY_H
#define MEMORY_H

#define MAX_MEMORY_SIZE 1000 /* Maximum memory size */
#define STARTING_IC 100      /* Instructions start at address 100 */
#define STARTING_DC 0        /* Data starts at address 0 */
#define MAX_TEMP_MEMORY 200
#define STARTING_TC 0

/* Instruction and Data Memory */
/* Structure for Instruction Memory */
typedef struct {
    int memory[MAX_MEMORY_SIZE];
    int IC;  /* Next free instruction slot */
    int ICF; /*final value of IC after firstpass*/
} InstructionMemory;

/* Structure for Data Memory */
typedef struct {
    int memory[MAX_MEMORY_SIZE];
    int DC;  /* Next free data slot */
    int DCF; /*final values of DC after firstpass*/
} DataMemory;

typedef struct
{
    int memory[MAX_TEMP_MEMORY];
    int TC;
} TempMemory;


/**
 * @brief Initializes instruction and data memory structures.
 * @param inst_mem Pointer to instruction memory.
 * @param data_mem Pointer to data memory.
 */
void initialize_memory(InstructionMemory *inst_mem, DataMemory *data_mem);
/**
 * @brief Initializes temporary memory structure.
 * @param temp_mem Pointer to temporary memory.
 */
void initialize_temp_memory(TempMemory *temp_mem);

#endif /* MEMORY_H */
