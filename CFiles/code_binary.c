#include <string.h>
#include <stdio.h>
#include "../HeaderFiles/code_binary.h"



void binary_to_octal_15bit(const char *binary, char *octal) {

    int i;
    for (i = 0; i < 15; ++i) {
        if (binary[i] == '\0') {
            printf("Error: Input must be at least a 15-bit binary string.\n");

        }
    }


    char binary_15[16];
    strncpy(binary_15, binary, 15);
    binary_15[15] = '\0';

    int j, sum;
    for (i = 0; i < 5; ++i) {
        sum = 0;

        for (j = 0; j < 3; ++j) {
            sum = (sum << 1) | (binary_15[i * 3 + j] - '0');
        }
        octal[i] = sum + '0';
    }

    octal[5] = '\0';

}
