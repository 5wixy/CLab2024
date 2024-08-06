//
// Created by gyank on 01/08/2024.
//

#ifndef CLAB_AM_HANDLER_H
#define CLAB_AM_HANDLER_H

#include "asm_data.h"

typedef struct command_parts {
    char *label;     /* The label associated with the command */
    int opcode;      /* The opcode of the command */
    char *source;    /* The source operand of the command */
    char *dest;      /* The destination operand of the command */
} command_parts;
typedef struct inst_parts {
    char *label;       /* The label associated with the instruction */
    short *nums;       /* An array of short integers representing instruction data */
    int len;           /* The number of data in *nums including '0' at the end of a string */
    char *arg_label;   /* The label associated with an argument, if applicable */
    int is_extern;     /* Indicates whether the instruction is marked as external */
} inst_parts;

char* get_first_word(char *str);
void skip_first_word(char *line_copy);
int match_opcodes(char *str);
int find_register_index( char *reg_name);
int is_data_instruction(char *word);
int is_valid_command(command_parts *command);
void process_register_operands(const char *src_operand, const char *dest_operand, char *output);
void process_register_operand(const char *operand, char *output, int position);
void process_direct_operand(const char *operand, char *output,HashTable *table);
void process_operand(const char *operand, int addressing_method, char *output, int *num_words, int position,HashTable *table);
void process_immediate_operand(const char *operand, char *output);
int is_valid_input(const char *line);
void apply_increment(int *IC, int increment);
int count_numbers(const char *line);
void increment_IC(char *line, int *IC);
int get_label_address(const char *label,HashTable *table);
void transform_to_binary(const char *line, char binary_output[][WORD_SIZE], int *num_words,HashTable *table);
int calculate_increment(const char *source, const char *dest);
void handle_directive_or_label(char *line_copy, char *first_word, HashTable *symbol_table, int *IC, int *DC, AssemblyData *ad);
#endif //CLAB_AM_HANDLER_H
