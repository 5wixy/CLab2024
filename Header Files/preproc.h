#ifndef PREPROC_H
#define PREPROC_H

#include "hash_table.h"

int open_read_file(char *file_name, HashTable *table);
char *save_macro_content(FILE *fp, fpos_t *pos,int *line_count);
void process_macro(FILE *fp, char *str, int line_count, HashTable *table);
int expand_macro(char file_name[]);
void remove_macros(FILE *original, FILE *copied);
void insert_macros(const char* source_file, const char* destination_file, HashTable *table);
#endif // PREPROC_H