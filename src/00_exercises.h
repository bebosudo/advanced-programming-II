#ifndef __EXERCISES_H__
#define __EXERCISES_H__

#include <algorithm>
#include <iostream>
#include <iterator>  // need to derive from std::iterator
#include <memory>
#include <utility>
#include <random>
#include <cmath>

// Many algorithms are implemented as public methods of classes contained in the following header,
// e.g. DFS, BFS, dijkstra, A*.
#include "00_structures.h"

// 01
void insertion_sort(int *A, int size);
size_t select_alg(int *input_arr, size_t length, size_t index);

// 02: matrix multiplication
void naive_matmul(double *A, double *B, double *C, unsigned N);
std::unique_ptr<double[]> strassen(unsigned N);

// 06: Floyd-Warshall
std::pair<std::unique_ptr<double[]>, std::unique_ptr<double[]>> floyd_warshall(
    std::unique_ptr<double[]> dist,
    const size_t &side);

// 0x
void kmp(char *text_array,
         char *pattern_array,
         size_t text_length,
         size_t pattern_length,
         size_t *occurrences);

#endif
