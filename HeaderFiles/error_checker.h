#include "data_strct.h"
#include "hash_table.h"

#ifndef CLAB_ERROR_CHECKER_H
#define CLAB_ERROR_CHECKER_H


/**
 * @brief Enum for status codes.
 *
 */
typedef enum {
    OK = 0,
    ERROR = 1
} StatusCode;

/**
 * @brief Checks if the given line in the assembly code contains any errors.
 *
 * @param asm_line The parsed assembly line as an AssemblyLine struct.
 * @param line The original assembly line as a string.
 * @param line_num The line number in the source file.
 * @param table The hash table containing labels and macros.
 * @return int Returns OK if no errors are found, otherwise returns ERROR.
 */
int check_line_error(AssemblyLine asm_line, char *line, int line_num, HashTable table);

/**
 * @brief Validates the syntax and existence of a label.
 *
 * @param label The label string to validate.
 * @param table The hash table containing existing labels.
 * @param line_num The line number in the source file.
 * @return int Returns OK if the label is valid, otherwise returns ERROR.
 */
int check_label(char *label, HashTable table, int line_num);

/**
 * @brief Validates the contents of an assembly line, including label, directive, and operands.
 *
 * @param asm_line The parsed assembly line as an AssemblyLine struct.
 * @param line The original assembly line as a string.
 * @param line_num The line number in the source file.
 * @param table The hash table containing labels and macros.
 * @return int Returns OK if the line is valid, otherwise returns ERROR.
 */
int check_assembly_line(AssemblyLine asm_line, char *line, int line_num, HashTable table);

/**
 * @brief Checks if the given string is a valid .string directive.
 *
 * @param string The string directive to validate.
 * @param line_num The line number in the source file.
 * @return int Returns OK if the string directive is valid, otherwise returns ERROR.
 */
int check_string_directive(char *string, int line_num);

/**
 * @brief Validates the operands in the assembly line.
 *
 * @param asm_line The parsed assembly line as an AssemblyLine struct.
 * @param line_num The line number in the source file.
 * @return int Returns OK if the operands are valid, otherwise returns ERROR.
 */
int check_operands(AssemblyLine asm_line, int line_num);

/**
 * @brief Checks if operands are correctly separated by commas.
 *
 * @param asm_line The parsed assembly line as an AssemblyLine struct.
 * @param line_num The line number in the source file.
 * @return int Returns OK if the comma usage is correct, otherwise returns ERROR.
 */
int check_comma_operands(AssemblyLine asm_line, int line_num);

/**
 * @brief Validates the number of operands for the given opcode.
 *
 * @param asm_line The parsed assembly line as an AssemblyLine struct.
 * @param line_num The line number in the source file.
 * @return int Returns OK if the operand count is correct, otherwise returns ERROR.
 */
int check_operand_amount(AssemblyLine asm_line, int line_num);

/**
 * @brief Validates the data in the .data directive.
 *
 * @param line The data directive line to validate.
 * @param line_num The line number in the source file.
 * @return int Returns OK if the data directive is valid, otherwise returns ERROR.
 */
int check_data_directive(char *line, int line_num);

/**
 * @brief Checks if the given string is a valid integer.
 *
 * @param str The string to validate as an integer.
 * @return int Returns 1 if the string is a valid integer, otherwise returns 0.
 */
int check_valid_integer(char *str);

/**
 * @brief Checks if the given value is within the valid range for immediate operands.
 *
 * @param value The integer value to check.
 * @return int Returns 1 if the value is within range, otherwise returns 0.
 */
int is_in_range(long value);

/**
 * @brief Validates if the operands in the assembly line are correctly formed.
 *
 * @param asm_line The parsed assembly line as an AssemblyLine struct.
 * @param line_num The line number in the source file.
 * @return int Returns OK if the operands are valid, otherwise returns ERROR.
 */
int is_valid_operand(AssemblyLine asm_line, int line_num);

/**
 * @brief Validates if an immediate operand is correctly formed and within the allowed range.
 *
 * @param operand The operand string to validate.
 * @param line_num The line number in the source file.
 * @return int Returns 1 if the operand is valid, otherwise returns 0.
 */
int check_valid_imm_operand(char *operand, int line_num);

#endif
