
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
#include "../HeaderFiles/Errors.h"
#include "../HeaderFiles/error_checker.h"


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

    if (line->label != NULL) {
        item = get_label(table, line->label);
    }

    if (item != NULL) {
        item->data.label.address = *IC;
    }  if (line->label != NULL) {
        insert_label(table, line->label, *IC, 0, INST);
    }

    int num_words = calculate_increment(line->src_operand, line->dest_operand);
    if (num_words < 0) {
        fprintf(stderr, "Error calculating number of words\n");
        return;
    }

    char binary_output[num_words][WORD_SIZE];
    transform_to_binary(line, binary_output, &num_words, table);


    if (line->type == INSTRUCTION) {
        for (i = 0; i < num_words; i++) {
            insert_instruction(ad, binary_output[i]);
        }
        if (line->type == DATA) {
            for (i = 0; i < num_words; i++) {
                add_data(ad, binary_output[i], strlen(line->src_operand));
            }
        }

        increment_IC(*line, IC);
    }
}

    int start_first_pass(char *file_name, HashTable *symbol_table, AssemblyData *ad) {
        int IC = 100;
        int DC = 0;
        int line_num = 1;
        FILE *fp = fopen(file_name, "r");
        if (!fp) {
            perror("Error opening file");
            return -1;
        }
        char line_copy[MAX_LINE_LEN];
        char str[MAX_LINE_LEN];
        while (fgets(str, sizeof(str), fp)) {
            strcpy(line_copy, str);
            AssemblyLine *line = malloc_helper(sizeof(AssemblyLine), __FILE_NAME__, __LINE__);
            if (line == NULL) {
                fprintf(stderr, "Memory allocation failed for AssemblyLine\n");
                fclose(fp);
                return -1;
            }
            remove_trailing_newline(str);


            parse_assembly_line(str, line);
            check_line_error(*line, line_copy, line_num);
            if (is_entry_or_extern(str, symbol_table, &IC)) {
                free_assembly_line(line);
                line_num++;
                continue;
            }
            process_label_line(line, symbol_table, &IC, &DC, ad);
            free_assembly_line(line);
            line_num++;
        }

        fclose(fp);
        return 0;
    }
    void process_extern(char *label_name, HashTable *symbol_table) {

        insert_label(symbol_table, label_name, 0, LABEL_EXTERN, -1);


    }
    void process_entry(char *label_name, HashTable *table, int *IC) {
        HashItem *item = get_label(table, label_name);

        if (item != NULL) {
            /* If the label exists, update the `is_extern` field to 0 */
            item->data.label.type = LABEL_ENTRY;
        } else {

            /* Insert the new label into the hash table with address 0 */
            insert_label(table, label_name, 0, LABEL_ENTRY, -1);
        }
    }


