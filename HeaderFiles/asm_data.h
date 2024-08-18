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


/**
 * @brief Initializes an AssemblyData struct.
 *
 * @param ad The pointer AssemblyData struct to initialize.
 */
void init_assembly_data(AssemblyData *ad);

/**
 * @brief Frees the memory allocated for an AssemblyData struct.
 *
 * @param ad The pointer to the AssemblyData struct to free.
 */
void free_assembly_data(AssemblyData *ad);

/**
 * @brief Adds data to the AssemblyData struct.
 *
 * @param ad The pointer to the AssemblyData struct.
 * @param data The data to add to the ad->data array.
 * @param size The size of the data.
 */
void add_data(AssemblyData *ad, const char *data, int size);

/**
 * @brief Adds an instruction to the AssemblyData struct.
 *
 * @param ad The pointer to the AssemblyData struct.
 * @param instruction The binary instruction to add to the ad->instructions array.
 */
void insert_instruction(AssemblyData *ad, char *instruction);
#endif
