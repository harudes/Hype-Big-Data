#pragma once

#include <future>
#include <set>
#include <vector>

namespace part {

class Partition
{
public:
    //make partitions move only
    Partition(Partition&&) = default;
    Partition() = delete;
    auto operator=(Partition &&)
        -> Partition& = default;
    auto operator=(const Partition&)
        -> Partition& = delete;

    Partition(std::size_t);

    //add node and its hyperedges to the partition
    auto addNode(int node,
                 const std::vector<int>& edges)
        -> void;

    //same as above
    auto addNode(int node,
                 const std::set<int>& edges)
        -> void;

    //checks if the partitions holds a vertex
    //which is connected to the given edge
    auto hasEdge(int edge) const
        -> bool;
    //return reference to the node set
    auto getNodes() const
        -> const std::set<int>&;
    //same as above
    auto getNodes()
        -> std::set<int>&;

    //return reference to the edge set
    auto getEdges() const
        -> const std::set<int>&;
    //same as above
    auto getEdges()
        -> std::set<int>&;

    //clears edge and node set
    auto clear()
        -> void;

    //returns the id of the partition
    auto getId() const
        -> std::size_t;

    //returns the number of hyperedges in the partition
    auto numberOfEdges() const
        -> std::size_t;

    //return the number of nodes in the partition
    auto numberOfNodes() const
        -> std::size_t;

    //returns a future which will hold the number of external degrees
    //of the partition to the other given partition
    auto externalDegree(const std::vector<Partition>& parts) const
        -> std::future<std::size_t>;

    auto toString() const
        -> std::string;

private:
    std::size_t _id; //id of the partition
    std::set<int> _nodes; //nodes in partition
    std::set<int> _edges; //edges in partition
};

} // namespace part
