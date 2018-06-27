#include "00_exercises.h"

// https://rcoh.me/posts/linear-time-median-finding/

size_t median_pivot(int *input_arr, size_t from, size_t to) {
    // Consider input_arr as a 2D matrix with (to-from)//5 rows and 5 columns (some elements may be
    // in end in a further row and thus get ignored).
    size_t mat_width = 5, mat_height = (size_t)floor((to - from) / 5);

    std::unique_ptr<int[]> temp{new int[mat_width]};

    // Order each chunks (made of 5 pieces) using insertion sort.
    for (size_t row = 0; row < mat_height; ++row) {
        std::copy(input_arr + (from + row * mat_width),              //
                  input_arr + (from + row * mat_width + mat_width),  //
                  temp.get());                                       //

        insertion_sort(temp.get(), mat_width);

        std::copy(temp.get(),                             //
                  temp.get() + mat_width,                 //
                  input_arr + (from + row * mat_width));  //
    }

    std::unique_ptr<int[]> medians{new int[mat_height]};

    // Extract the central element from the sorted chunks.
    for (size_t row = 0; row < mat_height; ++row)
        medians[row] = input_arr[from + row * mat_width + 2];

    // Use the quickselect to find the median of the medians.
    return select_alg(medians.get(), mat_height, (size_t)floor(mat_height / 2));
}

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

size_t select_alg_recur(int *input_arr, size_t from, size_t to, size_t index) {
    if (to - from + 1 < 5) {
        size_t size = to - from + 1;
        std::unique_ptr<int[]> temp{new int[size]};

        for (size_t i = 0; i < size; ++i)
            temp[i] = input_arr[i + from];

        insertion_sort(temp.get(), size);
        return temp[index - from];
    }

    // The median-of-medians selector doesn't seem to work yet.
    // size_t pivot = from + median_pivot(input_arr, from, to);
    size_t pivot = from + floor(rand() % (to - from + 1));

    pivot = tri_partition(input_arr, from, to, pivot);

    if (index == pivot or from == to)
        return input_arr[index];
    else if (index < pivot)
        return select_alg_recur(input_arr, from, pivot - 1, index);
    else
        return select_alg_recur(input_arr, pivot + 1, to, index);
}

// Just a wrapper to make sure that the behaviour is the same for arrays of even or odd length.
size_t select_alg(int *input_arr, size_t length, size_t index) {
    return select_alg_recur(input_arr, 0, length, length % 2 == 1 ? index : index + 1);
}
