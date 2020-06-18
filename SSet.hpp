#pragma once

#include "Hypergraph.hpp"
#include <iostream>

namespace part {

enum class NodeHeuristicMode {
    Cached,
    Exact
};

enum class NodeSelectionMode {
    TrulyRandom,
    NextBest
};

auto operator>>(std::istream& in, part::NodeHeuristicMode& num)
    -> std::istream&;
auto operator<<(std::ostream& os, const part::NodeHeuristicMode& num)
    -> std::ostream&;
auto operator>>(std::istream& in, part::NodeSelectionMode& num)
    -> std::istream&;
auto operator<<(std::ostream& os, const part::NodeSelectionMode& num)
    -> std::ostream&;

class SSet
{
public:
    SSet(Hypergraph& graph,
         std::size_t max_size,
         NodeHeuristicMode numb_of_neigs_flag,
         NodeSelectionMode node_select_flag)
        : _graph(graph),
          _max_size(max_size),
          _numb_of_neigs_flag(numb_of_neigs_flag),
          _node_select_flag(node_select_flag) {}

    auto addNodes(const std::set<int>& nodes_to_add)
        -> void;

    int getMinElement();

    int getNextNode();

    auto removeNode(const int& node)
        -> void;

private:
    auto getNodeHeuristic(int vtx) const
        -> std::size_t;

    auto selectANode() const
        -> int;

private:
    std::set<int> _nodes;
    Hypergraph& _graph;
    std::size_t _max_size;
    NodeHeuristicMode _numb_of_neigs_flag;
    NodeSelectionMode _node_select_flag;
};

} // namespace part
