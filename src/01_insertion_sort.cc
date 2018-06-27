#include "00_exercises.h"

void insertion_sort(int *A, int size) {
    int key, i;
    for (int j = 1; j < size; ++j) {
        key = A[j];
        i = j - 1;

        while (i >= 0 and A[i] > key) {
            A[i + 1] = A[i];
            i--;
        }

        A[i + 1] = key;
    }
}
