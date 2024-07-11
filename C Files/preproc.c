#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Header Files/globals.h"
#include "../Header Files/preproc.h"
#include "../Header Files/helper.h"
#include "../Header Files/hash_table.h"
#include "../Header Files/validation.h"
#include "../Header Files/macro.h"

// Define MAX_LINE_LEN, is_valid_macro_name, has_extra, save_macro_content, HashTable, initHashTable, insert, get, and other necessary parts

int open_read_file(char *file_name, HashTable *table) {
    FILE *fp;
    char str[MAX_LINE_LEN];
    int line_count = 0;

    // Debug print
    printf("Opening file: %s\n", file_name);

    fp = fopen(file_name, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    while (fgets(str, sizeof(str), fp)) {
        line_count++;
        if (strncmp(str, "macr", 4) == 0) {
            printf("Macro found at line %d: %s\n", line_count, str);
            process_macro(fp, str, line_count, table);
        }
    }

    fclose(fp);
    return 0;
}

void process_macro(FILE *fp, char *str, int line_count, HashTable *table) {
    fpos_t pos;
    char *macro_content;
    int success;
    char *macro_name = strtok(str + 5, " \n");

    printf("Processing macro: %s\n", macro_name);

    if (is_name_too_long(macro_name)) {
        printf("Macro name too long");
        return;
    }

    if (is_valid_macro_name(macro_name) == 0) {
        printf("\nMacro name is illegal");
        return;
    }

    if (has_extra(macro_name) == 1) {
        printf("\nEXTRA TEXT IN MACRO DECLARATION");
        return;
    }

    fgetpos(fp, &pos);
    macro_content = save_macro_content(fp, &pos, &line_count);
    if (macro_content == NULL) {
        success = 0;
        return;
    }

    insert(table, macro_name, macro_content);
    printf("Macro inserted: %s -> %s\n", macro_name, get(table, macro_name));
    fsetpos(fp, &pos);
}

char *save_macro_content(FILE *fp, fpos_t *pos, int *line_count) {
    int macro_length = 0;
    char *macro = NULL;
    char str[MAX_LINE_LEN];
    fpos_t start_pos;

    if (fsetpos(fp, pos) != 0) {
        return NULL;
    }
    (*line_count)++;

    fgetpos(fp, &start_pos);
    while (fgets(str, MAX_LINE_LEN, fp)) {
        (*line_count)++;
        if (strstr(str, "endmacr") != NULL) {
            break;
        }
        macro_length += strlen(str);
    }

    macro = copy_text(fp, &start_pos, macro_length);
    print_ascii_values(macro);
    macro = remove_newline(macro);
    if (macro == NULL) {
        return NULL;
    }
    return macro;
}

void remove_macros(FILE *original, FILE *copied) {
    char line[1024];
    int inside_macro = 0;

    while (fgets(line, sizeof(line), original)) {
        if (strncmp(line, "macr", 4) == 0) {
            inside_macro = 1;
            continue;
        }

        if (inside_macro && strncmp(line, "endmacr", 7) == 0) {
            inside_macro = 0;
            continue;
        }

        if (inside_macro) {
            continue;
        }

        fputs(line, copied);
    }

    printf("File copied with macro definitions removed.\n");
}

void insert_macros(const char* source_file, const char* destination_file, HashTable *table) {
    FILE* original, * copied;
    char line[MAX_LINE_LEN];

    original = fopen(source_file, "r");
    if (original == NULL) {
        perror("Error opening source file");
        exit(EXIT_FAILURE);
    }

    copied = fopen(destination_file, "w");
    if (copied == NULL) {
        perror("Error opening destination file");
        fclose(original);
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), original)) {
        char* token = strtok(line, " \n");

        while (token != NULL) {
            char* macro_content = get(table, token);
            if (macro_content != NULL) {
                fputs(macro_content, copied);
                //fputs("\n", copied);
            } else {
                fputs(token, copied);
                fputc(' ', copied);
            }

            token = strtok(NULL, " \n");
        }
        fputc('\n', copied);
    }

    printf("Macros expanded into the file.\n");

    fclose(original);
    fclose(copied);
}

int expand_macro(char file_name[]) {
    HashTable table;
    initHashTable(&table);
    char *temp_file, *complete;
    int remove_macros = 1;

    printf("Starting macro expansion for file: %s\n", file_name);
    open_read_file(file_name, &table);
    temp_file = create_new_file_name(file_name, ".tmp");
    copy_file_content(file_name, temp_file, remove_macros);
    complete = create_new_file_name(temp_file, ".am");
    insert_macros(temp_file, complete, &table);

    printf("Macro expansion complete.\n");
    return 0;
}
