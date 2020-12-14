#include "str_comparer_err.h"

str_comp_error_code_t sorting_error = STR_COMP_OK;

const char *STR_COMP_ERROR_MESSAGES[] = {
        "Worked correctly",
        "Expected to receive 5 arguments: <number of lines> <input filename> <output filename> <sorting name> <comparer name>",
        "The 1st argument <number of lines> isn't a number",
        "The 4th argument <sorting name> must be \"bubble\", \"insertion\", \"merge\", \"quick\", or \"radix\"",
        "The 5th argument <comparer name> must be \"asc\" or \"des\"",
        "Can't allocate memory",
        "Invalid 2nd argument <input filename>, file not found",
        "Read file error - fgets() error",
        "Fewer lines in the file than the 1st argument <number of lines>",
        "Invalid 3rd argument <output filename>, file not found",
        "Write file error - fputs() error",
};

int print_str_comp_error(str_comp_error_code_t err) {
    if (err) {
        fprintf(stderr, "Error: %s\n", STR_COMP_ERROR_MESSAGES[err]);
        return -1;
    }
    fprintf(stdout, "%s\n", STR_COMP_ERROR_MESSAGES[err]);
    return 0;
}
