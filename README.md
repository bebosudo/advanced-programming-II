# Advanced Programming and Algorithmic Design - module II
Second module of the Advanced Programming and Algorithmic Design course, held @ uniTS by prof. A. Casagrande (http://www.dmi.units.it/~casagran/).

## TOC: algorithms implemented

1. Insertion sort and selection (quickselect) algorithms;
2. Matrix multiplication: naive and Strassen algorithms;
3. Search on graphs: Depth-First-Search and Breadth-First-Search;
4. Tarjan's algorithm for finding Strongly Connected Components in a graph;
5. Dijkstra's and A* algorithms for finding the shortest path between 2 nodes of a graph (without and with an heuristic measure available, respectively);
6. Floyd-Warshall algorithm for finding the shortest paths between all the nodes of a graph.
7. Knuth-Morris-Pratt's algorithm for finding occurrences of a word/pattern within a text.


## Setup

To setup and run the tests provided, clone this repository somewhere, then enable submodules, which is just [`doctest`](https://github.com/onqtam/doctest), a small package used to do TDD:

    $ git submodule init
    $ git submodule update

Now run the tests with a simple:

    $ make

9 tests and 985 assertions are provided as of June 28, 2018 (results of Tarjan's algorithm are only printed and not tested, due to the difficulty of creating a dinamic structure able to host IDs of nodes part of a SCC).


## Instructions

The code contained in this repo has been written under the constraint `Homework's: implement what we see in C++ (do not cheat! Do not use STL or BOOST or any high level library)`; this means we were not allowed to use ready-made data structures such as vectors, lists, set of hashmaps/dictionaries, and therefore had to implement those with linked-lists of pointers.

Disclaimer: The code is provided as-is, without any guarantee of being usable in production: refer to the [`LICENSE`](./LICENSE) file for detailed instructions.
