#include <map>
#include <set>
#include <vector>

class HyperGraph{
    std::map<int,std::set<int> > edges;
    std::map<int,std::set<int> > vertex;
    std::map<int,double> neighbourMap;
public:
    HyperGraph() = default;
    void addVertex(int v);
    void addEdge(int e);
    void addEdgeList(int v, std::vector<int> edgeList);
    void addVertexList(int e, std::vector<int> vertexList);
    void getEdgesizeOfPercentBiggestEdge(double percent);
    void connect(int v, int e);
    auto getEdges();
    auto getVertices();
    void deleteVertex(int v);
    auto getEdgesOf(int v);
    auto getVerticesOf(int e);
    auto getSSetCandidates(int v, int n, int ignoreEdges);
    auto getNodeHeuristicExactly(int v);
    auto getNodeHeuristicEstimate(int v);
    auto getRandomNode();
    auto getAnyNode();
};