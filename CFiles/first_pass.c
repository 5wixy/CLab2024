
#include "../HeaderFiles/validation.h"
#include <stdio.h>
#include <ctype.h>
#include "../HeaderFiles/am_handler.h"
#include "../HeaderFiles/globals.h"
#include "../HeaderFiles/hash_table.h"
#include "../HeaderFiles/helper.h"
#include "../HeaderFiles/first_pass.h"
#include "../HeaderFiles/asm_data.h"
#include "../HeaderFiles/data_strct.h"
#include "../HeaderFiles/validation.h"


void process_label_line(AssemblyLine *line, HashTable *table, int *IC, int *DC, AssemblyData *ad) {
    if (line->opcode > -1) {
        process_operation_line(line, table, IC, ad);
    } else if (line->type == STRING) {
        process_string_directive(line, table, DC, ad);
    } else if (line->type == DATA) {
        process_data_directive(line, table, DC, ad);
    } else {
        printf("ERROR: Unknown directive or operation.\n");
    }
}


void process_labeled_operation(AssemblyLine *line, HashTable *table, int *IC, AssemblyData *ad) {
    int i;
    if (!line) {
        fprintf(stderr, "Error: line is NULL\n");
        return;
    }

    HashItem *item = NULL;

    /*If the line has a label, check if it exists in the hash table */
    if (line->label != NULL) {
        item = get_label(table, line->label);  /* Use a function that returns a HashItem* */
    }

    if (item != NULL) {
        /* If the label already exists, update its address*/
        item->data.label.address = *IC;
    } else if (line->label != NULL) {
        /* If the label does not exist, insert it with the current IC */
        insert_label(table, line->label, *IC, 0,INST);
    }

    /* Validate operands and calculate the number of words needed for the operation */
    int num_words = calculate_increment(line->src_operand, line->dest_operand);
    int slots = num_words;
    char binary_output[num_words][WORD_SIZE];

    /* Transform the operation into binary */
    transform_to_binary(line, binary_output, &slots, table);
    /* Store the binary words in the AssemblyData structure */
    for (i = 0; i < num_words; i++) {
        insert_instruction(ad, binary_output[i]);
    }

    /* Increment the IC based on the operation */
    increment_IC(*line, IC);
}


int start_first_pass(char *file_name, HashTable *symbol_table, AssemblyData *ad) {
    int IC = 100;
    int DC = 0;

    FILE *fp = fopen(file_name, "r");
    if (!fp) {
        perror("Error opening file");
        return -1;
    }
    char line_copy[MAX_LINE_LEN];
    char str[MAX_LINE_LEN];
    while (fgets(str, sizeof(str), fp)) {
       strcpy(line_copy,str);
        AssemblyLine *line = malloc_helper(sizeof(AssemblyLine));
        if (line == NULL) {
            fprintf(stderr, "Memory allocation failed for AssemblyLine\n");
            continue;
        }
        remove_trailing_newline(str);

        if (is_entry_or_extern(str, symbol_table, &IC)) {
            /* If it was an .entry or .extern, continue to the next line */
            continue;
        }
        /* Initialize the AssemblyLine struct */
        parse_assembly_line(str,line);
        /* Process the line using the populated AssemblyLine struct*/
        process_label_line(line, symbol_table, &IC, &DC, ad);
        /* Free the AssemblyLine struct */
        free_assembly_line(line);
    }

    fclose(fp);

    return 1;
}
void process_extern(char *label_name, HashTable *symbol_table) {

    insert_label(symbol_table, label_name, 0,  LABEL_EXTERN,-1);



}
void process_entry(char *label_name, HashTable *table, int *IC) {
    HashItem *item = get_label(table, label_name);

    if (item != NULL) {
        /* If the label exists, update the `is_extern` field to 0 */
        item->data.label.type = LABEL_ENTRY;
    } else {

        /* Insert the new label into the hash table with address 0 */
        insert_label(table, label_name, 0, LABEL_ENTRY,-1);
    }
}









