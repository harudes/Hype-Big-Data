#include <iostream>
#include "Partitioning.hpp"
#include <algorithm>
#include <time.h>

using namespace std;

void parse(Hypergraph &graph){
    int vertexNum, edgeNum, tempV, tempE;
    cin>>vertexNum>>edgeNum;
    for(int i=0; i<edgeNum; ++i){
        cin>>tempV>>tempE;
        graph.connect(tempV,tempE);
    }
    cout<<endl<<endl;
}

int main(int argc, char* argv[]){
    srand(time(NULL));
    Hypergraph grafo;
    parse(grafo);
    auto vertices = grafo.getEdgeVertices(6);
    for(auto it = vertices.begin(); it != vertices.end(); ++it){
        cout<<*it<<endl;
    }
    return 0;
}