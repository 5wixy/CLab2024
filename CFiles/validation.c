#include "../HeaderFiles/validation.h"
#include "../HeaderFiles/globals.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../HeaderFiles/hash_table.h"
#include "../HeaderFiles/helper.h"
#include "../HeaderFiles/am_handler.h"
#include "../HeaderFiles/first_pass.h"
#include "../HeaderFiles/Errors.h"

op_code op_arr[] = {{"mov",2},{"cmp",2}, {"add",2} ,{"sub",2} ,
                    {"lea",2},{"clr",1}, {"not",1}, {"inc",1},
                    {"dec",1}, {"jmp",1}, {"bne",1} ,{"red",1},
                    {"prn",1} ,{"jsr",1} ,{"rts",0} ,{"stop",0}};
char reg_arr[REG_ARR_SIZE][3] = {"r0","r1","r2","r3","r4","r5","r6","r7"};
char type_arr[TYPE_ARR_SIZE][7] = {".data", ".string", ".entry", ".extern"};


int is_valid_macro_name(char *macro_name) {
    remove_trailing_newline(macro_name);
    if (is_op_name(macro_name) && is_reg_name(macro_name) & is_type_data_type_name(macro_name)) {
        return 1;
    }

    return 0;
}
int has_extra(char *macro_name){
    char *extra;
    extra = strtok(NULL, "\n");
    if (extra != NULL) {
        return 1;
    }
    return 0;


}
/*Checking if macro name equals to one of the label names */
int is_type_data_type_name(char *macro_name) {
    int i;
    for (i = 0; i < TYPE_ARR_SIZE; ++i) {
        if (strcmp(macro_name, type_arr[i]) == 0) {
            return 0;
        }
    }
    return 1;

}
/*Checking if macro name equals to one of the operator names */
/*return 0 if match found 1 if not */
int is_op_name(char *macro_name) {

    int i;
    for (i = 0; i < OP_ARR_SIZE; ++i) {
        if (strcmp(macro_name, op_arr[i].opcode) == 0) {
            return 0;
        }
    }
    return 1;

}
int is_label_macro_name_collision(HashTable *ht,char *label_name){
    unsigned long idx = hash(label_name);
    HashItem *entry = ht->table[idx];


    while (entry != NULL) {
        if (entry->type == TYPE_MACRO && strcmp(entry->name, label_name) == 0) {
            return 1;
        }
        entry = entry->next;
    }

    return 0;
}
int is_external_label(const char *label, HashTable *table) {

    return 0;  /* Label is not marked as external or not found */
}



int is_name_too_long(char *name){
    return strlen(name) > MAX_MACRO_NAME_LEN;


}
/*Checking if macro name equals to one of the register names */
/*returns 0 if match found 1 if not */
int is_reg_name(char *macro_name){
    int i;
    for (i = 0; i < REG_ARR_SIZE; ++i) {
        if (strcmp(macro_name, reg_arr[i]) == 0) {
            return 0;
        }
    }

    return 1;


}
int is_legal_label(char *str){
    if (str == NULL || strlen(str) > MAX_LABEL_NAME_LEN) {
        return 0;
    }


    if(isalpha(*str) && !(is_op_name(str)) && (is_reg_name(str))){
        return 1;

    }
    return 0;

}
char* is_data_or_string(char *line){
    char *first = strtok(line," ");
    if(strcmp(first,".data") == 0){
        return "data";
    }
    if(strcmp(first,".string")==0){
        return "string";
    }
    return NULL;
}
int is_entry_or_extern(const char *line, HashTable *symbol_table, int *IC) {
    char first_word[MAX_LINE_LEN];
    char label_name[MAX_LINE_LEN];
    int result = 0;

    /* Copy the line to avoid modifying the original */
    char line_copy[MAX_LINE_LEN];
    strcpy(line_copy, line);


    char *token = strtok(line_copy, " \t");
    if (token == NULL) {
        return result;
    }
    strcpy(first_word, token);


    if (strcmp(first_word, ".entry") == 0) {

        token = strtok(NULL, " \t");
        if (token != NULL) {
            strcpy(label_name, token);
            process_entry(label_name, symbol_table, IC);
            result = 1;
        }
    } else if (strcmp(first_word, ".extern") == 0) {
        /* Get the label name after `.extern` */
        token = strtok(NULL, " \t");
        if (token != NULL) {
            process_extern(token, symbol_table);
            result = 1;
        }
    }

    return result;
}
int is_valid_command(AssemblyLine asm_line, int line_num) {


    switch (asm_line.opcode) {
        case MOV:
        case ADD:
        case SUB:
            /* Both source and destination are allowed
             Immediate addressing not allowed for destination */
            if (asm_line.dest_operand && detect_addressing_method(asm_line.dest_operand) == 0) {
                printf("line %d: ERROR: Cannot use immediate address for destination for MOV/ADD/SUB. \n", line_num);
                return 0;
            }
            break;

        case CMP:
            /* Both source and destination are allowed */
            break;

        case LEA:
            /* Source must be direct or memory-indirect, destination can be any valid method */
            if (asm_line.src_operand && detect_addressing_method(asm_line.src_operand) > 1) {
                printf("line %d: ERROR: LEA source must be direct or memory-indirect at line. \n",line_num);
                return 0;
            }
            break;
        case CLR:
        case NOT:
        case INC:
        case DEC:
        case RED:
            /* No source operand, destination can be any valid method */
            if (asm_line.src_operand != NULL) {
                printf("line %d: ERROR: CLR/NOT/INC/DEC/RED cannot have a source operand.\n",line_num);
                return 0;
            }
            break;

        case JMP:
        case BNE:
        case JSR:
            /* No source operand, destination must be direct or memory-indirect */
            if (asm_line.src_operand != NULL) {
                printf("line %d: ERROR: JMP/BNE/JSR cannot have a source operand.\n",line_num);
                return 0;
            }
            if (asm_line.dest_operand && detect_addressing_method(asm_line.dest_operand) < 1 && detect_addressing_method(asm_line.dest_operand) > 2) {
                printf("line %d: ERROR: JMP/BNE/JSR destination must be direct or memory-indirect.\n",line_num);
                return 0;
            }
            break;

        case PRN:
            /* No source operand, destination can be any valid method */
            if (asm_line.src_operand != NULL) {
                printf("line %d: ERROR: PRN cannot have a source operand.\n",line_num);
                return 0;
            }
            break;

        case RTS:
        case STOP:
            /* No source or destination operands */
            if (asm_line.src_operand != NULL || asm_line.dest_operand != NULL) {
                printf("line %d: ERROR: RTS/STOP cannot have operands.\n",line_num);
                return 0;
            }
            break;

        default:
            printf("line %d: ERROR: Unknown opcode.\n",line_num);
            return 0;
    }

    return 1;
}



int is_extern(const char *line) {
    return strncmp(line, ".extern", 7) == 0 && (line[7] == ' ' || line[7] == '\0');
}
int is_entry(const char *line) {
    return strncmp(line, ".entry", 6) == 0 && (line[6] == ' ' || line[6] == '\0');
}