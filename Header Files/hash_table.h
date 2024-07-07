//
// Created by gyank on 03/07/2024.
//

#ifndef CLAB_HASH_TABLE_H
#define CLAB_HASH_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the TABLE_SIZE
#define TABLE_SIZE 100 // Adjust this size as needed

// Define the HashItem structure
typedef struct HashItem {
    char *name;
    char *content;
    struct HashItem *next;
} HashItem;

// Define the HashTable structure
typedef struct {
    HashItem *table[TABLE_SIZE];
} HashTable;

// Function prototypes
unsigned long hash(const char *macro_name);
void initHashTable(HashTable *ht);
void insert(HashTable *ht, const char *key, const char *value);
char *get(HashTable *ht, const char *key);
void freeHashTable(HashTable *ht);

#endif // CLAB_HASH_TABLE_H