//
// Created by gyank on 01/08/2024.
//

#ifndef CLAB_AM_HANDLER_H
#define CLAB_AM_HANDLER_H
typedef struct command_parts {
    char *label;     /* The label associated with the command */
    int opcode;      /* The opcode of the command */
    char *source;    /* The source operand of the command */
    char *dest;      /* The destination operand of the command */
} command_parts;
typedef struct inst_parts {
    char *label;       /* The label associated with the instruction */
    short *nums;       /* An array of short integers representing instruction data */
    int len;           /* The number of data in *nums including '0' at the end of a string */
    char *arg_label;   /* The label associated with an argument, if applicable */
    int is_extern;     /* Indicates whether the instruction is marked as external */
} inst_parts;

void break_line_to_words(char *str, char *tokens[], int max_tokens);
char* get_first_word(char *str);
void skip_first_word(char *line_copy);
int match_opcodes(char *str);
int find_register_index( char *reg_name);
int is_data_instruction(char *word);
#endif //CLAB_AM_HANDLER_H
