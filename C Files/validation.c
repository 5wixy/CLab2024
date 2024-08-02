#include "../Header Files/validation.h"
#include "../Header Files/globals.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../Header Files/hash_table.h"
#include "../Header Files/helper.h"
op_code op_arr[] = {{"mov",2},{"cmp",2}, {"add",2} ,{"sub",2} ,
                    {"lea",2},{"clr",1}, {"not",1}, {"inc",1},
                    {"dec",1}, {"jmp",1}, {"bne",1} ,{"red",1},
                    {"prn",1} ,{"jsr",1} ,{"rts",0} ,{"stop",0}};
char reg_arr[REG_ARR_SIZE][3] = {"r0","r1","r2","r3","r4","r5","r6","r7"};
char type_arr[TYPE_ARR_SIZE][7] = {".data", ".string", ".entry", ".extern"};


int is_valid_macro_name(char *macro_name) {
    macro_name = remove_trailing_newline(macro_name);
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
//return 0 if match found 1 if not
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
    unsigned long idx = hash(label_name); // Compute the hash index for the label name
    HashItem *entry = ht->table[idx]; // Get the head of the chain at the index

    // Traverse the chain at the index
    while (entry != NULL) {
        if (entry->type == TYPE_MACRO && strcmp(entry->name, label_name) == 0) {
            return 1; // Collision found: the label name matches an existing macro name
        }
        entry = entry->next; // Move to the next item in the chain
    }

    return 0; // No collision found
}





int is_name_too_long(char *name){
    return strlen(name) > MAX_MACRO_NAME_LEN;


}
/*Checking if macro name equals to one of the register names */
//returns 0 if match found 1 if not
int is_reg_name(char *macro_name){
    int i;
    for (i = 0; i < REG_ARR_SIZE; ++i) {
        if (strcmp(macro_name, reg_arr[i]) == 0) {
            printf("Match found at index %d\n", i);
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
    //printf(line);
    char *first = strtok(line," ");
    //printf(first);
    if(strcmp(first,".data") == 0){
        return "data";
    }
    if(strcmp(first,".string")){
        return "string";


    }

}

