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
void process_operand(char *operand, int addressing_method, char *output, int position,HashTable *table) {
	
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
void process_register_operand(char *operand, char *output, int position) {
char binary[4];
    int reg_value = get_register_value(operand);

    /* Initialize the output string to all zeros */
    memset(output, '0', 15);

    /* Convert the register value to a binary string and place it in the correct position */
      /* 3 bits for the register and 1 for null-termination*/
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
    strncpy(output + 12, "010", 3);

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
void transform_to_binary(AssemblyLine *line, char **binary_output, int *num_words, HashTable *table) {

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
        process_operand(line->src_operand, src_method, binary_output[*num_words], 6, table);
        (*num_words)++;
    }

    /* Process destination operand if it exists and is not empty */
    if (line->dest_operand != NULL) {
        process_operand(line->dest_operand, dest_method, binary_output[*num_words], 9, table);
        (*num_words)++;
    }
}


int get_label_address(const char *label,HashTable *table) {
    if(label != NULL){
    get(table,label);
    }
    return -1;
}

void process_register_operands(char *src_operand, char *dest_operand, char *output) {
    int src_reg_value = get_register_value(src_operand);
    int dest_reg_value = get_register_value(dest_operand);
    char dest_binary[4];
    char src_binary[4];
    /* Initialize the output string to all zeros*/
    memset(output, '0', 15);

    /* Convert the source register value to binary and place it in bits 6-8 */
    /* 3 bits for the register value and 1 for null-termination */
    to_binary_string(src_reg_value, 3, src_binary);
    strncpy(output + 6, src_binary, 3);

    /* Convert the destination register value to binary and place it in bits 9-11 */

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

	/*Check if the there is a label in the line and insert to the symbol table */
    if (line->label) {
        insert_label(table, line->label, *DC, 0, DAT);
    }
	/*Error handling of invalid strings */
    if (line->data_count < 0 || line->data_array == NULL) {
        
        for (i = 0; i < strlen(line->src_operand); ++i) {
            add_data(ad, "000000000000000", WORD_SIZE);
            (*DC)++;
        }
        return; 
    }
	/*Insert string as binary words to data array */
    for (i = 0; i < line->data_count; ++i) {
        ascii_to_15_bit_binary(line->data_array[i][0], binary_output, WORD_SIZE);
        add_data(ad, binary_output, WORD_SIZE);
        (*DC)++;
    }

	/*Null terminator to string in data array*/
    add_data(ad, "000000000000000", WORD_SIZE);
    (*DC)++;
}
void process_data_directive(AssemblyLine *line, HashTable *table, int *DC, AssemblyData *ad) {
    int i;
    char binary_output[WORD_SIZE + 1];
	/*Check if the there is a label in the line and insert to the symbol table */
    if (line->label) {
        insert_label(table, line->label, *DC, 0, DAT);
    }
	/*Traverse given integer array and insert to data array as binary words */
    for (i = 0; i < line->data_count; ++i) {
        short num = (short)atoi(line->data_array[i]);

        to_binary(num, binary_output);

        add_data(ad, binary_output, WORD_SIZE);

        (*DC)++;
    }
}
void process_operation_line(AssemblyLine *line, HashTable *table, int *IC, AssemblyData *ad) {
    if (line->opcode > -1) {
        /* It's an operation */
        process_labeled_operation(line, table, IC, ad);
    } else {
		/*Error handle*/
        printf("ERROR: Invalid opcode for operation.\n");
    }
}
