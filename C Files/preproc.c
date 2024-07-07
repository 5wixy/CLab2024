// preproc.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Header Files/globals.h"
#include "../Header Files/preproc.h"
#include "../Header Files/helper.h"
#include "../Header Files/hash_table.h"
#include "../Header Files/validation.h"
#include "../Header Files/macro.h"

int open_read_file(char *file_name) {
    FILE *fp;
    char str[MAX_LINE_LEN];
    int line_count;
    HashTable table;
    char *macro_content;
    int success;
    char *keys[4];
    int counter = 0; //macro encounter counter for testing
    fpos_t pos;


   // printf("Opening file: %s\n", file_name);  // Debug print

    fp = fopen(file_name, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    line_count = 0;
    while (fgets(str, sizeof(str), fp)) {
        line_count++;

        // Print the line to the standard output
        if (strcmp(strtok(str, " "), "macr") == 0) {
            counter++;
            int macro_line = line_count;
            char *macro_name = strtok(NULL, " ");

            if((is_valid_macro_name(macro_name)) == 0){
                printf("\nMacro name is illegal");

            }
            fgetpos(fp, &pos);


            macro_content = save_macro_content(fp, &pos, &line_count);
           // printf(macro_content);
            if (macro_content == NULL) {
                success = 0;
                continue;
            }
            initHashTable(&table);
            insert(&table,macro_name,macro_content);


            printf(get(&table,macro_name));
            fsetpos(fp, &pos);
        }

    }


    fclose(fp);
    return 0;
}
char *save_macro_content(FILE *fp, fpos_t *pos, int *line_count) {
    int macro_length = 0;
    char *macro = NULL;
    char str[MAX_LINE_LEN];
    fpos_t start_pos;

    // Set the file pointer to the provided position
    if (fsetpos(fp, pos) != 0) {
        return NULL;
    }

    // Skip the `macr` line
    /*CHECK WHAT THE FUCK
     if (!fgets(str, MAX_LINE_LEN, fp)) {
        return NULL;
    } */
    (*line_count)++;

    // Save the starting position after skipping the `macr` line
    fgetpos(fp, &start_pos);
    // Read lines from the file until "endmacr" is encountered
    while (fgets(str, MAX_LINE_LEN, fp)) {
        (*line_count)++;
        if (strstr(str, "endmacr") != NULL) {
            break; // Exit the loop if "endmacr" is encountered
        }
        macro_length += strlen(str);
    }

    // Copy the macro content into a dynamically allocated string
    macro = copy_text(fp, &start_pos, macro_length);
    if (macro == NULL) {
        return NULL;
    }

    return macro;
}
