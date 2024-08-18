
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
	/*Directing function to process correct structure */
    if (line->opcode > -1) {
        process_operation_line(line, table, IC, ad);
    } else if (line->type == STRING) {
        process_string_directive(line, table, DC, ad);
    } else if (line->type == DATA) {
        process_data_directive(line, table, DC, ad);
    }
}


void process_labeled_operation(AssemblyLine *line, HashTable *table, int *IC, AssemblyData *ad) {
    int i,j;
    int num_words = calculate_increment(line->src_operand, line->dest_operand);
	char **binary_output = malloc(num_words * sizeof(char *));
    HashItem *item = NULL;

    if (!line) {
        fprintf(stderr, "Error: line is NULL\n");
        return;
    }

    if (line->label != NULL) {
        item = get_label(table, line->label);
    }

    if (item != NULL) {
        if (item->type == TYPE_LABEL) {
            /* Update the existing label's address */
            item->data.label.address = *IC;
        } else if (item->type == TYPE_MACRO) {
            /* It's a macro, so we insert the new label and push the macro to the next position */
            insert_label(table, line->label, *IC, 0, INST);
            item = get_label(table, line->label);
            item->next = item->next;
        }
    } else {
        /* Insert the label if it doesn't exist */
        if (line->label != NULL) {
            insert_label(table, line->label, *IC, 0, INST);
        }
    }

    if (num_words < 0) {
        fprintf(stderr, "Error calculating number of words\n");
        return;
    }

    /* Allocate memory for binary_output */
    
    if (!binary_output) {
        fprintf(stderr, "Memory allocation failed for binary_output\n");
        return;
    }
	/*Allocate memory for the binary words */
    for (i = 0; i < num_words; ++i) {
        binary_output[i] = malloc((WORD_SIZE + 1) * sizeof(char));
        if (!binary_output[i]) {
            fprintf(stderr, "Memory allocation failed for binary_output[%d]\n", i);

            /* Free previously allocated memory */
            for (j = 0; j < i; ++j) {
                free(binary_output[j]);
            }
            free(binary_output);
            return;
        }
    }
    transform_to_binary(line, binary_output, &num_words, table);
	/*Insert to correct array by the attributes */
    if (line->type == INSTRUCTION) {
        for (i = 0; i < num_words; i++) {
            insert_instruction(ad, binary_output[i]);
        }
    }

    if (line->type == DATA) {
        for (i = 0; i < num_words; i++) {
            add_data(ad, binary_output[i], strlen(line->src_operand));
        }
    }

    increment_IC(*line, IC);

    /* Free the dynamically allocated memory */
    for (i = 0; i < num_words; ++i) {
        free(binary_output[i]);
    }
    free(binary_output);
}

    int start_first_pass(char *file_name, HashTable *symbol_table, AssemblyData *ad) {
        int IC = 100; /*Init values */
        int DC = 0;
        int line_num = 1;
        int error_status = 0;
        char line_copy[MAX_LINE_LEN];
        char str[MAX_LINE_LEN];
        AssemblyLine *line;
        FILE *fp = fopen(file_name, "r");
        if (!fp) {
            perror("Error opening file");
            return -1;
        }
        while (fgets(str, sizeof(str), fp)) {
            strcpy(line_copy, str);
            if (str[0] == '\n' || str[0] == ';') { /*Skip empty or comment line */
                line_num++;
                continue;
            }
            line = malloc_helper(sizeof(AssemblyLine));
            if (line == NULL) {
                fprintf(stderr, "Memory allocation failed for AssemblyLine\n");
                fclose(fp);
                return -1;
            }
            remove_trailing_newline(str);

			/*Parse the assembly line to the data structure */
            parse_assembly_line(str, line);
            if(check_line_error(*line, line_copy, line_num,*symbol_table)){
                error_status = 1;
            }

			/*Process entry/extern directives*/
            if(is_valid_entry_or_extern_line(str, line_num)){
                if (is_entry_or_extern(str, symbol_table,line_num)) {
                    free_assembly_line(line);
                    line_num++;
                    continue;
                    }
                else{
                    free_assembly_line(line);
                    continue;
                }
            }

			/*Process the assembly line*/
            process_label_line(line, symbol_table, &IC, &DC, ad);
            free_assembly_line(line);
            line_num++;
        }

        fclose(fp);
        if(error_status){
            return ERROR;
        }
        return OK;
    }
    void process_extern(char *label_name, HashTable *symbol_table, int line_num) {

    /* Retrieve the label from the symbol table */
    HashItem *item = get_label(symbol_table, label_name);

    /* Check if the label already exists in the symbol table */
    if (item != NULL) {
        /* If the label is an entry label, report an error */
        if (item->data.label.type == LABEL_ENTRY) {
            print_error(ERR_CANNOT_ASSIGN_ENTRY_LABEL_AS_EXTERN, line_num);
            return;
        }
    }

    /* Insert the label into the symbol table as an extern label */
    insert_label(symbol_table, label_name, 0, LABEL_EXTERN, -1);
}

    void process_entry(char *label_name, HashTable *table,int line_num) {
        HashItem *item = get_label(table, label_name);

        if (item != NULL) {
            /* If the label exists, update the `is_extern` field to 0 */
            if(item->data.label.type == LABEL_EXTERN){
                print_error(ERR_CANNOT_ASSIGN_EXTERN_LABEL_AS_ENTRY, line_num);
                return;
            } else {
                /* Insert the new label into the hash table with address 0 */
                item->data.label.type = LABEL_ENTRY;
            }

        } else {

            /* Insert the new label into the hash table with address 0 */
            insert_label(table, label_name, 0, LABEL_ENTRY, -1);
        }
    }

int is_valid_entry_or_extern_line(char *str, int line_num) {
    char *directive;
    char *label;
    char *remainder;


    skip_leading_space(&str);

    /* Check if the line starts with .entry or .extern*/
    if (strncmp(str, ".entry", 6) == 0) {
        directive = ".entry";
        str += 6;
    } else if (strncmp(str, ".extern", 7) == 0) {
        directive = ".extern";
        str += 7;
    } else {
        return 0; /* Not an entry or extern directive */
    }


    skip_leading_space(&str);

    /* Extract the label*/
    label = strtok(str, " \n");

    if (label == NULL) {

        return 0;
    }


    remainder = strtok(NULL, " \n");
	/*Check for extra text*/
    if (remainder != NULL) {
        printf("Warning: Extra text after label in %s directive on line %d\n", directive, line_num);
        return 0;
    }


    return 1;
}

