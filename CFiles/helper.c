#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include "../HeaderFiles/validation.h"
#include "../HeaderFiles/globals.h"
#include "../HeaderFiles/am_handler.h"
#include "../HeaderFiles/preproc.h"
#include "../HeaderFiles/helper.h"
#include "../HeaderFiles/globals.h"
#include "../HeaderFiles/data_strct.h"
#include "../HeaderFiles/Errors.h"

Register register_table[] = {
        {"r0", R0_VALUE},
        {"r1", R1_VALUE},
        {"r2", R2_VALUE},
        {"r3", R3_VALUE},
        {"r4", R4_VALUE},
        {"r5", R5_VALUE},
        {"r6", R6_VALUE},
        {"r7", R7_VALUE},

};
char *copy_text(FILE *fp, fpos_t *pos, int str_len) {
    char *str;
    if (fsetpos(fp, pos) != 0) {
        return NULL;
    }


    str = malloc_helper((str_len + 1) * sizeof(char),__FILE_NAME__,__LINE__);
    if (str == NULL) {
        return NULL;
    }


    fread(str, sizeof(char), str_len, fp);
    str[str_len] = '\0';

    return str;
}


void *malloc_helper(int object_size, char *file, int line) {
    void *obj_ptr = malloc(object_size);
    if (obj_ptr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1); /* Exit the program on memory allocation failure */
    }
   // printf("Allocated %d bytes at %p (%s:%d)\n", object_size, obj_ptr, file, line);

    return obj_ptr;
}
void my_free(void* ptr, const char* file, int line) {
    //printf("Freeing memory at %p (%s:%d)\n", ptr, file, line);
    free(ptr);
    ptr = NULL;
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
char *trim_whitespace(char *str) {
    char *end;

    // Trim leading spaces
    while (isspace((unsigned char)*str)) str++;

    // If the string is empty after trimming leading spaces, return it
    if (*str == 0)
        return str;

    // Trim trailing spaces
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Null-terminate the trimmed string
    *(end + 1) = '\0';

    return str;
}
void trim_whitespaces(char *str) {
    // Remove leading whitespace
    char *start = str;
    while (isspace((unsigned char)*start)) {
        start++;
    }

    // Remove trailing whitespace
    char *end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) {
        end--;
    }

    // Null terminate the string
    *(end + 1) = '\0';

    // Move the trimmed string to the beginning
    memmove(str, start, strlen(start) + 1);
}
void trim_file_in_place(const char *file_name) {
    FILE *original_file = fopen(file_name, "r");
    if (original_file == NULL) {
        perror("Error opening file for reading");
        exit(EXIT_FAILURE);
    }

    FILE *temp_file = tmpfile(); // Creates a temporary file
    if (temp_file == NULL) {
        perror("Error creating temporary file");
        fclose(original_file);
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LEN];

    // Read each line, trim it, and write to the temporary file
    while (fgets(line, sizeof(line), original_file)) {
        trim_whitespaces(line);

        if (strlen(line) > 0) {
            fputs(line, temp_file);
            fputc('\n', temp_file);  // Add a newline after the trimmed line
        }
    }

    // Close the original file and reopen it for writing
    fclose(original_file);
    original_file = fopen(file_name, "w");
    if (original_file == NULL) {
        perror("Error reopening original file for writing");
        fclose(temp_file);
        exit(EXIT_FAILURE);
    }

    // Rewind the temporary file to the beginning
    rewind(temp_file);

    // Copy the trimmed content from the temporary file back to the original file
    while (fgets(line, sizeof(line), temp_file)) {
        fputs(line, original_file);
    }

    // Close both files
    fclose(temp_file);
    fclose(original_file);
}
void trim_file(const char *filename) {
    char temp_filename[] = "tempfile.txt";
    FILE *original, *temp;
    char line[MAX_LINE_LEN];

    original = fopen(filename, "r");
    if (!original) {
        perror("Error opening original file");
        exit(EXIT_FAILURE);
    }

    temp = fopen(temp_filename, "w");
    if (!temp) {
        perror("Error opening temporary file");
        fclose(original);
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), original)) {
        trim_line(line);
        if (strlen(line) > 0) { // Only write non-empty lines
            fprintf(temp, "%s\n", line);
        }
    }

    fclose(original);
    fclose(temp);

    // Replace the original file with the trimmed version
    if (remove(filename) != 0) {
        perror("Error deleting original file");
        exit(EXIT_FAILURE);
    }
    if (rename(temp_filename, filename) != 0) {
        perror("Error renaming temporary file");
        exit(EXIT_FAILURE);
    }
}
void trim_file_to_temp(const char *source_file, char *temp_file) {
    FILE *original, *temp;
    char line[MAX_LINE_LEN];

    original = fopen(source_file, "r");
    if (original == NULL) {
        perror("Error opening source file");
        exit(EXIT_FAILURE);
    }

    temp = fopen(temp_file, "w");
    if (temp == NULL) {
        perror("Error opening temp file");
        fclose(original);
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), original)) {
        char *trimmed_line = trim_whitespace(line);
        if (strlen(trimmed_line) > 0) {
            fputs(trimmed_line, temp);
            fputc('\n', temp);
        }
    }

    fclose(original);
    fclose(temp);
}
void trim_line(char *line) {
    char *end;

    // Trim leading space
    while (*line == ' ' || *line == '\t') {
        line++;
    }

    // Trim trailing space
    end = line + strlen(line) - 1;
    while (end > line && (*end == ' ' || *end == '\t' || *end == '\n')) {
        *end = '\0';
        end--;
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
void clean_file(const char *input_file, const char *output_file) {
    FILE *in_fp, *out_fp;
    char line[MAX_LINE_LEN];
    int previous_char = '\n'; // Initialize as newline to handle leading spaces
    int inside_string = 0; // Flag to track if we are inside a string

    in_fp = fopen(input_file, "r");
    if (in_fp == NULL) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    out_fp = fopen(output_file, "w");
    if (out_fp == NULL) {
        perror("Error opening output file");
        fclose(in_fp);
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), in_fp)) {
        // Skip lines that start with a semicolon
        if (line[0] == ';') {
            continue;
        }

        int i = 0;
        int has_written_char = 0; // Flag to check if we wrote a non-space character

        while (line[i] != '\0') {
            if (line[i] == '"') {
                // Toggle the inside_string flag when encountering a double quote
                inside_string = !inside_string;
                fputc(line[i], out_fp);
                has_written_char = 1;
            } else if (inside_string) {
                // If inside a string, directly write the character
                fputc(line[i], out_fp);
                has_written_char = 1;
            } else {
                // If not inside a string, process normally
                if (line[i] == ' ' || line[i] == '\t') {
                    // Skip spaces or tabs if the last character written was also a space or a newline
                    if (previous_char != ' ' && previous_char != '\n') {
                        fputc(' ', out_fp);
                        previous_char = ' ';
                    }
                } else if (line[i] == '\n') {
                    // If it's a newline, only write it if a character has been written
                    if (has_written_char) {
                        fputc('\n', out_fp);
                        previous_char = '\n';
                    }
                    has_written_char = 0; // Reset for the next line
                } else {
                    // Write non-space, non-newline characters normally
                    fputc(line[i], out_fp);
                    previous_char = line[i];
                    has_written_char = 1;
                }
            }
            i++;
        }
    }

    fclose(in_fp);
    fclose(out_fp);
}

char *create_new_file_name(char *file_name, char *ending) {
    char *c, *new_file_name;
    new_file_name = malloc_helper(MAX_LINE_LEN * sizeof(char),__FILE_NAME__,__LINE__);
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
            my_free(str,__FILE_NAME__,__LINE__);
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

    /* If no newline character is found, return a copy of the original string */
    if (last_newline == NULL) {
        char *copy = strdup(str);
        if (copy == NULL) {
            perror("Failed to allocate memory");
            exit(EXIT_FAILURE);
        }
        return copy;
    }

    /* Calculate the length of the new string */
    size_t len = strlen(str) - 1;

    /* Allocate memory for the new string */
    char *new_str = (char*)malloc((len + 1) * sizeof(char));
    if (new_str == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    /* Copy the original string up to the last newline character */
    strncpy(new_str, str, last_newline - str);

    /* Copy the rest of the original string after the last newline character */
    strcpy(new_str + (last_newline - str), last_newline + 1);

    return new_str;
}

/*-------------------------------------------------------------------------------------------*/

char* extract_label(char *line) {



        char *label_end = strchr(line, ':');
        if (label_end) {
            int label_len = label_end - line;
            char *label = malloc_helper(label_len + 1, __FILE_NAME__, __LINE__);
            if (label) {
                strncpy(label, line, label_len);
                label[label_len] = '\0';
                return label;
            }
        }

    //print_error(ERR_ILLEGAL_LABEL_NAME,0);

    return NULL;
}

int extract_opcode(char *line) {
    /* Find the end of the label, if present */
    char *label_end = strchr(line, ':');
    /* Start after the label or at the beginning of the line */
    char *start = label_end ? label_end + 1 : line;
    /* Skip leading spaces or tabs */
    while (*start == ' ' || *start == '\t') {
        start++;
    }
    /* Find the end of the opcode */
    char *opcode_end = start;
    while (*opcode_end != ' ' && *opcode_end != '\t' && *opcode_end != '\0') {
        opcode_end++;
    }
    /* Temporarily null-terminate the opcode */
    char saved_char = *opcode_end;
    *opcode_end = '\0';
    /* Match the opcode */
    int opcode = match_opcodes(start);
    /* Restore the character at the end of the opcode */
    *opcode_end = saved_char;
    if (opcode > -1) {
        return opcode;  /* Return the matched opcode */
    }
    return -1;  /* Return -1 if no opcode is matched */
}
char* extract_src_operand(const char *line) {
    /* Skip leading whitespace */
    while (*line == ' ') {
        line++;
    }

    /* Skip label if present */
    char *label_end = strchr(line, ':');
    if (label_end) {
        line = label_end + 1;
        while (*line == ' ') {
            line++;
        }
    }

    /* Find the start of the src_operand */
    char *src_start = strchr(line, ' ');
    if (!src_start) {
        return NULL;
    }
    src_start++; /* Skip the space */

    /* Find the end of the src_operand */
    char *src_end = strchr(src_start, ',');
    if (!src_end) {
        src_end = strchr(src_start, '\0'); /* Handle case where there's no comma */
    }

    /* Trim trailing whitespace from src_operand */
    while (src_end > src_start && *(src_end - 1) == ' ') {
        src_end--;
    }
    if(strcmp(src_start,"") == 0){
        return NULL;
    }
    /* Null-terminate the src_operand */
    size_t length = src_end - src_start;
    char *result = malloc_helper(length + 1,__FILE_NAME__,__LINE__);
    if (!result) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }
    strncpy(result, src_start, length);
    result[length] = '\0';

    return result;
}
char* extract_dest_operand(const char *line) {
    char *line_copy = malloc_helper(strlen(line) + 1, __FILE_NAME__, __LINE__);
    if (!line_copy) {
        return NULL;
    }
    strcpy(line_copy, line);

    /* Save the original pointer for later freeing */
    char *original_line_copy = line_copy;

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
        my_free(original_line_copy,__FILE_NAME__,__LINE__);
        return NULL; /* No opcode found */
    }

    char *src_operand = strtok(NULL, ",");
    if (!src_operand) {
        my_free(original_line_copy,__FILE_NAME__,__LINE__);
        return NULL; /* No source operand found */
    }

    char *remaining = strtok(NULL, "");
    if (remaining) {
        /* Remove leading whitespace */
        while (*remaining == ' ') {
            remaining++;
        }

        char *result = malloc_helper(strlen(remaining) + 1, __FILE_NAME__, __LINE__);
        if (result) {
            strcpy(result, remaining);
        }

        my_free(original_line_copy,__FILE_NAME__,__LINE__); /* Free the original pointer */
        return result;
    }

    my_free(original_line_copy,__FILE_NAME__,__LINE__); /* Free the original pointer */
    return NULL;
}

LineType determine_line_type(AssemblyLine *line, const char *raw_line) {
    if (line->opcode == NULL || line->opcode == (char*)-1) {
        if (strstr(raw_line, ".data")) {
            return DATA;
        } else if (strstr(raw_line, ".string")) {
            return STRING;
        }
    }

    return INSTRUCTION;
}


char** extract_string_data(AssemblyLine *line, const char *str) {

    int i, j;
    const char *start = strchr(str, '"');
    if (!start) return NULL;

    start++; /* Move past the opening quote */


    const char *end = strchr(start, '"');
    if (!end) return NULL;

    /* Calculate the length of the string and allocate memory */
    size_t length = end - start;
    line->data_count = length;
    if (length == 0) return NULL;

    /* Allocate memory for the data_array */
    char **data_array = malloc_helper(length * sizeof(char *),__FILE_NAME__,__LINE__);
    if (!data_array) return NULL;

    /* Copy each character into the data_array */
    for (i = 0; i < length; ++i) {
        data_array[i] = malloc_helper(2,__FILE_NAME__,__LINE__);
        if (data_array[i]) {
            data_array[i][0] = start[i];
            data_array[i][1] = '\0';
        } else {

            for (j = 0; j < i; ++j) {
                my_free(data_array[j],__FILE_NAME__,__LINE__);
            }
            my_free(data_array,__FILE_NAME__,__LINE__);
            return NULL;
        }
    }
    return data_array;
}


char** extract_data_array(AssemblyLine *line, const char *str, int *count) {
    int i;
    remove_trailing_newline(str);

    const char *start = strchr(str, ':');
    if (start) {
        start++;
    } else {
        start = str;
    }

    while (*start == ' ' || *start == '\t') {
        start++;
    }

    if (strncmp(start, ".data", 5) == 0) {
        start += 5;
    }

    while (*start == ' ' || *start == '\t') {
        start++;
    }

    if (*start == '\0') {
        *count = 0;
        return NULL;
    }

    int capacity = 10;
    char **data_array = malloc_helper(capacity * sizeof(char*), __FILE_NAME__, __LINE__);
    if (!data_array) return NULL;

    int index = 0;
    char *buffer = strdup(start);
    if (!buffer) {
        my_free(data_array, __FILE_NAME__, __LINE__);
        return NULL;
    }

    char *token = strtok(buffer, ", ");
    while (token != NULL) {
        if (index >= capacity) {
            capacity *= 2;
            char **new_data_array = realloc(data_array, capacity * sizeof(char*));
            if (!new_data_array) {
                my_free(buffer, __FILE_NAME__, __LINE__);
                for (i = 0; i < index; ++i) {
                    my_free(data_array[i], __FILE_NAME__, __LINE__);
                }
                my_free(data_array, __FILE_NAME__, __LINE__);
                return NULL;
            }
            data_array = new_data_array;
        }

        data_array[index] = strdup(token);
        if (data_array[index]) {
            strcpy(data_array[index], token);
        } else {
            my_free(buffer, __FILE_NAME__, __LINE__);
            for (i = 0; i < index; ++i) {
                my_free(data_array[i], __FILE_NAME__, __LINE__);
            }
            my_free(data_array, __FILE_NAME__, __LINE__);
            return NULL;
        }
        index++;
        token = strtok(NULL, ", ");
    }

    my_free(buffer, __FILE_NAME__, __LINE__);
    *count = index;
    return data_array;
}