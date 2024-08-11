//
// Created by gyank on 02/07/2024.
//

#ifndef CLAB_HELPER_H
#define CLAB_HELPER_H

#include "data_strct.h"
#include <stdio.h>
char *copy_text(FILE *fp, fpos_t *pos, int len);
void *malloc_helper(int object_size, char *file, int line);
void my_free(void* ptr, const char* file, int line);
void remove_trailing_newline(char *str);
char *create_new_file_name(char *file_name, char *ending);
void close_resources(int num_args, ...);
void copy_file_content(const char *source_file, const char *destination_file, int remove_macros);
char *trim_whitespace(char *str);
void trim_whitespaces(char *str);
void trim_file_in_place(const char *file_name);
void trim_line(char *line);
void trim_file(const char *filename);
void addressing_method_to_binary(int method, char *output);
void trim_file_to_temp(const char *source_file, char *temp_file);
void clean_file(const char *input_file, const char *output_file);
void print_ascii_values(char *str);
char* remove_newline(char *str);
void to_binary(short num, char *binary_output);
void trim_spaces(char *str);
void to_binary_address(short num, char *binary_output);
void update_first_12_bits(char *current_binary, const char *new_12_bits);
void twos_complement(char* binary);
void free_all_allocations();
void to_binary_string(int num, int bits, char *output);
int get_register_value(const char *register_name);
void remove_commas(char *str);
void ascii_to_15_bit_binary(char ch, char *binary_str, int max_len);
int detect_addressing_method(char *arg);
char* extract_label(char *line);
int extract_opcode(char *line);
char* extract_src_operand(const char *line);
void init_allocation_tracker();
void remove_trailing_spaces(char *str);
char* extract_dest_operand(const char *line);
LineType determine_line_type(AssemblyLine *line,const char *str);
char** extract_data_array(AssemblyLine *line, const char *str, int *count);
char** extract_string_data(AssemblyLine *line ,const char *str);
#endif //CLAB_HELPER_H
