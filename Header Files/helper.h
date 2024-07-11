//
// Created by gyank on 02/07/2024.
//

#ifndef CLAB_HELPER_H
#define CLAB_HELPER_H
char *copy_text(FILE *fp,fpos_t *pos,int len);
void *malloc_helper(long object_size);
char* remove_trailing_newline(char *str);
char *create_new_file_name(char *file_name, char *ending);
void close_resources(int num_args, ...);
void copy_file_content(const char *source_file, const char *destination_file, int remove_macros);
void print_ascii_values(char *str);
char* remove_newline(char *str);
#endif //CLAB_HELPER_H
