#include <stdio.h>
#include "../Header Files/preproc.h"
#include "../Header Files/first_pass.h"
#include "../Header Files/hash_table.h"

int main() {
    HashTable table;
    init_hashtable(&table);
    char *file_name = "D:\\CLab\\basic_test.as";
    if (expand_macro(file_name,&table) != 0) {
        return 1; // Error occurred
    }
    char *am_file_name = "D:\\CLab\\basic_test.am";
    if(start_first_pass(am_file_name,&table)){

    }
    return 0;
}