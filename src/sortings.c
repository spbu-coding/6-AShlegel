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
    for (array_size_t gap = 2; gap < array_size * 2; gap *= 2) {
        for (array_size_t offset = 0; offset < array_size; offset += gap) {
            array_size_t begin = offset,
                    middle = ((offset + (offset + gap) / 2) > array_size) ? array_size : (offset + (offset + gap) / 2),
                    end = ((offset + gap) > array_size) ? array_size : offset + gap;

            strings_array_t tmp_array = (char **) malloc((middle - begin) * sizeof(char));
            if (tmp_array == NULL) {
                sorting_error = STR_COMP_ALLOC_ERROR;
                return;
            }
            for (array_size_t i = begin; i < middle; i++) {
                tmp_array[i - begin] = strings_array[i];
            }
            array_size_t step = middle;
            while (step < end && begin < middle) {
                if (compare(tmp_array[begin - offset], strings_array[step]) > 0) {
                    strings_array[step + begin - middle] = strings_array[step];
                    step++;
                } else {
                    strings_array[step + begin - middle] = tmp_array[begin - offset];
                    begin++;
                }
            }
            for (; begin < middle; begin++) {
                strings_array[step + begin - middle] = tmp_array[begin - offset];
            }
            free(tmp_array);
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
    if (buffer == NULL){
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
