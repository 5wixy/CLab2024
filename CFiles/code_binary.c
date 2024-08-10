#include <string.h>
#include <stdio.h>
#include "../HeaderFiles/code_binary.h"



void binary_to_octal_15bit(const char *binary, char *octal) {
    if (strlen(binary) < 15) {
        printf("Error: Input must be at least a 15-bit binary string.\n");
        return;
    }

    // Ensure the binary string is exactly 15 bits
    char binary_15[16];  // 15 bits + null terminator
    strncpy(binary_15, binary, 15);
    binary_15[15] = '\0';  // Null-terminate the substring

    int i, j, sum;
    for (i = 0; i < 5; ++i) {
        sum = 0;
        // Convert each group of 3 binary digits to an octal digit
        for (j = 0; j < 3; ++j) {
            sum = (sum << 1) | (binary_15[i * 3 + j] - '0');
        }
        octal[i] = sum + '0';  // Store the octal digit
    }

    octal[5] = '\0';  // Null-terminate the octal string
}
