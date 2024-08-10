
#ifndef CLAB_SECOND_PASS_H
#define CLAB_SECOND_PASS_H

#include "hash_table.h"
#include "asm_data.h"
#include "data_strct.h"

int start_second_pass(char *file_name, HashTable *symbol_table, AssemblyData *ad);
void process_operands_for_labels(AssemblyLine *line, HashTable *symbol_table, AssemblyData *ad, int *IC);
void process_and_update_operand(AssemblyLine *line,const char *operand, HashTable *symbol_table, AssemblyData *ad, int index);
int count_entry(HashTable *table);
int count_extern(HashTable *table);
void fill_ob_file(const char *file_name, AssemblyData *ad);
#endif
