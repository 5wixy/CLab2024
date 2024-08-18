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


    str = malloc_helper((str_len + 1) * sizeof(char));
    if (str == NULL) {
        return NULL;
    }


    fread(str, sizeof(char), str_len, fp);
    str[str_len] = '\0';

    return str;
}


void *malloc_helper(int object_size) {
    void *obj_ptr = malloc(object_size);
    if (obj_ptr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1); /* Exit the program on memory allocation failure */
    }

    return obj_ptr;
}

/*Free pointer from memory, custom function for management*/
void my_free(void* ptr) {
    
    free(ptr);
    ptr = NULL;
}
/*Duplicate a given string*/
char *my_strdup(const char *src) {
    int len;
    char *dest;
    if (src == NULL) return NULL;
    len = strlen(src) + 1;
    dest = malloc_helper(len);
    strcpy(dest, src);
    return dest;
}

int detect_addressing_method(const char *operand) {
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

int match_opcodes(char *str){
    int i;
    if (str == NULL) {
        return -1;
    }


    for (i = 0; i < OP_ARR_SIZE; i++) {
        if (strcmp(str, op_arr[i].opcode) == 0) {
            return i; /* Return the index of the matching opcode */
        }
    }
    return -1; /* Return -1 if the string does not match any known opcodes */
}

void remove_commas(char *str) {
    char *src = str, *dst = str;

    while (*src) {
        if (*src != ',') {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';  /* Null-terminate the modified string */
}

/*Get a register numeric value*/
int get_register_value( char *register_name) {
    int i;
    int num_registers;
    remove_commas(register_name);
    trim_whitespace(register_name);
    if(register_name[0] == '*'){
        register_name++;
    }
    num_registers = 8;
    for (i = 0; i < num_registers; i++) {
        if (strcmp(register_name, register_table[i].name) == 0) {
            return register_table[i].value;
        }
    }
    /* Return -1 if the register is not found */
    return -1;
}
void to_binary_string(int num, int bits, char *output) {
    int i;
    for (i = bits - 1; i >= 0; i--) {
        output[bits - 1 - i] = (num & (1 << i)) ? '1' : '0';
    }
    output[bits] = '\0';

}
void remove_trailing_newline(char *str) {
    int len = strlen(str);

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
            strcpy(output, "0000");  /* Default to an invalid or error code */
            break;
    }
}

void remove_trailing_spaces(char *str) {
    int length;
    if (str == NULL) return;  /* Check for NULL pointer */

    length = strlen(str);

    /* Iterate from the end of the string and trim spaces */
    while (length > 0 && str[length - 1] == ' ') {
        str[length - 1] = '\0';
        length--;
    }
}
char *trim_whitespace(char *str) {
    char *end;

    while (isspace((unsigned char)*str)) str++;

    if (*str == 0)
        return str;

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    *(end + 1) = '\0';

    return str;
}


void copy_file_content(const char *source_file, const char *destination_file, int remove_macros_flag) {
    FILE *original, *copied;
    char line[1024];

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
    /* Find the end of the label, if present */
    char *label_end = strchr(line, ':');
    int opcode;
    char saved_char;
    char *opcode_end;
    /* Start after the label or at the beginning of the line */
    char *start = label_end ? label_end + 1 : line;
    /* Skip leading spaces or tabs */
    while (*start == ' ' || *start == '\t') {
        start++;
    }
    /* Find the end of the opcode */
    opcode_end = start;
    while (*opcode_end != ' ' && *opcode_end != '\t' && *opcode_end != '\0') {
        opcode_end++;
    }
    /* Temporarily null-terminate the opcode */
    saved_char = *opcode_end;
    *opcode_end = '\0';
    /* Match the opcode */
    opcode = match_opcodes(start);
    /* Restore the character at the end of the opcode */
    *opcode_end = saved_char;
    if (opcode > -1) {
        return opcode;  /* Return the matched opcode */
    }
    return -1;  /* Return -1 if no opcode is matched */
}
char* extract_src_operand(const char *line) {
    char *result,*src_start,*src_end,*label_end;
    int length;
    /* Skip leading whitespace */
    while (*line == ' ') {
        line++;
    }

    /* Skip label if present */
    label_end = strchr(line, ':');
    if (label_end) {
        line = label_end + 1;
        while (*line == ' ') {
            line++;
        }
    }

    /* Find the start of the src_operand */
    src_start = strchr(line, ' ');
    if (!src_start) {
        return NULL;
    }
    src_start++; /* Skip the space */

    /* Find the end of the src_operand */
    src_end = strchr(src_start, ',');
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
    length = src_end - src_start;
    result = malloc_helper(length + 1);
    strncpy(result, src_start, length);
    result[length] = '\0';

    return result;
}
char* extract_dest_operand(const char *line) {
    char *remaining,*label_end,*src_operand,*result,*opcode;
    char *original_line_copy;
    char *line_copy = malloc_helper(strlen(line) + 1);
    if (!line_copy) {
        return NULL;
    }
    strcpy(line_copy, line);

    /* Save the original pointer for later freeing */
    original_line_copy = line_copy;

    while (*line_copy == ' ') {
        line_copy++;
    }

     label_end = strchr(line_copy, ':');
    if (label_end) {
        line_copy = label_end + 1;
        while (*line_copy == ' ') {
            line_copy++;
        }
    }

    opcode = strtok(line_copy, " ");
    if (!opcode) {
        my_free(original_line_copy);
        return NULL; /* No opcode found */
    }

    src_operand = strtok(NULL, ",");
    if (!src_operand) {
        my_free(original_line_copy);
        return NULL; /* No source operand found */
    }

    remaining = strtok(NULL, "");
    if (remaining) {
        /* Remove leading whitespace */
        while (*remaining == ' ') {
            remaining++;
        }

        result = malloc_helper(strlen(remaining) + 1);
        if (result) {
            strcpy(result, remaining);
        }

        my_free(original_line_copy); /* Free the original pointer */
        return result;
    }

    my_free(original_line_copy); /* Free the original pointer */
    return NULL;
}

LineType determine_line_type(AssemblyLine *line, const char *raw_line) {
    if (line->opcode == INVALID_OPCODE) {
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
    const char *start, *end;
    int length;
    int data_index = 0;
    int initial_capacity;
    char **data_array;
    char **temp_array;

    /* Find the first quotation mark */
    start = strchr(str, '"');
    if (!start) {
        return NULL; /* No opening quote found */
    }
    start++; /* Move past the opening quote */

    /* Find the last quotation mark */
    end = strrchr(start, '"');
    if (!end || *(end + 1) != '\0') {
        return NULL; /* No closing quote found or extra characters after the closing quote */
    }

    /* Calculate the length of the string between the quotes */
    length = end - start;
    if (length == 0) {
        return NULL; /* Empty string */
    }

    /* Allocate initial memory for the data_array */
    initial_capacity = length;
    data_array = malloc_helper(initial_capacity * sizeof(char *));
    if (!data_array) return NULL;

    /* Copy each character into the data_array */
    for (i = 0; i < length; ++i) {
        /* Check if we need more space in the data_array */
        if (data_index >= initial_capacity) {
            initial_capacity *= 2;
            temp_array = realloc(data_array, initial_capacity * sizeof(char *));
            if (!temp_array) {
                /* Free previously allocated memory on failure */
                for (j = 0; j < data_index; ++j) {
                    my_free(data_array[j]);
                }
                my_free(data_array);
                return NULL;
            }
            data_array = temp_array;
        }

        data_array[data_index] = malloc_helper(2);
        if (data_array[data_index]) {
            data_array[data_index][0] = start[i];
            data_array[data_index][1] = '\0';
            data_index++;
        } else {
            /* Free previously allocated memory on failure */
            for (j = 0; j < data_index; ++j) {
                my_free(data_array[j]);
            }
            my_free(data_array);
            return NULL;
        }
    }

    line->data_count = data_index;

    /* Null-terminate the array */
    data_array = realloc(data_array, data_index * sizeof(char *));
    if (!data_array) {
        /* Free allocated memory on failure */
        return NULL;
    }

    return data_array;
}
void skip_leading_space(char **str) {
    while (**str == ' ' || **str == '\t') {
        (*str)++;
    }
}

char** extract_data_array(char *str, int *count) {

    int i;
    int index;
    const char *start;
    char **data_array;
    char **new_data_array;
    char *token;
    char *buffer;
    int capacity = 10;

    /* Remove trailing newline characters from the string */
    remove_trailing_newline(str);

    /* Find the start of the data segment */
    start = strchr(str, ':');
    if (start) {
        start++;
    } else {
        start = str;
    }

    /* Skip leading whitespace */
    while (*start == ' ' || *start == '\t') {
        start++;
    }

    /* Skip over the .data directive if present */
    if (strncmp(start, ".data", 5) == 0) {
        start += 5;
    }

    /* Skip any additional whitespace */
    while (*start == ' ' || *start == '\t') {
        start++;
    }

    /* Return NULL if the line is empty after processing */
    if (*start == '\0') {
        *count = 0;
        return NULL;
    }

    /* Initialize the data array with a certain capacity */
    data_array = malloc_helper(capacity * sizeof(char*));
    if (!data_array) return NULL;

    index = 0;
    buffer = my_strdup(start);
    if (!buffer) {
        my_free(data_array);
        return NULL;
    }

    /* Tokenize the string based on commas and spaces */
    token = strtok(buffer, ", ");
    while (token != NULL) {
        /* Resize the array if needed */
        if (index >= capacity) {
            capacity *= 2;
            new_data_array = realloc(data_array, capacity * sizeof(char*));
            if (!new_data_array) {
                my_free(buffer);
                for (i = 0; i < index; ++i) {
                    my_free(data_array[i]);
                }
                my_free(data_array);
                return NULL;
            }
            data_array = new_data_array;
        }

        /* Duplicate the token and add it to the array */
        data_array[index] = my_strdup(token);
        if (data_array[index]) {
            strcpy(data_array[index], token);
        } else {
            my_free(buffer);
            for (i = 0; i < index; ++i) {
                my_free(data_array[i]);
            }
            my_free(data_array);
            return NULL;
        }
        index++;
        token = strtok(NULL, ", ");
    }

    /* Clean up and return the populated data array */
    my_free(buffer);
    *count = index;
    return data_array;
}
