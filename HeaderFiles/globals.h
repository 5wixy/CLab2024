#ifndef CLAB_GLOBALS_H
#define CLAB_GLOBALS_H
#define MAX_LINE_LEN 81
#define TABLE_SIZE 100
#define OP_ARR_SIZE 16
#define REG_ARR_SIZE 8
#define TYPE_ARR_SIZE 4
#define INVALID_OPCODE -1
#define MAX_MACRO_NAME_LEN 31
#define MAX_LABEL_NAME_LEN 31
#define MAX_SIZE_IMMEDIATE_OPERAND_POS 2047
#define MIN_SIZE_IMMEDIATE_OPERAND_NEG -2048
#define MAX_SIZE_IMMEDIATE_DATA_POS 16383
#define MIN_SIZE_IMMEDIATE_DATA_NEG -16384
#define IMMEDIATE 0
#define DIRECT 1
#define REGISTER_POINTER 2
#define REGISTER_DIRECT 3
#define MEMORY_SIZE 4096
#define MAX_FREE_MEMORY 3996
#define WORD_SIZE 15
#define INITIAL_EXTERN_OPERANDS_CAPACITY 10


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
#define R0_VALUE 0
#define R1_VALUE 1
#define R2_VALUE 2
#define R3_VALUE 3
#define R4_VALUE 4
#define R5_VALUE 5
#define R6_VALUE 6
#define R7_VALUE 7

typedef struct {
    const char *name;
    int value;
} Register;


#define INITIAL_ALLOCATION_CAPACITY 10

typedef struct {
    void **allocations;
    size_t count;
    size_t capacity;
} AllocationTracker;



#endif
