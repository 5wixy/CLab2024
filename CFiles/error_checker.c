#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "../HeaderFiles/error_checker.h"
#include "../HeaderFiles/validation.h"
#include "../HeaderFiles/data_strct.h"
#include "../HeaderFiles/helper.h"
#include "../HeaderFiles/Errors.h"


int check_line_error(AssemblyLine asm_line, char *line, int line_num) {
    char line_copy[MAX_LINE_LEN];

    check_assembly_line(asm_line, line, line_num);





}
int check_label(char *label) {
    int len;
    int i;

    // Check if label is NULL or empty
    if (!label || !(*label)) return 0;

    len = strlen(label);

    // Check if label is too short or does not end with a colon
    if (len < 2 || label[len - 1] != ':') return 0;

    // Check if the first character is a letter
    if (!isalpha((unsigned char)label[0])) return 0;

    // Check if all characters before the colon are letters or digits
    for (i = 0; i < len - 1; i++) {
        if (!isalnum((unsigned char)label[i])) return 0;
    }

    return 1; // Label is valid
}

int check_assembly_line(AssemblyLine asm_line, char *line, int line_num) {
    char *token;
    char *delimiters = " \t\n";
    char line_copy[MAX_LINE_LEN];

    strcpy(line_copy,line);
    token = strtok(line_copy, delimiters);
    if(check_label(token)){
        token = strtok(NULL, delimiters);
    }

    if (strcmp(token, ".data") == 0) {
        // Get the rest of the line after .data
        char *data_part = strstr(line, ".data") + strlen(".data");

        // Trim leading spaces
        trim_whitespace(data_part);

        if (check_data_directive(data_part, line_num) != 0) {
            print_error(ERR_MULTIPLE_COMMAS_DATA,line_num);
            return -1;
        }

        return 0;
    }
    else if(strcmp(token,".string") == 0){
        //CHECK STRING CORRECTIVITY
        return 0;
    }
    else if(!(is_op_name(token))){
        check_operands(asm_line,line_num);
        return 0;
    }
    else if(strcmp(token,".entry") == 0){
        //CHECK ENTRY CORRECTIVITY
        return 0;
    }
    else if(strcmp(token,".extern") == 0){
        //CHECK EXTERN CORRECTIVITY
        return 0;
    }
    else{
        printf("ERROR: Unknown directive or operation.\n");
        return 1;
    }


}
int check_operands(AssemblyLine asm_line, int line_num) {
    if(check_operand_amount(asm_line,line_num)){
        return 1;
    }
    if(is_valid_operand(asm_line,line_num)){
        return 1;
    }
    if(is_valid_command(asm_line,line_num)) {
        return 1;
    }








}
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
        return 1;
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
                return 1;
            }
            if(asm_line.src_operand == NULL && asm_line.dest_operand == NULL){
                print_error(ERR_MISSING_OPERANDS, line_num);
                return 1;
            }
            break;
        }
        case RTS:
        case STOP:
        {
            if(asm_line.src_operand != NULL || asm_line.dest_operand != NULL){
                print_error(ERR_EXTRA_OPERANDS, line_num);
                return 1;
            }
        }

    }
    return 0;

}
int check_data_directive(char *line, int line_num) {
    char trimmed_line[MAX_LINE_LEN];
    char *ptr = line;
    char *start;

    while (*ptr != '\0') {
        // Skip leading whitespace
        while (isspace((unsigned char)*ptr)) ptr++;

        // Start of the next token
        start = ptr;

        // Move pointer to the end of the current token or line
        while (*ptr != ',' && *ptr != '\0') ptr++;

        // If we encountered a comma, set it to '\0' to terminate the current token
        if (*ptr == ',') {
            *ptr = '\0';
            ptr++;
        }

        // Trim the token
        strcpy(trimmed_line, trim_whitespace(start));

        if (strlen(trimmed_line) == 0) {
            // If the trimmed token is empty, it's an error if it's not the first token
            printf("Error on line %d: Multiple consecutive commas in .data directive\n", line_num);
            return -1;
        } else {
            // Check if the token is a valid integer
            if (!is_valid_integer(trimmed_line)) {
                printf("Error on line %d: Invalid data value '%s'\n", line_num, trimmed_line);
                return -1;
            }
        }
    }

    return 0;  // Return 0 if no errors were found
}



int is_valid_integer(char *str) {
    char *endptr;
    strtol(str, &endptr, 10);
    return *endptr == '\0' && endptr != str;
}
int is_valid_operand(AssemblyLine asm_line, int line_num) {
    if (asm_line.src_operand != NULL) {
        if (asm_line.src_operand[0] == '#') {
            if (!check_valid_imm_operand(asm_line.src_operand,line_num)) {
                print_error(ERR_INVALID_OPERAND_IMMEDIATE, line_num);
                return 1;
            }
        }
        if (asm_line.src_operand[0] == '*') {
            asm_line.src_operand++;
            if (is_reg_name(asm_line.src_operand)) {
                print_error(ERR_INVALID_OPERAND_POINTER, line_num);


            }
        }
        if (asm_line.dest_operand != NULL) {
            if (asm_line.dest_operand[0] == '#') {
                if (!check_valid_imm_operand(asm_line.dest_operand,line_num)) {
                    print_error(ERR_INVALID_OPERAND_IMMEDIATE, line_num);
                    return 1;
                }
            }
            if (asm_line.dest_operand[0] == '*') {
                asm_line.dest_operand++;
                if (is_reg_name(asm_line.dest_operand)) {
                    print_error(ERR_INVALID_OPERAND_IMMEDIATE, line_num);
                    return 1;
                }
            }


        }
        return 0;


    }
}

int check_valid_imm_operand(char *operand,int line_num) {
        if(strstr(operand,"#")) {
            operand++;}
        if (*operand == '-' || *operand == '+') operand++;
        char *operand_cp = operand;
        // The remaining characters should be all digits
        if (!isdigit((unsigned char) *operand)) return 0; // Ensure there's at least one digit
        while (*operand) {
            if (!isdigit((unsigned char) *operand)) return 0;
            operand++;
        }
        int value = strtol(operand_cp, NULL, 10);
        if(value > MAX_SIZE_IMMEDIATE_OPERAND_POS || value < MIN_SIZE_IMMEDIATE_OPERAND_NEG){
            print_error(ERR_IMMEDIATE_OPERAND_OUT_OF_RANGE,line_num);
            return 0;
        }

        return 1;
    }


