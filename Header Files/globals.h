//
// Created by gyank on 30/06/2024.
//

#ifndef CLAB_GLOBALS_H
#define CLAB_GLOBALS_H
#define MAX_LINE_LEN 80
#define TABLE_SIZE 100
#define OP_ARR_SIZE 16
#define REG_ARR_SIZE 8
#define TYPE_ARR_SIZE 4
#define MAX_MACRO_NAME_LEN 31
#define MAX_LABEL_NAME_LEN 31
#define IMMEDIATE 0
#define DIRECT 1
#define REGISTER_POINTER 2
#define MEMORY_SIZE 4096
#define WORD_SIZE 15
#define REGISTER_DIRECT 3
typedef enum {
    MOV = 0,
    CMP = 1,
    ADD = 2,
    SUB = 3,
    LEA = 4,
    CLR = 5,
    NOT = 6,
    INC = 7,
    DEC = 8,
    JMP = 9,
    BNE = 10,
    RED = 11,
    PRN = 12,
    JSR = 13,
    RTS = 14,
    STOP = 15
} OPCODE_NAME;
#endif //CLAB_GLOBALS_H
