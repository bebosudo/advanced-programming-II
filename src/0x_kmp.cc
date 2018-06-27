#include "00_exercises.h"

void precompute_pattern(char *pattern_array, int *table_of_substrings, size_t pattern_length) {
    size_t pos = 1, cnd = 0;
    table_of_substrings[0] = 0;

    for (; pos < pattern_length; ++pos) {
        while (cnd > 0 and pattern_array[cnd] != pattern_array[pos])
            cnd = table_of_substrings[cnd - 1];

        if (pattern_array[cnd] == pattern_array[pos])
            ++cnd;

        table_of_substrings[pos] = cnd;
    }
}

void kmp(char *text_array,
         char *pattern_array,
         size_t text_length,
         size_t pattern_length,
         size_t *occurrences) {
    //
    int table_of_substrings[pattern_length];
    size_t num_occurr = 0;
    precompute_pattern(pattern_array, table_of_substrings, pattern_length);
    size_t q = 0;
    for (size_t i = 1; i < text_length; ++i) {
        while (q > 0 and pattern_array[q] != text_array[i - 1])
            q = table_of_substrings[q - 1];

        if (pattern_array[q] == text_array[i - 1])
            ++q;

        if (q == pattern_length) {
            occurrences[num_occurr] = i - pattern_length;
            q = table_of_substrings[q - 1];
            ++num_occurr;
        }
    }
}
