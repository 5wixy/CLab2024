//
// Created by gyank on 01/08/2024.
//
#include "string.h"
#include "../Header Files/globals.h"
#include <ctype.h>
#include "stdlib.h"
#include "stdio.h"
#include "../Header Files/validation.h"
#include "../Header Files/helper.h"
#include "../Header Files/am_handler.h"

char* get_first_word(char *str) {
    char line_copy[MAX_LINE_LEN];
    strncpy(line_copy, str, sizeof(line_copy) - 1);
    line_copy[sizeof(line_copy) - 1] = '\0';

    char *token = strtok(line_copy, " \t");
    if (token != NULL) {
        char *first_word = (char*)malloc((strlen(token) + 1) * sizeof(char));
        if (first_word == NULL) {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }
        strncpy(first_word, token, strlen(token) + 1);
        return first_word;
    } else {
        return NULL;
    }
}
void break_line_to_words(char *str, char *tokens[], int max_tokens) {
    char *token;
    int token_count = 0;
    remove_commas(str);
    int i;
    // Initialize token array to NULL
    for (i = 0; i < max_tokens; i++) {
        tokens[i] = NULL;
    }

    // Get the first token
    token = strtok(str, " ");

    // Walk through the rest of the tokens
    while (token != NULL && token_count < max_tokens) {
        tokens[token_count] = (char *)malloc(strlen(token) + 1);
        if (tokens[token_count] != NULL) {
            strcpy(tokens[token_count], token);
            token_count++;
        }
        token = strtok(NULL, " ");
    }
}
void remove_commas(char *str) {
    char *src = str, *dst = str;

    while (*src) {
        if (*src != ',') {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';  // Null-terminate the modified string
}
void skip_first_word(char *line_copy) {
    char *src = line_copy;
    char *dst = line_copy;

    // Move the pointer to the end of the first word
    while (*src && !isspace((unsigned char)*src)) {
        src++;
    }

    // Skip any additional whitespace characters
    while (*src && isspace((unsigned char)*src)) {
        src++;
    }

    // Shift the remaining part of the string to the start of the array
    while (*src) {
        *dst++ = *src++;
    }

    *dst = '\0';  // Null-terminate the resulting string
}

int match_opcodes(char *str){
    int i;
    if (str == NULL) {
        return -1;
    }


    for (i = 0; i < OP_ARR_SIZE; i++) {
        if (strcmp(str, op_arr[i].opcode) == 0) {
            return i; /* Return the index of the matching opcode */
        }
    }
    return -1; /* Return -1 if the string does not match any known opcodes */
}

int is_legal_args(command_parts *command){



}
int is_reg_or_label(char *str) {
    /* Check if the string is a valid register using what_reg()
     * OR if it's a legal label using legal_label() */
    if ((find_register_index(str) >= 0) || is_legal_label(str)) {
        return 1;
    }
    return 0; /* Neither a valid register nor a legal label */
}
int find_register_index( char *reg_name) {
    int i;
    for (i = 0; i < REG_ARR_SIZE; ++i) {
        if (strcmp(reg_name, reg_arr[i]) == 0) {
            return i; // Return the index if the register name matches
        }
    }
    return -1; // Return -1 if the register name is not found
}
int is_data_instruction(char *word) {
    // Implement this function to check if the word is a data instruction (.data or .string)
    return (strcmp(word, ".data") == 0 || strcmp(word, ".string") == 0);
}

int is_valid_command(command_parts *command){


    switch (command->opcode) {
        case MOV:
        case ADD:
        case SUB:
        {

            if(command->dest[0]=="#"){
               printf("ERROR CANNOT DIRECT ADDRESS FOR THIS OPERATION");
                return 0;
            }
            //0,1,2,3 source, 1,2,3 dest.

        }
        case CMP:
        {
            //0,1,2,3 source and dest.
        }
        case LEA:
        {
            // 1 for source, 1,2,3 for dest
        }
        case CLR:
        case NOT:
        case INC:
        case DEC:
        case RED:

        {
            //no source. 1,2,3 for dest
        }
        case JMP:
        case BNE:
        case JSR:
        {
            //no source. addressing methods 1,2 for dest
        }
        case PRN:
        {
            //no source, 0,1,2,3 for dest
        }
        case RTS:
        case STOP:
        {
            //no source no dest
            if(command->source != NULL || command->dest != NULL){
                //ERROR OPCODE CANT HAVE OPERANDS
                return 0;

            }
        }

    }



}
