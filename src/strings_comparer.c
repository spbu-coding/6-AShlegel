#include <stdlib.h>
#include "sortings.h"
#include "str_comparer_err.h"

typedef void (*sort_func_t)(strings_array_t, array_size_t, comparator_func_t);

typedef struct {
    array_size_t num_line;
    char *input_filename;
    char *output_filename;
    sort_func_t sort_func;
    comparator_func_t comparator;
} input_data_t;

/*------------------------------------------------------------------------------------------------------ SORT METHODS */

static const char *const SORT_NAMES[] = {
        "bubble",
        "insertion",
        "merge",
        "quick",
        "radix",
};

static const size_t NUM_SORT = sizeof SORT_NAMES / sizeof *SORT_NAMES;

static const sort_func_t SORT_FUNC[] = {
        bubble,
        insertion,
        merge,
        quick,
        radix,
};

/*----------------------------------------------------------------------------------------------------- SORT COMPARER */

static const char *const CMP_NAMES[] = {
        "asc",
        "des",
};

static const size_t NUM_CMP = sizeof CMP_NAMES / sizeof *CMP_NAMES;

int asc_comparator(const char *s1, const char *s2);

int des_comparator(const char *s1, const char *s2);

static const comparator_func_t CMP_FUNC[] = {
        asc_comparator,
        des_comparator,
};

/*------------------------------------------------------------------------------------------------------ CAPTURE KEYS */

#define SUPPORT_ARGC 6

int get_num_line(const char *line, array_size_t *num_line) {
    *num_line = 0;
    char c = line[0];
    if (c == '0' && line[1] != '\0') return 1;
    for (int i = 0; c != '\0'; i++, c = line[i]) {
        if (c >= '0' && c <= '9') *num_line = *num_line * 10 + (c - '0');
        else return 1;
    }
    return 0;
}

str_comp_error_code_t get_input_data(int argc, char **argv, input_data_t *input_data) {
    if (argc != SUPPORT_ARGC) return STR_COMP_WRONG_ARGC_NUM;

    if (get_num_line(argv[1], &input_data->num_line)) return STR_COMP_WRONG_NUM_LINE;

    input_data->input_filename = argv[2];
    input_data->output_filename = argv[3];

    char *sort_name = argv[4];
    for (size_t i = 0; i < NUM_SORT; i++) {
        if (strcmp(sort_name, SORT_NAMES[i]) == 0) {
            input_data->sort_func = SORT_FUNC[i];
            break;
        }
    }
    if (input_data->sort_func == NULL) return STR_COMP_WRONG_SORT_FUNC;

    char *cmp_name = argv[5];
    for (size_t i = 0; i < NUM_CMP; i++) {
        if (strcmp(cmp_name, CMP_NAMES[i]) == 0) {
            input_data->comparator = CMP_FUNC[i];
            break;
        }
    }
    if (input_data->comparator == NULL) return STR_COMP_WRONG_COMPARATOR;

    return STR_COMP_OK;
}

#undef SUPPORT_ARGC

/*------------------------------------------------------------------------------------------------------ FREE STRINGS */

void free_strings(strings_array_t strings_array, size_t size) {
    if (!strings_array) return;
    for (size_t i = 0; i < size; i++) free(strings_array[i]);
    free(strings_array);
    strings_array = NULL;
}

/*---------------------------------------------------------------------------------------------- READ / WRITE STRINGS */

str_comp_error_code_t read_strings(input_data_t *input_data, strings_array_t array) {
    FILE *file = fopen(input_data->input_filename, "r");
    if (file == NULL) {
        return STR_COMP_NOT_FOUND_INPUT_FILE;
    }
    for (array_size_t i = 0; i < input_data->num_line; i++) {
        if (fgets(array[i], MAX_INPUT_STRING_SIZE, file) == NULL) {
            if (feof(file)) {
                fclose(file);
                return STR_COMP_NOT_ENOUGH_LINES;
            }
            fclose(file);
            return STR_COMP_READ_FILE_ERROR;
        }
    }
    if (input_data->num_line && !strchr(array[input_data->num_line - 1], '\n'))
        strcat(array[input_data->num_line - 1], "\n");
    fclose(file);
    return STR_COMP_OK;
}

int write_strings(input_data_t *input_data, strings_array_t array) {
    FILE *file = fopen(input_data->output_filename, "w");
    if (file == NULL) {
        return STR_COMP_NOT_FOUND_OUTPUT_FILE;
    }
    if (input_data->num_line != 0) {
        for (array_size_t i = 0; i < input_data->num_line; i++) {
            if (fputs(array[i], file) == EOF) {
                fclose(file);
                return STR_COMP_WRITE_ERROR;
            }
        }
    } else {
        if (fputs("\n", file) == -1) {
            fclose(file);
            return STR_COMP_WRITE_ERROR;
        }
    }
    fclose(file);
    return STR_COMP_OK;
}

/*-------------------------------------------------------------------------------------------------- STRINGS COMPARER */

str_comp_error_code_t string_comparer(input_data_t *input_data) {

    strings_array_t strings_array = NULL;
    if (input_data->num_line) {
        strings_array = calloc(input_data->num_line, sizeof *strings_array);
        if (!strings_array) return STR_COMP_ALLOC_ERROR;
        for (array_size_t i = 0; i < input_data->num_line; i++) {
            strings_array[i] = calloc(MAX_INPUT_STRING_SIZE + 1, sizeof(char));
            if (strings_array[i] == NULL) {
                free_strings(strings_array, input_data->num_line);
                return STR_COMP_ALLOC_ERROR;
            }
        }
    }
    str_comp_error_code_t err = read_strings(input_data, strings_array);
    if (!err) {
        input_data->sort_func(strings_array, input_data->num_line, input_data->comparator);
        err = sorting_error;
        if (!err) err = write_strings(input_data, strings_array);
    }
    free_strings(strings_array, input_data->num_line);
    return err;
}

/*-------------------------------------------------------------------------------------------------------------- MAIN */

int main(int argc, char *argv[]) {
    input_data_t input_data = {0};
    str_comp_error_code_t err = get_input_data(argc, argv, &input_data);
    if (!err) {
        err = string_comparer(&input_data);
    }
    return print_str_comp_error(err);
}
