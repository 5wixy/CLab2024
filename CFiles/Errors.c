#include <stdio.h>
#include "../HeaderFiles/Errors.h"


Error errors[] = {
    {ERR_MACRO_NAME_TOO_LONG, "Macro name too long"},
    {ERR_ILLEGAL_MACRO_NAME, "Macro name is illegal, cannot be a keyword"},
    {ERR_EXTRA_TEXT_IN_MACRO_DECLARATION, "Extra text in macro declaration"},
    {ERR_MACRO_MULTIPLE_DEFINITIONS, "Macro multiple definitions"},
    {ERR_MACRO_NOT_FOUND, "Macro not found"},
    {ERR_MACRO_WITHOUT_DEFINITION, "Macro without definition"},
    {ERR_MACRO_ALREADY_EXISTS, "Macro already exists"},
    {ERR_ENDMACR_WITH_EXTRA_TEXT, "ENDMACR with extra text"},
    {ERR_INVALID_OPCODE, "Invalid opcode"},
    {ERR_MISSING_COMMA, "Missing comma between operands"},
    {ERR_INVALID_SYNTAX, "Invalid syntax"},
    {ERR_UNDEFINED_LABEL, "Undefined label"},
    {ERR_LABEL_NAME_TOO_LONG, "Label name too long"},
    {ERR_LABEL_ALREADY_EXISTS, "Label already exists"},
    {ERR_LABEL_IS_MACRO, "Label cannot be a macro name"},
    {ERR_MEMORY_ALLOCATION_FAILED, "Memory allocation failed"},
    {ERR_FILE_NOT_FOUND, "File not found"},
    {ERR_DIRECTIVE_ERROR, "Directive error"},
    {ERR_LABEL_REDEFINITION, "Label redefinition"},
    {ERR_ILLEGAL_LABEL_NAME, "Illegal label name"},
    {ERR_LABEL_IS_EXTERN, "Cannot declare a label which is already declared as extern"},
    {ERR_CANNOT_ASSIGN_EXTERN_LABEL_AS_ENTRY, "Cannot assign an extern label as an entry label"},
    {ERR_CANNOT_ASSIGN_ENTRY_LABEL_AS_EXTERN, "Cannot assign an entry label as an extern label"},
    {ERR_MULTIPLE_COMMAS_DATA, "Multiple commas in between numbers"},
    {ERR_INVALID_DATA_INPUT, "Invalid data input"},
    {ERR_NO_STRING_QUOTES, "String must start and end with quotes"},
    {ERR_MISSING_OPERANDS, "Missing operands"},
    {ERR_EXTRA_OPERANDS, "Extra operands for this operator"},
    {ERR_INVALID_OPERAND_IMMEDIATE, "Invalid operand for immediate addressing, Must be a number"},
    {ERR_INVALID_OPERAND_POINTER, "Invalid operand for pointer addressing, Must be a register"},
    {ERR_IMMEDIATE_OPERAND_OUT_OF_RANGE, "Immediate operand out of range, Must be between -2048 and 2047"},
    {ERR_DATA_OUT_OF_RANGE, "Data out of range, Must be between -16384 and 16383"},
    {ERR_OUT_OF_MEMORY, "Out of memory, Cannot allocate more memory"},
    {ERR_MEMORY_OVERFLOW, "Memory overflow, Cannot write to memory"},
    {ERR_UNKNOWN, "Unknown error"},
    {ERR_COUNT, "Automatically keeps track of the number of error codes"}
};


void print_error(ErrorCode code,int line) {
    printf("Line %d: Error: %s\n", line, errors[code].message);
}



