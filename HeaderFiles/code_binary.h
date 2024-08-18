#ifndef CLAB_CODE_BINARY_H
#define CLAB_CODE_BINARY_H


/**
 * @brief Converts a 15-bit binary string to an octal string.
 *
 * This function takes a binary string of at least 15 bits, converts it to
 * its octal representation, and stores the result in the provided octal
 * string buffer.
 *
 * @param binary A string representing a binary number of at least 15 bits.
 * @param octal A buffer where the resulting octal string (5 characters) will be stored.
 */
void binary_to_octal_15bit(const char *binary, char *octal);

/**
 * @brief Converts a short integer to its binary representation.
 *
 * This function converts a short integer to a binary string representation
 * of fixed length specified by `WORD_SIZE`.
 *
 * @param num The short integer to be converted.
 * @param binary_output A buffer where the resulting binary string will be stored.
 */
void to_binary(short num, char *binary_output);

/**
 * @brief Converts an ASCII character to a 15-bit binary string.
 *
 * This function converts an ASCII character to a 15-bit binary string.
 * The resulting binary string is zero-padded on the left to ensure a length
 * of 15 bits.
 *
 * @param ch The ASCII character to be converted.
 * @param binary_str A buffer where the resulting 15-bit binary string will be stored.
 * @param max_len The maximum length of the buffer (should be at least 15).
 */
void ascii_to_15_bit_binary(char ch, char *binary_str, int max_len);

/**
 * @brief Converts a short integer to a 12-bit binary address.
 *
 * This function converts a short integer to its binary representation, ensuring
 * that the result is a 12-bit binary string.
 *
 * @param num The short integer to be converted.
 * @param binary_output A buffer where the resulting 12-bit binary string will be stored.
 */
void to_binary_address(short num, char *binary_output);
#endif
