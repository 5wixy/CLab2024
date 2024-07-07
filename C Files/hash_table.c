#include "../Header Files/hash_table.h"
#include "../Header Files/globals.h"
#include "string.h"
#include "stdlib.h"


void initHashTable(HashTable *ht) {
    int i;
    for (i = 0; i < TABLE_SIZE; i++) {
        ht->table[i] = NULL;
    }
}
void insert(HashTable *ht, const char *key, const char *value) {
    //printf("%s",key);
    unsigned long idx = hash(key);
    // Create a new entry
    HashItem *newItem = (HashItem *)malloc(sizeof(HashItem));
    if (!newItem) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }
    newItem->name = strdup(key);
    newItem->content = strdup(value);
    newItem->next = NULL;

    // Insert into the hash table using chaining for collision resolution
    if (ht->table[idx] == NULL) {

        ht->table[idx] = newItem;
    } else {
        // Collision: add to the front of the chain
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
            return entry->content;  // Found key, return value
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
            free(prev->content);
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

