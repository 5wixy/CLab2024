cmake_minimum_required(VERSION 3.27)
project(CLab C)

set(CMAKE_C_STANDARD 90)

add_executable(CLab "CFiles/main.c"
        "CFiles/preproc.c"
        "HeaderFiles/globals.h"
        "HeaderFiles/preproc.h"
        "CFiles/helper.c"
        "HeaderFiles/helper.h"
        "CFiles/macro.c"
        "HeaderFiles/macro.h"
        "CFiles/hash_table.c"
        "HeaderFiles/hash_table.h"
        "CFiles/validation.c"
        "HeaderFiles/validation.h"
        "CFiles/first_pass.c"
        "HeaderFiles/first_pass.h"
        "CFiles/am_handler.c"
        "HeaderFiles/am_handler.h"
        "CFiles/code_binary.c"
        "HeaderFiles/code_binary.h"
        "CFiles/asm_data.c"
        "HeaderFiles/asm_data.h"
        CFiles/second_pass.c
        HeaderFiles/second_pass.h
        CFiles/data_strct.c
        HeaderFiles/data_strct.h
        CFiles/Errors.c
        HeaderFiles/Errors.h
        CFiles/error_checker.c
        HeaderFiles/error_checker.h
)
