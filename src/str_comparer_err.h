#pragma once

#include <stdio.h>

typedef enum {
    STR_COMP_OK,
    STR_COMP_WRONG_ARGC_NUM,
    STR_COMP_WRONG_NUM_LINE,
    STR_COMP_WRONG_SORT_FUNC,
    STR_COMP_WRONG_COMPARATOR,
    STR_COMP_ALLOC_ERROR,
    STR_COMP_NOT_FOUND_INPUT_FILE,
    STR_COMP_READ_FILE_ERROR,
    STR_COMP_NOT_ENOUGH_LINES,
    STR_COMP_NOT_FOUND_OUTPUT_FILE,
    STR_COMP_WRITE_ERROR,
} str_comp_error_code_t;

extern str_comp_error_code_t sorting_error;

int print_str_comp_error(str_comp_error_code_t err);
