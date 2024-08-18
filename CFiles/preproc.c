#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../HeaderFiles/globals.h"
#include "../HeaderFiles/preproc.h"
#include "../HeaderFiles/helper.h"
#include "../HeaderFiles/hash_table.h"
#include "../HeaderFiles/validation.h"
#include "../HeaderFiles/Errors.h"
#include "../HeaderFiles/error_checker.h"


int is_macro_declaration(char *line) {
    const char *pos = strstr(line, "macr");

    /* Skip leading whitespace*/
    skip_leading_space(&line);

    if (pos != NULL && pos == line) {
        if (isspace(pos[4]) || pos[4] == '\0' || pos[4] == '\n') {
            return 1;
        }
    }
    return 0;
}
int open_read_file(char *file_name, HashTable *table) {
    FILE *fp;
    int error_flag = 0;
    char str[MAX_LINE_LEN];
    int line_count = 1;

    fp = fopen(file_name, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    while (fgets(str, sizeof(str), fp)) {
        if (is_macro_declaration(str)) {
            if (process_macro(fp, str, line_count, table)) {
                error_flag = 1;
            }
        }
        line_count++;
    }

    fclose(fp);
    return error_flag;
}

int process_macro(FILE *fp, char *str, int line_count, HashTable *table) {
   
    fpos_t pos;
    char *macro_content;
    char *macro_name;
    char *extra;

    /* Skip leading whitespace*/
    skip_leading_space(&str);

    /* Ensure we are now at the start of "macr" */
    if (strncmp(str, "macr", 4) != 0) {
        
        print_error(ERR_ILLEGAL_MACRO_NAME, line_count);
        return ERROR;
    }

    /* Move past "macr" and any following whitespace */
    str += 4;
    while (isspace(*str)) {
        str++;
    }

    /* Extract the macro name*/
    macro_name = strtok(str, " \n");

    if (macro_name == NULL) {
      
        print_error(ERR_MACRO_NOT_FOUND, line_count);
        return ERROR;
    }

    if (is_name_too_long(macro_name)) {
        print_error(ERR_MACRO_NAME_TOO_LONG, line_count);
        return ERROR;
    }

    if (!is_valid_name(macro_name)) {
        print_error(ERR_ILLEGAL_MACRO_NAME, line_count);
        return ERROR;
    }

    if (get(table, macro_name) != NULL) {
        print_error(ERR_MACRO_ALREADY_EXISTS, line_count);
        return ERROR;
    }

    /* Check for any extra text after the macro name*/
    extra = strtok(NULL, " \n");
    if (extra != NULL) {
        print_error(ERR_EXTRA_TEXT_IN_MACRO_DECLARATION, line_count);
        return ERROR;
    }

    fgetpos(fp, &pos);
    macro_content = save_macro_content(fp, &pos, &line_count);
    if (macro_content == NULL) {
        return OK;
    }

    insert_macro(table, macro_name, macro_content);
    my_free(macro_content);

    fsetpos(fp, &pos);
    return OK;
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
    char line[MAX_LINE_LEN];
    int inside_macro = 0;

    while (fgets(line, sizeof(line), original)) {
        char *trimmed_line = line;

        /* Skip leading whitespace*/
        skip_leading_space(&trimmed_line);

        /* Check if it's a macro declaration */
        if (strncmp(trimmed_line, "macr", 4) == 0 &&
            (isspace(trimmed_line[4]) || trimmed_line[4] == '\0')) {
            inside_macro = 1;
            continue;
        }

        /* Check if it's the end of a macro*/
        if (inside_macro && strncmp(trimmed_line, "endmacr", 7) == 0 &&
            (isspace(trimmed_line[7]) || trimmed_line[7] == '\0')) {
            inside_macro = 0;
            continue;
        }

        /* Skip lines inside a macro*/
        if (inside_macro) {
            continue;
        }

        /* Write non-macro lines to the output file */
        fputs(line, copied);
    }
}

void insert_macros(const char* source_file, const char* destination_file, HashTable* table) {
    FILE* original;
    FILE* copied;
    char line[MAX_LINE_LEN];
    char* start;
    char* end;
    char token[MAX_LINE_LEN];
    char* macro_content;

    /* Open the source file for reading */
    original = fopen(source_file, "r");
    if (original == NULL) {
        perror("Error opening source file");
        exit(EXIT_FAILURE);
    }

    /* Open the destination file for writing */
    copied = fopen(destination_file, "w");
    if (copied == NULL) {
        perror("Error opening destination file");
        fclose(original);
        exit(EXIT_FAILURE);
    }

    /* Process each line in the source file */
    while (fgets(line, sizeof(line), original)) {
        /* Trim leading whitespace */
        start = line;
        while (*start == ' ' || *start == '\t') {
            start++;
        }

        /* Trim trailing whitespace and newline characters */
        end = start + strlen(start) - 1;
        while (end > start && (*end == ' ' || *end == '\t' || *end == '\n')) {
            *end = '\0';
            end--;
        }

        /* If the line is empty after trimming, write a newline to the destination file */
        if (*start == '\0') {
            fputc('\n', copied);
            continue;
        }

        /* Extract the first token from the line */
        sscanf(start, "%s", token);

        /* Check if the token matches the start of the line */
        if (strcmp(start, token) == 0) {
            /* Retrieve the macro content from the hash table */
            macro_content = get(table, token);
            if (macro_content != NULL) {
                /* If a macro is found, write its content to the destination file */
                fputs(macro_content, copied);
                fputc('\n', copied);
                continue;
            }
        }

        /* Write the original line to the destination file */
        fputs(line, copied);
        if (line[strlen(line) - 1] != '\n') {
            fputc('\n', copied);
        }
    }

    /* Close both files */
    fclose(original);
    fclose(copied);
}

int expand_macro(char file_name[], HashTable *table) {
    char *temp_file, *complete;
    int remove_macros = 1;
    int error_flag = 0;

    printf("Starting macro expansion for file: %s\n", file_name);
	/*Start macro expansion*/
    if (open_read_file(file_name, table)) {
        error_flag = 1;
    }
	/*Temporary file for correct insertion and removal*/
    temp_file = create_new_file_name(file_name, ".tmp");
    if (temp_file == NULL) {
        fprintf(stderr, "Error creating temporary file name.\n");
        return 1;
    }

    copy_file_content(file_name, temp_file, remove_macros);

    if (!error_flag) {
        complete = create_new_file_name(temp_file, ".am");
        if (complete == NULL) {
            fprintf(stderr, "Error creating complete file name.\n");
            my_free(temp_file);
            return 1;
        }

        insert_macros(temp_file, complete, table);



        my_free(complete);

        printf("Macro expansion complete.\n");
    } else {
        printf("Cannot create .am file due to errors in macro expansion\n");
    }

	/*Remove temp file*/
    if (remove(temp_file) != 0) {
        perror("Error deleting temporary file");
    }

    my_free(temp_file);
    return error_flag;
}
