#ifndef PREPROC_H
#define PREPROC_H

int open_read_file( char *file_name);
char *save_macro_content(FILE *fp, fpos_t *pos,int *line_count);

#endif // PREPROC_H