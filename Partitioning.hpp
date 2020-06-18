#pragma once

#include "Partition.hpp"
#include "SSet.hpp"

namespace part {
    
auto partitionGraph(Hypergraph& graph,
                    std::size_t number_of_partitions,
                    std::size_t s_set_size,
                    std::size_t s_set_candidates,
                    double ignore_biggest_edges_in_percent,
                    NodeHeuristicMode num_neigs_flag,
                    NodeSelectionMode node_select_flag)
    -> std::vector<Partition>;
    
auto getSumOfExteralDegrees(const std::vector<Partition>& partitions)
    -> std::future<std::size_t>;

auto getHyperedgeCut(const std::vector<Partition>& partitions)
    -> std::future<std::size_t>;

auto getEdgeBalancing(const std::vector<Partition>& partitions)
    -> std::future<double>;

auto getVertexBalancing(const std::vector<Partition>& partitions)
    -> std::future<double>;

auto getKminus1Metric(const std::vector<Partition>& partitions,
                      std::size_t edges_in_graph)
    -> std::future<std::size_t>;

}
