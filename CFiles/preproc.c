#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../HeaderFiles/globals.h"
#include "../HeaderFiles/preproc.h"
#include "../HeaderFiles/helper.h"
#include "../HeaderFiles/hash_table.h"
#include "../HeaderFiles/validation.h"
#include "../HeaderFiles/macro.h"
#include "../HeaderFiles/Errors.h"


int open_read_file(char *file_name, HashTable *table) {
    FILE *fp;
    char str[MAX_LINE_LEN];
    int line_count = 1;



    fp = fopen(file_name, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    while (fgets(str, sizeof(str), fp)) {
        line_count++;
        char *pos = strstr(str, "macr");
        if (pos != NULL && (pos[4] == ' ' || pos[4] == '\t' || pos[4] == '\n' || pos[4] == '\0')) {
            process_macro(fp, str, line_count, table);
        }
    }

    fclose(fp);
    return 0;
}

void process_macro(FILE *fp, char *str, int line_count, HashTable *table) {
    fpos_t pos;
    char *macro_content;
    char *macro_name = strtok(str + 5, " \n");

    if (is_name_too_long(macro_name)) {
        print_error(ERR_MACRO_NAME_TOO_LONG,line_count);
        return;
    }

    if (is_valid_macro_name(macro_name) == 0) {
        print_error(ERR_ILLEGAL_MACRO_NAME,line_count);
        return;
    }

    if (has_extra(macro_name) == 1) {
        print_error(ERR_EXTRA_TEXT_IN_MACRO_DECLARATION,line_count);
        return;
    }

    fgetpos(fp, &pos);
    macro_content = save_macro_content(fp, &pos, &line_count);
    if (macro_content == NULL) {
        return;
    }

    insert_macro(table, macro_name, macro_content);
    my_free(macro_content, __FILE__, __LINE__);

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
    remove_trailing_newline(macro);
    if (macro == NULL) {
        return NULL;
    }
    return macro;
}

void remove_macros(FILE *original, FILE *copied) {
    char line[1024];
    int inside_macro = 0;

    while (fgets(line, sizeof(line), original)) {
        // Check if the line starts with "macr"
        if (strncmp(line, "macr", 4) == 0 && (line[4] == ' ' || line[4] == '\t' || line[4] == '\n' || line[4] == '\0')) {
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

}

void insert_macros(const char* source_file, const char* destination_file, HashTable *table) {
    FILE* original, * copied;
    char line[MAX_LINE_LEN];
    int i, j;

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
        i = 0;
        while (line[i] != '\0') {
            // Skip leading spaces
            if (line[i] == ' ' || line[i] == '\t') {
                fputc(line[i], copied);
                i++;
                continue;
            }

            // Extract token
            j = 0;
            char token[MAX_LINE_LEN];
            while (line[i] != '\0' && line[i] != ' ' && line[i] != '\n' && line[i] != '\t') {
                token[j++] = line[i++];
            }
            token[j] = '\0';

            // Check if token is a macro
            char* macro_content = get(table, token);
            if (macro_content != NULL) {
                fputs(macro_content, copied);
            } else {
                fputs(token, copied);
            }

            // Write the space or newline character
            if (line[i] == ' ' || line[i] == '\t' || line[i] == '\n') {
                fputc(line[i], copied);
            }

            if (line[i] != '\0') {
                i++;
            }
        }
    }

    fclose(original);
    fclose(copied);
}

int expand_macro(char file_name[], HashTable *table) {
    char *temp_file, *complete,*messy_file;
    int remove_macros = 1;

    printf("Starting macro expansion for file: %s\n", file_name);

    open_read_file(file_name, table);

    temp_file = create_new_file_name(file_name, ".tmp");
    if (temp_file == NULL) {
        fprintf(stderr, "Error creating temporary file name.\n");
        return 1;
    }

    copy_file_content(file_name, temp_file, remove_macros);
    messy_file = create_new_file_name(file_name, ".messy");

    complete = create_new_file_name(temp_file, ".am");
    if (complete == NULL) {
        fprintf(stderr, "Error creating complete file name.\n");
        my_free(temp_file, __FILE__, __LINE__);
        return 1;
    }

    insert_macros(temp_file, messy_file, table);
    clean_file(messy_file, complete);
    /* Delete the temporary file */
    if (remove(temp_file) != 0) {
        perror("Error deleting temporary file");
        return 1;
    }
    remove(messy_file);

    /* Free memory for file names */
    my_free(messy_file, __FILE__, __LINE__);
    my_free(temp_file, __FILE__, __LINE__);
    my_free(complete, __FILE__, __LINE__);



    printf("Macro expansion complete.\n");
    return 0;
}