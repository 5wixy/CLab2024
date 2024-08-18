
#ifndef CLAB_SECOND_PASS_H
#define CLAB_SECOND_PASS_H

#include "hash_table.h"
#include "asm_data.h"
#include "data_strct.h"

/**
 * Starts the second pass of the assembly file processing.
 * It reads the assembly file line by line, processes labels and operands, and updates the instruction counter (IC).
 * If no errors are found, it proceeds to finalize the process by creating output files.
 *
 * @param file_name The name of the assembly file being processed.
 * @param symbol_table A pointer to the symbol table containing labels and their addresses.
 * @param ad A pointer to the AssemblyData structure that contains the binary data of the program.
 * @param error_flag An integer flag indicating whether errors were encountered during the first pass.
 * @return int Returns OK if successful, or ERROR if an error occurred.
 */
int start_second_pass(char *file_name, HashTable *symbol_table, AssemblyData *ad, int error_flag);

/**
 * Finalizes the second pass by generating output files (.ob, .ent, .ext) based on the processed data.
 * It also handles the cleanup of external operands.
 *
 * @param file_name The name of the assembly file being processed.
 * @param symbol_table A pointer to the symbol table containing labels and their addresses.
 * @param ad A pointer to the AssemblyData structure that contains the binary data of the program.
 * @param extern_operands A pointer to the array of ExternOperand structures containing external operands.
 * @param extern_count The number of external operands.
 * @return int Returns OK if successful, or ERROR if an error occurred.
 */
int finalize_second_pass(char *file_name, HashTable *symbol_table, AssemblyData *ad, ExternOperand *extern_operands, int extern_count);

/**
 * Processes the operands in an assembly line, replacing labels with their corresponding addresses.
 * This function updates the instruction counter (IC) and manages external operands.
 *
 * @param line A pointer to the AssemblyLine structure representing the current line of assembly code.
 * @param symbol_table A pointer to the symbol table containing labels and their addresses.
 * @param ad A pointer to the AssemblyData structure that contains the binary data of the program.
 * @param IC A pointer to the instruction counter.
 * @param extern_operands A double pointer to the array of ExternOperand structures containing external operands.
 * @param extern_count A pointer to the number of external operands.
 * @param extern_capacity A pointer to the capacity of the external operands array.
 * @param line_num The current line number being processed.
 */
void process_operands_for_labels(AssemblyLine *line, HashTable *symbol_table, AssemblyData *ad, int *IC, ExternOperand **extern_operands, int *extern_count, int *extern_capacity, int line_num);

/**
 * Processes and updates a single operand in an assembly line by resolving any labels to their corresponding addresses.
 * This function handles the external operands and updates the instruction data.
 *
 * @param line A pointer to the AssemblyLine structure representing the current line of assembly code.
 * @param operand The operand string to be processed.
 * @param symbol_table A pointer to the symbol table containing labels and their addresses.
 * @param ad A pointer to the AssemblyData structure that contains the binary data of the program.
 * @param index The index of the operand in the instruction data.
 * @param extern_operands A double pointer to the array of ExternOperand structures containing external operands.
 * @param extern_count A pointer to the number of external operands.
 * @param extern_capacity A pointer to the capacity of the external operands array.
 * @param line_num The current line number being processed.
 */
void process_and_update_operand(char *operand, HashTable *symbol_table, AssemblyData *ad, int index, ExternOperand **extern_operands, int *extern_count, int *extern_capacity, int line_num);

/**
 * Counts the number of entry labels in the symbol table.
 *
 * @param table A pointer to the symbol table containing labels and their addresses.
 * @return int The number of entry labels.
 */
int count_entry(HashTable *table);

/**
 * Counts the number of external labels in the symbol table.
 *
 * @param table A pointer to the symbol table containing labels and their addresses.
 * @return int The number of external labels.
 */
int count_extern(HashTable *table);

/**
 * Fills the .ob file with the binary data from the AssemblyData structure.
 *
 * @param file_name The name of the .ob file to be created.
 * @param ad A pointer to the AssemblyData structure that contains the binary data of the program.
 */
void fill_ob_file(const char *file_name, AssemblyData *ad);

/**
 * Fills the .ent file with the entry labels and their addresses.
 *
 * @param file_name The name of the .ent file to be created.
 * @param table A pointer to the symbol table containing labels and their addresses.
 * @param ad A pointer to the AssemblyData structure that contains the binary data of the program.
 */
void fill_ent_file(char *file_name, HashTable *table, AssemblyData *ad);

/**
 * Fills the .ext file with the external operands and their addresses.
 *
 * @param file_name The name of the .ext file to be created.
 * @param extern_operands A pointer to the array of ExternOperand structures containing external operands.
 * @param extern_count The number of external operands.
 */
void fill_ext_file(const char *file_name, ExternOperand *extern_operands, int extern_count);

/**
 * Adds an external operand to the array of external operands, expanding the array if necessary.
 *
 * @param extern_operands A double pointer to the array of ExternOperand structures containing external operands.
 * @param extern_count A pointer to the number of external operands.
 * @param extern_capacity A pointer to the capacity of the external operands array.
 * @param operand The operand string to be added.
 * @param address The address associated with the external operand.
 */
void add_extern_operand(ExternOperand **extern_operands, int *extern_count, int *extern_capacity, const char *operand, int address);

/**
 * Frees the memory allocated for the array of external operands.
 *
 * @param extern_operands A pointer to the array of ExternOperand structures containing external operands.
 * @param extern_count The number of external operands.
 */
void free_extern_operands(ExternOperand *extern_operands, int extern_count);

#endif
