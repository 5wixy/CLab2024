#ifndef CLAB_ASM_DATA_H
#define CLAB_ASM_DATA_H
typedef struct {
    char **instructions;
    int *instruction_sizes;
    int instruction_count;
    int instruction_capacity;

    char **data;
    int *data_sizes;
    int data_count;
    int data_capacity;

    int IC;  /* Instruction Counter */
    int DC;  /* Data Counter */
} AssemblyData;

void init_assembly_data(AssemblyData *ad);
void free_assembly_data(AssemblyData *ad);
void add_data(AssemblyData *ad, const char *data, int size);
void insert_instruction(AssemblyData *ad, char *instruction);
#endif
