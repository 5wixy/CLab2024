#ifndef PREPROC_H
#define PREPROC_H

#include "hash_table.h"
/**
 * @brief Checks if a line declares a macro.
 *
 * This function examines the provided line to determine if it starts with
 * the macro declaration keyword "macr" followed by optional whitespace.
 *
 * @param line The line of text to be checked.
 * @return 1 if the line declares a macro, 0 otherwise.
 */
int is_macro_declaration(char *line);

/**
 * @brief Opens and reads a file to process macro declarations.
 *
 * This function opens the specified file and processes each line to check
 * for macro declarations. If a macro declaration is found, it invokes
 * the `process_macro` function to handle it.
 *
 * @param file_name The name of the file to be opened.
 * @param table A pointer to the hash table used for storing macros.
 * @return 0 if successful, 1 if an error occurred.
 */
int open_read_file(char *file_name, HashTable *table);

/**
 * @brief Processes a macro declaration from a line.
 *
 * This function processes the macro declaration line, extracts the macro
 * name and content, and stores it in the provided hash table.
 *
 * @param fp A pointer to the file stream.
 * @param str The line containing the macro declaration.
 * @param line_count The line number of the macro declaration.
 * @param table A pointer to the hash table used for storing macros.
 * @return 0 if successful, 1 if an error occurred.
 */
int process_macro(FILE *fp, char *str, int line_count, HashTable *table);

/**
 * @brief Saves the content of a macro.
 *
 * This function reads the macro content from the file until the "endmacr"
 * keyword is found and returns it as a dynamically allocated string.
 *
 * @param fp A pointer to the file stream.
 * @param pos A pointer to the file position indicator.
 * @param line_count A pointer to the current line number.
 * @return A dynamically allocated string containing the macro content,
 *         or NULL if an error occurred.
 */
char *save_macro_content(FILE *fp, fpos_t *pos, int *line_count);

/**
 * @brief Removes macro declarations from a file.
 *
 * This function reads an input file, skips lines within macro declarations,
 * and writes the remaining lines to the output file.
 *
 * @param original A pointer to the input file stream.
 * @param copied A pointer to the output file stream.
 */
void remove_macros(FILE *original, FILE *copied);

/**
 * @brief Inserts macros into a file based on macro definitions.
 *
 * This function reads the source file, replaces macro tokens with their
 * corresponding definitions from the hash table, and writes the result to
 * the destination file.
 *
 * @param source_file The name of the source file containing macro tokens.
 * @param destination_file The name of the destination file where macros
 *        are expanded.
 * @param table A pointer to the hash table containing macro definitions.
 */
void insert_macros(const char* source_file, const char* destination_file, HashTable* table);

/**
 * @brief Expands macros in a file.
 *
 * This function coordinates the macro expansion process, including creating
 * temporary files, processing macro declarations, and inserting macro
 * definitions into the output file.
 *
 * @param file_name The name of the file to be processed for macro expansion.
 * @param table A pointer to the hash table containing macro definitions.
 * @return 0 if successful, 1 if an error occurred.
 */
int expand_macro(char file_name[], HashTable *table);


#endif