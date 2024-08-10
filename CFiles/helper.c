#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include "../HeaderFiles/validation.h"
#include "../HeaderFiles/globals.h"
#include "../HeaderFiles/am_handler.h"
#include "../HeaderFiles/preproc.h"
#include "../HeaderFiles//helper.h"
#include "../HeaderFiles/globals.h"
#include "../HeaderFiles/data_strct.h"

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
void *allocations[100];
int alloc_count = 0;
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


void *malloc_helper(long object_size) {
    void *obj_ptr = malloc(object_size);
    if (obj_ptr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1); /* Exit the program on memory allocation failure */
    }

    return obj_ptr;
}

int detect_addressing_method(char *operand) {
    if(operand == NULL) {
        return -1;
    }
   else if (operand[0] == '#') {
        return IMMEDIATE; /* Immediate value */
    } else if (operand[0] == '*') {
        return REGISTER_POINTER; /* Register indirect addressing */
    } else if (operand[0] == 'r' && isdigit(operand[1])) {
        return REGISTER_DIRECT; /* Register direct addressing */
    }


    else {
        return DIRECT; /* Direct addressing (label by name) */
    }
}


int get_register_value(const char *register_name) {
    remove_commas(register_name);
    trim_spaces(register_name);
    if(register_name[0] == '*'){
        register_name++;
    }
    int num_registers = sizeof(register_table) / sizeof(register_table[0]);
    int i;
    for (i = 0; i < num_registers; i++) {
        if (strcmp(register_name, register_table[i].name) == 0) {
            return register_table[i].value;
        }
    }
    /* Return -1 if the register is not found */
    return -1;
}
void trim_spaces(char *str) {
    char *end;

    /* Trim leading spaces */
    while (isspace((unsigned char)*str)) str++;

    /* If all spaces or empty string */
    if (*str == 0) {
        str[0] = '\0';
        return;
    }

    /*/ Trim trailing spaces */
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    /* Write new null terminator */
    *(end + 1) = 0;
}
void to_binary_string(int num, int bits, char *output) {
    int i;
    for (i = bits - 1; i >= 0; i--) {
        output[bits - 1 - i] = (num & (1 << i)) ? '1' : '0';
    }
    output[bits] = '\0';

}
void remove_trailing_newline(char *str) {
    size_t len = strlen(str);

    /* Check if the last character is a newline and remove it */
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';  /* Replace newline with null terminator */
    }
}
void ascii_to_15_bit_binary(char ch, char *binary_str, int max_len) {
    int i;

    /* Ensure the buffer is zeroed out */
    memset(binary_str, '0', max_len);
    binary_str[WORD_SIZE] = '\0';  /* Null-terminate the string */

    unsigned char byte = (unsigned char)ch;
    for (i = 7; i >= 0; i--) {
        binary_str[14 - i] = (byte & (1 << i)) ? '1' : '0';
    }
}

void to_binary(short num, char *binary_output) {
    int i;
    for (i = 0; i < WORD_SIZE; ++i) {
        binary_output[i] = (num & (1 << (WORD_SIZE-1 - i))) ? '1' : '0';
    }
    binary_output[WORD_SIZE] = '\0'; /* Null-terminate the binary string */
}
void to_binary_address(short num, char *binary_output) {
    int i;
    for (i = 0; i < 12; ++i) {
        binary_output[i] = (num & (1 << (12-1 - i))) ? '1' : '0';
    }
    binary_output[12] = '\0'; /* Null-terminate the binary string */
}
void update_first_12_bits(char *current_binary, const char *new_12_bits) {
    if (current_binary == NULL || new_12_bits == NULL) {
        fprintf(stderr, "Error: NULL pointer passed to update_first_12_bits\n");
        return;
    }

    /* Ensure the new bits are exactly 12 bits long */
    if (strlen(new_12_bits) != 12) {
        fprintf(stderr, "Error: new_12_bits should be exactly 12 bits long\n");
        return;
    }

    /* Copy the new 12 bits into the start of the current binary string */
    memcpy(current_binary, new_12_bits, 12);

    /* Null-terminate the string */
    current_binary[12] = '\0';
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
            /* Handle invalid method index if necessary */
            strcpy(output, "0000");  /* Default to an invalid or error code */
            break;
    }
}

void remove_trailing_spaces(char *str) {
    if (str == NULL) return;  /* Check for NULL pointer */

    int length = strlen(str);

    /* Iterate from the end of the string and trim spaces */
    while (length > 0 && str[length - 1] == ' ') {
        str[length - 1] = '\0';
        length--;
    }
}

void copy_file_content(const char *source_file, const char *destination_file, int remove_macros_flag) {
    FILE *original, *copied;
    char line[1024];
    int remove_macros_detected = 0;

    /* Open source file in read mode */
    original = fopen(source_file, "r");
    if (original == NULL) {
        perror("Error opening source file");
        exit(EXIT_FAILURE);
    }

    /* Open destination file in write mode */
    copied = fopen(destination_file, "w");
    if (copied == NULL) {
        perror("Error opening destination file");
        fclose(original);
        exit(EXIT_FAILURE);
    }

    /* If remove_macros_flag is set, call the remove_macros function */
    if (remove_macros_flag) {
        remove_macros(original, copied);
    } else {
        /* Copy contents from source file to destination file */
        while (fgets(line, sizeof(line), original)) {
            fputs(line, copied);
        }
        printf("File copied successfully without removing macro definitions.\n");
    }


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

/*-------------------------------------------------------------------------------------------*/

char* extract_label(char *line) {
    char *label_end = strchr(line, ':');
    if (label_end) {
        int label_len = label_end - line;
        char *label = malloc_helper(label_len + 1);
        if (label) {
            strncpy(label, line, label_len);
            label[label_len] = '\0';
            return label;
        }
    }
    return NULL;
}

int extract_opcode(char *line) {
    // Find the end of the label, if present
    char *label_end = strchr(line, ':');
    // Start after the label or at the beginning of the line
    char *start = label_end ? label_end + 1 : line;
    // Skip leading spaces or tabs
    while (*start == ' ' || *start == '\t') {
        start++;
    }
    // Find the end of the opcode
    char *opcode_end = start;
    while (*opcode_end != ' ' && *opcode_end != '\t' && *opcode_end != '\0') {
        opcode_end++;
    }
    // Temporarily null-terminate the opcode
    char saved_char = *opcode_end;
    *opcode_end = '\0';
    // Match the opcode
    int opcode = match_opcodes(start);
    // Restore the character at the end of the opcode
    *opcode_end = saved_char;
    if (opcode > -1) {
        return opcode;  // Return the matched opcode
    }
    return -1;  // Return -1 if no opcode is matched
}
char* extract_src_operand(const char *line) {
    // Skip leading whitespace
    while (*line == ' ') {
        line++;
    }

    // Skip label if present
    char *label_end = strchr(line, ':');
    if (label_end) {
        line = label_end + 1;
        while (*line == ' ') {
            line++;
        }
    }


    char *opcode = strtok(line, " ");
    if (!opcode) {
        return NULL;
    }


    char *src_operand = strtok(NULL, ",");
    if (!src_operand) {
        return NULL;
    }


    while (*src_operand == ' ') {
        src_operand++;
    }

    char *dest_operand = strtok(NULL, " ");
    if (dest_operand) {

        while (*dest_operand == ' ') {
            dest_operand++;
        }
    }

    char *result = malloc_helper(strlen(src_operand) + 1);
    if (result) {
        strcpy(result, src_operand);
    }
    return result;

}
char* extract_dest_operand(const char *line) {
    char *line_copy = malloc_helper(strlen(line));
    strcpy(line_copy,line);


    while (*line_copy == ' ') {
        line_copy++;
    }


    char *label_end = strchr(line_copy, ':');
    if (label_end) {
        line_copy = label_end + 1;
        while (*line_copy == ' ') {
            line_copy++;
        }
    }


    char *opcode = strtok(line_copy, " ");
    if (!opcode) {
        free(line_copy);
        return NULL; // No opcode found
    }


    char *src_operand = strtok(NULL, ",");
    if (!src_operand) {
        return NULL;
    }

    char *remaining = strtok(NULL, "");
    if (remaining) {
        // Remove leading whitespace
        while (*remaining == ' ') {
            remaining++;
        }

        char *result = malloc_helper(strlen(remaining) + 1);
        if (result) {
            strcpy(result, remaining);
        }

        //free(line_copy);
        return result;
    }

    //free(line_copy);
    return NULL; // No destination operand found
}

LineType determine_line_type(AssemblyLine *line, const char *raw_line) {
    if (line->opcode == NULL || line->opcode == (char*)-1) {
        // Check the raw line for .data or .string
        if (strstr(raw_line, ".data")) {
            return DATA;
        } else if (strstr(raw_line, ".string")) {
            return STRING;
        }
    }

    return INSTRUCTION;
}


char** extract_string_data(AssemblyLine *line, const char *str) {
    // Find the starting position of the string after the opening quote
    int i, j;
    const char *start = strchr(str, '"');
    if (!start) return NULL; // No opening quote found

    start++; // Move past the opening quote

    // Find the end of the string (closing quote)
    const char *end = strchr(start, '"');
    if (!end) return NULL; // No closing quote found

    // Calculate the length of the string and allocate memory
    size_t length = end - start;
    line->data_count = length;
    if (length == 0) return NULL; // Empty string case

    // Allocate memory for the data_array
    char **data_array = malloc_helper(length * sizeof(char *));
    if (!data_array) return NULL;

    // Copy each character into the data_array
    for (i = 0; i < length; ++i) {
        data_array[i] = malloc_helper(2); // Allocate space for one character and null terminator
        if (data_array[i]) {
            data_array[i][0] = start[i]; // Copy the character
            data_array[i][1] = '\0'; // Null-terminate the string
        } else {
            // Handle memory allocation failure
            for (j = 0; j < i; ++j) {
                free(data_array[j]);
            }
            free(data_array);
            return NULL;
        }
    }
    return data_array;
}


char** extract_data_array(AssemblyLine *line, const char *str, int *count) {
    // Skip the label if present
    int i;
    remove_trailing_newline(str);
    const char *start = strchr(str, ':');
    if (start) {
        start++; // Move past the colon
    } else {
        start = str; // No label, start from the beginning
    }

    // Skip whitespace and the ".data" directive
    while (*start == ' ' || *start == '\t') {
        start++;
    }

    // Check for the ".data" directive
    if (strncmp(start, ".data", 5) == 0) {
        start += 5; // Skip the ".data" directive
    }

    // Skip any whitespace after the ".data" directive
    while (*start == ' ' || *start == '\t') {
        start++;
    }

    // Check if there is any data to process
    if (*start == '\0') {
        // No data found
        *count = 0;
        return NULL;
    }

    int capacity = 10; // Initial capacity, can be resized
    char **data_array = malloc_helper(capacity * sizeof(char*));
    if (!data_array) return NULL;

    int index = 0;
    char *buffer = strdup(start); // Create a modifiable copy of the input string
    if (!buffer) {
        free(data_array);
        return NULL;
    }

    char *token = strtok(buffer, ", ");
    while (token != NULL) {
        // Resize if necessary
        if (index >= capacity) {
            capacity *= 2;
            char **new_data_array = realloc(data_array, capacity * sizeof(char*));
            if (!new_data_array) {
                free(buffer);
                for (i = 0; i < index; ++i) {
                    free(data_array[i]);
                }
                free(data_array);
                return NULL;
            }
            data_array = new_data_array;
        }

        // Allocate and copy the token to the data array
        data_array[index] = malloc_helper(strlen(token) + 1);
        if (data_array[index]) {
            strcpy(data_array[index], token);
        } else {
            // Handle memory allocation failure
            free(buffer);
            for (i = 0; i < index; ++i) {
                free(data_array[i]);
            }
            free(data_array);
            return NULL;
        }
        index++;

        token = strtok(NULL, ", ");
    }

    free(buffer); // Free the modifiable copy
    *count = index; // Set the data count in the AssemblyLine struct
    return data_array;
}