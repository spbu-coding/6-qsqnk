#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sortings.h"

#define error(...) (fprintf(stderr, __VA_ARGS__))
#define NORMAL_COUNT_OF_PARAMETERS 5
#define COUNT_OF_STRINGS_INDEX 1
#define INPUT_FILENAME_INDEX 2
#define OUTPUT_FILENAME_INDEX 3
#define SORT_NAME_INDEX 4
#define COMPARATOR_NAME_INDEX 5

typedef void (*sort_func_t)(strings_array_t, array_size_t, comparator_func_t);

int compare_strings_asc(const char* string_1, const char* string_2) {
    return strcmp(string_1, string_2);
}

int compare_strings_des(const char* string_1, const char* string_2) {
    return -strcmp(string_1, string_2);
}

comparator_func_t get_comparator_func(const char* comparator_name) {
    if (strcmp(comparator_name, "asc") == 0) {
        return compare_strings_asc;
    }
    else if (strcmp(comparator_name, "des") == 0) {
        return compare_strings_des;
    }
    else {
        error("Incorrect comparator name\n");
        return NULL;
    }
}

sort_func_t get_sort_func(const char* sort_name) {
    if (strcmp(sort_name, "bubble") == 0) {
        return bubble;
    }
    else if (strcmp(sort_name, "insertion") == 0) {
        return insertion;
    }
    else if (strcmp(sort_name, "merge") == 0) {
        return merge;
    }
    else if (strcmp(sort_name, "quick") == 0) {
        return quick;
    }
    else if (strcmp(sort_name, "radix") == 0) {
        return radix;
    }
    else {
        error("Incorrect sort name\n");
        return NULL;
    }
}

long int get_count_of_strings(char* argument) {
    array_size_t count = strtol(argument, NULL, 10);
    if ((long int) count < 0) {
        error("Count of strings must be non-negative\n");
        return -1;
    }
    if (count == 0 && argument[0] != '0') {
        error("You entered not a number\n");
        return -1;
    }
    return count;
}

void free_strings_array(strings_array_t strings_array, array_size_t count_of_strings) {
    for (size_t i = 0; i < count_of_strings; i++) {
        free(strings_array[i]);
    }
    free(strings_array);
}

strings_array_t create_strings_array(array_size_t count_of_strings) {
    strings_array_t  strings_array = (char**)malloc(sizeof(char*) * count_of_strings);
    if (strings_array == NULL) {
        error("Can not allocate memory for strings array\n");
        return NULL;
    }
    for (size_t i = 0; i < count_of_strings; i++) {
        strings_array[i] = (char*)malloc(sizeof(char) * (MAX_INPUT_STRING_SIZE + 1));
        if (strings_array[i] == NULL) {
            error("Can not allocate memory for char array\n");
            free_strings_array(strings_array, count_of_strings);
            return NULL;
        }
    }
    return strings_array;
}

int read_file(FILE* input_file, strings_array_t strings_array, array_size_t count_of_strings) {
    if (input_file == NULL) {
        error("Can not read input file\n");
        return -1;
    }
    size_t index = 0;
    while (!feof(input_file) && index < count_of_strings) {
        if (fgets(strings_array[index], MAX_INPUT_STRING_SIZE, input_file) == NULL) {
            error("Can not read %d string\n", index);
            return -1;
        }
        index++;
    }
    if (feof(input_file) && index != count_of_strings) {
        error("Count of strings in file must be equal to first console parameter\n");
        return -1;
    }
    return 0;
}

int write_to_file(FILE* output_file, strings_array_t strings_array, array_size_t count_of_strings) {
    if (output_file == NULL) {
        error("Can not read input file\n");
        return -1;
    }
    if (count_of_strings == 0) {
        if (fputs("\n", output_file) == EOF) {
            error("Can not write to output file\n");
            return -1;
        }
    } else {
        for (size_t i = 0; i < count_of_strings; i++) {
            if (fputs(strings_array[i], output_file) == EOF) {
                error("Can not write to output file\n");
                return -1;
            }
            //check if string contains "\n" symbol
            if (strcspn(strings_array[i], "\n") == strlen(strings_array[i])) {
                if (fputs("\n", output_file) == EOF) {
                    error("Can not write to output file\n");
                    return -1;
                }
            }
        }
    }
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc - 1 != NORMAL_COUNT_OF_PARAMETERS) {
        error("Count of parameters must be equal to 5\n");
        return -1;
    }
    long int count_of_strings_us = get_count_of_strings(argv[COUNT_OF_STRINGS_INDEX]);
    if (count_of_strings_us == -1) {
        return -1;
    }
    size_t count_of_strings = count_of_strings_us;
    sort_func_t sort = get_sort_func(argv[SORT_NAME_INDEX]);
    if (sort == NULL) {
        return -1;
    }
    comparator_func_t comparator = get_comparator_func(argv[COMPARATOR_NAME_INDEX]);
    if (comparator == NULL) {
        return -1;
    }
    strings_array_t strings_array = create_strings_array(count_of_strings);
    if (strings_array == NULL) {
        return -1;
    }
    FILE* input_file = fopen(argv[INPUT_FILENAME_INDEX], "r");
    if (read_file(input_file, strings_array, count_of_strings)) {
        fclose(input_file);
        free_strings_array(strings_array, count_of_strings);
        return -1;
    }
    sort(strings_array, count_of_strings, comparator);
    FILE* output_file = fopen(argv[OUTPUT_FILENAME_INDEX], "w");
    if (write_to_file(output_file, strings_array, count_of_strings)) {
        fclose(output_file);
        free_strings_array(strings_array, count_of_strings);
        return -1;
    }
    free_strings_array(strings_array, count_of_strings);
    fclose(input_file);
    fclose(output_file);
}
