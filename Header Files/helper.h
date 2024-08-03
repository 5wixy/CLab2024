//
// Created by gyank on 02/07/2024.
//

#ifndef CLAB_HELPER_H
#define CLAB_HELPER_H
char *copy_text(FILE *fp,fpos_t *pos,int len);
void *malloc_helper(long object_size);
void remove_trailing_newline(char *str);
char *create_new_file_name(char *file_name, char *ending);
void close_resources(int num_args, ...);
void copy_file_content(const char *source_file, const char *destination_file, int remove_macros);
void print_ascii_values(char *str);
char* remove_newline(char *str);
void remove_commas(char *str);
void ascii_to_15_bit_binary(char ch, char *binary_str, size_t max_len);
int detect_addressing_method(char *arg);
void fill_nums_array(const char *line, short *nums, int count);
void extract_operands(const char *line, char *source, char *dest);
size_t count_15_bit_words(size_t total_length);
#endif //CLAB_HELPER_H
