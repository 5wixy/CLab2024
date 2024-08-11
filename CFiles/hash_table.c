#include "../HeaderFiles/hash_table.h"
#include "../HeaderFiles/globals.h"
#include "string.h"
#include "stdlib.h"
#include "../HeaderFiles/helper.h"


void init_hashtable(HashTable *ht) {
    int i;
    for (i = 0; i < TABLE_SIZE; i++) {
        ht->table[i] = NULL;
    }
}
void insert_macro(HashTable *ht, const char *key, const char *content) {
    unsigned long idx = hash(key);

    /* Create a new item */
    HashItem *newItem = (HashItem *)malloc(sizeof(HashItem));
    if (!newItem) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }
    newItem->name = strdup(key);
    newItem->type = TYPE_MACRO;
    newItem->data.content = strdup(content);
    newItem->next = NULL;

    if (ht->table[idx] == NULL) {
        ht->table[idx] = newItem;
    } else {
        newItem->next = ht->table[idx];
        ht->table[idx] = newItem;
    }
}
void insert_label(HashTable *ht, const char *key, int address, int type, int label_sort) {
    unsigned long idx = hash(key);

    /* Check if the label already exists */
    HashItem *current = ht->table[idx];
    while (current != NULL) {
        if (strcmp(current->name, key) == 0 && current->type == TYPE_LABEL) {
            /* Update the existing label's attributes */
            current->data.label.address = address;
            current->data.label.type = LABEL_ENTRY;  /* Ensure type is updated to LABEL_ENTRY */
            current->data.label.label_sort = label_sort;
            return;
        }
        current = current->next;
    }

    /* If the label does not exist, create a new entry */
    HashItem *newItem = (HashItem *)malloc_helper(sizeof(HashItem), __FILE_NAME__, __LINE__);
    if (!newItem) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }
    newItem->name = strdup(key);
    if (!newItem->name) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    newItem->type = TYPE_LABEL;
    newItem->data.label.address = address;
    newItem->data.label.type = type;
    newItem->data.label.label_sort = label_sort;
    newItem->next = ht->table[idx];
    ht->table[idx] = newItem;
}
char *get(HashTable *ht, const char *key) {
    unsigned long idx = hash(key);
    HashItem *entry = ht->table[idx];

    while (entry != NULL) {
        if (strcmp(entry->name, key) == 0) {
            return entry->data.content;
        }
        entry = entry->next;
    }

    return NULL;
}
HashItem *get_label(HashTable *ht, const char *key) {
    unsigned long idx = hash(key);
    HashItem *entry = ht->table[idx];


    while (entry != NULL) {
        if (strcmp(entry->name, key) == 0) {
            return entry;
        }
        entry = entry->next;
    }

    return NULL;
}
void freeHashTable(HashTable *ht) {
    int i;
    for (i = 0; i < TABLE_SIZE; i++) {
        HashItem *entry = ht->table[i];
        while (entry != NULL) {
            HashItem *prev = entry;
            entry = entry->next;
            my_free(prev->name,__FILE_NAME__,__LINE__);
            if (prev->type == TYPE_MACRO) {
                my_free(prev->data.content,__FILE_NAME__,__LINE__);
            } else if (prev->type == TYPE_LABEL) {

            }
            my_free(prev,__FILE_NAME__,__LINE__);
        }
    }
}
void free_macros_from_table(HashTable *ht) {
    int i;
    for (i = 0; i < TABLE_SIZE; i++) {
        HashItem *item = ht->table[i];
        HashItem *prev = NULL;
        while (item != NULL) {
            if (item->type == TYPE_MACRO) {
                HashItem *temp = item;
                // Move to the next item before freeing the current one
                item = item->next;

                // Free the memory associated with TYPE_MACRO
                my_free(temp->name, __FILE_NAME__, __LINE__);
                my_free(temp->data.content, __FILE_NAME__, __LINE__);
                my_free(temp, __FILE_NAME__, __LINE__);
            } else {
                // Move to the next item
                prev = item;
                item = item->next;
            }
        }
        // Ensure the table entry is reset
        ht->table[i] = NULL;
    }
}
unsigned long hash(const char *macro_name) {
    unsigned long hash = 5381;

    int c;
    while ((c = *macro_name++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash % TABLE_SIZE;
}
int get_address(HashTable *ht, const char *key) {
    unsigned long idx = hash(key);
    HashItem *entry = ht->table[idx];


    while (entry != NULL) {
        if (strcmp(entry->name, key) == 0 && entry->type == TYPE_LABEL) {
            return entry->data.label.address;
        }
        entry = entry->next;
    }

    return -1;
}
