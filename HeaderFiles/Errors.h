#ifndef CLAB_ERRORS_H
#define CLAB_ERRORS_H
typedef enum {
    /* Macro Errors */
    ERR_MACRO_NAME_TOO_LONG,
    ERR_ILLEGAL_MACRO_NAME,
    ERR_EXTRA_TEXT_IN_MACRO_DECLARATION,
    ERR_MACRO_MULTIPLE_DEFINITIONS,
    ERR_MACRO_NOT_FOUND,
    ERR_MACRO_WITHOUT_DEFINITION,
    ERR_MACRO_ALREADY_EXISTS,
    ERR_ENDMACR_WITH_EXTRA_TEXT,




    ERR_INVALID_OPCODE,
    ERR_MISSING_COMMA,
    ERR_INVALID_SYNTAX,
    ERR_UNDEFINED_LABEL,
    ERR_LABEL_NAME_TOO_LONG,
    ERR_LABEL_ALREADY_EXISTS,
    ERR_LABEL_IS_MACRO,
    ERR_MEMORY_ALLOCATION_FAILED,
    ERR_FILE_NOT_FOUND,
    ERR_DIRECTIVE_ERROR,
    ERR_LABEL_REDEFINITION,
    ERR_ILLEGAL_LABEL_NAME,
    ERR_LABEL_IS_EXTERN,
    ERR_CANNOT_ASSIGN_EXTERN_LABEL_AS_ENTRY,
    ERR_CANNOT_ASSIGN_ENTRY_LABEL_AS_EXTERN,
    ERR_MULTIPLE_COMMAS_DATA,
    ERR_INVALID_DATA_INPUT,
    ERR_NO_STRING_QUOTES,
    ERR_MISSING_OPERANDS,
    ERR_EXTRA_OPERANDS,
    ERR_INVALID_OPERAND_IMMEDIATE,
    ERR_INVALID_OPERAND_POINTER,
    ERR_IMMEDIATE_OPERAND_OUT_OF_RANGE,
    ERR_DATA_OUT_OF_RANGE,




    ERR_OUT_OF_MEMORY,
    ERR_MEMORY_OVERFLOW,
    ERR_UNKNOWN,
    ERR_COUNT
} ErrorCode;

typedef struct Error{
    ErrorCode code; /** The error code associated with the error. */
    char* message; /** A descriptive message explaining the error. */
} Error;

/**
 * @brief Prints the error message associated with the given error code.
 *
 * @param code The error code to be printed.
 * @param line The line number where the error occurred.
 */
void print_error(ErrorCode code,int line);


#endif
