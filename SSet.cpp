#include "SSet.hpp"
#include <algorithm>
#include <optional>


auto part::operator>>(std::istream& in, part::NodeHeuristicMode& num)
    -> std::istream&
{
    std::string token;
    in >> token;
    if(token == "exact")
        num = part::NodeHeuristicMode::Exact;
    else if(token == "cached")
        num = part::NodeHeuristicMode::Cached;
    else
        in.setstate(std::ios_base::failbit);
    return in;
}

auto part::operator<<(std::ostream& os, const part::NodeHeuristicMode& num)
    -> std::ostream&
{
    switch(num) {
    case part::NodeHeuristicMode::Exact:
        os << "exact";
        break;
    case part::NodeHeuristicMode::Cached:
        os << "cached";
        break;
    default:
        os.setstate(std::ios_base::failbit);
        break;
    }

    return os;
}

auto part::operator>>(std::istream& in, part::NodeSelectionMode& num)
    -> std::istream&
{
    std::string token;
    in >> token;
    if(token == "truly-random")
        num = part::NodeSelectionMode::TrulyRandom;
    else if(token == "next-best")
        num = part::NodeSelectionMode::NextBest;
    else
        in.setstate(std::ios_base::failbit);
    return in;
}

auto part::operator<<(std::ostream& os, const part::NodeSelectionMode& num)
    -> std::ostream&
{
    switch(num) {
    case part::NodeSelectionMode::TrulyRandom:
        os << "truly-random";
        break;
    case part::NodeSelectionMode::NextBest:
        os << "next-best";
        break;
    default:
        os.setstate(std::ios_base::failbit);
        break;
    }

    return os;
}

auto part::SSet::addNodes(const std::set<int>& nodes_to_add)
    -> void{
    std::vector<std::pair<int, std::size_t>> nodes_to_add_neig_vec;
    for(auto&& node : nodes_to_add) {
        auto neigs = getNodeHeuristic(node);
        nodes_to_add_neig_vec.emplace_back(node, neigs);
    }

    std::sort(std::begin(nodes_to_add_neig_vec),
              std::end(nodes_to_add_neig_vec),
              [](auto lhs, auto rhs) { return lhs.second < rhs.second; });

    std::vector<std::pair<int, std::size_t>> nodes_neig_vec;
    for(auto&& node : _nodes) {
        auto neigs = getNodeHeuristic(node);
        nodes_neig_vec.emplace_back(node, neigs);
    }

    std::sort(std::begin(nodes_neig_vec),
              std::end(nodes_neig_vec),
              [](auto lhs, auto rhs) { return lhs.second < rhs.second; });

    std::vector<std::pair<int, std::size_t>> merged_vec;
    std::merge(std::begin(nodes_neig_vec),
               std::end(nodes_neig_vec),
               std::begin(nodes_to_add_neig_vec),
               std::end(nodes_to_add_neig_vec),
               std::back_inserter(merged_vec),
               [](auto&& lhs, auto&& rhs) { return lhs.second < rhs.second; });

    _nodes.clear();

    auto range = std::min(merged_vec.size(), _max_size);
    for(std::size_t i{0}; i < range; ++i) {
        _nodes.insert(merged_vec[i].first);
    }
}

auto part::SSet::getMinElement() const
    -> std::optional<int>
{
    std::optional<std::size_t> neigs;
    std::optional<int> min_node;

    for(auto&& node : _nodes) {
        auto num_of_neigs = getNodeHeuristic(node);
        if(num_of_neigs <= 1) {
            return node;
        } else if(!neigs || neigs.value() > num_of_neigs) {
            neigs = num_of_neigs;
            min_node = node;
        }
    }

    return min_node;
}

auto part::SSet::getNextNode() const
    -> int
{
    auto min_node_opt = getMinElement();
    if(min_node_opt) {
        return min_node_opt.value();
    }

    return selectANode();
}

auto part::SSet::removeNode(const int& node)
    -> void
{
    auto iter = _nodes.begin();
    for(; iter != _nodes.end(); iter++) {
        if(*iter == node)
            break;
    }
    if(iter != _nodes.end()) {
        _nodes.erase(iter);
    }
}


auto part::SSet::selectANode() const
    -> int
{
    switch(_node_select_flag) {
    case NodeSelectionMode::TrulyRandom:
        return _graph.getRandomNode();
    default:
        return _graph.getAnyNode();
    }
}

auto part::SSet::getNodeHeuristic(int vtx) const
    -> std::size_t
{
    switch(_numb_of_neigs_flag) {
    case NodeHeuristicMode::Exact:
        return _graph.getNodeHeuristicExactly(vtx);
    default:
        return _graph.getNodeHeuristicEstimate(vtx);
    }
}
