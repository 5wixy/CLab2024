#include "../HeaderFiles/asm_data.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../HeaderFiles/globals.h"

void init_assembly_data(AssemblyData *ad) {
    ad->instruction_capacity = 10;
    ad->instructions = malloc(ad->instruction_capacity * sizeof(char *));
    ad->instruction_sizes = malloc(ad->instruction_capacity * sizeof(int));
    ad->instruction_count = 0;

    ad->data_capacity = 10;
    ad->data = malloc(ad->data_capacity * sizeof(char *));
    ad->data_sizes = malloc(ad->data_capacity * sizeof(int));
    ad->data_count = 0;
}

void free_assembly_data(AssemblyData *ad) {
    if (ad == NULL) return;

    int i;

    // Free each instruction string if allocated
    if (ad->instructions != NULL) {
        for (i = 0; i < ad->instruction_count; i++) {
            if (ad->instructions[i] != NULL) {
                int size = strlen(ad->instructions[i]);
                if(size == 15){
                    continue;
                }
                free(ad->instructions[i]);
            }
        }
        free(ad->instructions);
    }

    // Free each data string if allocated
    if (ad->data != NULL) {
        for (i = 0; i < ad->data_count; i++) {
            if (ad->data[i] != NULL) {
                int size = strlen(ad->data[i]);
                if(size == 15){
                    continue;
                }
                free(ad->data[i]);
            }
        }
        free(ad->data);
    }

    // Free other allocated memory
    if (ad->instruction_sizes != NULL) {
        free(ad->instruction_sizes);
    }
    if (ad->data_sizes != NULL) {
        free(ad->data_sizes);
    }
}

void insert_instruction(AssemblyData *ad, char *instruction) {
    if (ad->instruction_count >= ad->instruction_capacity) {
        ad->instruction_capacity *= 2;
        ad->instructions = realloc(ad->instructions, ad->instruction_capacity * sizeof(char *));
        if (ad->instructions == NULL) {
            fprintf(stderr, "Memory allocation failed for instructions\n");
            exit(EXIT_FAILURE);
        }
    }

    ad->instructions[ad->instruction_count] = malloc(WORD_SIZE*sizeof(char));
    if (ad->instructions[ad->instruction_count] == NULL) {
        fprintf(stderr, "Memory allocation failed for instruction\n");
        exit(EXIT_FAILURE);
    }

    strncpy(ad->instructions[ad->instruction_count], instruction, WORD_SIZE);
    ad->instruction_count++;
}

void add_data(AssemblyData *ad, const char *data, int size) {
    if (ad->data_count >= ad->data_capacity) {
        ad->data_capacity *= 2;
        ad->data = realloc(ad->data, ad->data_capacity * sizeof(char *));
        ad->data_sizes = realloc(ad->data_sizes, ad->data_capacity * sizeof(int));
    }
    ad->data[ad->data_count] = strdup(data);
    ad->data_sizes[ad->data_count] = size;
    ad->data_count++;
}