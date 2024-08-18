#ifndef CLAB_AM_HANDLER_H
#define CLAB_AM_HANDLER_H
#include "globals.h"
#include "asm_data.h"
#include "data_strct.h"
#include "hash_table.h"





/**
 * @brief Increments the instruction counter (IC) based on the addressing methods of operands.
 *
 * This function calculates the increment amount based on the addressing methods of the source
 * and destination operands and applies this increment to the instruction counter.
 *
 * @param line The AssemblyLine structure containing the operands.
 * @param IC Pointer to the instruction counter to be incremented.
 */
void increment_IC(AssemblyLine line, int *IC);

/**
 * @brief Calculates the increment amount based on addressing methods of operands.
 *
 * This function determines the increment amount based on whether the source and destination
 * operands use register or other addressing methods.
 *
 * @param source The source operand string.
 * @param dest The destination operand string.
 *
 * @return The increment amount.
 */
int calculate_increment(const char *source, const char *dest);

/**
 * @brief Applies the calculated increment to the instruction counter.
 *
 * This function increments the instruction counter by the specified amount.
 *
 * @param IC Pointer to the instruction counter to be updated.
 * @param increment The amount to increment the instruction counter by.
 */
void apply_increment(int *IC, int increment);

/**
 * @brief Processes an operand based on its addressing method and updates the output binary string.
 *
 * @param operand The operand string.
 * @param addressing_method The addressing method for the operand.
 * @param output The output binary string to be updated.
 * @param num_words Pointer to the number of words processed.
 * @param position The position in the output string to start processing.
 * @param table Pointer to the hash table for label resolution.
 */
void process_operand(char *operand, int addressing_method, char *output, int position, HashTable *table);

/**
 * @brief Processes a register operand and updates the output binary string.
 *
 * @param operand The register operand string.
 * @param output The output binary string to be updated.
 * @param position The position in the output string to start processing.
 */
void process_register_operand(char *operand, char *output, int position);

/**
 * @brief Processes a direct operand and updates the output binary string.
 *
 * @param operand The direct operand string.
 * @param output The output binary string to be updated.
 * @param table Pointer to the hash table for label resolution.
 */
void process_direct_operand(const char *operand, char *output, HashTable *table);

/**
 * @brief Processes an immediate operand and updates the output binary string.
 *
 * @param operand The immediate operand string.
 * @param output The output binary string to be updated.
 */
void process_immediate_operand(const char *operand, char *output);

/**
 * @brief Creates the first binary word of an operation based on opcode and addressing methods.
 *
 * @param opcode The opcode for the operation.
 * @param src_method The addressing method for the source operand.
 * @param dest_method The addressing method for the destination operand.
 * @param output The output binary string to be updated.
 */
void create_first_word(int opcode, int src_method, int dest_method, char *output);

/**
 * @brief Swaps the source and destination operands in an AssemblyLine structure if necessary.
 *
 * @param line Pointer to the AssemblyLine structure to be updated.
 */
void swap_src_dest(AssemblyLine *line);

/**
 * @brief Transforms an AssemblyLine into binary representation and updates the binary output.
 *
 * This function processes the opcode, source operand, and destination operand of an assembly line
 * and updates the binary output accordingly.
 *
 * @param line Pointer to the AssemblyLine structure to be transformed.
 * @param binary_output Array to store the resulting binary words.
 * @param num_words Pointer to the number of binary words produced.
 * @param table Pointer to the hash table for label resolution.
 */
void transform_to_binary(AssemblyLine *line, char **binary_output, int *num_words, HashTable *table);


/**
 * @brief Retrieves the address of a label from the hash table.
 *
 * @param label The label whose address is to be retrieved.
 * @param table Pointer to the hash table containing labels.
 *
 * @return The address of the label, or -1 if not found.
 */
int get_label_address(const char *label, HashTable *table);


/**
 * @brief Processes register operands and updates the output binary string.
 *
 * @param src_operand The source register operand string.
 * @param dest_operand The destination register operand string.
 * @param output The output binary string to be updated.
 */
void process_register_operands(char *src_operand, char *dest_operand, char *output);

/**
 * @brief Processes a .string directive and updates the assembly data and data counter.
 *
 * @param line Pointer to the AssemblyLine structure containing the .string directive.
 * @param table Pointer to the hash table for label resolution.
 * @param DC Pointer to the data counter to be updated.
 * @param ad Pointer to the AssemblyData structure to be updated.
 */
void process_string_directive(AssemblyLine *line, HashTable *table, int *DC, AssemblyData *ad);

/**
 * @brief Processes a .data directive and updates the assembly data and data counter.
 *
 * @param line Pointer to the AssemblyLine structure containing the .data directive.
 * @param table Pointer to the hash table for label resolution.
 * @param DC Pointer to the data counter to be updated.
 * @param ad Pointer to the AssemblyData structure to be updated.
 */
void process_data_directive(AssemblyLine *line, HashTable *table, int *DC, AssemblyData *ad);

/**
 * @brief Processes an operation line and updates the instruction counter and assembly data.
 *
 * @param line Pointer to the AssemblyLine structure containing the operation line.
 * @param table Pointer to the hash table for label resolution.
 * @param IC Pointer to the instruction counter to be updated.
 * @param ad Pointer to the AssemblyData structure to be updated.
 */
void process_operation_line(AssemblyLine *line, HashTable *table, int *IC, AssemblyData *ad);

#endif


