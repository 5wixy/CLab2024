#include "string.h"
#include "../HeaderFiles/globals.h"
#include <ctype.h>
#include "stdlib.h"
#include "stdio.h"
#include "../HeaderFiles/validation.h"
#include "../HeaderFiles/helper.h"
#include "../HeaderFiles/am_handler.h"
#include "../HeaderFiles/data_strct.h"
#include "../HeaderFiles/hash_table.h"
#include "../HeaderFiles/first_pass.h"
#include "../HeaderFiles/code_binary.h"

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
void skip_first_word(char *line_copy) {
    char *src = line_copy;
    char *dst = line_copy;

    /* Move the pointer to the end of the first word */
    while (*src && !isspace((unsigned char)*src)) {
        src++;
    }

    /* Skip any additional whitespace characters */
    while (*src && isspace((unsigned char)*src)) {
        src++;
    }

    /* Shift the remaining part of the string to the start of the array */
    while (*src) {
        *dst++ = *src++;
    }

    *dst = '\0';  /* Null-terminate the resulting string */
}
int is_valid_input(const char *line) {
    int i = 0;
    int length = strlen(line);
    int has_number = 0;
    int has_valid_data = 0;

    /* Skip leading whitespace */
    while (isspace(line[i])) {
        i++;
    }

    /* Check for the .data directive and skip it */
    if (strncmp(&line[i], ".data", 5) == 0) {
        i += 5;
        /* Skip any whitespace after .data */
        while (isspace(line[i])) {
            i++;
        }
    }

    /* Now check for numbers or commas */
    int comma_expected = 0;
    while (i < length) {
        if (line[i] == ',') {
            if (!has_number || comma_expected == 0) {
                return 0; /* Invalid input: consecutive commas or no number before comma */
            }
            comma_expected = 0;
            has_number = 0; /* Reset has_number flag */
        } else if (isdigit(line[i]) || (line[i] == '-' && isdigit(line[i + 1]))) {
            comma_expected = 1;
            has_number = 1;
            has_valid_data = 1; /* At least one valid number found */
        } else if (!isspace(line[i])) {
            return 0; /* Invalid input: unexpected character */
        }
        i++;
    }

    /* Check if there was at least one valid number */
    return has_valid_data && comma_expected;
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


int find_register_index( char *reg_name) {
    int i;
    for (i = 0; i < REG_ARR_SIZE; ++i) {
        if (strcmp(reg_name, reg_arr[i]) == 0) {
            return i; /* Return the index if the register name matches */
        }
    }
    return -1; /* Return -1 if the register name is not found */
}


void increment_IC(AssemblyLine line, int *IC) {

    /* Calculate the increment amount */
    int increment = calculate_increment(line.src_operand, line.dest_operand);

    /* Apply the increment to IC */
    apply_increment(IC, increment);
}

int calculate_increment(const char *source, const char *dest) {
    int src_method = (source != NULL) ? detect_addressing_method(source) : -1;
    int dest_method = (dest != NULL) ? detect_addressing_method(dest) : -1;

    /* Determine the increment amount based on the addressing methods */
    if ((src_method == REGISTER_POINTER || src_method == REGISTER_DIRECT) &&
        (dest_method == REGISTER_POINTER || dest_method == REGISTER_DIRECT)) {
        return 2;  /* Both operands are using register addressing methods */
    } else if (src_method == -1 && dest_method == -1) {
        return 1;  /* No operands */
    } else if (src_method == -1) {
        return 2;  /* No source operand, only destination operand */
    } else {
        return 3;  /* At least one operand is not using a register addressing method */
    }
}

void apply_increment(int *IC, int increment) {
    *IC += increment;
}
void process_operand(const char *operand, int addressing_method, char *output, int *num_words, int position,HashTable *table) {
    switch (addressing_method) {
        case IMMEDIATE:
            process_immediate_operand(operand, output);
            break;
        case DIRECT:
            process_direct_operand(operand, output,table);
            break;
        case REGISTER_POINTER:
        case REGISTER_DIRECT:
            process_register_operand(operand, output, position);
            break;
        default:
            break;
    }
}
void process_register_operand(const char *operand, char *output, int position) {
    int reg_value = get_register_value(operand);

    /* Initialize the output string to all zeros */
    memset(output, '0', 15);

    /* Convert the register value to a binary string and place it in the correct position */
    char binary[4];  /* 3 bits for the register and 1 for null-termination*/
    to_binary_string(reg_value, 3, binary);

    /* Copy the binary string into the output at the specified position*/
    strncpy(output + position, binary, 3);

    /* Set the ARE field to 100 in one line*/
    strncpy(output + 12, "100", 3);

    /* Null-terminate the output string if necessary*/
    output[15] = '\0';
}
void process_direct_operand(const char *operand, char *output,HashTable *table) {
    int address = get_label_address(operand,table);
    to_binary_string(address, 12, output);
    if (is_external_label(operand,table)) {
        strncpy(output + 12, "001", 3);
    } else {
        strncpy(output + 12, "010", 3);
    }
    output[15] = '\0';
}
void process_immediate_operand(const char *operand, char *output) {
    int value = atoi(operand + 1); /*Skip '#' */
    to_binary_string(value, 12, output);
    strncpy(output + 12, "100", 3);
    output[15] = '\0';
}
void create_first_word(int opcode, int src_method, int dest_method, char *output) {
    char opcode_bin[5];
    char src_method_bin[5];
    char dest_method_bin[5];
    char are_field[] = "100";

    /* Convert opcode to 4-bit binary */
    to_binary_string(opcode, 4, opcode_bin);

    /* Convert addressing methods to 4-bit binary*/
    addressing_method_to_binary(src_method, src_method_bin);
    addressing_method_to_binary(dest_method, dest_method_bin);

    /* Create the first word binary string */
    strncpy(output, opcode_bin, 4);
    strncpy(output + 4, src_method_bin, 4);
    strncpy(output + 8, dest_method_bin, 4);
    strncpy(output + 12, are_field, 3);
    output[15] = '\0';
}
void swap_src_dest(AssemblyLine *line) {
    if (line->dest_operand == NULL && line->src_operand != NULL && strlen(line->src_operand) > 0) {
        /* Swap the pointers */
        char *temp = line->src_operand;
        line->src_operand = line->dest_operand;
        line->dest_operand = temp;
    }
}
void transform_to_binary(AssemblyLine *line, char binary_output[][WORD_SIZE], int *num_words, HashTable *table) {

    int  src_method, dest_method;

    /* Get opcode and addressing methods*/

    src_method = detect_addressing_method(line->src_operand);
    dest_method = detect_addressing_method(line->dest_operand);

    /* Create the first binary word after setting the correct operands*/
    create_first_word(line->opcode, src_method, dest_method, binary_output[0]);
    *num_words = 1;

    /* Process both source and destination operands if they are registers */
    if (src_method == REGISTER_DIRECT || src_method == REGISTER_POINTER) {
        if (dest_method == REGISTER_DIRECT || dest_method == REGISTER_POINTER) {
            process_register_operands(line->src_operand, line->dest_operand, binary_output[*num_words]);
            (*num_words)++;
            return;
        }
    }

    /* Process source operand if it exists and is not empty */
    if (line->src_operand != NULL) {
        process_operand(line->src_operand, src_method, binary_output[*num_words], num_words, 6, table);
        (*num_words)++;
    }

    /* Process destination operand if it exists and is not empty */
    if (line->dest_operand != NULL) {
        process_operand(line->dest_operand, dest_method, binary_output[*num_words], num_words, 9, table);
        (*num_words)++;
    }
}


int get_label_address(const char *label,HashTable *table) {
    if(label != NULL){
    get(table,label);
    }
    return -1;
}

void update_label_addresses(HashTable *ht, int IC) {
    int i;
    for (i = 0; i < TABLE_SIZE; ++i) {
        HashItem *entry = ht->table[i];
        while (entry != NULL) {
            if (entry->type == TYPE_LABEL) {
                entry->data.label.address += IC;
            }
            entry = entry->next;
        }
    }
}
void process_register_operands(const char *src_operand, const char *dest_operand, char *output) {
    int src_reg_value = get_register_value(src_operand);
    int dest_reg_value = get_register_value(dest_operand);

    /* Initialize the output string to all zeros*/
    memset(output, '0', 15);

    /* Convert the source register value to binary and place it in bits 6-8 */
    char src_binary[4]; /* 3 bits for the register value and 1 for null-termination */
    to_binary_string(src_reg_value, 3, src_binary);
    strncpy(output + 6, src_binary, 3);

    /* Convert the destination register value to binary and place it in bits 9-11 */
    char dest_binary[4];
    to_binary_string(dest_reg_value, 3, dest_binary);
    strncpy(output + 9, dest_binary, 3);

    /* Set the ARE field to 100 in bits 13-15 */
    strncpy(output + 12, "100", 3);

    /* Null-terminate the output string if necessary */
    output[15] = '\0';
}

void process_string_directive(AssemblyLine *line, HashTable *table, int *DC, AssemblyData *ad) {
    int i;
    char binary_output[WORD_SIZE + 1];

    // Insert the label if it exists
    if (line->label) {
        insert_label(table, line->label, *DC, 0, DAT);
    }

    // Process each character in the string and add data
    for (i = 0; i < line->data_count; ++i) {
        ascii_to_15_bit_binary(line->data_array[i][0], binary_output, WORD_SIZE);
        add_data(ad, binary_output, WORD_SIZE);
        (*DC)++;
    }

    // Allocate and add the terminating 15-bit binary representation of NULL
    add_data(ad, "000000000000000", WORD_SIZE);
    (*DC)++;
}
void process_data_directive(AssemblyLine *line, HashTable *table, int *DC, AssemblyData *ad) {
    int i;
    char binary_output[WORD_SIZE + 1]; // Ensure there's space for null terminator

    // Insert the label if it exists
    if (line->label) {
        insert_label(table, line->label, *DC, 0, DAT);
    }

    // Process each piece of data in the data_array
    for (i = 0; i < line->data_count; ++i) {
        // Convert string to short integer
        short num = (short)atoi(line->data_array[i]);

        // Convert short integer to binary
        to_binary(num, binary_output);

        // Add binary data to the AssemblyData
        // Note: We pass binary_output and the size of binary_output to add_data
        add_data(ad, binary_output, WORD_SIZE);

        // Increment the data count and DC (Data Counter)
        (*DC)++;
    }
}
void process_operation_line(AssemblyLine *line, HashTable *table, int *IC, AssemblyData *ad) {
    if (line->opcode > -1) {
        /* It's an operation */
        process_labeled_operation(line, table, IC, ad);
    } else {
        printf("ERROR: Invalid opcode for operation.\n");
    }
}