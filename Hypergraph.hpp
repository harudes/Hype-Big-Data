#include <map>
#include <set>
#include <vector>

class HyperGraph{
    std::map<int,std::set<int> > edges;
    std::map<int,std::set<int> > vertices;
    std::map<int,double> neighbourMap;
public:
    HyperGraph() = default;
    auto addVertex(int v);
    auto addEdge(int e);
    void addEdgeList(int v, std::vector<int> &edgeList);
    void addVertexList(int e, std::vector<int> &vertexList);
    void getEdgesizeOfPercentBiggestEdge(double percent);
    void connect(int v, int e);
    auto getEdges();
    auto getVertices();
    void deleteVertex(int v);
    auto getVertexEdges(int v);
    auto getEdgeVertices(int e);
    auto getSSetCandidates(int v, std::size_t n, std::size_t maxEdgeSize);
    auto getNodeHeuristicExactly(int v);
    auto getNodeHeuristicEstimate(int v);
    auto getRandomNode();
    auto getAnyNode();
};