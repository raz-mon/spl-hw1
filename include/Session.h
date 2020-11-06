#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <string>
#include "Graph.h"

using namespace std;

class Agent;

enum TreeType{
  Cycle,
  MaxRank,
  Root
};

class Session{
public:
    Session(const std::string& path);

    //not in original
    Session();

    void simulate();
    void addAgent(const Agent& agent);
    void setGraph(const Graph& graph);
    
    void enqueueInfected(int);
    int dequeueInfected();
    TreeType getTreeType() const;

    //Almog adds-on: new functions
    Graph& getGraph();

    ~Session(){}

private:
    Graph g;
    TreeType treeType;
    std::vector<Agent*> agents;

};

#endif
