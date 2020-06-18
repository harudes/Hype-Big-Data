#pragma once

#include <future>
#include <set>
#include <vector>

namespace part {

class Partition
{
public:

    Partition(Partition&&) = default;
    Partition() = delete;
    auto operator=(Partition &&)
        -> Partition& = default;
    auto operator=(const Partition&)
        -> Partition& = delete;

    Partition(std::size_t);

    auto addNode(int node,
                 const std::vector<int>& edges)
        -> void;

    auto addNode(int node,
                 const std::set<int>& edges)
        -> void;

    auto hasEdge(int edge) const
        -> bool;
        
    auto getNodes() const
        -> const std::set<int>&;
        
    auto getNodes()
        -> std::set<int>&;

    auto getEdges() const
        -> const std::set<int>&;
        
    auto getEdges()
        -> std::set<int>&;

    auto clear()
        -> void;

    auto getId() const
        -> std::size_t;

    auto numberOfEdges() const
        -> std::size_t;

    auto numberOfNodes() const
        -> std::size_t;

    auto externalDegree(const std::vector<Partition>& parts) const
        -> std::future<std::size_t>;

    auto toString() const
        -> std::string;

private:
    std::size_t _id;
    std::set<int> _nodes;
    std::set<int> _edges;
};

} // namespace part
