#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "../HeaderFiles/data_strct.h"
#include "../HeaderFiles/globals.h"
#include "../HeaderFiles/helper.h"
#include "../HeaderFiles/am_handler.h"





	
/* Free the memory allocated for an AssemblyLine struct */
void free_assembly_line(AssemblyLine *line) {
    int i;


    if (line->label) {
        my_free(line->label);
    }


    if (line->src_operand) {
        my_free(line->src_operand);
    }


    if (line->dest_operand) {
        my_free(line->dest_operand);
    }


    if (line->data_array) {
        for (i = 0; i < line->data_count; ++i) {
            if (line->data_array[i]) {
                my_free(line->data_array[i]);
            }
        }
        my_free(line->data_array);
    }


    my_free(line);
}

/* Function to parse an assembly line string into an AssemblyLine struct */
int parse_assembly_line(const char *line_str, AssemblyLine *line) {
    char line_copy[MAX_LINE_LEN];
    strncpy(line_copy, line_str, MAX_LINE_LEN - 1);
    line_copy[MAX_LINE_LEN - 1] = '\0';
    line->label = extract_label(line_copy);

    line->opcode = extract_opcode(line_copy);
    line->src_operand = extract_src_operand(line_str);

    strncpy(line_copy, line_str, MAX_LINE_LEN - 1);
    line_copy[MAX_LINE_LEN - 1] = '\0';
    line->dest_operand = extract_dest_operand(line_copy);
    line->type = determine_line_type(line, line_copy);

    if (line->type == DATA) {
        line->data_array = extract_data_array(line_copy, &line->data_count);
    } else if (line->type == STRING) {
        line->data_array = extract_string_data(line, line_copy);
    } else {
        line->data_array = NULL;
        line->data_count = 0;
    }
	/* In the case where there is only one operand they are swapped 
		in order to address the operand as a destination*/
    if (line->src_operand && line->dest_operand == NULL && line->opcode > -1) {
        swap_src_dest(line);
    }

    return 0;
}
