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
    newItem->name = my_strdup(key);
    newItem->type = TYPE_MACRO;
    newItem->data.content = my_strdup(content);
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
    HashItem *newItem;
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
    newItem = (HashItem *)malloc_helper(sizeof(HashItem));
    if (!newItem) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    /* Duplicate the key for the label */
    newItem->name = my_strdup(key);
    if (!newItem->name) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    /* Set up the new label's data */
    newItem->type = TYPE_LABEL;
    newItem->data.label.address = address;
    newItem->data.label.type = type;
    newItem->data.label.label_sort = label_sort;

    /* Link the new label to the existing chain */
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

/*Free memory from table items */
void freeHashTable(HashTable *ht) {
    int i;
    if (ht == NULL || ht->table == NULL) {
        return;
    }
    for (i = 0; i < TABLE_SIZE; i++) {
        HashItem *entry = ht->table[i];
        while (entry != NULL) {
            HashItem *prev = entry;
            entry = entry->next;

            if (prev->name != NULL) {
                my_free(prev->name);
            }


            if (prev->type == TYPE_MACRO && prev->data.content != NULL) {
                my_free(prev->data.content);
            }


            my_free(prev);
        }
        ht->table[i] = NULL;
    }
}
/*Free memory from macros in table*/
void free_macros_from_table(HashTable *ht) {
    int i;
    if (ht == NULL || ht->table == NULL) {
        return;
    }
   
    for (i = 0; i < TABLE_SIZE; i++) {
        HashItem *item = ht->table[i];
        HashItem *prev = NULL;
        while (item != NULL) {
            if (item->type == TYPE_MACRO) {
                HashItem *temp = item;
                item = item->next;

                if (temp->name != NULL) {
                    my_free(temp->name);
                }
                if (temp->data.content != NULL) {
                    my_free(temp->data.content);
                }
                my_free(temp);

                if (prev != NULL) {
                    prev->next = item;
                } else {
                    ht->table[i] = item;
                }
            } else {
                prev = item;
                item = item->next;
            }
        }
    }
}
/*Hashing function for the hash table */
unsigned long hash(const char *macro_name) {
    unsigned long hash = 5381;

    int c;
    while ((c = *macro_name++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash % TABLE_SIZE;
}

