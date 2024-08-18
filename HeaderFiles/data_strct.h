#ifndef CLAB_DATA_STRCT_H
#define CLAB_DATA_STRCT_H
typedef enum {
    INSTRUCTION,
    DATA,
    STRING
} LineType;

typedef struct {
    char *label;
    int opcode;
    char *src_operand;
    char *dest_operand;
    LineType type;
    char **data_array;
    int data_count;
} AssemblyLine;
typedef struct {
    char *label_name;
    int address;
} ExternOperand;


/**
 * @brief Frees the memory allocated for an AssemblyLine structure.
 *
 * @param line Pointer to the AssemblyLine structure to be freed.
 */
void free_assembly_line(AssemblyLine *line);

/**
 * @brief Parses a string representing an assembly line into an AssemblyLine structure.
 *
 * This function extracts the label, opcode, source operand, and destination operand from
 * the provided string. It also determines the type of the line and extracts data or string
 * directives if applicable. If there is only one operand, the operand is treated as the
 * destination operand.
 *
 * @param line_str String representing the assembly line to be parsed.
 * @param line Pointer to the AssemblyLine structure to be filled with parsed data.
 *
 * @return Status code: 0 for success, or a non-zero value if an error occurs.
 */
int parse_assembly_line(const char *line_str, AssemblyLine *line);

#endif

