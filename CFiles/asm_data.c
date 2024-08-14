#include "../HeaderFiles/asm_data.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../HeaderFiles/helper.h"
#include "../HeaderFiles/globals.h"

void init_assembly_data(AssemblyData *ad) {
    ad->instruction_capacity = 16;
    ad->instructions = malloc_helper(ad->instruction_capacity * sizeof(char *),__FILE_NAME__,__LINE__);
    ad->instruction_sizes = malloc_helper(ad->instruction_capacity * sizeof(int),__FILE_NAME__,__LINE__);
    ad->instruction_count = 0;

    ad->data_capacity = 16;
    ad->data = malloc_helper(ad->data_capacity * sizeof(char *),__FILE_NAME__,__LINE__);
    ad->data_sizes = malloc_helper(ad->data_capacity * sizeof(int),__FILE_NAME__,__LINE__);
    ad->data_count = 0;
}

void free_assembly_data(AssemblyData *ad) {
    if (ad == NULL) return;

    int i;

    /* Free each instruction string if allocated */
    if (ad->instructions != NULL) {
        for (i = 0; i < ad->instruction_count; i++) {
            if (ad->instructions[i] != NULL) {
                my_free(ad->instructions[i], __FILE_NAME__, __LINE__);
            }
        }
        my_free(ad->instructions, __FILE_NAME__, __LINE__);
    }

    /* Free each data string if allocated */
    if (ad->data != NULL) {
        for (i = 0; i < ad->data_count; i++) {
            if (ad->data[i] != NULL) {
                my_free(ad->data[i], __FILE_NAME__, __LINE__);
            }
        }
        my_free(ad->data, __FILE_NAME__, __LINE__);
    }

    /* Free the sizes arrays */
    if (ad->instruction_sizes != NULL) {
        my_free(ad->instruction_sizes, __FILE_NAME__, __LINE__);
    }
    if (ad->data_sizes != NULL) {
        my_free(ad->data_sizes, __FILE_NAME__, __LINE__);
    }
}

void insert_instruction(AssemblyData *ad, char *instruction) {
    if (ad->instruction_count + ad->data_count >= MAX_FREE_MEMORY) {
        fprintf(stderr, "Instruction memory capacity exceeded\n");
        exit(EXIT_FAILURE);
    }

    if (ad->instruction_count >= ad->instruction_capacity) {
        if (ad->instruction_capacity * 2 > MEMORY_SIZE) {
            fprintf(stderr, "Cannot reallocate memory beyond %d words\n", MEMORY_SIZE);
            exit(EXIT_FAILURE);
        }
    }
        if (ad->instruction_count >= ad->instruction_capacity) {
            ad->instruction_capacity *= 2;
            char **new_instructions = realloc(ad->instructions, ad->instruction_capacity * sizeof(char *));
            if (new_instructions == NULL) {
                fprintf(stderr, "Memory allocation failed for instructions\n");
                exit(EXIT_FAILURE);
            }
            ad->instructions = new_instructions;
        }

        ad->instructions[ad->instruction_count] = malloc_helper(WORD_SIZE * sizeof(char), __FILE_NAME__, __LINE__);
        if (ad->instructions[ad->instruction_count] == NULL) {
            fprintf(stderr, "Memory allocation failed for instruction\n");
            exit(EXIT_FAILURE);
        }

        strncpy(ad->instructions[ad->instruction_count], instruction, WORD_SIZE);
        ad->instruction_count++;
    }

    void add_data(AssemblyData *ad, const char *data, int size) {
        if (ad->data_count + ad->instruction_count >= MAX_FREE_MEMORY) {
            printf("Data memory capacity exceeded\n");
            exit(EXIT_FAILURE);
        }

        if (ad->data_count >= ad->data_capacity) {
            if (ad->data_capacity * 2 > MEMORY_SIZE) {
                printf( "Cannot reallocate memory beyond %d words\n", MEMORY_SIZE);
                exit(EXIT_FAILURE);
            }
        }

        // Reallocate memory if necessary
        if (ad->data_count >= ad->data_capacity) {
            ad->data_capacity *= 2;
            char **new_data = realloc(ad->data, ad->data_capacity * sizeof(char *));
            int *new_data_sizes = realloc(ad->data_sizes, ad->data_capacity * sizeof(int));

            if (new_data == NULL || new_data_sizes == NULL) {
                fprintf(stderr, "Memory allocation failed for data or data_sizes\n");
                exit(EXIT_FAILURE);
            }

            ad->data = new_data;
            ad->data_sizes = new_data_sizes;
        }

        // Allocate memory for the data
        ad->data[ad->data_count] = malloc_helper(size + 1, __FILE__, __LINE__);
        if (ad->data[ad->data_count] == NULL) {
            fprintf(stderr, "Memory allocation failed for data\n");
            exit(EXIT_FAILURE);
        }

        // Copy data and size
        strncpy(ad->data[ad->data_count], data, size);
        ad->data[ad->data_count][size] = '\0'; // Ensure null-termination
        ad->data_sizes[ad->data_count] = size;
        ad->data_count++;
    }
