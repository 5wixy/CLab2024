#ifndef CLAB_CODE_BINARY_H
#define CLAB_CODE_BINARY_H
void binary_to_octal_15bit(const char *binary, char *octal);
void to_binary(short num, char *binary_output);
void to_binary_address(short num, char *binary_output);
void ascii_to_15_bit_binary(char ch, char *binary_str, int max_len);
#endif
