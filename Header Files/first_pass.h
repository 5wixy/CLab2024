//
// Created by gyank on 15/07/2024.
//

#ifndef CLAB_FIRST_PASS_H
#define CLAB_FIRST_PASS_H

#include "asm_data.h"

int start_first_pass(char *file_name, HashTable *symbol_table);
void process_operation(char *line, HashTable *table, int *IC, char *memory_array[][15]);
void process_labeled_operation(char *label_name, char *line, HashTable *table, int *IC, AssemblyData *ad);
void process_extern(char *label_name, HashTable *symbol_table);
void process_entry(char *label_name, HashTable *table);
void process_label_line(char *line, char *first_word, HashTable *table, int *IC, int *DC, AssemblyData *ad);
void process_data_or_string(char *label_name, const char *directive, char *remaining_line, HashTable *symbol_table, int *IC, int *DC, AssemblyData *ad);
void process_data_directive(char *label_name, char *line, HashTable *symbol_table, int *IC, int *DC, AssemblyData *ad);
void process_string_directive(char *label_name, char *line, HashTable *table, int *IC, int *DC, AssemblyData *ad);

#endif //CLAB_FIRST_PASS_H
