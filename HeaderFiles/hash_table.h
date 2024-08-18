#ifndef CLAB_HASH_TABLE_H
#define CLAB_HASH_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"


#define TABLE_SIZE 100
typedef enum {
    TYPE_MACRO,
    TYPE_LABEL
} ItemType;
typedef enum {
    LABEL_NORMAL = 0,
    LABEL_ENTRY = 1,
    LABEL_EXTERN = 2
} LabelType;
typedef enum{
    DAT = 0,
    INST = 1
} LabelSort;

typedef struct HashItem {
    char *name;
    ItemType type;
    union {
        char *content;
        struct {
            int address;
            LabelType type;
            LabelSort label_sort;
        } label;
    } data;
    struct HashItem *next;
} HashItem;


typedef struct {
    HashItem *table[TABLE_SIZE];
} HashTable;


unsigned long hash(const char *macro_name);
void init_hashtable(HashTable *ht);
void insert_macro(HashTable *ht, const char *key, const char *content);
char *get(HashTable *ht, const char *key);
void insert_label(HashTable *ht, const char *key, int address, int type,int label_sort);
void freeHashTable(HashTable *ht);
void free_macros_from_table(HashTable *ht);
HashItem *get_label(HashTable *ht, const char *key);
#endif