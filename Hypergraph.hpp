#include <map>
#include <set>

class HyperGraph{
    std::map<int,std::set<int> > edges;
    std::map<int,std::set<int> > vertex;
    std::map<int,double> neighbourMap;
public:
    HyperGraph() = default;
    void addVertex();
    void addEdge();
    void addEdgeList();
    void addNodeList();
    void getEdgesizeOfPercentBiggestEdge();
    void connect(int v, int e);
    auto getEdges();
    auto getVertices();
    void deleteVertex();
    auto getEdgesOf();
    auto getVerticesOf();
    auto getSSetCandidates();
    auto getNodeHeuristicExactly();
    auto getNodeHeuristicEstimate();
    auto getRandomNode();
    auto getANode();
};