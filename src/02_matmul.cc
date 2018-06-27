#include "00_exercises.h"

// #define PPRINT

struct BadInputException {};

void pprint(double *mat, int side) {
    for (int i = 0; i < side; ++i) {
        for (int j = 0; j < side; ++j) {
            printf("%*d ", 4, (int)mat[i * side + j]);
        }
        printf("\n");
    }
}

// The best way to avoid handling raw pointers would have been to create a (templated) class
// SmartMatrix, containing a smart pointer to the data, and then redefine operator+, +=, *, etc.
// Then, computing e.g. A + B would have been resulted in A.operator+(B), and no objects are copied
// around, there would have not been the need of manually deallocating things, etc.
void naive_matmul(double *A, double *B, double *C, unsigned N) {
    //
    unsigned N_sq = N * N;

    unsigned i, j, k, part;

    // Matrices initializations.
    for (i = 0; i < N_sq; i++) {
        A[i] = 1;  // A[i] = i;
        B[i] = 1;  // B[i] = i;
        C[i] = 0;
    }

#ifdef PPRINT
    printf("\nA\n");
    pprint(A, N);
    printf("\nB\n");
    pprint(B, N);
    printf("\nC\n");
    pprint(C, N);
#endif

    // ------------------------------- matmul -------------------------------
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++) {
            part = 0;
            for (k = 0; k < N; ++k) {
                part += (A[i * N + k] * B[j + k * N]);
            }
            C[i * N + j] = part;
        }

#ifdef PPRINT
    printf("\nC\n");
    pprint(C, N);
#endif
}
