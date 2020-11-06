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
    Graph(vector<vector<int>> matrix);

    //Not originally in files
    Graph();

    void infectNode(int nodeInd);
    bool isInfected(int nodeInd);

    // We inserted:

    bool finish();
    Status getStatus(int node);
private:
    vector<std::vector<int>> edges;
    // We inserted:
    vector<Status> nodesStatus;
};

#endif
