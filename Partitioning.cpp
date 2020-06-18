#include "Hypergraph.hpp"
#include "Partition.hpp"
#include "Partitioning.hpp"
#include "SSet.hpp"
#include <algorithm>
#include <iostream>
#include <numeric>


auto part::partitionGraph(Hypergraph& graph, std::size_t numberOfPartitions, std::size_t sSetSize, std::size_t sSetCandidates, double ignoreBiggestEdgesInPercent, NodeHeuristicMode numNeigsFlag, NodeSelectionMode nodeSelectFlag) -> std::vector<Partition>{
    const std::size_t delta = graph.getVertices().size() / numberOfPartitions;
    const std::size_t paddedPartitions = graph.getVertices().size() - numberOfPartitions*delta;
    auto isPartitionFull = [&delta, &paddedPartitions](std::size_t index, auto&& partition) {
        if (index < paddedPartitions) {
            return partition.numberOfNodes() >= delta + 1; 
        } else {
            return partition.numberOfNodes() >= delta;
        } 
    };
    const auto maxEdgeSize = graph.getEdgesizeOfPercentBiggestEdge(ignoreBiggestEdgesInPercent);
    std::vector<Partition> partVec;
    for(std::size_t i = 0; i < numberOfPartitions; ++i) {
        Partition part{static_cast<size_t>(i)};
        SSet sSet{graph, sSetSize, numNeigsFlag, nodeSelectFlag};
        while(!isPartitionFull(i, part) && !graph.getVertices().empty()) {
            auto nextNode = sSet.getNextNode();
            part.addNode(nextNode, graph.getVertexEdges(nextNode));
            sSet.removeNode(nextNode);
            auto addToS = graph.getSSetCandidates(nextNode, sSetCandidates, maxEdgeSize);
            graph.deleteVertex(nextNode);
            sSet.addNodes(std::move(addToS));
        }
        partVec.push_back(std::move(part));
    }
    return partVec;
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
