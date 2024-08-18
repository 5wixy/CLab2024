#include <stdio.h>
#include "../HeaderFiles/preproc.h"
#include "../HeaderFiles/first_pass.h"
#include "../HeaderFiles/hash_table.h"
#include "../HeaderFiles/helper.h"
#include "../HeaderFiles/second_pass.h"
#include "../HeaderFiles/helper.h"

int main(int argc, char *argv[]) {
    char *as_file, *am_file;
	HashTable table;
	AssemblyData ad;
    while(--argc > 0) {
        int error_flag = 0;
        init_hashtable(&table);
        as_file = create_new_file_name(argv[argc], ".as");


        if (expand_macro(as_file, &table)) {
            printf("\nAn error occurred while expanding macros\nRead logs above for further information\n########################################################\n\n");
            continue;
        }
        /* Initialize the AssemblyData structure as needed */
        init_assembly_data(&ad);

        am_file = create_new_file_name(argv[argc], ".am");
        if (start_first_pass(am_file, &table, &ad) != 0) {
            printf("\n\nAn error occurred in the first pass\nRead logs above for further information\n########################################################\n\n");
            error_flag = 1;
        }

        if (start_second_pass(am_file, &table, &ad, error_flag)) {
            printf("\n\nAn error occurred in the second pass\nRead logs above for further information\n\n########################################################\n\n");
            /*return 1; // Second pass failed */
        }
	my_free(as_file);
        my_free(am_file);
        free_macros_from_table(&table);
        freeHashTable(&table);
        free_assembly_data(&ad);
    }
    return 0;

}
