
#ifndef CLAB_SECOND_PASS_H
#define CLAB_SECOND_PASS_H

#include "hash_table.h"
#include "asm_data.h"
#include "data_strct.h"

int start_second_pass(char *file_name, HashTable *symbol_table, AssemblyData *ad);
void process_operands_for_labels(AssemblyLine *line, HashTable *symbol_table, AssemblyData *ad, int *IC, ExternOperand **extern_operands, int *extern_count, int *extern_capacity,int line_num);
void process_and_update_operand(AssemblyLine *line, const char *operand, HashTable *symbol_table, AssemblyData *ad, int index, ExternOperand **extern_operands, int *extern_count, int *extern_capacity,int line_num);
int count_entry(HashTable *table);
int count_extern(HashTable *table);
void fill_ob_file(const char *file_name, AssemblyData *ad);
void fill_ent_file(char *file_name, HashTable *table,AssemblyData *ad);
void fill_ext_file(const char *file_name, ExternOperand *extern_operands, int extern_count);
void add_extern_operand(ExternOperand **extern_operands, int *extern_count, int *extern_capacity, const char *operand, int address);
#endif
