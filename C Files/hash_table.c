#include "../Header Files/hash_table.h"
#include "../Header Files/globals.h"
#include "string.h"
#include "stdlib.h"


void init_hashtable(HashTable *ht) {
    int i;
    for (i = 0; i < TABLE_SIZE; i++) {
        ht->table[i] = NULL;
    }
}
void insert_macro(HashTable *ht, const char *key, const char *content) {
    unsigned long idx = hash(key);

    // Create a new entry
    HashItem *newItem = (HashItem *)malloc(sizeof(HashItem));
    if (!newItem) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }
    newItem->name = strdup(key);
    newItem->type = TYPE_MACRO;
    newItem->data.content = strdup(content);
    newItem->next = NULL;

    // Insert into the hash table using chaining for collision resolution
    if (ht->table[idx] == NULL) {
        ht->table[idx] = newItem;
    } else {
        newItem->next = ht->table[idx];
        ht->table[idx] = newItem;
    }
}
void insert_label(HashTable *ht, const char *key, int address, const char *data_or_instruction, int type) {
    unsigned long idx = hash(key);

    // Create a new entry
    HashItem *newItem = (HashItem *)malloc(sizeof(HashItem));
    if (!newItem) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }
    newItem->name = strdup(key);
    newItem->type = TYPE_LABEL;
    newItem->data.label.address = address;
    newItem->data.label.data_or_instruction = strdup(data_or_instruction);
    newItem->data.label.type = type;
    newItem->next = NULL;

    // Insert into the hash table using chaining for collision resolution
    if (ht->table[idx] == NULL) {
        ht->table[idx] = newItem;
    } else {
        newItem->next = ht->table[idx];
        ht->table[idx] = newItem;
    }
}
char *get(HashTable *ht, const char *key) {
    unsigned long idx = hash(key);
    HashItem *entry = ht->table[idx];

    // Traverse the chain at the index
    while (entry != NULL) {
        if (strcmp(entry->name, key) == 0) {
            return entry->data.content;  // Found key, return value
        }
        entry = entry->next;
    }

    return NULL;  // Key not found
}
void freeHashTable(HashTable *ht) {
    int i;
    for (i = 0; i < TABLE_SIZE; i++) {
        HashItem *entry = ht->table[i];
        while (entry != NULL) {
            HashItem *prev = entry;
            entry = entry->next;
            free(prev->name);
            if (prev->type == TYPE_MACRO) {
                free(prev->data.content);
            } else if (prev->type == TYPE_LABEL) {
                free(prev->data.label.data_or_instruction);
            }
            free(prev);
        }
    }
}
unsigned long hash(const char *macro_name) {
    unsigned long hash = 5381;  // Initial hash value

    int c;
    while ((c = *macro_name++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash % TABLE_SIZE;
}

