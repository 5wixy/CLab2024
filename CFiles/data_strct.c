#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "../HeaderFiles/data_strct.h"
#include "../HeaderFiles/globals.h"
#include "../HeaderFiles/helper.h"
#include "../HeaderFiles/am_handler.h"

void init_assembly_line(AssemblyLine *line) {
    line->label = NULL;
    line->opcode = -1;
    line->src_operand = NULL;
    line->dest_operand = NULL;
    line->type = INSTRUCTION;
    line->data_array = NULL;
    line->data_count = 0;
}

// Free the memory allocated for an AssemblyLine struct
void free_assembly_line(AssemblyLine *line) {
    if (line->label) free(line->label);
    if (line->src_operand) free(line->src_operand);
    if (line->dest_operand) free(line->dest_operand);

    // Free data_array if it exists
    if (line->data_array) {
        free(line->data_array);
    }
}

// Helper function to check if a string is a label
int is_label(const char *str) {
    if (!str || !(*str)) return 0;
    if (isdigit(*str)) return 0;
    int i;
    for (i = 0; str[i]; i++) {
        if (!isalnum(str[i]) && str[i] != '_') return 0;
    }
    return str[strlen(str) - 1] == ':';
}

// Function to parse an assembly line string into an AssemblyLine struct
int parse_assembly_line(const char *line_str, AssemblyLine *line) {
    char line_copy[MAX_LINE_LEN];
    strncpy(line_copy, line_str, MAX_LINE_LEN - 1);
    line_copy[MAX_LINE_LEN - 1] = '\0'; // Ensure null-termination

    // Tokenize the line
    line->label = extract_label(line_copy);
    line->opcode = extract_opcode(line_copy);
    line->src_operand = extract_src_operand(line_copy);
    strncpy(line_copy, line_str, MAX_LINE_LEN - 1);
    line_copy[MAX_LINE_LEN - 1] = '\0'; // Ensure null-termination
    line->dest_operand = extract_dest_operand(line_copy);
    line->type = determine_line_type(line, line_copy);

    if (line->type == DATA) {
        line->data_array = extract_data_array(line, line_copy, &line->data_count);
    } else if (line->type == STRING) {
        line->data_array = extract_string_data(line, line_copy);
    } else {
        line->data_array = NULL;
        line->data_count = 0;
    }
    if (line->src_operand && line->dest_operand == NULL && line->opcode > -1) {
        swap_src_dest(line);
    }
}