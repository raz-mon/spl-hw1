#include "../include/Session.h"
#include "../include/json.hpp"
#include <fstream>
#include <iostream>
#include "../include/Agent.h"
#include <string>
#include <vector>

using json = nlohmann::json;
using namespace std;

Session::Session():g(),treeType(Root), agents(),InfectedQueue(),cycle(){}       // Empty CTR

Session::Session(const std::string& path):g(),treeType(),agents(),InfectedQueue(),cycle(0){     // Regular CTR
    ifstream i(path);
    json j;
    i >> j;
    //Initializing treeType according to given treeType in json file
    if (j["tree"] == "M"){
        this->treeType  = MaxRank;
    }
    else if (j["tree"] == "C"){
        this->treeType  = Cycle;
    }
    else{
        treeType  = Root;
    }

    //Initializing Graph
    g = Graph(j["graph"]);

    //Initializing Agents
    for(uint m=0; m<j["agents"].size(); ++m){
        if(j["agents"][m][0] == "V"){
            Virus *v = new Virus(j["agents"][m][1], *this);
            agents.push_back(v);
            g.setStatus(j["agents"][m][1], C);
        }
        else {
            ContactTracer *ct = new ContactTracer(*this);
            agents.push_back(ct);
        }
    }
}

//copy constructor
Session::Session(const Session &otherSess): g(otherSess.g),
        treeType(otherSess.treeType), agents(), InfectedQueue(otherSess.InfectedQueue),cycle(0){
    for (uint i=0; i<agents.size(); ++i){
        this->agents.push_back(otherSess.agents[i]->clone());
    }
}

// copy assignment operator
Session & Session::operator=(const Session &otherSess){
    if (this == &otherSess)
        return *this;
    this->g = otherSess.g;
    this->treeType = otherSess.treeType;
    this->InfectedQueue = otherSess.InfectedQueue;
    this->cycle = otherSess.cycle;
    while (!agents.empty()){       // delete allocated memory by this's agents.
        delete(agents[0]);
        agents.erase(agents.begin());
    }
    for (uint i=0; i<otherSess.agents.size(); ++i){     // assign otherSess's agents to this's agents (but as new instances!).
        this->agents.push_back(otherSess.agents[i]->clone());
    }
    return *this;
}

//move assignment operator:
Session& Session::operator=(Session&& other){
    if (this == &other)
        return *this;
    this->g = other.g;
    this->cycle = other.cycle;
    this->InfectedQueue = other.InfectedQueue;
    this->treeType = other.treeType;
    while (!this->agents.empty()){      //free all memory alocated by this, and empty this->agents before assignment.
        delete(this->agents[0]);
        agents.erase(agents.begin());
    }
    for (uint i=0; i<other.agents.size();++i){      //"steal" other's memory and move it's pointers to null.
        agents.push_back(other.agents[i]);
        other.agents[i] = nullptr;
    }
    return *this;
}

//move CTR
Session::Session(Session&& other): g(other.g),treeType(other.treeType),      //beware of memory leaks due to the vector copy here!!
                                   agents(other.agents), InfectedQueue(other.InfectedQueue),cycle(other.cycle){
    for(uint i=0; i<other.agents.size();++i){
        other.agents[i] = nullptr;
    }
}

// Destructor
[[noreturn]] Session::~Session(){
    for (uint i=0; i<agents.size(); ++i){
        delete(agents[i]);
    }
}

void Session::simulate(){
    while (!g.finish()){
        int currentSize = agents.size();
        for(int i = 0; i < currentSize; i++){       // run only through agents that where created prior to this cycle.
            agents[i]->act();                       // relevant agent acts.
        }
        ++cycle;
    }
}

TreeType Session::getTreeType() const{
    return this->treeType;
}


Graph& Session::getGraph(){
    return g;
}

void Session::enqueueInfected(int node){
    this->InfectedQueue.push_back(node);        // Push node to the infectedQueue vector (That acts as a queue).
}

int Session::dequeueInfected(){
    if(InfectedQueue.empty())       //Queue is empty
        return -1;
    int infectedNode = InfectedQueue.front();            //Save the first element
    InfectedQueue.erase(InfectedQueue.cbegin()); //Erase the first element
    return infectedNode;
}

void Session::addAgent(const Agent& agent){
    agents.push_back(agent.clone());
}

int Session::getCycle() const{
    return this->cycle;
}

void Session::outputConfig(){
    json j;
    ofstream o("./output.json");
    j["graph"];                         // Create object graph, which will hold the final graph.
    j["infected"];                      //Create object infected, which will hold all infected nodes.
    for (int i=0; i<g.getSize(); ++i){
        j["graph"][i] = g.getEdges()[i];
    }
    vector<int> infectedGuys;
    for(uint i=0; i<g.getNodesStatus().size(); ++i){        // fill infected (vector) with infected nodes.
        if (g.getNodesStatus()[i]!=H){
            infectedGuys.push_back(i);
        }
    }
    j["infected"] = infectedGuys;
    o << j << endl;
}







