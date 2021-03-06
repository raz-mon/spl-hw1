#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <string>

using namespace std;

enum Status{
    H,          //Healthy
    C,          //Carrying
    I           //Infected
};

class Graph{
public:
    Graph(std::vector<std::vector<int>> matrix);
    Graph();

    void infectNode(int nodeInd);
    bool isInfected(int nodeInd);

    bool finish();
    Status getStatus(int node);
    void setStatus(int node, Status sts);

    int closestNode(int node);
    bool hasEdge(int i, int j);
    void isolate(int toIsolate);
    int getSize();
    vector<vector<int>> getEdges() const;
    vector<Status> getNodesStatus() const;
private:
    std::vector<std::vector<int>> edges;
    vector<Status> nodesStatus;
};

#endif
