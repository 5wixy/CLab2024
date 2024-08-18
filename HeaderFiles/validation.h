
#ifndef CLAB_VALIDATION_H
#include "hash_table.h"
#include "globals.h"
#include "data_strct.h"

#define CLAB_VALIDATION_H
typedef struct op_code {
    char *opcode;    /* The opcode corresponding to the operation */
    int args_num;     /* The number of arguments for the operation */
} op_code;
extern op_code op_arr[];
extern char reg_arr[REG_ARR_SIZE][3];

/**
 * @brief Checks if a given macro/label name is valid.
 *
 * Check that the name is not a reserved directive name, operator name, or register name.
 *
 * @param macro_name The macro/label name to check.
 * @return 1 if the name is valid, 0 otherwise.
 */
int is_valid_name(char *macro_name);

/**
 * @brief Checks if a given macro/label name is a valid operator name.
 *
 * There are 16 valid operators as defined in the op_arr array and the booklet.
 *
 * @param macro_name The macro/label name to check.
 * @return 1 if the name is a valid operator name, 0 otherwise.
 */
int is_op_name(char *macro_name);

/**
 * @brief Checks if a given macro/label name is a valid register name.
 *
 * A valid register name is one of the following: r0, r1, r2, r3, r4, r5, r6, r7.
 *
 * @param macro_name The macro/label name to check.
 * @return 1 if the name is a valid register name, 0 otherwise.
 */
int is_reg_name(char *macro_name);

/**
 * @brief Checks if a given macro/label name is a valid data type name.
 *
 * A valid data type name is one that is not a reserved directive name.
 *
 * @param macro_name The macro/label name to check.
 * @return 1 if the name is a valid data type name, 0 otherwise.
 */
int is_type_data_type_name(char *macro_name);

/**
 * @brief Checks if a given macro/label name is of valid length.
 * @param name
 * @return
 */
int is_name_too_long(char *name);

/**
 * @brief Checks if a given line of assembly code is a data or string directive.
 *
 * @param line The line of assembly code to check. Expected to be a valid, null-terminated C string.
 * @return a pointer to the string "data" if the line is a `.data` directive, "string" if the line is a `.string` directive, or NULL if the line is neither.
 */
char* is_data_or_string(char *line);

/**
 * @brief Checks if a given label is valid.
 *
 * A valid label must:
 * - Start with a letter.
 * - Contain only letters and digits.
 * - Not exceed the maximum label length.
 *
 * @param label The label to check.
 * @return 1 if the label is valid, 0 otherwise.
 */
int is_valid_command(AssemblyLine asm_line, int line_num);


/**
 * @brief Checks if a given label is valid.
 *
 * A valid label must:
 * - Start with a letter.
 * - Contain only letters and digits.
 * - Not exceed the maximum label length.
 *
 * @param label The label to check.
 * @return 1 if the label is valid, 0 otherwise.
 */
int is_external_label(const char *label,HashTable *table);


/**
 * @brief Checks if a line of assembly code is an `.entry` or `.extern` directive.
 *        If the line is a valid directive, the corresponding label is processed.
 *
 * @param line The line of assembly code to check. Expected to be a valid, null-terminated C string.
 * @param symbol_table A pointer to the symbol table where labels and their properties are stored.
 * @param IC A pointer to the Instruction Counter (IC), which tracks the location of instructions in memory.
 *           This is updated when processing `.entry` directives.
 * @param line_num The current line number in the assembly file, used for error reporting.
 *
 * @return 1 if the line is an `.entry` or `.extern` directive and the associated label is processed.
 *         Returns 0 if the line is not an `.entry` or `.extern` directive, or if the directive is improperly formatted.
 */
int is_entry_or_extern(const char *line, HashTable *symbol_table, int line_num);


/**
 * @param line - the line to check if it is a macro declaration
 * @return 1 if the line is an extern declaration, 0 otherwise
 */
int is_extern(const char *line);

/* @param line - the line to check if it is a macro declaration
 * @return 1 if the line is an entry declaration, 0 otherwise
 */
int is_entry(const char *line);
#endif
