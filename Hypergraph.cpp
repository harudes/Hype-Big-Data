#include "Hypergraph.hpp"
#include <algorithm>
#include <numeric>
#include <stdlib.h>

Hypergraph::Hypergraph(){

}

template<class From, class To, class Predicate>
auto addAtMostN(From&& from, std::size_t upto, To&& to, Predicate&& pred){
    auto end = from.begin();
    if(to.size() + from.size() > upto)
        std::advance(end, upto - to.size());
    else
        end = from.end();
    auto iter = from.begin();
    while(iter != end) {
        if(pred(*iter)) {
            to.insert(*iter);
        }
        iter++;
    }
    return std::move(to);
}

auto Hypergraph::addVertex(int v){
    auto it = vertices.find(v);
    if(it != vertices.end()){
        return make_pair(it,false);
    }
    else{
        return vertices.insert(make_pair(v,std::set<int>()));
    }
}

auto Hypergraph::addEdge(int e){
    auto it = edges.find(e);
    if(it != edges.end()){
        return make_pair(it,false);
    }
    else{
        return edges.insert(make_pair(e,std::set<int>()));
    }
}

void Hypergraph::addEdgeList(int v, std::vector<int> &edgeList){
    for(size_t i=0; i<edgeList.size(); ++i){
        connect(v,edgeList[i]);
    }
}

void Hypergraph::addVertexList(int e, std::vector<int> &vertexList){
    for(size_t i=0; i<vertexList.size(); ++i){
        connect(e,vertexList[i]);
    }
}

void Hypergraph::connect(int v, int e){
    auto vertexIt = addVertex(v).first;
    auto edgeIt = addEdge(e).first;

    vertexIt->second.insert(e);
    edgeIt->second.insert(v);
}

std::size_t Hypergraph::getEdgesizeOfPercentBiggestEdge(double percent){
    double factor = 1 - percent / 100;
    std::vector<std::size_t> sizeVec;
    for(std::map<int,std::set<int> >::iterator it = edges.begin(); it != edges.end(); ++it){
        sizeVec.push_back(it->second.size());
    }

    std::nth_element(sizeVec.begin(), sizeVec.begin() + (sizeVec.size() - 1) * factor, sizeVec.end());

    return sizeVec[(sizeVec.size() - 1) * factor];
}

std::map<int,std::set<int> > Hypergraph::getEdges(){
    return edges;
}

std::map<int,std::set<int> > Hypergraph::getVertices(){
    return vertices;
}

void Hypergraph::deleteVertex(int v){
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

std::set<int> Hypergraph::getVertexEdges(int v){
    auto it = vertices.find(v);
    if(it != vertices.end()){
        return it->second;
    }
    else{
        return std::set<int>();
    }
}

std::set<int> Hypergraph::getEdgeVertices(int e){
    auto it = edges.find(e);
    if(it != edges.end()){
        return it->second;
    }
    else{
        return std::set<int>();
    }
}

auto Hypergraph::getSSetCandidates(int v, std::size_t n, std::size_t maxEdgeSize){
    auto edges = getVertexEdges(v);
    std::set<int> neighboors;
    std::size_t currentMax = 2;

    while(currentMax < maxEdgeSize && neighboors.size() <= n){
        for(auto edge:edges){
            auto vertices = getEdgeVertices(edge);
            if(vertices.size() > currentMax)
                continue;
            neighboors = addAtMostN(vertices, n, std::move(neighboors), [&v](auto&& elem) { return elem != v;});
            if(neighboors.size() >=n)
                return neighboors;
        }
        currentMax*=2;
    }
    return neighboors;
}

double Hypergraph::getNodeHeuristicExactly(const int v){
    auto edges = getVertexEdges(v);

    if(edges.empty())
        return 0;
    
    return std::accumulate(std::begin(edges), std::end(edges), std::size_t{0}, [this](auto init, auto edge) {return init + this->getEdgeVertices(edge).size() - 1;})/ edges.size();
}

double Hypergraph::getNodeHeuristicEstimate(int v){
    auto iter = neighbourMap.find(v);
    if(iter != neighbourMap.end()) {
        return iter->second;
    }

    double neigs = getNodeHeuristicExactly(v);

    neighbourMap.insert({v, neigs});
    return neigs;
}

int Hypergraph::getRandomNode(){
    int v = rand()% vertices.size();

    auto it = std::begin(vertices);
    std::advance(it, v);

    return it->first;
}

int Hypergraph::getAnyNode(){

}