#include <stdio.h>
#include "../Header Files/preproc.h"
//#include "preproc.c"

int main() {
    const char *file_name = "D:\\CLab\\basic_test.as";
    if (open_read_file(file_name) != 0) {
        return 1; // Error occurred
    }
    return 0;
}