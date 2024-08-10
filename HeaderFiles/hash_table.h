//
// Created by gyank on 03/07/2024.
//

#ifndef CLAB_HASH_TABLE_H
#define CLAB_HASH_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../HeaderFiles/globals.h"

// Define the TABLE_SIZE
#define TABLE_SIZE 100 // Adjust this size as needed
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
// Define the HashItem structure
typedef struct HashItem {
    char *name;
    ItemType type;
    union {
        char *content; // For macros
        struct {
            int address;
            LabelType type;
            LabelSort label_sort;
        } label; // For labels
    } data;
    struct HashItem *next;
} HashItem;

// Define the HashTable structure
typedef struct {
    HashItem *table[TABLE_SIZE];
} HashTable;

// Function prototypes
unsigned long hash(const char *macro_name);
void init_hashtable(HashTable *ht);
void insert_macro(HashTable *ht, const char *key, const char *content);
char *get(HashTable *ht, const char *key);
void insert_label(HashTable *ht, const char *key, int address, int type,int label_sort);
void freeHashTable(HashTable *ht);
int get_address(HashTable *ht, const char *key);
HashItem *get_label(HashTable *ht, const char *key);
#endif // CLAB_HASH_TABLE_H