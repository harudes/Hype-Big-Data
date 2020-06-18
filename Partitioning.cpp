#include "Hypergraph.hpp"
#include "Partition.hpp"
#include "Partitioning.hpp"
#include "SSet.hpp"
#include <algorithm>
#include <iostream>
#include <numeric>


auto part::partitionGraph(Hypergraph& graph,
                          std::size_t number_of_partitions,
                          std::size_t s_set_size,
                          std::size_t s_set_candidates,
                          double ignore_biggest_edges_in_percent,
                          NodeHeuristicMode num_neigs_flag,
                          NodeSelectionMode node_select_flag)
    -> std::vector<Partition>{
    const auto delta = graph.getVertices().size() / number_of_partitions; // truncated
    const auto padded_partitions = graph.getVertices().size() - number_of_partitions*delta;

    auto is_partition_full = [&delta, &padded_partitions](std::size_t index, auto&& partition) {
        if (index < padded_partitions) {
            return partition.numberOfNodes() >= delta + 1; 
        } else {
            return partition.numberOfNodes() >= delta;
        } 
    };

    const auto max_edge_size =
        graph.getEdgesizeOfPercentBiggestEdge(ignore_biggest_edges_in_percent);

    std::vector<Partition> part_vec;

    for(std::size_t i = 0; i < number_of_partitions; ++i) {
        Partition part{static_cast<size_t>(i)};

        SSet s_set{graph,
                   s_set_size,
                   num_neigs_flag,
                   node_select_flag};

        while(!is_partition_full(i, part)
              && !graph.getVertices().empty()) {

            auto next_node = s_set.getNextNode();

            part.addNode(next_node,
                         graph.getVertexEdges(next_node));

            s_set.removeNode(next_node);

            auto add_to_s = graph.getSSetCandidates(next_node,
                                                    s_set_candidates,
                                                    max_edge_size);

            graph.deleteVertex(next_node);

            s_set.addNodes(std::move(add_to_s));
        }

        part_vec.push_back(std::move(part));
    }

    return part_vec;
}


auto part::getSumOfExteralDegrees(const std::vector<Partition>& partitions)
    -> std::future<std::size_t>
{
    return std::async(std::launch::async,
                      [&partitions]() -> std::size_t {
                          std::vector<std::future<std::size_t>> fut_vec;
                          for(auto&& part : partitions) {
                              fut_vec.emplace_back(part.externalDegree(partitions));
                          }
                          return std::accumulate(std::begin(fut_vec),
                                                 std::end(fut_vec),
                                                 0,
                                                 [](auto init, auto&& fut) {
                                                     return init + fut.get();
                                                 });
                      });
}


auto part::getHyperedgeCut(const std::vector<Partition>& partitions)
    -> std::future<std::size_t>
{
    return std::async(std::launch::async,
                      [&partitions]() -> std::size_t {
                          std::set<int> edges_cut;
                          for(auto&& part : partitions) {
                              const auto& edges = part.getEdges();

                              for(auto&& edge : edges) {
                                  for(auto&& nested_part : partitions) {
                                      if(part.getId() == nested_part.getId()) {
                                          continue;
                                      }

                                      if(nested_part.hasEdge(edge)) {
                                          edges_cut.insert(edge);
                                      }
                                  }
                              }
                          }

                          return edges_cut.size();
                      });
}


auto part::getEdgeBalancing(const std::vector<Partition>& partitions)
    -> std::future<double>
{
    return std::async(std::launch::async,
                      [&partitions]() -> double {
                          auto minmax =
                              std::minmax_element(std::cbegin(partitions),
                                                  std::cend(partitions),
                                                  [](auto&& lhs, auto&& rhs) {
                                                      return lhs.numberOfEdges() < rhs.numberOfEdges();
                                                  });

                          auto biggest = minmax.second->numberOfEdges();
                          auto smallest = minmax.first->numberOfEdges();

                          return (biggest - smallest)
                              / static_cast<double>(biggest);
                      });
}

auto part::getVertexBalancing(const std::vector<Partition>& partitions)
    -> std::future<double>
{
    return std::async(std::launch::async,
                      [&partitions]() -> double {
                          auto minmax = std::minmax_element(std::cbegin(partitions),
                                                  std::cend(partitions),
                                                  [](auto&& lhs, auto&& rhs) {
                                                      return lhs.numberOfNodes() < rhs.numberOfNodes();
                                                  });

                          auto biggest = minmax.second->numberOfNodes();
                          auto smallest = minmax.first->numberOfNodes();

                          return (biggest - smallest)
                              / static_cast<double>(biggest);
                      });
}


auto part::getKminus1Metric(const std::vector<Partition>& partitions,
                            std::size_t edges_in_graph)
    -> std::future<std::size_t>
{
    return std::async(std::launch::async,
                      [&partitions, edges_in_graph]()
                          -> std::size_t {
                          return std::accumulate(std::cbegin(partitions),
                                                 std::cend(partitions),
                                                 0,
                                                 [](auto init, auto&& part) {
                                                     return init + part.getEdges().size();
                                                 })
                              - edges_in_graph;
                      });
}
