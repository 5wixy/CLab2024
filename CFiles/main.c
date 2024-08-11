#include <stdio.h>
#include "../HeaderFiles/preproc.h"
#include "../HeaderFiles/first_pass.h"
#include "../HeaderFiles/hash_table.h"
#include "../HeaderFiles/helper.h"
#include "../HeaderFiles/second_pass.h"
#include "../HeaderFiles/helper.h"

int main() {
    HashTable table;
    init_hashtable(&table);



    char *file_name = "D:\\CLab\\valid_input_3.as";
    if (expand_macro(file_name, &table) != 0) {
        return 1; /* Error occurred */
    }
    AssemblyData ad;
    /* Initialize the AssemblyData structure as needed */
    init_assembly_data(&ad);

    char *am_file_name = "D:\\CLab\\valid_input_3.am";
    if (start_first_pass(am_file_name, &table, &ad) != 0) {
        /*return 1; // First pass failed */
    }

    if (start_second_pass(am_file_name, &table, &ad)) {
        /*return 1; // Second pass failed */
    }
    freeHashTable(&table);
    free_assembly_data(&ad);
    return 0;
}