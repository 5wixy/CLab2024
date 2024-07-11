#include "../Header Files/validation.h"
#include "../Header Files/globals.h"
#include <stdio.h>
#include <string.h>
#include "../Header Files/helper.h"
char op_arr[OP_ARR_SIZE][5] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};
char reg_arr[REG_ARR_SIZE][3] = {"r0","r1","r2","r3","r4","r5","r6","r7"};
char label_arr[LABEL_ARR_SIZE][7] = {".data",".string",".entry",".extern"};


int is_valid_macro_name(char *macro_name) {
    macro_name = remove_trailing_newline(macro_name);
    if (is_op_name(macro_name) && is_reg_name(macro_name) & is_label_name(macro_name)) {
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
int is_label_name(char *macro_name) {
    int i;
    for (i = 0; i < LABEL_ARR_SIZE; ++i) {
        if (strcmp(macro_name, label_arr[i]) == 0) {
            return 0;
        }
    }
    return 1;

}
/*Checking if macro name equals to one of the operator names */
int is_op_name(char *macro_name) {

    int i;
    for (i = 0; i < OP_ARR_SIZE; ++i) {
        if (strcmp(macro_name, op_arr[i]) == 0) {
            return 0;
        }
    }
    return 1;

}
int is_name_too_long(char *macro_name){
    return strlen(macro_name) > MAX_MACRO_NAME_LEN;


}
/*Checking if macro name equals to one of the register names */
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


