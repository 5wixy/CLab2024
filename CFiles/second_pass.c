
#include "../HeaderFiles/second_pass.h"
#include "../HeaderFiles/hash_table.h"
#include "../HeaderFiles/asm_data.h"
#include "../HeaderFiles/helper.h"
#include "../HeaderFiles/am_handler.h"
#include "../HeaderFiles/globals.h"
#include "../HeaderFiles/validation.h"
#include "../HeaderFiles/globals.h"
#include "../HeaderFiles/code_binary.h"
#include "../HeaderFiles/Errors.h"


int start_second_pass(char *file_name, HashTable *symbol_table, AssemblyData *ad) {
        int IC = 0;
        ExternOperand *extern_operands = NULL;
        int extern_count = 0;
        int extern_capacity = INITIAL_EXTERN_OPERANDS_CAPACITY;
        int line_num = 1;

    FILE *fp = fopen(file_name, "r");
        if (!fp) {
            perror("Error opening file");
            return -1;
        }
        extern_operands = (ExternOperand *)malloc_helper(extern_capacity * sizeof(ExternOperand), __FILE_NAME__, __LINE__);
        char str[MAX_LINE_LEN];
        char line_copy[MAX_LINE_LEN];
        while (fgets(str, sizeof(str), fp)) {
            remove_trailing_newline(str);
            trim_spaces(str);
            strcpy(line_copy, str);

            char *line_ptr = str;
            char *label = NULL;
            char *first_word = NULL;
            char *second_word = NULL;

            /* Tokenize the line to get the first word */
            first_word = strtok(line_ptr, " \t");

            /* Check if the first word is a label (ends with ':') */
            if (first_word && first_word[strlen(first_word) - 1] == ':') {
                label = first_word;
                first_word = strtok(NULL, " \t");
            }

            /* Now, first_word should point to the next word after the label, or the first word if no label */
            second_word = first_word ? strtok(NULL, " \t") : NULL;

            /* Check if the first or second word is .entry, .extern, .data, or .string */
            if (first_word && (is_entry(first_word) || is_extern(first_word) || is_data_or_string(first_word))) {
                line_num++;
                continue;
            }

            if (second_word && (is_entry(second_word) || is_extern(second_word) || is_data_or_string(second_word))) {
                line_num++;
                continue;
            }
            if(first_word == NULL && second_word == NULL){
                line_num++;
                continue;
            }
            /* If the line is an instruction, process it */
            AssemblyLine *line = malloc_helper(sizeof(AssemblyLine),__FILE_NAME__,__LINE__);
            if (line == NULL) {
                fprintf(stderr, "Memory allocation failed for AssemblyLine\n");
                continue;
            }


            if (extern_operands == NULL) {
                perror("Failed to allocate memory for extern_operands array");
                exit(EXIT_FAILURE);
            }

        parse_assembly_line(line_copy,line);

        /* Process operands to replace labels with their addresses */
        process_operands_for_labels(line, symbol_table, ad, &IC, &extern_operands, &extern_count, &extern_capacity,line_num);

        /* Increment IC based on the current instruction */
        IC += calculate_increment(line->src_operand, line->dest_operand);

        /* Clean up */
        free_assembly_line(line);
        line_num++;
    }


    fclose(fp);
    int entry_amount = count_entry(symbol_table);
    int extern_amount = count_extern(symbol_table);
    char *ext_file;
    char *ent_file;
    /*printf("%d",entry_amount);*/
    char *ob_file;
    ob_file = create_new_file_name(file_name,".ob");
    fill_ob_file(ob_file,ad);
    my_free(ob_file,__FILE_NAME__,__LINE__);
    if(entry_amount != 0){
        ent_file = create_new_file_name(file_name,".ent");
        fill_ent_file(ent_file,symbol_table,ad);
        my_free(ent_file,__FILE_NAME__,__LINE__);
    }
    if(extern_amount != 0){
        ext_file = create_new_file_name(file_name,".ext");
        fill_ext_file(ext_file,extern_operands,extern_count);
        int i;
        for (i = 0; i < extern_count; i++) {
            free(extern_operands[i].label_name);
        }
        my_free(extern_operands,__FILE_NAME__,__LINE__);
        my_free(ext_file,__FILE_NAME__,__LINE__);
    }

    return 1;
}

void fill_ext_file(const char *file_name, ExternOperand *extern_operands, int extern_count) {
    if (!file_name || !extern_operands) {
        fprintf(stderr, "Error: Invalid arguments to fill_ext_file\n");
        return;
    }

    /* Open the file for writing */
    FILE *file = fopen(file_name, "w");
    if (!file) {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
    }

    /* Write each extern operand's name and address to the file */

    int i;
    for (i = 0; i < extern_count; i++) {
        fprintf(file, "%s %04d\n", extern_operands[i].label_name, extern_operands[i].address);
    }

    /* Close the file */
    fclose(file);
}

void fill_ent_file(char *file_name, HashTable *table,AssemblyData *ad) {


    FILE *fp = fopen(file_name, "w");
    if (!fp) {
        perror("Error opening file");
        return;
    }

    int i;
    for (i = 0; i < TABLE_SIZE; ++i) {
        HashItem *current = table->table[i];

        while (current != NULL) {
            if (current->data.label.type == LABEL_ENTRY) {

                if (current->data.label.label_sort == DAT) {
                    fprintf(fp, "%s %d\n", current->name, current->data.label.address + ad->instruction_count + 100);
                } else if (current->data.label.label_sort == INST) {
                    fprintf(fp, "%s %d\n", current->name, current->data.label.address);
                }
            }



            current = current->next;  /* Move to the next item in the chain */
        }
    }

    fclose(fp);
}

int count_entry(HashTable *table) {
    int count = 0;
    int i;

    for (i = 0; i < TABLE_SIZE; ++i) {
        HashItem *current = table->table[i];

        while (current != NULL) {
            if (current->data.label.type == LABEL_ENTRY) {
                count++;
            }
            current = current->next;  /* Move to the next item in the chain */
        }
    }

    return count;
}
int count_extern(HashTable *table) {
    int count = 0;
    int i;

    for (i = 0; i < TABLE_SIZE; ++i) {
        HashItem *current = table->table[i];

        while (current != NULL) {
            if (current->data.label.type == LABEL_EXTERN) {
                count++;
            }
            current = current->next;  /* Move to the next item in the chain */
        }
    }

    return count;
}

void fill_ob_file(const char *file_name, AssemblyData *ad) {
    FILE *fp = fopen(file_name, "w");
    int i;
    if (!fp) {
        perror("Error opening file");
        return;
    }

    /* Allocate memory for octal conversion (5 characters + 1 for null terminator) */
    char *octal = malloc_helper(6 * sizeof(char), __FILE_NAME__, __LINE__);

    if (!octal) {
        perror("Memory allocation failed");
        fclose(fp);
        return;
    }

    /* Write the instruction and data count */
    fprintf(fp, "  %d %d\n", ad->instruction_count, ad->data_count);

    /* Process the instructions array */
    for (i = 0; i < ad->instruction_count; ++i) {
        binary_to_octal_15bit(ad->instructions[i], octal);
        fprintf(fp, "%04d %s\n", i + 100, octal);  // %04d ensures a 4-digit number with leading zeros
    }

    /* Process the data array */
    for (i = 0; i < ad->data_count; ++i) {
        binary_to_octal_15bit(ad->data[i], octal);
        fprintf(fp, "%04d %s\n", i + 100 + ad->instruction_count, octal);  // %04d ensures a 4-digit number with leading zeros
    }

    /* Free the memory after the loop */
    my_free(octal, __FILE_NAME__, __LINE__);

    fclose(fp);
}


void process_operands_for_labels(AssemblyLine *line, HashTable *symbol_table, AssemblyData *ad, int *IC, ExternOperand **extern_operands, int *extern_count, int *extern_capacity,int line_num) {
    if (!line || !ad || !symbol_table || !extern_operands || !extern_count || !extern_capacity) {
        fprintf(stderr, "Error: Invalid arguments to process_operands_for_labels\n");
        return;
    }

    int src_index = -1;
    int dest_index = -1;

    /* Determine the indexes based on operand count */
    if (line->src_operand && line->dest_operand) {
        src_index = *IC + 1;  /* src operand goes to 2nd binary word */
        dest_index = *IC + 2; /* dest operand goes to 3rd binary word */
    } else if (line->src_operand || line->dest_operand) {
        dest_index = *IC + 1;  /* Single operand goes to 2nd binary word */
    }

    /* Update source operand if it uses direct addressing */
    if (line->src_operand && src_index >= 0) {
        process_and_update_operand(line, line->src_operand, symbol_table, ad, src_index, extern_operands, extern_count, extern_capacity,line_num);
    }

    /* Update destination operand if it uses direct addressing */
    if (line->dest_operand && dest_index >= 0) {
        process_and_update_operand(line, line->dest_operand, symbol_table, ad, dest_index, extern_operands, extern_count, extern_capacity,line_num);
    }
}
void process_and_update_operand(AssemblyLine *line, const char *operand, HashTable *symbol_table, AssemblyData *ad, int index, ExternOperand **extern_operands, int *extern_count, int *extern_capacity,int line_num) {
    if (!operand || !symbol_table || !ad || index < 0) {
        fprintf(stderr, "Error: Invalid arguments in process_and_update_operand\n");
        return;
    }

    remove_trailing_spaces(operand);


    if(detect_addressing_method(operand) == DIRECT) {
        /* Check if the operand is a label */
        HashItem *item = get_label(symbol_table, operand);
        if (item) {
            /* Replace the operand with the label's address in binary */
            char address_binary[WORD_SIZE];
            int label_address = item->data.label.address;

            if (item->data.label.label_sort == DAT) {
                to_binary_address(label_address + 100 + ad->instruction_count, address_binary);
            } else {
                to_binary_address(label_address, address_binary);
            }

            if (index >= 0 && index < ad->instruction_count) {
                update_first_12_bits(ad->instructions[index], address_binary);
            }

            /* Ensure enough space for appending "001" or "010" */
            size_t current_len = strlen(ad->instructions[index]);
            size_t new_len = current_len + 3 + 1; /* 3 for "001" or "010", 1 for null terminator */

            char *new_instruction = realloc(ad->instructions[index], new_len);
            if (!new_instruction) {
                perror("Memory reallocation failed");
                exit(1);
            }

            ad->instructions[index] = new_instruction;

            if (item->data.label.type == LABEL_EXTERN) {
                strcat(ad->instructions[index], "001");


                add_extern_operand(extern_operands, extern_count, extern_capacity, operand, index + 100);

            } else {
                strcat(ad->instructions[index], "010");
            }
        }
        else{
            print_error(ERR_UNDEFINED_LABEL, line_num);


        }
    }
}
void add_extern_operand(ExternOperand **extern_operands, int *extern_count, int *extern_capacity, const char *operand, int address) {

    if (*extern_count >= *extern_capacity) {
        *extern_capacity *= 2;
        *extern_operands = realloc(*extern_operands, *extern_capacity * sizeof(ExternOperand));
        if (*extern_operands == NULL) {
            perror("Memory reallocation failed");
            exit(EXIT_FAILURE);
        }
    }


    (*extern_operands)[*extern_count].label_name = strdup(operand);
    if ((*extern_operands)[*extern_count].label_name == NULL) {
        perror("Memory allocation failed for label_name");
        exit(EXIT_FAILURE);
    }
    (*extern_operands)[*extern_count].address = address;


    (*extern_count)++;
}

