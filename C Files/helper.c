#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "../Header Files//helper.h"

char *copy_text(FILE *fp, fpos_t *pos, int str_len) {
    char *str;

    // Set the file pointer to the provided position
    if (fsetpos(fp, pos) != 0) {
        return NULL;
    }

    // Allocate memory for the content plus a null terminator
    str = malloc((str_len + 1) * sizeof(char));
    if (str == NULL) {
        return NULL;
    }

    // Read the specified number of characters from the file
    fread(str, sizeof(char), str_len, fp);
    str[str_len] = '\0'; // Null-terminate the string

    return str;
}


void *malloc_helper(long object_size){
    void *obj_ptr = malloc(object_size);
    if(obj_ptr == NULL){
        //ERROR
    }
    return obj_ptr;

}

void remove_trailing_newline(char *str) {
    size_t len = strcspn(str, "\n");
    if (len < strlen(str)) {
        str[len] = '\0';
    }
}