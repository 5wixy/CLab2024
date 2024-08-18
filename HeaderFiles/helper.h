#ifndef CLAB_HELPER_H
#define CLAB_HELPER_H

#include "data_strct.h"
#include <stdio.h>




/**
 * @brief Copies a string of specified length from a file stream.
 *
 * @param fp Pointer to the file stream.
 * @param pos File position indicator.
 * @param str_len Length of the string to copy.
 *
 * @return A newly allocated string containing the copied text, or NULL if an error occurs.
 */
char *copy_text(FILE *fp, fpos_t *pos, int str_len);

/**
 * @brief Allocates memory and handles allocation errors.
 *
 * @param object_size Size of the object to allocate.
 * @param file Name of the file where the allocation occurred.
 * @param line Line number where the allocation occurred.
 *
 * @return Pointer to the allocated memory.
 */
void *malloc_helper(int object_size);

/**
 * @brief Frees allocated memory and sets pointer to NULL.
 *
 * @param ptr Pointer to the memory to free.
 * @param file Name of the file where the deallocation occurred.
 * @param line Line number where the deallocation occurred.
 */
void my_free(void *ptr);

/**
 * @brief Duplicates a string by allocating new memory and copying the string.
 *
 * @param src Source string to duplicate.
 *
 * @return A new string that is a duplicate of the source, or NULL if allocation fails.
 */
char *my_strdup(const char *src);

/**
 * @brief Cleans a line by removing redundant spaces.
 *
 * @param line Input line to clean.
 * @param cleaned_line Output cleaned line.
 */
void clean_line(char *line, char *cleaned_line);

/**
 * @brief Detects the addressing method from an operand string.
 *
 * @param operand Operand string.
 *
 * @return Addressing method type as an integer, or -1 if unknown.
 */
int detect_addressing_method(const char *operand);

/**
 * @brief Gets the register value given a register name.
 *
 * @param register_name Name of the register.
 *
 * @return The value of the register, or -1 if not found.
 */
int get_register_value(char *register_name);

/**
 * @brief Converts an integer to a binary string of a specified number of bits.
 *
 * @param num Integer to convert.
 * @param bits Number of bits for the binary string.
 * @param output Output string to store the binary representation.
 */
void to_binary_string(int num, int bits, char *output);

/**
 * @brief Removes trailing newline character from a string.
 *
 * @param str Input string to modify.
 */
void remove_trailing_newline(char *str);

/**
 * @brief Updates the first 12 bits of a binary string with new 12-bit data.
 *
 * @param current_binary Original binary string.
 * @param new_12_bits New 12-bit data to insert.
 */
void update_first_12_bits(char *current_binary, const char *new_12_bits);

/**
 * @brief Converts addressing method integer to a binary string representation.
 *
 * @param method Addressing method integer.
 * @param output Output string for the binary representation.
 */
void addressing_method_to_binary(int method, char *output);

/**
 * @brief Removes trailing spaces from a string.
 *
 * @param str Input string to modify.
 */
void remove_trailing_spaces(char *str);

/**
 * @brief Trims leading and trailing whitespace from a string.
 *
 * @param str Input string to trim.
 *
 * @return Pointer to the trimmed string.
 */
char *trim_whitespace(char *str);

/**
 * @brief Copies content from a source file to a destination file, optionally removing macros.
 *
 * @param source_file Name of the source file.
 * @param destination_file Name of the destination file.
 * @param remove_macros_flag Flag to indicate whether to remove macros.
 */
void copy_file_content(const char *source_file, const char *destination_file, int remove_macros_flag);



/**
 * @brief Creates a new file name by modifying the existing file name with a new ending.
 *
 * @param file_name Original file name.
 * @param ending New ending to append.
 *
 * @return Newly created file name.
 */
char *create_new_file_name(char *file_name, char *ending);

/**
 * @brief Skips leading spaces in a string.
 *
 * @param str
 */
void skip_leading_space(char **str);

/**
 * @brief Extracts a label from a line of text.
 *
 * @param line Input line containing the label.
 *
 * @return Extracted label, or NULL if no label found.
 */
char *extract_label(char *line);

/**
 * @brief Extracts an opcode from a line of text.
 *
 * @param line Input line containing the opcode.
 *
 * @return Extracted opcode, or -1 if no opcode found.
 */
int extract_opcode(char *line);

/**
 * @brief Extracts the source operand from a line of text.
 *
 * @param line Input line containing the source operand.
 *
 * @return Extracted source operand, or NULL if not found.
 */
char *extract_src_operand(const char *line);

/**
 * @brief Removes commas from a string.
 *
 * @param str Input string to modify.
 */
void remove_commas(char *str);



/**
 * @brief Matches the opcode to the corresponding opcode value.
 *
 * @param str String containing the opcode.
 *
 * @return Opcode value, or -1 if not found.
 */
int match_opcodes(char *str);



/**
 * @brief Extracts the destination operand from a line of text.
 *
 * @param line Input line containing the destination operand.
 *
 * @return Extracted destination operand, or NULL if not found.
 */
char *extract_dest_operand(const char *line);

/**
 * @brief Determines the type of line (instruction, data, or string).
 *
 * @param line Pointer to an AssemblyLine structure.
 * @param raw_line Raw line of text.
 *
 * @return Line type as an enumerated value.
 */
LineType determine_line_type(AssemblyLine *line, const char *raw_line);

/**
 * @brief Extracts string data from a .string directive in an assembly line.
 *
 * @param line Pointer to an AssemblyLine structure.
 * @param str String containing the .string directive data.
 *
 * @return Array of string data, or NULL if extraction fails.
 */
char **extract_string_data(AssemblyLine *line, const char *str);

/**
 * @brief Extracts data array from a .data directive in an assembly line.
 *
 * @param line Pointer to an AssemblyLine structure.
 * @param str String containing the .data directive data.
 * @param data_count Pointer to store the number of data elements.
 *
 * @return Array of data elements, or NULL if extraction fails.
 */
char **extract_data_array(char *str, int *data_count);
#endif
