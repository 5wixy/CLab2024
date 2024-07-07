//
// Created by gyank on 02/07/2024.
//

#ifndef CLAB_HELPER_H
#define CLAB_HELPER_H
char *copy_text(FILE *fp,fpos_t *pos,int len);
void *malloc_helper(long object_size);
void remove_trailing_newline(char *str);
#endif //CLAB_HELPER_H
