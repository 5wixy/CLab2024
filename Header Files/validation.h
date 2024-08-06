//
// Created by gyank on 04/07/2024.
//

#ifndef CLAB_VALIDATION_H
#include "../Header Files/hash_table.h"
#include "globals.h"

#define CLAB_VALIDATION_H
typedef struct op_code {
    char *opcode;    /* The opcode corresponding to the operation */
    int args_num;     /* The number of arguments for the operation */
} op_code;
extern op_code op_arr[];
extern char reg_arr[REG_ARR_SIZE][3];
int is_valid_macro_name(char *macro_name);
int is_op_name(char *macro_name);
int is_reg_name(char *macro_name);
int is_type_data_type_name(char *macro_name);
int has_extra(char *macro_name);
int is_name_too_long(char *name);
int is_label_macro_name_collision(HashTable *ht,char *label_name);
int is_legal_label(char *str);
int get_operand_count(const char *name);
char* is_data_or_string(char *line);
int is_external_label(const char *label,HashTable *table);
#endif //CLAB_VALIDATION_H
