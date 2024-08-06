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
Register register_table[] = {
        {"r0", R0_VALUE},
        {"r1", R1_VALUE},
        {"r2", R2_VALUE},
        {"r3", R3_VALUE},
        {"r4", R4_VALUE},
        {"r5", R5_VALUE},
        {"r6", R6_VALUE},
        {"r7", R7_VALUE},
        // Add other registers here...
};
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
        // Skip non-numeric characters and delimiters
        while (*ptr && (!isdigit(*ptr) && *ptr != '-' && *ptr != '+')) {
            ptr++;
        }

        if (*ptr == '\0') {
            // End of string reached, no more numbers to read
            break;
        }

        nums[i] = (short)strtol(ptr, &endptr, 10);

        if (ptr == endptr) {
            // No valid conversion, handle the error if necessary
            break;
        }

        ptr = endptr;  // Move the pointer to the next character after the last parsed number

        // Skip any whitespace or delimiters
        while (*ptr == ' ' || *ptr == '\t' || *ptr == ',') {
            ptr++;
        }
    }
}
int detect_addressing_method(char *operand) {
    remove_commas(operand);
    if (operand[0] == '#') {
        return IMMEDIATE; // Immediate value
    } else if (operand[0] == '*') {
        return REGISTER_POINTER; // Register indirect addressing
    } else if (operand[0] == 'r' && isdigit(operand[1])) {
        return REGISTER_DIRECT; // Register direct addressing
    } else if(operand[0] == '\0'){
        return -1;
    }



    else {
        return DIRECT; // Direct addressing (label by name)
    }
}




int get_register_value(const char *register_name) {
    remove_commas(register_name);
    trim_spaces(register_name);
    size_t num_registers = sizeof(register_table) / sizeof(register_table[0]);
    size_t i;
    for (i = 0; i < num_registers; i++) {
        if (strcmp(register_name, register_table[i].name) == 0) {
            return register_table[i].value;
        }
    }
    // Return -1 if the register is not found
    return -1;
}
void trim_spaces(char *str) {
    char *end;

    // Trim leading spaces
    while (isspace((unsigned char)*str)) str++;

    // If all spaces or empty string
    if (*str == 0) {
        str[0] = '\0';
        return;
    }

    // Trim trailing spaces
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator
    *(end + 1) = 0;
}
void to_binary_string(int num, int bits, char *output) {
    int i;
    for (i = bits - 1; i >= 0; i--) {
        output[bits - 1 - i] = (num & (1 << i)) ? '1' : '0';
    }
    output[bits] = '\0';

}
void int_to_binary(int value, char *binary_str, int length) {
    int i;
    for (i = length - 1; i >= 0; i--) {
        binary_str[i] = (value & (1 << (length - 1 - i))) ? '1' : '0';
    }
    binary_str[length] = '\0'; // Null-terminate the string
}
void remove_trailing_newline(char *str) {
    size_t len = strlen(str);

    // Check if the last character is a newline and remove it
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';  // Replace newline with null terminator
    }
}
void ascii_to_15_bit_binary(char ch, char *binary_str, size_t max_len) {
    int i;

    // Ensure the buffer is zeroed out
    memset(binary_str, '0', max_len);
    binary_str[max_len - 1] = '\0';  // Null-terminate the string

    unsigned char byte = (unsigned char)ch;
    for (i = 7; i >= 0; i--) {
        binary_str[14 - i] = (byte & (1 << i)) ? '1' : '0';
    }
}

char* to_binary(short num) {
    int i;
    char* binary = (char*)malloc(WORD_SIZE); // 32 bits + null terminator
    if (binary == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    binary[WORD_SIZE] = '\0'; // Null terminator

    for (i = WORD_SIZE-1; i >= 0; i--) {
        binary[i] = (num & 1) ? '1' : '0';
        num >>= 1;
    }

    return binary;
}
void twos_complement(char* binary) {
    int carry = 1; // Initially adding 1
    int i;
    for (i = WORD_SIZE - 1; i >= 0; i--) {
        // Flip the bit and add carry
        if (binary[i] == '1') {
            binary[i] = (carry == 1) ? '0' : '1';
        } else {
            binary[i] = (carry == 1) ? '1' : '0';
            carry = 0; // No further carry needed after adding 1
        }
    }
}
void extract_operands(const char *line, char *source, char *dest) {
    char line_copy[MAX_LINE_LEN];
    strcpy(line_copy, line);

    // Skip over the label if present (assumes label ends with ':')
    char *label_end = strchr(line_copy, ':');
    if (label_end != NULL) {
        // Move past the label
        char *start_of_instruction = label_end + 1;
        // Skip leading whitespace after the label
        while (isspace(*start_of_instruction)) {
            start_of_instruction++;
        }
        strcpy(line_copy, start_of_instruction);
    }

    // Now we have the instruction part
    char *opcode = strtok(line_copy, " ,");
    if (opcode == NULL) {
        // Handle error: No opcode found
        printf("ERROR: No opcode found.\n");
        if (source != NULL) {
            source[0] = '\0';
        }
        if (dest != NULL) {
            dest[0] = '\0';
        }
        return;
    }

    // Extract the source operand
    char *operand = strtok(NULL, " ,");
    if (operand != NULL) {
        // Check if there are more operands
        char *next_operand = strtok(NULL, " ,");
        if (next_operand != NULL) {
            // Two operands present
            if (source != NULL) {
                strcpy(source, operand);
            }
            if (dest != NULL) {
                strcpy(dest, next_operand);
            }
        } else {
            // Only one operand present; treat it as the destination
            if (source != NULL) {
                source[0] = '\0';  // Ensure source is an empty string
            }
            if (dest != NULL) {
                strcpy(dest, operand);
            }
        }
    } else {
        // No operands present
        if (source != NULL) {
            source[0] = '\0';  // Ensure source is an empty string
        }
        if (dest != NULL) {
            dest[0] = '\0';    // Ensure dest is an empty string
        }
    }
}
void addressing_method_to_binary(int method, char *output) {
    switch (method) {
        case 0:
            strcpy(output, "0001");
            break;
        case 1:
            strcpy(output, "0010");
            break;
        case 2:
            strcpy(output, "0100");
            break;
        case 3:
            strcpy(output, "1000");
            break;
        default:
            // Handle invalid method index if necessary
            strcpy(output, "0000");  // Default to an invalid or error code
            break;
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



