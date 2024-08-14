#include "data_strct.h"

#ifndef CLAB_ERROR_CHECKER_H
#define CLAB_ERROR_CHECKER_H

int check_assembly_line(AssemblyLine asm_line, char *line, int line_num);
int is_valid_integer(char *str);
int check_data_directive(char *line,int line_num);
int check_line_error(AssemblyLine asm_line, char *line, int line_num);
int check_operands(AssemblyLine asm_line, int line_num);
int check_operand_amount(AssemblyLine asm_line, int line_num);
int check_valid_imm_operand(char *operand,int line_num);
int is_valid_operand(AssemblyLine asm_line, int line_num);
#endif
