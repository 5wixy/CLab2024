
#include "../Header Files/validation.h"
#include <stdio.h>
#include <ctype.h>
#include "../Header Files/am_handler.h"
#include "../Header Files/globals.h"
#include "../Header Files/hash_table.h"
#include "../Header Files/helper.h"
#include "../Header Files/first_pass.h"
#include "../Header Files/asm_data.h"



void process_label_line(char *line, char *first_word, HashTable *table, int *IC, int *DC, AssemblyData *ad) {
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
                process_labeled_operation(first_word, line_copy, table, IC, ad);
            } else if (is_data_or_string(second_word)) {
                // It's a directive
                process_data_or_string(first_word, second_word, rest_of_line, table, IC, DC, ad);
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


void process_labeled_operation(char *label_name, char *line, HashTable *table, int *IC, AssemblyData *ad) {
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
    int num_words = calculate_increment(command->source, command->dest);
    int slots = calculate_increment(command->source, command->dest);
    char binary_output[num_words][WORD_SIZE];
    transform_to_binary(line, binary_output, &slots, table);

    // Insert the label into the hash table
    insert_label(table, command->label, *IC, "instruction", 0);

    // Store binary words in the AssemblyData structure
    int i;
    for (i = 0; i < num_words; i++) {
        insert_instruction(ad,binary_output[i]);
    }

    increment_IC(line, IC);

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
void process_data_or_string(char *label_name, const char *directive, char *remaining_line, HashTable *symbol_table, int *IC, int *DC, AssemblyData *ad) {
    if (strcmp(directive, ".data") == 0) {
        process_data_directive(label_name, remaining_line, symbol_table, IC, DC, ad);
    } else if (strcmp(directive, ".string") == 0) {
        process_string_directive(label_name, remaining_line, symbol_table, IC, DC, ad);
    } else {
        printf("ERROR: Unknown directive.\n");
    }
}

void process_data_directive(char *label_name, char *line, HashTable *symbol_table, int *IC, int *DC, AssemblyData *ad) {
    int count_nums;
    if (is_valid_input(line)) {
        count_nums = count_numbers(line);
    } else {
        printf("Invalid input line\n");
        return;
    }

    short *nums = malloc_helper(count_nums * sizeof(short));
    if (nums == NULL) {
        fprintf(stderr, "Memory allocation failed for nums array\n");
        return;
    }

    fill_nums_array(line, nums, count_nums);
    int i;
    for (i = 0; i < count_nums; i++) {
        char binary_num[WORD_SIZE + 1];  // +1 for null-terminator
        to_binary_string(nums[i], WORD_SIZE, binary_num);
        add_data(ad, binary_num, WORD_SIZE);
    }

    if (label_name != NULL) {
        insert_label(symbol_table, label_name, *DC, "data", 0);
    }

    *DC += count_nums;

    free(nums);
}

void process_string_directive(char *label_name, char *line, HashTable *symbol_table, int *IC, int *DC, AssemblyData *ad) {
    if (line == NULL || IC == NULL || DC == NULL || ad == NULL) {
        printf("Error: NULL pointer provided.\n");
        return;
    }

    remove_trailing_newline(line);
    char trimmed_line[MAX_LINE_LEN];
    strncpy(trimmed_line, line, sizeof(trimmed_line) - 1);
    trimmed_line[sizeof(trimmed_line) - 1] = '\0';

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

    size_t string_length = end - start - 1;
    char string_content[MAX_LINE_LEN];
    strncpy(string_content, start + 1, string_length);
    string_content[string_length] = '\0';

    size_t total_length = string_length + 1;  // +1 for the null terminator
    size_t i;
    for (i = 0; i < total_length; i++) {
        char binary_char[WORD_SIZE + 1];  // +1 for null-terminator
        ascii_to_15_bit_binary(string_content[i], binary_char, WORD_SIZE);
        add_data(ad, binary_char, WORD_SIZE);
    }

    if (label_name != NULL) {
        insert_label(symbol_table, label_name, *DC, "data", 0);
    }

    *DC += total_length;
}


int start_first_pass(char *file_name, HashTable *symbol_table) {
    int IC = 100;
    int DC = 0;
    AssemblyData ad;
    init_assembly_data(&ad);

    FILE *fp = fopen(file_name, "r");
    if (!fp) {
        perror("Error opening file");
        return -1;
    }

    char str[MAX_LINE_LEN];
    while (fgets(str, sizeof(str), fp)) {
        char line_copy[MAX_LINE_LEN];
        strncpy(line_copy, str, sizeof(line_copy) - 1);
        line_copy[sizeof(line_copy) - 1] = '\0'; // Ensure null-termination
        char *first_word = get_first_word(str);
        remove_trailing_newline(line_copy);

        if (strchr(line_copy, '.')) {
            if (strstr(line_copy, ".entry") || strstr(line_copy, ".extern")) {
                if (strstr(line_copy, ".extern")) {
                    strtok(line_copy, " ");
                    char *extern_label = strtok(NULL, " ");
                    process_extern(extern_label, symbol_table);
                } else if (strstr(line_copy, ".entry")) {
                    strtok(line_copy, " ");
                    char *entry_label = strtok(NULL, " ");
                    process_entry(entry_label, symbol_table);
                }
            } else if (strstr(line_copy, ".data") || strstr(line_copy, ".string")) {
                handle_directive_or_label(line_copy, first_word, symbol_table, &IC, &DC, &ad);
            }
        } else if (first_word[strlen(first_word) - 1] == ':') {
            process_label_line(str, first_word, symbol_table, &IC, &DC, &ad);
        } else if (is_data_instruction(first_word)) {
            process_data_or_string(NULL, first_word, line_copy, symbol_table, &IC, &DC, &ad);
        } else {
            printf("OPERATION = %s, ADDRESS = %d\n", first_word, IC);

            // Allocate memory for source and dest operands
            char source[MAX_LINE_LEN] = {0};
            char dest[MAX_LINE_LEN] = {0};

            // Extract operands from the line
            extract_operands(line_copy, source, dest);
            int slots = calculate_increment(source, dest);
            int num_words;
            char binary_output[slots][WORD_SIZE];
            transform_to_binary(line_copy, binary_output, &num_words, symbol_table);

            // Insert binary words into the instruction array
            int i;
            for (i = 0; i < num_words; i++) {
                insert_instruction(&ad, binary_output[i]);
            }

            increment_IC(line_copy, &IC);
        }
    }

    fclose(fp);

    // At this point, ad contains all instructions and data, and symbol_table is populated.
    // Handle second pass here if needed.

    free_assembly_data(&ad);
    return 1;
}
void process_extern(char *label_name, HashTable *symbol_table) {
    inst_parts *inst = malloc_helper(sizeof(inst_parts));
    inst->label=label_name;
    inst->arg_label = NULL;
    inst->len = 0;
    inst->nums = NULL;
    inst->is_extern = 1;

    insert_label(symbol_table, label_name, 0, NULL, 1);



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









