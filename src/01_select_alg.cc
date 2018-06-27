#include "00_exercises.h"

// Pseudocode
/*
select(A, j, begin=1, end=|A|)

  # reorder the array
  if end-begin+1 < 140                               # O(1)
    insertion_sort(A, begin, end)                    # O(|A|)
    return A[j]

  # find the median of medians
  pivot = select_pivot(A, begin, end)                # O(|A|/5) + T(|A|/5)

  # tri-partition the array
  (k_1, k_2) = tri_partition(A, begin, end, pivot)   # O(|A|)

  # recursively call select in the correct interval
  if j < k_1
    return select(A, j, begin, k_1 - 1)

  if j > k_2
    return select(A, j, k_2 + 1, end)

  return A[j]

select_pivot(A, begin, end)
  num_of_blocks <- (end-begin+1)/5          # O(1)

  # initialize medians array
  medians <- array[num_of_blocks]           # O(|A|)

  # compute the medians for each block
  for i in [0,num_of_blocks]                # O(|A|/5) = O(|A|)

    # beginning point for each block
    cbegin = begin + 5 * i                  # O(1)

    # reorder the block
    insertion_sort(A, cbegin, cbegin+4)     # O(1)

    # add the i-th median to the array
    medians[i+1) <- A[cbegin+2]             # O(1)

  end-for

  # find the median of medians              # T(|A|/5)
  return select(medians, 1, num_of_blocks, (1+num_of_blocks)/2))
*/

// int select_pivot(int *A, int begin, int end) {
//     int num_of_blocks = (end - begin + 1) / 5;  // O(1)

//     // initialize medians array
//     int medians = array[num_of_blocks];  // O(|A|)

//     // compute the medians for each block
//     for (i in[ 0, num_of_blocks ]) {  // O(|A|/5) = O(|A|)

//         // beginning point for each block
//         int cbegin = begin + 5 * i;  // O(1)

//         // reorder the block
//         insertion_sort(A, cbegin, cbegin + 4);  // O(1)

//         // add the i - th median to the array
//         medians[i+1) = A[cbegin+2];             // O(1)
//     }

//     // find the median of medians #T(| A | / 5)
//   return select(medians, 1, num_of_blocks, (1+num_of_blocks)/2));
// }

// int select_element(int *A, int A_size, int j, int begin, int end, int sort_less_than) {
//     if (end - begin + 1 < sort_less_than) {
//         insertion_sort(A, A_size);
//         return A[j];
//     }

//     pivot = select_pivot(A, begin, end);
// }
