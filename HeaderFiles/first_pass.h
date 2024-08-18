
#ifndef CLAB_FIRST_PASS_H
#define CLAB_FIRST_PASS_H
#include "data_strct.h"
#include "asm_data.h"

/**
 * @brief Processes a line with a label, determining if it is an operation, data, or string directive.
 *
 * @param line Pointer to an AssemblyLine structure containing the parsed line data.
 * @param table Pointer to the hash table of labels.
 * @param IC Pointer to the instruction counter.
 * @param DC Pointer to the data counter.
 * @param ad Pointer to the AssemblyData structure.
 */
void process_label_line(AssemblyLine *line, HashTable *table, int *IC, int *DC, AssemblyData *ad);

/**
 * @brief Processes a labeled operation, updates the label table, and transforms the line into binary format.
 *
 * @param line Pointer to an AssemblyLine structure containing the labeled operation.
 * @param table Pointer to the hash table of labels.
 * @param IC Pointer to the instruction counter.
 * @param ad Pointer to the AssemblyData structure.
 */
void process_labeled_operation(AssemblyLine *line, HashTable *table, int *IC, AssemblyData *ad);

/**
 * @brief Starts the first pass of assembly file processing, handling labels, directives, and instructions.
 *
 * @param file_name Name of the assembly source file to process.
 * @param symbol_table Pointer to the hash table of symbols.
 * @param ad Pointer to the AssemblyData structure.
 *
 * @return Status code: 0 for success, or -1 if an error occurs.
 */
int start_first_pass(char *file_name, HashTable *symbol_table, AssemblyData *ad);

/**
 * @brief Processes an extern directive by updating the symbol table.
 *
 * @param label_name Name of the label specified in the extern directive.
 * @param symbol_table Pointer to the hash table of symbols.
 * @param line_num Line number of the extern directive in the source file.
 */
void process_extern(char *label_name, HashTable *symbol_table, int line_num);

/**
 * @brief Processes an entry directive by updating or inserting the label into the symbol table.
 *
 * @param label_name Name of the label specified in the entry directive.
 * @param table Pointer to the hash table of symbols.
 * @param line_num Line number of the entry directive in the source file.
 */
void process_entry(char *label_name, HashTable *table, int line_num);

/**
 * @brief Checks if a line contains a valid .entry or .extern directive.
 *
 * @param str Line of text to check.
 * @param line_num Line number of the directive in the source file.
 *
 * @return 1 if the line is a valid .entry or .extern directive, otherwise 0.
 */
int is_valid_entry_or_extern_line(char *str, int line_num);


#endif
