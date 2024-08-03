
#include "../Header Files/validation.h"
#include <stdio.h>
#include <ctype.h>
#include "../Header Files/am_handler.h"
#include "../Header Files/globals.h"
#include "../Header Files/hash_table.h"
#include "../Header Files/helper.h"
#include "../Header Files/first_pass.h"



void process_label_line(char *line, char *first_word, HashTable *table, int *IC,int *DC,char *memory_array) {
    inst_parts *inst = malloc_helper(sizeof(inst_parts));
    if (inst == NULL) {
        fprintf(stderr, "Memory allocation failed for inst_parts\n");
        return;
    }

    char line_copy[MAX_LINE_LEN];
    strcpy(line_copy, line);


    // Skip the first word (label)
    skip_first_word(line);

    // Get the second word to determine if it's an operation or directive
    char *second_word = strtok(line, " \n");

    // Debug: Print the second word
    if (second_word) {
        printf("Second word: %s\n", second_word);
    } else {
        printf("ERROR: No second word found\n");
        free(inst);
        return;
    }

    if (second_word) {
        // Find the position of the second word in the original line
        char *second_word_pos = strstr(line_copy, second_word);
        if (second_word_pos) {
            // Move past the second word to get the rest of the line
            char *rest_of_line = second_word_pos + strlen(second_word);

            // Skip any leading whitespace in the rest of the line
            while (isspace(*rest_of_line)) {
                rest_of_line++;
            }

            if (match_opcodes(second_word) > -1) {
                // It's an operation
                process_labeled_operation(first_word, line_copy, table, IC,memory_array);
            } else if (is_data_or_string(second_word)) {
                // It's a directive
                process_data_or_string(first_word, second_word, rest_of_line, table, IC,DC,memory_array);
            } else {
                printf("ERROR: Unknown directive or operation.\n");
            }
        } else {
            printf("ERROR: Second word not found in line copy\n");
        }
    } else {
        printf("ERROR: Missing directive or operation.\n");
    }

    // Free allocated memory (if any)
    free(inst);
}


void process_labeled_operation(char *label_name, char *line, HashTable *table, int *IC,char *memory_array) {
    line = remove_newline(line);
    command_parts *command = malloc_helper(sizeof(command_parts));
    if (command == NULL) {
        fprintf(stderr, "Memory allocation failed for command_parts\n");
        return;
    }

    char temp_line[MAX_LINE_LEN];
    strcpy(temp_line, line);

    // Discard the first word (label or directive)
    strtok(temp_line, " ");

    // Get the rest of the line
    char *remaining_line = strtok(NULL, "");

    // Extract the opcode
    char *opcode_word = strtok(remaining_line, " ");
    command->label = label_name;
    command->opcode = match_opcodes(opcode_word);

    // Initialize operands to NULL
    command->source = NULL;
    command->dest = NULL;

    // Extract operands
    char *operand;
    int operand_index = 0;
    while ((operand = strtok(NULL, " ,")) != NULL) {
        if (operand_index == 0) {
            if (command->opcode < RTS && command->opcode > LEA) {
                command->dest = strdup(operand);
                command->source = NULL;
                break; // Only one operand expected for PRN
            } else {
                command->source = strdup(operand);
            }
        } else if (operand_index == 1) {
            command->dest = strdup(operand);
        }
        operand_index++;
    }

    // Validate operands based on opcode


    // Insert the label into the hash table
    insert_label(table, command->label, *IC, "instruction", 0);
    increment_IC(line,IC);
    if (!is_valid_command(command)) {
        free(command->source);
        free(command->dest);
        free(command);
        return;
    }
    // Clean up
    if (command->source) {
        free(command->source);
    }
    if (command->dest) {
        free(command->dest);
    }
    free(command);
}
void process_data_or_string(char *label_name, char *directive, char *remaining_line, HashTable *table, int *IC,int *DC,char *memory_array) {
    if (strcmp(directive, ".data") == 0) {
        process_data_directive(label_name, remaining_line, table, IC,DC,memory_array);
    } else if (strcmp(directive, ".string") == 0) {
        process_string_directive(label_name, remaining_line, table, IC,DC,memory_array);
    } else {
        printf("ERROR: Unknown directive.\n");
    }
}

void process_data_directive(char *label_name, char *line, HashTable *table, int *IC,int *DC,char *memory_array) {
    int i;
    inst_parts *inst = malloc_helper(sizeof(inst_parts));
    if (inst == NULL) {
        fprintf(stderr, "Memory allocation failed for inst_parts\n");
        return;
    }


    int count_nums = 0;
    if (is_valid_input(line)) {
        count_nums = count_numbers(line);
    } else {
        printf("Invalid input line\n");
        free(inst);
        return;
    }

    inst->nums = malloc_helper(count_nums * sizeof(short));
    if (inst->nums == NULL) {
        fprintf(stderr, "Memory allocation failed for nums array\n");
        free(inst);
        return;
    }

    fill_nums_array(line, inst->nums, count_nums);

    // Debug: Print the nums array


    // Insert the label into the hash table
    if(label_name != NULL) {
        insert_label(table, label_name, *IC, "data", 0);
    }
    // Increment IC based on data size
    *IC += count_nums;

    // Free allocated memory
    free(inst->nums);
    free(inst);
}

void process_string_directive(char *label_name, char *line, HashTable *table, int *IC, int *DC, char memory_array[][WORD_SIZE]) {
    size_t i;
    if (line == NULL || IC == NULL || DC == NULL || memory_array == NULL) {
        printf("Error: NULL pointer provided.\n");
        return;
    }

    // Remove leading and trailing whitespace
    remove_trailing_newline(line);
    char trimmed_line[MAX_LINE_LEN];
    strncpy(trimmed_line, line, sizeof(trimmed_line) - 1);
    trimmed_line[sizeof(trimmed_line) - 1] = '\0';

    // Check for the string delimiters
    char *start = strchr(trimmed_line, '"');
    if (start == NULL) {
        printf("ERROR: String does not start with \"\n");
        return;
    }

    char *end = strrchr(trimmed_line, '"');
    if (end == NULL || end == start) {
        printf("ERROR: String does not end with \"\n");
        return;
    }

    // Extract the string between the quotes
    size_t string_length = end - start - 1;
    char string_content[MAX_LINE_LEN];
    strncpy(string_content, start + 1, string_length);
    string_content[string_length] = '\0';  // Null-terminate the string

    // Calculate the total length including the null terminator
    size_t total_length = string_length + 1;  // Include the null terminator

    // Calculate the number of 15-bit words needed
    size_t num_words = (total_length + 1) / 2;  // Each word is 15 bits, so 2 bytes
    num_words += (total_length % 2 == 0) ? 0 : 1;  // If odd length, add one more word

    // Initialize index for memory_array
    size_t index = *IC;

    // Convert each character to a 15-bit binary string and store in memory_array
    char binary_char[16];  // Temporary buffer for one 15-bit binary string
    for (i = 0; i < total_length; i++) {
        ascii_to_15_bit_binary(string_content[i], binary_char, sizeof(binary_char));

        // Copy 15-bit binary string into memory_array
        strncpy(memory_array[index], binary_char, WORD_SIZE);
        index++;
    }

    // Handle the case where we need to add padding if total_length is odd
    if (total_length % 2 != 0) {
        // Add the padding binary string
        ascii_to_15_bit_binary('\0', binary_char, sizeof(binary_char));
        strncpy(memory_array[index], binary_char, WORD_SIZE);
    }

    // Update the data counter (DC) and insert the string into the hash table
    *DC = *IC;
    *DC += num_words;
    printf("Updated DC: %d\n", *DC);
    if (label_name != NULL) {
        insert_label(table, label_name, *DC, "data", 0);
    }
}


int start_first_pass(char* file_name, HashTable* table) {
    int IC = 100;
    int DC = 0;
    char *memory_array;
    FILE* fp;
    char str[MAX_LINE_LEN];

    fp = fopen(file_name, "r");
    if (!fp) {
        perror("Error opening file");
        return -1;
    }

    while (fgets(str, sizeof(str), fp)) {
        char line_copy[MAX_LINE_LEN];
        strncpy(line_copy, str, sizeof(line_copy) - 1);
        char* first_word = get_first_word(str);
        if (strchr(str, '.')) {
            if (strstr(line_copy, ".entry") || strstr(line_copy, ".extern")) {
                if (strstr(line_copy, ".extern")) {
                    strtok(line_copy, " ");
                    char *extern_label = strtok(NULL, " ");
                    process_extern(extern_label, table);
                } else if (strstr(line_copy, ".entry")) {
                    strtok(line_copy, " ");
                    char *entry_label = strtok(NULL, " ");
                    process_entry(entry_label, table);
                }
            }
        }

        if (first_word[strlen(first_word) - 1] == ':') {
            printf("label = %s, ADDRESS = %d\n",first_word,IC);
            is_data_or_string(line_copy);
            process_label_line(str, first_word, table, &IC,&DC,memory_array);

        } else if (is_data_instruction(first_word)) {
            process_data_or_string(NULL,first_word,line_copy,table,&IC,&DC,memory_array);

        } else {
            // Normal operation without label
            printf("OPERATION = %s, ADDRESS = %d\n",first_word,IC);
            increment_IC(line_copy,&IC);
        }
    }

    fclose(fp);
    return 1;
}
void process_extern(char *label_name, HashTable *table) {
    inst_parts *inst = malloc_helper(sizeof(inst_parts));
    inst->label=label_name;
    inst->arg_label = NULL;
    inst->len = 0;
    inst->nums = NULL;
    inst->is_extern = 1;

    insert_label(table,label_name,0,NULL,1);



}
void process_entry(char *label_name, HashTable *table){
    inst_parts *inst = malloc_helper(sizeof(inst_parts ));

    inst->label = label_name;
    inst->arg_label = NULL;
    inst->len = 0;
    inst->nums = NULL;
    inst->is_extern = 0;

    insert_label(table,label_name,0,NULL,0);

}









