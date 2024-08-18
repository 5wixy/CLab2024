#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdatomic.h>
#include "../HeaderFiles/error_checker.h"
#include "../HeaderFiles/validation.h"
#include "../HeaderFiles/data_strct.h"
#include "../HeaderFiles/helper.h"
#include "../HeaderFiles/Errors.h"



int check_line_error(AssemblyLine asm_line, char *line, int line_num, HashTable table) {

    if(check_assembly_line(asm_line, line, line_num,table)){
        return ERROR;
    }
    return OK;


}
int check_label(char *label, HashTable table, int line_num) {

    int len;
    int i;
    HashItem *item;
    char label_copy[MAX_LABEL_NAME_LEN];

    /* Check for null or empty label */
    if (!label || !(*label)) return 0;

    len = strlen(label);

    /* Check if label length is valid and ends with a colon */
    if (len < 1 || label[len - 1] != ':') return ERROR;

    /* Ensure label length does not exceed maximum allowed */
    if (len > MAX_LABEL_NAME_LEN) {
        print_error(ERR_LABEL_NAME_TOO_LONG, line_num);
        return ERROR;
    }

    /* Copy the label (excluding the trailing colon) and null-terminate */
    strncpy(label_copy, label, len - 1);
    label_copy[len - 1] = '\0';

    /* Validate the label name */
    if (!is_valid_name(label_copy)) {
        print_error(ERR_ILLEGAL_LABEL_NAME, line_num);
        return ERROR;
    }

    /* Check if label already exists in the hash table */
    item = get_label(&table, label_copy);
    if (item != NULL) {
        /* Handle case where label is an extern label */
        if (item->data.label.type == LABEL_EXTERN) {
            print_error(ERR_LABEL_IS_EXTERN, line_num);
            return ERROR;
        }
        /* Handle case where label is a macro */
        if (item->type == TYPE_MACRO) {
            print_error(ERR_LABEL_IS_MACRO, line_num);
            return ERROR;
        } else if (item->data.label.address != 0) {
            /* Handle case where label already exists with an address */
            print_error(ERR_LABEL_ALREADY_EXISTS, line_num);
            return ERROR;
        }
    }

    /* Ensure the first character is an alphabetic letter */
    if (!isalpha((unsigned char)label[0])) return ERROR;

    /* Ensure all characters in the label (except the last colon) are alphanumeric */
    for (i = 0; i < len - 1; i++) {
        if (!isalnum((unsigned char)label[i])) return ERROR;
    }

    return OK;
}
int check_assembly_line(AssemblyLine asm_line, char *line, int line_num, HashTable table) {

    char *token;
    char *delimiters = " \t\n";
    char line_copy[MAX_LINE_LEN];

    /* Create a copy of the line to tokenize */
    strcpy(line_copy, line);

    /* Tokenize the line and check for label */
    token = strtok(line_copy, delimiters);
    if (!check_label(token, table, line_num)) {
        token = strtok(NULL, delimiters);
    }

    /* If no token after label, it's a valid line */
    if (token == NULL) {
        return OK;
    }

    /* Check for .data directive */
    if (strcmp(token, ".data") == 0) {
        char *data_part = strstr(line, ".data") + strlen(".data");
        trim_whitespace(data_part);

        if (check_data_directive(data_part, line_num) != 0) {
            print_error(ERR_INVALID_DATA_INPUT, line_num);
            return ERROR;
        }

        return OK;
    }
    /* Check for .string directive */
    else if (strcmp(token, ".string") == 0) {
        char *string_part = strstr(line, ".string") + strlen(".string");
        if (check_string_directive(string_part, line_num)) {
            return ERROR;
        }

        return OK;
    }
    /* Check for operation name */
    else if (!(is_op_name(token))) {
        check_operands(asm_line, line_num);
        return OK;
    }
    else if (strcmp(token, ".entry") == 0) {
        return OK;
    }
   
    else if (strcmp(token, ".extern") == 0) {
        return OK;
    }
    /* Error handling */
    else {
        printf("ERROR: Unknown directive or operation.\n");
        return ERROR;
    }
}
int check_string_directive(char *string, int line_num) {

    int len;

    /* Trim leading whitespace from the string */
    trim_whitespace(string);

    /* Skip any leading whitespace */
    while (isspace(*string)) {
        string++;
    }

    len = strlen(string);

    /* Check for opening quote */
    if (string[0] != '\"') {
        print_error(ERR_NO_STRING_QUOTES, line_num);
        return ERROR;
    }

    /* Check for closing quote */
    if (string[len - 1] != '\"') {
        print_error(ERR_NO_STRING_QUOTES, line_num);
        return ERROR;
    }

    return OK;
}
/*Checking operand validity*/
int check_operands(AssemblyLine asm_line, int line_num) {
    if(check_comma_operands(asm_line,line_num)){
        return ERROR;
    }
    if(check_operand_amount(asm_line,line_num)){
        return ERROR;
    }
    if(is_valid_operand(asm_line,line_num)){
        return ERROR;
    }
    if(is_valid_command(asm_line,line_num)) {
        return ERROR;
    }
    return OK;

}
/*Checking comma between operands*/
int check_comma_operands(AssemblyLine asm_line, int line_num) {
    if(asm_line.src_operand == NULL && asm_line.dest_operand != NULL){
        if(strstr(asm_line.dest_operand," ")){
            print_error(ERR_MISSING_COMMA,line_num);
            return ERROR;
        }
    }
    return OK;
}
/*Checking operand amount by the rules defined in the booklet*/
int check_operand_amount(AssemblyLine asm_line, int line_num) {

    switch (asm_line.opcode) {
    case MOV:
    case CMP:
    case ADD:
    case SUB:
    case LEA:
        {
        if (asm_line.src_operand == NULL || asm_line.dest_operand == NULL) {
        print_error(ERR_MISSING_OPERANDS, line_num);
        return ERROR;
        }
        break;

        }
        case CLR:
        case NOT:
        case INC:
        case DEC:
        case JMP:
        case BNE:
        case RED:
        case PRN:
        case JSR:
        {
            if(asm_line.src_operand != NULL && asm_line.dest_operand != NULL){
                print_error(ERR_EXTRA_OPERANDS, line_num);
                return ERROR;
            }
            if(asm_line.src_operand == NULL && asm_line.dest_operand == NULL){
                print_error(ERR_MISSING_OPERANDS, line_num);
                return ERROR;
            }
            break;
        }
        case RTS:
        case STOP:
        {
            if(asm_line.src_operand != NULL || asm_line.dest_operand != NULL){
                print_error(ERR_EXTRA_OPERANDS, line_num);
                return ERROR;
            }
        }

    }
    return OK;

}
int check_data_directive(char *line, int line_num) {

    char trimmed_line[MAX_LINE_LEN];
    char *ptr = line;
    char *start;

    /* Process each segment of the line separated by commas */
    while (*ptr != '\0') {

        /* Skip leading whitespace */
        while (isspace((unsigned char)*ptr)) ptr++;

        start = ptr;

        /* Find the next comma or end of string */
        while (*ptr != ',' && *ptr != '\0') ptr++;

        /* Null-terminate the current segment and move past the comma */
        if (*ptr == ',') {
            *ptr = '\0';
            ptr++;
        }

        /* Trim leading and trailing whitespace from the segment */
        strcpy(trimmed_line, trim_whitespace(start));

        /* Check for multiple consecutive commas */
        if (strlen(trimmed_line) == 0) {
            printf("Error on line %d: Multiple consecutive commas in .data directive\n", line_num);
            return ERROR;
        } else {
            /* Validate integer format */
            if (!check_valid_integer(trimmed_line)) {
                return ERROR;
            }

            /* Check if the integer is within the valid range */
            if (!is_in_range(strtol(trimmed_line, NULL, 10))) {
                print_error(ERR_DATA_OUT_OF_RANGE, line_num);
                printf("The number %ld is out of range\n", strtol(trimmed_line, NULL, 10));
                return ERROR;
            }
        }
    }

    return OK;
}



int check_valid_integer(char *str) {
    char *endptr;
    strtol(str, &endptr, 10);
    return *endptr == '\0' && endptr != str;
}
/*Check that the given integer is in the given range*/
int is_in_range(long value) {
    return value >= MIN_SIZE_IMMEDIATE_OPERAND_NEG && value <= MAX_SIZE_IMMEDIATE_OPERAND_POS;
}
/*Handle operand validity*/
int is_valid_operand(AssemblyLine asm_line, int line_num) {
    if (asm_line.src_operand != NULL) {
        if (asm_line.src_operand[0] == '#') {
            if (!check_valid_imm_operand(asm_line.src_operand, line_num)) {
                print_error(ERR_INVALID_OPERAND_IMMEDIATE, line_num);
                return ERROR;
            }
        }
        if (asm_line.src_operand[0] == '*') {
            asm_line.src_operand++;
            if (is_reg_name(asm_line.src_operand)) {
                print_error(ERR_INVALID_OPERAND_POINTER, line_num);


            }
        }
    }
        if (asm_line.dest_operand != NULL) {
            if (asm_line.dest_operand[0] == '#') {
                if (!check_valid_imm_operand(asm_line.dest_operand,line_num)) {
                    print_error(ERR_INVALID_OPERAND_IMMEDIATE, line_num);
                    return ERROR;
                }
            }
            if (asm_line.dest_operand[0] == '*') {
                asm_line.dest_operand++;
                if (is_reg_name(asm_line.dest_operand)) {
                    print_error(ERR_INVALID_OPERAND_POINTER, line_num);
                    return ERROR;
                }
            }


        }
        return OK;



}
/* Check immediate operand validty*/
int check_valid_imm_operand(char *operand,int line_num) {
    int value;
    char *operand_cp;
        if(strstr(operand,"#")) { /*Should begin with #*/
            operand++;}
        if (*operand == '-' || *operand == '+') operand++;
        operand_cp = operand;
        if (!isdigit((unsigned char) *operand)) return 0;
        while (*operand) {
            if (!isdigit((unsigned char) *operand)) return 0;
            operand++;
        }
	/*Check that operand value is in range */
        value = strtol(operand_cp, NULL, 10);
        if(value > MAX_SIZE_IMMEDIATE_OPERAND_POS || value < MIN_SIZE_IMMEDIATE_OPERAND_NEG){
            print_error(ERR_IMMEDIATE_OPERAND_OUT_OF_RANGE,line_num);
            return 0;
        }

        return 1;
    }


