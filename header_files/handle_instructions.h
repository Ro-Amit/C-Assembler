#ifndef HANDLE_INSTRUCTIONS_H
#define HANDLE_INSTRUCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "helpers.h"
#include "globals.h"

#include "../data_structures/headers/instructions_details.h"
#include "globals.h"
/* Instruction structure*/
typedef struct {
    char *instruction_name;
    int opcode;
    int funct;
    int num_operands;
    AddressingMode source_operands[4];
    AddressingMode dest_operands[4];
} Instruction;


/**
 * Stores an instruction into the instruction memory.
 *
 * @param line The instruction line to process.
 * @param instructions_details  Array of instruction details.
 * @param inst_details_index Index to write to.
 * @return 1 if stored successfully, 0 otherwise.
 */
int store_instruction(const char *line,INSTRUCTIONS_DETAILS *instructions_details, const int inst_details_index);

/**
 * Checks if the given instruction name is valid.
 *
 * @param instruction_name The name of the instruction.
 * @return 1 if valid, 0 otherwise.
 */
int is_valid_instruction_name(const char *instruction_name);


/**
 * Retrieves the expected number of operands for an instruction.
 *
 * @param instruction_name The instruction name.
 * @return The number of expected operands (0, 1, or 2), or -1 if invalid should not happen.
 */
int get_expected_num_operands(const char *instruction_name);


/**
 * Determines the addressing types of operands in an instruction.
 *
 * @param operand1_value Pointer to store the first operand value.
 * @param addressing_mode1 Pointer to store the addressing mode of the first operand.
 * @param operand2_value Pointer to store the second operand value.
 * @param addressing_mode2 Pointer to store the addressing mode of the second operand.
 * @param expected_num_operands The expected number of operands.
 * @param line The instruction line.
 * @param operand1 String to store operand 1.
 * @param operand2 String to store operand 2.
 * @return 1 if successful, 0 otherwise.
 */
int get_addressing_types(int *operand1_value, AddressingMode *addressing_mode1,
                         int *operand2_value, AddressingMode *addressing_mode2,
                         int const expected_num_operands, const char *line,
                         char *operand1, char *operand2);

/**
 * Determines the type of an operand based on the given instruction line.
 *
 * @param line The instruction line.
 * @param operand The operand string.
 * @param addressing_mode Pointer to store the detected addressing mode.
 * @return 1 if successful, 0 otherwise.
 */
int determine_operand_type(const char *line, char *operand, AddressingMode *addressing_mode);


/**
 * Processes an immediate operand.
 *
 * @param line The instruction line.
 * @param operand1_value Pointer to store the immediate operand value.
 * @param pos The position of the operand in the instruction.
 * @return 1 if successful, 0 otherwise.
 */
int process_immediate(const char *line, int *operand1_value, int pos);


/**
 * Processes a register operand.
 *
 * @param line The instruction line.
 * @param operand1_value Pointer to store the register operand value.
 * @return 1 if successful, 0 otherwise.
 */
int process_register(const char *line, int *operand1_value);


int process_label_operand(const char *line, char *operand_value, AddressingMode *addressing_mode, int pos);

/**
 * Encodes and stores an instruction during the first pass.
 *
 * @param instruction The instruction name.
 * @param expected_num_operands The expected number of operands.
 * @param operand1_value The first operand value.
 * @param addressing_mode1 The addressing mode of the first operand.
 * @param operand2_value The second operand value.
 * @param addressing_mode2 The addressing mode of the second operand.
 * @param num_of_storage_words The number of words required for storage.
 * @param inst_det Pointer to current instruction details sturct.
 * @return 1 if successful, 0 otherwise.
 */

int encode_and_store_instruction_firstpass(const char *instruction, int expected_num_operands,
                                            int operand1_value,AddressingMode addressing_mode1,
                                            int operand2_value, AddressingMode addressing_mode2,
                                            int num_of_storage_words,
                                            INST_DET *inst_det);

/**
 * Determines the number of storage words required for a given addressing mode.
 *
 * @param addressing_mode The addressing mode.
 * @return The number of storage words required.
 */
int address_mode_to_storage_words(AddressingMode addressing_mode);

/**
 * Sets bits in a given number.
 *
 * @param num The original number.
 * @param start_bit The starting bit position.
 * @param end_bit The ending bit position.
 * @param value The value to set in the bit range.
 * @return The modified number with the bits set.
 */
int set_bits(int num, int start_bit, int end_bit, int value);

/**
 * Validates whether a given label name is a valid operand.
 *
 * @param label_name The label name to validate.
 * @return 1 if valid, 0 otherwise.
 */
int validate_label_operand(const char *label_name);

/**
 * Retrieves the opcode of an instruction.
 *
 * @param instruction The instruction name.
 * @return The opcode value, or -1 if invalid.
 */
int get_opcode(const char *instruction);

/**
 * Retrieves the function code (funct) of an instruction.
 *
 * @param instruction The instruction name.
 * @return The function code, or -1 if invalid.
 */
int get_func(const char *instruction);

/**
 * Checks if the given addressing mode is allowed for a specific instruction.
 *
 * @param instruction The instruction name.
 * @param addressing_mode The addressing mode to check.
 * @param source_or_dest Specifies whether the operand is a source or destination.
 * @return 1 if allowed, 0 otherwise.
 */
int is_allowed_add_type(const char *instruction,AddressingMode addressing_mode, SOURCE_OR_DESTINATION source_or_dest);

int check_operands_types(const char *instruction,int expected_num_operands,
                        AddressingMode addressing_mode1, AddressingMode addressing_mode2);

#endif

