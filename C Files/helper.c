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
void fill_nums_array(const char *line, short *nums, int count) {
    const char *ptr = line;
    char *endptr;
    int i;
    for (i = 0; i < count; i++) {
        nums[i] = (short)strtol(ptr, &endptr, 10);
        ptr = endptr;
    }
}
int detect_addressing_method(char *operand) {
    if (operand[0] == '#') {
        return IMMEDIATE; // Immediate value
    } else if (operand[0] == '*') {
        return REGISTER_POINTER; // Register indirect addressing
    } else if (operand[0] == 'r' && isdigit(operand[1])) {
        return REGISTER_DIRECT; // Register direct addressing
    } else {
        return DIRECT; // Direct addressing (label by name)
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
void ascii_to_15_bit_binary(char ch, char *binary_str, size_t max_len) {
    int i;
    if (binary_str == NULL || max_len < 16) {
        printf("Error: Invalid pointer or insufficient buffer length.\n");
        return;
    }

    // Ensure the buffer is zeroed out
    memset(binary_str, '0', max_len);
    binary_str[max_len - 1] = '\0';  // Null-terminate the string

    unsigned char byte = (unsigned char)ch;
    for (i = 7; i >= 0; i--) {
        binary_str[14 - i] = (byte & (1 << i)) ? '1' : '0';
    }
}

// Function to count the number of 15-bit words needed
size_t count_15_bit_words(size_t total_length) {
    // Each 15-bit word corresponds to 2 bytes
    size_t num_words = (total_length + 1) / 2;  // +1 to account for null terminator
    if (total_length % 2 != 0) {
        num_words++;
    }
    return num_words;
}
void extract_operands(const char *line, char *source, char *dest) {
    // This function assumes operands are separated by a comma
    // and are placed after the opcode
    char line_copy[MAX_LINE_LEN];
    strcpy(line_copy, line);

    // Skip the opcode and whitespace
    char *opcode = strtok(line_copy, " ,");
    if (opcode != NULL) {
        // Extract source operand
        char *operand = strtok(NULL, " ,");
        if (operand != NULL) {
            strcpy(source, operand);
            // Extract destination operand
            operand = strtok(NULL, " ,");
            if (operand != NULL) {
                strcpy(dest, operand);
            }
        }
    }
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



