
#include "../Header Files/validation.h"
#include <stdio.h>
#include <ctype.h>
#include "../Header Files/am_handler.h"
#include "../Header Files/globals.h"
#include "../Header Files/hash_table.h"
#include "../Header Files/helper.h"
#include "../Header Files/first_pass.h"



void process_label_line(char *line, char *first_word, HashTable *table, int IC) {
    inst_parts *inst = malloc_helper(sizeof(inst_parts));
    if (inst == NULL) {
        fprintf(stderr, "Memory allocation failed for inst_parts\n");
        return;
    }

    char line_copy[MAX_LINE_LEN];
    strcpy(line_copy, line);

    // Debug: Print the line to be processed
    printf("Processing line: %s\n", line);

    // Skip the first word (label)
    skip_first_word(line);

    // Get the second word to determine if it's an operation or directive
    char *second_word = strtok(line, " \n");

    // Debug: Print the second word
    if (second_word) {
        printf("Second word: %s\n", second_word);
    } else {
        printf("ERROR: No second word found\n");
    }

    if (second_word) {
        if (match_opcodes(second_word) > -1) {
            // It's an operation
            process_labeled_operation(first_word, line_copy, table, IC);
        } else if (is_data_or_string(second_word)) {
            // It's a directive
            process_data_or_string(first_word, second_word, strtok(NULL, ""), table, IC);
        } else {
            printf("ERROR: Unknown directive or operation.\n");
        }
    } else {
        printf("ERROR: Missing directive or operation.\n");
    }

    // Free allocated memory (if any) and handle errors properly
    free(inst);
}


void process_labeled_operation(char *label_name, char *line, HashTable *table, int IC) {
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
            // Handle special cases for operations like PRN
            if (command->opcode < RTS && command->opcode > LEA) {
                command->dest = strdup(operand);
                // Ensure source is NULL for PRN
                command->source = NULL;
                break; // Exit loop as we only expect one operand for PRN
            } else {
                command->source = strdup(operand);
            }
        } else if (operand_index == 1) {
            command->dest = strdup(operand);
        }
        operand_index++;
    }

    // Validate operands based on opcode
    if (!is_valid_command(command)) {
        // Handle invalid command scenario
        free(command->source);
        free(command->dest);
        free(command);
        return;
    }

    // Insert the label into the hash table
    insert_label(table, command->label, IC, "instruction", 0);

    // Clean up
    if (command->source) {
        free(command->source);
        command->source = NULL;
    }
    if (command->dest) {
        free(command->dest);
        command->dest = NULL;
    }
    free(command);
    command = NULL;
}
void process_data_or_string(char *label_name, char *directive, char *remaining_line, HashTable *table, int IC) {
    if (strcmp(directive, ".data") == 0) {
        process_data_directive(label_name, remaining_line, table, IC);
    } else if (strcmp(directive, ".string") == 0) {
        process_string_directive(label_name, remaining_line, table, IC);
    } else {
        printf("ERROR: Unknown directive.\n");
    }
}

void process_data_directive(char *label_name, char *line, HashTable *table, int IC) {
    // Implement the processing of .data directive
    // Example: parse numbers and store them in the hash table
    // ...
}

void process_string_directive(char *label_name, char *line, HashTable *table, int IC) {
    // Implement the processing of .string directive
    // Example: store the string in the hash table
    // ...
}


int start_first_pass(char* file_name, HashTable* table) {
    int IC = 100, DC = 0;
    HashTable *entries,*externals;
    //TODO take care of entries and externals tables.
    command_parts *command;
    FILE* fp;
    int label_flag = 0;
    char str[MAX_LINE_LEN];

    fp = fopen(file_name, "r");
    if (!fp) {
        perror("Error opening file");
        return -1;
    }

    while (fgets(str, sizeof(str), fp)) {
        label_flag = 0;
        char line_copy[MAX_LINE_LEN];
        strncpy(line_copy, str, sizeof(line_copy) - 1);
        char* first_word = get_first_word(str);
        //TODO work on line processing. entry extern , label, operation.
        if(strchr(str,'.')){
            if(strstr(line_copy, ".entry") || strstr(line_copy, ".extern")){
                if(strstr(line_copy,".extern")){
                    //TODO check validity
                    strtok(line_copy," ");
                    char *extern_label = strtok(NULL, " ");
                    process_extern(extern_label,table);
                    continue;

                }
                if(strstr(line_copy,".entry")){
                    strtok(line_copy," ");
                    char *entry_label = strtok(NULL, " ");
                    process_entry(entry_label,table);
                    continue;

                }

            }
        }
        else if (first_word[strlen(first_word) - 1] == ':') {
            label_flag = 1;
           // printf("%s\n",str);
            is_data_or_string(line_copy);
            process_label_line(str, first_word, table, IC);
        }

        else if(is_data_instruction(first_word)){
            //printf(first_word);

        }
        else{
        IC++;
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









