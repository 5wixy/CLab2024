
#ifndef CLAB_FIRST_PASS_H
#define CLAB_FIRST_PASS_H
#include "../HeaderFiles/data_strct.h"
#include "asm_data.h"

int start_first_pass(char *file_name, HashTable *symbol_table,AssemblyData *ad);
void process_labeled_operation(AssemblyLine *line, HashTable *table, int *IC, AssemblyData *ad);
void process_extern(char *label_name, HashTable *symbol_table);
void process_entry(char *label_name, HashTable *table,int *IC);
void process_label_line(AssemblyLine *line, HashTable *table, int *IC, int *DC, AssemblyData *ad);

#endif
