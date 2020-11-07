#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <string>

using namespace std;

enum Status{
    H,          //Healthy
    C,          //Carrying
    I           //Infected.             *Check wheather "infected" means sick, or carrying.
};

class Graph{
public:
    Graph(std::vector<std::vector<int>> matrix);

    //Not originally in files
    Graph();

    void infectNode(int nodeInd);
    bool isInfected(int nodeInd);

    // We inserted:
    bool finish();

    Status getStatus(int node);
    void setStatus(int node, Status sts);

    int closestNode(int node);
private:
    std::vector<std::vector<int>> edges;
    // We inserted:
    vector<Status> nodesStatus;
};

#endif
