#include "Hypergraph.hpp"
#include <algorithm>
#include <numeric>

template<class From, class To>
auto addAtMostN(From from,
                std::size_t upto,
                To to,
                int v){
    auto end = from.begin();
    if(to.size() + from.size() > upto)
        std::advance(end, upto - to.size());
    else
        end = from.end();
    auto iter = from.begin();
    while(iter != end) {
        if(*iter != v) {
            to.insert(*iter);
        }
        iter++;
    }
    return std::move(to);
}

auto HyperGraph::addVertex(int v){
    auto it = vertices.find(v);
    if(it != vertices.end()){
        return make_pair(it,false);
    }
    else{
        return vertices.insert(make_pair(v,std::set<int>()));
    }
}

auto HyperGraph::addEdge(int e){
    auto it = edges.find(e);
    if(it != vertices.end()){
        return make_pair(it,false);
    }
    else{
        return vertices.insert(make_pair(e,std::set<int>()));
    }
}

void HyperGraph::addEdgeList(int v, std::vector<int> &edgeList){
    for(size_t i=0; i<edgeList.size(); ++i){
        connect(v,edgeList[i]);
    }
}

void HyperGraph::addVertexList(int e, std::vector<int> &vertexList){
    for(size_t i=0; i<vertexList.size(); ++i){
        connect(e,vertexList[i]);
    }
}

void HyperGraph::connect(int v, int e){
    auto vertexIt = addVertex(v).first;
    auto edgeIt = addEdge(e).first;

    vertexIt->second.insert(v);
    edgeIt->second.insert(e);
}

std::size_t HyperGraph::getEdgesizeOfPercentBiggestEdge(double percent){
    
}

auto HyperGraph::getEdges(){
    return edges;
}

auto HyperGraph::getVertices(){
    return vertices;
}

void HyperGraph::deleteVertex(int v){
    auto it = vertices.find(v);
    if(it != vertices.end()){
        for(auto edge:it->second){
            auto edgeIt = edges.find(edge);
            if(edgeIt != edges.end()){
                edgeIt->second.erase(v);
                if(edgeIt->second.empty()){
                    edges.erase(edgeIt);
                }
            }
        }
        neighbourMap.erase(v);
        vertices.erase(v);
    }
}

auto HyperGraph::getVertexEdges(int v){
    auto it = vertices.find(v);
    if(it != vertices.end()){
        return it->second;
    }
    else{
        return std::set<int>();
    }
}

auto HyperGraph::getEdgeVertices(int e){
    auto it = edges.find(e);
    if(it != edges.end()){
        return it->second;
    }
    else{
        return std::set<int>();
    }
}

auto HyperGraph::getSSetCandidates(int v, std::size_t n, std::size_t maxEdgeSize){
    auto edges = getVertexEdges(v);
    std::set<int> neighboors;
    std::size_t currentMax = 2;

    while(currentMax < maxEdgeSize && neighboors.size() <= n){
        for(auto edge:edges){
            auto vertices = getEdgeVertices(edge);
            if(vertices.size() > currentMax)
                continue;
            neighboors = addAtMostN(vertices, n, std::move(neighboors), v);
            if(neighboors.size() >=n)
                return neighboors;
        }
        currentMax*=2;
    }
    return neighboors;
}

double HyperGraph::getNodeHeuristicExactly(const int v){
    auto edges = getVertexEdges(v);

    if(edges.empty())
        return 0;
    
    return std::accumulate(std::begin(edges), std::end(edges), 0, [this](auto init, auto edge) { return init + getVerticesOf(edge).size() - 1;})/ edges.size();
}

double HyperGraph::getNodeHeuristicEstimate(int v){

}

int HyperGraph::getRandomNode(){

}

int HyperGraph::getAnyNode(){

}