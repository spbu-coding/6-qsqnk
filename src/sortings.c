#include <stdlib.h>
#include "sortings.h"

#define CHAR_VALUES_COUNT 256

void swap_strings(char** string_1, char** string_2) {
    char* temp = *string_1;
    *string_1 = *string_2;
    *string_2 = temp;
}

/************************** Bubble sort **************************/

void bubble(strings_array_t strings_array, array_size_t count_of_strings, comparator_func_t comparator) {
    for (size_t i = 0; i < count_of_strings; i++) {
        for (size_t j = 0; j < count_of_strings - i - 1; j++) {
            if (comparator(strings_array[j], strings_array[j + 1]) > 0) {
                swap_strings(&strings_array[j], &strings_array[j + 1]);
            }
        }
    }
}

/************************** Insertion sort **************************/

void insertion(strings_array_t strings_array, array_size_t count_of_strings, comparator_func_t comparator) {
    size_t j = 0;
    for (size_t i = 0; i < count_of_strings - 1; i++) {
        if (comparator(strings_array[i], strings_array[i + 1]) > 0) {
            j = i + 1;
            while (j > 0 && comparator(strings_array[j - 1], strings_array[j]) > 0) {
                swap_strings(&strings_array[j - 1], &strings_array[j]);
                j--;
            }
        }
    }
}

/************************** Merge sort functions **************************/

void merge_parts(strings_array_t strings_array, strings_array_t buffer,
                  size_t left, size_t mid, size_t right, comparator_func_t comparator) {
    size_t i = left;
    size_t j = mid;
    size_t k = left;
    while (i < mid && j < right) {
        if (comparator(strings_array[i], strings_array[j]) <= 0) {
            buffer[k++] = strings_array[i++];
        } else {
            buffer[k++] = strings_array[j++];
        }
    }
    while (i < mid) {
        buffer[k++] = strings_array[i++];
    }
    while (j < right) {
        buffer[k++] = strings_array[j++];
    }
    for (k = left; k < right; k++) {
        strings_array[k] = buffer[k];
    }
}

void recursive_merge(strings_array_t strings_array, strings_array_t  buffer,
                     size_t left, size_t right, comparator_func_t comparator) {
    if (right - left <= 1) {
        return;
    }
    size_t mid = (left + right) / 2;
    recursive_merge(strings_array, buffer, left, mid, comparator);
    recursive_merge(strings_array, buffer, mid, right, comparator);
    merge_parts(strings_array, buffer, left, mid, right, comparator);
}

void merge(strings_array_t strings_array, array_size_t count_of_strings, comparator_func_t comparator) {
    strings_array_t buffer = (strings_array_t)malloc(count_of_strings * sizeof(char*));
    recursive_merge(strings_array, buffer, 0, count_of_strings, comparator);
}

/************************** Quick sort functions **************************/

void recursive_quick(strings_array_t strings_array, size_t left, size_t right, comparator_func_t comparator) {
    if (left >= right) {
        return;
    }
    size_t mid = left;
    for (size_t i = left + 1; i < right; i++) {
        if (comparator(strings_array[i], strings_array[left]) <= 0) {
            mid++;
            swap_strings(&strings_array[mid], &strings_array[i]);
        }
    }
    swap_strings(&strings_array[left], &strings_array[mid]);
    recursive_quick(strings_array, left, mid, comparator);
    recursive_quick(strings_array, mid + 1, right, comparator);
}

void quick(strings_array_t strings_array, array_size_t count_of_strings, comparator_func_t comparator) {
    recursive_quick(strings_array, 0, count_of_strings, comparator);
}

/************* Radix sort functions *************/



void radix(strings_array_t strings_array, array_size_t count_of_strings, comparator_func_t comparator) {
    int order = (comparator("a", "b") < 0);
    unsigned int length = 0;
    long int max_length = 0;
    long int temp = 0;
    long int count_less = 0;
    for (long int i = 0; i < count_of_strings; i++) {
        length = strlen(strings_array[i]);
        if (max_length < length)
            max_length = length;
    }
    if (max_length == 0) {
        return;
    }
    unsigned char counting_array[CHAR_VALUES_COUNT];
    strings_array_t buffer = (strings_array_t)malloc(count_of_strings * sizeof(char*));
    for (long int column = max_length - 1; column >= 0; column--) {
        for (long int i = 0; i < CHAR_VALUES_COUNT; i++) {
            counting_array[i] = 0;
        }
        //counting chars starting from the end of strings
        for (long int line = 0; line < count_of_strings; line++) {
            counting_array[(unsigned char)(strings_array[line][column])]++;
        }
        count_less = 0;
        /* count_less stores how many strings in sorted array by the column char
         are before strings such that strings_array[line][column] = i */
        for (long int i = 0; i < CHAR_VALUES_COUNT; i++) {
            temp = counting_array[i];
            counting_array[i] = count_less;
            count_less += temp;
        }
        for (long int line = 0; line < count_of_strings; line++) {
            buffer[counting_array[(unsigned char)strings_array[line][column]]] = strings_array[line];
            counting_array[(unsigned char)strings_array[line][column]]++;
        }
        if (order > 0) {
            for (long int line = 0; line < count_of_strings; line++) {
                strings_array[line] = buffer[line];
            }
        } else {
            for (long int line = 0; line < count_of_strings; line++) {
                strings_array[line] = buffer[count_of_strings - line - 1];
            }
        }
    }
    free(buffer);
}