//
// Created by gyank on 15/07/2024.
//

#ifndef CLAB_FIRST_PASS_H
#define CLAB_FIRST_PASS_H
int start_first_pass(char *file_name,HashTable *table);
void process_labeled_operation(char *label_name,char *line,HashTable *table,int IC);
void process_extern(char *label_name, HashTable *table);
void process_entry(char *label_name, HashTable *table);
void process_data_or_string(char *label_name, char *directive, char *remaining_line, HashTable *table, int IC);
void process_data_directive(char *label_name, char *line, HashTable *table, int IC);
void process_string_directive(char *label_name, char *line, HashTable *table, int IC);
#endif //CLAB_FIRST_PASS_H
