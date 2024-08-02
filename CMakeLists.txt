cmake_minimum_required(VERSION 3.27)
project(CLab C)

set(CMAKE_C_STANDARD 90)

add_executable(CLab "C Files/main.c"
        "C Files/preproc.c"
        "Header Files/globals.h"
        "Header Files/preproc.h"
        "C Files/helper.c"
        "Header Files/helper.h"
        "C Files/macro.c"
        "Header Files/macro.h"
        "C Files/hash_table.c"
        "Header Files/hash_table.h"
        "C Files/validation.c"
        "Header Files/validation.h"
        "C Files/first_pass.c"
        "Header Files/first_pass.h"
        "C Files/am_handler.c"
        "Header Files/am_handler.h"
        "C Files/code_binary.c"
        "C Files/code_binary.h")
