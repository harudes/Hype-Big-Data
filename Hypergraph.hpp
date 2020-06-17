#include <map>
#include <set>

class HyperGraph{
    std::map<int,std::set<int> > edges;
    std::map<int,std::set<int> > vertex;
public:
    HyperGraph() = default;
};