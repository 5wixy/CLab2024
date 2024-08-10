
#include "../HeaderFiles/second_pass.h"
#include "../HeaderFiles/hash_table.h"
#include "../HeaderFiles/asm_data.h"
#include "../HeaderFiles/helper.h"
#include "../HeaderFiles/am_handler.h"
#include "../HeaderFiles/globals.h"
#include "../HeaderFiles/validation.h"
#include "../HeaderFiles/globals.h"
#include "../HeaderFiles/code_binary.h"



int start_second_pass(char *file_name, HashTable *symbol_table, AssemblyData *ad) {
        int IC = 0;

        FILE *fp = fopen(file_name, "r");
        if (!fp) {
            perror("Error opening file");
            return -1;
        }

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

            // Tokenize the line to get the first word
            first_word = strtok(line_ptr, " \t");

            // Check if the first word is a label (ends with ':')
            if (first_word && first_word[strlen(first_word) - 1] == ':') {
                label = first_word;  // This is the label
                first_word = strtok(NULL, " \t");  // Move to the next word after the label
            }

            // Now, first_word should point to the next word after the label, or the first word if no label
            second_word = first_word ? strtok(NULL, " \t") : NULL;

            // Check if the first or second word is .entry, .extern, .data, or .string
            if (first_word && (is_entry(first_word) || is_extern(first_word) || is_data_or_string(first_word))) {
                continue;  // Skip these lines
            }

            if (second_word && (is_entry(second_word) || is_extern(second_word) || is_data_or_string(second_word))) {
                continue;  // Skip these lines
            }

            // If the line is an instruction, process it
            AssemblyLine *line = malloc_helper(sizeof(AssemblyLine));
            if (line == NULL) {
                fprintf(stderr, "Memory allocation failed for AssemblyLine\n");
                continue;
            }

        parse_assembly_line(line_copy,line);

        // Process operands to replace labels with their addresses
        process_operands_for_labels(line, symbol_table, ad, &IC);

        // Increment IC based on the current instruction
        IC += calculate_increment(line->src_operand, line->dest_operand);

        // Clean up
        free_assembly_line(line);
    }


    fclose(fp);
    int entry_amount = count_entry(symbol_table);
    int extern_amount = count_extern(symbol_table);
    char *ext_file;
    char *ent_file;
    printf("%d",entry_amount);
    char *ob_file;
    ob_file = create_new_file_name(file_name,".ob");
    fill_ob_file(ob_file,ad);
    free(ob_file);
    if(entry_amount != 0){
        ent_file = create_new_file_name(file_name,".ent");
        free(ent_file);;
    }
    if(extern_amount != 0){
        ext_file = create_new_file_name(file_name,".ext");
        free(ext_file);
    }

    return 1;
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

void fill_ob_file(const char *file_name, AssemblyData *ad){
    FILE *fp = fopen(file_name, "w");
    int i;
    if (!fp) {
        perror("Error opening file");
        return;
    }

    char *octal = malloc_helper(5*sizeof(int));

    /* Instructions array */
        fprintf(fp, "  %d %d\n", ad->instruction_count, ad->data_count);
    for (i = 0; i < ad->instruction_count ; ++i) {
        binary_to_octal_15bit(ad->instructions[i], octal);
        fprintf(fp, "%d %s\n",i+100, octal);

    }

    /* Process the data array */

        for (i = 0; i < ad->data_count; ++i) {
            binary_to_octal_15bit(ad->data[i], octal);
            fprintf(fp, "%d %s\n", i + 100 + ad->instruction_count, octal);
        }
        /*free(octal);*/
    fclose(fp);
}



void process_operands_for_labels(AssemblyLine *line, HashTable *symbol_table, AssemblyData *ad, int *IC) {
    if (!line || !ad || !symbol_table) {
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
        process_and_update_operand(line,line->src_operand, symbol_table, ad, src_index);
    }

    /* Update destination operand if it uses direct addressing */
    if (line->dest_operand && dest_index >= 0) {
        process_and_update_operand(line,line->dest_operand, symbol_table, ad, dest_index);
    }
}
void process_and_update_operand(AssemblyLine *line,const char *operand, HashTable *symbol_table, AssemblyData *ad, int index) {

    if (!operand || !symbol_table || !ad || index < 0) {
        fprintf(stderr, "Error: Invalid arguments in process_and_update_operand\n");
        return;
    }
    remove_trailing_spaces(operand);

    /* Check if the operand is a label */
    HashItem *item = get_label(symbol_table, operand);
    if (item) {
        /* Replace the operand with the label's address in binary */
        char address_binary[WORD_SIZE];
        int label_address = item->data.label.address;
        if(item->data.label.label_sort == DAT ) {
            to_binary_address(label_address + 100 + ad->instruction_count, address_binary);
        }
        else{
            to_binary_address(label_address, address_binary);
        }
        // Update the corresponding binary word in the AssemblyData
        if (index >= 0 && index < ad->instruction_count) {
            update_first_12_bits(ad->instructions[index], address_binary);
        }
        if(item->data.label.type == LABEL_EXTERN){
            strcat(ad->instructions[index],"001");
        }
        else{
            strcat(ad->instructions[index],"010");
        }
    }
}

