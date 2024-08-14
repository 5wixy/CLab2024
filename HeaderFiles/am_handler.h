//
// Created by gyank on 01/08/2024.
//

#ifndef CLAB_AM_HANDLER_H
#define CLAB_AM_HANDLER_H
#include "../HeaderFiles/globals.h"
#include "asm_data.h"
#include "../HeaderFiles/data_strct.h"
#include "../HeaderFiles/hash_table.h"

typedef struct command_parts {
    char *label;     /* The label associated with the command */
    int opcode;      /* The opcode of the command */
    char *source;    /* The source operand of the command */
    char *dest;      /* The destination operand of the command */
} command_parts;


char* get_first_word(char *str);
void skip_first_word(char *line_copy);
int match_opcodes(char *str);
int find_register_index( char *reg_name);
void process_register_operands(const char *src_operand, const char *dest_operand, char *output);
void process_register_operand(const char *operand, char *output, int position);
void process_direct_operand(const char *operand, char *output,HashTable *table);
void process_operand(const char *operand, int addressing_method, char *output, int *num_words, int position,HashTable *table);
void process_immediate_operand(const char *operand, char *output);
int is_valid_input(const char *line);
void update_label_addresses(HashTable *ht, int IC);
void apply_increment(int *IC, int increment);
void swap_src_dest(AssemblyLine *line);
int count_numbers(const char *line);
void increment_IC(AssemblyLine line, int *IC);
int get_label_address(const char *label,HashTable *table);
void transform_to_binary(AssemblyLine *line, char binary_output[][WORD_SIZE], int *num_words, HashTable *table);
int calculate_increment(const char *source, const char *dest);
void process_string_directive(AssemblyLine *line, HashTable *table, int *DC, AssemblyData *ad);
void process_data_directive(AssemblyLine *line, HashTable *table, int *DC, AssemblyData *ad);
void process_operation_line(AssemblyLine *line, HashTable *table, int *IC, AssemblyData *ad);
#endif //CLAB_AM_HANDLER_H
