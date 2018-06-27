#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "00_exercises.h"
#include "doctest.h"

// In doctest, there are three kind of assertion macros: REQUIRE, CHECK and WARN.
// If a REQUIRE fails, it stops the whole test execution, if a CHECK fails, the tests continue to
// execute until the whole test suite is done, and then doctest graciously stops, reporting all the
// CHECKs that failed.

#ifdef DEBUG

TEST_CASE("--- 01 --- insertion-sort") {
    int A_in[] = {10, 123, 34, 809, 21, 35, 543, 345, 6, 2, 45, 23},
        A_out[] = {2, 6, 10, 21, 23, 34, 35, 45, 123, 345, 543, 809}, size = 12;

    insertion_sort(A_in, size);

    for (int i = 0; i < size; ++i)
        FAST_CHECK_EQ(A_in[i], A_out[i]);
}

// TEST_CASE("--- 01 --- select algorithm") {
//     int A[] = {10, 123, 34, 809, 21, 35, 543, 345, 6, 2, 45, 23}, A_size = 12, A_in = 7, A_out =
//     35,
//         B[] = {5, 2, 4, 6, 1, 3}, B_size = 6, B_in = 5, B_out = 5;

//     select_element(A, A_size, A_in, 0, A_size);
// }

TEST_CASE("--- 02 --- matmul: naive") {
    unsigned dim, dimensions[] = {4, 10, 20};

    for (int i = 0; i < 3; ++i) {
        dim = dimensions[i];
        double *m1{new double[dim * dim]}, *m2{new double[dim * dim]}, *res{new double[dim * dim]};
        naive_matmul(m1, m2, res, dim);

        for (size_t i = 0; i < dim * dim; ++i) {
            FAST_CHECK_EQ(res[i], dim);
        }

        delete[] m1;
        delete[] m2;
        delete[] res;
    }
}

TEST_CASE("--- 02 --- matmul: Strassen algorithm") {
    unsigned dim, dimensions[] = {4, 8, 16, 64};

    for (int i = 0; i < 3; ++i) {
        dim = dimensions[i];
        std::unique_ptr<double[]> result = strassen(dim);

        for (size_t i = 0; i < dim * dim; ++i) {
            FAST_CHECK_EQ(result[i], dim);
        }
    }
}

TEST_CASE("--- 03 --- searching on graphs (DFS & BFS)") {
    // The `node` class can be used to represent a directed graph.
    node<int> graph(2);
    graph.add_node(3);
    graph.add_node(4);
    graph.add_node(5);
    graph.add_node(10);

    graph.add_node(14);
    graph.add_node(17);

    graph.add_edge(2, 3);
    graph.add_edge(2, 4);
    graph.add_edge(2, 5);
    graph.add_edge(2, 10);
    graph.add_edge(3, 10);
    graph.add_edge(5, 10);
    graph.add_edge(10, 5);

    graph.add_edge(14, 17);

    // graph.print();

    int node_to_start_searching;

    SUBCASE("DFS: tree 1") {
        node_to_start_searching = 2;
        auto discovered_nodes_and_num_discovered = graph.DFS(node_to_start_searching);

        auto discovered_nodes = std::move(discovered_nodes_and_num_discovered.first);
        size_t actual_number_of_nodes_discovered = discovered_nodes_and_num_discovered.second;
        FAST_CHECK_EQ(actual_number_of_nodes_discovered, 5);

        int tree1[] = {2, 3, 10, 5, 4};
        for (size_t i = 0; i < actual_number_of_nodes_discovered; ++i)
            FAST_CHECK_EQ(discovered_nodes[i], tree1[i]);
    }

    SUBCASE("DFS: tree 2") {
        // If we start from node 14, we should discover only itself and node 17.
        node_to_start_searching = 14;
        auto discovered_nodes_and_num_discovered = graph.DFS(node_to_start_searching);

        auto discovered_nodes = std::move(discovered_nodes_and_num_discovered.first);
        size_t actual_number_of_nodes_discovered = discovered_nodes_and_num_discovered.second;
        FAST_CHECK_EQ(actual_number_of_nodes_discovered, 2);

        int tree2[] = {14, 17};
        for (size_t i = 0; i < actual_number_of_nodes_discovered; ++i)
            FAST_CHECK_EQ(discovered_nodes[i], tree2[i]);
    }

    SUBCASE("BFS: tree 1") {
        node_to_start_searching = 2;
        auto discovered_nodes_and_num_discovered = graph.BFS(node_to_start_searching);

        auto discovered_nodes = std::move(discovered_nodes_and_num_discovered.first);
        size_t actual_number_of_nodes_discovered = discovered_nodes_and_num_discovered.second;
        FAST_CHECK_EQ(actual_number_of_nodes_discovered, 5);

        int tree1[] = {2, 3, 4, 5, 10};
        for (size_t i = 0; i < actual_number_of_nodes_discovered; ++i)
            FAST_CHECK_EQ(discovered_nodes[i], tree1[i]);
    }

    SUBCASE("BFS: tree 2") {
        node_to_start_searching = 14;
        auto discovered_nodes_and_num_discovered = graph.BFS(node_to_start_searching);

        auto discovered_nodes = std::move(discovered_nodes_and_num_discovered.first);
        size_t actual_number_of_nodes_discovered = discovered_nodes_and_num_discovered.second;
        FAST_CHECK_EQ(actual_number_of_nodes_discovered, 2);

        int tree2[] = {14, 17};
        for (size_t i = 0; i < actual_number_of_nodes_discovered; ++i)
            FAST_CHECK_EQ(discovered_nodes[i], tree2[i]);
    }
}

TEST_CASE("--- 04 --- SCCs & topological sort") {
    SUBCASE("scc test 1") {
        // Test case from: https://www.programming-algorithms.net/image/id/44234
        std::cout << "scc test 1; the SCCs should be: {1}, {2}, {3, 4, 5, 6}, {7}, {8}\n";
        node<int> graph(1);
        graph.add_node(2);
        graph.add_node(3);
        graph.add_node(4);
        graph.add_node(5);
        graph.add_node(6);
        graph.add_node(7);
        graph.add_node(8);

        graph.add_edge(1, 2);
        graph.add_edge(2, 3);
        graph.add_edge(3, 4);
        graph.add_edge(4, 5);
        graph.add_edge(5, 6);
        graph.add_edge(6, 3);
        graph.add_edge(1, 7);
        graph.add_edge(7, 2);
        graph.add_edge(7, 8);

        graph.tarjan();
    }

    SUBCASE("scc test 2") {
        // Test case from: https://youtu.be/TyWtx7q2D7Y?t=11m
        // The nodes have been intentionally shuffled.
        std::cout << "\nscc test 2; the SCCs should be: {0, 1, 2}, {4, 5, 6}, {3, 7}\n";
        node<int> graph(5);
        graph.add_node(2);
        graph.add_node(7);
        graph.add_node(3);
        graph.add_node(1);
        graph.add_node(0);
        graph.add_node(6);
        graph.add_node(4);

        graph.add_edge(3, 4);
        graph.add_edge(3, 7);
        graph.add_edge(7, 3);
        graph.add_edge(7, 5);
        graph.add_edge(4, 5);
        graph.add_edge(5, 6);
        graph.add_edge(5, 0);
        graph.add_edge(6, 4);
        graph.add_edge(6, 0);
        graph.add_edge(6, 2);
        graph.add_edge(2, 0);
        graph.add_edge(0, 1);
        graph.add_edge(1, 2);
        graph.add_node(4);

        graph.tarjan();
    }
}

TEST_CASE("--- 05 --- Dijkstra's and A* algorithms") {
    // The algorithm implementation assumes that the nodes are a sequence starting from 0 (an
    // hashmap mapping values to indices would be needed otherwise).
    node<size_t> graph(5);
    graph.add_node(4);
    graph.add_node(1);
    graph.add_node(3);
    graph.add_node(2);
    graph.add_node(0);
    graph.add_node(6);

    //        (from, to, weight)
    graph.add_edge(1, 0, 4);
    graph.add_edge(1, 2, 7);
    graph.add_edge(1, 3, 9);
    graph.add_edge(1, 6, 14);
    graph.add_edge(2, 3, 10);
    graph.add_edge(2, 4, 15);
    graph.add_edge(3, 4, 11);
    graph.add_edge(3, 6, 2);
    graph.add_edge(4, 5, 6);
    graph.add_edge(6, 5, 9);

    // Make the graph an undirected graph.
    graph.add_edge(0, 1, 4);
    graph.add_edge(2, 1, 7);
    graph.add_edge(3, 1, 9);
    graph.add_edge(6, 1, 14);
    graph.add_edge(3, 2, 10);
    graph.add_edge(4, 2, 15);
    graph.add_edge(4, 3, 11);
    graph.add_edge(6, 3, 2);
    graph.add_edge(5, 4, 6);
    graph.add_edge(5, 6, 9);

    const size_t infty = -1, num_nodes = 7;

    SUBCASE("dijkstra test 1") {
        // test case from: https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
        // with the addition of a node 0, with weight 4 on the unique edge going to node 1.
        size_t node_to_start_from = 1;
        auto pair_distances_previous_nodes = graph.dijkstra(node_to_start_from);
        auto distances_arr = std::move(pair_distances_previous_nodes.first);
        auto previous_arr = std::move(pair_distances_previous_nodes.second);

        // This implementation produces a shortest-path tree, so we can test the minimum distance
        // required to reach each node from the source node.
        size_t short_path_tree[] = {4, 0, 7, 9, 20, 20, 11},
               passing_by_node[] = {1, infty, 1, 1, 3, 6, 3};
        for (size_t i = 0; i < num_nodes; ++i) {
            FAST_CHECK_EQ(distances_arr[i], short_path_tree[i]);
            FAST_CHECK_EQ(previous_arr[i], passing_by_node[i]);
        }
    }

    SUBCASE("dijkstra test 2") {
        // should be the same as 1, but the short-path tree should have an addition of the distance
        // between 0 and 1 on all the members.
        size_t node_to_start_from = 0;
        auto pair_distances_previous_nodes = graph.dijkstra(node_to_start_from);
        auto distances_arr = std::move(pair_distances_previous_nodes.first);
        auto previous_arr = std::move(pair_distances_previous_nodes.second);

        size_t short_path_tree[] = {0, 4, 11, 13, 24, 24, 15},
               passing_by_node[] = {infty, 0, 1, 1, 3, 6, 3};
        for (size_t i = 0; i < num_nodes; ++i) {
            FAST_CHECK_EQ(distances_arr[i], short_path_tree[i]);
            FAST_CHECK_EQ(previous_arr[i], passing_by_node[i]);
        }
    }

    SUBCASE("dijkstra test 3") {
        size_t node_to_start_from = 4;
        auto pair_distances_previous_nodes = graph.dijkstra(node_to_start_from);
        auto distances_arr = std::move(pair_distances_previous_nodes.first);
        auto previous_arr = std::move(pair_distances_previous_nodes.second);

        size_t short_path_tree[] = {24, 20, 15, 11, 0, 6, 13},
               passing_by_node[] = {1, 3, 4, 4, infty, 4, 3};
        for (size_t i = 0; i < num_nodes; ++i) {
            FAST_CHECK_EQ(distances_arr[i], short_path_tree[i]);
            FAST_CHECK_EQ(previous_arr[i], passing_by_node[i]);
        }
    }
    // heuristic is: w/2 if w is even, w+4 otherwise (w = weight):
    // edge(1, 0)  -> weight: 4   -> heuristic: 2
    // edge(1, 2)  -> weight: 7   -> heuristic: 11
    // edge(1, 3)  -> weight: 9   -> heuristic: 13
    // edge(1, 6)  -> weight: 14  -> heuristic: 7
    // edge(2, 3)  -> weight: 10  -> heuristic: 5
    // edge(2, 4)  -> weight: 15  -> heuristic: 19
    // edge(3, 4)  -> weight: 11  -> heuristic: 15
    // edge(3, 6)  -> weight: 2   -> heuristic: 1
    // edge(4, 5)  -> weight: 6   -> heuristic: 3
    // edge(6, 5)  -> weight: 9   -> heuristic: 13
    SUBCASE("A* test 1") {
        size_t node_to_start_from = 1;
        auto pair_distances_previous_nodes = graph.a_star(node_to_start_from);
        auto distances_arr = std::move(pair_distances_previous_nodes.first);
        auto previous_arr = std::move(pair_distances_previous_nodes.second);

        size_t short_path_tree[] = {6, 0, 18, 22, 48, 43, 21},
               passing_by_node[] = {1, infty, 1, 1, 3, 6, 1};
        for (size_t i = 0; i < num_nodes; ++i) {
            FAST_CHECK_EQ(distances_arr[i], short_path_tree[i]);
            FAST_CHECK_EQ(previous_arr[i], passing_by_node[i]);
        }
    }

    SUBCASE("A* test 2") {
        // should be the same as 1, but the short-path tree should have an addition of the distance
        // between 0 and 1 on all the members.
        size_t node_to_start_from = 0;
        auto pair_distances_previous_nodes = graph.a_star(node_to_start_from);
        auto distances_arr = std::move(pair_distances_previous_nodes.first);
        auto previous_arr = std::move(pair_distances_previous_nodes.second);

        size_t short_path_tree[] = {0, 6, 24, 28, 54, 49, 27},
               passing_by_node[] = {infty, 0, 1, 1, 3, 6, 1};
        for (size_t i = 0; i < num_nodes; ++i) {
            FAST_CHECK_EQ(distances_arr[i], short_path_tree[i]);
            FAST_CHECK_EQ(previous_arr[i], passing_by_node[i]);
        }
    }
}

TEST_CASE("--- 06 --- All-pairs shortest-path") {
    // Test case from:
    // http://www.programming-algorithms.net/article/45708/Floyd-Warshall-algorithm
    const size_t side = 5;
    const double inf = std::numeric_limits<double>::infinity();
    std::unique_ptr<double[]> distances{new double[side * side]{
        0,   5,   inf, 2,   inf,  //
        inf, 0,   2,   inf, inf,  //
        3,   inf, 0,   inf, 7,    //
        inf, inf, 4,   0,   1,    //
        1,   3,   inf, inf, 0     //
    }};

    std::unique_ptr<double[]> expected_distances{new double[side * side]{
        0, 5, 6, 2, 3,  //
        5, 0, 2, 7, 8,  //
        3, 8, 0, 5, 6,  //
        2, 4, 4, 0, 1,  //
        1, 3, 5, 3, 0   //
    }};

    auto distances_and_paths = floyd_warshall(std::move(distances), side);
    distances = std::move(distances_and_paths.first);
    auto paths = std::move(distances_and_paths.second);

    for (size_t i = 0; i < side * side; ++i)
        FAST_CHECK_EQ(distances[i], expected_distances[i]);
}

TEST_CASE("--- 0x --- Knuth Morris Pratt") {
    char A_text[] = {'a', 'b', 'c', 'x', 'a', 'b', 'c', 'd', 'a', 'b', 'x', 'a',
                     'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'y'},
         A_pattern[] = {'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd'};
    size_t A_text_len = 23, A_pattern_len = 8, A_occurrences_found[A_text_len] = {0},
           A_occurrences[] = {11};

    SUBCASE("single occurrence") {
        kmp(A_text, A_pattern, A_text_len, A_pattern_len, A_occurrences_found);

        for (size_t i = 0; i < A_text_len; ++i) {
            if (A_occurrences_found[i] == 0)
                break;

            FAST_CHECK_EQ(A_occurrences_found[i], A_occurrences[i]);
        }
    }

    SUBCASE("multiple occurrences") {
        char B_pattern[] = {'b', 'c'};
        size_t B_pattern_len = 2, B_occurrences_found[A_text_len] = {0},
               B_occurrences[] = {1, 5, 12, 16, 20};

        kmp(A_text, B_pattern, A_text_len, B_pattern_len, B_occurrences_found);

        for (size_t i = 0; i < A_text_len; ++i) {
            if (B_occurrences_found[i] == 0)
                break;

            FAST_CHECK_EQ(B_occurrences_found[i], B_occurrences[i]);
        }
    }
}

#endif
