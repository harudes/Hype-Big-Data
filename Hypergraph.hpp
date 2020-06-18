#pragma once

#include <map>
#include <set>
#include <vector>

class Hypergraph{
    std::map<int,std::set<int> > edges;
    std::map<int,std::set<int> > vertices;
    std::map<int,double> neighbourMap;
public:
    Hypergraph();
    auto addVertex(int v);
    auto addEdge(int e);
    void addEdgeList(int v, std::vector<int> &edgeList);
    void addVertexList(int e, std::vector<int> &vertexList);
    std::size_t getEdgesizeOfPercentBiggestEdge(double percent);
    void connect(int v, int e);
    auto getEdges();
    auto getVertices();
    void deleteVertex(int v);
    std::set<int> getVertexEdges(int v);
    auto getEdgeVertices(int e);
    auto getSSetCandidates(int v, std::size_t n, std::size_t maxEdgeSize);
    double getNodeHeuristicExactly(const int v);
    double getNodeHeuristicEstimate(int v);
    int getRandomNode();
    int getAnyNode();
};