
#include "../Header Files/validation.h"
#include <stdio.h>
#include <ctype.h>
#include "../Header Files/am_handler.h"
#include "../Header Files/globals.h"
#include "../Header Files/hash_table.h"
#include "../Header Files/helper.h"
#include "../Header Files/first_pass.h"



void process_label_line(char* line, char* first_word, HashTable* table, int IC) {
    command_parts *command = malloc_helper(sizeof(command_parts));
    inst_parts *inst = malloc_helper(sizeof (inst_parts));
    char line_copy[MAX_LINE_LEN];
    strcpy(line_copy,line);
    printf(line_copy);
    skip_first_word(line);
    char *token;
    token = strtok(line, " \n");
    if(match_opcodes(token) > -1){
        process_labeled_operation(first_word,line_copy,table,IC);

    }







    }




void process_labeled_operation(char *label_name, char *line, HashTable *table, int IC) {

}


int start_first_pass(char* file_name, HashTable* table) {
    int IC = 0, DC = 0;
    HashTable *entries,*externals;
   // init_hashtable(entries);
   // init_hashtable(externals);

    command_parts *command;
    inst_parts *inst;
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
        if(strchr(str,'.')){
            if(strstr(line_copy, ".entry") || strstr(line_copy, ".extern")){



            }
        }
        else if (first_word[strlen(first_word) - 1] == ':') {
            label_flag = 1;
            process_label_line(line_copy, first_word, table, IC);
        }

        else if(is_data_instruction(first_word)){
            printf(first_word);

        }
        else{
        IC++;
            }

    }

    fclose(fp);
    return 1;
}