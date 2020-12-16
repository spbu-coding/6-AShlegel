#include <stdlib.h>
#include "sortings.h"
#include "str_comparer_err.h"

#define SWAP_STR(s1, s2) char *tmp = s1; s1 = s2; s2 = tmp;

/*----------------------------------------------------------------------------------------------------- SORT COMPARER */

int asc_comparator(const char *s1, const char *s2) {
    return strcmp(s1, s2);
}

int des_comparator(const char *s1, const char *s2) {
    return -strcmp(s1, s2);
}

/*------------------------------------------------------------------------------------------------------ BUBBLES SORT */

void bubble(strings_array_t strings_array, array_size_t array_size, comparator_func_t compare) {
    for (array_size_t i = array_size - 1; i > 0; i--) {
        for (array_size_t j = 0; j < i; j++) {
            if (compare(strings_array[j], strings_array[j + 1]) > 0) {
                SWAP_STR(strings_array[j], strings_array[j + 1])
            }
        }
    }
}

/*---------------------------------------------------------------------------------------------------- INSERTION SORT */

void insertion(strings_array_t strings_array, array_size_t array_size, comparator_func_t compare) {
    for (array_size_t i = 1; i < array_size; i++) {
        for (array_size_t j = i; j > 0 && compare(strings_array[j - 1], strings_array[j]) > 0; j--) {
            SWAP_STR(strings_array[j - 1], strings_array[j])
        }
    }
}

/*-------------------------------------------------------------------------------------------------------- MERGE SORT */

void merge(strings_array_t strings_array, array_size_t array_size, comparator_func_t compare) {
    for (unsigned int size = array_size, part_len = 1; size > 1; size = (1 + (size - 1) / 2), part_len *= 2) {
        for (unsigned int i = 0; i < size - 1; i += 2) {
            size_t len1 = part_len, len2 = (((i + 2) * part_len) <= array_size) ? part_len : (array_size - (i + 1) *
                    part_len), buf_len =
                    len1 + len2;
            unsigned int ind1 = i * part_len, ind2 = (i + 1) * part_len;
            char *buf[buf_len];
            while (len1 > 0 && len2 > 0) {
                if (compare(strings_array[ind1], strings_array[ind2]) <= 0) {
                    buf[buf_len - (len1-- + len2)] = strings_array[ind1++];
                } else {
                    buf[buf_len - (len1 + len2--)] = strings_array[ind2++];
                }
            }
            if (len1 > 0) {
                memcpy(&buf[buf_len - len1], &strings_array[ind1], len1 * sizeof(char *));
            } else {
                memcpy(&buf[buf_len - len2], &strings_array[ind2], len2 * sizeof(char *));
            }
            memcpy(&strings_array[i * part_len], buf, buf_len * sizeof(char *));
        }
    }
}

/*-------------------------------------------------------------------------------------------------------- QUICK SORT */

void quick(strings_array_t strings_array, array_size_t array_size, comparator_func_t compare) {
    if (array_size <= 1) {
        return;
    }
    array_size_t begin = 0,
            end = array_size - 1,
            pivot = (begin + end) / 2;
    while (begin < end) {
        while (compare(strings_array[begin], strings_array[pivot]) < 0) {
            begin++;
        }
        while (compare(strings_array[end], strings_array[pivot]) > 0) {
            end--;
        }
        if (begin < end) {
            SWAP_STR(strings_array[begin], strings_array[end])
            end--;
            if (begin < end) {
                begin++;
            }
        }
    }
    quick(strings_array, end, compare);
    quick(strings_array + begin, array_size - begin - 1, compare);
}

/*-------------------------------------------------------------------------------------------------------- RADIX SORT */

#define ASCII_SIZE 128

void radix(strings_array_t strings_array, array_size_t array_size, comparator_func_t compare) {
    if (array_size <= 1) {
        return;
    }
    int max_len = 0;
    for (array_size_t i = 0; i < array_size; i++) {
        int len = (int) strlen(strings_array[i]);
        if (len > max_len) {
            max_len = len;
        }
    }
    strings_array_t buffer = malloc(array_size * sizeof *strings_array);
    if (buffer == NULL) {
        sorting_error = STR_COMP_ALLOC_ERROR;
        return;
    }

    for (int i = max_len - 1; i >= 0; i--) {
        array_size_t counter[ASCII_SIZE] = {0};
        for (array_size_t j = 0; j < array_size; j++) {
            counter[(array_size_t) strings_array[j][i]]++;
        }
        for (array_size_t j = 1; j < ASCII_SIZE; j++) {
            counter[j] += counter[j - 1];
        }
        for (int j = (int) array_size - 1; j >= 0; j--) {
            buffer[counter[(array_size_t) strings_array[j][i]] - 1] = strings_array[j];
            counter[(array_size_t) strings_array[j][i]]--;
        }
        for (int j = (int) array_size - 1; j >= 0; j--) {
            if (i > 0 || compare("a", "b") == -1) {
                strings_array[j] = buffer[j];
            } else {
                strings_array[array_size - j - 1] = buffer[j];
            }
        }
    }
    free(buffer);
}

#undef ASCII_SIZE
#undef SWAP_STR
