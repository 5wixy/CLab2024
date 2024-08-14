#include <string.h>
#include <stdio.h>
#include "../HeaderFiles/code_binary.h"
#include "../HeaderFiles/globals.h"



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
void to_binary(short num, char *binary_output) {
    int i;
    for (i = 0; i < WORD_SIZE; ++i) {
        binary_output[i] = (num & (1 << (WORD_SIZE-1 - i))) ? '1' : '0';
    }
    binary_output[WORD_SIZE] = '\0'; /* Null-terminate the binary string */
}
void ascii_to_15_bit_binary(char ch, char *binary_str, int max_len) {
    int i;

    /* Ensure the buffer is zeroed out */
    memset(binary_str, '0', max_len);
    binary_str[WORD_SIZE] = '\0';  /* Null-terminate the string */

    unsigned char byte = (unsigned char)ch;
    for (i = 7; i >= 0; i--) {
        binary_str[14 - i] = (byte & (1 << i)) ? '1' : '0';
    }
}

void to_binary_address(short num, char *binary_output) {
    int i;
    for (i = 0; i < 12; ++i) {
        binary_output[i] = (num & (1 << (12-1 - i))) ? '1' : '0';
    }
    binary_output[12] = '\0'; /* Null-terminate the binary string */
}