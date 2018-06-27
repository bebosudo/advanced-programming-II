#include "00_exercises.h"
#include <cmath>

struct BadInputException {};

void sum_matrices(double *m1, double *m2, double *ret, unsigned block_sq, std::string op = "+") {
    if (op == "+")
        for (size_t i = 0; i < block_sq; ++i)
            ret[i] = m1[i] + m2[i];
    else
        for (size_t i = 0; i < block_sq; ++i)
            ret[i] = m1[i] - m2[i];
}

inline std::unique_ptr<double[]> pack_matrices(double *C_11,
                                               double *C_12,
                                               double *C_21,
                                               double *C_22,
                                               unsigned block_side) {
    //
    std::unique_ptr<double[]> C_out{new double[block_side * block_side * 4]};

    size_t i = 0, half_C_mat = block_side * block_side * 2;
    // Copy the upper blocks first.
    for (; i < block_side; ++i) {
        std::copy(C_11 + block_side * i, C_11 + block_side * (i + 1),
                  C_out.get() + block_side * i * 2);
        std::copy(C_12 + block_side * i, C_12 + block_side * (i + 1),
                  C_out.get() + block_side * i * 2 + block_side);
    }

    for (i = 0; i < block_side; ++i) {
        std::copy(C_21 + block_side * i, C_21 + block_side * (i + 1),
                  C_out.get() + block_side * i * 2 + half_C_mat);
        std::copy(C_22 + block_side * i, C_22 + block_side * (i + 1),
                  C_out.get() + block_side * i * 2 + half_C_mat + block_side);
    }

    return std::move(C_out);
}

std::unique_ptr<double[]> strassen(unsigned N) {
    if (roundf(log2(N)) != log2(N)) {
        fprintf(stderr, "Enter a multiple of 2 as matrix side. Entered %d\n", N);
        throw BadInputException{};
    }

    unsigned block_side = N * .5, block_sq = N * N * .25;

    double *A_11{new double[block_sq]}, *A_12{new double[block_sq]}, *A_21{new double[block_sq]},
        *A_22{new double[block_sq]};

    double *B_11{new double[block_sq]}, *B_12{new double[block_sq]}, *B_21{new double[block_sq]},
        *B_22{new double[block_sq]};

    double *C_11{new double[block_sq]}, *C_12{new double[block_sq]}, *C_21{new double[block_sq]},
        *C_22{new double[block_sq]};

    double *M_1{new double[block_sq]}, *M_2{new double[block_sq]}, *M_3{new double[block_sq]},
        *M_4{new double[block_sq]}, *M_5{new double[block_sq]}, *M_6{new double[block_sq]},
        *M_7{new double[block_sq]};

    // A and B matrices initializations.
    std::fill(A_11, A_11 + block_sq, 1);
    std::fill(A_12, A_12 + block_sq, 1);
    std::fill(A_21, A_21 + block_sq, 1);
    std::fill(A_22, A_22 + block_sq, 1);
    std::fill(B_11, B_11 + block_sq, 1);
    std::fill(B_12, B_12 + block_sq, 1);
    std::fill(B_21, B_21 + block_sq, 1);
    std::fill(B_22, B_22 + block_sq, 1);

    double *temp1{new double[block_sq]}, *temp2{new double[block_sq]};

    // M1 = (A_11 + A_22) (B_11 + B_22)
    sum_matrices(A_11, A_22, temp1, block_sq);
    sum_matrices(B_11, B_22, temp2, block_sq);
    naive_matmul(temp1, temp2, M_1, block_side);

    // M6 = (A_21 - A_11) (B_11 + B_12)
    sum_matrices(A_21, A_11, temp1, block_sq, "-");
    sum_matrices(B_11, B_12, temp2, block_sq);
    naive_matmul(temp1, temp2, M_6, block_side);

    // M7 = (A_12 - A_22) (B_21 + B_22)
    sum_matrices(A_12, A_22, temp1, block_sq, "-");
    sum_matrices(B_21, B_22, temp2, block_sq);
    naive_matmul(temp1, temp2, M_7, block_side);

    // M2 = (A_21 + A_22) B_11
    sum_matrices(A_21, A_22, temp1, block_sq);
    naive_matmul(temp1, B_11, M_2, block_side);

    // M3 = A_11 (B_12 - B_22)
    sum_matrices(B_12, B_22, temp1, block_sq, "-");
    naive_matmul(A_11, temp1, M_3, block_side);

    // M4 = A_22 (B_21 - B_11)
    sum_matrices(B_21, B_11, temp1, block_sq, "*");
    naive_matmul(A_22, temp1, M_4, block_side);

    // M4 = (A_11 + A_12) B_22
    sum_matrices(A_11, A_12, temp1, block_sq, "-");
    naive_matmul(temp1, B_22, M_5, block_side);

    // Build the 4 blocks of the C matrix.
    // C_12 = M_3 + M_5
    sum_matrices(M_3, M_5, C_12, block_sq);

    // C_21 = M_2 + M_4
    sum_matrices(M_2, M_4, C_21, block_sq);

    // C_11 = M_1 + M_4 - M_5 + M_7
    sum_matrices(M_1, M_4, temp1, block_sq);
    sum_matrices(M_7, M_5, temp2, block_sq, "-");
    sum_matrices(temp1, temp2, C_11, block_sq);

    // C_22 = M_1 + M_4 - M_5 + M_7
    sum_matrices(M_1, M_2, temp1, block_sq, "-");
    sum_matrices(M_3, M_6, temp2, block_sq);
    sum_matrices(temp1, temp2, C_22, block_sq);

    std::unique_ptr<double[]> C{pack_matrices(C_11, C_12, C_21, C_22, block_side)};

    delete[] A_11;
    delete[] A_12;
    delete[] A_21;
    delete[] A_22;
    delete[] B_11;
    delete[] B_12;
    delete[] B_21;
    delete[] B_22;
    delete[] C_11;
    delete[] C_12;
    delete[] C_21;
    delete[] C_22;
    delete[] M_1;
    delete[] M_2;
    delete[] M_3;
    delete[] M_4;
    delete[] M_5;
    delete[] M_6;
    delete[] M_7;
    delete[] temp1;
    delete[] temp2;
    return C;
}
