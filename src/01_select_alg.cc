#include "00_exercises.h"

// https://rcoh.me/posts/linear-time-median-finding/

size_t tri_partition(int *input_arr, size_t from, size_t to, size_t index) {
    int pivot_val = input_arr[index];

    std::swap(input_arr[index], input_arr[to]);
    size_t pivot_idx = from;

    for (size_t i = from; i < to; i++)
        if (input_arr[i] < pivot_val) {
            std::swap(input_arr[pivot_idx], input_arr[i]);
            pivot_idx++;
        }

    std::swap(input_arr[to], input_arr[pivot_idx]);
    return pivot_idx;
}

size_t select_alg_recur(int *input_arr, size_t from, size_t to, size_t index, size_t strategy) {
    if (from == to)
        return input_arr[index];

    if (strategy == 0) {
        ;
    }
    size_t pivot = from + floor(rand() % (to - from + 1));

    pivot = tri_partition(input_arr, from, to, pivot);

    if (index == pivot)
        return input_arr[index];
    else if (index < pivot)
        return select_alg_recur(input_arr, from, pivot - 1, index, strategy);
    else
        return select_alg_recur(input_arr, pivot + 1, to, index, strategy);
}

// Just a wrapper to makes sure that the behaviour is the same for arrays of even or odd length.
size_t select_alg(int *input_arr, size_t length, size_t index, size_t strategy) {
    return select_alg_recur(input_arr, 0, length, length % 2 == 1 ? index : index + 1, strategy);
}
