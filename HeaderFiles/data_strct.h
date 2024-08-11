#ifndef CLAB_DATA_STRCT_H
#define CLAB_DATA_STRCT_H
typedef enum {
    INSTRUCTION,
    DATA,
    STRING
} LineType;

typedef struct {
    char *label;
    int opcode;
    char *src_operand;
    char *dest_operand;
    LineType type;
    char **data_array;
    int data_count;
} AssemblyLine;
typedef struct {
    char *label_name;
    int address;
} ExternOperand;


void init_assembly_line(AssemblyLine *line);
void free_assembly_line(AssemblyLine *line);
int parse_assembly_line(const char *line_str, AssemblyLine *line);
#endif
