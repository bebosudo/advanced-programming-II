#include "00_exercises.h"

std::pair<std::unique_ptr<double[]>, std::unique_ptr<double[]>> floyd_warshall(
    std::unique_ptr<double[]> dist,
    const size_t &side) {
    //
    std::unique_ptr<double[]> paths{new double[side * side]};

    for (size_t k = 0; k < side; ++k)
        for (size_t i = 0; i < side; ++i)
            for (size_t j = 0; j < side; ++j)
                if (dist[i * side + j] > dist[i * side + k] + dist[k * side + j]) {
                    dist[i * side + j] = dist[i * side + k] + dist[k * side + j];
                    paths[i * side + j] = paths[k * side + j];
                }
    return std::make_pair(std::move(dist), std::move(paths));
}
