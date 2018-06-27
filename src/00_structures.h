#ifndef __STRUCTURES_H__
#define __STRUCTURES_H__

struct OutOfBoundException {};
struct MissingValueException {};
struct PopFromEmptyQueue {};

const size_t DEFAULT_UNVISITED = -1;

// 03: graphs
template <typename V>
class queue {
    template <typename VV>
    class queue_node {
       public:
        VV _value;
        std::unique_ptr<queue_node> _next;

        queue_node<VV>(VV value) : _value{value} {};

        // Push on the head, append at the tail and pop from the head.
        void append(VV value) {
            if (_next) {
                _next->append(value);
                return;
            }
            _next = std::unique_ptr<queue_node<VV>>(new queue_node<VV>(value));
        }
        bool is_empty() { return not _next; }
        inline size_t num_elems() { return 1 + (bool)_next ? _next->num_elems() : 0; }

        void print() {
            std::cout << "'" << _value << "'";
            if (_next) {
                std::cout << ", ";
                _next->print();
            }
        }
        bool contains(VV value) {
            return value == _value or ((bool)_next and _next->contains(value));
        }

        void remove_el(VV value) {
            if (_next->_value == value)
                _next = std::move(_next->_next);
            else
                _next->remove_el(value);
        }
    };

    std::unique_ptr<queue_node<V>> _head;

   public:
    // Push on the head, append at the tail and pop from the head.
    void push(V value) {
        auto temp = std::unique_ptr<queue_node<V>>(new queue_node<V>(value));
        if (_head)
            temp->_next = std::move(_head);

        _head = std::move(temp);
    }
    void append(V value) {
        if (_head)
            _head->append(value);
        else
            _head = std::unique_ptr<queue_node<V>>(new queue_node<V>(value));
    }
    V pop() {
        if (not _head)
            throw PopFromEmptyQueue{};

        V temp = _head->_value;
        if (_head->_next) {
            _head->_value = _head->_next->_value;
            _head->_next = std::move(_head->_next->_next);
        } else
            _head.reset();

        return temp;
    }
    inline size_t num_elems() { return (bool)_head ? _head->num_elems() : 0; }
    void print() {
        std::cout << "Queue(";
        if (_head)
            _head->print();
        std::cout << "), num elems: " << num_elems() << std::endl;
    }
    bool is_empty() { return not _head; }
    bool is_not_empty() { return not is_empty(); }

    bool contains(V value) {
        if (not _head)
            return false;
        return _head->contains(value);
    }

    void remove_el(V value) {
        if (not _head)
            throw PopFromEmptyQueue{};

        if (_head->_value == value)
            _head = std::move(_head->_next);
        else
            _head->remove_el(value);
    }

    class Iterator : public std::iterator<std::forward_iterator_tag, V> {
        queue_node<V> *_current;

       public:
        Iterator(queue_node<V> *current) : _current{current} {};
        V &operator*() const { return _current->_value; }

        // ++it
        Iterator &operator++() {
            _current = _current->_next.get();
            return *this;
        }
        // it++
        Iterator operator++(int) {
            Iterator it{_current};
            ++(*this);
            return it;
        }
        bool operator==(const Iterator &other) { return this->_current == other._current; }
        bool operator!=(const Iterator &other) { return !(*this == other); }
    };

    Iterator begin() { return Iterator{_head.get()}; }
    Iterator end() { return Iterator{nullptr}; }
};

template <typename V>
class node {
    struct ref_node {
        V _adj_index;
        std::unique_ptr<ref_node> _next_adjacent;

        // The weight is used in Dijkstra's algorithm.
        size_t _weight;

        // The heuristic measure is used in the A* algorithm; since we don't have a specific
        // physical problem to solve, we create an heuristic based on the weight, see the
        // constructor below.
        size_t _heuristic;

        ref_node(V index, size_t weight = 0)
            : _adj_index{index},
              _weight{weight},
              _heuristic{weight % 2 == 0 ? weight / 2 : weight + 4} {}

        void add_adjacent(V new_adj_node_value, size_t weight = 0) {
            if (new_adj_node_value == _adj_index)
                return;
            if (_next_adjacent)
                _next_adjacent->add_adjacent(new_adj_node_value, weight);
            else
                _next_adjacent =
                    std::unique_ptr<ref_node>(new ref_node(new_adj_node_value, weight));
        }

        void print() const {
            std::cout << "'" << _adj_index << "', ";
            if (_next_adjacent)
                _next_adjacent->print();
        }

        size_t get_weight(V to_node_val) {
            if (_adj_index == to_node_val)
                return _weight;

            if (_next_adjacent)
                return _next_adjacent->get_weight(to_node_val);
            else
                throw MissingValueException{};
        }

        size_t get_heuristic(V to_node_val) {
            if (_adj_index == to_node_val)
                return _heuristic;

            if (_next_adjacent)
                return _next_adjacent->get_heuristic(to_node_val);
            else
                throw MissingValueException{};
        }

        // Here the logic is: two nodes are equal when the index is the same and, either both do
        // not have any node in the adjacency list or both have the same nodes (checked
        // recursively).
        bool operator==(const ref_node &other) const {
            return _adj_index == other._adj_index and
                   ((not _next_adjacent and not other._next_adjacent) or
                    (_next_adjacent and other._next_adjacent and
                     *_next_adjacent == *other._next_adjacent));
        }
    };

    V _node_index;
    bool discovered;
    std::unique_ptr<node> _next_node;
    // A pointer to the initial node of the adjacency list.
    std::unique_ptr<ref_node> _adjacency_list;
    size_t _adjancent_nodes;

    // We need an id and a lowlink value in the tarjan algorithm.
    size_t tarjan_id, tarjan_lowlink;

    node *find_node(V node_index) {
        if (node_index == _node_index)
            return this;

        if (_next_node)
            return _next_node->find_node(node_index);
        else
            throw MissingValueException{};
    }

    void set_discovered(V node_index) {
        if (node_index == _node_index) {
            discovered = true;
            return;
        }

        if (_next_node)
            _next_node->set_discovered(node_index);
        else
            throw MissingValueException{};
    }

    void reset_discovered() {
        discovered = false;

        if (_next_node)
            _next_node->reset_discovered();
    }

    void reset_id_tarjan() {
        tarjan_id = DEFAULT_UNVISITED;

        if (_next_node)
            _next_node->reset_id_tarjan();
    }

    std::unique_ptr<V[]> DFS_recur(V &value,
                                   std::unique_ptr<V[]> temp,
                                   size_t &number_of_nodes_inserted);

    void DFS_recur_tarjan(node *curr_node, size_t &id, queue<node *> &open_set);

   public:
    void print_node() { std::cout << _node_index; }
    node(V node_index) : _node_index{node_index}, discovered{false}, _adjancent_nodes{0} {};

    void add_node(V node_index) {
        if (node_index == _node_index)
            return;

        if (_next_node)
            _next_node->add_node(node_index);
        else
            _next_node = std::unique_ptr<node>(new node(node_index));
    }

    void add_edge(V src_node, V dst_node, size_t weight = 0) {
        if (src_node == _node_index) {
            // Make sure that the destination node exists when creating the link.
            // add_node(dst_node);
            // Has to be done from the outside because add_node of one of the last nodes in the list
            // does not see all the nodes prior to itself.

            if (not _adjacency_list)
                _adjacency_list = std::unique_ptr<ref_node>(new ref_node(dst_node, weight));
            else
                _adjacency_list->add_adjacent(dst_node, weight);

            _adjancent_nodes++;
            return;
        }

        if (_next_node)
            _next_node->add_edge(src_node, dst_node, weight);
        else
            throw MissingValueException{};
    }

    size_t get_weight(V from_node_val, V to_node_val) {
        auto from_node = find_node(from_node_val);
        return from_node->_adjacency_list->get_weight(to_node_val);
    }

    size_t get_heuristic(V from_node_val, V to_node_val) {
        auto from_node = find_node(from_node_val);
        return from_node->_adjacency_list->get_heuristic(to_node_val);
    }

    void print() const {
        std::cout << "'" << _node_index << "' (discovered: " << std::boolalpha << discovered
                  << ") --> [";
        if (_adjacency_list)
            _adjacency_list->print();
        std::cout << "]; num nodes in list: " << _adjancent_nodes << "\n";

        if (_next_node)
            _next_node->print();
    }

    size_t get_number_of_nodes() {
        if (_next_node)
            return 1 + _next_node->get_number_of_nodes();
        return 1;
    }

    bool operator==(const node &other) const {
        return _node_index == other._node_index and discovered == other.discovered and
               _adjancent_nodes == other._adjancent_nodes and
               ((not _adjacency_list and not other._adjacency_list) or
                (_adjacency_list and other._adjacency_list and
                 *_adjacency_list == *other._adjacency_list));
    }

    class Iterator : public std::iterator<std::forward_iterator_tag, V> {
        ref_node *_current_adj;
        node *_node_iterating_on;

       public:
        Iterator(node *start_node) : _node_iterating_on{start_node} {
            _current_adj = nullptr;
            if (_node_iterating_on and _node_iterating_on->_adjacency_list)
                _current_adj = _node_iterating_on->_adjacency_list.get();
        };
        V &operator*() const { return _current_adj->_adj_index; }

        // ++it
        Iterator &operator++() {
            if (not _node_iterating_on)
                return *this;

            _current_adj = _current_adj->_next_adjacent.get();
            return *this;
        }

        // it++
        Iterator operator++(int) {
            Iterator it{_node_iterating_on};
            ++(*this);
            return it;
        }
        bool operator==(const Iterator &other) {
            return (not _node_iterating_on and not other._node_iterating_on) or
                   (not _current_adj and not other._current_adj) or
                   ((_node_iterating_on and other._node_iterating_on and
                     *_node_iterating_on == *other._node_iterating_on) and
                    (_current_adj and other._current_adj and *_current_adj == *other._current_adj));
        }
        bool operator!=(const Iterator &other) { return not(*this == other); }
    };

    Iterator begin(node *node_to_start) { return Iterator{node_to_start}; }
    Iterator begin(V value_to_start) { return begin(find_node(value_to_start)); }
    Iterator end() { return Iterator{nullptr}; }

    // ------------------------------------------------------------------------
    // -------------------------- Actual algorithms  --------------------------
    // ------------------------------------------------------------------------

    std::pair<std::unique_ptr<V[]>, size_t> DFS(V &start_node);
    std::pair<std::unique_ptr<V[]>, size_t> BFS(V &start_node);
    void tarjan();
    std::pair<std::unique_ptr<size_t[]>, std::unique_ptr<size_t[]>> dijkstra(V &start_node);
    std::pair<std::unique_ptr<size_t[]>, std::unique_ptr<size_t[]>> a_star(V &start_node);
};

#include "03_search_on_graphs.hcc"
#include "04_sccs_tarjan.hcc"
#include "05_dijkstra_a_star.hcc"

#endif
