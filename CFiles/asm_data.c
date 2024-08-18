#include "../HeaderFiles/asm_data.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../HeaderFiles/helper.h"
#include "../HeaderFiles/globals.h"

void init_assembly_data(AssemblyData *ad) {
    ad->instruction_capacity = 16;
    ad->instructions = malloc_helper(ad->instruction_capacity * sizeof(char *));
    ad->instruction_sizes = malloc_helper(ad->instruction_capacity * sizeof(int));
    ad->instruction_count = 0;

    ad->data_capacity = 16;
    ad->data = malloc_helper(ad->data_capacity * sizeof(char *));
    ad->data_sizes = malloc_helper(ad->data_capacity * sizeof(int));
    ad->data_count = 0;
}

void free_assembly_data(AssemblyData *ad) {
	int i;
    if (ad == NULL) return;

    /* Free each instruction string if allocated */
    if (ad->instructions != NULL) {
        for (i = 0; i < ad->instruction_count; i++) {
            if (ad->instructions[i] != NULL) {
                my_free(ad->instructions[i]);
            }
        }
        my_free(ad->instructions);
    }

    /* Free each data string if allocated */
    if (ad->data != NULL) {
        for (i = 0; i < ad->data_count; i++) {
            if (ad->data[i] != NULL) {
                my_free(ad->data[i]);
            }
        }
        my_free(ad->data);
    }

    /* Free the sizes arrays */
    if (ad->instruction_sizes != NULL) {
        my_free(ad->instruction_sizes);
    }
    if (ad->data_sizes != NULL) {
        my_free(ad->data_sizes);
    }
}

void insert_instruction(AssemblyData *ad, char *instruction) {

    char **new_instructions; 

    /* Check if total memory (instructions + data) exceeds the maximum allowed limit */
    if (ad->instruction_count + ad->data_count >= MAX_FREE_MEMORY) {
        fprintf(stderr, "Instruction memory capacity exceeded\n");
        exit(EXIT_FAILURE);
    }

    /* Reallocate memory if instruction count exceeds current capacity */
    if (ad->instruction_count >= ad->instruction_capacity) {
        /* Prevent capacity from exceeding maximum allowed memory size */
        if (ad->instruction_capacity * 2 > MEMORY_SIZE) {
            fprintf(stderr, "Cannot reallocate memory beyond %d words\n", MEMORY_SIZE);
            exit(EXIT_FAILURE);
        }

        ad->instruction_capacity *= 2;

        /* Attempt to reallocate memory for instruction pointers */
        new_instructions = realloc(ad->instructions, ad->instruction_capacity * sizeof(char *));
        if (new_instructions == NULL) {
            fprintf(stderr, "Memory reallocation failed for instructions\n");
            exit(EXIT_FAILURE);
        }

        ad->instructions = new_instructions;
    }

    /* Allocate memory for the new instruction entry */
    ad->instructions[ad->instruction_count] = malloc_helper(WORD_SIZE * sizeof(char));
    if (ad->instructions[ad->instruction_count] == NULL) {
        fprintf(stderr, "Memory allocation failed for instruction\n");
        exit(EXIT_FAILURE);
    }

    /* Copy the instruction and ensure it fits within the allocated size */
    strncpy(ad->instructions[ad->instruction_count], instruction, WORD_SIZE);

    /* Increment the instruction count */
    ad->instruction_count++;
}

    void add_data(AssemblyData *ad, const char *data, int size) {

    char **new_data;          
    int *new_data_sizes;     

    /* Check if total memory (data + instructions) exceeds the maximum allowed limit */
    if (ad->data_count + ad->instruction_count >= MAX_FREE_MEMORY) {
        printf("Data memory capacity exceeded\n");
        exit(EXIT_FAILURE);
    }

    /* Reallocate memory if data count exceeds current capacity */
    if (ad->data_count >= ad->data_capacity) {
        /* Prevent capacity from exceeding maximum allowed memory size */
        if (ad->data_capacity * 2 > MEMORY_SIZE) {
            printf("Cannot reallocate memory beyond %d words\n", MEMORY_SIZE);
            exit(EXIT_FAILURE);
        }

        ad->data_capacity *= 2;

        /* Attempt to reallocate memory for data pointers and sizes */
        new_data = realloc(ad->data, ad->data_capacity * sizeof(char *));
        new_data_sizes = realloc(ad->data_sizes, ad->data_capacity * sizeof(int));

        if (new_data == NULL || new_data_sizes == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }

        ad->data = new_data;
        ad->data_sizes = new_data_sizes;
    }

    /* Allocate memory for the new data entry */
    ad->data[ad->data_count] = malloc_helper(size + 1);
    if (ad->data[ad->data_count] == NULL) {
        fprintf(stderr, "Memory allocation failed for data\n");
        exit(EXIT_FAILURE);
    }

    /* Copy the data and null-terminate the string */
    strncpy(ad->data[ad->data_count], data, size);
    ad->data[ad->data_count][size] = '\0';

    /* Store the size and increment the data count */
    ad->data_sizes[ad->data_count] = size;
    ad->data_count++;
}
