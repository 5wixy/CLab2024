#include <string.h>
#include <stdio.h>
#include "../HeaderFiles/code_binary.h"
#include "../HeaderFiles/globals.h"



void binary_to_octal_15bit(const char *binary, char *octal) {

    char binary_15[16];  /* Buffer to store the first 15 bits of the binary string with a null terminator */

    int i, j, sum;     

    /* Copy the first 15 bits from binary input and ensure it's null-terminated */
    strncpy(binary_15, binary, 15);
    binary_15[15] = '\0';

    /* Convert each group of 3 binary digits into an octal digit */
    for (i = 0; i < 5; ++i) {  /* There are 5 groups of 3 binary digits */
        sum = 0;  /* Reset sum to accumulate the binary value */

        for (j = 0; j < 3; ++j) {
            sum = (sum << 1) | (binary_15[i * 3 + j] - '0');  /* Shift and add binary digit */
        }

        octal[i] = sum + '0';  /* Convert decimal to ASCII character */
    }

    /* Null-terminate the octal string */
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
    unsigned char byte;
    /* Ensure the buffer is zeroed out */
    memset(binary_str, '0', max_len);
    binary_str[WORD_SIZE] = '\0';  /* Null-terminate the string */

    byte = (unsigned char)ch;
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
