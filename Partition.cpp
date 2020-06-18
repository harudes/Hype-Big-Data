#include "Partition.hpp"
#include <algorithm>
#include <iterator>
#include <numeric>
#include <string>


part::Partition::Partition(std::size_t id)
    : _id(id) {}

auto part::Partition::getNodes() const
    -> const std::set<int>&
{
    return _nodes;
}

auto part::Partition::getNodes()
    -> std::set<int>&
{
    return _nodes;
}

auto part::Partition::getEdges() const
    -> const std::set<int>&
{
    return _edges;
}

auto part::Partition::getEdges()
    -> std::set<int>&
{
    return _edges;
}

auto part::Partition::getId() const
    -> std::size_t
{
    return _id;
}

auto part::Partition::hasEdge(int elem) const
    -> bool
{
    auto iter = _edges.find(elem);
    return iter != _edges.end();
}

auto part::Partition::addNode(int elem,
                              const std::vector<int>& edges)
    -> void
{
    _nodes.insert(elem);
    _edges.insert(edges.begin(), edges.end());
}

auto part::Partition::addNode(int elem,
                              const std::set<int>& edges)
    -> void
{
    _nodes.insert(elem);
    _edges.insert(edges.begin(), edges.end());
}

auto part::Partition::numberOfNodes() const
    -> std::size_t
{
    return _nodes.size();
}

auto part::Partition::numberOfEdges() const
    -> std::size_t
{
    return _edges.size();
}

auto part::Partition::clear()
    -> void
{
    _nodes.clear();
    _edges.clear();
}

auto part::Partition::externalDegree(const std::vector<Partition>& parts) const
    -> std::future<std::size_t>
{
    return std::async(std::launch::async,
                      [this, &parts]()
                          -> std::size_t {
                          return std::accumulate(_edges.begin(),
                                                 _edges.end(),
                                                 0,
                                                 [&parts, this](auto init, auto&& edge)
                                                     -> std::size_t {
                                                     for(auto&& part : parts) {
                                                         if(part.getId() == getId())
                                                             continue;
                                                         if(part.hasEdge(edge))
                                                             return init + 1;
                                                     }
                                                     return init;
                                                 });
                      });
}


auto part::Partition::toString() const
    -> std::string
{
    return std::accumulate(std::cbegin(_nodes),
                           std::cend(_nodes),
                           "id:" + std::to_string(_id) + "\nnodes:\n",
                           [](auto init, auto node) {
                               auto node_str = std::to_string(node) + "\n";
                               return init + std::move(node_str);
                           });
}
