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
    Session();
    void simulate();
    void addAgent(const Agent& agent);
    void setGraph(const Graph& graph);
    //Rule of 5:
    virtual ~Session();
    Session(const Session &otherSess);
    Session & operator=(const Session &otherSess);
    Session& operator=(Session&& other);
    Session(Session&& other);

    void enqueueInfected(int);
    int dequeueInfected();
    TreeType getTreeType() const;

    Graph& getGraph();
    int getCycle() const;
    void outputConfig();

private:
    Graph g;
    TreeType treeType;
    std::vector<Agent*> agents;
    vector<int> InfectedQueue;
    int cycle;
};

#endif
