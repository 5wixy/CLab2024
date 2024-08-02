#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include "../Header Files/validation.h"
#include "../Header Files/globals.h"
#include "../Header Files/am_handler.h"
#include "../Header Files/preproc.h"
#include "../Header Files//helper.h"
#include "../Header Files/globals.h"

char *copy_text(FILE *fp, fpos_t *pos, int str_len) {
    char *str;

    // Set the file pointer to the provided position
    if (fsetpos(fp, pos) != 0) {
        return NULL;
    }

    // Allocate memory for the content plus a null terminator
    str = malloc((str_len + 1) * sizeof(char));
    if (str == NULL) {
        return NULL;
    }

    // Read the specified number of characters from the file
    fread(str, sizeof(char), str_len, fp);
    str[str_len] = '\0'; // Null-terminate the string

    return str;
}


void *malloc_helper(long object_size){
    void *obj_ptr = malloc(object_size);
    if(obj_ptr == NULL){
        //ERROR
    }
    return obj_ptr;

}
int detect_addressing_method(char *arg) {
    if (arg[0] == '#') {
        // Immediate addressing (method 0)
        return 0;
    } else if (arg[0] == '*') {
        // Memory-indirect addressing (method 2)
        return 2;
    } else if ((is_reg_name(arg))) {
        // Direct register addressing (method 3)
        return 3;
    } else {
        // Direct addressing (method 1)
        return 1;
    }
}

char* remove_trailing_newline(char *str) {
    // Find the position of the newline character
    size_t len = strcspn(str, "\n");

    // Allocate memory for the new string
    char *new_str = (char*)malloc((len + 1) * sizeof(char));
    if (new_str == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    // Copy the characters up to the newline
    strncpy(new_str, str, len);

    // Null-terminate the new string
    new_str[len] = '\0';

    return new_str;
}




// Function to copy file content from source to destination
void copy_file_content(const char *source_file, const char *destination_file, int remove_macros_flag) {
    FILE *original, *copied;
    char line[1024];
    int remove_macros_detected = 0;

    // Open source file in read mode
    original = fopen(source_file, "r");
    if (original == NULL) {
        perror("Error opening source file");
        exit(EXIT_FAILURE);
    }

    // Open destination file in write mode
    copied = fopen(destination_file, "w");
    if (copied == NULL) {
        perror("Error opening destination file");
        fclose(original);
        exit(EXIT_FAILURE);
    }

    // If remove_macros_flag is set, call the remove_macros function
    if (remove_macros_flag) {
        remove_macros(original, copied);
    } else {
        // Copy contents from source file to destination file
        while (fgets(line, sizeof(line), original)) {
            fputs(line, copied);
        }
        printf("File copied successfully without removing macro definitions.\n");
    }

    // Close both files
    fclose(original);
    fclose(copied);
}


char *create_new_file_name(char *file_name, char *ending) {
    char *c, *new_file_name;
    new_file_name = malloc_helper(MAX_LINE_LEN * sizeof(char));
    strcpy(new_file_name, file_name);
    /* deleting the file name if a '.' exists and forth */
    if ((c = strchr(new_file_name, '.')) != NULL) {
        *c = '\0';
    }
    /* adds the ending of the new file name */
    strcat(new_file_name, ending);
    return new_file_name;
}

void close_resources(int num_args, ...) {
    int i;
    FILE *fp;
    char *str, *type;
    va_list args;

    va_start(args, num_args);

    for (i = 0; i < num_args; i++) {
        type = va_arg(args, char*);

        if (strcmp(type, "FILE") == 0) {
            fp = va_arg(args, FILE*);
            fclose(fp);
        } else if (strcmp(type, "STRING") == 0) {
            str = va_arg(args, char*);
            remove(str);
            free(str);
        }
    }

    va_end(args);
}
void print_ascii_values(char *str) {
    while (*str) {
        printf("Character: %c, ASCII value: %d\n", *str, (int)*str);
        str++;
    }
}
char* remove_newline(char *str) {
    // Find the last occurrence of the newline character
    char *last_newline = strrchr(str, '\n');

    // If no newline character is found, return a copy of the original string
    if (last_newline == NULL) {
        char *copy = strdup(str);
        if (copy == NULL) {
            perror("Failed to allocate memory");
            exit(EXIT_FAILURE);
        }
        return copy;
    }

    // Calculate the length of the new string
    size_t len = strlen(str) - 1;

    // Allocate memory for the new string
    char *new_str = (char*)malloc((len + 1) * sizeof(char));
    if (new_str == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    // Copy the original string up to the last newline character
    strncpy(new_str, str, last_newline - str);

    // Copy the rest of the original string after the last newline character
    strcpy(new_str + (last_newline - str), last_newline + 1);

    return new_str;
}



